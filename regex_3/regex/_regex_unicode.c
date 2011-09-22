#include "_regex_unicode.h"

#define RE_PROP_CN 0
#define RE_PROP_LU 1
#define RE_PROP_LL 2
#define RE_PROP_LT 3
#define RE_PROP_LM 4
#define RE_PROP_LO 5
#define RE_PROP_MN 6
#define RE_PROP_ME 7
#define RE_PROP_MC 8
#define RE_PROP_ND 9
#define RE_PROP_NL 10
#define RE_PROP_NO 11
#define RE_PROP_ZS 12
#define RE_PROP_ZL 13
#define RE_PROP_ZP 14
#define RE_PROP_CC 15
#define RE_PROP_CF 16
#define RE_PROP_CO 17
#define RE_PROP_CS 18
#define RE_PROP_PD 19
#define RE_PROP_PS 20
#define RE_PROP_PE 21
#define RE_PROP_PC 22
#define RE_PROP_PO 23
#define RE_PROP_SM 24
#define RE_PROP_SC 25
#define RE_PROP_SK 26
#define RE_PROP_SO 27
#define RE_PROP_PI 28
#define RE_PROP_PF 29

#define RE_BLANK_MASK ((1 << RE_PROP_ZL) | (1 << RE_PROP_ZP))
#define RE_GRAPH_MASK ((1 << RE_PROP_CC) | (1 << RE_PROP_CS) | (1 << RE_PROP_CN))
#define RE_WORD_MASK (RE_PROP_M_MASK | (1 << RE_PROP_ND) | (1 << RE_PROP_PC))

typedef struct RE_AllCases {
    RE_INT32 diffs[RE_MAX_CASES - 1];
} RE_AllCases;

typedef struct RE_FullCaseFolding {
    RE_INT32 diff;
    RE_UINT16 codepoints[RE_MAX_FOLDED - 1];
} RE_FullCaseFolding;

/* strings. */

char* re_strings[] = {
    "-1/2",
    "0",
    "1",
    "1/10",
    "1/16",
    "1/2",
    "1/3",
    "1/4",
    "1/5",
    "1/6",
    "1/7",
    "1/8",
    "1/9",
    "10",
    "100",
    "1000",
    "10000",
    "100000",
    "100000000",
    "1000000000000",
    "103",
    "107",
    "11",
    "11/2",
    "118",
    "12",
    "122",
    "129",
    "13",
    "13/2",
    "130",
    "132",
    "14",
    "15",
    "15/2",
    "16",
    "17",
    "17/2",
    "18",
    "19",
    "2",
    "2/3",
    "2/5",
    "20",
    "200",
    "2000",
    "20000",
    "202",
    "21",
    "214",
    "216",
    "218",
    "22",
    "220",
    "222",
    "224",
    "226",
    "228",
    "23",
    "230",
    "232",
    "233",
    "234",
    "24",
    "240",
    "25",
    "26",
    "27",
    "28",
    "29",
    "3",
    "3/16",
    "3/2",
    "3/4",
    "3/5",
    "3/8",
    "30",
    "300",
    "3000",
    "30000",
    "31",
    "32",
    "33",
    "34",
    "35",
    "36",
    "37",
    "38",
    "39",
    "4",
    "4/5",
    "40",
    "400",
    "4000",
    "40000",
    "41",
    "42",
    "43",
    "44",
    "45",
    "46",
    "47",
    "48",
    "49",
    "5",
    "5/2",
    "5/6",
    "5/8",
    "50",
    "500",
    "5000",
    "50000",
    "6",
    "60",
    "600",
    "6000",
    "60000",
    "7",
    "7/2",
    "7/8",
    "70",
    "700",
    "7000",
    "70000",
    "8",
    "80",
    "800",
    "8000",
    "80000",
    "84",
    "9",
    "9/2",
    "90",
    "900",
    "9000",
    "90000",
    "91",
    "A",
    "ABOVE",
    "ABOVELEFT",
    "ABOVERIGHT",
    "AEGEANNUMBERS",
    "AHEX",
    "AI",
    "AIN",
    "AL",
    "ALAPH",
    "ALCHEMICALSYMBOLS",
    "ALEF",
    "ALETTER",
    "ALNUM",
    "ALPHA",
    "ALPHABETIC",
    "ALPHABETICPRESENTATIONFORMS",
    "ALPHANUMERIC",
    "AMBIGUOUS",
    "AN",
    "ANCIENTGREEKMUSICALNOTATION",
    "ANCIENTGREEKNUMBERS",
    "ANCIENTSYMBOLS",
    "ANY",
    "AR",
    "ARAB",
    "ARABIC",
    "ARABICLETTER",
    "ARABICNUMBER",
    "ARABICPRESENTATIONFORMSA",
    "ARABICPRESENTATIONFORMSB",
    "ARABICSUPPLEMENT",
    "ARMENIAN",
    "ARMI",
    "ARMN",
    "ARROWS",
    "ASCII",
    "ASCIIHEXDIGIT",
    "ASSIGNED",
    "AT",
    "ATA",
    "ATAR",
    "ATB",
    "ATERM",
    "ATTACHEDABOVE",
    "ATTACHEDABOVERIGHT",
    "ATTACHEDBELOW",
    "AVAGRAHA",
    "AVESTAN",
    "AVST",
    "B",
    "B2",
    "BA",
    "BALI",
    "BALINESE",
    "BAMU",
    "BAMUM",
    "BAMUMSUPPLEMENT",
    "BASICLATIN",
    "BATAK",
    "BATK",
    "BB",
    "BC",
    "BEH",
    "BELOW",
    "BELOWLEFT",
    "BELOWRIGHT",
    "BENG",
    "BENGALI",
    "BETH",
    "BIDIC",
    "BIDICLASS",
    "BIDICONTROL",
    "BIDIM",
    "BIDIMIRRORED",
    "BINDU",
    "BK",
    "BL",
    "BLANK",
    "BLK",
    "BLOCK",
    "BLOCKELEMENTS",
    "BN",
    "BOPO",
    "BOPOMOFO",
    "BOPOMOFOEXTENDED",
    "BOTTOM",
    "BOTTOMANDRIGHT",
    "BOUNDARYNEUTRAL",
    "BOXDRAWING",
    "BR",
    "BRAH",
    "BRAHMI",
    "BRAI",
    "BRAILLE",
    "BRAILLEPATTERNS",
    "BREAKAFTER",
    "BREAKBEFORE",
    "BREAKBOTH",
    "BREAKSYMBOLS",
    "BUGI",
    "BUGINESE",
    "BUHD",
    "BUHID",
    "BURUSHASKIYEHBARREE",
    "BYZANTINEMUSICALSYMBOLS",
    "C",
    "C&",
    "CAN",
    "CANADIANABORIGINAL",
    "CANADIANSYLLABICS",
    "CANONICAL",
    "CANONICALCOMBININGCLASS",
    "CANS",
    "CARI",
    "CARIAN",
    "CARRIAGERETURN",
    "CASED",
    "CASEIGNORABLE",
    "CB",
    "CC",
    "CCC",
    "CF",
    "CHAM",
    "CHANGESWHENCASEFOLDED",
    "CHANGESWHENCASEMAPPED",
    "CHANGESWHENLOWERCASED",
    "CHANGESWHENTITLECASED",
    "CHANGESWHENUPPERCASED",
    "CHER",
    "CHEROKEE",
    "CI",
    "CIRCLE",
    "CJKCOMPATIBILITY",
    "CJKCOMPATIBILITYFORMS",
    "CJKCOMPATIBILITYIDEOGRAPHS",
    "CJKCOMPATIBILITYIDEOGRAPHSSUPPLEMENT",
    "CJKRADICALSSUPPLEMENT",
    "CJKSTROKES",
    "CJKSYMBOLSANDPUNCTUATION",
    "CJKUNIFIEDIDEOGRAPHS",
    "CJKUNIFIEDIDEOGRAPHSEXTENSIONA",
    "CJKUNIFIEDIDEOGRAPHSEXTENSIONB",
    "CJKUNIFIEDIDEOGRAPHSEXTENSIONC",
    "CJKUNIFIEDIDEOGRAPHSEXTENSIOND",
    "CL",
    "CLOSE",
    "CLOSEPARENTHESIS",
    "CLOSEPUNCTUATION",
    "CM",
    "CN",
    "CNTRL",
    "CO",
    "COM",
    "COMBININGDIACRITICALMARKS",
    "COMBININGDIACRITICALMARKSFORSYMBOLS",
    "COMBININGDIACRITICALMARKSSUPPLEMENT",
    "COMBININGHALFMARKS",
    "COMBININGMARK",
    "COMBININGMARKSFORSYMBOLS",
    "COMMON",
    "COMMONINDICNUMBERFORMS",
    "COMMONSEPARATOR",
    "COMPAT",
    "COMPLEXCONTEXT",
    "CONNECTORPUNCTUATION",
    "CONSONANT",
    "CONSONANTDEAD",
    "CONSONANTFINAL",
    "CONSONANTHEADLETTER",
    "CONSONANTMEDIAL",
    "CONSONANTPLACEHOLDER",
    "CONSONANTREPHA",
    "CONSONANTSUBJOINED",
    "CONTINGENTBREAK",
    "CONTROL",
    "CONTROLPICTURES",
    "COPT",
    "COPTIC",
    "COUNTINGRODNUMERALS",
    "CP",
    "CPRT",
    "CR",
    "CS",
    "CUNEIFORM",
    "CUNEIFORMNUMBERSANDPUNCTUATION",
    "CURRENCYSYMBOL",
    "CURRENCYSYMBOLS",
    "CWCF",
    "CWCM",
    "CWL",
    "CWT",
    "CWU",
    "CYPRIOT",
    "CYPRIOTSYLLABARY",
    "CYRILLIC",
    "CYRILLICEXTENDEDA",
    "CYRILLICEXTENDEDB",
    "CYRILLICSUPPLEMENT",
    "CYRILLICSUPPLEMENTARY",
    "CYRL",
    "D",
    "DA",
    "DAL",
    "DALATHRISH",
    "DASH",
    "DASHPUNCTUATION",
    "DB",
    "DE",
    "DECIMAL",
    "DECIMALNUMBER",
    "DECOMPOSITIONTYPE",
    "DEFAULTIGNORABLECODEPOINT",
    "DEP",
    "DEPRECATED",
    "DESERET",
    "DEVA",
    "DEVANAGARI",
    "DEVANAGARIEXTENDED",
    "DI",
    "DIA",
    "DIACRITIC",
    "DIGIT",
    "DINGBATS",
    "DOMINOTILES",
    "DOUBLEABOVE",
    "DOUBLEBELOW",
    "DSRT",
    "DT",
    "DUALJOINING",
    "E",
    "EA",
    "EASTASIANWIDTH",
    "EGYP",
    "EGYPTIANHIEROGLYPHS",
    "EMOTICONS",
    "EN",
    "ENC",
    "ENCLOSEDALPHANUMERICS",
    "ENCLOSEDALPHANUMERICSUPPLEMENT",
    "ENCLOSEDCJKLETTERSANDMONTHS",
    "ENCLOSEDIDEOGRAPHICSUPPLEMENT",
    "ENCLOSINGMARK",
    "ES",
    "ET",
    "ETHI",
    "ETHIOPIC",
    "ETHIOPICEXTENDED",
    "ETHIOPICEXTENDEDA",
    "ETHIOPICSUPPLEMENT",
    "EUROPEANNUMBER",
    "EUROPEANSEPARATOR",
    "EUROPEANTERMINATOR",
    "EX",
    "EXCLAMATION",
    "EXT",
    "EXTEND",
    "EXTENDER",
    "EXTENDNUMLET",
    "F",
    "FALSE",
    "FARSIYEH",
    "FE",
    "FEH",
    "FIN",
    "FINAL",
    "FINALPUNCTUATION",
    "FINALSEMKATH",
    "FO",
    "FONT",
    "FORMAT",
    "FRA",
    "FRACTION",
    "FULLWIDTH",
    "GAF",
    "GAMAL",
    "GC",
    "GCB",
    "GENERALCATEGORY",
    "GENERALPUNCTUATION",
    "GEOMETRICSHAPES",
    "GEOR",
    "GEORGIAN",
    "GEORGIANSUPPLEMENT",
    "GL",
    "GLAG",
    "GLAGOLITIC",
    "GLUE",
    "GOTH",
    "GOTHIC",
    "GRAPH",
    "GRAPHEMEBASE",
    "GRAPHEMECLUSTERBREAK",
    "GRAPHEMEEXTEND",
    "GRAPHEMELINK",
    "GRBASE",
    "GREEK",
    "GREEKANDCOPTIC",
    "GREEKEXTENDED",
    "GREK",
    "GREXT",
    "GRLINK",
    "GUJARATI",
    "GUJR",
    "GURMUKHI",
    "GURU",
    "H",
    "H2",
    "H3",
    "HAH",
    "HALFWIDTH",
    "HALFWIDTHANDFULLWIDTHFORMS",
    "HAMZAONHEHGOAL",
    "HAN",
    "HANG",
    "HANGUL",
    "HANGULCOMPATIBILITYJAMO",
    "HANGULJAMO",
    "HANGULJAMOEXTENDEDA",
    "HANGULJAMOEXTENDEDB",
    "HANGULSYLLABLES",
    "HANGULSYLLABLETYPE",
    "HANI",
    "HANO",
    "HANUNOO",
    "HE",
    "HEBR",
    "HEBREW",
    "HEH",
    "HEHGOAL",
    "HETH",
    "HEX",
    "HEXDIGIT",
    "HIGHPRIVATEUSESURROGATES",
    "HIGHSURROGATES",
    "HIRA",
    "HIRAGANA",
    "HST",
    "HY",
    "HYPHEN",
    "ID",
    "IDC",
    "IDCONTINUE",
    "IDEO",
    "IDEOGRAPHIC",
    "IDEOGRAPHICDESCRIPTIONCHARACTERS",
    "IDS",
    "IDSB",
    "IDSBINARYOPERATOR",
    "IDST",
    "IDSTART",
    "IDSTRINARYOPERATOR",
    "IMPERIALARAMAIC",
    "IN",
    "INDICMATRACATEGORY",
    "INDICSYLLABICCATEGORY",
    "INFIXNUMERIC",
    "INHERITED",
    "INIT",
    "INITIAL",
    "INITIALPUNCTUATION",
    "INSCRIPTIONALPAHLAVI",
    "INSCRIPTIONALPARTHIAN",
    "INSEPARABLE",
    "INSEPERABLE",
    "INVISIBLE",
    "IOTASUBSCRIPT",
    "IPAEXTENSIONS",
    "IS",
    "ISO",
    "ISOLATED",
    "ITAL",
    "JAVA",
    "JAVANESE",
    "JG",
    "JL",
    "JOINC",
    "JOINCAUSING",
    "JOINCONTROL",
    "JOININGGROUP",
    "JOININGTYPE",
    "JT",
    "JV",
    "KA",
    "KAF",
    "KAITHI",
    "KALI",
    "KANA",
    "KANASUPPLEMENT",
    "KANAVOICING",
    "KANBUN",
    "KANGXIRADICALS",
    "KANNADA",
    "KAPH",
    "KATAKANA",
    "KATAKANAPHONETICEXTENSIONS",
    "KAYAHLI",
    "KHAPH",
    "KHAR",
    "KHAROSHTHI",
    "KHMER",
    "KHMERSYMBOLS",
    "KHMR",
    "KNDA",
    "KNOTTEDHEH",
    "KTHI",
    "KV",
    "L",
    "L&",
    "LAM",
    "LAMADH",
    "LANA",
    "LAO",
    "LAOO",
    "LATIN",
    "LATIN1",
    "LATIN1SUPPLEMENT",
    "LATINEXTENDEDA",
    "LATINEXTENDEDADDITIONAL",
    "LATINEXTENDEDB",
    "LATINEXTENDEDC",
    "LATINEXTENDEDD",
    "LATN",
    "LB",
    "LE",
    "LEADINGJAMO",
    "LEFT",
    "LEFTANDRIGHT",
    "LEFTTORIGHT",
    "LEFTTORIGHTEMBEDDING",
    "LEFTTORIGHTOVERRIDE",
    "LEPC",
    "LEPCHA",
    "LETTER",
    "LETTERLIKESYMBOLS",
    "LETTERNUMBER",
    "LF",
    "LIMB",
    "LIMBU",
    "LINB",
    "LINEARB",
    "LINEARBIDEOGRAMS",
    "LINEARBSYLLABARY",
    "LINEBREAK",
    "LINEFEED",
    "LINESEPARATOR",
    "LISU",
    "LL",
    "LM",
    "LO",
    "LOE",
    "LOGICALORDEREXCEPTION",
    "LOWER",
    "LOWERCASE",
    "LOWERCASELETTER",
    "LOWSURROGATES",
    "LRE",
    "LRO",
    "LT",
    "LU",
    "LV",
    "LVSYLLABLE",
    "LVT",
    "LVTSYLLABLE",
    "LYCI",
    "LYCIAN",
    "LYDI",
    "LYDIAN",
    "M",
    "M&",
    "MAHJONGTILES",
    "MALAYALAM",
    "MAND",
    "MANDAIC",
    "MANDATORYBREAK",
    "MARK",
    "MATH",
    "MATHEMATICALALPHANUMERICSYMBOLS",
    "MATHEMATICALOPERATORS",
    "MATHSYMBOL",
    "MB",
    "MC",
    "ME",
    "MED",
    "MEDIAL",
    "MEEM",
    "MEETEIMAYEK",
    "MIDLETTER",
    "MIDNUM",
    "MIDNUMLET",
    "MIM",
    "MISCELLANEOUSMATHEMATICALSYMBOLSA",
    "MISCELLANEOUSMATHEMATICALSYMBOLSB",
    "MISCELLANEOUSSYMBOLS",
    "MISCELLANEOUSSYMBOLSANDARROWS",
    "MISCELLANEOUSSYMBOLSANDPICTOGRAPHS",
    "MISCELLANEOUSTECHNICAL",
    "ML",
    "MLYM",
    "MN",
    "MODIFIERLETTER",
    "MODIFIERSYMBOL",
    "MODIFIERTONELETTERS",
    "MODIFYINGLETTER",
    "MONG",
    "MONGOLIAN",
    "MTEI",
    "MUSICALSYMBOLS",
    "MYANMAR",
    "MYANMAREXTENDEDA",
    "MYMR",
    "N",
    "N&",
    "NA",
    "NAN",
    "NAR",
    "NARROW",
    "NB",
    "NCHAR",
    "ND",
    "NEUTRAL",
    "NEWLINE",
    "NEWTAILUE",
    "NEXTLINE",
    "NK",
    "NKO",
    "NKOO",
    "NL",
    "NO",
    "NOBLOCK",
    "NOBREAK",
    "NOJOININGGROUP",
    "NONCHARACTERCODEPOINT",
    "NONE",
    "NONJOINING",
    "NONSPACINGMARK",
    "NONSTARTER",
    "NOON",
    "NOTAPPLICABLE",
    "NOTREORDERED",
    "NR",
    "NS",
    "NSM",
    "NT",
    "NU",
    "NUKTA",
    "NUMBER",
    "NUMBERFORMS",
    "NUMERIC",
    "NUMERICTYPE",
    "NUMERICVALUE",
    "NUN",
    "NV",
    "NYA",
    "OALPHA",
    "ODI",
    "OGAM",
    "OGHAM",
    "OGREXT",
    "OIDC",
    "OIDS",
    "OLCHIKI",
    "OLCK",
    "OLDITALIC",
    "OLDPERSIAN",
    "OLDSOUTHARABIAN",
    "OLDTURKIC",
    "OLETTER",
    "OLOWER",
    "OMATH",
    "ON",
    "OP",
    "OPENPUNCTUATION",
    "OPTICALCHARACTERRECOGNITION",
    "ORIYA",
    "ORKH",
    "ORYA",
    "OSMA",
    "OSMANYA",
    "OTHER",
    "OTHERALPHABETIC",
    "OTHERDEFAULTIGNORABLECODEPOINT",
    "OTHERGRAPHEMEEXTEND",
    "OTHERIDCONTINUE",
    "OTHERIDSTART",
    "OTHERLETTER",
    "OTHERLOWERCASE",
    "OTHERMATH",
    "OTHERNEUTRAL",
    "OTHERNUMBER",
    "OTHERPUNCTUATION",
    "OTHERSYMBOL",
    "OTHERUPPERCASE",
    "OUPPER",
    "OV",
    "OVERLAY",
    "OVERSTRUCK",
    "P",
    "P&",
    "PARAGRAPHSEPARATOR",
    "PATSYN",
    "PATTERNSYNTAX",
    "PATTERNWHITESPACE",
    "PATWS",
    "PC",
    "PD",
    "PDF",
    "PE",
    "PF",
    "PHAG",
    "PHAGSPA",
    "PHAISTOSDISC",
    "PHLI",
    "PHNX",
    "PHOENICIAN",
    "PHONETICEXTENSIONS",
    "PHONETICEXTENSIONSSUPPLEMENT",
    "PI",
    "PLAYINGCARDS",
    "PO",
    "POPDIRECTIONALFORMAT",
    "POSTFIXNUMERIC",
    "PP",
    "PR",
    "PREFIXNUMERIC",
    "PREPEND",
    "PRINT",
    "PRIVATEUSE",
    "PRIVATEUSEAREA",
    "PRTI",
    "PS",
    "PUNCT",
    "PUNCTUATION",
    "QAAC",
    "QAAI",
    "QAF",
    "QAPH",
    "QMARK",
    "QU",
    "QUOTATION",
    "QUOTATIONMARK",
    "R",
    "RADICAL",
    "REGISTERSHIFTER",
    "REH",
    "REJANG",
    "REVERSEDPE",
    "RIGHT",
    "RIGHTJOINING",
    "RIGHTTOLEFT",
    "RIGHTTOLEFTEMBEDDING",
    "RIGHTTOLEFTOVERRIDE",
    "RJNG",
    "RLE",
    "RLO",
    "RUMINUMERALSYMBOLS",
    "RUNIC",
    "RUNR",
    "S",
    "S&",
    "SA",
    "SAD",
    "SADHE",
    "SAMARITAN",
    "SAMR",
    "SARB",
    "SAUR",
    "SAURASHTRA",
    "SB",
    "SC",
    "SCONTINUE",
    "SCRIPT",
    "SD",
    "SE",
    "SEEN",
    "SEGMENTSEPARATOR",
    "SEMKATH",
    "SENTENCEBREAK",
    "SEP",
    "SEPARATOR",
    "SG",
    "SHAVIAN",
    "SHAW",
    "SHIN",
    "SINH",
    "SINHALA",
    "SK",
    "SM",
    "SMALL",
    "SMALLFORMVARIANTS",
    "SML",
    "SO",
    "SOFTDOTTED",
    "SP",
    "SPACE",
    "SPACESEPARATOR",
    "SPACINGMARK",
    "SPACINGMODIFIERLETTERS",
    "SPECIALS",
    "SQR",
    "SQUARE",
    "ST",
    "STERM",
    "SUB",
    "SUND",
    "SUNDANESE",
    "SUP",
    "SUPER",
    "SUPERSCRIPTSANDSUBSCRIPTS",
    "SUPPLEMENTALARROWSA",
    "SUPPLEMENTALARROWSB",
    "SUPPLEMENTALMATHEMATICALOPERATORS",
    "SUPPLEMENTALPUNCTUATION",
    "SUPPLEMENTARYPRIVATEUSEAREAA",
    "SUPPLEMENTARYPRIVATEUSEAREAB",
    "SURROGATE",
    "SWASHKAF",
    "SY",
    "SYLO",
    "SYLOTINAGRI",
    "SYMBOL",
    "SYRC",
    "SYRIAC",
    "SYRIACWAW",
    "T",
    "TAGALOG",
    "TAGB",
    "TAGBANWA",
    "TAGS",
    "TAH",
    "TAILE",
    "TAITHAM",
    "TAIVIET",
    "TAIXUANJINGSYMBOLS",
    "TALE",
    "TALU",
    "TAMIL",
    "TAML",
    "TAVT",
    "TAW",
    "TEHMARBUTA",
    "TEHMARBUTAGOAL",
    "TELU",
    "TELUGU",
    "TERM",
    "TERMINALPUNCTUATION",
    "TETH",
    "TFNG",
    "TGLG",
    "THAA",
    "THAANA",
    "THAI",
    "TIBETAN",
    "TIBT",
    "TIFINAGH",
    "TITLECASELETTER",
    "TONELETTER",
    "TONEMARK",
    "TOP",
    "TOPANDBOTTOM",
    "TOPANDBOTTOMANDRIGHT",
    "TOPANDLEFT",
    "TOPANDLEFTANDRIGHT",
    "TOPANDRIGHT",
    "TRAILINGJAMO",
    "TRANSPARENT",
    "TRANSPORTANDMAPSYMBOLS",
    "TRUE",
    "U",
    "UGAR",
    "UGARITIC",
    "UIDEO",
    "UNASSIGNED",
    "UNIFIEDCANADIANABORIGINALSYLLABICS",
    "UNIFIEDCANADIANABORIGINALSYLLABICSEXTENDED",
    "UNIFIEDIDEOGRAPH",
    "UNKNOWN",
    "UP",
    "UPPER",
    "UPPERCASE",
    "UPPERCASELETTER",
    "V",
    "VAI",
    "VAII",
    "VARIATIONSELECTOR",
    "VARIATIONSELECTORS",
    "VARIATIONSELECTORSSUPPLEMENT",
    "VEDICEXTENSIONS",
    "VERT",
    "VERTICAL",
    "VERTICALFORMS",
    "VIRAMA",
    "VISARGA",
    "VISUALORDERLEFT",
    "VOWEL",
    "VOWELDEPENDENT",
    "VOWELINDEPENDENT",
    "VOWELJAMO",
    "VR",
    "VS",
    "W",
    "WAW",
    "WB",
    "WHITESPACE",
    "WIDE",
    "WJ",
    "WORD",
    "WORDBREAK",
    "WORDJOINER",
    "WS",
    "WSPACE",
    "XDIGIT",
    "XIDC",
    "XIDCONTINUE",
    "XIDS",
    "XIDSTART",
    "XPEO",
    "XSUX",
    "XX",
    "Y",
    "YEH",
    "YEHBARREE",
    "YEHWITHTAIL",
    "YES",
    "YI",
    "YIII",
    "YIJINGHEXAGRAMSYMBOLS",
    "YIRADICALS",
    "YISYLLABLES",
    "YUDH",
    "YUDHHE",
    "Z",
    "Z&",
    "ZAIN",
    "ZHAIN",
    "ZINH",
    "ZL",
    "ZP",
    "ZS",
    "ZW",
    "ZWSPACE",
    "ZYYY",
    "ZZZZ",
};

/* strings: 8739 bytes. */

/* properties. */

RE_Property re_properties[] = {
    {415,  0,  0},
    {413,  0,  0},
    {217,  1,  1},
    {216,  1,  1},
    {808,  2,  2},
    {806,  2,  2},
    {944,  3,  3},
    {939,  3,  3},
    {429,  4,  4},
    {414,  4,  4},
    {814,  5,  5},
    {805,  5,  5},
    {613,  6,  6},
    {152,  7,  6},
    {151,  7,  6},
    {590,  8,  6},
    {589,  8,  6},
    {916,  9,  6},
    {915,  9,  6},
    {254, 10,  6},
    {255, 11,  6},
    {268, 11,  6},
    {263, 12,  6},
    {327, 12,  6},
    {265, 13,  6},
    {329, 13,  6},
    {264, 14,  6},
    {328, 14,  6},
    {261, 15,  6},
    {325, 15,  6},
    {262, 16,  6},
    {326, 16,  6},
    {487, 17,  6},
    {483, 17,  6},
    {479, 18,  6},
    {478, 18,  6},
    {952, 19,  6},
    {951, 19,  6},
    {950, 20,  6},
    {949, 20,  6},
    {349, 21,  6},
    {356, 21,  6},
    {430, 22,  6},
    {437, 22,  6},
    {428, 23,  6},
    {432, 23,  6},
    {431, 24,  6},
    {438, 24,  6},
    {940, 25,  6},
    {947, 25,  6},
    {831, 25,  6},
    {209, 26,  6},
    {207, 26,  6},
    {515, 27,  6},
    {513, 27,  6},
    {342, 28,  6},
    {476, 29,  6},
    {777, 30,  6},
    {774, 30,  6},
    {882, 31,  6},
    {881, 31,  6},
    {724, 32,  6},
    {706, 32,  6},
    {469, 33,  6},
    {468, 33,  6},
    {174, 34,  6},
    {142, 34,  6},
    {717, 35,  6},
    {691, 35,  6},
    {481, 36,  6},
    {480, 36,  6},
    {358, 37,  6},
    {357, 37,  6},
    {394, 38,  6},
    {392, 38,  6},
    {723, 39,  6},
    {705, 39,  6},
    {729, 40,  6},
    {730, 40,  6},
    {669, 41,  6},
    {655, 41,  6},
    {719, 42,  6},
    {695, 42,  6},
    {485, 43,  6},
    {484, 43,  6},
    {488, 44,  6},
    {486, 44,  6},
    {779, 45,  6},
    {912, 46,  6},
    {908, 46,  6},
    {718, 47,  6},
    {692, 47,  6},
    {351, 48,  6},
    {350, 48,  6},
    {829, 49,  6},
    {809, 49,  6},
    {588, 50,  6},
    {587, 50,  6},
    {721, 51,  6},
    {697, 51,  6},
    {720, 52,  6},
    {696, 52,  6},
    {839, 53,  6},
    {921, 54,  6},
    {936, 54,  6},
    {739, 55,  6},
    {740, 55,  6},
    {738, 56,  6},
    {737, 56,  6},
    {458, 57,  7},
    {474, 57,  7},
    {208, 58,  8},
    {199, 58,  8},
    {249, 59,  9},
    {258, 59,  9},
    {348, 60, 10},
    {365, 60, 10},
    {369, 61, 11},
    {368, 61, 11},
    {516, 62, 12},
    {511, 62, 12},
    {517, 63, 13},
    {518, 63, 13},
    {580, 64, 14},
    {560, 64, 14},
    {686, 65, 15},
    {680, 65, 15},
    {687, 66, 16},
    {689, 66, 16},
    {211, 67,  6},
    {210, 67,  6},
    {491, 68, 17},
    {492, 69, 18},
    {154, 70,  6},
    {150, 70,  6},
    {160, 71,  6},
    {173, 72,  6},
    {175, 73,  6},
    {215, 74,  6},
    {427, 75,  6},
    {763, 76,  6},
    {943, 77,  6},
    {948, 78,  6},
};

/* properties: 572 bytes. */

/* property values. */

RE_PropertyValue re_property_values[] = {
    {287,  0,   0},
    {909,  0,   0},
    {596,  0,   1},
    {917,  0,   1},
    {584,  0,   2},
    {591,  0,   2},
    {595,  0,   3},
    {892,  0,   3},
    {585,  0,   4},
    {637,  0,   4},
    {586,  0,   5},
    {722,  0,   5},
    {636,  0,   6},
    {672,  0,   6},
    {619,  0,   7},
    {379,  0,   7},
    {618,  0,   8},
    {833,  0,   8},
    {656,  0,   9},
    {359,  0,   9},
    {347,  0,   9},
    {664,  0,  10},
    {572,  0,  10},
    {665,  0,  11},
    {726,  0,  11},
    {975,  0,  12},
    {832,  0,  12},
    {973,  0,  13},
    {582,  0,  13},
    {974,  0,  14},
    {736,  0,  14},
    {257,  0,  15},
    {312,  0,  15},
    {288,  0,  15},
    {259,  0,  16},
    {407,  0,  16},
    {289,  0,  17},
    {764,  0,  17},
    {320,  0,  18},
    {852,  0,  18},
    {742,  0,  19},
    {343,  0,  19},
    {767,  0,  20},
    {709,  0,  20},
    {744,  0,  21},
    {285,  0,  21},
    {741,  0,  22},
    {302,  0,  22},
    {756,  0,  23},
    {727,  0,  23},
    {824,  0,  24},
    {616,  0,  24},
    {806,  0,  25},
    {323,  0,  25},
    {823,  0,  26},
    {638,  0,  26},
    {828,  0,  27},
    {728,  0,  27},
    {754,  0,  28},
    {497,  0,  28},
    {745,  0,  29},
    {403,  0,  29},
    {243,  0,  30},
    {716,  0,  30},
    {244,  0,  30},
    {544,  0,  31},
    {545,  0,  31},
    {570,  0,  31},
    {605,  0,  32},
    {606,  0,  32},
    {612,  0,  32},
    {648,  0,  33},
    {683,  0,  33},
    {649,  0,  33},
    {734,  0,  34},
    {768,  0,  34},
    {769,  0,  34},
    {735,  0,  34},
    {795,  0,  35},
    {857,  0,  35},
    {796,  0,  35},
    {968,  0,  36},
    {969,  0,  36},
    {816,  0,  36},
    {666,  1,   0},
    {195,  1,   1},
    {173,  1,   1},
    {553,  1,   2},
    {552,  1,   2},
    {554,  1,   3},
    {556,  1,   4},
    {504,  1,   5},
    {834,  1,   6},
    {291,  1,   7},
    {434,  1,   8},
    {433,  1,   8},
    {332,  1,   9},
    {335,  1,  10},
    {336,  1,  10},
    {169,  1,  11},
    {464,  1,  12},
    {163,  1,  13},
    {859,  1,  14},
    {168,  1,  15},
    {887,  1,  16},
    {662,  1,  17},
    {800,  1,  18},
    {610,  1,  19},
    {354,  1,  20},
    {205,  1,  21},
    {441,  1,  22},
    {439,  1,  23},
    {711,  1,  24},
    {873,  1,  25},
    {880,  1,  26},
    {529,  1,  27},
    {608,  1,  28},
    {822,  1,  29},
    {888,  1,  30},
    {549,  1,  31},
    {889,  1,  32},
    {645,  1,  33},
    {419,  1,  34},
    {454,  1,  35},
    {383,  1,  36},
    {386,  1,  37},
    {267,  1,  38},
    {910,  1,  39},
    {247,  1,  39},
    {694,  1,  40},
    {793,  1,  41},
    {862,  1,  42},
    {461,  1,  43},
    {240,  1,  44},
    {864,  1,  45},
    {537,  1,  46},
    {642,  1,  47},
    {911,  1,  48},
    {575,  1,  49},
    {867,  1,  50},
    {659,  1,  51},
    {538,  1,  52},
    {238,  1,  53},
    {868,  1,  54},
    {191,  1,  55},
    {842,  1,  56},
    {196,  1,  57},
    {569,  1,  58},
    {698,  1,  59},
    {924,  1,  60},
    {752,  1,  61},
    {753,  1,  62},
    {293,  1,  63},
    {555,  1,  64},
    {435,  1,  65},
    {416,  1,  66},
    {845,  1,  67},
    {324,  1,  68},
    {292,  1,  69},
    {296,  1,  69},
    {571,  1,  70},
    {684,  1,  71},
    {172,  1,  72},
    {615,  1,  73},
    {633,  1,  74},
    {313,  1,  75},
    {710,  1,  76},
    {375,  1,  77},
    {226,  1,  78},
    {218,  1,  79},
    {417,  1,  80},
    {630,  1,  81},
    {360,  1,  82},
    {628,  1,  83},
    {846,  1,  84},
    {232,  1,  85},
    {847,  1,  86},
    {629,  1,  87},
    {848,  1,  88},
    {631,  1,  89},
    {423,  1,  90},
    {557,  1,  91},
    {315,  1,  92},
    {420,  1,  93},
    {891,  1,  94},
    {384,  1,  95},
    {333,  1,  96},
    {849,  1,  97},
    {274,  1,  98},
    {528,  1,  99},
    {482,  1, 100},
    {276,  1, 101},
    {473,  1, 102},
    {531,  1, 103},
    {221,  1, 104},
    {453,  1, 105},
    {527,  1, 106},
    {222,  1, 107},
    {275,  1, 108},
    {532,  1, 109},
    {377,  1, 110},
    {270,  1, 111},
    {278,  1, 112},
    {963,  1, 113},
    {277,  1, 114},
    {965,  1, 115},
    {964,  1, 116},
    {583,  1, 117},
    {919,  1, 118},
    {334,  1, 119},
    {193,  1, 120},
    {639,  1, 121},
    {558,  1, 122},
    {856,  1, 123},
    {298,  1, 124},
    {747,  1, 125},
    {804,  1, 126},
    {355,  1, 127},
    {533,  1, 128},
    {782,  1, 129},
    {455,  1, 130},
    {510,  1, 131},
    {260,  1, 132},
    {646,  1, 133},
    {869,  1, 134},
    {385,  1, 135},
    {623,  1, 136},
    {457,  1, 137},
    {456,  1, 138},
    {471,  1, 139},
    {470,  1, 140},
    {592,  1, 141},
    {765,  1, 142},
    {764,  1, 142},
    {272,  1, 143},
    {153,  1, 144},
    {166,  1, 145},
    {922,  1, 146},
    {927,  1, 147},
    {294,  1, 148},
    {271,  1, 149},
    {826,  1, 150},
    {167,  1, 151},
    {448,  1, 152},
    {835,  1, 153},
    {579,  1, 154},
    {578,  1, 155},
    {141,  1, 156},
    {158,  1, 157},
    {159,  1, 158},
    {748,  1, 159},
    {602,  1, 160},
    {252,  1, 161},
    {700,  1, 162},
    {426,  1, 163},
    {907,  1, 164},
    {701,  1, 165},
    {352,  1, 166},
    {818,  1, 167},
    {715,  1, 168},
    {331,  1, 169},
    {489,  1, 170},
    {751,  1, 171},
    {604,  1, 172},
    {536,  1, 173},
    {702,  1, 174},
    {185,  1, 175},
    {499,  1, 176},
    {498,  1, 177},
    {703,  1, 178},
    {792,  1, 179},
    {229,  1, 180},
    {522,  1, 181},
    {321,  1, 182},
    {322,  1, 183},
    {371,  1, 184},
    {194,  1, 185},
    {525,  1, 186},
    {242,  1, 187},
    {644,  1, 188},
    {157,  1, 189},
    {870,  1, 190},
    {316,  1, 191},
    {614,  1, 192},
    {607,  1, 193},
    {361,  1, 194},
    {755,  1, 195},
    {376,  1, 196},
    {378,  1, 197},
    {632,  1, 198},
    {372,  1, 199},
    {903,  1, 200},
    {147,  1, 201},
    {279,  1, 202},
    {280,  1, 203},
    {281,  1, 204},
    {273,  1, 205},
    {865,  1, 206},
    {923,  1, 207},
    {850,  1, 208},
    {851,  1, 209},
    {913,  2,   0},
    {979,  2,   0},
    {297,  2,   1},
    {978,  2,   1},
    {551,  2,   2},
    {559,  2,   2},
    {433,  2,   3},
    {436,  2,   3},
    {332,  2,   4},
    {337,  2,   4},
    {169,  2,   5},
    {171,  2,   5},
    {464,  2,   6},
    {463,  2,   6},
    {163,  2,   7},
    {162,  2,   7},
    {859,  2,   8},
    {858,  2,   8},
    {887,  2,   9},
    {886,  2,   9},
    {354,  2,  10},
    {353,  2,  10},
    {205,  2,  11},
    {204,  2,  11},
    {441,  2,  12},
    {442,  2,  12},
    {439,  2,  13},
    {440,  2,  13},
    {711,  2,  14},
    {713,  2,  14},
    {873,  2,  15},
    {874,  2,  15},
    {880,  2,  16},
    {879,  2,  16},
    {529,  2,  17},
    {540,  2,  17},
    {608,  2,  18},
    {635,  2,  18},
    {822,  2,  19},
    {821,  2,  19},
    {888,  2,  20},
    {549,  2,  21},
    {550,  2,  21},
    {889,  2,  22},
    {890,  2,  22},
    {645,  2,  23},
    {647,  2,  23},
    {419,  2,  24},
    {418,  2,  24},
    {452,  2,  25},
    {451,  2,  25},
    {383,  2,  26},
    {382,  2,  26},
    {267,  2,  27},
    {266,  2,  27},
    {246,  2,  28},
    {250,  2,  28},
    {694,  2,  29},
    {693,  2,  29},
    {793,  2,  30},
    {794,  2,  30},
    {537,  2,  31},
    {539,  2,  31},
    {642,  2,  32},
    {641,  2,  32},
    {473,  2,  33},
    {472,  2,  33},
    {531,  2,  34},
    {524,  2,  34},
    {221,  2,  35},
    {220,  2,  35},
    {450,  2,  36},
    {459,  2,  36},
    {961,  2,  37},
    {962,  2,  37},
    {700,  2,  38},
    {508,  2,  38},
    {426,  2,  39},
    {425,  2,  39},
    {352,  2,  40},
    {364,  2,  40},
    {494,  2,  41},
    {771,  2,  41},
    {972,  2,  41},
    {862,  2,  42},
    {885,  2,  42},
    {461,  2,  43},
    {460,  2,  43},
    {240,  2,  44},
    {239,  2,  44},
    {864,  2,  45},
    {863,  2,  45},
    {575,  2,  46},
    {574,  2,  46},
    {867,  2,  47},
    {871,  2,  47},
    {577,  2,  48},
    {576,  2,  48},
    {907,  2,  49},
    {906,  2,  49},
    {818,  2,  50},
    {819,  2,  50},
    {715,  2,  51},
    {714,  2,  51},
    {330,  2,  52},
    {318,  2,  52},
    {231,  2,  53},
    {230,  2,  53},
    {238,  2,  54},
    {237,  2,  54},
    {315,  2,  55},
    {770,  2,  55},
    {314,  2,  55},
    {659,  2,  56},
    {872,  2,  56},
    {423,  2,  57},
    {422,  2,  57},
    {891,  2,  58},
    {884,  2,  58},
    {856,  2,  59},
    {855,  2,  59},
    {701,  2,  60},
    {953,  2,  60},
    {536,  2,  61},
    {535,  2,  61},
    {191,  2,  62},
    {190,  2,  62},
    {321,  2,  63},
    {954,  2,  63},
    {751,  2,  64},
    {750,  2,  64},
    {747,  2,  65},
    {746,  2,  65},
    {662,  2,  66},
    {663,  2,  66},
    {842,  2,  67},
    {841,  2,  67},
    {569,  2,  68},
    {568,  2,  68},
    {698,  2,  69},
    {699,  2,  69},
    {919,  2,  70},
    {920,  2,  70},
    {804,  2,  71},
    {803,  2,  71},
    {533,  2,  72},
    {523,  2,  72},
    {782,  2,  73},
    {789,  2,  73},
    {602,  2,  74},
    {601,  2,  74},
    {252,  2,  75},
    {251,  2,  75},
    {604,  2,  76},
    {603,  2,  76},
    {260,  2,  77},
    {868,  2,  78},
    {548,  2,  78},
    {869,  2,  79},
    {875,  2,  79},
    {185,  2,  80},
    {186,  2,  80},
    {371,  2,  81},
    {370,  2,  81},
    {800,  2,  82},
    {801,  2,  82},
    {583,  2,  83},
    {193,  2,  84},
    {192,  2,  84},
    {510,  2,  85},
    {509,  2,  85},
    {623,  2,  86},
    {643,  2,  86},
    {489,  2,  87},
    {170,  2,  87},
    {702,  2,  88},
    {802,  2,  88},
    {499,  2,  89},
    {766,  2,  89},
    {498,  2,  90},
    {749,  2,  90},
    {703,  2,  91},
    {712,  2,  91},
    {522,  2,  92},
    {542,  2,  92},
    {196,  2,  93},
    {197,  2,  93},
    {229,  2,  94},
    {228,  2,  94},
    {610,  2,  95},
    {609,  2,  95},
    {716,  3,   0},
    {955,  3,   0},
    {319,  3,   1},
    {573,  3,   2},
    {658,  3,   3},
    {664,  3,   3},
    {393,  3,   4},
    {407,  3,   5},
    {405,  3,   5},
    {531,  3,   6},
    {520,  3,   6},
    {149,  3,   7},
    {561,  3,   7},
    {624,  3,   8},
    {634,  3,   8},
    {625,  3,   9},
    {636,  3,   9},
    {626,  3,  10},
    {617,  3,  10},
    {685,  3,  11},
    {681,  3,  11},
    {395,  3,  12},
    {390,  3,  12},
    {716,  4,   0},
    {955,  4,   0},
    {319,  4,   1},
    {573,  4,   2},
    {312,  4,   3},
    {287,  4,   3},
    {393,  4,   4},
    {390,  4,   4},
    {762,  4,   5},
    {759,  4,   5},
    {833,  4,   6},
    {824,  4,   6},
    {544,  4,   7},
    {918,  4,   8},
    {861,  4,   9},
    {597,  4,  10},
    {599,  4,  11},
    {716,  5,   0},
    {955,  5,   0},
    {319,  5,   1},
    {573,  5,   2},
    {393,  5,   3},
    {390,  5,   3},
    {815,  5,   4},
    {810,  5,   4},
    {407,  5,   5},
    {405,  5,   5},
    {830,  5,   6},
    {589,  5,   7},
    {586,  5,   7},
    {915,  5,   8},
    {914,  5,   8},
    {704,  5,   9},
    {561,  5,   9},
    {685,  5,  10},
    {681,  5,  10},
    {180,  5,  11},
    {176,  5,  11},
    {839,  5,  12},
    {838,  5,  12},
    {283,  5,  13},
    {282,  5,  13},
    {807,  5,  14},
    {806,  5,  14},
    {665,  6,   0},
    {397,  6,   0},
    {396,  6,   0},
    {648,  6,   0},
    {960,  6,   1},
    {861,  6,   1},
    {956,  6,   1},
    {904,  6,   1},
    {675,  7,   0},
    {650,  7,   0},
    {544,  7,   1},
    {562,  7,   1},
    {918,  7,   2},
    {934,  7,   2},
    {861,  7,   3},
    {901,  7,   3},
    {597,  7,   4},
    {598,  7,   4},
    {599,  7,   5},
    {600,  7,   5},
    {565,  8,   0},
    {544,  8,   0},
    {778,  8,   1},
    {786,  8,   1},
    {373,  8,   2},
    {387,  8,   2},
    {380,  8,   3},
    {388,  8,   3},
    {381,  8,   4},
    {389,  8,   4},
    {156,  8,   5},
    {165,  8,   5},
    {320,  8,   6},
    {299,  8,   6},
    {187,  8,   7},
    {736,  8,   7},
    {795,  8,   8},
    {812,  8,   8},
    {946,  8,   9},
    {940,  8,   9},
    {707,  8,  10},
    {725,  8,  10},
    {219,  8,  11},
    {225,  8,  11},
    {679,  8,  12},
    {672,  8,  12},
    {145,  8,  13},
    {164,  8,  13},
    {594,  8,  14},
    {567,  8,  14},
    {791,  8,  15},
    {788,  8,  15},
    {593,  8,  16},
    {566,  8,  16},
    {790,  8,  17},
    {787,  8,  17},
    {743,  8,  18},
    {757,  8,  18},
    {676,  9,   0},
    {  1,  9,   0},
    {677,  9,   0},
    {  2,  9,   1},
    {731,  9,   1},
    {732,  9,   1},
    {117,  9,   2},
    {661,  9,   2},
    {682,  9,   2},
    {124,  9,   3},
    {526,  9,   3},
    {543,  9,   3},
    {130,  9,   4},
    {928,  9,   4},
    {935,  9,   4},
    { 13,  9,   5},
    { 22,  9,   6},
    { 25,  9,   7},
    { 28,  9,   8},
    { 32,  9,   9},
    { 33,  9,  10},
    { 35,  9,  11},
    { 36,  9,  12},
    { 38,  9,  13},
    { 39,  9,  14},
    { 43,  9,  15},
    { 48,  9,  16},
    { 52,  9,  17},
    { 58,  9,  18},
    { 63,  9,  19},
    { 65,  9,  20},
    { 66,  9,  21},
    { 67,  9,  22},
    { 68,  9,  23},
    { 69,  9,  24},
    { 76,  9,  25},
    { 80,  9,  26},
    { 81,  9,  27},
    { 82,  9,  28},
    { 83,  9,  29},
    { 84,  9,  30},
    { 85,  9,  31},
    {129,  9,  32},
    {136,  9,  33},
    { 20,  9,  34},
    { 21,  9,  35},
    { 24,  9,  36},
    { 26,  9,  37},
    { 27,  9,  38},
    { 30,  9,  39},
    { 31,  9,  40},
    { 47,  9,  41},
    {179,  9,  41},
    {183,  9,  41},
    { 49,  9,  42},
    {181,  9,  42},
    {177,  9,  42},
    { 50,  9,  43},
    {182,  9,  43},
    {178,  9,  43},
    { 51,  9,  44},
    {214,  9,  44},
    {202,  9,  44},
    { 53,  9,  45},
    {201,  9,  45},
    {187,  9,  45},
    { 54,  9,  46},
    {227,  9,  46},
    {203,  9,  46},
    { 55,  9,  47},
    {544,  9,  47},
    {563,  9,  47},
    { 56,  9,  48},
    {778,  9,  48},
    {784,  9,  48},
    { 57,  9,  49},
    {145,  9,  49},
    {139,  9,  49},
    { 59,  9,  50},
    {137,  9,  50},
    {138,  9,  50},
    { 60,  9,  51},
    {140,  9,  51},
    {161,  9,  51},
    { 61,  9,  52},
    {344,  9,  52},
    {363,  9,  52},
    { 62,  9,  53},
    {362,  9,  53},
    {339,  9,  53},
    { 64,  9,  54},
    {503,  9,  54},
    {505,  9,  54},
    {670, 10,   0},
    {248, 10,   1},
    {245, 10,   1},
    {300, 10,   2},
    {290, 10,   2},
    {406, 10,   3},
    {667, 10,   4},
    {654, 10,   4},
    {496, 10,   5},
    {495, 10,   5},
    {621, 10,   6},
    {620, 10,   6},
    {402, 10,   7},
    {401, 10,   7},
    {507, 10,   8},
    {506, 10,   8},
    {269, 10,   9},
    {374, 10,   9},
    {844, 10,  10},
    {843, 10,  10},
    {840, 10,  11},
    {926, 10,  12},
    {925, 10,  12},
    {941, 10,  13},
    {653, 10,  14},
    {652, 10,  14},
    {825, 10,  15},
    {827, 10,  15},
    {837, 10,  16},
    {836, 10,  16},
    {409, 10,  17},
    {408, 10,  17},
    {657, 11,   0},
    {648, 11,   0},
    {137, 11,   1},
    {155, 11,   1},
    {443, 11,   2},
    {447, 11,   2},
    {937, 11,   3},
    {941, 11,   3},
    {396, 11,   4},
    {410, 11,   4},
    {650, 11,   5},
    {653, 11,   5},
    {668, 12,   0},
    {144, 12,   1},
    {146, 12,   2},
    {148, 12,   3},
    {200, 12,   4},
    {206, 12,   5},
    {340, 12,   6},
    {341, 12,   7},
    {367, 12,   8},
    {400, 12,   9},
    {404, 12,  10},
    {411, 12,  11},
    {412, 12,  12},
    {446, 12,  13},
    {878, 12,  14},
    {449, 12,  14},
    {462, 12,  15},
    {465, 12,  16},
    {466, 12,  17},
    {467, 12,  18},
    {521, 12,  19},
    {530, 12,  20},
    {541, 12,  21},
    {546, 12,  22},
    {547, 12,  23},
    {622, 12,  24},
    {627, 12,  25},
    {674, 12,  26},
    {688, 12,  27},
    {744, 12,  28},
    {772, 12,  29},
    {773, 12,  30},
    {781, 12,  31},
    {783, 12,  32},
    {798, 12,  33},
    {799, 12,  34},
    {811, 12,  35},
    {813, 12,  36},
    {820, 12,  37},
    {853, 12,  38},
    {866, 12,  39},
    {876, 12,  40},
    {877, 12,  41},
    {883, 12,  42},
    {938, 12,  43},
    {860, 12,  44},
    {957, 12,  45},
    {958, 12,  46},
    {959, 12,  47},
    {966, 12,  48},
    {967, 12,  49},
    {970, 12,  50},
    {971, 12,  51},
    {534, 12,  52},
    {399, 12,  53},
    {241, 12,  54},
    {398, 12,  55},
    {690, 12,  56},
    {671, 13,   0},
    {905, 13,   0},
    {243, 13,   1},
    {514, 13,   1},
    {338, 13,   2},
    {366, 13,   2},
    {778, 13,   3},
    {785, 13,   3},
    {861, 13,   4},
    {902, 13,   4},
    {913, 14,   0},
    {955, 14,   0},
    {708, 14,   1},
    {709, 14,   1},
    {282, 14,   2},
    {285, 14,   2},
    {775, 14,   3},
    {776, 14,   3},
    {421, 14,   4},
    {424, 14,   4},
    {678, 14,   5},
    {673, 14,   5},
    {390, 14,   6},
    {391, 14,   6},
    {854, 14,   7},
    {236, 14,   7},
    {505, 14,   8},
    {493, 14,   8},
    {760, 14,   9},
    {761, 14,   9},
    {756, 14,  10},
    {758, 14,  10},
    {681, 14,  11},
    {685, 14,  11},
    {145, 14,  12},
    {152, 14,  12},
    {477, 14,  13},
    {481, 14,  13},
    {490, 14,  14},
    {501, 14,  14},
    {500, 14,  14},
    {475, 14,  15},
    {476, 14,  15},
    {286, 14,  16},
    {295, 14,  16},
    {198, 14,  17},
    {234, 14,  17},
    {189, 14,  18},
    {233, 14,  18},
    {830, 14,  19},
    {831, 14,  19},
    {213, 14,  20},
    {611, 14,  20},
    {319, 14,  21},
    {253, 14,  21},
    {573, 14,  22},
    {581, 14,  22},
    {256, 14,  23},
    {311, 14,  23},
    {797, 14,  24},
    {301, 14,  24},
    {143, 14,  25},
    {155, 14,  25},
    {188, 14,  26},
    {235, 14,  26},
    {817, 14,  27},
    {852, 14,  27},
    {976, 14,  28},
    {977, 14,  28},
    {664, 14,  29},
    {660, 14,  29},
    {942, 14,  30},
    {945, 14,  30},
    {512, 14,  31},
    {519, 14,  32},
    {518, 14,  33},
    {444, 14,  34},
    {445, 14,  35},
    {317, 14,  36},
    {284, 14,  36},
    {670, 15,   0},
    {685, 15,   1},
    {681, 15,   1},
    {359, 15,   2},
    {356, 15,   2},
    {346, 15,   3},
    {345, 15,   3},
    {651, 16,   0},
    {  0, 16,   1},
    {  1, 16,   2},
    {  4, 16,   3},
    {  3, 16,   4},
    { 12, 16,   5},
    { 11, 16,   6},
    { 10, 16,   7},
    {  9, 16,   8},
    { 71, 16,   9},
    {  8, 16,  10},
    {  7, 16,  11},
    {  6, 16,  12},
    { 75, 16,  13},
    { 42, 16,  14},
    {  5, 16,  15},
    { 74, 16,  16},
    {107, 16,  17},
    { 41, 16,  18},
    { 73, 16,  19},
    { 90, 16,  20},
    {106, 16,  21},
    {119, 16,  22},
    {  2, 16,  23},
    { 72, 16,  24},
    { 40, 16,  25},
    {105, 16,  26},
    { 70, 16,  27},
    {118, 16,  28},
    { 89, 16,  29},
    {131, 16,  30},
    {104, 16,  31},
    { 23, 16,  32},
    {112, 16,  33},
    { 29, 16,  34},
    {117, 16,  35},
    { 34, 16,  36},
    {124, 16,  37},
    { 37, 16,  38},
    {130, 16,  39},
    { 13, 16,  40},
    { 22, 16,  41},
    { 25, 16,  42},
    { 28, 16,  43},
    { 32, 16,  44},
    { 33, 16,  45},
    { 35, 16,  46},
    { 36, 16,  47},
    { 38, 16,  48},
    { 39, 16,  49},
    { 43, 16,  50},
    { 48, 16,  51},
    { 52, 16,  52},
    { 58, 16,  53},
    { 63, 16,  54},
    { 65, 16,  55},
    { 66, 16,  56},
    { 67, 16,  57},
    { 68, 16,  58},
    { 69, 16,  59},
    { 76, 16,  60},
    { 80, 16,  61},
    { 81, 16,  62},
    { 82, 16,  63},
    { 83, 16,  64},
    { 84, 16,  65},
    { 85, 16,  66},
    { 86, 16,  67},
    { 87, 16,  68},
    { 88, 16,  69},
    { 91, 16,  70},
    { 95, 16,  71},
    { 96, 16,  72},
    { 97, 16,  73},
    { 98, 16,  74},
    { 99, 16,  75},
    {100, 16,  76},
    {101, 16,  77},
    {102, 16,  78},
    {103, 16,  79},
    {108, 16,  80},
    {113, 16,  81},
    {120, 16,  82},
    {125, 16,  83},
    {132, 16,  84},
    { 14, 16,  85},
    { 44, 16,  86},
    { 77, 16,  87},
    { 92, 16,  88},
    {109, 16,  89},
    {114, 16,  90},
    {121, 16,  91},
    {126, 16,  92},
    {133, 16,  93},
    { 15, 16,  94},
    { 45, 16,  95},
    { 78, 16,  96},
    { 93, 16,  97},
    {110, 16,  98},
    {115, 16,  99},
    {122, 16, 100},
    {127, 16, 101},
    {134, 16, 102},
    { 16, 16, 103},
    { 46, 16, 104},
    { 79, 16, 105},
    { 94, 16, 106},
    {111, 16, 107},
    {116, 16, 108},
    {123, 16, 109},
    {128, 16, 110},
    {135, 16, 111},
    { 17, 16, 112},
    { 18, 16, 113},
    { 19, 16, 114},
    {650, 17,   0},
    {675, 17,   0},
    {784, 17,   1},
    {563, 17,   2},
    {930, 17,   3},
    {564, 17,   4},
    {895, 17,   5},
    {223, 17,   6},
    {896, 17,   7},
    {900, 17,   8},
    {898, 17,   9},
    {899, 17,  10},
    {224, 17,  11},
    {897, 17,  12},
    {733, 17,  13},
    {502, 17,  14},
    {716, 18,   0},
    {212, 18,   1},
    {929, 18,   2},
    {184, 18,   3},
    {682, 18,   4},
    {928, 18,   5},
    {933, 18,   6},
    {932, 18,   7},
    {931, 18,   8},
    {308, 18,   9},
    {303, 18,  10},
    {304, 18,  11},
    {309, 18,  12},
    {310, 18,  13},
    {307, 18,  14},
    {305, 18,  15},
    {306, 18,  16},
    {640, 18,  17},
    {893, 18,  18},
    {894, 18,  19},
    {780, 18,  20},
};

/* property values: 4204 bytes. */

/* Codepoints which expand on full case-folding. */

RE_UINT16 re_expand_on_folding[] = {
      223,   304,   329,   496,   912,   944,  1415,  7830,
     7831,  7832,  7833,  7834,  7838,  8016,  8018,  8020,
     8022,  8064,  8065,  8066,  8067,  8068,  8069,  8070,
     8071,  8072,  8073,  8074,  8075,  8076,  8077,  8078,
     8079,  8080,  8081,  8082,  8083,  8084,  8085,  8086,
     8087,  8088,  8089,  8090,  8091,  8092,  8093,  8094,
     8095,  8096,  8097,  8098,  8099,  8100,  8101,  8102,
     8103,  8104,  8105,  8106,  8107,  8108,  8109,  8110,
     8111,  8114,  8115,  8116,  8118,  8119,  8124,  8130,
     8131,  8132,  8134,  8135,  8140,  8146,  8147,  8150,
     8151,  8162,  8163,  8164,  8166,  8167,  8178,  8179,
     8180,  8182,  8183,  8188, 64256, 64257, 64258, 64259,
    64260, 64261, 64262, 64275, 64276, 64277, 64278, 64279,
};

/* expand_on_folding: 208 bytes. */

/* General_Category. */

static RE_UINT8 re_general_category_stage_1[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 14, 14, 14, 15,
    16, 17, 18, 19, 20, 19, 21, 19, 19, 19, 19, 19, 19, 22, 19, 19,
    19, 19, 19, 19, 19, 19, 23, 19, 19, 19, 24, 19, 19, 19, 25, 19,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 26,  7, 27, 28, 19, 19, 19, 19, 19, 19, 19, 29,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    30, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 31,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 31,
};

static RE_UINT8 re_general_category_stage_2[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
     32,  33,  34,  34,  35,  36,  37,  38,  39,  34,  34,  34,  40,  41,  42,  43,
     44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
     60,  61,  62,  63,  64,  64,  65,  66,  67,  68,  69,  70,  71,  69,  72,  73,
     69,  69,  64,  74,  64,  64,  75,  17,  76,  77,  78,  79,  80,  81,  69,  82,
     83,  84,  85,  86,  87,  88,  69,  69,  34,  34,  34,  34,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  89,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  90,
     91,  34,  34,  34,  34,  34,  34,  34,  34,  92,  34,  34,  93,  94,  95,  96,
     97,  98,  99, 100, 101, 102, 103, 104,  34,  34,  34,  34,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 105,
    106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106,
    107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
    107, 107,  34,  34, 108, 109, 110, 111,  34,  34, 112, 113, 114, 115, 116, 117,
    118, 119, 120, 121,  17, 122, 123, 124, 125, 126,  17,  17,  17,  17,  17,  17,
    127,  17, 128,  17, 129,  17, 130,  17, 131,  17,  17,  17, 132,  17,  17,  17,
    133, 134,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
     17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
     34,  34,  34,  34,  34,  34, 135,  17, 136,  17,  17,  17,  17,  17,  17,  17,
     34,  34,  34,  34,  34,  34,  34,  34, 137,  17,  17,  17,  17,  17,  17,  17,
     34,  34,  34,  34, 138,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
    139,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
     69, 140, 141, 142, 143,  17, 144,  17, 145, 146, 147, 148, 149, 150, 151, 152,
    153, 154, 155, 156, 157,  17, 158, 159, 160, 161, 162, 163, 164, 165, 166,  17,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 167,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 168,  34,
    169,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
     34,  34,  34,  34, 169,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
    170,  17, 171, 172,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
    107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 173,
};

static RE_UINT16 re_general_category_stage_3[] = {
      0,   0,   1,   2,   3,   4,   5,   6,   0,   0,   7,   8,   9,  10,  11,  12,
     13,  13,  13,  14,  15,  13,  13,  16,  17,  18,  19,  20,  21,  22,  13,  23,
     13,  13,  13,  24,  25,  11,  11,  11,  11,  26,  11,  27,  28,  29,  30,  31,
     32,  32,  32,  32,  32,  32,  32,  33,  34,  35,  36,  11,  37,  38,  13,  39,
      9,   9,   9,  11,  11,  11,  13,  13,  40,  13,  13,  13,  41,  13,  13,  13,
     13,  13,  42,  43,   9,  44,  45,  11,  46,  47,  32,  48,  49,  50,  51,  52,
     53,  54,  50,  50,  55,  32,  56,  57,  50,  50,  50,  50,  50,  58,  59,  60,
     61,  62,  50,  32,  63,  50,  50,  50,  50,  50,  64,  65,  66,  50,  67,  68,
     50,  69,  70,  71,  50,  72,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
     74,  50,  50,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,
     88,  81,  82,  89,  90,  91,  92,  93,  94,  95,  82,  96,  97,  98,  86,  99,
     80,  81,  82, 100, 101, 102,  86, 103, 104, 105, 106, 107, 108, 109,  92, 110,
    111, 112,  82, 113, 114, 115,  86, 116, 117, 112,  82, 118, 119, 120,  86, 121,
    117, 112,  50, 122, 123, 124,  86, 125, 126, 127,  50, 128, 129, 130,  73, 131,
    132,  50,  50, 133, 134, 135,  73,  73, 136, 137, 138, 139, 140, 141,  73,  73,
    142, 143, 144, 145, 146,  50, 147, 148, 149, 150,  32, 151, 152, 153,  73,  73,
     50,  50, 154, 155, 156, 157, 158, 159, 160, 161,   9,   9, 162,  50,  50, 163,
     50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 164, 165,  50,  50,
    164,  50,  50, 166, 167, 168,  50,  50,  50, 167,  50,  50,  50, 169, 170, 171,
     50, 172,  50,  50,  50,  50,  50, 173, 174,  50,  50,  50,  50,  50,  50,  50,
     50,  50,  50,  50,  50,  50, 175,  50, 176, 177,  50,  50,  50,  50, 178, 179,
    180, 181,  50, 182,  50, 183, 180, 184,  50,  50,  50, 185, 186, 187, 188, 189,
    190, 188,  50,  50, 191,  50,  50, 192,  50,  50, 193,  50,  50,  50,  50, 194,
     50, 147, 195, 196, 197,  50, 198, 173,  50,  50, 199, 200, 201, 202, 203, 203,
     50, 204,  50,  50,  50, 205, 206, 207, 188, 188, 208,  73,  73,  73,  73,  73,
    209,  50,  50, 210, 211, 156, 212, 213, 214,  50, 215, 188,  50,  50, 216, 217,
     50,  50, 218, 219, 220,  66,  50, 221,  73,  73,  73,  73,  73, 222, 223, 224,
     11,  11, 225,  27,  27,  27, 226, 227,  11, 228,  27,  27,  32,  32, 229, 230,
     13,  13,  13,  13,  13,  13,  13,  13,  13, 231,  13,  13,  13,  13,  13,  13,
    232, 233, 232, 232, 233, 234, 232, 235, 236, 236, 236, 237, 238, 239, 240, 241,
    242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,  73, 254, 255, 256,
    257, 258, 259, 260, 261, 262, 263, 263, 264, 265, 266, 203, 267, 268, 203, 269,
    270, 270, 270, 270, 270, 270, 270, 270, 271, 203, 272, 203, 203, 203, 203, 273,
    203, 274, 270, 275, 203, 276, 277, 278, 203, 203, 279,  73, 280,  73, 262, 262,
    262, 281, 203, 203, 203, 203, 282, 262, 203, 203, 203, 203, 203, 203, 203, 203,
    203, 203, 203, 283, 284, 203, 203, 285, 203, 203, 203, 203, 203, 203, 286, 203,
    287, 203, 203, 203, 203, 203, 288, 289, 262, 290, 203, 203, 291, 270, 292, 270,
    293, 294, 270, 270, 270, 295, 270, 296, 203, 203, 203, 270, 297, 172,  73,  73,
      9,   9, 298,  11,  11, 299, 300, 301,  13,  13,  13,  13,  13,  13, 302, 303,
     11,  11, 304,  50,  50,  50, 305, 306,  50, 307, 308, 308, 308, 308,  32,  32,
    309, 310, 311, 312,  73,  73,  73,  73, 203, 313, 203, 203, 203, 203, 203, 278,
    203, 203, 203, 203, 203, 314,  73, 315, 316, 317, 318, 319, 132,  50,  50,  50,
     50, 320, 174,  50,  50,  50,  50, 321, 322,  50, 198, 132,  50,  50,  50,  50,
    323, 324,  50,  51, 203, 203, 278,  50, 203, 325, 326, 203, 203, 327, 203, 203,
    326, 203, 203, 327, 203, 203, 203, 325,  50,  50,  50, 194, 203, 203, 203, 203,
     50,  50,  50,  50, 199,  73,  73,  73,  50, 328,  50,  50,  50,  50,  50,  50,
    147, 203, 203, 203, 279,  50,  50, 221, 329,  50, 330,  73,  13,  13, 331, 332,
     13,  42,  50,  50,  50,  50, 333, 334,  31, 335, 336, 337,  13,  13,  13, 338,
    339, 340, 341,  73,  73,  73,  73, 342, 343,  50, 344, 345,  50,  50,  50, 346,
    347,  50,  50, 348, 349, 188,  32, 350,  66,  50, 351,  50, 352, 353,  50, 147,
     74,  50,  50, 354, 355, 356,  73,  73,  50,  50, 357, 358, 359, 360,  50, 361,
     50,  50,  50, 362, 363, 364,  73,  73, 365, 366, 308,  73,  73,  73,  73,  73,
     73,  73,  73,  73,  50,  50, 367, 188,  50,  50, 368,  50, 369,  50,  50, 199,
    370, 370, 370, 370, 370, 370, 370, 370, 371, 371, 371, 371, 371, 371, 371, 371,
     50,  50, 198,  50,  50,  50, 198,  50,  50,  50,  50,  50,  50, 372,  73,  73,
    373, 374, 375, 376, 377,  50,  50,  50,  50,  50,  50, 378, 379, 380,  50,  50,
     50,  50,  50, 381,  73,  50,  50,  50,  50, 382,  50,  50, 192,  73,  73, 383,
     32, 384, 229, 385, 386, 387, 388, 389,  50,  50,  50,  50,  50,  50,  50, 390,
    391,   2,   3,   4,   5, 392, 393, 394,  50, 395,  50, 323, 396, 397, 398, 399,
    400,  50, 168, 401, 198, 198,  73,  73,  50,  50,  50,  50,  50,  50,  50,  51,
    402, 262, 262, 403, 263, 263, 263, 404, 405, 315,  73,  73,  73, 203, 203, 406,
     50, 147,  50,  50,  50,  98,  73,  73,  50, 323, 407,  50, 408,  73,  73,  73,
     50, 409,  50,  50, 410, 411,  73,  73,   9,   9, 412,  11,  11,  50,  50,  50,
     50, 198, 188,  73,  73,  73,  73,  73, 413,  50,  50, 414,  50, 415,  73,  73,
     50, 416,  50, 417,  73,  73,  73,  73, 418, 419,  50, 420, 421, 422,  50, 423,
     50,  50,  50, 424,  50, 425,  50, 426,  50,  50,  50,  50, 427,  73,  73,  73,
     73,  73,  73,  73,  73,  73, 262, 428, 429,  50,  50, 430, 431, 432, 433,  73,
    214,  50,  50, 434, 312,  73,  73,  73,  50,  50,  50,  50,  50,  50, 323,  73,
    263, 263, 263, 263, 263, 263, 435, 436,  50,  50, 323,  73,  73,  73,  73,  73,
     50,  50,  50, 427,  73,  73,  73,  73, 437,  73,  73,  73,  73,  73,  73,  73,
    203, 203, 203, 203, 203, 203, 203, 314, 203, 203, 438, 203, 203, 203, 439, 440,
    441, 203, 442, 203, 203, 443,  73,  73, 203, 203, 203, 203, 444,  73,  73,  73,
    203, 203, 203, 203, 203, 279, 262, 445,   9, 446,  11, 447, 448, 449, 232,   9,
    450, 451, 452, 453, 454,   9, 446,  11, 455, 456,  11, 457, 458, 459, 460,   9,
    461,  11,   9, 446,  11, 447, 448,  11, 232,   9, 450, 460,   9, 461,  11,   9,
    446,  11, 462,   9, 463, 464, 465, 466,  11, 467,   9, 468, 469, 470, 471,  11,
    472,   9, 473,  11, 474, 475, 475, 475, 203, 203, 315, 203, 203, 203, 203, 203,
    203, 278, 325, 476, 287, 287,  73,  73, 477, 203, 325, 203, 203, 203, 172, 203,
    203, 280,  73,  73,  73,  73, 478, 203, 479, 203, 203, 280, 480, 481,  73,  73,
    203, 203, 482, 483, 203, 203, 203, 484, 203, 278, 203, 203, 485,  73, 203, 482,
    203, 203, 203, 325, 486, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 487,
    203, 203, 203, 443,  73, 203, 488,  73,  73,  73,  73,  73,  73,  73,  73, 489,
    287, 490, 491, 492, 493,  73,  73,  73, 203, 203, 203, 203, 314,  73,  73,  73,
    203, 203, 203, 203, 203, 203, 203, 278,  50,  50,  50,  50,  50, 307,  73,  73,
     50,  50,  50, 173,  50,  50,  50,  50,  50, 198,  73,  73,  73,  73,  73,  73,
    494,  73, 495, 495, 495, 495, 495, 495,  32,  32,  32,  32,  32,  32,  32,  32,
     32,  32,  32,  32,  32,  32,  32,  73, 371, 371, 371, 371, 371, 371, 371, 496,
};

static RE_UINT8 re_general_category_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   3,   2,   4,   5,   6,   2,
      7,   7,   7,   7,   7,   2,   8,   9,  10,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  17,  18,  19,   1,  20,  20,  21,  22,  23,  24,  25,
     26,  27,  15,  28,  29,  30,  27,  31,  11,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  32,  11,  11,  11,  33,  16,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  34,  16,  16,  16,  16,  33,  33,  33,  33,  33,  33,  33,  33,
     33,  33,  33,  33,  35,  35,  35,  35,  35,  35,  35,  35,  16,  33,  33,  33,
     33,  33,  33,  33,  11,  35,  35,  16,  35,  33,  33,  11,  35,  11,  16,  11,
     11,  35,  33,  11,  33,  16,  11,  35,  33,  33,  33,  11,  35,  16,  33,  11,
     35,  11,  35,  35,  33,  36,  33,  16,  37,  37,  38,  35,  39,  38,  35,  35,
     35,  35,  35,  35,  35,  35,  16,  33,  35,  39,  33,  11,  33,  33,  33,  33,
     33,  33,  16,  16,  16,  11,  35,  33,  35,  35,  11,  33,  33,  33,  33,  33,
     16,  16,  40,  16,  16,  16,  16,  16,  41,  41,  41,  41,  41,  41,  41,  41,
     41,  42,  42,  41,  41,  41,  41,  41,  41,  42,  42,  42,  42,  42,  42,  42,
     41,  41,  43,  42,  42,  42,  43,  43,  42,  42,  42,  42,  42,  42,  42,  42,
     44,  44,  44,  44,  44,  44,  44,  44,  33,  33,  43,  33,  45,  46,  16,  47,
     45,  45,  42,  48,  11,  49,  49,  11,  35,  11,  11,  11,  11,  11,  11,  11,
     11,  50,  11,  11,  11,  11,  16,  16,  16,  16,  16,  16,  16,  16,  16,  35,
     16,  11,  33,  16,  33,  33,  33,  33,  16,  16,  33,  51,  35,  33,  35,  11,
     33,  52,  44,  44,  53,  33,  33,  33,  11,  35,  35,  35,  35,  35,  35,  16,
     33,  33,  33,  33,  45,  45,  45,  45,  50,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  49,  54,   2,   2,   2,  55,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  56,  57,  45,  45,  58,  44,  44,  44,  44,  44,  44,  44,
     44,  44,  44,  44,  44,  44,  44,  59,  60,  61,  44,  60,  45,  45,  45,  45,
     37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  62,  45,  45,
     37,  63,  47,  45,  45,  45,  45,  45,  64,  64,  45,   8,   9,  65,   2,  21,
     44,  44,  44,  44,  44,  61,  45,   2,  66,  37,  37,  37,  37,  67,  44,  44,
      7,   7,   7,   7,   7,   2,   2,  37,  68,  37,  37,  37,  37,  37,  37,  37,
     37,  37,  69,  44,  44,  44,  70,  52,  44,  44,  71,  72,  73,  44,  44,  37,
      7,   7,   7,   7,   7,  37,  74,  75,   2,   2,   2,   2,   2,   2,   2,  76,
     67,  37,  37,  37,  37,  37,  37,  37,  44,  44,  44,  44,  44,  77,  78,  37,
     37,  37,  37,  44,  44,  44,  44,  44,  68,  45,  45,  45,  45,  45,  45,  45,
      7,   7,   7,   7,   7,  37,  37,  37,  37,  37,  37,  37,  37,  67,  44,  44,
     44,  44,  41,  28,   2,  79,  45,  45,  37,  37,  37,  44,  44,  72,  44,  44,
     44,  44,  72,  44,  72,  44,  44,  45,   2,   2,   2,   2,   2,   2,   2,  47,
     37,  37,  37,  37,  67,  44,  45,  47,  45,  45,  45,  45,  45,  45,  45,  45,
     44,  80,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  80,  68,  81,
     82,  44,  44,  44,  80,  81,  82,  81,  67,  44,  44,  44,  37,  37,  37,  37,
     37,  44,   2,   7,   7,   7,   7,   7,  83,  37,  37,  37,  78,  37,  37,  37,
     58,  81,  78,  37,  37,  37,  62,  78,  62,  78,  37,  37,  37,  37,  37,  37,
     37,  37,  37,  37,  62,  37,  37,  37,  62,  62,  45,  37,  37,  45,  68,  81,
     82,  44,  77,  84,  85,  84,  82,  62,  45,  45,  45,  84,  45,  45,  37,  78,
     37,  44,  45,   7,   7,   7,   7,   7,  37,  20,  27,  27,  27,  86,  45,  45,
     58,  80,  78,  37,  37,  62,  45,  78,  62,  37,  78,  62,  37,  45,  77,  81,
     82,  77,  45,  58,  77,  58,  44,  45,  58,  45,  45,  45,  78,  37,  62,  62,
     45,  45,  45,   7,   7,   7,   7,   7,  44,  37,  67,  45,  45,  45,  45,  45,
     58,  80,  78,  37,  37,  37,  37,  78,  37,  78,  37,  37,  37,  37,  37,  37,
     62,  37,  78,  37,  37,  45,  68,  81,  82,  44,  44,  58,  80,  84,  82,  45,
     62,  45,  45,  45,  45,  45,  45,  45,  87,  45,  45,  45,  45,  45,  45,  45,
     62,  37,  78,  37,  37,  45,  68,  82,  82,  44,  77,  84,  85,  84,  82,  45,
     45,  45,  45,  80,  45,  45,  37,  78,  75,  27,  27,  27,  45,  45,  45,  45,
     45,  68,  78,  37,  37,  62,  45,  37,  62,  37,  37,  45,  78,  62,  62,  37,
     45,  78,  62,  45,  37,  62,  45,  37,  37,  37,  37,  37,  37,  45,  45,  81,
     80,  85,  45,  81,  85,  81,  82,  45,  62,  45,  45,  84,  45,  45,  45,  45,
     27,  88,  21,  21,  86,  89,  45,  45,  84,  81,  78,  37,  37,  37,  62,  37,
     62,  37,  37,  37,  37,  37,  37,  37,  37,  37,  78,  37,  37,  45,  78,  44,
     80,  81,  85,  44,  77,  44,  44,  45,  45,  45,  58,  77,  37,  45,  45,  45,
     45,  45,  45,  45,  27,  27,  27,  88,  45,  81,  78,  37,  37,  37,  62,  37,
     37,  37,  78,  37,  37,  45,  68,  82,  81,  81,  85,  80,  85,  81,  44,  45,
     45,  45,  84,  85,  45,  45,  45,  62,  78,  62,  45,  45,  45,  45,  45,  45,
     37,  37,  37,  37,  37,  62,  78,  81,  82,  44,  77,  81,  85,  81,  82,  62,
     45,  45,  45,  84,  45,  45,  45,  45,  27,  27,  27,  45,  90,  37,  37,  37,
     45,  81,  78,  37,  37,  37,  37,  37,  37,  37,  37,  62,  45,  37,  37,  37,
     37,  78,  37,  37,  37,  37,  78,  45,  37,  37,  37,  62,  45,  77,  45,  84,
     81,  44,  77,  77,  81,  81,  81,  81,  45,  81,  47,  45,  45,  45,  45,  45,
     78,  37,  37,  37,  37,  37,  37,  37,  67,  37,  44,  44,  44,  77,  45,  87,
     37,  37,  37,  72,  44,  44,  44,  61,   7,   7,   7,   7,   7,   2,  45,  45,
     78,  62,  62,  78,  62,  62,  78,  45,  45,  45,  37,  37,  78,  37,  37,  37,
     78,  37,  78,  78,  45,  37,  78,  37,  67,  37,  44,  44,  44,  58,  68,  45,
     37,  37,  62,  79,  44,  44,  44,  45,   7,   7,   7,   7,   7,  45,  37,  45,
     74,  21,   2,   2,   2,   2,   2,   2,   2,  91,  21,  21,  44,  21,  21,  21,
      7,   7,   7,   7,   7,  27,  27,  27,  27,  27,  52,  52,  52,   4,   4,  81,
     37,  37,  37,  37,  78,  37,  37,  37,  37,  37,  37,  37,  37,  37,  62,  45,
     58,  44,  44,  44,  44,  44,  44,  80,  44,  44,  61,  44,  37,  37,  67,  44,
     44,  44,  44,  44,  58,  44,  44,  44,  44,  44,  44,  44,  44,  44,  77,  21,
     21,  21,  21,  73,  21,  21,  89,  21,   2,   2,  91,  21,  28,  47,  45,  45,
     37,  37,  37,  37,  37,  92,  82,  44,  80,  44,  44,  44,  82,  80,  82,  68,
      7,   7,   7,   7,   7,   2,   2,   2,  37,  37,  37,  81,  44,  37,  37,  44,
     68,  81,  93,  92,  81,  81,  81,  37,  67,  44,  68,  37,  37,  37,  37,  37,
     37,  80,  82,  80,  81,  81,  82,  92,   7,   7,   7,   7,   7,  81,  82,  21,
     11,  11,  11,  45,  45,  45,  45,  45,  37,  37,  37,  37,  37,  63,  79,  45,
     37,  37,  37,  37,  62,  37,  37,  45,  37,  37,  37,  62,  62,  37,  37,  45,
     62,  37,  37,  45,  37,  37,  37,  62,  62,  37,  37,  45,  37,  37,  37,  37,
     37,  37,  37,  62,  37,  37,  37,  37,  37,  37,  37,  37,  37,  62,  58,  44,
     28,   2,   2,   2,  94,  27,  27,  27,  27,  27,  27,  27,  27,  27,  95,  45,
     21,  21,  21,  21,  21,  45,  45,  45,  37,  37,  62,  45,  45,  45,  45,  45,
     96,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  63,  69,
     97,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  98,  99,  45,
     37,  37,  37,  37,  37,  63,   2, 100, 101,  45,  45,  45,  45,  45,  45,  45,
     37,  37,  37,  37,  37,  37,  62,  37,  37,  44,  77,  45,  45,  45,  45,  45,
     37,  44,  61,  47,  45,  45,  45,  45,  37,  44,  45,  45,  45,  45,  45,  45,
     62,  44,  45,  45,  45,  45,  45,  45,  37,  37,  64,  82,  44,  44,  44,  81,
     81,  81,  81,  80,  82,  44,  44,  44,  44,  44,   2,  83,   2,  65,  67,  45,
      7,   7,   7,   7,   7,  45,  45,  45,  27,  27,  27,  27,  27,  45,  45,  45,
      2,   2,   2, 102,   2,  60,  44, 103,  37, 104,  37,  37,  37,  37,  37,  37,
     37,  37,  37,  37,  45,  45,  45,  45,  37,  37,  37,  37,  67,  62,  45,  45,
     37,  37,  37,  45,  45,  45,  45,  45,  44,  80,  81,  82,  80,  81,  45,  45,
     81,  80,  81,  81,  82,  44,  45,  45,  89,  45,   2,   7,   7,   7,   7,   7,
     37,  37,  37,  37,  37,  37,  37,  45,  37,  37,  37,  37,  37,  37,  45,  45,
     81,  81,  81,  81,  81,  81,  81,  81,  93,  37,  37,  37,  81,  45,  45,  45,
      7,   7,   7,   7,   7,  95,  45,  21,  21,  21,  21,  21,  21,  21,  21,  21,
     37,  37,  37,  67,  80,  81,  45,   2,  37,  37,  92,  80,  44,  44,  44,  77,
     80,  80,  82,  44,  44,  44,  80,  81,  81,  82,  44,  44,  44,  44,  77,  58,
      2,   2,   2,  83,   2,   2,   2,  45,  44,  44,  93,  37,  37,  37,  37,  37,
     37,  37,  80,  44,  44,  80,  80,  81,  81,  80,  93,  37,  37,  37,  45,  45,
     91,  21,  21,  21,  21,  52,  44,  44,  44,  44,  21,  21,  21,  21,  89,  45,
     44,  93,  37,  37,  37,  37,  37,  37,  92,  44,  44,  81,  44,  85,  45,  37,
     37,  37,  37,  80,  44,  81,  82,  82,  44,  81,  45,  45,  45,  45,   2,   2,
     37,  37,  81,  81,  81,  81,  44,  44,  44,  44,  81,  44,  45,  56,   2,   2,
      7,   7,   7,   7,   7,  45,  78,  37,  37,  37,  37,  37,  41,  41,  41,   2,
     44,  61,  44,  44,  44,  44,  44,  44,  80,  44,  44,  44,  68,  37,  67,  37,
     37,  85,  45,  45,  45,  45,  45,  45,  16,  16,  16,  16,  16,  16,  41,  41,
     41,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  46,  16,  16,  16,
     16,  16,  16,  16,  16, 105,  41,  41,  44,  44,  44,  77,  45,  45,  45,  45,
     45,  45,  45,  45,  45,  45,  44,  44,  33,  33,  33,  16,  16,  16,  16,  33,
     16,  16,  16,  16,  11,  11,  11,  11,  16,  16,  16,  45,  11,  11,  11,  45,
     16,  16,  16,  16,  50,  50,  50,  50,  16,  16,  16,  16,  16,  16,  16,  45,
     16,  16,  16,  16, 106, 106, 106, 106,  16,  16, 107,  16,  11,  11, 108, 109,
     42,  16, 107,  16,  11,  11, 108,  42,  16,  16,  45,  16,  11,  11, 110,  42,
     16,  16,  16,  16,  11,  11, 111,  42,  45,  16, 107,  16,  11,  11, 108, 112,
    113, 113, 113, 113, 113, 114,  64,  64, 115, 115, 115,   2, 116, 117, 116, 117,
      2,   2,   2,   2, 118,  64,  64, 119,   2,   2,   2,   2, 120, 121,   2, 122,
    123,   2, 124, 125,   2,   2,   2,   2,   2,   9, 123,   2,   2,   2,   2, 126,
     64,  64, 127,  45,  45,  64,  64,  64, 128,  45,  27,  27,  27,   8, 124, 129,
     27,  27,  27,  27,  27,   8, 124,  99,  41,  41,  41,  41,  41,  41,  79,  45,
     20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  45,  45,  45,
     44,  44,  44,  44,  44,  44, 130,  53, 131,  53, 131,  44,  44,  44,  44,  44,
     77,  45,  45,  45,  45,  45,  45,  45,  21, 132,  21, 133,  21,  35,  11,  16,
     11,  33, 133,  21,  51,  11,  11,  21,  21,  21, 132, 132, 132,  11,  11, 134,
     11,  11,  36,  37,  40,  21,  16,  11,   8,   8,  51,  16,  16,  26,  21, 135,
     27,  27,  27,  27,  27,  27,  27,  27, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 136, 137, 100, 138,  45,  45,  45,   8,   8, 139,  21,  21,   8,  21,  21,
    139,  26,  21, 139,  21,  21,  21, 139,  21,  21,  21,  21,  21,  21,  21,   8,
     21, 139, 139,  21,  21,  21,  21,  21,  21,  21,   8,   8,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8,   8,   8,  21,  21,  21,  21,   8,   8,  21,  21,
      8,  21,  21,  21, 140, 141,  21,  21,  21,  21,  21,  21,  21,  21, 139,  21,
     21,  21,  21,  21,  21,  26,   8,   8,   8,   8,  21,  21,  21,  21,  21,  21,
     21,  21,  21,  21,  21,  21,   8,   8,   8,  21,  21,  21,  21,  21,  21,  21,
     21,  21,  45,  45,  45,  45,  45,  45,  21,  21,  21,  89,  45,  45,  45,  45,
     21,  21,  21,  21,  21,  89,  45,  45,  27,  27,  27,  27,  27,  27,  21,  21,
     21,  21,  21,  21,  21,  27,  27,  27,  21,  21,  21,  26,  21,  21,  21,  21,
     26,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,   8,   8,   8,   8,
     21,  21,  21,  21,  21,  21,  21,  26,  90,  21,  21,  21,  21,  21,  21,  21,
     21,  21,  21,  21,   4,   4,   4,   4,   4,   4,   4,  27,  27,  27,  27,  27,
     27,  27,  21,  21,  21,  21,  21,  21,   8,   8, 124, 142,   8, 143, 143,   8,
      8,   8,   8,   4,   4,   4,   4,   4,   8, 124, 144, 144, 144, 144, 144, 144,
    144, 144, 144, 144, 142,   8,   8,   8,   8,   8,   8,   8,   4,   4,   8,   8,
      8,   8,   8,   8,   8,   8,   4,   8,   8,   8, 139,  26,   8,   8, 143,  45,
     11,  11,  11,  11,  11,  11,  11,  49,  16,  16,  16,  16,  16,  16,  16, 107,
     33,  11,  33,  35,  35,  35,  35,  11,  33,  33,  35,  16,  16,  16, 105,  11,
     33,  33, 135,  21,  21, 133,  35, 145,  44,  45,  45,  45,  56,   2,  94,   2,
     16,  16,  16,  45,  45,  45,  45,  45,  37,  37,  37,  45,  45,  45,  45,  54,
     47,  45,  45,  45,  45,  45,  45,  58,  37,  37,  37,  62,  45,  45,  45,  45,
     37,  37,  37,  62,  37,  37,  37,  62,   2, 116, 116,   2, 120, 121, 116,   2,
      2,   2,   2,   6,   2, 102, 116,   2, 116,   4,   4,   4,   4,   2,   2,  83,
      2,  45,  45,  45,  45,  45,  45,  45,  21,  21,  21,  21,  21,  90,  21,  21,
     21,  21,  21,  45,  45,  45,  45,  45,  21,  21,  21,  21,  21,  21,  45,  45,
      1,   2, 146, 147,   4,   4,   4,   4,   4,  21,   4,   4,   4,   4, 148, 149,
    150, 100, 100, 100, 100,  44,  44,  44, 151,  41,  41,  21, 100, 152,  63,  21,
     37,  37,  37,  62,  58, 153, 154,  66,  37,  37,  37,  37,  37,  63,  41,  66,
     45,  45,  78,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  62,
     21,  27,  27,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  89,
     27,  27,  27,  27,  27,  21,  21,  21, 155,  27,  27,  27,  27,  27,  27,  27,
     37,  37, 104,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37, 156,   2,
      7,   7,   7,   7,   7,  37,  45,  45,  33,  33,  33,  33,  33,  33,  33,  67,
     53, 157,  45,  45,  45,  45,  44,  83,  37,  37,  37, 100, 100, 100, 100, 100,
     44,   2,   2,   2,  45,  45,  45,  45,  42,  42,  42, 154,  41,  41,  41,  41,
     42,  33,  33,  33,  33,  33,  33,  33,  16,  33,  33,  33,  33,  33,  33,  33,
     46,  16,  16,  16,  35,  35,  35,  33,  33,  33,  33,  33,  43, 158,  35, 107,
     33,  45,  45,  45,  45,  45,  45,  45,  33,  33,  33,  33,  33,  45,  45,  45,
     45,  45,  45,  45,  45,  36,  37,  37,  37,  68,  37,  68,  37,  67,  37,  37,
     37,  92,  82,  80,  21,  21,  45,  45,  27,  27,  27,  21, 159,  45,  45,  45,
     37,  37,   2,   2,  45,  45,  45,  45,  81,  37,  37,  37,  37,  37,  37,  37,
     37,  37,  81,  81,  81,  81,  81,  81,  81,  81,  77,  45,  45,  45,  45,   2,
     44,  37,  37,  37,   2,  69,  45,  45,  37,  37,  37,  44,  44,  44,  44,   2,
     37,  37,  37,  67,  44,  44,  44,  44,  44,  81,  45,  45,  45,  45,  45,  56,
     37,  67,  81,  44,  44,  81,  80,  81, 160,   2,   2,   2,   2,   2,   2,  54,
      7,   7,   7,   7,   7,  45,  45,   2,  37,  37,  37,  37,  67,  44,  44,  80,
     82,  80,  82,  77,  45,  45,  45,  45,  37,  67,  37,  37,  37,  37,  80,  45,
      7,   7,   7,   7,   7,  45,   2,   2,  66,  37,  37,  74,  21,  92,  45,  45,
     68,  44,  68,  67,  68,  37,  37,  44,  67,  62,  45,  45,  45,  45,  45,  45,
     45,  45,  45,  45,  45,  78, 104,   2,  78,  37,  37,  62,  78,  37,  37,  62,
     78,  37,  37,  62,  45,  45,  45,  45,  37,  92,  82,  81,  80, 160,  82,  45,
     37,  37,  45,  45,  45,  45,  45,  45,  37,  37,  37,  62,  45,  78,  37,  37,
    161, 161, 161, 161, 161, 161, 161, 161, 162, 162, 162, 162, 162, 162, 162, 162,
     37,  37,  37,  37,  37,  45,  45,  45,  16,  16,  16, 107,  45,  45,  45,  45,
     45,  55,  16,  16,  45,  45,  78,  68,  37,  37,  37,  37, 163,  37,  37,  37,
     37,  37,  37,  62,  37,  37,  62,  62,  37,  78,  62,  37,  37,  37,  37,  37,
     37,  42,  42,  42,  42,  42,  42,  42,  42,  45,  45,  45,  45,  45,  45,  45,
     45,  78,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,   4,
     45,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37, 159,  45,
      2,   2,   2, 164, 125,  45,  45,  45,   6, 165, 166, 144, 144, 144, 144, 144,
    144, 144, 125, 164, 125,   2, 122, 167,   2,  47,   2,   2, 148, 144, 144, 125,
      2, 168,   8, 143,  65,   2,  45,  45,  37,  37,  62,  37,  37,  37,  37,  37,
     37,  37,  37,  37,  37,  37,  62,  76,  56,   2,   3,   2,   4,   5,   6,   2,
     16,  16,  16,  16,  16,  17,  18, 124, 125,   4,   2,  37,  37,  37,  37,  37,
     66,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  41,
     45,  37,  37,  37,  45,  37,  37,  37,  45,  37,  37,  37,  45,  37,  62,  45,
     20, 169,  86, 170,  26,   8, 139,  89,  45,  45,  45,  45,  76,  64,  21,  45,
     37,  37,  37,  37,  37,  37,  78,  37,  37,  37,  37,  37,  37,  62,  37,  78,
      2,  89,  45, 171,  27,  27,  27,  27,  27,  27,  45,  90,  21,  21,  21,  21,
    100, 100, 138,  27,  88,  21,  21,  21,  21,  21,  21,  21,  21,  95,  45,  45,
     21,  21,  21,  21,  21,  21,  52,  45,  27,  27,  45,  45,  45,  45,  45,  45,
    147,  37,  37,  37,  37, 101,  45,  45,  37,  37,  37,  37,  37,  37,  37,  56,
     37,  37,  45,  45,  37,  37,  37,  37, 172, 100, 100,  45,  45,  45,  45,  45,
     11,  11,  11,  11,  16,  16,  16,  16,  37,  37,  37,  45,  62,  37,  37,  37,
     37,  37,  37,  78,  62,  45,  62,  78,  37,  37,  37,  56,  27,  27,  27,  27,
     37,  37,  37,  27,  27,  27,  45,  56,  37,  37,  37,  37,  37,  45,  45,  56,
     67,  44,  58,  77,  45,  45,  44,  44,  37,  37,  78,  37,  78,  37,  37,  37,
     37,  37,  45,  45,  44,  77,  45,  58,  27,  27,  27,  27,  45,  45,  45,  45,
      2,   2,   2,   2,  47,  45,  45,  45,  37,  37,  37,  37,  37,  37, 173,  31,
     37,  37,  37,  45,  56,   2,   2,   2,  37,  37,  37,  45,  27,  27,  27,  27,
     37,  62,  45,  45,  27,  27,  27,  27,  37,  37,  37,  37,  62,  45,  45,  45,
     27,  27,  27,  27,  27,  27,  27,  95,  82,  93,  37,  37,  37,  37,  37,  37,
     37,  37,  37,  37,  44,  44,  44,  44,  44,  44,  44,  61,   2,   2,   2,  45,
     45,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,   7,   7,   7,   7,   7,
     81,  82,  44,  80,  82,  61, 174,   2, 100, 101,  45,  45,  45,  45,  45,  45,
      2,   2,  45,  45,  45,  45,  45,  45,  37,  45,  45,  45,  45,  45,  45,  45,
     21,  21,  21,  89,  90,  21,  21,  21,  21,  21, 175,  82,  44,  21, 175,  81,
     81, 176,  64,  64,  64, 177,  44,  44,  44,  73,  52,  44,  44,  44,  21,  21,
     21,  21,  21,  21,  21,  44,  44,  21,  21,  21,  21,  21,  21,  21,  21,  45,
     21,  44,  73,  45,  45,  45,  45,  45,  27,  45,  45,  45,  45,  45,  45,  45,
     11,  11,  11,  11,  11,  16,  16,  16,  16,  16,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  11,  11,  11,  16,  16,  16, 107,  16,  16,  16,  16,  16,
     11,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  49,  11,
     45,  49,  50,  49,  50,  11,  49,  11,  11,  11,  11,  16,  16,  55,  55,  16,
     16,  16,  55,  16,  16,  16,  16,  16,  16,  16,  11,  50,  11,  49,  50,  11,
     11,  11,  49,  11,  11,  11,  49,  16,  16,  16,  16,  16,  11,  50,  11,  49,
     11,  11,  49,  49,  45,  11,  11,  11,  49,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  16,  11,  11,  11,  11,  11,  16,  16,  16,  16,  16,
     16,  16,  16,  45,  11,  11,  11,  11,  32,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  34,  16,  16,  16,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  11,  32,  16,  16,  16,  16,  34,  16,  16,  16,  11,  11,
     11,  11,  32,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  34,
     16,  16,  16,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  32,
     16,  16,  16,  16,  34,  16,  16,  16,  11,  11,  11,  11,  32,  16,  16,  16,
     16,  34,  16,  16,  16,  33,  45,   7,   7,   7,   7,   7,   7,   7,   7,   7,
     90,  21,  21,  21,  21,  21,  21,  89,  27,  27,  27,  27,  27,  95,  45,  45,
     45,  45,  45,  21,  21,  21,  21,  21,  21,  89,  45,  45,  45,  45,  45,  45,
     21,  21,  21,  21,  89,  45,  45,  45,  21,  45,  45,  45,  45,  45,  45,  45,
     89,  45,  45,  45,  45,  45,  45,  45,  21,  21,  21,  90,  21,  21,  21,  21,
     21,  21,  21,  21,  21,  21,  89,  45,  21,  21,  89,  21,  21,  89,  45,  45,
     89,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  90,  21,  89,  45,
     21,  21,  21,  21,  45,  45,  45,  45,  45,  45,  45,  45,  45,  90,  21,  21,
     89,  21,  89,  89,  89,  89,  21,  89,  21,  21,  21,  45,  21,  21,  90,  45,
     21,  21,  90,  21,  21,  21,  21,  21,  89,  45,  90,  21,  21,  21,  21,  21,
     76,  45,  45,  45,  45,  45,  45,  45,  64,  64,  64,  64,  64,  64,  64,  64,
    162, 162, 162, 162, 162, 162, 162,  45,
};

static RE_UINT8 re_general_category_stage_5[] = {
    15, 15, 12, 23, 23, 23, 25, 23, 20, 21, 23, 24, 23, 19,  9,  9,
    24, 24, 24, 23, 23,  1,  1,  1,  1, 20, 23, 21, 26, 22, 26,  2,
     2,  2,  2, 20, 24, 21, 24, 15, 25, 25, 27, 27, 26, 27,  2, 28,
    24, 16, 27, 26, 27, 24, 11, 11, 27, 23, 26, 11,  2, 29, 11, 23,
     1, 24,  1,  2,  2, 24,  2,  1,  2,  5,  5,  5,  1,  3,  3,  2,
     5,  2,  4,  4, 26, 26,  4, 26,  6,  6,  0,  0,  4,  2, 23,  0,
     1, 23,  1,  0,  0,  1, 24,  1, 27,  6,  7,  7,  0,  4,  0,  2,
     0, 23, 19,  0,  0,  6, 19,  6, 23,  6,  6, 23,  5,  0,  5, 23,
    16, 16, 23, 25,  4,  5,  5,  6,  6,  5, 23,  5,  6, 16,  6,  4,
     4,  6,  6, 27,  5, 27, 27,  5,  0, 16,  6,  0,  0,  5,  4,  0,
     6,  8,  8,  8,  8,  6, 23,  4,  0,  8,  8,  0, 27, 25,  0, 25,
    11, 27, 27,  0,  0, 27, 23, 27,  5,  8,  8,  5, 23, 11, 11,  0,
    19,  5, 12,  5,  5, 20, 21,  0, 10, 10, 10,  0, 19, 23, 12,  0,
     5,  4,  2,  4,  3,  3,  2,  0,  3, 26,  2, 26,  0, 26,  1, 26,
    26,  0, 12, 12, 12, 16, 19, 19, 28, 29, 20, 28, 13, 14, 16, 12,
    23, 28, 29, 23, 23, 22, 22, 23, 24, 20, 21, 23, 23, 12, 16,  0,
    11,  4, 21,  4,  6,  7,  7,  6,  1, 27, 27,  1, 27,  2,  2, 27,
    10,  1,  2, 10, 10, 11, 24, 27, 27, 20, 21, 27, 21, 24, 24,  0,
    21, 20,  2,  6, 27,  4,  5, 10, 19, 20, 21, 21, 27, 10, 19,  4,
    10,  4,  6, 26, 26,  4, 27, 11,  4, 23,  7, 23, 26,  1, 25, 27,
     8, 23, 18, 18, 17, 17,  5, 24, 23, 20, 19, 22, 22, 20, 22, 22,
    24, 19, 24, 26, 25,  0,  0, 11, 23, 10,  5, 11, 23, 16, 27,  8,
     8, 16, 16,  6,
};

/* General_Category: 8172 bytes. */

RE_UINT32 re_get_general_category(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 11;
    code = ch ^ (f << 11);
    pos = (RE_UINT32)re_general_category_stage_1[f] << 4;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_general_category_stage_2[pos + f] << 3;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_general_category_stage_3[pos + f] << 3;
    f = code >> 1;
    code ^= f << 1;
    pos = (RE_UINT32)re_general_category_stage_4[pos + f] << 1;
    value = re_general_category_stage_5[pos + code];

    return value;
}

/* Block. */

static RE_UINT8 re_block_stage_1[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9, 10, 11, 12, 12, 12, 12, 13, 14, 15, 15, 15, 16,
    17, 18, 19, 20, 21, 20, 22, 20, 20, 20, 20, 20, 20, 23, 20, 20,
    20, 20, 20, 20, 20, 20, 24, 20, 20, 20, 25, 20, 20, 20, 26, 20,
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
    27, 27, 27, 27, 28, 29, 30, 31, 20, 20, 20, 20, 20, 20, 20, 32,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    33, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
    34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
    35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
    35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
};

static RE_UINT8 re_block_stage_2[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   8,   9,  10,  11,  11,  12,  13,
     14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  28,
     29,  30,  31,  31,  32,  32,  32,  33,  34,  34,  34,  34,  34,  35,  36,  37,
     38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  50,  51,  51,
     52,  53,  54,  55,  56,  56,  57,  57,  58,  59,  60,  61,  62,  62,  63,  64,
     65,  65,  66,  67,  68,  68,  69,  69,  70,  71,  72,  73,  74,  75,  76,  77,
     78,  79,  80,  81,  82,  82,  83,  83,  84,  84,  84,  84,  84,  84,  84,  84,
     84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,
     84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  85,  86,  86,  86,  86,
     86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,
     87,  87,  87,  87,  87,  87,  87,  87,  87,  88,  89,  89,  90,  91,  92,  93,
     94,  95,  96,  97,  98,  99, 100, 101, 102, 102, 102, 102, 102, 102, 102, 102,
    102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
    102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 103,
    104, 104, 104, 104, 104, 104, 104, 105, 106, 106, 106, 106, 106, 106, 106, 106,
    107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
    107, 107, 108, 108, 108, 108, 109, 110, 110, 110, 110, 110, 111, 112, 113, 114,
    115, 116, 117, 118,  15, 119, 120, 121, 122, 123,  15,  15,  15,  15,  15,  15,
    124,  15, 125,  15, 126,  15, 127,  15, 128,  15,  15,  15, 129,  15,  15,  15,
    130, 131,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
     15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
    132, 132, 132, 132, 132, 132, 132, 132, 133,  15,  15,  15,  15,  15,  15,  15,
    134, 134, 134, 134, 134, 134, 134, 134, 135,  15,  15,  15,  15,  15,  15,  15,
    136, 136, 136, 136, 137,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
    138, 138,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
    139, 139, 140, 140, 141,  15, 142,  15, 143, 143, 143, 143, 143, 143, 143, 143,
    144, 145, 146, 146, 147, 147, 148, 148, 148, 148, 148, 148, 149, 150, 151,  15,
    152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152,
    152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 153, 154, 154,
    154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
    154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 155, 156,
    157,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
    158, 158, 158, 158, 159,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
    160,  15, 161, 162,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
    163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
};

static RE_UINT8 re_block_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
      2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,
      3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,
      6,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      8,   8,   8,   8,   8,   8,   8,   8,   9,   9,   9,  10,  10,  10,  10,  10,
     10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
     13,  13,  13,  13,  13,  14,  14,  14,  15,  15,  15,  15,  16,  16,  16,  16,
     17,  17,  17,  17,  18,  18,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,
     20,  20,  20,  20,  20,  20,  20,  20,  21,  21,  21,  21,  21,  21,  21,  21,
     22,  22,  22,  22,  22,  22,  22,  22,  23,  23,  23,  23,  23,  23,  23,  23,
     24,  24,  24,  24,  24,  24,  24,  24,  25,  25,  25,  25,  25,  25,  25,  25,
     26,  26,  26,  26,  26,  26,  26,  26,  27,  27,  27,  27,  27,  27,  27,  27,
     28,  28,  28,  28,  28,  28,  28,  28,  29,  29,  29,  29,  29,  29,  29,  29,
     30,  30,  30,  30,  30,  30,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
     32,  32,  32,  32,  32,  32,  32,  32,  33,  33,  33,  33,  33,  33,  33,  33,
     33,  33,  34,  34,  34,  34,  34,  34,  35,  35,  35,  35,  35,  35,  35,  35,
     36,  36,  36,  36,  36,  36,  36,  36,  37,  37,  38,  38,  38,  38,  38,  38,
     39,  39,  39,  39,  39,  39,  39,  39,  40,  40,  41,  41,  41,  41,  41,  41,
     42,  42,  43,  43,  44,  44,  45,  45,  46,  46,  46,  46,  46,  46,  46,  46,
     47,  47,  47,  47,  47,  47,  47,  47,  47,  47,  47,  48,  48,  48,  48,  48,
     49,  49,  49,  49,  49,  50,  50,  50,  51,  51,  51,  51,  51,  51,  52,  52,
     53,  53,  54,  54,  54,  54,  54,  54,  54,  54,  54,  19,  19,  19,  19,  19,
     55,  55,  55,  55,  55,  55,  55,  55,  56,  56,  56,  56,  57,  57,  57,  57,
     58,  58,  58,  58,  58,  59,  59,  59,  19,  19,  19,  19,  19,  60,  60,  60,
     61,  61,  61,  61,  61,  61,  61,  61,  62,  62,  62,  62,  63,  63,  63,  63,
     64,  64,  64,  64,  64,  64,  64,  64,  65,  65,  65,  65,  65,  65,  65,  65,
     66,  66,  66,  66,  66,  66,  66,  67,  67,  67,  68,  68,  68,  69,  69,  69,
     70,  70,  70,  70,  70,  71,  71,  71,  71,  72,  72,  72,  72,  72,  72,  72,
     73,  73,  73,  73,  73,  73,  73,  73,  74,  74,  74,  74,  74,  74,  74,  74,
     75,  75,  75,  75,  76,  76,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,
     78,  78,  78,  78,  78,  78,  78,  78,  79,  79,  80,  80,  80,  80,  80,  80,
     81,  81,  81,  81,  81,  81,  81,  81,  82,  82,  82,  82,  82,  82,  82,  82,
     82,  82,  82,  82,  83,  83,  83,  84,  85,  85,  85,  85,  85,  85,  85,  85,
     86,  86,  86,  86,  86,  86,  86,  86,  87,  87,  87,  87,  87,  87,  87,  87,
     88,  88,  88,  88,  88,  88,  88,  88,  89,  89,  89,  89,  89,  89,  89,  89,
     90,  90,  90,  90,  90,  90,  91,  91,  92,  92,  92,  92,  92,  92,  92,  92,
     93,  93,  93,  94,  94,  94,  94,  94,  95,  95,  95,  95,  95,  95,  96,  96,
     97,  97,  97,  97,  97,  97,  97,  97,  98,  98,  98,  98,  98,  98,  98,  98,
     99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  19, 100,
    101, 101, 101, 101, 102, 102, 102, 102, 102, 102, 103, 103, 103, 103, 103, 103,
    104, 104, 104, 105, 105, 105, 105, 105, 105, 106, 107, 107, 108, 108, 108, 109,
    110, 110, 110, 110, 110, 110, 110, 110, 111, 111, 111, 111, 111, 111, 111, 111,
    112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 113, 113, 113, 113,
    114, 114, 114, 114, 114, 114, 114, 114, 115, 115, 115, 115, 115, 115, 115, 115,
    115, 116, 116, 116, 116, 117, 117, 117, 118, 118, 118, 118, 118, 118, 118, 118,
    118, 118, 118, 118, 119, 119, 119, 119, 119, 119, 120, 120, 120, 120, 120, 120,
    121, 121, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122,
    123, 123, 123, 124, 125, 125, 125, 125, 126, 126, 126, 126, 126, 126, 127, 127,
    128, 128, 128, 129, 129, 129, 130, 130, 131, 131, 131, 131, 131, 131,  19,  19,
    132, 132, 132, 132, 132, 132, 133, 133, 134, 134, 134, 134, 134, 134,  19,  19,
    135, 135, 135,  19,  19,  19,  19,  19,  19,  19,  19,  19, 136, 136, 136, 136,
    137, 137, 137, 137, 137, 137, 137, 137, 137, 137, 137, 138, 138, 138, 138, 138,
    139, 139, 139, 139, 139, 139, 139, 139, 140, 140, 140, 140, 140, 140, 140, 140,
    141, 141, 141, 141, 141, 141, 141, 141, 142, 142, 142, 142, 142, 142, 142, 142,
    143, 143, 143, 143, 143, 143, 143, 143, 144, 144, 144, 144, 144, 145, 145, 145,
    145, 145, 145, 145, 145, 145, 145, 145, 146, 147, 148, 149, 149, 150, 150, 151,
    151, 151, 151, 151, 151, 151, 151, 151, 152, 152, 152, 152, 152, 152, 152, 152,
    152, 152, 152, 152, 152, 152, 152, 153, 154, 154, 154, 154, 154, 154, 154, 154,
    155, 155, 155, 155, 155, 155, 155, 155, 156, 156, 156, 156, 157, 157, 157, 157,
    157, 158, 158, 158, 158, 159, 159, 159, 160, 160, 161, 161, 161, 161,  19,  19,
    162, 162, 162, 163, 163,  19,  19,  19, 164, 164, 165, 165, 165, 165,  19,  19,
    166, 166, 166, 166, 166, 167, 167, 167, 168, 168, 168,  19,  19,  19,  19,  19,
    169, 169, 169, 169, 170, 170,  19,  19, 171, 171, 172, 172,  19,  19,  19,  19,
    173, 173, 173, 173, 173, 173, 174, 174, 175, 175, 175, 175, 176, 176, 177, 177,
    178, 178, 178, 178, 178,  19,  19,  19,  19,  19,  19,  19,  19,  19, 179, 179,
    180, 180, 180, 180, 180, 180, 180, 180, 181, 181, 181, 181, 181,  19,  19,  19,
    182, 182, 182, 182, 182, 182, 182, 182, 183, 183, 183, 183, 183, 183, 183, 183,
    184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184,  19,  19,  19,  19,  19,
    185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185,  19,  19,  19,  19,
    186, 186, 186, 186, 186, 186, 186, 186, 187, 187, 187, 187, 187, 187, 187, 187,
    188, 188, 188, 188, 188, 188, 188, 188, 189, 189, 189, 189, 189,  19,  19,  19,
    190, 190, 190, 190, 190, 190, 191, 191, 192, 192, 192, 192, 192, 192, 192, 192,
    193, 193, 193, 194, 194, 194, 194, 194, 194, 194, 195, 195, 195, 195, 195, 195,
    196, 196, 196, 196, 196, 196, 196, 196, 197, 197, 197, 197, 197, 197, 197, 197,
    198, 198, 198, 198, 198, 198, 198, 198, 199, 199, 199, 199, 199,  19,  19,  19,
    200, 200, 200, 200, 200, 200, 200, 200, 201, 201, 201, 201, 201, 201, 201, 201,
    202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202,  19,  19,
    203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 204, 204, 204, 204,
    204, 204, 204, 204, 204, 204, 204, 204, 204, 204,  19,  19,  19,  19,  19,  19,
    205, 205, 205, 205, 205, 205, 205, 205, 205, 205,  19,  19,  19,  19,  19,  19,
    206, 206, 206, 206, 206, 206, 206, 206, 207, 207, 207, 207, 207, 207, 207, 207,
    207, 207, 207, 207, 207, 207, 207,  19, 208, 208, 208, 208, 208, 208, 208, 208,
    209, 209, 209, 209, 209, 209, 209, 209,
};

static RE_UINT8 re_block_stage_4[] = {
      0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,
      4,   4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,
      8,   8,   8,   8,   9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,
     12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,  15,  15,  15,
     16,  16,  16,  16,  17,  17,  17,  17,  18,  18,  18,  18,  19,  19,  19,  19,
     20,  20,  20,  20,  21,  21,  21,  21,  22,  22,  22,  22,  23,  23,  23,  23,
     24,  24,  24,  24,  25,  25,  25,  25,  26,  26,  26,  26,  27,  27,  27,  27,
     28,  28,  28,  28,  29,  29,  29,  29,  30,  30,  30,  30,  31,  31,  31,  31,
     32,  32,  32,  32,  33,  33,  33,  33,  34,  34,  34,  34,  35,  35,  35,  35,
     36,  36,  36,  36,  37,  37,  37,  37,  38,  38,  38,  38,  39,  39,  39,  39,
     40,  40,  40,  40,  41,  41,  41,  41,  42,  42,  42,  42,  43,  43,  43,  43,
     44,  44,  44,  44,  45,  45,  45,  45,  46,  46,  46,  46,  47,  47,  47,  47,
     48,  48,  48,  48,  49,  49,  49,  49,  50,  50,  50,  50,  51,  51,  51,  51,
     52,  52,  52,  52,  53,  53,  53,  53,  54,  54,  54,  54,  55,  55,  55,  55,
     56,  56,  56,  56,  57,  57,  57,  57,  58,  58,  58,  58,  59,  59,  59,  59,
     60,  60,  60,  60,  61,  61,  61,  61,  62,  62,  62,  62,  63,  63,  63,  63,
     64,  64,  64,  64,  65,  65,  65,  65,  66,  66,  66,  66,  67,  67,  67,  67,
     68,  68,  68,  68,  69,  69,  69,  69,  70,  70,  70,  70,  71,  71,  71,  71,
     72,  72,  72,  72,  73,  73,  73,  73,  74,  74,  74,  74,  75,  75,  75,  75,
     76,  76,  76,  76,  77,  77,  77,  77,  78,  78,  78,  78,  79,  79,  79,  79,
     80,  80,  80,  80,  81,  81,  81,  81,  82,  82,  82,  82,  83,  83,  83,  83,
     84,  84,  84,  84,  85,  85,  85,  85,  86,  86,  86,  86,  87,  87,  87,  87,
     88,  88,  88,  88,  89,  89,  89,  89,  90,  90,  90,  90,  91,  91,  91,  91,
     92,  92,  92,  92,  93,  93,  93,  93,  94,  94,  94,  94,  95,  95,  95,  95,
     96,  96,  96,  96,  97,  97,  97,  97,  98,  98,  98,  98,  99,  99,  99,  99,
    100, 100, 100, 100, 101, 101, 101, 101, 102, 102, 102, 102, 103, 103, 103, 103,
    104, 104, 104, 104, 105, 105, 105, 105, 106, 106, 106, 106, 107, 107, 107, 107,
    108, 108, 108, 108, 109, 109, 109, 109, 110, 110, 110, 110, 111, 111, 111, 111,
    112, 112, 112, 112, 113, 113, 113, 113, 114, 114, 114, 114, 115, 115, 115, 115,
    116, 116, 116, 116, 117, 117, 117, 117, 118, 118, 118, 118, 119, 119, 119, 119,
    120, 120, 120, 120, 121, 121, 121, 121, 122, 122, 122, 122, 123, 123, 123, 123,
    124, 124, 124, 124, 125, 125, 125, 125, 126, 126, 126, 126, 127, 127, 127, 127,
    128, 128, 128, 128, 129, 129, 129, 129, 130, 130, 130, 130, 131, 131, 131, 131,
    132, 132, 132, 132, 133, 133, 133, 133, 134, 134, 134, 134, 135, 135, 135, 135,
    136, 136, 136, 136, 137, 137, 137, 137, 138, 138, 138, 138, 139, 139, 139, 139,
    140, 140, 140, 140, 141, 141, 141, 141, 142, 142, 142, 142, 143, 143, 143, 143,
    144, 144, 144, 144, 145, 145, 145, 145, 146, 146, 146, 146, 147, 147, 147, 147,
    148, 148, 148, 148, 149, 149, 149, 149, 150, 150, 150, 150, 151, 151, 151, 151,
    152, 152, 152, 152, 153, 153, 153, 153, 154, 154, 154, 154, 155, 155, 155, 155,
    156, 156, 156, 156, 157, 157, 157, 157, 158, 158, 158, 158, 159, 159, 159, 159,
    160, 160, 160, 160, 161, 161, 161, 161, 162, 162, 162, 162, 163, 163, 163, 163,
    164, 164, 164, 164, 165, 165, 165, 165, 166, 166, 166, 166, 167, 167, 167, 167,
    168, 168, 168, 168, 169, 169, 169, 169, 170, 170, 170, 170, 171, 171, 171, 171,
    172, 172, 172, 172, 173, 173, 173, 173, 174, 174, 174, 174, 175, 175, 175, 175,
    176, 176, 176, 176, 177, 177, 177, 177, 178, 178, 178, 178, 179, 179, 179, 179,
    180, 180, 180, 180, 181, 181, 181, 181, 182, 182, 182, 182, 183, 183, 183, 183,
    184, 184, 184, 184, 185, 185, 185, 185, 186, 186, 186, 186, 187, 187, 187, 187,
    188, 188, 188, 188, 189, 189, 189, 189, 190, 190, 190, 190, 191, 191, 191, 191,
    192, 192, 192, 192, 193, 193, 193, 193, 194, 194, 194, 194, 195, 195, 195, 195,
    196, 196, 196, 196, 197, 197, 197, 197, 198, 198, 198, 198, 199, 199, 199, 199,
    200, 200, 200, 200, 201, 201, 201, 201, 202, 202, 202, 202, 203, 203, 203, 203,
    204, 204, 204, 204, 205, 205, 205, 205, 206, 206, 206, 206, 207, 207, 207, 207,
    208, 208, 208, 208, 209, 209, 209, 209,
};

static RE_UINT8 re_block_stage_5[] = {
      1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,
      5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,   8,
      9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,  12,  12,
     13,  13,  13,  13,  14,  14,  14,  14,  15,  15,  15,  15,  16,  16,  16,  16,
     17,  17,  17,  17,  18,  18,  18,  18,  19,  19,  19,  19,   0,   0,   0,   0,
     20,  20,  20,  20,  21,  21,  21,  21,  22,  22,  22,  22,  23,  23,  23,  23,
     24,  24,  24,  24,  25,  25,  25,  25,  26,  26,  26,  26,  27,  27,  27,  27,
     28,  28,  28,  28,  29,  29,  29,  29,  30,  30,  30,  30,  31,  31,  31,  31,
     32,  32,  32,  32,  33,  33,  33,  33,  34,  34,  34,  34,  35,  35,  35,  35,
     36,  36,  36,  36,  37,  37,  37,  37,  38,  38,  38,  38,  39,  39,  39,  39,
     40,  40,  40,  40,  41,  41,  41,  41,  42,  42,  42,  42,  43,  43,  43,  43,
     44,  44,  44,  44,  45,  45,  45,  45,  46,  46,  46,  46,  47,  47,  47,  47,
     48,  48,  48,  48,  49,  49,  49,  49,  50,  50,  50,  50,  51,  51,  51,  51,
     52,  52,  52,  52,  53,  53,  53,  53,  54,  54,  54,  54,  55,  55,  55,  55,
     56,  56,  56,  56,  57,  57,  57,  57,  58,  58,  58,  58,  59,  59,  59,  59,
     60,  60,  60,  60,  61,  61,  61,  61,  62,  62,  62,  62,  63,  63,  63,  63,
     64,  64,  64,  64,  65,  65,  65,  65,  66,  66,  66,  66,  67,  67,  67,  67,
     68,  68,  68,  68,  69,  69,  69,  69,  70,  70,  70,  70,  71,  71,  71,  71,
     72,  72,  72,  72,  73,  73,  73,  73,  74,  74,  74,  74,  75,  75,  75,  75,
     76,  76,  76,  76,  77,  77,  77,  77,  78,  78,  78,  78,  79,  79,  79,  79,
     80,  80,  80,  80,  81,  81,  81,  81,  82,  82,  82,  82,  83,  83,  83,  83,
     84,  84,  84,  84,  85,  85,  85,  85,  86,  86,  86,  86,  87,  87,  87,  87,
     88,  88,  88,  88,  89,  89,  89,  89,  90,  90,  90,  90,  91,  91,  91,  91,
     92,  92,  92,  92,  93,  93,  93,  93,  94,  94,  94,  94,  95,  95,  95,  95,
     96,  96,  96,  96,  97,  97,  97,  97,  98,  98,  98,  98,  99,  99,  99,  99,
    100, 100, 100, 100, 101, 101, 101, 101, 102, 102, 102, 102, 103, 103, 103, 103,
    104, 104, 104, 104, 105, 105, 105, 105, 106, 106, 106, 106, 107, 107, 107, 107,
    108, 108, 108, 108, 109, 109, 109, 109, 110, 110, 110, 110, 111, 111, 111, 111,
    112, 112, 112, 112, 113, 113, 113, 113, 114, 114, 114, 114, 115, 115, 115, 115,
    116, 116, 116, 116, 117, 117, 117, 117, 118, 118, 118, 118, 119, 119, 119, 119,
    120, 120, 120, 120, 121, 121, 121, 121, 122, 122, 122, 122, 123, 123, 123, 123,
    124, 124, 124, 124, 125, 125, 125, 125, 126, 126, 126, 126, 127, 127, 127, 127,
    128, 128, 128, 128, 129, 129, 129, 129, 130, 130, 130, 130, 131, 131, 131, 131,
    132, 132, 132, 132, 133, 133, 133, 133, 134, 134, 134, 134, 135, 135, 135, 135,
    136, 136, 136, 136, 137, 137, 137, 137, 138, 138, 138, 138, 139, 139, 139, 139,
    140, 140, 140, 140, 141, 141, 141, 141, 142, 142, 142, 142, 143, 143, 143, 143,
    144, 144, 144, 144, 145, 145, 145, 145, 146, 146, 146, 146, 147, 147, 147, 147,
    148, 148, 148, 148, 149, 149, 149, 149, 150, 150, 150, 150, 151, 151, 151, 151,
    152, 152, 152, 152, 153, 153, 153, 153, 154, 154, 154, 154, 155, 155, 155, 155,
    156, 156, 156, 156, 157, 157, 157, 157, 158, 158, 158, 158, 159, 159, 159, 159,
    160, 160, 160, 160, 161, 161, 161, 161, 162, 162, 162, 162, 163, 163, 163, 163,
    164, 164, 164, 164, 165, 165, 165, 165, 166, 166, 166, 166, 167, 167, 167, 167,
    168, 168, 168, 168, 169, 169, 169, 169, 170, 170, 170, 170, 171, 171, 171, 171,
    172, 172, 172, 172, 173, 173, 173, 173, 174, 174, 174, 174, 175, 175, 175, 175,
    176, 176, 176, 176, 177, 177, 177, 177, 178, 178, 178, 178, 179, 179, 179, 179,
    180, 180, 180, 180, 181, 181, 181, 181, 182, 182, 182, 182, 183, 183, 183, 183,
    184, 184, 184, 184, 185, 185, 185, 185, 186, 186, 186, 186, 187, 187, 187, 187,
    188, 188, 188, 188, 189, 189, 189, 189, 190, 190, 190, 190, 191, 191, 191, 191,
    192, 192, 192, 192, 193, 193, 193, 193, 194, 194, 194, 194, 195, 195, 195, 195,
    196, 196, 196, 196, 197, 197, 197, 197, 198, 198, 198, 198, 199, 199, 199, 199,
    200, 200, 200, 200, 201, 201, 201, 201, 202, 202, 202, 202, 203, 203, 203, 203,
    204, 204, 204, 204, 205, 205, 205, 205, 206, 206, 206, 206, 207, 207, 207, 207,
    208, 208, 208, 208, 209, 209, 209, 209,
};

/* Block: 4120 bytes. */

RE_UINT32 re_get_block(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 11;
    code = ch ^ (f << 11);
    pos = (RE_UINT32)re_block_stage_1[f] << 4;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_block_stage_2[pos + f] << 3;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_block_stage_3[pos + f] << 2;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_block_stage_4[pos + f] << 2;
    value = re_block_stage_5[pos + code];

    return value;
}

/* Script. */

static RE_UINT8 re_script_stage_1[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11, 12, 12, 12, 12, 13, 14, 14, 14, 14, 15,
    16, 17, 18, 14, 19, 14, 20, 14, 14, 14, 14, 14, 14, 21, 14, 14,
    14, 14, 14, 14, 14, 14, 22, 14, 14, 14, 23, 14, 14, 14, 24, 14,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 25,  7, 26, 27, 14, 14, 14, 14, 14, 14, 14, 28,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    29, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
};

static RE_UINT8 re_script_stage_2[] = {
      0,   1,   2,   2,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,
     14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
     30,  31,  32,  32,  33,  34,  35,  36,  37,  37,  37,  37,  37,  38,  39,  40,
     41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,   2,   2,  53,  54,
     55,  56,  57,  58,  59,  59,  59,  60,  61,  59,  59,  59,  59,  59,  62,  63,
     64,  64,  59,  59,  59,  59,  65,  15,  66,  67,  68,  69,  70,  71,  72,  73,
     74,  75,  76,  77,  78,  79,  80,  59,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  81,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  82,
     83,  83,  83,  83,  83,  83,  83,  83,  83,  84,  85,  85,  86,  87,  88,  89,
     90,  91,  92,  93,  94,  95,  96,  97,  32,  32,  32,  32,  32,  32,  32,  32,
     32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
     32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  98,
     15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
     15,  15,  72,  72,  99, 100, 101, 102, 103, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113,  15, 114, 115, 116, 117, 118,  15,  15,  15,  15,  15,  15,
    119,  15, 120,  15, 121,  15, 122,  15, 123,  15,  15,  15, 124,  15,  15,  15,
    125, 126,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
    127, 127, 127, 127, 127, 127, 128,  15, 129,  15,  15,  15,  15,  15,  15,  15,
    130, 130, 130, 130, 130, 130, 130, 130, 131,  15,  15,  15,  15,  15,  15,  15,
    132, 132, 132, 132, 133,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
    134,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
     59, 135, 136, 137, 138,  15, 139,  15, 140, 141, 142,  59,  59, 143,  59, 144,
    145, 146, 147, 148, 149,  15, 150, 151, 152, 153, 154, 155, 156, 157,  60,  15,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72, 158,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72, 159,  72,
    160,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
     72,  72,  72,  72, 160,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
    161,  15, 162, 163,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,
};

static RE_UINT16 re_script_stage_3[] = {
      0,   0,   0,   0,   1,   2,   1,   2,   0,   0,   3,   3,   4,   5,   4,   5,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   6,   0,   0,   7,   0,
      8,   8,   8,   8,   8,   8,   8,   9,  10,  11,  12,  11,  11,  11,  13,  11,
     14,  14,  14,  14,  14,  14,  14,  14,  15,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  16,  17,  18,  19,  17,  18,  20,  21,  22,  22,  23,  22,  24,  25,
     26,  27,  28,  28,  29,  30,  31,  32,  28,  28,  28,  28,  28,  33,  28,  28,
     34,  35,  35,  35,  36,  28,  28,  28,  37,  37,  37,  38,  39,  39,  39,  40,
     41,  41,  42,  43,  44,  45,  46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
     47,  47,  47,  47,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,
     59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,
     75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,
     91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,  46, 121,
    122, 123, 123, 124, 123, 125,  46,  46, 126, 127, 128, 129, 130, 131,  46,  46,
    132, 132, 132, 132, 133, 132, 134, 135, 132, 133, 132, 136, 136, 137,  46,  46,
    138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 139, 139, 140, 139, 139, 141,
    142, 142, 142, 142, 142, 142, 142, 142, 143, 143, 143, 143, 144, 145, 143, 143,
    144, 143, 143, 146, 147, 148, 143, 143, 143, 147, 143, 143, 143, 149, 143, 150,
    143, 151, 152, 152, 152, 152, 152, 153, 154, 154, 154, 154, 154, 154, 154, 154,
    155, 156, 157, 157, 157, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
    168, 168, 168, 168, 168, 169, 170, 170, 171, 172, 173, 173, 173, 173, 173, 174,
    173, 173, 175, 154, 154, 154, 154, 176, 177, 178, 179, 179, 180, 181, 182, 183,
    184, 184, 185, 184, 186, 187, 168, 168, 188, 189, 190, 190, 190, 191, 190, 192,
    193, 193, 194,  46,  46,  46,  46,  46, 195, 195, 195, 195, 196, 195, 195, 197,
    198, 198, 199, 200, 201, 201, 201, 202, 203, 203, 203, 204, 205, 206, 206, 206,
     46,  46,  46,  46,  46, 207, 208, 209,   4,   4, 210,   4,   4, 211, 212, 213,
      4,   4,   4, 214,   8,   8, 215, 216,  11, 217,  11,  11, 217, 218,  11, 219,
     11,  11,  11, 220, 220, 221,  11, 222, 223,   0,   0,   0,   0,   0, 224, 225,
    226, 227,   0, 228,  46,   8,   8, 229,   0,   0, 230, 231, 232,   0,   4,   4,
    233,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 234,   0,   0, 235,  46, 236,  46,   0,   0,
    237,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 238,   0,   0,   0,
    239, 239, 239, 239, 239, 239, 239, 239,   0,   0,   0,   0, 240, 228,  46,  46,
    241, 241, 242, 241, 241, 242,   4,   4, 243, 243, 243, 243, 243, 243, 243, 244,
    139, 139, 140, 245, 245, 245, 246, 247, 143, 248, 249, 249, 249, 249,  14,  14,
      0,   0,   0, 250,  46,  46,  46,  46, 251, 252, 251, 251, 251, 251, 251, 253,
    251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 254,  46, 255,
    256,   0, 257, 258, 259, 260, 260, 260, 260, 261, 262, 263, 263, 263, 263, 264,
    265, 266, 267, 268, 142, 142, 142, 142, 269,   0, 266, 270,   0,   0, 234, 263,
    142, 269,   0,   0,   0,   0, 142, 271,   0,   0,   0,   0,   0, 263, 263, 272,
    263, 263, 263, 263, 263, 273,   0,   0, 251, 251, 251, 254,   0,   0,   0,   0,
    251, 251, 251, 251, 274,  46,  46,  46, 275, 275, 275, 275, 275, 275, 275, 275,
    276, 275, 275, 275, 277, 278, 278, 278, 279, 279, 279, 279, 279, 279, 279, 279,
    279, 279, 280,  46,  14,  14,  14, 281,  14,  16, 282, 282, 282, 282, 282, 283,
      0,   0, 284,   4,   4,   4,   4,   4, 285, 286, 287,  46,  46,  46,  46, 288,
    289, 289, 290, 228, 291, 291, 291, 292, 293, 293, 293, 293, 294, 295,  47, 296,
    297, 297, 297, 298, 298, 299, 142, 300, 301, 301, 301, 301, 302, 303,  46,  46,
    304, 304, 304, 305, 306, 307, 138, 308, 309, 309, 309, 309, 310, 311,  46,  46,
    312, 313, 249,  46,  46,  46,  46,  46,  46,  46,  46,  46, 314, 314, 315, 316,
    142, 142, 317, 142, 318, 142, 142, 319, 251, 251, 320, 251, 251, 251, 320, 251,
    251, 251, 251, 251, 251, 321,  46,  46, 322, 323,  22, 324, 325,  28,  28,  28,
     28,  28,  28,  28, 326, 327,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,
     28,  28,  28, 328,  46,  28,  28,  28,  28, 329,  28,  28, 330,  46,  46, 331,
      8, 228, 215,   0,   0, 332, 333, 334,  28,  28,  28,  28,  28,  28,  28, 335,
    237,   0,   1,   2,   1,   2, 336, 262, 263, 337, 142, 269, 338, 339, 340, 341,
    342, 343, 344, 345, 346, 346,  46,  46, 343, 343, 343, 343, 343, 343, 343, 347,
    348,   0,   0, 349,  11,  11,  11,  11, 350, 255,  46,  46,  46,   0,   0, 351,
    352, 353, 354, 354, 354, 355,  46,  46, 356, 357, 358, 359, 360,  46,  46,  46,
    361, 362, 363, 363, 364, 365,  46,  46, 366, 366, 366, 366, 366, 367, 367, 367,
    368, 369, 370,  46,  46,  46,  46,  46, 371, 372, 372, 373, 374, 375,  46,  46,
    376, 377, 378, 379,  46,  46,  46,  46, 380, 381, 382, 383, 384, 385, 386, 386,
    387, 387, 387, 388, 389, 390, 391, 392, 393, 393, 393, 393, 394,  46,  46,  46,
     46,  46,  46,  46,  46,  46,  28, 395, 396, 396, 396, 396, 397, 398, 396,  46,
    399, 399, 399, 399, 400,  46,  46,  46, 401, 401, 401, 401, 401, 401, 401, 401,
    401, 401, 401, 401, 401, 401, 402,  46, 401, 401, 401, 401, 401, 401, 403, 404,
    405, 405, 405, 405, 405, 405, 405, 405, 405, 405, 406,  46,  46,  46,  46,  46,
    282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 407,  46,  46,  46,  46,
    408,  46,  46,  46,  46,  46,  46,  46,   0,   0,   0,   0,   0,   0,   0, 409,
      0,   0, 410,   0,   0,   0, 411, 412, 413,   0, 414,   0,   0, 415,  46,  46,
     11,  11,  11,  11, 416,  46,  46,  46,   0,   0,   0,   0,   0, 235,   0, 250,
      0,   0,   0,   0,   0, 417,   0,   0,   0, 418, 419, 420, 421,   0,   0,   0,
    422, 423,   0, 424, 425, 426,   0,   0,   0,   0, 427,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 428,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,
      0, 234, 226, 429, 237, 237,  46,  46, 236,   0, 226,   0,   0,   0, 228,   0,
      0, 236,  46,  46,  46,  46, 430,   0, 431,   0,   0, 236, 432, 250,  46,  46,
      0,   0, 433, 434,   0,   0,   0, 240,   0, 234,   0,   0, 435,  46,   0, 433,
      0,   0,   0, 226, 426,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 436,
      0,   0,   0, 415,  46,   0, 437,  46,  46,  46,  46,  46,  46,  46,  46, 438,
    237, 439, 440, 421, 441,  46,  46,  46,   0,   0,   0,   0, 409,  46,  46,  46,
    251, 251, 251, 251, 251, 442,  46,  46, 251, 251, 251, 443, 251, 251, 251, 251,
    251, 320,  46,  46,  46,  46,  46,  46, 444,  46,   0,   0,   0,   0,   0,   0,
      8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  46,
};

static RE_UINT8 re_script_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   3,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,
      2,   2,   2,   2,   3,   0,   0,   0,   2,   2,   3,   0,   0,   4,   0,   0,
      5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   7,   6,   8,   6,   6,   9,
      8,   8,  10,  10,   6,  11,  11,   6,   6,   6,   6,   6,   6,   6,   6,   6,
      6,  12,   6,   6,   6,   6,   6,   6,   6,  13,  13,  13,  13,  13,  13,  13,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  15,  16,  14,  14,  14,  14,
     14,  14,  14,  14,   8,   8,   8,   8,  17,  18,  18,  18,  18,  18,  18,  18,
     18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  19,  17,  18,  18,  18,
     18,  18,  18,  18,  20,  19,   8,   8,  21,  22,  22,  22,  22,  22,  22,  22,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,   8,   8,   8,   8,
     22,  22,  22,  22,  22,  23,   8,   8,  22,  22,  23,   8,   8,   8,   8,   8,
     24,  24,   8,  24,  24,  24,  25,  24,  24,  24,  24,  24,  24,  26,   8,  26,
     24,  24,  24,  24,  24,  24,  24,  24,  25,  24,  24,  24,  24,  27,   5,   5,
      5,   5,   5,  24,  24,  24,  24,  27,   0,   0,   0,   0,   0,  24,  24,  24,
     28,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  26,  24,
     29,  29,  29,  29,  29,  29,  29,  30,  29,  29,  29,  29,  29,  29,  29,  29,
     29,  29,  29,  29,  29,  31,  30,  29,  32,  32,  32,  32,  32,  32,  32,  32,
     32,   8,   8,   8,   8,   8,   8,   8,  33,  33,  33,  33,  33,  33,  33,  33,
     33,  33,  33,  33,  33,  34,   8,   8,  35,  35,  35,  35,  35,  35,  35,  35,
     35,  35,  35,  35,  35,  35,  35,   8,  35,  35,  35,  35,  35,  35,  35,  36,
     37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,   8,  38,
      8,   8,   8,   8,   8,   8,   8,   8,  39,  39,  39,  39,  39,  39,  39,  39,
     40,  41,  39,  39,  39,  39,  39,  39,  39,  39,   0,  39,  39,  39,  39,  39,
     42,  39,  39,  39,  43,  39,  39,  39,  44,  45,  44,  45,  45,  45,  46,  44,
     46,  44,  45,  45,  45,  45,  45,  45,  45,  45,  45,  45,  46,  45,  45,  45,
     46,  46,   8,  45,  45,   8,  45,  45,  45,  45,  46,  44,  46,  44,  45,  46,
      8,   8,   8,  44,   8,   8,  45,  44,  45,  45,   8,  45,  45,  45,  45,  45,
     45,  45,  45,  45,  45,  45,   8,   8,  47,  48,  47,  48,  48,  49,   8,  47,
     49,  47,  48,  48,  48,  48,  48,  48,  48,  48,  48,  48,  49,  48,  48,  48,
     49,  48,  47,  49,  48,   8,  49,  48,  48,  49,   8,  47,  49,  47,  48,   8,
     47,   8,   8,   8,  47,  48,  49,  49,   8,   8,   8,  48,  48,  48,  48,  48,
     48,  48,  48,   8,   8,   8,   8,   8,  50,  51,  50,  51,  51,  51,  51,  50,
     51,  50,  51,  51,  51,  51,  51,  51,  51,  51,  51,  51,  52,  51,  51,  51,
     52,  51,  50,  51,  51,   8,  51,  51,  51,  51,  51,  50,  51,  50,  51,   8,
     52,   8,   8,   8,   8,   8,   8,   8,  51,  51,   8,  51,  51,  51,  51,  51,
     50,   8,   8,   8,   8,   8,   8,   8,  53,  54,  53,  54,  54,  54,  55,  53,
     55,  53,  54,  54,  54,  54,  54,  54,  54,  54,  54,  54,  55,  54,  54,  54,
     55,  54,  53,  54,  54,   8,  54,  54,  54,  54,  55,  53,  55,  53,  54,   8,
      8,   8,   8,  54,   8,   8,  54,  53,  54,  54,   8,  54,  54,  54,  54,  54,
     54,  54,  54,  54,   8,   8,   8,   8,   8,  56,  57,  56,  56,  58,   8,  56,
     58,  56,  56,   8,  57,  58,  58,  56,   8,  57,  58,   8,  56,  58,   8,  56,
     56,  56,  56,  56,  56,   8,   8,  56,  56,  58,   8,  56,  58,  56,  56,   8,
     58,   8,   8,  57,   8,   8,   8,   8,   8,   8,   8,  56,  56,  56,  56,  56,
     56,  56,  56,  56,  56,  58,   8,   8,  59,  60,  59,  60,  60,  60,  61,  60,
     61,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  61,  60,  60,  60,
     60,  60,  59,  60,  60,   8,  59,  60,  60,  60,  61,  60,  61,  60,  60,   8,
      8,   8,  59,  61,  60,   8,   8,   8,  60,  60,   8,  60,  60,  60,  60,  60,
      8,   8,   8,   8,  60,  60,  60,  60,   8,  62,  63,  62,  62,  62,  64,  62,
     64,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  64,  62,  62,  62,
     62,  62,  63,  62,  62,   8,  62,  62,  62,  62,  64,  62,  64,  62,  62,   8,
      8,   8,  63,  64,   8,   8,   8,  64,  62,  62,   8,  62,  62,  62,  62,  62,
     63,  64,   8,   8,   8,   8,   8,   8,   8,  65,  66,  65,  65,  65,  67,  65,
     67,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,
     65,  65,  65,  65,  65,  67,  66,  65,  65,  65,  67,  65,  67,  65,  65,  67,
      8,   8,   8,  66,   8,   8,   8,   8,  65,  65,   8,  65,  65,  65,  65,  65,
     65,  65,  65,   8,  66,  65,  65,  65,   8,  68,  69,  68,  68,  68,  68,  68,
     68,  68,  68,  70,   8,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  69,  68,  68,  68,  68,  69,   8,  68,  68,  68,  70,   8,  70,   8,  69,
     68,  68,  70,  70,  68,  68,  68,  68,   8,  68,  70,   8,   8,   8,   8,   8,
     71,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  73,   8,  20,  72,  72,  72,  72,  72,  72,   8,   8,
     74,  75,  75,  74,  75,  75,  74,   8,   8,   8,  76,  76,  74,  76,  76,  76,
     74,  76,  74,  74,   8,  76,  74,  76,  76,  76,  76,  76,  76,  74,  76,   8,
     76,  76,  75,  75,  76,  76,  76,   8,  76,  76,  76,  76,  76,   8,  76,   8,
     77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  78,  77,  77,  77,
     77,  77,  77,  77,  77,  77,  79,   8,  78,  77,  77,  77,  77,  77,  77,  77,
     77,  77,  77,  77,  77,  77,  79,  77,  77,  77,  80,   0,  81,  79,   8,   8,
     82,  82,  82,  82,  82,  82,  82,  82,  83,  83,  83,  83,  83,  83,  83,  83,
     83,  83,  83,   8,   8,   8,   8,   8,  83,  83,  83,  83,  83,  84,  85,   8,
     86,  86,  86,  86,  86,  86,  86,  86,  87,  87,  87,  87,  87,  87,  87,  87,
     87,  87,  87,  87,  88,  87,  87,   8,  87,  87,  87,  88,  88,  87,  87,   8,
     88,  87,  87,   8,  87,  87,  87,  88,  88,  87,  87,   8,  87,  87,  87,  87,
     87,  87,  87,  88,  87,  87,  87,  87,  87,  87,  87,  87,  87,  88,  89,  87,
     87,  87,  87,  87,  87,  87,  88,   8,  87,  87,  87,  87,  87,   8,   8,   8,
     90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  91,   8,   8,   8,   8,   8,
     92,  92,  92,  92,  92,  92,  92,  92,  93,  93,  93,  93,  93,  93,  93,  93,
     93,  93,  93,  93,  93,  93,  94,   8,  95,  95,  95,  95,  95,  95,  95,  95,
     95,  95,  95,  95,  95,  96,   0,  95,  97,   8,   8,   8,   8,   8,   8,   8,
     98,  98,  98,  98,  98,  98,  99,  98,  98,  98,  99,   8,   8,   8,   8,   8,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 101,   9,   8,   8,   8,   8,
    102, 102, 102, 102, 102, 102, 102, 102, 102, 102,   8,   8,   8,   8,   8,   8,
    103, 103, 103, 103, 103, 103, 104, 103, 104, 103,   8,   8,   8,   8,   8,   8,
    105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105,   8,
    105, 105, 105, 105, 105,   8,   8,   8, 106,   0, 107, 106, 106, 106, 106, 108,
    106, 106, 106, 106, 106,   8,   8,   8, 106, 106, 106, 106, 106, 106, 106, 106,
    106, 106, 106, 106,   8,   8,   8,   8, 106, 106, 106, 106, 106, 108,   8,   8,
     92,  92,  92,   8,   8,   8,   8,   8, 109, 109, 109, 109, 109, 109, 109, 109,
    109, 109, 109, 109, 109, 109, 110,   8, 109, 109, 109, 109, 109, 109,   8,   8,
    110,   8, 109, 109, 109, 109, 109, 109, 111, 111, 111, 111, 111, 111, 111, 111,
    111, 111, 111, 111, 111, 111, 111,   8, 111, 111, 112,   8,   8,   8,   8,   8,
    113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113,   8,   8,
    113, 113, 113, 113, 113,   8,   8,   8, 113, 113, 113, 113, 113, 114,   8, 113,
    115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,   8, 115,
    116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 117,
    116, 116, 116, 116, 116, 116, 117, 118, 116, 116, 116, 116, 116,   8,   8,   8,
    116, 116, 116, 116, 116, 116, 116,   8, 119, 119, 119, 119, 119, 119, 119, 119,
    119, 119, 119, 119, 119, 119,   8,   8, 119, 119, 119, 119, 119, 119, 120,   8,
    121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 122,   8, 121,
    121, 121, 121, 121, 121,   8,   8,   8, 123, 123, 123, 123, 123, 123, 123, 123,
    123, 123,   8,   8,   8,   8, 123, 123, 124, 124, 124, 124, 124, 124, 124, 124,
    124, 124, 124, 124,   8, 125, 124, 124, 124, 124, 124, 124, 124,   8, 125, 124,
    126, 126, 126, 126, 126, 126, 126, 126,   5, 127,   5,   5,   5,   5,   5,   5,
    127,   5,   5,   5, 127,   0, 128,   0,   0,   9,   8,   8,   8,   8,   8,   8,
      2,   2,   2,   6,   6, 129,   2,   2,   2,   2,   2,   2,   2,   2, 130,   6,
      6,   2,   2,   6,   6, 131,   2,   2,   2,   2,   2,   2, 132,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2, 130,   5,   5,   5, 133,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8,   5,   5,   6,   6,   6,   8,   6,   6,   6,   8,
      6,   6,   6,   6,  12,  12,  12,  12,   6,   6,   6,   6,   6,   6,   6,   8,
      6,   6,  11,   6,   6,   6,   6,   6,   6,   6,   8,   6,   6,   6,  12,   6,
      8,   6,  11,   6,   6,   6,   6,  11,   0,   0,   0,   0,   0,   0,   5,   0,
      0,   0,   9,   8,   8,   0,   0,   0,   1,   8,   0,   0,   0,   0,   0,   1,
      0,   0,   0,   0,   0,   0,   0,   9,   2,   2,   2,   2,   2,   2, 134,   8,
      0,   0,   0,   0,   0,   8,   8,   8, 133,   8,   8,   8,   8,   8,   8,   8,
      0,   0,   0,  10,   0,   2,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   3,   2,   2,   2,   2,   3,   8,   8,   8,
      0,   0,   8,   8,   8,   8,   8,   8,   0,   0,   0,   9,   8,   8,   8,   8,
      0,   0,   0,   0,   0,   9,   8,   8,  20,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   9,   9,   0, 135, 135, 135, 135, 135, 135, 135, 135,
      0,   0,   0,   0,   0,   0,   9,   8, 136, 136, 136, 136, 136, 136, 136, 136,
    136, 136, 136, 136, 136, 136, 136, 137,  13,  13,  13,  13,  13,  13,  13,  13,
     13,   8,   8,   8, 138,  13,  13,  13, 139, 139, 139, 139, 139, 139, 139, 139,
    139, 139, 139,   8,   8,   8,   8, 140, 141,   8,   8,   8,   8,   8,   8, 140,
     87,  87,  87,  88,   8,   8,   8,   8,  87,  87,  87,  88,  87,  87,  87,  88,
      0,   8,   8,   8,   8,   8,   8,   8, 142, 142, 142, 142, 142, 142, 142, 142,
    142, 142, 142, 142, 142, 143, 142, 142, 142, 142,   8,   8,   8,   8,   8,   8,
    142, 142, 142,   8,   8,   8,   8,   8,   0,   0,   0,   0,   0,   0,   8,   8,
      0,   0, 144, 144,   0,   0,   0,   0, 144, 142, 142, 142, 142,   5,   5,  86,
      0,   0,   0,   0, 142, 142,   0,   0, 145, 146, 146, 146, 146, 146, 146, 146,
    146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 147, 148, 127, 149, 146,
    150, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151,
    151, 151, 151, 151, 151, 152, 150, 151,   8,   8, 153,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   8,
    154,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86, 155,
      4,   4,   4,   4,   4, 156,   8,   8,  86,  86,  86,  86,  86,  86,  86, 157,
    151, 151, 151, 151, 151, 151, 151, 158, 151, 151, 151, 151,   0,   0,   0,   0,
    142, 142, 142, 142, 142, 142,   8,   8, 159, 159, 159, 159, 159, 159, 159, 159,
    159, 159, 159, 159, 159, 159, 160,   8, 159, 159, 159, 160,   8,   8,   8,   8,
    161, 161, 161, 161, 161, 161, 161, 161, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162,   8,   8,  14,  14,   8,   8,   8,   8,  14,  14,
    163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163,   8,   8,   8,   8,
      0,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   0,   1,   2, 134,
      2,   8,   8,   8,   8,   8,   8,   8,   2,   2,   2,   2,   2,   8,   8,   8,
      8,   8,   8,   8,   8,   2,   2,   2, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164,   8,   8, 165, 165, 165, 165, 165, 165, 165, 165,
    165, 165, 165, 165,   8,   8,   8,   8, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 167,   8,   8,   8,   8, 166, 166, 166, 166, 166, 166,   8,   8,   8,
     39,  39,  39,  39,  39,  39,   8,   8, 168, 168, 168, 168, 168, 168, 168, 168,
    169, 169, 169, 169, 169, 169, 169, 169, 169, 169,   8,   8,   8,   8,   8, 170,
     86,  86,  86,  86,  86,  86, 155,   8, 171, 171, 171, 171, 171, 171, 171, 171,
    171, 171, 171, 171, 171, 171, 171, 172, 171, 171, 171, 171, 171,   8,   8, 171,
    173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 174,   8,   8,   8,   8,
    173, 173, 173, 173, 173, 173, 173,   8, 173, 173, 173, 173, 173,   8, 173, 173,
     82,  82,  82,  82,  82,  82,   8,   8, 175, 175, 175, 175, 175, 175, 175, 175,
    175, 176,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8, 177, 175, 175,
     89,  87,  87,  88,  89,  87,  87,  88,  89,  87,  87,  88,   8,   8,   8,   8,
    178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178,   8,
    178, 178, 178, 178, 178,   8,   8,   8,  86,  86,   8,   8,   8,   8,   8,   8,
     86,  86,  86, 155,   8, 154,  86,  86,  86,  86,  86,  86,  86,  86,   8,   8,
    142, 142, 142, 142, 142, 142, 142,   8, 142, 142, 142, 142, 142,   8,   8,   8,
      2,   2,   2, 134,   8,   8,   8,   8,   8,  17,  18,  18,   8,   8,  21,  22,
     22,  22,  22,  23,  22,  22,  23,  23,  22,  21,  23,  22,  22,  22,  22,  22,
     24,   8,   8,   8,   8,   8,   8,   8,   8, 179,  24,  24,  24,  24,  24,  24,
     24,  24,  24,  24,  24,  24,  24,   0,   8,  24,  24,  24,  24,  24,  24,  24,
     24,  24,  24,  24,   8,   8,   8,   8,  24,  24,  24,  24,  24,  24,  26,   8,
      0,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9,   0,   0,   8,   8,
     24,  24, 180,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 180,  20,
      0,   0,   0, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151,   0,
      8,  86,  86,  86,   8,  86,  86,  86,   8,  86,  86,  86,   8,  86, 155,   8,
      0,   0,   0,   9,   0,   0,   0,   9,   8,   8,   8,   8,  20,   0,   0,   8,
    181, 181, 181, 181, 181, 181, 182, 181, 181, 181, 181, 181, 181, 181, 181, 181,
    181, 181, 181, 183, 181, 181, 181, 181, 181, 181, 181, 181, 181, 183, 181, 182,
    181, 181, 181, 181, 181, 181, 181,   8, 181, 181, 181, 181, 181, 183,   8,   8,
      0,   9,   8,  20,   0,   0,   0,   0,   0,   0,   8,  20,   0,   0,   0,   0,
      6,   6,   6,   6,   6,  11,   8,   8,   0,   0,   0,   0,   0,   0, 128,   8,
    184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 185,   8,
    186, 186, 186, 186, 186, 186, 186, 186, 187,   8,   8,   8,   8,   8,   8,   8,
    188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 189,
    188, 188,   8,   8,   8,   8,   8,   8, 190, 190, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 191,   8,   8, 192, 192, 192, 192, 192, 192, 192, 192,
    192, 192, 192, 192, 192, 192, 192, 193, 194, 194, 194, 194, 194, 194, 194, 194,
    194, 194,   8,   8, 194, 194, 194, 194, 194, 194, 194,   8,   8,   8,   8,   8,
    195, 195, 195, 195, 195, 195, 195, 195, 196, 196, 196, 196, 196, 196, 196, 196,
    197, 197, 197, 197, 197, 197, 197, 197, 197, 197, 197, 197, 197, 197, 197,   8,
    197, 197, 197, 197, 197,   8,   8,   8, 198, 198, 198,   8, 199, 198, 198, 198,
    198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 200, 199,   8, 199, 200,
    201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 202, 201, 201, 201, 201,
    203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203,   8, 204,
    205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,   8,   8, 206,
    207, 207, 208, 209,   8,   8, 207, 207, 207, 207, 208, 207, 208, 207, 207, 207,
    207, 207, 207, 207, 207, 207, 207, 207, 207, 207,   8,   8, 207, 209,   8, 208,
    207, 207, 207, 207,   8,   8,   8,   8, 207, 207, 207, 207, 209,   8,   8,   8,
    210, 210, 210, 210, 210, 210, 210, 210, 211, 211, 211, 211, 211, 211, 211, 211,
    211, 211, 211,   8, 212, 211, 211, 211, 213, 213, 213, 213, 213, 213, 213, 213,
    213, 213, 213,   8, 213, 213, 213, 213, 214, 214, 214, 214, 214, 214, 214, 214,
    214, 215,   8,   8, 214, 214, 214, 214, 216, 216, 216, 216, 216, 216, 216, 216,
    216, 216, 216, 216, 217,   8,   8,   8,  24,  24,  24,  24,  24,  24,  24, 180,
    218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218,   8,
      8, 218, 218, 218, 218, 218, 218, 218, 219, 219, 219, 219, 219, 219, 219, 219,
    219,   8,   8,   8,   8,   8,   8,   8, 220, 220, 220, 220, 220, 220, 220, 220,
    220, 220, 220, 220, 220, 220, 220, 221, 220, 221,   8,   8,   8,   8,   8,   8,
    220, 220,   8,   8,   8,   8,   8,   8, 222, 222, 222, 222, 222, 222, 222, 222,
    222, 222, 222, 222, 222, 222, 222, 223, 163, 163, 163, 163, 224,   8,   8,   8,
    225,   8,   8,   8,   8,   8,   8,   8,   0,   0,   0,   8,   8,   8,   8,   8,
      0,   0,   0,   9,  20,   0,   0,   0,   0,   0,   0, 128,   5,   0,   0,   0,
      0,   0,   0,   0,   0, 128,   5,   5,   5, 127, 128,   5,   5,   5,   0,   0,
      0,   0,   0,   0,   0,   5,   5,   0,   0,   0,   0,   0,   0,   0,   0,   8,
      6,   6,   6,   8,   8,   8,   8,   8,   0,   0,   9,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   9,   0,   8,   9,  20,   9,  20,   0,   9,   0,
      0,   0,   0,   0,   0,  20,  20,   0,   0,   0,  20,   0,   0,   0,   0,   0,
      0,   0,   0,  20,   0,   9,  20,   0,   0,   0,   9,   0,   0,   0,   9,   0,
      0,   0,   0,   0,   0,  20,   0,   9,   0,   0,   9,   9,   8,   0,   0,   0,
      9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   8,   0,  20,   0,   0,   0,   0,   0,   0,   9,
      8,   8,   8,   0,   0,   0,   0,   0, 226,   9,   8,   8,   8,   8,   8,   8,
      0,   0,   0,   0,   9,   8,   8,   8,   9,   8,   8,   8,   8,   8,   8,   8,
      0,   0,   0,  20,   0,   0,   0,   0,   0,   0,   9,   0,   0,   9,   8,   8,
      0,   0,   0,   0,  20,   0,   9,   8,   0,   0,   0,   0,   8,   8,   8,   8,
      8,   8,   8,   8,   8,  20,   0,   0,   9,   0,   9,   9,   9,   9,   0,   9,
      0,   0,   0,   8,   0,   0,  20,   8,   9,   8,  20,   0,   0,   0,   0,   0,
    142, 142, 142, 227,   8,   8,   8,   8, 142, 142, 227,   8,   8,   8,   8,   8,
     20,   8,   8,   8,   8,   8,   8,   8,
};

static RE_UINT8 re_script_stage_5[] = {
     1,  1,  1,  2,  2,  2,  2,  1, 35, 35, 41, 41,  3,  3,  1,  3,
     0,  0,  1,  0,  3,  1,  3,  0,  0,  3, 55, 55,  4,  4,  4, 41,
    41,  4,  0,  5,  5,  5,  5,  0,  0,  1,  0,  6,  6,  6,  6,  0,
     7,  7,  1,  7,  7,  1,  7, 41, 41,  7,  8,  8,  0,  8,  8,  0,
     9,  9, 66, 66, 66,  0, 82, 82, 82,  0, 95, 95, 95,  0, 10, 10,
    10, 41, 41, 10,  1, 10,  0, 10,  0, 11, 11, 11, 11,  0,  0, 12,
    12, 12, 12,  0,  0, 13, 13, 13, 13,  0,  0, 14, 14, 14, 14,  0,
    15, 15,  0, 15, 15,  0,  0, 16, 16, 16, 16,  0, 17, 17,  0, 17,
    17,  0, 18, 18,  0, 18, 18,  0, 19, 19,  0, 19, 19,  0,  0, 20,
    20, 20, 20,  0,  0, 21, 21,  0, 21, 21, 22, 22,  0, 22, 22,  0,
    22,  1,  1, 22, 23, 23, 24, 24, 24,  1, 24,  0, 25, 25, 26, 26,
    26,  0,  0, 26, 27, 27, 27,  0, 28, 28, 29, 29, 29,  0, 30, 30,
    30,  1, 30,  0, 42, 42, 42,  0, 43, 43, 43,  1, 44, 44, 45, 45,
    45,  0, 31, 31, 32, 32, 32,  1, 32,  0, 46, 46, 46,  0, 47, 47,
    47,  0, 56, 56, 56,  0, 54, 54, 78, 78, 78,  0,  0, 78, 62, 62,
    62,  0, 67, 67, 67,  0, 93, 93, 68, 68,  0, 68, 69, 69, 41,  1,
     1, 41,  3,  4,  2,  3,  3,  2,  4,  2, 41,  0,  2,  0, 53, 53,
    57, 57, 57,  0,  0, 55, 58, 58,  0, 58, 58,  0, 36, 36,  0, 36,
     1, 36,  0, 33, 33, 33, 33,  0,  0, 41,  1, 33,  1, 34, 34, 34,
    34,  1,  0, 35,  0, 25, 25,  0, 35,  0, 25,  1, 34,  0, 37, 37,
    37,  0, 83, 83, 70, 70, 84, 84, 59, 59, 65, 65, 71, 71, 71,  0,
    72, 72, 73, 73,  0, 73, 85, 85,  0, 85, 77, 77, 77,  0, 79, 79,
    79,  0,  0, 79, 86, 86,  0,  7,  7,  0, 48, 48,  0, 48, 48,  0,
    74, 74, 74,  0, 75, 75, 75,  0, 38, 38, 38,  0, 39, 39, 39,  0,
    49, 49,  0, 49, 60, 60, 40, 40, 50, 50, 51, 51, 52, 52, 52,  0,
     0, 52, 87, 87,  0, 87, 64, 64,  0, 64, 76, 76,  0, 76, 61, 61,
     0, 61, 61,  0, 88, 88, 80, 80,  0, 80, 89, 89, 90, 90, 90,  0,
    91, 91, 91,  0, 94, 94, 92, 92, 63, 63, 63,  0, 81, 81, 81,  0,
    84,  0, 34, 33, 33,  1, 36,  0,
};

/* Script: 7664 bytes. */

RE_UINT32 re_get_script(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 11;
    code = ch ^ (f << 11);
    pos = (RE_UINT32)re_script_stage_1[f] << 4;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_script_stage_2[pos + f] << 3;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_script_stage_3[pos + f] << 3;
    f = code >> 1;
    code ^= f << 1;
    pos = (RE_UINT32)re_script_stage_4[pos + f] << 1;
    value = re_script_stage_5[pos + code];

    return value;
}

/* Word_Break. */

static RE_UINT8 re_word_break_stage_1[] = {
     0,  1,  2,  3,  4,  4,  4,  4,  4,  4,  5,  6,  6,  7,  4,  8,
     9, 10, 11, 12,  4,  4, 13,  4,  4,  4,  4, 14,  4, 15,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    16,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
};

static RE_UINT8 re_word_break_stage_2[] = {
      0,   1,   2,   2,   2,   3,   4,   5,   2,   6,   7,   8,   9,  10,  11,  12,
     13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
     29,  30,   2,   2,  31,  32,  33,  34,  35,   2,   2,   2,  36,  37,  38,  39,
     40,  41,  42,  43,  44,  45,  46,  47,  48,  49,   2,  50,   2,   2,  51,  52,
     53,  54,  55,  56,  14,  14,  14,  14,  14,  57,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  58,  59,  60,  61,  62,  14,  14,  14,
     63,  64,  65,  66,  14,  67,  68,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
      2,   2,   2,   2,   2,   2,   2,   2,   2,  69,   2,   2,  70,  71,  72,  73,
     74,  75,  76,  77,  78,  79,  80,  81,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  82,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  83,  84,   2,   2,  85,  86,  87,  88,  89,  90,
     91,  92,  93,  94,  14,  95,  96,  97,   2,  98,  14,  14,  14,  14,  14,  14,
     99,  14, 100,  14, 101,  14, 102,  14, 103,  14,  14,  14,  14,  14,  14,  14,
    104, 105,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
      2,   2,   2,   2,   2,   2, 106,  14, 107,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
      2,   2,   2,   2,   2,   2,   2,   2, 108,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
      2,   2,   2,   2, 109,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
    110,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14, 111, 112, 113,  14,  14,  14, 114, 115, 116,   2,   2, 117, 118, 119,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
    120,  14, 121, 122,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
};

static RE_UINT8 re_word_break_stage_3[] = {
      0,   1,   0,   0,   2,   3,   4,   5,   6,   7,   7,   8,   6,   7,   7,   9,
     10,   0,   0,   0,   0,  11,  12,  13,   7,   7,  14,   7,   7,   7,  14,   7,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   7,   7,  15,   7,  16,   0,  17,  18,   0,   0,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  20,  21,
     22,  23,   7,   7,  24,   7,   7,   7,   7,   7,   7,   7,   7,   7,  25,   7,
     26,  27,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   0,   6,   7,   7,   7,  14,  28,   6,   7,   7,   7,
      7,  29,  30,  19,  19,  19,  19,  31,  32,   0,   7,   7,   7,   9,  33,   0,
     34,  35,  19,  36,   7,   7,   7,   7,   7,  37,  19,  19,   4,  38,  39,   7,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  40,  41,  42,  43,   4,  44,
      0,  45,  46,   7,   7,   7,  19,  19,  19,  47,   7,   7,   7,   7,   7,   7,
      7,   7,   7,   7,  48,  19,  49,   0,   4,  50,   7,   7,   7,  37,  51,  52,
      7,   7,  48,  53,  54,  55,   0,   0,   7,   7,   7,  56,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     57,   7,   7,   7,   7,   7,   7,  58,  19,  19,  59,   7,  60,   4,   6,   6,
     61,  62,  63,   7,   7,  64,  65,  66,  67,  68,  69,  70,  60,   4,  16,   0,
     61,  71,  63,   7,   7,  64,  72,  73,  74,  75,  76,  77,  78,   4,  79,   0,
     61,  25,  24,   7,   7,  64,  80,  66,  31,  81,  82,   0,  60,   4,   0,   0,
     61,  62,  63,   7,   7,  64,  80,  66,  67,  75,  83,  70,  60,   4,  28,   0,
     84,  85,  86,  87,  88,  85,   7,  89,  90,  91,  92,   0,  78,   4,   0,   0,
     61,  20,  64,   7,   7,  64,  93,  94,  95,  91,  96,  16,  60,   4,   0,   0,
     97,  20,  64,   7,   7,  64,  93,  66,  95,  91,  96,  98,  60,   4,  99,   0,
     97,  20,  64,   7,   7,   7,   7, 100,  95, 101,  69,   0,  60,   4,   0, 102,
     97,   7,  14, 102,   7,   7,  24, 103,  14, 104, 105,  19,   0,   0, 106,   0,
      0,   0,   0,   0,   0,   0, 107,  36,  69, 108,   4, 109,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 107, 110,   0, 111,   4, 109,   0,   0,   0,   0,
     82,   0,   0, 112,   4, 109, 113, 114,   7,   6,   7,   7,   7,  17,  30,  19,
     95, 115,  19,  30,  19,  19,  19, 116, 117,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 118,  19, 108,   4, 109,  83, 119, 120, 111, 121,   0,
    122,  31,   4, 123,   7,   7,   7,   7, 124,   0,   7,   7,   7,   7,   7, 125,
      7,   7,   7,   7,   7,   7,   7,   7,   7,  86,  14,  86,   7,   7,   7,   7,
      7,  86,   7,   7,   7,   7,  86,  14,  86,   7,  14,   7,   7,   7,   7,   7,
      7,   7,  86,   7,   7,   7,   7,   7,   7,   7,   7, 126,   0,   0,   0,   0,
      7,   7,   0,   0,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  17,   0,
      6,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  62,   7,   7,
      6,   7,   7,   9,   7,   7,   7,   7,   7,   7,   7,   7,   7,  85,  82,   0,
      7,  20, 127,   0,   7,   7, 127,   0,   7,   7, 128,   0,   7,  20, 129,   0,
      0,   0,   0,   0,   0,   0, 130,  19,  19,  19, 131, 132,   4, 109,   0,   0,
      0, 133,   4, 109,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   0,
      7,   7,   7,   7,   7, 134,   7,   7,   7,   7,   7,   7,   7,   7, 124,   0,
      7,   7,   7,  17,  19, 131,  19, 131,  78,   4,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  19,  19, 135, 112,   4, 109,   0,   0,   0,   0,
      7,   7, 136, 131,   0,   0,   0,   0,   0,   0, 137, 108,  19,  19,  19,  67,
      4, 109,   4, 109,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    138,   7,   7,   7,   7,   7, 139,  19, 138, 140,   4, 109,   0, 118, 131,   0,
    141,   7,   7,   7,  59, 142,   4, 109,   7,   7,   7,   7,  48,  19, 131,   0,
      7,   7,   7,   7, 139,  19,  19,   0,   4, 143,   4,  50,   7,   7,   7, 124,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 144,  19,  19, 145, 146,   0,
      7,   7,   7,   7,   7,   7,   7,   7,  19,  19,  19,  19, 108,   0,   0, 147,
      7,   7, 124, 124,   7,   7,   7,   7, 124, 124,   7, 148,   7,   7,   7, 124,
      7,   7,   7,   7,   7,   7,  20, 149, 150,  17, 151, 140,   7,  17, 150,  17,
      0, 152,   0, 153, 154, 155,   0, 156, 157,   0, 158,   0, 159, 160,  28, 161,
      0,   0,   7,  17,   0,   0,   0,   0,   0,   0,  19,  19,  19,  19, 135,   0,
    162, 102, 103, 163,  18, 164,   7, 165, 166, 167,   0,   0,   7,   7,   7,   7,
      7,  82,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 168,   7,   7,   7,   7,   7,   7,  16,   0,   0,
      7,   7,   7,   7,   7,  14,   7,   7,   7,   7,   7,  14,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  17, 169, 112,   0,
      7,   7,   7,   7, 124,   0,   7,   7,   7,   7,   7,   7, 124, 161,   0,  69,
      7,   7,  14,   0,  14,  14,  14,  14,  14,  14,  14,  14,  19,  19,  19,  19,
      0,   0,   0,   0,   0, 161,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    170,   0,   0,   0,   0, 122, 171,  88,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 172, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 174,
    166,   7,   7,   7,   7, 124,   6,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      7,  14,   0,   0,   7,   7,   7,   9,   0,   0,   0,   0,   0,   0, 173, 173,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 173, 173, 173, 173, 173, 175,
    173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173,   0,   0,   0,   0,   0,
      7,  17,   0,   0,   0,   0,   0,   0,   0,   0,   7,   7,   7,   7,   7, 124,
      7,  17,   7,   7,   4, 176,   0,   0,   7,   7,   7,   7,   7, 136,  36, 177,
      7,   7,   7,   0,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7, 112,   0,
      0,   0, 161,   7, 102,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      7, 178,  16,   0,   7,  16,   0,   0,   0,   0,   0,   0,   0,   0,   0, 102,
    179, 180,   7,   7,  37,   0,   0,   0,   7,   7,   7,   7,   7,   7, 140,   0,
     27,   7,   7,   7,   7,   7, 139,  19, 116,   0,   4, 109,  19,  19,  27, 181,
      4,  50,   7,   7,  48, 111,   7,   7, 136,  19, 131,   0,   7,   7,   7,  17,
     57,   7,   7,   7,   7,   7,  37,  19, 135, 161,   4, 109,   0,   0,   0,   0,
      7,   7,   7,   7,   7,  59, 108,   0, 180, 182,   4, 109,   0,   0,   0, 183,
      0,   0,   0,   0,   0,   0, 120, 184,  76,   0,   0,   0,   0,   0,   0,   0,
    185, 185, 185,   0,  14,  14,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   7,   7,   7,   7,  37, 186,   4, 109,
      7,   7,   7,   7, 140,   0,   7,   7,  14, 187,   7,   7,   7,   7,   7, 140,
     14,   0, 187, 188,   7,  64,  14, 149, 189,   7,   7,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,  16,   0,   0,   0, 187,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   7, 124,   0,   0,   7,   7,   7,   7,   7,   7,
      7,   7, 102,   7,   7,   7,   7,   7,   7,   0,   0,   0,   0,   0,   7, 140,
     19,  19, 190,   0, 108,   0, 191,   0,   0, 192, 193,   0,   0,   0,  20,   7,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7, 194,
      2,   3,   0, 195,   6,   7,   7,   8,   6,   7,   7,   9, 196, 173, 173, 173,
    173, 173, 173, 197,   7,   7,   7,  14, 102, 102, 102, 198,   0,   0,   0, 199,
      7,  93,   7,   7,  14,   7,   7, 200,   7, 124,   7, 124,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   9,
      0,   0,   0,   0,   0,   0,   0,   0,   7,   7,   7,   7,   7,   7,  17,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 132,
      7,   7,   7,  17,   7,   7,   7,   7,   7,   7,  82,   0,   0,   0,   0,   0,
      7,   7,   7,  14,   0,   0,   7,   7,   7,   9,   0,   0,   0,   0,   0,   0,
      7,   7,   7, 124,   7,   7,   7,   7, 140,   7, 163,   0,   0,   0,   0,   0,
      7,   7,   7, 124,   4, 109,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    124,  64,   7,   7,   7,   7,  25, 201,   7,   7, 124,   0,   0,   0,   0,   0,
      7,   7, 124,   0,   7,   7,   7,  16,   0,   0,   0,   0,   0,   0,   0,   0,
    202, 147,  93,   6,   7,   7, 140,  74,   0,   0,   0,   0,   7,   7,   7,  17,
      7,   7,   7,   7,   7,   7, 124,   0,   7,   7, 124,   0,   7,   7,   9,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,  82,   0,   0,   0,   0,   0,   0,
    141,   7,   7,   7,   7,   7,   7,  19, 108,   0,   0,   0,  78,   4,   0,   0,
    141,   7,   7,   7,   7,   7,  19, 203,   0,   0,   0,   0,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  14,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   9,   0,   0,   0,
      7,   7,   7,   7,   7,  14,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,  82,   0,   0,   0,   0,   0,   0,   0,   0,
    204,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 137, 205, 206, 207,
    208, 131,   0,   0,   0, 209,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0, 210,   0,   0,   0,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  20,   7,   7,   7,   7,   7,
      7,   7,   7,  20, 211, 212,   7, 213,  93,   7,   7,   7,   7,   7,   7,   7,
     25, 214,  20,  20,   7,   7,   7, 215, 149, 102,  64,   7,   7,   7,   7,   7,
      7,   7,   7,   7, 124,   7,   7,   7,  64,   7,   7, 216,   7,   7,   7, 216,
      7,   7,  20,   7,   7,   7,  20,   7,   7,  14,   7,   7,   7,  14,   7,   7,
      7,  64,   7,   7,   7,  64,   7,   7, 216, 217,   4,   4,   4,   4,   4,   4,
    218,   0,   0,   0, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,   0,   0,
};

static RE_UINT8 re_word_break_stage_4[] = {
      0,   0,   1,   2,   0,   3,   0,   4,   5,   5,   6,   0,   7,   8,   8,   8,
      9,  10,   9,   0,   0,  11,  12,  13,   0,  14,  12,   0,   8,   9,  15,  16,
     15,   0,   8,  17,   0,  18,  19,  19,   8,  20,  16,  21,   0,  22,   9,  20,
     23,   8,   8,  23,  24,  19,  25,   8,  26,   0,  27,   0,  28,  19,  19,  29,
     30,  29,   8,  31,  32,   0,   0,  33,  34,   0,  35,  19,  36,  37,  38,   8,
      8,  39,  19,  40,  19,  41,  42,  25,  43,  44,   0,  45,  46,   8,  34,   7,
      8,  47,  48,   0,  43,   8,  19,  15,  49,   0,  50,  19,  19,  51,  51,  52,
     51,   0,  19,   8,  47,  53,  51,  19,  47,  54,  28,   7,   8,  44,  44,   8,
     20,   8,  18,  16,  15,  53,  19,  55,  55,  56,   0,  57,   0,  23,   9,  58,
     20,  59,  15,  42,  34,  57,  55,  52,  60,   0,   7,  18,   0,  54,  25,  61,
     20,   7,  29,  52,  17,   0,   0,  62,  63,   7,   9,  16,  20,  15,  59,  20,
     58,  17,  15,  62,  34,  62,  42,  52,  17,  57,   8,   7,  15,  39,  19,  42,
      0,  30,  62,   7,   0,  12,  59,   0,   9,  39,  42,  56,  16,   8,   8,  26,
     64,  57,  19,  65,  62,   0,  60,  19,  19,  34,  66,   0,  29,  67,  19,  52,
     52,   0,   0,  68,  60,  62,   8,  51,  19,  67,   0,  64,  57,  19,  52,  62,
     42,  55,  28,  67,  62,  19,  69,  52,   8,  15,   9,  17,   9,  28,  47,  67,
     47,   0,  70,   0,   0,  71,  19,   0,   0,  60,  57,  52,  72,   0,  67,   0,
      8,  35,   0,  28,  19,  38,   8,  19,   8,   0,  73,   8,  34,  16,  66,   7,
     34,  19,  38,  46,  74,   0,   0,  19,  75,  75,   8,  18,  16,  20,   8,  16,
      0,  76,  77,   0,   0,  78,  79,  80,   0,  10,  81,  82,   0,  81,  32,  83,
     84,  32,   0,  58,  12,  58,   7,  15,  20,  23,  15,   8,   0,   7,  15,  12,
      0,  16,  58,  35,   0,  26,  85,  86,  87,  88,  89,  89,  90,  89,  89,  90,
     43,   0,   0,  91,  44,   9,  92,  92,  35,   8,  58,   0,   8,  52,  57,   0,
     67,  62,   7,   9,  34,  52,  58,   8,   0,  93,  23,  20,  94,  82,  10,  81,
      0,  95,   4,  96,   8,  97,  98,   0,   0,  99,  89, 100,  16,  17, 101,   0,
      9,  23,  17,  44,  51,  30,  34,  13,  88,   0,  52,  28, 102,  32, 103,  19,
     34,  28,  62,  52,  62,  67,  12,  59,   7,  20,  23,   7,   9,   7,  23,   9,
      9,   8,   8,  54,  13,   0,  32,  32,
};

static RE_UINT8 re_word_break_stage_5[] = {
     0,  0,  0,  0,  0,  0,  2,  3,  3,  1,  0,  0,  0,  0,  0, 10,
     9,  0, 10,  0, 11, 11, 11, 11, 11, 11,  8,  9,  0,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  0,  0,  0,  0, 12,  0,  3,  0,  0,
     0,  0,  7,  0,  0,  5,  0,  0,  0,  7,  0,  8,  7,  7,  0,  0,
     0,  0,  7,  7,  7,  0,  0,  0,  7,  0,  7,  0,  4,  4,  4,  4,
     7,  0,  7,  7,  7,  7,  9,  0,  0,  0,  7,  8,  7,  7,  0,  7,
     7,  7,  0,  4,  4,  4,  7,  7,  0,  7,  0,  0,  0,  9,  0,  0,
     0,  4,  4,  4,  4,  4,  0,  4,  0,  4,  4,  0,  8,  0,  0,  0,
     5,  5,  5,  5,  9,  9,  0,  0,  4,  4,  4,  0,  7,  7,  7,  4,
    11, 11,  0, 11,  9,  0,  7,  7,  4,  7,  7,  7,  0,  7,  4,  4,
     4,  5,  0,  4,  4,  7,  7,  4,  4,  0,  4,  4, 11, 11,  7,  7,
     7,  0,  0,  7,  0,  0,  0,  5,  7,  4,  7,  7,  7,  7,  4,  4,
     4,  7,  0,  0,  9,  0,  7,  0,  4,  4,  7,  4,  7,  4,  4,  4,
     4,  4,  0,  0,  4,  7,  4,  4,  0,  0, 11, 11,  4,  0,  0,  4,
     4,  4,  7,  0,  0,  0,  0,  4,  0,  0,  0,  7,  0,  7,  7,  0,
     0,  4,  0,  0,  7,  4,  0,  0,  0,  0,  4,  4,  0,  0,  4,  7,
     0,  0,  4,  0,  4,  0,  4,  0, 11, 11,  0,  0,  4,  0,  0,  0,
     0,  4,  0,  4, 11, 11,  4,  4,  7,  0,  4,  4,  5,  5,  4,  4,
     7,  4,  7,  0,  4,  4,  4,  7,  7,  7,  4,  0,  0,  7,  0,  7,
     4,  4,  5,  5, 10, 10,  0,  0, 10,  0,  0,  8,  3,  3,  5,  5,
     5,  5,  5,  0, 12,  0,  0,  0,  9,  0,  0,  0,  5,  0,  0,  0,
     0,  0,  5,  5,  0,  6,  6,  6,  6,  6,  0,  0,  0,  4,  4,  6,
     6,  0,  0,  0,  6,  6,  6,  6,  6,  6,  6,  0,  4,  4,  0,  7,
     7,  7,  4,  7,  0,  7,  4,  7,  9,  0,  0,  8,  0, 12, 12, 12,
     9,  8,  0,  0,  7,  0,  0,  5,  0,  0,  8,  9,  0,  0,  6,  6,
     6,  6,  4,  4,  0,  5,  5,  5,  4,  4,  4,  5,  5,  5,  5,  4,
};

/* Word_Break: 3640 bytes. */

RE_UINT32 re_get_word_break(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_word_break_stage_1[f] << 5;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_word_break_stage_2[pos + f] << 4;
    f = code >> 3;
    code ^= f << 3;
    pos = (RE_UINT32)re_word_break_stage_3[pos + f] << 1;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_word_break_stage_4[pos + f] << 2;
    value = re_word_break_stage_5[pos + code];

    return value;
}

/* Grapheme_Cluster_Break. */

static RE_UINT8 re_grapheme_cluster_break_stage_1[] = {
    0, 1, 2, 2, 2, 3, 4, 5, 6, 2, 2, 2, 2, 2, 7, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_grapheme_cluster_break_stage_2[] = {
     0,  1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16,  1, 17,  1,  1,  1, 18, 19, 20, 21, 22, 23, 24,  1,  1,
    25,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 26, 27,  1,  1,
    28,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1, 29,  1, 30, 31, 32, 33, 34, 35, 36, 37,
    38, 39, 40, 34, 35, 36, 37, 38, 39, 40, 34, 35, 36, 37, 38, 39,
    40, 34, 35, 36, 37, 38, 39, 40, 34, 35, 36, 37, 38, 39, 40, 34,
    35, 36, 37, 38, 39, 40, 34, 41,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 42,  1,  1, 43, 44,
     1, 45,  1,  1,  1,  1,  1,  1,  1,  1, 46,  1,  1,  1,  1,  1,
    47,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 48, 49,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    50, 51,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_grapheme_cluster_break_stage_3[] = {
      0,   1,   2,   2,   2,   2,   2,   3,   1,   1,   4,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      5,   5,   5,   5,   5,   5,   5,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   6,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   7,   5,   8,   9,   2,   2,   2,
     10,  11,   2,   2,  12,   5,   2,  13,   2,   2,   2,   2,   2,  14,  15,   2,
      3,  16,   2,   5,  11,   2,   2,   2,   2,   2,  17,  13,   2,   2,  12,  18,
      2,  19,  20,   2,   2,  21,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
     22,   2,   2,  23,  24,  25,  26,   2,  27,   2,   2,  28,  29,  30,  26,   2,
     31,   2,   2,  32,  33,  16,   2,  34,  31,   2,   2,  32,  35,   2,  26,   2,
     27,   2,   2,  36,  29,  37,  26,   2,  38,   2,   2,  39,  40,  30,   2,   2,
     41,   2,   2,  42,  43,  44,  26,   2,  45,   2,   2,  46,  47,  44,  26,   2,
     45,   2,   2,  39,  48,  30,  26,   2,  45,   2,   2,   2,  49,  50,   2,  45,
      2,   2,   2,  51,  52,   2,   2,   2,   2,   2,   2,  53,  54,   2,   2,   2,
      2,  55,   2,  56,   2,   2,   2,  57,  58,  59,   5,  60,  61,   2,   2,   2,
      2,   2,  62,  63,   2,  64,  65,  66,  67,  68,   2,   2,   2,   2,   2,   2,
     69,  69,  69,  69,  69,  69,  70,  70,  70,  70,  71,  72,  72,  72,  72,  72,
      2,   2,   2,   2,   2,  73,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,  74,   2,  74,   2,  26,   2,  26,   2,   2,   2,  75,  76,  77,   2,   2,
     78,   2,   2,   2,   2,   2,   2,   2,   2,   2,  79,   2,   2,   2,   2,   2,
      2,   2,  80,  81,   2,   2,   2,   2,   2,   2,   2,  82,  83,   2,   2,   2,
      2,  84,   2,   2,   2,  85,  86,  87,   2,   2,   2,   2,   2,   2,   2,   2,
     88,   2,   2,  89,  90,   2,  12,  18,  91,   2,  92,   2,   2,   2,  93,  94,
      2,   2,  95,  96,   2,   2,   2,   2,   2,   2,   2,   2,   2,  97,  98,  99,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   5,   5, 100, 101,
    102,   2, 103,   2,   2,   2, 104,   2,   2,   2,   2,   2,   2,   5,   5,  13,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 105, 106,
      2,   2,   2,   2,   2,   2,   2, 105,   2,   2,   2,   2,   2,   2,   5,   5,
      2,   2, 107,   2,   2,   2,   2,   2,   2, 108,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2, 105, 109,   2,   2,   2,   2,   2,   2,   2, 106,
    110,   2, 111,   2,   2,   2,   2,   2, 112,   2,   2, 113, 114,   2,   5, 106,
      2,   2, 115,   2, 116,  94,  69, 117,  22,   2,   2, 118, 119,   2,   2,   2,
      2,   2, 120, 121, 122,   2,   2, 123,   2,   2,   2, 124,  16,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 125,   2,
    126, 127, 128, 129, 128, 130, 128, 126, 127, 128, 129, 128, 130, 128, 126, 127,
    128, 129, 128, 130, 128, 126, 127, 128, 129, 128, 130, 128, 126, 127, 128, 129,
    128, 130, 128, 126, 127, 128, 129, 128, 130, 128, 126, 127, 128, 129, 128, 130,
    128, 126, 127, 128, 129, 128, 130, 128, 126, 127, 128, 129, 128, 130, 128, 126,
    127, 128, 129, 128, 130, 128, 126, 127, 128, 129, 128, 130, 128, 126, 127, 128,
    129, 128, 130, 128, 126, 127, 128, 129, 128, 130, 128, 126, 127, 128, 129, 128,
    130, 128, 126, 127, 128, 129, 128, 130, 128, 126, 127, 128, 129, 128, 130, 128,
    128, 129, 128, 130, 128, 126, 127, 128, 129, 128, 131,  70, 132,  72,  72, 133,
      2, 134,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      5,   2, 100,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,
      2,   2,   2,   2,   2,   2,   2,   2,   2,  42,   2,   2,   2,   2,   2, 135,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 136,
    137,   2,   2, 138,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    139,   2,   2, 140, 100,   2,   2,   2,  91,   2,   2, 141,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2, 142, 143, 144,   2, 145,   2,   2,   2,   2,   2,
      2,   2,   2,   2,  74,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    146,   2,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
      5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   2,
};

static RE_UINT8 re_grapheme_cluster_break_stage_4[] = {
     0,  0,  1,  2,  0,  0,  0,  0,  3,  3,  3,  3,  3,  3,  3,  4,
     3,  3,  3,  5,  6,  6,  6,  6,  7,  6,  8,  3,  9,  6,  6,  6,
     6,  6,  6, 10, 11, 10,  3,  3,  0,  3,  3,  3,  6,  6, 12,  3,
     3,  3,  7,  6, 13,  3,  3,  3,  3, 14,  6, 15,  6, 16, 17,  8,
    18,  3,  3,  3,  3, 14,  6,  6,  6,  3,  3,  3,  3, 14, 10,  6,
     6,  9,  9,  8,  3,  3,  9,  3, 19,  3,  3,  3,  3,  3, 20, 21,
    22,  6, 23, 24,  9,  6,  3,  3, 14,  3,  3,  3, 25,  3,  3,  3,
     3,  3,  3, 26, 22, 27, 28, 29,  3,  7,  3,  3, 30,  3,  3,  3,
     3,  3,  3, 21, 31,  7, 16,  8,  8, 18,  3,  3, 22, 10, 32, 29,
     3,  3,  3, 17,  3, 14,  3,  3, 33,  3,  3,  3,  3,  3,  3, 20,
    34, 35, 36, 29, 37,  3,  3,  3,  3,  3,  3, 14, 23, 38, 17,  8,
     3, 11,  3,  3, 35,  3,  3,  3,  3,  3,  3, 39, 40, 41, 36,  8,
    22, 21, 36, 29,  3,  3, 33,  7, 42, 43, 44, 45, 24,  6, 12,  3,
    46, 47,  6, 12, 24,  6, 10, 13, 46, 48,  6,  8,  3,  3,  8,  3,
     3, 49, 18, 35,  9,  6,  6, 19,  6, 17,  3,  9,  6,  6,  9,  6,
     6,  6,  6, 13,  3, 33,  3,  3,  3,  3, 50, 22, 51,  6, 52, 31,
     3, 35,  8, 14, 21, 28, 44, 53,  9, 13,  3,  3, 20, 52, 44, 54,
     3,  3, 35, 29, 55, 55, 55, 55, 56, 56, 56, 56, 56, 56, 57, 57,
    57, 57, 57, 57,  3,  3,  3,  9, 14, 13,  3,  3,  3, 58,  6, 59,
    44, 40, 22,  6,  6,  3,  3, 18,  3,  3,  7,  8,  3,  3, 18,  3,
    19, 45, 23,  3, 40, 44, 22,  3, 44, 44, 44, 44, 60,  3, 53,  3,
     3,  7, 23,  3,  3, 61,  6, 12, 62, 22,  6, 23, 45,  6,  6, 16,
     6, 60,  3,  3,  3, 51, 19, 23, 40, 60,  3,  3, 63,  3,  3,  3,
    64, 59, 63,  3,  3, 20, 59, 65, 59,  3,  3,  3,  3, 44, 44,  6,
     6, 42,  3,  3, 12,  6,  6,  6, 51,  6, 13, 18, 66,  3,  3,  3,
     6, 12,  3,  3,  3,  3,  3,  6,  3,  3,  4, 67,  3,  3,  0, 68,
     0, 69, 70,  0,  3,  3,  3,  7,  8,  3,  3,  3,  3,  3, 14,  6,
     3,  3, 11,  3, 12,  3,  3,  8, 33, 33,  7,  3, 50, 52,  3,  3,
    53,  3,  3,  3,  3, 44, 44, 44, 44, 13,  3,  3,  3, 14,  6,  8,
     3,  7,  6,  6, 55, 55, 55, 71,  7, 42, 59, 23, 60,  3,  3,  3,
     3,  3,  9, 19, 52, 31,  3,  3,  7,  3,  3, 72,  3,  3, 50,  3,
    17, 73, 74, 75, 50, 24, 34, 29, 76, 77, 77, 77, 77, 77, 77, 76,
    77, 77, 77, 77, 77, 77, 76, 77, 77, 76, 77, 77, 77,  3,  3,  3,
    56, 78, 79, 57, 57, 57, 57,  3,  3,  3,  3, 33,  3,  3, 80,  3,
     3,  3,  3, 18,  9, 11,  3,  6,  3,  3, 12,  7, 81,  3,  3,  3,
     3,  3,  6,  6, 45, 19, 31,  5,  3, 82,  8, 64, 83,  0, 84,  6,
    12,  9,  6,  3,  3,  3, 14,  8,  5,  3,  3,  3,
};

static RE_UINT8 re_grapheme_cluster_break_stage_5[] = {
     3,  3,  3,  3,  3,  3,  2,  3,  3,  1,  3,  3,  0,  0,  0,  0,
     0,  0,  0,  3,  0,  3,  0,  0,  4,  4,  4,  4,  0,  0,  0,  4,
     4,  4,  0,  0,  0,  4,  4,  4,  4,  4,  0,  4,  0,  4,  4,  0,
     4,  4,  4,  0,  4,  0,  0,  0,  0,  0,  4,  4,  4,  3,  0,  4,
     4,  0,  0,  4,  4,  0,  4,  4,  0,  4,  0,  0,  4,  4,  4,  6,
     0,  0,  4,  6,  4,  0,  6,  6,  6,  4,  4,  4,  4,  6,  6,  6,
     6,  4,  6,  6,  0,  4,  6,  6,  4,  0,  4,  6,  4,  0,  0,  6,
     6,  0,  0,  6,  6,  4,  0,  0,  0,  4,  4,  6,  6,  4,  4,  0,
     4,  6,  0,  6,  0,  0,  4,  0,  4,  6,  6,  0,  0,  0,  6,  6,
     6,  0,  6,  6,  0,  6,  6,  6,  6,  0,  4,  4,  4,  0,  6,  4,
     6,  6,  4,  6,  6,  0,  4,  6,  6,  6,  4,  4,  4,  0,  4,  0,
     6,  6,  6,  6,  6,  6,  6,  4,  5,  5,  5,  5,  5,  6,  0,  4,
     5,  0,  0,  0,  0,  4,  0,  4,  0,  0,  0,  6,  4,  6,  4,  4,
     6,  4,  4,  6,  6,  6,  0,  0,  6,  4,  0,  6,  7,  7,  7,  7,
     8,  8,  8,  8,  9,  9,  9,  9,  3,  3,  6,  4,  4,  4,  6,  6,
     6,  0,  0,  0,  0,  6,  4,  6,  4,  6,  4,  6,  4,  4,  6,  0,
     0,  6,  4,  4,  6,  4,  6,  4,  0,  0,  6,  0,  4,  4,  3,  3,
     3,  3,  3,  0,  3,  0,  0,  0,  0,  0,  3,  3,  7,  0,  0,  0,
     4,  6,  0,  0,  4,  5,  5,  4,  4,  5,  0,  5,  5,  0,  4,  4,
    10, 11, 11, 11, 11, 11, 11, 11,  8,  8,  8,  0,  0,  0,  0,  9,
     0,  3,  3,  3,  6,  4,  6,  0,  0,  4,  6,  4,  4,  4,  4,  3,
     3,  3,  3,  4,
};

/* Grapheme_Cluster_Break: 2184 bytes. */

RE_UINT32 re_get_grapheme_cluster_break(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_grapheme_cluster_break_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_grapheme_cluster_break_stage_2[pos + f] << 4;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_grapheme_cluster_break_stage_3[pos + f] << 2;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_grapheme_cluster_break_stage_4[pos + f] << 2;
    value = re_grapheme_cluster_break_stage_5[pos + code];

    return value;
}

/* Sentence_Break. */

static RE_UINT8 re_sentence_break_stage_1[] = {
     0,  1,  2,  3,  4,  5,  5,  5,  5,  6,  7,  5,  5,  8,  9, 10,
    11, 12, 13, 14,  9,  9, 15,  9,  9,  9,  9, 16,  9, 17,  9,  9,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 18, 19,  9,  9,  9, 20,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
    21,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
};

static RE_UINT8 re_sentence_break_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 17, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 33, 33, 36, 33, 37, 33, 33, 38, 39, 40, 33,
    41, 42, 33, 33, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 43, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 44,
    17, 17, 17, 17, 45, 17, 46, 47, 48, 49, 50, 51, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 52, 33, 33, 33, 33, 33, 33, 33, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 33, 17, 53, 54, 17, 55, 56, 57,
    58, 59, 60, 61, 62, 33, 33, 33, 63, 64, 65, 66, 67, 33, 33, 33,
    68, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    17, 17, 17, 69, 70, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    17, 17, 17, 17, 71, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 17, 17, 72, 33, 33, 33, 33, 33,
    73, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    33, 74, 75, 33, 76, 77, 78, 79, 33, 33, 33, 33, 33, 33, 33, 33,
    17, 17, 17, 17, 17, 17, 80, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 81, 82, 33, 33, 33, 33, 33, 33, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 17, 17, 82, 33, 33, 33, 33, 33,
    83, 84, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
};

static RE_UINT16 re_sentence_break_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
      8,  16,  17,  18,  19,  20,  21,  22,  23,  23,  23,  24,  25,  26,  27,  28,
     29,  30,  18,   8,  31,   8,  32,   8,   8,  33,  34,  35,  36,  37,  38,  39,
     40,  41,  42,  43,  41,  41,  44,  45,  46,  47,  48,  41,  41,  49,  50,  51,
     52,  53,  54,  55,  55,  55,  55,  55,  56,  57,  58,  59,  60,  61,  62,  63,
     64,  65,  66,  67,  68,  69,  70,  71,  60,  69,  72,  73,  74,  75,  76,  77,
     78,  79,  80,  71,  81,  82,  83,  84,  81,  85,  86,  87,  88,  89,  90,  91,
     92,  93,  94,  55,  95,  96,  97,  55,  98,  99, 100, 101, 102, 103, 104,  55,
     41, 105, 106, 107, 108,  29, 109, 110,  41,  41,  41,  41,  41,  41,  41,  41,
     41,  41, 111,  41, 112, 113, 114,  41, 115,  41, 116, 117, 118,  41,  41, 119,
     92,  41,  41,  41,  41,  41,  41,  41,  41,  41,  41, 120, 121,  41,  41, 122,
    123, 124, 125, 126,  41, 127, 128, 129, 130,  41,  41, 131,  41, 132,  41, 133,
    134, 135, 136, 137,  41, 138, 139,  55, 140,  41, 141, 142, 143, 144,  55,  55,
    145, 146, 147, 148, 149, 150,  41, 151,  41, 152, 153, 154,  55,  55, 155, 156,
     18,  18,  18,  18,  18,  18,  23, 157,   8,   8,   8,   8, 158,   8,   8,   8,
    159, 160, 161, 162, 160, 163, 164, 165, 166, 167, 168, 169, 170,  55, 171, 172,
    173, 174, 175,  30, 176,  55,  55,  55,  55,  55,  55,  55,  55,  55,  55,  55,
     55, 177,  55,  55,  55,  55,  55,  55,  55,  55,  55,  55,  55, 178,  30, 179,
     55,  55, 180, 181,  55,  55, 182, 183,  55,  55,  55,  55, 184,  55, 185, 186,
     29, 187, 188, 189,   8,   8,   8, 190,  18, 191,  41, 192, 193, 194, 194,  23,
    195, 196,  55,  55,  55,  55,  55,  55, 197, 198,  92,  41, 199,  92,  41, 200,
    201, 202,  41,  41, 203, 204,  55, 205,  41,  41,  41,  41,  41, 133,  55,  55,
     41,  41,  41,  41,  41,  41, 206,  55,  41,  41,  41,  41, 207,  55, 205, 208,
    209, 210,   8, 211, 212,  41,  41, 213, 214, 215,   8, 216, 217, 218,  55, 219,
    220, 221,  41, 222, 223, 146, 224, 225,  50, 226, 227, 134,  56, 228, 229,  55,
     41, 230, 231, 232,  41, 233, 234,  55, 235, 236,  55,  55,  55,  55,  41, 237,
     41,  41,  41,  41,  41, 238, 239, 240,  41, 241,  41, 241,  41,  41, 242,  55,
    243, 244, 245,  41,  41, 246, 247,  41,  41, 248, 205,  41, 249,  41, 250, 251,
    252, 253, 254, 255,  41,  41,  41, 256, 257,   2, 258, 259, 260, 261, 262, 263,
    264, 265, 266,  55,  41,  41,  41, 204,  55,  55,  41, 119,  55,  55,  55, 267,
     55,  55,  55,  55, 134,  41, 268,  55, 261, 205, 269,  55, 270,  41, 271,  55,
     29, 272, 273,  41, 270, 129,  55,  55, 274, 275, 133,  55,  55,  55,  55,  55,
    133, 242,  55,  55,  55,  55,  55,  55, 276, 277, 278, 134,  55,  55,  55,  55,
     41, 133, 133, 279,  55,  55,  55,  55,  41,  41, 280,  55,  55,  55,  55,  55,
    149, 281, 282,  77, 149, 283, 284,  55,  41,  41,  41, 203,  55,  55,  55,  55,
     41,  41,  41, 285,  55,  55,  55,  55,  41, 203,  55,  55,  55,  55,  55,  55,
     41, 286,  55,  55,  55,  55,  55,  55, 287,  55,  55,  55,  55,  55,  55,  55,
     55,  55,  55, 288, 289, 290,  55,  55,  55,  55, 291,  55,  55,  55,  55,  55,
    292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 292, 293, 305,
    295, 306, 307, 308, 299, 309, 310, 311, 312, 313, 314, 187, 315, 316, 317, 318,
     41,  41,  41,  41,  41,  41, 193,  55,  41, 119,  41,  41,  41,  41,  41,  41,
    270,  55,  55,  55,  55,  55,  55,  55, 319, 320, 320, 320,  55,  55,  55,  55,
     23,  23,  23,  23,  23,  23,  23, 321,
};

static RE_UINT8 re_sentence_break_stage_4[] = {
      0,   0,   1,   2,   0,   0,   0,   0,   3,   4,   5,   6,   7,   7,   8,   9,
     10,  11,  11,  11,  11,  11,  12,  13,  14,  15,  15,  15,  15,  15,  16,  13,
      0,  17,   0,   0,   0,   0,   0,   0,  18,   0,  19,  20,   0,  21,  19,   0,
     11,  11,  11,  11,  11,  22,  11,  23,  15,  15,  15,  15,  15,  24,  15,  15,
     25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  26,  26,
     26,  26,  27,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  28,  29,
     30,  31,  32,  33,  28,  31,  34,  28,  25,  31,  29,  31,  32,  26,  35,  34,
     36,  28,  31,  26,  26,  26,  26,  27,  25,  25,  25,  25,  30,  31,  25,  25,
     25,  25,  25,  25,  25,  15,  33,  30,  26,  23,  25,  25,  15,  15,  15,  15,
     15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  37,  15,  15,
     15,  15,  15,  15,  15,  15,  38,  36,  39,  40,  36,  36,  41,   0,   0,   0,
     15,  42,   0,  43,   0,   0,   0,   0,  44,  44,  44,  44,  44,  44,  44,  44,
     44,  44,  44,  44,  25,  45,  46,  39,   0,  47,  22,  48,  32,  11,  11,  11,
     49,  11,  11,  15,  15,  15,  15,  15,  15,  15,  15,  50,  33,  34,  25,  25,
     25,  25,  25,  25,  15,  51,  30,  32,  11,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  15,  15,  15,  15,  52,  44,  53,  25,  25,  25,  25,  25,
     28,  26,  26,  29,  25,  25,  25,  25,  25,  25,   0,   0,  10,  11,  11,  11,
     11,  11,  11,  11,  11,  22,  54,  55,  14,  15,  15,  15,  15,  15,  15,  15,
     15,  15,  56,   0,  57,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  58,
     59,  58,   0,   0,  36,  36,  36,  36,  36,  36,  60,   0,  36,   0,   0,   0,
     61,   0,   0,  62,  44,  44,  63,   9,  36,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  64,  44,  44,  44,  44,  44,   7,   7,  65,  66,  67,  36,  36,  36,
     36,  36,  36,  36,  36,  68,  44,  69,  44,  70,  71,  72,   7,   7,  73,  74,
     75,   0,   0,  76,  77,  36,  36,  36,  36,  36,  36,  36,  44,  44,  44,  44,
     44,  44,  63,  78,  36,  36,  36,  36,  36,  79,  44,  44,  80,   0,   0,   0,
      7,   7,  73,  36,  36,  36,  36,  36,  36,  36,  64,  44,  44,  41,  81,   0,
     36,  36,  36,  36,  36,  79,  82,  44,  44,  83,  83,  84,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  83,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     44,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  79,  85,
     44,  44,  44,  44,  83,  44,  36,  36,  79,  86,   7,   7,  78,  36,  78,  36,
     57,  78,  36,  74,  74,  36,  36,  36,  36,  36,  87,  36,  43,  40,  41,  85,
     44,  88,  88,  89,   0,  90,   0,  91,  79,  92,   7,   7,  41,   0,   0,   0,
     57,  78,  60,  93,  74,  36,  36,  36,  36,  36,  87,  36,  87,  94,  41,  71,
     63,  90,  88,  84,  95,   0,  78,  43,   0,  92,   7,   7,  72,  96,   0,   0,
     57,  78,  36,  91,  91,  36,  36,  36,  36,  36,  87,  36,  87,  78,  41,  85,
     44,  58,  58,  84,  97,   0,   0,   0,  79,  92,   7,   7,   0,   0,   0,   0,
     44,  88,  88,  84,   0,  98,   0,  91,  79,  92,   7,   7,  54,   0,   0,   0,
     99,  78,  60,  40,  87,  41,  94,  87,  93,  97,  60,  40,  36,  36,  41,  98,
     63,  98,  71,  84,  97,  90,   0,   0,   0,  92,   7,   7,   0,   0,   0,   0,
     57,  78,  36,  87,  87,  36,  36,  36,  36,  36,  87,  36,  36,  78,  41, 100,
     44,  71,  71,  84,   0,  59,  41,   0,  98,  78,  36,  87,  87,  36,  36,  36,
     36,  36,  87,  36,  36,  78,  41,  85,  44,  71,  71,  84,   0,  59,   0, 101,
     79,  92,   7,   7,  94,   0,   0,   0,  36,  36,  36,  36,  36,  36,  60, 100,
     44,  71,  71,  89,   0,  90,   0,   0,  79,  92,   7,   7,   0,   0,  40,  36,
     98,  78,  36,  36,  36,  60,  40,  36,  36,  36,  36,  36,  91,  36,  36,  54,
     36,  60, 102,  90,  44, 103,  44,  44,   0,   0,   0,   0,  98,   0,   0,   0,
     78,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  77,  44,  63,   0,
     36,  64,  44,  63,   7,   7, 104,   0,  94,  74,  43,  54,   0,  36,  78,  36,
     78, 105,  40,  78,  77,  44,  58,  80,  36,  43,  44,  84,   7,   7, 104,  41,
     97,   0,   0,   0,   0,   0,  84,   0,   7,   7, 104,   0,   0, 106, 107, 108,
     36,  36,  78,  36,  36,  36,  36,  36,  36,  36,  36,  97,  57,  44,  44,  44,
     44,  71,  36,  83,  44,  44,  57,  44,  44,  44,  44,  44,  44,  44,  44, 109,
      0, 102,   0,   0,   0,   0,   0,   0,  36,  36,  64,  44,  44,  44,  44, 110,
      7,   7, 111,   0,  36,  79,  72,  79,  85,  70,  44,  72,  83,  67,  36,  36,
     79,  44,  44,  82,   7,   7, 112,  84,  11, 113,   0,   0,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  60,  97,  36,  36,  87,  41,  36,  60,  87,  41,
     36,  36,  87,  41,  36,  36,  36,  36,  36,  36,  36,  36,  87,  41,  36,  60,
     87,  41,  36,  36,  36,  60,  36,  36,  36,  36,  36,  36,  87,  41,  36,  36,
     36,  36,  36,  36,  36,  36,  60,  57, 114,   9, 115,   0,   0,   0,   0,   0,
     36,  36,  36,  36,   0,   0,   0,   0,  36,  36,  36,  36,  36,  97,   0,   0,
     36,  36,  36, 116,  36,  36,  36,  36, 117,  36,  36,  36,  36,  36, 118, 119,
     36,  36,  60,  40,  97,   0,   0,   0,  36,  36,  36,  87,  79, 109,   0,   0,
     36,  36,  36,  36,  79, 120,   0,   0,  36,  36,  36,  36,  79,   0,   0,   0,
     36,  36,  36,  87, 121,   0,   0,   0,  36,  36,  36,  36,  36, 122,  44,  44,
     44,  44,  44,  44,  44,  93,   0,  96,   7,   7, 104,   0,   0,   0,   0,   0,
    123,   0, 124, 125,   7,   7, 104,   0,  36,  36,  36,  36,  36,  36,   0,   0,
     36,  36, 126,   0,  36,  36,  36,  36,  36,  36,  36,  36,  36,  41,   0,   0,
     36,  36,  36,  36,  36,  36,  36,  97,  44,  44,  44,   0,  44,  44,  44,   0,
      0,  86,   7,   7,  36,  36,  36,  36,  36,  36,  36,  41,  36,  97,   0,   0,
     36,  36,  36,   0,  44,  44,  44,  44,  67,  36,  84,   0,   7,   7, 104,   0,
     36,  36,  36,  36,  36,  64,  44,   0,  36,  36,  36,  36,  36,  83,  44,  63,
     44,  44,  44,  44,  44,  44,  44,  88,   7,   7, 104,   0,   7,   7, 104,   0,
      0,  93, 127,   0,   0,   0,   0,   0,  44,  67,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  44,  44,  44,  44,  67,  36,   0,   7,   7, 111, 128,
      0,   0,  90,  44,  44,   0,   0,   0, 110,  36,  36,  36,  36,  36,  36,  36,
     83,  44,  63,  40,   7,   7, 104,   0,  36,  79,  44,  44,  44,   0,   0,   0,
     36,  44,  44,  44,  44,  44,   9, 115,   7,   7, 104,  78,   7,   7,  73,  36,
     36,  36,  36,  36,  36,  36,  36, 129,   0,   0,   0,   0,  63,  44,  44,  44,
     44,  44,  67,  77, 130,   0,   0,   0,  44,  63,   0,   0,   0,   0,   0,  44,
     25,  25,  25,  25,  25,  34,  15,  27,  15,  15,  11,  11,  15,  39,  11, 113,
     15,  15,  11,  11,  15,  15,  11,  11,  15,  39,  11, 113,  15,  15, 131, 131,
     15,  15,  11,  11,  15,  15,  15,  39,  15,  15,  11,  11,  15, 132,  11, 133,
     46, 132,  11, 134,  15,  46,  11,   0,  15,  15,  11, 134,  46, 132,  11, 134,
    135, 135, 136, 137, 138, 139, 140, 140,   0, 141, 142, 143,   0,   0, 144, 145,
      0, 146, 145,   0,   0,   0,   0, 147,  61, 148, 149,  61,  54,   0,   0, 150,
      0,   0,   0, 144,  15,  38,  36,  97,   0,   0,   0,   0,  44,  44,  44,  44,
     44,  44,  44,  44, 109,   0,   0,   0,  47, 151, 152, 153,  23, 154,  10, 113,
      0, 155,  48, 156,  11,  38, 157,  33,   0, 158,  39, 159,   0,   0,   0,   0,
    160,  38,  97,   0,   0,   0,   0,   0,   0,   0, 144,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 161,  11,  11,  15,  15,  39,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   4, 162,   0,   0, 140, 140, 140,   5,   0,   0,
      0, 144,   0,   0,   0,   0,   0,   0,   0, 163, 140, 140,   0,   0,   0,   0,
      4, 140, 140, 140, 140, 140, 119,   0,   0,   0,   0,   0,   0,   0, 140,   0,
      0,   0,   0,   0,   0,   0,   0,   5,  11,  11,  11,  22,  15,  15,  15,  15,
     15,  15,  15,  15,  15,  15,  15,  24,  31, 164,  26,  32,  25,  29,  15,  33,
     25,  42, 151, 165,  84,   0,   0,   0,  15,  39,   0,   0,  36,  36,  36,  36,
     36,  41,   0,  93,   0,   0,   0,  90,  36,  36,  36,  36,  36,  60,   0,   0,
     36,  60,  36,  60,  36,  60,  36,  60, 140, 140, 140,   5,   0,   0,   0,   5,
    140, 140,   5, 166,   0,   0,   0,   0, 167,  78, 140, 140,   5, 140, 140, 168,
     78,  36,  79,  44,  78,  41,  36,  97,  36,  36,  36,  36,  36,  60,  59,  78,
     36,  36,  36,  36,  36,  36,  60,  36,   0,  78,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  41,  78,  36,  36,  36,  36,  36,  36,  60,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  60,   0,   0,   0,   0,   0,  36,  36,  36,  36,
     36,  36,  36,   0,   0,   0,   0,   0,  36,  36,  36,  97,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  36, 169,  36,  36,  36, 170,  36,  36,  36,  36,
      7,   7,  73,   0,   0,   0,   0,   0,  25,  25,  25, 171,  63,   0,   0, 172,
     25,  25,  25,  25,  25,  25,   0,   0,  36,  36,  36,  36, 173,   9,   0,   0,
      0,   0,   0,   0,   0,  93,  36,  36, 174,  25,  25,  25,  27,  25,  25,  25,
     25,  25,  25,  25,  15,  15,  26,  30,  25,  25, 175, 176, 177,   0,   0,   0,
     25,  25, 177,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,  36,
    179, 179,  64,  36,  36,  36,  36,  36,  64,  44,   0,   0,   0,   0,   0,   0,
     36,  36,  36,  36,  36, 128,   0,   0,  72,  36,  36,  36,  36,  36,  36,  36,
     44, 109,   0, 128,   7,   7, 104,   0,  44,  44,  44,  44,  72,  36,  93,   0,
     36,  79,  44, 173,  36,  36,  36,  36,  36,  64,  44,  44,  44,   0,   0,   0,
     36,  36,  36,  36,  64,  44,  44,  44, 109,   0, 145,  93,   7,   7, 104,   0,
     36,  36,  83,  44,  44,  63,   0,   0,  64,  36,  36,  84,   7,   7, 104, 180,
     36,  36,  36,  36,  36,  60, 181,   0,  36,  36,  36,  36,  85,  70,  67,  79,
    126,   0,   0,   0,   0,   0,  93,  41,  78,  60,  78,  60,  78,  60,   0,   0,
     36,  60,  36,  60,   0,   0,   0,   0,  64,  44, 182,  84,   7,   7, 104,   0,
     36,   0,   0,   0,  36,  36,  36,  36,  36,  60,  93,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36,   0,  36,  36,  36,  41,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  41,   0,  15,  24,   0,   0, 183,  15,   0, 184,
     36,  36,  87,  36,  36,  60,  36,  43,  91,  87,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  41,   0,   0,   0,   0,   0,   0,   0,  93,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36, 185,  36,  36,  36,  36,  40,  36,  36,  36,
     36,  36,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  36,  36,  36,   0,
     44,  44,  44,  44, 186,   4, 119,   0,  44,  63,   0,   0, 187, 168, 140, 140,
    140, 188, 119,   0,   6, 189, 190, 162, 138,   0,   0,   0,  36,  87,  36,  36,
     36,  36,  36,  36,  36,  36,  36, 191,  56,   0,   5,   6,   0,   0, 192,   9,
     14,  15,  15,  15,  15,  15,  16, 193, 194, 195,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36,  79,  36,  36,  36,  36,  36,  36,  36,  60,
     40,  36,  40,  36,  40,  36,  40,  97,   0,   0,   0,   0,   0,   0, 196,   0,
     36,  36,  36,  78,  36,  36,  36,  36,  36,  60,  36,  36,  36,  36,  60,  91,
     36,  36,  36,  41,  36,  36,  36,  41,   0,   0,   0,   0,   0,   0,   0,  95,
     36,  36,  36,  36,  97,   0,   0,   0,  36,  36,  60,   0,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  36,  41,  36,   0,  36,  36,  78,  41,   0,   0,
     11,  11,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  36,  36,  36,  36,
     36,  41,  87,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  91,  97,  74,
     83,  59,   0,  44,  36,  78,  78,  36,  36,  36,  36,  36,  36,   0,  63,  90,
      0,   0,   0,   0,   0, 128,   0,   0,  36,  36,  36,  36,  60,   0,   0,   0,
     36,  36,  97,   0,   0,   0,   0,   0,  36,  36,  36,  36,  36,  36,  44,  44,
     44, 182, 115,   0,   0,   0,   0,   0,  36,  36,  36,  36,  44,  44,  63, 197,
    145,   0,   0,   0,   0,   0,   0,   0,  60,   0,   0,   0,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  97,   0,  41,   0,   0,   0,   0,   0,   0,   0,
      0,  57,  84,  57, 198,  61, 199,  44,  63,  57,  44,   0,   0,   0,   0,   0,
      0,   0,  98,  84,   0,   0,   0,   0,  98, 109,   0,   0,   0,   0,   0,   0,
     11,  11,  11,  11,  11,  11, 153,  15,  15,  15,  15,  15,  15,  11,  11,  11,
     11,  11,  11, 153,  15, 132,  15,  15,  15,  15,  11,  11,  11,  11,  11,  11,
    153,  15,  15,  15,  15,  15,  15,  48,  47, 200,  10,  48,  11, 153, 201,  14,
     15,  14,  15,  15,  11,  11,  11,  11,  11,  11, 153,  15,  15,  15,  15,  15,
     15,  49,  22,  10,  11,  48,  11, 202,  15,  15,  15,  15,  15,  15,  49,  22,
     11, 155, 161,  11, 202,  15,  15,  15,  15,  15,  15,  11,  11,  11,  11,  11,
     11, 153,  15,  15,  15,  15,  15,  15,  11,  11,  11, 153,  15,  15,  15,  15,
    153,  15,  15,  15,  15,  15,  15,  11,  11,  11,  11,  11,  11, 153,  15,  15,
     15,  15,  15,  15,  11,  11,  11,  11,  15,  39,  11,  11,  11,  11,  11,  11,
    202,  15,  15,  15,  15,  15,  24,  15,  33,  11,  11,  11,  11,  11,  22,  15,
     15,  15,  15,  15,  15, 132,  15,  11,  11,  11,  11,  11,  11, 202,  15,  15,
     15,  15,  15,  24,  15,  33,  11,  11,  15,  15, 132,  15,  11,  11,  11,  11,
     11,  11, 202,  15,  15,  15,  15,  15,  24,  15,  27,  92,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   7,   7,  20,   0,   0,   0,   0,   0,   0,   0,
     61,  61,  61,  61,  61,  61,  61,  61,  44,  44,  44,  44,   0,   0,   0,   0,
};

static RE_UINT8 re_sentence_break_stage_5[] = {
     0,  0,  0,  0,  0,  6,  2,  6,  6,  1,  0,  0,  6, 12, 13,  0,
     0,  0,  0, 13, 13, 13,  0,  0, 14, 14, 11,  0, 10, 10, 10, 10,
    10, 10, 14,  0,  0,  0,  0, 12,  0,  8,  8,  8,  8,  8,  8,  8,
     8,  8,  8, 13,  0, 13,  0,  0,  0,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7, 13,  0,  4,  0,  0,  6,  0,  0,  0,  0,  0,  7, 13,
     0,  5,  0,  0,  0,  7,  0,  0,  8,  8,  8,  0,  8,  8,  8,  7,
     7,  7,  7,  0,  8,  7,  8,  7,  7,  8,  7,  8,  7,  7,  8,  7,
     8,  8,  7,  8,  7,  8,  7,  7,  7,  8,  8,  7,  8,  7,  8,  8,
     7,  8,  8,  8,  7,  7,  8,  8,  8,  7,  7,  7,  8,  7,  7,  9,
     9,  9,  9,  9,  9,  7,  7,  7,  7,  9,  9,  9,  7,  7,  0,  0,
     0,  0,  9,  9,  9,  9,  0,  0,  7,  0,  0,  0,  9,  0,  9,  0,
     3,  3,  3,  3,  9,  0,  8,  7,  0,  0,  7,  7,  0,  0,  8,  0,
     8,  0,  8,  8,  8,  8,  0,  8,  7,  7,  7,  8,  8,  7,  0,  8,
     8,  7,  0,  3,  3,  3,  8,  7,  0,  9,  0,  0, 12, 14, 12,  0,
     0, 12,  0,  0,  0,  3,  3,  3,  3,  3,  0,  3,  0,  3,  3,  0,
     9,  9,  9,  0,  5,  5,  5,  5, 14, 14,  0,  0,  3,  3,  3,  0,
     9,  9,  9,  3, 10, 10,  0, 10, 10,  0,  9,  9,  3,  9,  9,  9,
    12,  9,  3,  3,  3,  5,  0,  3,  3,  9,  9,  3,  3,  0,  3,  3,
     3,  3,  9,  9, 10, 10,  9,  9,  9,  0,  0,  9, 12, 12, 12,  0,
     0,  0,  0,  5,  9,  3,  9,  9,  0,  9,  9,  9,  9,  9,  3,  3,
     3,  9,  0,  0, 14, 12,  9,  0,  3,  3,  9,  3,  9,  3,  3,  3,
     3,  3,  0,  0,  3,  9,  3,  3, 12, 12, 10, 10,  9,  0,  9,  9,
     3,  0,  0,  3,  3,  3,  9,  0,  0,  0,  0,  3,  9,  9,  0,  9,
     0,  0, 10, 10,  0,  0,  0,  9,  0,  9,  9,  0,  0,  3,  0,  0,
     9,  3,  0,  0,  9,  0,  0,  0,  0,  0,  3,  3,  0,  0,  3,  9,
     0,  9,  3,  3,  0,  0,  9,  0,  0,  0,  3,  0,  3,  0,  3,  0,
    10, 10,  0,  0,  0,  9,  0,  9,  0,  3,  0,  3,  0,  3, 13, 13,
    13, 13,  3,  3,  3,  0,  0,  0,  3,  3,  3,  9, 10, 10, 12, 12,
    10, 10,  3,  3,  8,  8,  0,  0,  0,  0, 12,  0, 12,  0,  0,  0,
     9,  0, 12,  9,  6,  9,  9,  9,  9,  9,  9, 13, 13,  0,  0,  0,
     3, 12, 12,  0,  9,  0,  3,  3,  5,  5,  3,  3,  0,  0, 14, 12,
    14, 12,  0,  3,  3,  3,  6,  0,  9,  3,  9,  0, 12, 12, 12, 12,
     0,  0, 12, 12,  9,  9, 12, 12,  9,  9,  3,  0,  0,  8,  0,  8,
     7,  0,  7,  7,  8,  0,  7,  0,  8,  0,  0,  0,  6,  6,  6,  6,
     6,  6,  6,  5,  3,  3,  5,  5,  0,  0,  0, 14, 14,  0,  0,  0,
    13, 13, 13, 13, 11,  0,  0,  0,  4,  4,  5,  5,  5,  5,  5,  6,
     0, 13, 13,  0, 12, 12,  0,  0,  0, 13, 13, 12,  0,  0,  0,  6,
     5,  0,  0,  0,  0,  0,  5,  5,  0, 13, 13,  9,  0,  0,  0,  8,
     0,  0,  7,  8,  8,  8,  7,  7,  0,  8,  0,  0,  8,  0,  8,  0,
     8,  8,  0,  7,  9,  7,  0,  0,  0,  8,  7,  7,  0,  0,  7,  0,
     9,  9,  9,  8,  0,  0,  8,  8, 13, 13, 13,  0,  0,  0, 13, 13,
     8,  7,  7,  8,  7,  8,  7,  3,  0,  0, 12,  9,  6, 14, 12,  0,
     0, 13, 13, 13,  9,  9,  0, 12,  9,  0, 12, 12,  8,  7,  9,  3,
     3,  3,  0,  9,  3,  3,  0, 12,  0,  0,  8,  7,  9,  0,  0,  8,
     7,  8,  7,  0,  8,  7,  0,  0,  0,  0,  7,  9,  9,  9,  3,  9,
     0, 12, 12, 12,  0,  0,  9,  3,  3,  3,  3, 12,  0,  0,  0,  7,
     0,  9,  3,  9,  9,  9, 13, 13, 14, 14,  0, 14,  0, 14, 14,  0,
    13,  0,  0, 13,  0, 14, 12, 12, 14, 13, 13, 13,  9,  0,  0,  5,
     0,  0, 14,  0,  0, 13,  0, 13, 13, 12, 13, 13, 14,  0,  9,  9,
     0,  5,  5,  5,  0,  5, 12, 12,  3,  3,  3,  5,  5,  5,  5,  3,
     0,  8,  8,  0,  7,  7,  0,  7,  8,  0,  7,  7,
};

/* Sentence_Break: 5372 bytes. */

RE_UINT32 re_get_sentence_break(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_sentence_break_stage_1[f] << 4;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_sentence_break_stage_2[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_sentence_break_stage_3[pos + f] << 3;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_sentence_break_stage_4[pos + f] << 2;
    value = re_sentence_break_stage_5[pos + code];

    return value;
}

/* Math. */

static RE_UINT8 re_math_stage_1[] = {
    0, 1, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2,
};

static RE_UINT8 re_math_stage_2[] = {
    0, 1, 1, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_math_stage_3[] = {
     0,  1,  2,  2,  2,  2,  2,  3,  2,  2,  2,  2,  4,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     5,  6,  7,  8,  9,  9, 10, 11,  2,  2,  2, 12, 13,  2,  2, 14,
     2,  2,  9,  9,  9,  9, 15,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2, 16,  2,  2,  2,  2,  2, 17,  2, 18, 19,
     2,  2,  2,  2,  2,  2,  2,  2, 20, 21, 22,  9,  9, 23,  9, 24,
};

static RE_UINT8 re_math_stage_4[] = {
     0,  1,  2,  3,  0,  4,  5,  5,  0,  0,  0,  0,  0,  0,  6,  7,
     8,  0,  0,  0,  9, 10, 11, 12, 13,  0, 14, 15, 16, 17, 18,  0,
    19, 20, 21, 22, 23, 23, 23, 23, 24, 25,  0, 26, 27, 28, 29, 30,
     0, 31, 32, 33, 34,  0, 35, 36,  0,  0, 37, 23,  0, 19, 38,  0,
     0, 39,  0,  0,  0,  0,  0, 40,  1,  3,  3,  0,  0,  0,  0, 41,
    23, 23, 42, 23, 43, 44, 45, 23, 46, 47, 48, 23, 23, 49, 23, 23,
    23, 23, 50, 23,
};

static RE_UINT8 re_math_stage_5[] = {
      0,   0,   0,   0,   0,   8,   0, 112,   0,   0,   0,  64,   0,   0,   0,  80,
      0,  16,   2,   0,   0,   0, 128,   0,   0,   0,  39,   0,   0,   0, 115,   0,
    192,   1,   0,   0,   0,   0,  64,   0,   0,   0,  28,   0,  17,   0,   4,   0,
     30,   0,   0, 124,   0, 124,   0,   0,   0,   0, 255,  31,  98, 248,   0,   0,
    132, 252,  47,  63,  16, 179, 251, 241, 255,  11,   0,   0,   0,   0, 255, 255,
    255, 126, 195, 240, 255, 255, 255,  47,  48,   0, 240, 255, 255, 255, 255, 255,
      0,  15,   0,   0,   3,   0,   0,   0,   0,   0,   0,  16,   0,   0,   0, 248,
    255, 255, 191,   0,   0,   0,   1, 240,   7,   0,   0,   0,   3, 192, 255, 240,
    195, 140,  15,   0, 148,  31,   0, 255,  96,   0,   0,   0,   5,   0,   0,   0,
     15, 224,   0,   0, 255, 215, 255, 255, 159,  31,   0,   0,   0,   2,   0,   0,
    126,   1,   0,   0,   4,  30,   0,   0, 255, 255, 223, 255, 255, 255, 255, 223,
    100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,
     95, 252, 253, 255,  63, 255, 255, 255, 255, 207, 255, 255,
};

/* Math: 498 bytes. */

RE_UINT32 re_get_math(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_math_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_math_stage_2[pos + f] << 4;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_math_stage_3[pos + f] << 2;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_math_stage_4[pos + f] << 5;
    pos += code;
    value = (re_math_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Alphabetic. */

static RE_UINT8 re_alphabetic_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3,
};

static RE_UINT8 re_alphabetic_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 13, 13, 13, 14,
    15, 16, 17, 13, 18, 13, 19, 13, 13, 13, 13, 13, 13, 20, 13, 13,
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 13, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 23,  7, 24, 25, 13, 13, 13, 13, 13, 13, 13, 26,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_alphabetic_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1, 17, 18, 19,  1, 20, 21, 22, 23, 24, 25, 26, 27,  1, 28,
    29, 30, 31, 31, 32, 31, 31, 31, 31, 31, 31, 31, 33, 34, 35, 31,
    36, 37, 31, 31,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1, 38,  1,  1,  1,  1,  1,  1,  1,  1,  1, 39,
     1,  1,  1,  1, 40,  1, 41, 42, 43, 44, 45, 46,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1, 47, 31, 31, 31, 31, 31, 31, 31, 31,
    31,  1, 48, 49,  1, 50, 51, 52, 53, 54, 55, 56, 57, 31, 31, 31,
    58, 59, 60, 61, 62, 31, 31, 31, 63, 31, 31, 31, 31, 31, 31, 31,
     1,  1,  1, 64, 65, 31, 31, 31,  1,  1,  1,  1, 66, 31, 31, 31,
     1,  1, 67, 31, 31, 31, 31, 31, 68, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 69, 70, 71, 72,  1,  1,  1,  1,  1,  1, 73,  1,
     1,  1,  1,  1,  1,  1,  1, 74, 75, 31, 31, 31, 31, 31, 31, 31,
     1,  1, 75, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_alphabetic_stage_4[] = {
      0,   0,   1,   1,   0,   2,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   5,   6,   0,   0,   7,   8,   9,  10,   4,  11,
      4,   4,   4,   4,  12,   4,   4,   4,   4,  13,  14,  15,  16,  17,  18,  19,
     20,   4,  21,  22,   4,   4,  23,  24,  25,   4,  26,   4,   4,  27,  28,  29,
     30,  31,  32,   0,   0,   0,   0,   0,   4,  33,  34,  35,  36,  37,  38,  39,
     40,  41,  42,  43,  44,  45,  46,  47,  36,  45,  48,  49,  50,  51,  52,   0,
     53,  54,  55,  47,  56,  54,  57,  58,  56,  59,  60,  61,  62,  63,  64,  65,
     15,  66,  67,   0,  68,  69,  70,   0,  71,   0,  72,  73,  74,  75,   0,   0,
      4,  76,  25,  77,  78,   4,  79,  80,   4,   4,  81,   4,  82,  83,  84,   4,
     85,   4,  86,   0,  87,   4,   4,  88,  15,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,  89,   1,   4,   4,  90,  91,  92,  92,  93,   4,  94,  95,   0,
      0,   4,   4,  96,   4,  97,   4,  98,  75,  99,  25, 100,   4, 101, 102,   0,
    103,   4, 104, 105,   0, 106,   0,   0,   4, 107, 108,   0,   4, 109,   4, 110,
      4,  98, 111, 112,   0,   0,   0, 113,   4,   4,   4,   4,   4,   4,   0,   0,
    114,   4, 115, 112,   4, 116, 117, 118,   0,   0,   0, 119, 120,   0,   0,   0,
    121, 122, 123,   4, 124,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 125,   4, 102,   4, 126, 104,   4,   4,   4,   4, 127,
      4,  79,   4, 128, 129, 130, 130,   4,   0, 131,   0,   0,   0,   0,   0,   0,
    132, 133,  15,   4, 134,  15,   4, 135, 136, 137,   4,   4, 138,  66,   0,  25,
      4,   4,   4,   4,   4,  98,   0,   0,   4,   4,   4,   4,   4,   4, 139,   0,
      4,   4,   4,   4,  31,   0,  25, 112, 140, 141,   4, 142,  96,   4,   4,  87,
    143, 144,   4,   4, 145, 102,   0, 146, 147,  16,   4,  92,   4,   4,  47, 148,
     28,  97, 149,  75,   4, 150, 131,   0,   4, 129, 151, 152,   4, 104, 153,   0,
    154, 155,   0,   0,   0,   0,   4, 156,   4,   4,   4,   4,   4, 157, 158, 103,
      4, 159,   4, 159,   4,   4, 160,   0, 161, 162, 163,   4,   4,  27, 164,   4,
      4, 112,  25,   4, 165,   4,  16, 166,   0,   0,   0, 167,   4,   4,   4,  75,
      0,   1,   1, 168,   4, 104, 169,   0, 170, 171, 172,   0,   4,   4,   4,  66,
      0,   0,   4,  88,   0,   0,   0,   0,   0,   0,   0,   0,  75,   4, 173,   0,
    104,  25, 156,   0, 112,   4, 174,   0,   4,   4,   4,   4, 112,   0,   0,   0,
    175, 176,  98,   0,   0,   0,   0,   0,  98, 160,   0,   0,   0,   0,   0,   0,
    177,  92,   0,  75,   0,   0,   0,   0,   4,  98,  98, 149,   0,   0,   0,   0,
      4,   4, 124,   0,   0,   0,   0,   0,   4,   4, 178,   0, 144,  32,   0,   0,
      4,   4,   4, 138,   0,   0,   0,   0,   4,   4,   4, 179,   0,   0,   0,   0,
      4, 138,   0,   0,   0,   0,   0,   0,   4,  32,   0,   0,   0,   0,   0,   0,
    180,   0,   0,   0,   0,   0,   0,   0,   4,   4, 181,   4, 182, 183, 184,   4,
    185, 186, 187,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 188, 189, 135,
    181, 181, 126, 126, 190, 190, 191,   0,   4,   4,   4,   4,   4,   4, 129,   0,
      4,  88,   4,   4,   4,   4,   4,   4, 112,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_alphabetic_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,  32,   0,   0,   0,
      0,   0, 223,  60,  64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,
      3, 252, 255, 255, 255,   0, 254, 255, 255, 255, 127,   2, 254, 255, 255, 255,
    255,   0,   0,   0,   0,   0, 255, 191, 182,   0, 255, 255, 255,   7,   7,   0,
      0,   0, 255,   7, 255, 255, 255, 254,   0, 192, 255, 255, 255, 255, 239,  31,
    254, 225,   0, 156,   0,   0, 255, 255,   0, 224, 255, 255, 255, 255,   3,   0,
      0, 252, 255, 255, 255,   7,  48,   4, 255, 255, 255, 252, 255,  31,   0,   0,
    255, 255, 255,   1, 255, 255, 255, 239, 255, 223, 225, 255,  15,   0, 254, 254,
    238, 159, 249, 255, 255, 253, 197, 227, 159,  89, 128, 176,  15,   0,   3,   0,
    238, 135, 249, 255, 255, 253, 109, 195, 135,  25,   2,  94,   0,   0,  63,   0,
    238, 191, 251, 255, 255, 253, 237, 227, 191,  27,   1,   0,  15,   0,   0,   0,
    159,  25, 192, 176,  15,   0,   2,   0, 236, 199,  61, 214,  24, 199, 255, 195,
    199,  29, 129,   0, 238, 223, 253, 255, 255, 253, 239, 227, 223,  29,  96,   3,
    236, 223, 253, 255, 223,  29,  96,  64,  15,   0,   6,   0, 255, 255, 255, 231,
    223,  93, 128,   0,  15,   0,   0, 252, 236, 255, 127, 252, 255, 255, 251,  47,
    127, 128,  95, 255,   0,   0,  12,   0, 255, 255, 255,   7, 127,  32,   0,   0,
    150,  37, 240, 254, 174, 236, 255,  59,  95,  32,   0,  48,   1,   0,   0,   0,
    255, 254, 255, 255, 255,  31, 254, 255,   3, 255, 255, 254, 255, 255, 255,  31,
    255, 255, 127, 249, 231, 193, 255, 255, 127,  64,   0,  48,  63,   0, 255, 255,
    255, 255, 255,  23, 255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,
     61, 255, 127, 255, 255, 255,  61, 255, 255, 255, 255, 135, 255, 255,   0,   0,
    255, 255,  31,   0, 255, 159, 255, 255, 255, 199,   1,   0, 255, 223,  15,   0,
    255, 255,  15,   0, 255, 223,  13,   0, 255, 255, 207, 255, 255,   1, 128,  16,
    255, 255, 255,   0, 255,   7, 255, 255, 255, 255,  63,   0, 255,  15, 255,   1,
    255,  63,  31,   0, 255,  15, 255, 255, 255,   3,   0,   0, 255, 255, 255,  15,
    255, 255, 255, 127, 254, 255,  31,   0, 128,   0,   0,   0, 255, 255, 239, 255,
    239,  15,   0,   0, 255, 195,   0,   0, 191, 255,   3,   0,   0, 224,   0, 252,
    255, 255, 255,  63,   0, 222,   7,   0, 255, 255,  63,  63,  63,  63, 255, 170,
    255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,   0,   0,   2, 128,
      0,   0, 255,  31, 132, 252,  47,  62,  80, 189, 255, 243, 224,  67,   0,   0,
    255,   1,   0,   0,   0,   0, 192, 255, 255, 127, 255, 255,  31, 120,   0,   0,
     63, 128,   0,   0, 255, 255, 127,   0, 127, 127, 127, 127,   0, 128,   0,   0,
    224,   0,   0,   0, 254,   3,  62,  31, 255, 255, 127, 224, 255, 255, 255, 247,
    224, 255, 255, 255, 255,  63, 254, 255, 255, 127,   0,   0, 255,  15,   0,   0,
    255,  31, 255, 255,   0,  12,   0,   0, 255, 127,   0, 128,   0,   0, 128, 255,
    252, 255, 255, 255, 255, 121,   3,   0,   0,   0,   0, 252, 187, 247, 255, 255,
      0,   0, 252,   8, 255, 255,   7,   0, 255, 255, 247, 255, 255,  63,   0,   0,
    255, 255, 127,   4,   5,   0,   0,  56, 126, 126, 126,   0, 127, 127,   0,   0,
    255,   7,   0,   0,  15,   0, 255, 255, 127, 248, 255, 255, 255,  63, 255, 255,
    255, 255, 255,   3, 127,   0, 248, 224, 255, 253, 127,  95, 219, 255, 255, 255,
      0,   0, 248, 255, 255, 255, 252, 255,   0,   0, 255,  15,   0,   0, 223, 255,
    192, 255, 255, 255, 252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183,
    255,  63, 255,  63, 255, 255,   1,   0,  15, 255,  62,   0,  63, 253, 255, 255,
    255, 255, 191, 145, 111, 240, 239, 254,  63,   0,   0,   0,   7,   0,   0,   0,
      3,   0,   0,   0, 255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235,
    239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,
     63, 255, 255, 255, 253, 255, 255, 247, 255, 253, 255, 255, 247,  15,   0,   0,
};

/* Alphabetic: 1737 bytes. */

RE_UINT32 re_get_alphabetic(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_alphabetic_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_alphabetic_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_alphabetic_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_alphabetic_stage_4[pos + f] << 5;
    pos += code;
    value = (re_alphabetic_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Lowercase. */

static RE_UINT8 re_lowercase_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_lowercase_stage_2[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 5,
    6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

static RE_UINT8 re_lowercase_stage_3[] = {
     0,  1,  2,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  4,  5,
     6,  3,  7,  3,  3,  3,  8,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  9,  3,  3,  3,  3,  3,  3,  3,  3,  3, 10,  3, 11,
     3,  3, 12,  3,  3,  3,  3,  3,  3,  3, 13, 14,  3,  3,  3,  3,
};

static RE_UINT8 re_lowercase_stage_4[] = {
     0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
     5, 13, 14, 15, 16, 17, 18, 19,  0,  0, 20, 21, 22, 23, 24, 25,
     0, 26, 15,  5, 27,  5, 28,  5,  5, 29,  0, 30, 31,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 15, 15, 15, 15, 15, 15,  0,  0,
     5,  5,  5,  5, 32,  5,  5,  5, 33, 34, 35, 36, 34, 37, 38, 39,
     0,  0,  0,  0, 40,  0,  0,  0, 41, 42, 43, 26, 44,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 26, 45,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 26, 46, 47,  5,  5,  5, 48, 15, 49,  0,  0,  0,  0,  0,  0,
     0,  0,  5, 50, 51,  0,  0,  0,  0, 52,  5, 53, 54, 55,  0, 56,
     0,  0,  0,  0,  0,  0,  0,  0, 57,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
     0, 58, 59,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    60, 61, 62, 31, 63, 64, 65, 66, 67, 68, 69, 70, 71, 60, 61, 72,
    31, 63, 73, 59, 66, 49, 74, 75, 76, 73, 77, 26, 78, 66, 79,  0,
};

static RE_UINT8 re_lowercase_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4,   0,   0,   0, 128,
    255, 255, 127, 255, 170, 170, 170, 170, 170, 170, 170,  85,  85, 171, 170, 170,
    170, 170, 170, 212,  41,  49,  36,  78,  42,  45,  81, 230,  64,  82,  85, 181,
    170, 170,  41, 170, 170, 170, 250, 147, 133, 170, 255, 255, 255, 255, 255, 255,
    255, 255, 239, 255, 255, 255, 255,   1,   3,   0,   0,   0,  31,   0,   0,   0,
     32,   0,   0,   0,   0,   0, 138,  60,   0,   0,   1,   0,   0, 240, 255, 255,
    255, 127, 227, 170, 170, 170,  47,  25,   0,   0, 255, 255,   2, 168, 170, 170,
     84, 213, 170, 170, 170,   0,   0,   0, 254, 255, 255, 255, 255,   0,   0,   0,
    170, 170, 234, 191, 255,   0,  63,   0, 255,   0, 255,   0,  63,   0, 255,   0,
    255,   0, 255,  63, 255,   0, 223,  64, 220,   0, 207,   0, 255,   0, 220,   0,
      0,   0,  31,   0,   0, 196,   8,   0,   0, 128,  16,  50, 192,  67,   0,   0,
     16,   0,   0,   0, 255,   3,   0,   0, 255, 255, 255, 127,  98,  21, 218,  63,
     26,  80,   0,   0,  63,   0,   0,   0, 170,  42,   0,   0, 170, 170, 170,   0,
    168, 170, 171, 170, 170, 170, 255, 149, 170,  80,   2,   0, 170,   2,   0,   0,
      0,   0,   0,   4, 127,   0, 248,   0,   0, 255, 255, 255, 255, 255,   0,   0,
      0,   0,   0, 252, 255, 255,  15,   0,   0, 192, 223, 255, 252, 255, 255,  15,
      0,   0, 192, 235, 239, 255,   0,   0,   0, 252, 255, 255,  15,   0,   0, 192,
    255, 255, 255,   0,   0,   0, 252, 255, 255,  15,   0,   0, 192, 255, 255, 255,
      0, 192, 255, 255,   0,   0, 192, 255, 252, 255, 255, 247,   3,   0,   0, 240,
    255, 255, 223,  15, 255, 127,  63,   0, 255, 253,   0,   0, 247,  11,   0,   0,
};

/* Lowercase: 689 bytes. */

RE_UINT32 re_get_lowercase(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_lowercase_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_lowercase_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_lowercase_stage_3[pos + f] << 4;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_lowercase_stage_4[pos + f] << 5;
    pos += code;
    value = (re_lowercase_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Uppercase. */

static RE_UINT8 re_uppercase_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_uppercase_stage_2[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 5,
    6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

static RE_UINT8 re_uppercase_stage_3[] = {
     0,  1,  2,  3,  3,  3,  3,  3,  4,  3,  3,  3,  3,  3,  3,  5,
     6,  3,  7,  3,  3,  3,  8,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  9,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 10,
     3,  3, 11,  3,  3,  3,  3,  3,  3,  3, 12, 13,  3,  3,  3,  3,
};

static RE_UINT8 re_uppercase_stage_4[] = {
     0,  0,  1,  0,  0,  0,  2,  0,  3,  4,  5,  6,  7,  8,  9, 10,
     3, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0, 13, 14, 15, 16, 17,
    18, 19,  0,  3, 20,  3, 21,  3,  3, 22, 23,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 18, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     3,  3,  3,  3, 25,  3,  3,  3, 26, 27, 28, 29,  0, 30, 31, 32,
     0,  0,  0,  0,  0,  0,  0,  0, 33, 34, 35, 19, 36,  0,  0,  0,
     0,  0,  0,  0,  0, 37, 19,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    18, 38,  0, 39,  3,  3,  3, 40,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  3, 41, 42,  0,  0,  0,  0, 43,  3, 44, 45, 46,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
    18, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 24, 48, 49, 50,
    51, 60, 61, 54, 55, 51, 62, 63, 64, 65, 37, 38, 54, 66, 67,  0,
};

static RE_UINT8 re_uppercase_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7, 255, 255, 127, 127,  85,  85,  85,  85,
     85,  85,  85, 170, 170,  84,  85,  85,  85,  85,  85,  43, 214, 206, 219, 177,
    213, 210, 174,  17, 144, 164, 170,  74,  85,  85, 210,  85,  85,  85,   5, 108,
    122,  85,   0,   0,   0,   0,  69,   0,  64, 215, 254, 255, 251,  15,   0,   0,
      0, 128,  28,  85,  85,  85, 144, 230, 255, 255, 255, 255, 255, 255,   0,   0,
      1,  84,  85,  85, 171,  42,  85,  85,  85,   0, 254, 255, 255, 255, 127,   0,
     63,   0,   0,   0,  85,  85,  21,  64,   0, 255,   0,  63,   0, 255,   0, 255,
      0,  63,   0, 170,   0, 255,   0,   0,   0,   0,   0,  15,   0,  15,   0,  15,
      0,  31,   0,  15, 132,  56,  39,  62,  80,  61,  15, 192,  32,   0,   0,   0,
      8,   0,   0,   0,   0,   0, 192, 255, 255, 127,   0,   0, 157, 234,  37, 192,
      5,  40,   0,   0,  85,  21,   0,   0,  85,  85,  85,   0,  84,  85,  84,  85,
     85,  85,   0, 106,  85,  40,   1,   0,  85,   1,   0,   0, 255,   0,   0,   0,
    255, 255, 255,   3,   0,   0, 240, 255, 255,  63,   0,   0,   0, 255, 255, 255,
      3,   0,   0, 208, 100, 222,  63,   0,   0,   0, 255, 255, 255,   3,   0,   0,
    176, 231, 223,  31,   0,   0,   0, 123,  95, 252,   1,   0,   0, 240, 255, 255,
      3,   0,   0, 240, 255, 255,  63,   0,   1,   0,   0,   0, 252, 255, 255,   7,
      0,   0,   0, 240, 255, 255,  31,   0, 255,   1,   0,   0,   0,   4,   0,   0,
};

/* Uppercase: 625 bytes. */

RE_UINT32 re_get_uppercase(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_uppercase_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_uppercase_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_uppercase_stage_3[pos + f] << 4;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_uppercase_stage_4[pos + f] << 5;
    pos += code;
    value = (re_uppercase_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Cased. */

static RE_UINT8 re_cased_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_cased_stage_2[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 5,
    6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

static RE_UINT8 re_cased_stage_3[] = {
     0,  1,  2,  3,  3,  3,  3,  3,  4,  3,  3,  3,  3,  3,  5,  6,
     7,  3,  8,  3,  3,  3,  9,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3, 10,  3,  3,  3,  3,  3,  3,  3,  3,  3, 11,  3, 12,
     3,  3, 13,  3,  3,  3,  3,  3,  3,  3, 14, 15,  3,  3,  3,  3,
};

static RE_UINT8 re_cased_stage_4[] = {
     0,  0,  1,  1,  0,  2,  3,  3,  4,  4,  4,  4,  4,  5,  6,  4,
     4,  4,  4,  4,  7,  8,  9, 10,  0,  0, 11, 12, 13, 14,  4, 15,
     4,  4,  4,  4, 16,  4,  4,  4,  4, 17, 18, 19, 20,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  4, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  4,  4,  4,  4,  4,  4,  0,  0,
     4,  4,  4,  4,  4,  4,  4,  4, 22,  4, 23, 24,  4, 25, 26, 27,
     0,  0,  0,  0, 28,  0,  0,  0, 29, 30, 31,  4, 32,  0,  0,  0,
     0,  0,  0,  0,  0, 33,  4, 34,  0,  0,  0,  0,  0,  0,  0,  0,
     4, 35, 36,  4,  4,  4,  4, 37,  4, 21,  0,  0,  0,  0,  0,  0,
     0,  0,  4, 38, 39,  0,  0,  0,  0, 40,  4,  4, 41, 34,  0, 42,
     0,  0,  0,  0,  0,  0,  0,  0, 43,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
     4,  4, 44,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     4,  4, 45,  4, 46, 47, 48,  4, 49, 50, 51,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4, 52, 53,  5, 45, 45, 35, 35, 54, 54, 55,  0,
};

static RE_UINT8 re_cased_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 255, 255, 255, 247, 240, 255, 255, 255, 255, 255, 239, 255,
    255, 255, 255,   1,   3,   0,   0,   0,  31,   0,   0,   0,  32,   0,   0,   0,
      0,   0, 207,  60,  64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,
      3, 252, 255, 255, 255,   0, 254, 255, 255, 255, 127,   0, 254, 255, 255, 255,
    255,   0,   0,   0,  63,   0,   0,   0, 255, 255,  63,  63,  63,  63, 255, 170,
    255, 255, 255,  63, 255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,
      0,   0,  31,   0, 132, 252,  47,  62,  80, 189,  31, 242, 224,  67,   0,   0,
     24,   0,   0,   0,   0,   0, 192, 255, 255,   3,   0,   0, 255, 127, 255, 255,
    255, 255, 255, 127,  31, 120,   0,   0, 255,  63,   0,   0, 255, 255, 255,   0,
    252, 255, 255, 255, 255, 120,   3,   0,   0,   0,   0,   4, 127,   0, 248,   0,
    255, 255,   0,   0, 255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235,
    239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,
     63, 255, 255, 255, 253, 255, 255, 247, 255, 253, 255, 255, 247,  15,   0,   0,
};

/* Cased: 609 bytes. */

RE_UINT32 re_get_cased(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_cased_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_cased_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_cased_stage_3[pos + f] << 4;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_cased_stage_4[pos + f] << 5;
    pos += code;
    value = (re_cased_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Case_Ignorable. */

static RE_UINT8 re_case_ignorable_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_case_ignorable_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  8,  9,  7,  7,  7,  7,  7,  7,  7,  7,  7, 10,
    11, 12, 13,  7,  7,  7,  7,  7,  7,  7, 14,  7,  7,  7,  7,  7,
    15,  7,  7,  7,  7,  7,  7,  7,
};

static RE_UINT8 re_case_ignorable_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1,  1, 17,  1,  1,  1, 18, 19, 20, 21, 22, 23, 24,  1, 25,
    26,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 27, 28, 29,  1,
    30,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    31,  1,  1,  1, 32,  1, 33, 34, 35, 36, 37, 38,  1,  1,  1,  1,
     1,  1,  1, 39,  1,  1, 40, 41,  1, 42,  1,  1,  1,  1,  1,  1,
     1,  1, 43,  1,  1,  1,  1,  1, 44,  1,  1,  1,  1,  1,  1,  1,
     1, 45, 46,  1,  1,  1,  1,  1, 47, 48,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_case_ignorable_stage_4[] = {
      0,   1,   2,   3,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   5,   6,   6,   6,   6,   6,   7,   8,   0,   0,   0,
      0,   0,   0,   0,   9,   0,   0,   0,   0,   0,  10,   0,  11,  12,  13,  14,
     15,   0,  16,  17,   0,   0,  18,  19,  20,   5,  21,   0,   0,  22,   0,  23,
     24,  25,  26,   0,   0,   0,   0,   0,  27,  28,  29,  30,  31,  32,  33,  34,
     35,  32,  36,  37,  35,  32,  38,  34,  31,  39,  40,  34,  41,   0,  42,   0,
      0,  43,  44,  34,   0,  39,  45,  34,   0,   0,  33,  34,   0,   0,  46,   0,
      0,  47,  48,   0,   0,  49,  50,   0,  51,  52,   0,  53,  54,  55,  56,   0,
      0,  57,  58,  59,  60,   0,   0,  32,   0,   0,  61,   0,   0,   0,   0,   0,
     62,  62,  63,  63,   0,  64,  65,   0,  66,   0,  67,   0,   0,  68,   0,   0,
      0,  69,   0,   0,   0,   0,   0,   0,  70,   0,  71,  72,   0,  73,   0,   0,
     74,  75,  41,  76,  77,  78,   0,  79,   0,  80,   0,  81,   0,   0,  82,  83,
      0,  84,   6,  85,  86,   6,   6,  87,   0,   0,   0,   0,   0,  88,  89,  90,
     91,  92,   0,  93,  94,   0,   5,  95,   0,   0,   0,  96,   0,   0,   0,  97,
      0,   0,   0,  98,   0,   0,   0,   6,   0,  99,   0,   0,   0,   0,   0,   0,
    100, 101,   0,   0, 102,   0,   0, 103, 104,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  81, 105,   0,   0, 106,   0,   0,   0, 107,
      6,  77,   0,  17, 108,   0,   0,   0, 109, 110,   0,   0,   0,   0, 111, 112,
      0, 113, 114,   0,  27, 115,  99,   0,   0, 116, 117,  17,   0, 118, 119,   0,
      0,   0,   0,   0,   0,   0,   0, 120,   2,   0,   0,   0,   0, 121,  77,   0,
    122, 123, 124,   0,   0,   0,   0, 125,   1,   2,   3,  17,  43,   0,   0, 126,
      0,   0,   0,   0,   0,   0,   0,  96, 127, 128,   0,   0,   0,   0,   0,   0,
     31, 129, 123,   0,  77, 130,   0,   0,   0,   0,   0, 131, 132, 133,   0,   0,
      0,   0, 134,   0,   0,   0,   0,   0,  31,   6,   6,   6,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   6, 135,
};

static RE_UINT8 re_case_ignorable_stage_5[] = {
      0,   0,   0,   0, 128,  64,   0,   4,   0,   0,   0,  64,   1,   0,   0,   0,
      0, 161, 144,   1,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255,  48,   4,
    176,   0,   0,   0, 248,   3,   0,   0,   0,   0,   0,   2,   0,   0, 254, 255,
    255, 255, 255, 191, 182,   0,   0,   0,   0,   0,  16,   0,  15,   0, 255,   7,
      1, 248, 255, 255,   0,   0,   1,   0,   0,   0, 192, 191, 255,  61,   0,   0,
      0, 128,   2,   0, 255,   7,   0,   0, 192, 255,   1,   0,   0, 248,  63,   4,
      0,   0, 192, 255, 255,  63,   0,   0,   0,   0,   0,  14,   7,   0,   0,   0,
      0,   0,   0,  20, 254,  33, 254,   0,  12,   0,   2,   0,   2,   0,   0,   0,
      0,   0,   0,  16,  30,  32,   0,   0,  12,   0,   0,   0,   6,   0,   0,   0,
    134,  57,   2,   0,   0,   0,  35,   0, 190,  33,   0,   0,   0,   0,   0, 144,
     30,  32,  64,   0,   4,   0,   0,   0,   1,  32,   0,   0,   0,   0,   0, 192,
    193,  61,  96,   0,  64,  48,   0,   0,   0,   4,  92,   0,   0,   0, 242,   7,
    192, 127,   0,   0,   0,   0, 242,  27,  64,  63,   0,   0,   0,   0,   0,   3,
      0,   0, 160,   2,   0,   0, 254, 127, 223, 224, 255, 254, 255, 255, 255,  31,
     64,   0,   0,   0,   0, 224, 253, 102,   0,   0,   0, 195,   1,   0,  30,   0,
    100,  32,   0,  32,   0,   0,   0, 224,   0,   0,  28,   0,   0,   0,  12,   0,
      0,   0, 176,  63,  64, 254, 143,  32,   0,  56,   0,   0,   8,   0,   0,   0,
      0,   2,   0,   0, 135,   1,   4,  14,   0,   0, 128,   1,   0,   0,  64, 127,
    229,  31, 248, 159, 128,   0,   0,   0,  15,   0,   0,   0,   0,   0, 208,  23,
      0, 248,  15,   0,   3,   0,   0,   0,  60,   3,   0,   0,  64, 163,   3,   0,
      0, 240, 207,   0,   0,   0,   0,  63,   0,   0, 247, 255, 253,  33,   0,   0,
      0, 240, 255, 255,   3,   0,   0,   1,   0,   0,   0, 248, 127,   0,   0, 240,
      0,   0,   0, 160,   3, 224,   0, 224,   0, 224,   0,  96,   0, 248,   0,   3,
    144, 124,   0,   0,  31, 252,   2, 128,   0,   0, 255,  31, 255, 255,   1,   0,
      0,   0,   0,  32,   0, 128,   3,   0,   0, 128,   0, 128,   0, 128,   0,   0,
     32,   0,   0,   0,   0, 252,  62,   8,   0,   0,   0, 126,   0,   0,   0, 112,
      0,   0,  32,   0,   0,  16,   0,   0,   0, 128,   7, 176,   0,   0,   3,   0,
      0,   7,   0,   0,  68,   8,   0,   0,  96,   0,   0,   0,  16,   0,   0,   0,
    255, 255,   3,   0, 192,  63,   0,   0, 128, 255,   3,   0,   0,   0, 200,  19,
      0, 126, 102,   0,   8,  16,   0,   0,   0,   0, 157, 193,   2,   0,   0,  32,
     32,  33,   0,   0,   0,   0, 252, 255, 255, 255,   8,   0, 127,   0,   0,   0,
      0,   0,  36,   0,   0,   0,   0, 128,   8,   0,   0,  14, 110, 240,   0,   0,
      0,   0,   0, 135,   0,   0,   0, 255,   0,   0, 120,  38, 128,   3, 248, 255,
    231,  15,   0,   0,   0,  60,   0,   0,  28,   0,   0,   0, 255, 255,   0,   0,
};

/* Case_Ignorable: 1188 bytes. */

RE_UINT32 re_get_case_ignorable(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_case_ignorable_stage_1[f] << 3;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_case_ignorable_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_case_ignorable_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_case_ignorable_stage_4[pos + f] << 5;
    pos += code;
    value = (re_case_ignorable_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Changes_When_Lowercased. */

static RE_UINT8 re_changes_when_lowercased_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3,
};

static RE_UINT8 re_changes_when_lowercased_stage_2[] = {
    0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7,
    8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_changes_when_lowercased_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  8,  9,
     6, 10,  6,  6, 11,  6,  6,  6,  6,  6,  6,  6, 12,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 13, 14,  6,  6,  6,  6,  6,  6,  6, 15,
     6,  6,  6,  6, 16,  6,  6,  6,
};

static RE_UINT8 re_changes_when_lowercased_stage_4[] = {
     0,  0,  1,  0,  0,  0,  2,  0,  3,  4,  5,  6,  7,  8,  9, 10,
     3, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0, 13, 14, 15, 16, 17,
    18, 19,  0,  3, 20,  3, 21,  3,  3, 22, 23,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18, 24,  0,
     3,  3,  3,  3, 25,  3,  3,  3, 26, 27, 28, 29, 27, 30, 31, 32,
     0, 33,  0, 19, 34,  0,  0,  0,  0,  0,  0,  0,  0, 35, 19,  0,
    18, 36,  0, 37,  3,  3,  3, 38,  0,  0,  3, 39, 40,  0,  0,  0,
     0, 41,  3, 42, 43, 44,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
    18, 45,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_lowercased_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7, 255, 255, 127, 127,  85,  85,  85,  85,
     85,  85,  85, 170, 170,  84,  85,  85,  85,  85,  85,  43, 214, 206, 219, 177,
    213, 210, 174,  17, 176, 173, 170,  74,  85,  85, 214,  85,  85,  85,   5, 108,
    122,  85,   0,   0,   0,   0,  69,   0,  64, 215, 254, 255, 251,  15,   0,   0,
      0, 128,   0,  85,  85,  85, 144, 230, 255, 255, 255, 255, 255, 255,   0,   0,
      1,  84,  85,  85, 171,  42,  85,  85,  85,   0, 254, 255, 255, 255, 127,   0,
     63,   0,   0,   0,  85,  85,  21,  64,   0, 255,   0,  63,   0, 255,   0, 255,
      0,  63,   0, 170,   0, 255,   0,   0,   0, 255,   0,  31,   0,  31,   0,  15,
      0,  31,   0,  31,  64,  12,   4,   0,   8,   0,   0,   0,   0,   0, 192, 255,
    255, 127,   0,   0, 157, 234,  37, 192,   5,  40,   0,   0,  85,  21,   0,   0,
     85,  85,  85,   0,  84,  85,  84,  85,  85,  85,   0, 106,  85,  40,   1,   0,
     85,   1,   0,   0, 255,   0,   0,   0,
};

/* Changes_When_Lowercased: 490 bytes. */

RE_UINT32 re_get_changes_when_lowercased(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_changes_when_lowercased_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_changes_when_lowercased_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_changes_when_lowercased_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_changes_when_lowercased_stage_4[pos + f] << 5;
    pos += code;
    value = (re_changes_when_lowercased_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Changes_When_Uppercased. */

static RE_UINT8 re_changes_when_uppercased_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3,
};

static RE_UINT8 re_changes_when_uppercased_stage_2[] = {
    0, 1, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6,
    7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_changes_when_uppercased_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  7,  8,  9,  6, 10,  6,  6, 11,  6,  6,  6,
     6,  6,  6,  6, 12, 13,  6,  6,  6,  6,  6,  6,  6,  6, 14, 15,
     6,  6,  6, 16,  6,  6,  6, 17,  6,  6,  6,  6, 18,  6,  6,  6,
};

static RE_UINT8 re_changes_when_uppercased_stage_4[] = {
     0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
     5, 13, 14, 15, 16,  0,  0,  0,  0,  0, 17, 18, 19, 20, 21, 22,
     0, 23, 24,  5, 25,  5, 26,  5,  5, 27,  0, 28, 29,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,  0,  0,  0,  0,
     5,  5,  5,  5, 31,  5,  5,  5, 32, 33, 34, 35, 24, 36, 37, 38,
     0,  0, 39, 23, 40,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23, 41,
     0, 23, 42, 43,  5,  5,  5, 44, 24, 45,  0,  0,  0,  0,  0,  0,
     0,  0,  5, 46, 47,  0,  0,  0,  0, 48,  5, 49, 50, 51,  0,  0,
    52,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
     0, 53, 54,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_uppercased_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   0,  32,   0,   0,   0,   0, 128,
    255, 255, 127, 255, 170, 170, 170, 170, 170, 170, 170,  84,  85, 171, 170, 170,
    170, 170, 170, 212,  41,  17,  36,  70,  42,  33,  81, 162,  96,  91,  85, 181,
    170, 170,  45, 170, 168, 170,  10, 144, 133, 170, 223,  10,  41, 139,  38,  32,
      9,  31,   4,   0,  32,   0,   0,   0,   0,   0, 138,  56,   0,   0,   1,   0,
      0, 240, 255, 255, 255, 127, 227, 170, 170, 170,  39,   9,   0,   0, 255, 255,
    255, 255, 255, 255,   2, 168, 170, 170,  84, 213, 170, 170, 170,   0,   0,   0,
    254, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,  34, 170, 170, 234,  15,
    255,   0,  63,   0, 255,   0, 255,   0,  63,   0, 255,   0, 255,   0, 255,  63,
    255, 255, 223,  80, 220,  16, 207,   0, 255,   0, 220,  16,   0,  64,   0,   0,
     16,   0,   0,   0, 255,   3,   0,   0, 255, 255, 255, 127,  98,  21,  72,   0,
     10,  80,   0,   0,  63,   0,   0,   0, 170,  42,   0,   0, 170, 170, 170,   0,
    168, 170, 168, 170, 170, 170,   0, 148, 170,  16,   2,   0, 170,   2,   0,   0,
    127,   0, 248,   0,   0, 255, 255, 255, 255, 255,   0,   0,
};

/* Changes_When_Uppercased: 534 bytes. */

RE_UINT32 re_get_changes_when_uppercased(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_changes_when_uppercased_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_changes_when_uppercased_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_changes_when_uppercased_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_changes_when_uppercased_stage_4[pos + f] << 5;
    pos += code;
    value = (re_changes_when_uppercased_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Changes_When_Titlecased. */

static RE_UINT8 re_changes_when_titlecased_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3,
};

static RE_UINT8 re_changes_when_titlecased_stage_2[] = {
    0, 1, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6,
    7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_changes_when_titlecased_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  7,  8,  9,  6, 10,  6,  6, 11,  6,  6,  6,
     6,  6,  6,  6, 12, 13,  6,  6,  6,  6,  6,  6,  6,  6, 14, 15,
     6,  6,  6, 16,  6,  6,  6, 17,  6,  6,  6,  6, 18,  6,  6,  6,
};

static RE_UINT8 re_changes_when_titlecased_stage_4[] = {
     0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
     5, 13, 14, 15, 16,  0,  0,  0,  0,  0, 17, 18, 19, 20, 21, 22,
     0, 23, 24,  5, 25,  5, 26,  5,  5, 27,  0, 28, 29,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,  0,  0,  0,  0,
     5,  5,  5,  5, 31,  5,  5,  5, 32, 33, 34, 35, 33, 36, 37, 38,
     0,  0, 39, 23, 40,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23, 41,
     0, 23, 42, 43,  5,  5,  5, 44, 24, 45,  0,  0,  0,  0,  0,  0,
     0,  0,  5, 46, 47,  0,  0,  0,  0, 48,  5, 49, 50, 51,  0,  0,
    52,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
     0, 53, 54,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_titlecased_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   0,  32,   0,   0,   0,   0, 128,
    255, 255, 127, 255, 170, 170, 170, 170, 170, 170, 170,  84,  85, 171, 170, 170,
    170, 170, 170, 212,  41,  17,  36,  70,  42,  33,  81, 162, 208,  86,  85, 181,
    170, 170,  43, 170, 168, 170,  10, 144, 133, 170, 223,  10,  41, 139,  38,  32,
      9,  31,   4,   0,  32,   0,   0,   0,   0,   0, 138,  56,   0,   0,   1,   0,
      0, 240, 255, 255, 255, 127, 227, 170, 170, 170,  39,   9,   0,   0, 255, 255,
    255, 255, 255, 255,   2, 168, 170, 170,  84, 213, 170, 170, 170,   0,   0,   0,
    254, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,  34, 170, 170, 234,  15,
    255,   0,  63,   0, 255,   0, 255,   0,  63,   0, 255,   0, 255,   0, 255,  63,
    255,   0, 223,  64, 220,   0, 207,   0, 255,   0, 220,   0,   0,  64,   0,   0,
     16,   0,   0,   0, 255,   3,   0,   0, 255, 255, 255, 127,  98,  21,  72,   0,
     10,  80,   0,   0,  63,   0,   0,   0, 170,  42,   0,   0, 170, 170, 170,   0,
    168, 170, 168, 170, 170, 170,   0, 148, 170,  16,   2,   0, 170,   2,   0,   0,
    127,   0, 248,   0,   0, 255, 255, 255, 255, 255,   0,   0,
};

/* Changes_When_Titlecased: 534 bytes. */

RE_UINT32 re_get_changes_when_titlecased(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_changes_when_titlecased_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_changes_when_titlecased_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_changes_when_titlecased_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_changes_when_titlecased_stage_4[pos + f] << 5;
    pos += code;
    value = (re_changes_when_titlecased_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Changes_When_Casefolded. */

static RE_UINT8 re_changes_when_casefolded_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3,
};

static RE_UINT8 re_changes_when_casefolded_stage_2[] = {
    0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7,
    8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_changes_when_casefolded_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  8,  9,
     6, 10,  6,  6, 11,  6,  6,  6,  6,  6,  6,  6, 12,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 13, 14,  6,  6,  6, 15,  6,  6,  6, 16,
     6,  6,  6,  6, 17,  6,  6,  6,
};

static RE_UINT8 re_changes_when_casefolded_stage_4[] = {
     0,  0,  1,  0,  0,  2,  3,  0,  4,  5,  6,  7,  8,  9, 10, 11,
     4, 12, 13,  0,  0,  0,  0,  0,  0,  0, 14, 15, 16, 17, 18, 19,
    20, 21,  0,  4, 22,  4, 23,  4,  4, 24, 25,  0, 26,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 20, 27,  0,
     4,  4,  4,  4, 28,  4,  4,  4, 29, 30, 31, 32, 20, 33, 34, 35,
     0, 36,  0, 21, 37,  0,  0,  0,  0,  0,  0,  0,  0, 38, 21,  0,
    20, 39,  0, 40,  4,  4,  4, 41,  0,  0,  4, 42, 43,  0,  0,  0,
     0, 44,  4, 45, 46, 47,  0,  0, 48,  0,  0,  0,  0,  0,  0,  0,
     0,  1,  0,  0,  0,  0,  0,  0, 20, 49,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_casefolded_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   0,  32,   0, 255, 255, 127, 255,
     85,  85,  85,  85,  85,  85,  85, 170, 170,  86,  85,  85,  85,  85,  85, 171,
    214, 206, 219, 177, 213, 210, 174,  17, 176, 173, 170,  74,  85,  85, 214,  85,
     85,  85,   5, 108, 122,  85,   0,   0,  32,   0,   0,   0,   0,   0,  69,   0,
     64, 215, 254, 255, 251,  15,   0,   0,   4, 128,  99,  85,  85,  85, 179, 230,
    255, 255, 255, 255, 255, 255,   0,   0,   1,  84,  85,  85, 171,  42,  85,  85,
     85,   0, 254, 255, 255, 255, 127,   0, 128,   0,   0,   0,  63,   0,   0,   0,
     85,  85,  21,  76,   0, 255,   0,  63,   0, 255,   0, 255,   0,  63,   0, 170,
      0, 255,   0,   0, 255, 255, 156,  31, 156,  31,   0,  15,   0,  31, 156,  31,
     64,  12,   4,   0,   8,   0,   0,   0,   0,   0, 192, 255, 255, 127,   0,   0,
    157, 234,  37, 192,   5,  40,   0,   0,  85,  21,   0,   0,  85,  85,  85,   0,
     84,  85,  84,  85,  85,  85,   0, 106,  85,  40,   1,   0,  85,   1,   0,   0,
    127,   0, 248,   0, 255,   0,   0,   0,
};

/* Changes_When_Casefolded: 514 bytes. */

RE_UINT32 re_get_changes_when_casefolded(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_changes_when_casefolded_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_changes_when_casefolded_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_changes_when_casefolded_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_changes_when_casefolded_stage_4[pos + f] << 5;
    pos += code;
    value = (re_changes_when_casefolded_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Changes_When_Casemapped. */

static RE_UINT8 re_changes_when_casemapped_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3,
};

static RE_UINT8 re_changes_when_casemapped_stage_2[] = {
    0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7,
    8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_changes_when_casemapped_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  8,  9, 10,
     6, 11,  6,  6, 12,  6,  6,  6,  6,  6,  6,  6, 13, 14,  6,  6,
     6,  6,  6,  6,  6,  6, 15, 16,  6,  6,  6, 17,  6,  6,  6, 18,
     6,  6,  6,  6, 19,  6,  6,  6,
};

static RE_UINT8 re_changes_when_casemapped_stage_4[] = {
     0,  0,  1,  1,  0,  2,  3,  3,  4,  5,  4,  4,  6,  7,  8,  4,
     4,  9, 10, 11, 12,  0,  0,  0,  0,  0, 13, 14, 15, 16, 17, 18,
     4,  4,  4,  4, 19,  4,  4,  4,  4, 20, 21, 22, 23,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4, 24,  0,
     0,  0,  0, 25,  0,  0,  0,  0,  4,  4,  4,  4, 26,  4,  4,  4,
    27,  4, 28, 29,  4, 30, 31, 32,  0, 33, 34,  4, 35,  0,  0,  0,
     0,  0,  0,  0,  0, 36,  4, 37,  4, 38, 39, 40,  4,  4,  4, 41,
     4, 24,  0,  0,  0,  0,  0,  0,  0,  0,  4, 42, 43,  0,  0,  0,
     0, 44,  4, 45, 46, 37,  0,  0, 47,  0,  0,  0,  0,  0,  0,  0,
     0,  1,  1,  0,  0,  0,  0,  0,  4,  4, 48,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_casemapped_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   0,  32,   0, 255, 255, 127, 255,
    255, 255, 255, 255, 255, 255, 255, 254, 255, 223, 255, 247, 255, 243, 255, 179,
    240, 255, 255, 255, 253, 255,  15, 252, 255, 255, 223,  10,  41, 139,  38,  32,
      9,  31,   4,   0,  32,   0,   0,   0,   0,   0, 207,  56,  64, 215, 255, 255,
    251, 255, 255, 255, 255, 255, 227, 255, 255, 255, 183, 239,   3, 252, 255, 255,
    255,   0, 254, 255, 255, 255, 127,   0, 254, 255, 255, 255, 255,   0,   0,   0,
     63,   0,   0,   0,   0,   0,   0,  34, 255, 255, 255,  79, 255, 255,  63,  63,
     63,  63, 255, 170, 255, 255, 255,  63, 255, 255, 223,  95, 220,  31, 207,  15,
    255,  31, 220,  31,  64,  12,   4,   0,   0,  64,   0,   0,  24,   0,   0,   0,
      0,   0, 192, 255, 255,   3,   0,   0, 255, 127, 255, 255, 255, 255, 255, 127,
    255, 255, 109, 192,  15, 120,   0,   0, 255,  63,   0,   0, 255, 255, 255,   0,
    252, 255, 252, 255, 255, 255,   0, 254, 255,  56,   3,   0, 127,   0, 248,   0,
    255, 255,   0,   0,
};

/* Changes_When_Casemapped: 526 bytes. */

RE_UINT32 re_get_changes_when_casemapped(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_changes_when_casemapped_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_changes_when_casemapped_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_changes_when_casemapped_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_changes_when_casemapped_stage_4[pos + f] << 5;
    pos += code;
    value = (re_changes_when_casemapped_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* ID_Start. */

static RE_UINT8 re_id_start_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3,
};

static RE_UINT8 re_id_start_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 13, 13, 13, 14,
    15, 16, 17, 13, 18, 13, 19, 13, 13, 13, 13, 13, 13, 20, 13, 13,
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 13, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 23,  7, 24, 25, 13, 13, 13, 13, 13, 13, 13, 26,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_id_start_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1, 17, 18, 19,  1, 20, 21, 22, 23, 24, 25, 26, 27,  1, 28,
    29, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33, 31, 31,
    34, 35, 31, 31,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1, 36,  1,  1,  1,  1,  1,  1,  1,  1,  1, 37,
     1,  1,  1,  1, 38,  1, 39, 40, 41, 42, 43, 44,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1, 45, 31, 31, 31, 31, 31, 31, 31, 31,
    31,  1, 46, 47,  1, 48, 49, 50, 51, 52, 53, 54, 55, 31, 31, 31,
    56, 57, 58, 59, 60, 31, 31, 31, 61, 31, 31, 31, 31, 31, 31, 31,
     1,  1,  1, 62, 63, 31, 31, 31,  1,  1,  1,  1, 64, 31, 31, 31,
     1,  1, 65, 31, 31, 31, 31, 31, 66, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 67, 68, 69, 70,  1,  1,  1,  1,  1,  1, 71,  1,
     1,  1,  1,  1,  1,  1,  1, 72, 73, 31, 31, 31, 31, 31, 31, 31,
     1,  1, 73, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_id_start_stage_4[] = {
      0,   0,   1,   1,   0,   2,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   5,   6,   0,   0,   0,   7,   8,   9,   4,  10,
      4,   4,   4,   4,  11,   4,   4,   4,   4,  12,  13,  14,  15,   0,  16,  17,
      0,   4,  18,  19,   4,   4,  20,  21,  22,  23,  24,   4,   4,  25,  26,  27,
     28,  29,  30,   0,   0,   0,   0,   0,  31,  32,  33,  34,  35,  36,  37,  38,
     39,  40,  41,  42,  43,  44,  45,  46,  35,  44,  47,  48,  49,  50,  45,   0,
     51,  52,  53,  46,  51,  52,  54,  55,  51,  56,  57,  58,  59,  60,  61,   0,
     14,  62,  61,   0,  63,  64,  65,   0,  66,   0,  67,  68,  69,   0,   0,   0,
      4,  70,  71,  72,  73,   4,  74,  75,   4,   4,  76,   4,  77,  78,  79,   4,
     80,   4,  81,   0,  23,   4,   4,  82,  14,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,  83,   1,   4,   4,  84,  85,  86,  86,  87,   4,  88,  89,   0,
      0,   4,   4,  90,   4,  91,   4,  92,  93,   0,  16,  94,   4,  95,  96,   0,
     97,   4,  82,   0,   0,  98,   0,   0,  99,  88, 100,   0, 101, 102,   4, 103,
      4, 104, 105, 106,   0,   0,   0, 107,   4,   4,   4,   4,   4,   4,   0,   0,
    108,   4, 109, 106,   4, 110, 111, 112,   0,   0,   0, 113, 114,   0,   0,   0,
    115, 116, 117,   4, 118,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      4, 119, 120,   4,   4,   4,   4, 121,   4,  74,   4, 122,  97, 123, 123,   0,
    124, 125,  14,   4, 126,  14,   4, 127,  99, 128,   4,   4, 129,  81,   0,  16,
      4,   4,   4,   4,   4,  92,   0,   0,   4,   4,   4,   4,   4,   4,  95,   0,
      4,   4,   4,   4,  68,   0,  16, 106, 130, 131,   4, 132,  90,   4,   4,  23,
    133, 134,   4,   4, 135, 136,   0, 137, 138, 139,   4,  88, 134,  88,   0, 140,
     26,  74,  61,  93,  31, 141, 142,   0,   4, 118, 143, 144,   4, 145, 146,   0,
    147, 148,   0,   0,   0,   0,   4, 139,   4,   4,   4,   4,   4, 149, 150, 151,
      4, 152,   4, 152,   4,   4, 153,   0, 154, 155, 156,   4,   4,  86, 157,   4,
      4, 106,  16,   4, 158,   4,  15, 159,   0,   0,   0, 160,   4,   4,   4,  93,
      0,   1,   1, 161,   4, 120, 162,   0, 163, 164, 165,   0,   4,   4,   4,  81,
      0,   0,   4,  82,   0,   0,   0,   0,   0,   0,   0,   0,  93,   4, 166,   0,
    120,  16,  18,   0, 106,   4, 167,   0,   4,   4,   4,   4, 106,   0,   0,   0,
    168, 169,  92,   0,   0,   0,   0,   0,  92, 153,   0,   0,   0,   0,   0,   0,
    170,  88,   0,  93,   0,   0,   0,   0,   4,  92,  92, 141,   0,   0,   0,   0,
      4,   4, 118,   0,   0,   0,   0,   0, 101,  90,   0,   0, 101,  23,   0,   0,
      4,   4,   4, 129,   0,   0,   0,   0,   4,   4,   4, 139,   0,   0,   0,   0,
      4, 129,   0,   0,   0,   0,   0,   0,   4,  30,   0,   0,   0,   0,   0,   0,
     46,   0,   0,   0,   0,   0,   0,   0,   4,   4, 171,   4, 172, 173, 174,   4,
    175, 176, 177,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 178, 179, 127,
    171, 171, 119, 119, 180, 180, 143,   0,   4,   4,   4,   4,   4,   4,  97,   0,
      4,  82,   4,   4,   4,   4,   4,   4, 106,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_id_start_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,   0,   0, 223,  60,
     64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,   3, 252, 255, 255,
    255,   0, 254, 255, 255, 255, 127,   2, 254, 255, 255, 255, 255,   0,   0,   0,
      0,   0, 255, 255, 255,   7,   7,   0, 255,   7,   0,   0,   0, 192, 254, 255,
    255, 255,  47,   0,  96, 192,   0, 156,   0,   0, 253, 255, 255, 255,   0,   0,
      0, 224, 255, 255,  63,   0,   2,   0,   0, 252, 255, 255, 255,   7,  48,   4,
    255, 255,  63,   4,  16,   1,   0,   0, 255, 255, 255,   1, 240, 255, 255, 255,
    255, 255, 255,  35,   0,   0,   1, 255,   3,   0, 254, 254, 224, 159, 249, 255,
    255, 253, 197,  35,   0,  64,   0, 176,   3,   0,   3,   0, 224, 135, 249, 255,
    255, 253, 109,   3,   0,   0,   0,  94,   0,   0,  28,   0, 224, 191, 251, 255,
    255, 253, 237,  35,   0,   0,   1,   0,   3,   0,   0,   0,   0,   0,   0, 176,
      3,   0,   2,   0, 232, 199,  61, 214,  24, 199, 255,   3, 224, 223, 253, 255,
    255, 253, 239,  35,   0,   0,   0,   3,   0,   0,   0,  64,   3,   0,   6,   0,
    255, 255, 255,  39,   0,  64,   0,   0,   3,   0,   0, 252, 224, 255, 127, 252,
    255, 255, 251,  47, 127,   0,   0,   0, 255, 255,  13,   0, 150,  37, 240, 254,
    174, 236,  13,  32,  95,   0,   0,  48,   1,   0,   0,   0, 255, 254, 255, 255,
    255,  31,   0,   0,   0,  31,   0,   0, 255,   7,   0, 128,   0,   0,  63,  60,
     98, 192, 225, 255,   3,  64,   0,   0,  63,   0, 255, 255, 255, 255, 255,  23,
    255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255,
    255, 255,  61, 255, 255, 255, 255,   7, 255, 255,  31,   0, 255, 159, 255, 255,
    255, 199,   1,   0, 255, 223,   3,   0, 255, 255,   3,   0, 255, 223,   1,   0,
    255, 255,  15,   0,   0,   0, 128,  16, 255, 255, 255,   0, 255,   5, 255, 255,
    255, 255,  63,   0, 255, 255, 255,  31, 255,  63,  31,   0, 255,  15,   0,   0,
    254,   0,   0,   0, 255, 255, 127,   0, 128,   0,   0,   0, 224, 255, 255, 255,
    224,  15,   0,   0, 248, 255, 255, 255,   1, 192,   0,   0,  63,   0,   0,   0,
     15,   0,   0,   0,   0, 224,   0, 252, 255, 255, 255,  63,   0, 222,   3,   0,
    255, 255,  63,  63,  63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15,
    255,  31, 220,  31,   0,   0,   2, 128,   0,   0, 255,  31, 132, 252,  47,  63,
     80, 253, 255, 243, 224,  67,   0,   0, 255,   1,   0,   0, 255, 127, 255, 255,
    255, 255, 255, 127,  31, 120,   0,   0,  63, 128,   0,   0, 127, 127, 127, 127,
    224,   0,   0,   0, 254,   3,  62,  31, 255, 255, 127, 248, 255, 255, 255, 247,
    255,  63, 254, 255, 255, 127,   0,   0, 255,  31, 255, 255,   0,  12,   0,   0,
    255, 127,   0, 128,   0,   0, 128, 255, 252, 255, 255, 255, 255, 121,   3,   0,
    255,   3,   0,   0,   0,   0,   0, 252, 187, 247, 255, 255,   7,   0,   0,   0,
      0,   0, 252,   8, 255, 255,   7,   0,   0, 128,   0,   0, 247,  15,   0,   0,
    255, 255, 127,   4, 255, 255,  98,  62,   5,   0,   0,  56, 126, 126, 126,   0,
    127, 127,   0,   0,  15,   0, 255, 255, 127, 248, 255, 255, 255, 255, 255,  15,
    255,  63, 255, 255, 255, 255, 255,   3, 127,   0, 248, 160, 255, 253, 127,  95,
    219, 255, 255, 255,   0,   0, 248, 255, 255, 255, 252, 255,   0,   0, 255,  15,
      0,   0, 223, 255, 192, 255, 255, 255, 252, 252, 252,  28, 255, 239, 255, 255,
    127, 255, 255, 183, 255,  63, 255,  63, 255, 255,   1,   0,  15, 255,  62,   0,
     63, 253, 255, 255, 255, 255, 191, 145,   1,   0, 239, 254, 255, 255, 223, 255,
    255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223,
    255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255, 253, 255, 255, 247,
    255, 253, 255, 255,
};

/* ID_Start: 1677 bytes. */

RE_UINT32 re_get_id_start(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_id_start_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_id_start_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_id_start_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_id_start_stage_4[pos + f] << 5;
    pos += code;
    value = (re_id_start_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* ID_Continue. */

static RE_UINT8 re_id_continue_stage_1[] = {
    0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 6, 6,
    6, 6,
};

static RE_UINT8 re_id_continue_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 13, 13, 13, 14,
    15, 16, 17, 13, 18, 13, 19, 13, 13, 13, 13, 13, 13, 20, 13, 13,
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 13, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 23,  7, 24, 25, 13, 13, 13, 13, 13, 13, 13, 26,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    27, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_id_continue_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1, 17, 18, 19,  1, 20, 21, 22, 23, 24, 25, 26, 27,  1, 28,
    29, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33, 31, 31,
    34, 35, 31, 31,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1, 36,  1,  1,  1,  1,  1,  1,  1,  1,  1, 37,
     1,  1,  1,  1, 38,  1, 39, 40, 41, 42, 43, 44,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1, 45, 31, 31, 31, 31, 31, 31, 31, 31,
    31,  1, 46, 47,  1, 48, 49, 50, 51, 52, 53, 54, 55, 31, 31, 31,
    56, 57, 58, 59, 60, 31, 31, 31, 61, 31, 31, 31, 31, 31, 31, 31,
     1,  1,  1, 62, 63, 31, 31, 31,  1,  1,  1,  1, 64, 31, 31, 31,
     1,  1, 65, 31, 31, 31, 31, 31, 66, 31, 31, 31, 31, 31, 31, 31,
    31, 67, 68, 31, 69, 70, 71, 72,  1,  1,  1,  1,  1,  1, 73,  1,
     1,  1,  1,  1,  1,  1,  1, 74, 75, 31, 31, 31, 31, 31, 31, 31,
     1,  1, 75, 31, 31, 31, 31, 31, 31, 76, 31, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_id_continue_stage_4[] = {
      0,   1,   2,   3,   0,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   7,   8,   6,   6,   6,   9,  10,  11,   6,  12,
      6,   6,   6,   6,  13,   6,   6,   6,   6,  14,  15,  16,  14,  17,  18,  19,
     20,   6,   6,  21,   6,   6,  22,  23,  24,   6,  25,   6,   6,  26,   6,  27,
      6,  28,  29,   0,   0,   0,   0,   0,   6,   6,   6,  30,  31,  32,  33,  34,
     35,  36,  37,  38,  39,  40,  41,  42,  31,  40,  43,  44,  45,  46,  47,  48,
     49,  50,  51,  42,  52,  53,  54,  55,  52,  56,  57,  58,  59,  60,  61,  62,
     16,  63,  64,   0,  65,  66,  67,   0,  68,  69,  70,  71,  72,  73,  74,   0,
      6,   6,  75,   6,  76,   6,  77,  78,   6,   6,  79,   6,  80,  81,  82,   6,
     83,   6,  56,  84,  85,   6,   6,  86,  16,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,  87,   3,   6,   6,  88,  89,  86,  90,  91,   6,  92,  93,  94,
     95,   6,   6,  96,   6,  97,   6,  98,  73,  99, 100, 101,   6, 102, 103,   0,
     29,   6, 104, 105, 106, 107,   0,   0,   6,   6, 108, 109,   6, 110,   6,  90,
      6,  96, 111,  76,   0,   0, 112, 113,   6,   6,   6,   6,   6,   6,   6, 114,
    115,   6, 116,  76,   6, 117, 118, 119,   0, 120, 121, 122, 123,   0, 123, 124,
    125, 126, 127,   6, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      6, 129, 104,   6,   6,   6,   6, 130,   6,  77,   6, 131, 132, 133, 133,   6,
    134, 135,  16,   6, 136,  16,   6, 137, 138, 139,   6,   6, 140,  63,   0,  24,
      6,   6,   6,   6,   6,  98,   0,   0,   6,   6,   6,   6,   6,   6, 141,   0,
      6,   6,   6,   6, 142,   0,  24,  76, 143, 141,   6, 144,  96,   6,   6,  26,
    145, 146,   6,   6, 147,  94,   0, 148,   6, 149,   6,  90,   6,   6, 150, 151,
      6, 152,  90,  73,   6,   6, 153,   0,   6, 132, 154, 155,   6,   6, 156,   0,
    157, 158,   0,   0,   0,   0,   6, 159,   6,   6,   6,   6,   6, 160, 161,  29,
      6, 152,   6, 152,   6,   6, 162,   0, 163, 164, 165,   6,   6,  26, 166,   6,
      6,  76,  24,   6, 167,   6, 149, 168,  85, 169, 170, 171,   6,   6,   6,  73,
      1,   2,   3, 100,   6, 104, 172,   0, 173, 174, 175,   0,   6,   6,   6,  63,
      0,   0,   6,  86,   0,   0,   0, 176,   0,   0,   0,   0,  73,   6, 177,   0,
    104,  24, 178,   0,  76,   6, 179,   0,   6,   6,   6,   6,  76,  94,   0,   0,
    180, 181,  98,   0,   0,   0,   0,   0,  98, 162,   0,   0,   0,   0,   0,   0,
    182, 183,   0,  73,   0,   0,   0,   0,   6,  98,  98, 113,   0,   0,   0,   0,
      6,   6, 128,   0,   0,   0,   0,   0,   6,   6, 184,  48,   6,  63,   0,   0,
      6,   6,   6, 140,   0,   0,   0,   0,   6,   6,   6, 185,   0,   0,   0,   0,
      6, 140,   0,   0,   0,   0,   0,   0,   6, 186,   0,   0,   0,   0,   0,   0,
    187,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 188, 189, 190,   0,   0,
      0,   0, 191,   0,   0,   0,   0,   0,   6,   6, 192,   6, 193, 194, 195,   6,
    196, 197, 198,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6, 199, 200, 137,
    192, 192, 129, 129, 201, 201, 202,   6,   6,   6,   6,   6,   6,   6, 132,   0,
      6,  86,   6,   6,   6,   6,   6,   6,  76,   0,   0,   0,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   6,  85,
};

static RE_UINT8 re_id_continue_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 254, 255, 255, 135, 254, 255, 255,   7,
      0,   4, 160,   4, 255, 255, 127, 255, 255, 255, 255, 255, 195, 255,   3,   0,
     31,  80,   0,   0, 255, 255, 223,  60, 192, 215, 255, 255, 251, 255, 255, 255,
    255, 255, 191, 255, 251, 252, 255, 255, 255,   0, 254, 255, 255, 255, 127,   2,
    254, 255, 255, 255, 255, 255, 255, 191, 182,   0, 255, 255, 255,   7,   7,   0,
      0,   0, 255,   7, 255, 195, 255, 255, 255, 255, 239, 159, 255, 253, 255, 159,
      0,   0, 255, 255, 255, 231, 255, 255, 255, 255,   3,   0, 255, 255,  63,   4,
    255,  63,   0,   0, 255, 255, 255,  15, 207, 255, 254, 254, 238, 159, 249, 255,
    255, 253, 197, 243, 159, 121, 128, 176, 207, 255,   3,   0, 238, 135, 249, 255,
    255, 253, 109, 211, 135,  57,   2,  94, 192, 255,  63,   0, 238, 191, 251, 255,
    255, 253, 237, 243, 191,  59,   1,   0, 207, 255,   0,   0, 159,  57, 192, 176,
    207, 255,   2,   0, 236, 199,  61, 214,  24, 199, 255, 195, 199,  61, 129,   0,
    192, 255,   0,   0, 238, 223, 253, 255, 255, 253, 239, 227, 223,  61,  96,   3,
    236, 223, 253, 255, 255, 253, 239, 243, 223,  61,  96,  64, 207, 255,   6,   0,
    255, 255, 255, 231, 223, 125, 128,   0, 207, 255,   0, 252, 236, 255, 127, 252,
    255, 255, 251,  47, 127, 132,  95, 255,   0,   0,  12,   0, 255, 255, 255,   7,
    255, 127, 255,   3, 150,  37, 240, 254, 174, 236, 255,  59,  95,  63, 255,  51,
      1,   0,   0,   3, 255,   3, 160, 194, 255, 254, 255, 255, 255,  31, 254, 255,
    223, 255, 255, 254, 255, 255, 255,  31,  64,   0,   0,   0, 255,   3, 255, 255,
    255, 255, 255,  63,  63,   0, 255, 255, 255, 255, 255,  23, 255,  61, 127,  61,
    255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255, 255, 255,  61, 255,
      0, 254,   3,   0, 255, 255,   0,   0, 255, 255,  31,   0, 255, 159, 255, 255,
    255, 199,   1,   0, 255, 223,  31,   0, 255, 255,  15,   0, 255, 223,  13,   0,
    255, 255, 207, 255, 255, 255, 143,  48, 255,   3,   0,   0,   0,  56, 255,   3,
    255, 255, 255,   0, 255,   7, 255, 255, 255, 255,  63,   0, 255,  15, 255,  15,
    192, 255, 255, 255, 255,  63,  31,   0, 255,  15, 255, 255, 255,   3, 255,   7,
    255, 255, 255, 127, 255, 255, 255, 159, 255,   3, 255,   3, 128,   0,   0,   0,
    255,  15, 255,   3,   0, 248,  15,   0, 255, 199, 255,   3, 255, 227, 255, 255,
      0,   0, 247, 255, 255, 255,   7,   0, 127,   0,   0, 240, 255, 255,  63,  63,
     63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,
      0,   0,   0, 128,   1,   0,  16,   0,   0,   0,   2, 128,   0,   0, 255,  31,
    226, 255,   1,   0, 132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0,
    255,   1,   0,   0, 255, 127, 255, 255,  31, 248,   3,   0,  63, 128,   0, 128,
    255, 255, 127,   0, 127, 127, 127, 127, 224,   0,   0,   0, 254, 255,  62,  31,
    255, 255, 127, 254, 255, 255, 255, 247, 224, 255, 255, 255, 255,  63, 254, 255,
    255, 127,   0,   0, 255,  15,   0,   0, 255,  31,   0,   0, 255,  31, 255, 255,
    255, 255,   0, 176,   0,   0, 128, 255, 252, 255, 255, 255, 255, 121,   3,   0,
      0,   0,   0, 252, 255,   0,   0,   0,  31,   0, 255,   3, 255, 255, 255,   8,
    255,  63, 255, 255,   1, 128, 255,   3, 255,  63, 255,   3, 255, 255, 127,  12,
      7,   0,   0,  56, 126, 126, 126,   0, 127, 127,   0,   0, 255,  55, 255,   3,
     15,   0, 255, 255, 127, 248, 255, 255, 255, 255, 255,   3, 127,   0, 248, 224,
    255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255, 255, 255, 252, 255,
      0,   0, 255,  15, 127,   0,  24,   0,   0, 224,   0,   0,   0,   0, 223, 255,
    252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63,
      0,   0,   0,  32, 255, 255,   1,   0, 255,   7,   0,   0,  15, 255,  62,   0,
     63, 253, 255, 255, 255, 255, 191, 145, 111, 240, 239, 254, 255, 255,  15, 135,
    127,   0,   0,   0,   7,   0,   0,   0, 255, 255, 255,   1,   3,   0,   0,   0,
    224, 227,   7, 248, 231,  15,   0,   0,   0,  60,   0,   0,  28,   0,   0,   0,
    255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255,
    191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255,
    253, 255, 255, 247, 255, 253, 255, 255, 247, 207, 255, 255,
};

/* ID_Continue: 1814 bytes. */

RE_UINT32 re_get_id_continue(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_id_continue_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_id_continue_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_id_continue_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_id_continue_stage_4[pos + f] << 5;
    pos += code;
    value = (re_id_continue_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* XID_Start. */

static RE_UINT8 re_xid_start_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3,
};

static RE_UINT8 re_xid_start_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 13, 13, 13, 14,
    15, 16, 17, 13, 18, 13, 19, 13, 13, 13, 13, 13, 13, 20, 13, 13,
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 13, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 23,  7, 24, 25, 13, 13, 13, 13, 13, 13, 13, 26,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_xid_start_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1, 17, 18, 19,  1, 20, 21, 22, 23, 24, 25, 26, 27,  1, 28,
    29, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33, 31, 31,
    34, 35, 31, 31,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1, 36,  1,  1,  1,  1,  1,  1,  1,  1,  1, 37,
     1,  1,  1,  1, 38,  1, 39, 40, 41, 42, 43, 44,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1, 45, 31, 31, 31, 31, 31, 31, 31, 31,
    31,  1, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 31, 31, 31,
    57, 58, 59, 60, 61, 31, 31, 31, 62, 31, 31, 31, 31, 31, 31, 31,
     1,  1,  1, 63, 64, 31, 31, 31,  1,  1,  1,  1, 65, 31, 31, 31,
     1,  1, 66, 31, 31, 31, 31, 31, 67, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 68, 69, 70, 71,  1,  1,  1,  1,  1,  1, 72,  1,
     1,  1,  1,  1,  1,  1,  1, 73, 74, 31, 31, 31, 31, 31, 31, 31,
     1,  1, 74, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_xid_start_stage_4[] = {
      0,   0,   1,   1,   0,   2,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   5,   6,   0,   0,   0,   7,   8,   9,   4,  10,
      4,   4,   4,   4,  11,   4,   4,   4,   4,  12,  13,  14,  15,   0,  16,  17,
      0,   4,  18,  19,   4,   4,  20,  21,  22,  23,  24,   4,   4,  25,  26,  27,
     28,  29,  30,   0,   0,   0,   0,   0,  31,  32,  33,  34,  35,  36,  37,  38,
     39,  40,  41,  42,  43,  44,  45,  46,  35,  44,  47,  48,  49,  50,  45,   0,
     51,  52,  53,  46,  51,  52,  54,  55,  51,  56,  57,  58,  59,  60,  61,   0,
     14,  62,  61,   0,  63,  64,  65,   0,  66,   0,  67,  68,  69,   0,   0,   0,
      4,  70,  71,  72,  73,   4,  74,  75,   4,   4,  76,   4,  77,  78,  79,   4,
     80,   4,  81,   0,  23,   4,   4,  82,  14,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,  83,   1,   4,   4,  84,  85,  86,  86,  87,   4,  88,  89,   0,
      0,   4,   4,  90,   4,  91,   4,  92,  93,   0,  16,  94,   4,  95,  96,   0,
     97,   4,  82,   0,   0,  98,   0,   0,  99,  88, 100,   0, 101, 102,   4, 103,
      4, 104, 105, 106,   0,   0,   0, 107,   4,   4,   4,   4,   4,   4,   0,   0,
    108,   4, 109, 106,   4, 110, 111, 112,   0,   0,   0, 113, 114,   0,   0,   0,
    115, 116, 117,   4, 118,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      4, 119, 120,   4,   4,   4,   4, 121,   4,  74,   4, 122,  97, 123, 123,   0,
    124, 125,  14,   4, 126,  14,   4, 127,  99, 128,   4,   4, 129,  81,   0,  16,
      4,   4,   4,   4,   4,  92,   0,   0,   4,   4,   4,   4,   4,   4,  95,   0,
      4,   4,   4,   4,  68,   0,  16, 106, 130, 131,   4, 132,  90,   4,   4,  23,
    133, 134,   4,   4, 135, 136,   0, 137, 138, 139,   4,  88, 134,  88,   0, 140,
     26,  74,  61,  93,  31, 141, 142,   0,   4, 118, 143, 144,   4, 145, 146,   0,
    147, 148,   0,   0,   0,   0,   4, 139,   4,   4,   4,   4,   4, 149, 150, 151,
      4, 152,   4, 152,   4,   4, 153,   0, 154, 155, 156,   4,   4,  86, 157,   4,
      4,   4, 106,  31,   4,   4,   4,   4,   4, 106,  16,   4, 158,   4,  15, 159,
      0,   0,   0, 160,   4,   4,   4,  93,   0,   1,   1, 161, 106, 120, 162,   0,
    163, 164, 165,   0,   4,   4,   4,  81,   0,   0,   4,  82,   0,   0,   0,   0,
      0,   0,   0,   0,  93,   4, 166,   0, 120,  16,  18,   0, 106,   4, 167,   0,
      4,   4,   4,   4, 106,   0,   0,   0, 168, 169,  92,   0,   0,   0,   0,   0,
     92, 153,   0,   0,   0,   0,   0,   0, 170,  88,   0,  93,   0,   0,   0,   0,
      4,  92,  92, 141,   0,   0,   0,   0,   4,   4, 118,   0,   0,   0,   0,   0,
    101,  90,   0,   0, 101,  23,   0,   0,   4,   4,   4, 129,   0,   0,   0,   0,
      4,   4,   4, 139,   0,   0,   0,   0,   4, 129,   0,   0,   0,   0,   0,   0,
      4,  30,   0,   0,   0,   0,   0,   0,  46,   0,   0,   0,   0,   0,   0,   0,
      4,   4, 171,   4, 172, 173, 174,   4, 175, 176, 177,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4, 178, 179, 127, 171, 171, 119, 119, 180, 180, 143,   0,
      4,   4,   4,   4,   4,   4,  97,   0,   4,  82,   4,   4,   4,   4,   4,   4,
    106,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_xid_start_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,   0,   0, 223,  56,
     64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,   3, 252, 255, 255,
    255,   0, 254, 255, 255, 255, 127,   2, 254, 255, 255, 255, 255,   0,   0,   0,
      0,   0, 255, 255, 255,   7,   7,   0, 255,   7,   0,   0,   0, 192, 254, 255,
    255, 255,  47,   0,  96, 192,   0, 156,   0,   0, 253, 255, 255, 255,   0,   0,
      0, 224, 255, 255,  63,   0,   2,   0,   0, 252, 255, 255, 255,   7,  48,   4,
    255, 255,  63,   4,  16,   1,   0,   0, 255, 255, 255,   1, 240, 255, 255, 255,
    255, 255, 255,  35,   0,   0,   1, 255,   3,   0, 254, 254, 224, 159, 249, 255,
    255, 253, 197,  35,   0,  64,   0, 176,   3,   0,   3,   0, 224, 135, 249, 255,
    255, 253, 109,   3,   0,   0,   0,  94,   0,   0,  28,   0, 224, 191, 251, 255,
    255, 253, 237,  35,   0,   0,   1,   0,   3,   0,   0,   0,   0,   0,   0, 176,
      3,   0,   2,   0, 232, 199,  61, 214,  24, 199, 255,   3, 224, 223, 253, 255,
    255, 253, 239,  35,   0,   0,   0,   3,   0,   0,   0,  64,   3,   0,   6,   0,
    255, 255, 255,  39,   0,  64,   0,   0,   3,   0,   0, 252, 224, 255, 127, 252,
    255, 255, 251,  47, 127,   0,   0,   0, 255, 255,   5,   0, 150,  37, 240, 254,
    174, 236,   5,  32,  95,   0,   0,  48,   1,   0,   0,   0, 255, 254, 255, 255,
    255,  31,   0,   0,   0,  31,   0,   0, 255,   7,   0, 128,   0,   0,  63,  60,
     98, 192, 225, 255,   3,  64,   0,   0,  63,   0, 255, 255, 255, 255, 255,  23,
    255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255,
    255, 255,  61, 255, 255, 255, 255,   7, 255, 255,  31,   0, 255, 159, 255, 255,
    255, 199,   1,   0, 255, 223,   3,   0, 255, 255,   3,   0, 255, 223,   1,   0,
    255, 255,  15,   0,   0,   0, 128,  16, 255, 255, 255,   0, 255,   5, 255, 255,
    255, 255,  63,   0, 255, 255, 255,  31, 255,  63,  31,   0, 255,  15,   0,   0,
    254,   0,   0,   0, 255, 255, 127,   0, 128,   0,   0,   0, 224, 255, 255, 255,
    224,  15,   0,   0, 248, 255, 255, 255,   1, 192,   0,   0,  63,   0,   0,   0,
     15,   0,   0,   0,   0, 224,   0, 252, 255, 255, 255,  63,   0, 222,   3,   0,
    255, 255,  63,  63,  63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15,
    255,  31, 220,  31,   0,   0,   2, 128,   0,   0, 255,  31, 132, 252,  47,  63,
     80, 253, 255, 243, 224,  67,   0,   0, 255,   1,   0,   0, 255, 127, 255, 255,
    255, 255, 255, 127,  31, 120,   0,   0,  63, 128,   0,   0, 127, 127, 127, 127,
    224,   0,   0,   0, 254,   3,  62,  31, 255, 255, 127, 224, 255, 255, 255, 247,
    255,  63, 254, 255, 255, 127,   0,   0, 255,  31, 255, 255,   0,  12,   0,   0,
    255, 127,   0, 128,   0,   0, 128, 255, 252, 255, 255, 255, 255, 121,   3,   0,
    255,   3,   0,   0,   0,   0,   0, 252, 187, 247, 255, 255,   7,   0,   0,   0,
      0,   0, 252,   8, 255, 255,   7,   0,   0, 128,   0,   0, 247,  15,   0,   0,
    255, 255, 127,   4, 255, 255,  98,  62,   5,   0,   0,  56, 126, 126, 126,   0,
    127, 127,   0,   0,  15,   0, 255, 255, 127, 248, 255, 255, 255, 255, 255,  15,
    255,  63, 255, 255, 255, 255, 255,   3, 127,   0, 248, 160, 255, 253, 127,  95,
    219, 255, 255, 255,   0,   0, 248, 255, 255, 255, 252, 255,   0,   0, 255,   3,
      0,   0, 138, 170, 192, 255, 255, 255, 252, 252, 252,  28, 255, 239, 255, 255,
    127, 255, 255, 183, 255,  63, 255,  63, 255, 255,   1,   0,  15, 255,  62,   0,
     63, 253, 255, 255, 255, 255, 191, 145,   1,   0, 239, 254, 255, 255, 223, 255,
    255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223,
    255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255, 253, 255, 255, 247,
    255, 253, 255, 255,
};

/* XID_Start: 1685 bytes. */

RE_UINT32 re_get_xid_start(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_xid_start_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_xid_start_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_xid_start_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_xid_start_stage_4[pos + f] << 5;
    pos += code;
    value = (re_xid_start_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* XID_Continue. */

static RE_UINT8 re_xid_continue_stage_1[] = {
    0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 6, 6,
    6, 6,
};

static RE_UINT8 re_xid_continue_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 13, 13, 13, 14,
    15, 16, 17, 13, 18, 13, 19, 13, 13, 13, 13, 13, 13, 20, 13, 13,
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 13, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 23,  7, 24, 25, 13, 13, 13, 13, 13, 13, 13, 26,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    27, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_xid_continue_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1, 17, 18, 19,  1, 20, 21, 22, 23, 24, 25, 26, 27,  1, 28,
    29, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33, 31, 31,
    34, 35, 31, 31,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1, 36,  1,  1,  1,  1,  1,  1,  1,  1,  1, 37,
     1,  1,  1,  1, 38,  1, 39, 40, 41, 42, 43, 44,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1, 45, 31, 31, 31, 31, 31, 31, 31, 31,
    31,  1, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 31, 31, 31,
    57, 58, 59, 60, 61, 31, 31, 31, 62, 31, 31, 31, 31, 31, 31, 31,
     1,  1,  1, 63, 64, 31, 31, 31,  1,  1,  1,  1, 65, 31, 31, 31,
     1,  1, 66, 31, 31, 31, 31, 31, 67, 31, 31, 31, 31, 31, 31, 31,
    31, 68, 69, 31, 70, 71, 72, 73,  1,  1,  1,  1,  1,  1, 74,  1,
     1,  1,  1,  1,  1,  1,  1, 75, 76, 31, 31, 31, 31, 31, 31, 31,
     1,  1, 76, 31, 31, 31, 31, 31, 31, 77, 31, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_xid_continue_stage_4[] = {
      0,   1,   2,   3,   0,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   7,   8,   6,   6,   6,   9,  10,  11,   6,  12,
      6,   6,   6,   6,  13,   6,   6,   6,   6,  14,  15,  16,  14,  17,  18,  19,
     20,   6,   6,  21,   6,   6,  22,  23,  24,   6,  25,   6,   6,  26,   6,  27,
      6,  28,  29,   0,   0,   0,   0,   0,   6,   6,   6,  30,  31,  32,  33,  34,
     35,  36,  37,  38,  39,  40,  41,  42,  31,  40,  43,  44,  45,  46,  47,  48,
     49,  50,  51,  42,  52,  53,  54,  55,  52,  56,  57,  58,  59,  60,  61,  62,
     16,  63,  64,   0,  65,  66,  67,   0,  68,  69,  70,  71,  72,  73,  74,   0,
      6,   6,  75,   6,  76,   6,  77,  78,   6,   6,  79,   6,  80,  81,  82,   6,
     83,   6,  56,  84,  85,   6,   6,  86,  16,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,  87,   3,   6,   6,  88,  89,  86,  90,  91,   6,  92,  93,  94,
     95,   6,   6,  96,   6,  97,   6,  98,  73,  99, 100, 101,   6, 102, 103,   0,
     29,   6, 104, 105, 106, 107,   0,   0,   6,   6, 108, 109,   6, 110,   6,  90,
      6,  96, 111,  76,   0,   0, 112, 113,   6,   6,   6,   6,   6,   6,   6, 114,
    115,   6, 116,  76,   6, 117, 118, 119,   0, 120, 121, 122, 123,   0, 123, 124,
    125, 126, 127,   6, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      6, 129, 104,   6,   6,   6,   6, 130,   6,  77,   6, 131, 132, 133, 133,   6,
    134, 135,  16,   6, 136,  16,   6, 137, 138, 139,   6,   6, 140,  63,   0,  24,
      6,   6,   6,   6,   6,  98,   0,   0,   6,   6,   6,   6,   6,   6, 141,   0,
      6,   6,   6,   6, 142,   0,  24,  76, 143, 141,   6, 144,  96,   6,   6,  26,
    145, 146,   6,   6, 147,  94,   0, 148,   6, 149,   6,  90,   6,   6, 150, 151,
      6, 152,  90,  73,   6,   6, 153,   0,   6, 132, 154, 155,   6,   6, 156,   0,
    157, 158,   0,   0,   0,   0,   6, 159,   6,   6,   6,   6,   6, 160, 161,  29,
      6, 152,   6, 152,   6,   6, 162,   0, 163, 164, 165,   6,   6,  26, 166,   6,
      6,   6,  76, 167,   6,   6,   6,   6,   6,  76,  24,   6, 168,   6, 149,   1,
     85, 169, 170, 171,   6,   6,   6,  73,   1,   2,   3, 100,   6, 104, 172,   0,
    173, 174, 175,   0,   6,   6,   6,  63,   0,   0,   6,  86,   0,   0,   0, 176,
      0,   0,   0,   0,  73,   6, 177,   0, 104,  24, 178,   0,  76,   6, 179,   0,
      6,   6,   6,   6,  76,  94,   0,   0, 180, 181,  98,   0,   0,   0,   0,   0,
     98, 162,   0,   0,   0,   0,   0,   0, 182, 183,   0,  73,   0,   0,   0,   0,
      6,  98,  98, 113,   0,   0,   0,   0,   6,   6, 128,   0,   0,   0,   0,   0,
      6,   6, 184,  48,   6,  63,   0,   0,   6,   6,   6, 140,   0,   0,   0,   0,
      6,   6,   6, 185,   0,   0,   0,   0,   6, 140,   0,   0,   0,   0,   0,   0,
      6, 186,   0,   0,   0,   0,   0,   0, 187,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 188, 189, 190,   0,   0,   0,   0, 191,   0,   0,   0,   0,   0,
      6,   6, 192,   6, 193, 194, 195,   6, 196, 197, 198,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6, 199, 200, 137, 192, 192, 129, 129, 201, 201, 202,   6,
      6,   6,   6,   6,   6,   6, 132,   0,   6,  86,   6,   6,   6,   6,   6,   6,
     76,   0,   0,   0,   0,   0,   0,   0,   6,   6,   6,   6,   6,   6,   6,  85,
};

static RE_UINT8 re_xid_continue_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 254, 255, 255, 135, 254, 255, 255,   7,
      0,   4, 160,   4, 255, 255, 127, 255, 255, 255, 255, 255, 195, 255,   3,   0,
     31,  80,   0,   0, 255, 255, 223,  56, 192, 215, 255, 255, 251, 255, 255, 255,
    255, 255, 191, 255, 251, 252, 255, 255, 255,   0, 254, 255, 255, 255, 127,   2,
    254, 255, 255, 255, 255, 255, 255, 191, 182,   0, 255, 255, 255,   7,   7,   0,
      0,   0, 255,   7, 255, 195, 255, 255, 255, 255, 239, 159, 255, 253, 255, 159,
      0,   0, 255, 255, 255, 231, 255, 255, 255, 255,   3,   0, 255, 255,  63,   4,
    255,  63,   0,   0, 255, 255, 255,  15, 207, 255, 254, 254, 238, 159, 249, 255,
    255, 253, 197, 243, 159, 121, 128, 176, 207, 255,   3,   0, 238, 135, 249, 255,
    255, 253, 109, 211, 135,  57,   2,  94, 192, 255,  63,   0, 238, 191, 251, 255,
    255, 253, 237, 243, 191,  59,   1,   0, 207, 255,   0,   0, 159,  57, 192, 176,
    207, 255,   2,   0, 236, 199,  61, 214,  24, 199, 255, 195, 199,  61, 129,   0,
    192, 255,   0,   0, 238, 223, 253, 255, 255, 253, 239, 227, 223,  61,  96,   3,
    236, 223, 253, 255, 255, 253, 239, 243, 223,  61,  96,  64, 207, 255,   6,   0,
    255, 255, 255, 231, 223, 125, 128,   0, 207, 255,   0, 252, 236, 255, 127, 252,
    255, 255, 251,  47, 127, 132,  95, 255,   0,   0,  12,   0, 255, 255, 255,   7,
    255, 127, 255,   3, 150,  37, 240, 254, 174, 236, 255,  59,  95,  63, 255,  51,
      1,   0,   0,   3, 255,   3, 160, 194, 255, 254, 255, 255, 255,  31, 254, 255,
    223, 255, 255, 254, 255, 255, 255,  31,  64,   0,   0,   0, 255,   3, 255, 255,
    255, 255, 255,  63,  63,   0, 255, 255, 255, 255, 255,  23, 255,  61, 127,  61,
    255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255, 255, 255,  61, 255,
      0, 254,   3,   0, 255, 255,   0,   0, 255, 255,  31,   0, 255, 159, 255, 255,
    255, 199,   1,   0, 255, 223,  31,   0, 255, 255,  15,   0, 255, 223,  13,   0,
    255, 255, 207, 255, 255, 255, 143,  48, 255,   3,   0,   0,   0,  56, 255,   3,
    255, 255, 255,   0, 255,   7, 255, 255, 255, 255,  63,   0, 255,  15, 255,  15,
    192, 255, 255, 255, 255,  63,  31,   0, 255,  15, 255, 255, 255,   3, 255,   7,
    255, 255, 255, 127, 255, 255, 255, 159, 255,   3, 255,   3, 128,   0,   0,   0,
    255,  15, 255,   3,   0, 248,  15,   0, 255, 199, 255,   3, 255, 227, 255, 255,
      0,   0, 247, 255, 255, 255,   7,   0, 127,   0,   0, 240, 255, 255,  63,  63,
     63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,
      0,   0,   0, 128,   1,   0,  16,   0,   0,   0,   2, 128,   0,   0, 255,  31,
    226, 255,   1,   0, 132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0,
    255,   1,   0,   0, 255, 127, 255, 255,  31, 248,   3,   0,  63, 128,   0, 128,
    255, 255, 127,   0, 127, 127, 127, 127, 224,   0,   0,   0, 254, 255,  62,  31,
    255, 255, 127, 230, 255, 255, 255, 247, 224, 255, 255, 255, 255,  63, 254, 255,
    255, 127,   0,   0, 255,  15,   0,   0, 255,  31,   0,   0, 255,  31, 255, 255,
    255, 255,   0, 176,   0,   0, 128, 255, 252, 255, 255, 255, 255, 121,   3,   0,
      0,   0,   0, 252, 255,   0,   0,   0,  31,   0, 255,   3, 255, 255, 255,   8,
    255,  63, 255, 255,   1, 128, 255,   3, 255,  63, 255,   3, 255, 255, 127,  12,
      7,   0,   0,  56, 126, 126, 126,   0, 127, 127,   0,   0, 255,  55, 255,   3,
     15,   0, 255, 255, 127, 248, 255, 255, 255, 255, 255,   3, 127,   0, 248, 224,
    255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255, 240, 255, 255, 255,
    255, 255, 252, 255, 127,   0,  24,   0,   0, 224,   0,   0,   0,   0, 138, 170,
    252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63,
      0,   0,   0,  32, 255, 255,   1,   0, 255,   7,   0,   0,  15, 255,  62,   0,
     63, 253, 255, 255, 255, 255, 191, 145, 111, 240, 239, 254, 255, 255,  15, 135,
    127,   0,   0,   0,   7,   0,   0,   0, 255, 255, 255,   1,   3,   0,   0,   0,
    224, 227,   7, 248, 231,  15,   0,   0,   0,  60,   0,   0,  28,   0,   0,   0,
    255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255,
    191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255,
    253, 255, 255, 247, 255, 253, 255, 255, 247, 207, 255, 255,
};

/* XID_Continue: 1822 bytes. */

RE_UINT32 re_get_xid_continue(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_xid_continue_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_xid_continue_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_xid_continue_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_xid_continue_stage_4[pos + f] << 5;
    pos += code;
    value = (re_xid_continue_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Default_Ignorable_Code_Point. */

static RE_UINT8 re_default_ignorable_code_point_stage_1[] = {
    0, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_default_ignorable_code_point_stage_2[] = {
    0, 1, 2, 3, 4, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 7, 1, 1, 1, 1, 1,
    8, 8, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_default_ignorable_code_point_stage_3[] = {
     0,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  3,  1,  1,  1,  1,  1,  4,  5,  1,  1,  1,  1,  1,  1,  1,
     6,  1,  1,  1,  1,  1,  1,  1,  1,  7,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  8,  9,  1, 10,  1,  1,  1,  1,  1,  1,
    11, 11, 11, 11, 11, 11, 11, 11,
};

static RE_UINT8 re_default_ignorable_code_point_stage_4[] = {
     0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  3,  4,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  5,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,
     7,  8,  0,  9,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,
     9,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  4,  0, 11,
     0,  0,  0, 12,  0,  0,  0,  0, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_default_ignorable_code_point_stage_5[] = {
      0,   0,   0,   0,   0,  32,   0,   0,   0, 128,   0,   0,   0,   0,   0, 128,
      1,   0,   0,   0,   0,   0,  48,   0,   0,  56,   0,   0,   0, 248,   0,   0,
      0, 124,   0,   0, 255, 255,   0,   0,  16,   0,   0,   0,   0,   0, 255,   1,
      0,   0, 248,   7, 255, 255, 255, 255,
};

/* Default_Ignorable_Code_Point: 332 bytes. */

RE_UINT32 re_get_default_ignorable_code_point(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_default_ignorable_code_point_stage_1[f] << 3;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_default_ignorable_code_point_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_default_ignorable_code_point_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_default_ignorable_code_point_stage_4[pos + f] << 5;
    pos += code;
    value = (re_default_ignorable_code_point_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Grapheme_Extend. */

static RE_UINT8 re_grapheme_extend_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_grapheme_extend_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  8,  9,  7,  7,  7,  7,  7,  7,  7,  7,  7, 10,
    11, 12, 13,  7,  7,  7,  7,  7,  7,  7, 14,  7,  7,  7,  7,  7,
    15,  7,  7,  7,  7,  7,  7,  7,
};

static RE_UINT8 re_grapheme_extend_stage_3[] = {
     0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13,
    14,  0,  0, 15,  0,  0,  0, 16, 17, 18, 19, 20, 21, 22,  0,  0,
    23,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 25,  0,  0,
    26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 27,  0, 28, 29, 30, 31,  0,  0,  0,  0,
     0,  0,  0, 32,  0,  0, 33, 34,  0, 35,  0,  0,  0,  0,  0,  0,
     0,  0, 36,  0,  0,  0,  0,  0, 37,  0,  0,  0,  0,  0,  0,  0,
     0, 38, 39,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_grapheme_extend_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   2,   0,   0,   0,   0,
      0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   4,   5,   6,   0,
      7,   0,   8,   9,   0,   0,  10,  11,  12,  13,  14,   0,   0,  15,   0,  16,
     17,  18,  19,   0,   0,   0,   0,   0,  20,  21,  22,  23,  24,  25,  26,  23,
     27,  28,  29,  30,  27,  28,  31,  23,  24,  32,  33,  23,  34,  35,  36,   0,
      0,  37,  38,  23,   0,  39,  40,  23,   0,  35,  26,  23,   0,   0,  41,   0,
      0,  42,  43,   0,   0,  44,  45,   0,  46,  47,   0,  48,  49,  50,  51,   0,
      0,  52,  53,  54,  55,   0,   0,   0,   0,   0,  56,   0,   0,   0,   0,   0,
     57,  57,  58,  58,   0,  59,  60,   0,  61,   0,   0,   0,   0,  62,   0,   0,
      0,  63,   0,   0,   0,   0,   0,   0,  64,   0,  65,  66,   0,   0,   0,   0,
     67,  68,  34,  16,  69,  70,   0,  71,   0,  72,   0,   0,   0,   0,  73,  74,
      0,   0,   0,   0,   0,   0,   1,  75,  76,   0,   0,   0,   0,   0,  13,  77,
      0,   0,   0,   0,   0,   0,   0,  78,   0,   0,   0,  79,   0,   0,   0,   1,
      0,  80,   0,   0,  81,   0,   0,   0,   0,   0,   0,  82,   0,   0,   0,  83,
     84,  85,   0,   0,   0,   0,  86,  87,   0,  88,  89,   0,  20,  90,   0,   0,
      0,  91,  92,   0,   0,  93,  24,   0,   0,   0,   0,   0,   0,   0,   0,  94,
     35,   0,   0,   0,   0,   0,   0,   0,   2,  95,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  96,
     97,  98,   0,   0,   0,   0,   0,   0,  24,  99,  95,   0,  69, 100,   0,   0,
      0,   0,   0, 101, 102, 103,   0,   0,   0,   0, 104,   0,   0,   0,   0,   0,
      1,   1,   1,   1,   1,   1,   1,   2,
};

static RE_UINT8 re_grapheme_extend_stage_5[] = {
      0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0, 248,   3,   0,   0,
      0,   0, 254, 255, 255, 255, 255, 191, 182,   0,   0,   0,   0,   0, 255,   7,
      0, 248, 255, 255,   0,   0,   1,   0,   0,   0, 192, 159, 159,  61,   0,   0,
      0,   0,   2,   0,   0,   0, 255, 255, 255,   7,   0,   0, 192, 255,   1,   0,
      0, 248,  15,   0,   0,   0, 192, 251, 239,  62,   0,   0,   0,   0,   0,  14,
      7,   0,   0,   0,   0,   0,   0,  20, 254,  33, 254,   0,  12,   0,   0,   0,
      2,   0,   0,   0,   0,   0,   0,  80,  30,  32, 128,   0,   6,   0,   0,   0,
      0,   0,   0,  16, 134,  57,   2,   0,   0,   0,  35,   0, 190,  33,   0,   0,
      0,   0,   0, 208,  30,  32, 192,   0,   4,   0,   0,   0,   0,   0,   0,  64,
      1,  32, 128,   0,   0,   0,   0, 192, 193,  61,  96,   0,   0,   0,   0, 144,
     68,  48,  96,   0,   0, 132,  92, 128,   0,   0, 242,   7, 128, 127,   0,   0,
      0,   0, 242,  27,   0,  63,   0,   0,   0,   0,   0,   3,   0,   0, 160,   2,
      0,   0, 254, 127, 223, 224, 255, 254, 255, 255, 255,  31,  64,   0,   0,   0,
      0, 224, 253, 102,   0,   0,   0, 195,   1,   0,  30,   0, 100,  32,   0,  32,
      0,   0,   0, 224,   0,   0,  28,   0,   0,   0,  12,   0,   0,   0, 128,  63,
     64, 254,  15,  32,   0,  56,   0,   0,   0,   2,   0,   0, 135,   1,   4,  14,
      0,   0, 128,   1,   0,   0,  64, 127, 229,  31, 248, 159,  15,   0,   0,   0,
      0,   0, 208,  23,   3,   0,   0,   0,  60,   3,   0,   0,  64, 163,   3,   0,
      0, 240, 207,   0,   0,   0, 247, 255, 253,  33,   0,   0, 127,   0,   0, 240,
      0,  48,   0,   0, 255, 255,   1,   0,   0, 128,   3,   0,   0,   0,   0, 128,
      0, 252,   0,   0,   0,   0,   0,   6,   0, 128,   7,  48,   0,   0,   3,   0,
     68,   8,   0,   0,  96,   0,   0,   0,  16,   0,   0,   0, 255, 255,   3,   0,
    192,  63,   0,   0, 128, 255,   3,   0,   0,   0, 200,  19,   0, 126, 102,   0,
      8,  16,   0,   0,   0,   0, 157, 193,  32,  33,   0,   0, 127,   0,   0,   0,
      0,   0,   0,  32, 110, 240,   0,   0,   0,   0,   0, 135,   0,   0,   0, 255,
      0,   0, 120,   6, 160, 195,   7, 248, 231,  15,   0,   0,   0,  60,   0,   0,
     28,   0,   0,   0,
};

/* Grapheme_Extend: 1000 bytes. */

RE_UINT32 re_get_grapheme_extend(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_grapheme_extend_stage_1[f] << 3;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_grapheme_extend_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_grapheme_extend_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_grapheme_extend_stage_4[pos + f] << 5;
    pos += code;
    value = (re_grapheme_extend_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Grapheme_Base. */

static RE_UINT8 re_grapheme_base_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3,
};

static RE_UINT8 re_grapheme_base_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 13, 13, 13, 14,
    15, 16, 17, 13, 18, 13, 19, 13, 13, 13, 13, 13, 13, 20, 13, 13,
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 13, 23, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 24,  7, 25, 26, 13, 13, 13, 13, 13, 13, 13, 27,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_grapheme_base_stage_3[] = {
     0,  1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15,  1, 16, 17,  1,  1, 18, 19, 20, 21, 22, 23, 24, 25,  1, 26,
    27, 28,  1, 29, 30,  1,  1, 31,  1,  1,  1, 32, 33, 34, 35, 36,
    37, 38, 39,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1, 40,  1,  1,  1,  1,  1,  1,  1,  1,  1, 41,
     1,  1,  1,  1, 42,  1, 43, 44, 45, 46, 47, 48,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1, 49, 50, 50, 50, 50, 50, 50, 50, 50,
    50,  1, 51, 52,  1, 53, 54, 55, 56, 57, 58, 59, 60, 50, 50, 50,
    61, 62, 63, 64, 65, 50, 66, 50, 67, 50, 50, 50, 50, 50, 50, 50,
     1,  1,  1, 68, 69, 50, 50, 50,  1,  1,  1,  1, 70, 50, 50, 50,
     1,  1, 71, 50, 50, 50, 50, 50, 72, 50, 50, 50, 50, 50, 50, 50,
    73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
     1,  1,  1,  1,  1,  1, 89,  1,  1,  1,  1,  1,  1,  1,  1, 90,
    91, 50, 50, 50, 50, 50, 50, 50,  1,  1, 91, 50, 50, 50, 50, 50,
};

static RE_UINT8 re_grapheme_base_stage_4[] = {
      0,   1,   1,   2,   0,   3,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      0,   0,   0,   4,   5,   6,   1,   1,   1,   1,   1,   1,   7,   1,   1,   1,
      1,   8,   9,  10,  11,  12,  13,  14,  15,   1,  16,  17,   1,   1,  18,  19,
     20,  21,  22,   1,   1,  23,   1,  24,  25,  26,  27,   0,   0,   0,   0,   0,
     28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,
     32,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,
     55,  59,  60,  61,  62,  63,  64,  65,  10,  66,  67,   0,  68,  69,  70,   0,
     71,  72,  73,  74,  75,  76,  77,   0,   1,  78,  79,  80,  81,   1,  82,  83,
      1,   1,  84,   1,  85,  86,  87,   1,  88,   1,  89,  83,  90,   1,   1,  91,
      1,   1,   1,   1,  83,   1,   1,  92,  93,  94,  95,  96,   1,  97,  98,  99,
    100,   1,   1, 101,   1, 102,   1, 103,  83, 104, 105, 106,   1, 107, 108,   1,
    109,   1, 110, 111,  99, 112,   0,   0, 113, 114, 115, 116, 117, 118,   1, 119,
      1, 120, 121,   1,   0,   0, 122, 123,   1,   1,   1,   1,   1,   1,   0,   0,
    124,   1, 125, 126,   1, 127, 128, 129, 130, 131,   1, 132, 133,  90,   0,   0,
      1,   1,   1,   1, 134,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 135,
      1, 136,  16,   1,   1,   1,   1,   1,  10,   1,   1,   1,   1,   1, 137,   1,
      1,   1, 138,   0,   0,   0,   0,   0,   1, 139,   2,   1,   1,   1,   1, 140,
      1,  82,   1, 141, 142, 143, 143,   0,   1,  95,   0,   0, 144,   1,   1, 135,
      1,   1,   1,   1,   1,   1, 103, 145,   1, 134,  10,   1, 146,   1,   1,   1,
    147, 148,   1,   1, 139,  89,   1, 149,   2,   1,   1,   1,   1,   1,   1,   2,
      1,   1,   1,   1,   1, 103,   1,   1,   1,   1,   1,   1,   1,   1, 150,   0,
      1,   1,   1,   1, 151,   1, 152,   1,   1, 150,   1, 153, 101,   1,   1, 154,
      1,   1,   1,   1, 155, 156,   0, 157, 158, 159,   1, 101,   1,   1, 160, 161,
      1, 162, 163,  83,  28, 164, 165,   0,   1, 166, 167, 168,   1, 169, 170,   0,
    171, 172,   0,   0,   0,   0,   1, 173,   1,   1,   1,   1,   1, 149, 174, 168,
      0,   0,   0,   0,   0,   0,   0,   0,   1, 175,   1, 175,   1,   1,  90,   0,
    176, 177, 178,   1,   1,   1, 179,   1,   1,   1, 180,   1, 181,   1, 182, 183,
    184, 180, 185, 186,   1,   1,   1,  83,  10,   1,   1,   1, 126,   2, 187, 188,
    189, 190, 191,   0,   1,   1,   1,  89, 192, 193,   1,   1, 194,   0, 180,  83,
      0,   0,   0,   0,  83,   1,  92,   0,   2, 149,  16,   0, 195,   1, 196,   0,
      1,   1,   1,   1, 126, 156,   0,   0, 197, 198, 199,   0,   0,   0,   0,   0,
    200, 201,   0,   0,   0,   0,   0,   0, 202, 135, 203,   1,   0,   0,   0,   0,
      1, 204, 205, 206,   0,   0,   0,   0,   1,   1, 207,   0,   0,   0,   0,   0,
      0,   0,   0,   2,   0,   0,   0,   0, 208, 101, 209,  21, 117, 210, 211,   0,
      1,   1,   1, 212,   0,   0,   0,   0,   1,   1,   1, 213,   0,   0,   0,   0,
      1, 212,   0,   0,   0,   0,   0,   0,   1, 214,   0,   0,   0,   0,   0,   0,
    211,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1, 103,
      1, 215,   1, 216, 217, 218, 126,   0,   1,   1, 219,   0,   0,   0,   0,   0,
      1,   1, 142,  95,   0,   0,   0,   0,   1,   1, 127,   1, 220, 221, 222,   1,
    223, 224, 225,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 226,   1,   1,
      1,   1,   1,   1,   1,   1, 227,   1,   1, 107,   1,   1, 135, 228, 229,   0,
    130, 139,   1, 134,  89,   0,   0, 230, 231,  89, 232,   0,   0,   0,   0,   0,
      1, 233,   1,  83, 135,   1, 234,  92,   1,   2, 208,   1,   1,   1,   1, 235,
      1, 126, 180, 182,   0,   0,   0, 236, 237, 238, 239,   0,   1,   1, 240,   0,
      1,   1,   1, 135,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1, 142,   0,
      1,  91,   1,   1,   1,   1,   1,   1, 126,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_grapheme_base_stage_5[] = {
      0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 255, 223, 255, 255,
      0,   0, 255, 124, 240, 215, 255, 255, 251, 255, 255, 255,   7, 252, 255, 255,
    255,   0, 254, 255, 255, 255, 127, 254, 254, 255, 255, 255, 255,   6,   0,   0,
      0,   0,   0,  64,  73,   0, 255, 255, 255,   7,  31,   0, 192, 255,   0, 200,
    255,   7,   0,   0, 255, 255, 254, 255, 255, 255,  63,  64,  96, 194, 255, 255,
    255,  63, 253, 255, 255, 255,   0,   0,   0, 224, 255, 255,  63,   0,   2,   0,
    255,   7, 240,   7, 255, 255,  63,   4,  16,   1, 255, 127, 255, 255, 255,  65,
    248, 255, 255, 255, 255, 255, 255, 235,   1, 222,   1, 255, 243, 255, 255, 254,
    236, 159, 249, 255, 255, 253, 197, 163, 129,  89,   0, 176, 195, 255, 255,  15,
    232, 135, 249, 255, 255, 253, 109, 195,   1,   0,   0,  94, 192, 255,  28,   0,
    232, 191, 251, 255, 255, 253, 237, 227,   1,  26,   1,   0, 195, 255,   2,   0,
    255, 253, 237,  35, 129,  25,   0, 176, 195, 255, 255,   0, 232, 199,  61, 214,
     24, 199, 255, 131, 198,  29,   1,   0, 192, 255, 255,   7, 238, 223, 253, 255,
    255, 253, 239,  35,  30,   0,   0,   3, 195, 255,   0, 255, 236, 223, 253, 255,
    255, 253, 239,  99, 155,  13,   0,  64, 195, 255,   6,   0, 255, 255, 255, 167,
    193,  93,   0,   0, 195, 255,  63, 254, 236, 255, 127, 252, 255, 255, 251,  47,
    127,   0,   3, 127,   0,   0,  28,   0, 255, 255,  13, 128, 127, 128, 255,  15,
    150,  37, 240, 254, 174, 236,  13,  32,  95,   0, 255,  51, 255, 255, 255, 252,
    255, 255,  95, 253, 255, 254, 255, 255, 255,  31,   0, 128,  32,  31,   0,   0,
      0,   0,   0, 192, 191, 223, 255,   7, 255,  31,   2, 153, 255, 255, 255,  60,
    254, 255, 225, 255, 155, 223, 255, 223,  63,   0, 255, 255, 255, 255, 255,  31,
    255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255,
    255, 255,  61, 255, 255, 255, 255,   7, 255, 255, 255,   3, 255, 255,  31,   0,
    255, 255,   1,   0, 255, 223,   3,   0, 255, 255,  99,   0, 255, 255,   3,   0,
    255, 223,   1,   0, 255, 255,  79, 192, 191,   1, 240,  31, 255,   3, 255,   3,
    255,  71, 255,   3, 255, 255, 255,   0, 255,   5, 255, 255, 255, 255,  63,   0,
    120,  14, 251,   1, 241, 255, 255, 255, 255,  63,  31,   0, 255,  15, 255, 255,
    255,   3, 255, 199, 255, 255, 127, 206, 255, 255, 191,   0,  26, 224,   7,   0,
    255,  63,   0,   0, 240, 255, 255, 255, 255, 255,  47, 232, 251,  15, 255, 255,
    255,   7, 240,  31, 252, 255, 255, 255, 195, 196, 255,   3, 191,  92,  12, 240,
    255,  15,  48, 248, 255, 227, 255, 255,   0,   0,   8,   0,   2, 222,   7,   0,
    255, 255,  63,  63,  63,  63, 255, 170, 255, 255, 255,  63, 255, 255, 223, 255,
    223, 255, 207, 239, 255, 255, 220, 127, 255,   7, 255, 255, 255, 128, 255, 255,
      0,   0, 243, 255, 255, 127, 255,  31, 255,   3, 255, 255, 255, 255,  15,   0,
    127,   0,   0,   0, 255, 215, 255, 255, 255,  31, 255,   3, 255, 127, 255, 255,
    255, 127,   0, 254,  63, 128,   1,   0, 255, 255, 127,   0, 127, 127, 127, 127,
    255, 255, 255, 251,   0,   0, 255,  15, 255, 255, 127, 248, 224, 255, 255, 255,
    255,  63, 254, 255,  15,   0, 255, 255, 255,  15,   0,   0, 255,  31, 255, 255,
    127,   0, 255, 255, 255, 127,   8, 192, 255, 255, 252,   0, 255, 127,   3,   0,
    255,   3,   0,   0,   0,   0,   0, 252, 187, 247, 255, 255, 159,  15, 255,   3,
     15, 192, 255,   3,   0,   0, 252,  15,  63, 192, 255, 255, 127,   0,  12, 128,
    255, 255,  55, 236, 255, 191, 255, 195, 255, 129,  25,   0, 247,  47, 255, 243,
    255, 255, 255,  15, 255, 255,  98,  62,   5,   0,   0, 248, 126, 126, 126,   0,
    127, 127,   0,   0, 223,  30, 255,   3, 127, 248, 255, 255, 255,  63, 255, 255,
    127,   0, 248, 160, 255, 255, 127,  95, 219, 255, 255, 255,   3,   0, 248, 255,
      0,   0, 255, 255, 255, 255, 252, 255, 255,   0,   0,   0,   0,   0, 255,  63,
      0,   0, 255,   3, 255, 255, 247, 255, 127,  15, 223, 255, 252, 252, 252,  28,
    127, 127,   0,  48, 255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63,
    135, 255, 255, 255, 255, 255, 143, 255, 255,   7, 255,  15, 255, 255, 255, 191,
     15, 255,  63,   0,  63, 253, 255, 255, 255, 255, 191, 145, 255, 255, 191, 255,
    255, 255, 255, 143, 255, 255, 255, 131,   1,   0, 239, 254, 255,   0, 255,   1,
    255, 255,  63, 254, 255, 255,  63, 255, 255, 255,   7, 255, 255,   1,   0,   0,
    253, 255, 255, 255, 128,  63, 252, 255, 255, 255, 135, 217,   3,   0,   0,   0,
    255, 127,   0,   0,   7,   0,  15,   0, 255, 255, 255,   1, 127, 254, 255, 255,
     95,  60,   0,   0,  24, 240, 255, 255, 255, 195, 255, 255,  35,   0,   0,   0,
    255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223,
    255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255, 255, 207, 255, 255,
    255, 127, 254, 127, 254, 255, 254, 255, 192, 255, 255, 255,   7,   0, 255, 255,
    255,   1,   3,   0,   1,   0, 191, 255, 223,   7,   0,   0, 255, 255, 255,  30,
      0,   0,   0, 248, 254, 255,  93, 117,  63,  47, 239, 255, 225, 255,   0,   0,
     63,   0,   0,   0,
};

/* Grapheme_Base: 2069 bytes. */

RE_UINT32 re_get_grapheme_base(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_grapheme_base_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_grapheme_base_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_grapheme_base_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_grapheme_base_stage_4[pos + f] << 5;
    pos += code;
    value = (re_grapheme_base_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Grapheme_Link. */

static RE_UINT8 re_grapheme_link_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3,
};

static RE_UINT8 re_grapheme_link_stage_2[] = {
    0, 1, 2, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_grapheme_link_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  2,  3,  4,
     5,  0,  0,  0,  0,  0,  0,  6,  0,  0,  7,  8,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  9,  0,  0, 10, 11,  0, 12,  0,  0,  0,  0,
     0,  0, 13,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_grapheme_link_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,
     0,  0,  1,  0,  0,  0,  2,  0,  0,  3,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  4,  0,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,
     6,  6,  0,  0,  0,  0,  7,  0,  0,  0,  0,  8,  0,  0,  0,  0,
     0,  0,  4,  0,  0,  2,  0,  9,  0,  0,  0, 10,  0,  0,  0,  0,
    11,  0,  0,  0,  0,  0,  4,  0,  0,  0, 12,  0,  0,  0,  8,  0,
     0,  0,  0,  0,  0,  0,  0,  1,  0, 10,  0,  0,  0,  0,  0,  0,
     0,  0, 11,  0,  0, 13,  0,  0,
};

static RE_UINT8 re_grapheme_link_stage_5[] = {
      0,   0,   0,   0,   0,  32,   0,   0,   0,   4,   0,   0,   0,   0,   0,   4,
     16,   0,   0,   0,   0,   0,   0,   6,   0,   0,  16,   0,   0,   0,   4,   0,
      1,   0,   0,   0,   0,   0,  12,   0,   0,   0,   0, 128,  64,   0,   0,   0,
      0,   0,   8,   0,   0,   0,   0,   2,
};

/* Grapheme_Link: 338 bytes. */

RE_UINT32 re_get_grapheme_link(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_grapheme_link_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_grapheme_link_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_grapheme_link_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_grapheme_link_stage_4[pos + f] << 5;
    pos += code;
    value = (re_grapheme_link_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* White_Space. */

static RE_UINT8 re_white_space_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_white_space_stage_2[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_white_space_stage_3[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 1, 1, 1,
    4, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_white_space_stage_4[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 3, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1,
    5, 6, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_white_space_stage_5[] = {
      0,  62,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     32,   0,   0,   0,   1,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,
      0,  64,   0,   0,   0,   0,   0,   0, 255,   7,   0,   0,   0, 131,   0,   0,
      0,   0,   0, 128,   0,   0,   0,   0,
};

/* White_Space: 185 bytes. */

RE_UINT32 re_get_white_space(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_white_space_stage_1[f] << 3;
    f = code >> 13;
    code ^= f << 13;
    pos = (RE_UINT32)re_white_space_stage_2[pos + f] << 4;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_white_space_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_white_space_stage_4[pos + f] << 6;
    pos += code;
    value = (re_white_space_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Bidi_Control. */

static RE_UINT8 re_bidi_control_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_bidi_control_stage_2[] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_bidi_control_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_bidi_control_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_bidi_control_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0, 192,   0,   0,   0, 124,   0,   0,
};

/* Bidi_Control: 97 bytes. */

RE_UINT32 re_get_bidi_control(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_bidi_control_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_bidi_control_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_bidi_control_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_bidi_control_stage_4[pos + f] << 6;
    pos += code;
    value = (re_bidi_control_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Join_Control. */

static RE_UINT8 re_join_control_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_join_control_stage_2[] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_join_control_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_join_control_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_join_control_stage_5[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0,  0,
};

/* Join_Control: 97 bytes. */

RE_UINT32 re_get_join_control(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_join_control_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_join_control_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_join_control_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_join_control_stage_4[pos + f] << 6;
    pos += code;
    value = (re_join_control_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Dash. */

static RE_UINT8 re_dash_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_dash_stage_2[] = {
    0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
};

static RE_UINT8 re_dash_stage_3[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 3, 1, 4, 1, 1, 1,
    5, 6, 1, 1, 1, 1, 1, 7, 8, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9,
};

static RE_UINT8 re_dash_stage_4[] = {
     0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  2,  1,  3,  1,  1,  1,  1,  1,  1,  1,
     4,  1,  1,  1,  1,  1,  1,  1,  5,  6,  7,  1,  1,  1,  1,  1,
     8,  1,  1,  1,  1,  1,  1,  1,  9,  1,  1,  1,  1,  1,  1,  1,
    10,  1, 11,  1,  1,  1,  1,  1, 12, 13,  1,  1, 14,  1,  1,  1,
};

static RE_UINT8 re_dash_stage_5[] = {
      0,   0,   0,   0,   0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   4,   0,   0,   0,   0,   0,  64,   1,   0,   0,   0,   0,   0,   0,   0,
     64,   0,   0,   0,   0,   0,   0,   0,   0,   0,  63,   0,   0,   0,   0,   0,
      0,   0,   8,   0,   0,   0,   0,   8,   0,   8,   0,   0,   0,   0,   0,   0,
      0,   0,   4,   0,   0,   0,   0,   0,   0,   0, 128,   4,   0,   0,   0,   0,
      0,   0,   0,  16,   0,   0,   1,   0,   0,   0,   0,   0,   1,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   6,   0,   0,   0,   0,   1,   8,   0,   0,   0,
      0,  32,   0,   0,   0,   0,   0,   0,
};

/* Dash: 297 bytes. */

RE_UINT32 re_get_dash(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_dash_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_dash_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_dash_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_dash_stage_4[pos + f] << 6;
    pos += code;
    value = (re_dash_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Hyphen. */

static RE_UINT8 re_hyphen_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_hyphen_stage_2[] = {
    0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
};

static RE_UINT8 re_hyphen_stage_3[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1,
    4, 1, 1, 1, 1, 1, 1, 5, 6, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7,
};

static RE_UINT8 re_hyphen_stage_4[] = {
    0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 1, 1,
    4, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 6, 1, 1, 1, 1, 1, 7, 1, 1, 8, 9, 1, 1,
};

static RE_UINT8 re_hyphen_stage_5[] = {
      0,   0,   0,   0,   0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   4,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   3,   0,   0,   0,   0,   0,   0,   0, 128,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   8,   0,   0,   0,   0,   8,   0,   0,   0,
      0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  32,   0,   0,   0,
};

/* Hyphen: 241 bytes. */

RE_UINT32 re_get_hyphen(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_hyphen_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_hyphen_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_hyphen_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_hyphen_stage_4[pos + f] << 6;
    pos += code;
    value = (re_hyphen_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Quotation_Mark. */

static RE_UINT8 re_quotation_mark_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_quotation_mark_stage_2[] = {
    0, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_quotation_mark_stage_3[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 4,
};

static RE_UINT8 re_quotation_mark_stage_4[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1,
    1, 5, 1, 1, 6, 7, 1, 1,
};

static RE_UINT8 re_quotation_mark_stage_5[] = {
      0,   0,   0,   0, 132,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   8,   0,   8,   0,   0,   0, 255,   0,   0,   0,   6,
      0, 240,   0, 224,   0,   0,   0,   0,  30,   0,   0,   0,   0,   0,   0,   0,
    132,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  12,   0,   0,   0,
};

/* Quotation_Mark: 193 bytes. */

RE_UINT32 re_get_quotation_mark(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_quotation_mark_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_quotation_mark_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_quotation_mark_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_quotation_mark_stage_4[pos + f] << 6;
    pos += code;
    value = (re_quotation_mark_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Terminal_Punctuation. */

static RE_UINT8 re_terminal_punctuation_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_terminal_punctuation_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  9, 10, 11,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9, 12, 13,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 14,
    15,  9, 16,  9, 17,  9,  9,  9,  9, 18,  9,  9,  9,  9,  9,  9,
};

static RE_UINT8 re_terminal_punctuation_stage_3[] = {
     0,  1,  1,  1,  1,  1,  2,  3,  1,  1,  1,  4,  5,  6,  7,  8,
     9,  1, 10,  1,  1,  1,  1,  1,  1,  1,  1,  1, 11,  1, 12,  1,
    13,  1,  1,  1,  1,  1, 14,  1,  1,  1,  1,  1, 15, 16,  1, 17,
    18,  1, 19,  1,  1, 20, 21,  1, 22,  1,  1,  1,  1,  1,  1,  1,
    23,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1, 24,  1,  1,  1, 25,  1,  1,  1,  1,  1,  1,  1,
     1, 26,  1,  1, 27, 28,  1,  1, 29, 30, 31, 32, 33, 34,  1, 35,
     1,  1,  1,  1, 36,  1, 37,  1,  1,  1,  1,  1,  1,  1,  1, 38,
    39,  1, 40,  1,  1,  1, 41,  1, 42, 43,  1,  1,  1,  1,  1,  1,
    44,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_terminal_punctuation_stage_4[] = {
     0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  3,  0,  0,  0,
     4,  0,  5,  0,  6,  0,  0,  0,  0,  0,  7,  0,  8,  0,  0,  0,
     0,  0,  0,  9,  0, 10,  2,  0,  0,  0,  0, 11,  0,  0, 12,  0,
    13,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0, 15,  0,  0,  0, 16,
     0,  0,  0, 17,  0,  0, 18,  0, 19,  0,  0,  0,  0,  0, 11,  0,
     0, 20,  0,  0,  0,  0, 21,  0,  0, 22,  0, 23,  0, 24, 25,  0,
     0, 26,  0,  0, 27,  0,  0,  0,  0,  0,  0, 23, 28,  0,  0,  0,
     0,  0,  0, 29,  0,  0,  0, 30,  0,  0, 31,  0,  0, 32,  0,  0,
     0,  0, 25,  0,  0,  0, 33,  0,  0,  0, 34,  0,  0,  0,  0, 35,
     0,  0, 36,  0,  1,  0,  0, 37, 34,  0, 38,  0,  0,  0, 39,  0,
    34,  0,  0,  0,  0, 40,  0,  0,  0,  0, 41,  0,  0, 23, 42,  0,
     0,  0,  0, 43,
};

static RE_UINT8 re_terminal_punctuation_stage_5[] = {
      0,   0,   0,   0,   2,  80,   0, 140,   0,   0,   0,  64, 128,   0,   0,   0,
      0,   2,   0,   0,   8,   0,   0,   0,   0,  16,   0, 136,   0,   0,  16,   0,
    255,  23,   0,   0,   0,   0,   0,   3,   0,   0, 255, 127,  48,   0,   0,   0,
      0,   0,   0,  12,   0, 225,   7,   0,   0,  12,   0,   0, 254,   1,   0,   0,
      0,  96,   0,   0,   0,  56,   0,   0,   0,   0, 112,   4,  60,   3,   0,   0,
      0,  15,   0,   0,   0,   0,   0, 236,   0,   0,   0, 248,   0,   0,   0, 192,
      0,   0,   0,  48, 128,   3,   0,   0,   0,  64,   0,   0,   6,   0,   0,   0,
      0, 224,   0,   0,   0,   0, 248,   0,   0,   0, 192,   0,   0, 192,   0,   0,
      0, 128,   0,   0,   0,   0,   0, 224,   0,   0,   0, 128,   0,   8,   0,   0,
      0,   0, 247,   0,  18,   0,   0,   0,   0,   0,   1,   0,   0,   0, 128,   0,
      0,   0,   0, 252, 128,  63,   0,   0,   3,   0,   0,   0,   0,   0,  15,   0,
};

/* Terminal_Punctuation: 656 bytes. */

RE_UINT32 re_get_terminal_punctuation(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_terminal_punctuation_stage_1[f] << 4;
    f = code >> 10;
    code ^= f << 10;
    pos = (RE_UINT32)re_terminal_punctuation_stage_2[pos + f] << 3;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_terminal_punctuation_stage_3[pos + f] << 2;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_terminal_punctuation_stage_4[pos + f] << 5;
    pos += code;
    value = (re_terminal_punctuation_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Other_Math. */

static RE_UINT8 re_other_math_stage_1[] = {
    0, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_other_math_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_other_math_stage_3[] = {
     0,  1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  1,
     3,  4,  5,  6,  1,  1,  1,  7,  1,  1,  1,  8,  9,  1,  1, 10,
     1,  1,  1, 11,  1,  1,  1,  1,  1,  1,  1,  1, 12,  1, 13,  1,
    14, 15, 16, 17, 17, 18, 19, 20,
};

static RE_UINT8 re_other_math_stage_4[] = {
     0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  2,  3,  4,  5,  6,  7,
     8,  0,  9, 10, 11, 12, 13,  0, 14, 15, 16, 17,  0, 18, 19, 20,
     0, 21, 22, 23, 24,  0, 25, 26,  0,  0, 24, 27, 28,  0, 29, 30,
     0,  0,  0, 31,  0, 32,  0,  0, 33, 33, 34, 33, 35, 36, 37, 33,
    38, 39, 40, 33, 33, 33, 33, 33, 33, 41, 42, 43, 34, 34, 44, 44,
    45, 45, 46, 33,
};

static RE_UINT8 re_other_math_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,  64,   0,   0,  39,   0,   0,   0,  51,   0,
      0,   0,  64,   0,   0,   0,  28,   0,   1,   0,   0,   0,  30,   0,   0,  96,
      0,  96,   0,   0,   0,   0, 255,  31,  98, 248,   0,   0, 132, 252,  47,  62,
     16, 179, 251, 241, 224,   3,   0,   0,   0,   0, 224, 243, 182,  62, 195, 240,
    255,  63, 235,  47,  48,   0,   0,   0,   0,   0, 176,   0,   0,   0,   1,   0,
      4,   0,   0,   0,   3, 192, 127, 240, 193, 140,  15,   0, 148,  31,   0,   0,
     96,   0,   0,   0,   5,   0,   0,   0,  15,  96,   0,   0, 192, 255,   0,   0,
    248, 255, 255,   1,   0,   0,   0,  15,   0,   0,   0,  48,  10,   1,   0,   0,
      0,   0,   0,  80, 255, 255, 255, 255, 255, 255, 223, 255, 255, 255, 255, 223,
    100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,
     95, 252, 253, 255,  63, 255, 255, 255, 253, 255, 255, 247, 255, 255, 255, 247,
    255, 127, 255, 255, 255, 253, 255, 255, 247, 207, 255, 255,
};

/* Other_Math: 460 bytes. */

RE_UINT32 re_get_other_math(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_other_math_stage_1[f] << 4;
    f = code >> 10;
    code ^= f << 10;
    pos = (RE_UINT32)re_other_math_stage_2[pos + f] << 3;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_other_math_stage_3[pos + f] << 2;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_other_math_stage_4[pos + f] << 5;
    pos += code;
    value = (re_other_math_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Hex_Digit. */

static RE_UINT8 re_hex_digit_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_hex_digit_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_hex_digit_stage_3[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 2,
};

static RE_UINT8 re_hex_digit_stage_4[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1,
};

static RE_UINT8 re_hex_digit_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 126,   0,   0,   0, 126,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0, 255,   3, 126,   0,   0,   0, 126,   0,   0,   0,   0,   0,   0,   0,
};

/* Hex_Digit: 129 bytes. */

RE_UINT32 re_get_hex_digit(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_hex_digit_stage_1[f] << 3;
    f = code >> 13;
    code ^= f << 13;
    pos = (RE_UINT32)re_hex_digit_stage_2[pos + f] << 3;
    f = code >> 10;
    code ^= f << 10;
    pos = (RE_UINT32)re_hex_digit_stage_3[pos + f] << 3;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_hex_digit_stage_4[pos + f] << 7;
    pos += code;
    value = (re_hex_digit_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* ASCII_Hex_Digit. */

static RE_UINT8 re_ascii_hex_digit_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_ascii_hex_digit_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_ascii_hex_digit_stage_3[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_ascii_hex_digit_stage_4[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_ascii_hex_digit_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 126,   0,   0,   0, 126,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

/* ASCII_Hex_Digit: 97 bytes. */

RE_UINT32 re_get_ascii_hex_digit(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_ascii_hex_digit_stage_1[f] << 3;
    f = code >> 13;
    code ^= f << 13;
    pos = (RE_UINT32)re_ascii_hex_digit_stage_2[pos + f] << 3;
    f = code >> 10;
    code ^= f << 10;
    pos = (RE_UINT32)re_ascii_hex_digit_stage_3[pos + f] << 3;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_ascii_hex_digit_stage_4[pos + f] << 7;
    pos += code;
    value = (re_ascii_hex_digit_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Other_Alphabetic. */

static RE_UINT8 re_other_alphabetic_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3,
};

static RE_UINT8 re_other_alphabetic_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  7,  6,  6,  6,  6,  6,  6,  6,  6,  6,  8,
     6,  9, 10,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_other_alphabetic_stage_3[] = {
     0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
    13,  0,  0, 14,  0,  0,  0, 15, 16, 17, 18, 19, 20,  0,  0,  0,
     0,  0,  0,  0, 21,  0,  0,  0,  0,  0,  0,  0,  0, 22,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 23, 24, 25, 26,  0,  0,  0,  0,
     0,  0,  0, 27,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,
    29,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_other_alphabetic_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  2,  3,  0,  4,  0,  5,  6,  0,  0,  7,  8,
     9, 10,  0,  0,  0, 11,  0,  0, 12, 13,  0,  0,  0,  0,  0,  0,
    14, 15, 16, 17, 18, 19, 20, 17, 18, 19, 21, 22, 18, 19, 23, 17,
    18, 19, 24, 17, 25, 19, 26,  0, 18, 19, 27, 17, 17, 19, 27, 17,
    17, 19, 28, 17, 17,  0, 29, 30,  0, 31, 32,  0,  0, 33, 32,  0,
     0,  0,  0, 34, 35, 36,  0,  0,  0, 37, 38, 39, 40,  0,  0,  0,
     0,  0, 41,  0,  0,  0,  0,  0, 30, 30, 30, 30,  0, 42, 43,  0,
     0,  0,  0,  0,  0, 44,  0,  0,  0, 45,  0,  0,  0, 10, 46,  0,
    47,  0, 48, 49,  0,  0,  0,  0, 50, 51, 14,  0, 52, 53,  0, 54,
     0, 55,  0,  0,  0,  0,  0, 56,  0,  0,  0,  0,  0, 42, 57, 58,
     0,  0,  0,  0,  0,  0,  0, 57,  0, 59,  0,  0, 60, 61, 14,  0,
     0, 62, 63,  0, 14, 61,  0,  0,  0, 64, 65,  0,  0, 66,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 67, 68,  0,  0,  0,  0,  0,  0,  0,
    69,  0,  0,  0,  0,  0,  0,  0, 52, 70, 71,  0, 25, 72,  0,  0,
};

static RE_UINT8 re_other_alphabetic_stage_5[] = {
      0,   0,   0,   0,  32,   0,   0,   0,   0,   0, 255, 191, 182,   0,   0,   0,
      0,   0, 255,   7,   0, 248, 255, 254,   0,   0,   1,   0,   0,   0, 192,  31,
    158,  33,   0,   0,   0,   0,   2,   0,   0,   0, 255, 255, 192, 255,   1,   0,
      0,   0, 192, 248, 239,  30,   0,   0,  15,   0,   0,   0,   0,   0,   0, 204,
    255, 223, 224,   0,  12,   0,   0,   0,  14,   0,   0,   0,   0,   0,   0, 192,
    159,  25, 128,   0, 135,  25,   2,   0,   0,   0,  35,   0, 191,  27,   0,   0,
    159,  25, 192,   0,   4,   0,   0,   0, 199,  29, 128,   0, 223,  29,  96,   0,
    223,  29, 128,   0,   0, 128,  95, 255,   0,   0,  12,   0,   0,   0, 242,   7,
      0,  32,   0,   0,   0,   0, 242,  27,   0,   0, 254, 255,   3, 224, 255, 254,
    255, 255, 255,  31,   0, 248, 127, 121,   0,   0, 192, 195, 133,   1,  30,   0,
    124,   0,   0,  48,   0,   0,   0, 128,   0,   0, 192, 255, 255,   1,   0,   0,
      0,   2,   0,   0, 255,  15, 255,   1,   1,   3,   0,   0,   0,   0, 128,  15,
      0,   0, 224, 127, 254, 255,  31,   0,  31,   0,   0,   0,   0,   0, 224, 255,
      7,   0,   0,   0, 254,   3,   0,   0, 128, 255,   3,   0, 240, 255,  63,   0,
      0,   0,   4,   0, 255, 255, 255, 255, 255,   3,   0,   0, 248,   0,   0,   0,
      3,   0,   0,   0,   0,   0, 240, 255, 192,   7,   0,   0, 128, 255,   7,   0,
      0, 254, 127,   0,   8,  48,   0,   0,   0,   0, 157,  65, 248,   7,   0,   0,
      0,   0,   0,  64, 110, 240,   0,   0,   0,   0,   0, 255,  63,   0,   0,   0,
      0,   0, 255,   1,
};

/* Other_Alphabetic: 718 bytes. */

RE_UINT32 re_get_other_alphabetic(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_other_alphabetic_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_other_alphabetic_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_other_alphabetic_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_other_alphabetic_stage_4[pos + f] << 5;
    pos += code;
    value = (re_other_alphabetic_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Ideographic. */

static RE_UINT8 re_ideographic_stage_1[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_ideographic_stage_2[] = {
    0, 0, 0, 1, 2, 3, 3, 3, 3, 4, 0, 0, 0, 0, 0, 5,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 6, 7, 0, 0, 0, 8,
};

static RE_UINT8 re_ideographic_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 4, 0, 0, 0, 0, 5, 6, 0, 0,
    2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 8, 9, 0, 0, 0,
    0, 0, 0, 0, 2, 9, 0, 0,
};

static RE_UINT8 re_ideographic_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0,
    2, 2, 2, 2, 2, 2, 2, 4, 0, 0, 0, 0, 2, 2, 2, 2,
    5, 5, 2, 6, 0, 0, 0, 0, 2, 2, 2, 7, 2, 2, 2, 2,
    2, 2, 2, 2, 8, 2, 2, 2, 9, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_ideographic_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0, 192,   0,   0,   0, 254,   3,   0,   7,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  63,   0,
    255,  15,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255,  63, 255, 255,
    255, 255, 255,   3,   0,   0,   0,   0, 255, 255, 127,   0,   0,   0,   0,   0,
    255, 255, 255, 255, 255, 255,  31,   0, 255, 255, 255,  63,   0,   0,   0,   0,
};

/* Ideographic: 297 bytes. */

RE_UINT32 re_get_ideographic(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_ideographic_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_ideographic_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_ideographic_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_ideographic_stage_4[pos + f] << 6;
    pos += code;
    value = (re_ideographic_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Diacritic. */

static RE_UINT8 re_diacritic_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_diacritic_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  7,  8,  4,  4,  4,  4,  4,  4,  4,  4,  4,  9,
     4,  4, 10,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 11,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
};

static RE_UINT8 re_diacritic_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1,  1,  1,  1,  1,  1, 17,  1, 18, 19, 20, 21, 22,  1, 23,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 24,  1, 25,  1,
    26,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 27, 28,
    29, 30, 31, 32,  1,  1,  1,  1,  1,  1,  1, 33,  1,  1, 34, 35,
    36,  1,  1,  1,  1,  1,  1,  1,  1, 37,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_diacritic_stage_4[] = {
     0,  0,  1,  2,  0,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  4,  5,  5,  5,  5,  6,  7,  8,  0,  0,  0,
     0,  0,  0,  0,  9,  0,  0,  0,  0,  0, 10,  0, 11, 12, 13,  0,
     0,  0, 14,  0,  0,  0, 15, 16,  0,  4, 17,  0,  0, 18,  0, 19,
    20,  0,  0,  0,  0,  0,  0,  0,  0, 21, 22, 23,  0, 21, 24,  0,
     0, 21, 24,  0,  0, 21, 24,  0,  0, 21, 24,  0,  0,  0, 24,  0,
     0,  0, 24,  0,  0, 21, 24,  0,  0,  0, 24,  0,  0,  0, 25,  0,
     0,  0, 26,  0,  0,  0, 27,  0, 20, 28,  0,  0, 29,  0, 30,  0,
     0, 31,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,  0, 33,  0,
     0, 34,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,
     0, 36, 37, 38,  0, 25,  0,  0,  0, 39,  0, 40,  0,  0,  4, 41,
     0, 42,  5, 17,  0,  0, 43, 44,  0,  0,  0,  0,  0, 45, 46, 47,
     0,  0,  0,  0,  0,  0,  0, 48,  0, 49,  0,  0,  0,  0,  0,  0,
     0, 50,  0,  0, 51,  0,  0, 21,  0,  0,  0, 52,  0,  0,  0, 53,
    54, 55,  0,  0, 56,  0,  0,  0,  0,  0,  0,  0,  0,  0, 37, 57,
     0, 58, 59,  0,  0, 59,  2,  0,  0,  0,  0, 60,  0, 15, 61,  0,
     0,  0,  0,  0,  0,  0,  0, 62,  1,  0,  0,  0,  0,  0,  0,  0,
     0, 63,  0,  0,  0,  0,  0,  0,  0,  1,  2, 64, 65,  0,  0, 66,
     0,  0,  0,  0,  0, 67,  0,  0,  0,  0,  0, 68, 69, 70,  0,  0,
};

static RE_UINT8 re_diacritic_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,  64,   1,   0,   0,   0,   0, 129, 144,   1,
      0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 255, 224,   7,   0,  48,   4,
     48,   0,   0,   0, 248,   0,   0,   0,   0,   0,   0,   2,   0,   0, 254, 255,
    251, 255, 255, 191,  22,   0,   0,   0,   0, 248, 135,   1,   0,   0,   0, 128,
     97,  28,   0,   0, 255,   7,   0,   0, 192, 255,   1,   0,   0, 248,  63,   0,
      0,   0,   0,   3,   0,   0,   0,  16,   0,  32,  30,   0,   0,   0,   2,   0,
      0,  32,   0,   0,   0,   4,   0,   0, 128,  95,   0,   0,   0,  31,   0,   0,
      0,   0, 160, 194, 220,   0,   0,   0,  64,   0,   0,   0,   0,   0, 128,   6,
    128, 191,   0,  12,   0, 254,  15,  32,   0,   0,   0,  14,   0,   0, 224, 159,
      0,   0,  16,   0,  16,   0,   0,   0,   0, 248,  15,   0,   0,   0, 192,   0,
      0,   0,   0,  63, 255,  33,   0,   0,   0, 240, 255, 255, 240, 255,   0,   0,
      0,   0,   0, 224,   0,   0,   0, 160,   3, 224,   0, 224,   0, 224,   0,  96,
      0, 128,   3,   0,   0, 128,   0,   0,   0, 252,   0,   0,   0,   0,   0,  30,
      0, 128,   0, 176,   0,   0,   3,   0,   0,   0, 128, 255,   3,   0,   0,   0,
      0,   1,   0,   0, 255, 255,   3,   0,   0, 120,   0,   0,   0,   0,   8,   0,
      0,   0,   0,   8,   7,   0,   0,   0,   0,  48,   0,   0, 127,   0,   0,   0,
      0,   0,   1,   0,   0,   0,   0, 192,   8,   0,   0,   0,   0,   0,   0,   6,
    128, 227,   7, 248, 231,  15,   0,   0,   0,  60,   0,   0,
};

/* Diacritic: 797 bytes. */

RE_UINT32 re_get_diacritic(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_diacritic_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_diacritic_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_diacritic_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_diacritic_stage_4[pos + f] << 5;
    pos += code;
    value = (re_diacritic_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Extender. */

static RE_UINT8 re_extender_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_extender_stage_2[] = {
    0, 1, 2, 3, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 5, 6, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_extender_stage_3[] = {
     0,  1,  2,  1,  1,  1,  3,  4,  1,  1,  1,  1,  1,  1,  5,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  6,  1,  7,  1,  8,  1,  1,  1,
     9,  1,  1,  1,  1,  1,  1,  1, 10,  1,  1,  1,  1,  1, 11,  1,
     1, 12, 13,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 14,
};

static RE_UINT8 re_extender_stage_4[] = {
     0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  3,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  5,  0,  0,  0,  5,  0,
     0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,
     0,  8,  0,  9,  0,  0,  0,  0, 10, 11,  0,  0, 12,  0,  0, 13,
    14,  0,  0,  0,  0,  0,  0,  0, 15,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0, 17,  0,  0, 18,  0,
     0,  0,  0, 17,  0,  0,  0,  0,
};

static RE_UINT8 re_extender_stage_5[] = {
      0,   0,   0,   0,   0,   0, 128,   0,   0,   0,   3,   0,   1,   0,   0,   0,
      0,   0,   0,   4,  64,   0,   0,   0,   8,   0,   0,   0, 128,   0,   0,   0,
      0,   0,  64,   0,   0,   0,   0,   8,  32,   0,   0,   0,   0,   0,  62,   0,
      0,   0,   0,  96,   0,   0,   0, 112,   0,   0,  32,   0,   0,  16,   0,   0,
      0, 128,   0,   0,   0,   0,   1,   0,   0,   0,   0,  32,
};

/* Extender: 341 bytes. */

RE_UINT32 re_get_extender(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_extender_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_extender_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_extender_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_extender_stage_4[pos + f] << 5;
    pos += code;
    value = (re_extender_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Other_Lowercase. */

static RE_UINT8 re_other_lowercase_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_other_lowercase_stage_2[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

static RE_UINT8 re_other_lowercase_stage_3[] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
    3, 0, 4, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 6, 0, 0, 0, 0,
};

static RE_UINT8 re_other_lowercase_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  0,  3,  0,  0,
     0,  0,  0,  0,  4,  5,  6,  0,  0,  0,  7,  0,  0,  8,  0,  0,
     0,  0,  0,  9,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 11,  0,  0,
};

static RE_UINT8 re_other_lowercase_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255,   1,
      3,   0,   0,   0,  31,   0,   0,   0,  32,   0,   0,   0,   0,   0,   0,   4,
      0,   0,   0,   0,   0, 240, 255, 255, 255, 255, 255, 255,   3,   0,   0,   1,
      0,   0,   0, 248, 255, 255, 255, 255,   0,   0,  31,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255, 255,   0,   0, 255, 255, 255,   3,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  32,   0,   0,   0,   0,   0,   0,   1,   0,
};

/* Other_Lowercase: 241 bytes. */

RE_UINT32 re_get_other_lowercase(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_other_lowercase_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_other_lowercase_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_other_lowercase_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_other_lowercase_stage_4[pos + f] << 6;
    pos += code;
    value = (re_other_lowercase_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Other_Uppercase. */

static RE_UINT8 re_other_uppercase_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_other_uppercase_stage_2[] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_uppercase_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_uppercase_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 1, 0,
};

static RE_UINT8 re_other_uppercase_stage_5[] = {
      0,   0,   0,   0, 255, 255,   0,   0,   0,   0, 192, 255,
};

/* Other_Uppercase: 117 bytes. */

RE_UINT32 re_get_other_uppercase(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_other_uppercase_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_other_uppercase_stage_2[pos + f] << 4;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_other_uppercase_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_other_uppercase_stage_4[pos + f] << 5;
    pos += code;
    value = (re_other_uppercase_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Noncharacter_Code_Point. */

static RE_UINT8 re_noncharacter_code_point_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_noncharacter_code_point_stage_2[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
};

static RE_UINT8 re_noncharacter_code_point_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
    0, 0, 0, 0, 0, 0, 0, 2,
};

static RE_UINT8 re_noncharacter_code_point_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 2,
};

static RE_UINT8 re_noncharacter_code_point_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 192,
};

/* Noncharacter_Code_Point: 121 bytes. */

RE_UINT32 re_get_noncharacter_code_point(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_noncharacter_code_point_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_noncharacter_code_point_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_noncharacter_code_point_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_noncharacter_code_point_stage_4[pos + f] << 6;
    pos += code;
    value = (re_noncharacter_code_point_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Other_Grapheme_Extend. */

static RE_UINT8 re_other_grapheme_extend_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_other_grapheme_extend_stage_2[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_other_grapheme_extend_stage_3[] = {
    0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
    6, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_grapheme_extend_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
    0, 0, 0, 0, 1, 2, 1, 2, 0, 0, 0, 3, 1, 2, 0, 4,
    5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0,
    0, 0, 0, 0, 0, 7, 0, 0,
};

static RE_UINT8 re_other_grapheme_extend_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  64,
      0,   0, 128,   0,   0,   0,   0,   0,   4,   0,  96,   0,   0,   0,   0,   0,
      0, 128,   0, 128,   0,   0,   0,   0,   0,  48,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 192,   0,   0,   0,   0,   0,   0,   0,   0,  32, 192,   7,   0,
};

/* Other_Grapheme_Extend: 225 bytes. */

RE_UINT32 re_get_other_grapheme_extend(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_other_grapheme_extend_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_other_grapheme_extend_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_other_grapheme_extend_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_other_grapheme_extend_stage_4[pos + f] << 6;
    pos += code;
    value = (re_other_grapheme_extend_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* IDS_Binary_Operator. */

static RE_UINT8 re_ids_binary_operator_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_ids_binary_operator_stage_2[] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_ids_binary_operator_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static RE_UINT8 re_ids_binary_operator_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static RE_UINT8 re_ids_binary_operator_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 243,  15,
};

/* IDS_Binary_Operator: 97 bytes. */

RE_UINT32 re_get_ids_binary_operator(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_ids_binary_operator_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_ids_binary_operator_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_ids_binary_operator_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_ids_binary_operator_stage_4[pos + f] << 6;
    pos += code;
    value = (re_ids_binary_operator_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* IDS_Trinary_Operator. */

static RE_UINT8 re_ids_trinary_operator_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_ids_trinary_operator_stage_2[] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_ids_trinary_operator_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static RE_UINT8 re_ids_trinary_operator_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static RE_UINT8 re_ids_trinary_operator_stage_5[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12,  0,
};

/* IDS_Trinary_Operator: 97 bytes. */

RE_UINT32 re_get_ids_trinary_operator(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_ids_trinary_operator_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_ids_trinary_operator_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_ids_trinary_operator_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_ids_trinary_operator_stage_4[pos + f] << 6;
    pos += code;
    value = (re_ids_trinary_operator_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Radical. */

static RE_UINT8 re_radical_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_radical_stage_2[] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_radical_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static RE_UINT8 re_radical_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 2, 2, 3, 2, 2, 2, 2, 2, 2, 4, 0,
};

static RE_UINT8 re_radical_stage_5[] = {
      0,   0,   0,   0, 255, 255, 255, 251, 255, 255, 255, 255, 255, 255,  15,   0,
    255, 255,  63,   0,
};

/* Radical: 117 bytes. */

RE_UINT32 re_get_radical(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_radical_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_radical_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_radical_stage_3[pos + f] << 4;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_radical_stage_4[pos + f] << 5;
    pos += code;
    value = (re_radical_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Unified_Ideograph. */

static RE_UINT8 re_unified_ideograph_stage_1[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_unified_ideograph_stage_2[] = {
    0, 0, 0, 1, 2, 3, 3, 3, 3, 4, 0, 0, 0, 0, 0, 5,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 6, 7, 0, 0, 0, 0,
};

static RE_UINT8 re_unified_ideograph_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 4, 0, 0,
    1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 6, 7, 0, 0, 0,
};

static RE_UINT8 re_unified_ideograph_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1, 3,
    4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 5, 1, 1, 1, 1,
    1, 1, 1, 1, 6, 1, 1, 1, 7, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_unified_ideograph_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,  63,   0, 255,  15,   0,   0,   0,   0,   0,   0,
      0, 192,  26, 128, 154,   3,   0,   0, 255, 255, 127,   0,   0,   0,   0,   0,
    255, 255, 255, 255, 255, 255,  31,   0, 255, 255, 255,  63,   0,   0,   0,   0,
};

/* Unified_Ideograph: 257 bytes. */

RE_UINT32 re_get_unified_ideograph(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_unified_ideograph_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_unified_ideograph_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_unified_ideograph_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_unified_ideograph_stage_4[pos + f] << 6;
    pos += code;
    value = (re_unified_ideograph_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Other_Default_Ignorable_Code_Point. */

static RE_UINT8 re_other_default_ignorable_code_point_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1,
};

static RE_UINT8 re_other_default_ignorable_code_point_stage_2[] = {
    0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
};

static RE_UINT8 re_other_default_ignorable_code_point_stage_3[] = {
    0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
    6, 7, 7, 7, 7, 7, 7, 7,
};

static RE_UINT8 re_other_default_ignorable_code_point_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6,
    7, 0, 8, 8, 0, 0, 0, 9, 8, 8, 8, 8, 8, 8, 8, 8,
};

static RE_UINT8 re_other_default_ignorable_code_point_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 128,   1,   0,   0,   0,   0,   0,   0,   0, 224,   3,   0,   0,
      0,   0,   0,   0,  16,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   1, 253, 255, 255, 255,   0,   0,   0,   0,
    255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0, 255, 255,
};

/* Other_Default_Ignorable_Code_Point: 265 bytes. */

RE_UINT32 re_get_other_default_ignorable_code_point(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_other_default_ignorable_code_point_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_other_default_ignorable_code_point_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_other_default_ignorable_code_point_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_other_default_ignorable_code_point_stage_4[pos + f] << 6;
    pos += code;
    value = (re_other_default_ignorable_code_point_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Deprecated. */

static RE_UINT8 re_deprecated_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1,
    1, 1,
};

static RE_UINT8 re_deprecated_stage_2[] = {
    0, 1, 2, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

static RE_UINT8 re_deprecated_stage_3[] = {
    0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 0, 0, 6, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_deprecated_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,
    0, 6, 0, 0, 0, 0, 0, 0, 7, 8, 8, 8, 0, 0, 0, 0,
};

static RE_UINT8 re_deprecated_stage_5[] = {
      0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   8,   0,   0,   0, 128,   2,
     24,   0,   0,   0,   0, 252,   0,   0,   0,   6,   0,   0,   2,   0,   0,   0,
    255, 255, 255, 255,
};

/* Deprecated: 230 bytes. */

RE_UINT32 re_get_deprecated(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_deprecated_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_deprecated_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_deprecated_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_deprecated_stage_4[pos + f] << 5;
    pos += code;
    value = (re_deprecated_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Soft_Dotted. */

static RE_UINT8 re_soft_dotted_stage_1[] = {
    0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1,
};

static RE_UINT8 re_soft_dotted_stage_2[] = {
    0, 1, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_soft_dotted_stage_3[] = {
     0,  1,  2,  3,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
     5,  5,  5,  5,  5,  6,  7,  5,  8,  9,  5,  5,  5,  5,  5,  5,
     5,  5,  5,  5, 10,  5,  5,  5,  5,  5,  5,  5, 11, 12, 13,  5,
};

static RE_UINT8 re_soft_dotted_stage_4[] = {
     0,  0,  0,  1,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,
     0,  0,  3,  4,  5,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,
     0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  9, 10, 11,  0,  0,  0, 12,  0,  0,  0,  0, 13,  0,
     0,  0,  0, 14,  0,  0,  0,  0,  0,  0, 15,  0,  0,  0,  0,  0,
     0,  0,  0, 16,  0,  0,  0,  0,  0, 17, 18,  0, 19, 20,  0, 21,
     0, 22, 23,  0, 24,  0, 17, 18,  0, 19, 20,  0, 21,  0,  0,  0,
};

static RE_UINT8 re_soft_dotted_stage_5[] = {
      0,   0,   0,   0,   0,   6,   0,   0,   0, 128,   0,   0,   0,   2,   0,   0,
      0,   1,   0,   0,   0,   0,   0,  32,   0,   0,   4,   0,   0,   0,   8,   0,
      0,   0,  64,   1,   4,   0,   0,   0,   0,   0,  64,   0,  16,   1,   0,   0,
      0,  32,   0,   0,   0,   8,   0,   0,   0,   0,   2,   0,   0,   3,   0,   0,
      0,   0,   0,  16,  12,   0,   0,   0,   0,   0, 192,   0,   0,  12,   0,   0,
      0,   0,   0, 192,   0,   0,  12,   0, 192,   0,   0,   0,   0,   0,   0,  12,
      0, 192,   0,   0,
};

/* Soft_Dotted: 342 bytes. */

RE_UINT32 re_get_soft_dotted(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_soft_dotted_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_soft_dotted_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_soft_dotted_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_soft_dotted_stage_4[pos + f] << 5;
    pos += code;
    value = (re_soft_dotted_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Logical_Order_Exception. */

static RE_UINT8 re_logical_order_exception_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_logical_order_exception_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_logical_order_exception_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 0, 0,
};

static RE_UINT8 re_logical_order_exception_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
    0, 0, 2, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_logical_order_exception_stage_5[] = {
     0,  0,  0,  0,  0,  0,  0,  0, 31,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 96, 26,
};

/* Logical_Order_Exception: 121 bytes. */

RE_UINT32 re_get_logical_order_exception(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_logical_order_exception_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_logical_order_exception_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_logical_order_exception_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_logical_order_exception_stage_4[pos + f] << 6;
    pos += code;
    value = (re_logical_order_exception_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Other_ID_Start. */

static RE_UINT8 re_other_id_start_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_other_id_start_stage_2[] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_id_start_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_id_start_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 2, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_id_start_stage_5[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0, 64,  0,  0,
     0,  0,  0, 24,  0,  0,  0,  0,
};

/* Other_ID_Start: 113 bytes. */

RE_UINT32 re_get_other_id_start(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_other_id_start_stage_1[f] << 3;
    f = code >> 13;
    code ^= f << 13;
    pos = (RE_UINT32)re_other_id_start_stage_2[pos + f] << 4;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_other_id_start_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_other_id_start_stage_4[pos + f] << 6;
    pos += code;
    value = (re_other_id_start_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Other_ID_Continue. */

static RE_UINT8 re_other_id_continue_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_other_id_continue_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_other_id_continue_stage_3[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 4, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_other_id_continue_stage_4[] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 4,
};

static RE_UINT8 re_other_id_continue_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,
    128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 254,   3,   0,
      0,   0,   0,   4,   0,   0,   0,   0,
};

/* Other_ID_Continue: 145 bytes. */

RE_UINT32 re_get_other_id_continue(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_other_id_continue_stage_1[f] << 3;
    f = code >> 13;
    code ^= f << 13;
    pos = (RE_UINT32)re_other_id_continue_stage_2[pos + f] << 4;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_other_id_continue_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_other_id_continue_stage_4[pos + f] << 6;
    pos += code;
    value = (re_other_id_continue_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* STerm. */

static RE_UINT8 re_sterm_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_sterm_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  3,  3,  9, 10,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3, 11, 12,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 13,
     3,  3, 14,  3, 15,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
};

static RE_UINT8 re_sterm_stage_3[] = {
     0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  3,  4,  5,  6,  7,
     1,  1,  8,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     9,  1,  1,  1,  1,  1, 10,  1,  1,  1,  1,  1, 11,  1, 12,  1,
    13,  1, 14,  1,  1, 15, 16,  1, 17,  1,  1,  1,  1,  1,  1,  1,
    18,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 19,  1,  1,  1,
    20,  1,  1,  1,  1,  1,  1,  1,  1, 21,  1,  1, 22, 23,  1,  1,
    24, 25, 26, 27, 28,  1,  1, 29,  1,  1,  1,  1, 30,  1, 31,  1,
     1,  1,  1,  1, 32,  1,  1,  1, 33, 34,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_sterm_stage_4[] = {
     0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  3,  0,  0,  0,
     4,  0,  0,  0,  0,  0,  5,  0,  6,  0,  0,  0,  0,  0,  0,  7,
     0,  0,  0,  8,  0,  0,  9,  0,  0,  0,  0, 10,  0,  0,  0, 11,
     0, 12,  0,  0, 13,  0,  0,  0,  0,  0,  8,  0,  0, 14,  0,  0,
     0,  0, 15,  0,  0, 16,  0, 17,  0, 18, 19,  0,  0, 11,  0,  0,
    20,  0,  0,  0,  0,  0,  0,  4, 21,  0,  0,  0,  0,  0,  0, 22,
     0,  0,  0, 23,  0,  0, 21,  0,  0, 24,  0,  0,  0,  0, 25,  0,
     0,  0, 26,  0,  0,  0,  0, 27,  0,  0, 28,  0,  1,  0,  0, 29,
     0,  0, 23,  0,  0,  0, 30,  0,  0, 17, 31,  0,
};

static RE_UINT8 re_sterm_stage_5[] = {
      0,   0,   0,   0,   2,  64,   0, 128,   0,   0,   0,  80,   0,   2,   0,   0,
      0,   0,   0, 128,   0,   0,  16,   0,   7,   0,   0,   0,   0,   0,   0,   2,
     48,   0,   0,   0,   0,  12,   0,   0, 132,   1,   0,   0,   0,  64,   0,   0,
      0,   0,  96,   0,   8,   2,   0,   0,   0,  15,   0,   0,   0,   0,   0, 204,
      0,   0,   0,  24,   0,   0,   0, 192,   0,   0,   0,  48, 128,   3,   0,   0,
      4,   0,   0,   0,   0, 192,   0,   0,   0,   0, 136,   0,   0,   0, 192,   0,
      0, 128,   0,   0,   0,   3,   0,   0,   0,   0,   0, 224,   0,   8,   0,   0,
      0,   0, 196,   0,   2,   0,   0,   0, 128,   1,   0,   0,   3,   0,   0,   0,
};

/* STerm: 544 bytes. */

RE_UINT32 re_get_sterm(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_sterm_stage_1[f] << 4;
    f = code >> 10;
    code ^= f << 10;
    pos = (RE_UINT32)re_sterm_stage_2[pos + f] << 3;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_sterm_stage_3[pos + f] << 2;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_sterm_stage_4[pos + f] << 5;
    pos += code;
    value = (re_sterm_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Variation_Selector. */

static RE_UINT8 re_variation_selector_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1,
};

static RE_UINT8 re_variation_selector_stage_2[] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_variation_selector_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_variation_selector_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 4,
};

static RE_UINT8 re_variation_selector_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,  56,   0,   0,   0,   0,   0,   0,
    255, 255,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,   0,   0,
};

/* Variation_Selector: 169 bytes. */

RE_UINT32 re_get_variation_selector(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_variation_selector_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_variation_selector_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_variation_selector_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_variation_selector_stage_4[pos + f] << 6;
    pos += code;
    value = (re_variation_selector_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Pattern_White_Space. */

static RE_UINT8 re_pattern_white_space_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_pattern_white_space_stage_2[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_pattern_white_space_stage_3[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_pattern_white_space_stage_4[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_pattern_white_space_stage_5[] = {
      0,  62,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     32,   0,   0,   0,   0,   0,   0,   0,   0, 192,   0,   0,   0,   3,   0,   0,
};

/* Pattern_White_Space: 129 bytes. */

RE_UINT32 re_get_pattern_white_space(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_pattern_white_space_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_pattern_white_space_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_pattern_white_space_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_pattern_white_space_stage_4[pos + f] << 6;
    pos += code;
    value = (re_pattern_white_space_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Pattern_Syntax. */

static RE_UINT8 re_pattern_syntax_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_pattern_syntax_stage_2[] = {
    0, 1, 1, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_pattern_syntax_stage_3[] = {
     0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     2,  3,  4,  4,  5,  4,  4,  6,  4,  4,  4,  4,  1,  1,  7,  1,
     8,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  9, 10,  1,
};

static RE_UINT8 re_pattern_syntax_stage_4[] = {
     0,  1,  2,  2,  0,  3,  4,  4,  0,  0,  0,  0,  0,  0,  0,  0,
     5,  6,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  8,  8,  8,
     8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,
     8,  8,  8,  9, 10,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,
    11, 12,  0,  0,  0,  0,  0,  0,  0, 13,  0,  0,  0,  0,  0,  0,
     0,  0, 14,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_pattern_syntax_stage_5[] = {
      0,   0,   0,   0, 254, 255,   0, 252,   1,   0,   0, 120, 254,  90,  67, 136,
      0,   0, 128,   0,   0,   0, 255, 255, 255,   0, 255, 127, 254, 255, 239, 127,
    255, 255, 255, 255, 255, 255,  63,   0,   0,   0, 240, 255,  14, 255, 255, 255,
      1,   0,   1,   0,   0,   0,   0, 192,  96,   0,   0,   0,
};

/* Pattern_Syntax: 277 bytes. */

RE_UINT32 re_get_pattern_syntax(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_pattern_syntax_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_pattern_syntax_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_pattern_syntax_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_pattern_syntax_stage_4[pos + f] << 5;
    pos += code;
    value = (re_pattern_syntax_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Hangul_Syllable_Type. */

static RE_UINT8 re_hangul_syllable_type_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_hangul_syllable_type_stage_2[] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_hangul_syllable_type_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  3,  0,  0,  0,  0,  0,  4,  5,  6,  7,  8,  9, 10,  4,
     5,  6,  7,  8,  9, 10,  4,  5,  6,  7,  8,  9, 10,  4,  5,  6,
     7,  8,  9, 10,  4,  5,  6,  7,  8,  9, 10,  4,  5,  6,  7,  8,
     9, 10,  4,  5,  6,  7,  8,  9, 10,  4,  5,  6,  7,  8,  9, 10,
     4,  5,  6,  7,  8,  9, 10,  4,  5,  6,  7,  8,  9, 10,  4,  5,
     6,  7,  8,  9, 10,  4,  5,  6,  7,  8,  9, 10,  4,  5,  6, 11,
};

static RE_UINT8 re_hangul_syllable_type_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  4,
     5,  6,  6,  7,  6,  6,  6,  5,  6,  6,  7,  6,  6,  6,  5,  6,
     6,  7,  6,  6,  6,  5,  6,  6,  7,  6,  6,  6,  5,  6,  6,  7,
     6,  6,  6,  5,  6,  6,  7,  6,  6,  6,  5,  6,  6,  7,  6,  6,
     6,  5,  6,  6,  7,  6,  6,  6,  5,  6,  6,  7,  6,  6,  6,  5,
     6,  6,  7,  6,  6,  6,  5,  6,  6,  7,  6,  6,  6,  5,  6,  6,
     7,  6,  6,  6,  5,  6,  6,  7,  6,  6,  6,  5,  6,  6,  7,  6,
     6,  6,  5,  6,  6,  7,  6,  6,  6,  5,  6,  6,  7,  6,  6,  6,
     6,  5,  6,  6,  8,  0,  2,  2,  9, 10,  3,  3,  3,  3,  3, 11,
};

static RE_UINT8 re_hangul_syllable_type_stage_5[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    1, 1, 1, 1, 1, 0, 0, 0, 4, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5, 5,
    5, 5, 5, 5, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0,
};

/* Hangul_Syllable_Type: 497 bytes. */

RE_UINT32 re_get_hangul_syllable_type(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_hangul_syllable_type_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_hangul_syllable_type_stage_2[pos + f] << 4;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_hangul_syllable_type_stage_3[pos + f] << 4;
    f = code >> 3;
    code ^= f << 3;
    pos = (RE_UINT32)re_hangul_syllable_type_stage_4[pos + f] << 3;
    value = re_hangul_syllable_type_stage_5[pos + code];

    return value;
}

/* Bidi_Class. */

static RE_UINT8 re_bidi_class_stage_1[] = {
     0,  1,  2,  3,  4,  5,  5,  5,  5,  5,  6,  5,  5,  5,  5,  7,
     8,  9,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 10, 11, 12,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
    14,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
};

static RE_UINT8 re_bidi_class_stage_2[] = {
      0,   1,   2,   2,   2,   3,   4,   5,   2,   6,   2,   7,   8,   9,  10,  11,
     12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
     28,  29,   2,   2,   2,   2,  30,  31,  32,   2,   2,   2,   2,  33,  34,  35,
     36,  37,  38,  39,  40,   2,  41,  42,  43,  44,   2,  45,   2,   2,   2,  46,
     47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  52,  52,  52,  57,  58,  59,
      2,   2,  52,  52,  52,  52,  60,   2,   2,  61,  62,  63,  64,  65,  52,  66,
     67,  68,   2,  69,  70,  71,  72,  73,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  74,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,  75,   2,   2,  76,  77,  78,  79,
     80,  81,  82,  83,  84,  85,   2,  86,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,  87,  88,  88,  88,  89,  90,  91,  92,  93,  94,
      2,   2,  95,  96,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
     13,  13,  97,  13,  98,  13,  99,  13,  13,  13,  13,  13, 100,  13,  13,  13,
    101, 102,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2, 103, 104, 105,   2, 106,   2,   2,   2,   2,   2,   2, 107, 108, 109,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
     13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,
    110, 111, 112,   2,   2,   2, 113, 114, 115, 116, 117, 118, 119, 120, 121,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 122,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 122,
    123, 123, 124, 125, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
    123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
};

static RE_UINT8 re_bidi_class_stage_3[] = {
      0,   1,   2,   3,   4,   5,   4,   6,   7,   8,   9,  10,  11,  12,  11,  12,
     11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  13,  14,  14,  15,  16,
     17,  17,  17,  17,  17,  17,  17,  18,  19,  11,  11,  11,  11,  11,  11,  20,
     21,  11,  11,  11,  11,  11,  11,  11,  22,  23,  17,  24,  25,  26,  26,  26,
     27,  28,  29,  29,  30,  17,  31,  32,  29,  29,  29,  29,  29,  33,  34,  35,
     36,  37,  29,  17,  28,  29,  29,  29,  29,  29,  38,  32,  26,  26,  39,  40,
     26,  41,  42,  26,  26,  43,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     44,  11,  11,  45,  46,  47,  48,  11,  49,  11,  11,  50,  51,  11,  48,  52,
     53,  11,  11,  50,  54,  49,  11,  55,  53,  11,  11,  50,  56,  11,  48,  57,
     49,  11,  11,  58,  51,  59,  48,  11,  60,  11,  11,  11,  61,  11,  11,  62,
     11,  11,  11,  63,  64,  65,  48,  66,  11,  11,  11,  50,  67,  11,  48,  11,
     11,  11,  11,  11,  51,  11,  48,  11,  11,  11,  11,  11,  68,  69,  11,  11,
     11,  11,  11,  70,  71,  11,  11,  11,  11,  11,  11,  72,  73,  11,  11,  11,
     11,  74,  11,  75,  11,  11,  11,  76,  77,  78,  17,  79,  59,  11,  11,  11,
     11,  11,  80,  81,  11,  82,  83,  84,  85,  86,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  11,  80,  11,  11,  11,  87,  11,  11,  11,  11,  11,  11,
      4,  11,  11,  11,  11,  11,  11,  11,  88,  89,  11,  11,  11,  11,  11,  11,
     11,  90,  11,  90,  11,  48,  11,  48,  11,  11,  11,  91,  92,  93,  11,  87,
     94,  11,  11,  11,  11,  11,  11,  11,  11,  11,  95,  11,  11,  11,  11,  11,
     11,  11,  96,  97,  98,  11,  11,  11,  11,  11,  11,  11,  11,  99,  16,  16,
     11, 100,  11,  11,  11, 101, 102, 103, 104,  11,  11, 105,  60,  11, 106, 104,
    107,  11, 108,  11,  11,  11, 109, 107,  11,  11, 110, 111,  11,  11,  11,  11,
     11,  11,  11,  11,  11, 112, 113,  11,  11,  11,  11,  11,  17,  17, 114, 110,
     11,  11,  11, 115, 116, 117, 117, 118, 119,  16, 120, 121, 122, 123,   8, 124,
    125,  11, 126, 127,  11,  17,  17,  83, 128, 129, 130, 131, 132,  16,  11,  11,
    133,  16,  16,  16,  16,  16,  16,  16,  16, 134,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16, 135,  11,  11,  11,   5,
     16, 136,  16,  16,  16,  16,  16, 137,  16,  16, 138,  11, 139,  11,  16,  16,
    140, 141,  11,  11,  11,  11, 142,  16,  16,  16, 143,  16,  16,  16,  16,  16,
    144,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16, 145,  16,  16,  16,
     16,  16,  16,  16, 146,  87,  11,  11,  11,  11,  11,  11,  11,  11, 147, 148,
     11,  11,  11,  11,  11,  11,  11, 149,  11,  11,  11,  11,  11,  11,  17,  17,
     16,  16,  16, 150,  11,  11,  11,  11,  16, 151,  16,  16,  16,  16,  16, 137,
     16,  16,  16,  16,  16, 135,  11, 152, 153,  16, 154, 155,  11,  11,  11,  11,
     11, 156,   4,  11,  11,  11,  11, 157,  11,  11,  11,  11,  16,  16, 137,  11,
     11, 118,  11,  11,  11,  16,  11, 158,  11,  11,  11, 144, 159,  11,  11,  11,
     11,  11,  11,  11,  11,  11,  11, 160,  11,  11,  11,  11,  11,  99,  11, 161,
     11,  11,  11,  11,  16,  16,  16,  16,  11,  16,  16,  16, 138,  11,  11,  11,
    117,  11,  11,  11,  11,  11, 149, 162,  11,  11,  11,  11,  11,  11,  11, 107,
     16,  16, 150,  11,  11,  11,  11,  11, 163,  11,  11,  11,  11,  11,  11,  11,
    164,  11, 165, 166,  11,  11,  11, 167,  11,  11,  11,  11, 168,  11,  17, 107,
     11,  11, 169,  11, 170, 107,  11,  11,  44,  11,  11, 171,  11,  11,  11,  11,
     11,  11, 172, 173, 174,  11,  11,  11,  11,  11,  11, 175,  49,  11,  11,  11,
     11,  11,  11,  11,  11,  11, 176,  11,  11, 177, 178,  26,  26,  29,  29,  29,
     29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 179,  29,  29,  29,  29,
     29,  29,  29,  29,  29,   8,   8, 180,  17,  87, 114,  16,  16, 181, 182,  29,
     29,  29,  29,  29,  29,  29,  29, 183, 184,   3,   4,   5,   4,   5, 135,  11,
     11,  11,  11,  11,  11,  11, 185, 186, 187,  11,  11,  11,  16,  16,  16,  16,
    139, 152,  11,  11,  11,  11,  11,  86,  26, 188,  26,  26,  26,  26,  26,  26,
    189,  26,  26, 190,  26,  26,  26,  26,  26,  26,  26, 191,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26, 192, 193,  49,  11,  11, 194, 114,  14, 135,  11,
    107,  11,  11, 195,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11, 196, 197,
    198,  11, 199,  11,  11,  11,  11,  11,  16,  16,  16,  16, 200,  11,  11,  11,
     16,  16,  16,  16,  16, 138,  11,  11,  11,  11,  11,  11,  11, 157,  11,  11,
     11, 201,  11,  11, 161,  11,  11,  11, 133,  11,  11,  11, 202, 203, 203, 203,
     16,  16, 152,  16,  16,  16,  16,  16,  16, 137, 204, 205, 144, 144,  11,  11,
    206,  11,  11,  11,  11,  11,  11,  11,  16,  16,   4, 207,  16,  16,  16, 146,
     16, 137,  16,  16, 208,  11,  16,   4,  16,  16,  16, 204, 209,  16,  16,  16,
    143,  16,  16,  16,  16,  16,  16, 210,  16,  16, 211, 212,  11,  16, 213,  11,
     11,  11,  11,  11,  11,  11,  11,   5, 144, 214, 215, 211, 216,  11,  11,  11,
     16,  16,  16,  16, 135,  11,  11,  11,  16,  16,  16,  16,  16,  16,  16, 137,
     11,  11,  11,  11,  11,  11,  11, 217,   8,   8,   8,   8,   8,   8,   8,   8,
     17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,   8,
};

static RE_UINT8 re_bidi_class_stage_4[] = {
      0,   0,   1,   2,   0,   0,   0,   3,   4,   5,   6,   7,   8,   8,   9,  10,
     11,  12,  12,  12,  12,  12,  13,  10,  12,  12,  13,  14,   0,  15,   0,   0,
      0,   0,   0,   0,  16,   5,  17,  18,  19,  20,  21,  10,  12,  12,  12,  12,
     12,  13,  12,  12,  12,  12,  22,  12,  23,  10,  10,  10,  12,  24,  10,  17,
     10,  10,  10,  10,  25,  25,  25,  25,  12,  26,  12,  27,  12,  17,  12,  12,
     12,  27,  12,  12,  28,  25,  29,  12,  12,  12,  27,  12,  30,  25,  25,  25,
     25,  25,  25,  31,  32,  31,  33,  33,  33,  33,  33,  33,  34,  35,  36,  37,
     25,  25,  38,  39,  39,  39,  39,  39,  39,  39,  40,  25,  34,  34,  41,  42,
     43,  39,  39,  39,  39,  44,  25,  45,  25,  46,  47,  48,   8,   8,  49,  39,
     39,  39,  39,  50,  51,  39,  39,  39,  39,  44,  25,  25,  33,  33,  52,  25,
     25,  53,  54,  33,  33,  55,  31,  25,  25,  30,  30,  56,  33,  33,  30,  33,
     57,  12,  12,  12,  12,  12,  58,  59,  60,  25,  59,  61,  60,  25,  12,  12,
     62,  12,  12,  12,  61,  12,  12,  12,  12,  12,  12,  59,  60,  59,  12,  61,
     63,  12,  64,  12,  65,  12,  12,  12,  65,  28,  66,  29,  29,  61,  12,  12,
     60,  67,  59,  61,  68,  12,  12,  12,  12,  12,  12,  66,  12,  58,  12,  12,
     58,  12,  12,  12,  59,  12,  12,  61,  13,  10,  69,  12,  12,  12,  12,  62,
     59,  62,  70,  29,  12,  65,  12,  12,  12,  12,  10,  71,  12,  12,  12,  29,
     12,  12,  58,  12,  62,  72,  12,  12,  61,  25,  57,  64,  12,  28,  25,  57,
     61,  25,  67,  59,  12,  12,  25,  29,  12,  12,  29,  12,  12,  73,  74,  26,
     60,  25,  25,  57,  25,  70,  12,  60,  25,  25,  60,  25,  25,  25,  25,  59,
     12,  12,  12,  60,  70,  25,  65,  65,  12,  12,  29,  62,  59,  12,  12,  12,
     60,  59,  12,  12,  58,  65,  12,  61,  12,  12,  12,  61,  10,  10,  26,  12,
     75,  12,  12,  12,  12,  12,  13,  11,  62,  59,  12,  12,  12,  28,  25,  29,
     12,  58,  60,  25,  25,  12,  64,  61,  10,  10,  76,  77,  12,  12,  61,  12,
     57,  28,  59,  12,  58,  12,  60,  12,  11,  26,  12,  12,  12,  12,  12,  23,
     12,  28,  59,  12,  12,  58,  25,  57,  72,  60,  25,  59,  28,  25,  25,  66,
     25,  12,  12,  12,  12,  70,  57,  59,  12,  12,  28,  25,  29,  12,  12,  12,
     62,  29,  29,  12,  12,  58,  29,  73,  12,  12,  12,  25,  25,  62,  12,  12,
     57,  25,  25,  25,  70,  25,  59,  61,  25,  57,  12,  12,  12,  12,  12,  78,
     26,  12,  12,  24,  12,  12,  12,  24,  12,  12,  12,  22,  79,  79,  80,  81,
     10,  10,  82,  83,  84,  85,  10,  10,  10,  86,  10,  10,  10,  10,  10,  87,
     88,   8,  89,  71,   8,   8,  89,  71,  84,  84,  84,  84,  84,  84,  90,  12,
     17,  71,  26,  12,  12,  20,  11,  23,  10,  78,  91,  92,  12,  12,  23,  12,
     10,  11,  23,  26,  12,  12,  91,  12,  93,  10,  10,  10,  10,  26,  12,  12,
     10,  20,  10,  10,  10,  12,  12,  12,  10,  71,  12,  12,  10,  10,  71,  12,
     10,  10,   8,   8,   8,   8,   8,  12,  12,  12,  23,  10,  10,  10,  10,  24,
     24,  10,  10,  10,  10,  10,  71,  20,  10,  10,  10,  11,  12,  24,  71,  28,
     29,  12,  24,  10,  12,  12,  12,  28,  26,  12,  12,  12,  10,  10,  17,  10,
     10,  10,  10,  12,  94,  11,  10,  10,  11,  12,  62,  25,  11,  23,  12,  24,
     12,  12,  95,  11,  12,  12,  13,  12,  12,  12,  12,  71,  12,  12,  12,  10,
     12,  13,  71,  12,  12,  12,  12,  13,  96,  12,  12,  97,  12,  12,  11,  12,
     58,  58,  28,  12,  12,  65,  10,  12,  12,  12,  90,  12,  12,  10,  12,  12,
     12,  59,  12,  12,  12,  62,  25,  29,  12,  28,  25,  25,  28,  62,  29,  59,
     12,  12,  60,  57,  65,  65,  12,  12,  28,  12,  12,  59,  70,  66,  59,  62,
     12,  61,  59,  61,  12,  12,  12,  98,  33,  33,  99,  33,  39,  39,  39,  35,
     39,  39,  39, 100, 101, 102,  10, 103, 104,  71, 105,  12,  39,  39,  39, 106,
    107,   5,   6,   7,   5, 108,  10,  71,   0,   0, 109, 110,  91,  12,  12,  12,
     33,  33,  33, 111,  30,  32,  33,  25,  33,  33, 112,  52,  33,  33, 113,  10,
     34,  34,  34,  34,  34,  34,  34, 114,  12,  12,  25,  25,  28,  57,  65,  12,
     12,  28,  29,  12, 115,   0, 116,  25,  57,  60,  25,  12,  12,  12,  62,  29,
    117, 118,  12,  12,  12,  91,  12,  12,  13,  12,  12, 119,   8,   8,   8,   8,
     10,  10,  10,  71,  24,  10,  10,  71,   8,   8, 120,  12,  10,  17,  10,  10,
     10,  20,  71,  12,  20,  10,  10,  10,  10,  10,  24,  11,  10,  24,  10,  10,
     10,  10,  10,  26,  10,  10,  12,  12,  20, 121, 121,  71,  10,  26,  10,  91,
     11,  24,  10,  10,  12,  12,  12, 122,
};

static RE_UINT8 re_bidi_class_stage_5[] = {
    11, 11, 11, 11, 11,  8,  7,  8,  9,  7, 11, 11,  7,  7,  7,  8,
     9, 10, 10,  4,  4,  4, 10, 10, 10, 10, 10,  3,  6,  3,  6,  6,
     2,  2,  2,  2,  2,  2,  6, 10, 10, 10, 10, 10, 10,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 10, 10, 10, 10, 11, 11,  7, 11, 11,
     6, 10,  4,  4, 10, 10,  0, 10, 10, 11, 10, 10,  4,  4,  2,  2,
    10,  0, 10, 10, 10,  2,  0, 10,  0, 10, 10,  0,  0,  0, 10, 10,
     0, 10, 10, 10, 12, 12, 12, 12, 10, 10,  0,  0,  0,  0, 10,  0,
     0,  0,  0, 12, 12, 12,  0,  0,  1, 12, 12, 12, 12, 12,  1, 12,
     1, 12, 12,  1,  1,  1,  1,  1,  5,  5,  5,  5, 13, 13, 10, 10,
    13,  4,  4, 13,  6, 13, 10, 10, 12, 12, 12, 13, 13, 13, 13, 13,
    13, 13, 13, 12,  5,  5,  4,  5,  5, 13, 13, 13, 12, 13, 13, 13,
    13, 13, 12, 12, 12,  5, 10, 12, 12, 13, 13, 12, 12, 10, 12, 12,
    12, 12, 13, 13,  2,  2, 13, 13, 13, 13, 13,  5, 13, 12, 13, 13,
     1,  1,  1, 12,  1,  1, 10, 10, 10, 10,  1,  1,  1,  1, 12, 12,
    12, 12,  1,  1, 12, 12, 12,  0,  0,  0, 12,  0, 12,  0,  0,  0,
     0, 12, 12, 12,  0, 12,  0,  0,  0,  0, 12, 12,  0,  0,  4,  4,
     0,  0,  0,  4,  0, 12, 12,  0, 12,  0,  0, 12, 12, 12,  0, 12,
     0,  4,  0,  0, 10,  4, 10,  0, 12,  0, 12, 12, 10, 10, 10,  0,
    12,  0, 12,  0,  0, 12,  0, 12,  0, 12, 10, 10,  9,  0,  0,  0,
    10, 10, 10, 12, 12, 12,  9,  0,  0, 10,  0, 10,  9,  9,  9,  9,
     9,  9,  9, 11, 11, 11,  0,  1,  9,  7, 16, 17, 18, 14, 15,  6,
     4,  4,  4,  4,  4, 10, 10, 10,  6, 10, 10, 10, 10, 10, 10,  9,
     2,  0,  0,  0,  2,  2,  3,  3,  4,  4,  0,  0,  0, 10,  0,  0,
     0,  0,  4,  0, 10, 10,  3,  4,  9, 10, 10, 10,  0, 12, 12, 10,
    12, 12, 12, 10, 12, 12, 10, 10,  0,  1, 12,  1,  1,  3,  1,  1,
    13, 10, 13, 13,  6, 10,  6,  0, 10,  6, 10, 10, 10, 10, 10,  4,
    10, 10,  3,  3, 10,  4,  4, 10, 13, 13, 13, 11,  0, 10, 10,  4,
    10,  4,  4,  0, 11, 10, 10, 10, 10, 10, 11, 11,  1,  1,  1, 10,
    12, 12, 12,  1,  1, 10, 10, 10,  5,  5,  5,  1,  0,  0,  0, 11,
    11, 11, 11, 12, 10, 10, 12, 12, 12, 10,  0,  0,  0,  0,  2,  2,
     2,  2,  2,  0, 10,  0, 10,  0,  0,  0, 11, 11,
};

/* Bidi_Class: 3124 bytes. */

RE_UINT32 re_get_bidi_class(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_bidi_class_stage_1[f] << 5;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_bidi_class_stage_2[pos + f] << 3;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_bidi_class_stage_3[pos + f] << 2;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_bidi_class_stage_4[pos + f] << 2;
    value = re_bidi_class_stage_5[pos + code];

    return value;
}

/* Canonical_Combining_Class. */

static RE_UINT8 re_canonical_combining_class_stage_1[] = {
    0, 1, 2, 2, 2, 3, 2, 4, 5, 2, 2, 2, 2, 2, 6, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_canonical_combining_class_stage_2[] = {
     0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13,
    14,  0,  0, 15,  0,  0,  0, 16, 17, 18, 19, 20, 21, 22,  0,  0,
    23,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 25,  0,  0,
    26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 27,  0, 28, 29, 30, 31,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 32,  0,  0, 33,  0,
     0, 34,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,
    36,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 37, 38,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_canonical_combining_class_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  0,  0,  0,  0,
     0,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  6,  7,  8,  0,
     9,  0, 10, 11,  0,  0, 12, 13, 14, 15, 16,  0,  0,  0,  0, 17,
    18, 19, 20,  0,  0,  0,  0,  0,  0, 21, 22,  0,  0, 21, 23,  0,
     0, 21, 23,  0,  0, 21, 23,  0,  0, 21, 23,  0,  0,  0, 23,  0,
     0,  0, 24,  0,  0, 21, 23,  0,  0,  0, 23,  0,  0,  0, 25,  0,
     0, 26, 27,  0,  0, 28, 29,  0, 30, 31,  0, 32, 33,  0, 34,  0,
     0, 35,  0,  0, 36,  0,  0,  0,  0,  0, 37,  0,  0,  0,  0,  0,
    38, 38,  0,  0,  0,  0, 39,  0,  0,  0,  0,  0,  0, 40,  0,  0,
     0, 41,  0,  0,  0,  0,  0,  0, 42,  0,  0, 43,  0,  0,  0,  0,
     0, 44, 45, 46,  0, 25,  0, 47,  0, 48,  0,  0,  0,  0, 49, 50,
     0,  0,  0,  0,  0,  0, 51, 52,  0,  0,  0,  0,  0,  0, 53, 54,
     0,  0,  0,  0,  0,  0,  0, 55,  0,  0,  0, 56,  0,  0,  0, 57,
     0, 58,  0,  0, 59,  0,  0,  0,  0,  0,  0, 60,  0,  0,  0, 61,
    62,  0,  0,  0,  0,  0, 45, 63,  0, 64, 65,  0,  0, 66, 67,  0,
     0,  0,  0,  0,  0, 68, 69,  0,  0,  0,  0,  0,  0,  0,  0, 23,
    70,  0,  0,  0,  0,  0,  0,  0,  0, 71,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 72, 73, 74,  0,  0,  0,  0,  0,  0,
     0,  0, 62,  0,  0, 75,  0,  0,  0,  0,  0, 76, 77, 78,  0,  0,
     0,  0, 79,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_canonical_combining_class_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   2,   3,   4,
      5,   6,   7,   4,   4,   8,   9,  10,   1,  11,  12,  13,  14,  15,  16,  17,
     18,   1,   1,   1,   0,   0,   0,   0,  19,   1,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  20,  21,  22,   1,  23,   4,  21,  24,  25,  26,  27,  28,
     29,  30,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,  31,   0,
      0,   0,  32,  33,  34,  35,   1,  36,   0,   0,   0,   0,  37,   0,   0,   0,
      0,   0,   0,   0,   0,  38,   1,  39,  14,  39,  40,  41,   0,   0,   0,   0,
      0,   0,   0,   0,  42,   0,   0,   0,   0,   0,   0,   0,  43,  36,  44,  45,
     21,  45,  46,   0,   0,   0,   0,   0,   0,   0,  19,   1,  21,   0,   0,   0,
      0,   0,   0,   0,   0,  38,  47,   1,   1,  48,  48,  49,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,  51,
      0,   0,   0,  52,  53,  54,   0,   0,   0,   0,   0,  52,   0,   0,   0,   0,
      0,   0,   0,  52,   0,  55,   0,   0,   0,   0,  56,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  57,   0,   0,   0,  58,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  59,   0,   0,   0,  60,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  61,   0,   0,   0,   0,   0,   0,  62,  63,   0,
      0,   0,   0,   0,  64,  65,  66,  67,  68,  69,   0,   0,   0,   0,   0,   0,
      0,  70,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  71,  72,   0,
      0,   0,   0,  73,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  48,
      0,   0,   0,   0,   0,  74,   0,   0,   0,   0,   0,   0,  56,   0,   0,  75,
      0,   0,  76,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  77,   0,
      0,   0,   0,   0,   0,  19,  78,   0,  74,   0,   0,   0,   0,  48,   1,  79,
      0,   0,   0,   0,   0,  51,   0,   0,   0,  74,   0,   0,   0,   0,   0,   0,
      0,   0,  19,  10,   1,   0,   0,   0,   0,  80,   0,   0,  81,   0,   0,   0,
      0,   0,   0,   0,   0,  71,   0,   0,   0,   0,   0,   0,  82,   9,  12,   4,
     83,   8,  84,  73,   0,   0,   0,   0,  21,   1,  21,  85,  86,   1,   1,   1,
      1,  82,   0,   0,   0,   0,   0,  87,   0,   0,   0,   0,  88,   1,  89,  54,
     75,  90,  91,   4,  54,   0,   0,   0,   0,   0,   0,  19,  49,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  92,   1,   1,   1,   1,   1,   1,   1,   1,
      0,   0,  93,  94,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  95,   0,
      0,   0,   0,  19,   0,   0,   0,  49,   0,   0,   0,   0,  49,   0,   0,   0,
      0,  56,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,  49,   0,   0,   0,
      0,   0,  96,  61,   0,   0,   0,   0,   0,   0,   0,   0,  92,   0,   0,   0,
      0,   0,   0,   0,  71,   0,   0,   0,  74,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  97,  98,  54,  38,  75,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  99,   1,  82,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  73,   0,   0,   0, 100,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 101,  92,   0,   0,   0,   0,   0,   0, 102,   0,
      0, 103, 104, 105, 106,   0,  96,   4, 107,  48,  23,   0,   0,   0,   0,   0,
      0,   0,  38,  49,   0,   0,   0,   0,  38,  54,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_canonical_combining_class_stage_5[] = {
     0,  0,  0,  0, 50, 50, 50, 50, 50, 51, 45, 45, 45, 45, 51, 43,
    45, 45, 45, 45, 45, 41, 41, 45, 45, 45, 45, 41, 41, 45, 45, 45,
     1,  1,  1,  1,  1, 45, 45, 45, 45, 50, 50, 50, 50, 54, 50, 45,
    45, 45, 50, 50, 50, 45, 45,  0, 50, 50, 50, 45, 45, 45, 45, 50,
    51, 45, 45, 50, 52, 53, 53, 52, 53, 53, 52, 50,  0,  0,  0, 50,
     0, 45, 50, 50, 50, 50, 45, 50, 50, 50, 46, 45, 50, 50, 45, 45,
    50, 46, 49, 50,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 14, 15,
    16, 17,  0, 18,  0, 19, 20,  0, 50, 45,  0, 13, 25, 26, 27,  0,
     0,  0,  0, 22, 23, 24, 25, 26, 27, 28, 29, 50, 50, 45, 45, 50,
    45, 50, 50, 45, 30,  0,  0,  0,  0,  0, 50, 50, 50,  0,  0, 50,
    50,  0, 45, 50, 50, 45,  0,  0,  0, 31,  0,  0, 50, 45, 50, 50,
    45, 45, 50, 45, 45, 50, 45, 50, 45, 50, 50,  0, 50, 50,  0, 50,
     0, 50, 50, 50, 50, 50,  0,  0,  0, 45, 45, 45,  2,  0,  0,  0,
     0,  4,  0,  0,  0, 50, 45, 50, 50,  0,  0,  0,  0, 32, 33,  0,
     0,  0,  4,  0, 34, 34,  4,  0, 35, 35, 35, 35, 36, 36,  0,  0,
    37, 37, 37, 37, 45, 45,  0,  0,  0, 45,  0, 45,  0, 43,  0,  0,
     0, 38, 39,  0, 40,  0,  0,  0,  0,  0, 39, 39, 39, 39,  0,  0,
    39,  0, 50, 50,  4,  0, 50, 50,  0,  0, 45,  0,  0,  0,  0,  2,
     0,  4,  4,  0,  0, 45,  0,  0,  4,  0,  0,  0,  0, 50,  0,  0,
     0, 49,  0,  0,  0, 46, 50, 45, 45,  0,  0,  0, 50,  0,  0, 45,
     0,  0,  2,  0,  0,  0,  4,  4, 50, 50, 50,  0, 50,  0,  1,  1,
     1,  0,  0,  0, 50, 53, 42, 45, 41, 50, 50, 50, 52, 45, 50, 45,
    50, 50,  1,  1,  1,  1,  1, 50,  0,  1,  1, 50, 45, 50,  1,  1,
     0,  0,  0,  4,  0,  0, 44, 49, 51, 46, 47, 47,  0,  3,  3,  0,
     0,  0,  0, 45, 50,  0, 50, 50, 45,  0,  0, 50,  0,  0, 21,  0,
     0, 45,  0, 50, 50,  1, 45,  0,  0,  4,  2,  0,  0, 43, 43,  1,
     1,  1,  0,  0,  0, 48, 43, 43, 43, 43, 43,  0, 45, 45, 45,  0,
};

/* Canonical_Combining_Class: 1744 bytes. */

RE_UINT32 re_get_canonical_combining_class(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_canonical_combining_class_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_canonical_combining_class_stage_2[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_canonical_combining_class_stage_3[pos + f] << 3;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_canonical_combining_class_stage_4[pos + f] << 2;
    value = re_canonical_combining_class_stage_5[pos + code];

    return value;
}

/* Decomposition_Type. */

static RE_UINT8 re_decomposition_type_stage_1[] = {
    0, 1, 2, 2, 2, 3, 4, 5, 6, 2, 2, 2, 2, 2, 7, 8,
    2, 2, 2, 2, 2, 2, 2, 9, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_decomposition_type_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  9, 10, 11, 12, 13, 14,
    15,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 16,  7, 17, 18, 19,
    20, 21, 22, 23, 24,  7,  7,  7,  7,  7, 25,  7, 26, 27, 28, 29,
    30, 31, 32, 33,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7, 34,  7,  7,  7,  7, 35, 35, 35, 35,
    35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
    35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
    35, 35, 35, 35, 35, 35, 35, 36,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7, 35, 37, 38, 39, 40, 41, 42,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
    43,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7, 44,  7,  7, 45, 46, 47, 48,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7, 49, 50,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7, 35, 35, 51,  7,  7,  7,  7,  7,
};

static RE_UINT8 re_decomposition_type_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   3,   4,   3,   5,
      6,   7,   8,   9,  10,  11,   8,  12,   0,   0,  13,  14,  15,  16,  17,  18,
      6,  19,  20,  21,   0,   0,   0,   0,   0,   0,   0,  22,   0,  23,  24,   0,
      0,   0,   0,   0,  25,   0,   0,  26,  27,  14,  28,  14,  29,  30,   0,  31,
     32,  33,   0,  33,   0,  32,   0,  34,   0,   0,   0,   0,  35,  36,  37,  38,
      0,   0,   0,   0,   0,   0,   0,   0,  39,   0,   0,   0,   0,   0,   0,   0,
      0,   0,  40,   0,   0,   0,   0,  41,   0,   0,   0,   0,  42,  43,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,  33,  44,   0,  45,   0,   0,   0,   0,   0,   0,  46,  47,   0,   0,
      0,   0,   0,  48,   0,  49,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  50,  51,   0,   0,   0,  52,   0,   0,  53,   0,   0,   0,
      0,   0,   0,   0,  54,   0,   0,   0,   0,   0,   0,   0,  55,   0,   0,   0,
      0,   0,   0,   0,  53,   0,   0,   0,   0,   0,   0,   0,   0,  56,   0,   0,
      0,   0,   0,  57,   0,   0,   0,   0,   0,   0,   0,  57,   0,  58,   0,   0,
     59,   0,   0,   0,  60,  61,  33,  62,  63,  60,  61,  33,   0,   0,   0,   0,
      0,   0,  64,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  65,
     66,  67,   0,  68,  69,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,  70,  71,  72,  73,  74,  75,   0,  76,  73,  73,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   6,   6,   6,  77,   6,   6,   6,   6,   6,  78,
      6,  79,   6,   6,  79,  80,   6,  81,   6,   6,   6,  82,  83,  84,   6,  85,
     86,  87,  88,  89,  90,  91,   0,  92,  93,  94,  95,   0,   0,   0,   0,   0,
     96,  97,  98,  99, 100, 101, 102, 102, 103, 104, 105,   0, 106,   0,   0,   0,
    107,   0, 108, 109, 110,   0, 111, 112, 112,   0, 113,   0,   0,   0, 114,   0,
      0,   0, 115,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 116, 117, 102, 102, 102, 118, 116, 116, 119,   0,
    120,   0,   0,   0,   0,   0,   0, 121,   0,   0,   0,   0,   0, 122,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 123,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 124,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0, 125,   0,   0,   0,   0,   0,  57,
    102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 126,   0,   0,
    127,   0,   0, 128, 129, 130, 131, 132,   0, 133, 129, 130, 131, 132,   0, 134,
      0,   0,   0, 135, 102, 102, 102, 102, 136, 137,   0,   0,   0,   0,   0,   0,
    102, 136, 102, 102, 138, 139, 116, 140, 116, 116, 116, 116, 141, 116, 116, 140,
    142, 142, 142, 142, 142, 143, 102, 144, 142, 142, 142, 142, 142, 142, 102, 145,
      0,   0,   0,   0,   0,   0,   0, 146,   0,   0,   0,   0,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   6,   6,   6,   6,  21,   0,   0,   0,   0,   0,
     81, 147, 148,   6,   6,   6,  81,   6,   6,   6,   6,   6,   6,  78,   0,   0,
    149, 150, 151, 152, 153, 154, 155, 155, 156, 155, 157, 158,   0, 159, 160, 161,
    162, 162, 162, 162, 162, 162, 163, 164, 164, 165, 166, 166, 166, 167, 168, 169,
    162, 170, 171, 172,   0, 173, 174, 175, 176, 177, 164, 178, 179,   0,   0, 180,
      0, 181,   0, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 191, 192, 193,
    194, 195, 195, 195, 195, 195, 196, 197, 197, 197, 197, 198, 199, 200, 201,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0, 202, 203,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 204, 205,   0,   0,   0,   0, 206,  14,   0,   0,   0,
    207, 207, 207, 207, 207, 208, 207, 207, 207, 209, 210, 211, 212, 207, 207, 207,
    213, 214, 207, 215, 216, 217, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207,
    207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 218, 207, 207, 207, 207, 207,
    207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 219, 207, 207, 207,
    220, 102, 221, 142, 142,   0,   0,   0,   0, 222,   0,   0,   0,   0,   0,   0,
    223, 142, 142, 224, 225, 226,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      6,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_decomposition_type_stage_4[] = {
      0,   0,   0,   0,   1,   0,   2,   3,   4,   5,   6,   7,   8,   9,   8,   8,
     10,  11,  10,  12,  10,  11,  10,   9,   8,   8,   8,   8,  13,   8,   8,   8,
      8,  12,   8,   8,  14,   8,  10,  15,  16,   8,  17,   8,  12,   8,   8,   8,
      8,   8,   8,  15,  12,   0,   0,  18,  19,   0,   0,   0,   0,  20,  20,  21,
      8,   8,   8,  22,   8,  13,   8,   8,  23,  12,   8,   8,   8,   8,   8,  13,
      0,  13,   8,   8,   8,   0,   0,   0,  24,  24,  25,   0,   0,   0,  20,   5,
     24,  25,   0,   0,   9,  19,   0,   0,   0,  19,  26,  27,   0,  21,  11,  22,
      0,   0,  13,   8,   0,   0,  13,  11,  28,  29,   0,   0,  30,   5,  31,   0,
      9,  18,   0,  11,   0,   0,  32,   0,   0,  13,   0,   0,  33,   0,   0,   0,
      8,  13,  13,   8,  13,   8,  13,   8,   8,  12,  12,   0,   0,   3,   0,   0,
     13,  11,   0,   0,   0,  34,  35,   0,  36,   0,   0,   0,  18,   0,   0,   0,
     32,  19,   0,   0,   0,   0,   8,   8,   0,   0,  18,  19,   0,   0,   0,   9,
     18,  27,   0,   0,   0,   0,  10,  27,   0,   0,  37,  19,   0,   0,   0,  12,
      0,  19,   0,   0,   0,   0,  13,  19,   0,   0,  19,   0,  19,  18,  22,   0,
      0,   0,  27,  11,   3,   0,   0,   0,   0,   0,   0,   5,   0,   0,   0,   1,
     18,   0,   0,  32,  27,  18,   0,  19,  18,  38,  17,   0,  32,   0,   0,   0,
      0,  27,   0,   0,   0,   0,   0,  25,   0,  27,  36,  36,  27,   0,   0,   0,
      0,   0,  18,  32,   9,   0,   0,   0,   0,   0,   0,  39,  24,  24,  39,  24,
     24,  24,  24,  40,  24,  24,  24,  24,  41,  42,  43,   0,   0,   0,  25,   0,
      0,   0,  44,  24,   8,   8,  45,   0,   8,   8,  12,   0,   8,  12,   8,  12,
      8,   8,  46,  46,   8,   8,   8,  12,   8,  22,   8,  47,  21,  22,   8,   8,
      8,  13,   8,  10,  13,  22,   8,  48,  49,  50,  30,   0,  51,   3,   0,   0,
      0,  30,   0,  52,   3,  53,   0,  54,   0,   3,   5,   0,   0,   3,   0,   3,
     55,  24,  24,  24,  42,  42,  42,  43,  42,  42,  42,  56,   0,   0,  35,   0,
     57,  34,  58,  59,  59,  60,  61,  62,  63,  64,  65,  66,  66,  67,  68,  59,
     69,  61,  62,   0,  70,  70,  70,  70,  20,  20,  20,  20,   0,   0,  71,   0,
      0,   0,  13,   0,   0,   0,   0,  27,   0,   0,   0,  10,   0,  19,  32,  19,
      0,  36,   0,  72,  35,   0,   0,   0,  32,  37,  32,   0,  36,   0,   0,  10,
     12,  12,  12,   0,   0,   0,   0,   8,   8,   0,  13,  12,   0,   0,  33,   0,
     73,  73,  73,  73,  73,  20,  20,  20,  20,  74,  73,  73,  73,  73,  75,   0,
      0,   0,   0,  35,   0,  30,   0,   0,   0,   0,   0,  19,   0,   0,   0,  76,
      0,   0,   0,  44,   0,   0,   0,   3,  20,   5,   0,   0,  77,   0,   0,   0,
      0,  26,  30,   0,   0,   0,   0,  36,  36,  36,  36,  36,  36,  46,  32,   0,
      9,  22,  33,  12,   0,  19,   3,  78,   0,  37,  11,  79,  34,  20,  20,  20,
     20,  20,  20,  30,   4,  24,  24,  24,  20,  73,   0,   0,  80,  73,  73,  73,
     73,  73,  73,  75,  20,  20,  20,  81,  81,  81,  81,  81,  81,  81,  20,  20,
     82,  81,  81,  81,  20,  20,  20,  83,  25,   0,   0,   0,  36,  10,   8,  11,
     36,  33,  13,  12,  20,  30,   0,   0,   3,  20,   0,  46,  59,  59,  84,   8,
      8,  11,   8,  36,   9,  22,   8,  15,  85,  86,  86,  86,  86,  86,  86,  86,
     86,  85,  85,  85,  87,  85,  86,  86,  88,   0,   0,   0,  89,  90,  91,  92,
     85,  87,  86,  85,  85,  85,  93,  87,  94,  94,  94,  94,  94,  95,  95,  95,
     95,  95,  95,  95,  95,  96,  97,  97,  97,  97,  97,  97,  97,  97,  97,  98,
     99,  99,  99,  99,  99, 100,  94,  94, 101,  95,  95,  95,  95,  95,  95, 102,
     97,  99,  99, 103, 104,  97, 105, 106, 107, 105, 108, 105, 104,  96,  95, 105,
     96, 109, 110,  97, 111, 106, 112, 105,  95, 106, 113,  95,  96, 106,   0,   0,
     94,  94,  94, 114, 115, 115, 116,   0, 115, 115, 115, 115, 115, 117, 118,  20,
    119, 120, 120, 120, 120, 119, 120,   0, 121, 122, 123, 123, 124,  91, 125, 126,
     90, 125, 127, 127, 127, 127, 126,  91, 125, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 126, 125, 126,  91, 128, 129, 130, 130, 130, 130, 130, 130, 130,
    131, 132, 132, 132, 132, 132, 132, 132, 132, 132, 132, 133, 134, 132, 134, 132,
    134, 132, 134, 135, 130, 136, 132, 133,   0,   0,  27,  19,   0,   0,  18,   0,
      0,   0,   0,  13,   8,  19,   0,   0,   0,   0,  18,   8,  59,  59,  59,  59,
     59, 137,  59,  59,  59,  59,  59, 137, 138, 139,  61, 137,  59,  59,  66,  61,
     59,  61,  59,  59,  59,  66, 140,  61,  59, 137,  59, 137,  59,  59,  66, 140,
     59, 141, 142,  59, 137,  59,  59,  59,  59,  62,  59,  59,  59,  59,  59, 142,
     20,  20,  30,   0,  20,  20, 143,  75, 144,   0,   0,   0, 145,   0,   0,   0,
     81,  81, 145,   0,  20,  20,  35,   0, 146,   0,   0,   0,
};

static RE_UINT8 re_decomposition_type_stage_5[] = {
     0,  0,  0,  0,  4,  0,  0,  0,  2,  0, 10,  0,  0,  0,  0,  2,
     0,  0, 10, 10,  2,  2,  0,  0,  2, 10, 10,  0, 17, 17, 17,  0,
     1,  1,  1,  1,  1,  1,  0,  1,  0,  1,  1,  1,  1,  1,  1,  0,
     1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  2,  2,  1,  1,  1,  2,
     2,  0,  0,  1,  1,  2,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
     2,  2,  2,  2,  2,  1,  1,  1,  1,  0,  1,  1,  1,  2,  2,  2,
    10, 10, 10, 10, 10,  0,  0,  0,  0,  0,  2,  0,  0,  0,  1,  0,
     2,  2,  2,  1,  1,  2,  2,  0,  2,  2,  2,  0,  0,  2,  0,  0,
     0,  1,  0,  0,  0,  1,  1,  0,  0,  2,  2,  2,  2,  0,  0,  0,
     1,  0,  1,  0,  1,  0,  0,  1,  0,  1,  1,  2, 10, 10, 10,  0,
    10, 10,  0, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11,  0,
     0,  0,  0, 10,  1,  1,  2,  1,  0,  1,  0,  1,  1,  2,  1,  2,
     1,  1,  2,  0,  1,  1,  2,  2,  2,  2,  2,  4,  0,  4,  0,  0,
     0,  0,  0,  4,  2,  0,  2,  2,  2,  0,  2,  0, 10, 10,  0,  0,
    11,  0,  0,  0,  2,  2,  3,  2,  0,  2,  3,  3,  3,  3,  3,  3,
     0,  3,  2,  0,  0,  3,  3,  3,  3,  3,  0,  0, 10,  2, 10,  0,
     3,  0,  1,  0,  3,  0,  1,  1,  3,  3,  0,  3,  3,  2,  2,  2,
     2,  3,  0,  2,  3,  0,  0,  0, 17, 17, 17, 17,  0, 17,  0,  0,
     2,  2,  0,  2,  9,  9,  9,  9,  2,  2,  9,  9,  9,  9,  9,  0,
    11, 10,  0,  0, 13,  0,  0,  0,  2,  0,  1, 12,  0,  0,  1, 12,
    16,  9,  9,  9, 16, 16, 16, 16,  2, 16, 16, 16,  2,  2,  2, 16,
     3,  3,  1,  1,  8,  7,  8,  7,  5,  6,  8,  7,  8,  7,  5,  6,
     8,  7,  0,  0,  0,  0,  0,  8,  7,  5,  6,  8,  7,  8,  7,  8,
     7,  8,  8,  7,  5,  8,  7,  5,  8,  8,  8,  8,  7,  7,  7,  7,
     7,  7,  7,  5,  5,  5,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,
     6,  8,  8,  8,  8,  7,  7,  7,  7,  5,  5,  5,  7,  8,  0,  0,
     5,  7,  5,  5,  7,  5,  7,  7,  5,  5,  7,  7,  5,  5,  7,  5,
     5,  7,  7,  5,  7,  7,  5,  7,  5,  5,  5,  7,  0,  0,  5,  5,
     5,  7,  7,  7,  5,  7,  5,  7,  8,  0,  0,  0, 12, 12, 12, 12,
    12, 12,  0,  0, 12,  0,  0, 12, 12,  2,  2,  2, 15, 15, 15,  0,
    15, 15, 15, 15,  8,  6,  8,  0,  8,  0,  8,  6,  8,  6,  8,  6,
     8,  8,  7,  8,  7,  8,  7,  5,  6,  8,  7,  8,  6,  8,  7,  5,
     7,  0,  0,  0,  0, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14,  0,  0,  0, 14, 14, 14,  0,  0,  0,
    13, 13, 13,  0,  3,  0,  3,  3,  0,  0,  3,  0,  0,  3,  3,  0,
     3,  3,  3,  0,  3,  0,  3,  0,  0,  0,  3,  3,  2,  2,  2,  9,
    16,  0,  0,  0, 16, 16, 16,  0,  9,  9,  0,  0,
};

/* Decomposition_Type: 2784 bytes. */

RE_UINT32 re_get_decomposition_type(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_decomposition_type_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_decomposition_type_stage_2[pos + f] << 4;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_decomposition_type_stage_3[pos + f] << 2;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_decomposition_type_stage_4[pos + f] << 2;
    value = re_decomposition_type_stage_5[pos + code];

    return value;
}

/* East_Asian_Width. */

static RE_UINT8 re_east_asian_width_stage_1[] = {
     0,  1,  2,  3,  4,  5,  5,  5,  5,  5,  6,  5,  5,  7,  8,  9,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 10, 10, 10, 12,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 13,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    14, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
     8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 15,
     8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 15,
};

static RE_UINT8 re_east_asian_width_stage_2[] = {
     0,  1,  2,  3,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
     5,  6,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
     7,  8,  9, 10, 11, 12, 13, 14,  5, 15,  5, 16,  5,  5, 17, 18,
    19, 20, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 24,  5,  5,  5,  5, 25,  5,  5, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 26,  5,  5,  5,  5,  5,  5,  5,  5,
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
    27, 27, 27, 27, 27, 27, 27, 27, 27, 22, 22,  5,  5,  5, 28, 29,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
    30,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
     5, 31, 32,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 33,
     5, 34,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 35,
};

static RE_UINT8 re_east_asian_width_stage_3[] = {
      0,   0,   1,   1,   1,   1,   1,   2,   0,   0,   3,   4,   5,   6,   7,   8,
      9,  10,  11,  12,  13,  14,  11,   0,   0,   0,   0,   0,  15,  16,   0,   0,
      0,   0,   0,   0,   0,   9,   9,   0,   0,   0,   0,   0,  17,  18,   0,   0,
     19,  19,  19,  19,  19,  19,  19,   0,   0,  20,  21,  20,  21,   0,   0,   0,
      9,  19,  19,  19,  19,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     22,  22,  22,  22,  22,  22,   0,   0,   0,   0,  23,   0,   0,   0,   0,  24,
      0,  25,  26,  27,   0,   0,   0,  28,  29,   0,  30,   0,   0,   0,   0,   0,
     31,  32,  33,   0,   0,  34,  35,  36,  37,  36,   0,  38,   0,  39,  40,   0,
     41,  42,  43,  44,  45,  46,  47,   0,  48,  49,  50,  51,   0,   0,   0,   0,
      0,  46,  52,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  19,  19,  19,  19,  19,  19,  19,  19,  53,  19,
     19,  19,  19,  19,  35,  19,  19,  54,  19,  55,  21,  56,  57,  58,  59,   0,
     60,  61,   0,   0,  62,   0,  63,   0,   0,  64,   0,  64,  65,  19,  66,  19,
      0,   0,   0,  67,   0,  40,   0,  68,   0,   0,   0,   0,   0,   0,  69,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  70,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  71,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  22,  72,  22,  22,  22,  22,  22,  73,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  74,   0,  75,
     76,  22,  22,  77,  78,  22,  22,  22,  22,  79,  22,  22,  22,  22,  22,  22,
     80,  22,  81,  78,  22,  22,  22,  22,  77,  22,  22,  82,  22,  22,  73,  22,
     22,  77,  22,  22,  83,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  77,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,   0,   0,   0,   0,
     22,  22,  22,  22,  22,  22,  22,  22,  84,  22,  22,  22,  85,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  22,  84,   0,   0,   0,   0,   0,   0,   0,   0,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  73,  22,  86,  22,  22,  75,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,
     19,  87,   0,  22,  22,  88,  89,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     90,  91,  91,  91,  91,  91,  92,  93,  93,  93,  93,  94,  95,  96,  97,  67,
     98,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     99,  19, 100,  19,  19,  19,  36,  19,  19,  99,   0,   0,   0,   0,   0,   0,
    101,  22,  22,  82, 102,  98,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  81,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,   0,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19, 100,
};

static RE_UINT8 re_east_asian_width_stage_4[] = {
     0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  2,  3,  4,  5,  6,
     7,  8,  9,  7,  0, 10,  0,  0, 11, 12, 11, 13, 14, 10,  9, 14,
     8, 12,  9,  5, 15,  0,  0,  0, 16,  0, 12,  0,  0, 13, 12,  0,
    17,  0, 11, 12,  9, 11,  7, 15, 13,  0,  0,  0,  0,  0,  0, 10,
     5,  5,  5, 11,  0, 18, 17, 15, 11,  0,  7, 16,  7,  7,  7,  7,
    17,  7,  7,  7, 19,  7, 14,  0, 20, 20, 20, 20, 21, 20,  0,  0,
     0,  0, 22, 20, 18,  9, 14, 14,  9,  7,  0,  0,  8, 15, 12, 10,
     0, 11,  0, 12, 17, 11,  0,  0,  0,  0, 23, 11, 12, 15, 15,  0,
    12, 10,  0,  0, 24, 10, 12,  0, 12, 11, 12,  9,  7,  7,  7,  0,
     7,  7, 14,  0,  0,  0, 15,  0,  0,  0, 14,  0, 10, 11,  0,  0,
     0, 12,  0,  0,  8, 12, 18, 12, 15, 15, 10, 17, 18, 16,  7,  5,
     0,  7,  0, 14,  0,  0, 11, 11, 10,  0,  0,  0, 14,  7, 13, 13,
    13, 13,  0,  0,  0, 15, 15,  0,  0, 15,  0,  0,  0,  0,  0, 12,
     0,  0, 25,  0,  7,  7, 19,  7,  7,  0,  0,  0, 13, 14,  0,  0,
    13, 13,  0, 14, 14, 13, 18, 13, 14,  0,  0,  0, 13, 14,  0, 12,
     0, 24, 15, 13,  0, 14,  0,  5,  5,  0,  0,  0, 19, 19,  9, 19,
     0,  0,  0, 13,  0,  7,  7, 19, 19,  0,  7,  7,  0,  0,  0, 15,
     0, 13,  7,  7,  0, 26,  1, 27,  0, 28,  0,  0,  0, 17, 14,  0,
    20, 20, 29, 20, 20,  0,  0,  0, 20, 30,  0,  0, 20, 20, 20,  0,
    31, 20, 20, 20, 20, 20, 20, 32, 33, 20, 20, 20, 20, 32, 33, 20,
     0, 33, 20, 20, 20, 20, 20, 30, 20, 20, 32,  0, 20, 20,  7,  7,
    20, 20, 20, 34, 20, 32,  0,  0, 20, 32, 21, 20, 20, 20, 30,  0,
    32, 20, 20, 20, 20, 32, 20,  0, 35, 36, 36, 36, 36, 36, 36, 36,
    37, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 39, 40, 38, 40, 38,
    40, 38, 40, 41, 36, 42, 38, 39, 30,  0,  0,  0,  7,  7,  9,  0,
     7,  7,  7, 14, 32,  0,  0,  0, 20, 20, 34,  0,
};

static RE_UINT8 re_east_asian_width_stage_5[] = {
    0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 1, 5, 5,
    1, 5, 5, 1, 1, 0, 1, 0, 5, 1, 1, 5, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0,
    0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
    3, 3, 3, 3, 0, 0, 0, 3, 0, 0, 3, 3, 0, 2, 0, 0,
    0, 1, 1, 0, 0, 3, 3, 0, 0, 0, 5, 5, 5, 5, 0, 0,
    0, 5, 5, 0, 3, 3, 0, 3, 3, 3, 0, 0, 4, 3, 3, 3,
    3, 3, 3, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 4, 4, 4,
    4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 2, 2, 2, 0, 0, 0, 4, 4, 4, 0,
};

/* East_Asian_Width: 1688 bytes. */

RE_UINT32 re_get_east_asian_width(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_east_asian_width_stage_1[f] << 4;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_east_asian_width_stage_2[pos + f] << 4;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_east_asian_width_stage_3[pos + f] << 2;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_east_asian_width_stage_4[pos + f] << 2;
    value = re_east_asian_width_stage_5[pos + code];

    return value;
}

/* Joining_Group. */

static RE_UINT8 re_joining_group_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1,
};

static RE_UINT8 re_joining_group_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_joining_group_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_joining_group_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  0,  0,  0,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,  0,  0, 21,  0, 22,
     0,  0, 23, 24, 25, 26,  0,  0,  0, 27, 28, 29, 30, 31, 32, 33,
};

static RE_UINT8 re_joining_group_stage_5[] = {
     0,  0,  0,  0,  0,  0,  0,  0, 45,  0,  3,  3, 43,  3, 45,  3,
     4, 41,  4,  4, 13, 13, 13,  6,  6, 31, 31, 35, 35, 33, 33, 39,
    39,  1,  1, 11, 11, 55, 55, 55,  0,  9, 29, 19, 22, 24, 26, 16,
    43, 45, 45,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4, 29,
     0,  3,  3,  3,  0,  3, 43, 43, 45,  4,  4,  4,  4,  4,  4,  4,
     4, 13, 13, 13, 13, 13, 13, 13,  6,  6,  6,  6,  6,  6,  6,  6,
     6, 31, 31, 31, 31, 31, 31, 31, 31, 31, 35, 35, 35, 33, 33, 39,
     1,  9,  9,  9,  9,  9,  9, 29, 29, 11, 38, 11, 19, 19, 19, 11,
    11, 11, 11, 11, 11, 22, 22, 22, 22, 26, 26, 26, 26, 56, 21, 13,
    41, 17, 17, 14, 43, 43, 43, 43, 43, 43, 43, 43, 55, 47, 55, 43,
    45, 45, 46, 46,  0, 41,  0,  0,  0,  0,  0,  0,  0,  0,  6, 31,
     0,  0, 35, 33,  1,  0,  0, 21,  2,  0,  5, 12, 12,  7,  7, 15,
    44, 50, 18, 42, 42, 48, 49, 20, 23, 25, 27, 36, 10,  8, 28, 32,
    34, 30,  7, 37, 40,  5, 12,  7,  0,  0,  0,  0,  0, 51, 52, 53,
     4,  4,  4,  4,  4,  4,  4, 13, 13,  6,  6, 31, 35,  1,  1,  1,
     9,  9, 11, 11, 11, 24, 24, 26, 26, 26, 22, 31, 31, 35, 13, 13,
    35, 31, 13,  3,  3, 55, 55, 45, 43, 43, 54, 54, 13, 35, 35, 19,
};

/* Joining_Group: 434 bytes. */

RE_UINT32 re_get_joining_group(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_joining_group_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_joining_group_stage_2[pos + f] << 4;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_joining_group_stage_3[pos + f] << 4;
    f = code >> 3;
    code ^= f << 3;
    pos = (RE_UINT32)re_joining_group_stage_4[pos + f] << 3;
    value = re_joining_group_stage_5[pos + code];

    return value;
}

/* Joining_Type. */

static RE_UINT8 re_joining_type_stage_1[] = {
    0, 1, 2, 2, 2, 3, 2, 4, 5, 2, 2, 2, 2, 2, 6, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_joining_type_stage_2[] = {
     0,  1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15,  1,  1, 16,  1,  1,  1, 17, 18, 19, 20, 21, 22, 23,  1,  1,
    24,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 25, 26,  1,  1,
    27,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1, 28,  1, 29, 30, 31, 32,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 33,  1,  1, 34, 35,
     1, 36,  1,  1,  1,  1,  1,  1,  1,  1, 37,  1,  1,  1,  1,  1,
    38,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 39, 40,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    41, 42,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_joining_type_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   2,   5,   6,   0,   0,   0,
      0,   7,   8,   9,  10,   2,  11,  12,  13,  14,  15,  15,  16,  17,  18,  19,
     20,  21,  22,   2,  23,  24,  25,  26,   0,   0,  27,  28,  29,  15,  30,  31,
      0,  32,  33,   0,   0,  34,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     35,   0,   0,  36,  37,  38,  39,   0,  40,   0,   0,  41,  42,   0,  39,   0,
     43,   0,   0,  41,  44,  40,   0,  45,  43,   0,   0,  41,  46,   0,  39,   0,
     40,   0,   0,  47,  42,  48,  39,   0,  49,   0,   0,   0,  50,   0,   0,   0,
      0,   0,   0,  51,  52,  53,  39,   0,   0,   0,   0,  47,  54,   0,  39,   0,
      0,   0,   0,   0,  42,   0,  39,   0,   0,   0,   0,   0,  55,  56,   0,   0,
      0,   0,   0,  57,  58,   0,   0,   0,   0,   0,   0,  59,  60,   0,   0,   0,
      0,  61,   0,  62,   0,   0,   0,  63,  64,  65,   2,  66,  48,   0,   0,   0,
      0,   0,  67,  68,   0,  69,  28,  70,  71,   1,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  67,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,  72,   0,  72,   0,  39,   0,  39,   0,   0,   0,  73,  74,  75,   0,   0,
     76,   0,   0,   0,   0,   0,   0,   0,   0,   0,  77,   0,   0,   0,   0,   0,
      0,   0,  78,  79,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,  80,   0,   0,   0,  81,  82,  83,   0,   0,   0,   0,   0,   0,   0,   0,
     84,   0,   0,  85,  49,   0,  86,  84,  87,   0,  88,   0,   0,   0,  89,  87,
      0,   0,  90,  91,   0,   0,   0,   0,   0,   0,   0,   0,   0,  92,  93,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,  94,  90,
     95,   0,  96,   0,   0,   0,  97,   0,   0,   0,   0,   0,   0,   2,   2,  28,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  87,
      0,   0,   0,   0,   0,   0,   0,  20,   0,   0,   0,   0,   0,   0,   2,   2,
      0,   0,  98,   0,   0,   0,   0,   0,   0,  99,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  20, 100,   0,   0,   0,   0,   0,   0,   0,  87,
    101,   0,  53,   0,   0,   0,   0,   0,   0,   0,   0,   0, 102,   0,   2,  87,
      0,   0, 103,   0, 104,  87,   0,   0,  35,   0,   0, 105,   0,   0,   0,   0,
      0,   0, 106, 107, 108,   0,   0,   0,   0,   0,   0, 109,  40,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 110,   0,
      0, 111,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      2,   0,  94,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  34,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
    112,   0,   0, 113,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     40,   0,   0, 114,  94,   0,   0,   0,  87,   0,   0, 115,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 116, 117, 118,   0, 119,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  72,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     40,   0,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   0,
};

static RE_UINT8 re_joining_type_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  1,  2,  2,  2,  2,  3,  2,  4,  0,
     5,  2,  2,  2,  2,  2,  2,  6,  7,  6,  0,  0,  2,  2,  8,  0,
     9, 10, 11, 12, 13, 14, 14, 14, 15, 14, 16,  2,  0,  0,  0, 17,
    18, 19, 14, 14, 14, 14, 20, 20, 20, 20, 21, 14, 14, 14, 14, 14,
    22, 20, 20, 23, 24, 25,  2, 26,  2, 26, 27, 28,  0,  0, 17, 29,
     0,  0,  0,  3, 30, 31, 21, 32, 14, 14, 33, 22,  2,  2,  8, 34,
    14, 14, 31, 14, 14, 14, 12, 35, 23, 35, 21, 14,  0, 36,  2,  2,
    37,  0,  0,  0,  0,  0, 17, 14, 14, 14, 38,  2,  2,  0, 39,  0,
     0, 36,  6,  2,  2,  5,  5,  4,  0,  0,  5,  0,  8,  0,  0,  0,
     0,  0, 40, 37,  5,  2, 37,  1,  5,  2,  0,  0, 36,  0,  0,  0,
     1,  0,  0,  0,  0,  0,  0, 37,  5, 37,  0,  1,  7,  0,  0,  0,
     7,  3, 26,  4,  4,  1,  0,  0,  5,  6, 37,  1,  0,  0,  0, 26,
     0, 40,  0,  0, 40,  0,  0,  0, 37,  0,  0,  1,  0,  0,  0, 36,
    37, 36, 27,  4,  0,  7,  0,  0,  0, 40,  0,  4,  0,  0, 40,  0,
    36, 41,  0,  0,  1,  2,  8,  0,  0,  3,  2,  8,  1,  2,  6, 37,
     0,  0,  2,  4,  0,  0,  4,  0,  0, 42,  1,  0,  5,  2,  2,  8,
     2, 27,  0,  5,  2,  2,  5,  2,  2,  2,  2, 37,  0,  0,  0,  5,
    27,  2,  7,  7,  0,  0,  4, 36,  5, 37,  0,  0, 40,  7,  0,  1,
    36, 37,  0,  0,  0,  6,  2,  4,  0, 40,  5,  2,  2,  0,  0,  1,
     0,  0,  3,  4,  0,  0,  1,  0,  8,  3, 37,  0, 40,  0,  5,  0,
     0,  3, 37,  0,  0, 40,  2,  8, 41,  5,  2, 37,  3,  2,  2, 26,
     2,  0,  0,  0,  0, 27,  8, 37,  0,  0,  3,  2,  4,  0,  0,  0,
    36,  4,  4,  0,  0, 40,  4, 42,  0,  0,  0,  2,  2, 36,  0,  0,
     8,  2,  2,  2, 27,  2, 37,  1,  2,  8,  0,  0,  0,  0,  3, 43,
     0,  0, 36,  8,  2, 37, 36,  2,  0,  0, 36,  2,  0,  0,  7,  0,
     8,  0,  0,  4, 40, 40,  3,  0,  0, 37,  0,  0,  0, 36,  2,  4,
     0,  3,  2,  2,  3, 36,  4, 37,  0,  0,  5,  8,  7,  7,  0,  0,
     3,  0,  0, 37, 27, 26, 37, 36,  0,  1, 37,  1,  0,  0,  0, 40,
     5,  7,  0,  2,  0,  0,  8,  3,  0,  0,  2,  2,  3,  8,  7,  1,
     0,  3,  4,  0,  3,  2,  2,  2,  8,  5,  2,  0,  0,  0, 36,  4,
};

static RE_UINT8 re_joining_type_stage_5[] = {
    0, 0, 0, 0, 0, 4, 0, 0, 4, 4, 4, 4, 0, 0, 0, 4,
    4, 4, 0, 0, 0, 4, 4, 4, 4, 4, 0, 4, 0, 4, 4, 0,
    4, 4, 4, 0, 2, 0, 3, 3, 3, 3, 2, 3, 2, 3, 2, 2,
    2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 2, 2, 2,
    3, 2, 2, 4, 0, 0, 2, 2, 4, 3, 3, 3, 0, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 2, 2, 3, 2, 2, 3, 2, 3, 2, 3,
    2, 2, 3, 3, 0, 3, 4, 4, 4, 0, 0, 4, 4, 0, 4, 4,
    4, 4, 3, 3, 2, 0, 0, 2, 3, 4, 2, 2, 2, 3, 3, 3,
    2, 2, 3, 2, 3, 2, 3, 2, 0, 3, 2, 2, 3, 2, 2, 2,
    0, 0, 4, 4, 4, 0, 0, 0, 2, 2, 2, 4, 0, 0, 1, 0,
    0, 0, 4, 0, 4, 0, 4, 0, 0, 4, 0, 4, 0, 1, 4, 4,
};

/* Joining_Type: 1736 bytes. */

RE_UINT32 re_get_joining_type(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_joining_type_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_joining_type_stage_2[pos + f] << 4;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_joining_type_stage_3[pos + f] << 2;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_joining_type_stage_4[pos + f] << 2;
    value = re_joining_type_stage_5[pos + code];

    return value;
}

/* Line_Break. */

static RE_UINT8 re_line_break_stage_1[] = {
     0,  1,  2,  3,  4,  5,  5,  5,  5,  5,  6,  7,  8,  9, 10, 11,
    12, 13, 14, 15, 10, 10, 16, 10, 10, 10, 10, 17, 10, 18, 10, 19,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 20,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 20,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    21, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
};

static RE_UINT8 re_line_break_stage_2[] = {
      0,   1,   2,   2,   2,   3,   4,   5,   2,   6,   7,   8,   9,  10,  11,  12,
     13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
     29,  30,  31,  32,  33,  34,  35,  36,  37,   2,   2,   2,   2,  38,  39,  40,
     41,  42,  43,  44,  45,  46,  47,  48,  49,  50,   2,  51,   2,   2,  52,  53,
     54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      2,   2,   2,  70,   2,   2,  71,  14,  72,  73,  74,  75,  76,  77,  78,  79,
     80,  81,  82,  83,  84,  85,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,
     78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,
     78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,
     78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  86,  78,  78,  78,  78,
     78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,
     78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,
     87,  78,  78,  78,  78,  78,  78,  78,  78,  88,   2,   2,  89,  90,   2,  91,
     92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 100,
    101, 102, 103, 104, 105, 106, 100, 101, 102, 103, 104, 105, 106, 100, 101, 102,
    103, 104, 105, 106, 100, 101, 102, 103, 104, 105, 106, 100, 101, 102, 103, 104,
    105, 106, 100, 101, 102, 103, 104, 105, 106, 100, 101, 102, 103, 104, 105, 106,
    100, 101, 102, 103, 104, 105, 106, 100, 101, 102, 103, 104, 105, 106, 100, 101,
    102, 103, 104, 105, 106, 100, 101, 102, 103, 104, 105, 106, 100, 101, 102, 107,
    108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  78,  78,  78,  78, 109, 110,   2,   2, 111, 112, 113, 114, 115, 116,
    117, 118, 119, 120,  14, 121, 122, 123,   2, 124,  14,  14,  14,  14,  14,  14,
    125,  14, 126,  14, 127,  14, 128,  14, 129,  14,  14,  14, 130,  14,  14,  14,
    131, 132,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
      2,   2,   2,   2,   2,   2, 133,  14, 134,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
      2,   2,   2,   2, 135, 136, 137,   2, 138,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
      2,   2,   2,   2, 139,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
    140,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
      2, 141, 142, 143, 144,  14, 145,  14, 146, 147, 148,   2,   2, 149,   2, 150,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
    151, 152, 153, 154, 155,  14, 156, 157, 158, 159, 160, 161, 162, 163,  61,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,
     78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78, 164,
    165,  14, 166, 167,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
};

static RE_UINT16 re_line_break_stage_3[] = {
      0,   1,   2,   3,   4,   5,   4,   6,   7,   1,   8,   9,   4,  10,   4,  10,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  11,  12,   4,   4,
      1,   1,   1,   1,  13,  14,  15,  16,  17,   4,  18,   4,   4,   4,   4,   4,
     19,   4,   4,   4,   4,   4,   4,   4,   4,   4,  20,  21,   4,  22,  21,   4,
     23,  24,   1,  25,  26,   4,  27,  28,  29,  30,   4,   4,  31,   1,  32,  33,
      4,   4,   4,   4,   4,  34,  35,  36,  37,  38,   4,   1,  39,   4,   4,   4,
      4,   4,  40,  41,  36,   4,  31,  42,   4,  43,  44,  45,   4,  46,  47,  47,
     47,  47,  47,  47,  47,  47,  47,  47,  48,   4,   4,  49,   1,  50,  51,  52,
     53,  54,  55,  56,  57,  58,  59,  60,  61,  54,  55,  62,  63,  64,  65,  66,
     67,  18,  55,  68,  69,  70,  59,  71,  53,  54,  55,  68,  72,  73,  59,  20,
     74,  75,  76,  77,  78,  79,  65,  80,  81,  82,  55,  83,  84,  85,  59,  86,
     87,  82,  55,  88,  84,  89,  59,  90,  87,  82,   4,  91,  92,  93,  59,  94,
     95,  96,   4,  97,  98,  99,  47, 100, 101, 102, 102, 103, 104, 105,  47,  47,
    106, 107, 108, 109, 110, 111,  47,  47, 112, 113,  36, 114,  52,   4, 115, 116,
    117, 118,   1, 119, 120, 121,  47,  47, 102, 102, 102, 102, 122, 102, 102, 102,
    102, 123,   4,   4, 124,   4,   4, 115, 125, 125, 125, 125, 125, 125, 126, 126,
    126, 126, 127, 128, 128, 128, 128, 128,   4,   4,   4,   4, 129, 130,   4,   4,
    129,   4,   4, 131, 132, 133,   4,   4,   4, 132,   4,   4,   4, 134, 135, 115,
      4, 136,   4,   4,   4,   4,   4,  28, 137,   4,   4,   4,   4,   4,   4,   4,
    137, 138,   4,   4,   4,   4, 139,  70, 140, 141,   4, 142,   4, 143, 140, 144,
    102, 102, 102, 102, 102, 145, 146, 136, 147, 146,   4,   4,   4,   4,   4,  20,
      4,   4, 148,   4,   4,   4,   4, 124,   4, 115, 149, 149, 150, 102, 151, 152,
    102, 102, 153, 102, 154, 155,   4,   4,   4, 156, 102, 102, 102, 157, 102, 158,
    146, 146, 151,  47,  47,  47,  47,  47, 159,   4,   4, 160, 161, 162, 163, 164,
    165,   4, 166, 146,   4,   4,  40, 167,   4,   4, 160, 168, 169,  36,   4, 170,
     47,  47,  47,  47,  47, 171, 172, 173,   4,   4,   4,   4,   1,   1, 174, 175,
      4, 176,   4,   4, 176, 177,   4, 178,   4,   4,   4, 179, 179, 180,   4, 181,
    182, 183, 184, 185, 186, 187, 188, 189, 190, 115, 191, 192,  47,   1,   1, 193,
    194, 195, 196,   4,   4, 197, 198, 199, 200, 199,   4,   4,   4, 201,   4,   4,
    202, 203, 204, 205, 206, 207, 208,   4, 209, 210, 211, 212,   4,   4,   4,   4,
      4, 207, 213,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 214,
      4,   4, 215,  47,  27,  47, 216, 216, 216, 216, 216, 216, 216, 216, 216, 217,
    216, 216, 216, 216, 198, 216, 216, 218, 216, 219, 220, 221, 222, 223, 224,   4,
    225, 226,   4,   4, 227,   4, 228,   4,   4, 229,   4, 229, 230, 216, 231, 216,
     21,   4,   4,   4,   4, 232, 233, 234, 216, 235,   4,   4, 236,   4, 237,   4,
    238, 239,   4,   4,   4, 240,   4, 241,   4,   4,   4,   4, 115, 242,  47,  47,
      4,   4,  45,   4,   4,  45,   4,   4,   4,   4,   4,   4,   4,   4, 243, 244,
      4,   4, 124,   4,   4,   4, 245, 246,   4, 215, 247, 247, 247, 247,   1,   1,
    248, 249, 250, 251,  47,  47,  47,  47, 252, 253, 252, 252, 252, 252, 252, 254,
    252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 255,  47, 256,
    257, 258, 259, 260, 261, 252, 262, 252, 263, 264, 265, 252, 262, 252, 263, 266,
    267, 252, 268, 269, 252, 252, 252, 252, 270, 252, 252, 271, 252, 252, 254, 272,
    252, 270, 252, 252, 273, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 270,
    252, 252, 252, 252,   4,   4,   4,   4, 252, 274, 252, 252, 252, 252, 252, 252,
    275, 252, 252, 252, 276,   4,   4, 170, 277,   4, 278,  47,   4,   4, 243, 279,
      4,  20,   4,   4,   4,   4,   4, 280,  45, 281, 136,  47,  47,  47,  47, 282,
    283,   4, 284, 285,   4,   4,   4, 286, 287,   4,   4, 160, 288, 146,   1, 289,
     36,   4, 290,   4, 291, 292, 125, 293,  48,   4,   4, 294, 295, 296,  47,  47,
      4,   4, 297, 174, 298, 299, 102, 153, 102, 102, 102, 102, 300, 301,  47,  47,
    302, 303, 247,  47,  47,  47,  47,  47,  47,  47,  47,  47,   4,   4, 304, 146,
    305, 306, 307, 308, 307, 309, 307, 305, 306, 307, 308, 307, 309, 307, 305, 306,
    307, 308, 307, 309, 307, 305, 306, 307, 308, 307, 309, 307, 305, 306, 307, 308,
    307, 309, 307, 305, 306, 307, 308, 307, 309, 307, 305, 306, 307, 308, 307, 309,
    307, 305, 306, 307, 308, 307, 309, 307, 308, 307, 310, 126, 311, 128, 128, 312,
    313, 313, 313, 313, 313, 313, 313, 313, 215, 314,   4, 315, 316,   4,   4,   4,
      4,   4,   4,   4, 281, 317,   4,   4,   4,   4,   4, 318,  47,   4,   4,   4,
      4, 319,   4,   4,  20,  47,  47, 320,   1, 321, 174, 322, 323, 324, 325, 179,
      4,   4,   4,   4,   4,   4,   4, 326, 327, 328, 252, 329, 252, 330, 331, 332,
      4, 333,   4,  45, 334, 335, 336, 337, 338,   4, 133, 339, 178, 178,  47,  47,
      4,   4,   4,   4,   4,   4,   4,  27, 340,   4,   4, 341,   4,   4,   4,   4,
     27, 342,  47,  47,  47,   4,   4, 343,   4, 115,   4,   4,   4,  70,  47,  47,
      4,  45, 214,   4,  27,  47,  47,  47,   4, 344,   4,   4, 345, 346,  47,  47,
      4, 178, 146,  47,  47,  47,  47,  47, 347,   4,   4, 348,   4, 349,  47,  47,
      4, 350,   4, 351,  47,  47,  47,  47, 352, 353,   4, 354,  20, 355,   4,   4,
      4,   4,   4, 356,   4, 357,   4, 358,   4,   4,   4,   4, 359,  47,  47,  47,
     47,  47,  47,  47,  47,  47,   4,  45, 165,   4,   4, 360, 361, 319, 362,  47,
    165,   4,   4, 363, 251,  47,  47,  47,   4,   4,   4,   4,   4,   4,  45,  47,
      4,   4,   4,   4,   4,   4, 364, 365,   4,   4,   4,   4,   4, 366,   4,   4,
    367,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 368,
      4,   4,  45,  47,  47,  47,  47,  47,   4,   4,   4, 359,  47,  47,  47,  47,
    369,  47,  47,  47,  47,  47,  47,  47,   4,   4,   4,   4,   4,   4,   4, 124,
      4,   4,  22,   4,   4,   4, 370,   1, 371,   4, 372,   4,   4, 178,  47,  47,
      4,   4,   4,   4, 373,  47,  47,  47,   4,   4,   4,   4,   4, 215,   4, 281,
      4,   4,   4,   4,   4, 179,   4,   4,   4, 140, 374, 375, 376,   4,   4,   4,
    377, 378,   4, 379, 380,  82,   4,   4,   4,   4, 357,   4,   4,   4,   4,   4,
      4,   4,   4,   4, 381, 382, 382, 382,   4,   4, 342,   4,   4,   4,   4,   4,
      4, 214,  45, 383,  21,  21,  47,  47, 384, 216, 385, 216, 216, 216, 386, 216,
    216, 384,  47,  47,  47,  47, 387,   4, 388, 252, 252, 271, 389, 369,  47,  47,
      4,   4,  70, 390,   4,   4,   4, 115,   4, 214,   4,   4, 391,  47,   4,  70,
      4,   4,   4,  45,  82,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 392,
      4,   4,   4, 178,  47,   4,  20,  47,  47,  47,  47,  47,  47,  47,  47, 393,
     21, 394, 395, 376, 396,  47,  47,  47,   4,   4,   4,   4, 124,  47,  47,  47,
    252, 252, 252, 252, 252, 252, 252, 268, 397,  47,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  47,
};

static RE_UINT8 re_line_break_stage_4[] = {
      0,   0,   0,   0,   1,   2,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      4,   5,   6,   7,   8,   9,  10,  11,  12,  12,  12,  12,  12,  13,  14,  15,
     14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  16,  17,  14,
     14,  14,  14,  14,  14,  16,  18,  19,   0,   0,  20,   0,   0,   0,   0,   0,
     21,  22,  23,  24,  25,  26,  27,  14,  22,  28,  29,  28,  28,  26,  28,  30,
     14,  14,  14,  24,  14,  14,  14,  14,  14,  14,  14,  24,  31,  28,  31,  14,
     25,  14,  14,  14,  28,  28,  24,  32,   0,   0,   0,   0,   0,   0,   0,  33,
      0,   0,   0,   0,   0,   0,  34,  34,  34,  35,   0,   0,   0,   0,   0,   0,
     14,  14,  14,  14,  36,  14,  14,  37,  36,  36,  14,  14,  14,  38,  38,  14,
     14,  39,  14,  14,  14,  14,  14,  14,  14,  19,   0,   0,   0,  14,  14,  14,
     14,  14,  14,  14,  36,  36,  36,  36,  39,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  38,  39,  14,  14,  14,  14,  14,  14,  14,  40,  41,  36,  36,
     42,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  43,
     19,  44,   0,  45,  36,  36,  36,  36,  14,  14,  14,  14,  14,  38,  36,  36,
     14,  14,  38,  36,  36,  36,  36,  36,  14,  14,  36,  14,  46,  47,  13,  14,
      0,   0,   0,   0,   0,  48,  36,  49,  14,  14,  14,  14,  14,  19,   0,   0,
     12,  12,  12,  12,  12,  50,  51,  14,  44,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  52,   0,   0,   0,  44,  19,   0,   0,  44,  19,  44,   0,   0,  14,
     12,  12,  12,  12,  12,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  39,
     19,  14,  14,  14,  14,  14,  14,  14,   0,   0,   0,   0,   0,  53,  39,  14,
     14,  14,  14,   0,   0,   0,   0,   0,  44,  36,  36,  36,  36,  36,  36,  36,
      0,   0,  14,  14,  54,  38,  36,  36,  14,  14,  14,   0,   0,  19,   0,   0,
      0,   0,  19,   0,  19,   0,   0,  36,  14,  14,  14,  14,  14,  14,  14,  38,
     14,  14,  14,  14,  19,   0,  36,  38,  36,  36,  36,  36,  36,  36,  36,  36,
      0,   0,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,   0,  44,   0,
     19,   0,   0,   0,  14,  14,  14,  14,  14,   0,  55,  12,  12,  12,  12,  12,
     14,  14,  14,  14,  39,  14,  14,  14,  42,   0,  39,  14,  14,  14,  38,  39,
     38,  39,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  38,  14,  14,  14,
     38,  38,  36,  14,  14,  36,  44,   0,   0,   0,  53,  42,  53,  42,   0,  38,
     36,  36,  36,  42,  36,  36,  14,  39,  14,   0,  36,  12,  12,  12,  12,  12,
     14,  47,  14,  14,  46,   9,  36,  36,  42,   0,  39,  14,  14,  38,  36,  39,
     38,  14,  39,  38,  14,  36,  53,   0,   0,  53,  36,  42,  53,  42,   0,  36,
     42,  36,  36,  36,  39,  14,  38,  38,  36,  36,  36,  12,  12,  12,  12,  12,
      0,  14,  19,  36,  36,  36,  36,  36,  42,   0,  39,  14,  14,  14,  14,  39,
     38,  14,  39,  14,  14,  36,  44,   0,   0,   0,   0,  42,   0,  42,   0,  36,
     38,  36,  36,  36,  36,  36,  36,  36,  56,  36,  36,  36,  36,  36,  36,  36,
      0,   0,  53,  42,  53,  42,   0,  36,  36,  36,  36,   0,  36,  36,  14,  39,
     36,  44,  39,  14,  14,  38,  36,  14,  38,  14,  14,  36,  39,  38,  38,  14,
     36,  39,  38,  36,  14,  38,  36,  14,  14,  14,  14,  14,  14,  36,  36,   0,
      0,  53,  36,   0,  53,   0,   0,  36,  38,  36,  36,  42,  36,  36,  36,  36,
     14,  14,  14,  14,   9,  38,  36,  36,  42,   0,  39,  14,  14,  14,  38,  14,
     38,  14,  14,  14,  14,  14,  14,  14,  14,  14,  39,  14,  14,  36,  39,   0,
      0,   0,  53,   0,  53,   0,   0,  36,  36,  36,  42,  53,  14,  36,  36,  36,
     36,  36,  36,  36,  14,  14,  14,  14,  36,   0,  39,  14,  14,  14,  38,  14,
     14,  14,  39,  14,  14,  36,  44,   0,  36,  36,  42,  53,  36,  36,  36,  38,
     39,  38,  36,  36,  36,  36,  36,  36,  14,  14,  14,  14,  14,  38,  39,   0,
      0,   0,  53,   0,  53,   0,   0,  38,  36,  36,  36,  42,  36,  36,  36,  36,
     14,  14,  14,  36,  57,  14,  14,  14,  36,   0,  39,  14,  14,  14,  14,  14,
     14,  14,  14,  38,  36,  14,  14,  14,  14,  39,  14,  14,  14,  14,  39,  36,
     14,  14,  14,  38,  36,  53,  36,  42,   0,   0,  53,  53,   0,   0,   0,   0,
     36,   0,  38,  36,  36,  36,  36,  36,  58,  59,  59,  59,  59,  59,  59,  59,
     59,  59,  59,  59,  59,  59,  59,  59,  59,  59,  59,  59,  59,  60,  36,  56,
     59,  59,  59,  59,  59,  59,  59,  61,  12,  12,  12,  12,  12,  55,  36,  36,
     58,  60,  60,  58,  60,  60,  58,  36,  36,  36,  59,  59,  58,  59,  59,  59,
     58,  59,  58,  58,  36,  59,  58,  59,  59,  59,  59,  59,  59,  58,  59,  36,
     59,  59,  60,  60,  59,  59,  59,  36,  12,  12,  12,  12,  12,  36,  59,  36,
     32,  62,  29,  62,  63,  64,  65,  49,  49,  66,  52,  14,   0,  14,  14,  14,
     14,  14,  43,  19,  19,  67,  67,   0,  14,  14,  14,  14,  14,  14,  38,  36,
     42,   0,   0,   0,   0,   0,   0,   1,   0,   0,   1,   0,  14,  14,  19,   0,
      0,   0,   0,   0,  42,   0,   0,   0,   0,   0,   0,   0,   0,   0,  53,  55,
     14,  14,  14,  44,  14,  14,  38,  14,  62,  68,  14,  14,  69,  70,  36,  36,
     12,  12,  12,  12,  12,  55,  14,  14,  12,  12,  12,  12,  12,  59,  59,  59,
     14,  14,  14,  36,  36,  36,  36,  36,  71,  71,  71,  71,  71,  71,  71,  71,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  73,  73,  73,  73,
     73,  73,  73,  73,  73,  73,  73,  73,  14,  14,  14,  14,  38,  14,  14,  36,
     14,  14,  14,  38,  38,  14,  14,  36,  38,  14,  14,  36,  14,  14,  14,  38,
     38,  14,  14,  36,  14,  14,  14,  14,  14,  14,  14,  38,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  38,  42,   0,  27,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  36,  36,  36,  74,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  16,  75,  36,  14,  14,  14,  14,  14,  27,  55,  14,
     14,  14,  14,  14,  14,  14,  38,  14,  14,   0,  53,  36,  36,  36,  36,  36,
     14,   0,   1,  41,  36,  36,  36,  36,  14,   0,  36,  36,  36,  36,  36,  36,
     38,   0,  36,  36,  36,  36,  36,  36,  59,  59,  55,  76,  74,  77,  59,  36,
     12,  12,  12,  12,  12,  36,  36,  36,  14,  49,  55,  29,  49,  19,   0,  70,
     14,  14,  14,  14,  19,  38,  36,  36,   0,   0,   0,   0,   0,   0,  36,  36,
     38,  36,  49,  12,  12,  12,  12,  12,  59,  59,  59,  59,  59,  59,  59,  36,
     59,  59,  60,  36,  36,  36,  36,  36,  59,  59,  59,  59,  59,  59,  36,  36,
     59,  59,  59,  59,  59,  36,  36,  36,  12,  12,  12,  12,  12,  60,  36,  59,
     14,  14,  14,  19,   0,   0,  36,  14,  59,  59,  59,  59,  59,  59,  59,  60,
     59,  59,  59,  59,  59,  59,  60,  42,   0,   0,  44,  14,  14,  14,  14,  14,
     14,  14,   0,   0,   0,   0,   0,   0,   0,   0,  44,  14,  14,  14,  36,  36,
     12,  12,  12,  12,  12,  55,  27,  55,  74,  14,  14,  14,  14,  19,   0,   0,
      0,   0,  14,  14,  14,  14,  38,  36,   0,  44,  14,  14,  14,  14,  14,  14,
     19,   0,   0,   0,   0,  53,  36,  14,   0,   0,  36,  36,  36,  36,  14,  14,
      0,   0,   0,   0,  36,  78,  55,  55,  12,  12,  12,  12,  12,  36,  39,  14,
     14,  14,  14,  14,  14,  14,  14,  55,   0,  44,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  44,  14,  19,  14,  14,  53,  36,  36,  36,  36,  36,  36,
      0,   0,   0,  53,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,   0,   0,
     14,  14,  14,  36,  14,  14,  14,  36,  14,  14,  14,  14,  39,  39,  39,  39,
     14,  14,  14,  14,  14,  14,  14,  36,  14,  14,  38,  14,  14,  14,  14,  14,
     14,  14,  36,  14,  14,  14,  39,  14,  36,  14,  38,  14,  14,  14,  32,  38,
     55,  55,  55,  79,  55,  80,   0,   0,  79,  55,  81,  25,  82,  83,  82,  83,
     28,  14,  84,  85,  86,   0,   0,  33,  47,  47,  47,  47,   7,  87,  88,  14,
     14,  14,  89,  90,  88,  14,  14,  14,  14,  14,  14,  74,  55,  55,  27,  55,
     91,  14,  38,  36,  36,   0,   0,   0,  14,  36,  25,  14,  14,  14,  16,  92,
     24,  28,  25,  14,  14,  14,  16,  75,  23,  23,  23,   6,  23,  23,  23,  23,
     23,  23,  23,  22,  23,  36,  36,  36,  53,  36,  36,  36,  36,  36,  36,  36,
     14,  46,  24,  14,  46,  14,  14,  14,  14,  24,  14,  93,  14,  14,  14,  14,
     24,  25,  14,  14,  14,  24,  14,  14,  14,  14,  28,  14,  14,  24,  14,  25,
     28,  28,  28,  28,  28,  28,  14,  14,  28,  28,  28,  28,  28,  14,  14,  14,
     14,  14,  14,  14,  24,  36,  36,  36,  14,  25,  25,  14,  14,  14,  14,  14,
     25,  28,  14,  24,  25,  24,  14,  24,  24,  23,  24,  14,  14,  25,  24,  28,
     25,  24,  24,  24,  28,  28,  25,  25,  14,  14,  28,  28,  14,  14,  28,  14,
     14,  14,  14,  14,  25,  14,  25,  14,  14,  25,  14,  14,  14,  14,  14,  14,
     28,  14,  28,  28,  14,  28,  14,  28,  14,  28,  14,  28,  14,  14,  14,  14,
     14,  14,  24,  14,  24,  14,  14,  14,  14,  14,  24,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  24,  14,  14,  14,  14,  16,  94,  14,  14,
     14,  14,  36,  36,  36,  36,  36,  36,  14,  14,  14,  38,  36,  36,  36,  36,
     28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  25,
     28,  28,  25,  14,  14,  14,  14,  14,  14,  28,  28,  14,  14,  14,  14,  14,
     28,  24,  28,  28,  28,  14,  14,  14,  14,  28,  14,  28,  14,  14,  28,  14,
     28,  14,  14,  28,  25,  24,  14,  28,  28,  14,  14,  14,  14,  14,  14,  14,
     14,  28,  28,  14,  14,  14,  14,  24,  14,  14,  24,  25,  24,  14,  14,  28,
     14,  14,  28,  28,  14,  14,  25,  25,  25,  25,  14,  14,  14,  14,  14,  14,
     28,  24,  28,  24,  28,  25,  28,  24,  14,  14,  14,  14,  14,  14,  14,  28,
     14,  14,  28,  28,  28,  28,  28,  24,  28,  24,  14,  14,  28,  28,  28,  28,
     14,  14,  14,  24,  14,   7,  82,   5,  14,  49,  14,  14,  67,  67,  67,  67,
     67,  67,  67,  28,  28,  28,  28,  28,  28,  28,  14,  14,  14,  14,  14,  14,
     14,  14,  16,  94,  14,  38,  38,  14,  14,  14,  14,  67,  67,  67,  67,  67,
     14,  16,  95,  95,  95,  95,  95,  95,  95,  95,  95,  95,  94,  14,  14,  14,
     14,  14,  14,  14,  67,  67,  14,  14,  14,  14,  14,  14,  14,  14,  67,  14,
     14,  14,  24,  28,  28,  36,  36,  36,  14,  14,  14,  14,  14,  14,  14,  19,
      0,  36,  36,  36,  96,  55,  74,  97,  14,  14,  14,  36,  36,  36,  36,  39,
     41,  36,  36,  36,  36,  36,  36,  42,  14,  14,  14,  38,  14,  14,  14,  38,
     82,  82,  82,  82,  82,  82,  82,  55,  55,  55,  55,  27,  98,  14,  82,  14,
     82,  67,  67,  67,  67,  55,  55,  52,  55,  36,  36,  36,  36,  36,  36,  36,
     99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99, 100,  99,  99,
     99,  99,  36,  36,  36,  36,  36,  36,  99,  99,  99,  36,  36,  36,  36,  36,
     99,  99,  99,  99,  99,  99,  36,  36, 101, 102, 103,  99,  67,  67,  67,  67,
     67,  99,  67,  67,  67,  67, 104, 105,  99,  99,  99,  99,  99,   0,   0,   0,
     99,  99,  99,  99,  99, 103, 106,  99, 107, 103, 103, 103, 103,  99,  99,  99,
     99, 103,  99,  99,  99,  99,  99,  99,  99, 103, 103, 103,  99,  99,  99, 106,
     99,  99, 103, 108,  42, 109,  88, 106,  88, 103, 103, 103, 103,  99,  99,  99,
     99,  99, 103, 106,  99, 103,  88, 106,  36,  36, 100,  99,  99,  99,  99,  99,
     99,  99,  99,  99,  99,  99,  99,  36, 100,  99,  99,  99,  99,  99,  99,  99,
     99,  99,  99,  99,  99,  99,  99, 110,  99,  99,  99,  99,  99, 110,  36,  36,
     88,  88,  88,  88,  88,  88,  88,  88,  99,  99,  99,  99,  28,  28,  28,  28,
     99,  99, 103,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99, 110,  36,
     99,  99,  99, 110,  36,  36,  36,  36,  14,  14,  14,  14,  14,  14,  27,  97,
     12,  12,  12,  12,  12,  14,  36,  36,   0,  44,  36,  36,  36,  36,   0,  14,
      0,  27,  55,  55,  36,  36,  36,  36,  14,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  14,  14,  14,  14,  44,  14,  44,  14,  19,  14,  14,
     14,  19,   0,   0,  14,  14,  36,  36,  14,  14,  14,  14, 111,  36,  36,  36,
     14,  14,  62,  49,  36,  36,  36,  36,   0,  14,  14,  14,  14,  14,  14,  14,
      0,   0,  53,  36,  36,  36,  36,  55,   0,  14,  14,  14,  14,  14,  36,  36,
     14,  14,  14,   0,   0,   0,   0,  55,  14,  14,  14,  19,   0,   0,   0,   0,
      0,   0,  36,  36,  36,  36,  36,  39,  71,  71,  71,  71,  71,  71, 112,  36,
     14,  19,   0,   0,   0,   0,   0,   0,  44,  14,  14,  27,  55,  14,  14,  39,
     12,  12,  12,  12,  12,  36,  36,  14,  14,  14,  14,  14,  19,   0,   0,   0,
     14,  19,  14,  14,  14,  14,   0,  36,  12,  12,  12,  12,  12,  36,  27,  55,
     59,  60,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  58,  59,  59,
     39,  14,  14,  38,  39,  14,  14,  38,  39,  14,  14,  38,  36,  36,  36,  36,
     14,  19,   0,   0,   0,   1,   0,  36, 113, 114, 114, 114, 114, 114, 114, 114,
    114, 114, 114, 114, 114, 114, 113, 114, 114, 114, 114, 114, 114, 114, 114, 114,
    114, 114, 114, 114, 113, 114, 114, 114, 114, 114, 113, 114, 114, 114, 114, 114,
    114, 114,  36,  36,  36,  36,  36,  36,  72,  72,  72, 115,  36, 116,  73,  73,
     73,  73,  73,  73,  73,  73,  36,  36, 117, 117, 117, 117, 117, 117, 117, 117,
     36,  39,  14,  14,  36,  36,  39,  44,  14,  14,  14,  38,  14,  14,  38,  38,
     14,  39,  38,  14,  14,  14,  14,  14,  36,  39,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  67,  36,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14, 111,  36, 118, 119,  54, 120, 121,  36,  36,  36,
     99,  99, 122,  95,  95,  95,  95,  95,  95,  95, 102, 122, 102,  99,  99,  99,
    102,  75,  88,  49, 122,  95,  95, 102,  99,  99,  99, 110, 123, 124,  36,  36,
     14,  14,  14,  14,  14,  14,  38, 125,  96,  99,   6,  99,  67,  99, 102, 102,
     99,  99,  99,  99,  99,  88,  99, 126,  99,  99,  99,  99,  99, 122, 101,  99,
     99,  99,  99,  99,  99, 122, 101, 122, 105,  67,  90, 127,  88,  88,  88,  88,
    128,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  88,
     36,  14,  14,  14,  36,  14,  14,  14,  36,  14,  14,  14,  36,  14,  38,  36,
     22,  99, 123, 129,  14,  14,  14,  38,  36,  36,  36,  36,  42,   0, 130,  36,
     14,  14,  14,  14,  14,  14,  39,  14,  14,  14,  14,  14,  14,  38,  14,  39,
     55,  41,  36,  39,  14,  14,  14,  14,  14,  14,  36,  39,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  36,  36,  14,  14,  14,  14,  14,  14,  19,  36,
     14,  14,  14,  14,  14,  14,  14,  78,  14,  14,  36,  36,  14,  14,  14,  14,
     74,  14,  14,  36,  36,  36,  36,  36,  14,  14,  14,  36,  38,  14,  14,  14,
     14,  14,  14,  39,  38,  36,  38,  39,  14,  14,  14,  78,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  36,  78,  14,  14,  14,  14,  14,  36,  36,  39,
     19,   0,  42,  53,  36,  36,   0,   0,  14,  14,  39,  14,  39,  14,  14,  14,
     14,  14,  36,  36,   0,  53,  36,  42,  55,  55,  55,  55,  38,  36,  36,  36,
     14,  14,  14,  36,  78,  55,  55,  55,  14,  14,  14,  36,  14,  14,  14,  14,
     14,  38,  36,  36,  14,  14,  14,  14,  14,  14,  14,  14,  38,  36,  36,  36,
     14,  14,  14,  14,   0,   0,   0,   0,   0,   0,   0,   1,  74,  14,  14,  36,
     14,  14,  14,  12,  12,  12,  12,  12,   0,   0,   0,   0,   0,  44,  14,  55,
     14,  38,  36,  36,  36,  36,  36,  36,  55,  55,  36,  36,  36,  36,  36,  36,
     14,  14,  14,  14, 131,  67, 105,  14,  14,  94,  14,  67,  67,  14,  14,  14,
     14,  14,  14,  14,  16, 105,  14,  14,  99,  36,  36,  36,  36,  36,  36,  36,
     14,  14,  19,   0,   0,  14,  19,   0,   0,  44,  19,   0,   0,   0,  14,  14,
     14,  14,  14,  14,  14,   0,   0,  14,  14,   0,  44,  36,  36,  36,  36,  36,
     36,  38,  39,  38,  39,  14,  38,  14,  14,  14,  14,  14,  14,  39,  39,  14,
     14,  14,  39,  14,  14,  14,  14,  14,  14,  14,  14,  39,  14,  38,  39,  14,
     14,  14,  38,  14,  14,  14,  38,  14,  14,  14,  14,  14,  14,  39,  14,  38,
     14,  14,  38,  38,  36,  14,  14,  14,  14,  14,  14,  14,  14,  14,  36,  12,
     12,  12,  12,  12,  12,  12,  12,  12,  39,  14,  14,  14,  14,  14,  14,  38,
     28,  28,  28,  28,  28, 132,  36,  36,  28,  28,  28,  28,  28,  28,  28,  38,
     28,  28,  28,  28,  28,  36,  36,  36,  36,  36,  36,  14,  14,  14,  14,  14,
     99, 110,  36,  36,  36,  36,  36,  36,  99,  99,  99,  99, 110,  36,  36,  36,
     14,  14,  14,  39,  14,  14,  14,  14,  14,  14,  38,  14,  14,  38,  36,  36,
     14,  14,  14,  14,  39,  14,  38,  36,  36,  36,  36,  36,  36,  39,  14,  14,
     38,  14,  38,  38,  38,  38,  14,  38,  14,  14,  14,  36,  14,  14,  39,  36,
     38,  36,  39,  14,  14,  14,  14,  14,  42,  36,  36,  36,  36,  36,  36,  36,
};

static RE_UINT8 re_line_break_stage_5[] = {
    16, 16, 16, 18, 22, 20, 20, 21, 19,  6,  3, 12,  9, 10, 12,  3,
     1, 36, 12,  9,  8, 15,  8,  7, 11, 11,  8,  8, 12, 12, 12,  6,
    12,  1,  9, 36, 18,  2, 12, 16, 16, 29,  4,  1, 10,  9,  9,  9,
    12, 25, 25, 12, 25,  3, 12, 18, 25, 25, 17, 12, 25,  1, 17, 25,
    12, 17, 16,  4,  4,  4,  4, 16,  0,  0,  8,  0, 12,  0,  0, 12,
     0,  8, 18,  0,  0, 16, 18, 16, 16, 12,  6, 16, 12, 10, 10, 10,
    16,  6,  6,  6, 10, 11, 11, 12,  6, 12, 16,  0,  8,  6, 18, 18,
     0,  9,  0, 10,  0, 24, 24, 24, 24,  0, 24, 12, 17, 17,  4, 17,
    17, 18,  4,  6,  4, 12,  1,  2, 18, 17, 12,  4,  4,  0, 31, 31,
    32, 32, 33, 33, 18, 12,  2,  0,  5, 24, 18,  9,  0, 18, 18,  4,
    18, 28, 26, 25,  3,  3,  1,  3, 14, 14, 14, 18, 20, 20,  3, 25,
     5,  5,  8,  1,  2,  5, 30, 12,  2, 25,  9, 12,  2, 12,  2,  1,
     0,  6,  6, 18,  1, 18, 13, 13,  0, 13, 13,  2,  2, 13, 13,  5,
     5,  1,  2,  2,  5, 13,  0,  5,  5,  0, 16,  5, 13,  0, 10, 12,
    31,  0, 34, 35, 35, 35, 32,  0,  0, 33, 27, 27,  8,  2,  2,  8,
     6,  1,  2, 14, 13,  1, 13,  9, 10, 13,  0, 30, 13,  6, 12,  5,
     5, 12,  9,  0, 23, 25,  1,  1, 25,  0,
};

/* Line_Break: 7114 bytes. */

RE_UINT32 re_get_line_break(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_line_break_stage_1[f] << 5;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_line_break_stage_2[pos + f] << 3;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_line_break_stage_3[pos + f] << 3;
    f = code >> 1;
    code ^= f << 1;
    pos = (RE_UINT32)re_line_break_stage_4[pos + f] << 1;
    value = re_line_break_stage_5[pos + code];

    return value;
}

/* Numeric_Type. */

static RE_UINT8 re_numeric_type_stage_1[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 11, 11, 11, 12,
    13, 14, 15, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 16, 11, 17,
    18, 11, 19, 20, 11, 11, 21, 11, 11, 11, 11, 11, 11, 11, 11, 22,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
};

static RE_UINT8 re_numeric_type_stage_2[] = {
     0,  1,  1,  1,  1,  1,  2,  3,  1,  4,  5,  6,  7,  8,  9, 10,
    11,  1,  1, 12,  1,  1, 13, 14, 15, 16, 17, 18, 19,  1,  1,  1,
    20, 21,  1,  1, 22,  1,  1, 23,  1,  1,  1,  1, 24,  1,  1,  1,
    25, 26, 27,  1, 28,  1,  1,  1, 29,  1,  1, 30,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 31, 32,
     1, 33,  1, 34,  1,  1, 35,  1, 36,  1,  1,  1,  1,  1, 37, 38,
     1,  1, 39, 40,  1,  1,  1, 41,  1,  1,  1,  1,  1,  1,  1, 42,
     1,  1,  1, 43,  1,  1, 44,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    45,  1,  1,  1, 46,  1,  1,  1,  1,  1,  1,  1, 47, 48,  1,  1,
     1,  1,  1,  1,  1,  1, 49,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1, 50,  1, 51, 52, 53, 54,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1, 55,  1,  1,  1,  1,  1, 15,
     1, 56,  1, 57, 58,  1,  1,  1, 59, 60, 61, 62,  1,  1, 63,  1,
    64,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1, 65,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1, 66,  1,  1,  1, 67,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 68,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    69, 70,  1,  1,  1,  1,  1,  1,  1, 71, 72, 73,  1,  1,  1,  1,
     1,  1,  1, 74,  1,  1,  1,  1,  1, 75,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 76,  1,  1,  1,  1,
     1,  1, 77,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 74,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_numeric_type_stage_3[] = {
     0,  1,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  3,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  3,  0,
     0,  0,  0,  4,  0,  0,  0,  5,  0,  0,  0,  4,  0,  0,  0,  4,
     0,  0,  0,  6,  0,  0,  0,  7,  0,  0,  0,  8,  0,  0,  0,  4,
     0,  0,  0,  9,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,
     0, 10,  0,  0,  0,  0,  0,  0,  0,  0,  3,  0,  1,  0,  0,  0,
     0,  0,  0, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12,
     0,  0,  0,  0,  0,  0,  0, 13,  1,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  4,  0,  0,  0, 14,  0,  0,  0,  0,  0, 15,  0,  0,  0,
     0,  0,  1,  0,  0,  1,  0,  0,  0,  0, 15,  0,  0,  0,  0,  0,
     0,  0,  0, 16, 17,  0,  0,  0,  0,  0, 18, 19, 20,  0,  0,  0,
     0,  0,  0, 21, 22,  0,  0, 23,  0,  0,  0, 24, 25,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 26, 27, 28,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0, 29,  0,  0,  0,  0, 30, 31,  0, 30, 31,  0,  0,
    32,  0,  0,  0, 33,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0,  0,
     0,  0, 35,  0,  0,  0,  0,  0, 36,  0, 26,  0, 37, 38, 39, 40,
    35,  0,  0, 41,  0,  0,  0,  0, 42,  0, 43, 44,  0,  0,  0,  0,
     0,  0, 45,  0,  0,  0, 46,  0,  0,  0,  0,  0,  0,  0, 47,  0,
     0,  0,  0,  0,  0,  0,  0, 48,  0,  0,  0, 49,  0,  0,  0, 50,
    51,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 52,
     0,  0, 53,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 54,  0,
    43,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55,  0,  0,  0,
     0,  0,  0, 52,  0,  0,  0,  0,  0,  0,  0,  0, 43,  0,  0,  0,
     0, 53,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 56,  0,  0,
     0, 41,  0,  0,  0,  0,  0,  0,  0, 57, 58, 59,  0,  0,  0, 55,
     0,  3,  0,  0,  0,  0,  0, 60,  0, 61,  0,  0,  0,  0,  1,  0,
     3,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0, 62,  0, 54, 63, 26,
    64, 65, 19, 66, 34,  0,  0,  0,  0, 67, 68,  0,  0,  0, 69,  0,
     0,  0,  0,  0,  0,  3,  0,  0,  0,  0, 70,  0,  0,  0,  0,  0,
    71,  0,  0,  0,  0,  0,  0,  0,  0,  0, 72, 73,  0,  0,  0,  0,
     0,  0, 70, 70,  0,  0,  0,  0,  0,  0,  0, 74,  0,  0,  0,  0,
     0,  0, 75, 76,  0,  0,  0,  0, 19, 77, 78, 79,  0,  0,  0,  0,
     0,  0,  0, 80,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 81, 82,
    83,  0,  0,  0,  0,  0,  0,  0, 57,  0,  0, 42,  0,  0,  0, 84,
     0, 57,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0, 85,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 86, 87,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0, 41,  0,  0,  0,  0,  0,  0,  0, 59,  0,  0,  0,
    47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,
};

static RE_UINT8 re_numeric_type_stage_4[] = {
     0,  0,  0,  0,  0,  0,  1,  2,  0,  0,  3,  4,  1,  2,  0,  0,
     5,  1,  0,  0,  5,  1,  6,  7,  5,  1,  8,  0,  5,  1,  9,  0,
     5,  1,  0, 10,  5,  1, 11,  0,  1, 12, 13,  0,  0, 14, 15, 16,
     0, 17, 18,  0,  1,  2, 19,  7,  0,  0,  1, 20,  1,  2,  1,  2,
     0,  0, 21, 22, 23, 22,  0,  0,  0,  0, 19, 19, 19, 19, 19, 19,
    24,  7,  0,  0, 23, 25, 26, 27, 19, 23, 25, 13,  0, 28, 29, 30,
     0,  0, 31, 32, 23, 33, 34,  0,  0,  0,  0, 35, 36,  0,  0,  0,
    37,  7,  0,  9,  0,  0, 38,  0, 19,  7,  0,  0,  0,  0, 37, 19,
    35,  0,  0,  0, 39,  0,  0,  0,  0, 40,  0,  0,  0, 35,  0,  0,
    41, 42,  0,  0,  0, 43, 44,  0,  0,  0,  0, 36, 18,  0,  0, 36,
     0, 18,  0,  0,  0,  0, 18,  0, 43,  0,  0,  0, 45,  0,  0,  0,
     0, 46,  0,  0, 47, 43,  0,  0, 48,  0,  0,  0,  0,  0,  0, 39,
     0,  0, 42, 42,  0,  0,  0, 40,  0,  0,  0, 17,  0, 49, 18,  0,
     0,  0,  0, 45,  0, 43,  0,  0,  0,  0, 40,  0,  0,  0, 45,  0,
     0, 45, 39,  0, 42,  0,  0,  0, 45, 43,  0,  0,  0,  0,  0, 18,
    17, 19,  0,  0,  0,  0, 11,  0,  0, 39, 39, 18,  0,  0, 50,  0,
    36, 19, 19, 19, 19, 19, 13,  0, 19, 19, 19, 18, 13,  0,  0,  0,
    42, 40,  0,  0,  0,  0, 51,  0,  0,  0,  0, 19,  0,  0, 17, 13,
    52,  0,  0,  0,  0,  0,  0, 53, 23, 25, 19, 10,  0,  0, 54, 55,
    56,  1,  0,  0, 19, 19, 57, 19, 19, 19, 11, 19,  9,  0,  0,  0,
    19, 19,  7,  0,  0,  5,  1,  1,  1,  1,  1,  1, 23, 58,  0,  0,
    40,  0,  0,  0, 39, 43,  0, 43,  0, 40,  0, 35,  0,  0,  0, 42,
};

static RE_UINT8 re_numeric_type_stage_5[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0,
    0, 2, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 3,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    3, 3, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 2,
    2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 0, 0, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
    0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 1, 2, 0, 0, 0, 0, 0, 0, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
    2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 0, 0, 1, 1, 1, 1,
    2, 2, 2, 0, 0, 0, 0, 0,
};

/* Numeric_Type: 2088 bytes. */

RE_UINT32 re_get_numeric_type(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_numeric_type_stage_1[f] << 4;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_numeric_type_stage_2[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_numeric_type_stage_3[pos + f] << 2;
    f = code >> 3;
    code ^= f << 3;
    pos = (RE_UINT32)re_numeric_type_stage_4[pos + f] << 3;
    value = re_numeric_type_stage_5[pos + code];

    return value;
}

/* Numeric_Value. */

static RE_UINT8 re_numeric_value_stage_1[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 11, 11, 11, 12,
    13, 14, 15, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 16, 11, 17,
    18, 11, 19, 20, 11, 11, 21, 11, 11, 11, 11, 11, 11, 11, 11, 22,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
};

static RE_UINT8 re_numeric_value_stage_2[] = {
     0,  1,  1,  1,  1,  1,  2,  3,  1,  4,  5,  6,  7,  8,  9, 10,
    11,  1,  1, 12,  1,  1, 13, 14, 15, 16, 17, 18, 19,  1,  1,  1,
    20, 21,  1,  1, 22,  1,  1, 23,  1,  1,  1,  1, 24,  1,  1,  1,
    25, 26, 27,  1, 28,  1,  1,  1, 29,  1,  1, 30,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 31, 32,
     1, 33,  1, 34,  1,  1, 35,  1, 36,  1,  1,  1,  1,  1, 37, 38,
     1,  1, 39, 40,  1,  1,  1, 41,  1,  1,  1,  1,  1,  1,  1, 42,
     1,  1,  1, 43,  1,  1, 44,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    45,  1,  1,  1, 46,  1,  1,  1,  1,  1,  1,  1, 47, 48,  1,  1,
     1,  1,  1,  1,  1,  1, 49,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1, 50,  1, 51, 52, 53, 54,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1, 55,  1,  1,  1,  1,  1, 15,
     1, 56,  1, 57, 58,  1,  1,  1, 59, 60, 61, 62,  1,  1, 63,  1,
    64,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1, 65,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1, 66,  1,  1,  1, 67,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 68,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    69, 70,  1,  1,  1,  1,  1,  1,  1, 71, 72, 73,  1,  1,  1,  1,
     1,  1,  1, 74,  1,  1,  1,  1,  1, 75,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 76,  1,  1,  1,  1,
     1,  1, 77,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 78,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_numeric_value_stage_3[] = {
      0,   1,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   3,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   3,   0,
      0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   4,   0,   0,   0,   4,
      0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   4,
      0,   0,   0,   9,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   1,   0,
      0,  10,   0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   1,   0,   0,   0,
      0,   0,   0,  11,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  12,
      0,   0,   0,   0,   0,   0,   0,  13,   1,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   4,   0,   0,   0,  14,   0,   0,   0,   0,   0,  13,   0,   0,   0,
      0,   0,   1,   0,   0,   1,   0,   0,   0,   0,  13,   0,   0,   0,   0,   0,
      0,   0,   0,  15,   3,   0,   0,   0,   0,   0,  16,  17,  18,   0,   0,   0,
      0,   0,   0,  19,  20,   0,   0,  21,   0,   0,   0,  22,  23,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  24,  25,  26,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  27,   0,   0,   0,   0,  28,  29,   0,  28,  30,   0,   0,
     31,   0,   0,   0,  32,   0,   0,   0,   0,  33,   0,   0,   0,   0,   0,   0,
      0,   0,  34,   0,   0,   0,   0,   0,  35,   0,  36,   0,  37,  38,  39,  40,
     41,   0,   0,  42,   0,   0,   0,   0,  43,   0,  44,  45,   0,   0,   0,   0,
      0,   0,  46,   0,   0,   0,  47,   0,   0,   0,   0,   0,   0,   0,  48,   0,
      0,   0,   0,   0,   0,   0,   0,  49,   0,   0,   0,  50,   0,   0,   0,  51,
     52,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  53,
      0,   0,  54,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  55,   0,
     56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  57,   0,   0,   0,
      0,   0,   0,  58,   0,   0,   0,   0,   0,   0,   0,   0,  59,   0,   0,   0,
      0,  60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  61,   0,   0,
      0,  62,   0,   0,   0,   0,   0,   0,   0,  63,  64,  65,   0,   0,   0,  66,
      0,   3,   0,   0,   0,   0,   0,  67,   0,  68,   0,   0,   0,   0,   1,   0,
      3,   0,   0,   0,   0,   0,   1,   0,   0,   0,   1,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,  69,   0,  70,  71,  72,
     73,  74,  75,  76,  77,   0,   0,   0,   0,  78,  79,   0,   0,   0,  80,   0,
      0,   0,   0,   0,   0,   3,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,
     82,   0,   0,   0,   0,   0,   0,   0,   0,   0,  83,  84,   0,   0,   0,   0,
      0,   0,  85,  85,   0,   0,   0,   0,   0,   0,   0,  86,   0,   0,   0,   0,
      0,   0,  87,  88,   0,   0,   0,   0,  89,  90,  91,  92,   0,   0,   0,   0,
      0,   0,   0,  93,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  94,  95,
     96,   0,   0,   0,   0,   0,   0,   0,  97,   0,   0,  98,   0,   0,   0,  99,
      0, 100,   0,   0,   0,   0,   0,   0,   0, 101,   0,   0, 102,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 103, 104,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  62,   0,   0,   0,   0,   0,   0,   0, 105,   0,   0,   0,
    106,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 107,   0,   0,   0,   0,
      0,   0,   0,   0, 108,   0,   0,   0,
};

static RE_UINT8 re_numeric_value_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   3,   0,
      0,   0,   0,   0,   4,   0,   5,   6,   1,   2,   3,   0,   0,   0,   0,   0,
      0,   7,   8,   9,   0,   0,   0,   0,   0,   7,   8,   9,   0,  10,  11,   0,
      0,   7,   8,   9,  12,  13,   0,   0,   0,   7,   8,   9,  14,   0,   0,   0,
      0,   7,   8,   9,   0,   0,   1,  15,   0,   7,   8,   9,  16,  17,   0,   0,
      1,   2,  18,  19,  20,   0,   0,   0,   0,   0,  21,   2,  22,  23,  24,  25,
      0,   0,   0,  26,  27,   0,   0,   0,   1,   2,   3,   0,   1,   2,   3,   0,
      0,   0,   0,   0,   1,   2,  28,   0,   0,   0,   0,   0,  29,   2,   3,   0,
      0,   0,   0,   0,  30,  31,  32,  33,  34,  35,  36,  37,  34,  35,  36,  37,
     38,  39,  40,   0,   0,   0,   0,   0,  34,  35,  36,  41,  42,  34,  35,  36,
     41,  42,  34,  35,  36,  41,  42,   0,   0,   0,  43,  44,  45,  46,   2,  47,
      0,   0,   0,   0,   0,  48,  49,  50,  34,  35,  51,  49,  50,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  52,   0,  53,   0,   0,   0,   0,   0,   0,
     21,   2,   3,   0,   0,   0,  54,   0,   0,   0,   0,   0,  48,  55,   0,   0,
     34,  35,  56,   0,   0,   0,   0,   0,   0,   0,   0,   0,  57,  58,  59,  60,
      0,   0,   0,   0,  61,  62,  63,  64,   0,  65,   0,   0,   0,   0,   0,   0,
     66,   0,   0,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,   0,   0,
      0,   0,   0,  68,   0,   0,   0,   0,  69,  70,  71,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  72,   0,   0,   0,  73,   0,  74,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  75,  76,   0,   0,   0,   0,   0,   0,  77,
      0,   0,  78,   0,   0,   0,   0,   0,   0,   0,   0,  65,   0,   0,   0,   0,
      0,   0,   0,   0,  79,   0,   0,   0,   0,  80,   0,   0,   0,   0,   0,   0,
      0,  81,   0,   0,   0,   0,   0,   0,   0,   0,  82,  83,   0,   0,   0,   0,
     84,  85,   0,  86,   0,   0,   0,   0,  87,  78,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  88,   0,   0,   0,   0,   0,   5,   0,   5,   0,
      0,   0,   0,   0,   0,   0,  89,   0,   0,   0,   0,   0,   0,   0,   0,  90,
      0,   0,   0,  15,  73,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  91,
      0,   0,   0,  92,   0,   0,   0,   0,   0,   0,   0,   0,  93,   0,   0,   0,
      0,  93,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  94,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  95,   0,  96,   0,   0,   0,   0,   0,   0,
      0,   0,   0,  25,   0,   0,   0,   0,   0,   0,   0,  97,  66,   0,   0,   0,
      0,   0,   0,   0,  73,   0,   0,   0,  98,   0,   0,   0,   0,   0,   0,   0,
      0,  99,   0,  79,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 100,   0,
      0,   0,   0,   0,   0, 101,   0,   0,   0,  48,  49, 102,   0,   0,   0,   0,
      0,   0,   0,   0, 103, 104,   0,   0,   0,   0, 105,   0, 106,   0,  73,   0,
      0,   0,   0,   0, 101,   0,   0,   0,   0,   0,   0,   0, 107,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 108,   0, 109,   8,   9, 110, 111, 112, 113,
    114, 115, 116, 117, 118,   0,   0,   0, 119, 120, 121, 122, 123, 124, 125, 126,
    127, 128, 129, 130, 122, 131, 132,   0,   0,   0, 101,   0,   0,   0,   0,   0,
    133,   0,   0,   0,   0,   0,   0,   0, 134,   0, 135,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 136, 137,   0,   0,   0,   0,   0,   0,   0,   0, 138, 139,
      0,   0,   0,   0,   0, 140, 141,   0,  34, 142,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 143,   0,   0,   0,   0,   0,   0,  34, 142,
     34,  35, 144, 145, 146, 147, 148, 149,   0,   0,   0,   0,  48,  49,  50, 150,
    151, 152,   8,   9,   0,   0,   0,   0,   8,   9,  49, 153,  35, 154,   2, 155,
    156, 157,   9, 158, 159, 158, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
    170,   0,   0,   0,   0,   0,   0,   0,  34,  35, 144, 145, 171,   0,   0,   0,
      0,   0,   0,   7,   8,   9,   1,   2, 172,   8,   9,   1,   2, 172,   8,   9,
    173,  49, 174,   0,   0,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,
      0, 175,   0,   0,   0,   0,   0,   0,  96,   0,   0,   0,   0,   0,   0,   0,
     65,   0,   0,   0,   0,   0,   0,   0,   0,   0,  89,   0,   0,   0,   0,   0,
    176,   0,   0,  86,   0,   0,   0,  86,   0,   0,  99,   0,   0,   0,   0,  71,
      0,   0,   0,   0,   0,   0,  71,   0,   0,   0,   0,   0,   0,   0,  78,   0,
      0,   0,   0,   0,   0,   0, 105,   0,   0,   0,   0, 177,   0,   0,   0,   0,
      0,   0,   0,   0, 178,   0,   0,   0,
};

static RE_UINT8 re_numeric_value_stage_5[] = {
      0,   0,   0,   0,   2,  23,  25,  27,  29,  31,  33,  35,  37,  39,   0,   0,
      0,   0,  25,  27,   0,  23,   0,   0,  11,  15,  19,   0,   0,   0,   2,  23,
     25,  27,  29,  31,  33,  35,  37,  39,   3,   6,   9,  11,  19,  46,   0,   0,
      0,   0,  11,  15,  19,   3,   6,   9,  40,  85,  94,   0,  23,  25,  27,   0,
     40,  85,  94,  11,  15,  19,   0,   0,  37,  39,  15,  24,  26,  28,  30,  32,
     34,  36,  38,   1,   0,  23,  25,  27,  37,  39,  40,  50,  60,  70,  80,  81,
     82,  83,  84,  85, 103,   0,   0,   0,   0,   0,  47,  48,  49,   0,   0,   0,
     37,  39,  23,   0,   2,   0,   0,   0,   7,   5,   4,  12,  18,  10,  14,  16,
     20,   8,  21,   6,  13,  17,  22,  23,  23,  25,  27,  29,  31,  33,  35,  37,
     39,  40,  41,  42,  80,  85,  89,  94,  94,  98, 103,   0,   0,  33,  80, 107,
    112,   2,   0,   0,  43,  44,  45,  46,  47,  48,  49,  50,   0,   0,   2,  41,
     42,  43,  44,  45,  46,  47,  48,  49,  50,  23,  25,  27,  37,  39,  40,   2,
      0,   0,  23,  25,  27,  29,  31,  33,  35,  37,  39,  40,  39,  40,  23,  25,
      0,  15,   0,   0,   0,   0,   0,   2,  40,  50,  60,   0,  27,  29,   0,   0,
     39,  40,   0,   0,   0,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,
     62,  63,  64,  65,   0,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,
     77,  78,  79,  80,   0,  31,   0,   0,   0,   0,   0,  25,   0,   0,  31,   0,
      0,  35,   0,   0,  23,   0,   0,  35,   0,   0,   0, 103,   0,  27,   0,   0,
      0,  39,   0,   0,  25,   0,   0,   0,  31,   0,  29,   0,   0,   0,   0, 113,
     40,   0,   0,   0,   0,   0,   0,  94,  27,   0,   0,   0,  85,   0,   0,   0,
    113,   0,   0,   0,   0,   0, 114,   0,   0,  25,   0,  37,   0,  33,   0,   0,
      0,  40,   0,  94,  50,  60,   0,   0,  70,   0,   0,   0,   0,  27,  27,  27,
      0,   0,   0,  29,   0,   0,  23,   0,   0,   0,  39,  50,   0,   0,  40,   0,
     37,   0,   0,   0,   0,   0,  35,   0,   0,   0,  39,   0,   0,   0,  85,   0,
      0,   0,  29,   0,   0,   0,  25,   0,   0,  94,   0,   0,   0,   0,  33,   0,
     33,   0,   0,   0,   0,   0,   2,   0,  35,  37,  39,   2,  11,  15,  19,   3,
      6,   9,   0,   0,   0,   0,   0,  27,   0,   0,   0,  40,   0,  33,   0,  33,
      0,  40,   0,   0,   0,   0,   0,  23,  40,  50,  60,  70,  80,  81,  82,  83,
     84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,  11,  15,  23,  31,
     80,  89,  98, 107,  31,  40,  80,  85,  89,  94,  98,  31,  40,  80,  85,  89,
     94, 103, 107,  40,  23,  23,  23,  25,  25,  25,  25,  31,  40,  40,  40,  40,
     40,  60,  80,  80,  80,  80,  85,  87,  89,  89,  89,  89,  80,  15,  15,  18,
     19,   0,   0,   0,  23,  31,  40,  80,   0,  84,   0,   0,   0,   0,  93,   0,
      0,  23,  25,  40,  50,  85,   0,   0,  23,  25,  27,  40,  50,  85,  94, 103,
      0,   0,  23,  40,  50,  85,  25,  27,  40,  50,  85,  94,   0,  23,  80,   0,
     39,  40,  50,  60,  70,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,
     91,  92,  93,  15,  11,  12,  18,   0,  50,  60,  70,  80,  81,  82,  83,  84,
     85,  94,   2,  23,  35,  37,  39,  29,  39,  23,  25,  27,  37,  39,  23,  25,
     27,  29,  31,  25,  27,  27,  29,  31,  23,  25,  27,  27,  29,  31,   0,   0,
     29,  31,  27,  27,  29,  29,  29,  29,  33,  35,  35,  35,  37,  37,  39,  39,
     39,  39,  25,  27,  29,  31,  33,  23,  25,  27,  29,  29,  31,  31,   0,   0,
     23,  25,  12,  18,  21,  12,  18,   6,  11,   8,  11,   0,  83,  84,   0,   0,
     37,  39,   2,  23,   2,   2,  23,  25,  35,  37,  39,   0,  29,   0,   0,   0,
      0,   0,   0,  60,   0,  29,   0,   0,  39,   0,   0,   0,
};

/* Numeric_Value: 2860 bytes. */

RE_UINT32 re_get_numeric_value(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_numeric_value_stage_1[f] << 4;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_numeric_value_stage_2[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_numeric_value_stage_3[pos + f] << 3;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_numeric_value_stage_4[pos + f] << 2;
    value = re_numeric_value_stage_5[pos + code];

    return value;
}

/* Bidi_Mirrored. */

static RE_UINT8 re_bidi_mirrored_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_bidi_mirrored_stage_2[] = {
    0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
};

static RE_UINT8 re_bidi_mirrored_stage_3[] = {
     0,  1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  3,  1,  1,  1,  1,
     4,  5,  1,  6,  7,  8,  1,  9, 10,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 11,
     1,  1,  1, 12,  1,  1,  1,  1,
};

static RE_UINT8 re_bidi_mirrored_stage_4[] = {
     0,  1,  2,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  4,  3,  3,  3,  3,  3,  5,  3,  3,  3,  3,  3,
     6,  7,  8,  3,  3,  9,  3,  3, 10, 11, 12, 13, 14,  3,  3,  3,
     3,  3,  3,  3,  3, 15,  3, 16,  3,  3,  3,  3,  3,  3, 17, 18,
    19, 20, 21, 22,  3,  3,  3,  3, 23,  3,  3,  3,  3,  3,  3,  3,
    24,  3,  3,  3,  3,  3,  3,  3,  3, 25,  3,  3, 26, 27,  3,  3,
     3,  3,  3, 28, 29, 30, 31, 32,
};

static RE_UINT8 re_bidi_mirrored_stage_5[] = {
      0,   0,   0,   0,   0,   3,   0,  80,   0,   0,   0,  40,   0,   0,   0,  40,
      0,   0,   0,   0,   0,   8,   0,   8,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  60,   0,   0,   0,  24,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   6,  96,   0,   0,   0,   0,   0,   0,  96,
      0,  96,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,
     30,  63,  98, 188,  87, 248,  15, 250, 255,  31,  60, 128, 245, 207, 255, 255,
    255, 159,   7,   1, 204, 255, 255, 193,   0,  62, 195, 255, 255,  63, 255, 255,
      0,  15,   0,   0,   3,   6,   0,   0,   0,   0,   0,   0,   0, 255,  63,   0,
    121,  19, 120, 112, 252, 255,   0,   0, 248, 255, 255, 249, 255, 255,   0,   1,
     63, 194,  55,  31,  58,   3, 240,  51,   0, 252, 255, 223,  83, 122,  48, 112,
      0,   0, 128,   1,  48, 188,  25, 254, 255, 255, 255, 255, 207, 191, 255, 255,
    255, 255, 127,  80, 124, 112, 136,  47,  60,  54,   0,  48, 255,   3,   0,   0,
      0, 255, 243,  15,   0,   0,   0,   0,   0,   0,   0, 126,  48,   0,   0,   0,
      0,   3,   0,  80,   0,   0,   0,  40,   0,   0,   0, 168,  13,   0,   0,   0,
      0,   0,   0,   8,   0,   0,   0,   0,   0,   0,  32,   0,   0,   0,   0,   0,
      0, 128,   0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,
      8,   0,   0,   0,   0,   0,   0,   0,
};

/* Bidi_Mirrored: 489 bytes. */

RE_UINT32 re_get_bidi_mirrored(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_bidi_mirrored_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_bidi_mirrored_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_bidi_mirrored_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_bidi_mirrored_stage_4[pos + f] << 6;
    pos += code;
    value = (re_bidi_mirrored_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Indic_Matra_Category. */

static RE_UINT8 re_indic_matra_category_stage_1[] = {
    0, 1, 1, 1, 1, 2, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_indic_matra_category_stage_2[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,
     8,  0,  0,  0,  0,  0,  0,  9,  0, 10, 11, 12, 13,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 14, 15, 16, 17,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,
    19,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_indic_matra_category_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  1,  2,  3,  4,  0,  0,  0,  0,  5,  6,  7,  4,  0,
     0,  0,  0,  5,  8,  0,  0,  0,  0,  0,  0,  5,  9,  0,  4,  0,
     0,  0,  0, 10, 11, 12,  4,  0,  0,  0,  0, 13, 14,  7,  0,  0,
     0,  0,  0, 15, 16, 17,  4,  0,  0,  0,  0, 10, 18, 19,  4,  0,
     0,  0,  0, 13, 20,  7,  4,  0,  0,  0,  0,  0, 21, 22,  0, 23,
     0,  0,  0, 24, 25,  0,  0,  0,  0,  0,  0, 26, 27,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 28, 29,  0,  0,  0,  0,  0,  0,  0,
     0,  0, 30, 31,  0, 32, 33, 34, 35, 36,  0,  0,  0,  0,  0,  0,
     0, 37,  0, 37,  0, 38,  0, 38,  0,  0,  0, 39, 40, 41,  0,  0,
     0,  0, 42,  0,  0,  0,  0,  0,  0,  0,  0, 43, 44,  0,  0,  0,
     0, 45,  0,  0,  0,  0, 46, 47,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0, 48, 49,  0,  0,  0,  0,  0, 50,  0,  0,  0,  0, 23,
     0,  0, 51,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    52,  0, 53,  0,  0,  0,  0,  0,  0,  0,  0, 54, 55,  0,  0,  0,
     0,  0,  0,  0, 56, 57,  0,  0,  0,  0,  0, 58, 59,  0,  0,  0,
     0,  0, 60, 61,  0,  0,  0,  0,  0,  0,  0, 62,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 63,  0,
    64,  0,  0, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0, 66, 67,  0,  0,  0,  0,  0,  0, 68,  0,  0,  0,  0,
};

static RE_UINT8 re_indic_matra_category_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  2,
     3,  4,  5,  6,  1,  7,  3,  8,  0,  0,  9,  4,  0,  0,  0,  0,
     0,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,
     3,  4, 10, 11, 12, 13, 14,  0,  0,  0,  0, 15,  0,  0,  0,  0,
     3, 10,  0,  9, 16,  9, 17,  0,  3,  4,  5,  9, 18, 15,  3,  0,
     0,  0,  0,  0,  0,  0,  0, 19,  3,  4, 10, 11, 20, 13, 21,  0,
     0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,
    17, 10,  0, 22, 12, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  6,
     1,  7, 25,  6, 26,  6,  6,  0,  0,  0,  9, 10,  0,  0,  0,  0,
    27,  7, 25, 18, 28, 29,  6,  0,  0,  0, 15, 25,  0,  0,  0,  0,
     7,  3, 10, 22, 12, 23, 24,  0,  0,  0,  0,  0,  0, 16,  0, 15,
     7,  6, 10, 10,  2, 30, 23, 31,  0,  7,  0,  0,  0,  0,  0,  0,
    19,  7,  6,  6,  4, 10,  0,  0, 32, 32, 33,  9,  0,  0,  0, 16,
    19,  7,  6,  6,  4,  9,  0,  0, 32, 32, 34,  0,  0,  0,  0,  0,
    35, 36,  4, 37, 37,  6,  6,  0, 36,  0, 10,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 15, 19, 17, 38,  6,  6,  0, 39, 16,  0,  0,
     0,  0,  0,  7,  4,  0,  0,  0,  0, 25,  0, 15, 25,  0,  0,  0,
     9,  6, 16,  0,  0,  0,  0,  0,  0, 15, 40, 16,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 19,  0,  0, 17, 10,  0,  0,  0,  0,  0,
     0, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19,  6, 17,  4, 41,
    42, 22, 23,  0, 25,  0,  0,  0,  9, 43,  0,  0,  0,  0,  0,  0,
     6, 44, 45, 46, 16,  0,  0,  0,  7,  7,  2, 22,  7,  8,  7,  7,
    25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9, 38,  2,  0,  0,
    47,  1, 19,  6, 17,  5, 44, 22, 22, 40, 16,  0,  0,  0,  0,  0,
     0,  0, 15,  6,  4, 48, 49, 22, 23, 18, 25,  0,  0,  0,  0,  0,
     0,  0, 17,  8,  6, 25,  0,  0,  0,  0,  0,  2, 50,  7, 10,  0,
     0,  0,  0, 16,  0,  0,  0,  0,  0, 15,  3,  1,  0,  0,  0,  0,
     0,  0, 15,  7,  7,  7,  7,  7,  7,  7, 10,  0,  0,  0,  0,  0,
     0,  0,  0, 35,  4, 17,  4, 10,  0, 15,  0,  0,  0,  0,  0,  0,
     0,  0,  7,  6,  4, 22, 16,  0, 51,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  9,  6, 17, 52, 40, 10,  0,  0,  0,  0,  0,  0,
     1,  6, 53, 54, 55, 56, 33, 16,  0, 15, 19,  7, 44, 25, 35,  0,
    57,  4,  9, 58,  0,  0, 10,  0,  0,  0,  0,  0,  0,  0,  0, 39,
     0,  0,  0,  0,  6,  6,  4,  4,  4,  6,  6, 16,  0,  0,  0,  0,
     2,  3,  5,  1,  3,  0,  0,  0,
};

static RE_UINT8 re_indic_matra_category_stage_5[] = {
     0,  0,  5,  1,  1,  2,  1,  6,  6,  6,  6,  5,  5,  5,  1,  1,
     2,  1,  0,  5,  6,  0,  0,  2,  2,  0,  0,  4,  4,  6,  0,  1,
     5,  0,  5,  6,  5,  8,  1,  5,  9,  0, 10,  6,  2,  2,  4,  4,
     4,  5,  1,  0,  7,  0,  8,  1,  8,  0,  8,  8,  9,  2,  4,  1,
     3,  3,  3,  1,  3,  0,  0,  6,  5,  7,  7,  7,  6,  2,  0, 14,
     2,  5,  9, 10,  4,  2, 14,  0,  6,  1,  1,  8,  8,  5, 14,  1,
     6, 11,  7, 12,  2,  9, 11,  0,  5,  2,  6,  3,  3,  5,  5,  3,
     1,  3,  0, 13, 13,  0,
};

/* Indic_Matra_Category: 1254 bytes. */

RE_UINT32 re_get_indic_matra_category(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_indic_matra_category_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_indic_matra_category_stage_2[pos + f] << 4;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_indic_matra_category_stage_3[pos + f] << 3;
    f = code >> 1;
    code ^= f << 1;
    pos = (RE_UINT32)re_indic_matra_category_stage_4[pos + f] << 1;
    value = re_indic_matra_category_stage_5[pos + code];

    return value;
}

/* Indic_Syllabic_Category. */

static RE_UINT8 re_indic_syllabic_category_stage_1[] = {
    0, 1, 2, 2, 2, 3, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_indic_syllabic_category_stage_2[] = {
     0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  3,  4,  5,  6,  7,  8,
     9,  1,  1,  1,  1,  1,  1, 10,  1, 11, 12, 13, 14,  1,  1,  1,
     1,  1,  1,  1,  1, 15,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 16, 17, 18, 19,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 20,  1,  1,  1,  1,  1,
    21,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_indic_syllabic_category_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,   8,  16,
     17,  11,  12,  18,  19,  20,   0,  21,  22,  23,  12,  24,  25,   0,   8,   0,
     10,  11,  12,  24,  26,  27,   8,  28,  29,  30,  31,  32,  33,  34,   0,   0,
     35,  36,  12,  37,  38,  39,   8,   0,  40,  36,  12,  41,  38,  42,   8,   0,
     40,  36,   4,  43,  44,  34,   8,  45,  46,  47,   4,  48,  49,  50,   0,  51,
     52,   4,  53,  54,  55,   0,   0,   0,  56,  57,  58,  59,  60,  61,   0,   0,
      0,   0,   0,   0,  62,   4,  63,  64,  65,  66,  67,  68,   0,   0,   0,   0,
      4,   4,  69,  70,   0,  71,  72,  73,  74,  75,   0,   0,   0,   0,   0,   0,
     76,  77,  78,  77,  78,  79,  76,  80,   4,   4,  81,  82,  83,  84,   0,   0,
     85,  63,  86,  87,   0,   4,  88,  89,   4,   4,  90,  91,  92,   0,   0,   0,
      4,  93,   4,   4,  94,  95,  96,  97,   0,   0,   0,   0,   0,   0,   0,   0,
     98,  78,   4,  99, 100,   0,   0,   0, 101,   4, 102,   0,   4,   4, 103, 104,
      4,   4, 105, 106, 107,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 108,   0,   0,   0,
    109,   4, 110,   0,   4, 111, 112, 113, 114, 115,   4, 116, 117,   0,   0,   0,
    118,   4, 119,   4, 120, 121,   0,   0, 122,   4,   4, 123, 124,   0,   0,   0,
    125,   4, 126, 127, 128,   0,   4, 129,   4,   4,   4, 130, 131,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4, 132, 133,   0,
    134, 135,   4, 136,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    137,  78,   4, 138, 139,   0,   0,   0, 140,   4,   4, 141,   0,   0,   0,   0,
};

static RE_UINT8 re_indic_syllabic_category_stage_4[] = {
      0,   0,   0,   0,   1,   0,   0,   0,   2,   3,   3,   3,   3,   4,   5,   5,
      5,   5,   5,   5,   5,   5,   6,   7,   8,   8,   8,   9,   0,  10,   5,   5,
     11,   0,   0,   0,  12,   3,  13,   5,  14,  15,   3,  16,  16,   4,   5,   5,
      5,   5,  17,   5,  18,  19,  20,   7,   8,  21,  21,  22,   0,  23,   0,  24,
     20,   0,   0,   0,  14,  15,  25,  26,  17,  27,  20,  28,  29,  23,  21,  30,
      0,   0,  13,  18,  31,  32,   0,   0,  14,  15,   3,  33,  33,   4,   5,   5,
     17,  13,  20,   7,   8,  34,  34,  30,   8,  21,  21,  30,   0,  35,   0,  24,
     36,   0,   0,   0,  37,  15,  25,  12,  38,  39,  27,  17,  40,  41,  42,  19,
      5,   5,  20,  35,  29,  35,  43,  30,   0,  23,   0,   0,  14,  15,   3,  38,
     38,   4,   5,   5,   5,  13,  20,  44,   8,  43,  43,  30,   0,  45,  20,   0,
     46,  15,   3,  38,   5,  13,  20,   7,   0,  45,   0,  47,   5,   5,  42,  44,
      8,  43,  43,  48,   0,   0,  49,  50,  46,  15,   3,   3,   3,  25,  19,   5,
     24,   5,   5,  36,   5,  42,  51,  23,   8,  52,   8,   8,  35,   0,   0,   0,
     13,   5,   5,   5,   5,   5,   5,  42,   8,   8,  53,   0,   8,  34,  54,  55,
     27,  56,  18,  36,   0,   5,  13,   5,  13,  57,  19,  27,   8,   8,  34,  58,
      8,  59,  54,  60,   0,   0,   0,  20,   5,   5,  13,   5,   5,   5,   5,  41,
     10,   8,   8,  61,  62,  63,  64,  65,  66,  66,  67,  66,  66,  66,  66,  66,
     66,  66,  66,  68,  69,   3,  70,   8,   8,  71,  72,  73,  74,  11,  75,  76,
     77,  78,  79,  80,  81,  82,   5,   5,  83,  84,  54,  85,   0,   0,  86,  87,
     88,   5,   5,  17,   6,  89,   0,   0,  88,   5,   5,   5,   6,   0,   0,   0,
     90,   0,   0,   0,  91,   3,   3,   3,   3,  35,   8,   8,   8,  61,  92,  93,
     94,   0,   0,  95,  96,   5,   5,   5,   8,   8,  97,   0,  98,  99, 100,   0,
    101, 102, 102, 103, 104, 105,   0,   0,   5,   5,   5,   0,   8,   8,   8,   8,
    106,  99, 107,   0,   5, 108,   8,   0,   5,   5,   5,  69,  88, 109,  99, 110,
    111,   8,   8,   8,   8,  79, 107,   0, 112, 113,   3,   3,   5, 114,   8,   8,
      8, 115,   5,   0, 116,   3, 117,   5, 118,   8,  53,  19,   5, 119,   8,   8,
    120,   0,   0,   0,   5, 121,   8, 106,  99, 122,   0,   0,   0,   0,   0,  13,
      0,   0,   0,   1,  33, 123, 124,   5, 108,   8,   0,   0,   5,   5,   5, 125,
    126, 127, 128,   5, 129,   0,   0,   0, 130,   3,   3,   3, 117,   5,   5,   5,
      5, 131,   8,   8,   8,  89,   0,   0,   0,   0,  19,   5, 125, 102, 132, 107,
      5, 108,   8, 133, 134,   0,   0,   0, 135,   3,   4,  88, 136,   8,   8, 137,
     89,   0,   0,   0,   3, 117,   5,   5,   5,   5,  81,   8, 138, 139,   0,   0,
     99,  99,  99, 140,  13,   0, 141,   0,   8,   8,   8,  84, 142,   0,   0,   0,
      5,   5, 143,  99, 144,   8,  29, 145,  81,  45,   0, 146,   5,  13,  13,   5,
      5,   0,   0, 147, 148,  15,   3,   3,   5,   5,   8,   8,   8,  53,   0,   0,
    149,   3,   3,   4,   8,   8, 150,   0,
};

static RE_UINT8 re_indic_syllabic_category_stage_5[] = {
     0,  0,  0,  0,  9,  0,  0,  0,  1,  1,  1,  2,  6,  6,  6,  6,
     6, 10, 10, 10, 10, 10, 10, 10, 10, 10,  7,  7,  4,  3,  7,  7,
     7,  7,  7,  7,  7,  5,  7,  7,  0,  7,  7,  7,  6,  6,  7,  7,
     0,  0,  6,  6,  0, 10, 10, 10,  0,  1,  1,  2,  0,  6,  6,  6,
     6,  0,  0,  6, 10,  0, 10, 10, 10,  0, 10,  0,  0,  0, 10, 10,
    10, 10,  0,  0,  7,  0,  0,  7,  7,  5, 11,  0,  0,  0,  0,  7,
    10, 10,  0, 10,  6,  6,  6,  0,  0,  0,  0,  6,  0, 10, 10,  0,
     4,  0,  7,  7,  7,  7,  7,  0,  7,  5,  0,  0,  1,  0,  9,  9,
     0, 14,  0,  0,  6,  6,  0,  6,  7,  7,  0,  7,  0,  0,  7,  7,
     0, 10,  0,  0,  0,  0,  1, 17,  6,  0,  6,  6,  6, 10,  0,  0,
     0,  0,  0, 10, 10,  0,  0,  0, 10, 10, 10,  0,  7,  0,  7,  7,
     0,  3,  7,  7,  0,  7,  7,  0,  0,  0,  1,  2,  0,  0, 10,  0,
     7,  5, 12,  0,  0,  0, 11, 11, 11, 11, 11, 11,  0,  0,  5,  0,
     7,  0,  7,  0,  7,  7,  5,  0, 19, 19, 19, 19,  0,  1,  5,  0,
    10,  0,  0, 10,  0, 10,  0, 10, 14, 14,  0,  0,  7,  0,  0,  0,
     0,  1,  0,  0,  7,  7,  1,  2,  7,  7,  1,  1,  5,  3,  0,  0,
    16, 16, 16, 16, 16, 13, 13, 13, 13, 13, 13, 13,  0, 13, 13, 13,
    13,  0,  0,  0, 10,  6,  6,  6,  6,  6,  6,  7,  7,  7,  1, 19,
     2,  5,  5, 14, 14, 14, 14, 10, 10, 10,  6,  6,  7,  7, 10, 10,
    10, 10, 14, 14, 14, 10,  7, 19, 19, 10, 10,  7,  7, 19, 19, 19,
    19, 19, 10, 10, 10,  7,  7,  7,  7, 10, 10, 10, 10, 10, 14,  7,
     7,  7,  7, 19, 19, 19, 10, 19,  0,  0, 19, 19,  7,  7,  0,  0,
     6,  6,  6, 10,  5,  0,  0,  0, 10,  0,  7,  7, 10, 10, 10,  6,
     7, 20, 20,  0, 12,  0,  0,  0,  0,  5,  5,  0,  3,  0,  0,  0,
     9, 10, 10, 10,  7, 13, 13, 13, 15, 15,  1, 15, 15, 15, 15, 15,
    15,  0,  0,  0, 10, 10, 10,  8,  8,  8,  8,  8,  8,  8,  0,  0,
    18, 18, 18, 18, 18,  0,  0,  0,  7, 15, 15, 15, 19, 19,  0,  0,
    10, 10, 10,  7, 10, 14, 14, 15, 15, 15, 15,  0,  5,  7,  7,  7,
     1,  1,  1, 12,  2,  6,  6,  6,  4,  7,  7,  7,  5, 10, 10, 10,
     1, 12,  2,  6,  6,  6, 10, 10, 10, 13, 13, 13,  6,  6,  4,  7,
    15, 15,  5,  5, 13, 13,  7,  7,  1,  1,  0,  4,  6,  6,  5, 10,
    10, 10, 10,  1, 10, 10,  8,  8,  8,  8, 10, 10, 10, 10,  8, 13,
    13, 10, 10, 10, 10, 13, 10,  1,  1,  2,  6,  6, 15,  7,  7,  7,
     8,  8,  8, 19,  7,  7,  7, 15, 15, 15, 15,  5,  1,  1, 12,  2,
    10, 10, 10,  4,  7, 13, 14, 14,  7,  7,  7, 14, 14, 14, 14,  0,
    15, 15,  0,  0,  0,  0, 10, 19, 18, 19, 18,  0, 10, 10, 10, 15,
    15, 15, 15,  7, 19,  5,  0,  0,  7,  0,  1,  2,  0,  0,  0,  5,
     1,  1,  2,  0,  1,  1,  2,  6,  7,  5,  4,  0,
};

/* Indic_Syllabic_Category: 1820 bytes. */

RE_UINT32 re_get_indic_syllabic_category(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_indic_syllabic_category_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_indic_syllabic_category_stage_2[pos + f] << 4;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_indic_syllabic_category_stage_3[pos + f] << 2;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_indic_syllabic_category_stage_4[pos + f] << 2;
    value = re_indic_syllabic_category_stage_5[pos + code];

    return value;
}

/* Alphanumeric. */

RE_UINT32 re_get_alphanumeric(RE_UINT32 ch) {
    RE_UINT32 v;

    v = re_get_alphabetic(ch);
    if (v == 1)
        return 1;

    v = re_get_general_category(ch);
    if (v == RE_PROP_ND)
        return 1;

    return 0;
}

/* Any. */

RE_UINT32 re_get_any(RE_UINT32 ch) {
    return 1;
}

/* ASCII. */

RE_UINT32 re_get_ascii(RE_UINT32 ch) {
    if (ch <= RE_ASCII_MAX)
        return 1;

    return 0;
}

/* Assigned. */

RE_UINT32 re_get_assigned(RE_UINT32 ch) {
    if (re_get_general_category(ch) != RE_PROP_CN)
        return 1;

    return 0;
}

/* Blank. */

RE_UINT32 re_get_blank(RE_UINT32 ch) {
    RE_UINT32 v;

    if (0x0A <= ch && ch <= 0x0D || ch == 0x85)
        return 0;

    v = re_get_white_space(ch);
    if (v == 0)
        return 0;

    v = re_get_general_category(ch);
    if ((RE_BLANK_MASK & (1 << v)) != 0)
        return 0;

    return 1;
}

/* Graph. */

RE_UINT32 re_get_graph(RE_UINT32 ch) {
    RE_UINT32 v;

    v = re_get_white_space(ch);
    if (v == 1)
        return 0;

    v = re_get_general_category(ch);
    if ((RE_GRAPH_MASK & (1 << v)) != 0)
        return 0;

    return 1;
}

/* Print. */

RE_UINT32 re_get_print(RE_UINT32 ch) {
    RE_UINT32 v;

    v = re_get_general_category(ch);
    if (v == RE_PROP_CC)
        return 0;

    v = re_get_graph(ch);
    if (v == 1)
        return 1;

    v = re_get_blank(ch);
    if (v == 1)
        return 1;

    return 0;
}

/* Word. */

RE_UINT32 re_get_word(RE_UINT32 ch) {
    RE_UINT32 v;

    v = re_get_alphabetic(ch);
    if (v == 1)
        return 1;

    v = re_get_general_category(ch);
    if ((RE_WORD_MASK & (1 << v)) != 0)
        return 1;

    return 0;
}

/* XDigit. */

RE_UINT32 re_get_xdigit(RE_UINT32 ch) {
    RE_UINT32 v;

    v = re_get_general_category(ch);
    if (v == RE_PROP_ND)
        return 1;

    v = re_get_hex_digit(ch);
    if (v == 1)
        return 1;

    return 0;
}

/* All_Cases. */

static RE_UINT8 re_all_cases_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_all_cases_stage_2[] = {
     0,  1,  2,  3,  3,  3,  3,  3,  4,  3,  3,  3,  3,  3,  5,  6,
     7,  3,  8,  3,  3,  3,  9,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3, 10,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 11,  3, 12,
     3,  3, 13,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
};

static RE_UINT8 re_all_cases_stage_3[] = {
     0,  0,  1,  2,  0,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13,
     6, 14, 15, 16, 17,  0,  0,  0,  0,  0, 18, 19, 20, 21, 22, 23,
    24, 25, 26,  6, 27,  6, 28,  6,  6, 29, 30, 31, 32,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 33, 34,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,
     6,  6,  6, 36, 37,  6,  6,  6, 38, 39, 40, 41, 39, 42, 43, 44,
     0,  0,  0,  0,  0,  0,  0,  0,  0, 45, 46, 47, 48,  0,  0,  0,
     0,  0,  0,  0,  0, 49, 50, 51,  0,  0,  0,  0,  0,  0,  0,  0,
    52, 53, 54, 55,  6,  6,  6, 56, 57, 58,  0,  0,  0,  0,  0,  0,
     0,  0,  6, 59, 60,  0,  0,  0,  0, 61,  6, 62, 63, 64,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 65,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0, 66, 67,  0,  0,  0,  0,  0,
    68, 69, 70,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_all_cases_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   3,   2,   4,   2,   5,   0,
      6,   7,   8,   7,   9,   7,  10,   0,   0,   0,   0,   0,   0,  11,   0,   0,
      2,  12,   2,   2,   2,   5,   2,  13,   7,  14,   7,   7,   7,  10,   7,  15,
     16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  17,  16,  18,  19,
     19,  19,  20,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  21,  22,
     23,  24,  25,  26,  27,  28,  29,  30,  16,  31,  32,  31,  33,  34,  35,  36,
      0,  37,  38,  39,  19,  19,  19,  40,  16,  16,  16,  16,  41,  42,  16,  16,
     43,  16,  16,  16,  16,   0,  44,  45,  46,  47,  16,  16,  48,  49,  50,   0,
     51,  52,  53,  54,  55,  56,   0,  57,  58,   0,  59,  60,  61,   0,   0,   0,
      0,  62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,  63,  64,  65,
      0,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,
     81,  82,  83,  84,  85,  86,  16,  16,  16,  16,  16,  16,  87,  88,  89,  90,
     91,  91,  91,  91,   2,   2,   2,   2,   2,   2,   2,   2,   7,   7,   7,   7,
      7,   7,   7,   7,  92,  92,  92,  92,  35,   0,  63,  16,  16,  16,  16,  16,
     93,  19,  19,  94,  16,  16,  16,  16,  16,  16,   0,   0,  95,  96,  96,  96,
     96,  96,  96,  96,  96,  97,   0,   0,  98,  99,  99,  99,  99,  99,  99,  99,
     99, 100,   0,   0,   0,   0,   0,   0, 101, 101, 101, 101, 101, 101, 101, 101,
    101, 102,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 103, 104,
    105,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  35, 106, 107,
    108, 108, 109, 109, 108, 110, 109, 111, 108, 108, 109, 109, 108, 108, 109, 109,
    108, 110, 109, 111, 112, 112, 113, 113, 108, 108, 109, 109, 114, 115, 116, 117,
    108, 108, 109, 109, 118,   0, 119, 120, 121,   0, 122, 123, 124,   0, 125,   0,
    126, 127, 128, 129, 121,   0, 130, 123,   0, 131, 132,   0, 133,   0,   0,   0,
      0,   0,   0, 134,   0,   0,   0,   0, 135, 135, 135, 135, 136, 136, 136, 136,
    137, 138,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 139, 140, 140,
    140, 140, 140, 140, 141, 141, 141, 141, 141, 141, 142,   0,   0,   0,   0,   0,
     96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  97,  99,  99,  99,  99,
     99,  99,  99,  99,  99,  99,  99, 100, 143, 144,  19, 145, 146, 147,   0, 148,
     16,   0, 137, 149,   0,   0,   0,   0, 150, 150, 150, 150, 150, 150, 150, 150,
    150, 151,   0,   0,   0,   0,   0,   0,  16,  16,  16,  35,   0,   0,   0,   0,
     16,  16,  16,  16,  16,  16,   0,   0,  63,  16,  16,  16,  63,  16,  16,  16,
     16,  16,  16,  16,   0,   0,  18, 152,  16,  16, 137, 153,  35,   0,   0,   0,
     16,  16,  35,   0,   0,   0,   0,   0,   0, 147,   0,   0,   0,   0,   0,   0,
      1,   2,   2,   2,   2,   2,   5,   0,   6,   7,   7,   7,   7,   7,  10,   0,
    154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 155, 155, 155, 155, 155, 155,
    155, 155, 155, 155,   0,   0,   0,   0,
};

static RE_UINT8 re_all_cases_stage_5[] = {
      0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   2,   1,   3,
      1,   1,   1,   4,   1,   1,   1,   0,   0,   5,   5,   5,   5,   5,   5,   5,
      5,   6,   5,   7,   5,   5,   5,   8,   5,   5,   5,   0,   0,   9,   0,   0,
      1,  10,   1,   1,   1,   1,   1,  11,   5,  12,   5,   5,   5,   5,   5,  13,
     14,  15,  14,  15,  16,  17,  14,  15,   0,  14,  15,  14,  15,  14,  15,  14,
     15,   0,  14,  15,  18,  14,  15,  14,  15,  14,  15,  19,  20,  21,  14,  15,
     14,  15,  22,  14,  15,  23,  23,  14,  15,   0,  24,  25,  26,  14,  15,  23,
     27,  28,  29,  30,  14,  15,  31,   0,  29,  32,  33,  34,  14,  15,  35,  14,
     15,  35,   0,   0,  15,  36,  36,  14,  15,  14,  15,  37,  14,  15,   0,   0,
     14,  15,   0,  38,  39,  40,  41,  39,  40,  41,  39,  40,  41,  14,  15,  14,
     15,  42,  14,  15,   0,  39,  40,  41,  14,  15,  43,  44,  45,   0,  14,  15,
      0,   0,  46,  14,  15,  47,  48,  49,  49,  14,  15,  50,  51,  52,  14,  15,
     53,  54,  55,  56,  57,   0,  58,  58,   0,  59,   0,  60,  58,   0,   0,  61,
      0,  62,   0,   0,  63,  64,   0,  65,   0,   0,   0,  64,   0,  66,  67,   0,
      0,  68,   0,   0,   0,  69,   0,   0,  70,   0,   0,  70,  70,  71,  72,  72,
     73,   0,   0,   0,   0,   0,  74,   0,   0,  75,   0,   0,   0,   0,  14,  15,
      0,   0,   0,  33,  33,  33,   0,   0,   0,   0,  76,   0,  77,  77,  77,   0,
     78,   0,  79,  79,  80,   1,  81,   1,   1,  82,   1,   1,  83,  84,  85,   1,
     86,   1,   1,   1,  87,  88,   0,  89,   1,   1,  90,   1,   1,  91,   1,   1,
     92,  93,  93,  93,  94,   5,  95,   5,   5,  96,   5,   5,  97,  98,  99,   5,
    100,   5,   5,   5, 101, 102, 103, 104,   5,   5, 105,   5,   5, 106,   5,   5,
    107, 108, 108, 109, 110, 111,   0,   0,   0, 112, 113, 114, 115, 116, 117,   0,
    118, 119,   0,  14,  15, 120,  14,  15,   0,  45,  45,  45, 121, 121, 121, 121,
    122, 122, 122, 122, 123,  14,  15,  14,  15,  14,  15, 124,   0, 125, 125, 125,
    125, 125, 125, 125, 125, 125, 125,   0,   0, 126, 126, 126, 126, 126, 126, 126,
    126, 126, 126,   0, 127, 127, 127, 127, 127, 127,   0,   0,   0, 128,   0,   0,
      0, 129,   0,   0, 130, 131,  14,  15,   0,   0,   0, 132,   0,   0, 133,   0,
    109, 109, 109, 109, 114, 114, 114, 114, 109, 109,   0,   0, 114, 114,   0,   0,
      0, 109,   0, 109,   0, 114,   0, 114, 134, 134, 135, 135, 135, 135, 136, 136,
    137, 137, 138, 138, 139, 139,   0,   0, 109, 109,   0, 140, 114, 114, 141, 141,
    142,   0, 143,   0,   0,   0,   0, 140, 144, 144, 144, 144, 142,   0,   0,   0,
    109, 109,   0, 145, 114, 114, 146, 146, 109, 109,   0, 147,   0, 117,   0,   0,
    114, 114, 148, 148, 120,   0,   0,   0, 149, 149, 150, 150,   0,   0, 151,   0,
      0,   0, 152, 153,   0,   0, 154,   0,   0,   0, 155,   0, 156, 156, 156, 156,
    157, 157, 157, 157,   0,   0,   0,  14,  15,   0,   0,   0,   0,   0, 158, 158,
    158, 158, 158, 158, 159, 159, 159, 159, 159, 159,   0,   0,  14,  15, 160, 161,
    162, 163, 164,  14,  15, 165, 166, 167, 168,   0,  14,  15,   0,  14,  15,   0,
      0,   0, 169, 169,  15,  14,  15,   0, 170, 170, 170, 170, 170, 170,   0,   0,
     15, 171,  14,  15,  15, 172,   0,   0, 173, 173, 173, 173, 174, 174, 174, 174,
};

/* All_Cases: 1644 bytes. */

static RE_AllCases re_all_cases_table[] = {
    {     0,     0,     0},
    {    32,     0,     0},
    {    32,   232,     0},
    {    32,  8415,     0},
    {    32,   300,     0},
    {   -32,     0,     0},
    {   -32,   199,     0},
    {   -32,  8383,     0},
    {   -32,   268,     0},
    {   743,   775,     0},
    {    32,  8294,     0},
    {  7615,     0,     0},
    {   -32,  8262,     0},
    {   121,     0,     0},
    {     1,     0,     0},
    {    -1,     0,     0},
    {  -199,     0,     0},
    {  -232,     0,     0},
    {  -121,     0,     0},
    {  -300,  -268,     0},
    {   195,     0,     0},
    {   210,     0,     0},
    {   206,     0,     0},
    {   205,     0,     0},
    {    79,     0,     0},
    {   202,     0,     0},
    {   203,     0,     0},
    {   207,     0,     0},
    {    97,     0,     0},
    {   211,     0,     0},
    {   209,     0,     0},
    {   163,     0,     0},
    {   213,     0,     0},
    {   130,     0,     0},
    {   214,     0,     0},
    {   218,     0,     0},
    {   217,     0,     0},
    {   219,     0,     0},
    {    56,     0,     0},
    {     1,     2,     0},
    {    -1,     1,     0},
    {    -2,    -1,     0},
    {   -79,     0,     0},
    {   -97,     0,     0},
    {   -56,     0,     0},
    {  -130,     0,     0},
    { 10795,     0,     0},
    {  -163,     0,     0},
    { 10792,     0,     0},
    { 10815,     0,     0},
    {  -195,     0,     0},
    {    69,     0,     0},
    {    71,     0,     0},
    { 10783,     0,     0},
    { 10780,     0,     0},
    { 10782,     0,     0},
    {  -210,     0,     0},
    {  -206,     0,     0},
    {  -205,     0,     0},
    {  -202,     0,     0},
    {  -203,     0,     0},
    {  -207,     0,     0},
    { 42280,     0,     0},
    {  -209,     0,     0},
    {  -211,     0,     0},
    { 10743,     0,     0},
    { 10749,     0,     0},
    {  -213,     0,     0},
    {  -214,     0,     0},
    { 10727,     0,     0},
    {  -218,     0,     0},
    {   -69,     0,     0},
    {  -217,     0,     0},
    {   -71,     0,     0},
    {  -219,     0,     0},
    {    84,   116,  7289},
    {    38,     0,     0},
    {    37,     0,     0},
    {    64,     0,     0},
    {    63,     0,     0},
    {  7235,     0,     0},
    {    32,    62,     0},
    {    32,    96,     0},
    {    32,    57,    92},
    {   -84,    32,  7205},
    {    32,    86,     0},
    {  -743,    32,     0},
    {    32,    54,     0},
    {    32,    80,     0},
    {    31,    32,     0},
    {    32,    47,     0},
    {    32,  7549,     0},
    {   -38,     0,     0},
    {   -37,     0,     0},
    {  7219,     0,     0},
    {   -32,    30,     0},
    {   -32,    64,     0},
    {   -32,    25,    60},
    {  -116,   -32,  7173},
    {   -32,    54,     0},
    {  -775,   -32,     0},
    {   -32,    22,     0},
    {   -32,    48,     0},
    {   -31,     1,     0},
    {   -32,    -1,     0},
    {   -32,    15,     0},
    {   -32,  7517,     0},
    {   -64,     0,     0},
    {   -63,     0,     0},
    {     8,     0,     0},
    {   -62,   -30,     0},
    {   -57,   -25,    35},
    {   -47,   -15,     0},
    {   -54,   -22,     0},
    {    -8,     0,     0},
    {   -86,   -54,     0},
    {   -80,   -48,     0},
    {     7,     0,     0},
    {   -92,   -60,   -35},
    {   -96,   -64,     0},
    {    -7,     0,     0},
    {    80,     0,     0},
    {   -80,     0,     0},
    {    15,     0,     0},
    {   -15,     0,     0},
    {    48,     0,     0},
    {   -48,     0,     0},
    {  7264,     0,     0},
    { 35332,     0,     0},
    {  3814,     0,     0},
    {     1,    59,     0},
    {    -1,    58,     0},
    {   -59,   -58,     0},
    { -7615,     0,     0},
    {    74,     0,     0},
    {    86,     0,     0},
    {   100,     0,     0},
    {   128,     0,     0},
    {   112,     0,     0},
    {   126,     0,     0},
    {     9,     0,     0},
    {   -74,     0,     0},
    {    -9,     0,     0},
    { -7289, -7205, -7173},
    {   -86,     0,     0},
    { -7235,     0,     0},
    {  -100,     0,     0},
    { -7219,     0,     0},
    {  -112,     0,     0},
    {  -128,     0,     0},
    {  -126,     0,     0},
    { -7549, -7517,     0},
    { -8415, -8383,     0},
    { -8294, -8262,     0},
    {    28,     0,     0},
    {   -28,     0,     0},
    {    16,     0,     0},
    {   -16,     0,     0},
    {    26,     0,     0},
    {   -26,     0,     0},
    {-10743,     0,     0},
    { -3814,     0,     0},
    {-10727,     0,     0},
    {-10795,     0,     0},
    {-10792,     0,     0},
    {-10780,     0,     0},
    {-10749,     0,     0},
    {-10783,     0,     0},
    {-10782,     0,     0},
    {-10815,     0,     0},
    { -7264,     0,     0},
    {-35332,     0,     0},
    {-42280,     0,     0},
    {    40,     0,     0},
    {   -40,     0,     0},
};

/* All_Cases: 1050 bytes. */

int re_get_all_cases(RE_UINT32 ch, RE_UINT32* codepoints) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;
    RE_AllCases* all_cases;
    int count;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_all_cases_stage_1[f] << 5;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_all_cases_stage_2[pos + f] << 4;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_all_cases_stage_3[pos + f] << 3;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_all_cases_stage_4[pos + f] << 2;
    value = re_all_cases_stage_5[pos + code];

    all_cases = &re_all_cases_table[value];

    codepoints[0] = ch;
    count = 1;

    while (count < RE_MAX_CASES && all_cases->diffs[count - 1] != 0) {
        codepoints[count] = ch + all_cases->diffs[count - 1];
        ++count;
    }

    return count;
}

/* Simple_Case_Folding. */

static RE_UINT8 re_simple_case_folding_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_simple_case_folding_stage_2[] = {
     0,  1,  2,  3,  3,  3,  3,  3,  4,  3,  3,  3,  3,  3,  3,  5,
     6,  3,  7,  3,  3,  3,  8,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  9,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 10,
     3,  3, 11,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
};

static RE_UINT8 re_simple_case_folding_stage_3[] = {
     0,  0,  1,  0,  0,  2,  3,  0,  4,  5,  6,  7,  8,  9, 10, 11,
     4, 12, 13,  0,  0,  0,  0,  0,  0,  0, 14, 15, 16, 17, 18, 19,
    20, 21,  0,  4, 22,  4, 23,  4,  4, 24, 25,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 26, 27,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     4,  4,  4,  4, 28,  4,  4,  4, 29, 30, 31, 32, 30, 33, 34, 35,
     0,  0,  0,  0,  0,  0,  0,  0,  0, 36,  0, 37, 38,  0,  0,  0,
     0,  0,  0,  0,  0, 39, 40,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    41, 42,  0, 43,  4,  4,  4, 44,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  4, 45, 46,  0,  0,  0,  0, 47,  4, 48, 49, 50,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
    51, 52,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_simple_case_folding_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  2,  2,  2,  2,  3,  0,
     0,  0,  0,  0,  0,  4,  0,  0,  2,  2,  2,  2,  2,  3,  2,  3,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  6,  5,  7,  7,
     7,  7,  6,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17,  5, 18, 19, 18, 20, 21, 16, 16,
     0, 22, 23,  7,  7,  7,  7,  6,  5,  5,  5,  5, 24, 25,  5,  5,
    26,  5,  5,  5,  5,  0, 27, 28, 29, 30,  5,  5,  0,  0,  0,  0,
     0, 31,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  6,  0,  0,
     0, 32, 33, 34,  1,  2,  2,  2, 35,  2,  2,  0,  0,  0,  0,  0,
     6,  0,  0, 36, 37, 38,  5,  5,  5,  5,  5,  5, 39, 40, 41, 42,
    43, 43, 43, 43,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,
    16,  0,  6,  5,  5,  5,  5,  5, 44,  7,  7, 45,  5,  5,  5,  5,
     5,  5,  0,  0, 46, 47, 47, 47, 47, 47, 47, 47, 47, 48,  0,  0,
    49, 49, 49, 49, 49, 49, 49, 49, 49, 50,  0,  0,  0,  0,  0,  0,
     5,  5,  5,  5,  5, 16, 51, 52,  0,  0, 53, 53,  0,  0, 53, 54,
     0,  0, 53, 53,  0,  0, 53, 53,  0,  0, 53, 54,  0,  0, 55, 55,
     0,  0, 53, 53,  0,  0,  0,  0,  0,  0, 53, 53,  0,  0, 56, 57,
     0,  0, 58, 59,  0,  0, 60,  0,  0,  0, 61, 62,  0,  0, 63, 59,
     0, 64, 65,  0, 66,  0,  0,  0, 67, 67, 67, 67,  0,  0,  0,  0,
    68,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 69, 70, 70,
    70, 70, 70, 70,  0,  0,  0,  0, 47, 47, 47, 47, 47, 47, 47, 47,
    47, 47, 47, 48,  0,  0,  0,  0, 71, 72,  7, 73, 74, 45,  0, 75,
     5,  0, 68, 45,  0,  0,  0,  0,  5,  5,  5, 16,  0,  0,  0,  0,
     5,  5,  5,  5,  5,  5,  0,  0,  6,  5,  5,  5,  6,  5,  5,  5,
     5,  5,  5,  5,  0,  0,  7, 76,  5,  5, 68, 77, 16,  0,  0,  0,
     5,  5, 16,  0,  0,  0,  0,  0, 78, 78, 78, 78, 78, 78, 78, 78,
    78, 78,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_simple_case_folding_stage_5[] = {
     0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,
     0,  2,  0,  0,  3,  0,  3,  0,  0,  0,  3,  0,  0,  3,  0,  3,
     4,  3,  0,  3,  0,  3,  0,  5,  0,  6,  3,  0,  3,  0,  7,  3,
     0,  8,  8,  3,  0,  0,  9, 10, 11,  3,  0,  8, 12,  0, 13, 14,
     3,  0,  0,  0, 13, 15,  0, 16,  3,  0, 17,  3,  0, 17,  0,  0,
     0, 18, 18,  3,  0,  3,  0, 19, 20,  3,  0, 20,  3,  0, 20,  3,
     0, 20,  3,  0,  3,  0, 21, 22, 23,  0,  3,  0,  0,  0, 24,  3,
     0, 25, 26,  0,  0,  3,  0, 27, 28, 29,  3,  0,  0, 30,  0,  0,
     0,  0, 31,  0, 32, 32, 32,  0, 33,  0, 34, 34,  1,  1,  0,  1,
     0,  0,  0, 35, 36, 37,  0,  0,  0, 38, 39,  0, 40, 41,  0,  0,
    42, 43,  0,  3,  0, 44,  3,  0,  0, 23, 23, 23, 45, 45, 45, 45,
    46,  3,  0,  3,  0,  3,  0,  0,  0, 47, 47, 47, 47, 47, 47, 47,
    47, 47, 47,  0, 48, 48, 48, 48, 48, 48,  0,  0,  0,  0,  0, 49,
     0,  0, 50,  0, 51, 51, 51, 51, 51, 51,  0,  0,  0, 51,  0, 51,
    51, 51, 52, 52, 53,  0, 54,  0, 55, 55, 55, 55, 53,  0,  0,  0,
    51, 51, 56, 56, 51, 51, 57, 57, 44,  0,  0,  0, 58, 58, 59, 59,
     0,  0, 60,  0,  0,  0, 61, 62,  0,  0, 63,  0, 64, 64, 64, 64,
     0,  0,  0,  3,  0,  0, 65, 65, 65, 65, 65, 65,  3,  0, 66, 67,
    68,  0,  0,  3,  0, 69, 70, 71, 72,  0,  3,  0,  0,  0, 73, 73,
     0, 74,  3,  0,  0, 75,  0,  0, 76, 76, 76, 76,
};

/* Simple_Case_Folding: 1160 bytes. */

static RE_INT32 re_simple_case_folding_table[] = {
         0,
        32,
       775,
         1,
      -121,
      -268,
       210,
       206,
       205,
        79,
       202,
       203,
       207,
       211,
       209,
       213,
       214,
       218,
       217,
       219,
         2,
       -97,
       -56,
      -130,
     10795,
      -163,
     10792,
      -195,
        69,
        71,
       116,
        38,
        37,
        64,
        63,
         8,
       -30,
       -25,
       -15,
       -22,
       -54,
       -48,
       -60,
       -64,
        -7,
        80,
        15,
        48,
      7264,
       -58,
     -7615,
        -8,
       -74,
        -9,
     -7173,
       -86,
      -100,
      -112,
      -128,
      -126,
     -7517,
     -8383,
     -8262,
        28,
        16,
        26,
    -10743,
     -3814,
    -10727,
    -10780,
    -10749,
    -10783,
    -10782,
    -10815,
    -35332,
    -42280,
        40,
};

/* Simple_Case_Folding: 308 bytes. */

RE_UINT32 re_get_simple_case_folding(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;
    RE_INT32 diff;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_simple_case_folding_stage_1[f] << 5;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_simple_case_folding_stage_2[pos + f] << 4;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_simple_case_folding_stage_3[pos + f] << 3;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_simple_case_folding_stage_4[pos + f] << 2;
    value = re_simple_case_folding_stage_5[pos + code];

    diff = re_simple_case_folding_table[value];

    return ch + diff;
}

/* Full_Case_Folding. */

static RE_UINT8 re_full_case_folding_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_full_case_folding_stage_2[] = {
     0,  1,  2,  3,  3,  3,  3,  3,  4,  3,  3,  3,  3,  3,  3,  5,
     6,  3,  7,  3,  3,  3,  8,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  9,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 10,  3, 11,
     3,  3, 12,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
};

static RE_UINT8 re_full_case_folding_stage_3[] = {
     0,  0,  1,  0,  0,  2,  3,  0,  4,  5,  6,  7,  8,  9, 10, 11,
     4, 12, 13,  0,  0,  0,  0,  0,  0,  0, 14, 15, 16, 17, 18, 19,
    20, 21,  0,  4, 22,  4, 23,  4,  4, 24, 25,  0, 26,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 27, 28,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     4,  4,  4,  4, 29,  4,  4,  4, 30, 31, 32, 33, 34, 35, 36, 37,
     0,  0,  0,  0,  0,  0,  0,  0,  0, 38,  0, 39, 40,  0,  0,  0,
     0,  0,  0,  0,  0, 41, 42,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    43, 44,  0, 45,  4,  4,  4, 46,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  4, 47, 48,  0,  0,  0,  0, 49,  4, 50, 51, 52,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 53,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
    54, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_full_case_folding_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   2,   2,   2,   2,   3,   0,
      0,   0,   0,   0,   0,   4,   0,   0,   2,   2,   2,   2,   2,   3,   2,   5,
      6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   7,   6,   8,   8,
      8,   8,   9,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,  10,  11,
     12,  13,  14,  15,  16,  17,  18,  19,   6,  20,  21,  20,  22,  23,  18,  18,
      0,  24,  25,   8,   8,   8,   8,  26,   6,   6,   6,   6,  27,  28,   6,   6,
     29,   6,   6,   6,   6,   0,  30,  31,  32,  33,   6,   6,   0,   0,   0,   0,
      0,  34,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6,  26,   0,   0,
      0,  35,  36,  37,  38,   2,   2,   2,  39,   2,   2,   0,  40,   0,   0,   0,
     26,   0,   0,  41,  42,  43,   6,   6,   6,   6,   6,   6,  44,  45,  46,  47,
     48,  48,  48,  48,   2,   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,
     18,   0,  26,   6,   6,   6,   6,   6,  49,   8,   8,  50,   6,   6,   6,   6,
      6,   6,   0,   0,  51,  52,  52,  52,  52,  52,  52,  52,  52,  53,   0,   0,
      0,  54,   0,   0,   0,   0,   0,   0,  55,  55,  55,  55,  55,  55,  55,  55,
     55,  56,   0,   0,   0,   0,   0,   0,   6,   6,   6,   6,   6,  57,  58,  59,
      0,   0,  60,  60,   0,   0,  60,  61,   0,   0,  60,  60,   0,   0,  60,  60,
      0,   0,  60,  61,  62,  63,  64,  64,   0,   0,  60,  60,   0,   0,   0,   0,
     65,  65,  66,  66,  67,  67,  68,  68,  69,  69,  70,  70,  71,  72,  73,  74,
     75,  76,  77,  78,  79,  80,  81,   0,  82,  83,  84,  85,  86,  87,  88,  89,
      0,  90,  91,   0,  92,   0,   0,   0,  93,  93,  93,  93,   0,   0,   0,   0,
     94,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  95,  96,  96,
     96,  96,  96,  96,   0,   0,   0,   0,  52,  52,  52,  52,  52,  52,  52,  52,
     52,  52,  52,  53,   0,   0,   0,   0,  97,  98,   8,  99, 100,  50,   0, 101,
      6,   0,  94,  50,   0,   0,   0,   0,   6,   6,   6,  18,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   0,   0,  26,   6,   6,   6,  26,   6,   6,   6,
      6,   6,   6,   6,   0,   0,   8, 102,   6,   6,  94, 103,  18,   0,   0,   0,
      6,   6,  18,   0,   0,   0,   0,   0, 104, 105,   0,   0, 106, 107,   0,   0,
    108, 108, 108, 108, 108, 108, 108, 108, 108, 108,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_full_case_folding_stage_5[] = {
      0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   0,
      0,   2,   0,   0,   1,   1,   1,   3,   4,   0,   4,   0,   5,   0,   4,   0,
      0,   4,   0,   4,   0,   6,   4,   0,   7,   4,   0,   4,   0,   4,   0,   8,
      0,   9,   4,   0,   4,   0,  10,   4,   0,  11,  11,   4,   0,   0,  12,  13,
     14,   4,   0,  11,  15,   0,  16,  17,   4,   0,   0,   0,  16,  18,   0,  19,
      4,   0,  20,   4,   0,  20,   0,   0,   0,  21,  21,   4,   0,   4,   0,  22,
     23,   4,   0,  23,   4,   0,  23,   4,   0,   0,   4,   0,  24,  23,   4,   0,
      4,   0,  25,  26,  27,   0,   4,   0,   0,   0,  28,   4,   0,  29,  30,   0,
      0,   4,   0,  31,  32,  33,   4,   0,   0,  34,   0,   0,   0,   0,  35,   0,
     36,  36,  36,   0,  37,   0,  38,  38,  39,   1,   1,   1,   1,   1,   0,   1,
     40,   0,   0,   0,   0,   0,   0,  41,  42,  43,   0,   0,   0,  44,  45,   0,
     46,  47,   0,   0,  48,  49,   0,   4,   0,  50,   4,   0,   0,  27,  27,  27,
     51,  51,  51,  51,  52,   4,   0,   4,   0,   4,   0,   0,   0,  53,  53,  53,
     53,  53,  53,  53,  53,  53,  53,   0,   0,   0,   0,  54,  55,  55,  55,  55,
     55,  55,   0,   0,   4,   0,  56,  57,  58,  59,  60,  61,   0,   0,  62,   0,
     63,  63,  63,  63,  63,  63,   0,   0,  64,   0,  65,   0,  66,   0,  67,   0,
      0,  63,   0,  63,  68,  68,  68,  68,  69,  69,  69,  69,  70,  70,  70,  70,
     71,  71,  71,  71,  72,  72,  72,  72,  73,  73,  73,  73,   0,   0,  74,  75,
     76,   0,  77,  78,  63,  63,  79,  79,  80,   0,  81,   0,   0,   0,  82,  83,
     84,   0,  85,  86,  87,  87,  87,  87,  88,   0,   0,   0,   0,   0,  89,  90,
      0,   0,  91,  92,  63,  63,  93,  93,   0,   0,  94,  95,  96,   0,  97,  98,
     63,  63,  99,  99,  50,   0,   0,   0,   0,   0, 100, 101, 102,   0, 103, 104,
    105, 105, 106, 106, 107,   0,   0,   0,   0,   0, 108,   0,   0,   0, 109, 110,
      0,   0, 111,   0, 112, 112, 112, 112,   0,   0,   0,   4,   0,   0, 113, 113,
    113, 113, 113, 113,   4,   0, 114, 115, 116,   0,   0,   4,   0, 117, 118, 119,
    120,   0,   4,   0,   0,   0, 121, 121,   0, 122,   4,   0,   0, 123,   0,   0,
    124, 125, 126, 127, 128, 129, 130,   0,   0,   0,   0, 131, 132, 133, 134, 135,
    136, 136, 136, 136,
};

/* Full_Case_Folding: 1320 bytes. */

static RE_FullCaseFolding re_full_case_folding_table[] = {
    {     0,    0,   0},
    {    32,    0,   0},
    {   775,    0,   0},
    {  -108,  115,   0},
    {     1,    0,   0},
    {  -199,  775,   0},
    {   371,  110,   0},
    {  -121,    0,   0},
    {  -268,    0,   0},
    {   210,    0,   0},
    {   206,    0,   0},
    {   205,    0,   0},
    {    79,    0,   0},
    {   202,    0,   0},
    {   203,    0,   0},
    {   207,    0,   0},
    {   211,    0,   0},
    {   209,    0,   0},
    {   213,    0,   0},
    {   214,    0,   0},
    {   218,    0,   0},
    {   217,    0,   0},
    {   219,    0,   0},
    {     2,    0,   0},
    {  -390,  780,   0},
    {   -97,    0,   0},
    {   -56,    0,   0},
    {  -130,    0,   0},
    { 10795,    0,   0},
    {  -163,    0,   0},
    { 10792,    0,   0},
    {  -195,    0,   0},
    {    69,    0,   0},
    {    71,    0,   0},
    {   116,    0,   0},
    {    38,    0,   0},
    {    37,    0,   0},
    {    64,    0,   0},
    {    63,    0,   0},
    {    41,  776, 769},
    {    21,  776, 769},
    {     8,    0,   0},
    {   -30,    0,   0},
    {   -25,    0,   0},
    {   -15,    0,   0},
    {   -22,    0,   0},
    {   -54,    0,   0},
    {   -48,    0,   0},
    {   -60,    0,   0},
    {   -64,    0,   0},
    {    -7,    0,   0},
    {    80,    0,   0},
    {    15,    0,   0},
    {    48,    0,   0},
    {   -34, 1410,   0},
    {  7264,    0,   0},
    { -7726,  817,   0},
    { -7715,  776,   0},
    { -7713,  778,   0},
    { -7712,  778,   0},
    { -7737,  702,   0},
    {   -58,    0,   0},
    { -7723,  115,   0},
    {    -8,    0,   0},
    { -7051,  787,   0},
    { -7053,  787, 768},
    { -7055,  787, 769},
    { -7057,  787, 834},
    {  -128,  953,   0},
    {  -136,  953,   0},
    {  -112,  953,   0},
    {  -120,  953,   0},
    {   -64,  953,   0},
    {   -72,  953,   0},
    {   -66,  953,   0},
    { -7170,  953,   0},
    { -7176,  953,   0},
    { -7173,  834,   0},
    { -7174,  834, 953},
    {   -74,    0,   0},
    { -7179,  953,   0},
    { -7173,    0,   0},
    {   -78,  953,   0},
    { -7180,  953,   0},
    { -7190,  953,   0},
    { -7183,  834,   0},
    { -7184,  834, 953},
    {   -86,    0,   0},
    { -7189,  953,   0},
    { -7193,  776, 768},
    { -7194,  776, 769},
    { -7197,  834,   0},
    { -7198,  776, 834},
    {  -100,    0,   0},
    { -7197,  776, 768},
    { -7198,  776, 769},
    { -7203,  787,   0},
    { -7201,  834,   0},
    { -7202,  776, 834},
    {  -112,    0,   0},
    {  -118,  953,   0},
    { -7210,  953,   0},
    { -7206,  953,   0},
    { -7213,  834,   0},
    { -7214,  834, 953},
    {  -128,    0,   0},
    {  -126,    0,   0},
    { -7219,  953,   0},
    { -7517,    0,   0},
    { -8383,    0,   0},
    { -8262,    0,   0},
    {    28,    0,   0},
    {    16,    0,   0},
    {    26,    0,   0},
    {-10743,    0,   0},
    { -3814,    0,   0},
    {-10727,    0,   0},
    {-10780,    0,   0},
    {-10749,    0,   0},
    {-10783,    0,   0},
    {-10782,    0,   0},
    {-10815,    0,   0},
    {-35332,    0,   0},
    {-42280,    0,   0},
    {-64154,  102,   0},
    {-64155,  105,   0},
    {-64156,  108,   0},
    {-64157,  102, 105},
    {-64158,  102, 108},
    {-64146,  116,   0},
    {-64147,  116,   0},
    {-62879, 1398,   0},
    {-62880, 1381,   0},
    {-62881, 1387,   0},
    {-62872, 1398,   0},
    {-62883, 1389,   0},
    {    40,    0,   0},
};

/* Full_Case_Folding: 1096 bytes. */

int re_get_full_case_folding(RE_UINT32 ch, RE_UINT32* codepoints) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;
    RE_FullCaseFolding* case_folding;
    int count;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_full_case_folding_stage_1[f] << 5;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_full_case_folding_stage_2[pos + f] << 4;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_full_case_folding_stage_3[pos + f] << 3;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_full_case_folding_stage_4[pos + f] << 2;
    value = re_full_case_folding_stage_5[pos + code];

    case_folding = &re_full_case_folding_table[value];

    codepoints[0] = ch + case_folding->diff;
    count = 1;

    while (count < RE_MAX_FOLDED && case_folding->codepoints[count - 1] != 0) {
        codepoints[count] = case_folding->codepoints[count - 1];
        ++count;
    }

    return count;
}

/* Property function table. */

RE_GetPropertyFunc re_get_property[] = {
    re_get_general_category,
    re_get_block,
    re_get_script,
    re_get_word_break,
    re_get_grapheme_cluster_break,
    re_get_sentence_break,
    re_get_math,
    re_get_alphabetic,
    re_get_lowercase,
    re_get_uppercase,
    re_get_cased,
    re_get_case_ignorable,
    re_get_changes_when_lowercased,
    re_get_changes_when_uppercased,
    re_get_changes_when_titlecased,
    re_get_changes_when_casefolded,
    re_get_changes_when_casemapped,
    re_get_id_start,
    re_get_id_continue,
    re_get_xid_start,
    re_get_xid_continue,
    re_get_default_ignorable_code_point,
    re_get_grapheme_extend,
    re_get_grapheme_base,
    re_get_grapheme_link,
    re_get_white_space,
    re_get_bidi_control,
    re_get_join_control,
    re_get_dash,
    re_get_hyphen,
    re_get_quotation_mark,
    re_get_terminal_punctuation,
    re_get_other_math,
    re_get_hex_digit,
    re_get_ascii_hex_digit,
    re_get_other_alphabetic,
    re_get_ideographic,
    re_get_diacritic,
    re_get_extender,
    re_get_other_lowercase,
    re_get_other_uppercase,
    re_get_noncharacter_code_point,
    re_get_other_grapheme_extend,
    re_get_ids_binary_operator,
    re_get_ids_trinary_operator,
    re_get_radical,
    re_get_unified_ideograph,
    re_get_other_default_ignorable_code_point,
    re_get_deprecated,
    re_get_soft_dotted,
    re_get_logical_order_exception,
    re_get_other_id_start,
    re_get_other_id_continue,
    re_get_sterm,
    re_get_variation_selector,
    re_get_pattern_white_space,
    re_get_pattern_syntax,
    re_get_hangul_syllable_type,
    re_get_bidi_class,
    re_get_canonical_combining_class,
    re_get_decomposition_type,
    re_get_east_asian_width,
    re_get_joining_group,
    re_get_joining_type,
    re_get_line_break,
    re_get_numeric_type,
    re_get_numeric_value,
    re_get_bidi_mirrored,
    re_get_indic_matra_category,
    re_get_indic_syllabic_category,
    re_get_alphanumeric,
    re_get_any,
    re_get_ascii,
    re_get_assigned,
    re_get_blank,
    re_get_graph,
    re_get_print,
    re_get_word,
    re_get_xdigit,
};
