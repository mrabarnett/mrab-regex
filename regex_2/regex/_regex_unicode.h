typedef unsigned char RE_UINT8;
typedef unsigned short RE_UINT16;
typedef unsigned int RE_UINT32;
typedef signed int RE_INT32;

typedef unsigned char BOOL;
enum {FALSE, TRUE};

#define RE_ASCII_MAX 0x7F
#define RE_LOCALE_MAX 0xFF

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

#define RE_PROP_C_MASK 0x0000001F
#define RE_PROP_L_MASK 0x000003E0
#define RE_PROP_M_MASK 0x00001C00
#define RE_PROP_N_MASK 0x0000E000
#define RE_PROP_P_MASK 0x007F0000
#define RE_PROP_S_MASK 0x07800000
#define RE_PROP_Z_MASK 0x38000000

#define RE_PROP_ALNUM 0x380001
#define RE_PROP_ALPHA 0x130001
#define RE_PROP_ASCII 0x390001
#define RE_PROP_BLANK 0x3A0001
#define RE_PROP_CNTRL 0x1
#define RE_PROP_DIGIT 0xD
#define RE_PROP_GRAPH 0x3B0001
#define RE_PROP_LOWER 0x70001
#define RE_PROP_PRINT 0x3C0001
#define RE_PROP_PUNCT 0x22
#define RE_PROP_SPACE 0x370001
#define RE_PROP_UPPER 0xA0001
#define RE_PROP_WORD 0x3D0001
#define RE_PROP_XDIGIT 0x3E0001

#define RE_BREAK_OTHER 0
#define RE_BREAK_LF 1
#define RE_BREAK_MIDNUM 2
#define RE_BREAK_MIDLETTER 3
#define RE_BREAK_EXTEND 4
#define RE_BREAK_FORMAT 5
#define RE_BREAK_ALETTER 6
#define RE_BREAK_NEWLINE 7
#define RE_BREAK_NUMERIC 8
#define RE_BREAK_MIDNUMLET 9
#define RE_BREAK_EXTENDNUMLET 10
#define RE_BREAK_CR 11
#define RE_BREAK_KATAKANA 12

#define RE_GBREAK_OTHER 0
#define RE_GBREAK_CONTROL 1
#define RE_GBREAK_LF 2
#define RE_GBREAK_EXTEND 3
#define RE_GBREAK_LVT 4
#define RE_GBREAK_V 5
#define RE_GBREAK_L 6
#define RE_GBREAK_PREPEND 7
#define RE_GBREAK_LV 8
#define RE_GBREAK_T 9
#define RE_GBREAK_SPACINGMARK 10
#define RE_GBREAK_CR 11

extern char* re_strings[542];
extern RE_Property re_properties[115];
extern RE_PropertyValue re_property_values[1139];
extern RE_GetPropertyFunc re_get_property[64];

RE_UINT32 re_get_general_category(RE_UINT32 ch);
RE_UINT32 re_get_block(RE_UINT32 ch);
RE_UINT32 re_get_script(RE_UINT32 ch);
RE_UINT32 re_get_word_break(RE_UINT32 ch);
RE_UINT32 re_get_grapheme_break(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_titlecased(RE_UINT32 ch);
RE_UINT32 re_get_grapheme_extend(RE_UINT32 ch);
RE_UINT32 re_get_lowercase(RE_UINT32 ch);
RE_UINT32 re_get_grapheme_base(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_uppercased(RE_UINT32 ch);
RE_UINT32 re_get_uppercase(RE_UINT32 ch);
RE_UINT32 re_get_id_start(RE_UINT32 ch);
RE_UINT32 re_get_grapheme_link(RE_UINT32 ch);
RE_UINT32 re_get_cased(RE_UINT32 ch);
RE_UINT32 re_get_id_continue(RE_UINT32 ch);
RE_UINT32 re_get_xid_continue(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_lowercased(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_casefolded(RE_UINT32 ch);
RE_UINT32 re_get_math(RE_UINT32 ch);
RE_UINT32 re_get_alphabetic(RE_UINT32 ch);
RE_UINT32 re_get_xid_start(RE_UINT32 ch);
RE_UINT32 re_get_default_ignorable_code_point(RE_UINT32 ch);
RE_UINT32 re_get_case_ignorable(RE_UINT32 ch);
RE_UINT32 re_get_changes_when_casemapped(RE_UINT32 ch);
RE_UINT32 re_get_pattern_white_space(RE_UINT32 ch);
RE_UINT32 re_get_other_default_ignorable_code_point(RE_UINT32 ch);
RE_UINT32 re_get_pattern_syntax(RE_UINT32 ch);
RE_UINT32 re_get_ids_binary_operator(RE_UINT32 ch);
RE_UINT32 re_get_sterm(RE_UINT32 ch);
RE_UINT32 re_get_other_lowercase(RE_UINT32 ch);
RE_UINT32 re_get_other_alphabetic(RE_UINT32 ch);
RE_UINT32 re_get_ids_trinary_operator(RE_UINT32 ch);
RE_UINT32 re_get_bidi_control(RE_UINT32 ch);
RE_UINT32 re_get_ascii_hex_digit(RE_UINT32 ch);
RE_UINT32 re_get_other_id_continue(RE_UINT32 ch);
RE_UINT32 re_get_other_grapheme_extend(RE_UINT32 ch);
RE_UINT32 re_get_extender(RE_UINT32 ch);
RE_UINT32 re_get_deprecated(RE_UINT32 ch);
RE_UINT32 re_get_soft_dotted(RE_UINT32 ch);
RE_UINT32 re_get_ideographic(RE_UINT32 ch);
RE_UINT32 re_get_other_math(RE_UINT32 ch);
RE_UINT32 re_get_join_control(RE_UINT32 ch);
RE_UINT32 re_get_quotation_mark(RE_UINT32 ch);
RE_UINT32 re_get_terminal_punctuation(RE_UINT32 ch);
RE_UINT32 re_get_logical_order_exception(RE_UINT32 ch);
RE_UINT32 re_get_variation_selector(RE_UINT32 ch);
RE_UINT32 re_get_diacritic(RE_UINT32 ch);
RE_UINT32 re_get_radical(RE_UINT32 ch);
RE_UINT32 re_get_hyphen(RE_UINT32 ch);
RE_UINT32 re_get_unified_ideograph(RE_UINT32 ch);
RE_UINT32 re_get_dash(RE_UINT32 ch);
RE_UINT32 re_get_hex_digit(RE_UINT32 ch);
RE_UINT32 re_get_noncharacter_code_point(RE_UINT32 ch);
RE_UINT32 re_get_other_id_start(RE_UINT32 ch);
RE_UINT32 re_get_other_uppercase(RE_UINT32 ch);
RE_UINT32 re_get_white_space(RE_UINT32 ch);
RE_UINT32 re_get_alphanumeric(RE_UINT32 ch);
RE_UINT32 re_get_ascii(RE_UINT32 ch);
RE_UINT32 re_get_blank(RE_UINT32 ch);
RE_UINT32 re_get_graph(RE_UINT32 ch);
RE_UINT32 re_get_print(RE_UINT32 ch);
RE_UINT32 re_get_word(RE_UINT32 ch);
RE_UINT32 re_get_xdigit(RE_UINT32 ch);
BOOL re_is_same_char_ign(RE_UINT32 ch1, RE_UINT32 ch2);
