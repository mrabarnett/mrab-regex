/* Secret Labs' Regular Expression Engine
 *
 * regular expression matching engine
 *
 * partial history:
 * 1999-10-24 fl   created (based on existing template matcher code)
 * 2000-03-06 fl   first alpha, sort of
 * 2000-08-01 fl   fixes for 1.6b1
 * 2000-08-07 fl   use PyOS_CheckStack() if available
 * 2000-09-20 fl   added expand method
 * 2001-03-20 fl   lots of fixes for 2.1b2
 * 2001-04-15 fl   export copyright as Python attribute, not global
 * 2001-04-28 fl   added __copy__ methods (work in progress)
 * 2001-05-14 fl   fixes for 1.5.2 compatibility
 * 2001-07-01 fl   added BIGCHARSET support (from Martin von Loewis)
 * 2001-10-18 fl   fixed group reset issue (from Matthew Mueller)
 * 2001-10-20 fl   added split primitive; reenable unicode for 1.6/2.0/2.1
 * 2001-10-21 fl   added sub/subn primitive
 * 2001-10-24 fl   added finditer primitive (for 2.2 only)
 * 2001-12-07 fl   fixed memory leak in sub/subn (Guido van Rossum)
 * 2002-11-09 fl   fixed empty sub/subn return type
 * 2003-04-18 mvl  fully support 4-byte codes
 * 2003-10-17 gn   implemented non recursive scheme
 * 2009-07-26 mrab completely re-designed matcher code
 *
 * Copyright (c) 1997-2001 by Secret Labs AB.  All rights reserved.
 *
 * This version of the SRE library can be redistributed under CNRI's
 * Python 1.6 license.  For any other use, please contact Secret Labs
 * AB (info@pythonware.com).
 *
 * Portions of this engine have been developed in cooperation with
 * CNRI.  Hewlett-Packard provided funding for 1.6 integration and
 * other compatibility work.
 */

/* #define VERBOSE */

#if defined(VERBOSE)
#define TRACE(X) printf X;
#else
#define TRACE(X)
#endif

#define RE_MULTITHREADED

#include "Python.h"
#include "structmember.h" /* offsetof */
#include <ctype.h>
#include "_regex.h"
#include "pyport.h"

typedef enum {FALSE, TRUE} BOOL;

/* Name of this module, minus the leading underscore. */
#define RE_MODULE "regex"
#define RE_MODULE_UPPER "REGEX"

/* Release the GIL when matching on a immutable string. */
#define RE_MULTITHREADED

/* Error codes. */
#define RE_ERROR_SUCCESS 1 /* Successful match. */
#define RE_ERROR_FAILURE 0 /* Unsuccessful match. */
#define RE_ERROR_ILLEGAL -1 /* Illegal opcode. */
#define RE_ERROR_MEMORY -9 /* Out of memory. */
#define RE_ERROR_INTERRUPTED -10 /* Signal handler raised exception. */
#define RE_ERROR_REPLACEMENT -11 /* Invalid replacement string. */
#define RE_ERROR_INVALID_GROUP_REF -12 /* Invalid group reference. */
#define RE_ERROR_GROUP_INDEX_TYPE -13 /* Group index type error. */
#define RE_ERROR_NO_SUCH_GROUP -14 /* No such group. */
#define RE_ERROR_INDEX -15 /* String index. */

/* Number of backtrack entries per allocated block. */
#define RE_BACKTRACK_BLOCK_SIZE 64

#define RE_BITS_PER_INDEX 16
#define RE_BITS_PER_CODE 32
#define RE_BITS_PER_CODE_SHIFT 5
#define RE_BITS_PER_CODE_MASK 0x1F
#define RE_INDEXES_PER_CODE 2
#define RE_INDEXES_PER_CODE_SHIFT 1
#define RE_INDEXES_PER_CODE_MASK 0x1

#define RE_MATCH_OP 0x1
#define RE_ZEROWIDTH_OP 0x2

static char copyright[] =
    " RE 2.3.0 Copyright (c) 1997-2002 by Secret Labs AB ";

/* The exception to return on error. */
static PyObject* error_exception;

/* The shortest string prefix for which we'll use a fast string search. */
#define RE_MIN_FAST_LENGTH 3

/* Handlers for ASCII, locale and Unicode. */
typedef struct RE_EncodingTable {
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);
    RE_CODE (*lower)(RE_CODE ch);
    RE_CODE (*upper)(RE_CODE ch);
    RE_CODE (*title)(RE_CODE ch);
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    BOOL (*same_char_ign_3)(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
      RE_CODE ch2_upper, RE_CODE ch2_title);
    BOOL (*at_boundary)(struct RE_State* state, Py_ssize_t text_pos);
    BOOL (*at_default_boundary)(struct RE_State* state, Py_ssize_t text_pos);
} RE_EncodingTable;

/* Position with the regex and text. */
typedef struct RE_Position {
    struct RE_Node* node;
    Py_ssize_t text_pos;
} RE_Position;

/* Storage for backtrack data. */
typedef struct RE_BacktrackData {
    RE_CODE op;
    union {
        struct {
            RE_Position position;
        } branch;
        struct {
            RE_Position position;
            Py_ssize_t ofs;
            Py_ssize_t start;
            size_t count;
            size_t max_count;
        } repeat;
    };
} RE_BacktrackData;

/* Storage for backtrack data is allocated in blocks for speed. */
typedef struct RE_BacktrackBlock {
    struct RE_BacktrackBlock* previous;
    struct RE_BacktrackBlock* next;
    size_t capacity;
    size_t count;
    RE_BacktrackData items[RE_BACKTRACK_BLOCK_SIZE];
} RE_BacktrackBlock;

typedef struct RE_NextNode {
    struct RE_Node* node;
    struct RE_Node* test;
    struct RE_Node* match_next;
    Py_ssize_t match_step;
} RE_NextNode;

/* A pattern node. */
typedef struct RE_Node {
    BOOL used;
    RE_CODE op;
    BOOL match;
    Py_ssize_t step;
    Py_ssize_t value_capacity;
    Py_ssize_t value_count;
    RE_CODE* values;
    RE_NextNode next_1;
    RE_NextNode next_2;
    Py_ssize_t* bad_character_offset;
    Py_ssize_t* good_suffix_offset;
} RE_Node;

/* Data about a group in a context. */
typedef struct RE_GroupData {
    Py_ssize_t begin;
    Py_ssize_t end;
} RE_GroupData;

/* Data about a repeat in a context. */
typedef struct RE_RepeatData {
    size_t count;
    Py_ssize_t start;
    size_t max_count;
} RE_RepeatData;

/* Data about groups or repeats. */
typedef union RE_Data {
    RE_GroupData group;
    RE_RepeatData repeat;
} RE_Data;

/* Info about a capture group. */
typedef struct RE_GroupInfo {
    int id;
    BOOL referenced;
    BOOL has_name;
    Py_ssize_t value_offset;
    Py_ssize_t end_index;
} RE_GroupInfo;

/* Info about a repeat. */
typedef struct RE_RepeatInfo {
    int id;
    Py_ssize_t value_offset;
} RE_RepeatInfo;

/* The state object used during matching. */
typedef struct RE_State {
    struct PatternObject* pattern; /* Parent PatternObject. */
    /* Info about the string being matched. */
    PyObject* string;
    Py_ssize_t charsize;
    BOOL wide;
    void* text;
    Py_ssize_t text_length;
    Py_ssize_t slice_start;
    Py_ssize_t slice_end;
    BOOL overlapped; /* Matches can be overlapped. */
    BOOL reverse; /* Search backwards. */
    Py_ssize_t* marks;
    Py_ssize_t lastindex;
    Py_ssize_t lastgroup;
    Py_ssize_t search_anchor;
    Py_ssize_t match_pos;
    Py_ssize_t text_pos;
    Py_ssize_t final_newline; /* Index of newline at end of string, or -1. */
    BOOL zero_width; /* Enable the correct handling of zero-width matches. */
    BOOL must_advance; /* The end of the match must advance past its start. */
    /* Storage for backtrack info. */
    RE_BacktrackBlock backtrack_block;
    RE_BacktrackBlock* current_block;
    RE_BacktrackData* backtrack;
    Py_ssize_t saved_groups_capacity;
    Py_ssize_t saved_groups_count;
    RE_Data* saved_groups;
    RE_Data* data; /* The data (groups followed by repeats). */
#if defined(RE_MULTITHREADED)
    BOOL is_multithreaded; /* Whether to release the GIL while matching. */
    PyThreadState* saved_GIL; /* Storage for GIL handling. */
#endif
    size_t min_width;
    RE_EncodingTable* encoding;
    RE_CODE (*char_at)(void* text, Py_ssize_t pos);
    BOOL do_check;
} RE_State;

/* The PatternObject created from a regular expression. */
typedef struct PatternObject {
    PyObject_HEAD
    PyObject* pattern; /* Pattern source (or None). */
    Py_ssize_t flags; /* Flags used when compiling pattern source. */
    PyObject* weakreflist; /* List of weak references */
    /* Nodes into which the regular expression is compiled. */
    RE_Node* start_node;
    RE_Node* success_node;
    Py_ssize_t data_count; /* Number of data (groups + repeats). */
    Py_ssize_t group_count; /* Number of capture groups. */
    Py_ssize_t repeat_count; /* Number of repeats. */
    Py_ssize_t group_end_index; /* Number of group closures. */
    PyObject* groupindex;
    PyObject* indexgroup;
    /* Storage for the pattern nodes. */
    Py_ssize_t node_capacity;
    Py_ssize_t node_count;
    RE_Node** node_list;
    /* Info about the capture groups. */
    Py_ssize_t group_info_capacity;
    RE_GroupInfo* group_info;
    /* Info about the repeats. */
    Py_ssize_t repeat_info_capacity;
    RE_RepeatInfo* repeat_info;
    size_t min_width;
    RE_EncodingTable* encoding; /* Encoding handlers. */
    Py_ssize_t* marks_storage;
    Py_ssize_t saved_groups_capacity_storage;
    RE_Data* saved_groups_storage;
    RE_Data* data_storage;
    BOOL do_check;
} PatternObject;

/* The MatchObject created when a match is found. */
typedef struct MatchObject {
    PyObject_HEAD
    PyObject* string; /* Link to the target string. */
    PatternObject* pattern; /* Link to the regex (pattern) object. */
    Py_ssize_t pos; /* Start of current slice. */
    Py_ssize_t endpos; /* End of current slice. */
    Py_ssize_t lastindex; /* Last group seen by the engine (-1 if none). */
    Py_ssize_t lastgroup; /* Last named group seen by the engine (-1 if none). */
    Py_ssize_t group_count;
    Py_ssize_t* marks;
    PyObject* regs;
} MatchObject;

/* The ScannerObject. */
typedef struct ScannerObject {
    PyObject_HEAD
    PatternObject* pattern;
    RE_State state;
} ScannerObject;

/* The SplitterObject. */
typedef struct SplitterObject {
    PyObject_HEAD
    PatternObject* pattern;
    RE_State state;
    Py_ssize_t maxsplit;
    Py_ssize_t last;
    Py_ssize_t split_count;
    Py_ssize_t index;
    BOOL finished;
} SplitterObject;

typedef struct RE_CompileArgs {
    RE_CODE* code;
    RE_CODE* end_code;
    PatternObject* pattern;
    BOOL forward;
    size_t min_width;
    RE_Node* start;
    RE_Node* end;
} RE_CompileArgs;

typedef struct JoinInfo {
    PyObject* list;
    PyObject* item;
    BOOL reversed;
} JoinInfo;

typedef Py_UNICODE RE_UCHAR;
typedef unsigned char RE_BCHAR;

#define UCHAR_AT(text, pos) *((RE_UCHAR*)(text) + (pos))
#define BCHAR_AT(text, pos) *((RE_BCHAR*)(text) + (pos))

/* The problem of case-insensitive matches.
 *
 * The current re module performs case-insensitive matches by forcing the
 * character in the pattern to lowercase and comparing with the lowercase form
 * of the character in the text.
 *
 * Unfortunately, this doesn't always work.
 *
 * For example, in the Turkish version of the Latin alphabet the letter pair:
 *
 *     'lowercase I' vs 'uppercase I'
 *
 * has been split into dotted and a dotless pairs, each with lowercase and
 * uppercase forms:
 *
 *     'lowercase dotless I' vs 'uppercase dotless I'
 *
 *     'lowercase dotted I' vs 'uppercase dotted I'
 *
 * 'Lowercase dotless I' and 'uppercase dotted I' will convert to the other
 * case correctly, but 'lowercase dotted I' and 'uppercase dotless I', which
 * are not specifically Turkish, will convert the standard way, which is
 * incorrect for Turkish.
 *
 * If you just forced two characters to lowercase then the comparison:
 *
 *     'lowercase dotted I' vs 'uppercase dotted I'
 *
 * would become one of:
 *
 *     'lowercase dotted I' vs 'lowercase dotted I'
 *
 * but the comparison:
 *
 *     'lowercase dotless I' vs 'uppercase dotless I'
 *
 * would become one of:
 *
 *     'lowercase dotless I' vs 'lowercase dotted I'
 *
 * which wouldn't match, but should.
 *
 * Forcing both characters to uppercase would work in this case, but fail in
 * others.
 *
 * Trying both together wouldn't work either: the comparison:
 *
 *     'uppercase dotless I' vs 'uppercase dotted I'
 *
 * for example, could become one of:
 *
 *     'lowercase dotted I' vs 'lowercase dotted I'
 *
 * which would match, but shouldn't.
 *
 * Another factor is that some characters have 3 cases: lowercase, uppercase
 * and titlecase.
 *
 * There's the need, therefore, is to find a combination of comparisons which
 * gives the correct answer (or as close as is possible!), but without
 * excessive cost.
 *
 * What seems to work is never to compare 2 converted characters with each
 * other, but only a converted character with an unconverted character.
 */

/* Gets a byte character at the given position. */
static RE_CODE bytes_char_at(void* text, Py_ssize_t pos) {
    return *((RE_BCHAR*)text + pos);
}

/* Gets a Unicode character at the given position. */
static RE_CODE unicode_char_at(void* text, Py_ssize_t pos) {
    return *((RE_UCHAR*)text + pos);
}

/* ASCII-specific. */

#define RE_ASCII_MAX 0x7F

/* Checks whether an ASCII character has the given property. */
static BOOL ascii_has_property(RE_CODE property, RE_CODE ch) {
    if (ch > RE_ASCII_MAX)
        /* Outside the ASCII range. */
        return FALSE;

    switch (property) {
    case RE_PROP_ALNUM:
        return (re_ascii_property[ch] & RE_MASK_ALNUM) != 0;
    case RE_PROP_ALPHA:
        return (re_ascii_property[ch] & RE_MASK_ALPHA) != 0;
    case RE_PROP_ASCII:
        return TRUE;
    case RE_PROP_BLANK:
        return ch == '\t' || ch == ' ';
    case RE_PROP_CNTRL:
        return ch < 0x20 || ch == 0x7F;
    case RE_PROP_DIGIT:
        return (re_ascii_property[ch] & RE_MASK_DIGIT) != 0;
    case RE_PROP_GRAPH:
        return 0x21 <= ch && ch <= 0x7E;
    case RE_PROP_LINEBREAK:
        return ch == '\n';
    case RE_PROP_LOWER:
        return (re_ascii_property[ch] & RE_MASK_LOWER) != 0;
    case RE_PROP_PRINT:
        return 0x20 <= ch && ch <= 0x7E;
    case RE_PROP_PUNCT:
        return (re_ascii_property[ch] & RE_MASK_PUNCT) != 0;
    case RE_PROP_SPACE:
        return (re_ascii_property[ch] & RE_MASK_SPACE) != 0;
    case RE_PROP_UPPER:
        return (re_ascii_property[ch] & RE_MASK_UPPER) != 0;
    case RE_PROP_WORD:
        return ch == '_' || (re_ascii_property[ch] & RE_MASK_ALNUM) != 0;
    case RE_PROP_XDIGIT:
        return (re_ascii_property[ch] & RE_MASK_XDIGIT) != 0;
    default:
        return FALSE;
    }
}

/* Converts an ASCII character to lowercase. */
static RE_CODE ascii_lower(RE_CODE ch) {
    if (ch > RE_ASCII_MAX || (re_ascii_property[ch] & RE_MASK_UPPER) == 0)
        return ch;

    return ch ^ 0x20;
}

/* Converts an ASCII character to uppercase. */
static RE_CODE ascii_upper(RE_CODE ch) {
    if (ch > RE_ASCII_MAX || (re_ascii_property[ch] & RE_MASK_LOWER) == 0)
        return ch;

    return ch ^ 0x20;
}

/* Checks whether 2 ASCII characters are the same, ignoring case.
 *
 * We can ignore titlecase because for ASCII it's the same as uppercase.
 */
static BOOL ascii_same_char_ign(RE_CODE ch1, RE_CODE ch2) {
    return ch1 == ch2 || ascii_lower(ch1) == ascii_lower(ch2);
}

/* Checks whether 2 ASCII characters are the same, ignoring case.
 *
 * Optimised for comparing multiple times against a fixed character.
 *
 * We can ignore titlecase because for ASCII it's the same as uppercase.
 */
static BOOL ascii_same_char_ign_3(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
  RE_CODE ch2_upper, RE_CODE ch2_title) {
    return ch1 == ch2_lower || ch1 == ch2_upper;
}

/* Checks whether the current text position is on a word boundary. */
static BOOL ascii_at_boundary(RE_State* state, Py_ssize_t text_pos) {
    BOOL before;
    BOOL after;

    before = text_pos > 0 && ascii_has_property(RE_PROP_WORD,
      state->char_at(state->text, text_pos - 1));
    after = text_pos < state->text_length && ascii_has_property(RE_PROP_WORD,
      state->char_at(state->text, text_pos));

    return before != after;
}

/* The handlers for ASCII characters. */
static RE_EncodingTable ascii_encoding = {
    ascii_has_property,
    ascii_lower,
    ascii_upper,
    ascii_upper, /* For ASCII, titlecase is the same as uppercase. */
    ascii_same_char_ign,
    ascii_same_char_ign_3,
    ascii_at_boundary,
    ascii_at_boundary, /* No special "default word boundary" for ASCII. */
};

/* Locale-specific. */

#define RE_LOCALE_MAX 0xFF

/* Checks whether a locale character has the given property. */
static BOOL locale_has_property(RE_CODE property, RE_CODE ch) {
    if (ch > RE_LOCALE_MAX)
        /* Outside the locale range. */
        return FALSE;

    switch (property) {
    case RE_PROP_ALNUM:
        return isalnum(ch) != 0;
    case RE_PROP_ALPHA:
        return isalpha(ch) != 0;
    case RE_PROP_ASCII:
        return ch <= RE_ASCII_MAX;
    case RE_PROP_BLANK:
        return ch == '\t' || ch == ' ';
    case RE_PROP_CNTRL:
        return iscntrl(ch) != 0;
    case RE_PROP_DIGIT:
        return isdigit(ch) != 0;
    case RE_PROP_GRAPH:
        return isgraph(ch) != 0;
    case RE_PROP_LINEBREAK:
        return ch == '\n';
    case RE_PROP_LOWER:
        return islower(ch) != 0;
    case RE_PROP_PRINT:
        return isprint(ch) != 0;
    case RE_PROP_PUNCT:
        return ispunct(ch) != 0;
    case RE_PROP_SPACE:
        return isspace(ch) != 0;
    case RE_PROP_UPPER:
        return isupper(ch) != 0;
    case RE_PROP_WORD:
        return ch == '_' || isalnum(ch) != 0;
    case RE_PROP_XDIGIT:
        return ch <= RE_ASCII_MAX && (re_ascii_property[ch] & RE_MASK_XDIGIT)
          != 0;
    default:
        return FALSE;
    }
}

/* Converts a locale character to lowercase. */
static RE_CODE locale_lower(RE_CODE ch) {
    if (ch > RE_LOCALE_MAX)
        return ch;

    return tolower(ch);
}

/* Converts a locale character to uppercase. */
static RE_CODE locale_upper(RE_CODE ch) {
    if (ch > RE_LOCALE_MAX)
        return ch;

    return toupper(ch);
}

/* Checks whether 2 characters are the same, ignoring case.
 *
 * We need to handle possible non-reversible case conversions.
 *
 * We can ignore titlecase because for locale it's the same as uppercase.
 */
static BOOL locale_same_char_ign(RE_CODE ch1, RE_CODE ch2) {
    RE_CODE ch1_lower;
    RE_CODE ch1_upper;

    /* Start with a simple comparison. */
    if (ch1 == ch2)
        return TRUE;

    /* Are there different cases? */
    ch1_lower = locale_lower(ch1);
    ch1_upper = locale_upper(ch1);
    if (ch1_lower == ch1_upper)
        /* Lowercase == uppercase, so the characters are caseless and would've
         * matched already if they could.
         */
        return FALSE;

    /* There are cases, so compare the remaining combinations. */
    return ch1_lower == ch2 || ch1_upper == ch2 || ch1 == locale_lower(ch2) ||
      ch1 == locale_upper(ch2);
}

/* Checks whether 2 characters are the same, ignoring case.
 *
 * Optimised for comparing multiple times against a fixed character.
 *
 * We need to handle possible non-reversible case conversions.
 *
 * We can ignore titlecase because for locale it's the same as uppercase.
 */
static BOOL locale_same_char_ign_3(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
  RE_CODE ch2_upper, RE_CODE ch2_title) {
    /* Start with simple comparisons. */
    if (ch1 == ch2_lower || ch1 == ch2_upper)
        return TRUE;

    /* Are there different cases? */
    if (ch2_lower == ch2_upper)
        /* Lowercase == uppercase, so the characters are caseless and would've
         * matched already if they could.
         */
        return FALSE;

    /* There are cases, so compare the remaining combinations. */
    return locale_lower(ch1) == ch2 || locale_upper(ch1) == ch2;
}

/* Checks whether the current text position is on a word boundary. */
static BOOL locale_at_boundary(RE_State* state, Py_ssize_t text_pos) {
    BOOL before;
    BOOL after;

    before = text_pos > 0 && locale_has_property(RE_PROP_WORD,
      state->char_at(state->text, text_pos - 1));
    after = text_pos < state->text_length && locale_has_property(RE_PROP_WORD,
      state->char_at(state->text, text_pos));

    return before != after;
}

/* The handlers for locale characters. */
static RE_EncodingTable locale_encoding = {
    locale_has_property,
    locale_lower,
    locale_upper,
    locale_upper, /* For locale, titlecase is the same as uppercase. */
    locale_same_char_ign,
    locale_same_char_ign_3,
    locale_at_boundary,
    locale_at_boundary, /* No special "default word boundary" for locale. */
};

/* Unicode-specific. */

/* Unicode character properties. */

/* (Typedefs copied from unicodedata.c) */

#if PY_VERSION_HEX < 0x02060000
typedef struct {
    const unsigned char category;	/* index into
					   _PyUnicode_CategoryNames */
    const unsigned char	combining; 	/* combining class value 0 - 255 */
    const unsigned char	bidirectional; 	/* index into
					   _PyUnicode_BidirectionalNames */
    const unsigned char mirrored;	/* true if mirrored in bidir mode */
    const unsigned char east_asian_width;	/* index into
						   _PyUnicode_EastAsianWidth */
} _PyUnicode_DatabaseRecord;

typedef struct change_record {
    /* sequence of fields should be the same as in merge_old_version */
    const unsigned char bidir_changed;
    const unsigned char category_changed;
    const unsigned char decimal_changed;
    const int numeric_changed;
} change_record;
#elif PY_VERSION_HEX < 0x02070000
typedef struct {
    const unsigned char category;	/* index into
					   _PyUnicode_CategoryNames */
    const unsigned char	combining; 	/* combining class value 0 - 255 */
    const unsigned char	bidirectional; 	/* index into
					   _PyUnicode_BidirectionalNames */
    const unsigned char mirrored;	/* true if mirrored in bidir mode */
    const unsigned char east_asian_width;	/* index into
						   _PyUnicode_EastAsianWidth */
} _PyUnicode_DatabaseRecord;

typedef struct change_record {
    /* sequence of fields should be the same as in merge_old_version */
    const unsigned char bidir_changed;
    const unsigned char category_changed;
    const unsigned char decimal_changed;
    const unsigned char mirrored_changed;
    const int numeric_changed;
} change_record;
#else
typedef struct {
    const unsigned char category;	/* index into
					   _PyUnicode_CategoryNames */
    const unsigned char	combining; 	/* combining class value 0 - 255 */
    const unsigned char	bidirectional; 	/* index into
					   _PyUnicode_BidirectionalNames */
    const unsigned char mirrored;	/* true if mirrored in bidir mode */
    const unsigned char east_asian_width;	/* index into
						   _PyUnicode_EastAsianWidth */
    const unsigned char normalization_quick_check; /* see is_normalized() */
} _PyUnicode_DatabaseRecord;

typedef struct change_record {
    /* sequence of fields should be the same as in merge_old_version */
    const unsigned char bidir_changed;
    const unsigned char category_changed;
    const unsigned char decimal_changed;
    const unsigned char mirrored_changed;
    const double numeric_changed;
} change_record;
#endif

/* data file generated by Tools/unicode/makeunicodedata.py */
#include "unicodedata_db.h"

static const _PyUnicode_DatabaseRecord*
_getrecord_ex(Py_UCS4 code)
{
    int index;
    if (code >= 0x110000)
        index = 0;
    else {
        index = index1[(code>>SHIFT)];
        index = index2[(index<<SHIFT)+(code&((1<<SHIFT)-1))];
    }

    return &_PyUnicode_Database_Records[index];
}
/* End of copied code. */

/* Locates the info for a Unicode codepoint. */
Py_LOCAL(RE_InfoRange*) get_codepoint_info(RE_CODE ch) {
    size_t lo;
    size_t hi;

    lo = 0;
    hi = sizeof(re_codepoint_info) / sizeof(re_codepoint_info[0]) - 1;
    while (lo <= hi) {
        size_t mid;
        RE_InfoRange* info;

        mid = (lo + hi) / 2;
        info = &re_codepoint_info[mid];
        if (ch < info->min_char)
            hi = mid - 1;
        else if (ch <= info->max_char)
            return info;
        else
            lo = mid + 1;
    }

    return NULL;
}

/* Checks whether a Unicode character has the given property. */
static BOOL unicode_has_property(RE_CODE property, RE_CODE ch) {
    unsigned int flag;

    switch (property) {
    case RE_PROP_ALNUM:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_ALNUM) != 0;
    case RE_PROP_ALPHA:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_L) != 0;
    case RE_PROP_ASCII:
        return ch <= RE_ASCII_MAX;
    case RE_PROP_BLANK:
        if (ch == '\t')
            return TRUE;

        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_ZS) != 0;
    case RE_PROP_C:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_C) != 0;
    case RE_PROP_CC:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_CC) != 0;
    case RE_PROP_CF:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_CF) != 0;
    case RE_PROP_CN:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_CN) != 0;
    case RE_PROP_CNTRL:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_CC) != 0;
    case RE_PROP_CO:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_CO) != 0;
    case RE_PROP_CS:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_CS) != 0;
    case RE_PROP_DIGIT:
        return _PyUnicode_IsDigit(ch);
    case RE_PROP_GRAPH:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_NONGRAPH) == 0;
    case RE_PROP_L:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_L) != 0;
    case RE_PROP_LINEBREAK:
        return ch == '\n';
    case RE_PROP_LL:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_LL) != 0;
    case RE_PROP_LM:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_LM) != 0;
    case RE_PROP_LO:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_LO) != 0;
    case RE_PROP_LOWER:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_LL) != 0;
    case RE_PROP_LT:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_LT) != 0;
    case RE_PROP_LU:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_LU) != 0;
    case RE_PROP_M:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_M) != 0;
    case RE_PROP_MC:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_MC) != 0;
    case RE_PROP_ME:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_ME) != 0;
    case RE_PROP_MN:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_MN) != 0;
    case RE_PROP_N:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_N) != 0;
    case RE_PROP_ND:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_ND) != 0;
    case RE_PROP_NL:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_NL) != 0;
    case RE_PROP_NO:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_NO) != 0;
    case RE_PROP_P:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_P) != 0;
    case RE_PROP_PC:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_PC) != 0;
    case RE_PROP_PD:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_PD) != 0;
    case RE_PROP_PE:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_PE) != 0;
    case RE_PROP_PF:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_PF) != 0;
    case RE_PROP_PI:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_PI) != 0;
    case RE_PROP_PO:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_PO) != 0;
    case RE_PROP_PRINT:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_C) == 0;
    case RE_PROP_PS:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_PS) != 0;
    case RE_PROP_PUNCT:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_PUNCT) != 0;
    case RE_PROP_S:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_S) != 0;
    case RE_PROP_SC:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_SC) != 0;
    case RE_PROP_SK:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_SK) != 0;
    case RE_PROP_SM:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_SM) != 0;
    case RE_PROP_SO:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_SO) != 0;
    case RE_PROP_SPACE:
        return _PyUnicode_IsWhitespace(ch);
    case RE_PROP_UPPER:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_LU) != 0;
    case RE_PROP_WORD:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_WORD) != 0;
    case RE_PROP_XDIGIT:
        return ch <= RE_ASCII_MAX && (re_ascii_property[ch] & RE_MASK_XDIGIT)
          !=0;
    case RE_PROP_Z:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_Z) != 0;
    case RE_PROP_ZL:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_ZL) != 0;
    case RE_PROP_ZP:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_ZP) != 0;
    case RE_PROP_ZS:
        flag = 1 << _getrecord_ex((Py_UCS4)ch)->category;
        return (flag & RE_PROP_MASK_ZS) != 0;
    default:
    {
        RE_InfoRange *info;

        info = get_codepoint_info(ch);
        if (!info)
            return FALSE;

        if (RE_MIN_BLOCK <= property && property <= RE_MAX_BLOCK)
            return info->block == property;

        if (RE_MIN_SCRIPT <= property && property <= RE_MAX_SCRIPT)
            return info->script == property;

        return FALSE;
    }
    }
}

/* Converts a Unicode character to lowercase. */
static RE_CODE unicode_lower(RE_CODE ch) {
    return Py_UNICODE_TOLOWER((Py_UNICODE)ch);
}

/* Converts a Unicode character to uppercase. */
static RE_CODE unicode_upper(RE_CODE ch) {
    return Py_UNICODE_TOUPPER((Py_UNICODE)ch);
}

/* Converts a Unicode character to titlecase. */
static RE_CODE unicode_title(RE_CODE ch) {
    return Py_UNICODE_TOTITLE((Py_UNICODE)ch);
}

/* Checks whether 2 characters are the same, ignoring case. */
static BOOL unicode_same_char_ign(RE_CODE ch1, RE_CODE ch2) {
    RE_CODE ch1_lower;
    RE_CODE ch1_upper;
    RE_CODE ch1_title;

    /* Start with a simple comparison. */
    if (ch1 == ch2)
        return TRUE;

    /* Are there different cases? */
    ch1_lower = unicode_lower(ch1);
    ch1_upper = unicode_upper(ch1);
    if (ch1_lower == ch1_upper)
        /* Lowercase == uppercase, so the characters are caseless and would've
         * matched already if they could.
         */
        return FALSE;

    /* There are cases, so compare some more combinations. */
    if (ch1_lower == ch2 || ch1_upper == ch2)
        return TRUE;

    /* There might be titlecase too. */
    ch1_title = unicode_title(ch1);
    if (ch1_title == ch2)
        return TRUE;

    /* Compare yet more combinations. */
    if (ch1 == unicode_lower(ch2) || ch1 == unicode_upper(ch2))
        return TRUE;

    /* If titlecase == uppercase, we know already that they don't match. */
    return ch1_title != ch1_upper && ch1 == unicode_title(ch2);
}

/* Checks whether 2 characters are the same, ignoring case.
 *
 * Optimised for comparing multiple times against a fixed character.
 */
static BOOL unicode_same_char_ign_3(RE_CODE ch1, RE_CODE ch2, RE_CODE
  ch2_lower, RE_CODE ch2_upper, RE_CODE ch2_title) {
    /* Start with a simple comparison. */
    if (ch1 == ch2)
        return TRUE;

    /* Are there different cases? */
    if (ch2_lower == ch2_upper)
        /* Lowercase == uppercase, so the characters are caseless and would've
         * matched already if they could.
         */
        return FALSE;

    /* There are cases, so compare some more combinations. */
    if (ch1 == ch2_lower || ch1 == ch2_upper || ch1 == ch2_title)
        return TRUE;

    /* Compare yet more combinations. */
    if (unicode_lower(ch1) == ch2 || unicode_upper(ch1) == ch2)
        return TRUE;

    /* If titlecase == uppercase, we know already that they don't match. */
    return ch2_title != ch2_upper && unicode_title(ch1) == ch2;
}

/* Checks whether the current text position is on a word boundary. */
static BOOL unicode_at_boundary(RE_State* state, Py_ssize_t text_pos) {
    BOOL before;
    BOOL after;

    before = text_pos > 0 && unicode_has_property(RE_PROP_WORD,
      state->char_at(state->text, text_pos - 1));
    after = text_pos < state->text_length && unicode_has_property(RE_PROP_WORD,
      state->char_at(state->text, text_pos));

    return before != after;
}

/* Gets the Unicode word break property for a character. */
Py_LOCAL(int) word_break_property(RE_CODE ch) {
    size_t lo;
    size_t hi;

    lo = 0;
    hi = sizeof(re_codepoint_info) / sizeof(re_codepoint_info[0]) - 1;
    while (lo <= hi) {
        size_t mid;
        RE_InfoRange* info;

        mid = (lo + hi) / 2;
        info = &re_codepoint_info[mid];
        if (ch < info->min_char)
            hi = mid - 1;
        else if (ch <= info->max_char)
            return info->word_break;
        else
            lo = mid + 1;
    }

    return RE_BREAK_OTHER;
}

/* Checks whether a character is a Unicode vowel.
 *
 * Only a limited number are treated as vowels.
 */
Py_LOCAL(BOOL) is_unicode_vowel(RE_CODE ch) {
    switch (unicode_lower(ch)) {
    case 'a': case 0xE0: case 0xE1: case 0xE2:
    case 'e': case 0xE8: case 0xE9: case 0xEA:
    case 'i': case 0xEC: case 0xED: case 0xEE:
    case 'o': case 0xF2: case 0xF3: case 0xF4:
    case 'u': case 0xF9: case 0xFA: case 0xFB:
        return TRUE;
    default:
        return FALSE;
    }
}

