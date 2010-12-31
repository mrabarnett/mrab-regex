#
# Secret Labs' Regular Expression Engine core module
#
# Copyright (c) 1998-2001 by Secret Labs AB.  All rights reserved.
#
# This version of the SRE library can be redistributed under CNRI's
# Python 1.6 license.  For any other use, please contact Secret Labs
# AB (info@pythonware.com).
#
# Portions of this engine have been developed in cooperation with
# CNRI.  Hewlett-Packard provided funding for 1.6 integration and
# other compatibility work.
#
# 2010-01-16 mrab Python front-end re-written and extended

# Flags.
A = ASCII = 0x80      # Assume ASCII locale.
D = DEBUG = 0x200     # Print parsed pattern.
I = IGNORECASE = 0x2  # Ignore case.
L = LOCALE = 0x4      # Assume current 8-bit locale.
M = MULTILINE = 0x8   # Make anchors look for newline.
R = REVERSE = 0x400   # Search backwards.
S = DOTALL = 0x10     # Make dot match newline.
U = UNICODE = 0x20    # Assume Unicode locale.
W = WORD = 0x800      # Default Unicode word breaks.
X = VERBOSE = 0x40    # Ignore whitespace and comments.
Z = ZEROWIDTH = 0x100 # Correct handling of zero-width matches.
T = TEMPLATE = 0x1    # Template.

# The mask for the flags.
_GLOBAL_FLAGS = ASCII | DEBUG | LOCALE | REVERSE | UNICODE | ZEROWIDTH
_LOCAL_FLAGS = IGNORECASE | MULTILINE | DOTALL | WORD | VERBOSE

# regex exception.
class error(Exception):
   pass

