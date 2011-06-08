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

#include "Python.h"
#include "structmember.h" /* offsetof */
#include <ctype.h>
#include "_regex.h"
#include "pyport.h"
#include "pythread.h"

typedef RE_UINT32 RE_CODE;

/* Unlimited repeat count. */
#define RE_UNLIMITED (~(RE_CODE)0)

typedef unsigned char BYTE;

typedef unsigned short RE_STATUS_T;

enum {RE_CONC_NO, RE_CONC_YES, RE_CONC_DEFAULT};

/* Name of this module, minus the leading underscore. */
#define RE_MODULE "regex"
#define RE_MODULE_UPPER "REGEX"

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

/* The nominal number of capture counts per allocated block. */
#define RE_CAPTURES_BLOCK_SIZE 1024

#define RE_BITS_PER_INDEX 16
#define RE_BITS_PER_CODE 32
#define RE_BITS_PER_CODE_SHIFT 5
#define RE_BITS_PER_CODE_MASK 0x1F
#define RE_INDEXES_PER_CODE 2
#define RE_INDEXES_PER_CODE_SHIFT 1
#define RE_INDEXES_PER_CODE_MASK 0x1

#define RE_MATCH_OP 0x1
#define RE_ZEROWIDTH_OP 0x2

/* Guards against further matching can occur at the start of the body and the
 * tail of a repeat containing a repeat.
 */
#define RE_STATUS_BODY 0x1
#define RE_STATUS_TAIL 0x2

/* Whether a guard is added depends on whether there's a repeat in the body of
 * a repeat or a group reference in the body or tail of the repeat.
 */
#define RE_STATUS_NEITHER 0x0
#define RE_STATUS_REPEAT 0x4
#define RE_STATUS_LIMITED 0x8
#define RE_STATUS_REF 0x10
#define RE_STATUS_VISITED_AG 0x20

#define RE_STATUS_VISITED_REP 0x40
#define RE_STATUS_VISITED_NC 0x80
#define RE_STATUS_FAST_INIT 0x100
#define RE_STATUS_USED 0x200
#define RE_STATUS_STRING 0x400
#define RE_STATUS_INNER 0x800

static char copyright[] =
    " RE 2.3.0 Copyright (c) 1997-2002 by Secret Labs AB ";

/* The exception to return on error. */
static PyObject* error_exception;

static PyObject* property_dict;

/* The shortest string prefix for which we'll use a fast string search. */
#define RE_MIN_FAST_LENGTH 3

/* Handlers for ASCII, locale and Unicode. */
typedef struct RE_EncodingTable {
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);
    RE_CODE (*lower)(RE_CODE ch);
    RE_CODE (*upper)(RE_CODE ch);
    RE_CODE (*title)(RE_CODE ch);
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    BOOL (*at_boundary)(struct RE_State* state, Py_ssize_t text_pos);
    BOOL (*at_default_boundary)(struct RE_State* state, Py_ssize_t text_pos);
    BOOL (*at_grapheme_boundary)(struct RE_State* state, Py_ssize_t text_pos);
    BOOL (*is_line_sep)(RE_CODE ch);
    BOOL (*at_line_start)(struct RE_State* state, Py_ssize_t text_pos);
    BOOL (*at_line_end)(struct RE_State* state, Py_ssize_t text_pos);
    BOOL (*possible_turkic)(RE_CODE ch);
    RE_CODE (*folded_case)(RE_CODE ch);
    int (*get_all_cases)(RE_CODE ch, RE_CODE* cases);
} RE_EncodingTable;

/* Position with the regex and text. */
typedef struct RE_Position {
    struct RE_Node* node;
    Py_ssize_t text_pos;
} RE_Position;

/* Storage for backtrack data. */
typedef struct RE_BacktrackData {
    BYTE op;
    union {
        struct {
            RE_Position position;
        } branch;
        struct {
            RE_Position position;
            size_t index;
            Py_ssize_t text_pos;
            size_t count;
            size_t max_count;
        } repeat;
        struct {
            size_t index;
        } group;
        struct {
            size_t* capture_counts;
        } captures;
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

/* Storage for backtrack capture counts is allocated in blocks for speed. */
typedef struct RE_CaptureCountsBlock {
    struct RE_CaptureCountsBlock* previous;
    struct RE_CaptureCountsBlock* next;
    size_t capacity;
    size_t count;
    size_t* items;
} RE_CaptureCountsBlock;

/* Storage for info around a recursive by 'basic'match'. */
typedef struct RE_Info {
    RE_BacktrackBlock* current_backtrack_block;
    size_t backtrack_count;
    RE_CaptureCountsBlock* current_capture_counts_block;
    size_t captures_count;
    size_t saved_groups_count;
    BOOL must_advance;
} RE_Info;

/* Storage for the next node. */
typedef struct RE_NextNode {
    struct RE_Node* node;
    struct RE_Node* test;
    struct RE_Node* check;
    struct RE_Node* match_next;
    Py_ssize_t match_step;
} RE_NextNode;

/* A pattern node. */
typedef struct RE_Node {
    BYTE op;
    BOOL match;
    RE_STATUS_T status;
    Py_ssize_t step;
    Py_ssize_t value_capacity;
    Py_ssize_t value_count;
    RE_CODE* values;
    RE_NextNode next_1;
    union {
        struct {
            RE_NextNode next_2;
        } nonstring;
        struct {
            /* Used only if (node->status & RE_STATUS_STRING) is true. */
            Py_ssize_t* bad_character_offset;
            Py_ssize_t* good_suffix_offset;
        } string;
    };
    struct RE_Node* next_check;
} RE_Node;

/* Data about a group's span. */
typedef struct RE_GroupSpan {
    Py_ssize_t start;
    Py_ssize_t end;
} RE_GroupSpan;

/* Data about a group in a context. */
typedef struct RE_GroupData {
    RE_GroupSpan span;
    size_t capture_count;
    size_t capture_capacity;
    RE_GroupSpan* captures;
} RE_GroupData;

/* Span of a guard (inclusive range). */
typedef struct RE_GuardSpan {
    Py_ssize_t low;
    Py_ssize_t high;
} RE_GuardSpan;

/* Spans guarded against further matching. */
typedef struct RE_GuardList {
    size_t capacity;
    size_t count;
    RE_GuardSpan* spans;
    Py_ssize_t last_text_pos;
    size_t last_low;
} RE_GuardList;

/* Data about a repeat in a context. */
typedef struct RE_RepeatData {
    size_t count;
    Py_ssize_t start;
    size_t max_count;
    RE_GuardList body_guard_list;
    RE_GuardList tail_guard_list;
} RE_RepeatData;

/* Info about a capture group. */
typedef struct RE_GroupInfo {
    Py_ssize_t end_index;
    BOOL referenced;
    BOOL has_name;
} RE_GroupInfo;

/* Info about a repeat. */
typedef struct RE_RepeatInfo {
    RE_STATUS_T status;
} RE_RepeatInfo;

/* The state object used during matching. */
typedef struct RE_State {
    struct PatternObject* pattern; /* Parent PatternObject. */
    /* Info about the string being matched. */
    PyObject* string;
    Py_ssize_t charsize;
    void* text;
    Py_ssize_t text_length;
    Py_ssize_t slice_start;
    Py_ssize_t slice_end;
    RE_GroupData* groups;
    RE_RepeatData* repeats;
    Py_ssize_t lastindex;
    Py_ssize_t lastgroup;
    Py_ssize_t search_anchor;
    Py_ssize_t match_pos; /* Start position of the match. */
    Py_ssize_t text_pos; /* Current position of the match. */
    Py_ssize_t final_newline; /* Index of newline at end of string, or -1. */
    Py_ssize_t final_line_sep; /* Index of line separator at end of string, or -1. */
    /* Storage for backtrack info. */
    RE_BacktrackBlock backtrack_block;
    RE_BacktrackBlock* current_backtrack_block;
    RE_CaptureCountsBlock* current_capture_counts_block;
    RE_BacktrackData* backtrack;
    Py_ssize_t saved_groups_capacity;
    Py_ssize_t saved_groups_count;
    RE_GroupData* saved_groups;
    size_t min_width;
    RE_EncodingTable* encoding;
    RE_CODE (*char_at)(void* text, Py_ssize_t pos);
    BOOL wide;
    BOOL overlapped; /* Matches can be overlapped. */
    BOOL reverse; /* Search backwards. */
    BOOL save_captures;
    BOOL has_captures;
    BOOL zero_width; /* Enable the correct handling of zero-width matches. */
    BOOL must_advance; /* The end of the match must advance past its start. */
    BOOL is_multithreaded; /* Whether to release the GIL while matching. */
    BOOL do_check;
    PyThread_type_lock lock;
} RE_State;

/* Storage for the regex state and thread state.
 *
 * Scanner objects can sometimes be shared across threads, which means that
 * their RE_State structs are also shared. This isn't safe when the GIL is
 * released, so in such instances we have a lock (mutex) in the RE_State struct
 * to protect it during matching. We also need a thread-safe place to store the
 * thread state when releasing the GIL.
 */
typedef struct RE_SafeState {
    RE_State* re_state;
    PyThreadState* thread_state;
} RE_SafeState;

/* The PatternObject created from a regular expression. */
typedef struct PatternObject {
    PyObject_HEAD
    PyObject* pattern; /* Pattern source (or None). */
    Py_ssize_t flags; /* Flags used when compiling pattern source. */
    PyObject* weakreflist; /* List of weak references */
    /* Nodes into which the regular expression is compiled. */
    RE_Node* start_node;
    RE_Node* check_node;
    RE_Node* success_node;
    Py_ssize_t group_count; /* Number of capture groups. */
    Py_ssize_t repeat_count; /* Number of repeats. */
    Py_ssize_t group_end_index; /* Number of group closures. */
    PyObject* groupindex;
    PyObject* indexgroup;
    PyObject* ref_lists;
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
    Py_ssize_t saved_groups_capacity_storage;
    RE_GroupData* saved_groups_storage;
    RE_GroupData* groups_storage;
    RE_RepeatData* repeats_storage;
    BOOL do_check;
} PatternObject;

/* The MatchObject created when a match is found. */
typedef struct MatchObject {
    PyObject_HEAD
    PyObject* string; /* Link to the target string. */
    PatternObject* pattern; /* Link to the regex (pattern) object. */
    Py_ssize_t pos; /* Start of current slice. */
    Py_ssize_t endpos; /* End of current slice. */
    Py_ssize_t match_start; /* Start of matched slice. */
    Py_ssize_t match_end; /* End of matched slice. */
    Py_ssize_t lastindex; /* Last group seen by the engine (-1 if none). */
    Py_ssize_t lastgroup; /* Last named group seen by the engine (-1 if none). */
    Py_ssize_t group_count;
    RE_GroupData* groups;
    PyObject* regs;
} MatchObject;

/* The ScannerObject. */
typedef struct ScannerObject {
    PyObject_HEAD
    PatternObject* pattern;
    RE_State state;
    int status;
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
    int status;
} SplitterObject;

typedef struct RE_CompileArgs {
    RE_CODE* code;
    RE_CODE* end_code;
    PatternObject* pattern;
    size_t min_width;
    RE_Node* start;
    RE_Node* end;
    size_t repeat_depth;
    BOOL forward;
    BOOL save_captures;
    BOOL has_captures;
} RE_CompileArgs;

typedef struct JoinInfo {
    PyObject* list;
    PyObject* item;
    BOOL reversed;
} JoinInfo;

typedef Py_UNICODE RE_UCHAR;
typedef unsigned char RE_BCHAR;

/* Function types for getting info from a MatchObject. */
typedef PyObject* (*RE_GetByIndexFunc)(MatchObject* self, Py_ssize_t index);

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

Py_LOCAL_INLINE(Py_ssize_t) abs_ssize_t(Py_ssize_t x) {
    return x >= 0 ? x : -x;
}

Py_LOCAL_INLINE(int) max2(int a, int b) {
    return a >= b ? a : b;
}

Py_LOCAL_INLINE(int) max3(int a, int b, int c) {
    return max2(max2(a, b), c);
}

Py_LOCAL_INLINE(int) max4(int a, int b, int c, int d) {
    return max2(max3(a, b, c), d);
}

/* Gets a byte character at the given position. */
static RE_CODE bytes_char_at(void* text, Py_ssize_t pos) {
    return *((RE_BCHAR*)text + pos);
}

/* Gets a Unicode character at the given position. */
static RE_CODE unicode_char_at(void* text, Py_ssize_t pos) {
    return *((RE_UCHAR*)text + pos);
}

/* Default for whether the current text position is on a boundary. */
static BOOL at_boundary_always(RE_State* state, Py_ssize_t text_pos) {
    return TRUE;
}

/* ASCII-specific. */

static BOOL unicode_has_property(RE_CODE property, RE_CODE ch);
static RE_CODE unicode_folded_case(RE_CODE ch);
static int unicode_get_all_cases(RE_CODE ch, RE_CODE* cases);

/* Checks whether an ASCII character has the given property. */
static BOOL ascii_has_property(RE_CODE property, RE_CODE ch) {
    if (ch > RE_ASCII_MAX) {
        /* Outside the ASCII range. */
        RE_UINT32 value;

        value = property & 0xFFFF;

        return value == 0;
    }

    return unicode_has_property(property, ch);
}

/* Converts an ASCII character to lowercase. */
static RE_CODE ascii_lower(RE_CODE ch) {
    if (ch > RE_ASCII_MAX || !re_get_uppercase(ch))
        return ch;

    return ch ^ 0x20;
}

/* Converts an ASCII character to uppercase. */
static RE_CODE ascii_upper(RE_CODE ch) {
    if (ch > RE_ASCII_MAX || !re_get_lowercase(ch))
        return ch;

    return ch ^ 0x20;
}

/* Checks whether 2 ASCII characters are the same, ignoring case. */
static BOOL ascii_same_char_ign(RE_CODE ch1, RE_CODE ch2) {
    if (ch1 > RE_ASCII_MAX)
        return ch1 == ch2;

    return re_is_same_char_ign(ch1, ch2);
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

/* Checks whether a character is a line separator. */
static BOOL ascii_is_line_sep(RE_CODE ch) {
    return 0x0A <= ch && ch <= 0x0D;
}

/* Checks whether the current text position is at the start of a line. */
static BOOL ascii_at_line_start(RE_State* state, Py_ssize_t text_pos) {
    RE_CODE ch;

    if (text_pos == 0)
        return TRUE;

    ch = state->char_at(state->text, text_pos - 1);

    if (ch == 0x0D)
        /* No line break inside CRLF. */
        return text_pos >= state->text_length || state->char_at(state->text,
          text_pos) != 0x0A;

    return 0x0A <= ch && ch <= 0x0D;
}

/* Checks whether the current text position is at the end of a line. */
static BOOL ascii_at_line_end(RE_State* state, Py_ssize_t text_pos) {
    RE_CODE ch;

    if (text_pos >= state->text_length)
        return TRUE;

    ch = state->char_at(state->text, text_pos);

    if (ch == 0x0A)
        /* No line break inside CRLF. */
        return text_pos >= 1 || state->char_at(state->text, text_pos - 1) !=
          0x0D;

    return 0x0A <= ch && ch <= 0x0D;
}

/* Checks whether a character could be Turkic (variants of I/i). For ASCII, it won't be. */
static BOOL ascii_possible_turkic(RE_CODE ch) {
    return FALSE;
}

/* Returns a character with its case folded. */
static RE_CODE ascii_folded_case(RE_CODE ch) {
    if (ch > RE_ASCII_MAX)
        return ch;

    return unicode_folded_case(ch);
}

/* Gets all the case variants of a character. */
static int ascii_get_all_cases(RE_CODE ch, RE_CODE* cases) {
    if (ch <= RE_ASCII_MAX)
        return unicode_get_all_cases(ch, cases);

    cases[0] = ch;

    return 1;
}

/* The handlers for ASCII characters. */
static RE_EncodingTable ascii_encoding = {
    ascii_has_property,
    ascii_lower,
    ascii_upper,
    ascii_upper, /* For ASCII, titlecase is the same as uppercase. */
    ascii_same_char_ign,
    ascii_at_boundary,
    ascii_at_boundary, /* No special "default word boundary" for ASCII. */
    at_boundary_always, /* No special "grapheme boundary" for ASCII. */
    ascii_is_line_sep,
    ascii_at_line_start,
    ascii_at_line_end,
    ascii_possible_turkic,
    ascii_folded_case,
    ascii_get_all_cases,
};

/* Locale-specific. */

/* Checks whether a locale character has the given property. */
static BOOL locale_has_property(RE_CODE property, RE_CODE ch) {
    RE_UINT32 value;
    RE_UINT32 v;

    value = property & 0xFFFF;

    if (ch > RE_LOCALE_MAX)
        /* Outside the locale range. */
        return value == 0;

    switch (property >> 16) {
    case RE_PROP_ANY >> 16:
        v = 1;
        break;
    case RE_PROP_ALNUM >> 16:
        v = isalnum(ch) != 0;
        break;
    case RE_PROP_ALPHA >> 16:
        v = isalpha(ch) != 0;
        break;
    case RE_PROP_ASCII >> 16:
        v = ch <= RE_ASCII_MAX;
        break;
    case RE_PROP_ASSIGNED >> 16:
        v = ch <= RE_LOCALE_MAX;
        break;
    case RE_PROP_BLANK >> 16:
        v = ch == '\t' || ch == ' ';
        break;
    case RE_PROP_GRAPH >> 16:
        v = isgraph(ch) != 0;
        break;
    case RE_PROP_LOWER >> 16:
        v = islower(ch) != 0;
        break;
    case RE_PROP_PRINT >> 16:
        v = isprint(ch) != 0;
        break;
    case RE_PROP_SPACE >> 16:
        v = isspace(ch) != 0;
        break;
    case RE_PROP_UPPER >> 16:
        v = isupper(ch) != 0;
        break;
    case RE_PROP_WORD >> 16:
        v = ch == '_' || isalnum(ch) != 0;
        break;
    case RE_PROP_XDIGIT >> 16:
        v = re_get_hex_digit(ch) != 0;
        break;
    case 0:
        switch (property) {
        case RE_PROP_CNTRL:
            v = iscntrl(ch) != 0 ? value : 0;
            break;
        case RE_PROP_DIGIT:
            v = isdigit(ch) != 0 ? value : 0;
            break;
        case RE_PROP_PUNCT:
            v = ispunct(ch) != 0 ? value : 0;
            break;
        default:
            v = 0;
            break;
        }
        break;
    default:
        v = 0;
        break;
    }

    return v == value;
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

/* Checks whether a character could be Turkic (variants of I/i). */
static BOOL locale_possible_turkic(RE_CODE ch) {
    return toupper(ch) == 'I' || tolower(ch) == 'i';
}

/* Returns a character with its case folded. */
static RE_CODE locale_folded_case(RE_CODE ch) {
    if (ch > RE_LOCALE_MAX)
        return ch;

    return tolower(ch);
}

/* Gets all the case variants of a character. */
static int locale_get_all_cases(RE_CODE ch, RE_CODE* cases) {
    int count;
    RE_CODE other;

    count = 0;

    cases[count++] = ch;

    if (ch > RE_LOCALE_MAX)
        return count;

    other = toupper(ch);
    if (other != ch)
        cases[count++] = other;

    other = tolower(ch);
    if (other != ch)
        cases[count++] = other;

    return count;
}

/* The handlers for locale characters. */
static RE_EncodingTable locale_encoding = {
    locale_has_property,
    locale_lower,
    locale_upper,
    locale_upper, /* For locale, titlecase is the same as uppercase. */
    locale_same_char_ign,
    locale_at_boundary,
    locale_at_boundary, /* No special "default word boundary" for locale. */
    at_boundary_always, /* No special "grapheme boundary" for locale. */
    ascii_is_line_sep, /* Assume locale line separators are same as ASCII. */
    ascii_at_line_start, /* Assume locale line separators are same as ASCII. */
    ascii_at_line_end, /* Assume locale line separators are same as ASCII. */
    locale_possible_turkic,
    locale_folded_case,
    locale_get_all_cases,
};

/* Unicode-specific. */

/* Unicode character properties. */

/* Checks whether a Unicode character has the given property. */
static BOOL unicode_has_property(RE_CODE property, RE_CODE ch) {
    RE_UINT32 prop;
    RE_UINT32 value;
    RE_UINT32 v;

    prop = property >> 16;
    if (prop >= sizeof(re_get_property) / sizeof(re_get_property[0]))
        return FALSE;

    value = property & 0xFFFF;
    v = re_get_property[prop](ch);

    if (v == value)
        return TRUE;

    switch (property) {
    case RE_PROP_C:
        return (RE_PROP_C_MASK & (1 << v)) != 0;
    case RE_PROP_L:
        return (RE_PROP_L_MASK & (1 << v)) != 0;
    case RE_PROP_M:
        return (RE_PROP_M_MASK & (1 << v)) != 0;
    case RE_PROP_N:
        return (RE_PROP_N_MASK & (1 << v)) != 0;
    case RE_PROP_P:
        return (RE_PROP_P_MASK & (1 << v)) != 0;
    case RE_PROP_S:
        return (RE_PROP_S_MASK & (1 << v)) != 0;
    case RE_PROP_Z:
        return (RE_PROP_Z_MASK & (1 << v)) != 0;
    }

    return FALSE;
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

/* Checks whether a character is a Unicode vowel.
 *
 * Only a limited number are treated as vowels.
 */
Py_LOCAL_INLINE(BOOL) is_unicode_vowel(RE_CODE ch) {
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

    prop = re_get_word_break(char_at(text, text_pos));
    prop_m1 = re_get_word_break(char_at(text, text_pos - 1));

    /* Don't break within CRLF. */
    if (prop_m1 == RE_BREAK_CR && prop == RE_BREAK_LF)
        return FALSE;

    /* Otherwise break before and after Newlines (including CR and LF). */
    if (prop_m1 == RE_BREAK_NEWLINE || prop_m1 == RE_BREAK_CR || prop_m1 ==
      RE_BREAK_LF || prop == RE_BREAK_NEWLINE || prop == RE_BREAK_CR || prop
      == RE_BREAK_LF)
        return TRUE;

    /* Don't break just before Format or Extend characters. */
    if (prop == RE_BREAK_EXTEND || prop == RE_BREAK_FORMAT)
        return FALSE;

    /* Get the property of the previous character. */
    pos_m1 = text_pos - 1;
    prop_m1 = RE_BREAK_OTHER;
    while (pos_m1 >= 0) {
        prop_m1 = re_get_word_break(char_at(text, pos_m1));
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
        prop_p1 = re_get_word_break(char_at(text, pos_p1));
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
        prop_m2 = re_get_word_break(char_at(text, pos_m2));
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
    if ((prop_m1 == RE_BREAK_NUMERIC || prop_m1 == RE_BREAK_ALETTER) && prop
      == RE_BREAK_NUMERIC)
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

/* Checks whether the current text position is on a grapheme boundary. */
static BOOL unicode_at_grapheme_boundary(RE_State* state, Py_ssize_t text_pos) {
    void* text;
    RE_CODE (*char_at)(void* text, Py_ssize_t pos);
    int prop;
    int prop_m1;

    /* Break at the start and end of text. */
    if (text_pos <= 0 || text_pos >= state->text_length)
        return TRUE;

    text = state->text;
    char_at = state->char_at;

    prop = re_get_grapheme_cluster_break(char_at(text, text_pos));
    prop_m1 = re_get_grapheme_cluster_break(char_at(text, text_pos - 1));

    /* Don't break within CRLF. */
    if (prop_m1 == RE_GBREAK_CR && prop == RE_GBREAK_LF)
        return FALSE;

    /* Otherwise break before and after controls (including CR and LF). */
    if (prop_m1 == RE_GBREAK_CONTROL || prop_m1 == RE_GBREAK_CR || prop_m1 ==
      RE_GBREAK_LF || prop == RE_GBREAK_CONTROL || prop == RE_GBREAK_CR || prop
      == RE_GBREAK_LF)
        return TRUE;

    /* Don't break Hangul syllable sequences. */
    if (prop_m1 == RE_GBREAK_L && (prop == RE_GBREAK_L || prop == RE_GBREAK_V ||
      prop == RE_GBREAK_LV || prop == RE_GBREAK_LVT))
        return FALSE;
    if ((prop_m1 == RE_GBREAK_LV || prop_m1 == RE_GBREAK_V) && (prop ==
      RE_GBREAK_V || prop == RE_GBREAK_T))
        return FALSE;
    if ((prop_m1 == RE_GBREAK_LVT || prop_m1 == RE_GBREAK_T) && (prop ==
      RE_GBREAK_T))
        return FALSE;

    /* Don't break just before Extend characters. */
    if (prop == RE_GBREAK_EXTEND)
        return FALSE;

    /* Don't break before SpacingMarks, or after Prepend characters. */
    if (prop == RE_GBREAK_SPACINGMARK)
        return FALSE;

    if (prop_m1 == RE_GBREAK_PREPEND)
        return FALSE;

    /* Otherwise, break everywhere. */
    return TRUE;
}

/* Checks whether a character is a line separator. */
static BOOL unicode_is_line_sep(RE_CODE ch) {
    return 0x0A <= ch && ch <= 0x0D || ch == 0x85 || ch == 0x2028 || ch ==
      0x2029;
}

/* Checks whether the current text position is at the start of a line. */
static BOOL unicode_at_line_start(RE_State* state, Py_ssize_t text_pos) {
    RE_CODE ch;

    if (text_pos == 0)
        return TRUE;

    ch = state->char_at(state->text, text_pos - 1);

    if (ch == 0x0D)
        /* No line break inside CRLF. */
        return text_pos >= state->text_length || state->char_at(state->text,
          text_pos) != 0x0A;

    return 0x0A <= ch && ch <= 0x0D || ch == 0x85 || ch == 0x2028 || ch ==
      0x2029;
}

/* Checks whether the current text position is at the end of a line. */
static BOOL unicode_at_line_end(RE_State* state, Py_ssize_t text_pos) {
    RE_CODE ch;

    if (text_pos >= state->text_length)
        return TRUE;

    ch = state->char_at(state->text, text_pos);

    if (ch == 0x0A)
        /* No line break inside CRLF. */
        return text_pos >= 1 || state->char_at(state->text, text_pos - 1) !=
          0x0D;

    return 0x0A <= ch && ch <= 0x0D || ch == 0x85 || ch == 0x2028 || ch ==
      0x2029;
}

/* Checks whether a character could be Turkic (variants of I/i). */
static BOOL unicode_possible_turkic(RE_CODE ch) {
    return ch == 'I' || ch == 'i' || ch == 0x0130 || ch == 0x0131;
}

/* Returns a character with its case folded. */
static RE_CODE unicode_folded_case(RE_CODE ch) {
    return re_folded_case(ch);
}

/* Gets all the case variants of a character. */
static int unicode_get_all_cases(RE_CODE ch, RE_CODE* cases) {
    return re_get_all_cases(ch, cases);
}

/* The handlers for Unicode characters. */
static RE_EncodingTable unicode_encoding = {
    unicode_has_property,
    unicode_lower,
    unicode_upper,
    unicode_title,
    re_is_same_char_ign,
    unicode_at_boundary,
    unicode_at_default_boundary,
    unicode_at_grapheme_boundary,
    unicode_is_line_sep,
    unicode_at_line_start,
    unicode_at_line_end,
    unicode_possible_turkic,
    unicode_folded_case,
    unicode_get_all_cases,
};

Py_LOCAL_INLINE(PyObject*) get_object(char* module_name, char* object_name);

/* Sets the error message. */
Py_LOCAL_INLINE(void) set_error(int status, PyObject* object) {
    TRACE(("<<set_error>>\n"))

    if (!error_exception)
        error_exception = get_object("_" RE_MODULE "_core", "error");

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
Py_LOCAL_INLINE(void*) re_alloc(size_t size) {
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
Py_LOCAL_INLINE(void*) re_realloc(void* ptr, size_t size) {
    void* new_ptr;

    new_ptr = PyMem_Realloc(ptr, size);
    if (!new_ptr)
        set_error(RE_ERROR_MEMORY, NULL);

    return new_ptr;
}

/* Deallocates memory. */
Py_LOCAL_INLINE(void) re_dealloc(void* ptr) {
    PyMem_Free(ptr);
}

/* Releases the GIL if multithreading is enabled. */
Py_LOCAL_INLINE(void) release_GIL(RE_SafeState* safe_state) {
    if (safe_state->re_state->is_multithreaded)
        safe_state->thread_state = PyEval_SaveThread();
}

/* Acquires the GIL if multithreading is enabled. */
Py_LOCAL_INLINE(void) acquire_GIL(RE_SafeState* safe_state) {
    if (safe_state->re_state->is_multithreaded)
        PyEval_RestoreThread(safe_state->thread_state);
}

/* Allocates memory, holding the GIL during the allocation.
 *
 * Sets the Python error handler and returns NULL if the allocation fails.
 */
Py_LOCAL_INLINE(void*) safe_alloc(RE_SafeState* safe_state, size_t size) {
    void* new_ptr;

    acquire_GIL(safe_state);

    new_ptr = re_alloc(size);

    release_GIL(safe_state);

    return new_ptr;
}

/* Reallocates memory, holding the GIL during the reallocation.
 *
 * Sets the Python error handler and returns NULL if the reallocation fails.
 */
Py_LOCAL_INLINE(void*) safe_realloc(RE_SafeState* safe_state, void* ptr,
  size_t size) {
    void* new_ptr;

    acquire_GIL(safe_state);

    new_ptr = re_realloc(ptr, size);

    release_GIL(safe_state);

    return new_ptr;
}

/* Deallocates memory, holding the GIL during the deallocation. */
Py_LOCAL_INLINE(void) safe_dealloc(RE_SafeState* safe_state, void* ptr) {
    acquire_GIL(safe_state);

    re_dealloc(ptr);

    release_GIL(safe_state);
}

/* Checks for KeyboardInterrupt, holding the GIL during the check. */
Py_LOCAL_INLINE(BOOL) safe_check_signals(RE_SafeState* safe_state) {
    BOOL result;

    acquire_GIL(safe_state);

    result = PyErr_CheckSignals();

    release_GIL(safe_state);

    return result;
}

/* Checks whether a character is in a big bitset. */
Py_LOCAL_INLINE(BOOL) in_big_bitset(RE_Node* node, RE_CODE ch) {
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
Py_LOCAL_INLINE(BOOL) in_small_bitset(RE_Node* node, RE_CODE ch) {
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

Py_LOCAL_INLINE(BOOL) in_set_diff(RE_EncodingTable* encoding, RE_Node* node,
  RE_CODE ch);
Py_LOCAL_INLINE(BOOL) in_set_inter(RE_EncodingTable* encoding, RE_Node* node,
  RE_CODE ch);
Py_LOCAL_INLINE(BOOL) in_set_sym_diff(RE_EncodingTable* encoding, RE_Node* node,
  RE_CODE ch);
Py_LOCAL_INLINE(BOOL) in_set_union(RE_EncodingTable* encoding, RE_Node* node,
  RE_CODE ch);

/* Checks whether a character is in a set diff. */
Py_LOCAL_INLINE(BOOL) in_set_diff(RE_EncodingTable* encoding, RE_Node* node,
  RE_CODE ch) {
    RE_Node* member;
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);

    member = node->nonstring.next_2.node;
    has_property = encoding->has_property;

    switch (member->op) {
    case RE_OP_BIG_BITSET:
        /* values are: size max_char indexes... subsets... */
        TRACE(("%s\n", re_op_text[member->op]))
        if (!in_big_bitset(member, ch))
            return !node->match;
        break;
    case RE_OP_CHARACTER:
        /* values are: char_code */
        TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
          member->values[0]))
        if ((ch == member->values[0]) != member->match)
            return !node->match;
        break;
    case RE_OP_PROPERTY:
        /* values are: property */
        TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
          member->values[0]))
        if (has_property(member->values[0], ch) != member->match)
            return !node->match;
        break;
    case RE_OP_SET_DIFF:
        TRACE(("%s\n", re_op_text[member->op]))
        if (!in_set_diff(encoding, member, ch))
            return !node->match;
        break;
    case RE_OP_SET_INTER:
        TRACE(("%s\n", re_op_text[member->op]))
        if (!in_set_inter(encoding, member, ch))
            return !node->match;
        break;
    case RE_OP_SET_SYM_DIFF:
        TRACE(("%s\n", re_op_text[member->op]))
        if (!in_set_sym_diff(encoding, member, ch))
            return !node->match;
        break;
    case RE_OP_SET_UNION:
        TRACE(("%s\n", re_op_text[member->op]))
        if (!in_set_union(encoding, member, ch))
            return !node->match;
        break;
    case RE_OP_SMALL_BITSET:
        /* values are: size top_bits bitset */
        TRACE(("%s\n", re_op_text[member->op]))
        if (!in_small_bitset(member, ch))
            return !node->match;
        break;
    default:
        return FALSE;
    }

    member = member->next_1.node;

    while (member) {
        switch (member->op) {
        case RE_OP_BIG_BITSET:
            /* values are: size max_char indexes... subsets... */
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_big_bitset(member, ch))
                return !node->match;
            break;
        case RE_OP_CHARACTER:
            /* values are: char_code */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              member->values[0]))
            if ((ch == member->values[0]) == member->match)
                return !node->match;
            break;
        case RE_OP_PROPERTY:
            /* values are: property */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              member->values[0]))
            if (has_property(member->values[0], ch) == member->match)
                return !node->match;
            break;
        case RE_OP_SET_DIFF:
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_set_diff(encoding, member, ch))
                return !node->match;
            break;
        case RE_OP_SET_INTER:
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_set_inter(encoding, member, ch))
                return !node->match;
            break;
        case RE_OP_SET_SYM_DIFF:
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_set_sym_diff(encoding, member, ch))
                return !node->match;
            break;
        case RE_OP_SET_UNION:
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_set_union(encoding, member, ch))
                return !node->match;
            break;
        case RE_OP_SMALL_BITSET:
            /* values are: size top_bits bitset */
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_small_bitset(member, ch))
                return !node->match;
            break;
        default:
            return FALSE;
        }

        member = member->next_1.node;
    }

    return node->match;
}