/* Checks whether the current text position is on a default word boundary. */
static BOOL unicode_at_default_boundary(RE_State* state, Py_ssize_t text_pos) {
    void* text;
    RE_CODE (*char_at)(void* text, Py_ssize_t pos);
    int prop;
    int prop_m1;
    Py_ssize_t pos_m1;
    Py_ssize_t pos_p1;
    int prop_p1;
    Py_ssize_t pos_m2;
    int prop_m2;

    /* Break at the start and end of text. */
    if (text_pos <= 0 || text_pos >= state->text_length)
        return TRUE;

    text = state->text;
    char_at = state->char_at;

    prop = word_break_property(char_at(text, text_pos));
    prop_m1 = word_break_property(char_at(text, text_pos - 1));

    /* Don't break within CRLF. */
    if (prop_m1 == RE_BREAK_CR && prop == RE_BREAK_LF)
        return FALSE;

    /* Otherwise break before and after Newlines (including CR and LF). */
    if (prop_m1 == RE_BREAK_NEWLINE || prop_m1 == RE_BREAK_CR || prop_m1 ==
      RE_BREAK_LF || prop == RE_BREAK_NEWLINE || prop == RE_BREAK_CR || prop ==
      RE_BREAK_LF)
        return TRUE;

    /* Don't break just before Format or Extend characters. */
    if (prop == RE_BREAK_EXTEND || prop == RE_BREAK_FORMAT)
        return FALSE;

    /* Get the property of the previous character. */
    pos_m1 = text_pos - 1;
    prop_m1 = RE_BREAK_OTHER;
    while (pos_m1 >= 0) {
        prop_m1 = word_break_property(char_at(text, pos_m1));
        if (prop_m1 != RE_BREAK_EXTEND && prop_m1 != RE_BREAK_FORMAT)
            break;
        --pos_m1;
    }

    /* Don't break between most letters. */
    if (prop_m1 == RE_BREAK_ALETTER && prop == RE_BREAK_ALETTER)
        return FALSE;

    if (pos_m1 >= 0 && char_at(text, pos_m1) == '\'' &&
      is_unicode_vowel(char_at(text, text_pos)))
        return TRUE;

    pos_p1 = text_pos + 1;
    prop_p1 = RE_BREAK_OTHER;
    while (pos_p1 < state->text_length) {
        prop_p1 = word_break_property(char_at(text, pos_p1));
        if (prop_p1 != RE_BREAK_EXTEND && prop_p1 != RE_BREAK_FORMAT)
            break;
        --pos_p1;
    }

    /* Don't break letters across certain punctuation. */
    if (prop_m1 == RE_BREAK_ALETTER && (prop == RE_BREAK_MIDLETTER || prop ==
      RE_BREAK_MIDNUMLET) && prop_p1 == RE_BREAK_ALETTER)
        return FALSE;

    pos_m2 = pos_m1 - 1;
    prop_m2 = RE_BREAK_OTHER;
    while (pos_m2 >= 0) {
        prop_m2 = word_break_property(char_at(text, pos_m2));
        if (prop_m2 != RE_BREAK_EXTEND && prop_m1 != RE_BREAK_FORMAT)
            break;
        --pos_m2;
    }

    if (prop_m2 == RE_BREAK_ALETTER && (prop_m1 == RE_BREAK_MIDLETTER ||
      prop_m1 == RE_BREAK_MIDNUMLET) && prop == RE_BREAK_ALETTER)
        return FALSE;

    /* Don't break within sequences of digits, or digits adjacent to letters
     * ("3a", or "A3").
     */
    if ((prop_m1 == RE_BREAK_NUMERIC || prop_m1 == RE_BREAK_ALETTER) && prop ==
      RE_BREAK_NUMERIC)
        return FALSE;

    if (prop_m1 == RE_BREAK_NUMERIC && prop == RE_BREAK_ALETTER)
        return FALSE;

    /* Don't break within sequences, such as "3.2" or "3,456.789". */
    if (prop_m2 == RE_BREAK_NUMERIC && (prop_m1 == RE_BREAK_MIDNUM || prop_m1
      == RE_BREAK_MIDNUMLET) && prop == RE_BREAK_NUMERIC)
        return FALSE;

    if (prop_m1 == RE_BREAK_NUMERIC && (prop == RE_BREAK_MIDNUM || prop ==
      RE_BREAK_MIDNUMLET) && prop_p1 == RE_BREAK_NUMERIC)
        return FALSE;

    /* Don't break between Katakana. */
    if (prop_m1 == RE_BREAK_KATAKANA && prop == RE_BREAK_KATAKANA)
        return FALSE;

    /* Don't break from extenders. */
    if ((prop_m1 == RE_BREAK_ALETTER || prop_m1 == RE_BREAK_NUMERIC || prop_m1
      == RE_BREAK_KATAKANA || prop_m1 == RE_BREAK_EXTENDNUMLET) && prop ==
      RE_BREAK_EXTENDNUMLET)
        return FALSE;

    if (prop_m1 == RE_BREAK_EXTENDNUMLET && (prop == RE_BREAK_ALETTER || prop
      == RE_BREAK_NUMERIC || prop == RE_BREAK_KATAKANA))
        return FALSE;

    /* Otherwise, break everywhere (including around ideographs). */
    return TRUE;
}

/* The handlers for Unicode characters. */
static RE_EncodingTable unicode_encoding = {
    unicode_has_property,
    unicode_lower,
    unicode_upper,
    unicode_title,
    unicode_same_char_ign,
    unicode_same_char_ign_3,
    unicode_at_boundary,
    unicode_at_default_boundary,
};

/* Sets the error message. */
Py_LOCAL(void) set_error(int status, PyObject* object) {
    TRACE(("<<set_error>>\n"))

    switch (status) {
    case RE_ERROR_GROUP_INDEX_TYPE:
        if (object)
            PyErr_Format(PyExc_TypeError,
              "group indices must be integers or strings, not %.200s",
              object->ob_type->tp_name);
        else
            PyErr_Format(PyExc_TypeError,
              "group indices must be integers or strings");
        break;
    case RE_ERROR_ILLEGAL:
        PyErr_SetString(PyExc_RuntimeError, "invalid RE code");
        break;
    case RE_ERROR_INTERRUPTED:
        /* An exception has already been raised, so let it fly. */
        break;
    case RE_ERROR_INVALID_GROUP_REF:
        PyErr_SetString(error_exception, "invalid group reference");
        break;
    case RE_ERROR_MEMORY:
        PyErr_NoMemory();
        break;
    case RE_ERROR_NO_SUCH_GROUP:
        PyErr_SetString(PyExc_IndexError, "no such group");
        break;
    case RE_ERROR_REPLACEMENT:
        PyErr_SetString(error_exception, "invalid replacement");
        break;
    case RE_ERROR_INDEX:
        PyErr_SetString(PyExc_TypeError, "string indices must be integers");
        break;
    default:
        /* Other error codes indicate compiler/engine bugs. */
        PyErr_SetString(PyExc_RuntimeError,
          "internal error in regular expression engine");
        break;
    }
}

/* Allocates memory.
 *
 * Sets the Python error handler and returns NULL if the allocation fails.
 */
Py_LOCAL(void*) re_alloc(size_t size) {
    void* new_ptr;

    new_ptr = PyMem_Malloc(size);
    if (!new_ptr)
        set_error(RE_ERROR_MEMORY, NULL);

    return new_ptr;
}

/* Reallocates memory.
 *
 * Sets the Python error handler and returns NULL if the reallocation fails.
 */
Py_LOCAL(void*) re_realloc(void* ptr, size_t size) {
    void* new_ptr;

    new_ptr = PyMem_Realloc(ptr, size);
    if (!new_ptr)
        set_error(RE_ERROR_MEMORY, NULL);

    return new_ptr;
}

/* Deallocates memory. */
Py_LOCAL(void) re_dealloc(void* ptr) {
    PyMem_Free(ptr);
}

#if defined(RE_MULTITHREADED)
/* Releases the GIL. */
Py_LOCAL(void) release_GIL(RE_State* state) {
    state->saved_GIL = PyEval_SaveThread();
}

/* Acquires the GIL. */
Py_LOCAL(void) acquire_GIL(RE_State* state) {
    PyEval_RestoreThread(state->saved_GIL);
}

#endif
/* Allocates memory, holding the GIL during the allocation.
 *
 * Sets the Python error handler and returns NULL if the allocation fails.
 */
Py_LOCAL(void*) safe_alloc(RE_State* state, size_t size) {
    void* new_ptr;

#if defined(RE_MULTITHREADED)
    if (state->is_multithreaded)
        acquire_GIL(state);

#endif
    new_ptr = re_alloc(size);

#if defined(RE_MULTITHREADED)
    if (state->is_multithreaded)
        release_GIL(state);

#endif
    return new_ptr;
}

/* Reallocates memory, holding the GIL during the reallocation.
 *
 * Sets the Python error handler and returns NULL if the reallocation fails.
 */
Py_LOCAL(void*) safe_realloc(RE_State* state, void* ptr, size_t size) {
    void* new_ptr;

#if defined(RE_MULTITHREADED)
    if (state->is_multithreaded)
        acquire_GIL(state);

#endif
    new_ptr = re_realloc(ptr, size);

#if defined(RE_MULTITHREADED)
    if (state->is_multithreaded)
        release_GIL(state);

#endif
    return new_ptr;
}

/* Checks for KeyboardInterrupt, holding the GIL during the check. */
Py_LOCAL(BOOL) safe_check_signals(RE_State* state) {
    BOOL result;

#if defined(RE_MULTITHREADED)
    if (state->is_multithreaded)
        acquire_GIL(state);

#endif
    result = PyErr_CheckSignals();

#if defined(RE_MULTITHREADED)
    if (state->is_multithreaded)
        release_GIL(state);

#endif
    return result;
}

/* Checks whether a character is in a big bitset. */
Py_LOCAL(BOOL) in_big_bitset(RE_Node* node, RE_CODE ch) {
    /* values are: max_char indexes... subsets... */
    RE_CODE* values;
    RE_CODE max_char;
    RE_CODE* start_of_indexes;
    RE_CODE* start_of_subsets;
    size_t index_of_subset;
    size_t offset_into_subset;
    size_t index;
    RE_CODE* subset;
    size_t b;
    BOOL match;

    values = node->values;
    max_char = values[0];
    if (ch > max_char)
        return !node->match;

    /* Point to the start of the indexes and subsets. */
    start_of_indexes = values + 1;
    start_of_subsets = start_of_indexes + (max_char >> (8 +
      RE_INDEXES_PER_CODE_SHIFT)) + 1;

    /* We want the index of the subset and the offset within the subset. */
    index_of_subset = ch >> 8;
    offset_into_subset = ch & 0xFF;

    /* Identical subsets are merged together, so we need to determine which
     * subset to look in.
     */
    index = start_of_indexes[index_of_subset >> RE_INDEXES_PER_CODE_SHIFT];
    index >>= RE_BITS_PER_INDEX * (index_of_subset & RE_INDEXES_PER_CODE_MASK);
    index_of_subset = index & ~(~(RE_CODE)0 << RE_BITS_PER_INDEX);

    /* Point to the subset. */
    subset = start_of_subsets + index_of_subset * (256 / RE_BITS_PER_CODE);

    /* Get the bit flag for the character. */
    b = subset[offset_into_subset >> RE_BITS_PER_CODE_SHIFT] >>
      (offset_into_subset & RE_BITS_PER_CODE_MASK);

    match = (b & 1) != 0;

    return match == node->match;
}

/* Checks whether a character is in a small bitset. */
Py_LOCAL(BOOL) in_small_bitset(RE_Node* node, RE_CODE ch) {
    /* values are: top_bits bitset */
    RE_CODE* values;
    RE_CODE* subset;
    size_t index;
    size_t b;
    BOOL match;

    values = node->values;
    if ((ch >> 8) != values[0])
        return !node->match;

    subset = values + 1;
    index = ch & 0xFF;
    b = subset[index >> RE_BITS_PER_CODE_SHIFT] >> (index &
      RE_BITS_PER_CODE_MASK);
    match = (b & 1) != 0;

    return match == node->match;
}

/* Checks whether a character is in a set. */
Py_LOCAL(BOOL) in_set(RE_EncodingTable* encoding, RE_Node* node, RE_CODE ch) {
    RE_Node* member;
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);

    member = node->next_2.node;
    has_property = encoding->has_property;

    while (member) {
        switch (member->op) {
        case RE_OP_ANY:
            TRACE(("%s\n", re_op_text[member->op]))
            if (ch != '\n')
                return node->match;
            break;
        case RE_OP_BIG_BITSET:
            /* values are: size max_char indexes... subsets... */
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_big_bitset(member, ch))
                return node->match;
            break;
        case RE_OP_CHARACTER: /* A character literal. */
            /* values are: char_code */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              values[0]))
            if ((ch == member->values[0]) == member->match)
                return node->match;
            break;
        case RE_OP_PROPERTY: /* A character property. */
            /* values are: property */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              member->values[0]))
            if (has_property(member->values[0], ch) == member->match)
                return node->match;
            break;
        case RE_OP_SMALL_BITSET:
            /* values are: size top_bits bitset */
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_small_bitset(member, ch))
                return node->match;
            break;
        default:
            return FALSE;
        }

        member = member->next_1.node;
    }

    return !node->match;
}

/* Pushes the groups. */
Py_LOCAL(BOOL) push_groups(RE_State* state) {
    PatternObject* pattern;
    size_t new_count;
    size_t new_capacity;

    pattern = state->pattern;
    if (pattern->group_count == 0)
        return TRUE;

    new_count = state->saved_groups_count + pattern->group_count;
    new_capacity = state->saved_groups_capacity;
    while (new_count > new_capacity)
        new_capacity *= 2;

    if (new_capacity != state->saved_groups_capacity) {
        RE_Data* new_groups;

        new_groups = (RE_Data*)safe_realloc(state, state->saved_groups,
          new_capacity * sizeof(RE_Data));
        if (!new_groups)
            return FALSE;
        state->saved_groups_capacity = new_capacity;
        state->saved_groups = new_groups;
    }

    memmove(state->saved_groups + state->saved_groups_count, state->data,
      pattern->group_count * sizeof(RE_Data));
    state->saved_groups_count = new_count;

    return TRUE;
}

/* Pops the groups. */
Py_LOCAL(void) pop_groups(RE_State* state) {
    PatternObject* pattern;

    pattern = state->pattern;
    if (pattern->group_count == 0)
        return;

    state->saved_groups_count -= pattern->group_count;
    memmove(state->data, state->saved_groups + state->saved_groups_count,
      pattern->group_count * sizeof(RE_Data));
}

/* Reloads the groups that have been pushed.
 *
 * Equivalent to pop then push.
 */
Py_LOCAL(void) reload_groups(RE_State* state) {
    PatternObject* pattern;

    pattern = state->pattern;
    if (pattern->group_count == 0)
        return;

    memmove(state->data, state->saved_groups + state->saved_groups_count -
      pattern->group_count, pattern->group_count * sizeof(RE_Data));
}

/* Drops the groups that have been pushed. */
Py_LOCAL(void) drop_groups(RE_State* state) {
    state->saved_groups_count -= state->pattern->group_count;
}

/* Initialises the state for a match. */
Py_LOCAL(void) init_match(RE_State* state) {
    Py_ssize_t g;

    state->saved_groups_count = 0;

    /* Reset the backtrack. */
    state->current_block = &state->backtrack_block;
    state->current_block->count = 0;

    state->backtrack = NULL;
    state->search_anchor = state->text_pos;
    state->match_pos = state->text_pos;

    memset(state->data, 0, state->pattern->data_count * sizeof(RE_Data));

    for (g = 0; g < state->pattern->group_count; g++) {
        Py_ssize_t ofs;

        ofs = state->pattern->group_info[g].value_offset;
        state->data[ofs].group.begin = -1;
        state->data[ofs].group.end = -1;
    }
}

/* Adds a new backtrack entry. */
Py_LOCAL(BOOL) add_backtrack(RE_State* state, RE_CODE op) {
    RE_BacktrackBlock* current = state->current_block;

    if (current->count >= current->capacity) {
        if (!current->next) {
            size_t capacity;
            size_t size;
            RE_BacktrackBlock* next;

            capacity = current->capacity * 2;
            size = sizeof(RE_BacktrackBlock) + (capacity -
              RE_BACKTRACK_BLOCK_SIZE) * sizeof(RE_BacktrackData);
            next = (RE_BacktrackBlock*)safe_alloc(state, size);
            if (!next)
                return FALSE;

            next->previous = current;
            next->next = NULL;
            next->capacity = capacity;
            current->next = next;
        }
        current = current->next;
        current->count = 0;
        state->current_block = current;
    }
    state->backtrack = &current->items[current->count++];
    state->backtrack->op = op;

    return TRUE;
}

/* Gets the last backtrack entry.
 *
 * It'll never be called when there are _no_ entries.
 */
Py_LOCAL(RE_BacktrackData*) last_backtrack(RE_State* state) {
    return &state->current_block->items[state->current_block->count - 1];
}

/* Discards the last backtrack entry.
 *
 * It'll never be called to discard the _only_ entry.
 */
Py_LOCAL(void) discard_backtrack(RE_State* state) {
    --state->current_block->count;
    if (state->current_block->count == 0 && state->current_block->previous)
        state->current_block = state->current_block->previous;
}

/* Locates the start node for testing ahead. */
Py_LOCAL(RE_Node*) locate_test_start(RE_Node* node) {
    for (;;) {
        switch (node->op) {
        case RE_OP_BEGIN_GROUP:
        case RE_OP_END_GROUP:
            node = node->next_1.node;
            break;
        case RE_OP_LOOKAROUND:
            node = node->next_1.node;
            break;
        case RE_OP_GREEDY_REPEAT:
        case RE_OP_LAZY_REPEAT:
            if (node->values[1] == 0)
                return node;
            node = node->next_1.node;
            break;
        case RE_OP_GREEDY_REPEAT_ONE:
        case RE_OP_LAZY_REPEAT_ONE:
            if (node->values[1] == 0)
                return node;
            return node->next_2.node;
        default:
            return node;
        }
    }
}

Py_LOCAL(BOOL) try_match(RE_State* state, RE_NextNode* next, Py_ssize_t
  text_pos, RE_Position* next_position);

/* Matches many ANYs. */
Py_LOCAL(Py_ssize_t) match_many_ANY(RE_State* state, RE_Node* node, Py_ssize_t
  text_pos, Py_ssize_t limit, BOOL match) {
    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        if (match) {
            while (text_ptr < limit_ptr && text_ptr[0] != '\n')
                ++text_ptr;
        } else {
            while (text_ptr < limit_ptr && text_ptr[0] == '\n')
                ++text_ptr;
        }

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        if (match) {
            while (text_ptr < limit_ptr && text_ptr[0] != '\n')
                ++text_ptr;
        } else {
            while (text_ptr < limit_ptr && text_ptr[0] == '\n')
                ++text_ptr;
        }

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many ANYs backwards. */
Py_LOCAL(Py_ssize_t) match_many_ANY_REV(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        if (match) {
            while (text_ptr > limit_ptr && text_ptr[-1] != '\n')
                --text_ptr;
        } else {
            while (text_ptr > limit_ptr && text_ptr[-1] == '\n')
                --text_ptr;
        }

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        if (match) {
            while (text_ptr > limit_ptr && text_ptr[-1] != '\n')
                --text_ptr;
        } else {
            while (text_ptr > limit_ptr && text_ptr[-1] == '\n')
                --text_ptr;
        }

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many BIG_BITSETs. */
Py_LOCAL(Py_ssize_t) match_many_BIG_BITSET(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && in_big_bitset(node, text_ptr[0]) ==
          match)
            ++text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && in_big_bitset(node, text_ptr[0]) ==
          match)
            ++text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many BIG_BITSETs backwards. */
Py_LOCAL(Py_ssize_t) match_many_BIG_BITSET_REV(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && in_big_bitset(node, text_ptr[-1]) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && in_big_bitset(node, text_ptr[-1]) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many CHARACTERs. */
Py_LOCAL(Py_ssize_t) match_many_CHARACTER(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    RE_CODE ch;

    ch = node->values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        if (node->match == match) {
            while (text_ptr < limit_ptr && text_ptr[0] == ch)
                ++text_ptr;
        } else {
            while (text_ptr < limit_ptr && text_ptr[0] != ch)
                ++text_ptr;
        }

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        if (node->match == match) {
            while (text_ptr < limit_ptr && text_ptr[0] == ch)
                ++text_ptr;
        } else {
            while (text_ptr < limit_ptr && text_ptr[0] != ch)
                ++text_ptr;
        }

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many CHARACTERs, ignoring case. */
Py_LOCAL(Py_ssize_t) match_many_CHARACTER_IGN(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    BOOL (*same_char_ign_3)(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
      RE_CODE ch2_upper, RE_CODE ch2_title);
    RE_CODE ch;
    RE_CODE ch_lower;
    RE_CODE ch_upper;
    RE_CODE ch_title;

    match = node->match == match;

    same_char_ign_3 = state->encoding->same_char_ign_3;

    ch = node->values[0];
    ch_lower = state->encoding->lower(ch);
    ch_title = ch_upper = state->encoding->upper(ch);
    if (ch_lower != ch_upper)
        ch_title = state->encoding->title(ch);

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && same_char_ign_3(text_ptr[0], ch,
          ch_lower, ch_upper, ch_title) == match)
            ++text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && same_char_ign_3(text_ptr[0], ch,
          ch_lower, ch_upper, ch_title) == match)
            ++text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many CHARACTERs backwards, ignoring case. */
Py_LOCAL(Py_ssize_t) match_many_CHARACTER_IGN_REV(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    BOOL (*same_char_ign_3)(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
      RE_CODE ch2_upper, RE_CODE ch2_title);
    RE_CODE ch;
    RE_CODE ch_lower;
    RE_CODE ch_upper;
    RE_CODE ch_title;

    match = node->match == match;

    same_char_ign_3 = state->encoding->same_char_ign_3;

    ch = node->values[0];
    ch_lower = state->encoding->lower(ch);
    ch_title = ch_upper = state->encoding->upper(ch);
    if (ch_lower != ch_upper)
        ch_title = state->encoding->title(ch);

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && same_char_ign_3(text_ptr[-1], ch,
          ch_lower, ch_upper, ch_title) == match)
            --text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && same_char_ign_3(text_ptr[-1], ch,
          ch_lower, ch_upper, ch_title) == match)
            --text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many CHARACTERs backwards. */
Py_LOCAL(Py_ssize_t) match_many_CHARACTER_REV(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    RE_CODE ch;

    ch = node->values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        if (node->match == match) {
            while (text_ptr > limit_ptr && text_ptr[-1] == ch)
                --text_ptr;
        } else {
            while (text_ptr > limit_ptr && text_ptr[-1] != ch)
                --text_ptr;
        }

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        if (node->match == match) {
            while (text_ptr > limit_ptr && text_ptr[-1] == ch)
                --text_ptr;
        } else {
            while (text_ptr > limit_ptr && text_ptr[-1] != ch)
                --text_ptr;
        }

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many PROPERTYs. */
Py_LOCAL(Py_ssize_t) match_many_PROPERTY(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);
    RE_CODE property;

    match = node->match == match;

    has_property = state->encoding->has_property;

    property = node->values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && has_property(property, text_ptr[0]) ==
          match)
            ++text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && has_property(property, text_ptr[0]) ==
          match)
            ++text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many PROPERTYs backwards. */
Py_LOCAL(Py_ssize_t) match_many_PROPERTY_REV(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);
    RE_CODE property;

    match = node->match == match;

    has_property = state->encoding->has_property;

    property = node->values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && has_property(property, text_ptr[-1]) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && has_property(property, text_ptr[-1]) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many SETs. */
Py_LOCAL(Py_ssize_t) match_many_SET(RE_State* state, RE_Node* node, Py_ssize_t
  text_pos, Py_ssize_t limit, BOOL match) {
    RE_EncodingTable* encoding;

    encoding = state->encoding;

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && in_set(encoding, node, text_ptr[0]) ==
          match)
            ++text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && in_set(encoding, node, text_ptr[0]) ==
          match)
            ++text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many SETs backwards. */
Py_LOCAL(Py_ssize_t) match_many_SET_REV(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    RE_EncodingTable* encoding;

    encoding = state->encoding;

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && in_set(encoding, node, text_ptr[-1]) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && in_set(encoding, node, text_ptr[-1]) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many SMALL_BITSETs. */
Py_LOCAL(Py_ssize_t) match_many_SMALL_BITSET(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && in_small_bitset(node, text_ptr[0]) ==
          match)
            ++text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && in_small_bitset(node, text_ptr[0]) ==
          match)
            ++text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many SMALL_BITSETs backwards. */
Py_LOCAL(Py_ssize_t) match_many_SMALL_BITSET_REV(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    Py_ssize_t start_pos;

    start_pos = text_pos;

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && in_small_bitset(node, text_ptr[-1]) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && in_small_bitset(node, text_ptr[-1]) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Counts a repeated character pattern. */
Py_LOCAL(size_t) count_one(RE_State* state, RE_Node* node, Py_ssize_t text_pos,
  RE_CODE max_count) {
    size_t available;

    if (max_count < 1)
        return 0;

    switch (node->op) {
    case RE_OP_ANY:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_ANY(state, node, text_pos, text_pos + max_count,
          TRUE) - text_pos;
    case RE_OP_ANY_ALL:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return max_count;
    case RE_OP_ANY_ALL_REV:
        available = text_pos - state->slice_start;
        if (max_count > available)
            max_count = available;

        return max_count;
    case RE_OP_ANY_REV:
        available = text_pos - state->slice_start;
        if (max_count > available)
            max_count = available;

        return text_pos - match_many_ANY_REV(state, node, text_pos, text_pos -
          max_count, TRUE);
    case RE_OP_BIG_BITSET:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_BIG_BITSET(state, node, text_pos, text_pos +
          max_count, TRUE) - text_pos;
    case RE_OP_BIG_BITSET_REV:
        available = text_pos - state->slice_start;
        if (max_count > available)
            max_count = available;

        return text_pos - match_many_BIG_BITSET_REV(state, node, text_pos,
          text_pos - max_count, TRUE);
    case RE_OP_CHARACTER:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_CHARACTER(state, node, text_pos, text_pos +
          max_count, TRUE) - text_pos;
    case RE_OP_CHARACTER_IGN:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_CHARACTER_IGN(state, node, text_pos, text_pos +
          max_count, TRUE) - text_pos;
    case RE_OP_CHARACTER_IGN_REV:
        available = text_pos - state->slice_start;
        if (max_count > available)
            max_count = available;

        return text_pos - match_many_CHARACTER_IGN_REV(state, node, text_pos,
          text_pos - max_count, TRUE);
    case RE_OP_CHARACTER_REV:
        available = text_pos - state->slice_start;
        if (max_count > available)
            max_count = available;

        return text_pos - match_many_CHARACTER_REV(state, node, text_pos,
          text_pos - max_count, TRUE);
    case RE_OP_PROPERTY:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_PROPERTY(state, node, text_pos, text_pos + max_count,
          TRUE) - text_pos;
    case RE_OP_PROPERTY_REV:
        available = text_pos - state->slice_start;
        if (max_count > available)
            max_count = available;

        return text_pos - match_many_PROPERTY_REV(state, node, text_pos,
          text_pos - max_count, TRUE);
    case RE_OP_SET:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_SET(state, node, text_pos, text_pos + max_count,
          TRUE) - text_pos;
    case RE_OP_SET_REV:
        available = text_pos - state->slice_start;
        if (max_count > available)
            max_count = available;

        return text_pos - match_many_SET_REV(state, node, text_pos, text_pos -
          max_count, TRUE);
    case RE_OP_SMALL_BITSET:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_SMALL_BITSET(state, node, text_pos, text_pos +
          max_count, TRUE) - text_pos;
    case RE_OP_SMALL_BITSET_REV:
        available = text_pos - state->slice_start;
        if (max_count > available)
            max_count = available;

        return text_pos - match_many_SMALL_BITSET_REV(state, node, text_pos,
          text_pos - max_count, TRUE);
    }

    return 0;
}

/* Tries to match a character pattern. */
Py_LOCAL(BOOL) match_one(RE_State* state, RE_Node* node, Py_ssize_t text_pos) {
    void* text;
    RE_CODE (*char_at)(void* text, Py_ssize_t pos);

    text = state->text;
    char_at = state->char_at;

    switch (node->op) {
    case RE_OP_ANY:
        return text_pos < state->slice_end && char_at(text, text_pos) != '\n';
    case RE_OP_ANY_ALL:
        return text_pos != state->slice_end;
    case RE_OP_ANY_ALL_REV:
        return text_pos != state->slice_start;
    case RE_OP_ANY_REV:
        return text_pos > state->slice_start && char_at(text, text_pos - 1) !=
          '\n';
    case RE_OP_BIG_BITSET:
        return text_pos < state->slice_end && in_big_bitset(node, char_at(text,
          text_pos));
    case RE_OP_BIG_BITSET_REV:
        return text_pos > state->slice_start && in_big_bitset(node,
          char_at(text, text_pos - 1));
    case RE_OP_CHARACTER:
        return text_pos < state->slice_end && (char_at(text, text_pos) ==
          node->values[0]) == node->match;
    case RE_OP_CHARACTER_IGN:
        return text_pos < state->slice_end &&
          state->encoding->same_char_ign(char_at(text, text_pos),
          node->values[0]) == node->match;
    case RE_OP_CHARACTER_IGN_REV:
        return text_pos > state->slice_start &&
          state->encoding->same_char_ign(char_at(text, text_pos - 1),
          node->values[0]) == node->match;
    case RE_OP_CHARACTER_REV:
        return text_pos > state->slice_start && (char_at(text, text_pos - 1) ==
          node->values[0]) == node->match;
    case RE_OP_PROPERTY:
        return text_pos < state->slice_end &&
          state->encoding->has_property(node->values[0], char_at(text,
          text_pos)) == node->match;
    case RE_OP_PROPERTY_REV:
        return text_pos > state->slice_start &&
          state->encoding->has_property(node->values[0], char_at(text, text_pos
          - 1)) == node->match;
    case RE_OP_SET:
        return text_pos < state->slice_end && in_set(state->encoding, node,
          char_at(text, text_pos));
    case RE_OP_SET_REV:
        return text_pos > state->slice_start && in_set(state->encoding, node,
          char_at(text, text_pos - 1));
    case RE_OP_SMALL_BITSET:
        return text_pos < state->slice_end && in_small_bitset(node,
          char_at(text, text_pos));
    case RE_OP_SMALL_BITSET_REV:
        return text_pos > state->slice_start && in_small_bitset(node,
          char_at(text, text_pos - 1));
    }

    return FALSE;
}

/* Performs a simple string search. */
Py_LOCAL(Py_ssize_t) simple_string_search(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    Py_ssize_t length;
    RE_CODE* values;
    RE_CODE first_char;

    text = state->text;

    length = node->value_count;
    values = node->values;

    first_char = values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        for (;;) {
            if (text_ptr[0] == first_char) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = text_ptr[pos] == values[pos];
                if (match)
                    return text_ptr - (RE_UCHAR*)text;
            }

            ++text_ptr;

            if (text_ptr > limit_ptr)
                break;
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        for (;;) {
            if (text_ptr[0] == first_char) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = text_ptr[pos] == values[pos];
                if (match)
                    return text_ptr - (RE_BCHAR*)text;
            }

            ++text_ptr;

            if (text_ptr > limit_ptr)
                break;
        }
    }

    return -1;
}

/* Performs a simple string search, ignoring case. */
Py_LOCAL(Py_ssize_t) simple_string_search_ign(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    Py_ssize_t length;
    RE_CODE* values;
    RE_EncodingTable* encoding;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    BOOL (*same_char_ign_3)(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
      RE_CODE ch2_upper, RE_CODE ch2_title);
    RE_CODE first_char;
    RE_CODE first_lower;
    RE_CODE first_upper;
    RE_CODE first_title;

    text = state->text;

    length = node->value_count;
    values = node->values;

    encoding = state->encoding;
    same_char_ign = encoding->same_char_ign;
    same_char_ign_3 = encoding->same_char_ign_3;

    first_char = values[0];
    first_lower = encoding->lower(first_char);
    first_title = first_upper = encoding->upper(first_char);
    if (first_lower != first_upper)
        first_title = encoding->title(first_char);

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        for (;;) {
            if (same_char_ign_3(text_ptr[0], first_char, first_lower,
              first_upper, first_title)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return text_ptr - (RE_UCHAR*)text;
            }

            ++text_ptr;

            if (text_ptr > limit_ptr)
                break;
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        for (;;) {
            if (same_char_ign_3(text_ptr[0], first_char, first_lower,
              first_upper, first_title)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return text_ptr - (RE_BCHAR*)text;
            }

            ++text_ptr;

            if (text_ptr > limit_ptr)
                break;
        }
    }

    return -1;
}

/* Performs a simple string search backwards, ignoring case. */
Py_LOCAL(Py_ssize_t) simple_string_search_ign_rev(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    Py_ssize_t length;
    RE_CODE* values;
    RE_EncodingTable* encoding;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    BOOL (*same_char_ign_3)(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
      RE_CODE ch2_upper, RE_CODE ch2_title);
    RE_CODE first_char;
    RE_CODE first_lower;
    RE_CODE first_upper;
    RE_CODE first_title;

    text = state->text;

    length = node->value_count;
    values = node->values;

    text_pos -= length;
    limit -= length;

    encoding = state->encoding;
    same_char_ign = encoding->same_char_ign;
    same_char_ign_3 = encoding->same_char_ign_3;

    first_char = values[0];
    first_lower = encoding->lower(first_char);
    first_title = first_upper = encoding->upper(first_char);
    if (first_lower != first_upper)
        first_title = encoding->title(first_char);

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        for (;;) {
            if (same_char_ign_3(text_ptr[0], first_char, first_lower,
              first_upper, first_title)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return (text_ptr + length) - (RE_UCHAR*)text;
            }

            --text_pos;

            if (text_ptr < limit_ptr)
                break;
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        for (;;) {
            if (same_char_ign_3(text_ptr[0], first_char, first_lower,
              first_upper, first_title)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return (text_ptr + length) - (RE_BCHAR*)text;
            }

            --text_pos;

            if (text_ptr < limit_ptr)
                break;
        }
    }

    return -1;
}

/* Performs a simple string search backwards. */
Py_LOCAL(Py_ssize_t) simple_string_search_rev(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    Py_ssize_t length;
    RE_CODE* values;
    RE_CODE first_char;

    text = state->text;

    length = node->value_count;
    values = node->values;

    text_pos -= length;
    limit -= length;

    first_char = values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        for (;;) {
            if (text_ptr[0] == first_char) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = text_ptr[pos] == values[pos];
                if (match)
                    return (text_ptr + length) - (RE_UCHAR*)text;
            }

            --text_pos;

            if (text_ptr < limit_ptr)
                break;
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        for (;;) {
            if (text_ptr[0] == first_char) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = text_ptr[pos] == values[pos];
                if (match)
                    return (text_ptr + length) - (RE_BCHAR*)text;
            }

            --text_pos;

            if (text_ptr < limit_ptr)
                break;
        }
    }

    return -1;
}

/* Performs a Boyer-Moore fast string search. */
Py_LOCAL(Py_ssize_t) fast_string_search(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad_character_offset;
    Py_ssize_t* good_suffix_offset;
    Py_ssize_t last_pos;
    RE_CODE last_char;

    text = state->text;

    length = node->value_count;
    values = node->values;
    good_suffix_offset = node->good_suffix_offset;
    bad_character_offset = node->bad_character_offset;

    last_pos = length - 1;
    last_char = values[last_pos];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        while (text_ptr <= limit_ptr) {
            if (text_ptr[last_pos] == last_char) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = last_pos - 1; match && pos >= 0; pos--)
                    match = text_ptr[pos] == values[pos];
                if (match)
                    return text_ptr - (RE_UCHAR*)text;

                text_ptr += good_suffix_offset[pos + 1];
            } else
                text_ptr += bad_character_offset[text_ptr[last_pos] & 0xFF];
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        while (text_ptr <= limit_ptr) {
            if (text_ptr[last_pos] == last_char) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = last_pos - 1; match && pos >= 0; pos--)
                    match = text_ptr[pos] == values[pos];
                if (match)
                    return text_ptr - (RE_BCHAR*)text;

                text_ptr += good_suffix_offset[pos + 1];
            } else
                text_ptr += bad_character_offset[text_ptr[last_pos]];
        }
    }

    return -1;
}