_ALPHA = frozenset("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
_DIGITS = frozenset("0123456789")
_ALNUM = _ALPHA | _DIGITS
_OCT_DIGITS = frozenset("01234567")
_HEX_DIGITS = frozenset("0123456789ABCDEFabcdef")

import sys
import unicodedata
from collections import defaultdict

def _shrink_cache(cache_dict, max_length, divisor=5):
    """Make room in the given cache.

    Args:
        cache_dict: The cache dictionary to modify.
        max_length: Maximum # of entries in cache_dict before it is shrunk.
        divisor: Cache will shrink to max_length - 1/divisor*max_length items.
    """
    # Toss out a fraction of the entries at random to make room for new ones.
    # A random algorithm was chosen as opposed to simply cache_dict.popitem()
    # as popitem could penalize the same regular expression repeatedly based
    # on its internal hash value.  Being random should spread the cache miss
    # love around.
    cache_keys = tuple(cache_dict.keys())
    overage = len(cache_keys) - max_length
    if overage < 0:
        # Cache is already within limits.  Normally this should not happen
        # but it could due to multithreading.
        return
    number_to_toss = max_length // divisor + overage
    # The import is done here to avoid a circular depencency.
    import random
    if not hasattr(random, 'sample'):
        # Do nothing while resolving the circular dependency:
        #  re->random->warnings->tokenize->string->re
        return
    for doomed_key in random.sample(cache_keys, number_to_toss):
        try:
            del cache_dict[doomed_key]
        except KeyError:
            # Ignore problems if the cache changed from another thread.
            pass

# The repeat count which represents infinity.
_UNLIMITED = 0xFFFFFFFF

# The names of the opcodes.
_OPCODES = """\
FAILURE
SUCCESS
ANY
ANY_ALL
ANY_ALL_REV
ANY_REV
ATOMIC
BEGIN_GROUP
BIG_BITSET
BIG_BITSET_IGN
BIG_BITSET_IGN_REV
BIG_BITSET_REV
BOUNDARY
BRANCH
CHARACTER
CHARACTER_IGN
CHARACTER_IGN_REV
CHARACTER_REV
CHECK
CHECK_REV
DEFAULT_BOUNDARY
END
END_GREEDY_REPEAT
END_GROUP
END_LAZY_REPEAT
END_OF_LINE
END_OF_STRING
END_OF_STRING_LINE
GREEDY_REPEAT
GREEDY_REPEAT_ONE
GROUP
GROUP_EXISTS
LAZY_REPEAT
LAZY_REPEAT_ONE
LOOKAROUND
NEXT
PROPERTY
PROPERTY_REV
RANGE
RANGE_IGN
RANGE_IGN_REV
RANGE_REV
REF_GROUP
REF_GROUP_IGN
REF_GROUP_IGN_REV
REF_GROUP_REV
SEARCH_ANCHOR
SET
SET_IGN
SET_IGN_REV
SET_REV
SMALL_BITSET
SMALL_BITSET_IGN
SMALL_BITSET_IGN_REV
SMALL_BITSET_REV
START_OF_LINE
START_OF_STRING
STRING
STRING_IGN
STRING_IGN_REV
STRING_REV
"""

def _define_opcodes(opcodes):
    "Defines the opcodes and their numeric values."
    # The namespace for the opcodes.
    class Record:
        pass

    op_list = [op for op in opcodes.splitlines() if op]

    _OP = Record()

    for i, op in enumerate(op_list):
        setattr(_OP, op, i)

    return _OP

# Define the opcodes in a namespace.
_OP = _define_opcodes(_OPCODES)

# The regular expression flags.
_REGEX_FLAGS = {"a": ASCII, "i": IGNORECASE, "L": LOCALE, "m": MULTILINE,
  "r": REVERSE, "s": DOTALL, "u": UNICODE, "w": WORD, "x": VERBOSE,
  "z": ZEROWIDTH}

# Unicode properties.
_PROPERTIES = """\
0 Cn Unassigned
1 Lu Uppercase_Letter
2 Ll Lowercase_Letter
3 Lt Titlecase_Letter
4 Mn Non_Spacing_Mark
5 Mc Spacing_Combining_Mark
6 Enclosing_Mark Me
7 Decimal_Digit_Number Nd
8 Letter_Number Nl
9 No Other_Number
10 Space_Separator Zs
11 Line_Separator Zl
12 Paragraph_Separator Zp
13 Cc Control
14 Cf Format
15 Cs Surrogate
16 Co Private_Use
18 Lm Modifier_Letter
19 Lo Other_Letter
20 Connector_Punctuation Pc
21 Dash_Punctuation Pd
22 Open_Punctuation Ps
23 Close_Punctuation Pe
24 Initial_Punctuation Pi
25 Final_Punctuation Pf
26 Other_Punctuation Po
27 Math_Symbol Sm
28 Currency_Symbol Sc
29 Modifier_Symbol Sk
30 Other_Symbol So
32 L L& Letter Letter&
33 M M& Mark Mark&
34 Separator Separator& Z Z&
35 S S& Symbol Symbol&
36 N N& Number Number&
37 P P& Punctuation Punctuation&
38 C C& Other Other&
39 Alpha
40 Alnum
41 ASCII
42 Blank
43 Cntrl
44 Digit
45 Graph
46 Linebreak
47 Lower
48 Print
49 Punct
50 Space
51 Upper
52 Word
53 XDigit
54 InNo_Block No_Block
55 BasicLatin InBasicLatin
56 InLatin1Supplement Latin1Supplement
57 InLatinExtendedA LatinExtendedA
58 InLatinExtendedB LatinExtendedB
59 IPAExtensions InIPAExtensions
60 InSpacingModifierLetters SpacingModifierLetters
61 CombiningDiacriticalMarks InCombiningDiacriticalMarks
62 GreekandCoptic InGreekandCoptic
63 Cyrillic InCyrillic
64 CyrillicSupplement InCyrillicSupplement
65 Armenian InArmenian
66 Hebrew InHebrew
67 Arabic InArabic
68 InSyriac Syriac
69 ArabicSupplement InArabicSupplement
70 InThaana Thaana
71 InNKo NKo
72 InSamaritan Samaritan
73 Devanagari InDevanagari
74 Bengali InBengali
75 Gurmukhi InGurmukhi
76 Gujarati InGujarati
77 InOriya Oriya
78 InTamil Tamil
79 InTelugu Telugu
80 InKannada Kannada
81 InMalayalam Malayalam
82 InSinhala Sinhala
83 InThai Thai
84 InLao Lao
85 InTibetan Tibetan
86 InMyanmar Myanmar
87 Georgian InGeorgian
88 HangulJamo InHangulJamo
89 Ethiopic InEthiopic
90 EthiopicSupplement InEthiopicSupplement
91 Cherokee InCherokee
92 InUnifiedCanadianAboriginalSyllabics UnifiedCanadianAboriginalSyllabics
93 InOgham Ogham
94 InRunic Runic
95 InTagalog Tagalog
96 Hanunoo InHanunoo
97 Buhid InBuhid
98 InTagbanwa Tagbanwa
99 InKhmer Khmer
100 InMongolian Mongolian
101 InUnifiedCanadianAboriginalSyllabicsExtended UnifiedCanadianAboriginalSyllabicsExtended
102 InLimbu Limbu
103 InTaiLe TaiLe
104 InNewTaiLue NewTaiLue
105 InKhmerSymbols KhmerSymbols
106 Buginese InBuginese
107 InTaiTham TaiTham
108 Balinese InBalinese
109 InSundanese Sundanese
110 InLepcha Lepcha
111 InOlChiki OlChiki
112 InVedicExtensions VedicExtensions
113 InPhoneticExtensions PhoneticExtensions
114 InPhoneticExtensionsSupplement PhoneticExtensionsSupplement
115 CombiningDiacriticalMarksSupplement InCombiningDiacriticalMarksSupplement
116 InLatinExtendedAdditional LatinExtendedAdditional
117 GreekExtended InGreekExtended
118 GeneralPunctuation InGeneralPunctuation
119 InSuperscriptsandSubscripts SuperscriptsandSubscripts
120 CurrencySymbols InCurrencySymbols
121 CombiningDiacriticalMarksforSymbols InCombiningDiacriticalMarksforSymbols
122 InLetterlikeSymbols LetterlikeSymbols
123 InNumberForms NumberForms
124 Arrows InArrows
125 InMathematicalOperators MathematicalOperators
126 InMiscellaneousTechnical MiscellaneousTechnical
127 ControlPictures InControlPictures
128 InOpticalCharacterRecognition OpticalCharacterRecognition
129 EnclosedAlphanumerics InEnclosedAlphanumerics
130 BoxDrawing InBoxDrawing
131 BlockElements InBlockElements
132 GeometricShapes InGeometricShapes
133 InMiscellaneousSymbols MiscellaneousSymbols
134 Dingbats InDingbats
135 InMiscellaneousMathematicalSymbolsA MiscellaneousMathematicalSymbolsA
136 InSupplementalArrowsA SupplementalArrowsA
137 BraillePatterns InBraillePatterns
138 InSupplementalArrowsB SupplementalArrowsB
139 InMiscellaneousMathematicalSymbolsB MiscellaneousMathematicalSymbolsB
140 InSupplementalMathematicalOperators SupplementalMathematicalOperators
141 InMiscellaneousSymbolsandArrows MiscellaneousSymbolsandArrows
142 Glagolitic InGlagolitic
143 InLatinExtendedC LatinExtendedC
144 Coptic InCoptic
145 GeorgianSupplement InGeorgianSupplement
146 InTifinagh Tifinagh
147 EthiopicExtended InEthiopicExtended
148 CyrillicExtendedA InCyrillicExtendedA
149 InSupplementalPunctuation SupplementalPunctuation
150 CJKRadicalsSupplement InCJKRadicalsSupplement
151 InKangxiRadicals KangxiRadicals
152 IdeographicDescriptionCharacters InIdeographicDescriptionCharacters
153 CJKSymbolsandPunctuation InCJKSymbolsandPunctuation
154 Hiragana InHiragana
155 InKatakana Katakana
156 Bopomofo InBopomofo
157 HangulCompatibilityJamo InHangulCompatibilityJamo
158 InKanbun Kanbun
159 BopomofoExtended InBopomofoExtended
160 CJKStrokes InCJKStrokes
161 InKatakanaPhoneticExtensions KatakanaPhoneticExtensions
162 EnclosedCJKLettersandMonths InEnclosedCJKLettersandMonths
163 CJKCompatibility InCJKCompatibility
164 CJKUnifiedIdeographsExtensionA InCJKUnifiedIdeographsExtensionA
165 InYijingHexagramSymbols YijingHexagramSymbols
166 CJKUnifiedIdeographs InCJKUnifiedIdeographs
167 InYiSyllables YiSyllables
168 InYiRadicals YiRadicals
169 InLisu Lisu
170 InVai Vai
171 CyrillicExtendedB InCyrillicExtendedB
172 Bamum InBamum
173 InModifierToneLetters ModifierToneLetters
174 InLatinExtendedD LatinExtendedD
175 InSylotiNagri SylotiNagri
176 CommonIndicNumberForms InCommonIndicNumberForms
177 InPhagspa Phagspa
178 InSaurashtra Saurashtra
179 DevanagariExtended InDevanagariExtended
180 InKayahLi KayahLi
181 InRejang Rejang
182 HangulJamoExtendedA InHangulJamoExtendedA
183 InJavanese Javanese
184 Cham InCham
185 InMyanmarExtendedA MyanmarExtendedA
186 InTaiViet TaiViet
187 InMeeteiMayek MeeteiMayek
188 HangulSyllables InHangulSyllables
189 HangulJamoExtendedB InHangulJamoExtendedB
190 HighSurrogates InHighSurrogates
191 HighPrivateUseSurrogates InHighPrivateUseSurrogates
192 InLowSurrogates LowSurrogates
193 InPrivateUseArea PrivateUseArea
194 CJKCompatibilityIdeographs InCJKCompatibilityIdeographs
195 AlphabeticPresentationForms InAlphabeticPresentationForms
196 ArabicPresentationFormsA InArabicPresentationFormsA
197 InVariationSelectors VariationSelectors
198 InVerticalForms VerticalForms
199 CombiningHalfMarks InCombiningHalfMarks
200 CJKCompatibilityForms InCJKCompatibilityForms
201 InSmallFormVariants SmallFormVariants
202 ArabicPresentationFormsB InArabicPresentationFormsB
203 HalfwidthandFullwidthForms InHalfwidthandFullwidthForms
204 InSpecials Specials
205 InLinearBSyllabary LinearBSyllabary
206 InLinearBIdeograms LinearBIdeograms
207 AegeanNumbers InAegeanNumbers
208 AncientGreekNumbers InAncientGreekNumbers
209 AncientSymbols InAncientSymbols
210 InPhaistosDisc PhaistosDisc
211 InLycian Lycian
212 Carian InCarian
213 InOldItalic OldItalic
214 Gothic InGothic
215 InUgaritic Ugaritic
216 InOldPersian OldPersian
217 Deseret InDeseret
218 InShavian Shavian
219 InOsmanya Osmanya
220 CypriotSyllabary InCypriotSyllabary
221 ImperialAramaic InImperialAramaic
222 InPhoenician Phoenician
223 InLydian Lydian
224 InKharoshthi Kharoshthi
225 InOldSouthArabian OldSouthArabian
226 Avestan InAvestan
227 InInscriptionalParthian InscriptionalParthian
228 InInscriptionalPahlavi InscriptionalPahlavi
229 InOldTurkic OldTurkic
230 InRumiNumeralSymbols RumiNumeralSymbols
231 InKaithi Kaithi
232 Cuneiform InCuneiform
233 CuneiformNumbersandPunctuation InCuneiformNumbersandPunctuation
234 EgyptianHieroglyphs InEgyptianHieroglyphs
235 ByzantineMusicalSymbols InByzantineMusicalSymbols
236 InMusicalSymbols MusicalSymbols
237 AncientGreekMusicalNotation InAncientGreekMusicalNotation
238 InTaiXuanJingSymbols TaiXuanJingSymbols
239 CountingRodNumerals InCountingRodNumerals
240 InMathematicalAlphanumericSymbols MathematicalAlphanumericSymbols
241 InMahjongTiles MahjongTiles
242 DominoTiles InDominoTiles
243 EnclosedAlphanumericSupplement InEnclosedAlphanumericSupplement
244 EnclosedIdeographicSupplement InEnclosedIdeographicSupplement
245 CJKUnifiedIdeographsExtensionB InCJKUnifiedIdeographsExtensionB
246 CJKUnifiedIdeographsExtensionC InCJKUnifiedIdeographsExtensionC
247 CJKCompatibilityIdeographsSupplement InCJKCompatibilityIdeographsSupplement
248 InTags Tags
249 InVariationSelectorsSupplement VariationSelectorsSupplement
250 InSupplementaryPrivateUseAreaA SupplementaryPrivateUseAreaA
251 InSupplementaryPrivateUseAreaB SupplementaryPrivateUseAreaB
252 IsUnknown Unknown
253 Common IsCommon
254 IsLatin Latin
255 Greek IsGreek
256 Cyrillic IsCyrillic
257 Armenian IsArmenian
258 Hebrew IsHebrew
259 Arabic IsArabic
260 IsSyriac Syriac
261 IsThaana Thaana
262 Devanagari IsDevanagari
263 Bengali IsBengali
264 Gurmukhi IsGurmukhi
265 Gujarati IsGujarati
266 IsOriya Oriya
267 IsTamil Tamil
268 IsTelugu Telugu
269 IsKannada Kannada
270 IsMalayalam Malayalam
271 IsSinhala Sinhala
272 IsThai Thai
273 IsLao Lao
274 IsTibetan Tibetan
275 IsMyanmar Myanmar
276 Georgian IsGeorgian
277 Hangul IsHangul
278 Ethiopic IsEthiopic
279 Cherokee IsCherokee
280 CanadianAboriginal IsCanadianAboriginal
281 IsOgham Ogham
282 IsRunic Runic
283 IsKhmer Khmer
284 IsMongolian Mongolian
285 Hiragana IsHiragana
286 IsKatakana Katakana
287 Bopomofo IsBopomofo
288 Han IsHan
289 IsYi Yi
290 IsOldItalic OldItalic
291 Gothic IsGothic
292 Deseret IsDeseret
293 Inherited IsInherited
294 IsTagalog Tagalog
295 Hanunoo IsHanunoo
296 Buhid IsBuhid
297 IsTagbanwa Tagbanwa
298 IsLimbu Limbu
299 IsTaiLe TaiLe
300 IsLinearB LinearB
301 IsUgaritic Ugaritic
302 IsShavian Shavian
303 IsOsmanya Osmanya
304 Cypriot IsCypriot
305 Braille IsBraille
306 Buginese IsBuginese
307 Coptic IsCoptic
308 IsNewTaiLue NewTaiLue
309 Glagolitic IsGlagolitic
310 IsTifinagh Tifinagh
311 IsSylotiNagri SylotiNagri
312 IsOldPersian OldPersian
313 IsKharoshthi Kharoshthi
314 Balinese IsBalinese
315 Cuneiform IsCuneiform
316 IsPhoenician Phoenician
317 IsPhagsPa PhagsPa
318 IsNko Nko
319 IsSundanese Sundanese
320 IsLepcha Lepcha
321 IsOlChiki OlChiki
322 IsVai Vai
323 IsSaurashtra Saurashtra
324 IsKayahLi KayahLi
325 IsRejang Rejang
326 IsLycian Lycian
327 Carian IsCarian
328 IsLydian Lydian
329 Cham IsCham
330 IsTaiTham TaiTham
331 IsTaiViet TaiViet
332 Avestan IsAvestan
333 EgyptianHieroglyphs IsEgyptianHieroglyphs
334 IsSamaritan Samaritan
335 IsLisu Lisu
336 Bamum IsBamum
337 IsJavanese Javanese
338 IsMeeteiMayek MeeteiMayek
339 ImperialAramaic IsImperialAramaic
340 IsOldSouthArabian OldSouthArabian
341 InscriptionalParthian IsInscriptionalParthian
342 InscriptionalPahlavi IsInscriptionalPahlavi
343 IsOldTurkic OldTurkic
344 IsKaithi Kaithi
"""

def _create_properties(definitions):
    "Creates the Unicode properties."
    # Normalise the names.
    definitions = definitions.upper().replace("_", "").replace("-", "")

    properties = {}
    for line in definitions.splitlines():
        if line:
            fields = line.split()
            id, names = int(fields[0]), fields[1 : ]
            for name in names:
                properties[name] = id

    return properties

# Build the property dictionary.
_properties = _create_properties(_PROPERTIES)

def _compile_firstset(info, fs):
    if not fs or _VOID_ITEM in fs or None in fs:
        return []
    members, members_ign = [], []
    for i in fs:
        t = type(i)
        if t is _Any:
            members.append(i)
        elif t in (_Character, _Property) and i.positive:
            members.append(i)
        elif t is _CharacterIgn and i.positive:
            members_ign.append(i)
        elif t is _Set and i.positive:
            members.extend(i.members)
        elif t is _SetIgn and i.positive:
            members_ign.extend(i.members)
        else:
            return []
    if members_ign:
        fs = _SetIgn(members + members_ign, zerowidth=True)
    elif members:
        fs = _Set(members, zerowidth=True)
    else:
        return []
    fs = fs.optimise(info)
    if not fs:
        return []
    rev = bool(info.global_flags & REVERSE)
    return fs.compile(rev)

def _count_ones(n):
    "Counts the number of set bits in an int."
    count = 0
    while n:
        count += n & 0x1
        n >>= 1
    return count

def _flatten_code(code):
    "Flattens the code from a list of tuples."
    flat_code = []
    for c in code:
        flat_code.extend(c)
    return flat_code

def _parse_pattern(source, info):
    "Parses a pattern, eg. 'a|b|c'."
    # Capture group names can be duplicated provided that their matching is
    # mutually exclusive.
    previous_groups = info.used_groups.copy()
    branches = [_parse_sequence(source, info)]
    all_groups = info.used_groups
    while source.match("|"):
        info.used_groups = previous_groups.copy()
        branches.append(_parse_sequence(source, info))
        all_groups |= info.used_groups
    info.used_groups = all_groups
    return _Branch(branches)

def _parse_sequence(source, info):
    "Parses a sequence, eg. 'abc'."
    sequence = []
    item = _parse_item(source, info)
    while item:
        sequence.append(item)
        item = _parse_item(source, info)
    return _Sequence(sequence)

def _parse_item(source, info):
    "Parses an item, which might be repeated. Returns None if there's no item."
    element = _parse_element(source, info)
    if not element:
        return element
    here = source.tell()
    lazy = possessive = False
    try:
        min_count, max_count = _parse_quantifier(source, info)
        if not element.can_repeat():
            raise error("nothing to repeat")
        if source.match("?"):
            lazy = True
        elif source.match("+"):
            possessive = True
        if min_count == max_count == 1:
            return element
    except error:
        # Not a quantifier, so we'll parse it later as a literal.
        source.seek(here)
        return element
    if lazy:
        return _LazyRepeat(element, min_count, max_count)
    elif possessive:
        return _Atomic(_GreedyRepeat(element, min_count, max_count))
    else:
        return _GreedyRepeat(element, min_count, max_count)

def _parse_quantifier(source, info):
    "Parses a quantifier."
    if source.match("?"):
        # Optional element, eg. 'a?'.
        return 0, 1
    elif source.match("*"):
        # Repeated element, eg. 'a*'.
        return 0, None
    elif source.match("+"):
        # Repeated element, eg. 'a+'.
        return 1, None
    elif source.match("{"):
        # Limited repeated element, eg. 'a{2,3}'.
        min_count = _parse_count(source)
        if source.match(","):
            max_count = _parse_count(source)
            if source.match("}"):
                # An empty minimum means 0 and an empty maximum means unlimited.
                min_count = int(min_count) if min_count else 0
                max_count = int(max_count) if max_count else None
                if max_count is not None and min_count > max_count:
                    raise error("min repeat greater than max repeat")
                if min_count >= _UNLIMITED or max_count is not None and max_count >= _UNLIMITED:
                    raise error("repeat count too big")
                return min_count, max_count
            else:
                raise error("missing }")
        elif source.match("}"):
            if min_count:
                min_count = max_count = int(min_count)
                if min_count >= _UNLIMITED:
                    raise error("repeat count too big")
                return min_count, max_count
            else:
                raise error("invalid quantifier")
        else:
            raise error("invalid quantifier")
    else:
        # No quantifier.
        return 1, 1

def _parse_count(source):
    "Parses a quantifier's count, which can be empty."
    count = []
    here = source.tell()
    ch = source.get()
    while ch in _DIGITS:
        count.append(ch)
        here = source.tell()
        ch = source.get()
    source.seek(here)
    return source.sep.join(count)

def _parse_element(source, info):
    "Parses an element. An element might actually be a flag, eg. '(?i)'."
    while True:
        here = source.tell()
        ch = source.get()
        if ch in ")|":
            # The end of a sequence. At the end of the pattern ch is "".
            source.seek(here)
            return None
        elif ch in "?*+{":
            # Looks like a quantifier.
            source.seek(here)
            try:
                _parse_quantifier(source, info)
            except error:
                # Not a quantifier, so it's a literal.
                # None of these characters are case-dependent.
                source.seek(here)
                ch = source.get()
                return _Character(ch)
            # A quantifier where we expected an element.
            raise error("nothing to repeat")
        elif ch == "(":
            # A parenthesised subpattern or a flag.
            element = _parse_paren(source, info)
            if element:
                return element
        elif ch == "^":
            # The start of a line or the string.
            if info.local_flags & MULTILINE:
                return _StartOfLine()
            else:
                return _StartOfString()
        elif ch == "$":
            # The end of a line or the string.
            if info.local_flags & MULTILINE:
                return _EndOfLine()
            else:
                return _EndOfStringLine()
        elif ch == ".":
            # Any character.
            if info.local_flags & DOTALL:
                return _AnyAll()
            else:
                return _Any()
        elif ch == "[":
            # A character set.
            return _parse_set(source, info)
        elif ch == "\\":
            # An escape sequence.
            return _parse_escape(source, info, False)
        elif ch == "#" and (info.local_flags & VERBOSE):
            # A comment.
            source.ignore_space = False
            # Ignore characters until a newline or the end of the pattern.
            while source.get() not in "\n":
                pass
            source.ignore_space = True
        elif ch.isspace() and (info.local_flags & VERBOSE):
            pass
        else:
            # A literal.
            if info.local_flags & IGNORECASE:
                return _CharacterIgn(ch)
            return _Character(ch)

def _parse_paren(source, info):
    "Parses a parenthesised subpattern or a flag."
    if source.match("?P"):
        # A Python extension.
        return _parse_extension(source, info)
    elif source.match("?#"):
        # A comment.
        return _parse_comment(source)
    elif source.match("?="):
        # Positive lookahead.
        return _parse_lookaround(source, info, False, True)
    elif source.match("?!"):
        # Negative lookahead.
        return _parse_lookaround(source, info, False, False)
    elif source.match("?<="):
        # Positive lookbehind.
        return _parse_lookaround(source, info, True, True)
    elif source.match("?<!"):
        # Negative lookbehind.
        return _parse_lookaround(source, info, True, False)
    elif source.match("?<"):
        # A named capture group.
        name = _parse_name(source)
        group = info.new_group(name)
        source.expect(">")
        saved_local_flags = info.local_flags
        saved_ignore = source.ignore_space
        try:
            subpattern = _parse_pattern(source, info)
        finally:
            info.local_flags = saved_local_flags
            source.ignore_space = saved_ignore
        source.expect(")")
        info.close_group(group)
        return _Group(info, group, subpattern)
    elif source.match("?("):
        # A conditonal subpattern.
        return _parse_conditional(source, info)
    elif source.match("?>"):
        # An atomic subpattern.
        return _parse_atomic(source, info)
    elif source.match("?|"):
        # A common groups branch.
        return _parse_common(source, info)
    elif source.match("?"):
        # A flags subpattern.
        return _parse_flags_subpattern(source, info)
    else:
        # An unnamed capture group.
        group = info.new_group()
        saved_local_flags = info.local_flags
        saved_ignore = source.ignore_space
        try:
            subpattern = _parse_pattern(source, info)
        finally:
            info.local_flags = saved_local_flags
            source.ignore_space = saved_ignore
        source.expect(")")
        info.close_group(group)
        return _Group(info, group, subpattern)

def _parse_extension(source, info):
    "Parses a Python extension."
    if source.match("<"):
        # A named capture group.
        name = _parse_name(source)
        group = info.new_group(name)
        source.expect(">")
        saved_local_flags = info.local_flags
        saved_ignore = source.ignore_space
        try:
            subpattern = _parse_pattern(source, info)
        finally:
            info.local_flags = saved_local_flags
            source.ignore_space = saved_ignore
        source.expect(")")
        info.close_group(group)
        return _Group(info, group, subpattern)
    elif source.match("="):
        # A named group reference.
        name = _parse_name(source)
        source.expect(")")
        if info.is_open_group(name):
            raise error("can't refer to an open group")
        if info.local_flags & IGNORECASE:
            return _RefGroupIgn(info, name)
        return _RefGroup(info, name)
    else:
        raise error("unknown extension")

def _parse_comment(source):
    "Parses a comment."
    ch = source.get()
    while ch not in ")":
        ch = source.get()
    if not ch:
        raise error("missing )")
    return None

def _parse_lookaround(source, info, behind, positive):
    "Parses a lookaround."
    saved_local_flags = info.local_flags
    saved_ignore = source.ignore_space
    try:
        subpattern = _parse_pattern(source, info)
    finally:
        info.local_flags = saved_local_flags
        source.ignore_space = saved_ignore
    source.expect(")")
    return _LookAround(behind, positive, subpattern)

def _parse_conditional(source, info):
    "Parses a conditional subpattern."
    saved_local_flags = info.local_flags
    saved_ignore = source.ignore_space
    try:
        group = _parse_name(source, True)
        source.expect(")")
        previous_groups = info.used_groups.copy()
        yes_branch = _parse_sequence(source, info)
        if source.match("|"):
            yes_groups = info.used_groups
            info.used_groups = previous_groups
            no_branch = _parse_sequence(source, info)
            info.used_groups |= yes_groups
        else:
            no_branch = None
    finally:
        info.local_flags = saved_local_flags
        source.ignore_space = saved_ignore
    source.expect(")")
    return _Conditional(info, group, yes_branch, no_branch)

def _parse_atomic(source, info):
    "Parses an atomic subpattern."
    saved_local_flags = info.local_flags
    saved_ignore = source.ignore_space
    try:
        subpattern = _parse_pattern(source, info)
    finally:
        info.local_flags = saved_local_flags
        source.ignore_space = saved_ignore
    source.expect(")")
    return _Atomic(subpattern)

def _parse_common(source, info):
    "Parses a common groups branch."
    # Capture group numbers in different branches can reuse the group nunmbers.
    previous_groups = info.used_groups.copy()
    initial_group_count = info.group_count
    branches = [_parse_sequence(source, info)]
    final_group_count = info.group_count
    all_groups = info.used_groups
    while source.match("|"):
        info.used_groups = previous_groups.copy()
        info.group_count = initial_group_count
        branches.append(_parse_sequence(source, info))
        final_group_count = max(final_group_count, info.group_count)
        all_groups |= info.used_groups
    info.used_groups = all_groups
    info.group_count = final_group_count
    source.expect(")")
    return _Branch(branches)

def _parse_flags_subpattern(source, info):
    "Parses a flags subpattern."
    # It could be inline flags or a subpattern possibly with local flags.
    # Parse the flags.
    flags_on, flags_off = 0, 0
    try:
        while True:
            here = source.tell()
            ch = source.get()
            flags_on |= _REGEX_FLAGS[ch]
    except KeyError:
        pass
    if ch == "-":
        try:
            while True:
                here = source.tell()
                ch = source.get()
                flags_off |= _REGEX_FLAGS[ch]
        except KeyError:
            pass
        if not flags_off or (flags_off & _GLOBAL_FLAGS):
            error("bad inline flags")
    # Separate the global and local flags.
    source.seek(here)
    info.global_flags |= flags_on & _GLOBAL_FLAGS
    flags_on &= _LOCAL_FLAGS
    new_local_flags = (info.local_flags | flags_on) & ~flags_off
    saved_local_flags = info.local_flags
    saved_ignore = source.ignore_space
    info.local_flags = new_local_flags
    source.ignore_space = bool(info.local_flags & VERBOSE)
    if source.match(":"):
        # A subpattern with local flags.
        try:
            subpattern = _parse_pattern(source, info)
        finally:
            info.local_flags = saved_local_flags
            source.ignore_space = saved_ignore
        source.expect(")")
        return subpattern
    else:
        # Inline flags.
        if not source.match(")"):
            raise error("bad inline flags")
        return None

def _parse_name(source, allow_numeric=False):
    "Parses a name."
    saved_ignore = source.ignore_space
    source.ignore_space = False
    name = []
    here = source.tell()
    ch = source.get()
    while ch in _ALNUM or ch == "_":
        name.append(ch)
        here = source.tell()
        ch = source.get()
    source.seek(here)
    source.ignore_space = saved_ignore
    name = source.sep.join(name)
    if not name:
        raise error("bad group name")
    if name.isdigit():
        if not allow_numeric:
            raise error("bad group name")
    else:
        if name[0].isdigit():
            raise error("bad group name")
    return name

def _is_octal(string):
    "Checks whether a string is octal."
    return all(ch in _OCT_DIGITS for ch in string)

def _is_decimal(string):
    "Checks whether a string is decimal."
    return all(ch in _DIGITS for ch in string)

def _is_hexadecimal(string):
    "Checks whether a string is hexadecimal."
    return all(ch in _HEX_DIGITS for ch in string)

def _parse_escape(source, info, in_set):
    "Parses an escape sequence."
    ch = source.get()
    if not ch:
        # A backslash at the end of the pattern.
        raise error("bad escape")
    if ch == "x":
        # A 2-digit hexadecimal escape sequence.
        return _parse_hex_escape(source, info, 2, in_set)
    elif ch == "u":
        # A 4-digit hexadecimal escape sequence.
        return _parse_hex_escape(source, info, 4, in_set)
    elif ch == "U":
        # A 8-digit hexadecimal escape sequence.
        return _parse_hex_escape(source, info, 8, in_set)
    elif ch == "g" and not in_set:
        # A group reference.
        here = source.tell()
        try:
            return _parse_group_ref(source, info)
        except error:
            # Invalid as a group reference, so assume it's a literal.
            source.seek(here)
            return _char_literal(info, in_set, ch)
    elif ch == "G" and not in_set:
        # A search anchor.
        return _SearchAnchor()
    elif ch == "N":
        # A named codepoint.
        return _parse_named_char(source, info, in_set)
    elif ch in "pP":
        # A Unicode property.
        return _parse_property(source, info, in_set, ch)
    elif ch == "X" and not in_set:
        return _grapheme()
    elif ch in _ALPHA:
        # An alphabetic escape sequence.
        # Positional escapes aren't allowed inside a character set.
        if not in_set:
            if info.local_flags & WORD:
                value = _WORD_POSITION_ESCAPES.get(ch)
            else:
                value = _POSITION_ESCAPES.get(ch)
            if value:
                return value
        value = _CHARSET_ESCAPES.get(ch)
        if value:
            return value
        value = _CHARACTER_ESCAPES.get(ch)
        if value:
            return _Character(value)
        return _char_literal(info, in_set, ch)
    elif ch in _DIGITS:
        # A numeric escape sequence.
        return _parse_numeric_escape(source, info, ch, in_set)
    else:
        # A literal.
        return _char_literal(info, in_set, ch)

def _char_literal(info, in_set, ch):
    "Creates a character literal, which might be in a set."
    if (info.local_flags & IGNORECASE) and not in_set:
        return _CharacterIgn(ch)
    return _Character(ch)

def _parse_numeric_escape(source, info, ch, in_set):
    "Parses a numeric escape sequence."
    if in_set or ch == "0":
        # Octal escape sequence, max 3 digits.
        return _parse_octal_escape(source, info, [ch], in_set)
    else:
        # At least 1 digit, so either octal escape or group.
        digits = ch
        here = source.tell()
        ch = source.get()
        if ch in _DIGITS:
            # At least 2 digits, so either octal escape or group.
            digits += ch
            here = source.tell()
            ch = source.get()
            if _is_octal(digits) and ch in _OCT_DIGITS:
                # 3 octal digits, so octal escape sequence.
                value = int(digits + ch, 8) & 0xFF
                if info.local_flags & IGNORECASE:
                    return _CharacterIgn(value)
                return _Character(value)
            else:
                # 2 digits, so group.
                source.seek(here)
                if info.is_open_group(digits):
                    raise error("can't refer to an open group")
                return _RefGroup(info, digits)
        else:
            # 1 digit, so group.
            source.seek(here)
            if info.is_open_group(digits):
                raise error("can't refer to an open group")
            return _RefGroup(info, digits)

def _parse_octal_escape(source, info, digits, in_set):
    "Parses an octal escape sequence."
    here = source.tell()
    ch = source.get()
    while len(digits) < 3 and ch in _OCT_DIGITS:
        digits.append(ch)
        here = source.tell()
        ch = source.get()
    source.seek(here)
    try:
        value = int(source.sep.join(digits), 8) & 0xFF
        if (info.local_flags & IGNORECASE) and not in_set:
            return _CharacterIgn(value)
        return _Character(value)
    except ValueError:
        raise error("bad escape")

def _parse_hex_escape(source, info, max_len, in_set):
    "Parses a hex escape sequence."
    digits = []
    here = source.tell()
    ch = source.get()
    while len(digits) < max_len and ch in _HEX_DIGITS:
        digits.append(ch)
        here = source.tell()
        ch = source.get()
    if len(digits) != max_len:
        raise error("bad hex escape")
    source.seek(here)
    value = int(source.sep.join(digits), 16)
    if (info.local_flags & IGNORECASE) and not in_set:
        return _CharacterIgn(value)
    return _Character(value)

def _parse_group_ref(source, info):
    "Parses a group reference."
    source.expect("<")
    name = _parse_name(source, True)
    source.expect(">")
    if info.is_open_group(name):
        raise error("can't refer to an open group")
    if info.local_flags & IGNORECASE:
        return _RefGroupIgn(info, name)
    return _RefGroup(info, name)

def _parse_named_char(source, info, in_set):
    "Parses a named character."
    here = source.tell()
    ch = source.get()
    if ch == "{":
        name = []
        ch = source.get()
        while ch in _ALPHA or ch == " ":
            name.append(ch)
            ch = source.get()
        if ch == "}":
            try:
                value = unicodedata.lookup(source.sep.join(name))
                if (info.local_flags & IGNORECASE) and not in_set:
                    return _CharacterIgn(value)
                return _Character(value)
            except KeyError:
                raise error("undefined character name")
    source.seek(here)
    return _char_literal(info, in_set, "N")

def _parse_property(source, info, in_set, prop_ch):
    "Parses a Unicode property."
    here = source.tell()
    ch = source.get()
    if ch == "{":
        name = []
        ch = source.get()
        while ch and (ch.isalnum() or ch.isspace() or ch in "&_-."):
            name.append(ch)
            ch = source.get()
        if ch == "}":
            # The normalised name.
            norm_name = source.sep.join(ch.upper() for ch in name if ch.isalnum())
            # The un-normalised name.
            name = source.sep.join(name)
            value = _properties.get(norm_name)
            if value is not None:
                return _Property(value, positive=prop_ch == "p")
            raise error("undefined property name")
    source.seek(here)
    return _char_literal(info, in_set, prop_ch)

def _grapheme():
    "Returns a sequence that matches a grapheme."
    # To match a grapheme use \P{M}\p{M}*
    mod = _properties.get("M")
    return _Sequence([_Property(mod, positive=False), _GreedyRepeat(_Property(mod), 0, None)])

def _parse_set(source, info):
    "Parses a character set."
    # Negative character set?
    saved_ignore = source.ignore_space
    source.ignore_space = False
    negate = source.match("^")
    members = []
    try:
        item = _parse_set_member(source, info)
        members.append(item)
        while not source.match("]"):
            item = _parse_set_member(source, info)
            members.append(item)
    finally:
        source.ignore_space = saved_ignore
    if info.local_flags & IGNORECASE:
        return _SetIgn(members, positive=not negate)
    return _Set(members, positive=not negate)

def _parse_set_member(source, info):
    "Parses a member in a character set."
    # It might actually be a single value, a range, or a predefined set.
    start = _parse_set_item(source, info)
    here = source.tell()
    if isinstance(start, _Character) and source.match("-"):
        if source.match("]"):
            source.seek(here)
            return start
        end = _parse_set_item(source, info)
        if isinstance(end, _Character):
            if start.value > end.value:
                raise error("bad character range")
            return _Range(start.value, end.value)
        source.seek(here)
    return start

def _parse_set_item(source, info):
    "Parses an item in a character set."
    if source.match("\\"):
        return _parse_escape(source, info, True)
    elif source.match("[:^"):
        return _parse_character_class(source, False, info)
    elif source.match("[:"):
        return _parse_character_class(source, True, info)
    else:
        ch = source.get()
        if not ch:
            raise error("bad set")
        return _Character(ch)

def _parse_character_class(source, positive, info):
    name = _parse_name(source)
    source.expect(":]")
    value = _properties.get(name.upper())
    if value is not None:
        return _Property(value, positive=positive)
    raise error("undefined character class name")

def _compile_repl_escape(source, pattern):
    "Compiles a replacement template escape sequence."
    here = source.tell()
    ch = source.get()
    if ch in _ALPHA:
        # An alphabetic escape sequence.
        value = _CHARACTER_ESCAPES.get(ch)
        if value:
            return False, value
        if ch == "g":
            # A group preference.
            return True, _compile_repl_group(source, pattern)
        else:
            source.seek(here)
            return False, ord("\\")
    elif ch == "0":
        # An octal escape sequence.
        digits = ch
        while len(digits) < 3:
            here = source.tell()
            ch = source.get()
            if ch not in _OCT_DIGITS:
                source.seek(here)
                break
            digits += ch
        return False, int(digits, 8) & 0xFF
    elif ch in _DIGITS:
        # Either an octal escape sequence (3 digits) or a group reference (max
        # 2 digits).
        digits = ch
        here = source.tell()
        ch = source.get()
        if ch in _DIGITS:
            digits += ch
            here = source.tell()
            ch = source.get()
            if ch and _is_octal(digits + ch):
                # An octal escape sequence.
                return False, int(digits + ch, 8) & 0xFF
            else:
                # A group reference.
                source.seek(here)
                return True, int(digits)
        else:
            source.seek(here)
            # A group reference.
            return True, int(digits)
    else:
        # A literal.
        return False, ord(ch)

def _compile_repl_group(source, pattern):
    "Compiles a replacement template group reference."
    source.expect("<")
    name = _parse_name(source, True)
    source.expect(">")
    if name.isdigit():
        index = int(name)
        if not 0 <= index <= pattern.groups:
            raise error("invalid group")
        return index
    else:
        try:
            return pattern.groupindex[name]
        except KeyError:
            raise IndexError("unknown group")

# The regular expression is parsed into a syntax tree. The different types of
# node are defined below.

def prefix_checks(checks, parsed):
    if isinstance(checks, _Sequence) and not checks.sequence:
        return parsed
    return _Check(checks, parsed)

_INDENT = "  "
_ZEROWIDTH_OP = 0x2

_VOID_ITEM = object()

# Common base for all nodes.
class _RegexBase:
    def __init__(self):
        self._key = self.__class__
    def fix_groups(self):
        pass
    def optimise(self, info):
        return self
    def pack_characters(self):
        return self
    def remove_captures(self):
        return self
    def is_empty(self):
        return False
    def is_atomic(self):
        return True
    def contains_group(self):
        return False
    def get_first(self):
        return self
    def drop_first(self):
        return _Sequence()
    def get_last(self):
        return self
    def drop_last(self):
        return _Sequence()
    def get_range(self):
        return None
    def can_repeat(self):
        return True
    def firstset(self):
        return set([_VOID_ITEM])
    def has_simple_start(self):
        return False
    def __hash__(self):
        return hash(self._key)
    def __eq__(self, other):
        return type(self) is type(other) and self._key == other._key
    def get_checks(self, reverse):
        return _Sequence()

# Base for zero-width nodes.
class _ZeroWidthBase(_RegexBase):
    def firstset(self):
        return set([None])
    def can_repeat(self):
        return False

# Base for 'structure' nodes, ie those containing subpatterns.
class _StructureBase(_RegexBase):
    def get_first(self):
        return None
    def drop_first(self):
        raise error("internal error")
    def get_last(self):
        return None
    def drop_last(self):
        raise error("internal error")

class _Any(_RegexBase):
    _opcode = {False: _OP.ANY, True: _OP.ANY_REV}
    _op_name = {False: "ANY", True: "ANY_REV"}
    def compile(self, reverse=False):
        return [(self._opcode[reverse], )]
    def dump(self, indent=0, reverse=False):
        print("{}{}".format(_INDENT * indent, self._op_name[reverse]))
    def has_simple_start(self):
        return True

class _AnyAll(_Any):
    _opcode = {False: _OP.ANY_ALL, True: _OP.ANY_ALL_REV}
    _op_name = {False: "ANY_ALL", True: "ANY_ALL_REV"}

class _Atomic(_StructureBase):
    def __init__(self, subpattern):
        self.subpattern = subpattern
    def fix_groups(self):
        self.subpattern.fix_groups()
    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)
        # Leading items which are atomic can be moved out of the atomic expression.
        sequence = []
        while True:
            item = subpattern.get_first()
            if not item or not item.is_atomic():
                break
            sequence.append(item)
            subpattern = subpattern.drop_first()
        # Is there anything left in the atomic expression?
        if not subpattern.is_empty():
            sequence.append(_Atomic(subpattern))
        if len(sequence) == 1:
            return sequence[0]
        return _Sequence(sequence)
    def pack_characters(self):
        return _Atomic(self.subpattern.pack_characters())
    def is_empty(self):
        return self.subpattern.is_empty()
    def contains_group(self):
        return self.subpattern.contains_group()
    def compile(self, reverse=False):
        return [(_OP.ATOMIC, )] + self.subpattern.compile(reverse) + [(_OP.END, )]
    def dump(self, indent=0, reverse=False):
        print("{}{}".format(_INDENT * indent, "ATOMIC"))
        self.subpattern.dump(indent + 1, reverse)
    def firstset(self):
        return self.subpattern.firstset()
    def __eq__(self, other):
        return type(self) is type(other) and self.subpattern == other.subpattern
    def get_checks(self, reverse):
        return self.subpattern.get_checks(reverse)

