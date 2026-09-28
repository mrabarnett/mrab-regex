#include <stdbool.h>
#include <stdint.h>

#define RE_UNICODE_VERSION "18.0.0"

#define RE_ASCII_MAX 0x7F
#define RE_LOCALE_MAX 0xFF

#define RE_MAX_CASES 4
#define RE_MAX_FOLDED 3
#define RE_MAX_SCX 23

typedef struct RE_Property {
    uint16_t name;
    uint8_t id;
    uint8_t value_set;
} RE_Property;

typedef struct RE_PropertyValue {
    uint16_t name;
    uint8_t value_set;
    uint16_t id;
} RE_PropertyValue;

typedef uint32_t (*RE_GetPropertyFunc)(uint32_t codepoint);

#define RE_PROP_GC 0x1E
#define RE_PROP_CASED 0xA
#define RE_PROP_UPPERCASE 0x5C
#define RE_PROP_LOWERCASE 0x38
#define RE_PROP_SCX 0x56

#define RE_PROP_C 30
#define RE_PROP_L 31
#define RE_PROP_M 32
#define RE_PROP_N 33
#define RE_PROP_P 34
#define RE_PROP_S 35
#define RE_PROP_Z 36
#define RE_PROP_ASSIGNED 37
#define RE_PROP_CASEDLETTER 38

#define RE_PROP_CN 0
#define RE_PROP_CC 1
#define RE_PROP_ZS 2
#define RE_PROP_PO 3
#define RE_PROP_SC 4
#define RE_PROP_PS 5
#define RE_PROP_PE 6
#define RE_PROP_SM 7
#define RE_PROP_PD 8
#define RE_PROP_ND 9
#define RE_PROP_LU 10
#define RE_PROP_SK 11
#define RE_PROP_PC 12
#define RE_PROP_LL 13
#define RE_PROP_SO 14
#define RE_PROP_LO 15
#define RE_PROP_PI 16
#define RE_PROP_CF 17
#define RE_PROP_NO 18
#define RE_PROP_PF 19
#define RE_PROP_LT 20
#define RE_PROP_LM 21
#define RE_PROP_MN 22
#define RE_PROP_ME 23
#define RE_PROP_MC 24
#define RE_PROP_NL 25
#define RE_PROP_ZL 26
#define RE_PROP_ZP 27
#define RE_PROP_CS 28
#define RE_PROP_CO 29

#define RE_PROP_C_MASK 0x30020003
#define RE_PROP_L_MASK 0x0030A400
#define RE_PROP_M_MASK 0x01C00000
#define RE_PROP_N_MASK 0x02040200
#define RE_PROP_P_MASK 0x00091168
#define RE_PROP_S_MASK 0x00004890
#define RE_PROP_Z_MASK 0x0C000004

#define RE_PROP_ALNUM 0x010001
#define RE_PROP_ALPHA 0x000001
#define RE_PROP_ANY 0x020001
#define RE_PROP_ASCII 0x080001
#define RE_PROP_BLANK 0x070001
#define RE_PROP_CNTRL 0x1E0001
#define RE_PROP_DIGIT 0x1E0009
#define RE_PROP_GRAPH 0x1F0001
#define RE_PROP_LOWER 0x380001
#define RE_PROP_PRINT 0x510001
#define RE_PROP_SPACE 0x5F0001
#define RE_PROP_UPPER 0x5C0001
#define RE_PROP_WORD 0x600001
#define RE_PROP_XDIGIT 0x620001
#define RE_PROP_POSIX_ALNUM 0x4C0001
#define RE_PROP_POSIX_DIGIT 0x4D0001
#define RE_PROP_POSIX_PUNCT 0x4E0001
#define RE_PROP_POSIX_XDIGIT 0x4F0001