/* Performs a Boyer-Moore fast string search, ignoring case. */
Py_LOCAL(Py_ssize_t) fast_string_search_ign(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    RE_EncodingTable* encoding;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    BOOL (*same_char_ign_3)(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
      RE_CODE ch2_upper, RE_CODE ch2_title);
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad_character_offset;
    Py_ssize_t* good_suffix_offset;
    Py_ssize_t last_pos;
    RE_CODE last_char;
    RE_CODE last_lower;
    RE_CODE last_upper;
    RE_CODE last_title;

    text = state->text;
    encoding = state->encoding;
    same_char_ign = encoding->same_char_ign;
    same_char_ign_3 = encoding->same_char_ign_3;

    length = node->value_count;
    values = node->values;
    good_suffix_offset = node->good_suffix_offset;
    bad_character_offset = node->bad_character_offset;

    last_pos = length - 1;
    last_char = values[last_pos];
    last_lower = encoding->lower(last_char);
    last_title = last_upper = encoding->upper(last_char);
    if (last_lower != last_upper)
        last_title = encoding->title(last_char);

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        while (text_ptr <= limit_ptr) {
            if (same_char_ign_3(text_ptr[last_pos], last_char, last_lower,
              last_upper, last_title)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = last_pos - 1; match && pos >= 0; pos--)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return text_ptr - (RE_UCHAR*)text;

                text_ptr += good_suffix_offset[pos + 1];
            } else
                text_ptr += bad_character_offset[text_ptr[last_pos] & 0xFF];
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        while (text_ptr <= limit_ptr) {
            if (same_char_ign_3(text_ptr[last_pos], last_char, last_lower,
              last_upper, last_title)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = last_pos - 1; match && pos >= 0; pos--)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return text_ptr - (RE_BCHAR*)text;

                text_ptr += good_suffix_offset[pos + 1];
            } else
                text_ptr += bad_character_offset[text_ptr[last_pos]];
        }
    }

    return -1;
}

/* Performs a Boyer-Moore fast string search backwards, ignoring case. */
Py_LOCAL(Py_ssize_t) fast_string_search_ign_rev(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    RE_EncodingTable* encoding;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    BOOL (*same_char_ign_3)(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
      RE_CODE ch2_upper, RE_CODE ch2_title);
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad_character_offset;
    Py_ssize_t* good_suffix_offset;
    RE_CODE first_char;
    RE_CODE first_lower;
    RE_CODE first_upper;
    RE_CODE first_title;

    text = state->text;
    encoding = state->encoding;
    same_char_ign = encoding->same_char_ign;
    same_char_ign_3 = encoding->same_char_ign_3;

    length = node->value_count;
    values = node->values;
    good_suffix_offset = node->good_suffix_offset;
    bad_character_offset = node->bad_character_offset;

    first_char = values[0];
    first_lower = encoding->lower(first_char);
    first_title = first_upper = encoding->upper(first_char);
    if (first_lower != first_upper)
        first_title = encoding->title(first_char);

    text_pos -= length;
    limit -= length;

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        while (text_ptr >= limit_ptr) {
            if (same_char_ign_3(text_ptr[0], first_char, first_lower,
              first_upper, first_title)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return (text_ptr + length) - (RE_UCHAR*)text;

                text_ptr += good_suffix_offset[pos - 1];
            } else
                text_ptr += bad_character_offset[text_ptr[0] & 0xFF];
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        while (text_ptr >= limit_ptr) {
            if (same_char_ign_3(text_ptr[0], first_char, first_lower,
              first_upper, first_title)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return (text_ptr + length) - (RE_BCHAR*)text;

                text_ptr += good_suffix_offset[pos - 1];
            } else
                text_ptr += bad_character_offset[text_ptr[0]];
        }
    }

    return -1;
}

/* Performs a Boyer-Moore fast string search backwards. */
Py_LOCAL(Py_ssize_t) fast_string_search_rev(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad_character_offset;
    Py_ssize_t* good_suffix_offset;
    RE_CODE first_char;

    text = state->text;

    length = node->value_count;
    values = node->values;
    good_suffix_offset = node->good_suffix_offset;
    bad_character_offset = node->bad_character_offset;

    first_char = values[0];

    text_pos -= length;
    limit -= length;

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        while (text_ptr >= limit_ptr) {
            if (text_ptr[0] == first_char) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = text_ptr[pos] == values[pos];
                if (match)
                    return (text_ptr + length) - (RE_UCHAR*)text;

                text_ptr += good_suffix_offset[pos - 1];
            } else
                text_ptr += bad_character_offset[text_ptr[0] & 0xFF];
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        while (text_ptr >= limit_ptr) {
            if (text_ptr[0] == first_char) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = text_ptr[pos] == values[pos];
                if (match)
                    return (text_ptr + length) - (RE_BCHAR*)text;

                text_ptr += good_suffix_offset[pos - 1];
            } else
                text_ptr += bad_character_offset[text_ptr[0]];
        }
    }

    return -1;
}

/* Performs a string search. */
Py_LOCAL(Py_ssize_t) string_search(RE_State* state, RE_Node* node, Py_ssize_t
  text_pos, Py_ssize_t limit) {
    if (text_pos > limit)
        return -1;

    if (node->bad_character_offset)
        text_pos = fast_string_search(state, node, text_pos, limit);
    else
        text_pos = simple_string_search(state, node, text_pos, limit);

    return text_pos;
}

/* Performs a string search, ignoring case. */
Py_LOCAL(Py_ssize_t) string_search_ign(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    if (text_pos > limit)
        return -1;

    if (node->bad_character_offset)
        text_pos = fast_string_search_ign(state, node, text_pos, limit);
    else
        text_pos = simple_string_search_ign(state, node, text_pos, limit);

    return text_pos;
}

/* Performs a string search backwards, ignoring case. */
Py_LOCAL(Py_ssize_t) string_search_ign_rev(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    if (text_pos < limit)
        return -1;

    if (node->bad_character_offset)
        text_pos = fast_string_search_ign_rev(state, node, text_pos, limit);
    else
        text_pos = simple_string_search_ign_rev(state, node, text_pos, limit);

    return text_pos;
}

/* Performs a string search backwards. */
Py_LOCAL(Py_ssize_t) string_search_rev(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit) {
    if (text_pos < limit)
        return -1;

    if (node->bad_character_offset)
        text_pos = fast_string_search_rev(state, node, text_pos, limit);
    else
        text_pos = simple_string_search_rev(state, node, text_pos, limit);

    return text_pos;
}

/* Tries a match at the current text position.
 *
 * Returns TRUE and the next node and text position if the match succeeds.
 */
Py_LOCAL(BOOL) try_match(RE_State* state, RE_NextNode* next, Py_ssize_t
  text_pos, RE_Position* next_position) {
    RE_Node* test;
    void* text;
    RE_CODE (*char_at)(void* text, Py_ssize_t pos);

    test = next->test;
    text = state->text;
    char_at = state->char_at;

    switch (test->op) {
    case RE_OP_ANY: /* Any character, except a newline. */
        if (text_pos >= state->slice_end || char_at(text, text_pos) == '\n')
            return FALSE;
        break;
    case RE_OP_ANY_ALL: /* Any character at all. */
        if (text_pos >= state->slice_end)
            return FALSE;
        break;
    case RE_OP_ANY_ALL_REV: /* Any character at all. */
        if (text_pos <= state->slice_start)
            return FALSE;
        break;
    case RE_OP_ANY_REV: /* Any character, except a newline. */
        if (text_pos <= state->slice_start || char_at(text, text_pos - 1) ==
          '\n')
            return FALSE;
        break;
    case RE_OP_BIG_BITSET: /* Big bitset. */
        if (text_pos >= state->slice_end || !in_big_bitset(test, char_at(text,
          text_pos)))
            return FALSE;
        break;
    case RE_OP_BIG_BITSET_REV: /* Big bitset. */
        if (text_pos <= state->slice_start || !in_big_bitset(test,
          char_at(text, text_pos - 1)))
            return FALSE;
        break;
    case RE_OP_BOUNDARY: /* At a word boundary. */
        if (state->encoding->at_boundary(state, text_pos) != test->match)
            return FALSE;
        break;
    case RE_OP_BRANCH: /* 2-way branch. */
        if (!try_match(state, &test->next_1, text_pos, next_position) &&
          !try_match(state, &test->next_2, text_pos, next_position))
            return FALSE;
        break;
    case RE_OP_CHARACTER: /* A character literal. */
        if (text_pos >= state->slice_end || (char_at(text, text_pos) ==
          test->values[0]) != test->match)
            return FALSE;
        break;
    case RE_OP_CHARACTER_IGN: /* A character literal, ignoring case. */
        if (text_pos >= state->slice_end ||
          state->encoding->same_char_ign(char_at(text, text_pos),
          test->values[0]) != test->match)
            return FALSE;
        break;
    case RE_OP_CHARACTER_IGN_REV: /* A character literal, ignoring case. */
        if (text_pos <= state->slice_start ||
          state->encoding->same_char_ign(char_at(text, text_pos - 1),
          test->values[0]) != test->match)
            return FALSE;
        break;
    case RE_OP_CHARACTER_REV: /* A character literal. */
        if (text_pos <= state->slice_start || (char_at(text, text_pos - 1) ==
          test->values[0]) != test->match)
            return FALSE;
        break;
    case RE_OP_DEFAULT_BOUNDARY: /* At a default word boundary. */
        if (state->encoding->at_default_boundary(state, text_pos) !=
          test->match)
            return FALSE;
        break;
    case RE_OP_END_OF_LINE: /* At the end of a line. */
        if (text_pos != state->text_length && char_at(text, text_pos) != '\n')
            return FALSE;
        break;
    case RE_OP_END_OF_STRING: /* At the end of the string. */
        if (text_pos != state->text_length)
            return FALSE;
        break;
    case RE_OP_END_OF_STRING_LINE: /* At end of string or final newline. */
        if (text_pos != state->text_length && text_pos != state->final_newline)
            return FALSE;
        break;
    case RE_OP_PROPERTY: /* A character property. */
        /* values are: property */
        if (text_pos >= state->slice_end ||
          state->encoding->has_property(test->values[0], char_at(text,
          text_pos)) != test->match)
            return FALSE;
        break;
    case RE_OP_PROPERTY_REV: /* A character property. */
        /* values are: property */
        if (text_pos <= state->slice_start ||
          state->encoding->has_property(test->values[0], char_at(text, text_pos
          - 1)) != test->match)
            return FALSE;
        break;
    case RE_OP_SEARCH_ANCHOR: /* At the start of the search. */
        if (text_pos != state->search_anchor)
            return FALSE;
        break;
    case RE_OP_SET: /* Character set. */
        if (text_pos >= state->slice_end || !in_set(state->encoding, test,
          char_at(text, text_pos)))
            return FALSE;
        break;
    case RE_OP_SET_REV: /* Character set. */
        if (text_pos <= state->slice_start || !in_set(state->encoding, test,
          char_at(text, text_pos - 1)))
            return FALSE;
        break;
    case RE_OP_SMALL_BITSET: /* Small bitset. */
        if (text_pos >= state->slice_end || !in_small_bitset(test,
          char_at(text, text_pos)))
            return FALSE;
        break;
    case RE_OP_SMALL_BITSET_REV: /* Small bitset. */
        if (text_pos <= state->slice_start || !in_small_bitset(test,
          char_at(text, text_pos - 1)))
            return FALSE;
        break;
    case RE_OP_START_OF_LINE: /* At the start of a line. */
        if (text_pos != 0 && char_at(text, text_pos - 1) != '\n')
            return FALSE;
        break;
    case RE_OP_START_OF_STRING: /* At the start of the string. */
        if (text_pos != 0)
            return FALSE;
        break;
    case RE_OP_STRING: /* A string literal. */
    {
        size_t length;
        size_t available;
        RE_CODE* values;
        size_t i;

        length = test->value_count;
        available = state->slice_end - text_pos;
        if (length > available)
            return FALSE;

        values = test->values;

        if (state->wide) {
            RE_UCHAR* text_ptr;

            text_ptr = (RE_UCHAR*)text + text_pos;

            for (i = 0; i < length; i++) {
                if (text_ptr[i] != values[i])
                    return FALSE;
            }
        } else {
            RE_BCHAR* text_ptr;

            text_ptr = (RE_BCHAR*)text + text_pos;

            for (i = 0; i < length; i++) {
                if (text_ptr[i] != values[i])
                    return FALSE;
            }
        }
        break;
    }
    case RE_OP_STRING_IGN: /* A string literal, ignoring case. */
    {
        size_t length;
        size_t available;
        BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
        RE_CODE* values;
        size_t i;

        length = test->value_count;
        available = state->slice_end - text_pos;
        if (length > available)
            return FALSE;

        same_char_ign = state->encoding->same_char_ign;

        values = test->values;

        if (state->wide) {
            RE_UCHAR* text_ptr;

            text_ptr = (RE_UCHAR*)text + text_pos;

            for (i = 0; i < length; i++) {
                if (!same_char_ign(text_ptr[i], values[i]))
                    return FALSE;
            }
        } else {
            RE_BCHAR* text_ptr;

            text_ptr = (RE_BCHAR*)text + text_pos;

            for (i = 0; i < length; i++) {
                if (!same_char_ign(text_ptr[i], values[i]))
                    return FALSE;
            }
        }
        break;
    }
    case RE_OP_STRING_IGN_REV: /* A string literal, ignoring case. */
    {
        size_t length;
        size_t available;
        BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
        RE_CODE* values;
        size_t i;

        length = test->value_count;
        available = text_pos - state->slice_start;
        if (length > available)
            return FALSE;

        same_char_ign = state->encoding->same_char_ign;

        values = test->values;

        if (state->wide) {
            RE_UCHAR* text_ptr;

            text_ptr = (RE_UCHAR*)text + text_pos - length;

            for (i = 0; i < length; i++) {
                if (!same_char_ign(text_ptr[i], values[i]))
                    return FALSE;
            }
        } else {
            RE_BCHAR* text_ptr;

            text_ptr = (RE_BCHAR*)text + text_pos - length;

            for (i = 0; i < length; i++) {
                if (!same_char_ign(text_ptr[i], values[i]))
                    return FALSE;
            }
        }
        break;
    }
    case RE_OP_STRING_REV: /* A string literal. */
    {
        size_t length;
        size_t available;
        RE_CODE* values;
        size_t i;

        length = test->value_count;
        available = text_pos - state->slice_start;
        if (length > available)
            return FALSE;

        values = test->values;

        if (state->wide) {
            RE_UCHAR* text_ptr;

            text_ptr = (RE_UCHAR*)text + text_pos - length;

            for (i = 0; i < length; i++) {
                if (text_ptr[i] != values[i])
                    return FALSE;
            }
        } else {
            RE_BCHAR* text_ptr;

            text_ptr = (RE_BCHAR*)text + text_pos - length;

            for (i = 0; i < length; i++) {
                if (text_ptr[i] != values[i])
                    return FALSE;
            }
        }
        break;
    }
    }

    next_position->node = next->match_next;
    next_position->text_pos = text_pos + next->match_step;

    return TRUE;
}

/* Performs a general check. */
Py_LOCAL(BOOL) general_check(RE_State* state, RE_Node* node, Py_ssize_t
  text_pos) {
    for (;;) {
        switch (node->op) {
        case RE_OP_BRANCH:
            if (general_check(state, node->next_1.node, text_pos))
                return TRUE;
            node = node->next_2.node;
            break;
        case RE_OP_CHARACTER:
            text_pos = match_many_CHARACTER(state, node, text_pos,
              state->slice_end, FALSE);
            return text_pos < state->slice_end;
        case RE_OP_CHARACTER_IGN:
            text_pos = match_many_CHARACTER_IGN(state, node, text_pos,
              state->slice_end, FALSE);
            return text_pos < state->slice_end;
        case RE_OP_CHARACTER_IGN_REV:
            text_pos = match_many_CHARACTER_IGN_REV(state, node, text_pos,
              state->slice_start, FALSE);
            return text_pos > state->slice_start;
        case RE_OP_CHARACTER_REV:
            text_pos = match_many_CHARACTER_REV(state, node, text_pos,
              state->slice_start, FALSE);
            return text_pos > state->slice_start;
        case RE_OP_END_GREEDY_REPEAT:
        case RE_OP_END_LAZY_REPEAT:
            node = node->next_2.node;
            break;
        case RE_OP_GREEDY_REPEAT:
        case RE_OP_LAZY_REPEAT:
            if (node->values[1] >= 1)
                node = node->next_1.node;
            else
                node = node->next_2.node;
            break;
        case RE_OP_GROUP_EXISTS:
            if (general_check(state, node->next_1.node, text_pos))
                return TRUE;
            node = node->next_2.node;
            break;
        case RE_OP_STRING:
            text_pos = string_search(state, node, text_pos, state->slice_end -
              node->value_count);
            return text_pos >= 0;
        case RE_OP_STRING_IGN:
            text_pos = string_search_ign(state, node, text_pos,
              state->slice_end - node->value_count);
            return text_pos >= 0;
        case RE_OP_STRING_IGN_REV:
            text_pos = string_search_ign_rev(state, node, text_pos,
              state->slice_start + node->value_count);
            return text_pos >= 0;
        case RE_OP_STRING_REV:
            text_pos = string_search_rev(state, node, text_pos,
              state->slice_start + node->value_count);
            return text_pos >= 0;
        case RE_OP_SUCCESS:
            return TRUE;
        default:
            node = node->next_1.node;
            break;
        }
    }
}

/* Searches for the start of a match. */
Py_LOCAL(BOOL) search_start(RE_State* state, RE_NextNode* next, RE_Position*
  new_position) {
    Py_ssize_t text_pos;
    RE_Node* test;
    RE_Node* node;
    Py_ssize_t start_pos;
    Py_ssize_t step;
    Py_ssize_t limit;

    start_pos = state->text_pos;

    test = next->test;
    node = next->node;

    if (state->reverse) {
        step = -1;
        limit = state->slice_start + state->min_width;
    } else {
        step = 1;
        limit = state->slice_end - state->min_width;
    }

again:
    if (state->reverse) {
        if (start_pos < limit)
            return FALSE;
    } else {
        if (start_pos > limit)
            return FALSE;
    }

    switch (test->op) {
    case RE_OP_ANY: /* Any character, except a newline. */
        start_pos = match_many_ANY(state, node, start_pos, limit + 1, FALSE);
        if (start_pos > limit)
            return FALSE;
        break;
    case RE_OP_ANY_ALL: /* Any character at all. */
        break;
    case RE_OP_ANY_ALL_REV: /* Any character at all backwards. */
        break;
    case RE_OP_ANY_REV: /* Any character backwards, except a newline. */
        start_pos = match_many_ANY_REV(state, node, start_pos, limit - 1,
          FALSE);
        if (start_pos < limit)
            return FALSE;
        break;
    case RE_OP_BIG_BITSET: /* Big bitset. */
        start_pos = match_many_BIG_BITSET(state, test, start_pos, limit + 1,
          FALSE);
        if (start_pos > limit)
            return FALSE;
        break;
    case RE_OP_BIG_BITSET_REV: /* Big bitset backwards. */
        start_pos = match_many_BIG_BITSET_REV(state, test, start_pos, limit -
          1, FALSE);
        if (start_pos < limit)
            return FALSE;
        break;
    case RE_OP_BOUNDARY: /* At a word boundary. */
    {
        BOOL match;
        Py_ssize_t step;
        BOOL (*at_boundary)(RE_State* state, Py_ssize_t start_pos);

        match = test->match;
        step = state->reverse ? -1 : 1;
        at_boundary = state->encoding->at_boundary;

        for (;;) {
            if (at_boundary(state, start_pos) == match)
                break;
            if (start_pos == limit)
                return FALSE;
            start_pos += step;
        }
        break;
    }
    case RE_OP_CHARACTER: /* A character literal. */
        start_pos = match_many_CHARACTER(state, test, start_pos, limit + 1,
          FALSE);
        if (start_pos > limit)
            return FALSE;
        break;
    case RE_OP_CHARACTER_IGN: /* A character literal, ignoring case. */
        limit = state->slice_end - state->min_width + 1;
        start_pos = match_many_CHARACTER_IGN(state, test, start_pos, limit + 1,
          FALSE);
        if (start_pos > limit)
            return FALSE;
        break;
    case RE_OP_CHARACTER_IGN_REV: /* A character literal backwards, ignoring case. */
        limit = state->slice_start + state->min_width - 1;
        start_pos = match_many_CHARACTER_IGN_REV(state, test, start_pos, limit
          - 1, FALSE);
        if (start_pos < limit)
            return FALSE;
        break;
    case RE_OP_CHARACTER_REV: /* A character literal backwards. */
        limit = state->slice_start + state->min_width - 1;
        start_pos = match_many_CHARACTER_REV(state, test, start_pos, limit - 1,
          FALSE);
        if (start_pos < limit)
            return FALSE;
        break;
    case RE_OP_DEFAULT_BOUNDARY: /* At a default word boundary. */
    {
        BOOL match;
        Py_ssize_t step;
        BOOL (*at_default_boundary)(RE_State* state, Py_ssize_t start_pos);

        match = test->match;
        step = state->reverse ? -1 : 1;
        at_default_boundary = state->encoding->at_default_boundary;

        for (;;) {
            if (at_default_boundary(state, start_pos) == match)
                break;
            if (start_pos == limit)
                return FALSE;
            start_pos += step;
        }
        break;
    }
    case RE_OP_END_OF_LINE: /* At the end of a line. */
    {
        Py_ssize_t step;

        step = state->reverse ? -1 : 1;

        if (state->wide) {
            RE_UCHAR* text_ptr;
            RE_UCHAR* limit_ptr;
            RE_UCHAR* end_ptr;

            text_ptr = (RE_UCHAR*)state->text + start_pos;
            limit_ptr = (RE_UCHAR*)state->text + limit;
            end_ptr = (RE_UCHAR*)state->text + state->text_length;

            for (;;) {
                if (text_ptr == end_ptr || text_ptr[0] == '\n')
                    break;
                if (text_ptr == limit_ptr)
                    return FALSE;
                text_ptr += step;
            }

            start_pos = text_ptr - (RE_UCHAR*)state->text;
        } else {
            RE_BCHAR* text_ptr;
            RE_BCHAR* limit_ptr;
            RE_BCHAR* end_ptr;

            text_ptr = (RE_BCHAR*)state->text + start_pos;
            limit_ptr = (RE_BCHAR*)state->text + limit;
            end_ptr = (RE_BCHAR*)state->text + state->text_length;

            for (;;) {
                if (text_ptr == end_ptr || text_ptr[0] == '\n')
                    break;
                if (text_ptr == limit_ptr)
                    return FALSE;
                text_ptr += step;
            }

            start_pos = text_ptr - (RE_BCHAR*)state->text;
        }
        break;
    }
    case RE_OP_END_OF_STRING: /* At the end of the string. */
        if (state->reverse) {
            if (start_pos != state->text_length)
                return FALSE;
        } else {
            if (state->slice_end != state->text_length)
                return FALSE;
        }

        start_pos = state->text_length;
        break;
    case RE_OP_END_OF_STRING_LINE: /* At end of string or final newline. */
        if (state->reverse) {
            if (start_pos >= state->text_length)
                start_pos = state->text_length;
            else if (start_pos >= state->final_newline)
                start_pos = state->final_newline;
            else
                return FALSE;

            if (start_pos < state->slice_start)
                return FALSE;
        } else {
            if (start_pos <= state->final_newline)
                start_pos = state->final_newline;
            else if (start_pos <= state->text_length)
                start_pos = state->text_length;
            else
                return FALSE;

            if (start_pos > state->slice_end)
                return FALSE;
        }
        break;
    case RE_OP_PROPERTY: /* A character property. */
        start_pos = match_many_PROPERTY(state, test, start_pos, limit + 1,
          FALSE);
        if (start_pos > limit)
            return FALSE;
        break;
    case RE_OP_PROPERTY_REV: /* A character property backwards. */
        start_pos = match_many_PROPERTY_REV(state, test, start_pos, limit - 1,
          FALSE);
        if (start_pos < limit)
            return FALSE;
        break;
    case RE_OP_SEARCH_ANCHOR: /* At the start of the search. */
        if (state->reverse) {
            if (start_pos < state->search_anchor)
                return FALSE;
        } else {
            if (start_pos > state->search_anchor)
                return FALSE;
        }

        start_pos = state->search_anchor;
        break;
    case RE_OP_SET: /* A set. */
        start_pos = match_many_SET(state, test, start_pos, limit + 1, FALSE);
        if (start_pos > limit)
            return FALSE;
        break;
    case RE_OP_SET_REV: /* A set backwards. */
        start_pos = match_many_SET_REV(state, test, start_pos, limit - 1,
          FALSE);
        if (start_pos < limit)
            return FALSE;
        break;
    case RE_OP_SMALL_BITSET: /* Small bitset. */
        start_pos = match_many_SMALL_BITSET(state, test, start_pos, limit + 1,
          FALSE);
        if (start_pos > limit)
            return FALSE;
        break;
    case RE_OP_SMALL_BITSET_REV: /* Small bitset backwards. */
        start_pos = match_many_SMALL_BITSET_REV(state, test, start_pos, limit -
          1, FALSE);
        if (start_pos < limit)
            return FALSE;
        break;
    case RE_OP_START_OF_LINE: /* At the start of a line. */
    {
        Py_ssize_t step;

        step = state->reverse ? -1 : 1;

        if (state->wide) {
            RE_UCHAR* text_ptr;
            RE_UCHAR* limit_ptr;
            RE_UCHAR* start_ptr;

            text_ptr = (RE_UCHAR*)state->text + start_pos;
            limit_ptr = (RE_UCHAR*)state->text + limit;
            start_ptr = (RE_UCHAR*)state->text;

            for (;;) {
                if (text_ptr == start_ptr || text_ptr[-1] == '\n')
                    break;
                if (text_ptr == limit_ptr)
                    return FALSE;
                text_ptr += step;
            }

            start_pos = text_ptr - (RE_UCHAR*)state->text;
        } else {
            RE_BCHAR* text_ptr;
            RE_BCHAR* limit_ptr;
            RE_BCHAR* start_ptr;

            text_ptr = (RE_BCHAR*)state->text + start_pos;
            limit_ptr = (RE_BCHAR*)state->text + limit;
            start_ptr = (RE_BCHAR*)state->text;

            for (;;) {
                if (text_ptr == start_ptr || text_ptr[-1] == '\n')
                    break;
                if (text_ptr == limit_ptr)
                    return FALSE;
                text_ptr += step;
            }

            start_pos = text_ptr - (RE_BCHAR*)state->text;
        }
        break;
    }
    case RE_OP_START_OF_STRING: /* At the start of the string. */
        if (state->reverse) {
            if (state->slice_start != 0)
                return FALSE;
        } else {
            if (start_pos != 0)
                return FALSE;
        }

        start_pos = 0;
        break;
    case RE_OP_STRING: /* A string literal. */
        start_pos = string_search(state, test, start_pos, limit);
        if (start_pos < 0)
            return FALSE;
        break;
    case RE_OP_STRING_IGN: /* A string literal, ignoring case. */
        start_pos = string_search_ign(state, test, start_pos, limit);
        if (start_pos < 0)
            return FALSE;
        break;
    case RE_OP_STRING_IGN_REV: /* A string literal backwards, ignoring case. */
        start_pos = string_search_ign_rev(state, test, start_pos, limit);
        if (start_pos < 0)
            return FALSE;
        break;
    case RE_OP_STRING_REV: /* A string literal backwards. */
        start_pos = string_search_rev(state, test, start_pos, limit);
        if (start_pos < 0)
            return FALSE;
        break;
    default:
        state->match_pos = start_pos;
        new_position->node = node;
        new_position->text_pos = start_pos;
        return TRUE;
    }

    text_pos = start_pos;

    /* Can we look further ahead? */
    if (test == node) {
        text_pos += test->step;

        if (test->next_1.node && !try_match(state, &test->next_1, text_pos,
          new_position)) {
            start_pos += step;
            goto again;
        }
    } else {
        new_position->node = node;
        new_position->text_pos = start_pos;
    }

    /* It's a possible match. */
    state->match_pos = start_pos;

    return TRUE;
}

/* Performs a depth-first match or search from the context. */
Py_LOCAL(int) basic_match(RE_State* state, RE_Node* start_node, BOOL search) {
    Py_ssize_t slice_start;
    Py_ssize_t slice_end;
    Py_ssize_t text_pos;
    RE_EncodingTable* encoding;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    BOOL (*same_char_ign_3)(RE_CODE ch1, RE_CODE ch2, RE_CODE ch2_lower,
      RE_CODE ch2_upper, RE_CODE ch2_title);
    BOOL (*at_boundary)(RE_State* state, Py_ssize_t text_pos);
    BOOL (*at_default_boundary)(RE_State* state, Py_ssize_t text_pos);
    PatternObject* pattern;
    Py_ssize_t text_length;
    RE_NextNode start_pair;
    size_t iterations;
    Py_ssize_t final_newline;
    void* text;
    RE_CODE (*char_at)(void* text, Py_ssize_t pos);
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);
    RE_GroupInfo* group_info;
    RE_RepeatInfo* repeat_info;
    Py_ssize_t step;
    RE_Node* node;
    TRACE(("<<basic_match>>\n"))

    slice_start = state->slice_start;
    slice_end = state->slice_end;
    text_pos = state->text_pos;

    encoding = state->encoding;
    same_char_ign = encoding->same_char_ign;
    same_char_ign_3 = encoding->same_char_ign_3;
    at_boundary = encoding->at_boundary;
    at_default_boundary = encoding->at_default_boundary;

    pattern = state->pattern;

    text_length = state->text_length;

    /* Look beyond any initial group node. */
    start_pair.node = start_node;
    start_pair.test = locate_test_start(start_node);

    /* Is the pattern anchored to the start or end of the string? */
    switch (start_pair.test->op) {
    case RE_OP_START_OF_STRING:
        /* The pattern can be successful only at the start of the string. */
        if (!state->reverse && text_pos != 0)
            return RE_ERROR_FAILURE;

        /* Don't bother to search further because it's anchored. */
        search = FALSE;
        break;
    case RE_OP_END_OF_STRING:
        /* The pattern can be successful only at the end of the string. */
        if (state->reverse && text_pos != text_length)
            return RE_ERROR_FAILURE;

        /* Don't bother to search further because it's anchored. */
        search = FALSE;
        break;
    }

    iterations = 0;
    final_newline = state->final_newline;
    text = state->text;
    char_at = state->char_at;
    has_property = encoding->has_property;
    group_info = pattern->group_info;
    repeat_info = pattern->repeat_info;
    step = state->reverse ? -1 : 1;

    /* Save the groups in case we need to restore them for searching. */
    if (!push_groups(state))
        return RE_ERROR_MEMORY;

    /* Add a backtrack entry for failure. */
    if (!add_backtrack(state, RE_OP_FAILURE))
        return RE_ERROR_MEMORY;

start_match:
    /* If we're searching, advance along the string until there could be a
     * match.
     */
    if (search) {
        RE_Position new_position;

next_match:
        if (!search_start(state, &start_pair, &new_position))
            return RE_ERROR_FAILURE;

        node = new_position.node;
        text_pos = new_position.text_pos;

        if (node->op == RE_OP_SUCCESS) {
            /* Must the match advance past its start? */
            if (text_pos != state->search_anchor || !state->must_advance) {
                state->text_pos = text_pos;
                return RE_ERROR_SUCCESS;
            }

            state->text_pos = state->match_pos + step;
            goto next_match;
        }
    } else
        node = start_node;