class _Boundary(_ZeroWidthBase):
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, positive=True):
        self.positive = bool(positive)
        self._key = self.__class__, self.positive
    def compile(self, reverse=False):
        return [(_OP.BOUNDARY, int(self.positive))]
    def dump(self, indent=0, reverse=False):
        print("{}BOUNDARY {}".format(_INDENT * indent, self._pos_text[self.positive]))

class _Branch(_StructureBase):
    def __init__(self, branches):
        self.branches = branches
    def fix_groups(self):
        for branch in self.branches:
            branch.fix_groups()
    def optimise(self, info):
        # Flatten the branches so that there isn't a branch containing just a
        # branch.
        branches = []
        for branch in self.branches:
            branch = branch.optimise(info)
            if isinstance(branch, _Branch):
                branches.extend(branch.branches)
            else:
                branches.append(branch)

        # Common leading items can be moved out of the branches.
        sequence = []
        while True:
            item = branches[0].get_first()
            if not item:
                break
            if any(branch.get_first() != item for branch in branches[1 : ]):
                break
            sequence.append(item)
            branches = [branch.drop_first() for branch in branches]

        # Common trailing items can be moved out of the branches.
        suffix = []
        while True:
            item = branches[0].get_last()
            if not item:
                break
            if any(branch.get_last() != item for branch in branches[1 : ]):
                break
            suffix.append(item)
            branches = [branch.drop_last() for branch in branches]

        # Branches with the same character prefix can be grouped together if
        # they are separated only by other branches with a character prefix.
        char_type = None
        char_prefixes = defaultdict(list)
        order = {}
        new_branches = []
        for branch in branches:
            first = branch.get_first()
            if isinstance(first, _Character) and first.positive:
                if type(first) is not char_type:
                    self._flush_char_prefix(info, char_type, char_prefixes, order, new_branches)
                    char_type = type(first)
                    char_prefixes.clear()
                    order.clear()
                char_prefixes[first.value].append(branch)
                order.setdefault(first.value, len(order))
            else:
                self._flush_char_prefix(info, char_type, char_prefixes, order, new_branches)
                char_type = None
                char_prefixes.clear()
                order.clear()
                new_branches.append(branch)
        self._flush_char_prefix(info, char_type, char_prefixes, order, new_branches)
        branches = new_branches

        # Can the branches be reduced to a set?
        new_branches = []
        set_members = []
        for branch in branches:
            if type(branch) is _Any:
                set_members.append(branch)
            elif type(branch) is _Character and branch.positive:
                set_members.append(branch)
            elif isinstance(branch, _Property):
                set_members.append(branch)
            elif type(branch) is _Set and branch.positive:
                set_members.extend(branch.members)
            else:
                self._flush_set_members(info, True, set_members, new_branches)
                set_members = []
                new_branches.append(branch)
        self._flush_set_members(info, True, set_members, new_branches)

        if len(new_branches) == 1:
            sequence.append(new_branches[0])
        elif len(new_branches) > 1:
            sequence.append(_Branch(new_branches))
        sequence.extend(reversed(suffix))
        if len(sequence) == 1:
            return sequence[0]
        return _Sequence(sequence)
    def pack_characters(self):
        return _Branch([branch.pack_characters() for branch in self.branches])
    def is_empty(self):
        return all(branch.is_empty() for branch in self.branches)
    def is_atomic(self):
        return all(branch.is_atomic() for branch in self.branches)
    def contains_group(self):
        return any(branch.contains_group() for branch in self.branches)
    def compile(self, reverse=False):
        code = [(_OP.BRANCH, )]
        for branch in self.branches:
            code.extend(branch.compile(reverse))
            code.append((_OP.NEXT, ))
        code[-1] = (_OP.END, )
        return code
    def remove_captures(self):
        self.branches = [branch.remove_captures() for branch in self.branches]
        return self
    def dump(self, indent=0, reverse=False):
        print("{}BRANCH".format(_INDENT * indent))
        self.branches[0].dump(indent + 1, reverse)
        for branch in self.branches[1 : ]:
            print("{}OR".format(_INDENT * indent))
            branch.dump(indent + 1, reverse)
    def firstset(self):
        fs = set()
        for branch in self.branches:
            fs |= branch.firstset()
        return fs or set([None])
    def get_checks(self, reverse):
        branches = []
        for b in self.branches:
            b = b.get_checks(reverse)
            if isinstance(b, _Sequence) and not b.sequence:
                return b
            branches.append(b)
        if not branches:
            return _Sequence()
        if len(branches) == 1:
            return branches[0]
        return _Branch(branches)
    @staticmethod
    def _flush_char_prefix(info, char_type, prefixed, order, new_branches):
        for value, branches in sorted(prefixed.items(), key=lambda pair: order[pair[0]]):
            if len(branches) == 1:
                new_branches.extend(branches)
            else:
                subbranches = []
                optional = False
                for branch in branches:
                    b = branch.drop_first()
                    if b:
                        subbranches.append(b)
                    elif not optional:
                        subbranches.append(_Sequence())
                        optional = True
                sequence = _Sequence([char_type(value), _Branch(subbranches)])
                new_branches.append(sequence.optimise(info))
    @staticmethod
    def _flush_set_members(info, positive, set_members, new_branches):
        if set_members:
            new_branches.append(_Set(set_members, positive=positive).optimise(info))
    def __eq__(self, other):
        return type(self) is type(other) and self.branches == other.branches

