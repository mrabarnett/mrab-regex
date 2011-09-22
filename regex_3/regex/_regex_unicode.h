typedef unsigned char RE_UINT8;
typedef signed char RE_INT8;
typedef unsigned short RE_UINT16;
typedef signed short RE_INT16;
typedef unsigned int RE_UINT32;
typedef signed int RE_INT32;

typedef unsigned char BOOL;
enum {FALSE, TRUE};

#define RE_ASCII_MAX 0x7F
#define RE_LOCALE_MAX 0xFF
#define RE_UNICODE_MAX 0x10FFFF

#define RE_MAX_CASES 4
#define RE_MAX_FOLDED 3

typedef struct RE_Property {
    RE_UINT16 name;
    RE_UINT8 id;
    RE_UINT8 value_set;
} RE_Property;

typedef struct RE_PropertyValue {
    RE_UINT16 name;
    RE_UINT8 value_set;
    RE_UINT8 id;
} RE_PropertyValue;

typedef RE_UINT32 (*RE_GetPropertyFunc)(RE_UINT32 ch);

#define RE_PROP_C 30
#define RE_PROP_L 31
#define RE_PROP_M 32
#define RE_PROP_N 33
#define RE_PROP_P 34
#define RE_PROP_S 35
#define RE_PROP_Z 36

#define RE_PROP_C_MASK 0x00078001
#define RE_PROP_L_MASK 0x0000003E
#define RE_PROP_M_MASK 0x000001C0
#define RE_PROP_N_MASK 0x00000E00
#define RE_PROP_P_MASK 0x30F80000
#define RE_PROP_S_MASK 0x0F000000
#define RE_PROP_Z_MASK 0x00007000

#define RE_PROP_ALNUM 0x460001
#define RE_PROP_ALPHA 0x070001
#define RE_PROP_ANY 0x470001
#define RE_PROP_ASCII 0x480001
#define RE_PROP_ASSIGNED 0x490001
#define RE_PROP_BLANK 0x4A0001
#define RE_PROP_CNTRL 0x00000F
#define RE_PROP_DIGIT 0x000009
#define RE_PROP_GRAPH 0x4B0001
#define RE_PROP_LOWER 0x080001
#define RE_PROP_PRINT 0x4C0001
#define RE_PROP_PUNCT 0x000022
#define RE_PROP_SPACE 0x190001
#define RE_PROP_UPPER 0x090001
#define RE_PROP_WORD 0x4D0001
#define RE_PROP_XDIGIT 0x4E0001

#define RE_BREAK_OTHER 0
#define RE_BREAK_CR 1
#define RE_BREAK_LF 2
#define RE_BREAK_NEWLINE 3
#define RE_BREAK_EXTEND 4
#define RE_BREAK_FORMAT 5
#define RE_BREAK_KATAKANA 6
#define RE_BREAK_ALETTER 7
#define RE_BREAK_MIDLETTER 8
#define RE_BREAK_MIDNUM 9
#define RE_BREAK_MIDNUMLET 10
#define RE_BREAK_NUMERIC 11
#define RE_BREAK_EXTENDNUMLET 12

#define RE_GBREAK_OTHER 0
#define RE_GBREAK_CR 1
#define RE_GBREAK_LF 2
#define RE_GBREAK_CONTROL 3
#define RE_GBREAK_EXTEND 4
#define RE_GBREAK_PREPEND 5
#define RE_GBREAK_SPACINGMARK 6
#define RE_GBREAK_L 7
#define RE_GBREAK_V 8
#define RE_GBREAK_T 9
#define RE_GBREAK_LV 10
#define RE_GBREAK_LVT 11

extern char* re_strings[980];
extern RE_Property re_properties[143];
extern RE_PropertyValue re_property_values[1051];
extern RE_UINT16 re_expand_on_folding[104];
extern RE_GetPropertyFunc re_get_property[79];