advance:
    /* The main matching loop. */
    for (;;) {
        TRACE(("%d|", text_pos))

        switch (node->op) {
        case RE_OP_ANY: /* Any character, except a newline. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos >= slice_end || char_at(text, text_pos) == '\n')
                goto backtrack;
            ++text_pos;
            node = node->next_1.node;
            break;
        case RE_OP_ANY_ALL: /* Any character at all. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos >= slice_end)
                goto backtrack;
            ++text_pos;
            node = node->next_1.node;
            break;
        case RE_OP_ANY_ALL_REV: /* Any character at all. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos <= slice_start)
                goto backtrack;
            --text_pos;
            node = node->next_1.node;
            break;
        case RE_OP_ANY_REV: /* Any character, except a newline. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos <= slice_start || char_at(text, text_pos - 1) == '\n')
                goto backtrack;
            --text_pos;
            node = node->next_1.node;
            break;
        case RE_OP_ATOMIC: /* Atomic subpattern. */
        {
            RE_BacktrackBlock* current_block;
            size_t backtrack_count;
            size_t saved_groups_count;
            BOOL must_advance;
            int status;
            TRACE(("%s\n", re_op_text[node->op]))

            /* Try to match the subpattern. */
            current_block = state->current_block;
            backtrack_count = current_block->count;
            saved_groups_count = state->saved_groups_count;
            must_advance = state->must_advance;
            state->text_pos = text_pos;
            state->must_advance = FALSE;

            status = basic_match(state, node->next_2.node, FALSE);

            if (status < 0)
                return status;

            state->must_advance = must_advance;
            state->saved_groups_count = saved_groups_count;
            current_block->count = backtrack_count;
            state->current_block = current_block;

            if (status != RE_ERROR_SUCCESS)
                goto backtrack;

            node = node->next_1.node;
            text_pos = state->text_pos;
            break;
        }
        case RE_OP_BEGIN_GROUP: /* Beginning of capture group. */
        {
            Py_ssize_t ofs;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the group value in the context. Capture group
             * indexes are 1-based (excluding group 0, which is the entire
             * matched string).
             */
            ofs = group_info[node->values[0] - 1].value_offset;
            state->data[ofs].group.begin = text_pos;
            node = node->next_1.node;
            break;
        }
        case RE_OP_BIG_BITSET: /* Big bitset. */
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (text_pos >= slice_end || !in_big_bitset(node, char_at(text,
              text_pos)))
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_BIG_BITSET_REV: /* Big bitset. */
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (text_pos <= slice_start || !in_big_bitset(node, char_at(text,
              text_pos - 1)))
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_BOUNDARY: /* At a word boundary. */
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (at_boundary(state, text_pos) != node->match)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_BRANCH: /* 2-way branch. */
        {
            BOOL try_first;
            RE_Position next_first_position;
            BOOL try_second;
            RE_Position next_second_position;

            try_first = try_match(state, &node->next_1, text_pos,
              &next_first_position);
            try_second = try_match(state, &node->next_2, text_pos,
              &next_second_position);
            if (try_first) {
                if (try_second) {
                    if (!add_backtrack(state, RE_OP_BRANCH) ||
                      !push_groups(state))
                        return RE_ERROR_MEMORY;
                    state->backtrack->branch.position = next_second_position;
                }
                node = next_first_position.node;
                text_pos = next_first_position.text_pos;
            } else {
                if (try_second) {
                    node = next_second_position.node;
                    text_pos = next_second_position.text_pos;
                } else
                    goto backtrack;
            }
            break;
        }
        case RE_OP_CHARACTER: /* A character literal. */
            TRACE(("%s %d %d\n", re_op_text[node->op], node->match,
              node->values[0]))
            if (text_pos >= slice_end || (char_at(text, text_pos) ==
              node->values[0]) != node->match)
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_CHARACTER_IGN: /* A character literal, ignoring case. */
            TRACE(("%s %d %d\n", re_op_text[node->op], node->match,
              node->values[0]))
            if (text_pos >= slice_end || same_char_ign(char_at(text, text_pos),
              node->values[0]) != node->match)
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_CHARACTER_IGN_REV: /* A character literal, ignoring case. */
            TRACE(("%s %d %d\n", re_op_text[node->op], node->match,
              node->values[0]))
            if (text_pos <= slice_start || same_char_ign(char_at(text, text_pos
              - 1), node->values[0]) != node->match)
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_CHARACTER_REV: /* A character literal. */
            TRACE(("%s %d %d\n", re_op_text[node->op], node->match,
              node->values[0]))
            if (text_pos <= slice_start || (char_at(text, text_pos - 1) ==
              node->values[0]) != node->match)
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_DEFAULT_BOUNDARY: /* At a default word boundary. */
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (at_default_boundary(state, text_pos) != node->match)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_END_GREEDY_REPEAT: /* End of a greedy repeat. */
        {
            Py_ssize_t ofs;
            RE_RepeatData* rp_data;
            BOOL try_body;
            RE_Position next_body_position;
            BOOL try_tail;
            RE_Position next_tail_position;
            RE_BacktrackData* bt_data;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the repeat values in the context. Repeat
             * indexes are 0-based.
             */
            ofs = repeat_info[node->values[0]].value_offset;
            rp_data = &state->data[ofs].repeat;

            /* Matched the body again. */
            ++rp_data->count;

            /* Could the body or tail match?
             *
             * We won't try to match the body again if it didn't consume any
             * characters this time because that could stop us from advancing
             * through the text.
             */
            try_body = rp_data->count < rp_data->max_count && try_match(state,
              &node->next_1, text_pos, &next_body_position);
            try_tail = rp_data->count >= node->values[1] && try_match(state,
              &node->next_2, text_pos, &next_tail_position);
            if (!try_body && !try_tail) {
                /* Neither the body nor the tail could match. */
                --rp_data->count;
                goto backtrack;
            }

            if (try_body) {
                if (try_tail) {
                    /* Both the body and the tail could match, but the body
                     * takes precedence. If the body fails to match then we
                     * want to try the tail before backtracking into the body.
                     */

                    /* Record backtracking info for backtracking into the body.
                     */
                    bt_data = last_backtrack(state);
                    if (bt_data->op == RE_OP_END_GREEDY_REPEAT &&
                      !bt_data->repeat.position.node && bt_data->repeat.ofs ==
                      ofs) {
                        /* The last backtrack entry is for backtracking into
                         * the body like we want to do now, so we can save work
                         * by just re-using it.
                         */
                    } else {
                        if (!add_backtrack(state, RE_OP_END_GREEDY_REPEAT))
                            return RE_ERROR_MEMORY;
                        bt_data = state->backtrack;
                        bt_data->repeat.position.node = NULL; /* Restore then backtrack. */
                        bt_data->repeat.ofs = ofs;
                    }
                    bt_data->repeat.count = rp_data->count - 1;
                    bt_data->repeat.max_count = rp_data->max_count;

                    /* Record backtracking info for matching the tail. */
                    if (!add_backtrack(state, RE_OP_END_GREEDY_REPEAT) ||
                      !push_groups(state))
                        return RE_ERROR_MEMORY;
                    bt_data = state->backtrack;
                    bt_data->repeat.position = next_tail_position;
                    bt_data->repeat.ofs = ofs;
                    bt_data->repeat.count = rp_data->count;
                    bt_data->repeat.max_count = rp_data->max_count;
                } else {
                    /* Only the body could match. If the body fails to match
                     * then we want to backtrack into the body.
                     */

                    /* Record backtracking info for backtracking into the body.
                     */
                    bt_data = last_backtrack(state);
                    if (bt_data->op == RE_OP_END_GREEDY_REPEAT &&
                      !bt_data->repeat.position.node && bt_data->repeat.ofs ==
                      ofs) {
                        /* The last backtrack entry is for backtracking into
                         * the body like we want to do now, so we can save work
                         * by just re-using it.
                         */
                    } else {
                        if (!add_backtrack(state, RE_OP_END_GREEDY_REPEAT))
                            return RE_ERROR_MEMORY;
                        bt_data = state->backtrack;
                        bt_data->repeat.position.node = NULL; /* Restore then backtrack. */
                        bt_data->repeat.ofs = ofs;
                    }
                    bt_data->repeat.count = rp_data->count - 1;
                    bt_data->repeat.max_count = rp_data->max_count;
                }

                /* Advance into the body. */
                node = next_body_position.node;
                text_pos = next_body_position.text_pos;
            } else {
                /* Only the tail could match. If the tail fails to match then
                 * we want to backtrack into the body.
                 */

                /* Advance into the tail. */
                node = next_tail_position.node;
                text_pos = next_tail_position.text_pos;
            }
            break;
        }
        case RE_OP_END_GROUP: /* End of a capture group. */
        {
            Py_ssize_t ofs;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the group value in the context. Capture group
             * indexes are 1-based (excluding group 0, which is the entire
             * matched string).
             */
            ofs = group_info[node->values[0] - 1].value_offset;
            state->data[ofs].group.end = text_pos;
            node = node->next_1.node;
            break;
        }
        case RE_OP_END_LAZY_REPEAT: /* End of a lazy repeat. */
        {
            Py_ssize_t ofs;
            RE_RepeatData* rp_data;
            BOOL try_body;
            BOOL try_tail;
            RE_BacktrackData* bt_data;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the repeat values in the context. Repeat
             * indexes are 0-based.
             */
            ofs = repeat_info[node->values[0]].value_offset;
            rp_data = &state->data[ofs].repeat;

            /* Matched the body again. */
            ++rp_data->count;

            /* Could the body or tail match?
             *
             * We won't try to match the body again if it didn't consume any
             * characters this time because that could stop us from advancing
             * through the text.
             */
            try_body = rp_data->count < rp_data->max_count;
            try_tail = rp_data->count >= node->values[1];
            if (try_body) {
                if (try_tail) {
                    /* Both the body and the tail could match, but the tail
                     * takes preference. If the tail fails to match then we
                     * want to try the body again before backtracking into the
                     * head.
                     */

                    /* Record backtracking info for backtracking into the body.
                     */
                    if (!add_backtrack(state, RE_OP_END_LAZY_REPEAT) ||
                      !push_groups(state))
                        return RE_ERROR_MEMORY;
                    bt_data = state->backtrack;
                    bt_data->repeat.ofs = ofs;
                    bt_data->repeat.position.node = node->next_1.node;
                    bt_data->repeat.position.text_pos = text_pos;

                    /* Advance into the tail. */
                    node = node->next_2.node;
                } else
                    /* Only the body could match. */

                    /* Advance into the body. */
                    node = node->next_1.node;
            } else
                /* Only the tail could match. */

                /* Advance into the tail. */
                node = node->next_2.node;
            break;
        }
        case RE_OP_END_OF_LINE: /* At the end of a line. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos != text_length && char_at(text, text_pos) != '\n')
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_END_OF_STRING: /* At the end of the string. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos != text_length)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_END_OF_STRING_LINE: /* At end of string or final newline. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos != text_length && text_pos != final_newline)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_GREEDY_REPEAT: /* Greedy repeat. */
        {
            Py_ssize_t ofs;
            RE_RepeatData* rp_data;
            size_t max_count;
            BOOL try_tail;
            RE_Position next_tail_position;
            RE_BacktrackData* bt_data;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the repeat values in the context. Repeat
             * indexes are 0-based.
             */
            ofs = repeat_info[node->values[0]].value_offset;
            rp_data = &state->data[ofs].repeat;

            /* We might need to backtrack into the head. */
            if (!add_backtrack(state, RE_OP_GREEDY_REPEAT) ||
              !push_groups(state))
                return RE_ERROR_MEMORY;
            bt_data = state->backtrack;
            bt_data->repeat.ofs = ofs;
            bt_data->repeat.count = rp_data->count;
            bt_data->repeat.max_count = rp_data->max_count;

            max_count = node->values[3] ? slice_end - text_pos : text_pos -
              slice_start;
            if (max_count > node->values[2])
                max_count = node->values[2];

            rp_data->count = 0;
            rp_data->max_count = max_count;

            /* Does the body have to match at all? */
            try_tail = node->values[1] == 0 && try_match(state, &node->next_2,
              text_pos, &next_tail_position);
            if (try_tail) {
                /* The body doesn't have to match, but it takes precedence over
                 * the tail. If the body fails to match then we want to try the
                 * tail before backtracking into the head.
                 */

                /* Record backtracking info for matching the tail. */
                if (!add_backtrack(state, RE_OP_END_GREEDY_REPEAT) ||
                  !push_groups(state))
                    return RE_ERROR_MEMORY;
                bt_data = state->backtrack;
                bt_data->repeat.position = next_tail_position;
                bt_data->repeat.ofs = ofs;
                bt_data->repeat.count = rp_data->count;
                bt_data->repeat.max_count = rp_data->max_count;
            }

            /* Advance into the body. */
            node = node->next_1.node;
            break;
        }
        case RE_OP_GREEDY_REPEAT_ONE: /* Greedy repeat for one character. */
        {
            size_t count;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Count how many times the character repeats, up to the maximum.
             */
            count = count_one(state, node->next_2.node, text_pos,
              node->values[2]);

            /* Have we matched at least the minimum? */
            if (count < node->values[1])
                goto backtrack;

            if (count > node->values[1]) {
                /* The match is longer than the minimum, so we might need to
                 * backtrack the repeat to consume less.
                 */
                Py_ssize_t ofs;
                RE_RepeatData* rp_data;
                RE_BacktrackData* bt_data;

                /* Get the offset to the repeat values in the context. Repeat
                 * indexes are 0-based.
                 */
                ofs = repeat_info[node->values[0]].value_offset;
                rp_data = &state->data[ofs].repeat;
                if (!add_backtrack(state, RE_OP_GREEDY_REPEAT_ONE) ||
                  !push_groups(state))
                    return RE_ERROR_MEMORY;
                bt_data = state->backtrack;
                bt_data->repeat.position.node = node;
                bt_data->repeat.ofs = ofs;
                bt_data->repeat.start = rp_data->start;
                bt_data->repeat.count = rp_data->count;

                rp_data->start = text_pos;
                rp_data->count = count;
            }

            text_pos += (Py_ssize_t)count * node->step;
            node = node->next_1.node;
            break;
        }
        case RE_OP_GROUP_EXISTS: /* Capture group exists. */
        {
            Py_ssize_t ofs;
            Py_ssize_t begin_group;
            Py_ssize_t end_group;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the group value in the context. Capture group
             * indexes are 1-based (excluding group 0, which is the entire
             * matched string).
             */
            ofs = group_info[node->values[0] - 1].value_offset;

            /* Check whether the captured text, if any, exists at this position
             * in the string.
             */
            begin_group = state->data[ofs].group.begin;
            end_group = state->data[ofs].group.end;
            if (0 <= begin_group && begin_group <= end_group && end_group <=
              text_length)
                node = node->next_1.node;
            else
                node = node->next_2.node;
            break;
        }
        case RE_OP_LAZY_REPEAT: /* Lazy repeat. */
        {
            Py_ssize_t ofs;
            RE_RepeatData* rp_data;
            size_t max_count;
            RE_BacktrackData* bt_data;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the repeat values in the context. Repeat
             * indexes are 0-based.
             */
            ofs = repeat_info[node->values[0]].value_offset;
            rp_data = &state->data[ofs].repeat;

            /* We might need to backtrack into the head. */
            if (!add_backtrack(state, RE_OP_LAZY_REPEAT) ||
              !push_groups(state))
                return RE_ERROR_MEMORY;
            bt_data = state->backtrack;
            bt_data->repeat.ofs = ofs;
            bt_data->repeat.count = rp_data->count;
            bt_data->repeat.max_count = rp_data->max_count;

            max_count = node->values[3] ? slice_end - text_pos : text_pos -
              slice_start;
            if (max_count > node->values[2])
                max_count = node->values[2];

            rp_data->count = 0;
            rp_data->max_count = max_count;

            /* Does the body have to match at all? */
            if (node->values[1] == 0) {
                /* The body doesn't have to match, but the tail takes
                 * precedence over it. If the tail fails to match then we want
                 * to try the body again before backtracking into the head.
                 */

                /* Record backtracking info for matching the body. */
                if (!add_backtrack(state, RE_OP_END_LAZY_REPEAT) ||
                  !push_groups(state))
                    return RE_ERROR_MEMORY;
                bt_data = state->backtrack;
                bt_data->repeat.ofs = ofs;
                bt_data->repeat.position.node = node->next_1.node;
                bt_data->repeat.position.text_pos = text_pos;

                /* Advance into the tail. */
                node = node->next_2.node;
            } else {
                /* The body must match. */

                /* Advance into the body. */
                node = node->next_1.node;
            }
            break;
        }
        case RE_OP_LAZY_REPEAT_ONE: /* Lazy repeat for one character. */
        {
            size_t count;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Count how many times the character repeats, up to the minimum.
             */
            count = count_one(state, node->next_2.node, text_pos,
              node->values[1]);

            /* Have we matched at least the minimum? */
            if (count < node->values[1])
                goto backtrack;

            if (count < node->values[2]) {
                /* The match is shorter than the maximum, so we might need to
                 * backtrack the repeat to consume more.
                 */
                Py_ssize_t ofs;
                RE_RepeatData* rp_data;
                RE_BacktrackData* bt_data;

                /* Get the offset to the repeat values in the context. Repeat
                 * indexes are 0-based.
                 */
                ofs = repeat_info[node->values[0]].value_offset;
                rp_data = &state->data[ofs].repeat;
                if (!add_backtrack(state, RE_OP_LAZY_REPEAT_ONE) ||
                  !push_groups(state))
                    return RE_ERROR_MEMORY;
                bt_data = state->backtrack;
                bt_data->repeat.position.node = node;
                bt_data->repeat.ofs = ofs;
                bt_data->repeat.start = rp_data->start;
                bt_data->repeat.count = rp_data->count;

                rp_data->start = text_pos;
                rp_data->count = count;
            }

            text_pos += (Py_ssize_t)count * step;
            node = node->next_1.node;
            break;
        }
        case RE_OP_LOOKAROUND: /* Lookaround. */
        {
            RE_BacktrackBlock* current_block;
            size_t backtrack_count;
            size_t saved_groups_count;
            BOOL must_advance;
            int status;
            BOOL matched;
            TRACE(("%s %d\n", re_op_text[node->op], node->match))

            /* Try to match the subpattern. */
            current_block = state->current_block;
            backtrack_count = current_block->count;
            saved_groups_count = state->saved_groups_count;
            must_advance = state->must_advance;
            state->slice_start = 0;
            state->slice_end = text_length;
            state->text_pos = text_pos;
            state->must_advance = FALSE;

            status = basic_match(state, node->next_2.node, FALSE);

            if (status < 0)
                return status;

            state->slice_end = slice_end;
            state->slice_start = slice_start;
            state->must_advance = must_advance;
            state->saved_groups_count = saved_groups_count;
            current_block->count = backtrack_count;
            state->current_block = current_block;

            matched = status == RE_ERROR_SUCCESS;
            if (matched != node->match)
                goto backtrack;

            node = node->next_1.node;
            break;
        }
        case RE_OP_PROPERTY: /* A property. */
            /* values are: property */
            TRACE(("%s %d %d\n", re_op_text[node->op], node->match,
              node->values[0]))
            if (text_pos >= slice_end || has_property(node->values[0],
              char_at(text, text_pos)) != node->match)
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_PROPERTY_REV: /* A property. */
            /* values are: property */
            TRACE(("%s %d %d\n", re_op_text[node->op], node->match,
              node->values[0]))
            if (text_pos <= slice_start || has_property(node->values[0],
              char_at(text, text_pos - 1)) != node->match)
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_REF_GROUP: /* Reference to a capture group. */
        {
            Py_ssize_t ofs;
            Py_ssize_t begin_group;
            Py_ssize_t end_group;
            size_t length;
            size_t available;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the group value in the context. Capture group
             * indexes are 1-based (excluding group 0, which is the entire
             * matched string).
             */
            ofs = group_info[node->values[0] - 1].value_offset;

            /* Check whether the captured text, if any, exists at this position
             * in the string.
             */
            begin_group = state->data[ofs].group.begin;
            end_group = state->data[ofs].group.end;
            if (!(0 <= begin_group && begin_group <= end_group && end_group <=
              text_length))
                goto backtrack;

            length = end_group - begin_group;
            available = slice_end - text_pos;
            if (length > available)
                goto backtrack;

            if (state->wide) {
                RE_UCHAR* text_ptr;
                RE_UCHAR* group_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;
                group_ptr = (RE_UCHAR*)text + begin_group;

                for (i = 0; i < length; i++) {
                    if (text_ptr[i] != group_ptr[i])
                        goto backtrack;
                }
            } else {
                RE_BCHAR* text_ptr;
                RE_BCHAR* group_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;
                group_ptr = (RE_BCHAR*)text + begin_group;

                for (i = 0; i < length; i++) {
                    if (text_ptr[i] != group_ptr[i])
                        goto backtrack;
                }
            }
            text_pos += length;

            node = node->next_1.node;
            break;
        }
        case RE_OP_REF_GROUP_IGN: /* Reference to a capture group, ignoring case. */
        {
            Py_ssize_t ofs;
            Py_ssize_t begin_group;
            Py_ssize_t end_group;
            size_t length;
            size_t available;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the group value in the context. Capture group
             * indexes are 1-based (excluding group 0, which is the entire
             * matched string).
             */
            ofs = group_info[node->values[0] - 1].value_offset;

            /* Check whether the captured text, if any, exists at this position
             * in the string.
             */
            begin_group = state->data[ofs].group.begin;
            end_group = state->data[ofs].group.end;
            if (!(0 <= begin_group && begin_group <= end_group && end_group <=
              text_length))
                goto backtrack;

            length = end_group - begin_group;
            available = slice_end - text_pos;
            if (length > available)
                goto backtrack;

            if (state->wide) {
                RE_UCHAR* text_ptr;
                RE_UCHAR* group_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;
                group_ptr = (RE_UCHAR*)text + begin_group;

                for (i = 0; i < length; i++) {
                    if (!same_char_ign(text_ptr[i], group_ptr[i]))
                        goto backtrack;
                }
            } else {
                RE_BCHAR* text_ptr;
                RE_BCHAR* group_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;
                group_ptr = (RE_BCHAR*)text + begin_group;

                for (i = 0; i < length; i++) {
                    if (!same_char_ign(text_ptr[i], group_ptr[i]))
                        goto backtrack;
                }
            }
            text_pos += length;

            node = node->next_1.node;
            break;
        }
        case RE_OP_REF_GROUP_IGN_REV: /* Reference to a capture group, ignoring case. */
        {
            Py_ssize_t ofs;
            Py_ssize_t begin_group;
            Py_ssize_t end_group;
            size_t length;
            size_t available;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the group value in the context. Capture group
             * indexes are 1-based (excluding group 0, which is the entire
             * matched string).
             */
            ofs = group_info[node->values[0] - 1].value_offset;

            /* Check whether the captured text, if any, exists at this position
             * in the string.
             */
            begin_group = state->data[ofs].group.begin;
            end_group = state->data[ofs].group.end;
            if (!(0 <= begin_group && begin_group <= end_group && end_group <=
              text_length))
                goto backtrack;

            length = end_group - begin_group;
            available = text_pos - slice_start;
            if (length > available)
                goto backtrack;

            text_pos -= length;

            if (state->wide) {
                RE_UCHAR* text_ptr;
                RE_UCHAR* group_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;
                group_ptr = (RE_UCHAR*)text + begin_group;

                for (i = 0; i < length; i++) {
                    if (!same_char_ign(text_ptr[i], group_ptr[i]))
                        goto backtrack;
                }
            } else {
                RE_BCHAR* text_ptr;
                RE_BCHAR* group_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;
                group_ptr = (RE_BCHAR*)text + begin_group;

                for (i = 0; i < length; i++) {
                    if (!same_char_ign(text_ptr[i], group_ptr[i]))
                        goto backtrack;
                }
            }

            node = node->next_1.node;
            break;
        }
        case RE_OP_REF_GROUP_REV: /* Reference to a capture group. */
        {
            Py_ssize_t ofs;
            Py_ssize_t begin_group;
            Py_ssize_t end_group;
            size_t length;
            size_t available;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Get the offset to the group value in the context. Capture group
             * indexes are 1-based (excluding group 0, which is the entire
             * matched string).
             */
            ofs = group_info[node->values[0] - 1].value_offset;

            /* Check whether the captured text, if any, exists at this position
             * in the string.
             */
            begin_group = state->data[ofs].group.begin;
            end_group = state->data[ofs].group.end;
            if (!(0 <= begin_group && begin_group <= end_group && end_group <=
              text_length))
                goto backtrack;

            length = end_group - begin_group;
            available = text_pos - slice_start;
            if (length > available)
                goto backtrack;

            text_pos -= length;

            if (state->wide) {
                RE_UCHAR* text_ptr;
                RE_UCHAR* group_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;
                group_ptr = (RE_UCHAR*)text + begin_group;

                for (i = 0; i < length; i++) {
                    if (text_ptr[i] != group_ptr[i])
                        goto backtrack;
                }
            } else {
                RE_BCHAR* text_ptr;
                RE_BCHAR* group_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;
                group_ptr = (RE_BCHAR*)text + begin_group;

                for (i = 0; i < length; i++) {
                    if (text_ptr[i] != group_ptr[i])
                        goto backtrack;
                }
            }

            node = node->next_1.node;
            break;
        }
        case RE_OP_SEARCH_ANCHOR: /* At the start of the search. */
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))
            if (text_pos != state->search_anchor)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_SET: /* Character set. */
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (text_pos >= slice_end || !in_set(encoding, node, char_at(text,
              text_pos)))
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_SET_REV: /* Character set. */
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (text_pos <= slice_start || !in_set(encoding, node,
              char_at(text, text_pos - 1)))
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_SMALL_BITSET: /* Small bitset. */
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (text_pos >= slice_end || !in_small_bitset(node, char_at(text,
              text_pos)))
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_SMALL_BITSET_REV: /* Small bitset. */
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (text_pos <= slice_start || !in_small_bitset(node, char_at(text,
              text_pos - 1)))
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_START_OF_LINE: /* At the start of a line. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos != 0 && char_at(text, text_pos - 1) != '\n')
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_START_OF_STRING: /* At the start of the string. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos != 0)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_STRING: /* A string literal. */
        {
            size_t length;
            size_t available;
            RE_CODE* values;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->value_count))

            length = node->value_count;
            available = slice_end - text_pos;
            if (length > available)
                goto backtrack;

            values = node->values;

            if (state->wide) {
                RE_UCHAR* text_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;

                for (i = 0; i < length; i++) {
                    if (text_ptr[i] != values[i])
                        goto backtrack;
                }
            } else {
                RE_BCHAR* text_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;

                for (i = 0; i < length; i++) {
                    if (text_ptr[i] != values[i])
                        goto backtrack;
                }
            }
            text_pos += length;
            node = node->next_1.node;
            break;
        }
        case RE_OP_STRING_IGN: /* A string literal, ignoring case. */
        {
            size_t length;
            size_t available;
            RE_CODE* values;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->value_count))

            length = node->value_count;
            available = slice_end - text_pos;
            if (length > available)
                goto backtrack;

            values = node->values;

            if (state->wide) {
                RE_UCHAR* text_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;

                for (i = 0; i < length; i++) {
                    if (!same_char_ign(text_ptr[i], values[i]))
                        goto backtrack;
                }
            } else {
                RE_BCHAR* text_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;

                for (i = 0; i < length; i++) {
                    if (!same_char_ign(text_ptr[i], values[i]))
                        goto backtrack;
                }
            }
            text_pos += length;
            node = node->next_1.node;
            break;
        }
        case RE_OP_STRING_IGN_REV: /* A string literal, ignoring case. */
        {
            size_t length;
            size_t available;
            RE_CODE* values;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->value_count))

            length = node->value_count;
            available = text_pos - slice_start;
            if (length > available)
                goto backtrack;

            values = node->values;
            text_pos -= length;

            if (state->wide) {
                RE_UCHAR* text_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;

                for (i = 0; i < length; i++) {
                    if (!same_char_ign(text_ptr[i], values[i]))
                        goto backtrack;
                }
            } else {
                RE_BCHAR* text_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;

                for (i = 0; i < length; i++) {
                    if (!same_char_ign(text_ptr[i], values[i]))
                        goto backtrack;
                }
            }
            node = node->next_1.node;
            break;
        }
        case RE_OP_STRING_REV: /* A string literal. */
        {
            size_t length;
            size_t available;
            RE_CODE* values;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->value_count))

            length = node->value_count;
            available = text_pos - slice_start;
            if (length > available)
                goto backtrack;

            values = node->values;
            text_pos -= length;

            if (state->wide) {
                RE_UCHAR* text_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;

                for (i = 0; i < length; i++) {
                    if (text_ptr[i] != values[i])
                        goto backtrack;
                }
            } else {
                RE_BCHAR* text_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;

                for (i = 0; i < length; i++) {
                    if (text_ptr[i] != values[i])
                        goto backtrack;
                }
            }
            node = node->next_1.node;
            break;
        }
        case RE_OP_SUCCESS: /* Success. */
            /* Must the match advance past its start? */
            if (text_pos == state->search_anchor && state->must_advance)
                goto backtrack;

            state->text_pos = text_pos;
            return RE_ERROR_SUCCESS;
        default: /* Illegal opcode! */
            TRACE(("UNKNOWN OP %d\n", node->op))
            return RE_ERROR_ILLEGAL;
        }

        /* Should we abort the matching? */
        ++iterations;
        if ((iterations & 0xFFFF) == 0 && safe_check_signals(state))
            return RE_ERROR_INTERRUPTED;
    }