class _Character(_RegexBase):
    _opcode = {False: _OP.CHARACTER, True: _OP.CHARACTER_REV}
    _op_name = {False: "CHARACTER", True: "CHARACTER_REV"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, ch, positive=True, zerowidth=False):
        try:
            self.value = ord(ch)
        except TypeError:
            self.value = ch
        self.positive, self.zerowidth = bool(positive), bool(zerowidth)
        self._key = self.__class__, self.value, self.positive, self.zerowidth
    def get_range(self):
        if self.positive:
            return (self.value, self.value)
        return None
    def compile(self, reverse=False):
        flags = int(self.positive) + _ZEROWIDTH_OP * int(self.zerowidth)
        return [(self._opcode[reverse], flags, self.value)]
    def dump(self, indent=0, reverse=False):
        print("{}{} {} {}".format(_INDENT * indent, self._op_name[reverse], self._pos_text[self.positive], self.value))
    def firstset(self):
        return set([self])
    def get_checks(self, reverse):
        if not self.positive:
            return _Sequence()
        return self
    def has_simple_start(self):
        return True
    def is_case_sensitive(self, info):
        if (info.global_flags & UNICODE) or (self.value <= 0x7F):
            # It's case-sensitive only if it's a letter.
            return chr(self.value).isalpha()
        else:
            # Assume that it's case-sensitive.
            return True