RE_UINT32 re_get_general_category(RE_UINT32 ch);
RE_UINT32 re_get_block(RE_UINT32 ch);
RE_UINT32 re_get_script(RE_UINT32 ch);
RE_UINT32 re_get_word_break(RE_UINT32 ch);
RE_UINT32 re_get_grapheme_cluster_break(RE_UINT32 ch);
RE_UINT32 re_get_sentence_break(RE_UINT32 ch);
RE_UINT32 re_get_math(RE_UINT32 ch);
RE_UINT32 re_get_alphabetic(RE_UINT32 ch);
RE_UINT32 re_get_lowercase(RE_UINT32 ch);
RE_UINT32 re_get_uppercase(RE_UINT32 ch);
RE_UINT32 re_get_cased(RE_UINT32 ch);
RE_UINT32 re_get_case_ignorable(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_lowercased(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_uppercased(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_titlecased(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_casefolded(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_casemapped(RE_UINT32 ch);
RE_UINT32 re_get_id_start(RE_UINT32 ch);
RE_UINT32 re_get_id_continue(RE_UINT32 ch);
RE_UINT32 re_get_xid_start(RE_UINT32 ch);
RE_UINT32 re_get_xid_continue(RE_UINT32 ch);
RE_UINT32 re_get_default_ignorable_code_point(RE_UINT32 ch);
RE_UINT32 re_get_grapheme_extend(RE_UINT32 ch);
RE_UINT32 re_get_grapheme_base(RE_UINT32 ch);
RE_UINT32 re_get_grapheme_link(RE_UINT32 ch);
RE_UINT32 re_get_white_space(RE_UINT32 ch);
RE_UINT32 re_get_bidi_control(RE_UINT32 ch);
RE_UINT32 re_get_join_control(RE_UINT32 ch);
RE_UINT32 re_get_dash(RE_UINT32 ch);
RE_UINT32 re_get_hyphen(RE_UINT32 ch);
RE_UINT32 re_get_quotation_mark(RE_UINT32 ch);
RE_UINT32 re_get_terminal_punctuation(RE_UINT32 ch);
RE_UINT32 re_get_other_math(RE_UINT32 ch);
RE_UINT32 re_get_hex_digit(RE_UINT32 ch);
RE_UINT32 re_get_ascii_hex_digit(RE_UINT32 ch);
RE_UINT32 re_get_other_alphabetic(RE_UINT32 ch);
RE_UINT32 re_get_ideographic(RE_UINT32 ch);
RE_UINT32 re_get_diacritic(RE_UINT32 ch);
RE_UINT32 re_get_extender(RE_UINT32 ch);
RE_UINT32 re_get_other_lowercase(RE_UINT32 ch);
RE_UINT32 re_get_other_uppercase(RE_UINT32 ch);
RE_UINT32 re_get_noncharacter_code_point(RE_UINT32 ch);
RE_UINT32 re_get_other_grapheme_extend(RE_UINT32 ch);
RE_UINT32 re_get_ids_binary_operator(RE_UINT32 ch);
RE_UINT32 re_get_ids_trinary_operator(RE_UINT32 ch);
RE_UINT32 re_get_radical(RE_UINT32 ch);
RE_UINT32 re_get_unified_ideograph(RE_UINT32 ch);
RE_UINT32 re_get_other_default_ignorable_code_point(RE_UINT32 ch);
RE_UINT32 re_get_deprecated(RE_UINT32 ch);
RE_UINT32 re_get_soft_dotted(RE_UINT32 ch);
RE_UINT32 re_get_logical_order_exception(RE_UINT32 ch);
RE_UINT32 re_get_other_id_start(RE_UINT32 ch);
RE_UINT32 re_get_other_id_continue(RE_UINT32 ch);
RE_UINT32 re_get_sterm(RE_UINT32 ch);
RE_UINT32 re_get_variation_selector(RE_UINT32 ch);
RE_UINT32 re_get_pattern_white_space(RE_UINT32 ch);
RE_UINT32 re_get_pattern_syntax(RE_UINT32 ch);
RE_UINT32 re_get_hangul_syllable_type(RE_UINT32 ch);
RE_UINT32 re_get_bidi_class(RE_UINT32 ch);
RE_UINT32 re_get_canonical_combining_class(RE_UINT32 ch);
RE_UINT32 re_get_decomposition_type(RE_UINT32 ch);
RE_UINT32 re_get_east_asian_width(RE_UINT32 ch);
RE_UINT32 re_get_joining_group(RE_UINT32 ch);
RE_UINT32 re_get_joining_type(RE_UINT32 ch);
RE_UINT32 re_get_line_break(RE_UINT32 ch);
RE_UINT32 re_get_numeric_type(RE_UINT32 ch);
RE_UINT32 re_get_numeric_value(RE_UINT32 ch);
RE_UINT32 re_get_bidi_mirrored(RE_UINT32 ch);
RE_UINT32 re_get_indic_matra_category(RE_UINT32 ch);
RE_UINT32 re_get_indic_syllabic_category(RE_UINT32 ch);
RE_UINT32 re_get_alphanumeric(RE_UINT32 ch);
RE_UINT32 re_get_any(RE_UINT32 ch);
RE_UINT32 re_get_ascii(RE_UINT32 ch);
RE_UINT32 re_get_assigned(RE_UINT32 ch);
RE_UINT32 re_get_blank(RE_UINT32 ch);
RE_UINT32 re_get_graph(RE_UINT32 ch);
RE_UINT32 re_get_print(RE_UINT32 ch);
RE_UINT32 re_get_word(RE_UINT32 ch);
RE_UINT32 re_get_xdigit(RE_UINT32 ch);
int re_get_all_cases(RE_UINT32 ch, RE_UINT32* codepoints);
RE_UINT32 re_get_simple_case_folding(RE_UINT32 ch);
int re_get_full_case_folding(RE_UINT32 ch, RE_UINT32* codepoints);