backtrack:
    for (;;) {
        RE_BacktrackData* bt_data;

        bt_data = last_backtrack(state);

        switch (bt_data->op) {
        case RE_OP_BRANCH: /* 2-way branch. */
        {
            pop_groups(state);
            node = bt_data->branch.position.node;
            text_pos = bt_data->branch.position.text_pos;
            discard_backtrack(state);
            goto advance;
        }
        case RE_OP_END_GREEDY_REPEAT: /* End of a greedy repeat. */
        {
            RE_RepeatData* rp_data;

            rp_data = &state->data[bt_data->repeat.ofs].repeat;
            rp_data->count = bt_data->repeat.count;
            rp_data->max_count = bt_data->repeat.max_count;
            if (bt_data->repeat.position.node) {
                /* Restore then advance. */
                pop_groups(state);
                node = bt_data->repeat.position.node;
                text_pos = bt_data->repeat.position.text_pos;
                discard_backtrack(state);
                goto advance;
            } else
                /* Restore then backtrack. */
                discard_backtrack(state);
            break;
        }
        case RE_OP_END_LAZY_REPEAT: /* End of a lazy repeat. */
        {
            /* Restore then advance. */
            RE_RepeatData* rp_data;

            pop_groups(state);
            rp_data = &state->data[bt_data->repeat.ofs].repeat;
            node = bt_data->repeat.position.node;
            text_pos = bt_data->repeat.position.text_pos;
            discard_backtrack(state);
            goto advance;
        }
        case RE_OP_FAILURE:
        {
            Py_ssize_t end_pos;

            /* Do we have to advance? */
            if (!search)
                return RE_ERROR_FAILURE;

            /* Can we advance? */
            text_pos = state->match_pos;
            end_pos = state->reverse ? slice_start : slice_end;
            if (text_pos == end_pos)
                return RE_ERROR_FAILURE;

            /* Skip over any repeated leading characters. */
            switch (start_node->op) {
            case RE_OP_GREEDY_REPEAT_ONE:
            case RE_OP_LAZY_REPEAT_ONE:
            {
                size_t available;
                size_t count;

                available = state->reverse ? text_pos - slice_start : slice_end
                  - text_pos;
                count = count_one(state, start_node->next_2.node, text_pos,
                  available);
                if (count > start_node->values[2])
                    count -= start_node->values[2];
                text_pos += (Py_ssize_t)count * step;
                break;
            }
            }

            /* Advance and try to match again. */
            state->text_pos = text_pos + step;
            reload_groups(state);
            goto start_match;
        }
        case RE_OP_GREEDY_REPEAT: /* Greedy repeat. */
        {
            RE_RepeatData* rp_data;

            pop_groups(state);
            rp_data = &state->data[bt_data->repeat.ofs].repeat;
            rp_data->count = bt_data->repeat.count;
            rp_data->max_count = bt_data->repeat.max_count;
            discard_backtrack(state);
            break;
        }
        case RE_OP_GREEDY_REPEAT_ONE: /* Greedy repeat for one character. */
        {
            RE_RepeatData* rp_data;
            size_t count;
            Py_ssize_t step;
            Py_ssize_t pos;
            Py_ssize_t limit;
            BOOL match;
            RE_CODE ch;
            BOOL m;

            node = bt_data->repeat.position.node;

            rp_data = &state->data[bt_data->repeat.ofs].repeat;

            /* Unmatch one character at a time until the tail could match or we
             * have reached the minimum.
             */
            text_pos = rp_data->start;

            count = rp_data->count;
            step = node->step;
            pos = text_pos + (Py_ssize_t)count * step;
            limit = text_pos + (Py_ssize_t)node->values[1] * step;

            ch = node->next_1.test->values[0];
            m = node->next_1.test->match;

            switch (node->next_1.test->op) {
            case RE_OP_CHARACTER:
            {
                if (state->wide) {
                    RE_UCHAR* ptr;
                    RE_UCHAR* limit_ptr;

                    ptr = (RE_UCHAR*)text + pos;
                    limit_ptr = (RE_UCHAR*)text + limit;

                    for (;;) {
                        ptr -= step;
                        match = (ptr[0] == ch) == m;
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_UCHAR*)text;
                } else {
                    RE_BCHAR* ptr;
                    RE_BCHAR* limit_ptr;

                    ptr = (RE_BCHAR*)text + pos;
                    limit_ptr = (RE_BCHAR*)text + limit;

                    for (;;) {
                        ptr -= step;
                        match = (ptr[0] == ch) == m;
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_BCHAR*)text;
                }
                break;
            }
            case RE_OP_CHARACTER_IGN:
            {
                RE_CODE ch_lower;
                RE_CODE ch_upper;
                RE_CODE ch_title;

                ch_lower = encoding->lower(ch);
                ch_title = ch_upper = encoding->upper(ch);
                if (ch_lower != ch_upper)
                    ch_title = encoding->title(ch);

                if (state->wide) {
                    RE_UCHAR* ptr;
                    RE_UCHAR* limit_ptr;

                    ptr = (RE_UCHAR*)text + pos;
                    limit_ptr = (RE_UCHAR*)text + limit;

                    for (;;) {
                        ptr -= step;
                        match = same_char_ign_3(ptr[0], ch, ch_lower, ch_upper,
                          ch_title) == m;
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_UCHAR*)text;
                } else {
                    RE_BCHAR* ptr;
                    RE_BCHAR* limit_ptr;

                    ptr = (RE_BCHAR*)text + pos;
                    limit_ptr = (RE_BCHAR*)text + limit;

                    for (;;) {
                        ptr -= step;
                        match = same_char_ign_3(ptr[0], ch, ch_lower, ch_upper,
                          ch_title) == m;
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_BCHAR*)text;
                }
                break;
            }
            case RE_OP_CHARACTER_IGN_REV:
            {
                RE_CODE ch_lower;
                RE_CODE ch_upper;
                RE_CODE ch_title;

                ch_lower = encoding->lower(ch);
                ch_upper = encoding->upper(ch);
                ch_title = ch_upper = encoding->upper(ch);
                if (ch_lower != ch_upper)
                    ch_title = encoding->title(ch);

                if (state->wide) {
                    RE_UCHAR* ptr;
                    RE_UCHAR* limit_ptr;

                    ptr = (RE_UCHAR*)text + pos;
                    limit_ptr = (RE_UCHAR*)text + limit;

                    for (;;) {
                        ptr -= step;
                        match = same_char_ign_3(ptr[-1], ch, ch_lower,
                          ch_upper, ch_title) == m;
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_UCHAR*)text;
                } else {
                    RE_BCHAR* ptr;
                    RE_BCHAR* limit_ptr;

                    ptr = (RE_BCHAR*)text + pos;
                    limit_ptr = (RE_BCHAR*)text + limit;

                    for (;;) {
                        ptr -= step;
                        match = same_char_ign_3(ptr[-1], ch, ch_lower,
                          ch_upper, ch_title) == m;
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_BCHAR*)text;
                }
                break;
            }
            case RE_OP_CHARACTER_REV:
            {
                if (state->wide) {
                    RE_UCHAR* ptr;
                    RE_UCHAR* limit_ptr;

                    ptr = (RE_UCHAR*)text + pos;
                    limit_ptr = (RE_UCHAR*)text + limit;

                    for (;;) {
                        ptr -= step;
                        match = (ptr[-1] == ch) == m;
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_UCHAR*)text;
                } else {
                    RE_BCHAR* ptr;
                    RE_BCHAR* limit_ptr;

                    ptr = (RE_BCHAR*)text + pos;
                    limit_ptr = (RE_BCHAR*)text + limit;

                    for (;;) {
                        ptr -= step;
                        match = (ptr[-1] == ch) == m;
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_BCHAR*)text;
                }
                break;
            }
            case RE_OP_STRING:
            {
                if (state->wide) {
                    RE_UCHAR* ptr;
                    RE_UCHAR* limit_ptr;

                    ptr = (RE_UCHAR*)text + pos;
                    limit_ptr = (RE_UCHAR*)text + limit;

                    for (;;) {
                        RE_Position next_position;

                        ptr -= step;
                        match = ptr[0] == ch && try_match(state, &node->next_1,
                          ptr - (RE_UCHAR*)text, &next_position);
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_UCHAR*)text;
                } else {
                    RE_BCHAR* ptr;
                    RE_BCHAR* limit_ptr;

                    ptr = (RE_BCHAR*)text + pos;
                    limit_ptr = (RE_BCHAR*)text + limit;

                    for (;;) {
                        RE_Position next_position;

                        ptr -= step;
                        match = ptr[0] == ch && try_match(state, &node->next_1,
                          ptr - (RE_BCHAR*)text, &next_position);
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_BCHAR*)text;
                }
                break;
            }
            case RE_OP_STRING_IGN:
            {
                RE_CODE ch_lower;
                RE_CODE ch_upper;
                RE_CODE ch_title;

                ch_lower = encoding->lower(ch);
                ch_upper = encoding->upper(ch);
                ch_title = ch_upper = encoding->upper(ch);
                if (ch_lower != ch_upper)
                    ch_title = encoding->title(ch);

                if (state->wide) {
                    RE_UCHAR* ptr;
                    RE_UCHAR* limit_ptr;

                    ptr = (RE_UCHAR*)text + pos;
                    limit_ptr = (RE_UCHAR*)text + limit;

                    for (;;) {
                        RE_Position next_position;

                        ptr -= step;
                        match = same_char_ign_3(ptr[0], ch, ch_lower, ch_upper,
                          ch_title) && try_match(state, &node->next_1, ptr -
                          (RE_UCHAR*)text, &next_position);
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_UCHAR*)text;
                } else {
                    RE_BCHAR* ptr;
                    RE_BCHAR* limit_ptr;

                    ptr = (RE_BCHAR*)text + pos;
                    limit_ptr = (RE_BCHAR*)text + limit;

                    for (;;) {
                        RE_Position next_position;

                        ptr -= step;
                        match = same_char_ign_3(ptr[0], ch, ch_lower, ch_upper,
                          ch_title) && try_match(state, &node->next_1, ptr -
                          (RE_BCHAR*)text, &next_position);
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_BCHAR*)text;
                }
                break;
            }
            case RE_OP_STRING_IGN_REV:
            {
                RE_CODE ch_lower;
                RE_CODE ch_upper;
                RE_CODE ch_title;

                ch = node->next_1.test->values[node->next_1.test->value_count -
                  1];
                ch_lower = encoding->lower(ch);
                ch_upper = encoding->upper(ch);
                ch_title = ch_upper = encoding->upper(ch);
                if (ch_lower != ch_upper)
                    ch_title = encoding->title(ch);

                if (state->wide) {
                    RE_UCHAR* ptr;
                    RE_UCHAR* limit_ptr;

                    ptr = (RE_UCHAR*)text + pos;
                    limit_ptr = (RE_UCHAR*)text + limit;

                    for (;;) {
                        RE_Position next_position;

                        ptr -= step;
                        match = same_char_ign_3(ptr[-1], ch, ch_lower,
                          ch_upper, ch_title) && try_match(state,
                          &node->next_1, ptr - (RE_UCHAR*)text,
                          &next_position);
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_UCHAR*)text;
                } else {
                    RE_BCHAR* ptr;
                    RE_BCHAR* limit_ptr;

                    ptr = (RE_BCHAR*)text + pos;
                    limit_ptr = (RE_BCHAR*)text + limit;

                    for (;;) {
                        RE_Position next_position;

                        ptr -= step;
                        match = same_char_ign_3(ptr[-1], ch, ch_lower,
                          ch_upper, ch_title) && try_match(state,
                          &node->next_1, ptr - (RE_BCHAR*)text,
                          &next_position);
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_BCHAR*)text;
                }
                break;
            }
            case RE_OP_STRING_REV:
            {
                ch = node->next_1.test->values[node->next_1.test->value_count -
                  1];

                if (state->wide) {
                    RE_UCHAR* ptr;
                    RE_UCHAR* limit_ptr;

                    ptr = (RE_UCHAR*)text + pos;
                    limit_ptr = (RE_UCHAR*)text + limit;

                    for (;;) {
                        RE_Position next_position;

                        ptr -= step;
                        match = ptr[-1] == ch && try_match(state,
                          &node->next_1, ptr - (RE_UCHAR*)text,
                          &next_position);
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_UCHAR*)text;
                } else {
                    RE_BCHAR* ptr;
                    RE_BCHAR* limit_ptr;

                    ptr = (RE_BCHAR*)text + pos;
                    limit_ptr = (RE_BCHAR*)text + limit;

                    for (;;) {
                        RE_Position next_position;

                        ptr -= step;
                        match = ptr[-1] == ch && try_match(state,
                          &node->next_1, ptr - (RE_BCHAR*)text,
                          &next_position);
                        if (match)
                            break;
                        if (ptr == limit_ptr)
                            break;
                    }

                    pos = ptr - (RE_BCHAR*)text;
                }
                break;
            }
            default:
                for (;;) {
                    RE_Position next_position;

                    pos -= step;
                    match = try_match(state, &node->next_1, pos,
                      &next_position);
                    if (match)
                        break;
                    if (pos == limit)
                        break;
                }
                break;
            }

            if (match) {
                count = abs(pos - text_pos);

                /* The tail could match. */
                if (count > node->values[1]) {
                    /* The match is longer than the minimum, so we might need
                     * to backtrack the repeat again to consume less.
                     */
                    rp_data->count = count;
                    reload_groups(state);
                } else {
                    /* We've reached the minimum, so we won't need to backtrack
                     * the repeat again.
                     */
                    pop_groups(state);
                    rp_data->start = bt_data->repeat.start;
                    rp_data->count = bt_data->repeat.count;
                    discard_backtrack(state);
                }

                node = node->next_1.node;
                text_pos = pos;
                goto advance;
            } else {
                /* We've backtracked the repeat as far as we can. */
                drop_groups(state);
                rp_data->start = bt_data->repeat.start;
                rp_data->count = bt_data->repeat.count;
                discard_backtrack(state);
            }
            break;
        }
        case RE_OP_LAZY_REPEAT: /* Lazy repeat. */
        {
            RE_RepeatData* rp_data;

            pop_groups(state);
            rp_data = &state->data[bt_data->repeat.ofs].repeat;
            rp_data->count = bt_data->repeat.count;
            rp_data->max_count = bt_data->repeat.max_count;
            discard_backtrack(state);
            break;
        }
        case RE_OP_LAZY_REPEAT_ONE: /* Lazy repeat for one character. */
        {
            RE_RepeatData* rp_data;
            size_t count;
            Py_ssize_t step;
            Py_ssize_t pos;
            size_t available;
            size_t max_count;
            Py_ssize_t limit;
            RE_Node* repeated;
            RE_Node* test;
            BOOL match;
            RE_CODE ch;
            BOOL m;

            node = bt_data->repeat.position.node;

            rp_data = &state->data[bt_data->repeat.ofs].repeat;

            /* Match one character at a time until the tail could match or we
             * have reached the maximum.
             */
            text_pos = rp_data->start;
            count = rp_data->count;

            step = node->step;
            pos = text_pos + (Py_ssize_t)count * step;
            available = step > 0 ? slice_end - text_pos : text_pos -
              slice_start;
            max_count = node->values[2];
            if (max_count > available)
                max_count = available;
            limit = text_pos + (Py_ssize_t)max_count * step;

            repeated = node->next_2.node;
            test = node->next_1.test;

            ch = test->values[0];
            m = test->match;

            switch (test->op) {
            case RE_OP_CHARACTER:
            {
                limit -= step;

                if (state->wide) {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = (UCHAR_AT(text, pos) == ch) == m;
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = (BCHAR_AT(text, pos) == ch) == m;
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_CHARACTER_IGN:
            {
                RE_CODE ch_lower;
                RE_CODE ch_upper;
                RE_CODE ch_title;

                limit -= step;

                ch_lower = encoding->lower(ch);
                ch_upper = encoding->upper(ch);
                ch_title = ch_upper = encoding->upper(ch);
                if (ch_lower != ch_upper)
                    ch_title = encoding->title(ch);

                if (state->wide) {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign_3(UCHAR_AT(text, pos), ch,
                          ch_lower, ch_upper, ch_title) == m;
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign_3(BCHAR_AT(text, pos), ch,
                          ch_lower, ch_upper, ch_title) == m;
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_CHARACTER_IGN_REV:
            {
                RE_CODE ch_lower;
                RE_CODE ch_upper;
                RE_CODE ch_title;

                limit -= step;

                ch_lower = encoding->lower(ch);
                ch_upper = encoding->upper(ch);
                ch_title = ch_upper = encoding->upper(ch);
                if (ch_lower != ch_upper)
                    ch_title = encoding->title(ch);

                if (state->wide) {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign_3(UCHAR_AT(text, pos - 1), ch,
                          ch_lower, ch_upper, ch_title) == m;
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign_3(BCHAR_AT(text, pos - 1), ch,
                          ch_lower, ch_upper, ch_title) == m;
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_CHARACTER_REV:
            {
                limit -= step;

                if (state->wide) {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = (UCHAR_AT(text, pos - 1) == ch) == m;
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = (BCHAR_AT(text, pos - 1) == ch) == m;
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING:
            {
                limit -= test->value_count * step;

                if (state->wide) {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = UCHAR_AT(text, pos) == ch && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = BCHAR_AT(text, pos) == ch && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING_IGN:
            {
                RE_CODE ch_lower;
                RE_CODE ch_upper;
                RE_CODE ch_title;

                limit -= test->value_count * step;

                ch_lower = encoding->lower(ch);
                ch_upper = encoding->upper(ch);
                ch_title = ch_upper = encoding->upper(ch);
                if (ch_lower != ch_upper)
                    ch_title = encoding->title(ch);

                if (state->wide) {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign_3(BCHAR_AT(text, pos), ch,
                          ch_lower, ch_upper, ch_title) && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign_3(BCHAR_AT(text, pos), ch,
                          ch_lower, ch_upper, ch_title) && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING_IGN_REV:
            {
                RE_CODE ch_lower;
                RE_CODE ch_upper;
                RE_CODE ch_title;

                limit -= test->value_count * step;

                ch = test->values[test->value_count - 1];
                ch_lower = encoding->lower(ch);
                ch_upper = encoding->upper(ch);
                ch_title = ch_upper = encoding->upper(ch);
                if (ch_lower != ch_upper)
                    ch_title = encoding->title(ch);

                if (state->wide) {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign_3(UCHAR_AT(text, pos - 1), ch,
                          ch_lower, ch_upper, ch_title) && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign_3(BCHAR_AT(text, pos - 1), ch,
                          ch_lower, ch_upper, ch_title) && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING_REV:
            {
                limit -= test->value_count * step;

                ch = test->values[test->value_count - 1];

                if (state->wide) {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = UCHAR_AT(text, pos - 1) == ch &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = BCHAR_AT(text, pos - 1) == ch &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match)
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            default:
                for (;;) {
                    RE_Position next_position;

                    match = match_one(state, repeated, pos);
                    if (!match)
                        break;
                    pos += step;
                    match = try_match(state, &node->next_1, pos,
                      &next_position);
                    if (match)
                        break;
                    if (pos == limit)
                        break;
                }
                break;
            }

            if (match) {
                /* The tail could match. */
                count = abs(pos - text_pos);
                text_pos = pos;

                if (count < max_count) {
                    /* The match is shorter than the maximum, so we might need
                     * to backtrack the repeat again to consume more.
                     */
                    rp_data->count = count;
                    reload_groups(state);
                } else {
                    /* We've reached the maximum, so we won't need to backtrack
                     * the repeat again.
                     */
                    pop_groups(state);
                    rp_data->start = bt_data->repeat.start;
                    rp_data->count = bt_data->repeat.count;
                    discard_backtrack(state);
                }

                node = node->next_1.node;
                goto advance;
            } else {
                /* The tail couldn't match. */
                drop_groups(state);
                rp_data->start = bt_data->repeat.start;
                rp_data->count = bt_data->repeat.count;
                discard_backtrack(state);
            }
            break;
        }
        default:
            return RE_ERROR_ILLEGAL;
        }
    }
}

/* Performs a match or search from the current text position. */
Py_LOCAL(int) do_match(RE_State* state, BOOL search) {
    size_t available;
    int status;
    TRACE(("<<do_match>>\n"))

    /* Is there enough to search? */
    available = state->reverse ? state->text_pos - state->slice_start :
      state->slice_end - state->text_pos;
    if (available < state->min_width)
        return RE_ERROR_FAILURE;

#if defined(RE_MULTITHREADED)
    /* Release the GIL. */
    if (state->is_multithreaded)
        release_GIL(state);

#endif
    /* Initialise the base context. */
    init_match(state);

    if (state->do_check) {
        state->do_check = FALSE;
        if (!general_check(state, state->pattern->start_node, state->text_pos))
          {
#if defined(RE_MULTITHREADED)
            /* Re-acquire the GIL. */
            if (state->is_multithreaded)
                acquire_GIL(state);

#endif
            return RE_ERROR_FAILURE;
        }
    }

    /* Perform the match. */
    status = basic_match(state, state->pattern->start_node, search);
    if (status == RE_ERROR_SUCCESS) {
        Py_ssize_t max_end_index;
        RE_GroupInfo* group_info;
        PatternObject* pattern;
        Py_ssize_t g;

        /* Store the results. */
        state->lastindex = -1;
        state->lastgroup = -1;
        max_end_index = -1;

        /* Store the capture groups. */
        group_info = state->pattern->group_info;
        pattern = state->pattern;
        for (g = 0; g < pattern->group_count; g++) {
            Py_ssize_t ofs;
            Py_ssize_t m;
            Py_ssize_t begin;
            Py_ssize_t end;

            ofs = group_info[g].value_offset;
            begin = state->data[ofs].group.begin;
            end = state->data[ofs].group.end;
            m = g * 2;
            state->marks[m] = begin;
            state->marks[m + 1] = end;
            TRACE(("group %d at %d from %d to %d\n", g + 1, ofs, begin, end))
            if (begin >= 0 && end >= 0 && group_info[g].end_index >
              max_end_index) {
                max_end_index = group_info[g].end_index;
                state->lastindex = g + 1;
                if (group_info[g].has_name)
                    state->lastgroup = g + 1;
            }
        }
    }

#if defined(RE_MULTITHREADED)
    /* Re-acquire the GIL. */
    if (state->is_multithreaded)
        acquire_GIL(state);

#endif
    /* Has an error occurred which we haven't reported yet? */
    if (status < 0 && !PyErr_Occurred())
        set_error(status, NULL);

    return status;
}

/* Gets a string from a Python object. */
Py_LOCAL(BOOL) get_string(PyObject* string, void** characters, Py_ssize_t*
  length, Py_ssize_t* charsize) {
    /* Given a Python object, return a data pointer, a length (in characters),
     * and a character size. Return FALSE if the object is not a string (or not
     * compatible).
     */
    PyBufferProcs* buffer;
    Py_ssize_t bytes;
    Py_ssize_t size;

    /* Unicode objects do not support the buffer API. So, get the data
       directly instead. */
    if (PyUnicode_Check(string)) {
        /* Unicode strings doesn't always support the buffer interface. */
        *characters = (void*)PyUnicode_AS_DATA(string);
        *length = PyUnicode_GET_SIZE(string);
        *charsize = sizeof(Py_UNICODE);
        return TRUE;
    }

    /* Get pointer to string buffer. */
    buffer = string->ob_type->tp_as_buffer;
    if (!buffer || !buffer->bf_getreadbuffer || !buffer->bf_getsegcount ||
      buffer->bf_getsegcount(string, NULL) != 1) {
        PyErr_SetString(PyExc_TypeError, "expected string or buffer");
        return FALSE;
    }

    /* Determine buffer size. */
    bytes = buffer->bf_getreadbuffer(string, 0, characters);

    if (bytes < 0) {
        PyErr_SetString(PyExc_TypeError, "buffer has negative size");
        return FALSE;
    }

    /* Determine character size. */
    size = PyObject_Size(string);

    if (PyString_Check(string) || bytes == size)
        *charsize = 1;
    else if (bytes == (Py_ssize_t)(size * sizeof(Py_UNICODE)))
        *charsize = sizeof(Py_UNICODE);
    else {
        PyErr_SetString(PyExc_TypeError, "buffer size mismatch");
        return FALSE;
    }

    *length = size;

    return TRUE;
}

/* Initialises a state object. */
Py_LOCAL(BOOL) state_init_2(RE_State* state, PatternObject* pattern, PyObject*
  string, void* characters, Py_ssize_t length, Py_ssize_t charsize, Py_ssize_t
  start, Py_ssize_t end, BOOL overlapped) {
    state->marks = NULL;
    state->saved_groups = NULL;
    state->data = NULL;
    state->backtrack_block.previous = NULL;
    state->backtrack_block.next = NULL;
    state->backtrack_block.capacity = RE_BACKTRACK_BLOCK_SIZE;

    /* The marks for the capture groups. */
    if (pattern->group_count) {
        Py_ssize_t threshold;

        if (pattern->marks_storage) {
            state->marks = pattern->marks_storage;
            pattern->marks_storage = NULL;
        } else {
            state->marks = (Py_ssize_t*)re_alloc(pattern->group_count * 2 *
              sizeof(Py_ssize_t));
            if (!state->marks)
                goto error;
        }

        if (pattern->saved_groups_storage) {
            state->saved_groups_capacity =
              pattern->saved_groups_capacity_storage;
            state->saved_groups = pattern->saved_groups_storage;
            pattern->saved_groups_storage = NULL;
        } else {
            state->saved_groups_capacity = 16;
            threshold = pattern->group_count * 16;
            while (state->saved_groups_capacity < threshold)
                state->saved_groups_capacity *= 2;
            state->saved_groups =
              (RE_Data*)re_alloc(state->saved_groups_capacity *
              sizeof(RE_Data));
            if (!state->saved_groups)
                goto error;
        }
    } else
        state->saved_groups_capacity = 0;

    /* Adjust boundaries. */
    if (start < 0)
        start += length;
    if (start < 0)
        start = 0;
    else if (start > length)
        start = length;

    if (end < 0)
        end += length;
    if (end < 0)
        end = 0;
    else if (end > length)
        end = length;

    state->overlapped = overlapped;
    state->min_width = pattern->min_width;

    state->charsize = charsize;
    state->wide = charsize > 1;
    state->char_at = state->wide ? unicode_char_at : bytes_char_at;
    state->encoding = pattern->encoding;

    /* The state object contains a reference to the string and also a pointer
     * to its contents.
     *
     * The documentation says that the end of the slice behaves like the end of
     * the string.
     */
    state->text = characters;
    state->text_length = end;

    state->reverse = pattern->flags & RE_FLAG_REVERSE;

    state->slice_start = start;
    state->slice_end = state->text_length;
    state->text_pos = state->reverse ? state->slice_end : state->slice_start;

    /* Point to the final newline if it's at the end of the string, otherwise
     * just the end of the string.
     */
    state->final_newline = -1;
    if (state->text_length > 0) {
        Py_ssize_t final_pos;

        final_pos = state->text_length - 1;
        if (state->char_at(state->text, final_pos) == '\n')
            state->final_newline = final_pos;
    }

    state->zero_width = (pattern->flags & RE_FLAG_NEW) != 0;
    state->must_advance = FALSE;
    state->do_check = pattern->do_check;

    state->pattern = pattern;
    state->string = string;

    if (pattern->data_count > 0) {
        if (pattern->data_storage) {
            state->data = pattern->data_storage;
            pattern->data_storage = NULL;
        } else
            state->data = re_alloc(pattern->data_count * sizeof(RE_Data));
    }

    Py_INCREF(state->pattern);
    Py_INCREF(state->string);

#if defined(RE_MULTITHREADED)
    /* Multithreading is possible during matching on immutable strings. */
    state->is_multithreaded = PyUnicode_Check(string) ||
      PyString_Check(string);

#endif
    return TRUE;

error:
    re_dealloc(state->data);
    re_dealloc(state->saved_groups);
    re_dealloc(state->marks);
    state->data = NULL;
    state->saved_groups = NULL;
    state->marks = NULL;
    return FALSE;
}

/* Initialises a state object. */
Py_LOCAL(BOOL) state_init(RE_State* state, PatternObject* pattern, PyObject*
  string, Py_ssize_t start, Py_ssize_t end, BOOL overlapped) {
    void* characters;
    Py_ssize_t length;
    Py_ssize_t charsize;

    /* Get the string to search or match. */
    if (!get_string(string, &characters, &length, &charsize))
        return FALSE;

    return state_init_2(state, pattern, string, characters, length, charsize,
      start, end, overlapped);
}

/* Finalises a state objects, discarding its contents. */
Py_LOCAL(void) state_fini(RE_State* state) {
    PatternObject* pattern;
    RE_BacktrackBlock* current;

    pattern = state->pattern;
    if (pattern->marks_storage)
        re_dealloc(state->marks);
    else
        pattern->marks_storage = state->marks;

    /* Deallocate the backtrack blocks. */
    current = state->backtrack_block.next;
    while (current) {
        RE_BacktrackBlock* next;

        next = current->next;
        re_dealloc(current);
        current = next;
    }

    if (pattern->data_storage)
        re_dealloc(state->data);
    else
        pattern->data_storage = state->data;

    if (pattern->saved_groups_storage)
        re_dealloc(state->saved_groups);
    else {
        pattern->saved_groups_capacity_storage = state->saved_groups_capacity;
        pattern->saved_groups_storage = state->saved_groups;
    }

    Py_DECREF(state->pattern);
    Py_DECREF(state->string);
}

Py_LOCAL(long) as_string_index(PyObject* obj, long def) {
    if (PyInt_Check(obj))
        return PyInt_AsLong(obj);
    else if (PyLong_Check(obj))
        return PyLong_AsLong(obj);
    else if (obj == Py_None)
        return def;

    set_error(RE_ERROR_INDEX, NULL);
    return 0;
}

/* Deallocates a MatchObject. */
static void match_dealloc(MatchObject* self) {
    Py_XDECREF(self->string);
    Py_DECREF(self->pattern);
    re_dealloc(self->marks);
    Py_XDECREF(self->regs);
    PyObject_DEL(self);
}

/* Gets a MatchObject's group by integer index. */
Py_LOCAL(PyObject*) match_get_group_by_index(MatchObject* self, long index,
  PyObject* def) {
    Py_ssize_t m;

    if (index < 0 || index > self->group_count) {
        /* Raise error if we were given a bad group number. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0)
        return PySequence_GetSlice(self->string, self->pos, self->endpos);

    m = index * 2 - 2;

    if (self->string == Py_None || self->marks[m] < 0 || self->marks[m + 1] <
      0) {
        /* Return default value if the string or group is undefined. */
        Py_INCREF(def);
        return def;
    }

    return PySequence_GetSlice(self->string, self->marks[m], self->marks[m +
      1]);
}

/* Gets a MatchObject's group index.
 *
 * The supplied index can be an integer or a string (group name) object.
 */
Py_LOCAL(long) match_get_group_index(MatchObject* self, PyObject* index, BOOL
  allow_neg) {
    long group = -1;

    /* Is the index an integer? */
    if (PyInt_Check(index)) {
        group = PyInt_AsLong(index);
        if (PyErr_Occurred())
            goto error;

        /* Adjust negative indices where valid and allowed. */
        if (allow_neg && -self->group_count <= group && group <= -1)
            group += self->group_count + 1;
    } else if (PyLong_Check(index)) {
        group = PyLong_AsLong(index);
        if (PyErr_Occurred())
            goto error;

        /* Adjust negative indices where valid and allowed. */
        if (allow_neg && -self->group_count <= group && group <= -1)
            group += self->group_count + 1;
    } else {
        /* The index might be a group name. */
        if (self->pattern->groupindex) {
            /* Look up the name. */
            index = PyObject_GetItem(self->pattern->groupindex, index);
            if (!index)
                goto error;

            /* Check that we have an integer. */
            if (PyInt_Check(index))
                group = PyInt_AsLong(index);
            else if (PyLong_Check(index))
                group = PyLong_AsLong(index);
            else
                goto error;

            if (PyErr_Occurred())
                goto error;

            Py_DECREF(index);
        }
    }

    return group;

error:
    PyErr_Clear();
    return -1;
}

/* Gets a MatchObject's group by object index. */
Py_LOCAL(PyObject*) match_get_group(MatchObject* self, PyObject* index,
  PyObject* def, BOOL allow_neg) {
    /* Check that the index is an integer or a string. */
    if (PyInt_Check(index) || PyLong_Check(index) || PyUnicode_Check(index) || PyString_Check(index))
        return match_get_group_by_index(self, match_get_group_index(self,
          index, allow_neg), def);

    set_error(RE_ERROR_GROUP_INDEX_TYPE, index);
    return NULL;
}

/* MatchObject's 'group' method. */
static PyObject* match_group(MatchObject* self, PyObject* args) {
    Py_ssize_t size;
    PyObject* result;
    Py_ssize_t i;

    size = PyTuple_GET_SIZE(args);

    switch (size) {
    case 0:
        /* group() */
        result = match_get_group_by_index(self, 0, Py_None);
        break;
    case 1:
        /* group(x) */
        result = match_get_group(self, PyTuple_GET_ITEM(args, 0), Py_None,
          FALSE);
        break;
    default:
        /* group(x, y, z, ...) */
        /* Fetch multiple items. */
        result = PyTuple_New(size);
        if (!result)
            return NULL;
        for (i = 0; i < size; i++) {
            PyObject* item = match_get_group(self, PyTuple_GET_ITEM(args, i),
              Py_None, FALSE);
            if (!item) {
                Py_DECREF(result);
                return NULL;
            }
            PyTuple_SET_ITEM(result, i, item);
        }
        break;
    }
    return result;
}

/* MatchObject's 'start' method. */
static PyObject* match_start(MatchObject* self, PyObject* args) {
    long index;

    PyObject* index_ = Py_False; /* Default index is 0. */
    if (!PyArg_UnpackTuple(args, "start", 0, 1, &index_))
        return NULL;

    index = match_get_group_index(self, index_, FALSE);
    if (index < 0 || index > self->group_count) {
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0)
        return Py_BuildValue("i", self->pos);

    /* A mark is -1 if the group is undefined. */
    return Py_BuildValue("i", self->marks[index * 2 - 2]);
}

/* MatchObject's 'end' method. */
static PyObject* match_end(MatchObject* self, PyObject* args) {
    long index;

    PyObject* index_ = Py_False; /* Default index is 0. */
    if (!PyArg_UnpackTuple(args, "end", 0, 1, &index_))
        return NULL;

    index = match_get_group_index(self, index_, FALSE);
    if (index < 0 || index > self->group_count) {
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0)
        return Py_BuildValue("i", self->endpos);

    /* A mark is -1 if the group is undefined. */
    return Py_BuildValue("i", self->marks[index * 2 - 1]);
}

/* Creates an integer next (2-tuple). */
Py_LOCAL(PyObject*) _pair(Py_ssize_t i1, Py_ssize_t i2) {
    PyObject* next;
    PyObject* item;

    next = PyTuple_New(2);
    if (!next)
        return NULL;

    item = PyInt_FromSsize_t(i1);
    if (!item)
        goto error;
    PyTuple_SET_ITEM(next, 0, item);

    item = PyInt_FromSsize_t(i2);
    if (!item)
        goto error;
    PyTuple_SET_ITEM(next, 1, item);

    return next;

error:
    Py_DECREF(next);
    return NULL;
}

/* MatchObject's 'span' method. */
static PyObject* match_span(MatchObject* self, PyObject* args) {
    long index;

    PyObject* index_ = Py_False; /* Default index is 0. */
    if (!PyArg_UnpackTuple(args, "span", 0, 1, &index_))
        return NULL;

    index = match_get_group_index(self, index_, FALSE);
    if (index < 0 || index > self->group_count) {
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0)
        return _pair(self->pos, self->endpos);

    /* Marks are -1 if group is undefined. */
    return _pair(self->marks[index * 2 - 2], self->marks[index * 2 - 1]);
}

/* MatchObject's 'groups' method. */
static PyObject* match_groups(MatchObject* self, PyObject* args, PyObject* kw)
  {
    PyObject* result;
    long index;

    PyObject* def = Py_None;
    static char* kwlist[] = { "default", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "|O:groups", kwlist, &def))
        return NULL;

    result = PyTuple_New(self->group_count);
    if (!result)
        return NULL;

    /* Group 0 is the entire matched portion of the string. */
    for (index = 0; index < self->group_count; index++) {
        PyObject* item;
        item = match_get_group_by_index(self, index + 1, def);
        if (!item) {
            Py_DECREF(result);
            return NULL;
        }
        PyTuple_SET_ITEM(result, index, item);
    }

    return result;
}

/* MatchObject's 'groupdict' method. */
static PyObject* match_groupdict(MatchObject* self, PyObject* args, PyObject*
  kw) {
    PyObject* result;
    PyObject* keys;
    Py_ssize_t index;

    PyObject* def = Py_None;
    static char* kwlist[] = { "default", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "|O:groupdict", kwlist, &def))
        return NULL;

    result = PyDict_New();
    if (!result || !self->pattern->groupindex)
        return result;

    keys = PyMapping_Keys(self->pattern->groupindex);
    if (!keys)
        goto failed;

    for (index = 0; index < PyList_GET_SIZE(keys); index++) {
        int status;
        PyObject* key;
        PyObject* value;
        key = PyList_GET_ITEM(keys, index);
        if (!key)
            goto failed;
        value = match_get_group(self, key, def, FALSE);
        if (!value) {
            Py_DECREF(key);
            goto failed;
        }
        status = PyDict_SetItem(result, key, value);
        Py_DECREF(value);
        if (status < 0)
            goto failed;
    }

    Py_DECREF(keys);

    return result;

failed:
    Py_XDECREF(keys);
    Py_DECREF(result);
    return NULL;
}

/* Calls a function in a module. */
Py_LOCAL(PyObject*) call(char* module, char* function, PyObject* args) {
    PyObject* name;
    PyObject* mod;
    PyObject* func;
    PyObject* result;

    if (!args)
        return NULL;
    name = PyString_FromString(module);
    if (!name)
        return NULL;
    mod = PyImport_Import(name);
    Py_DECREF(name);
    if (!mod)
        return NULL;
    func = PyObject_GetAttrString(mod, function);
    Py_DECREF(mod);
    if (!func)
        return NULL;
    result = PyObject_CallObject(func, args);
    Py_DECREF(func);
    Py_DECREF(args);
    return result;
}

/* Gets a replacement item from the replacement list.
 *
 * The replacement item could be a string literal or a group.
 */
Py_LOCAL(PyObject*) get_match_replacement(MatchObject* self, PyObject* item,
  PyObject* string, Py_ssize_t group_count) {
    long group;

    if (PyUnicode_Check(item) || PyString_Check(item)) {
        /* It's a literal, which can be added directly to the list. */
        Py_INCREF(item);
        return item;
    }

    /* Is it a group reference? */
    if (PyInt_Check(item))
        group = PyInt_AsLong(item);
    else if (PyLong_Check(item))
        group = PyLong_AsLong(item);
    else {
        /* Not a group either! */
        set_error(RE_ERROR_REPLACEMENT, NULL);
        return NULL;
    }

    if (PyErr_Occurred()) {
        set_error(RE_ERROR_REPLACEMENT, NULL);
        return NULL;
    }

    if (group == 0) {
        /* The entire matched portion of the string. */
        return PySequence_GetSlice(string, self->pos, self->endpos);
    } else if (group >= 1 && group <= group_count) {
        /* A group. If it didn't match then return None instead. */
        Py_ssize_t m = group * 2 - 2;
        if (self->marks[m] >= 0)
            return PySequence_GetSlice(string, self->marks[m], self->marks[m +
              1]);
        else {
            Py_INCREF(Py_None);
            return Py_None;
        }
    } else {
        /* No such group. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }
}

/* Joins together a list of strings. */
Py_LOCAL(PyObject*) join_list(PyObject* list, PyObject* string, BOOL reversed)
  {
    /* Join list elements. */
    PyObject* joiner;
    PyObject* function;
    PyObject* args;
    PyObject* result;

    if (PyList_GET_SIZE(list) == 1) {
        result = PyList_GET_ITEM(list, 0);
        if (PyObject_Type(result) == PyObject_Type(string)) {
            Py_INCREF(result);
            Py_DECREF(list);
            return result;
        }
    }

    joiner = PySequence_GetSlice(string, 0, 0);
    if (!joiner) {
        Py_DECREF(list);
        return NULL;
    }

    if (PyList_GET_SIZE(list) == 0) {
        Py_DECREF(list);
        return joiner;
    }

    if (reversed) {
        /* The list needs to be reversed before being joined. */
        function = PyObject_GetAttrString(list, "reverse");
        if (!function) {
            Py_DECREF(joiner);
            Py_DECREF(list);
            return NULL;
        }

        args = PyTuple_New(0);
        if (!args) {
            Py_DECREF(function);
            Py_DECREF(joiner);
            Py_DECREF(list);
            return NULL;
        }

        result = PyObject_CallObject(function, args);

        Py_DECREF(args);
        Py_DECREF(function);
        Py_DECREF(result);
    }

    function = PyObject_GetAttrString(joiner, "join");
    if (!function) {
        Py_DECREF(joiner);
        Py_DECREF(list);
        return NULL;
    }

    args = PyTuple_New(1);
    if (!args) {
        Py_DECREF(function);
        Py_DECREF(joiner);
        Py_DECREF(list);
        return NULL;
    }

    PyTuple_SET_ITEM(args, 0, list); /* args will have the only reference to list. */

    result = PyObject_CallObject(function, args);

    Py_DECREF(args); /* Also removes list. */
    Py_DECREF(function);
    Py_DECREF(joiner);

    return result;
}

/* Adds an item to be joined. */
Py_LOCAL(int) add_item(JoinInfo* join_info, PyObject* item) {
    int status;

    /* If the list already exists then just add the item to it. */
    if (join_info->list) {
        status = PyList_Append(join_info->list, item);
        if (status < 0)
            goto error;

        return status;
    }

    /* If we already have an item then we now have 2(!) and need to put them
     * into a list.
     */
    if (join_info->item) {
        join_info->list = PyList_New(0);
        if (!join_info->list) {
            status = RE_ERROR_MEMORY;
            goto error;
        }

        status = PyList_Append(join_info->list, join_info->item);
        if (status < 0)
            goto error;

        status = PyList_Append(join_info->list, item);
        if (status < 0)
            goto error;

        return 0;
    }

    /* This is the first item. */
    join_info->item = item;
    Py_INCREF(join_info->item);

    return 0;

error:
    if (join_info->list)
        Py_DECREF(join_info->list);
    if (join_info->item)
        Py_DECREF(join_info->item);
    return status;
}

/* Joins together a list of strings for pattern_subx. */
Py_LOCAL(PyObject*) join_list_info(JoinInfo* join_info, PyObject* string) {
    /* If the list already exists then just do the join. */
    if (join_info->list) {
        Py_DECREF(join_info->item);
        return join_list(join_info->list, string, join_info->reversed);
    }

    /* If we have only 1 item then we _might_ be able to just return it. */
    if (join_info->item) {
        int status;

        /* We can return the single item only if it's the same type of string
         * as the joiner.
         */
        if (PyObject_Type(join_info->item) == PyObject_Type(string))
            return join_info->item;

        /* We'll default to the normal joining method, which requires the item
         * to be in a list.
         */
        join_info->list = PyList_New(0);
        if (!join_info->list) {
            Py_DECREF(join_info->item);
            return NULL;
        }

        status = PyList_Append(join_info->list, join_info->item);
        if (status < 0) {
            Py_DECREF(join_info->list);
            Py_DECREF(join_info->item);
            return NULL;
        }

        Py_DECREF(join_info->item);
        return join_list(join_info->list, string, join_info->reversed);
    }

    /* There are no items, so return an empty string. */
    return PySequence_GetSlice(string, 0, 0);
}

/* MatchObject's 'expand' method. */
static PyObject* match_expand(MatchObject* self, PyObject* str_template) {
    PyObject* replacement;
    JoinInfo join_info;
    Py_ssize_t size;
    Py_ssize_t i;

    /* Hand the template to the template compiler. */
    replacement = call(RE_MODULE, "_compile_replacement", PyTuple_Pack(2,
      self->pattern, str_template));
    if (!replacement)
        return NULL;

    join_info.list = NULL;
    join_info.item = NULL;
    join_info.reversed = FALSE;

    /* Add each part of the template to the list. */
    size = PyList_GET_SIZE(replacement);
    for (i = 0; i < size; i++) {
        PyObject* item;
        PyObject* str_item;

        item = PyList_GET_ITEM(replacement, i);
        str_item = get_match_replacement(self, item, self->string,
          self->group_count);
        if (!str_item)
            goto error;

        /* Add to the list. */
        if (str_item == Py_None)
            Py_DECREF(str_item);
        else {
            int status;

            status = add_item(&join_info, str_item);
            Py_DECREF(str_item);
            if (status < 0) {
                set_error(status, NULL);
                goto error;
            }
        }
    }

    Py_DECREF(replacement);

    /* Convert the list to a single string (also cleans up join_info). */
    return join_list_info(&join_info, self->string);

error:
    if (join_info.list)
        Py_DECREF(join_info.list);
    if (join_info.item)
        Py_DECREF(join_info.item);
    Py_DECREF(replacement);
    return NULL;
}

/* MatchObject's 'copy' method. */
static PyObject* match_copy(MatchObject* self, PyObject *unused) {
    PyErr_SetString(PyExc_TypeError, "cannot copy this MatchObject");
    return NULL;
}

/* MatchObject's 'deepcopy' method. */
static PyObject* match_deepcopy(MatchObject* self, PyObject* memo) {
    PyErr_SetString(PyExc_TypeError, "cannot deepcopy this MatchObject");
    return NULL;
}

/* MatchObject's 'regs' method. */
static PyObject* match_regs(MatchObject* self) {
    PyObject* regs;
    PyObject* item;
    Py_ssize_t index;

    regs = PyTuple_New(self->group_count + 1);
    if (!regs)
        return NULL;

    item = _pair(self->pos, self->endpos);
    if (!item) {
        Py_DECREF(regs);
        return NULL;
    }
    PyTuple_SET_ITEM(regs, 0, item);

    for (index = 0; index < self->group_count; index++) {
        item = _pair(self->marks[index * 2], self->marks[index * 2 + 1]);
        if (!item) {
            Py_DECREF(regs);
            return NULL;
        }
        PyTuple_SET_ITEM(regs, index + 1, item);
    }

    Py_INCREF(regs);
    self->regs = regs;

    return regs;
}

/* MatchObject's slice method. */
Py_LOCAL(PyObject*) match_get_group_slice(MatchObject* self, PySliceObject*
  slice) {
    Py_ssize_t start;
    Py_ssize_t end;
    Py_ssize_t step;
    Py_ssize_t slice_length;

    if (PySlice_GetIndicesEx(slice, self->group_count + 1, &start, &end, &step,
      &slice_length) < 0) {
        return NULL;
    }

    if (slice_length <= 0)
        return PyTuple_New(0);
    else {
        PyObject* result;
        long cur;
        Py_ssize_t i;

        result = PyTuple_New(slice_length);
        if (!result)
            return NULL;

        cur = start;
        for (i = 0; i < slice_length; i++) {
            PyTuple_SetItem(result, i, match_get_group_by_index(self, cur,
              Py_None));
            cur += step;
        }

        return result;
    }
}

/* MatchObject's length method. */
static Py_ssize_t match_length(MatchObject* self) {
    return self->group_count + 1;
}

/* MatchObject's subscript method. */
static PyObject* match_subscript(MatchObject* self, PyObject* item) {
    if (PySlice_Check(item))
        return match_get_group_slice(self, (PySliceObject*)item);

    return match_get_group(self, item, Py_None, TRUE);
}

/* The documentation of a MatchObject. */
PyDoc_STRVAR(match_group_doc,
    "group([group1, ...]) --> string or tuple of strings.\n\
    Return one or more subgroups of the match.  If there is a single argument,\n\
    the result is a single string; if there are multiple arguments, the result\n\
    is a tuple with one item per argument; if there are no arguments, the whole\n\
    match is returned. Group 0 is the whole match.");

PyDoc_STRVAR(match_start_doc,
    "start([group]) --> int.\n\
    Return the index of the start of a subgroup of the match.  Defaults to\n\
    group 0 which is the whole match.  Return -1 if the group exists but did\n\
    not contribute to the match.");

PyDoc_STRVAR(match_end_doc,
    "end([group]) --> int.\n\
    Return the index of the start of a subgroup of the match.  Defaults to\n\
    group 0 which is the whole match.  Return -1 if the group exists but did\n\
    not contribute to the match.");

PyDoc_STRVAR(match_span_doc,
    "span([group]) --> 2-tuple of int.\n\
    Return the 2-tuple of the indices of the start and end of a subgroup of the\n\
    match.  If a group did not contribute to the match, this is (-1, -1).\n\
    Defaults to group 0 which is the entire match.");

PyDoc_STRVAR(match_groups_doc,
    "groups(default=None) --> tuple of strings.\n\
    Return a tuple containing all the subgroups of the match.  The argument is\n\
    the default for groups that did not participate in the match.");

PyDoc_STRVAR(match_groupdict_doc,
    "groupdict(default=None) --> dict.\n\
    Return a dictionary containing all the named subgroups of the match, keyed\n\
    by the subgroup name.  The argument is groups that did not participate in\n\
    the match.");

PyDoc_STRVAR(match_expand_doc,
    "expand(template) --> string.\n\
    Return the string obtained by doing backslash substitution on the template,\n\
    as done by the sub() method.");

/* MatchObject's methods. */
static PyMethodDef match_methods[] = {
    {"group", (PyCFunction)match_group, METH_VARARGS, match_group_doc},
    {"start", (PyCFunction)match_start, METH_VARARGS, match_start_doc},
    {"end", (PyCFunction)match_end, METH_VARARGS, match_end_doc},
    {"span", (PyCFunction)match_span, METH_VARARGS, match_span_doc},
    {"groups", (PyCFunction)match_groups, METH_VARARGS|METH_KEYWORDS,
      match_groups_doc},
    {"groupdict", (PyCFunction)match_groupdict, METH_VARARGS|METH_KEYWORDS,
      match_groupdict_doc},
    {"expand", (PyCFunction)match_expand, METH_O, match_expand_doc},
    {"__copy__", (PyCFunction)match_copy, METH_NOARGS},
    {"__deepcopy__", (PyCFunction)match_deepcopy, METH_O},
    {"__getitem__", (PyCFunction)match_subscript, METH_O|METH_COEXIST},
    {NULL, NULL}
};

/* Gets an attribute of a MatchObject. */
static PyObject* match_getattr(MatchObject* self, char* name) {
    PyObject* res;

    res = Py_FindMethod(match_methods, (PyObject*)self, name);
    if (res)
        return res;

    PyErr_Clear();

    if (!strcmp(name, "lastindex")) {
        if (self->lastindex >= 0)
            return Py_BuildValue("i", self->lastindex);

        Py_INCREF(Py_None);
        return Py_None;
    }

    if (!strcmp(name, "lastgroup")) {
        if (self->pattern->indexgroup && self->lastgroup >= 0) {
            PyObject* index = Py_BuildValue("i", self->lastgroup);
            PyObject* result = PyDict_GetItem(self->pattern->indexgroup, index);
            Py_DECREF(index);
            if (result)
                return result;
            PyErr_Clear();
        }

        Py_INCREF(Py_None);
        return Py_None;
    }

    if (!strcmp(name, "regs")) {
        if (self->regs) {
            Py_INCREF(self->regs);
            return self->regs;
        } else
            return match_regs(self);
    }

    if (!strcmp(name, "string")) {
        if (self->string) {
            Py_INCREF(self->string);
            return self->string;
        } else {
            Py_INCREF(Py_None);
            return Py_None;
        }
    }

    if (!strcmp(name, "re")) {
        Py_INCREF(self->pattern);
        return (PyObject*)self->pattern;
    }

    if (!strcmp(name, "pos"))
        return Py_BuildValue("i", self->pos);

    if (!strcmp(name, "endpos"))
        return Py_BuildValue("i", self->endpos);

    PyErr_SetString(PyExc_AttributeError, name);
    return NULL;
}

static PyMappingMethods match_as_mapping = {
    (lenfunc)match_length,       /* mp_length */
    (binaryfunc)match_subscript, /* mp_subscript */
    0,                           /* mp_ass_subscript */
};

/* FIXME: implement setattr("string", None) as a special case (to detach the
 * associated string, if any.
 */
static PyTypeObject Match_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "_" RE_MODULE "." "Match",
    sizeof(MatchObject), 0,
    (destructor)match_dealloc,  /* tp_dealloc */
    0,                          /* tp_print */
    (getattrfunc)match_getattr, /* tp_getattr */
    0,                          /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    &match_as_mapping,          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    0,                          /* tp_getattro */
    0,                          /* tp_setattro */
    0,                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,         /* tp_flags */
    0,                          /* tp_doc */
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    match_methods,              /* tp_methods */
};

/* Creates a new MatchObject. */
Py_LOCAL(PyObject*) pattern_new_match(PatternObject* pattern, RE_State* state,
  int status) {
    /* Create MatchObject (from state object). */
    if (status > 0) {
        MatchObject* match;
        Py_ssize_t g;

        /* Create MatchObject. */
        match = PyObject_NEW(MatchObject, &Match_Type);
        if (!match)
            return NULL;

        match->string = state->string;
        match->pattern = pattern;
        match->regs = NULL;
        Py_INCREF(match->string);
        Py_INCREF(match->pattern);

        match->marks = (Py_ssize_t*)re_alloc(pattern->group_count * 2 *
          sizeof(Py_ssize_t));
        if (!match->marks) {
            Py_DECREF(match);
            return NULL;
        }

        match->regs = NULL;
        match->group_count = pattern->group_count;

        if (state->reverse) {
            match->pos = state->text_pos;
            match->endpos = state->match_pos;
        } else {
            match->pos = state->match_pos;
            match->endpos = state->text_pos;
        }

        /* Fill in group slices. */
        for (g = 0; g < pattern->group_count; g++) {
            Py_ssize_t m = g * 2;
            Py_ssize_t start = state->marks[m];
            Py_ssize_t end = state->marks[m + 1];
            if (0 <= start && start <= end) {
                match->marks[m] = start;
                match->marks[m + 1] = end;
            } else {
                match->marks[m] = -1;
                match->marks[m + 1] = -1;
            }
        }

        match->lastindex = state->lastindex;
        match->lastgroup = state->lastgroup;

        return (PyObject*)match;
    } else if (status == 0) {
        /* No match. */
        Py_INCREF(Py_None);
        return Py_None;
    }

    /* Internal error. */
    set_error(status, NULL);
    return NULL;
}

/* Gets the text of a capture group from a state. */
Py_LOCAL(PyObject*) state_get_group(RE_State* state, Py_ssize_t index,
  PyObject* string, BOOL empty) {
    Py_ssize_t m;
    Py_ssize_t start;
    Py_ssize_t end;

    m = index * 2 - 2;
    if (string != Py_None && index >= 1 && index <= state->pattern->group_count
      && state->marks[m] >= 0 && state->marks[m + 1] >= 0) {
        start = state->marks[m];
        end = state->marks[m + 1];
    } else {
        if (empty)
            /* Want an empty string. */
            start = end = 0;
        else {
            Py_INCREF(Py_None);
            return Py_None;
        }
    }

    return PySequence_GetSlice(string, start, end);
}

/* ScannerObject's 'match' method. */
static PyObject* scanner_match(ScannerObject* self, PyObject* unused) {
    RE_State* state;
    int status;
    PyObject* match;

    state = &self->state;

    status = do_match(state, FALSE);

    if (PyErr_Occurred())
        return NULL;

    match = pattern_new_match(self->pattern, state, status);

    /* Continue from where we left off, but don't allow a contiguous
     * zero-width match.
     */
    state->must_advance = state->text_pos == state->match_pos;

    return match;
}

/* ScannerObject's 'search' method. */
static PyObject* scanner_search(ScannerObject* self, PyObject *unused) {
    RE_State* state;
    int status;
    PyObject* match;

    state = &self->state;

    status = do_match(state, TRUE);

    if (PyErr_Occurred())
        return NULL;

    match = pattern_new_match(self->pattern, state, status);

    if (state->overlapped) {
        /* Advance one character. */
        Py_ssize_t step;

        step = state->reverse ? -1 : 1;
        state->text_pos = state->search_anchor + step;
        state->must_advance = FALSE;
    } else
        /* Continue from where we left off, but don't allow a contiguous
         * zero-width match.
         */
        state->must_advance = state->text_pos == state->match_pos;

    return match;
}

/* ScannerObject's methods. */
static PyMethodDef scanner_methods[] = {
    {"match", (PyCFunction)scanner_match, METH_NOARGS},
    {"search", (PyCFunction)scanner_search, METH_NOARGS},
    {NULL, NULL}
};

/* Deallocates a ScannerObject. */
static void scanner_dealloc(ScannerObject* self) {
    state_fini(&self->state);
    Py_DECREF(self->pattern);
    PyObject_DEL(self);
}

/* Gets an attribute of a ScannerObject. */
static PyObject* scanner_getattr(ScannerObject* self, char* name) {
    PyObject* res;

    res = Py_FindMethod(scanner_methods, (PyObject*)self, name);
    if (res)
        return res;

    PyErr_Clear();

    if (!strcmp(name, "pattern")) {
        Py_INCREF(self->pattern);
        return (PyObject*)self->pattern;
    }

    PyErr_SetString(PyExc_AttributeError, name);
    return NULL;
}

static PyTypeObject Scanner_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "_" RE_MODULE "." "Scanner",
    sizeof(ScannerObject), 0,
    (destructor)scanner_dealloc,  /* tp_dealloc */
    0,                            /* tp_print */
    (getattrfunc)scanner_getattr, /* tp_getattr */
};

/* Creates a new ScannerObject. */
static PyObject* pattern_scanner(PatternObject* pattern, PyObject* args,
  PyObject* kw) {
    /* Create search state object. */
    ScannerObject* self;
    Py_ssize_t start;
    Py_ssize_t end;

    PyObject* string;
    PyObject* pos = Py_None;
    PyObject* endpos = Py_None;
    Py_ssize_t overlapped = FALSE;
    static char* kwlist[] = { "string", "pos", "endpos", "overlapped", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|OOn:scanner", kwlist,
      &string, &pos, &endpos, &overlapped))
        return NULL;

    start = as_string_index(pos, 0);
    end = as_string_index(endpos, PY_SSIZE_T_MAX);
    if (PyErr_Occurred())
        return NULL;

    /* Create a scanner object. */
    self = PyObject_NEW(ScannerObject, &Scanner_Type);
    if (!self)
        return NULL;

    self->pattern = pattern;
    Py_INCREF(self->pattern);

    if (!state_init(&self->state, pattern, string, start, end, overlapped)) {
        PyObject_DEL(self);
        return NULL;
    }

    return (PyObject*) self;
}

/* SplitterObject's 'split' method. */
static PyObject* splitter_split(SplitterObject* self, PyObject *unused) {
    RE_State* state;
    PyObject* result;
    int status;

    if (self->finished) {
        result = Py_False;
        Py_INCREF(result);
        return result;
    }

    state = &self->state;

    if (self->index == 0) {
        if (self->split_count < self->maxsplit) {
            Py_ssize_t step;
            Py_ssize_t end_pos;

            if (state->reverse) {
                step = -1;
                end_pos = state->slice_start;
            } else {
                step = 1;
                end_pos = state->slice_end;
            }

retry:
            status = do_match(state, TRUE);

            if (PyErr_Occurred())
                goto error;

            if (status == RE_ERROR_SUCCESS) {
                if (!state->zero_width) {
                    /* The current behaviour is to advance one character if the
                     * split was zero-width. Unfortunately, this can give an
                     * incorrect result. GvR wants this behaviour to be
                     * retained so as not to break any existing software which
                     * might rely on it. The correct behaviour is enabled by
                     * setting the 'zero_width' flag.
                     */
                     if (state->text_pos == state->match_pos) {
                         if (self->last == end_pos)
                             goto no_match;

                         /* Advance one character. */
                         state->text_pos += step;
                         state->must_advance = FALSE;
                         goto retry;
                     }
                }

                ++self->split_count;

                /* Get segment before this match. */
                if (state->reverse)
                    result = PySequence_GetSlice(state->string,
                      state->match_pos, self->last);
                else
                    result = PySequence_GetSlice(state->string, self->last,
                      state->match_pos);
                if (!result)
                    goto error;

                self->last = state->text_pos;

                /* The correct behaviour is to reject a zero-width match just
                 * after a split point. The current behaviour is to advance one
                 * character if the match was zero-width. Unfortunately, this
                 * can give an incorrect result. GvR wants this behaviour to be
                 * retained so as not to break any existing software which
                 *  mightrely on it. The correct behaviour is enabled by
                 * setting the 'zero_width' flag.
                 */
                if (state->zero_width)
                    /* Continue from where we left off, but don't allow a
                     * contiguous zero-width match.
                     */
                    state->must_advance = TRUE;
                else {
                    if (state->text_pos == state->match_pos)
                        /* Advance one character. */
                        state->text_pos += step;

                    state->must_advance = FALSE;
                }
            }
        } else
            goto no_match;

        if (status == RE_ERROR_FAILURE) {
no_match:
            /* Get segment following last match (even if empty). */
            if (state->reverse)
                result = PySequence_GetSlice(state->string, 0, self->last);
            else
                result = PySequence_GetSlice(state->string, self->last,
                  state->text_length);
            if (!result)
                goto error;

            self->finished = TRUE;
        }
    } else {
        /* Add group. */
        result = state_get_group(state, self->index, state->string, FALSE);
        if (!result)
            goto error;
    }

    ++self->index;
    if (self->index > state->pattern->group_count)
        self->index = 0;

    return result;

error:
    state_fini(state);
    return NULL;
}

/* SplitterObject's methods. */
static PyMethodDef splitter_methods[] = {
    {"split", (PyCFunction)splitter_split, METH_NOARGS},
    {NULL, NULL}
};

/* Deallocates a SplitterObject. */
static void splitter_dealloc(SplitterObject* self) {
    state_fini(&self->state);
    Py_DECREF(self->pattern);
    PyObject_DEL(self);
}

/* Gets an attribute of a SplitterObject. */
static PyObject* splitter_getattr(SplitterObject* self, char* name) {
    PyObject* res;

    res = Py_FindMethod(splitter_methods, (PyObject*)self, name);
    if (res)
        return res;

    PyErr_Clear();

    if (!strcmp(name, "pattern")) {
        Py_INCREF(self->pattern);
        return (PyObject*)self->pattern;
    }

    PyErr_SetString(PyExc_AttributeError, name);
    return NULL;
}

static PyTypeObject Splitter_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "_" RE_MODULE "." "Splitter",
    sizeof(SplitterObject), 0,
    (destructor)splitter_dealloc,  /* tp_dealloc */
    0,                            /* tp_print */
    (getattrfunc)splitter_getattr, /* tp_getattr */
};

/* Creates a new SplitterObject. */
static PyObject* pattern_splitter(PatternObject* pattern, PyObject* args,
  PyObject* kw) {
    /* Create split state object. */
    SplitterObject* self;
    RE_State* state;

    PyObject* string;
    Py_ssize_t maxsplit = 0;
    static char* kwlist[] = { "string", "maxsplit", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|n:splitter", kwlist, &string,
      &maxsplit))
        return NULL;

    /* Create a splitter object. */
    self = PyObject_NEW(SplitterObject, &Splitter_Type);
    if (!self)
        return NULL;

    self->pattern = pattern;
    Py_INCREF(self->pattern);

    if (maxsplit == 0)
        maxsplit = PY_SSIZE_T_MAX;

    state = &self->state;

    if (!state_init(state, pattern, string, 0, PY_SSIZE_T_MAX, FALSE)) {
        PyObject_DEL(self);
        return NULL;
    }

    self->maxsplit = maxsplit;
    self->last = state->reverse ? state->text_length : 0;
    self->split_count = 0;
    self->index = 0;
    self->finished = FALSE;

    return (PyObject*) self;
}

/* PatternObject's 'match' method. */
static PyObject* pattern_match(PatternObject* self, PyObject* args, PyObject*
  kw) {
    Py_ssize_t start;
    Py_ssize_t end;
    RE_State state;
    int status;
    PyObject* match;

    PyObject* string;
    PyObject* pos = Py_None;
    PyObject* endpos = Py_None;
    static char* kwlist[] = { "string", "pos", "endpos", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|OO:match", kwlist, &string,
      &pos, &endpos))
        return NULL;

    start = as_string_index(pos, 0);
    end = as_string_index(endpos, PY_SSIZE_T_MAX);
    if (PyErr_Occurred())
        return NULL;

    if (!state_init(&state, self, string, start, end, FALSE))
        return NULL;

    status = do_match(&state, FALSE);

    if (PyErr_Occurred()) {
        state_fini(&state);
        return NULL;
    }

    match = pattern_new_match(self, &state, status);

    state_fini(&state);

    return match;
}

/* PatternObject's 'search' method. */
static PyObject* pattern_search(PatternObject* self, PyObject* args, PyObject*
  kw) {
    Py_ssize_t start;
    Py_ssize_t end;
    void* characters;
    Py_ssize_t length;
    Py_ssize_t charsize;
    RE_State state;
    int status;
    PyObject* match;

    PyObject* string;
    PyObject* pos = Py_None;
    PyObject* endpos = Py_None;
    static char* kwlist[] = { "string", "pos", "endpos", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|OO:search", kwlist, &string,
      &pos, &endpos))
        return NULL;

    start = as_string_index(pos, 0);
    end = as_string_index(endpos, PY_SSIZE_T_MAX);
    if (PyErr_Occurred())
        return NULL;

    /* Get the string. */
    if (!get_string(string, &characters, &length, &charsize))
        return NULL;

    /* Adjust boundaries. */
    if (start < 0)
        start += length;
    if (start < 0)
        start = 0;
    else if (start > length)
        start = length;

    if (end < 0)
        end += length;
    if (end < 0)
        end = 0;
    else if (end > length)
        end = length;

    /* If the pattern is too long for the string, then take a shortcut. */
    if ((Py_ssize_t)self->min_width > end - start) {
        /* No match. */
        Py_INCREF(Py_None);
        return Py_None;
    }

    if (!state_init_2(&state, self, string, characters, length, charsize,
      start, end, FALSE))
        return NULL;

    status = do_match(&state, TRUE);

    if (PyErr_Occurred()) {
        state_fini(&state);
        return NULL;
    }

    match = pattern_new_match(self, &state, status);

    state_fini(&state);

    return match;
}

/* Gets a replacement item from the replacement list.
 *
 * The replacement item could be a string literal or a group.
 *
 * It can return None to represent an empty string.
 */
Py_LOCAL(PyObject*) get_sub_replacement(PyObject* item, PyObject* string,
  RE_State* state, Py_ssize_t group_count) {
    long group;

    if (PyString_Check(item) || PyUnicode_Check(item)) {
        /* It's a literal, which can be added directly to the list. */
        Py_INCREF(item);
        return item;
    }

    /* Is it a group reference? */
    if (PyInt_Check(item))
        group = PyInt_AsLong(item);
    else if (PyLong_Check(item))
        group = PyLong_AsLong(item);
    else {
        /* Not a group either! */
        set_error(RE_ERROR_REPLACEMENT, NULL);
        return NULL;
    }

    if (PyErr_Occurred()) {
        set_error(RE_ERROR_REPLACEMENT, NULL);
        return NULL;
    }

    if (group == 0) {
        /* The entire matched portion of the string. */
        if (state->match_pos == state->text_pos) {
            /* Return None for "". */
            Py_INCREF(Py_None);
            return Py_None;
        }

        if (state->reverse)
            return PySequence_GetSlice(string, state->text_pos,
              state->match_pos);
        else
            return PySequence_GetSlice(string, state->match_pos,
              state->text_pos);
    } else if (1 <= group && group <= group_count) {
        /* A group. */
        Py_ssize_t m = group * 2 - 2;
        if (state->marks[m] < 0 || state->marks[m] == state->marks[m + 1]) {
            /* The group didn't match or is "", so return None for "". */
            Py_INCREF(Py_None);
            return Py_None;
        }

        return PySequence_GetSlice(string, state->marks[m], state->marks[m +
          1]);
    } else {
        /* No such group. */
        set_error(RE_ERROR_INVALID_GROUP_REF, NULL);
        return NULL;
    }
}

/* Checks whether a template is a literal.
 *
 * To keep it simple we'll say that a literal is a string which can be used
 * as-is, so no backslashes.
 *
 * Returns its length if it is a literal, otherwise -1.
 */
Py_LOCAL(int) check_template(PyObject* str_template) {
    void* characters;
    Py_ssize_t length;
    Py_ssize_t charsize;

    if (!get_string(str_template, &characters, &length, &charsize))
        return -1;

    if (charsize == 1) {
        char* c_ptr;
        char* end_ptr;

        c_ptr = (char*)characters;
        end_ptr = (char*)characters + length;

        while (c_ptr < end_ptr) {
            if (*c_ptr == '\\')
                return -1;

            ++c_ptr;
        }
    } else {
        Py_UNICODE* c_ptr;
        Py_UNICODE* end_ptr;

        c_ptr = (Py_UNICODE*)characters;
        end_ptr = (Py_UNICODE*)characters + length;

        while (c_ptr < end_ptr) {
            if (*c_ptr == '\\')
                return -1;

            ++c_ptr;
        }
    }

    return length;
}

/* PatternObject's 'subx' method. */
Py_LOCAL(PyObject*) pattern_subx(PatternObject* self, PyObject* str_template,
  PyObject* string, Py_ssize_t maxsub, Py_ssize_t subn) {
    void* characters;
    Py_ssize_t length;
    Py_ssize_t charsize;
    BOOL is_callable;
    BOOL is_literal;
    PyObject* replacement;
    RE_State state;
    JoinInfo join_info;
    Py_ssize_t sub_count;
    Py_ssize_t last;
    PyObject* item;
    Py_ssize_t end_pos;

    /* Get the string. */
    if (!get_string(string, &characters, &length, &charsize))
        return NULL;

    /* If the pattern is too long for the string, then take a shortcut. */
    if ((Py_ssize_t)self->min_width > length) {
        Py_INCREF(string);

        if (subn)
            return Py_BuildValue("Ni", string, 0);

        return string;
    }

    if (maxsub == 0)
        maxsub = PY_SSIZE_T_MAX;

    is_callable = FALSE;
    is_literal = FALSE;

    /* sub/subn takes either a function or a string template. */
    if (PyCallable_Check(str_template)) {
        /* It's callable. */
        is_callable = TRUE;

        replacement = str_template;
        Py_INCREF(replacement);
    } else {
        /* The template isn't callable, so is it a literal?
         *
         * To keep it simple we'll say that a literal is a string which can be
         * used as-is, so no backslashes.
         */
        Py_ssize_t literal_length;

        literal_length = check_template(str_template);
        if (literal_length >= 0) {
            /* It's a literal. */
            is_literal = TRUE;

            /* We'll represent "" with None. */
            replacement = literal_length > 0 ? str_template : Py_None;
            Py_INCREF(replacement);
        } else {
            /* The template isn't a literal either, so hand it over to the
             * template compiler.
             */
            replacement = call(RE_MODULE, "_compile_replacement",
              PyTuple_Pack(2, self, str_template));
            if (!replacement)
                return NULL;
        }
    }

    if (!state_init_2(&state, self, string, characters, length, charsize, 0,
      PY_SSIZE_T_MAX, FALSE)) {
        Py_DECREF(replacement);
        return NULL;
    }

    join_info.item = NULL;
    join_info.list = NULL;
    join_info.reversed = state.reverse;

    sub_count = 0;
    last = state.reverse ? state.text_length : 0;
    while (sub_count < maxsub) {
        int status;

        status = do_match(&state, TRUE);

        if (PyErr_Occurred())
            goto error;

        if (status == 0)
            break;

        /* Append the segment before this match. */
        if (state.match_pos != last) {
            if (state.reverse)
                item = PySequence_GetSlice(string, state.match_pos, last);
            else
                item = PySequence_GetSlice(string, last, state.match_pos);
            if (!item)
                goto error;

            /* Add to the list. */
            status = add_item(&join_info, item);
            Py_DECREF(item);
            if (status < 0) {
                set_error(status, NULL);
                goto error;
            }
        }

        /* Add this match. */
        if (is_callable) {
            /* Pass the MatchObject to the replacement function. */
            PyObject* match;
            PyObject* args;

            match = pattern_new_match(self, &state, 1);
            if (!match)
                goto error;
            args = PyTuple_Pack(1, match);
            if (!args) {
                Py_DECREF(match);
                goto error;
            }
            item = PyObject_CallObject(replacement, args);
            Py_DECREF(args);
            Py_DECREF(match);
            if (!item)
                goto error;

            /* Add to the list. */
            if (item != Py_None) {
                status = add_item(&join_info, item);
                Py_DECREF(item);
                if (status < 0) {
                    set_error(status, NULL);
                    goto error;
                }
            }
        } else if (is_literal) {
            /* The replacement is a literal string.
             *
             * None represents "", otherwise add it to the list.
             */
            if (replacement != Py_None) {
                status = add_item(&join_info, replacement);
                if (status < 0) {
                    set_error(status, NULL);
                    goto error;
                }
            }
        } else {
            /* The replacement is a list template. */
            Py_ssize_t size;
            Py_ssize_t i;

            /* Add each part of the template to the list. */
            size = PyList_GET_SIZE(replacement);
            for (i = 0; i < size; i++) {
                PyObject* item;
                PyObject* str_item;

                item = PyList_GET_ITEM(replacement, i);
                str_item = get_sub_replacement(item, string, &state,
                  self->group_count);
                if (!str_item)
                    goto error;

                /* Add to the list. */
                if (str_item == Py_None)
                    /* None for "". */
                    Py_DECREF(str_item);
                else {
                    status = add_item(&join_info, str_item);
                    Py_DECREF(str_item);
                    if (status < 0) {
                        set_error(status, NULL);
                        goto error;
                    }
                }
            }
        }

        ++sub_count;

        /* Continue from where we left off, but don't allow a contiguous
         * zero-width match.
         */
        last = state.text_pos;
        state.must_advance = TRUE;
    }

    /* Get the segment following the last match. */
    end_pos = state.reverse ? 0 : state.text_length;
    if (last != end_pos) {
        int status;

        /* The segment is part of the original string. */
        if (state.reverse)
            item = PySequence_GetSlice(string, 0, last);
        else
            item = PySequence_GetSlice(string, last, state.text_length);
        if (!item)
            goto error;
        status = add_item(&join_info, item);
        Py_DECREF(item);
        if (status < 0) {
            set_error(status, NULL);
            goto error;
        }
    }

    state_fini(&state);

    Py_DECREF(replacement);

    /* Convert the list to a single string (also cleans up join_info). */
    item = join_list_info(&join_info, string);
    if (!item)
        return NULL;

    if (subn)
        return Py_BuildValue("Ni", item, sub_count);

    return item;

error:
    if (join_info.list)
        Py_DECREF(join_info.list);
    if (join_info.item)
        Py_DECREF(join_info.item);
    state_fini(&state);
    Py_DECREF(replacement);
    return NULL;
}

/* PatternObject's 'sub' method. */
static PyObject* pattern_sub(PatternObject* self, PyObject* args, PyObject* kw)
  {
    PyObject* ptemplate;
    PyObject* string;
    Py_ssize_t count = 0;

    static char* kwlist[] = { "repl", "string", "count", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "OO|n:sub", kwlist, &ptemplate,
      &string, &count))
        return NULL;

    return pattern_subx(self, ptemplate, string, count, 0);
}

/* PatternObject's 'subn' method. */
static PyObject* pattern_subn(PatternObject* self, PyObject* args, PyObject*
  kw) {
    PyObject* ptemplate;
    PyObject* string;
    Py_ssize_t count = 0;

    static char* kwlist[] = { "repl", "string", "count", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "OO|n:subn", kwlist, &ptemplate,
      &string, &count))
        return NULL;

    return pattern_subx(self, ptemplate, string, count, 1);
}

/* PatternObject's 'split' method. */
static PyObject* pattern_split(PatternObject* self, PyObject* args, PyObject*
  kw) {
    RE_State state;
    PyObject* list;
    PyObject* item;
    int status;
    Py_ssize_t split_count;
    Py_ssize_t i;
    Py_ssize_t start_pos;
    Py_ssize_t end_pos;
    Py_ssize_t step;
    Py_ssize_t last;

    PyObject* string;
    Py_ssize_t maxsplit = 0;
    static char* kwlist[] = { "string", "maxsplit", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|n:split", kwlist, &string,
      &maxsplit))
        return NULL;

    if (maxsplit == 0)
        maxsplit = PY_SSIZE_T_MAX;

    if (!state_init(&state, self, string, 0, PY_SSIZE_T_MAX, FALSE))
        return NULL;

    list = PyList_New(0);
    if (!list) {
        state_fini(&state);
        return NULL;
    }

    split_count = 0;
    if (state.reverse) {
        start_pos = state.text_length;
        end_pos = 0;
        step = -1;
    } else {
        start_pos = 0;
        end_pos = state.text_length;
        step = 1;
    }
    last = start_pos;
    while (split_count < maxsplit) {
        status = do_match(&state, TRUE);

        if (PyErr_Occurred())
            goto error;

        if (status == 0)
            /* No more matches. */
            break;

        if (!state.zero_width) {
            /* The current behaviour is to advance one character if the split
             * was zero-width. Unfortunately, this can give an incorrect
             * result. GvR wants this behaviour to be retained so as not to
             * break any existing software which might rely on it. The correct
             * behaviour is enabled by setting the 'zero_width' flag.
             */
            if (state.text_pos == state.match_pos) {
                if (last == end_pos)
                    break;

                /* Advance one character. */
                state.text_pos += step;
                state.must_advance = FALSE;
                continue;
            }
        }

        /* Get segment before this match. */
        if (state.reverse)
            item = PySequence_GetSlice(string, state.match_pos, last);
        else
            item = PySequence_GetSlice(string, last, state.match_pos);
        if (!item)
            goto error;
        status = PyList_Append(list, item);
        Py_DECREF(item);
        if (status < 0)
            goto error;

        /* Add groups (if any). */
        for (i = 1; i <= self->group_count; i++) {
            item = state_get_group(&state, i, string, FALSE);
            if (!item)
                goto error;
            status = PyList_Append(list, item);
            Py_DECREF(item);
            if (status < 0)
                goto error;
        }

        ++split_count;
        last = state.text_pos;

        /* The correct behaviour is to reject a zero-width match just after a
         * split point. The current behaviour is to advance one character if
         * the match was zero-width. Unfortunately, this can give an incorrect
         * result. GvR wants this behaviour to be retained so as not to break
         * any existing software which might rely on it. The correct behaviour
         * is enabled by setting the 'zero_width' flag.
         */
        if (state.zero_width)
            /* Continue from where we left off, but don't allow a contiguous
             * zero-width match.
             */
            state.must_advance = TRUE;
        else {
            if (state.text_pos == state.match_pos)
                /* Advance one character. */
                state.text_pos += step;

            state.must_advance = FALSE;
        }
    }

    /* Get segment following last match (even if empty). */
    if (state.reverse)
        item = PySequence_GetSlice(string, 0, last);
    else
        item = PySequence_GetSlice(string, last, state.text_length);
    if (!item)
        goto error;
    status = PyList_Append(list, item);
    Py_DECREF(item);
    if (status < 0)
        goto error;

    state_fini(&state);

    return list;

error:
    Py_DECREF(list);
    state_fini(&state);
    return NULL;
}

/* PatternObject's 'splititer' method. */
static PyObject* pattern_splititer(PatternObject* pattern, PyObject* args,
  PyObject* kw) {
    PyObject* splitter;
    PyObject* split;
    PyObject* iterator;

    splitter = pattern_splitter(pattern, args, kw);
    if (!splitter)
        return NULL;

    split = PyObject_GetAttrString(splitter, "split");
    Py_DECREF(splitter);
    if (!split)
        return NULL;

    iterator = PyCallIter_New(split, Py_False);
    Py_DECREF(split);

    return iterator;
}

/* PatternObject's 'findall' method. */
static PyObject* pattern_findall(PatternObject* self, PyObject* args, PyObject*
  kw) {
    Py_ssize_t start;
    Py_ssize_t end;
    RE_State state;
    PyObject* list;
    Py_ssize_t step;
    int status;
    Py_ssize_t i, b, e;

    PyObject* string;
    PyObject* pos = Py_None;
    PyObject* endpos = Py_None;
    Py_ssize_t overlapped = FALSE;
    static char* kwlist[] = { "string", "pos", "endpos", "overlapped", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|OOn:findall", kwlist,
      &string, &pos, &endpos, &overlapped))
        return NULL;

    start = as_string_index(pos, 0);
    end = as_string_index(endpos, PY_SSIZE_T_MAX);
    if (PyErr_Occurred())
        return NULL;

    if (!state_init(&state, self, string, start, end, overlapped))
        return NULL;

    list = PyList_New(0);
    if (!list) {
        state_fini(&state);
        return NULL;
    }

    step = state.reverse ? -1 : 1;
    while (state.slice_start <= state.text_pos && state.text_pos <=
      state.slice_end) {
        PyObject* item;

        status = do_match(&state, TRUE);

        if (PyErr_Occurred())
            goto error;

        if (status == 0)
            break;

        /* Don't bother to build a MatchObject. */
        switch (self->group_count) {
        case 0:
            if (state.reverse) {
                b = state.text_pos;
                e = state.match_pos;
            } else {
                b = state.match_pos;
                e = state.text_pos;
            }
            item = PySequence_GetSlice(string, b, e);
            if (!item)
                goto error;
            break;
        case 1:
            item = state_get_group(&state, 1, string, TRUE);
            if (!item)
                goto error;
            break;
        default:
            item = PyTuple_New(self->group_count);
            if (!item)
                goto error;
            for (i = 0; i < self->group_count; i++) {
                PyObject* o = state_get_group(&state, i + 1, string, TRUE);
                if (!o) {
                    Py_DECREF(item);
                    goto error;
                }
                PyTuple_SET_ITEM(item, i, o);
            }
            break;
        }

        status = PyList_Append(list, item);
        Py_DECREF(item);
        if (status < 0)
            goto error;

        if (state.overlapped) {
            /* Advance one character. */
            state.text_pos = state.search_anchor + step;
            state.must_advance = FALSE;
        } else
            /* Continue from where we left off, but don't allow a contiguous
             * zero-width match.
             */
            state.must_advance = state.text_pos == state.match_pos;
    }

    state_fini(&state);

    return list;

error:
    Py_DECREF(list);
    state_fini(&state);
    return NULL;
}

/* PatternObject's 'finditer' method. */
static PyObject* pattern_finditer(PatternObject* pattern, PyObject* args,
  PyObject* kw) {
    PyObject* scanner;
    PyObject* search;
    PyObject* iterator;

    scanner = pattern_scanner(pattern, args, kw);
    if (!scanner)
        return NULL;

    search = PyObject_GetAttrString(scanner, "search");
    Py_DECREF(scanner);
    if (!search)
        return NULL;

    iterator = PyCallIter_New(search, Py_None);
    Py_DECREF(search);

    return iterator;
}

/* PatternObject's 'copy' method. */
static PyObject* pattern_copy(PatternObject* self, PyObject *unused) {
    PyErr_SetString(PyExc_TypeError, "cannot copy this PatternObject");
    return NULL;
}

/* PatternObject's 'deepcopy' method. */
static PyObject* pattern_deepcopy(PatternObject* self, PyObject* memo) {
    PyErr_SetString(PyExc_TypeError, "cannot deepcopy this PatternObject");
    return NULL;
}

/* The documentation of a PatternObject. */
PyDoc_STRVAR(pattern_match_doc,
    "match(string[, pos[, endpos]]) --> MatchObject or None.\n\
    Matches zero or more characters at the beginning of the string.");

PyDoc_STRVAR(pattern_search_doc,
    "search(string[, pos[, endpos]]) --> MatchObject or None.\n\
    Scan through string looking for a match, and return a corresponding\n\
    MatchObject instance.  Return None if no position in the string matches.");

PyDoc_STRVAR(pattern_sub_doc,
    "sub(repl, string[, count=0]) --> newstring\n\
    Return the string obtained by replacing the leftmost non-overlapping\n\
    occurrences of pattern in string by the replacement repl.");

PyDoc_STRVAR(pattern_subn_doc,
    "subn(repl, string[, count=0]) --> (newstring, number of subs)\n\
    Return the tuple (new_string, number_of_subs_made) found by replacing the\n\
    leftmost non-overlapping occurrences of pattern with the replacement repl.");

PyDoc_STRVAR(pattern_split_doc,
    "split(string[, maxsplit=0]) --> list.\n\
    Split string by the occurrences of pattern.");

PyDoc_STRVAR(pattern_splititer_doc,
    "splititer(string[, maxsplit=0]) --> iterator.\n\
    Return an iterator yielding the parts of a split string.");

PyDoc_STRVAR(pattern_findall_doc,
    "findall(string[, pos[, endpos[, overlapped=False]]]) --> list.\n\
    Return a list of all matches of pattern in string.  The matches may be\n\
    overlapped if overlapped is True.");

PyDoc_STRVAR(pattern_finditer_doc,
    "finditer(string[, pos[, endpos[, overlapped=False]]]) --> iterator.\n\
    Return an iterator over all matches for the RE pattern in string.  The\n\
    matches may be overlapped if overlapped is True.  For each match, the\n\
    iterator returns a MatchObject.");

/* The methods of a PatternObject. */
static PyMethodDef pattern_methods[] = {
    {"match", (PyCFunction)pattern_match, METH_VARARGS|METH_KEYWORDS,
      pattern_match_doc},
    {"search", (PyCFunction)pattern_search, METH_VARARGS|METH_KEYWORDS,
      pattern_search_doc},
    {"sub", (PyCFunction)pattern_sub, METH_VARARGS|METH_KEYWORDS,
      pattern_sub_doc},
    {"subn", (PyCFunction)pattern_subn, METH_VARARGS|METH_KEYWORDS,
      pattern_subn_doc},
    {"split", (PyCFunction)pattern_split, METH_VARARGS|METH_KEYWORDS,
      pattern_split_doc},
    {"splititer", (PyCFunction)pattern_splititer, METH_VARARGS|METH_KEYWORDS,
      pattern_splititer_doc},
    {"findall", (PyCFunction)pattern_findall, METH_VARARGS|METH_KEYWORDS,
      pattern_findall_doc},
    {"finditer", (PyCFunction)pattern_finditer, METH_VARARGS|METH_KEYWORDS,
      pattern_finditer_doc},
    {"scanner", (PyCFunction)pattern_scanner, METH_VARARGS|METH_KEYWORDS},
    {"__copy__", (PyCFunction)pattern_copy, METH_NOARGS},
    {"__deepcopy__", (PyCFunction)pattern_deepcopy, METH_O},
    {NULL, NULL}
};

PyDoc_STRVAR(pattern_doc, "Compiled regular expression objects");

/* Deallocates a PatternObject. */
static void pattern_dealloc(PatternObject* self) {
    Py_ssize_t i;

    /* Discard the nodes. */
    for (i = 0; i < self->node_count; i++) {
        RE_Node* node;

        node = self->node_list[i];
        re_dealloc(node->values);
        re_dealloc(node->bad_character_offset);
        re_dealloc(node->good_suffix_offset);
        re_dealloc(node);
    }
    re_dealloc(self->node_list);

    /* Discard the group info. */
    re_dealloc(self->group_info);

    /* Discard the repeat info. */
    re_dealloc(self->repeat_info);

    re_dealloc(self->marks_storage);
    re_dealloc(self->saved_groups_storage);
    re_dealloc(self->data_storage);

    if (self->weakreflist)
        PyObject_ClearWeakRefs((PyObject*)self);
    Py_XDECREF(self->pattern);
    Py_XDECREF(self->groupindex);
    Py_XDECREF(self->indexgroup);
    PyObject_DEL(self);
}

/* Gets an attribute of a PatternObject. */
static PyObject* pattern_getattr(PatternObject* self, char* name) {
    PyObject* res;

    res = Py_FindMethod(pattern_methods, (PyObject*)self, name);
    if (res)
        return res;

    PyErr_Clear();

    if (!strcmp(name, "pattern")) {
        Py_INCREF(self->pattern);
        return (PyObject*)self->pattern;
    }

    if (!strcmp(name, "flags"))
        return Py_BuildValue("i", self->flags);

    if (!strcmp(name, "groups"))
        return Py_BuildValue("i", self->group_count);

    if (!strcmp(name, "groupindex")) {
        Py_INCREF(self->groupindex);
        return (PyObject*)self->groupindex;
    }

    PyErr_SetString(PyExc_AttributeError, name);
    return NULL;
}

static PyTypeObject Pattern_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "_" RE_MODULE "." "Pattern",
    sizeof(PatternObject), 0,
    (destructor)pattern_dealloc,            /* tp_dealloc */
    0,                                      /* tp_print */
    (getattrfunc)pattern_getattr,           /* tp_getattr */
    0,                                      /* tp_setattr */
    0,                                      /* tp_compare */
    0,                                      /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    0,                                      /* tp_hash */
    0,                                      /* tp_call */
    0,                                      /* tp_str */
    0,                                      /* tp_getattro */
    0,                                      /* tp_setattro */
    0,                                      /* tp_as_buffer */
    Py_TPFLAGS_HAVE_WEAKREFS,               /* tp_flags */
    pattern_doc,                            /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    offsetof(PatternObject, weakreflist),   /* tp_weaklistoffset */
};

/* Build the tables for a Boyer-Moore fast string search. */
Py_LOCAL(BOOL) build_fast_tables(RE_Node* node) {
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad;
    Py_ssize_t* good;
    RE_CODE ch;
    Py_ssize_t last_pos;
    Py_ssize_t pos;
    Py_ssize_t suffix_len;
    BOOL saved_start;
    Py_ssize_t s;
    Py_ssize_t i;
    Py_ssize_t s_start;

    length = node->value_count;

    if (length < RE_MIN_FAST_LENGTH)
        return TRUE;

    values = node->values;

    bad = (Py_ssize_t*)re_alloc(256 * sizeof(bad[0]));
    good = (Py_ssize_t*)re_alloc(length * sizeof(good[0]));

    if (!bad || !good) {
        re_dealloc(bad);
        re_dealloc(good);

        return FALSE;
    }

    for (ch = 0; ch < 256; ch++)
        bad[ch] = length;

    last_pos = length - 1;

    for (pos = 0; pos < last_pos; pos++) {
        Py_ssize_t offset;

        offset = last_pos - pos;
        ch = values[pos] & 0xFF;
        bad[ch] = offset;
    }

    suffix_len = 2;
    pos = length - suffix_len;
    saved_start = FALSE;
    s = pos - 1;
    i = suffix_len - 1;
    while (pos >= 0) {
        /* Look for another occurrence of the suffix. */
        while (i > 0) {
            /* Have we dropped off the end of the string? */
            if (s + i < 0)
                break;

            if (values[s + i] == values[pos + i])
                /* It still matches. */
                --i;
            else {
                /* Start again further along. */
                --s;
                i = suffix_len - 1;
            }
        }

        if (s >= 0 && values[s] == values[pos]) {
            /* We haven't dropped off the end of the string, and the suffix
             * has matched this far, so this is a good starting point for
             * the next iteration.
             */
            --s;
            if (!saved_start) {
                s_start = s;
                saved_start = TRUE;
            }
        } else {
            /* Calculate the suffix offset. */
            good[pos] = pos - s;

            /* Extend the suffix and start searching for _this_ one. */
            --pos;
            ++suffix_len;

            /* Where's a good place to start searching? */
            if (saved_start) {
                s = s_start;
                saved_start = FALSE;
            } else
                --s;

            /* Can we short-circuit the searching? */
            if (s < 0)
                break;
        }

        i = suffix_len - 1;
    }

    /* Fill-in any remaining entries. */
    while (pos >= 0) {
        good[pos] = pos - s;
        --pos;
        --s;
    }

    node->bad_character_offset = bad;
    node->good_suffix_offset = good;

    return TRUE;
}

/* Build the tables for a Boyer-Moore fast string search, ignoring case. */
Py_LOCAL(BOOL) build_fast_tables_ign(RE_EncodingTable* encoding, RE_Node* node)
  {
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad;
    Py_ssize_t* good;
    RE_CODE ch;
    Py_ssize_t last_pos;
    Py_ssize_t pos;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    Py_ssize_t suffix_len;
    BOOL saved_start;
    Py_ssize_t s;
    Py_ssize_t i;
    Py_ssize_t s_start;

    length = node->value_count;

    if (length < RE_MIN_FAST_LENGTH)
        return TRUE;

    values = node->values;

    bad = (Py_ssize_t*)re_alloc(256 * sizeof(bad[0]));
    good = (Py_ssize_t*)re_alloc(length * sizeof(good[0]));

    if (!bad || !good) {
        re_dealloc(bad);
        re_dealloc(good);

        return FALSE;
    }

    for (ch = 0; ch < 256; ch++)
        bad[ch] = length;

    last_pos = length - 1;

    for (pos = 0; pos < last_pos; pos++) {
        Py_ssize_t offset;
        RE_CODE ch_lower;
        RE_CODE ch_upper;
        RE_CODE ch_title;

        offset = last_pos - pos;
        bad[ch & 0xFF] = offset;
        ch_lower = encoding->lower(values[pos]);
        bad[ch_lower & 0xFF] = offset;
        ch_upper = encoding->upper(values[pos]);
        bad[ch_upper & 0xFF] = offset;
        if (ch_lower != ch_upper) {
            ch_title = encoding->title(values[pos]);
            bad[ch_title & 0xFF] = offset;
        }
    }

    same_char_ign = encoding->same_char_ign;

    suffix_len = 2;
    pos = length - suffix_len;
    saved_start = FALSE;
    s = pos - 1;
    i = suffix_len - 1;
    while (pos >= 0) {
        /* Look for another occurrence of the suffix. */
        while (i > 0) {
            /* Have we dropped off the end of the string? */
            if (s + i < 0)
                break;

            if (same_char_ign(values[s + i], values[pos + i]))
                /* It still matches. */
                --i;
            else {
                /* Start again further along. */
                --s;
                i = suffix_len - 1;
            }
        }

        if (s >= 0 && same_char_ign(values[s], values[pos])) {
            /* We haven't dropped off the end of the string, and the suffix
             * has matched this far, so this is a good starting point for
             * the next iteration.
             */
            --s;
            if (!saved_start) {
                s_start = s;
                saved_start = TRUE;
            }
        } else {
            /* Calculate the suffix offset. */
            good[pos] = pos - s;

            /* Extend the suffix and start searching for _this_ one. */
            --pos;
            ++suffix_len;

            /* Where's a good place to start searching? */
            if (saved_start) {
                s = s_start;
                saved_start = FALSE;
            } else
                --s;

            /* Can we short-circuit the searching? */
            if (s < 0)
                break;
        }

        i = suffix_len - 1;
    }

    /* Fill-in any remaining entries. */
    while (pos >= 0) {
        good[pos] = pos - s;
        --pos;
        --s;
    }

    node->bad_character_offset = bad;
    node->good_suffix_offset = good;

    return TRUE;
}

/* Build the tables for a Boyer-Moore fast string search backwards, ignoring
 * case. */
Py_LOCAL(BOOL) build_fast_tables_ign_rev(RE_EncodingTable* encoding, RE_Node*
  node) {
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad;
    Py_ssize_t* good;
    RE_CODE ch;
    Py_ssize_t pos;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    Py_ssize_t suffix_len;
    BOOL saved_start;
    Py_ssize_t s;
    Py_ssize_t i;
    Py_ssize_t s_start;

    length = node->value_count;

    if (length < RE_MIN_FAST_LENGTH)
        return TRUE;

    values = node->values;

    bad = (Py_ssize_t*)re_alloc(256 * sizeof(bad[0]));
    good = (Py_ssize_t*)re_alloc(length * sizeof(good[0]));

    if (!bad || !good) {
        re_dealloc(bad);
        re_dealloc(good);

        return FALSE;
    }

    for (ch = 0; ch < 256; ch++)
        bad[ch] = -length;

    for (pos = length - 1; pos >= 1; pos--) {
        Py_ssize_t offset;
        RE_CODE ch_lower;
        RE_CODE ch_upper;
        RE_CODE ch_title;

        offset = -pos;
        bad[ch & 0xFF] = offset;
        ch_lower = encoding->lower(values[pos]);
        bad[ch_lower & 0xFF] = offset;
        ch_upper = encoding->upper(values[pos]);
        bad[ch_upper & 0xFF] = offset;
        if (ch_lower != ch_upper) {
            ch_title = encoding->title(values[pos]);
            bad[ch_title & 0xFF] = offset;
        }
    }

    same_char_ign = encoding->same_char_ign;

    suffix_len = 2;
    pos = suffix_len - 1;
    saved_start = FALSE;
    s = pos + 1;
    i = suffix_len - 1;
    while (pos < length) {
        /* Look for another occurrence of the suffix. */
        while (i > 0) {
            /* Have we dropped off the end of the string? */
            if (s - i >= length)
                break;

            if (same_char_ign(values[s - i], values[pos - i]))
                /* It still matches. */
                --i;
            else {
                /* Start again further along. */
                ++s;
                i = suffix_len - 1;
            }
        }

        if (s < length && same_char_ign(values[s], values[pos])) {
            /* We haven't dropped off the end of the string, and the suffix
             * has matched this far, so this is a good starting point for
             * the next iteration.
             */
            ++s;
            if (!saved_start) {
                s_start = s;
                saved_start = TRUE;
            }
        } else {
            /* Calculate the suffix offset. */
            good[pos] = pos - s;

            /* Extend the suffix and start searching for _this_ one. */
            ++pos;
            ++suffix_len;

            /* Where's a good place to start searching? */
            if (saved_start) {
                s = s_start;
                saved_start = FALSE;
            } else
                ++s;

            /* Can we short-circuit the searching? */
            if (s >= length)
                break;
        }

        i = suffix_len - 1;
    }

    /* Fill-in any remaining entries. */
    while (pos < length) {
        good[pos] = pos - s;
        ++pos;
        ++s;
    }

    node->bad_character_offset = bad;
    node->good_suffix_offset = good;

    return TRUE;
}

/* Build the tables for a Boyer-Moore fast string search backwards. */
Py_LOCAL(BOOL) build_fast_tables_rev(RE_Node* node) {
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad;
    Py_ssize_t* good;
    RE_CODE ch;
    Py_ssize_t pos;
    Py_ssize_t suffix_len;
    BOOL saved_start;
    Py_ssize_t s;
    Py_ssize_t i;
    Py_ssize_t s_start;

    length = node->value_count;

    if (length < RE_MIN_FAST_LENGTH)
        return TRUE;

    values = node->values;

    bad = (Py_ssize_t*)re_alloc(256 * sizeof(bad[0]));
    good = (Py_ssize_t*)re_alloc(length * sizeof(good[0]));

    if (!bad || !good) {
        re_dealloc(bad);
        re_dealloc(good);

        return FALSE;
    }

    for (ch = 0; ch < 256; ch++)
        bad[ch] = -length;

    for (pos = length - 1; pos >= 1; pos--) {
        Py_ssize_t offset;

        offset = -pos;
        ch = values[pos] & 0xFF;
        bad[ch] = offset;
    }

    suffix_len = 2;
    pos = suffix_len - 1;
    saved_start = FALSE;
    s = pos + 1;
    i = suffix_len - 1;
    while (pos < length) {
        /* Look for another occurrence of the suffix. */
        while (i > 0) {
            /* Have we dropped off the end of the string? */
            if (s - i >= length)
                break;

            if (values[s - i] == values[pos - i])
                /* It still matches. */
                --i;
            else {
                /* Start again further along. */
                ++s;
                i = suffix_len - 1;
            }
        }

        if (s < length && values[s] == values[pos]) {
            /* We haven't dropped off the end of the string, and the suffix
             * has matched this far, so this is a good starting point for
             * the next iteration.
             */
            ++s;
            if (!saved_start) {
                s_start = s;
                saved_start = TRUE;
            }
        } else {
            /* Calculate the suffix offset. */
            good[pos] = pos - s;

            /* Extend the suffix and start searching for _this_ one. */
            ++pos;
            ++suffix_len;

            /* Where's a good place to start searching? */
            if (saved_start) {
                s = s_start;
                saved_start = FALSE;
            } else
                ++s;

            /* Can we short-circuit the searching? */
            if (s >= length)
                break;
        }

        i = suffix_len - 1;
    }

    /* Fill-in any remaining entries. */
    while (pos < length) {
        good[pos] = pos - s;
        ++pos;
        ++s;
    }

    node->bad_character_offset = bad;
    node->good_suffix_offset = good;

    return TRUE;
}

/* Building the nodes is made simpler by allowing branches to have a single
 * exit. These need to be removed.
 */
Py_LOCAL(void) skip_one_way_branches(PatternObject* pattern) {
    BOOL modified;

    /* If a node refers to a 1-way branch then make the former refer to the
     * latter's destination. Repeat until they're all done.
     */
    do {
        Py_ssize_t i;

        modified = FALSE;

        for (i = 0; i < pattern->node_count; i++) {
            RE_Node* node;
            RE_Node* next;

            node = pattern->node_list[i];

            /* Check the first destination. */
            next = node->next_1.node;
            if (next && next->op == RE_OP_BRANCH && !next->next_2.node) {
                node->next_1.node = next->next_1.node;
                modified = TRUE;
            }

            /* Check the second destination. */
            next = node->next_2.node;
            if (next && next->op == RE_OP_BRANCH && !next->next_2.node) {
                node->next_2.node = next->next_1.node;
                modified = TRUE;
            }
        }
    } while (modified);

    /* The start node might be a 1-way branch. Skip over it because it'll be
     * removed. It might even be the first in a chain.
     */
    while (pattern->start_node->op == RE_OP_BRANCH &&
      !pattern->start_node->next_2.node)
        pattern->start_node = pattern->start_node->next_1.node;
}

/* Marks nodes which are being used as used. */
Py_LOCAL(void) use_nodes(RE_Node* node) {
    while (node && !node->used) {
        node->used = TRUE;
        if (node->next_1.node) {
            if (node->next_2.node)
                use_nodes(node->next_2.node);
            node = node->next_1.node;
        } else
            node = node->next_2.node;
    }
}

/* Discards any unused nodes.
 *
 * Optimising the nodes might result in some nodes no longer being used.
 */
Py_LOCAL(void) discard_unused_nodes(PatternObject* pattern) {
    Py_ssize_t new_count;
    Py_ssize_t i;

    /* Mark the nodes which are being used. */
    use_nodes(pattern->start_node);

    new_count = 0;
    for (i = 0; i < pattern->node_count; i++) {
        RE_Node* node;

        node = pattern->node_list[i];
        if (node->used)
            pattern->node_list[new_count++] = node;
        else {
            re_dealloc(node->values);
            re_dealloc(node->bad_character_offset);
            re_dealloc(node->good_suffix_offset);
            re_dealloc(node);
        }
    }

    pattern->node_count = new_count;
}

/* Assigns offsets for the groups into the values array.
 *
 * Entries for groups come before those for repeats.
 */
Py_LOCAL(BOOL) assign_group_offsets(PatternObject* pattern) {
    Py_ssize_t offset;
    Py_ssize_t i;

    offset = 0;

    for (i = 0; i < pattern->group_count; i++) {
        RE_GroupInfo* group_info;

        group_info = &pattern->group_info[i];
        if (group_info->id < 1) {
            /* There's an entry for a non-existent group. */
            set_error(RE_ERROR_ILLEGAL, NULL);
            return FALSE;
        }

         group_info->value_offset = offset++;
    }

    return TRUE;
}

/* Marks all the group which are named. */
Py_LOCAL(void) mark_named_groups(PatternObject* pattern) {
    Py_ssize_t i;

    for (i = 0; i < pattern->group_count; i++) {
        RE_GroupInfo* group_info;
        PyObject* index;

        group_info = &pattern->group_info[i];
        index = Py_BuildValue("i", i + 1);
        group_info->has_name = PyDict_Contains(pattern->indexgroup, index);
        Py_DECREF(index);
    }
}

/* Gets the test node. */
Py_LOCAL(void) set_test_node(RE_NextNode* next) {
    RE_Node* node = next->node;
    RE_Node* test;

    next->test = node;
    next->match_next = node;
    next->match_step = 0;

    if (!node)
        return;

    test = node;
    while (test->op == RE_OP_BEGIN_GROUP || test->op == RE_OP_END_GROUP)
        test = test->next_1.node;

    next->test = test;

    if (test != node)
        return;

    switch (test->op) {
    case RE_OP_ANY: /* Any character, except newline. */
    case RE_OP_ANY_ALL: /* Any character at all. */
    case RE_OP_ANY_ALL_REV: /* Any character at all. */
    case RE_OP_ANY_REV: /* Any character, except newline. */
    case RE_OP_BIG_BITSET: /* Big bitset. */
    case RE_OP_BIG_BITSET_REV: /* Big bitset. */
    case RE_OP_BOUNDARY: /* At word boundary. */
    case RE_OP_CHARACTER: /* Character literal. */
    case RE_OP_CHARACTER_IGN: /* Character literal, ignoring case. */
    case RE_OP_CHARACTER_IGN_REV: /* Character literal, ignoring case. */
    case RE_OP_CHARACTER_REV: /* Character literal. */
    case RE_OP_DEFAULT_BOUNDARY: /* At a default word boundary. */
    case RE_OP_END_OF_LINE: /* At the end of a line. */
    case RE_OP_END_OF_STRING: /* At the end of the string. */
    case RE_OP_END_OF_STRING_LINE: /* At end of string or final newline. */
    case RE_OP_PROPERTY: /* Character property. */
    case RE_OP_PROPERTY_REV: /* Character property. */
    case RE_OP_SEARCH_ANCHOR: /* At the start of the search. */
    case RE_OP_SET: /* Character set. */
    case RE_OP_SET_REV: /* Character set. */
    case RE_OP_SMALL_BITSET: /* Small bitset. */
    case RE_OP_SMALL_BITSET_REV: /* Small bitset. */
    case RE_OP_START_OF_LINE: /* At the start of a line. */
    case RE_OP_START_OF_STRING: /* At the start of the string. */
    case RE_OP_STRING: /* A string literal. */
    case RE_OP_STRING_IGN: /* A string literal, ignoring case. */
    case RE_OP_STRING_IGN_REV: /* A string literal, ignoring case. */
    case RE_OP_STRING_REV: /* A string literal. */
        next->match_next = test->next_1.node;
        next->match_step = test->step;
        break;
    case RE_OP_GREEDY_REPEAT_ONE:
    case RE_OP_LAZY_REPEAT_ONE:
        if (test->values[1] > 0)
            next->test = test;
        break;
    }
}

/* Sets the test nodes. */
Py_LOCAL(void) set_test_nodes(PatternObject* pattern) {
    RE_Node** node_list;
    Py_ssize_t i;

    node_list = pattern->node_list;
    for (i = 0; i < pattern->node_count; i++) {
        RE_Node* node;

        node = node_list[i];
        set_test_node(&node->next_1);
        set_test_node(&node->next_2);
    }
}

/* Assigns offsets for the repeats into the values array.
 *
 * Entries for groups come before those for repeats.
 */
Py_LOCAL(void) assign_repeat_offsets(PatternObject* pattern) {
    Py_ssize_t offset;
    Py_ssize_t r;

    offset = pattern->group_count;

    for (r = 0; r < pattern->repeat_count; r++)
        pattern->repeat_info[r].value_offset = offset++;
}

/* Checks whether the matcher should do the initial check.
 *
 * It sets pattern->do_check accordingly and returns TRUE when it has done so.
 */
Py_LOCAL(BOOL) should_do_check(PatternObject* pattern, RE_Node* node, BOOL
  early) {
    for (;;) {
        switch (node->op) {
        case RE_OP_ANY:
        case RE_OP_ANY_ALL:
        case RE_OP_ANY_ALL_REV:
        case RE_OP_ANY_REV:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_ATOMIC:
            early = FALSE;
            if (should_do_check(pattern, node->next_2.node, early))
                return TRUE;
            node = node->next_1.node;
            break;
        case RE_OP_BEGIN_GROUP:
            node = node->next_1.node;
            break;
        case RE_OP_BIG_BITSET:
        case RE_OP_BIG_BITSET_REV:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_BOUNDARY:
        case RE_OP_DEFAULT_BOUNDARY:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_BRANCH:
            early = FALSE;
            if (should_do_check(pattern, node->next_1.node, early))
                return TRUE;
            node = node->next_2.node;
            break;
        case RE_OP_CHARACTER:
        case RE_OP_CHARACTER_IGN:
        case RE_OP_CHARACTER_IGN_REV:
        case RE_OP_CHARACTER_REV:
            if (node->match) {
                pattern->do_check = !early;
                return TRUE;
            }
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_END_GREEDY_REPEAT:
        case RE_OP_END_LAZY_REPEAT:
            early = FALSE;
            node = node->next_2.node;
            break;
        case RE_OP_END_GROUP:
            node = node->next_1.node;
            break;
        case RE_OP_END_OF_LINE:
        case RE_OP_END_OF_STRING:
        case RE_OP_END_OF_STRING_LINE:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_GREEDY_REPEAT:
        case RE_OP_LAZY_REPEAT:
            early = FALSE;
            if (node->values[1] >= 1 && should_do_check(pattern,
              node->next_2.node, early))
                return TRUE;
            node = node->next_1.node;
            break;
        case RE_OP_GREEDY_REPEAT_ONE:
        case RE_OP_LAZY_REPEAT_ONE:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_GROUP_EXISTS:
            early = FALSE;
            if (should_do_check(pattern, node->next_1.node, early))
                return TRUE;
            node = node->next_2.node;
            break;
        case RE_OP_LOOKAROUND:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_PROPERTY:
        case RE_OP_PROPERTY_REV:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_REF_GROUP:
        case RE_OP_REF_GROUP_IGN:
        case RE_OP_REF_GROUP_IGN_REV:
        case RE_OP_REF_GROUP_REV:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_SEARCH_ANCHOR:
            early = FALSE;
            node = node->next_1.node;
            early = FALSE;
            break;
        case RE_OP_SET:
        case RE_OP_SET_REV:
        case RE_OP_SMALL_BITSET:
        case RE_OP_SMALL_BITSET_REV:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_START_OF_LINE:
        case RE_OP_START_OF_STRING:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_STRING:
        case RE_OP_STRING_IGN:
        case RE_OP_STRING_IGN_REV:
        case RE_OP_STRING_REV:
            pattern->do_check = !early;
            return TRUE;
        case RE_OP_SUCCESS:
            return FALSE;
        default:
            return FALSE;
        }
    }
}

/* Optimises the pattern. */
Py_LOCAL(BOOL) optimise_pattern(PatternObject* pattern) {
    /* Building the nodes is made simpler by allowing branches to have a single
     * exit. These need to be removed.
     */
    skip_one_way_branches(pattern);

    /* Discard any unused nodes. */
    discard_unused_nodes(pattern);

    /* Set the test nodes. */
    set_test_nodes(pattern);

    /* Assign group offsets into the values array. */
    if (!assign_group_offsets(pattern))
        return FALSE;

    /* Assign repeat offsets into the values array. */
    assign_repeat_offsets(pattern);

    /* Mark all the group that are named. */
    mark_named_groups(pattern);

    /* Check whether an initial check should be done when matching. */
    pattern->do_check = FALSE;
    should_do_check(pattern, pattern->start_node, TRUE);

    return TRUE;
}

/* Creates a new pattern node. */
Py_LOCAL(RE_Node*) create_node(PatternObject* pattern, RE_CODE op, BOOL match,
  Py_ssize_t step, Py_ssize_t value_count) {
    RE_Node* node;

    node = (RE_Node*)re_alloc(sizeof(*node));
    if (!node)
        return NULL;

    node->value_capacity = value_count;
    node->value_count = value_count;
    node->values = (RE_CODE*)re_alloc(node->value_capacity * sizeof(RE_CODE));
    if (!node->values)
        goto error;

    node->used = FALSE;
    node->op = op;
    node->match = match & RE_MATCH_OP;
    node->step = step;
    node->next_1.node = NULL;
    node->next_1.test = NULL;
    node->next_2.node = NULL;
    node->next_2.test = NULL;
    node->bad_character_offset = NULL;
    node->good_suffix_offset = NULL;

    /* Ensure that there's enough storage to record the new node. */
    if (pattern->node_count >= pattern->node_capacity) {
        RE_Node** new_node_list;

        pattern->node_capacity += 16;
        new_node_list = (RE_Node**)re_realloc(pattern->node_list,
          pattern->node_capacity * sizeof(RE_Node*));
        if (!new_node_list)
            goto error;
        pattern->node_list = new_node_list;
    }

    /* Record the new node. */
    pattern->node_list[pattern->node_count++] = node;

    return node;

error:
    re_dealloc(node->values);
    re_dealloc(node);
    return NULL;
}

/* Adds a node as a next node for another node. */
Py_LOCAL(void) add_node(RE_Node* node_1, RE_Node* node_2) {
    if (!node_1->next_1.node)
        node_1->next_1.node = node_2;
    else
        node_1->next_2.node = node_2;
}

/* Ensures that the entry for a group's details actually exists. */
Py_LOCAL(BOOL) ensure_group(PatternObject* pattern, Py_ssize_t group) {
    Py_ssize_t old_capacity;
    Py_ssize_t new_capacity;
    RE_GroupInfo* new_group_info;

    if (group <= pattern->group_count)
        /* We already have an entry for the group. */
        return TRUE;

    /* Increase the storage capacity to include the new entry if it's
     * insufficient.
     */
    old_capacity = pattern->group_info_capacity;
    new_capacity = pattern->group_info_capacity;
    while (group > new_capacity)
        new_capacity += 16;

    if (new_capacity > old_capacity) {
        new_group_info = (RE_GroupInfo*)re_realloc(pattern->group_info,
          new_capacity * sizeof(RE_GroupInfo));
        if (!new_group_info)
            return FALSE;
        memset(new_group_info + old_capacity, 0, (new_capacity - old_capacity)
          * sizeof(RE_GroupInfo));

        pattern->group_info = new_group_info;
        pattern->group_info_capacity = new_capacity;
    }

    pattern->group_count = group;

    return TRUE;
}

/* Records that there's a reference to a group. */
Py_LOCAL(BOOL) record_ref_group(PatternObject* pattern, Py_ssize_t group) {
    if (!ensure_group(pattern, group))
        return FALSE;

    pattern->group_info[group - 1].referenced = TRUE;

    return TRUE;
}

/* Records that there's a new group. */
Py_LOCAL(BOOL) record_group(PatternObject* pattern, Py_ssize_t group) {
    if (!ensure_group(pattern, group))
        return FALSE;

    if (group >= 1) {
        pattern->group_info[group - 1].id = group;
        pattern->group_info[group - 1].end_index = pattern->group_count;
    }

    return TRUE;
}

/* Records that a group has closed. */
Py_LOCAL(void) record_group_end(PatternObject* pattern, Py_ssize_t group) {
    if (group >= 1)
        pattern->group_info[group - 1].end_index = ++pattern->group_end_index;
}

/* Checks whether a node matches one and only one character. */
Py_LOCAL(BOOL) sequence_matches_one(RE_Node* node) {
    while (node->op == RE_OP_BRANCH && !node->next_2.node)
        node = node->next_1.node;

    if (node->next_1.node)
        return FALSE;

    switch (node->op) {
    case RE_OP_ANY:
    case RE_OP_ANY_ALL:
    case RE_OP_ANY_ALL_REV:
    case RE_OP_ANY_REV:
    case RE_OP_BIG_BITSET:
    case RE_OP_BIG_BITSET_REV:
    case RE_OP_CHARACTER:
    case RE_OP_CHARACTER_IGN:
    case RE_OP_CHARACTER_IGN_REV:
    case RE_OP_CHARACTER_REV:
    case RE_OP_PROPERTY:
    case RE_OP_PROPERTY_REV:
    case RE_OP_SET:
    case RE_OP_SET_REV:
    case RE_OP_SMALL_BITSET:
    case RE_OP_SMALL_BITSET_REV:
        return TRUE;
    default:
        return FALSE;
    }
}

/* Records a repeat. */
Py_LOCAL(BOOL) record_repeat(PatternObject* pattern, int id) {
    Py_ssize_t old_capacity;
    Py_ssize_t new_capacity;
    RE_RepeatInfo* new_repeat_info;

    /* Increase the storage capacity to include the new entry if it's
     * insufficient.
     */
    old_capacity = pattern->repeat_info_capacity;
    new_capacity = pattern->repeat_info_capacity;
    while (id >= new_capacity)
        new_capacity += 16;

    if (new_capacity > old_capacity) {
        new_repeat_info = (RE_RepeatInfo*)re_realloc(pattern->repeat_info,
          new_capacity * sizeof(RE_RepeatInfo));
        if (!new_repeat_info)
            return FALSE;
        memset(new_repeat_info + old_capacity, 0, (new_capacity - old_capacity)
          * sizeof(RE_RepeatInfo));

        pattern->repeat_info = new_repeat_info;
        pattern->repeat_info_capacity = new_capacity;
    }

    if (id >= pattern->repeat_count)
        pattern->repeat_count = id + 1;

    return TRUE;
}

Py_LOCAL(BOOL) build_sequence(RE_CompileArgs* args);

/* Builds ANY. */
Py_LOCAL(BOOL) build_ANY(RE_CompileArgs* args) {
    RE_CODE op;
    Py_ssize_t step;
    RE_Node* node;

    /* codes: opcode. */
    if (args->code + 1 > args->end_code)
        return FALSE;

    op = args->code[0];

    step = op == RE_OP_ANY || op == RE_OP_ANY_ALL ? 1 : -1;

    /* Create the node. */
    node = create_node(args->pattern, op, TRUE, step, 0);
    if (!node)
        return FALSE;

    ++args->code;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    ++args->min_width;

    return TRUE;
}

/* Builds ATOMIC. */
Py_LOCAL(BOOL) build_ATOMIC(RE_CompileArgs* args) {
    RE_Node* node;
    RE_CompileArgs subargs;
    RE_Node* success_node;

    /* codes: opcode, sequence, end. */
    node = create_node(args->pattern, RE_OP_ATOMIC, TRUE, 0, 0);
    if (!node)
        return FALSE;

    ++args->code;

    subargs = *args;

    /* Compile the sequence and check that we've reached the end of the
     * subpattern.
     */
    if (!build_sequence(&subargs))
        return FALSE;

    if (subargs.code[0] != RE_OP_END)
        return FALSE;

    /* Create the success node to terminate the subpattern. */
    success_node = create_node(subargs.pattern, RE_OP_SUCCESS, FALSE, 0, 0);
    if (!success_node)
        return FALSE;

    /* Append the SUCCESS node. */
    add_node(subargs.end, success_node);

    /* Insert the subpattern. */
    node->next_2.node = subargs.start;

    args->code = subargs.code;
    args->min_width = subargs.min_width;

    ++args->code;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    return TRUE;
}

/* Builds BIG_BITSET. */
Py_LOCAL(BOOL) build_BIG_BITSET(RE_CompileArgs* args) {
    RE_CODE flags;
    RE_CODE op;
    Py_ssize_t step;
    RE_CODE max_char;
    size_t index_count;
    RE_CODE* cur_ptr;
    size_t subset_count;
    size_t i;
    size_t size;
    RE_Node* node;

    /* codes: opcode, flags, max_char, indexes, subsets. */
    flags = args->code[1];
    if (flags & ~(RE_MATCH_OP | RE_ZEROWIDTH_OP))
        return FALSE;

    op = args->code[0];

    step = op == RE_OP_BIG_BITSET || op == RE_OP_BIG_BITSET ? 1 : -1;

    if (flags & RE_ZEROWIDTH_OP)
        step = 0;

    max_char = args->code[2];

    index_count = (max_char >> 8) + 1;
    cur_ptr = args->code + 3;
    subset_count = 0;
    for (i = 0; i < index_count; i++) {
        RE_CODE idx;

        idx = cur_ptr[i >> RE_INDEXES_PER_CODE_SHIFT];
        idx >>= RE_BITS_PER_INDEX * (i & RE_INDEXES_PER_CODE_MASK);
        idx &= (1 << RE_BITS_PER_INDEX) - 1;
        if (idx > subset_count)
            return FALSE;

        if (idx == subset_count)
            ++subset_count;
    }

    cur_ptr += (index_count + RE_INDEXES_PER_CODE - 1) >>
      RE_INDEXES_PER_CODE_SHIFT;
    cur_ptr += (256 / RE_BITS_PER_CODE) * subset_count;
    if (cur_ptr > args->end_code)
        return FALSE;

    size = cur_ptr - (args->code + 3);

    /* Create the node. */
    node = create_node(args->pattern, op, flags, step, 1 + size);
    if (!node)
        return FALSE;

    node->values[0] = max_char;
    for (i = 0; i < size; i++)
        node->values[1 + i] = args->code[3 + i];

    args->code += 3 + size;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    if (step != 0)
        ++args->min_width;

    return TRUE;
}

/* Builds BOUNDARY. */
Py_LOCAL(BOOL) build_BOUNDARY(RE_CompileArgs* args) {
    RE_CODE flags;
    RE_Node* node;

    /* codes: opcode, flags. */
    if (args->code + 2 > args->end_code)
        return FALSE;

    flags = args->code[1];
    if (flags & ~RE_MATCH_OP)
        return FALSE;

    /* Create the node. */
    node = create_node(args->pattern, args->code[0], flags, 0, 0);
    if (!node)
        return FALSE;

    args->code += 2;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    return TRUE;
}

/* Builds BRANCH. */
Py_LOCAL(BOOL) build_BRANCH(RE_CompileArgs* args) {
    RE_Node* branch_node;
    RE_Node* join_node;
    size_t smallest_min_width;
    RE_CompileArgs subargs;

    /* codes: opcode, branch, next, branch, end. */

    /* Create nodes for the start and end of the branch sequence. */
    branch_node = create_node(args->pattern, RE_OP_BRANCH, FALSE, 0, 0);
    join_node = create_node(args->pattern, RE_OP_BRANCH, FALSE, 0, 0);
    if (!branch_node || !join_node)
        return FALSE;

    /* Append the node. */
    add_node(args->end, branch_node);
    args->end = join_node;

    smallest_min_width = ~(size_t)0;

    subargs = *args;

    /* A branch in the regular expression is compiled into a series of
     * 2-way branches.
     */
    do {
        RE_Node* next_branch_node;

        /* Skip over the 'BRANCH' or 'NEXT' opcode. */
        ++subargs.code;

        /* Compile the sequence until the next 'BRANCH' or 'NEXT'
         * opcode.
         */
        subargs.min_width = 0;
        if (!build_sequence(&subargs))
            return FALSE;

        if (subargs.min_width < smallest_min_width)
            smallest_min_width = subargs.min_width;

        /* Append the sequence. */
        add_node(branch_node, subargs.start);
        add_node(subargs.end, join_node);

        /* Create a start node for the next sequence and append it. */
        next_branch_node = create_node(subargs.pattern, RE_OP_BRANCH, FALSE, 0,
          0);
        if (!next_branch_node)
            return FALSE;

        add_node(branch_node, next_branch_node);
        branch_node = next_branch_node;
    } while (subargs.code < subargs.end_code && subargs.code[0] == RE_OP_NEXT);

    /* We should have reached the end of the branch. */
    if (subargs.code[0] != RE_OP_END)
        return FALSE;

    args->code = subargs.code;

    ++args->code;
    args->min_width += smallest_min_width;

    return TRUE;
}

/* Builds CHARACTER. */
Py_LOCAL(BOOL) build_CHARACTER(RE_CompileArgs* args) {
    RE_CODE flags;
    RE_CODE op;
    Py_ssize_t step;
    RE_Node* node;

    /* codes: opcode, flags, value. */
    if (args->code + 3 > args->end_code)
        return FALSE;

    flags = args->code[1];
    if (flags & ~(RE_MATCH_OP | RE_ZEROWIDTH_OP))
        return FALSE;

    op = args->code[0];

    step = op == RE_OP_CHARACTER || op == RE_OP_CHARACTER_IGN ? 1 : -1;

    if (flags & RE_ZEROWIDTH_OP)
        step = 0;

    /* Create the node. */
    node = create_node(args->pattern, op, args->code[1], step, 1);
    if (!node)
        return FALSE;

    node->values[0] = args->code[2];

    args->code += 3;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    if (step != 0)
        ++args->min_width;

    return TRUE;
}

/* Builds GROUP. */
Py_LOCAL(BOOL) build_GROUP(RE_CompileArgs* args) {
    RE_CODE group;
    RE_Node* start_node;
    RE_Node* end_node;
    RE_CompileArgs subargs;

    /* codes: opcode, group. */
    group = args->code[1];
    args->code += 2;

    /* Create nodes for the start and end of the capture group. */
    if (args->forward) {
        start_node = create_node(args->pattern, RE_OP_BEGIN_GROUP, FALSE, 0,
          1);
        end_node = create_node(args->pattern, RE_OP_END_GROUP, FALSE, 0, 1);
    } else {
        start_node = create_node(args->pattern, RE_OP_END_GROUP, FALSE, 0, 1);
        end_node = create_node(args->pattern, RE_OP_BEGIN_GROUP, FALSE, 0, 1);
    }
    if (!start_node || !end_node)
        return FALSE;

    start_node->values[0] = group;
    end_node->values[0] = group;

    /* Record that we have a new capture group. */
    if (!record_group(args->pattern, group))
        return FALSE;

    /* Compile the sequence and check that we've reached the end of the
     * capture group.
     */
    subargs = *args;
    if (!build_sequence(&subargs))
        return FALSE;

    if (subargs.code[0] != RE_OP_END)
        return FALSE;

    args->code = subargs.code;
    args->min_width = subargs.min_width;

    ++args->code;

    /* Record that the capture group has closed. */
    record_group_end(args->pattern, group);

    /* Append the capture group. */
    add_node(args->end, start_node);
    add_node(start_node, subargs.start);
    add_node(subargs.end, end_node);
    args->end = end_node;

    return TRUE;
}

/* Builds GROUP_EXISTS. */
Py_LOCAL(BOOL) build_GROUP_EXISTS(RE_CompileArgs* args) {
    RE_CODE group;
    RE_Node* start_node;
    RE_Node* end_node;
    RE_CompileArgs subargs;
    size_t min_width;

    /* codes: opcode, sequence, next, sequence, end. */
    group = args->code[1];
    args->code += 2;

    /* Create nodes for the start and end of the structure. */
    start_node = create_node(args->pattern, RE_OP_GROUP_EXISTS, FALSE, 0, 1);
    end_node = create_node(args->pattern, RE_OP_BRANCH, FALSE, 0, 0);
    if (!start_node || !end_node)
        return FALSE;

    start_node->values[0] = group;

    subargs = *args;
    subargs.min_width = 0;
    if (!build_sequence(&subargs))
        return FALSE;

    args->code = subargs.code;
    min_width = subargs.min_width;

    /* Append the start node. */
    add_node(args->end, start_node);
    add_node(start_node, subargs.start);
    add_node(subargs.end, end_node);

    if (args->code[0] == RE_OP_NEXT) {
        ++args->code;

        subargs.code = args->code;
        subargs.min_width = 0;
        if (!build_sequence(&subargs))
            return FALSE;

        args->code = subargs.code;

        if (subargs.min_width < min_width)
            min_width = subargs.min_width;

        add_node(start_node, subargs.start);
        add_node(subargs.end, end_node);
    } else {
        add_node(start_node, end_node);

        min_width = 0;
    }

    args->min_width += min_width;

    if (args->code[0] != RE_OP_END)
        return FALSE;

    ++args->code;

    args->end = end_node;

    return TRUE;
}

/* Builds LOOKAROUND. */
Py_LOCAL(BOOL) build_LOOKAROUND(RE_CompileArgs* args) {
    RE_CODE flags;
    RE_CODE forward;
    RE_Node* lookaround_node;
    RE_Node* success_node;
    RE_CompileArgs subargs;

    /* codes: opcode, flags, forward, sequence, end. */
    flags = args->code[1];
    forward = args->code[2];
    if (flags & ~RE_MATCH_OP)
        return FALSE;

    /* Create a node for the lookaround. */
    lookaround_node = create_node(args->pattern, RE_OP_LOOKAROUND, flags, 0,
      0);
    if (!lookaround_node)
        return FALSE;

    args->code += 3;

    /* Compile the sequence and check that we've reached the end of the
     * subpattern.
     */
    subargs = *args;
    subargs.forward = forward;
    if (!build_sequence(&subargs))
        return FALSE;

    if (subargs.code[0] != RE_OP_END)
        return FALSE;

    args->code = subargs.code;
    ++args->code;

    /* Create the 'SUCCESS' node and append it to the subpattern. */
    success_node = create_node(args->pattern, RE_OP_SUCCESS, FALSE, 0, 0);
    if (!success_node)
        return FALSE;

    /* Append the SUCCESS node. */
    add_node(subargs.end, success_node);

    /* Insert the subpattern into the node. */
    lookaround_node->next_2.node = subargs.start;

    /* Append the lookaround. */
    add_node(args->end, lookaround_node);
    args->end = lookaround_node;

    return TRUE;
}

/* Builds PROPERTY. */
Py_LOCAL(BOOL) build_PROPERTY(RE_CompileArgs* args) {
    RE_CODE flags;
    RE_CODE op;
    Py_ssize_t step;
    RE_Node* node;

    /* codes: opcode, flags, value. */
    if (args->code + 3 > args->end_code)
        return FALSE;

    flags = args->code[1];
    if (flags & ~(RE_MATCH_OP | RE_ZEROWIDTH_OP))
        return FALSE;

    op = args->code[0];

    step = op == RE_OP_PROPERTY ? 1 : -1;

    if (flags & RE_ZEROWIDTH_OP)
        step = 0;

    /* Create the node. */
    node = create_node(args->pattern, op, flags, step, 1);
    if (!node)
        return FALSE;

    node->values[0] = args->code[2];

    args->code += 3;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    if (step != 0)
        ++args->min_width;

    return TRUE;
}

/* Builds REF_GROUP. */
Py_LOCAL(BOOL) build_REF_GROUP(RE_CompileArgs* args) {
    Py_ssize_t group;
    RE_Node* node;

    /* codes: opcode, group. */
    group = args->code[1];
    node = create_node(args->pattern, args->code[0], FALSE, 0, 1);
    if (!node)
        return FALSE;

    node->values[0] = group;
    args->code += 2;

    /* Record that we have a reference to a group. */
    if (!record_ref_group(args->pattern, group))
        return FALSE;

    /* Append the reference. */
    add_node(args->end, node);
    args->end = node;

    return TRUE;
}

/* Builds REPEAT. */
Py_LOCAL(BOOL) build_REPEAT(RE_CompileArgs* args) {
    BOOL greedy;
    RE_CODE min_count;
    RE_CODE max_count;

    /* codes: opcode, min_count, max_count, sequence, end. */

    /* This includes special cases such as optional items, which we'll
     * check for and treat specially. They don't need repeat counts,
     * which helps us avoid unnecessary work when matching.
     */
    greedy = args->code[0] == RE_OP_GREEDY_REPEAT;
    min_count = args->code[1];
    max_count = args->code[2];
    if (min_count > max_count)
        return FALSE;

    args->code += 3;

    if (min_count == 0 && max_count == 1) {
        /* Optional sequence. */
        RE_Node* branch_node;
        RE_Node* join_node;
        RE_CompileArgs subargs;

        /* Create the start and end nodes. */
        branch_node = create_node(args->pattern, RE_OP_BRANCH, FALSE, 0, 0);
        join_node = create_node(args->pattern, RE_OP_BRANCH, FALSE, 0, 0);
        if (!branch_node || !join_node)
            return FALSE;

        /* Compile the sequence and check that we've reached the end of
         * it.
         */
        subargs = *args;
        if (!build_sequence(&subargs))
            return FALSE;

        if (subargs.code[0] != RE_OP_END)
            return FALSE;

        args->code = subargs.code;

        ++args->code;

        if (greedy) {
            /* It's a greedy option. */
            add_node(branch_node, subargs.start);
            add_node(branch_node, join_node);
        } else {
            /* It's a lazy option. */
            add_node(branch_node, join_node);
            add_node(branch_node, subargs.start);
        }
        add_node(subargs.end, join_node);

        /* Append the optional sequence. */
        add_node(args->end, branch_node);
        args->end = join_node;
    } else if (min_count == 1 && max_count == 1) {
        /* Singly-repeated sequence. */
        RE_CompileArgs subargs;

        subargs = *args;
        if (!build_sequence(&subargs))
            return FALSE;

        if (subargs.code[0] != RE_OP_END)
            return FALSE;

        args->code = subargs.code;
        args->min_width = subargs.min_width;

        ++args->code;

        /* Append the sequence. */
        add_node(args->end, subargs.start);
        args->end = subargs.end;
    } else {
        RE_Node* repeat_node;
        RE_CompileArgs subargs;

        /* Create the nodes for the repeat. */
        repeat_node = create_node(args->pattern, greedy ? RE_OP_GREEDY_REPEAT :
          RE_OP_LAZY_REPEAT, FALSE, args->forward ? 1 : -1, 4);
        if (!repeat_node || !record_repeat(args->pattern,
          args->pattern->repeat_count))
            return FALSE;

        repeat_node->values[0] = args->pattern->repeat_count++;
        repeat_node->values[1] = min_count;
        repeat_node->values[2] = max_count;
        repeat_node->values[3] = args->forward;

        /* Compile the 'body' and check that we've reached the end of it. */
        subargs = *args;
        subargs.min_width = 0;
        if (!build_sequence(&subargs))
            return FALSE;

        if (subargs.code[0] != RE_OP_END)
            return FALSE;

        args->code = subargs.code;
        args->min_width += min_count * subargs.min_width;

        ++args->code;

        if (sequence_matches_one(subargs.start)) {
            repeat_node->op = greedy ? RE_OP_GREEDY_REPEAT_ONE :
              RE_OP_LAZY_REPEAT_ONE;

            /* Append the new sequence. */
            add_node(args->end, repeat_node);
            repeat_node->next_2.node = subargs.start;
            args->end = repeat_node;
        } else {
            RE_Node* end_repeat_node;
            RE_Node* end_node;

            end_repeat_node = create_node(args->pattern, greedy ?
              RE_OP_END_GREEDY_REPEAT : RE_OP_END_LAZY_REPEAT, FALSE,
              args->forward ? 1 : -1, 4);
            if (!end_repeat_node)
                return FALSE;

            end_repeat_node->values[0] = repeat_node->values[0];
            end_repeat_node->values[1] = repeat_node->values[1];
            end_repeat_node->values[2] = repeat_node->values[2];
            end_repeat_node->values[3] = args->forward;

            end_node = create_node(args->pattern, RE_OP_BRANCH, FALSE, 0, 0);
            if (!end_node)
                return FALSE;

            /* Append the new sequence. */
            add_node(args->end, repeat_node);
            add_node(repeat_node, subargs.start);
            add_node(repeat_node, end_node);
            add_node(subargs.end, end_repeat_node);
            add_node(end_repeat_node, subargs.start);
            add_node(end_repeat_node, end_node);
            args->end = end_node;
        }
    }

    return TRUE;
}

/* Builds SMALL_BITSET. */
Py_LOCAL(BOOL) build_SMALL_BITSET(RE_CompileArgs* args) {
    size_t size;
    RE_CODE flags;
    RE_CODE op;
    Py_ssize_t step;
    RE_Node* node;
    size_t i;

    /* codes: opcode, flags, top_bits, bitset. */
    size = 256 / RE_BITS_PER_CODE;
    if (args->code + 3 + size > args->end_code)
        return FALSE;

    flags = args->code[1];
    if (flags & ~(RE_MATCH_OP | RE_ZEROWIDTH_OP))
        return FALSE;

    op = args->code[0];

    step = op == RE_OP_SMALL_BITSET ? 1 : -1;

    if (flags & RE_ZEROWIDTH_OP)
        step = 0;

    /* Create the node. */
    node = create_node(args->pattern, op, flags, step, size + 1);
    if (!node)
        return FALSE;

    node->values[0] = args->code[2];
    for (i = 0; i < size; i++)
        node->values[1 + i] = args->code[3 + i];

    args->code += 3 + size;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    if (step != 0)
        ++args->min_width;

    return TRUE;
}

/* Builds STRING. */
Py_LOCAL(BOOL) build_STRING(RE_CompileArgs* args) {
    Py_ssize_t length;
    RE_CODE op;
    Py_ssize_t step;
    RE_Node* node;
    Py_ssize_t i;

    /* codes: opcode, length, characters. */
    length = args->code[1];
    if (args->code + 2 + length > args->end_code)
        return FALSE;

    op = args->code[0];

    step = op == RE_OP_STRING || op == RE_OP_STRING_IGN ? 1 : -1;

    /* Create the node. */
    node = create_node(args->pattern, op, TRUE, step * length, length);
    if (!node)
        return FALSE;

    for (i = 0; i < length; i++)
        node->values[i] = args->code[2 + i];

    args->code += 2 + length;

    switch (op) {
    case RE_OP_STRING:
        build_fast_tables(node);
        break;
    case RE_OP_STRING_IGN:
        build_fast_tables_ign(args->pattern->encoding, node);
        break;
    case RE_OP_STRING_IGN_REV:
        build_fast_tables_ign_rev(args->pattern->encoding, node);
        break;
    case RE_OP_STRING_REV:
        build_fast_tables_rev(node);
        break;
    }

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    args->min_width += length;

    return TRUE;
}

/* Builds SET. */
Py_LOCAL(BOOL) build_SET(RE_CompileArgs* args) {
    RE_CODE flags;
    RE_CODE op;
    Py_ssize_t step;
    RE_Node* node;
    size_t saved_min_width;

    /* codes: opcode, flags, members. */
    flags = args->code[1];
    if (flags & ~(RE_MATCH_OP | RE_ZEROWIDTH_OP))
        return FALSE;

    op = args->code[0];

    step = op == RE_OP_SET ? 1 : -1;

    if (flags & RE_ZEROWIDTH_OP)
        step = 0;

    node = create_node(args->pattern, op, flags, step, 0);
    if (!node)
        return FALSE;

    args->code += 2;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    saved_min_width = args->min_width;

    /* Compile the character set. */
    do {
        switch (args->code[0]) {
        case RE_OP_ANY:
            if (!build_ANY(args))
                return FALSE;
            break;
        case RE_OP_BIG_BITSET:
            if (!build_BIG_BITSET(args))
                return FALSE;
            break;
        case RE_OP_CHARACTER:
            if (!build_CHARACTER(args))
                return FALSE;
            break;
        case RE_OP_PROPERTY:
            if (!build_PROPERTY(args))
                return FALSE;
            break;
        case RE_OP_SMALL_BITSET:
            if (!build_SMALL_BITSET(args))
                return FALSE;
            break;
        default:
            /* Illegal opcode for a character set. */
            return FALSE;
        }
    } while (args->code < args->end_code && args->code[0] != RE_OP_END);

    /* Check that we've reached the end correctly. (The last opcode
     * should be 'END'.)
     */
    if (args->code >= args->end_code || args->code[0] != RE_OP_END)
        return FALSE;

    ++args->code;

    /* At this point the set's members are in the main sequence. They need to
     * be moved out-of-line.
     */
    node->next_2.node = node->next_1.node;
    node->next_1.node = NULL;
    args->end = node;

    args->min_width = saved_min_width;

    if (step != 0)
        ++args->min_width;

    return TRUE;
}

/* Builds zero-width. */
Py_LOCAL(BOOL) build_zerowidth(RE_CompileArgs* args) {
    RE_Node* node;

    /* Create the node. */
    node = create_node(args->pattern, args->code[0], TRUE, 0, 0);
    if (!node)
        return FALSE;

    ++args->code;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    return TRUE;
}

/* Builds a sequence of nodes from regular expression code. */
Py_LOCAL(BOOL) build_sequence(RE_CompileArgs* args) {
    /* Guarantee that there's something to attach to. */
    args->start = create_node(args->pattern, RE_OP_BRANCH, FALSE, 0, 0);
    args->end = args->start;

    /* The sequence should end with an opcode we don't understand. If it
     * doesn't then the code is illegal.
     */
    while (args->code < args->end_code) {
        /* The following code groups opcodes by format, not function. */
        switch (args->code[0]) {
        case RE_OP_ANY:
        case RE_OP_ANY_ALL:
        case RE_OP_ANY_ALL_REV:
        case RE_OP_ANY_REV:
            /* A simple opcode with no trailing codewords and width of 1. */
            if (!build_ANY(args))
                return FALSE;
            break;
        case RE_OP_ATOMIC:
            /* An atomic sequence. */
            if (!build_ATOMIC(args))
                return FALSE;
            break;
        case RE_OP_BIG_BITSET:
        case RE_OP_BIG_BITSET_REV:
            /* A big bitset. */
            if (!build_BIG_BITSET(args))
                return FALSE;
            break;
        case RE_OP_BOUNDARY:
        case RE_OP_DEFAULT_BOUNDARY:
            /* A word boundary. */
            if (!build_BOUNDARY(args))
                return FALSE;
            break;
        case RE_OP_BRANCH:
            /* A 2-way branch. */
            if (!build_BRANCH(args))
                return FALSE;
            break;
        case RE_OP_CHARACTER:
        case RE_OP_CHARACTER_IGN:
        case RE_OP_CHARACTER_IGN_REV:
        case RE_OP_CHARACTER_REV:
            /* A character literal. */
            if (!build_CHARACTER(args))
                return FALSE;
            break;
        case RE_OP_END_OF_LINE:
        case RE_OP_END_OF_STRING:
        case RE_OP_END_OF_STRING_LINE:
        case RE_OP_SEARCH_ANCHOR:
        case RE_OP_START_OF_LINE:
        case RE_OP_START_OF_STRING:
            /* A simple opcode with no trailing codewords and width of 0. */
            if (!build_zerowidth(args))
                return FALSE;
            break;
        case RE_OP_GREEDY_REPEAT:
        case RE_OP_LAZY_REPEAT:
            /* A repeated sequence. */
            if (!build_REPEAT(args))
                return FALSE;
            break;
        case RE_OP_GROUP:
            /* A capture group. */
            if (!build_GROUP(args))
                return FALSE;
            break;
        case RE_OP_GROUP_EXISTS:
            /* A conditional sequence. */
            if (!build_GROUP_EXISTS(args))
                return FALSE;
            break;
        case RE_OP_LOOKAROUND:
            /* A lookaround. */
            if (!build_LOOKAROUND(args))
                return FALSE;
            break;
        case RE_OP_PROPERTY:
        case RE_OP_PROPERTY_REV:
            /* A property. */
            if (!build_PROPERTY(args))
                return FALSE;
            break;
        case RE_OP_REF_GROUP:
        case RE_OP_REF_GROUP_IGN:
        case RE_OP_REF_GROUP_REV:
        case RE_OP_REF_GROUP_IGN_REV:
            /* A reference to a group. */
            if (!build_REF_GROUP(args))
                return FALSE;
            break;
        case RE_OP_SET:
        case RE_OP_SET_REV:
            /* A character set. */
            if (!build_SET(args))
                return FALSE;
            break;
        case RE_OP_SMALL_BITSET:
        case RE_OP_SMALL_BITSET_REV:
            /* A small bitset. */
            if (!build_SMALL_BITSET(args))
                return FALSE;
            break;
        case RE_OP_STRING:
        case RE_OP_STRING_IGN:
        case RE_OP_STRING_IGN_REV:
        case RE_OP_STRING_REV:
            /* A string literal. */
            if (!build_STRING(args))
                return FALSE;
            break;
        default:
            /* We've found an opcode which we don't recognise. We'll leave it
             * for the caller, but we'll first check whether we've reached the
             * end of the codes (there _should_ be more codes remaining for the
             * caller).
             */
            if (args->code >= args->end_code)
                return FALSE;

            return TRUE;
        }
    }

    /* If we get here then we're past the end of the regular expression code,
     * but the code should end with 'SUCCESS' (which isn't recognised by this
     * function), so we have an error.
     */
    return FALSE;
}

/* Compiles the regular expression code to 'nodes'.
 *
 * Various details about the regular expression are discovered during
 * compilation and stored in the PatternObject.
 */
Py_LOCAL(BOOL) compile_to_nodes(RE_CODE* code, RE_CODE* end_code,
  PatternObject* pattern) {
    RE_CompileArgs args;
    RE_Node* success_node;

    /* Compile a regex sequence and then check that we've reached the end
     * correctly. (The last opcode should be 'SUCCESS'.)
     *
     * If successful, 'start' and 'end' will point to the start and end nodes
     * of the compiled sequence.
     */
    args.code = code;
    args.end_code = end_code;
    args.pattern = pattern;
    args.forward = (pattern->flags & RE_FLAG_REVERSE) == 0;
    args.min_width = 0;
    if (!build_sequence(&args))
        return FALSE;

    if (args.code + 1 != end_code || args.code[0] != RE_OP_SUCCESS)
        return FALSE;

    pattern->min_width = args.min_width;

    /* Create the 'SUCCESS' node and append it to the sequence. */
    success_node = create_node(pattern, RE_OP_SUCCESS, FALSE, 0, 0);
    if (!success_node)
        return FALSE;

    add_node(args.end, success_node);
    pattern->start_node = args.start;
    pattern->success_node = success_node;

    pattern->data_count = pattern->group_count + pattern->repeat_count;

    /* Optimise the pattern. */
    if (!optimise_pattern(pattern))
        return FALSE;

    return TRUE;
}

/* Compiles regular expression code to a PatternObject.
 *
 * The regular expression code is provided as a list and is then compiled to
 * 'nodes'. Various details about the regular expression are discovered during
 * compilation and stored in the PatternObject.
 */
static PyObject* re_compile(PyObject* self_, PyObject* args) {
    PyObject* pattern;
    Py_ssize_t flags = 0;
    PyObject* code_list;
    PyObject* groupindex;
    PyObject* indexgroup;
    Py_ssize_t code_len;
    RE_CODE* code;
    Py_ssize_t i;
    PatternObject* self;
    BOOL ascii;
    BOOL locale;
    BOOL unicode;
    BOOL ok;

    if (!PyArg_ParseTuple(args, "OiOOO", &pattern, &flags, &code_list,
      &groupindex, &indexgroup))
        return NULL;

    /* Read the regular expression code. */
    code_len = PyList_GET_SIZE(code_list);
    code = (RE_CODE*)re_alloc(code_len * sizeof(RE_CODE));
    if (!code)
        return NULL;

    for (i = 0; i < code_len; i++) {
        PyObject* o = PyList_GET_ITEM(code_list, i);
        unsigned long value;

        if (PyInt_Check(o)) {
            long svalue;

            svalue = PyInt_AsLong(o);
            if (svalue < 0)
                goto error;

            value = svalue;
        } else if (PyLong_Check(o)) {
            value = PyLong_AsUnsignedLong(o);
            if (PyErr_Occurred())
                goto error;
        } else
            goto error;

        code[i] = (RE_CODE)value;
        if (code[i] != value)
            goto error;
    }

    /* Create the PatternObject. */
    self = PyObject_NEW(PatternObject, &Pattern_Type);
    if (!self) {
        set_error(RE_ERROR_MEMORY, NULL);
        re_dealloc(code);
        return NULL;
    }

    /* Initialise the PatternObject. */
    self->pattern = pattern;
    self->flags = flags;
    self->weakreflist = NULL;
    self->start_node = NULL;
    self->success_node = NULL;
    self->repeat_count = 0;
    self->group_count = 0;
    self->group_end_index = 0;
    self->groupindex = groupindex;
    self->indexgroup = indexgroup;
    self->node_capacity = 0;
    self->node_count = 0;
    self->node_list = NULL;
    self->group_info_capacity = 0;
    self->group_info = NULL;
    self->repeat_info_capacity = 0;
    self->repeat_info = NULL;
    self->marks_storage = NULL;
    self->saved_groups_storage = NULL;
    self->data_storage = NULL;
    Py_INCREF(self->pattern);
    Py_INCREF(self->groupindex);
    Py_INCREF(self->indexgroup);

    /* Initialise the character encoding. */
    unicode = flags & RE_FLAG_UNICODE;
    locale = flags & RE_FLAG_LOCALE;
    ascii = flags & RE_FLAG_ASCII;
    if (!unicode && !locale && !ascii) {
        if (PyString_Check(self->pattern))
            ascii = RE_FLAG_ASCII;
        else
            unicode = RE_FLAG_UNICODE;
    }
    if (unicode)
        self->encoding = &unicode_encoding;
    else if (locale)
        self->encoding = &locale_encoding;
    else if (ascii)
        self->encoding = &ascii_encoding;

    /* Compile the regular expression code to nodes. */
    ok = compile_to_nodes(code, code + code_len, self);

    /* We no longer need the regular expression code. */
    re_dealloc(code);

    if (!ok) {
        if (!PyErr_Occurred())
            set_error(RE_ERROR_ILLEGAL, NULL);

        Py_DECREF(self);
        return NULL;
    }

    return (PyObject*)self;

error:
    re_dealloc(code);
    set_error(RE_ERROR_ILLEGAL, NULL);
    return NULL;
}

/* Gets the size of the codewords. */
static PyObject* get_code_size(PyObject* self, PyObject* unused) {
    return Py_BuildValue("l", sizeof(RE_CODE));
}

/* Sets the exception to return on error. */
static PyObject* set_exception(PyObject* self_, PyObject* args) {
    if (!PyArg_ParseTuple(args, "O", &error_exception))
        return NULL;

    Py_INCREF(error_exception);

    Py_INCREF(Py_None);
    return Py_None;
}

/* The table of the module's functions. */
static PyMethodDef _functions[] = {
    {"compile", (PyCFunction)re_compile, METH_VARARGS},
    {"get_code_size", (PyCFunction)get_code_size, METH_NOARGS},
    {"set_exception", (PyCFunction)set_exception, METH_VARARGS},
    {NULL, NULL}
};

/* Initialises the module. */
PyMODINIT_FUNC init_regex(void) {
    PyObject* m;
    PyObject* d;
    PyObject* x;
#if defined(VERBOSE)
    /* Unbuffered in case it crashes! */
    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    /* Patch object types. */
    Pattern_Type.ob_type = Match_Type.ob_type = Scanner_Type.ob_type =
      Splitter_Type.ob_type = &PyType_Type;

    error_exception = PyExc_RuntimeError;

    m = Py_InitModule("_" RE_MODULE, _functions);
    if (!m)
        return;
    d = PyModule_GetDict(m);

    x = PyInt_FromLong(RE_MAGIC);
    if (x) {
        PyDict_SetItemString(d, "MAGIC", x);
        Py_DECREF(x);
    }

    x = PyInt_FromLong(sizeof(RE_CODE));
    if (x) {
        PyDict_SetItemString(d, "CODE_SIZE", x);
        Py_DECREF(x);
    }

    x = PyString_FromString(copyright);
    if (x) {
        PyDict_SetItemString(d, "copyright", x);
        Py_DECREF(x);
    }
}

/* vim:ts=4:sw=4:et */