class _CharacterIgn(_Character):
    _opcode = {False: _OP.CHARACTER_IGN, True: _OP.CHARACTER_IGN_REV}
    _op_name = {False: "CHARACTER_IGN", True: "CHARACTER_IGN_REV"}
    def optimise(self, info):
        # Case-sensitive matches are faster, so convert to a case-sensitive
        # instance if the character is case-insensitive.
        if self.is_case_sensitive(info):
            return self
        return _Character(self.value, positive=self.positive, zerowidth=self.zerowidth)

class _Check(_RegexBase):
    _opcode = {False: _OP.CHECK, True: _OP.CHECK_REV}
    _op_name = {False: "CHECK", True: "CHECK_REV"}
    def __init__(self, check, body):
        self.check, self.body = check, body
        self._key = self.__class__
    def compile(self, reverse=False):
        code = self.check.compile(reverse)
        if code:
            code = [(self._opcode[reverse], )] + code + [(_OP.END, )]
        return code + self.body.compile(reverse)
    def dump(self, indent=0, reverse=False):
        print("{}{}".format(_INDENT * indent, self._op_name[reverse]))
        self.check.dump(indent + 1, reverse)
        self.body.dump(indent, reverse)
    def get_first_char(self):
        raise error("internal error")
    def drop_first_char(self):
        raise error("internal error")