/* Checks whether a character is in a set inter. */
Py_LOCAL_INLINE(BOOL) in_set_inter(RE_EncodingTable* encoding, RE_Node* node,
  RE_CODE ch) {
    RE_Node* member;
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);

    member = node->nonstring.next_2.node;
    has_property = encoding->has_property;

    while (member) {
        switch (member->op) {
        case RE_OP_BIG_BITSET:
            /* values are: size max_char indexes... subsets... */
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_big_bitset(member, ch))
                return !node->match;
            break;
        case RE_OP_CHARACTER:
            /* values are: char_code */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              member->values[0]))
            if ((ch == member->values[0]) != member->match)
                return !node->match;
            break;
        case RE_OP_PROPERTY:
            /* values are: property */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              member->values[0]))
            if (has_property(member->values[0], ch) != member->match)
                return !node->match;
            break;
        case RE_OP_SET_DIFF:
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_set_diff(encoding, member, ch))
                return !node->match;
            break;
        case RE_OP_SET_INTER:
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_set_inter(encoding, member, ch))
                return !node->match;
            break;
        case RE_OP_SET_SYM_DIFF:
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_set_sym_diff(encoding, member, ch))
                return !node->match;
            break;
        case RE_OP_SET_UNION:
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_set_union(encoding, member, ch))
                return !node->match;
            break;
        case RE_OP_SMALL_BITSET:
            /* values are: size top_bits bitset */
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_small_bitset(member, ch))
                return !node->match;
            break;
        default:
            return FALSE;
        }

        member = member->next_1.node;
    }

    return node->match;
}

/* Checks whether a character is in a set sym diff. */
Py_LOCAL_INLINE(BOOL) in_set_sym_diff(RE_EncodingTable* encoding, RE_Node* node,
  RE_CODE ch) {
    RE_Node* member;
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);
    BOOL result;

    member = node->nonstring.next_2.node;
    has_property = encoding->has_property;

    result = !node->match;

    while (member) {
        switch (member->op) {
        case RE_OP_BIG_BITSET:
            /* values are: size max_char indexes... subsets... */
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_big_bitset(member, ch))
                result = !result;
            break;
        case RE_OP_CHARACTER:
            /* values are: char_code */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              member->values[0]))
            if ((ch == member->values[0]) != member->match)
                result = !result;
            break;
        case RE_OP_PROPERTY:
            /* values are: property */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              member->values[0]))
            if (has_property(member->values[0], ch) != member->match)
                result = !result;
            break;
        case RE_OP_SET_DIFF:
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_set_diff(encoding, member, ch))
                result = !result;
            break;
        case RE_OP_SET_INTER:
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_set_inter(encoding, member, ch))
                result = !result;
            break;
        case RE_OP_SET_SYM_DIFF:
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_set_sym_diff(encoding, member, ch))
                result = !result;
            break;
        case RE_OP_SET_UNION:
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_set_union(encoding, member, ch))
                result = !result;
            break;
        case RE_OP_SMALL_BITSET:
            /* values are: size top_bits bitset */
            TRACE(("%s\n", re_op_text[member->op]))
            if (!in_small_bitset(member, ch))
                result = !result;
            break;
        default:
            return FALSE;
        }

        member = member->next_1.node;
    }

    return result;
}

/* Checks whether a character is in a set union. */
Py_LOCAL_INLINE(BOOL) in_set_union(RE_EncodingTable* encoding, RE_Node* node,
  RE_CODE ch) {
    RE_Node* member;
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);

    member = node->nonstring.next_2.node;
    has_property = encoding->has_property;

    while (member) {
        switch (member->op) {
        case RE_OP_BIG_BITSET:
            /* values are: size max_char indexes... subsets... */
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_big_bitset(member, ch))
                return node->match;
            break;
        case RE_OP_CHARACTER:
            /* values are: char_code */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              member->values[0]))
            if ((ch == member->values[0]) == member->match)
                return node->match;
            break;
        case RE_OP_PROPERTY:
            /* values are: property */
            TRACE(("%s %d %d\n", re_op_text[member->op], member->match,
              member->values[0]))
            if (has_property(member->values[0], ch) == member->match)
                return node->match;
            break;
        case RE_OP_SET_DIFF:
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_set_diff(encoding, member, ch))
                return node->match;
            break;
        case RE_OP_SET_INTER:
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_set_inter(encoding, member, ch))
                return node->match;
            break;
        case RE_OP_SET_SYM_DIFF:
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_set_sym_diff(encoding, member, ch))
                return node->match;
            break;
        case RE_OP_SET_UNION:
            TRACE(("%s\n", re_op_text[member->op]))
            if (in_set_union(encoding, member, ch))
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

/* Checks whether a character is in a set. */
Py_LOCAL_INLINE(BOOL) in_set(RE_EncodingTable* encoding, RE_Node* node, RE_CODE
  ch) {
    switch (node->op) {
    case RE_OP_SET_INTER:
        return in_set_inter(encoding, node, ch);
    case RE_OP_SET_UNION:
        return in_set_union(encoding, node, ch);
    case RE_OP_SET_DIFF:
        return in_set_diff(encoding, node, ch);
    case RE_OP_SET_SYM_DIFF:
        return in_set_sym_diff(encoding, node, ch);
    }

    return FALSE;
}

/* Pushes the groups. */
Py_LOCAL_INLINE(BOOL) push_groups(RE_SafeState* safe_state)
  {
    RE_State* state;
    PatternObject* pattern;
    size_t new_count;
    size_t new_capacity;
    RE_GroupData* saved;
    Py_ssize_t g;

    state = safe_state->re_state;

    pattern = state->pattern;

    new_count = state->saved_groups_count + pattern->group_count;
    new_capacity = state->saved_groups_capacity;
    while (new_count > new_capacity)
        new_capacity *= 2;

    if (new_capacity != state->saved_groups_capacity) {
        RE_GroupData* new_groups;

        new_groups = (RE_GroupData*)safe_realloc(safe_state,
          state->saved_groups, new_capacity * sizeof(RE_GroupData));
        if (!new_groups)
            return FALSE;
        state->saved_groups_capacity = new_capacity;
        state->saved_groups = new_groups;
    }

    saved = &state->saved_groups[state->saved_groups_count];
    for (g = 0; g < pattern->group_count; g++)
        saved[g].span = state->groups[g].span;
    state->saved_groups_count = new_count;

    return TRUE;
}

/* Pops the groups. */
Py_LOCAL_INLINE(void) pop_groups(RE_State* state) {
    PatternObject* pattern;
    RE_GroupData* saved;
    Py_ssize_t g;

    pattern = state->pattern;

    state->saved_groups_count -= pattern->group_count;
    saved = &state->saved_groups[state->saved_groups_count];
    for (g = 0; g < pattern->group_count; g++)
        state->groups[g].span = saved[g].span;
}

/* Reloads the groups that have been pushed.
 *
 * Equivalent to pop then push.
 */
Py_LOCAL_INLINE(void) reload_groups(RE_State* state) {
    PatternObject* pattern;
    RE_GroupData* saved;
    Py_ssize_t g;

    pattern = state->pattern;

    saved = &state->saved_groups[state->saved_groups_count -
      pattern->group_count];
    for (g = 0; g < pattern->group_count; g++)
        state->groups[g].span = saved[g].span;
}

/* Drops the groups that have been pushed. */
Py_LOCAL_INLINE(void) drop_groups(RE_State* state) {
    state->saved_groups_count -= state->pattern->group_count;
}

/* Initialises the state for a match. */
Py_LOCAL_INLINE(void) init_match(RE_State* state) {
    Py_ssize_t i;

    state->saved_groups_count = 0;

    /* Reset the backtrack. */
    state->current_backtrack_block = &state->backtrack_block;
    state->current_backtrack_block->count = 0;
    if (state->current_capture_counts_block) {
        while (state->current_capture_counts_block->previous)
            state->current_capture_counts_block =
              state->current_capture_counts_block->previous;
        state->current_capture_counts_block->count = 0;
    }
    state->backtrack = NULL;
    state->search_anchor = state->text_pos;
    state->match_pos = state->text_pos;

    for (i = 0; i < state->pattern->repeat_count; i++) {
        RE_GuardList* guard_list;

        guard_list = &state->repeats[i].body_guard_list;
        guard_list->count = 0;
        guard_list->last_text_pos = -1;

        guard_list = &state->repeats[i].tail_guard_list;
        guard_list->count = 0;
        guard_list->last_text_pos = -1;
    }

    for (i = 0; i < state->pattern->group_count; i++) {
        RE_GroupData* group;

        group = &state->groups[i];
        group->span.start = -1;
        group->span.end = -1;
        group->capture_count = 0;
    }
}

/* Adds a new backtrack entry. */
Py_LOCAL_INLINE(BOOL) add_backtrack(RE_SafeState* safe_state, BYTE op) {
    RE_State* state;
    RE_BacktrackBlock* current;

    state = safe_state->re_state;

    current = state->current_backtrack_block;
    if (current->count >= current->capacity) {
        if (!current->next) {
            size_t capacity;
            size_t size;
            RE_BacktrackBlock* next;

            capacity = current->capacity * 2;
            size = sizeof(RE_BacktrackBlock) + (capacity -
              RE_BACKTRACK_BLOCK_SIZE) * sizeof(RE_BacktrackData);
            next = (RE_BacktrackBlock*)safe_alloc(safe_state, size);
            if (!next)
                return FALSE;

            next->previous = current;
            next->next = NULL;
            next->capacity = capacity;
            current->next = next;
        }
        current = current->next;
        current->count = 0;
        state->current_backtrack_block = current;
    }
    state->backtrack = &current->items[current->count++];
    state->backtrack->op = op;

    return TRUE;
}

/* Gets the last backtrack entry.
 *
 * It'll never be called when there are _no_ entries.
 */
Py_LOCAL_INLINE(RE_BacktrackData*) last_backtrack(RE_State* state) {
    RE_BacktrackBlock* current;

    current = state->current_backtrack_block;
    return &current->items[current->count - 1];
}

/* Discards the last backtrack entry.
 *
 * It'll never be called to discard the _only_ entry.
 */
Py_LOCAL_INLINE(void) discard_backtrack(RE_State* state) {
    RE_BacktrackBlock* current;

    current = state->current_backtrack_block;
    --current->count;
    if (current->count == 0 && current->previous)
        state->current_backtrack_block = current->previous;
}

/* Locates the start node for testing ahead. */
Py_LOCAL_INLINE(RE_Node*) locate_test_start(RE_Node* node) {
    for (;;) {
        switch (node->op) {
        case RE_OP_END_GROUP:
        case RE_OP_START_GROUP:
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
            return node->nonstring.next_2.node;
        default:
            return node;
        }
    }
}

Py_LOCAL_INLINE(BOOL) try_match(RE_State* state, RE_NextNode* next, Py_ssize_t
  text_pos, RE_Position* next_position);

/* Matches many ANYs. */
Py_LOCAL_INLINE(Py_ssize_t) match_many_ANY(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_ANY_REV(RE_State* state, RE_Node* node,
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

/* Matches many ANY_Us. */
Py_LOCAL_INLINE(Py_ssize_t) match_many_ANY_U(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    BOOL (*is_line_sep)(RE_CODE ch);

    is_line_sep = state->encoding->is_line_sep;

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && is_line_sep(text_ptr[0]) != match)
            ++text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && is_line_sep(text_ptr[0]) != match)
            ++text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many ANY_Us backwards. */
Py_LOCAL_INLINE(Py_ssize_t) match_many_ANY_U_REV(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    BOOL (*is_line_sep)(RE_CODE ch);

    is_line_sep = state->encoding->is_line_sep;

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && is_line_sep(text_ptr[-1]) != match)
            --text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && is_line_sep(text_ptr[-1]) != match)
            --text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many BIG_BITSETs. */
Py_LOCAL_INLINE(Py_ssize_t) match_many_BIG_BITSET(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_BIG_BITSET_REV(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_CHARACTER(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_CHARACTER_IGN(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    RE_CODE ch;

    match = node->match == match;

    same_char_ign = state->encoding->same_char_ign;

    ch = node->values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && same_char_ign(text_ptr[0], ch) == match)
            ++text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr < limit_ptr && same_char_ign(text_ptr[0], ch) == match)
            ++text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many CHARACTERs backwards, ignoring case. */
Py_LOCAL_INLINE(Py_ssize_t) match_many_CHARACTER_IGN_REV(RE_State* state,
  RE_Node* node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    RE_CODE ch;

    match = node->match == match;

    same_char_ign = state->encoding->same_char_ign;

    ch = node->values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)state->text + text_pos;
        limit_ptr = (RE_UCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && same_char_ign(text_ptr[-1], ch) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_UCHAR*)state->text;
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)state->text + text_pos;
        limit_ptr = (RE_BCHAR*)state->text + limit;

        while (text_ptr > limit_ptr && same_char_ign(text_ptr[-1], ch) ==
          match)
            --text_ptr;

        text_pos = text_ptr - (RE_BCHAR*)state->text;
    }

    return text_pos;
}