#define RE_WBREAK_OTHER 0
#define RE_WBREAK_LF 1
#define RE_WBREAK_NEWLINE 2
#define RE_WBREAK_CR 3
#define RE_WBREAK_WSEGSPACE 4
#define RE_WBREAK_DOUBLEQUOTE 5
#define RE_WBREAK_SINGLEQUOTE 6
#define RE_WBREAK_MIDNUM 7
#define RE_WBREAK_MIDNUMLET 8
#define RE_WBREAK_NUMERIC 9
#define RE_WBREAK_MIDLETTER 10
#define RE_WBREAK_ALETTER 11
#define RE_WBREAK_EXTENDNUMLET 12
#define RE_WBREAK_FORMAT 13
#define RE_WBREAK_EXTEND 14
#define RE_WBREAK_HEBREWLETTER 15
#define RE_WBREAK_ZWJ 16
#define RE_WBREAK_KATAKANA 17
#define RE_WBREAK_REGIONALINDICATOR 18
#define RE_WBREAK_EBASE 19
#define RE_WBREAK_EBASEGAZ 20
#define RE_WBREAK_EMODIFIER 21
#define RE_WBREAK_GLUEAFTERZWJ 22

#define RE_GBREAK_OTHER 0
#define RE_GBREAK_CONTROL 1
#define RE_GBREAK_LF 2
#define RE_GBREAK_CR 3
#define RE_GBREAK_EXTEND 4
#define RE_GBREAK_PREPEND 5
#define RE_GBREAK_SPACINGMARK 6
#define RE_GBREAK_L 7
#define RE_GBREAK_V 8
#define RE_GBREAK_T 9
#define RE_GBREAK_ZWJ 10
#define RE_GBREAK_LV 11
#define RE_GBREAK_LVT 12
#define RE_GBREAK_REGIONALINDICATOR 13
#define RE_GBREAK_EBASE 14
#define RE_GBREAK_EBASEGAZ 15
#define RE_GBREAK_EMODIFIER 16
#define RE_GBREAK_GLUEAFTERZWJ 17

#define RE_LBREAK_UNKNOWN 0
#define RE_LBREAK_COMBININGMARK 1
#define RE_LBREAK_BREAKAFTER 2
#define RE_LBREAK_LINEFEED 3
#define RE_LBREAK_MANDATORYBREAK 4
#define RE_LBREAK_CARRIAGERETURN 5
#define RE_LBREAK_SPACE 6
#define RE_LBREAK_EXCLAMATION 7
#define RE_LBREAK_QUOTATION 8
#define RE_LBREAK_ALPHABETIC 9
#define RE_LBREAK_PREFIXNUMERIC 10
#define RE_LBREAK_POSTFIXNUMERIC 11
#define RE_LBREAK_OPENPUNCTUATION 12
#define RE_LBREAK_CLOSEPARENTHESIS 13
#define RE_LBREAK_INFIXNUMERIC 14
#define RE_LBREAK_HYPHEN 15
#define RE_LBREAK_BREAKSYMBOLS 16
#define RE_LBREAK_NUMERIC 17
#define RE_LBREAK_CLOSEPUNCTUATION 18
#define RE_LBREAK_NEXTLINE 19
#define RE_LBREAK_GLUE 20
#define RE_LBREAK_AMBIGUOUS 21
#define RE_LBREAK_UNAMBIGUOUSHYPHEN 22
#define RE_LBREAK_BREAKBEFORE 23
#define RE_LBREAK_HEBREWLETTER 24
#define RE_LBREAK_COMPLEXCONTEXT 25
#define RE_LBREAK_JL 26
#define RE_LBREAK_JV 27
#define RE_LBREAK_JT 28
#define RE_LBREAK_NONSTARTER 29
#define RE_LBREAK_AKSARA 30
#define RE_LBREAK_VIRAMA 31
#define RE_LBREAK_AKSARASTART 32
#define RE_LBREAK_IDEOGRAPHIC 33
#define RE_LBREAK_VIRAMAFINAL 34
#define RE_LBREAK_ZWSPACE 35
#define RE_LBREAK_ZWJ 36
#define RE_LBREAK_BREAKBOTH 37
#define RE_LBREAK_INSEPARABLE 38
#define RE_LBREAK_WORDJOINER 39
#define RE_LBREAK_EBASE 40
#define RE_LBREAK_CONDITIONALJAPANESESTARTER 41
#define RE_LBREAK_H2 42
#define RE_LBREAK_H3 43
#define RE_LBREAK_SURROGATE 44
#define RE_LBREAK_CONTINGENTBREAK 45
#define RE_LBREAK_AKSARAPREBASE 46
#define RE_LBREAK_REGIONALINDICATOR 47
#define RE_LBREAK_EMODIFIER 48