class _Conditional(_StructureBase):
    def __init__(self, info, group, yes_item, no_item):
        self.info, self.group, self.yes_item, self.no_item = info, group, yes_item, no_item
    def fix_groups(self):
        try:
            self.group = int(self.group)
        except ValueError:
            try:
                self.group = self.info.group_index[self.group]
            except KeyError:
                raise error("unknown group")
        if not 1 <= self.group <= self.info.group_count:
            raise error("unknown group")
        self.yes_item.fix_groups()
        if self.no_item:
            self.no_item.fix_groups()
        else:
            self.no_item = _Sequence()
    def optimise(self, info):
        if self.yes_item.is_empty() and self.no_item.is_empty():
            return _Sequence()
        return _Conditional(self.info, self.group, self.yes_item.optimise(info), self.no_item.optimise(info))
    def pack_characters(self):
        return _Conditional(self.info, self.group, self.yes_item.pack_characters(), self.no_item.pack_characters())
    def is_empty(self):
        return self.yes_item.is_empty() and self.no_item.is_empty()
    def is_atomic(self):
        return self.yes_item.is_atomic() and self.no_item.is_atomic()
    def contains_group(self):
        return self.yes_item.contains_group() or self.no_item.contains_group()
    def compile(self, reverse=False):
        code = [(_OP.GROUP_EXISTS, self.group)]
        code.extend(self.yes_item.compile(reverse))
        add_code = self.no_item.compile(reverse)
        if add_code:
            code.append((_OP.NEXT, ))
            code.extend(add_code)
        code.append((_OP.END, ))
        return code
    def remove_captures(self):
        self.yes_item = self.yes_item.remove_captures()
        if self.no_item:
            self.no_item = self.no_item.remove_captures()
    def dump(self, indent=0, reverse=False):
        print("{}GROUP_EXISTS {}".format(_INDENT * indent, self.group))
        self.yes_item.dump(indent + 1, reverse)
        if self.no_item:
            print("{}ELSE".format(_INDENT * indent))
            self.no_item.dump(indent + 1, reverse)
    def firstset(self):
        return self.yes_item.firstset() | self.no_item.firstset()
    def get_checks(self, reverse):
        return _Branch([self.yes_item.get_checks(reverse), self.no_item.get_checks(reverse)])
    def __eq__(self, other):
        return type(self) is type(other) and (self.group, self.yes_item, self.no_item) == (other.group, other.yes_item, other.no_item)

class _DefaultBoundary(_ZeroWidthBase):
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, positive=True):
        self.positive = bool(positive)
        self._key = self.__class__, self.positive
    def compile(self, reverse=False):
        return [(_OP.DEFAULT_BOUNDARY, int(self.positive))]
    def dump(self, indent=0, reverse=False):
        print("{}DEFAULT_BOUNDARY {}".format(_INDENT * indent, self._pos_text[self.positive]))

class _EndOfLine(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.END_OF_LINE, )]
    def dump(self, indent=0, reverse=False):
        print("{}END_OF_LINE".format(_INDENT * indent))

class _EndOfString(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.END_OF_STRING, )]
    def dump(self, indent=0, reverse=False):
        print("{}END_OF_STRING".format(_INDENT * indent))

class _EndOfStringLine(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.END_OF_STRING_LINE, )]
    def dump(self, indent=0, reverse=False):
        print("{}END_OF_STRING_LINE".format(_INDENT * indent))

class _GreedyRepeat(_StructureBase):
    _opcode = _OP.GREEDY_REPEAT
    _op_name = "GREEDY_REPEAT"
    def __init__(self, subpattern, min_count, max_count):
        self.subpattern, self.min_count, self.max_count = subpattern, min_count, max_count
    def fix_groups(self):
        self.subpattern.fix_groups()
    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)
        if (self.min_count, self.max_count) == (1, 1) or subpattern.is_empty():
            return subpattern
        return type(self)(subpattern, self.min_count, self.max_count)
    def pack_characters(self):
        return type(self)(self.subpattern.pack_characters(), self.min_count, self.max_count)
    def is_empty(self):
        return self.subpattern.is_empty()
    def is_atomic(self):
        return self.min_count == self.max_count and self.subpattern.is_atomic()
    def contains_group(self):
        return self.subpattern.contains_group()
    def compile(self, reverse=False):
        repeat = [self._opcode, self.min_count]
        if self.max_count is None:
            repeat.append(_UNLIMITED)
        else:
            repeat.append(self.max_count)
        return [tuple(repeat)] + self.subpattern.compile(reverse) + [(_OP.END, )]
    def remove_captures(self):
        self.subpattern = self.subpattern.remove_captures()
        return self
    def dump(self, indent=0, reverse=False):
        if self.max_count is None:
            print("{}{} {} INF".format(_INDENT * indent, self._op_name, self.min_count))
        else:
            print("{}{} {} {}".format(_INDENT * indent, self._op_name, self.min_count, self.max_count))
        self.subpattern.dump(indent + 1, reverse)
    def firstset(self):
        fs = self.subpattern.firstset()
        if self.min_count == 0:
            fs.add(None)
        return fs or set([None])
    def get_checks(self, reverse):
        if self.min_count == 0:
            return _Sequence()
        return self.subpattern.get_checks(reverse)
    def __eq__(self, other):
        return type(self) is type(other) and (self.subpattern, self.min_count, self.max_count) == (other.subpattern, other.min_count, other.max_count)

class _Group(_StructureBase):
    def __init__(self, info, group, subpattern):
        self.info, self.group, self.subpattern = info, group, subpattern
    def fix_groups(self):
        self.subpattern.fix_groups()
    def optimise(self, info):
        return _Group(self.info, self.group, self.subpattern.optimise(info))
    def pack_characters(self):
        return _Group(self.info, self.group, self.subpattern.pack_characters())
    def is_empty(self):
        return self.subpattern.is_empty()
    def is_atomic(self):
        return self.subpattern.is_atomic()
    def contains_group(self):
        return True
    def compile(self, reverse=False):
        return [(_OP.GROUP, self.group)] + self.subpattern.compile(reverse) + [(_OP.END, )]
    def remove_captures(self):
        return self.subpattern.remove_captures()
    def dump(self, indent=0, reverse=False):
        print("{}GROUP {}".format(_INDENT * indent, self.group))
        self.subpattern.dump(indent + 1, reverse)
    def firstset(self):
        return self.subpattern.firstset()
    def get_checks(self, reverse):
        return self.subpattern.get_checks(reverse)
    def __eq__(self, other):
        return type(self) is type(other) and (self.group, self.subpattern) == (other.group, other.subpattern)

class _LazyRepeat(_GreedyRepeat):
    _opcode = _OP.LAZY_REPEAT
    _op_name = "LAZY_REPEAT"

class _LookAround(_StructureBase):
    _dir_text = {False: "AHEAD", True: "BEHIND"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, behind, positive, subpattern):
        self.behind, self.positive, self.subpattern = bool(behind), bool(positive), subpattern
    def fix_groups(self):
        self.subpattern.fix_groups()
    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)
        if self.positive and subpattern.is_empty():
            return subpattern
        return _LookAround(self.behind, self.positive, subpattern)
    def pack_characters(self):
        return _LookAround(self.behind, self.positive, self.subpattern.pack_characters())
    def is_empty(self):
        return self.subpattern.is_empty()
    def is_atomic(self):
        return self.subpattern.is_atomic()
    def contains_group(self):
        return self.subpattern.contains_group()
    def compile(self, reverse=False):
        return [(_OP.LOOKAROUND, int(self.positive), int(not self.behind))] + self.subpattern.compile(self.behind) + [(_OP.END, )]
    def dump(self, indent=0, reverse=False):
        print("{}LOOKAROUND {} {}".format(_INDENT * indent, self._dir_text[self.behind], self._pos_text[self.positive]))
        self.subpattern.dump(indent + 1, self.behind)
    def firstset(self):
        return set([None])
    def __eq__(self, other):
        return type(self) is type(other) and (self.behind, self.positive, self.subpattern) == (other.behind, other.positive, other.subpattern)
    def can_repeat(self):
        return False

class _Property(_RegexBase):
    _opcode = {False: _OP.PROPERTY, True: _OP.PROPERTY_REV}
    _op_name = {False: "PROPERTY", True: "PROPERTY_REV"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, value, positive=True, zerowidth=False):
        self.value, self.positive, self.zerowidth = value, bool(positive), bool(zerowidth)
        self._key = self.__class__, self.value, self.positive, self.zerowidth
    def compile(self, reverse=False):
        flags = int(self.positive) + _ZEROWIDTH_OP * int(self.zerowidth)
        return [(self._opcode[reverse], flags, self.value)]
    def dump(self, indent=0, reverse=False):
        print("{}{} {} {}".format(_INDENT * indent, self._op_name[reverse], self._pos_text[self.positive], self.value))
    def firstset(self):
        return set([self])
    def has_simple_start(self):
        return True
    def is_case_sensitive(self, info):
        return True

