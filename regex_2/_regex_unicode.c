/* For Unicode version 10.0.0 */

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
    "-1/2",
    "0",
    "1",
    "1/10",
    "1/12",
    "1/16",
    "1/160",
    "1/2",
    "1/20",
    "1/3",
    "1/4",
    "1/40",
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
    "1000000",
    "100000000",
    "10000000000",
    "1000000000000",
    "103",
    "107",
    "11",
    "11/12",
    "11/2",
    "118",
    "12",
    "122",
    "129",
    "13",
    "13/2",
    "130",
    "132",
    "133",
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
    "200000",
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
    "3/20",
    "3/4",
    "3/5",
    "3/8",
    "3/80",
    "30",
    "300",
    "3000",
    "30000",
    "300000",
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
    "400000",
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
    "5/12",
    "5/2",
    "5/6",
    "5/8",
    "50",
    "500",
    "5000",
    "50000",
    "500000",
    "6",
    "60",
    "600",
    "6000",
    "60000",
    "600000",
    "7",
    "7/12",
    "7/2",
    "7/8",
    "70",
    "700",
    "7000",
    "70000",
    "700000",
    "8",
    "80",
    "800",
    "8000",
    "80000",
    "800000",
    "84",
    "9",
    "9/2",
    "90",
    "900",
    "9000",
    "90000",
    "900000",
    "91",
    "A",
    "ABOVE",
    "ABOVELEFT",
    "ABOVERIGHT",
    "ADLAM",
    "ADLM",
    "AEGEANNUMBERS",
    "AFRICANFEH",
    "AFRICANNOON",
    "AFRICANQAF",
    "AGHB",
    "AHEX",
    "AHOM",
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
    "ANATOLIANHIEROGLYPHS",
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
    "ATBL",
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
    "BASS",
    "BASSAVAH",
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
    "BHAIKSUKI",
    "BHKS",
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
    "BOTTOMANDLEFT",
    "BOTTOMANDRIGHT",
    "BOUNDARYNEUTRAL",
    "BOXDRAWING",
    "BR",
    "BRAH",
    "BRAHMI",
    "BRAHMIJOININGNUMBER",
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
    "CANTILLATIONMARK",
    "CARI",
    "CARIAN",
    "CARRIAGERETURN",
    "CASED",
    "CASEDLETTER",
    "CASEIGNORABLE",
    "CAUCASIANALBANIAN",
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
    "CHEROKEESUP",
    "CHEROKEESUPPLEMENT",
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
    "CJKEXTE",
    "CJKEXTF",
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
    "CJKUNIFIEDIDEOGRAPHSEXTENSIONE",
    "CJKUNIFIEDIDEOGRAPHSEXTENSIONF",
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
    "COMBININGDIACRITICALMARKSEXTENDED",
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
    "CONSONANTKILLER",
    "CONSONANTMEDIAL",
    "CONSONANTPLACEHOLDER",
    "CONSONANTPRECEDINGREPHA",
    "CONSONANTPREFIXED",
    "CONSONANTSUBJOINED",
    "CONSONANTSUCCEEDINGREPHA",
    "CONSONANTWITHSTACKER",
    "CONTINGENTBREAK",
    "CONTROL",
    "CONTROLPICTURES",
    "COPT",
    "COPTIC",
    "COPTICEPACTNUMBERS",
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
    "CYRILLICEXTC",
    "CYRILLICEXTENDEDA",
    "CYRILLICEXTENDEDB",
    "CYRILLICEXTENDEDC",
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
    "DIACRITICALSEXT",
    "DIACRITICALSFORSYMBOLS",
    "DIACRITICALSSUP",
    "DIGIT",
    "DINGBATS",
    "DOMINO",
    "DOMINOTILES",
    "DOUBLEABOVE",
    "DOUBLEBELOW",
    "DOUBLEQUOTE",
    "DQ",
    "DSRT",
    "DT",
    "DUALJOINING",
    "DUPL",
    "DUPLOYAN",
    "E",
    "EA",
    "EARLYDYNASTICCUNEIFORM",
    "EASTASIANWIDTH",
    "EB",
    "EBASE",
    "EBASEGAZ",
    "EBG",
    "EGYP",
    "EGYPTIANHIEROGLYPHS",
    "ELBA",
    "ELBASAN",
    "EM",
    "EMODIFIER",
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
    "FIRSTSTRONGISOLATE",
    "FO",
    "FONT",
    "FORMAT",
    "FRA",
    "FRACTION",
    "FSI",
    "FULLWIDTH",
    "GAF",
    "GAMAL",
    "GAZ",
    "GC",
    "GCB",
    "GEMINATIONMARK",
    "GENERALCATEGORY",
    "GENERALPUNCTUATION",
    "GEOMETRICSHAPES",
    "GEOMETRICSHAPESEXT",
    "GEOMETRICSHAPESEXTENDED",
    "GEOR",
    "GEORGIAN",
    "GEORGIANSUP",
    "GEORGIANSUPPLEMENT",
    "GL",
    "GLAG",
    "GLAGOLITIC",
    "GLAGOLITICSUP",
    "GLAGOLITICSUPPLEMENT",
    "GLUE",
    "GLUEAFTERZWJ",
    "GONM",
    "GOTH",
    "GOTHIC",
    "GRAN",
    "GRANTHA",
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
    "HATR",
    "HATRAN",
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
    "HLUW",
    "HMNG",
    "HRKT",
    "HST",
    "HUNG",
    "HY",
    "HYPHEN",
    "ID",
    "IDC",
    "IDCONTINUE",
    "IDEO",
    "IDEOGRAPHIC",
    "IDEOGRAPHICDESCRIPTIONCHARACTERS",
    "IDEOGRAPHICSYMBOLS",
    "IDEOGRAPHICSYMBOLSANDPUNCTUATION",
    "IDS",
    "IDSB",
    "IDSBINARYOPERATOR",
    "IDST",
    "IDSTART",
    "IDSTRINARYOPERATOR",
    "IMPERIALARAMAIC",
    "IN",
    "INDICNUMBERFORMS",
    "INDICPOSITIONALCATEGORY",
    "INDICSYLLABICCATEGORY",
    "INFIXNUMERIC",
    "INHERITED",
    "INIT",
    "INITIAL",
    "INITIALPUNCTUATION",
    "INPC",
    "INSC",
    "INSCRIPTIONALPAHLAVI",
    "INSCRIPTIONALPARTHIAN",
    "INSEPARABLE",
    "INSEPERABLE",
    "INVISIBLESTACKER",
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
    "JOINER",
    "JOININGGROUP",
    "JOININGTYPE",
    "JT",
    "JV",
    "KA",
    "KAF",
    "KAITHI",
    "KALI",
    "KANA",
    "KANAEXTA",
    "KANAEXTENDEDA",
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
    "KHOJ",
    "KHOJKI",
    "KHUDAWADI",
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
    "LATINEXTE",
    "LATINEXTENDEDA",
    "LATINEXTENDEDADDITIONAL",
    "LATINEXTENDEDB",
    "LATINEXTENDEDC",
    "LATINEXTENDEDD",
    "LATINEXTENDEDE",
    "LATN",
    "LB",
    "LC",
    "LE",
    "LEADINGJAMO",
    "LEFT",
    "LEFTANDRIGHT",
    "LEFTJOINING",
    "LEFTTORIGHT",
    "LEFTTORIGHTEMBEDDING",
    "LEFTTORIGHTISOLATE",
    "LEFTTORIGHTOVERRIDE",
    "LEPC",
    "LEPCHA",
    "LETTER",
    "LETTERLIKESYMBOLS",
    "LETTERNUMBER",
    "LF",
    "LIMB",
    "LIMBU",
    "LINA",
    "LINB",
    "LINEARA",
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
    "LRI",
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
    "MAHAJANI",
    "MAHJ",
    "MAHJONG",
    "MAHJONGTILES",
    "MALAYALAM",
    "MALAYALAMBHA",
    "MALAYALAMJA",
    "MALAYALAMLLA",
    "MALAYALAMLLLA",
    "MALAYALAMNGA",
    "MALAYALAMNNA",
    "MALAYALAMNNNA",
    "MALAYALAMNYA",
    "MALAYALAMRA",
    "MALAYALAMSSA",
    "MALAYALAMTTA",
    "MAND",
    "MANDAIC",
    "MANDATORYBREAK",
    "MANI",
    "MANICHAEAN",
    "MANICHAEANALEPH",
    "MANICHAEANAYIN",
    "MANICHAEANBETH",
    "MANICHAEANDALETH",
    "MANICHAEANDHAMEDH",
    "MANICHAEANFIVE",
    "MANICHAEANGIMEL",
    "MANICHAEANHETH",
    "MANICHAEANHUNDRED",
    "MANICHAEANKAPH",
    "MANICHAEANLAMEDH",
    "MANICHAEANMEM",
    "MANICHAEANNUN",
    "MANICHAEANONE",
    "MANICHAEANPE",
    "MANICHAEANQOPH",
    "MANICHAEANRESH",
    "MANICHAEANSADHE",
    "MANICHAEANSAMEKH",
    "MANICHAEANTAW",
    "MANICHAEANTEN",
    "MANICHAEANTETH",
    "MANICHAEANTHAMEDH",
    "MANICHAEANTWENTY",
    "MANICHAEANWAW",
    "MANICHAEANYODH",
    "MANICHAEANZAYIN",
    "MARC",
    "MARCHEN",
    "MARK",
    "MASARAMGONDI",
    "MATH",
    "MATHALPHANUM",
    "MATHEMATICALALPHANUMERICSYMBOLS",
    "MATHEMATICALOPERATORS",
    "MATHOPERATORS",
    "MATHSYMBOL",
    "MAYBE",
    "MB",
    "MC",
    "ME",
    "MED",
    "MEDIAL",
    "MEEM",
    "MEETEIMAYEK",
    "MEETEIMAYEKEXT",
    "MEETEIMAYEKEXTENSIONS",
    "MEND",
    "MENDEKIKAKUI",
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
    "MODI",
    "MODIFIERLETTER",
    "MODIFIERLETTERS",
    "MODIFIERSYMBOL",
    "MODIFIERTONELETTERS",
    "MODIFYINGLETTER",
    "MONG",
    "MONGOLIAN",
    "MONGOLIANSUP",
    "MONGOLIANSUPPLEMENT",
    "MRO",
    "MROO",
    "MTEI",
    "MULT",
    "MULTANI",
    "MUSIC",
    "MUSICALSYMBOLS",
    "MYANMAR",
    "MYANMAREXTA",
    "MYANMAREXTB",
    "MYANMAREXTENDEDA",
    "MYANMAREXTENDEDB",
    "MYMR",
    "N",
    "N&",
    "NA",
    "NABATAEAN",
    "NAN",
    "NAR",
    "NARB",
    "NARROW",
    "NB",
    "NBAT",
    "NCHAR",
    "ND",
    "NEUTRAL",
    "NEWA",
    "NEWLINE",
    "NEWTAILUE",
    "NEXTLINE",
    "NFCQC",
    "NFCQUICKCHECK",
    "NFDQC",
    "NFDQUICKCHECK",
    "NFKCQC",
    "NFKCQUICKCHECK",
    "NFKDQC",
    "NFKDQUICKCHECK",
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
    "NONJOINER",
    "NONJOINING",
    "NONSPACINGMARK",
    "NONSTARTER",
    "NOON",
    "NOTAPPLICABLE",
    "NOTREORDERED",
    "NR",
    "NS",
    "NSHU",
    "NSM",
    "NT",
    "NU",
    "NUKTA",
    "NUMBER",
    "NUMBERFORMS",
    "NUMBERJOINER",
    "NUMERIC",
    "NUMERICTYPE",
    "NUMERICVALUE",
    "NUN",
    "NUSHU",
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
    "OLDHUNGARIAN",
    "OLDITALIC",
    "OLDNORTHARABIAN",
    "OLDPERMIC",
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
    "ORNAMENTALDINGBATS",
    "ORYA",
    "OSAGE",
    "OSGE",
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
    "PAHAWHHMONG",
    "PALM",
    "PALMYRENE",
    "PARAGRAPHSEPARATOR",
    "PATSYN",
    "PATTERNSYNTAX",
    "PATTERNWHITESPACE",
    "PATWS",
    "PAUC",
    "PAUCINHAU",
    "PC",
    "PCM",
    "PD",
    "PDF",
    "PDI",
    "PE",
    "PERM",
    "PF",
    "PHAG",
    "PHAGSPA",
    "PHAISTOS",
    "PHAISTOSDISC",
    "PHLI",
    "PHLP",
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
    "POPDIRECTIONALISOLATE",
    "POSIXALNUM",
    "POSIXDIGIT",
    "POSIXPUNCT",
    "POSIXXDIGIT",
    "POSTFIXNUMERIC",
    "PP",
    "PR",
    "PREFIXNUMERIC",
    "PREPEND",
    "PREPENDEDCONCATENATIONMARK",
    "PRINT",
    "PRIVATEUSE",
    "PRIVATEUSEAREA",
    "PRTI",
    "PS",
    "PSALTERPAHLAVI",
    "PUA",
    "PUNCT",
    "PUNCTUATION",
    "PUREKILLER",
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
    "RIGHTTOLEFTISOLATE",
    "RIGHTTOLEFTOVERRIDE",
    "RJNG",
    "RLE",
    "RLI",
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
    "SENTENCETERMINAL",
    "SEP",
    "SEPARATOR",
    "SG",
    "SGNW",
    "SHARADA",
    "SHAVIAN",
    "SHAW",
    "SHIN",
    "SHORTHANDFORMATCONTROLS",
    "SHRD",
    "SIDD",
    "SIDDHAM",
    "SIGNWRITING",
    "SIND",
    "SINGLEQUOTE",
    "SINH",
    "SINHALA",
    "SINHALAARCHAICNUMBERS",
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
    "SOYO",
    "SOYOMBO",
    "SP",
    "SPACE",
    "SPACESEPARATOR",
    "SPACINGMARK",
    "SPACINGMODIFIERLETTERS",
    "SPECIALS",
    "SQ",
    "SQR",
    "SQUARE",
    "ST",
    "STERM",
    "STRAIGHTWAW",
    "SUB",
    "SUND",
    "SUNDANESE",
    "SUNDANESESUP",
    "SUNDANESESUPPLEMENT",
    "SUP",
    "SUPARROWSA",
    "SUPARROWSB",
    "SUPARROWSC",
    "SUPER",
    "SUPERANDSUB",
    "SUPERSCRIPTSANDSUBSCRIPTS",
    "SUPMATHOPERATORS",
    "SUPPLEMENTALARROWSA",
    "SUPPLEMENTALARROWSB",
    "SUPPLEMENTALARROWSC",
    "SUPPLEMENTALMATHEMATICALOPERATORS",
    "SUPPLEMENTALPUNCTUATION",
    "SUPPLEMENTALSYMBOLSANDPICTOGRAPHS",
    "SUPPLEMENTARYPRIVATEUSEAREAA",
    "SUPPLEMENTARYPRIVATEUSEAREAB",
    "SUPPUAA",
    "SUPPUAB",
    "SUPPUNCTUATION",
    "SUPSYMBOLSANDPICTOGRAPHS",
    "SURROGATE",
    "SUTTONSIGNWRITING",
    "SWASHKAF",
    "SY",
    "SYLLABLEMODIFIER",
    "SYLO",
    "SYLOTINAGRI",
    "SYMBOL",
    "SYRC",
    "SYRIAC",
    "SYRIACSUP",
    "SYRIACSUPPLEMENT",
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
    "TANG",
    "TANGUT",
    "TANGUTCOMPONENTS",
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
    "TIRH",
    "TIRHUTA",
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
    "WARA",
    "WARANGCITI",
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
    "ZANABAZARSQUARE",
    "ZANB",
    "ZHAIN",
    "ZINH",
    "ZL",
    "ZP",
    "ZS",
    "ZW",
    "ZWJ",
    "ZWSPACE",
    "ZYYY",
    "ZZZZ",
};

/* strings: 13038 bytes. */

/* properties. */

RE_Property re_properties[] = {
    { 571,  0,  0},
    { 568,  0,  0},
    { 264,  1,  1},
    { 263,  1,  1},
    {1145,  2,  2},
    {1143,  2,  2},
    {1331,  3,  3},
    {1326,  3,  3},
    { 594,  4,  4},
    { 569,  4,  4},
    {1151,  5,  5},
    {1142,  5,  5},
    { 869,  6,  6},
    { 182,  7,  6},
    { 181,  7,  6},
    { 799,  8,  6},
    { 798,  8,  6},
    {1299,  9,  6},
    {1298,  9,  6},
    { 307, 10,  6},
    { 309, 11,  6},
    { 363, 11,  6},
    { 356, 12,  6},
    { 448, 12,  6},
    { 358, 13,  6},
    { 450, 13,  6},
    { 357, 14,  6},
    { 449, 14,  6},
    { 354, 15,  6},
    { 446, 15,  6},
    { 355, 16,  6},
    { 447, 16,  6},
    { 666, 17,  6},
    { 662, 17,  6},
    { 656, 18,  6},
    { 655, 18,  6},
    {1339, 19,  6},
    {1338, 19,  6},
    {1337, 20,  6},
    {1336, 20,  6},
    { 475, 21,  6},
    { 483, 21,  6},
    { 595, 22,  6},
    { 603, 22,  6},
    { 593, 23,  6},
    { 597, 23,  6},
    { 596, 24,  6},
    { 604, 24,  6},
    {1327, 25,  6},
    {1334, 25,  6},
    {1184, 25,  6},
    { 256, 26,  6},
    { 254, 26,  6},
    { 701, 27,  6},
    { 699, 27,  6},
    { 468, 28,  6},
    { 653, 29,  6},
    {1108, 30,  6},
    {1105, 30,  6},
    {1260, 31,  6},
    {1259, 31,  6},
    {1033, 32,  6},
    {1012, 32,  6},
    { 640, 33,  6},
    { 639, 33,  6},
    { 214, 34,  6},
    { 170, 34,  6},
    {1026, 35,  6},
    { 993, 35,  6},
    { 658, 36,  6},
    { 657, 36,  6},
    { 485, 37,  6},
    { 484, 37,  6},
    { 546, 38,  6},
    { 544, 38,  6},
    {1032, 39,  6},
    {1011, 39,  6},
    {1038, 40,  6},
    {1039, 40,  6},
    { 967, 41,  6},
    { 944, 41,  6},
    {1028, 42,  6},
    { 998, 42,  6},
    { 664, 43,  6},
    { 663, 43,  6},
    { 667, 44,  6},
    { 665, 44,  6},
    {1110, 45,  6},
    {1295, 46,  6},
    {1291, 46,  6},
    {1027, 47,  6},
    { 995, 47,  6},
    { 477, 48,  6},
    { 476, 48,  6},
    {1178, 49,  6},
    {1146, 49,  6},
    { 797, 50,  6},
    { 796, 50,  6},
    {1030, 51,  6},
    {1000, 51,  6},
    {1029, 52,  6},
    { 999, 52,  6},
    {1152, 53,  6},
    {1193, 53,  6},
    {1304, 54,  6},
    {1320, 54,  6},
    {1051, 55,  6},
    {1052, 55,  6},
    {1050, 56,  6},
    {1049, 56,  6},
    {1090, 57,  6},
    {1056, 57,  6},
    {1111, 58,  6},
    {1116, 58,  6},
    { 626, 59,  7},
    { 650, 59,  7},
    { 255, 60,  8},
    { 244, 60,  8},
    { 301, 61,  9},
    { 313, 61,  9},
    { 474, 62, 10},
    { 499, 62, 10},
    { 506, 63, 11},
    { 504, 63, 11},
    { 703, 64, 12},
    { 697, 64, 12},
    { 704, 65, 13},
    { 705, 65, 13},
    { 789, 66, 14},
    { 764, 66, 14},
    { 987, 67, 15},
    { 980, 67, 15},
    { 988, 68, 16},
    { 991, 68, 16},
    { 258, 69,  6},
    { 257, 69,  6},
    { 671, 70, 17},
    { 678, 70, 17},
    { 672, 71, 18},
    { 679, 71, 18},
    { 954, 72, 19},
    { 953, 72, 19},
    { 952, 73, 20},
    { 951, 73, 20},
    { 958, 74, 19},
    { 957, 74, 19},
    { 956, 75, 20},
    { 955, 75, 20},
    { 185, 76,  6},
    { 180, 76,  6},
    { 193, 77,  6},
    { 262, 78,  6},
    { 592, 79,  6},
    {1091, 80,  6},
    {1330, 81,  6},
    {1335, 82,  6},
    {1082, 83,  6},
    {1081, 84,  6},
    {1083, 85,  6},
    {1084, 86,  6},
};

/* properties: 640 bytes. */

/* property values. */

RE_PropertyValue re_property_values[] = {
    {1292,  0,   0},
    { 398,  0,   0},
    {1300,  0,   1},
    { 806,  0,   1},
    { 800,  0,   2},
    { 793,  0,   2},
    {1272,  0,   3},
    { 805,  0,   3},
    { 912,  0,   4},
    { 794,  0,   4},
    {1031,  0,   5},
    { 795,  0,   5},
    { 971,  0,   6},
    { 910,  0,   6},
    { 528,  0,   7},
    { 878,  0,   7},
    {1186,  0,   8},
    { 877,  0,   8},
    { 473,  0,   9},
    { 945,  0,   9},
    { 490,  0,   9},
    { 779,  0,  10},
    { 962,  0,  10},
    {1035,  0,  11},
    { 963,  0,  11},
    {1185,  0,  12},
    {1365,  0,  12},
    { 791,  0,  13},
    {1363,  0,  13},
    {1048,  0,  14},
    {1364,  0,  14},
    { 430,  0,  15},
    { 312,  0,  15},
    { 399,  0,  15},
    { 560,  0,  16},
    { 351,  0,  16},
    {1092,  0,  17},
    { 400,  0,  17},
    {1220,  0,  18},
    { 440,  0,  18},
    { 469,  0,  19},
    {1057,  0,  19},
    {1015,  0,  20},
    {1095,  0,  20},
    { 396,  0,  21},
    {1060,  0,  21},
    { 416,  0,  22},
    {1055,  0,  22},
    {1036,  0,  23},
    {1078,  0,  23},
    { 874,  0,  24},
    {1172,  0,  24},
    { 444,  0,  25},
    {1143,  0,  25},
    { 914,  0,  26},
    {1171,  0,  26},
    {1037,  0,  27},
    {1177,  0,  27},
    { 677,  0,  28},
    {1075,  0,  28},
    { 555,  0,  29},
    {1062,  0,  29},
    {1025,  0,  30},
    { 294,  0,  30},
    { 295,  0,  30},
    { 777,  0,  31},
    { 740,  0,  31},
    { 741,  0,  31},
    { 867,  0,  32},
    { 815,  0,  32},
    { 407,  0,  32},
    { 816,  0,  32},
    { 983,  0,  33},
    { 934,  0,  33},
    { 935,  0,  33},
    {1099,  0,  34},
    {1043,  0,  34},
    {1098,  0,  34},
    {1044,  0,  34},
    {1227,  0,  35},
    {1132,  0,  35},
    {1133,  0,  35},
    {1154,  0,  36},
    {1356,  0,  36},
    {1357,  0,  36},
    { 308,  0,  37},
    { 765,  0,  37},
    { 215,  0,  38},
    { 964,  1,   0},
    { 942,  1,   0},
    { 238,  1,   1},
    { 213,  1,   1},
    { 750,  1,   2},
    { 749,  1,   2},
    { 748,  1,   2},
    { 757,  1,   3},
    { 751,  1,   3},
    { 759,  1,   4},
    { 753,  1,   4},
    { 687,  1,   5},
    { 686,  1,   5},
    {1187,  1,   6},
    { 913,  1,   6},
    { 402,  1,   7},
    { 486,  1,   7},
    { 599,  1,   8},
    { 598,  1,   8},
    { 453,  1,   9},
    { 461,  1,  10},
    { 460,  1,  10},
    { 462,  1,  10},
    { 209,  1,  11},
    { 634,  1,  12},
    { 196,  1,  13},
    {1229,  1,  14},
    { 208,  1,  15},
    { 207,  1,  15},
    {1265,  1,  16},
    { 960,  1,  17},
    {1137,  1,  18},
    { 834,  1,  19},
    {1231,  1,  20},
    {1230,  1,  20},
    { 198,  1,  21},
    { 197,  1,  21},
    { 480,  1,  22},
    { 250,  1,  23},
    { 607,  1,  24},
    { 605,  1,  25},
    {1017,  1,  26},
    {1248,  1,  27},
    {1258,  1,  28},
    { 720,  1,  29},
    { 821,  1,  30},
    {1169,  1,  31},
    {1266,  1,  32},
    { 745,  1,  33},
    {1267,  1,  34},
    { 928,  1,  35},
    { 577,  1,  36},
    { 622,  1,  37},
    { 692,  1,  37},
    { 532,  1,  38},
    { 538,  1,  39},
    { 537,  1,  39},
    { 360,  1,  40},
    {1293,  1,  41},
    {1287,  1,  41},
    { 299,  1,  41},
    { 997,  1,  42},
    {1130,  1,  43},
    {1234,  1,  44},
    { 629,  1,  45},
    { 290,  1,  46},
    {1236,  1,  47},
    { 730,  1,  48},
    { 918,  1,  49},
    {1294,  1,  50},
    {1288,  1,  50},
    { 782,  1,  51},
    {1239,  1,  52},
    { 949,  1,  53},
    { 731,  1,  54},
    { 288,  1,  55},
    {1240,  1,  56},
    { 403,  1,  57},
    { 487,  1,  57},
    { 233,  1,  58},
    {1197,  1,  59},
    { 241,  1,  60},
    { 776,  1,  61},
    {1001,  1,  62},
    { 459,  1,  63},
    { 456,  1,  63},
    {1199,  1,  64},
    {1198,  1,  64},
    {1308,  1,  65},
    {1307,  1,  65},
    {1072,  1,  66},
    {1071,  1,  66},
    {1073,  1,  67},
    {1074,  1,  67},
    { 405,  1,  68},
    { 489,  1,  68},
    { 758,  1,  69},
    { 752,  1,  69},
    { 601,  1,  70},
    { 600,  1,  70},
    { 572,  1,  71},
    {1099,  1,  71},
    {1206,  1,  72},
    {1205,  1,  72},
    { 445,  1,  73},
    { 404,  1,  74},
    { 488,  1,  74},
    { 408,  1,  74},
    { 778,  1,  75},
    { 984,  1,  76},
    { 212,  1,  77},
    { 872,  1,  78},
    { 873,  1,  78},
    { 902,  1,  79},
    { 907,  1,  79},
    { 431,  1,  80},
    {1016,  1,  81},
    { 994,  1,  81},
    { 521,  1,  82},
    { 520,  1,  82},
    { 275,  1,  83},
    { 265,  1,  84},
    { 573,  1,  85},
    { 899,  1,  86},
    { 906,  1,  86},
    { 491,  1,  87},
    { 897,  1,  88},
    { 903,  1,  88},
    {1208,  1,  89},
    {1201,  1,  89},
    { 282,  1,  90},
    { 281,  1,  90},
    {1209,  1,  91},
    {1202,  1,  91},
    { 898,  1,  92},
    { 904,  1,  92},
    {1211,  1,  93},
    {1207,  1,  93},
    { 900,  1,  94},
    { 896,  1,  94},
    { 582,  1,  95},
    { 760,  1,  96},
    { 754,  1,  96},
    { 433,  1,  97},
    { 579,  1,  98},
    { 578,  1,  98},
    {1269,  1,  99},
    { 535,  1, 100},
    { 533,  1, 100},
    { 457,  1, 101},
    { 454,  1, 101},
    {1212,  1, 102},
    {1218,  1, 102},
    { 382,  1, 103},
    { 381,  1, 103},
    { 719,  1, 104},
    { 718,  1, 104},
    { 659,  1, 105},
    { 655,  1, 105},
    { 385,  1, 106},
    { 384,  1, 106},
    { 645,  1, 107},
    { 722,  1, 108},
    { 268,  1, 109},
    { 621,  1, 110},
    { 413,  1, 110},
    { 717,  1, 111},
    { 270,  1, 112},
    { 269,  1, 112},
    { 383,  1, 113},
    { 725,  1, 114},
    { 723,  1, 114},
    { 525,  1, 115},
    { 524,  1, 115},
    { 369,  1, 116},
    { 367,  1, 116},
    { 387,  1, 117},
    { 375,  1, 117},
    {1351,  1, 118},
    {1350,  1, 118},
    { 386,  1, 119},
    { 366,  1, 119},
    {1353,  1, 120},
    {1352,  1, 121},
    { 792,  1, 122},
    {1302,  1, 123},
    { 458,  1, 124},
    { 455,  1, 124},
    { 235,  1, 125},
    { 915,  1, 126},
    { 761,  1, 127},
    { 755,  1, 127},
    {1226,  1, 128},
    { 410,  1, 129},
    { 670,  1, 129},
    {1064,  1, 130},
    {1141,  1, 131},
    { 482,  1, 132},
    { 481,  1, 132},
    { 726,  1, 133},
    {1114,  1, 134},
    { 623,  1, 135},
    { 693,  1, 135},
    { 696,  1, 136},
    { 932,  1, 137},
    { 930,  1, 137},
    { 353,  1, 138},
    { 931,  1, 139},
    { 929,  1, 139},
    {1241,  1, 140},
    { 884,  1, 141},
    { 883,  1, 141},
    { 536,  1, 142},
    { 534,  1, 142},
    { 762,  1, 143},
    { 756,  1, 143},
    { 362,  1, 144},
    { 361,  1, 144},
    { 882,  1, 145},
    { 625,  1, 146},
    { 620,  1, 146},
    { 624,  1, 147},
    { 694,  1, 147},
    { 643,  1, 148},
    { 641,  1, 149},
    { 642,  1, 149},
    { 801,  1, 150},
    {1093,  1, 151},
    {1097,  1, 151},
    {1092,  1, 151},
    { 371,  1, 152},
    { 373,  1, 152},
    { 184,  1, 153},
    { 183,  1, 153},
    { 205,  1, 154},
    { 203,  1, 154},
    {1305,  1, 155},
    {1320,  1, 155},
    {1311,  1, 156},
    { 406,  1, 157},
    { 614,  1, 157},
    { 370,  1, 158},
    { 368,  1, 158},
    {1175,  1, 159},
    {1174,  1, 159},
    { 206,  1, 160},
    { 204,  1, 160},
    { 616,  1, 161},
    { 613,  1, 161},
    {1188,  1, 162},
    { 788,  1, 163},
    { 787,  1, 164},
    { 165,  1, 165},
    { 191,  1, 166},
    { 192,  1, 167},
    {1066,  1, 168},
    {1065,  1, 168},
    { 812,  1, 169},
    { 305,  1, 170},
    { 434,  1, 171},
    {1004,  1, 172},
    { 589,  1, 173},
    {1006,  1, 174},
    {1290,  1, 175},
    {1007,  1, 176},
    { 478,  1, 177},
    {1158,  1, 178},
    {1024,  1, 179},
    {1021,  1, 180},
    { 514,  1, 181},
    { 310,  1, 182},
    { 785,  1, 183},
    { 452,  1, 184},
    { 668,  1, 185},
    {1047,  1, 186},
    { 937,  1, 187},
    { 631,  1, 188},
    {1070,  1, 189},
    { 814,  1, 190},
    { 890,  1, 191},
    { 889,  1, 192},
    { 729,  1, 193},
    {1008,  1, 194},
    {1005,  1, 195},
    { 837,  1, 196},
    { 227,  1, 197},
    { 681,  1, 198},
    { 680,  1, 199},
    {1096,  1, 200},
    {1009,  1, 201},
    {1003,  1, 202},
    {1129,  1, 203},
    {1128,  1, 203},
    { 278,  1, 204},
    { 709,  1, 205},
    {1180,  1, 206},
    { 352,  1, 207},
    { 817,  1, 208},
    {1157,  1, 209},
    {1170,  1, 210},
    { 734,  1, 211},
    { 925,  1, 212},
    { 735,  1, 213},
    { 591,  1, 214},
    { 947,  1, 215},
    {1271,  1, 216},
    {1164,  1, 217},
    { 911,  1, 218},
    { 920,  1, 219},
    { 919,  1, 219},
    {1245,  1, 220},
    { 171,  1, 221},
    {1324,  1, 222},
    {1359,  1, 223},
    {1182,  1, 224},
    {1054,  1, 225},
    { 252,  1, 226},
    { 866,  1, 227},
    { 868,  1, 228},
    { 441,  1, 229},
    { 443,  1, 230},
    { 442,  1, 230},
    { 505,  1, 231},
    { 512,  1, 232},
    { 188,  1, 233},
    { 237,  1, 234},
    { 236,  1, 234},
    { 921,  1, 235},
    { 240,  1, 236},
    {1045,  1, 237},
    { 891,  1, 238},
    { 661,  1, 239},
    { 660,  1, 239},
    {1251,  1, 240},
    {1252,  1, 241},
    { 715,  1, 242},
    { 714,  1, 242},
    { 713,  1, 243},
    { 712,  1, 243},
    { 990,  1, 244},
    { 502,  1, 245},
    {1161,  1, 246},
    { 293,  1, 247},
    { 292,  1, 247},
    { 927,  1, 248},
    { 926,  1, 248},
    { 190,  1, 249},
    { 189,  1, 249},
    {1243,  1, 250},
    {1242,  1, 250},
    { 436,  1, 251},
    { 435,  1, 251},
    { 871,  1, 252},
    { 870,  1, 252},
    {1221,  1, 253},
    { 584,  1, 254},
    { 583,  1, 254},
    { 886,  1, 255},
    { 163,  1, 256},
    { 201,  1, 257},
    { 200,  1, 257},
    { 820,  1, 258},
    { 819,  1, 258},
    { 493,  1, 259},
    { 492,  1, 259},
    {1076,  1, 260},
    { 522,  1, 261},
    { 523,  1, 261},
    { 527,  1, 262},
    { 526,  1, 262},
    { 901,  1, 263},
    { 905,  1, 263},
    { 517,  1, 264},
    {1019,  1, 265},
    {1284,  1, 266},
    {1283,  1, 266},
    { 177,  1, 267},
    { 176,  1, 267},
    { 575,  1, 268},
    { 574,  1, 268},
    {1210,  1, 269},
    {1203,  1, 269},
    {1213,  1, 270},
    {1219,  1, 270},
    { 388,  1, 271},
    { 376,  1, 271},
    { 389,  1, 272},
    { 377,  1, 272},
    { 390,  1, 273},
    { 378,  1, 273},
    { 391,  1, 274},
    { 379,  1, 274},
    { 392,  1, 275},
    { 380,  1, 275},
    { 372,  1, 276},
    { 374,  1, 276},
    {1237,  1, 277},
    {1306,  1, 278},
    {1321,  1, 278},
    {1214,  1, 279},
    {1216,  1, 279},
    {1215,  1, 280},
    {1217,  1, 280},
    {1296,  2,   0},
    {1370,  2,   0},
    { 409,  2,   1},
    {1369,  2,   1},
    { 747,  2,   2},
    { 763,  2,   2},
    { 598,  2,   3},
    { 602,  2,   3},
    { 453,  2,   4},
    { 463,  2,   4},
    { 209,  2,   5},
    { 211,  2,   5},
    { 634,  2,   6},
    { 633,  2,   6},
    { 196,  2,   7},
    { 195,  2,   7},
    {1229,  2,   8},
    {1228,  2,   8},
    {1265,  2,   9},
    {1264,  2,   9},
    { 480,  2,  10},
    { 479,  2,  10},
    { 250,  2,  11},
    { 249,  2,  11},
    { 607,  2,  12},
    { 608,  2,  12},
    { 605,  2,  13},
    { 606,  2,  13},
    {1017,  2,  14},
    {1020,  2,  14},
    {1248,  2,  15},
    {1249,  2,  15},
    {1258,  2,  16},
    {1257,  2,  16},
    { 720,  2,  17},
    { 736,  2,  17},
    { 821,  2,  18},
    { 909,  2,  18},
    {1169,  2,  19},
    {1168,  2,  19},
    {1266,  2,  20},
    { 745,  2,  21},
    { 746,  2,  21},
    {1267,  2,  22},
    {1268,  2,  22},
    { 928,  2,  23},
    { 933,  2,  23},
    { 577,  2,  24},
    { 576,  2,  24},
    { 620,  2,  25},
    { 619,  2,  25},
    { 532,  2,  26},
    { 531,  2,  26},
    { 360,  2,  27},
    { 359,  2,  27},
    { 298,  2,  28},
    { 302,  2,  28},
    { 997,  2,  29},
    { 996,  2,  29},
    {1130,  2,  30},
    {1131,  2,  30},
    { 730,  2,  31},
    { 732,  2,  31},
    { 918,  2,  32},
    { 917,  2,  32},
    { 645,  2,  33},
    { 644,  2,  33},
    { 722,  2,  34},
    { 711,  2,  34},
    { 268,  2,  35},
    { 267,  2,  35},
    { 618,  2,  36},
    { 627,  2,  36},
    {1348,  2,  37},
    {1349,  2,  37},
    {1004,  2,  38},
    { 691,  2,  38},
    { 589,  2,  39},
    { 588,  2,  39},
    { 478,  2,  40},
    { 498,  2,  40},
    { 674,  2,  41},
    {1362,  2,  41},
    {1102,  2,  41},
    {1234,  2,  42},
    {1263,  2,  42},
    { 629,  2,  43},
    { 628,  2,  43},
    { 290,  2,  44},
    { 289,  2,  44},
    {1236,  2,  45},
    {1235,  2,  45},
    { 782,  2,  46},
    { 781,  2,  46},
    {1239,  2,  47},
    {1246,  2,  47},
    { 786,  2,  48},
    { 784,  2,  48},
    {1290,  2,  49},
    {1289,  2,  49},
    {1158,  2,  50},
    {1159,  2,  50},
    {1024,  2,  51},
    {1023,  2,  51},
    { 451,  2,  52},
    { 438,  2,  52},
    { 281,  2,  53},
    { 280,  2,  53},
    { 288,  2,  54},
    { 287,  2,  54},
    { 433,  2,  55},
    { 432,  2,  55},
    {1101,  2,  55},
    { 949,  2,  56},
    {1247,  2,  56},
    { 582,  2,  57},
    { 581,  2,  57},
    {1269,  2,  58},
    {1262,  2,  58},
    {1226,  2,  59},
    {1225,  2,  59},
    {1007,  2,  60},
    {1340,  2,  60},
    { 729,  2,  61},
    { 728,  2,  61},
    { 233,  2,  62},
    { 232,  2,  62},
    { 441,  2,  63},
    {1341,  2,  63},
    {1070,  2,  64},
    {1069,  2,  64},
    {1064,  2,  65},
    {1063,  2,  65},
    { 960,  2,  66},
    { 961,  2,  66},
    {1197,  2,  67},
    {1196,  2,  67},
    { 776,  2,  68},
    { 775,  2,  68},
    {1001,  2,  69},
    {1002,  2,  69},
    {1302,  2,  70},
    {1303,  2,  70},
    {1141,  2,  71},
    {1140,  2,  71},
    { 726,  2,  72},
    { 710,  2,  72},
    {1114,  2,  73},
    {1123,  2,  73},
    { 812,  2,  74},
    { 811,  2,  74},
    { 305,  2,  75},
    { 304,  2,  75},
    { 814,  2,  76},
    { 813,  2,  76},
    { 353,  2,  77},
    {1240,  2,  78},
    { 744,  2,  78},
    {1241,  2,  79},
    {1253,  2,  79},
    { 227,  2,  80},
    { 228,  2,  80},
    { 512,  2,  81},
    { 511,  2,  81},
    {1137,  2,  82},
    {1138,  2,  82},
    { 792,  2,  83},
    { 235,  2,  84},
    { 234,  2,  84},
    { 696,  2,  85},
    { 695,  2,  85},
    { 882,  2,  86},
    { 923,  2,  86},
    { 668,  2,  87},
    { 210,  2,  87},
    {1008,  2,  88},
    {1139,  2,  88},
    { 681,  2,  89},
    {1094,  2,  89},
    { 680,  2,  90},
    {1067,  2,  90},
    {1009,  2,  91},
    {1018,  2,  91},
    { 709,  2,  92},
    { 738,  2,  92},
    { 241,  2,  93},
    { 242,  2,  93},
    { 278,  2,  94},
    { 277,  2,  94},
    { 834,  2,  95},
    { 833,  2,  95},
    { 352,  2,  96},
    { 296,  2,  96},
    { 889,  2,  97},
    { 887,  2,  97},
    { 890,  2,  98},
    { 888,  2,  98},
    { 891,  2,  99},
    {1077,  2,  99},
    {1157,  2, 100},
    {1162,  2, 100},
    {1180,  2, 101},
    {1179,  2, 101},
    {1245,  2, 102},
    {1244,  2, 102},
    { 310,  2, 103},
    { 169,  2, 103},
    { 240,  2, 104},
    { 239,  2, 104},
    { 502,  2, 105},
    { 501,  2, 105},
    { 514,  2, 106},
    { 513,  2, 106},
    { 591,  2, 107},
    { 590,  2, 107},
    {1045,  2, 108},
    { 648,  2, 108},
    { 734,  2, 109},
    { 733,  2, 109},
    { 785,  2, 110},
    { 783,  2, 110},
    { 817,  2, 111},
    { 818,  2, 111},
    { 837,  2, 112},
    { 836,  2, 112},
    { 886,  2, 113},
    { 885,  2, 113},
    { 911,  2, 114},
    { 921,  2, 115},
    { 922,  2, 115},
    {1005,  2, 116},
    { 940,  2, 116},
    { 937,  2, 117},
    { 943,  2, 117},
    {1047,  2, 118},
    {1046,  2, 118},
    {1054,  2, 119},
    {1053,  2, 119},
    {1006,  2, 120},
    {1061,  2, 120},
    {1096,  2, 121},
    {1068,  2, 121},
    {1164,  2, 122},
    {1163,  2, 122},
    { 735,  2, 123},
    {1166,  2, 123},
    {1271,  2, 124},
    {1270,  2, 124},
    {1324,  2, 125},
    {1323,  2, 125},
    { 171,  2, 126},
    { 188,  2, 127},
    { 647,  2, 127},
    { 631,  2, 128},
    { 630,  2, 128},
    { 925,  2, 129},
    { 924,  2, 129},
    {1003,  2, 130},
    { 651,  2, 130},
    {1165,  2, 131},
    {1156,  2, 131},
    { 163,  2, 132},
    { 164,  2, 132},
    { 252,  2, 133},
    { 253,  2, 133},
    { 866,  2, 134},
    { 865,  2, 134},
    { 947,  2, 135},
    {1021,  2, 136},
    {1022,  2, 136},
    {1251,  2, 137},
    {1250,  2, 137},
    { 868,  2, 138},
    { 587,  2, 138},
    { 990,  2, 139},
    { 978,  2, 139},
    {1182,  2, 140},
    {1181,  2, 140},
    {1359,  2, 141},
    {1360,  2, 141},
    { 724,  2, 142},
    { 649,  2, 142},
    {1025,  3,   0},
    {1342,  3,   0},
    { 496,  3,   1},
    { 497,  3,   1},
    {1167,  3,   2},
    {1189,  3,   2},
    { 635,  3,   3},
    { 646,  3,   3},
    { 439,  3,   4},
    { 780,  3,   5},
    { 948,  3,   6},
    { 962,  3,   6},
    { 545,  3,   7},
    {1111,  3,   8},
    {1116,  3,   8},
    { 560,  3,   9},
    { 558,  3,   9},
    { 722,  3,  10},
    { 707,  3,  10},
    { 179,  3,  11},
    { 766,  3,  11},
    { 892,  3,  12},
    { 908,  3,  12},
    { 893,  3,  13},
    { 910,  3,  13},
    { 894,  3,  14},
    { 876,  3,  14},
    { 986,  3,  15},
    { 981,  3,  15},
    { 547,  3,  16},
    { 542,  3,  16},
    { 508,  3,  17},
    { 507,  3,  17},
    { 516,  3,  18},
    { 515,  3,  18},
    {1367,  3,  19},
    { 586,  3,  20},
    { 567,  3,  20},
    { 509,  3,  21},
    { 510,  3,  21},
    {1025,  4,   0},
    {1342,  4,   0},
    {1089,  4,   1},
    {1086,  4,   1},
    { 439,  4,   2},
    { 780,  4,   3},
    { 430,  4,   4},
    { 398,  4,   4},
    { 545,  4,   5},
    { 542,  4,   5},
    {1111,  4,   6},
    {1116,  4,   6},
    {1186,  4,   7},
    {1172,  4,   7},
    { 740,  4,   8},
    {1301,  4,   9},
    {1233,  4,  10},
    { 807,  4,  11},
    { 809,  4,  12},
    { 508,  4,  13},
    { 507,  4,  13},
    { 516,  4,  14},
    { 515,  4,  14},
    {1367,  4,  15},
    { 586,  4,  16},
    { 567,  4,  16},
    { 509,  4,  17},
    { 510,  4,  17},
    {1025,  5,   0},
    {1342,  5,   0},
    { 439,  5,   1},
    { 780,  5,   2},
    { 545,  5,   3},
    { 542,  5,   3},
    {1153,  5,   4},
    {1147,  5,   4},
    { 560,  5,   5},
    { 558,  5,   5},
    {1183,  5,   6},
    { 798,  5,   7},
    { 795,  5,   7},
    {1298,  5,   8},
    {1297,  5,   8},
    {1010,  5,   9},
    { 766,  5,   9},
    { 986,  5,  10},
    { 981,  5,  10},
    { 221,  5,  11},
    { 216,  5,  11},
    {1193,  5,  12},
    {1192,  5,  12},
    { 394,  5,  13},
    { 393,  5,  13},
    {1144,  5,  14},
    {1143,  5,  14},
    { 963,  6,   0},
    { 934,  6,   0},
    { 548,  6,   0},
    { 549,  6,   0},
    {1347,  6,   1},
    {1343,  6,   1},
    {1233,  6,   1},
    {1285,  6,   1},
    { 974,  7,   0},
    { 936,  7,   0},
    { 767,  7,   1},
    { 740,  7,   1},
    {1318,  7,   2},
    {1301,  7,   2},
    {1281,  7,   3},
    {1233,  7,   3},
    { 808,  7,   4},
    { 807,  7,   4},
    { 810,  7,   5},
    { 809,  7,   5},
    { 771,  8,   0},
    { 740,  8,   0},
    {1119,  8,   1},
    {1109,  8,   1},
    { 539,  8,   2},
    { 518,  8,   2},
    { 540,  8,   3},
    { 529,  8,   3},
    { 541,  8,   4},
    { 530,  8,   4},
    { 202,  8,   5},
    { 187,  8,   5},
    { 411,  8,   6},
    { 440,  8,   6},
    {1048,  8,   7},
    { 229,  8,   7},
    {1149,  8,   8},
    {1132,  8,   8},
    {1327,  8,   9},
    {1333,  8,   9},
    {1034,  8,  10},
    {1013,  8,  10},
    { 274,  8,  11},
    { 266,  8,  11},
    { 971,  8,  12},
    { 979,  8,  12},
    { 199,  8,  13},
    { 174,  8,  13},
    { 774,  8,  14},
    { 804,  8,  14},
    {1122,  8,  15},
    {1126,  8,  15},
    { 772,  8,  16},
    { 802,  8,  16},
    {1120,  8,  17},
    {1124,  8,  17},
    {1079,  8,  18},
    {1058,  8,  18},
    { 773,  8,  19},
    { 803,  8,  19},
    {1121,  8,  20},
    {1125,  8,  20},
    { 557,  8,  21},
    { 563,  8,  21},
    {1080,  8,  22},
    {1059,  8,  22},
    { 975,  9,   0},
    {   1,  9,   0},
    { 976,  9,   0},
    {1041,  9,   1},
    {   2,  9,   1},
    {1040,  9,   1},
    { 982,  9,   2},
    { 135,  9,   2},
    { 959,  9,   2},
    { 716,  9,   3},
    { 144,  9,   3},
    { 739,  9,   3},
    {1312,  9,   4},
    { 151,  9,   4},
    {1319,  9,   4},
    { 314,  9,   5},
    {  17,  9,   5},
    { 317,  9,   6},
    {  28,  9,   6},
    { 319,  9,   7},
    {  32,  9,   7},
    { 322,  9,   8},
    {  35,  9,   8},
    { 326,  9,   9},
    {  40,  9,   9},
    { 327,  9,  10},
    {  41,  9,  10},
    { 328,  9,  11},
    {  43,  9,  11},
    { 329,  9,  12},
    {  44,  9,  12},
    { 330,  9,  13},
    {  46,  9,  13},
    { 331,  9,  14},
    {  47,  9,  14},
    { 332,  9,  15},
    {  51,  9,  15},
    { 333,  9,  16},
    {  57,  9,  16},
    { 334,  9,  17},
    {  62,  9,  17},
    { 335,  9,  18},
    {  68,  9,  18},
    { 336,  9,  19},
    {  73,  9,  19},
    { 337,  9,  20},
    {  75,  9,  20},
    { 338,  9,  21},
    {  76,  9,  21},
    { 339,  9,  22},
    {  77,  9,  22},
    { 340,  9,  23},
    {  78,  9,  23},
    { 341,  9,  24},
    {  79,  9,  24},
    { 342,  9,  25},
    {  88,  9,  25},
    { 343,  9,  26},
    {  93,  9,  26},
    { 344,  9,  27},
    {  94,  9,  27},
    { 345,  9,  28},
    {  95,  9,  28},
    { 346,  9,  29},
    {  96,  9,  29},
    { 347,  9,  30},
    {  97,  9,  30},
    { 348,  9,  31},
    {  98,  9,  31},
    { 349,  9,  32},
    { 150,  9,  32},
    { 350,  9,  33},
    { 158,  9,  33},
    { 315,  9,  34},
    {  26,  9,  34},
    { 316,  9,  35},
    {  27,  9,  35},
    { 318,  9,  36},
    {  31,  9,  36},
    { 320,  9,  37},
    {  33,  9,  37},
    { 321,  9,  38},
    {  34,  9,  38},
    { 323,  9,  39},
    {  37,  9,  39},
    { 324,  9,  40},
    {  38,  9,  40},
    { 224,  9,  41},
    {  56,  9,  41},
    { 219,  9,  41},
    { 222,  9,  42},
    {  58,  9,  42},
    { 217,  9,  42},
    { 223,  9,  43},
    {  59,  9,  43},
    { 218,  9,  43},
    { 247,  9,  44},
    {  61,  9,  44},
    { 261,  9,  44},
    { 246,  9,  45},
    {  63,  9,  45},
    { 229,  9,  45},
    { 248,  9,  46},
    {  64,  9,  46},
    { 276,  9,  46},
    { 768,  9,  47},
    {  65,  9,  47},
    { 740,  9,  47},
    {1117,  9,  48},
    {  66,  9,  48},
    {1109,  9,  48},
    { 161,  9,  49},
    {  67,  9,  49},
    { 174,  9,  49},
    { 160,  9,  50},
    {  69,  9,  50},
    { 159,  9,  50},
    { 162,  9,  51},
    {  70,  9,  51},
    { 194,  9,  51},
    { 495,  9,  52},
    {  71,  9,  52},
    { 470,  9,  52},
    { 494,  9,  53},
    {  72,  9,  53},
    { 465,  9,  53},
    { 685,  9,  54},
    {  74,  9,  54},
    { 688,  9,  54},
    { 325,  9,  55},
    {  39,  9,  55},
    { 225,  9,  56},
    {  52,  9,  56},
    { 220,  9,  56},
    { 968, 10,   0},
    { 300, 10,   1},
    { 297, 10,   1},
    { 412, 10,   2},
    { 401, 10,   2},
    { 559, 10,   3},
    { 965, 10,   4},
    { 942, 10,   4},
    { 676, 10,   5},
    { 675, 10,   5},
    { 880, 10,   6},
    { 879, 10,   6},
    { 554, 10,   7},
    { 553, 10,   7},
    { 690, 10,   8},
    { 689, 10,   8},
    { 364, 10,   9},
    { 519, 10,   9},
    {1204, 10,  10},
    {1200, 10,  10},
    {1195, 10,  11},
    {1310, 10,  12},
    {1309, 10,  12},
    {1328, 10,  13},
    { 941, 10,  14},
    { 939, 10,  14},
    {1173, 10,  15},
    {1176, 10,  15},
    {1191, 10,  16},
    {1190, 10,  16},
    { 562, 10,  17},
    { 561, 10,  17},
    { 946, 11,   0},
    { 934, 11,   0},
    { 186, 11,   1},
    { 159, 11,   1},
    { 615, 11,   2},
    { 609, 11,   2},
    {1328, 11,   3},
    {1322, 11,   3},
    { 564, 11,   4},
    { 548, 11,   4},
    { 941, 11,   5},
    { 936, 11,   5},
    { 966, 12,   0},
    { 173, 12,   1},
    { 175, 12,   2},
    { 178, 12,   3},
    { 245, 12,   4},
    { 251, 12,   5},
    { 466, 12,   6},
    { 467, 12,   7},
    { 503, 12,   8},
    { 552, 12,   9},
    { 556, 12,  10},
    { 565, 12,  11},
    { 566, 12,  12},
    { 612, 12,  13},
    { 617, 12,  14},
    {1256, 12,  14},
    { 632, 12,  15},
    { 636, 12,  16},
    { 637, 12,  17},
    { 638, 12,  18},
    { 708, 12,  19},
    { 721, 12,  20},
    { 737, 12,  21},
    { 742, 12,  22},
    { 743, 12,  23},
    { 881, 12,  24},
    { 895, 12,  25},
    { 973, 12,  26},
    { 989, 12,  27},
    {1060, 12,  28},
    {1103, 12,  29},
    {1104, 12,  30},
    {1113, 12,  31},
    {1115, 12,  32},
    {1135, 12,  33},
    {1136, 12,  34},
    {1148, 12,  35},
    {1150, 12,  36},
    {1160, 12,  37},
    {1222, 12,  38},
    {1238, 12,  39},
    {1254, 12,  40},
    {1255, 12,  41},
    {1261, 12,  42},
    {1325, 12,  43},
    {1232, 12,  44},
    {1344, 12,  45},
    {1345, 12,  46},
    {1346, 12,  47},
    {1354, 12,  48},
    {1355, 12,  49},
    {1358, 12,  50},
    {1361, 12,  51},
    { 727, 12,  52},
    { 551, 12,  53},
    { 291, 12,  54},
    { 550, 12,  55},
    { 992, 12,  56},
    {1127, 12,  57},
    {1194, 12,  58},
    { 838, 12,  59},
    { 839, 12,  60},
    { 840, 12,  61},
    { 841, 12,  62},
    { 842, 12,  63},
    { 843, 12,  64},
    { 844, 12,  65},
    { 845, 12,  66},
    { 846, 12,  67},
    { 847, 12,  68},
    { 848, 12,  69},
    { 849, 12,  70},
    { 850, 12,  71},
    { 851, 12,  72},
    { 852, 12,  73},
    { 853, 12,  74},
    { 854, 12,  75},
    { 855, 12,  76},
    { 856, 12,  77},
    { 857, 12,  78},
    { 858, 12,  79},
    { 859, 12,  80},
    { 860, 12,  81},
    { 861, 12,  82},
    { 862, 12,  83},
    { 863, 12,  84},
    { 864, 12,  85},
    { 166, 12,  86},
    { 168, 12,  87},
    { 167, 12,  88},
    { 826, 12,  89},
    { 823, 12,  90},
    { 829, 12,  91},
    { 832, 12,  92},
    { 827, 12,  93},
    { 828, 12,  94},
    { 822, 12,  95},
    { 830, 12,  96},
    { 824, 12,  97},
    { 825, 12,  98},
    { 831, 12,  99},
    { 970, 13,   0},
    {1286, 13,   0},
    { 700, 13,   1},
    { 294, 13,   1},
    { 500, 13,   2},
    { 464, 13,   2},
    {1118, 13,   3},
    {1109, 13,   3},
    { 770, 13,   4},
    { 740, 13,   4},
    {1282, 13,   5},
    {1233, 13,   5},
    {1296, 14,   0},
    {1342, 14,   0},
    {1015, 14,   1},
    {1014, 14,   1},
    { 396, 14,   2},
    { 393, 14,   2},
    {1107, 14,   3},
    {1106, 14,   3},
    { 585, 14,   4},
    { 580, 14,   4},
    { 972, 14,   5},
    { 977, 14,   5},
    { 543, 14,   6},
    { 542, 14,   6},
    { 286, 14,   7},
    {1223, 14,   7},
    { 673, 14,   8},
    { 688, 14,   8},
    {1088, 14,   9},
    {1087, 14,   9},
    {1085, 14,  10},
    {1078, 14,  10},
    { 986, 14,  11},
    { 981, 14,  11},
    { 182, 14,  12},
    { 174, 14,  12},
    { 658, 14,  13},
    { 654, 14,  13},
    { 682, 14,  14},
    { 669, 14,  14},
    { 683, 14,  14},
    { 653, 14,  15},
    { 652, 14,  15},
    { 407, 14,  16},
    { 397, 14,  16},
    { 284, 14,  17},
    { 243, 14,  17},
    { 283, 14,  18},
    { 231, 14,  18},
    {1184, 14,  19},
    {1183, 14,  19},
    { 835, 14,  20},
    { 260, 14,  20},
    { 306, 14,  21},
    { 439, 14,  21},
    { 790, 14,  22},
    { 780, 14,  22},
    { 429, 14,  23},
    { 311, 14,  23},
    { 414, 14,  24},
    {1134, 14,  24},
    { 186, 14,  25},
    { 172, 14,  25},
    { 285, 14,  26},
    { 230, 14,  26},
    {1220, 14,  27},
    {1155, 14,  27},
    {1368, 14,  28},
    {1366, 14,  28},
    { 950, 14,  29},
    { 962, 14,  29},
    {1332, 14,  30},
    {1329, 14,  30},
    { 698, 14,  31},
    { 706, 14,  32},
    { 705, 14,  33},
    { 610, 14,  34},
    { 611, 14,  35},
    { 395, 14,  36},
    { 437, 14,  36},
    { 635, 14,  37},
    { 646, 14,  37},
    { 415, 14,  38},
    { 365, 14,  38},
    {1111, 14,  39},
    {1116, 14,  39},
    { 508, 14,  40},
    { 507, 14,  40},
    { 516, 14,  41},
    { 515, 14,  41},
    {1367, 14,  42},
    { 968, 15,   0},
    { 986, 15,   1},
    { 981, 15,   1},
    { 490, 15,   2},
    { 483, 15,   2},
    { 472, 15,   3},
    { 471, 15,   3},
    { 938, 16,   0},
    {   0, 16,   1},
    {   1, 16,   2},
    {   6, 16,   3},
    {  11, 16,   4},
    {  87, 16,   5},
    {   8, 16,   6},
    {   5, 16,   7},
    {   4, 16,   8},
    {   3, 16,   9},
    {  16, 16,  10},
    {  15, 16,  11},
    {  14, 16,  12},
    {  83, 16,  13},
    {  13, 16,  14},
    {  81, 16,  15},
    {  12, 16,  16},
    {  10, 16,  17},
    {   9, 16,  18},
    {  86, 16,  19},
    {  50, 16,  20},
    { 120, 16,  21},
    {   7, 16,  22},
    { 136, 16,  23},
    {  85, 16,  24},
    { 123, 16,  25},
    {  49, 16,  26},
    {  84, 16,  27},
    { 103, 16,  28},
    { 122, 16,  29},
    { 138, 16,  30},
    {  29, 16,  31},
    {   2, 16,  32},
    {  82, 16,  33},
    {  48, 16,  34},
    { 121, 16,  35},
    {  80, 16,  36},
    { 137, 16,  37},
    { 102, 16,  38},
    { 152, 16,  39},
    { 119, 16,  40},
    {  30, 16,  41},
    { 129, 16,  42},
    {  36, 16,  43},
    { 135, 16,  44},
    {  42, 16,  45},
    { 144, 16,  46},
    {  45, 16,  47},
    { 151, 16,  48},
    {  17, 16,  49},
    {  28, 16,  50},
    {  32, 16,  51},
    {  35, 16,  52},
    {  40, 16,  53},
    {  41, 16,  54},
    {  43, 16,  55},
    {  44, 16,  56},
    {  46, 16,  57},
    {  47, 16,  58},
    {  51, 16,  59},
    {  57, 16,  60},
    {  62, 16,  61},
    {  68, 16,  62},
    {  73, 16,  63},
    {  75, 16,  64},
    {  76, 16,  65},
    {  77, 16,  66},
    {  78, 16,  67},
    {  79, 16,  68},
    {  88, 16,  69},
    {  93, 16,  70},
    {  94, 16,  71},
    {  95, 16,  72},
    {  96, 16,  73},
    {  97, 16,  74},
    {  98, 16,  75},
    {  99, 16,  76},
    { 100, 16,  77},
    { 101, 16,  78},
    { 104, 16,  79},
    { 109, 16,  80},
    { 110, 16,  81},
    { 111, 16,  82},
    { 113, 16,  83},
    { 114, 16,  84},
    { 115, 16,  85},
    { 116, 16,  86},
    { 117, 16,  87},
    { 118, 16,  88},
    { 124, 16,  89},
    { 130, 16,  90},
    { 139, 16,  91},
    { 145, 16,  92},
    { 153, 16,  93},
    {  18, 16,  94},
    {  52, 16,  95},
    {  89, 16,  96},
    { 105, 16,  97},
    { 125, 16,  98},
    { 131, 16,  99},
    { 140, 16, 100},
    { 146, 16, 101},
    { 154, 16, 102},
    {  19, 16, 103},
    {  53, 16, 104},
    {  90, 16, 105},
    { 106, 16, 106},
    { 126, 16, 107},
    { 132, 16, 108},
    { 141, 16, 109},
    { 147, 16, 110},
    { 155, 16, 111},
    {  20, 16, 112},
    {  54, 16, 113},
    {  91, 16, 114},
    { 107, 16, 115},
    { 127, 16, 116},
    { 133, 16, 117},
    { 142, 16, 118},
    { 148, 16, 119},
    { 156, 16, 120},
    {  21, 16, 121},
    {  55, 16, 122},
    {  60, 16, 123},
    {  92, 16, 124},
    { 108, 16, 125},
    { 112, 16, 126},
    { 128, 16, 127},
    { 134, 16, 128},
    { 143, 16, 129},
    { 149, 16, 130},
    { 157, 16, 131},
    {  22, 16, 132},
    {  23, 16, 133},
    {  24, 16, 134},
    {  25, 16, 135},
    { 936, 17,   0},
    {1117, 17,   1},
    { 768, 17,   2},
    {1314, 17,   3},
    { 769, 17,   4},
    {1275, 17,   5},
    { 271, 17,   6},
    {1276, 17,   7},
    {1280, 17,   8},
    {1278, 17,   9},
    {1279, 17,  10},
    { 273, 17,  11},
    { 272, 17,  12},
    {1277, 17,  13},
    {1042, 17,  14},
    {1025, 18,   0},
    { 259, 18,   1},
    {1313, 18,   2},
    { 226, 18,   3},
    { 982, 18,   4},
    {1312, 18,   5},
    {1100, 18,   6},
    { 684, 18,   7},
    {1317, 18,   8},
    {1316, 18,   9},
    {1315, 18,  10},
    { 423, 18,  11},
    { 417, 18,  12},
    { 418, 18,  13},
    { 428, 18,  14},
    { 425, 18,  15},
    { 424, 18,  16},
    { 427, 18,  17},
    { 426, 18,  18},
    { 422, 18,  19},
    { 419, 18,  20},
    { 420, 18,  21},
    { 916, 18,  22},
    {1273, 18,  23},
    {1274, 18,  24},
    { 570, 18,  25},
    { 303, 18,  26},
    {1112, 18,  27},
    {1224, 18,  28},
    { 421, 18,  29},
    { 969, 18,  30},
    { 702, 18,  31},
    { 985, 18,  32},
    { 983, 18,  33},
    { 279, 18,  34},
    {1347, 19,   0},
    {1343, 19,   0},
    { 963, 19,   1},
    { 934, 19,   1},
    {1347, 20,   0},
    {1343, 20,   0},
    { 963, 20,   1},
    { 934, 20,   1},
    { 875, 20,   2},
    { 815, 20,   2},
};

/* property values: 6036 bytes. */

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
    16, 17, 18, 19, 20, 21, 22, 21, 23, 21, 21, 21, 21, 24,  7,  7,
    25, 26, 21, 21, 21, 21, 27, 28, 21, 21, 29, 30, 31, 32, 33, 34,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 35,  7, 36, 37,  7, 38,  7,  7,  7, 39, 21, 40,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    41, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 42,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 42,
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
    119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 123,  34,  34, 130, 123,
    131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 123, 123, 141, 123, 123, 123,
    142, 143, 144, 145, 146, 147, 148, 123, 149, 150, 123, 151, 152, 153, 154, 123,
    123, 155, 123, 123, 156, 157, 123, 123, 158, 159, 160, 123, 123, 123, 123, 123,
     34,  34,  34,  34,  34,  34,  34, 161, 162,  34, 163, 123, 123, 123, 123, 123,
    123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
     34,  34,  34,  34,  34,  34,  34,  34, 164, 123, 123, 123, 123, 123, 123, 123,
    123, 123, 123, 123, 123, 123, 123, 123,  34,  34,  34,  34, 165, 123, 123, 123,
     34,  34,  34,  34, 166, 167, 168, 169, 123, 123, 123, 123, 123, 123, 170, 171,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 172,
     34,  34,  34,  34,  34, 173, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
     34,  34, 174,  34,  34, 175, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
    123, 123, 123, 123, 123, 123, 123, 123, 176, 177, 123, 123, 123, 123, 123, 123,
     69, 178, 179, 180, 181, 123, 182, 123, 183, 184, 185, 186, 187, 188, 189, 190,
     69,  69,  69,  69, 191, 192, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
    193, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
     34, 194, 195, 123, 123, 123, 123, 123, 123, 123, 123, 123, 196, 197, 123, 123,
    198, 199, 200, 201, 202, 123,  69, 203,  69,  69,  69,  69,  69, 204, 205, 206,
    207, 208, 209, 210, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 211,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 212,  34,
    213,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34, 214,  34,  34,
     34,  34,  34,  34,  34,  34,  34, 215, 123, 123, 123, 123, 123, 123, 123, 123,
     34,  34,  34,  34, 216, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
    217, 123, 218, 219, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
    108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 220,
};

static RE_UINT16 re_general_category_stage_3[] = {
      0,   0,   1,   2,   3,   4,   5,   6,   0,   0,   7,   8,   9,  10,  11,  12,
     13,  13,  13,  14,  15,  13,  13,  16,  17,  18,  19,  20,  21,  22,  13,  23,
     13,  13,  13,  24,  25,  11,  11,  11,  11,  26,  11,  27,  28,  29,  30,  31,
     32,  32,  32,  32,  32,  32,  32,  33,  34,  35,  36,  11,  37,  38,  13,  39,
      9,   9,   9,  11,  11,  11,  13,  13,  40,  13,  13,  13,  41,  13,  13,  13,
     13,  13,  13,  42,   9,  43,  44,  11,  45,  46,  32,  47,  48,  49,  50,  51,
     52,  53,  49,  49,  54,  32,  55,  56,  49,  49,  49,  49,  49,  57,  58,  59,
     60,  61,  49,  32,  62,  49,  49,  49,  49,  49,  63,  64,  65,  49,  66,  67,
     49,  68,  69,  70,  49,  71,  50,  72,  72,  72,  49,  73,  72,  74,  75,  32,
     76,  49,  49,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
     90,  83,  84,  91,  92,  93,  94,  95,  96,  97,  84,  98,  99, 100,  88, 101,
    102,  83,  84, 103, 104, 105,  88, 106, 107, 108, 109, 110, 111, 112,  94, 113,
    114, 115,  84, 116, 117, 118,  88, 119, 120, 115,  84, 121, 122, 123,  88, 124,
    125, 115,  49, 126, 127, 128,  88, 129, 130, 131,  49, 132, 133, 134,  94, 135,
    136,  49,  49, 137, 138, 139,  72,  72, 140, 141, 142, 143, 144, 145,  72,  72,
    146, 147, 148, 149, 150,  49, 151, 152, 153, 154,  32, 155, 156, 157,  72,  72,
     49,  49, 158, 159, 160, 161, 162, 163, 164, 165,   9,   9, 166,  49,  49, 167,
     49,  49,  49,  49,  49,  49,  49,  49,  49,  49,  49,  49, 168, 169,  49,  49,
    168,  49,  49, 170, 171, 172,  49,  49,  49, 171,  49,  49,  49, 173, 174, 175,
     49, 176,   9,   9,   9,   9,   9, 177, 178,  49,  49,  49,  49,  49,  49,  49,
     49,  49,  49,  49,  49,  49, 179,  49, 180, 181,  49,  49,  49,  49, 182, 183,
    184, 185,  49, 186,  49, 187, 184, 188,  49,  49,  49, 189, 190, 191, 192, 193,
    194, 192,  49,  49, 195,  49,  49, 196, 197,  49, 198,  49,  49,  49,  49, 199,
     49, 200, 201, 202, 203,  49, 204, 205,  49,  49, 206,  49, 207, 208, 209, 209,
     49, 210,  49,  49,  49, 211, 212, 213, 192, 192, 214, 215,  72,  72,  72,  72,
    216,  49,  49, 217, 218, 160, 219, 220, 221,  49, 222,  65,  49,  49, 223, 224,
     49,  49, 225, 226, 227,  65,  49, 228, 229,  72,  72,  72, 230, 231, 232, 233,
     11,  11, 234,  27,  27,  27, 235, 236,  11, 237,  27,  27,  32,  32,  32, 238,
     13,  13,  13,  13,  13,  13,  13,  13,  13, 239,  13,  13,  13,  13,  13,  13,
    240, 241, 240, 240, 241, 242, 240, 243, 244, 244, 244, 245, 246, 247, 248, 249,
    250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 260,  72, 261, 262, 263,
    264, 265, 266, 267, 268, 269, 270, 270, 271, 272, 273, 209, 274, 275, 209, 276,
    277, 277, 277, 277, 277, 277, 277, 277, 278, 209, 279, 209, 209, 209, 209, 280,
    209, 281, 277, 282, 209, 283, 284, 209, 209, 209, 285,  72, 286,  72, 269, 269,
    269, 287, 209, 209, 209, 209, 288, 269, 209, 209, 209, 209, 209, 209, 209, 209,
    209, 209, 209, 289, 290, 209, 209, 291, 209, 209, 209, 209, 209, 209, 292, 209,
    209, 209, 209, 209, 209, 209, 293, 294, 269, 295, 209, 209, 296, 277, 297, 277,
    298, 299, 277, 277, 277, 300, 277, 301, 209, 209, 209, 277, 302, 209, 209, 303,
    209, 304, 209, 305, 306, 307, 308,  72,   9,   9, 309,  11,  11, 310, 311, 312,
     13,  13,  13,  13,  13,  13, 313, 314,  11,  11, 315,  49,  49,  49, 316, 317,
     49, 318, 319, 319, 319, 319,  32,  32, 320, 321, 322, 323, 324,  72,  72,  72,
    209, 325, 209, 209, 209, 209, 209, 326, 209, 209, 209, 209, 209, 327,  72, 328,
    329, 330, 331, 332, 136,  49,  49,  49,  49, 333, 178,  49,  49,  49,  49, 334,
    335,  49, 200, 136,  49,  49,  49,  49, 200, 336,  49,  50, 209, 209, 326,  49,
    209, 337, 338, 209, 339, 340, 209, 209, 338, 209, 209, 340, 209, 209, 209, 337,
     49,  49,  49, 199, 209, 209, 209, 209,  49,  49,  49,  49,  49,  49,  50,  72,
     49, 341,  49,  49,  49,  49,  49,  49, 151, 209, 209, 209, 285,  49,  49, 228,
    342,  49, 343,  72,  13,  13, 344, 345,  13, 346,  49,  49,  49,  49, 347, 348,
     31, 349, 350, 351,  13,  13,  13, 352, 353, 354, 355, 356,  72,  72,  72, 357,
    358,  49, 359, 360,  49,  49,  49, 361, 362,  49,  49, 363, 364, 192,  32, 365,
     65,  49, 366,  49, 367, 368,  49, 151,  76,  49,  49, 369, 370, 371, 372, 373,
     49,  49, 374, 375, 376, 377,  49, 378,  49,  49,  49, 379, 380, 381, 382, 383,
    384, 385, 319,  11,  11, 386, 387,  11,  11,  11,  11,  11,  49,  49, 388, 192,
     49,  49, 389,  49, 390,  49,  49, 206, 391, 391, 391, 391, 391, 391, 391, 391,
    392, 392, 392, 392, 392, 392, 392, 392,  49,  49,  49,  49,  49,  49, 204,  49,
     49,  49,  49,  49,  49, 207,  72,  72, 393, 394, 395, 396, 397,  49,  49,  49,
     49,  49,  49, 398, 399, 400,  49,  49,  49,  49,  49, 401,  72,  49,  49,  49,
     49, 402,  49,  49, 196,  72,  72, 403,  32, 404,  32, 405, 406, 407, 408, 409,
     49,  49,  49,  49,  49,  49,  49, 410, 411,   2,   3,   4,   5, 412, 413, 414,
     49, 415,  49, 200, 416, 417, 418, 419, 420,  49, 172, 421, 204, 204,  72,  72,
     49,  49,  49,  49,  49,  49,  49,  50, 422, 269, 269, 423, 270, 270, 270, 424,
    425, 328, 426,  72,  72, 209, 209, 427,  72,  72,  72,  72,  72,  72,  72,  72,
     49, 151,  49,  49,  49, 100, 428, 429,  49,  49, 430,  49, 431,  49,  49, 432,
     49, 433,  49,  49, 434, 435,  72,  72,   9,   9, 436,  11,  11,  49,  49,  49,
     49, 204, 192,   9,   9, 437,  11, 438,  49,  49, 196,  49,  49,  49, 439,  72,
     49,  49,  49, 318,  49, 199, 196,  72, 440,  49,  49, 441,  49, 442,  49, 443,
     49, 200, 444,  72,  72,  72,  49, 445,  49, 446,  49, 447,  72,  72,  72,  72,
     49,  49,  49, 448, 269, 449, 269, 269, 450, 451,  49, 452, 453, 454,  49, 455,
     49, 456,  72,  72, 457,  49, 458, 459,  49,  49,  49, 460,  49, 461,  49, 462,
     49, 463, 464,  72,  72,  72,  72,  72,  49,  49,  49,  49, 465,  72,  72,  72,
      9,   9,   9, 466,  11,  11,  11, 467,  72,  72,  72,  72,  72,  72, 269, 468,
    469,  49,  49, 470, 471, 449, 472, 473, 221,  49,  49, 474, 475,  49, 465, 192,
    476,  49, 477, 478, 479,  49,  49, 480, 221,  49,  49, 481, 482, 483, 484, 485,
     49,  97, 486, 487,  72,  72,  72,  72, 488, 489, 490,  49,  49, 491, 492, 192,
    493,  83,  84,  98, 494, 495, 496, 497,  49,  49,  49, 498, 499, 500,  72,  72,
     49,  49,  49, 501, 502, 192,  72,  72,  49,  49, 503, 504, 505, 506,  72,  72,
     49,  49,  49, 507, 508, 192, 509,  72,  49,  49, 510, 511, 192,  72,  72,  72,
     49, 512, 513, 514,  72,  72,  72,  72,  72,  72,   9,   9,  11,  11, 148, 515,
    516,  49,  49, 517, 518, 519,  49,  49, 520, 521, 522,  72,  49,  49,  49, 465,
     84,  49, 503, 523, 524, 148, 175, 525,  49, 526, 527, 528,  72,  72,  72,  72,
    529,  49,  49, 530, 531, 192,  72,  72,  49, 207,  72,  72,  72,  72,  72,  72,
    270, 270, 270, 270, 270, 270, 532, 533,  49,  49,  49,  49, 389,  72,  72,  72,
     49,  49, 200,  72,  72,  72,  72,  72,  49,  49,  49,  49, 318,  72,  72,  72,
     49,  49,  49, 465,  49, 200, 371,  72,  72,  72,  72,  72,  72,  49, 204, 534,
     49,  49,  49, 535, 536, 537, 538, 539,  49,  72,  72,  72,  72,  72,  72,  72,
     49,  49,  49,  49, 205, 540, 541, 542, 473, 543,  72,  72,  72,  72, 544,  72,
     49,  49,  49,  49,  49,  49, 151,  72,  49,  49,  49,  49,  49,  49,  49, 545,
     49, 200,  72,  72,  72,  72,  72,  49,  49,  49,  49,  49,  49,  49,  49, 206,
     49,  49,  49,  49,  49,  49,  50, 151, 465, 546, 547,  72,  72,  72,  72,  72,
    209, 209, 209, 209, 209, 209, 209, 327, 209, 209, 548, 209, 209, 209, 549, 550,
    551, 209, 552, 209, 209, 209, 553,  72, 209, 209, 209, 209, 554,  72,  72,  72,
    209, 209, 209, 209, 209, 285, 269, 555,   9, 556,  11, 557, 558, 559, 240,   9,
    560, 561, 562, 563, 564,   9, 556,  11, 565, 566,  11, 567, 568, 569, 570,   9,
    571,  11,   9, 556,  11, 557, 558,  11, 240,   9, 560, 570,   9, 571,  11,   9,
    556,  11, 572,   9, 573, 574, 575, 576,  11, 577,   9, 578, 579, 580, 581,  11,
    582,   9, 583,  11, 584, 585, 585, 585,  32,  32,  32, 586,  32,  32, 587, 588,
    589, 590,  46,  72,  72,  72,  72,  72, 591, 592, 593,  72,  72,  72,  72,  72,
     49,  49,  49,  49, 594, 595,  72,  72,   9,   9, 560,  11, 596, 371,  72,  72,
    597,  49, 598, 599, 600, 601, 602, 603, 604, 206, 605, 206,  72,  72,  72, 606,
    209, 209, 328, 209, 209, 209, 209, 209, 209, 326, 337, 607, 607, 607, 209, 327,
    175, 209, 337, 209, 209, 209, 328, 209, 209, 209, 608,  72,  72,  72, 609, 209,
    307, 209, 209, 328, 553, 610, 327,  72, 209, 209, 209, 209, 209, 209, 209, 611,
    209, 209, 209, 209, 209, 612, 608, 553, 209, 209, 209, 209, 209, 209, 209, 326,
    209, 209, 209, 209, 209, 612,  72,  72, 328, 209, 209, 209, 613, 176, 209, 209,
    613, 209, 614,  72,  72,  72,  72,  72, 328, 209, 209, 337, 608, 209, 328,  72,
    209, 613,  72,  72, 426, 209, 285,  72,  49,  49,  49,  49,  49, 318,  72,  72,
     49,  49,  49, 205,  49,  49,  49,  49,  49, 204,  49,  49,  49,  49,  49,  49,
     49,  49, 615,  49,  49,  49,  49,  49,  49,  49,  49,  49,  49,  49, 100,  72,
     49, 204,  72,  72,  72,  72,  72,  72, 616,  72, 617, 617, 617, 617, 617, 617,
     32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  72,
    392, 392, 392, 392, 392, 392, 392, 618,
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
     43,  43,  43,  43,  43,  43,  43,  43,  32,  32,  42,  32,  44,  45,  16,  10,
     44,  44,  41,  46,  11,  47,  47,  11,  34,  11,  11,  11,  11,  11,  11,  11,
     11,  48,  11,  11,  11,  11,  16,  16,  16,  16,  16,  16,  16,  16,  16,  34,
     16,  11,  32,  16,  32,  32,  32,  32,  16,  16,  32,  49,  34,  32,  34,  11,
     32,  50,  43,  43,  51,  32,  32,  32,  11,  34,  34,  34,  34,  34,  34,  16,
     48,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  47,  52,   2,   2,   2,
     53,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  54,  55,  56,  57,
     58,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  59,
     60,  61,  43,  60,  44,  44,  44,  44,  36,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  62,  44,  44,  36,  63,  64,  44,  44,  44,  44,  44,
     65,  65,  65,   8,   9,  66,   2,  67,  43,  43,  43,  43,  43,  61,  68,   2,
     69,  36,  36,  36,  36,  70,  43,  43,   7,   7,   7,   7,   7,   2,   2,  36,
     71,  36,  36,  36,  36,  36,  36,  36,  36,  36,  72,  43,  43,  43,  73,  50,
     43,  43,  74,  75,  76,  43,  43,  36,   7,   7,   7,   7,   7,  36,  77,  78,
      2,   2,   2,   2,   2,   2,   2,  79,  70,  36,  36,  36,  36,  36,  36,  36,
     43,  43,  43,  43,  43,  80,  81,  36,  36,  36,  36,  43,  43,  43,  43,  43,
     71,  44,  44,  44,  44,  44,  44,  44,   7,   7,   7,   7,   7,  36,  36,  36,
     36,  36,  36,  36,  36,  70,  43,  43,  43,  43,  40,  21,   2,  82,  44,  44,
     36,  36,  36,  43,  43,  75,  43,  43,  43,  43,  75,  43,  75,  43,  43,  44,
      2,   2,   2,   2,   2,   2,   2,  64,  36,  36,  36,  36,  70,  43,  44,  64,
     44,  44,  44,  44,  44,  44,  44,  44,  36,  36,  62,  36,  36,  36,  36,  44,
     44,  44,  43,  43,  43,  43,  43,  43,  43,  83,  43,  43,  43,  43,  43,  43,
     43,  84,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  84,  71,  85,
     86,  43,  43,  43,  84,  85,  86,  85,  70,  43,  43,  43,  36,  36,  36,  36,
     36,  43,   2,   7,   7,   7,   7,   7,  87,  36,  36,  36,  36,  36,  36,  36,
     70,  85,  81,  36,  36,  36,  62,  81,  62,  81,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  62,  36,  36,  36,  62,  62,  44,  36,  36,  44,  71,  85,
     86,  43,  80,  88,  89,  88,  86,  62,  44,  44,  44,  88,  44,  44,  36,  81,
     36,  43,  44,   7,   7,   7,   7,   7,  36,  20,  27,  27,  27,  57,  63,  44,
     58,  84,  81,  36,  36,  62,  44,  81,  62,  36,  81,  62,  36,  44,  80,  85,
     86,  80,  44,  58,  80,  58,  43,  44,  58,  44,  44,  44,  81,  36,  62,  62,
     44,  44,  44,   7,   7,   7,   7,   7,  43,  36,  70,  44,  44,  44,  44,  44,
     58,  84,  81,  36,  36,  36,  36,  81,  36,  81,  36,  36,  36,  36,  36,  36,
     62,  36,  81,  36,  36,  44,  71,  85,  86,  43,  43,  58,  84,  88,  86,  44,
     62,  44,  44,  44,  44,  44,  44,  44,  66,  44,  44,  44,  81,  43,  43,  43,
     58,  85,  81,  36,  36,  36,  62,  81,  62,  36,  81,  36,  36,  44,  71,  86,
     86,  43,  80,  88,  89,  88,  86,  44,  44,  44,  44,  84,  44,  44,  36,  81,
     78,  27,  27,  27,  44,  44,  44,  44,  44,  71,  81,  36,  36,  62,  44,  36,
     62,  36,  36,  44,  81,  62,  62,  36,  44,  81,  62,  44,  36,  62,  44,  36,
     36,  36,  36,  36,  36,  44,  44,  85,  84,  89,  44,  85,  89,  85,  86,  44,
     62,  44,  44,  88,  44,  44,  44,  44,  27,  90,  67,  67,  57,  91,  44,  44,
     84,  85,  81,  36,  36,  36,  62,  36,  62,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  44,  81,  43,  84,  85,  89,  43,  80,  43,  43,  44,
     44,  44,  58,  80,  36,  62,  44,  44,  44,  44,  44,  44,  27,  27,  27,  90,
     70,  85,  81,  36,  36,  36,  62,  36,  36,  36,  81,  36,  36,  44,  71,  86,
     85,  85,  89,  84,  89,  85,  43,  44,  44,  44,  88,  89,  44,  44,  44,  62,
     81,  62,  44,  44,  44,  44,  44,  44,  43,  85,  81,  36,  36,  36,  62,  36,
     36,  36,  36,  36,  36,  70,  71,  85,  86,  43,  80,  85,  89,  85,  86,  77,
     44,  44,  36,  92,  27,  27,  27,  93,  27,  27,  27,  27,  90,  36,  36,  36,
     44,  85,  81,  36,  36,  36,  36,  36,  36,  36,  36,  62,  44,  36,  36,  36,
     36,  81,  36,  36,  36,  36,  81,  44,  36,  36,  36,  62,  44,  80,  44,  88,
     85,  43,  80,  80,  85,  85,  85,  85,  44,  85,  64,  44,  44,  44,  44,  44,
     81,  36,  36,  36,  36,  36,  36,  36,  70,  36,  43,  43,  43,  80,  44,  94,
     36,  36,  36,  75,  43,  43,  43,  61,   7,   7,   7,   7,   7,   2,  44,  44,
     81,  62,  62,  81,  62,  62,  81,  44,  44,  44,  36,  36,  81,  36,  36,  36,
     81,  36,  81,  81,  44,  36,  81,  36,  70,  36,  43,  43,  43,  58,  71,  44,
     36,  36,  62,  82,  43,  43,  43,  44,   7,   7,   7,   7,   7,  44,  36,  36,
     77,  67,   2,   2,   2,   2,   2,   2,   2,  95,  95,  67,  43,  67,  67,  67,
      7,   7,   7,   7,   7,  27,  27,  27,  27,  27,  50,  50,  50,   4,   4,  85,
     36,  36,  36,  36,  81,  36,  36,  36,  36,  36,  36,  36,  36,  36,  62,  44,
     58,  43,  43,  43,  43,  43,  43,  84,  43,  43,  61,  43,  36,  36,  70,  43,
     43,  43,  43,  43,  58,  43,  43,  43,  43,  43,  43,  43,  43,  43,  80,  67,
     67,  67,  67,  76,  67,  67,  91,  67,   2,   2,  95,  67,  21,  64,  44,  44,
     36,  36,  36,  36,  36,  92,  86,  43,  84,  43,  43,  43,  86,  84,  86,  71,
      7,   7,   7,   7,   7,   2,   2,   2,  36,  36,  36,  85,  43,  36,  36,  43,
     71,  85,  96,  92,  85,  85,  85,  36,  70,  43,  71,  36,  36,  36,  36,  36,
     36,  84,  86,  84,  85,  85,  86,  92,   7,   7,   7,   7,   7,  85,  86,  67,
     11,  11,  11,  48,  44,  44,  48,  44,  36,  36,  36,  36,  36,  63,  69,  36,
     36,  36,  36,  36,  62,  36,  36,  44,  36,  36,  36,  62,  62,  36,  36,  44,
     62,  36,  36,  44,  36,  36,  36,  62,  62,  36,  36,  44,  36,  36,  36,  36,
     36,  36,  36,  62,  36,  36,  36,  36,  36,  36,  36,  36,  36,  62,  58,  43,
      2,   2,   2,   2,  97,  27,  27,  27,  27,  27,  27,  27,  27,  27,  98,  44,
     67,  67,  67,  67,  67,  44,  44,  44,  11,  11,  11,  44,  16,  16,  16,  44,
     99,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  63,  72,
    100,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36, 101, 102,  44,
     36,  36,  36,  36,  36,  63,   2, 103, 104,  36,  36,  36,  62,  44,  44,  44,
     36,  36,  36,  36,  36,  36,  62,  36,  36,  43,  80,  44,  44,  44,  44,  44,
     36,  43,  61,  64,  44,  44,  44,  44,  36,  43,  44,  44,  44,  44,  44,  44,
     62,  43,  44,  44,  44,  44,  44,  44,  36,  36,  43,  86,  43,  43,  43,  85,
     85,  85,  85,  84,  86,  43,  43,  43,  43,  43,   2,  87,   2,  66,  70,  44,
      7,   7,   7,   7,   7,  44,  44,  44,  27,  27,  27,  27,  27,  44,  44,  44,
      2,   2,   2, 105,   2,  60,  43,  68,  36, 106,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  44,  44,  44,  44,  36,  36,  70,  71,  36,  36,  36,  36,
     36,  36,  36,  36,  70,  62,  44,  44,  36,  36,  36,  44,  44,  44,  44,  44,
     36,  36,  36,  36,  36,  36,  36,  62,  43,  84,  85,  86,  84,  85,  44,  44,
     85,  84,  85,  85,  86,  43,  44,  44,  91,  44,   2,   7,   7,   7,   7,   7,
     36,  36,  36,  36,  36,  36,  36,  44,  36,  36,  62,  44,  44,  44,  44,  44,
     36,  36,  36,  36,  36,  36,  44,  44,  36,  36,  36,  36,  36,  44,  44,  44,
      7,   7,   7,   7,   7,  98,  44,  67,  67,  67,  67,  67,  67,  67,  67,  67,
     36,  36,  36,  70,  84,  86,  44,   2,  36,  36,  92,  84,  43,  43,  43,  80,
     84,  84,  86,  43,  43,  43,  84,  85,  85,  86,  43,  43,  43,  43,  80,  58,
      2,   2,   2,  87,   2,   2,   2,  44,  43,  43,  43,  43,  43,  43,  43, 107,
     43,  43,  96,  36,  36,  36,  36,  36,  36,  36,  84,  43,  43,  84,  84,  85,
     85,  84,  96,  36,  36,  36,  44,  44,  95,  67,  67,  67,  67,  50,  43,  43,
     43,  43,  67,  67,  67,  67,  91,  44,  43,  96,  36,  36,  36,  36,  36,  36,
     92,  43,  43,  85,  43,  86,  43,  36,  36,  36,  36,  84,  43,  85,  86,  86,
     43,  85,  44,  44,  44,  44,   2,   2,  36,  36,  85,  85,  85,  85,  43,  43,
     43,  43,  85,  43,  44,  54,   2,   2,   7,   7,   7,   7,   7,  44,  81,  36,
     36,  36,  36,  36,  40,  40,  40,   2,  16,  16,  16,  16, 108,  44,  44,  44,
      2,   2,   2,   2,  44,  44,  44,  44,  43,  61,  43,  43,  43,  43,  43,  43,
     84,  43,  43,  43,  71,  36,  70,  36,  36,  85,  71,  92,  43,  44,  44,  44,
     16,  16,  16,  16,  16,  16,  40,  40,  40,  40,  40,  40,  40,  45,  16,  16,
     16,  16,  16,  16,  45,  16,  16,  16,  16,  16,  16,  16,  16, 109,  40,  40,
     43,  43,  43,  43,  43,  58,  43,  43,  32,  32,  32,  16,  16,  16,  16,  32,
     16,  16,  16,  16,  11,  11,  11,  11,  16,  16,  16,  44,  11,  11,  11,  44,
     16,  16,  16,  16,  48,  48,  48,  48,  16,  16,  16,  16,  16,  16,  16,  44,
     16,  16,  16,  16, 110, 110, 110, 110,  16,  16, 108,  16,  11,  11, 111, 112,
     41,  16, 108,  16,  11,  11, 111,  41,  16,  16,  44,  16,  11,  11, 113,  41,
     16,  16,  16,  16,  11,  11, 114,  41,  44,  16, 108,  16,  11,  11, 111, 115,
    116, 116, 116, 116, 116, 117,  65,  65, 118, 118, 118,   2, 119, 120, 119, 120,
      2,   2,   2,   2, 121,  65,  65, 122,   2,   2,   2,   2, 123, 124,   2, 125,
    126,   2, 127, 128,   2,   2,   2,   2,   2,   9, 126,   2,   2,   2,   2, 129,
     65,  65,  68,  65,  65,  65,  65,  65, 130,  44,  27,  27,  27,   8, 127, 131,
     27,  27,  27,  27,  27,   8, 127, 102,  40,  40,  40,  40,  40,  40,  82,  44,
     20,  20,  20,  20,  20,  20,  20,  20,  43,  43,  43,  43,  43,  43, 132,  51,
    133,  51, 133,  43,  43,  43,  43,  43,  80,  44,  44,  44,  44,  44,  44,  44,
     67, 134,  67, 135,  67,  34,  11,  16,  11,  32, 135,  67,  49,  11,  11,  67,
     67,  67, 134, 134, 134,  11,  11, 136,  11,  11,  35,  36,  39,  67,  16,  11,
      8,   8,  49,  16,  16,  26,  67, 137,  27,  27,  27,  27,  27,  27,  27,  27,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 138, 139, 103, 140,  67,  44,  44,
      8,   8, 141,  67,  67,   8,  67,  67, 141,  26,  67, 141,  67,  67,  67, 141,
     67,  67,  67,  67,  67,  67,  67,   8,  67, 141, 141,  67,  67,  67,  67,  67,
     67,  67,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
     67,  67,  67,  67,   4,   4,  67,  67,   8,  67,  67,  67, 142, 143,  67,  67,
     67,  67,  67,  67,  67,  67, 141,  67,  67,  67,  67,  67,  67,  26,   8,   8,
      8,   8,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,   8,   8,
      8,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  91,  44,  44,  44,  44,
     67,  67,  67,  67,  67,  91,  44,  44,  27,  27,  27,  27,  27,  27,  67,  67,
     67,  67,  67,  67,  67,  27,  27,  27,  67,  67,  67,  26,  67,  67,  67,  67,
     26,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,   8,   8,   8,   8,
     67,  67,  67,  67,  67,  67,  67,  26,  67,  67,  67,  67,   4,   4,   4,   4,
      4,   4,   4,  27,  27,  27,  27,  27,  27,  27,  67,  67,  67,  67,  67,  67,
      8,   8, 127, 144,   8,   8,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4,
      8, 127, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 144,   8,   8,   8,
      8,   8,   8,   8,   4,   4,   8,   8,   8,   8,   8,   8,   8,   8,   4,   8,
      8,   8, 141,  26,   8,   8, 141,  67,  67,  67,  44,  67,  67,  67,  67,  67,
     67,  67,  67,  44,  67,  67,  67,  67,  67,  67,  67,  67,  67,  44,  56,  67,
     67,  67,  67,  67,  91,  67,  67,  67,  67,  91,  44,  44,  44,  44,  44,  44,
     44,  44,  44,  44,  44,  44,  67,  67,  11,  11,  11,  11,  11,  11,  11,  47,
     16,  16,  16,  16,  16,  16,  16, 108,  32,  11,  32,  34,  34,  34,  34,  11,
     32,  32,  34,  16,  16,  16,  40,  11,  32,  32, 137,  67,  67, 135,  34, 146,
     43,  32,  44,  44,  54,   2,  97,   2,  16,  16,  16,  53,  44,  44,  53,  44,
     36,  36,  36,  36,  44,  44,  44,  52,  64,  44,  44,  44,  44,  44,  44,  58,
     36,  36,  36,  62,  44,  44,  44,  44,  36,  36,  36,  62,  36,  36,  36,  62,
      2, 119, 119,   2, 123, 124, 119,   2,   2,   2,   2,   6,   2, 105, 119,   2,
    119,   4,   4,   4,   4,   2,   2,  87,   2,   2,   2,   2,   2, 118,   2,   2,
    105, 147,   2,   2,   2,  44,  44,  44,  67,  67,  67,  67,  67,  56,  67,  67,
     67,  67,  44,  44,  44,  44,  44,  44,  67,  67,  67,  44,  44,  44,  44,  44,
     67,  67,  67,  67,  67,  67,  44,  44,   1,   2, 148, 149,   4,   4,   4,   4,
      4,  67,   4,   4,   4,   4, 150, 151, 152, 103, 103, 103, 103,  43,  43,  85,
    153,  40,  40,  67, 103, 154,  63,  67,  36,  36,  36,  62,  58, 155, 156,  69,
     36,  36,  36,  36,  36,  63,  40,  69,  44,  44,  81,  36,  36,  36,  36,  36,
     67,  27,  27,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,  91,
     27,  27,  27,  27,  27,  67,  67,  67,  67,  67,  67,  67,  27,  27,  27,  27,
    157,  27,  27,  27,  27,  27,  27,  27,  36,  36, 106,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36, 158,   2,   7,   7,   7,   7,   7,  36,  44,  44,
     32,  32,  32,  32,  32,  32,  32,  70,  51, 159,  43,  43,  43,  43,  43,  87,
     32,  32,  32,  32,  32,  32,  40,  43,  36,  36,  36, 103, 103, 103, 103, 103,
     43,   2,   2,   2,  44,  44,  44,  44,  41,  41,  41, 156,  40,  40,  40,  40,
     41,  32,  32,  32,  32,  32,  32,  32,  16,  32,  32,  32,  32,  32,  32,  32,
     45,  16,  16,  16,  34,  34,  34,  32,  32,  32,  32,  32,  42, 160,  34,  35,
     32,  32,  16,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  11,  11,  47,
     11,  11,  32,  32,  44,  44,  44,  44,  44,  44,  44,  81,  40,  35,  36,  36,
     36,  71,  36,  71,  36,  70,  36,  36,  36,  92,  86,  84,  67,  67,  44,  44,
     27,  27,  27,  67, 161,  44,  44,  44,  36,  36,   2,   2,  44,  44,  44,  44,
     85,  36,  36,  36,  36,  36,  36,  36,  36,  36,  85,  85,  85,  85,  85,  85,
     85,  85,  43,  44,  44,  44,  44,   2,  43,  36,  36,  36,   2,  72,  72,  44,
     36,  36,  36,  43,  43,  43,  43,   2,  36,  36,  36,  70,  43,  43,  43,  43,
     43,  85,  44,  44,  44,  44,  44,  54,  36,  70,  85,  43,  43,  85,  84,  85,
    162,   2,   2,   2,   2,   2,   2,  52,   7,   7,   7,   7,   7,  44,  44,   2,
     36,  36,  70,  69,  36,  36,  36,  36,   7,   7,   7,   7,   7,  36,  36,  62,
     36,  36,  36,  36,  70,  43,  43,  84,  86,  84,  86,  80,  44,  44,  44,  44,
     36,  70,  36,  36,  36,  36,  84,  44,   7,   7,   7,   7,   7,  44,   2,   2,
     69,  36,  36,  77,  67,  92,  84,  36,  71,  43,  71,  70,  71,  36,  36,  43,
     70,  62,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  81, 106,   2,
     36,  36,  36,  36,  36,  92,  43,  85,   2, 106, 163,  80,  44,  44,  44,  44,
     81,  36,  36,  62,  81,  36,  36,  62,  81,  36,  36,  62,  44,  44,  44,  44,
     16,  16,  16,  16,  16, 112,  40,  40,  16,  16,  16,  44,  44,  44,  44,  44,
     36,  92,  86,  85,  84, 162,  86,  44,  36,  36,  44,  44,  44,  44,  44,  44,
     36,  36,  36,  62,  44,  81,  36,  36, 164, 164, 164, 164, 164, 164, 164, 164,
    165, 165, 165, 165, 165, 165, 165, 165,  16,  16,  16, 108,  44,  44,  44,  44,
     44,  53,  16,  16,  44,  44,  81,  71,  36,  36,  36,  36, 166,  36,  36,  36,
     36,  36,  36,  62,  36,  36,  62,  62,  36,  81,  62,  36,  36,  36,  36,  36,
     36,  41,  41,  41,  41,  41,  41,  41,  41,  44,  44,  44,  44,  44,  44,  44,
     44,  81,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36, 145,
     44,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36, 161,  44,
      2,   2,   2, 167, 128,  44,  44,  44,   6, 168, 169, 145, 145, 145, 145, 145,
    145, 145, 128, 167, 128,   2, 125, 170,   2,  64,   2,   2, 150, 145, 145, 128,
      2, 171,   8, 172,  66,   2,  44,  44,  36,  36,  62,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  62,  79,  54,   2,   3,   2,   4,   5,   6,   2,
     16,  16,  16,  16,  16,  17,  18, 127, 128,   4,   2,  36,  36,  36,  36,  36,
     69,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  40,
     44,  36,  36,  36,  44,  36,  36,  36,  44,  36,  36,  36,  44,  36,  62,  44,
     20, 173,  57, 174,  26,   8, 141,  91,  44,  44,  44,  44,  79,  65,  67,  44,
     36,  36,  36,  36,  36,  36,  81,  36,  36,  36,  36,  36,  36,  62,  36,  81,
      2,  64,  44, 175,  27,  27,  27,  27,  27,  27,  44,  56,  67,  67,  67,  67,
    103, 103, 140,  27,  90,  67,  67,  67,  67,  67,  67,  67,  67,  27,  67,  91,
     91,  44,  44,  44,  44,  44,  44,  44,  67,  67,  67,  67,  67,  67,  50,  44,
    176,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,  44,  44,
     27,  27,  44,  44,  44,  44,  81,  36, 149,  36,  36,  36,  36, 177,  44,  44,
     36,  36,  36,  43,  43,  80,  44,  44,  36,  36,  36,  36,  36,  36,  36,  54,
     36,  36,  44,  44,  36,  36,  36,  36, 178, 103, 103,  44,  44,  44,  44,  44,
     11,  11,  11,  11,  16,  16,  16,  16,  11,  11,  44,  44,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  16,  44,  44,  36,  36,  44,  44,  44,  44,  44,  54,
     36,  36,  36,  44,  62,  36,  36,  36,  36,  36,  36,  81,  62,  44,  62,  81,
     36,  36,  36,  54,  27,  27,  27,  27,  36,  36,  36,  77, 157,  27,  27,  27,
     44,  44,  44, 175,  27,  27,  27,  27,  36,  62,  36,  44,  44, 175,  27,  27,
     36,  36,  36,  27,  27,  27,  44,  54,  36,  36,  36,  36,  36,  44,  44,  54,
     36,  36,  36,  36,  44,  44,  27,  36,  44,  27,  27,  27,  27,  27,  27,  27,
     70,  43,  58,  80,  44,  44,  43,  43,  36,  36,  81,  36,  81,  36,  36,  36,
     36,  36,  44,  44,  43,  80,  44,  58,  27,  27,  27,  27,  44,  44,  44,  44,
      2,   2,   2,   2,  64,  44,  44,  44,  36,  36,  36,  36,  36,  36, 179,  30,
     36,  36,  36,  36,  36,  36, 179,  27,  36,  36,  36,  36,  78,  36,  36,  36,
     36,  36,  70,  80,  44, 175,  27,  27,   2,   2,   2,  64,  44,  44,  44,  44,
     36,  36,  36,  44,  54,   2,   2,   2,  36,  36,  36,  44,  27,  27,  27,  27,
     36,  62,  44,  44,  27,  27,  27,  27,  36,  44,  44,  44,  54,   2,  64,  44,
     44,  44,  44,  44, 175,  27,  27,  27,  36,  36,  36,  36,  62,  44,  44,  44,
     11,  47,  44,  44,  44,  44,  44,  44,  16, 108,  44,  44,  44,  27,  27,  27,
     27,  27,  27,  27,  27,  27,  27,  98,  86,  96,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  43,  43,  43,  43,  43,  43,  43,  61,   2,   2,   2,  44,
     27,  27,  27,   7,   7,   7,   7,   7,  44,  44,  44,  44,  44,  44,  44,  58,
     85,  86,  43,  84,  86,  61, 180,   2,   2,  44,  44,  44,  44,  44,  44,  44,
     43,  71,  36,  36,  36,  36,  36,  36,  36,  36,  36,  70,  43,  43,  86,  43,
     43,  43,  80,   7,   7,   7,   7,   7,   2,   2,  44,  44,  44,  44,  44,  44,
     36,  70,   2,  62,  44,  44,  44,  44,  36,  92,  85,  43,  43,  43,  43,  84,
     96,  36,  63,   2,   2,  43,  61,  44,   7,   7,   7,   7,   7,  63,  63,   2,
    175,  27,  27,  27,  27,  27,  27,  27,  27,  27,  98,  44,  44,  44,  44,  44,
     36,  36,  36,  36,  36,  36,  85,  86,  43,  85,  84,  43,   2,   2,   2,  80,
     36,  36,  36,  62,  62,  36,  36,  81,  36,  36,  36,  36,  36,  36,  36,  81,
     36,  36,  36,  36,  63,  44,  44,  44,  36,  36,  36,  36,  36,  36,  36,  70,
     85,  86,  43,  43,  43,  80,  44,  44,  43,  85,  81,  36,  36,  36,  62,  81,
     84,  85,  89,  88,  89,  88,  85,  44,  62,  44,  44,  88,  44,  44,  81,  36,
     36,  85,  44,  43,  43,  43,  80,  44,  43,  43,  80,  44,  44,  44,  44,  44,
     36,  36,  92,  85,  43,  43,  43,  43,  85,  43,  84,  71,  36,  63,   2,   2,
      7,   7,   7,   7,   7,  54,  54,  44,  85,  86,  43,  43,  84,  84,  85,  86,
     84,  43,  36,  72,  44,  44,  44,  44,  36,  36,  36,  36,  36,  36,  36,  92,
     85,  43,  43,  44,  85,  85,  43,  86,  61,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,  36,  36,  43,  44,  85,  86,  43,  43,  43,  84,  86,  86,
     61,   2,  62,  44,  44,  44,  44,  44,   2,   2,   2,   2,   2,   2,  64,  44,
     36,  36,  36,  36,  36,  70,  86,  85,  43,  43,  43,  86,  44,  44,  44,  44,
     36,  36,  36,  36,  36,  44,  58,  43,  85,  43,  43,  86,  43,  43,  44,  44,
      7,   7,   7,   7,   7,  27,   2,  95,  27,  98,  44,  44,  44,  44,  44,  81,
     70,  43,  43,  84,  86,  71,  36,  36,  36,  70,  43,  43,  84,  70,  43,  61,
      2,   2,   2,  60,  44,  44,  44,  44,  70,  43,  43,  84,  86,  43,  36,  36,
     36,  36,  44,  36,  36,  43,  43,  43,  43,  43,  43,  84,  43,   2,  64,   2,
      2,  64,  44,  44,  44,  44,  44,  44,  43,  43,  43,  80,  43,  43,  43,  86,
     63,   2,   2,  44,  44,  44,  44,  44,   2,  36,  36,  36,  36,  36,  36,  36,
     44,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  88,  43,  43,  43,
     84,  43,  86,  80,  44,  44,  44,  44,  36,  36,  36,  62,  36,  81,  36,  36,
     70,  43,  43,  80,  44,  80,  43,  58,  43,  43,  43,  70,  44,  44,  44,  44,
    103, 103, 103, 103, 103, 103, 103, 177,   2,   2,  64,  44,  44,  44,  44,  44,
     43,  43,  61,  44,  44,  44,  44,  44,  43,  43,  43,  61,   2,   2,  67,  67,
     40,  40,  95,  44,  44,  44,  44,  44,   7,   7,   7,   7,   7, 175,  27,  27,
     27,  81,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  44,  44,  81,  36,
     92,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,
     85,  85,  85,  85,  85,  85,  85,  89,  43,  74,  40,  40,  40,  40,  40,  40,
     40,  44,  44,  44,  44,  44,  44,  44,  36,  62,  44,  44,  44,  44,  44,  44,
     36,  36,  36,  36,  36,  44,  50,  61,  65,  65,  44,  44,  44,  44,  44,  44,
     67,  67,  67,  91,  56,  67,  67,  67,  67,  67, 181,  86,  43,  67, 181,  85,
     85, 182,  65,  65,  65,  83,  43,  43,  43,  76,  50,  43,  43,  43,  67,  67,
     67,  67,  67,  67,  67,  43,  43,  67,  67,  67,  67,  67,  91,  44,  44,  44,
     67,  43,  76,  44,  44,  44,  44,  44,  27,  44,  44,  44,  44,  44,  44,  44,
     11,  11,  11,  11,  11,  16,  16,  16,  16,  16,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  11,  11,  11,  16,  16,  16, 108,  16,  16,  16,  16,  16,
     11,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  47,  11,
     44,  47,  48,  47,  48,  11,  47,  11,  11,  11,  11,  16,  16,  53,  53,  16,
     16,  16,  53,  16,  16,  16,  16,  16,  16,  16,  11,  48,  11,  47,  48,  11,
     11,  11,  47,  11,  11,  11,  47,  16,  16,  16,  16,  16,  11,  48,  11,  47,
     11,  11,  47,  47,  44,  11,  11,  11,  47,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  16,  11,  11,  11,  11,  11,  16,  16,  16,  16,  16,
     16,  16,  16,  44,  11,  11,  11,  11,  31,  16,  16,  16,  16,  16,  16,  16,
     16,  16,  16,  16,  16,  33,  16,  16,  16,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  11,  31,  16,  16,  16,  16,  33,  16,  16,  16,  11,  11,
     11,  11,  31,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  33,
     16,  16,  16,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  31,
     16,  16,  16,  16,  33,  16,  16,  16,  11,  11,  11,  11,  31,  16,  16,  16,
     16,  33,  16,  16,  16,  32,  44,   7,   7,   7,   7,   7,   7,   7,   7,   7,
     43,  43,  43,  76,  67,  50,  43,  43,  43,  43,  43,  43,  43,  43,  76,  67,
     67,  67,  50,  67,  67,  67,  67,  67,  67,  67,  76,  21,   2,   2,  44,  44,
     44,  44,  44,  44,  44,  58,  43,  43,  43,  43,  43,  80,  43,  43,  43,  43,
     43,  43,  43,  43,  80,  58,  43,  43,  43,  58,  80,  43,  43,  80,  44,  44,
     36,  36,  62, 175,  27,  27,  27,  27,  43,  43,  43,  80,  44,  44,  44,  44,
     16,  16,  43,  43,  43,  80,  44,  44,  36,  36,  81,  36,  36,  36,  36,  36,
     81,  62,  62,  81,  81,  36,  36,  36,  36,  62,  36,  36,  81,  81,  44,  44,
     44,  62,  44,  81,  81,  81,  81,  36,  81,  62,  62,  81,  81,  81,  81,  81,
     81,  62,  62,  81,  36,  62,  36,  36,  36,  62,  36,  36,  81,  36,  62,  62,
     36,  36,  36,  36,  36,  81,  36,  36,  81,  36,  81,  36,  36,  81,  36,  36,
      8,  44,  44,  44,  44,  44,  44,  44,  56,  67,  67,  67,  67,  67,  67,  67,
     67,  67,  67,  67,  67,  67,  91,  44,  44,  44,  44,  67,  67,  67,  67,  67,
     67,  44,  44,  44,  44,  44,  44,  44,  67,  67,  67,  67,  67,  25,  41,  41,
     67,  67,  91,  44,  44,  44,  44,  44,  67,  67,  67,  67,  44,  44,  44,  44,
     67,  67,  67,  67,  67,  67,  67,  44,  36,  44,  44,  44,  44,  44,  44,  44,
     79,  44,  44,  44,  44,  44,  44,  44,  65,  65,  65,  65,  65,  65,  65,  65,
    165, 165, 165, 165, 165, 165, 165,  44,
};

static RE_UINT8 re_general_category_stage_5[] = {
    15, 15, 12, 23, 23, 23, 25, 23, 20, 21, 23, 24, 23, 19,  9,  9,
    24, 24, 24, 23, 23,  1,  1,  1,  1, 20, 23, 21, 26, 22, 26,  2,
     2,  2,  2, 20, 24, 21, 24, 15, 25, 25, 27, 23, 26, 27,  5, 28,
    24, 16, 27, 26, 27, 24, 11, 11, 26, 11,  5, 29, 11, 23,  1, 24,
     1,  2,  2, 24,  2,  1,  2,  5,  5,  5,  1,  3,  3,  2,  5,  2,
     4,  4, 26, 26,  4, 26,  6,  6,  0,  0,  4,  2,  1, 23,  1,  0,
     0,  1, 24,  1, 27,  6,  7,  7,  0,  4,  0,  2,  0, 23, 19,  0,
     0, 27, 27, 25,  0,  6, 19,  6, 23,  6,  6, 23,  5,  0,  5, 23,
    23,  0, 16, 16, 23, 25, 27, 27, 16,  0,  4,  5,  5,  6,  6,  5,
    23,  5,  6, 16,  6,  4,  4,  6,  6, 27,  5, 27, 27,  5,  0, 16,
     6,  0,  0,  5,  4,  0, 16,  6,  6,  8,  8,  8,  8,  6, 23,  4,
     0,  8,  8,  0, 11, 27, 27,  0,  5,  8, 11,  5,  0, 25, 23, 27,
     8,  5, 23, 11, 11,  0, 19,  5, 12,  5,  5, 20, 21,  0, 10, 10,
    10,  5, 19, 23,  5,  4,  7,  0,  2,  0,  2,  4,  3,  3,  3, 26,
     2, 26,  0, 26,  1, 26, 26,  0, 12, 12, 12, 16, 19, 19, 28, 29,
    20, 28, 13, 14, 16, 12, 23, 28, 29, 23, 23, 22, 22, 23, 24, 20,
    21, 23, 23, 12, 11,  4, 21,  4,  6,  7,  7,  6,  1, 27, 27,  1,
    27,  2,  2, 27, 10,  1,  2, 10, 10, 11, 24, 27, 27, 20, 21, 27,
    21, 24, 21, 20,  2,  6, 20, 23, 27,  4,  5, 10, 19, 20, 21, 21,
    27, 10, 19,  4, 10,  4,  6, 26, 26,  4, 27, 11,  4, 23,  7, 23,
    26,  1, 25, 27,  8, 23,  4,  8, 18, 18, 17, 17,  5, 24, 23, 20,
    19, 22, 22, 20, 22, 22, 24, 19, 24,  0, 24, 26, 25,  0,  0, 11,
     6, 11, 10,  0, 23, 10,  5, 11, 23, 16, 27,  8,  8, 16,
};

/* General_Category: 10086 bytes. */

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
    17, 18, 19, 20, 21, 22, 23, 22, 24, 22, 22, 22, 22, 25, 26, 26,
    26, 27, 22, 22, 22, 22, 28, 29, 22, 22, 30, 31, 32, 33, 34, 35,
    36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
    36, 36, 36, 36, 37, 38, 39, 40, 41, 42, 43, 43, 43, 44, 22, 45,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    46, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47,
    47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47,
    48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
    48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
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
    115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 119, 126, 126, 126, 119,
    127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 119, 119, 137, 119, 119, 119,
    138, 139, 140, 141, 142, 143, 144, 119, 145, 146, 119, 147, 148, 149, 150, 119,
    119, 151, 119, 119, 152, 153, 119, 119, 154, 155, 156, 119, 119, 119, 119, 119,
    157, 157, 157, 157, 157, 157, 157, 157, 158, 159, 160, 119, 119, 119, 119, 119,
    119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    161, 161, 161, 161, 161, 161, 161, 161, 162, 119, 119, 119, 119, 119, 119, 119,
    119, 119, 119, 119, 119, 119, 119, 119, 163, 163, 163, 163, 163, 119, 119, 119,
    164, 164, 164, 164, 165, 166, 167, 168, 119, 119, 119, 119, 119, 119, 169, 170,
    171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
    172, 172, 172, 172, 172, 172, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    173, 173, 174, 175, 175, 175, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    119, 119, 119, 119, 119, 119, 119, 119, 176, 177, 119, 119, 119, 119, 119, 119,
    178, 178, 179, 179, 180, 119, 181, 119, 182, 182, 182, 182, 182, 182, 182, 182,
    183, 183, 183, 183, 183, 184, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    185, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    186, 187, 188, 119, 119, 119, 119, 119, 119, 119, 119, 119, 189, 189, 119, 119,
    190, 191, 192, 192, 193, 193, 194, 194, 194, 194, 194, 194, 195, 196, 197, 198,
    199, 199, 200, 200, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
    201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 202, 203, 203,
    203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203,
    203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 204, 205,
    206, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207,
    207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207,
    207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 208, 209, 209,
    209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
    209, 209, 209, 209, 209, 209, 209, 210, 119, 119, 119, 119, 119, 119, 119, 119,
    211, 211, 211, 211, 212, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    213, 119, 214, 215, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216,
    217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217,
};

static RE_UINT16 re_block_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
      2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,
      3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,
      6,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      8,   8,   8,   8,   8,   8,   8,   8,   9,   9,   9,  10,  10,  10,  10,  10,
     10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
     13,  13,  13,  13,  13,  14,  14,  14,  15,  15,  15,  15,  16,  16,  16,  16,
     17,  17,  17,  17,  18,  18,  19,  20,  20,  20,  21,  21,  21,  21,  21,  21,
     22,  22,  22,  22,  22,  22,  22,  22,  23,  23,  23,  23,  23,  23,  23,  23,
     24,  24,  24,  24,  24,  24,  24,  24,  25,  25,  25,  25,  25,  25,  25,  25,
     26,  26,  26,  26,  26,  26,  26,  26,  27,  27,  27,  27,  27,  27,  27,  27,
     28,  28,  28,  28,  28,  28,  28,  28,  29,  29,  29,  29,  29,  29,  29,  29,
     30,  30,  30,  30,  30,  30,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
     32,  32,  32,  32,  32,  32,  32,  32,  33,  33,  33,  33,  33,  33,  33,  33,
     34,  34,  34,  34,  34,  34,  34,  34,  35,  35,  35,  35,  35,  35,  35,  35,
     35,  35,  36,  36,  36,  36,  36,  36,  37,  37,  37,  37,  37,  37,  37,  37,
     38,  38,  38,  38,  38,  38,  38,  38,  39,  39,  40,  40,  40,  40,  40,  40,
     41,  41,  41,  41,  41,  41,  41,  41,  42,  42,  43,  43,  43,  43,  43,  43,
     44,  44,  45,  45,  46,  46,  47,  47,  48,  48,  48,  48,  48,  48,  48,  48,
     49,  49,  49,  49,  49,  49,  49,  49,  49,  49,  49,  50,  50,  50,  50,  50,
     51,  51,  51,  51,  51,  52,  52,  52,  53,  53,  53,  53,  53,  53,  54,  54,
     55,  55,  56,  56,  56,  56,  56,  56,  56,  56,  56,  57,  57,  57,  57,  57,
     58,  58,  58,  58,  58,  58,  58,  58,  59,  59,  59,  59,  60,  60,  60,  60,
     61,  61,  61,  61,  61,  62,  62,  62,  63,  20,  20,  20,  64,  65,  65,  65,
     66,  66,  66,  66,  66,  66,  66,  66,  67,  67,  67,  67,  68,  68,  68,  68,
     69,  69,  69,  69,  69,  69,  69,  69,  70,  70,  70,  70,  70,  70,  70,  70,
     71,  71,  71,  71,  71,  71,  71,  72,  72,  72,  73,  73,  73,  74,  74,  74,
     75,  75,  75,  75,  75,  76,  76,  76,  76,  77,  77,  77,  77,  77,  77,  77,
     78,  78,  78,  78,  78,  78,  78,  78,  79,  79,  79,  79,  79,  79,  79,  79,
     80,  80,  80,  80,  81,  81,  82,  82,  82,  82,  82,  82,  82,  82,  82,  82,
     83,  83,  83,  83,  83,  83,  83,  83,  84,  84,  85,  85,  85,  85,  85,  85,
     86,  86,  86,  86,  86,  86,  86,  86,  87,  87,  87,  87,  87,  87,  87,  87,
     87,  87,  87,  87,  88,  88,  88,  89,  90,  90,  90,  90,  90,  90,  90,  90,
     91,  91,  91,  91,  91,  91,  91,  91,  92,  92,  92,  92,  92,  92,  92,  92,
     93,  93,  93,  93,  93,  93,  93,  93,  94,  94,  94,  94,  94,  94,  94,  94,
     95,  95,  95,  95,  95,  95,  96,  96,  97,  97,  97,  97,  97,  97,  97,  97,
     98,  98,  98,  99,  99,  99,  99,  99, 100, 100, 100, 100, 100, 100, 101, 101,
    102, 102, 102, 102, 102, 102, 102, 102, 103, 103, 103, 103, 103, 103, 103, 103,
    104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104,  20, 105,
    106, 106, 106, 106, 107, 107, 107, 107, 107, 107, 108, 108, 108, 108, 108, 108,
    109, 109, 109, 110, 110, 110, 110, 110, 110, 111, 112, 112, 113, 113, 113, 114,
    115, 115, 115, 115, 115, 115, 115, 115, 116, 116, 116, 116, 116, 116, 116, 116,
    117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 118, 118, 118, 118,
    119, 119, 119, 119, 119, 119, 119, 119, 120, 120, 120, 120, 120, 120, 120, 120,
    120, 121, 121, 121, 121, 122, 122, 122, 123, 123, 123, 123, 123, 123, 123, 123,
    123, 123, 123, 123, 124, 124, 124, 124, 124, 124, 125, 125, 125, 125, 125, 125,
    126, 126, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    128, 128, 128, 129, 130, 130, 130, 130, 131, 131, 131, 131, 131, 131, 132, 132,
    133, 133, 133, 134, 134, 134, 135, 135, 136, 136, 136, 136, 136, 136, 137, 137,
    138, 138, 138, 138, 138, 138, 139, 139, 140, 140, 140, 140, 140, 140, 141, 141,
    142, 142, 142, 143, 143, 143, 143, 144, 144, 144, 144, 144, 145, 145, 145, 145,
    146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 147, 147, 147, 147, 147,
    148, 148, 148, 148, 148, 148, 148, 148, 149, 149, 149, 149, 149, 149, 149, 149,
    150, 150, 150, 150, 150, 150, 150, 150, 151, 151, 151, 151, 151, 151, 151, 151,
    152, 152, 152, 152, 152, 152, 152, 152, 153, 153, 153, 153, 153, 154, 154, 154,
    154, 154, 154, 154, 154, 154, 154, 154, 155, 156, 157, 158, 158, 159, 159, 160,
    160, 160, 160, 160, 160, 160, 160, 160, 161, 161, 161, 161, 161, 161, 161, 161,
    161, 161, 161, 161, 161, 161, 161, 162, 163, 163, 163, 163, 163, 163, 163, 163,
    164, 164, 164, 164, 164, 164, 164, 164, 165, 165, 165, 165, 166, 166, 166, 166,
    166, 167, 167, 167, 167, 168, 168, 168,  20,  20,  20,  20,  20,  20,  20,  20,
    169, 169, 170, 170, 170, 170, 171, 171, 172, 172, 172, 173, 173, 174, 174, 174,
    175, 175, 176, 176, 176, 176,  20,  20, 177, 177, 177, 177, 177, 178, 178, 178,
    179, 179, 179, 180, 180, 180, 180, 180, 181, 181, 181, 182, 182, 182, 182,  20,
    183, 183, 183, 183, 183, 183, 183, 183, 184, 184, 184, 184, 185, 185, 186, 186,
    187, 187, 187,  20,  20,  20, 188, 188, 189, 189, 190, 190,  20,  20,  20,  20,
    191, 191, 192, 192, 192, 192, 192, 192, 193, 193, 193, 193, 193, 193, 194, 194,
    195, 195,  20,  20, 196, 196, 196, 196, 197, 197, 197, 197, 198, 198, 199, 199,
    200, 200, 200,  20,  20,  20,  20,  20, 201, 201, 201, 201, 201,  20,  20,  20,
    202, 202, 202, 202, 202, 202, 202, 202,  20,  20,  20,  20,  20,  20, 203, 203,
    204, 204, 204, 204, 204, 204, 204, 204, 205, 205, 205, 205, 205, 206, 206, 206,
    207, 207, 207, 207, 207, 208, 208, 208, 209, 209, 209, 209, 209, 209, 210, 210,
    211, 211, 211, 211, 211,  20,  20,  20, 212, 212, 212, 213, 213, 213, 213, 213,
    214, 214, 214, 214, 214, 214, 214, 214, 215, 215, 215, 215, 215, 215, 215, 215,
    216, 216, 216, 216, 216, 216,  20,  20, 217, 217, 217, 217, 217, 217, 217, 217,
    218, 218, 218, 218, 218, 218, 219, 219, 220, 220, 220, 220, 220,  20,  20,  20,
    221, 221, 221, 221,  20,  20,  20,  20,  20,  20, 222, 222, 222, 222, 222, 222,
    223, 223, 223, 223, 223, 224, 224, 224, 224, 224, 224,  20, 225, 225, 225, 225,
    226, 226, 226, 226, 226, 226, 226, 227, 227, 227, 227, 227,  20,  20,  20,  20,
    228, 228, 228, 228, 228, 228,  20,  20, 229, 229, 229, 229, 229, 229, 229, 229,
    230, 230, 230, 230, 230, 230, 230, 230, 231, 231, 231, 231, 231, 231, 231, 231,
    231, 231, 231, 231, 231,  20,  20,  20, 232, 232, 232, 232, 232, 232, 232, 232,
    232, 232, 232,  20,  20,  20,  20,  20, 233, 233, 233, 233, 233, 233, 233, 233,
    234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 235, 235, 235,  20,
     20,  20,  20,  20,  20, 236, 236, 236, 237, 237, 237, 237, 237, 237, 237, 237,
    237,  20,  20,  20,  20,  20,  20,  20, 238, 238, 238, 238, 238, 238, 238, 238,
    238, 238,  20,  20,  20,  20, 239, 239, 240, 240, 240, 240, 240, 240, 240, 240,
    241, 241, 241, 241, 241, 241, 241, 241, 242, 242, 242, 242, 242, 242, 242, 242,
    243, 243, 243,  20,  20,  20,  20, 244, 244, 244, 244, 244, 244, 244, 244, 244,
    245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 246,  20,  20,  20,  20,  20,
    247, 247, 247, 247, 247, 247, 247, 247, 248, 248, 248, 248, 248, 248, 248, 248,
    249, 249, 249, 249, 249,  20,  20,  20, 250, 250, 250, 250, 250, 250, 251, 251,
    252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253,
    253, 253, 253,  20,  20,  20,  20,  20, 254, 254, 254,  20,  20,  20,  20,  20,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  20,  20,
    256, 256, 256, 256, 256, 256,  20,  20, 257, 257, 257, 257, 257, 257, 257, 257,
    258, 258, 258, 259, 259, 259, 259, 259, 259, 259, 260, 260, 260, 260, 260, 260,
    261, 261, 261, 261, 261, 261, 261, 261, 262, 262, 262, 262, 262, 262, 262, 262,
    263, 263, 263, 263, 263, 263, 263, 263, 264, 264, 264, 264, 264, 265, 265, 265,
    266, 266, 266, 266, 266, 266, 266, 266, 267, 267, 267, 267, 267, 267, 267, 267,
    268, 268, 268, 268, 268, 268, 268, 268, 269, 269, 269, 269, 269, 269, 269, 269,
    270, 270, 270, 270, 270, 270, 270, 270, 271, 271, 271, 271, 271, 271, 271, 271,
    271, 271, 271, 271, 271, 271,  20,  20, 272, 272, 272, 272, 272, 272, 272, 272,
    272, 272, 272, 272, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273,
    273, 273, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274,
    274, 274, 274, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
    275, 275, 275, 275, 275, 275, 275,  20, 276, 276, 276, 276, 276, 276, 276, 276,
    276, 276,  20,  20,  20,  20,  20,  20, 277, 277, 277, 277, 277, 277, 277, 277,
    278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278,  20,
    279, 279, 279, 279, 279, 279, 279, 279, 280, 280, 280, 280, 280, 280, 280, 280,
};

static RE_UINT16 re_block_stage_4[] = {
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
    220, 220, 220, 220, 221, 221, 221, 221, 222, 222, 222, 222, 223, 223, 223, 223,
    224, 224, 224, 224, 225, 225, 225, 225, 226, 226, 226, 226, 227, 227, 227, 227,
    228, 228, 228, 228, 229, 229, 229, 229, 230, 230, 230, 230, 231, 231, 231, 231,
    232, 232, 232, 232, 233, 233, 233, 233, 234, 234, 234, 234, 235, 235, 235, 235,
    236, 236, 236, 236, 237, 237, 237, 237, 238, 238, 238, 238, 239, 239, 239, 239,
    240, 240, 240, 240, 241, 241, 241, 241, 242, 242, 242, 242, 243, 243, 243, 243,
    244, 244, 244, 244, 245, 245, 245, 245, 246, 246, 246, 246, 247, 247, 247, 247,
    248, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 251, 251, 251, 251,
    252, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 254, 255, 255, 255, 255,
    256, 256, 256, 256, 257, 257, 257, 257, 258, 258, 258, 258, 259, 259, 259, 259,
    260, 260, 260, 260, 261, 261, 261, 261, 262, 262, 262, 262, 263, 263, 263, 263,
    264, 264, 264, 264, 265, 265, 265, 265, 266, 266, 266, 266, 267, 267, 267, 267,
    268, 268, 268, 268, 269, 269, 269, 269, 270, 270, 270, 270, 271, 271, 271, 271,
    272, 272, 272, 272, 273, 273, 273, 273, 274, 274, 274, 274, 275, 275, 275, 275,
    276, 276, 276, 276, 277, 277, 277, 277, 278, 278, 278, 278, 279, 279, 279, 279,
    280, 280, 280, 280,
};

static RE_UINT16 re_block_stage_5[] = {
      1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,
      5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,   8,
      9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,  12,  12,
     13,  13,  13,  13,  14,  14,  14,  14,  15,  15,  15,  15,  16,  16,  16,  16,
     17,  17,  17,  17,  18,  18,  18,  18,  19,  19,  19,  19,  20,  20,  20,  20,
      0,   0,   0,   0,  21,  21,  21,  21,  22,  22,  22,  22,  23,  23,  23,  23,
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
    220, 220, 220, 220, 221, 221, 221, 221, 222, 222, 222, 222, 223, 223, 223, 223,
    224, 224, 224, 224, 225, 225, 225, 225, 226, 226, 226, 226, 227, 227, 227, 227,
    228, 228, 228, 228, 229, 229, 229, 229, 230, 230, 230, 230, 231, 231, 231, 231,
    232, 232, 232, 232, 233, 233, 233, 233, 234, 234, 234, 234, 235, 235, 235, 235,
    236, 236, 236, 236, 237, 237, 237, 237, 238, 238, 238, 238, 239, 239, 239, 239,
    240, 240, 240, 240, 241, 241, 241, 241, 242, 242, 242, 242, 243, 243, 243, 243,
    244, 244, 244, 244, 245, 245, 245, 245, 246, 246, 246, 246, 247, 247, 247, 247,
    248, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 251, 251, 251, 251,
    252, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 254, 255, 255, 255, 255,
    256, 256, 256, 256, 257, 257, 257, 257, 258, 258, 258, 258, 259, 259, 259, 259,
    260, 260, 260, 260, 261, 261, 261, 261, 262, 262, 262, 262, 263, 263, 263, 263,
    264, 264, 264, 264, 265, 265, 265, 265, 266, 266, 266, 266, 267, 267, 267, 267,
    268, 268, 268, 268, 269, 269, 269, 269, 270, 270, 270, 270, 271, 271, 271, 271,
    272, 272, 272, 272, 273, 273, 273, 273, 274, 274, 274, 274, 275, 275, 275, 275,
    276, 276, 276, 276, 277, 277, 277, 277, 278, 278, 278, 278, 279, 279, 279, 279,
    280, 280, 280, 280,
};

/* Block: 9312 bytes. */

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
    16, 17, 18, 19, 20, 14, 21, 14, 22, 14, 14, 14, 14, 23, 24, 24,
    25, 26, 14, 14, 14, 14, 27, 28, 14, 14, 29, 30, 31, 32, 33, 34,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 35,  7, 36, 37,  7, 38,  7,  7,  7, 39, 14, 40,
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
    41, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
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
     55,  56,  57,  58,  59,  59,  59,  59,  60,  59,  59,  59,  59,  59,  59,  59,
     61,  61,  59,  59,  59,  59,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,
     72,  73,  74,  75,  76,  77,  78,  59,  70,  70,  70,  70,  70,  70,  70,  70,
     70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,
     70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  79,  70,  70,  70,  70,
     70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  80,
     81,  81,  81,  81,  81,  81,  81,  81,  81,  82,  83,  83,  84,  85,  86,  87,
     88,  89,  90,  91,  92,  93,  94,  95,  32,  32,  32,  32,  32,  32,  32,  32,
     32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
     32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  96,
     97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,
     97,  97,  70,  70,  98,  99, 100, 101, 102, 102, 103, 104, 105, 106, 107, 108,
    109, 110, 111, 112,  97, 113, 114, 115, 116, 117, 118,  97, 119, 119, 120,  97,
    121, 122, 123, 124, 125, 126, 127, 128, 129, 130,  97,  97, 131,  97,  97,  97,
    132, 133, 134, 135, 136, 137, 138,  97, 139, 140,  97, 141, 142, 143, 144,  97,
     97, 145,  97,  97, 146, 147,  97,  97, 148, 149, 150,  97,  97,  97,  97,  97,
    151, 151, 151, 151, 151, 151, 151, 152, 153, 151, 154,  97,  97,  97,  97,  97,
    155, 155, 155, 155, 155, 155, 155, 155, 156,  97,  97,  97,  97,  97,  97,  97,
     97,  97,  97,  97,  97,  97,  97,  97, 157, 157, 157, 157, 158,  97,  97,  97,
    159, 159, 159, 159, 160, 161, 162, 163,  97,  97,  97,  97,  97,  97, 164, 165,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 167,
    166, 166, 166, 166, 166, 168,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,
    169, 170, 171, 172, 172, 173,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,
     97,  97,  97,  97,  97,  97,  97,  97, 174, 175,  97,  97,  97,  97,  97,  97,
     59, 176, 177, 178, 179,  97, 180,  97, 181, 182, 183,  59,  59, 184,  59, 185,
    186, 186, 186, 186, 186, 187,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,
    188,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,
    189, 190, 191,  97,  97,  97,  97,  97,  97,  97,  97,  97, 192, 193,  97,  97,
    194, 195, 196, 197, 198,  97,  59,  59,  59,  59,  59,  59,  59, 199, 200, 201,
    202, 203, 204, 205,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,
     70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70, 206,  70,  70,
     70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70, 207,  70,
    208,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,
     70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70, 209,  70,  70,
     70,  70,  70,  70,  70,  70,  70, 210,  97,  97,  97,  97,  97,  97,  97,  97,
     70,  70,  70,  70, 211,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,
    212,  97, 213, 214,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,  97,
};

static RE_UINT16 re_script_stage_3[] = {
      0,   0,   0,   0,   1,   2,   1,   2,   0,   0,   3,   3,   4,   5,   4,   5,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   6,   0,   0,   7,   0,
      8,   8,   8,   8,   8,   8,   8,   9,  10,  11,  12,  11,  11,  11,  13,  11,
     14,  14,  14,  14,  14,  14,  14,  14,  15,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  16,  17,  18,  16,  17,  19,  20,  21,  21,  22,  21,  23,  24,
     25,  26,  27,  27,  28,  29,  27,  30,  27,  27,  27,  27,  27,  31,  27,  27,
     32,  33,  33,  33,  34,  27,  27,  27,  35,  35,  35,  36,  37,  37,  37,  38,
     39,  39,  40,  41,  42,  43,  44,  45,  45,  45,  27,  46,  45,  47,  48,  27,
     49,  49,  49,  49,  49,  50,  51,  49,  52,  53,  54,  55,  56,  57,  58,  59,
     60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,
     76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,
     92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107,
    108, 109, 110, 110, 111, 112, 113, 110, 114, 115, 116, 117, 118, 119, 120, 121,
    122, 123, 123, 124, 123, 125,  45,  45, 126, 127, 128, 129, 130, 131,  45,  45,
    132, 132, 132, 132, 133, 132, 134, 135, 132, 133, 132, 136, 136, 137,  45,  45,
    138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 139, 139, 140, 139, 139, 141,
    142, 142, 142, 142, 142, 142, 142, 142, 143, 143, 143, 143, 144, 145, 143, 143,
    144, 143, 143, 146, 147, 148, 143, 143, 143, 147, 143, 143, 143, 149, 143, 150,
    143, 151, 152, 152, 152, 152, 152, 153, 154, 154, 154, 154, 154, 154, 154, 154,
    155, 156, 157, 157, 157, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
    168, 168, 168, 168, 168, 169, 170, 170, 171, 172, 173, 173, 173, 173, 173, 174,
    173, 173, 175, 154, 154, 154, 154, 176, 177, 178, 179, 179, 180, 181, 182, 183,
    184, 184, 185, 184, 186, 187, 168, 168, 188, 189, 190, 190, 190, 191, 190, 192,
    193, 193, 194, 195,  45,  45,  45,  45, 196, 196, 196, 196, 197, 196, 196, 198,
    199, 199, 199, 199, 200, 200, 200, 201, 202, 202, 202, 203, 204, 205, 205, 205,
    206,  45,  45,  45, 207, 208, 209, 210,   4,   4, 211,   4,   4, 212, 213, 214,
      4,   4,   4, 215,   8,   8,   8, 216,  11, 217,  11,  11, 217, 218,  11, 219,
     11,  11,  11, 220, 220, 221,  11, 222, 223,   0,   0,   0,   0,   0, 224, 225,
    226, 227,   0,   0,  45,   8,   8, 228,   0,   0, 229, 230, 231,   0,   4,   4,
    232,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0, 233,  45, 234,  45,   0,   0, 235, 235, 235, 235, 235, 235, 235, 235,
      0,   0,   0,   0,   0,   0,   0, 236,   0, 237,   0, 238, 239, 240, 241,  45,
    242, 242, 243, 242, 242, 243,   4,   4, 244, 244, 244, 244, 244, 244, 244, 245,
    139, 139, 140, 246, 246, 246, 247, 248, 143, 249, 250, 250, 250, 250,  14,  14,
      0,   0,   0,   0, 251,  45,  45,  45, 252, 253, 252, 252, 252, 252, 252, 254,
    252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 255,  45, 256,
    257,   0, 258, 259, 260, 261, 261, 261, 261, 262, 263, 264, 264, 264, 264, 265,
    266, 267, 268, 269, 142, 142, 142, 142, 270,   0, 267, 271,   0,   0, 272, 264,
    142, 270,   0,   0,   0,   0, 142, 273,   0,   0,   0,   0,   0, 264, 264, 274,
    264, 264, 264, 264, 264, 275,   0,   0, 252, 252, 252, 255,   0,   0,   0,   0,
    252, 252, 252, 252, 252, 252, 276,  45, 277, 277, 277, 277, 277, 277, 277, 277,
    278, 277, 277, 277, 279, 280, 280, 280, 281, 281, 281, 281, 281, 281, 281, 281,
    281, 281, 282,  45,  14,  14,  14,  14,  14,  14, 283, 283, 283, 283, 283, 284,
      0,   0, 285,   4,   4,   4,   4,   4, 286,   4, 287, 288,  45,  45,  45, 289,
    290, 290, 291, 251, 292, 292, 292, 293, 294, 294, 294, 294, 295, 296,  49, 297,
    298, 298, 299, 300, 300, 301, 142, 302, 303, 303, 303, 303, 304, 305, 138, 306,
    307, 307, 307, 308, 309, 310, 138, 138, 311, 311, 311, 311, 312, 313, 314, 315,
    316, 317, 250,   4,   4, 318, 319, 152, 152, 152, 152, 152, 314, 314, 320, 321,
    142, 142, 322, 142, 323, 142, 142, 324,  45,  45,  45,  45,  45,  45,  45,  45,
    252, 252, 252, 252, 252, 252, 325, 252, 252, 252, 252, 252, 252, 326,  45,  45,
    327, 328,  21, 329, 330,  27,  27,  27,  27,  27,  27,  27, 331, 332,  27,  27,
     27,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27, 333,  45,  27,  27,  27,
     27, 334,  27,  27, 335,  45,  45, 336,   8, 251, 337,   0,   0, 338, 339, 340,
     27,  27,  27,  27,  27,  27,  27, 341, 342,   0,   1,   2,   1,   2, 343, 263,
    264, 344, 142, 270, 345, 346, 347, 348, 349, 350, 351, 352, 353, 353,  45,  45,
    350, 350, 350, 350, 350, 350, 350, 354, 355,   0,   0, 356,  11,  11,  11,  11,
    357, 256, 358,  45,  45,   0,   0, 359, 360, 361, 362, 362, 362, 363, 364, 256,
    365, 365, 366, 367, 368, 369, 369, 370, 371, 372, 373, 373, 374, 375,  45,  45,
    376, 376, 376, 376, 376, 377, 377, 377, 378, 379, 380, 381, 381, 382, 381, 383,
    384, 384, 385, 386, 386, 386, 387,  45, 388, 388, 388, 388, 388, 388, 388, 388,
    388, 388, 388, 389, 388, 390, 391,  45, 392, 393, 393, 394, 395, 396, 397, 397,
    398, 399, 400,  45,  45,  45, 401, 402, 403, 404, 405, 406,  45,  45,  45,  45,
    407, 407, 408, 409, 408, 410, 408, 408, 411, 412, 413, 414, 415, 416, 417, 417,
    418, 418,  45,  45, 419, 419, 420, 421, 422, 422, 422, 423, 424, 425, 426, 427,
    428, 429, 430,  45,  45,  45,  45,  45, 431, 431, 431, 431, 432,  45,  45,  45,
    433, 433, 433, 434, 433, 433, 433, 435,  45,  45,  45,  45,  45,  45,  27, 436,
    437, 437, 437, 437, 438, 439, 437, 440, 441, 441, 441, 441, 442, 443, 444, 445,
    446, 446, 446, 447, 448, 449, 449, 450, 451, 451, 451, 451, 452, 451, 453, 454,
    455, 456, 455, 457,  45,  45,  45,  45, 458, 459, 460, 461, 461, 461, 462, 463,
    464, 465, 466, 467, 468, 469, 470, 471, 472, 472, 472, 472, 472, 473,  45,  45,
    474, 474, 474, 474, 475, 476,  45,  45, 477, 477, 477, 478, 477, 479,  45,  45,
    480, 480, 480, 480, 481, 482, 483,  45, 484, 484, 484, 485, 486,  45,  45,  45,
    487, 488, 489, 487,  45,  45,  45,  45,  45,  45, 490, 490, 490, 490, 490, 491,
    492, 492, 492, 492, 493, 494, 494, 494, 495, 496, 497,  45, 498, 498, 498, 499,
    500, 501, 501, 502, 503, 501, 504, 505, 505, 506, 507, 508,  45,  45,  45,  45,
    509, 510, 510, 511, 512, 513,  45,  45, 514, 514, 514, 514, 514, 514, 514, 514,
    514, 515,  45,  45,  45,  45,  45,  45, 514, 514, 514, 514, 514, 514, 516, 517,
    514, 514, 514, 514, 518,  45,  45,  45, 519, 519, 519, 519, 519, 519, 519, 519,
    519, 519, 520,  45,  45,  45,  45,  45, 521, 521, 521, 521, 521, 521, 521, 521,
    521, 521, 521, 521, 522,  45,  45,  45, 283, 283, 283, 283, 283, 283, 283, 283,
    283, 283, 283, 523, 524, 525, 526,  45,  45,  45,  45,  45,  45, 527, 528, 529,
    530, 530, 530, 530, 531, 532, 533, 534, 530,  45,  45,  45,  45,  45,  45,  45,
    535, 535, 535, 535, 536, 535, 535, 537, 538, 535,  45,  45,  45,  45, 539,  45,
    540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 541,  45,
    540, 540, 540, 540, 540, 540, 540, 542, 543, 261, 261, 261, 261, 261, 261, 261,
    261, 261, 261, 261, 261, 261, 261, 261, 261, 544,  45,  45,  45,  45,  45, 545,
    545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 546,
    547, 547, 547, 547, 547, 547, 548, 549, 550, 551, 272,  45,  45,  45,  45,  45,
      0,   0,   0,   0,   0,   0,   0, 552,   0,   0, 553,   0,   0,   0, 554, 555,
    556,   0, 557,   0,   0,   0, 558,  45,  11,  11,  11,  11, 559,  45,  45,  45,
      0,   0,   0,   0,   0, 233,   0, 560,   0,   0,   0,   0,   0, 224,   0,   0,
      0, 561, 562, 563, 564,   0,   0,   0, 565, 566,   0, 567, 568, 569,   0,   0,
      0,   0, 237,   0,   0,   0,   0,   0,   0,   0,   0,   0, 570,   0,   0,   0,
    571, 571, 571, 571, 571, 571, 571, 571, 572, 573, 574,  45,  45,  45,  45,  45,
    575, 576, 577,  45,  45,  45,  45,  45, 578, 578, 578, 578, 578, 578, 578, 578,
    578, 578, 578, 578, 579, 580,  45,  45, 581, 581, 581, 581, 582, 583,  45,  45,
    584,  27, 585, 586, 587, 588, 589, 590, 591, 592, 593, 592,  45,  45,  45, 331,
      0,   0, 256,   0,   0,   0,   0,   0,   0, 272, 226, 342, 342, 342,   0, 552,
    594,   0, 226,   0,   0,   0, 256,   0,   0,   0, 594,  45,  45,  45, 595,   0,
    596,   0,   0, 256, 558, 560, 552,  45,   0,   0,   0,   0,   0, 597, 594, 558,
      0,   0,   0,   0,   0,   0,   0, 272,   0,   0,   0,   0,   0, 597,  45,  45,
    256,   0,   0,   0, 598, 251,   0,   0, 598,   0, 599,  45,  45,  45,  45,  45,
    256,   0,   0, 226, 594,   0, 256,  45,   0, 598,  45,  45, 600,   0, 233,  45,
    252, 252, 252, 252, 252, 601,  45,  45, 252, 252, 252, 602, 252, 252, 252, 252,
    252, 325, 252, 252, 252, 252, 252, 252, 252, 252, 603, 252, 252, 252, 252, 252,
    252, 252, 252, 252, 252, 252, 604,  45, 252, 325,  45,  45,  45,  45,  45,  45,
    605,  45,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8,   8,  45,
};

static RE_UINT16 re_script_stage_4[] = {
      0,   0,   0,   0,   1,   2,   2,   2,   2,   2,   3,   0,   0,   0,   4,   0,
      2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   5,   0,   2,   5,   6,   0,
      7,   7,   7,   7,   8,   9,  10,  11,  12,  13,  14,  15,   8,   8,   8,   8,
     16,   8,   8,   8,  17,  18,  18,  18,  19,  19,  19,  19,  19,  20,  19,  19,
     21,  22,  22,  22,  22,  22,  22,  22,  22,  23,  21,  22,  22,  22,  24,  21,
     25,  26,  26,  26,  26,  26,  26,  26,  26,  26,  12,  12,  26,  26,  27,  12,
     26,  28,  12,  12,  29,  30,  29,  31,  29,  29,  32,  33,  29,  29,  29,  29,
     31,  29,  34,   7,   7,  35,  29,  29,  36,  29,  29,  29,  29,  29,  29,  30,
     37,  37,  37,  38,  37,  37,  37,  37,  37,  37,  39,  40,  41,  41,  41,  41,
     42,  12,  12,  12,  43,  43,  43,  43,  43,  43,  44,  12,  45,  45,  45,  45,
     45,  45,  45,  46,  45,  45,  45,  47,  48,  48,  48,  48,  48,  48,  48,  49,
     37,  37,  39,  12,  12,  12,  12,  12,  29,  50,  29,  51,  12,  29,  29,  29,
     52,  29,  29,  29,  53,  53,  53,  53,  54,  53,  53,  53,  53,  55,  53,  53,
     56,  57,  56,  58,  58,  56,  56,  56,  56,  56,  59,  56,  60,  61,  62,  56,
     56,  58,  58,  63,  12,  64,  12,  65,  56,  61,  56,  56,  56,  56,  56,  62,
     66,  66,  67,  68,  69,  70,  70,  70,  70,  70,  71,  70,  71,  72,  73,  71,
     67,  68,  69,  73,  74,  12,  66,  75,  12,  76,  70,  70,  70,  73,  12,  12,
     77,  77,  78,  79,  79,  78,  78,  78,  78,  78,  80,  78,  80,  77,  81,  78,
     78,  79,  79,  81,  82,  12,  12,  12,  78,  83,  78,  78,  81,  12,  77,  78,
     84,  84,  85,  86,  86,  85,  85,  85,  85,  85,  87,  85,  87,  84,  88,  85,
     85,  86,  86,  88,  12,  89,  12,  90,  85,  89,  85,  85,  85,  85,  12,  12,
     91,  92,  93,  91,  94,  95,  96,  94,  97,  98,  93,  91,  99,  99,  95,  91,
     93,  91,  94,  95,  98,  97,  12,  12,  12,  91,  99,  99,  99,  99,  93,  12,
    100, 101, 100, 102, 102, 100, 100, 100, 100, 100, 102, 100, 100, 100, 103, 101,
    100, 102, 102, 103,  12, 104, 105,  12, 100, 106, 100, 100,  12,  12, 100, 100,
    107, 108, 107, 109, 109, 107, 107, 107, 107, 107, 109, 107, 107, 108, 110, 107,
    107, 109, 109, 110,  12, 111,  12, 112, 107, 113, 107, 107, 111,  12,  12,  12,
    114, 115, 114, 116, 116, 114, 114, 114, 114, 114, 114, 114, 114, 116, 116, 114,
     12, 114, 114, 114, 114, 117, 114, 114, 118, 119, 120, 120, 120, 121, 118, 120,
    120, 120, 120, 120, 122, 120, 120, 123, 120, 121, 124, 125, 120, 126, 120, 120,
     12, 118, 120, 120, 118, 127,  12,  12, 128, 129, 129, 129, 129, 129, 129, 129,
    129, 129, 130, 131, 129, 129, 129,  12, 132, 133, 134, 135,  12, 136, 137, 136,
    137, 138, 139, 137, 136, 136, 140, 141, 136, 134, 136, 141, 136, 136, 141, 136,
    142, 142, 142, 142, 142, 142, 143, 142, 142, 142, 142, 144, 143, 142, 142, 142,
    142, 142, 142, 145, 142, 146, 147,  12, 148, 148, 148, 148, 149, 149, 149, 149,
    149, 150,  12, 151, 149, 149, 152, 149, 153, 153, 153, 153, 154, 154, 154, 154,
    154, 154, 155, 156, 154, 157, 155, 156, 155, 156, 154, 157, 155, 156, 154, 154,
    154, 157, 154, 154, 154, 154, 157, 158, 154, 154, 154, 159, 154, 154, 156,  12,
    160, 160, 160, 160, 160, 161, 160, 161, 162, 162, 162, 162, 163, 163, 163, 163,
    163, 163, 163, 164, 165, 165, 165, 165, 165, 165, 166, 167, 165, 165, 168,  12,
    169, 169, 169, 170, 169, 171,  12,  12, 172, 172, 172, 172, 172, 173,  12,  12,
    174, 174, 174, 174, 174,  12,  12,  12, 175, 175, 175, 176, 176,  12,  12,  12,
    177, 177, 177, 177, 177, 177, 177, 178, 177, 177, 178,  12, 179, 180, 181, 182,
    181, 181, 183,  12, 181, 181, 181, 181, 181, 181,  12,  12, 181, 181, 182,  12,
    162, 184,  12,  12, 185, 185, 185, 185, 185, 185, 185, 186, 185, 185, 185,  12,
    187, 185, 185, 185, 188, 188, 188, 188, 188, 188, 188, 189, 188, 190,  12,  12,
    191, 191, 191, 191, 191, 191, 191,  12, 191, 191, 192,  12, 191, 191, 193, 194,
    195, 195, 195, 195, 195, 195, 195, 196, 197, 197, 197, 197, 197, 197, 197, 198,
    197, 197, 197, 199, 197, 197, 200,  12, 197, 197, 197, 200,   7,   7,   7, 201,
    202, 202, 202, 202, 202, 202, 202,  12, 202, 202, 202, 203, 204, 204, 204, 204,
    205, 205, 205, 205, 205,  12,  12, 205, 206, 206, 206, 206, 206, 206, 207, 206,
    206, 206, 208, 209, 210, 210, 210, 210,  19,  19, 211,  12, 204, 204,  12,  12,
    212,   7,   7,   7, 213,   7, 214, 215,   0, 214, 216,  12,   2, 217, 218,   2,
      2,   2,   2, 219, 220, 217, 221,   2,   2,   2, 222,   2,   2,   2,   2, 223,
      7,   7, 224,   7,   8, 225,   8, 225,   8,   8, 226, 226,   8,   8,   8, 225,
      8,  15,   8,   8,   8,  10,   8, 227,  10,  15,   8,  14,   0,   0,   0, 228,
      0, 229,   0,   0, 230,   0,   0, 231,   0,   0,   0, 232,   2,   2,   2, 233,
    234,  12,  12,  12,   0, 235, 236,   0,   4,   0,   0,   0,   0,   0,   0,   4,
      2,   2,   5,  12,   0, 232,  12,  12,   0,   0, 232,  12, 237, 237, 237, 237,
      0, 238,   0,   0,   0, 239,   0,   0,   0,   0, 239, 240,   0,   0, 229,   0,
    232,  12,  12,  12,  12,  12,  12,   0, 241, 241, 241, 241, 241, 241, 241, 242,
     18,  18,  18,  18,  18,  12, 243,  18, 244, 244, 244, 244, 244, 244,  12, 245,
    246,  12,  12, 245, 154, 157,  12,  12, 154, 157, 154, 157,   0,   0, 239,  12,
    247, 247, 247, 247, 247, 247, 248, 247, 247,  12,  12,  12, 247, 249,  12,  12,
      0,   0,   0,  12,   0, 250,   0,   0, 251, 247, 252, 253,   0,   0, 247,   0,
    254, 255, 255, 255, 255, 255, 255, 255, 255, 256, 257, 258, 259, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 261, 259,  12, 262, 263, 263, 263, 263, 263, 263,
    263, 263, 263, 264, 265, 153, 153, 153, 153, 153, 153, 266, 263, 263, 264,  12,
      0,  12,  12,  12, 153, 153, 153, 267, 260, 260, 260, 268, 260, 260,   0,   0,
    247, 247, 269,  12, 270, 270, 270, 270, 270, 270, 270, 271, 270, 272,  12,  12,
    273, 273, 273, 273, 274, 274, 274, 274, 274, 274, 274,  12, 275, 275, 275, 275,
    275, 275,  12,  12, 236,   2,   2,   2,   2,   2, 231,   2,   2,   2,   2, 276,
      2,   2,  12,  12,  12, 277,   2,   2, 278, 278, 278, 278, 278, 278, 278,  12,
    279, 279, 279, 279, 279, 279,  12,  12, 280, 280, 280, 280, 280, 281,  12, 282,
    280, 280, 281,  12,  53,  53,  53, 283, 284, 284, 284, 284, 284, 284, 284, 285,
    286, 286, 286, 286, 286,  12,  12, 287, 153, 153, 153, 288, 289, 289, 289, 289,
    289, 289, 289, 290, 289, 289, 291, 292, 148, 148, 148, 293, 294, 294, 294, 294,
    294, 295,  12,  12, 294, 294, 294, 296, 294, 294, 296, 294, 297, 297, 297, 297,
    298,  12,  12,  12,  12,  12, 299, 297, 300, 300, 300, 300, 300, 301,  12,  12,
    158, 157, 158, 157, 158, 157,  12,  12,   2,   2,   3,   2,   2, 302,  12,  12,
    300, 300, 300, 303, 300, 300, 303,  12, 153,  12,  12,  12, 153, 266, 304, 153,
    153, 153, 153,  12, 247, 247, 247, 249, 247, 247, 249,  12,   2, 276,  12,  12,
    305,  22,  12,  25,  26,  27,  26, 306, 307, 308,  26,  26,  51,  12,  12,  12,
    309,  29,  29,  29,  29,  29,  29, 310, 311,  29,  29,  29,  29,  29,  12,  12,
     29,  29,  29,  51,   7,   7,   7, 312, 232,   0,   0,   0,   0, 232,   0,  12,
     29,  50,  29,  29,  29,  29,  29, 313, 240,   0,   0,   0,   0, 314, 260, 260,
    260, 260, 260, 315, 316, 153, 316, 153, 316, 153, 316, 288,   0, 232,   0, 232,
     12,  12, 240, 239, 317, 317, 317, 318, 317, 317, 317, 317, 317, 319, 317, 317,
    317, 317, 319, 320, 317, 317, 317, 321, 317, 317, 319,  12, 232, 131,   0,   0,
      0, 131,   0,   0,   8,   8,   8,  14, 322,  12,  12,  12,   0,   0,   0, 323,
    324, 324, 324, 324, 324, 324, 324, 325, 326, 326, 326, 326, 327,  12,  12,  12,
    214,   0,   0,   0, 328, 328, 328, 328, 328,  12,  12, 329, 330, 330, 330, 330,
    330, 330, 331,  12, 332, 332, 332, 332, 332, 332, 333,  12, 334, 334, 334, 334,
    334, 334, 334, 335, 336, 336, 336, 336, 336,  12, 336, 336, 336, 337,  12,  12,
    338, 338, 338, 338, 339, 339, 339, 339, 340, 340, 340, 340, 340, 340, 340, 341,
    340, 340, 341,  12, 342, 342, 342, 342, 342,  12, 342, 342, 342, 342, 342,  12,
    343, 343, 343, 343, 343, 343,  12,  12, 344, 344, 344, 344, 344,  12,  12, 345,
    346, 346, 346, 346, 346, 347,  12,  12, 346, 348,  12,  12, 346, 346,  12,  12,
    349, 350, 351, 349, 349, 349, 349, 349, 349, 352, 353, 354, 355, 355, 355, 355,
    355, 356, 355, 355, 357, 357, 357, 357, 358, 358, 358, 358, 358, 358, 358, 359,
     12, 360, 358, 358, 361, 361, 361, 361, 362, 363, 364, 361, 365, 365, 365, 365,
    365, 365, 365, 366, 367, 367, 367, 367, 367, 367, 368, 369, 370, 370, 370, 370,
    371, 371, 371, 371, 371, 371,  12, 371, 372, 371, 371, 371, 373, 374,  12, 373,
    373, 375, 375, 373, 373, 373, 373, 373, 373,  12, 376, 377, 373, 373,  12,  12,
    373, 373, 378,  12, 379, 379, 379, 379, 380, 380, 380, 380, 381, 381, 381, 381,
    381, 382, 383, 381, 381, 382,  12,  12, 384, 384, 384, 384, 384, 385, 386, 384,
    387, 387, 387, 387, 387, 388, 387, 387, 389, 389, 389, 389, 390,  12, 389, 389,
    391, 391, 391, 391, 392,  12, 393, 394,  12,  12, 393, 391, 395, 395, 395, 395,
    395, 395, 396,  12, 397, 397, 397, 397, 398,  12,  12,  12, 398,  12, 399, 397,
     29,  29,  29, 400, 401, 401, 401, 401, 401, 401, 401, 402, 403, 401, 401, 401,
     12,  12,  12, 404, 405, 405, 405, 405, 406,  12,  12,  12, 407, 407, 407, 407,
    407, 407, 408,  12, 407, 407, 409,  12, 410, 410, 410, 410, 410, 411, 410, 410,
    410,  12,  12,  12, 412, 412, 412, 412, 412, 413,  12,  12, 414, 414, 414, 414,
    414, 414, 414, 415, 119, 120, 120, 120, 120, 127,  12,  12, 416, 416, 416, 416,
    417, 416, 416, 416, 416, 416, 416, 418, 419, 420, 421, 422, 419, 419, 419, 422,
    419, 419, 423,  12, 424, 424, 424, 424, 424, 424, 425,  12, 424, 424, 426,  12,
    427, 428, 427, 429, 429, 427, 427, 427, 427, 427, 430, 427, 430, 428, 431, 427,
    427, 429, 429, 431, 432, 433,  12, 428, 427, 434, 427, 432, 427, 432,  12,  12,
    435, 435, 435, 435, 435, 435, 436, 437, 438, 438, 438, 438, 438, 438,  12,  12,
    438, 438, 439,  12, 440, 440, 440, 440, 440, 441, 440, 440, 440, 440, 440, 441,
    442, 442, 442, 442, 442, 443,  12,  12, 442, 442, 444,  12, 181, 181, 181, 445,
    446, 446, 446, 446, 446, 446,  12,  12, 446, 446, 447,  12, 448, 448, 448, 448,
    448, 448, 449, 450, 448, 448, 448,  12, 451, 451, 451, 451, 452,  12,  12, 453,
    454, 454, 454, 454, 454, 454,  12,  12, 455, 455, 455, 455, 455, 456, 455, 455,
    455, 455, 455, 457, 458,  12,  12,  12, 459, 459, 459, 459, 459, 459, 460,  12,
    461, 461, 462, 461, 461, 461, 461, 461, 461, 463, 461, 461, 461, 464,  12,  12,
    461, 461, 461, 465, 466, 466, 466, 466, 467, 466, 466, 466, 466, 466, 468, 466,
    466, 469,  12,  12, 470, 471, 472, 470, 470, 470, 470, 470, 470, 471, 473, 472,
    470, 470,  12,  12, 470, 470, 474,  12, 475, 475, 475, 475, 475, 475, 476,  12,
    475, 475, 475, 477, 475, 478,  12,  12, 475,  12,  12,  12, 479, 479, 479, 479,
    479, 479, 479, 480, 481, 481, 481, 481, 481, 482,  12,  12, 275, 275, 483,  12,
    484, 484, 484, 484, 484, 484, 484, 485, 484, 484, 486, 487, 488, 488, 488, 488,
    488, 488, 488, 489, 488, 489,  12,  12, 490, 490, 490, 490, 490, 491,  12,  12,
    490, 490, 492, 490, 492, 490, 490, 490, 490, 490,  12, 493, 494, 494, 494, 494,
    494, 495,  12,  12, 494, 494, 494, 496,  12,  12,  12, 497, 498,  12,  12,  12,
    499, 499, 499, 499, 499, 499, 499, 500, 501,  12,  12,  12, 502, 255, 255, 255,
    255, 255, 255, 256, 503, 503, 503, 503, 503, 503, 503,  12, 504, 504, 504, 504,
    504, 504, 505,  12, 504, 504, 504, 506, 504, 504, 506,  12, 504, 504, 507, 504,
      0, 239,  12,  12,   0, 232, 240,   0,   0, 508, 228,   0,   0,   0, 508,   7,
    212, 509,   7,   0,   0,   0, 510, 228,   0,   0, 511,  12,   8, 225,  12,  12,
    239,  12,  12,  12,   0,   0,   0, 229, 512, 513, 240, 229,   0,   0, 514, 240,
      0, 240,   0,   0,   0, 514, 232, 240,   0, 229,   0, 229,   0,   0, 514, 232,
      0, 515, 238,   0, 229,   0,   0,   0,   0,   0,   0, 238, 516, 516, 516, 516,
    516, 516, 516,  12,  12,  12, 517, 516, 518, 516, 516, 516, 241, 242, 241, 241,
    241, 241, 519, 241, 520, 521, 242,  12, 522, 522, 522, 522, 522, 523, 522, 522,
    522, 524,  12,  12, 525, 525, 525, 525, 525, 525, 526,  12, 525, 525, 527, 528,
     29, 529,  29,  29, 530, 531, 529,  29, 400,  29, 532,  12, 533, 309, 532, 529,
    530, 531, 532, 532, 530, 531, 400,  29, 400,  29, 529, 534,  29,  29, 535,  29,
     29,  29,  29,  12, 529, 529, 535,  29,   0,   0,   0, 511,  12, 238,   0,   0,
    536,  12,  12,  12,   0, 511,  12,  12,   0,   0,  12,  12,   0,   0,   0, 239,
    511,  12,  12,  12, 247, 269,  12,  12, 247, 537,  12,  12, 249,  12,  12,  12,
    537,  12,  12,  12, 538,  12,  12,  12,
};

static RE_UINT8 re_script_stage_5[] = {
      1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   1,
      1,   1,   2,   1,   2,   1,   1,   1,   1,   1,  35,  35,  41,  41,  41,  41,
      3,   3,   3,   3,   1,   3,   3,   3,   0,   0,   3,   3,   3,   3,   1,   3,
      0,   0,   0,   0,   3,   1,   3,   1,   3,   3,   3,   0,   3,   0,   3,   3,
      3,   3,   0,   3,   3,   3,  55,  55,  55,  55,  55,  55,   4,   4,   4,   4,
      4,  41,  41,   4,   0,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   0,
      0,   1,   5,   0,   0,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   0,
      6,   0,   0,   0,   7,   7,   7,   7,   7,   1,   7,   7,   1,   7,   7,   7,
      7,   7,   7,   1,   7,   0,   7,   1,   7,   7,   7,  41,  41,  41,   7,   7,
     41,   7,   7,   7,   8,   8,   8,   8,   8,   8,   0,   8,   8,   8,   8,   0,
      0,   8,   8,   8,   9,   9,   9,   9,   9,   9,   0,   0,  66,  66,  66,  66,
     66,  66,  66,   0,  82,  82,  82,  82,  82,  82,   0,   0,  82,  82,  82,   0,
     95,  95,  95,  95,   0,   0,  95,   0,   7,   0,   7,   7,   7,   7,   0,   0,
      7,   7,   1,   7,  10,  10,  10,  10,  10,  41,  41,  10,   1,   1,  10,  10,
     11,  11,  11,  11,   0,  11,  11,  11,  11,   0,   0,  11,  11,   0,  11,  11,
     11,   0,  11,   0,   0,   0,  11,  11,  11,  11,   0,   0,  11,  11,  11,   0,
      0,   0,   0,  11,  11,  11,   0,  11,   0,  12,  12,  12,  12,  12,  12,   0,
      0,   0,   0,  12,  12,   0,   0,  12,  12,  12,  12,  12,  12,   0,  12,  12,
      0,  12,  12,   0,  12,  12,   0,   0,   0,  12,   0,   0,  12,   0,  12,   0,
      0,   0,  12,  12,   0,  13,  13,  13,  13,  13,  13,  13,  13,  13,   0,  13,
     13,   0,  13,  13,  13,  13,   0,   0,  13,   0,   0,   0,   0,   0,  13,  13,
      0,  14,  14,  14,  14,  14,  14,  14,  14,   0,   0,  14,  14,   0,  14,  14,
     14,  14,   0,   0,   0,   0,  14,  14,  14,  14,   0,  14,   0,   0,  15,  15,
      0,  15,  15,  15,  15,  15,  15,   0,  15,   0,  15,  15,  15,  15,   0,   0,
      0,  15,  15,   0,   0,   0,   0,  15,  15,   0,   0,   0,  15,  15,  15,  15,
     16,  16,  16,  16,   0,  16,  16,  16,  16,   0,  16,  16,  16,  16,   0,   0,
      0,  16,  16,   0,  16,  16,  16,   0,   0,   0,  16,  16,  17,  17,  17,  17,
      0,  17,  17,  17,  17,   0,  17,  17,  17,  17,   0,   0,   0,  17,  17,   0,
      0,   0,  17,   0,   0,   0,  17,  17,  18,  18,  18,  18,   0,  18,  18,  18,
     18,   0,  18,  18,   0,   0,  18,  18,   0,   0,  19,  19,   0,  19,  19,  19,
     19,  19,  19,  19,  19,  19,  19,   0,  19,  19,   0,  19,   0,  19,   0,   0,
      0,   0,  19,   0,   0,   0,   0,  19,  19,   0,  19,   0,  19,   0,   0,   0,
      0,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,   0,   0,   0,   0,   1,
      0,  21,  21,   0,  21,   0,   0,  21,  21,   0,  21,   0,   0,  21,   0,   0,
     21,  21,  21,  21,   0,  21,  21,  21,   0,  21,   0,  21,   0,   0,  21,  21,
     21,  21,   0,  21,  21,  21,   0,   0,  22,  22,  22,  22,   0,  22,  22,  22,
     22,   0,   0,   0,  22,   0,  22,  22,  22,   1,   1,   1,   1,  22,  22,   0,
     23,  23,  23,  23,  24,  24,  24,  24,  24,  24,   0,  24,   0,  24,   0,   0,
     24,  24,  24,   1,  25,  25,  25,  25,  26,  26,  26,  26,  26,   0,  26,  26,
     26,  26,   0,   0,  26,  26,  26,   0,   0,  26,  26,  26,  26,   0,   0,   0,
     27,  27,  27,  27,  27,  27,   0,   0,  28,  28,  28,  28,  29,  29,  29,  29,
     29,   0,   0,   0,  30,  30,  30,  30,  30,  30,  30,   1,   1,   1,  30,  30,
     30,   0,   0,   0,  42,  42,  42,  42,  42,   0,  42,  42,  42,   0,   0,   0,
     43,  43,  43,  43,  43,   1,   1,   0,  44,  44,  44,  44,  45,  45,  45,  45,
     45,   0,  45,  45,  31,  31,  31,  31,  31,  31,   0,   0,  32,  32,   1,   1,
     32,   1,  32,  32,  32,  32,  32,  32,  32,  32,  32,   0,  32,  32,   0,   0,
     28,  28,   0,   0,  46,  46,  46,  46,  46,  46,  46,   0,  46,   0,   0,   0,
     47,  47,  47,  47,  47,  47,   0,   0,  47,   0,   0,   0,  56,  56,  56,  56,
     56,  56,   0,   0,  56,  56,  56,   0,   0,   0,  56,  56,  54,  54,  54,  54,
      0,   0,  54,  54,  78,  78,  78,  78,  78,  78,  78,   0,  78,   0,   0,  78,
     78,  78,   0,   0,  41,  41,  41,   0,  62,  62,  62,  62,  62,   0,   0,   0,
     67,  67,  67,  67,  93,  93,  93,  93,  68,  68,  68,  68,   0,   0,   0,  68,
     68,  68,   0,   0,   0,  68,  68,  68,  69,  69,  69,  69,   4,   0,   0,   0,
     41,  41,  41,   1,  41,   1,  41,  41,  41,   1,   1,   1,   1,  41,   1,   1,
     41,  41,   0,   0,   2,   2,   3,   3,   3,   3,   3,   4,   2,   3,   3,   3,
      3,   3,   2,   2,   3,   3,   3,   2,   4,   2,   2,   2,   2,   2,   2,   3,
     41,  41,   0,  41,   3,   3,   0,   0,   0,   3,   0,   3,   0,   3,   3,   3,
     41,  41,   1,   1,   1,   0,   1,   1,   1,   2,   0,   0,   1,   1,   1,   2,
      1,   1,   1,   0,   2,   0,   0,   0,  41,   0,   0,   0,   1,   1,   3,   1,
      1,   1,   2,   2,  53,  53,  53,  53,   0,   0,   1,   1,   1,   1,   0,   0,
      0,   1,   1,   1,  57,  57,  57,  57,  57,  57,  57,   0,   0,  55,  55,  55,
     58,  58,  58,  58,   0,   0,   0,  58,  58,   0,   0,   0,  36,  36,  36,  36,
     36,  36,   0,  36,  36,  36,   0,   0,   1,  36,   1,  36,   1,  36,  36,  36,
     36,  36,  41,  41,  41,  41,  25,  25,   0,  33,  33,  33,  33,  33,  33,  33,
     33,  33,  33,   0,   0,  41,  41,   1,   1,  33,  33,  33,   1,  34,  34,  34,
     34,  34,  34,  34,  34,  34,  34,   1,   0,  35,  35,  35,  35,  35,  35,  35,
     35,  35,  35,   0,   0,  25,  25,  25,  25,  25,  25,   0,  25,  25,  25,   1,
     34,  34,  34,   0,  36,  36,  36,   0,  37,  37,  37,  37,  37,   0,   0,   0,
     37,  37,  37,   0,  83,  83,  83,  83,  70,  70,  70,  70,  84,  84,  84,  84,
      2,   2,   2,   0,   0,   0,   0,   2,  59,  59,  59,  59,  65,  65,  65,  65,
     71,  71,  71,  71,  71,  71,   0,   0,   0,   0,  71,  71,  10,  10,   0,   0,
     72,  72,  72,  72,  72,  72,   1,  72,  73,  73,  73,  73,   0,   0,   0,  73,
     25,   0,   0,   0,  85,  85,  85,  85,  85,  85,   0,   1,  85,  85,   0,   0,
      0,   0,  85,  85,  23,  23,  23,   0,  77,  77,  77,  77,  77,  77,  77,   0,
     77,  77,   0,   0,  79,  79,  79,  79,  79,  79,  79,   0,   0,   0,   0,  79,
     86,  86,  86,  86,  86,  86,  86,   0,   2,   3,   0,   0,  86,  86,   0,   0,
      0,   0,   0,  25,   0,   0,   0,   5,   6,   0,   6,   0,   6,   6,   0,   6,
      6,   0,   6,   6,   0,   0,   0,   7,   7,   7,   1,   1,   0,   0,   7,   7,
     41,  41,   4,   4,   7,   0,   0,   1,   1,   1,  34,  34,  34,  34,   1,   1,
      0,   0,  25,  25,  48,  48,  48,  48,   0,  48,  48,  48,  48,  48,  48,   0,
     48,  48,   0,  48,  48,  48,   0,   0,   3,   0,   0,   0,   1,  41,   0,   0,
     74,  74,  74,  74,  74,   0,   0,   0,  75,  75,  75,  75,  75,   0,   0,   0,
     38,  38,  38,  38,   0,  38,  38,  38,  39,  39,  39,  39,  39,  39,  39,   0,
    120, 120, 120, 120, 120, 120, 120,   0,  49,  49,  49,  49,  49,  49,   0,  49,
     60,  60,  60,  60,  60,  60,   0,   0,  40,  40,  40,  40,  50,  50,  50,  50,
     51,  51,  51,  51,  51,  51,   0,   0, 136, 136, 136, 136, 106, 106, 106, 106,
    103, 103, 103, 103,   0,   0,   0, 103, 110, 110, 110, 110, 110, 110, 110,   0,
    110, 110,   0,   0,  52,  52,  52,  52,  52,  52,   0,   0,  52,   0,  52,  52,
     52,  52,   0,  52,  52,   0,   0,   0,  52,   0,   0,  52,  87,  87,  87,  87,
     87,  87,   0,  87, 118, 118, 118, 118, 117, 117, 117, 117, 117, 117, 117,   0,
      0,   0,   0, 117, 128, 128, 128, 128, 128, 128, 128,   0, 128, 128,   0,   0,
      0,   0,   0, 128,  64,  64,  64,  64,   0,   0,   0,  64,  76,  76,  76,  76,
     76,  76,   0,   0,   0,   0,   0,  76,  98,  98,  98,  98,  97,  97,  97,  97,
      0,   0,  97,  97,  61,  61,  61,  61,   0,  61,  61,   0,   0,  61,  61,  61,
     61,  61,  61,   0,   0,   0,   0,  61,  61,   0,   0,   0,  88,  88,  88,  88,
    116, 116, 116, 116, 112, 112, 112, 112, 112, 112, 112,   0,   0,   0,   0, 112,
     80,  80,  80,  80,  80,  80,   0,   0,   0,  80,  80,  80,  89,  89,  89,  89,
     89,  89,   0,   0,  90,  90,  90,  90,  90,  90,  90,   0, 121, 121, 121, 121,
    121, 121,   0,   0,   0, 121, 121, 121, 121,   0,   0,   0,  91,  91,  91,  91,
     91,   0,   0,   0, 130, 130, 130, 130, 130, 130, 130,   0,   0,   0, 130, 130,
      7,   7,   7,   0,  94,  94,  94,  94,  94,  94,   0,   0,   0,   0,  94,  94,
      0,   0,   0,  94,  92,  92,  92,  92,  92,  92,   0,   0, 101, 101, 101, 101,
    101,   0,   0,   0, 101, 101,   0,   0,  96,  96,  96,  96,  96,   0,  96,  96,
    111, 111, 111, 111, 111, 111, 111,   0, 100, 100, 100, 100, 100, 100,   0,   0,
    109, 109, 109, 109, 109, 109,   0, 109, 109, 109, 109,   0, 129, 129, 129, 129,
    129, 129, 129,   0, 129,   0, 129, 129, 129, 129,   0, 129, 129, 129,   0,   0,
    123, 123, 123, 123, 123, 123, 123,   0, 123, 123,   0,   0, 107, 107, 107, 107,
      0, 107, 107, 107, 107,   0,   0, 107, 107,   0, 107, 107, 107, 107,   0,   0,
    107,   0,   0,   0,   0,   0,   0, 107,   0,   0, 107, 107, 135, 135, 135, 135,
    135, 135,   0, 135,   0, 135,   0,   0, 124, 124, 124, 124, 124, 124,   0,   0,
    122, 122, 122, 122, 122, 122,   0,   0, 114, 114, 114, 114, 114,   0,   0,   0,
    114, 114,   0,   0,  32,   0,   0,   0, 102, 102, 102, 102, 102, 102,   0,   0,
    126, 126, 126, 126, 126, 126,   0,   0,   0, 126, 126, 126, 125, 125, 125, 125,
    125, 125, 125,   0,   0,   0,   0, 125, 141, 141, 141, 141, 140, 140, 140, 140,
      0,   0, 140, 140, 140,   0, 140, 140, 140, 140, 140,   0, 119, 119, 119, 119,
    119,   0,   0,   0, 133, 133, 133, 133, 133,   0, 133, 133, 133, 133, 133,   0,
    133, 133,   0,   0, 133,   0,   0,   0, 134, 134, 134, 134,   0,   0, 134, 134,
      0, 134, 134, 134, 134, 134, 134,   0, 138, 138, 138, 138, 138, 138, 138,   0,
    138, 138,   0, 138,   0,   0, 138,   0, 138, 138,   0,   0,  63,  63,  63,  63,
     63,  63,   0,   0,  63,  63,  63,   0,  63,   0,   0,   0,  81,  81,  81,  81,
     81,  81,  81,   0, 127, 127, 127, 127, 127, 127, 127,   0,  84,   0,   0,   0,
    115, 115, 115, 115, 115, 115, 115,   0, 115, 115,   0,   0,   0,   0, 115, 115,
    104, 104, 104, 104, 104, 104,   0,   0, 108, 108, 108, 108, 108, 108,   0,   0,
    108, 108,   0, 108,   0, 108, 108, 108,  99,  99,  99,  99,  99,   0,   0,   0,
     99,  99,  99,   0,   0,   0,   0,  99, 137, 139,   0,   0, 137, 137, 137, 137,
    137,   0,   0,   0, 137, 137, 137,   0,  34,  33,  33,  33, 139, 139, 139, 139,
    105, 105, 105, 105, 105, 105, 105,   0, 105,   0,   0,   0, 105, 105,   0,   0,
      1,   1,   1,  41,   1,  41,  41,  41,   1,   1,  41,  41,   1,   0,   0,   0,
      0,   0,   1,   0,   0,   1,   1,   0,   1,   1,   0,   1,   1,   0,   1,   0,
    131, 131, 131, 131,   0,   0,   0, 131,   0, 131, 131, 131,  57,   0,   0,  57,
     57,  57,   0,  57,  57,   0,  57,  57, 113, 113, 113, 113, 113,   0,   0, 113,
    113, 113, 113,   0, 132, 132, 132, 132, 132, 132, 132,   0, 132, 132,   0,   0,
      0,   0, 132, 132,   0,   7,   7,   7,   0,   7,   7,   0,   7,   0,   0,   7,
      0,   7,   0,   7,   0,   0,   7,   0,   7,   0,   7,   0,   7,   7,   0,   7,
     33,   1,   1,   0,  36,   0,   0,   0,   0,   1,   0,   0,
};

/* Script: 11660 bytes. */

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
    pos = (RE_UINT32)re_script_stage_3[pos + f] << 2;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_script_stage_4[pos + f] << 2;
    value = re_script_stage_5[pos + code];

    return value;
}

/* Word_Break. */

static RE_UINT8 re_word_break_stage_1[] = {
     0,  1,  2,  2,  2,  3,  4,  5,  6,  7,  8,  9,  2, 10, 11, 12,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    13,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,
};

static RE_UINT8 re_word_break_stage_2[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,   1,  17,  18,  19,   1,  20,  21,  22,  23,  24,  25,  26,  27,   1,  28,
     29,  30,  31,  31,  32,  31,  33,  34,  31,  31,  31,  31,  35,  36,  37,  31,
     38,  39,  40,  41,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
      1,   1,   1,   1,  42,   1,  43,  44,  45,  46,  47,  48,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  49,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  50,   1,  51,  52,  53,
     54,  55,  56,  57,  58,  59,   1,  60,  61,  62,  63,  64,  65,  31,  31,  31,
     66,  67,  68,  69,  70,  71,  72,  73,  74,  31,  75,  31,  76,  77,  31,  31,
      1,   1,   1,  78,  79,  80,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
      1,   1,   1,   1,  81,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  31,  31,  31,   1,   1,  82,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  31,  31,  31,  31,   1,   1,  83,  84,  31,  31,  31,  85,
     31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     86,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  87,  31,  31,  31,
     31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  88,  89,  31,  90,  91,  92,  93,  31,  31,  94,  31,  31,  31,  31,  31,
     95,  31,  31,  31,  31,  31,  31,  31,  96,  97,  31,  31,  31,  31,  98,  31,
     31,  99,  31, 100, 101, 102, 103,  31,  31, 104,  31,  31,  31,  31,  31,  31,
    105, 106,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

static RE_UINT16 re_word_break_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   6,   7,   7,   7,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   8,   9,  10,  10,  10,  11,  12,  13,   7,  14,
      7,   7,   7,   7,  15,   7,   7,   7,   7,  16,  17,  18,  19,  20,  21,  22,
     23,   7,  24,  25,   7,   7,  26,  27,  28,  29,  30,   7,   7,  31,  32,  33,
     34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
     50,  51,  52,  53,  54,  55,  56,  57,  58,  55,  59,  60,  61,  62,  63,  64,
     65,  66,  67,  68,  69,  70,  71,  72,  65,  73,  74,  75,  76,  77,  78,  79,
     38,  80,  81,  38,  38,  82,  83,  38,  84,  85,  86,  87,  88,  89,  90,  38,
     38,  91,  92,  93,  94,   7,  95,  96,   7,   7,  97,   7,  98,  99, 100,   7,
    101,   7, 102,  38, 103,   7,   7, 104,  18,   7,   7,   7,   7,   7,   7,   7,
      7,   7,   7, 105,   3,   7,   7, 106, 107, 108, 109, 110,  38,  40, 111, 112,
    113,   7,   7, 114, 115, 116,   7, 117, 118, 119,  64,  38,  38,  38, 120,  38,
    121,  38, 122, 123, 124, 125,  38,  38, 126, 127, 128, 129, 130, 131,   7, 132,
      7, 133, 134, 135, 136,  38, 137, 138,   7,   7,   7,   7,   7,   7,  10, 139,
    104,   7, 140, 135,   7, 141, 142, 143, 144, 145, 146, 147, 148,  38, 149, 150,
    151, 152, 153,   7, 136,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,
     38,  38,  38,  38,  38, 154,   7, 155, 156,  38, 157,  38, 158,  38,  38, 159,
    160,  38,  38, 161,  38,  38,  38,  38,   7, 162, 118,   7,   7,   7,   7, 163,
      7,  95,   7, 164, 165, 166, 166,  10,  38, 167,  38,  38,  38,  38,  38,  38,
    168, 169,  38,  38, 170, 171, 171, 172, 173, 174,   7,   7, 175, 176,  38, 177,
     38,  38,  38,  38,  38,  38, 177, 178, 171, 171, 179,  38,  38,  38,  38,  38,
      7,   7,   7,   7, 180,  38, 181, 135, 182, 183,   7, 184, 185,   7,   7, 186,
    187,   7,   7,   7,   7, 188,  38, 187, 189, 190,   7, 191, 192, 127, 193, 194,
     32, 195, 196, 197,  42, 198, 199, 200,   7, 201, 202, 203,  38, 204, 205, 206,
    207, 208,   7, 209,   7,   7,   7, 210,   7,   7,   7,   7,   7, 211, 212, 213,
    214, 215, 216,   7,   7, 217, 218,   7,   7, 135, 181,   7, 219,   7, 220, 221,
    222, 223, 224, 225,   7,   7,   7, 226, 227,   2,   3, 228, 229, 118, 230, 231,
    232, 233, 234,  38,   7,   7,   7, 176,  38,  38,   7, 235,  38,  38,  38, 236,
     38,  38,  38,  38, 197,   7, 237, 238,   7, 239, 240, 241, 135,   7, 242,  38,
      7,   7,   7,   7, 135, 243, 244, 213,   7, 245,   7, 246,  38,  38,  38,  38,
      7, 165, 117, 220,  38,  38,  38,  38, 247, 248, 117, 165, 118,  38,  38, 249,
    117, 250,  38,  38,   7,   8,  38,  38, 251, 252,  38, 197, 197,  38,  86, 253,
      7, 117, 117, 254, 217,  38,  38,  38,   7,   7, 136,  38,   7, 254,   7, 254,
    130, 255, 256, 257, 130, 258, 181, 259, 130, 260, 181, 261, 130, 198, 262,  38,
    263, 264,  38,  38, 265, 266, 267, 268, 269,  55, 270, 271,  38,  38,  38,  38,
      7, 272, 273,  38,   7,  29, 274,  38,  38,  38,  38,  38,   7, 275, 276,  38,
      7,  29, 277,  38,   7, 278, 112,  38, 279, 280,  38,  38,  38,  38,  38,  38,
     38,  38,  38,  38,  38,   7,   7, 281, 282, 283, 284,   7, 285,  38,   7, 286,
    287, 288, 289, 290, 291, 292,  38,  38, 293, 294, 295,  38,  38,  38,  38,  38,
      7,   7,   7,   7, 250,  38,  38,  38,   7,   7,   7, 175,   7,   7,   7,   7,
      7,   7, 246,  38,  38,  38,  38,  38,   7, 175,  38,  38,  38,  38,  38,  38,
      7,   7, 296,  38,  38,  38,  38,  38,   7, 286, 118, 112,  38,  38, 181, 297,
      7, 298, 299, 300, 103,  38,  38,  38,   7,   7, 301, 302, 303,  38,  38, 304,
    305,  38,  38,  38,  38,  38,  38,  38,   7,   7,   7, 306, 307, 308,  38,  38,
     38,  38,  38, 309, 310, 311,  38,  38,  38,  38, 312,  38,  38,  38,  38,  38,
      7,   7, 313,   7, 314, 315, 316,   7, 317, 318, 319,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7, 320, 321,  96, 313, 313, 162, 162, 287, 287, 322, 323,
     10, 324,  10, 325, 326, 327,  38,  38, 328, 329,  38,  38,  38,  38,  38,  38,
      7,   7,   7,   7,   7,   7, 330,  38,   7,   7, 331,  38,  38,  38,  38,  38,
    316, 332, 333, 334, 335, 336,  38,  38,  38, 181, 337, 337, 155,  38,  38, 338,
    339, 340,  38, 341, 342, 343, 344, 345,  38,  38, 346, 347, 348, 349,  38,  38,
     38, 350,  38, 351, 352,  38,  38, 339,  38,  38, 353,  38, 354, 355, 356,  38,
    357, 358,  38,  38,  38,  38, 359,  38, 360,  10,  10,  10,  38,  38,  38,  38,
     10,  10,  10,  10,  10,  10,  10, 361,
};

static RE_UINT8 re_word_break_stage_4[] = {
      0,   0,   1,   2,   0,   0,   0,   0,   3,   4,   0,   5,   6,   6,   7,   0,
      8,   9,   9,   9,   9,   9,  10,  11,   8,   9,   9,   9,   9,   9,  10,   0,
      0,  12,   0,   0,   0,   0,   0,   0,   0,   0,  13,  14,   0,  15,  13,   0,
      9,   9,   9,   9,   9,  10,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,
      9,   9,   9,   9,   9,   9,   0,  16,   9,  17,   0,   9,   9,   9,   9,   9,
     18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,   9,  19,  16,  20,
      0,  21,  10,  19,   9,   9,   9,   9,  22,   9,   9,   9,   9,   9,   9,   9,
      9,   9,   9,   9,   9,  22,   9,   9,  23,  18,  24,   9,   9,   9,   9,   9,
      9,   9,   9,   9,   8,   9,   9,   9,   9,   9,   9,   9,   9,  10,  25,   0,
      8,   9,   9,   9,   9,   9,   9,   9,   9,   9,  26,   0,  27,  18,  18,  18,
     18,  18,  18,  18,  18,  18,  18,  28,  29,  28,   0,   0,  30,  30,  30,  30,
     30,  30,  31,   0,  32,  33,   0,   0,  34,  35,   0,  36,  18,  18,  37,  38,
      9,   9,  39,  18,  18,  18,  18,  18,   6,   6,  40,  41,  42,   9,   9,   9,
      9,   9,   9,   9,   9,  43,  18,  44,  18,  45,  46,  24,   6,   6,  47,  48,
      0,   0,   0,  49,  50,   9,   9,   9,   9,   9,   9,   9,  18,  18,  18,  18,
     18,  18,  37,   8,   9,   9,   9,   9,   9,  51,  18,  18,  52,   0,   0,   0,
      6,   6,  47,   9,   9,   9,   9,   9,   9,   9,  39,  18,  18,  53,  54,   0,
      9,   9,   9,   9,   9,  51,  55,  18,  18,  56,  56,  57,   0,   0,   0,   0,
      9,   9,   9,   9,   9,   9,  56,   0,   9,   9,  10,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   9,   9,   9,   9,   9,  19,   9,  53,
      0,   0,   0,   0,   0,  18,  18,  18,  58,  18,  18,  18,  18,  18,  18,  18,
     18,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,  51,  59,
     18,  18,  18,  18,  56,  18,   9,   9,  51,  60,   6,   6,   8,   9,   9,   9,
     56,   8,   9,  48,  48,   9,   9,   9,   9,   9,  19,   9,  61,  16,  53,  59,
     18,  62,  62,  63,   0,  64,   0,  22,  51,  60,   6,   6,  53,   0,   0,  17,
     27,   8,  10,  65,  48,   9,   9,   9,   9,   9,  19,   9,  19,  66,  53,  46,
     37,  64,  62,  57,  67,   0,   8,  61,   0,  60,   6,   6,  24,  68,   0,   0,
     27,   8,   9,  22,  22,   9,   9,   9,   9,   9,  19,   9,  19,   8,  53,  59,
     18,  28,  28,  57,  17,   0,   0,   0,  51,  60,   6,   6,   0,   0,  43,  18,
     27,   8,   9,  48,  48,   9,   9,   9,  18,  62,  62,  57,   0,  69,   0,  22,
     51,  60,   6,   6,  25,   0,   0,   0,  70,   8,  10,  16,  19,  53,  66,  19,
     65,  17,  10,  16,   9,   9,  53,  69,  37,  69,  46,  57,  17,  64,   0,   0,
      0,  60,   6,   6,   0,   0,   0,   0,  18,   8,   9,  19,  19,   9,   9,   9,
      9,   9,  19,   9,   9,   9,  53,  43,  18,  46,  46,  57,   0,  29,  10,   0,
     51,  60,   6,   6,   0,   0,   0,   0,  56,   8,   9,  19,  19,   9,   9,   9,
      9,   9,  19,   9,   9,   8,  53,  59,  18,  46,  46,  57,   0,  29,   0,  13,
     51,  60,   6,   6,  66,   0,   0,   0,   9,   9,   9,   9,   9,   9,  39,  59,
     18,  46,  46,  63,   0,  39,   0,  65,  51,  60,   6,   6,   0,   0,  16,   9,
     69,   8,   9,   9,   9,  10,  16,   9,   9,   9,   9,   9,  22,   9,   9,  25,
      9,  10,  71,  64,  18,  72,  18,  18,   0,  60,   6,   6,  69,   0,   0,   0,
      0,   0,   0,   0,  67,  18,  37,   0,   0,  64,  18,  37,   6,   6,  73,   0,
      0,   0,   0,   0,  67,  18,  28,  74,   0,   0,  18,  57,   6,   6,  73,   0,
     17,   0,   0,   0,   0,   0,  57,   0,   6,   6,  73,   0,   0,  75,  67,  69,
      9,   9,   8,   9,   9,   9,   9,   9,   9,   9,   9,  17,  27,  18,  18,  18,
     18,  46,   9,  56,  18,  18,  27,  18,  18,  18,  18,  18,  18,  18,  18,  74,
      0,  71,   0,   0,   0,   0,   0,   0,   0,   0,  64,  18,  18,  18,  18,  37,
      6,   6,  73,   0,   0,  69,  57,  69,  46,  62,  18,  57,  27,  74,   0,   0,
     69,  18,  18,  28,   6,   6,  76,  57,   9,  22,   0,  25,   9,   9,   9,   9,
      9,   9,   9,   9,   9,   9,  10,   9,   9,   9,  19,  53,   9,  10,  19,  53,
      9,   9,  19,  53,   9,   9,   9,   9,   9,   9,   9,   9,  19,  53,   9,  10,
     19,  53,   9,   9,   9,  10,   9,   9,   9,   9,   9,   9,  19,  53,   9,   9,
      9,   9,   9,   9,   9,   9,  10,  27,   9,   9,   9,   9,   0,   0,   0,   0,
      9,   9,   9,   9,   9,  53,   9,  53,   9,   9,   9,  48,   9,   9,   9,   9,
      9,   9,  10,  16,   9,   9,  17,   0,   9,   9,   9,  19,  51,  74,   0,   0,
      9,   9,   9,   9,  51,  74,   0,   0,   9,   9,   9,   9,  51,   0,   0,   0,
      9,   9,   9,  19,  77,   0,   0,   0,  18,  18,  18,  18,  18,   0,   0,  67,
      6,   6,  73,   0,   0,   0,   0,   0,   0,   0,  64,  78,   6,   6,  73,   0,
      9,   9,   9,   9,   9,   9,   0,   0,   9,  79,   9,   9,   9,   9,   9,   9,
      9,   9,  80,   0,   9,   9,   9,   9,   9,   9,   9,   9,   9,  53,   0,   0,
      9,   9,   9,   9,   9,   9,   9,  10,  18,  18,  18,   0,  18,  18,  18,   0,
      0,   0,   0,   0,   6,   6,  73,   0,   9,   9,   9,   9,   9,  39,  18,   0,
      0,   0,   0,   0,   0,  27,  18,  37,  18,  18,  18,  18,  18,  18,  18,  62,
      6,   6,  73,   0,   6,   6,  73,   0,   0,   0,   0,   0,  18,  18,  18,  37,
     18,  42,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,  18,  18,  18,
     18,  42,   9,   0,   6,   6,  73,   0,   0,   0,  64,  18,  18,   0,   0,   0,
     81,   9,   9,   9,   9,   9,   9,   9,  56,  18,  18,  24,   6,   6,  47,   9,
      9,  51,  18,  18,  18,   0,   0,   0,   9,  18,  18,  18,  18,  18,   0,   0,
      6,   6,  73,   8,   6,   6,  47,   9,   9,   9,   9,   9,   9,   9,   9,  53,
      9,   9,  17,   0,   0,   0,   0,   0,   0,   0,   0,   0,  37,  18,  18,  18,
     18,  18,  42,  50,  51,  45,  57,   0,  18,  18,  18,  18,  18,  18,  28,  18,
      9,  53,   9,  53,   9,   9,  82,  82,   9,   9,   9,   9,   9,  19,   9,  61,
     16,  19,   9,  17,   9,  16,   9,   0,   9,   9,   9,  17,  16,  19,   9,  17,
      0,   0,   0,  83,   0,   0,  84,   0,   0,  85,  86,  87,   0,   0,   0,  11,
     88,  89,   0,   0,   0,  88,   0,   0,  34,  90,  34,  34,  25,   0,   0,  65,
      0,   0,   0,   0,   9,   9,   9,  17,   0,   0,   0,   0,  18,  18,  18,  18,
     18,  18,  18,  18,  74,   0,   0,   0,  13,  65,  16,   9,   9,  25,   8,  53,
      0,  61,  19,  22,   9,   9,  53,   9,   0,   8,  53,  13,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  16,   9,   9,   9,   9,  53,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  91,  92,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  93,   0,   0,   0,   0,   0,   0,   0,   0,  91,   0,
      0,   0,  94,  95,   0,   0,   0,   0,   0,  96,   0,   0,   0,   0,   0,   0,
      9,   9,   9,  10,   9,   9,   9,   9,   9,  17,  65,  39,  24,   0,   0,   0,
      9,   9,   0,  65,   0,   0,   0,  64,   9,   9,   9,   9,   9,  10,   0,   0,
      9,  10,   9,  10,   9,  10,   9,  10,   0,   0,   0,  65,   0,   0,   0,   0,
      0,  25,   0,   0,   0,   0,   0,   0,   0,   0,  69,  18,  97,  98,  65,  17,
      0,   0,   0,   0,   0,   0,  99, 100, 101, 101, 101, 101, 101, 101, 101, 101,
    101, 101, 101, 101, 101, 101, 102, 101,   0,   8,   9,   9,   9,   9,   9,   9,
      9,   9,   9,  10,   8,   9,   9,   9,   9,   9,   9,  10,   0,   0,   0,   0,
      9,   9,   9,   9,   9,   9,  10,   0,   0,   0,   0,   0, 101, 101, 101, 101,
    101, 101, 101, 101, 101, 101, 101, 102, 101, 101, 101, 101, 101, 101,   0,   0,
      9,   9,   9,  17,   0,   0,   0,   0,   0,   0,   0,   0,   9,   9,   9,   9,
      9,   9,   9,  17,   9,   9,   9,   9,   6,   6,  47,   0,   0,   0,   0,   0,
      9,   9,   9,  39,  37,  18,  18, 103,   9,   9,   9,   9,   9,   9,   9,  51,
      9,   9,   9,   9,  57,   0,   0,   0,   0,   0,   0,   0,   0,  65,   9,   9,
      9,   9,   9,  10,   9,   9,   0,   0, 104, 104,  39,   9,   9,   9,   9,   9,
     39,  18,   0,   0,   0,   0,   0,   0,   9,   9,   9,   9,   9,   0,   0,   0,
     24,   9,   9,   9,   9,   9,   9,   9,  18,  57,   0,   0,   6,   6,  73,   0,
     18,  18,  18,  18,  24,   9,  65,  25,   9,  51,  18,  57,   9,   9,   9,   9,
      9,  39,  18,  18,  18,   0,   0,   0,   9,   9,   9,   9,   9,   9,   9,  17,
      9,   9,   9,   9,  39,  18,  18,  18,  74,   0,   0,  65,   6,   6,  73,   0,
      0,  67,   0,   0,   6,   6,  73,   0,   9,   9,  56,  18,  18,  37,   0,   0,
     39,   9,   9,  57,   6,   6,  73,   0,   0,   0,   0,   0,   0,   0,  64,  57,
      0,   0,   0,   0,  46,  62,  74,  69,  67,   0,   0,   0,   0,   0,   0,   0,
      9,   9,  39,  18,  16, 105,   0,   0,   8,  10,   8,  10,   8,  10,   0,   0,
      9,  10,   9,  10,   9,   9,   9,   9,   9,  53,   0,   0,   9,   9,   9,   9,
     39,  18,  37,  57,   6,   6,  73,   0,   9,   0,   0,   0,   9,   9,   9,   9,
      9,  10,  65,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   0,
      9,  10,   0,   0,  65,   9,   0, 106,  30,  30, 107,  30,  30,  31,  30, 108,
    109, 107,  30,  30,   9,   9,   9,   9,   9,   9,   9,   9,  53,   0,   0,   0,
      0,   0,   0,   0,  65,   9,   9,   9,   9,   9,   9,   9,  16,   9,   9,   9,
      9,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9,   9,   9,   0,
     18,  18,  18,  18, 110,  89,   0,   0,  18,  18,  18,  18,  11,  88,   0,   0,
      0,   0,   0, 111,   5, 112,   0,   0,   0,   0,   0,   0,   9,  19,   9,   9,
      9,   9,   9,   9,   9,   9,   9, 113,   0, 114,   0,   5,   0,   0, 115,   0,
      0, 116, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 117,
     16,   9,  16,   9,  16,   9,  16,  17,   0,   0,   0,   0,   0,   0, 118,   0,
      9,   9,   9,   8,   9,   9,   9,   9,   9,  10,   9,   9,   9,   9,  10,  22,
      9,   9,   9,  53,   9,   9,   9,  53,   9,   9,   9,   9,   9,  17,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  67,   9,   9,   9,   9,  17,   0,   0,   0,
     74,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   9,   9,   9,   9,
      9,   9,  10,   0,   9,   9,   9,   9,   9,   9,   9,   9,   9,  51,  37,   0,
      9,   0,   9,   9,   8,  53,   0,   0,   6,   6,  73,   0,   9,   9,   9,   9,
      9,   9,   9,   9,   9,   0,   9,   9,   9,   9,   0,   0,   9,   9,   9,   9,
      9,   0,   0,   0,   0,   0,   0,   0,   9,  53,  19,   9,   9,   9,   9,   9,
      9,   9,   9,   9,   9,  22,  17,  48,   9,   9,   9,   9,  10,  53,   0,   0,
      9,   9,   9,   9,   9,   9,  53,   0,  56,  29,   0,  18,   9,   8,   8,   9,
      9,   9,   9,   9,   9,   0,  37,  64,   9, 105,   0,   0,   0,   0,   0,   0,
      9,   9,   9,   9,  10,   0,   0,   0,   9,   9,   9,   9,   9,   9,  18,  18,
     18,  37,   0,   0,   0,   0,   0,   0,   0,  60,   6,   6,   0,   0,   0,  64,
      9,   9,   9,   9,  18,  18,  37,  14,   9,   9,  17,   0,   6,   6,  73,   0,
      9,  39,  18,  18,  18, 119,   6,   6,   9,   9,   9,   9,  39,  13,   0,   0,
     42,  17,  69,  74,   6,   6, 120,  17,   9,   9,   9,   9,  22,   9,   9,   9,
      9,   9,   9,  18,  18,  18,   0,  71,   9,  10,  19,  22,   9,   9,   9,  22,
      9,   9,  17,   0,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,  39,
     18,  18,  37,   0,   6,   6,  73,   0,  18,   8,   9,  48,  48,   9,   9,   9,
     18,  62,  62,  57,  17,  64,   0,   8,  51,  69,  18,  74,  18,  74,   0,   0,
      9,   9,   9,   9,   9,  56,  18,  18,  18,  81,  10,   0,   6,   6,  73,   0,
     18,  22,   0,   0,   6,   6,  73,   0,   9,   9,   9,  39,  18,  57,  18,  18,
     74,   0,   0,   0,   0,   0,   9,  57,  74,  17,   0,   0,   6,   6,  73,   0,
      9,   9,  39,  18,  18,  18,   0,   0,   0,   0,   0,   0,   0,   0,   0,  27,
     18,  18,  18,   0,   6,   6,  73,   0,   6,   6,  73,   0,   0,   0,   0,  65,
     56,  18,  81,   9,   9,   9,   9,   9,   9,   9,   9,   9,  39,  18,  55,  37,
      0,  64,   0,   0,  56,  18,  18,   9,   9,  16,  51,  18,  18,  18,  57,   0,
      9,   9,   9,   9,   9,   9,  17,   0,   9,   9,  19,   9,   9,   9,   9,   9,
      9,   9,   9,  39,  18,  37,  18,  18,  17,   0,   0,   0,   6,   6,  73,   0,
      0,   0,   0,   0,  16,   9,   9,   9,   9,   9,   9,   9,  69,  18,  18,  18,
     18,  18,  27,  18,  18,  37,   0,   0,   9,  10,  22,   9,   9,   9,   9,   9,
      9,   9,   9,   9,  56,  37,  71,  28,  18,  55,   0,   0,   6,   6,  73,   0,
      9,  10,   0,   0,   0,   0,   0,   0,   9,   9,   9,  53,  18,  74,   0,   0,
      9,   9,   9,   9,  18,  37,   0,   0,   9,   0,   0,   0,   6,   6,  73,   0,
     65,   9,   9,   9,   9,   9,   0,   8,   9,  17,   0,   0,  56,  18,  18,  18,
     18,  18,  18,  18,  18,  18,  18,  37,   0,   0,   0,  64,  81,   9,   9,   9,
     53,   0,   0,   0,   0,   0,   0,   0, 100,   0,   0,   0,   0,   0,   0,   0,
      9,   9,  10,   0,   9,   9,   9,  17,   9,   9,  17,   0,   9,   9,  53,  29,
     34,   0,   0,   0,   0,   0,   0,   0,   0,  27,  57,  27, 121,  34, 122,  18,
     37,  27,  18,   0,   0,   0,   0,   0,   0,   0,  69,  57,   0,   0,   0,   0,
     69,  74,   0,   0,   0,   0,   0,   0,   9,   9,   9,   9,   9,  19,   9,   9,
      9,   9,   9,   9,   9,   9,   9,  19,  13,  66,   8,  19,   9,   9,  22,   8,
      9,   8,   9,   9,   9,   9,   9,   9,   9,  22,  10,   8,   9,  19,   9,  19,
      9,   9,   9,   9,   9,   9,  22,  10,   9,  61,  16,   9,  19,   9,   9,   9,
      9,  53,   9,   9,   9,   9,   9,   9,  19,   9,   9,   9,   9,   9,  10,   9,
     10,   9,   9,  60,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
     18,  18,  18,  18,  18,  37,  64,  18,  18,  18,  18,  74,   0,  67,   0,   0,
      0,  74,   0,   0,   0,   0,  64,  18,  27,  18,  18,  18,   0,   0,   0,   0,
     18,  37,  18,  18,  18,  18,  62,  18,  28,  46,  37,   0,   0,   0,   0,   0,
      9,  17,   0,   0,  18,  37,   0,   0,   9,  18,  37,   0,   6,   6,  73,   0,
     66,  48,   8,   9,  10,   9,  82,   0,  13,  65,  82,   8,  66,  48,  82,  82,
     66,  48,  10,   9,  10,   9,   8,  61,   9,   9,  22,   9,   9,   9,   9,   0,
      8,   8,  22,   9,   9,   9,   9,   0,   9,   9,  53,   0,   9,   9,   9,   9,
      0, 123, 124, 124, 124, 124, 124, 124,   0,   0,  96,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 125,   0,   0,   0,   0, 126,   0,   0,   0,
      0,  91,   0,   0, 126,   0,   0,   0,   0,  96,  96,   0,   0,   0,   0,   0,
    127, 128, 127, 129,   0,   0,   0,   0,   0,   0, 126, 130,   0,   0, 131, 132,
    127, 127, 133, 133, 129,   0,   0,   0,   0, 134, 135, 136, 133, 133, 129, 129,
    137, 137, 126,   0,   0,   0,   0,   0,   0,   0, 136,   0,   0,   0, 126,  96,
      0, 126,   0,  96,   0,   0,   0,   0,   0,   0,   0,   0,   0,  95, 136,   0,
      0,   0,   0,   0, 129, 138,   0,   0,   0, 137, 139, 133,   0,   0,   0,   0,
     96,   0,   0,   0, 125,   0,   0,   0, 139,   0,   0,   0,   0, 140,   0,   0,
    129,   0,   0, 129,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 133, 141,
      0, 136,   0,   0, 133, 133,  95, 138,   0,   0,   0,   0, 137, 133, 133,  95,
     14,   0,   0,   0,   0,   0,   0,   0,  18,  18,  18,  18,   0,   0,   0,   0,
};

static RE_UINT8 re_word_break_stage_5[] = {
     0,  0,  0,  0,  0,  0,  5,  6,  6,  4,  0,  0,  0,  0,  1,  0,
     0,  0,  0,  2, 13,  0, 14,  0, 15, 15, 15, 15, 15, 15, 12, 13,
     0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,  0,  0,  0,  0, 16,
     0,  6,  0,  0,  0,  0, 11,  0,  0,  9,  0,  0,  0, 11,  0, 12,
     0,  0, 11, 11, 11,  0,  0,  0,  7,  7,  7,  7, 11,  0, 11, 11,
    11, 11, 13, 11,  0,  0, 11, 12, 11, 11,  0, 11, 11, 11,  0,  7,
     7,  7, 11, 11,  0, 11,  0,  0,  0, 13,  0,  0,  0,  7,  7,  7,
     7,  7,  0,  7,  0,  7,  7,  0,  3,  3,  3,  3,  3,  3,  3,  0,
     3,  3,  3, 11, 12,  0,  0,  0,  9,  9,  9,  9,  9,  9,  0,  0,
    13, 13,  0,  0,  7,  7,  7,  0,  9,  0,  0,  0, 11, 11, 11,  7,
    15, 15,  0, 15, 13,  0, 11, 11,  7, 11, 11, 11,  0, 11,  7,  7,
     7,  9,  0,  7,  7, 11, 11,  7,  7,  0,  7,  7, 15, 15, 11, 11,
    11,  0,  0, 11,  0,  0,  0,  9, 11,  7, 11, 11, 11, 11,  7,  7,
     7, 11,  0,  0, 11, 11,  0,  0, 13,  0, 11,  0,  7,  7, 11,  7,
    11,  7,  7,  7,  7,  7,  0,  0,  7,  7,  9,  7,  7, 11,  7,  7,
     0,  0, 15, 15, 11,  0, 11,  0,  7,  0,  0,  7,  7,  7, 11,  0,
     0,  0,  0,  7,  0,  0,  0, 11,  0, 11, 11,  0,  0,  7,  0,  0,
    11,  7,  0,  0,  0,  0,  7,  7,  0,  0,  7, 11,  0,  0,  7,  0,
     7,  0,  7,  0, 15, 15,  0,  0,  7,  0,  0,  0,  0,  7,  0,  7,
    15, 15,  7,  7, 11,  0,  7,  7,  7,  7,  9,  0, 11,  7,  7, 11,
    11,  7, 11,  0,  7,  7,  7, 11,  0, 11,  0, 11,  7, 19,  9,  9,
    14, 14,  0,  0, 14,  0,  0, 12,  6,  6,  9,  9,  9,  9,  9, 16,
    16,  0,  0,  0, 13,  0,  0,  0,  9,  0,  9,  9,  0, 17,  0,  0,
    20,  0, 20,  0,  0, 20, 20,  0, 20,  0, 17, 17, 17, 17,  0,  0,
    20,  0,  0,  0,  0, 10, 10, 10, 10, 10,  0,  0,  0,  7,  7, 10,
    10,  0,  0,  0, 10, 10, 10, 10, 10, 10, 10,  0,  7,  7,  0, 11,
    11, 11,  7, 11, 11,  7,  7,  0,  0,  3,  7,  3,  3,  0,  3,  3,
     3,  0,  3,  0,  3,  3,  0,  3, 13,  0,  0, 12,  0, 16, 16, 16,
    13, 12,  0,  0, 11,  0,  0,  9,  0,  0,  0, 14,  0,  0, 12, 13,
     0,  0, 10, 10, 10, 10,  7,  7,  0,  9,  9,  9,  7,  0, 15, 15,
    15, 15, 11,  0,  7,  7,  7,  9,  9,  9,  9,  7,  0,  0,  8,  8,
     8,  8,  8,  8,  0,  0, 20,  0,  0,  0,  0, 20,  0,  0, 17, 17,
    17,  0,  0, 17, 17,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0, 18,
    18, 18, 18, 18, 17, 17, 17, 17,  0,  0, 21, 21, 21, 21,  0,  0,
     0,  0, 17,  0,  0, 17, 17, 17,  0, 17, 17,  0,  0,  0,  0, 17,
    17, 17, 17,  0, 17,  0, 17, 17,
};

/* Word_Break: 5760 bytes. */

RE_UINT32 re_get_word_break(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_word_break_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_word_break_stage_2[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_word_break_stage_3[pos + f] << 3;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_word_break_stage_4[pos + f] << 2;
    value = re_word_break_stage_5[pos + code];

    return value;
}

/* Grapheme_Cluster_Break. */

static RE_UINT8 re_grapheme_cluster_break_stage_1[] = {
     0,  1,  2,  2,  2,  3,  4,  5,  6,  2,  2,  7,  2,  8,  9, 10,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    11,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,
};

static RE_UINT8 re_grapheme_cluster_break_stage_2[] = {
     0,  1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16,  1, 17,  1,  1,  1, 18, 19, 20, 21, 22, 23, 24,  1,  1,
    25,  1,  1,  1,  1,  1, 26, 27,  1,  1,  1,  1, 28, 29,  1,  1,
    30,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1, 31,  1, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 36, 37, 38, 39, 40, 41, 42, 36, 37, 38, 39, 40, 41,
    42, 36, 37, 38, 39, 40, 41, 42, 36, 37, 38, 39, 40, 41, 42, 36,
    37, 38, 39, 40, 41, 42, 36, 43, 44, 44, 44, 44, 44, 44, 44, 44,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 45,  1,  1, 46, 47,
     1, 48, 49, 50,  1,  1,  1,  1,  1,  1, 51,  1,  1,  1,  1,  1,
    52, 53, 54, 55, 56, 57, 58, 59,  1,  1, 60,  1, 61, 62,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 63, 64,  1,  1,  1, 65,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 66,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1, 67, 68,  1,  1,  1,  1,  1,  1,  1, 69,  1,  1,  1,  1,  1,
    70,  1,  1,  1,  1,  1,  1,  1, 71, 72,  1,  1,  1,  1,  1,  1,
     1, 73,  1, 74, 75, 76, 77,  1,  1, 78,  1,  1,  1,  1,  1,  1,
    79, 80, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_grapheme_cluster_break_stage_3[] = {
      0,   1,   2,   2,   2,   2,   2,   3,   1,   1,   4,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      5,   5,   5,   5,   5,   5,   5,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   6,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   7,   5,   8,   9,   2,   2,   2,
     10,  11,   2,   2,  12,   5,   2,  13,   2,   2,   2,   2,   2,  14,  15,   2,
     16,  17,   2,   5,  18,   2,   2,   2,   2,   2,  19,  13,   2,   2,  12,  20,
      2,  21,  22,   2,   2,  23,   2,   2,   2,   2,   2,   2,   2,  24,  25,   5,
     26,   2,   2,  27,  28,  29,  30,   2,  31,   2,   2,  32,  33,  34,  30,   2,
     35,   2,   2,  36,  37,  17,   2,  38,  35,   2,   2,  36,  39,   2,  30,  40,
     31,   2,   2,  41,  33,  42,  30,   2,  43,   2,   2,  44,  45,  34,   2,   2,
     46,   2,   2,  47,  48,  49,  30,   2,  31,   2,   2,  50,  51,  49,  30,   2,
     52,   2,   2,  53,  54,  34,  30,   2,  55,   2,   2,   2,  56,  57,   2,  55,
      2,   2,   2,  58,  59,   2,   2,   2,   2,   2,   2,  60,  61,   2,   2,   2,
      2,  62,   2,  63,   2,   2,   2,  64,  65,  66,   5,  67,  68,   2,   2,   2,
      2,   2,  69,  70,   2,  71,  13,  72,  73,  74,   2,   2,   2,   2,   2,   2,
     75,  75,  75,  75,  75,  75,  76,  76,  76,  76,  77,  78,  78,  78,  78,  78,
      2,   2,   2,   2,   2,  69,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,  79,   2,  79,   2,  30,   2,  30,   2,   2,   2,  80,  81,  82,   2,   2,
     83,   2,   2,   2,   2,   2,   2,   2,  49,   2,  84,   2,   2,   2,   2,   2,
      2,   2,  85,  86,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,  87,   2,   2,   2,  88,  89,  90,   2,   2,   2,  91,   2,   2,   2,   2,
     92,   2,   2,  93,  94,   2,  12,  20,  95,   2,  96,   2,   2,   2,  97,  52,
      2,   2,  98,  99,   2,   2,   2,   2,   2,   2,   2,   2,   2, 100, 101, 102,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   5,   5,   5, 103,
    104,   2, 105,   2,   2,   2,   1,   2,   2,   2,   2,   2,   2,   5,   5,  13,
      2, 106,   2,   2, 107,   2,   2,   2,   2, 108,   2,   2,   2,   2,   2, 109,
    110,   2,   2,   2,   2,   2, 111,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 112, 113,
      2,   2,   2,   2,   2,   2,   2, 112,   2,   2,   2,   2,   2,   2,   5,   5,
      2,   2,  40,   2,   2,   2,   2,   2,   2, 114,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2, 112, 115,   2,  47,   2,   2,   2,   2,   2, 113,
    116,   2, 117,   2,   2,   2,   2,   2, 118,   2,   2, 119, 120,   2,   5, 113,
      2,   2, 121,   2, 122,  52,  75, 123,  26,   2,   2, 124, 125,   2, 126,   2,
      2,   2, 127, 128, 129,   2,   2, 130,   2,   2,   2, 131,  17,   2, 132, 133,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 134,   2,
    135, 136, 137, 138, 137, 139, 137, 135, 136, 137, 138, 137, 139, 137, 135, 136,
    137, 138, 137, 139, 137, 135, 136, 137, 138, 137, 139, 137, 135, 136, 137, 138,
    137, 139, 137, 135, 136, 137, 138, 137, 139, 137, 135, 136, 137, 138, 137, 139,
    137, 135, 136, 137, 138, 137, 139, 137, 135, 136, 137, 138, 137, 139, 137, 135,
    136, 137, 138, 137, 139, 137, 135, 136, 137, 138, 137, 139, 137, 135, 136, 137,
    138, 137, 139, 137, 135, 136, 137, 138, 137, 139, 137, 135, 136, 137, 138, 137,
    139, 137, 135, 136, 137, 138, 137, 139, 137, 135, 136, 137, 138, 137, 139, 137,
    137, 138, 137, 139, 137, 135, 136, 137, 138, 137, 140,  76, 141,  78,  78, 142,
      1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      2, 143,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      5,   2,   5,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,
      2,   2,   2,   2,   2,   2,   2,   2,   2,  47,   2,   2,   2,   2,   2, 144,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  74,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  13,   2,
      2,   2,   2,   2,   2,   2,   2, 145,   2,   2,   2,   2,   2,   2,   2,   2,
    146,   2,   2, 147,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  49,   2,
    148,   2,   2, 149, 150,   2,   2, 112,  95,   2,   2, 151,   2,   2,   2,   2,
    152,   2, 153, 154,   2,   2,   2, 155,  95,   2,   2, 156, 157,   2,   2,   2,
      2,   2, 158, 159,   2,   2,   2,   2,   2,   2,   2,   2,   2, 112, 160,   2,
     52,   2,   2,  32, 161,  34, 162, 154,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2, 163, 164,   2,   2,   2,   2,   2,   2, 165, 166,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 112, 167,  13, 168,   2,   2,
      2,   2,   2, 169,  13,   2,   2,   2,   2,   2, 170, 171,   2,   2,   2,   2,
      2,  69, 172,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    173,   2,   2, 174,  34, 175,   2,   2, 176, 177,   2,   2,   2,   2,   2,   2,
      2,   2, 178, 179,   2,   2,   2,   2,   2, 180, 181, 182,   2,   2,   2,   2,
      2,   2,   2, 183, 184,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 154,
      2,   2,   2, 150,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2, 185, 186, 187, 112, 152,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2, 188, 189,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2, 190, 191, 192,   2, 193,   2,   2,   2,   2,   2,
      2,   2,   2,   2,  79,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      5,   5,   5, 194,   5,   5,  67, 126, 195,  12,   7,   2,   2,   2,   2,   2,
    196, 197, 198,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 150,   2,   2,
      2,   2,   2,   2, 199,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 200, 201,
    202,   2,   2, 203,   2,   2,   2, 204, 205, 204, 206,   2, 207,   2, 208, 209,
      2,   2,   2,   2, 210, 211, 212, 213, 214,   2, 215, 216,   2,   2,   2,   2,
      2,   2, 217,   2,   2,   2,   2, 218,   2, 219,   2,   2,   2,   2, 202,   2,
      2,   2,   2,   2, 220,   2,   2,   2, 221, 222, 223, 224, 225,   2,   2,   2,
      2, 226, 227, 228,   2,   2,   2,   2,   2,   2,   2,   2,   2, 229,   2,   2,
      1,   1,   5,   5,   5,   5,   5,   5,   1,   1,   1,   1,   1,   1,   1,   1,
      5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   1,
};

static RE_UINT8 re_grapheme_cluster_break_stage_4[] = {
      0,   0,   1,   2,   0,   0,   0,   0,   3,   3,   3,   3,   3,   3,   3,   4,
      3,   3,   3,   5,   6,   6,   6,   6,   7,   6,   8,   3,   9,   6,   6,   6,
      6,   6,   6,  10,  11,  10,   3,   3,  12,  13,   3,   3,   6,   6,  14,  15,
      3,   3,   7,   6,  16,   3,   3,   3,   3,  17,   6,  18,   6,  19,  20,   8,
      3,   3,   3,  21,  22,   3,   3,   3,   6,   6,  14,   3,   3,  17,   6,   6,
      6,   3,   3,   3,   3,  17,  10,   6,   6,   9,   9,   8,   3,   3,   9,   3,
      3,   6,   6,   6,  23,   6,   6,   6,  24,   3,   3,   3,   3,   3,  25,  26,
     27,   6,  28,  29,   9,   6,   3,   3,  17,   3,   3,   3,  30,   3,   3,   3,
      3,   3,   3,  31,  27,  32,  33,  34,   3,   7,   3,   3,  35,   3,   3,   3,
      3,   3,   3,  26,  36,   7,  19,   8,   8,  22,   3,   3,  27,  10,  37,  34,
      3,   3,  17,   6,   3,   3,   3,  20,   3,  17,   3,   3,  38,   3,   3,   3,
      3,   3,   3,  25,  39,  40,  41,  34,  28,   3,   3,   3,   3,   3,   3,  17,
     28,  42,  20,   8,   3,  11,   3,   3,   3,   3,   3,  43,  44,  45,  41,   8,
     46,   3,   3,   3,   3,   3,   7,  31,  27,  26,  41,  47,  40,   3,   3,   3,
      3,   3,  38,   7,  48,  49,  50,  51,  52,   6,  14,   3,   3,   7,   6,  14,
     52,   6,  10,  16,   3,   3,   6,   8,   3,   3,   8,   3,   3,  53,  22,  40,
      9,   6,   6,  24,   6,  20,   3,   9,   6,   6,   9,   6,   6,   6,   6,  16,
      3,  38,   3,   3,   3,   3,   3,   9,  54,   6,  35,  36,   3,  40,   8,  17,
      9,  16,   3,   3,  38,  36,   3,  22,   3,   3,   3,  22,  55,  55,  55,  55,
     56,  56,  56,  56,  56,  56,  57,  57,  57,  57,  57,  57,  17,  16,   3,   3,
      3,  58,   6,  46,  50,  44,  27,   6,   6,   3,   3,  22,   3,   3,   7,  59,
      3,   3,  22,   3,  24,  51,  28,   3,  44,  50,  27,   3,   3,   7,  60,   3,
      3,  61,   6,  14,  49,   9,   6,  28,  51,   6,   6,  19,   6,   6,   6,  14,
      6,  62,   3,   3,   3,  54,  24,  28,  44,  62,   3,   3,  63,   3,   3,   3,
     64,  46,  58,   8,   3,  25,  46,  65,   3,  50,  50,   6,   6,  48,   3,   3,
     14,   6,   6,   6,  54,   6,  16,  22,  40,  32,   8,   3,   6,   6,  10,   6,
      3,   3,   4,  66,   3,   3,   0,  67,   3,   3,   3,  68,  69,   3,   3,   3,
      3,  70,   3,   3,   3,   3,  68,   3,   3,   3,  71,  72,   3,  73,   3,   3,
      3,   3,   3,   7,   8,   3,   3,   3,   3,   3,  11,   3,  14,   6,   6,   8,
     38,  38,   7,   3,  74,  75,   3,   3,  76,   3,   3,   3,   3,  50,  50,  50,
     50,   8,   3,   3,   3,  17,   6,   8,   3,   7,   6,   6,  55,  55,  55,  77,
      7,  48,  46,  28,  62,   3,   3,   3,   3,  22,   3,   3,   3,   3,   9,  24,
     75,  36,   3,   3,   7,   3,   3,  78,   3,   3,   3,  16,  20,  19,  16,  17,
      3,   3,  74,  46,   3,  79,   3,   3,  74,  29,  39,  34,  80,  81,  81,  81,
     81,  81,  81,  80,  81,  81,  81,  81,  81,  81,  80,  81,  81,  80,  81,  81,
     81,   3,   3,   3,  56,  82,  83,  57,  57,  57,  57,   3,   3,   3,   3,  38,
      0,   0,   0,   3,   3,  17,  14,   3,   9,  11,   3,   6,   3,   3,  14,   7,
     84,   3,   3,   3,   3,   3,   6,   6,   6,  14,   3,   3,  51,  24,  36,  85,
     14,   3,   3,   3,   3,   7,   6,  27,   6,  16,   3,   3,   7,   3,   3,   3,
     74,  48,   6,  24,  86,   3,  17,  16,   3,   3,   3,  51,  46,  54,   3,  38,
     51,   6,  14,   3,  28,  33,  33,  76,  40,  17,   6,  16,   3,  87,   6,   6,
     48,  88,   3,   3,  60,   6,  89,  65,  54,   3,   3,   3,  48,   8,  50,  58,
      3,   3,   3,   8,  51,   6,  24,  65,   3,   3,   7,  29,   6,  58,   3,   3,
     48,  58,   6,   3,   9,  24,  36,   3,   7,   6,  90,  14,   9,  24,  27,   3,
      3,  91,  92,   6,   6,  24,   8,   3,   3,   3,   3,  74,   6,  14,   6,  58,
     17,   6,   6,   6,   6,   6,  64,   6,  54,  36,   3,   3,   9,  14,  38,  10,
      6,  23,   3,   3,  87,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  93,
      3,   3,   3,  11,   0,   3,   3,   3,   3,  94,   8,  64,  95,   0,  96,   6,
     14,   9,   6,   3,   3,   3,  17,   8,   6,  14,   7,   6,   3,  16,   3,   3,
      6,  14,   6,   6,   6,   6,  19,   6,  10,  20,  14,   3,   3,   6,  14,   3,
      3,  97,  98,  98,  98,  98,  98,  98,   3,   3,  73,   3,   3,   3,   3,  99,
    100,   3,   3,   3,   3,  68,   3,   3,   3,  73,  73,   3, 101, 102, 101, 103,
      3,   3, 100, 104,   3,   3, 105, 106, 101, 101, 107, 107, 103,   3,   3,   3,
      3, 108, 109, 110, 107, 107, 103, 103, 111, 111, 100,   3,   3,   3, 110,   3,
      3,   3, 100,  73,   3, 100,   3,  73,   3,  72, 110,   3, 103, 112,   3,   3,
      3, 111, 113, 107,  73,   3,   3,   3,  99,   3,   3,   3, 113,   3,   3,   3,
      3, 114,   3,   3, 103,   3,   3, 103,   3,   3, 107, 115,   3, 110,   3,   3,
    107, 107,  72, 112, 111, 107, 107,  72,
};

static RE_UINT8 re_grapheme_cluster_break_stage_5[] = {
     4,  4,  4,  4,  4,  4,  3,  4,  4,  2,  4,  4,  0,  0,  0,  0,
     0,  0,  0,  4,  0,  4,  0,  0,  5,  5,  5,  5,  0,  0,  0,  5,
     5,  5,  0,  0,  0,  5,  5,  5,  5,  5,  0,  5,  0,  5,  5,  0,
     1,  1,  1,  1,  1,  1,  0,  0,  5,  5,  5,  0,  4,  0,  0,  0,
     5,  0,  0,  0,  0,  0,  5,  5,  5,  1,  0,  5,  5,  0,  0,  5,
     5,  0,  5,  5,  0,  0,  0,  1,  0,  5,  0,  0,  5,  5,  1,  5,
     5,  5,  5,  7,  0,  0,  5,  7,  5,  0,  7,  7,  7,  5,  5,  5,
     5,  7,  7,  7,  7,  5,  7,  7,  0,  5,  7,  7,  5,  0,  5,  7,
     5,  0,  0,  7,  7,  0,  0,  7,  7,  5,  0,  0,  0,  5,  5,  7,
     7,  5,  5,  0,  5,  7,  0,  7,  0,  0,  5,  0,  5,  7,  7,  0,
     0,  0,  7,  7,  7,  0,  7,  7,  7,  0,  5,  5,  5,  0,  7,  5,
     7,  7,  5,  7,  7,  0,  5,  7,  5,  5,  7,  7,  7,  5,  1,  0,
     7,  7,  5,  5,  5,  0,  5,  0,  7,  7,  7,  7,  7,  7,  7,  5,
     0,  5,  0,  7,  0,  5,  0,  5,  5,  7,  5,  5,  8,  8,  8,  8,
     9,  9,  9,  9, 10, 10, 10, 10,  5,  5,  7,  5,  5,  5,  4,  0,
     5,  7,  7,  5,  0,  7,  5,  7,  7,  0,  0,  0,  5,  5,  7,  0,
     0,  7,  5,  5,  7,  5,  7,  5,  5, 15,  4,  4,  4,  4,  4,  0,
     0, 13,  0,  0, 16,  0, 16,  0,  0, 16, 16,  0, 16,  0, 13, 13,
    13, 13,  0,  0, 16,  0,  0,  0,  0,  0,  0,  7,  7,  5,  5,  7,
     7,  7,  0,  0,  8,  0,  0,  0,  5,  7,  0,  0,  0,  7,  5,  0,
    11, 12, 12, 12, 12, 12, 12, 12,  9,  9,  9,  0,  0,  0,  0, 10,
     7,  5,  7,  0,  0,  1,  0,  0,  7,  0,  1,  1,  0,  7,  7,  7,
     5,  7,  5,  0,  5,  7,  5,  7,  5,  7,  1,  5,  0,  0,  1,  1,
     1,  1,  5,  5,  7,  7,  7,  0,  0,  5,  7,  5,  5,  5,  5,  4,
     4,  4,  4,  5,  0,  0,  6,  6,  6,  6,  6,  6,  0,  0, 16,  0,
     0,  0,  0, 16,  0,  0, 13, 13, 13,  0,  0, 13, 13,  0,  0,  0,
     0, 16,  0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 13, 13, 13, 13,
     0,  0, 17, 17, 17, 17,  0,  0,  0,  0, 13,  0,  0, 13, 13, 13,
     0, 13, 13,  0,  0,  0,  0, 13, 13, 13, 13,  0, 13,  0, 13, 13,
};

/* Grapheme_Cluster_Break: 3200 bytes. */

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
    11, 12, 13, 14, 15,  9, 16,  5, 17,  9,  9, 18,  9, 19, 20, 21,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 22, 23, 24,  5, 25, 26,
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
    27,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
};

static RE_UINT8 re_sentence_break_stage_2[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,  17,  18,  19,  20,  17,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
     31,  32,  33,  34,  35,  33,  33,  36,  33,  37,  33,  33,  38,  39,  40,  33,
     41,  42,  33,  33,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
     17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  43,  17,  17,
     17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
     17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  44,
     17,  17,  17,  17,  45,  17,  46,  47,  48,  49,  50,  51,  17,  17,  17,  17,
     17,  17,  17,  17,  17,  17,  17,  52,  33,  33,  33,  33,  33,  33,  33,  33,
     33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,
     33,  33,  33,  33,  33,  33,  33,  33,  33,  17,  53,  54,  17,  55,  56,  57,
     58,  59,  60,  61,  62,  63,  17,  64,  65,  66,  67,  68,  69,  33,  33,  33,
     70,  71,  72,  73,  74,  75,  76,  77,  78,  33,  79,  33,  80,  81,  33,  33,
     17,  17,  17,  82,  83,  84,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,
     17,  17,  17,  17,  85,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,
     33,  33,  33,  33,  17,  17,  86,  33,  33,  33,  33,  33,  33,  33,  33,  33,
     33,  33,  33,  33,  33,  33,  33,  33,  17,  17,  87,  88,  33,  33,  33,  89,
     17,  17,  17,  17,  17,  17,  17,  90,  17,  17,  91,  33,  33,  33,  33,  33,
     17,  92,  93,  33,  33,  33,  33,  33,  33,  33,  33,  33,  94,  33,  33,  33,
     33,  95,  96,  33,  97,  98,  99, 100,  33,  33, 101,  33,  33,  33,  33,  33,
    102,  33,  33,  33,  33,  33,  33,  33, 103, 104,  33,  33,  33,  33, 105,  33,
     33, 106,  33,  33,  33,  33, 107,  33,  33,  33,  33,  33,  33,  33,  33,  33,
     17,  17,  17,  17,  17,  17, 108,  17,  17,  17,  17,  17,  17,  17,  17,  17,
     17,  17,  17,  17,  17,  17,  17, 109, 110,  17,  17,  17,  17,  17,  17,  17,
     17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17, 111,  17,
     17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17, 112,  33,  33,  33,  33,
     33,  33,  33,  33,  33,  33,  33,  33,  17,  17, 113,  33,  33,  33,  33,  33,
    114, 115,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,  33,
};

static RE_UINT16 re_sentence_break_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
      8,  16,  17,  18,  19,  20,  21,  22,  23,  23,  23,  24,  25,  26,  27,  28,
     29,  30,  18,   8,  31,   8,  32,   8,   8,  33,  34,  35,  36,  37,  38,  39,
     40,  41,  42,  43,  41,  41,  44,  45,  46,  47,  48,  41,  41,  49,  50,  51,
     52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
     68,  69,  70,  71,  72,  73,  74,  75,  76,  73,  77,  78,  79,  80,  81,  82,
     83,  84,  85,  86,  87,  88,  89,  90,  83,  91,  92,  93,  94,  95,  96,  97,
     98,  99, 100,  56, 101, 102, 103,  56, 104, 105, 106, 107, 108, 109, 110,  56,
     41, 111, 112, 113, 114,  29, 115, 116,  41,  41,  41,  41,  41,  41,  41,  41,
     41,  41, 117,  41, 118, 119, 120,  41, 121,  41, 122, 123, 124,  29,  29, 125,
     98,  41,  41,  41,  41,  41,  41,  41,  41,  41,  41, 126, 127,  41,  41, 128,
    129, 130, 131, 132,  41, 133, 134, 135, 136,  41,  41, 137, 138, 139,  41, 140,
    141, 142, 143, 144,  41, 145, 146,  56, 147,  41, 148, 149, 150, 151,  56,  56,
    152, 133, 153, 154, 155, 156,  41, 157,  41, 158, 159, 160, 161,  56, 162, 163,
     18,  18,  18,  18,  18,  18,  23, 164,   8,   8,   8,   8, 165,   8,   8,   8,
    166, 167, 168, 169, 167, 170, 171, 172, 173, 174, 175, 176, 177,  56, 178, 179,
    180, 181, 182,  30, 183,  56,  56,  56,  56,  56,  56,  56,  56,  56,  56,  56,
    184, 185,  56,  56,  56,  56,  56,  56,  56,  56,  56,  56,  56, 186,  30, 187,
     56,  56, 188, 189,  56,  56, 190, 191,  56,  56,  56,  56, 192,  56, 193, 194,
     29, 195, 196, 197,   8,   8,   8, 198,  18, 199,  41, 200, 201, 202, 202,  23,
    203, 204, 205,  56,  56,  56,  56,  56, 206, 207,  98,  41, 208,  98,  41, 116,
    209, 210,  41,  41, 211, 212,  56, 213,  41,  41,  41,  41,  41, 140,  56,  56,
     41,  41,  41,  41,  41,  41,  41,  55,  41,  41,  41,  41, 214,  56, 213, 215,
    216, 217,   8, 218, 219,  41,  41, 220, 221, 222,   8, 223, 224, 225,  56, 226,
    227, 228,  41, 229, 230, 133, 231, 232,  50, 233, 234, 235,  60, 236, 237, 238,
     41, 239, 240, 241,  41, 242, 243, 244, 245, 246, 247, 248,  18,  18,  41, 249,
     41,  41,  41,  41,  41, 250, 251, 252,  41,  41,  41, 253,  41,  41, 254,  56,
    255, 256, 257,  41,  41, 258, 259,  41,  41, 260, 213,  41, 261,  41, 262, 263,
    264, 265, 266, 267,  41,  41,  41, 268, 269,   2, 270, 271, 272, 141, 273, 274,
    275, 276, 277,  56,  41,  41,  41, 212,  56,  56,  41, 278,  56,  56,  56, 279,
     56,  56,  56,  56, 235,  41, 280, 281,  41, 282, 283, 284, 285,  41, 286,  56,
     29, 287, 288,  41, 285, 289, 290, 291,  41, 292,  41, 293,  56,  56,  56,  56,
     41, 201, 140, 262,  56,  56,  56,  56, 294, 295, 140, 201, 141,  56,  56, 296,
    140, 254,  56,  56,  41, 297,  56,  56, 298, 299, 300, 235, 235,  56, 106, 301,
     41, 140, 140, 302, 258,  56,  56,  56,  41,  41, 303,  56,  29, 304,  18, 305,
    155, 306, 307, 308, 155, 309, 310, 311, 155, 312, 313, 314, 155, 236, 315,  56,
    316, 317,  56,  56, 318, 319, 320, 321, 322,  73, 323, 324,  56,  56,  56,  56,
     41, 325, 326,  56,  41,  47, 327,  56,  56,  56,  56,  56,  41, 328, 329,  56,
     41,  47, 330,  56,  41, 331, 135,  56, 332, 333,  56,  56,  56,  56,  56,  56,
     56,  56,  56,  56,  56,  29,  18, 334, 335, 336, 337,  41, 338,  56,  41, 339,
    340, 341, 342, 343, 344, 345,  56,  56, 346, 347, 348,  56,  56,  56,  56,  56,
     41,  41,  41,  41, 254,  56,  56,  56,  41,  41,  41, 211,  41,  41,  41,  41,
     41,  41, 293,  56,  56,  56,  56,  56,  41, 211,  56,  56,  56,  56,  56,  56,
     41,  41, 349,  56,  56,  56,  56,  56,  41, 339, 141, 350,  56,  56, 213, 351,
     41, 352, 353, 354, 124,  56,  56,  56,  41,  41, 355, 356, 357,  56,  56, 358,
     41,  41,  41,  41,  41,  41,  41, 214,  41,  41,  41,  41,  41,  41,  41, 302,
    141,  56,  56, 213,  41,  41,  41,  41,  41,  41,  41,  41,  41,  41,  41, 252,
     41,  41,  41, 359, 360, 361,  56,  56,  56,  56,  56, 362, 363, 364,  56,  56,
     56,  56, 365,  56,  56,  56,  56,  56, 366, 367, 368, 369, 370, 371, 372, 373,
    374, 375, 376, 377, 378, 366, 367, 379, 369, 380, 381, 382, 373, 383, 384, 385,
    386, 387, 388, 195, 389, 390, 391, 392,  23, 393,  23, 394, 395, 396,  56,  56,
    397, 398,  56,  56,  56,  56,  56,  56,  41,  41,  41,  41,  41,  41, 399,  56,
     29, 400, 401,  56,  56,  56,  56,  56, 402, 403, 404, 405, 406, 407,  56,  56,
     56, 408, 409, 409, 410,  56,  56,  56,  56,  56,  56, 411,  56,  56,  56,  56,
     41,  41,  41,  41,  41,  41, 201,  56,  41, 278,  41,  41,  41,  41,  41,  41,
    285,  41,  41,  41,  41,  41,  41,  41,  41,  41,  41,  41,  41, 412,  41,  41,
     41,  41,  41,  41,  41,  41,  41, 413, 285,  56,  56,  56,  56,  56,  56,  56,
    414,  23,  23,  23,  56,  56,  56,  56,  23,  23,  23,  23,  23,  23,  23, 415,
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
     44,  44,  44,  44,  25,  45,  46,  47,   0,  48,  22,  49,  32,  11,  11,  11,
     50,  11,  11,  15,  15,  15,  15,  15,  15,  15,  15,  51,  33,  34,  25,  25,
     25,  25,  25,  25,  15,  52,  30,  32,  11,  11,  11,  11,  11,  11,  11,  11,
     11,  11,  11,  11,  15,  15,  15,  15,  53,  44,  54,  25,  25,  25,  25,  25,
     28,  26,  26,  29,  25,  25,  25,  25,  25,  25,  25,  25,  10,  11,  11,  11,
     11,  11,  11,  11,  11,  22,  55,  56,  14,  15,  15,  15,  15,  15,  15,  15,
     15,  15,  57,   0,  58,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  59,
     60,  59,   0,   0,  36,  36,  36,  36,  36,  36,  61,   0,  36,   0,   0,   0,
     62,  63,   0,  64,  44,  44,  65,  66,  36,  36,  36,  36,  36,  36,  36,  36,
     36,  36,  67,  44,  44,  44,  44,  44,   7,   7,  68,  69,  70,  36,  36,  36,
     36,  36,  36,  36,  36,  71,  44,  72,  44,  73,  74,  75,   7,   7,  76,  77,
     78,   0,   0,  79,  80,  36,  36,  36,  36,  36,  36,  36,  44,  44,  44,  44,
     44,  44,  65,  81,  36,  36,  36,  36,  36,  82,  44,  44,  83,   0,   0,   0,
      7,   7,  76,  36,  36,  36,  36,  36,  36,  36,  67,  44,  44,  41,  84,   0,
     36,  36,  36,  36,  36,  82,  85,  44,  44,  86,  86,  87,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  86,   0,  36,  36,  61,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  36,  36,  36,  36,  36,  88,  36,  41,
      0,   0,   0,   0,   0,  44,  44,  44,  89,  44,  44,  44,  44,  44,  44,  44,
     44,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  82,  90,
     44,  44,  44,  44,  86,  44,  36,  36,  82,  91,   7,   7,  81,  36,  36,  36,
     86,  81,  36,  77,  77,  36,  36,  36,  36,  36,  88,  36,  43,  40,  41,  90,
     44,  92,  92,  93,   0,  94,   0,  95,  82,  96,   7,   7,  41,   0,   0,  97,
     58,  81,  61,  98,  77,  36,  36,  36,  36,  36,  88,  36,  88,  99,  41,  74,
     65,  94,  92,  87, 100,   0,  81,  43,   0,  96,   7,   7,  75, 101,   0,   0,
     58,  81,  36,  95,  95,  36,  36,  36,  36,  36,  88,  36,  88,  81,  41,  90,
     44,  59,  59,  87,  97,   0,   0,   0,  82,  96,   7,   7,   0,   0, 102,  44,
     58,  81,  36,  77,  77,  36,  36,  36,  44,  92,  92,  87,   0, 103,   0,  95,
     82,  96,   7,   7,  55,   0,   0,   0, 104,  81,  61,  40,  88,  41,  99,  88,
     98,  97,  61,  40,  36,  36,  41, 103,  65, 103,  74,  87,  97,  94,   0,   0,
      0,  96,   7,   7,   0,   0,   0,   0,  44,  81,  36,  88,  88,  36,  36,  36,
     36,  36,  88,  36,  36,  36,  41, 102,  44,  74,  74,  87,   0,  60,  61,   0,
     82,  96,   7,   7,   0,   0,   0,   0,  86,  81,  36,  88,  88,  36,  36,  36,
     36,  36,  88,  36,  36,  81,  41,  90,  44,  74,  74,  87,   0,  60,   0, 105,
     82,  96,   7,   7,  99,   0,   0,   0,  36,  36,  36,  36,  36,  36,  67,  90,
     44,  74,  74,  93,   0,  67,   0,  98,  82,  96,   7,   7,   0,   0,  40,  36,
    103,  81,  36,  36,  36,  61,  40,  36,  36,  36,  36,  36,  95,  36,  36,  55,
     36,  61, 106,  94,  44, 107,  44,  44,   0,  96,   7,   7, 103,   0,   0,   0,
     81,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  80,  44,  65,   0,
     36,  67,  44,  65,   7,   7, 108,   0,  99,  77,  43,  55,   0,  36,  81,  36,
     81, 109,  40,  81,  80,  44,  59,  83,  36,  43,  44,  87,   7,   7, 108,  36,
     97,   0,   0,   0,   0,   0,  87,   0,   7,   7, 108,   0,   0, 110, 111, 112,
     36,  36,  81,  36,  36,  36,  36,  36,  36,  36,  36,  97,  58,  44,  44,  44,
     44,  74,  36,  86,  44,  44,  58,  44,  44,  44,  44,  44,  44,  44,  44, 113,
      0, 106,   0,   0,   0,   0,   0,   0,  36,  36,  67,  44,  44,  44,  44, 114,
      7,   7, 115,   0,  36,  82,  75,  82,  90,  73,  44,  75,  86,  70,  36,  36,
     82,  44,  44,  85,   7,   7, 116,  87,  11,  50,   0, 117,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  61,  36,  36,  36,  88,  41,  36,  61,  88,  41,
     36,  36,  88,  41,  36,  36,  36,  36,  36,  36,  36,  36,  88,  41,  36,  61,
     88,  41,  36,  36,  36,  61,  36,  36,  36,  36,  36,  36,  88,  41,  36,  36,
     36,  36,  36,  36,  36,  36,  61,  58, 118,   9, 119,   0,   0,   0,   0,   0,
     36,  36,  36,  36,   0,   0,   0,   0,  11,  11,  11,  11,  11, 120,  15,  39,
     36,  36,  36, 121,  36,  36,  36,  36, 122,  36,  36,  36,  36,  36, 123, 124,
     36,  36,  61,  40,  36,  36,  97,   0,  36,  36,  36,  88,  82, 113,   0,   0,
     36,  36,  36,  36,  82, 125,   0,   0,  36,  36,  36,  36,  82,   0,   0,   0,
     36,  36,  36,  88, 126,   0,   0,   0,  36,  36,  36,  36,  36,  44,  44,  44,
     44,  44,  44,  44,  44,  98,   0, 101,   7,   7, 108,   0,   0,   0,   0,   0,
    127,   0, 128, 129,   7,   7, 108,   0,  36,  36,  36,  36,  36,  36,   0,   0,
     36, 130,  36,  36,  36,  36,  36,  36,  36,  36, 131,   0,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  41,   0,   0,  36,  36,  36,  36,  36,  36,  36,  61,
     44,  44,  44,   0,  44,  44,  44,   0,   0,  91,   7,   7,  36,  36,  36,  36,
     36,  36,  36,  41,  36,  97,   0,   0,  36,  36,  36,   0,  36,  36,  36,  36,
     36,  36,  41,   0,   7,   7, 108,   0,  36,  36,  36,  36,  36,  67,  44,   0,
     36,  36,  36,  36,  36,  86,  44,  65,  44,  44,  44,  44,  44,  44,  44,  92,
      7,   7, 108,   0,   7,   7, 108,   0,   0,  98, 132,   0,  44,  44,  44,  65,
     44,  70,  36,  36,  36,  36,  36,  36,  44,  70,  36,   0,   7,   7, 115, 133,
      0,   0,  94,  44,  44,   0,   0,   0, 114,  36,  36,  36,  36,  36,  36,  36,
     86,  44,  44,  75,   7,   7,  76,  36,  36,  82,  44,  44,  44,   0,   0,   0,
     36,  44,  44,  44,  44,  44,   9, 119,   7,   7, 108,  81,   7,   7,  76,  36,
     36,  36,  36,  36,  36,  36,  36, 134,  15,  15,  42,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  65,  44,  44,  44,  44,  44,  70,  80,  82,  73,  87,   0,
     44,  44,  44,  44,  44,  44,  59,  44,  25,  25,  25,  25,  25,  34,  15,  27,
     15,  15,  11,  11,  15,  39,  11, 120,  15,  15,  11,  11,  15,  15,  11,  11,
     15,  39,  11, 120,  15,  15, 135, 135,  15,  15,  11,  11,  15,  15,  15,  39,
     15,  15,  11,  11,  15, 136,  11, 137,  46, 136,  11, 138,  15,  46,  11,   0,
     15,  15,  11, 138,  46, 136,  11, 138, 139, 139, 140, 141, 142, 143, 144, 144,
      0, 145, 146, 147,   0,   0, 148, 149,   0, 150, 149,   0,   0,   0,   0, 151,
     62, 152,  62,  62,  21,   0,   0, 153,   0,   0,   0, 148,  15,  15,  15,  42,
      0,   0,   0,   0,  44,  44,  44,  44,  44,  44,  44,  44, 113,   0,   0,   0,
     48, 154, 155, 156,  23, 117,  10, 120,   0, 157,  49, 158,  11,  38, 159,  33,
      0, 160,  39, 161,   0,   0,   0,   0, 162,  38,  97,   0,   0,   0,   0,   0,
      0,   0, 144,   0,   0,   0,   0,   0,   0,   0, 148,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 163,  11,  11,  15,  15,  39,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   4, 144, 124,   0, 144, 144, 144,   5,   0,   0,
      0, 148,   0,   0,   0,   0,   0,   0,   0, 164, 144, 144,   0,   0,   0,   0,
      4, 144, 144, 144, 144, 144, 124,   0,   0,   0,   0,   0,   0,   0, 144,   0,
      0,   0,   0,   0,   0,   0,   0,   5,  11,  11,  11,  22,  15,  15,  15,  15,
     15,  15,  15,  15,  15,  15,  15,  24,  31, 165,  26,  32,  25,  29,  15,  33,
     25,  42, 154, 166,  54,   0,   0,   0,  15, 167,   0,  21,  36,  36,  36,  36,
     36,  36,   0,  98,   0,   0,   0,  94,  36,  36,  36,  36,  36,  61,   0,   0,
     36,  61,  36,  61,  36,  61,  36,  61, 144, 144, 144,   5,   0,   0,   0,   5,
    144, 144,   5, 168,   0,   0,   0, 119, 169,   0,   0,   0,   0,   0,   0,   0,
    170,  81, 144, 144,   5, 144, 144, 171,  81,  36,  82,  44,  81,  41,  36,  97,
     36,  36,  36,  36,  36,  61,  60,  81,   0,  81,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  61,  81,  36,  36,  36,  36,  36,  36,  61,   0,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  61,   0,   0,   0,   0,   0,  36,  36,  36,  36,
     36,  36,  36,  97,   0,   0,   0,   0,  36,  36,  36,  36,  36,  36,  36, 172,
     36,  36,  36, 173,  36,  36,  36,  36,   7,   7,  76,   0,   0,   0,   0,   0,
     25,  25,  25, 174,  65,  44,  44, 175,  25,  25,  25,  25,  25,  25,  25, 176,
     36,  36,  36,  36, 177,   9,   0,   0,   0,   0,   0,   0,   0,  98,  36,  36,
    178,  25,  25,  25,  27,  25,  25,  25,  25,  25,  25,  25,  15,  15,  26,  30,
     25,  25, 179, 180,  25,  27,  25,  25,  25,  25,  31,  22,  11,  25,   0,   0,
      0,   0,   0,   0,   0,  98, 181,  36, 182, 182,  67,  36,  36,  36,  36,  36,
     67,  44,   0,   0,   0,   0,   0,   0,  36,  36,  36,  36,  36, 133,   0,   0,
     75,  36,  36,  36,  36,  36,  36,  36,  44,  87,   0, 133,   7,   7, 108,   0,
     44,  44,  44,  44,  75,  36,  98,  55,  36,  82,  44, 177,  36,  36,  36,  36,
     36,  67,  44,  44,  44,   0,   0,   0,  36,  36,  36,  36,  36,  36,  36,  97,
     36,  36,  36,  36,  67,  44,  44,  44, 113,   0, 149,  98,   7,   7, 108,   0,
     36,  80,  36,  36,   7,   7,  76,  61,  36,  36,  86,  44,  44,  65,   0,   0,
     67,  36,  36,  87,   7,   7, 108, 183,  36,  36,  36,  36,  36,  61, 184,  75,
     36,  36,  36,  36,  90,  73,  70,  82, 131,   0,   0,   0,   0,   0,  98,  41,
     36,  36,  67,  44, 185, 186,   0,   0,  81,  61,  81,  61,  81,  61,   0,   0,
     36,  61,  36,  61,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  24,  15,
     15,  39,   0,   0,  15,  15,  15,  15,  67,  44, 187,  87,   7,   7, 108,   0,
     36,   0,   0,   0,  36,  36,  36,  36,  36,  61,  98,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36,   0,  36,  36,  36,  41,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  41,   0,  15,  24,   0,   0, 188,  15,   0, 189,
     36,  36,  88,  36,  36,  61,  36,  43,  95,  88,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  41,   0,   0,   0,   0,   0,   0,   0,  98,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36, 190,  36,  36,  36,  36,  40,  36,  36,  36,
     36,  36,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  36,  36,  36,   0,
     44,  44,  44,  44, 191,   4, 124,   0,  44,  44,  44,  44, 192, 171, 144, 144,
    144, 193, 124,   0,   6, 194, 195, 196, 142,   0,   0,   0,  36,  88,  36,  36,
     36,  36,  36,  36,  36,  36,  36, 197,  57,   0,   5,   6,   0,   0, 198,   9,
     14,  15,  15,  15,  15,  15,  16, 199, 200, 201,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36,  82,  40,  36,  40,  36,  40,  36,  40,  97,
      0,   0,   0,   0,   0,   0, 202,   0,  36,  36,  36,  81,  36,  36,  36,  36,
     36,  61,  36,  36,  36,  36,  61,  95,  36,  36,  36,  41,  36,  36,  36,  41,
     36,  36,  36,  36,  36,  97,   0,   0,   0,   0,   0,   0,   0,   0,   0, 100,
     36,  36,  36,  36,  97,   0,   0,   0, 113,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,  81,  36,  36,  36,  36,  36,  36,  61,   0,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  82,  65,   0,  36,  36,  36,  36,  36,  36,  36,  41,
     36,   0,  36,  36,  81,  41,   0,   0,  11,  11,  15,  15,  15,  15,  15,  15,
     15,  15,  15,  15,  36,  36,  36,  36,   7,   7, 108,   0,  11,  11,  11,  11,
     11,  11,  11,  11,  11,   0,  15,  15,  15,  15,  15,  15,  15,  15,  15,   0,
     36,  36,   0,   0,  36,  36,  36,  36,  36,   0,   0,   0,   0,   0,   0,   0,
     36,  41,  88,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  95,  97,  77,
     36,  36,  36,  36,  61,  41,   0,   0,  36,  36,  36,  36,  36,  36,   0,  40,
     86,  60,   0,  44,  36,  81,  81,  36,  36,  36,  36,  36,  36,   0,  65,  94,
      0,   0,   0,   0,   0, 133,   0,   0,  36, 186,   0,   0,   0,   0,   0,   0,
     36,  36,  36,  36,  61,   0,   0,   0,  36,  36,  97,   0,   0,   0,   0,   0,
     11,  11,  11,  11,  22,   0,   0,   0,  15,  15,  15,  15,  24,   0,   0,   0,
     36,  36,  36,  36,  36,  36,  44,  44,  44, 187, 119,   0,   0,   0,   0,   0,
      0,  96,   7,   7,   0,   0,   0,  94,  36,  36,  36,  36,  44,  44,  65, 203,
    149,   0,   0,   0,  36,  36,  36,  36,  36,  36,  97,   0,   7,   7, 108,   0,
     36,  67,  44,  44,  44, 204,   7,   7, 183,   0,   0,   0,  36,  36,  36,  36,
     36,  36,  36,  36,  67, 105,   0,   0,  70, 205, 103, 206,   7,   7, 207, 173,
     36,  36,  36,  36,  95,  36,  36,  36,  36,  36,  36,  44,  44,  44, 208, 209,
     36,  61,  88,  95,  36,  36,  36,  95,  36,  36, 210,   0,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36,  67,  44,  44,  65,   0,   7,   7, 108,   0,
     44,  81,  36,  77,  77,  36,  36,  36,  44,  92,  92,  87,  97,  94,   0,  81,
     82, 103,  44, 113,  44, 113,   0,   0,  36,  36,  36,  36,  36,  86,  44,  44,
     44, 114, 211, 119,   7,   7, 108,   0,  44,  95,   0,   0,   7,   7, 108,   0,
     36,  36,  36,  67,  44,  87,  44,  44, 212,   0, 183, 132, 132, 132,  36,  87,
    125,  97,   0,   0,   7,   7, 108,   0,  36,  36,  67,  44,  44,  44,   0,   0,
     36,  36,  36,  36,  36,  36,  41,  58,  44,  44,  44,   0,   7,   7, 108,  78,
      7,   7, 108,   0,   0,   0,   0,  98,  86,  44, 114,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  67,  44,  85,  65, 133,  94,   0,   0,  86,  44,  44,  36,
     36,  40,  82,  44,  44,  44, 177, 119,  36,  36,  36,  36,  36,  36,  97,   0,
     36,  36,  88,  36,  36,  36,  36,  36,  36,  36,  36,  67,  44,  65,  44,  44,
    205,   0,   0,   0,   7,   7, 108,   0,   0,   0,   0,   0,  40,  36,  36,  36,
     36,  36,  36,  36, 103,  44,  44,  44,  44,  44,  58,  44,  44,  65,   0,   0,
     36,  61,  95,  36,  36,  36,  36,  36,  36,  36,  36,  36,  86,  65, 106,  59,
     44,  85,   0,   0,   7,   7, 108,   0,  36,  61,   0,   0,   0,   0,   0,   0,
      7,   7, 108, 133,   0,   0,   0,   0,  36,  36,  36,  41,  44, 206,   0,   0,
     36,  36,  36,  36,  44, 187, 119,   0,  36, 119,   0,   0,   7,   7, 108,   0,
     98,  36,  36,  36,  36,  36,   0,  81,  36,  97,   0,   0,  86,  44,  44,  44,
     44,  44,  44,  44,  44,  44,  44,  65,   0,   0,   0,  94, 114,  36,  36,  36,
     41,   0,   0,   0,   0,   0,   0,   0,  36,  36,  61,   0,  36,  36,  36,  97,
     36,  36,  97,   0,  36,  36,  41, 213,  62,   0,   0,   0,   0,   0,   0,   0,
      0,  58,  87,  58, 214,  62, 215,  44,  65,  58,  44,   0,   0,   0,   0,   0,
      0,   0, 103,  87,   0,   0,   0,   0, 103, 113,   0,   0,   0,   0,   0,   0,
     11,  11,  11,  11,  11,  11, 156,  15,  15,  15,  15,  15,  15,  11,  11,  11,
     11,  11,  11, 156,  15, 136,  15,  15,  15,  15,  11,  11,  11,  11,  11,  11,
    156,  15,  15,  15,  15,  15,  15,  49,  48, 216,  10,  49,  11, 156, 167,  14,
     15,  14,  15,  15,  11,  11,  11,  11,  11,  11, 156,  15,  15,  15,  15,  15,
     15,  50,  22,  10,  11,  49,  11, 217,  15,  15,  15,  15,  15,  15,  50,  22,
     11, 157, 163,  11, 217,  15,  15,  15,  15,  15,  15,  11,  11,  11,  11,  11,
     11, 156,  15,  15,  15,  15,  15,  15,  11,  11,  11, 156,  15,  15,  15,  15,
    156,  15,  15,  15,  15,  15,  15,  11,  11,  11,  11,  11,  11, 156,  15,  15,
     15,  15,  15,  15,  11,  11,  11,  11,  15,  39,  11,  11,  11,  11,  11,  11,
    217,  15,  15,  15,  15,  15,  24,  15,  33,  11,  11,  11,  11,  11,  22,  15,
     15,  15,  15,  15,  15, 136,  15,  11,  11,  11,  11,  11,  11, 217,  15,  15,
     15,  15,  15,  24,  15,  33,  11,  11,  15,  15, 136,  15,  11,  11,  11,  11,
     11,  11, 217,  15,  15,  15,  15,  15,  24,  15,  27,  96,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,   7,   7,  44,  44,  44,  44,  44,  65,  94,  44,
     44,  44,  44, 113,   0, 100,   0,   0,   0, 113, 119,   0,   0,   0,  94,  44,
     58,  44,  44,  44,   0,   0,   0,   0,  44,  65,  44,  44,  44,  44,  92,  44,
     59,  74,  65,   0,   0,   0,   0,   0,  36,  97,   0,   0,  44,  65,   0,   0,
    156,  15,  15,  15,  15,  15,  15,  15,  15,  44,  65,   0,   7,   7, 108,   0,
     36,  81,  36,  36,  36,  36,  36,  36,  99,  77,  81,  36,  61,  36, 109,   0,
    105,  98, 109,  81,  99,  77, 109, 109,  99,  77,  61,  36,  61,  36,  81,  43,
     36,  36,  95,  36,  36,  36,  36,   0,  81,  81,  95,  36,  36,  36,  36,   0,
      0,   0,   0,   0,  11,  11,  11,  11,  11,  11, 120,   0,  11,  11,  11,  11,
     11,  11, 120,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 164, 124,   0,
     41,   0,   0,   0,  36,  36,  36,  36,  97,   0,   0,   0,   0,   0,   0,   0,
     20,   0,   0,   0,   0,   0,   0,   0,  44,  44,  44,  44,   0,   0,   0,   0,
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
     3,  3,  3,  3,  9,  0,  8,  7,  0,  0,  7,  7,  7,  7,  0,  8,
     0,  0,  8,  0,  8,  0,  8,  8,  8,  8,  0,  8,  7,  7,  7,  8,
     8,  7,  0,  8,  8,  7,  0,  3,  3,  3,  8,  7,  0,  9,  0,  0,
     0, 14,  0,  0,  0, 12,  0,  0,  0,  3,  3,  3,  3,  3,  0,  3,
     0,  3,  3,  0,  9,  9,  9,  0,  5,  5,  5,  5,  5,  5,  0,  0,
    14, 14,  0,  0,  3,  3,  3,  0,  5,  0,  0, 12,  9,  9,  9,  3,
    10, 10,  0, 10, 10,  0,  9,  9,  3,  9,  9,  9, 12,  9,  3,  3,
     3,  5,  0,  3,  3,  9,  9,  3,  3,  0,  3,  3,  3,  3,  9,  9,
    10, 10,  9,  9,  9,  0,  0,  9, 12, 12, 12,  0,  0,  0,  0,  5,
     9,  3,  9,  9,  0,  9,  9,  9,  9,  9,  3,  3,  3,  9,  0,  0,
    14, 12,  9,  0,  3,  3,  9,  3,  9,  3,  3,  3,  3,  3,  0,  0,
     9,  0,  9,  9,  3,  3,  5,  3,  3,  9,  3,  3, 12, 12, 10, 10,
     3,  0,  0,  3,  3,  3,  9,  0,  0,  0,  0,  3,  9,  9,  0,  9,
     0,  0, 10, 10,  9,  0,  0,  0,  0,  0,  0,  9,  0,  9,  9,  0,
     0,  3,  0,  0,  9,  3,  0,  0,  0,  9,  3,  3,  0,  0,  3,  3,
     0,  0,  3,  9,  0,  0,  9,  0,  0,  0,  3,  0,  3,  0,  3,  0,
    10, 10,  0,  0,  0,  9,  0,  9,  0,  3,  0,  3,  0,  3, 13, 13,
    13, 13,  3,  3,  3,  0,  0,  0,  3,  3,  3,  9, 10, 10, 12, 12,
    10, 10,  3,  3,  0,  8,  0,  0,  0,  0, 12,  0, 12,  0,  0,  0,
     8,  8,  0,  0,  9,  0, 12,  9,  6,  9,  9,  9,  9,  9,  9, 13,
    13,  0,  0,  0,  3, 12, 12,  0,  9,  0,  3,  3,  0,  0, 14, 12,
    14, 12,  0,  3,  3,  3,  5,  0,  9,  3,  3,  9,  9,  3,  9,  0,
    12, 12, 12, 12,  0,  0, 12, 12,  9,  9, 12, 12,  0,  8,  0,  8,
     7,  0,  7,  7,  8,  0,  7,  0,  8,  0,  0,  0,  6,  6,  6,  6,
     6,  6,  6,  5,  3,  3,  5,  5,  0,  0,  0, 14, 14,  0,  0,  0,
    13, 13, 13, 13, 11,  0,  0,  0,  4,  4,  5,  5,  5,  5,  5,  6,
     0, 13, 13,  0, 12, 12,  0,  0,  0, 13, 13, 12,  0,  0,  0,  6,
     5,  0,  5,  5,  0, 13, 13,  7,  0,  0,  0,  8,  0,  0,  7,  8,
     8,  8,  7,  7,  8,  0,  8,  0,  8,  8,  0,  7,  9,  7,  0,  0,
     0,  8,  7,  7,  0,  0,  7,  0,  9,  9,  9,  8,  0,  0,  8,  8,
     0,  0, 13, 13,  8,  7,  7,  8,  7,  8,  7,  3,  7,  7,  0,  7,
     0,  0, 12,  9,  0,  0, 13,  0,  6, 14, 12,  0,  0, 13, 13, 13,
     9,  9,  0, 12,  9,  0, 12, 12,  8,  7,  9,  3,  3,  3,  0,  9,
     7,  7,  3,  3,  3,  3,  0, 12,  0,  0,  8,  7,  9,  0,  0,  8,
     7,  8,  7,  9,  7,  7,  7,  9,  9,  9,  3,  9,  0, 12, 12, 12,
     0,  0,  9,  3, 12, 12,  9,  9,  9,  3,  3,  0,  3,  3,  3, 12,
     0,  0,  0,  7,  0,  9,  3,  9,  9,  9, 13, 13, 14, 14,  0, 14,
     0, 14, 14,  0, 13,  0,  0, 13,  0, 14, 12, 12, 14, 13, 13, 13,
    13, 13, 13,  0,  9,  0,  0,  5,  0,  0, 14,  0,  0, 13,  0, 13,
    13, 12, 13, 13, 14,  0,  9,  9,  0,  5,  5,  5,  0,  5, 12, 12,
     3,  0, 10, 10,  9, 12, 12,  0,  3, 12,  0,  0, 10, 10,  9,  0,
    12, 12,  0, 12, 12,  0,  3,  0,  9, 12,  0,  0,  9,  9,  9, 12,
     3,  0, 12, 12,  0,  3,  3, 12,  3,  3,  3,  5,  5,  5,  5,  3,
     0,  8,  8,  0,  8,  0,  7,  7,
};

/* Sentence_Break: 6776 bytes. */

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
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 13, 28, 29, 30, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 31,  7, 32, 33,  7, 34,  7,  7,  7, 35, 13, 36,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_alphabetic_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,   1,  17,  18,  19,   1,  20,  21,  22,  23,  24,  25,  26,  27,   1,  28,
     29,  30,  31,  31,  32,  31,  31,  31,  31,  31,  31,  31,  33,  34,  35,  31,
     36,  37,  31,  31,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,  38,   1,   1,   1,   1,   1,   1,   1,   1,   1,  39,
      1,   1,   1,   1,  40,   1,  41,  42,  43,  44,  45,  46,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  47,  31,  31,  31,  31,  31,  31,  31,  31,
     31,   1,  48,  49,   1,  50,  51,  52,  53,  54,  55,  56,  57,  58,   1,  59,
     60,  61,  62,  63,  64,  31,  31,  31,  65,  66,  67,  68,  69,  70,  71,  72,
     73,  31,  74,  31,  75,  76,  31,  31,   1,   1,   1,  77,  78,  79,  31,  31,
      1,   1,   1,   1,  80,  31,  31,  31,  31,  31,  31,  31,   1,   1,  81,  31,
      1,   1,  82,  83,  31,  31,  31,  84,   1,   1,   1,   1,   1,   1,   1,  85,
      1,   1,  86,  31,  31,  31,  31,  31,   1,  87,  88,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  89,  31,  31,  31,  31,  31,  31,  31,  90,  91,  92,  93,
     94,  31,  31,  31,  31,  31,  31,  31,  95,  96,  31,  31,  31,  31,  97,  31,
     31,  98,  31,  31,  31,  31,  31,  31,   1,   1,   1,   1,   1,   1,  99,   1,
      1,   1,   1,   1,   1,   1,   1, 100, 101,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1, 102,   1,   1,   1,   1, 103,  31,  31,  31,  31,
      1,   1, 104,  31,  31,  31,  31,  31,
};

static RE_UINT8 re_alphabetic_stage_4[] = {
      0,   0,   1,   1,   0,   2,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   5,   6,   0,   0,   7,   8,   9,  10,   4,  11,
      4,   4,   4,   4,  12,   4,   4,   4,   4,  13,  14,  15,  16,  17,  18,  19,
     20,   4,  21,  22,   4,   4,  23,  24,  25,   4,  26,   4,   4,  27,  28,  29,
     30,  31,  32,  33,   0,  34,  35,  36,   4,  37,  38,  39,  40,  41,  42,  43,
     44,  45,  46,  47,  48,  49,  50,  51,  52,  49,  53,  54,  55,  56,  57,   0,
     58,  59,  60,  61,  58,  62,  63,  64,  58,  65,  66,  67,  68,  69,  70,  71,
     15,  72,  73,   0,  74,  75,  76,   0,  77,   0,  78,  79,  80,  81,   0,   0,
      4,  82,  25,  83,  84,   4,  85,  86,   4,   4,  87,   4,  88,  89,  90,   4,
     91,   4,  92,   0,  93,   4,   4,  94,  15,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,  95,   1,   4,   4,  96,  97,  98,  98,  99,   4, 100, 101,   0,
      0,   4,   4, 102,   4, 103,   4, 104, 105, 106,  25, 107,   4, 108, 109,   0,
    110,   4, 105, 111,   0, 112,   0,   0,   4, 113, 114,   0,   4, 115,   4, 116,
      4, 104, 117, 118, 119,   0,   0, 120,   4,   4,   4,   4,   4,   4,   0, 121,
     94,   4, 122, 118,   4, 123, 124, 125,   0,   0,   0, 126, 127,   0,   0,   0,
    128, 129, 130,   4, 119,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 131,   4, 109,   4, 132, 105,   4,   4,   4,   4, 133,
      4,  85,   4, 134, 135, 136, 136,   4,   0, 137,   0,   0,   0,   0,   0,   0,
    138, 139,  15,   4, 140,  15,   4,  86, 141, 142,   4,   4, 143,  72,   0,  25,
      4,   4,   4,   4,   4, 104,   0,   0,   4,   4,   4,   4,   4,   4,   4,  33,
      4,   4,   4,   4,  31,   0,  25, 118, 144, 145,   4, 146,   4,   4,   4,  93,
    147, 148,   4,   4, 149, 150,   0, 147, 151,  16,   4,  98,   4,   4, 152, 153,
     28, 103, 154,  81,   4, 155, 137, 156,   4, 135, 157, 158,   4, 105, 159, 160,
    161, 162,  86, 163,   4,   4,   4,  33,   4,   4,   4,   4,   4, 164, 165, 110,
      4,   4,   4, 166,   4,   4, 167,   0, 168, 169, 170,   4,   4,  27, 171,   4,
      4, 118,  25,   4, 172,   4,  16, 173,   0,   0,   0, 174,   4,   4,   4,  81,
      0,   1,   1, 175,   4, 105, 176,   0, 177, 178, 179,   0,   4,   4,   4,  72,
      0,   0,   4, 180,   0,   0,   0,   0,   0,   0,   0,   0,  81,   4, 181,   0,
      4,  26, 103,  72, 118,   4, 182,   0,   4,   4,   4,   4, 118,  25, 183, 110,
      4, 184,   4,  61,   0,   0,   0,   0,   4, 135, 104,  16,   0,   0,   0,   0,
    185, 186, 104, 135, 105,   0,   0, 187, 104, 167,   0,   0,   4, 188,   0,   0,
    189,  98,   0,  81,  81,   0,  78, 190,   4, 104, 104, 154,  27,   0,   0,   0,
      4,   4, 119,   0,   4, 154,   4, 154,   4,   4, 191,   0, 148,  32,  25, 119,
      4, 154,  25, 192,   4,   4, 193,   0, 194, 195,   0,   0, 196, 197,   4, 119,
     40,  49, 198,  61,   0,   0,   0,   0,   4,   4, 199,   0,   4,   4, 200,   0,
      0,   0,   0,   0,   4, 201, 202,   0,   4, 105, 203,   0,   4, 104,   0,   0,
    204,  33,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4, 205,
      4, 206,  25,   4, 207,   0,   4,  32, 208, 209,  77, 210, 172, 211,   0,   0,
    212, 213, 214,   0,   0,   0,   0,   0,   4,   4,   4,   4, 167,   0,   0,   0,
      4,   4,   4, 143,   4,   4,   4,   4,   4,   4,  61,   0,   0,   0,   0,   0,
      4, 143,   0,   0,   0,   0,   0,   0,   4,   4, 215,   0,   0,   0,   0,   0,
      4,  32, 105,   0,   0,   0,  25, 157,   4, 135,  61, 216,  93,   0,   0,   0,
      4,   4, 217, 105, 171,   0,   0, 218,   4,   4,   4,   4,   4,   4,   4,  31,
      4,   4,   4,   4,   4,   4,   4, 154, 105,   0,   0,  25,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4, 110,   4,   4,   4, 219, 220,   0,   0,   0,
      4,   4, 221,   4, 222, 223, 224,   4, 225, 226, 227,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4, 228, 229,  86, 221, 221, 132, 132, 208, 208, 230,   0,
    231, 232,   0,   0,   0,   0,   0,   0,   4,   4,   4,   4,   4,   4, 190,   0,
      4,   4, 233,   0,   0,   0,   0,   0, 224, 234, 235, 236, 237, 238,   0,   0,
      0,  25, 239, 239, 109,   0,   0,   0,   4,   4,   4,   4,   4,   4, 135,   0,
      4, 180,   4,   4,   4,   4,   4,   4, 118,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4, 240,   4,   4,   4,   4,   4,   4,   4,   4,   4,  77,
    118,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_alphabetic_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,  32,   0,   0,   0,
      0,   0, 223, 188,  64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,
      3, 252, 255, 255, 255, 255, 254, 255, 255, 255, 127,   2, 254, 255, 255, 255,
    255,   0,   0,   0,   0,   0, 255, 191, 182,   0, 255, 255, 255,   7,   7,   0,
      0,   0, 255,   7, 255, 255, 255, 254,   0, 192, 255, 255, 255, 255, 239,  31,
    254, 225,   0, 156,   0,   0, 255, 255,   0, 224, 255, 255, 255, 255,   3,   0,
      0, 252, 255, 255, 255,   7,  48,   4, 255, 255, 255, 252, 255,  31,   0,   0,
    255, 255, 255,   1, 255,   7,   0,   0, 255, 255, 223,  63,   0,   0, 240, 255,
    248,   3, 255, 255, 255, 255, 255, 239, 255, 223, 225, 255,  15,   0, 254, 255,
    239, 159, 249, 255, 255, 253, 197, 227, 159,  89, 128, 176,  15,   0,   3,  16,
    238, 135, 249, 255, 255, 253, 109, 195, 135,  25,   2,  94,   0,   0,  63,   0,
    238, 191, 251, 255, 255, 253, 237, 227, 191,  27,   1,   0,  15,   0,   0,  30,
    238, 159, 249, 255, 159,  25, 192, 176,  15,   0,   2,   0, 236, 199,  61, 214,
     24, 199, 255, 195, 199,  29, 129,   0, 239, 223, 253, 255, 255, 253, 255, 227,
    223,  29,  96,   7,  15,   0,   0,   0, 255, 253, 239, 227, 223,  29,  96,  64,
     15,   0,   6,   0, 255, 255, 255, 231, 223,  93, 240, 128,  15,   0,   0, 252,
    236, 255, 127, 252, 255, 255, 251,  47, 127, 128,  95, 255,   0,   0,  12,   0,
    255, 255, 255,   7, 127,  32,   0,   0, 150,  37, 240, 254, 174, 236, 255,  59,
     95,  32,   0, 240,   1,   0,   0,   0, 255, 254, 255, 255, 255,  31, 254, 255,
      3, 255, 255, 254, 255, 255, 255,  31, 255, 255, 127, 249, 231, 193, 255, 255,
    127,  64,   0,  48, 191,  32, 255, 255, 255, 255, 255, 247, 255,  61, 127,  61,
    255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255, 255, 255,  61, 255,
    255, 255, 255, 135, 255, 255,   0,   0, 255, 255,  63,  63, 255, 159, 255, 255,
    255, 199, 255,   1, 255, 223,  15,   0, 255, 255,  15,   0, 255, 223,  13,   0,
    255, 255, 207, 255, 255,   1, 128,  16, 255, 255, 255,   0, 255,   7, 255, 255,
    255, 255,  63,   0, 255, 255, 255, 127, 255,  15, 255,   1, 255,  63,  31,   0,
    255,  15, 255, 255, 255,   3,   0,   0, 255, 255, 255,  15, 254, 255,  31,   0,
    128,   0,   0,   0, 255, 255, 239, 255, 239,  15,   0,   0, 255, 243,   0, 252,
    191, 255,   3,   0,   0, 224,   0, 252, 255, 255, 255,  63, 255,   1,   0,   0,
      0, 222, 111,   0, 128, 255,  31,   0,  63,  63, 255, 170, 255, 255, 223,  95,
    220,  31, 207,  15, 255,  31, 220,  31,   0,   0,   2, 128,   0,   0, 255,  31,
    132, 252,  47,  62,  80, 189, 255, 243, 224,  67,   0,   0,   0,   0, 192, 255,
    255, 127, 255, 255,  31, 120,  12,   0, 255, 128,   0,   0, 255, 255, 127,   0,
    127, 127, 127, 127,   0, 128,   0,   0, 224,   0,   0,   0, 254,   3,  62,  31,
    255, 255, 127, 224, 224, 255, 255, 255, 255, 127, 254, 255, 255, 127,   0,   0,
    255,  31, 255, 255,   0,  12,   0,   0, 255, 127, 240, 143,   0,   0, 128, 255,
    252, 255, 255, 255, 255, 249, 255, 255, 255, 127, 255,   0, 187, 247, 255, 255,
     47,   0,   0,   0,   0,   0, 252,  40, 255, 255,   7,   0, 255, 255, 247, 255,
    223, 255,   0, 124, 255,  63,   0,   0, 255, 255, 127, 196,   5,   0,   0,  56,
    255, 255,  60,   0, 126, 126, 126,   0, 127, 127, 255, 255,  63,   0, 255, 255,
     15,   0, 255, 255, 127, 248, 255, 255, 255,  63, 255, 255, 255, 255, 255,   3,
    127,   0, 248, 224, 255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255,
    255, 255, 252, 255,   0,   0, 255,  15,   0,   0, 223, 255, 192, 255, 255, 255,
    252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63,
    255, 255,  31,   0, 255, 255,   1,   0,  15, 255,  62,   0, 255, 255,  15, 255,
    255,   0, 255, 255,  63, 253, 255, 255, 255, 255, 191, 145, 255, 255,  55,   0,
    255, 255, 255, 192, 111, 240, 239, 254,  31,   0,   0,   0,  63,   0,   0,   0,
    255, 255,  71,   0,  30,   0,   0,  20, 255, 255, 251, 255, 255, 255, 159,  64,
    127, 189, 255, 191, 255,   1, 255, 255, 159,  25, 129, 224, 187,   7,   0,   0,
    179,   0,   0,   0, 255, 255,  63, 127,   0,   0,   0,  63,  17,   0,   0,   0,
    255, 255, 255, 227,   0,   0,   0, 128, 255, 255, 231, 127, 207, 255, 255,   0,
    255, 253, 255, 255, 255, 255, 127, 127,   0,   0, 252, 255, 255, 254, 127,   0,
    127, 251, 255, 255, 255, 255, 127, 180, 203,   0,   0,   0, 127,   0,   0,   0,
    248, 255, 255, 224,  31,   0, 255, 255,   3,   0,   0,   0, 255,   7, 255,  31,
    255,   1, 255,  67, 255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235,
    239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,
     63, 255, 255, 255, 253, 255, 255, 247, 247,  15,   0,   0, 127, 255, 255, 249,
    219,   7,   0,   0, 143,   0,   0,   0, 150, 254, 247,  10, 132, 234, 150, 170,
    150, 247, 247,  94, 255, 251, 255,  15, 238, 251, 255,  15, 255,   3, 255, 255,
      3,   0, 255, 255,
};

/* Alphabetic: 2245 bytes. */

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
     0,  1,  2,  3,  4,  5,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  6,  7,  1,  1,  1,  1,  1,  1,  1,  1,  1,  8,
     9, 10,  1, 11,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 12,  1,  1, 13,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_lowercase_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  7,  6,  6,  6,  6,  6,  6,  6,  6,  8,  9, 10, 11,
    12, 13,  6,  6, 14,  6,  6,  6,  6,  6,  6,  6, 15, 16,  6,  6,
     6,  6,  6,  6,  6,  6, 17, 18,  6,  6,  6, 19,  6,  6,  6,  6,
     6,  6,  6, 20,  6,  6,  6, 21,  6,  6,  6,  6, 22,  6,  6,  6,
     6,  6,  6,  6, 23,  6,  6,  6, 24,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6, 25, 26, 27, 28,  6, 29,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_lowercase_stage_4[] = {
     0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
     5, 13, 14, 15, 16, 17, 18, 19,  0,  0, 20, 21, 22, 23, 24, 25,
     0, 26, 15,  5, 27,  5, 28,  5,  5, 29,  0, 30, 31,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 32,
     0,  0,  0,  0, 33,  0,  0,  0, 15, 15, 15, 15, 15, 15,  0,  0,
     5,  5,  5,  5, 34,  5,  5,  5, 35, 36, 37, 38, 36, 39, 40, 41,
     0,  0,  0, 42, 43,  0,  0,  0, 44, 45, 46, 26, 47,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 26, 48,  0, 26, 49, 50,  5,  5,  5, 51,
    15, 52,  0,  0,  0,  0,  0,  0,  0,  0,  5, 53, 54,  0,  0,  0,
     0, 55,  5, 56, 57, 58,  0, 59,  0, 26, 60, 61, 15, 15,  0,  0,
    62,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
     0, 63, 64,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0, 15, 67,
     0,  0,  0,  0,  0,  0, 15,  0, 68, 69, 70, 31, 71, 72, 73, 74,
    75, 76, 77, 78, 79, 68, 69, 80, 31, 71, 81, 64, 74, 82, 83, 84,
    85, 81, 86, 26, 87, 74, 88,  0,  0, 89, 90,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_lowercase_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4,   0,   0,   0, 128,
    255, 255, 127, 255, 170, 170, 170, 170, 170, 170, 170,  85,  85, 171, 170, 170,
    170, 170, 170, 212,  41,  49,  36,  78,  42,  45,  81, 230,  64,  82,  85, 181,
    170, 170,  41, 170, 170, 170, 250, 147, 133, 170, 255, 255, 255, 255, 255, 255,
    255, 255, 239, 255, 255, 255, 255,   1,   3,   0,   0,   0,  31,   0,   0,   0,
     32,   0,   0,   0,   0,   0, 138,  60,   0,   0,   1,   0,   0, 240, 255, 255,
    255, 127, 227, 170, 170, 170,  47,  25,   0,   0, 255, 255,   2, 168, 170, 170,
     84, 213, 170, 170, 170, 170,   0,   0, 254, 255, 255, 255, 255,   0,   0,   0,
      0,   0,   0,  63, 255,   1,   0,   0, 170, 170, 234, 191, 255,   0,  63,   0,
    255,   0, 255,   0,  63,   0, 255,   0, 255,   0, 255,  63, 255,   0, 223,  64,
    220,   0, 207,   0, 255,   0, 220,   0,   0,   0,   2, 128,   0,   0, 255,  31,
      0, 196,   8,   0,   0, 128,  16,  50, 192,  67,   0,   0,  16,   0,   0,   0,
    255,   3,   0,   0, 255, 255, 255, 127,  98,  21, 218,  63,  26,  80,   8,   0,
    191,  32,   0,   0, 170,  42,   0,   0, 170, 170, 170,  58, 168, 170, 171, 170,
    170, 170, 255, 149, 170,  80, 186, 170, 170,   2, 160,   0,   0,   0,   0,   7,
    255, 255, 255, 247,  63,   0, 255, 255, 127,   0, 248,   0,   0, 255, 255, 255,
    255, 255,   0,   0,   0,   0,   0, 255, 255, 255, 255,  15, 255, 255,   7,   0,
      0,   0,   0, 252, 255, 255,  15,   0,   0, 192, 223, 255, 252, 255, 255,  15,
      0,   0, 192, 235, 239, 255,   0,   0,   0, 252, 255, 255,  15,   0,   0, 192,
    255, 255, 255,   0,   0,   0, 252, 255, 255,  15,   0,   0, 192, 255, 255, 255,
      0, 192, 255, 255,   0,   0, 192, 255,  63,   0,   0,   0, 252, 255, 255, 247,
      3,   0,   0, 240, 255, 255, 223,  15, 255, 127,  63,   0, 255, 253,   0,   0,
    247,  11,   0,   0, 252, 255, 255, 255,  15,   0,   0,   0,
};

/* Lowercase: 829 bytes. */

RE_UINT32 re_get_lowercase(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_lowercase_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_lowercase_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_lowercase_stage_3[pos + f] << 3;
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
     0,  1,  2,  3,  4,  5,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  6,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  7,
     8,  9,  1, 10,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 11,  1,  1, 12, 13,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_uppercase_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  8,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  9, 10,
     6, 11,  6,  6, 12,  6,  6,  6,  6,  6,  6,  6, 13,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 14, 15,  6,  6,  6,  6,  6,  6,  6, 16,
     6,  6,  6,  6, 17,  6,  6,  6,  6,  6,  6,  6, 18,  6,  6,  6,
    19,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 20, 21, 22, 23,
     6, 24,  6,  6,  6,  6,  6,  6,  6, 25,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_uppercase_stage_4[] = {
     0,  0,  1,  0,  0,  0,  2,  0,  3,  4,  5,  6,  7,  8,  9, 10,
     3, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0, 13, 14, 15, 16, 17,
    18, 19,  0,  3, 20,  3, 21,  3,  3, 22, 23,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18, 24,  0,
     0,  0,  0,  0,  0, 18, 18, 25,  3,  3,  3,  3, 26,  3,  3,  3,
    27, 28, 29, 30,  0, 31, 32, 33, 34, 35, 36, 19, 37,  0,  0,  0,
     0,  0,  0,  0,  0, 38, 19,  0, 18, 39,  0, 40,  3,  3,  3, 41,
     0,  0,  3, 42, 43,  0,  0,  0,  0, 44,  3, 45, 46, 47,  0,  0,
     0,  1,  0,  0,  0,  0,  0,  0, 18, 48,  0,  0,  0, 49, 50,  0,
     0,  0,  0,  0, 18, 51,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,
    52, 53, 54, 55, 56, 57, 49, 58, 59, 60, 61, 62, 63, 52, 53, 54,
    55, 64, 25, 49, 58, 55, 65, 66, 67, 68, 38, 39, 49, 69, 70,  0,
    18, 71,  0,  0,  0,  0,  0,  0,  0, 49, 72, 72, 58,  0,  0,  0,
};

static RE_UINT8 re_uppercase_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7, 255, 255, 127, 127,  85,  85,  85,  85,
     85,  85,  85, 170, 170,  84,  85,  85,  85,  85,  85,  43, 214, 206, 219, 177,
    213, 210, 174,  17, 144, 164, 170,  74,  85,  85, 210,  85,  85,  85,   5, 108,
    122,  85,   0,   0,   0,   0,  69, 128,  64, 215, 254, 255, 251,  15,   0,   0,
      0, 128,  28,  85,  85,  85, 144, 230, 255, 255, 255, 255, 255, 255,   0,   0,
      1,  84,  85,  85, 171,  42,  85,  85,  85,  85, 254, 255, 255, 255, 127,   0,
    191,  32,   0,   0, 255, 255,  63,   0,  85,  85,  21,  64,   0, 255,   0,  63,
      0, 255,   0, 255,   0,  63,   0, 170,   0, 255,   0,   0,   0,   0,   0,  15,
      0,  15,   0,  15,   0,  31,   0,  15, 132,  56,  39,  62,  80,  61,  15, 192,
     32,   0,   0,   0,   8,   0,   0,   0,   0,   0, 192, 255, 255, 127,   0,   0,
    157, 234,  37, 192,   5,  40,   4,   0,  85,  21,   0,   0,  85,  85,  85,   5,
     84,  85,  84,  85,  85,  85,   0, 106,  85,  40,  69,  85,  85, 125,  95,   0,
    255,   0,   0,   0,   0,   0, 255, 255, 255, 255,  15,   0, 255, 255,   7,   0,
    255, 255, 255,   3,   0,   0, 240, 255, 255,  63,   0,   0,   0, 255, 255, 255,
      3,   0,   0, 208, 100, 222,  63,   0, 255,   3,   0,   0, 176, 231, 223,  31,
      0,   0,   0, 123,  95, 252,   1,   0,   0, 240, 255, 255,  63,   0,   0,   0,
      3,   0,   0, 240,   1,   0,   0,   0, 252, 255, 255,   7,   0,   0,   0, 240,
    255, 255,  31,   0, 255,   1,   0,   0,   0,   4,   0,   0,   3,   0,   0,   0,
    255,   3, 255, 255,
};

/* Uppercase: 725 bytes. */

RE_UINT32 re_get_uppercase(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_uppercase_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_uppercase_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_uppercase_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_uppercase_stage_4[pos + f] << 5;
    pos += code;
    value = (re_uppercase_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Cased. */

static RE_UINT8 re_cased_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_cased_stage_2[] = {
     0,  1,  2,  2,  3,  2,  2,  4,  5,  6,  2,  7,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  8,  9,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 10, 11,
     2, 12,  2, 13,  2,  2, 14,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2, 15,  2,  2,  2,  2, 16,  2, 17,  2,  2,  2,
};

static RE_UINT8 re_cased_stage_3[] = {
     0,  1,  2,  3,  2,  4,  5,  6,  2,  7,  8,  9, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 12,
    10, 13,  2, 14,  2,  2, 15, 16, 17, 18, 19, 20, 10, 10, 10, 10,
    10, 21, 10, 10, 10, 10, 10, 10, 22, 23, 24, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 25, 26, 27, 28, 10, 10, 10, 10, 10, 10, 29, 14,
    10, 10, 10, 10, 10, 10, 30, 10, 10, 10, 10, 10, 10, 10, 31, 10,
    32, 33, 10, 10, 10, 10, 10, 10, 10, 34, 10, 10, 10, 10, 10, 10,
    10, 35, 10, 10, 10, 10, 10, 10, 36, 37, 38,  2,  2, 39, 40, 41,
    10, 10, 42, 10, 10, 10, 10, 10, 10, 10, 43, 44, 10, 10, 10, 10,
};

static RE_UINT8 re_cased_stage_4[] = {
     0,  0,  1,  1,  0,  2,  3,  3,  4,  4,  4,  4,  4,  5,  6,  4,
     7,  8,  9, 10,  0,  0, 11, 12, 13, 14,  4, 15, 16,  4,  4,  4,
     4, 17, 18, 19, 20,  0,  0,  0,  0,  0,  0,  0,  0,  4, 21,  0,
     0,  4,  4, 22, 23,  0,  0,  0,  4,  4,  0,  0, 22,  4, 24, 25,
     4, 26, 27, 28,  0,  0,  0, 29, 30,  0,  0,  0, 31, 32, 33,  4,
    34,  0,  0,  0,  0, 35,  4, 36,  4, 37, 38,  4,  4,  4,  4, 39,
     4, 21,  0,  0,  0,  0,  4, 40, 25,  0,  0,  0,  0, 41,  4,  4,
    42, 43,  0, 44,  0, 45,  5, 46, 47,  0,  0,  0,  0,  1,  1,  0,
     4,  4, 48,  0,  0, 45, 49, 50,  4, 51,  4, 51,  0,  4,  4,  0,
     4,  4, 52,  4, 53, 54, 55,  4, 56, 57, 58,  4,  4, 59, 60,  5,
    52, 52, 37, 37, 61, 61, 62,  0,  4,  4, 63,  0,  0, 45, 64, 64,
    36,  0,  0,  0,
};

static RE_UINT8 re_cased_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 255, 255, 255, 247, 240, 255, 255, 255, 255, 255, 239, 255,
    255, 255, 255,   1,   3,   0,   0,   0,  31,   0,   0,   0,  32,   0,   0,   0,
      0,   0, 207, 188,  64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,
      3, 252, 255, 255, 255, 255, 254, 255, 255, 255, 127,   0, 254, 255, 255, 255,
    255,   0,   0,   0, 191,  32,   0,   0, 255, 255,  63,  63, 255,   1,   0,   0,
     63,  63, 255, 170, 255, 255, 255,  63, 255, 255, 223,  95, 220,  31, 207,  15,
    255,  31, 220,  31,   0,   0,   2, 128,   0,   0, 255,  31, 132, 252,  47,  62,
     80, 189,  31, 242, 224,  67,   0,   0,  24,   0,   0,   0,   0,   0, 192, 255,
    255,   3,   0,   0, 255, 127, 255, 255, 255, 255, 255, 127,  31, 120,  12,   0,
    255,  63,   0,   0, 252, 255, 255, 255, 255, 120, 255, 255, 255, 127, 255,   0,
      0,   0,   0,   7,   0,   0, 255, 255,  63,   0, 255, 255, 127,   0, 248,   0,
    255, 255,   0,   0, 255, 255,  15, 255, 255, 255, 255,  15, 255, 255,   7,   0,
    255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255,
    191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255,
    253, 255, 255, 247, 255, 253, 255, 255, 247,  15,   0,   0,  15,   0,   0,   0,
    255,   3, 255, 255,
};

/* Cased: 748 bytes. */

RE_UINT32 re_get_cased(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_cased_stage_1[f] << 4;
    f = code >> 10;
    code ^= f << 10;
    pos = (RE_UINT32)re_cased_stage_2[pos + f] << 3;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_cased_stage_3[pos + f] << 2;
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
    11, 12, 13, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7, 15,  7,  7,
     7,  7,  7,  7,  7,  7,  7, 16,  7,  7, 17, 18, 19, 20, 21,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
    22,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
};

static RE_UINT8 re_case_ignorable_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1,  1, 17,  1,  1,  1, 18, 19, 20, 21, 22, 23, 24,  1, 25,
    26,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 27, 28, 29,  1,
    30,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    31,  1,  1,  1, 32,  1, 33, 34, 35, 36, 37, 38,  1,  1,  1,  1,
     1,  1,  1, 39,  1,  1, 40, 41,  1, 42, 43, 44,  1,  1,  1,  1,
     1,  1, 45,  1,  1,  1,  1,  1, 46, 47, 48, 49, 50, 51, 52, 53,
     1,  1, 54,  1, 55, 56,  1,  1,  1,  1, 57, 58,  1,  1,  1, 59,
     1,  1,  1,  1, 60,  1,  1,  1,  1, 61, 62,  1,  1,  1,  1,  1,
     1,  1, 63,  1,  1,  1,  1,  1, 64,  1,  1,  1,  1,  1,  1,  1,
    65, 66,  1,  1,  1,  1,  1,  1,  1,  1,  1, 67,  1,  1,  1,  1,
    68, 69,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_case_ignorable_stage_4[] = {
      0,   1,   2,   3,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   5,   6,   6,   6,   6,   6,   7,   8,   0,   0,   0,
      0,   0,   0,   0,   9,   0,   0,   0,   0,   0,  10,   0,  11,  12,  13,  14,
     15,   0,  16,  17,   0,   0,  18,  19,  20,   5,  21,   0,   0,  22,   0,  23,
     24,  25,  26,   0,   0,   0,  27,   6,  28,  29,  30,  31,  32,  33,  34,  35,
     36,  33,  37,  38,  36,  33,  39,  40,  32,  41,  42,  35,  43,   0,  44,   0,
      3,  45,  46,  35,  32,  41,  47,  35,  48,  49,  34,  35,   0,   0,  50,   0,
      0,  51,  52,   0,   0,  53,  54,   0,  55,  56,   0,  57,  58,  59,  60,   0,
      0,  61,  62,  63,  64,   0,   0,  33,   0,   0,  65,   0,   0,   0,   0,   0,
     66,  66,  67,  67,   0,  68,  69,   0,  70,   0,  71,   0,  72,  73,   0,   0,
      0,  74,   0,   0,   0,   0,   0,   0,  75,   0,  76,  77,   0,  78,   0,   0,
     79,  80,  43,  81,  48,  82,   0,  83,   0,  84,   0,  85,   0,   0,  86,  87,
      0,  88,   6,  89,  90,   6,   6,  91,   0,   0,   0,   0,   0,  92,  93,  94,
     95,  96,   0,  97,  98,   0,   5,  99,   0,   0,   0, 100,   0,   0,   0, 101,
      0,   0,   0, 102,   0,   0,   0,   6,   0, 103,   0,   0,   0,   0,   0,   0,
    104, 105,   0,   0, 106,   0,   0, 107, 108,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  85, 109,   0,   0, 110, 111,   0,   0, 112,
      6,  48,   0,  17, 113,   0,   0,  55, 114,  72,   0,   0,   0,   0, 115, 116,
      0, 117, 118,   0,  28, 119, 103,  72,   0, 120, 121, 122,   0, 123, 124, 125,
      0,   0,  90,   0,   0,   0,   0, 126,   2,   0,   0,   0,   0, 127,  48,   0,
    128, 129, 130,   0,   0,   0,   0, 131,   1,   2,   3,  17,  45,   0,   0, 132,
      0,   0,   0,   0,   0,   0,   0, 133,   0,   0,   0,   0,   0,   0,   0,   3,
      0,   0,   0, 134,   0,   0,   0,   0, 135, 136,   0,   0,   0,   0,   0,  72,
     32, 137, 138, 131,  48, 139,   0,   0,  28, 140,   0, 141,  48, 142, 143,   0,
      0, 144,   0,   0,   0,   0, 131, 145,  48,  33,   3, 146,   0,   0,   0,   0,
      0, 137, 147,   0,   0, 148, 149,   0,   0,   0,   0,   0,   0, 150, 151,   0,
      0, 152,   3,   0,   0, 153,   0,   0,  65, 154,   0,   0,   0,   0,   0,   0,
    155, 156, 157,   0, 158,   0,   0,   0,   0, 159,   0,   0, 127, 160,   0,   0,
      0, 161, 162,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 163,
      0, 164,  79,   0,   0,   0,   0,   0,   0,   0,   0,   0, 165,   0,   0,  48,
      0,   0,   0,   0, 166,  79,   0,   0,   0,   0,   0, 167, 168, 169,   0,   0,
      0,   0, 170,   0,   0,   0,   0,   0,   6, 171,   6, 172, 173, 174,   0,   0,
    175, 176,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 164,   0,
      0,   0, 177,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  90,
     32,   6,   6,   6,   0,   0,   0,   0,   6,   6,   6,   6,   6,   6,   6, 129,
};

static RE_UINT8 re_case_ignorable_stage_5[] = {
      0,   0,   0,   0, 128,  64,   0,   4,   0,   0,   0,  64,   1,   0,   0,   0,
      0, 161, 144,   1,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255,  48,   4,
    176,   0,   0,   0, 248,   3,   0,   0,   0,   0,   0,   2,   0,   0, 254, 255,
    255, 255, 255, 191, 182,   0,   0,   0,   0,   0,  16,   0,  63,   0, 255,  23,
      1, 248, 255, 255,   0,   0,   1,   0,   0,   0, 192, 191, 255,  61,   0,   0,
      0, 128,   2,   0, 255,   7,   0,   0, 192, 255,   1,   0,   0, 248,  63,   4,
      0,   0, 192, 255, 255,  63,   0,   0,   0,   0,   0,  14,   0,   0, 240, 255,
      7,   0,   0,   0,   0,   0,   0,  20, 254,  33, 254,   0,  12,   0,   2,   0,
      2,   0,   0,   0,   0,   0,   0,  16,  30,  32,   0,   0,  12,   0,   0,   0,
      6,   0,   0,   0, 134,  57,   2,   0,   0,   0,  35,   0, 190,  33,   0,   0,
     12,   0,   0, 252,   0,   0,   0, 144,  30,  32,  64,   0,   4,   0,   0,   0,
      1,  32,   0,   0,   0,   0,   0, 192, 193,  61,  96,   0,  64,  48,   0,   0,
      3,   0,   0,   0,   0,   0,   0,  24,   0,   4,  92,   0,   0,   0, 242,   7,
    192, 127,   0,   0,   0,   0, 242,  27,  64,  63,   0,   0,   0,   0,   0,   3,
      0,   0, 160,   2,   0,   0, 254, 127, 223, 224, 255, 254, 255, 255, 255,  31,
     64,   0,   0,   0,   0, 224, 253, 102,   0,   0,   0, 195,   1,   0,  30,   0,
    100,  32,   0,  32,   0,   0,   0, 224,   0,   0,  28,   0,   0,   0,  12,   0,
      0,   0, 176,  63,  64, 254, 143,  32,   0, 120,   0,   0,   8,   0,   0,   0,
     96,   0,   0,   0,   0,   2,   0,   0, 135,   1,   4,  14,   0,   0, 128,   9,
      0,   0,  64, 127, 229,  31, 248, 159, 128,   0, 255, 127,  15,   0,   0,   0,
      0,   0, 208,  23,   0, 248,  15,   0,  60,  59,   0,   0,  64, 163,   3,   0,
      0, 240, 207,   0,   0,   0,   0,  63,   0,   0, 247, 255, 253,  33,  16,   3,
      0, 240, 255, 255, 255,   7,   0,   1,   0,   0,   0, 248, 255, 255, 255, 251,
      0,   0,   0, 160,   3, 224,   0, 224,   0, 224,   0,  96,   0, 248,   0,   3,
    144, 124,   0,   0, 223, 255,   2, 128,   0,   0, 255,  31, 255, 255,   1,   0,
      0,   0,   0,  48,   0, 128,   3,   0,   0, 128,   0, 128,   0, 128,   0,   0,
     32,   0,   0,   0,   0,  60,  62,   8,   0,   0,   0, 126,   0,   0,   0, 112,
      0,   0,  32,   0,   0,  16,   0,   0,   0, 128, 247, 191,   0,   0,   0, 240,
      0,   0,   3,   0,   0,   7,   0,   0,  68,   8,   0,   0,  48,   0,   0,   0,
    255, 255,   3,   0, 192,  63,   0,   0, 128, 255,   3,   0,   0,   0, 200,  19,
      0, 126, 102,   0,   8,  16,   0,   0,   0,   0,   1,  16,   0,   0, 157, 193,
      2,   0,   0,  32,   0,  48,  88,   0,  32,  33,   0,   0,   0,   0, 252, 255,
    255, 255,   8,   0, 255, 255,   0,   0,   0,   0,  36,   0,   0,   0,   0, 128,
      8,   0,   0,  14,   0,   0,   0,  32,   0,   0, 192,   7, 110, 240,   0,   0,
      0,   0,   0, 135,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0, 120,  38,
    128, 239,  31,   0,   0,   0,   8,   0,   0,   0, 192, 127,   0,  28,   0,   0,
      0, 128, 211,  64, 248,   7,   0,   0, 192,  31,  31,   0,  92,   0,   0,   0,
      0,   0, 248, 133,  13,   0,   0,   0,   0,   0,  60, 176,   1,   0,   0,  48,
      0,   0, 248, 167,   0,  40, 191,   0, 188,  15,   0,   0, 126,   6,   0,   0,
      0,   0, 248, 121, 128,   0, 126,  14,   0, 252, 127,   3,   0,   0, 127, 191,
    255, 252, 109,   0,   0,   0, 126, 180, 191,   0,   0,   0,   0,   0,  31,   0,
      0,   0, 127,   0,   0, 128, 255, 255,   0,   0,   0,  96, 128,   3, 248, 255,
    231,  15,   0,   0,   0,  60,   0,   0,  28,   0,   0,   0, 255, 255, 127, 248,
    255,  31,  32,   0,  16,   0,   0, 248, 254, 255,   0,   0, 127, 255, 255, 249,
    219,   7,   0,   0, 240,   7,   0,   0,
};

/* Case_Ignorable: 1586 bytes. */

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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_changes_when_lowercased_stage_2[] = {
     0,  1,  2,  3,  4,  5,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  6,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  7,
     8,  9,  1, 10,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 11,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_changes_when_lowercased_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  8,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  9, 10,
     6, 11,  6,  6, 12,  6,  6,  6,  6,  6,  6,  6, 13,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 14, 15,  6,  6,  6,  6,  6,  6,  6, 16,
     6,  6,  6,  6, 17,  6,  6,  6,  6,  6,  6,  6, 18,  6,  6,  6,
    19,  6,  6,  6,  6,  6,  6,  6,  6, 20,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_changes_when_lowercased_stage_4[] = {
     0,  0,  1,  0,  0,  0,  2,  0,  3,  4,  5,  6,  7,  8,  9, 10,
     3, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0, 13, 14, 15, 16, 17,
    18, 19,  0,  3, 20,  3, 21,  3,  3, 22, 23,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18, 24,  0,
     0,  0,  0,  0,  0, 18, 18, 25,  3,  3,  3,  3, 26,  3,  3,  3,
    27, 28, 29, 30, 28, 31, 32, 33,  0, 34,  0, 19, 35,  0,  0,  0,
     0,  0,  0,  0,  0, 36, 19,  0, 18, 37,  0, 38,  3,  3,  3, 39,
     0,  0,  3, 40, 41,  0,  0,  0,  0, 42,  3, 43, 44, 45,  0,  0,
     0,  1,  0,  0,  0,  0,  0,  0, 18, 46,  0,  0,  0, 47, 48,  0,
     0,  0,  0,  0, 18, 49,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,
    18, 50,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_lowercased_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7, 255, 255, 127, 127,  85,  85,  85,  85,
     85,  85,  85, 170, 170,  84,  85,  85,  85,  85,  85,  43, 214, 206, 219, 177,
    213, 210, 174,  17, 176, 173, 170,  74,  85,  85, 214,  85,  85,  85,   5, 108,
    122,  85,   0,   0,   0,   0,  69, 128,  64, 215, 254, 255, 251,  15,   0,   0,
      0, 128,   0,  85,  85,  85, 144, 230, 255, 255, 255, 255, 255, 255,   0,   0,
      1,  84,  85,  85, 171,  42,  85,  85,  85,  85, 254, 255, 255, 255, 127,   0,
    191,  32,   0,   0, 255, 255,  63,   0,  85,  85,  21,  64,   0, 255,   0,  63,
      0, 255,   0, 255,   0,  63,   0, 170,   0, 255,   0,   0,   0, 255,   0,  31,
      0,  31,   0,  15,   0,  31,   0,  31,  64,  12,   4,   0,   8,   0,   0,   0,
      0,   0, 192, 255, 255, 127,   0,   0, 157, 234,  37, 192,   5,  40,   4,   0,
     85,  21,   0,   0,  85,  85,  85,   5,  84,  85,  84,  85,  85,  85,   0, 106,
     85,  40,  69,  85,  85, 125,  95,   0, 255,   0,   0,   0,   0,   0, 255, 255,
    255, 255,  15,   0, 255, 255,   7,   0,   3,   0,   0,   0,
};

/* Changes_When_Lowercased: 581 bytes. */

RE_UINT32 re_get_changes_when_lowercased(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_changes_when_lowercased_stage_1[f] << 5;
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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_changes_when_uppercased_stage_2[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 5,
    6, 7, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

static RE_UINT8 re_changes_when_uppercased_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  7,  6,  6,  6,  6,  6,  6,  6,  6,  8,  9, 10, 11,
     6, 12,  6,  6, 13,  6,  6,  6,  6,  6,  6,  6, 14, 15,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 16, 17,  6,  6,  6, 18,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 19,  6,  6,  6, 20,
     6,  6,  6,  6, 21,  6,  6,  6,  6,  6,  6,  6, 22,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6, 23,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6, 24,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_changes_when_uppercased_stage_4[] = {
     0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
     5, 13, 14, 15, 16,  0,  0,  0,  0,  0, 17, 18, 19, 20, 21, 22,
     0, 23, 24,  5, 25,  5, 26,  5,  5, 27,  0, 28, 29,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,
     0,  0,  0,  0, 31,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,
     5,  5,  5,  5, 33,  5,  5,  5, 34, 35, 36, 37, 24, 38, 39, 40,
     0,  0, 41, 23, 42,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23, 43,
     0, 23, 44, 45,  5,  5,  5, 46, 24, 47,  0,  0,  0,  0,  0,  0,
     0,  0,  5, 48, 49,  0,  0,  0,  0, 50,  5, 51, 52, 53,  0,  0,
     0,  0, 54, 23, 24, 24,  0,  0, 55,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  1,  0,  0,  0,  0,  0,  0, 56, 57,  0,  0,  0, 58, 59,
     0,  0,  0,  0,  0,  0, 24, 60,  0,  0,  0,  0,  0,  0, 24,  0,
     0, 61, 62,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_uppercased_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   0,  32,   0,   0,   0,   0, 128,
    255, 255, 127, 255, 170, 170, 170, 170, 170, 170, 170,  84,  85, 171, 170, 170,
    170, 170, 170, 212,  41,  17,  36,  70,  42,  33,  81, 162,  96,  91,  85, 181,
    170, 170,  45, 170, 168, 170,  10, 144, 133, 170, 223,  26, 107, 159,  38,  32,
    137,  31,   4,  96,  32,   0,   0,   0,   0,   0, 138,  56,   0,   0,   1,   0,
      0, 240, 255, 255, 255, 127, 227, 170, 170, 170,  47,   9,   0,   0, 255, 255,
    255, 255, 255, 255,   2, 168, 170, 170,  84, 213, 170, 170, 170, 170,   0,   0,
    254, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,  63, 255,   1,   0,   0,
      0,   0,   0,  34, 170, 170, 234,  15, 255,   0,  63,   0, 255,   0, 255,   0,
     63,   0, 255,   0, 255,   0, 255,  63, 255, 255, 223,  80, 220,  16, 207,   0,
    255,   0, 220,  16,   0,  64,   0,   0,  16,   0,   0,   0, 255,   3,   0,   0,
    255, 255, 255, 127,  98,  21,  72,   0,  10,  80,   8,   0, 191,  32,   0,   0,
    170,  42,   0,   0, 170, 170, 170,  10, 168, 170, 168, 170, 170, 170,   0, 148,
    170,  16, 138, 170, 170,   2, 160,   0,   0,   0,   8,   0, 127,   0, 248,   0,
      0, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 255, 255, 255, 255,  15,
    255, 255,   7,   0, 252, 255, 255, 255,  15,   0,   0,   0,
};

/* Changes_When_Uppercased: 661 bytes. */

RE_UINT32 re_get_changes_when_uppercased(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_changes_when_uppercased_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_changes_when_uppercased_stage_2[pos + f] << 4;
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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_changes_when_titlecased_stage_2[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 5,
    6, 7, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

static RE_UINT8 re_changes_when_titlecased_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  7,  6,  6,  6,  6,  6,  6,  6,  6,  8,  9, 10, 11,
     6, 12,  6,  6, 13,  6,  6,  6,  6,  6,  6,  6, 14, 15,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 16, 17,  6,  6,  6, 18,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 19,  6,  6,  6, 20,
     6,  6,  6,  6, 21,  6,  6,  6,  6,  6,  6,  6, 22,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6, 23,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6, 24,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_changes_when_titlecased_stage_4[] = {
     0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
     5, 13, 14, 15, 16,  0,  0,  0,  0,  0, 17, 18, 19, 20, 21, 22,
     0, 23, 24,  5, 25,  5, 26,  5,  5, 27,  0, 28, 29,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,
     0,  0,  0,  0, 31,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,
     5,  5,  5,  5, 33,  5,  5,  5, 34, 35, 36, 37, 35, 38, 39, 40,
     0,  0, 41, 23, 42,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23, 43,
     0, 23, 44, 45,  5,  5,  5, 46, 24, 47,  0,  0,  0,  0,  0,  0,
     0,  0,  5, 48, 49,  0,  0,  0,  0, 50,  5, 51, 52, 53,  0,  0,
     0,  0, 54, 23, 24, 24,  0,  0, 55,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  1,  0,  0,  0,  0,  0,  0, 56, 57,  0,  0,  0, 58, 59,
     0,  0,  0,  0,  0,  0, 24, 60,  0,  0,  0,  0,  0,  0, 24,  0,
     0, 61, 62,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_titlecased_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   0,  32,   0,   0,   0,   0, 128,
    255, 255, 127, 255, 170, 170, 170, 170, 170, 170, 170,  84,  85, 171, 170, 170,
    170, 170, 170, 212,  41,  17,  36,  70,  42,  33,  81, 162, 208,  86,  85, 181,
    170, 170,  43, 170, 168, 170,  10, 144, 133, 170, 223,  26, 107, 159,  38,  32,
    137,  31,   4,  96,  32,   0,   0,   0,   0,   0, 138,  56,   0,   0,   1,   0,
      0, 240, 255, 255, 255, 127, 227, 170, 170, 170,  47,   9,   0,   0, 255, 255,
    255, 255, 255, 255,   2, 168, 170, 170,  84, 213, 170, 170, 170, 170,   0,   0,
    254, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,  63, 255,   1,   0,   0,
      0,   0,   0,  34, 170, 170, 234,  15, 255,   0,  63,   0, 255,   0, 255,   0,
     63,   0, 255,   0, 255,   0, 255,  63, 255,   0, 223,  64, 220,   0, 207,   0,
    255,   0, 220,   0,   0,  64,   0,   0,  16,   0,   0,   0, 255,   3,   0,   0,
    255, 255, 255, 127,  98,  21,  72,   0,  10,  80,   8,   0, 191,  32,   0,   0,
    170,  42,   0,   0, 170, 170, 170,  10, 168, 170, 168, 170, 170, 170,   0, 148,
    170,  16, 138, 170, 170,   2, 160,   0,   0,   0,   8,   0, 127,   0, 248,   0,
      0, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 255, 255, 255, 255,  15,
    255, 255,   7,   0, 252, 255, 255, 255,  15,   0,   0,   0,
};

/* Changes_When_Titlecased: 661 bytes. */

RE_UINT32 re_get_changes_when_titlecased(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_changes_when_titlecased_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_changes_when_titlecased_stage_2[pos + f] << 4;
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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_changes_when_casefolded_stage_2[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 5,
    6, 7, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

static RE_UINT8 re_changes_when_casefolded_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  8,  6,  6,  6,  6,  6,  6,  6,  6,  9,  6, 10, 11,
     6, 12,  6,  6, 13,  6,  6,  6,  6,  6,  6,  6, 14,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 15, 16,  6,  6,  6, 17,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 18,  6,  6,  6, 19,
     6,  6,  6,  6, 20,  6,  6,  6,  6,  6,  6,  6, 21,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6, 22,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6, 23,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_changes_when_casefolded_stage_4[] = {
     0,  0,  1,  0,  0,  2,  3,  0,  4,  5,  6,  7,  8,  9, 10, 11,
     4, 12, 13,  0,  0,  0,  0,  0,  0,  0, 14, 15, 16, 17, 18, 19,
    20, 21,  0,  4, 22,  4, 23,  4,  4, 24, 25,  0, 26,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 20, 27,  0,
     0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0, 29,  0,  0,  0,
     4,  4,  4,  4, 30,  4,  4,  4, 31, 32, 33, 34, 20, 35, 36, 37,
     0, 38,  0, 21, 39,  0,  0,  0,  0,  0,  0,  0,  0, 40, 21,  0,
    20, 41,  0, 42,  4,  4,  4, 43,  0,  0,  4, 44, 45,  0,  0,  0,
     0, 46,  4, 47, 48, 49,  0,  0,  0,  0,  0, 50, 20, 20,  0,  0,
    51,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
    20, 52,  0,  0,  0, 50, 53,  0,  0,  0,  0,  0, 20, 54,  0,  0,
     0,  0,  0,  0,  0, 20,  0,  0, 20, 55,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_casefolded_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   0,  32,   0, 255, 255, 127, 255,
     85,  85,  85,  85,  85,  85,  85, 170, 170,  86,  85,  85,  85,  85,  85, 171,
    214, 206, 219, 177, 213, 210, 174,  17, 176, 173, 170,  74,  85,  85, 214,  85,
     85,  85,   5, 108, 122,  85,   0,   0,  32,   0,   0,   0,   0,   0,  69, 128,
     64, 215, 254, 255, 251,  15,   0,   0,   4, 128,  99,  85,  85,  85, 179, 230,
    255, 255, 255, 255, 255, 255,   0,   0,   1,  84,  85,  85, 171,  42,  85,  85,
     85,  85, 254, 255, 255, 255, 127,   0, 128,   0,   0,   0, 191,  32,   0,   0,
      0,   0,   0,  63, 255,   1,   0,   0,  85,  85,  21,  76,   0, 255,   0,  63,
      0, 255,   0, 255,   0,  63,   0, 170,   0, 255,   0,   0, 255, 255, 156,  31,
    156,  31,   0,  15,   0,  31, 156,  31,  64,  12,   4,   0,   8,   0,   0,   0,
      0,   0, 192, 255, 255, 127,   0,   0, 157, 234,  37, 192,   5,  40,   4,   0,
     85,  21,   0,   0,  85,  85,  85,   5,  84,  85,  84,  85,  85,  85,   0, 106,
     85,  40,  69,  85,  85, 125,  95,   0,   0,   0, 255, 255, 127,   0, 248,   0,
    255,   0,   0,   0, 255, 255,  15,   0, 255, 255,   7,   0,   3,   0,   0,   0,
};

/* Changes_When_Casefolded: 625 bytes. */

RE_UINT32 re_get_changes_when_casefolded(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_changes_when_casefolded_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_changes_when_casefolded_stage_2[pos + f] << 4;
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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_changes_when_casemapped_stage_2[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 5,
    6, 7, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

static RE_UINT8 re_changes_when_casemapped_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  8,  6,  6,  6,  6,  6,  6,  6,  6,  9, 10, 11, 12,
     6, 13,  6,  6, 14,  6,  6,  6,  6,  6,  6,  6, 15, 16,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 17, 18,  6,  6,  6, 19,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 20,  6,  6,  6, 21,
     6,  6,  6,  6, 22,  6,  6,  6,  6,  6,  6,  6, 23,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6, 24,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6, 25,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_changes_when_casemapped_stage_4[] = {
     0,  0,  1,  1,  0,  2,  3,  3,  4,  5,  4,  4,  6,  7,  8,  4,
     4,  9, 10, 11, 12,  0,  0,  0,  0,  0, 13, 14, 15, 16, 17, 18,
     4,  4,  4,  4, 19,  4,  4,  4,  4, 20, 21, 22, 23,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4, 24,  0,
     0,  0,  0,  0,  0,  4,  4, 25,  0,  0,  0,  0, 26,  0,  0,  0,
     0,  0,  0, 27,  0,  0,  0,  0,  4,  4,  4,  4, 28,  4,  4,  4,
    25,  4, 29, 30,  4, 31, 32, 33,  0, 34, 35,  4, 36,  0,  0,  0,
     0,  0,  0,  0,  0, 37,  4, 38,  4, 39, 40, 41,  4,  4,  4, 42,
     4, 24,  0,  0,  0,  0,  0,  0,  0,  0,  4, 43, 44,  0,  0,  0,
     0, 45,  4, 46, 47, 48,  0,  0,  0,  0, 49, 50,  4,  4,  0,  0,
    51,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
     4,  4, 52,  0,  0, 50, 53, 44,  0,  0,  0,  0,  4, 54,  4, 54,
     0,  0,  0,  0,  0,  4,  4,  0,  4,  4, 55,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_changes_when_casemapped_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   0,  32,   0, 255, 255, 127, 255,
    255, 255, 255, 255, 255, 255, 255, 254, 255, 223, 255, 247, 255, 243, 255, 179,
    240, 255, 255, 255, 253, 255,  15, 252, 255, 255, 223,  26, 107, 159,  38,  32,
    137,  31,   4,  96,  32,   0,   0,   0,   0,   0, 207, 184,  64, 215, 255, 255,
    251, 255, 255, 255, 255, 255, 227, 255, 255, 255, 191, 239,   3, 252, 255, 255,
    255, 255, 254, 255, 255, 255, 127,   0, 254, 255, 255, 255, 255,   0,   0,   0,
    191,  32,   0,   0, 255, 255,  63,  63, 255,   1,   0,   0,   0,   0,   0,  34,
    255, 255, 255,  79,  63,  63, 255, 170, 255, 255, 255,  63, 255, 255, 223,  95,
    220,  31, 207,  15, 255,  31, 220,  31,  64,  12,   4,   0,   0,  64,   0,   0,
     24,   0,   0,   0,   0,   0, 192, 255, 255,   3,   0,   0, 255, 127, 255, 255,
    255, 255, 255, 127, 255, 255, 109, 192,  15, 120,  12,   0, 255,  63,   0,   0,
    255, 255, 255,  15, 252, 255, 252, 255, 255, 255,   0, 254, 255,  56, 207, 255,
    255, 127, 255,   0,   0,   0,   8,   0,   0,   0, 255, 255, 127,   0, 248,   0,
    255, 255,   0,   0, 255, 255,  15, 255, 255, 255,   7,   0,  15,   0,   0,   0,
};

/* Changes_When_Casemapped: 641 bytes. */

RE_UINT32 re_get_changes_when_casemapped(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_changes_when_casemapped_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_changes_when_casemapped_stage_2[pos + f] << 4;
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
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 13, 13, 28, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 29,  7, 30, 31,  7, 32,  7,  7,  7, 33, 13, 34,
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
    31,  1, 46, 47,  1, 48, 49, 50, 51, 52, 53, 54, 55, 56,  1, 57,
    58, 59, 60, 61, 62, 31, 31, 31, 63, 64, 65, 66, 67, 68, 69, 70,
    71, 31, 72, 31, 73, 74, 31, 31,  1,  1,  1, 75, 76, 77, 31, 31,
     1,  1,  1,  1, 78, 31, 31, 31, 31, 31, 31, 31,  1,  1, 79, 31,
     1,  1, 80, 81, 31, 31, 31, 82,  1,  1,  1,  1,  1,  1,  1, 83,
     1,  1, 84, 31, 31, 31, 31, 31,  1, 85, 86, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 87, 31, 31, 31, 31, 31, 31, 31, 88, 89, 90, 91,
    92, 77, 31, 31, 31, 31, 93, 31,  1,  1,  1,  1,  1,  1, 94,  1,
     1,  1,  1,  1,  1,  1,  1, 95, 96,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1, 97,  1,  1,  1,  1, 98, 31, 31, 31, 31,
     1,  1, 99, 31, 31, 31, 31, 31,
};

static RE_UINT8 re_id_start_stage_4[] = {
      0,   0,   1,   1,   0,   2,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   5,   6,   0,   0,   0,   7,   8,   9,   4,  10,
      4,   4,   4,   4,  11,   4,   4,   4,   4,  12,  13,  14,  15,   0,  16,  17,
      0,   4,  18,  19,   4,   4,  20,  21,  22,  23,  24,   4,   4,  25,  26,  27,
     28,  29,  30,  18,   0,  31,   0,   0,  32,  33,  34,  35,  36,  37,  38,  39,
     40,  41,  42,  43,  44,  45,  46,  47,  48,  45,  49,  50,  51,  52,  46,   0,
     53,  54,  55,  56,  57,  58,  59,  60,  53,  61,  62,  63,  64,  65,  66,   0,
     14,  67,  66,   0,  68,  69,  70,   0,  71,   0,  72,  73,  74,   0,   0,   0,
      4,  75,  76,  77,  78,   4,  79,  80,   4,   4,  81,   4,  82,  83,  84,   4,
     85,   4,  86,   0,  23,   4,   4,  87,  14,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,  88,   1,   4,   4,  89,  90,  91,  91,  92,   4,  93,  94,   0,
      0,   4,   4,  95,   4,  96,   4,  97,  98,   0,  16,  99,   4, 100, 101,   0,
    102,   4, 103,   0,   0, 104,   0,   0, 105,  93, 106,   0, 107, 108,   4, 109,
      4, 110, 111, 112, 113,   0,   0, 114,   4,   4,   4,   4,   4,   4,   0,   0,
     87,   4, 115, 112,   4, 116, 117, 118,   0,   0,   0, 119, 120,   0,   0,   0,
    121, 122, 123,   4, 113,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      4, 124,  98,   4,   4,   4,   4, 125,   4,  79,   4, 126, 102, 127, 127,   0,
    128, 129,  14,   4, 130,  14,   4,  80, 105, 131,   4,   4, 132,  86,   0,  16,
      4,   4,   4,   4,   4,  97,   0,   0,   4,   4,   4,   4,   4,   4,   4,  18,
      4,   4,   4,   4,  73,   0,  16, 112, 133, 134,   4, 135, 112,   4,   4,  23,
    136, 137,   4,   4, 138, 139,   0, 136, 140, 141,   4,  93, 137,  93,   0, 142,
     26, 143,  66, 144,  32, 145, 146, 147,   4, 113, 148, 149,   4, 150, 151, 152,
    153, 154,  80, 143,   4,   4,   4, 141,   4,   4,   4,   4,   4, 155, 156, 157,
      4,   4,   4, 158,   4,   4, 159,   0, 160, 161, 162,   4,   4,  91, 163,   4,
      4, 112,  16,   4, 164,   4,  15, 165,   0,   0,   0, 166,   4,   4,   4, 144,
      0,   1,   1, 167,   4,  98, 168,   0, 169, 170, 171,   0,   4,   4,   4,  86,
      0,   0,   4, 103,   0,   0,   0,   0,   0,   0,   0,   0, 144,   4, 172,   0,
      4,  24, 173,  97, 112,   4, 174,   0,   4,   4,   4,   4, 112,  16, 175, 157,
      4, 176,   4, 110,   0,   0,   0,   0,   4, 102,  97,  15,   0,   0,   0,   0,
    177, 178,  97, 102,  98,   0,   0, 179,  97, 159,   0,   0,   4, 180,   0,   0,
    181,  93,   0, 144, 144,   0,  72, 182,   4,  97,  97, 145,  91,   0,   0,   0,
      4,   4, 113,   0,   4, 145,   4, 145, 107,  95,   0,   0, 107,  23,  16, 113,
    107,  66,  16, 183, 107, 145, 184,   0, 185, 186,   0,   0, 187, 188,  98,   0,
     48,  45, 189,  56,   0,   0,   0,   0,   4, 103, 190,   0,   4,  23, 191,   0,
      0,   0,   0,   0,   4, 132, 192,   0,   4,  23, 193,   0,   4,  18,   0,   0,
    159,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4, 194,
    195, 196, 197,   4, 198,   0,   4,  30, 199, 132,  71, 200,  23,   0,   0,   0,
    201, 172, 202,   0,   0,   0,   0,   0,   4,   4,   4,   4, 159,   0,   0,   0,
      4,   4,   4, 132,   4,   4,   4,   4,   4,   4, 110,   0,   0,   0,   0,   0,
      4, 132,   0,   0,   0,   0,   0,   0,   4,   4,  66,   0,   0,   0,   0,   0,
      4,  30,  98,   0,   0,   0,  16, 203,   4,  23, 110, 204,  23,   0,   0,   0,
      4,   4, 205,   0, 163,   0,   0,  56,   4,   4,   4,   4,   4,   4,   4,  73,
      4,   4,   4,   4,   4,   4,   4, 145,  98,   0,   0,  16,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4, 157,   4,   4,   4, 206, 207,   0,   0,   0,
      4,   4, 208,   4, 209, 210, 211,   4, 212, 213, 214,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4, 215, 216,  80, 208, 208, 124, 124, 199, 199, 148,   0,
      4,   4,   4,   4,   4,   4, 182,   0, 211, 217, 218, 219, 220, 221,   0,   0,
      4,   4,   4,   4,   4,   4, 102,   0,   4, 103,   4,   4,   4,   4,   4,   4,
    112,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 222,   4,   4,
      4,   4,   4,   4,   4,   4,   4,  71, 112,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_id_start_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,   0,   0, 223, 188,
     64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,   3, 252, 255, 255,
    255, 255, 254, 255, 255, 255, 127,   2, 254, 255, 255, 255, 255,   0,   0,   0,
      0,   0, 255, 255, 255,   7,   7,   0, 255,   7,   0,   0,   0, 192, 254, 255,
    255, 255,  47,   0,  96, 192,   0, 156,   0,   0, 253, 255, 255, 255,   0,   0,
      0, 224, 255, 255,  63,   0,   2,   0,   0, 252, 255, 255, 255,   7,  48,   4,
    255, 255,  63,   4,  16,   1,   0,   0, 255, 255, 255,   1, 255, 255, 223,  63,
    240, 255, 255, 255, 255, 255, 255,  35,   0,   0,   1, 255,   3,   0, 254, 255,
    225, 159, 249, 255, 255, 253, 197,  35,   0,  64,   0, 176,   3,   0,   3,  16,
    224, 135, 249, 255, 255, 253, 109,   3,   0,   0,   0,  94,   0,   0,  28,   0,
    224, 191, 251, 255, 255, 253, 237,  35,   0,   0,   1,   0,   3,   0,   0,   2,
    224, 159, 249, 255,   0,   0,   0, 176,   3,   0,   2,   0, 232, 199,  61, 214,
     24, 199, 255,   3, 224, 223, 253, 255, 255, 253, 255,  35,   0,   0,   0,   7,
      3,   0,   0,   0, 225, 223, 253, 255, 255, 253, 239,  35,   0,   0,   0,  64,
      3,   0,   6,   0, 255, 255, 255,  39,   0,  64, 112, 128,   3,   0,   0, 252,
    224, 255, 127, 252, 255, 255, 251,  47, 127,   0,   0,   0, 255, 255,  13,   0,
    150,  37, 240, 254, 174, 236,  13,  32,  95,   0,   0, 240,   1,   0,   0,   0,
    255, 254, 255, 255, 255,  31,   0,   0,   0,  31,   0,   0, 255,   7,   0, 128,
      0,   0,  63,  60,  98, 192, 225, 255,   3,  64,   0,   0, 191,  32, 255, 255,
    255, 255, 255, 247, 255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,
     61, 255, 127, 255, 255, 255,  61, 255, 255, 255, 255,   7, 255, 255,  63,  63,
    255, 159, 255, 255, 255, 199, 255,   1, 255, 223,   3,   0, 255, 255,   3,   0,
    255, 223,   1,   0, 255, 255,  15,   0,   0,   0, 128,  16, 255, 255, 255,   0,
    255,   5, 255, 255, 255, 255,  63,   0, 255, 255, 255, 127, 255,  63,  31,   0,
    255,  15, 255, 255, 255,   3,   0,   0, 255, 255, 127,   0, 255, 255,  31,   0,
    128,   0,   0,   0, 224, 255, 255, 255, 224,  15,   0,   0, 248, 255, 255, 255,
      1, 192,   0, 252,  63,   0,   0,   0,  15,   0,   0,   0,   0, 224,   0, 252,
    255, 255, 255,  63, 255,   1,   0,   0,   0, 222,  99,   0,  63,  63, 255, 170,
    255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,   0,   0,   2, 128,
      0,   0, 255,  31, 132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0,
    255, 127, 255, 255,  31, 120,  12,   0, 255, 128,   0,   0, 127, 127, 127, 127,
    224,   0,   0,   0, 254,   3,  62,  31, 255, 255, 127, 248, 255, 127, 254, 255,
    255, 127,   0,   0, 255,  31, 255, 255,   0,  12,   0,   0, 255, 127,   0, 128,
      0,   0, 128, 255, 252, 255, 255, 255, 255, 249, 255, 255, 255, 127, 255,   0,
    187, 247, 255, 255,   7,   0,   0,   0,   0,   0, 252,  40,  63,   0, 255, 255,
    255, 255, 255,  31, 255, 255,   7,   0,   0, 128,   0,   0, 223, 255,   0, 124,
    247,  15,   0,   0, 255, 255, 127, 196, 255, 255,  98,  62,   5,   0,   0,  56,
    255,   7,  28,   0, 126, 126, 126,   0, 127, 127, 255, 255,  15,   0, 255, 255,
    127, 248, 255, 255, 255, 255, 255,  15, 255,  63, 255, 255, 255, 255, 255,   3,
    127,   0, 248, 160, 255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255,
    255, 255, 252, 255,   0,   0, 255,  15,   0,   0, 223, 255, 192, 255, 255, 255,
    252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63,
    255, 255,   1,   0, 255,   7, 255, 255,  15, 255,  62,   0, 255, 255,  15, 255,
    255,   0, 255, 255,  63, 253, 255, 255, 255, 255, 191, 145, 255, 255,  55,   0,
    255, 255, 255, 192,   1,   0, 239, 254,  31,   0,   0,   0, 255, 255,  71,   0,
     30,   0,   0,  20, 255, 255, 251, 255, 255,  15,   0,   0, 127, 189, 255, 191,
    255,   1, 255, 255,   0,   0,   1, 224, 128,   7,   0,   0, 176,   0,   0,   0,
      0,   0,   0,  15,  16,   0,   0,   0,   0,   0,   0, 128,   1, 248, 255, 255,
    255, 255,   7,   4,   0,   0,   1, 240, 207,   3,   0,   0, 255, 253, 255, 255,
      0,   0, 252, 255, 127, 251, 255, 255,  64,   0,   0,   0, 255,  63,   0,   0,
    248, 255, 255, 224,  31,   0,   1,   0, 255,   7, 255,  31, 255,   1, 255,   3,
    255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255,
    191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255,
    253, 255, 255, 247, 150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94,
    255, 251, 255,  15, 238, 251, 255,  15,   3,   0, 255, 255,
};

/* ID_Start: 2117 bytes. */

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
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 28, 29, 30, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 31,  7, 32, 33,  7, 34,  7,  7,  7, 35, 13, 36,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    37, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_id_continue_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,   1,  17,  18,  19,   1,  20,  21,  22,  23,  24,  25,  26,  27,   1,  28,
     29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  32,  33,  31,  31,
     34,  35,  31,  31,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,  36,   1,   1,   1,   1,   1,   1,   1,   1,   1,  37,
      1,   1,   1,   1,  38,   1,  39,  40,  41,  42,  43,  44,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  45,  31,  31,  31,  31,  31,  31,  31,  31,
     31,   1,  46,  47,   1,  48,  49,  50,  51,  52,  53,  54,  55,  56,   1,  57,
     58,  59,  60,  61,  62,  31,  31,  31,  63,  64,  65,  66,  67,  68,  69,  70,
     71,  31,  72,  31,  73,  74,  31,  31,   1,   1,   1,  75,  76,  77,  31,  31,
      1,   1,   1,   1,  78,  31,  31,  31,  31,  31,  31,  31,   1,   1,  79,  31,
      1,   1,  80,  81,  31,  31,  31,  82,   1,   1,   1,   1,   1,   1,   1,  83,
      1,   1,  84,  31,  31,  31,  31,  31,   1,  85,  86,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  87,  31,  31,  31,  31,  88,  89,  31,  90,  91,  92,  93,
     31,  31,  94,  31,  31,  31,  31,  31,  95,  31,  31,  31,  31,  31,  31,  31,
     96,  97,  31,  31,  31,  31,  98,  31,   1,   1,   1,   1,   1,   1,  99,   1,
      1,   1,   1,   1,   1,   1,   1, 100, 101,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1, 102,   1,   1,   1,   1, 103,  31,  31,  31,  31,
      1,   1, 104,  31,  31,  31,  31,  31,  31, 105,  31,  31,  31,  31,  31,  31,
};

static RE_UINT8 re_id_continue_stage_4[] = {
      0,   1,   2,   3,   0,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   7,   8,   6,   6,   6,   9,  10,  11,   6,  12,
      6,   6,   6,   6,  13,   6,   6,   6,   6,  14,  15,  16,  17,  18,  19,  20,
     21,   6,   6,  22,   6,   6,  23,  24,  25,   6,  26,   6,   6,  27,   6,  28,
      6,  29,  30,  31,   0,  32,  33,  11,   6,   6,   6,  34,  35,  36,  37,  38,
     39,  40,  41,  42,  43,  44,  45,  46,  47,  44,  48,  49,  50,  51,  52,  53,
     54,  55,  56,  57,  54,  58,  59,  60,  54,   6,  61,  62,  63,  64,  65,  66,
     16,  67,  68,   0,  69,  70,  71,   0,  72,  73,  74,  75,  76,  77,  78,   0,
      6,   6,  79,   6,  80,   6,  81,  82,   6,   6,  83,   6,  84,  85,  86,   6,
     87,   6,  88,  89,  90,   6,   6,  91,  16,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,  92,   3,   6,   6,  93,  94,  95,  96,  97,   6,   6,  98,  99,
    100,   6,   6, 101,   6, 102,   6, 103, 104, 105, 106, 107,   6, 108, 109,   0,
     30,   6, 104, 110, 111, 112,   0,   0,   6,   6, 113, 114,   6,   6,   6,  96,
      6, 101, 115,  80, 116,   0, 117, 118,   6,   6,   6,   6,   6,   6,   6, 119,
     91,   6, 120,  80,   6, 121, 122, 123,   0, 124, 125, 126, 127,   0, 127, 128,
    129, 130, 131,   6, 116,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      6, 132, 104,   6,   6,   6,   6, 133,   6,  81,   6, 134, 135, 136, 136,   6,
    137, 138,  16,   6, 139,  16,   6,  82, 140, 141,   6,   6, 142,  67,   0,  25,
      6,   6,   6,   6,   6, 103,   0,   0,   6,   6,   6,   6,   6,   6,   6,  31,
      6,   6,   6,   6, 143,   0,  25,  80, 144, 145,   6, 146,   6,   6,   6,  27,
    147, 148,   6,   6, 149, 150,   0, 147,   6, 151,   6,  96,   6,   6, 152, 153,
      6, 154,  96,  77,   6,   6, 155, 104,   6, 135, 156, 157,   6,   6, 158, 159,
    160, 161,  82, 162,   6,   6,   6, 163,   6,   6,   6,   6,   6, 164, 165,  30,
      6,   6,   6, 154,   6,   6, 118,   0, 166, 167, 168,   6,   6,  27, 169,   6,
      6,  80,  25,   6, 170,   6, 151, 171,  90, 172, 173, 174,   6,   6,   6,  77,
      1,   2,   3, 106,   6, 104, 175,   0, 176, 177, 178,   0,   6,   6,   6,  67,
      0,   0,   6,  95,   0,   0,   0, 179,   0,   0,   0,   0,  77,   6, 180, 181,
      6, 182, 102,  67,  80,   6, 183,   0,   6,   6,   6,   6,  80,  79, 184,  30,
      6, 185,   6, 186,   0,   0,   0,   0,   6, 135, 103, 151,   0,   0,   0,   0,
    187, 188, 103, 135, 104,   0,   0, 189, 103, 118,   0,   0,   6, 190,   0,   0,
    191, 192,   0,  77,  77,   0,  74, 193,   6, 103, 103, 194,  27,   0,   0,   0,
      6,   6, 116,   0,   6, 194,   6, 194,   6,   6, 193, 195,   6,  67,  25, 196,
      6, 197,  25, 198,   6,   6, 199,   0, 200, 201,   0,   0, 202, 203,   6, 204,
     35,  44, 205, 206,   0,   0,   0,   0,   6,   6, 204,   0,   6,   6, 207,   0,
      0,   0,   0,   0,   6, 208, 209,   0,   6,   6, 210,   0,   6, 101,  99,   0,
    211, 113,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6,   6, 212,
      6, 104, 213,   6, 214,   0,   6, 215, 216,   5, 217, 218, 170, 219,   0,   0,
    220, 221, 222,   0,   0,   0,   0,   0,   6,   6,   6,   6, 118,   0,   0,   0,
      6,   6,   6, 142,   6,   6,   6,   6,   6,   6, 186,   0,   0,   0,   0,   0,
      6, 142,   0,   0,   0,   0,   0,   0,   6,   6, 193,   0,   0,   0,   0,   0,
      6, 215, 104,  99,   0,   0,  25, 107,   6, 135, 223, 224,  90,   0,   0,   0,
      6,   6, 225, 104, 226,   0,   0, 227,   6,   6,   6,   6,   6,   6,   6, 143,
      6,   6,   6,   6,   6,   6,   6, 194, 104,   0,   0,  25,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   6,  30,   6,   6,   6, 228, 229,   0,   0,   0,
      0,   0,   0, 230, 231, 232,   0,   0,   0,   0, 233,   0,   0,   0,   0,   0,
      6,   6, 197,   6, 234, 235, 236,   6, 237, 238, 239,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6, 240, 241,  82, 197, 197, 132, 132, 216, 216, 242,   6,
      6, 243,   6, 244, 245, 246,   0,   0, 247, 248,   0,   0,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6, 249,   0,   6,   6, 204,   0,   0,   0,   0,   0,
    236, 250, 251, 252, 253, 254,   0,   0,   6,   6,   6,   6,   6,   6, 135,   0,
      6,  95,   6,   6,   6,   6,   6,   6,  80,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6, 255,   6,   6,   6,   6,   6,   6,   6,   6,   6, 181,
     80,   0,   0,   0,   0,   0,   0,   0,   6,   6,   6,   6,   6,   6,   6,  90,
};

static RE_UINT8 re_id_continue_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 254, 255, 255, 135, 254, 255, 255,   7,
      0,   4, 160,   4, 255, 255, 127, 255, 255, 255, 255, 255, 195, 255,   3,   0,
     31,  80,   0,   0, 255, 255, 223, 188, 192, 215, 255, 255, 251, 255, 255, 255,
    255, 255, 191, 255, 251, 252, 255, 255, 255, 255, 254, 255, 255, 255, 127,   2,
    254, 255, 255, 255, 255,   0, 254, 255, 255, 255, 255, 191, 182,   0, 255, 255,
    255,   7,   7,   0,   0,   0, 255,   7, 255, 195, 255, 255, 255, 255, 239, 159,
    255, 253, 255, 159,   0,   0, 255, 255, 255, 231, 255, 255, 255, 255,   3,   0,
    255, 255,  63,   4, 255,  63,   0,   0, 255, 255, 255,  15, 255,   7,   0,   0,
    255, 255, 223,  63,   0,   0, 240, 255, 207, 255, 254, 255, 239, 159, 249, 255,
    255, 253, 197, 243, 159, 121, 128, 176, 207, 255,   3,  16, 238, 135, 249, 255,
    255, 253, 109, 211, 135,  57,   2,  94, 192, 255,  63,   0, 238, 191, 251, 255,
    255, 253, 237, 243, 191,  59,   1,   0, 207, 255,   0, 254, 238, 159, 249, 255,
    159,  57, 192, 176, 207, 255,   2,   0, 236, 199,  61, 214,  24, 199, 255, 195,
    199,  61, 129,   0, 192, 255,   0,   0, 239, 223, 253, 255, 255, 253, 255, 227,
    223,  61,  96,   7, 207, 255,   0,   0, 255, 253, 239, 243, 223,  61,  96,  64,
    207, 255,   6,   0, 223, 125, 240, 128, 207, 255,   0, 252, 236, 255, 127, 252,
    255, 255, 251,  47, 127, 132,  95, 255, 192, 255,  12,   0, 255, 255, 255,   7,
    255, 127, 255,   3, 150,  37, 240, 254, 174, 236, 255,  59,  95,  63, 255, 243,
      1,   0,   0,   3, 255,   3, 160, 194, 255, 254, 255, 255, 255,  31, 254, 255,
    223, 255, 255, 254, 255, 255, 255,  31,  64,   0,   0,   0, 255,   3, 255, 255,
    255, 255, 255,  63, 191,  32, 255, 255, 255, 255, 255, 247, 255,  61, 127,  61,
    255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255, 255, 255,  61, 255,
    255, 255, 255, 231,   0, 254,   3,   0, 255, 255,   0,   0, 255, 255,  63,  63,
    255, 159, 255, 255, 255, 199, 255,   1, 255, 223,  31,   0, 255, 255,  31,   0,
    255, 255,  15,   0, 255, 223,  13,   0, 255, 255, 143,  48, 255,   3,   0,   0,
      0,  56, 255,   3, 255, 255, 255,   0, 255,   7, 255, 255, 255, 255,  63,   0,
    255, 255, 255, 127, 255,  15, 255,  15, 192, 255, 255, 255, 255,  63,  31,   0,
    255,  15, 255, 255, 255,   3, 255,   7, 255, 255, 255, 159, 255,   3, 255,   3,
    128,   0, 255,  63, 255,  15, 255,   3,   0, 248,  15,   0, 255, 227, 255, 255,
    255,   1,   0,   0,   0,   0, 247, 255, 255, 255, 255,   3, 255, 255, 255, 251,
     63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,
      0,   0,   0, 128,   1,   0,  16,   0,   0,   0,   2, 128,   0,   0, 255,  31,
    226, 255,   1,   0, 132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0,
    255, 127, 255, 255,  31, 248,  15,   0, 255, 128,   0, 128, 255, 255, 127,   0,
    127, 127, 127, 127, 224,   0,   0,   0, 254, 255,  62,  31, 255, 255, 127, 254,
    224, 255, 255, 255, 255, 127, 254, 255, 255, 127,   0,   0, 255,  31,   0,   0,
    255,  31, 255, 255, 255,  15,   0,   0, 255, 255, 240, 191,   0,   0, 128, 255,
    252, 255, 255, 255, 255, 249, 255, 255, 255, 127, 255,   0, 255,   0,   0,   0,
     63,   0, 255,   3, 255, 255, 255,  40, 255,  63, 255, 255,   1, 128, 255,   3,
    255,  63, 255,   3, 255, 255, 127, 252,   7,   0,   0,  56, 255, 255, 124,   0,
    126, 126, 126,   0, 127, 127, 255, 255,  63,   0, 255, 255, 255,  55, 255,   3,
     15,   0, 255, 255, 127, 248, 255, 255, 127,   0, 248, 224, 255, 253, 127,  95,
    219, 255, 255, 255,   0,   0, 248, 255, 255, 255, 252, 255,   0,   0, 255,  15,
    255, 255,  24,   0,   0, 224,   0,   0,   0,   0, 223, 255, 252, 252, 252,  28,
    255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63,   0,   0,   0,  32,
    255, 255,   1,   0,   1,   0,   0,   0,   0, 224, 255, 255,  15, 255,  62,   0,
    255, 255,  15, 255, 255,   0, 255, 255,  15,   0,   0,   0,  63, 253, 255, 255,
    255, 255, 191, 145, 255, 255,  55,   0, 255, 255, 255, 192, 111, 240, 239, 254,
    255, 255,  15, 135, 127,   0,   0,   0, 255, 255,   7,   0, 192, 255,   0, 128,
    255,   1, 255,   3, 255, 255, 223, 255, 255, 255,  79,   0,  31,  28, 255,  23,
    255, 255, 251, 255, 255, 255, 255,  64, 127, 189, 255, 191, 255,   1, 255, 255,
    255,   7, 255,   3, 159,  57, 129, 224, 207,  31,  31,   0, 191,   0, 255,   3,
    255, 255,  63, 255,   1,   0,   0,  63,  17,   0, 255,   3, 255, 255, 255, 227,
    255,   3,   0, 128, 128,   0, 255, 255, 207, 255, 255,   3, 255, 255, 255,   1,
    255, 253, 255, 255,   1,   0, 255,   3,   0,   0, 252, 255, 255, 254, 127,   0,
    127, 251, 255, 255, 255, 255, 127, 180, 255,   0, 255,   3,  15,   0, 255,   3,
    248, 255, 255, 224,  31,   0, 255, 255,   0, 128, 255, 255,   3,   0,   0,   0,
    255,   7, 255,  31, 255,   1, 255,  99, 224, 227,   7, 248, 231,  15,   0,   0,
      0,  60,   0,   0,  28,   0,   0,   0, 255, 255, 255, 223, 100, 222, 255, 235,
    239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,
     63, 255, 255, 255, 253, 255, 255, 247, 247, 207, 255, 255, 255, 255, 127, 248,
    255,  31,  32,   0,  16,   0,   0, 248, 254, 255,   0,   0, 127, 255, 255, 249,
    219,   7,   0,   0,  31,   0, 127,   0, 150, 254, 247,  10, 132, 234, 150, 170,
    150, 247, 247,  94, 255, 251, 255,  15, 238, 251, 255,  15,   3,   0, 255, 255,
};

/* ID_Continue: 2338 bytes. */

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
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 13, 13, 28, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 29,  7, 30, 31,  7, 32,  7,  7,  7, 33, 13, 34,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_xid_start_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,   1,  17,  18,  19,   1,  20,  21,  22,  23,  24,  25,  26,  27,   1,  28,
     29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  32,  33,  31,  31,
     34,  35,  31,  31,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,  36,   1,   1,   1,   1,   1,   1,   1,   1,   1,  37,
      1,   1,   1,   1,  38,   1,  39,  40,  41,  42,  43,  44,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  45,  31,  31,  31,  31,  31,  31,  31,  31,
     31,   1,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,   1,  58,
     59,  60,  61,  62,  63,  31,  31,  31,  64,  65,  66,  67,  68,  69,  70,  71,
     72,  31,  73,  31,  74,  75,  31,  31,   1,   1,   1,  76,  77,  78,  31,  31,
      1,   1,   1,   1,  79,  31,  31,  31,  31,  31,  31,  31,   1,   1,  80,  31,
      1,   1,  81,  82,  31,  31,  31,  83,   1,   1,   1,   1,   1,   1,   1,  84,
      1,   1,  85,  31,  31,  31,  31,  31,   1,  86,  87,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  88,  31,  31,  31,  31,  31,  31,  31,  89,  90,  91,  92,
     93,  78,  31,  31,  31,  31,  94,  31,   1,   1,   1,   1,   1,   1,  95,   1,
      1,   1,   1,   1,   1,   1,   1,  96,  97,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,  98,   1,   1,   1,   1,  99,  31,  31,  31,  31,
      1,   1, 100,  31,  31,  31,  31,  31,
};

static RE_UINT8 re_xid_start_stage_4[] = {
      0,   0,   1,   1,   0,   2,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   5,   6,   0,   0,   0,   7,   8,   9,   4,  10,
      4,   4,   4,   4,  11,   4,   4,   4,   4,  12,  13,  14,  15,   0,  16,  17,
      0,   4,  18,  19,   4,   4,  20,  21,  22,  23,  24,   4,   4,  25,  26,  27,
     28,  29,  30,  18,   0,  31,   0,   0,  32,  33,  34,  35,  36,  37,  38,  39,
     40,  41,  42,  43,  44,  45,  46,  47,  48,  45,  49,  50,  51,  52,  46,   0,
     53,  54,  55,  56,  57,  58,  59,  60,  53,  61,  62,  63,  64,  65,  66,   0,
     14,  67,  66,   0,  68,  69,  70,   0,  71,   0,  72,  73,  74,   0,   0,   0,
      4,  75,  76,  77,  78,   4,  79,  80,   4,   4,  81,   4,  82,  83,  84,   4,
     85,   4,  86,   0,  23,   4,   4,  87,  14,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,  88,   1,   4,   4,  89,  90,  91,  91,  92,   4,  93,  94,   0,
      0,   4,   4,  95,   4,  96,   4,  97,  98,   0,  16,  99,   4, 100, 101,   0,
    102,   4, 103,   0,   0, 104,   0,   0, 105,  93, 106,   0, 107, 108,   4, 109,
      4, 110, 111, 112, 113,   0,   0, 114,   4,   4,   4,   4,   4,   4,   0,   0,
     87,   4, 115, 112,   4, 116, 117, 118,   0,   0,   0, 119, 120,   0,   0,   0,
    121, 122, 123,   4, 113,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      4, 124,  98,   4,   4,   4,   4, 125,   4,  79,   4, 126, 102, 127, 127,   0,
    128, 129,  14,   4, 130,  14,   4,  80, 105, 131,   4,   4, 132,  86,   0,  16,
      4,   4,   4,   4,   4,  97,   0,   0,   4,   4,   4,   4,   4,   4,   4,  18,
      4,   4,   4,   4,  73,   0,  16, 112, 133, 134,   4, 135, 112,   4,   4,  23,
    136, 137,   4,   4, 138, 139,   0, 136, 140, 141,   4,  93, 137,  93,   0, 142,
     26, 143,  66, 144,  32, 145, 146, 147,   4, 113, 148, 149,   4, 150, 151, 152,
    153, 154,  80, 143,   4,   4,   4, 141,   4,   4,   4,   4,   4, 155, 156, 157,
      4,   4,   4, 158,   4,   4, 159,   0, 160, 161, 162,   4,   4,  91, 163,   4,
      4,   4, 112,  32,   4,   4,   4,   4,   4, 112,  16,   4, 164,   4,  15, 165,
      0,   0,   0, 166,   4,   4,   4, 144,   0,   1,   1, 167, 112,  98, 168,   0,
    169, 170, 171,   0,   4,   4,   4,  86,   0,   0,   4, 103,   0,   0,   0,   0,
      0,   0,   0,   0, 144,   4, 172,   0,   4,  24, 173,  97, 112,   4, 174,   0,
      4,   4,   4,   4, 112,  16, 175, 157,   4, 176,   4, 110,   0,   0,   0,   0,
      4, 102,  97,  15,   0,   0,   0,   0, 177, 178,  97, 102,  98,   0,   0, 179,
     97, 159,   0,   0,   4, 180,   0,   0, 181,  93,   0, 144, 144,   0,  72, 182,
      4,  97,  97, 145,  91,   0,   0,   0,   4,   4, 113,   0,   4, 145,   4, 145,
    107,  95,   0,   0, 107,  23,  16, 113, 107,  66,  16, 183, 107, 145, 184,   0,
    185, 186,   0,   0, 187, 188,  98,   0,  48,  45, 189,  56,   0,   0,   0,   0,
      4, 103, 190,   0,   4,  23, 191,   0,   0,   0,   0,   0,   4, 132, 192,   0,
      4,  23, 193,   0,   4,  18,   0,   0, 159,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   4,   4, 194, 195, 196, 197,   4, 198,   0,   4,  30,
    199, 132,  71, 200,  23,   0,   0,   0, 201, 172, 202,   0,   0,   0,   0,   0,
      4,   4,   4,   4, 159,   0,   0,   0,   4,   4,   4, 132,   4,   4,   4,   4,
      4,   4, 110,   0,   0,   0,   0,   0,   4, 132,   0,   0,   0,   0,   0,   0,
      4,   4,  66,   0,   0,   0,   0,   0,   4,  30,  98,   0,   0,   0,  16, 203,
      4,  23, 110, 204,  23,   0,   0,   0,   4,   4, 205,   0, 163,   0,   0,  56,
      4,   4,   4,   4,   4,   4,   4,  73,   4,   4,   4,   4,   4,   4,   4, 145,
     98,   0,   0,  16,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 157,
      4,   4,   4, 206, 207,   0,   0,   0,   4,   4, 208,   4, 209, 210, 211,   4,
    212, 213, 214,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 215, 216,  80,
    208, 208, 124, 124, 199, 199, 148,   0,   4,   4,   4,   4,   4,   4, 182,   0,
    211, 217, 218, 219, 220, 221,   0,   0,   4,   4,   4,   4,   4,   4, 102,   0,
      4, 103,   4,   4,   4,   4,   4,   4, 112,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4, 222,   4,   4,   4,   4,   4,   4,   4,   4,   4,  71,
    112,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_xid_start_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255,   7,   0,   4,  32,   4, 255, 255, 127, 255,
    255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,   0,   0, 223, 184,
     64, 215, 255, 255, 251, 255, 255, 255, 255, 255, 191, 255,   3, 252, 255, 255,
    255, 255, 254, 255, 255, 255, 127,   2, 254, 255, 255, 255, 255,   0,   0,   0,
      0,   0, 255, 255, 255,   7,   7,   0, 255,   7,   0,   0,   0, 192, 254, 255,
    255, 255,  47,   0,  96, 192,   0, 156,   0,   0, 253, 255, 255, 255,   0,   0,
      0, 224, 255, 255,  63,   0,   2,   0,   0, 252, 255, 255, 255,   7,  48,   4,
    255, 255,  63,   4,  16,   1,   0,   0, 255, 255, 255,   1, 255, 255, 223,  63,
    240, 255, 255, 255, 255, 255, 255,  35,   0,   0,   1, 255,   3,   0, 254, 255,
    225, 159, 249, 255, 255, 253, 197,  35,   0,  64,   0, 176,   3,   0,   3,  16,
    224, 135, 249, 255, 255, 253, 109,   3,   0,   0,   0,  94,   0,   0,  28,   0,
    224, 191, 251, 255, 255, 253, 237,  35,   0,   0,   1,   0,   3,   0,   0,   2,
    224, 159, 249, 255,   0,   0,   0, 176,   3,   0,   2,   0, 232, 199,  61, 214,
     24, 199, 255,   3, 224, 223, 253, 255, 255, 253, 255,  35,   0,   0,   0,   7,
      3,   0,   0,   0, 225, 223, 253, 255, 255, 253, 239,  35,   0,   0,   0,  64,
      3,   0,   6,   0, 255, 255, 255,  39,   0,  64, 112, 128,   3,   0,   0, 252,
    224, 255, 127, 252, 255, 255, 251,  47, 127,   0,   0,   0, 255, 255,   5,   0,
    150,  37, 240, 254, 174, 236,   5,  32,  95,   0,   0, 240,   1,   0,   0,   0,
    255, 254, 255, 255, 255,  31,   0,   0,   0,  31,   0,   0, 255,   7,   0, 128,
      0,   0,  63,  60,  98, 192, 225, 255,   3,  64,   0,   0, 191,  32, 255, 255,
    255, 255, 255, 247, 255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,
     61, 255, 127, 255, 255, 255,  61, 255, 255, 255, 255,   7, 255, 255,  63,  63,
    255, 159, 255, 255, 255, 199, 255,   1, 255, 223,   3,   0, 255, 255,   3,   0,
    255, 223,   1,   0, 255, 255,  15,   0,   0,   0, 128,  16, 255, 255, 255,   0,
    255,   5, 255, 255, 255, 255,  63,   0, 255, 255, 255, 127, 255,  63,  31,   0,
    255,  15, 255, 255, 255,   3,   0,   0, 255, 255, 127,   0, 255, 255,  31,   0,
    128,   0,   0,   0, 224, 255, 255, 255, 224,  15,   0,   0, 248, 255, 255, 255,
      1, 192,   0, 252,  63,   0,   0,   0,  15,   0,   0,   0,   0, 224,   0, 252,
    255, 255, 255,  63, 255,   1,   0,   0,   0, 222,  99,   0,  63,  63, 255, 170,
    255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,   0,   0,   2, 128,
      0,   0, 255,  31, 132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0,
    255, 127, 255, 255,  31, 120,  12,   0, 255, 128,   0,   0, 127, 127, 127, 127,
    224,   0,   0,   0, 254,   3,  62,  31, 255, 255, 127, 224, 255, 127, 254, 255,
    255, 127,   0,   0, 255,  31, 255, 255,   0,  12,   0,   0, 255, 127,   0, 128,
      0,   0, 128, 255, 252, 255, 255, 255, 255, 249, 255, 255, 255, 127, 255,   0,
    187, 247, 255, 255,   7,   0,   0,   0,   0,   0, 252,  40,  63,   0, 255, 255,
    255, 255, 255,  31, 255, 255,   7,   0,   0, 128,   0,   0, 223, 255,   0, 124,
    247,  15,   0,   0, 255, 255, 127, 196, 255, 255,  98,  62,   5,   0,   0,  56,
    255,   7,  28,   0, 126, 126, 126,   0, 127, 127, 255, 255,  15,   0, 255, 255,
    127, 248, 255, 255, 255, 255, 255,  15, 255,  63, 255, 255, 255, 255, 255,   3,
    127,   0, 248, 160, 255, 253, 127,  95, 219, 255, 255, 255,   0,   0, 248, 255,
    255, 255, 252, 255,   0,   0, 255,   3,   0,   0, 138, 170, 192, 255, 255, 255,
    252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63,
    255, 255,   1,   0, 255,   7, 255, 255,  15, 255,  62,   0, 255, 255,  15, 255,
    255,   0, 255, 255,  63, 253, 255, 255, 255, 255, 191, 145, 255, 255,  55,   0,
    255, 255, 255, 192,   1,   0, 239, 254,  31,   0,   0,   0, 255, 255,  71,   0,
     30,   0,   0,  20, 255, 255, 251, 255, 255,  15,   0,   0, 127, 189, 255, 191,
    255,   1, 255, 255,   0,   0,   1, 224, 128,   7,   0,   0, 176,   0,   0,   0,
      0,   0,   0,  15,  16,   0,   0,   0,   0,   0,   0, 128,   1, 248, 255, 255,
    255, 255,   7,   4,   0,   0,   1, 240, 207,   3,   0,   0, 255, 253, 255, 255,
      0,   0, 252, 255, 127, 251, 255, 255,  64,   0,   0,   0, 255,  63,   0,   0,
    248, 255, 255, 224,  31,   0,   1,   0, 255,   7, 255,  31, 255,   1, 255,   3,
    255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255,
    191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255,
    253, 255, 255, 247, 150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94,
    255, 251, 255,  15, 238, 251, 255,  15,   3,   0, 255, 255,
};

/* XID_Start: 2125 bytes. */

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
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 28, 29, 30, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 31,  7, 32, 33,  7, 34,  7,  7,  7, 35, 13, 36,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    37, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_xid_continue_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,   1,  17,  18,  19,   1,  20,  21,  22,  23,  24,  25,  26,  27,   1,  28,
     29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  32,  33,  31,  31,
     34,  35,  31,  31,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,  36,   1,   1,   1,   1,   1,   1,   1,   1,   1,  37,
      1,   1,   1,   1,  38,   1,  39,  40,  41,  42,  43,  44,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  45,  31,  31,  31,  31,  31,  31,  31,  31,
     31,   1,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,   1,  58,
     59,  60,  61,  62,  63,  31,  31,  31,  64,  65,  66,  67,  68,  69,  70,  71,
     72,  31,  73,  31,  74,  75,  31,  31,   1,   1,   1,  76,  77,  78,  31,  31,
      1,   1,   1,   1,  79,  31,  31,  31,  31,  31,  31,  31,   1,   1,  80,  31,
      1,   1,  81,  82,  31,  31,  31,  83,   1,   1,   1,   1,   1,   1,   1,  84,
      1,   1,  85,  31,  31,  31,  31,  31,   1,  86,  87,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  88,  31,  31,  31,  31,  89,  90,  31,  91,  92,  93,  94,
     31,  31,  95,  31,  31,  31,  31,  31,  96,  31,  31,  31,  31,  31,  31,  31,
     97,  98,  31,  31,  31,  31,  99,  31,   1,   1,   1,   1,   1,   1, 100,   1,
      1,   1,   1,   1,   1,   1,   1, 101, 102,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1, 103,   1,   1,   1,   1, 104,  31,  31,  31,  31,
      1,   1, 105,  31,  31,  31,  31,  31,  31, 106,  31,  31,  31,  31,  31,  31,
};

static RE_UINT8 re_xid_continue_stage_4[] = {
      0,   1,   2,   3,   0,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   7,   8,   6,   6,   6,   9,  10,  11,   6,  12,
      6,   6,   6,   6,  13,   6,   6,   6,   6,  14,  15,  16,  17,  18,  19,  20,
     21,   6,   6,  22,   6,   6,  23,  24,  25,   6,  26,   6,   6,  27,   6,  28,
      6,  29,  30,  31,   0,  32,  33,  11,   6,   6,   6,  34,  35,  36,  37,  38,
     39,  40,  41,  42,  43,  44,  45,  46,  47,  44,  48,  49,  50,  51,  52,  53,
     54,  55,  56,  57,  54,  58,  59,  60,  54,   6,  61,  62,  63,  64,  65,  66,
     16,  67,  68,   0,  69,  70,  71,   0,  72,  73,  74,  75,  76,  77,  78,   0,
      6,   6,  79,   6,  80,   6,  81,  82,   6,   6,  83,   6,  84,  85,  86,   6,
     87,   6,  88,  89,  90,   6,   6,  91,  16,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,  92,   3,   6,   6,  93,  94,  95,  96,  97,   6,   6,  98,  99,
    100,   6,   6, 101,   6, 102,   6, 103, 104, 105, 106, 107,   6, 108, 109,   0,
     30,   6, 104, 110, 111, 112,   0,   0,   6,   6, 113, 114,   6,   6,   6,  96,
      6, 101, 115,  80, 116,   0, 117, 118,   6,   6,   6,   6,   6,   6,   6, 119,
     91,   6, 120,  80,   6, 121, 122, 123,   0, 124, 125, 126, 127,   0, 127, 128,
    129, 130, 131,   6, 116,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      6, 132, 104,   6,   6,   6,   6, 133,   6,  81,   6, 134, 135, 136, 136,   6,
    137, 138,  16,   6, 139,  16,   6,  82, 140, 141,   6,   6, 142,  67,   0,  25,
      6,   6,   6,   6,   6, 103,   0,   0,   6,   6,   6,   6,   6,   6,   6,  31,
      6,   6,   6,   6, 143,   0,  25,  80, 144, 145,   6, 146,   6,   6,   6,  27,
    147, 148,   6,   6, 149, 150,   0, 147,   6, 151,   6,  96,   6,   6, 152, 153,
      6, 154,  96,  77,   6,   6, 155, 104,   6, 135, 156, 157,   6,   6, 158, 159,
    160, 161,  82, 162,   6,   6,   6, 163,   6,   6,   6,   6,   6, 164, 165,  30,
      6,   6,   6, 154,   6,   6, 118,   0, 166, 167, 168,   6,   6,  27, 169,   6,
      6,   6,  80, 170,   6,   6,   6,   6,   6,  80,  25,   6, 171,   6, 151,   1,
     90, 172, 173, 174,   6,   6,   6,  77,   1,   2,   3, 106,   6, 104, 175,   0,
    176, 177, 178,   0,   6,   6,   6,  67,   0,   0,   6,  95,   0,   0,   0, 179,
      0,   0,   0,   0,  77,   6, 180, 181,   6, 182, 102,  67,  80,   6, 183,   0,
      6,   6,   6,   6,  80,  79, 184,  30,   6, 185,   6, 186,   0,   0,   0,   0,
      6, 135, 103, 151,   0,   0,   0,   0, 187, 188, 103, 135, 104,   0,   0, 189,
    103, 118,   0,   0,   6, 190,   0,   0, 191, 192,   0,  77,  77,   0,  74, 193,
      6, 103, 103, 194,  27,   0,   0,   0,   6,   6, 116,   0,   6, 194,   6, 194,
      6,   6, 193, 195,   6,  67,  25, 196,   6, 197,  25, 198,   6,   6, 199,   0,
    200, 201,   0,   0, 202, 203,   6, 204,  35,  44, 205, 206,   0,   0,   0,   0,
      6,   6, 204,   0,   6,   6, 207,   0,   0,   0,   0,   0,   6, 208, 209,   0,
      6,   6, 210,   0,   6, 101,  99,   0, 211, 113,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   6,   6, 212,   6, 104, 213,   6, 214,   0,   6, 215,
    216,   5, 217, 218, 171, 219,   0,   0, 220, 221, 222,   0,   0,   0,   0,   0,
      6,   6,   6,   6, 118,   0,   0,   0,   6,   6,   6, 142,   6,   6,   6,   6,
      6,   6, 186,   0,   0,   0,   0,   0,   6, 142,   0,   0,   0,   0,   0,   0,
      6,   6, 193,   0,   0,   0,   0,   0,   6, 215, 104,  99,   0,   0,  25, 107,
      6, 135, 223, 224,  90,   0,   0,   0,   6,   6, 225, 104, 226,   0,   0, 227,
      6,   6,   6,   6,   6,   6,   6, 143,   6,   6,   6,   6,   6,   6,   6, 194,
    104,   0,   0,  25,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,  30,
      6,   6,   6, 228, 229,   0,   0,   0,   0,   0,   0, 230, 231, 232,   0,   0,
      0,   0, 233,   0,   0,   0,   0,   0,   6,   6, 197,   6, 234, 235, 236,   6,
    237, 238, 239,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6, 240, 241,  82,
    197, 197, 132, 132, 216, 216, 242,   6,   6, 243,   6, 244, 245, 246,   0,   0,
    247, 248,   0,   0,   0,   0,   0,   0,   6,   6,   6,   6,   6,   6, 249,   0,
      6,   6, 204,   0,   0,   0,   0,   0, 236, 250, 251, 252, 253, 254,   0,   0,
      6,   6,   6,   6,   6,   6, 135,   0,   6,  95,   6,   6,   6,   6,   6,   6,
     80,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6, 255,   6,   6,
      6,   6,   6,   6,   6,   6,   6, 181,  80,   0,   0,   0,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   6,  90,
};

static RE_UINT8 re_xid_continue_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 254, 255, 255, 135, 254, 255, 255,   7,
      0,   4, 160,   4, 255, 255, 127, 255, 255, 255, 255, 255, 195, 255,   3,   0,
     31,  80,   0,   0, 255, 255, 223, 184, 192, 215, 255, 255, 251, 255, 255, 255,
    255, 255, 191, 255, 251, 252, 255, 255, 255, 255, 254, 255, 255, 255, 127,   2,
    254, 255, 255, 255, 255,   0, 254, 255, 255, 255, 255, 191, 182,   0, 255, 255,
    255,   7,   7,   0,   0,   0, 255,   7, 255, 195, 255, 255, 255, 255, 239, 159,
    255, 253, 255, 159,   0,   0, 255, 255, 255, 231, 255, 255, 255, 255,   3,   0,
    255, 255,  63,   4, 255,  63,   0,   0, 255, 255, 255,  15, 255,   7,   0,   0,
    255, 255, 223,  63,   0,   0, 240, 255, 207, 255, 254, 255, 239, 159, 249, 255,
    255, 253, 197, 243, 159, 121, 128, 176, 207, 255,   3,  16, 238, 135, 249, 255,
    255, 253, 109, 211, 135,  57,   2,  94, 192, 255,  63,   0, 238, 191, 251, 255,
    255, 253, 237, 243, 191,  59,   1,   0, 207, 255,   0, 254, 238, 159, 249, 255,
    159,  57, 192, 176, 207, 255,   2,   0, 236, 199,  61, 214,  24, 199, 255, 195,
    199,  61, 129,   0, 192, 255,   0,   0, 239, 223, 253, 255, 255, 253, 255, 227,
    223,  61,  96,   7, 207, 255,   0,   0, 255, 253, 239, 243, 223,  61,  96,  64,
    207, 255,   6,   0, 223, 125, 240, 128, 207, 255,   0, 252, 236, 255, 127, 252,
    255, 255, 251,  47, 127, 132,  95, 255, 192, 255,  12,   0, 255, 255, 255,   7,
    255, 127, 255,   3, 150,  37, 240, 254, 174, 236, 255,  59,  95,  63, 255, 243,
      1,   0,   0,   3, 255,   3, 160, 194, 255, 254, 255, 255, 255,  31, 254, 255,
    223, 255, 255, 254, 255, 255, 255,  31,  64,   0,   0,   0, 255,   3, 255, 255,
    255, 255, 255,  63, 191,  32, 255, 255, 255, 255, 255, 247, 255,  61, 127,  61,
    255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255, 255, 255,  61, 255,
    255, 255, 255, 231,   0, 254,   3,   0, 255, 255,   0,   0, 255, 255,  63,  63,
    255, 159, 255, 255, 255, 199, 255,   1, 255, 223,  31,   0, 255, 255,  31,   0,
    255, 255,  15,   0, 255, 223,  13,   0, 255, 255, 143,  48, 255,   3,   0,   0,
      0,  56, 255,   3, 255, 255, 255,   0, 255,   7, 255, 255, 255, 255,  63,   0,
    255, 255, 255, 127, 255,  15, 255,  15, 192, 255, 255, 255, 255,  63,  31,   0,
    255,  15, 255, 255, 255,   3, 255,   7, 255, 255, 255, 159, 255,   3, 255,   3,
    128,   0, 255,  63, 255,  15, 255,   3,   0, 248,  15,   0, 255, 227, 255, 255,
    255,   1,   0,   0,   0,   0, 247, 255, 255, 255, 255,   3, 255, 255, 255, 251,
     63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,
      0,   0,   0, 128,   1,   0,  16,   0,   0,   0,   2, 128,   0,   0, 255,  31,
    226, 255,   1,   0, 132, 252,  47,  63,  80, 253, 255, 243, 224,  67,   0,   0,
    255, 127, 255, 255,  31, 248,  15,   0, 255, 128,   0, 128, 255, 255, 127,   0,
    127, 127, 127, 127, 224,   0,   0,   0, 254, 255,  62,  31, 255, 255, 127, 230,
    224, 255, 255, 255, 255, 127, 254, 255, 255, 127,   0,   0, 255,  31,   0,   0,
    255,  31, 255, 255, 255,  15,   0,   0, 255, 255, 240, 191,   0,   0, 128, 255,
    252, 255, 255, 255, 255, 249, 255, 255, 255, 127, 255,   0, 255,   0,   0,   0,
     63,   0, 255,   3, 255, 255, 255,  40, 255,  63, 255, 255,   1, 128, 255,   3,
    255,  63, 255,   3, 255, 255, 127, 252,   7,   0,   0,  56, 255, 255, 124,   0,
    126, 126, 126,   0, 127, 127, 255, 255,  63,   0, 255, 255, 255,  55, 255,   3,
     15,   0, 255, 255, 127, 248, 255, 255, 127,   0, 248, 224, 255, 253, 127,  95,
    219, 255, 255, 255,   0,   0, 248, 255, 240, 255, 255, 255, 255, 255, 252, 255,
    255, 255,  24,   0,   0, 224,   0,   0,   0,   0, 138, 170, 252, 252, 252,  28,
    255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63,   0,   0,   0,  32,
    255, 255,   1,   0,   1,   0,   0,   0,   0, 224, 255, 255,  15, 255,  62,   0,
    255, 255,  15, 255, 255,   0, 255, 255,  15,   0,   0,   0,  63, 253, 255, 255,
    255, 255, 191, 145, 255, 255,  55,   0, 255, 255, 255, 192, 111, 240, 239, 254,
    255, 255,  15, 135, 127,   0,   0,   0, 255, 255,   7,   0, 192, 255,   0, 128,
    255,   1, 255,   3, 255, 255, 223, 255, 255, 255,  79,   0,  31,  28, 255,  23,
    255, 255, 251, 255, 255, 255, 255,  64, 127, 189, 255, 191, 255,   1, 255, 255,
    255,   7, 255,   3, 159,  57, 129, 224, 207,  31,  31,   0, 191,   0, 255,   3,
    255, 255,  63, 255,   1,   0,   0,  63,  17,   0, 255,   3, 255, 255, 255, 227,
    255,   3,   0, 128, 128,   0, 255, 255, 207, 255, 255,   3, 255, 255, 255,   1,
    255, 253, 255, 255,   1,   0, 255,   3,   0,   0, 252, 255, 255, 254, 127,   0,
    127, 251, 255, 255, 255, 255, 127, 180, 255,   0, 255,   3,  15,   0, 255,   3,
    248, 255, 255, 224,  31,   0, 255, 255,   0, 128, 255, 255,   3,   0,   0,   0,
    255,   7, 255,  31, 255,   1, 255,  99, 224, 227,   7, 248, 231,  15,   0,   0,
      0,  60,   0,   0,  28,   0,   0,   0, 255, 255, 255, 223, 100, 222, 255, 235,
    239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,
     63, 255, 255, 255, 253, 255, 255, 247, 247, 207, 255, 255, 255, 255, 127, 248,
    255,  31,  32,   0,  16,   0,   0, 248, 254, 255,   0,   0, 127, 255, 255, 249,
    219,   7,   0,   0,  31,   0, 127,   0, 150, 254, 247,  10, 132, 234, 150, 170,
    150, 247, 247,  94, 255, 251, 255,  15, 238, 251, 255,  15,   3,   0, 255, 255,
};

/* XID_Continue: 2346 bytes. */

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
    0, 1, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2,
    2, 2,
};

static RE_UINT8 re_default_ignorable_code_point_stage_2[] = {
    0, 1, 2, 3, 4, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 7, 1, 1, 8, 1, 1, 1, 1, 1,
    9, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_default_ignorable_code_point_stage_3[] = {
     0,  1,  1,  2,  1,  1,  3,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  4,  1,  1,  1,  1,  1,  5,  6,  1,  1,  1,  1,  1,  1,  1,
     7,  1,  1,  1,  1,  1,  1,  1,  1,  8,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  9, 10,  1,  1,  1,  1, 11,  1,  1,  1,
     1, 12,  1,  1,  1,  1,  1,  1, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_default_ignorable_code_point_stage_4[] = {
     0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  2,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  4,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,
     7,  0,  0,  0,  0,  0,  0,  0,  8,  9,  0, 10,  0,  0,  0,  0,
     0,  0,  0, 11,  0,  0,  0,  0, 10,  0,  0,  0,  0,  0,  0,  4,
     0,  0,  0,  0,  0,  5,  0, 12,  0,  0,  0,  0,  0, 13,  0,  0,
     0,  0,  0, 14,  0,  0,  0,  0, 15, 15, 15, 15, 15, 15, 15, 15,
};

static RE_UINT8 re_default_ignorable_code_point_stage_5[] = {
      0,   0,   0,   0,   0,  32,   0,   0,   0, 128,   0,   0,   0,   0,   0,  16,
      0,   0,   0, 128,   1,   0,   0,   0,   0,   0,  48,   0,   0, 120,   0,   0,
      0, 248,   0,   0,   0, 124,   0,   0, 255, 255,   0,   0,  16,   0,   0,   0,
      0,   0, 255,   1,  15,   0,   0,   0,   0,   0, 248,   7, 255, 255, 255, 255,
};

/* Default_Ignorable_Code_Point: 370 bytes. */

RE_UINT32 re_get_default_ignorable_code_point(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_default_ignorable_code_point_stage_1[f] << 4;
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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2,
};

static RE_UINT8 re_grapheme_extend_stage_2[] = {
     0,  1,  2,  3,  4,  4,  4,  4,  4,  4,  5,  4,  4,  4,  4,  6,
     7,  8,  4,  4,  4,  4,  9,  4,  4,  4,  4, 10,  4, 11, 12,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    13,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
};

static RE_UINT8 re_grapheme_extend_stage_3[] = {
     0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13,
    14,  0,  0, 15,  0,  0,  0, 16, 17, 18, 19, 20, 21, 22,  0,  0,
    23,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 25,  0,  0,
    26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 27,  0, 28, 29, 30, 31,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 32,  0,  0, 33, 34,
     0, 35, 36, 37,  0,  0,  0,  0,  0,  0, 38,  0,  0,  0,  0,  0,
    39, 40, 41, 42, 43, 44, 45, 46,  0,  0, 47,  0, 48, 49,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 50, 51,  0,  0,  0, 52,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 53,  0,  0,  0,
     0, 54, 55,  0,  0,  0,  0,  0,  0,  0, 56,  0,  0,  0,  0,  0,
    57,  0,  0,  0,  0,  0,  0,  0, 58, 59,  0,  0,  0,  0,  0,  0,
    60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_grapheme_extend_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   2,   0,   0,   0,   0,
      0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   4,   5,   6,   0,
      7,   0,   8,   9,   0,   0,  10,  11,  12,  13,  14,   0,   0,  15,   0,  16,
     17,  18,  19,   0,   0,   0,  20,  21,  22,  23,  24,  25,  26,  27,  28,  25,
     29,  30,  31,  32,  29,  30,  33,  34,  26,  35,  36,  25,  37,  38,  39,   0,
     40,  41,  42,  25,  26,  43,  44,  25,  45,  46,  28,  25,   0,   0,  47,   0,
      0,  48,  49,   0,   0,  50,  51,   0,  52,  53,   0,  54,  55,  56,  57,   0,
      0,  58,  59,  60,  61,   0,   0,   0,   0,   0,  62,   0,   0,   0,   0,   0,
     63,  63,  64,  64,   0,  65,  66,   0,  67,   0,   0,   0,  68,  69,   0,   0,
      0,  70,   0,   0,   0,   0,   0,   0,  71,   0,  72,  73,   0,  74,   0,   0,
     75,  76,  37,  16,  45,  77,   0,  78,   0,  79,   0,   0,   0,   0,  80,  81,
      0,   0,   0,   0,   0,   0,   1,  82,  83,   0,   0,   0,   0,   0,  13,  84,
      0,   0,   0,   0,   0,   0,   0,  85,   0,   0,   0,  86,   0,   0,   0,   1,
      0,  87,   0,   0,  88,   0,   0,   0,   0,   0,   0,  89,  41,   0,   0,  90,
     91,  68,   0,   0,   0,   0,  92,  93,   0,  94,  95,   0,  22,  96,   0,  97,
      0,  98,  99,  30,   0, 100,  26, 101,   0,   0,   0,   0,   0,   0,   0, 102,
     38,   0,   0,   0,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 103,
      0,   0,   0,   0,   0,   0,   0,  40,   0,   0,   0, 104,   0,   0,   0,   0,
    105, 106,   0,   0,   0,   0,   0,  68,  26, 107, 108,  86,  45, 109,   0,   0,
     22, 110,   0, 111,  45, 112, 113,   0,   0, 114,   0,   0,   0,   0,  86, 115,
     45,  27, 116, 117,   0,   0,   0,   0,   0, 107, 118,   0,   0, 119, 120,   0,
      0,   0,   0,   0,   0, 121, 122,   0,   0, 123,  40,   0,   0, 124,   0,   0,
     62, 125,   0,   0,   0,   0,   0,   0, 126, 127, 128,   0, 129,   0,   0,   0,
      0, 130,   0,   0, 131, 132,   0,   0,   0, 133, 134,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 135,   0, 136,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 137,   0,   0,   0,   0,   0,   0,   0, 138,   0,   0,   0,
      0,   0,   0, 139, 140, 141,   0,   0,   0,   0, 142,   0,   0,   0,   0,   0,
      1, 143,   1, 144, 145, 146,   0,   0, 147, 148,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 136,   0,   0,   0, 149,   0,   0,   0,   0,   0,
      0,   1,   1,   1,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   2,
};

static RE_UINT8 re_grapheme_extend_stage_5[] = {
      0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0, 248,   3,   0,   0,
      0,   0, 254, 255, 255, 255, 255, 191, 182,   0,   0,   0,   0,   0, 255,   7,
      0, 248, 255, 255,   0,   0,   1,   0,   0,   0, 192, 159, 159,  61,   0,   0,
      0,   0,   2,   0,   0,   0, 255, 255, 255,   7,   0,   0, 192, 255,   1,   0,
      0, 248,  15,   0,   0,   0, 192, 251, 239,  62,   0,   0,   0,   0,   0,  14,
      0,   0, 240, 255, 251, 255, 255, 255,   7,   0,   0,   0,   0,   0,   0,  20,
    254,  33, 254,   0,  12,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,  80,
     30,  32, 128,   0,   6,   0,   0,   0,   0,   0,   0,  16, 134,  57,   2,   0,
      0,   0,  35,   0, 190,  33,   0,   0,  12,   0,   0, 252,   0,   0,   0, 208,
     30,  32, 192,   0,   4,   0,   0,   0,   0,   0,   0,  64,   1,  32, 128,   0,
      1,   0,   0,   0,   0,   0,   0, 192, 193,  61,  96,   0,   0,   0,   0, 144,
     68,  48,  96,   0,   3,   0,   0,   0,   0,   0,   0,  88,   0, 132,  92, 128,
      0,   0, 242,   7, 128, 127,   0,   0,   0,   0, 242,  27,   0,  63,   0,   0,
      0,   0,   0,   3,   0,   0, 160,   2,   0,   0, 254, 127, 223, 224, 255, 254,
    255, 255, 255,  31,  64,   0,   0,   0,   0, 224, 253, 102,   0,   0,   0, 195,
      1,   0,  30,   0, 100,  32,   0,  32,   0,   0,   0, 224,   0,   0,  28,   0,
      0,   0,  12,   0,   0,   0, 176,  63,  64, 254,  15,  32,   0,  56,   0,   0,
     96,   0,   0,   0,   0,   2,   0,   0, 135,   1,   4,  14,   0,   0, 128,   9,
      0,   0,  64, 127, 229,  31, 248, 159,   0,   0, 255, 127,  15,   0,   0,   0,
      0,   0, 208,  23,  60,  59,   0,   0,  64, 163,   3,   0,   0, 240, 207,   0,
      0,   0, 247, 255, 253,  33,  16,   3, 255, 255, 255, 251,   0,  16,   0,   0,
    255, 255,   1,   0,   0, 128,   3,   0,   0,   0,   0, 128,   0, 252,   0,   0,
      0,   0,   0,   6,   0, 128, 247,  63,   0,   0,   3,   0,  68,   8,   0,   0,
     48,   0,   0,   0, 255, 255,   3,   0, 192,  63,   0,   0, 128, 255,   3,   0,
      0,   0, 200,  19,  32,   0,   0,   0,   0, 126, 102,   0,   8,  16,   0,   0,
      0,   0, 157, 193,   0,  48,  64,   0,  32,  33,   0,   0,   0,   0,   0,  32,
      0,   0, 192,   7, 110, 240,   0,   0,   0,   0,   0, 135,   0,   0,   0, 255,
    127,   0,   0,   0,   0,   0, 120,   6, 128, 239,  31,   0,   0,   0,   8,   0,
      0,   0, 192, 127,   0,  28,   0,   0,   0, 128, 211,  64, 248,   7,   0,   0,
      1,   0, 128,   0, 192,  31,  31,   0,  92,   0,   0,   0,   0,   0, 249, 165,
     13,   0,   0,   0,   0, 128,  60, 176,   1,   0,   0,  48,   0,   0, 248, 167,
      0,  40, 191,   0, 188,  15,   0,   0, 126,   6,   0,   0,   0,   0, 248, 121,
    128,   0, 126,  14,   0, 252, 127,   3,   0,   0, 127, 191,   0,   0, 252, 255,
    255, 252, 109,   0,   0,   0, 126, 180, 191,   0,   0,   0,   0,   0,  31,   0,
      0,   0, 127,   0,   0, 128,   7,   0,   0,   0,   0,  96, 160, 195,   7, 248,
    231,  15,   0,   0,   0,  60,   0,   0,  28,   0,   0,   0, 255, 255, 127, 248,
    255,  31,  32,   0,  16,   0,   0, 248, 254, 255,   0,   0, 127, 255, 255, 249,
    219,   7,   0,   0, 240,   7,   0,   0,
};

/* Grapheme_Extend: 1401 bytes. */

RE_UINT32 re_get_grapheme_extend(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_grapheme_extend_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_grapheme_extend_stage_2[pos + f] << 4;
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
    0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6,
};

static RE_UINT8 re_grapheme_base_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 13, 13,
    13, 13, 13, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 15, 13, 16, 17, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 19, 19, 13, 32, 19, 19,
    19, 33, 19, 19, 19, 19, 19, 19, 19, 19, 34, 35, 13, 13, 13, 13,
    13, 36, 37, 19, 19, 19, 19, 19, 19, 19, 19, 19, 38, 19, 19, 39,
    19, 19, 19, 19, 40, 41, 42, 19, 19, 19, 43, 44, 45, 46, 47, 19,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 48, 13, 13, 13, 49, 50, 13,
    13, 13, 13, 51, 13, 13, 13, 13, 13, 13, 52, 19, 19, 19, 53, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
};

static RE_UINT8 re_grapheme_base_stage_3[] = {
      0,   1,   2,   2,   2,   2,   3,   4,   2,   5,   6,   7,   8,   9,  10,  11,
     12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
     28,  29,   2,   2,  30,  31,  32,  33,   2,   2,   2,   2,   2,  34,  35,  36,
     37,  38,  39,  40,  41,  42,  43,  44,  45,  46,   2,  47,   2,   2,  48,  49,
     50,  51,   2,  52,   2,   2,   2,   2,  53,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,  54,  55,  56,  57,  58,  59,  60,  61,   2,  62,
     63,  64,  65,  66,  67,  68,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,  69,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  70,
      2,  71,   2,   2,  72,  73,   2,  74,  75,  76,  77,  78,  79,  80,  81,  82,
      2,   2,   2,   2,   2,   2,   2,  83,  84,  84,  84,  84,  84,  84,  84,  84,
     84,  84,   2,   2,  85,  86,  87,  88,   2,   2,  89,  90,  91,  92,  93,  94,
     95,  96,  97,  98,  84,  99, 100, 101,   2, 102, 103,  84,   2,   2, 104,  84,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114,  84,  84, 115,  84,  84,  84,
    116, 117, 118, 119, 120, 121, 122,  84, 123, 124,  84, 125, 126, 127, 128,  84,
     84, 129,  84,  84, 130, 131,  84,  84, 132, 133, 134,  84,  84,  84,  84,  84,
      2,   2,   2,   2,   2,   2,   2, 135, 136,   2, 137,  84,  84,  84,  84,  84,
    138,  84,  84,  84,  84,  84,  84,  84,   2,   2,   2,   2, 139,  84,  84,  84,
      2,   2,   2,   2, 140, 141, 142, 143,  84,  84,  84,  84,  84,  84, 144, 145,
      2,   2,   2,   2,   2,   2,   2, 146,   2,   2,   2,   2,   2, 147,  84,  84,
      2,   2, 148,   2,   2, 149,  84,  84, 150, 151,  84,  84,  84,  84,  84,  84,
      2, 152, 153, 154, 155,  84, 156,  84, 157, 158, 159,   2,   2, 160,   2, 161,
      2,   2,   2,   2, 162, 163,  84,  84,   2, 164, 165,  84,  84,  84,  84,  84,
     84,  84,  84,  84, 166, 167,  84,  84, 168, 169, 170, 171, 172,  84,   2,   2,
      2,   2,   2,   2,   2, 173, 174, 175, 176, 177, 178, 179,  84,  84,  84,  84,
      2,   2,   2,   2,   2, 180,   2,   2,   2,   2,   2,   2,   2,   2, 181,   2,
    182,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 183,   2,   2,
      2,   2,   2,   2,   2,   2,   2, 184,   2,   2,   2,   2, 185,  84,  84,  84,
};

static RE_UINT8 re_grapheme_base_stage_4[] = {
      0,   0,   1,   1,   1,   1,   1,   2,   0,   0,   3,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,   4,
      5,   1,   6,   1,   1,   1,   1,   1,   7,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   8,   1,   9,   8,   1,  10,   0,   0,  11,  12,   1,  13,  14,
     15,  16,   1,   1,  13,   0,   1,   8,   1,   1,   1,   1,   1,  17,  18,   1,
     19,  20,   1,   0,  21,   1,   1,   1,   1,   1,  22,  23,   1,   1,  13,  24,
      1,  25,  26,   2,   1,  27,  13,   0,   0,   0,   1,  28,   0,   0,   0,   0,
     29,   1,   1,  30,  31,  32,  33,   1,  34,  35,  36,  37,  38,  39,  40,  19,
     41,  35,  36,  42,  43,  44,  15,  45,  46,   6,  36,  47,  48,  43,  40,  49,
     50,  35,  36,  51,  52,  39,  40,  53,  54,  55,  56,  57,  58,  43,  15,  13,
     59,  20,  36,  60,  61,  62,  40,  63,  64,  20,  36,  65,  66,  11,  40,  67,
     68,  20,   1,  69,  70,  71,  40,   1,  72,  73,   1,  74,  75,  76,  15,  45,
      8,   1,   1,  77,  78,  79,   0,   0,  80,  81,  82,  83,  84,  85,   0,   0,
      1,   4,   1,  86,  87,   1,  88,  89,  90,   0,   0,  91,  92,  13,   0,   0,
      1,   1,  88,  93,   1,  94,   8,  95,  96,   3,   1,   1,  97,   1,   1,   1,
      1,   1,   1,   1,  98,  99,   1,   1,  98,   1,   1, 100, 101, 102,   1,   1,
      1, 101,   1,   1,   1,  13,   1,  88,   1, 103,   1,   1,   1,   1,   1, 104,
      1,  88,   1,   1,   1,   1,   1, 105,   3, 106,   1, 107,   1, 106,   3,  43,
      1,   1,   1, 108, 109, 110, 103, 103,  13, 103,   1,   1,   1,   1,   1,  53,
    111,   1, 112,   1,   1,   1,   1,  22,   1,   2, 113, 114, 115,   1,  19,  14,
      1,   1,  79,   1, 103, 116,   1,   1,   1, 117,   1,   1,   1, 118, 119, 120,
    103, 103,  19,   0,   0,   0,   0,   0, 121,   1,   1, 122, 123,   1,  13, 110,
    124,   1, 125,   1,   1,   1, 126, 127,   1,   1,  79, 128, 129,   1,   1,   1,
    105,   0,   0,   0,  53, 130, 131, 132,   1,   1,   1,   1,   0,   0,   0,   0,
      1, 104,   1,   1, 104, 133,   1,  19,   1,   1,   1, 134, 134, 135,   1, 136,
     13,   1, 137,   1,   1,   1,   0,  33,   2,  88,   1,   1,   0,   0,   0,   0,
     79,   1,   1,   1,   1,   1,   1,   1,   1,   1,  75,   0,  13,   0,   1,   1,
      1,   1,   1,   1,   1,   1,   1, 138,   1, 139,   1, 129,  36, 120, 140,   0,
      1,   1,   2,   1,   1,   2,   1,   1,   1,   1,   1,   1,   1,   1,   2, 141,
      1,   1,  97,   1,   1,   1, 137,  43,   1,  75, 142, 142, 142, 142,   0,   0,
      1,   1,   1,   1, 103,   0,   0,   0,   1, 143,   1,   1,   1,   1,   1, 144,
      1,   1,   1,   1,   1,  22,   0,  79,   1,   1, 103,   1,   8,   1,   1,   1,
      1, 145,   1,   1,   1,   1,   1,   1, 146,   1,   2,   8,   1,   1,   1,   1,
      2,   1,   1,  13,   1,   1, 144,   1,   1,   2,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,   2,   1,   1,   1,  22,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,  13,   0,  88,   1,   1,   1,  75,   1,   1,   1,
      1,   1,  79,   0,   1,   1,   2, 147,   1,  19,   1,   1,   1,   1,   1, 148,
      1,   1,   2,  53,   0,   0,   0, 149, 150,   1, 151, 103,   1,   1,   1,  53,
      1,   1,   1,   1, 152, 103,   0, 153,   1,   1, 154,   1,  75, 155,   1,  88,
     29,   1,   1, 156, 157, 158, 134,   2,   1,   1, 159, 160, 161,  85,   1, 162,
      1,   1,   1, 163, 164, 165, 166,  22, 167, 168, 142,   1,   1,   1,  22,   1,
      1,   1,   1,   1,   1,   1, 169, 103,   1,   1, 144,   1, 145,   1,   1,  79,
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,  19,   1,
      1,   1,   1,   1,   1, 103,   0,   0,  75, 170,   1, 171, 172,   1,   1,   1,
      1,   1,   1,   1, 106,  29,   1,   1,   1,   1,   1,   1,   0,   1,   1,   1,
      1, 124,   1,   1,  53,   0,   0,  19,   0, 103,   0,   1,   1, 173, 174, 134,
      1,   1,   1,   1,   1,   1,   1,  88,   8,   1,   1,   1,   1,   1,   1,   1,
      1,  19,   1,   2, 175, 176, 142, 177, 162,   1, 102, 178,  19,  19,   0,   0,
      1,   1,   1,   1,   1,   1,   1,  13, 179,   1,   1, 180,   1,   1,   1,   1,
      2,  79,  43,   0,   0,   1,   1,  88,   1,  88,   1,   1,   1,  43,   8,  79,
      1,   1, 181,   1,  13,   1,   1,  22,   1, 157,   1,   1, 182,  22,   0,   0,
      1,  19, 103,   1,   1, 182,   1,  79,   1,   1,  53,   1,   1,   1, 183,   0,
      1,   1,   1,  75,   1,  22,  53,   0, 184,   1,   1, 185,   1, 186,   1,   1,
      1,   2, 149,   0,   0,   0,   1, 187,   1, 188,   1,  57,   0,   0,   0,   0,
      1,   1,   1, 189,   1, 124,   1,   1,  43, 190,   1, 144,  53, 105,   1,   1,
      1,   1,   0,   0,   1,   1, 191,  75,   1,   1,   1, 192,   1, 139,   1, 193,
      1, 194, 195,   0,   0,   0,   0,   0,   1,   1,   1,   1, 105,   0,   0,   0,
      1,   1,   1, 120,   1,   1,   1,   7,   0,   0,   0,   0,   0,   0,   1,   2,
     20,   1,   1,  53, 196, 124,   1,   0, 124,   1,   1, 197, 106,   1, 105, 103,
     29,   1, 198,  15, 144,   1,   1, 199, 124,   1,   1, 200,  60,   1,   8,  14,
      1,   6,   2, 201,   0,   0,   0,   0, 202, 157, 103,   1,   1,   2, 120, 103,
     50,  35,  36, 203, 204, 205, 144,   0,   1,   1,   1,  53, 206, 207,   0,   0,
      1,   1,   1, 208, 209, 103,   0,   0,   1,   1,   2, 210,   8,  79,   0,   0,
      1,   1,   1, 211,  61, 103,  88,   0,   1,   1, 212, 213, 103,   0,   0,   0,
      1, 103, 214,   1,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1, 215,
    216,   1,   1, 217,  75, 218,   1,   1, 219, 220, 120,   0,   1,   1,   1, 105,
     36,   1,   1,  11,  22,   1,  88,   1,   1,   0, 221, 222,   0,   0,   0,   0,
    223,   1,   1,  43, 213, 103,   0,   0,   1, 103,   0,   0,   0,   0,   0,   0,
      1,   1,   1,   1,   1,   1,   2,  14,   1,   1,   1,   1, 144,   0,   0,   0,
      1,   1,   2,   0,   0,   0,   0,   0,   1,   1,   1,   1,  75,   0,   0,   0,
      1,   1,   1, 105,   1,   2, 158,   0,   0,   0,   0,   0,   0,   1,  19, 224,
      1,   1,   1, 149,  22, 143,   6, 225,   1,   0,   0,   0,   0,   0,   0,   0,
      1,   1,   1,   1,  14,   1,   1,   2,   0,  29,   0,   0,   0,   0, 106,   0,
      1,   1,   1,   1,   1,   1,  88,   0,   1,   1,   1,   1,   1,   1,   1, 120,
      1,   2,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,  79,
      1,   1,   1,   1,   1,   1,  13,  88, 105, 226,   0,   0,   0,   0,   0,   0,
      1,   1,   1,   1,   1,   1,   1,  22,   1,   1,   9,   1,   1,   1, 227,   0,
    228,   1, 158,   1,   1,   1, 105,   0,   1,   1,   1,   1, 229,   0,   0,   0,
      1,   1,   1,   1,   1,  75,   1, 106,   1,   1,   1,   1,   1, 134,   1,   1,
      1,   3, 230,  30, 231,   1,   1,   1, 232, 233,   1, 234, 235,  20,   1,   1,
      1,   1, 139,   1,   1,   1,   1,   1,   1,   1,   1,   1, 166,   1,   1,   1,
      0,   0,   0, 236,   0,   0,  21, 134, 237,   0,   0,   0,   0,   0,   0,   0,
      1,   1,   1,   1, 111,   0,   0,   0,   1,   1,   1,   1, 144, 158,   0,   0,
    231,   1, 238, 239, 240, 241, 242, 243, 143,  79, 244,  79,   0,   0,   0, 106,
      1,   1,  79,   1,   1,   1,   1,   1,   1, 144,   2,   8,   8,   8,   1,  22,
     88,   1,   2,   1,   1,   1,  79,   1,   1,   1,  88,   0,   0,   0,  15,   1,
    120,   1,   1,  79, 105, 106,  22,   0,   1,   1,   1,   1,   1,  14,  88, 105,
      1,   1,   1,   1,   1,   1,   1, 144,   1,   1,   1,   1,   1,  14,   0,   0,
     79,   1,   1,   1,  53, 103,   1,   1,  53,   1,  19,   0,   0,   0,   0,   0,
     79,   1,   1,   2,  88,   1,  79,   0,   1,  53,   0,   0,  43,   1,  75,   0,
      1,   1,   1,   1,   1,  75,   0,   0,   1,   1,   1,  14,   1,   1,   1,   1,
      1,  19,   1,   1,   1,   1,   1,   1,   1,   1, 106,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,  43,   0,   1,  19,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_grapheme_base_stage_5[] = {
      0,   0, 255, 255, 255, 127, 255, 223, 255, 252, 240, 215, 251, 255,   7, 252,
    254, 255, 127, 254, 255, 230,   0,  64,  73,   0, 255,   7,  31,   0, 192, 255,
      0, 200,  63,  64,  96, 194, 255,  63, 253, 255,   0, 224,  63,   0,   2,   0,
    240,   7,  63,   4,  16,   1, 255,  65, 223,  63, 248, 255, 255, 235,   1, 222,
      1, 255, 243, 255, 237, 159, 249, 255, 255, 253, 197, 163, 129,  89,   0, 176,
    195, 255, 232, 135, 109, 195,   1,   0,   0,  94,  28,   0, 232, 191, 237, 227,
      1,  26,   3,   2, 236, 159, 237,  35, 129,  25, 255,   0, 232, 199,  61, 214,
     24, 199, 255, 131, 198,  29, 238, 223, 255,  35,  30,   0,   0,   7,   0, 255,
    237, 223, 239,  99, 155,  13,   6,   0, 236, 223, 255, 167, 193, 221, 112, 255,
    236, 255, 127, 252, 251,  47, 127,   0,   3, 127,  13, 128, 127, 128, 255,  15,
    150,  37, 240, 254, 174, 236,  13,  32,  95,   0, 255, 243,  95, 253, 255, 254,
    255,  31,   0, 128,  32,  31,   0, 192, 191, 223,   2, 153, 255,  60, 225, 255,
    155, 223, 191,  32, 255,  61, 127,  61,  61, 127,  61, 255, 127, 255, 255,   3,
     63,  63, 255,   1,   3,   0,  99,   0,  79, 192, 191,   1, 240,  31, 159, 255,
    255,   5, 120,  14, 251,   1, 241, 255, 255, 199, 127, 198, 191,   0,  26, 224,
      7,   0, 240, 255,  47, 232, 251,  15, 252, 255, 195, 196, 191,  92,  12, 240,
     48, 248, 255, 227,   8,   0,   2, 222, 239,   0, 255, 170, 223, 255, 207, 239,
    220, 127, 255, 128, 207, 255,  63, 255,   0, 240,  12, 254, 127, 127, 255, 251,
     15,   0, 127, 248, 224, 255,   8, 192, 252,   0, 128, 255, 187, 247, 159,  15,
     15, 192, 252,  63,  63, 192,  12, 128,  55, 236, 255, 191, 255, 195, 255, 129,
     25,   0, 247,  47, 255, 239,  98,  62,   5,   0,   0, 248, 255, 207, 126, 126,
    126,   0, 223,  30, 248, 160, 127,  95, 219, 255, 247, 255, 127,  15, 252, 252,
    252,  28,   0,  48, 255, 183, 135, 255, 143, 255,  15, 224,  15, 255,  15, 128,
     63, 253, 191, 145, 191, 255,  55, 248, 255, 143, 255, 240, 239, 254,  31, 248,
     63, 254,   7, 255,   3,  30,   0, 254, 128,  63, 135, 217, 127,  16, 119,   0,
     63, 128,  44,  63, 127, 189, 237, 163, 158,  57,   1, 224, 163, 255, 255,  43,
      6,  90, 242,   0,   3,  79,   7,  88, 255, 215,  64,   0,  67,   0,   7, 128,
    129, 249,   7, 134, 129, 241, 207,   3, 128, 220,   0,   2,  18,   0, 127, 251,
     32,   0, 255, 224, 255, 147,  95,  60,  24, 240,  35,   0, 100, 222, 239, 255,
    191, 231, 223, 223, 255, 123,  95, 252, 128,   7, 239,  15, 150, 254, 247,  10,
    132, 234, 150, 170, 150, 247, 247,  94, 238, 251,
};

/* Grapheme_Base: 2668 bytes. */

RE_UINT32 re_get_grapheme_base(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_grapheme_base_stage_1[f] << 5;
    f = code >> 10;
    code ^= f << 10;
    pos = (RE_UINT32)re_grapheme_base_stage_2[pos + f] << 3;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_grapheme_base_stage_3[pos + f] << 3;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_grapheme_base_stage_4[pos + f] << 4;
    pos += code;
    value = (re_grapheme_base_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Grapheme_Link. */

static RE_UINT8 re_grapheme_link_stage_1[] = {
    0, 1, 2, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_grapheme_link_stage_2[] = {
     0,  0,  1,  2,  3,  4,  5,  0,  0,  0,  0,  6,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,
     0,  0,  8,  0,  9, 10, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_grapheme_link_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,
     1,  1,  2,  3,  4,  0,  0,  5,  6,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  7,  8,  0,  0,  0,  0,  9,  0, 10, 11,
     0,  0, 12,  0,  0,  0,  0,  0, 13, 10, 14, 15,  0, 16,  0, 17,
     0,  0,  0,  0, 18,  0,  0,  0, 19, 20, 21, 15, 22, 23,  1,  0,
    24, 24,  0, 18, 18, 25, 26,  0,  0,  0,  0,  0, 27, 28,  0,  0,
    18,  0, 29,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_grapheme_link_stage_4[] = {
     0,  0,  0,  0,  0,  0,  1,  0,  0,  2,  1,  0,  0,  0,  3,  0,
     0,  4,  0,  0,  5,  0,  0,  0,  0,  6,  0,  0,  7,  7,  0,  0,
     0,  0,  8,  0,  0,  0,  0,  9,  0,  0,  5,  0,  0, 10,  0, 11,
     0,  0,  0, 12, 13,  0,  0,  0,  0,  0, 14,  0,  0,  0,  9,  0,
     0,  0,  0, 15,  0,  0,  0,  1,  0, 12,  0,  0,  0,  0, 13, 12,
     0, 16,  0,  0,  0, 17,  0,  0,  0, 18,  0,  0,  0,  0,  0,  3,
     0,  0, 19,  0,  0, 15,  0,  0,  0, 20,  0,  0,  0,  7, 21,  0,
    16,  0,  0,  0,  0,  0, 22,  0,
};

static RE_UINT8 re_grapheme_link_stage_5[] = {
      0,   0,   0,   0,   0,  32,   0,   0,   0,   0,   0,  24,   0,   4,   0,   0,
      0,   0,   0,   4,  16,   0,   0,   0,   0,   0,   0,   6,   0,   0,  16,   0,
      0,   0,   4,   0,   1,   0,   0,   0,   0,  12,   0,   0,   0,   0,  12,   0,
      0,   0,   0, 128,  64,   0,   0,   0,   0,   0,   8,   0,   0,   0,  64,   0,
      0,   0,   0,   2,   0,   0,  24,   0,   0,   0,  32,   0,   4,   0,   0,   0,
      0,   8,   0,   0, 128,   0,   0,   0,  48,   0,   0,   0,
};

/* Grapheme_Link: 448 bytes. */

RE_UINT32 re_get_grapheme_link(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_grapheme_link_stage_1[f] << 4;
    f = code >> 10;
    code ^= f << 10;
    pos = (RE_UINT32)re_grapheme_link_stage_2[pos + f] << 3;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_grapheme_link_stage_3[pos + f] << 2;
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
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_white_space_stage_4[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 3, 1, 1, 1, 1, 1, 4, 5, 1, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_white_space_stage_5[] = {
      0,  62,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     32,   0,   0,   0,   1,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,
    255,   7,   0,   0,   0, 131,   0,   0,   0,   0,   0, 128,   0,   0,   0,   0,
};

/* White_Space: 169 bytes. */

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
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_bidi_control_stage_3[] = {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_bidi_control_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    2, 3, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_bidi_control_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,   0,   0,   0,   0,
      0, 192,   0,   0,   0, 124,   0,   0,   0,   0,   0,   0, 192,   3,   0,   0,
};

/* Bidi_Control: 129 bytes. */

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
     8,  1,  1,  1,  1,  1,  1,  1,  9,  3,  1,  1,  1,  1,  1,  1,
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
    2, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 5,
};

static RE_UINT8 re_quotation_mark_stage_4[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 7, 8, 1, 1,
};

static RE_UINT8 re_quotation_mark_stage_5[] = {
      0,   0,   0,   0, 132,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   8,   0,   8,   0,   0,   0, 255,   0,   0,   0,   6,
      4,   0,   0,   0,   0,   0,   0,   0,   0, 240,   0, 224,   0,   0,   0,   0,
     30,   0,   0,   0,   0,   0,   0,   0, 132,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  12,   0,   0,   0,
};

/* Quotation_Mark: 209 bytes. */

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
    0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4,
};

static RE_UINT8 re_terminal_punctuation_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  9, 10, 11,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9, 12, 13,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 14,
    15,  9, 16,  9, 17, 18, 19, 20,  9, 21,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 22,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 23,
     9,  9,  9,  9,  9,  9, 24,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
};

static RE_UINT8 re_terminal_punctuation_stage_3[] = {
     0,  1,  1,  1,  1,  1,  2,  3,  1,  1,  1,  4,  5,  6,  7,  8,
     9,  1, 10,  1,  1,  1,  1,  1,  1,  1,  1,  1, 11,  1, 12,  1,
    13,  1,  1,  1,  1,  1, 14,  1,  1,  1,  1,  1, 15, 16, 17, 18,
    19,  1, 20,  1,  1, 21, 22,  1, 23,  1,  1,  1,  1,  1,  1,  1,
    24,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1, 25,  1,  1,  1, 26,  1,  1,  1,  1,  1,  1,  1,
     1, 27,  1,  1, 28, 29,  1,  1, 30, 31, 32, 33, 34, 35,  1, 36,
     1,  1,  1,  1, 37,  1, 38,  1,  1,  1,  1,  1,  1,  1,  1, 39,
    40,  1, 41,  1, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,  1,  1,
    52,  1,  1, 53, 54,  1, 55,  1,  1,  1,  1,  1, 56, 57,  1,  1,
    58,  1,  1,  1,  1,  1,  1,  1, 59,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1, 60, 61, 62,  1,  1, 41,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1, 63,  1,  1,
};

static RE_UINT8 re_terminal_punctuation_stage_4[] = {
     0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  3,  0,  0,  0,
     4,  0,  5,  0,  6,  0,  0,  0,  0,  0,  7,  0,  8,  0,  0,  0,
     0,  0,  0,  9,  0, 10,  2,  0,  0,  0,  0, 11,  0,  0, 12,  0,
    13,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0, 15,  0,  0,  0, 16,
     0,  0,  0, 17,  0, 18,  0,  0,  0,  0, 19,  0, 20,  0,  0,  0,
     0,  0, 11,  0,  0, 21,  0,  0,  0,  0, 22,  0,  0, 23,  0, 24,
     0, 25, 26,  0,  0, 27, 28,  0, 29,  0,  0,  0,  0,  0,  0, 24,
    30,  0,  0,  0,  0,  0,  0, 31,  0,  0,  0, 32,  0,  0, 33,  0,
     0, 34,  0,  0,  0,  0, 26,  0,  0,  0, 35,  0,  0,  0, 36, 37,
     0,  0,  0, 38,  0,  0, 39,  0,  1,  0,  0, 40, 36,  0, 41,  0,
     0,  0, 42,  0, 36,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0, 43,
     0, 44,  0,  0, 45,  0,  0,  0,  0,  0, 46,  0,  0, 24, 47,  0,
     0,  0, 48,  0,  0,  0, 49,  0,  0, 50,  0,  0,  0,  4,  0,  0,
     0,  0, 51,  0,  0,  0, 52,  0,  0,  0, 29,  0,  0, 53,  0,  0,
     0,  0, 54,  0, 55, 29,  0,  0,  0,  0, 48, 56,  0,  0,  0, 57,
     0,  0,  0, 33,  0,  0,  0, 58,  0, 59, 60,  0, 61,  0,  0,  0,
};

static RE_UINT8 re_terminal_punctuation_stage_5[] = {
      0,   0,   0,   0,   2,  80,   0, 140,   0,   0,   0,  64, 128,   0,   0,   0,
      0,   2,   0,   0,   8,   0,   0,   0,   0,  16,   0, 136,   0,   0,  16,   0,
    255,  23,   0,   0,   0,   0,   0,   3,   0,   0, 255, 127,  48,   0,   0,   0,
      0,   0,   0,  12,   0, 225,   7,   0,   0,  12,   0,   0, 254,   1,   0,   0,
      0,  96,   0,   0,   0,  56,   0,   0,   0,   0,  96,   0,   0,   0, 112,   4,
     60,   3,   0,   0,   0,  15,   0,   0,   0,   0,   0, 236,   0,   0,   0, 248,
      0,   0,   0, 192,   0,   0,   0,  48, 128,   3,   0,   0,   0,  64,   0,  16,
      2,   0,   0,   0,   6,   0,   0,   0,   0, 224,   0,   0,   0,   0, 248,   0,
      0,   0, 192,   0,   0, 192,   0,   0,   0, 128,   0,   0,   0,   0,   0, 224,
      0,   0,   0, 128,   0,   0,   3,   0,   0,   8,   0,   0,   0,   0, 247,   0,
     18,   0,   0,   0,   0,   0,   1,   0,   0,   0, 128,   0,   0,   0,  63,   0,
      0,   0,   0, 252,   0,   0,   0,  30, 128,  63,   0,   0,   3,   0,   0,   0,
     14,   0,   0,   0,  96,  32,   0, 192,   0,   0,   0,  31,   0,  56,   0,   8,
     60, 254, 255,   0,   0,   0,   0, 112,  12,   0,   0,   0,   0,   0,   0,  24,
      0,   0,   2,   0,   0,   0,  31,   0,   0,   0,  32,   0,   0,   0, 128,   3,
     16,   0,   0,   0, 128,   7,   0,   0,
};

/* Terminal_Punctuation: 898 bytes. */

RE_UINT32 re_get_terminal_punctuation(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_terminal_punctuation_stage_1[f] << 5;
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
    0, 1, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2,
};

static RE_UINT8 re_other_math_stage_2[] = {
    0, 1, 1, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 6, 1, 1,
};

static RE_UINT8 re_other_math_stage_3[] = {
     0,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     3,  4,  1,  5,  1,  6,  7,  8,  1,  9,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1, 10, 11,  1,  1,  1,  1, 12, 13, 14, 15,
     1,  1,  1,  1,  1,  1, 16,  1,
};

static RE_UINT8 re_other_math_stage_4[] = {
     0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  2,  3,  4,  5,  6,  7,  8,  0,  9, 10,
    11, 12, 13,  0, 14, 15, 16, 17, 18,  0,  0,  0,  0, 19, 20, 21,
     0,  0,  0,  0,  0, 22, 23, 24, 25,  0, 26, 27,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 25, 28,  0,  0,  0,  0, 29,  0, 30, 31,
     0,  0,  0, 32,  0,  0,  0,  0,  0, 33,  0,  0,  0,  0,  0,  0,
    34, 34, 35, 34, 36, 37, 38, 34, 39, 40, 41, 34, 34, 34, 34, 34,
    34, 34, 34, 34, 34, 42, 43, 44, 35, 35, 45, 45, 46, 46, 47, 34,
    38, 48, 49, 50, 51, 52,  0,  0,
};

static RE_UINT8 re_other_math_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,  64,   0,   0,  39,   0,   0,   0,  51,   0,
      0,   0,  64,   0,   0,   0,  28,   0,   1,   0,   0,   0,  30,   0,   0,  96,
      0,  96,   0,   0,   0,   0, 255,  31,  98, 248,   0,   0, 132, 252,  47,  62,
     16, 179, 251, 241, 224,   3,   0,   0,   0,   0, 224, 243, 182,  62, 195, 240,
    255,  63, 235,  47,  48,   0,   0,   0,   0,  15,   0,   0,   0,   0, 176,   0,
      0,   0,   1,   0,   4,   0,   0,   0,   3, 192, 127, 240, 193, 140,  15,   0,
    148,  31,   0,   0,  96,   0,   0,   0,   5,   0,   0,   0,  15,  96,   0,   0,
    192, 255,   0,   0, 248, 255, 255,   1,   0,   0,   0,  15,   0,   0,   0,  48,
     10,   1,   0,   0,   0,   0,   0,  80, 255, 255, 255, 255, 255, 255, 223, 255,
    255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223,
    255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255, 253, 255, 255, 247,
    255, 255, 255, 247, 255, 127, 255, 255, 255, 253, 255, 255, 247, 207, 255, 255,
    150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94, 255, 251, 255,  15,
    238, 251, 255,  15,
};

/* Other_Math: 502 bytes. */

RE_UINT32 re_get_other_math(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_other_math_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_other_math_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_other_math_stage_3[pos + f] << 3;
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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_other_alphabetic_stage_2[] = {
     0,  1,  2,  3,  3,  3,  3,  3,  3,  3,  4,  3,  3,  3,  3,  5,
     6,  7,  3,  3,  3,  3,  8,  3,  3,  3,  3,  9,  3,  3, 10, 11,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
};

static RE_UINT8 re_other_alphabetic_stage_3[] = {
     0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
    13,  0,  0, 14,  0,  0,  0, 15, 16, 17, 18, 19, 20, 21,  0,  0,
     0,  0,  0,  0, 22,  0,  0,  0,  0,  0,  0,  0,  0, 23,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 24,  0, 25, 26, 27, 28,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29,  0,  0,  0,  0,
     0,  0,  0, 30,  0,  0,  0,  0,  0,  0, 31,  0,  0,  0,  0,  0,
    32, 33, 34, 35, 36, 37, 38, 39,  0,  0, 40,  0, 41, 42,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 43,  0,  0,  0, 44,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 45,  0,  0,  0,
    46,  0,  0,  0,  0,  0,  0,  0,  0, 47,  0,  0,  0,  0,  0,  0,
     0, 48,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_other_alphabetic_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   2,   3,   0,   4,   0,   5,   6,   0,   0,   7,   8,
      9,  10,   0,   0,   0,  11,   0,   0,  12,  13,   0,   0,   0,   0,  14,  15,
     16,  17,  18,  19,  20,  21,  22,  19,  20,  21,  23,  24,  20,  21,  25,  26,
     20,  21,  27,  19,  28,  21,  29,   0,  16,  21,  30,  19,  20,  21,  30,  19,
     16,  21,  31,  19,  19,   0,  32,  33,   0,  34,  35,   0,   0,  36,  35,   0,
      0,   0,   0,  37,  38,  39,   0,   0,   0,  40,  41,  42,  43,   0,   0,   0,
      0,   0,  44,   0,   0,   0,   0,   0,  33,  33,  33,  33,   0,  45,  46,   0,
      0,   0,   0,   0,  47,  48,   0,   0,   0,  49,   0,   0,   0,   0,   0,   0,
     50,   0,  51,  52,   0,   0,   0,   0,  53,  54,  16,   0,  55,  56,   0,  57,
      0,  58,   0,   0,   0,   0,   0,  33,   0,   0,   0,   0,   0,   0,   0,  59,
      0,   0,   0,   0,   0,  45,  60,  61,   0,   0,   0,   0,   0,   0,   0,  60,
      0,   0,   0,  62,  21,   0,   0,   0,   0,  63,   0,   0,  64,  14,  65,   0,
      0,  66,  67,   0,  16,  14,   0,   0,   0,  68,  69,   0,   0,  70,   0,  71,
      0,   0,   0,   0,   0,   0,   0,  72,  73,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,  74,   0,   0,   0,   0,  75,   0,   0,   0,   0,   0,   0,   0,
     55,  76,  77,   0,  28,  78,   0,   0,  55,  67,   0,   0,  55,  79,   0,   0,
      0,  80,   0,   0,   0,   0,  44,  46,  16,  21,  22,  19,   0,   0,   0,   0,
      0,  54,  81,   0,   0,  10,  64,   0,   0,   0,   0,   0,   0,  82,  83,   0,
      0,  84,  85,   0,   0,  86,   0,   0,  87,  88,   0,   0,   0,   0,   0,   0,
     89,  90,  91,   0,  92,   0,   0,   0,   0,  93,   0,   0,  94,  95,   0,   0,
      0,  96,  97,   0,   0,   0,   0,   0,   0,  98,   0,   0,   0,   0,   0,   0,
      0,   0,  37,  99,   0,   0,   0,   0,   0,   0,   0,   0,  73,   0,   0,   0,
    100, 101,   0,   0,   0,   0,   0,   0,   0,   0, 102,   0,   0,   0,   0,   0,
      0,  10, 103, 103,  61,   0,   0,   0,
};

static RE_UINT8 re_other_alphabetic_stage_5[] = {
      0,   0,   0,   0,  32,   0,   0,   0,   0,   0, 255, 191, 182,   0,   0,   0,
      0,   0, 255,   7,   0, 248, 255, 254,   0,   0,   1,   0,   0,   0, 192,  31,
    158,  33,   0,   0,   0,   0,   2,   0,   0,   0, 255, 255, 192, 255,   1,   0,
      0,   0, 192, 248, 239,  30,   0,   0,   0,   0, 240, 255, 248,   3, 255, 255,
     15,   0,   0,   0,   0,   0,   0, 204, 255, 223, 224,   0,  12,   0,   0,   0,
     14,   0,   0,   0,   0,   0,   0, 192, 159,  25, 128,   0, 135,  25,   2,   0,
      0,   0,  35,   0, 191,  27,   0,   0,  12,   0,   0,  28, 159,  25, 192,   0,
      4,   0,   0,   0, 199,  29, 128,   0, 223,  29,  96,   0, 223,  29, 128,   0,
      0, 128,  95, 255,   0,   0,  12,   0,   0,   0, 242,   7,   0,  32,   0,   0,
      0,   0, 242,  27,   0,   0, 254, 255,   3, 224, 255, 254, 255, 255, 255,  31,
      0, 248, 127, 121,   0,   0, 192, 195, 133,   1,  30,   0, 124,   0,   0,  48,
      0,   0,   0, 128,   0,   0, 192, 255, 255,   1,   0,   0,  96,   0,   0,   0,
      0,   2,   0,   0, 255,  15, 255,   1,   0,   0, 128,  15,   0,   0, 224, 127,
    254, 255,  31,   0,  31,   0,   0,   0,   0,   0, 224, 255,   7,   0,   0,   0,
    254,  51,   0,   0, 128, 255,   3,   0, 240, 255,  63,   0, 128, 255,  31,   0,
    255, 255, 255, 255, 255,   3,   0,   0,   0,   0, 240,  15, 248,   0,   0,   0,
      3,   0,   0,   0,  47,   0,   0,   0, 192,   7,   0,   0, 128, 255,   7,   0,
      0, 254, 127,   0,   8,  48,   0,   0,   0,   0, 157,  65,   0, 248,  32,   0,
    248,   7,   0,   0,   0,   0,   0,  64,   0,   0, 192,   7, 110, 240,   0,   0,
      0,   0,   0, 255,  63,   0,   0,   0,   0,   0, 255,   1,   0,   0, 248, 255,
      0, 240, 159,  64,  59,   0,   0,   0,   0, 128,  63, 127,   0,   0,   0,  48,
      0,   0, 255, 127,   1,   0,   0,   0,   0, 248,  63,   0,   0,   0,   0, 224,
    255,   7,   0,   0, 254,   7,   0,   0,   0,   0, 224, 123,   0,   0, 254,  15,
      0, 252, 255,   0,   0, 128, 127, 127,   0,   0, 252, 255, 255, 254, 127,   0,
      0,   0, 126, 180, 139,   0,   0,   0,   0,   0, 127,   0, 255, 255, 255, 127,
    127, 255, 255, 249, 219,   7,   0,   0, 128,   0,   0,   0, 255,   3, 255, 255,
};

/* Other_Alphabetic: 1065 bytes. */

RE_UINT32 re_get_other_alphabetic(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_other_alphabetic_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_other_alphabetic_stage_2[pos + f] << 4;
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
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3,
};

static RE_UINT8 re_ideographic_stage_2[] = {
     0,  0,  0,  1,  2,  3,  3,  3,  3,  4,  0,  0,  0,  0,  0,  5,
     0,  0,  0,  0,  0,  0,  0,  3,  6,  0,  0,  7,  0,  0,  0,  0,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  8,  9, 10,  3, 11, 12,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_ideographic_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  3,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  4,  0,  0,  0,  0,  5,  6,  0,  0,
     2,  2,  2,  7,  2,  8,  0,  0,  9, 10,  0,  0,  0,  0,  0,  0,
     2,  2,  2, 11,  2,  2,  2,  2,  2,  2,  2, 12, 13,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2, 14,  2,  2,  2,  2,  2, 15,  0,  0,
     0,  0,  0,  0,  2, 16,  0,  0,
};

static RE_UINT8 re_ideographic_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  0,
     2,  2,  2,  2,  2,  2,  2,  4,  0,  0,  0,  0,  2,  2,  2,  2,
     2,  5,  2,  6,  0,  0,  0,  0,  2,  2,  2,  2,  2,  2,  2,  7,
     2,  2,  2,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  2,  2,
     2,  2,  2, 10,  0,  0,  0,  0,  2,  2,  2, 11,  2,  2,  2,  2,
     2,  2,  2,  2, 12,  2,  2,  2, 13,  2,  2,  2,  2,  2,  2,  2,
     2,  2, 14,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 15,
    16,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_ideographic_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0, 192,   0,   0,   0, 254,   3,   0,   7,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  63,   0,
    255, 255, 255, 255, 255,   7,   0,   0, 255, 255, 255, 255, 255,  63, 255, 255,
    255, 255, 255,   3,   0,   0,   0,   0, 255, 255, 255, 255, 255,  31,   0,   0,
    255, 255, 255, 255, 255, 255,   7,   0,   0,   0,   0,   0,   0,   0, 255, 255,
    255, 255, 255, 255, 255, 255, 255,  15, 255, 255, 127,   0,   0,   0,   0,   0,
    255, 255, 255, 255, 255, 255,  31,   0, 255, 255, 255,  63, 255, 255, 255, 255,
    255, 255, 255, 255,   3,   0, 255, 255, 255, 255, 255, 255,   1,   0,   0,   0,
    255, 255, 255,  63,   0,   0,   0,   0,
};

/* Ideographic: 457 bytes. */

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
    10, 11, 12, 13,  4,  4,  4,  4,  4,  4,  4,  4,  4, 14,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 15,  4,  4, 16,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
};

static RE_UINT8 re_diacritic_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1,  1,  1,  1,  1,  1, 17,  1, 18, 19, 20, 21, 22,  1, 23,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 24,  1, 25,  1,
    26,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 27, 28,
    29, 30, 31, 32,  1,  1,  1,  1,  1,  1,  1, 33,  1,  1, 34, 35,
     1,  1, 36,  1,  1,  1,  1,  1,  1,  1, 37,  1,  1,  1,  1,  1,
    38, 39, 40, 41, 42, 43, 44, 45,  1,  1, 46,  1, 47, 48,  1,  1,
     1,  1, 49,  1,  1,  1,  1, 50,  1, 51,  1,  1,  1,  1,  1,  1,
    52, 53,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_diacritic_stage_4[] = {
     0,  0,  1,  2,  0,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  4,  5,  5,  5,  5,  6,  7,  8,  0,  0,  0,
     0,  0,  0,  0,  9,  0,  0,  0,  0,  0, 10,  0, 11, 12, 13,  0,
     0,  0, 14,  0,  0,  0, 15, 16,  0,  4, 17,  0,  0, 18,  0, 19,
    20,  0,  0,  0,  0,  0,  0, 21,  0, 22, 23, 24,  0, 22, 25,  0,
     0, 22, 25,  0,  0, 22, 25, 26,  0, 22, 25,  0,  0,  0, 25,  0,
     0,  0, 25,  0,  0, 22, 25,  0,  0, 27, 25,  0,  0,  0, 28,  0,
     0,  0, 29,  0,  0,  0, 30,  0, 20, 31,  0,  0, 32,  0, 33,  0,
     0, 34,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0, 36,  0,
     0, 37,  0,  0,  0,  0,  0,  0,  0,  0,  0, 38,  0, 39,  0,  0,
     0, 40, 41, 42,  0, 43,  0,  0,  0, 44,  0, 45,  0,  0,  4, 46,
     0, 47,  5, 17,  0,  0, 48, 49,  0,  0,  0,  0,  0, 50, 51, 52,
     0,  0,  0,  0,  0,  0,  0, 53,  0, 54,  0,  0,  0,  0,  0,  0,
     0, 55,  0,  0, 56,  0,  0, 22,  0,  0,  0, 57, 58,  0,  0, 59,
    60, 61,  0,  0, 62,  0,  0, 20,  0,  0,  0,  0,  0,  0, 41, 63,
     0, 64, 65,  0,  0, 65,  2, 66,  0,  0,  0, 67,  0, 15, 68, 69,
     0,  0, 70,  0,  0,  0,  0, 71,  1,  0,  0,  0,  0,  0,  0,  0,
     0, 72,  0,  0,  0,  0,  0,  0,  0,  1,  2, 73, 74,  0,  0, 75,
     0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0, 76,
     0,  0,  0,  0,  0, 77,  0,  0,  0, 78,  0, 65,  0,  0, 79,  0,
     0, 80,  0,  0,  0,  0,  0, 81,  0, 22, 25, 82,  0,  0,  0,  0,
     0,  0, 83,  0,  0,  0, 84,  0,  0,  0,  0,  0,  0, 15,  2,  0,
     0, 15,  0,  0,  0, 44,  0,  0,  0, 85,  0,  0,  0,  0,  0,  0,
     0, 40, 86,  0, 10,  0,  0,  0,  0, 15,  0,  0,  0,  0,  0,  0,
     0,  0, 87,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 88,
     0,  0,  0,  0, 89,  0,  0,  0,  0,  0,  0, 90, 91, 92,  0,  0,
     0,  0,  0,  0,  0,  0, 93,  0,  0,  0, 94,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_diacritic_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,  64,   1,   0,   0,   0,   0, 129, 144,   1,
      0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 255, 224,   7,   0,  48,   4,
     48,   0,   0,   0, 248,   0,   0,   0,   0,   0,   0,   2,   0,   0, 254, 255,
    251, 255, 255, 191,  22,   0,   0,   0,   0, 248, 135,   1,   0,   0,   0, 128,
     97,  28,   0,   0, 255,   7,   0,   0, 192, 255,   1,   0,   0, 248,  63,   0,
      0,   0,   0,   3, 248, 255, 255, 127,   0,   0,   0,  16,   0,  32,  30,   0,
      0,   0,   2,   0,   0,  32,   0,   0,   0,   0,   0, 224,   0,   0,   0,  24,
      0,   4,   0,   0, 128,  95,   0,   0,   0,  31,   0,   0,   0,   0, 160, 194,
    220,   0,   0,   0,  64,   0,   0,   0,   0,   0, 128,   6, 128, 191,   0,  12,
      0, 254,  15,  32,   0,   0,   0,  14,   0,   0, 224, 159,   0,   0, 255,  63,
      0,   0,  16,   0,  16,   0,   0,   0,   0, 248,  15,   0,   0,  12,   0,   0,
      0,   0, 192,   0,   0,   0,   0,  63, 255,  33, 144,   3,   0, 240, 255, 255,
    240, 255,   0,   0,   0,   0, 224, 227,   0,   0,   0, 160,   3, 224,   0, 224,
      0, 224,   0,  96,   0, 128,   3,   0,   0, 128,   0,   0,   0, 252,   0,   0,
      0,   0,   0,  30,   0, 128,   0, 176,   0,   0,   0,  48,   0,   0,   3,   0,
      0,   0, 128, 255,   3,   0,   0,   0,   0,   1,   0,   0, 255, 255,   3,   0,
      0, 120,   0,   0,   0,   0,   8,   0,  32,   0,   0,   0,   0,   0,   0,  56,
      7,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0, 248,   0,  48,   0,   0,
    255, 255,   0,   0,   0,   0,   1,   0,   0,   0,   0, 192,   8,   0,   0,   0,
     96,   0,   0,   0,   0,   0,   0,   6,   0,   0,  24,   0,   1,  28,   0,   0,
      0,   0,  96,   0,   0,   6,   0,   0, 192,  31,  31,   0,  68,   0,   0,   0,
     12,   0,   0,   0,   0,   8,   0,   0, 128,   0,   0,   0,  52,   0,   0,   0,
      0,   0,  31,   0,   0, 128, 255, 255, 128, 227,   7, 248, 231,  15,   0,   0,
      0,  60,   0,   0,   0,   0, 127,   0, 112,   7,   0,   0,
};

/* Diacritic: 1061 bytes. */

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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_extender_stage_2[] = {
     0,  1,  2,  3,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  5,  6,  2,  2,  2,  2,  2,  2,  2,  2,  2,  7,
     2,  2,  8,  9,  2,  2,  2,  2,  2,  2,  2,  2,  2, 10,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 11,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
};

static RE_UINT8 re_extender_stage_3[] = {
     0,  1,  2,  1,  1,  1,  3,  4,  1,  1,  1,  1,  1,  1,  5,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  6,  1,  7,  1,  8,  1,  1,  1,
     9,  1,  1,  1,  1,  1,  1,  1, 10,  1,  1,  1,  1,  1, 11,  1,
     1, 12, 13,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 14,
     1,  1,  1, 15,  1, 16,  1,  1,  1,  1, 17,  1,  1,  1,  1,  1,
     1,  1,  1, 18,  1,  1,  1, 19,  1, 20,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_extender_stage_4[] = {
     0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  3,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  5,  0,  0,  0,  5,  0,
     6,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,
     0,  9,  0, 10,  0,  0,  0,  0, 11, 12,  0,  0, 13,  0,  0, 14,
    15,  0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 17,  5,  0,  0,  0, 18,  0,  0, 19, 20,
     0,  0,  0, 18,  0,  0,  0,  0,  0,  0, 19,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 21,  0,  0,  0,  0,  0, 22,  0,  0,  0,
     0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24,
     0,  0, 25,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_extender_stage_5[] = {
      0,   0,   0,   0,   0,   0, 128,   0,   0,   0,   3,   0,   1,   0,   0,   0,
      0,   0,   0,   4,  64,   0,   0,   0,   0,   4,   0,   0,   8,   0,   0,   0,
    128,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   8,  32,   0,   0,   0,
      0,   0,  62,   0,   0,   0,   0,  96,   0,   0,   0, 112,   0,   0,  32,   0,
      0,  16,   0,   0,   0, 128,   0,   0,   0,   0,   1,   0,   0,   0,   0,  32,
      0,   0,  24,   0, 192,   1,   0,   0,   0,   0,   0,   1,  12,   0,   0,   0,
      3,   0,   0,   0, 112,   0,   0,   0,
};

/* Extender: 481 bytes. */

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
    2, 2, 2, 7, 2, 8, 2, 2,
};

static RE_UINT8 re_other_lowercase_stage_4[] = {
     0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  2,  3,  0,  4,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  6,  7,  0,
     0,  8,  9,  0,  0, 10,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,
     0, 12,  0,  0,  0,  0,  0,  0,  0,  0, 13,  0,  0, 14,  0, 15,
     0,  0,  0,  0,  0, 16,  0,  0,
};

static RE_UINT8 re_other_lowercase_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   0,   4,
      0,   0,   0,   0,   0,   0, 255,   1,   3,   0,   0,   0,  31,   0,   0,   0,
     32,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0, 240, 255, 255,
    255, 255, 255, 255, 255,   7,   0,   1,   0,   0,   0, 248, 255, 255, 255, 255,
      0,   0,   0,   0,   0,   0,   2, 128,   0,   0, 255,  31,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255, 255,   0,   0, 255, 255, 255,   3,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  48,   0,   0,   0,  48,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   3,
      0,   0,   0, 240,   0,   0,   0,   0,
};

/* Other_Lowercase: 297 bytes. */

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
    0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1,
};

static RE_UINT8 re_other_uppercase_stage_2[] = {
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
};

static RE_UINT8 re_other_uppercase_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_uppercase_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 1, 0, 0, 3, 4, 4, 5, 0, 0, 0,
};

static RE_UINT8 re_other_uppercase_stage_5[] = {
      0,   0,   0,   0, 255, 255,   0,   0,   0,   0, 192, 255,   0,   0, 255, 255,
    255,   3, 255, 255, 255,   3,   0,   0,
};

/* Other_Uppercase: 162 bytes. */

RE_UINT32 re_get_other_uppercase(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_other_uppercase_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_other_uppercase_stage_2[pos + f] << 3;
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
    0, 1, 1, 2, 3, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_other_grapheme_extend_stage_2[] = {
    0, 1, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 5, 0, 0, 0, 0, 0,
    0, 0, 6, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_grapheme_extend_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  2,  3,  4,  0,  0,
     5,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  8,  9, 10,  0,  0,
     0, 11,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_other_grapheme_extend_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  0,
     0,  1,  2,  0,  0,  1,  2,  0,  0,  0,  0,  0,  0,  0,  3,  0,
     0,  1,  2,  0,  0,  0,  4,  0,  5,  0,  0,  0,  0,  0,  0,  0,
     0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,
     0,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,
     0,  0,  0,  0,  0,  9,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,
     0, 11, 11, 11,  0,  0,  0,  0,
};

static RE_UINT8 re_other_grapheme_extend_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,  64,   0,   0, 128,   0,   4,   0,  96,   0,
      0, 128,   0, 128,   0,  16,   0,   0,   0, 192,   0,   0,   0,   0,   0, 192,
      0,   0,   1,  32,   0, 128,   0,   0,  32, 192,   7,   0, 255, 255, 255, 255,
};

/* Other_Grapheme_Extend: 332 bytes. */

RE_UINT32 re_get_other_grapheme_extend(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_other_grapheme_extend_stage_1[f] << 3;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_other_grapheme_extend_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_other_grapheme_extend_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_other_grapheme_extend_stage_4[pos + f] << 5;
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
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 6, 7, 8, 3, 9, 0,
};

static RE_UINT8 re_unified_ideograph_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 4, 0, 0,
    1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 6, 7, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 9, 0, 0,
};

static RE_UINT8 re_unified_ideograph_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1, 3,
    4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 5, 1, 1, 1, 1,
    1, 1, 1, 1, 6, 1, 1, 1, 7, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9,
};

static RE_UINT8 re_unified_ideograph_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,  63,   0, 255, 255, 255, 255, 255,   7,   0,   0,
      0, 192,  26, 128, 154,   3,   0,   0, 255, 255, 127,   0,   0,   0,   0,   0,
    255, 255, 255, 255, 255, 255,  31,   0, 255, 255, 255,  63, 255, 255, 255, 255,
    255, 255, 255, 255,   3,   0, 255, 255, 255, 255, 255, 255,   1,   0,   0,   0,
};

/* Unified_Ideograph: 305 bytes. */

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
      0,   0,   0,   0,  32,   0,   0,   0,   0,   0,   0,   0,  16,   0,   0,   0,
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
    0, 6, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_deprecated_stage_5[] = {
      0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   8,   0,   0,   0, 128,   2,
     24,   0,   0,   0,   0, 252,   0,   0,   0,   6,   0,   0,   2,   0,   0,   0,
};

/* Deprecated: 226 bytes. */

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
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_logical_order_exception_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
};

static RE_UINT8 re_logical_order_exception_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_logical_order_exception_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,  31,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 224,   4,   0,   0,   0,   0,   0,   0,  96,  26,
};

/* Logical_Order_Exception: 145 bytes. */

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
    0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_id_start_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_id_start_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_other_id_start_stage_5[] = {
     0,  0,  0,  0,  0,  0,  0,  0, 96,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  1,  0, 64,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,
};

/* Other_ID_Start: 145 bytes. */

RE_UINT32 re_get_other_id_start(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_other_id_start_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_other_id_start_stage_2[pos + f] << 3;
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

/* Sentence_Terminal. */

static RE_UINT8 re_sentence_terminal_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_sentence_terminal_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  8,  9,  7,  7,  7,  7,  7,  7,  7,  7,  7, 10,
     7, 11, 12, 13,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14,  7,  7,
     7,  7,  7,  7,  7,  7,  7, 15,  7,  7,  7, 16,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
};

static RE_UINT8 re_sentence_terminal_stage_3[] = {
     0,  1,  1,  1,  1,  2,  3,  4,  1,  5,  1,  1,  1,  1,  1,  1,
     6,  1,  1,  7,  1,  1,  8,  9, 10, 11, 12, 13, 14,  1,  1,  1,
    15,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 16,  1,
    17,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1, 18,  1, 19,  1, 20, 21, 22, 23,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1, 24, 25,  1,  1, 26,  1,  1,  1,  1,  1,
    27, 28, 29,  1, 30, 31, 32, 33,  1,  1, 34,  1, 32,  1,  1,  1,
     1,  1, 35, 36,  1,  1,  1,  1,  1,  1,  1,  1, 37,  1,  1,  1,
     1,  1, 38,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_sentence_terminal_stage_4[] = {
     0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  2,  0,  0,  0,  3,  0,  0,  0,  0,  0,  4,  0,
     5,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  7,  0,  0,  0,  0,
     0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  0,  0,  0,
     0,  0,  0, 10,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,  0,
    12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 13,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,
     0, 15,  0, 16,  0,  0,  0,  0,  0, 17, 18,  0,  0,  0,  0,  0,
     0, 19,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  3, 21,  0,  0,  0,  0,  0,  0, 22,
     0,  0,  0, 23,  0,  0, 21,  0,  0, 24,  0,  0,  0,  0, 25,  0,
     0,  0, 26,  0,  0,  0,  0, 27,  0,  0,  0,  0,  0,  0,  0, 28,
     0,  0, 29,  0,  0,  0,  0,  0,  1,  0,  0, 30,  0,  0,  0,  0,
     0,  0, 23,  0,  0,  0,  0,  0,  0,  0, 31,  0,  0, 16, 32,  0,
     0,  0, 33,  0,  0,  0, 34,  0,  0, 35,  0,  0,  0,  2,  0,  0,
     0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 37,  0,
     0,  0, 38,  0,  0,  0,  0,  0,  0, 39,  0,  0,  0,  0,  0,  0,
     0,  0, 40,  0, 15,  0,  0,  0,  0,  0,  0, 21,  0,  0,  0, 41,
     0, 42, 43,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,
     0,  0,  0,  0, 44,  0,  0,  0,
};

static RE_UINT8 re_sentence_terminal_stage_5[] = {
      0,   0,   0,   0,   2,  64,   0, 128,   0,   2,   0,   0,   0,   0,   0, 128,
      0,   0,  16,   0,   7,   0,   0,   0,   0,   0,   0,   2,  48,   0,   0,   0,
      0,  12,   0,   0, 132,   1,   0,   0,   0,  64,   0,   0,   0,   0,  96,   0,
      8,   2,   0,   0,   0,  15,   0,   0,   0,   0,   0, 204,   0,   0,   0,  24,
      0,   0,   0, 192,   0,   0,   0,  48, 128,   3,   0,   0,   0,  64,   0,  16,
      4,   0,   0,   0,   0, 192,   0,   0,   0,   0, 136,   0,   0,   0, 192,   0,
      0, 128,   0,   0,   0,   3,   0,   0,   0,   0,   0, 224,   0,   0,   3,   0,
      0,   8,   0,   0,   0,   0, 196,   0,   2,   0,   0,   0, 128,   1,   0,   0,
      3,   0,   0,   0,  14,   0,   0,   0,  96,  32,   0, 192,   0,   0,   0,  27,
      0,  24,   0,   0,  12, 254, 255,   0,   6,   0,   0,   0,   0,   0,   0, 112,
     12,   0,   0,   0,   0,   0,  32,   0,   0,   0, 128,   1,  16,   0,   0,   0,
      0,   1,   0,   0,
};

/* Sentence_Terminal: 741 bytes. */

RE_UINT32 re_get_sentence_terminal(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_sentence_terminal_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_sentence_terminal_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_sentence_terminal_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_sentence_terminal_stage_4[pos + f] << 5;
    pos += code;
    value = (re_sentence_terminal_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

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

/* Prepended_Concatenation_Mark. */

static RE_UINT8 re_prepended_concatenation_mark_stage_1[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1,
};

static RE_UINT8 re_prepended_concatenation_mark_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_prepended_concatenation_mark_stage_3[] = {
    0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_prepended_concatenation_mark_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 3, 0, 0, 0,
    0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_prepended_concatenation_mark_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,  63,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,  32,   0,   0,   0,   0,   0, 128,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  32,
};

/* Prepended_Concatenation_Mark: 162 bytes. */

RE_UINT32 re_get_prepended_concatenation_mark(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_prepended_concatenation_mark_stage_1[f] << 3;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_prepended_concatenation_mark_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_prepended_concatenation_mark_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_prepended_concatenation_mark_stage_4[pos + f] << 6;
    pos += code;
    value = (re_prepended_concatenation_mark_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Regional_Indicator. */

static RE_UINT8 re_regional_indicator_stage_1[] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static RE_UINT8 re_regional_indicator_stage_2[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static RE_UINT8 re_regional_indicator_stage_3[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_regional_indicator_stage_4[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static RE_UINT8 re_regional_indicator_stage_5[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 192, 255, 255, 255,
};

/* Regional_Indicator: 97 bytes. */

RE_UINT32 re_get_regional_indicator(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_regional_indicator_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_regional_indicator_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_regional_indicator_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_regional_indicator_stage_4[pos + f] << 6;
    pos += code;
    value = (re_regional_indicator_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

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
     8,  9,  5,  5,  5,  5, 10,  5,  5,  5,  5, 11,  5, 12, 13, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
    16,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
};

static RE_UINT8 re_bidi_class_stage_2[] = {
      0,   1,   2,   3,   4,   4,   4,   4,   4,   4,   5,   6,   7,   8,   9,  10,
      4,   4,  11,   4,   4,   4,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,
     22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  30,  32,  33,  34,  35,  36,
     37,  38,  28,  39,  40,  41,   4,  42,  43,  44,  45,  46,  47,  48,  49,  50,
     51,  52,  53,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  54,  55,   4,
     56,   4,   4,   4,   4,   4,   4,   4,   4,   4,  57,   4,  58,  59,  60,  61,
     62,   4,  63,   4,  64,  65,   4,  66,  67,  68,  69,   4,  70,  71,  72,  73,
     74,   4,   4,  75,   4,   4,   4,  76,   4,   4,   4,   4,   4,   4,  77,  78,
     79,  80,  81,  82,  83,  84,  85,  86,  87,  86,  86,  86,  88,  89,  90,  86,
     91,  92,  93,  94,  86,  86,  86,  86,  86,  86,  95,  86,  86,  86,  86,  86,
      4,   4,   4,   4,  86,  86,  86,  86,  86,  86,  86,  86,  86,  96,  97,  98,
      4,   4,   4,  99,   4, 100,   4, 101,  86, 102, 103, 104,  86,  86,  86, 105,
    106,   4, 107, 108,   4,   4,   4, 109, 110, 111, 112, 113,   4, 114,   4, 115,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,  86,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4, 116, 117,   4,   4,   4,   4, 118, 119, 120, 121, 122,   4, 123,   4,
    124, 125,   4, 126, 127, 128, 129, 130, 131, 132, 133, 134,   4,   4,   4, 135,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 136, 137,  16,  16,
     16,  16,  16,  16, 138,  16,  16, 139, 140, 141,  16, 142, 143, 144,   4, 145,
      4,   4,   4,   4, 146,  86, 147, 148,   4,   4,   4, 149,   4, 150,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    151, 151, 151, 151, 152, 151, 151, 151, 153, 151, 151, 154, 155, 151, 151, 151,
    151, 151, 151, 151, 151, 151, 151, 151, 151, 156, 151, 151, 151, 151, 151, 151,
    157, 158, 159,   4, 160, 161, 162, 163, 164,   4,   4, 165, 166, 167,   4,   4,
    168, 169, 170, 171,   4,   4, 172, 173, 174, 175, 176,   4, 177,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4, 178, 179, 180,   4,   4,   4,   4,   4,
    181,   4, 182,   4, 183, 184,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 185, 186,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 187,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4, 188,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4, 189, 190,   4,  86, 191,   4,   4,  86, 192,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 193, 194, 195, 196, 197,
      4,   4,   4,   4,   4,   4,   4,   4, 198, 199, 200,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    201,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    151, 151, 151, 202, 151, 203, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151,
    151, 151, 151, 151, 151, 151, 151, 151,  16,  16,  16, 204, 151, 151, 151, 151,
    205,  86, 206, 207, 208, 209,   4,   4,   4, 210,   4,   4,  86,  86,  86,  86,
     86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86, 211,  86, 104,  86, 212,
    213, 214, 215,   4, 216, 217, 218, 219,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 220,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 220,
    221, 221, 221, 221,   7,   7,   7, 222, 221, 221, 221, 221, 221, 221, 221, 221,
    221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221,
    221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221,
    221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221,
};

static RE_UINT8 re_bidi_class_stage_3[] = {
      0,   1,   0,   2,   3,   4,   5,   6,   7,   8,   8,   9,   7,   8,   8,  10,
     11,   0,   0,   0,  12,  13,  14,  15,   8,   8,  16,   8,   8,   8,  16,   8,
      8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  17,
     18,  19,  18,  19,  20,  21,  19,  19,  22,  22,  22,  22,  22,  22,  22,  22,
     22,  22,  22,  22,  22,  22,  23,  24,  25,   8,   8,   8,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8,  24,   8,  26,  27,   8,   8,   8,   8,   8,   8,
      8,  28,  29,  22,  22,  22,  22,  30,  31,  32,  32,  32,  32,  32,  32,  32,
     33,  34,  22,  35,  36,  36,  36,  36,  36,  37,  22,  22,  38,  39,  40,  36,
     36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  41,  42,  43,  44,   5,  45,
     36,  36,  46,  36,  36,  36,  22,  22,  22,  35,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  41,  22,  40,  36,  32,  32,  32,  32,  32,  47,  48,  49,
     32,  32,  50,  51,  52,  53,  32,  32,  32,  32,  32,  54,  36,  36,  32,  32,
     32,  32,  32,  32,  36,  36,  36,  36,  36,  36,  55,  22,  56,  22,  22,  22,
     57,   8,   8,   8,   8,   8,   8,  58,  59,  60,  59,   8,  61,   8,   8,   8,
     62,   8,   8,   8,   8,   8,   8,  63,  64,  65,   8,   8,  61,   8,  66,  67,
     68,   8,   8,   8,   8,   8,   8,  63,  69,  70,  62,   8,   8,   8,  71,   8,
     72,  60,   8,   8,  61,   8,  73,  74,  62,   8,   8,   8,   8,   8,   8,  75,
     64,  65,  76,   8,  61,   8,   8,   8,  77,   8,   8,   8,   8,   8,   8,   8,
     78,  65,   8,   8,   8,   8,   9,  79,  78,   8,   8,   8,   8,   8,   8,  80,
     81,  82,  83,   8,  61,   8,   8,  84,   8,  85,   8,   8,  61,   8,   8,   8,
     27,   8,   8,   8,   8,   8,   8,  86,  64,  65,   8,   8,  61,   8,   8,   8,
      8,  77,  87,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  88,  89,
     90,  91,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  88,  92,
      8,  93,   8,   8,   8,   8,   8,   8,   8,   8,   8,  27,   8,   8,  94,  95,
      8,   8,   8,   8,   8,   8,  59,  91,  96,  97,  22,  59,  22,  22,  22,  98,
     76,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  97,  99, 100,
      8,   8,   8, 101,  78,   8,  64,   8, 102,  65,   8,  65,   8,   8,   8,   8,
      8,   8,   8,  97,   8,   8,   8,   8,   8,   8,  19, 103,   8,   8,   8,   8,
      7,   8,   8,   8,   8,   8,   8,   8, 104,   8,   8, 105,   8,   8,   8,   8,
      8,   8, 106,   8,   8,   8, 106,   8,   8,   8,  61,   8,   8,   8,  61,   8,
      8,   8,   8,   8,   8,   8, 107,  93,  76,  59, 108, 109,   8,   8,  19, 103,
     19, 110,   8,   8,   8,   8,   8,   8,  83,   8,   8,   8,   8,  62,   8,   8,
      8,   8,   8,   8, 111,  78,  77, 112, 113,   8,   8,   8,   8,   8,   8,   8,
      8,   8,   8, 114,  19,  19,  19,  19,   8,   8,  90, 115,   8,   8,   8,   8,
      8,   8,  76,  91, 116,  98,  26, 117,   8,   8,   8,   8,   8,   8,  22,  91,
    108,   8,   8,   8,   8,   8, 118, 119,  77,   8,   8,   8,   8,  26, 108,   8,
     27,   8,   8,   8, 120, 121,   8,   8,   8,   8,   8,   8,  76, 122,  27,   8,
      8,   8,   8,   8,   8, 123, 124,   8,   8,   8, 125,  22,  99,  60,  63,  27,
     22,  22,  22,  22,  22,  22,  22, 126,   8,   8,   8,   8,   8,   8,   8, 127,
    103,  20,   8,  20,   8,  20,   8, 128, 129, 130,  19,  19,  19, 131, 132,  19,
    133,  19,  19, 134, 135, 136, 137, 138,   5, 138,   8,   8, 139, 139, 139, 139,
    139, 139,  22,  22,  22,  22,  78,   8, 140, 103, 141, 142, 143, 144,   8, 145,
    146, 147,  19,  19,   8,   8,   8,   8,   8, 148,  19,  19,  19,  19,  19,  19,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19, 149,  19,  19,  19,  19,  19,
     19,  19,  19,  19,  19,  19, 150,   8,   8,   8,   8,   8,   8,   8,   8,   9,
     19,  19, 151,  19,  19,  19,  19,  19,  19,  19,  19,  19,  84,   8,   8,   8,
     19, 152,   8,   8,  19,  19,  19,  19,  19,   5,   5, 153,   8,   8,   8,   8,
      8,   8,   8,   8,   8,  18,  19,  19,  19,  19,  19,  19,  19, 154,  19,  19,
     19,  19,  19,  19,  19,  19, 155,  19,  19,  19, 150,  19,  19,  19,  19, 156,
     19, 157, 152,   8,   8, 158,   8,   8,   8,   8,   8,   8,  20, 159,  27, 160,
      8,   8,   8,   8,   8,   8,   8,  90,   8,   8,   8,   8,  22,  22,  22,  22,
     19, 103,   8,   8,   8,   8,   8,   8,  19,  19,  19, 161,  19,  19,  19,  19,
     19,  19,  19,  19,  19,  19, 162,   8,  19,  19, 150,   8,   8,   8,  19, 162,
    163,  19,  19,  19,   7, 120, 142,  20,   8,   8,   8, 164,   7,   8,   8,   8,
      8,   8,   8,   8,   8,   8,   8, 165,  19,  19,  19,  19, 162,   8,   8,   8,
      8,   8,   8, 128,   8,   8,   8,   8,   8,   8,  19,  19,   8,   8,   8, 166,
      8,   8,   8,   8,   8,   8, 160,  19,   8, 158,   8,   8,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8,  16, 152,   8,   8,   8, 114,   8,   8,   8,  16,
      8,   8,  19,  19,  19,  19,  19,  19,  84,   8,   8,   8,   8,   8,   8,   8,
      8,  20,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  90, 167, 168,
      8,   8,   8,  80,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  27,   8,
     19,  19,  19,  19, 103,   8,   8,   8,   8,   7,   8,   8,   8,   8,   8,   8,
    169, 170,   8,   8,  83, 162,   8, 171,   8,   8,   8,   8,   8,   8, 158,   8,
     85,   8,   8,   8,  22,  22,  27,   8,   8,   8,   8,   8,  80,  93,   8,   8,
     90,  22,  27,   8,   8,   8,   8,   8,  57,   8,   8,   8,   8,   8, 172, 173,
      8,   8,   8,   8,  65,   8,   8,   8,   8,   8,   8,   8,   8, 174, 100,   8,
    170,  63,   8,   8,   8,   8,   8,  63,   8,   8,   8,   8,   8,   8, 175,  81,
     62,   8,   8,   8,   8,  85,  76,   8,   8,   8,   8,   8,  65,  60,   8,   8,
      8,   8,   8, 176,  32, 177,  32,  32,  32,  32,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  36,  36, 178,  36,  36,   0,   0,   0,   0,  36, 179,
     22,  22,  19, 103,  22,  22,  19,  19,  19,  19, 180, 181, 182, 183,  36,  36,
     36,  36,  36,  36,  36,  36,  36, 184, 185,   4,   5,   6,   7,   8,   8,   9,
      7,   8,   8,   9, 150,   8,   8,   8,   8,   8,   8,   8, 186,  84,   0, 187,
    188,   8,   8,   8,   8,   8,   8,   8,  19, 146,  19, 162,   7,   8,   8,   8,
      8,   8,   8,   8,   8,   8,   8,  65,   8,   8,   8,   8, 189,   5,   5, 153,
      8,   8,   8,   8,   8,   8,  80,  57,  32,  32,  32,  32,  32,  32,  32,  32,
     32,  32,  32, 190,  32,  32,  32,  32, 191, 192,  32,  32,  32,  32,  32, 193,
     32,  32,  32,  32, 194,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 195,
     32,  32,  32,  32,  38,  38,  38, 196,  62,   8,   8,   8,   8,   8,   8,  22,
     91,   8,  18,  19, 150,   8,   8,  90,  27,   8,   8,   8,   8,   8, 197,  68,
     57,   8,   8,   8,  90, 198,  98,   8,   8,   8,   8,   8,   8,   8, 170,   8,
     27,   8,   8,   8,   8,   8,  80,  91,   8, 106,   8,   8,   8,   8,   8,   8,
      8,   8,   8,   8,   8,  90, 199,  76,   8,   8,   8,  90,  26,  57,   8,   8,
     27,   8,   8,   8,   8,   8,   8,  63,  78,   8,   8,   8,  80,  98,  98,   8,
      8,   8,   8,   8,   8,   8,   8,  22,  87,   8,   8,   8,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8,  26, 200, 201,   8,   8,   8,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8, 120, 107,  78,   8,   8,  85,   8,   8,   8,   8,
      8,   8,   8,   8,   8,   8,  26, 202,  78,   8,   8,   8,  19, 146,   8,   8,
      8,   8,   8,   8,   8, 203, 204,   8,   8,   8,   8,  97, 205, 108,   8,   8,
    174,  68,   8,   8,   8,   8,  26, 206,  90,   8, 174, 112,   8,   8,   8,   8,
      8,  74,  91,  27,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  91,  93,
      8,   8,  74,  22,  22,  74, 207,   8,   8,   8,   8,   8,   8,   8, 174, 208,
    204,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  98,   8,
      8,   8,   8,   8,   8,   8,  91,   8,   8,  90,  57,   8,   8,   8,   8,   8,
      8,   8,   8,  83, 209,   8,   8,   8,   8,   8,   8,   8,  90,  27, 210, 211,
    212, 108,   8,   8,   8, 120,   8,   8, 213,   8,   8,   8,   8,   8,   8,   8,
     19,  19,  84,   8,   8,   8,   8,   8,   8,   8,   8, 165,   8,   8,   8,   8,
      8,   8, 214,   8,   8,   8,   8,   8,   8,  16,   8,   8,   8,   8,   8,   8,
      8, 188,   8,   8,   8,   8,   8,   8, 165, 215,   5,   5,   5,   5,   5,   5,
     22,  22,  22,  22,  22,  22,  91,  26,  22,  22,  22,  22,  22,  98,  65,   8,
     63,   8,   8,  26,  59,  22,   8,   8,  91,  22,  22, 216, 217,  57,   8,   8,
     32,  32, 218,  32,  32,  32,  32,  32, 192, 219,  32,  32,  32,  32,  32,  32,
     36,  36,  36,  36,  36,  36, 220,  36,  19,  19,  19,  19,  19, 162,  19,  19,
     19,  19, 162,   8,  19,  84, 160,  19, 160,  19, 160,  19,  19,  19, 150,   8,
      5, 221,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8, 145,   8,   8,
      8,   8,   8,   8, 150,   8,   8,   8,  19,  19, 146,   8,  19, 146,  19,   7,
     19,  19, 146,   8,   8,   8,   8,   8,  19, 162,  19,  19,  19,  19,  19,  19,
     19,   8,  19, 103,  19,  19,  19,  19,  19,   8,  19,  19,  19, 150,   8,   8,
     19, 162,  19,  19,  19,  19,  19,  84,  19, 146,  19,  19,  19, 162,   8,   8,
     19,  19,  19,   8,   8,   8,   8,   8,   7,   8,  19,  19,  84,   8,   8,   8,
      8,   8,   8,   8,   8,   8,   8, 222,   0,   0,   0,   0,   0,   0,   0,   0,
     22,  22,  22,  22,  22,  22,   0,   0,
};

static RE_UINT8 re_bidi_class_stage_4[] = {
      0,   0,   1,   2,   0,   3,   4,   5,   6,   7,   8,   8,   9,  10,  11,  12,
     12,  12,  13,  10,  13,  14,   0,  15,  16,   5,  17,  18,  19,  20,  21,  10,
     12,  13,  22,  12,  23,  10,  10,  10,  12,  24,  10,  17,  25,  25,  12,  26,
     12,  27,  12,  17,  28,  25,  29,  12,  27,  30,  31,  25,  25,  32,  33,  32,
     34,  34,  35,  36,  37,  38,  39,  40,  40,  40,  41,  25,  35,  35,  42,  43,
     44,  40,  40,  45,  25,  46,  25,  47,  48,  49,  50,  40,  51,  40,  52,  25,
     25,  53,  54,  34,  34,  55,  32,  25,  25,  31,  31,  56,  31,  34,  40,  25,
     57,  25,  58,  12,  59,  60,  61,  25,  60,  62,  63,  12,  62,  12,  12,  60,
     61,  60,  12,  62,  64,  12,  65,  12,  66,  12,  66,  28,  67,  29,  29,  62,
     61,  68,  69,  12,  63,  25,  12,  67,  12,  59,  59,  12,  60,  12,  70,  12,
     12,  63,  60,  63,  71,  29,  12,  66,  10,  72,  12,  29,  28,  60,  63,  73,
     62,  25,  58,  65,  12,  28,  25,  58,  68,  60,  25,  29,  12,  74,  75,  26,
     25,  71,  12,  61,  25,  60,  71,  25,  66,  66,  29,  63,  59,  66,  26,  12,
     76,  12,  13,  11,  63,  60,  12,  68,  25,  12,  65,  62,  77,  78,  58,  28,
     61,  12,  11,  26,  12,  23,  67,  12,  73,  61,  25,  67,  12,  71,  58,  60,
     63,  29,  68,  29,  29,  74,  12,  25,  25,  63,  58,  25,  68,  25,  12,  79,
     12,  22,  80,  80,  81,  82,  83,  84,  85,  86,  10,  87,  10,  88,   0,  89,
     90,   0,  91,   8,  92,  72,  85,  85,  17,  72,  12,  20,  11,  23,  10,  79,
     93,  94,  23,  12,  10,  11,  23,  26,  24,  12,  95,  10,  10,  26,  10,  20,
     72,  12,   8,  12,  10,  24,  10,  23,  26,  24,  20,  10,  12,  10,  72,  28,
     24,  10,  17,  10,  10,  12,  96,  11,  97,  11,  13,  12,  12,  72,  98,  25,
     25,  99,  59,  59,  28,  12, 100,  12,  28,  63,  29,  60,  61,  58,  71,  67,
     12, 101, 102,  34,  40, 103,  40, 104, 105, 106,  10, 107, 108,  72, 109,  12,
     40, 110,  30,   5,   5, 111, 112, 113,  93,  12, 114,   8,  34, 115,  31,  33,
     34,  25, 116,  52,  34,  33, 117,  10,  35, 118,  28,  58,  25,  61,  29,  71,
     73,  28,  71,  12,  58,  74,  28,  62,  25,  68,  63,  68,  67,  58,  71,  66,
     59,  68,   0,  12, 119,   0, 120,  25,  58,  61, 121, 122,  12,  93,  12, 123,
     67,  25,  68,  71,  25, 116, 116,  34, 124,  40, 125,  11,  12, 126,
};

static RE_UINT8 re_bidi_class_stage_5[] = {
    11, 11, 11, 11, 11,  8,  7,  8,  9,  7, 11, 11,  7,  7,  7,  8,
     9, 10, 10,  4,  4,  4, 10, 10, 10, 10, 10,  3,  6,  3,  6,  6,
     2,  2,  2,  2,  2,  2,  6, 10, 10, 10, 10, 10, 10,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 10, 10, 10, 10, 11, 11,  7, 11, 11,
     6, 10,  4,  4, 10, 10,  0, 10, 10, 11, 10, 10,  4,  4,  2,  2,
    10,  0, 10, 10, 10,  2,  0, 10,  0, 10, 10,  0,  0,  0, 10, 10,
     0, 10, 10, 10, 12, 12, 12, 12, 10, 10,  0,  0,  0,  0, 10,  0,
     0,  0,  0, 12, 12, 12,  0,  0,  0, 10, 10,  4,  1, 12, 12, 12,
    12, 12,  1, 12,  1, 12, 12,  1,  1,  1,  1,  1,  5,  5,  5,  5,
     5,  5, 10, 10, 13,  4,  4, 13,  6, 13, 10, 10, 12, 12, 12, 13,
    13, 13, 13, 13, 13, 13, 13, 12,  5,  5,  4,  5,  5, 13, 13, 13,
    12, 13, 13, 13, 13, 13, 12, 12, 12,  5, 10, 12, 12, 13, 13, 12,
    12, 10, 12, 12, 12, 12, 13, 13,  2,  2, 13, 13, 13, 12, 13, 13,
     1,  1,  1, 12,  1,  1, 10, 10, 10, 10,  1,  1,  1,  1, 12, 12,
    12, 12,  1,  1, 12, 12,  5, 12, 12, 12, 12,  0,  0,  0, 12,  0,
    12,  0,  0,  0,  0, 12, 12, 12,  0, 12,  0,  0,  0,  0, 12, 12,
     0,  0,  4,  4,  0,  0,  0,  4,  0, 12, 12,  0, 12,  0,  0, 12,
    12, 12,  0, 12,  0,  4,  0,  0, 10,  4, 10,  0, 12,  0, 12, 12,
    10, 10, 10,  0, 12,  0, 12,  0,  0, 12,  0, 12,  0, 12, 10, 10,
     9,  0,  0,  0, 10, 10, 10, 12, 12, 12, 11,  0,  0, 10,  0, 10,
     9,  9,  9,  9,  9,  9,  9, 11, 11, 11,  0,  1,  9,  7, 16, 17,
    18, 14, 15,  6,  4,  4,  4,  4,  4, 10, 10, 10,  6, 10, 10, 10,
    10, 10, 10,  9, 11, 11, 19, 20, 21, 22, 11, 11,  2,  0,  0,  0,
     2,  2,  3,  3,  0, 10,  0,  0,  0,  0,  4,  0, 10, 10,  3,  4,
     9, 10, 10, 10,  0, 12, 12, 10, 12, 12, 12, 10, 12, 12, 10, 10,
     4,  4,  0,  0,  0,  1, 12,  1,  1,  3,  1,  1, 13, 13, 10, 10,
    13, 10, 13, 13,  6, 10,  6,  0, 10,  6, 10, 10, 10, 10, 10,  4,
    10, 10,  3,  3, 10,  4,  4, 10, 13, 13, 13, 11, 10,  4,  4,  0,
    11, 10, 10, 10, 10, 10, 11, 11, 12,  2,  2,  2,  1,  1,  1, 10,
    12, 12, 12,  1,  1, 10, 10, 10,  5,  5,  5,  1,  0,  0,  0, 11,
    11, 11, 11, 12, 10, 10, 12, 12, 12, 10,  0,  0,  0,  0,  2,  2,
    10, 10, 13, 13,  2,  2,  2, 10,  0,  0, 11, 11,
};

/* Bidi_Class: 4098 bytes. */

RE_UINT32 re_get_bidi_class(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_bidi_class_stage_1[f] << 6;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_bidi_class_stage_2[pos + f] << 3;
    f = code >> 3;
    code ^= f << 3;
    pos = (RE_UINT32)re_bidi_class_stage_3[pos + f] << 1;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_bidi_class_stage_4[pos + f] << 2;
    value = re_bidi_class_stage_5[pos + code];

    return value;
}

/* Canonical_Combining_Class. */

static RE_UINT8 re_canonical_combining_class_stage_1[] = {
    0, 1, 2, 2, 2, 3, 2, 4, 5, 2, 2, 6, 2, 7, 8, 9,
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
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0, 10, 11, 12, 13,  0,
    14,  0,  0,  0,  0,  0, 15,  0, 16,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0, 17, 18, 19,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 20,  0, 21,
    22, 23,  0,  0,  0, 24,  0,  0, 25, 26, 27, 28,  0, 29, 30,  0,
     0,  0,  0,  0,  0, 31,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 32,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 33, 34,  0,  0,  0,  0,  0,  0,
    35,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_canonical_combining_class_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   3,   4,   0,   0,   0,   0,
      0,   0,   0,   0,   5,   0,   0,   0,   0,   0,   0,   0,   6,   7,   8,   0,
      9,   0,  10,  11,   0,   0,  12,  13,  14,  15,  16,   0,   0,   0,   0,  17,
     18,  19,  20,   0,   0,   0,  21,  22,   0,  23,  24,   0,   0,  23,  25,   0,
      0,  23,  25,   0,   0,  23,  25,   0,   0,  23,  25,   0,   0,   0,  25,   0,
      0,   0,  26,   0,   0,  23,  25,   0,   0,  27,  25,   0,   0,   0,  28,   0,
      0,  29,  30,   0,   0,  31,  32,   0,  33,  34,   0,  35,  36,   0,  37,   0,
      0,  38,   0,   0,  39,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  40,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  41,  41,   0,   0,   0,   0,  42,   0,
      0,   0,   0,   0,   0,  43,   0,   0,   0,  44,   0,   0,   0,   0,   0,   0,
     45,   0,   0,  46,   0,  47,   0,   0,   0,  48,  49,  50,   0,  51,   0,  52,
      0,  53,   0,   0,   0,   0,  54,  55,   0,   0,   0,   0,   0,   0,  56,  57,
      0,   0,   0,   0,   0,   0,  58,  59,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  60,   0,   0,   0,  61,   0,   0,   0,  62,
      0,  63,   0,   0,  64,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,  65,  66,   0,   0,  67,   0,   0,   0,   0,   0,   0,   0,   0,
     68,   0,   0,   0,   0,   0,  49,  69,   0,  70,  71,   0,   0,  72,  73,   0,
      0,   0,   0,   0,   0,  74,  75,  76,   0,   0,   0,   0,   0,   0,   0,  25,
      0,   0,   0,   0,   0,   0,   0,   0,  77,   0,   0,   0,   0,   0,   0,   0,
      0,  78,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  79,
      0,   0,   0,   0,   0,   0,   0,  80,   0,   0,   0,  81,   0,   0,   0,   0,
     82,  83,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,  68,  61,   0,  85,   0,   0,  86,  87,   0,  72,   0,   0,  88,   0,
      0,  89,   0,   0,   0,   0,   0,  90,   0,  23,  25,  91,   0,   0,   0,   0,
      0,   0,  92,   0,   0,   0,  93,   0,   0,   0,   0,   0,   0,  61,  94,   0,
      0,  61,   0,   0,   0,  95,   0,   0,   0,  96,   0,   0,   0,   0,   0,   0,
      0,  41,  97,   0,  98,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,  61,   0,   0,   0,   0,   0,   0,   0,   0,  99,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 100,   0, 101,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 102,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 103, 104, 105,   0,   0,
      0,   0, 106,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    107, 108,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 109,   0,   0,   0, 110,   0,   0,   0,   0,   0,
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
      0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   1,   1,   1,
     51,  21,  43,  52,  53,  21,  35,   1,   0,   0,   0,   0,   0,   0,   0,  54,
      0,   0,   0,  55,  56,  57,   0,   0,   0,   0,   0,  55,   0,   0,   0,   0,
      0,   0,   0,  55,   0,  58,   0,   0,   0,   0,   0,   0,   0,   0,  59,  60,
      0,   0,  61,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  62,   0,
      0,   0,  63,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  64,   0,
      0,   0,  65,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  66,   0,
      0,   0,   0,   0,   0,  67,  68,   0,   0,   0,   0,   0,  69,  70,  71,  72,
     73,  74,   0,   0,   0,   0,   0,   0,   0,  75,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  76,  77,   0,   0,   0,   0,  78,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  48,   0,   0,   0,   0,   0,  60,   0,   0,
      0,   0,   0,   0,  61,   0,   0,  79,   0,   0,  80,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,  19,  82,   0,
     60,   0,   0,   0,   0,  48,   1,  83,   0,   0,   0,   0,   1,  52,  15,  41,
      0,   0,   0,   0,   0,  54,   0,   0,   0,  60,   0,   0,   0,   0,   0,   0,
      0,   0,  19,  10,   1,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,
      0,  85,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,  76,   0,   0,
      0,   0,   0,   0,  86,   9,  12,   4,  87,   8,  88,  78,   0,  57,  49,   0,
     21,   1,  21,  89,  90,   1,   1,   1,   1,   1,   1,   1,   1,  91,  92,  93,
      0,   0,   0,   0,  94,   1,  95,  57,  79,  96,  97,   4,  57,   0,   0,   0,
      0,   0,   0,  19,  49,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  59,
      1,   1,   1,   1,   1,   1,   1,   1,   0,   0,  98,  99,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 100,   0,   0,   0,   0,  19,   0,   1,   1,  49,
      0,   0,   0,   0,   0,   0,   0,  38,   0,   0,   0,   0,  49,   0,   0,   0,
      0,  61,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,  49,   0,   0,   0,
      0,   0, 101,  66,   0,   0,   0,   0,   0,   0,   0,   0,  59,   0,   0,   0,
      0,   0,   0,   0,  76,   0,   0,   0,  60,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 102, 103,  57,  38,  79,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  61,   0,   0,   0,   0,   0,   0,   0,   0,   0, 104,
      1,  14,   4,  12,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  78,
     82,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  38,  86,   0,
      0,   0,   0, 105,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 106,  59,
      0, 107,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 108,   0,
     86,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  59,  60,   0,   0,
     60,   0,  85,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 108,   0,   0,
      0,   0, 109,   0,   0,   0,   0,   0,   0,  38,   1,  57,   1,  57,   0,   0,
     61,  85,   0,   0,   0,   0,   0,   0, 110,   0,   0,   0,   0,   0,   0,   0,
     54,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 110,   0,   0,
      0,   0,  59,   0,   0,   0,   0,   0,   0,  59,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  55,   0,  85, 111,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   8,  88,   0,   0,   0,   0,   0,   0,   1,  86,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 112,   0, 113, 114, 115, 116,   0, 101,   4,
    117,  48,  23,   0,   0,   0,   0,   0,   0,   0,  38,  49,   0,   0,   0,   0,
     38,  57,   0,   0,   0,   0,   0,   0,   1,  86,   1,   1,   1,   1,  39,   1,
     47, 102,  86,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4, 117,   0,   0,
      0,   1, 118,   0,   0,   0,   0,   0,
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
     0, 50, 50, 50, 50, 50,  0,  0,  0, 45, 45, 45, 50, 50,  0, 45,
    50, 45, 45, 45, 22, 23, 24, 50,  2,  0,  0,  0,  0,  4,  0,  0,
     0, 50, 45, 50, 50,  0,  0,  0,  0, 32, 33,  0,  0,  0,  0,  4,
     4,  0,  0,  0,  0,  0,  4,  0, 34, 34,  4,  0, 35, 35, 35, 35,
    36, 36,  0,  0, 37, 37, 37, 37, 45, 45,  0,  0,  0, 45,  0, 45,
     0, 43,  0,  0,  0, 38, 39,  0, 40,  0,  0,  0,  0,  0, 39, 39,
    39, 39,  0,  0, 39,  0, 50, 50,  4,  0, 50, 50,  0,  0, 45,  0,
     0,  0,  0,  2,  0,  4,  4,  0,  0, 45,  0,  0,  0, 50,  0,  0,
     0, 49,  0,  0,  0, 46, 50, 45, 45,  0,  0,  0, 50,  0,  0, 45,
     0,  0,  4,  4,  0,  0,  2,  0, 50, 50, 50,  0, 50,  0,  1,  1,
     1,  0,  0,  0, 50, 53, 42, 45, 41, 50, 50, 50, 50, 50, 51, 49,
    49, 45,  0, 50, 52, 45, 50, 45, 50, 50,  1,  1,  1,  1,  1, 50,
     0,  1,  1, 50, 45, 50,  1,  1,  0,  0, 44, 49, 51, 46, 47, 47,
     0,  3,  3,  0,  0,  0,  0, 45, 50,  0, 50, 50, 45,  0,  0, 50,
     0,  0, 21,  0,  0, 45,  0, 50, 50,  1, 45,  0,  0, 50, 45,  0,
     0,  4,  2,  0,  0,  2,  4,  0,  0,  0,  4,  2,  4,  4,  0,  0,
     0,  0,  1,  0,  0, 43, 43,  1,  1,  1,  0,  0,  0, 48, 43, 43,
    43, 43, 43,  0, 45, 45, 45,  0, 50, 50,  2,  0,
};

/* Canonical_Combining_Class: 2252 bytes. */

RE_UINT32 re_get_canonical_combining_class(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_canonical_combining_class_stage_1[f] << 4;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_canonical_combining_class_stage_2[pos + f] << 4;
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
     7,  7,  7,  7,  7,  7, 34, 35,  7,  7,  7, 36, 37, 37, 37, 37,
    37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
    37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
    37, 37, 37, 37, 37, 37, 37, 38,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7, 37, 39, 40, 41, 42, 43, 44,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
    45, 46,  7, 47, 48, 49,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7, 50,  7,  7, 51, 52, 53, 54,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 55,  7,
     7, 56, 57,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7, 37, 37, 58,  7,  7,  7,  7,  7,
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
      0,   0,   0,   0,   0,   0,   0,   0,   0, 146,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 147,   0,   0,   0,   0,   0,   0,   0, 148,
      0,   0,   0,   0,   0, 149,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   6,   6,   6,   6,  21,   0,   0,   0,   0,   0,
     81, 150, 151,   6,   6,   6,  81,   6,   6,   6,   6,   6,   6,  78,   0,   0,
    152, 153, 154, 155, 156, 157, 158, 158, 159, 158, 160, 161,   0, 162, 163, 164,
    165, 165, 165, 165, 165, 165, 166, 167, 167, 168, 169, 169, 169, 170, 171, 172,
    165, 173, 174, 175,   0, 176, 177, 178, 179, 180, 167, 181, 182,   0,   0, 183,
      0, 184,   0, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 194, 195, 196,
    197, 198, 198, 198, 198, 198, 199, 200, 200, 200, 200, 201, 202, 203, 204,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0, 205, 206,   0,   0,   0,   0,   0,
      0,   0, 207,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  46,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 208,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 104,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 207, 209,   0,   0,   0,   0, 210,  14,   0,   0,   0,
    211, 211, 211, 211, 211, 212, 211, 211, 211, 213, 214, 215, 216, 211, 211, 211,
    217, 218, 211, 219, 220, 221, 211, 211, 211, 211, 211, 211, 211, 211, 211, 211,
    211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 222, 211, 211, 211, 211, 211,
    211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 223, 211, 211, 211,
    216, 211, 224, 225, 226, 227, 228, 229, 230, 231, 232, 231,   0,   0,   0,   0,
    233, 102, 234, 142, 142,   0, 235,   0,   0, 236,   0,   0,   0,   0,   0,   0,
    237, 142, 142, 238, 239, 240,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
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
     82,  81,  81,  81,  20,  20,  20,  83,   0,   0,   0,  55,  25,   0,   0,   0,
      0,   0,  55,   0,   0,   0,   0,  24,  36,  10,   8,  11,  36,  33,  13,   8,
     20,  30,   0,   0,   3,  20,   0,  46,  59,  59,  84,   8,   8,  11,   8,  36,
      9,  22,   8,  15,  85,  86,  86,  86,  86,  86,  86,  86,  86,  85,  85,  85,
     87,  85,  86,  86,  88,   0,   0,   0,  89,  90,  91,  92,  85,  87,  86,  85,
     85,  85,  93,  87,  94,  94,  94,  94,  94,  95,  95,  95,  95,  95,  95,  95,
     95,  96,  97,  97,  97,  97,  97,  97,  97,  97,  97,  98,  99,  99,  99,  99,
     99, 100,  94,  94, 101,  95,  95,  95,  95,  95,  95, 102,  97,  99,  99, 103,
    104,  97, 105, 106, 107, 105, 108, 105, 104,  96,  95, 105,  96, 109, 110,  97,
    111, 106, 112, 105,  95, 106, 113,  95,  96, 106,   0,   0,  94,  94,  94, 114,
    115, 115, 116,   0, 115, 115, 115, 115, 115, 117, 118,  20, 119, 120, 120, 120,
    120, 119, 120,   0, 121, 122, 123, 123, 124,  91, 125, 126,  90, 125, 127, 127,
    127, 127, 126,  91, 125, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 126,
    125, 126,  91, 128, 129, 130, 130, 130, 130, 130, 130, 130, 131, 132, 132, 132,
    132, 132, 132, 132, 132, 132, 132, 133, 134, 132, 134, 132, 134, 132, 134, 135,
    130, 136, 132, 133,   0,   0,  27,  19,   0,   0,  18,   0,   0,   0,   0,  13,
      0,   0,  18,  36,   8,  19,   0,   0,   0,   0,  18,   8,  59,  59,  59,  59,
     59, 137,  59,  59,  59,  59,  59, 137, 138, 139,  61, 137,  59,  59,  66,  61,
     59,  61,  59,  59,  59,  66, 140,  61,  59, 137,  59, 137,  59,  59,  66, 140,
     59, 141, 142,  59, 137,  59,  59,  59,  59,  62,  59,  59,  59,  59,  59, 142,
    139, 143,  61,  59, 140,  59, 144,   0, 138, 145, 144,  61, 139, 143, 144, 144,
    139, 143, 140,  59, 140,  59,  61, 141,  59,  59,  66,  59,  59,  59,  59,   0,
     61,  61,  66,  59,  20,  20,  30,   0,  20,  20, 146,  75,   0,   0,   4,   0,
    147,   0,   0,   0, 148,   0,   0,   0,  81,  81,  81,   0,  20,  20,  35,   0,
    149,   0,   0,   0,
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

/* Decomposition_Type: 2964 bytes. */

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
    10, 10, 10, 10, 10, 10, 11,  5, 12, 10, 10, 13, 10, 10, 10, 14,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 15,
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
    16, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
     8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 17,
     8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 17,
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
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 30,
    22, 22, 22, 22, 22, 22, 22, 31, 22, 22, 32,  5,  5,  5,  5,  5,
    22, 33, 34,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
    35, 36, 37, 38, 39, 40, 41,  5,  5, 42,  5,  5,  5,  5,  5,  5,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 43,
     5, 44,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 45,
};

static RE_UINT8 re_east_asian_width_stage_3[] = {
      0,   0,   1,   1,   1,   1,   1,   2,   0,   0,   3,   4,   5,   6,   7,   8,
      9,  10,  11,  12,  13,  14,  11,   0,   0,   0,   0,   0,  15,  16,   0,   0,
      0,   0,   0,   0,   0,   9,   9,   0,   0,   0,   0,   0,  17,  18,   0,   0,
     19,  19,  19,  19,  19,  19,  19,   0,   0,  20,  21,  20,  21,   0,   0,   0,
      9,  19,  19,  19,  19,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     22,  22,  22,  22,  22,  22,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,  23,  24,  25,   0,   0,   0,  26,  27,   0,  28,   0,   0,   0,   0,   0,
     29,  30,  31,   0,   0,  32,  33,  34,  35,  34,   0,  36,   0,  37,  38,   0,
     39,  40,  41,  42,  43,  44,  45,   0,  46,  47,  48,  49,   0,   0,   0,   0,
      0,  50,  51,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  52,  53,
      0,   0,   0,   0,   0,   0,  19,  19,  19,  19,  19,  19,  19,  19,  54,  19,
     19,  19,  19,  19,  33,  19,  19,  55,  19,  56,  21,  57,  58,  59,  60,  61,
     62,  63,   0,   0,  64,  65,  66,  67,   0,  68,  69,  70,  71,  72,  73,  74,
     75,   0,  76,  77,  78,  79,   0,  80,   0,  81,   0,  82,   0,   0,  83,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,
      0,  85,   0,   0,   0,  86,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  22,  87,  22,  22,  22,  22,  22,  65,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  88,   0,  89,
     90,  22,  22,  91,  92,  22,  22,  22,  22,  93,  22,  22,  22,  22,  22,  22,
     94,  22,  91,  92,  22,  22,  22,  22,  91,  22,  22,  95,  22,  22,  65,  22,
     22,  91,  22,  22,  96,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  91,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,   0,   0,   0,   0,
     22,  22,  22,  22,  22,  22,  22,  22,  97,  22,  22,  22,  98,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  22,  97,   0,   0,   0,   0,   0,   0,   0,   0,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  65,   0,   0,   0,   0,   0,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,
     19,  99,   0,  22,  22, 100, 101,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    102, 103, 103, 103, 103, 103, 104, 105, 105, 105, 105, 106, 107, 108, 109,  77,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 110,   0,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  97,   0,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22, 111,
     22,  91,   0,   0,   0,   0,   0,  22,  22,  22,  22,  22,  22,  22,  22,  22,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  89,
    112,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,
    113,  19, 114,  19,  19,  19,  34,  19, 115, 116, 117,   0,   0,   0,   0,   0,
    111,  22,  22,  89, 118, 110,  88,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     22,  22, 119, 120,  22,  22,  22, 121,  22,  65,  22,  22, 122,  65,  22, 123,
     22,  22,  22,  91, 124,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22, 125,
     22,  22,  22, 126, 127,  22, 128, 129,   0, 130, 112,   0,   0,   0,   0, 131,
     22,  22,  22,  22,  22,   0,   0,   0,  22,  22,  22,  22, 132, 111,  85, 133,
      0,  22,  22,  91,  97,  22,  89,   0,  22, 128,   0,   0, 134,  22,  98,   0,
     22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,  22, 126,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,   0,
     19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19, 114,
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
     0, 15,  0,  0,  0,  0,  0, 12, 10,  0, 23,  0,  0,  0, 24,  0,
     0,  0, 25, 26, 27,  0,  0,  0,  7,  7, 19,  7,  7,  0,  0,  0,
    13, 14,  0,  0, 13, 13,  0, 14, 14, 13, 18, 13, 14,  0,  0,  0,
    13, 14,  0, 12,  0,  0,  0, 24,  0, 22, 15, 13,  0, 28,  0,  5,
     5,  0, 20, 20, 20,  0,  0,  0, 19, 19,  9, 19,  0,  0,  0, 29,
    29,  0,  0, 13, 30,  0, 23,  0,  0,  0,  0, 31,  0, 32,  7, 33,
     7, 34,  7,  7, 19,  0, 33,  7, 35, 36, 33, 36,  0, 30, 23,  0,
     0,  0, 26,  0,  0,  0,  0, 15,  0,  0,  0, 37, 29, 38,  0,  0,
     0, 13,  7,  7,  0, 25,  0,  0, 26,  0,  0, 29,  0, 39,  1, 40,
     0, 41,  0,  0,  0,  0, 29, 26, 26, 42, 14,  0, 20, 20, 38, 20,
    20, 28,  0,  0, 20, 20, 20,  0, 43, 20, 20, 20, 20, 20, 20, 44,
    25, 20, 20, 20, 20, 44, 25, 20,  0, 25, 20, 20, 20, 20, 44,  0,
    20, 20,  7,  7, 20, 20, 20, 26, 20, 44,  0,  0, 20, 20, 28,  0,
    44, 20, 20, 20, 20, 44, 20,  0, 45, 46, 46, 46, 46, 46, 46, 46,
    47, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 49, 50, 48, 50, 48,
    50, 48, 50, 51, 46, 52, 48, 49, 28,  0,  0,  0, 44,  0,  0,  0,
     0, 26,  0,  0,  7,  7,  9,  0,  7,  7,  7, 14,  7,  7,  7, 33,
    53, 20, 54,  7,  7,  7,  7, 11, 20, 20, 26,  0, 26,  0,  0, 25,
    20, 38, 20, 20, 20, 20, 20, 55, 20, 20, 44, 29, 26, 26, 20, 20,
    55, 20, 20, 20, 20, 20, 20, 27, 20, 20, 20, 28,  0,  0, 29, 44,
    20, 20,  0,  0,  0,  0, 56,  0,  0, 24,  0,  0,  0,  0, 29, 20,
    20, 28,  0, 26,  0, 20, 26,  0, 26,  0,  0,  0,
};

static RE_UINT8 re_east_asian_width_stage_5[] = {
    0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 1, 5, 5,
    1, 5, 5, 1, 1, 0, 1, 0, 5, 1, 1, 5, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0,
    0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
    3, 3, 3, 3, 0, 2, 0, 0, 0, 1, 1, 0, 0, 0, 3, 3,
    0, 3, 3, 0, 0, 3, 3, 3, 3, 0, 0, 0, 3, 0, 0, 3,
    3, 3, 0, 0, 0, 0, 0, 3, 0, 3, 0, 0, 0, 3, 3, 1,
    3, 3, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 3, 3,
    1, 3, 1, 1, 3, 0, 3, 0, 3, 3, 0, 3, 0, 0, 5, 5,
    5, 5, 0, 0, 0, 5, 5, 0, 0, 3, 1, 1, 4, 3, 3, 3,
    3, 3, 3, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 0,
    4, 4, 4, 0, 1, 3, 3, 3, 3, 3, 3, 1, 3, 0, 3, 3,
    0, 0, 3, 0,
};

/* East_Asian_Width: 2064 bytes. */

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
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1,
};

static RE_UINT8 re_joining_group_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_joining_group_stage_3[] = {
    0, 0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0,
};

static RE_UINT8 re_joining_group_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  1,  2,  3,  0,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13,
     0, 14, 15,  0, 16, 17, 18, 19,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 20,  0,  0,  0, 21, 22,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23, 24, 25,  0,
};

static RE_UINT8 re_joining_group_stage_5[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    45,  0,  3,  3, 43,  3, 45,  3,  4, 41,  4,  4, 13, 13, 13,  6,
     6, 31, 31, 35, 35, 33, 33, 39, 39,  1,  1, 11, 11, 55, 55, 55,
     0,  9, 29, 19, 22, 24, 26, 16, 43, 45, 45,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4, 29,
     0,  3,  3,  3,  0,  3, 43, 43, 45,  4,  4,  4,  4,  4,  4,  4,
     4, 13, 13, 13, 13, 13, 13, 13,  6,  6,  6,  6,  6,  6,  6,  6,
     6, 31, 31, 31, 31, 31, 31, 31, 31, 31, 35, 35, 35, 33, 33, 39,
     1,  9,  9,  9,  9,  9,  9, 29, 29, 11, 38, 11, 19, 19, 19, 11,
    11, 11, 11, 11, 11, 22, 22, 22, 22, 26, 26, 26, 26, 56, 21, 13,
    41, 17, 17, 14, 43, 43, 43, 43, 43, 43, 43, 43, 55, 47, 55, 43,
    45, 45, 46, 46,  0, 41,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6, 31,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35, 33,  1,  0,  0, 21,
     2,  0,  5, 12, 12,  7,  7, 15, 44, 50, 18, 42, 42, 48, 49, 20,
    23, 25, 27, 36, 10,  8, 28, 32, 34, 30,  7, 37, 40,  5, 12,  7,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 51, 52, 53,
     4,  4,  4,  4,  4,  4,  4, 13, 13,  6,  6, 31, 35,  1,  1,  1,
     9,  9, 11, 11, 11, 24, 24, 26, 26, 26, 22, 31, 31, 35, 13, 13,
    35, 31, 13,  3,  3, 55, 55, 45, 43, 43, 54, 54, 13, 35, 35, 19,
    89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,  0,  0,  0,  0,  0,
     4,  4, 13, 39,  9, 29, 22, 24, 45, 45, 31, 43, 57,  0,  6, 33,
    11, 58, 31,  1, 19,  0,  4,  4,  4, 31, 45, 86, 87, 88,  0,  0,
    59, 61, 61, 65, 65, 62,  0, 83,  0, 85, 85,  0,  0, 66, 80, 84,
    68, 68, 68, 69, 63, 81, 70, 71, 77, 60, 60, 73, 73, 76, 74, 74,
    74, 75,  0,  0, 78,  0,  0,  0,  0,  0,  0, 72, 64, 79, 82, 67,
};

/* Joining_Group: 602 bytes. */

RE_UINT32 re_get_joining_group(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_joining_group_stage_1[f] << 3;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_joining_group_stage_2[pos + f] << 4;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_joining_group_stage_3[pos + f] << 4;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_joining_group_stage_4[pos + f] << 4;
    value = re_joining_group_stage_5[pos + code];

    return value;
}

/* Joining_Type. */

static RE_UINT8 re_joining_type_stage_1[] = {
     0,  1,  2,  3,  4,  4,  4,  4,  4,  4,  5,  4,  4,  4,  4,  6,
     7,  8,  4,  4,  4,  4,  9,  4,  4,  4,  4, 10,  4, 11, 12,  4,
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
    13,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
};

static RE_UINT8 re_joining_type_stage_2[] = {
     0,  1,  0,  0,  0,  0,  2,  0,  0,  3,  0,  4,  5,  6,  7,  8,
     9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26,  0,  0,  0,  0, 27,  0,  0,  0,  0,  0,  0,  0, 28, 29,
    30, 31, 32,  0, 33, 34, 35, 36, 37, 38,  0, 39,  0,  0,  0,  0,
    40, 41,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0, 42, 43, 44,  0,  0,  0,  0,
    45, 46,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 47, 48,  0,  0,
    49, 50, 51, 52, 53, 54,  0, 55,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 56,  0,  0,  0,  0,  0, 57, 43,  0, 58,
     0,  0,  0, 59,  0, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0, 62, 63,  0, 64,  0,  0,  0,  0,  0,  0,  0,  0,
    65, 66, 67, 68, 69, 70, 71,  0, 72, 73,  0, 74, 75, 76, 77,  0,
     0,  0,  0,  0, 78, 79,  0,  0, 80, 81, 82,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 83, 84,  0,  0,  0,  0,  0,  0,  0,  0, 85,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0, 86,  0,  0,  0,  0,  0,  0,
     0,  0, 87, 88, 89,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0, 90, 91,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    92,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 93, 94,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    95,  0, 96,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_joining_type_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,
      2,   2,   2,   2,   2,   2,   2,   0,   3,   0,   0,   0,   0,   0,   0,   0,
      0,   4,   2,   5,   6,   0,   0,   0,   0,   7,   8,   9,  10,   2,  11,  12,
     13,  14,  15,  15,  16,  17,  18,  19,  20,  21,  22,   2,  23,  24,  25,  26,
      0,   0,  27,  28,  29,  15,  30,  31,   0,  32,  33,   0,  34,  35,  36,   0,
      0,   0,  37,  38,   0,  39,  40,   2,  41,   0,   0,  42,  43,  44,  45,   0,
     46,   0,   0,  47,  48,   0,  45,   0,  49,   0,   0,  47,  50,  46,   0,  51,
     49,   0,   0,  47,  52,   0,  45,  53,  46,   0,   0,  54,  48,  55,  45,   0,
     56,   0,   0,   0,  57,   0,   0,   0,  28,   0,   0,  58,  59,  60,  45,   0,
     46,   0,   0,  54,  61,   0,  45,   0,  62,   0,   0,  63,  48,   0,  45,   0,
      0,   0,   0,   0,  64,  65,   0,   0,   0,   0,   0,  66,  67,   0,   0,   0,
      0,   0,   0,  68,  69,   0,   0,   0,   0,  70,   0,  71,   0,   0,   0,  72,
     73,  74,   2,  75,  55,   0,   0,   0,   0,   0,  76,  77,   0,  78,  28,  79,
     80,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  76,   0,   0,
      0,  81,   0,  81,   0,  45,   0,  45,   0,   0,   0,  82,  83,  84,   0,   0,
     85,   0,  15,  15,  15,  15,  15,  86,  87,  15,  88,   0,   0,   0,   0,   0,
      0,   0,  89,  90,   0,   0,   0,   0,   0,  91,   0,   0,   0,  92,  93,  94,
      0,   0,   0,  95,   0,   0,   0,   0,  96,   0,   0,  97,  56,   0,  98,  96,
     62,   0,  99,   0,   0,   0, 100,  62,   0,   0, 101, 102,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 103, 104, 105,   0,   0,   0,   0,   2,   2,   2, 106,
    107,   0, 108,   0,   0,   0, 109,   0,   0,   0,   0,   0,   0,   2,   2,  28,
      0,   0,   0,   0,   0,   0,  20,  62,   0,   0,   0,   0,   0,   0,   0,  20,
      0,   0,   0,   0,   0,   0,   2,   2,   0,   0, 110,   0,   0,   0,   0,   0,
      0, 111,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20, 112,
      0,  58,   0,   0,   0,   0,   0,  62, 113,   0,  60,   0,  15,  15,  15, 114,
      0,   0,   0,   0, 115,   0,   2,  62,   0,   0, 116,   0, 117,  62,   0,   0,
     41,   0,   0, 118,   0,   0, 119,   0,   0,   0, 120, 121, 122,   0,   0,  47,
      0,   0,   0, 123,  46,   0, 124,  55,   0,   0,   0,   0,   0,   0, 125,   0,
      0, 126,   0,   0,   0,   0,   0,   0,   2,   0,   2,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   1,
      0,   0,   0,   0,   0,   0,  28,   0,   0,   0,   0,   0,   0,   0,   0, 128,
    129,   0,   0, 130,   0,   0,   0,   0,   0,   0,   0,   0, 131, 132, 133,   0,
    134, 135, 136,   0,   0,   0,   0,   0,  46,   0,   0, 137, 138,   0,   0,  20,
     62,   0,   0, 139,   0,   0,   0,   0,  41,   0, 140, 141,   0,   0,   0, 142,
     62,   0,   0, 143, 144,   0,   0,   0,   0,   0,  20, 145,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  20, 146,   0,  62,   0,   0,  47,  28,   0, 147, 141,
      0,   0,   0, 137,  65,   0,   0,   0,   0,   0,   0, 148, 149,   0,   0,   0,
      0,   0,   0, 150,  28, 124,   0,   0,   0,   0,   0, 151,  28,   0,   0,   0,
      0,   0, 152, 153,   0,   0,   0,   0,   0,  76, 154,   0,   0,   0,   0,   0,
    155,   0,   0, 156, 157, 158,   0,   0,  53, 159,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 160,   0,   0,   0,   0,   0, 161, 162, 163,   0,   0,   0,   0,
      0,   0,   0, 164, 153,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 141,
      0,   0,   0, 138,   0,   0,   0,   0,  20,  41,   0,   0,   0,   0,   0,   0,
      0, 165,  96,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 166, 167,
    168,   0, 110,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,
      2,   2,   2, 169,   2,   2,  75, 119, 170,  98,   4,   0,   0,   0,   0,   0,
    171, 172, 173,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 138,   0,   0,
     15,  15,  15,  15, 174,   0,   0,   0,  46,   0,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2,   2,   2,   2,
};

static RE_UINT8 re_joining_type_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  1,  2,  2,  2,  2,  3,  2,  4,  0,
     5,  2,  2,  2,  2,  2,  2,  6,  7,  6,  0,  0,  2,  2,  8,  9,
    10, 11, 12, 13, 14, 15, 15, 15, 16, 15, 17,  2,  0,  0,  0, 18,
    19, 20, 15, 15, 15, 15, 21, 21, 21, 21, 22, 15, 15, 15, 15, 15,
    23, 21, 21, 24, 25, 26,  2, 27,  2, 27, 28, 29,  0,  0, 18, 30,
     0,  0,  0,  3, 31, 32, 22, 33, 15, 15, 34, 23,  2,  2,  8, 35,
    15, 15, 32, 15, 15, 15, 13, 36, 24, 36, 22, 15,  0, 37,  2,  2,
     9,  0,  0,  0,  0,  0, 18, 15, 15, 15, 38,  2,  2,  0, 39,  0,
     0, 37,  6,  2,  2,  5,  5,  4, 36, 25, 12, 15, 15, 40,  5,  0,
    41, 42, 43,  0, 15, 15, 25, 44, 45, 41, 12, 46,  0,  2,  2,  2,
     6,  2,  2,  2,  8,  0,  0,  0,  0,  0, 47,  9,  5,  2,  9,  1,
     5,  2,  0,  0, 37,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  9,
     5,  9,  0,  1,  7,  0,  0,  0,  7,  3, 27,  4,  4,  1,  0,  0,
     5,  6,  9,  1,  0,  0, 37,  2,  0,  0,  0, 27,  0, 47,  0,  0,
    47,  0,  0,  0,  9,  0,  0,  1,  0,  0,  0, 37,  9, 37, 28,  4,
     0,  7,  0,  0,  0, 47,  0,  4,  4,  0,  0,  0,  0,  0,  3,  9,
     0,  0, 47,  0, 37, 48,  0,  0,  1,  2,  8,  0,  0,  3,  2,  8,
     1,  2,  6,  9,  0,  0,  2,  4,  0,  0,  4,  0,  0, 49,  1,  0,
     5,  2,  2,  8,  2, 28,  0,  5,  2,  2,  5,  2,  2,  2,  2,  9,
     0,  0,  0,  5, 28,  2,  7,  7,  0,  0,  4, 37,  5,  9,  0,  0,
    47,  7,  0,  1, 37,  9,  0,  0,  0,  6,  2,  4,  0, 47,  5,  2,
     2,  0,  0,  1,  0, 50, 51,  4, 15, 15,  0,  0,  0, 52, 15, 15,
    15, 15, 53,  0,  8,  3,  9,  0, 47,  0,  5,  0,  0,  3, 27,  0,
     0, 47,  2,  8, 48,  5,  2,  9,  3,  2,  2, 27,  2,  2,  2,  8,
     2,  0,  0,  0,  0, 28,  8,  9,  0,  0,  3,  2, 37,  4,  6,  4,
     0, 47,  4, 49,  0,  0,  0,  2,  2, 37,  0,  0,  8,  2,  2,  2,
    28,  2,  9,  1,  0,  9,  4,  0,  2,  2,  6,  2,  0,  0,  3, 54,
     0,  0, 37,  8,  2,  9, 37,  2,  0,  0, 37,  4,  0,  0,  7,  0,
     8,  2,  2,  4, 47, 47,  3,  0, 55,  0,  0,  0,  0,  4,  0,  0,
     0, 37,  2,  4,  0,  3,  2,  2,  3, 37,  4,  9,  0,  1,  0,  0,
     0,  0,  5,  8,  7,  7,  0,  0,  3,  0,  0,  9, 28, 27,  9, 37,
     0,  0,  0,  4,  0,  1,  9,  1,  0,  0,  0, 47,  0,  0,  5,  0,
     0, 37,  8,  0,  5,  7,  0,  2,  0,  0,  8,  3, 15, 56, 57, 58,
    14, 59, 15, 12, 60, 61, 50, 13, 24, 22, 12, 62, 60,  0,  0,  0,
     0,  0, 20, 63,  0,  0,  2,  2,  2,  8,  0,  0,  3,  8,  7,  1,
     0,  3,  2,  5,  2,  9,  0,  0,  3,  0,  0,  0,  0, 37,  2,  8,
     0,  0, 37,  9,  4, 28,  0, 47,  3,  2,  8,  0,  0, 37,  2,  9,
     3,  2, 48,  3, 28,  0,  0,  0, 37,  4,  0,  6,  3,  2,  8, 49,
     0,  0,  3,  1,  2,  6,  0,  0, 37,  6,  2,  0,  5,  8,  7,  0,
     3,  2, 27,  8,  0,  3,  0,  0,  5,  8,  5,  0,  2,  8,  4,  0,
     2,  8,  2,  6, 37,  2,  2,  2,  2,  2, 37,  2, 28,  7,  0,  0,
     5,  8, 47,  6,  0,  0,  0,  7,  0,  3,  4,  0,  3,  2,  2,  2,
     8,  5,  2,  0,  2,  8,  3,  2,  0,  9,  0,  0,  2,  8,  2,  2,
     2,  2, 27,  2,  6, 28,  8,  0, 15,  2,  8,  0,
};

static RE_UINT8 re_joining_type_stage_5[] = {
    0, 0, 0, 0, 0, 5, 0, 0, 5, 5, 5, 5, 0, 0, 0, 5,
    5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 0, 5, 0, 5, 5, 0,
    5, 5, 5, 0, 5, 0, 0, 0, 2, 0, 3, 3, 3, 3, 2, 3,
    2, 3, 2, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 3, 2, 2, 5, 0, 0, 2, 2, 5, 3, 3, 3,
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 2, 2, 3,
    2, 3, 2, 3, 2, 2, 3, 3, 0, 3, 5, 5, 5, 0, 0, 5,
    5, 0, 5, 5, 5, 5, 3, 3, 2, 0, 0, 2, 3, 5, 2, 2,
    2, 3, 3, 3, 2, 2, 3, 2, 3, 2, 3, 2, 0, 3, 2, 2,
    3, 2, 2, 2, 0, 0, 5, 5, 2, 2, 2, 5, 0, 0, 1, 0,
    3, 2, 0, 0, 2, 0, 2, 2, 2, 2, 0, 3, 2, 3, 3, 0,
    3, 0, 3, 2, 2, 3, 3, 2, 2, 2, 0, 0, 0, 0, 5, 0,
    5, 0, 5, 0, 0, 5, 0, 5, 0, 0, 0, 2, 0, 0, 1, 5,
    0, 5, 5, 2, 2, 5, 2, 0, 0, 1, 5, 5, 2, 2, 4, 0,
    2, 3, 0, 3, 0, 3, 3, 0, 0, 4, 3, 3, 2, 2, 2, 4,
    2, 3, 0, 0, 3, 5, 5, 0, 3, 2, 3, 3, 3, 2, 2, 0,
};

/* Joining_Type: 2452 bytes. */

RE_UINT32 re_get_joining_type(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_joining_type_stage_1[f] << 5;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_joining_type_stage_2[pos + f] << 3;
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
    12, 13, 14, 15, 16, 10, 17,  5, 18, 10, 10, 19, 10, 20, 21, 22,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 23,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 23,
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
    24, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
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
    110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
    110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
    110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
    110, 110,  79,  79,  79,  79, 111, 112,   2,   2, 113, 114, 115, 116, 117, 118,
    119, 120, 121, 122, 110, 123, 124, 125,   2, 126, 127, 110,   2,   2, 128, 110,
    129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 110, 110, 139, 110, 110, 110,
    140, 141, 142, 143, 144, 145, 146, 110, 147, 148, 110, 149, 150, 151, 152, 110,
    110, 153, 110, 110, 154, 155, 110, 110, 156, 157, 158, 110, 110, 110, 110, 110,
      2,   2,   2,   2,   2,   2,   2, 159, 160,   2, 161, 110, 110, 110, 110, 110,
    110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      2,   2,   2,   2, 162, 163, 164,   2, 165, 110, 110, 110, 110, 110, 110, 110,
    110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
    110, 110, 110, 110, 110, 110, 110, 110,   2,   2,   2, 166, 167, 110, 110, 110,
    110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
    110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      2,   2,   2,   2, 168, 169, 170, 171, 110, 110, 110, 110, 110, 110, 172, 173,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79, 174,
     79,  79,  79,  79,  79, 175, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
     79,  79, 176,  79,  79, 177, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
    110, 110, 110, 110, 110, 110, 110, 110, 178, 179, 110, 110, 110, 110, 110, 110,
      2, 180, 181, 182, 183, 110, 184, 110, 185, 186, 187,   2,   2, 188,   2, 189,
      2,   2,   2,   2, 190, 191, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
    192, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      2, 193, 194, 110, 110, 110, 110, 110, 110, 110, 110, 110, 195, 196, 110, 110,
     79,  79, 197, 198,  79,  79,  79, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79, 212,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,
     79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79, 212,
    213, 110, 214, 215, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
    110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
};

static RE_UINT16 re_line_break_stage_3[] = {
      0,   1,   2,   3,   4,   5,   4,   6,   7,   1,   8,   9,   4,  10,   4,  10,
      4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  11,  12,   4,   4,
      1,   1,   1,   1,  13,  14,  15,  16,  17,   4,  18,   4,   4,   4,   4,   4,
     19,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  20,   4,  21,  20,   4,
     22,  23,   1,  24,  25,  26,  27,  28,  29,  30,   4,   4,  31,   1,  32,  33,
      4,   4,   4,   4,   4,  34,  35,  36,  37,  38,   4,   1,  39,   4,   4,   4,
      4,   4,  40,  41,  36,   4,  31,  42,   4,  43,  44,  45,   4,  46,  47,  48,
     48,  48,   4,  49,  48,  50,  51,   1,  52,   4,   4,  53,   1,  54,  55,   4,
     56,  57,  58,  59,  60,  61,  62,  63,  64,  57,  58,  65,  66,  67,  68,  69,
     70,  18,  58,  71,  72,  73,  62,  74,  75,  57,  58,  71,  76,  77,  62,  78,
     79,  80,  81,  82,  83,  84,  68,  85,  86,  87,  58,  88,  89,  90,  62,  91,
     92,  87,  58,  93,  89,  94,  62,  95,  86,  87,   4,  96,  97,  98,  62,  99,
    100, 101,   4, 102, 103, 104,  68, 105, 106, 107, 107, 108, 109, 110,  48,  48,
    111, 112, 113, 114, 115, 116,  48,  48, 117, 118,  36, 119, 120,   4, 121, 122,
    123, 124,   1, 125, 126, 127,  48,  48, 107, 107, 107, 107, 128, 107, 107, 107,
    107, 129,   4,   4, 130,   4,   4,   4, 131, 131, 131, 131, 131, 131, 132, 132,
    132, 132, 133, 134, 134, 134, 134, 134,   4,   4,   4,   4, 135, 136,   4,   4,
    135,   4,   4, 137, 138, 139,   4,   4,   4, 138,   4,   4,   4, 140, 141, 121,
      4, 142,   4,   4,   4,   4,   4, 143, 144,   4,   4,   4,   4,   4,   4,   4,
    144, 145,   4,   4,   4,   4, 146, 147, 148, 149,   4, 150,   4, 151, 148, 152,
    107, 107, 107, 107, 107, 153, 154, 142, 155, 154,   4,   4,   4,   4,   4,  78,
    156,   4, 157,   4,   4,   4,   4, 158,   4,  45, 159, 159, 160, 107, 161, 162,
    107, 107, 163, 107, 164, 165,   4,   4,   4, 166, 107, 107, 107, 167, 107, 168,
    154, 154, 161, 169,  48,  48,  48,  48, 170,   4,   4, 171, 172, 173, 174, 175,
    176,   4, 177,  36,   4,   4,  40, 178,   4,   4, 171, 179, 180,  36,   4, 181,
    147,  48,  48,  48,  78, 182, 183, 184,   4,   4,   4,   4,   1,   1,   1, 185,
      4, 143,   4,   4, 143, 186,   4, 187,   4,   4,   4, 188, 188, 189,   4, 190,
    191, 192, 193, 194, 195, 196, 197, 198, 199, 121, 200, 201, 202,   1,   1, 203,
    204, 205, 206,   4,   4, 207, 208, 209, 210, 209,   4,   4,   4, 211,   4,   4,
    212, 213, 214, 215, 216, 217, 218,   4, 219, 220, 221, 222,   4,   4, 223,   4,
    224, 225, 226,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 227,
      4,   4, 228,  48,  47,  48, 229, 229, 229, 229, 229, 229, 229, 229, 229, 230,
    229, 229, 229, 229, 208, 229, 229, 231, 229, 232, 233, 234, 235, 236, 237,   4,
    238, 239,   4, 240, 241,   4, 242, 243,   4, 244,   4, 245, 246, 247, 248, 249,
    250,   4,   4,   4,   4, 251, 252, 253, 229, 254,   4,   4, 255,   4, 256,   4,
    257, 258,   4,   4,   4, 224,   4, 259,   4,   4,   4,   4,   4, 260,   4, 261,
      4, 262,   4, 263,  58, 264, 265,  48,   4,   4,  45,   4,   4,  45,   4,   4,
      4,   4,   4,   4,   4,   4, 266, 267,   4,   4, 130,   4,   4,   4, 268, 269,
      4, 228, 270, 270, 270, 270,   1,   1, 271, 272, 273, 274, 275,  48,  48,  48,
    276, 277, 276, 276, 276, 276, 276, 278, 276, 276, 276, 276, 276, 276, 276, 276,
    276, 276, 276, 276, 276, 279,  48, 280, 281, 282, 283, 284, 285, 276, 286, 276,
    287, 288, 289, 276, 286, 276, 287, 290, 291, 276, 292, 293, 276, 276, 276, 276,
    292, 276, 276, 294, 276, 276, 278, 295, 276, 292, 276, 276, 296, 276, 276, 276,
    276, 276, 276, 276, 276, 276, 276, 292, 276, 276, 276, 276,   4,   4,   4,   4,
    276, 297, 276, 276, 276, 276, 276, 276, 298, 276, 276, 276, 299,   4,   4, 181,
    300,   4, 301,  48,   4,   4, 266, 302,   4, 303,   4,   4,   4,   4,   4, 304,
      4,   4,  45,  78,  48,  48,  48, 305, 306,   4, 307, 308,   4,   4,   4, 309,
    310,   4,   4, 171, 311, 154,   1, 312,  36,   4, 313,   4, 314, 315, 131, 316,
     52,   4,   4, 317, 318, 319, 107, 320,   4,   4, 321, 322, 323, 324, 107, 107,
    107, 107, 107, 107, 325, 326,  31, 327, 328, 329, 270,   4,   4,   4, 158,   4,
      4,   4,   4,   4,   4,   4, 330, 154, 331, 332, 333, 334, 333, 335, 333, 331,
    332, 333, 334, 333, 335, 333, 331, 332, 333, 334, 333, 335, 333, 331, 332, 333,
    334, 333, 335, 333, 331, 332, 333, 334, 333, 335, 333, 331, 332, 333, 334, 333,
    335, 333, 331, 332, 333, 334, 333, 335, 333, 331, 332, 333, 334, 333, 335, 333,
    334, 333, 336, 132, 337, 134, 134, 338, 339, 339, 339, 339, 339, 339, 339, 339,
     48,  48,  48,  48,  48,  48,  48,  48, 228, 340, 341, 342, 343,   4,   4,   4,
      4,   4,   4,   4, 344, 345,   4,   4,   4,   4,   4, 346,  48,   4,   4,   4,
      4, 347,   4,   4,  78,  48,  48, 348,   1, 349,   1, 350, 351, 352, 353, 188,
      4,   4,   4,   4,   4,   4,   4, 354, 355, 356, 276, 357, 276, 358, 359, 360,
    276, 361, 276, 292, 362, 363, 364, 365, 366,   4, 139, 367, 187, 187,  48,  48,
      4,   4,   4,   4,   4,   4,   4,  47, 368,   4,   4, 369,   4,   4,   4,   4,
     45, 370,  73,  48,  48,   4,   4, 371,   4, 121,   4,   4,   4,  73,  33, 370,
      4,   4, 372,   4,  47,   4,   4, 373,   4, 374,   4,   4, 375, 376,  48,  48,
      4, 187, 154,   4,   4, 375,   4, 370,   4,   4,  78,   4,   4,   4, 377,  48,
      4,   4,   4, 228,   4, 158,  78,  48, 378,   4,   4, 379,   4, 380,   4,   4,
      4,  45, 305,  48,  48,  48,   4, 381,   4, 382,   4, 383,  48,  48,  48,  48,
      4,   4,   4, 384,   4, 347,   4,   4, 385, 386,   4, 387,  78, 388,   4,   4,
      4,   4,  48,  48,   4,   4, 389, 390,   4,   4,   4, 391,   4, 262,   4, 392,
      4, 393, 394,  48,  48,  48,  48,  48,   4,   4,   4,   4, 147,  48,  48,  48,
      4,   4,   4, 264,   4,   4,   4, 395,  48,  48,  48,  48,  48,  48,   4,  45,
    176,   4,   4, 396, 397, 347, 398, 399, 176,   4,   4, 400, 401,   4, 147, 154,
    176,   4, 314, 402, 403,   4,   4, 404, 176,   4,   4, 317, 405, 406,  20, 407,
      4,  18, 408, 409,  48,  48,  48,  48, 410,  37, 411,   4,   4, 266, 412, 154,
    413,  57,  58,  71,  76, 414, 415, 416,   4,   4,   4, 417, 418, 419,  48,  48,
      4,   4,   4,   1, 420, 154,  48,  48,   4,   4, 266, 421, 422, 423,  48,  48,
      4,   4,   4,   1, 424, 154, 425,  48,   4,   4,  31, 426, 154,  48,  48,  48,
    107, 427, 163, 428,  48,  48,  48,  48,  48,  48,   4,   4,   4,   4,  36, 429,
    430,   4,   4, 431, 432, 433,   4,   4, 434, 435, 436,  48,   4,   4,   4, 147,
     58,   4, 266, 437, 438,  36, 121, 439,   4, 440, 124, 322,  48,  48,  48,  48,
    441,   4,   4, 442, 443, 154,  48,  48,   4, 142,  48,  48,  48,  48,  48,  48,
      4,   4,   4,   4,   4,   4,  45, 444,   4,   4,   4,   4, 445,  48,  48,  48,
      4,   4,   4,   4,   4, 446,   4,   4, 447,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   4,   4,   4,   4, 448,   4,   4,  45,  48,  48,  48,  48,  48,
      4,   4,   4,   4, 449,   4,   4,   4,   4,   4,   4,   4, 228,  48,  48,  48,
      4,   4,   4, 147,   4,  45, 450,  48,  48,  48,  48,  48,  48,   4, 187, 451,
      4,   4,   4, 452, 453, 454,  18, 455,   4,  48,  48,  48,  48,  48,  48,  48,
      4,   4,   4,   4, 407, 456,   1, 169, 399, 176,  48,  48,  48,  48, 457,  48,
    276, 276, 276, 276, 276, 276, 298,  48, 276, 276, 276, 276, 276, 276, 276, 458,
    276, 292,  48,  48,  48,  48,  48, 276, 276, 276, 276, 276, 276, 276, 276, 280,
      4,   4,   4,   4,   4,   4,  47, 121, 147, 459, 460,  48,  48,  48,  48,  48,
      4,   4,   4,   4,   4,   4,   4, 158,   4,   4,  21,   4,   4,   4, 461,   1,
    462,   4, 463,   4,   4,   4, 147,  48,   4,   4,   4,   4, 464,  48,  48,  48,
      4,   4,   4,   4,   4, 228,   4, 344,   4,   4,   4,   4,   4, 188,   4,   4,
      4, 148, 465, 466, 467,   4,   4,   4, 468, 469,   4, 470, 471,  87,   4,   4,
      4,   4, 262,   4,   4,   4,   4,   4,   4,   4,   4,   4, 472, 473, 473, 473,
      1,   1,   1, 474,   1,   1, 475, 476, 477, 478,  23,  48,  48,  48,  48,  48,
    437, 479, 480,  48,  48,  48,  48,  48,   4,   4,   4,   4, 481, 322,  48,  48,
      4,   4,   4,   4, 482, 483,  48,  48, 467,   4, 484, 485, 486, 487, 488, 489,
    490, 370, 491, 370,  48,  48,  48, 344, 492, 229, 493, 229, 229, 229, 494, 229,
    229, 229, 492, 276, 276, 276, 495, 496, 497, 498, 276, 499, 500, 276, 276, 501,
    276, 276, 276, 276, 502, 503, 504, 505, 506, 276, 507, 508, 276, 276, 276, 276,
    509, 510, 511, 512, 513, 276, 276, 514, 276, 515, 276, 276, 276, 516, 276, 517,
    276, 276, 276, 276, 518,   4,   4, 519, 276, 276, 520, 521, 522, 276, 276, 276,
      4,   4,   4,   4,   4,   4,   4, 523,   4,   4,   4,   4,   4, 511, 276, 276,
    524,   4,   4,   4, 525, 513,   4,   4, 525,   4, 526, 276, 276, 276, 276, 276,
    524, 527, 528, 529, 276, 276, 276, 276, 276, 276, 276, 276, 276, 530, 276, 276,
    276, 276, 276, 276, 276, 276, 276, 531, 532,  48,   1,   1,   1,   1,   1,   1,
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
     39,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  38,  39,  14,  14,  14,
     14,  14,  14,  14,  40,  41,  39,   9,  42,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  43,  19,  44,   0,  45,  36,  36,  36,  36,
     46,  46,  46,  46,  46,  46,  46,  46,  46,  46,  46,  46,  46,  47,  36,  36,
     46,  48,  38,  36,  36,  36,  36,  36,  14,  14,  14,  14,  49,  50,  13,  14,
      0,   0,   0,   0,   0,  51,  52,  53,  14,  14,  14,  14,  14,  19,   0,   0,
     12,  12,  12,  12,  12,  54,  55,  14,  44,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  56,   0,   0,   0,  44,  19,   0,   0,  44,  19,  44,   0,   0,  14,
     12,  12,  12,  12,  12,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  39,
     19,  14,  14,  14,  14,  14,  14,  14,   0,   0,   0,   0,   0,  52,  39,  14,
     14,  14,  14,   0,   0,   0,   0,   0,  44,  36,  36,  36,  36,  36,  36,  36,
      0,   0,  14,  14,  57,  38,  36,  36,  14,  14,  14,   0,   0,  19,   0,   0,
      0,   0,  19,   0,  19,   0,   0,  36,  14,  14,  14,  14,  14,  14,  14,  38,
     14,  14,  14,  14,  19,   0,  36,  38,  14,  14,  14,  14,  14,  38,  36,  36,
     36,  36,  36,  36,  36,  36,  36,  36,  14,  14,  38,  14,  14,  14,  14,  36,
     36,  36,   0,   0,   0,   0,   0,   0,   0,  19,   0,   0,   0,   0,   0,   0,
      0,   0,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,   0,  44,   0,
     19,   0,   0,   0,  14,  14,  14,  14,  14,   0,  58,  12,  12,  12,  12,  12,
     19,   0,  39,  14,  14,  14,  38,  39,  38,  39,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  38,  14,  14,  14,  38,  38,  36,  14,  14,  36,  44,   0,
      0,   0,  52,  42,  52,  42,   0,  38,  36,  36,  36,  42,  36,  36,  14,  39,
     14,   0,  36,  12,  12,  12,  12,  12,  14,  50,  14,  14,  49,   9,  14,  36,
     42,   0,  39,  14,  14,  38,  36,  39,  38,  14,  39,  38,  14,  36,  52,   0,
      0,  52,  36,  42,  52,  42,   0,  36,  42,  36,  36,  36,  39,  14,  38,  38,
     36,  36,  36,  12,  12,  12,  12,  12,   0,  14,  19,  36,  36,  36,  36,  36,
     42,   0,  39,  14,  14,  14,  14,  39,  38,  14,  39,  14,  14,  36,  44,   0,
      0,   0,   0,  42,   0,  42,   0,  36,  38,  36,  36,  36,  36,  36,  36,  36,
      9,  36,  36,  36,  39,   0,   0,   0,  42,   0,  39,  14,  14,  14,  38,  39,
      0,   0,  52,  42,  52,  42,   0,  36,  36,  36,  36,   0,  36,  36,  14,  39,
     14,  14,  14,  14,  36,  36,  36,  36,  36,  44,  39,  14,  14,  38,  36,  14,
     38,  14,  14,  36,  39,  38,  38,  14,  36,  39,  38,  36,  14,  38,  36,  14,
     14,  14,  14,  14,  14,  36,  36,   0,   0,  52,  36,   0,  52,   0,   0,  36,
     38,  36,  36,  42,  36,  36,  36,  36,  14,  14,  14,  14,   9,  38,  36,  36,
      0,   0,  39,  14,  14,  14,  38,  14,  38,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  36,  39,   0,   0,   0,  52,   0,  52,   0,   0,  36,
     36,  36,  42,  52,  14,  38,  36,  36,  36,  36,  36,  36,  14,  14,  14,  14,
     19,   0,  39,  14,  14,  14,  38,  14,  14,  14,  39,  14,  14,  36,  44,   0,
     36,  36,  42,  52,  36,  36,  36,  38,  39,  38,  36,  36,  36,  36,  36,  36,
     14,  14,  14,  14,  14,  19,  44,   0,   0,   0,  52,   0,  52,   0,   0,  14,
     36,  36,  14,  19,  14,  14,  14,  14,  14,  14,  14,  14,  49,  14,  14,  14,
     36,   0,  39,  14,  14,  14,  14,  14,  14,  14,  14,  38,  36,  14,  14,  14,
     14,  39,  14,  14,  14,  14,  39,  36,  14,  14,  14,  38,  36,  52,  36,  42,
      0,   0,  52,  52,   0,   0,   0,   0,  36,   0,  38,  36,  36,  36,  36,  36,
     59,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,
     60,  60,  60,  60,  60,  61,  36,  62,  60,  60,  60,  60,  60,  60,  60,  63,
     12,  12,  12,  12,  12,  58,  36,  36,  59,  61,  61,  59,  61,  61,  59,  36,
     36,  36,  60,  60,  59,  60,  60,  60,  59,  60,  59,  59,  36,  60,  59,  60,
     60,  60,  60,  60,  60,  59,  60,  36,  60,  60,  61,  61,  60,  60,  60,  36,
     12,  12,  12,  12,  12,  36,  60,  60,  32,  64,  29,  64,  65,  66,  67,  53,
     53,  68,  56,  14,   0,  14,  14,  14,  14,  14,  43,  19,  19,  69,  69,   0,
     14,  14,  14,  14,  39,  14,  14,  14,  14,  14,  14,  14,  14,  14,  38,  36,
     42,   0,   0,   0,   0,   0,   0,   1,   0,   0,   1,   0,  14,  14,  19,   0,
      0,   0,   0,   0,  42,   0,   0,   0,   0,   0,   0,   0,   0,   0,  52,  58,
     14,  14,  14,  44,  14,  14,  38,  14,  64,  70,  14,  14,  71,  72,  36,  36,
     12,  12,  12,  12,  12,  58,  14,  14,  12,  12,  12,  12,  12,  60,  60,  60,
     14,  14,  14,  39,  36,  36,  39,  36,  73,  73,  73,  73,  73,  73,  73,  73,
     74,  74,  74,  74,  74,  74,  74,  74,  74,  74,  74,  74,  75,  75,  75,  75,
     75,  75,  75,  75,  75,  75,  75,  75,  14,  14,  14,  14,  38,  14,  14,  36,
     14,  14,  14,  38,  38,  14,  14,  36,  38,  14,  14,  36,  14,  14,  14,  38,
     38,  14,  14,  36,  14,  14,  14,  14,  14,  14,  14,  38,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  38,  42,   0,  27,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  36,  36,  36,  14,  14,  14,  36,  14,  14,  14,  36,
     76,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  16,  77,  36,
     14,  14,  14,  14,  14,  27,  58,  14,  14,  14,  14,  14,  38,  36,  36,  36,
     14,  14,  14,  14,  14,  14,  38,  14,  14,   0,  52,  36,  36,  36,  36,  36,
     14,   0,   1,  41,  36,  36,  36,  36,  14,   0,  36,  36,  36,  36,  36,  36,
     38,   0,  36,  36,  36,  36,  36,  36,  60,  60,  58,  78,  76,  79,  60,  36,
     12,  12,  12,  12,  12,  36,  36,  36,  14,  53,  58,  29,  53,  19,   0,  72,
     14,  14,  19,  44,  14,  14,  14,  14,  14,  14,  14,  14,  19,  38,  36,  36,
     14,  14,  14,  36,  36,  36,  36,  36,   0,   0,   0,   0,   0,   0,  36,  36,
     38,  36,  53,  12,  12,  12,  12,  12,  60,  60,  60,  60,  60,  60,  60,  36,
     60,  60,  61,  36,  36,  36,  36,  36,  60,  60,  60,  60,  60,  60,  36,  36,
     60,  60,  60,  60,  60,  36,  36,  36,  12,  12,  12,  12,  12,  61,  36,  60,
     14,  14,  14,  19,   0,   0,  36,  14,  60,  60,  60,  60,  60,  60,  60,  61,
     60,  60,  60,  60,  60,  60,  61,  42,   0,   0,   0,   0,   0,   0,   0,  52,
      0,   0,  44,  14,  14,  14,  14,  14,  14,  14,   0,   0,   0,   0,   0,   0,
      0,   0,  44,  14,  14,  14,  36,  36,  12,  12,  12,  12,  12,  58,  27,  58,
     76,  14,  14,  14,  14,  19,   0,   0,   0,   0,  14,  14,  14,  14,  38,  36,
      0,  44,  14,  14,  14,  14,  14,  14,  19,   0,   0,   0,   0,   0,   0,  14,
      0,   0,  36,  36,  36,  36,  14,  14,   0,   0,   0,   0,  36,  80,  58,  58,
     12,  12,  12,  12,  12,  36,  39,  14,  14,  14,  14,  14,  14,  14,  14,  58,
      0,  44,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  44,  14,  19,  14,
     14,   0,  44,  19,   0,  36,  36,  36,   0,   0,   0,   0,   0,  42,   0,   0,
     14,  14,  14,  14,  39,  39,  39,  39,  14,  14,  14,  14,  14,  14,  14,  36,
     14,  14,  38,  14,  14,  14,  14,  14,  14,  14,  36,  14,  14,  14,  39,  14,
     36,  14,  38,  14,  14,  14,  32,  38,  58,  58,  58,  81,  58,  82,  83,   0,
     81,  58,  84,  25,  85,  86,  85,  86,  28,  14,  87,  88,  89,   0,   0,  33,
     50,  50,  50,  50,   7,  90,  91,  14,  14,  14,  92,  93,  91,  14,  14,  14,
     14,  14,  14,  76,  58,  58,  27,  58,  94,  14,  38,   0,   0,   0,   0,   0,
     14,  36,  25,  14,  14,  14,  16,  95,  24,  28,  25,  14,  14,  14,  16,  77,
     23,  23,  23,   6,  23,  23,  23,  23,  23,  23,  23,  22,  23,   6,  23,  22,
     23,  23,  23,  23,  23,  23,  23,  23,  52,  36,  36,  36,  36,  36,  36,  36,
     14,  49,  24,  14,  49,  14,  14,  14,  14,  24,  14,  96,  14,  14,  14,  14,
     24,  25,  14,  14,  14,  24,  14,  14,  14,  14,  28,  14,  14,  24,  14,  25,
     28,  28,  28,  28,  28,  28,  14,  14,  28,  28,  28,  28,  28,  14,  14,  14,
     14,  14,  14,  14,  24,  14,  36,  36,  14,  25,  25,  14,  14,  14,  14,  14,
     25,  28,  14,  24,  25,  24,  14,  24,  24,  23,  24,  14,  14,  25,  24,  28,
     25,  24,  24,  24,  28,  28,  25,  25,  14,  14,  28,  28,  14,  14,  28,  14,
     14,  14,  14,  14,  25,  14,  25,  14,  14,  25,  14,  14,  14,  14,  14,  14,
     28,  14,  28,  28,  14,  28,  14,  28,  14,  28,  14,  28,  14,  14,  14,  14,
     14,  14,  24,  14,  24,  14,  14,  14,  14,  14,  24,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  24,  14,  14,  14,  14,  14,  14,  14,  97,
     14,  14,  14,  14,  69,  69,  14,  14,  14,  25,  14,  14,  14,  98,  14,  14,
     14,  14,  14,  14,  16,  99,  14,  14,  98,  98,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  38,  36,  36,  36,  36,  28,  28,  28,  28,  28,  28,  28,  28,
     28,  28,  28,  28,  28,  28,  28,  25,  28,  28,  25,  14,  14,  14,  14,  14,
     14,  28,  28,  14,  14,  14,  14,  14,  28,  24,  28,  28,  28,  14,  14,  14,
     14,  28,  14,  28,  14,  14,  28,  14,  28,  14,  14,  28,  25,  24,  14,  28,
     28,  14,  14,  14,  14,  14,  14,  14,  14,  28,  28,  14,  14,  14,  14,  24,
     98,  98,  24,  25,  24,  14,  14,  28,  14,  14,  98,  28, 100,  98, 101,  98,
     14,  14,  14,  14, 102,  98,  14,  14,  25,  25,  14,  14,  14,  14,  14,  14,
     28,  24,  28,  24, 103,  25,  28,  24,  14,  14,  14,  14,  14,  14,  14, 102,
     14,  14,  14,  14,  14,  14,  14,  28,  14,  14,  14,  14,  14,  14, 102,  98,
     98,  98,  98,  98, 103,  28, 104, 102,  98, 104, 103,  28,  98,  28, 103, 104,
     98,  24,  14,  14,  28, 103,  28,  28, 104,  98,  98, 104, 101, 103, 104,  98,
     98,  98, 100,  14,  98, 105, 105,  14,  14,  14,  14,  24,  14,   7,  85,  85,
      5,  53, 100,  14,  69,  69,  69,  69,  69,  69,  69,  28,  28,  28,  28,  28,
     28,  28,  14,  14,  14,  14,  14,  14,  14,  14,  16,  99,  14,  14,  14,  14,
     14,  14,  14,  69,  69,  69,  69,  69,  14,  16, 106, 106, 106, 106, 106, 106,
    106, 106, 106, 106,  99,  14,  14,  14,  14,  14,  14,  14,  14,  14,  69,  14,
     14,  14,  24,  28,  28,  14,  14,  14,  14,  14,  36,  14,  14,  14,  14,  14,
     14,  14,  14,  36,  14,  14,  14,  14,  14,  14,  14,  14,  14,  36,  39,  14,
     14,  38,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,  14,  14,
     14,  14,  14,  14,  14,  14,  14,  19,   0,  14,  36,  36, 107,  58,  76, 108,
     14,  14,  14,  14,  36,  36,  36,  39,  41,  36,  36,  36,  36,  36,  36,  42,
     14,  14,  14,  38,  14,  14,  14,  38,  85,  85,  85,  85,  85,  85,  85,  58,
     58,  58,  58,  27, 109,  14,  85,  14,  85,  69,  69,  69,  69,  58,  58,  56,
     58,  27,  76,  14,  14, 110,  58,  76,  58, 109,  58,  58,  58,  36,  36,  36,
     98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98, 111,  98,  98,
     98,  98,  36,  36,  36,  36,  36,  36,  98,  98,  98,  36,  36,  36,  36,  36,
     98,  98,  98,  98,  98,  98,  36,  36,  18, 112, 113,  98,  69,  69,  69,  69,
     69,  98,  69,  69,  69,  69, 114, 115,  98,  98,  98,  98,  98,   0,   0,   0,
     98,  98, 116,  98,  98, 113, 117,  98, 118, 119, 119, 119, 119,  98,  98,  98,
     98, 119,  98,  98,  98,  98,  98,  98,  98, 119, 119, 119,  98,  98,  98, 120,
     98,  98, 119, 121,  42, 122,  91, 117, 123, 119, 119, 119, 119,  98,  98,  98,
     98,  98, 119, 120,  98, 113, 124, 117,  36,  36, 111,  98,  98,  98,  98,  98,
     98,  98,  98,  98,  98,  98,  98, 125, 111,  98,  98,  98,  98,  98,  98,  98,
     98,  98,  98,  98,  98, 125,  36,  36, 126, 126, 126, 126, 126, 126, 126, 126,
     98,  98,  98,  98,  28,  28,  28,  28,  98,  98, 113,  98,  98,  98,  98,  98,
     98,  98,  98,  98,  98,  98, 125,  36,  98,  98,  98, 125,  36,  36,  36,  36,
     14,  14,  14,  14,  14,  14,  27, 108,  12,  12,  12,  12,  12,  14,  36,  36,
      0,  44,   0,   0,   0,   0,   0,  14,  14,  14,  14,  14,  14,  14,  14,   0,
      0,  27,  58,  58,  36,  36,  36,  36,  36,  36,  36,  39,  14,  14,  14,  14,
     14,  44,  14,  44,  14,  19,  14,  14,  14,  19,   0,   0,  14,  14,  36,  36,
     14,  14,  14,  14, 127,  36,  36,  36,  14,  14,  64,  53,  36,  36,  36,  36,
      0,  14,  14,  14,  14,  14,  14,  14,   0,   0,   0,  36,  36,  36,  36,  58,
      0,  14,  14,  14,  14,  14,  29,  36,  14,  14,  14,   0,   0,   0,   0,  58,
     14,  14,  14,  19,   0,   0,   0,   0,   0,   0,  36,  36,  36,  36,  36,  39,
     73,  73,  73,  73,  73,  73, 128,  36,  14,  19,   0,   0,   0,   0,   0,   0,
     44,  14,  14,  27,  58,  14,  14,  39,  12,  12,  12,  12,  12,  36,  36,  14,
     12,  12,  12,  12,  12,  60,  60,  61,  14,  14,  14,  14,  19,   0,   0,   0,
      0,   0,   0,  52,  36,  36,  36,  36,  14,  19,  14,  14,  14,  14,   0,  36,
     12,  12,  12,  12,  12,  36,  27,  58,  60,  61,  36,  36,  36,  36,  36,  36,
     36,  36,  36,  36,  36,  59,  60,  60,  58,  14,  19,  52,  36,  36,  36,  36,
     39,  14,  14,  38,  39,  14,  14,  38,  39,  14,  14,  38,  36,  36,  36,  36,
     14,  19,   0,   0,   0,   1,   0,  36, 129, 130, 130, 130, 130, 130, 130, 130,
    130, 130, 130, 130, 130, 130, 129, 130, 130, 130, 130, 130, 130, 130, 130, 130,
    130, 130, 130, 130, 129, 130, 130, 130, 130, 130, 129, 130, 130, 130, 130, 130,
    130, 130,  36,  36,  36,  36,  36,  36,  74,  74,  74, 131,  36, 132,  75,  75,
     75,  75,  75,  75,  75,  75,  36,  36, 133, 133, 133, 133, 133, 133, 133, 133,
     36,  39,  14,  14,  36,  36, 134, 135,  46,  46,  46,  46,  48,  46,  46,  46,
     46,  46,  46,  47,  46,  46,  47,  47,  46, 134,  47,  46,  46,  46,  46,  46,
     14,  36,  36,  36,  36,  36,  36,  36,  36,  39,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  14, 106,  36,  14,  14,  14,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14, 127,  36, 136, 137,  57, 138, 139,  36,  36,  36,
     98,  98, 140, 106, 106, 106, 106, 106, 106, 106, 112, 140, 112,  98,  98,  98,
    112,  77,  91,  53, 140, 106, 106, 112,  98,  98,  98, 125, 141, 142,  36,  36,
     14,  14,  14,  14,  14,  14,  38, 143, 107,  98,   6,  98,  69,  98, 112, 112,
     98,  98,  98,  98,  98,  91,  98, 144,  98,  98,  98,  98,  98, 140, 145,  98,
     98,  98,  98,  98,  98, 140, 145, 140, 115,  69,  93, 119, 126, 126, 126, 126,
    120,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,  91,
     36,  98,  98,  98,  36,  98,  98,  98,  36,  98,  98,  98,  36,  98, 125,  36,
     22,  98, 141, 146,  14,  14,  14,  38,  36,  36,  36,  36,  42,   0, 147,  36,
     14,  14,  14,  14,  14,  14,  39,  14,  14,  14,  14,  14,  14,  38,  14,  39,
     58,  41,  36,  39,  14,  14,  14,  14,  14,  14,  36,  39,  14,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  36,  36,  14,  14,  14,  14,  14,  14,  19,  36,
     14,  14,  36,  36,  36,  36,  39,  14,  14,  14,  14,   0,   0,  52,  36,  36,
     14,  14,  14,  14,  14,  14,  14,  80,  14,  14,  36,  36,  14,  14,  14,  14,
     76,  14,  14,  36,  36,  36,  36,  36,  14,  14,  36,  36,  36,  36,  36,  39,
     14,  14,  14,  36,  38,  14,  14,  14,  14,  14,  14,  39,  38,  36,  38,  39,
     14,  14,  14,  80,  14,  14,  14,  14,  14,  38,  14,  36,  36,  39,  14,  14,
     14,  14,  14,  14,  14,  14,  36,  80,  14,  14,  14,  14,  14,  36,  36,  39,
     14,  14,  14,  14,  36,  36,  14,  14,  19,   0,  42,  52,  36,  36,   0,   0,
     14,  14,  39,  14,  39,  14,  14,  14,  14,  14,  36,  36,   0,  52,  36,  42,
     58,  58,  58,  58,  38,  36,  36,  36,  14,  14,  19,  52,  36,  39,  14,  14,
     58,  58,  58, 148,  36,  36,  36,  36,  14,  14,  14,  36,  80,  58,  58,  58,
     14,  38,  36,  36,  14,  14,  14,  14,  14,  36,  36,  36,  39,  14,  38,  36,
     36,  36,  36,  36,  39,  14,  14,  14,  14,  38,  36,  36,  36,  14,  14,  14,
     14,  14,  14,  14,   0,   0,   0,   0,   0,   0,   0,   1,  76,  14,  14,  36,
     14,  14,  14,  12,  12,  12,  12,  12,  36,  36,  36,  36,  36,  36,  36,  42,
      0,   0,   0,   0,   0,  44,  14,  58,  58,  36,  36,  36,  36,  36,  36,  36,
      0,   0,  52,  12,  12,  12,  12,  12,  58,  58,  36,  36,  36,  36,  36,  36,
     14,  19,  32,  38,  36,  36,  36,  36,  44,  14,  27,  76,  76,   0,  44,  36,
     12,  12,  12,  12,  12,  32,  27,  58,  14,  14,  38,  36,  36,  36,  36,  36,
     14,  14,  14,  14,  14,  14,   0,   0,   0,   0,   0,   0,  58,  27,  76,  52,
     14,  14,  14,  38,  38,  14,  14,  39,  14,  14,  14,  14,  27,  36,  36,  36,
      0,   0,   0,   0,   0,  52,  36,  36,   0,   0,  39,  14,  14,  14,  38,  39,
     38,  36,  36,  42,  36,  36,  39,  14,  14,   0,  36,   0,   0,   0,  52,  36,
      0,   0,  52,  36,  36,  36,  36,  36,  14,  14,  19,   0,   0,   0,   0,   0,
      0,   0,   0,  44,  14,  27,  58,  76,  12,  12,  12,  12,  12,  80,  39,  36,
      0,   0,  14,  14,  36,  36,  36,  36,   0,   0,   0,  36,   0,   0,   0,   0,
    149,  58,  53,  14,  27,  58,  58,  58,  58,  58,  58,  58,  14,  14,   0,  36,
      1,  76,  38,  36,  36,  36,  36,  36,  64,  64,  64,  64,  64,  64, 150,  36,
      0,   0,   0,   0,  36,  36,  36,  36,  60,  60,  60,  60,  60,  36,  59,  60,
     12,  12,  12,  12,  12,  60,  58, 151,  14,  38,  36,  36,  36,  36,  36,  39,
     19,   0,   0,   0,   0,  44,  14,  14,  14,  19,   0,   0,   0,  19,   0, 149,
     27,  58,  70,  19,  36,  36,  36,  36,  19,   0,   0,   0,   0,   0,  14,  14,
     14,  14,  36,  14,  14,   0,   0,   0,   0,   0,   0,   0,   0,  58,  41,  64,
     66,  41,  36,  36,  36,  36,  36,  36,   0,   0,   0,  52,   0,   0,   0,   0,
     27,  58,  58,  36,  36,  36,  36,  36, 152,  14,  14,  14,  14,  14,  14,  14,
     36,   0,   0,   0,   0,   0,   0,   0,  14,  14,  14,  38,  14,  39,  14,  14,
     19,   0,   0,  52,  36,  52,   0,  42,   0,   0,   0,  19,  36,  36,  36,  36,
     58,  58,  41,  36,  36,  36,  36,  36,  14,  14,  36,  36,  36,  36,  36,  36,
     14,  14,  14,  14, 153,  69, 115,  14,  14,  99,  14,  69,  69,  14,  14,  14,
     14,  14,  14,  14,  16, 115,  14,  14,  14,  14,  14,  14,  14,  14,  14,  69,
     12,  12,  12,  12,  12,  36,  36,  58,   0,   0,   1,  36,  36,  36,  36,  36,
      0,   0,   0,   1,  58,  14,  14,  14,  14,  14,  76,  36,  36,  36,  36,  36,
     12,  12,  12,  12,  12,  39,  14,  14,  14,  14,  14,  14,  36,  36,  39,  14,
     19,   0,   0,   0,   0,   0,   0,   0,  91,  36,  36,  36,  36,  36,  36,  36,
     98, 125,  36,  36,  36,  36,  36,  36,  14,  14,  14,  14,  14,  36,  19,   1,
      0,   0,  36,  36,  36,  36,  36,  36,  14,  14,  19,   0,   0,  14,  19,   0,
      0,  44,  19,   0,   0,   0,  14,  14,  14,  14,  14,  14,  14,   0,   0,  14,
     14,   0,  44,  36,  36,  36,  36,  36,  36,  38,  39,  38,  39,  14,  38,  14,
     14,  14,  14,  14,  14,  39,  39,  14,  14,  14,  39,  14,  14,  14,  14,  14,
     14,  14,  14,  39,  14,  38,  39,  14,  14,  14,  38,  14,  14,  14,  38,  14,
     14,  14,  14,  14,  14,  39,  14,  38,  14,  14,  38,  38,  36,  14,  14,  14,
     14,  14,  14,  14,  14,  14,  36,  12,  12,  12,  12,  12,  12,  12,  12,  12,
      0,   0,   0,  44,  14,  19,   0,   0,   0,   0,   0,   0,   0,   0,  44,  14,
     14,  14,  19,  14,  14,  14,  14,  14,  14,  14,  44,  27,  58,  76,  36,  36,
     36,  36,  36,  36,  36,  42,   0,   0,   0,   0,   0,   0,  52,  42,   0,   0,
      0,  42,  52,   0,   0,  52,  36,  36,  14,  14,  38,  39,  14,  14,  14,  14,
     14,  14,   0,   0,   0,  52,  36,  36,  12,  12,  12,  12,  12,  36,  36, 153,
     39,  38,  38,  39,  39,  14,  14,  14,  14,  38,  14,  14,  39,  39,  36,  36,
     36,  38,  36,  39,  39,  39,  39,  14,  39,  38,  38,  39,  39,  39,  39,  39,
     39,  38,  38,  39,  14,  38,  14,  14,  14,  38,  14,  14,  39,  14,  38,  38,
     14,  14,  14,  14,  14,  39,  14,  14,  39,  14,  39,  14,  14,  39,  14,  14,
     28,  28,  28,  28,  28,  28, 104,  98,  28,  28,  28,  28,  28,  28,  28, 102,
     28,  28,  28,  28,  28,  14,  98,  98,  98,  98,  98, 154, 154, 154, 154, 154,
    154, 154, 154, 154, 154, 154, 154, 154,  98,  98, 101,  98,  98,  98,  98,  98,
     98,  98,  98,  98,  98,  98,  14,  98,  98,  98, 100, 102,  98,  98, 102,  98,
     98, 105, 155, 101,  98, 105, 155,  98,  98,  98,  98,  98,  98, 156, 157, 157,
     98, 105,  98, 105, 105, 105, 105, 105, 155,  98,  98,  98,  98,  98,  98,  98,
     98,  98,  98, 105, 105,  98,  98, 155, 105, 105, 105, 105, 155,  98, 155,  98,
    101, 105, 101, 105,  98,  98,  98,  98, 102, 102, 102,  98,  98, 155,  98, 100,
    100, 102,  98,  98,  98,  98,  98,  98,  14,  14,  14, 102,  98,  98,  98,  98,
     98,  98,  98, 100,  14,  14,  14,  14,  14,  14, 102,  98,  98,  98,  98,  98,
     98,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,  98,  98,  98,
     98,  98, 105,  98,  98, 155,  98,  98, 155,  98, 101, 155,  98,  98,  98,  98,
     98,  98,  14,  14,  14,  14,  98,  98,  98,  98,  14,  14,  14,  98,  98,  98,
     98,  98, 101, 105,  98, 101, 105, 105,  14,  14,  14,  85, 158,  91,  14,  14,
     98, 101,  98,  98,  98,  98,  98,  98,  98,  98, 105, 155,  98,  98,  98,  98,
    155,  98,  98,  98,  98,  98, 155,  98,  14,  14,  98,  98,  98,  98,  98,  98,
     14,  14,  14,  14,  14,  14,  98,  98,  14,  14,  14,  14,  98,  98,  98,  98,
     14,  14,  14,  14,  14,  14,  14,  98,  98,  98,  98,  98, 105, 105, 155, 105,
     98,  98,  98, 155,  98,  98,  98,  98, 105, 105, 105, 105, 105,  98, 101, 155,
    101, 105, 105, 105, 105, 105, 105,  98,  98,  98,  98,  98,  98,  98,  98,  36,
     42,  36,  36,  36,  36,  36,  36,  36,
};

static RE_UINT8 re_line_break_stage_5[] = {
    16, 16, 16, 18, 22, 20, 20, 21, 19,  6,  3, 12,  9, 10, 12,  3,
     1, 36, 12,  9,  8, 15,  8,  7, 11, 11,  8,  8, 12, 12, 12,  6,
    12,  1,  9, 36, 18,  2, 12, 16, 16, 29,  4,  1, 10,  9,  9,  9,
    12, 25, 25, 12, 25,  3, 12, 18, 25, 25, 17, 12, 25,  1, 17, 25,
    12, 17, 16,  4,  4,  4,  4, 16,  0,  0,  8, 12, 12,  0,  0, 12,
     0,  8, 18,  0,  0, 16, 18, 16, 16, 12,  6, 16, 37, 37, 37,  0,
    37, 12, 12, 10, 10, 10, 16,  6, 16,  0,  6,  6, 10, 11, 11, 12,
     6, 12,  8,  6, 18, 18,  0, 24, 24, 24, 24,  0,  0,  9, 24, 12,
    17, 17,  4, 17, 17, 18,  4,  6,  4, 12,  1,  2, 18, 17, 12,  4,
     4,  0, 31, 31, 32, 32, 33, 33, 18, 12,  2,  0,  5, 24, 18,  9,
     0, 18, 18,  4, 18, 28, 16, 42, 26, 25,  3,  3,  1,  3, 14, 14,
    14, 18, 20, 20,  3, 25,  5,  5,  8,  1,  2,  5, 30, 12,  2, 25,
     9, 12, 12, 14, 13, 13,  2, 12, 13, 12, 13, 40, 12, 13, 13, 25,
    25, 13, 40, 40,  2,  1,  0,  6,  6, 18,  1, 18, 26, 26,  0, 13,
     2, 13, 13,  5,  5,  1,  2,  2, 13, 16,  5, 13,  0, 38, 13, 38,
    38, 13, 38,  0, 16,  5,  5, 38, 38,  5, 13,  0, 38, 38, 10, 12,
    31,  0, 34, 35, 35, 35, 32,  0,  0, 33, 27, 27,  0, 37, 16, 37,
     8,  2,  2,  8,  6,  1,  2, 14, 13,  1, 13,  9, 10, 13,  0, 30,
    13,  6, 13,  2,  9,  0, 23, 25, 14,  0, 16, 17, 17,  0, 18, 24,
    17,  6,  1,  1, 39, 39, 40, 13, 13, 41, 41, 41,  3,  5,
};

/* Line_Break: 9110 bytes. */

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
    13, 14, 15, 11, 11, 11, 16, 11, 11, 11, 11, 11, 11, 17, 18, 19,
    20, 11, 21, 22, 11, 11, 23, 11, 11, 11, 11, 11, 11, 11, 11, 24,
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
     1, 56, 57, 58, 59,  1,  1,  1, 60, 61, 62, 63, 64,  1, 65,  1,
    66, 67, 54,  1,  9,  1, 68, 69, 70,  1,  1,  1, 71, 53,  1,  1,
     1,  1,  1,  1, 72,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 73, 74,  1,  1,  1,  1,
     1,  1,  1, 75,  1,  1,  1, 76,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 77, 53,  1,  1,  1,  1,  1,  1,
     1, 78,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    79, 80,  1,  1,  1,  1,  1,  1,  1, 81, 82, 83,  1,  1,  1,  1,
     1,  1,  1, 84,  1,  1,  1,  1,  1, 85,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 86,  1,  1,  1,  1,
     1,  1, 87,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 84,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_numeric_type_stage_3[] = {
      0,   1,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   3,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   3,   0,
      0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   4,   0,   0,   0,   4,
      0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   4,
      0,   0,   9,  10,   0,   0,   0,   4,   0,   0,   1,   0,   0,   0,   1,   0,
      0,  11,   0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   1,   0,   0,   0,
      0,   0,   0,  12,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  13,
      0,   0,   0,   0,   0,   0,   0,  14,   1,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   4,   0,   0,   0,  15,   0,   0,   0,   0,   0,  16,   0,   0,   0,
      0,   0,   1,   0,   0,   1,   0,   0,   0,   0,  16,   0,   0,   0,   0,   0,
      0,   0,   0,  17,  18,   0,   0,   0,   0,   0,  19,  20,  21,   0,   0,   0,
      0,   0,   0,  22,  23,   0,   0,  24,   0,   0,   0,  25,  26,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  27,  28,  29,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  30,   0,   0,   0,   0,  31,  32,   0,  31,  33,   0,   0,
     34,   0,   0,   0,  35,   0,   0,   0,   0,  36,   0,   0,   0,   0,   0,   0,
      0,   0,  37,   0,   0,   0,   0,   0,  38,   0,  27,   0,  39,  40,  41,  42,
     37,   0,   0,  43,   0,   0,   0,   0,  44,   0,  45,  46,   0,   0,   0,   0,
      0,   0,  47,   0,   0,   0,  48,   0,   0,   0,   0,   0,   0,   0,  49,   0,
      0,   0,   0,   0,   0,   0,   0,  50,   0,   0,   0,  51,   0,   0,   0,  52,
     53,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  54,
      0,   0,  55,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  56,   0,
     45,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  57,   0,   0,   0,
      0,   0,   0,  54,   0,   0,   0,   0,   0,   0,   0,   0,  45,   0,   0,   0,
      0,  55,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  58,   0,   0,
      0,  43,   0,   0,   0,   0,   0,   0,   0,  59,  60,  61,   0,   0,   0,  57,
      0,   3,   0,   0,   0,   0,   0,  62,   0,  63,   0,   0,   0,   0,   1,   0,
      3,   0,   0,   0,   0,   0,   1,   1,   0,   0,   1,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,  64,   0,  56,  65,  27,
     66,  67,  20,  68,  69,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  70,
      0,  71,  72,   0,   0,   0,  73,   0,   0,   0,   0,   0,   0,   3,   0,   0,
      0,   0,  74,  75,   0,  76,   0,  77,  78,   0,   0,   0,   0,  79,  80,  20,
      0,   0,  81,  82,  83,   0,   0,  84,   0,   0,  74,  74,   0,  85,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  86,   0,   0,   0,  87,   0,   0,   0,   0,
      0,   0,  88,  89,   0,   0,   0,   1,   0,  90,   0,   0,   0,   0,   1,  91,
      0,   0,   1,   0,   0,   0,   3,   0,   0,  92,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  93,   0,   0,  94,  95,   0,   0,   0,   0,
     20,  20,  20,  96,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,
      0,   0,  97,  98,   0,   0,   0,   0,   0,   0,   0,  99,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 100, 101,   0,   0,   0,   0,   0,   0,  76,   0,
    102,   0,   0,   0,   0,   0,   0,   0,  59,   0,   0,  44,   0,   0,   0, 103,
      0,  59,   0,   0,   0,   0,   0,   0,   0,  36,   0,   0, 104,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 105, 106,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  43,   0,   0,   0,   0,   0,   0,   0,  61,   0,   0,   0,
     49,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  37,   0,   0,   0,   0,
};

static RE_UINT8 re_numeric_type_stage_4[] = {
     0,  0,  0,  0,  0,  0,  1,  2,  0,  0,  3,  4,  1,  2,  0,  0,
     5,  1,  0,  0,  5,  1,  6,  7,  5,  1,  8,  0,  5,  1,  9,  0,
     5,  1,  0, 10,  0,  0,  0, 10,  5,  1, 11, 12,  1, 13, 14,  0,
     0, 15, 16, 17,  0, 18, 12,  0,  1,  2, 11,  7,  0,  0,  1, 19,
     1,  2,  1,  2,  0,  0, 20, 21, 22, 21,  0,  0,  0,  0, 11, 11,
    11, 11, 11, 11, 23,  7,  0,  0, 22, 24, 25, 26, 11, 22, 24, 14,
     0, 27, 28, 29,  0,  0, 30, 31, 22, 32, 33,  0,  0,  0,  0, 34,
    35,  0,  0,  0, 36,  7,  0,  9,  0,  0, 37,  0, 11,  7,  0,  0,
     0, 11, 36, 11,  0,  0, 36, 11, 34,  0,  0,  0, 38,  0,  0,  0,
     0, 39,  0,  0,  0, 34,  0,  0, 40, 41,  0,  0,  0, 42, 43,  0,
     0,  0,  0, 35, 12,  0,  0, 35,  0, 12,  0,  0,  0,  0, 12,  0,
    42,  0,  0,  0, 44,  0,  0,  0,  0, 45,  0,  0, 46, 42,  0,  0,
    47,  0,  0,  0,  0,  0,  0, 38,  0,  0, 41, 41,  0,  0,  0, 39,
     0,  0,  0, 18,  0, 48, 12,  0,  0,  0,  0, 44,  0, 42,  0,  0,
     0,  0, 39,  0,  0,  0, 44,  0,  0, 44, 38,  0, 41,  0,  0,  0,
    44, 42,  0,  0,  0,  0,  0, 12, 18, 11,  0,  0,  0,  0, 49,  0,
     0, 38, 38, 12,  0,  0, 50,  0, 35, 11, 11, 11, 11, 11, 14,  0,
    11, 11, 11, 12,  0, 51,  0,  0, 36, 11, 11, 14, 14,  0,  0,  0,
    41, 39,  0,  0,  0,  0, 52,  0,  0,  0,  0, 11,  0,  0,  0, 36,
    35, 11,  0,  0,  0,  0,  0, 53,  0,  0, 18, 14,  0,  0,  0, 54,
    11, 11,  8, 11, 55,  0,  0,  0,  0,  0,  0, 56,  0,  0,  0, 57,
     0, 53,  0,  0,  0, 36,  0,  0,  0,  0,  0,  8, 22, 24, 11, 10,
     0,  0, 58, 59, 60,  1,  0,  0,  0,  0,  5,  1, 36, 11, 17,  0,
     0,  0,  1, 61,  1, 13,  9,  0,  0,  0,  1, 13, 11, 17,  0,  0,
    11, 10,  0,  0,  0,  0,  1, 62,  7,  0,  0,  0, 11, 11,  7,  0,
     0,  5,  1,  1,  1,  1,  1,  1, 22, 63,  0,  0, 39,  0,  0,  0,
    38, 42,  0, 42,  0, 39,  0, 34,  0,  0,  0, 41,
};

static RE_UINT8 re_numeric_type_stage_5[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0,
    0, 2, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 3,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 3, 3, 2, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0, 1, 1, 1,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2,
    0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0,
    0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 1, 1, 0, 0, 0, 0,
    3, 3, 0, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 0, 0, 0,
};

/* Numeric_Type: 2316 bytes. */

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
    13, 14, 15, 11, 11, 11, 16, 11, 11, 11, 11, 11, 11, 17, 18, 19,
    20, 11, 21, 22, 11, 11, 23, 11, 11, 11, 11, 11, 11, 11, 11, 24,
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
     1, 56, 57, 58, 59,  1,  1,  1, 60, 61, 62, 63, 64,  1, 65,  1,
    66, 67, 54,  1,  9,  1, 68, 69, 70,  1,  1,  1, 71, 53,  1,  1,
     1,  1,  1,  1, 72,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 73, 74,  1,  1,  1,  1,
     1,  1,  1, 75,  1,  1,  1, 76,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 77, 53,  1,  1,  1,  1,  1,  1,
     1, 78,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    79, 80,  1,  1,  1,  1,  1,  1,  1, 81, 82, 83,  1,  1,  1,  1,
     1,  1,  1, 84,  1,  1,  1,  1,  1, 85,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 86,  1,  1,  1,  1,
     1,  1, 87,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 88,  1,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_numeric_value_stage_3[] = {
      0,   1,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   3,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   3,   0,
      0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   4,   0,   0,   0,   4,
      0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   4,
      0,   0,   9,  10,   0,   0,   0,   4,   0,   0,   1,   0,   0,   0,   1,   0,
      0,  11,   0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   1,   0,   0,   0,
      0,   0,   0,  12,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  13,
      0,   0,   0,   0,   0,   0,   0,  14,   1,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   4,   0,   0,   0,  15,   0,   0,   0,   0,   0,  14,   0,   0,   0,
      0,   0,   1,   0,   0,   1,   0,   0,   0,   0,  14,   0,   0,   0,   0,   0,
      0,   0,   0,  16,   3,   0,   0,   0,   0,   0,  17,  18,  19,   0,   0,   0,
      0,   0,   0,  20,  21,   0,   0,  22,   0,   0,   0,  23,  24,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  25,  26,  27,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  28,   0,   0,   0,   0,  29,  30,   0,  29,  31,   0,   0,
     32,   0,   0,   0,  33,   0,   0,   0,   0,  34,   0,   0,   0,   0,   0,   0,
      0,   0,  35,   0,   0,   0,   0,   0,  36,   0,  37,   0,  38,  39,  40,  41,
     42,   0,   0,  43,   0,   0,   0,   0,  44,   0,  45,  46,   0,   0,   0,   0,
      0,   0,  47,   0,   0,   0,  48,   0,   0,   0,   0,   0,   0,   0,  49,   0,
      0,   0,   0,   0,   0,   0,   0,  50,   0,   0,   0,  51,   0,   0,   0,  52,
     53,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  54,
      0,   0,  55,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  56,   0,
     57,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  58,   0,   0,   0,
      0,   0,   0,  59,   0,   0,   0,   0,   0,   0,   0,   0,  60,   0,   0,   0,
      0,  61,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  62,   0,   0,
      0,  63,   0,   0,   0,   0,   0,   0,   0,  64,  65,  66,   0,   0,   0,  67,
      0,   3,   0,   0,   0,   0,   0,  68,   0,  69,   0,   0,   0,   0,   1,   0,
      3,   0,   0,   0,   0,   0,   1,   1,   0,   0,   1,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,  70,   0,  71,  72,  73,
     74,  75,  76,  77,  78,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  79,
      0,  80,  81,   0,   0,   0,  82,   0,   0,   0,   0,   0,   0,   3,   0,   0,
      0,   0,  83,  84,   0,  85,   0,  86,  87,   0,   0,   0,   0,  88,  89,  90,
      0,   0,  91,  92,  93,   0,   0,  94,   0,   0,  95,  95,   0,  96,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  97,   0,   0,   0,  98,   0,   0,   0,   0,
      0,   0,  99, 100,   0,   0,   0,   1,   0, 101,   0,   0,   0,   0,   1, 102,
      0,   0,   1,   0,   0,   0,   3,   0,   0, 103,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 104,   0,   0, 105, 106,   0,   0,   0,   0,
    107, 108, 109, 110,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,
      0,   0, 111, 112,   0,   0,   0,   0,   0,   0,   0, 113,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 114, 115,   0,   0,   0,   0,   0,   0, 116,   0,
    117,   0,   0,   0,   0,   0,   0,   0, 118,   0,   0, 119,   0,   0,   0, 120,
      0, 121,   0,   0,   0,   0,   0,   0,   0, 122,   0,   0, 123,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 124, 125,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  63,   0,   0,   0,   0,   0,   0,   0, 126,   0,   0,   0,
    127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,   0,   0,   0,
      0,   0,   0,   0, 129,   0,   0,   0,
};

static RE_UINT8 re_numeric_value_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   3,   0,
      0,   0,   0,   0,   4,   0,   5,   6,   1,   2,   3,   0,   0,   0,   0,   0,
      0,   7,   8,   9,   0,   0,   0,   0,   0,   7,   8,   9,   0,  10,  11,   0,
      0,   7,   8,   9,  12,  13,   0,   0,   0,   7,   8,   9,  14,   0,   0,   0,
      0,   7,   8,   9,   0,   0,   1,  15,   0,   0,   0,   0,   0,   0,  16,  17,
      0,   7,   8,   9,  18,  19,  20,   0,   1,   2,  21,  22,  23,   0,   0,   0,
      0,   0,  24,   2,  25,  26,  27,  28,   0,   0,   0,  29,  30,   0,   0,   0,
      1,   2,   3,   0,   1,   2,   3,   0,   0,   0,   0,   0,   1,   2,  31,   0,
      0,   0,   0,   0,  32,   2,   3,   0,   0,   0,   0,   0,  33,  34,  35,  36,
     37,  38,  39,  40,  37,  38,  39,  40,  41,  42,  43,   0,   0,   0,   0,   0,
     37,  38,  39,  44,  45,  37,  38,  39,  44,  45,  37,  38,  39,  44,  45,   0,
      0,   0,  46,  47,  48,  49,   2,  50,   0,   0,   0,   0,   0,  51,  52,  53,
     37,  38,  54,  52,  53,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  55,
      0,  56,   0,   0,   0,   0,   0,   0,  24,   2,   3,   0,   0,   0,  57,   0,
      0,   0,   0,   0,  51,  58,   0,   0,  37,  38,  59,   0,   0,   0,   0,   0,
      0,   0,  60,  61,  62,  63,  64,  65,   0,   0,   0,   0,  66,  67,  68,  69,
      0,  70,   0,   0,   0,   0,   0,   0,  71,   0,   0,   0,   0,   0,   0,   0,
      0,   0,  72,   0,   0,   0,   0,   0,   0,   0,   0,  73,   0,   0,   0,   0,
     74,  75,  76,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  77,
      0,   0,   0,  78,   0,  79,   0,   0,   0,   0,   0,   0,   0,   0,   0,  80,
     81,   0,   0,   0,   0,   0,   0,  82,   0,   0,  83,   0,   0,   0,   0,   0,
      0,   0,   0,  70,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,
      0,  85,   0,   0,   0,   0,   0,   0,   0,  86,   0,   0,   0,   0,   0,   0,
      0,   0,  87,  88,   0,   0,   0,   0,  89,  90,   0,  91,   0,   0,   0,   0,
     92,  83,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  93,   0,
      0,   0,   0,   0,   5,   0,   5,   0,   0,   0,   0,   0,   0,   0,  94,   0,
      0,   0,   0,   0,   0,   0,   0,  95,   0,   0,   0,  15,  78,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  96,   0,   0,   0,  97,   0,   0,   0,   0,
      0,   0,   0,   0,  98,   0,   0,   0,   0,  98,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0, 100,
      0, 101,   0,   0,   0,   0,   0,   0,   0,   0,   0,  28,   0,   0,   0,   0,
      0,   0,   0, 102,  71,   0,   0,   0,   0,   0,   0,   0,  78,   0,   0,   0,
    103,   0,   0,   0,   0,   0,   0,   0,   0, 104,   0,  84,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 105,   0,   0,   0,   0,   0,   0, 106,   0,   0,
      0,  51,  52, 107,   0,   0,   0,   0,   0,   0,   0,   0, 108, 109,   0,   0,
      0,   0, 110,   0, 111,   0,  78,   0,   0,   0,   0,   0, 106,   0,   0,   0,
      0,   0,   0,   0, 112,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 113,
      0, 114,   8,   9,  60,  61, 115, 116, 117, 118, 119, 120, 121,   0,   0,   0,
    122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 125, 134, 135,   0,
      0,   0, 136,   0,   0,   0,   0,   0,  24,   2,  25,  26,  27, 137, 138,   0,
    139,   0,   0,   0,   0,   0,   0,   0, 140,   0, 141,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 142, 143,   0,   0,   0,   0,   0,   0,   0,   0, 144, 145,
      0,   0,   0,   0,   0,   0,  24, 146,   0, 114, 147, 148,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 114, 148,   0,   0,   0,   0,   0, 149, 150,   0,
      0,   0,   0,   0,   0,   0,   0, 151,  37,  38, 152, 153, 154, 155, 156, 157,
    158, 159, 160, 161, 162, 163, 164, 165,  37, 166,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0, 167,   0,   0,   0,   0,   0,   0,   0, 168,
      0,   0, 114, 148,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  37, 166,
      0,   0,  24, 169,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 170, 171,
     37,  38, 152, 153, 172, 155, 173, 174,   0,   0,   0,   0,  51,  52,  53, 175,
    176, 177,   8,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   7,   8,   9,
     24,   2,  25,  26,  27, 178,   0,   0,   0,   0,   0,   0,   1,   2,  25,   0,
      1,   2,  25,  26, 179,   0,   0,   0,   0,   0,   0,   0,   1,   2, 180,  52,
     53, 175, 176,  84,   0,   0,   0,   0,   8,   9,  52, 181,  38, 182,   2, 180,
    183, 184,   9, 185, 186, 185, 187, 188, 189, 190, 191, 192, 147, 193, 194, 195,
    196, 197, 198, 199,   0,   0,   0,   0,   0,   0,   0,   0,   1,   2, 200, 201,
    202,   0,   0,   0,   0,   0,   0,   0,  37,  38, 152, 153, 203,   0,   0,   0,
      0,   0,   0,   7,   8,   9,   1,   2, 204,   8,   9,   1,   2, 204,   8,   9,
      0, 114,   8,   9,   0,   0,   0,   0, 205,  52, 107,  32,   0,   0,   0,   0,
     73,   0,   0,   0,   0,   0,   0,   0,   0, 206,   0,   0,   0,   0,   0,   0,
    101,   0,   0,   0,   0,   0,   0,   0,  70,   0,   0,   0,   0,   0,   0,   0,
      0,   0,  94,   0,   0,   0,   0,   0, 207,   0,   0,  91,   0,   0,   0,  91,
      0,   0, 104,   0,   0,   0,   0,  76,   0,   0,   0,   0,   0,   0,  76,   0,
      0,   0,   0,   0,   0,   0,  83,   0,   0,   0,   0,   0,   0,   0, 110,   0,
      0,   0,   0, 208,   0,   0,   0,   0,   0,   0,   0,   0, 209,   0,   0,   0,
};

static RE_UINT8 re_numeric_value_stage_5[] = {
      0,   0,   0,   0,   2,  32,  34,  36,  38,  40,  42,  44,  46,  48,   0,   0,
      0,   0,  34,  36,   0,  32,   0,   0,  17,  22,  27,   0,   0,   0,   2,  32,
     34,  36,  38,  40,  42,  44,  46,  48,   7,  11,  15,  17,  27,  55,   0,   0,
      0,   0,  17,  22,  27,   7,  11,  15,  49,  94, 103,   0,  32,  34,  36,   0,
      3,   4,   5,   6,   9,  13,  16,   0,  49,  94, 103,  17,  22,  27,   7,  11,
     15,   0,   0,   0,  46,  48,  22,  33,  35,  37,  39,  41,  43,  45,  47,   1,
      0,  32,  34,  36,  46,  48,  49,  59,  69,  79,  89,  90,  91,  92,  93,  94,
    112,   0,   0,   0,   0,   0,  56,  57,  58,   0,   0,   0,  46,  48,  32,   0,
      2,   0,   0,   0,  12,  10,   9,  18,  26,  16,  20,  24,  28,  14,  29,  11,
     19,  25,  30,  32,  32,  34,  36,  38,  40,  42,  44,  46,  48,  49,  50,  51,
     89,  94,  98, 103, 103, 107, 112,   0,   0,  42,  89, 116, 121,   2,   0,   0,
     52,  53,  54,  55,  56,  57,  58,  59,   0,   0,   2,  50,  51,  52,  53,  54,
     55,  56,  57,  58,  59,  32,  34,  36,  46,  48,  49,   2,   0,   0,  32,  34,
     36,  38,  40,  42,  44,  46,  48,  49,  48,  49,  32,  34,   0,  22,   0,   0,
      0,   0,   0,   2,  49,  59,  69,   0,  36,  38,   0,   0,  48,  49,   0,   0,
     49,  59,  69,  79,  89,  90,  91,  92,   0,  60,  61,  62,  63,  64,  65,  66,
     67,  68,  69,  70,  71,  72,  73,  74,   0,  75,  76,  77,  78,  79,  80,  81,
     82,  83,  84,  85,  86,  87,  88,  89,   0,  40,   0,   0,   0,   0,   0,  34,
      0,   0,  40,   0,   0,  44,   0,   0,  32,   0,   0,  44,   0,   0,   0, 112,
      0,  36,   0,   0,   0,  48,   0,   0,  34,   0,   0,   0,  40,   0,  38,   0,
      0,   0,   0, 133,  49,   0,   0,   0,   0,   0,   0, 103,  36,   0,   0,   0,
     94,   0,   0,   0, 133,   0,   0,   0,   0,   0, 135,   0,   0,  34,   0,  46,
      0,  42,   0,   0,   0,  49,   0, 103,  59,  69,   0,   0,  79,   0,   0,   0,
      0,  36,  36,  36,   0,   0,   0,  38,   0,   0,  32,   0,   0,   0,  48,  59,
      0,   0,  49,   0,  46,   0,   0,   0,   0,   0,  44,   0,   0,   0,  48,   0,
      0,   0,  94,   0,   0,   0,  38,   0,   0,   0,  34,   0,   0, 103,   0,   0,
      0,   0,  42,   0,  42,   0,   0,   0,   0,   0,   2,   0,  44,  46,  48,   2,
     17,  22,  27,   7,  11,  15,   0,   0,   0,   0,   0,  36,   0,   0,   0,  49,
      0,  42,   0,  42,   0,  49,   0,   0,   0,   0,   0,  32,  93,  94,  95,  96,
     97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
    113, 114, 115, 116, 117, 118, 119, 120,  17,  22,  32,  40,  89,  98, 107, 116,
     40,  49,  89,  94,  98, 103, 107,  40,  49,  89,  94,  98, 103, 112, 116,  49,
     32,  32,  32,  34,  34,  34,  34,  40,  49,  49,  49,  49,  49,  69,  89,  89,
     89,  89,  94,  96,  98,  98,  98,  98,  89,  22,  22,  26,  27,   0,   0,   0,
      0,   0,   2,  17,  95,  96,  97,  98,  99, 100, 101, 102,  32,  40,  49,  89,
      0,  93,   0,   0,   0,   0, 102,   0,   0,  32,  34,  49,  59,  94,   0,   0,
     32,  34,  36,  49,  59,  94, 103, 112,  38,  40,  49,  59,  34,  36,  38,  38,
     40,  49,  59,  94,   0,   0,  32,  49,  59,  94,  34,  36,  31,  22,   0,   0,
     48,  49,  59,  69,  79,  89,  90,  91,   0,   0,  94,  95,  96,  97,  98,  99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
    116, 117, 118, 119, 120, 121, 122, 124, 125, 127, 128, 129, 130, 131,   8,  14,
     17,  18,  21,  22,  23,  26,  27,  29,  49,  59,  94, 103,   0,  32,  89,   0,
      0,  32,  49,  59,  38,  49,  59,  94,   0,   0,  32,  40,  49,  89,  94, 103,
     92,  93,  94,  95, 100, 101, 102,  22,  17,  18,  26,   0,  59,  69,  79,  89,
     90,  91,  92,  93,  94, 103,   2,  32, 103,   0,   0,   0,  91,  92,  93,   0,
     46,  48,  32,  34,  44,  46,  48,  38,  48,  32,  34,  36,  36,  38,  40,  34,
     36,  36,  38,  40,  32,  34,  36,  36,  38,  40, 123, 126,  38,  40,  36,  36,
     38,  38,  38,  38,  42,  44,  44,  44,  46,  46,  48,  48,  48,  48,  34,  36,
     38,  40,  42,  32,  40,  40,  34,  36,  32,  34,  18,  26,  29,  18,  26,  11,
     17,  14,  17,  17,  22,  18,  26,  79,  89,  38,  40,  42,  44,  46,  48,   0,
     46,  48,   0,  49,  94, 112, 132, 133, 134, 135,   0,   0,  92,  93,   0,   0,
     46,  48,   2,  32,   2,   2,  32,  34,  38,   0,   0,   0,   0,   0,   0,  69,
      0,  38,   0,   0,  48,   0,   0,   0,
};

/* Numeric_Value: 3264 bytes. */

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

/* Indic_Positional_Category. */

static RE_UINT8 re_indic_positional_category_stage_1[] = {
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

static RE_UINT8 re_indic_positional_category_stage_2[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,
     8,  0,  0,  0,  0,  0,  0,  9,  0, 10, 11, 12, 13, 14,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 15, 16, 17, 18,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19,  0,  0,  0,  0,  0,
    20, 21, 22, 23, 24, 25, 26, 27,  0,  0, 28,  0, 29, 30,  0,  0,
};

static RE_UINT8 re_indic_positional_category_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      1,   0,   0,   2,   3,   4,   5,   0,   6,   0,   0,   7,   8,   9,   5,   0,
     10,   0,   0,   7,  11,   0,   0,  12,  10,   0,   0,   7,  13,   0,   5,  14,
      6,   0,   0,  15,  16,  17,   5,   0,  18,   0,   0,  19,  20,   9,   0,   0,
     21,   0,   0,  22,  23,  24,   5,   0,   6,   0,   0,  15,  25,  26,   5,   0,
     27,   0,   0,  28,  29,   9,   5,   0,  30,   0,   0,   0,  31,  32,   0,  30,
      0,   0,   0,  33,  34,   0,   0,   0,   0,   0,   0,  35,  36,   0,   0,   0,
      0,  37,   0,  38,   0,   0,   0,  39,  40,  41,  42,  43,  44,   0,   0,   0,
      0,   0,  45,  46,   0,  47,  48,  49,  50,  51,   0,   0,   0,   0,   0,   0,
      0,  52,   0,  52,   0,  53,   0,  53,   0,   0,   0,  54,  55,  56,   0,   0,
      0,   0,  57,  58,   0,   0,   0,   0,   0,   0,   0,  59,  60,   0,   0,   0,
      0,  61,   0,   0,   0,  62,  63,  64,   0,   0,   0,   0,   0,   0,   0,   0,
     65,   0,   0,  66,  67,   0,  68,  69,  70,   0,  71,   0,   0,   0,  72,  27,
      0,   0,  73,  74,   0,   0,   0,   0,   0,   0,   0,   0,   0,  75,  76,  77,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  78,
     79,   0,  80,   0,   0,   0,   0,   0,  81,   0,   0,  82,  83,   0,  84,  85,
      0,   0,  86,   0,  87,  27,   0,   0,   1,   0,   0,  88,  89,   0,  90,   0,
      0,   0,  91,  92,  93,   0,   0,  94,   0,   0,   0,  95,  96,   0,  97,  98,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99,   0,
    100,   0,   0, 101,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    102,   0,   0, 103, 104,   0,   0,   0,  70,   0,   0, 105,   0,   0,   0,   0,
    106,   0, 107, 108,   0,   0,   0, 109,  70,   0,   0, 110, 111,   0,   0,   0,
      0,   0, 112, 113,   0,   0,   0,   0,   0,   0,   0,   0,   0, 114, 115,   0,
      6,   0,   0,  19, 116,   9, 117, 118,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 119, 120,   0,   0,   0,   0,   0,   0, 121, 122,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 123, 124, 125, 126,   0,   0,
      0,   0,   0, 127, 128,   0,   0,   0,   0,   0, 129, 130,   0,   0,   0,   0,
      0, 131, 132,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    133,   0,   0, 134,   0, 135,   0,   0, 136, 137,   0,   0,   0,   0,   0,   0,
      0,   0, 123, 138,   0,   0,   0,   0,   0, 139, 140, 141,   0,   0,   0,   0,
      0,   0,   0, 142, 143,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_indic_positional_category_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  2,  3,  4,  5,  6,  7,  1,  2,  8,  5,  9,
    10,  7,  1,  6,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,
    10,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  4,
     5,  6,  3, 11, 12, 13, 14,  0,  0,  0,  0, 15,  0,  0,  0,  0,
    10,  2,  0,  0,  0,  0,  0,  0,  5,  3,  0, 10, 16, 10, 17,  0,
     1,  0, 18,  0,  0,  0,  0,  0,  5,  6,  7, 10, 19, 15,  5,  0,
     0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  3, 20,
     5,  6,  3, 11, 21, 13, 22,  0,  0,  0,  0, 19,  0,  0,  0,  0,
     0, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,
     2, 23,  0, 24, 12, 25, 26,  0,  2,  8,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  1,  2,  8, 23,  1, 27,  1,  1,  0,
     0,  0, 10,  3,  0,  0,  0,  0, 28,  8, 23, 19, 29, 30,  1,  0,
     0,  0, 15, 23,  0,  0,  0,  0,  1,  8,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 10, 16,  8,  8,  5,  3, 24, 12, 25, 26,  0,
     0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 16,  0, 15,
     8,  1,  3,  3,  4, 31, 32, 33, 20,  8,  1,  1,  6,  3,  0,  0,
    34, 34, 35, 10,  1,  1,  1, 16, 20,  8,  1,  1,  6, 10,  3,  0,
    34, 34, 36,  0,  1,  1,  1,  0,  0,  0,  0,  0,  6,  0,  0,  0,
     0,  0, 18, 18, 10,  0,  0,  4, 18, 37,  6, 38, 38,  1,  1,  2,
    37,  1,  3,  1,  0,  0, 18,  6,  6,  6,  6,  6, 18,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  3,  0,
     0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15, 20, 17,
    39,  1,  1, 17, 23,  2, 18,  3,  0,  0,  0,  8,  6,  0,  0,  6,
     3,  8, 23, 15,  8,  8,  8,  0, 10,  1, 16,  0,  0,  0,  0,  0,
     0, 40, 41,  2,  8,  8,  5, 15,  0,  0,  0,  0,  0,  8, 20,  0,
     0, 17,  3,  0,  0,  0,  0,  0,  0, 17,  0,  0,  0,  0,  0,  0,
     0,  0,  0, 20,  1, 17,  6, 42, 43, 24, 25,  2, 20,  1,  1,  1,
     1, 10,  0,  0,  0,  0, 10,  0,  1, 40, 44, 45,  2,  8,  0,  0,
     8, 40,  8,  8,  5, 17,  0,  0,  8,  8, 46, 34,  8, 35,  8,  8,
    23,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0, 10, 39, 20,  0,  0,
     0,  0, 11, 40,  1, 17,  6,  3, 15,  2, 20,  1, 17,  7, 40, 24,
    24, 41,  1,  1,  1,  1, 16, 18,  1,  1, 23,  0,  0,  0,  0,  0,
     0,  0,  2,  1,  6, 47, 48, 24, 25, 19, 23,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 10,  7,  1,  1,  1,  0,  0,  0,  0,  0,  0,
     1, 23,  0,  0,  0,  0,  0,  0, 15,  6, 17,  9,  1, 23,  6,  0,
     0,  0,  0,  2,  1,  8, 20, 20,  0,  0,  8,  4, 49,  8,  7,  1,
     1,  1, 24, 17,  0,  0,  0,  0,  1, 16, 50,  6,  6,  1,  6,  6,
     2, 51, 51, 51, 52,  0, 18,  0,  0,  0, 16, 15,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 10,  0,  0,  0,  0,  0, 16,  0, 10,  0,  0,
     0, 15,  5,  2,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 18,  6,  0,  0,  0,  0, 18,  6, 17,  6,  7,
     0, 10,  8,  1,  6, 24,  2, 53, 54,  0,  0,  0,  0,  0,  0,  0,
     0,  0, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,  1, 17, 55,
    41, 40, 56,  3,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,  2,  0,
     0,  0,  0,  0,  0, 15,  2,  0,  2,  1, 57, 58, 59, 46, 35,  1,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  7,  9,
     0,  0, 15,  0,  0,  0,  0,  0,  0, 15, 20,  8, 40, 23,  5,  0,
    60,  6, 10, 52,  0,  0,  6,  7,  0,  0,  0,  0, 17,  3,  0,  0,
    20, 23,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  6,  6,
     6,  1,  1, 16,  0,  0,  0,  0,  4,  5,  7,  2,  5,  3,  0,  0,
     1, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,  1,  6, 41, 38,
    17,  3, 16,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,
     0, 15,  9,  6,  6,  6,  1, 19, 23,  0,  0,  0,  0, 10,  3,  0,
     0,  0,  0,  0,  0,  0,  8,  5,  1, 30,  2,  1,  0,  0,  0, 16,
     0,  0,  0,  0,  0,  0,  0, 10,  4,  5,  7,  1, 17,  3,  0,  0,
     2,  8, 23, 11, 12, 13, 33,  0,  0,  8,  0,  1,  1,  1, 16,  0,
     1,  1, 16,  0,  0,  0,  0,  0,  0,  0, 15,  9,  6,  6,  6,  1,
     8,  7,  2,  3,  0,  0,  0,  0,  4,  5,  6,  6, 39, 61, 33, 26,
     2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15,
     9,  6,  6,  0, 49, 32,  1,  5,  3,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  6,  0,  8,  5,  6,  6,  7,  2, 20,  5,
    16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10, 20,  9,
     6,  1,  1,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18, 10,
     8,  1,  6, 41,  7,  1,  0,  0, 10,  6,  1,  1,  1,  3,  0,  0,
     0, 18,  7,  1,  2, 18,  6,  3, 10,  6,  1,  2,  5,  6,  0,  0,
     0,  0,  0,  0,  0,  6,  6,  6,  6,  6,  6,  2, 16,  0,  0,  0,
     1,  6,  6,  3,  1,  1,  1,  5,  0,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6, 15,  6,  6,  6, 39,  7, 20, 16,  0,  0,  0,  0,
    10,  1,  1,  3,  0, 16,  1, 10,  1,  7,  3, 18,  0,  0,  0,  0,
};

static RE_UINT8 re_indic_positional_category_stage_5[] = {
     0,  0,  5,  5,  5,  1,  6,  0,  1,  2,  1,  6,  6,  6,  6,  5,
     1,  1,  2,  1,  0,  5,  0,  2,  2,  0,  0,  4,  4,  6,  0,  1,
     5,  0,  5,  6,  0,  6,  5,  8,  1,  5,  9,  0, 10,  6,  1,  0,
     2,  2,  4,  4,  4,  5,  7,  0,  8,  1,  8,  0,  8,  8,  9,  2,
     4, 10,  4,  1,  3,  3,  3,  1,  3,  0,  5,  7,  7,  7,  6,  2,
     6,  1,  2,  5,  9, 10,  4,  2,  1,  8,  8,  5,  1,  3,  6, 11,
     7, 13,  2,  9, 14,  6, 14, 14, 14,  0,  1, 12, 11,  0,  5,  2,
     2,  6,  6,  3,  3,  5,  5,  3,  0, 14,  5,  9,
};

/* Indic_Positional_Category: 2036 bytes. */

RE_UINT32 re_get_indic_positional_category(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_indic_positional_category_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_indic_positional_category_stage_2[pos + f] << 4;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_indic_positional_category_stage_3[pos + f] << 3;
    f = code >> 1;
    code ^= f << 1;
    pos = (RE_UINT32)re_indic_positional_category_stage_4[pos + f] << 1;
    value = re_indic_positional_category_stage_5[pos + code];

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
     9,  1,  1,  1,  1,  1,  1, 10,  1, 11, 12, 13, 14, 15,  1,  1,
    16,  1,  1,  1,  1, 17,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1, 18, 19, 20, 21,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 22,  1,  1,  1,  1,  1,
    23, 24, 25, 26, 27, 28, 29, 30,  1,  1, 31,  1, 32, 33,  1,  1,
};

static RE_UINT8 re_indic_syllabic_category_stage_3[] = {
      0,   0,   1,   2,   0,   0,   0,   0,   0,   0,   3,   4,   0,   5,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  12,  20,
     21,  15,  16,  22,  23,  24,  25,  26,  27,  28,  16,  29,  30,   0,  12,  31,
     14,  15,  16,  29,  32,  33,  12,  34,  35,  36,  37,  38,  39,  40,  25,   0,
     41,  42,  16,  43,  44,  45,  12,   0,  46,  42,  16,  47,  44,  48,  12,  49,
     41,  42,   8,  50,  51,  52,  12,  53,  54,  55,   8,  56,  57,  58,  25,  59,
     60,   8,  61,  62,  63,   2,   0,   0,  64,  65,  66,  67,  68,  69,   0,   0,
      0,   0,  70,  71,  72,   8,  73,  74,  75,  76,  77,  78,  79,   0,   0,   0,
      8,   8,  80,  81,  82,  83,  84,  85,  86,  87,   0,   0,   0,   0,   0,   0,
     88,  89,  90,  89,  90,  91,  88,  92,   8,   8,  93,  94,  95,  96,   2,   0,
     97,  61,  98,  99,  25,   8, 100, 101,   8,   8, 102, 103, 104,   2,   0,   0,
      8, 105,   8,   8, 106, 107, 108, 109,   2,   2,   0,   0,   0,   0,   0,   0,
    110,  90,   8, 111, 112,   2,   0,   0, 113,   8, 114, 115,   8,   8, 116, 117,
      8,   8, 118, 119, 120,   0,   0,   0,   0,   0,   0,   0,   0, 121, 122, 123,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 124,
    125, 126,   0,   0,   0,   0,   0, 127, 128,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 129,   0,   0,   0,
    130,   8, 131,   0,   8, 132, 133, 134, 135, 136,   8, 137, 138,   2, 139, 140,
    141,   8, 142,   8, 143, 144,   0,   0, 145,   8,   8, 146, 147,   2, 148, 149,
    150,   8, 151, 152, 153,   2,   8, 154,   8,   8,   8, 155, 156,   0, 157, 158,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 159, 160, 161,   2,
    162, 163,   8, 164, 165,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    166,  90,   8, 167, 168, 169, 170, 171, 172,   8,   8, 173,   0,   0,   0,   0,
    174,   8, 175, 176,   0, 177,   8, 178, 179, 180,   8, 181, 182,   2, 183, 184,
    185, 186, 187, 188,   0,   0,   0,   0, 189, 190, 191, 192,   8, 193, 194,   2,
    195,  15,  16,  29,  32, 196, 197, 198,   0,   0,   0,   0,   0,   0,   0,   0,
    199,   8,   8, 200, 201,   2,   0,   0, 202,   8,   8, 203, 204,   2,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0, 199,   8, 205, 206, 207, 208,   0,   0,
    199,   8,   8, 209, 210,   2,   0,   0, 192,   8, 211, 212,   2,   0,   0,   0,
      8, 213, 214, 215,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    216,   8,   8, 217, 218, 219,   8,   8, 220, 221,   0,   0,   0,   0,   0,   0,
    222,   8, 205, 223, 224,  70, 225, 226,   8, 227,  76, 228,   0,   0,   0,   0,
    229,   8,   8, 230, 231,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_indic_syllabic_category_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   1,   2,   2,   3,   0,   4,   0,   0,   0,
      5,   0,   0,   0,   0,   6,   0,   0,   7,   8,   8,   8,   8,   9,  10,  10,
     10,  10,  10,  10,  10,  10,  11,  12,  13,  13,  13,  14,  15,  16,  10,  10,
     17,  18,   2,   2,  19,   8,  10,  10,  20,  21,   8,  22,  22,   9,  10,  10,
     10,  10,  23,  10,  24,  25,  26,  12,  13,  27,  27,  28,   0,  29,   0,  30,
     26,   0,   0,  31,  20,  21,  32,  33,  23,  34,  26,  35,  36,  29,  27,  37,
      0,   0,  38,  24,   0,  18,   2,   2,  39,  40,   0,   0,  20,  21,   8,  41,
     41,   9,  10,  10,  23,  38,  26,  12,  13,  42,  42,  37,   0,   0,  43,  44,
     13,  27,  27,  37,   0,  45,   0,  30,  46,   0,   0,   0,  47,  21,  32,  19,
     48,  49,  34,  23,  50,  51,  52,  25,  10,  10,  26,  45,  36,  45,  53,  37,
      0,  29,   0,   0,   7,  21,   8,  48,  48,   9,  10,  10,  10,  10,  26,  54,
     13,  53,  53,  37,   0,  55,  52,   0,  20,  21,   8,  48,  10,  38,  26,  12,
      0,  55,   0,  56,  57,   0,   0,   0,  10,  10,  58,  59,  13,  53,  53,  60,
      0,  61,   0,  33,   0,   0,  62,  63,  64,  21,   8,   8,   8,  32,  25,  10,
     30,  10,  10,  46,  10,  52,  65,  29,  13,  66,  13,  13,  45,   0,   0,   0,
     38,  10,  10,  10,  10,  10,  10,  52,  13,  13,  67,   0,  13,  42,  68,  69,
     34,  70,  24,  46,   0,  10,  38,  10,  38,  71,  25,  34,  13,  13,  42,  72,
     13,  73,  68,  74,   2,   2,   3,  10,   2,   2,   2,   2,   2,  75,  76,   0,
     10,  10,  38,  10,  10,  10,  10,  51,  16,  13,  13,  77,  78,  79,  80,  81,
     82,  82,  83,  82,  82,  82,  82,  82,  82,  82,  82,  84,   0,  85,   0,   0,
     86,   8,  87,  13,  13,  88,  89,  90,   2,   2,   3,  91,  92,  17,  93,  94,
     95,  96,  97,  98,  99, 100,  10,  10, 101, 102,  68, 103,   2,   2, 104, 105,
    106,  10,  10,  23,  11, 107,   0,   0, 106,  10,  10,  10,  11,   0,   0,   0,
    108,   0,   0,   0, 109,   8,   8,   8,   8,  45,  13,  13,  13,  77, 110, 111,
    112,   0,   0, 113, 114,  10,  10,  10,  13,  13, 115,   0, 116, 117, 118,   0,
    119, 120, 120, 121, 122, 123,   0,   0,  10,  10,  10,   0,  13,  13,  13,  13,
    124, 117, 125,   0,  10, 126,  13,   0,  10,  10,  10,  86, 106, 127, 128, 129,
    130,  13,  13,  13,  13, 131, 132, 133, 134, 135,   8,   8,  10, 136,  13,  13,
     13, 137,  10,   0, 138,   8, 139,  10, 140,  13, 141, 142,   2,   2, 143, 144,
     10, 145,  13,  13, 146,   0,   0,   0,  10, 147,  13, 124, 117, 148,   0,   0,
      2,   2,   3,  38, 149, 150, 150, 150, 151,   0,   0,   0, 152, 153, 151,   0,
      0,   0, 154,   0,   0,   0,   0, 155, 156,   4,   0,   0,   0, 157,   0,   0,
      5, 157,   0,   0,   0,   0,   0,   4,  41, 158, 159,  10, 126,  13,   0,   0,
     10,  10,  10, 160, 161, 162, 163,  10, 164,   0,   0,   0, 165,   8,   8,   8,
    139,  10,  10,  10,  10, 166,  13,  13,  13, 167,   0,   0, 150, 150, 150, 150,
    168,   0,   0,   0,   2,   2, 169,  10, 160, 120, 170, 125,  10, 126,  13, 171,
    172,   0,   0,   0, 173,   8,   9, 106, 174,  13,  13, 175, 176,   0,   0,   0,
     10, 177,  10,  10,   2,   2, 169,  52,   8, 139,  10,  10,  10,  10,  99,  13,
    178, 179,   0,   0, 117, 117, 117, 180,  38, 181, 182,  98,  13,  13,  13, 102,
    183,   0,   0,   0, 139,  10, 126,  13,   0, 184,   0,   0,  10,  10,  10,  92,
    185,  10, 186, 117, 187,  13,  36, 188,  99,  55,   0,  77,  10,  38,  38,  10,
     10,   0, 189, 190,   2,   2,   0,   0, 191, 192,   8,   8,  10,  10,  13,  13,
     13, 193,   0,   0, 194, 195, 195, 195, 195, 196,   2,   2,   0,   0,   0, 197,
    198,   8,   8,   9,  13,  13, 199,   0, 198, 106,  10,  10,  10, 126,  13,  13,
    200, 201,   2,   2, 120, 202,  10,  10, 174,   0,   0,   0, 198,   8,   8,   8,
      9,  10,  10,  10, 126,  13,  13,  13, 203,   0, 204,  73, 205,   2,   2,   2,
      2, 206,   0,   0,   8,   8,  10,  10,  30,  10,  10,  10,  10,  10,  10,  13,
     13, 207,   0, 208,   8,  52,  23,  30,  10,  10,  10,  30,  10,  10,  51,   0,
      8,   8, 139,  10,  10,  10,  10, 159,  13,  13, 209,   0,   7,  21,   8,  22,
      0,  29,   0, 210,  17, 211, 150, 212, 150, 212,   0,   0,   8,   8,   8, 139,
     10,  99,  13,  13, 213, 214,   0,   0,  21,   8,   8, 106,  13,  13,  13, 215,
    216, 217,   0,   0,  10,  10,  10, 126,  13, 105,  13, 218, 219,   0,   0,   0,
      0,   0,   8, 105,  13,  13,  13, 220,  73,   0,   0,   0,  10,  10, 159, 221,
     13, 222,   0,   0,  10,  10,  26, 223,  13,  13, 224,   0,   2,   2,   2,   0,
    225,  13, 226,  10, 227, 228, 229, 230,   0, 231,   0,   0, 225,  13,  13,  10,
     10, 232, 233, 117, 117, 234, 235,   0,   8,   8,  48, 139,  13,  36,  13, 218,
    217,   0,   0,   0,   2,   2,   2, 206,  25,  10,  10,  10, 236,  82,  82,  82,
     13, 237,   0,   0,   8,  32,  41,  10,  99,  36, 238,  42, 239, 240,   0,   0,
};

static RE_UINT8 re_indic_syllabic_category_stage_5[] = {
     0,  0,  0,  0,  0, 11,  0,  0, 33, 33, 33, 33, 33, 33,  0,  0,
    11,  0,  0,  0,  0,  0, 28, 28,  0,  0,  0, 11,  1,  1,  1,  2,
     8,  8,  8,  8,  8, 12, 12, 12, 12, 12, 12, 12, 12, 12,  9,  9,
     4,  3,  9,  9,  9,  9,  9,  9,  9,  5,  9,  9,  0, 26, 26,  0,
     0,  9,  9,  9,  8,  8,  9,  9,  0,  0, 33, 33,  0,  0,  8,  8,
     0,  1,  1,  2,  0,  8,  8,  8,  8,  0,  0,  8, 12,  0, 12, 12,
    12,  0, 12,  0,  0,  0, 12, 12, 12, 12,  0,  0,  9,  0,  0,  9,
     9,  5, 13,  0,  0,  0,  0,  9, 12, 12,  0, 12,  1,  0,  0,  0,
     8,  8,  8,  0,  0,  0,  0,  8,  0, 12, 12,  0,  4,  0,  9,  9,
     9,  9,  9,  0,  9,  5,  0,  0,  0, 12, 12, 12,  1, 25, 11, 11,
     0, 19,  0,  0,  8,  8,  0,  8,  9,  9,  0,  9,  0, 12, 26, 26,
    26,  4,  4,  4,  0,  0,  9,  9,  0, 12,  0,  0,  0,  0,  1, 22,
     8,  0,  8,  8,  8, 12,  0,  0,  0,  0,  0, 12, 12,  0,  0,  0,
    12, 12, 12,  0,  9,  0,  9,  9,  0,  3,  9,  9,  0,  9,  9,  0,
     0,  0, 12,  0,  0, 14, 14,  0, 12, 12, 12,  6,  6,  3,  9,  9,
     9,  5, 16,  0, 13, 13, 13,  9,  0,  0, 13, 13, 13, 13, 13, 13,
     0,  0,  1,  2,  0,  0,  5,  0,  9,  0,  9,  0,  9,  9,  6,  0,
    24, 24, 24, 24, 29,  1,  6,  0, 12,  0,  0, 12,  0, 12,  0, 12,
    19, 19,  0,  0,  9,  0,  0,  0,  0,  1,  0,  0,  0, 28,  0, 28,
     0,  4,  0,  0,  9,  9,  1,  2,  9,  9,  1,  1,  6,  3,  0,  0,
    21, 21, 21, 21, 21, 18, 18, 18, 18, 18, 18, 18,  0, 18, 18, 18,
    18,  0,  0,  0,  0,  0, 28,  0, 12,  8,  8,  8,  8,  8,  8,  9,
     9,  9,  1, 24,  2,  7,  6, 19, 19, 19, 19, 12,  0,  0, 11,  0,
    12, 12,  8,  8,  9,  9, 12, 12, 12, 12, 19, 19, 19, 12,  9, 24,
    24, 12, 12,  9,  9, 24, 24, 24, 24, 24, 12, 12, 12,  9,  9,  9,
     9, 12, 12, 12, 12, 12, 19,  9,  9,  9,  9, 24, 24, 24, 12, 24,
    33, 33, 24, 24,  9,  9,  0,  0,  8,  8,  8, 12,  6,  0,  0,  0,
    12,  0,  9,  9, 12, 12, 12,  8,  9, 27, 27, 28, 17, 29, 28, 28,
    28,  6,  7, 28,  3, 28,  0,  0, 11, 12, 12, 12,  9, 18, 18, 18,
    20, 20,  1, 20, 20, 20, 20, 20, 20, 20,  9, 28, 12, 12, 12, 10,
    10, 10, 10, 10, 10, 10,  0,  0, 23, 23, 23, 23, 23,  0,  0,  0,
     9, 20, 20, 20, 24, 24,  0,  0, 12, 12, 12,  9, 12, 19, 19, 18,
    20, 20, 17, 18, 18, 18, 18,  0,  7,  9,  9,  9,  1, 24, 24, 24,
    24, 24,  6, 28, 28,  0,  0, 28,  1,  1,  1, 17,  2,  8,  8,  8,
     4,  9,  9,  9,  5, 12, 12, 12,  1, 17,  2,  8,  8,  8, 12, 12,
    12, 18, 18, 18,  9,  9,  6,  7, 18, 18, 12, 12, 33, 33,  3, 12,
    12, 12, 20, 20,  8,  8,  4,  9, 20, 20,  6,  6, 18, 18,  9,  9,
     1,  1, 28,  4, 26, 26, 26,  0, 26, 26, 26, 26, 26, 26,  0,  0,
     0,  0,  2,  2, 26,  0,  0, 26,  0,  0,  0, 28, 30, 31,  0,  0,
    11, 11, 11, 11, 28,  0,  0,  0,  8,  8,  6, 12, 12, 12, 12,  1,
    12, 12, 10, 10, 10, 10, 12, 12, 12, 12, 10, 18, 18, 12, 12, 12,
    12, 18, 12,  1,  1,  2,  8,  8, 20,  9,  9,  9,  5,  1,  0,  0,
    26, 26,  1,  1, 33, 33, 12, 12, 10, 10, 10, 24,  9,  9,  9, 20,
    20, 20, 20,  6,  1,  1, 17,  2, 12, 12, 12,  4,  9, 18, 19, 19,
     5,  0,  0,  0, 12,  9,  0, 12,  9,  9,  9, 19, 19, 19, 19,  0,
    20, 20,  0,  0, 11, 11, 11,  0,  0,  0, 12, 24, 23, 24, 23,  0,
     0,  2,  7,  0, 12,  8, 12, 12, 12, 12, 12, 20, 20, 20, 20,  9,
    24,  6,  0,  0,  4,  4,  4,  0,  0,  0,  0,  7,  1,  1,  2, 14,
    14,  8,  8,  8,  9,  9,  5,  0,  0,  0, 34, 34, 34, 34, 34, 34,
    34, 34, 33, 33,  0,  0,  0, 32,  1,  1,  2,  8,  9,  5,  4,  0,
     9,  9,  9,  7,  6,  0, 33, 33, 10, 12, 12, 12,  5,  3, 15, 15,
     0,  0,  4,  9,  0, 33, 33, 33, 33,  0,  0,  0,  1,  5,  4, 25,
     0,  0, 26,  0,  9,  4,  6,  0,  0,  0,  1,  1,  0,  0, 26, 26,
    26,  0,  0,  0,  9,  9,  5,  1,  1,  2,  4,  3,  9,  9,  9,  1,
     1,  2,  5,  4,  3,  0,  0,  0,  1,  1,  2,  5,  4,  0,  0,  0,
     9,  1,  2,  5,  2,  9,  9,  9,  9,  9,  5,  4,  0, 19, 19, 19,
     9,  9,  9,  6,  8,  9,  9,  9,  9,  9,  9, 12, 12, 12, 12, 28,
     6,  1,  1,  1,  1,  2, 15, 19, 19, 19, 19, 11,  0, 11,  0,  7,
     0,  0, 15, 15, 15, 15, 20, 20, 20, 20,  1,  2, 25,  7,  0,  0,
     0,  0, 18, 18,  9,  1,  1,  0,  0,  0,  9,  0,  1,  2,  4,  9,
     6,  7, 16, 19,
};

/* Indic_Syllabic_Category: 2732 bytes. */

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

/* NFD_Quick_Check. */

static RE_UINT8 re_nfd_quick_check_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 5, 1, 1, 1, 6, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_nfd_quick_check_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  8,  9,  9,  9,  9, 10,  7,  7,  7,  7, 11,
     7,  7, 12,  7,  7,  7,  7,  7,  7,  7, 13,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7, 14,
};

static RE_UINT8 re_nfd_quick_check_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  5,  5,  7,  8,  9, 10, 11,  5, 12,
    13,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, 14,  5,  5, 15, 16,
    17, 18, 19, 20,  5,  5,  5,  5,  5,  5, 21,  5,  5,  5,  5,  5,
    22,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
     5,  5,  5,  5, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 24,  5, 23, 25, 26,  5,  5,  5,  5,
    27, 28,  5, 29, 30, 31,  5,  5,  5, 32,  5,  5,  5,  5,  5,  5,
    23, 23, 33,  5,  5,  5,  5,  5,
};

static RE_UINT8 re_nfd_quick_check_stage_4[] = {
     0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  0,  7,  8,  9,
    10, 11,  0,  0,  0,  0,  0,  0,  0,  0, 12, 13, 14, 15, 16,  0,
    17, 18, 19, 20,  0,  0, 21, 22,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 23,  0,  0,  0,  0, 24,  0,  0, 25, 26,  0,  0,  0, 27,  0,
     0, 28, 29,  0,  0,  0,  0,  0,  0,  0, 30,  0, 31,  0, 32,  0,
     0,  0, 33,  0,  0,  0, 34,  0,  0,  0, 32,  0,  0,  0, 35,  0,
     0,  0, 36, 37, 38, 39,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,
    41, 42, 43,  0,  0,  0,  0,  0, 44, 44, 44, 44, 45, 44, 44, 46,
    47, 44, 48, 49, 44, 50, 51, 52, 53,  0,  0,  0,  0,  0,  0,  0,
     0, 54,  0,  0, 55, 56, 57,  0, 58, 59, 60, 61, 62, 63,  0, 64,
     0, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 66,  0,
     0,  0, 67, 68, 13, 67, 68, 69, 44, 44, 44, 44, 44, 44, 44, 44,
    44, 44, 44, 44, 44, 70,  0,  0, 71, 72, 44, 73, 44, 44, 46,  0,
    74, 75, 76,  0,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,
     0, 79,  0,  0,  0,  0,  0,  0,  0,  0, 80,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 81,  0,  0,  0,  0,  0,  0,  0, 55,  0,  0,
     0,  0, 82, 83,  0, 84, 85,  0, 49,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_nfd_quick_check_stage_5[] = {
      0,   0,   0,   0, 191, 255, 126,  62, 191, 255, 126, 190, 255, 255, 252, 255,
     63, 255, 241, 126, 248, 241, 243, 255,  63, 255, 255, 127,   3, 128,   1,   0,
      0, 224, 255, 223, 207, 255,  49, 255, 255, 255, 255, 207, 192, 255,  15,   0,
     27,   0,   0,   0,   0,   0,  16,  64, 224, 215,   1,   0,   0, 252,   1,   0,
      0, 124,  24,   0, 139, 112,   0,   2,   0,   0,   0,   2,   0,   0, 139, 112,
      0,   0, 192,   0,   6,   0, 207, 252, 252, 252,  63,   3, 124,   0,   0,   0,
      5,   0,   8,   0,   0,   2,  18,   0,   0,   0,   0, 255,   0,  24,   0, 176,
      0,   0,  72,   0,   0,   0,   0,  78,   0,  25,   0,  48,   0,   0,  16,   0,
      0,  28,   0,   0,   0,   1,   0,   0, 129,  13,   0,   0,   0,   0,   0, 116,
      8,  32, 132,  16,   0,   2, 104,   1,   2,   0,   8,  32, 132,  16,   0,   2,
     64,   0,   0,   0,  64,  85,   4,   0,   0,   0,   0,  40,  11,   0,   0,   0,
    255, 255, 255, 255, 255, 255, 255,  11, 255, 255, 255,   3, 255, 255,  63,  63,
     63,  63, 255, 170, 255, 255, 255,  63, 255, 255, 223,  95, 222, 255, 207, 239,
    255, 255, 220,  63,   3,   0,   0,   0,  64,  12,   0,   0,   0,   0,   0,  12,
      0,  64,   0,   0,   0, 224,   0,   0,  16,  18,   0,   0,  80,   0,   0,   0,
    146,   2,   0,   0,   5, 224,  51,   3,  51,   3,   0,   0,   0, 240,   0,   0,
     15,  60,   0,   0,   0,   6,   0,   0,   0,   0,   0,  16,   0,  80,  85,  85,
    165,   2, 219,  54,   0,   0, 144,  71,  15,   0,   0,   0, 255,  63, 229, 127,
    101, 252, 255, 255, 255,  63, 255, 255,   0,   0,   0, 160,   0, 252, 127,  95,
    219, 127,   0,   0,   0,   0,   0,  20,   0,   8,   0,   0,   0, 192,   0,   0,
      0,  24,   0,   0,   0,   0,   0,  88,   0,   0,   0, 192,  31,   0,   0,   0,
      0,   0,   0, 248,   1,   0,   0,   0,
};

/* NFD_Quick_Check: 860 bytes. */

RE_UINT32 re_get_nfd_quick_check(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_nfd_quick_check_stage_1[f] << 3;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_nfd_quick_check_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_nfd_quick_check_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_nfd_quick_check_stage_4[pos + f] << 5;
    pos += code;
    value = (re_nfd_quick_check_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* NFC_Quick_Check. */

static RE_UINT8 re_nfc_quick_check_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 3, 4, 2, 2, 2, 2, 2, 5, 2,
    2, 2, 2, 2, 2, 2, 2, 6, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_nfc_quick_check_stage_2[] = {
     0,  0,  0,  1,  0,  0,  2,  0,  0,  3,  4,  5,  6,  7,  0,  8,
     9, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0, 12,
    13, 14,  0, 15,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,
    17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0, 18, 19, 20,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    21, 22,  0, 23, 24, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 18, 18, 27,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_nfc_quick_check_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  0,  0,  0,
     0,  0,  6,  0,  0,  0,  0,  0,  0,  7,  8,  0,  0,  9, 10,  0,
     0, 11, 12,  0,  0,  0,  0,  0,  0,  9, 13,  0,  0,  9, 14,  0,
     0,  0, 15,  0,  0,  0, 16,  0,  0,  9, 14,  0,  0,  0, 17,  0,
     0,  0, 18, 19, 20, 21,  0,  0,  0, 22,  0,  0,  0,  0,  0,  0,
     0,  0,  0, 23,  0, 24, 25,  0,  0, 26,  0,  0,  0,  0,  0,  0,
     0,  0,  0, 27,  0, 28, 29, 30, 31,  0,  0,  0,  0,  0,  0,  0,
     0, 32,  0,  0,  0,  0,  0,  0,  0, 33,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0, 35,  0,  0,  0,
    36, 36, 36, 36, 36, 36, 36, 36, 37, 38, 36, 39, 36, 36, 40,  0,
    41, 42, 43,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 44,  0,  0,
     0, 45,  0,  0,  0,  0,  0,  0,  0,  9, 14,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 46,  0,  0,  0,  0,  0,  0,  0, 47,  0,  0,
     0,  0, 48, 49,  0, 50, 51,  0, 52,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_nfc_quick_check_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  1,  3,  4,  5,  6,  0,
     6,  1,  5,  7,  8,  0,  5,  0,  9, 10,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 11,  0, 12,  0, 13,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  6,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,
     0,  0,  0,  0,  0,  0, 14, 14,  0,  0,  0,  0,  0,  0,  0, 15,
     0,  0,  0,  0,  0,  6,  0, 16,  0,  0,  0,  0, 13, 12,  0,  0,
     0,  0,  0,  0,  0,  0, 17, 12,  0,  0,  0,  0,  0, 18,  0, 19,
     0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0, 15,  0,  0,
    15,  0,  0,  0,  0,  7,  0,  0,  0,  0, 15,  6,  0,  0,  0,  6,
    13,  0,  0, 20, 12, 13,  0, 11,  0,  0, 20,  0, 13, 21, 11,  0,
    20,  0,  0,  0, 13,  0,  0, 20, 12, 13,  0, 11,  0,  0, 20,  0,
     0,  0,  0, 15,  0,  0,  0,  0, 22,  1,  1,  1,  1,  8,  0,  0,
     0,  0,  1,  1,  1,  1,  1,  1, 23,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0, 25, 25, 25, 20,
     0,  0,  0,  0,  0,  0, 13, 12,  0,  0, 25,  0, 13,  0, 13,  0,
    13,  0, 13, 26,  0,  0, 25, 20, 19,  0,  0,  0,  0,  0,  0,  0,
     0, 12, 26,  0,  0,  0,  0,  0,  0,  0, 21,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,  0,  7,  0,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 19, 27, 17, 14, 28,
    27, 21, 26, 14, 14, 14, 14, 14, 14, 14, 14, 19, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 19,  0,  0,  0,  0,  0,  0,  0,  0, 25,
     0,  0, 26, 14, 14, 28, 14, 27, 16, 29, 14, 28,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 15,  0,  0,  6,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  5,  0, 15, 24,  0,  0,  0,  6,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 26, 14, 11,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 13, 14, 11,  0,  0,  0,  0,  0,  0,  0,
    14, 14, 14, 14, 14, 14, 14, 19,
};

static RE_UINT8 re_nfc_quick_check_stage_5[] = {
    0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 0, 2,
    0, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 2, 0,
    2, 2, 0, 0, 1, 1, 2, 1, 1, 2, 0, 0, 1, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 2, 0,
    1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 2, 2, 1, 1, 0, 0,
    0, 1, 0, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0,
    1, 1, 1, 0, 1, 0, 1, 1,
};

/* NFC_Quick_Check: 1128 bytes. */

RE_UINT32 re_get_nfc_quick_check(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_nfc_quick_check_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_nfc_quick_check_stage_2[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_nfc_quick_check_stage_3[pos + f] << 3;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_nfc_quick_check_stage_4[pos + f] << 2;
    value = re_nfc_quick_check_stage_5[pos + code];

    return value;
}

/* NFKD_Quick_Check. */

static RE_UINT8 re_nfkd_quick_check_stage_1[] = {
    0, 1, 2, 3, 4, 1, 1, 5, 1, 1, 1, 6, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static RE_UINT8 re_nfkd_quick_check_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  8,  9, 10, 10, 10, 10, 11,  7,  7,  7,  7, 12,
     7,  7, 13,  7,  7,  7,  7,  7,  7,  7, 14,  7,  7, 15, 16,  7,
     7,  7,  7,  7,  7,  7,  7, 17,
};

static RE_UINT8 re_nfkd_quick_check_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  9, 10, 11, 12, 13, 14,
    15,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 16,  7, 17, 18, 19,
    20, 21, 22, 23, 24,  7,  7,  7,  7,  7, 25,  7, 26, 27, 28, 29,
    30, 31, 32, 33,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7, 34, 35,  7,  7,  7, 36, 33, 33, 33, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 37,
     7, 33, 38, 39, 33, 40, 41, 42, 43, 44,  7, 45, 46, 47,  7,  7,
     7, 48,  7,  7, 49, 50, 51, 52,  7,  7,  7,  7,  7,  7, 53,  7,
     7, 54, 55,  7,  7,  7,  7,  7, 33, 33, 56,  7,  7,  7,  7,  7,
};

static RE_UINT8 re_nfkd_quick_check_stage_4[] = {
      0,   0,   0,   0,   0,   1,   2,   3,   4,   5,   6,   7,   0,   8,   9,  10,
     11,  12,   0,   0,   0,  13,  14,  15,   0,   0,  16,  17,  18,  19,  20,  21,
     22,  23,  24,  25,   0,   0,  26,  27,   0,   0,   0,   0,  28,   0,   0,   0,
      0,  29,   0,  30,   0,   0,  31,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,  32,  33,   0,   0,   0,  34,   0,   0,  35,  36,   0,   0,   0,   0,   0,
      0,   0,  37,   0,  38,   0,  39,   0,   0,   0,  40,   0,   0,   0,  41,   0,
      0,   0,  39,   0,   0,   0,  42,   0,   0,  43,   0,   0,   0,  43,  44,   0,
     45,   0,  46,  47,  48,  49,   0,   0,   0,  50,   0,   0,   0,   0,   0,  51,
     52,  53,  54,   0,   0,   0,   0,   0,   0,  55,  56,  57,  58,  59,   0,   0,
     59,  59,  59,  59,  60,  59,  59,  61,  62,  59,  63,  64,  59,  65,  66,  67,
     68,  69,  70,  71,  72,  40,   0,   0,  73,  74,  75,  59,  76,  77,  78,   0,
     79,  80,  81,  82,  83,  84,   0,  85,   0,  86,   0,   0,   0,   0,   0,   0,
      0,   0,   0,  59,  59,  59,  59,  87,  45,   0,   0,  88,   0,   0,  51,   0,
      0,   0,   0,  44,   0,   0,   0,   0,   0,   0,   0,  89,   0,   0,   0,   0,
      0,   0,   0,   0,  90,   0,   0,  43,  59,  59,  59,  59,  59,  59,  91,   0,
     92,  93,  94,  95,  96,  94,  95,  97,   0,  98,  59,  59,  99,   0,   0,   0,
    100,  59, 101, 100,  59,  59,  59, 100,  59,  59,  59,  59,  59,  59,  59,  59,
      0,   0,   0,   0,  44,   0,   0,   0,   0,   0,   0, 102,   0,   0,   0, 103,
      0,   0, 104,   0,   0,   0,   0,   0,  59,  59,  59,  59,  59, 105,   0,   0,
    106, 107,  59, 108,  59,  59,  61,   0, 109, 110, 111,  59,  59, 112, 113,  59,
     59,  64, 114,  59,   4,  59, 115, 116, 117, 114, 118, 119,  59,  59,  59, 120,
    121,  59,  59,  59,  59, 100, 122, 123,   0,   0,   0,   0, 124, 125,   0,   0,
      0, 126,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 128,   0,   0,   0,   0,   0,   0,   0, 129,   0,   0,
      0,   0, 130,  15,   0,  58,  92,   0,  59,  59,  65,  59, 131, 132, 133,  59,
    134, 135, 136,  59,  59,  59,  59,  59,  59,  59,  59,  59,  59,   7,  59,  59,
     59,  59,  59,  59,  59,  59, 137,  59, 133, 138, 139, 140, 141, 142,   0,   0,
    143, 144, 145, 146, 102,   0,   0,   0, 147,  60, 148,   0,   0,   0,   0,   0,
     64,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_nfkd_quick_check_stage_5[] = {
      0,   0,   0,   0,   1, 133,  60, 119, 191, 255, 126,  62, 191, 255, 126, 190,
    255, 255, 252, 255,  63, 255, 253, 254, 249, 243, 243, 255,  63, 255, 255, 255,
      3, 128,   1,   0, 240, 255, 255, 223, 207, 255,  63, 255, 255, 255, 255, 207,
    192, 255,  15,   0,   0,   0, 255,   1,   0,   0,   0,  63,  31,   0,   0,   0,
     27,   0,   0,   0,   0,   0,  16,  68, 240, 215,   1,   0,   0, 252,   1,   0,
      0, 124, 127,   0,   0,   0,  55,   2, 139, 112,   0,   2,   0,   0,   0,   2,
      0,   0, 139, 112,   0,   0, 192,   0,   6,   0, 207, 252, 252, 252,  63,   3,
    128,   0,   0,   0, 124,   0,   0,   0,   0,   0, 224,   1,   5,   0,   8,   0,
      0,   2,  18,   0,   0,   0,   0, 255,   0,  24,   0, 176,   0,   0,  72,   0,
      0,   0,   0,  78,   0,  25,   0,  48,   0,   0,  16,   0,   0,  28,   0,   0,
      0,   1,   0,   0, 129,  13,   0,   0,   0,   0,   0, 116,   0,   0,   8,   0,
      0,   0,   0,  48,   0,  16,   0,   0,   8,  32, 132,  16,   0,   2, 232,   3,
      2,   0,   8,  32, 132,  16,   0,   2,  64,   0,   0,   0,   0,   0,   0,  16,
     64,  85,   4,   0,   0,   0,   0,  40,  11,   0,   0,   0,   0, 112, 255, 247,
    255, 191, 255, 255, 255,   7,   0,   1,   0,   0,   0, 248, 255, 255, 255, 255,
    255, 255, 255,  15, 255, 255, 255,   3, 255, 255,  63,  63,  63,  63, 255, 170,
    255, 255, 255,  63, 255, 255, 223, 255, 223, 255, 207, 239, 255, 255, 220, 127,
    255,   7, 130,   0, 112, 128, 216,  80, 128,   3, 128, 128,   0,   0, 243, 255,
    255, 127, 255,  31, 239, 254, 111,  62,  87, 189, 251, 251, 225,   3, 255, 255,
      0,   2,   0,  12,   0,  64,   0,   0,   0, 224,   0,   0,  16,  18,   0,   0,
     80, 176,   1,   0, 146,   2,   0,   0,   5, 224,  51,   3,  51,   3,   0,   0,
      0, 240,   0,   0,  15,  60,   0,   0,   0,   6,   0,   0, 255,   7,   0,   0,
      0,   0, 112,   0,   0, 128,   0,   0,   0,   0,   0, 128, 255, 255,  63,   0,
      1,   0,   0,   0,   0,   0,  64,   7,   0,  80,  85,  85, 165,   2, 219,  54,
      0,   0,  16, 216,   0,   0, 144, 199,   0,   0, 254, 255, 255, 127, 252, 255,
    255, 255, 255, 127, 255,   0, 255, 255,   0,   0,   1,   0,   0,   0,   0,   3,
      0,   0,   0, 240,  15,   0,   0,   0, 255,  63, 229, 127, 101, 252, 255, 255,
    255,  63, 255, 255, 127,   0, 248, 160, 255, 255, 127,  95, 219, 255, 255, 255,
    255, 255,   3,   0,   0,   0, 248, 255,   0,   0, 255, 255, 255,   0,   0,   0,
      0,   0, 255,  31,   0,   0, 255,   3, 159, 255, 247, 255, 127,  15, 215, 255,
    255, 255, 255,  31, 254, 255, 255, 255, 252, 252, 252,  28, 127, 127,   0,   0,
      0,   0,   0,  20,   0,   8,   0,   0,   0, 192,   0,   0,   0,  24,   0,   0,
      0,   0,   0,  88,   0,   0,   0,  12,   0,   0,   0, 192, 255, 255, 255, 223,
    100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,
     95, 252, 253, 255, 255, 207, 255, 255, 150, 254, 247,  10, 132, 234, 150, 170,
    150, 247, 247,  94, 255, 251, 255,  15, 238, 251, 255,  15, 255,   7, 255, 255,
    255, 127, 255, 255, 255, 255,   0,   0,   0,  12,   0,   0,   7,   0, 255, 255,
    255,   1,   3,   0,
};

/* NFKD_Quick_Check: 1320 bytes. */

RE_UINT32 re_get_nfkd_quick_check(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 14;
    code = ch ^ (f << 14);
    pos = (RE_UINT32)re_nfkd_quick_check_stage_1[f] << 3;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_nfkd_quick_check_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_nfkd_quick_check_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_nfkd_quick_check_stage_4[pos + f] << 5;
    pos += code;
    value = (re_nfkd_quick_check_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* NFKC_Quick_Check. */

static RE_UINT8 re_nfkc_quick_check_stage_1[] = {
     0,  1,  2,  3,  4,  4,  4,  4,  4,  4,  5,  4,  4,  4,  4,  6,
     4,  7,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  8,  9, 10,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 11,
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
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
};

static RE_UINT8 re_nfkc_quick_check_stage_2[] = {
     0,  1,  2,  3,  0,  4,  5,  6,  0,  0,  0,  7,  8,  0,  0,  0,
     0,  0,  9, 10, 11,  0, 12, 13, 14, 15, 13, 16, 17, 18, 19, 20,
    21, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 25,  0,  0,  0, 26, 27,  0, 28, 29, 30,
    31, 32, 33, 34, 35,  0, 36,  0, 37, 38,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0, 39, 40,  0,  0, 41,  0, 42,  0,  0, 43, 44, 45,
    46, 47, 48, 49, 50, 51, 44, 44,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 52, 53, 54,
     0,  0,  0,  0,  0,  0, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0, 44, 44, 56, 57, 58, 59, 44, 44, 60, 61, 62, 63, 64, 65,
     0, 66, 67,  0,  0,  0, 13,  0,  0, 68,  0, 69,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0, 70, 71,  0,  0,  0,  0, 72, 73, 74, 44, 44, 75, 44, 76,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,
     0,  0, 79, 80, 81,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    44, 44, 44, 44, 82,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_nfkc_quick_check_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   0,   0,   0,   0,
      0,   0,   0,   3,   4,   0,   0,   5,   0,   0,   0,   0,   6,   0,   0,   7,
      0,   0,   0,   8,   0,   9,  10,   0,  11,  12,  13,  14,  15,   0,   0,  16,
     17,   0,   0,   0,   0,  18,   0,  19,  20,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,  21,   0,  22,   0,   0,   0,  23,   0,  24,   0,   0,
      0,   0,   0,  25,   0,  26,   0,   0,   0,   0,   0,  27,   0,  28,   0,   0,
      0,   0,   0,  25,   0,  29,   0,   0,   0,   0,   0,  25,   0,  30,   0,   0,
      0,   0,   0,   0,   0,  31,   0,   0,   0,   0,   0,   0,  32,  33,   0,   0,
      0,   0,   0,   0,  34,  35,   0,   0,   0,   0,   0,  36,   0,   0,   0,   0,
      0,   0,   0,  36,   0,  37,   0,   0,  38,   0,   0,   0,  39,  40,  41,  42,
     43,  39,  40,  41,   0,   0,   0,   0,   0,   0,  25,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  38,   0,   0,   0,   0,   0,   0,  44,  45,
      0,   0,  46,  47,  48,   0,   0,   0,   0,   0,   0,  49,   0,   0,   0,   0,
      0,   0,  50,  51,  52,  53,  54,  55,   0,  56,  53,  53,   0,   0,   0,   0,
      0,  57,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  58,
      0,   0,   0,  59,  60,  61,  61,  62,  54,  63,  64,  65,  66,  67,   0,  68,
     69,  70,  55,   0,   0,   0,   0,   0,  71,  72,  73,  74,  75,  53,  53,  53,
     41,   0,   0,   0,   0,   0,   0,   0,   0,   0,  76,  77,   0,   0,   0,   0,
      0,   0,  78,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  53,  53,
     53,  53,  53,  53,  53,  53,  54,   0,  38,   0,   0,   0,   0,   0,   0,  79,
      0,   0,   0,   0,   0,  38,   0,   0,   0,   0,   0,   0,   0,   0,   0,  37,
      0,   0,   0,   0,   0,   0,   5,   0,   0,   5,   0,   0,   0,   0,   0,  36,
     53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  80,   0,   0,
     77,   0,   0,  81,   0,   0,   0,   0,   0,  82,   0,   0,   0,   0,   0,   5,
      0,   0,   0,  83,  53,  53,  53,  53,  69,  84,   0,   0,   0,   0,   0,   0,
     53,  69,  53,  53,  85,  53,  53,  69,  53,  53,  53,  53,  53,  53,  53,  69,
      0,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  77,
      0,   0,   0,   0,   0,   0,   0,  86,   0,   0,   0,   0,   0,  87,   0,   0,
     88,  89,  90,  53,  53,  53,  88,  53,  53,  53,  53,  53,  53,  91,   0,   0,
     18,  92,  53,  93,  94,  53,  53,  53,  53,  53,  53,  95,   0,  96,  53,  53,
     53,  53,  53,  88,   0,  53,  53,  53,  53,  84,  53,  53,  85,   0,   0,  70,
      0,  91,   0,  53,  97,  98,  99, 100,  53,  53,  53,  53,  53,  53,  53,  70,
     83,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  69, 101, 102, 103,   0,
      0,   0,   0, 104,   0,   0,   0,   0,   0,   0,  30,   0,   0,   0,   0,   0,
      0,   0,   0, 105,   0,   0,   0,   0,   0,   0,  35,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 106,  10,   0,   0,   0,   0,  56,  77,   0,   0,   0,
     53,  53,  53,  53,  53, 107,  53,  53,  53, 108, 109, 110, 111,  53,  53,  53,
    112, 113,  53, 114, 115, 116,  53,  53,  53,  53, 117,  53,  53,  53,  53,  53,
     53,  53,  53,  53, 118,  53,  53,  53, 111,  53, 119, 120, 121, 122, 123, 124,
    125, 126, 127, 126,   0,   0,   0,   0,  54,  53,  69,  53,  53,   0,  57,   0,
      0,  77,   0,   0,   0,   0,   0,   0, 128,  53,  53, 126,   8,  95,   0,   0,
     53,  88,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_nfkc_quick_check_stage_4[] = {
     0,  0,  0,  0,  1,  0,  2,  3,  4,  5,  6,  6,  4,  0,  0,  3,
     1,  0,  7,  0,  0,  0,  0,  3,  0,  8,  8,  1,  9,  0,  0,  0,
     8,  8,  1,  0,  0,  0,  8,  5,  8,  1,  0,  0, 10, 11, 10, 12,
    13, 14, 15,  0, 15, 10, 14, 16, 17,  0, 14,  0, 18, 19,  0,  0,
     0,  1, 20, 20,  0, 21,  0,  0,  8,  6,  0,  0,  6,  5,  7,  0,
     0,  3,  0,  0, 15, 17,  0,  0,  0,  9,  1,  0,  0,  0,  0, 14,
     0,  0,  8,  8,  0,  0,  0, 22,  0, 15,  0, 21,  3, 20,  0,  0,
     0,  0,  9, 20,  0, 23,  0,  5,  0, 15,  0,  0,  0, 22,  0,  0,
    22,  0,  0,  0,  0, 16,  0,  0,  0,  0, 22, 15,  0,  0,  0, 15,
     3,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,  1,  3,  0,  0,  7,
    20,  3,  0,  1,  0,  0,  7,  0,  3,  9,  5,  0,  7,  0,  0,  0,
    24, 10, 10, 10, 10, 17,  0,  0,  0,  0, 10, 10, 10, 10, 10, 10,
    25,  0,  0,  0,  0, 26,  0,  0,  0,  0,  0,  6,  8,  8,  6,  8,
     8,  8,  8, 21,  8,  8,  8,  8,  8,  8,  6,  0,  0,  0,  1,  0,
     0,  0,  3,  8,  0,  0,  4,  0, 27, 27, 27,  7,  0,  0,  3,  9,
     5,  0, 27,  9,  3,  0,  3,  9,  0,  0, 27, 28,  7,  3,  0,  0,
     0,  6,  0,  3,  3, 29,  0,  2,  0,  3,  5,  0,  0,  3,  0,  3,
     5,  8,  8,  8,  8,  8,  8,  6,  8,  8,  8,  1,  8,  9,  9,  8,
     8, 28,  9,  5,  6,  2, 29, 21, 21,  8, 21,  8,  1,  9,  5,  0,
     0,  0,  0, 21,  1,  0,  0,  0,  0,  0, 28,  0,  0,  6,  0,  0,
     8,  5,  0,  0,  0, 20,  6,  0,  0,  0, 30, 31,  9,  8,  8,  8,
     4,  8,  8,  8,  8,  8,  0,  0,  0,  0,  5,  0,  0,  0,  0,  8,
     8,  8,  8,  5,  2,  9,  8,  6,  2, 28,  4,  8,  8,  8,  5,  0,
     3,  8,  0, 27,  8,  6,  8,  2, 21, 29,  8,  8,  5,  0,  0,  0,
     3,  8,  8,  8,  8, 31,  8,  8,  6,  8,  8,  8,  8,  6,  8,  0,
     6, 29,  8,  8,  4,  8,  4,  8,  4,  8,  4,  1,  8,  6,  8,  6,
     0,  0, 22,  0, 14,  0, 22, 26,  0,  0,  0,  4,  8, 29,  8,  8,
     8,  8,  8, 29, 20, 28,  9, 29,  8,  8, 21,  9,  8,  9,  8,  8,
     8, 21,  6,  9,  8, 29,  8, 29,  8,  8, 21,  6,  8,  2,  4,  8,
    29,  8,  8,  8,  8,  5,  8,  8,  8,  8,  8,  4, 28, 31,  9,  8,
     6,  8, 27,  0, 20,  3, 27,  9, 28, 31, 27, 27, 28, 31,  6,  8,
     6,  8,  9,  2,  8,  8, 21,  8,  8,  8,  8,  0,  9,  9, 21,  8,
     6,  0,  0,  0,
};

static RE_UINT8 re_nfkc_quick_check_stage_5[] = {
    0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1,
    0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0,
    1, 1, 1, 1, 0, 1, 1, 1, 2, 2, 2, 2, 2, 0, 2, 2,
    2, 0, 0, 2, 0, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2,
    0, 2, 2, 0, 2, 2, 0, 0, 1, 1, 2, 1, 1, 2, 0, 0,
    0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 2, 0, 0, 0, 2, 2,
    0, 2, 2, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 1, 0, 1,
    0, 1, 1, 0, 1, 0, 1, 1, 0, 2, 2, 1, 1, 0, 0, 1,
};

/* NFKC_Quick_Check: 1964 bytes. */

RE_UINT32 re_get_nfkc_quick_check(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 12;
    code = ch ^ (f << 12);
    pos = (RE_UINT32)re_nfkc_quick_check_stage_1[f] << 5;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_nfkc_quick_check_stage_2[pos + f] << 3;
    f = code >> 4;
    code ^= f << 4;
    pos = (RE_UINT32)re_nfkc_quick_check_stage_3[pos + f] << 2;
    f = code >> 2;
    code ^= f << 2;
    pos = (RE_UINT32)re_nfkc_quick_check_stage_4[pos + f] << 2;
    value = re_nfkc_quick_check_stage_5[pos + code];

    return value;
}

/* Alphanumeric. */

static RE_UINT8 re_alphanumeric_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3,
};

static RE_UINT8 re_alphanumeric_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 13, 13, 13, 14,
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 13, 28, 29, 30, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 31,  7, 32, 33,  7, 34,  7,  7,  7, 35, 13, 36,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_alphanumeric_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,   1,  17,  18,  19,   1,  20,  21,  22,  23,  24,  25,  26,  27,   1,  28,
     29,  30,  31,  31,  32,  31,  31,  31,  31,  31,  31,  31,  33,  34,  35,  31,
     36,  37,  31,  31,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,  38,   1,   1,   1,   1,   1,   1,   1,   1,   1,  39,
      1,   1,   1,   1,  40,   1,  41,  42,  43,  44,  45,  46,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  47,  31,  31,  31,  31,  31,  31,  31,  31,
     31,   1,  48,  49,   1,  50,  51,  52,  53,  54,  55,  56,  57,  58,   1,  59,
     60,  61,  62,  63,  64,  31,  31,  31,  65,  66,  67,  68,  69,  70,  71,  72,
     73,  31,  74,  31,  75,  76,  31,  31,   1,   1,   1,  77,  78,  79,  31,  31,
      1,   1,   1,   1,  80,  31,  31,  31,  31,  31,  31,  31,   1,   1,  81,  31,
      1,   1,  82,  83,  31,  31,  31,  84,   1,   1,   1,   1,   1,   1,   1,  85,
      1,   1,  86,  31,  31,  31,  31,  31,   1,  87,  88,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  89,  31,  31,  31,  31,  31,  31,  31,  90,  91,  92,  93,
     94,  31,  31,  31,  31,  31,  31,  31,  95,  96,  31,  31,  31,  31,  97,  31,
     31,  98,  31,  31,  31,  31,  31,  31,   1,   1,   1,   1,   1,   1,  99,   1,
      1,   1,   1,   1,   1,   1,   1, 100, 101,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1, 102,   1,   1,   1,   1, 103,  31,  31,  31,  31,
      1,   1, 104,  31,  31,  31,  31,  31,
};

static RE_UINT8 re_alphanumeric_stage_4[] = {
      0,   1,   2,   2,   0,   3,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
      5,   5,   5,   5,   5,   5,   6,   7,   0,   0,   8,   9,  10,  11,   5,  12,
      5,   5,   5,   5,  13,   5,   5,   5,   5,  14,  15,  16,  17,  18,  19,  20,
     21,   5,  22,  23,   5,   5,  24,  25,  26,   5,  27,   5,   5,  28,   5,  29,
     30,  31,  32,  33,   0,  34,  35,  36,   5,  37,  38,  39,  40,  41,  42,  43,
     44,  45,  46,  47,  48,  49,  50,  51,  52,  49,  53,  54,  55,  56,  57,  58,
     59,  60,  61,  62,  59,  63,  64,  65,  59,  66,  67,  68,  69,  70,  71,  72,
     16,  73,  74,   0,  75,  76,  77,   0,  78,  79,  80,  81,  82,  83,   0,   0,
      5,  84,  85,  86,  87,   5,  88,  89,   5,   5,  90,   5,  91,  92,  93,   5,
     94,   5,  95,   0,  96,   5,   5,  97,  16,   5,   5,   5,   5,   5,   5,   5,
      5,   5,   5,  98,   2,   5,   5,  99, 100, 101, 101, 102,   5, 103, 104,  79,
      1,   5,   5, 105,   5, 106,   5, 107, 108, 109, 110, 111,   5, 112, 113,   0,
    114,   5, 108, 115, 113, 116,   0,   0,   5, 117, 118,   0,   5, 119,   5, 120,
      5, 107, 121, 122, 123,   0,   0, 124,   5,   5,   5,   5,   5,   5,   0, 125,
     97,   5, 126, 122,   5, 127, 128, 129,   0,   0,   0, 130, 131,   0,   0,   0,
    132, 133, 134,   5, 123,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 135,   5,  79,   5, 136, 108,   5,   5,   5,   5, 137,
      5,  88,   5, 138, 139, 140, 140,   5,   0, 141,   0,   0,   0,   0,   0,   0,
    142, 143,  16,   5, 144,  16,   5,  89, 145, 146,   5,   5, 147,  73,   0,  26,
      5,   5,   5,   5,   5, 107,   0,   0,   5,   5,   5,   5,   5,   5,   5,  33,
      5,   5,   5,   5,  31,   0,  26, 122, 148, 149,   5, 150,   5,   5,   5,  96,
    151, 152,   5,   5, 153, 154,   0, 151, 155,  17,   5, 101,   5,   5, 156, 157,
      5, 106, 158,  83,   5, 159, 160, 161,   5, 139, 162, 163,   5, 108, 164, 165,
    166, 167,  89, 168,   5,   5,   5, 169,   5,   5,   5,   5,   5, 170, 171, 114,
      5,   5,   5, 172,   5,   5, 173,   0, 174, 175, 176,   5,   5,  28, 177,   5,
      5, 122,  26,   5, 178,   5,  17, 179,   0,   0,   0, 180,   5,   5,   5,  83,
      1,   2,   2, 110,   5, 108, 181,   0, 182, 183, 184,   0,   5,   5,   5,  73,
      0,   0,   5, 185,   0,   0,   0,   0,   0,   0,   0,   0,  83,   5, 186,   0,
      5,  27, 106,  73, 122,   5, 187,   0,   5,   5,   5,   5, 122,  85, 188, 114,
      5, 189,   5, 190,   0,   0,   0,   0,   5, 139, 107,  17,   0,   0,   0,   0,
    191, 192, 107, 139, 108,   0,   0, 193, 107, 173,   0,   0,   5, 194,   0,   0,
    195, 101,   0,  83,  83,   0,  80, 196,   5, 107, 107, 158,  28,   0,   0,   0,
      5,   5, 123,   0,   5, 158,   5, 158,   5,   5, 197,  58, 152,  32,  26, 198,
      5, 199,  26, 200,   5,   5, 201,   0, 202, 203,   0,   0, 204, 205,   5, 198,
     40,  49, 206, 190,   0,   0,   0,   0,   5,   5, 207,   0,   5,   5, 208,   0,
      0,   0,   0,   0,   5, 209, 210,   0,   5, 108, 211,   0,   5, 107,  79,   0,
    212, 169,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,   5, 213,
      5, 214,  26,   5, 215,   0,   5,  32, 216, 217, 218, 219, 178, 220,   0,   0,
    221, 222, 223,   0,   0,   0,   0,   0,   5,   5,   5,   5, 173,   0,   0,   0,
      5,   5,   5, 147,   5,   5,   5,   5,   5,   5, 190,   0,   0,   0,   0,   0,
      5, 147,   0,   0,   0,   0,   0,   0,   5,   5, 224,   0,   0,   0,   0,   0,
      5,  32, 108,  79,   0,   0,  26, 225,   5, 139, 226, 227,  96,   0,   0,   0,
      5,   5, 228, 108, 177,   0,   0, 229,   5,   5,   5,   5,   5,   5,   5,  31,
      5,   5,   5,   5,   5,   5,   5, 158, 108,   0,   0,  26,   5,   5,   5,   5,
      5,   5,   5,   5,   5,   5,   5, 114,   5,   5,   5, 230, 231,   0,   0,   0,
      5,   5, 232,   5, 233, 234, 235,   5, 236, 237, 238,   5,   5,   5,   5,   5,
      5,   5,   5,   5,   5, 239, 240,  89, 232, 232, 136, 136, 216, 216, 241,   5,
    242, 243,   0,   0,   0,   0,   0,   0,   5,   5,   5,   5,   5,   5, 196,   0,
      5,   5, 244,   0,   0,   0,   0,   0, 235, 245, 246, 247, 248, 249,   0,   0,
      0,  26,  85,  85,  79,   0,   0,   0,   5,   5,   5,   5,   5,   5, 139,   0,
      5, 185,   5,   5,   5,   5,   5,   5, 122,   5,   5,   5,   5,   5,   5,   5,
      5,   5,   5,   5,   5, 250,   5,   5,   5,   5,   5,   5,   5,   5,   5,  78,
    122,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_alphanumeric_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 254, 255, 255,   7,   0,   4,  32,   4,
    255, 255, 127, 255, 255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,
     32,   0,   0,   0,   0,   0, 223, 188,  64, 215, 255, 255, 251, 255, 255, 255,
    255, 255, 191, 255,   3, 252, 255, 255, 255, 255, 254, 255, 255, 255, 127,   2,
    254, 255, 255, 255, 255,   0,   0,   0,   0,   0, 255, 191, 182,   0, 255, 255,
    255,   7,   7,   0,   0,   0, 255,   7, 255, 255, 255, 254, 255, 195, 255, 255,
    255, 255, 239,  31, 254, 225, 255, 159,   0,   0, 255, 255,   0, 224, 255, 255,
    255, 255,   3,   0, 255,   7,  48,   4, 255, 255, 255, 252, 255,  31,   0,   0,
    255, 255, 255,   1, 255,   7,   0,   0, 255, 255, 223,  63,   0,   0, 240, 255,
    248,   3, 255, 255, 255, 255, 255, 239, 255, 223, 225, 255, 207, 255, 254, 255,
    239, 159, 249, 255, 255, 253, 197, 227, 159,  89, 128, 176, 207, 255,   3,  16,
    238, 135, 249, 255, 255, 253, 109, 195, 135,  25,   2,  94, 192, 255,  63,   0,
    238, 191, 251, 255, 255, 253, 237, 227, 191,  27,   1,   0, 207, 255,   0,  30,
    238, 159, 249, 255, 159,  25, 192, 176, 207, 255,   2,   0, 236, 199,  61, 214,
     24, 199, 255, 195, 199,  29, 129,   0, 192, 255,   0,   0, 239, 223, 253, 255,
    255, 253, 255, 227, 223,  29,  96,   7, 207, 255,   0,   0, 255, 253, 239, 227,
    223,  29,  96,  64, 207, 255,   6,   0, 255, 255, 255, 231, 223,  93, 240, 128,
    207, 255,   0, 252, 236, 255, 127, 252, 255, 255, 251,  47, 127, 128,  95, 255,
    192, 255,  12,   0, 255, 255, 255,   7, 127,  32, 255,   3, 150,  37, 240, 254,
    174, 236, 255,  59,  95,  32, 255, 243,   1,   0,   0,   0, 255,   3,   0,   0,
    255, 254, 255, 255, 255,  31, 254, 255,   3, 255, 255, 254, 255, 255, 255,  31,
    255, 255, 127, 249, 255,   3, 255, 255, 231, 193, 255, 255, 127,  64, 255,  51,
    191,  32, 255, 255, 255, 255, 255, 247, 255,  61, 127,  61, 255,  61, 255, 255,
    255, 255,  61, 127,  61, 255, 127, 255, 255, 255,  61, 255, 255, 255, 255, 135,
    255, 255,   0,   0, 255, 255,  63,  63, 255, 159, 255, 255, 255, 199, 255,   1,
    255, 223,  15,   0, 255, 255,  15,   0, 255, 223,  13,   0, 255, 255, 207, 255,
    255,   1, 128,  16, 255, 255, 255,   0, 255,   7, 255, 255, 255, 255,  63,   0,
    255, 255, 255, 127, 255,  15, 255,   1, 192, 255, 255, 255, 255,  63,  31,   0,
    255,  15, 255, 255, 255,   3, 255,   3, 255, 255, 255,  15, 254, 255,  31,   0,
    128,   0,   0,   0, 255, 255, 239, 255, 239,  15, 255,   3, 255, 243, 255, 255,
    191, 255,   3,   0, 255, 227, 255, 255, 255, 255, 255,  63, 255,   1,   0,   0,
      0, 222, 111,   0, 128, 255,  31,   0,  63,  63, 255, 170, 255, 255, 223,  95,
    220,  31, 207,  15, 255,  31, 220,  31,   0,   0,   2, 128,   0,   0, 255,  31,
    132, 252,  47,  62,  80, 189, 255, 243, 224,  67,   0,   0,   0,   0, 192, 255,
    255, 127, 255, 255,  31, 120,  12,   0, 255, 128,   0,   0, 255, 255, 127,   0,
    127, 127, 127, 127,   0, 128,   0,   0, 224,   0,   0,   0, 254,   3,  62,  31,
    255, 255, 127, 224, 224, 255, 255, 255, 255, 127, 254, 255, 255, 127,   0,   0,
    255,  31, 255, 255, 255,  15,   0,   0, 255, 127, 240, 143,   0,   0, 128, 255,
    252, 255, 255, 255, 255, 249, 255, 255, 255, 127, 255,   0, 187, 247, 255, 255,
     47,   0, 255,   3,   0,   0, 252,  40, 255, 255,   7,   0, 255, 255, 247, 255,
      0, 128, 255,   3, 223, 255, 255, 127, 255,  63, 255,   3, 255, 255, 127, 196,
      5,   0,   0,  56, 255, 255,  60,   0, 126, 126, 126,   0, 127, 127, 255, 255,
     63,   0, 255, 255, 255,   7, 255,   3,  15,   0, 255, 255, 127, 248, 255, 255,
    255,  63, 255, 255, 255, 255, 255,   3, 127,   0, 248, 224, 255, 253, 127,  95,
    219, 255, 255, 255,   0,   0, 248, 255, 255, 255, 252, 255,   0,   0, 255,  15,
      0,   0, 223, 255, 252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183,
    255,  63, 255,  63, 255, 255,  31,   0, 255, 255,   1,   0,  15, 255,  62,   0,
    255, 255,  15, 255, 255,   0, 255, 255,  15,   0,   0,   0,  63, 253, 255, 255,
    255, 255, 191, 145, 255, 255,  55,   0, 255, 255, 255, 192, 111, 240, 239, 254,
     31,   0,   0,   0,  63,   0,   0,   0, 255,   1, 255,   3, 255, 255, 199, 255,
    255, 255,  71,   0,  30,   0, 255,  23, 255, 255, 251, 255, 255, 255, 159,  64,
    127, 189, 255, 191, 255,   1, 255, 255, 159,  25, 129, 224, 187,   7, 255,   3,
    179,   0, 255,   3, 255, 255,  63, 127,   0,   0,   0,  63,  17,   0, 255,   3,
    255, 255, 255, 227, 255,   3,   0, 128, 255, 255, 231, 127, 207, 255, 255,   0,
    255, 253, 255, 255, 255, 255, 127, 127,   1,   0, 255,   3,   0,   0, 252, 255,
    255, 254, 127,   0, 127, 251, 255, 255, 255, 255, 127, 180, 203,   0, 255,   3,
    127,   0,   0,   0, 255,  63,   0,   0,  15,   0, 255,   3, 248, 255, 255, 224,
     31,   0, 255, 255,   3,   0,   0,   0, 255,   7, 255,  31, 255,   1, 255,  67,
    255, 255, 223, 255, 255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255,
    191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255,
    253, 255, 255, 247, 247, 207, 255, 255, 127, 255, 255, 249, 219,   7,   0,   0,
    143,   0, 255,   3, 150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94,
    255, 251, 255,  15, 238, 251, 255,  15,   3,   0, 255, 255,
};

/* Alphanumeric: 2285 bytes. */

RE_UINT32 re_get_alphanumeric(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_alphanumeric_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_alphanumeric_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_alphanumeric_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_alphanumeric_stage_4[pos + f] << 5;
    pos += code;
    value = (re_alphanumeric_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Any. */

RE_UINT32 re_get_any(RE_UINT32 ch) {
    return 1;
}

/* Blank. */

static RE_UINT8 re_blank_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_blank_stage_2[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_blank_stage_3[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_blank_stage_4[] = {
    0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 3, 1, 1, 1, 1, 1, 4, 5, 1, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_blank_stage_5[] = {
      0,   2,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   1,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,
    255,   7,   0,   0,   0, 128,   0,   0,   0,   0,   0, 128,   0,   0,   0,   0,
};

/* Blank: 169 bytes. */

RE_UINT32 re_get_blank(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_blank_stage_1[f] << 3;
    f = code >> 13;
    code ^= f << 13;
    pos = (RE_UINT32)re_blank_stage_2[pos + f] << 4;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_blank_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_blank_stage_4[pos + f] << 6;
    pos += code;
    value = (re_blank_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Graph. */

static RE_UINT8 re_graph_stage_1[] = {
    0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 4, 8,
    4, 8,
};

static RE_UINT8 re_graph_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13,  7,  7,  7, 14,
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 28, 29, 30, 31, 32,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 33,  7, 34, 35,  7, 36,  7,  7,  7, 37, 13, 38,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    39, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 40,
};

static RE_UINT8 re_graph_stage_3[] = {
      0,   1,   1,   2,   1,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,
     14,   1,  15,  16,   1,   1,  17,  18,  19,  20,  21,  22,  23,  24,   1,  25,
     26,  27,   1,   1,  28,   1,   1,   1,   1,   1,   1,  29,  30,  31,  32,  33,
     34,  35,  36,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,  37,   1,   1,   1,   1,   1,   1,   1,   1,   1,  38,
      1,   1,   1,   1,  39,   1,  40,  41,  42,  43,  44,  45,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  46,  47,  47,  47,  47,  47,  47,  47,  47,
      1,   1,  48,  49,   1,  50,  51,  52,  53,  54,  55,  56,  57,  58,   1,  59,
     60,  61,  62,  63,  64,  47,  65,  47,  66,  67,  68,  69,  70,  71,  72,  73,
     74,  47,  75,  47,  76,  77,  47,  47,   1,   1,   1,  78,  79,  80,  47,  47,
      1,   1,   1,   1,  81,  47,  47,  47,  47,  47,  47,  47,   1,   1,  82,  47,
      1,   1,  83,  84,  47,  47,  47,  85,   1,   1,   1,   1,   1,   1,   1,  86,
      1,   1,  87,  47,  47,  47,  47,  47,   1,  88,  89,  47,  47,  47,  47,  47,
     47,  47,  47,  47,  90,  47,  47,  47,  91,  92,  93,  94,  95,  96,  97,  98,
      1,   1,  99,  47,  47,  47,  47,  47, 100,  47,  47,  47,  47,  47,  47,  47,
    101, 102,  47,  47,  47,  47, 103,  47, 104, 105, 106,   1,   1,   1, 107, 108,
    109, 110,  47,  47,  47,  47,  47,  47,   1,   1,   1,   1,   1,   1, 111,   1,
      1,   1,   1,   1,   1,   1,   1, 112, 113,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1, 114,   1,   1,   1,   1, 115,  47,  47,  47,  47,
      1,   1, 116,  47,  47,  47,  47,  47, 117, 118,  47,  47,  47,  47,  47,  47,
      1,   1,   1,   1,   1,   1,   1, 119,
};

static RE_UINT8 re_graph_stage_4[] = {
      0,   1,   2,   3,   0,   1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   4,   5,   6,   2,   2,   2,   7,   8,   1,   9,   2,  10,  11,
     12,   2,   2,   2,   2,   2,   2,   2,  13,   2,  14,   2,   2,  15,   2,  16,
      2,  17,  18,  19,   0,  20,  21,   2,   2,   2,   2,   2,  22,  23,  24,  25,
     26,  27,  28,  29,  30,  31,  32,  33,  34,  31,  35,  36,  37,  38,  39,  40,
     41,  42,  43,  44,  41,  45,  46,  47,  41,   2,  48,  49,  50,  51,  52,  53,
      1,  54,  55,   0,  56,  57,  58,   0,   2,   2,  59,  60,  61,  12,  62,   0,
      2,   2,   2,   2,   2,   2,  63,   2,   2,   2,  64,   2,  65,  66,  67,   2,
     68,   2,  69,  70,  71,   2,   2,  72,   2,   2,   2,   2,  73,   2,   2,  74,
     75,  76,  77,  78,   2,   2,  79,  80,  81,   2,   2,  82,   2,  83,   2,  84,
      3,  85,  86,  87,   2,  88,  89,   2,  90,   2,   3,  91,  80,  17,   0,   0,
      2,   2,  88,  70,   2,   2,   2,  92,   2,  93,  94,   2,  95,   0,  10,  71,
      2,   2,   2,   2,   2,   2,   2,  96,  72,   2,  97,  79,   2,  98,  99, 100,
    101, 102,   3, 103, 104,   2, 105, 106,   2,   2,   2,   2,  88,   2,   2,   2,
      2, 107,  19,   2,   2,   2,   2,   2,   2,   2,   2, 108, 109, 110, 111, 112,
      2, 113,   3,   2,   2,   2,   2, 114,   2,  63,   2, 115,  76, 116, 116,   2,
      2,   2, 117,   0,  96,   2,   2,  77,   2,   2,   2,   2,   2,   2,  84, 118,
      1,   2,   1,   2,   8,   2,   2,   2, 119, 120,   2,   2, 113,  16,   2, 121,
      3,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,  84,   2,   2,
      2,   2,   2,   2,   2,   2,   2,  19,   2,   2,   2,   2, 122,   2, 123,   2,
      2, 124,   2,   2,   2,   2,   2,  82,   2,   2,   2,   2,   2, 125,   0, 126,
      2, 127,   2,  82,   2,   2, 128,  79,   2,   2, 129,  70,   2,   2, 130,   3,
      2,  76, 131,   2,   2,   2, 132,  76, 133, 134,   2, 135,   2,   2,   2, 136,
      2,   2,   2,   2,   2, 121, 137,  55,   0,   0,   0,   0,   0,   0,   0,   0,
      2,   2,   2, 138,   2,   2,  71,   0, 139, 140, 141,   2,   2,   2, 142,   2,
      2,   2, 105,   2, 143,   2, 144, 145,  71,   2, 146, 147,   2,   2,   2,  91,
      1,   2,   2,   2,   2,   3, 148, 149, 150, 151, 152,   0,   2,   2,   2,  16,
    153, 154,   2,   2, 155, 156, 105,  79,   0,   0,   0,   0,  70,   2, 106,  55,
      2, 157,  83,  16, 158,   2, 159,   0,   2,   2,   2,   2,  79, 160, 161,  55,
      2,  10,   2, 162,   0,   0,   0,   0,   2,  76,  84, 144,   0,   0,   0,   0,
    163, 164, 165,   2,   3, 166,   0, 167, 168, 169,   0,   0,   2, 170, 143,   2,
    171, 172, 173,   2,   2,   0,   2, 174,   2, 175, 109, 176, 177, 178,   0,   0,
      2,   2,  95,   0,   2, 179,   2, 180,   0,   0,   0,   3,   0,   0,   0,   0,
      2,   2, 181, 182,   2,   2, 183, 184,   2,  98, 121,  76,   2,   2, 138, 185,
    186,   3,   0,   0, 187, 160,   2, 188,  22,  31, 189, 190,   0,   0,   0,   0,
      2,   2, 191,   0,   2,   2, 192,   0,   0,   0,   0,   0,   2, 109,  79,   0,
      2,   2, 193, 194,   2,  82, 117,   0, 110,  88,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   2,   2, 195,   2,   2,  10,   2, 196, 197,   2,  74,
    198, 199, 135, 122, 143, 200,   0,   0, 201, 202, 192,   0,   0,   0,   0,   0,
      2,   2,   2,   2,  71,   0,   0,   0,   2,   2,   2, 203,   2,   2,   2,   2,
      2,   2, 204,   0,   0,   0,   0,   0,   2, 205,   0,   0,   0,   0,   0,   0,
      2,   2, 107,   0,   0,   0,   0,   0,   2,  74,   3, 206,   0,   0, 105, 207,
      2,   2, 208, 209, 210,   0,   0,   0,   2,   2, 211,   3, 212,   0,   0, 213,
      2,   2,   2,   2,   2,   2,   2, 194,   2,   2,   2,   2,   2,   2,   2, 179,
      3,   0,   0, 105,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  55,
      2,   2,   2, 214, 215, 204,   0,   0,   2,   2,   2,   2,   2,   2,   2,  84,
      2, 216,   2,   2,   2,   2,   2,  95,   2,   2, 217,   0,   0,   0,   0,   0,
      2,   2,  76,  15,   0,   0,   0,   0,   2,   2,  98,   2,  12, 218, 219,   2,
    220, 221, 222,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 223,   2,   2,
      2,   2,   2,   2,   2,   2, 224,   2,   2,   2,   2,   2, 225, 226,   0,   0,
    227, 228,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2, 229,   0,
      2,   2, 230,   0,   0,   0,   0,   0, 219, 231, 232, 233, 234, 235,   0, 236,
      2,  88,   2,   2,  77, 120, 237,  84, 122, 113,   2,  88,   2, 194,   0, 238,
    239,  55, 240, 217,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2, 241, 242,
      2,   2,   2,  77,   2,   2, 241,   0,  88,   2, 192,   2,  10, 243,   0,   0,
     88,   3, 122, 124,  82,   0, 244, 107,   2,   2,   2,   2,   2,   2,  76,   0,
      2, 241,   2,   2,   2,   2,   2,   2,  79,   2,   2,   2,   2,   2,   2,   2,
      2,   2,   2,   2,   2, 183,   2,   2,   2,   2,   2,   2,   2,   2,   2, 156,
     79,   0,   0,   0,   0,   0,   0,   0, 245,   2,   2,   2,   0,   0,   0,   0,
      2,   2,   2,   2,   2,   2,   2, 210,   2,   2,   2,   2,   2,   2,   2,  79,
};

static RE_UINT8 re_graph_stage_5[] = {
      0,   0,   0,   0, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,
    255, 255, 255, 252, 240, 215, 255, 255, 251, 255, 255, 255, 255, 255, 254, 255,
    255, 255, 127, 254, 255, 230, 254, 255, 255,   0, 255, 255, 255,   7,  31,   0,
    255, 255, 255, 223, 255, 191, 255, 255, 255, 231, 255, 255, 255, 255,   3,   0,
    255, 255, 255,   7, 255,  63, 255, 127, 255, 255, 255,  79, 255,   7,   0,   0,
    255, 255, 223,  63,   0,   0, 240, 255, 239, 159, 249, 255, 255, 253, 197, 243,
    159, 121, 128, 176, 207, 255, 255,  63, 238, 135, 249, 255, 255, 253, 109, 211,
    135,  57,   2,  94, 192, 255,  63,   0, 238, 191, 251, 255, 255, 253, 237, 243,
    191,  59,   1,   0, 207, 255,   3, 254, 238, 159, 249, 255, 159,  57, 192, 176,
    207, 255, 255,   0, 236, 199,  61, 214,  24, 199, 255, 195, 199,  61, 129,   0,
    192, 255, 255,   7, 239, 223, 253, 255, 255, 253, 255, 227, 223,  61,  96,   7,
    207, 255,   0, 255, 255, 253, 239, 243, 223,  61,  96,  64, 207, 255,   6,   0,
    223, 253, 240, 255, 207, 255, 255, 255, 236, 255, 127, 252, 255, 255, 251,  47,
    127, 132,  95, 255, 192, 255,  28,   0, 255, 255, 255, 135, 255, 255, 255,  15,
    150,  37, 240, 254, 174, 236, 255,  59,  95,  63, 255, 243, 255, 254, 255, 255,
    255,  31, 254, 255, 255, 255, 255, 254, 255, 223, 255,   7, 191,  32, 255, 255,
    255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255,
    255, 255,  61, 255, 255, 255, 255, 231, 255, 255, 255,  31, 255, 255, 255,   3,
    255, 255,  63,  63, 254, 255, 255,  31, 255, 255, 255,   1, 255, 223,  31,   0,
    255, 255, 127,   0, 255, 255,  15,   0, 255, 223,  13,   0, 255, 255, 255,  63,
    255,   3, 255,   3, 255, 127, 255,   3, 255, 255, 255,   0, 255,   7, 255, 255,
    255, 255,  63,   0, 255,  15, 255,  15, 241, 255, 255, 255, 255,  63,  31,   0,
    255,  15, 255, 255, 255,   3, 255, 199, 255, 255, 255, 207, 255, 255, 255, 159,
    255, 255,  15, 240, 255, 255, 255, 248, 255, 227, 255, 255, 255,   1,   0,   0,
    255, 255, 255, 251,  63,  63, 255, 170, 255, 255, 223, 255, 223, 255, 207, 239,
    255, 255, 220, 127,   0, 248, 255, 255, 255, 124, 255, 255, 223, 255, 243, 255,
    255, 127, 255,  31,   0,   0, 255, 255, 255, 255,   1,   0, 127,   0,   0,   0,
    255, 255, 207, 255, 255, 255,  63, 255, 255, 255, 255, 227, 255, 253,   7,   0,
      0, 240,   0,   0, 255, 127, 255, 255, 255, 255,  15, 254, 255, 128,   1, 128,
    127, 127, 127, 127, 255,   3,   0,   0,   0,   0, 255,  15, 224, 255, 255, 255,
    255, 127, 254, 255,  15,   0, 255, 255, 255,  31, 255, 255, 127,   0, 255, 255,
    255,  15,   0,   0, 255, 127, 255,   0,   0,   0, 128, 255, 255,  15, 255,   3,
     63, 192, 255,   3, 255, 255,  15, 128, 255, 191, 255, 195, 255,  63, 255, 243,
      7,   0,   0, 248, 126, 126, 126,   0, 127, 127, 255, 255,  63,   0, 255, 255,
    255,  63, 255,   3, 127, 248, 255, 255, 255,  63, 255, 255, 127,   0, 248, 224,
    255, 255, 127,  95, 219, 255, 255, 255,   3,   0, 248, 255, 255, 255, 252, 255,
    255,   0,   0,   0,   0,   0, 255,  63, 255, 255, 247, 255, 127,  15, 223, 255,
    252, 252, 252,  28, 127, 127,   0,  62, 255, 239, 255, 255, 127, 255, 255, 183,
    255,  63, 255,  63, 135, 255, 255, 255, 255, 255, 143, 255, 255, 127, 255,  15,
      1,   0,   0,   0,  15, 224, 255, 255, 255, 255, 255, 191,  15, 255,  63,   0,
    255,   3, 255, 255, 255, 255,  15, 255,  15, 128,   0,   0,  63, 253, 255, 255,
    255, 255, 191, 145, 255, 255, 191, 255, 128, 255,   0,   0, 255, 255,  55, 248,
    255, 255, 255, 143, 255, 255, 255, 131, 255, 255, 255, 240, 111, 240, 239, 254,
    255, 255,  15, 135, 255,   0, 255,   1, 127, 248, 127,   0, 255, 255,  63, 254,
    255, 255,   7, 255, 255, 255,   3,  30,   0, 254,   0,   0, 255, 255,   7,   0,
    255, 255,   7, 252, 255,  63, 252, 255, 255, 255,   0, 128,   3,   0, 255, 255,
    255,   1, 255,   3, 254, 255,  31,   0, 255, 255, 251, 255, 127, 189, 255, 191,
    255,   7, 255,   3, 159,  57, 129, 224, 207,  31,  31,   0, 255, 255, 255,  43,
    255,   0, 255,   3,  31,   0, 255,   3, 255,  31,   0,   0, 255, 255,   7, 128,
    207, 255, 255, 223,   7,   0,   0,   0, 255, 253, 255, 255, 255, 255, 127, 255,
    255, 254, 127,   0, 127, 251, 255, 255, 255, 255, 127, 180, 255, 127,  31,   0,
     15,   0,   0,   0, 255, 127,   0,   0, 255, 195,   0,   0, 255,  63,  63,   0,
     63,   0, 255, 251, 251, 255, 255, 224, 255, 255,   0,   0,  31,   0, 255, 255,
      0, 128, 255, 255,   3,   0,   0,   0, 255,   7, 255,  31, 255,   1, 255, 243,
    127, 254, 255, 255,  63,   0,   0,   0, 100, 222, 255, 235, 239, 255, 255, 255,
    191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255,
    255, 207, 255, 255, 255,  15,   0, 248, 254, 255,   0,   0, 127, 255, 255, 249,
    219,   7,   0,   0, 159, 255, 127,   0, 255,   7, 255, 195, 150, 254, 247,  10,
    132, 234, 150, 170, 150, 247, 247,  94, 255, 251, 255,  15, 238, 251, 255,  15,
      0,   0,   3,   0, 254, 255, 254, 255, 192, 255, 255, 255,   7,   0, 255, 255,
    255,   1,   3,   0, 255, 255,  31,   0, 255,  31, 255,   1, 255,  63,   0,   0,
      1,   0, 255, 255,   2,   0,   0,   0,
};

/* Graph: 2450 bytes. */

RE_UINT32 re_get_graph(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_graph_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_graph_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_graph_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_graph_stage_4[pos + f] << 5;
    pos += code;
    value = (re_graph_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Print. */

static RE_UINT8 re_print_stage_1[] = {
    0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 4, 8,
    4, 8,
};

static RE_UINT8 re_print_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13,  7,  7,  7, 14,
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 28, 29, 30, 31, 32,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 33,  7, 34, 35,  7, 36,  7,  7,  7, 37, 13, 38,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    39, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 40,
};

static RE_UINT8 re_print_stage_3[] = {
      0,   1,   1,   2,   1,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,
     14,   1,  15,  16,   1,   1,  17,  18,  19,  20,  21,  22,  23,  24,   1,  25,
     26,  27,   1,   1,  28,   1,   1,   1,   1,   1,   1,  29,  30,  31,  32,  33,
     34,  35,  36,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,  37,   1,   1,   1,   1,   1,   1,   1,   1,   1,  38,
      1,   1,   1,   1,  39,   1,  40,  41,  42,  43,  44,  45,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  46,  47,  47,  47,  47,  47,  47,  47,  47,
      1,   1,  48,  49,   1,  50,  51,  52,  53,  54,  55,  56,  57,  58,   1,  59,
     60,  61,  62,  63,  64,  47,  65,  47,  66,  67,  68,  69,  70,  71,  72,  73,
     74,  47,  75,  47,  76,  77,  47,  47,   1,   1,   1,  78,  79,  80,  47,  47,
      1,   1,   1,   1,  81,  47,  47,  47,  47,  47,  47,  47,   1,   1,  82,  47,
      1,   1,  83,  84,  47,  47,  47,  85,   1,   1,   1,   1,   1,   1,   1,  86,
      1,   1,  87,  47,  47,  47,  47,  47,   1,  88,  89,  47,  47,  47,  47,  47,
     47,  47,  47,  47,  90,  47,  47,  47,  91,  92,  93,  94,  95,  96,  97,  98,
      1,   1,  99,  47,  47,  47,  47,  47, 100,  47,  47,  47,  47,  47,  47,  47,
    101, 102,  47,  47,  47,  47, 103,  47, 104, 105, 106,   1,   1,   1, 107, 108,
    109, 110,  47,  47,  47,  47,  47,  47,   1,   1,   1,   1,   1,   1, 111,   1,
      1,   1,   1,   1,   1,   1,   1, 112, 113,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1, 114,   1,   1,   1,   1, 115,  47,  47,  47,  47,
      1,   1, 116,  47,  47,  47,  47,  47, 117, 118,  47,  47,  47,  47,  47,  47,
      1,   1,   1,   1,   1,   1,   1, 119,
};

static RE_UINT8 re_print_stage_4[] = {
      0,   1,   1,   2,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   3,   4,   5,   1,   1,   1,   6,   7,   8,   9,   1,  10,  11,
     12,   1,   1,   1,   1,   1,   1,   1,  13,   1,  14,   1,   1,  15,   1,  16,
      1,  17,  18,  19,   0,  20,  21,   1,   1,   1,   1,   1,  22,  23,  24,  25,
     26,  27,  28,  29,  30,  31,  32,  33,  34,  31,  35,  36,  37,  38,  39,  40,
     41,  42,  43,  44,  41,  45,  46,  47,  41,   1,  48,  49,  50,  51,  52,  53,
      8,  54,  55,   0,  56,  57,  58,   0,   1,   1,  59,  60,  61,  12,  62,   0,
      1,   1,   1,   1,   1,   1,  63,   1,   1,   1,  64,   1,  65,  66,  67,   1,
     68,   1,  69,  70,  71,   1,   1,  72,   1,   1,   1,   1,  70,   1,   1,  73,
     74,  75,  76,  77,   1,   1,  78,  79,  80,   1,   1,  81,   1,  82,   1,  83,
      2,  84,  85,  86,   1,  87,  88,   1,  89,   1,   2,  90,  79,  17,   0,   0,
      1,   1,  87,  70,   1,   1,   1,  91,   1,  92,  93,   1,  94,   0,  10,  71,
      1,   1,   1,   1,   1,   1,   1,  95,  72,   1,  96,  78,   1,  97,  98,  99,
      1, 100,   1, 101, 102,   1, 103, 104,   1,   1,   1,   1,  87,   1,   1,   1,
      1, 105,  19,   1,   1,   1,   1,   1,   1,   1,   1, 106, 107, 108, 109, 110,
      1, 111,   2,   1,   1,   1,   1, 112,   1,  63,   1, 113,  75, 114, 114,   1,
      1,   1, 115,   0,  95,   1,   1,  76,   1,   1,   1,   1,   1,   1,  83, 116,
      1,   1,   8,   1,   7,   1,   1,   1, 117, 118,   1,   1, 111,  16,   1, 119,
      2,   1,   1,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,  83,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  19,   1,   1,   1,   1, 120,   1, 121,   1,
      1, 122,   1,   1,   1,   1,   1,  81,   1,   1,   1,   1,   1, 123,   0, 124,
      1, 125,   1,  81,   1,   1, 126,  78,   1,   1, 127,  70,   1,   1, 128,   2,
      1,  75, 129,   1,   1,   1, 130,  75, 131, 132,   1, 133,   1,   1,   1, 134,
      1,   1,   1,   1,   1, 119, 135,  55,   0,   0,   0,   0,   0,   0,   0,   0,
      1,   1,   1, 136,   1,   1,  71,   0, 137, 138, 139,   1,   1,   1, 140,   1,
      1,   1, 103,   1, 141,   1, 142, 143,  71,   1, 144, 145,   1,   1,   1,  90,
      8,   1,   1,   1,   1,   2, 146, 147, 148, 149, 150,   0,   1,   1,   1,  16,
    151, 152,   1,   1, 153, 154, 103,  78,   0,   0,   0,   0,  70,   1, 104,  55,
      1, 155,  82,  16, 156,   1, 157,   0,   1,   1,   1,   1,  78, 158, 159,  55,
      1,  10,   1, 160,   0,   0,   0,   0,   1,  75,  83, 142,   0,   0,   0,   0,
    161, 162, 163,   1,   2, 164,   0, 165, 166, 167,   0,   0,   1, 168, 141,   1,
    169, 170, 171,   1,   1,   0,   1, 172,   1, 173, 107, 174, 175, 176,   0,   0,
      1,   1,  94,   0,   1, 177,   1, 178,   0,   0,   0,   2,   0,   0,   0,   0,
      1,   1, 179, 180,   1,   1, 181, 182,   1,  97, 119,  75,   1,   1, 136, 183,
    184,   2,   0,   0, 185, 158,   1, 186,  22,  31, 187, 188,   0,   0,   0,   0,
      1,   1, 189,   0,   1,   1, 190,   0,   0,   0,   0,   0,   1, 107,  78,   0,
      1,   1, 191, 192,   1,  81, 115,   0, 108,  87,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   1,   1, 193,   1,   1,  10,   1, 194, 195,   1,  73,
    196, 197, 133, 120, 141, 198,   0,   0, 199, 200, 190,   0,   0,   0,   0,   0,
      1,   1,   1,   1,  71,   0,   0,   0,   1,   1,   1, 201,   1,   1,   1,   1,
      1,   1, 202,   0,   0,   0,   0,   0,   1, 203,   0,   0,   0,   0,   0,   0,
      1,   1, 105,   0,   0,   0,   0,   0,   1,  73,   2, 204,   0,   0, 103, 205,
      1,   1, 206, 207, 208,   0,   0,   0,   1,   1, 209,   2, 210,   0,   0, 211,
      1,   1,   1,   1,   1,   1,   1, 192,   1,   1,   1,   1,   1,   1,   1, 177,
      2,   0,   0, 103,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  55,
      1,   1,   1, 212, 213, 202,   0,   0,   1,   1,   1,   1,   1,   1,   1,  83,
      1, 214,   1,   1,   1,   1,   1,  94,   1,   1, 215,   0,   0,   0,   0,   0,
      1,   1,  75,  15,   0,   0,   0,   0,   1,   1,  97,   1,  12, 216, 217,   1,
    218, 219, 220,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 221,   1,   1,
      1,   1,   1,   1,   1,   1, 222,   1,   1,   1,   1,   1, 223, 224,   0,   0,
    225, 226,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1, 227,   0,
      1,   1, 228,   0,   0,   0,   0,   0, 217, 229, 230, 231, 232, 233,   0, 234,
      1,  87,   1,   1,  76, 118, 235,  83, 120, 111,   1,  87,   1, 192,   0, 236,
    237,  55, 238, 215,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1, 239, 240,
      1,   1,   1,  76,   1,   1, 239,   0,  87,   1, 190,   1,  10, 241,   0,   0,
     87,   2, 120, 122,  81,   0, 242, 105,   1,   1,   1,   1,   1,   1,  75,   0,
      1, 239,   1,   1,   1,   1,   1,   1,  78,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1, 181,   1,   1,   1,   1,   1,   1,   1,   1,   1, 154,
     78,   0,   0,   0,   0,   0,   0,   0, 243,   1,   1,   1,   0,   0,   0,   0,
      1,   1,   1,   1,   1,   1,   1, 208,   1,   1,   1,   1,   1,   1,   1,  78,
};

static RE_UINT8 re_print_stage_5[] = {
      0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 255, 255, 255, 252,
    240, 215, 255, 255, 251, 255, 255, 255, 255, 255, 254, 255, 255, 255, 127, 254,
    254, 255, 255, 255, 255, 230, 254, 255, 255,   0, 255, 255, 255,   7,  31,   0,
    255, 255, 255, 223, 255, 191, 255, 255, 255, 231, 255, 255, 255, 255,   3,   0,
    255, 255, 255,   7, 255,  63, 255, 127, 255, 255, 255,  79, 255,   7,   0,   0,
    255, 255, 223,  63,   0,   0, 240, 255, 239, 159, 249, 255, 255, 253, 197, 243,
    159, 121, 128, 176, 207, 255, 255,  63, 238, 135, 249, 255, 255, 253, 109, 211,
    135,  57,   2,  94, 192, 255,  63,   0, 238, 191, 251, 255, 255, 253, 237, 243,
    191,  59,   1,   0, 207, 255,   3, 254, 238, 159, 249, 255, 159,  57, 192, 176,
    207, 255, 255,   0, 236, 199,  61, 214,  24, 199, 255, 195, 199,  61, 129,   0,
    192, 255, 255,   7, 239, 223, 253, 255, 255, 253, 255, 227, 223,  61,  96,   7,
    207, 255,   0, 255, 255, 253, 239, 243, 223,  61,  96,  64, 207, 255,   6,   0,
    223, 253, 240, 255, 207, 255, 255, 255, 236, 255, 127, 252, 255, 255, 251,  47,
    127, 132,  95, 255, 192, 255,  28,   0, 255, 255, 255, 135, 255, 255, 255,  15,
    150,  37, 240, 254, 174, 236, 255,  59,  95,  63, 255, 243, 255, 254, 255, 255,
    255,  31, 254, 255, 255, 255, 255, 254, 255, 223, 255,   7, 191,  32, 255, 255,
    255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255,
    255, 255,  61, 255, 255, 255, 255, 231, 255, 255, 255,  31, 255, 255, 255,   3,
    255, 255,  63,  63, 255, 255, 255,   1, 255, 223,  31,   0, 255, 255, 127,   0,
    255, 255,  15,   0, 255, 223,  13,   0, 255, 255, 255,  63, 255,   3, 255,   3,
    255, 127, 255,   3, 255, 255, 255,   0, 255,   7, 255, 255, 255, 255,  63,   0,
    255,  15, 255,  15, 241, 255, 255, 255, 255,  63,  31,   0, 255,  15, 255, 255,
    255,   3, 255, 199, 255, 255, 255, 207, 255, 255, 255, 159, 255, 255,  15, 240,
    255, 255, 255, 248, 255, 227, 255, 255, 255,   1,   0,   0, 255, 255, 255, 251,
     63,  63, 255, 170, 255, 255, 223, 255, 223, 255, 207, 239, 255, 255, 220, 127,
    255, 252, 255, 255, 223, 255, 243, 255, 255, 127, 255,  31,   0,   0, 255, 255,
    255, 255,   1,   0, 127,   0,   0,   0, 255, 255, 207, 255, 255, 255,  63, 255,
    255, 255, 255, 227, 255, 253,   7,   0,   0, 240,   0,   0, 255, 127, 255, 255,
    255, 255,  15, 254, 255, 128,   1, 128, 127, 127, 127, 127, 255,   3,   0,   0,
      0,   0, 255,  15, 224, 255, 255, 255, 255, 127, 254, 255,  15,   0, 255, 255,
    255,  31, 255, 255, 127,   0, 255, 255, 255,  15,   0,   0, 255, 127, 255,   0,
      0,   0, 128, 255, 255,  15, 255,   3,  63, 192, 255,   3, 255, 255,  15, 128,
    255, 191, 255, 195, 255,  63, 255, 243,   7,   0,   0, 248, 126, 126, 126,   0,
    127, 127, 255, 255,  63,   0, 255, 255, 255,  63, 255,   3, 127, 248, 255, 255,
    255,  63, 255, 255, 127,   0, 248, 224, 255, 255, 127,  95, 219, 255, 255, 255,
      3,   0, 248, 255, 255, 255, 252, 255, 255,   0,   0,   0,   0,   0, 255,  63,
    255, 255, 247, 255, 127,  15, 223, 255, 252, 252, 252,  28, 127, 127,   0,  62,
    255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63, 135, 255, 255, 255,
    255, 255, 143, 255, 255, 127, 255,  15,   1,   0,   0,   0,  15, 224, 255, 255,
    255, 255, 255, 191,  15, 255,  63,   0, 255,   3, 255, 255, 255, 255,  15, 255,
     15, 128,   0,   0,  63, 253, 255, 255, 255, 255, 191, 145, 255, 255, 191, 255,
    128, 255,   0,   0, 255, 255,  55, 248, 255, 255, 255, 143, 255, 255, 255, 131,
    255, 255, 255, 240, 111, 240, 239, 254, 255, 255,  15, 135, 255,   0, 255,   1,
    127, 248, 127,   0, 255, 255,  63, 254, 255, 255,   7, 255, 255, 255,   3,  30,
      0, 254,   0,   0, 255, 255,   7,   0, 255, 255,   7, 252, 255,  63, 252, 255,
    255, 255,   0, 128,   3,   0, 255, 255, 255,   1, 255,   3, 254, 255,  31,   0,
    255, 255, 251, 255, 127, 189, 255, 191, 255,   7, 255,   3, 159,  57, 129, 224,
    207,  31,  31,   0, 255, 255, 255,  43, 255,   0, 255,   3,  31,   0, 255,   3,
    255,  31,   0,   0, 255, 255,   7, 128, 207, 255, 255, 223,   7,   0,   0,   0,
    255, 253, 255, 255, 255, 255, 127, 255, 255, 254, 127,   0, 127, 251, 255, 255,
    255, 255, 127, 180, 255, 127,  31,   0,  15,   0,   0,   0, 255, 127,   0,   0,
    255, 195,   0,   0, 255,  63,  63,   0,  63,   0, 255, 251, 251, 255, 255, 224,
    255, 255,   0,   0,  31,   0, 255, 255,   0, 128, 255, 255,   3,   0,   0,   0,
    255,   7, 255,  31, 255,   1, 255, 243, 127, 254, 255, 255,  63,   0,   0,   0,
    100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,
     95, 252, 253, 255,  63, 255, 255, 255, 255, 207, 255, 255, 255,  15,   0, 248,
    254, 255,   0,   0, 127, 255, 255, 249, 219,   7,   0,   0, 159, 255, 127,   0,
    255,   7, 255, 195, 150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94,
    255, 251, 255,  15, 238, 251, 255,  15,   0,   0,   3,   0, 254, 255, 254, 255,
    192, 255, 255, 255,   7,   0, 255, 255, 255,   1,   3,   0, 255, 255,  31,   0,
    255,  31, 255,   1, 255,  63,   0,   0,   1,   0, 255, 255,   2,   0,   0,   0,
};

/* Print: 2442 bytes. */

RE_UINT32 re_get_print(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_print_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_print_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_print_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_print_stage_4[pos + f] << 5;
    pos += code;
    value = (re_print_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Word. */

static RE_UINT8 re_word_stage_1[] = {
    0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 6, 6,
    6, 6,
};

static RE_UINT8 re_word_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 13, 13, 13, 14,
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 28, 29, 30, 31, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 32,  7, 33, 34,  7, 35,  7,  7,  7, 36, 13, 37,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    38, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_word_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,   1,  17,  18,  19,   1,  20,  21,  22,  23,  24,  25,  26,  27,   1,  28,
     29,  30,  31,  31,  32,  31,  31,  31,  31,  31,  31,  31,  33,  34,  35,  31,
     36,  37,  31,  31,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,  38,   1,   1,   1,   1,   1,   1,   1,   1,   1,  39,
      1,   1,   1,   1,  40,   1,  41,  42,  43,  44,  45,  46,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  47,  31,  31,  31,  31,  31,  31,  31,  31,
     31,   1,  48,  49,   1,  50,  51,  52,  53,  54,  55,  56,  57,  58,   1,  59,
     60,  61,  62,  63,  64,  31,  31,  31,  65,  66,  67,  68,  69,  70,  71,  72,
     73,  31,  74,  31,  75,  76,  31,  31,   1,   1,   1,  77,  78,  79,  31,  31,
      1,   1,   1,   1,  80,  31,  31,  31,  31,  31,  31,  31,   1,   1,  81,  31,
      1,   1,  82,  83,  31,  31,  31,  84,   1,   1,   1,   1,   1,   1,   1,  85,
      1,   1,  86,  31,  31,  31,  31,  31,   1,  87,  88,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  89,  31,  31,  31,  31,  90,  91,  31,  92,  93,  94,  95,
     31,  31,  96,  31,  31,  31,  31,  31,  97,  31,  31,  31,  31,  31,  31,  31,
     98,  99,  31,  31,  31,  31, 100,  31,  31, 101,  31,  31,  31,  31,  31,  31,
      1,   1,   1,   1,   1,   1, 102,   1,   1,   1,   1,   1,   1,   1,   1, 103,
    104,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 105,   1,
      1,   1,   1, 106,  31,  31,  31,  31,   1,   1, 107,  31,  31,  31,  31,  31,
     31, 108,  31,  31,  31,  31,  31,  31,
};

static RE_UINT8 re_word_stage_4[] = {
      0,   1,   2,   3,   0,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   7,   8,   6,   6,   6,   9,  10,  11,   6,  12,
      6,   6,   6,   6,  11,   6,   6,   6,   6,  13,  14,  15,  16,  17,  18,  19,
     20,   6,   6,  21,   6,   6,  22,  23,  24,   6,  25,   6,   6,  26,   6,  27,
      6,  28,  29,  30,   0,  31,  32,  11,   6,   6,   6,  33,  34,  35,  36,  37,
     38,  39,  40,  41,  42,  43,  44,  45,  46,  43,  47,  48,  49,  50,  51,  52,
     53,  54,  55,  56,  53,  57,  58,  59,  53,   6,  60,  61,  62,  63,  64,  65,
     15,  66,  67,   0,  68,  69,  70,   0,  71,  72,  73,  74,  75,  76,  77,   0,
      6,   6,  78,   6,  79,   6,  80,  81,   6,   6,  82,   6,  83,  84,  85,   6,
     86,   6,  87,   0,  88,   6,   6,  89,  15,   6,   6,   6,   6,   6,   6,   6,
      6,   6,   6,  90,   3,   6,   6,  91,  92,  93,  94,  95,   6,   6,  96,  97,
     98,   6,   6,  99,   6, 100,   6, 101, 102, 103, 104, 105,   6, 106, 107,   0,
     29,   6, 102, 108, 107, 109,   0,   0,   6,   6, 110, 111,   6,   6,   6,  94,
      6,  99, 112,  79, 113,   0, 114, 115,   6,   6,   6,   6,   6,   6,   6, 116,
     89,   6, 117,  79,   6, 118, 119, 120, 121, 122, 123, 124, 125,   0,  24, 126,
    127, 128, 129,   6, 113,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 130,   6,  97,   6, 131, 102,   6,   6,   6,   6, 132,
      6,  80,   6, 133, 134, 135, 135,   6,   0, 136,   0,   0,   0,   0,   0,   0,
    137, 138,  15,   6, 139,  15,   6,  81, 140, 141,   6,   6, 142,  66,   0,  24,
      6,   6,   6,   6,   6, 101,   0,   0,   6,   6,   6,   6,   6,   6,   6,  30,
      6,   6,   6,   6, 143,   0,  24,  79, 144, 145,   6, 146,   6,   6,   6,  26,
    147, 148,   6,   6, 149, 150,   0, 147,   6, 151,   6,  94,   6,   6, 152, 153,
      6, 154,  94,  76,   6,   6, 155, 102,   6, 134, 156, 157,   6,   6, 158, 159,
    160, 161,  81, 162,   6,   6,   6, 163,   6,   6,   6,   6,   6, 164, 165,  29,
      6,   6,   6, 154,   6,   6, 115,   0, 166, 167, 168,   6,   6,  26, 169,   6,
      6,  79,  24,   6, 170,   6, 151, 171,  88, 172, 173, 174,   6,   6,   6,  76,
      1,   2,   3, 104,   6, 102, 175,   0, 176, 177, 178,   0,   6,   6,   6,  66,
      0,   0,   6,  93,   0,   0,   0, 179,   0,   0,   0,   0,  76,   6, 126, 180,
      6, 181, 100,  66,  79,   6, 182,   0,   6,   6,   6,   6,  79,  78, 183,  29,
      6, 184,   6, 185,   0,   0,   0,   0,   6, 134, 101, 151,   0,   0,   0,   0,
    186, 187, 101, 134, 102,   0,   0, 188, 101, 115,   0,   0,   6, 189,   0,   0,
    190, 191,   0,  76,  76,   0,  73, 192,   6, 101, 101, 193,  26,   0,   0,   0,
      6,   6, 113,   0,   6, 193,   6, 193,   6,   6, 192, 194,   6,  66,  24, 195,
      6, 196,  24, 197,   6,   6, 198,   0, 199, 200,   0,   0, 201, 202,   6, 203,
     34,  43, 204, 205,   0,   0,   0,   0,   6,   6, 203,   0,   6,   6, 206,   0,
      0,   0,   0,   0,   6, 207, 208,   0,   6,   6, 209,   0,   6,  99,  97,   0,
    210, 110,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6,   6, 211,
      6, 102, 212,   6, 213,   0,   6, 214, 215,   5, 216, 217, 170, 218,   0,   0,
    219, 220, 221,   0,   0,   0,   0,   0,   6,   6,   6,   6, 115,   0,   0,   0,
      6,   6,   6, 142,   6,   6,   6,   6,   6,   6, 185,   0,   0,   0,   0,   0,
      6, 142,   0,   0,   0,   0,   0,   0,   6,   6, 192,   0,   0,   0,   0,   0,
      6, 214, 102,  97,   0,   0,  24, 105,   6, 134, 222, 223,  88,   0,   0,   0,
      6,   6, 224, 102, 225,   0,   0, 226,   6,   6,   6,   6,   6,   6,   6, 143,
      6,   6,   6,   6,   6,   6,   6, 193, 102,   0,   0,  24,   6,   6,   6,   6,
      6,   6,   6,   6,   6,   6,   6,  29,   6,   6,   6, 227, 228,   0,   0,   0,
      0,   0,   0, 229, 230, 231,   0,   0,   0,   0, 232,   0,   0,   0,   0,   0,
      6,   6, 196,   6, 233, 234, 235,   6, 236, 237, 238,   6,   6,   6,   6,   6,
      6,   6,   6,   6,   6, 239, 240,  81, 196, 196, 131, 131, 215, 215, 241,   6,
      6, 242,   6, 243, 244, 245,   0,   0, 246, 247,   0,   0,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6, 248,   0,   6,   6, 203,   0,   0,   0,   0,   0,
    235, 249, 250, 251, 252, 253,   0,   0,   0,  24,  78,  78,  97,   0,   0,   0,
      6,   6,   6,   6,   6,   6, 134,   0,   6,  93,   6,   6,   6,   6,   6,   6,
     79,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6, 254,   6,   6,
      6,   6,   6,   6,   6,   6,   6, 180,  79,   0,   0,   0,   0,   0,   0,   0,
      6,   6,   6,   6,   6,   6,   6,  88,
};

static RE_UINT8 re_word_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 254, 255, 255, 135, 254, 255, 255,   7,
      0,   4,  32,   4, 255, 255, 127, 255, 255, 255, 255, 255, 195, 255,   3,   0,
     31,  80,   0,   0, 255, 255, 223, 188,  64, 215, 255, 255, 251, 255, 255, 255,
    255, 255, 191, 255, 255, 255, 254, 255, 255, 255, 127,   2, 254, 255, 255, 255,
    255,   0, 254, 255, 255, 255, 255, 191, 182,   0, 255, 255, 255,   7,   7,   0,
      0,   0, 255,   7, 255, 195, 255, 255, 255, 255, 239, 159, 255, 253, 255, 159,
      0,   0, 255, 255, 255, 231, 255, 255, 255, 255,   3,   0, 255, 255,  63,   4,
    255,  63,   0,   0, 255, 255, 255,  15, 255,   7,   0,   0, 255, 255, 223,  63,
      0,   0, 240, 255, 207, 255, 254, 255, 239, 159, 249, 255, 255, 253, 197, 243,
    159, 121, 128, 176, 207, 255,   3,  16, 238, 135, 249, 255, 255, 253, 109, 211,
    135,  57,   2,  94, 192, 255,  63,   0, 238, 191, 251, 255, 255, 253, 237, 243,
    191,  59,   1,   0, 207, 255,   0, 254, 238, 159, 249, 255, 159,  57, 192, 176,
    207, 255,   2,   0, 236, 199,  61, 214,  24, 199, 255, 195, 199,  61, 129,   0,
    192, 255,   0,   0, 239, 223, 253, 255, 255, 253, 255, 227, 223,  61,  96,   7,
    207, 255,   0,   0, 255, 253, 239, 243, 223,  61,  96,  64, 207, 255,   6,   0,
    223, 125, 240, 128, 207, 255,   0, 252, 236, 255, 127, 252, 255, 255, 251,  47,
    127, 132,  95, 255, 192, 255,  12,   0, 255, 255, 255,   7, 255, 127, 255,   3,
    150,  37, 240, 254, 174, 236, 255,  59,  95,  63, 255, 243,   1,   0,   0,   3,
    255,   3, 160, 194, 255, 254, 255, 255, 255,  31, 254, 255, 223, 255, 255, 254,
    255, 255, 255,  31,  64,   0,   0,   0, 255,   3, 255, 255, 255, 255, 255,  63,
    191,  32, 255, 255, 255, 255, 255, 247, 255,  61, 127,  61, 255,  61, 255, 255,
    255, 255,  61, 127,  61, 255, 127, 255, 255, 255,  61, 255, 255, 255, 255, 231,
    255, 255,   0,   0, 255, 255,  63,  63, 255, 159, 255, 255, 255, 199, 255,   1,
    255, 223,  31,   0, 255, 255,  31,   0, 255, 255,  15,   0, 255, 223,  13,   0,
    255, 255, 143,  48, 255,   3,   0,   0,   0,  56, 255,   3, 255, 255, 255,   0,
    255,   7, 255, 255, 255, 255,  63,   0, 255, 255, 255, 127, 255,  15, 255,  15,
    192, 255, 255, 255, 255,  63,  31,   0, 255,  15, 255, 255, 255,   3, 255,   3,
    255, 255, 255, 159, 128,   0, 255, 127, 255,  15, 255,   3,   0, 248,  15,   0,
    255, 227, 255, 255, 255,   1,   0,   0,   0,   0, 247, 255, 255, 255, 255,   3,
    255, 255, 255, 251,  63,  63, 255, 170, 255, 255, 223,  95, 220,  31, 207,  15,
    255,  31, 220,  31,   0,  48,   0,   0,   0,   0,   0, 128,   1,   0,  16,   0,
      0,   0,   2, 128,   0,   0, 255,  31, 255, 255,   1,   0, 132, 252,  47,  62,
     80, 189, 255, 243, 224,  67,   0,   0,   0,   0, 192, 255, 255, 127, 255, 255,
     31, 248,  15,   0, 255, 128,   0, 128, 255, 255, 127,   0, 127, 127, 127, 127,
      0, 128,   0,   0, 224,   0,   0,   0, 254, 255,  62,  31, 255, 255, 127, 230,
    224, 255, 255, 255, 255, 127, 254, 255, 255, 127,   0,   0, 255,  31,   0,   0,
    255,  31, 255, 255, 255,  15,   0,   0, 255, 255, 247, 191,   0,   0, 128, 255,
    252, 255, 255, 255, 255, 249, 255, 255, 255, 127, 255,   0, 255,   0,   0,   0,
     63,   0, 255,   3, 255, 255, 255,  40, 255,  63, 255, 255,   1, 128, 255,   3,
    255,  63, 255,   3, 255, 255, 127, 252,   7,   0,   0,  56, 255, 255, 124,   0,
    126, 126, 126,   0, 127, 127, 255, 255,  63,   0, 255, 255, 255,  55, 255,   3,
     15,   0, 255, 255, 127, 248, 255, 255, 127,   0, 248, 224, 255, 253, 127,  95,
    219, 255, 255, 255,   0,   0, 248, 255, 255, 255, 252, 255,   0,   0, 255,  15,
    255, 255,  24,   0,   0, 224,   0,   0,   0,   0, 223, 255, 252, 252, 252,  28,
    255, 239, 255, 255, 127, 255, 255, 183, 255,  63, 255,  63,   0,   0,   0,  32,
      1,   0,   0,   0,   0, 224, 255, 255,  15, 255,  62,   0, 255, 255,  15, 255,
    255,   0, 255, 255,  15,   0,   0,   0,  63, 253, 255, 255, 255, 255, 191, 145,
    255, 255,  55,   0, 255, 255, 255, 192, 111, 240, 239, 254, 255, 255,  15, 135,
    127,   0,   0,   0, 255, 255,   7,   0, 192, 255,   0, 128, 255,   1, 255,   3,
    255, 255, 223, 255, 255, 255,  79,   0,  31,  28, 255,  23, 255, 255, 251, 255,
    255, 255, 255,  64, 127, 189, 255, 191, 255,   1, 255, 255, 255,   7, 255,   3,
    159,  57, 129, 224, 207,  31,  31,   0, 191,   0, 255,   3, 255, 255,  63, 255,
      1,   0,   0,  63,  17,   0, 255,   3, 255, 255, 255, 227, 255,   3,   0, 128,
    128,   0, 255, 255, 207, 255, 255,   3, 255, 255, 255,   1, 255, 253, 255, 255,
      1,   0, 255,   3,   0,   0, 252, 255, 255, 254, 127,   0, 127, 251, 255, 255,
    255, 255, 127, 180, 255,   0, 255,   3,  15,   0, 255,   3, 248, 255, 255, 224,
     31,   0, 255, 255,   0, 128, 255, 255,   3,   0,   0,   0, 255,   7, 255,  31,
    255,   1, 255,  99, 224, 227,   7, 248, 231,  15,   0,   0,   0,  60,   0,   0,
     28,   0,   0,   0, 255, 255, 255, 223, 100, 222, 255, 235, 239, 255, 255, 255,
    191, 231, 223, 223, 255, 255, 255, 123,  95, 252, 253, 255,  63, 255, 255, 255,
    253, 255, 255, 247, 247, 207, 255, 255, 255, 255, 127, 248, 255,  31,  32,   0,
     16,   0,   0, 248, 254, 255,   0,   0, 127, 255, 255, 249, 219,   7,   0,   0,
     31,   0, 127,   0, 150, 254, 247,  10, 132, 234, 150, 170, 150, 247, 247,  94,
    255, 251, 255,  15, 238, 251, 255,  15,   3,   0, 255, 255,
};

/* Word: 2366 bytes. */

RE_UINT32 re_get_word(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 15;
    code = ch ^ (f << 15);
    pos = (RE_UINT32)re_word_stage_1[f] << 4;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_word_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_word_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_word_stage_4[pos + f] << 5;
    pos += code;
    value = (re_word_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* XDigit. */

static RE_UINT8 re_xdigit_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_xdigit_stage_2[] = {
     0,  1,  2,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  5,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4,  7,
     8,  4,  9, 10,  4,  4,  4,  4,  4,  4,  4,  4,  4, 11,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 12,  4,  4, 13,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
};

static RE_UINT8 re_xdigit_stage_3[] = {
     0,  1,  1,  1,  1,  1,  2,  3,  1,  4,  4,  4,  4,  4,  5,  6,
     7,  1,  1,  1,  1,  1,  1,  8,  9, 10, 11, 12, 13,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  6,  1,
    14, 15, 16, 17,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 18,
     1,  1,  1,  1, 19,  1,  1,  1, 20, 21, 17,  1,  5,  1, 22, 23,
     8,  1,  1,  1, 16, 16,  1,  1,  1,  1, 24, 16,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1, 25,  1, 16,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_xdigit_stage_4[] = {
     0,  1,  2,  2,  2,  2,  2,  2,  2,  3,  2,  0,  2,  2,  2,  4,
     2,  5,  2,  5,  2,  6,  2,  6,  3,  2,  2,  2,  2,  4,  6,  2,
     2,  2,  2,  3,  6,  2,  2,  2,  2,  7,  2,  6,  2,  2,  8,  2,
     2,  6,  0,  2,  2,  8,  2,  2,  2,  2,  2,  6,  4,  2,  2,  9,
     2,  6,  2,  2,  2,  2,  2,  0, 10, 11,  2,  2,  2,  2,  3,  2,
     2,  5,  2,  0, 12,  2,  2,  6,  2,  6,  2,  4,  0,  2,  2,  2,
     2,  3,  2,  2,  2,  2,  2, 13,
};

static RE_UINT8 re_xdigit_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 126,   0,   0,   0, 126,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255,   3,   0,   0,
    255,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 192, 255,   0,   0,
      0,   0, 255,   3,   0,   0,   0,   0, 192, 255,   0,   0,   0,   0,   0,   0,
    255,   3, 255,   3,   0,   0,   0,   0,   0,   0, 255,   3,   0,   0, 255,   3,
      0,   0, 255,   3, 126,   0,   0,   0, 126,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 192, 255,   0, 192, 255, 255, 255, 255, 255, 255,
};

/* XDigit: 441 bytes. */

RE_UINT32 re_get_xdigit(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_xdigit_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_xdigit_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_xdigit_stage_3[pos + f] << 2;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_xdigit_stage_4[pos + f] << 6;
    pos += code;
    value = (re_xdigit_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Posix_Digit. */

static RE_UINT8 re_posix_digit_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_posix_digit_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_posix_digit_stage_3[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_posix_digit_stage_4[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_posix_digit_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3,   0,   0,   0,   0,   0,   0,   0,   0,
};

/* Posix_Digit: 97 bytes. */

RE_UINT32 re_get_posix_digit(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_posix_digit_stage_1[f] << 4;
    f = code >> 12;
    code ^= f << 12;
    pos = (RE_UINT32)re_posix_digit_stage_2[pos + f] << 3;
    f = code >> 9;
    code ^= f << 9;
    pos = (RE_UINT32)re_posix_digit_stage_3[pos + f] << 3;
    f = code >> 6;
    code ^= f << 6;
    pos = (RE_UINT32)re_posix_digit_stage_4[pos + f] << 6;
    pos += code;
    value = (re_posix_digit_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Posix_AlNum. */

static RE_UINT8 re_posix_alnum_stage_1[] = {
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3,
};

static RE_UINT8 re_posix_alnum_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  9, 10, 11,  7,  7,  7,  7, 12, 13, 13, 13, 13, 14,
    15, 16, 17, 18, 19, 13, 20, 13, 21, 13, 13, 13, 13, 22,  7,  7,
    23, 24, 13, 13, 13, 13, 25, 26, 13, 13, 27, 13, 28, 29, 30, 13,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7, 31,  7, 32, 33,  7, 34,  7,  7,  7, 35, 13, 36,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
};

static RE_UINT8 re_posix_alnum_stage_3[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,   1,  17,  18,  19,   1,  20,  21,  22,  23,  24,  25,  26,  27,   1,  28,
     29,  30,  31,  31,  32,  31,  31,  31,  31,  31,  31,  31,  33,  34,  35,  31,
     36,  37,  31,  31,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,  38,   1,   1,   1,   1,   1,   1,   1,   1,   1,  39,
      1,   1,   1,   1,  40,   1,  41,  42,  43,  44,  45,  46,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,  47,  31,  31,  31,  31,  31,  31,  31,  31,
     31,   1,  48,  49,   1,  50,  51,  52,  53,  54,  55,  56,  57,  58,   1,  59,
     60,  61,  62,  63,  64,  31,  31,  31,  65,  66,  67,  68,  69,  70,  71,  72,
     73,  31,  74,  31,  75,  76,  31,  31,   1,   1,   1,  77,  78,  79,  31,  31,
      1,   1,   1,   1,  80,  31,  31,  31,  31,  31,  31,  31,   1,   1,  81,  31,
      1,   1,  82,  83,  31,  31,  31,  84,   1,   1,   1,   1,   1,   1,   1,  85,
      1,   1,  86,  31,  31,  31,  31,  31,   1,  87,  88,  31,  31,  31,  31,  31,
     31,  31,  31,  31,  89,  31,  31,  31,  31,  31,  31,  31,  90,  91,  92,  93,
     94,  31,  31,  31,  31,  31,  31,  31,  95,  96,  31,  31,  31,  31,  97,  31,
     31,  98,  31,  31,  31,  31,  31,  31,   1,   1,   1,   1,   1,   1,  99,   1,
      1,   1,   1,   1,   1,   1,   1, 100, 101,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1, 102,   1,   1,   1,   1, 103,  31,  31,  31,  31,
      1,   1, 104,  31,  31,  31,  31,  31,
};

static RE_UINT8 re_posix_alnum_stage_4[] = {
      0,   1,   2,   2,   0,   3,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
      5,   5,   5,   5,   5,   5,   6,   7,   0,   0,   8,   9,  10,  11,   5,  12,
      5,   5,   5,   5,  13,   5,   5,   5,   5,  14,  15,  16,  17,  18,  19,  20,
     21,   5,  22,  23,   5,   5,  24,  25,  26,   5,  27,   5,   5,  28,  29,  30,
     31,  32,  33,  34,   0,  35,  36,  37,   5,  38,  39,  40,  41,  42,  43,  44,
     45,  46,  47,  48,  49,  50,  51,  52,  53,  50,  54,  55,  56,  57,  58,   0,
     59,  60,  61,  62,  59,  63,  64,  65,  59,  66,  67,  68,  69,  70,  71,  72,
     16,  73,  74,   0,  75,  76,  77,   0,  78,   0,  79,  80,  81,  82,   0,   0,
      5,  83,  26,  84,  85,   5,  86,  87,   5,   5,  88,   5,  89,  90,  91,   5,
     92,   5,  93,   0,  94,   5,   5,  95,  16,   5,   5,   5,   5,   5,   5,   5,
      5,   5,   5,  96,   2,   5,   5,  97,  98,  99,  99, 100,   5, 101, 102,   0,
      0,   5,   5, 103,   5, 104,   5, 105, 106, 107,  26, 108,   5, 109, 110,   0,
    111,   5, 106, 112,   0, 113,   0,   0,   5, 114, 115,   0,   5, 116,   5, 117,
      5, 105, 118, 119, 120,   0,   0, 121,   5,   5,   5,   5,   5,   5,   0, 122,
     95,   5, 123, 119,   5, 124, 125, 126,   0,   0,   0, 127, 128,   0,   0,   0,
    129, 130, 131,   5, 120,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0, 132,   5, 110,   5, 133, 106,   5,   5,   5,   5, 134,
      5,  86,   5, 135, 136, 137, 137,   5,   0, 138,   0,   0,   0,   0,   0,   0,
    139, 140,  16,   5, 141,  16,   5,  87, 142, 143,   5,   5, 144,  73,   0,  26,
      5,   5,   5,   5,   5, 105,   0,   0,   5,   5,   5,   5,   5,   5,   5,  34,
      5,   5,   5,   5,  32,   0,  26, 119, 145, 146,   5, 147,   5,   5,   5,  94,
    148, 149,   5,   5, 150, 151,   0, 148, 152,  17,   5,  99,   5,   5, 153, 154,
     29, 104, 155,  82,   5, 156, 138, 157,   5, 136, 158, 159,   5, 106, 160, 161,
    162, 163,  87, 164,   5,   5,   5,  34,   5,   5,   5,   5,   5, 165, 166, 111,
      5,   5,   5, 167,   5,   5, 168,   0, 169, 170, 171,   5,   5,  28, 172,   5,
      5, 119,  26,   5, 173,   5,  17, 174,   0,   0,   0, 175,   5,   5,   5,  82,
      0,   2,   2, 176,   5, 106, 177,   0, 178, 179, 180,   0,   5,   5,   5,  73,
      0,   0,   5, 181,   0,   0,   0,   0,   0,   0,   0,   0,  82,   5, 182,   0,
      5,  27, 104,  73, 119,   5, 183,   0,   5,   5,   5,   5, 119,  26, 184, 111,
      5, 185,   5,  62,   0,   0,   0,   0,   5, 136, 105,  17,   0,   0,   0,   0,
    186, 187, 105, 136, 106,   0,   0, 188, 105, 168,   0,   0,   5, 189,   0,   0,
    190,  99,   0,  82,  82,   0,  79, 191,   5, 105, 105, 155,  28,   0,   0,   0,
      5,   5, 120,   0,   5, 155,   5, 155,   5,   5, 192,   0, 149,  33,  26, 120,
      5, 155,  26, 193,   5,   5, 194,   0, 195, 196,   0,   0, 197, 198,   5, 120,
     41,  50, 199,  62,   0,   0,   0,   0,   5,   5, 200,   0,   5,   5, 201,   0,
      0,   0,   0,   0,   5, 202, 203,   0,   5, 106, 204,   0,   5, 105,   0,   0,
    205,  34,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,   5, 206,
      5, 207,  26,   5, 208,   0,   5,  33, 209, 210,  78, 211, 173, 212,   0,   0,
    213, 214, 215,   0,   0,   0,   0,   0,   5,   5,   5,   5, 168,   0,   0,   0,
      5,   5,   5, 144,   5,   5,   5,   5,   5,   5,  62,   0,   0,   0,   0,   0,
      5, 144,   0,   0,   0,   0,   0,   0,   5,   5, 216,   0,   0,   0,   0,   0,
      5,  33, 106,   0,   0,   0,  26, 158,   5, 136,  62, 217,  94,   0,   0,   0,
      5,   5, 218, 106, 172,   0,   0, 219,   5,   5,   5,   5,   5,   5,   5,  32,
      5,   5,   5,   5,   5,   5,   5, 155, 106,   0,   0,  26,   5,   5,   5,   5,
      5,   5,   5,   5,   5,   5,   5, 111,   5,   5,   5, 220, 221,   0,   0,   0,
      5,   5, 222,   5, 223, 224, 225,   5, 226, 227, 228,   5,   5,   5,   5,   5,
      5,   5,   5,   5,   5, 229, 230,  87, 222, 222, 133, 133, 209, 209, 231,   0,
    232, 233,   0,   0,   0,   0,   0,   0,   5,   5,   5,   5,   5,   5, 191,   0,
      5,   5, 234,   0,   0,   0,   0,   0, 225, 235, 236, 237, 238, 239,   0,   0,
      0,  26, 240, 240, 110,   0,   0,   0,   5,   5,   5,   5,   5,   5, 136,   0,
      5, 181,   5,   5,   5,   5,   5,   5, 119,   5,   5,   5,   5,   5,   5,   5,
      5,   5,   5,   5,   5, 241,   5,   5,   5,   5,   5,   5,   5,   5,   5,  78,
    119,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_posix_alnum_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 254, 255, 255,   7,   0,   4,  32,   4,
    255, 255, 127, 255, 255, 255, 255, 255, 195, 255,   3,   0,  31,  80,   0,   0,
     32,   0,   0,   0,   0,   0, 223, 188,  64, 215, 255, 255, 251, 255, 255, 255,
    255, 255, 191, 255,   3, 252, 255, 255, 255, 255, 254, 255, 255, 255, 127,   2,
    254, 255, 255, 255, 255,   0,   0,   0,   0,   0, 255, 191, 182,   0, 255, 255,
    255,   7,   7,   0,   0,   0, 255,   7, 255, 255, 255, 254,   0, 192, 255, 255,
    255, 255, 239,  31, 254, 225,   0, 156,   0,   0, 255, 255,   0, 224, 255, 255,
    255, 255,   3,   0,   0, 252, 255, 255, 255,   7,  48,   4, 255, 255, 255, 252,
    255,  31,   0,   0, 255, 255, 255,   1, 255,   7,   0,   0, 255, 255, 223,  63,
      0,   0, 240, 255, 248,   3, 255, 255, 255, 255, 255, 239, 255, 223, 225, 255,
     15,   0, 254, 255, 239, 159, 249, 255, 255, 253, 197, 227, 159,  89, 128, 176,
     15,   0,   3,  16, 238, 135, 249, 255, 255, 253, 109, 195, 135,  25,   2,  94,
      0,   0,  63,   0, 238, 191, 251, 255, 255, 253, 237, 227, 191,  27,   1,   0,
     15,   0,   0,  30, 238, 159, 249, 255, 159,  25, 192, 176,  15,   0,   2,   0,
    236, 199,  61, 214,  24, 199, 255, 195, 199,  29, 129,   0, 239, 223, 253, 255,
    255, 253, 255, 227, 223,  29,  96,   7,  15,   0,   0,   0, 255, 253, 239, 227,
    223,  29,  96,  64,  15,   0,   6,   0, 255, 255, 255, 231, 223,  93, 240, 128,
     15,   0,   0, 252, 236, 255, 127, 252, 255, 255, 251,  47, 127, 128,  95, 255,
      0,   0,  12,   0, 255, 255, 255,   7, 127,  32,   0,   0, 150,  37, 240, 254,
    174, 236, 255,  59,  95,  32,   0, 240,   1,   0,   0,   0, 255, 254, 255, 255,
    255,  31, 254, 255,   3, 255, 255, 254, 255, 255, 255,  31, 255, 255, 127, 249,
    231, 193, 255, 255, 127,  64,   0,  48, 191,  32, 255, 255, 255, 255, 255, 247,
    255,  61, 127,  61, 255,  61, 255, 255, 255, 255,  61, 127,  61, 255, 127, 255,
    255, 255,  61, 255, 255, 255, 255, 135, 255, 255,   0,   0, 255, 255,  63,  63,
    255, 159, 255, 255, 255, 199, 255,   1, 255, 223,  15,   0, 255, 255,  15,   0,
    255, 223,  13,   0, 255, 255, 207, 255, 255,   1, 128,  16, 255, 255, 255,   0,
    255,   7, 255, 255, 255, 255,  63,   0, 255, 255, 255, 127, 255,  15, 255,   1,
    255,  63,  31,   0, 255,  15, 255, 255, 255,   3,   0,   0, 255, 255, 255,  15,
    254, 255,  31,   0, 128,   0,   0,   0, 255, 255, 239, 255, 239,  15,   0,   0,
    255, 243,   0, 252, 191, 255,   3,   0,   0, 224,   0, 252, 255, 255, 255,  63,
    255,   1,   0,   0,   0, 222, 111,   0, 128, 255,  31,   0,  63,  63, 255, 170,
    255, 255, 223,  95, 220,  31, 207,  15, 255,  31, 220,  31,   0,   0,   2, 128,
      0,   0, 255,  31, 132, 252,  47,  62,  80, 189, 255, 243, 224,  67,   0,   0,
      0,   0, 192, 255, 255, 127, 255, 255,  31, 120,  12,   0, 255, 128,   0,   0,
    255, 255, 127,   0, 127, 127, 127, 127,   0, 128,   0,   0, 224,   0,   0,   0,
    254,   3,  62,  31, 255, 255, 127, 224, 224, 255, 255, 255, 255, 127, 254, 255,
    255, 127,   0,   0, 255,  31, 255, 255,   0,  12,   0,   0, 255, 127, 240, 143,
      0,   0, 128, 255, 252, 255, 255, 255, 255, 249, 255, 255, 255, 127, 255,   0,
    187, 247, 255, 255,  47,   0,   0,   0,   0,   0, 252,  40, 255, 255,   7,   0,
    255, 255, 247, 255, 223, 255,   0, 124, 255,  63,   0,   0, 255, 255, 127, 196,
      5,   0,   0,  56, 255, 255,  60,   0, 126, 126, 126,   0, 127, 127, 255, 255,
     63,   0, 255, 255,  15,   0, 255, 255, 127, 248, 255, 255, 255,  63, 255, 255,
    255, 255, 255,   3, 127,   0, 248, 224, 255, 253, 127,  95, 219, 255, 255, 255,
      0,   0, 248, 255, 255, 255, 252, 255,   0,   0, 255,  15,   0,   0, 223, 255,
    192, 255, 255, 255, 252, 252, 252,  28, 255, 239, 255, 255, 127, 255, 255, 183,
    255,  63, 255,  63, 255, 255,  31,   0, 255, 255,   1,   0,  15, 255,  62,   0,
    255, 255,  15, 255, 255,   0, 255, 255,  63, 253, 255, 255, 255, 255, 191, 145,
    255, 255,  55,   0, 255, 255, 255, 192, 111, 240, 239, 254,  31,   0,   0,   0,
     63,   0,   0,   0, 255, 255,  71,   0,  30,   0,   0,  20, 255, 255, 251, 255,
    255, 255, 159,  64, 127, 189, 255, 191, 255,   1, 255, 255, 159,  25, 129, 224,
    187,   7,   0,   0, 179,   0,   0,   0, 255, 255,  63, 127,   0,   0,   0,  63,
     17,   0,   0,   0, 255, 255, 255, 227,   0,   0,   0, 128, 255, 255, 231, 127,
    207, 255, 255,   0, 255, 253, 255, 255, 255, 255, 127, 127,   0,   0, 252, 255,
    255, 254, 127,   0, 127, 251, 255, 255, 255, 255, 127, 180, 203,   0,   0,   0,
    127,   0,   0,   0, 248, 255, 255, 224,  31,   0, 255, 255,   3,   0,   0,   0,
    255,   7, 255,  31, 255,   1, 255,  67, 255, 255, 223, 255, 255, 255, 255, 223,
    100, 222, 255, 235, 239, 255, 255, 255, 191, 231, 223, 223, 255, 255, 255, 123,
     95, 252, 253, 255,  63, 255, 255, 255, 253, 255, 255, 247, 247,  15,   0,   0,
    127, 255, 255, 249, 219,   7,   0,   0, 143,   0,   0,   0, 150, 254, 247,  10,
    132, 234, 150, 170, 150, 247, 247,  94, 255, 251, 255,  15, 238, 251, 255,  15,
    255,   3, 255, 255,   3,   0, 255, 255,
};

/* Posix_AlNum: 2249 bytes. */

RE_UINT32 re_get_posix_alnum(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_posix_alnum_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_posix_alnum_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_posix_alnum_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_posix_alnum_stage_4[pos + f] << 5;
    pos += code;
    value = (re_posix_alnum_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Posix_Punct. */

static RE_UINT8 re_posix_punct_stage_1[] = {
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2,
};

static RE_UINT8 re_posix_punct_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  9, 10,  7,  7,  7,  7,  7,  7,  7,  7,  7, 11,
    12, 13, 14, 15, 16,  7,  7,  7,  7,  7,  7,  7,  7, 17,  7,  7,
     7,  7,  7,  7,  7,  7,  7, 18,  7,  7, 19, 20,  7, 21, 22, 23,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
};

static RE_UINT8 re_posix_punct_stage_3[] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16,  1,  1, 17, 18,  1, 19, 20, 21, 22, 23, 24, 25,  1,  1, 26,
    27, 28, 29, 29, 30, 29, 29, 31, 29, 29, 29, 32, 33, 34, 35, 36,
    37, 38, 39, 29,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1, 40,  1,  1,  1,  1,  1,  1, 41,  1, 42, 43,
    44, 45, 46, 47,  1,  1,  1,  1,  1,  1,  1, 48,  1, 49, 50, 51,
     1, 52,  1, 53,  1, 54,  1,  1, 55, 56, 57, 58,  1,  1,  1,  1,
    59, 60, 61,  1, 62, 63, 64, 65,  1,  1, 66,  1, 67,  1,  1,  1,
     1,  1,  1,  1, 68,  1,  1,  1,  1,  1, 69, 70,  1,  1,  1,  1,
     1,  1,  1,  1, 71,  1,  1,  1, 72, 73, 74, 75,  1,  1, 76, 77,
    29, 29, 78,  1,  1,  1,  1,  1,  1, 79,  1,  1,  1,  1, 10,  1,
    80, 81, 82, 29, 29, 29, 83, 84, 85, 86,  1,  1,  1,  1,  1,  1,
};

static RE_UINT8 re_posix_punct_stage_4[] = {
      0,   1,   2,   3,   0,   4,   5,   5,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   6,   7,   0,   0,   0,   8,   9,   0,   0,  10,
      0,   0,   0,   0,  11,   0,   0,   0,   0,   0,  12,   0,  13,  14,  15,  16,
     17,   0,   0,  18,   0,   0,  19,  20,  21,   0,   0,   0,   0,   0,   0,  22,
      0,  23,  14,   0,   0,   0,   0,   0,   0,   0,   0,  24,   0,   0,   0,  25,
      0,   0,   0,   0,   0,   0,   0,  26,   0,   0,   0,  27,   0,   0,   0,  28,
      0,   0,   0,  29,   0,   0,   0,   0,   0,   0,  30,  31,   0,   0,   0,  32,
      0,  29,  33,   0,   0,   0,   0,   0,  34,  35,   0,   0,  36,  37,  38,   0,
      0,   0,  39,   0,  37,   0,   0,  40,   0,   0,   0,  41,  42,   0,   0,   0,
     43,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  44,  45,   0,   0,  46,
      0,  47,   0,   0,   0,   0,  48,   0,  49,   0,   0,   0,   0,   0,   0,   0,
      0,   0,  50,   0,   0,   0,  37,  51,  37,   0,   0,   0,   0,  52,   0,   0,
      0,   0,  12,  53,   0,   0,   0,  54,   0,  55,   0,  37,   0,   0,  56,   0,
      0,   0,   0,   0,   0,  57,  58,  59,  60,  61,  62,  63,  64,  51,   0,   0,
     65,  66,  67,   0,  68,  51,  51,  51,  51,  51,  51,  51,  51,  51,  51,  51,
     51,  69,  49,   0,  54,  70,   0,   0,  51,  51,  51,  70,  71,  51,  51,  51,
     51,  51,  51,  72,  73,  74,  75,  76,   0,   0,   0,   0,   0,   0,   0,  77,
      0,   0,   0,  27,   0,   0,   0,   0,  51,  78,  79,   0,  80,  51,  51,  81,
     51,  51,  51,  51,  51,  51,  70,  82,  83,  84,   0,   0,  45,  43,   0,  40,
      0,   0,   0,   0,  85,   0,  51,  86,  62,  87,  88,  51,  87,  89,  51,  62,
      0,   0,   0,   0,   0,   0,  51,  51,   0,   0,   0,   0,  60,  51,  69,  37,
     90,   0,   0,  91,   0,   0,   0,  92,  93,  94,   0,   0,  95,   0,   0,   0,
      0,  96,   0,  97,   0,   0,  98,  99,   0,  98,  29,   0,   0,   0, 100,   0,
      0,   0,  54, 101,   0,   0,  37,  26,   0,   0,  40,   0,   0,   0,   0, 102,
      0, 103,   0,   0,   0, 104,  94,   0,   0,  37,   0,   0,   0,   0,   0, 105,
     42,  60, 106, 107,   0,   0,   0,   0,   1,   2,   2, 108,   0,   0,   0, 109,
    110, 111,   0, 112, 113,  43,  60, 114,   0,   0,   0,   0,  29,   0,  27,   0,
      0,   0,   0,  30,   0,   0,   0,   0,   0,   0,   5, 115,   0,   0,   0,   0,
     29,  29,   0,   0,   0,   0,   0,   0,   0,   0, 116,  29,   0,   0, 117, 118,
      0, 112,   0,   0, 119,   0,   0,   0,   0,   0, 120,   0,   0, 121,  94,   0,
      0,   0,  86, 122,   0,   0, 123,   0,   0, 124,   0,   0,   0, 103,   0,   0,
      0,   0, 125,   0,   0,   0, 126,   0,   0,   0,   0,   0,   0,   0, 127,   0,
      0,   0, 128, 129,   0,   0,   0,   0,   0,  54,   0,   0,   0,   0,   0,   0,
      0,  29,  69,   0, 130, 110,   0,   0,   0,   0, 131,  26,   0,   0,   0,   0,
      0,   0,   0, 132,   0,   0,   0,   0,   0,   0,   0,  98,   0,   0,   0, 133,
      0, 111, 134,   0,   0,   0,   0,   0,   0,   0,   0,   0, 135,   0,   0,   0,
     51,  51,  51,  51,  51,  51,  51,  70,  51, 136,  51, 137, 138, 139,  51,  41,
     51,  51, 140,   0,   0,   0,   0,   0,  51,  51,  93,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 141,  40, 133, 133,  30,  30, 103, 103, 142,   0,
      0, 143,   0, 144, 145,   0,   0,   0,   0,   0,  37,   0,   0,   0,   0,   0,
     51, 146,  51,  51,  81, 147, 148,  70,  60, 149,  39, 150,  87, 129,   0, 151,
    152, 153, 154, 108,   0,   0,   0,   0,  51,  51,  51,  51,  51,  51, 155, 156,
     51,  51,  51,  81,  51,  51, 155,   0, 146,  51, 157,  51,  61,  21,   0,   0,
    146,  62, 158, 159, 160,   0, 161,  69,
};

static RE_UINT8 re_posix_punct_stage_5[] = {
      0,   0,   0,   0, 254, 255,   0, 252,   1,   0,   0, 248,   1,   0,   0, 120,
    254, 219, 211, 137,   0,   0, 128,   0,  60,   0, 252, 255, 224, 175, 255, 255,
      0,   0,  32,  64, 176,   0,   0,   0,   0,   0,  64,   0,   4,   0,   0,   0,
      0,   0,   0, 252,   0, 230,   0,   0,   0,   0,   0,  64,  73,   0,   0,   0,
      0,   0,  24,   0, 192, 255,   0, 200,   0,  60,   0,   0,   0,   0,  16,  64,
      0,   2,   0,  96, 255,  63,   0,   0,   0,   0, 192,   3,   0,   0, 255, 127,
     48,   0,   1,   0,   0,   0,  12,  44,   0,   0,   3,   0,   0,   0,   1,   0,
      0,   0, 248,   7,   0,   0,   0, 128,   0, 128,   0,   0,   0,   0,   0,   2,
      0,   0,  16,   0,   0, 128,   0,  12, 254, 255, 255, 252,   0,   0,  80,  61,
     32,   0,   0,   0,   0,   0,   0, 192, 191, 223, 255,   7,   0, 252,   0,   0,
      0,   0,   0,   8, 255,   1,   0,   0,   0,   0, 255,   3,   1,   0,   0,   0,
      0,  96,   0,   0,   0,   0,   0,  24,   0,  56,   0,   0,   0,   0,  96,   0,
      0,   0, 112,  15, 255,   7,   0,   0,  49,   0,   0,   0, 255, 255, 255, 255,
    127,  63,   0,   0, 255,   7, 240,  31,   0,   0,   0, 240,   0,   0,   0, 248,
    255,   0,   8,   0,   0,   0,   0, 160,   3, 224,   0, 224,   0, 224,   0,  96,
      0,   0, 255, 255, 255,   0, 255, 255, 255, 255, 255, 127,   0,   0,   0, 124,
      0, 124,   0,   0, 123,   3, 208, 193, 175,  66,   0,  12,  31, 188,   0,   0,
      0,  12, 255, 255, 127,   0,   0,   0, 255, 255,  63,   0,   0,   0, 240, 255,
    255, 255, 207, 255, 255, 255,  63, 255, 255, 255, 255, 227, 255, 253,   7,   0,
      0, 240,   0,   0, 224,   7,   0, 222, 255, 127, 255, 255, 255,   3,   0,   0,
    255, 255, 255, 251, 255, 255,  15,   0,   0,   0, 255,  15,  30, 255, 255, 255,
      1,   0, 193, 224,   0,   0, 195, 255,  15,   0,   0,   0,   0, 252, 255, 255,
    255,   0,   1,   0, 255, 255,   1,   0,   0, 224,   0,   0,   0,   0,   8,  64,
      0,   0, 252,   0, 255, 255, 127,   0,   3,   0,   0,   0,   0,   6,   0,   0,
      0,  15, 192,   3,   0,   0, 240,   0,   0, 192,   0,   0,   0,   0,   0,  23,
    254,  63,   0, 192,   0,   0, 128,   3,   0,   8,   0,   0,   0,   2,   0,   0,
      0,   0, 252, 255,   0,   0,   0,  48, 255, 255, 247, 255, 127,  15,   0,   0,
     63,   0,   0,   0, 127, 127,   0,  48,   7,   0,   0,   0,   0,   0, 128, 255,
      0,   0,   0, 254, 255, 115, 255,  15, 255, 255, 255,  31,   0,   0, 128,   1,
      0,   0, 255,   1,   0,   1,   0,   0,   0,   0, 127,   0,   0,   0,   0,  30,
    128,  63,   0,   0,   0,   0,   0, 216,   0,   0,  48,   0, 224,  35,   0, 232,
      0,   0,   0,  63,   0, 248,   0,  40,  64,   0,   0,   0, 254, 255, 255,   0,
     14,   0,   0,   0, 255,  31,   0,   0,   0,   0,   0, 220,  62,   0,   0,   0,
      0,   0,  31,   0,   0,   0,  32,   0,  48,   0,   0,   0,   0,   0,   0, 144,
    127, 254, 255, 255,  31,  28,   0,   0,  24, 240, 255, 255, 255, 195, 255, 255,
     35,   0,   0,   0,   2,   0,   0,   8,   8,   0,   0,   0,   0,   0, 128,   7,
      0, 224, 223, 255, 239,  15,   0,   0, 255,  15, 255, 255, 255, 127, 254, 255,
    254, 255, 254, 255, 255, 127,   0,   0,   0,  12,   0,   0, 192, 255, 255, 255,
      7,   0, 255, 255, 255, 255, 255,  15, 255,   1,   3,   0, 255, 255,  31,   0,
    255,  31, 255,   1, 255,   0, 255,   3, 255,  31, 255, 255, 255,  15,   0,   0,
    255, 255, 255,   0,   1,   0, 255, 255,
};

/* Posix_Punct: 1649 bytes. */

RE_UINT32 re_get_posix_punct(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_posix_punct_stage_1[f] << 5;
    f = code >> 11;
    code ^= f << 11;
    pos = (RE_UINT32)re_posix_punct_stage_2[pos + f] << 3;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_posix_punct_stage_3[pos + f] << 3;
    f = code >> 5;
    code ^= f << 5;
    pos = (RE_UINT32)re_posix_punct_stage_4[pos + f] << 5;
    pos += code;
    value = (re_posix_punct_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* Posix_XDigit. */

static RE_UINT8 re_posix_xdigit_stage_1[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static RE_UINT8 re_posix_xdigit_stage_2[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_posix_xdigit_stage_3[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_posix_xdigit_stage_4[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static RE_UINT8 re_posix_xdigit_stage_5[] = {
      0,   0,   0,   0,   0,   0, 255,   3, 126,   0,   0,   0, 126,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

/* Posix_XDigit: 97 bytes. */

RE_UINT32 re_get_posix_xdigit(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;

    f = ch >> 16;
    code = ch ^ (f << 16);
    pos = (RE_UINT32)re_posix_xdigit_stage_1[f] << 3;
    f = code >> 13;
    code ^= f << 13;
    pos = (RE_UINT32)re_posix_xdigit_stage_2[pos + f] << 3;
    f = code >> 10;
    code ^= f << 10;
    pos = (RE_UINT32)re_posix_xdigit_stage_3[pos + f] << 3;
    f = code >> 7;
    code ^= f << 7;
    pos = (RE_UINT32)re_posix_xdigit_stage_4[pos + f] << 7;
    pos += code;
    value = (re_posix_xdigit_stage_5[pos >> 3] >> (pos & 0x7)) & 0x1;

    return value;
}

/* All_Cases. */

static RE_UINT8 re_all_cases_stage_1[] = {
    0, 1, 2, 2, 2, 3, 2, 4, 5, 2, 2, 2, 2, 2, 2, 6,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_all_cases_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  8,  6,  6,  6,  6,  6,  6,  6,  6,  9, 10, 11, 12,
     6, 13,  6,  6, 14,  6,  6,  6,  6,  6,  6,  6, 15, 16,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 17, 18,  6,  6,  6, 19,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 20,  6,  6,  6, 21,
     6,  6,  6,  6, 22,  6,  6,  6,  6,  6,  6,  6, 23,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6, 24,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6, 25,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_all_cases_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   3,   4,   5,   6,   7,   8,
      0,   0,   0,   0,   0,   0,   9,   0,  10,  11,  12,  13,  14,  15,  16,  17,
     18,  18,  18,  18,  18,  18,  19,  20,  21,  22,  18,  18,  18,  18,  18,  23,
     24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  21,  34,  18,  18,  35,  18,
     18,  18,  18,  18,  36,  18,  37,  38,  39,  18,  40,  41,  42,  43,  44,  45,
     46,  47,  48,  49,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  51,  52,
     53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  18,  18,  18,  64,  65,
     66,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  75,  76,  18,  18,  18,
     77,  78,  18,  18,  18,  18,  18,  18,  79,  80,  18,  18,  18,  18,  18,  18,
     18,  18,  18,  18,  18,  18,  81,  82,  82,  82,  83,   0,  84,  85,  85,  85,
     86,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  87,  87,  87,  87,  88,  89,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  91,  92,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     93,  94,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  95,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  96,  18,  18,  18,
     18,  18,  97,  98,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,
     99, 100,  91,  92,  99, 100,  99, 100,  91,  92, 101, 102,  99, 100, 103, 104,
     99, 100,  99, 100,  99, 100, 105, 106, 107, 108, 109, 110, 111, 112, 107, 113,
      0,   0,   0,   0, 114, 115, 116,   0,   0, 117,   0,   0, 118, 118, 119, 119,
    120,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 121, 122, 122, 122, 123, 123, 123, 124,   0,   0,
     82,  82,  82,  82,  82,  83,  85,  85,  85,  85,  85,  86, 125, 126, 127, 128,
     18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  37, 129, 130,   0,
    131, 131, 131, 131, 132, 133,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  18, 134,  18,  18,  18,  97,   0,   0,
     18,  18,  18,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  78,  18,  78,  18,  18,  18,  18,  18,  18,  18,   0, 135,
     18, 136,  51,  18,  18, 137, 138,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 139,   0,   0,   0, 140, 140,
    140, 140, 140, 140, 140, 140, 140, 140,   0,   0,   0,   0,   0,   0,   0,   0,
    141,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   1,  11,  11,   4,   5,  15,  15,   8,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    142, 142, 142, 142, 142, 143, 143, 143, 143, 143,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 142, 142, 142, 142, 144, 143, 143, 143, 143, 145,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    146, 146, 146, 146, 146, 146, 147,   0, 148, 148, 148, 148, 148, 148, 149,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  11,  11,  11,  11,  15,  15,  15,  15,   0,   0,   0,   0,
    150, 150, 150, 150, 151, 152, 152, 152, 153,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_all_cases_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,
      1,   2,   1,   3,   1,   1,   1,   1,   1,   1,   1,   4,   1,   1,   1,   1,
      1,   1,   1,   0,   0,   0,   0,   0,   0,   5,   5,   5,   5,   5,   5,   5,
      5,   6,   5,   7,   5,   5,   5,   5,   5,   5,   5,   8,   5,   5,   5,   5,
      5,   5,   5,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9,   0,   0,
      1,   1,   1,   1,   1,  10,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,   0,   1,   1,   1,   1,   1,   1,   1,  11,
      5,   5,   5,   5,   5,  12,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
      5,   5,   5,   5,   5,   5,   5,   0,   5,   5,   5,   5,   5,   5,   5,  13,
     14,  15,  14,  15,  14,  15,  14,  15,  16,  17,  14,  15,  14,  15,  14,  15,
      0,  14,  15,  14,  15,  14,  15,  14,  15,  14,  15,  14,  15,  14,  15,  14,
     15,   0,  14,  15,  14,  15,  14,  15,  18,  14,  15,  14,  15,  14,  15,  19,
     20,  21,  14,  15,  14,  15,  22,  14,  15,  23,  23,  14,  15,   0,  24,  25,
     26,  14,  15,  23,  27,  28,  29,  30,  14,  15,  31,   0,  29,  32,  33,  34,
     14,  15,  14,  15,  14,  15,  35,  14,  15,  35,   0,   0,  14,  15,  35,  14,
     15,  36,  36,  14,  15,  14,  15,  37,  14,  15,   0,   0,  14,  15,   0,  38,
      0,   0,   0,   0,  39,  40,  41,  39,  40,  41,  39,  40,  41,  14,  15,  14,
     15,  14,  15,  14,  15,  42,  14,  15,   0,  39,  40,  41,  14,  15,  43,  44,
     45,   0,  14,  15,  14,  15,  14,  15,  14,  15,  14,  15,   0,   0,   0,   0,
      0,   0,  46,  14,  15,  47,  48,  49,  49,  14,  15,  50,  51,  52,  14,  15,
     53,  54,  55,  56,  57,   0,  58,  58,   0,  59,   0,  60,  61,   0,   0,   0,
     58,  62,   0,  63,   0,  64,  65,   0,  66,  67,  65,  68,  69,   0,   0,  67,
      0,  70,  71,   0,   0,  72,   0,   0,   0,   0,   0,   0,   0,  73,   0,   0,
     74,   0,   0,  74,   0,   0,   0,  75,  74,  76,  77,  77,  78,   0,   0,   0,
      0,   0,  79,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  80,  81,   0,
      0,   0,   0,   0,   0,  82,   0,   0,  14,  15,  14,  15,   0,   0,  14,  15,
      0,   0,   0,  33,  33,  33,   0,  83,   0,   0,   0,   0,   0,   0,  84,   0,
     85,  85,  85,   0,  86,   0,  87,  87,  88,   1,  89,   1,   1,  90,   1,   1,
     91,  92,  93,   1,  94,   1,   1,   1,  95,  96,   0,  97,   1,   1,  98,   1,
      1,  99,   1,   1, 100, 101, 101, 101, 102,   5, 103,   5,   5, 104,   5,   5,
    105, 106, 107,   5, 108,   5,   5,   5, 109, 110, 111, 112,   5,   5, 113,   5,
      5, 114,   5,   5, 115, 116, 116, 117, 118, 119,   0,   0,   0, 120, 121, 122,
    123, 124, 125, 126, 127, 128,   0,  14,  15, 129,  14,  15,   0,  45,  45,  45,
    130, 130, 130, 130, 130, 130, 130, 130,   1,   1, 131,   1, 132,   1,   1,   1,
      1,   1,   1,   1,   1,   1, 133,   1,   1, 134, 135,   1,   1,   1,   1,   1,
      1,   1, 136,   1,   1,   1,   1,   1,   5,   5, 137,   5, 138,   5,   5,   5,
      5,   5,   5,   5,   5,   5, 139,   5,   5, 140, 141,   5,   5,   5,   5,   5,
      5,   5, 142,   5,   5,   5,   5,   5, 143, 143, 143, 143, 143, 143, 143, 143,
     14,  15, 144, 145,  14,  15,  14,  15,  14,  15,   0,   0,   0,   0,   0,   0,
      0,   0,  14,  15,  14,  15,  14,  15, 146,  14,  15,  14,  15,  14,  15,  14,
     15,  14,  15,  14,  15,  14,  15, 147,   0, 148, 148, 148, 148, 148, 148, 148,
    148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148,   0,
      0, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
    149, 149, 149, 149, 149, 149, 149,   0, 150, 150, 150, 150, 150, 150, 150, 150,
    150, 150, 150, 150, 150, 150,   0, 150,   0,   0,   0,   0,   0, 150,   0,   0,
    151, 151, 151, 151, 151, 151, 151, 151, 117, 117, 117, 117, 117, 117,   0,   0,
    122, 122, 122, 122, 122, 122,   0,   0, 152, 153, 154, 155, 156, 157, 158, 159,
    160,   0,   0,   0,   0,   0,   0,   0,   0, 161,   0,   0,   0, 162,   0,   0,
    163, 164,  14,  15,  14,  15,  14,  15,  14,  15,  14,  15,  14,  15,   0,   0,
      0,   0,   0, 165,   0,   0, 166,   0, 117, 117, 117, 117, 117, 117, 117, 117,
    122, 122, 122, 122, 122, 122, 122, 122,   0, 117,   0, 117,   0, 117,   0, 117,
      0, 122,   0, 122,   0, 122,   0, 122, 167, 167, 168, 168, 168, 168, 169, 169,
    170, 170, 171, 171, 172, 172,   0,   0, 117, 117,   0, 173,   0,   0,   0,   0,
    122, 122, 174, 174, 175,   0, 176,   0,   0,   0,   0, 173,   0,   0,   0,   0,
    177, 177, 177, 177, 175,   0,   0,   0, 117, 117,   0, 178,   0,   0,   0,   0,
    122, 122, 179, 179,   0,   0,   0,   0, 117, 117,   0, 180,   0, 125,   0,   0,
    122, 122, 181, 181, 129,   0,   0,   0, 182, 182, 183, 183, 175,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 184,   0,   0,   0, 185, 186,   0,   0,   0,   0,
      0,   0, 187,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 188,   0,
    189, 189, 189, 189, 189, 189, 189, 189, 190, 190, 190, 190, 190, 190, 190, 190,
      0,   0,   0,  14,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0, 191, 191,
    191, 191, 191, 191, 191, 191, 191, 191, 192, 192, 192, 192, 192, 192, 192, 192,
    192, 192,   0,   0,   0,   0,   0,   0,  14,  15, 193, 194, 195, 196, 197,  14,
     15,  14,  15,  14,  15, 198, 199, 200, 201,   0,  14,  15,   0,  14,  15,   0,
      0,   0,   0,   0,   0,   0, 202, 202,   0,   0,   0,  14,  15,  14,  15,   0,
      0,   0,  14,  15,   0,   0,   0,   0, 203, 203, 203, 203, 203, 203, 203, 203,
    203, 203, 203, 203, 203, 203,   0, 203,   0,   0,   0,   0,   0, 203,   0,   0,
     14,  15, 204, 205,  14,  15,  14,  15,   0,  14,  15,  14,  15, 206,  14,  15,
      0,   0,   0,  14,  15, 207,   0,   0,  14,  15, 208, 209, 210, 211, 208,   0,
    212, 213, 214, 215,  14,  15,  14,  15,   0,   0,   0, 216,   0,   0,   0,   0,
    217, 217, 217, 217, 217, 217, 217, 217,   0,   0,   0,   0,   0,  14,  15,   0,
    218, 218, 218, 218, 218, 218, 218, 218, 219, 219, 219, 219, 219, 219, 219, 219,
    218, 218, 218, 218,   0,   0,   0,   0, 219, 219, 219, 219,   0,   0,   0,   0,
     86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,   0,   0,   0,   0,   0,
    115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,   0,   0,   0,   0,   0,
    220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 221, 221, 221, 221, 221, 221,
    221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221,   0,   0,   0,   0,
};

/* All_Cases: 2424 bytes. */

static RE_AllCases re_all_cases_table[] = {
    {{     0,     0,     0}},
    {{    32,     0,     0}},
    {{    32,   232,     0}},
    {{    32,  8415,     0}},
    {{    32,   300,     0}},
    {{   -32,     0,     0}},
    {{   -32,   199,     0}},
    {{   -32,  8383,     0}},
    {{   -32,   268,     0}},
    {{   743,   775,     0}},
    {{    32,  8294,     0}},
    {{  7615,     0,     0}},
    {{   -32,  8262,     0}},
    {{   121,     0,     0}},
    {{     1,     0,     0}},
    {{    -1,     0,     0}},
    {{  -199,     0,     0}},
    {{  -232,     0,     0}},
    {{  -121,     0,     0}},
    {{  -300,  -268,     0}},
    {{   195,     0,     0}},
    {{   210,     0,     0}},
    {{   206,     0,     0}},
    {{   205,     0,     0}},
    {{    79,     0,     0}},
    {{   202,     0,     0}},
    {{   203,     0,     0}},
    {{   207,     0,     0}},
    {{    97,     0,     0}},
    {{   211,     0,     0}},
    {{   209,     0,     0}},
    {{   163,     0,     0}},
    {{   213,     0,     0}},
    {{   130,     0,     0}},
    {{   214,     0,     0}},
    {{   218,     0,     0}},
    {{   217,     0,     0}},
    {{   219,     0,     0}},
    {{    56,     0,     0}},
    {{     1,     2,     0}},
    {{    -1,     1,     0}},
    {{    -2,    -1,     0}},
    {{   -79,     0,     0}},
    {{   -97,     0,     0}},
    {{   -56,     0,     0}},
    {{  -130,     0,     0}},
    {{ 10795,     0,     0}},
    {{  -163,     0,     0}},
    {{ 10792,     0,     0}},
    {{ 10815,     0,     0}},
    {{  -195,     0,     0}},
    {{    69,     0,     0}},
    {{    71,     0,     0}},
    {{ 10783,     0,     0}},
    {{ 10780,     0,     0}},
    {{ 10782,     0,     0}},
    {{  -210,     0,     0}},
    {{  -206,     0,     0}},
    {{  -205,     0,     0}},
    {{  -202,     0,     0}},
    {{  -203,     0,     0}},
    {{ 42319,     0,     0}},
    {{ 42315,     0,     0}},
    {{  -207,     0,     0}},
    {{ 42280,     0,     0}},
    {{ 42308,     0,     0}},
    {{  -209,     0,     0}},
    {{  -211,     0,     0}},
    {{ 10743,     0,     0}},
    {{ 42305,     0,     0}},
    {{ 10749,     0,     0}},
    {{  -213,     0,     0}},
    {{  -214,     0,     0}},
    {{ 10727,     0,     0}},
    {{  -218,     0,     0}},
    {{ 42282,     0,     0}},
    {{   -69,     0,     0}},
    {{  -217,     0,     0}},
    {{   -71,     0,     0}},
    {{  -219,     0,     0}},
    {{ 42261,     0,     0}},
    {{ 42258,     0,     0}},
    {{    84,   116,  7289}},
    {{   116,     0,     0}},
    {{    38,     0,     0}},
    {{    37,     0,     0}},
    {{    64,     0,     0}},
    {{    63,     0,     0}},
    {{  7235,     0,     0}},
    {{    32,    62,     0}},
    {{    32,    96,     0}},
    {{    32,    57,    92}},
    {{   -84,    32,  7205}},
    {{    32,    86,     0}},
    {{  -743,    32,     0}},
    {{    32,    54,     0}},
    {{    32,    80,     0}},
    {{    31,    32,     0}},
    {{    32,    47,     0}},
    {{    32,  7549,     0}},
    {{   -38,     0,     0}},
    {{   -37,     0,     0}},
    {{  7219,     0,     0}},
    {{   -32,    30,     0}},
    {{   -32,    64,     0}},
    {{   -32,    25,    60}},
    {{  -116,   -32,  7173}},
    {{   -32,    54,     0}},
    {{  -775,   -32,     0}},
    {{   -32,    22,     0}},
    {{   -32,    48,     0}},
    {{   -31,     1,     0}},
    {{   -32,    -1,     0}},
    {{   -32,    15,     0}},
    {{   -32,  7517,     0}},
    {{   -64,     0,     0}},
    {{   -63,     0,     0}},
    {{     8,     0,     0}},
    {{   -62,   -30,     0}},
    {{   -57,   -25,    35}},
    {{   -47,   -15,     0}},
    {{   -54,   -22,     0}},
    {{    -8,     0,     0}},
    {{   -86,   -54,     0}},
    {{   -80,   -48,     0}},
    {{     7,     0,     0}},
    {{  -116,     0,     0}},
    {{   -92,   -60,   -35}},
    {{   -96,   -64,     0}},
    {{    -7,     0,     0}},
    {{    80,     0,     0}},
    {{    32,  6254,     0}},
    {{    32,  6253,     0}},
    {{    32,  6244,     0}},
    {{    32,  6242,     0}},
    {{    32,  6242,  6243}},
    {{    32,  6236,     0}},
    {{   -32,  6222,     0}},
    {{   -32,  6221,     0}},
    {{   -32,  6212,     0}},
    {{   -32,  6210,     0}},
    {{   -32,  6210,  6211}},
    {{   -32,  6204,     0}},
    {{   -80,     0,     0}},
    {{     1,  6181,     0}},
    {{    -1,  6180,     0}},
    {{    15,     0,     0}},
    {{   -15,     0,     0}},
    {{    48,     0,     0}},
    {{   -48,     0,     0}},
    {{  7264,     0,     0}},
    {{ 38864,     0,     0}},
    {{ -6254, -6222,     0}},
    {{ -6253, -6221,     0}},
    {{ -6244, -6212,     0}},
    {{ -6242, -6210,     0}},
    {{ -6242, -6210,     1}},
    {{ -6243, -6211,    -1}},
    {{ -6236, -6204,     0}},
    {{ -6181, -6180,     0}},
    {{ 35266, 35267,     0}},
    {{ 35332,     0,     0}},
    {{  3814,     0,     0}},
    {{     1,    59,     0}},
    {{    -1,    58,     0}},
    {{   -59,   -58,     0}},
    {{ -7615,     0,     0}},
    {{    74,     0,     0}},
    {{    86,     0,     0}},
    {{   100,     0,     0}},
    {{   128,     0,     0}},
    {{   112,     0,     0}},
    {{   126,     0,     0}},
    {{     9,     0,     0}},
    {{   -74,     0,     0}},
    {{    -9,     0,     0}},
    {{ -7289, -7205, -7173}},
    {{   -86,     0,     0}},
    {{ -7235,     0,     0}},
    {{  -100,     0,     0}},
    {{ -7219,     0,     0}},
    {{  -112,     0,     0}},
    {{  -128,     0,     0}},
    {{  -126,     0,     0}},
    {{ -7549, -7517,     0}},
    {{ -8415, -8383,     0}},
    {{ -8294, -8262,     0}},
    {{    28,     0,     0}},
    {{   -28,     0,     0}},
    {{    16,     0,     0}},
    {{   -16,     0,     0}},
    {{    26,     0,     0}},
    {{   -26,     0,     0}},
    {{-10743,     0,     0}},
    {{ -3814,     0,     0}},
    {{-10727,     0,     0}},
    {{-10795,     0,     0}},
    {{-10792,     0,     0}},
    {{-10780,     0,     0}},
    {{-10749,     0,     0}},
    {{-10783,     0,     0}},
    {{-10782,     0,     0}},
    {{-10815,     0,     0}},
    {{ -7264,     0,     0}},
    {{-35266,     1,     0}},
    {{-35267,    -1,     0}},
    {{-35332,     0,     0}},
    {{-42280,     0,     0}},
    {{-42308,     0,     0}},
    {{-42319,     0,     0}},
    {{-42315,     0,     0}},
    {{-42305,     0,     0}},
    {{-42258,     0,     0}},
    {{-42282,     0,     0}},
    {{-42261,     0,     0}},
    {{   928,     0,     0}},
    {{  -928,     0,     0}},
    {{-38864,     0,     0}},
    {{    40,     0,     0}},
    {{   -40,     0,     0}},
    {{    34,     0,     0}},
    {{   -34,     0,     0}},
};

/* All_Cases: 2664 bytes. */

int re_get_all_cases(RE_UINT32 ch, RE_UINT32* codepoints) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;
    RE_AllCases* all_cases;
    int count;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_all_cases_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_all_cases_stage_2[pos + f] << 5;
    f = code >> 3;
    code ^= f << 3;
    pos = (RE_UINT32)re_all_cases_stage_3[pos + f] << 3;
    value = re_all_cases_stage_4[pos + code];

    all_cases = &re_all_cases_table[value];

    codepoints[0] = ch;
    count = 1;

    while (count < RE_MAX_CASES && all_cases->diffs[count - 1] != 0) {
        codepoints[count] = (RE_UINT32)((RE_INT32)ch + all_cases->diffs[count -
          1]);
        ++count;
    }

    return count;
}

/* Simple_Case_Folding. */

static RE_UINT8 re_simple_case_folding_stage_1[] = {
    0, 1, 2, 2, 2, 3, 2, 4, 5, 2, 2, 2, 2, 2, 2, 6,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_simple_case_folding_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  8,  6,  6,  6,  6,  6,  6,  6,  6,  9,  6, 10, 11,
     6, 12,  6,  6, 13,  6,  6,  6,  6,  6,  6,  6, 14,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 15, 16,  6,  6,  6, 17,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 18,
     6,  6,  6,  6, 19,  6,  6,  6,  6,  6,  6,  6, 20,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6, 21,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6, 22,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_simple_case_folding_stage_3[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  2,  3,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  4,  0,  2,  2,  5,  5,  0,  0,  0,  0,
     6,  6,  6,  6,  6,  6,  7,  8,  8,  7,  6,  6,  6,  6,  6,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,  8, 20,  6,  6, 21,  6,
     6,  6,  6,  6, 22,  6, 23, 24, 25,  6,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0, 26,  0,  0,  0,  0,  0, 27, 28,
    29, 30,  1,  2, 31, 32,  0,  0, 33, 34, 35,  6,  6,  6, 36, 37,
    38, 38,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  6,  6,  6,  6,
    39,  7,  6,  6,  6,  6,  6,  6, 40, 41,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 42, 43, 43, 43, 44,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0, 45, 45, 45, 45, 46, 47,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    49, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6, 51, 52,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     0, 53,  0, 48,  0, 53,  0, 53,  0, 48,  0, 54,  0, 53,  0,  0,
     0, 53,  0, 53,  0, 53,  0, 55,  0, 56,  0, 57,  0, 58,  0, 59,
     0,  0,  0,  0, 60, 61, 62,  0,  0,  0,  0,  0, 63, 63,  0,  0,
    64,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 65, 66, 66, 66,  0,  0,  0,  0,  0,  0,
    43, 43, 43, 43, 43, 44,  0,  0,  0,  0,  0,  0, 67, 68, 69, 70,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 23, 71, 33,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  6,  6,  6, 51,  0,  0,
     6,  6,  6, 23,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  7,  6,  7,  6,  6,  6,  6,  6,  6,  6,  0, 72,
     6, 73, 27,  6,  6, 74, 75,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 76, 76,
    76, 76, 76, 76, 76, 76, 76, 76,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  1,  2,  2,  3,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    77, 77, 77, 77, 77,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 77, 77, 77, 77, 78,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    79, 79, 79, 79, 79, 79, 80,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
    81, 81, 81, 81, 82,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static RE_UINT8 re_simple_case_folding_stage_4[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  2,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,
     3,  0,  3,  0,  3,  0,  3,  0,  0,  0,  3,  0,  3,  0,  3,  0,
     0,  3,  0,  3,  0,  3,  0,  3,  4,  3,  0,  3,  0,  3,  0,  5,
     0,  6,  3,  0,  3,  0,  7,  3,  0,  8,  8,  3,  0,  0,  9, 10,
    11,  3,  0,  8, 12,  0, 13, 14,  3,  0,  0,  0, 13, 15,  0, 16,
     3,  0,  3,  0,  3,  0, 17,  3,  0, 17,  0,  0,  3,  0, 17,  3,
     0, 18, 18,  3,  0,  3,  0, 19,  3,  0,  0,  0,  3,  0,  0,  0,
     0,  0,  0,  0, 20,  3,  0, 20,  3,  0, 20,  3,  0,  3,  0,  3,
     0,  3,  0,  3,  0,  0,  3,  0,  0, 20,  3,  0,  3,  0, 21, 22,
    23,  0,  3,  0,  3,  0,  3,  0,  3,  0,  3,  0,  0,  0,  0,  0,
     0,  0, 24,  3,  0, 25, 26,  0,  0,  3,  0, 27, 28, 29,  3,  0,
     0,  0,  0,  0,  0, 30,  0,  0,  3,  0,  3,  0,  0,  0,  3,  0,
     0,  0,  0,  0,  0,  0,  0, 30,  0,  0,  0,  0,  0,  0, 31,  0,
    32, 32, 32,  0, 33,  0, 34, 34,  1,  1,  0,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0, 35, 36, 37,  0,  0,  0, 38, 39,  0,
    40, 41,  0,  0, 42, 43,  0,  3,  0, 44,  3,  0,  0, 23, 23, 23,
    45, 45, 45, 45, 45, 45, 45, 45,  3,  0,  0,  0,  0,  0,  0,  0,
    46,  3,  0,  3,  0,  3,  0,  3,  0,  3,  0,  3,  0,  3,  0,  0,
     0, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47,
    47, 47, 47, 47, 47, 47, 47,  0, 48, 48, 48, 48, 48, 48, 48, 48,
    48, 48, 48, 48, 48, 48,  0, 48,  0,  0,  0,  0,  0, 48,  0,  0,
    49, 49, 49, 49, 49, 49,  0,  0, 50, 51, 52, 53, 53, 54, 55, 56,
    57,  0,  0,  0,  0,  0,  0,  0,  3,  0,  3,  0,  3,  0,  0,  0,
     0,  0,  0, 58,  0,  0, 59,  0, 49, 49, 49, 49, 49, 49, 49, 49,
     0, 49,  0, 49,  0, 49,  0, 49, 49, 49, 60, 60, 61,  0, 62,  0,
    63, 63, 63, 63, 61,  0,  0,  0, 49, 49, 64, 64,  0,  0,  0,  0,
    49, 49, 65, 65, 44,  0,  0,  0, 66, 66, 67, 67, 61,  0,  0,  0,
     0,  0,  0,  0,  0,  0, 68,  0,  0,  0, 69, 70,  0,  0,  0,  0,
     0,  0, 71,  0,  0,  0,  0,  0, 72, 72, 72, 72, 72, 72, 72, 72,
     0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 73, 73,
    73, 73, 73, 73, 73, 73, 73, 73,  3,  0, 74, 75, 76,  0,  0,  3,
     0,  3,  0,  3,  0, 77, 78, 79, 80,  0,  3,  0,  0,  3,  0,  0,
     0,  0,  0,  0,  0,  0, 81, 81,  0,  0,  0,  3,  0,  3,  0,  0,
     0,  3,  0,  3,  0, 82,  3,  0,  0,  0,  0,  3,  0, 83,  0,  0,
     3,  0, 84, 85, 86, 87, 84,  0, 88, 89, 90, 91,  3,  0,  3,  0,
    92, 92, 92, 92, 92, 92, 92, 92, 93, 93, 93, 93, 93, 93, 93, 93,
    93, 93, 93, 93,  0,  0,  0,  0, 33, 33, 33, 33, 33, 33, 33, 33,
    33, 33, 33,  0,  0,  0,  0,  0, 94, 94, 94, 94, 94, 94, 94, 94,
    94, 94,  0,  0,  0,  0,  0,  0,
};

/* Simple_Case_Folding: 1760 bytes. */

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
        -8,
     -6222,
     -6221,
     -6212,
     -6210,
     -6211,
     -6204,
     -6180,
     35267,
       -58,
     -7615,
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
    -42319,
    -42315,
    -42305,
    -42258,
    -42282,
    -42261,
       928,
    -38864,
        40,
        34,
};

/* Simple_Case_Folding: 380 bytes. */

RE_UINT32 re_get_simple_case_folding(RE_UINT32 ch) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;
    RE_INT32 diff;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_simple_case_folding_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_simple_case_folding_stage_2[pos + f] << 5;
    f = code >> 3;
    code ^= f << 3;
    pos = (RE_UINT32)re_simple_case_folding_stage_3[pos + f] << 3;
    value = re_simple_case_folding_stage_4[pos + code];

    diff = re_simple_case_folding_table[value];

    return (RE_UINT32)((RE_INT32)ch + diff);
}

/* Full_Case_Folding. */

static RE_UINT8 re_full_case_folding_stage_1[] = {
    0, 1, 2, 2, 2, 3, 2, 4, 5, 2, 2, 2, 2, 2, 2, 6,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

static RE_UINT8 re_full_case_folding_stage_2[] = {
     0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     7,  6,  6,  8,  6,  6,  6,  6,  6,  6,  6,  6,  9,  6, 10, 11,
     6, 12,  6,  6, 13,  6,  6,  6,  6,  6,  6,  6, 14,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6, 15, 16,  6,  6,  6, 17,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 18,  6,  6,  6, 19,
     6,  6,  6,  6, 20,  6,  6,  6,  6,  6,  6,  6, 21,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6, 22,  6,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6, 23,  6,  6,  6,  6,  6,  6,
     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
};

static RE_UINT8 re_full_case_folding_stage_3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   2,   3,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   4,   0,   2,   2,   5,   6,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   8,   9,   9,  10,   7,   7,   7,   7,   7,  11,
     12,  13,  14,  15,  16,  17,  18,  19,  20,  21,   9,  22,   7,   7,  23,   7,
      7,   7,   7,   7,  24,   7,  25,  26,  27,   7,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,  28,   0,   0,   0,   0,   0,  29,  30,
     31,  32,  33,   2,  34,  35,  36,   0,  37,  38,  39,   7,   7,   7,  40,  41,
     42,  42,   2,   2,   2,   2,   0,   0,   0,   0,   0,   0,   7,   7,   7,   7,
     43,  44,   7,   7,   7,   7,   7,   7,  45,  46,   7,   7,   7,   7,   7,   7,
      7,   7,   7,   7,   7,   7,  47,  48,  48,  48,  49,   0,   0,   0,   0,   0,
     50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  51,  51,  51,  51,  52,  53,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  54,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     55,  56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      7,   7,  57,  58,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      0,  59,   0,  54,   0,  59,   0,  59,   0,  54,  60,  61,   0,  59,   0,   0,
     62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,
      0,   0,   0,   0,  78,  79,  80,   0,   0,   0,   0,   0,  81,  81,   0,   0,
     82,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  83,  84,  84,  84,   0,   0,   0,   0,   0,   0,
     48,  48,  48,  48,  48,  49,   0,   0,   0,   0,   0,   0,  85,  86,  87,  88,
      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  25,  89,  37,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   7,   7,   7,   7,   7,  90,   0,   0,
      7,   7,   7,  25,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,  44,   7,  44,   7,   7,   7,   7,   7,   7,   7,   0,  91,
      7,  92,  29,   7,   7,  93,  94,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  95,  95,
     95,  95,  95,  95,  95,  95,  95,  95,   0,   0,   0,   0,   0,   0,   0,   0,
     96,   0,  97,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   1,   2,   2,   3,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     98,  98,  98,  98,  98,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  98,  98,  98,  98,  99,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    100, 100, 100, 100, 100, 100, 101,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   2,   2,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,
    102, 102, 102, 102, 103,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static RE_UINT8 re_full_case_folding_stage_4[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   2,   0,   0,   1,   1,   1,   1,   1,   1,   1,   0,
      1,   1,   1,   1,   1,   1,   1,   3,   4,   0,   4,   0,   4,   0,   4,   0,
      5,   0,   4,   0,   4,   0,   4,   0,   0,   4,   0,   4,   0,   4,   0,   4,
      0,   6,   4,   0,   4,   0,   4,   0,   7,   4,   0,   4,   0,   4,   0,   8,
      0,   9,   4,   0,   4,   0,  10,   4,   0,  11,  11,   4,   0,   0,  12,  13,
     14,   4,   0,  11,  15,   0,  16,  17,   4,   0,   0,   0,  16,  18,   0,  19,
      4,   0,   4,   0,   4,   0,  20,   4,   0,  20,   0,   0,   4,   0,  20,   4,
      0,  21,  21,   4,   0,   4,   0,  22,   4,   0,   0,   0,   4,   0,   0,   0,
      0,   0,   0,   0,  23,   4,   0,  23,   4,   0,  23,   4,   0,   4,   0,   4,
      0,   4,   0,   4,   0,   0,   4,   0,  24,  23,   4,   0,   4,   0,  25,  26,
     27,   0,   4,   0,   4,   0,   4,   0,   4,   0,   4,   0,   0,   0,   0,   0,
      0,   0,  28,   4,   0,  29,  30,   0,   0,   4,   0,  31,  32,  33,   4,   0,
      0,   0,   0,   0,   0,  34,   0,   0,   4,   0,   4,   0,   0,   0,   4,   0,
      0,   0,   0,   0,   0,   0,   0,  34,   0,   0,   0,   0,   0,   0,  35,   0,
     36,  36,  36,   0,  37,   0,  38,  38,  39,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   0,   0,   0,   0,
     40,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,  41,  42,  43,   0,   0,   0,  44,  45,   0,
     46,  47,   0,   0,  48,  49,   0,   4,   0,  50,   4,   0,   0,  27,  27,  27,
     51,  51,  51,  51,  51,  51,  51,  51,   4,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   4,   0,   4,   0,   4,   0,  52,   4,   0,   4,   0,   4,   0,   4,
      0,   4,   0,   4,   0,   4,   0,   0,   0,  53,  53,  53,  53,  53,  53,  53,
     53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,   0,
      0,   0,   0,   0,   0,   0,   0,  54,  55,  55,  55,  55,  55,  55,  55,  55,
     55,  55,  55,  55,  55,  55,   0,  55,   0,   0,   0,   0,   0,  55,   0,   0,
     56,  56,  56,  56,  56,  56,   0,   0,  57,  58,  59,  60,  60,  61,  62,  63,
     64,   0,   0,   0,   0,   0,   0,   0,   4,   0,   4,   0,   4,   0,  65,  66,
     67,  68,  69,  70,   0,   0,  71,   0,  56,  56,  56,  56,  56,  56,  56,  56,
     72,   0,  73,   0,  74,   0,  75,   0,   0,  56,   0,  56,   0,  56,   0,  56,
     76,  76,  76,  76,  76,  76,  76,  76,  77,  77,  77,  77,  77,  77,  77,  77,
     78,  78,  78,  78,  78,  78,  78,  78,  79,  79,  79,  79,  79,  79,  79,  79,
     80,  80,  80,  80,  80,  80,  80,  80,  81,  81,  81,  81,  81,  81,  81,  81,
      0,   0,  82,  83,  84,   0,  85,  86,  56,  56,  87,  87,  88,   0,  89,   0,
      0,   0,  90,  91,  92,   0,  93,  94,  95,  95,  95,  95,  96,   0,   0,   0,
      0,   0,  97,  98,   0,   0,  99, 100,  56,  56, 101, 101,   0,   0,   0,   0,
      0,   0, 102, 103, 104,   0, 105, 106,  56,  56, 107, 107,  50,   0,   0,   0,
      0,   0, 108, 109, 110,   0, 111, 112, 113, 113, 114, 114, 115,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 116,   0,   0,   0, 117, 118,   0,   0,   0,   0,
      0,   0, 119,   0,   0,   0,   0,   0, 120, 120, 120, 120, 120, 120, 120, 120,
      0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 121, 121,
    121, 121, 121, 121, 121, 121, 121, 121,   4,   0, 122, 123, 124,   0,   0,   4,
      0,   4,   0,   4,   0, 125, 126, 127, 128,   0,   4,   0,   0,   4,   0,   0,
      0,   0,   0,   0,   0,   0, 129, 129,   0,   0,   0,   4,   0,   4,   0,   0,
      4,   0,   4,   0,   4,   0,   0,   0,   0,   4,   0,   4,   0, 130,   4,   0,
      0,   0,   0,   4,   0, 131,   0,   0,   4,   0, 132, 133, 134, 135, 132,   0,
    136, 137, 138, 139,   4,   0,   4,   0, 140, 140, 140, 140, 140, 140, 140, 140,
    141, 142, 143, 144, 145, 146, 147,   0,   0,   0,   0, 148, 149, 150, 151, 152,
    153, 153, 153, 153, 153, 153, 153, 153, 153, 153, 153, 153,   0,   0,   0,   0,
     37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,   0,   0,   0,   0,   0,
    154, 154, 154, 154, 154, 154, 154, 154, 154, 154,   0,   0,   0,   0,   0,   0,
};

/* Full_Case_Folding: 1960 bytes. */

static RE_FullCaseFolding re_full_case_folding_table[] = {
    {     0, {   0,   0}},
    {    32, {   0,   0}},
    {   775, {   0,   0}},
    {  -108, { 115,   0}},
    {     1, {   0,   0}},
    {  -199, { 775,   0}},
    {   371, { 110,   0}},
    {  -121, {   0,   0}},
    {  -268, {   0,   0}},
    {   210, {   0,   0}},
    {   206, {   0,   0}},
    {   205, {   0,   0}},
    {    79, {   0,   0}},
    {   202, {   0,   0}},
    {   203, {   0,   0}},
    {   207, {   0,   0}},
    {   211, {   0,   0}},
    {   209, {   0,   0}},
    {   213, {   0,   0}},
    {   214, {   0,   0}},
    {   218, {   0,   0}},
    {   217, {   0,   0}},
    {   219, {   0,   0}},
    {     2, {   0,   0}},
    {  -390, { 780,   0}},
    {   -97, {   0,   0}},
    {   -56, {   0,   0}},
    {  -130, {   0,   0}},
    { 10795, {   0,   0}},
    {  -163, {   0,   0}},
    { 10792, {   0,   0}},
    {  -195, {   0,   0}},
    {    69, {   0,   0}},
    {    71, {   0,   0}},
    {   116, {   0,   0}},
    {    38, {   0,   0}},
    {    37, {   0,   0}},
    {    64, {   0,   0}},
    {    63, {   0,   0}},
    {    41, { 776, 769}},
    {    21, { 776, 769}},
    {     8, {   0,   0}},
    {   -30, {   0,   0}},
    {   -25, {   0,   0}},
    {   -15, {   0,   0}},
    {   -22, {   0,   0}},
    {   -54, {   0,   0}},
    {   -48, {   0,   0}},
    {   -60, {   0,   0}},
    {   -64, {   0,   0}},
    {    -7, {   0,   0}},
    {    80, {   0,   0}},
    {    15, {   0,   0}},
    {    48, {   0,   0}},
    {   -34, {1410,   0}},
    {  7264, {   0,   0}},
    {    -8, {   0,   0}},
    { -6222, {   0,   0}},
    { -6221, {   0,   0}},
    { -6212, {   0,   0}},
    { -6210, {   0,   0}},
    { -6211, {   0,   0}},
    { -6204, {   0,   0}},
    { -6180, {   0,   0}},
    { 35267, {   0,   0}},
    { -7726, { 817,   0}},
    { -7715, { 776,   0}},
    { -7713, { 778,   0}},
    { -7712, { 778,   0}},
    { -7737, { 702,   0}},
    {   -58, {   0,   0}},
    { -7723, { 115,   0}},
    { -7051, { 787,   0}},
    { -7053, { 787, 768}},
    { -7055, { 787, 769}},
    { -7057, { 787, 834}},
    {  -128, { 953,   0}},
    {  -136, { 953,   0}},
    {  -112, { 953,   0}},
    {  -120, { 953,   0}},
    {   -64, { 953,   0}},
    {   -72, { 953,   0}},
    {   -66, { 953,   0}},
    { -7170, { 953,   0}},
    { -7176, { 953,   0}},
    { -7173, { 834,   0}},
    { -7174, { 834, 953}},
    {   -74, {   0,   0}},
    { -7179, { 953,   0}},
    { -7173, {   0,   0}},
    {   -78, { 953,   0}},
    { -7180, { 953,   0}},
    { -7190, { 953,   0}},
    { -7183, { 834,   0}},
    { -7184, { 834, 953}},
    {   -86, {   0,   0}},
    { -7189, { 953,   0}},
    { -7193, { 776, 768}},
    { -7194, { 776, 769}},
    { -7197, { 834,   0}},
    { -7198, { 776, 834}},
    {  -100, {   0,   0}},
    { -7197, { 776, 768}},
    { -7198, { 776, 769}},
    { -7203, { 787,   0}},
    { -7201, { 834,   0}},
    { -7202, { 776, 834}},
    {  -112, {   0,   0}},
    {  -118, { 953,   0}},
    { -7210, { 953,   0}},
    { -7206, { 953,   0}},
    { -7213, { 834,   0}},
    { -7214, { 834, 953}},
    {  -128, {   0,   0}},
    {  -126, {   0,   0}},
    { -7219, { 953,   0}},
    { -7517, {   0,   0}},
    { -8383, {   0,   0}},
    { -8262, {   0,   0}},
    {    28, {   0,   0}},
    {    16, {   0,   0}},
    {    26, {   0,   0}},
    {-10743, {   0,   0}},
    { -3814, {   0,   0}},
    {-10727, {   0,   0}},
    {-10780, {   0,   0}},
    {-10749, {   0,   0}},
    {-10783, {   0,   0}},
    {-10782, {   0,   0}},
    {-10815, {   0,   0}},
    {-35332, {   0,   0}},
    {-42280, {   0,   0}},
    {-42308, {   0,   0}},
    {-42319, {   0,   0}},
    {-42315, {   0,   0}},
    {-42305, {   0,   0}},
    {-42258, {   0,   0}},
    {-42282, {   0,   0}},
    {-42261, {   0,   0}},
    {   928, {   0,   0}},
    {-38864, {   0,   0}},
    {-64154, { 102,   0}},
    {-64155, { 105,   0}},
    {-64156, { 108,   0}},
    {-64157, { 102, 105}},
    {-64158, { 102, 108}},
    {-64146, { 116,   0}},
    {-64147, { 116,   0}},
    {-62879, {1398,   0}},
    {-62880, {1381,   0}},
    {-62881, {1387,   0}},
    {-62872, {1398,   0}},
    {-62883, {1389,   0}},
    {    40, {   0,   0}},
    {    34, {   0,   0}},
};

/* Full_Case_Folding: 1240 bytes. */

int re_get_full_case_folding(RE_UINT32 ch, RE_UINT32* codepoints) {
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;
    RE_FullCaseFolding* case_folding;
    int count;

    f = ch >> 13;
    code = ch ^ (f << 13);
    pos = (RE_UINT32)re_full_case_folding_stage_1[f] << 5;
    f = code >> 8;
    code ^= f << 8;
    pos = (RE_UINT32)re_full_case_folding_stage_2[pos + f] << 5;
    f = code >> 3;
    code ^= f << 3;
    pos = (RE_UINT32)re_full_case_folding_stage_3[pos + f] << 3;
    value = re_full_case_folding_stage_4[pos + code];

    case_folding = &re_full_case_folding_table[value];

    codepoints[0] = (RE_UINT32)((RE_INT32)ch + case_folding->diff);
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
    re_get_sentence_terminal,
    re_get_variation_selector,
    re_get_pattern_white_space,
    re_get_pattern_syntax,
    re_get_prepended_concatenation_mark,
    re_get_regional_indicator,
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
    re_get_indic_positional_category,
    re_get_indic_syllabic_category,
    re_get_nfd_quick_check,
    re_get_nfc_quick_check,
    re_get_nfkd_quick_check,
    re_get_nfkc_quick_check,
    re_get_alphanumeric,
    re_get_any,
    re_get_blank,
    re_get_graph,
    re_get_print,
    re_get_word,
    re_get_xdigit,
    re_get_posix_digit,
    re_get_posix_alnum,
    re_get_posix_punct,
    re_get_posix_xdigit,
};