#define RE_INCB_NONE 0
#define RE_INCB_EXTEND 1
#define RE_INCB_CONSONANT 2
#define RE_INCB_LINKER 3

extern char* re_strings[1572];
extern RE_Property re_properties[185];
extern RE_PropertyValue re_property_values[1729];
extern uint32_t re_expand_on_folding[105];
extern RE_GetPropertyFunc re_get_property[101];

uint32_t re_get_alphabetic(uint32_t codepoint);
uint32_t re_get_alphanumeric(uint32_t codepoint);
uint32_t re_get_any(uint32_t codepoint);
uint32_t re_get_ascii_hex_digit(uint32_t codepoint);
uint32_t re_get_bidi_class(uint32_t codepoint);
uint32_t re_get_bidi_control(uint32_t codepoint);
uint32_t re_get_bidi_mirrored(uint32_t codepoint);
uint32_t re_get_blank(uint32_t codepoint);
uint32_t re_get_block(uint32_t codepoint);
uint32_t re_get_canonical_combining_class(uint32_t codepoint);
uint32_t re_get_cased(uint32_t codepoint);
uint32_t re_get_case_ignorable(uint32_t codepoint);
uint32_t re_get_changes_when_casefolded(uint32_t codepoint);
uint32_t re_get_changes_when_casemapped(uint32_t codepoint);
uint32_t re_get_changes_when_lowercased(uint32_t codepoint);
uint32_t re_get_changes_when_titlecased(uint32_t codepoint);
uint32_t re_get_changes_when_uppercased(uint32_t codepoint);
uint32_t re_get_dash(uint32_t codepoint);
uint32_t re_get_decomposition_type(uint32_t codepoint);
uint32_t re_get_default_ignorable_code_point(uint32_t codepoint);
uint32_t re_get_deprecated(uint32_t codepoint);
uint32_t re_get_diacritic(uint32_t codepoint);
uint32_t re_get_east_asian_width(uint32_t codepoint);
uint32_t re_get_emoji(uint32_t codepoint);
uint32_t re_get_emoji_component(uint32_t codepoint);
uint32_t re_get_emoji_modifier(uint32_t codepoint);
uint32_t re_get_emoji_modifier_base(uint32_t codepoint);
uint32_t re_get_emoji_presentation(uint32_t codepoint);
uint32_t re_get_extended_pictographic(uint32_t codepoint);
uint32_t re_get_extender(uint32_t codepoint);
uint32_t re_get_general_category(uint32_t codepoint);
uint32_t re_get_graph(uint32_t codepoint);
uint32_t re_get_grapheme_base(uint32_t codepoint);
uint32_t re_get_grapheme_cluster_break(uint32_t codepoint);
uint32_t re_get_grapheme_extend(uint32_t codepoint);
uint32_t re_get_grapheme_link(uint32_t codepoint);
uint32_t re_get_hangul_syllable_type(uint32_t codepoint);
uint32_t re_get_hex_digit(uint32_t codepoint);
uint32_t re_get_horiz_space(uint32_t codepoint);
uint32_t re_get_hyphen(uint32_t codepoint);
uint32_t re_get_id_compat_math_continue(uint32_t codepoint);
uint32_t re_get_id_compat_math_start(uint32_t codepoint);
uint32_t re_get_id_continue(uint32_t codepoint);
uint32_t re_get_ideographic(uint32_t codepoint);
uint32_t re_get_ids_binary_operator(uint32_t codepoint);
uint32_t re_get_id_start(uint32_t codepoint);
uint32_t re_get_ids_trinary_operator(uint32_t codepoint);
uint32_t re_get_ids_unary_operator(uint32_t codepoint);
uint32_t re_get_indic_conjunct_break(uint32_t codepoint);
uint32_t re_get_indic_positional_category(uint32_t codepoint);
uint32_t re_get_indic_syllabic_category(uint32_t codepoint);
uint32_t re_get_join_control(uint32_t codepoint);
uint32_t re_get_joining_group(uint32_t codepoint);
uint32_t re_get_joining_type(uint32_t codepoint);
uint32_t re_get_line_break(uint32_t codepoint);
uint32_t re_get_logical_order_exception(uint32_t codepoint);
uint32_t re_get_lowercase(uint32_t codepoint);
uint32_t re_get_math(uint32_t codepoint);
uint32_t re_get_modifier_combining_mark(uint32_t codepoint);
uint32_t re_get_nfc_quick_check(uint32_t codepoint);
uint32_t re_get_nfd_quick_check(uint32_t codepoint);
uint32_t re_get_nfkc_quick_check(uint32_t codepoint);
uint32_t re_get_nfkd_quick_check(uint32_t codepoint);
uint32_t re_get_noncharacter_code_point(uint32_t codepoint);
uint32_t re_get_numeric_type(uint32_t codepoint);
uint32_t re_get_numeric_value(uint32_t codepoint);
uint32_t re_get_other_alphabetic(uint32_t codepoint);
uint32_t re_get_other_default_ignorable_code_point(uint32_t codepoint);
uint32_t re_get_other_grapheme_extend(uint32_t codepoint);
uint32_t re_get_other_id_continue(uint32_t codepoint);
uint32_t re_get_other_id_start(uint32_t codepoint);
uint32_t re_get_other_lowercase(uint32_t codepoint);
uint32_t re_get_other_math(uint32_t codepoint);
uint32_t re_get_other_uppercase(uint32_t codepoint);
uint32_t re_get_pattern_syntax(uint32_t codepoint);
uint32_t re_get_pattern_white_space(uint32_t codepoint);
uint32_t re_get_posix_alnum(uint32_t codepoint);
uint32_t re_get_posix_digit(uint32_t codepoint);
uint32_t re_get_posix_punct(uint32_t codepoint);
uint32_t re_get_posix_xdigit(uint32_t codepoint);
uint32_t re_get_prepended_concatenation_mark(uint32_t codepoint);
uint32_t re_get_print(uint32_t codepoint);
uint32_t re_get_quotation_mark(uint32_t codepoint);
uint32_t re_get_radical(uint32_t codepoint);
uint32_t re_get_regional_indicator(uint32_t codepoint);
uint32_t re_get_script(uint32_t codepoint);
int re_get_script_extensions(uint32_t codepoint, uint8_t* scripts);
uint32_t re_get_sentence_break(uint32_t codepoint);
uint32_t re_get_sentence_terminal(uint32_t codepoint);
uint32_t re_get_soft_dotted(uint32_t codepoint);
uint32_t re_get_terminal_punctuation(uint32_t codepoint);
uint32_t re_get_unified_ideograph(uint32_t codepoint);
uint32_t re_get_uppercase(uint32_t codepoint);
uint32_t re_get_variation_selector(uint32_t codepoint);
uint32_t re_get_vert_space(uint32_t codepoint);
uint32_t re_get_white_space(uint32_t codepoint);
uint32_t re_get_word(uint32_t codepoint);
uint32_t re_get_word_break(uint32_t codepoint);
uint32_t re_get_xdigit(uint32_t codepoint);
uint32_t re_get_xid_continue(uint32_t codepoint);
uint32_t re_get_xid_start(uint32_t codepoint);
int re_get_all_cases(uint32_t codepoint, uint32_t* cases);
uint32_t re_get_simple_case_folding(uint32_t codepoint);
int re_get_full_case_folding(uint32_t codepoint, uint32_t* folded);