class _Range(_RegexBase):
    _opcode = {False: _OP.RANGE, True: _OP.RANGE_REV}
    _op_name = {False: "RANGE", True: "RANGE_REV"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, min_value, max_value, positive=True, zerowidth=False):
        self.min_value, self.max_value, self.positive, self.zerowidth = min_value, max_value, positive, zerowidth
        self._key = self.__class__, self.min_value, self.max_value, self.positive, self.zerowidth
    def optimise(self, info):
        if self.min_value == self.max_value:
            return _as_character(self.min_value)
        return self
    def compile(self, reverse=False):
        flags = int(self.positive) + _ZEROWIDTH_OP * int(self.zerowidth)
        return [(self._opcode[reverse], flags, self.min_value, self.max_value)]
    def dump(self, indent=0, reverse=False):
        print("{}{} {} {} {}".format(_INDENT * indent, self._op_name[reverse], self._pos_text[self.positive], self.min_value, self.max_value))
    def get_range(self):
        return (self.min_value, self.max_value)
    def is_case_sensitive(self, info):
        return True
    def _as_character(self, value):
        return _Character(value, positive=self.positive, zerowidth=self.zerowidth)

class _RangeIgn(_Range):
    _opcode = {False: _OP.RANGE_IGN, True: _OP.RANGE_IGN_REV}
    _op_name = {False: "RANGE_IGN", True: "RANGE_IGN_REV"}
    def is_case_sensitive(self, info):
        return False
    def _as_character(self, value):
        return _CharacterIgn(value, positive=self.positive, zerowidth=self.zerowidth)

class _RefGroup(_RegexBase):
    _opcode = {False: _OP.REF_GROUP, True: _OP.REF_GROUP_REV}
    _op_name = {False: "REF_GROUP", True: "REF_GROUP_REV"}
    def __init__(self, info, group):
        self.info, self.group = info, group
        self._key = self.__class__, self.group
    def fix_groups(self):
        try:
            self.group = int(self.group)
        except ValueError:
            try:
                self.group = self.info.group_index[self.group]
            except KeyError:
                raise error("unknown group")
        if not 1 <= self.group <= self.info.group_count:
            raise error("unknown group")
        self._key = self.__class__, self.group
    def compile(self, reverse=False):
        return [(self._opcode[reverse], self.group)]
    def remove_captures(self):
        raise error("group reference not allowed")
    def dump(self, indent=0, reverse=False):
        print("{}{} {}".format(_INDENT * indent, self._op_name[reverse], self.group))

class _RefGroupIgn(_RefGroup):
    _opcode = {False: _OP.REF_GROUP_IGN, True: _OP.REF_GROUP_IGN_REV}
    _op_name = {False: "REF_GROUP_IGN", True: "REF_GROUP_IGN_REV"}

class _SearchAnchor(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.SEARCH_ANCHOR, )]
    def dump(self, indent=0, reverse=False):
        print("{}SEARCH_ANCHOR".format(_INDENT * indent))

class _Sequence(_StructureBase):
    def __init__(self, sequence=None):
        if sequence is None:
            sequence = []
        self.sequence = sequence
    def fix_groups(self):
        for subpattern in self.sequence:
            subpattern.fix_groups()
    def optimise(self, info):
        # Flatten the sequences.
        sequence = []
        for subpattern in self.sequence:
            subpattern = subpattern.optimise(info)
            if isinstance(subpattern, _Sequence):
                sequence.extend(subpattern.sequence)
            else:
                sequence.append(subpattern)
        if len(sequence) == 1:
            return sequence[0]
        return _Sequence(sequence)
    def pack_characters(self):
        sequence = []
        char_type, characters = _Character, []
        for subpattern in self.sequence:
            if type(subpattern) is char_type and subpattern.positive:
                characters.append(subpattern.value)
            else:
                self._flush_characters(char_type, characters, sequence)
                characters = []
                if type(subpattern) in _all_char_types and subpattern.positive:
                    char_type = type(subpattern)
                    characters.append(subpattern.value)
                else:
                    sequence.append(subpattern.pack_characters())
        self._flush_characters(char_type, characters, sequence)
        if len(sequence) == 1:
            return sequence[0]
        return _Sequence(sequence)
    def is_empty(self):
        return all(subpattern.is_empty() for subpattern in self.sequence)
    def is_atomic(self):
        return all(subpattern.is_atomic() for subpattern in self.sequence)
    def contains_group(self):
        return any(subpattern.contains_group() for subpattern in self.sequence)
    def get_first(self):
        if self.sequence:
            return self.sequence[0]
        return None
    def drop_first(self):
        if len(self.sequence) == 2:
            return self.sequence[1]
        return _Sequence(self.sequence[1 : ])
    def get_last(self):
        if self.sequence:
            return self.sequence[-1]
        return None
    def drop_last(self):
        if len(self.sequence) == 2:
            return self.sequence[-2]
        return _Sequence(self.sequence[ : -1])
    def compile(self, reverse=False):
        if reverse:
            seq = reversed(self.sequence)
        else:
            seq = self.sequence
        check_code = []
        code = []
        for subpattern in seq:
            if isinstance(subpattern, _Check):
                check_code.extend(subpattern.compile(reverse))
            else:
                code.extend(subpattern.compile(reverse))
        return check_code + code
    def remove_captures(self):
        self.sequence = [subpattern.remove_captures() for subpattern in self.sequence]
        return self
    def dump(self, indent=0, reverse=False):
        for subpattern in self.sequence:
            subpattern.dump(indent, reverse)
    def firstset(self):
        fs = set()
        for subpattern in self.sequence:
            fs = (fs - set([None])) | subpattern.firstset()
            if None not in fs:
                return fs
        return fs or set([None])
    def has_simple_start(self):
        return self.sequence and self.sequence[0].has_simple_start()
    def get_checks(self, reverse):
        return _Sequence([subpattern.get_checks(reverse) for subpattern in self.sequence])
    @staticmethod
    def _flush_characters(char_type, characters, sequence):
        if not characters:
            return
        if len(characters) == 1:
            sequence.append(char_type(characters[0]))
        else:
            sequence.append(_string_classes[char_type](characters))
    def __eq__(self, other):
        return type(self) is type(other) and self.sequence == other.sequence

class _Set(_RegexBase):
    _opcode = {False: _OP.SET, True: _OP.SET_REV}
    _op_name = {False: "SET", True: "SET_REV"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    _big_bitset_opcode = {
      (False, False): _OP.BIG_BITSET, (False, True): _OP.BIG_BITSET_REV,
      (True, False): _OP.BIG_BITSET_IGN, (True, True): _OP.BIG_BITSET_IGN_REV
    }
    _small_bitset_opcode = {
      (False, False): _OP.SMALL_BITSET, (False, True): _OP.SMALL_BITSET_REV,
      (True, False): _OP.SMALL_BITSET_IGN, (True, True): _OP.SMALL_BITSET_IGN_REV
    }
    _ignore = False
    def __init__(self, members, positive=True, zerowidth=False):
        self.members, self.positive, self.zerowidth = members, bool(positive), zerowidth
        self._key = self.__class__, tuple(self.members), self.positive, self.zerowidth
    def optimise(self, info):
        # Divide the set members into ranges and non-ranges.
        ranges = []
        others = set()
        for member in self.members:
            r = member.get_range()
            if r:
                ranges.append(r)
            else:
                others.add(member)

        # If the set contains complementary properties then the set itself is
        # equivalent to _AnyAll.
        prop = {False: set(), True: set()}
        for o in others:
            if isinstance(o, _Property):
                prop[o.positive].add(o.value)
        if self.positive and (prop[False] & prop[True]) and not self.zerowidth:
            return _AnyAll()

        # Merge consecutive and overlapping ranges.
        ranges.sort()
        for i in range(len(ranges), 1, -1):
            r1, r2 = ranges[i - 2 : i]
            if r1[1] + 1 >= r2[0]:
                ranges[i - 2 : i] = [(r1[0], max(r1[1], r2[1]))]

        # We can simplify if the set contains just a single range.
        if len(ranges) == 1 and not others:
            r = ranges[0]
            if r[0] == r[1]:
                return self._as_character(r[0])
            else:
                return self._as_range(*r)

        # We can simplify if the set contains just a single non-range (with
        # certain exceptions).
        if not ranges and len(others) == 1:
            o = list(others)[0]
            if type(o) is _Any and self.positive and not self.zerowidth:
                return o
            if isinstance(o, _Property):
                return _Property(o.value, positive=self.positive == o.positive, zerowidth=self.zerowidth)

        # Rebuild the set.
        new_ranges = []
        for r in ranges:
            if r[0] == r[1]:
                new_ranges.append(_Character(r[0]))
            else:
                new_ranges.append(_Range(*r))

        return type(self)(new_ranges + list(others), positive=self.positive, zerowidth=self.zerowidth)
    def compile(self, reverse=False):
        # Divide the set members into ranges and non-ranges.
        ranges = []
        others = []
        for m in self.members:
            r = m.get_range()
            if r:
                ranges.append(r)
            else:
                others.append(m)

        # If there are only ranges then compile to a character, range or bitset.
        if not others:
            if len(ranges) == 1:
                r = ranges[0]
                if r[0] == r[1]:
                    return self._as_character(r[0]).compile(reverse)
                else:
                    return self._as_range(r[0], r[1]).compile(reverse)
            else:
                return self._make_bitset(ranges, self._ignore, reverse)

        # If there's only a single other then compile to just that.
        if not ranges and len(others) == 1:
            o = others[0]
            o.positive = o.positive == self.positive
            o.zerowidth = self.zerowidth
            return o.compile(reverse)

        # Compile a compound set.
        flags = int(self.positive) + _ZEROWIDTH_OP * int(self.zerowidth)
        code = [(self._opcode[reverse], flags)]
        code.extend(self._make_bitset(ranges, False, False))
        for o in others:
            code.extend(o.compile())
        code.append((_OP.END, ))
        return code
    def dump(self, indent=0, reverse=False):
        print("{}{} {}".format(_INDENT * indent, self._op_name[reverse], self._pos_text[self.positive]))
        for subpattern in self.members:
            subpattern.dump(indent + 1)
    def firstset(self):
        return set([self])
    def has_simple_start(self):
        return True
    BITS_PER_CODE = 32
    BITS_PER_INDEX = 16
    INDEXES_PER_CODE = BITS_PER_CODE // BITS_PER_INDEX
    CODE_MASK = (1 << BITS_PER_CODE) - 1
    CODES_PER_SUBSET = 256 // BITS_PER_CODE
    SUBSET_MASK = (1 << 256) - 1
    def _make_bitset(self, ranges, ignore, reverse):
        code = []
        # values for big bitset are: max_char indexes... subsets...
        # values for small bitset are: top_bits bitset
        bitset_dict = defaultdict(int)
        for r in ranges:
            lo_top, lo_bottom = r[0] >> 8, r[0] & 0xFF
            hi_top, hi_bottom = r[1] >> 8, r[1] & 0xFF
            if lo_top == hi_top:
                # The range is in a single subset.
                bitset_dict[lo_top] |= (1 << (hi_bottom + 1)) - (1 << lo_bottom)
            else:
                # The range crosses a subset boundary.
                bitset_dict[lo_top] |= (1 << 256) - (1 << lo_bottom)
                for top in range(lo_top + 1, hi_top):
                    bitset_dict[top] = (1 << 256) - 1
                bitset_dict[hi_top] |= (1 << (hi_bottom + 1)) - 1
        if len(bitset_dict) > 1:
            # Build a big bitset.
            indexes = []
            subset_index = {}
            for top in range(max(bitset_dict.keys()) + 1):
                subset = bitset_dict[top]
                ind = subset_index.setdefault(subset, len(subset_index))
                indexes.append(ind)
            if len(indexes) % self.INDEXES_PER_CODE > 0:
                indexes.extend([0] * (self.INDEXES_PER_CODE - len(indexes) % self.INDEXES_PER_CODE))
            max_char = ranges[-1][1]
            data = []
            for i in range(0, len(indexes), self.INDEXES_PER_CODE):
                ind = 0
                for s in range(self.INDEXES_PER_CODE):
                    ind |= indexes[i + s] << (self.BITS_PER_INDEX * s)
                data.append(ind)
            for subset, ind in sorted(subset_index.items(), key=lambda pair: pair[1]):
                data.extend(self._bitset_to_codes(subset))
            flags = int(self.positive) + _ZEROWIDTH_OP * int(self.zerowidth)
            code.append((self._big_bitset_opcode[(ignore, reverse)], flags, max_char) + tuple(data))
        else:
            # Build a small bitset.
            flags = int(self.positive) + _ZEROWIDTH_OP * int(self.zerowidth)
            for top_bits, bitset in bitset_dict.items():
                if bitset:
                    code.append((self._small_bitset_opcode[(ignore, reverse)],
                      flags, top_bits) + tuple(self._bitset_to_codes(bitset)))
        return code
    def _bitset_to_codes(self, bitset):
        codes = []
        for i in range(self.CODES_PER_SUBSET):
            codes.append(bitset & self.CODE_MASK)
            bitset >>= self.BITS_PER_CODE
        return codes
    def _as_character(self, value):
        return _Character(value, positive=self.positive, zerowidth=self.zerowidth)
    def _as_range(self, min_value, max_value):
        return _Range(min_value, max_value, positive=self.positive, zerowidth=self.zerowidth)

class _SetIgn(_Set):
    _opcode = {False: _OP.SET_IGN, True: _OP.SET_IGN_REV}
    _op_name = {False: "SET_IGN", True: "SET_IGN_REV"}
    _ignore = True
    def optimise(self, info):
        # Case-sensitive matches are faster, so convert to a case-sensitive
        # instance if all the members are case-insensitive.
        if any(member.is_case_sensitive(info) for member in self.members):
            return _Set.optimise(self, info)
        return _Set(self.members, positive=self.positive, zerowidth=self.zerowidth).optimise(info)
    def _as_character(self, value):
        return _CharacterIgn(value, positive=self.positive, zerowidth=self.zerowidth)
    def _as_range(self, min_value, max_value):
        return _RangeIgn(min_value, max_value, positive=self.positive, zerowidth=self.zerowidth)

class _StartOfLine(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.START_OF_LINE, )]
    def dump(self, indent=0, reverse=False):
        print("{}START_OF_LINE".format(_INDENT * indent))