/* Matches many CHARACTERs backwards. */
Py_LOCAL_INLINE(Py_ssize_t) match_many_CHARACTER_REV(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_PROPERTY(RE_State* state, RE_Node* node,
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_PROPERTY_REV(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_SET(RE_State* state, RE_Node* node,
  Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_SET_REV(RE_State* state, RE_Node* node,
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_SMALL_BITSET(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
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
Py_LOCAL_INLINE(Py_ssize_t) match_many_SMALL_BITSET_REV(RE_State* state,
  RE_Node* node, Py_ssize_t text_pos, Py_ssize_t limit, BOOL match) {
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
Py_LOCAL_INLINE(size_t) count_one(RE_State* state, RE_Node* node, Py_ssize_t
  text_pos, RE_CODE max_count) {
    size_t available;

    if (max_count < 1)
        return 0;

    switch (node->op) {
    case RE_OP_ANY:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_ANY(state, node, text_pos, text_pos + max_count, TRUE)
          - text_pos;
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
    case RE_OP_ANY_U:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_ANY_U(state, node, text_pos, text_pos + max_count,
          TRUE) - text_pos;
    case RE_OP_ANY_U_REV:
        available = text_pos - state->slice_start;
        if (max_count > available)
            max_count = available;

        return text_pos - match_many_ANY_U_REV(state, node, text_pos, text_pos -
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
    case RE_OP_SET_DIFF:
    case RE_OP_SET_INTER:
    case RE_OP_SET_SYM_DIFF:
    case RE_OP_SET_UNION:
        available = state->slice_end - text_pos;
        if (max_count > available)
            max_count = available;

        return match_many_SET(state, node, text_pos, text_pos + max_count,
          TRUE) - text_pos;
    case RE_OP_SET_DIFF_REV:
    case RE_OP_SET_INTER_REV:
    case RE_OP_SET_SYM_DIFF_REV:
    case RE_OP_SET_UNION_REV:
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
Py_LOCAL_INLINE(BOOL) match_one(RE_State* state, RE_Node* node, Py_ssize_t
  text_pos) {
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
    case RE_OP_ANY_U:
        return text_pos < state->slice_end &&
          !state->encoding->is_line_sep(char_at(text, text_pos));
    case RE_OP_ANY_U_REV:
        return text_pos > state->slice_start &&
          !state->encoding->is_line_sep(char_at(text, text_pos - 1));
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
    case RE_OP_SET_DIFF:
    case RE_OP_SET_INTER:
    case RE_OP_SET_SYM_DIFF:
    case RE_OP_SET_UNION:
        return text_pos < state->slice_end && in_set(state->encoding, node,
          char_at(text, text_pos));
    case RE_OP_SET_DIFF_REV:
    case RE_OP_SET_INTER_REV:
    case RE_OP_SET_SYM_DIFF_REV:
    case RE_OP_SET_UNION_REV:
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
Py_LOCAL_INLINE(Py_ssize_t) simple_string_search(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit) {
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
Py_LOCAL_INLINE(Py_ssize_t) simple_string_search_ign(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    Py_ssize_t length;
    RE_CODE* values;
    RE_EncodingTable* encoding;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    RE_CODE first_char;

    text = state->text;

    length = node->value_count;
    values = node->values;

    encoding = state->encoding;
    same_char_ign = encoding->same_char_ign;

    first_char = values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        for (;;) {
            if (same_char_ign(text_ptr[0], first_char)) {
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
            if (same_char_ign(text_ptr[0], first_char)) {
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
Py_LOCAL_INLINE(Py_ssize_t) simple_string_search_ign_rev(RE_State* state,
  RE_Node* node, Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    Py_ssize_t length;
    RE_CODE* values;
    RE_EncodingTable* encoding;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    RE_CODE first_char;

    text = state->text;

    length = node->value_count;
    values = node->values;

    text_pos -= length;
    limit -= length;

    encoding = state->encoding;
    same_char_ign = encoding->same_char_ign;

    first_char = values[0];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        for (;;) {
            if (same_char_ign(text_ptr[0], first_char)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return (text_ptr + length) - (RE_UCHAR*)text;
            }

            --text_ptr;

            if (text_ptr < limit_ptr)
                break;
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        for (;;) {
            if (same_char_ign(text_ptr[0], first_char)) {
                BOOL match = TRUE;
                Py_ssize_t pos;

                for (pos = 1; match && pos < length; pos++)
                    match = same_char_ign(text_ptr[pos], values[pos]);
                if (match)
                    return (text_ptr + length) - (RE_BCHAR*)text;
            }

            --text_ptr;

            if (text_ptr < limit_ptr)
                break;
        }
    }

    return -1;
}

/* Performs a simple string search backwards. */
Py_LOCAL_INLINE(Py_ssize_t) simple_string_search_rev(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit) {
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

            --text_ptr;

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

            --text_ptr;

            if (text_ptr < limit_ptr)
                break;
        }
    }

    return -1;
}

/* Performs a Boyer-Moore fast string search. */
Py_LOCAL_INLINE(Py_ssize_t) fast_string_search(RE_State* state, RE_Node* node,
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
    good_suffix_offset = node->string.good_suffix_offset;
    bad_character_offset = node->string.bad_character_offset;

    last_pos = length - 1;
    last_char = values[last_pos];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        while (text_ptr <= limit_ptr) {
            RE_UCHAR ch;

            ch = text_ptr[last_pos];
            if (ch == last_char) {
                Py_ssize_t pos;

                pos = last_pos - 1;
                while (pos >= 0 && text_ptr[pos] == values[pos])
                    --pos;
                if (pos < 0)
                    return text_ptr - (RE_UCHAR*)text;

                text_ptr += good_suffix_offset[pos];
            } else
                text_ptr += bad_character_offset[ch & 0xFF];
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        while (text_ptr <= limit_ptr) {
            RE_BCHAR ch;

            ch = text_ptr[last_pos];
            if (ch == last_char) {
                Py_ssize_t pos;

                pos = last_pos - 1;
                while (pos >= 0 && text_ptr[pos] == values[pos])
                    --pos;
                if (pos < 0)
                    return text_ptr - (RE_BCHAR*)text;

                text_ptr += good_suffix_offset[pos];
            } else
                text_ptr += bad_character_offset[ch & 0xFF];
        }
    }

    return -1;
}

/* Performs a Boyer-Moore fast string search, ignoring case. */
Py_LOCAL_INLINE(Py_ssize_t) fast_string_search_ign(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad_character_offset;
    Py_ssize_t* good_suffix_offset;
    Py_ssize_t last_pos;
    RE_CODE last_char;

    text = state->text;
    same_char_ign = state->encoding->same_char_ign;

    length = node->value_count;
    values = node->values;
    good_suffix_offset = node->string.good_suffix_offset;
    bad_character_offset = node->string.bad_character_offset;

    last_pos = length - 1;
    last_char = values[last_pos];

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        while (text_ptr <= limit_ptr) {
            RE_UCHAR ch;

            ch = text_ptr[last_pos];
            if (same_char_ign(ch, last_char)) {
                Py_ssize_t pos;

                pos = last_pos - 1;
                while (pos >= 0 && same_char_ign(text_ptr[pos], values[pos]))
                    --pos;
                if (pos < 0)
                    return text_ptr - (RE_UCHAR*)text;

                text_ptr += good_suffix_offset[pos];
            } else
                text_ptr += bad_character_offset[ch & 0xFF];
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        while (text_ptr <= limit_ptr) {
            RE_BCHAR ch;

            ch = text_ptr[last_pos];
            if (same_char_ign(ch, last_char)) {
                Py_ssize_t pos;

                pos = last_pos - 1;
                while (pos >= 0 && same_char_ign(text_ptr[pos], values[pos]))
                    --pos;
                if (pos < 0)
                    return text_ptr - (RE_BCHAR*)text;

                text_ptr += good_suffix_offset[pos];
            } else
                text_ptr += bad_character_offset[ch & 0xFF];
        }
    }

    return -1;
}

/* Performs a Boyer-Moore fast string search backwards, ignoring case. */
Py_LOCAL_INLINE(Py_ssize_t) fast_string_search_ign_rev(RE_State* state,
  RE_Node* node, Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad_character_offset;
    Py_ssize_t* good_suffix_offset;
    RE_CODE first_char;

    text = state->text;
    same_char_ign = state->encoding->same_char_ign;

    length = node->value_count;
    values = node->values;
    good_suffix_offset = node->string.good_suffix_offset;
    bad_character_offset = node->string.bad_character_offset;

    first_char = values[0];

    text_pos -= length;
    limit -= length;

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        while (text_ptr >= limit_ptr) {
            RE_UCHAR ch;

            ch = text_ptr[0];
            if (same_char_ign(ch, first_char)) {
                Py_ssize_t pos;

                pos = 1;
                while (pos < length && same_char_ign(text_ptr[pos],
                  values[pos]))
                    ++ pos;
                if (pos >= length)
                    return (text_ptr + length) - (RE_UCHAR*)text;

                text_ptr += good_suffix_offset[pos];
            } else
                text_ptr += bad_character_offset[ch & 0xFF];
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        while (text_ptr >= limit_ptr) {
            RE_BCHAR ch;

            ch = text_ptr[0];
            if (same_char_ign(ch, first_char)) {
                Py_ssize_t pos;

                pos = 1;
                while (pos < length && same_char_ign(text_ptr[pos],
                  values[pos]))
                    ++ pos;
                if (pos >= length)
                    return (text_ptr + length) - (RE_BCHAR*)text;

                text_ptr += good_suffix_offset[pos];
            } else
                text_ptr += bad_character_offset[ch & 0xFF];
        }
    }

    return -1;
}

/* Performs a Boyer-Moore fast string search backwards. */
Py_LOCAL_INLINE(Py_ssize_t) fast_string_search_rev(RE_State* state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit) {
    void* text;
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad_character_offset;
    Py_ssize_t* good_suffix_offset;
    RE_CODE first_char;

    text = state->text;

    length = node->value_count;
    values = node->values;
    good_suffix_offset = node->string.good_suffix_offset;
    bad_character_offset = node->string.bad_character_offset;

    first_char = values[0];

    text_pos -= length;
    limit -= length;

    if (state->wide) {
        RE_UCHAR* text_ptr;
        RE_UCHAR* limit_ptr;

        text_ptr = (RE_UCHAR*)text + text_pos;
        limit_ptr = (RE_UCHAR*)text + limit;

        while (text_ptr >= limit_ptr) {
            RE_UCHAR ch;

            ch = text_ptr[0];
            if (ch == first_char) {
                Py_ssize_t pos;

                pos = 1;
                while (pos < length && text_ptr[pos] == values[pos])
                    ++pos;
                if (pos >= length)
                    return (text_ptr + length) - (RE_UCHAR*)text;

                text_ptr += good_suffix_offset[pos];
            } else
                text_ptr += bad_character_offset[ch & 0xFF];
        }
    } else {
        RE_BCHAR* text_ptr;
        RE_BCHAR* limit_ptr;

        text_ptr = (RE_BCHAR*)text + text_pos;
        limit_ptr = (RE_BCHAR*)text + limit;

        while (text_ptr >= limit_ptr) {
            RE_BCHAR ch;

            ch = text_ptr[0];
            if (ch == first_char) {
                Py_ssize_t pos;

                pos = 1;
                while (pos < length && text_ptr[pos] == values[pos])
                    ++pos;
                if (pos >= length)
                    return (text_ptr + length) - (RE_BCHAR*)text;

                text_ptr += good_suffix_offset[pos];
            } else
                text_ptr += bad_character_offset[ch & 0xFF];
        }
    }

    return -1;
}

/* Check whether 2 characters are the same. */
static BOOL same_char(RE_CODE ch1, RE_CODE ch2) {
    return ch1 == ch2;
}

/* Build the tables for a Boyer-Moore fast string search. */
Py_LOCAL_INLINE(BOOL) build_fast_tables(RE_EncodingTable* encoding, RE_Node*
  node, BOOL ignore) {
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad;
    Py_ssize_t* good;
    RE_CODE ch;
    Py_ssize_t last_pos;
    Py_ssize_t pos;
    BOOL (*is_same_char)(RE_CODE ch1, RE_CODE ch2);
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

    for (ch = 0; ch < 0x100; ch++)
        bad[ch] = length;

    last_pos = length - 1;

    for (pos = 0; pos < last_pos; pos++) {
        Py_ssize_t offset;
        RE_CODE ch_lower;
        RE_CODE ch_upper;
        RE_CODE ch_title;

        offset = last_pos - pos;
        ch = values[pos];
        if (ignore) {
            ch_lower = encoding->lower(ch);
            bad[ch_lower & 0xFF] = offset;
            ch_upper = encoding->upper(ch);
            bad[ch_upper & 0xFF] = offset;
            if (ch_lower != ch_upper) {
                ch_title = encoding->title(ch);
                bad[ch_title & 0xFF] = offset;
            }
        } else
            bad[ch & 0xFF] = offset;
    }

    is_same_char = ignore ? encoding->same_char_ign : same_char;

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

            if (is_same_char(values[s + i], values[pos + i]))
                /* It still matches. */
                --i;
            else {
                /* Start again further along. */
                --s;
                i = suffix_len - 1;
            }
        }

        if (s >= 0 && is_same_char(values[s], values[pos])) {
            /* We haven't dropped off the end of the string, and the suffix has
             * matched this far, so this is a good starting point for the next
             * iteration.
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

    node->string.bad_character_offset = bad;
    node->string.good_suffix_offset = good;

    return TRUE;
}

/* Build the tables for a Boyer-Moore fast string search backwards. */
Py_LOCAL_INLINE(BOOL) build_fast_tables_rev(RE_EncodingTable* encoding,
  RE_Node* node, BOOL ignore) {
    Py_ssize_t length;
    RE_CODE* values;
    Py_ssize_t* bad;
    Py_ssize_t* good;
    RE_CODE ch;
    Py_ssize_t pos;
    BOOL (*is_same_char)(RE_CODE ch1, RE_CODE ch2);
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
        ch = values[pos];
        if (ignore) {
           ch_lower = encoding->lower(ch);
           bad[ch_lower & 0xFF] = offset;
           ch_upper = encoding->upper(ch);
           bad[ch_upper & 0xFF] = offset;
           if (ch_lower != ch_upper) {
               ch_title = encoding->title(ch);
               bad[ch_title & 0xFF] = offset;
           }
        } else
            bad[ch & 0xFF] = offset;
    }

    is_same_char = ignore ? encoding->same_char_ign : same_char;

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

            if (is_same_char(values[s - i], values[pos - i]))
                /* It still matches. */
                --i;
            else {
                /* Start again further along. */
                ++s;
                i = suffix_len - 1;
            }
        }

        if (s < length && is_same_char(values[s], values[pos])) {
            /* We haven't dropped off the end of the string, and the suffix has
             * matched this far, so this is a good starting point for the next
             * iteration.
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

    node->string.bad_character_offset = bad;
    node->string.good_suffix_offset = good;

    return TRUE;
}

/* Performs a string search. */
Py_LOCAL_INLINE(Py_ssize_t) string_search(RE_SafeState* safe_state, RE_Node*
  node, Py_ssize_t text_pos, Py_ssize_t limit) {
    RE_State* state;

    state = safe_state->re_state;

    if (text_pos > limit)
        return -1;

    /* Has the node been initialised for fast searching, if necessary? */
    if (!(node->status & RE_STATUS_FAST_INIT)) {
        /* Ideally the pattern should immutable and shareable across threads.
         * Internally, however, it isn't. For safety we need to hold the GIL.
         */
        acquire_GIL(safe_state);

        /* Double-check because of multithreading. */
        if (!(node->status & RE_STATUS_FAST_INIT)) {
            build_fast_tables(state->encoding, node, FALSE);
            node->status |= RE_STATUS_FAST_INIT;
        }

        release_GIL(safe_state);
    }

    if (node->string.bad_character_offset)
        text_pos = fast_string_search(state, node, text_pos, limit);
    else
        text_pos = simple_string_search(state, node, text_pos, limit);

    return text_pos;
}

/* Performs a string search, ignoring case. */
Py_LOCAL_INLINE(Py_ssize_t) string_search_ign(RE_SafeState* safe_state,
  RE_Node* node, Py_ssize_t text_pos, Py_ssize_t limit) {
    RE_State* state;

    state = safe_state->re_state;

    if (text_pos > limit)
        return -1;

    /* Has the node been initialised for fast searching, if necessary? */
    if (!(node->status & RE_STATUS_FAST_INIT)) {
        /* Ideally the pattern should immutable and shareable across threads.
         * Internally, however, it isn't. For safety we need to hold the GIL.
         */
        acquire_GIL(safe_state);

        /* Double-check because of multithreading. */
        if (!(node->status & RE_STATUS_FAST_INIT)) {
            build_fast_tables(state->encoding, node, TRUE);
            node->status |= RE_STATUS_FAST_INIT;
        }

        release_GIL(safe_state);
    }

    if (node->string.bad_character_offset)
        text_pos = fast_string_search_ign(state, node, text_pos, limit);
    else
        text_pos = simple_string_search_ign(state, node, text_pos, limit);

    return text_pos;
}

/* Performs a string search backwards, ignoring case. */
Py_LOCAL_INLINE(Py_ssize_t) string_search_ign_rev(RE_SafeState* safe_state,
  RE_Node* node, Py_ssize_t text_pos, Py_ssize_t limit) {
    RE_State* state;

    state = safe_state->re_state;

    if (text_pos < limit)
        return -1;

    /* Has the node been initialised for fast searching, if necessary? */
    if (!(node->status & RE_STATUS_FAST_INIT)) {
        /* Ideally the pattern should immutable and shareable across threads.
         * Internally, however, it isn't. For safety we need to hold the GIL.
         */
        acquire_GIL(safe_state);

        /* Double-check because of multithreading. */
        if (!(node->status & RE_STATUS_FAST_INIT)) {
            build_fast_tables_rev(state->encoding, node, TRUE);
            node->status |= RE_STATUS_FAST_INIT;
        }

        release_GIL(safe_state);
    }

    if (node->string.bad_character_offset)
        text_pos = fast_string_search_ign_rev(state, node, text_pos, limit);
    else
        text_pos = simple_string_search_ign_rev(state, node, text_pos, limit);

    return text_pos;
}

/* Performs a string search backwards. */
Py_LOCAL_INLINE(Py_ssize_t) string_search_rev(RE_SafeState* safe_state,
  RE_Node* node, Py_ssize_t text_pos, Py_ssize_t limit) {
    RE_State* state;

    state = safe_state->re_state;

    if (text_pos < limit)
        return -1;

    /* Has the node been initialised for fast searching, if necessary? */
    if (!(node->status & RE_STATUS_FAST_INIT)) {
        /* Ideally the pattern should immutable and shareable across threads.
         * Internally, however, it isn't. For safety we need to hold the GIL.
         */
        acquire_GIL(safe_state);

        /* Double-check because of multithreading. */
        if (!(node->status & RE_STATUS_FAST_INIT)) {
            build_fast_tables_rev(state->encoding, node, FALSE);
            node->status |= RE_STATUS_FAST_INIT;
        }

        release_GIL(safe_state);
    }

    if (node->string.bad_character_offset)
        text_pos = fast_string_search_rev(state, node, text_pos, limit);
    else
        text_pos = simple_string_search_rev(state, node, text_pos, limit);

    return text_pos;
}

/* Tries a match at the current text position.
 *
 * Returns TRUE and the next node and text position if the match succeeds.
 */
Py_LOCAL_INLINE(BOOL) try_match(RE_State* state, RE_NextNode* next, Py_ssize_t
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
    case RE_OP_ANY_U: /* Any character, except a line separator. */
        if (text_pos >= state->slice_end ||
          state->encoding->is_line_sep(char_at(text, text_pos)))
            return FALSE;
        break;
    case RE_OP_ANY_U_REV: /* Any character, except a line separator. */
        if (text_pos <= state->slice_start ||
          state->encoding->is_line_sep(char_at(text, text_pos - 1)))
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
          !try_match(state, &test->nonstring.next_2, text_pos, next_position))
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
    case RE_OP_END_OF_LINE_U: /* At the end of a line. */
        if (!state->encoding->at_line_end(state, text_pos))
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
    case RE_OP_END_OF_STRING_LINE_U: /* At end of string or final newline. */
        if (text_pos != state->text_length && text_pos != state->final_line_sep)
            return FALSE;
        break;
    case RE_OP_GRAPHEME_BOUNDARY: /* At a grapheme boundary. */
        if (state->encoding->at_grapheme_boundary(state, text_pos) !=
          test->match)
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
    case RE_OP_SET_DIFF: /* Character set. */
    case RE_OP_SET_INTER:
    case RE_OP_SET_SYM_DIFF:
    case RE_OP_SET_UNION:
        if (text_pos >= state->slice_end || !in_set(state->encoding, test,
          char_at(text, text_pos)))
            return FALSE;
        break;
    case RE_OP_SET_DIFF_REV: /* Character set. */
    case RE_OP_SET_INTER_REV:
    case RE_OP_SET_SYM_DIFF_REV:
    case RE_OP_SET_UNION_REV:
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
    case RE_OP_START_OF_LINE_U: /* At the start of a line. */
        if (!state->encoding->at_line_start(state, text_pos))
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
Py_LOCAL_INLINE(BOOL) general_check(RE_SafeState* safe_state, RE_Node* node,
  Py_ssize_t text_pos) {
    RE_State* state;

    state = safe_state->re_state;

    for (;;) {
        switch (node->op) {
        case RE_OP_BRANCH:
            if (general_check(safe_state, node->next_1.check, text_pos))
                return TRUE;
            node = node->nonstring.next_2.check;
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
            node = node->nonstring.next_2.check;
            break;
        case RE_OP_GREEDY_REPEAT:
        case RE_OP_LAZY_REPEAT:
            if (node->values[1] >= 1)
                node = node->next_1.check;
            else
                node = node->nonstring.next_2.check;
            break;
        case RE_OP_GROUP_EXISTS:
            if (general_check(safe_state, node->next_1.check, text_pos))
                return TRUE;
            node = node->nonstring.next_2.check;
            break;
        case RE_OP_STRING:
            text_pos = string_search(safe_state, node, text_pos,
              state->slice_end - node->value_count);
            return text_pos >= 0;
        case RE_OP_STRING_IGN:
            text_pos = string_search_ign(safe_state, node, text_pos,
              state->slice_end - node->value_count);
            return text_pos >= 0;
        case RE_OP_STRING_IGN_REV:
            text_pos = string_search_ign_rev(safe_state, node, text_pos,
              state->slice_start + node->value_count);
            return text_pos >= 0;
        case RE_OP_STRING_REV:
            text_pos = string_search_rev(safe_state, node, text_pos,
              state->slice_start + node->value_count);
            return text_pos >= 0;
        case RE_OP_SUCCESS:
            return TRUE;
        default:
            node = node->next_1.check;
            break;
        }
    }
}

/* Returns the next node to check, also setting the checks for the following
 * nodes.
 */
Py_LOCAL_INLINE(RE_Node*) next_check(RE_Node* node) {
    for (;;) {
        if (node->status & RE_STATUS_VISITED_NC)
            return node->next_check;

        switch (node->op) {
        case RE_OP_BRANCH:
        {
            RE_Node* branch_1;
            RE_Node* branch_2;
            RE_Node* next;

            branch_1 = next_check(node->next_1.node);
            branch_2 = next_check(node->nonstring.next_2.node);
            if (branch_1 == branch_2)
                next = branch_1;
            else if (!branch_1 || !branch_2)
                next = NULL;
            else {
                node->next_1.check = branch_1;
                node->nonstring.next_2.check = branch_2;
                next = node;
            }
            node->next_check = next;
            node->status |= RE_STATUS_VISITED_NC;
            return next;
        }
        case RE_OP_CHARACTER:
        case RE_OP_CHARACTER_IGN:
        case RE_OP_CHARACTER_IGN_REV:
        case RE_OP_CHARACTER_REV:
            node->next_check = node;
            node->status |= RE_STATUS_VISITED_NC;
            return node;
        case RE_OP_END_GREEDY_REPEAT:
        case RE_OP_END_LAZY_REPEAT:
            node->next_check = NULL;
            node->status |= RE_STATUS_VISITED_NC;
            return NULL;
        case RE_OP_GREEDY_REPEAT:
        case RE_OP_LAZY_REPEAT:
        {
            RE_Node* next;

            next = NULL;
            if (node->values[1] >= 1)
                next = next_check(node->next_1.node);
            if (!next)
                next = next_check(node->nonstring.next_2.node);
            node->next_check = next;
            node->status |= RE_STATUS_VISITED_NC;
            return next;
        }
        case RE_OP_GROUP_EXISTS:
        {
            RE_Node* branch_1;
            RE_Node* branch_2;
            RE_Node* next;

            branch_1 = next_check(node->next_1.node);
            branch_2 = next_check(node->nonstring.next_2.node);
            if (branch_1 == branch_2)
                next = branch_1;
            else if (!branch_1 || !branch_2)
                next = NULL;
            else {
                node->next_1.check = branch_1;
                node->nonstring.next_2.check = branch_2;
                next = node;
            }
            node->next_check = next;
            node->status |= RE_STATUS_VISITED_NC;
            return next;
        }
        case RE_OP_STRING:
        case RE_OP_STRING_IGN:
        case RE_OP_STRING_IGN_REV:
        case RE_OP_STRING_REV:
            node->next_check = node;
            node->status |= RE_STATUS_VISITED_NC;
            return node;
        case RE_OP_SUCCESS:
            node->next_check = NULL;
            node->status |= RE_STATUS_VISITED_NC;
            return NULL;
        default:
            node = node->next_1.node;
            break;
        }
    }
}

/* Searches for the start of a match. */
Py_LOCAL_INLINE(BOOL) search_start(RE_SafeState* safe_state, RE_NextNode* next,
  RE_Position* new_position) {
    RE_State* state;
    Py_ssize_t text_pos;
    RE_Node* test;
    RE_Node* node;
    Py_ssize_t start_pos;
    Py_ssize_t step;
    Py_ssize_t limit;

    state = safe_state->re_state;

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
    case RE_OP_ANY_U: /* Any character, except a line separator. */
        start_pos = match_many_ANY_U(state, node, start_pos, limit + 1, FALSE);
        if (start_pos > limit)
            return FALSE;
        break;
    case RE_OP_ANY_U_REV: /* Any character backwards, except a line separator. */
        start_pos = match_many_ANY_U_REV(state, node, start_pos, limit - 1,
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
        start_pos = match_many_BIG_BITSET_REV(state, test, start_pos, limit - 1,
          FALSE);
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
        start_pos = match_many_CHARACTER_IGN_REV(state, test, start_pos, limit -
          1, FALSE);
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
    case RE_OP_GRAPHEME_BOUNDARY: /* At a grapheme boundary. */
    {
        BOOL match;
        Py_ssize_t step;
        BOOL (*at_boundary)(RE_State* state, Py_ssize_t start_pos);

        match = test->match;
        step = state->reverse ? -1 : 1;
        at_boundary = state->encoding->at_grapheme_boundary;

        for (;;) {
            if (at_boundary(state, start_pos) == match)
                break;
            if (start_pos == limit)
                return FALSE;
            start_pos += step;
        }
        break;
    }
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
    case RE_OP_SET_DIFF: /* A set. */
    case RE_OP_SET_INTER:
    case RE_OP_SET_SYM_DIFF:
    case RE_OP_SET_UNION:
        start_pos = match_many_SET(state, test, start_pos, limit + 1, FALSE);
        if (start_pos > limit)
            return FALSE;
        break;
    case RE_OP_SET_DIFF_REV: /* A set backwards. */
    case RE_OP_SET_INTER_REV:
    case RE_OP_SET_SYM_DIFF_REV:
    case RE_OP_SET_UNION_REV:
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
        start_pos = string_search(safe_state, test, start_pos, limit);
        if (start_pos < 0)
            return FALSE;
        break;
    case RE_OP_STRING_IGN: /* A string literal, ignoring case. */
        start_pos = string_search_ign(safe_state, test, start_pos, limit);
        if (start_pos < 0)
            return FALSE;
        break;
    case RE_OP_STRING_IGN_REV: /* A string literal backwards, ignoring case. */
        start_pos = string_search_ign_rev(safe_state, test, start_pos, limit);
        if (start_pos < 0)
            return FALSE;
        break;
    case RE_OP_STRING_REV: /* A string literal backwards. */
        start_pos = string_search_rev(safe_state, test, start_pos, limit);
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

/* Saves a capture group. */
Py_LOCAL_INLINE(BOOL) save_capture(RE_SafeState* safe_state, size_t index) {
    RE_State* state;
    RE_GroupData* group;

    state = safe_state->re_state;

    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    group = &state->groups[index - 1];
    if (group->capture_count >= group->capture_capacity) {
        size_t new_capacity;
        RE_GroupSpan* new_captures;

        new_capacity = group->capture_capacity * 2;
        if (new_capacity == 0)
            new_capacity = 16;
        new_captures = (RE_GroupSpan*)safe_realloc(safe_state, group->captures,
          new_capacity * sizeof(RE_GroupSpan));
        if (!new_captures)
            return FALSE;

        group->captures = new_captures;
        group->capture_capacity = new_capacity;
    }
    group->captures[group->capture_count++] = group->span;

    return TRUE;
}

/* Unsaves a capture group. */
Py_LOCAL_INLINE(void) unsave_capture(RE_State* state, size_t index) {
    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    --state->groups[index - 1].capture_count;
}

/* Saves all the capture counts to backtrack. */
Py_LOCAL_INLINE(BOOL) save_all_captures(RE_SafeState* safe_state,
  RE_BacktrackData* bt_data) {
    RE_State* state;
    Py_ssize_t group_count;
    RE_CaptureCountsBlock* current;
    size_t* capture_counts;
    Py_ssize_t g;

    state = safe_state->re_state;

    group_count = state->pattern->group_count;
    if (group_count == 0) {
        bt_data->captures.capture_counts = NULL;
        return TRUE;
    }

    current = state->current_capture_counts_block;
    if (!current || current->count + group_count > current->capacity) {
        RE_CaptureCountsBlock* new_block;
        size_t n;
        size_t new_capacity;

        if (current && current->next)
            new_block = current->next;
        else {
            new_block = (RE_CaptureCountsBlock*)safe_alloc(safe_state,
              sizeof(RE_CaptureCountsBlock));
            if (!new_block)
                return FALSE;

            n = RE_CAPTURES_BLOCK_SIZE / group_count;
            if (n < 1)
                n = 1;
            new_capacity = n * group_count;

            new_block->items = (size_t*)safe_alloc(safe_state, new_capacity *
              sizeof(size_t));
            if (!new_block->items) {
                safe_dealloc(safe_state, new_block);
                return FALSE;
            }

            new_block->previous = current;
            new_block->next = NULL;
            new_block->capacity = new_capacity;
        }

        new_block->count = 0;
        state->current_capture_counts_block = new_block;
        current = new_block;
    }

    capture_counts = &current->items[current->count];
    for (g = 0; g < group_count; g++)
        capture_counts[g] = state->groups[g].capture_count;
    current->count += group_count;

    return TRUE;
}

/* Restores all the capture counts from backtrack. */
Py_LOCAL_INLINE(void) restore_all_captures(RE_State* state, RE_BacktrackData*
  bt_data) {
    Py_ssize_t group_count;
    RE_CaptureCountsBlock* current;
    size_t* capture_counts;
    Py_ssize_t g;

    group_count = state->pattern->group_count;
    if (group_count == 0)
        return;

    current = state->current_capture_counts_block;
    if (current->count == 0)
        current = current->previous;
    current->count -= group_count;
    capture_counts = &current->items[current->count];
    for (g = 0; g < group_count; g++)
        state->groups[g].capture_count = capture_counts[g];
    state->current_capture_counts_block = current;
}

/* Saves state info before a recusive call by 'basic_match'. */
Py_LOCAL_INLINE(void) save_info(RE_State* state, RE_Info* info) {
    info->current_backtrack_block = state->current_backtrack_block;
    info->backtrack_count = info->current_backtrack_block->count;
    info->current_capture_counts_block = state->current_capture_counts_block;
    if (info->current_capture_counts_block)
        info->captures_count = info->current_capture_counts_block->count;
    info->saved_groups_count = state->saved_groups_count;
    info->must_advance = state->must_advance;
}

/* Restores state info after a recusive call by 'basic_match'. */
Py_LOCAL_INLINE(void) restore_info(RE_State* state, RE_Info* info) {
    state->must_advance = info->must_advance;
    state->saved_groups_count = info->saved_groups_count;
    if (info->current_capture_counts_block) {
        info->current_capture_counts_block->count = info->captures_count;
        state->current_capture_counts_block =
          info->current_capture_counts_block;
    }
    info->current_backtrack_block->count = info->backtrack_count;
    state->current_backtrack_block = info->current_backtrack_block;
}

/* Deletes a new span in a guard list. */
Py_LOCAL_INLINE(BOOL) insert_guard_span(RE_SafeState* safe_state, RE_GuardList*
  guard_list, Py_ssize_t index) {
    size_t n;

    if (guard_list->count >= guard_list->capacity) {
        size_t new_capacity;
        RE_GuardSpan* new_spans;

        new_capacity = guard_list->capacity * 2;
        if (new_capacity == 0)
            new_capacity = 16;
        new_spans = (RE_GuardSpan*)safe_realloc(safe_state, guard_list->spans,
          new_capacity * sizeof(RE_GuardSpan));
        if (!new_spans)
            return FALSE;

        guard_list->capacity = new_capacity;
        guard_list->spans = new_spans;
    }

    n = guard_list->count - index;
    if (n > 0)
        memmove(guard_list->spans + index + 1, guard_list->spans + index, n *
          sizeof(RE_GuardSpan));
    ++guard_list->count;

    return TRUE;
}

/* Deletes a span in a guard list. */
Py_LOCAL_INLINE(void) delete_guard_span(RE_GuardList* guard_list, Py_ssize_t
  index) {
    size_t n;

    n = guard_list->count - index - 1;
    if (n > 0)
        memmove(guard_list->spans + index, guard_list->spans + index + 1, n *
          sizeof(RE_GuardSpan));
    --guard_list->count;
}

/* Guards a position against further matching. */
Py_LOCAL_INLINE(BOOL) guard(RE_SafeState* safe_state, size_t index, Py_ssize_t
  text_pos, RE_STATUS_T guard) {
    RE_State* state;
    RE_GuardList* guard_list;
    size_t low;
    size_t high;

    state = safe_state->re_state;

    /* Is a guard active here? */
    if (!(state->pattern->repeat_info[index].status & guard))
        return TRUE;

    /* Which guard list? */
    if (guard & RE_STATUS_BODY)
        guard_list = &state->repeats[index].body_guard_list;
    else
        guard_list = &state->repeats[index].tail_guard_list;

    /* Where should be new position be added? */
    if (text_pos == guard_list->last_text_pos)
        low = guard_list->last_low;
    else {
        low = 0;
        high = guard_list->count;
        while (low < high) {
            size_t mid;

            mid = (low + high) / 2;
            if (text_pos < guard_list->spans[mid].low)
                high = mid;
            else if (text_pos > guard_list->spans[mid].high)
                low = mid + 1;
            else
                return TRUE;
        }
    }

    /* Add the position to the guard list. */
    if (low > 0 && guard_list->spans[low - 1].high + 1 == text_pos) {
        if (low < guard_list->count && guard_list->spans[low].low - 1 ==
          text_pos) {
            /* The new position joins 2 spans */
            guard_list->spans[low - 1].high = guard_list->spans[low].high;
            delete_guard_span(guard_list, low);
        } else
            /* Extend the span. */
            guard_list->spans[low - 1].high = text_pos;
    } else if (low < guard_list->count && guard_list->spans[low].low - 1 ==
      text_pos)
            /* Extend the span. */
        guard_list->spans[low].low = text_pos;
    else {
        /* Insert a new span.*/
        if (!insert_guard_span(safe_state, guard_list, low))
            return FALSE;
        guard_list->spans[low].low = text_pos;
        guard_list->spans[low].high = text_pos;
    }

    guard_list->last_text_pos = -1;

    return TRUE;
}

/* Checks whether a position is guarded against further matching. */
Py_LOCAL_INLINE(BOOL) is_guarded(RE_State* state, size_t index, Py_ssize_t
  text_pos, RE_STATUS_T guard) {
    RE_GuardList* guard_list;
    size_t low;
    size_t high;

    /* Is a guard active here? */
    if (!(state->pattern->repeat_info[index].status & guard))
        return FALSE;

    /* Which guard list? */
    if (guard == RE_STATUS_BODY)
        guard_list = &state->repeats[index].body_guard_list;
    else
        guard_list = &state->repeats[index].tail_guard_list;

    /* Is this position in the guard list? */
    low = 0;
    high = guard_list->count;
    while (low < high) {
        size_t mid;

        mid = (low + high) / 2;
        if (text_pos < guard_list->spans[mid].low)
            high = mid;
        else if (text_pos > guard_list->spans[mid].high)
            low = mid + 1;
        else
            return TRUE;
    }

    guard_list->last_text_pos = text_pos;
    guard_list->last_low = low;

    return FALSE;
}

/* Resets the guards inside atomic subpatterns and lookarounds. */
Py_LOCAL_INLINE(void) reset_guards(RE_State* state, RE_CODE* values) {
    size_t i;

    if (values) {
        for (i = 1; i <= values[0]; i++) {
            RE_GuardList* guard_list;

            guard_list = &state->repeats[values[i]].body_guard_list;
            guard_list->count = 0;
            guard_list->last_text_pos = -1;

            guard_list = &state->repeats[values[i]].tail_guard_list;
            guard_list->count = 0;
            guard_list->last_text_pos = -1;
        }
    } else {
        PatternObject* pattern;

        pattern = state->pattern;

        for (i = 0; i < (size_t)pattern->repeat_count; i++) {
            RE_GuardList* guard_list;

            guard_list = &state->repeats[i].body_guard_list;
            guard_list->count = 0;
            guard_list->last_text_pos = -1;

            guard_list = &state->repeats[i].tail_guard_list;
            guard_list->count = 0;
            guard_list->last_text_pos = -1;
        }
    }
}

Py_LOCAL_INLINE(void) fold_string_case_u(RE_UCHAR* buffer, Py_ssize_t len) {
    Py_ssize_t i;

    for (i = 0; i < len; i++) {
        if (!unicode_possible_turkic(buffer[i]))
            buffer[i] = unicode_folded_case(buffer[i]);
    }
}

Py_LOCAL_INLINE(void) fold_string_case_b(RE_EncodingTable* encoding, RE_BCHAR*
  buffer, Py_ssize_t len) {
    Py_ssize_t i;

    for (i = 0; i < len; i++) {
        if (!encoding->possible_turkic(buffer[i]))
            buffer[i] = encoding->folded_case(buffer[i]);
    }
}

Py_LOCAL_INLINE(int) look_in_string_set_u(PyObject* string_set, RE_UCHAR*
  buffer, Py_ssize_t index, Py_ssize_t len) {
    RE_CODE cases[RE_MAX_CASE_DIFFS + 1];
    int count;
    int i;

    while (index < len && !unicode_possible_turkic(buffer[index]))
        ++index;

    if (index >= len) {
        PyObject* string;
        int status;

        string = Py_BuildValue("u#", buffer, len);
        if (!string)
            return -1;

        status = PySet_Contains(string_set, string);
        Py_DECREF(string);

        return status;
    }

    count = unicode_get_all_cases(buffer[index], cases);

    for (i = 0; i < count; i++) {
        int status;

        buffer[index] = cases[i];

        status = look_in_string_set_u(string_set, buffer, index + 1, len);
        if (status != 0)
            return status;
    }

    return 0;
}

Py_LOCAL_INLINE(int) look_in_string_set_b(RE_EncodingTable* encoding, PyObject*
  string_set, RE_BCHAR* buffer, Py_ssize_t index, Py_ssize_t len) {
    RE_CODE cases[RE_MAX_CASE_DIFFS + 1];
    int count;
    int i;

    while (index < len && !encoding->possible_turkic(buffer[index]))
        ++index;

    if (index >= len) {
        PyObject* string;
        int status;

        string = Py_BuildValue("y#", buffer, len);
        if (!string)
            return -1;

        status = PySet_Contains(string_set, string);
        Py_DECREF(string);

        return status;
    }

    count = encoding->get_all_cases(buffer[index], cases);

    for (i = 0; i < count; i++) {
        int status;

        buffer[index] = cases[i];

        status = look_in_string_set_b(encoding, string_set, buffer, index + 1,
          len);
        if (status != 0)
            return status;
    }

    return 0;
}

Py_LOCAL_INLINE(int) string_set_contains_ign_u(PyObject* string_set, RE_UCHAR*
  text_ptr, Py_ssize_t len) {
    RE_UCHAR* buffer;
    int status;

    buffer = PyMem_MALLOC(len * sizeof(RE_UCHAR));
    if (!buffer)
        return -1;

    memmove(buffer, text_ptr, len * sizeof(RE_UCHAR));

    fold_string_case_u(buffer, len);

    status = look_in_string_set_u(string_set, buffer, 0, len);

    PyMem_FREE(buffer);

    return status;
}

Py_LOCAL_INLINE(int) string_set_contains_ign_b(RE_EncodingTable* encoding,
  PyObject* string_set, RE_BCHAR* text_ptr, Py_ssize_t len) {
    RE_BCHAR* buffer;
    int status;

    buffer = PyMem_MALLOC(len * sizeof(RE_BCHAR));
    if (!buffer)
        return -1;

    memmove(buffer, text_ptr, len * sizeof(RE_BCHAR));

    fold_string_case_b(encoding, buffer, len);

    status = look_in_string_set_b(encoding, string_set, buffer, 0, len);

    PyMem_FREE(buffer);

    return status;
}

Py_LOCAL_INLINE(BOOL) string_set_match(RE_SafeState* safe_state, RE_Node* node)
  {
    Py_ssize_t index;
    Py_ssize_t min_len;
    Py_ssize_t max_len;
    RE_State* state;
    Py_ssize_t available;
    PyObject* string_set;
    int status;
    Py_ssize_t len;

    index = node->values[0];
    min_len = node->values[1];
    max_len = node->values[2];

    state = safe_state->re_state;

    available = state->slice_end - state->text_pos;
    if (available < min_len)
        return FALSE;

    if (max_len > available)
        max_len = available;

    acquire_GIL(safe_state);

    string_set = PyList_GET_ITEM(state->pattern->ref_lists, index);
    if (!string_set)
        goto error;

    status = 0;

    for (len = max_len; status == 0 && len >= min_len; len--) {
        PyObject* string;

        if (state->wide) {
            RE_UCHAR* text_ptr;

            text_ptr = (RE_UCHAR*)state->text + state->text_pos;
            string = Py_BuildValue("u#", text_ptr, len);
        } else {
            RE_BCHAR* text_ptr;

            text_ptr = (RE_BCHAR*)state->text + state->text_pos;
            string = Py_BuildValue("y#", text_ptr, len);
        }
        if (!string)
            goto error;

        status = PySet_Contains(string_set, string);
        Py_DECREF(string);

        if (status == 1)
            state->text_pos += len;
    }

    release_GIL(safe_state);

    return status == 1;

error:
    release_GIL(safe_state);
    return FALSE;
}

Py_LOCAL_INLINE(BOOL) string_set_match_ign(RE_SafeState* safe_state, RE_Node* node)
  {
    Py_ssize_t index;
    Py_ssize_t min_len;
    Py_ssize_t max_len;
    RE_State* state;
    Py_ssize_t available;
    PyObject* string_set;
    int status;
    Py_ssize_t len;

    index = node->values[0];
    min_len = node->values[1];
    max_len = node->values[2];

    state = safe_state->re_state;

    available = state->slice_end - state->text_pos;
    if (available < min_len)
        return FALSE;

    if (max_len > available)
        max_len = available;

    acquire_GIL(safe_state);

    string_set = PyList_GET_ITEM(state->pattern->ref_lists, index);
    if (!string_set)
        goto error;

    status = 0;

    for (len = max_len; status == 0 && len >= min_len; len--) {
        if (state->wide) {
            RE_UCHAR* text_ptr;

            text_ptr = (RE_UCHAR*)state->text + state->text_pos;
            status = string_set_contains_ign_u(string_set, text_ptr, len);
        } else {
            RE_BCHAR* text_ptr;

            text_ptr = (RE_BCHAR*)state->text + state->text_pos;
            status = string_set_contains_ign_b(state->encoding, string_set,
              text_ptr, len);
        }

        if (status == 1)
            state->text_pos += len;
    }

    release_GIL(safe_state);

    return status == 1;

error:
    release_GIL(safe_state);
    return FALSE;
}

Py_LOCAL_INLINE(BOOL) string_set_match_ign_rev(RE_SafeState* safe_state,
  RE_Node* node) {
    Py_ssize_t index;
    Py_ssize_t min_len;
    Py_ssize_t max_len;
    RE_State* state;
    Py_ssize_t available;
    PyObject* string_set;
    int status;
    Py_ssize_t len;

    index = node->values[0];
    min_len = node->values[1];
    max_len = node->values[2];

    state = safe_state->re_state;

    available = state->text_pos - state->slice_start;
    if (available < min_len)
        return FALSE;

    if (max_len > available)
        max_len = available;

    acquire_GIL(safe_state);

    string_set = PyList_GET_ITEM(state->pattern->ref_lists, index);
    if (!string_set)
        goto error;

    status = 0;

    for (len = max_len; status == 0 && len >= min_len; len--) {
        if (state->wide) {
            RE_UCHAR* text_ptr;

            text_ptr = (RE_UCHAR*)state->text + state->text_pos - len;
            status = string_set_contains_ign_u(string_set, text_ptr, len);
        } else {
            RE_BCHAR* text_ptr;

            text_ptr = (RE_BCHAR*)state->text + state->text_pos - len;
            status = string_set_contains_ign_b(state->encoding, string_set,
              text_ptr, len);
        }

        if (status == 1)
            state->text_pos -= len;
    }

    release_GIL(safe_state);

    return status == 1;

error:
    release_GIL(safe_state);
    return FALSE;
}

Py_LOCAL_INLINE(BOOL) string_set_match_rev(RE_SafeState* safe_state, RE_Node*
  node) {
    Py_ssize_t index;
    Py_ssize_t min_len;
    Py_ssize_t max_len;
    RE_State* state;
    Py_ssize_t available;
    PyObject* string_set;
    int status;
    Py_ssize_t len;

    index = node->values[0];
    min_len = node->values[1];
    max_len = node->values[2];

    state = safe_state->re_state;

    available = state->text_pos - state->slice_start;
    if (available < min_len)
        return FALSE;

    if (max_len > available)
        max_len = available;

    acquire_GIL(safe_state);

    string_set = PyList_GET_ITEM(state->pattern->ref_lists, index);
    if (!string_set)
        goto error;

    status = 0;

    for (len = max_len; status == 0 && len >= min_len; len--) {
        PyObject* string;

        if (state->wide) {
            RE_UCHAR* text_ptr;

            text_ptr = (RE_UCHAR*)state->text + state->text_pos - len;
            string = Py_BuildValue("u#", text_ptr, len);
        } else {
            RE_BCHAR* text_ptr;

            text_ptr = (RE_BCHAR*)state->text + state->text_pos - len;
            string = Py_BuildValue("y#", text_ptr, len);
        }
        if (!string)
            goto error;

        status = PySet_Contains(string_set, string);
        Py_DECREF(string);

        if (status == 1)
            state->text_pos -= len;
    }

    release_GIL(safe_state);

    return status == 1;

error:
    release_GIL(safe_state);
    return FALSE;
}

/* Performs a depth-first match or search from the context. */
Py_LOCAL_INLINE(int) basic_match(RE_SafeState* safe_state, RE_Node* start_node,
  BOOL search) {
    RE_State* state;
    Py_ssize_t slice_start;
    Py_ssize_t slice_end;
    Py_ssize_t text_pos;
    RE_EncodingTable* encoding;
    BOOL (*same_char_ign)(RE_CODE ch1, RE_CODE ch2);
    PatternObject* pattern;
    Py_ssize_t text_length;
    RE_NextNode start_pair;
    size_t iterations;
    void* text;
    RE_CODE (*char_at)(void* text, Py_ssize_t pos);
    BOOL (*has_property)(RE_CODE property, RE_CODE ch);
    RE_GroupInfo* group_info;
    Py_ssize_t step;
    BOOL has_groups;
    RE_Node* node;
    TRACE(("<<basic_match>>\n"))

    state = safe_state->re_state;

    slice_start = state->slice_start;
    slice_end = state->slice_end;
    text_pos = state->text_pos;

    encoding = state->encoding;
    same_char_ign = encoding->same_char_ign;

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
    text = state->text;
    char_at = state->char_at;
    has_property = encoding->has_property;
    group_info = pattern->group_info;
    step = state->reverse ? -1 : 1;
    has_groups = pattern->group_count > 0;

    /* Save the groups in case we need to restore them for searching. */
    if (has_groups && !push_groups(safe_state))
        return RE_ERROR_MEMORY;

    /* Add a backtrack entry for failure. */
    if (!add_backtrack(safe_state, RE_OP_FAILURE))
        return RE_ERROR_MEMORY;

start_match:
    /* If we're searching, advance along the string until there could be a
     * match.
     */
    if (search) {
        RE_Position new_position;

next_match:
        if (!search_start(safe_state, &start_pair, &new_position))
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

        /* Should we abort the matching? */
        ++iterations;
        if ((iterations & 0xFFFF) == 0 && safe_check_signals(safe_state))
            return RE_ERROR_INTERRUPTED;

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
        case RE_OP_ANY_U: /* Any character, except a line separator. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos >= slice_end ||
              state->encoding->is_line_sep(char_at(text, text_pos)))
                goto backtrack;
            ++text_pos;
            node = node->next_1.node;
            break;
        case RE_OP_ANY_U_REV: /* Any character, except a line separator. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos <= slice_start ||
              state->encoding->is_line_sep(char_at(text, text_pos - 1) == '\n'))
                goto backtrack;
            --text_pos;
            node = node->next_1.node;
            break;
        case RE_OP_ATOMIC: /* Atomic subpattern. */
        {
            RE_Info info;
            int status;
            TRACE(("%s\n", re_op_text[node->op]))

            /* Try to match the subpattern. */
            if (node->values[0]) {
                /* The atomic group contains captures, so save them. */
                if (!add_backtrack(safe_state, RE_OP_ATOMIC))
                    return RE_ERROR_MEMORY;
                if (!save_all_captures(safe_state, state->backtrack))
                    return RE_ERROR_MEMORY;
            }

            save_info(state, &info);
            state->text_pos = text_pos;
            state->must_advance = FALSE;

            status = basic_match(safe_state, node->nonstring.next_2.node,
              FALSE);
            if (status < 0)
                return status;

            reset_guards(state, node->values);

            restore_info(state, &info);

            if (status != RE_ERROR_SUCCESS)
                goto backtrack;

            node = node->next_1.node;
            text_pos = state->text_pos;
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
            if (encoding->at_boundary(state, text_pos) != node->match)
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
            try_second = try_match(state, &node->nonstring.next_2, text_pos,
              &next_second_position);
            if (try_first) {
                if (try_second) {
                    if (!add_backtrack(safe_state, RE_OP_BRANCH) || has_groups
                      && !push_groups(safe_state))
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
            if (encoding->at_default_boundary(state, text_pos) != node->match)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_END_GREEDY_REPEAT: /* End of a greedy repeat. */
        {
            size_t index;
            RE_RepeatData* rp_data;
            BOOL try_body;
            RE_Position next_body_position;
            BOOL try_tail;
            RE_Position next_tail_position;
            RE_BacktrackData* bt_data;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Repeat indexes are 0-based. */
            index = node->values[0];
            rp_data = &state->repeats[index];

            /* Matched the body again. */
            ++rp_data->count;
            /* The counts are of type size_t, so the format needs to specify
             * that.
             */
            TRACE(("min is %" PY_FORMAT_SIZE_T "u, max is %" PY_FORMAT_SIZE_T
              "u, count is %" PY_FORMAT_SIZE_T "u\n", node->values[1],
              node->values[2], rp_data->count))

            /* Could the body or tail match? */
            try_body = rp_data->count < rp_data->max_count &&
              !is_guarded(state, index, text_pos, RE_STATUS_BODY) &&
              try_match(state, &node->next_1, text_pos, &next_body_position);
            try_tail = rp_data->count >= node->values[1] && !is_guarded(state,
              index, text_pos, RE_STATUS_TAIL) && try_match(state,
              &node->nonstring.next_2, text_pos, &next_tail_position);
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

                    /* Record backtracking info for matching the tail. */
                    if (!add_backtrack(safe_state, RE_OP_END_GREEDY_REPEAT) ||
                      has_groups && !push_groups(safe_state))
                        return RE_ERROR_MEMORY;
                    bt_data = state->backtrack;
                    bt_data->repeat.position = next_tail_position;
                    bt_data->repeat.index = index;
                    bt_data->repeat.count = rp_data->count;
                    bt_data->repeat.max_count = rp_data->max_count;
                    bt_data->repeat.text_pos = text_pos;
                } else {
                    /* Only the body could match. If the body fails to match
                     * then we want to backtrack into the body.
                     */

                    /* Record backtracking info for backtracking into the body.
                     */
                    bt_data = last_backtrack(state);
                    if (bt_data->op == RE_OP_END_GREEDY_REPEAT &&
                      !bt_data->repeat.position.node && bt_data->repeat.index
                      == index) {
                        /* The last backtrack entry is for backtracking into
                         * the body like we want to do now, so we can save work
                         * by just re-using it.
                         */
                    } else {
                        if (!add_backtrack(safe_state,
                          RE_OP_END_GREEDY_REPEAT))
                            return RE_ERROR_MEMORY;
                        bt_data = state->backtrack;
                        bt_data->repeat.position.node = NULL; /* Restore then backtrack. */
                        bt_data->repeat.index = index;
                    }
                    bt_data->repeat.count = rp_data->count - 1;
                    bt_data->repeat.max_count = rp_data->max_count;
                }

                /* Advance into the body. */
                if (!guard(safe_state, index, text_pos, RE_STATUS_BODY))
                    return RE_ERROR_MEMORY;

                node = next_body_position.node;
                text_pos = next_body_position.text_pos;
            } else {
                /* Only the tail could match. If the tail fails to match then
                 * we want to backtrack into the body.
                 */

                /* Record backtracking info for backtracking into the body. */
                bt_data = last_backtrack(state);
                if (bt_data->op == RE_OP_END_GREEDY_REPEAT &&
                  !bt_data->repeat.position.node && bt_data->repeat.index ==
                  index) {
                    /* The last backtrack entry is for backtracking into the
                     * body like we want to do now, so we can save work by just
                     * re-using it.
                     */
                } else {
                    if (!add_backtrack(safe_state, RE_OP_END_GREEDY_REPEAT))
                        return RE_ERROR_MEMORY;
                    bt_data = state->backtrack;
                    bt_data->repeat.position.node = NULL; /* Restore then backtrack. */
                    bt_data->repeat.index = index;
                }
                bt_data->repeat.count = rp_data->count - 1;
                bt_data->repeat.max_count = rp_data->max_count;

                /* Advance into the tail. */
                if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                    return RE_ERROR_MEMORY;

                node = next_tail_position.node;
                text_pos = next_tail_position.text_pos;
            }
            break;
        }
        case RE_OP_END_GROUP: /* End of a capture group. */
        {
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Capture group indexes are 1-based (excluding group 0, which is
             * the entire matched string).
             */
            state->groups[node->values[0] - 1].span.end = text_pos;

            /* Save the capture? */
            if (node->values[1] && state->save_captures) {
                RE_BacktrackData* bt_data;

                if (!save_capture(safe_state, node->values[0]))
                    return RE_ERROR_MEMORY;

                /* Record backtracking info for unsaving the capture. */
                if (!add_backtrack(safe_state, RE_OP_END_GROUP))
                    return RE_ERROR_MEMORY;
                bt_data = state->backtrack;
                bt_data->group.index = node->values[0];
            }
            node = node->next_1.node;
            break;
        }
        case RE_OP_END_LAZY_REPEAT: /* End of a lazy repeat. */
        {
            size_t index;
            RE_RepeatData* rp_data;
            BOOL try_body;
            RE_Position next_body_position;
            BOOL try_tail;
            RE_Position next_tail_position;
            RE_BacktrackData* bt_data;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Repeat indexes are 0-based. */
            index = node->values[0];
            rp_data = &state->repeats[index];

            /* Matched the body again. */
            ++rp_data->count;
            TRACE(("min is %d, max is %d, count is %d\n", node->values[1],
              node->values[2], rp_data->count))

            /* Could the body or tail match? */
            try_body = rp_data->count < rp_data->max_count &&
              !is_guarded(state, index, text_pos, RE_STATUS_BODY) &&
              try_match(state, &node->next_1, text_pos, &next_body_position);
            try_tail = rp_data->count >= node->values[1] && !is_guarded(state,
              index, text_pos, RE_STATUS_TAIL) && try_match(state,
              &node->nonstring.next_2, text_pos, &next_tail_position);
            if (!try_body && !try_tail) {
                /* Neither the body nor the tail could match. */
                --rp_data->count;
                goto backtrack;
            }

            if (try_body) {
                if (try_tail) {
                    /* Both the body and the tail could match, but the tail
                     * takes preference. If the tail fails to match then we
                     * want to try the body again before backtracking into the
                     * head.
                     */

                    /* Record backtracking info for matching the body. */
                    if (!add_backtrack(safe_state, RE_OP_END_LAZY_REPEAT) ||
                      has_groups && !push_groups(safe_state))
                        return RE_ERROR_MEMORY;
                    bt_data = state->backtrack;
                    bt_data->repeat.position = next_body_position;
                    bt_data->repeat.index = index;
                    bt_data->repeat.count = rp_data->count;
                    bt_data->repeat.max_count = rp_data->max_count;
                    bt_data->repeat.text_pos = text_pos;

                    /* Advance into the tail. */
                    if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                        return RE_ERROR_MEMORY;

                    node = next_tail_position.node;
                    text_pos = next_tail_position.text_pos;
                } else {
                    /* Only the body could match. */

                    /* Record backtracking info for backtracking into the body.
                     */
                    bt_data = last_backtrack(state);
                    if (bt_data->op == RE_OP_END_LAZY_REPEAT &&
                      !bt_data->repeat.position.node && bt_data->repeat.index
                      == index) {
                        /* The last backtrack entry is for backtracking into
                         * the body like we want to do now, so we can save work
                         * by just re-using it.
                         */
                    } else {
                        if (!add_backtrack(safe_state, RE_OP_END_LAZY_REPEAT))
                            return RE_ERROR_MEMORY;
                        bt_data = state->backtrack;
                        bt_data->repeat.position.node = NULL; /* Restore then backtrack. */
                        bt_data->repeat.index = index;
                    }
                    bt_data->repeat.count = rp_data->count - 1;
                    bt_data->repeat.max_count = rp_data->max_count;

                    /* Advance into the body. */
                    if (!guard(safe_state, index, text_pos, RE_STATUS_BODY))
                        return RE_ERROR_MEMORY;

                    node = next_body_position.node;
                    text_pos = next_body_position.text_pos;
                }
            } else {
                /* Only the tail could match. */

                /* Record backtracking info for backtracking into the body. */
                bt_data = last_backtrack(state);
                if (bt_data->op == RE_OP_END_LAZY_REPEAT &&
                  !bt_data->repeat.position.node && bt_data->repeat.index ==
                  index) {
                    /* The last backtrack entry is for backtracking into the
                     * body like we want to do now, so we can save work by just
                     * re-using it.
                     */
                } else {
                    if (!add_backtrack(safe_state, RE_OP_END_LAZY_REPEAT))
                        return RE_ERROR_MEMORY;
                    bt_data = state->backtrack;
                    bt_data->repeat.position.node = NULL; /* Restore then backtrack. */
                    bt_data->repeat.index = index;
                }
                bt_data->repeat.count = rp_data->count - 1;
                bt_data->repeat.max_count = rp_data->max_count;

                /* Advance into the tail. */
                if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                    return RE_ERROR_MEMORY;

                node = next_tail_position.node;
                text_pos = next_tail_position.text_pos;
            }
            break;
        }
        case RE_OP_END_OF_LINE: /* At the end of a line. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos != text_length && char_at(text, text_pos) != '\n')
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_END_OF_LINE_U: /* At the end of a line. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (!encoding->at_line_end(state, text_pos))
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
            if (text_pos != text_length && text_pos != state->final_newline)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_END_OF_STRING_LINE_U: /* At end of string or final newline. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos != text_length && text_pos != state->final_line_sep)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_GRAPHEME_BOUNDARY: /* At a grapheme boundary. */
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (encoding->at_grapheme_boundary(state, text_pos) != node->match)
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_GREEDY_REPEAT: /* Greedy repeat. */
        {
            size_t index;
            RE_RepeatData* rp_data;
            RE_BacktrackData* bt_data;
            size_t max_count;
            BOOL try_body;
            RE_Position next_body_position;
            BOOL try_tail;
            RE_Position next_tail_position;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Repeat indexes are 0-based. */
            index = node->values[0];
            rp_data = &state->repeats[index];

            /* We might need to backtrack into the head. */
            if (!add_backtrack(safe_state, RE_OP_GREEDY_REPEAT) || has_groups
              && !push_groups(safe_state))
                return RE_ERROR_MEMORY;
            bt_data = state->backtrack;
            bt_data->repeat.index = index;
            bt_data->repeat.count = rp_data->count;
            bt_data->repeat.max_count = rp_data->max_count;

            max_count = node->values[3] ? slice_end - text_pos : text_pos -
              slice_start;
            if (max_count > node->values[2])
                max_count = node->values[2];

            rp_data->count = 0;
            rp_data->max_count = max_count;

            /* Could the body or tail match? */
            try_body = !is_guarded(state, index, text_pos, RE_STATUS_BODY) &&
              try_match(state, &node->next_1, text_pos, &next_body_position);
            try_tail = node->values[1] == 0 && !is_guarded(state, index,
              text_pos, RE_STATUS_TAIL) && try_match(state,
              &node->nonstring.next_2, text_pos, &next_tail_position);
            if (!try_body && !try_tail)
                /* Neither the body nor the tail could match. */
                goto backtrack;

            if (try_body) {
                if (try_tail) {
                    /* The body doesn't have to match, but it takes precedence
                     * over the tail. If the body fails to match then we want
                     * to try the tail before backtracking into the head.
                     */

                    /* Record backtracking info for matching the tail. */
                    if (!add_backtrack(safe_state, RE_OP_END_GREEDY_REPEAT) ||
                      has_groups && !push_groups(safe_state))
                        return RE_ERROR_MEMORY;
                    bt_data = state->backtrack;
                    bt_data->repeat.position = next_tail_position;
                    bt_data->repeat.index = index;
                    bt_data->repeat.count = rp_data->count;
                    bt_data->repeat.max_count = rp_data->max_count;
                    bt_data->repeat.text_pos = text_pos;
                }

                /* Advance into the body. */
                if (!guard(safe_state, index, text_pos, RE_STATUS_BODY))
                    return RE_ERROR_MEMORY;

                node = next_body_position.node;
                text_pos = next_body_position.text_pos;
            } else {
                /* Advance into the tail. */
                if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                    return RE_ERROR_MEMORY;

                node = next_tail_position.node;
                text_pos = next_tail_position.text_pos;
            }
            break;
        }
        case RE_OP_GREEDY_REPEAT_ONE: /* Greedy repeat for one character. */
        {
            size_t index;
            size_t count;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            index = node->values[0];
            if (is_guarded(state, index, text_pos, RE_STATUS_BODY))
                goto backtrack;

            if (!guard(safe_state, index, text_pos, RE_STATUS_BODY))
                return RE_ERROR_MEMORY;

            /* Count how many times the character repeats, up to the maximum.
             */
            count = count_one(state, node->nonstring.next_2.node, text_pos,
              node->values[2]);

            /* Have we matched at least the minimum? */
            if (count < node->values[1])
                goto backtrack;

            if (count > node->values[1]) {
                /* The match is longer than the minimum, so we might need to
                 * backtrack the repeat to consume less.
                 */
                RE_RepeatData* rp_data;
                RE_BacktrackData* bt_data;

                /* Get the offset to the repeat values in the context. Repeat
                 * indexes are 0-based.
                 */
                rp_data = &state->repeats[index];
                if (!add_backtrack(safe_state, RE_OP_GREEDY_REPEAT_ONE) ||
                  has_groups && !push_groups(safe_state))
                    return RE_ERROR_MEMORY;
                bt_data = state->backtrack;
                bt_data->repeat.position.node = node;
                bt_data->repeat.index = index;
                bt_data->repeat.text_pos = rp_data->start;
                bt_data->repeat.count = rp_data->count;

                rp_data->start = text_pos;
                rp_data->count = count;
            }

            text_pos += (Py_ssize_t)count * node->step;
            node = node->next_1.node;
            if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                return RE_ERROR_MEMORY;
            break;
        }
        case RE_OP_GROUP_EXISTS: /* Capture group exists. */
        {
            RE_GroupSpan* span;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Capture group indexes are 1-based (excluding group 0, which is
             * the entire matched string).
             *
             * Check whether the captured text, if any, exists at this position
             * in the string.
             */
            span = &state->groups[node->values[0] - 1].span;
            if (0 <= span->start && span->start <= span->end && span->end <=
              text_length)
                node = node->next_1.node;
            else
                node = node->nonstring.next_2.node;
            break;
        }
        case RE_OP_LAZY_REPEAT: /* Lazy repeat. */
        {
            size_t index;
            RE_RepeatData* rp_data;
            RE_BacktrackData* bt_data;
            size_t max_count;
            BOOL try_body;
            RE_Position next_body_position;
            BOOL try_tail;
            RE_Position next_tail_position;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Repeat indexes are 0-based. */
            index = node->values[0];
            rp_data = &state->repeats[index];

            /* We might need to backtrack into the head. */
            if (!add_backtrack(safe_state, RE_OP_LAZY_REPEAT) || has_groups &&
              !push_groups(safe_state))
                return RE_ERROR_MEMORY;
            bt_data = state->backtrack;
            bt_data->repeat.index = index;
            bt_data->repeat.count = rp_data->count;
            bt_data->repeat.max_count = rp_data->max_count;

            max_count = node->values[3] ? slice_end - text_pos : text_pos -
              slice_start;
            if (max_count > node->values[2])
                max_count = node->values[2];

            rp_data->count = 0;
            rp_data->max_count = max_count;

            /* Could the body or tail match? */
            try_body = !is_guarded(state, index, text_pos, RE_STATUS_BODY) &&
              try_match(state, &node->next_1, text_pos, &next_body_position);
            try_tail = node->values[1] == 0 && !is_guarded(state, index,
              text_pos, RE_STATUS_TAIL) && try_match(state,
              &node->nonstring.next_2, text_pos, &next_tail_position);
            if (!try_body && !try_tail)
                /* Neither the body nor the tail could match. */
                goto backtrack;

            if (try_body) {
                if (try_tail) {
                    /* The body doesn't have to match, but the tail takes
                     * precedence over it. If the tail fails to match then we
                     * want to try the body again before backtracking into the
                     * head.
                     */

                    /* Record backtracking info for matching the body. */
                    if (!add_backtrack(safe_state, RE_OP_END_LAZY_REPEAT) ||
                      has_groups && !push_groups(safe_state))
                        return RE_ERROR_MEMORY;
                    bt_data = state->backtrack;
                    bt_data->repeat.index = index;
                    bt_data->repeat.position.node = node->next_1.node;
                    bt_data->repeat.position.text_pos = text_pos;
                    bt_data->repeat.count = rp_data->count;
                    bt_data->repeat.max_count = rp_data->max_count;
                    bt_data->repeat.text_pos = text_pos;

                    /* Advance into the tail. */
                    if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                        return RE_ERROR_MEMORY;

                    node = next_tail_position.node;
                    text_pos = next_tail_position.text_pos;
                } else {
                    /* Advance into the body. */
                    if (!guard(safe_state, index, text_pos, RE_STATUS_BODY))
                        return RE_ERROR_MEMORY;

                    node = next_body_position.node;
                    text_pos = next_body_position.text_pos;
                }
            } else {
                /* Advance into the tail. */
                if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                    return RE_ERROR_MEMORY;

                node = next_tail_position.node;
                text_pos = next_tail_position.text_pos;
            }
            break;
        }
        case RE_OP_LAZY_REPEAT_ONE: /* Lazy repeat for one character. */
        {
            size_t index;
            size_t count;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            index = node->values[0];
            if (is_guarded(state, index, text_pos, RE_STATUS_BODY))
                goto backtrack;

            if (!guard(safe_state, index, text_pos, RE_STATUS_BODY))
                return RE_ERROR_MEMORY;

            /* Count how many times the character repeats, up to the minimum.
             */
            count = count_one(state, node->nonstring.next_2.node, text_pos,
              node->values[1]);

            /* Have we matched at least the minimum? */
            if (count < node->values[1])
                goto backtrack;

            index = node->values[0];
            if (count < node->values[2]) {
                /* The match is shorter than the maximum, so we might need to
                 * backtrack the repeat to consume more.
                 */
                RE_RepeatData* rp_data;
                RE_BacktrackData* bt_data;

                /* Get the offset to the repeat values in the context. Repeat
                 * indexes are 0-based.
                 */
                rp_data = &state->repeats[index];
                if (!add_backtrack(safe_state, RE_OP_LAZY_REPEAT_ONE) ||
                  has_groups && !push_groups(safe_state))
                    return RE_ERROR_MEMORY;
                bt_data = state->backtrack;
                bt_data->repeat.position.node = node;
                bt_data->repeat.index = index;
                bt_data->repeat.text_pos = rp_data->start;
                bt_data->repeat.count = rp_data->count;

                rp_data->start = text_pos;
                rp_data->count = count;
            }

            text_pos += (Py_ssize_t)count * step;
            node = node->next_1.node;
            if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                return RE_ERROR_MEMORY;
            break;
        }
        case RE_OP_LOOKAROUND: /* Lookaround. */
        {
            RE_Info info;
            int status;
            BOOL matched;
            TRACE(("%s %d\n", re_op_text[node->op], node->match))

            /* Try to match the subpattern. */
            if (node->values[0]) {
                /* The lookaround contains captures, so save them. */
                if (!add_backtrack(safe_state, RE_OP_LOOKAROUND))
                    return RE_ERROR_MEMORY;
                if (!save_all_captures(safe_state, state->backtrack))
                    return RE_ERROR_MEMORY;
            }

            save_info(state, &info);
            state->slice_start = 0;
            state->slice_end = text_length;
            state->text_pos = text_pos;
            state->must_advance = FALSE;

            status = basic_match(safe_state, node->nonstring.next_2.node,
              FALSE);
            if (status < 0)
                return status;

            reset_guards(state, node->values + 1);

            state->slice_end = slice_end;
            state->slice_start = slice_start;
            restore_info(state, &info);

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
        case RE_OP_REF_GROUP_IGN: /* Reference to a capture group, ignoring case. */
        {
            RE_GroupSpan* span;
            size_t length;
            size_t available;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Capture group indexes are 1-based (excluding group 0, which is
             * the entire matched string).
             *
             * Check whether the captured text, if any, exists at this position
             * in the string.
             */
            span = &state->groups[node->values[0] - 1].span;
            if (!(0 <= span->start && span->start <= span->end && span->end <=
              text_length))
                goto backtrack;

            length = span->end - span->start;
            available = slice_end - text_pos;
            if (length > available)
                goto backtrack;

            if (state->wide) {
                RE_UCHAR* text_ptr;
                RE_UCHAR* group_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;
                group_ptr = (RE_UCHAR*)text + span->start;

                if (node->op == RE_OP_REF_GROUP_IGN) {
                    for (i = 0; i < length; i++) {
                        if (!same_char_ign(text_ptr[i], group_ptr[i]))
                            goto backtrack;
                    }
                } else {
                    for (i = 0; i < length; i++) {
                        if (text_ptr[i] != group_ptr[i])
                            goto backtrack;
                    }
                }
            } else {
                RE_BCHAR* text_ptr;
                RE_BCHAR* group_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;
                group_ptr = (RE_BCHAR*)text + span->start;

                if (node->op == RE_OP_REF_GROUP_IGN) {
                    for (i = 0; i < length; i++) {
                        if (!same_char_ign(text_ptr[i], group_ptr[i]))
                            goto backtrack;
                    }
                } else {
                    for (i = 0; i < length; i++) {
                        if (text_ptr[i] != group_ptr[i])
                            goto backtrack;
                    }
                }
            }
            text_pos += length;

            node = node->next_1.node;
            break;
        }
        case RE_OP_REF_GROUP_IGN_REV: /* Reference to a capture group, ignoring case. */
        case RE_OP_REF_GROUP_REV: /* Reference to a capture group. */
        {
            RE_GroupSpan* span;
            size_t length;
            size_t available;
            size_t i;
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Capture group indexes are 1-based (excluding group 0, which is
             * the entire matched string).
             *
             * Check whether the captured text, if any, exists at this position
             * in the string.
             */
            span = &state->groups[node->values[0] - 1].span;
            if (!(0 <= span->start && span->start <= span->end && span->end <=
              text_length))
                goto backtrack;

            length = span->end - span->start;
            available = text_pos - slice_start;
            if (length > available)
                goto backtrack;

            text_pos -= length;

            if (state->wide) {
                RE_UCHAR* text_ptr;
                RE_UCHAR* group_ptr;

                text_ptr = (RE_UCHAR*)text + text_pos;
                group_ptr = (RE_UCHAR*)text + span->start;

                if (node->op == RE_OP_REF_GROUP_IGN_REV) {
                    for (i = 0; i < length; i++) {
                        if (!same_char_ign(text_ptr[i], group_ptr[i]))
                            goto backtrack;
                    }
                } else {
                    for (i = 0; i < length; i++) {
                        if (text_ptr[i] != group_ptr[i])
                            goto backtrack;
                    }
                }
            } else {
                RE_BCHAR* text_ptr;
                RE_BCHAR* group_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;
                group_ptr = (RE_BCHAR*)text + span->start;

                if (node->op == RE_OP_REF_GROUP_IGN_REV) {
                    for (i = 0; i < length; i++) {
                        if (!same_char_ign(text_ptr[i], group_ptr[i]))
                            goto backtrack;
                    }
                } else {
                    for (i = 0; i < length; i++) {
                        if (text_ptr[i] != group_ptr[i])
                            goto backtrack;
                    }
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
        case RE_OP_SET_DIFF: /* Character set. */
        case RE_OP_SET_INTER:
        case RE_OP_SET_SYM_DIFF:
        case RE_OP_SET_UNION:
            TRACE(("%s %d\n", re_op_text[node->op], node->match))
            if (text_pos >= slice_end || !in_set(encoding, node, char_at(text,
              text_pos)))
                goto backtrack;
            text_pos += node->step;
            node = node->next_1.node;
            break;
        case RE_OP_SET_DIFF_REV: /* Character set. */
        case RE_OP_SET_INTER_REV:
        case RE_OP_SET_SYM_DIFF_REV:
        case RE_OP_SET_UNION_REV:
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
        case RE_OP_START_GROUP: /* Start of capture group. */
        {
            TRACE(("%s %d\n", re_op_text[node->op], node->values[0]))

            /* Capture group indexes are 1-based (excluding group 0, which is
             * the entire matched string).
             */
            state->groups[node->values[0] - 1].span.start = text_pos;

            /* Save the capture? */
            if (node->values[1] && state->save_captures) {
                RE_BacktrackData* bt_data;

                if (!save_capture(safe_state, node->values[0]))
                    return RE_ERROR_MEMORY;

                /* Record backtracking info for unsaving the capture. */
                if (!add_backtrack(safe_state, RE_OP_START_GROUP))
                    return RE_ERROR_MEMORY;
                bt_data = state->backtrack;
                bt_data->group.index = node->values[0];
            }
            node = node->next_1.node;
            break;
        }
        case RE_OP_START_OF_LINE: /* At the start of a line. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (text_pos != 0 && char_at(text, text_pos - 1) != '\n')
                goto backtrack;
            node = node->next_1.node;
            break;
        case RE_OP_START_OF_LINE_U: /* At the start of a line. */
            TRACE(("%s\n", re_op_text[node->op]))
            if (!encoding->at_line_start(state, text_pos))
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

                if (node->op == RE_OP_STRING_IGN) {
                    for (i = 0; i < length; i++) {
                        if (!same_char_ign(text_ptr[i], values[i]))
                            goto backtrack;
                    }
                } else {
                    for (i = 0; i < length; i++) {
                        if (text_ptr[i] != values[i])
                            goto backtrack;
                    }
                }
            } else {
                RE_BCHAR* text_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;

                if (node->op == RE_OP_STRING_IGN) {
                    for (i = 0; i < length; i++) {
                        if (!same_char_ign(text_ptr[i], values[i]))
                            goto backtrack;
                    }
                } else {
                    for (i = 0; i < length; i++) {
                        if (text_ptr[i] != values[i])
                            goto backtrack;
                    }
                }
            }
            text_pos += length;
            node = node->next_1.node;
            break;
        }
        case RE_OP_STRING_IGN_REV: /* A string literal, ignoring case. */
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

                if (node->op == RE_OP_STRING_IGN_REV) {
                    for (i = 0; i < length; i++) {
                        if (!same_char_ign(text_ptr[i], values[i]))
                            goto backtrack;
                    }
                } else {
                    for (i = 0; i < length; i++) {
                        if (text_ptr[i] != values[i])
                            goto backtrack;
                    }
                }
            } else {
                RE_BCHAR* text_ptr;

                text_ptr = (RE_BCHAR*)text + text_pos;

                if (node->op == RE_OP_STRING_IGN_REV) {
                    for (i = 0; i < length; i++) {
                        if (!same_char_ign(text_ptr[i], values[i]))
                            goto backtrack;
                    }
                } else {
                    for (i = 0; i < length; i++) {
                        if (text_ptr[i] != values[i])
                            goto backtrack;
                    }
                }
            }
            node = node->next_1.node;
            break;
        }
        case RE_OP_STRING_SET: /* Member of a string set. */
            TRACE(("%s\n", re_op_text[node->op]))
            state->text_pos = text_pos;
            if (!string_set_match(safe_state, node))
                goto backtrack;
            text_pos = state->text_pos;
            node = node->next_1.node;
            break;
        case RE_OP_STRING_SET_IGN: /* Member of a string set, ignoring case. */
            TRACE(("%s\n", re_op_text[node->op]))
            state->text_pos = text_pos;
            if (!string_set_match_ign(safe_state, node))
                goto backtrack;
            text_pos = state->text_pos;
            node = node->next_1.node;
            break;
        case RE_OP_STRING_SET_REV: /* Member of a string set. */
            TRACE(("%s\n", re_op_text[node->op]))
            state->text_pos = text_pos;
            if (!string_set_match_rev(safe_state, node))
                goto backtrack;
            text_pos = state->text_pos;
            node = node->next_1.node;
            break;
        case RE_OP_STRING_SET_IGN_REV: /* Member of a string set, ignoring case. */
            TRACE(("%s\n", re_op_text[node->op]))
            state->text_pos = text_pos;
            if (!string_set_match_ign_rev(safe_state, node))
                goto backtrack;
            text_pos = state->text_pos;
            node = node->next_1.node;
            break;
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
    }

backtrack:
    for (;;) {
        RE_BacktrackData* bt_data;
        TRACE(("BACKTRACK "))

        bt_data = last_backtrack(state);

        switch (bt_data->op) {
        case RE_OP_ATOMIC: /* Atomic subpattern. */
        {
            TRACE(("%s\n", re_op_text[bt_data->op]))

            /* Restore the captures and backtrack. */
            restore_all_captures(state, bt_data);
            discard_backtrack(state);
            break;
        }
        case RE_OP_BRANCH: /* 2-way branch. */
        {
            TRACE(("%s\n", re_op_text[bt_data->op]))
            if (has_groups)
                pop_groups(state);
            node = bt_data->branch.position.node;
            text_pos = bt_data->branch.position.text_pos;
            discard_backtrack(state);
            goto advance;
        }
        case RE_OP_END_GROUP: /* End of capture group. */
        case RE_OP_START_GROUP: /* Start of capture group. */
        {
            TRACE(("%s %d\n", re_op_text[bt_data->op], bt_data->group.index))

            /* Unsave the capture and backtrack. */
            unsave_capture(state, bt_data->group.index);
            discard_backtrack(state);
            break;
        }
        case RE_OP_END_GREEDY_REPEAT: /* End of a greedy repeat. */
        {
            RE_RepeatData* rp_data;
            TRACE(("%s\n", re_op_text[bt_data->op]))

            /* Restore the count. */
            rp_data = &state->repeats[bt_data->repeat.index];
            rp_data->count = bt_data->repeat.count;
            rp_data->max_count = bt_data->repeat.max_count;
            if (bt_data->repeat.position.node) {
                /* Advance into the tail. */
                if (!guard(safe_state, bt_data->repeat.index,
                  bt_data->repeat.text_pos, RE_STATUS_TAIL))
                    return RE_ERROR_MEMORY;

                node = bt_data->repeat.position.node;
                text_pos = bt_data->repeat.position.text_pos;

                if (has_groups)
                    pop_groups(state);

                if (rp_data->count > 0) {
                    /* Restore then backtrack into body next time. */
                    --bt_data->repeat.count;
                    bt_data->repeat.position.node = NULL;
                } else
                    discard_backtrack(state);
                goto advance;
            } else
                discard_backtrack(state);
            break;
        }
        case RE_OP_END_LAZY_REPEAT: /* End of a lazy repeat. */
        {
            RE_RepeatData* rp_data;
            TRACE(("%s\n", re_op_text[bt_data->op]))

            /* Restore the count. */
            rp_data = &state->repeats[bt_data->repeat.index];
            rp_data->count = bt_data->repeat.count;
            rp_data->max_count = bt_data->repeat.max_count;
            if (bt_data->repeat.position.node) {
                /* Advance into the body. */
                if (!guard(safe_state, bt_data->repeat.index,
                  bt_data->repeat.text_pos, RE_STATUS_BODY))
                    return RE_ERROR_MEMORY;

                node = bt_data->repeat.position.node;
                text_pos = bt_data->repeat.position.text_pos;

                if (has_groups)
                    pop_groups(state);

                if (rp_data->count > 0) {
                    /* Restore then backtrack into body next time. */
                    --bt_data->repeat.count;
                    bt_data->repeat.position.node = NULL;
                } else
                    discard_backtrack(state);
                goto advance;
            } else
                discard_backtrack(state);
            break;
        }
        case RE_OP_FAILURE:
        {
            Py_ssize_t end_pos;
            TRACE(("%s\n", re_op_text[bt_data->op]))

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

                /* How many characters did the repeat actually match? */
                count = count_one(state, start_node->nonstring.next_2.node,
                  text_pos, start_node->values[2]);

                /* If it's fewer than the maximum then skip over those
                 * characters.
                 */
                if (count < start_node->values[2])
                    text_pos += (Py_ssize_t)count * step;
                break;
            }
            }

            /* Advance and try to match again. */
            state->text_pos = text_pos + step;
            if (has_groups)
                reload_groups(state);

            goto start_match;
        }
        case RE_OP_GREEDY_REPEAT: /* Greedy repeat. */
        {
            RE_RepeatData* rp_data;
            TRACE(("%s\n", re_op_text[bt_data->op]))

            if (has_groups)
                pop_groups(state);
            rp_data = &state->repeats[bt_data->repeat.index];
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
            size_t index;
            TRACE(("%s\n", re_op_text[bt_data->op]))

            node = bt_data->repeat.position.node;

            rp_data = &state->repeats[bt_data->repeat.index];

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
            index = node->values[0];

            switch (node->next_1.test->op) {
            case RE_OP_CHARACTER:
            {
                if (state->wide) {
                    RE_UCHAR* text_ptr;

                    text_ptr = (RE_UCHAR*)text;

                    for (;;) {
                        pos -= step;
                        match = (text_ptr[pos] == ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    RE_BCHAR* text_ptr;

                    text_ptr = (RE_BCHAR*)text;

                    for (;;) {
                        pos -= step;
                        match = (text_ptr[pos] == ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_CHARACTER_IGN:
            {
                if (state->wide) {
                    RE_UCHAR* text_ptr;

                    text_ptr = (RE_UCHAR*)text;

                    for (;;) {
                        pos -= step;
                        match = same_char_ign(text_ptr[pos], ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    RE_BCHAR* text_ptr;

                    text_ptr = (RE_BCHAR*)text;

                    for (;;) {
                        pos -= step;
                        match = same_char_ign(text_ptr[pos], ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_CHARACTER_IGN_REV:
            {
                if (state->wide) {
                    RE_UCHAR* text_ptr;

                    text_ptr = (RE_UCHAR*)text;

                    for (;;) {
                        pos -= step;
                        match = same_char_ign(text_ptr[pos - 1], ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    RE_BCHAR* text_ptr;

                    text_ptr = (RE_BCHAR*)text;

                    for (;;) {
                        pos -= step;
                        match = same_char_ign(text_ptr[pos - 1], ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_CHARACTER_REV:
            {
                if (state->wide) {
                    RE_UCHAR* text_ptr;

                    text_ptr = (RE_UCHAR*)text;

                    for (;;) {
                        pos -= step;
                        match = (text_ptr[pos - 1] == ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    RE_BCHAR* text_ptr;

                    text_ptr = (RE_BCHAR*)text;

                    for (;;) {
                        pos -= step;
                        match = (text_ptr[pos - 1] == ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING:
            {
                if (state->wide) {
                    RE_UCHAR* text_ptr;

                    text_ptr = (RE_UCHAR*)text;

                    for (;;) {
                        RE_Position next_position;

                        pos -= step;
                        match = text_ptr[pos] == ch && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    RE_BCHAR* text_ptr;

                    text_ptr = (RE_BCHAR*)text;

                    for (;;) {
                        RE_Position next_position;

                        pos -= step;
                        match = text_ptr[pos] == ch && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING_IGN:
            {
                if (state->wide) {
                    RE_UCHAR* text_ptr;

                    text_ptr = (RE_UCHAR*)text;

                    for (;;) {
                        RE_Position next_position;

                        pos -= step;
                        match = same_char_ign(text_ptr[pos], ch) &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    RE_BCHAR* text_ptr;

                    text_ptr = (RE_BCHAR*)text;

                    for (;;) {
                        RE_Position next_position;

                        pos -= step;
                        match = same_char_ign(text_ptr[pos], ch) &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING_IGN_REV:
            {
                ch = node->next_1.test->values[node->next_1.test->value_count -
                  1];
                if (state->wide) {
                    RE_UCHAR* text_ptr;

                    text_ptr = (RE_UCHAR*)text;

                    for (;;) {
                        RE_Position next_position;

                        pos -= step;
                        match = same_char_ign(text_ptr[pos - 1], ch) &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    RE_BCHAR* text_ptr;

                    text_ptr = (RE_BCHAR*)text;

                    for (;;) {
                        RE_Position next_position;

                        pos -= step;
                        match = same_char_ign(text_ptr[pos - 1], ch) &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING_REV:
            {
                ch = node->next_1.test->values[node->next_1.test->value_count -
                  1];

                if (state->wide) {
                    RE_UCHAR* text_ptr;

                    text_ptr = (RE_UCHAR*)text;

                    for (;;) {
                        RE_Position next_position;

                        pos -= step;
                        match = text_ptr[pos - 1] == ch && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                } else {
                    RE_BCHAR* text_ptr;

                    text_ptr = (RE_BCHAR*)text;

                    for (;;) {
                        RE_Position next_position;

                        pos -= step;
                        match = text_ptr[pos - 1] == ch && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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

                    pos -= step;
                    match = try_match(state, &node->next_1, pos,
                      &next_position);
                    if (match && !is_guarded(state, index, pos,
                      RE_STATUS_TAIL))
                        break;
                    if (pos == limit)
                        break;
                }
                break;
            }

            if (match) {
                count = abs_ssize_t(pos - text_pos);

                /* The tail could match. */
                if (count > node->values[1]) {
                    /* The match is longer than the minimum, so we might need
                     * to backtrack the repeat again to consume less.
                     */
                    rp_data->count = count;
                    if (has_groups)
                        reload_groups(state);
                } else {
                    /* We've reached the minimum, so we won't need to backtrack
                     * the repeat again.
                     */
                    if (has_groups)
                        pop_groups(state);
                    rp_data->start = bt_data->repeat.text_pos;
                    rp_data->count = bt_data->repeat.count;
                    discard_backtrack(state);
                }

                node = node->next_1.node;
                text_pos = pos;
                if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                    return RE_ERROR_MEMORY;
                goto advance;
            } else {
                /* We've backtracked the repeat as far as we can. */
                if (has_groups)
                    drop_groups(state);
                rp_data->start = bt_data->repeat.text_pos;
                rp_data->count = bt_data->repeat.count;
                discard_backtrack(state);
            }
            break;
        }
        case RE_OP_LAZY_REPEAT: /* Lazy repeat. */
        {
            RE_RepeatData* rp_data;
            TRACE(("%s\n", re_op_text[bt_data->op]))

            if (has_groups)
                pop_groups(state);
            rp_data = &state->repeats[bt_data->repeat.index];
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
            size_t index;
            TRACE(("%s\n", re_op_text[bt_data->op]))

            node = bt_data->repeat.position.node;

            rp_data = &state->repeats[bt_data->repeat.index];

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

            repeated = node->nonstring.next_2.node;
            test = node->next_1.test;

            ch = test->values[0];
            m = test->match;
            index = node->values[0];

            switch (test->op) {
            case RE_OP_CHARACTER:
            {
                size_t extra;

                extra = available - max_count;
                if (extra < 1)
                    limit -= (1 - (Py_ssize_t)extra) * step;

                if (state->wide) {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = (UCHAR_AT(text, pos) == ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_CHARACTER_IGN:
            {
                size_t extra;

                extra = available - max_count;
                if (extra < 1)
                    limit -= (1 - (Py_ssize_t)extra) * step;

                if (state->wide) {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign(UCHAR_AT(text, pos), ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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
                        match = same_char_ign(BCHAR_AT(text, pos), ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_CHARACTER_IGN_REV:
            {
                size_t extra;

                extra = available - max_count;
                if (extra < 1)
                    limit -= (1 - (Py_ssize_t)extra) * step;

                if (state->wide) {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign(UCHAR_AT(text, pos - 1), ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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
                        match = same_char_ign(BCHAR_AT(text, pos - 1), ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_CHARACTER_REV:
            {
                size_t extra;

                extra = available - max_count;
                if (extra < 1)
                    limit -= (1 - (Py_ssize_t)extra) * step;

                if (state->wide) {
                    for (;;) {
                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = (UCHAR_AT(text, pos - 1) == ch) == m;
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING:
            {
                size_t extra;

                extra = available - max_count;
                if (extra < (size_t)test->value_count)
                    limit -= (test->value_count - (Py_ssize_t)extra) * step;

                if (state->wide) {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = UCHAR_AT(text, pos) == ch && try_match(state,
                          &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING_IGN:
            {
                size_t extra;

                extra = available - max_count;
                if (extra < (size_t)test->value_count)
                    limit -= (test->value_count - (Py_ssize_t)extra) * step;

                if (state->wide) {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign(UCHAR_AT(text, pos), ch) &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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
                        match = same_char_ign(BCHAR_AT(text, pos), ch) &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING_IGN_REV:
            {
                size_t extra;

                extra = available - max_count;
                if (extra < (size_t)test->value_count)
                    limit -= (test->value_count - (Py_ssize_t)extra) * step;

                ch = test->values[test->value_count - 1];

                if (state->wide) {
                    for (;;) {
                        RE_Position next_position;

                        match = match_one(state, repeated, pos);
                        if (!match)
                            break;
                        pos += step;
                        match = same_char_ign(UCHAR_AT(text, pos - 1), ch) &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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
                        match = same_char_ign(BCHAR_AT(text, pos - 1), ch) &&
                          try_match(state, &node->next_1, pos, &next_position);
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
                            break;
                        if (pos == limit)
                            break;
                    }
                }
                break;
            }
            case RE_OP_STRING_REV:
            {
                size_t extra;

                extra = available - max_count;
                if (extra < (size_t)test->value_count)
                    limit -= (test->value_count - (Py_ssize_t)extra) * step;

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
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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
                        if (match && !is_guarded(state, index, pos,
                          RE_STATUS_TAIL))
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
                    if (match && !is_guarded(state, index, pos,
                      RE_STATUS_TAIL))
                        break;
                    if (pos == limit)
                        break;
                }
                break;
            }

            if (match) {
                /* The tail could match. */
                count = abs_ssize_t(pos - text_pos);
                text_pos = pos;

                if (count < max_count) {
                    /* The match is shorter than the maximum, so we might need
                     * to backtrack the repeat again to consume more.
                     */
                    rp_data->count = count;
                    if (has_groups)
                        reload_groups(state);
                } else {
                    /* We've reached the maximum, so we won't need to backtrack
                     * the repeat again.
                     */
                    if (has_groups)
                        pop_groups(state);
                    rp_data->start = bt_data->repeat.text_pos;
                    rp_data->count = bt_data->repeat.count;
                    discard_backtrack(state);
                }

                node = node->next_1.node;
                if (!guard(safe_state, index, text_pos, RE_STATUS_TAIL))
                    return RE_ERROR_MEMORY;
                goto advance;
            } else {
                /* The tail couldn't match. */
                if (has_groups)
                    drop_groups(state);
                rp_data->start = bt_data->repeat.text_pos;
                rp_data->count = bt_data->repeat.count;
                discard_backtrack(state);
            }
            break;
        }
        case RE_OP_LOOKAROUND: /* Lookaround. */
        {
            TRACE(("%s %d\n", re_op_text[node->op], node->match))

            /* Restore the captures and backtrack. */
            restore_all_captures(state, bt_data);
            discard_backtrack(state);
            break;
        }
        default:
            TRACE(("UNKNOWN OP %d\n", bt_data->op))
            return RE_ERROR_ILLEGAL;
        }
    }
}

/* Performs a match or search from the current text position.
 *
 * The state can sometimes be shared across threads. In such instances there's
 * a lock (mutex) on it. The lock is held for the duration of matching.
 */
Py_LOCAL_INLINE(int) do_match(RE_SafeState* safe_state, BOOL search) {
    RE_State* state;
    size_t available;
    int status;
    TRACE(("<<do_match>>\n"))

    state = safe_state->re_state;

    /* Is there enough to search? */
    available = state->reverse ? state->text_pos - state->slice_start :
      state->slice_end - state->text_pos;
    if (available < state->min_width || available == 0 && state->must_advance)
        return RE_ERROR_FAILURE;

    /* Release the GIL. */
    release_GIL(safe_state);

    /* Initialise the base context. */
    init_match(state);

    if (state->do_check) {
        state->do_check = FALSE;
        if (!general_check(safe_state, state->pattern->check_node,
          state->text_pos))
          {
            /* Re-acquire the GIL. */
            acquire_GIL(safe_state);
            return RE_ERROR_FAILURE;
        }
    }

    /* Perform the match. */
    status = basic_match(safe_state, state->pattern->start_node, search);
    if (status == RE_ERROR_SUCCESS) {
        Py_ssize_t max_end_index;
        PatternObject* pattern;
        RE_GroupInfo* group_info;
        Py_ssize_t g;

        /* Store the results. */
        state->lastindex = -1;
        state->lastgroup = -1;
        max_end_index = -1;

        /* Store the capture groups. */
        pattern = state->pattern;
        group_info = pattern->group_info;
        for (g = 0; g < pattern->group_count; g++) {
            RE_GroupSpan* span;

            span = &state->groups[g].span;
            /* The string positions are of type Py_ssize_t, so the format needs
             * to specify that.
             */
            TRACE(("group %d at %d from %" PY_FORMAT_SIZE_T "d to %"
              PY_FORMAT_SIZE_T "d\n", g + 1, ofs, span->start, span->end))

            if (span->start >= 0 && span->end >= 0 && group_info[g].end_index >
              max_end_index) {
                max_end_index = group_info[g].end_index;
                state->lastindex = g + 1;
                if (group_info[g].has_name)
                    state->lastgroup = g + 1;
            }
        }
    }

    /* Re-acquire the GIL. */
    acquire_GIL(safe_state);

    if (status == RE_ERROR_ILLEGAL)
        set_error(status, NULL);

    return status;
}

/* Gets a string from a Python object. */
Py_LOCAL_INLINE(BOOL) get_string(PyObject* string, void** characters,
  Py_ssize_t* length, Py_ssize_t* charsize) {
    /* Given a Python object, return a data pointer, a length (in characters),
     * and a character size. Return FALSE if the object is not a string (or not
     * compatible).
     */
    PyBufferProcs* buffer;
    Py_ssize_t bytes;
    Py_ssize_t size;

    /* Unicode objects do not support the buffer API. So, get the data directly
     * instead.
     */
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

/* Deallocates the groups storage. */
Py_LOCAL_INLINE(void) dealloc_groups(RE_GroupData* groups, size_t group_count)
  {
    size_t g;

    if (!groups)
        return;

    for (g = 0; g < group_count; g++)
        re_dealloc(groups[g].captures);

    re_dealloc(groups);
}

/* Initialises a state object. */
Py_LOCAL_INLINE(BOOL) state_init_2(RE_State* state, PatternObject* pattern,
  PyObject* string, void* characters, Py_ssize_t length, Py_ssize_t charsize,
  Py_ssize_t start, Py_ssize_t end, BOOL overlapped, int concurrent, BOOL
  use_lock) {
    Py_ssize_t final_pos;

    state->groups = NULL;
    state->repeats = NULL;
    state->save_captures = TRUE;
    state->saved_groups = NULL;
    state->backtrack_block.previous = NULL;
    state->backtrack_block.next = NULL;
    state->backtrack_block.capacity = RE_BACKTRACK_BLOCK_SIZE;
    state->current_capture_counts_block = NULL;
    state->lock = NULL;

    /* The capture groups. */
    if (pattern->group_count) {
        Py_ssize_t threshold;

        if (pattern->groups_storage) {
            state->groups = pattern->groups_storage;
            pattern->groups_storage = NULL;
        } else {
            state->groups = (RE_GroupData*)re_alloc(pattern->group_count *
              sizeof(RE_GroupData));
            if (!state->groups)
                goto error;
            memset(state->groups, 0, pattern->group_count *
              sizeof(RE_GroupData));
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
              (RE_GroupData*)re_alloc(state->saved_groups_capacity *
              sizeof(RE_GroupData));
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

    state->reverse = (pattern->flags & RE_FLAG_REVERSE) != 0;

    state->slice_start = start;
    state->slice_end = state->text_length;
    state->text_pos = state->reverse ? state->slice_end : state->slice_start;

    /* Point to the final newline and line separator if it's at the end of the
     * string, otherwise just -1.
     */
    state->final_newline = -1;
    state->final_line_sep = -1;
    final_pos = state->text_length - 1;
    if (final_pos >= 0) {
        RE_CODE ch;

        ch = state->char_at(state->text, final_pos);
        if (ch == '\n')
            state->final_newline = final_pos;
        if (state->encoding->is_line_sep(ch))
            state->final_line_sep = final_pos;

        if (ch == 0x0A) {
            --final_pos;
            if (final_pos >= 0 && state->char_at(state->text, final_pos) ==
              0x0D)
                state->final_line_sep = final_pos;
        }
    }

    /* If the 'new' behaviour is enabled then split correctly on zero-width
     * matches.
     */
    state->zero_width = (pattern->flags & RE_FLAG_NEW) != 0;
    state->must_advance = FALSE;
    state->do_check = pattern->do_check;

    state->pattern = pattern;
    state->string = string;

    if (pattern->repeat_count) {
        if (pattern->repeats_storage) {
            state->repeats = pattern->repeats_storage;
            pattern->repeats_storage = NULL;
        } else {
            state->repeats = (RE_RepeatData*)re_alloc(pattern->repeat_count *
              sizeof(RE_RepeatData));
            if (!state->repeats)
                goto error;
            memset(state->repeats, 0, pattern->repeat_count *
              sizeof(RE_RepeatData));
        }
    }

    Py_INCREF(state->pattern);
    Py_INCREF(state->string);

    /* Multithreading is allowed during matching when explicitly enabled or on
     * immutable strings.
     */
    switch (concurrent) {
    case RE_CONC_YES:
        state->is_multithreaded = TRUE;
        break;
    case RE_CONC_NO:
        state->is_multithreaded = FALSE;
        break;
    default:
        state->is_multithreaded = PyUnicode_Check(string) || PyString_Check(string);
        break;
    }

    /* A state struct can sometimes be shared across threads. In such
     * instances, if multithreading is enabled we need to protect the state
     * with a lock (mutex) during matching.
     */
    if (state->is_multithreaded && use_lock)
        state->lock = PyThread_allocate_lock();

    return TRUE;

error:
    re_dealloc(state->saved_groups);
    re_dealloc(state->repeats);
    dealloc_groups(state->groups, pattern->group_count);
    state->saved_groups = NULL;
    state->repeats = NULL;
    state->groups = NULL;
    return FALSE;
}

/* Initialises a state object. */
Py_LOCAL_INLINE(BOOL) state_init(RE_State* state, PatternObject* pattern,
  PyObject* string, Py_ssize_t start, Py_ssize_t end, BOOL overlapped, int
  concurrent, BOOL use_lock) {
    void* characters;
    Py_ssize_t length;
    Py_ssize_t charsize;

    /* Get the string to search or match. */
    if (!get_string(string, &characters, &length, &charsize))
        return FALSE;

    return state_init_2(state, pattern, string, characters, length, charsize,
      start, end, overlapped, concurrent, use_lock);
}

/* Deallocates repeat data. */
Py_LOCAL_INLINE(void) dealloc_repeats(RE_RepeatData* repeats, Py_ssize_t
  repeat_count) {
    Py_ssize_t i;

    if (!repeats)
        return;

    for (i = 0; i < repeat_count; i++) {
        re_dealloc(repeats[i].body_guard_list.spans);
        re_dealloc(repeats[i].tail_guard_list.spans);
    }

    re_dealloc(repeats);
}

/* Finalises a state object, discarding its contents. */
Py_LOCAL_INLINE(void) state_fini(RE_State* state) {
    PatternObject* pattern;
    RE_BacktrackBlock* current;
    RE_CaptureCountsBlock* current2;

    /* Discard the lock (mutex) if there's one. */
    if (state->lock)
        PyThread_free_lock(state->lock);

    /* Deallocate the backtrack blocks. */
    current = state->backtrack_block.next;
    while (current) {
        RE_BacktrackBlock* next;

        next = current->next;
        re_dealloc(current);
        current = next;
    }

    current2 = state->current_capture_counts_block;
    if (current2) {
        while (current2->next)
            current2 = current2->next;

        while (current2) {
            RE_CaptureCountsBlock* previous;

            previous = current2->previous;
            re_dealloc(current2->items);
            re_dealloc(current2);
            current2 = previous;
        }
    }

    pattern = state->pattern;

    if (pattern->groups_storage)
        dealloc_groups(state->groups, pattern->group_count);
    else
        pattern->groups_storage = state->groups;

    if (pattern->repeats_storage)
        dealloc_repeats(state->repeats, pattern->repeat_count);
    else
        pattern->repeats_storage = state->repeats;

    if (pattern->saved_groups_storage)
        re_dealloc(state->saved_groups);
    else {
        pattern->saved_groups_capacity_storage = state->saved_groups_capacity;
        pattern->saved_groups_storage = state->saved_groups;
    }

    Py_DECREF(state->pattern);
    Py_DECREF(state->string);
}

/* Converts a string index to an integer.
 *
 * If the index is None then the default will be returned.
 */
Py_LOCAL_INLINE(Py_ssize_t) as_string_index(PyObject* obj, Py_ssize_t def) {
    Py_ssize_t value;

    if (obj == Py_None)
        return def;

    value = PyInt_AsSsize_t(obj);
    if (value != -1 || !PyErr_Occurred())
        return value;

    PyErr_Clear();

    value = PyLong_AsLong(obj);
    if (value != -1 || !PyErr_Occurred())
        return value;

    set_error(RE_ERROR_INDEX, NULL);
    return 0;
}

/* Deallocates a MatchObject. */
static void match_dealloc(MatchObject* self) {
    Py_XDECREF(self->string);
    Py_DECREF(self->pattern);
    dealloc_groups(self->groups, self->group_count);
    Py_XDECREF(self->regs);
    PyObject_DEL(self);
}

/* Gets a MatchObject's group by integer index. */
Py_LOCAL_INLINE(PyObject*) match_get_group_by_index(MatchObject* self,
  Py_ssize_t index, PyObject* def) {
    RE_GroupSpan* span;

    if (index < 0 || index > self->group_count) {
        /* Raise error if we were given a bad group number. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0)
        return PySequence_GetSlice(self->string, self->match_start,
          self->match_end);

    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    span = &self->groups[index - 1].span;

    if (self->string == Py_None || span->start < 0 || span->end < 0) {
        /* Return default value if the string or group is undefined. */
        Py_INCREF(def);
        return def;
    }

    return PySequence_GetSlice(self->string, span->start, span->end);
}

/* Gets a MatchObject's start by integer index. */
static PyObject* match_get_start_by_index(MatchObject* self, Py_ssize_t index)
  {
    RE_GroupSpan* span;

    if (index < 0 || index > self->group_count) {
        /* Raise error if we were given a bad group number. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0)
        return Py_BuildValue("n", self->match_start);

    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    span = &self->groups[index - 1].span;
    return Py_BuildValue("n", span->start);
}

/* Gets a MatchObject's starts by integer index. */
static PyObject* match_get_starts_by_index(MatchObject* self, Py_ssize_t index)
  {
    RE_GroupData* group;
    PyObject* result;
    PyObject* item;
    RE_GroupSpan* span;

    if (index < 0 || index > self->group_count) {
        /* Raise error if we were given a bad group number. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0) {
        result = PyList_New(1);
        if (!result)
            return NULL;

        item = Py_BuildValue("n", self->match_start);
        if (!item)
            goto error;
        PyList_SET_ITEM(result, 0, item);

        return result;
    }

    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    group = &self->groups[index - 1];
    if (group->capture_count > 0) {
        /* At least one capture. */
        size_t i;

        result = PyList_New(group->capture_count);
        if (!result)
            return NULL;

        for (i = 0; i < group->capture_count; i++) {
            span = &group->captures[i];
            item = Py_BuildValue("n", span->start);
            if (!item)
                goto error;
            PyList_SET_ITEM(result, i, item);
        }
    } else {
        span = &group->span;

        if (0 <= span->start && span->start <= span->end) {
            /* Only one capture. */
            result = PyList_New(1);
            if (!result)
                return NULL;

            item = Py_BuildValue("n", span->start);
            if (!item)
                goto error;
            PyList_SET_ITEM(result, 0, item);
        } else {
            /* No spans. */
            result = PyList_New(0);
            if (!result)
                return NULL;
        }
    }

    return result;

error:
    Py_DECREF(result);
    return NULL;
}

/* Gets a MatchObject's end by integer index. */
static PyObject* match_get_end_by_index(MatchObject* self, Py_ssize_t index) {
    RE_GroupSpan* span;

    if (index < 0 || index > self->group_count) {
        /* Raise error if we were given a bad group number. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0)
        return Py_BuildValue("n", self->match_end);

    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    span = &self->groups[index - 1].span;
    return Py_BuildValue("n", span->end);
}

/* Gets a MatchObject's ends by integer index. */
static PyObject* match_get_ends_by_index(MatchObject* self, Py_ssize_t index) {
    RE_GroupData* group;
    PyObject* result;
    PyObject* item;
    RE_GroupSpan* span;

    if (index < 0 || index > self->group_count) {
        /* Raise error if we were given a bad group number. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0) {
        result = PyList_New(1);
        if (!result)
            return NULL;

        item = Py_BuildValue("n", self->match_end);
        if (!item)
            goto error;
        PyList_SET_ITEM(result, 0, item);

        return result;
    }

    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    group = &self->groups[index - 1];
    if (group->capture_count > 0) {
        /* At least one capture. */
        size_t i;

        result = PyList_New(group->capture_count);
        if (!result)
            return NULL;

        for (i = 0; i < group->capture_count; i++) {
            span = &group->captures[i];
            item = Py_BuildValue("n", span->end);
            if (!item)
                goto error;
            PyList_SET_ITEM(result, i, item);
        }
    } else {
        span = &group->span;

        if (0 <= span->start && span->start <= span->end) {
            /* Only one capture. */
            result = PyList_New(1);
            if (!result)
                return NULL;

            item = Py_BuildValue("n", span->end);
            if (!item)
                goto error;
            PyList_SET_ITEM(result, 0, item);
        } else {
            /* No spans. */
            result = PyList_New(0);
            if (!result)
                return NULL;
        }
    }

    return result;

error:
    Py_DECREF(result);
    return NULL;
}

/* Gets a MatchObject's span by integer index. */
static PyObject* match_get_span_by_index(MatchObject* self, Py_ssize_t index) {
    RE_GroupSpan* span;

    if (index < 0 || index > self->group_count) {
        /* Raise error if we were given a bad group number. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0)
        return Py_BuildValue("nn", self->match_start, self->match_end);

    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    span = &self->groups[index - 1].span;
    return Py_BuildValue("nn", span->start, span->end);
}

/* Gets a MatchObject's spans by integer index. */
static PyObject* match_get_spans_by_index(MatchObject* self, Py_ssize_t index)
  {
    RE_GroupData* group;
    PyObject* result;
    PyObject* item;
    RE_GroupSpan* span;

    if (index < 0 || index > self->group_count) {
        /* Raise error if we were given a bad group number. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0) {
        result = PyList_New(1);
        if (!result)
            return NULL;

        item = Py_BuildValue("nn", self->match_start, self->match_end);
        if (!item)
            goto error;
        PyList_SET_ITEM(result, 0, item);

        return result;
    }

    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    group = &self->groups[index - 1];
    if (group->capture_count > 0) {
        /* At least one capture. */
        size_t i;

        result = PyList_New(group->capture_count);
        if (!result)
            return NULL;

        for (i = 0; i < group->capture_count; i++) {
            span = &group->captures[i];
            item = Py_BuildValue("nn", span->start, span->end);
            if (!item)
                goto error;
            PyList_SET_ITEM(result, i, item);
        }
    } else {
        span = &group->span;

        if (0 <= span->start && span->start <= span->end) {
            /* Only one capture. */
            result = PyList_New(1);
            if (!result)
                return NULL;

            item = Py_BuildValue("nn", span->start, span->end);
            if (!item)
                goto error;
            PyList_SET_ITEM(result, 0, item);
        } else {
            /* No spans. */
            result = PyList_New(0);
            if (!result)
                return NULL;
        }
    }

    return result;

error:
    Py_DECREF(result);
    return NULL;
}

/* Gets a MatchObject's captures by integer index. */
static PyObject* match_get_captures_by_index(MatchObject* self, Py_ssize_t
  index) {
    RE_GroupData* group;
    PyObject* result;
    PyObject* slice;
    RE_GroupSpan* span;

    if (index < 0 || index > self->group_count) {
        /* Raise error if we were given a bad group number. */
        set_error(RE_ERROR_NO_SUCH_GROUP, NULL);
        return NULL;
    }

    if (index == 0) {
        result = PyList_New(1);
        if (!result)
            return NULL;

        slice = PySequence_GetSlice(self->string, self->match_start,
          self->match_end);
        if (!slice)
            goto error;
        PyList_SET_ITEM(result, 0, slice);

        return result;
    }

    /* Capture group indexes are 1-based (excluding group 0, which is the
     * entire matched string).
     */
    group = &self->groups[index - 1];
    if (group->capture_count > 0) {
        /* At least one capture. */
        size_t i;

        result = PyList_New(group->capture_count);
        if (!result)
            return NULL;

        for (i = 0; i < group->capture_count; i++) {
            span = &group->captures[i];
            slice = PySequence_GetSlice(self->string, span->start, span->end);
            if (!slice)
                goto error;
            PyList_SET_ITEM(result, i, slice);
        }
    } else {
        span = &group->span;

        if (0 <= span->start && span->start <= span->end) {
            /* Only one capture. */
            result = PyList_New(1);
            if (!result)
                return NULL;

            slice = PySequence_GetSlice(self->string, span->start, span->end);
            if (!slice)
                goto error;
            PyList_SET_ITEM(result, 0, slice);
        } else {
            /* No captures. */
            result = PyList_New(0);
            if (!result)
                return NULL;
        }
    }

    return result;

error:
    Py_DECREF(result);
    return NULL;
}

/* Converts a group index to an integer. */
Py_LOCAL_INLINE(Py_ssize_t) as_group_index(PyObject* obj) {
    Py_ssize_t value;

    value = PyInt_AsSsize_t(obj);
    if (value != -1 || !PyErr_Occurred())
        return value;

    PyErr_Clear();

    value = PyLong_AsLong(obj);
    if (value != -1 || !PyErr_Occurred())
        return value;

    set_error(RE_ERROR_INDEX, NULL);
    return 0;
}

/* Gets a MatchObject's group index.
 *
 * The supplied index can be an integer or a string (group name) object.
 */
Py_LOCAL_INLINE(Py_ssize_t) match_get_group_index(MatchObject* self, PyObject*
  index, BOOL allow_neg) {
    Py_ssize_t group;

    /* Is the index an integer? */
    group = as_group_index(index);
    if (!PyErr_Occurred()) {
        /* Adjust negative indices where valid and allowed. */
        if (allow_neg && -self->group_count <= group && group <= -1)
            group += self->group_count + 1;

        return group;
    }

    /* The index might be a group name. */
    if (self->pattern->groupindex) {
        /* Look up the name. */
        PyErr_Clear();

        index = PyObject_GetItem(self->pattern->groupindex, index);
        if (index) {
            /* Check that we have an integer. */
            group = as_group_index(index);
            if (!PyErr_Occurred()) {
                Py_DECREF(index);
                return group;
            }
        }
    }

    PyErr_Clear();
    return -1;
}

/* Gets a MatchObject's group by object index. */
Py_LOCAL_INLINE(PyObject*) match_get_group(MatchObject* self, PyObject* index,
  PyObject* def, BOOL allow_neg) {
    /* Check that the index is an integer or a string. */
    if (PyInt_Check(index) || PyLong_Check(index) || PyUnicode_Check(index) ||
      PyString_Check(index))
        return match_get_group_by_index(self, match_get_group_index(self,
          index, allow_neg), def);

    set_error(RE_ERROR_GROUP_INDEX_TYPE, index);
    return NULL;
}

/* Gets info from a MatchObject by object index. */
static PyObject* get_by_arg(MatchObject* self, PyObject* index,
  RE_GetByIndexFunc get_by_index) {
    /* Check that the index is an integer or a string. */
    if (PyInt_Check(index) || PyLong_Check(index) || PyUnicode_Check(index) ||
      PyString_Check(index))
        return get_by_index(self, match_get_group_index(self, index, FALSE));

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

/* Generic method for getting info from a MatchObject. */
static PyObject* get_from_match(MatchObject* self, PyObject* args,
  RE_GetByIndexFunc get_by_index) {
    Py_ssize_t size;
    PyObject* result;
    Py_ssize_t i;

    size = PyTuple_GET_SIZE(args);

    switch (size) {
    case 0:
        /* get() */
        result = get_by_index(self, 0);
        break;
    case 1:
        /* get(x) */
        result = get_by_arg(self, PyTuple_GET_ITEM(args, 0), get_by_index);
        break;
    default:
        /* get(x, y, z, ...) */
        /* Fetch multiple items. */
        result = PyTuple_New(size);
        if (!result)
            return NULL;
        for (i = 0; i < size; i++) {
            PyObject* item = get_by_arg(self, PyTuple_GET_ITEM(args, i),
              get_by_index);
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
    return get_from_match(self, args, match_get_start_by_index);
}

/* MatchObject's 'starts' method. */
static PyObject* match_starts(MatchObject* self, PyObject* args) {
    return get_from_match(self, args, match_get_starts_by_index);
}

/* MatchObject's 'end' method. */
static PyObject* match_end(MatchObject* self, PyObject* args) {
    return get_from_match(self, args, match_get_end_by_index);
}

/* MatchObject's 'ends' method. */
static PyObject* match_ends(MatchObject* self, PyObject* args) {
    return get_from_match(self, args, match_get_ends_by_index);
}

/* MatchObject's 'span' method. */
static PyObject* match_span(MatchObject* self, PyObject* args) {
    return get_from_match(self, args, match_get_span_by_index);
}

/* MatchObject's 'spans' method. */
static PyObject* match_spans(MatchObject* self, PyObject* args) {
    return get_from_match(self, args, match_get_spans_by_index);
}

/* MatchObject's 'captures' method. */
static PyObject* match_captures(MatchObject* self, PyObject* args) {
    return get_from_match(self, args, match_get_captures_by_index);
}

/* MatchObject's 'groups' method. */
static PyObject* match_groups(MatchObject* self, PyObject* args, PyObject* kw)
  {
    PyObject* result;
    Py_ssize_t g;

    PyObject* def = Py_None;
    static char* kwlist[] = { "default", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "|O:groups", kwlist, &def))
        return NULL;

    result = PyTuple_New(self->group_count);
    if (!result)
        return NULL;

    /* Group 0 is the entire matched portion of the string. */
    for (g = 0; g < self->group_count; g++) {
        PyObject* item;
        item = match_get_group_by_index(self, g + 1, def);
        if (!item) {
            Py_DECREF(result);
            return NULL;
        }
        PyTuple_SET_ITEM(result, g, item);
    }

    return result;
}

/* MatchObject's 'groupdict' method. */
static PyObject* match_groupdict(MatchObject* self, PyObject* args, PyObject*
  kw) {
    PyObject* result;
    PyObject* keys;
    Py_ssize_t g;

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

    for (g = 0; g < PyList_GET_SIZE(keys); g++) {
        int status;
        PyObject* key;
        PyObject* value;
        key = PyList_GET_ITEM(keys, g);
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

/* Gets a Python object by name from a named module. */
Py_LOCAL_INLINE(PyObject*) get_object(char* module_name, char* object_name) {
    PyObject* module;
    PyObject* object;

    module = PyImport_ImportModule(module_name);
    if (!module)
        return NULL;

    object = PyObject_GetAttrString(module, object_name);
    Py_DECREF(module);

    return object;
}

/* Calls a function in a module. */
Py_LOCAL_INLINE(PyObject*) call(char* module_name, char* function_name,
  PyObject* args) {
    PyObject* function;
    PyObject* result;

    if (!args)
        return NULL;

    function = get_object(module_name, function_name);
    if (!function)
        return NULL;

    result = PyObject_CallObject(function, args);
    Py_DECREF(function);
    Py_DECREF(args);

    return result;
}

/* Gets a replacement item from the replacement list.
 *
 * The replacement item could be a string literal or a group.
 */
Py_LOCAL_INLINE(PyObject*) get_match_replacement(MatchObject* self, PyObject*
  item, PyObject* string, Py_ssize_t group_count) {
    Py_ssize_t index;

    if (PyUnicode_Check(item) || PyString_Check(item)) {
        /* It's a literal, which can be added directly to the list. */
        Py_INCREF(item);
        return item;
    }

    /* Is it a group reference? */
    index = as_group_index(item);
    if (PyErr_Occurred()) {
        /* Not a group either! */
        set_error(RE_ERROR_REPLACEMENT, NULL);
        return NULL;
    }

    if (index == 0) {
        /* The entire matched portion of the string. */
        return PySequence_GetSlice(string, self->match_start, self->match_end);
    } else if (index >= 1 && index <= group_count) {
        /* A group. If it didn't match then return None instead. */
        RE_GroupSpan* span;

        span = &self->groups[index - 1].span;
        if (0 <= span->start && span->start <= span->end)
            return PySequence_GetSlice(string, span->start, span->end);
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
Py_LOCAL_INLINE(PyObject*) join_list(PyObject* list, PyObject* string, BOOL
  reversed) {
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
Py_LOCAL_INLINE(int) add_item(JoinInfo* join_info, PyObject* item) {
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
        join_info->list = PyList_New(2);
        if (!join_info->list) {
            status = RE_ERROR_MEMORY;
            goto error;
        }

        PyList_SET_ITEM(join_info->list, 0, join_info->item);
        join_info->item = NULL;

        PyList_SET_ITEM(join_info->list, 1, item);
        Py_INCREF(item);

        return 0;
    }

    /* This is the first item. */
    join_info->item = item;
    Py_INCREF(join_info->item);

    return 0;

error:
    Py_XDECREF(join_info->list);
    Py_XDECREF(join_info->item);
    return status;
}

/* Joins together a list of strings for pattern_subx. */
Py_LOCAL_INLINE(PyObject*) join_list_info(JoinInfo* join_info, PyObject*
  string) {
    /* If the list already exists then just do the join. */
    if (join_info->list)
        return join_list(join_info->list, string, join_info->reversed);

    /* If we have only 1 item then we _might_ be able to just return it. */
    if (join_info->item) {
        /* We can return the single item only if it's the same type of string
         * as the joiner.
         */
        if (PyObject_Type(join_info->item) == PyObject_Type(string))
            return join_info->item;

        /* We'll default to the normal joining method, which requires the item
         * to be in a list.
         */
        join_info->list = PyList_New(1);
        if (!join_info->list) {
            Py_DECREF(join_info->item);
            return NULL;
        }

        PyList_SET_ITEM(join_info->list, 0, join_info->item);

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
    replacement = call(RE_MODULE, "compile_replacement", PyTuple_Pack(2,
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
    Py_XDECREF(join_info.list);
    Py_XDECREF(join_info.item);
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
    Py_ssize_t g;

    regs = PyTuple_New(self->group_count + 1);
    if (!regs)
        return NULL;

    item = Py_BuildValue("nn", self->match_start, self->match_end);
    if (!item) {
        Py_DECREF(regs);
        return NULL;
    }
    PyTuple_SET_ITEM(regs, 0, item);

    for (g = 0; g < self->group_count; g++) {
        RE_GroupSpan* span;

        span = &self->groups[g].span;
        item = Py_BuildValue("nn", span->start, span->end);
        if (!item) {
            Py_DECREF(regs);
            return NULL;
        }
        PyTuple_SET_ITEM(regs, g + 1, item);
    }

    Py_INCREF(regs);
    self->regs = regs;

    return regs;
}

/* MatchObject's slice method. */
Py_LOCAL_INLINE(PyObject*) match_get_group_slice(MatchObject* self, PyObject*
  slice) {
    Py_ssize_t start;
    Py_ssize_t end;
    Py_ssize_t step;
    Py_ssize_t slice_length;

    if (PySlice_GetIndicesEx((PySliceObject*)slice, self->group_count + 1,
      &start, &end, &step, &slice_length) < 0)
        return NULL;

    if (slice_length <= 0)
        return PyTuple_New(0);
    else {
        PyObject* result;
        Py_ssize_t cur;
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
        return match_get_group_slice(self, item);

    return match_get_group(self, item, Py_None, TRUE);
}

/* The documentation of a MatchObject. */
PyDoc_STRVAR(match_group_doc,
    "group([group1, ...]) --> string or tuple of strings.\n\
    Return one or more subgroups of the match.  If there is a single argument,\n\
    the result is a single string, or None if the group did not contribute to\n\
    the match; if there are multiple arguments, the result is a tuple with one\n\
    item per argument; if there are no arguments, the whole match is returned.\n\
    Group 0 is the whole match.");

PyDoc_STRVAR(match_start_doc,
    "start([group1, ...]) --> int or tuple of ints.\n\
    Return the index of the start of one or more subgroups of the match.  If\n\
    there is a single argument, the result is an index, or -1 if the group did\n\
    not contribute to the match; if there are multiple arguments, the result is\n\
    a tuple with one item per argument; if there are no arguments, the index of\n\
    the start of the whole match is returned.  Group 0 is the whole match.");

PyDoc_STRVAR(match_end_doc,
    "end([group1, ...]) --> int or tuple of ints.\n\
    Return the index of the end of one or more subgroups of the match.  If there\n\
    is a single argument, the result is an index, or -1 if the group did not\n\
    contribute to the match; if there are multiple arguments, the result is a\n\
    tuple with one item per argument; if there are no arguments, the index of\n\
    the end of the whole match is returned.  Group 0 is the whole match.");

PyDoc_STRVAR(match_span_doc,
    "span([group1, ...]) --> 2-tuple of int or tuple of 2-tuple of ints.\n\
    Return the span (a 2-tuple of the indices of the start and end) of one or\n\
    more subgroups of the match.  If there is a single argument, the result is a\n\
    span, or (-1, -1) if the group did not contribute to the match; if there are\n\
    multiple arguments, the result is a tuple with one item per argument; if\n\
    there are no arguments, the span of the whole match is returned.  Group 0 is\n\
    the whole match.");

PyDoc_STRVAR(match_groups_doc,
    "groups(default=None) --> tuple of strings.\n\
    Return a tuple containing all the subgroups of the match.  The argument is\n\
    the default for groups that did not participate in the match.");

PyDoc_STRVAR(match_groupdict_doc,
    "groupdict(default=None) --> dict.\n\
    Return a dictionary containing all the named subgroups of the match, keyed\n\
    by the subgroup name.  The argument is the value to be given for groups that\n\
    did not participate in the match.");

PyDoc_STRVAR(match_expand_doc,
    "expand(template) --> string.\n\
    Return the string obtained by doing backslash substitution on the template,\n\
    as done by the sub() method.");

PyDoc_STRVAR(match_captures_doc,
    "captures([group1, ...]) --> list of strings or tuple of list of strings.\n\
    Return the captures of one or more subgroups of the match.  If there is a\n\
    single argument, the result is a list of strings; if there are multiple\n\
    arguments, the result is a tuple of lists with one item per argument; if\n\
    there are no arguments, the captures of the whole match is returned.  Group\n\
    0 is the whole match.");

PyDoc_STRVAR(match_starts_doc,
    "starts([group1, ...]) --> list of ints or tuple of list of ints.\n\
    Return the indices of the starts of the captures of one or more subgroups of\n\
    the match.  If there is a single argument, the result is a list of indices;\n\
    if there are multiple arguments, the result is a tuple of lists with one\n\
    item per argument; if there are no arguments, the indices of the starts of\n\
    the captures of the whole match is returned.  Group 0 is the whole match.");

PyDoc_STRVAR(match_ends_doc,
    "ends([group1, ...]) --> list of ints or tuple of list of ints.\n\
    Return the indices of the ends of the captures of one or more subgroups of\n\
    the match.  If there is a single argument, the result is a list of indices;\n\
    if there are multiple arguments, the result is a tuple of lists with one\n\
    item per argument; if there are no arguments, the indices of the ends of the\n\
    captures of the whole match is returned.  Group 0 is the whole match.");

PyDoc_STRVAR(match_spans_doc,
    "spans([group1, ...]) --> list of 2-tuple of ints or tuple of list of 2-tuple of ints.\n\
    Return the spans (a 2-tuple of the indices of the start and end) of the\n\
    captures of one or more subgroups of the match.  If there is a single\n\
    argument, the result is a list of spans; if there are multiple arguments,\n\
    the result is a tuple of lists with one item per argument; if there are no\n\
    arguments, the spans of the captures of the whole match is returned.  Group\n\
    0 is the whole match.");

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
    {"captures", (PyCFunction)match_captures, METH_VARARGS,
      match_captures_doc},
    {"starts", (PyCFunction)match_starts, METH_VARARGS, match_starts_doc},
    {"ends", (PyCFunction)match_ends, METH_VARARGS, match_ends_doc},
    {"spans", (PyCFunction)match_spans, METH_VARARGS, match_spans_doc},
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
            return Py_BuildValue("n", self->lastindex);

        Py_INCREF(Py_None);
        return Py_None;
    }

    if (!strcmp(name, "lastgroup")) {
        if (self->pattern->indexgroup && self->lastgroup >= 0) {
            PyObject* index = Py_BuildValue("n", self->lastgroup);
            PyObject* result = PyDict_GetItem(self->pattern->indexgroup, index);
            Py_DECREF(index);
            if (result) {
                Py_INCREF(result);
                return result;
            }
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

/* Copies the groups. */
Py_LOCAL_INLINE(RE_GroupData*) copy_groups(RE_GroupData* groups, Py_ssize_t
  group_count) {
    RE_GroupData* groups_copy;
    Py_ssize_t g;

    groups_copy = (RE_GroupData*)re_alloc(group_count * sizeof(RE_GroupData));
    if (!groups_copy)
        return NULL;

    memset(groups_copy, 0, group_count * sizeof(RE_GroupData));

    for (g = 0; g < group_count; g++) {
        RE_GroupData* orig;
        RE_GroupData* copy;

        orig = &groups[g];
        copy = &groups_copy[g];
        copy->span = orig->span;
        if (orig->capture_count > 0) {
            copy->captures = (RE_GroupSpan*)re_alloc(orig->capture_count *
              sizeof(RE_GroupSpan));
            if (!copy->captures)
                goto error;
            Py_MEMCPY(copy->captures, orig->captures, orig->capture_count *
              sizeof(RE_GroupSpan));
            copy->capture_capacity = orig->capture_count;
            copy->capture_count = orig->capture_count;
        }
    }

    return groups_copy;

error:
    dealloc_groups(groups_copy, group_count);
    return NULL;
}

/* Creates a new MatchObject. */
Py_LOCAL_INLINE(PyObject*) pattern_new_match(PatternObject* pattern, RE_State*
  state, int status) {
    /* Create MatchObject (from state object). */
    if (status > 0) {
        MatchObject* match;

        /* Create a NatchObject. */
        match = PyObject_NEW(MatchObject, &Match_Type);
        if (!match)
            return NULL;

        match->string = state->string;
        match->pattern = pattern;
        match->regs = NULL;
        Py_INCREF(match->string);
        Py_INCREF(match->pattern);

        /* Copy the groups to the MatchObject. */
        if (pattern->group_count > 0) {
            match->groups = copy_groups(state->groups, pattern->group_count);
            if (!match->groups) {
                Py_DECREF(match);
                return NULL;
            }
        } else
            match->groups = NULL;

        match->group_count = pattern->group_count;

        match->pos = state->slice_start;
        match->endpos = state->slice_end;

        if (state->reverse) {
            match->match_start = state->text_pos;
            match->match_end = state->match_pos;
        } else {
            match->match_start = state->match_pos;
            match->match_end = state->text_pos;
        }

        match->lastindex = state->lastindex;
        match->lastgroup = state->lastgroup;

        return (PyObject*)match;
    } else if (status == 0) {
        /* No match. */
        Py_INCREF(Py_None);
        return Py_None;
    } else {
        /* Internal error. */
        set_error(status, NULL);
        return NULL;
    }
}

/* Gets the text of a capture group from a state. */
Py_LOCAL_INLINE(PyObject*) state_get_group(RE_State* state, Py_ssize_t index,
  PyObject* string, BOOL empty) {
    RE_GroupSpan* span;
    Py_ssize_t start;
    Py_ssize_t end;

    span = &state->groups[index - 1].span;
    if (string != Py_None && index >= 1 && index <= state->pattern->group_count
      && span->start >= 0 && span->end >= 0) {
        start = span->start;
        end = span->end;
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

/* Acquires the lock (mutex) on the state if there's one.
 *
 * It also increments the owner's refcount just to ensure that it won't be
 * destroyed by another thread.
 */
void acquire_state_lock(PyObject* owner, RE_SafeState* safe_state) {
    RE_State* state;

    state = safe_state->re_state;

    if (state->lock) {
        /* In order to avoid deadlock we need to release the GIL while trying
         * to acquire the lock.
         */
        Py_INCREF(owner);
        if (!PyThread_acquire_lock(state->lock, 0)) {
            release_GIL(safe_state);
            PyThread_acquire_lock(state->lock, 1);
            acquire_GIL(safe_state);
        }
    }
}

/* Releases the lock (mutex) on the state if there's one.
 *
 * It also decrements the owner's refcount, which was incremented when the lock
 * was acquired.
 */
void release_state_lock(PyObject* owner, RE_SafeState* safe_state) {
    RE_State* state;

    state = safe_state->re_state;

    if (state->lock) {
        PyThread_release_lock(state->lock);
        Py_DECREF(owner);
    }
}

/* ScannerObject's 'match' method. */
static PyObject* scanner_match(ScannerObject* self, PyObject* unused) {
    RE_State* state;
    RE_SafeState safe_state;
    PyObject* match;

    state = &self->state;

    /* Initialise the "safe state" structure. */
    safe_state.re_state = state;
    safe_state.thread_state = NULL;

    /* Acquire the state lock in case we're sharing the scanner object across
     * threads.
     */
    acquire_state_lock((PyObject*)self, &safe_state);

    if (self->status == 0) {
        /* No match. */
        release_state_lock((PyObject*)self, &safe_state);
        Py_INCREF(Py_None);
        return Py_None;
    } else if (self->status < 0) {
        /* Internal error. */
        release_state_lock((PyObject*)self, &safe_state);
        set_error(self->status, NULL);
        return NULL;
    }

    self->status = do_match(&safe_state, FALSE);
    if (self->status < 0) {
        release_state_lock((PyObject*)self, &safe_state);
        return NULL;
    }

    match = pattern_new_match(self->pattern, state, self->status);

    /* Continue from where we left off, but don't allow 2 contiguous zero-width
     * matches.
     */
    state->must_advance = state->text_pos == state->match_pos;

    /* Release the state lock. */
    release_state_lock((PyObject*)self, &safe_state);

    return match;
}

/* ScannerObject's 'search' method. */
static PyObject* scanner_search(ScannerObject* self, PyObject *unused) {
    RE_State* state;
    RE_SafeState safe_state;
    PyObject* match;

    state = &self->state;

    /* Initialise the "safe state" structure. */
    safe_state.re_state = state;
    safe_state.thread_state = NULL;

    /* Acquire the state lock in case we're sharing the scanner object across
     * threads.
     */
    acquire_state_lock((PyObject*)self, &safe_state);

    if (self->status == 0) {
        /* No match. */
        release_state_lock((PyObject*)self, &safe_state);
        Py_INCREF(Py_None);
        return Py_None;
    } else if (self->status < 0) {
        /* Internal error. */
        release_state_lock((PyObject*)self, &safe_state);
        set_error(self->status, NULL);
        return NULL;
    }

    self->status = do_match(&safe_state, TRUE);
    if (self->status < 0) {
        release_state_lock((PyObject*)self, &safe_state);
        return NULL;
    }

    match = pattern_new_match(self->pattern, state, self->status);

    if (state->overlapped) {
        /* Advance one character. */
        Py_ssize_t step;

        step = state->reverse ? -1 : 1;
        state->text_pos = state->match_pos + step;
        state->must_advance = FALSE;
    } else
        /* Continue from where we left off, but don't allow 2 contiguous
         * zero-width matches.
         */
        state->must_advance = state->text_pos == state->match_pos;

    /* Release the state lock. */
    release_state_lock((PyObject*)self, &safe_state);

    return match;
}

/* ScannerObject's 'next' method. */
static PyObject* scanner_next(PyObject* self) {
    PyObject* match;

    match = scanner_search((ScannerObject*)self, NULL);

    if (match == Py_None) {
        /* No match. */
        Py_DECREF(Py_None);
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    return match;
}

/* Returns an iterator for a ScannerObject.
 *
 * The iterator is actually the ScannerObject itself.
 */
static PyObject* scanner_iter(PyObject* self) {
    Py_INCREF(self);
    return self;
}

/* Gets the next results from a scanner iterator. */
static PyObject* scanner_iternext(PyObject* self) {
    PyObject* match;

    match = scanner_search((ScannerObject*)self, NULL);

    if (match == Py_None) {
        /* No match. */
        Py_DECREF(Py_None);
        return NULL;
    }

    return match;
}

/* ScannerObject's methods. */
static PyMethodDef scanner_methods[] = {
    {"match", (PyCFunction)scanner_match, METH_NOARGS},
    {"search", (PyCFunction)scanner_search, METH_NOARGS},
    {"next", (PyCFunction)scanner_next, METH_NOARGS},
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
    0,                            /* tp_setattr */
    0,                            /* tp_compare */
    0,                            /* tp_repr */
    0,                            /* tp_as_number */
    0,                            /* tp_as_sequence */
    0,                            /* tp_as_mapping */
    0,                            /* tp_hash */
    0,                            /* tp_call */
    0,                            /* tp_str */
    0,                            /* tp_getattro */
    0,                            /* tp_setattro */
    0,                            /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,           /* tp_flags */
    0,                            /* tp_doc */
    0,                            /* tp_traverse */
    0,                            /* tp_clear */
    0,                            /* tp_richcompare */
    0,                            /* tp_weaklistoffset */
    scanner_iter,                 /* tp_iter */
    scanner_iternext,             /* tp_iternext */
    scanner_methods,              /* tp_methods */
};

/* Decodes a 'concurrent' argument. */
Py_LOCAL_INLINE(Py_ssize_t) decode_concurrent(PyObject* concurrent) {
    Py_ssize_t value;

    if (concurrent == Py_None)
        return RE_CONC_DEFAULT;

    value = PyLong_AsLong(concurrent);
    if (value != -1 || !PyErr_Occurred())
        return value == 0 ? RE_CONC_NO : RE_CONC_YES;

    return RE_CONC_DEFAULT;
}

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
    PyObject* concurrent = Py_None;
    static char* kwlist[] = { "string", "pos", "endpos", "overlapped",
      "concurrent", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|OOnO:scanner", kwlist,
      &string, &pos, &endpos, &overlapped, &concurrent))
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

    if (!state_init(&self->state, pattern, string, start, end, overlapped != 0,
      decode_concurrent(concurrent), TRUE)) {
        PyObject_DEL(self);
        return NULL;
    }

    self->status = 1;

    return (PyObject*) self;
}

/* SplitterObject's 'split' method. */
static PyObject* splitter_split(SplitterObject* self, PyObject *unused) {
    RE_State* state;
    RE_SafeState safe_state;
    PyObject* result;

    state = &self->state;

    /* Initialise the "safe state" structure. */
    safe_state.re_state = state;
    safe_state.thread_state = NULL;

    /* Acquire the state lock in case we're sharing the splitter object across
     * threads.
     */
    acquire_state_lock((PyObject*)self, &safe_state);

    if (self->status == 0) {
        /* Finished. */
        release_state_lock((PyObject*)self, &safe_state);
        result = Py_False;
        Py_INCREF(result);
        return result;
    } else if (self->status < 0) {
        /* Internal error. */
        release_state_lock((PyObject*)self, &safe_state);
        set_error(self->status, NULL);
        return NULL;
    }

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
            self->status = do_match(&safe_state, TRUE);
            if (self->status < 0)
                goto error;

            if (self->status == RE_ERROR_SUCCESS) {
                if (!state->zero_width) {
                    /* The current behaviour is to advance one character if the
                     * split was zero-width. Unfortunately, this can give an
                     * incorrect result. GvR wants this behaviour to be
                     * retained so as not to break any existing software which
                     * might rely on it. The correct behaviour is enabled by
                     * setting the 'new' flag.
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
                 * might rely on it. The correct behaviour is enabled by
                 * setting the 'new' flag.
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

        if (self->status == RE_ERROR_FAILURE) {
no_match:
            /* Get segment following last match (even if empty). */
            if (state->reverse)
                result = PySequence_GetSlice(state->string, 0, self->last);
            else
                result = PySequence_GetSlice(state->string, self->last,
                  state->text_length);
            if (!result)
                goto error;
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

    /* Release the state lock. */
    release_state_lock((PyObject*)self, &safe_state);

    return result;

error:
    /* Release the state lock. */
    release_state_lock((PyObject*)self, &safe_state);

    return NULL;
}

/* SplitterObject's 'next' method. */
static PyObject* splitter_next(PyObject* self) {
    PyObject* result;

    result = splitter_split((SplitterObject*)self, NULL);

    if (result == Py_False) {
        /* No match. */
        Py_DECREF(Py_False);
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    return result;
}

/* Returns an iterator for a SplitterObject.
 *
 * The iterator is actually the SplitterObject itself.
 */
static PyObject* splitter_iter(PyObject* self) {
    Py_INCREF(self);
    return self;
}

/* Gets the next result from a splitter iterator. */
static PyObject* splitter_iternext(PyObject* self) {
    PyObject* result;

    result = splitter_split((SplitterObject*)self, NULL);

    if (result == Py_False) {
        /* No match. */
        Py_DECREF(Py_False);
        return NULL;
    }

    return result;
}

/* SplitterObject's methods. */
static PyMethodDef splitter_methods[] = {
    {"split", (PyCFunction)splitter_split, METH_NOARGS},
    {"next", (PyCFunction)splitter_next, METH_NOARGS},
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
    0,                             /* tp_print */
    (getattrfunc)splitter_getattr, /* tp_getattr */
    0,                             /* tp_setattr */
    0,                             /* tp_compare */
    0,                             /* tp_repr */
    0,                             /* tp_as_number */
    0,                             /* tp_as_sequence */
    0,                             /* tp_as_mapping */
    0,                             /* tp_hash */
    0,                             /* tp_call */
    0,                             /* tp_str */
    0,                             /* tp_getattro */
    0,                             /* tp_setattro */
    0,                             /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,            /* tp_flags */
    0,                             /* tp_doc */
    0,                             /* tp_traverse */
    0,                             /* tp_clear */
    0,                             /* tp_richcompare */
    0,                             /* tp_weaklistoffset */
    splitter_iter,                 /* tp_iter */
    splitter_iternext,             /* tp_iternext */
    splitter_methods,              /* tp_methods */
};

/* Creates a new SplitterObject. */
static PyObject* pattern_splitter(PatternObject* pattern, PyObject* args,
  PyObject* kw) {
    /* Create split state object. */
    SplitterObject* self;
    RE_State* state;

    PyObject* string;
    Py_ssize_t maxsplit = 0;
    PyObject* concurrent = Py_None;
    static char* kwlist[] = { "string", "maxsplit", "concurrent", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|nO:splitter", kwlist,
      &string, &maxsplit, &concurrent))
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

    if (!state_init(state, pattern, string, 0, PY_SSIZE_T_MAX, FALSE,
      decode_concurrent(concurrent), TRUE)) {
        PyObject_DEL(self);
        return NULL;
    }

    self->maxsplit = maxsplit;
    self->last = state->reverse ? state->text_length : 0;
    self->split_count = 0;
    self->index = 0;
    self->status = 1;

    /* The MatchObject, and therefore repeated captures, will never be visible.
     */
    state->save_captures = FALSE;

    return (PyObject*) self;
}

/* PatternObject's 'match' method. */
static PyObject* pattern_match(PatternObject* self, PyObject* args, PyObject*
  kw) {
    Py_ssize_t start;
    Py_ssize_t end;
    RE_State state;
    RE_SafeState safe_state;
    int status;
    PyObject* match;

    PyObject* string;
    PyObject* pos = Py_None;
    PyObject* endpos = Py_None;
    PyObject* concurrent = Py_None;
    static char* kwlist[] = { "string", "pos", "endpos", "concurrent", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|OOO:match", kwlist, &string,
      &pos, &endpos, &concurrent))
        return NULL;

    start = as_string_index(pos, 0);
    end = as_string_index(endpos, PY_SSIZE_T_MAX);
    if (PyErr_Occurred())
        return NULL;

    if (!state_init(&state, self, string, start, end, FALSE,
      decode_concurrent(concurrent), FALSE))
        return NULL;

    /* Initialise the "safe state" structure. */
    safe_state.re_state = &state;
    safe_state.thread_state = NULL;

    status = do_match(&safe_state, FALSE);
    if (status < 0) {
        state_fini(&state);
        return NULL;
    }

    match = pattern_new_match(self, &state, status);

    state_fini(&state);

    return match;
}

/* Gets the limits of the matching. */
Py_LOCAL_INLINE(BOOL) get_limits(PyObject* pos, PyObject* endpos, Py_ssize_t
  length, Py_ssize_t* start, Py_ssize_t* end) {
    Py_ssize_t s;
    Py_ssize_t e;

    s = as_string_index(pos, 0);
    e = as_string_index(endpos, PY_SSIZE_T_MAX);
    if (PyErr_Occurred())
        return FALSE;

    /* Adjust boundaries. */
    if (s < 0)
        s += length;
    if (s < 0)
        s = 0;
    else if (s > length)
        s = length;

    if (e < 0)
        e += length;
    if (e < 0)
        e = 0;
    else if (e > length)
        e = length;

    *start = s;
    *end = e;

    return TRUE;
}

/* PatternObject's 'search' method. */
static PyObject* pattern_search(PatternObject* self, PyObject* args, PyObject*
  kw) {
    void* characters;
    Py_ssize_t length;
    Py_ssize_t charsize;
    Py_ssize_t start;
    Py_ssize_t end;
    RE_State state;
    RE_SafeState safe_state;
    int status;
    PyObject* match;

    PyObject* string;
    PyObject* pos = Py_None;
    PyObject* endpos = Py_None;
    PyObject* concurrent = Py_None;
    static char* kwlist[] = { "string", "pos", "endpos", "concurrent", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|OOO:search", kwlist, &string,
      &pos, &endpos, &concurrent))
        return NULL;

    /* Get the string. */
    if (!get_string(string, &characters, &length, &charsize))
        return NULL;

    /* Get the limits of the search. */
    if (!get_limits(pos, endpos, length, &start, &end))
        return NULL;

    /* If the pattern is too long for the string, then take a shortcut. */
    if ((Py_ssize_t)self->min_width > end - start) {
        /* No match. */
        Py_INCREF(Py_None);
        return Py_None;
    }

    if (!state_init_2(&state, self, string, characters, length, charsize,
      start, end, FALSE, decode_concurrent(concurrent), FALSE))
        return NULL;

    /* Initialise the "safe state" structure. */
    safe_state.re_state = &state;
    safe_state.thread_state = NULL;

    status = do_match(&safe_state, TRUE);
    if (status < 0) {
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
Py_LOCAL_INLINE(PyObject*) get_sub_replacement(PyObject* item, PyObject*
  string, RE_State* state, Py_ssize_t group_count) {
    Py_ssize_t index;

    if (PyUnicode_Check(item) || PyString_Check(item)) {
        /* It's a literal, which can be added directly to the list. */
        Py_INCREF(item);
        return item;
    }

    /* Is it a group reference? */
    index = as_group_index(item);
    if (PyErr_Occurred()) {
        /* Not a group either! */
        set_error(RE_ERROR_REPLACEMENT, NULL);
        return NULL;
    }

    if (index == 0) {
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
    } else if (1 <= index && index <= group_count) {
        /* A group. */
        RE_GroupSpan* span;

        span = &state->groups[index - 1].span;
        if (span->start < 0 || span->start == span->end) {
            /* The group didn't match or is "", so return None for "". */
            Py_INCREF(Py_None);
            return Py_None;
        }

        return PySequence_GetSlice(string, span->start, span->end);
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
Py_LOCAL_INLINE(Py_ssize_t) check_template(PyObject* str_template) {
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
Py_LOCAL_INLINE(PyObject*) pattern_subx(PatternObject* self, PyObject*
  str_template, PyObject* string, Py_ssize_t maxsub, Py_ssize_t subn, PyObject*
  pos, PyObject* endpos, int concurrent) {
    void* characters;
    Py_ssize_t length;
    Py_ssize_t charsize;
    Py_ssize_t start;
    Py_ssize_t end;
    BOOL is_callable;
    BOOL is_literal;
    PyObject* replacement;
    RE_State state;
    RE_SafeState safe_state;
    JoinInfo join_info;
    Py_ssize_t sub_count;
    Py_ssize_t last;
    PyObject* item;
    Py_ssize_t end_pos;

    /* Get the string. */
    if (!get_string(string, &characters, &length, &charsize))
        return NULL;

    /* Get the limits of the search. */
    if (!get_limits(pos, endpos, length, &start, &end))
        return NULL;

    /* If the pattern is too long for the string, then take a shortcut. */
    if ((Py_ssize_t)self->min_width > end - start) {
        Py_INCREF(string);

        if (subn)
            return Py_BuildValue("Nn", string, 0);

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
            replacement = call(RE_MODULE, "compile_replacement", PyTuple_Pack(2,
              self, str_template));
            if (!replacement)
                return NULL;
        }
    }

    if (!state_init_2(&state, self, string, characters, length, charsize,
      start, end, FALSE, concurrent, FALSE)) {
        Py_DECREF(replacement);
        return NULL;
    }

    /* Initialise the "safe state" structure. */
    safe_state.re_state = &state;
    safe_state.thread_state = NULL;

    if (!is_callable)
        /* The MatchObject, and therefore repeated captures, will never be
         * visible.
         */
        state.save_captures = FALSE;

    join_info.item = NULL;
    join_info.list = NULL;
    join_info.reversed = state.reverse;

    sub_count = 0;
    last = state.reverse ? state.text_length : 0;
    while (sub_count < maxsub) {
        int status;

        status = do_match(&safe_state, TRUE);
        if (status < 0)
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
        if (is_literal) {
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
        } else if (is_callable) {
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

        /* Continue from where we left off, but don't allow 2 contiguous
         * zero-width matches.
         */
        state.must_advance = state.match_pos == state.text_pos;
        last = state.text_pos;
    }

    /* Get the segment following the last match. We use 'length' instead of
     * 'text_length' because the latter is truncated to 'slice_end', a
     * documented idiosyncracy of the 're' module.
     */
    end_pos = state.reverse ? 0 : length;
    if (last != end_pos) {
        int status;

        /* The segment is part of the original string. */
        if (state.reverse)
            item = PySequence_GetSlice(string, 0, last);
        else
            item = PySequence_GetSlice(string, last, length);
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
        return Py_BuildValue("Nn", item, sub_count);

    return item;

error:
    Py_XDECREF(join_info.list);
    Py_XDECREF(join_info.item);
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
    PyObject* pos = Py_None;
    PyObject* endpos = Py_None;
    PyObject* concurrent = Py_None;
    static char* kwlist[] = { "repl", "string", "count", "pos", "endpos",
      "concurrent", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "OO|nOOO:sub", kwlist,
      &ptemplate, &string, &count, &pos, &endpos, &concurrent))
        return NULL;

    return pattern_subx(self, ptemplate, string, count, 0, pos, endpos,
      decode_concurrent(concurrent));
}

/* PatternObject's 'subn' method. */
static PyObject* pattern_subn(PatternObject* self, PyObject* args, PyObject*
  kw) {
    PyObject* ptemplate;
    PyObject* string;
    Py_ssize_t count = 0;
    PyObject* pos = Py_None;
    PyObject* endpos = Py_None;
    PyObject* concurrent = Py_None;
    static char* kwlist[] = { "repl", "string", "count", "pos", "endpos",
      "concurrent", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "OO|nOOO:subn", kwlist,
      &ptemplate, &string, &count, &pos, &endpos, &concurrent))
        return NULL;

    return pattern_subx(self, ptemplate, string, count, 1, pos, endpos,
      decode_concurrent(concurrent));
}

/* PatternObject's 'split' method. */
static PyObject* pattern_split(PatternObject* self, PyObject* args, PyObject*
  kw) {
    RE_State state;
    RE_SafeState safe_state;
    PyObject* list;
    PyObject* item;
    int status;
    Py_ssize_t split_count;
    Py_ssize_t g;
    Py_ssize_t start_pos;
    Py_ssize_t end_pos;
    Py_ssize_t step;
    Py_ssize_t last;

    PyObject* string;
    Py_ssize_t maxsplit = 0;
    PyObject* concurrent = Py_None;
    static char* kwlist[] = { "string", "maxsplit", "concurrent", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|nO:split", kwlist, &string,
      &maxsplit, &concurrent))
        return NULL;

    if (maxsplit == 0)
        maxsplit = PY_SSIZE_T_MAX;

    if (!state_init(&state, self, string, 0, PY_SSIZE_T_MAX, FALSE,
      decode_concurrent(concurrent), FALSE))
        return NULL;

    /* Initialise the "safe state" structure. */
    safe_state.re_state = &state;
    safe_state.thread_state = NULL;

    /* The MatchObject, and therefore repeated captures, will never be visible.
     */
    state.save_captures = FALSE;

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
        status = do_match(&safe_state, TRUE);
        if (status < 0)
            goto error;

        if (status == 0)
            /* No more matches. */
            break;

        if (!state.zero_width) {
            /* The current behaviour is to advance one character if the split
             * was zero-width. Unfortunately, this can give an incorrect
             * result. GvR wants this behaviour to be retained so as not to
             * break any existing software which might rely on it. The correct
             * behaviour is enabled by setting the 'new' flag.
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
        for (g = 1; g <= self->group_count; g++) {
            item = state_get_group(&state, g, string, FALSE);
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
         * is enabled by setting the 'new' flag.
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
    return splitter;
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
    RE_SafeState safe_state;
    PyObject* list;
    Py_ssize_t step;
    int status;
    Py_ssize_t g;
    Py_ssize_t b;
    Py_ssize_t e;

    PyObject* string;
    PyObject* pos = Py_None;
    PyObject* endpos = Py_None;
    Py_ssize_t overlapped = FALSE;
    PyObject* concurrent = Py_None;
    static char* kwlist[] = { "string", "pos", "endpos", "overlapped",
      "concurrent", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|OOnO:findall", kwlist,
      &string, &pos, &endpos, &overlapped, &concurrent))
        return NULL;

    start = as_string_index(pos, 0);
    end = as_string_index(endpos, PY_SSIZE_T_MAX);
    if (PyErr_Occurred())
        return NULL;

    if (!state_init(&state, self, string, start, end, overlapped != 0,
      decode_concurrent(concurrent), FALSE))
        return NULL;

    /* Initialise the "safe state" structure. */
    safe_state.re_state = &state;
    safe_state.thread_state = NULL;

    /* The MatchObject, and therefore repeated captures, will never be visible.
     */
    state.save_captures = FALSE;

    list = PyList_New(0);
    if (!list) {
        state_fini(&state);
        return NULL;
    }

    step = state.reverse ? -1 : 1;
    while (state.slice_start <= state.text_pos && state.text_pos <=
      state.slice_end) {
        PyObject* item;

        status = do_match(&safe_state, TRUE);
        if (status < 0)
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
            for (g = 0; g < self->group_count; g++) {
                PyObject* o = state_get_group(&state, g + 1, string, TRUE);
                if (!o) {
                    Py_DECREF(item);
                    goto error;
                }
                PyTuple_SET_ITEM(item, g, o);
            }
            break;
        }

        status = PyList_Append(list, item);
        Py_DECREF(item);
        if (status < 0)
            goto error;

        if (state.overlapped) {
            /* Advance one character. */
            state.text_pos = state.match_pos + step;
            state.must_advance = FALSE;
        } else
            /* Continue from where we left off, but don't allow 2 contiguous
             * zero-width matches.
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
    return pattern_scanner(pattern, args, kw);
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
    "match(string, pos=None, endpos=None, concurrent=None) --> MatchObject or None.\n\
    Match zero or more characters at the beginning of the string.");

PyDoc_STRVAR(pattern_search_doc,
    "search(string, pos=None, endpos=None, concurrent=None) --> MatchObject or None.\n\
    Search through string looking for a match, and return a corresponding\n\
    match object instance.  Return None if no match is found.");

PyDoc_STRVAR(pattern_sub_doc,
    "sub(repl, string, count=0, flags=0, pos=None, endpos=None, concurrent=None) --> newstring\n\
    Return the string obtained by replacing the leftmost (or rightmost with a\n\
    reverse pattern) non-overlapping occurrences of pattern in string by the\n\
    replacement repl.");

PyDoc_STRVAR(pattern_subn_doc,
    "subn(repl, string, count=0, flags=0, pos=None, endpos=None, concurrent=None) --> (newstring, number of subs)\n\
    Return the tuple (new_string, number_of_subs_made) found by replacing the\n\
    leftmost (or rightmost with a reverse pattern) non-overlapping occurrences\n\
    of pattern with the replacement repl.");

PyDoc_STRVAR(pattern_split_doc,
    "split(string, string, maxsplit=0, concurrent=None) --> list.\n\
    Split string by the occurrences of pattern.");

PyDoc_STRVAR(pattern_splititer_doc,
    "splititer(string, maxsplit=0, concurrent=None) --> iterator.\n\
    Return an iterator yielding the parts of a split string.");

PyDoc_STRVAR(pattern_findall_doc,
    "findall(string, pos=None, endpos=None, overlapped=False, concurrent=None) --> list.\n\
    Return a list of all matches of pattern in string.  The matches may be\n\
    overlapped if overlapped is True.");

PyDoc_STRVAR(pattern_finditer_doc,
    "finditer(string, pos=None, endpos=None, overlapped=False, concurrent=None) --> iterator.\n\
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
        if (node->status & RE_STATUS_STRING) {
            re_dealloc(node->string.bad_character_offset);
            re_dealloc(node->string.good_suffix_offset);
        }
        re_dealloc(node);
    }
    re_dealloc(self->node_list);

    /* Discard the group info. */
    re_dealloc(self->group_info);

    /* Discard the repeat info. */
    re_dealloc(self->repeat_info);

    dealloc_groups(self->groups_storage, self->group_count);

    dealloc_repeats(self->repeats_storage, self->repeat_count);

    re_dealloc(self->saved_groups_storage);

    if (self->weakreflist)
        PyObject_ClearWeakRefs((PyObject*)self);
    Py_XDECREF(self->pattern);
    Py_XDECREF(self->groupindex);
    Py_XDECREF(self->indexgroup);
    Py_DECREF(self->ref_lists);
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
    (destructor)pattern_dealloc,          /* tp_dealloc */
    0,                                    /* tp_print */
    (getattrfunc)pattern_getattr,         /* tp_getattr */
    0,                                    /* tp_setattr */
    0,                                    /* tp_compare */
    0,                                    /* tp_repr */
    0,                                    /* tp_as_number */
    0,                                    /* tp_as_sequence */
    0,                                    /* tp_as_mapping */
    0,                                    /* tp_hash */
    0,                                    /* tp_call */
    0,                                    /* tp_str */
    0,                                    /* tp_getattro */
    0,                                    /* tp_setattro */
    0,                                    /* tp_as_buffer */
    Py_TPFLAGS_HAVE_WEAKREFS,             /* tp_flags */
    pattern_doc,                          /* tp_doc */
    0,                                    /* tp_traverse */
    0,                                    /* tp_clear */
    0,                                    /* tp_richcompare */
    offsetof(PatternObject, weakreflist), /* tp_weaklistoffset */
    0,                                    /* tp_iter */
    0,                                    /* tp_iternext */
    pattern_methods,                      /* tp_methods */
};

/* Building the nodes is made simpler by allowing branches to have a single
 * exit. These need to be removed.
 */
Py_LOCAL_INLINE(void) skip_one_way_branches(PatternObject* pattern) {
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
            if (next && next->op == RE_OP_BRANCH &&
              !next->nonstring.next_2.node) {
                node->next_1.node = next->next_1.node;
                modified = TRUE;
            }

            /* Check the second destination. */
            next = node->nonstring.next_2.node;
            if (next && next->op == RE_OP_BRANCH &&
              !next->nonstring.next_2.node) {
                node->nonstring.next_2.node = next->next_1.node;
                modified = TRUE;
            }
        }
    } while (modified);

    /* The start node might be a 1-way branch. Skip over it because it'll be
     * removed. It might even be the first in a chain.
     */
    while (pattern->start_node->op == RE_OP_BRANCH &&
      !pattern->start_node->nonstring.next_2.node)
        pattern->start_node = pattern->start_node->next_1.node;
}

/* Adds guards to repeats which are followed by a reference to a group.
 *
 * Returns whether a guard was added for a node at or after the given node.
 */
Py_LOCAL_INLINE(RE_STATUS_T) add_guards(PatternObject* pattern, RE_Node* node)
  {
    RE_STATUS_T result;

    result = RE_STATUS_NEITHER;

    for (;;) {
        if (node->status & RE_STATUS_VISITED_AG)
            return node->status & (RE_STATUS_REPEAT | RE_STATUS_REF);

        switch (node->op) {
        case RE_OP_ATOMIC:
        case RE_OP_LOOKAROUND:
        {
            RE_STATUS_T body_result;
            RE_STATUS_T tail_result;
            RE_STATUS_T status;

            body_result = add_guards(pattern, node->nonstring.next_2.node);
            tail_result = add_guards(pattern, node->next_1.node);
            status = max3(result, body_result, tail_result);
            node->status = RE_STATUS_VISITED_AG | status;
            return status;
        }
        case RE_OP_BRANCH:
        {
            RE_STATUS_T branch_1_result;
            RE_STATUS_T branch_2_result;
            RE_STATUS_T status;

            branch_1_result = add_guards(pattern, node->next_1.node);
            branch_2_result = add_guards(pattern, node->nonstring.next_2.node);
            status = max3(result, branch_1_result, branch_2_result);
            node->status = RE_STATUS_VISITED_AG | status;
            return status;
        }
        case RE_OP_END_GREEDY_REPEAT:
        case RE_OP_END_LAZY_REPEAT:
            node->status = RE_STATUS_VISITED_AG | result;
            return result;
        case RE_OP_GREEDY_REPEAT:
        case RE_OP_LAZY_REPEAT:
        {
            BOOL limited;
            RE_STATUS_T body_result;
            RE_STATUS_T tail_result;
            RE_RepeatInfo* repeat_info;
            RE_STATUS_T status;

            limited = ~node->values[2] != 0;
            if (limited)
                body_result = RE_STATUS_LIMITED;
            else
                body_result = add_guards(pattern, node->next_1.node);
            tail_result = add_guards(pattern, node->nonstring.next_2.node);

            repeat_info = &pattern->repeat_info[node->values[0]];
            if (body_result == RE_STATUS_REPEAT && !limited)
                repeat_info->status |= RE_STATUS_BODY;
            if (tail_result == RE_STATUS_REPEAT)
                repeat_info->status |= RE_STATUS_TAIL;
            if (limited)
                result = max2(result, RE_STATUS_LIMITED);
            else
                result = max2(result, RE_STATUS_REPEAT);
            status = max3(result, body_result, tail_result);
            node->status = RE_STATUS_VISITED_AG | status;
            return status;
        }
        case RE_OP_GREEDY_REPEAT_ONE:
        case RE_OP_LAZY_REPEAT_ONE:
        {
            BOOL limited;
            RE_STATUS_T tail_result;
            RE_RepeatInfo* repeat_info;
            RE_STATUS_T status;

            limited = ~node->values[2] != 0;
            tail_result = add_guards(pattern, node->next_1.node);

            repeat_info = &pattern->repeat_info[node->values[0]];
            if (!limited)
                repeat_info->status |= RE_STATUS_BODY;
            if (tail_result == RE_STATUS_REPEAT)
                repeat_info->status |= RE_STATUS_TAIL;
            if (limited)
                result = max2(result, RE_STATUS_LIMITED);
            else
                result = max2(result, RE_STATUS_REPEAT);
            status = max3(result, RE_STATUS_REPEAT, tail_result);
            node->status = RE_STATUS_VISITED_AG | status;
            return status;
        }
        case RE_OP_GROUP_EXISTS:
        {
            RE_STATUS_T branch_1_result;
            RE_STATUS_T branch_2_result;
            RE_STATUS_T status;

            branch_1_result = add_guards(pattern, node->next_1.node);
            branch_2_result = add_guards(pattern, node->nonstring.next_2.node);
            status = max3(result, branch_1_result, branch_2_result);
            node->status = RE_STATUS_VISITED_AG | status;
            return status;
        }
        case RE_OP_REF_GROUP:
        case RE_OP_REF_GROUP_IGN:
        case RE_OP_REF_GROUP_IGN_REV:
        case RE_OP_REF_GROUP_REV:
            result = RE_STATUS_REF;
            node = node->next_1.node;
            break;
        case RE_OP_SUCCESS:
            node->status = RE_STATUS_VISITED_AG | result;
            return result;
        default:
            node = node->next_1.node;
            break;
        }
    }
}

/* Adds a repeat index to a node's values unless it's already present.
 *
 * 'offset' is the offset of the index count within the values.
 */
Py_LOCAL_INLINE(BOOL) add_repeat_index(RE_Node* node, size_t offset, size_t
  index) {
    size_t index_count;
    size_t first_index;
    size_t i;
    RE_CODE* new_values;

    if (!node)
        return TRUE;

    index_count = node->values[offset];
    first_index = offset + 1;
    /* Is the index already present? */
    for (i = 0; i < index_count; i++) {
        if (node->values[first_index + i] == index)
            return TRUE;
    }

    /* Allocate more space for the new index. */
    new_values = re_realloc(node->values, (node->value_count + 1) *
      sizeof(RE_CODE));
    if (!new_values)
        return FALSE;
    ++node->value_count;
    node->values = new_values;

    node->values[first_index + node->values[offset]++] = index;
    return TRUE;
}

/* Records the index of every repeat within atomic subpatterns and lookarounds.
 */
Py_LOCAL_INLINE(BOOL) record_subpattern_repeats(RE_Node* parent_node, size_t
  offset, RE_Node* node) {
    for (;;) {
        if (node->status & RE_STATUS_VISITED_REP)
            return TRUE;

        node->status |= RE_STATUS_VISITED_REP;

        switch (node->op) {
        case RE_OP_ATOMIC:
            if (!record_subpattern_repeats(node, 0,
              node->nonstring.next_2.node))
                return FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_LOOKAROUND:
            if (!record_subpattern_repeats(node, 1,
              node->nonstring.next_2.node))
                return FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_BRANCH:
            if (!record_subpattern_repeats(parent_node, offset,
              node->next_1.node))
                return FALSE;
            node = node->nonstring.next_2.node;
            break;
        case RE_OP_END_GREEDY_REPEAT:
        case RE_OP_END_LAZY_REPEAT:
            return TRUE;
        case RE_OP_GREEDY_REPEAT:
        case RE_OP_LAZY_REPEAT:
            /* Record the index. */
            if (!add_repeat_index(parent_node, offset, node->values[0]))
                return FALSE;
            if (!record_subpattern_repeats(parent_node, offset,
              node->next_1.node))
                return FALSE;
            node = node->nonstring.next_2.node;
            break;
        case RE_OP_GREEDY_REPEAT_ONE:
        case RE_OP_LAZY_REPEAT_ONE:
            /* Record the index. */
            if (!add_repeat_index(parent_node, offset, node->values[0]))
                return FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_GROUP_EXISTS:
            if (!record_subpattern_repeats(parent_node, offset,
              node->next_1.node))
                return FALSE;
            node = node->nonstring.next_2.node;
            break;
        case RE_OP_REF_GROUP:
        case RE_OP_REF_GROUP_IGN:
        case RE_OP_REF_GROUP_IGN_REV:
        case RE_OP_REF_GROUP_REV:
            node = node->next_1.node;
            break;
        case RE_OP_SUCCESS:
            return TRUE;
        default:
            node = node->next_1.node;
            break;
        }
    }
}

/* Marks nodes which are being used as used. */
Py_LOCAL_INLINE(void) use_nodes(RE_Node* node) {
    while (node && !(node->status & RE_STATUS_USED)) {
        node->status |= RE_STATUS_USED;
        if (!(node->status & RE_STATUS_STRING)) {
            if (node->nonstring.next_2.node)
                use_nodes(node->nonstring.next_2.node);
        }
        node = node->next_1.node;
    }
}

/* Discards any unused nodes.
 *
 * Optimising the nodes might result in some nodes no longer being used.
 */
Py_LOCAL_INLINE(void) discard_unused_nodes(PatternObject* pattern) {
    Py_ssize_t new_count;
    Py_ssize_t i;

    /* Mark the nodes which are being used. */
    use_nodes(pattern->start_node);

    new_count = 0;
    for (i = 0; i < pattern->node_count; i++) {
        RE_Node* node;

        node = pattern->node_list[i];
        if (node->status & RE_STATUS_USED)
            pattern->node_list[new_count++] = node;
        else {
            re_dealloc(node->values);
            if (node->status & RE_STATUS_STRING) {
                re_dealloc(node->string.bad_character_offset);
                re_dealloc(node->string.good_suffix_offset);
            }
            re_dealloc(node);
        }
    }

    pattern->node_count = new_count;
}

/* Marks all the group which are named. */
Py_LOCAL_INLINE(BOOL) mark_named_groups(PatternObject* pattern) {
    Py_ssize_t i;

    for (i = 0; i < pattern->group_count; i++) {
        RE_GroupInfo* group_info;
        PyObject* index;

        group_info = &pattern->group_info[i];
        index = Py_BuildValue("n", i + 1);
        if (!index)
            return FALSE;
        group_info->has_name = PyDict_Contains(pattern->indexgroup, index);
        Py_DECREF(index);
    }

    return TRUE;
}

/* Gets the test node. */
Py_LOCAL_INLINE(void) set_test_node(RE_NextNode* next) {
    RE_Node* node = next->node;
    RE_Node* test;

    next->test = node;
    next->match_next = node;
    next->match_step = 0;

    if (!node)
        return;

    test = node;
    while (test->op == RE_OP_END_GROUP || test->op == RE_OP_START_GROUP)
        test = test->next_1.node;

    next->test = test;

    if (test != node)
        return;

    switch (test->op) {
    case RE_OP_ANY:
    case RE_OP_ANY_ALL:
    case RE_OP_ANY_ALL_REV:
    case RE_OP_ANY_REV:
    case RE_OP_ANY_U:
    case RE_OP_ANY_U_REV:
    case RE_OP_BIG_BITSET:
    case RE_OP_BIG_BITSET_REV:
    case RE_OP_BOUNDARY:
    case RE_OP_CHARACTER:
    case RE_OP_CHARACTER_IGN:
    case RE_OP_CHARACTER_IGN_REV:
    case RE_OP_CHARACTER_REV:
    case RE_OP_DEFAULT_BOUNDARY:
    case RE_OP_END_OF_LINE:
    case RE_OP_END_OF_LINE_U:
    case RE_OP_END_OF_STRING:
    case RE_OP_END_OF_STRING_LINE:
    case RE_OP_END_OF_STRING_LINE_U:
    case RE_OP_GRAPHEME_BOUNDARY:
    case RE_OP_PROPERTY:
    case RE_OP_PROPERTY_REV:
    case RE_OP_SEARCH_ANCHOR:
    case RE_OP_SET_DIFF:
    case RE_OP_SET_DIFF_REV:
    case RE_OP_SET_INTER:
    case RE_OP_SET_INTER_REV:
    case RE_OP_SET_SYM_DIFF:
    case RE_OP_SET_SYM_DIFF_REV:
    case RE_OP_SET_UNION:
    case RE_OP_SET_UNION_REV:
    case RE_OP_SMALL_BITSET:
    case RE_OP_SMALL_BITSET_REV:
    case RE_OP_START_OF_LINE:
    case RE_OP_START_OF_LINE_U:
    case RE_OP_START_OF_STRING:
    case RE_OP_STRING:
    case RE_OP_STRING_IGN:
    case RE_OP_STRING_IGN_REV:
    case RE_OP_STRING_REV:
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
Py_LOCAL_INLINE(void) set_test_nodes(PatternObject* pattern) {
    RE_Node** node_list;
    Py_ssize_t i;

    node_list = pattern->node_list;
    for (i = 0; i < pattern->node_count; i++) {
        RE_Node* node;

        node = node_list[i];
        set_test_node(&node->next_1);
        if (!(node->status & RE_STATUS_STRING))
            set_test_node(&node->nonstring.next_2);
    }
}

/* Checks whether the matcher should do the initial check.
 *
 * It sets pattern->do_check accordingly and returns TRUE when it has done so.
 */
Py_LOCAL_INLINE(BOOL) should_do_check(PatternObject* pattern, RE_Node* node,
  BOOL early) {
    for (;;) {
        switch (node->op) {
        case RE_OP_ANY:
        case RE_OP_ANY_ALL:
        case RE_OP_ANY_ALL_REV:
        case RE_OP_ANY_REV:
        case RE_OP_ANY_U:
        case RE_OP_ANY_U_REV:
        case RE_OP_BIG_BITSET:
        case RE_OP_BIG_BITSET_REV:
        case RE_OP_BOUNDARY:
        case RE_OP_DEFAULT_BOUNDARY:
        case RE_OP_END_OF_LINE:
        case RE_OP_END_OF_LINE_U:
        case RE_OP_END_OF_STRING:
        case RE_OP_END_OF_STRING_LINE:
        case RE_OP_END_OF_STRING_LINE_U:
        case RE_OP_GRAPHEME_BOUNDARY:
        case RE_OP_PROPERTY:
        case RE_OP_PROPERTY_REV:
        case RE_OP_SET_DIFF:
        case RE_OP_SET_DIFF_REV:
        case RE_OP_SET_INTER:
        case RE_OP_SET_INTER_REV:
        case RE_OP_SET_SYM_DIFF:
        case RE_OP_SET_SYM_DIFF_REV:
        case RE_OP_SET_UNION:
        case RE_OP_SET_UNION_REV:
        case RE_OP_SMALL_BITSET:
        case RE_OP_SMALL_BITSET_REV:
        case RE_OP_START_OF_LINE:
        case RE_OP_START_OF_LINE_U:
        case RE_OP_START_OF_STRING:
        case RE_OP_SEARCH_ANCHOR:
            early = FALSE;
            node = node->next_1.node;
            break;
        case RE_OP_ATOMIC:
            early = FALSE;
            if (should_do_check(pattern, node->nonstring.next_2.node, early))
                return TRUE;
            node = node->next_1.node;
            break;
        case RE_OP_END_GROUP:
        case RE_OP_START_GROUP:
            node = node->next_1.node;
            break;
        case RE_OP_BRANCH:
            early = FALSE;
            if (should_do_check(pattern, node->next_1.node, early))
                return TRUE;
            node = node->nonstring.next_2.node;
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
            node = node->nonstring.next_2.node;
            break;
        case RE_OP_GREEDY_REPEAT:
        case RE_OP_LAZY_REPEAT:
            early = FALSE;
            if (node->values[1] >= 1 && should_do_check(pattern,
              node->nonstring.next_2.node, early))
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
            node = node->nonstring.next_2.node;
            break;
        case RE_OP_LOOKAROUND:
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
Py_LOCAL_INLINE(BOOL) optimise_pattern(PatternObject* pattern) {
    /* Building the nodes is made simpler by allowing branches to have a single
     * exit. These need to be removed.
     */
    skip_one_way_branches(pattern);

    /* Add position guards for repeat bodies containing a reference to a group
     * or repeat tails followed at some point by a reference to a group.
     */
    add_guards(pattern, pattern->start_node);

    /* Record the index of repeats within the body of atomic and lookaround
     * nodes.
     */
    if (!record_subpattern_repeats(NULL, 0, pattern->start_node))
        return FALSE;

    /* Discard any unused nodes. */
    discard_unused_nodes(pattern);

    /* Set the test nodes. */
    set_test_nodes(pattern);

    /* Mark all the group that are named. */
    if (!mark_named_groups(pattern))
        return FALSE;

    /* Check whether an initial check should be done when matching. */
    pattern->do_check = FALSE;
    should_do_check(pattern, pattern->start_node, TRUE);
    if (pattern->do_check) {
        pattern->check_node = next_check(pattern->start_node);
        if (!pattern->check_node)
            pattern->do_check = FALSE;
    }

    return TRUE;
}

/* Creates a new pattern node. */
Py_LOCAL_INLINE(RE_Node*) create_node(PatternObject* pattern, BYTE op, BOOL
  match, Py_ssize_t step, Py_ssize_t value_count) {
    RE_Node* node;

    node = (RE_Node*)re_alloc(sizeof(*node));
    if (!node)
        return NULL;
    memset(node, 0, sizeof(RE_Node));

    node->value_capacity = value_count;
    node->value_count = value_count;
    node->values = (RE_CODE*)re_alloc(node->value_capacity * sizeof(RE_CODE));
    if (!node->values)
        goto error;

    node->op = op;
    node->match = match & RE_MATCH_OP;
    node->step = step;

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
Py_LOCAL_INLINE(void) add_node(RE_Node* node_1, RE_Node* node_2) {
    if (!node_1->next_1.node)
        node_1->next_1.node = node_2;
    else
        node_1->nonstring.next_2.node = node_2;
}

/* Ensures that the entry for a group's details actually exists. */
Py_LOCAL_INLINE(BOOL) ensure_group(PatternObject* pattern, Py_ssize_t group) {
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
Py_LOCAL_INLINE(BOOL) record_ref_group(PatternObject* pattern, Py_ssize_t
  group) {
    if (!ensure_group(pattern, group))
        return FALSE;

    pattern->group_info[group - 1].referenced = TRUE;

    return TRUE;
}

/* Records that there's a new group. */
Py_LOCAL_INLINE(BOOL) record_group(PatternObject* pattern, Py_ssize_t group) {
    if (!ensure_group(pattern, group))
        return FALSE;

    if (group >= 1)
        pattern->group_info[group - 1].end_index = pattern->group_count;

    return TRUE;
}

/* Records that a group has closed. */
Py_LOCAL_INLINE(void) record_group_end(PatternObject* pattern, Py_ssize_t
  group) {
    if (group >= 1)
        pattern->group_info[group - 1].end_index = ++pattern->group_end_index;
}

/* Checks whether a node matches one and only one character. */
Py_LOCAL_INLINE(BOOL) sequence_matches_one(RE_Node* node) {
    while (node->op == RE_OP_BRANCH && !node->nonstring.next_2.node)
        node = node->next_1.node;

    if (node->next_1.node)
        return FALSE;

    switch (node->op) {
    case RE_OP_ANY:
    case RE_OP_ANY_ALL:
    case RE_OP_ANY_ALL_REV:
    case RE_OP_ANY_REV:
    case RE_OP_ANY_U:
    case RE_OP_ANY_U_REV:
    case RE_OP_BIG_BITSET:
    case RE_OP_BIG_BITSET_REV:
    case RE_OP_CHARACTER:
    case RE_OP_CHARACTER_IGN:
    case RE_OP_CHARACTER_IGN_REV:
    case RE_OP_CHARACTER_REV:
    case RE_OP_PROPERTY:
    case RE_OP_PROPERTY_REV:
    case RE_OP_SET_DIFF:
    case RE_OP_SET_DIFF_REV:
    case RE_OP_SET_INTER:
    case RE_OP_SET_INTER_REV:
    case RE_OP_SET_SYM_DIFF:
    case RE_OP_SET_SYM_DIFF_REV:
    case RE_OP_SET_UNION:
    case RE_OP_SET_UNION_REV:
    case RE_OP_SMALL_BITSET:
    case RE_OP_SMALL_BITSET_REV:
        return TRUE;
    default:
        return FALSE;
    }
}

/* Records a repeat. */
Py_LOCAL_INLINE(BOOL) record_repeat(PatternObject* pattern, int index, size_t
  repeat_depth) {
    Py_ssize_t old_capacity;
    Py_ssize_t new_capacity;

    /* Increase the storage capacity to include the new entry if it's
     * insufficient.
     */
    old_capacity = pattern->repeat_info_capacity;
    new_capacity = pattern->repeat_info_capacity;
    while (index >= new_capacity)
        new_capacity += 16;

    if (new_capacity > old_capacity) {
        RE_RepeatInfo* new_repeat_info;

        new_repeat_info = (RE_RepeatInfo*)re_realloc(pattern->repeat_info,
          new_capacity * sizeof(RE_RepeatInfo));
        if (!new_repeat_info)
            return FALSE;
        memset(new_repeat_info + old_capacity, 0, (new_capacity - old_capacity)
          * sizeof(RE_RepeatInfo));

        pattern->repeat_info = new_repeat_info;
        pattern->repeat_info_capacity = new_capacity;
    }

    if (index >= pattern->repeat_count)
        pattern->repeat_count = index + 1;

    if (repeat_depth > 0)
        pattern->repeat_info[index].status |= RE_STATUS_INNER;

    return TRUE;
}

Py_LOCAL_INLINE(Py_ssize_t) get_step(RE_CODE op) {
    switch (op) {
    case RE_OP_ANY:
    case RE_OP_ANY_ALL:
    case RE_OP_ANY_U:
    case RE_OP_BIG_BITSET:
    case RE_OP_CHARACTER:
    case RE_OP_CHARACTER_IGN:
    case RE_OP_PROPERTY:
    case RE_OP_SET_DIFF:
    case RE_OP_SET_INTER:
    case RE_OP_SET_SYM_DIFF:
    case RE_OP_SET_UNION:
    case RE_OP_SMALL_BITSET:
    case RE_OP_STRING:
    case RE_OP_STRING_IGN:
         return 1;
    case RE_OP_ANY_ALL_REV:
    case RE_OP_ANY_REV:
    case RE_OP_ANY_U_REV:
    case RE_OP_BIG_BITSET_REV:
    case RE_OP_CHARACTER_IGN_REV:
    case RE_OP_CHARACTER_REV:
    case RE_OP_PROPERTY_REV:
    case RE_OP_SET_DIFF_REV:
    case RE_OP_SET_INTER_REV:
    case RE_OP_SET_SYM_DIFF_REV:
    case RE_OP_SET_UNION_REV:
    case RE_OP_SMALL_BITSET_REV:
    case RE_OP_STRING_IGN_REV:
    case RE_OP_STRING_REV:
        return -1;
    }

    return 0;
}

Py_LOCAL_INLINE(BOOL) build_sequence(RE_CompileArgs* args);

/* Builds ANY. */
Py_LOCAL_INLINE(BOOL) build_ANY(RE_CompileArgs* args) {
    BYTE op;
    Py_ssize_t step;
    RE_Node* node;

    /* codes: opcode. */
    if (args->code + 1 > args->end_code)
        return FALSE;

    op = args->code[0];

    step = get_step(op);

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
Py_LOCAL_INLINE(BOOL) build_ATOMIC(RE_CompileArgs* args) {
    RE_Node* atomic_node;
    RE_CompileArgs subargs;
    RE_Node* success_node;

    /* codes: opcode, sequence, end. */
    atomic_node = create_node(args->pattern, RE_OP_ATOMIC, TRUE, 0, 1);
    if (!atomic_node)
        return FALSE;

    /* The number of repeat indexes. */
    atomic_node->values[0] = 0;

    ++args->code;

    subargs = *args;
    subargs.has_captures = FALSE;

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
    atomic_node->nonstring.next_2.node = subargs.start;

    args->code = subargs.code;
    args->min_width = subargs.min_width;
    args->has_captures |= subargs.has_captures;

    ++args->code;

    /* Append the node. */
    add_node(args->end, atomic_node);
    args->end = atomic_node;

    return TRUE;
}

/* Builds BIG_BITSET. */
Py_LOCAL_INLINE(BOOL) build_BIG_BITSET(RE_CompileArgs* args) {
    RE_CODE flags;
    BYTE op;
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

    step = get_step(op);

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
Py_LOCAL_INLINE(BOOL) build_BOUNDARY(RE_CompileArgs* args) {
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
Py_LOCAL_INLINE(BOOL) build_BRANCH(RE_CompileArgs* args) {
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

    /* A branch in the regular expression is compiled into a series of 2-way
     * branches.
     */
    do {
        RE_Node* next_branch_node;

        /* Skip over the 'BRANCH' or 'NEXT' opcode. */
        ++subargs.code;

        /* Compile the sequence until the next 'BRANCH' or 'NEXT' opcode. */
        subargs.min_width = 0;
        subargs.has_captures = FALSE;
        if (!build_sequence(&subargs))
            return FALSE;

        if (subargs.min_width < smallest_min_width)
            smallest_min_width = subargs.min_width;

        args->has_captures |= subargs.has_captures;

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
Py_LOCAL_INLINE(BOOL) build_CHARACTER(RE_CompileArgs* args) {
    RE_CODE flags;
    BYTE op;
    Py_ssize_t step;
    RE_Node* node;

    /* codes: opcode, flags, value. */
    if (args->code + 3 > args->end_code)
        return FALSE;

    flags = args->code[1];
    if (flags & ~(RE_MATCH_OP | RE_ZEROWIDTH_OP))
        return FALSE;

    op = args->code[0];

    step = get_step(op);

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
Py_LOCAL_INLINE(BOOL) build_GROUP(RE_CompileArgs* args) {
    RE_CODE group;
    RE_Node* start_node;
    RE_Node* end_node;
    RE_CompileArgs subargs;

    /* codes: opcode, group. */
    group = args->code[1];
    args->code += 2;

    /* Create nodes for the start and end of the capture group. */
    start_node = create_node(args->pattern, args->forward ? RE_OP_START_GROUP :
      RE_OP_END_GROUP, FALSE, 0, 2);
    end_node = create_node(args->pattern, args->forward ? RE_OP_END_GROUP :
      RE_OP_START_GROUP, FALSE, 0, 2);
    if (!start_node || !end_node)
        return FALSE;

    start_node->values[0] = group;
    end_node->values[0] = group;

    /* Signal that the capture should be saved when it's complete. */
    start_node->values[1] = 0;
    end_node->values[1] = args->save_captures ? 1 : 0;

    /* Record that we have a new capture group. */
    if (!record_group(args->pattern, group))
        return FALSE;

    /* Compile the sequence and check that we've reached the end of the capture
     * group.
     */
    subargs = *args;
    subargs.has_captures = FALSE;
    if (!build_sequence(&subargs))
        return FALSE;

    if (subargs.code[0] != RE_OP_END)
        return FALSE;

    args->code = subargs.code;
    args->min_width = subargs.min_width;
    args->has_captures |= subargs.has_captures || subargs.save_captures;

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
Py_LOCAL_INLINE(BOOL) build_GROUP_EXISTS(RE_CompileArgs* args) {
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
    subargs.has_captures = FALSE;
    if (!build_sequence(&subargs))
        return FALSE;

    args->code = subargs.code;
    args->has_captures |= subargs.has_captures;

    min_width = subargs.min_width;

    /* Append the start node. */
    add_node(args->end, start_node);
    add_node(start_node, subargs.start);
    add_node(subargs.end, end_node);

    if (args->code[0] == RE_OP_NEXT) {
        ++args->code;

        subargs.code = args->code;
        subargs.min_width = 0;
        subargs.has_captures = FALSE;
        if (!build_sequence(&subargs))
            return FALSE;

        args->code = subargs.code;
        args->has_captures |= subargs.has_captures;

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
Py_LOCAL_INLINE(BOOL) build_LOOKAROUND(RE_CompileArgs* args) {
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
      2);
    if (!lookaround_node)
        return FALSE;

    /* The number of repeat indexes. */
    lookaround_node->values[1] = 0;

    args->code += 3;

    /* Compile the sequence and check that we've reached the end of the
     * subpattern.
     */
    subargs = *args;
    subargs.forward = forward;
    subargs.has_captures = FALSE;
    if (!build_sequence(&subargs))
        return FALSE;

    lookaround_node->values[0] = subargs.has_captures;

    if (subargs.code[0] != RE_OP_END)
        return FALSE;

    args->code = subargs.code;
    args->has_captures |= subargs.has_captures;
    ++args->code;

    /* Create the 'SUCCESS' node and append it to the subpattern. */
    success_node = create_node(args->pattern, RE_OP_SUCCESS, FALSE, 0, 0);
    if (!success_node)
        return FALSE;

    /* Append the SUCCESS node. */
    add_node(subargs.end, success_node);

    /* Insert the subpattern into the node. */
    lookaround_node->nonstring.next_2.node = subargs.start;

    /* Append the lookaround. */
    add_node(args->end, lookaround_node);
    args->end = lookaround_node;

    return TRUE;
}

/* Builds PROPERTY. */
Py_LOCAL_INLINE(BOOL) build_PROPERTY(RE_CompileArgs* args) {
    RE_CODE flags;
    BYTE op;
    Py_ssize_t step;
    RE_Node* node;

    /* codes: opcode, flags, value. */
    if (args->code + 3 > args->end_code)
        return FALSE;

    flags = args->code[1];
    if (flags & ~(RE_MATCH_OP | RE_ZEROWIDTH_OP))
        return FALSE;

    op = args->code[0];

    step = get_step(op);

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
Py_LOCAL_INLINE(BOOL) build_REF_GROUP(RE_CompileArgs* args) {
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
Py_LOCAL_INLINE(BOOL) build_REPEAT(RE_CompileArgs* args) {
    BOOL greedy;
    RE_CODE min_count;
    RE_CODE max_count;

    /* codes: opcode, min_count, max_count, sequence, end. */

    /* This includes special cases such as optional items, which we'll check
     * for and treat specially. They don't need repeat counts, which helps us
     * avoid unnecessary work when matching.
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

        /* Compile the sequence and check that we've reached the end of it. */
        subargs = *args;
        subargs.has_captures = FALSE;
        if (!build_sequence(&subargs))
            return FALSE;

        if (subargs.code[0] != RE_OP_END)
            return FALSE;

        args->code = subargs.code;
        args->has_captures |= subargs.has_captures;

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
        subargs.has_captures = FALSE;
        if (!build_sequence(&subargs))
            return FALSE;

        if (subargs.code[0] != RE_OP_END)
            return FALSE;

        args->code = subargs.code;
        args->min_width = subargs.min_width;
        args->has_captures |= subargs.has_captures;

        ++args->code;

        /* Append the sequence. */
        add_node(args->end, subargs.start);
        args->end = subargs.end;
    } else {
        size_t index;
        RE_Node* repeat_node;
        RE_CompileArgs subargs;

        index = args->pattern->repeat_count;

        /* Create the nodes for the repeat. */
        repeat_node = create_node(args->pattern, greedy ? RE_OP_GREEDY_REPEAT :
          RE_OP_LAZY_REPEAT, FALSE, args->forward ? 1 : -1, 4);
        if (!repeat_node || !record_repeat(args->pattern, index,
          args->repeat_depth))
            return FALSE;

        repeat_node->values[0] = index;
        repeat_node->values[1] = min_count;
        repeat_node->values[2] = max_count;
        repeat_node->values[3] = args->forward;

        /* Compile the 'body' and check that we've reached the end of it. */
        subargs = *args;
        subargs.min_width = 0;
        subargs.save_captures = TRUE;
        subargs.has_captures = FALSE;
        ++subargs.repeat_depth;
        if (!build_sequence(&subargs))
            return FALSE;

        if (subargs.code[0] != RE_OP_END)
            return FALSE;

        args->code = subargs.code;
        args->min_width += min_count * subargs.min_width;
        args->has_captures |= subargs.has_captures;

        ++args->code;

        if (sequence_matches_one(subargs.start)) {
            repeat_node->op = greedy ? RE_OP_GREEDY_REPEAT_ONE :
              RE_OP_LAZY_REPEAT_ONE;

            /* Append the new sequence. */
            add_node(args->end, repeat_node);
            repeat_node->nonstring.next_2.node = subargs.start;
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
Py_LOCAL_INLINE(BOOL) build_SMALL_BITSET(RE_CompileArgs* args) {
    size_t size;
    RE_CODE flags;
    BYTE op;
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

    step = get_step(op);

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

/* Builds SET. */
Py_LOCAL_INLINE(BOOL) build_SET(RE_CompileArgs* args) {
    RE_CODE flags;
    BYTE op;
    Py_ssize_t step;
    RE_Node* node;
    size_t saved_min_width;

    /* codes: opcode, flags, members. */
    flags = args->code[1];
    if (flags & ~(RE_MATCH_OP | RE_ZEROWIDTH_OP))
        return FALSE;

    op = args->code[0];

    step = get_step(op);

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
        case RE_OP_SET_DIFF:
        case RE_OP_SET_DIFF_REV:
        case RE_OP_SET_INTER:
        case RE_OP_SET_INTER_REV:
        case RE_OP_SET_SYM_DIFF:
        case RE_OP_SET_SYM_DIFF_REV:
        case RE_OP_SET_UNION:
        case RE_OP_SET_UNION_REV:
            if (!build_SET(args))
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

    /* Check that we've reached the end correctly. (The last opcode should be
     * 'END'.)
     */
    if (args->code >= args->end_code || args->code[0] != RE_OP_END)
        return FALSE;

    ++args->code;

    /* At this point the set's members are in the main sequence. They need to
     * be moved out-of-line.
     */
    node->nonstring.next_2.node = node->next_1.node;
    node->next_1.node = NULL;
    args->end = node;

    args->min_width = saved_min_width;

    if (step != 0)
        ++args->min_width;

    return TRUE;
}

/* Builds STRING. */
Py_LOCAL_INLINE(BOOL) build_STRING(RE_CompileArgs* args) {
    Py_ssize_t length;
    BYTE op;
    Py_ssize_t step;
    RE_Node* node;
    Py_ssize_t i;

    /* codes: opcode, length, characters. */
    length = args->code[1];
    if (args->code + 2 + length > args->end_code)
        return FALSE;

    op = args->code[0];

    step = get_step(op);

    /* Create the node. */
    node = create_node(args->pattern, op, TRUE, step * length, length);
    if (!node)
        return FALSE;
    node->status |= RE_STATUS_STRING;

    for (i = 0; i < length; i++)
        node->values[i] = args->code[2 + i];

    args->code += 2 + length;

    /* Append the node. */
    add_node(args->end, node);
    args->end = node;

    args->min_width += length;

    return TRUE;
}

/* Builds STRING_SET. */
Py_LOCAL_INLINE(BOOL) build_STRING_SET(RE_CompileArgs* args) {
    Py_ssize_t index;
    Py_ssize_t min_len;
    Py_ssize_t max_len;
    RE_Node* node;

    /* codes: opcode, index, min_len, max_len. */
    index = args->code[1];
    min_len = args->code[2];
    max_len = args->code[3];
    node = create_node(args->pattern, args->code[0], FALSE, 0, 3);
    if (!node)
        return FALSE;

    node->values[0] = index;
    node->values[1] = min_len;
    node->values[2] = max_len;
    args->code += 4;

    /* Append the reference. */
    add_node(args->end, node);
    args->end = node;

    return TRUE;
}

/* Builds zero-width. */
Py_LOCAL_INLINE(BOOL) build_zerowidth(RE_CompileArgs* args) {
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
Py_LOCAL_INLINE(BOOL) build_sequence(RE_CompileArgs* args) {
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
        case RE_OP_ANY_U:
        case RE_OP_ANY_U_REV:
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
        case RE_OP_GRAPHEME_BOUNDARY:
            /* A word or grapheme boundary. */
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
        case RE_OP_END_OF_LINE_U:
        case RE_OP_END_OF_STRING:
        case RE_OP_END_OF_STRING_LINE:
        case RE_OP_END_OF_STRING_LINE_U:
        case RE_OP_SEARCH_ANCHOR:
        case RE_OP_START_OF_LINE:
        case RE_OP_START_OF_LINE_U:
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
        case RE_OP_STRING_SET:
        case RE_OP_STRING_SET_IGN:
        case RE_OP_STRING_SET_IGN_REV:
        case RE_OP_STRING_SET_REV:
            /* A reference to a list. */
            if (!build_STRING_SET(args))
                return FALSE;
            break;
        case RE_OP_SET_DIFF:
        case RE_OP_SET_DIFF_REV:
        case RE_OP_SET_INTER:
        case RE_OP_SET_INTER_REV:
        case RE_OP_SET_SYM_DIFF:
        case RE_OP_SET_SYM_DIFF_REV:
        case RE_OP_SET_UNION:
        case RE_OP_SET_UNION_REV:
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
Py_LOCAL_INLINE(BOOL) compile_to_nodes(RE_CODE* code, RE_CODE* end_code,
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
    args.save_captures = FALSE;
    args.has_captures = FALSE;
    args.repeat_depth = 0;
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
    PyObject* ref_lists;
    Py_ssize_t code_len;
    RE_CODE* code;
    Py_ssize_t i;
    PatternObject* self;
    BOOL ascii;
    BOOL locale;
    BOOL unicode;
    BOOL ok;

    if (!PyArg_ParseTuple(args, "OnOOOO", &pattern, &flags, &code_list,
      &groupindex, &indexgroup, &ref_lists))
        return NULL;

    /* Read the regular expression code. */
    code_len = PyList_GET_SIZE(code_list);
    code = (RE_CODE*)re_alloc(code_len * sizeof(RE_CODE));
    if (!code)
        return NULL;

    for (i = 0; i < code_len; i++) {
        PyObject* o = PyList_GET_ITEM(code_list, i);
        size_t value;

        value = PyLong_AsUnsignedLong(o);
        if (PyErr_Occurred())
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
    self->ref_lists = ref_lists;
    self->node_capacity = 0;
    self->node_count = 0;
    self->node_list = NULL;
    self->group_info_capacity = 0;
    self->group_info = NULL;
    self->repeat_info_capacity = 0;
    self->repeat_info = NULL;
    self->groups_storage = NULL;
    self->repeats_storage = NULL;
    self->saved_groups_capacity_storage = 0;
    self->saved_groups_storage = NULL;
    Py_INCREF(self->pattern);
    Py_INCREF(self->groupindex);
    Py_INCREF(self->indexgroup);
    Py_INCREF(self->ref_lists);

    /* Initialise the character encoding. */
    unicode = (flags & RE_FLAG_UNICODE) != 0;
    locale = (flags & RE_FLAG_LOCALE) != 0;
    ascii = (flags & RE_FLAG_ASCII) != 0;
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
    return Py_BuildValue("n", sizeof(RE_CODE));
}

/* Gets the property dict. */
static PyObject* get_properties(PyObject* self_, PyObject* args) {
    Py_INCREF(property_dict);

    return property_dict;
}

/* Gets all the possible cases of a character. */
static PyObject* get_all_cases(PyObject* self_, PyObject* args) {
    PyObject* all_cases;
    int count;
    int i;
    RE_CODE cases[RE_MAX_CASE_DIFFS + 1];

    Py_ssize_t flags;
    Py_ssize_t ch;
    if (!PyArg_ParseTuple(args, "nn", &flags, &ch))
        return NULL;

    all_cases = PyList_New(0);
    if (!all_cases)
        return NULL;

    if (flags & RE_FLAG_UNICODE)
        count = unicode_get_all_cases(ch, cases);
    else if (flags & RE_FLAG_LOCALE)
        count = locale_get_all_cases(ch, cases);
    else if (flags & RE_FLAG_ASCII)
        count = ascii_get_all_cases(ch, cases);
    else
        count = ascii_get_all_cases(ch, cases);

    if (count == 0)
        goto error;

    for (i = 0; i < count; i++) {
        PyObject* item;
        int status;

        item = Py_BuildValue("n", cases[i]);
        if (!item)
            goto error;
        status = PyList_Append(all_cases, item);
        Py_DECREF(item);
        if (status < 0)
            goto error;
    }

    return all_cases;

error:
    Py_DECREF(all_cases);
    return NULL;
}

/* Folds the case of a character. */
static PyObject* folded_case(PyObject* self_, PyObject* args) {
    Py_ssize_t flags;
    Py_ssize_t ch;
    if (!PyArg_ParseTuple(args, "nn", &flags, &ch))
        return NULL;

    if (flags & RE_FLAG_UNICODE)
        ch = unicode_folded_case(ch);
    else if (flags & RE_FLAG_LOCALE)
        ch = locale_folded_case(ch);
    else if (flags & RE_FLAG_ASCII)
        ch = ascii_folded_case(ch);
    else
        ch = ascii_folded_case(ch);

    return Py_BuildValue("n", ch);
}

/* The table of the module's functions. */
static PyMethodDef _functions[] = {
    {"compile", (PyCFunction)re_compile, METH_VARARGS},
    {"get_code_size", (PyCFunction)get_code_size, METH_NOARGS},
    {"get_properties", (PyCFunction)get_properties, METH_VARARGS},
    {"all_cases", (PyCFunction)get_all_cases, METH_VARARGS},
    {"folded_case", (PyCFunction)folded_case, METH_VARARGS},
    {NULL, NULL}
};

static BOOL init_property_dict() {
    int value_set_count;
    int i;
    PyObject** value_dicts;

    property_dict = NULL;

    value_set_count = 0;

    for (i = 0; i < sizeof(re_property_values) / sizeof(re_property_values[0]);
      i++) {
        RE_PropertyValue* value;

        value = &re_property_values[i];
        if (value->value_set >= value_set_count)
            value_set_count = value->value_set + 1;
    }

    value_dicts = (PyObject**)PyMem_Malloc(value_set_count *
      sizeof(value_dicts[0]));
    if (!value_dicts)
        return FALSE;

    memset(value_dicts, 0, value_set_count * sizeof(value_dicts[0]));

    for (i = 0; i < sizeof(re_property_values) / sizeof(re_property_values[0]);
      i++) {
        RE_PropertyValue* value;
        PyObject* v;

        value = &re_property_values[i];
        if (!value_dicts[value->value_set]) {
            value_dicts[value->value_set] = PyDict_New();
            if (!value_dicts[value->value_set])
                goto error;
        }

        v = Py_BuildValue("i", value->id);
        if (!v)
            goto error;

        PyDict_SetItemString(value_dicts[value->value_set],
          re_strings[value->name], v);
    }

    property_dict = PyDict_New();

    if (!property_dict)
        goto error;

    for (i = 0; i < sizeof(re_properties) / sizeof(re_properties[0]); i++) {
        RE_Property* property;
        PyObject* v;

        property = &re_properties[i];
        v = Py_BuildValue("iO", property->id, value_dicts[property->value_set]);
        if (!v)
            goto error;

        PyDict_SetItemString(property_dict, re_strings[property->name], v);
    }

    for (i = 0; i < value_set_count; i++)
        Py_XDECREF(value_dicts[i]);

    PyMem_Free(value_dicts);

    return TRUE;

error:
    Py_XDECREF(property_dict);

    for (i = 0; i < value_set_count; i++)
        Py_XDECREF(value_dicts[i]);

    PyMem_Free(value_dicts);

    return FALSE;
}

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

    error_exception = NULL;

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

    if (!init_property_dict())
        return;
}

/* vim:ts=4:sw=4:et */
