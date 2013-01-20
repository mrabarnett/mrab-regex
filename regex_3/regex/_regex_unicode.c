#include "_regex_unicode.h"

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
    "-1",
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
    "216000",
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
    "432000",
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
    "ALCHEMICAL",
    "ALCHEMICALSYMBOLS",
    "ALEF",
    "ALETTER",
    "ALNUM",
    "ALPHA",
    "ALPHABETIC",
    "ALPHABETICPF",
    "ALPHABETICPRESENTATIONFORMS",
    "ALPHANUMERIC",
    "AMBIGUOUS",
    "AN",
    "ANCIENTGREEKMUSIC",
    "ANCIENTGREEKMUSICALNOTATION",
    "ANCIENTGREEKNUMBERS",
    "ANCIENTSYMBOLS",
    "ANY",
    "AR",
    "ARAB",
    "ARABIC",
    "ARABICEXTA",
    "ARABICEXTENDEDA",
    "ARABICLETTER",
    "ARABICMATH",
    "ARABICMATHEMATICALALPHABETICSYMBOLS",
    "ARABICNUMBER",
    "ARABICPFA",
    "ARABICPFB",
    "ARABICPRESENTATIONFORMSA",
    "ARABICPRESENTATIONFORMSB",
    "ARABICSUP",
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
    "ATTACHEDBELOWLEFT",
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
    "BAMUMSUP",
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
    "BOPOMOFOEXT",
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
    "BYZANTINEMUSIC",
    "BYZANTINEMUSICALSYMBOLS",
    "C",
    "C&",
    "CAKM",
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
    "CASEDLETTER",
    "CASEIGNORABLE",
    "CB",
    "CC",
    "CCC",
    "CCC10",
    "CCC103",
    "CCC107",
    "CCC11",
    "CCC118",
    "CCC12",
    "CCC122",
    "CCC129",
    "CCC13",
    "CCC130",
    "CCC132",
    "CCC133",
    "CCC14",
    "CCC15",
    "CCC16",
    "CCC17",
    "CCC18",
    "CCC19",
    "CCC20",
    "CCC21",
    "CCC22",
    "CCC23",
    "CCC24",
    "CCC25",
    "CCC26",
    "CCC27",
    "CCC28",
    "CCC29",
    "CCC30",
    "CCC31",
    "CCC32",
    "CCC33",
    "CCC34",
    "CCC35",
    "CCC36",
    "CCC84",
    "CCC91",
    "CF",
    "CHAKMA",
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
    "CJ",
    "CJK",
    "CJKCOMPAT",
    "CJKCOMPATFORMS",
    "CJKCOMPATIBILITY",
    "CJKCOMPATIBILITYFORMS",
    "CJKCOMPATIBILITYIDEOGRAPHS",
    "CJKCOMPATIBILITYIDEOGRAPHSSUPPLEMENT",
    "CJKCOMPATIDEOGRAPHS",
    "CJKCOMPATIDEOGRAPHSSUP",
    "CJKEXTA",
    "CJKEXTB",
    "CJKEXTC",
    "CJKEXTD",
    "CJKRADICALSSUP",
    "CJKRADICALSSUPPLEMENT",
    "CJKSTROKES",
    "CJKSYMBOLS",
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
    "COMPATJAMO",
    "COMPLEXCONTEXT",
    "CONDITIONALJAPANESESTARTER",
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
    "COUNTINGROD",
    "COUNTINGRODNUMERALS",
    "CP",
    "CPRT",
    "CR",
    "CS",
    "CUNEIFORM",
    "CUNEIFORMNUMBERS",
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
    "CYRILLICEXTA",
    "CYRILLICEXTB",
    "CYRILLICEXTENDEDA",
    "CYRILLICEXTENDEDB",
    "CYRILLICSUP",
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
    "DEVANAGARIEXT",
    "DEVANAGARIEXTENDED",
    "DI",
    "DIA",
    "DIACRITIC",
    "DIACRITICALS",
    "DIACRITICALSFORSYMBOLS",
    "DIACRITICALSSUP",
    "DIGIT",
    "DINGBATS",
    "DOMINO",
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
    "ENCLOSEDALPHANUM",
    "ENCLOSEDALPHANUMERICS",
    "ENCLOSEDALPHANUMERICSUPPLEMENT",
    "ENCLOSEDALPHANUMSUP",
    "ENCLOSEDCJK",
    "ENCLOSEDCJKLETTERSANDMONTHS",
    "ENCLOSEDIDEOGRAPHICSUP",
    "ENCLOSEDIDEOGRAPHICSUPPLEMENT",
    "ENCLOSINGMARK",
    "ES",
    "ET",
    "ETHI",
    "ETHIOPIC",
    "ETHIOPICEXT",
    "ETHIOPICEXTA",
    "ETHIOPICEXTENDED",
    "ETHIOPICEXTENDEDA",
    "ETHIOPICSUP",
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
    "GEORGIANSUP",
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
    "GREEKEXT",
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
    "HALFANDFULLFORMS",
    "HALFMARKS",
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
    "HEBREWLETTER",
    "HEH",
    "HEHGOAL",
    "HETH",
    "HEX",
    "HEXDIGIT",
    "HIGHPRIVATEUSESURROGATES",
    "HIGHPUSURROGATES",
    "HIGHSURROGATES",
    "HIRA",
    "HIRAGANA",
    "HL",
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
    "INDICNUMBERFORMS",
    "INDICSYLLABICCATEGORY",
    "INFIXNUMERIC",
    "INHERITED",
    "INIT",
    "INITIAL",
    "INITIALPUNCTUATION",
    "INMC",
    "INSC",
    "INSCRIPTIONALPAHLAVI",
    "INSCRIPTIONALPARTHIAN",
    "INSEPARABLE",
    "INSEPERABLE",
    "INVISIBLE",
    "IOTASUBSCRIPT",
    "IPAEXT",
    "IPAEXTENSIONS",
    "IS",
    "ISO",
    "ISOLATED",
    "ITAL",
    "JAMO",
    "JAMOEXTA",
    "JAMOEXTB",
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
    "KANASUP",
    "KANASUPPLEMENT",
    "KANAVOICING",
    "KANBUN",
    "KANGXI",
    "KANGXIRADICALS",
    "KANNADA",
    "KAPH",
    "KATAKANA",
    "KATAKANAEXT",
    "KATAKANAORHIRAGANA",
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
    "LATIN1SUP",
    "LATIN1SUPPLEMENT",
    "LATINEXTA",
    "LATINEXTADDITIONAL",
    "LATINEXTB",
    "LATINEXTC",
    "LATINEXTD",
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
    "LEFTJOINING",
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
    "MAHJONG",
    "MAHJONGTILES",
    "MALAYALAM",
    "MAND",
    "MANDAIC",
    "MANDATORYBREAK",
    "MARK",
    "MATH",
    "MATHALPHANUM",
    "MATHEMATICALALPHANUMERICSYMBOLS",
    "MATHEMATICALOPERATORS",
    "MATHOPERATORS",
    "MATHSYMBOL",
    "MB",
    "MC",
    "ME",
    "MED",
    "MEDIAL",
    "MEEM",
    "MEETEIMAYEK",
    "MEETEIMAYEKEXT",
    "MEETEIMAYEKEXTENSIONS",
    "MERC",
    "MERO",
    "MEROITICCURSIVE",
    "MEROITICHIEROGLYPHS",
    "MIAO",
    "MIDLETTER",
    "MIDNUM",
    "MIDNUMLET",
    "MIM",
    "MISCARROWS",
    "MISCELLANEOUSMATHEMATICALSYMBOLSA",
    "MISCELLANEOUSMATHEMATICALSYMBOLSB",
    "MISCELLANEOUSSYMBOLS",
    "MISCELLANEOUSSYMBOLSANDARROWS",
    "MISCELLANEOUSSYMBOLSANDPICTOGRAPHS",
    "MISCELLANEOUSTECHNICAL",
    "MISCMATHSYMBOLSA",
    "MISCMATHSYMBOLSB",
    "MISCPICTOGRAPHS",
    "MISCSYMBOLS",
    "MISCTECHNICAL",
    "ML",
    "MLYM",
    "MN",
    "MODIFIERLETTER",
    "MODIFIERLETTERS",
    "MODIFIERSYMBOL",
    "MODIFIERTONELETTERS",
    "MODIFYINGLETTER",
    "MONG",
    "MONGOLIAN",
    "MTEI",
    "MUSIC",
    "MUSICALSYMBOLS",
    "MYANMAR",
    "MYANMAREXTA",
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
    "OCR",
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
    "PHAISTOS",
    "PHAISTOSDISC",
    "PHLI",
    "PHNX",
    "PHOENICIAN",
    "PHONETICEXT",
    "PHONETICEXTENSIONS",
    "PHONETICEXTENSIONSSUPPLEMENT",
    "PHONETICEXTSUP",
    "PI",
    "PLAYINGCARDS",
    "PLRD",
    "PO",
    "POPDIRECTIONALFORMAT",
    "POSTFIXNUMERIC",
    "PR",
    "PREFIXNUMERIC",
    "PREPEND",
    "PRINT",
    "PRIVATEUSE",
    "PRIVATEUSEAREA",
    "PRTI",
    "PS",
    "PUA",
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
    "REGIONALINDICATOR",
    "REGISTERSHIFTER",
    "REH",
    "REJANG",
    "REVERSEDPE",
    "RI",
    "RIGHT",
    "RIGHTJOINING",
    "RIGHTTOLEFT",
    "RIGHTTOLEFTEMBEDDING",
    "RIGHTTOLEFTOVERRIDE",
    "RJNG",
    "RLE",
    "RLO",
    "ROHINGYAYEH",
    "RUMI",
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
    "SHARADA",
    "SHAVIAN",
    "SHAW",
    "SHIN",
    "SHRD",
    "SINH",
    "SINHALA",
    "SK",
    "SM",
    "SMALL",
    "SMALLFORMS",
    "SMALLFORMVARIANTS",
    "SML",
    "SO",
    "SOFTDOTTED",
    "SORA",
    "SORASOMPENG",
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
    "SUNDANESESUP",
    "SUNDANESESUPPLEMENT",
    "SUP",
    "SUPARROWSA",
    "SUPARROWSB",
    "SUPER",
    "SUPERANDSUB",
    "SUPERSCRIPTSANDSUBSCRIPTS",
    "SUPMATHOPERATORS",
    "SUPPLEMENTALARROWSA",
    "SUPPLEMENTALARROWSB",
    "SUPPLEMENTALMATHEMATICALOPERATORS",
    "SUPPLEMENTALPUNCTUATION",
    "SUPPLEMENTARYPRIVATEUSEAREAA",
    "SUPPLEMENTARYPRIVATEUSEAREAB",
    "SUPPUAA",
    "SUPPUAB",
    "SUPPUNCTUATION",
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
    "TAIXUANJING",
    "TAIXUANJINGSYMBOLS",
    "TAKR",
    "TAKRI",
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
    "TRANSPORTANDMAP",
    "TRANSPORTANDMAPSYMBOLS",
    "TRUE",
    "U",
    "UCAS",
    "UCASEXT",
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
    "VEDICEXT",
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
    "VSSUP",
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
    "YIJING",
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

/* strings: 10453 bytes. */

/* properties. */

RE_Property re_properties[] = {
    { 503,  0,  0},
    { 501,  0,  0},
    { 232,  1,  1},
    { 231,  1,  1},
    { 949,  2,  2},
    { 947,  2,  2},
    {1107,  3,  3},
    {1102,  3,  3},
    { 518,  4,  4},
    { 502,  4,  4},
    { 955,  5,  5},
    { 946,  5,  5},
    { 727,  6,  6},
    { 156,  7,  6},
    { 155,  7,  6},
    { 703,  8,  6},
    { 702,  8,  6},
    {1077,  9,  6},
    {1076,  9,  6},
    { 272, 10,  6},
    { 274, 11,  6},
    { 325, 11,  6},
    { 320, 12,  6},
    { 400, 12,  6},
    { 322, 13,  6},
    { 402, 13,  6},
    { 321, 14,  6},
    { 401, 14,  6},
    { 318, 15,  6},
    { 398, 15,  6},
    { 319, 16,  6},
    { 399, 16,  6},
    { 582, 17,  6},
    { 578, 17,  6},
    { 574, 18,  6},
    { 573, 18,  6},
    {1115, 19,  6},
    {1114, 19,  6},
    {1113, 20,  6},
    {1112, 20,  6},
    { 425, 21,  6},
    { 433, 21,  6},
    { 519, 22,  6},
    { 527, 22,  6},
    { 517, 23,  6},
    { 521, 23,  6},
    { 520, 24,  6},
    { 528, 24,  6},
    {1103, 25,  6},
    {1110, 25,  6},
    { 977, 25,  6},
    { 224, 26,  6},
    { 222, 26,  6},
    { 617, 27,  6},
    { 615, 27,  6},
    { 418, 28,  6},
    { 571, 29,  6},
    { 914, 30,  6},
    { 911, 30,  6},
    {1040, 31,  6},
    {1039, 31,  6},
    { 857, 32,  6},
    { 839, 32,  6},
    { 562, 33,  6},
    { 561, 33,  6},
    { 187, 34,  6},
    { 145, 34,  6},
    { 850, 35,  6},
    { 823, 35,  6},
    { 576, 36,  6},
    { 575, 36,  6},
    { 435, 37,  6},
    { 434, 37,  6},
    { 482, 38,  6},
    { 480, 38,  6},
    { 856, 39,  6},
    { 838, 39,  6},
    { 862, 40,  6},
    { 863, 40,  6},
    { 801, 41,  6},
    { 787, 41,  6},
    { 852, 42,  6},
    { 828, 42,  6},
    { 580, 43,  6},
    { 579, 43,  6},
    { 583, 44,  6},
    { 581, 44,  6},
    { 916, 45,  6},
    {1073, 46,  6},
    {1069, 46,  6},
    { 851, 47,  6},
    { 825, 47,  6},
    { 427, 48,  6},
    { 426, 48,  6},
    { 973, 49,  6},
    { 950, 49,  6},
    { 701, 50,  6},
    { 700, 50,  6},
    { 854, 51,  6},
    { 830, 51,  6},
    { 853, 52,  6},
    { 829, 52,  6},
    { 985, 53,  6},
    {1082, 54,  6},
    {1098, 54,  6},
    { 872, 55,  6},
    { 873, 55,  6},
    { 871, 56,  6},
    { 870, 56,  6},
    { 550, 57,  7},
    { 569, 57,  7},
    { 223, 58,  8},
    { 214, 58,  8},
    { 267, 59,  9},
    { 277, 59,  9},
    { 424, 60, 10},
    { 446, 60, 10},
    { 450, 61, 11},
    { 449, 61, 11},
    { 618, 62, 12},
    { 613, 62, 12},
    { 619, 63, 13},
    { 620, 63, 13},
    { 693, 64, 14},
    { 672, 64, 14},
    { 818, 65, 15},
    { 812, 65, 15},
    { 819, 66, 16},
    { 821, 66, 16},
    { 226, 67,  6},
    { 225, 67,  6},
    { 586, 68, 17},
    { 594, 68, 17},
    { 588, 69, 18},
    { 595, 69, 18},
    { 159, 70,  6},
    { 154, 70,  6},
    { 166, 71,  6},
    { 186, 72,  6},
    { 188, 73,  6},
    { 230, 74,  6},
    { 516, 75,  6},
    { 899, 76,  6},
    {1106, 77,  6},
    {1111, 78,  6},
};

/* properties: 580 bytes. */

/* property values. */

RE_PropertyValue re_property_values[] = {
    {1070,  0,   0},
    { 356,  0,   0},
    {1078,  0,   1},
    { 709,  0,   1},
    { 704,  0,   2},
    { 697,  0,   2},
    {1050,  0,   3},
    { 708,  0,   3},
    { 766,  0,   4},
    { 698,  0,   4},
    { 855,  0,   5},
    { 699,  0,   5},
    { 804,  0,   6},
    { 765,  0,   6},
    { 464,  0,   7},
    { 735,  0,   7},
    { 979,  0,   8},
    { 734,  0,   8},
    { 423,  0,   9},
    { 439,  0,   9},
    { 788,  0,   9},
    { 685,  0,  10},
    { 796,  0,  10},
    { 859,  0,  11},
    { 797,  0,  11},
    { 978,  0,  12},
    {1139,  0,  12},
    { 695,  0,  13},
    {1137,  0,  13},
    { 869,  0,  14},
    {1138,  0,  14},
    { 383,  0,  15},
    { 276,  0,  15},
    { 357,  0,  15},
    { 495,  0,  16},
    { 315,  0,  16},
    { 900,  0,  17},
    { 358,  0,  17},
    {1007,  0,  18},
    { 392,  0,  18},
    { 419,  0,  19},
    { 875,  0,  19},
    { 842,  0,  20},
    { 903,  0,  20},
    { 354,  0,  21},
    { 877,  0,  21},
    { 373,  0,  22},
    { 874,  0,  22},
    { 860,  0,  23},
    { 893,  0,  23},
    { 732,  0,  24},
    { 967,  0,  24},
    { 396,  0,  25},
    { 947,  0,  25},
    { 768,  0,  26},
    { 966,  0,  26},
    { 861,  0,  27},
    { 972,  0,  27},
    { 593,  0,  28},
    { 890,  0,  28},
    { 491,  0,  29},
    { 878,  0,  29},
    { 849,  0,  30},
    { 260,  0,  30},
    { 261,  0,  30},
    { 683,  0,  31},
    { 651,  0,  31},
    { 650,  0,  31},
    { 726,  0,  32},
    { 364,  0,  32},
    { 719,  0,  32},
    { 718,  0,  32},
    { 815,  0,  33},
    { 781,  0,  33},
    { 780,  0,  33},
    { 906,  0,  34},
    { 867,  0,  34},
    { 868,  0,  34},
    { 905,  0,  34},
    {1012,  0,  35},
    { 936,  0,  35},
    { 937,  0,  35},
    { 957,  0,  36},
    {1133,  0,  36},
    {1132,  0,  36},
    { 273,  0,  37},
    { 798,  1,   0},
    { 786,  1,   0},
    { 210,  1,   1},
    { 186,  1,   1},
    { 660,  1,   2},
    { 659,  1,   2},
    { 658,  1,   2},
    { 666,  1,   3},
    { 661,  1,   3},
    { 668,  1,   4},
    { 663,  1,   4},
    { 603,  1,   5},
    { 602,  1,   5},
    { 980,  1,   6},
    { 767,  1,   6},
    { 360,  1,   7},
    { 436,  1,   7},
    { 523,  1,   8},
    { 522,  1,   8},
    { 405,  1,   9},
    { 411,  1,  10},
    { 412,  1,  10},
    { 410,  1,  10},
    { 182,  1,  11},
    { 556,  1,  12},
    { 169,  1,  13},
    {1014,  1,  14},
    { 181,  1,  15},
    { 180,  1,  15},
    {1045,  1,  16},
    { 794,  1,  17},
    { 941,  1,  18},
    { 724,  1,  19},
    { 171,  1,  20},
    { 170,  1,  20},
    { 430,  1,  21},
    { 220,  1,  22},
    { 531,  1,  23},
    { 529,  1,  24},
    { 844,  1,  25},
    {1031,  1,  26},
    {1038,  1,  27},
    { 633,  1,  28},
    { 722,  1,  29},
    { 965,  1,  30},
    {1046,  1,  31},
    { 655,  1,  32},
    {1047,  1,  33},
    { 776,  1,  34},
    { 507,  1,  35},
    { 546,  1,  36},
    { 608,  1,  36},
    { 468,  1,  37},
    { 474,  1,  38},
    { 473,  1,  38},
    { 324,  1,  39},
    {1071,  1,  40},
    {1065,  1,  40},
    { 265,  1,  40},
    { 827,  1,  41},
    { 934,  1,  42},
    {1017,  1,  43},
    { 553,  1,  44},
    { 256,  1,  45},
    {1019,  1,  46},
    { 643,  1,  47},
    { 772,  1,  48},
    {1072,  1,  49},
    {1066,  1,  49},
    { 688,  1,  50},
    {1022,  1,  51},
    { 791,  1,  52},
    { 644,  1,  53},
    { 254,  1,  54},
    {1023,  1,  55},
    { 205,  1,  56},
    { 988,  1,  57},
    { 211,  1,  58},
    { 682,  1,  59},
    { 831,  1,  60},
    { 990,  1,  61},
    { 989,  1,  61},
    {1086,  1,  62},
    {1085,  1,  62},
    { 887,  1,  63},
    { 886,  1,  63},
    { 888,  1,  64},
    { 889,  1,  64},
    { 362,  1,  65},
    { 438,  1,  65},
    { 667,  1,  66},
    { 662,  1,  66},
    { 525,  1,  67},
    { 524,  1,  67},
    { 504,  1,  68},
    { 906,  1,  68},
    { 996,  1,  69},
    { 995,  1,  69},
    { 397,  1,  70},
    { 361,  1,  71},
    { 365,  1,  71},
    { 437,  1,  71},
    { 684,  1,  72},
    { 816,  1,  73},
    { 185,  1,  74},
    { 730,  1,  75},
    { 731,  1,  75},
    { 757,  1,  76},
    { 762,  1,  76},
    { 384,  1,  77},
    { 843,  1,  78},
    { 824,  1,  78},
    { 457,  1,  79},
    { 456,  1,  79},
    { 242,  1,  80},
    { 233,  1,  81},
    { 505,  1,  82},
    { 754,  1,  83},
    { 761,  1,  83},
    { 440,  1,  84},
    { 752,  1,  85},
    { 758,  1,  85},
    { 998,  1,  86},
    { 992,  1,  86},
    { 248,  1,  87},
    { 247,  1,  87},
    { 999,  1,  88},
    { 993,  1,  88},
    { 753,  1,  89},
    { 759,  1,  89},
    {1000,  1,  90},
    { 997,  1,  90},
    { 755,  1,  91},
    { 751,  1,  91},
    { 512,  1,  92},
    { 669,  1,  93},
    { 664,  1,  93},
    { 386,  1,  94},
    { 509,  1,  95},
    { 508,  1,  95},
    {1049,  1,  96},
    { 471,  1,  97},
    { 469,  1,  97},
    { 408,  1,  98},
    { 406,  1,  98},
    {1001,  1,  99},
    {1006,  1,  99},
    { 342,  1, 100},
    { 341,  1, 100},
    { 632,  1, 101},
    { 631,  1, 101},
    { 577,  1, 102},
    { 573,  1, 102},
    { 345,  1, 103},
    { 344,  1, 103},
    { 567,  1, 104},
    { 635,  1, 105},
    { 236,  1, 106},
    { 545,  1, 107},
    { 370,  1, 107},
    { 630,  1, 108},
    { 238,  1, 109},
    { 237,  1, 109},
    { 343,  1, 110},
    { 638,  1, 111},
    { 636,  1, 111},
    { 461,  1, 112},
    { 460,  1, 112},
    { 331,  1, 113},
    { 329,  1, 113},
    { 347,  1, 114},
    { 337,  1, 114},
    {1127,  1, 115},
    {1126,  1, 115},
    { 346,  1, 116},
    { 328,  1, 116},
    {1129,  1, 117},
    {1128,  1, 118},
    { 696,  1, 119},
    {1080,  1, 120},
    { 409,  1, 121},
    { 407,  1, 121},
    { 207,  1, 122},
    { 769,  1, 123},
    { 670,  1, 124},
    { 665,  1, 124},
    {1011,  1, 125},
    { 367,  1, 126},
    { 587,  1, 126},
    { 880,  1, 127},
    { 945,  1, 128},
    { 432,  1, 129},
    { 431,  1, 129},
    { 639,  1, 130},
    { 920,  1, 131},
    { 547,  1, 132},
    { 609,  1, 132},
    { 612,  1, 133},
    { 317,  1, 134},
    { 778,  1, 135},
    { 777,  1, 135},
    {1024,  1, 136},
    { 741,  1, 137},
    { 740,  1, 137},
    { 472,  1, 138},
    { 470,  1, 138},
    { 739,  1, 139},
    { 549,  1, 140},
    { 544,  1, 140},
    { 548,  1, 141},
    { 610,  1, 141},
    { 565,  1, 142},
    { 563,  1, 143},
    { 564,  1, 143},
    { 705,  1, 144},
    { 901,  1, 145},
    { 904,  1, 145},
    { 900,  1, 145},
    { 333,  1, 146},
    { 335,  1, 146},
    { 158,  1, 147},
    { 157,  1, 147},
    { 178,  1, 148},
    { 176,  1, 148},
    {1083,  1, 149},
    {1098,  1, 149},
    {1089,  1, 150},
    { 363,  1, 151},
    { 538,  1, 151},
    { 332,  1, 152},
    { 330,  1, 152},
    { 970,  1, 153},
    { 969,  1, 153},
    { 179,  1, 154},
    { 177,  1, 154},
    { 540,  1, 155},
    { 537,  1, 155},
    { 981,  1, 156},
    { 692,  1, 157},
    { 691,  1, 158},
    { 144,  1, 159},
    { 164,  1, 160},
    { 165,  1, 161},
    { 882,  1, 162},
    { 881,  1, 162},
    { 715,  1, 163},
    { 270,  1, 164},
    { 833,  1, 165},
    { 515,  1, 166},
    {1068,  1, 167},
    { 834,  1, 168},
    { 428,  1, 169},
    { 960,  1, 170},
    { 848,  1, 171},
    { 404,  1, 172},
    { 584,  1, 173},
    { 885,  1, 174},
    { 717,  1, 175},
    { 745,  1, 176},
    { 744,  1, 177},
    { 642,  1, 178},
    { 835,  1, 179},
    { 199,  1, 180},
    { 597,  1, 181},
    { 596,  1, 182},
    { 836,  1, 183},
    { 933,  1, 184},
    { 932,  1, 184},
    { 245,  1, 185},
    { 624,  1, 186},
    { 975,  1, 187},
    { 316,  1, 188},
    { 959,  1, 189},
    {1028,  1, 190},
    { 393,  1, 191},
    { 395,  1, 192},
    { 394,  1, 192},
    { 452,  1, 193},
    { 209,  1, 194},
    { 208,  1, 194},
    { 746,  1, 195},
    { 628,  1, 196},
    { 627,  1, 196},
    { 259,  1, 197},
    { 258,  1, 197},
    { 775,  1, 198},
    { 774,  1, 198},
    { 163,  1, 199},
    { 162,  1, 199},
    {1026,  1, 200},
    {1025,  1, 200},
    { 388,  1, 201},
    { 387,  1, 201},
    { 729,  1, 202},
    { 728,  1, 202},
    { 174,  1, 203},
    { 173,  1, 203},
    { 721,  1, 204},
    { 720,  1, 204},
    { 442,  1, 205},
    { 441,  1, 205},
    { 891,  1, 206},
    { 458,  1, 207},
    { 459,  1, 207},
    { 463,  1, 208},
    { 462,  1, 208},
    { 756,  1, 209},
    { 760,  1, 209},
    { 453,  1, 210},
    {1062,  1, 211},
    {1061,  1, 211},
    { 151,  1, 212},
    { 150,  1, 212},
    { 348,  1, 213},
    { 338,  1, 213},
    { 349,  1, 214},
    { 339,  1, 214},
    { 350,  1, 215},
    { 340,  1, 215},
    { 334,  1, 216},
    { 336,  1, 216},
    {1020,  1, 217},
    {1084,  1, 218},
    {1099,  1, 218},
    {1002,  1, 219},
    {1004,  1, 219},
    {1003,  1, 220},
    {1005,  1, 220},
    {1074,  2,   0},
    {1143,  2,   0},
    { 366,  2,   1},
    {1142,  2,   1},
    { 657,  2,   2},
    { 671,  2,   2},
    { 522,  2,   3},
    { 526,  2,   3},
    { 405,  2,   4},
    { 413,  2,   4},
    { 182,  2,   5},
    { 184,  2,   5},
    { 556,  2,   6},
    { 555,  2,   6},
    { 169,  2,   7},
    { 168,  2,   7},
    {1014,  2,   8},
    {1013,  2,   8},
    {1045,  2,   9},
    {1044,  2,   9},
    { 430,  2,  10},
    { 429,  2,  10},
    { 220,  2,  11},
    { 219,  2,  11},
    { 531,  2,  12},
    { 532,  2,  12},
    { 529,  2,  13},
    { 530,  2,  13},
    { 844,  2,  14},
    { 846,  2,  14},
    {1031,  2,  15},
    {1032,  2,  15},
    {1038,  2,  16},
    {1037,  2,  16},
    { 633,  2,  17},
    { 646,  2,  17},
    { 722,  2,  18},
    { 764,  2,  18},
    { 965,  2,  19},
    { 964,  2,  19},
    {1046,  2,  20},
    { 655,  2,  21},
    { 656,  2,  21},
    {1047,  2,  22},
    {1048,  2,  22},
    { 776,  2,  23},
    { 779,  2,  23},
    { 507,  2,  24},
    { 506,  2,  24},
    { 544,  2,  25},
    { 543,  2,  25},
    { 468,  2,  26},
    { 467,  2,  26},
    { 324,  2,  27},
    { 323,  2,  27},
    { 264,  2,  28},
    { 268,  2,  28},
    { 827,  2,  29},
    { 826,  2,  29},
    { 934,  2,  30},
    { 935,  2,  30},
    { 643,  2,  31},
    { 645,  2,  31},
    { 772,  2,  32},
    { 771,  2,  32},
    { 567,  2,  33},
    { 566,  2,  33},
    { 635,  2,  34},
    { 626,  2,  34},
    { 236,  2,  35},
    { 235,  2,  35},
    { 542,  2,  36},
    { 551,  2,  36},
    {1124,  2,  37},
    {1125,  2,  37},
    { 833,  2,  38},
    { 607,  2,  38},
    { 515,  2,  39},
    { 514,  2,  39},
    { 428,  2,  40},
    { 445,  2,  40},
    { 590,  2,  41},
    { 908,  2,  41},
    {1136,  2,  41},
    {1017,  2,  42},
    {1043,  2,  42},
    { 553,  2,  43},
    { 552,  2,  43},
    { 256,  2,  44},
    { 255,  2,  44},
    {1019,  2,  45},
    {1018,  2,  45},
    { 688,  2,  46},
    { 687,  2,  46},
    {1022,  2,  47},
    {1029,  2,  47},
    { 690,  2,  48},
    { 689,  2,  48},
    {1068,  2,  49},
    {1067,  2,  49},
    { 960,  2,  50},
    { 961,  2,  50},
    { 848,  2,  51},
    { 847,  2,  51},
    { 403,  2,  52},
    { 390,  2,  52},
    { 247,  2,  53},
    { 246,  2,  53},
    { 254,  2,  54},
    { 253,  2,  54},
    { 386,  2,  55},
    { 907,  2,  55},
    { 385,  2,  55},
    { 791,  2,  56},
    {1030,  2,  56},
    { 512,  2,  57},
    { 511,  2,  57},
    {1049,  2,  58},
    {1042,  2,  58},
    {1011,  2,  59},
    {1010,  2,  59},
    { 834,  2,  60},
    {1116,  2,  60},
    { 642,  2,  61},
    { 641,  2,  61},
    { 205,  2,  62},
    { 204,  2,  62},
    { 393,  2,  63},
    {1117,  2,  63},
    { 885,  2,  64},
    { 884,  2,  64},
    { 880,  2,  65},
    { 879,  2,  65},
    { 794,  2,  66},
    { 795,  2,  66},
    { 988,  2,  67},
    { 987,  2,  67},
    { 682,  2,  68},
    { 681,  2,  68},
    { 831,  2,  69},
    { 832,  2,  69},
    {1080,  2,  70},
    {1081,  2,  70},
    { 945,  2,  71},
    { 944,  2,  71},
    { 639,  2,  72},
    { 625,  2,  72},
    { 920,  2,  73},
    { 928,  2,  73},
    { 715,  2,  74},
    { 714,  2,  74},
    { 270,  2,  75},
    { 269,  2,  75},
    { 717,  2,  76},
    { 716,  2,  76},
    { 317,  2,  77},
    {1023,  2,  78},
    { 654,  2,  78},
    {1024,  2,  79},
    {1033,  2,  79},
    { 199,  2,  80},
    { 200,  2,  80},
    { 452,  2,  81},
    { 451,  2,  81},
    { 941,  2,  82},
    { 942,  2,  82},
    { 696,  2,  83},
    { 207,  2,  84},
    { 206,  2,  84},
    { 612,  2,  85},
    { 611,  2,  85},
    { 739,  2,  86},
    { 773,  2,  86},
    { 584,  2,  87},
    { 183,  2,  87},
    { 835,  2,  88},
    { 943,  2,  88},
    { 597,  2,  89},
    { 902,  2,  89},
    { 596,  2,  90},
    { 883,  2,  90},
    { 836,  2,  91},
    { 845,  2,  91},
    { 624,  2,  92},
    { 648,  2,  92},
    { 211,  2,  93},
    { 212,  2,  93},
    { 245,  2,  94},
    { 244,  2,  94},
    { 724,  2,  95},
    { 723,  2,  95},
    { 316,  2,  96},
    { 262,  2,  96},
    { 744,  2,  97},
    { 742,  2,  97},
    { 745,  2,  98},
    { 743,  2,  98},
    { 746,  2,  99},
    { 892,  2,  99},
    { 959,  2, 100},
    { 963,  2, 100},
    { 975,  2, 101},
    { 974,  2, 101},
    {1028,  2, 102},
    {1027,  2, 102},
    { 637,  2, 103},
    { 849,  3,   0},
    {1118,  3,   0},
    { 391,  3,   1},
    { 686,  3,   2},
    { 790,  3,   3},
    { 796,  3,   3},
    { 481,  3,   4},
    { 917,  3,   5},
    { 922,  3,   5},
    { 495,  3,   6},
    { 493,  3,   6},
    { 635,  3,   7},
    { 622,  3,   7},
    { 153,  3,   8},
    { 673,  3,   8},
    { 747,  3,   9},
    { 763,  3,   9},
    { 748,  3,  10},
    { 765,  3,  10},
    { 749,  3,  11},
    { 733,  3,  11},
    { 817,  3,  12},
    { 813,  3,  12},
    { 483,  3,  13},
    { 478,  3,  13},
    { 849,  4,   0},
    {1118,  4,   0},
    { 391,  4,   1},
    { 686,  4,   2},
    { 383,  4,   3},
    { 356,  4,   3},
    { 481,  4,   4},
    { 478,  4,   4},
    { 917,  4,   5},
    { 922,  4,   5},
    { 979,  4,   6},
    { 967,  4,   6},
    { 650,  4,   7},
    {1079,  4,   8},
    {1016,  4,   9},
    { 710,  4,  10},
    { 712,  4,  11},
    { 898,  4,  12},
    { 849,  5,   0},
    {1118,  5,   0},
    { 391,  5,   1},
    { 686,  5,   2},
    { 481,  5,   3},
    { 478,  5,   3},
    { 956,  5,   4},
    { 951,  5,   4},
    { 495,  5,   5},
    { 493,  5,   5},
    { 976,  5,   6},
    { 702,  5,   7},
    { 699,  5,   7},
    {1076,  5,   8},
    {1075,  5,   8},
    { 837,  5,   9},
    { 673,  5,   9},
    { 817,  5,  10},
    { 813,  5,  10},
    { 193,  5,  11},
    { 189,  5,  11},
    { 985,  5,  12},
    { 984,  5,  12},
    { 352,  5,  13},
    { 351,  5,  13},
    { 948,  5,  14},
    { 947,  5,  14},
    { 797,  6,   0},
    { 485,  6,   0},
    { 484,  6,   0},
    { 780,  6,   0},
    {1123,  6,   1},
    {1016,  6,   1},
    {1119,  6,   1},
    {1063,  6,   1},
    { 807,  7,   0},
    { 782,  7,   0},
    { 674,  7,   1},
    { 650,  7,   1},
    {1096,  7,   2},
    {1079,  7,   2},
    {1059,  7,   3},
    {1016,  7,   3},
    { 711,  7,   4},
    { 710,  7,   4},
    { 713,  7,   5},
    { 712,  7,   5},
    { 678,  8,   0},
    { 650,  8,   0},
    { 925,  8,   1},
    { 915,  8,   1},
    { 475,  8,   2},
    { 454,  8,   2},
    { 476,  8,   3},
    { 465,  8,   3},
    { 477,  8,   4},
    { 466,  8,   4},
    { 175,  8,   5},
    { 161,  8,   5},
    { 368,  8,   6},
    { 392,  8,   6},
    { 869,  8,   7},
    { 201,  8,   7},
    { 953,  8,   8},
    { 936,  8,   8},
    {1103,  8,   9},
    {1109,  8,   9},
    { 858,  8,  10},
    { 840,  8,  10},
    { 241,  8,  11},
    { 234,  8,  11},
    { 804,  8,  12},
    { 811,  8,  12},
    { 172,  8,  13},
    { 148,  8,  13},
    { 680,  8,  14},
    { 707,  8,  14},
    { 927,  8,  15},
    { 930,  8,  15},
    { 679,  8,  16},
    { 706,  8,  16},
    { 926,  8,  17},
    { 929,  8,  17},
    { 894,  8,  18},
    { 876,  8,  18},
    { 808,  9,   0},
    {   2,  9,   0},
    { 809,  9,   0},
    { 865,  9,   1},
    {   3,  9,   1},
    { 864,  9,   1},
    { 814,  9,   2},
    { 793,  9,   2},
    { 120,  9,   2},
    { 629,  9,   3},
    { 127,  9,   3},
    { 649,  9,   3},
    {1090,  9,   4},
    { 133,  9,   4},
    {1097,  9,   4},
    { 278,  9,   5},
    {  14,  9,   5},
    { 281,  9,   6},
    {  23,  9,   6},
    { 283,  9,   7},
    {  26,  9,   7},
    { 286,  9,   8},
    {  29,  9,   8},
    { 290,  9,   9},
    {  33,  9,   9},
    { 291,  9,  10},
    {  34,  9,  10},
    { 292,  9,  11},
    {  36,  9,  11},
    { 293,  9,  12},
    {  37,  9,  12},
    { 294,  9,  13},
    {  39,  9,  13},
    { 295,  9,  14},
    {  40,  9,  14},
    { 296,  9,  15},
    {  44,  9,  15},
    { 297,  9,  16},
    {  49,  9,  16},
    { 298,  9,  17},
    {  54,  9,  17},
    { 299,  9,  18},
    {  60,  9,  18},
    { 300,  9,  19},
    {  65,  9,  19},
    { 301,  9,  20},
    {  67,  9,  20},
    { 302,  9,  21},
    {  68,  9,  21},
    { 303,  9,  22},
    {  69,  9,  22},
    { 304,  9,  23},
    {  70,  9,  23},
    { 305,  9,  24},
    {  71,  9,  24},
    { 306,  9,  25},
    {  78,  9,  25},
    { 307,  9,  26},
    {  82,  9,  26},
    { 308,  9,  27},
    {  83,  9,  27},
    { 309,  9,  28},
    {  84,  9,  28},
    { 310,  9,  29},
    {  85,  9,  29},
    { 311,  9,  30},
    {  86,  9,  30},
    { 312,  9,  31},
    {  87,  9,  31},
    { 313,  9,  32},
    { 132,  9,  32},
    { 314,  9,  33},
    { 139,  9,  33},
    { 279,  9,  34},
    {  21,  9,  34},
    { 280,  9,  35},
    {  22,  9,  35},
    { 282,  9,  36},
    {  25,  9,  36},
    { 284,  9,  37},
    {  27,  9,  37},
    { 285,  9,  38},
    {  28,  9,  38},
    { 287,  9,  39},
    {  31,  9,  39},
    { 288,  9,  40},
    {  32,  9,  40},
    { 196,  9,  41},
    {  48,  9,  41},
    { 192,  9,  41},
    { 194,  9,  42},
    {  50,  9,  42},
    { 190,  9,  42},
    { 195,  9,  43},
    {  51,  9,  43},
    { 191,  9,  43},
    { 217,  9,  44},
    { 229,  9,  44},
    {  53,  9,  44},
    { 216,  9,  45},
    { 201,  9,  45},
    {  55,  9,  45},
    { 218,  9,  46},
    {  56,  9,  46},
    { 243,  9,  46},
    { 675,  9,  47},
    {  57,  9,  47},
    { 650,  9,  47},
    { 923,  9,  48},
    {  58,  9,  48},
    { 915,  9,  48},
    { 142,  9,  49},
    { 148,  9,  49},
    {  59,  9,  49},
    { 141,  9,  50},
    { 140,  9,  50},
    {  61,  9,  50},
    { 143,  9,  51},
    { 167,  9,  51},
    {  62,  9,  51},
    { 444,  9,  52},
    { 420,  9,  52},
    {  63,  9,  52},
    { 443,  9,  53},
    {  64,  9,  53},
    { 415,  9,  53},
    { 601,  9,  54},
    { 604,  9,  54},
    {  66,  9,  54},
    { 289,  9,  55},
    { 197,  9,  56},
    { 802, 10,   0},
    { 266, 10,   1},
    { 263, 10,   1},
    { 369, 10,   2},
    { 359, 10,   2},
    { 494, 10,   3},
    { 799, 10,   4},
    { 786, 10,   4},
    { 592, 10,   5},
    { 591, 10,   5},
    { 737, 10,   6},
    { 736, 10,   6},
    { 490, 10,   7},
    { 489, 10,   7},
    { 606, 10,   8},
    { 605, 10,   8},
    { 326, 10,   9},
    { 455, 10,   9},
    { 994, 10,  10},
    { 991, 10,  10},
    { 986, 10,  11},
    {1088, 10,  12},
    {1087, 10,  12},
    {1104, 10,  13},
    { 785, 10,  14},
    { 784, 10,  14},
    { 968, 10,  15},
    { 971, 10,  15},
    { 983, 10,  16},
    { 982, 10,  16},
    { 497, 10,  17},
    { 496, 10,  17},
    { 789, 11,   0},
    { 780, 11,   0},
    { 160, 11,   1},
    { 140, 11,   1},
    { 539, 11,   2},
    { 533, 11,   2},
    {1104, 11,   3},
    {1100, 11,   3},
    { 498, 11,   4},
    { 484, 11,   4},
    { 785, 11,   5},
    { 782, 11,   5},
    { 800, 12,   0},
    { 147, 12,   1},
    { 149, 12,   2},
    { 152, 12,   3},
    { 215, 12,   4},
    { 221, 12,   5},
    { 416, 12,   6},
    { 417, 12,   7},
    { 448, 12,   8},
    { 488, 12,   9},
    { 492, 12,  10},
    { 499, 12,  11},
    { 500, 12,  12},
    { 536, 12,  13},
    { 541, 12,  14},
    {1036, 12,  14},
    { 554, 12,  15},
    { 558, 12,  16},
    { 559, 12,  17},
    { 560, 12,  18},
    { 623, 12,  19},
    { 634, 12,  20},
    { 647, 12,  21},
    { 652, 12,  22},
    { 653, 12,  23},
    { 738, 12,  24},
    { 750, 12,  25},
    { 806, 12,  26},
    { 820, 12,  27},
    { 877, 12,  28},
    { 909, 12,  29},
    { 910, 12,  30},
    { 919, 12,  31},
    { 921, 12,  32},
    { 939, 12,  33},
    { 940, 12,  34},
    { 952, 12,  35},
    { 954, 12,  36},
    { 962, 12,  37},
    {1008, 12,  38},
    {1021, 12,  39},
    {1034, 12,  40},
    {1035, 12,  41},
    {1041, 12,  42},
    {1101, 12,  43},
    {1015, 12,  44},
    {1120, 12,  45},
    {1121, 12,  46},
    {1122, 12,  47},
    {1130, 12,  48},
    {1131, 12,  49},
    {1134, 12,  50},
    {1135, 12,  51},
    { 640, 12,  52},
    { 487, 12,  53},
    { 257, 12,  54},
    { 486, 12,  55},
    { 822, 12,  56},
    { 931, 12,  57},
    { 803, 13,   0},
    {1064, 13,   0},
    { 616, 13,   1},
    { 260, 13,   1},
    { 447, 13,   2},
    { 414, 13,   2},
    { 924, 13,   3},
    { 915, 13,   3},
    {1060, 13,   4},
    {1016, 13,   4},
    { 677, 13,   5},
    {1074, 14,   0},
    {1118, 14,   0},
    { 842, 14,   1},
    { 841, 14,   1},
    { 354, 14,   2},
    { 351, 14,   2},
    { 913, 14,   3},
    { 912, 14,   3},
    { 513, 14,   4},
    { 510, 14,   4},
    { 805, 14,   5},
    { 810, 14,   5},
    { 479, 14,   6},
    { 478, 14,   6},
    { 252, 14,   7},
    {1009, 14,   7},
    { 589, 14,   8},
    { 604, 14,   8},
    { 897, 14,   9},
    { 896, 14,   9},
    { 895, 14,  10},
    { 893, 14,  10},
    { 817, 14,  11},
    { 813, 14,  11},
    { 156, 14,  12},
    { 148, 14,  12},
    { 576, 14,  13},
    { 572, 14,  13},
    { 598, 14,  14},
    { 599, 14,  14},
    { 585, 14,  14},
    { 571, 14,  15},
    { 570, 14,  15},
    { 364, 14,  16},
    { 355, 14,  16},
    { 250, 14,  17},
    { 213, 14,  17},
    { 249, 14,  18},
    { 203, 14,  18},
    { 977, 14,  19},
    { 976, 14,  19},
    { 725, 14,  20},
    { 228, 14,  20},
    { 271, 14,  21},
    { 391, 14,  21},
    { 694, 14,  22},
    { 686, 14,  22},
    { 382, 14,  23},
    { 275, 14,  23},
    { 371, 14,  24},
    { 938, 14,  24},
    { 160, 14,  25},
    { 146, 14,  25},
    { 251, 14,  26},
    { 202, 14,  26},
    {1007, 14,  27},
    { 958, 14,  27},
    {1141, 14,  28},
    {1140, 14,  28},
    { 792, 14,  29},
    { 796, 14,  29},
    {1108, 14,  30},
    {1105, 14,  30},
    { 614, 14,  31},
    { 621, 14,  32},
    { 620, 14,  33},
    { 534, 14,  34},
    { 535, 14,  35},
    { 353, 14,  36},
    { 389, 14,  36},
    { 557, 14,  37},
    { 568, 14,  37},
    { 372, 14,  38},
    { 327, 14,  38},
    { 917, 14,  39},
    { 922, 14,  39},
    { 802, 15,   0},
    { 817, 15,   1},
    { 813, 15,   1},
    { 439, 15,   2},
    { 433, 15,   2},
    { 422, 15,   3},
    { 421, 15,   3},
    { 783, 16,   0},
    {   0, 16,   1},
    {   1, 16,   2},
    {   2, 16,   3},
    {   5, 16,   4},
    {   4, 16,   5},
    {  13, 16,   6},
    {  12, 16,   7},
    {  11, 16,   8},
    {  10, 16,   9},
    {  73, 16,  10},
    {   9, 16,  11},
    {   8, 16,  12},
    {   7, 16,  13},
    {  77, 16,  14},
    {  43, 16,  15},
    {   6, 16,  16},
    {  76, 16,  17},
    { 110, 16,  18},
    {  42, 16,  19},
    {  75, 16,  20},
    {  92, 16,  21},
    { 109, 16,  22},
    { 122, 16,  23},
    {   3, 16,  24},
    {  74, 16,  25},
    {  41, 16,  26},
    { 108, 16,  27},
    {  72, 16,  28},
    { 121, 16,  29},
    {  91, 16,  30},
    { 134, 16,  31},
    { 107, 16,  32},
    {  24, 16,  33},
    { 115, 16,  34},
    {  30, 16,  35},
    { 120, 16,  36},
    {  35, 16,  37},
    { 127, 16,  38},
    {  38, 16,  39},
    { 133, 16,  40},
    {  14, 16,  41},
    {  23, 16,  42},
    {  26, 16,  43},
    {  29, 16,  44},
    {  33, 16,  45},
    {  34, 16,  46},
    {  36, 16,  47},
    {  37, 16,  48},
    {  39, 16,  49},
    {  40, 16,  50},
    {  44, 16,  51},
    {  49, 16,  52},
    {  54, 16,  53},
    {  60, 16,  54},
    {  65, 16,  55},
    {  67, 16,  56},
    {  68, 16,  57},
    {  69, 16,  58},
    {  70, 16,  59},
    {  71, 16,  60},
    {  78, 16,  61},
    {  82, 16,  62},
    {  83, 16,  63},
    {  84, 16,  64},
    {  85, 16,  65},
    {  86, 16,  66},
    {  87, 16,  67},
    {  88, 16,  68},
    {  89, 16,  69},
    {  90, 16,  70},
    {  93, 16,  71},
    {  97, 16,  72},
    {  98, 16,  73},
    {  99, 16,  74},
    { 101, 16,  75},
    { 102, 16,  76},
    { 103, 16,  77},
    { 104, 16,  78},
    { 105, 16,  79},
    { 106, 16,  80},
    { 111, 16,  81},
    { 116, 16,  82},
    { 123, 16,  83},
    { 128, 16,  84},
    { 135, 16,  85},
    {  15, 16,  86},
    {  45, 16,  87},
    {  79, 16,  88},
    {  94, 16,  89},
    { 112, 16,  90},
    { 117, 16,  91},
    { 124, 16,  92},
    { 129, 16,  93},
    { 136, 16,  94},
    {  16, 16,  95},
    {  46, 16,  96},
    {  80, 16,  97},
    {  95, 16,  98},
    { 113, 16,  99},
    { 118, 16, 100},
    { 125, 16, 101},
    { 130, 16, 102},
    { 137, 16, 103},
    {  17, 16, 104},
    {  47, 16, 105},
    {  81, 16, 106},
    {  96, 16, 107},
    { 114, 16, 108},
    { 119, 16, 109},
    { 126, 16, 110},
    { 131, 16, 111},
    { 138, 16, 112},
    {  18, 16, 113},
    {  52, 16, 114},
    { 100, 16, 115},
    {  19, 16, 116},
    {  20, 16, 117},
    { 782, 17,   0},
    { 923, 17,   1},
    { 675, 17,   2},
    {1092, 17,   3},
    { 676, 17,   4},
    {1053, 17,   5},
    { 239, 17,   6},
    {1054, 17,   7},
    {1058, 17,   8},
    {1056, 17,   9},
    {1057, 17,  10},
    { 240, 17,  11},
    {1055, 17,  12},
    { 866, 17,  13},
    { 600, 17,  14},
    { 849, 18,   0},
    { 227, 18,   1},
    {1091, 18,   2},
    { 198, 18,   3},
    { 814, 18,   4},
    {1090, 18,   5},
    {1095, 18,   6},
    {1094, 18,   7},
    {1093, 18,   8},
    { 379, 18,   9},
    { 374, 18,  10},
    { 375, 18,  11},
    { 380, 18,  12},
    { 381, 18,  13},
    { 378, 18,  14},
    { 376, 18,  15},
    { 377, 18,  16},
    { 770, 18,  17},
    {1051, 18,  18},
    {1052, 18,  19},
    { 918, 18,  20},
};

/* property values: 4920 bytes. */

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
    19, 19, 19, 19, 19, 19, 23, 19, 19, 19, 24, 19, 19, 25, 26, 19,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 27,  7, 28, 29, 19, 19, 19, 19, 19, 19, 19, 30,
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
    31, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 32,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 32,
};

static RE_UINT8 re_general_category_stage_2[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
     32,  33,  34,  34,  35,  36,  37,  38,  39,  34,  34,  34,  40,  41,  42,  43,
     44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
     60,  61,  62,  63,  64,  64,  65,  66,  67,  68,  69,  70,  71,  69,  72,  73,
     69,  69,  64,  74,  64,  64,  75,  76,  77,  78,  79,  80,  81,  82,  69,  83,
     84,  85,  86,  87,  88,  89,  69,  69,  34,  34,  34,  34,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  90,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  91,
     92,  34,  34,  34,  34,  34,  34,  34,  34,  93,  34,  34,  94,  95,  96,  97,
     98,  99, 100, 101, 102, 103, 104, 105,  34,  34,  34,  34,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 106,
    107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
    108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108,
    108, 108,  34,  34, 109, 110, 111, 112,  34,  34, 113, 114, 115, 116, 117, 118,
    119, 120, 121, 122,  76, 123, 124, 125, 126, 127,  76,  76,  76,  76,  76,  76,
    128,  76, 129, 130, 131,  76, 132,  76, 133,  76,  76,  76, 134,  76,  76,  76,
    135, 136, 137, 138,  76,  76,  76,  76,  76,  76,  76,  76,  76, 139,  76,  76,
     76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,
     34,  34,  34,  34,  34,  34, 140,  76, 141,  76,  76,  76,  76,  76,  76,  76,
     34,  34,  34,  34,  34,  34,  34,  34, 142,  76,  76,  76,  76,  76,  76,  76,
     34,  34,  34,  34, 143,  76,  76,  76,  76,  76,  76,  76,  76,  76, 144, 145,
    146,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,
     69, 147, 148, 149, 150,  76, 151,  76, 152, 153, 154, 155, 156, 157, 158, 159,
     76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76, 160, 161,  76,  76,
    162, 163, 164, 165, 166,  76, 167, 168, 169, 170, 171, 172, 173, 174, 175,  76,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 176,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 177,  34,
    178,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,
     34,  34,  34,  34, 178,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,
    179,  76, 180, 181,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,
    108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 182,
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
     50,  69,  70,  71,  50,  72,  73,  73,  73,  73,  74,  73,  73,  73,  75,  76,
     77,  50,  50,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,
     91,  84,  85,  92,  93,  94,  95,  96,  97,  98,  85,  99, 100, 101,  89, 102,
     83,  84,  85, 103, 104, 105,  89, 106, 107, 108, 109, 110, 111, 112,  95, 113,
    114, 115,  85, 116, 117, 118,  89, 119, 120, 115,  85, 121, 122, 123,  89, 124,
    120, 115,  50, 125, 126, 127,  89, 128, 129, 130,  50, 131, 132, 133,  73, 134,
    135,  50,  50, 136, 137, 138,  73,  73, 139, 140, 141, 142, 143, 144,  73,  73,
    145, 146, 147, 148, 149,  50, 150, 151, 152, 153,  32, 154, 155, 156,  73,  73,
     50,  50, 157, 158, 159, 160, 161, 162, 163, 164,   9,   9, 165,  50,  50, 166,
     50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 167, 168,  50,  50,
    167,  50,  50, 169, 170, 171,  50,  50,  50, 170,  50,  50,  50, 172, 173, 174,
     50, 175,  50,  50,  50,  50,  50, 176, 177,  50,  50,  50,  50,  50,  50,  50,
     50,  50,  50,  50,  50,  50, 178,  50, 179, 180,  50,  50,  50,  50, 181, 182,
    183, 184,  50, 185,  50, 186, 183, 187,  50,  50,  50, 188, 189, 190, 191, 192,
    193, 191,  50,  50, 194,  50,  50, 195,  50,  50, 196,  50,  50,  50,  50, 197,
     50, 150, 198, 199, 200,  50, 201, 176,  50,  50, 202, 203, 204, 205, 206, 206,
     50, 207,  50,  50,  50, 208, 209, 210, 191, 191, 211,  73,  73,  73,  73,  73,
    212,  50,  50, 213, 214, 159, 215, 216, 217,  50, 218,  66,  50,  50, 219, 220,
     50,  50, 221, 222, 223,  66,  50, 224,  73,  73,  73,  73, 225, 226, 227, 228,
     11,  11, 229,  27,  27,  27, 230, 231,  11, 232,  27,  27,  32,  32, 233, 234,
     13,  13,  13,  13,  13,  13,  13,  13,  13, 235,  13,  13,  13,  13,  13,  13,
    236, 237, 236, 236, 237, 238, 236, 239, 240, 240, 240, 241, 242, 243, 244, 245,
    246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257,  73, 258, 259, 260,
    261, 262, 263, 264, 265, 266, 267, 267, 268, 269, 270, 206, 271, 272, 206, 273,
    274, 274, 274, 274, 274, 274, 274, 274, 275, 206, 276, 206, 206, 206, 206, 277,
    206, 278, 274, 279, 206, 280, 281, 282, 206, 206, 283,  73, 284,  73, 266, 266,
    266, 285, 206, 206, 206, 206, 286, 266, 206, 206, 206, 206, 206, 206, 206, 206,
    206, 206, 206, 287, 288, 206, 206, 289, 206, 206, 206, 206, 206, 206, 290, 206,
    291, 206, 206, 206, 206, 206, 292, 293, 266, 294, 206, 206, 295, 274, 296, 274,
    297, 298, 274, 274, 274, 299, 274, 300, 206, 206, 206, 274, 301, 175,  73,  73,
     73,  73,  73,  73,  73,  73,  73,  73,   9,   9, 302,  11,  11, 303, 304, 305,
     13,  13,  13,  13,  13,  13, 306, 307,  11,  11, 308,  50,  50,  50, 309, 310,
     50, 311, 312, 312, 312, 312,  32,  32, 313, 314, 315, 316,  73,  73,  73,  73,
    206, 317, 206, 206, 206, 206, 206, 282, 206, 206, 206, 206, 206, 318,  73, 319,
    320, 321, 322, 323, 135,  50,  50,  50,  50, 324, 177,  50,  50,  50,  50, 325,
    326,  50, 201, 135,  50,  50,  50,  50, 327, 328,  50,  51, 206, 206, 282,  50,
    206, 329, 330, 206, 331, 332, 206, 206, 330, 206, 206, 332, 206, 206, 206, 329,
     50,  50,  50, 197, 206, 206, 206, 206,  50,  50,  50,  50, 150,  73,  73,  73,
     50, 333,  50,  50,  50,  50,  50,  50, 150, 206, 206, 206, 283,  50,  50, 224,
    334,  50, 335,  73,  13,  13, 336, 337,  13, 338,  50,  50,  50,  50, 339, 340,
     31, 341, 342, 343,  13,  13,  13, 344, 345, 346, 347,  73,  73,  73,  73, 348,
    349,  50, 350, 351,  50,  50,  50, 352, 353,  50,  50, 354, 355, 191,  32, 356,
     66,  50, 357,  50, 358, 359,  50, 150,  77,  50,  50, 360, 361, 362,  73,  73,
     50,  50, 363, 364, 365, 366,  50, 367,  50,  50,  50, 368, 369, 370, 371, 372,
    373, 374, 312,  73,  73,  73,  73,  73,  73,  73,  73,  73,  50,  50, 375, 191,
     50,  50, 376,  50, 377,  50,  50, 202, 378, 378, 378, 378, 378, 378, 378, 378,
    379, 379, 379, 379, 379, 379, 379, 379,  50,  50,  50,  50,  50,  50, 201,  50,
     50,  50,  50,  50,  50, 380,  73,  73, 381, 382, 383, 384, 385,  50,  50,  50,
     50,  50,  50, 386, 387, 388,  50,  50,  50,  50,  50, 389,  73,  50,  50,  50,
     50, 390,  50,  50, 195,  73,  73, 391,  32, 392, 233, 393, 394, 395, 396, 397,
     50,  50,  50,  50,  50,  50,  50, 398, 399,   2,   3,   4,   5, 400, 401, 402,
     50, 403,  50, 327, 404, 405, 406, 407, 408,  50, 171, 409, 201, 201,  73,  73,
     50,  50,  50,  50,  50,  50,  50,  51, 410, 266, 266, 411, 267, 267, 267, 412,
    413, 319,  73,  73,  73, 206, 206, 414,  50, 150,  50,  50,  50, 101,  73,  73,
     50, 327, 415,  50, 416,  73,  73,  73,  50, 417,  50,  50, 418, 419,  73,  73,
      9,   9, 420,  11,  11,  50,  50,  50,  50, 201, 191,  73,  73,  73,  73,  73,
    421,  50,  50, 422,  50, 423,  73,  73,  50, 424,  50, 425,  73,  73,  73,  73,
     50,  50,  50, 426,  73,  73,  73,  73, 427, 428,  50, 429, 430, 431,  50, 432,
     50,  50,  50, 433,  50, 434,  50, 435,  50,  50,  50,  50, 436,  73,  73,  73,
     73,  73,  73,  73,  73,  73, 266, 437, 438,  50,  50, 439, 440, 441, 442,  73,
    217,  50,  50, 443, 444,  50, 436, 191, 445,  50, 446, 447, 448,  73,  73,  73,
    217,  50,  50, 449, 450, 191,  73,  73,  50,  50, 451, 452, 191,  73,  73,  73,
     50,  50,  50,  50,  50,  50, 327,  73, 267, 267, 267, 267, 267, 267, 453, 448,
     50,  50, 327,  73,  73,  73,  73,  73,  50,  50,  50, 436,  73,  73,  73,  73,
     50,  50,  50,  50, 176, 454, 203, 455, 456, 457,  73,  73,  73,  73,  73,  73,
    458,  73,  73,  73,  73,  73,  73,  73, 206, 206, 206, 206, 206, 206, 206, 318,
    206, 206, 459, 206, 206, 206, 460, 461, 462, 206, 463, 206, 206, 464,  73,  73,
    206, 206, 206, 206, 465,  73,  73,  73, 206, 206, 206, 206, 206, 283, 266, 466,
      9, 467,  11, 468, 469, 470, 236,   9, 471, 472, 473, 474, 475,   9, 467,  11,
    476, 477,  11, 478, 479, 480, 481,   9, 482,  11,   9, 467,  11, 468, 469,  11,
    236,   9, 471, 481,   9, 482,  11,   9, 467,  11, 483,   9, 484, 485, 486, 487,
     11, 488,   9, 489, 490, 491, 492,  11, 493,   9, 494,  11, 495, 496, 496, 496,
    497,  50, 498, 499, 500, 501, 502, 503, 504, 202, 505, 202,  73,  73,  73, 506,
    206, 206, 319, 206, 206, 206, 206, 206, 206, 282, 329, 507, 291, 291,  73,  73,
    508, 206, 329, 206, 206, 206, 319, 206, 206, 284,  73,  73,  73,  73, 509, 206,
    510, 206, 206, 284, 511, 512,  73,  73, 206, 206, 513, 514, 206, 206, 206, 515,
    206, 282, 206, 206, 516,  73, 206, 513, 206, 206, 206, 329, 517, 206, 206, 206,
    206, 206, 206, 206, 206, 206, 206, 518, 206, 206, 206, 464, 282, 206, 519,  73,
     73,  73,  73,  73,  73,  73,  73, 520, 206, 206, 206, 206, 521,  73,  73,  73,
    206, 206, 206, 206, 318,  73,  73,  73, 206, 206, 206, 206, 206, 206, 206, 282,
     50,  50,  50,  50,  50, 311,  73,  73,  50,  50,  50, 176,  50,  50,  50,  50,
     50, 201,  73,  73,  73,  73,  73,  73, 522,  73, 523, 523, 523, 523, 523, 523,
     32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  73,
    379, 379, 379, 379, 379, 379, 379, 524,
};

static RE_UINT8 re_general_category_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   3,   2,   4,   5,   6,   2,
      7,   7,   7,   7,   7,   2,   8,   9,  10,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  17,  18,  19,   1,  20,  20,  21,  22,  23,  24,  25,
     26,  27,  15,   2,  28,  29,  27,  30,  11,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  31,  11,  11,  11,  32,  16,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  33,  16,  16,  16,  16,  32,  32,  32,  32,  32,  32,  32,  32,
     32,  32,  32,  32,  34,  34,  34,  34,  34,  34,  34,  34,  16,  32,  32,  32,
     32,  32,  32,  32,  11,  34,  34,  16,  34,  32,  32,  11,  34,  11,  16,  11,
     11,  34,  32,  11,  32,  16,  11,  34,  32,  32,  32,  11,  34,  16,  32,  11,
     34,  11,  34,  34,  32,  35,  32,  16,  36,  36,  37,  34,  38,  37,  34,  34,
     34,  34,  34,  34,  34,  34,  16,  32,  34,  38,  32,  11,  32,  32,  32,  32,
     32,  32,  16,  16,  16,  11,  34,  32,  34,  34,  11,  32,  32,  32,  32,  32,
     16,  16,  39,  16,  16,  16,  16,  16,  40,  40,  40,  40,  40,  40,  40,  40,
     40,  41,  41,  40,  40,  40,  40,  40,  40,  41,  41,  41,  41,  41,  41,  41,
     40,  40,  42,  41,  41,  41,  42,  42,  41,  41,  41,  41,  41,  41,  41,  41,
     43,  43,  43,  43,  43,  43,  43,  43,  32,  32,  42,  32,  44,  45,  16,  46,
     44,  44,  41,  47,  11,  48,  48,  11,  34,  11,  11,  11,  11,  11,  11,  11,
     11,  49,  11,  11,  11,  11,  16,  16,  16,  16,  16,  16,  16,  16,  16,  34,
     16,  11,  32,  16,  32,  32,  32,  32,  16,  16,  32,  50,  34,  32,  34,  11,
     32,  51,  43,  43,  52,  32,  32,  32,  11,  34,  34,  34,  34,  34,  34,  16,
     32,  32,  32,  32,  44,  44,  44,  44,  49,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  48,  53,   2,   2,   2,  54,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  55,  56,  44,  57,  58,  43,  43,  43,  43,  43,  43,  43,
     43,  43,  43,  43,  43,  43,  43,  59,  60,  61,  43,  60,  44,  44,  44,  44,
     36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  62,  44,  44,
     36,  63,  46,  44,  44,  44,  44,  44,  64,  64,  65,   8,   9,  66,   2,  67,
     43,  43,  43,  43,  43,  61,  44,   2,  68,  36,  36,  36,  36,  69,  43,  43,
      7,   7,   7,   7,   7,   2,   2,  36,  70,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  71,  43,  43,  43,  72,  51,  43,  43,  73,  74,  75,  43,  43,  36,
      7,   7,   7,   7,   7,  36,  76,  77,   2,   2,   2,   2,   2,   2,   2,  78,
     69,  36,  36,  36,  36,  36,  36,  36,  43,  43,  43,  43,  43,  79,  80,  36,
     36,  36,  36,  43,  43,  43,  43,  43,  70,  44,  44,  44,  44,  44,  44,  44,
      7,   7,   7,   7,   7,  36,  36,  36,  36,  36,  36,  36,  36,  69,  43,  43,
     43,  43,  40,  21,   2,  81,  44,  44,  36,  36,  36,  43,  43,  74,  43,  43,
     43,  43,  74,  43,  74,  43,  43,  44,   2,   2,   2,   2,   2,   2,   2,  46,
     36,  36,  36,  36,  69,  43,  44,  46,  44,  44,  44,  44,  44,  44,  44,  44,
     62,  36,  36,  36,  36,  36,  62,  44,  44,  44,  43,  43,  43,  43,  43,  43,
     43,  43,  43,  43,  43,  43,  43,  79,  43,  82,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  82,  70,  83,  84,  43,  43,  43,  82,  83,  84,  83,
     69,  43,  43,  43,  36,  36,  36,  36,  36,  43,   2,   7,   7,   7,   7,   7,
     85,  36,  36,  36,  80,  36,  36,  36,  58,  83,  80,  36,  36,  36,  62,  80,
     62,  80,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  62,  36,  36,  36,
     62,  62,  44,  36,  36,  44,  70,  83,  84,  43,  79,  86,  87,  86,  84,  62,
     44,  44,  44,  86,  44,  44,  36,  80,  36,  43,  44,   7,   7,   7,   7,   7,
     36,  20,  27,  27,  27,  88,  44,  44,  58,  82,  80,  36,  36,  62,  44,  80,
     62,  36,  80,  62,  36,  44,  79,  83,  84,  79,  44,  58,  79,  58,  43,  44,
     58,  44,  44,  44,  80,  36,  62,  62,  44,  44,  44,   7,   7,   7,   7,   7,
     43,  36,  69,  44,  44,  44,  44,  44,  58,  82,  80,  36,  36,  36,  36,  80,
     36,  80,  36,  36,  36,  36,  36,  36,  62,  36,  80,  36,  36,  44,  70,  83,
     84,  43,  43,  58,  82,  86,  84,  44,  62,  44,  44,  44,  44,  44,  44,  44,
     66,  44,  44,  44,  44,  44,  44,  44,  62,  36,  80,  36,  36,  44,  70,  84,
     84,  43,  79,  86,  87,  86,  84,  44,  44,  44,  44,  82,  44,  44,  36,  80,
     77,  27,  27,  27,  44,  44,  44,  44,  44,  70,  80,  36,  36,  62,  44,  36,
     62,  36,  36,  44,  80,  62,  62,  36,  44,  80,  62,  44,  36,  62,  44,  36,
     36,  36,  36,  36,  36,  44,  44,  83,  82,  87,  44,  83,  87,  83,  84,  44,
     62,  44,  44,  86,  44,  44,  44,  44,  27,  89,  67,  67,  88,  90,  44,  44,
     86,  83,  80,  36,  36,  36,  62,  36,  62,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  80,  36,  36,  44,  80,  43,  82,  83,  87,  43,  79,  43,  43,  44,
     44,  44,  58,  79,  36,  44,  44,  44,  44,  44,  44,  44,  27,  27,  27,  89,
     44,  83,  80,  36,  36,  36,  62,  36,  36,  36,  80,  36,  36,  44,  70,  84,
     83,  83,  87,  82,  87,  83,  43,  44,  44,  44,  86,  87,  44,  44,  44,  62,
     80,  62,  44,  44,  44,  44,  44,  44,  36,  36,  36,  36,  36,  62,  80,  83,
     84,  43,  79,  83,  87,  83,  84,  62,  44,  44,  44,  86,  44,  44,  44,  44,
     27,  27,  27,  44,  91,  36,  36,  36,  44,  83,  80,  36,  36,  36,  36,  36,
     36,  36,  36,  62,  44,  36,  36,  36,  36,  80,  36,  36,  36,  36,  80,  44,
     36,  36,  36,  62,  44,  79,  44,  86,  83,  43,  79,  79,  83,  83,  83,  83,
     44,  83,  46,  44,  44,  44,  44,  44,  80,  36,  36,  36,  36,  36,  36,  36,
     69,  36,  43,  43,  43,  79,  44,  57,  36,  36,  36,  74,  43,  43,  43,  61,
      7,   7,   7,   7,   7,   2,  44,  44,  80,  62,  62,  80,  62,  62,  80,  44,
     44,  44,  36,  36,  80,  36,  36,  36,  80,  36,  80,  80,  44,  36,  80,  36,
     69,  36,  43,  43,  43,  58,  70,  44,  36,  36,  62,  81,  43,  43,  43,  44,
      7,   7,   7,   7,   7,  44,  36,  36,  76,  67,   2,   2,   2,   2,   2,   2,
      2,  92,  92,  67,  43,  67,  67,  67,   7,   7,   7,   7,   7,  27,  27,  27,
     27,  27,  51,  51,  51,   4,   4,  83,  36,  36,  36,  36,  80,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  62,  44,  58,  43,  43,  43,  43,  43,  43,  82,
     43,  43,  61,  43,  36,  36,  69,  43,  43,  43,  43,  43,  58,  43,  43,  43,
     43,  43,  43,  43,  43,  43,  79,  67,  67,  67,  67,  75,  67,  67,  90,  67,
      2,   2,  92,  67,  21,  46,  44,  44,  36,  36,  36,  36,  36,  93,  84,  43,
     82,  43,  43,  43,  84,  82,  84,  70,   7,   7,   7,   7,   7,   2,   2,   2,
     36,  36,  36,  83,  43,  36,  36,  43,  70,  83,  94,  93,  83,  83,  83,  36,
     69,  43,  70,  36,  36,  36,  36,  36,  36,  82,  84,  82,  83,  83,  84,  93,
      7,   7,   7,   7,   7,  83,  84,  67,  11,  11,  11,  49,  44,  44,  49,  44,
     36,  36,  36,  36,  36,  63,  68,  36,  36,  36,  36,  36,  62,  36,  36,  44,
     36,  36,  36,  62,  62,  36,  36,  44,  62,  36,  36,  44,  36,  36,  36,  62,
     62,  36,  36,  44,  36,  36,  36,  36,  36,  36,  36,  62,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  62,  58,  43,   2,   2,   2,   2,  95,  27,  27,  27,
     27,  27,  27,  27,  27,  27,  96,  44,  67,  67,  67,  67,  67,  44,  44,  44,
     36,  36,  62,  44,  44,  44,  44,  44,  97,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  63,  71,  98,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  99, 100,  44,  36,  36,  36,  36,  36,  63,   2, 101,
    102,  44,  44,  44,  44,  44,  44,  44,  36,  36,  36,  36,  36,  36,  62,  36,
     36,  43,  79,  44,  44,  44,  44,  44,  36,  43,  61,  46,  44,  44,  44,  44,
     36,  43,  44,  44,  44,  44,  44,  44,  62,  43,  44,  44,  44,  44,  44,  44,
     36,  36,  43,  84,  43,  43,  43,  83,  83,  83,  83,  82,  84,  43,  43,  43,
     43,  43,   2,  85,   2,  66,  69,  44,   7,   7,   7,   7,   7,  44,  44,  44,
     27,  27,  27,  27,  27,  44,  44,  44,   2,   2,   2, 103,   2,  60,  43, 104,
     36, 105,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  44,  44,  44,  44,
     36,  36,  36,  36,  69,  62,  44,  44,  36,  36,  36,  44,  44,  44,  44,  44,
     43,  82,  83,  84,  82,  83,  44,  44,  83,  82,  83,  83,  84,  43,  44,  44,
     90,  44,   2,   7,   7,   7,   7,   7,  36,  36,  36,  36,  36,  36,  36,  44,
     36,  36,  36,  36,  36,  36,  44,  44,  83,  83,  83,  83,  83,  83,  83,  83,
     94,  36,  36,  36,  83,  44,  44,  44,   7,   7,   7,   7,   7,  96,  44,  67,
     67,  67,  67,  67,  67,  67,  67,  67,  36,  36,  36,  69,  82,  83,  44,   2,
     36,  36,  93,  82,  43,  43,  43,  79,  82,  82,  84,  43,  43,  43,  82,  83,
     83,  84,  43,  43,  43,  43,  79,  58,   2,   2,   2,  85,   2,   2,   2,  44,
     43,  43,  94,  36,  36,  36,  36,  36,  36,  36,  82,  43,  43,  82,  82,  83,
     83,  82,  94,  36,  36,  36,  44,  44,  92,  67,  67,  67,  67,  51,  43,  43,
     43,  43,  67,  67,  67,  67,  90,  44,  43,  94,  36,  36,  36,  36,  36,  36,
     93,  43,  43,  83,  43,  84,  83,  36,  36,  36,  36,  82,  43,  83,  84,  84,
     43,  83,  44,  44,  44,  44,   2,   2,  36,  36,  83,  83,  83,  83,  43,  43,
     43,  43,  83,  43,  44,  55,   2,   2,   7,   7,   7,   7,   7,  44,  80,  36,
     36,  36,  36,  36,  40,  40,  40,   2,   2,   2,   2,   2,  44,  44,  44,  44,
     43,  61,  43,  43,  43,  43,  43,  43,  82,  43,  43,  43,  70,  36,  69,  36,
     36,  83,  70,  62,  44,  44,  44,  44,  16,  16,  16,  16,  16,  16,  40,  40,
     40,  40,  40,  40,  40,  45,  16,  16,  16,  16,  16,  16,  45,  16,  16,  16,
     16,  16,  16,  16,  16, 106,  40,  40,  43,  43,  43,  79,  44,  44,  44,  44,
     44,  44,  44,  44,  44,  44,  43,  43,  32,  32,  32,  16,  16,  16,  16,  32,
     16,  16,  16,  16,  11,  11,  11,  11,  16,  16,  16,  44,  11,  11,  11,  44,
     16,  16,  16,  16,  49,  49,  49,  49,  16,  16,  16,  16,  16,  16,  16,  44,
     16,  16,  16,  16, 107, 107, 107, 107,  16,  16, 108,  16,  11,  11, 109, 110,
     41,  16, 108,  16,  11,  11, 109,  41,  16,  16,  44,  16,  11,  11, 111,  41,
     16,  16,  16,  16,  11,  11, 112,  41,  44,  16, 108,  16,  11,  11, 109, 113,
    114, 114, 114, 114, 114, 115,  64,  64, 116, 116, 116,   2, 117, 118, 117, 118,
      2,   2,   2,   2, 119,  64,  64, 120,   2,   2,   2,   2, 121, 122,   2, 123,
    124,   2, 125, 126,   2,   2,   2,   2,   2,   9, 124,   2,   2,   2,   2, 127,
     64,  64,  65,  44,  44,  64,  64,  64, 128,  44,  27,  27,  27,   8, 125, 129,
     27,  27,  27,  27,  27,   8, 125, 100,  40,  40,  40,  40,  40,  40,  81,  44,
     20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20, 130,  44,  44,
     43,  43,  43,  43,  43,  43, 131,  52, 132,  52, 132,  43,  43,  43,  43,  43,
     79,  44,  44,  44,  44,  44,  44,  44,  67, 133,  67, 134,  67,  34,  11,  16,
     11,  32, 134,  67,  50,  11,  11,  67,  67,  67, 133, 133, 133,  11,  11, 135,
     11,  11,  35,  36,  39,  67,  16,  11,   8,   8,  50,  16,  16,  26,  67, 136,
     27,  27,  27,  27,  27,  27,  27,  27, 101, 101, 101, 101, 101, 101, 101, 101,
    101, 137, 138, 101, 139,  44,  44,  44,   8,   8, 140,  67,  67,   8,  67,  67,
    140,  26,  67, 140,  67,  67,  67, 140,  67,  67,  67,  67,  67,  67,  67,   8,
     67, 140, 140,  67,  67,  67,  67,  67,  67,  67,   8,   8,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8,   8,   8,  67,  67,  67,  67,   8,   8,  67,  67,
      8,  67,  67,  67, 141, 142,  67,  67,  67,  67,  67,  67,  67,  67, 140,  67,
     67,  67,  67,  67,  67,  26,   8,   8,   8,   8,  67,  67,  67,  67,  67,  67,
     67,  67,  67,  67,  67,  67,   8,   8,   8,  67,  67,  67,  67,  67,  67,  67,
     67,  67,  44,  44,  44,  44,  44,  44,  67,  67,  67,  90,  44,  44,  44,  44,
     67,  67,  67,  67,  67,  90,  44,  44,  27,  27,  27,  27,  27,  27,  67,  67,
     67,  67,  67,  67,  67,  27,  27,  27,  67,  67,  67,  26,  67,  67,  67,  67,
     26,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,   8,   8,   8,   8,
     67,  67,  67,  67,  67,  67,  67,  26,  91,  67,  67,  67,  67,  67,  67,  67,
     67,  67,  67,  67,   4,   4,   4,   4,   4,   4,   4,  27,  27,  27,  27,  27,
     27,  27,  67,  67,  67,  67,  67,  67,   8,   8, 125, 143,   8,   8,   8,   8,
      8,   8,   8,   4,   4,   4,   4,   4,   8, 125, 144, 144, 144, 144, 144, 144,
    144, 144, 144, 144, 143,   8,   8,   8,   8,   8,   8,   8,   4,   4,   8,   8,
      8,   8,   8,   8,   8,   8,   4,   8,   8,   8, 140,  26,   8,   8, 145,  44,
     11,  11,  11,  11,  11,  11,  11,  48,  16,  16,  16,  16,  16,  16,  16, 108,
     32,  11,  32,  34,  34,  34,  34,  11,  32,  32,  34,  16,  16,  16,  40,  11,
     32,  32, 136,  67,  67, 134,  34, 146,  43,  32,  44,  44,  55,   2,  95,   2,
     16,  16,  16,  54,  44,  44,  54,  44,  36,  36,  36,  36,  44,  44,  44,  53,
     46,  44,  44,  44,  44,  44,  44,  58,  36,  36,  36,  62,  44,  44,  44,  44,
     36,  36,  36,  62,  36,  36,  36,  62,   2, 117, 117,   2, 121, 122, 117,   2,
      2,   2,   2,   6,   2, 103, 117,   2, 117,   4,   4,   4,   4,   2,   2,  85,
      2,   2,   2,   2,   2, 116,  44,  44,  67,  67,  67,  67,  67,  91,  67,  67,
     67,  67,  67,  44,  44,  44,  44,  44,  67,  67,  67,  67,  67,  67,  44,  44,
      1,   2, 147, 148,   4,   4,   4,   4,   4,  67,   4,   4,   4,   4, 149, 150,
    151, 101, 101, 101, 101,  43,  43,  83, 152,  40,  40,  67, 101, 153,  63,  67,
     36,  36,  36,  62,  58, 154, 155,  68,  36,  36,  36,  36,  36,  63,  40,  68,
     44,  44,  80,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  62,
     67,  27,  27,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  90,
     27,  27,  27,  27,  27,  67,  67,  67,  67,  67,  67,  67,  27,  27,  27,  27,
    156,  27,  27,  27,  27,  27,  27,  27,  36,  36, 105,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36, 157,   2,   7,   7,   7,   7,   7,  36,  44,  44,
     32,  32,  32,  32,  32,  32,  32,  69,  52, 158,  43,  43,  43,  43,  43,  85,
     32,  32,  32,  32,  44,  44,  44,  58,  36,  36,  36, 101, 101, 101, 101, 101,
     43,   2,   2,   2,  44,  44,  44,  44,  41,  41,  41, 155,  40,  40,  40,  40,
     41,  32,  32,  32,  32,  32,  32,  32,  16,  32,  32,  32,  32,  32,  32,  32,
     45,  16,  16,  16,  34,  34,  34,  32,  32,  32,  32,  32,  42, 159,  34, 108,
     32,  32,  44,  44,  44,  44,  44,  44,  32,  32,  32,  32,  32,  48,  44,  44,
     44,  44,  44,  44,  40,  35,  36,  36,  36,  70,  36,  70,  36,  69,  36,  36,
     36,  93,  84,  82,  67,  67,  44,  44,  27,  27,  27,  67, 160,  44,  44,  44,
     36,  36,   2,   2,  44,  44,  44,  44,  83,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  83,  83,  83,  83,  83,  83,  83,  83,  79,  44,  44,  44,  44,   2,
     43,  36,  36,  36,   2,  71,  44,  44,  36,  36,  36,  43,  43,  43,  43,   2,
     36,  36,  36,  69,  43,  43,  43,  43,  43,  83,  44,  44,  44,  44,  44,  55,
     36,  69,  83,  43,  43,  83,  82,  83, 161,   2,   2,   2,   2,   2,   2,  53,
      7,   7,   7,   7,   7,  44,  44,   2,  36,  36,  36,  36,  69,  43,  43,  82,
     84,  82,  84,  79,  44,  44,  44,  44,  36,  69,  36,  36,  36,  36,  82,  44,
      7,   7,   7,   7,   7,  44,   2,   2,  68,  36,  36,  76,  67,  93,  44,  44,
     70,  43,  70,  69,  70,  36,  36,  43,  69,  62,  44,  44,  44,  44,  44,  44,
     44,  44,  44,  44,  44,  80, 105,   2,  36,  36,  36,  36,  36,  93,  43,  83,
      2, 105, 162,  79,  44,  44,  44,  44,  80,  36,  36,  62,  80,  36,  36,  62,
     80,  36,  36,  62,  44,  44,  44,  44,  36,  93,  84,  83,  82, 161,  84,  44,
     36,  36,  44,  44,  44,  44,  44,  44,  36,  36,  36,  62,  44,  80,  36,  36,
    163, 163, 163, 163, 163, 163, 163, 163, 164, 164, 164, 164, 164, 164, 164, 164,
     36,  36,  36,  36,  36,  44,  44,  44,  16,  16,  16, 108,  44,  44,  44,  44,
     44,  54,  16,  16,  44,  44,  80,  70,  36,  36,  36,  36, 165,  36,  36,  36,
     36,  36,  36,  62,  36,  36,  62,  62,  36,  80,  62,  36,  36,  36,  36,  36,
     36,  41,  41,  41,  41,  41,  41,  41,  41,  44,  44,  44,  44,  44,  44,  44,
     44,  80,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,   4,
     44,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36, 160,  44,
      2,   2,   2, 166, 126,  44,  44,  44,   6, 167, 168, 144, 144, 144, 144, 144,
    144, 144, 126, 166, 126,   2, 123, 169,   2,  46,   2,   2, 149, 144, 144, 126,
      2, 170,   8, 145,  66,   2,  44,  44,  36,  36,  62,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  62,  78,  55,   2,   3,   2,   4,   5,   6,   2,
     16,  16,  16,  16,  16,  17,  18, 125, 126,   4,   2,  36,  36,  36,  36,  36,
     68,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  40,
     44,  36,  36,  36,  44,  36,  36,  36,  44,  36,  36,  36,  44,  36,  62,  44,
     20, 171,  88, 130,  26,   8, 140,  90,  44,  44,  44,  44,  78,  64,  67,  44,
     36,  36,  36,  36,  36,  36,  80,  36,  36,  36,  36,  36,  36,  62,  36,  80,
      2,  46,  44, 172,  27,  27,  27,  27,  27,  27,  44,  91,  67,  67,  67,  67,
    101, 101, 139,  27,  89,  67,  67,  67,  67,  67,  67,  67,  67,  96,  44,  44,
     67,  67,  67,  67,  67,  67,  51,  44,  27,  27,  44,  44,  44,  44,  44,  44,
    148,  36,  36,  36,  36, 102,  44,  44,  36,  36,  36,  36,  36,  36,  36,  55,
     36,  36,  44,  44,  36,  36,  36,  36, 173, 101, 101,  44,  44,  44,  44,  44,
     11,  11,  11,  11,  16,  16,  16,  16,  36,  36,  36,  44,  62,  36,  36,  36,
     36,  36,  36,  80,  62,  44,  62,  80,  36,  36,  36,  55,  27,  27,  27,  27,
     36,  36,  36,  27,  27,  27,  44,  55,  36,  36,  36,  36,  36,  44,  44,  55,
     36,  36,  36,  36,  44,  44,  44,  36,  69,  43,  58,  79,  44,  44,  43,  43,
     36,  36,  80,  36,  80,  36,  36,  36,  36,  36,  44,  44,  43,  79,  44,  58,
     27,  27,  27,  27,  44,  44,  44,  44,   2,   2,   2,   2,  46,  44,  44,  44,
     36,  36,  36,  36,  36,  36, 174,  30,  36,  36,  36,  44,  55,   2,   2,   2,
     36,  36,  36,  44,  27,  27,  27,  27,  36,  62,  44,  44,  27,  27,  27,  27,
     36,  36,  36,  36,  62,  44,  44,  44,  27,  27,  27,  27,  27,  27,  27,  96,
     84,  94,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  43,  43,  43,  43,
     43,  43,  43,  61,   2,   2,   2,  44,  44,  27,  27,  27,  27,  27,  27,  27,
     27,  27,  27,   7,   7,   7,   7,   7,  83,  84,  43,  82,  84,  61, 175,   2,
      2,  44,  44,  44,  44,  44,  44,  44,  43,  70,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  69,  43,  43,  84,  43,  43,  43,  79,   7,   7,   7,   7,   7,
      2,   2,  44,  44,  44,  44,  44,  44,  36,  93,  83,  43,  43,  43,  43,  82,
     94,  36,  63,   2,  46,  44,  44,  44,  36,  36,  36,  36,  36,  69,  84,  83,
     43,  43,  43,  84,  44,  44,  44,  44, 101, 102,  44,  44,  44,  44,  44,  44,
     93,  83,  83,  83,  83,  83,  83,  83,  83,  83,  83,  83,  83,  83,  83,  87,
     44,  44,  44,  44,  44,  44,  44,  58,  43,  73,  40,  40,  40,  40,  40,  40,
     36,  44,  44,  44,  44,  44,  44,  44,  67,  67,  67,  90,  91,  67,  67,  67,
     67,  67, 176,  84,  43,  67, 176,  83,  83, 177,  64,  64,  64, 178,  43,  43,
     43,  75,  51,  43,  43,  43,  67,  67,  67,  67,  67,  67,  67,  43,  43,  67,
     67,  67,  67,  67,  67,  67,  67,  44,  67,  43,  75,  44,  44,  44,  44,  44,
     27,  44,  44,  44,  44,  44,  44,  44,  11,  11,  11,  11,  11,  16,  16,  16,
     16,  16,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  16,
     16,  16, 108,  16,  16,  16,  16,  16,  11,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  16,  48,  11,  44,  48,  49,  48,  49,  11,  48,  11,
     11,  11,  11,  16,  16,  54,  54,  16,  16,  16,  54,  16,  16,  16,  16,  16,
     16,  16,  11,  49,  11,  48,  49,  11,  11,  11,  48,  11,  11,  11,  48,  16,
     16,  16,  16,  16,  11,  49,  11,  48,  11,  11,  48,  48,  44,  11,  11,  11,
     48,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  11,  11,
     11,  11,  11,  16,  16,  16,  16,  16,  16,  16,  16,  44,  11,  11,  11,  11,
     31,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  33,  16,  16,
     16,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  31,  16,  16,
     16,  16,  33,  16,  16,  16,  11,  11,  11,  11,  31,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  16,  16,  33,  16,  16,  16,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  11,  11,  11,  31,  16,  16,  16,  16,  33,  16,  16,  16,
     11,  11,  11,  11,  31,  16,  16,  16,  16,  33,  16,  16,  16,  32,  44,   7,
      7,   7,   7,   7,   7,   7,   7,   7,  36,  36,  80,  36,  36,  36,  36,  36,
     80,  62,  62,  80,  80,  36,  36,  36,  36,  62,  36,  36,  80,  80,  44,  44,
     44,  62,  44,  80,  80,  80,  80,  36,  80,  62,  62,  80,  80,  80,  80,  80,
     80,  62,  62,  80,  36,  62,  36,  36,  36,  62,  36,  36,  80,  36,  62,  62,
     36,  36,  36,  36,  36,  80,  36,  36,  80,  36,  80,  36,  36,  80,  36,  36,
      8,  44,  44,  44,  44,  44,  44,  44,  91,  67,  67,  67,  67,  67,  67,  90,
     27,  27,  27,  27,  27,  96,  44,  44,  44,  44,  44,  67,  67,  67,  67,  67,
     67,  90,  44,  44,  44,  44,  44,  44,  67,  67,  67,  67,  90,  44,  44,  44,
     67,  44,  44,  44,  44,  44,  44,  44,  90,  44,  44,  44,  44,  44,  44,  44,
     67,  67,  67,  91,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  90,  44,
     67,  67,  90,  67,  67,  90,  44,  44,  90,  67,  67,  67,  67,  67,  67,  67,
     67,  67,  67,  67,  91,  67,  90,  44,  67,  67,  67,  67,  44,  44,  44,  44,
     44,  44,  44,  44,  44,  91,  67,  67,  90,  44,  91,  67,  67,  67,  67,  67,
     78,  44,  44,  44,  44,  44,  44,  44,  64,  64,  64,  64,  64,  64,  64,  64,
    164, 164, 164, 164, 164, 164, 164,  44,
};

static RE_UINT8 re_general_category_stage_5[] = {
    15, 15, 12, 23, 23, 23, 25, 23, 20, 21, 23, 24, 23, 19,  9,  9,
    24, 24, 24, 23, 23,  1,  1,  1,  1, 20, 23, 21, 26, 22, 26,  2,
     2,  2,  2, 20, 24, 21, 24, 15, 25, 25, 27, 23, 26, 27,  5, 28,
    24, 16, 27, 26, 27, 24, 11, 11, 26, 11,  5, 29, 11, 23,  1, 24,
     1,  2,  2, 24,  2,  1,  2,  5,  5,  5,  1,  3,  3,  2,  5,  2,
     4,  4, 26, 26,  4, 26,  6,  6,  0,  0,  4,  2, 23,  0,  1, 23,
     1,  0,  0,  1, 24,  1, 27,  6,  7,  7,  0,  4,  0,  2,  0, 23,
    19,  0,  0, 25,  0,  6, 19,  6, 23,  6,  6, 23,  5,  0,  5, 23,
    16, 16, 16,  0, 23, 25, 27, 27,  4,  5,  5,  6,  6,  5, 23,  5,
     6, 16,  6,  4,  4,  6,  6, 27,  5, 27, 27,  5,  0, 16,  6,  0,
     0,  5,  4,  0,  6,  8,  8,  8,  8,  6, 23,  4,  0,  8,  8,  0,
    27, 25, 11, 27, 27,  0,  0, 27, 23, 27,  5,  8,  8,  5, 23, 11,
    11,  0, 19,  5, 12,  5,  5, 20, 21,  0, 10, 10, 10,  0, 19, 23,
    12,  0,  5,  4,  2,  4,  3,  3,  2,  0,  3, 26,  2, 26,  0, 26,
     1, 26, 26,  0, 12, 12, 12, 16, 19, 19, 28, 29, 20, 28, 13, 14,
    16, 12, 23, 28, 29, 23, 23, 22, 22, 23, 24, 20, 21, 23, 23, 12,
    11,  4, 21,  4, 25,  0,  6,  7,  7,  6,  1, 27, 27,  1, 27,  2,
     2, 27, 10,  1,  2, 10, 10, 11, 24, 27, 27, 20, 21, 27, 21, 24,
    21, 20, 24,  0,  2,  6, 27,  4,  5, 10, 19, 20, 21, 21, 27, 10,
    19,  4, 10,  4,  6, 26, 26,  4, 27, 11,  4, 23,  7, 23, 26,  1,
    25, 27,  8, 23,  4,  8, 18, 18, 17, 17,  5, 24, 23, 20, 19, 22,
    22, 20, 22, 22, 24, 19, 24, 26,  0, 11, 23, 10,  5, 11, 23, 16,
    27,  8,  8, 16, 16,  6,
};

/* General_Category: 8558 bytes. */

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
    20, 20, 20, 20, 20, 20, 24, 20, 20, 20, 25, 20, 20, 26, 27, 20,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 29, 30, 31, 32, 20, 20, 20, 20, 20, 20, 20, 33,
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
    34, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
    35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
    36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
    36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
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
    115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 119, 119, 119, 119, 119, 119,
    125, 119, 126, 127, 128, 119, 129, 119, 130, 119, 119, 119, 131, 119, 119, 119,
    132, 133, 134, 135, 119, 119, 119, 119, 119, 119, 119, 119, 119, 136, 119, 119,
    119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    137, 137, 137, 137, 137, 137, 137, 137, 138, 119, 119, 119, 119, 119, 119, 119,
    139, 139, 139, 139, 139, 139, 139, 139, 140, 119, 119, 119, 119, 119, 119, 119,
    141, 141, 141, 141, 142, 119, 119, 119, 119, 119, 119, 119, 119, 119, 143, 144,
    145, 145, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    146, 146, 147, 147, 148, 119, 149, 119, 150, 150, 150, 150, 150, 150, 150, 150,
    119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 151, 151, 119, 119,
    152, 153, 154, 154, 155, 155, 156, 156, 156, 156, 156, 156, 157, 158, 159, 119,
    160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
    160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 161, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 163, 164,
    165, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    166, 166, 166, 166, 167, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    168, 119, 169, 170, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
    172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172,
};

static RE_UINT8 re_block_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
      2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,
      3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,
      6,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      8,   8,   8,   8,   8,   8,   8,   8,   9,   9,   9,  10,  10,  10,  10,  10,
     10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
     13,  13,  13,  13,  13,  14,  14,  14,  15,  15,  15,  15,  16,  16,  16,  16,
     17,  17,  17,  17,  18,  18,  19,  19,  19,  19,  20,  20,  20,  20,  20,  20,
     21,  21,  21,  21,  21,  21,  21,  21,  22,  22,  22,  22,  22,  22,  22,  22,
     23,  23,  23,  23,  23,  23,  23,  23,  24,  24,  24,  24,  24,  24,  24,  24,
     25,  25,  25,  25,  25,  25,  25,  25,  26,  26,  26,  26,  26,  26,  26,  26,
     27,  27,  27,  27,  27,  27,  27,  27,  28,  28,  28,  28,  28,  28,  28,  28,
     29,  29,  29,  29,  29,  29,  29,  29,  30,  30,  30,  30,  30,  30,  30,  30,
     31,  31,  31,  31,  31,  31,  31,  31,  32,  32,  32,  32,  32,  32,  32,  32,
     33,  33,  33,  33,  33,  33,  33,  33,  34,  34,  34,  34,  34,  34,  34,  34,
     34,  34,  35,  35,  35,  35,  35,  35,  36,  36,  36,  36,  36,  36,  36,  36,
     37,  37,  37,  37,  37,  37,  37,  37,  38,  38,  39,  39,  39,  39,  39,  39,
     40,  40,  40,  40,  40,  40,  40,  40,  41,  41,  42,  42,  42,  42,  42,  42,
     43,  43,  44,  44,  45,  45,  46,  46,  47,  47,  47,  47,  47,  47,  47,  47,
     48,  48,  48,  48,  48,  48,  48,  48,  48,  48,  48,  49,  49,  49,  49,  49,
     50,  50,  50,  50,  50,  51,  51,  51,  52,  52,  52,  52,  52,  52,  53,  53,
     54,  54,  55,  55,  55,  55,  55,  55,  55,  55,  55,  19,  19,  19,  19,  19,
     56,  56,  56,  56,  56,  56,  56,  56,  57,  57,  57,  57,  58,  58,  58,  58,
     59,  59,  59,  59,  59,  60,  60,  60,  19,  19,  19,  19,  61,  62,  62,  62,
     63,  63,  63,  63,  63,  63,  63,  63,  64,  64,  64,  64,  65,  65,  65,  65,
     66,  66,  66,  66,  66,  66,  66,  66,  67,  67,  67,  67,  67,  67,  67,  67,
     68,  68,  68,  68,  68,  68,  68,  69,  69,  69,  70,  70,  70,  71,  71,  71,
     72,  72,  72,  72,  72,  73,  73,  73,  73,  74,  74,  74,  74,  74,  74,  74,
     75,  75,  75,  75,  75,  75,  75,  75,  76,  76,  76,  76,  76,  76,  76,  76,
     77,  77,  77,  77,  78,  78,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,
     80,  80,  80,  80,  80,  80,  80,  80,  81,  81,  82,  82,  82,  82,  82,  82,
     83,  83,  83,  83,  83,  83,  83,  83,  84,  84,  84,  84,  84,  84,  84,  84,
     84,  84,  84,  84,  85,  85,  85,  86,  87,  87,  87,  87,  87,  87,  87,  87,
     88,  88,  88,  88,  88,  88,  88,  88,  89,  89,  89,  89,  89,  89,  89,  89,
     90,  90,  90,  90,  90,  90,  90,  90,  91,  91,  91,  91,  91,  91,  91,  91,
     92,  92,  92,  92,  92,  92,  93,  93,  94,  94,  94,  94,  94,  94,  94,  94,
     95,  95,  95,  96,  96,  96,  96,  96,  97,  97,  97,  97,  97,  97,  98,  98,
     99,  99,  99,  99,  99,  99,  99,  99, 100, 100, 100, 100, 100, 100, 100, 100,
    101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101,  19, 102,
    103, 103, 103, 103, 104, 104, 104, 104, 104, 104, 105, 105, 105, 105, 105, 105,
    106, 106, 106, 107, 107, 107, 107, 107, 107, 108, 109, 109, 110, 110, 110, 111,
    112, 112, 112, 112, 112, 112, 112, 112, 113, 113, 113, 113, 113, 113, 113, 113,
    114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 115, 115, 115, 115,
    116, 116, 116, 116, 116, 116, 116, 116, 117, 117, 117, 117, 117, 117, 117, 117,
    117, 118, 118, 118, 118, 119, 119, 119, 120, 120, 120, 120, 120, 120, 120, 120,
    120, 120, 120, 120, 121, 121, 121, 121, 121, 121, 122, 122, 122, 122, 122, 122,
    123, 123, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124,
    125, 125, 125, 126, 127, 127, 127, 127, 128, 128, 128, 128, 128, 128, 129, 129,
    130, 130, 130, 131, 131, 131, 132, 132, 133, 133, 133, 133, 133, 133,  19,  19,
    134, 134, 134, 134, 134, 134, 135, 135, 136, 136, 136, 136, 136, 136, 137, 137,
    138, 138, 138,  19,  19,  19,  19,  19,  19,  19,  19,  19, 139, 139, 139, 139,
    140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 141, 141, 141, 141, 141,
    142, 142, 142, 142, 142, 142, 142, 142, 143, 143, 143, 143, 143, 143, 143, 143,
    144, 144, 144, 144, 144, 144, 144, 144, 145, 145, 145, 145, 145, 145, 145, 145,
    146, 146, 146, 146, 146, 146, 146, 146, 147, 147, 147, 147, 147, 148, 148, 148,
    148, 148, 148, 148, 148, 148, 148, 148, 149, 150, 151, 152, 152, 153, 153, 154,
    154, 154, 154, 154, 154, 154, 154, 154, 155, 155, 155, 155, 155, 155, 155, 155,
    155, 155, 155, 155, 155, 155, 155, 156, 157, 157, 157, 157, 157, 157, 157, 157,
    158, 158, 158, 158, 158, 158, 158, 158, 159, 159, 159, 159, 160, 160, 160, 160,
    160, 161, 161, 161, 161, 162, 162, 162,  19,  19,  19,  19,  19,  19,  19,  19,
    163, 163, 164, 164, 164, 164,  19,  19, 165, 165, 165, 166, 166,  19,  19,  19,
    167, 167, 168, 168, 168, 168,  19,  19, 169, 169, 169, 169, 169, 170, 170, 170,
    171, 171, 171,  19,  19,  19,  19,  19, 172, 172, 172, 172, 173, 173,  19,  19,
    174, 174, 175, 175,  19,  19,  19,  19, 176, 176, 177, 177, 177, 177, 177, 177,
    178, 178, 178, 178, 178, 178, 179, 179, 180, 180, 180, 180, 181, 181, 182, 182,
    183, 183, 183, 183, 183,  19,  19,  19,  19,  19,  19,  19,  19,  19, 184, 184,
    185, 185, 185, 185, 185, 185, 185, 185, 186, 186, 186, 186, 186, 187, 187, 187,
    188, 188, 188, 188, 188,  19,  19,  19, 189, 189, 189, 189, 189, 189,  19,  19,
    190, 190, 190, 190, 190,  19,  19,  19, 191, 191, 191, 191, 191, 191, 191, 191,
    192, 192, 192, 192, 192, 192, 192, 192, 193, 193, 193, 193, 193, 193, 193, 193,
    193, 193, 193,  19,  19,  19,  19,  19, 194, 194, 194, 194, 194, 194, 194, 194,
    194, 194, 194, 194,  19,  19,  19,  19, 195, 195, 195, 195, 195, 195, 195, 195,
    195, 195,  19,  19,  19,  19,  19,  19, 196, 196, 196, 196, 196, 196, 196, 196,
    197, 197, 197, 197, 197, 197, 197, 197, 198, 198, 198, 198, 198, 198, 198, 198,
    199, 199, 199, 199, 199,  19,  19,  19, 200, 200, 200, 200, 200, 200, 201, 201,
    202, 202, 202, 202, 202, 202, 202, 202, 203, 203, 203, 203, 203, 203, 203, 203,
    204, 204, 204, 205, 205, 205, 205, 205, 205, 205, 206, 206, 206, 206, 206, 206,
    207, 207, 207, 207, 207, 207, 207, 207, 208, 208, 208, 208, 208, 208, 208, 208,
    209, 209, 209, 209, 209, 209, 209, 209, 210, 210, 210, 210, 210,  19,  19,  19,
    211, 211, 211, 211, 211, 211, 211, 211, 212, 212, 212, 212, 212, 212, 212, 212,
    213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213,  19,  19,
    214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 215, 215, 215, 215,
    215, 215, 215, 215, 215, 215, 215, 215, 215, 215,  19,  19,  19,  19,  19,  19,
    216, 216, 216, 216, 216, 216, 216, 216, 216, 216,  19,  19,  19,  19,  19,  19,
    217, 217, 217, 217, 217, 217, 217, 217, 218, 218, 218, 218, 218, 218, 218, 218,
    218, 218, 218, 218, 218, 218, 218,  19, 219, 219, 219, 219, 219, 219, 219, 219,
    220, 220, 220, 220, 220, 220, 220, 220,
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
    208, 208, 208, 208, 209, 209, 209, 209, 210, 210, 210, 210, 211, 211, 211, 211,
    212, 212, 212, 212, 213, 213, 213, 213, 214, 214, 214, 214, 215, 215, 215, 215,
    216, 216, 216, 216, 217, 217, 217, 217, 218, 218, 218, 218, 219, 219, 219, 219,
    220, 220, 220, 220,
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
    208, 208, 208, 208, 209, 209, 209, 209, 210, 210, 210, 210, 211, 211, 211, 211,
    212, 212, 212, 212, 213, 213, 213, 213, 214, 214, 214, 214, 215, 215, 215, 215,
    216, 216, 216, 216, 217, 217, 217, 217, 218, 218, 218, 218, 219, 219, 219, 219,
    220, 220, 220, 220,
};

/* Block: 4288 bytes. */

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
    14, 14, 14, 14, 14, 14, 22, 14, 14, 14, 23, 14, 14, 24, 25, 14,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 26,  7, 27, 28, 14, 14, 14, 14, 14, 14, 14, 29,
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
    30, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
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
     55,  56,  57,  58,  59,  59,  59,  60,  61,  59,  59,  59,  59,  59,  62,  59,
     63,  63,  59,  59,  59,  59,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,
     74,  75,  76,  77,  78,  79,  80,  59,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  81,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  82,
     83,  83,  83,  83,  83,  83,  83,  83,  83,  84,  85,  85,  86,  87,  88,  89,
     90,  91,  92,  93,  94,  95,  96,  97,  32,  32,  32,  32,  32,  32,  32,  32,
     32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
     32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  98,
     65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,
     65,  65,  72,  72,  99, 100, 101, 102, 103, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113,  65, 114, 115, 116, 117, 118,  65,  65,  65,  65,  65,  65,
    119,  65, 120, 121, 122,  65, 123,  65, 124,  65,  65,  65, 125,  65,  65,  65,
    126, 127, 128, 129,  65,  65,  65,  65,  65,  65,  65,  65,  65, 130,  65,  65,
    131, 131, 131, 131, 131, 131, 132,  65, 133,  65,  65,  65,  65,  65,  65,  65,
    134, 134, 134, 134, 134, 134, 134, 134, 135,  65,  65,  65,  65,  65,  65,  65,
    136, 136, 136, 136, 137,  65,  65,  65,  65,  65,  65,  65,  65,  65, 138, 139,
    140,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,
     59, 141, 142, 143, 144,  65, 145,  65, 146, 147, 148,  59,  59, 149,  59, 150,
     65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65, 151, 152,  65,  65,
    153, 154, 155, 156, 157,  65, 158, 159, 160, 161, 162, 163, 164, 165,  60,  65,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72, 166,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72, 167,  72,
    168,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,
     72,  72,  72,  72, 168,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,
    169,  65, 170, 171,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,
};

static RE_UINT16 re_script_stage_3[] = {
      0,   0,   0,   0,   1,   2,   1,   2,   0,   0,   3,   3,   4,   5,   4,   5,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   6,   0,   0,   7,   0,
      8,   8,   8,   8,   8,   8,   8,   9,  10,  11,  12,  11,  11,  11,  13,  11,
     14,  14,  14,  14,  14,  14,  14,  14,  15,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  16,  17,  18,  19,  17,  18,  20,  21,  22,  22,  23,  22,  24,  25,
     26,  27,  28,  28,  29,  30,  31,  32,  28,  28,  28,  28,  28,  33,  28,  28,
     34,  35,  35,  35,  36,  28,  28,  28,  37,  37,  37,  38,  39,  39,  39,  40,
     41,  41,  42,  43,  44,  45,  46,  46,  46,  46,  47,  46,  46,  46,  48,  49,
     50,  50,  50,  50,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,
     62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,
     78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,
     94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,  46, 124,
    125, 126, 126, 127, 126, 128,  46,  46, 129, 130, 131, 132, 133, 134,  46,  46,
    135, 135, 135, 135, 136, 135, 137, 138, 135, 136, 135, 139, 139, 140,  46,  46,
    141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 142, 142, 143, 142, 142, 144,
    145, 145, 145, 145, 145, 145, 145, 145, 146, 146, 146, 146, 147, 148, 146, 146,
    147, 146, 146, 149, 150, 151, 146, 146, 146, 150, 146, 146, 146, 152, 146, 153,
    146, 154, 155, 155, 155, 155, 155, 156, 157, 157, 157, 157, 157, 157, 157, 157,
    158, 159, 160, 160, 160, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
    171, 171, 171, 171, 171, 172, 173, 173, 174, 175, 176, 176, 176, 176, 176, 177,
    176, 176, 178, 157, 157, 157, 157, 179, 180, 181, 182, 182, 183, 184, 185, 186,
    187, 187, 188, 187, 189, 190, 171, 171, 191, 192, 193, 193, 193, 194, 193, 195,
    196, 196, 197,  46,  46,  46,  46,  46, 198, 198, 198, 198, 199, 198, 198, 200,
    201, 201, 201, 201, 202, 202, 202, 203, 204, 204, 204, 205, 206, 207, 207, 207,
     46,  46,  46,  46, 208, 209, 210, 211,   4,   4, 212,   4,   4, 213, 214, 215,
      4,   4,   4, 216,   8,   8, 217, 218,  11, 219,  11,  11, 219, 220,  11, 221,
     11,  11,  11, 222, 222, 223,  11, 224, 225,   0,   0,   0,   0,   0, 226, 227,
    228, 229,   0, 230,  46,   8,   8, 231,   0,   0, 232, 233, 234,   0,   4,   4,
    235,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 236,   0,   0, 237,  46, 230,  46,   0,   0,
    238,   0,   0,   0,   0,   0,   0,   0, 239, 239, 239, 239, 239, 239, 239, 239,
      0,   0,   0,   0, 240, 241,  46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    242, 242, 243, 242, 242, 243,   4,   4, 244, 244, 244, 244, 244, 244, 244, 245,
    142, 142, 143, 246, 246, 246, 247, 248, 146, 249, 250, 250, 250, 250,  14,  14,
      0,   0,   0, 251,  46,  46,  46,  46, 252, 253, 252, 252, 252, 252, 252, 254,
    252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 255,  46, 251,
    256,   0, 257, 258, 259, 260, 260, 260, 260, 261, 262, 263, 263, 263, 263, 264,
    265, 266, 267, 268, 145, 145, 145, 145, 269,   0, 266, 270,   0,   0, 236, 263,
    145, 269,   0,   0,   0,   0, 145, 271,   0,   0,   0,   0,   0, 263, 263, 272,
    263, 263, 263, 263, 263, 273,   0,   0, 252, 252, 252, 255,   0,   0,   0,   0,
    252, 252, 252, 252, 274,  46,  46,  46, 275, 275, 275, 275, 275, 275, 275, 275,
    276, 275, 275, 275, 277, 278, 278, 278, 279, 279, 279, 279, 279, 279, 279, 279,
    279, 279, 280,  46,  14,  14,  14,  14,  14, 281, 282, 282, 282, 282, 282, 283,
      0,   0, 284,   4,   4,   4,   4,   4, 285, 286, 287,  46,  46,  46,  46, 288,
    289, 289, 290, 241, 291, 291, 291, 292, 293, 293, 293, 293, 294, 295,  50, 296,
    297, 297, 297, 298, 298, 299, 145, 300, 301, 301, 301, 301, 302, 303,  46,  46,
    304, 304, 304, 305, 306, 307, 141, 308, 309, 309, 309, 309, 310, 311, 312, 313,
    314, 315, 250,  46,  46,  46,  46,  46,  46,  46,  46,  46, 312, 312, 316, 317,
    145, 145, 318, 145, 319, 145, 145, 320, 252, 252, 252, 252, 252, 252, 321, 252,
    252, 252, 252, 252, 252, 322,  46,  46, 323, 324,  22, 325, 326,  28,  28,  28,
     28,  28,  28,  28, 327, 328,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,
     28,  28,  28, 329,  46,  28,  28,  28,  28, 330,  28,  28, 331,  46,  46, 332,
      8, 241, 217,   0,   0, 333, 334, 335,  28,  28,  28,  28,  28,  28,  28, 336,
    238,   0,   1,   2,   1,   2, 337, 262, 263, 338, 145, 269, 339, 340, 341, 342,
    343, 344, 345, 346, 347, 347,  46,  46, 344, 344, 344, 344, 344, 344, 344, 348,
    349,   0,   0, 350,  11,  11,  11,  11, 351, 251,  46,  46,  46,   0,   0, 352,
    353, 354, 355, 355, 355, 356,  46,  46, 357, 358, 359, 360, 361,  46,  46,  46,
    362, 363, 364, 364, 365, 366,  46,  46, 367, 367, 367, 367, 367, 368, 368, 368,
    369, 370, 371,  46,  46,  46,  46,  46, 372, 373, 373, 374, 375, 376,  46,  46,
    377, 378, 379, 380,  46,  46,  46,  46, 381, 381, 382, 383,  46,  46,  46,  46,
    384, 385, 386, 387, 388, 389, 390, 390, 391, 391, 391, 392, 393, 394, 395, 396,
    397, 397, 397, 397, 398,  46,  46,  46,  46,  46,  46,  46,  46,  46,  28,  49,
    399, 399, 399, 399, 400, 401, 399,  46, 402, 402, 402, 402, 403, 404, 405, 406,
    407, 407, 407, 408, 409,  46,  46,  46, 410, 410, 410, 410, 411, 412,  46,  46,
    413, 413, 413, 414, 415,  46,  46,  46, 416, 416, 416, 416, 416, 416, 416, 416,
    416, 416, 416, 416, 416, 416, 417,  46, 416, 416, 416, 416, 416, 416, 418, 419,
    420, 420, 420, 420, 420, 420, 420, 420, 420, 420, 421,  46,  46,  46,  46,  46,
    282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 422,  46,  46,  46,  46,
    423, 423, 423, 423, 424, 423, 423, 425, 426, 423,  46,  46,  46,  46,  46,  46,
    427,  46,  46,  46,  46,  46,  46,  46,   0,   0,   0,   0,   0,   0,   0, 428,
      0,   0, 429,   0,   0,   0, 430, 431, 432,   0, 433,   0,   0, 434,  46,  46,
     11,  11,  11,  11, 435,  46,  46,  46,   0,   0,   0,   0,   0, 237,   0, 436,
      0,   0,   0,   0,   0, 437,   0,   0,   0, 438, 439, 440, 441,   0,   0,   0,
    442, 443,   0, 444, 445, 446,   0,   0,   0,   0, 447,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 448,   0,   0,   0, 449,  28, 450, 451, 452, 453, 454, 455,
    456, 457, 458, 457,  46,  46,  46, 327,   0,   0, 251,   0,   0,   0,   0,   0,
      0, 236, 228, 459, 238, 238,  46,  46, 230,   0, 228,   0,   0,   0, 251,   0,
      0, 230,  46,  46,  46,  46, 460,   0, 461,   0,   0, 230, 462, 436,  46,  46,
      0,   0, 463, 464,   0,   0,   0, 240,   0, 236,   0,   0, 465,  46,   0, 463,
      0,   0,   0, 228, 446,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 466,
      0,   0,   0, 434, 236,   0, 467,  46,  46,  46,  46,  46,  46,  46,  46, 468,
      0,   0,   0,   0, 469,  46,  46,  46,   0,   0,   0,   0, 428,  46,  46,  46,
    252, 252, 252, 252, 252, 470,  46,  46, 252, 252, 252, 471, 252, 252, 252, 252,
    252, 321,  46,  46,  46,  46,  46,  46, 472,  46,   0,   0,   0,   0,   0,   0,
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
     18,  18,  18,  18,  20,  19,   8,  17,  21,  22,  22,  22,  22,  22,  22,  22,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,   8,   8,   8,   8,
     22,  22,  22,  22,  22,  23,   8,   8,  22,  22,  23,   8,   8,   8,   8,   8,
     24,  24,  25,  24,  24,  24,  26,  24,  24,  24,  24,  24,  24,  27,   8,  27,
     24,  24,  24,  24,  24,  24,  24,  24,  26,  24,  24,  24,  24,  28,   5,   5,
      5,   5,   5,  24,  24,  24,  24,  24,   0,   0,   0,   0,   0,  24,  24,  24,
     29,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  27,  24,
     30,  30,  30,  30,  30,  30,  30,  31,  30,  30,  30,  30,  30,  30,  30,  30,
     30,  30,  30,  30,  30,  32,  31,  30,  33,  33,  33,  33,  33,  33,  33,  33,
     33,   8,   8,   8,   8,   8,   8,   8,  34,  34,  34,  34,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  35,   8,   8,  36,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36,   8,  36,  36,  36,  36,  36,  36,  36,  37,
     38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,   8,  39,
      8,   8,   8,   8,   8,   8,   8,   8,  25,  24,  24,  24,  24,  24,  25,   8,
      8,   8,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  25,
     40,  40,  40,  40,  40,  40,  40,  40,  41,  42,  40,  40,  40,  40,  40,  40,
     40,  40,   0,  40,  40,  40,  40,  40,  40,  40,  40,  40,  43,  40,  40,  40,
     44,  45,  44,  45,  45,  45,  46,  44,  46,  44,  45,  45,  45,  45,  45,  45,
     45,  45,  45,  45,  46,  45,  45,  45,  46,  46,   8,  45,  45,   8,  45,  45,
     45,  45,  46,  44,  46,  44,  45,  46,   8,   8,   8,  44,   8,   8,  45,  44,
     45,  45,   8,  45,  45,  45,  45,  45,  45,  45,  45,  45,  45,  45,   8,   8,
     47,  48,  47,  48,  48,  49,   8,  47,  49,  47,  48,  48,  48,  48,  48,  48,
     48,  48,  48,  48,  49,  48,  48,  48,  49,  48,  47,  49,  48,   8,  49,  48,
     48,  49,   8,  47,  49,  47,  48,   8,  47,   8,   8,   8,  47,  48,  49,  49,
      8,   8,   8,  48,  48,  48,  48,  48,  48,  48,  48,   8,   8,   8,   8,   8,
     50,  51,  50,  51,  51,  51,  51,  50,  51,  50,  51,  51,  51,  51,  51,  51,
     51,  51,  51,  51,  52,  51,  51,  51,  52,  51,  50,  51,  51,   8,  51,  51,
     51,  51,  51,  50,  51,  50,  51,   8,  52,   8,   8,   8,   8,   8,   8,   8,
     51,  51,   8,  51,  51,  51,  51,  51,  51,   8,   8,   8,   8,   8,   8,   8,
     53,  54,  53,  54,  54,  54,  55,  53,  55,  53,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  55,  54,  54,  54,  55,  54,  53,  54,  54,   8,  54,  54,
     54,  54,  55,  53,  55,  53,  54,   8,   8,   8,   8,  54,   8,   8,  54,  53,
     54,  54,   8,  54,  54,  54,  54,  54,  54,  54,  54,  54,   8,   8,   8,   8,
      8,  56,  57,  56,  56,  58,   8,  56,  58,  56,  56,   8,  57,  58,  58,  56,
      8,  57,  58,   8,  56,  58,   8,  56,  56,  56,  56,  56,  56,   8,   8,  56,
     56,  58,   8,  56,  58,  56,  56,   8,  58,   8,   8,  57,   8,   8,   8,   8,
      8,   8,   8,  56,  56,  56,  56,  56,  56,  56,  56,  56,  56,  58,   8,   8,
     59,  60,  59,  60,  60,  60,  61,  60,  61,  60,  60,  60,  60,  60,  60,  60,
     60,  60,  60,  60,  61,  60,  60,  60,  60,  60,  59,  60,  60,   8,  59,  60,
     60,  60,  61,  60,  61,  60,  60,   8,   8,   8,  59,  61,  60,   8,   8,   8,
     60,  60,   8,  60,  60,  60,  60,  60,   8,   8,   8,   8,  60,  60,  60,  60,
      8,  62,  63,  62,  62,  62,  64,  62,  64,  62,  62,  62,  62,  62,  62,  62,
     62,  62,  62,  62,  64,  62,  62,  62,  62,  62,  63,  62,  62,   8,  62,  62,
     62,  62,  64,  62,  64,  62,  62,   8,   8,   8,  63,  64,   8,   8,   8,  64,
     62,  62,   8,  62,  62,  62,  62,  62,  63,  64,   8,   8,   8,   8,   8,   8,
      8,  65,  66,  65,  65,  65,  67,  65,  67,  65,  65,  65,  65,  65,  65,  65,
     65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  65,  67,  66,  65,
     65,  65,  67,  65,  67,  65,  65,  67,   8,   8,   8,  66,   8,   8,   8,   8,
     65,  65,   8,  65,  65,  65,  65,  65,  65,  65,  65,   8,  66,  65,  65,  65,
      8,  68,  69,  68,  68,  68,  68,  68,  68,  68,  68,  70,   8,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  69,  68,  68,  68,  68,  69,   8,
     68,  68,  68,  70,   8,  70,   8,  69,  68,  68,  70,  70,  68,  68,  68,  68,
      8,  68,  70,   8,   8,   8,   8,   8,  71,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  73,   8,  20,
     72,  72,  72,  72,  72,  72,   8,   8,  74,  75,  75,  74,  75,  75,  74,   8,
      8,   8,  76,  76,  74,  76,  76,  76,  74,  76,  74,  74,   8,  76,  74,  76,
     76,  76,  76,  76,  76,  74,  76,   8,  76,  76,  75,  75,  76,  76,  76,   8,
     76,  76,  76,  76,  76,   8,  76,  76,  77,  77,  77,  77,  77,  77,  77,  77,
     77,  77,  77,  77,  78,  77,  77,  77,  77,  77,  77,  77,  77,  77,  79,   8,
     78,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  79,  77,
     77,  77,  80,   0,  81,  79,   8,   8,  82,  82,  82,  82,  82,  82,  82,  82,
     83,  83,  83,  83,  83,  83,  83,  83,  83,  83,  83,  84,   8,   8,  84,   8,
     83,  83,  83,  83,  83,  85,  83,  83,  86,  86,  86,  86,  86,  86,  86,  86,
     87,  87,  87,  87,  87,  87,  87,  87,  87,  87,  87,  87,  88,  87,  87,   8,
     87,  87,  87,  88,  88,  87,  87,   8,  88,  87,  87,   8,  87,  87,  87,  88,
     88,  87,  87,   8,  87,  87,  87,  87,  87,  87,  87,  88,  87,  87,  87,  87,
     87,  87,  87,  87,  87,  88,  89,  87,  87,  87,  87,  87,  87,  87,  88,   8,
     87,  87,  87,  87,  87,   8,   8,   8,  90,  90,  90,  90,  90,  90,  90,  90,
     90,  90,  91,   8,   8,   8,   8,   8,  92,  92,  92,  92,  92,  92,  92,  92,
     93,  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,  94,   8,
     95,  95,  95,  95,  95,  95,  95,  95,  95,  95,  95,  95,  95,  96,   0,  95,
     97,   8,   8,   8,   8,   8,   8,   8,  98,  98,  98,  98,  98,  98,  99,  98,
     98,  98,  99,   8,   8,   8,   8,   8, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 101,   9,   8,   8,   8,   8, 102, 102, 102, 102, 102, 102, 102, 102,
    102, 102,   8,   8,   8,   8,   8,   8, 103, 103, 103, 103, 103, 103, 104, 103,
    104, 103,   8,   8,   8,   8,   8,   8, 105, 105, 105, 105, 105, 105, 105, 105,
    105, 105, 105, 105, 105, 105, 105,   8, 105, 105, 105, 105, 105,   8,   8,   8,
    106,   0, 107, 106, 106, 106, 106, 108, 106, 106, 106, 106, 106,   8,   8,   8,
    106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106,   8,   8,   8,   8,
    106, 106, 106, 106, 106, 108,   8,   8,  92,  92,  92,   8,   8,   8,   8,   8,
    109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 110,   8,
    109, 109, 109, 109, 109, 109,   8,   8, 110,   8, 109, 109, 109, 109, 109, 109,
    111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111,   8,
    111, 111, 112,   8,   8,   8,   8,   8, 113, 113, 113, 113, 113, 113, 113, 113,
    113, 113, 113, 113, 113, 113,   8,   8, 113, 113, 113, 113, 113,   8,   8,   8,
    113, 113, 113, 113, 113, 114,   8, 113, 115, 115, 115, 115, 115, 115, 115, 115,
    115, 115, 115, 115, 115, 115,   8, 115, 116, 116, 116, 116, 116, 116, 116, 116,
    116, 116, 116, 116, 116, 116, 116, 117, 116, 116, 116, 116, 116, 116, 117, 118,
    116, 116, 116, 116, 116,   8,   8,   8, 116, 116, 116, 116, 116, 116, 116,   8,
    119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,   8,   8,
    119, 119, 119, 119, 119, 119, 120,   8, 121, 121, 121, 121, 121, 121, 121, 121,
    122, 122, 122, 122, 122, 122, 122, 122, 122, 122,   8,   8,   8,   8, 122, 122,
    123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,   8, 124, 123, 123,
    123, 123, 123, 123, 123,   8, 124, 123, 125, 125, 125, 125, 125, 125, 125, 125,
    121, 121, 121, 121,   8,   8,   8,   8,   5, 126,   5,   5,   5,   5,   5,   5,
    126,   5,   5,   5, 126,   0, 127,   0,   0,   0, 126,   9,   8,   8,   8,   8,
      2,   2,   2,   6,   6, 128,   2,   2,   2,   2,   2,   2,   2,   2, 129,   6,
      6,   2,   2,   6,   6, 130,   2,   2,   2,   2,   2,   2, 131,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2, 129,   5,   5,   5, 132,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8,   5,   5,   6,   6,   6,   8,   6,   6,   6,   8,
      6,   6,   6,   6,  12,  12,  12,  12,   6,   6,   6,   6,   6,   6,   6,   8,
      6,   6,  11,   6,   6,   6,   6,   6,   6,   6,   8,   6,   6,   6,  12,   6,
      8,   6,  11,   6,   6,   6,   6,  11,   0,   0,   0,   0,   0,   0,   5,   0,
      0,   0,   9,   8,   8,   0,   0,   0,   1,   8,   0,   0,   0,   0,   0,   1,
      0,   0,   0,   0,   0,   0,   0,   9,   2,   2,   2,   2,   2,   2, 133,   8,
      0,   0,   0,   0,   0,   9,   8,   8, 132,   8,   8,   8,   8,   8,   8,   8,
      0,   0,   0,  10,   0,   2,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   3,   2,   2,   2,   2,   3,   8,   8,   8,
      0,   0,   8,   8,   8,   8,   8,   8,   0,   0,   0,   9,   8,   8,   8,   8,
     20,   0,   0,   0,   0,   0,   0,   0, 134, 134, 134, 134, 134, 134, 134, 134,
      0,   0,   0,   0,   0,   0,   9,   8,   0,   0,   0,   0,   0,   8,   8,   8,
    135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 136,
     13,  13,  13,  13,  13,  13,  13,  13,  13,  13,   8,   8, 137,  13,  13,  13,
    138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,   8,   8,   8, 139,
    140,   8,   8,   8,   8,   8,   8, 139,  87,  87,  87,  88,   8,   8,   8,   8,
     87,  87,  87,  88,  87,  87,  87,  88,   0,   0,   0,   0,   0,   0,   8,   8,
    141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 142, 141, 141,
    141, 141,   8,   8,   8,   8,   8,   8, 141, 141, 141,   8,   8,   8,   8,   8,
      0,   0, 143, 143,   0,   0,   0,   0, 143, 141, 141, 141, 141,   5,   5,  86,
      0,   0,   0,   0, 141, 141,   0,   0, 144, 145, 145, 145, 145, 145, 145, 145,
    145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 146, 147, 126, 148, 145,
    149, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
    150, 150, 150, 150, 150, 151, 149, 150,   8,   8, 152,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   8,
    153,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86, 154,
      4,   4,   4,   4,   4, 155,   8,   8,  86,  86,  86,  86,  86,  86,  86, 156,
    150, 150, 150, 150, 150, 150, 150, 157, 150, 150, 150, 150,   0,   0,   0,   0,
    141, 141, 141, 141, 141, 141, 158,   8, 159, 159, 159, 159, 159, 159, 159, 159,
    159, 159, 159, 159, 159, 159, 160,   8, 159, 159, 159, 160,   8,   8,   8,   8,
    161, 161, 161, 161, 161, 161, 161, 161, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162,   8,   8,  14,  14,  14,  14,   8,   8,   8, 163,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164,   8,   8,   8,   8,
      0,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   0,   1,   2, 133,
      2,   2,   8,   8,   8,   8,   8,   8,   2,   2,   2,   2,   2, 133,   8,   8,
      8,   8,   8,   8,   2,   2,   2,   2, 165, 165, 165, 165, 165, 165, 165, 165,
    165, 165, 165, 165, 165, 165,   8,   8, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166,   8,   8,   8,   8, 167, 167, 167, 167, 167, 167, 167, 167,
    167, 167, 168,   8,   8,   8,   8, 167, 167, 167, 167, 167, 167,   8,   8,   8,
     40,  40,  40,  40,  40,  40,   8,   8, 169, 169, 169, 169, 169, 169, 169, 169,
    170, 170, 170, 170, 170, 170, 170, 170, 170, 170,   8,   8,   8,   8,   8, 171,
     86,  86,  86,  86,  86,  86, 154,   8, 172, 172, 172, 172, 172, 172, 172, 172,
    172, 172, 172, 172, 172, 172, 172, 173, 172, 172, 172, 172, 172,   8,   8, 172,
    174, 174, 174, 174, 174, 174, 174, 174, 174, 174, 174, 175,   8,   8,   8,   8,
    174, 174, 174, 174, 174, 174, 174,   8, 174, 174, 174, 174, 174,   8, 174, 174,
     82,  82,  82,  82,  82,  82,   8,   8, 176, 176, 176, 176, 176, 176, 176, 176,
    176, 177,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8, 178, 176, 176,
    179, 179, 179, 179, 179, 179, 179, 179, 179, 179, 179, 180,   8,   8,   8,   8,
     89,  87,  87,  88,  89,  87,  87,  88,  89,  87,  87,  88,   8,   8,   8,   8,
    179, 179, 179, 179, 179, 179, 179,   8, 179, 179, 179, 179, 179,   8,   8,   8,
     86,  86,   8,   8,   8,   8,   8,   8,  86,  86,  86, 154,   8, 153,  86,  86,
     86,  86,  86,  86,  86,  86,   8,   8, 141, 141, 141, 141, 141, 141, 141,   8,
    141, 141, 141, 141, 141,   8,   8,   8,   2,   2,   2, 133,   8,   8,   8,   8,
      8,  17,  18,  18,   8,   8,  21,  22,  22,  22,  22,  23,  22,  22,  23,  23,
     22,  21,  23,  22,  22,  22,  22,  22,  24,   8,   8,   8,   8,   8,   8,   8,
      8, 181,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,   0,
      8,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,   8,   8,   8,   8,
     24,  24,  24,  24,  24,  24,  27,   8,   0,   9,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   9,   0,   0,   8,   8,  24,  24,  25,  24,  24,  24,  24,  24,
     24,  24,  24,  24,  24,  24,  25,  20,   0,   0,   0, 150, 150, 150, 150, 150,
    150, 150, 150, 150, 150, 150, 150,   0,   8,  86,  86,  86,   8,  86,  86,  86,
      8,  86,  86,  86,   8,  86, 154,   8,   0,   0,   0,   9,   0,   0,   0,   9,
      8,   8,   8,   8,  20,   0,   0,   8, 182, 182, 182, 182, 182, 182, 183, 182,
    182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 184, 182, 182, 182, 182,
    182, 182, 182, 182, 182, 184, 182, 183, 182, 182, 182, 182, 182, 182, 182,   8,
    182, 182, 182, 182, 182, 184,   8,   8,   0,   9,   8,  20,   0,   0,   0,   0,
      0,   0,   8,  20,   0,   0,   0,   0,   6,   6,   6,   6,   6,  11,   8,   8,
      0,   0,   0,   0,   0,   0, 127,   8, 185, 185, 185, 185, 185, 185, 185, 185,
    185, 185, 185, 185, 185, 185, 186,   8, 187, 187, 187, 187, 187, 187, 187, 187,
    188,   8,   8,   8,   8,   8,   8,   8, 189, 189, 189, 189, 189, 189, 189, 189,
    189, 189, 189, 189, 189, 189, 189, 190, 189, 189,   8,   8,   8,   8,   8,   8,
    191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 192,   8,   8,
    193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 194,
    195, 195, 195, 195, 195, 195, 195, 195, 195, 195,   8,   8, 195, 195, 195, 195,
    195, 195, 195,   8,   8,   8,   8,   8, 196, 196, 196, 196, 196, 196, 196, 196,
    197, 197, 197, 197, 197, 197, 197, 197, 198, 198, 198, 198, 198, 198, 198, 198,
    198, 198, 198, 198, 198, 198, 198,   8, 198, 198, 198, 198, 198,   8,   8,   8,
    199, 199, 199,   8, 200, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199,
    199, 199, 199, 201, 200,   8, 200, 201, 202, 202, 202, 202, 202, 202, 202, 202,
    202, 202, 202, 203, 202, 202, 202, 202, 204, 204, 204, 204, 204, 204, 204, 204,
    204, 204, 204, 204, 204, 204,   8, 205, 206, 206, 206, 206, 206, 206, 206, 206,
    206, 206, 206, 206, 206,   8,   8, 207, 208, 208, 208, 208, 208, 208, 208, 208,
    209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209,   8,   8,   8, 209,
    210, 210, 211, 212,   8,   8, 210, 210, 210, 210, 211, 210, 211, 210, 210, 210,
    210, 210, 210, 210, 210, 210, 210, 210, 210, 210,   8,   8, 210, 212,   8, 211,
    210, 210, 210, 210,   8,   8,   8,   8, 210, 210, 210, 210, 212,   8,   8,   8,
    213, 213, 213, 213, 213, 213, 213, 213, 214, 214, 214, 214, 214, 214, 214, 214,
    214, 214, 214,   8, 215, 214, 214, 214, 216, 216, 216, 216, 216, 216, 216, 216,
    216, 216, 216,   8, 216, 216, 216, 216, 217, 217, 217, 217, 217, 217, 217, 217,
    217, 218,   8,   8, 217, 217, 217, 217, 219, 219, 219, 219, 219, 219, 219, 219,
    219, 219, 219, 219, 220,   8,   8,   8, 221, 221, 221, 221, 221, 221, 221, 221,
    221, 221, 221, 221, 221, 221, 221,   8,   8, 221, 221, 221, 221, 221, 221, 221,
    222, 222, 222, 222, 222, 222, 222, 222, 222,   8,   8,   8,   8,   8,   8,   8,
    223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 224,   8,   8,   8,
    223, 223, 223, 223, 223,   8,   8,   8, 225, 225, 225, 225, 225, 225, 225, 225,
    225, 225, 226, 225, 225, 225, 225, 225, 225, 225,   8,   8,   8,   8,   8,   8,
    227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 228,   8,   8,   8,
    227, 227, 227, 227, 227,   8,   8,   8, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229,   8,   8,   8,   8, 229, 229, 229, 229, 229,   8,   8,   8,
    230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 231,
    230, 231,   8,   8,   8,   8,   8,   8, 230, 230,   8,   8,   8,   8,   8,   8,
    232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 233,
    164, 164, 164, 164, 234,   8,   8,   8, 235, 235, 235, 235, 235, 235, 235, 235,
    235, 235, 236,   8,   8,   8,   8,   8, 235, 235, 235, 235, 235, 235, 235, 236,
      8,   8,   8,   8,   8,   8,   8, 237, 238,   8,   8,   8,   8,   8,   8,   8,
      0,   0,   0,   8,   8,   8,   8,   8,   0,   0,   0,   9,  20,   0,   0,   0,
      0,   0,   0, 127,   5,   0,   0,   0,   0,   0,   0,   0,   0, 127,   5,   5,
      5, 126, 127,   5,   5,   5,   0,   0,   0,   0,   0,   0,   0,   5,   5,   0,
      0,   0,   0,   0,   0,   0,   0,   8,   6,   6,   6,   8,   8,   8,   8,   8,
      0,   8,   8,   8,   8,   8,   8,   8,   0,   0,   9,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   9,   0,   8,   9,  20,   9,  20,   0,   9,   0,
      0,   0,   0,   0,   0,  20,  20,   0,   0,   0,  20,   0,   0,   0,   0,   0,
      0,   0,   0,  20,   0,   9,  20,   0,   0,   0,   9,   0,   0,   0,   9,   0,
      0,   0,   0,   0,   0,  20,   0,   9,   0,   0,   9,   9,   8,   0,   0,   0,
      9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   8,   0,  24,  24, 181,  24,  24,  24,  24,  24,
    181,  25,  25, 181, 181,  24,  24,  24,  24,  25,  24,  24, 181, 181,   8,   8,
      8,  25,   8, 181, 181, 181, 181,  24, 181,  25,  25, 181, 181, 181, 181, 181,
    181,  25,  25, 181,  24,  25,  24,  24,  24,  25,  24,  24, 181,  24,  25,  25,
     24,  24,  24,  24,  24, 181,  24,  24,  24,  24,  24,  24,  24,  24,   8,   8,
    181,  24, 181,  24,  24, 181,  24,  24,  20,   0,   0,   0,   0,   0,   0,   9,
      8,   8,   8,   0,   0,   0,   0,   0, 239,   9,   8,   8,   8,   8,   8,   8,
      0,   0,   0,   0,   9,   8,   8,   8,   9,   8,   8,   8,   8,   8,   8,   8,
      0,   0,   0,  20,   0,   0,   0,   0,   0,   0,   9,   0,   0,   9,   8,   8,
      0,   0,   0,   0,  20,   0,   9,   8,   0,   0,   0,   0,   8,   8,   8,   8,
      8,   8,   8,   8,   8,  20,   0,   0,   9,   8,  20,   0,   0,   0,   0,   0,
    141, 141, 141, 158,   8,   8,   8,   8, 141, 141, 158,   8,   8,   8,   8,   8,
     20,   8,   8,   8,   8,   8,   8,   8,
};

static RE_UINT8 re_script_stage_5[] = {
      1,   1,   1,   2,   2,   2,   2,   1,  35,  35,  41,  41,   3,   3,   1,   3,
      0,   0,   1,   0,   3,   1,   3,   0,   0,   3,  55,  55,   4,   4,   4,  41,
     41,   4,   0,   5,   5,   5,   5,   0,   0,   1,   0,   6,   6,   6,   6,   0,
      7,   7,   7,   0,   1,   7,   7,   1,   7,  41,  41,   7,   8,   8,   0,   8,
      8,   0,   9,   9,  66,  66,  66,   0,  82,  82,  82,   0,  95,  95,  95,   0,
     10,  10,  10,  41,  41,  10,   0,  10,   0,  11,  11,  11,  11,   0,   0,  12,
     12,  12,  12,   0,   0,  13,  13,  13,  13,   0,   0,  14,  14,  14,  14,   0,
     15,  15,   0,  15,  15,   0,   0,  16,  16,  16,  16,   0,  17,  17,   0,  17,
     17,   0,  18,  18,   0,  18,  18,   0,  19,  19,   0,  19,  19,   0,   0,  20,
     20,  20,  20,   0,   0,  21,  21,   0,  21,  21,  22,  22,   0,  22,  22,   0,
     22,   1,   1,  22,  23,  23,  24,  24,   0,  24,  24,   1,  25,  25,  26,  26,
     26,   0,   0,  26,  27,  27,  27,   0,  28,  28,  29,  29,  29,   0,  30,  30,
     30,   1,  30,   0,  42,  42,  42,   0,  43,  43,  43,   1,  44,  44,  45,  45,
     45,   0,  31,  31,  32,  32,  32,   1,  32,   0,  46,  46,  46,   0,  47,  47,
     47,   0,  56,  56,  56,   0,  54,  54,  78,  78,  78,   0,   0,  78,  62,  62,
     62,   0,  67,  67,  93,  93,  68,  68,   0,  68,  69,  69,  41,   1,   1,  41,
      3,   4,   2,   3,   3,   2,   4,   2,  41,   0,   2,   0,  53,  53,  57,  57,
     57,   0,   0,  55,  58,  58,   0,  58,  58,   0,  36,  36,   0,  36,   1,  36,
      0,  33,  33,  33,  33,   0,   0,  41,   1,  33,   1,  34,  34,  34,  34,   1,
      0,  35,   0,  25,  25,   0,  35,   0,  25,   1,  34,   0,  36,   0,  37,  37,
     37,   0,  83,  83,  70,  70,   0,   4,  84,  84,  59,  59,  65,  65,  71,  71,
     71,   0,  72,  72,  73,  73,   0,  73,  85,  85,   0,  85,  77,  77,  77,   0,
     79,  79,  79,   0,   0,  79,  86,  86,  86,   0,   0,   7,  48,  48,   0,  48,
     48,   0,  74,  74,  74,   0,  75,  75,  75,   0,  38,  38,  38,   0,  39,  39,
     39,   0,  49,  49,   0,  49,  60,  60,  40,  40,  50,  50,  51,  51,  52,  52,
     52,   0,   0,  52,  87,  87,   0,  87,  64,  64,   0,  64,  76,  76,   0,  76,
     98,  98,  97,  97,  61,  61,   0,  61,  61,   0,  88,  88,  80,  80,   0,  80,
     89,  89,  90,  90,  90,   0,  91,  91,  91,   0,  94,  94,  92,  92, 101, 101,
    101,   0,  96,  96,  96,   0, 100, 100, 100,   0, 102, 102,  63,  63,  63,   0,
     81,  81,  81,   0,  84,   0,  99,  99,  99,   0,   0,  99,  34,  33,  33,   1,
};

/* Script: 8056 bytes. */

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
     9, 10, 11, 12,  4,  4, 13,  4,  4,  4,  4, 14,  4, 15, 16, 17,
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
    18,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
};

static RE_UINT8 re_word_break_stage_2[] = {
      0,   1,   2,   2,   2,   3,   4,   5,   2,   6,   7,   8,   9,  10,  11,  12,
     13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
     29,  30,   2,   2,  31,  32,  33,  34,  35,   2,   2,   2,  36,  37,  38,  39,
     40,  41,  42,  43,  44,  45,  46,  47,  48,  49,   2,  50,   2,   2,  51,  52,
     53,  54,  55,  56,  57,  57,  57,  57,  57,  58,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  59,  60,  61,  62,  63,  57,  57,  57,
     64,  65,  66,  67,  57,  68,  69,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
      2,   2,   2,   2,   2,   2,   2,   2,   2,  70,   2,   2,  71,  72,  73,  74,
     75,  76,  77,  78,  79,  80,  81,  82,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  83,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  84,  85,   2,   2,  86,  87,  88,  89,  90,  91,
     92,  93,  94,  95,  57,  96,  97,  98,   2,  99,  57,  57,  57,  57,  57,  57,
    100,  57, 101, 102, 103,  57, 104,  57, 105,  57,  57,  57,  57,  57,  57,  57,
    106, 107, 108, 109,  57,  57,  57,  57,  57,  57,  57,  57,  57, 110,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
      2,   2,   2,   2,   2,   2, 111,  57, 112,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
      2,   2,   2,   2,   2,   2,   2,   2, 113,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
      2,   2,   2,   2, 114,  57,  57,  57,  57,  57,  57,  57,  57,  57, 115, 116,
    117,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57, 118, 119, 120,  57,  57,  57, 121, 122, 123,   2,   2, 124, 125, 126,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57, 127, 128,  57,  57,
     57,  57,  57, 129,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
    130,  57, 131, 132,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
     57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,  57,
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
      0,   0,   0,   0,  57,  17,   0,   0,   0,   0,   0,   0,  58,  19,  19,  59,
     60,   7,   7,   7,   7,   7,   7,  61,  19,  19,  62,   7,  63,   4,   6,   6,
     64,  65,  66,   7,   7,  57,  67,  68,  69,  70,  71,  72,  63,   4,  16,   0,
     64,  73,  66,   7,   7,  57,  74,  75,  76,  77,  78,  79,  80,   4,  81,   0,
     64,  25,  24,   7,   7,  57,  82,  68,  31,  83,  84,   0,  63,   4,   0,   0,
     64,  65,  66,   7,   7,  57,  82,  68,  69,  77,  85,  72,  63,   4,  28,   0,
     86,  87,  88,  89,  90,  87,   7,  91,  92,  93,  94,   0,  80,   4,   0,   0,
     64,  20,  57,   7,   7,  57,  95,  96,  97,  93,  98,  16,  63,   4,   0,   0,
     99,  20,  57,   7,   7,  57,  95,  68,  97,  93,  98, 100,  63,   4, 101,   0,
     99,  20,  57,   7,   7,   7,   7, 102,  97, 103,  71,   0,  63,   4,   0, 104,
     99,   7,  14, 104,   7,   7,  24, 105,  14, 106, 107,  19,   0,   0, 108,   0,
      0,   0,   0,   0,   0,   0, 109,  36,  71,  59,   4, 110,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 109, 111,   0, 112,   4, 110,   0,   0,   0,   0,
     84,   0,   0, 113,   4, 110, 114, 115,   7,   6,   7,   7,   7,  17,  30,  19,
     97, 116,  19,  30,  19,  19,  19, 117, 118,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 119,  19,  59,   4, 110,  85, 120, 121, 112, 122,   0,
    123,  31,   4, 124,   7,   7,   7,   7,  25, 125,   7,   7,   7,   7,   7, 126,
      7,   7,   7,   7,   7,   7,   7,   7,   7,  88,  14,  88,   7,   7,   7,   7,
      7,  88,   7,   7,   7,   7,  88,  14,  88,   7,  14,   7,   7,   7,   7,   7,
      7,   7,  88,   7,   7,   7,   7,   7,   7,   7,   7, 127,   0,   0,   0,   0,
      7,   7,   0,   0,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  17,   0,
      6,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  65,   7,   7,
      6,   7,   7,   9,   7,   7,   7,   7,   7,   7,   7,   7,   7,  87,  84,   0,
      7,  20, 128,   0,   7,   7, 128,   0,   7,   7, 129,   0,   7,  20, 130,   0,
      0,   0,   0,   0,   0,   0,  58,  19,  19,  19, 131, 132,   4, 110,   0,   0,
      0, 133,   4, 110,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   0,
      7,   7,   7,   7,   7, 134,   7,   7,   7,   7,   7,   7,   7,   7, 135,   0,
      7,   7,   7,  17,  19, 131,  19, 131,  80,   4,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  19,  19, 136, 113,   4, 110,   0,   0,   0,   0,
      7,   7, 137, 131,   0,   0,   0,   0,   0,   0, 138,  59,  19,  19,  19,  69,
      4, 110,   4, 110,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    139,   7,   7,   7,   7,   7, 140,  19, 139, 141,   4, 110,   0, 119, 131,   0,
    142,   7,   7,   7,  62, 143,   4,  50,   7,   7,   7,   7,  48,  19, 131,   0,
      7,   7,   7,   7, 140,  19,  19,   0,   4, 144,   4,  50,   7,   7,   7, 135,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 145,  19,  19, 146, 147,   0,
      7,   7,   7,   7,   7,   7,   7,   7,  19,  19,  19,  19,  59,   0,   0,  58,
      7,   7, 135, 135,   7,   7,   7,   7, 135, 135,   7, 148,   7,   7,   7, 135,
      7,   7,   7,   7,   7,   7,  20, 149, 150,  17, 151, 141,   7,  17, 150,  17,
      0, 152,   0, 153, 154, 155,   0, 156, 157,   0, 158,   0,  34, 159,  28, 160,
      0,   0,   7,  17,   0,   0,   0,   0,   0,   0,  19,  19,  19,  19, 136,   0,
    161, 104, 105, 162,  18, 163,   7, 164, 165, 166,   0,   0,   7,   7,   7,   7,
      7,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 167,   7,   7,   7,   7,   7,   7,  16,   0,   0,
      7,   7,   7,   7,   7,  14,   7,   7,   7,   7,   7,  14,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  17, 168, 169,   0,
      7,   7,   7,   7,  25, 125,   7,   7,   7,   7,   7,   7,   7, 160,   0,  71,
      7,   7,  14,   0,  14,  14,  14,  14,  14,  14,  14,  14,  19,  19,  19,  19,
      0,   0,   0,   0,   0, 160,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    125,   0,   0,   0,   0, 123, 170,  90,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 171, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 173,
    165,   7,   7,   7,   7, 135,   6,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      7,  14,   0,   0,   7,   7,   7,   9,   0,   0,   0,   0,   0,   0, 172, 172,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 172, 172, 172, 172, 172, 174,
    172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172,   0,   0,   0,   0,   0,
      7,  17,   0,   0,   0,   0,   0,   0,   0,   0,   7,   7,   7,   7,   7, 135,
      7,  17,   7,   7,   4, 175,   0,   0,   7,   7,   7,   7,   7, 137, 145, 176,
      7,   7,   7,  71,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7, 113,   0,
      0,   0, 160,   7, 104,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      7, 177, 141,   0,   7,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   7,
    178, 179,   7,   7,  37,   0,   0,   0,   7,   7,   7,   7,   7,   7, 141,   0,
     27,   7,   7,   7,   7,   7, 140,  19, 117,   0,   4, 110,  19,  19,  27, 180,
      4,  50,   7,   7,  48, 112,   7,   7, 137,  19, 131,   0,   7,   7,   7,  17,
     60,   7,   7,   7,   7,   7,  37,  19, 136, 160,   4, 110,   0,   0,   0,   0,
      7,   7,   7,   7,   7,  62,  59,   0, 179, 181,   4, 110,   0,   0,   0, 182,
      0,   0,   0,   0,   0,   0, 121, 183,  78,   0,   0,   0,   7,  37, 184,   0,
    185, 185, 185,   0,  14,  14,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   7,   7,   7,   7,  37, 186,   4, 110,
      7,   7,   7,   7, 141,   0,   7,   7,  14, 187,   7,   7,   7,   7,   7, 141,
     14,   0, 187, 188,   7,  57,  14, 149, 189,   7,   7,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,  16,   0,   0,   0, 187,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   7, 135,   0,   0,   7,   7,   7,   7,   7,   7,
      7,   7, 104,   7,   7,   7,   7,   7,   7,   0,   0,   0,   0,   0,   7, 141,
     19,  19, 190,   0,  59,   0, 191,   0,   0, 192, 193,   0,   0,   0,  20,   7,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7, 194,
      2,   3,   0, 195,   6,   7,   7,   8,   6,   7,   7,   9, 196, 172, 172, 172,
    172, 172, 172, 197,   7,   7,   7,  14, 104, 104, 104, 198,   0,   0,   0, 199,
      7,  95,   7,   7,  14,   7,   7, 200,   7, 135,   7, 135,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   9,
      0,   0,   0,   0,   0,   0,   0,   0,   7,   7,   7,   7,   7,   7,  17,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 132,
      7,   7,   7,  17,   7,   7,   7,   7,   7,   7,  84,   0,   0,   0,   0,   0,
      7,   7,   7,  14,   0,   0,   7,   7,   7,   9,   0,   0,   0,   0,   0,   0,
      7,   7,   7, 135,   7,   7,   7,   7, 141,   7, 162,   0,   0,   0,   0,   0,
      7,   7,   7, 135,   4, 110,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    135,  57,   7,   7,   7,   7,  25, 201,   7,   7, 135,   0,   0,   0,   0,   0,
      7,   7, 135,   0,   7,   7,   7,  16,   0,   0,   0,   0,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7, 167,   0,   0,   0,   0,   0,   0,   0,   0,
    202,  58,  95,   6,   7,   7, 141,  76,   0,   0,   0,   0,   7,   7,   7,  17,
      7,   7,   7,   7,   7,   7, 135,   0,   7,   7, 135,   0,   7,   7,   9,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,  84,   0,   0,   0,   0,   0,   0,
    142,   7,   7,   7,   7,   7,   7,  19,  59,   0,   0,   0,  80,   4,   0,   0,
    142,   7,   7,   7,   7,   7,  19, 203,   0,   0,   7,   7,   7,  84,   4, 110,
    142,   7,   7,   7, 137,  19, 204,   4,   0,   0,   0,   0,   0,   0,   0,   0,
    142,   7,   7,   7,   7,   7,  37,  19, 205,   0,   4, 110,   0,   0,   0,   0,
      7,   7,   7,   7,   7,  37,  19,   0,   4, 110,   0,   0,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  14,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   9,   0,   0,   0,
      7,   7,   7,   7,   7,  14,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,  84,   0,   0,   0,   0,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,  17,   0,  62,  19,  19,  19,  19,  59,
      0,  71, 142,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    206,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 138, 207, 208, 209,
    210, 131,   0,   0,   0, 211,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0, 212,   0,   0,   0,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  20,   7,   7,   7,   7,   7,
      7,   7,   7,  20, 213, 214,   7, 215,  95,   7,   7,   7,   7,   7,   7,   7,
     25, 216,  20,  20,   7,   7,   7, 217, 149, 104,  57,   7,   7,   7,   7,   7,
      7,   7,   7,   7, 135,   7,   7,   7,  57,   7,   7, 126,   7,   7,   7, 126,
      7,   7,  20,   7,   7,   7,  20,   7,   7,  14,   7,   7,   7,  14,   7,   7,
      7,  57,   7,   7,   7,  57,   7,   7, 126, 218,   4,   4,   4,   4,   4,   4,
     95,   7,   7,   7, 219,   6, 126, 220, 161, 221, 219, 148, 219, 126, 126,  79,
      7,  24,   7, 141, 222,  24,   7, 141,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 223, 224, 224, 224,
    225,   0,   0,   0, 226, 226, 226, 226, 226, 226, 226, 226, 226, 226, 226, 226,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,   0,   0,
};

static RE_UINT8 re_word_break_stage_4[] = {
      0,   0,   1,   2,   0,   3,   0,   4,   5,   5,   6,   0,   7,   8,   8,   8,
      9,  10,   9,   0,   0,  11,  12,  13,   0,  14,  12,   0,   8,   9,  15,  16,
     15,   0,   8,  17,   0,  18,  19,  19,   8,  20,  16,  21,   0,  22,   9,  20,
     23,   8,   8,  23,  24,  19,  25,   8,  26,   0,  27,   0,  28,  19,  19,  29,
     30,  29,   8,  31,  32,  33,   0,  34,  35,   0,  36,  19,  37,  38,  39,   8,
      8,  40,  19,  41,  19,  42,  43,  25,  44,  45,   0,  46,  47,   8,  35,   7,
      8,  48,  49,   0,  44,   8,  19,  15,  50,   0,  51,  19,  19,  52,  52,  53,
     52,   0,  20,   8,   0,  19,  19,  35,  19,   8,  48,  54,  52,  19,  48,  55,
     28,   7,   8,  45,  45,   8,  18,  16,  15,  54,  19,  56,  56,  57,   0,  58,
      0,  23,   9,  59,  20,  60,  15,  43,  35,  58,  56,  53,  61,   0,   7,  18,
      0,  55,  25,  62,  20,   7,  29,  53,  17,   0,   0,  63,  64,   7,   9,  16,
     20,  15,  60,  20,  59,  17,  15,  63,  35,  63,  43,  53,  17,  58,   8,   7,
     15,  40,  19,  43,   0,  30,  63,   7,   0,  12,  60,   0,   9,  40,  43,  57,
     16,   8,   8,  26,  65,  58,  19,  66,  63,   0,  61,  19,  67,   0,  29,  68,
     19,  53,  53,   0,   0,  69,  61,  63,   8,  52,  19,  68,   0,  65,  58,  19,
     53,  63,  43,  56,  28,  68,  63,  19,  70,  53,   0,  26,   9,   8,   9,  28,
     48,  68,  48,   0,  71,   0,  19,   0,   0,  61,  58,  53,  72,   0,   8,  15,
     68,   0,   8,  36,   0,  28,  19,  39,   8,  19,   8,   0,  73,   8,  19,  25,
     67,   7,  35,  19,  39,  47,  48,  74,  75,  75,   8,  18,  16,  20,   8,  16,
      0,  76,  77,   0,   0,  78,  79,  80,   0,  10,  81,  82,   0,  81,  83,  32,
      0,  59,  12,  59,   7,  15,  20,  23,  15,   8,   0,   7,  15,  12,   0,  16,
     59,  36,  25,   0,  84,  85,  86,  87,  88,  88,  89,  88,  88,  89,  44,   0,
     19,  90,  45,   9,  91,  91,  36,   8,  59,   0,   8,  53,  58,   0,  68,  63,
     16,  92,   7,   9,  35,  53,  59,   8,   0,  93,  23,  20,  94,  82,  10,  81,
      0,  95,   4,  96,   8,  97,  98,   0,   0,  99,  88, 100,  16,  17, 101,   0,
      9,  23,  17,  45,  52,  30,  35,  13,  19, 102,  39,  17,  87,   0,  53,  28,
    103,  32, 104,  19,  35,  28,  63,  53,  63,  68,  12,  60,   7,  20,  23,   7,
      9,   7,  23,   9,   8,  55,  60,  45,  75,   0,  75,   7,   7,   7,   0, 105,
    106, 106,  13,   0,  32,  32,
};

static RE_UINT8 re_word_break_stage_5[] = {
     0,  0,  0,  0,  0,  0,  2,  3,  3,  1,  0,  0,  0,  0,  0, 11,
    10,  0, 11,  0, 12, 12, 12, 12, 12, 12,  9, 10,  0,  8,  8,  8,
     8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0, 13,  0,  3,  0,  0,
     0,  0,  8,  0,  0,  6,  0,  0,  0,  8,  0,  9,  8,  8,  0,  0,
     0,  0,  8,  8,  8,  0,  0,  0,  8,  0,  8,  0,  4,  4,  4,  4,
     8,  0,  8,  8,  8,  8, 10,  0,  0,  0,  8,  9,  8,  8,  0,  8,
     8,  8,  0,  4,  4,  4,  8,  8,  0,  8,  0,  0,  0, 10,  0,  0,
     0,  4,  4,  4,  4,  4,  0,  4,  0,  4,  4,  0,  9,  0,  0,  0,
     6,  6,  6,  6,  6,  0,  0,  0, 10, 10,  0,  0,  4,  4,  4,  0,
     8,  8,  8,  4, 12, 12,  0, 12, 10,  0,  8,  8,  4,  8,  8,  8,
     0,  8,  4,  4,  4,  6,  0,  4,  4,  8,  8,  4,  4,  0,  4,  4,
    12, 12,  8,  8,  8,  0,  0,  8,  0,  0,  0,  6,  8,  4,  8,  8,
     8,  8,  4,  4,  4,  8,  0,  0, 10,  0,  8,  0,  4,  4,  8,  4,
     8,  4,  4,  4,  4,  4,  0,  0,  4,  8,  4,  4,  0,  0, 12, 12,
     4,  0,  0,  4,  4,  4,  8,  0,  0,  0,  0,  4,  0,  0,  0,  8,
     0,  8,  8,  0,  0,  4,  0,  0,  8,  4,  0,  0,  0,  0,  4,  4,
     0,  0,  4,  8,  0,  0,  4,  0,  4,  0,  4,  0, 12, 12,  0,  0,
     4,  0,  0,  0,  0,  4,  0,  4, 12, 12,  4,  4,  8,  0,  4,  4,
     8,  4,  8,  0,  4,  4,  4,  8,  4,  8,  8,  0,  0,  8,  0,  8,
     4,  4,  6,  6, 11, 11,  0,  0, 11,  0,  0,  9,  3,  3,  6,  6,
     6,  6,  6,  0, 13,  0,  0,  0, 10,  0,  0,  0,  0,  0,  6,  6,
     0,  7,  7,  7,  7,  7,  0,  0,  0,  4,  4,  7,  7,  0,  0,  0,
     7,  7,  7,  7,  7,  7,  7,  0,  4,  4,  0,  8,  8,  8,  4,  8,
     8,  4,  4,  0,  0,  8,  4,  8, 10,  0,  0,  9,  0, 13, 13, 13,
    10,  9,  0,  0,  8,  0,  0,  6,  0,  0,  9, 10,  0,  0,  7,  7,
     7,  7,  4,  4,  0,  6,  6,  6,  4,  0, 12, 12,  4,  4,  4,  6,
     6,  6,  6,  4,  0,  0,  5,  5,  5,  5,  5,  5,
};

/* Word_Break: 3890 bytes. */

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
     0,  1,  2,  2,  2,  3,  4,  5,  6,  2,  2,  7,  2,  2,  8,  9,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    10,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,
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
    35, 36, 37, 38, 39, 40, 34, 41, 42, 42, 42, 42, 42, 42, 42, 42,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 43,  1,  1, 44, 45,
     1, 46,  1,  1,  1,  1,  1,  1,  1,  1, 47,  1,  1,  1,  1,  1,
    48, 49,  1,  1,  1,  1, 50,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 51,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 52, 53,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 54,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    42, 55, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
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
      2,  19,  20,   2,   2,  21,   2,   2,   2,   2,   2,   2,   2,   2,  22,  23,
     24,   2,   2,  25,  26,  27,  28,   2,  29,   2,   2,  30,  31,  32,  28,   2,
     33,   2,   2,  34,  35,  16,   2,  36,  33,   2,   2,  34,  37,   2,  28,   2,
     29,   2,   2,  38,  31,  39,  28,   2,  40,   2,   2,  41,  42,  32,   2,   2,
     43,   2,   2,  44,  45,  46,  28,   2,  47,   2,   2,  48,  49,  46,  28,   2,
     47,   2,   2,  41,  50,  32,  28,   2,  47,   2,   2,   2,  51,  52,   2,  47,
      2,   2,   2,  53,  54,   2,   2,   2,   2,   2,   2,  55,  56,   2,   2,   2,
      2,  57,   2,  58,   2,   2,   2,  59,  60,  61,   5,  62,  63,   2,   2,   2,
      2,   2,  64,  65,   2,  66,  13,  67,  68,  69,   2,   2,   2,   2,   2,   2,
     70,  70,  70,  70,  70,  70,  71,  71,  71,  71,  72,  73,  73,  73,  73,  73,
      2,   2,   2,   2,   2,  64,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,  74,   2,  74,   2,  28,   2,  28,   2,   2,   2,  75,  76,  77,   2,   2,
     78,   2,   2,   2,   2,   2,   2,   2,   2,   2,  79,   2,   2,   2,   2,   2,
      2,   2,  80,  81,   2,   2,   2,   2,   2,   2,   2,  82,   2,   2,   2,   2,
      2,  83,   2,   2,   2,  84,  85,  86,   2,   2,   2,   2,   2,   2,   2,   2,
     87,   2,   2,  88,  89,   2,  12,  18,  90,   2,  91,   2,   2,   2,  92,  93,
      2,   2,  94,  95,   2,   2,   2,   2,   2,   2,   2,   2,   2,  96,  97,  98,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   5,   5,  99, 100,
    101,   2, 102,   2,   2,   2,   1,   2,   2,   2,   2,   2,   2,   5,   5,  13,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 103, 104,
      2,   2,   2,   2,   2,   2,   2, 103,   2,   2,   2,   2,   2,   2,   5,   5,
      2,   2, 105,   2,   2,   2,   2,   2,   2, 106,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2, 103, 107,   2, 103,   2,   2,   2,   2,   2, 104,
    108,   2, 109,   2,   2,   2,   2,   2, 110,   2,   2, 111, 112,   2,   5, 104,
      2,   2, 113,   2, 114,  93,  70, 115,  24,   2,   2, 116, 117,   2,   2,   2,
      2,   2, 118, 119, 120,   2,   2,   2,   2,   2,   2, 121,  16,   2, 122, 123,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 124,   2,
    125, 126, 127, 128, 127, 129, 127, 125, 126, 127, 128, 127, 129, 127, 125, 126,
    127, 128, 127, 129, 127, 125, 126, 127, 128, 127, 129, 127, 125, 126, 127, 128,
    127, 129, 127, 125, 126, 127, 128, 127, 129, 127, 125, 126, 127, 128, 127, 129,
    127, 125, 126, 127, 128, 127, 129, 127, 125, 126, 127, 128, 127, 129, 127, 125,
    126, 127, 128, 127, 129, 127, 125, 126, 127, 128, 127, 129, 127, 125, 126, 127,
    128, 127, 129, 127, 125, 126, 127, 128, 127, 129, 127, 125, 126, 127, 128, 127,
    129, 127, 125, 126, 127, 128, 127, 129, 127, 125, 126, 127, 128, 127, 129, 127,
    127, 128, 127, 129, 127, 125, 126, 127, 128, 127, 130,  71, 131,  73,  73, 132,
      1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      2, 133,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      5,   2,  99,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,
      2,   2,   2,   2,   2,   2,   2,   2,   2,  44,   2,   2,   2,   2,   2, 134,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  69,
    135,   2,   2, 136,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    137,   2,   2, 138,  99,   2,   2,   2,  90,   2,   2, 139,   2,   2,   2,   2,
    140,   2, 141, 142,   2,   2,   2,   2,  90,   2,   2, 143, 117,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 144, 145,   2,   2,   2,   2,
      2,   2,   2,   2,   2, 146, 147, 148, 103, 140,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2, 149, 150, 151,   2, 152,   2,   2,   2,   2,   2,
      2,   2,   2,   2,  74,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 153, 154,
      5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   1,
};

static RE_UINT8 re_grapheme_cluster_break_stage_4[] = {
     0,  0,  1,  2,  0,  0,  0,  0,  3,  3,  3,  3,  3,  3,  3,  4,
     3,  3,  3,  5,  6,  6,  6,  6,  7,  6,  8,  3,  9,  6,  6,  6,
     6,  6,  6, 10, 11, 10,  3,  3,  0, 12,  3,  3,  6,  6, 13,  3,
     3,  3,  7,  6, 14,  3,  3,  3,  3, 15,  6, 16,  6, 17, 18,  8,
    19,  3,  3,  3,  3, 15,  6,  6,  6,  3,  3,  3,  3, 15, 10,  6,
     6,  9,  9,  8,  3,  3,  9,  3,  3,  6,  6,  6,  6,  6,  6, 13,
    20,  3,  3,  3,  3,  3, 21, 22, 23,  6, 24, 25,  9,  6,  3,  3,
    15,  3,  3,  3, 26,  3,  3,  3,  3,  3,  3, 27, 23, 28, 29, 30,
     3,  7,  3,  3, 31,  3,  3,  3,  3,  3,  3, 22, 32,  7, 17,  8,
     8, 19,  3,  3, 23, 10, 33, 30,  3,  3,  3, 18,  3, 15,  3,  3,
    34,  3,  3,  3,  3,  3,  3, 21, 35, 36, 37, 30, 38,  3,  3,  3,
     3,  3,  3, 15, 24, 39, 18,  8,  3, 11,  3,  3, 36,  3,  3,  3,
     3,  3,  3, 40, 41, 42, 37,  8, 23, 22, 37, 30,  3,  3, 34,  7,
    43, 44, 45, 46, 47,  6, 13,  3,  3,  7,  6, 13, 47,  6, 10, 14,
     3,  3,  6,  8,  3,  3,  8,  3,  3, 48, 19, 36,  9,  6,  6, 20,
     6, 18,  3,  9,  6,  6,  9,  6,  6,  6,  6, 14,  3, 34,  3,  3,
     3,  3,  3,  9, 49,  6, 31, 32,  3, 36,  8, 15,  9, 14,  3,  3,
    34, 32,  3, 19,  3,  3,  3, 19, 50, 50, 50, 50, 51, 51, 51, 51,
    51, 51, 52, 52, 52, 52, 52, 52, 15, 14,  3,  3,  3, 53,  6, 54,
    45, 41, 23,  6,  6,  3,  3, 19,  3,  3,  7,  8,  3,  3, 19,  3,
    20, 46, 24,  3, 41, 45, 23,  3,  3, 38, 55,  3,  3,  7, 24,  3,
     3, 56,  6, 13, 44,  9,  6, 24, 46,  6,  6, 17,  6, 57,  3,  3,
     3, 49, 20, 24, 41, 57,  3,  3, 58,  3,  3,  3, 59, 54, 53, 60,
     3, 21, 54, 61, 54,  3,  3,  3,  3, 45, 45,  6,  6, 43,  3,  3,
    13,  6,  6,  6, 49,  6, 14, 19, 36, 14,  3,  3,  6, 13,  3,  3,
     3,  3,  3,  6,  3,  3,  4, 62,  3,  3,  0, 63,  3,  3,  3,  7,
     8,  3,  3,  3,  3,  3, 15,  6,  3,  3, 11,  3, 13,  6,  6,  8,
    34, 34,  7,  3, 64, 65,  3,  3, 60,  3,  3,  3,  3, 45, 45, 45,
    45, 14,  3,  3,  3, 15,  6,  8,  3,  7,  6,  6, 50, 50, 50, 66,
     7, 43, 54, 24, 57,  3,  3,  3,  3,  3,  9, 20, 65, 32,  3,  3,
     7,  3,  3, 67, 18, 17, 14, 15,  3,  3, 64, 54,  3, 68,  3,  3,
    64, 25, 35, 30, 69, 70, 70, 70, 70, 70, 70, 69, 70, 70, 70, 70,
    70, 70, 69, 70, 70, 69, 70, 70, 70,  3,  3,  3, 51, 71, 72, 52,
    52, 52, 52,  3,  3,  3,  3, 34,  0,  0,  0,  3,  9, 11,  3,  6,
     3,  3, 13,  7, 73,  3,  3,  3,  3,  3,  6,  6, 46, 20, 32,  5,
    13,  3,  3,  3,  3,  7,  6, 23,  6, 14,  3,  3, 64, 43,  6, 20,
     3,  3,  7, 25,  6, 53,  3,  3, 38, 45, 45, 45, 45, 45, 45, 45,
    45, 45, 45, 74,  3, 75,  8, 59, 76,  0, 77,  6, 13,  9,  6,  3,
     3,  3, 15,  8,  3, 78, 79, 79, 79, 79, 79, 79,
};

static RE_UINT8 re_grapheme_cluster_break_stage_5[] = {
     3,  3,  3,  3,  3,  3,  2,  3,  3,  1,  3,  3,  0,  0,  0,  0,
     0,  0,  0,  3,  0,  3,  0,  0,  4,  4,  4,  4,  0,  0,  0,  4,
     4,  4,  0,  0,  0,  4,  4,  4,  4,  4,  0,  4,  0,  4,  4,  0,
     3,  0,  0,  0,  4,  4,  4,  0,  4,  0,  0,  0,  0,  0,  4,  4,
     4,  3,  0,  4,  4,  0,  0,  4,  4,  0,  4,  4,  0,  4,  0,  0,
     4,  4,  4,  6,  0,  0,  4,  6,  4,  0,  6,  6,  6,  4,  4,  4,
     4,  6,  6,  6,  6,  4,  6,  6,  0,  4,  6,  6,  4,  0,  4,  6,
     4,  0,  0,  6,  6,  0,  0,  6,  6,  4,  0,  0,  0,  4,  4,  6,
     6,  4,  4,  0,  4,  6,  0,  6,  0,  0,  4,  0,  4,  6,  6,  0,
     0,  0,  6,  6,  6,  0,  6,  6,  0,  6,  6,  6,  6,  0,  4,  4,
     4,  0,  6,  4,  6,  6,  4,  6,  6,  0,  4,  6,  6,  6,  4,  4,
     4,  0,  4,  0,  6,  6,  6,  6,  6,  6,  6,  4,  0,  4,  0,  6,
     0,  4,  0,  4,  4,  6,  4,  4,  7,  7,  7,  7,  8,  8,  8,  8,
     9,  9,  9,  9,  4,  4,  6,  4,  4,  4,  6,  6,  0,  0,  6,  0,
     0,  6,  4,  6,  6,  0,  0,  0,  4,  4,  6,  0,  0,  6,  4,  4,
     6,  6,  0,  0,  6,  4,  6,  4,  4,  4,  3,  3,  3,  3,  3,  0,
     0,  0,  0,  6,  6,  4,  4,  6,  7,  0,  0,  0,  4,  6,  0,  0,
     0,  6,  4,  0, 10, 11, 11, 11, 11, 11, 11, 11,  8,  8,  8,  0,
     0,  0,  0,  9,  6,  4,  6,  0,  6,  6,  6,  0,  0,  4,  6,  4,
     4,  4,  4,  3,  3,  3,  3,  4,  0,  0,  5,  5,  5,  5,  5,  5,
};

/* Grapheme_Cluster_Break: 2324 bytes. */

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
    11, 12, 13, 14,  9,  9, 15,  9,  9,  9,  9, 16,  9, 17, 18,  9,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 19, 20,  9,  9,  9, 21,
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
    22,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
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
    68, 69, 33, 33, 33, 33, 70, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    17, 17, 17, 71, 72, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    17, 17, 17, 17, 73, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 17, 17, 74, 33, 33, 33, 33, 75,
    76, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    33, 77, 78, 33, 79, 80, 81, 82, 33, 33, 33, 33, 33, 33, 33, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 83, 33,
    17, 17, 17, 17, 17, 17, 84, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 85, 86, 33, 33, 33, 33, 33, 33, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 17, 17, 86, 33, 33, 33, 33, 33,
    87, 88, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
};

static RE_UINT16 re_sentence_break_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
      8,  16,  17,  18,  19,  20,  21,  22,  23,  23,  23,  24,  25,  26,  27,  28,
     29,  30,  18,   8,  31,   8,  32,   8,   8,  33,  34,  35,  36,  37,  38,  39,
     40,  41,  42,  43,  41,  41,  44,  45,  46,  47,  48,  41,  41,  49,  50,  51,
     52,  53,  54,  55,  55,  56,  55,  57,  58,  59,  60,  61,  62,  63,  64,  65,
     66,  67,  68,  69,  70,  71,  72,  73,  62,  71,  74,  75,  76,  77,  78,  79,
     80,  81,  82,  73,  83,  84,  85,  86,  83,  87,  88,  89,  90,  91,  92,  93,
     94,  95,  96,  55,  97,  98,  99,  55, 100, 101, 102, 103, 104, 105, 106,  55,
     41, 107, 108, 109, 110,  29, 111, 112,  41,  41,  41,  41,  41,  41,  41,  41,
     41,  41, 113,  41, 114, 115, 116,  41, 117,  41, 118, 119, 120,  41,  41, 121,
     94,  41,  41,  41,  41,  41,  41,  41,  41,  41,  41, 122, 123,  41,  41, 124,
    125, 126, 127, 128,  41, 129, 130, 131, 132,  41,  41, 133,  41, 134,  41, 135,
    136, 137, 138, 139,  41, 140, 141,  55, 142,  41, 143, 144, 145, 146,  55,  55,
    147, 129, 148, 149, 150, 151,  41, 152,  41, 153, 154, 155,  55,  55, 156, 157,
     18,  18,  18,  18,  18,  18,  23, 158,   8,   8,   8,   8, 159,   8,   8,   8,
    160, 161, 162, 163, 161, 164, 165, 166, 167, 168, 169, 170, 171,  55, 172, 173,
    174, 175, 176,  30, 177,  55,  55,  55,  55,  55,  55,  55,  55,  55,  55,  55,
     55, 178,  55,  55,  55,  55,  55,  55,  55,  55,  55,  55,  55, 179,  30, 180,
     55,  55, 181, 182,  55,  55, 183, 184,  55,  55,  55,  55, 185,  55, 186, 187,
     29, 188, 189, 190,   8,   8,   8, 191,  18, 192,  41, 193, 194, 195, 195,  23,
    196, 197,  55,  55,  55,  55,  55,  55, 198, 199,  94,  41, 200,  94,  41, 112,
    201, 202,  41,  41, 203, 204,  55, 205,  41,  41,  41,  41,  41, 135,  55,  55,
     41,  41,  41,  41,  41,  41, 206,  55,  41,  41,  41,  41, 206,  55, 205, 207,
    208, 209,   8, 210, 211,  41,  41, 212, 213, 214,   8, 215, 216, 217,  55, 218,
    219, 220,  41, 221, 222, 129, 223, 224,  50, 225, 226, 136,  58, 227, 228,  55,
     41, 229, 230, 231,  41, 232, 233, 234, 235, 236,  55,  55,  55,  55,  41, 237,
     41,  41,  41,  41,  41, 238, 239, 240,  41,  41,  41, 241,  41,  41, 242,  55,
    243, 244, 245,  41,  41, 246, 247,  41,  41, 248, 205,  41, 249,  41, 250, 251,
    252, 253, 254, 255,  41,  41,  41, 256, 257,   2, 258, 259, 260, 261, 262, 263,
    264, 265, 266,  55,  41,  41,  41, 204,  55,  55,  41, 121,  55,  55,  55, 267,
     55,  55,  55,  55, 136,  41, 268,  55, 261, 205, 269,  55, 270,  41, 271,  55,
     29, 272, 273,  41, 270, 131,  55,  55, 274, 275, 135,  55,  55,  55,  55,  55,
    135, 242,  55,  55,  41, 276,  55,  55, 277, 278, 279, 136,  55,  55,  55,  55,
     41, 135, 135, 280,  55,  55,  55,  55,  41,  41, 281,  55,  55,  55,  55,  55,
    150, 282, 283,  79, 150, 284, 285, 286, 150, 287, 288,  55, 150, 227, 289,  55,
     55,  55,  55,  55,  41, 290, 131,  55,  41,  41,  41, 203,  55,  55,  55,  55,
     41,  41,  41, 291,  55,  55,  55,  55,  41, 203,  55,  55,  55,  55,  55,  55,
     41, 292,  55,  55,  55,  55,  55,  55,  41,  41, 293, 294, 295,  55,  55,  55,
    296,  55,  55,  55,  55,  55,  55,  55,  55,  55,  55, 297, 298, 299,  55,  55,
     55,  55, 300,  55,  55,  55,  55,  55, 301, 302, 303, 304, 305, 306, 307, 308,
    309, 310, 311, 312, 313, 301, 302, 314, 304, 315, 316, 317, 308, 318, 319, 320,
    321, 322, 323, 188, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333,  55,  55,
     41,  41,  41,  41,  41,  41, 194,  55,  41, 121,  41,  41,  41,  41,  41,  41,
    270,  55,  55,  55,  55,  55,  55,  55, 334, 335, 335, 335,  55,  55,  55,  55,
     23,  23,  23,  23,  23,  23,  23, 336,
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
     61,  62,   0,  63,  44,  44,  64,   9,  36,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  65,  44,  44,  44,  44,  44,   7,   7,  66,  67,  68,  36,  36,  36,
     36,  36,  36,  36,  36,  69,  44,  70,  44,  71,  72,  73,   7,   7,  74,  75,
     76,   0,   0,  77,  78,  36,  36,  36,  36,  36,  36,  36,  44,  44,  44,  44,
     44,  44,  64,  79,  36,  36,  36,  36,  36,  80,  44,  44,  81,   0,   0,   0,
      7,   7,  74,  36,  36,  36,  36,  36,  36,  36,  65,  44,  44,  41,  82,   0,
     36,  36,  36,  36,  36,  80,  83,  44,  44,  84,  84,  85,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     86,  36,  36,  87,   0,   0,   0,   0,   0,  44,  44,  44,  44,  44,  44,  64,
     44,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  80,  88,
     44,  44,  44,  44,  84,  44,  36,  36,  80,  89,   7,   7,  79,  36,  79,  36,
     57,  79,  36,  75,  75,  36,  36,  36,  36,  36,  86,  36,  43,  40,  41,  88,
     44,  90,  90,  91,   0,  92,   0,  93,  80,  94,   7,   7,  41,   0,   0,   0,
     57,  79,  60,  95,  75,  36,  36,  36,  36,  36,  86,  36,  86,  96,  41,  72,
     64,  92,  90,  85,  97,   0,  79,  43,   0,  94,   7,   7,  73,  98,   0,   0,
     57,  79,  36,  93,  93,  36,  36,  36,  36,  36,  86,  36,  86,  79,  41,  88,
     44,  58,  58,  85,  87,   0,   0,   0,  80,  94,   7,   7,   0,   0,   0,   0,
     44,  90,  90,  85,   0,  99,   0,  93,  80,  94,   7,   7,  54,   0,   0,   0,
    100,  79,  60,  40,  86,  41,  96,  86,  95,  87,  60,  40,  36,  36,  41,  99,
     64,  99,  72,  85,  87,  92,   0,   0,   0,  94,   7,   7,   0,   0,   0,   0,
     57,  79,  36,  86,  86,  36,  36,  36,  36,  36,  86,  36,  36,  79,  41, 101,
     44,  72,  72,  85,   0,  59,  41,   0,  99,  79,  36,  86,  86,  36,  36,  36,
     36,  36,  86,  36,  36,  79,  41,  88,  44,  72,  72,  85,   0,  59,   0, 102,
     80,  94,   7,   7,  96,   0,   0,   0,  36,  36,  36,  36,  36,  36,  60, 101,
     44,  72,  72,  91,   0,  92,   0,   0,  80,  94,   7,   7,   0,   0,  40,  36,
     99,  79,  36,  36,  36,  60,  40,  36,  36,  36,  36,  36,  93,  36,  36,  54,
     36,  60, 103,  92,  44, 104,  44,  44,   0,   0,   0,   0,  99,   0,   0,   0,
     79,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  78,  44,  64,   0,
     36,  65,  44,  64,   7,   7, 105,   0,  96,  75,  43,  54,   0,  36,  79,  36,
     79, 106,  40,  79,  78,  44,  58,  81,  36,  43,  44,  85,   7,   7, 105,  36,
     87,   0,   0,   0,   0,   0,  85,   0,   7,   7, 105,   0,   0, 107, 108, 109,
     36,  36,  79,  36,  36,  36,  36,  36,  36,  36,  36,  87,  57,  44,  44,  44,
     44,  72,  36,  84,  44,  44,  57,  44,  44,  44,  44,  44,  44,  44,  44, 110,
      0, 103,   0,   0,   0,   0,   0,   0,  36,  36,  65,  44,  44,  44,  44, 111,
      7,   7, 112,   0,  36,  80,  73,  80,  88,  71,  44,  73,  84,  68,  36,  36,
     80,  44,  44,  83,   7,   7, 113,  85,  11,  49,   0, 114,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  60,  36,  36,  36,  86,  41,  36,  60,  86,  41,
     36,  36,  86,  41,  36,  36,  36,  36,  36,  36,  36,  36,  86,  41,  36,  60,
     86,  41,  36,  36,  36,  60,  36,  36,  36,  36,  36,  36,  86,  41,  36,  36,
     36,  36,  36,  36,  36,  36,  60,  57, 115,   9, 116,   0,   0,   0,   0,   0,
     36,  36,  36,  36,   0,   0,   0,   0,  36,  36,  36,  36,  36,  87,   0,   0,
     36,  36,  36, 117,  36,  36,  36,  36, 118,  36,  36,  36,  36,  36, 119, 120,
     36,  36,  60,  40,  87,   0,   0,   0,  36,  36,  36,  86,  80, 110,   0,   0,
     36,  36,  36,  36,  80, 121,   0,   0,  36,  36,  36,  36,  80,   0,   0,   0,
     36,  36,  36,  86, 122,   0,   0,   0,  36,  36,  36,  36,  36,  44,  44,  44,
     44,  44,  44,  44,  44,  95,   0,  98,   7,   7, 105,   0,   0,   0,   0,   0,
    123,   0, 124, 125,   7,   7, 105,   0,  36,  36,  36,  36,  36,  36,   0,   0,
     36,  36, 126,   0,  36,  36,  36,  36,  36,  36,  36,  36,  36,  41,   0,   0,
     36,  36,  36,  36,  36,  36,  36,  87,  44,  44,  44,   0,  44,  44,  44,   0,
      0,  89,   7,   7,  36,  36,  36,  36,  36,  36,  36,  41,  36,  87,   0,   0,
     36,  36,  36,   0,  44,  44,  44,  44,  68,  36,  85,   0,   7,   7, 105,   0,
     36,  36,  36,  36,  36,  65,  44,   0,  36,  36,  36,  36,  36,  84,  44,  64,
     44,  44,  44,  44,  44,  44,  44,  90,   7,   7, 105,   0,   7,   7, 105,   0,
      0,  95, 127,   0,   0,   0,   0,   0,  44,  68,  36,  36,  36,  36,  36,  36,
     44,  68,  36,   0,   7,   7, 112, 128,   0,   0,  92,  44,  44,   0,   0,   0,
    111,  36,  36,  36,  36,  36,  36,  36,  84,  44,  44,  73,   7,   7,  74,  36,
     36,  80,  44,  44,  44,   0,   0,   0,  36,  44,  44,  44,  44,  44,   9, 116,
      7,   7, 105,  79,   7,   7,  74,  36,  36,  36,  36,  36,  36,  36,  36, 129,
      0,   0,   0,   0,  64,  44,  44,  44,  44,  44,  68,  78,  80, 130,   0,   0,
     44,  64,   0,   0,   0,   0,   0,  44,  25,  25,  25,  25,  25,  34,  15,  27,
     15,  15,  11,  11,  15,  39,  11, 131,  15,  15,  11,  11,  15,  15,  11,  11,
     15,  39,  11, 131,  15,  15, 132, 132,  15,  15,  11,  11,  15,  15,  15,  39,
     15,  15,  11,  11,  15, 133,  11, 134,  46, 133,  11, 135,  15,  46,  11,   0,
     15,  15,  11, 135,  46, 133,  11, 135, 136, 136, 137, 138, 139, 140, 141, 141,
      0, 142, 143, 144,   0,   0, 145, 146,   0, 147, 146,   0,   0,   0,   0, 148,
     61,  62, 149,  61,  21,   0,   0, 150,   0,   0,   0, 145,  15,  15,  15,  42,
      0,   0,   0,   0,  44,  44,  44,  44,  44,  44,  44,  44, 110,   0,   0,   0,
     47, 151, 152, 153,  23, 114,  10, 131,   0, 154,  48, 155,  11,  38, 156,  33,
      0, 157,  39, 158,   0,   0,   0,   0, 159,  38,  87,   0,   0,   0,   0,   0,
      0,   0, 145,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 160,  11,  11,
     15,  15,  39,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4, 161,
      0,   0, 141, 141, 141,   5,   0,   0,   0, 145,   0,   0,   0,   0,   0,   0,
      0, 162, 141, 141,   0,   0,   0,   0,   4, 141, 141, 141, 141, 141, 120,   0,
      0,   0,   0,   0,   0,   0, 141,   0,   0,   0,   0,   0,   0,   0,   0,   5,
     11,  11,  11,  22,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  24,
     31, 163,  26,  32,  25,  29,  15,  33,  25,  42, 151, 164,  53,   0,   0,   0,
     15, 165,   0,  21,  36,  36,  36,  36,  36,  36,   0,  95,   0,   0,   0,  92,
     36,  36,  36,  36,  36,  60,   0,   0,  36,  60,  36,  60,  36,  60,  36,  60,
    141, 141, 141,   5,   0,   0,   0,   5, 141, 141,   5, 166,   0,   0,   0,   0,
    167,  79, 141, 141,   5, 141, 141, 168,  79,  36,  80,  44,  79,  41,  36,  87,
     36,  36,  36,  36,  36,  60,  59,  79,   0,  79,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  41,  79,  36,  36,  36,  36,  36,  36,  60,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  60,   0,   0,   0,   0,   0,  36,  36,  36,  36,
     36,  36,  36,  87,   0,   0,   0,   0,  36,  36,  36,  36,  36,  36,  36, 169,
     36,  36,  36, 170,  36,  36,  36,  36,   7,   7,  74,   0,   0,   0,   0,   0,
     25,  25,  25, 171,  64,  44,  44, 172,  25,  25,  25,  25,  25,  25,   0,  92,
     36,  36,  36,  36, 173,   9,   0,   0,   0,   0,   0,   0,   0,  95,  36,  36,
    174,  25,  25,  25,  27,  25,  25,  25,  25,  25,  25,  25,  15,  15,  26,  30,
     25,  25, 175, 176,  25,   0,   0,   0,  25,  25, 177,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 178,  36, 179, 179,  65,  36,  36,  36,  36,  36,
     65,  44,   0,   0,   0,   0,   0,   0,  36,  36,  36,  36,  36, 128,   0,   0,
     73,  36,  36,  36,  36,  36,  36,  36,  44, 110,   0, 128,   7,   7, 105,   0,
     44,  44,  44,  44,  73,  36,  95,   0,  36,  80,  44, 173,  36,  36,  36,  36,
     36,  65,  44,  44,  44,   0,   0,   0,  36,  36,  36,  36,  65,  44,  44,  44,
    110,   0, 146,  95,   7,   7, 105,   0,  36,  36,  84,  44,  44,  64,   0,   0,
     65,  36,  36,  85,   7,   7, 105, 180,  36,  36,  36,  36,  36,  60, 181,   0,
     36,  36,  36,  36,  88,  71,  68,  80, 126,   0,   0,   0,   0,   0,  95,  41,
     36,  36,  65,  44, 182, 183,   0,   0,  79,  60,  79,  60,  79,  60,   0,   0,
     36,  60,  36,  60,   0,   0,   0,   0,  65,  44, 184,  85,   7,   7, 105,   0,
     36,   0,   0,   0,  36,  36,  36,  36,  36,  60,  95,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36,   0,  36,  36,  36,  41,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  41,   0,  15,  24,   0,   0, 185,  15,   0, 186,
     36,  36,  86,  36,  36,  60,  36,  43,  93,  86,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  41,   0,   0,   0,   0,   0,   0,   0,  95,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36, 187,  36,  36,  36,  36,  40,  36,  36,  36,
     36,  36,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  36,  36,  36,   0,
     44,  44,  44,  44, 188,   4, 120,   0,  44,  64,   0,   0, 189, 168, 141, 141,
    141, 190, 120,   0,   6, 191, 192, 161, 139,   0,   0,   0,  36,  86,  36,  36,
     36,  36,  36,  36,  36,  36,  36, 193,  56,   0,   5,   6,   0,   0, 194,   9,
     14,  15,  15,  15,  15,  15,  16, 195, 196, 197,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36,  80,  36,  36,  36,  36,  36,  36,  36,  60,
     40,  36,  40,  36,  40,  36,  40,  87,   0,   0,   0,   0,   0,   0, 198,   0,
     36,  36,  36,  79,  36,  36,  36,  36,  36,  60,  36,  36,  36,  36,  60,  93,
     36,  36,  36,  41,  36,  36,  36,  41,   0,   0,   0,   0,   0,   0,   0,  97,
     36,  36,  36,  36,  87,   0,   0,   0,  36,  36,  60,   0,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  36,  41,  36,   0,  36,  36,  79,  41,   0,   0,
     11,  11,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  36,  36,  36,  36,
     36,  41,  86,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  93,  87,  75,
     36,  36,  36,  36,  36,  36,   0,  40,  84,  59,   0,  44,  36,  79,  79,  36,
     36,  36,  36,  36,  36,   0,  64,  92,   0,   0,   0,   0,   0, 128,   0,   0,
     36,  36,  36,  36,  60,   0,   0,   0,  36,  36,  87,   0,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  44,  44,  44, 184, 116,   0,   0,   0,   0,   0,
     36,  36,  36,  36,  44,  44,  64, 199, 146,   0,   0,   0,  36,  36,  36,  36,
     36,  36,  87,   0,   7,   7, 105,   0,  36,  65,  44,  44,  44, 200,   7,   7,
    180,   0,   0,   0,   0,   0,   0,   0,  68, 201,   0,   0,   7,   7, 105,   0,
     36,  36,  65,  44,  44,  44,   0,   0,  60,   0,   0,   0,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  87,   0,  36,  87,   0,   0,  84,  44,  44,  44,
     44,  44,  44,  44,  44,  44,  44,  64,   0,   0,   0,  92, 111,  36,  36,  36,
     41,   0,   0,   0,   0,   0,   0,   0,   0,  57,  85,  57, 202,  61, 203,  44,
     64,  57,  44,   0,   0,   0,   0,   0,   0,   0,  99,  85,   0,   0,   0,   0,
     99, 110,   0,   0,   0,   0,   0,   0,  11,  11,  11,  11,  11,  11, 153,  15,
     15,  15,  15,  15,  15,  11,  11,  11,  11,  11,  11, 153,  15, 133,  15,  15,
     15,  15,  11,  11,  11,  11,  11,  11, 153,  15,  15,  15,  15,  15,  15,  48,
     47, 204,  10,  48,  11, 153, 165,  14,  15,  14,  15,  15,  11,  11,  11,  11,
     11,  11, 153,  15,  15,  15,  15,  15,  15,  49,  22,  10,  11,  48,  11, 205,
     15,  15,  15,  15,  15,  15,  49,  22,  11, 154, 160,  11, 205,  15,  15,  15,
     15,  15,  15,  11,  11,  11,  11,  11,  11, 153,  15,  15,  15,  15,  15,  15,
     11,  11,  11, 153,  15,  15,  15,  15, 153,  15,  15,  15,  15,  15,  15,  11,
     11,  11,  11,  11,  11, 153,  15,  15,  15,  15,  15,  15,  11,  11,  11,  11,
     15,  39,  11,  11,  11,  11,  11,  11, 205,  15,  15,  15,  15,  15,  24,  15,
     33,  11,  11,  11,  11,  11,  22,  15,  15,  15,  15,  15,  15, 133,  15,  11,
     11,  11,  11,  11,  11, 205,  15,  15,  15,  15,  15,  24,  15,  33,  11,  11,
     15,  15, 133,  15,  11,  11,  11,  11,  11,  11, 205,  15,  15,  15,  15,  15,
     24,  15,  27,  94,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
     36,  79,  36,  36,  36,  36,  36,  36,  96,  75,  79,  36,  60,  36, 106,   0,
    102,  95, 106,  79,  96,  75, 106, 106,  96,  75,  60,  36,  60,  36,  79,  43,
     36,  36,  93,  36,  36,  36,  36,   0,  79,  79,  93,  36,  36,  36,  36,   0,
     20,   0,   0,   0,   0,   0,   0,   0,  61,  61,  61,  61,  61,  61,  61,  61,
     44,  44,  44,  44,   0,   0,   0,   0,
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
     9,  9,  9,  0,  5,  5,  5,  5,  5,  0,  0,  0, 14, 14,  0,  0,
     3,  3,  3,  0,  9,  9,  9,  3, 10, 10,  0, 10, 10,  0,  9,  9,
     3,  9,  9,  9, 12,  9,  3,  3,  3,  5,  0,  3,  3,  9,  9,  3,
     3,  0,  3,  3,  3,  3,  9,  9, 10, 10,  9,  9,  9,  0,  0,  9,
    12, 12, 12,  0,  0,  0,  0,  5,  9,  3,  9,  9,  0,  9,  9,  9,
     9,  9,  3,  3,  3,  9,  0,  0, 14, 12,  9,  0,  3,  3,  9,  3,
     9,  3,  3,  3,  3,  3,  0,  0,  9,  0,  9,  9,  9,  0,  0,  0,
     3,  9,  3,  3, 12, 12, 10, 10,  3,  0,  0,  3,  3,  3,  9,  0,
     0,  0,  0,  3,  9,  9,  0,  9,  0,  0, 10, 10,  0,  0,  0,  9,
     0,  9,  9,  0,  0,  3,  0,  0,  9,  3,  0,  0,  0,  0,  3,  3,
     0,  0,  3,  9,  0,  9,  3,  3,  0,  0,  9,  0,  0,  0,  3,  0,
     3,  0,  3,  0, 10, 10,  0,  0,  0,  9,  0,  9,  0,  3,  0,  3,
     0,  3, 13, 13, 13, 13,  3,  3,  3,  0,  0,  0,  3,  3,  3,  9,
    10, 10, 12, 12, 10, 10,  3,  3,  0,  8,  0,  0,  0,  0, 12,  0,
    12,  0,  0,  0,  9,  0, 12,  9,  6,  9,  9,  9,  9,  9,  9, 13,
    13,  0,  0,  0,  3, 12, 12,  0,  9,  0,  3,  3,  0,  0, 14, 12,
    14, 12,  0,  3,  3,  3,  6,  0,  9,  3,  9,  0, 12, 12, 12, 12,
     0,  0, 12, 12,  9,  9, 12, 12,  3,  9,  9,  0,  8,  8,  0,  0,
     0,  8,  0,  8,  7,  0,  7,  7,  8,  0,  7,  0,  8,  0,  0,  0,
     6,  6,  6,  6,  6,  6,  6,  5,  3,  3,  5,  5,  0,  0,  0, 14,
    14,  0,  0,  0, 13, 13, 13, 13, 11,  0,  0,  0,  4,  4,  5,  5,
     5,  5,  5,  6,  0, 13, 13,  0, 12, 12,  0,  0,  0, 13, 13, 12,
     0,  0,  0,  6,  0,  0,  5,  5,  0, 13, 13,  7,  0,  0,  0,  8,
     0,  0,  7,  8,  8,  8,  7,  7,  8,  0,  8,  0,  8,  8,  0,  7,
     9,  7,  0,  0,  0,  8,  7,  7,  0,  0,  7,  0,  9,  9,  9,  8,
     0,  0,  8,  8, 13, 13, 13,  0,  0,  0, 13, 13,  8,  7,  7,  8,
     7,  8,  7,  3,  7,  7,  0,  7,  0,  0, 12,  9,  6, 14, 12,  0,
     0, 13, 13, 13,  9,  9,  0, 12,  9,  0, 12, 12,  8,  7,  9,  3,
     3,  3,  0,  9,  3,  3,  0, 12,  0,  0,  8,  7,  9,  0,  0,  8,
     7,  8,  7,  0,  8,  7,  8,  0,  7,  7,  7,  9,  9,  9,  3,  9,
     0, 12, 12, 12,  0,  0,  9,  3, 12, 12,  9,  9,  9,  3,  3,  0,
     3,  3,  3, 12,  0,  0,  0,  7,  0,  9,  3,  9,  9,  9, 13, 13,
    14, 14,  0, 14,  0, 14, 14,  0, 13,  0,  0, 13,  0, 14, 12, 12,
    14, 13, 13, 13,  9,  0,  0,  5,  0,  0, 14,  0,  0, 13,  0, 13,
    13, 12, 13, 13, 14,  0,  9,  9,  0,  5,  5,  5,  0,  5, 12, 12,
     3,  0, 10, 10,  9, 12, 12,  0,  3,  3,  3,  5,  5,  5,  5,  3,
     0,  8,  8,  0,  8,  0,  7,  7,
};

/* Sentence_Break: 5584 bytes. */

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
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 6, 1, 1,
};

static RE_UINT8 re_math_stage_3[] = {
     0,  1,  1,  2,  1,  1,  3,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     4,  5,  6,  7,  1,  8,  9, 10,  1,  6,  6, 11,  1,  1,  1,  1,
     1,  1,  1, 12,  1,  1, 13, 14,  1,  1,  1,  1, 15, 16, 17, 18,
     1,  1,  1,  1,  1,  1, 19,  1,
};

static RE_UINT8 re_math_stage_4[] = {
     0,  1,  2,  3,  0,  4,  5,  5,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  6,  7,  8,  0,  0,  0,  0,  0,  0,  0,
     9, 10, 11, 12, 13,  0, 14, 15, 16, 17, 18,  0, 19, 20, 21, 22,
    23, 23, 23, 23, 23, 23, 23, 23, 24, 25,  0, 26, 27, 28, 29, 30,
     0,  0,  0,  0,  0, 31, 32, 33, 34,  0, 35, 36,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 23, 23,  0, 19, 37,  0,  0,  0,  0,  0,
     0, 38,  0,  0,  0,  0,  0,  0,  0,  0,  0, 39,  0,  0,  0,  0,
     1,  3,  3,  0,  0,  0,  0, 40, 23, 23, 41, 23, 42, 43, 44, 23,
    45, 46, 47, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 48, 23, 23,
    23, 23, 23, 23, 23, 23, 49, 23, 44, 50, 51, 52, 53, 54,  0, 55,
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
     15, 224,   0,   0, 159,  31,   0,   0,   0,   2,   0,   0, 126,   1,   0,   0,
      4,  30,   0,   0, 255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235,
    239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,
     63, 255, 255, 255, 255, 207, 255, 255, 150, 254, 247,  10, 132, 234, 150, 170,
    150, 247, 247,  94, 255, 251, 255,  15, 238, 251, 255,  15,   0,   0,   3,   0,
};

/* Math: 538 bytes. */

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
    pos = (RE_UINT32)re_math_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_math_stage_3[pos + f] << 3;
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
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 23, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 24,  7, 25, 26, 13, 13, 13, 13, 13, 13, 13, 27,
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
    58, 59, 60, 61, 62, 31, 31, 31, 63, 64, 31, 31, 31, 31, 65, 31,
     1,  1,  1, 66, 67, 31, 31, 31,  1,  1,  1,  1, 68, 31, 31, 31,
     1,  1, 69, 31, 31, 31, 31, 70, 71, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 72, 73, 74, 75, 31, 31, 31, 31, 31, 31, 76, 31,
     1,  1,  1,  1,  1,  1, 77,  1,  1,  1,  1,  1,  1,  1,  1, 78,
    79, 31, 31, 31, 31, 31, 31, 31,  1,  1, 79, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_alphabetic_stage_4[] = {
      0,   0,   1,   1,   0,   2,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   5,   6,   0,   0,   7,   8,   9,  10,   4,  11,
      4,   4,   4,   4,  12,   4,   4,   4,   4,  13,  14,  15,  16,  17,  18,  19,
     20,   4,  21,  22,   4,   4,  23,  24,  25,   4,  26,   4,   4,  27,  28,  29,
     30,  31,  32,   0,   0,  33,   0,  34,   4,  35,  36,  37,  38,  39,  40,  41,
     42,  43,  44,  45,  46,  47,  48,  49,  38,  47,  50,  51,  52,  53,  54,   0,
     55,  56,  57,  49,  58,  56,  59,  60,  58,  61,  62,  63,  64,  65,  66,  67,
     15,  68,  69,   0,  70,  71,  72,   0,  73,   0,  74,  75,  76,  77,   0,   0,
      4,  78,  25,  79,  80,   4,  81,  82,   4,   4,  83,   4,  84,  85,  86,   4,
     87,   4,  88,   0,  89,   4,   4,  90,  15,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,  91,   1,   4,   4,  92,  93,  94,  94,  95,   4,  96,  97,   0,
      0,   4,   4,  98,   4,  99,   4, 100,  77, 101,  25, 102,   4, 103, 104,   0,
    105,   4, 106, 107,   0, 108,   0,   0,   4, 109, 110,   0,   4, 111,   4, 112,
      4, 100, 113, 114,   0,   0,   0, 115,   4,   4,   4,   4,   4,   4,   0,   0,
    116,   4, 117, 114,   4, 118, 119, 120,   0,   0,   0, 121, 122,   0,   0,   0,
    123, 124, 125,   4, 126,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 127,   4, 104,   4, 128, 106,   4,   4,   4,   4, 129,
      4,  81,   4, 130, 131, 132, 132,   4,   0, 133,   0,   0,   0,   0,   0,   0,
    134, 135,  15,   4, 136,  15,   4,  82, 137, 138,   4,   4, 139,  68,   0,  25,
      4,   4,   4,   4,   4, 100,   0,   0,   4,   4,   4,   4,   4,   4,  31,   0,
      4,   4,   4,   4,  31,   0,  25, 114, 140, 141,   4, 142, 143,   4,   4,  89,
    144, 145,   4,   4, 146, 147,   0, 148, 149,  16,   4,  94,   4,   4,  49, 150,
     28,  99, 151,  77,   4, 152, 133,   0,   4, 131, 153, 154,   4, 106, 155, 156,
    157, 158,   0,   0,   0,   0,   4, 147,   4,   4,   4,   4,   4, 159, 160, 105,
      4,   4,   4, 161,   4,   4, 162,   0, 163, 164, 165,   4,   4,  27, 166,   4,
      4, 114,  25,   4, 167,   4,  16, 168,   0,   0,   0, 169,   4,   4,   4,  77,
      0,   1,   1, 170,   4, 106, 171,   0, 172, 173, 174,   0,   4,   4,   4,  68,
      0,   0,   4,  90,   0,   0,   0,   0,   0,   0,   0,   0,  77,   4, 175,   0,
    106,  25, 147,   0, 114,   4, 176,   0,   4,   4,   4,   4, 114,   0,   0,   0,
    177, 178, 100,   0,   0,   0,   0,   0, 100, 162,   0,   0,   4, 179,   0,   0,
    180,  94,   0,  77,   0,   0,   0,   0,   4, 100, 100, 151,   0,   0,   0,   0,
      4,   4, 126,   0,   0,   0,   0,   0,   4,   4, 181,   0, 145,  32,  25, 126,
      4, 151,   0,   0,   4,   4, 182,   0,   0,   0,   0,   0,   4, 100,   0,   0,
      4,   4,   4, 139,   0,   0,   0,   0,   4,   4,   4, 183,   0,   0,   0,   0,
      4, 139,   0,   0,   0,   0,   0,   0,   4,  32,   0,   0,   0,   0,   0,   0,
      4,   4, 184, 106, 166,   0,   0,   0, 185,   0,   0,   0,   0,   0,   0,   0,
      4,   4, 186,   4, 187, 188, 189,   4, 190, 191, 192,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4, 193, 194,  82, 186, 186, 128, 128, 195, 195, 196,   0,
    189, 197, 198, 199, 200, 201,   0,   0,   4,   4,   4,   4,   4,   4, 131,   0,
      4,  90,   4,   4,   4,   4,   4,   4, 114,   0,   0,   0,   0,   0,   0,   0,
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
    255, 255, 255,   1, 253,  31,   0,   0, 240,   3, 255, 127, 255, 255, 255, 239,
    255, 223, 225, 255,  15,   0, 254, 254, 238, 159, 249, 255, 255, 253, 197, 227,
    159,  89, 128, 176,  15,   0,   3,   0, 238, 135, 249, 255, 255, 253, 109, 195,
    135,  25,   2,  94,   0,   0,  63,   0, 238, 191, 251, 255, 255, 253, 237, 227,
    191,  27,   1,   0,  15,   0,   0,   0, 159,  25, 192, 176,  15,   0,   2,   0,
    236, 199,  61, 214,  24, 199, 255, 195, 199,  29, 129,   0, 238, 223, 253, 255,
    255, 253, 239, 227, 223,  29,  96,   3, 236, 223, 253, 255, 223,  29,  96,  64,
     15,   0,   6,   0, 255, 255, 255, 231, 223,  93, 128,   0,  15,   0,   0, 252,
    236, 255, 127, 252, 255, 255, 251,  47, 127, 128,  95, 255,   0,   0,  12,   0,
    255, 255, 255,   7, 127,  32,   0,   0, 150,  37, 240, 254, 174, 236, 255,  59,
     95,  32,   0, 240,   1,   0,   0,   0, 255, 254, 255, 255, 255,  31, 254, 255,
      3, 255, 255, 254, 255, 255, 255,  31, 255, 255, 127, 249, 231, 193, 255, 255,
    127,  64,   0,  48, 191,  32, 255, 255, 255, 255, 255, 247, 255,  61, 127,  61,
    255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255, 255, 255,  61, 255,
    255, 255, 255, 135, 255, 255,   0,   0, 255, 255,  31,   0, 255, 159, 255, 255,
    255, 199,   1,   0, 255, 223,  15,   0, 255, 255,  15,   0, 255, 223,  13,   0,
    255, 255, 207, 255, 255,   1, 128,  16, 255, 255, 255,   0, 255,   7, 255, 255,
    255, 255,  63,   0, 255,  15, 255,   1, 255,  63,  31,   0, 255,  15, 255, 255,
    255,   3,   0,   0, 255, 255, 255,  15, 255, 255, 255, 127, 254, 255,  31,   0,
    128,   0,   0,   0, 255, 255, 239, 255, 239,  15,   0,   0, 255, 243,   0, 252,
    191, 255,   3,   0,   0, 224,   0, 252, 255, 255, 255,  63,   0, 222, 111,   0,
    255, 255,  63,  63,  63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15,
    255,  31, 220,  31,   0,   0,   2, 128,   0,   0, 255,  31, 132, 252,  47,  62,
     80, 189, 255, 243, 224,  67,   0,   0, 255,   1,   0,   0,   0,   0, 192, 255,
    255, 127, 255, 255,  31, 120,  12,   0, 255, 128,   0,   0, 255, 255, 127,   0,
    127, 127, 127, 127,   0, 128,   0,   0, 224,   0,   0,   0, 254,   3,  62,  31,
    255, 255, 127, 224, 224, 255, 255, 255, 255,  63, 254, 255, 255, 127,   0,   0,
    255,  31, 255, 255,   0,  12,   0,   0, 255, 127, 240, 143, 255, 255, 255, 128,
      0,   0, 128, 255, 252, 255, 255, 255, 255, 121,  15,   0, 255,   7,   0,   0,
      0,   0,   0, 255, 187, 247, 255, 255,   0,   0, 252,   8, 255, 255,   7,   0,
    255, 255, 247, 255, 255,  63,   0,   0, 255, 255, 127,   4,   5,   0,   0,  56,
    255, 255,  60,   0, 126, 126, 126,   0, 127, 127,   0,   0,  15,   0, 255, 255,
    127, 248, 255, 255, 255,  63, 255, 255, 255, 255, 255,   3, 127,   0, 248, 224,
    255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255, 255, 255, 252, 255,
      0,   0, 255,  15,   0,   0, 223, 255, 192, 255, 255, 255, 252, 252, 252,  28,
    255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63, 255, 255,   1,   0,
     15, 255,  62,   0,  63, 253, 255, 255, 255, 255, 191, 145, 255, 255, 255, 192,
    111, 240, 239, 254,  63,   0,   0,   0,  30,   0,   0,   0,   7,   0,   0,   0,
     31,   0, 255, 255,   3,   0,   0,   0, 255, 255, 223, 255, 255, 255, 255, 223,
    100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,
     95, 252, 253, 255,  63, 255, 255, 255, 253, 255, 255, 247, 255, 253, 255, 255,
    247,  15,   0,   0, 150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94,
    255, 251, 255,  15, 238, 251, 255,  15,
};

/* Alphabetic: 1817 bytes. */

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
     0,  0,  0, 40, 41,  0,  0,  0, 42, 43, 44, 26, 45,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 26, 46,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 26, 47, 48,  5,  5,  5, 49, 15, 50,  0,  0,  0,  0,  0,  0,
     0,  0,  5, 51, 52,  0,  0,  0,  0, 53,  5, 54, 55, 56,  0, 57,
     0,  0,  0,  0,  0,  0,  0,  0, 58,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
     0, 59, 60,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    61, 62, 63, 31, 64, 65, 66, 67, 68, 69, 70, 71, 72, 61, 62, 73,
    31, 64, 74, 60, 67, 75, 76, 77, 78, 74, 79, 26, 80, 67, 81,  0,
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
      0,   0,   2, 128,   0,   0, 255,  31,   0, 196,   8,   0,   0, 128,  16,  50,
    192,  67,   0,   0,  16,   0,   0,   0, 255,   3,   0,   0, 255, 255, 255, 127,
     98,  21, 218,  63,  26,  80,   8,   0, 191,  32,   0,   0, 170,  42,   0,   0,
    170, 170, 170,   0, 168, 170, 171, 170, 170, 170, 255, 149, 170,  80,  10,   0,
    170,   2,   0,   0,   0,   0,   0,   7, 127,   0, 248,   0,   0, 255, 255, 255,
    255, 255,   0,   0,   0,   0,   0, 252, 255, 255,  15,   0,   0, 192, 223, 255,
    252, 255, 255,  15,   0,   0, 192, 235, 239, 255,   0,   0,   0, 252, 255, 255,
     15,   0,   0, 192, 255, 255, 255,   0,   0,   0, 252, 255, 255,  15,   0,   0,
    192, 255, 255, 255,   0, 192, 255, 255,   0,   0, 192, 255,  63,   0,   0,   0,
    252, 255, 255, 247,   3,   0,   0, 240, 255, 255, 223,  15, 255, 127,  63,   0,
    255, 253,   0,   0, 247,  11,   0,   0,
};

/* Lowercase: 697 bytes. */

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
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 48, 49, 50,
    51, 61, 62, 54, 55, 51, 63, 64, 65, 66, 37, 38, 54, 67, 68,  0,
};

static RE_UINT8 re_uppercase_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7, 255, 255, 127, 127,  85,  85,  85,  85,
     85,  85,  85, 170, 170,  84,  85,  85,  85,  85,  85,  43, 214, 206, 219, 177,
    213, 210, 174,  17, 144, 164, 170,  74,  85,  85, 210,  85,  85,  85,   5, 108,
    122,  85,   0,   0,   0,   0,  69,   0,  64, 215, 254, 255, 251,  15,   0,   0,
      0, 128,  28,  85,  85,  85, 144, 230, 255, 255, 255, 255, 255, 255,   0,   0,
      1,  84,  85,  85, 171,  42,  85,  85,  85,   0, 254, 255, 255, 255, 127,   0,
    191,  32,   0,   0,  85,  85,  21,  64,   0, 255,   0,  63,   0, 255,   0, 255,
      0,  63,   0, 170,   0, 255,   0,   0,   0,   0,   0,  15,   0,  15,   0,  15,
      0,  31,   0,  15, 132,  56,  39,  62,  80,  61,  15, 192,  32,   0,   0,   0,
      8,   0,   0,   0,   0,   0, 192, 255, 255, 127,   0,   0, 157, 234,  37, 192,
      5,  40,   4,   0,  85,  21,   0,   0,  85,  85,  85,   0,  84,  85,  84,  85,
     85,  85,   0, 106,  85,  40,   5,   0,  85,   5,   0,   0, 255,   0,   0,   0,
    255, 255, 255,   3,   0,   0, 240, 255, 255,  63,   0,   0,   0, 255, 255, 255,
      3,   0,   0, 208, 100, 222,  63,   0,   0,   0, 255, 255, 255,   3,   0,   0,
    176, 231, 223,  31,   0,   0,   0, 123,  95, 252,   1,   0,   0, 240, 255, 255,
     63,   0,   0,   0,   3,   0,   0, 240, 255, 255,  63,   0,   1,   0,   0,   0,
    252, 255, 255,   7,   0,   0,   0, 240, 255, 255,  31,   0, 255,   1,   0,   0,
      0,   4,   0,   0,
};

/* Uppercase: 629 bytes. */

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
     0,  0,  0, 28, 29,  0,  0,  0, 30, 31, 32,  4, 33,  0,  0,  0,
     0,  0,  0,  0,  0, 34,  4, 35,  0,  0,  0,  0,  0,  0,  0,  0,
     4, 36, 37,  4,  4,  4,  4, 38,  4, 21,  0,  0,  0,  0,  0,  0,
     0,  0,  4, 39, 40,  0,  0,  0,  0, 41,  4,  4, 42, 43,  0, 44,
     0,  0,  0,  0,  0,  0,  0,  0, 45,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
     4,  4, 46,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     4,  4, 47,  4, 48, 49, 50,  4, 51, 52, 53,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4, 54, 55,  5, 47, 47, 36, 36, 56, 56, 57,  0,
};

static RE_UINT8 re_cased_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 255, 255, 255, 247, 240, 255, 255, 255, 255, 255, 239, 255,
    255, 255, 255,   1,   3,   0,   0,   0,  31,   0,   0,   0,  32,   0,   0,   0,
      0,   0, 207,  60,  64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,
      3, 252, 255, 255, 255,   0, 254, 255, 255, 255, 127,   0, 254, 255, 255, 255,
    255,   0,   0,   0, 191,  32,   0,   0, 255, 255,  63,  63,  63,  63, 255, 170,
    255, 255, 255,  63, 255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,
      0,   0,   2, 128,   0,   0, 255,  31, 132, 252,  47,  62,  80, 189,  31, 242,
    224,  67,   0,   0,  24,   0,   0,   0,   0,   0, 192, 255, 255,   3,   0,   0,
    255, 127, 255, 255, 255, 255, 255, 127,  31, 120,  12,   0, 255,  63,   0,   0,
    255, 255, 255,   0, 252, 255, 255, 255, 255, 120,  15,   0, 255,   7,   0,   0,
      0,   0,   0,   7, 127,   0, 248,   0, 255, 255,   0,   0, 255, 255, 223, 255,
    255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223,
    255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255, 253, 255, 255, 247,
    255, 253, 255, 255, 247,  15,   0,   0,
};

/* Cased: 617 bytes. */

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
    0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 4, 4,
    4, 4,
};

static RE_UINT8 re_case_ignorable_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  8,  9,  7,  7,  7,  7,  7,  7,  7,  7,  7, 10,
    11, 12, 13,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 15,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
    16,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
};

static RE_UINT8 re_case_ignorable_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1,  1, 17,  1,  1,  1, 18, 19, 20, 21, 22, 23, 24,  1, 25,
    26,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 27, 28, 29,  1,
    30,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    31,  1,  1,  1, 32,  1, 33, 34, 35, 36, 37, 38,  1,  1,  1,  1,
     1,  1,  1, 39,  1,  1, 40, 41,  1, 42,  1,  1,  1,  1,  1,  1,
     1,  1, 43,  1,  1,  1,  1,  1, 44, 45,  1,  1,  1,  1, 46,  1,
     1,  1,  1,  1,  1,  1,  1, 47,  1, 48, 49,  1,  1,  1,  1,  1,
    50, 51,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_case_ignorable_stage_4[] = {
      0,   1,   2,   3,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   5,   6,   6,   6,   6,   6,   7,   8,   0,   0,   0,
      0,   0,   0,   0,   9,   0,   0,   0,   0,   0,  10,   0,  11,  12,  13,  14,
     15,   0,  16,  17,   0,   0,  18,  19,  20,   5,  21,   0,   0,  22,   0,  23,
     24,  25,  26,   0,   0,   0,   0,  27,  28,  29,  30,  31,  32,  33,  34,  35,
     36,  33,  37,  38,  36,  33,  39,  35,  32,  40,  41,  35,  42,   0,  43,   0,
      0,  44,  45,  35,   0,  40,  46,  35,   0,   0,  34,  35,   0,   0,  47,   0,
      0,  48,  49,   0,   0,  50,  51,   0,  52,  53,   0,  54,  55,  56,  57,   0,
      0,  58,  59,  60,  61,   0,   0,  33,   0,   0,  62,   0,   0,   0,   0,   0,
     63,  63,  64,  64,   0,  65,  66,   0,  67,   0,  68,   0,   0,  69,   0,   0,
      0,  70,   0,   0,   0,   0,   0,   0,  71,   0,  72,  73,   0,  74,   0,   0,
     75,  76,  42,  77,  78,  79,   0,  80,   0,  81,   0,  82,   0,   0,  83,  84,
      0,  85,   6,  86,  87,   6,   6,  88,   0,   0,   0,   0,   0,  89,  90,  91,
     92,  93,   0,  94,  95,   0,   5,  96,   0,   0,   0,  97,   0,   0,   0,  98,
      0,   0,   0,  99,   0,   0,   0,   6,   0, 100,   0,   0,   0,   0,   0,   0,
    101, 102,   0,   0, 103,   0,   0, 104, 105,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  82, 106,   0,   0, 107, 108,   0,   0, 109,
      6,  78,   0,  17, 110,   0,   0,  52, 111, 112,   0,   0,   0,   0, 113, 114,
      0, 115, 116,   0,  28, 117, 100,   0,   0, 118, 119,  17,   0, 120, 121, 122,
      0,   0,   0,   0,   0,   0,   0, 123,   2,   0,   0,   0,   0, 124,  78,   0,
    125, 126, 127,   0,   0,   0,   0, 108,   1,   2,   3,  17,  44,   0,   0, 128,
      0,   0,   0,   0,   0,   0,   0, 129, 130, 131,   0,   0,   0,   0,   0,   0,
     32, 132, 126,   0,  78, 133,   0,   0,  28, 134,   0,   0,  78, 135,   0,   0,
      0,   0,   0,   0,   0, 136,   0,   0,   0,   0,   0,   0, 137,   0,   0,   0,
      0,   0,   0, 138, 139, 140,   0,   0,   0,   0, 141,   0,   0,   0,   0,   0,
     32,   6,   6,   6,   0,   0,   0,   0,   6,   6,   6,   6,   6,   6,   6, 142,
};

static RE_UINT8 re_case_ignorable_stage_5[] = {
      0,   0,   0,   0, 128,  64,   0,   4,   0,   0,   0,  64,   1,   0,   0,   0,
      0, 161, 144,   1,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255,  48,   4,
    176,   0,   0,   0, 248,   3,   0,   0,   0,   0,   0,   2,   0,   0, 254, 255,
    255, 255, 255, 191, 182,   0,   0,   0,   0,   0,  16,   0,  31,   0, 255,   7,
      1, 248, 255, 255,   0,   0,   1,   0,   0,   0, 192, 191, 255,  61,   0,   0,
      0, 128,   2,   0, 255,   7,   0,   0, 192, 255,   1,   0,   0, 248,  63,   4,
      0,   0, 192, 255, 255,  63,   0,   0,   0,   0,   0,  14, 240, 255, 255, 127,
      7,   0,   0,   0,   0,   0,   0,  20, 254,  33, 254,   0,  12,   0,   2,   0,
      2,   0,   0,   0,   0,   0,   0,  16,  30,  32,   0,   0,  12,   0,   0,   0,
      6,   0,   0,   0, 134,  57,   2,   0,   0,   0,  35,   0, 190,  33,   0,   0,
      0,   0,   0, 144,  30,  32,  64,   0,   4,   0,   0,   0,   1,  32,   0,   0,
      0,   0,   0, 192, 193,  61,  96,   0,  64,  48,   0,   0,   0,   4,  92,   0,
      0,   0, 242,   7, 192, 127,   0,   0,   0,   0, 242,  27,  64,  63,   0,   0,
      0,   0,   0,   3,   0,   0, 160,   2,   0,   0, 254, 127, 223, 224, 255, 254,
    255, 255, 255,  31,  64,   0,   0,   0,   0, 224, 253, 102,   0,   0,   0, 195,
      1,   0,  30,   0, 100,  32,   0,  32,   0,   0,   0, 224,   0,   0,  28,   0,
      0,   0,  12,   0,   0,   0, 176,  63,  64, 254, 143,  32,   0,  56,   0,   0,
      8,   0,   0,   0,   0,   2,   0,   0, 135,   1,   4,  14,   0,   0, 128,   1,
      0,   0,  64, 127, 229,  31, 248, 159, 128,   0,   0,   0,  15,   0,   0,   0,
      0,   0, 208,  23,   0, 248,  15,   0,   3,   0,   0,   0,  60,  11,   0,   0,
     64, 163,   3,   0,   0, 240, 207,   0,   0,   0,   0,  63,   0,   0, 247, 255,
    253,  33,  16,   0,   0, 240, 255, 255, 255,   7,   0,   1,   0,   0,   0, 248,
    127,   0,   0, 240,   0,   0,   0, 160,   3, 224,   0, 224,   0, 224,   0,  96,
      0, 248,   0,   3, 144, 124,   0,   0,  31, 252,   2, 128,   0,   0, 255,  31,
    255, 255,   1,   0,   0,   0,   0,  48,   0, 128,   3,   0,   0, 128,   0, 128,
      0, 128,   0,   0,  32,   0,   0,   0,   0,  60,  62,   8,   0,   0,   0, 126,
      0,   0,   0, 112,   0,   0,  32,   0,   0,  16,   0,   0,   0, 128, 247, 191,
      0,   0,   0, 128,   0,   0,   3,   0,   0,   7,   0,   0,  68,   8,   0,   0,
     96,   0,   0,   0,  16,   0,   0,   0, 255, 255,   3,   0, 192,  63,   0,   0,
    128, 255,   3,   0,   0,   0, 200,  19,   0, 126, 102,   0,   8,  16,   0,   0,
      0,   0, 157, 193,   2,   0,   0,  32,   0,  48,  88,   0,  32,  33,   0,   0,
      0,   0, 252, 255, 255, 255,   8,   0, 127,   0,   0,   0,   0,   0,  36,   0,
      8,   0,   0,  14,   0,   0,   0,  32, 110, 240,   0,   0,   0,   0,   0, 135,
      0,   0,   0, 255,   0,   0, 120,  38, 128, 239,  31,   0,   0,   0, 192, 127,
      0,  40, 191,   0,   0, 128, 255, 255, 128,   3, 248, 255, 231,  15,   0,   0,
      0,  60,   0,   0,  28,   0,   0,   0, 255, 255,   0,   0,
};

/* Case_Ignorable: 1254 bytes. */

RE_UINT32 re_get_case_ignorable(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_case_ignorable_stage_1[f] << 4;
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
    191,  32,   0,   0,  85,  85,  21,  64,   0, 255,   0,  63,   0, 255,   0, 255,
      0,  63,   0, 170,   0, 255,   0,   0,   0, 255,   0,  31,   0,  31,   0,  15,
      0,  31,   0,  31,  64,  12,   4,   0,   8,   0,   0,   0,   0,   0, 192, 255,
    255, 127,   0,   0, 157, 234,  37, 192,   5,  40,   4,   0,  85,  21,   0,   0,
     85,  85,  85,   0,  84,  85,  84,  85,  85,  85,   0, 106,  85,  40,   5,   0,
     85,   5,   0,   0, 255,   0,   0,   0,
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
    170, 170,  45, 170, 168, 170,  10, 144, 133, 170, 223,  10, 105, 139,  38,  32,
      9,  31,   4,   0,  32,   0,   0,   0,   0,   0, 138,  56,   0,   0,   1,   0,
      0, 240, 255, 255, 255, 127, 227, 170, 170, 170,  39,   9,   0,   0, 255, 255,
    255, 255, 255, 255,   2, 168, 170, 170,  84, 213, 170, 170, 170,   0,   0,   0,
    254, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,  34, 170, 170, 234,  15,
    255,   0,  63,   0, 255,   0, 255,   0,  63,   0, 255,   0, 255,   0, 255,  63,
    255, 255, 223,  80, 220,  16, 207,   0, 255,   0, 220,  16,   0,  64,   0,   0,
     16,   0,   0,   0, 255,   3,   0,   0, 255, 255, 255, 127,  98,  21,  72,   0,
     10,  80,   8,   0, 191,  32,   0,   0, 170,  42,   0,   0, 170, 170, 170,   0,
    168, 170, 168, 170, 170, 170,   0, 148, 170,  16,  10,   0, 170,   2,   0,   0,
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
    170, 170,  43, 170, 168, 170,  10, 144, 133, 170, 223,  10, 105, 139,  38,  32,
      9,  31,   4,   0,  32,   0,   0,   0,   0,   0, 138,  56,   0,   0,   1,   0,
      0, 240, 255, 255, 255, 127, 227, 170, 170, 170,  39,   9,   0,   0, 255, 255,
    255, 255, 255, 255,   2, 168, 170, 170,  84, 213, 170, 170, 170,   0,   0,   0,
    254, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,  34, 170, 170, 234,  15,
    255,   0,  63,   0, 255,   0, 255,   0,  63,   0, 255,   0, 255,   0, 255,  63,
    255,   0, 223,  64, 220,   0, 207,   0, 255,   0, 220,   0,   0,  64,   0,   0,
     16,   0,   0,   0, 255,   3,   0,   0, 255, 255, 255, 127,  98,  21,  72,   0,
     10,  80,   8,   0, 191,  32,   0,   0, 170,  42,   0,   0, 170, 170, 170,   0,
    168, 170, 168, 170, 170, 170,   0, 148, 170,  16,  10,   0, 170,   2,   0,   0,
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
     85,   0, 254, 255, 255, 255, 127,   0, 128,   0,   0,   0, 191,  32,   0,   0,
     85,  85,  21,  76,   0, 255,   0,  63,   0, 255,   0, 255,   0,  63,   0, 170,
      0, 255,   0,   0, 255, 255, 156,  31, 156,  31,   0,  15,   0,  31, 156,  31,
     64,  12,   4,   0,   8,   0,   0,   0,   0,   0, 192, 255, 255, 127,   0,   0,
    157, 234,  37, 192,   5,  40,   4,   0,  85,  21,   0,   0,  85,  85,  85,   0,
     84,  85,  84,  85,  85,  85,   0, 106,  85,  40,   5,   0,  85,   5,   0,   0,
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
     0, 44,  4, 45, 46, 47,  0,  0, 48,  0,  0,  0,  0,  0,  0,  0,
     0,  1,  1,  0,  0,  0,  0,  0,  4,  4, 49,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_casemapped_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   0,  32,   0, 255, 255, 127, 255,
    255, 255, 255, 255, 255, 255, 255, 254, 255, 223, 255, 247, 255, 243, 255, 179,
    240, 255, 255, 255, 253, 255,  15, 252, 255, 255, 223,  10, 105, 139,  38,  32,
      9,  31,   4,   0,  32,   0,   0,   0,   0,   0, 207,  56,  64, 215, 255, 255,
    251, 255, 255, 255, 255, 255, 227, 255, 255, 255, 183, 239,   3, 252, 255, 255,
    255,   0, 254, 255, 255, 255, 127,   0, 254, 255, 255, 255, 255,   0,   0,   0,
    191,  32,   0,   0,   0,   0,   0,  34, 255, 255, 255,  79, 255, 255,  63,  63,
     63,  63, 255, 170, 255, 255, 255,  63, 255, 255, 223,  95, 220,  31, 207,  15,
    255,  31, 220,  31,  64,  12,   4,   0,   0,  64,   0,   0,  24,   0,   0,   0,
      0,   0, 192, 255, 255,   3,   0,   0, 255, 127, 255, 255, 255, 255, 255, 127,
    255, 255, 109, 192,  15, 120,  12,   0, 255,  63,   0,   0, 255, 255, 255,   0,
    252, 255, 252, 255, 255, 255,   0, 254, 255,  56,  15,   0, 255,   7,   0,   0,
    127,   0, 248,   0, 255, 255,   0,   0,
};

/* Changes_When_Casemapped: 530 bytes. */

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
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 23, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 24,  7, 25, 26, 13, 13, 13, 13, 13, 13, 13, 27,
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
    56, 57, 58, 59, 60, 31, 31, 31, 61, 62, 31, 31, 31, 31, 63, 31,
     1,  1,  1, 64, 65, 31, 31, 31,  1,  1,  1,  1, 66, 31, 31, 31,
     1,  1, 67, 31, 31, 31, 31, 68, 69, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 70, 71, 72, 73, 31, 31, 31, 31, 31, 31, 74, 31,
     1,  1,  1,  1,  1,  1, 75,  1,  1,  1,  1,  1,  1,  1,  1, 76,
    77, 31, 31, 31, 31, 31, 31, 31,  1,  1, 77, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_id_start_stage_4[] = {
      0,   0,   1,   1,   0,   2,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   5,   6,   0,   0,   0,   7,   8,   9,   4,  10,
      4,   4,   4,   4,  11,   4,   4,   4,   4,  12,  13,  14,  15,   0,  16,  17,
      0,   4,  18,  19,   4,   4,  20,  21,  22,  23,  24,   4,   4,  25,  26,  27,
     28,  29,  30,   0,   0,  31,   0,   0,  32,  33,  34,  35,  36,  37,  38,  39,
     40,  41,  42,  43,  44,  45,  46,  47,  36,  45,  48,  49,  50,  51,  46,   0,
     52,  53,  54,  47,  52,  53,  55,  56,  52,  57,  58,  59,  60,  61,  62,   0,
     14,  63,  62,   0,  64,  65,  66,   0,  67,   0,  68,  69,  70,   0,   0,   0,
      4,  71,  72,  73,  74,   4,  75,  76,   4,   4,  77,   4,  78,  79,  80,   4,
     81,   4,  82,   0,  23,   4,   4,  83,  14,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,  84,   1,   4,   4,  85,  86,  87,  87,  88,   4,  89,  90,   0,
      0,   4,   4,  91,   4,  92,   4,  93,  94,   0,  16,  95,   4,  96,  97,   0,
     98,   4,  83,   0,   0,  99,   0,   0, 100,  89, 101,   0, 102, 103,   4, 104,
      4, 105, 106, 107,   0,   0,   0, 108,   4,   4,   4,   4,   4,   4,   0,   0,
    109,   4, 110, 107,   4, 111, 112, 113,   0,   0,   0, 114, 115,   0,   0,   0,
    116, 117, 118,   4, 119,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      4, 120, 121,   4,   4,   4,   4, 122,   4,  75,   4, 123,  98, 124, 124,   0,
    125, 126,  14,   4, 127,  14,   4,  76, 100, 128,   4,   4, 129,  82,   0,  16,
      4,   4,   4,   4,   4,  93,   0,   0,   4,   4,   4,   4,   4,   4,  69,   0,
      4,   4,   4,   4,  69,   0,  16, 107, 130, 131,   4, 132,  91,   4,   4,  23,
    133, 134,   4,   4, 135,  18,   0, 136, 137, 138,   4,  89, 134,  89,   0, 139,
     26, 140,  62,  94,  32, 141, 142,   0,   4, 119, 143, 144,   4, 145, 146, 147,
    148, 149,   0,   0,   0,   0,   4, 138,   4,   4,   4,   4,   4, 150, 151, 152,
      4,   4,   4, 153,   4,   4, 154,   0, 155, 156, 157,   4,   4,  87, 158,   4,
      4, 107,  16,   4, 159,   4,  15, 160,   0,   0,   0, 161,   4,   4,   4,  94,
      0,   1,   1, 162,   4, 121, 163,   0, 164, 165, 166,   0,   4,   4,   4,  82,
      0,   0,   4,  83,   0,   0,   0,   0,   0,   0,   0,   0,  94,   4, 167,   0,
    121,  16,  18,   0, 107,   4, 168,   0,   4,   4,   4,   4, 107,   0,   0,   0,
    169, 170,  93,   0,   0,   0,   0,   0,  93, 154,   0,   0,   4, 171,   0,   0,
    172,  89,   0,  94,   0,   0,   0,   0,   4,  93,  93, 141,   0,   0,   0,   0,
      4,   4, 119,   0,   0,   0,   0,   0, 102,  91,   0,   0, 102,  23,  16, 119,
    102,  62,   0,   0, 102, 141, 173,   0,   0,   0,   0,   0,   4,  18,   0,   0,
      4,   4,   4, 129,   0,   0,   0,   0,   4,   4,   4, 138,   0,   0,   0,   0,
      4, 129,   0,   0,   0,   0,   0,   0,   4,  30,   0,   0,   0,   0,   0,   0,
      4,   4, 174,   0, 158,   0,   0,   0,  47,   0,   0,   0,   0,   0,   0,   0,
      4,   4, 175,   4, 176, 177, 178,   4, 179, 180, 181,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4, 182, 183,  76, 175, 175, 120, 120, 184, 184, 143,   0,
    178, 185, 186, 187, 188, 189,   0,   0,   4,   4,   4,   4,   4,   4,  98,   0,
      4,  83,   4,   4,   4,   4,   4,   4, 107,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_id_start_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,   0,   0, 223,  60,
     64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,   3, 252, 255, 255,
    255,   0, 254, 255, 255, 255, 127,   2, 254, 255, 255, 255, 255,   0,   0,   0,
      0,   0, 255, 255, 255,   7,   7,   0, 255,   7,   0,   0,   0, 192, 254, 255,
    255, 255,  47,   0,  96, 192,   0, 156,   0,   0, 253, 255, 255, 255,   0,   0,
      0, 224, 255, 255,  63,   0,   2,   0,   0, 252, 255, 255, 255,   7,  48,   4,
    255, 255,  63,   4,  16,   1,   0,   0, 255, 255, 255,   1, 253,  31,   0,   0,
    240, 255, 255, 255, 255, 255, 255,  35,   0,   0,   1, 255,   3,   0, 254, 254,
    224, 159, 249, 255, 255, 253, 197,  35,   0,  64,   0, 176,   3,   0,   3,   0,
    224, 135, 249, 255, 255, 253, 109,   3,   0,   0,   0,  94,   0,   0,  28,   0,
    224, 191, 251, 255, 255, 253, 237,  35,   0,   0,   1,   0,   3,   0,   0,   0,
      0,   0,   0, 176,   3,   0,   2,   0, 232, 199,  61, 214,  24, 199, 255,   3,
    224, 223, 253, 255, 255, 253, 239,  35,   0,   0,   0,   3,   0,   0,   0,  64,
      3,   0,   6,   0, 255, 255, 255,  39,   0,  64,   0,   0,   3,   0,   0, 252,
    224, 255, 127, 252, 255, 255, 251,  47, 127,   0,   0,   0, 255, 255,  13,   0,
    150,  37, 240, 254, 174, 236,  13,  32,  95,   0,   0, 240,   1,   0,   0,   0,
    255, 254, 255, 255, 255,  31,   0,   0,   0,  31,   0,   0, 255,   7,   0, 128,
      0,   0,  63,  60,  98, 192, 225, 255,   3,  64,   0,   0, 191,  32, 255, 255,
    255, 255, 255, 247, 255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,
     61, 255, 127, 255, 255, 255,  61, 255, 255, 255, 255,   7, 255, 255,  31,   0,
    255, 159, 255, 255, 255, 199,   1,   0, 255, 223,   3,   0, 255, 255,   3,   0,
    255, 223,   1,   0, 255, 255,  15,   0,   0,   0, 128,  16, 255, 255, 255,   0,
    255,   5, 255, 255, 255, 255,  63,   0, 255, 255, 255,  31, 255,  63,  31,   0,
    255,  15,   0,   0, 254,   0,   0,   0, 255, 255, 127,   0, 128,   0,   0,   0,
    224, 255, 255, 255, 224,  15,   0,   0, 248, 255, 255, 255,   1, 192,   0, 252,
     63,   0,   0,   0,  15,   0,   0,   0,   0, 224,   0, 252, 255, 255, 255,  63,
      0, 222,  99,   0, 255, 255,  63,  63,  63,  63, 255, 170, 255, 255, 223,  95,
    220,  31, 207,  15, 255,  31, 220,  31,   0,   0,   2, 128,   0,   0, 255,  31,
    132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0, 255,   1,   0,   0,
    255, 127, 255, 255, 255, 255, 255, 127,  31, 120,  12,   0, 255, 128,   0,   0,
    127, 127, 127, 127, 224,   0,   0,   0, 254,   3,  62,  31, 255, 255, 127, 248,
    255,  63, 254, 255, 255, 127,   0,   0, 255,  31, 255, 255,   0,  12,   0,   0,
    255, 127,   0, 128,   0,   0, 128, 255, 252, 255, 255, 255, 255, 121,  15,   0,
      0,   0,   0, 255, 187, 247, 255, 255,   7,   0,   0,   0,   0,   0, 252,   8,
     63,   0, 255, 255, 255, 255,   7,   0,   0, 128,   0,   0, 247,  15,   0,   0,
    255, 255, 127,   4, 255, 255,  98,  62,   5,   0,   0,  56, 255,   7,  28,   0,
    126, 126, 126,   0, 127, 127,   0,   0,  15,   0, 255, 255, 127, 248, 255, 255,
    255, 255, 255,  15, 255,  63, 255, 255, 255, 255, 255,   3, 127,   0, 248, 160,
    255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255, 255, 255, 252, 255,
      0,   0, 255,  15,   0,   0, 223, 255, 192, 255, 255, 255, 252, 252, 252,  28,
    255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63, 255, 255,   1,   0,
     15, 255,  62,   0,  63, 253, 255, 255, 255, 255, 191, 145, 255, 255, 255, 192,
      1,   0, 239, 254,  30,   0,   0,   0,  31,   0,   1,   0, 255, 255, 223, 255,
    255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223,
    255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255, 253, 255, 255, 247,
    255, 253, 255, 255, 150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94,
    255, 251, 255,  15, 238, 251, 255,  15,
};

/* ID_Start: 1753 bytes. */

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
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 23, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 24,  7, 25, 26, 13, 13, 13, 13, 13, 13, 13, 27,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    28, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
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
    56, 57, 58, 59, 60, 31, 31, 31, 61, 62, 31, 31, 31, 31, 63, 31,
     1,  1,  1, 64, 65, 31, 31, 31,  1,  1,  1,  1, 66, 31, 31, 31,
     1,  1, 67, 31, 31, 31, 31, 68, 69, 31, 31, 31, 31, 31, 31, 31,
    31, 70, 71, 31, 72, 73, 74, 75, 31, 31, 31, 31, 31, 31, 76, 31,
     1,  1,  1,  1,  1,  1, 77,  1,  1,  1,  1,  1,  1,  1,  1, 78,
    79, 31, 31, 31, 31, 31, 31, 31,  1,  1, 79, 31, 31, 31, 31, 31,
    31, 80, 31, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_id_continue_stage_4[] = {
      0,   1,   2,   3,   0,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   7,   8,   6,   6,   6,   9,  10,  11,   6,  12,
      6,   6,   6,   6,  13,   6,   6,   6,   6,  14,  15,  16,  14,  17,  18,  19,
     20,   6,   6,  21,   6,   6,  22,  23,  24,   6,  25,   6,   6,  26,   6,  27,
      6,  28,  29,   0,   0,  30,   0,  31,   6,   6,   6,  32,  33,  34,  35,  36,
     37,  38,  39,  40,  41,  42,  43,  44,  33,  42,  45,  46,  47,  48,  49,  50,
     51,  52,  53,  44,  54,  55,  56,  57,  54,  58,  59,  60,  61,  62,  63,  64,
     16,  65,  66,   0,  67,  68,  69,   0,  70,  71,  72,  73,  74,  75,  76,   0,
      6,   6,  77,   6,  78,   6,  79,  80,   6,   6,  81,   6,  82,  83,  84,   6,
     85,   6,  58,  86,  87,   6,   6,  88,  16,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,  89,   3,   6,   6,  90,  91,  88,  92,  93,   6,   6,  94,  95,
     96,   6,   6,  97,   6,  98,   6,  99,  75, 100, 101, 102,   6, 103, 104,   0,
     29,   6, 105, 106, 107, 108,   0,   0,   6,   6, 109, 110,   6,   6,   6,  92,
      6,  97, 111,  78,   0,   0, 112, 113,   6,   6,   6,   6,   6,   6,   6, 114,
    115,   6, 116,  78,   6, 117, 118, 119,   0, 120, 121, 122, 123,   0, 123, 124,
    125, 126, 127,   6, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      6, 129, 105,   6,   6,   6,   6, 130,   6,  79,   6, 131, 113, 132, 132,   6,
    133, 134,  16,   6, 135,  16,   6,  80, 136, 137,   6,   6, 138,  65,   0,  24,
      6,   6,   6,   6,   6,  99,   0,   0,   6,   6,   6,   6,   6,   6, 139,   0,
      6,   6,   6,   6, 139,   0,  24,  78, 140, 141,   6, 142, 143,   6,   6,  26,
    144, 145,   6,   6, 146, 147,   0, 148,   6, 149,   6,  92,   6,   6, 150, 151,
      6, 152,  92,  75,   6,   6, 153,   0,   6, 113, 154, 155,   6,   6, 156, 157,
    158, 159,   0,   0,   0,   0,   6, 160,   6,   6,   6,   6,   6, 161, 162,  29,
      6,   6,   6, 152,   6,   6, 163,   0, 164, 165, 166,   6,   6,  26, 167,   6,
      6,  78,  24,   6, 168,   6, 149, 169,  87, 170, 171, 172,   6,   6,   6,  75,
      1,   2,   3, 101,   6, 105, 173,   0, 174, 175, 176,   0,   6,   6,   6,  65,
      0,   0,   6,  88,   0,   0,   0, 177,   0,   0,   0,   0,  75,   6, 178,   0,
    105,  24, 147,   0,  78,   6, 179,   0,   6,   6,   6,   6,  78,  95,   0,   0,
    180, 181,  99,   0,   0,   0,   0,   0,  99, 163,   0,   0,   6, 182,   0,   0,
    183, 184,   0,  75,   0,   0,   0,   0,   6,  99,  99, 185,   0,   0,   0,   0,
      6,   6, 128,   0,   0,   0,   0,   0,   6,   6, 186,  50,   6,  65,  24, 187,
      6, 188,   0,   0,   6,   6, 150,   0,   0,   0,   0,   0,   6,  97,  95,   0,
      6,   6,   6, 138,   0,   0,   0,   0,   6,   6,   6, 189,   0,   0,   0,   0,
      6, 138,   0,   0,   0,   0,   0,   0,   6, 190,   0,   0,   0,   0,   0,   0,
      6,   6, 191, 105, 192,   0,   0,   0, 193,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 194, 195, 196,   0,   0,   0,   0, 197,   0,   0,   0,   0,   0,
      6,   6, 188,   6, 198, 199, 200,   6, 201, 202, 203,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6, 204, 205,  80, 188, 188, 129, 129, 206, 206, 207,   6,
    200, 208, 209, 210, 211, 212,   0,   0,   6,   6,   6,   6,   6,   6, 113,   0,
      6,  88,   6,   6,   6,   6,   6,   6,  78,   0,   0,   0,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   6,  87,
};

static RE_UINT8 re_id_continue_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 254, 255, 255, 135, 254, 255, 255,   7,
      0,   4, 160,   4, 255, 255, 127, 255, 255, 255, 255, 255, 195, 255,   3,   0,
     31,  80,   0,   0, 255, 255, 223,  60, 192, 215, 255, 255, 251, 255, 255, 255,
    255, 255, 191, 255, 251, 252, 255, 255, 255,   0, 254, 255, 255, 255, 127,   2,
    254, 255, 255, 255, 255, 255, 255, 191, 182,   0, 255, 255, 255,   7,   7,   0,
      0,   0, 255,   7, 255, 195, 255, 255, 255, 255, 239, 159, 255, 253, 255, 159,
      0,   0, 255, 255, 255, 231, 255, 255, 255, 255,   3,   0, 255, 255,  63,   4,
    255,  63,   0,   0, 255, 255, 255,  15, 253,  31,   0,   0, 240, 255, 255, 127,
    207, 255, 254, 254, 238, 159, 249, 255, 255, 253, 197, 243, 159, 121, 128, 176,
    207, 255,   3,   0, 238, 135, 249, 255, 255, 253, 109, 211, 135,  57,   2,  94,
    192, 255,  63,   0, 238, 191, 251, 255, 255, 253, 237, 243, 191,  59,   1,   0,
    207, 255,   0,   0, 159,  57, 192, 176, 207, 255,   2,   0, 236, 199,  61, 214,
     24, 199, 255, 195, 199,  61, 129,   0, 192, 255,   0,   0, 238, 223, 253, 255,
    255, 253, 239, 227, 223,  61,  96,   3, 236, 223, 253, 255, 255, 253, 239, 243,
    223,  61,  96,  64, 207, 255,   6,   0, 255, 255, 255, 231, 223, 125, 128,   0,
    207, 255,   0, 252, 236, 255, 127, 252, 255, 255, 251,  47, 127, 132,  95, 255,
      0,   0,  12,   0, 255, 255, 255,   7, 255, 127, 255,   3, 150,  37, 240, 254,
    174, 236, 255,  59,  95,  63, 255, 243,   1,   0,   0,   3, 255,   3, 160, 194,
    255, 254, 255, 255, 255,  31, 254, 255, 223, 255, 255, 254, 255, 255, 255,  31,
     64,   0,   0,   0, 255,   3, 255, 255, 255, 255, 255,  63, 191,  32, 255, 255,
    255, 255, 255, 247, 255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,
     61, 255, 127, 255, 255, 255,  61, 255,   0, 254,   3,   0, 255, 255,   0,   0,
    255, 255,  31,   0, 255, 159, 255, 255, 255, 199,   1,   0, 255, 223,  31,   0,
    255, 255,  15,   0, 255, 223,  13,   0, 255, 255, 143,  48, 255,   3,   0,   0,
      0,  56, 255,   3, 255, 255, 255,   0, 255,   7, 255, 255, 255, 255,  63,   0,
    255,  15, 255,  15, 192, 255, 255, 255, 255,  63,  31,   0, 255,  15, 255, 255,
    255,   3, 255,   7, 255, 255, 255, 127, 255, 255, 255, 159, 255,   3, 255,   3,
    128,   0,   0,   0, 255,  15, 255,   3,   0, 248,  15,   0, 255, 227, 255, 255,
      0,   0, 247, 255, 255, 255, 127,   0, 127,   0,   0, 240, 255, 255,  63,  63,
     63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,
      0,   0,   0, 128,   1,   0,  16,   0,   0,   0,   2, 128,   0,   0, 255,  31,
    226, 255,   1,   0, 132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0,
    255,   1,   0,   0, 255, 127, 255, 255,  31, 248,  15,   0, 255, 128,   0, 128,
    127, 127, 127, 127, 224,   0,   0,   0, 254, 255,  62,  31, 255, 255, 127, 254,
    224, 255, 255, 255, 255,  63, 254, 255, 255, 127,   0,   0, 255,  31,   0,   0,
    255,  31, 255, 255, 255,  15,   0,   0, 255, 255, 240, 191, 255, 255, 255, 128,
      0,   0, 128, 255, 252, 255, 255, 255, 255, 121,  15,   0, 255,   7,   0,   0,
      0,   0,   0, 255, 255,   0,   0,   0,  31,   0, 255,   3, 255, 255, 255,   8,
    255,  63, 255, 255,   1, 128, 255,   3, 255,  63, 255,   3, 255, 255, 127,  12,
      7,   0,   0,  56, 255, 255, 124,   0, 126, 126, 126,   0, 127, 127,   0,   0,
    255,  55, 255,   3,  15,   0, 255, 255, 127, 248, 255, 255, 255, 255, 255,   3,
    127,   0, 248, 224, 255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255,
    255, 255, 252, 255,   0,   0, 255,  15, 127,   0,  24,   0,   0, 224,   0,   0,
      0,   0, 223, 255, 252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183,
    255,  63, 255,  63,   0,   0,   0,  32, 255, 255,   1,   0,  15, 255,  62,   0,
     63, 253, 255, 255, 255, 255, 191, 145, 255, 255, 255, 192, 111, 240, 239, 254,
    255, 255,  15, 135, 255, 255,   7,   0, 127,   0,   0,   0, 255,   1, 255,   3,
    255, 255, 223, 255,   7,   0,   0,   0, 255, 255, 255,   1,  31,   0, 255, 255,
      0, 128, 255, 255,   3,   0,   0,   0, 224, 227,   7, 248, 231,  15,   0,   0,
      0,  60,   0,   0,  28,   0,   0,   0, 255, 255, 255, 223, 100, 222, 255, 235,
    239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,
     63, 255, 255, 255, 253, 255, 255, 247, 255, 253, 255, 255, 247, 207, 255, 255,
    150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94, 255, 251, 255,  15,
    238, 251, 255,  15,
};

/* ID_Continue: 1894 bytes. */

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
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 23, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 24,  7, 25, 26, 13, 13, 13, 13, 13, 13, 13, 27,
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
    57, 58, 59, 60, 61, 31, 31, 31, 62, 63, 31, 31, 31, 31, 64, 31,
     1,  1,  1, 65, 66, 31, 31, 31,  1,  1,  1,  1, 67, 31, 31, 31,
     1,  1, 68, 31, 31, 31, 31, 69, 70, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 71, 72, 73, 74, 31, 31, 31, 31, 31, 31, 75, 31,
     1,  1,  1,  1,  1,  1, 76,  1,  1,  1,  1,  1,  1,  1,  1, 77,
    78, 31, 31, 31, 31, 31, 31, 31,  1,  1, 78, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_xid_start_stage_4[] = {
      0,   0,   1,   1,   0,   2,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   5,   6,   0,   0,   0,   7,   8,   9,   4,  10,
      4,   4,   4,   4,  11,   4,   4,   4,   4,  12,  13,  14,  15,   0,  16,  17,
      0,   4,  18,  19,   4,   4,  20,  21,  22,  23,  24,   4,   4,  25,  26,  27,
     28,  29,  30,   0,   0,  31,   0,   0,  32,  33,  34,  35,  36,  37,  38,  39,
     40,  41,  42,  43,  44,  45,  46,  47,  36,  45,  48,  49,  50,  51,  46,   0,
     52,  53,  54,  47,  52,  53,  55,  56,  52,  57,  58,  59,  60,  61,  62,   0,
     14,  63,  62,   0,  64,  65,  66,   0,  67,   0,  68,  69,  70,   0,   0,   0,
      4,  71,  72,  73,  74,   4,  75,  76,   4,   4,  77,   4,  78,  79,  80,   4,
     81,   4,  82,   0,  23,   4,   4,  83,  14,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,  84,   1,   4,   4,  85,  86,  87,  87,  88,   4,  89,  90,   0,
      0,   4,   4,  91,   4,  92,   4,  93,  94,   0,  16,  95,   4,  96,  97,   0,
     98,   4,  83,   0,   0,  99,   0,   0, 100,  89, 101,   0, 102, 103,   4, 104,
      4, 105, 106, 107,   0,   0,   0, 108,   4,   4,   4,   4,   4,   4,   0,   0,
    109,   4, 110, 107,   4, 111, 112, 113,   0,   0,   0, 114, 115,   0,   0,   0,
    116, 117, 118,   4, 119,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      4, 120, 121,   4,   4,   4,   4, 122,   4,  75,   4, 123,  98, 124, 124,   0,
    125, 126,  14,   4, 127,  14,   4,  76, 100, 128,   4,   4, 129,  82,   0,  16,
      4,   4,   4,   4,   4,  93,   0,   0,   4,   4,   4,   4,   4,   4,  69,   0,
      4,   4,   4,   4,  69,   0,  16, 107, 130, 131,   4, 132,  91,   4,   4,  23,
    133, 134,   4,   4, 135,  18,   0, 136, 137, 138,   4,  89, 134,  89,   0, 139,
     26, 140,  62,  94,  32, 141, 142,   0,   4, 119, 143, 144,   4, 145, 146, 147,
    148, 149,   0,   0,   0,   0,   4, 138,   4,   4,   4,   4,   4, 150, 151, 152,
      4,   4,   4, 153,   4,   4, 154,   0, 155, 156, 157,   4,   4,  87, 158,   4,
      4,   4, 107,  32,   4,   4,   4,   4,   4, 107,  16,   4, 159,   4,  15, 160,
      0,   0,   0, 161,   4,   4,   4,  94,   0,   1,   1, 162, 107, 121, 163,   0,
    164, 165, 166,   0,   4,   4,   4,  82,   0,   0,   4,  83,   0,   0,   0,   0,
      0,   0,   0,   0,  94,   4, 167,   0, 121,  16,  18,   0, 107,   4, 168,   0,
      4,   4,   4,   4, 107,   0,   0,   0, 169, 170,  93,   0,   0,   0,   0,   0,
     93, 154,   0,   0,   4, 171,   0,   0, 172,  89,   0,  94,   0,   0,   0,   0,
      4,  93,  93, 141,   0,   0,   0,   0,   4,   4, 119,   0,   0,   0,   0,   0,
    102,  91,   0,   0, 102,  23,  16, 119, 102,  62,   0,   0, 102, 141, 173,   0,
      0,   0,   0,   0,   4,  18,   0,   0,   4,   4,   4, 129,   0,   0,   0,   0,
      4,   4,   4, 138,   0,   0,   0,   0,   4, 129,   0,   0,   0,   0,   0,   0,
      4,  30,   0,   0,   0,   0,   0,   0,   4,   4, 174,   0, 158,   0,   0,   0,
     47,   0,   0,   0,   0,   0,   0,   0,   4,   4, 175,   4, 176, 177, 178,   4,
    179, 180, 181,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 182, 183,  76,
    175, 175, 120, 120, 184, 184, 143,   0, 178, 185, 186, 187, 188, 189,   0,   0,
      4,   4,   4,   4,   4,   4,  98,   0,   4,  83,   4,   4,   4,   4,   4,   4,
    107,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_xid_start_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,   0,   0, 223,  56,
     64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,   3, 252, 255, 255,
    255,   0, 254, 255, 255, 255, 127,   2, 254, 255, 255, 255, 255,   0,   0,   0,
      0,   0, 255, 255, 255,   7,   7,   0, 255,   7,   0,   0,   0, 192, 254, 255,
    255, 255,  47,   0,  96, 192,   0, 156,   0,   0, 253, 255, 255, 255,   0,   0,
      0, 224, 255, 255,  63,   0,   2,   0,   0, 252, 255, 255, 255,   7,  48,   4,
    255, 255,  63,   4,  16,   1,   0,   0, 255, 255, 255,   1, 253,  31,   0,   0,
    240, 255, 255, 255, 255, 255, 255,  35,   0,   0,   1, 255,   3,   0, 254, 254,
    224, 159, 249, 255, 255, 253, 197,  35,   0,  64,   0, 176,   3,   0,   3,   0,
    224, 135, 249, 255, 255, 253, 109,   3,   0,   0,   0,  94,   0,   0,  28,   0,
    224, 191, 251, 255, 255, 253, 237,  35,   0,   0,   1,   0,   3,   0,   0,   0,
      0,   0,   0, 176,   3,   0,   2,   0, 232, 199,  61, 214,  24, 199, 255,   3,
    224, 223, 253, 255, 255, 253, 239,  35,   0,   0,   0,   3,   0,   0,   0,  64,
      3,   0,   6,   0, 255, 255, 255,  39,   0,  64,   0,   0,   3,   0,   0, 252,
    224, 255, 127, 252, 255, 255, 251,  47, 127,   0,   0,   0, 255, 255,   5,   0,
    150,  37, 240, 254, 174, 236,   5,  32,  95,   0,   0, 240,   1,   0,   0,   0,
    255, 254, 255, 255, 255,  31,   0,   0,   0,  31,   0,   0, 255,   7,   0, 128,
      0,   0,  63,  60,  98, 192, 225, 255,   3,  64,   0,   0, 191,  32, 255, 255,
    255, 255, 255, 247, 255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,
     61, 255, 127, 255, 255, 255,  61, 255, 255, 255, 255,   7, 255, 255,  31,   0,
    255, 159, 255, 255, 255, 199,   1,   0, 255, 223,   3,   0, 255, 255,   3,   0,
    255, 223,   1,   0, 255, 255,  15,   0,   0,   0, 128,  16, 255, 255, 255,   0,
    255,   5, 255, 255, 255, 255,  63,   0, 255, 255, 255,  31, 255,  63,  31,   0,
    255,  15,   0,   0, 254,   0,   0,   0, 255, 255, 127,   0, 128,   0,   0,   0,
    224, 255, 255, 255, 224,  15,   0,   0, 248, 255, 255, 255,   1, 192,   0, 252,
     63,   0,   0,   0,  15,   0,   0,   0,   0, 224,   0, 252, 255, 255, 255,  63,
      0, 222,  99,   0, 255, 255,  63,  63,  63,  63, 255, 170, 255, 255, 223,  95,
    220,  31, 207,  15, 255,  31, 220,  31,   0,   0,   2, 128,   0,   0, 255,  31,
    132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0, 255,   1,   0,   0,
    255, 127, 255, 255, 255, 255, 255, 127,  31, 120,  12,   0, 255, 128,   0,   0,
    127, 127, 127, 127, 224,   0,   0,   0, 254,   3,  62,  31, 255, 255, 127, 224,
    255,  63, 254, 255, 255, 127,   0,   0, 255,  31, 255, 255,   0,  12,   0,   0,
    255, 127,   0, 128,   0,   0, 128, 255, 252, 255, 255, 255, 255, 121,  15,   0,
      0,   0,   0, 255, 187, 247, 255, 255,   7,   0,   0,   0,   0,   0, 252,   8,
     63,   0, 255, 255, 255, 255,   7,   0,   0, 128,   0,   0, 247,  15,   0,   0,
    255, 255, 127,   4, 255, 255,  98,  62,   5,   0,   0,  56, 255,   7,  28,   0,
    126, 126, 126,   0, 127, 127,   0,   0,  15,   0, 255, 255, 127, 248, 255, 255,
    255, 255, 255,  15, 255,  63, 255, 255, 255, 255, 255,   3, 127,   0, 248, 160,
    255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255, 255, 255, 252, 255,
      0,   0, 255,   3,   0,   0, 138, 170, 192, 255, 255, 255, 252, 252, 252,  28,
    255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63, 255, 255,   1,   0,
     15, 255,  62,   0,  63, 253, 255, 255, 255, 255, 191, 145, 255, 255, 255, 192,
      1,   0, 239, 254,  30,   0,   0,   0,  31,   0,   1,   0, 255, 255, 223, 255,
    255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223,
    255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255, 253, 255, 255, 247,
    255, 253, 255, 255, 150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94,
    255, 251, 255,  15, 238, 251, 255,  15,
};

/* XID_Start: 1761 bytes. */

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
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 23, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 24,  7, 25, 26, 13, 13, 13, 13, 13, 13, 13, 27,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    28, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
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
    57, 58, 59, 60, 61, 31, 31, 31, 62, 63, 31, 31, 31, 31, 64, 31,
     1,  1,  1, 65, 66, 31, 31, 31,  1,  1,  1,  1, 67, 31, 31, 31,
     1,  1, 68, 31, 31, 31, 31, 69, 70, 31, 31, 31, 31, 31, 31, 31,
    31, 71, 72, 31, 73, 74, 75, 76, 31, 31, 31, 31, 31, 31, 77, 31,
     1,  1,  1,  1,  1,  1, 78,  1,  1,  1,  1,  1,  1,  1,  1, 79,
    80, 31, 31, 31, 31, 31, 31, 31,  1,  1, 80, 31, 31, 31, 31, 31,
    31, 81, 31, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_xid_continue_stage_4[] = {
      0,   1,   2,   3,   0,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   7,   8,   6,   6,   6,   9,  10,  11,   6,  12,
      6,   6,   6,   6,  13,   6,   6,   6,   6,  14,  15,  16,  14,  17,  18,  19,
     20,   6,   6,  21,   6,   6,  22,  23,  24,   6,  25,   6,   6,  26,   6,  27,
      6,  28,  29,   0,   0,  30,   0,  31,   6,   6,   6,  32,  33,  34,  35,  36,
     37,  38,  39,  40,  41,  42,  43,  44,  33,  42,  45,  46,  47,  48,  49,  50,
     51,  52,  53,  44,  54,  55,  56,  57,  54,  58,  59,  60,  61,  62,  63,  64,
     16,  65,  66,   0,  67,  68,  69,   0,  70,  71,  72,  73,  74,  75,  76,   0,
      6,   6,  77,   6,  78,   6,  79,  80,   6,   6,  81,   6,  82,  83,  84,   6,
     85,   6,  58,  86,  87,   6,   6,  88,  16,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,  89,   3,   6,   6,  90,  91,  88,  92,  93,   6,   6,  94,  95,
     96,   6,   6,  97,   6,  98,   6,  99,  75, 100, 101, 102,   6, 103, 104,   0,
     29,   6, 105, 106, 107, 108,   0,   0,   6,   6, 109, 110,   6,   6,   6,  92,
      6,  97, 111,  78,   0,   0, 112, 113,   6,   6,   6,   6,   6,   6,   6, 114,
    115,   6, 116,  78,   6, 117, 118, 119,   0, 120, 121, 122, 123,   0, 123, 124,
    125, 126, 127,   6, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      6, 129, 105,   6,   6,   6,   6, 130,   6,  79,   6, 131, 113, 132, 132,   6,
    133, 134,  16,   6, 135,  16,   6,  80, 136, 137,   6,   6, 138,  65,   0,  24,
      6,   6,   6,   6,   6,  99,   0,   0,   6,   6,   6,   6,   6,   6, 139,   0,
      6,   6,   6,   6, 139,   0,  24,  78, 140, 141,   6, 142, 143,   6,   6,  26,
    144, 145,   6,   6, 146, 147,   0, 148,   6, 149,   6,  92,   6,   6, 150, 151,
      6, 152,  92,  75,   6,   6, 153,   0,   6, 113, 154, 155,   6,   6, 156, 157,
    158, 159,   0,   0,   0,   0,   6, 160,   6,   6,   6,   6,   6, 161, 162,  29,
      6,   6,   6, 152,   6,   6, 163,   0, 164, 165, 166,   6,   6,  26, 167,   6,
      6,   6,  78, 168,   6,   6,   6,   6,   6,  78,  24,   6, 169,   6, 149,   1,
     87, 170, 171, 172,   6,   6,   6,  75,   1,   2,   3, 101,   6, 105, 173,   0,
    174, 175, 176,   0,   6,   6,   6,  65,   0,   0,   6,  88,   0,   0,   0, 177,
      0,   0,   0,   0,  75,   6, 178,   0, 105,  24, 147,   0,  78,   6, 179,   0,
      6,   6,   6,   6,  78,  95,   0,   0, 180, 181,  99,   0,   0,   0,   0,   0,
     99, 163,   0,   0,   6, 182,   0,   0, 183, 184,   0,  75,   0,   0,   0,   0,
      6,  99,  99, 185,   0,   0,   0,   0,   6,   6, 128,   0,   0,   0,   0,   0,
      6,   6, 186,  50,   6,  65,  24, 187,   6, 188,   0,   0,   6,   6, 150,   0,
      0,   0,   0,   0,   6,  97,  95,   0,   6,   6,   6, 138,   0,   0,   0,   0,
      6,   6,   6, 189,   0,   0,   0,   0,   6, 138,   0,   0,   0,   0,   0,   0,
      6, 190,   0,   0,   0,   0,   0,   0,   6,   6, 191, 105, 192,   0,   0,   0,
    193,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 194, 195, 196,   0,   0,
      0,   0, 197,   0,   0,   0,   0,   0,   6,   6, 188,   6, 198, 199, 200,   6,
    201, 202, 203,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6, 204, 205,  80,
    188, 188, 129, 129, 206, 206, 207,   6, 200, 208, 209, 210, 211, 212,   0,   0,
      6,   6,   6,   6,   6,   6, 113,   0,   6,  88,   6,   6,   6,   6,   6,   6,
     78,   0,   0,   0,   0,   0,   0,   0,   6,   6,   6,   6,   6,   6,   6,  87,
};

static RE_UINT8 re_xid_continue_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 254, 255, 255, 135, 254, 255, 255,   7,
      0,   4, 160,   4, 255, 255, 127, 255, 255, 255, 255, 255, 195, 255,   3,   0,
     31,  80,   0,   0, 255, 255, 223,  56, 192, 215, 255, 255, 251, 255, 255, 255,
    255, 255, 191, 255, 251, 252, 255, 255, 255,   0, 254, 255, 255, 255, 127,   2,
    254, 255, 255, 255, 255, 255, 255, 191, 182,   0, 255, 255, 255,   7,   7,   0,
      0,   0, 255,   7, 255, 195, 255, 255, 255, 255, 239, 159, 255, 253, 255, 159,
      0,   0, 255, 255, 255, 231, 255, 255, 255, 255,   3,   0, 255, 255,  63,   4,
    255,  63,   0,   0, 255, 255, 255,  15, 253,  31,   0,   0, 240, 255, 255, 127,
    207, 255, 254, 254, 238, 159, 249, 255, 255, 253, 197, 243, 159, 121, 128, 176,
    207, 255,   3,   0, 238, 135, 249, 255, 255, 253, 109, 211, 135,  57,   2,  94,
    192, 255,  63,   0, 238, 191, 251, 255, 255, 253, 237, 243, 191,  59,   1,   0,
    207, 255,   0,   0, 159,  57, 192, 176, 207, 255,   2,   0, 236, 199,  61, 214,
     24, 199, 255, 195, 199,  61, 129,   0, 192, 255,   0,   0, 238, 223, 253, 255,
    255, 253, 239, 227, 223,  61,  96,   3, 236, 223, 253, 255, 255, 253, 239, 243,
    223,  61,  96,  64, 207, 255,   6,   0, 255, 255, 255, 231, 223, 125, 128,   0,
    207, 255,   0, 252, 236, 255, 127, 252, 255, 255, 251,  47, 127, 132,  95, 255,
      0,   0,  12,   0, 255, 255, 255,   7, 255, 127, 255,   3, 150,  37, 240, 254,
    174, 236, 255,  59,  95,  63, 255, 243,   1,   0,   0,   3, 255,   3, 160, 194,
    255, 254, 255, 255, 255,  31, 254, 255, 223, 255, 255, 254, 255, 255, 255,  31,
     64,   0,   0,   0, 255,   3, 255, 255, 255, 255, 255,  63, 191,  32, 255, 255,
    255, 255, 255, 247, 255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,
     61, 255, 127, 255, 255, 255,  61, 255,   0, 254,   3,   0, 255, 255,   0,   0,
    255, 255,  31,   0, 255, 159, 255, 255, 255, 199,   1,   0, 255, 223,  31,   0,
    255, 255,  15,   0, 255, 223,  13,   0, 255, 255, 143,  48, 255,   3,   0,   0,
      0,  56, 255,   3, 255, 255, 255,   0, 255,   7, 255, 255, 255, 255,  63,   0,
    255,  15, 255,  15, 192, 255, 255, 255, 255,  63,  31,   0, 255,  15, 255, 255,
    255,   3, 255,   7, 255, 255, 255, 127, 255, 255, 255, 159, 255,   3, 255,   3,
    128,   0,   0,   0, 255,  15, 255,   3,   0, 248,  15,   0, 255, 227, 255, 255,
      0,   0, 247, 255, 255, 255, 127,   0, 127,   0,   0, 240, 255, 255,  63,  63,
     63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,
      0,   0,   0, 128,   1,   0,  16,   0,   0,   0,   2, 128,   0,   0, 255,  31,
    226, 255,   1,   0, 132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0,
    255,   1,   0,   0, 255, 127, 255, 255,  31, 248,  15,   0, 255, 128,   0, 128,
    127, 127, 127, 127, 224,   0,   0,   0, 254, 255,  62,  31, 255, 255, 127, 230,
    224, 255, 255, 255, 255,  63, 254, 255, 255, 127,   0,   0, 255,  31,   0,   0,
    255,  31, 255, 255, 255,  15,   0,   0, 255, 255, 240, 191, 255, 255, 255, 128,
      0,   0, 128, 255, 252, 255, 255, 255, 255, 121,  15,   0, 255,   7,   0,   0,
      0,   0,   0, 255, 255,   0,   0,   0,  31,   0, 255,   3, 255, 255, 255,   8,
    255,  63, 255, 255,   1, 128, 255,   3, 255,  63, 255,   3, 255, 255, 127,  12,
      7,   0,   0,  56, 255, 255, 124,   0, 126, 126, 126,   0, 127, 127,   0,   0,
    255,  55, 255,   3,  15,   0, 255, 255, 127, 248, 255, 255, 255, 255, 255,   3,
    127,   0, 248, 224, 255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255,
    240, 255, 255, 255, 255, 255, 252, 255, 127,   0,  24,   0,   0, 224,   0,   0,
      0,   0, 138, 170, 252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183,
    255,  63, 255,  63,   0,   0,   0,  32, 255, 255,   1,   0,  15, 255,  62,   0,
     63, 253, 255, 255, 255, 255, 191, 145, 255, 255, 255, 192, 111, 240, 239, 254,
    255, 255,  15, 135, 255, 255,   7,   0, 127,   0,   0,   0, 255,   1, 255,   3,
    255, 255, 223, 255,   7,   0,   0,   0, 255, 255, 255,   1,  31,   0, 255, 255,
      0, 128, 255, 255,   3,   0,   0,   0, 224, 227,   7, 248, 231,  15,   0,   0,
      0,  60,   0,   0,  28,   0,   0,   0, 255, 255, 255, 223, 100, 222, 255, 235,
    239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,
     63, 255, 255, 255, 253, 255, 255, 247, 255, 253, 255, 255, 247, 207, 255, 255,
    150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94, 255, 251, 255,  15,
    238, 251, 255,  15,
};

/* XID_Continue: 1902 bytes. */

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
    0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 4, 4,
    4, 4,
};

static RE_UINT8 re_grapheme_extend_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  8,  9,  7,  7,  7,  7,  7,  7,  7,  7,  7, 10,
    11, 12, 13,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 15,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
    16,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
};

static RE_UINT8 re_grapheme_extend_stage_3[] = {
     0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13,
    14,  0,  0, 15,  0,  0,  0, 16, 17, 18, 19, 20, 21, 22,  0,  0,
    23,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 25,  0,  0,
    26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 27,  0, 28, 29, 30, 31,  0,  0,  0,  0,
     0,  0,  0, 32,  0,  0, 33, 34,  0, 35,  0,  0,  0,  0,  0,  0,
     0,  0, 36,  0,  0,  0,  0,  0, 37, 38,  0,  0,  0,  0, 39,  0,
     0,  0,  0,  0,  0,  0,  0, 40,  0, 41, 42,  0,  0,  0,  0,  0,
     0, 43,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_grapheme_extend_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   2,   0,   0,   0,   0,
      0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   4,   5,   6,   0,
      7,   0,   8,   9,   0,   0,  10,  11,  12,  13,  14,   0,   0,  15,   0,  16,
     17,  18,  19,   0,   0,   0,   0,  20,  21,  22,  23,  24,  25,  26,  27,  24,
     28,  29,  30,  31,  28,  29,  32,  24,  25,  33,  34,  24,  35,  36,  37,   0,
      0,  38,  39,  24,   0,  40,  41,  24,   0,  36,  27,  24,   0,   0,  42,   0,
      0,  43,  44,   0,   0,  45,  46,   0,  47,  48,   0,  49,  50,  51,  52,   0,
      0,  53,  54,  55,  56,   0,   0,   0,   0,   0,  57,   0,   0,   0,   0,   0,
     58,  58,  59,  59,   0,  60,  61,   0,  62,   0,   0,   0,   0,  63,   0,   0,
      0,  64,   0,   0,   0,   0,   0,   0,  65,   0,  66,  67,   0,   0,   0,   0,
     68,  69,  35,  16,  70,  71,   0,  72,   0,  73,   0,   0,   0,   0,  74,  75,
      0,   0,   0,   0,   0,   0,   1,  76,  77,   0,   0,   0,   0,   0,  13,  78,
      0,   0,   0,   0,   0,   0,   0,  79,   0,   0,   0,  80,   0,   0,   0,   1,
      0,  81,   0,   0,  82,   0,   0,   0,   0,   0,   0,  83,  80,   0,   0,  84,
     85,  86,   0,   0,   0,   0,  87,  88,   0,  89,  90,   0,  21,  91,   0,   0,
      0,  92,  93,   0,   0,  94,  25,  95,   0,   0,   0,   0,   0,   0,   0,  96,
     36,   0,   0,   0,   0,   0,   0,   0,   2,  97,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  38,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  98,
     99, 100,   0,   0,   0,   0,   0,   0,  25, 101,  97,   0,  70, 102,   0,   0,
     21, 103,   0,   0,  70, 104,   0,   0,   0,   0,   0,   0,   0, 105,   0,   0,
      0,   0,   0,   0, 106,   0,   0,   0,   0,   0,   0, 107, 108, 109,   0,   0,
      0,   0, 110,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   2,
};

static RE_UINT8 re_grapheme_extend_stage_5[] = {
      0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0, 248,   3,   0,   0,
      0,   0, 254, 255, 255, 255, 255, 191, 182,   0,   0,   0,   0,   0, 255,   7,
      0, 248, 255, 255,   0,   0,   1,   0,   0,   0, 192, 159, 159,  61,   0,   0,
      0,   0,   2,   0,   0,   0, 255, 255, 255,   7,   0,   0, 192, 255,   1,   0,
      0, 248,  15,   0,   0,   0, 192, 251, 239,  62,   0,   0,   0,   0,   0,  14,
    240, 255, 255, 127,   7,   0,   0,   0,   0,   0,   0,  20, 254,  33, 254,   0,
     12,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,  80,  30,  32, 128,   0,
      6,   0,   0,   0,   0,   0,   0,  16, 134,  57,   2,   0,   0,   0,  35,   0,
    190,  33,   0,   0,   0,   0,   0, 208,  30,  32, 192,   0,   4,   0,   0,   0,
      0,   0,   0,  64,   1,  32, 128,   0,   0,   0,   0, 192, 193,  61,  96,   0,
      0,   0,   0, 144,  68,  48,  96,   0,   0, 132,  92, 128,   0,   0, 242,   7,
    128, 127,   0,   0,   0,   0, 242,  27,   0,  63,   0,   0,   0,   0,   0,   3,
      0,   0, 160,   2,   0,   0, 254, 127, 223, 224, 255, 254, 255, 255, 255,  31,
     64,   0,   0,   0,   0, 224, 253, 102,   0,   0,   0, 195,   1,   0,  30,   0,
    100,  32,   0,  32,   0,   0,   0, 224,   0,   0,  28,   0,   0,   0,  12,   0,
      0,   0, 176,  63,  64, 254,  15,  32,   0,  56,   0,   0,   0,   2,   0,   0,
    135,   1,   4,  14,   0,   0, 128,   1,   0,   0,  64, 127, 229,  31, 248, 159,
     15,   0,   0,   0,   0,   0, 208,  23,   3,   0,   0,   0,  60,  11,   0,   0,
     64, 163,   3,   0,   0, 240, 207,   0,   0,   0, 247, 255, 253,  33,  16,   0,
    127,   0,   0, 240,   0,  48,   0,   0, 255, 255,   1,   0,   0, 128,   3,   0,
      0,   0,   0, 128,   0, 252,   0,   0,   0,   0,   0,   6,   0, 128, 247,  63,
      0,   0,   3,   0,  68,   8,   0,   0,  96,   0,   0,   0,  16,   0,   0,   0,
    255, 255,   3,   0, 192,  63,   0,   0, 128, 255,   3,   0,   0,   0, 200,  19,
      0, 126, 102,   0,   8,  16,   0,   0,   0,   0, 157, 193,   0,  48,  64,   0,
     32,  33,   0,   0, 127,   0,   0,   0,   0,   0,   0,  32, 110, 240,   0,   0,
      0,   0,   0, 135,   0,   0,   0, 255,   0,   0, 120,   6, 128, 239,  31,   0,
      0,   0, 192, 127,   0,  40, 191,   0,   0, 128,   7,   0, 160, 195,   7, 248,
    231,  15,   0,   0,   0,  60,   0,   0,  28,   0,   0,   0,
};

/* Grapheme_Extend: 1062 bytes. */

RE_UINT32 re_get_grapheme_extend(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_grapheme_extend_stage_1[f] << 4;
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
    13, 13, 13, 13, 13, 13, 21, 13, 13, 13, 22, 13, 13, 23, 24, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 25,  7, 26, 27, 13, 13, 13, 13, 13, 13, 13, 28,
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
    61, 62, 63, 64, 65, 50, 66, 50, 67, 68, 50, 50, 50, 50, 69, 50,
     1,  1,  1, 70, 71, 50, 50, 50,  1,  1,  1,  1, 72, 50, 50, 50,
     1,  1, 73, 50, 50, 50, 50, 74, 75, 50, 50, 50, 50, 50, 50, 50,
    76, 77, 78, 79, 80, 81, 82, 83, 50, 50, 50, 50, 50, 50, 84, 50,
    85, 86, 87, 88, 89, 90, 91, 92,  1,  1,  1,  1,  1,  1, 93,  1,
     1,  1,  1,  1,  1,  1,  1, 94, 95, 50, 50, 50, 50, 50, 50, 50,
     1,  1, 95, 50, 50, 50, 50, 50,
};

static RE_UINT8 re_grapheme_base_stage_4[] = {
      0,   1,   1,   2,   0,   3,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      0,   0,   0,   4,   5,   6,   1,   1,   1,   1,   1,   1,   7,   1,   1,   1,
      1,   8,   9,  10,  11,  12,  13,  14,  15,   1,  16,  17,   1,   1,  18,  19,
     20,  21,  22,   1,   1,  23,   1,  24,  25,  26,  27,   0,   0,  28,   0,   0,
     29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
     33,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
     56,  60,  61,  62,  63,  64,  65,  66,  10,  67,  68,   0,  69,  70,  71,   0,
     72,  73,  74,  75,  76,  77,  78,   0,   1,  79,  80,  81,  82,   1,  83,   1,
      1,   1,  84,   1,  85,  86,  87,   1,  88,   1,  89,  90,  91,   1,   1,  92,
      1,   1,   1,   1,  90,   1,   1,  93,  94,  95,  96,  97,   1,  98,  99, 100,
    101,   1,   1, 102,   1, 103,   1, 104,  90, 105, 106, 107,   1, 108, 109,   1,
    110,   1, 111, 112, 100, 113,   0,   0, 114, 115, 116, 117, 118, 119,   1, 120,
      1, 121, 122,   1,   0,   0, 123, 124,   1,   1,   1,   1,   1,   1,   0,   0,
    125,   1, 126, 127,   1, 128, 129, 130, 131, 132,   1, 133, 134,  89,   0,   0,
      1,   1,   1,   1, 135,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 136,
      1, 137,  16,   1,   1,   1,   1,   1,  10,   1,   1,   1,   1,   1,   1,   1,
      1,   1, 138,   0,   0,   0,   0,   0,   1, 139,   2,   1,   1,   1,   1, 140,
      1,  83,   1, 141, 142, 143, 143,   0,   1, 144,   0,   0, 145,   1,   1, 136,
      1,   1,   1,   1,   1,   1, 104, 146,   1, 135,  10,   1, 147,   1,   1,   1,
    148, 149,   1,   1, 139,  89,   1, 150,   2,   1,   1,   1,   1,   1,   1,   2,
      1,   1,   1,   1,   1, 104,   1,   1,   1,   1,   1,   1,   1,   1, 151,   0,
      1,   1,   1,   1, 152,   1, 153,   1,   1, 154,   1, 155, 102,   1,   1, 156,
      1,   1,   1,   1, 157,  16,   0, 158, 159, 160,   1, 102,   1,   1, 161, 162,
      1, 163, 164,  90,  29, 165, 166,   0,   1, 167, 168, 144,   1, 169, 170, 171,
    172, 173,   0,   0,   0,   0,   1, 174,   1,   1,   1,   1,   1, 150, 175, 144,
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1, 176,   1,   1,  91,   0,
    177, 178, 179,   1,   1,   1, 180,   1,   1,   1, 181,   1, 182,   1, 183, 184,
    185, 181, 186, 187,   1,   1,   1,  90,  10,   1,   1,   1, 127,   2, 188, 189,
    190, 191, 192,   0,   1,   1,   1,  89, 193, 194,   1,   1, 195,   0, 181,  90,
      0,   0,   0,   0,  90,   1,  93,   0,   2, 150,  16,   0, 196,   1, 197,   0,
      1,   1,   1,   1, 127, 198,   0,   0, 199, 200, 201,   0,   0,   0,   0,   0,
    202, 203,   0,   0,   1, 204,   0,   0, 205, 136, 206,   1,   0,   0,   0,   0,
      1, 207, 208, 209,   0,   0,   0,   0,   1,   1, 210,   0,   0,   0,   0,   0,
      0,   0,   0,   2,   0,   0,   0,   0, 211, 102, 212,  21, 118, 213, 214, 215,
     29, 216, 217,   0, 118, 218, 215,   0,   0,   0,   0,   0,   1, 219, 198,   0,
      1,   1,   1, 220,   0,   0,   0,   0,   1,   1,   1, 221,   0,   0,   0,   0,
      1, 220,   0,   0,   0,   0,   0,   0,   1, 222,   0,   0,   0,   0,   0,   0,
      1,   1, 223,   2, 224,   0,   0,   0, 225,   0,   0,   0,   0,   0,   0,   0,
      1,   1,   1,   1,   1,   1,   1, 104,   1, 226,   1, 227, 228, 229, 127,   0,
      1,   1, 230,   0,   0,   0,   0,   0,   1,   1, 142,  96,   0,   0,   0,   0,
      1,   1, 128,   1, 231, 232, 233,   1, 234, 235, 236,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1, 237,   1,   1,   1,   1,   1,   1,   1,   1, 238,   1,
    233, 239, 240, 241, 242, 243,   0, 244,   1, 108,   1,   1, 136, 245, 246,   0,
    131, 139,   1, 108,  89,   0,   0, 247, 248,  89, 249,   0,   0,   0,   0,   0,
      1, 250,   1,  90, 136,   1, 251,  93,   1,   2, 211,   1,   1,   1,   1, 252,
      1, 127, 150, 183,   0,   0,   0, 253,   1,   1, 254,   0,   1,   1, 255,   0,
      1,   1,   1, 136,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1, 142,   0,
      1,  92,   1,   1,   1,   1,   1,   1, 127,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_grapheme_base_stage_5[] = {
      0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 255, 223, 255, 255,
      0,   0, 255, 124, 240, 215, 255, 255, 251, 255, 255, 255,   7, 252, 255, 255,
    255,   0, 254, 255, 255, 255, 127, 254, 254, 255, 255, 255, 255, 134,   0,   0,
      0,   0,   0,  64,  73,   0, 255, 255, 255,   7,  31,   0, 192, 255,   0, 200,
    255,   7,   0,   0, 255, 255, 254, 255, 255, 255,  63,  64,  96, 194, 255, 255,
    255,  63, 253, 255, 255, 255,   0,   0,   0, 224, 255, 255,  63,   0,   2,   0,
    255,   7, 240,   7, 255, 255,  63,   4,  16,   1, 255, 127, 255, 255, 255,  65,
    253,  31,   0,   0, 248, 255, 255, 255, 255, 255, 255, 235,   1, 222,   1, 255,
    243, 255, 255, 254, 236, 159, 249, 255, 255, 253, 197, 163, 129,  89,   0, 176,
    195, 255, 255,  15, 232, 135, 249, 255, 255, 253, 109, 195,   1,   0,   0,  94,
    192, 255,  28,   0, 232, 191, 251, 255, 255, 253, 237, 227,   1,  26,   1,   0,
    195, 255,   3,   0, 255, 253, 237,  35, 129,  25,   0, 176, 195, 255, 255,   0,
    232, 199,  61, 214,  24, 199, 255, 131, 198,  29,   1,   0, 192, 255, 255,   7,
    238, 223, 253, 255, 255, 253, 239,  35,  30,   0,   0,   3, 195, 255,   0, 255,
    236, 223, 253, 255, 255, 253, 239,  99, 155,  13,   0,  64, 195, 255,   6,   0,
    255, 255, 255, 167, 193,  93,   0,   0, 195, 255,  63, 254, 236, 255, 127, 252,
    255, 255, 251,  47, 127,   0,   3, 127,   0,   0,  28,   0, 255, 255,  13, 128,
    127, 128, 255,  15, 150,  37, 240, 254, 174, 236,  13,  32,  95,   0, 255, 243,
    255, 255, 255, 252, 255, 255,  95, 253, 255, 254, 255, 255, 255,  31,   0, 128,
     32,  31,   0,   0,   0,   0,   0, 192, 191, 223, 255,   7, 255,  31,   2, 153,
    255, 255, 255,  60, 254, 255, 225, 255, 155, 223, 255, 223, 191,  32, 255, 255,
    255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255,
    255, 255,  61, 255, 255, 255, 255,   7, 255, 255, 255,  31, 255, 255, 255,   3,
    255, 255,  31,   0, 255, 255,   1,   0, 255, 223,   3,   0, 255, 255,  99,   0,
    255, 255,   3,   0, 255, 223,   1,   0, 255, 255,  79, 192, 191,   1, 240,  31,
    255,   3, 255,   3, 255,  71, 255,   3, 255, 255, 255,   0, 255,   5, 255, 255,
    255, 255,  63,   0, 120,  14, 251,   1, 241, 255, 255, 255, 255,  63,  31,   0,
    255,  15, 255, 255, 255,   3, 255, 199, 255, 255, 127, 206, 255, 255, 191,   0,
     26, 224,   7,   0, 255,  63,   0,   0, 240, 255, 255, 255, 255, 255,  47, 232,
    251,  15, 255, 255, 255,   7, 240,  31, 252, 255, 255, 255, 195, 244, 255, 255,
    191,  92,  12, 240, 255,  15,  48, 248, 255, 227, 255, 255, 255,   0,   8,   0,
      2, 222, 111,   0, 255, 255,  63,  63,  63,  63, 255, 170, 255, 255, 255,  63,
    255, 255, 223, 255, 223, 255, 207, 239, 255, 255, 220, 127, 255,   7, 255, 255,
    255, 128, 255, 255,   0,   0, 243, 255, 255, 127, 255,  31, 255,   3, 255, 255,
    255, 255,  15,   0, 127,   0,   0,   0, 255,  31, 255,   3, 255, 127, 255, 255,
    255, 127,  12, 254, 255, 128,   1,   0, 255, 255, 127,   0, 127, 127, 127, 127,
    255, 255, 255,  15, 255, 255, 255, 251,   0,   0, 255,  15, 255, 255, 127, 248,
    224, 255, 255, 255, 255,  63, 254, 255,  15,   0, 255, 255, 255,  31,   0,   0,
    255,  31, 255, 255, 127,   0, 255, 255, 255,  15,   0,   0, 255, 127,   8, 192,
    255, 255, 252,   0, 255, 127,  15,   0,   0,   0,   0, 255, 187, 247, 255, 255,
    159,  15, 255,   3,  15, 192, 255,   3,   0,   0, 252,  15,  63, 192, 255, 255,
    127,   0,  12, 128, 255, 255,  55, 236, 255, 191, 255, 195, 255, 129,  25,   0,
    247,  47, 255, 243, 255, 255,  98,  62,   5,   0,   0, 248, 255, 207,  63,   0,
    126, 126, 126,   0, 127, 127,   0,   0, 223,  30, 255,   3, 127, 248, 255, 255,
    255,  63, 255, 255, 127,   0, 248, 160, 255, 255, 127,  95, 219, 255, 255, 255,
      3,   0, 248, 255,   0,   0, 255, 255, 255, 255, 252, 255, 255,   0,   0,   0,
      0,   0, 255,  63,   0,   0, 255,   3, 255, 255, 247, 255, 127,  15, 223, 255,
    252, 252, 252,  28, 127, 127,   0,  48, 255, 239, 255, 255, 127, 255, 255, 183,
    255,  63, 255,  63, 135, 255, 255, 255, 255, 255, 143, 255, 255,   7, 255,  15,
    255, 255, 255, 191,  15, 255,  63,   0, 255,   3,   0,   0,  63, 253, 255, 255,
    255, 255, 191, 145, 255, 255, 191, 255, 255, 255, 255, 143, 255, 255, 255, 131,
    255, 255, 255, 192,   1,   0, 239, 254, 255,   0, 255,   1, 255, 255,  63, 254,
    255, 255,  63, 255, 255, 255,   7, 255, 255,   1,   0,   0, 253, 255, 255, 255,
    128,  63, 252, 255, 255, 255, 135, 217,   3,   0, 255, 255, 255,   1, 255,   3,
    127,  16, 192, 255,  15,   0,   0,   0, 255, 255,  63, 128, 255, 215,  64,   0,
    255, 127,   0,   0,   7,   0,  15,   0, 255, 255, 255,   1,  31,   0, 255, 255,
      0,   0, 248, 255,   3,   0,   0,   0, 127, 254, 255, 255,  95,  60,   0,   0,
     24, 240, 255, 255, 255, 195, 255, 255,  35,   0,   0,   0, 255, 255, 255, 223,
    100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,
     95, 252, 253, 255,  63, 255, 255, 255, 255, 207, 255, 255, 150, 254, 247,  10,
    132, 234, 150, 170, 150, 247, 247,  94, 255, 251, 255,  15, 238, 251, 255,  15,
      0,   0,   3,   0, 255, 127, 254, 127, 254, 255, 254, 255, 192, 255, 255, 255,
      7,   0, 255, 255, 255,   1,   3,   0,   1,   0, 191, 255, 223,   7,   0,   0,
    255, 255, 255,  30,   0,   0,   0, 248, 225, 255,   0,   0,  63,   0,   0,   0,
};

/* Grapheme_Base: 2169 bytes. */

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
     0,  0,  0,  0,  0,  9,  0,  0, 10, 11, 12, 13,  0,  0,  0,  0,
     0,  0, 14,  0,  0,  0,  0,  0, 15, 16,  0,  0,  0,  0, 17,  0,
};

static RE_UINT8 re_grapheme_link_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,
     0,  0,  1,  0,  0,  0,  2,  0,  0,  3,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  4,  0,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,
     6,  6,  0,  0,  0,  0,  7,  0,  0,  0,  0,  8,  0,  0,  0,  0,
     0,  0,  4,  0,  0,  9,  0, 10,  0,  0,  0, 11,  0,  0,  0,  0,
    12,  0,  0,  0,  0,  0,  4,  0,  0,  0, 13,  0,  0,  0,  8,  0,
     0,  0,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,  0,  0,  1,
     0, 11,  0,  0,  0,  0,  0,  0,  0,  0, 12,  0,  0, 15,  0,  0,
     0, 16,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0, 14,  0,  0,
};

static RE_UINT8 re_grapheme_link_stage_5[] = {
      0,   0,   0,   0,   0,  32,   0,   0,   0,   4,   0,   0,   0,   0,   0,   4,
     16,   0,   0,   0,   0,   0,   0,   6,   0,   0,  16,   0,   0,   0,   4,   0,
      1,   0,   0,   0,   0,  12,   0,   0,   0,   0,  12,   0,   0,   0,   0, 128,
     64,   0,   0,   0,   0,   0,   8,   0,   0,   0,  64,   0,   0,   0,   0,   2,
      0,   0,  24,   0,
};

/* Grapheme_Link: 374 bytes. */

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
      0,   0,   4,   0,   0,   0,   0,   0,   0,   0, 128,   4,   0,   0,   0,  12,
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
    39,  1, 40,  1,  1,  1, 41,  1, 42, 43, 44, 45,  1,  1,  1,  1,
    46,  1,  1,  1,  1,  1,  1,  1,
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
     0,  0, 25,  0,  0,  0, 33,  0,  0,  0, 34, 35,  0,  0,  0, 36,
     0,  0, 37,  0,  1,  0,  0, 38, 34,  0, 39,  0,  0,  0, 40,  0,
    34,  0,  0,  0,  0, 41,  0,  0,  0,  0, 42,  0,  0, 23, 43,  0,
     0,  0, 44,  0,  0,  0, 45,  0,  0,  0,  0, 46,
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
      0, 128,   0,   0,   0,   0,   0, 224,   0,   0,   0, 128,   0,   0,   3,   0,
      0,   8,   0,   0,   0,   0, 247,   0,  18,   0,   0,   0,   0,   0,   1,   0,
      0,   0, 128,   0,   0,   0,   0, 252, 128,  63,   0,   0,   3,   0,   0,   0,
     14,   0,   0,   0,  96,   0,   0,   0,   0,   0,  15,   0,
};

/* Terminal_Punctuation: 676 bytes. */

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
    1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 7, 1, 1, 1, 1,
};

static RE_UINT8 re_other_math_stage_3[] = {
     0,  1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  1,
     3,  4,  5,  6,  1,  1,  1,  7,  1,  1,  1,  8,  9,  1,  1, 10,
     1,  1,  1, 11,  1,  1,  1,  1,  1,  1,  1,  1, 12,  1, 13,  1,
    14, 15, 16, 17, 17, 18, 19, 20,  1,  1,  1,  1, 21, 22,  1,  1,
};

static RE_UINT8 re_other_math_stage_4[] = {
     0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  2,  3,  4,  5,  6,  7,
     8,  0,  9, 10, 11, 12, 13,  0, 14, 15, 16, 17,  0, 18, 19, 20,
     0, 21, 22, 23, 24,  0, 25, 26,  0,  0, 24, 27, 28,  0, 29, 30,
     0,  0,  0, 31,  0, 32,  0,  0, 33, 33, 34, 33, 35, 36, 37, 33,
    38, 39, 40, 33, 33, 33, 33, 33, 33, 41, 42, 43, 34, 34, 44, 44,
    45, 45, 46, 33, 37, 47, 48, 49, 50, 51,  0,  0,
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
    255, 127, 255, 255, 255, 253, 255, 255, 247, 207, 255, 255, 150, 254, 247,  10,
    132, 234, 150, 170, 150, 247, 247,  94, 255, 251, 255,  15, 238, 251, 255,  15,
};

/* Other_Math: 496 bytes. */

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
     6,  6,  6,  6,  7,  8,  6,  6,  6,  6,  6,  6,  6,  6,  6,  9,
     6, 10, 11,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 12,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_other_alphabetic_stage_3[] = {
     0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
    13,  0,  0, 14,  0,  0,  0, 15, 16, 17, 18, 19, 20,  0,  0,  0,
     0,  0,  0,  0, 21,  0,  0,  0,  0,  0,  0,  0,  0, 22,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23,  0,
    24, 25, 26, 27,  0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,
     0,  0, 29,  0,  0,  0,  0,  0, 30, 31,  0,  0,  0,  0, 32,  0,
     0,  0,  0,  0,  0,  0,  0, 33,
};

static RE_UINT8 re_other_alphabetic_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  2,  3,  0,  4,  0,  5,  6,  0,  0,  7,  8,
     9, 10,  0,  0,  0, 11,  0,  0, 12, 13,  0,  0,  0,  0,  0, 14,
    15, 16, 17, 18, 19, 20, 21, 18, 19, 20, 22, 23, 19, 20, 24, 18,
    19, 20, 25, 18, 26, 20, 27,  0, 19, 20, 28, 18, 18, 20, 28, 18,
    18, 20, 29, 18, 18,  0, 30, 31,  0, 32, 33,  0,  0, 34, 33,  0,
     0,  0,  0, 35, 36, 37,  0,  0,  0, 38, 39, 40, 41,  0,  0,  0,
     0,  0, 42,  0,  0,  0,  0,  0, 31, 31, 31, 31,  0, 43, 44,  0,
     0,  0,  0,  0,  0, 45,  0,  0,  0, 46,  0,  0,  0, 10, 47,  0,
    48,  0, 49, 50,  0,  0,  0,  0, 51, 52, 15,  0, 53, 54,  0, 55,
     0, 56,  0,  0,  0,  0,  0, 31,  0,  0,  0,  0,  0, 43, 57, 58,
     0,  0,  0,  0,  0,  0,  0, 57,  0,  0,  0, 59, 42,  0,  0,  0,
     0, 60,  0,  0, 61, 62, 15,  0,  0, 63, 64,  0, 15, 62,  0,  0,
     0, 65, 66,  0,  0, 67,  0, 68,  0,  0,  0,  0,  0,  0,  0, 69,
    70,  0,  0,  0,  0,  0,  0,  0, 71,  0,  0,  0,  0,  0,  0,  0,
    53, 72, 73,  0, 26, 74,  0,  0, 53, 64,  0,  0, 53, 75,  0,  0,
     0,  0,  0,  0,  0, 76,  0,  0,  0,  0, 35, 77,  0,  0,  0,  0,
};

static RE_UINT8 re_other_alphabetic_stage_5[] = {
      0,   0,   0,   0,  32,   0,   0,   0,   0,   0, 255, 191, 182,   0,   0,   0,
      0,   0, 255,   7,   0, 248, 255, 254,   0,   0,   1,   0,   0,   0, 192,  31,
    158,  33,   0,   0,   0,   0,   2,   0,   0,   0, 255, 255, 192, 255,   1,   0,
      0,   0, 192, 248, 239,  30,   0,   0, 240,   3, 255, 127,  15,   0,   0,   0,
      0,   0,   0, 204, 255, 223, 224,   0,  12,   0,   0,   0,  14,   0,   0,   0,
      0,   0,   0, 192, 159,  25, 128,   0, 135,  25,   2,   0,   0,   0,  35,   0,
    191,  27,   0,   0, 159,  25, 192,   0,   4,   0,   0,   0, 199,  29, 128,   0,
    223,  29,  96,   0, 223,  29, 128,   0,   0, 128,  95, 255,   0,   0,  12,   0,
      0,   0, 242,   7,   0,  32,   0,   0,   0,   0, 242,  27,   0,   0, 254, 255,
      3, 224, 255, 254, 255, 255, 255,  31,   0, 248, 127, 121,   0,   0, 192, 195,
    133,   1,  30,   0, 124,   0,   0,  48,   0,   0,   0, 128,   0,   0, 192, 255,
    255,   1,   0,   0,   0,   2,   0,   0, 255,  15, 255,   1,   1,   3,   0,   0,
      0,   0, 128,  15,   0,   0, 224, 127, 254, 255,  31,   0,  31,   0,   0,   0,
      0,   0, 224, 255,   7,   0,   0,   0, 254,  51,   0,   0, 128, 255,   3,   0,
    240, 255,  63,   0, 255, 255, 255, 255, 255,   3,   0,   0,   0,   0, 240,  15,
    248,   0,   0,   0,   3,   0,   0,   0,   0,   0, 240, 255, 192,   7,   0,   0,
    128, 255,   7,   0,   0, 254, 127,   0,   8,  48,   0,   0,   0,   0, 157,  65,
      0, 248,  32,   0, 248,   7,   0,   0,   0,   0,   0,  64, 110, 240,   0,   0,
      0,   0,   0, 255,  63,   0,   0,   0,   0,   0, 255,   1,   0,   0, 248, 255,
      0, 248,  63,   0, 255, 255, 255, 127,
};

/* Other_Alphabetic: 786 bytes. */

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
    2, 5, 2, 6, 0, 0, 0, 0, 2, 2, 2, 7, 2, 2, 2, 2,
    2, 2, 2, 2, 8, 2, 2, 2, 9, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_ideographic_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0, 192,   0,   0,   0, 254,   3,   0,   7,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  63,   0,
    255,  31,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255,  63, 255, 255,
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
     4,  4, 10,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 11,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
};

static RE_UINT8 re_diacritic_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1,  1,  1,  1,  1,  1, 17,  1, 18, 19, 20, 21, 22,  1, 23,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 24,  1, 25,  1,
    26,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 27, 28,
    29, 30, 31, 32,  1,  1,  1,  1,  1,  1,  1, 33,  1,  1, 34, 35,
    36, 37,  1,  1,  1,  1, 38,  1,  1,  1,  1,  1,  1,  1,  1, 39,
     1, 40,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_diacritic_stage_4[] = {
     0,  0,  1,  2,  0,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  4,  5,  5,  5,  5,  6,  7,  8,  0,  0,  0,
     0,  0,  0,  0,  9,  0,  0,  0,  0,  0, 10,  0, 11, 12, 13,  0,
     0,  0, 14,  0,  0,  0, 15, 16,  0,  4, 17,  0,  0, 18,  0, 19,
    20,  0,  0,  0,  0,  0,  0, 21,  0, 22, 23, 24,  0, 22, 25,  0,
     0, 22, 25,  0,  0, 22, 25,  0,  0, 22, 25,  0,  0,  0, 25,  0,
     0,  0, 25,  0,  0, 22, 25,  0,  0,  0, 25,  0,  0,  0, 26,  0,
     0,  0, 27,  0,  0,  0, 28,  0, 20, 29,  0,  0, 30,  0, 31,  0,
     0, 32,  0,  0, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,  0,
     0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,
     0, 37, 38, 39,  0, 40,  0,  0,  0, 41,  0, 42,  0,  0,  4, 43,
     0, 44,  5, 17,  0,  0, 45, 46,  0,  0,  0,  0,  0, 47, 48, 49,
     0,  0,  0,  0,  0,  0,  0, 50,  0, 51,  0,  0,  0,  0,  0,  0,
     0, 52,  0,  0, 53,  0,  0, 22,  0,  0,  0, 54,  0,  0,  0, 55,
    56, 57,  0,  0, 58,  0,  0, 20,  0,  0,  0,  0,  0,  0, 38, 59,
     0, 60, 61,  0,  0, 61,  2,  0,  0,  0,  0, 62,  0, 15, 63, 64,
     0,  0,  0,  0,  0,  0,  0, 65,  1,  0,  0,  0,  0,  0,  0,  0,
     0, 66,  0,  0,  0,  0,  0,  0,  0,  1,  2, 67, 68,  0,  0, 69,
     0,  0,  0,  0,  0, 70,  0,  0,  0, 71,  0,  0,  0,  0,  2,  0,
     0,  0,  0,  0,  0, 41,  0,  0,  0,  0,  0,  0, 72,  0,  0,  0,
     0,  0,  0, 73, 74, 75,  0,  0,
};

static RE_UINT8 re_diacritic_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,  64,   1,   0,   0,   0,   0, 129, 144,   1,
      0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 255, 224,   7,   0,  48,   4,
     48,   0,   0,   0, 248,   0,   0,   0,   0,   0,   0,   2,   0,   0, 254, 255,
    251, 255, 255, 191,  22,   0,   0,   0,   0, 248, 135,   1,   0,   0,   0, 128,
     97,  28,   0,   0, 255,   7,   0,   0, 192, 255,   1,   0,   0, 248,  63,   0,
      0,   0,   0,   3, 240, 255, 255, 127,   0,   0,   0,  16,   0,  32,  30,   0,
      0,   0,   2,   0,   0,  32,   0,   0,   0,   4,   0,   0, 128,  95,   0,   0,
      0,  31,   0,   0,   0,   0, 160, 194, 220,   0,   0,   0,  64,   0,   0,   0,
      0,   0, 128,   6, 128, 191,   0,  12,   0, 254,  15,  32,   0,   0,   0,  14,
      0,   0, 224, 159,   0,   0,  16,   0,  16,   0,   0,   0,   0, 248,  15,   0,
      0,  12,   0,   0,   0,   0, 192,   0,   0,   0,   0,  63, 255,  33,  16,   0,
      0, 240, 255, 255, 240, 255,   0,   0,   0,   0,   0, 224,   0,   0,   0, 160,
      3, 224,   0, 224,   0, 224,   0,  96,   0, 128,   3,   0,   0, 128,   0,   0,
      0, 252,   0,   0,   0,   0,   0,  30,   0, 128,   0, 176,   0,   0,   3,   0,
      0,   0, 128, 255,   3,   0,   0,   0,   0,   1,   0,   0, 255, 255,   3,   0,
      0, 120,   0,   0,   0,   0,   8,   0,   0,   0,   0,   8,   7,   0,   0,   0,
      0,   0,  64,   0,   0,  48,   0,   0, 127,   0,   0,   0,   0,   0,   1,   0,
      0,   0,   0, 192,   8,   0,   0,   0,   0,   0,   0,   6,   0,   0,  24,   0,
      0, 128, 255, 255, 128, 227,   7, 248, 231,  15,   0,   0,   0,  60,   0,   0,
};

/* Diacritic: 849 bytes. */

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
     6,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,
     0,  9,  0, 10,  0,  0,  0,  0, 11, 12,  0,  0, 13,  0,  0, 14,
    15,  0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 17,  0,  0,  0,  0, 18,  0,  0, 19, 20,
     0,  0,  0, 18,  0,  0,  0,  0,
};

static RE_UINT8 re_extender_stage_5[] = {
      0,   0,   0,   0,   0,   0, 128,   0,   0,   0,   3,   0,   1,   0,   0,   0,
      0,   0,   0,   4,  64,   0,   0,   0,   0,   4,   0,   0,   8,   0,   0,   0,
    128,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   8,  32,   0,   0,   0,
      0,   0,  62,   0,   0,   0,   0,  96,   0,   0,   0, 112,   0,   0,  32,   0,
      0,  16,   0,   0,   0, 128,   0,   0,   0,   0,   1,   0,   0,   0,   0,  32,
      0,   0,  24,   0,
};

/* Extender: 349 bytes. */

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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    4, 2, 5, 2, 2, 2, 6, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 7, 2, 2, 2, 2,
};

static RE_UINT8 re_other_lowercase_stage_4[] = {
     0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  2,  3,  0,  4,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  6,  7,  0,
     0,  8,  9,  0,  0, 10,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,
     0, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 13,  0, 14,
};

static RE_UINT8 re_other_lowercase_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   0,   4,
      0,   0,   0,   0,   0,   0, 255,   1,   3,   0,   0,   0,  31,   0,   0,   0,
     32,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0, 240, 255, 255,
    255, 255, 255, 255, 255,   7,   0,   1,   0,   0,   0, 248, 255, 255, 255, 255,
      0,   0,   0,   0,   0,   0,   2, 128,   0,   0, 255,  31,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255, 255,   0,   0, 255, 255, 255,   3,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  48,   0,   0,   0,   0,   0,   0,   1,   0,
      0,   0,   0,   0,   0,   0,   0,   3,
};

/* Other_Lowercase: 273 bytes. */

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
    0, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_other_grapheme_extend_stage_3[] = {
    0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 6, 7, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_grapheme_extend_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
    0, 0, 0, 0, 1, 2, 1, 2, 0, 0, 0, 3, 1, 2, 0, 4,
    5, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 8, 0, 0,
};

static RE_UINT8 re_other_grapheme_extend_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  64,
      0,   0, 128,   0,   0,   0,   0,   0,   4,   0,  96,   0,   0,   0,   0,   0,
      0, 128,   0, 128,   0,   0,   0,   0,   0,  48,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 192,   0,   0,   0,   0,   0, 192,   0,   0,   0,   0,
      0,   0,   0,   0,  32, 192,   7,   0,
};

/* Other_Grapheme_Extend: 249 bytes. */

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
    255, 255, 255, 255, 255, 255,  63,   0, 255,  31,   0,   0,   0,   0,   0,   0,
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
    0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 3, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6,
    7, 8, 8, 8, 8, 8, 8, 8,
};

static RE_UINT8 re_other_default_ignorable_code_point_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,
     0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  3,  0,
     0,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  0,
     0,  0,  0,  0,  0,  0,  6,  7,  8,  0,  9,  9,  0,  0,  0, 10,
     9,  9,  9,  9,  9,  9,  9,  9,
};

static RE_UINT8 re_other_default_ignorable_code_point_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 128,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,  48,   0,
      0,   0,   0,   0, 224,   3,   0,   0,   0,   0,   0,   0,  16,   0,   0,   0,
      0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255,   1,
    253, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255,
      0,   0,   0,   0,   0,   0, 255, 255,
};

/* Other_Default_Ignorable_Code_Point: 281 bytes. */

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
    24, 25, 26, 27, 28, 29,  1, 30,  1,  1,  1,  1, 31,  1, 32,  1,
     1,  1,  1,  1, 33,  1,  1,  1, 34, 35, 36, 37,  1,  1,  1,  1,
};

static RE_UINT8 re_sterm_stage_4[] = {
     0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  3,  0,  0,  0,
     4,  0,  0,  0,  0,  0,  5,  0,  6,  0,  0,  0,  0,  0,  0,  7,
     0,  0,  0,  8,  0,  0,  9,  0,  0,  0,  0, 10,  0,  0,  0, 11,
     0, 12,  0,  0, 13,  0,  0,  0,  0,  0,  8,  0,  0, 14,  0,  0,
     0,  0, 15,  0,  0, 16,  0, 17,  0, 18, 19,  0,  0, 11,  0,  0,
    20,  0,  0,  0,  0,  0,  0,  4, 21,  0,  0,  0,  0,  0,  0, 22,
     0,  0,  0, 23,  0,  0, 21,  0,  0, 24,  0,  0,  0,  0, 25,  0,
     0,  0, 26,  0,  0,  0,  0, 27,  0,  0,  0, 28,  0,  0, 29,  0,
     1,  0,  0, 30,  0,  0, 23,  0,  0,  0, 31,  0,  0, 17, 32,  0,
     0,  0, 33,  0,  0,  0, 34,  0,
};

static RE_UINT8 re_sterm_stage_5[] = {
      0,   0,   0,   0,   2,  64,   0, 128,   0,   0,   0,  80,   0,   2,   0,   0,
      0,   0,   0, 128,   0,   0,  16,   0,   7,   0,   0,   0,   0,   0,   0,   2,
     48,   0,   0,   0,   0,  12,   0,   0, 132,   1,   0,   0,   0,  64,   0,   0,
      0,   0,  96,   0,   8,   2,   0,   0,   0,  15,   0,   0,   0,   0,   0, 204,
      0,   0,   0,  24,   0,   0,   0, 192,   0,   0,   0,  48, 128,   3,   0,   0,
      4,   0,   0,   0,   0, 192,   0,   0,   0,   0, 136,   0,   0,   0, 192,   0,
      0, 128,   0,   0,   0,   3,   0,   0,   0,   0,   0, 224,   0,   0,   3,   0,
      0,   8,   0,   0,   0,   0, 196,   0,   2,   0,   0,   0, 128,   1,   0,   0,
      3,   0,   0,   0,  14,   0,   0,   0,  96,   0,   0,   0,
};

/* STerm: 568 bytes. */

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
     8,  9,  5,  5,  5,  5, 10,  5,  5,  5,  5,  5,  5, 11, 12, 13,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
    15,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,
};

static RE_UINT8 re_bidi_class_stage_2[] = {
      0,   1,   2,   2,   2,   3,   4,   5,   2,   6,   2,   7,   8,   9,  10,  11,
     12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
     28,  29,   2,   2,   2,   2,  30,  31,  32,   2,   2,   2,   2,  33,  34,  35,
     36,  37,  38,  39,  40,   2,  41,  42,  43,  44,   2,  45,   2,   2,   2,  46,
     47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  52,  52,  52,  57,  58,  52,
      2,   2,  52,  52,  52,  52,  59,   2,   2,  60,  61,  62,  63,  64,  52,  65,
     66,  67,   2,  68,  69,  70,  71,  72,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  73,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,  74,   2,   2,  75,  76,  77,  78,
     79,  80,  81,  82,  83,  84,   2,  85,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,  86,  87,  87,  87,  88,  89,  90,  91,  92,  93,
      2,   2,  94,  95,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
     96,  96,  97,  96,  98,  96,  99,  96,  96,  96,  96,  96, 100,  96,  96,  96,
    101, 102, 103, 104,   2,   2,   2,   2,   2,   2,   2,   2,   2, 105,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 106,
      2,   2, 107, 108, 109,   2, 110,   2,   2,   2,   2,   2,   2, 111, 112, 113,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
     96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  87, 114,  96,  96,
    115, 116, 117,   2,   2,   2, 118, 119, 120, 121, 122, 123, 124, 125, 126,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 127,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 127,
    128, 128, 129, 130, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
};

static RE_UINT8 re_bidi_class_stage_3[] = {
      0,   1,   2,   3,   4,   5,   4,   6,   7,   8,   9,  10,  11,  12,  11,  12,
     11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  13,  14,  14,  15,  16,
     17,  17,  17,  17,  17,  17,  17,  18,  19,  11,  11,  11,  11,  11,  11,  20,
     21,  11,  11,  11,  11,  11,  11,  11,  22,  23,  17,  24,  25,  26,  26,  26,
     27,  28,  29,  29,  30,  17,  31,  32,  29,  29,  29,  29,  29,  33,  34,  35,
     29,  36,  29,  17,  28,  29,  29,  29,  29,  29,  37,  32,  26,  26,  38,  39,
     26,  40,  41,  26,  26,  42,  26,  26,  26,  26,  29,  29,  29,  29,  43,  44,
     45,  11,  11,  46,  47,  48,  49,  11,  50,  11,  11,  51,  52,  11,  49,  53,
     54,  11,  11,  51,  55,  50,  11,  56,  54,  11,  11,  51,  57,  11,  49,  58,
     50,  11,  11,  59,  52,  60,  49,  11,  61,  11,  11,  11,  62,  11,  11,  63,
     11,  11,  11,  64,  65,  66,  49,  67,  11,  11,  11,  51,  68,  11,  49,  11,
     11,  11,  11,  11,  52,  11,  49,  11,  11,  11,  11,  11,  69,  70,  11,  11,
     11,  11,  11,  71,  72,  11,  11,  11,  11,  11,  11,  73,  74,  11,  11,  11,
     11,  75,  11,  76,  11,  11,  11,  77,  78,  79,  17,  80,  60,  11,  11,  11,
     11,  11,  81,  82,  11,  83,  84,  85,  86,  87,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  11,  81,  11,  11,  11,  88,  11,  11,  11,  11,  11,  11,
      4,  11,  11,  11,  11,  11,  11,  11,  89,  90,  11,  11,  11,  11,  11,  11,
     11,  91,  11,  91,  11,  49,  11,  49,  11,  11,  11,  92,  93,  94,  11,  88,
     95,  11,  11,  11,  11,  11,  11,  11,  11,  11,  96,  11,  11,  11,  11,  11,
     11,  11,  97,  98,  99,  11,  11,  11,  11,  11,  11,  11,  11, 100,  16,  16,
     11, 101,  11,  11,  11, 102, 103, 104, 105,  11,  11, 106,  61,  11, 107, 105,
    108,  11, 109,  11,  11,  11, 110, 108,  11,  11, 111, 112,  11,  11,  11,  11,
     11,  11,  11,  11,  11, 113, 114, 115,  11,  11,  11,  11,  17,  17, 116, 111,
     11,  11,  11, 117, 118, 119, 119, 120, 121,  16, 122, 123, 124, 125,   8, 126,
    127,  11, 128, 129,  11,  17,  17,  84, 130, 131, 132, 133, 134,  16,  11,  11,
    135,  16,  16,  16,  16,  16,  16,  16,  16, 136,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16, 137,  11,  11,  11,   5,
     16, 138,  16,  16,  16,  16,  16, 139,  16,  16, 140,  11, 141,  11,  16,  16,
    142, 143,  11,  11,  11,  11, 144,  16,  16,  16, 145,  16,  16,  16,  16,  16,
    146,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16, 147,  88,  11,  11,
     11,  11,  11,  11,  11,  11, 148, 149,  11,  11,  11,  11,  11,  11,  11, 150,
     11,  11,  11,  11,  11,  11,  17,  17,  16,  16,  16, 151,  11,  11,  11,  11,
     16, 152,  16,  16,  16,  16,  16, 139,  16,  16,  16,  16,  16, 137,  11, 151,
    153,  16, 154, 155,  11,  11,  11,  11,  11, 156,   4,  11,  11,  11,  11, 157,
     11,  11,  11,  11,  16,  16, 139,  11,  11, 120,  11,  11,  11,  16,  11, 158,
     11,  11,  11, 146, 159,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11, 160,
     11,  11,  11,  11,  11, 100,  11, 161,  11,  11,  11,  11,  16,  16,  16,  16,
     11,  16,  16,  16, 140,  11,  11,  11, 119,  11,  11,  11,  11,  11, 150, 162,
     11, 150,  11,  11,  11,  11,  11, 108,  16,  16, 163,  11,  11,  11,  11,  11,
    164,  11,  11,  11,  11,  11,  11,  11, 165,  11, 166, 167,  11,  11,  11, 168,
     11,  11,  11,  11, 115,  11,  17, 108,  11,  11, 169,  11, 170, 108,  11,  11,
     45,  11,  11, 171,  11,  11,  11,  11,  11,  11, 172, 173, 174,  11,  11,  11,
     11,  11,  11, 175,  50,  11,  68,  60,  11,  11,  11,  11,  11,  11, 176,  11,
     11, 177, 178,  26,  26,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,
     29,  29,  29, 179,  29,  29,  29,  29,  29,  29,  29,  29,  29,   8,   8, 180,
     17,  88, 116,  16,  16, 181, 182,  29,  29,  29,  29,  29,  29,  29,  29, 183,
    184,   3,   4,   5,   4,   5, 137,  11,  11,  11,  11,  11,  11,  11, 185, 186,
    187,  11,  11,  11,  16,  16,  16,  16, 141, 151,  11,  11,  11,  11,  11,  87,
     26,  26,  26,  26,  26,  26,  26,  26,  26, 188,  26,  26,  26,  26,  26,  26,
    189,  26,  26, 190,  26,  26,  26,  26,  26,  26,  26, 191,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26, 192, 193,  50,  11,  11, 194, 116,  14, 137,  11,
    108,  11,  11, 195,  11,  11,  11,  11,  45,  11, 196, 197,  11,  11,  11,  11,
    108,  11,  11, 198,  11,  11,  11,  11,  11,  11, 199, 200,  11,  11,  11,  11,
    150,  45,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11, 201, 202,
    203,  11, 204,  11,  11,  11,  11,  11,  16,  16,  16,  16, 205,  11,  11,  11,
     16,  16,  16,  16,  16, 140,  11,  11,  11,  11,  11,  11,  11, 157,  11,  11,
     11, 206,  11,  11, 161,  11,  11,  11, 135,  11,  11,  11, 207, 208, 208, 208,
     29,  29,  29,  29,  29,  29,  29, 209,  16,  16, 151,  16,  16,  16,  16,  16,
     16, 139, 210, 211, 146, 146,  11,  11, 212,  11,  11,  11,  11,  11, 133,  11,
     16,  16,   4, 213,  16,  16,  16, 147,  16, 139,  16,  16, 214,  11,  16,   4,
     16,  16,  16, 210, 215,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16, 216,
     16,  16,  16, 217, 139,  16, 218,  11,  11,  11,  11,  11,  11,  11,  11,   5,
     16,  16,  16,  16, 219,  11,  11,  11,  16,  16,  16,  16, 137,  11,  11,  11,
     16,  16,  16,  16,  16,  16,  16, 139,  11,  11,  11,  11,  11,  11,  11, 220,
      8,   8,   8,   8,   8,   8,   8,   8,  17,  17,  17,  17,  17,  17,  17,  17,
     17,  17,  17,  17,  17,  17,  17,   8,
};

static RE_UINT8 re_bidi_class_stage_4[] = {
      0,   0,   1,   2,   0,   0,   0,   3,   4,   5,   6,   7,   8,   8,   9,  10,
     11,  12,  12,  12,  12,  12,  13,  10,  12,  12,  13,  14,   0,  15,   0,   0,
      0,   0,   0,   0,  16,   5,  17,  18,  19,  20,  21,  10,  12,  12,  12,  12,
     12,  13,  12,  12,  12,  12,  22,  12,  23,  10,  10,  10,  12,  24,  10,  17,
     10,  10,  10,  10,  25,  25,  25,  25,  12,  26,  12,  27,  12,  17,  12,  12,
     12,  27,  12,  12,  28,  25,  29,  12,  12,  12,  27,  30,  31,  25,  25,  25,
     25,  25,  25,  32,  33,  32,  34,  34,  34,  34,  34,  34,  35,  36,  37,  38,
     25,  25,  39,  40,  40,  40,  40,  40,  40,  40,  41,  25,  35,  35,  42,  43,
     44,  40,  40,  40,  40,  45,  25,  46,  25,  47,  48,  49,   8,   8,  50,  40,
     51,  40,  40,  40,  40,  45,  25,  25,  34,  34,  52,  25,  25,  53,  54,  34,
     34,  55,  32,  25,  25,  31,  31,  56,  34,  34,  31,  34,  40,  25,  25,  25,
     25,  25,  25,  39,  57,  12,  12,  12,  12,  12,  58,  59,  60,  25,  59,  61,
     60,  25,  12,  12,  62,  12,  12,  12,  61,  12,  12,  12,  12,  12,  12,  59,
     60,  59,  12,  61,  63,  12,  30,  12,  64,  12,  12,  12,  64,  28,  65,  29,
     29,  61,  12,  12,  60,  66,  59,  61,  67,  12,  12,  12,  12,  12,  12,  65,
     12,  58,  12,  12,  58,  12,  12,  12,  59,  12,  12,  61,  13,  10,  68,  12,
     12,  12,  12,  62,  59,  62,  69,  29,  12,  64,  12,  12,  12,  12,  10,  70,
     12,  12,  12,  29,  12,  12,  58,  12,  62,  71,  12,  12,  61,  25,  57,  30,
     12,  28,  25,  57,  61,  25,  66,  59,  12,  12,  25,  29,  12,  12,  29,  12,
     12,  72,  73,  26,  60,  25,  25,  57,  25,  69,  12,  60,  25,  25,  60,  25,
     25,  25,  25,  59,  12,  12,  12,  60,  69,  25,  64,  64,  12,  12,  29,  62,
     59,  12,  12,  12,  60,  59,  12,  12,  58,  64,  12,  61,  12,  12,  12,  61,
     10,  10,  26,  12,  74,  12,  12,  12,  12,  12,  13,  11,  62,  59,  12,  12,
     12,  66,  25,  29,  12,  58,  60,  25,  25,  12,  30,  61,  10,  10,  75,  76,
     12,  12,  61,  12,  57,  28,  59,  12,  58,  12,  60,  12,  11,  26,  12,  12,
     12,  12,  12,  23,  12,  28,  59,  12,  12,  58,  25,  57,  71,  60,  25,  59,
     28,  25,  25,  65,  25,  12,  12,  12,  12,  69,  57,  59,  12,  12,  28,  25,
     29,  12,  12,  12,  62,  29,  66,  12,  12,  58,  29,  72,  12,  12,  12,  25,
     25,  62,  12,  12,  57,  25,  25,  25,  69,  25,  59,  61,  12,  59,  12,  12,
     25,  57,  12,  12,  12,  12,  12,  77,  26,  12,  12,  24,  12,  12,  12,  24,
     12,  12,  12,  22,  78,  78,  79,  80,  10,  10,  81,  82,  83,  84,  10,  10,
     10,  85,  10,  10,  10,  10,  10,  86,  87,   8,  88,  70,   8,   8,  88,  70,
     83,  83,  83,  83,  83,  83,  89,  12,  17,  70,  26,  12,  12,  20,  11,  23,
     10,  77,  90,  91,  12,  12,  23,  12,  10,  11,  23,  26,  12,  12,  90,  12,
     92,  10,  10,  10,  10,  26,  12,  12,  10,  20,  10,  10,  10,  12,  12,  12,
     10,  70,  12,  12,  10,  10,  70,  12,  10,  10,   8,   8,   8,   8,   8,  12,
     12,  12,  23,  10,  10,  10,  10,  24,  24,  10,  10,  10,  10,  10,  10,  11,
     12,  24,  70,  28,  29,  12,  24,  10,  12,  12,  12,  28,  10,  10,  10,  12,
     10,  10,  17,  10,  93,  11,  10,  10,  11,  12,  62,  29,  11,  23,  12,  24,
     12,  12,  94,  11,  12,  12,  13,  12,  12,  12,  12,  70,  12,  12,  12,  10,
     12,  13,  70,  12,  12,  12,  12,  13,  95,  25,  25,  96,  26,  12,  12,  12,
     12,  12,  11,  12,  58,  58,  28,  12,  12,  64,  10,  12,  12,  12,  97,  12,
     12,  10,  12,  12,  12,  62,  25,  29,  12,  28,  25,  25,  28,  62,  29,  59,
     12,  12,  60,  57,  64,  64,  12,  12,  28,  12,  12,  59,  69,  65,  59,  62,
     12,  61,  59,  61,  12,  12,  12,  98,  34,  34,  99,  34,  40,  40,  40, 100,
     40,  40,  40, 101, 102, 103,  10, 104, 105,  70, 106,  12,  40,  40,  40, 107,
    108,   5,   6,   7,   5, 109,  10,  70,   0,   0, 110, 111,  90,  12,  12,  12,
     34,  34,  34, 112,  31,  33,  34,  25,  34,  34, 113,  52,  34,  34, 114,  10,
     35,  35,  35,  35,  35,  35,  35, 115,  12,  12,  25,  25,  28,  57,  64,  12,
     12,  28,  25,  60,  25,  59,  12,  12,  12,  62,  25,  57,  12,  12,  28,  61,
     25,  66,  12,  12,  12,  28,  29,  12, 116,   0, 117,  25,  57,  60,  25,  12,
     12,  12,  62,  29, 118, 119,  12,  12,  12,  90,  12,  12,  13,  12,  12, 120,
      8,   8,   8,   8, 121,  40,  40,  40,  10,  10,  10,  70,  24,  10,  10,  70,
      8,   8, 122,  12,  10,  17,  10,  10,  10,  20,  70,  12,  20,  10,  10,  10,
     10,  10,  24,  11,  10,  10,  10,  26,  10,  10,  12,  12,  11,  24,  10,  10,
     12,  12,  12, 123,
};

static RE_UINT8 re_bidi_class_stage_5[] = {
    11, 11, 11, 11, 11,  8,  7,  8,  9,  7, 11, 11,  7,  7,  7,  8,
     9, 10, 10,  4,  4,  4, 10, 10, 10, 10, 10,  3,  6,  3,  6,  6,
     2,  2,  2,  2,  2,  2,  6, 10, 10, 10, 10, 10, 10,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 10, 10, 10, 10, 11, 11,  7, 11, 11,
     6, 10,  4,  4, 10, 10,  0, 10, 10, 11, 10, 10,  4,  4,  2,  2,
    10,  0, 10, 10, 10,  2,  0, 10,  0, 10, 10,  0,  0,  0, 10, 10,
     0, 10, 10, 10, 12, 12, 12, 12, 10, 10,  0,  0,  0,  0, 10,  0,
     0,  0,  0, 12, 12, 12,  0,  0,  0,  0,  0,  4,  1, 12, 12, 12,
    12, 12,  1, 12,  1, 12, 12,  1,  1,  1,  1,  1,  5,  5,  5,  5,
     5, 13, 10, 10, 13,  4,  4, 13,  6, 13, 10, 10, 12, 12, 12, 13,
    13, 13, 13, 13, 13, 13, 13, 12,  5,  5,  4,  5,  5, 13, 13, 13,
    12, 13, 13, 13, 13, 13, 12, 12, 12,  5, 10, 12, 12, 13, 13, 12,
    12, 10, 12, 12, 12, 12, 13, 13,  2,  2, 13, 13, 13, 12, 13, 13,
     1,  1,  1, 12,  1,  1, 10, 10, 10, 10,  1,  1,  1,  1, 12, 12,
    12, 12,  1,  1, 12, 12, 12,  0,  0,  0, 12,  0, 12,  0,  0,  0,
     0, 12, 12, 12,  0, 12,  0,  0,  0,  0, 12, 12,  0,  0,  4,  4,
     0, 12, 12,  0, 12,  0,  0, 12, 12, 12,  0, 12,  0,  4,  0,  0,
    10,  4, 10,  0, 12,  0, 12, 12, 10, 10, 10,  0, 12,  0, 12,  0,
     0, 12,  0, 12,  0, 12, 10, 10,  9,  0,  0,  0, 10, 10, 10, 12,
    12, 12,  9,  0,  0, 10,  0, 10,  9,  9,  9,  9,  9,  9,  9, 11,
    11, 11,  0,  1,  9,  7, 16, 17, 18, 14, 15,  6,  4,  4,  4,  4,
     4, 10, 10, 10,  6, 10, 10, 10, 10, 10, 10,  9,  2,  0,  0,  0,
     2,  2,  3,  3,  4,  4,  4,  0,  0, 10,  0,  0,  0,  0,  4,  0,
    10, 10,  3,  4,  9, 10, 10, 10,  0, 12, 12, 10, 12, 12, 12, 10,
    12, 12, 10, 10,  4,  4,  0,  0,  0,  1, 12,  1,  1,  3,  1,  1,
    13, 13, 10, 10, 13, 10, 13, 13,  6, 10,  6,  0, 10,  6, 10, 10,
    10, 10, 10,  4, 10, 10,  3,  3, 10,  4,  4, 10, 13, 13, 13, 11,
     0, 10, 10,  4, 10,  4,  4,  0, 11, 10, 10, 10, 10, 10, 11, 11,
     1,  1,  1, 10, 12, 12, 12,  1,  1, 10, 10, 10,  5,  5,  5,  1,
     0,  0,  0, 11, 11, 11, 11, 12, 10, 10, 12, 12, 12, 10,  0,  0,
     0,  0,  2,  2, 10, 10, 13, 13,  2,  2,  2,  0,  0,  0, 11, 11,
};

/* Bidi_Class: 3212 bytes. */

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
    36, 37,  0,  0,  0,  0, 38,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 39, 40,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_canonical_combining_class_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  0,  0,  0,  0,
     0,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  6,  7,  8,  0,
     9,  0, 10, 11,  0,  0, 12, 13, 14, 15, 16,  0,  0,  0,  0, 17,
    18, 19, 20,  0,  0,  0,  0, 21,  0, 22, 23,  0,  0, 22, 24,  0,
     0, 22, 24,  0,  0, 22, 24,  0,  0, 22, 24,  0,  0,  0, 24,  0,
     0,  0, 25,  0,  0, 22, 24,  0,  0,  0, 24,  0,  0,  0, 26,  0,
     0, 27, 28,  0,  0, 29, 30,  0, 31, 32,  0, 33, 34,  0, 35,  0,
     0, 36,  0,  0, 37,  0,  0,  0,  0,  0, 38,  0,  0,  0,  0,  0,
    39, 39,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0, 41,  0,  0,
     0, 42,  0,  0,  0,  0,  0,  0, 43,  0,  0, 44,  0,  0,  0,  0,
     0, 45, 46, 47,  0, 48,  0, 49,  0, 50,  0,  0,  0,  0, 51, 52,
     0,  0,  0,  0,  0,  0, 53, 54,  0,  0,  0,  0,  0,  0, 55, 56,
     0,  0,  0,  0,  0,  0,  0, 57,  0,  0,  0, 58,  0,  0,  0, 59,
     0, 60,  0,  0, 61,  0,  0,  0,  0,  0,  0, 62, 63,  0,  0, 64,
    65,  0,  0,  0,  0,  0, 46, 66,  0, 67, 68,  0,  0, 69, 70,  0,
     0,  0,  0,  0,  0, 71, 72, 73,  0,  0,  0,  0,  0,  0,  0, 24,
    74,  0,  0,  0,  0,  0,  0,  0,  0, 75,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 76, 77, 78,  0,  0,  0,  0,  0,  0,
     0,  0, 65,  0,  0, 79,  0,  0, 80, 81,  0,  0,  0,  0, 70,  0,
     0,  0,  0,  0,  0, 82,  0,  0,  0,  0,  0, 83, 84, 85,  0,  0,
     0,  0, 86,  0,  0,  0,  0,  0,
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
      0,   0,   0,   0,   0,   0,  50,   0,   0,  21,  43,  51,  52,  21,  35,  53,
      0,   0,   0,   0,   0,   0,   0,  54,   0,   0,   0,  55,  56,  57,   0,   0,
      0,   0,   0,  55,   0,   0,   0,   0,   0,   0,   0,  55,   0,  58,   0,   0,
      0,   0,  59,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  60,   0,
      0,   0,  61,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  62,   0,
      0,   0,  63,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  64,   0,
      0,   0,   0,   0,   0,  65,  66,   0,   0,   0,   0,   0,  67,  68,  69,  70,
     71,  72,   0,   0,   0,   0,   0,   0,   0,  73,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  74,  75,   0,   0,   0,   0,  76,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  48,   0,   0,   0,   0,   0,  77,   0,   0,
      0,   0,   0,   0,  59,   0,   0,  78,   0,   0,  79,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  80,   0,   0,   0,   0,   0,   0,  19,  81,   0,
     77,   0,   0,   0,   0,  48,   1,  82,   0,   0,   0,   0,   0,  54,   0,   0,
      0,  77,   0,   0,   0,   0,   0,   0,   0,   0,  19,  10,   1,   0,   0,   0,
      0,   0,  83,   0,   0,   0,   0,   0,   0,  84,   0,   0,  83,   0,   0,   0,
      0,   0,   0,   0,   0,  74,   0,   0,   0,   0,   0,   0,  53,   9,  12,   4,
     85,   8,  86,  76,   0,  57,   0,   0,  21,   1,  21,  87,  88,   1,   1,   1,
      1,  53,   0,   0,   0,   0,   0,  89,   0,   0,   0,   0,  90,   1,  91,  57,
     78,  92,  93,   4,  57,   0,   0,   0,   0,   0,   0,  19,  49,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  94,   1,   1,   1,   1,   1,   1,   1,   1,
      0,   0,  95,  96,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  97,   0,
      0,   0,   0,  19,   0,   1,   1,  49,   0,   0,   0,   0,   0,   0,   0,  19,
      0,   0,   0,   0,  49,   0,   0,   0,   0,  59,   0,   0,   0,   0,   0,   0,
      1,   1,   1,   1,  49,   0,   0,   0,   0,   0,  98,  64,   0,   0,   0,   0,
      0,   0,   0,   0,  94,   0,   0,   0,   0,   0,   0,   0,  74,   0,   0,   0,
     77,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99, 100,  57,  38,
     78,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  59,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 101,   1,  53,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  76,   0,   0,   0, 102,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 103,  94,   0,   0,   0,   0,   0,   0, 104,   0,
     53,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  94,  77,   0,   0,
      0,   0,   0,   0,   0, 105,   0,   0,   0, 106, 107, 108, 109,   0,  98,   4,
    110,  48,  23,   0,   0,   0,   0,   0,   0,   0,  38,  49,   0,   0,   0,   0,
     38,  57,   0,   0,   0,   0,   0,   0,
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
     0, 50, 50, 50, 50, 50,  0,  0,  0, 45, 45, 45, 50, 45, 45, 45,
    22, 23, 24, 50, 50, 50, 50,  0,  2,  0,  0,  0,  0,  4,  0,  0,
     0, 50, 45, 50, 50,  0,  0,  0,  0, 32, 33,  0,  0,  0,  4,  0,
    34, 34,  4,  0, 35, 35, 35, 35, 36, 36,  0,  0, 37, 37, 37, 37,
    45, 45,  0,  0,  0, 45,  0, 45,  0, 43,  0,  0,  0, 38, 39,  0,
    40,  0,  0,  0,  0,  0, 39, 39, 39, 39,  0,  0, 39,  0, 50, 50,
     4,  0, 50, 50,  0,  0, 45,  0,  0,  0,  0,  2,  0,  4,  4,  0,
     0, 45,  0,  0,  4,  0,  0,  0,  0, 50,  0,  0,  0, 49,  0,  0,
     0, 46, 50, 45, 45,  0,  0,  0, 50,  0,  0, 45,  0,  0,  4,  4,
     0,  0,  2,  0, 50,  0,  1,  1,  1,  0,  0,  0, 50, 53, 42, 45,
    41, 50, 50, 50, 52, 45, 50, 45, 50, 50,  1,  1,  1,  1,  1, 50,
     0,  1,  1, 50, 45, 50,  1,  1,  0,  0,  0,  4,  0,  0, 44, 49,
    51, 46, 47, 47,  0,  3,  3,  0,  0,  0,  0, 45, 50,  0, 50, 50,
    45,  0,  0, 50,  0,  0, 21,  0,  0, 45,  0, 50, 50,  1, 45,  0,
     0,  4,  2,  0,  0,  0,  4,  2,  0, 43, 43,  1,  1,  1,  0,  0,
     0, 48, 43, 43, 43, 43, 43,  0, 45, 45, 45,  0,
};

/* Canonical_Combining_Class: 1828 bytes. */

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
    43, 44,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7, 45,  7,  7, 46, 47, 48, 49,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 50,  7,
     7, 51, 52,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7, 35, 35, 53,  7,  7,  7,  7,  7,
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
      0,   0,   0,   0,   0,   0,   0, 146,   0,   0,   0,   0,   0,   0,   0, 147,
      6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   6,   6,   6,   6,  21,   0,   0,   0,   0,   0,
     81, 148, 149,   6,   6,   6,  81,   6,   6,   6,   6,   6,   6,  78,   0,   0,
    150, 151, 152, 153, 154, 155, 156, 156, 157, 156, 158, 159,   0, 160, 161, 162,
    163, 163, 163, 163, 163, 163, 164, 165, 165, 166, 167, 167, 167, 168, 169, 170,
    163, 171, 172, 173,   0, 174, 175, 176, 177, 178, 165, 179, 180,   0,   0, 181,
      0, 182,   0, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 192, 193, 194,
    195, 196, 196, 196, 196, 196, 197, 198, 198, 198, 198, 199, 200, 201, 202,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0, 203, 204,   0,   0,   0,   0,   0,
      0,   0, 205,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 205, 206,   0,   0,   0,   0, 207,  14,   0,   0,   0,
    208, 208, 208, 208, 208, 209, 208, 208, 208, 210, 211, 212, 213, 208, 208, 208,
    214, 215, 208, 216, 217, 218, 208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
    208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 219, 208, 208, 208, 208, 208,
    208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 220, 208, 208, 208,
    213, 208, 221, 222, 223, 224, 225, 226, 227, 228, 229, 228,   0,   0,   0,   0,
    230, 102, 231, 142, 142,   0, 232,   0,   0, 233,   0,   0,   0,   0,   0,   0,
    234, 142, 142, 235, 236, 237,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
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
     82,  81,  81,  81,  20,  20,  20,  83,  25,   0,   0,   0,   0,   0,  55,   0,
     36,  10,   8,  11,  36,  33,  13,   8,  20,  30,   0,   0,   3,  20,   0,  46,
     59,  59,  84,   8,   8,  11,   8,  36,   9,  22,   8,  15,  85,  86,  86,  86,
     86,  86,  86,  86,  86,  85,  85,  85,  87,  85,  86,  86,  88,   0,   0,   0,
     89,  90,  91,  92,  85,  87,  86,  85,  85,  85,  93,  87,  94,  94,  94,  94,
     94,  95,  95,  95,  95,  95,  95,  95,  95,  96,  97,  97,  97,  97,  97,  97,
     97,  97,  97,  98,  99,  99,  99,  99,  99, 100,  94,  94, 101,  95,  95,  95,
     95,  95,  95, 102,  97,  99,  99, 103, 104,  97, 105, 106, 107, 105, 108, 105,
    104,  96,  95, 105,  96, 109, 110,  97, 111, 106, 112, 105,  95, 106, 113,  95,
     96, 106,   0,   0,  94,  94,  94, 114, 115, 115, 116,   0, 115, 115, 115, 115,
    115, 117, 118,  20, 119, 120, 120, 120, 120, 119, 120,   0, 121, 122, 123, 123,
    124,  91, 125, 126,  90, 125, 127, 127, 127, 127, 126,  91, 125, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 126, 125, 126,  91, 128, 129, 130, 130, 130,
    130, 130, 130, 130, 131, 132, 132, 132, 132, 132, 132, 132, 132, 132, 132, 133,
    134, 132, 134, 132, 134, 132, 134, 135, 130, 136, 132, 133,   0,   0,  27,  19,
      0,   0,  18,   0,   0,   0,   0,  13,   8,  19,   0,   0,   0,   0,  18,   8,
     59,  59,  59,  59,  59, 137,  59,  59,  59,  59,  59, 137, 138, 139,  61, 137,
     59,  59,  66,  61,  59,  61,  59,  59,  59,  66, 140,  61,  59, 137,  59, 137,
     59,  59,  66, 140,  59, 141, 142,  59, 137,  59,  59,  59,  59,  62,  59,  59,
     59,  59,  59, 142, 139, 143,  61,  59, 140,  59, 144,   0, 138, 145, 144,  61,
    139, 143, 144, 144, 139, 143, 140,  59, 140,  59,  61, 141,  59,  59,  66,  59,
     59,  59,  59,   0,  61,  61,  66,  59,  20,  20,  30,   0,  20,  20, 146,  75,
      0,   0,   4,   0, 147,   0,   0,   0, 148,   0,   0,   0,  81,  81, 148,   0,
     20,  20,  35,   0, 149,   0,   0,   0,
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
     3,  3,  3,  0,  3,  0,  3,  0,  0,  0,  3,  3,  3,  0,  0,  3,
     0,  3,  0,  3,  0,  0,  0,  3,  2,  2,  2,  9, 16,  0,  0,  0,
    16, 16, 16,  0,  9,  9,  0,  0,
};

/* Decomposition_Type: 2872 bytes. */

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
     0,  0,  1,  1,  1,  1,  1,  2,  0,  0,  3,  4,  5,  6,  7,  8,
     9, 10, 11, 12, 13, 14, 11,  0,  0,  0,  0,  0, 15, 16,  0,  0,
     0,  0,  0,  0,  0,  9,  9,  0,  0,  0,  0,  0, 17, 18,  0,  0,
    19, 19, 19, 19, 19, 19, 19,  0,  0, 20, 21, 20, 21,  0,  0,  0,
     9, 19, 19, 19, 19,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    22, 22, 22, 22, 22, 22,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 23, 24, 25,  0,  0,  0, 26, 27,  0, 28,  0,  0,  0,  0,  0,
    29, 30, 31,  0,  0, 32, 33, 34, 35, 34,  0, 36,  0, 37, 38,  0,
    39, 40, 41, 42, 43, 44, 45,  0, 46, 47, 48, 49,  0,  0,  0,  0,
     0, 44, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 19, 19, 19, 19, 19, 19, 19, 19, 51, 19,
    19, 19, 19, 19, 33, 19, 19, 52, 19, 53, 21, 54, 55, 56, 57,  0,
    58, 59,  0,  0, 60,  0, 61,  0,  0, 62,  0, 62, 63, 19, 64, 19,
     0,  0,  0, 65,  0, 38,  0, 66,  0,  0,  0,  0,  0,  0, 67,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 68,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 69,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 22, 70, 22, 22, 22, 22, 22, 71,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 72,  0, 73,
    74, 22, 22, 75, 76, 22, 22, 22, 22, 77, 22, 22, 22, 22, 22, 22,
    78, 22, 79, 76, 22, 22, 22, 22, 75, 22, 22, 80, 22, 22, 71, 22,
    22, 75, 22, 22, 81, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 75,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,  0,  0,  0,  0,
    22, 22, 22, 22, 22, 22, 22, 22, 82, 22, 22, 22, 83,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 22, 82,  0,  0,  0,  0,  0,  0,  0,  0,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 71,  0,  0,  0,  0,  0,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 84,  0, 22, 22, 85, 86,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    87, 88, 88, 88, 88, 88, 89, 90, 90, 90, 90, 91, 92, 93, 94, 65,
    95,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    96, 19, 97, 19, 19, 19, 34, 19, 19, 96,  0,  0,  0,  0,  0,  0,
    98, 22, 22, 80, 99, 95,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 79,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,  0,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 97,
};

static RE_UINT8 re_east_asian_width_stage_4[] = {
     0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  2,  3,  4,  5,  6,
     7,  8,  9,  7,  0, 10,  0,  0, 11, 12, 11, 13, 14, 10,  9, 14,
     8, 12,  9,  5, 15,  0,  0,  0, 16,  0, 12,  0,  0, 13, 12,  0,
    17,  0, 11, 12,  9, 11,  7, 15, 13,  0,  0,  0,  0,  0,  0, 10,
     5,  5,  5, 11,  0, 18, 17, 15, 11,  0,  7, 16,  7,  7,  7,  7,
    17,  7,  7,  7, 19,  7, 14,  0, 20, 20, 20, 20, 18,  9, 14, 14,
     9,  7,  0,  0,  8, 15, 12, 10,  0, 11,  0, 12, 17, 11,  0,  0,
     0,  0, 21, 11, 12, 15, 15,  0, 12, 10,  0,  0, 22, 10, 12,  0,
    12, 11, 12,  9,  7,  7,  7,  0,  7,  7, 14,  0,  0,  0, 15,  0,
     0,  0, 14,  0, 10, 11,  0,  0,  0, 12,  0,  0,  8, 12, 18, 12,
    15, 15, 10, 17, 18, 16,  7,  5,  0,  7,  0, 14,  0,  0, 11, 11,
    10,  0,  0,  0, 14,  7, 13, 13, 13, 13,  0,  0,  0, 15, 15,  0,
     0, 15,  0,  0,  0,  0,  0, 12,  0,  0, 23,  0,  7,  7, 19,  7,
     7,  0,  0,  0, 13, 14,  0,  0, 13, 13,  0, 14, 14, 13, 18, 13,
    14,  0,  0,  0, 13, 14,  0, 12,  0, 22, 15, 13,  0, 14,  0,  5,
     5,  0,  0,  0, 19, 19,  9, 19,  0,  0,  0, 13,  0,  7,  7, 19,
    19,  0,  7,  7,  0,  0,  0, 15,  0, 13,  7,  7,  0, 24,  1, 25,
     0, 26,  0,  0,  0, 17, 14,  0, 20, 20, 27, 20, 20,  0,  0,  0,
    20, 28,  0,  0, 20, 20, 20,  0, 29, 20, 20, 20, 20, 20, 20, 30,
    31, 20, 20, 20, 20, 30, 31, 20,  0, 31, 20, 20, 20, 20, 20, 28,
    20, 20, 30,  0, 20, 20,  7,  7, 20, 20, 20, 32, 20, 30,  0,  0,
    20, 20, 28,  0, 30, 20, 20, 20, 20, 30, 20,  0, 33, 34, 34, 34,
    34, 34, 34, 34, 35, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 37,
    38, 36, 38, 36, 38, 36, 38, 39, 34, 40, 36, 37, 28,  0,  0,  0,
     7,  7,  9,  0,  7,  7,  7, 14, 30,  0,  0,  0, 20, 20, 32,  0,
};

static RE_UINT8 re_east_asian_width_stage_5[] = {
    0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 1, 5, 5,
    1, 5, 5, 1, 1, 0, 1, 0, 5, 1, 1, 5, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0,
    0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
    3, 3, 3, 3, 0, 2, 0, 0, 0, 1, 1, 0, 0, 3, 3, 0,
    0, 0, 5, 5, 5, 5, 0, 0, 0, 5, 5, 0, 3, 3, 0, 3,
    3, 3, 0, 0, 4, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3,
    3, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 0,
    4, 4, 4, 0,
};

/* East_Asian_Width: 1668 bytes. */

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
    1,
};

static RE_UINT8 re_joining_group_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_joining_group_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 0,
    0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_joining_group_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  0,  0,  0,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,  0,  0, 21,  0, 22,
     0,  0, 23, 24, 25, 26,  0,  0,  0, 27, 28, 29, 30, 31, 32, 33,
     0,  0,  0,  0, 34, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
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
     4,  0, 13, 39,  9, 29, 22, 24, 45, 45, 31, 43, 57,  0,  0,  0,
};

/* Joining_Group: 481 bytes. */

RE_UINT32 re_get_joining_group(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_joining_group_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_joining_group_stage_2[pos + f] << 5;
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
    0, 1, 2, 2, 2, 3, 2, 4, 5, 2, 2, 6, 2, 2, 7, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
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
    38, 39,  1,  1,  1,  1, 40,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 41,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 42, 43,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    44, 45,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
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
      0,  32,  33,   0,  34,  35,   0,   0,   0,   0,  36,   0,   0,   0,  37,  38,
     39,   0,   0,  40,  41,  42,  43,   0,  44,   0,   0,  45,  46,   0,  43,   0,
     47,   0,   0,  45,  48,  44,   0,  49,  47,   0,   0,  45,  50,   0,  43,   0,
     44,   0,   0,  51,  46,  52,  43,   0,  53,   0,   0,   0,  54,   0,   0,   0,
      0,   0,   0,  55,  56,  57,  43,   0,   0,   0,   0,  51,  58,   0,  43,   0,
      0,   0,   0,   0,  46,   0,  43,   0,   0,   0,   0,   0,  59,  60,   0,   0,
      0,   0,   0,  61,  62,   0,   0,   0,   0,   0,   0,  63,  64,   0,   0,   0,
      0,  65,   0,  66,   0,   0,   0,  67,  68,  69,   2,  70,  52,   0,   0,   0,
      0,   0,  71,  72,   0,  73,  28,  74,  75,   1,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  71,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,  76,   0,  76,   0,  43,   0,  43,   0,   0,   0,  77,  78,  79,   0,   0,
     80,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,
      0,   0,  82,  83,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,  84,   0,   0,   0,  85,  86,  87,   0,   0,   0,   0,   0,   0,   0,   0,
     88,   0,   0,  89,  53,   0,  90,  88,  91,   0,  92,   0,   0,   0,  93,  91,
      0,   0,  94,  95,   0,   0,   0,   0,   0,   0,   0,   0,   0,  96,  97,  98,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,  99,  94,
    100,   0, 101,   0,   0,   0, 102,   0,   0,   0,   0,   0,   0,   2,   2,  28,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  91,
      0,   0,   0,   0,   0,   0,   0,  20,   0,   0,   0,   0,   0,   0,   2,   2,
      0,   0, 103,   0,   0,   0,   0,   0,   0, 104,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  20, 105,   0,  20,   0,   0,   0,   0,   0,  91,
    106,   0,  57,   0,   0,   0,   0,   0,   0,   0,   0,   0,  98,   0,   2,  91,
      0,   0, 107,   0, 108,  91,   0,   0,  39,   0,   0, 109,   0,   0,   0,   0,
      0,   0, 110, 111, 112,   0,   0,   0,   0,   0,   0, 113,  44,   0, 114,  52,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 115,   0,
      0, 116,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      2,   0,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 117,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
    118,   0,   0, 119,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     44,   0,   0, 120,  99,   0,   0,   0,  91,   0,   0, 121,   0,   0,   0,   0,
     39,   0, 122, 123,   0,   0,   0,   0,  91,   0,   0, 124,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 125, 126,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  20,  39,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 127, 128, 129,   0, 103,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  76,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     44,   0,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,
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
     0, 36,  6,  2,  2,  5,  5,  4, 35, 32, 11, 12, 14, 40,  5,  0,
    41, 14, 24, 42,  0,  2,  2,  2,  2,  2,  2,  8,  8,  0,  0,  0,
     0,  0, 43, 37,  5,  2, 37,  1,  5,  2,  0,  0, 36,  0,  0,  0,
     1,  0,  0,  0,  0,  0,  0, 37,  5, 37,  0,  1,  7,  0,  0,  0,
     7,  3, 26,  4,  4,  1,  0,  0,  5,  6, 37,  1,  0,  0,  0, 26,
     0, 43,  0,  0, 43,  0,  0,  0, 37,  0,  0,  1,  0,  0,  0, 36,
    37, 36, 27,  4,  0,  7,  0,  0,  0, 43,  0,  4,  0,  0, 43,  0,
    36, 44,  0,  0,  1,  2,  8,  0,  0,  3,  2,  8,  1,  2,  6, 37,
     0,  0,  2,  4,  0,  0,  4,  0,  0, 45,  1,  0,  5,  2,  2,  8,
     2, 27,  0,  5,  2,  2,  5,  2,  2,  2,  2, 37,  0,  0,  0,  5,
    27,  2,  7,  7,  0,  0,  4, 36,  5, 37,  0,  0, 43,  7,  0,  1,
    36, 37,  0,  0,  0,  6,  2,  4,  0, 43,  5,  2,  2,  0,  0,  1,
     0,  0,  3,  4,  0,  0,  1,  0,  8,  3, 37,  0, 43,  0,  5,  0,
     0,  3, 37,  0,  0, 43,  2,  8, 44,  5,  2, 37,  3,  2,  2, 26,
     2,  0,  0,  0,  0, 27,  8, 37,  0,  0,  3,  2,  4,  0,  0,  0,
    36,  4,  6,  0,  0, 43,  4, 45,  0,  0,  0,  2,  2, 36,  0,  0,
     8,  2,  2,  2, 27,  2, 37,  1,  0, 37,  0,  0,  2,  8,  0,  0,
     0,  0,  3, 46,  0,  0, 36,  8,  2, 37, 36,  2,  0,  0, 36,  4,
     0,  0,  7,  0,  8,  2,  2,  4, 43, 43,  3,  0,  0, 36,  2,  4,
     0,  3,  2,  2,  3, 36,  4, 37,  0,  0,  5,  8,  7,  7,  0,  0,
     3,  0,  0, 37, 27, 26, 37, 36,  0,  0,  0,  4,  0,  1, 37,  1,
     0,  0,  0, 43,  0,  0,  5,  0,  5,  7,  0,  2,  0,  0,  8,  3,
     0,  0,  2,  2,  3,  8,  7,  1,  0,  3,  2,  5,  2, 37,  0,  0,
     0, 36,  2,  8,  0,  0,  3,  1,  2,  6,  0,  0,  0,  3,  4,  0,
     3,  2,  2,  2,  8,  5,  2,  0,
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
    3, 2, 0, 0, 2, 0, 2, 2, 3, 0, 0, 0, 0, 0, 4, 0,
    4, 0, 4, 0, 0, 4, 0, 4, 0, 1, 4, 4,
};

/* Joining_Type: 1868 bytes. */

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
    12, 13, 14, 15, 10, 10, 16, 10, 10, 10, 10, 17, 10, 18, 19, 20,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 21,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 21,
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
    22, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
};

static RE_UINT8 re_line_break_stage_2[] = {
      0,   1,   2,   2,   2,   3,   4,   5,   2,   6,   7,   8,   9,  10,  11,  12,
     13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
     29,  30,  31,  32,  33,  34,  35,  36,  37,   2,   2,   2,   2,  38,  39,  40,
     41,  42,  43,  44,  45,  46,  47,  48,  49,  50,   2,  51,   2,   2,  52,  53,
     54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      2,   2,   2,  70,   2,   2,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,
     81,  82,  83,  84,  85,  86,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  87,  79,  79,  79,  79,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,
     88,  79,  79,  79,  79,  79,  79,  79,  79,  89,   2,   2,  90,  91,   2,  92,
     93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 101,
    102, 103, 104, 105, 106, 107, 101, 102, 103, 104, 105, 106, 107, 101, 102, 103,
    104, 105, 106, 107, 101, 102, 103, 104, 105, 106, 107, 101, 102, 103, 104, 105,
    106, 107, 101, 102, 103, 104, 105, 106, 107, 101, 102, 103, 104, 105, 106, 107,
    101, 102, 103, 104, 105, 106, 107, 101, 102, 103, 104, 105, 106, 107, 101, 102,
    103, 104, 105, 106, 107, 101, 102, 103, 104, 105, 106, 107, 101, 102, 103, 108,
    109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  79,  79,  79,  79, 110, 111,   2,   2, 112, 113, 114, 115, 116, 117,
    118, 119, 120, 121,  72, 122, 123, 124,   2, 125,  72,  72,  72,  72,  72,  72,
    126,  72, 127, 128, 129,  72, 130,  72, 131,  72,  72,  72, 132,  72,  72,  72,
    133, 134, 135, 136,  72,  72,  72,  72,  72,  72,  72,  72,  72, 137,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
      2,   2,   2,   2,   2,   2, 138,  72, 139,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
      2,   2,   2,   2, 140, 141, 142,   2, 143,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
      2,   2,   2,   2, 144,  72,  72,  72,  72,  72,  72,  72,  72,  72, 145, 146,
    147,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
      2, 148, 149, 150, 151,  72, 152,  72, 153, 154, 155,   2,   2, 156,   2, 157,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72, 158, 159,  72,  72,
    160, 161, 162, 163, 164,  72, 165, 166, 167, 168, 169, 170, 171, 172, 173,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79, 174,
    175,  72, 176, 177,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
     72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
};

static RE_UINT16 re_line_break_stage_3[] = {
      0,   1,   2,   3,   4,   5,   4,   6,   7,   1,   8,   9,   4,  10,   4,  10,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  11,  12,   4,   4,
      1,   1,   1,   1,  13,  14,  15,  16,  17,   4,  18,   4,   4,   4,   4,   4,
     19,   4,   4,   4,   4,   4,   4,   4,   4,   4,  20,  21,   4,  22,  21,   4,
     23,  24,   1,  25,  26,  27,  28,  29,  30,  31,   4,   4,  32,   1,  33,  34,
      4,   4,   4,   4,   4,  35,  36,  37,  38,  39,   4,   1,  40,   4,   4,   4,
      4,   4,  41,  42,  37,   4,  32,  43,   4,  44,  45,  46,   4,  47,  48,  48,
     48,  48,  49,  48,  48,  48,  50,  51,  52,   4,   4,  53,   1,  54,  55,  56,
     57,  58,  59,  60,  61,  62,  63,  64,  65,  58,  59,  66,  67,  68,  69,  70,
     71,  18,  59,  72,  73,  74,  63,  75,  57,  58,  59,  72,  76,  77,  63,  20,
     78,  79,  80,  81,  82,  83,  69,  84,  85,  86,  59,  87,  88,  89,  63,  90,
     91,  86,  59,  92,  88,  93,  63,  94,  91,  86,   4,  95,  96,  97,  63,  98,
     99, 100,   4, 101, 102, 103,  48, 104, 105, 106, 106, 107, 108, 109,  48,  48,
    110, 111, 112, 113, 114, 115,  48,  48, 116, 117,  37, 118,  56,   4, 119, 120,
    121, 122,   1, 123, 124, 125,  48,  48, 106, 106, 106, 106, 126, 106, 106, 106,
    106, 127,   4,   4, 128,   4,   4,   4, 129, 129, 129, 129, 129, 129, 130, 130,
    130, 130, 131, 132, 132, 132, 132, 132,   4,   4,   4,   4, 133, 134,   4,   4,
    133,   4,   4, 135, 136, 137,   4,   4,   4, 136,   4,   4,   4, 138, 139, 119,
      4, 140,   4,   4,   4,   4,   4, 141, 142,   4,   4,   4,   4,   4,   4,   4,
    142, 143,   4,   4,   4,   4, 144,  74, 145, 146,   4, 147,   4, 148, 145, 149,
    106, 106, 106, 106, 106, 150, 151, 140, 152, 151,   4,   4,   4,   4,   4,  20,
      4,   4, 153,   4,   4,   4,   4, 154,   4, 119, 155, 155, 156, 106, 157, 158,
    106, 106, 159, 106, 160, 161,   4,   4,   4, 162, 106, 106, 106, 163, 106, 164,
    151, 151, 157,  48,  48,  48,  48,  48, 165,   4,   4, 166, 167, 168, 169, 170,
    171,   4, 172,  37,   4,   4,  41, 173,   4,   4, 166, 174, 175,  37,   4, 176,
     48,  48,  48,  48,  20, 177, 178, 179,   4,   4,   4,   4,   1,   1, 180, 181,
      4, 182,   4,   4, 182, 183,   4, 184,   4,   4,   4, 185, 185, 186,   4, 187,
    188, 189, 190, 191, 192, 193, 194, 195, 196, 119, 197, 198,  48,   1,   1, 199,
    200, 201, 202,   4,   4, 203, 204, 205, 206, 205,   4,   4,   4, 207,   4,   4,
    208, 209, 210, 211, 212, 213, 214,   4, 215, 216, 217, 218,   4,   4,   4,   4,
      4, 219, 220,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 221,
      4,   4, 222,  48, 223,  48, 224, 224, 224, 224, 224, 224, 224, 224, 224, 225,
    224, 224, 224, 224, 204, 224, 224, 226, 224, 227, 228, 229, 230, 231, 232,   4,
    233, 234,   4, 235, 236,   4, 237, 238,   4, 239,   4, 240, 241, 242, 243, 244,
    245,   4,   4,   4,   4, 246, 247, 248, 224, 249,   4,   4, 250,   4, 251,   4,
    252, 253,   4,   4,   4, 254,   4, 255,   4,   4,   4,   4, 119, 256,  48,  48,
     48,  48,  48,  48,  48,  48,  48,  48,   4,   4,  46,   4,   4,  46,   4,   4,
      4,   4,   4,   4,   4,   4, 257, 258,   4,   4, 128,   4,   4,   4, 259, 260,
      4, 222, 261, 261, 261, 261,   1,   1, 262, 263, 264, 265,  48,  48,  48,  48,
    266, 267, 266, 266, 266, 266, 266, 221, 266, 266, 266, 266, 266, 266, 266, 266,
    266, 266, 266, 266, 266, 268,  48, 269, 270, 271, 272, 273, 274, 266, 275, 266,
    276, 277, 278, 266, 275, 266, 276, 279, 280, 266, 281, 282, 266, 266, 266, 266,
    283, 266, 266, 284, 266, 266, 221, 285, 266, 283, 266, 266, 286, 266, 266, 266,
    266, 266, 266, 266, 266, 266, 266, 283, 266, 266, 266, 266,   4,   4,   4,   4,
    266, 287, 266, 266, 266, 266, 266, 266, 288, 266, 266, 266, 289,   4,   4, 176,
    290,   4, 291,  48,   4,   4, 257, 292,   4, 293,   4,   4,   4,   4,   4, 294,
     46, 295, 223,  48,  48,  48,  48,  90, 296,   4, 297, 298,   4,   4,   4, 299,
    300,   4,   4, 166, 301, 151,   1, 302,  37,   4, 303,   4, 304, 305, 129, 306,
     52,   4,   4, 307, 308, 309,  48,  48,   4,   4, 310, 180, 311, 312, 106, 159,
    106, 106, 106, 106, 313, 314,  32, 315, 316, 317, 261,  48,  48,  48,  48,  48,
     48,  48,  48,  48,   4,   4, 318, 151, 319, 320, 321, 322, 321, 323, 321, 319,
    320, 321, 322, 321, 323, 321, 319, 320, 321, 322, 321, 323, 321, 319, 320, 321,
    322, 321, 323, 321, 319, 320, 321, 322, 321, 323, 321, 319, 320, 321, 322, 321,
    323, 321, 319, 320, 321, 322, 321, 323, 321, 319, 320, 321, 322, 321, 323, 321,
    322, 321, 324, 130, 325, 132, 132, 326, 327, 327, 327, 327, 327, 327, 327, 327,
    222, 328, 329, 330, 331,   4,   4,   4,   4,   4,   4,   4, 332, 333,   4,   4,
      4,   4,   4, 334,  48,   4,   4,   4,   4, 335,   4,   4,  20,  48,  48, 336,
      1, 337, 180, 338, 339, 340, 341, 185,   4,   4,   4,   4,   4,   4,   4, 342,
    343, 344, 266, 345, 266, 346, 347, 348,   4, 349,   4,  46, 350, 351, 352, 353,
    354,   4, 137, 355, 184, 184,  48,  48,   4,   4,   4,   4,   4,   4,   4, 223,
    356,   4,   4, 357,   4,   4,   4,   4, 223, 358,  48,  48,  48,   4,   4, 359,
      4, 119,   4,   4,   4,  74,  48,  48,   4,  46, 295,   4, 223,  48,  48,  48,
      4, 360,   4,   4, 361, 362,  48,  48,   4, 184, 151,  48,  48,  48,  48,  48,
    363,   4,   4, 364,   4, 365,  48,  48,   4, 366,   4, 367,  48,  48,  48,  48,
      4,   4,   4, 368,  48,  48,  48,  48, 369, 370,   4, 371,  20, 372,   4,   4,
      4,   4,   4, 373,   4, 374,   4, 375,   4,   4,   4,   4, 376,  48,  48,  48,
     48,  48,  48,  48,  48,  48,   4,  46, 171,   4,   4, 377, 378, 335, 379,  48,
    171,   4,   4, 380, 381,   4, 376, 151, 171,   4, 304, 382, 383,  48,  48,  48,
    171,   4,   4, 307, 384, 151,  48,  48,   4,   4,  32, 385, 151,  48,  48,  48,
      4,   4,   4,   4,   4,   4,  46,  48,   4,   4,   4,   4,   4,   4, 386, 383,
      4,   4,   4,   4,   4, 387,   4,   4, 388,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4, 389,   4,   4,  46,  48,  48,  48,  48,  48,
      4,   4,   4, 376,  48,  48,  48,  48,   4,   4,   4,   4, 141, 390,   1,  51,
    391, 171,  48,  48,  48,  48,  48,  48, 392,  48,  48,  48,  48,  48,  48,  48,
      4,   4,   4,   4,   4,   4,   4, 154,   4,   4,  22,   4,   4,   4, 393,   1,
    394,   4, 395,   4,   4, 184,  48,  48,   4,   4,   4,   4, 396,  48,  48,  48,
      4,   4,   4,   4,   4, 222,   4, 332,   4,   4,   4,   4,   4, 185,   4,   4,
      4, 145, 397, 398, 399,   4,   4,   4, 400, 401,   4, 402, 403,  86,   4,   4,
      4,   4, 374,   4,   4,   4,   4,   4,   4,   4,   4,   4, 404, 405, 405, 405,
    399,   4, 406, 407, 408, 409, 410, 411, 412, 358, 413, 358,  48,  48,  48, 332,
    266, 266, 269, 266, 266, 266, 266, 266, 266, 221, 283, 414, 282, 282,  48,  48,
    415, 224, 416, 224, 224, 224, 417, 224, 224, 415,  48,  48,  48,  48, 418, 419,
    420, 266, 266, 284, 421, 392,  48,  48, 266, 266, 422, 423, 266, 266, 266, 288,
    266, 221, 266, 424, 425,  48, 266, 422, 266, 266, 266, 283, 426, 266, 266, 266,
    266, 266, 427, 428, 266, 266, 266, 429, 430, 431, 432, 433, 295, 266, 434,  48,
     48,  48,  48,  48,  48,  48,  48, 435, 266, 266, 266, 266, 436,  48,  48,  48,
    266, 266, 266, 266, 268,  48,  48,  48,   4,   4,   4,   4,   4,   4,   4, 295,
    266, 266, 266, 266, 266, 266, 266, 281, 437,  48,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  48,
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
     14,  14,  14,  38,  39,  14,  14,  14,  14,  14,  14,  14,  40,  41,  36,  42,
     43,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  44,
     19,  45,   0,  46,  36,  36,  36,  36,  47,  47,  47,  47,  47,  47,  47,  47,
     47,  47,  47,  47,  47,  48,  36,  36,  47,  49,  38,  36,  36,  36,  36,  36,
     14,  14,  38,  14,  50,  51,  13,  14,   0,   0,   0,   0,   0,  52,  36,  53,
     14,  14,  14,  14,  14,  19,   0,   0,  12,  12,  12,  12,  12,  54,  55,  14,
     45,  14,  14,  14,  14,  14,  14,  14,  14,  14,  56,   0,   0,   0,  45,  19,
      0,   0,  45,  19,  45,   0,   0,  14,  12,  12,  12,  12,  12,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  39,  19,  14,  14,  14,  14,  14,  14,  14,
      0,   0,   0,   0,   0,  57,  39,  14,  14,  14,  14,   0,   0,   0,   0,   0,
     45,  36,  36,  36,  36,  36,  36,  36,   0,   0,  14,  14,  58,  38,  36,  36,
     14,  14,  14,   0,   0,  19,   0,   0,   0,   0,  19,   0,  19,   0,   0,  36,
     14,  14,  14,  14,  14,  14,  14,  38,  14,  14,  14,  14,  19,   0,  36,  38,
     36,  36,  36,  36,  36,  36,  36,  36,  38,  14,  14,  14,  14,  14,  38,  36,
     36,  36,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  57,
      0,   0,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,   0,  45,   0,
     19,   0,   0,   0,  14,  14,  14,  14,  14,   0,  59,  12,  12,  12,  12,  12,
     14,  14,  14,  14,  39,  14,  14,  14,  43,   0,  39,  14,  14,  14,  38,  39,
     38,  39,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  38,  14,  14,  14,
     38,  38,  36,  14,  14,  36,  45,   0,   0,   0,  57,  43,  57,  43,   0,  38,
     36,  36,  36,  43,  36,  36,  14,  39,  14,   0,  36,  12,  12,  12,  12,  12,
     14,  51,  14,  14,  50,   9,  36,  36,  43,   0,  39,  14,  14,  38,  36,  39,
     38,  14,  39,  38,  14,  36,  57,   0,   0,  57,  36,  43,  57,  43,   0,  36,
     43,  36,  36,  36,  39,  14,  38,  38,  36,  36,  36,  12,  12,  12,  12,  12,
      0,  14,  19,  36,  36,  36,  36,  36,  43,   0,  39,  14,  14,  14,  14,  39,
     38,  14,  39,  14,  14,  36,  45,   0,   0,   0,   0,  43,   0,  43,   0,  36,
     38,  36,  36,  36,  36,  36,  36,  36,   9,  36,  36,  36,  36,  36,  36,  36,
      0,   0,  57,  43,  57,  43,   0,  36,  36,  36,  36,   0,  36,  36,  14,  39,
     36,  45,  39,  14,  14,  38,  36,  14,  38,  14,  14,  36,  39,  38,  38,  14,
     36,  39,  38,  36,  14,  38,  36,  14,  14,  14,  14,  14,  14,  36,  36,   0,
      0,  57,  36,   0,  57,   0,   0,  36,  38,  36,  36,  43,  36,  36,  36,  36,
     14,  14,  14,  14,   9,  38,  36,  36,  43,   0,  39,  14,  14,  14,  38,  14,
     38,  14,  14,  14,  14,  14,  14,  14,  14,  14,  39,  14,  14,  36,  39,   0,
      0,   0,  57,   0,  57,   0,   0,  36,  36,  36,  43,  57,  14,  36,  36,  36,
     36,  36,  36,  36,  14,  14,  14,  14,  36,   0,  39,  14,  14,  14,  38,  14,
     14,  14,  39,  14,  14,  36,  45,   0,  36,  36,  43,  57,  36,  36,  36,  38,
     39,  38,  36,  36,  36,  36,  36,  36,  14,  14,  14,  14,  14,  38,  39,   0,
      0,   0,  57,   0,  57,   0,   0,  38,  36,  36,  36,  43,  36,  36,  36,  36,
     14,  14,  14,  36,  60,  14,  14,  14,  36,   0,  39,  14,  14,  14,  14,  14,
     14,  14,  14,  38,  36,  14,  14,  14,  14,  39,  14,  14,  14,  14,  39,  36,
     14,  14,  14,  38,  36,  57,  36,  43,   0,   0,  57,  57,   0,   0,   0,   0,
     36,   0,  38,  36,  36,  36,  36,  36,  61,  62,  62,  62,  62,  62,  62,  62,
     62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  63,  36,  42,
     62,  62,  62,  62,  62,  62,  62,  64,  12,  12,  12,  12,  12,  59,  36,  36,
     61,  63,  63,  61,  63,  63,  61,  36,  36,  36,  62,  62,  61,  62,  62,  62,
     61,  62,  61,  61,  36,  62,  61,  62,  62,  62,  62,  62,  62,  61,  62,  36,
     62,  62,  63,  63,  62,  62,  62,  36,  12,  12,  12,  12,  12,  36,  62,  62,
     32,  65,  29,  65,  66,  67,  68,  53,  53,  69,  56,  14,   0,  14,  14,  14,
     14,  14,  44,  19,  19,  70,  70,   0,  14,  14,  14,  14,  14,  14,  38,  36,
     43,   0,   0,   0,   0,   0,   0,   1,   0,   0,   1,   0,  14,  14,  19,   0,
      0,   0,   0,   0,  43,   0,   0,   0,   0,   0,   0,   0,   0,   0,  57,  59,
     14,  14,  14,  45,  14,  14,  38,  14,  65,  71,  14,  14,  72,  73,  36,  36,
     12,  12,  12,  12,  12,  59,  14,  14,  12,  12,  12,  12,  12,  62,  62,  62,
     14,  14,  14,  39,  36,  36,  39,  36,  74,  74,  74,  74,  74,  74,  74,  74,
     75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  76,  76,  76,  76,
     76,  76,  76,  76,  76,  76,  76,  76,  14,  14,  14,  14,  38,  14,  14,  36,
     14,  14,  14,  38,  38,  14,  14,  36,  38,  14,  14,  36,  14,  14,  14,  38,
     38,  14,  14,  36,  14,  14,  14,  14,  14,  14,  14,  38,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  38,  43,   0,  27,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  36,  36,  36,  14,  14,  38,  36,  36,  36,  36,  36,
     77,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  16,  78,  36,
     14,  14,  14,  14,  14,  27,  59,  14,  14,  14,  14,  14,  14,  14,  38,  14,
     14,   0,  57,  36,  36,  36,  36,  36,  14,   0,   1,  41,  36,  36,  36,  36,
     14,   0,  36,  36,  36,  36,  36,  36,  38,   0,  36,  36,  36,  36,  36,  36,
     62,  62,  59,  79,  77,  80,  62,  36,  12,  12,  12,  12,  12,  36,  36,  36,
     14,  53,  59,  29,  53,  19,   0,  73,  14,  14,  14,  14,  19,  38,  36,  36,
     14,  14,  14,  36,  36,  36,  36,  36,   0,   0,   0,   0,   0,   0,  36,  36,
     38,  36,  53,  12,  12,  12,  12,  12,  62,  62,  62,  62,  62,  62,  62,  36,
     62,  62,  63,  36,  36,  36,  36,  36,  62,  62,  62,  62,  62,  62,  36,  36,
     62,  62,  62,  62,  62,  36,  36,  36,  12,  12,  12,  12,  12,  63,  36,  62,
     14,  14,  14,  19,   0,   0,  36,  14,  62,  62,  62,  62,  62,  62,  62,  63,
     62,  62,  62,  62,  62,  62,  63,  43,   0,   0,  45,  14,  14,  14,  14,  14,
     14,  14,   0,   0,   0,   0,   0,   0,   0,   0,  45,  14,  14,  14,  36,  36,
     12,  12,  12,  12,  12,  59,  27,  59,  77,  14,  14,  14,  14,  19,   0,   0,
      0,   0,  14,  14,  14,  14,  38,  36,   0,  45,  14,  14,  14,  14,  14,  14,
     19,   0,   0,   0,   0,   0,   0,  14,   0,   0,  36,  36,  36,  36,  14,  14,
      0,   0,   0,   0,  36,  81,  59,  59,  12,  12,  12,  12,  12,  36,  39,  14,
     14,  14,  14,  14,  14,  14,  14,  59,   0,  45,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  45,  14,  19,  14,  14,   0,  45,  38,  36,  36,  36,  36,
      0,   0,   0,  57,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,   0,   0,
     14,  14,  14,  36,  14,  14,  14,  36,  14,  14,  14,  14,  39,  39,  39,  39,
     14,  14,  14,  14,  14,  14,  14,  36,  14,  14,  38,  14,  14,  14,  14,  14,
     14,  14,  36,  14,  14,  14,  39,  14,  36,  14,  38,  14,  14,  14,  32,  38,
     59,  59,  59,  82,  59,  83,   0,   0,  82,  59,  84,  25,  85,  86,  85,  86,
     28,  14,  87,  88,  89,   0,   0,  33,  51,  51,  51,  51,   7,  90,  91,  14,
     14,  14,  92,  93,  91,  14,  14,  14,  14,  14,  14,  77,  59,  59,  27,  59,
     94,  14,  38,  36,  36,   0,   0,   0,  14,  36,  25,  14,  14,  14,  16,  95,
     24,  28,  25,  14,  14,  14,  16,  78,  23,  23,  23,   6,  23,  23,  23,  23,
     23,  23,  23,  22,  23,  96,  36,  36,  57,  36,  36,  36,  36,  36,  36,  36,
     14,  50,  24,  14,  50,  14,  14,  14,  14,  24,  14,  97,  14,  14,  14,  14,
     24,  25,  14,  14,  14,  24,  14,  14,  14,  14,  28,  14,  14,  24,  14,  25,
     28,  28,  28,  28,  28,  28,  14,  14,  28,  28,  28,  28,  28,  14,  14,  14,
     14,  14,  14,  14,  24,  36,  36,  36,  14,  25,  25,  14,  14,  14,  14,  14,
     25,  28,  14,  24,  25,  24,  14,  24,  24,  23,  24,  14,  14,  25,  24,  28,
     25,  24,  24,  24,  28,  28,  25,  25,  14,  14,  28,  28,  14,  14,  28,  14,
     14,  14,  14,  14,  25,  14,  25,  14,  14,  25,  14,  14,  14,  14,  14,  14,
     28,  14,  28,  28,  14,  28,  14,  28,  14,  28,  14,  28,  14,  14,  14,  14,
     14,  14,  24,  14,  24,  14,  14,  14,  14,  14,  24,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  24,  14,  25,  14,  14,  14,  98,  14,  14,
     14,  14,  14,  14,  16,  99,  14,  14,  98,  98,  36,  36,  36,  36,  36,  36,
     14,  14,  14,  38,  36,  36,  36,  36,  14,  14,  14,  14,  14,  38,  36,  36,
     28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  25,
     28,  28,  25,  14,  14,  14,  14,  14,  14,  28,  28,  14,  14,  14,  14,  14,
     28,  24,  28,  28,  28,  14,  14,  14,  14,  28,  14,  28,  14,  14,  28,  14,
     28,  14,  14,  28,  25,  24,  14,  28,  28,  14,  14,  14,  14,  14,  14,  14,
     14,  28,  28,  14,  14,  14,  14,  24,  98,  98,  24,  25,  24,  14,  14,  28,
     14,  14,  98,  28, 100,  98,  98,  98,  14,  14,  14,  14, 101,  98,  14,  14,
     25,  25,  14,  14,  14,  14,  14,  14,  28,  24,  28,  24, 102,  25,  28,  24,
     14,  14,  14,  14,  14,  14,  14, 101,  14,  14,  14,  14,  14,  14,  14,  28,
     14,  14,  14,  14,  14,  14, 101,  98,  98,  98,  98,  98, 102,  28, 103, 101,
     98, 103, 102,  28,  98,  28, 102, 103,  98,  24,  14,  14,  28, 102,  28,  28,
    103,  98,  98, 103,  98, 102, 103,  98, 104,  98, 100,  14,  98,  98,  98,  14,
     14,  14,  14,  24,  14,   7,  85,   5,  14,  53,  14,  14,  70,  70,  70,  70,
     70,  70,  70,  28,  28,  28,  28,  28,  28,  28,  14,  14,  14,  14,  14,  14,
     14,  14,  16,  99,  14,  14,  14,  14,  14,  14,  14,  70,  70,  70,  70,  70,
     14,  16, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105,  99,  14,  14,  14,
     14,  14,  14,  14,  70,  70,  14,  14,  14,  14,  14,  14,  14,  14,  70,  14,
     14,  14,  24,  28,  28,  36,  36,  36,  14,  14,  14,  14,  14,  14,  14,  19,
      0,  14,  36,  36, 106,  59,  77, 107,  14,  14,  14,  14,  36,  36,  36,  39,
     41,  36,  36,  36,  36,  36,  36,  43,  14,  14,  14,  38,  14,  14,  14,  38,
     85,  85,  85,  85,  85,  85,  85,  59,  59,  59,  59,  27, 108,  14,  85,  14,
     85,  70,  70,  70,  70,  59,  59,  56,  59,  27,  77,  14,  14, 109,  36,  36,
     98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98, 104,  98,  98,
     98,  98,  98,  36,  36,  36,  36,  36,  98,  98,  98,  98,  98,  98,  36,  36,
    110, 111, 112,  98,  70,  70,  70,  70,  70,  98,  70,  70,  70,  70, 113, 114,
     98,  98,  98,  98,  98,   0,   0,   0,  98,  98,  98,  98,  98, 112, 115,  98,
    116, 117, 117, 117, 117,  98,  98,  98,  98, 117,  98,  98,  98,  98,  98,  98,
     98, 117, 117, 117,  98,  98,  98, 118,  98,  98, 117, 119,  43, 120,  91, 115,
    121, 117, 117, 117, 117,  98,  98,  98,  98,  98, 117, 118,  98, 112, 122, 115,
     36,  36, 104,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  36,
    104,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98, 123,
     98,  98,  98,  98,  98, 123,  36,  36, 124, 124, 124, 124, 124, 124, 124, 124,
     98,  98,  98,  98,  28,  28,  28,  28,  98,  98, 112,  98,  98,  98,  98,  98,
     98,  98,  98,  98,  98,  98, 123,  36,  98,  98,  98, 123,  36,  36,  36,  36,
     14,  14,  14,  14,  14,  14,  27, 107,  12,  12,  12,  12,  12,  14,  36,  36,
      0,  45,   0,   0,   0,   0,   0,  14,  14,  14,  14,  14,  36,  36,  36,  43,
      0,  27,  59,  59,  36,  36,  36,  36,  14,  14,  36,  36,  36,  36,  36,  36,
     14,  45,  14,  45,  14,  19,  14,  14,  14,  19,   0,   0,  14,  14,  36,  36,
     14,  14,  14,  14, 125,  36,  36,  36,  14,  14,  65,  53,  36,  36,  36,  36,
      0,  14,  14,  14,  14,  14,  14,  14,   0,   0,  57,  36,  36,  36,  36,  59,
      0,  14,  14,  14,  14,  14,  36,  36,  14,  14,  14,   0,   0,   0,   0,  59,
     14,  14,  14,  19,   0,   0,   0,   0,   0,   0,  36,  36,  36,  36,  36,  39,
     74,  74,  74,  74,  74,  74, 126,  36,  14,  19,   0,   0,   0,   0,   0,   0,
     45,  14,  14,  27,  59,  14,  14,  39,  12,  12,  12,  12,  12,  36,  36,  14,
     14,  14,  14,  14,  19,   0,   0,   0,  14,  19,  14,  14,  14,  14,   0,  36,
     12,  12,  12,  12,  12,  36,  27,  59,  62,  63,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  61,  62,  62,  59,  14,  19,  57,  36,  36,  36,  36,
     39,  14,  14,  38,  39,  14,  14,  38,  39,  14,  14,  38,  36,  36,  36,  36,
     14,  19,   0,   0,   0,   1,   0,  36, 127, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 127, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 127, 128, 128, 128, 128, 128, 127, 128, 128, 128, 128, 128,
    128, 128,  36,  36,  36,  36,  36,  36,  75,  75,  75, 129,  36, 130,  76,  76,
     76,  76,  76,  76,  76,  76,  36,  36, 131, 131, 131, 131, 131, 131, 131, 131,
     36,  39,  14,  14,  36,  36, 132, 133,  47,  47,  47,  47,  49,  47,  47,  47,
     47,  47,  47,  48,  47,  47,  48,  48,  47, 132,  48,  47,  47,  47,  47,  47,
     14,  36,  36,  36,  36,  36,  36,  36,  36,  39,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  70,  36,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14, 125,  36, 134, 135,  58, 136, 137,  36,  36,  36,
     98,  98, 138, 105, 105, 105, 105, 105, 105, 105, 111, 138, 111,  98,  98,  98,
    111,  78,  91,  53, 138, 105, 105, 111,  98,  98,  98, 123, 139, 140,  36,  36,
     14,  14,  14,  14,  14,  14,  38, 141, 106,  98,   6,  98,  70,  98, 111, 111,
     98,  98,  98,  98,  98,  91,  98, 142,  98,  98,  98,  98,  98, 138, 110,  98,
     98,  98,  98,  98,  98, 138, 110, 138, 114,  70,  93, 143, 124, 124, 124, 124,
    144,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  91,
     36,  14,  14,  14,  36,  14,  14,  14,  36,  14,  14,  14,  36,  14,  38,  36,
     22,  98, 139,  96,  14,  14,  14,  38,  36,  36,  36,  36,  43,   0, 145,  36,
     14,  14,  14,  14,  14,  14,  39,  14,  14,  14,  14,  14,  14,  38,  14,  39,
     59,  41,  36,  39,  14,  14,  14,  14,  14,  14,  36,  39,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  36,  36,  14,  14,  14,  14,  14,  14,  19,  36,
     14,  14,  14,  14,  14,  14,  14,  81,  14,  14,  36,  36,  14,  14,  14,  14,
     77,  14,  14,  36,  36,  36,  36,  36,  14,  14,  14,  36,  38,  14,  14,  14,
     14,  14,  14,  39,  38,  36,  38,  39,  14,  14,  14,  81,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  36,  81,  14,  14,  14,  14,  14,  36,  36,  39,
     14,  14,  14,  14,  36,  36,  36,  14,  19,   0,  43,  57,  36,  36,   0,   0,
     14,  14,  39,  14,  39,  14,  14,  14,  14,  14,  36,  36,   0,  57,  36,  43,
     59,  59,  59,  59,  38,  36,  36,  36,  14,  14,  14,  36,  81,  59,  59,  59,
     14,  14,  14,  36,  14,  14,  14,  14,  14,  38,  36,  36,  14,  14,  14,  14,
     14,  14,  14,  14,  38,  36,  36,  36,  14,  14,  14,  14,   0,   0,   0,   0,
      0,   0,   0,   1,  77,  14,  14,  36,  14,  14,  14,  12,  12,  12,  12,  12,
      0,   0,   0,   0,   0,  45,  14,  59,  59,  36,  36,  36,  36,  36,  36,  36,
      0,   0,  57,  12,  12,  12,  12,  12,  59,  59,  36,  36,  36,  36,  36,  36,
     45,  14,  27,  77,  41,  36,  36,  36,   0,   0,   0,   0,  36,  36,  36,  36,
     14,  38,  36,  36,  36,  36,  36,  36,  14,  14,  14,  14, 146,  70, 114,  14,
     14,  99,  14,  70,  70,  14,  14,  14,  14,  14,  14,  14,  16, 114,  14,  14,
     19,   0,   0,   0,   0,   0,   0,   0,  36,  36,  36,  36,  36,  36,  36,  43,
     98,  36,  36,  36,  36,  36,  36,  36,  14,  14,  19,   0,   0,  14,  19,   0,
      0,  45,  19,   0,   0,   0,  14,  14,  14,  14,  14,  14,  14,   0,   0,  14,
     14,   0,  45,  36,  36,  36,  36,  36,  36,  38,  39,  38,  39,  14,  38,  14,
     14,  14,  14,  14,  14,  39,  39,  14,  14,  14,  39,  14,  14,  14,  14,  14,
     14,  14,  14,  39,  14,  38,  39,  14,  14,  14,  38,  14,  14,  14,  38,  14,
     14,  14,  14,  14,  14,  39,  14,  38,  14,  14,  38,  38,  36,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  36,  12,  12,  12,  12,  12,  12,  12,  12,  12,
     39,  38,  38,  39,  39,  14,  14,  14,  14,  38,  14,  14,  39,  39,  36,  36,
     36,  38,  36,  39,  39,  39,  39,  14,  39,  38,  38,  39,  39,  39,  39,  39,
     39,  38,  38,  39,  14,  38,  14,  14,  14,  38,  14,  14,  39,  14,  38,  38,
     14,  14,  14,  14,  14,  39,  14,  14,  39,  14,  39,  14,  14,  39,  14,  14,
    104,  98,  98,  98,  98,  98,  98, 123,  28,  28,  28,  28,  28, 147,  36,  36,
     28,  28,  28,  28,  28,  28,  28,  38,  28,  28,  28,  28,  28,  14,  36,  36,
     36,  36,  36, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148,
     98, 123,  36,  36,  36,  36,  36,  36,  98,  98,  98,  98, 123,  36,  36,  36,
    123,  36,  36,  36,  36,  36,  36,  36,  98,  98,  98, 104,  98,  98,  98,  98,
     98,  98, 100, 101,  98,  98, 101,  98,  98,  98, 123,  98,  98, 123,  36,  36,
    123,  98,  98,  98,  98,  98,  98,  98, 101, 101, 101,  98,  98,  98,  98, 100,
    100, 101,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98, 104,  98, 123,  36,
     14,  14,  14, 101,  98,  98,  98,  98,  98,  98,  98, 100,  14,  14,  14,  14,
     14,  14, 101,  98,  98,  98,  98,  98,  98,  14,  14,  14,  14,  14,  14,  36,
     98,  98,  98,  98,  36,  36,  36,  36,  36,  36,  36,  36,  36, 104,  98,  98,
    123,  36, 104,  98,  98,  98,  98,  98,  43,  36,  36,  36,  36,  36,  36,  36,
};

static RE_UINT8 re_line_break_stage_5[] = {
    16, 16, 16, 18, 22, 20, 20, 21, 19,  6,  3, 12,  9, 10, 12,  3,
     1, 36, 12,  9,  8, 15,  8,  7, 11, 11,  8,  8, 12, 12, 12,  6,
    12,  1,  9, 36, 18,  2, 12, 16, 16, 29,  4,  1, 10,  9,  9,  9,
    12, 25, 25, 12, 25,  3, 12, 18, 25, 25, 17, 12, 25,  1, 17, 25,
    12, 17, 16,  4,  4,  4,  4, 16,  0,  0,  8,  0, 12,  0,  0, 12,
     0,  8, 18,  0,  0,  9,  0, 16, 18, 16, 16, 12,  6, 16, 37, 37,
    37,  0, 37, 12, 12, 10, 10, 10, 16,  6,  6,  6, 10, 11, 11, 12,
     6, 12, 16,  0,  8,  6, 18, 18,  0, 10,  0, 24, 24, 24, 24,  0,
    24, 12, 17, 17,  4, 17, 17, 18,  4,  6,  4, 12,  1,  2, 18, 17,
    12,  4,  4,  0, 31, 31, 32, 32, 33, 33, 18, 12,  2,  0,  5, 24,
    18,  9,  0, 18, 18,  4, 18, 28, 26, 25,  3,  3,  1,  3, 14, 14,
    14, 18, 20, 20,  3, 25,  5,  5,  8,  1,  2,  5, 30, 12,  2, 25,
     9,  0,  9, 12, 13, 13,  2, 12, 13, 12, 12, 13, 13, 25, 25, 13,
     0, 13,  2,  1,  0,  6,  6, 18,  1, 18, 26, 26, 13,  2,  2, 13,
    13,  5,  5,  1,  2,  2,  5, 13,  0, 38, 13, 38, 38, 13, 38,  0,
    16,  5,  5, 38, 38,  5, 13,  0, 38, 38, 10, 12, 31,  0, 34, 35,
    35, 35, 32,  0,  0, 33, 27, 27,  0, 37, 16, 37,  8,  2,  2,  8,
     6,  1,  2, 14, 13,  1, 13,  9, 10, 13,  0, 30, 13,  6, 12, 38,
    38, 12, 23, 25,  1,  1, 25,  0, 39, 39,
};

/* Line_Break: 7658 bytes. */

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
    64, 65,  1,  1,  1,  1,  3,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1, 66,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1, 67,  1,  1,  1, 68,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 69,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    70, 71,  1,  1,  1,  1,  1,  1,  1, 72, 73, 74,  1,  1,  1,  1,
     1,  1,  1, 75,  1,  1,  1,  1,  1, 76,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 77,  1,  1,  1,  1,
     1,  1, 78,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 75,  1,  1,  1,  1,  1,  1,  1,
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
     0,  0,  0,  0, 29,  0,  0,  0,  0, 30, 31,  0, 30, 32,  0,  0,
    33,  0,  0,  0, 34,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,
     0,  0, 36,  0,  0,  0,  0,  0, 37,  0, 26,  0, 38, 39, 40, 41,
    36,  0,  0, 42,  0,  0,  0,  0, 43,  0, 44, 45,  0,  0,  0,  0,
     0,  0, 46,  0,  0,  0, 47,  0,  0,  0,  0,  0,  0,  0, 48,  0,
     0,  0,  0,  0,  0,  0,  0, 49,  0,  0,  0, 50,  0,  0,  0, 51,
    52,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 53,
     0,  0, 54,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55,  0,
    44,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 56,  0,  0,  0,
     0,  0,  0, 53,  0,  0,  0,  0,  0,  0,  0,  0, 44,  0,  0,  0,
     0, 54,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 57,  0,  0,
     0, 42,  0,  0,  0,  0,  0,  0,  0, 58, 59, 60,  0,  0,  0, 56,
     0,  3,  0,  0,  0,  0,  0, 61,  0, 62,  0,  0,  0,  0,  1,  0,
     3,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0, 63,  0, 55, 64, 26,
    65, 66, 19, 67, 35,  0,  0,  0,  0, 68, 69,  0,  0,  0, 70,  0,
     0,  0,  0,  0,  0,  3,  0,  0,  0,  0, 71,  0,  0,  0,  0,  0,
    72,  0,  0,  0,  0,  0,  0,  0,  0,  0, 73, 74,  0,  0,  0,  0,
     0,  0, 71, 71,  0,  0,  0,  0,  0,  0,  0, 75,  0,  0,  0,  0,
     0,  0, 76, 77,  0,  0,  0,  1,  0, 78,  0,  0,  0,  0,  1,  0,
    19, 19, 19, 79,  0,  0,  0,  0,  0,  0,  0, 80,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 81, 82, 83,  0,  0,  0,  0,  0,  0,  0,
    58,  0,  0, 43,  0,  0,  0, 84,  0, 58,  0,  0,  0,  0,  0,  0,
     0, 35,  0,  0, 85,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 86,
    87,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 42,  0,  0,  0,
     0,  0,  0,  0, 60,  0,  0,  0, 48,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0, 36,  0,  0,  0,  0,
};

static RE_UINT8 re_numeric_type_stage_4[] = {
     0,  0,  0,  0,  0,  0,  1,  2,  0,  0,  3,  4,  1,  2,  0,  0,
     5,  1,  0,  0,  5,  1,  6,  7,  5,  1,  8,  0,  5,  1,  9,  0,
     5,  1,  0, 10,  5,  1, 11,  0,  1, 12, 13,  0,  0, 14, 15, 16,
     0, 17, 18,  0,  1,  2, 19,  7,  0,  0,  1, 20,  1,  2,  1,  2,
     0,  0, 21, 22, 23, 22,  0,  0,  0,  0, 19, 19, 19, 19, 19, 19,
    24,  7,  0,  0, 23, 25, 26, 27, 19, 23, 25, 13,  0, 28, 29, 30,
     0,  0, 31, 32, 23, 33, 34,  0,  0,  0,  0, 35, 36,  0,  0,  0,
    37,  7,  0,  9,  0,  0, 38,  0, 19,  7,  0,  0,  0, 19, 37, 19,
     0,  0, 37, 19, 35,  0,  0,  0, 39,  0,  0,  0,  0, 40,  0,  0,
     0, 35,  0,  0, 41, 42,  0,  0,  0, 43, 44,  0,  0,  0,  0, 36,
    18,  0,  0, 36,  0, 18,  0,  0,  0,  0, 18,  0, 43,  0,  0,  0,
    45,  0,  0,  0,  0, 46,  0,  0, 47, 43,  0,  0, 48,  0,  0,  0,
     0,  0,  0, 39,  0,  0, 42, 42,  0,  0,  0, 40,  0,  0,  0, 17,
     0, 49, 18,  0,  0,  0,  0, 45,  0, 43,  0,  0,  0,  0, 40,  0,
     0,  0, 45,  0,  0, 45, 39,  0, 42,  0,  0,  0, 45, 43,  0,  0,
     0,  0,  0, 18, 17, 19,  0,  0,  0,  0, 11,  0,  0, 39, 39, 18,
     0,  0, 50,  0, 36, 19, 19, 19, 19, 19, 13,  0, 19, 19, 19, 18,
    13,  0,  0,  0, 42, 40,  0,  0,  0,  0, 51,  0,  0,  0,  0, 19,
     0,  0, 17, 13, 52,  0,  0,  0,  0,  0,  0, 53, 23, 25, 19, 10,
     0,  0, 54, 55, 56,  1,  0,  0,  0,  0,  5,  1,  9,  0,  0,  0,
    19, 19,  7,  0,  0,  5,  1,  1,  1,  1,  1,  1, 23, 57,  0,  0,
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
    1, 1, 1, 1, 1, 1, 3, 3, 2, 2, 2, 0, 0, 0, 0, 0,
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
    64, 65,  1,  1,  1,  1,  3,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1, 66,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1, 67,  1,  1,  1, 68,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 69,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    70, 71,  1,  1,  1,  1,  1,  1,  1, 72, 73, 74,  1,  1,  1,  1,
     1,  1,  1, 75,  1,  1,  1,  1,  1, 76,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 77,  1,  1,  1,  1,
     1,  1, 78,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 79,  1,  1,  1,  1,  1,  1,  1,
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
      0,   0,  87,  88,   0,   0,   0,   1,   0,  89,   0,   0,   0,   0,   1,   0,
     90,  91,  92,  93,   0,   0,   0,   0,   0,   0,   0,  94,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  95,  96,  97,   0,   0,   0,   0,   0,   0,   0,
     98,   0,   0,  99,   0,   0,   0, 100,   0, 101,   0,   0,   0,   0,   0,   0,
      0, 102,   0,   0, 103,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 104,
    105,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  62,   0,   0,   0,
      0,   0,   0,   0, 106,   0,   0,   0, 107,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 108,   0,   0,   0,   0,   0,   0,   0,   0, 109,   0,   0,   0,
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
     34,  35,  56,   0,   0,   0,   0,   0,   0,   0,  57,  58,  59,  60,  61,  62,
      0,   0,   0,   0,  63,  64,  65,  66,   0,  67,   0,   0,   0,   0,   0,   0,
     68,   0,   0,   0,   0,   0,   0,   0,   0,   0,  69,   0,   0,   0,   0,   0,
      0,   0,   0,  70,   0,   0,   0,   0,  71,  72,  73,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  74,   0,   0,   0,  75,   0,  76,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  77,  78,   0,   0,   0,   0,   0,   0,  79,
      0,   0,  80,   0,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,   0,
      0,   0,   0,   0,  81,   0,   0,   0,   0,  82,   0,   0,   0,   0,   0,   0,
      0,  83,   0,   0,   0,   0,   0,   0,   0,   0,  84,  85,   0,   0,   0,   0,
     86,  87,   0,  88,   0,   0,   0,   0,  89,  80,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  90,   0,   0,   0,   0,   0,   5,   0,   5,   0,
      0,   0,   0,   0,   0,   0,  91,   0,   0,   0,   0,   0,   0,   0,   0,  92,
      0,   0,   0,  15,  75,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  93,
      0,   0,   0,  94,   0,   0,   0,   0,   0,   0,   0,   0,  95,   0,   0,   0,
      0,  95,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  96,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  97,   0,  98,   0,   0,   0,   0,   0,   0,
      0,   0,   0,  25,   0,   0,   0,   0,   0,   0,   0,  99,  68,   0,   0,   0,
      0,   0,   0,   0,  75,   0,   0,   0, 100,   0,   0,   0,   0,   0,   0,   0,
      0, 101,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 102,   0,
      0,   0,   0,   0,   0, 103,   0,   0,   0,  48,  49, 104,   0,   0,   0,   0,
      0,   0,   0,   0, 105, 106,   0,   0,   0,   0, 107,   0, 108,   0,  75,   0,
      0,   0,   0,   0, 103,   0,   0,   0,   0,   0,   0,   0, 109,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 110,   0, 111,   8,   9,  57,  58, 112, 113,
    114, 115, 116, 117, 118,   0,   0,   0, 119, 120, 121, 122, 123, 124, 125, 126,
    127, 128, 129, 130, 122, 131, 132,   0,   0,   0, 103,   0,   0,   0,   0,   0,
    133,   0,   0,   0,   0,   0,   0,   0, 134,   0, 135,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 136, 137,   0,   0,   0,   0,   0,   0,   0,   0, 138, 139,
      0,   0,   0,   0,   0, 140, 141,   0,  34, 142,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 143,   0,   0,   0,   0,   0,   0,  34, 142,
     34,  35, 144, 145, 146, 147, 148, 149,   0,   0,   0,   0,  48,  49,  50, 150,
    151, 152,   8,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   7,   8,   9,
      8,   9,  49, 153,  35, 154,   2, 155, 156, 157,   9, 158, 159, 158, 160, 161,
    162, 163, 164, 165, 166, 167, 168, 169, 170,   0,   0,   0,   0,   0,   0,   0,
     34,  35, 144, 145, 171,   0,   0,   0,   0,   0,   0,   7,   8,   9,   1,   2,
    172,   8,   9,   1,   2, 172,   8,   9, 173,  49, 174,   0,   0,   0,   0,   0,
     70,   0,   0,   0,   0,   0,   0,   0,   0, 175,   0,   0,   0,   0,   0,   0,
     98,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,   0,   0,   0,   0,
      0,   0,  91,   0,   0,   0,   0,   0, 176,   0,   0,  88,   0,   0,   0,  88,
      0,   0, 101,   0,   0,   0,   0,  73,   0,   0,   0,   0,   0,   0,  73,   0,
      0,   0,   0,   0,   0,   0,  80,   0,   0,   0,   0,   0,   0,   0, 107,   0,
      0,   0,   0, 177,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0,
};

static RE_UINT8 re_numeric_value_stage_5[] = {
      0,   0,   0,   0,   3,  24,  26,  28,  30,  32,  34,  36,  38,  40,   0,   0,
      0,   0,  26,  28,   0,  24,   0,   0,  12,  16,  20,   0,   0,   0,   3,  24,
     26,  28,  30,  32,  34,  36,  38,  40,   4,   7,  10,  12,  20,  47,   0,   0,
      0,   0,  12,  16,  20,   4,   7,  10,  41,  86,  95,   0,  24,  26,  28,   0,
     41,  86,  95,  12,  16,  20,   0,   0,  38,  40,  16,  25,  27,  29,  31,  33,
     35,  37,  39,   2,   0,  24,  26,  28,  38,  40,  41,  51,  61,  71,  81,  82,
     83,  84,  85,  86, 104,   0,   0,   0,   0,   0,  48,  49,  50,   0,   0,   0,
     38,  40,  24,   0,   3,   0,   0,   0,   8,   6,   5,  13,  19,  11,  15,  17,
     21,   9,  22,   7,  14,  18,  23,  24,  24,  26,  28,  30,  32,  34,  36,  38,
     40,  41,  42,  43,  81,  86,  90,  95,  95,  99, 104,   0,   0,  34,  81, 108,
    113,   3,   0,   0,  44,  45,  46,  47,  48,  49,  50,  51,   0,   0,   3,  42,
     43,  44,  45,  46,  47,  48,  49,  50,  51,  24,  26,  28,  38,  40,  41,   3,
      0,   0,  24,  26,  28,  30,  32,  34,  36,  38,  40,  41,  40,  41,  24,  26,
      0,  16,   0,   0,   0,   0,   0,   3,  41,  51,  61,   0,  28,  30,   0,   0,
     40,  41,   0,   0,  41,  51,  61,  71,  81,  82,  83,  84,   0,  52,  53,  54,
     55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,   0,  67,  68,  69,
     70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,   0,  32,   0,   0,
      0,   0,   0,  26,   0,   0,  32,   0,   0,  36,   0,   0,  24,   0,   0,  36,
      0,   0,   0, 104,   0,  28,   0,   0,   0,  40,   0,   0,  26,   0,   0,   0,
     32,   0,  30,   0,   0,   0,   0, 116,  41,   0,   0,   0,   0,   0,   0,  95,
     28,   0,   0,   0,  86,   0,   0,   0, 116,   0,   0,   0,   0,   0, 117,   0,
      0,  26,   0,  38,   0,  34,   0,   0,   0,  41,   0,  95,  51,  61,   0,   0,
     71,   0,   0,   0,   0,  28,  28,  28,   0,   0,   0,  30,   0,   0,  24,   0,
      0,   0,  40,  51,   0,   0,  41,   0,  38,   0,   0,   0,   0,   0,  36,   0,
      0,   0,  40,   0,   0,   0,  86,   0,   0,   0,  30,   0,   0,   0,  26,   0,
      0,  95,   0,   0,   0,   0,  34,   0,  34,   0,   0,   0,   0,   0,   3,   0,
     36,  38,  40,   3,  12,  16,  20,   4,   7,  10,   0,   0,   0,   0,   0,  28,
      0,   0,   0,  41,   0,  34,   0,  34,   0,  41,   0,   0,   0,   0,   0,  24,
     85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100,
    101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,  12,  16,  24,  32,
     81,  90,  99, 108,  32,  41,  81,  86,  90,  95,  99,  32,  41,  81,  86,  90,
     95, 104, 108,  41,  24,  24,  24,  26,  26,  26,  26,  32,  41,  41,  41,  41,
     41,  61,  81,  81,  81,  81,  86,  88,  90,  90,  90,  90,  81,  16,  16,  19,
     20,   0,   0,   0,  24,  32,  41,  81,   0,  85,   0,   0,   0,   0,  94,   0,
      0,  24,  26,  41,  51,  86,   0,   0,  24,  26,  28,  41,  51,  86,  95, 104,
      0,   0,  24,  41,  51,  86,  26,  28,  41,  51,  86,  95,   0,  24,  81,   0,
     40,  41,  51,  61,  71,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,
     92,  93,  94,  16,  12,  13,  19,   0,  51,  61,  71,  81,  82,  83,  84,  85,
     86,  95,   3,  24,  36,  38,  40,  30,  40,  24,  26,  28,  38,  40,  24,  26,
     28,  30,  32,  26,  28,  28,  30,  32,  24,  26,  28,  28,  30,  32, 114, 115,
     30,  32,  28,  28,  30,  30,  30,  30,  34,  36,  36,  36,  38,  38,  40,  40,
     40,  40,  26,  28,  30,  32,  34,  24,  26,  28,  30,  30,  32,  32,   1,   1,
     24,  26,  13,  19,  22,  13,  19,   7,  12,   9,  12,   0,  84,  85,   0,   0,
     38,  40,   3,  24,   3,   3,  24,  26,  36,  38,  40,   0,  30,   0,   0,   0,
      0,   0,   0,  61,   0,  30,   0,   0,  40,   0,   0,   0,
};

/* Numeric_Value: 2876 bytes. */

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
    121,  59, 120, 112, 252, 255,   0,   0, 248, 255, 255, 249, 255, 255,   0,   1,
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
    19, 20,  0,  0,  0,  0, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,
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
     0,  0, 60, 61,  0,  0,  0,  0,  0,  0,  0, 62,  0,  0, 63, 64,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65,  0,
    66,  0,  0, 67,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0, 68, 69,  0,  0,  0,  0,  0,  0, 70,  0,  0,  0,  0,
     0,  0, 71, 72,  0,  0,  0,  0,  0,  0,  0, 73, 44,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 74, 69,  0,  0,  0,  0,
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
     1,  6, 53, 54, 55, 56, 33, 16,  0,  0,  0,  0,  0, 11,  5,  8,
     0,  0,  0, 43,  0,  0,  0,  0,  0, 15, 19,  7, 44, 25, 35,  0,
    57,  4,  9, 58,  0,  0, 10,  0,  0,  0,  0,  0,  0,  0,  0, 39,
     0,  0,  0,  0,  6,  6,  4,  4,  4,  6,  6, 16,  0,  0,  0,  0,
     2,  3,  5,  1,  3,  0,  0,  0,  0,  0,  0,  9,  6,  4, 40, 37,
    17, 59, 16,  0,  0,  0,  0,  0,  0, 15,  8,  4,  4,  4,  6, 18,
     0,  0,  0,  0,  0,  0,  9,  8,
};

static RE_UINT8 re_indic_matra_category_stage_5[] = {
     0,  0,  5,  1,  1,  2,  1,  6,  6,  6,  6,  5,  5,  5,  1,  1,
     2,  1,  0,  5,  6,  0,  0,  2,  2,  0,  0,  4,  4,  6,  0,  1,
     5,  0,  5,  6,  5,  8,  1,  5,  9,  0, 10,  6,  2,  2,  4,  4,
     4,  5,  1,  0,  7,  0,  8,  1,  8,  0,  8,  8,  9,  2,  4,  1,
     3,  3,  3,  1,  3,  0,  0,  6,  5,  7,  7,  7,  6,  2,  0, 14,
     2,  5,  9, 10,  4,  2, 14,  0,  6,  1,  1,  8,  8,  5, 14,  1,
     6, 11,  7, 12,  2,  9, 11,  0,  5,  2,  6,  3,  3,  5,  5,  3,
     1,  3,  0, 13, 13,  0,  6, 14,
};

/* Indic_Matra_Category: 1336 bytes. */

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
    21, 22,  1,  1,  1,  1, 23,  1,  1,  1,  1,  1,  1,  1,  1,  1,
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
     98,  78,   4,  99, 100,   0,   0,   0, 101,   4, 102, 103,   4,   4, 104, 105,
      4,   4, 106, 107, 108,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 109,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 110,   0,   0,   0,
    111,   4, 112,   0,   4, 113, 114, 115, 116, 117,   4, 118, 119,   0,   0,   0,
    120,   4, 121,   4, 122, 123,   0,   0, 124,   4,   4, 125, 126,   0,   0,   0,
    127,   4, 128, 129, 130,   0,   4, 131,   4,   4,   4, 132, 133,   0, 134, 135,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 136, 137, 138,   0,
    139, 140,   4, 141,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    142,  78,   4, 143, 144,   0,   0,   0, 145,   4,   4, 146,   0,   0,   0,   0,
    147,   4, 148, 149,   0,   0,   0,   0, 150, 151,   4, 152, 153,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0, 154,   4, 155, 156,   0,   0,   0,   0,
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
      8, 115,   5,   0, 116,   3, 117,   5, 118,   8, 119, 120,   0,   0, 121, 122,
      5, 123,   8,   8, 124,   0,   0,   0,   5, 125,   8, 106,  99, 126,   0,   0,
      0,   0,   0,  13, 127,   0,   0,   0,   0,   0,   0,   1,  33, 128, 129,   5,
    108,   8,   0,   0,   5,   5,   5, 130, 131, 132, 133,   5, 134,   0,   0,   0,
    135,   3,   3,   3, 117,   5,   5,   5,   5, 136,   8,   8,   8,  89,   0,   0,
      0,   0,  19,   5, 130, 102, 137, 107,   5, 108,   8, 138, 139,   0,   0,   0,
    140,   3,   4,  88, 141,   8,   8, 142,  89,   0,   0,   0,   3, 117,   5,   5,
      5,   5,  81,   8, 143, 144,   0,   0,  99,  99,  99, 145,  13,   0, 146,   0,
      8,   8,   8,  84, 147,   0,   0,   0, 117,   5, 108,   8,   0, 148,   0,   0,
      5,   5,   5,  74, 149,   5, 150,  99, 151,   8,  29, 152,  81,  45,   0, 153,
      5,  13,  13,   5,   5,   0,   0, 154, 155,  15,   3,   3,   5,   5,   8,   8,
      8,  53,   0,   0, 156,   3,   3,   4,   8,   8, 157,   0, 156,  88,   5,   5,
      5, 108,   8,   8, 158,  89,   0,   0, 156,   3,   3,   3,   4,   5,   5,   5,
    108,   8,   8,   8,  63,   0,   0,   0,   3,   3, 117,   5,   5,   5, 129, 159,
      8, 160,   0,   0,
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
     1, 12,  2,  6,  6,  6, 10, 10, 10, 13, 13, 13,  7,  7,  5,  5,
    13, 13, 10, 10,  0,  0,  3, 10, 10, 10, 15, 15,  6,  6,  4,  7,
    15, 15,  5,  5, 13, 13,  7,  7,  1,  1,  0,  4,  0,  0,  2,  2,
     6,  6,  5, 10, 10, 10, 10,  1, 10, 10,  8,  8,  8,  8, 10, 10,
    10, 10,  8, 13, 13, 10, 10, 10, 10, 13, 10,  1,  1,  2,  6,  6,
    15,  7,  7,  7,  8,  8,  8, 19,  7,  7,  7, 15, 15, 15, 15,  5,
     1,  1, 12,  2, 10, 10, 10,  4,  7, 13, 14, 14,  7,  7,  7, 14,
    14, 14, 14,  0, 15, 15,  0,  0,  0,  0, 10, 19, 18, 19, 18,  0,
     0,  2,  5,  0, 10,  6, 10, 10, 10, 10, 10, 15, 15, 15, 15,  7,
    19,  5,  0,  0,  7,  0,  1,  2,  0,  0,  0,  5,  1,  1,  2,  0,
     1,  1,  2,  6,  7,  5,  4,  0,  7,  7,  7,  5,  2,  7,  7,  7,
     7,  7,  5,  4,
};

/* Indic_Syllabic_Category: 1952 bytes. */

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
    return re_get_alphabetic(ch) || re_get_general_category(ch) == RE_PROP_ND;
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
    return re_get_general_category(ch) != RE_PROP_CN;
}

/* Blank. */

RE_UINT32 re_get_blank(RE_UINT32 ch) {
    return ch == 0x09 || re_get_general_category(ch) == RE_PROP_ZS;
}

/* Graph. */

RE_UINT32 re_get_graph(RE_UINT32 ch) {
    return !re_get_white_space(ch) &&
      (RE_GRAPH_MASK & (1 << re_get_general_category(ch))) == 0;
}

/* Print. */

RE_UINT32 re_get_print(RE_UINT32 ch) {
    return (re_get_graph(ch) || re_get_blank(ch)) &&
      re_get_general_category(ch) != RE_PROP_CC;
}

/* Word. */

RE_UINT32 re_get_word(RE_UINT32 ch) {
    return re_get_alphabetic(ch) || (RE_WORD_MASK & (1 <<
      re_get_general_category(ch))) != 0 || re_get_join_control(ch);
}

/* XDigit. */

RE_UINT32 re_get_xdigit(RE_UINT32 ch) {
    return re_get_general_category(ch) == RE_PROP_ND || re_get_hex_digit(ch);
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
    101, 102,   0, 103,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 104, 105,
    106,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  35, 107, 108,
    109, 109, 110, 110, 109, 111, 110, 112, 109, 109, 110, 110, 109, 109, 110, 110,
    109, 111, 110, 112, 113, 113, 114, 114, 109, 109, 110, 110, 115, 116, 117, 118,
    109, 109, 110, 110, 119,   0, 120, 121, 122,   0, 123, 124, 125,   0, 126,   0,
    127, 128, 129, 130, 122,   0, 131, 124,   0, 132, 133,   0, 134,   0,   0,   0,
      0,   0,   0, 135,   0,   0,   0,   0, 136, 136, 136, 136, 137, 137, 137, 137,
    138, 139,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 140, 141, 141,
    141, 141, 141, 141, 142, 142, 142, 142, 142, 142, 143,   0,   0,   0,   0,   0,
     96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  97,  99,  99,  99,  99,
     99,  99,  99,  99,  99,  99,  99, 100, 144, 145,  19, 146, 147, 148,   0, 149,
     16,   0, 138, 150,  63,   0,   0,   0, 151, 151, 151, 151, 151, 151, 151, 151,
    151, 152,   0, 153,   0,   0,   0,   0,  16,  16,  16,  35,   0,   0,   0,   0,
     16,  16,  16,  16,  16,  16,   0,   0,  63,  16,  16,  16,  63,  16,  16,  16,
     16,  16,  16,  16,   0,   0,  18, 154,  16,  16, 138, 155,  16,   0,   0,   0,
     16,  16, 156,   0,   0,   0,   0,   0,   0, 148,   0,   0,   0,   0,   0,   0,
      1,   2,   2,   2,   2,   2,   5,   0,   6,   7,   7,   7,   7,   7,  10,   0,
    157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 158, 158, 158, 158, 158, 158,
    158, 158, 158, 158,   0,   0,   0,   0,
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
      0,  62,  63,   0,  64,  65,   0,  66,   0,   0,   0,  65,   0,  67,  68,   0,
      0,  69,   0,   0,   0,  70,   0,   0,  71,   0,   0,  71,  71,  72,  73,  73,
     74,   0,   0,   0,   0,   0,  75,   0,   0,  76,   0,   0,   0,   0,  14,  15,
      0,   0,   0,  33,  33,  33,   0,   0,   0,   0,  77,   0,  78,  78,  78,   0,
     79,   0,  80,  80,  81,   1,  82,   1,   1,  83,   1,   1,  84,  85,  86,   1,
     87,   1,   1,   1,  88,  89,   0,  90,   1,   1,  91,   1,   1,  92,   1,   1,
     93,  94,  94,  94,  95,   5,  96,   5,   5,  97,   5,   5,  98,  99, 100,   5,
    101,   5,   5,   5, 102, 103, 104, 105,   5,   5, 106,   5,   5, 107,   5,   5,
    108, 109, 109, 110, 111, 112,   0,   0,   0, 113, 114, 115, 116, 117, 118,   0,
    119, 120,   0,  14,  15, 121,  14,  15,   0,  45,  45,  45, 122, 122, 122, 122,
    123, 123, 123, 123, 124,  14,  15,  14,  15,  14,  15, 125,   0, 126, 126, 126,
    126, 126, 126, 126, 126, 126, 126,   0,   0, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127,   0, 128, 128, 128, 128, 128, 128,   0, 128,   0, 128,   0,   0,
      0, 129,   0,   0,   0, 130,   0,   0, 131, 132,  14,  15,   0,   0,   0, 133,
      0,   0, 134,   0, 110, 110, 110, 110, 115, 115, 115, 115, 110, 110,   0,   0,
    115, 115,   0,   0,   0, 110,   0, 110,   0, 115,   0, 115, 135, 135, 136, 136,
    136, 136, 137, 137, 138, 138, 139, 139, 140, 140,   0,   0, 110, 110,   0, 141,
    115, 115, 142, 142, 143,   0, 144,   0,   0,   0,   0, 141, 145, 145, 145, 145,
    143,   0,   0,   0, 110, 110,   0, 146, 115, 115, 147, 147, 110, 110,   0, 148,
      0, 118,   0,   0, 115, 115, 149, 149, 121,   0,   0,   0, 150, 150, 151, 151,
      0,   0, 152,   0,   0,   0, 153, 154,   0,   0, 155,   0,   0,   0, 156,   0,
    157, 157, 157, 157, 158, 158, 158, 158,   0,   0,   0,  14,  15,   0,   0,   0,
      0,   0, 159, 159, 159, 159, 159, 159, 160, 160, 160, 160, 160, 160,   0,   0,
     14,  15, 161, 162, 163, 164, 165,  14,  15, 166, 167, 168, 169,   0,  14,  15,
      0,  14,  15,   0,   0,   0, 170, 170,  15,  14,  15,   0, 171, 171, 171, 171,
    171, 171,   0, 171,   0, 171,   0,   0,  15, 172,  14,  15,  15, 173,   0,   0,
     14,  15, 174,   0, 175, 175, 175, 175, 176, 176, 176, 176,
};

/* All_Cases: 1656 bytes. */

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
    { 42308,     0,     0},
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
    {-42308,     0,     0},
    {    40,     0,     0},
    {   -40,     0,     0},
};

/* All_Cases: 1062 bytes. */

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
    49, 49, 49, 49, 49, 49, 49, 49, 49, 50,  0, 51,  0,  0,  0,  0,
     5,  5,  5,  5,  5, 16, 52, 53,  0,  0, 54, 54,  0,  0, 54, 55,
     0,  0, 54, 54,  0,  0, 54, 54,  0,  0, 54, 55,  0,  0, 56, 56,
     0,  0, 54, 54,  0,  0,  0,  0,  0,  0, 54, 54,  0,  0, 57, 58,
     0,  0, 59, 60,  0,  0, 61,  0,  0,  0, 62, 63,  0,  0, 64, 60,
     0, 65, 66,  0, 67,  0,  0,  0, 68, 68, 68, 68,  0,  0,  0,  0,
    69,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 70, 71, 71,
    71, 71, 71, 71,  0,  0,  0,  0, 47, 47, 47, 47, 47, 47, 47, 47,
    47, 47, 47, 48,  0,  0,  0,  0, 72, 73,  7, 74, 75, 45,  0, 76,
     5,  0, 69, 45,  6,  0,  0,  0,  5,  5,  5, 16,  0,  0,  0,  0,
     5,  5,  5,  5,  5,  5,  0,  0,  6,  5,  5,  5,  6,  5,  5,  5,
     5,  5,  5,  5,  0,  0,  7, 77,  5,  5, 69, 78,  5,  0,  0,  0,
     5,  5, 79,  0,  0,  0,  0,  0, 80, 80, 80, 80, 80, 80, 80, 80,
    80, 80,  0,  0,  0,  0,  0,  0,
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
    47, 47, 47,  0, 48, 48, 48, 48, 48, 48,  0, 48,  0, 48,  0,  0,
     0,  0,  0, 49,  0,  0, 50,  0, 51, 51, 51, 51, 51, 51,  0,  0,
     0, 51,  0, 51, 51, 51, 52, 52, 53,  0, 54,  0, 55, 55, 55, 55,
    53,  0,  0,  0, 51, 51, 56, 56, 51, 51, 57, 57, 44,  0,  0,  0,
    58, 58, 59, 59,  0,  0, 60,  0,  0,  0, 61, 62,  0,  0, 63,  0,
    64, 64, 64, 64,  0,  0,  0,  3,  0,  0, 65, 65, 65, 65, 65, 65,
     3,  0, 66, 67, 68,  0,  0,  3,  0, 69, 70, 71, 72,  0,  3,  0,
     0,  0, 73, 73,  0, 74,  3,  0,  0, 75,  0,  0,  3,  0, 76,  0,
    77, 77, 77, 77,
};

/* Simple_Case_Folding: 1168 bytes. */

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
    -42308,
        40,
};

/* Simple_Case_Folding: 312 bytes. */

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
     55,  56,   0,  57,   0,   0,   0,   0,   6,   6,   6,   6,   6,  58,  59,  60,
      0,   0,  61,  61,   0,   0,  61,  62,   0,   0,  61,  61,   0,   0,  61,  61,
      0,   0,  61,  62,  63,  64,  65,  65,   0,   0,  61,  61,   0,   0,   0,   0,
     66,  66,  67,  67,  68,  68,  69,  69,  70,  70,  71,  71,  72,  73,  74,  75,
     76,  77,  78,  79,  80,  81,  82,   0,  83,  84,  85,  86,  87,  88,  89,  90,
      0,  91,  92,   0,  93,   0,   0,   0,  94,  94,  94,  94,   0,   0,   0,   0,
     95,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  96,  97,  97,
     97,  97,  97,  97,   0,   0,   0,   0,  52,  52,  52,  52,  52,  52,  52,  52,
     52,  52,  52,  53,   0,   0,   0,   0,  98,  99,   8, 100, 101,  50,   0, 102,
      6,   0,  95,  50,  26,   0,   0,   0,   6,   6,   6,  18,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   0,   0,  26,   6,   6,   6,  26,   6,   6,   6,
      6,   6,   6,   6,   0,   0,   8, 103,   6,   6,  95, 104,   6,   0,   0,   0,
      6,   6, 105,   0,   0,   0,   0,   0, 106, 107,   0,   0, 108, 109,   0,   0,
    110, 110, 110, 110, 110, 110, 110, 110, 110, 110,   0,   0,   0,   0,   0,   0,
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
     55,  55,   0,  55,   0,  55,   0,   0,   4,   0,  56,  57,  58,  59,  60,  61,
      0,   0,  62,   0,  63,  63,  63,  63,  63,  63,   0,   0,  64,   0,  65,   0,
     66,   0,  67,   0,   0,  63,   0,  63,  68,  68,  68,  68,  69,  69,  69,  69,
     70,  70,  70,  70,  71,  71,  71,  71,  72,  72,  72,  72,  73,  73,  73,  73,
      0,   0,  74,  75,  76,   0,  77,  78,  63,  63,  79,  79,  80,   0,  81,   0,
      0,   0,  82,  83,  84,   0,  85,  86,  87,  87,  87,  87,  88,   0,   0,   0,
      0,   0,  89,  90,   0,   0,  91,  92,  63,  63,  93,  93,   0,   0,  94,  95,
     96,   0,  97,  98,  63,  63,  99,  99,  50,   0,   0,   0,   0,   0, 100, 101,
    102,   0, 103, 104, 105, 105, 106, 106, 107,   0,   0,   0,   0,   0, 108,   0,
      0,   0, 109, 110,   0,   0, 111,   0, 112, 112, 112, 112,   0,   0,   0,   4,
      0,   0, 113, 113, 113, 113, 113, 113,   4,   0, 114, 115, 116,   0,   0,   4,
      0, 117, 118, 119, 120,   0,   4,   0,   0,   0, 121, 121,   0, 122,   4,   0,
      0, 123,   0,   0,   4,   0, 124,   0, 125, 126, 127, 128, 129, 130, 131,   0,
      0,   0,   0, 132, 133, 134, 135, 136, 137, 137, 137, 137,
};

/* Full_Case_Folding: 1328 bytes. */

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
    {-42308,    0,   0},
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

/* Full_Case_Folding: 1104 bytes. */

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