class _StartOfString(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.START_OF_STRING, )]
    def dump(self, indent=0, reverse=False):
        print("{}START_OF_STRING".format(_INDENT * indent))

class _String(_RegexBase):
    _opcode = {False: _OP.STRING, True: _OP.STRING_REV}
    _op_name = {False: "STRING", True: "STRING_REV"}
    def __init__(self, characters):
        self.characters = characters
        self._key = self.__class__, tuple(self.characters)
    def compile(self, reverse=False):
        return [(self._opcode[reverse], len(self.characters)) + tuple(self.characters)]
    def dump(self, indent=0, reverse=False):
        print("{}{} {}".format(_INDENT * indent, self._op_name[reverse], " ".join(str(ch) for ch in self.characters)))
    def firstset(self):
        return set([_Character(self.characters[0])])
    def has_simple_start(self):
        return True
    def get_checks(self, reverse):
        return self
    def get_first_char(self):
        raise error("internal error")
    def drop_first_char(self):
        raise error("internal error")

class _StringIgn(_String):
    _opcode = {False: _OP.STRING_IGN, True: _OP.STRING_IGN_REV}
    _op_name = {False: "STRING_IGN", True: "STRING_IGN_REV"}
    def firstset(self):
        return set([_CharacterIgn(self.characters[0])])

_all_char_types = (_Character, _CharacterIgn)
_string_classes = {_Character: _String, _CharacterIgn: _StringIgn}

# Character escape sequences.
_CHARACTER_ESCAPES = {
    "a": ord("\a"),
    "b": ord("\b"),
    "f": ord("\f"),
    "n": ord("\n"),
    "r": ord("\r"),
    "t": ord("\t"),
    "v": ord("\v"),
}

# Predefined character set escape sequences.
_CHARSET_ESCAPES = {
    "d": _Property(_properties["DIGIT"]),
    "D": _Property(_properties["DIGIT"], positive=False),
    "s": _Property(_properties["SPACE"]),
    "S": _Property(_properties["SPACE"], positive=False),
    "w": _Property(_properties["WORD"]),
    "W": _Property(_properties["WORD"], positive=False),
}

# Positional escape sequences.
_POSITION_ESCAPES = {
    "A": _StartOfString(),
    "b": _Boundary(),
    "B": _Boundary(False),
    "Z": _EndOfString(),
}

# Positional escape sequences when WORD flag set.
_WORD_POSITION_ESCAPES = dict(_POSITION_ESCAPES)
_WORD_POSITION_ESCAPES.update({
    "b": _DefaultBoundary(),
    "B": _DefaultBoundary(False),
})

class _Source:
    "Scanner for the regular expression source string."
    def __init__(self, string):
        if isinstance(string, bytes):
            string = "".join(chr(c) for c in string)
        self.string = string
        self.pos = 0
        self.ignore_space = False
        self.sep = string[ : 0]
    def get(self):
        try:
            if self.ignore_space:
                while self.string[self.pos].isspace():
                    self.pos += 1
            ch = self.string[self.pos]
            self.pos += 1
            return ch
        except IndexError:
            return self.sep
    def match(self, substring):
        try:
            if self.ignore_space:
                while self.string[self.pos].isspace():
                    self.pos += 1
            if not self.string.startswith(substring, self.pos):
                return False
            self.pos += len(substring)
            return True
        except IndexError:
            return False
    def expect(self, substring):
        if not self.match(substring):
            raise error("missing {}".format(substring))
    def at_end(self):
        pos = self.pos
        try:
            if self.ignore_space:
                while self.string[pos].isspace():
                    pos += 1
            return pos >= len(self.string)
        except IndexError:
            return True
    def tell(self):
        return self.pos
    def seek(self, pos):
        self.pos = pos

class _Info:
    "Info about the regular expression."
    def __init__(self, flags=0):
        self.global_flags = flags & _GLOBAL_FLAGS
        self.local_flags = flags & _LOCAL_FLAGS
        self.group_count = 0
        self.group_index = {}
        self.group_name = {}
        self.used_groups = set()
        self.group_state = {}
    def new_group(self, name=None):
        group = self.group_index.get(name)
        if group is not None:
            if group in self.used_groups:
                raise error("duplicate group")
        else:
            while True:
                self.group_count += 1
                if name is None or self.group_count not in self.group_name:
                    break
            group = self.group_count
            if name:
                self.group_index[name] = group
                self.group_name[group] = name
        self.used_groups.add(group)
        self.group_state[group] = "OPEN"
        return group
    def close_group(self, group):
        self.group_state[group] = "CLOSED"
    def is_open_group(self, name):
        if name.isdigit():
            group = int(name)
        else:
            group = self.group_index.get(name)
        return self.group_state.get(group) == "OPEN"

# --------------------------------------------------------------------
# Experimental stuff (see python-dev discussions for details).

import _regex

class Scanner:
    def __init__(self, lexicon, flags=0):
        self.lexicon = lexicon

        # Combine phrases into a compound pattern.
        patterns = []
        for phrase, action in lexicon:
            # Parse the regular expression.
            source = _Source(phrase)
            info = _Info(flags)
            source.ignore_space = bool(info.local_flags & VERBOSE)
            parsed = _parse_pattern(source, info)
            if not source.at_end():
                raise error("trailing characters")

            # We want to forbid capture groups within each phrase.
            patterns.append(parsed.remove_captures())

        # Combine all the subpatterns into one pattern.
        info = _Info(flags)
        patterns = [_Group(info, g + 1, p) for g, p in enumerate(patterns)]
        parsed = _Branch(patterns)

        # Optimise the compound pattern.
        parsed = parsed.optimise(info)

        # Compile the compound pattern. The result is a list of tuples.
        code = parsed.compile() + [(_OP.SUCCESS, )]

        # Flatten the code into a list of ints.
        code = _flatten_code(code)

        # Create the PatternObject.
        #
        # Local flags like IGNORECASE affect the code generation, but aren't
        # needed by the PatternObject itself. Conversely, global flags like
        # LOCALE _don't_ affect the code generation but _are_ needed by the
        # PatternObject.
        self.scanner = _regex.compile(None, flags & _GLOBAL_FLAGS, code, {}, {})
    def scan(self, string):
        result = []
        append = result.append
        match = self.scanner.scanner(string).match
        i = 0
        while True:
            m = match()
            if not m:
                break
            j = m.end()
            if i == j:
                break
            action = self.lexicon[m.lastindex - 1][1]
            if hasattr(action, '__call__'):
                self.match = m
                action = action(self, m.group())
            if action is not None:
                append(action)
            i = j
        return result, string[i : ]
