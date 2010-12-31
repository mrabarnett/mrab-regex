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
A = ASCII = 0x80         # Assume ASCII locale.
D = DEBUG = 0x200        # Print parsed pattern.
I = IGNORECASE = 0x2     # Ignore case.
L = LOCALE = 0x4         # Assume current 8-bit locale.
M = MULTILINE = 0x8      # Make anchors look for newline.
N = NEW = 0x100          # Scoped inline flags and correct handling of zero-width matches.
R = REVERSE = 0x400      # Search backwards.
S = DOTALL = 0x10        # Make dot match newline.
U = UNICODE = 0x20       # Assume Unicode locale.
W = WORD = 0x800         # Default Unicode word breaks.
X = VERBOSE = 0x40       # Ignore whitespace and comments.
T = TEMPLATE = 0x1       # Template.

# The mask for the flags.
_GLOBAL_FLAGS = ASCII | DEBUG | LOCALE | NEW | REVERSE | UNICODE
_SCOPED_FLAGS = IGNORECASE | MULTILINE | DOTALL | WORD | VERBOSE

# The regex exception.
class error(Exception):
   pass

# The exception for when a positional flag has been turned on in the old
# behaviour.
class _UnscopedFlagSet(Exception):
    def __init__(self, global_flags):
        Exception.__init__(self)
        self.global_flags = global_flags

_ALPHA = frozenset("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
_DIGITS = frozenset("0123456789")
_ALNUM = _ALPHA | _DIGITS
_OCT_DIGITS = frozenset("01234567")
_HEX_DIGITS = frozenset("0123456789ABCDEFabcdef")
_SPECIAL = frozenset("()[]{}?*+|^$\\")

import sys
import unicodedata
from collections import defaultdict

import _regex

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
    # The import is done here to avoid a circular dependency.
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

# The width of the code words inside the regex engine.
_BYTES_PER_CODE = _regex.get_code_size()
_BITS_PER_CODE = _BYTES_PER_CODE * 8

# The repeat count which represents infinity.
_UNLIMITED = (1 << _BITS_PER_CODE) - 1

# The names of the opcodes.
_OPCODES = """\
FAILURE
SUCCESS
ANY
ANY_ALL
ANY_ALL_REV
ANY_REV
ATOMIC
BIG_BITSET
BIG_BITSET_REV
BOUNDARY
BRANCH
CHARACTER
CHARACTER_IGN
CHARACTER_IGN_REV
CHARACTER_REV
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
REF_GROUP
REF_GROUP_IGN
REF_GROUP_IGN_REV
REF_GROUP_REV
SEARCH_ANCHOR
SET
SET_REV
SMALL_BITSET
SMALL_BITSET_REV
START_GROUP
START_OF_LINE
START_OF_STRING
STRING
STRING_IGN
STRING_IGN_REV
STRING_REV
TAIL_GUARD
"""

def _define_opcodes(opcodes):
    "Defines the opcodes and their numeric values."
    # The namespace for the opcodes.
    class Record(object):
        pass

    _OP = Record()

    for i, op in enumerate(filter(None, opcodes.splitlines())):
        setattr(_OP, op, i)

    return _OP

# Define the opcodes in a namespace.
_OP = _define_opcodes(_OPCODES)

# The regular expression flags.
_REGEX_FLAGS = {"a": ASCII, "i": IGNORECASE, "L": LOCALE, "m": MULTILINE, "n":
  NEW, "r": REVERSE, "s": DOTALL, "u": UNICODE, "w": WORD, "x": VERBOSE}

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
73 InMandaic Mandaic
74 Devanagari InDevanagari
75 Bengali InBengali
76 Gurmukhi InGurmukhi
77 Gujarati InGujarati
78 InOriya Oriya
79 InTamil Tamil
80 InTelugu Telugu
81 InKannada Kannada
82 InMalayalam Malayalam
83 InSinhala Sinhala
84 InThai Thai
85 InLao Lao
86 InTibetan Tibetan
87 InMyanmar Myanmar
88 Georgian InGeorgian
89 HangulJamo InHangulJamo
90 Ethiopic InEthiopic
91 EthiopicSupplement InEthiopicSupplement
92 Cherokee InCherokee
93 InUnifiedCanadianAboriginalSyllabics UnifiedCanadianAboriginalSyllabics
94 InOgham Ogham
95 InRunic Runic
96 InTagalog Tagalog
97 Hanunoo InHanunoo
98 Buhid InBuhid
99 InTagbanwa Tagbanwa
100 InKhmer Khmer
101 InMongolian Mongolian
102 InUnifiedCanadianAboriginalSyllabicsExtended UnifiedCanadianAboriginalSyllabicsExtended
103 InLimbu Limbu
104 InTaiLe TaiLe
105 InNewTaiLue NewTaiLue
106 InKhmerSymbols KhmerSymbols
107 Buginese InBuginese
108 InTaiTham TaiTham
109 Balinese InBalinese
110 InSundanese Sundanese
111 Batak InBatak
112 InLepcha Lepcha
113 InOlChiki OlChiki
114 InVedicExtensions VedicExtensions
115 InPhoneticExtensions PhoneticExtensions
116 InPhoneticExtensionsSupplement PhoneticExtensionsSupplement
117 CombiningDiacriticalMarksSupplement InCombiningDiacriticalMarksSupplement
118 InLatinExtendedAdditional LatinExtendedAdditional
119 GreekExtended InGreekExtended
120 GeneralPunctuation InGeneralPunctuation
121 InSuperscriptsandSubscripts SuperscriptsandSubscripts
122 CurrencySymbols InCurrencySymbols
123 CombiningDiacriticalMarksforSymbols InCombiningDiacriticalMarksforSymbols
124 InLetterlikeSymbols LetterlikeSymbols
125 InNumberForms NumberForms
126 Arrows InArrows
127 InMathematicalOperators MathematicalOperators
128 InMiscellaneousTechnical MiscellaneousTechnical
129 ControlPictures InControlPictures
130 InOpticalCharacterRecognition OpticalCharacterRecognition
131 EnclosedAlphanumerics InEnclosedAlphanumerics
132 BoxDrawing InBoxDrawing
133 BlockElements InBlockElements
134 GeometricShapes InGeometricShapes
135 InMiscellaneousSymbols MiscellaneousSymbols
136 Dingbats InDingbats
137 InMiscellaneousMathematicalSymbolsA MiscellaneousMathematicalSymbolsA
138 InSupplementalArrowsA SupplementalArrowsA
139 BraillePatterns InBraillePatterns
140 InSupplementalArrowsB SupplementalArrowsB
141 InMiscellaneousMathematicalSymbolsB MiscellaneousMathematicalSymbolsB
142 InSupplementalMathematicalOperators SupplementalMathematicalOperators
143 InMiscellaneousSymbolsandArrows MiscellaneousSymbolsandArrows
144 Glagolitic InGlagolitic
145 InLatinExtendedC LatinExtendedC
146 Coptic InCoptic
147 GeorgianSupplement InGeorgianSupplement
148 InTifinagh Tifinagh
149 EthiopicExtended InEthiopicExtended
150 CyrillicExtendedA InCyrillicExtendedA
151 InSupplementalPunctuation SupplementalPunctuation
152 CJKRadicalsSupplement InCJKRadicalsSupplement
153 InKangxiRadicals KangxiRadicals
154 IdeographicDescriptionCharacters InIdeographicDescriptionCharacters
155 CJKSymbolsandPunctuation InCJKSymbolsandPunctuation
156 Hiragana InHiragana
157 InKatakana Katakana
158 Bopomofo InBopomofo
159 HangulCompatibilityJamo InHangulCompatibilityJamo
160 InKanbun Kanbun
161 BopomofoExtended InBopomofoExtended
162 CJKStrokes InCJKStrokes
163 InKatakanaPhoneticExtensions KatakanaPhoneticExtensions
164 EnclosedCJKLettersandMonths InEnclosedCJKLettersandMonths
165 CJKCompatibility InCJKCompatibility
166 CJKUnifiedIdeographsExtensionA InCJKUnifiedIdeographsExtensionA
167 InYijingHexagramSymbols YijingHexagramSymbols
168 CJKUnifiedIdeographs InCJKUnifiedIdeographs
169 InYiSyllables YiSyllables
170 InYiRadicals YiRadicals
171 InLisu Lisu
172 InVai Vai
173 CyrillicExtendedB InCyrillicExtendedB
174 Bamum InBamum
175 InModifierToneLetters ModifierToneLetters
176 InLatinExtendedD LatinExtendedD
177 InSylotiNagri SylotiNagri
178 CommonIndicNumberForms InCommonIndicNumberForms
179 InPhagspa Phagspa
180 InSaurashtra Saurashtra
181 DevanagariExtended InDevanagariExtended
182 InKayahLi KayahLi
183 InRejang Rejang
184 HangulJamoExtendedA InHangulJamoExtendedA
185 InJavanese Javanese
186 Cham InCham
187 InMyanmarExtendedA MyanmarExtendedA
188 InTaiViet TaiViet
189 EthiopicExtendedA InEthiopicExtendedA
190 InMeeteiMayek MeeteiMayek
191 HangulSyllables InHangulSyllables
192 HangulJamoExtendedB InHangulJamoExtendedB
193 HighSurrogates InHighSurrogates
194 HighPrivateUseSurrogates InHighPrivateUseSurrogates
195 InLowSurrogates LowSurrogates
196 InPrivateUseArea PrivateUseArea
197 CJKCompatibilityIdeographs InCJKCompatibilityIdeographs
198 AlphabeticPresentationForms InAlphabeticPresentationForms
199 ArabicPresentationFormsA InArabicPresentationFormsA
200 InVariationSelectors VariationSelectors
201 InVerticalForms VerticalForms
202 CombiningHalfMarks InCombiningHalfMarks
203 CJKCompatibilityForms InCJKCompatibilityForms
204 InSmallFormVariants SmallFormVariants
205 ArabicPresentationFormsB InArabicPresentationFormsB
206 HalfwidthandFullwidthForms InHalfwidthandFullwidthForms
207 InSpecials Specials
208 InLinearBSyllabary LinearBSyllabary
209 InLinearBIdeograms LinearBIdeograms
210 AegeanNumbers InAegeanNumbers
211 AncientGreekNumbers InAncientGreekNumbers
212 AncientSymbols InAncientSymbols
213 InPhaistosDisc PhaistosDisc
214 InLycian Lycian
215 Carian InCarian
216 InOldItalic OldItalic
217 Gothic InGothic
218 InUgaritic Ugaritic
219 InOldPersian OldPersian
220 Deseret InDeseret
221 InShavian Shavian
222 InOsmanya Osmanya
223 CypriotSyllabary InCypriotSyllabary
224 ImperialAramaic InImperialAramaic
225 InPhoenician Phoenician
226 InLydian Lydian
227 InKharoshthi Kharoshthi
228 InOldSouthArabian OldSouthArabian
229 Avestan InAvestan
230 InInscriptionalParthian InscriptionalParthian
231 InInscriptionalPahlavi InscriptionalPahlavi
232 InOldTurkic OldTurkic
233 InRumiNumeralSymbols RumiNumeralSymbols
234 Brahmi InBrahmi
235 InKaithi Kaithi
236 Cuneiform InCuneiform
237 CuneiformNumbersandPunctuation InCuneiformNumbersandPunctuation
238 EgyptianHieroglyphs InEgyptianHieroglyphs
239 BamumSupplement InBamumSupplement
240 InKanaSupplement KanaSupplement
241 ByzantineMusicalSymbols InByzantineMusicalSymbols
242 InMusicalSymbols MusicalSymbols
243 AncientGreekMusicalNotation InAncientGreekMusicalNotation
244 InTaiXuanJingSymbols TaiXuanJingSymbols
245 CountingRodNumerals InCountingRodNumerals
246 InMathematicalAlphanumericSymbols MathematicalAlphanumericSymbols
247 InMahjongTiles MahjongTiles
248 DominoTiles InDominoTiles
249 InPlayingCards PlayingCards
250 EnclosedAlphanumericSupplement InEnclosedAlphanumericSupplement
251 EnclosedIdeographicSupplement InEnclosedIdeographicSupplement
252 InMiscellaneousSymbolsAndPictographs MiscellaneousSymbolsAndPictographs
253 Emoticons InEmoticons
254 InTransportAndMapSymbols TransportAndMapSymbols
255 AlchemicalSymbols InAlchemicalSymbols
256 CJKUnifiedIdeographsExtensionB InCJKUnifiedIdeographsExtensionB
257 CJKUnifiedIdeographsExtensionC InCJKUnifiedIdeographsExtensionC
258 CJKUnifiedIdeographsExtensionD InCJKUnifiedIdeographsExtensionD
259 CJKCompatibilityIdeographsSupplement InCJKCompatibilityIdeographsSupplement
260 InTags Tags
261 InVariationSelectorsSupplement VariationSelectorsSupplement
262 InSupplementaryPrivateUseAreaA SupplementaryPrivateUseAreaA
263 InSupplementaryPrivateUseAreaB SupplementaryPrivateUseAreaB
264 IsUnknown Unknown
265 Common IsCommon
266 IsLatin Latin
267 Greek IsGreek
268 Cyrillic IsCyrillic
269 Armenian IsArmenian
270 Hebrew IsHebrew
271 Arabic IsArabic
272 IsSyriac Syriac
273 IsThaana Thaana
274 Devanagari IsDevanagari
275 Bengali IsBengali
276 Gurmukhi IsGurmukhi
277 Gujarati IsGujarati
278 IsOriya Oriya
279 IsTamil Tamil
280 IsTelugu Telugu
281 IsKannada Kannada
282 IsMalayalam Malayalam
283 IsSinhala Sinhala
284 IsThai Thai
285 IsLao Lao
286 IsTibetan Tibetan
287 IsMyanmar Myanmar
288 Georgian IsGeorgian
289 Hangul IsHangul
290 Ethiopic IsEthiopic
291 Cherokee IsCherokee
292 CanadianAboriginal IsCanadianAboriginal
293 IsOgham Ogham
294 IsRunic Runic
295 IsKhmer Khmer
296 IsMongolian Mongolian
297 Hiragana IsHiragana
298 IsKatakana Katakana
299 Bopomofo IsBopomofo
300 Han IsHan
301 IsYi Yi
302 IsOldItalic OldItalic
303 Gothic IsGothic
304 Deseret IsDeseret
305 Inherited IsInherited
306 IsTagalog Tagalog
307 Hanunoo IsHanunoo
308 Buhid IsBuhid
309 IsTagbanwa Tagbanwa
310 IsLimbu Limbu
311 IsTaiLe TaiLe
312 IsLinearB LinearB
313 IsUgaritic Ugaritic
314 IsShavian Shavian
315 IsOsmanya Osmanya
316 Cypriot IsCypriot
317 Braille IsBraille
318 Buginese IsBuginese
319 Coptic IsCoptic
320 IsNewTaiLue NewTaiLue
321 Glagolitic IsGlagolitic
322 IsTifinagh Tifinagh
323 IsSylotiNagri SylotiNagri
324 IsOldPersian OldPersian
325 IsKharoshthi Kharoshthi
326 Balinese IsBalinese
327 Cuneiform IsCuneiform
328 IsPhoenician Phoenician
329 IsPhagsPa PhagsPa
330 IsNko Nko
331 IsSundanese Sundanese
332 IsLepcha Lepcha
333 IsOlChiki OlChiki
334 IsVai Vai
335 IsSaurashtra Saurashtra
336 IsKayahLi KayahLi
337 IsRejang Rejang
338 IsLycian Lycian
339 Carian IsCarian
340 IsLydian Lydian
341 Cham IsCham
342 IsTaiTham TaiTham
343 IsTaiViet TaiViet
344 Avestan IsAvestan
345 EgyptianHieroglyphs IsEgyptianHieroglyphs
346 IsSamaritan Samaritan
347 IsLisu Lisu
348 Bamum IsBamum
349 IsJavanese Javanese
350 IsMeeteiMayek MeeteiMayek
351 ImperialAramaic IsImperialAramaic
352 IsOldSouthArabian OldSouthArabian
353 InscriptionalParthian IsInscriptionalParthian
354 InscriptionalPahlavi IsInscriptionalPahlavi
355 IsOldTurkic OldTurkic
356 IsKaithi Kaithi
357 Batak IsBatak
358 Brahmi IsBrahmi
359 IsMandaic Mandaic
"""

def _create_properties(definitions):
    "Creates the Unicode properties."
    # Normalise the names.
    definitions = definitions.upper().replace("_", "").replace("-", "")

    properties = {}
    for line in filter(None, definitions.splitlines()):
        fields = line.split()
        id, names = int(fields[0]), fields[1 : ]
        for name in names:
            properties[name] = id

    return properties

# Build the property dictionary.
_properties = _create_properties(_PROPERTIES)

def _compile_firstset(info, fs):
    "Compiles the firstset for the pattern."
    if not fs or _VOID_ITEM in fs or None in fs:
        # No firstset.
        return []
    characters, others = [], []
    for m in fs:
        t = type(m)
        if t is _Any:
            others.append(m)
        elif t is _Character and m.positive:
            characters.append(m.value)
        elif t is _Property:
            others.append(m)
        elif t is _CharacterIgn and m.positive:
            ch = info.char_type(m.value)
            characters.extend([m.value, ord(ch.lower()), ord(ch.upper()),
              ord(ch.title())])
        elif t is _Set and m.positive:
            characters.extend(m.characters)
            others.extend(m.others)
        else:
            return []
    if characters or others:
        fs = _Set(characters, others, zerowidth=True)
    else:
        # No firstset.
        return []
    fs = fs.optimise(info)
    if not fs:
        # No firstset.
        return []
    rev = bool(info.global_flags & REVERSE)
    # Compile the firstset.
    return fs.compile(rev)

def _count_ones(n):
    "Counts the number of set bits in an int."
    count = 0
    while n:
        count += 1
        n &= n - 1
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
    if len(branches) == 1:
        return branches[0]
    return _Branch(branches)

def _parse_sequence(source, info):
    "Parses a sequence, eg. 'abc'."
    sequence = []
    item = _parse_item(source, info)
    while item:
        sequence.append(item)
        item = _parse_item(source, info)
    if len(sequence) == 1:
        return sequence[0]
    return _Sequence(sequence)

def _PossessiveRepeat(element, min_count, max_count):
    return _Atomic(_GreedyRepeat(element, min_count, max_count))

def _parse_item(source, info):
    "Parses an item, which might be repeated. Returns None if there's no item."
    element = _parse_element(source, info)
    counts = _parse_quantifier(source, info)
    if not counts:
        # No quantifier.
        return element
    if not element or not element.can_repeat():
        raise error("nothing to repeat")
    min_count, max_count = counts
    here = source.pos
    ch = source.get()
    if ch == "?":
        # The "?" suffix that means it's a lazy repeat.
        repeated = _LazyRepeat
    elif ch == "+":
        # The "+" suffix that means it's a possessive repeat.
        repeated = _PossessiveRepeat
    else:
        # No suffix means that it's a greedy repeat.
        source.pos = here
        repeated = _GreedyRepeat
    if min_count == max_count == 1:
        # Only ever one repeat.
        return element
    return repeated(element, min_count, max_count)

def _parse_quantifier(source, info):
    "Parses a quantifier."
    here = source.pos
    ch = source.get()
    if ch == "?":
        # Optional element, eg. 'a?'.
        return 0, 1
    if ch == "*":
        # Repeated element, eg. 'a*'.
        return 0, None
    if ch == "+":
        # Repeated element, eg. 'a+'.
        return 1, None
    if ch == "{":
        # Looks like a limited repeated element, eg. 'a{2,3}'.
        min_count = _parse_count(source)
        ch = source.get()
        if ch == ",":
            max_count = _parse_count(source)
            if not source.match("}"):
                # Not a quantifier, so parse it later as a literal.
                source.pos = here
                return None
            # No minimum means 0 and no maximum means unlimited.
            min_count = int(min_count) if min_count else 0
            max_count = int(max_count) if max_count else None
            if max_count is not None and min_count > max_count:
                raise error("min repeat greater than max repeat")
            if min_count >= _UNLIMITED or max_count is not None and max_count \
              >= _UNLIMITED:
                raise error("repeat count too big")
            return min_count, max_count
        if ch == "}":
            if not min_count:
                # Not a quantifier, so parse it later as a literal.
                source.pos = here
                return None
            min_count = max_count = int(min_count)
            if min_count >= _UNLIMITED:
                raise error("repeat count too big")
            return min_count, max_count
    # No quantifier.
    source.pos = here
    return None

def _parse_count(source):
    "Parses a quantifier's count, which can be empty."
    count = []
    here = source.pos
    ch = source.get()
    while ch in _DIGITS:
        count.append(ch)
        here = source.pos
        ch = source.get()
    source.pos = here
    return "".join(count)

_SPECIAL_CHARS = set("()|?*+{^$.[\\#") | set([""])

def _parse_element(source, info):
    "Parses an element. An element might actually be a flag, eg. '(?i)'."
    while True:
        here = source.pos
        ch = source.get()
        if ch in _SPECIAL_CHARS:
            if ch in ")|":
                # The end of a sequence. At the end of the pattern ch is "".
                source.pos = here
                return None
            elif ch == "\\":
                # An escape sequence.
                return _parse_escape(source, info, False)
            elif ch == "(":
                # A parenthesised subpattern or a flag.
                element = _parse_paren(source, info)
                if element:
                    return element
            elif ch == ".":
                # Any character.
                if info.all_flags & DOTALL:
                    return _AnyAll()
                else:
                    return _Any()
            elif ch == "[":
                # A character set.
                return _parse_set(source, info)
            elif ch == "^":
                # The start of a line or the string.
                if info.all_flags & MULTILINE:
                    return _StartOfLine()
                else:
                    return _StartOfString()
            elif ch == "$":
                # The end of a line or the string.
                if info.all_flags & MULTILINE:
                    return _EndOfLine()
                else:
                    return _EndOfStringLine()
            elif ch == "{":
                # Looks like a limited quantifier.
                here2 = source.pos
                source.pos = here
                counts = _parse_quantifier(source, info)
                if counts:
                    # A quantifier where we expected an element.
                    raise error("nothing to repeat")
                # Not a quantifier, so it's a literal.
                source.pos = here2
                return _Character(ord(ch))
            elif ch in "?*+":
                # A quantifier where we expected an element.
                raise error("nothing to repeat")
            elif info.all_flags & VERBOSE:
                if ch == "#":
                    # A comment.
                    source.ignore_space = False
                    # Ignore characters until a newline or the end of the pattern.
                    while source.get() not in "\n":
                        pass
                    source.ignore_space = True
                else:
                    # A literal.
                    if info.all_flags & IGNORECASE:
                        return _CharacterIgn(ord(ch))
                    return _Character(ord(ch))
            else:
                # A literal.
                if info.all_flags & IGNORECASE:
                    return _CharacterIgn(ord(ch))
                return _Character(ord(ch))
        else:
            # A literal.
            if info.all_flags & IGNORECASE:
                return _CharacterIgn(ord(ch))
            return _Character(ord(ch))

def _parse_paren(source, info):
    "Parses a parenthesised subpattern or a flag."
    here = source.pos
    ch = source.get()
    if ch == "?":
        here2 = source.pos
        ch = source.get()
        if ch == "<":
            here3 = source.pos
            ch = source.get()
            if ch == "=":
                # Positive lookbehind.
                return _parse_lookaround(source, info, True, True)
            if ch == "!":
                # Negative lookbehind.
                return _parse_lookaround(source, info, True, False)
            # A named capture group.
            source.pos = here3
            name = _parse_name(source)
            group = info.new_group(name)
            source.expect(">")
            saved_scoped_flags = info.scoped_flags
            saved_ignore = source.ignore_space
            try:
                subpattern = _parse_pattern(source, info)
            finally:
                info.scoped_flags = saved_scoped_flags
                source.ignore_space = saved_ignore
            source.expect(")")
            info.close_group(group)
            return _Group(info, group, subpattern)
        if ch == "=":
            # Positive lookahead.
            return _parse_lookaround(source, info, False, True)
        if ch == "!":
            # Negative lookahead.
            return _parse_lookaround(source, info, False, False)
        if ch == "P":
            # A Python extension.
            return _parse_extension(source, info)
        if ch == "#":
            # A comment.
            return _parse_comment(source)
        if ch == "(":
            # A conditional subpattern.
            return _parse_conditional(source, info)
        if ch == ">":
            # An atomic subpattern.
            return _parse_atomic(source, info)
        if ch == "|":
            # A common groups branch.
            return _parse_common(source, info)
        # A flags subpattern.
        source.pos = here2
        return _parse_flags_subpattern(source, info)
    # An unnamed capture group.
    source.pos = here
    group = info.new_group()
    saved_scoped_flags = info.scoped_flags
    saved_ignore = source.ignore_space
    try:
        subpattern = _parse_pattern(source, info)
    finally:
        info.scoped_flags = saved_scoped_flags
        info.all_flags = info.global_flags | info.scoped_flags
        source.ignore_space = saved_ignore
    source.expect(")")
    info.close_group(group)
    return _Group(info, group, subpattern)

def _parse_extension(source, info):
    "Parses a Python extension."
    here = source.pos
    ch = source.get()
    if ch == "<":
        # A named capture group.
        name = _parse_name(source)
        group = info.new_group(name)
        source.expect(">")
        saved_scoped_flags = info.scoped_flags
        saved_ignore = source.ignore_space
        try:
            subpattern = _parse_pattern(source, info)
        finally:
            info.scoped_flags = saved_scoped_flags
            source.ignore_space = saved_ignore
        source.expect(")")
        info.close_group(group)
        return _Group(info, group, subpattern)
    if ch == "=":
        # A named group reference.
        name = _parse_name(source)
        source.expect(")")
        if info.is_open_group(name):
            raise error("can't refer to an open group")
        if info.all_flags & IGNORECASE:
            return _RefGroupIgn(info, name)
        return _RefGroup(info, name)
    source.pos = here
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
    saved_scoped_flags = info.scoped_flags
    saved_ignore = source.ignore_space
    try:
        subpattern = _parse_pattern(source, info)
    finally:
        info.scoped_flags = saved_scoped_flags
        source.ignore_space = saved_ignore
    source.expect(")")
    return _LookAround(behind, positive, subpattern)

def _parse_conditional(source, info):
    "Parses a conditional subpattern."
    saved_scoped_flags = info.scoped_flags
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
        info.scoped_flags = saved_scoped_flags
        source.ignore_space = saved_ignore
    source.expect(")")
    return _Conditional(info, group, yes_branch, no_branch)

def _parse_atomic(source, info):
    "Parses an atomic subpattern."
    saved_scoped_flags = info.scoped_flags
    saved_ignore = source.ignore_space
    try:
        subpattern = _parse_pattern(source, info)
    finally:
        info.scoped_flags = saved_scoped_flags
        source.ignore_space = saved_ignore
    source.expect(")")
    return _Atomic(subpattern)

def _parse_common(source, info):
    "Parses a common groups branch."
    # Capture group numbers in different branches can reuse the group numbers.
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
    if len(branches) == 1:
        return branches[0]
    return _Branch(branches)

def _parse_flags_subpattern(source, info):
    "Parses a flags subpattern."
    # It could be inline flags or a subpattern possibly with local flags.
    # Parse the flags.
    flags_on, flags_off = 0, 0
    try:
        while True:
            here = source.pos
            ch = source.get()
            flags_on |= _REGEX_FLAGS[ch]
    except KeyError:
        pass
    if ch == "-":
        try:
            while True:
                here = source.pos
                ch = source.get()
                flags_off |= _REGEX_FLAGS[ch]
        except KeyError:
            pass
        if not flags_off or (flags_off & _GLOBAL_FLAGS):
            error("bad inline flags")

    # Separate the global and scoped flags.
    source.pos = here
    old_global_flags = info.global_flags
    info.global_flags |= flags_on & _GLOBAL_FLAGS
    flags_on &= _SCOPED_FLAGS
    flags_off &= _SCOPED_FLAGS
    new_scoped_flags = (info.scoped_flags | flags_on) & ~flags_off
    saved_scoped_flags = info.scoped_flags
    saved_ignore = source.ignore_space
    info.scoped_flags = new_scoped_flags
    info.all_flags = info.global_flags | info.scoped_flags
    source.ignore_space = bool(info.all_flags & VERBOSE)
    if source.match(":"):
        # A subpattern with local flags.
        saved_global_flags = info.global_flags
        info.global_flags &= ~flags_off
        info.all_flags = info.global_flags | info.scoped_flags
        try:
            subpattern = _parse_pattern(source, info)
        finally:
            info.global_flags = saved_global_flags
            info.scoped_flags = saved_scoped_flags
            info.all_flags = info.global_flags | info.scoped_flags
            source.ignore_space = saved_ignore
        source.expect(")")
        return subpattern
    else:
        # Positional flags.
        if not source.match(")"):
            raise error("bad inline flags")
        new_behaviour = bool(info.global_flags & NEW)
        if not new_behaviour:
            # Old behaviour: positional flags are global and can only be turned
            # on.
            info.global_flags |= flags_on
        if info.global_flags & ~old_global_flags:
            # A global has been turned on, so reparse the pattern.
            if new_behaviour:
                # New behaviour: positional flags are scoped.
                info.global_flags &= _GLOBAL_FLAGS
            raise _UnscopedFlagSet(info.global_flags)
        info.all_flags = info.global_flags | info.scoped_flags
        return None

def _parse_name(source, allow_numeric=False):
    "Parses a name."
    saved_ignore = source.ignore_space
    source.ignore_space = False
    name = []
    here = source.pos
    ch = source.get()
    while ch in _ALNUM or ch == "_":
        name.append(ch)
        here = source.pos
        ch = source.get()
    source.pos = here
    source.ignore_space = saved_ignore
    name = "".join(name)
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
        here = source.pos
        try:
            return _parse_group_ref(source, info)
        except error:
            # Invalid as a group reference, so assume it's a literal.
            source.pos = here
            return _char_literal(info, in_set, ch)
    elif ch == "G" and not in_set:
        # A search anchor.
        return _SearchAnchor()
    elif ch == "N":
        # A named codepoint.
        return _parse_named_char(source, info, in_set)
    elif ch in "pP":
        # A Unicode property.
        return _parse_property(source, info, in_set, ch == "p")
    elif ch == "X" and not in_set:
        return _grapheme()
    elif ch in _ALPHA:
        # An alphabetic escape sequence.
        # Positional escapes aren't allowed inside a character set.
        if not in_set:
            if info.all_flags & WORD:
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
            return _Character(ord(value))
        return _char_literal(info, in_set, ch)
    elif ch in _DIGITS:
        # A numeric escape sequence.
        return _parse_numeric_escape(source, info, ch, in_set)
    else:
        # A literal.
        return _char_literal(info, in_set, ch)

def _char_literal(info, in_set, ch):
    "Creates a character literal, which might be in a set."
    if (info.all_flags & IGNORECASE) and not in_set:
        return _CharacterIgn(ord(ch))
    return _Character(ord(ch))

def _parse_numeric_escape(source, info, ch, in_set):
    "Parses a numeric escape sequence."
    if in_set or ch == "0":
        # Octal escape sequence, max 3 digits.
        return _parse_octal_escape(source, info, [ch], in_set)
    # At least 1 digit, so either octal escape or group.
    digits = ch
    here = source.pos
    ch = source.get()
    if ch in _DIGITS:
        # At least 2 digits, so either octal escape or group.
        digits += ch
        here = source.pos
        ch = source.get()
        if _is_octal(digits) and ch in _OCT_DIGITS:
            # 3 octal digits, so octal escape sequence.
            value = int(digits + ch, 8) & 0xFF
            if info.all_flags & IGNORECASE:
                return _CharacterIgn(value)
            return _Character(value)
        else:
            # 2 digits, so group.
            source.pos = here
            if info.is_open_group(digits):
                raise error("can't refer to an open group")
            if info.all_flags & IGNORECASE:
                return _RefGroupIgn(info, digits)
            return _RefGroup(info, digits)
    # 1 digit, so group.
    source.pos = here
    if info.is_open_group(digits):
        raise error("can't refer to an open group")
    if info.all_flags & IGNORECASE:
        return _RefGroupIgn(info, digits)
    return _RefGroup(info, digits)

def _parse_octal_escape(source, info, digits, in_set):
    "Parses an octal escape sequence."
    here = source.pos
    ch = source.get()
    while len(digits) < 3 and ch in _OCT_DIGITS:
        digits.append(ch)
        here = source.pos
        ch = source.get()
    source.pos = here
    try:
        value = int("".join(digits), 8) & 0xFF
        if (info.all_flags & IGNORECASE) and not in_set:
            return _CharacterIgn(value)
        return _Character(value)
    except ValueError:
        raise error("bad escape")

def _parse_hex_escape(source, info, max_len, in_set):
    "Parses a hex escape sequence."
    digits = []
    here = source.pos
    ch = source.get()
    while len(digits) < max_len and ch in _HEX_DIGITS:
        digits.append(ch)
        here = source.pos
        ch = source.get()
    if len(digits) != max_len:
        raise error("bad hex escape")
    source.pos = here
    value = int("".join(digits), 16)
    if (info.all_flags & IGNORECASE) and not in_set:
        return _CharacterIgn(value)
    return _Character(value)

def _parse_group_ref(source, info):
    "Parses a group reference."
    source.expect("<")
    name = _parse_name(source, True)
    source.expect(">")
    if info.is_open_group(name):
        raise error("can't refer to an open group")
    if info.all_flags & IGNORECASE:
        return _RefGroupIgn(info, name)
    return _RefGroup(info, name)

def _parse_named_char(source, info, in_set):
    "Parses a named character."
    here = source.pos
    ch = source.get()
    if ch == "{":
        name = []
        ch = source.get()
        while ch in _ALPHA or ch == " ":
            name.append(ch)
            ch = source.get()
        if ch == "}":
            try:
                value = unicodedata.lookup("".join(name))
                if (info.all_flags & IGNORECASE) and not in_set:
                    return _CharacterIgn(ord(value))
                return _Character(ord(value))
            except KeyError:
                raise error("undefined character name")
    source.pos = here
    return _char_literal(info, in_set, "N")

def _parse_property(source, info, in_set, positive):
    "Parses a Unicode property."
    here = source.pos
    ch = source.get()
    if ch == "{":
        name = []
        ch = source.get()
        while ch.isspace():
            ch = source.get()
        negate = ch == "^"
        if negate:
            ch = source.get()
        while ch and (ch.isalnum() or ch.isspace() or ch in "&_-."):
            name.append(ch)
            ch = source.get()
        if ch == "}":
            # The normalised name.
            norm_name = "".join(ch.upper() for ch in name if ch.isalnum())
            # The un-normalised name.
            name = "".join(name)
            value = _properties.get(norm_name)
            if value is not None:
                return _Property(value, positive=positive != negate)
            raise error("undefined property name")
    source.pos = here
    return _char_literal(info, in_set, "p" if positive else "P")

def _grapheme():
    "Returns a sequence that matches a grapheme."
    # To match a grapheme use \P{M}\p{M}*
    mod = _properties.get("M")
    return _Sequence([_Property(mod, positive=False),
      _GreedyRepeat(_Property(mod), 0, None)])

def _parse_set(source, info):
    "Parses a character set."
    # Negative character set?
    saved_ignore = source.ignore_space
    source.ignore_space = False
    negate = source.match("^")
    characters, others = [], []
    try:
        # Parse a set member. It might be a character or range of characters,
        # expanded to a list, or a property or character class.
        c, o = _parse_set_member(source, info)
        characters.extend(c)
        others.extend(o)
        while not source.match("]"):
            c, o = _parse_set_member(source, info)
            characters.extend(c)
            others.extend(o)
    finally:
        source.ignore_space = saved_ignore
    if info.all_flags & IGNORECASE:
        # Expand the list of characters to include all their case-forms.
        all_characters = []
        char_type = info.char_type
        for c in characters:
            ch = char_type(c)
            all_characters.extend([c, ord(ch.lower()), ord(ch.upper()),
              ord(ch.title())])
        characters = all_characters
    return _Set(characters, others, positive=not negate)

def _parse_set_member(source, info):
    "Parses a member in a character set."
    # Parse a character or property.
    start = _parse_set_item(source, info)
    if not isinstance(start, _Character):
        # Return only a property.
        return [], [start]
    if not source.match("-"):
        # Not a range, so return only a character.
        return [start.value], []
    # It looks like a range of characters.
    here = source.pos
    if source.match("]"):
        # We've reached the end of the set, so return both the character and
        # the hyphen.
        source.pos = here
        return [start.value, ord("-")], []
    # Parse a character or property.
    end = _parse_set_item(source, info)
    if not isinstance(end, _Character):
        # It's a property, so return the character, hyphen and property.
        return [start, ord("-")], [end]
    # It _is_ a range.
    if start.value > end.value:
        raise error("bad character range")
    # Expand the range to a list of characters.
    characters = range(start.value, end.value + 1)
    # Return the characters.
    return characters, []

def _parse_set_item(source, info):
    "Parses an item in a character set."
    if source.match("[:"):
        return _parse_character_class(source, not source.match("^"), info)
    if source.match("\\"):
        return _parse_escape(source, info, True)
    ch = source.get()
    if not ch:
        raise error("bad set")
    return _Character(ord(ch))

def _parse_character_class(source, positive, info):
    "Parses a POSIX character class."
    name = _parse_name(source)
    source.expect(":]")
    value = _properties.get(name.upper())
    if value is None:
        raise error("undefined character class name")
    return _Property(value, positive=positive)

def _compile_repl_escape(source, pattern):
    "Compiles a replacement template escape sequence."
    ch = source.get()
    if ch in _ALPHA:
        # An alphabetic escape sequence.
        value = _CHARACTER_ESCAPES.get(ch)
        if value:
            return False, [ord(value)]
        if ch == "g":
            # A group preference.
            return True, [_compile_repl_group(source, pattern)]
        return False, [ord("\\"), ord(ch)]
    if ch == "0":
        # An octal escape sequence.
        digits = ch
        while len(digits) < 3:
            here = source.pos
            ch = source.get()
            if ch not in _OCT_DIGITS:
                source.pos = here
                break
            digits += ch
        return False, [int(digits, 8) & 0xFF]
    if ch in _DIGITS:
        # Either an octal escape sequence (3 digits) or a group reference (max
        # 2 digits).
        digits = ch
        here = source.pos
        ch = source.get()
        if ch in _DIGITS:
            digits += ch
            here = source.pos
            ch = source.get()
            if ch and _is_octal(digits + ch):
                # An octal escape sequence.
                return False, [int(digits + ch, 8) & 0xFF]
        # A group reference.
        source.pos = here
        return True, [int(digits)]
    if ch == "\\":
        # An escaped backslash is a backslash.
        return False, [ord("\\")]
    # An escaped non-backslash is a backslash followed by the literal.
    return False, [ord("\\"), ord(ch)]

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
    try:
        return pattern.groupindex[name]
    except KeyError:
        raise IndexError("unknown group")

# The regular expression is parsed into a syntax tree. The different types of
# node are defined below.

_INDENT = "  "
_ZEROWIDTH_OP = 0x2

_VOID_ITEM = object()

# Common base for all nodes.
class _RegexBase(object):
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
    def __ne__(self, other):
        return not self.__eq__(other)

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
        print "%s%s" % (_INDENT * indent, self._op_name[reverse])
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
        sequence = self.subpattern.optimise(info)
        prefix, sequence = _Atomic._split_atomic_prefix(sequence)
        suffix, sequence = _Atomic._split_atomic_suffix(sequence)
        # Is there anything left in the atomic sequence?
        if sequence.is_empty():
            sequence = []
        else:
            sequence = [_Atomic(sequence)]
        sequence = prefix + sequence + suffix
        if len(sequence) == 1:
            return sequence[0]
        return _Sequence(sequence)
    def pack_characters(self):
        self.subpattern = self.subpattern.pack_characters()
        return self
    def is_empty(self):
        return self.subpattern.is_empty()
    def contains_group(self):
        return self.subpattern.contains_group()
    def compile(self, reverse=False):
        return [(_OP.ATOMIC, )] + self.subpattern.compile(reverse) + [(_OP.END,
          )]
    def dump(self, indent=0, reverse=False):
        print "%s%s" % (_INDENT * indent, "ATOMIC")
        self.subpattern.dump(indent + 1, reverse)
    def firstset(self):
        return self.subpattern.firstset()
    def has_simple_start(self):
        return self.subpattern.has_simple_start()
    def __eq__(self, other):
        return type(self) is type(other) and self.subpattern == other.subpattern
    @staticmethod
    def _split_atomic_prefix(sequence):
        # Leading atomic items can be moved out of an atomic sequence.
        prefix = []
        while True:
            item = sequence.get_first()
            if not item or not item.is_atomic():
                break
            prefix.append(item)
            sequence = sequence.drop_first()
        return prefix, sequence
    @staticmethod
    def _split_atomic_suffix(sequence):
        # Trailing atomic items can be moved out of an atomic sequence.
        suffix = []
        while True:
            item = sequence.get_last()
            if not item or not item.is_atomic():
                break
            suffix.append(item)
            sequence = sequence.drop_last()
        return list(reversed(suffix)), sequence

class _Boundary(_ZeroWidthBase):
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, positive=True):
        self.positive = bool(positive)
        self._key = self.__class__, self.positive
    def compile(self, reverse=False):
        return [(_OP.BOUNDARY, int(self.positive))]
    def dump(self, indent=0, reverse=False):
        print "%sBOUNDARY %s" % (_INDENT * indent,
          self._pos_text[self.positive])

class _Branch(_StructureBase):
    def __init__(self, branches):
        self.branches = branches
    def fix_groups(self):
        for branch in self.branches:
            branch.fix_groups()
    def optimise(self, info):
        branches = _Branch._flatten_branches(info, self.branches)
        prefix, branches = _Branch._split_common_prefix(branches)
        suffix, branches = _Branch._split_common_suffix(branches)
        branches = _Branch._merge_character_prefixes(info, branches)
        branches = _Branch._reduce_to_set(info, branches)
        if len(branches) > 1:
            branches = [_Branch(branches)]
        elif len(branches) == 1:
            branches = [branches[0]]
        sequence = prefix + branches + suffix
        if len(sequence) == 1:
            return sequence[0]
        return _Sequence(sequence)
    def pack_characters(self):
        self.branches = [branch.pack_characters() for branch in self.branches]
        return self
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
        print "%sBRANCH" % (_INDENT * indent)
        self.branches[0].dump(indent + 1, reverse)
        for branch in self.branches[1 : ]:
            print "%sOR" % (_INDENT * indent)
            branch.dump(indent + 1, reverse)
    def firstset(self):
        fs = set()
        for branch in self.branches:
            fs |= branch.firstset()
        return fs or set([None])
    def __eq__(self, other):
        return type(self) is type(other) and self.branches == other.branches
    @staticmethod
    def _flatten_branches(info, branches):
        # Flatten the branches so that there aren't branches of branches.
        new_branches = []
        for branch in branches:
            branch = branch.optimise(info)
            if isinstance(branch, _Branch):
                new_branches.extend(branch.branches)
            else:
                new_branches.append(branch)
        return new_branches
    @staticmethod
    def _split_common_prefix(branches):
        # Common leading items can be moved out of the branches.
        prefix = []
        while True:
            item = branches[0].get_first()
            if not item:
                break
            if any(branch.get_first() != item for branch in branches[1 : ]):
                break
            prefix.append(item)
            branches = [branch.drop_first() for branch in branches]
        return prefix, branches
    @staticmethod
    def _split_common_suffix(branches):
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
        return list(reversed(suffix)), branches
    @staticmethod
    def _merge_character_prefixes(info, branches):
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
                    if char_prefixes:
                        _Branch._flush_char_prefix(info, char_type,
                          char_prefixes, order, new_branches)
                        char_prefixes.clear()
                        order.clear()
                    char_type = type(first)
                char_prefixes[first.value].append(branch)
                order.setdefault(first.value, len(order))
            else:
                if char_prefixes:
                    _Branch._flush_char_prefix(info, char_type, char_prefixes,
                      order, new_branches)
                    char_prefixes.clear()
                    order.clear()
                char_type = None
                new_branches.append(branch)
        if char_prefixes:
            _Branch._flush_char_prefix(info, char_type, char_prefixes, order,
              new_branches)
        return new_branches
    @staticmethod
    def _reduce_to_set(info, branches):
        # Can the branches be reduced to a set?
        new_branches = []
        characters, others = [], []
        for branch in branches:
            t = type(branch)
            if t is _Any:
                others.append(branch)
            elif t is _Character and branch.positive:
                characters.append(branch.value)
            elif isinstance(branch, _Property):
                others.append(branch)
            elif t is _Set and branch.positive:
                characters.extend(branch.characters)
                others.extend(branch.others)
            else:
                _Branch._flush_set_members(info, characters, others,
                  new_branches)
                characters, others = [], []
                new_branches.append(branch)
        _Branch._flush_set_members(info, characters, others, new_branches)
        return new_branches
    @staticmethod
    def _flush_char_prefix(info, char_type, prefixed, order, new_branches):
        for value, branches in sorted(prefixed.items(), key=lambda pair:
          order[pair[0]]):
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
    def _flush_set_members(info, characters, others, new_branches):
        if characters or others:
            new_branches.append(_Set(characters, others,
              positive=True).optimise(info))

class _Character(_RegexBase):
    _opcode = {False: _OP.CHARACTER, True: _OP.CHARACTER_REV}
    _op_name = {False: "CHARACTER", True: "CHARACTER_REV"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, ch, positive=True, zerowidth=False):
        self.value, self.positive, self.zerowidth = ch, bool(positive), \
          bool(zerowidth)
        self._key = self.__class__, self.value, self.positive, self.zerowidth
    def compile(self, reverse=False):
        flags = int(self.positive) + _ZEROWIDTH_OP * int(self.zerowidth)
        return [(self._opcode[reverse], flags, self.value)]
    def dump(self, indent=0, reverse=False):
        print "%s%s %s %s" % (_INDENT * indent, self._op_name[reverse],
          self._pos_text[self.positive], self.value)
    def firstset(self):
        try:
            return set([self])
        except TypeError:
            print("_key of character is {}".format(repr(self._key)))
            print("Hash value is {}".format(hash(self._key)))
            raise
    def has_simple_start(self):
        return True
    def is_case_sensitive(self, info):
        char_type = info.char_type
        return char_type(self.value).lower() != char_type(self.value).upper()

class _CharacterIgn(_Character):
    _opcode = {False: _OP.CHARACTER_IGN, True: _OP.CHARACTER_IGN_REV}
    _op_name = {False: "CHARACTER_IGN", True: "CHARACTER_IGN_REV"}
    def optimise(self, info):
        # Case-sensitive matches are faster, so convert to a case-sensitive
        # instance if the character is case-insensitive.
        if self.is_case_sensitive(info):
            return self
        return _Character(self.value, positive=self.positive,
          zerowidth=self.zerowidth)

class _Conditional(_StructureBase):
    def __init__(self, info, group, yes_item, no_item):
        self.info, self.group, self.yes_item, self.no_item = info, group, \
          yes_item, no_item
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
        self.yes_item = self.yes_item.optimise(info)
        self.no_item = self.no_item.optimise(info)
        return self
    def pack_characters(self):
        self.yes_item = self.yes_item.pack_characters()
        self.no_item = self.no_item.pack_characters()
        return self
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
        print "%sGROUP_EXISTS %s" % (_INDENT * indent, self.group)
        self.yes_item.dump(indent + 1, reverse)
        if self.no_item:
            print "%sOR" % (_INDENT * indent)
            self.no_item.dump(indent + 1, reverse)
    def firstset(self):
        return self.yes_item.firstset() | self.no_item.firstset()
    def __eq__(self, other):
        return type(self) is type(other) and (self.group, self.yes_item,
          self.no_item) == (other.group, other.yes_item, other.no_item)

class _DefaultBoundary(_ZeroWidthBase):
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, positive=True):
        self.positive = bool(positive)
        self._key = self.__class__, self.positive
    def compile(self, reverse=False):
        return [(_OP.DEFAULT_BOUNDARY, int(self.positive))]
    def dump(self, indent=0, reverse=False):
        print "%sDEFAULT_BOUNDARY %s" % (_INDENT * indent,
          self._pos_text[self.positive])

class _EndOfLine(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.END_OF_LINE, )]
    def dump(self, indent=0, reverse=False):
        print "%sEND_OF_LINE" % (_INDENT * indent)

class _EndOfString(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.END_OF_STRING, )]
    def dump(self, indent=0, reverse=False):
        print "%sEND_OF_STRING" % (_INDENT * indent)

class _EndOfStringLine(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.END_OF_STRING_LINE, )]
    def dump(self, indent=0, reverse=False):
        print "%sEND_OF_STRING_LINE" % (_INDENT * indent)

class _GreedyRepeat(_StructureBase):
    _opcode = _OP.GREEDY_REPEAT
    _op_name = "GREEDY_REPEAT"
    def __init__(self, subpattern, min_count, max_count):
        self.subpattern, self.min_count, self.max_count = subpattern, \
          min_count, max_count
    def fix_groups(self):
        self.subpattern.fix_groups()
    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)
        if (self.min_count, self.max_count) == (1, 1) or subpattern.is_empty():
            return subpattern
        self.subpattern = subpattern
        return self
    def pack_characters(self):
        self.subpattern = self.subpattern.pack_characters()
        return self
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
        return [tuple(repeat)] + self.subpattern.compile(reverse) + [(_OP.END,
          )]
    def remove_captures(self):
        self.subpattern = self.subpattern.remove_captures()
        return self
    def dump(self, indent=0, reverse=False):
        if self.max_count is None:
            print "%s%s %s INF" % (_INDENT * indent, self._op_name,
              self.min_count)
        else:
            print "%s%s %s %s" % (_INDENT * indent, self._op_name,
              self.min_count, self.max_count)
        self.subpattern.dump(indent + 1, reverse)
    def firstset(self):
        fs = self.subpattern.firstset()
        if self.min_count == 0:
            fs.add(None)
        return fs or set([None])
    def __eq__(self, other):
        return type(self) is type(other) and (self.subpattern, self.min_count,
          self.max_count) == (other.subpattern, other.min_count,
          other.max_count)

class _Group(_StructureBase):
    def __init__(self, info, group, subpattern):
        self.info, self.group, self.subpattern = info, group, subpattern
    def fix_groups(self):
        self.subpattern.fix_groups()
    def optimise(self, info):
        self.subpattern = self.subpattern.optimise(info)
        return self
    def pack_characters(self):
        self.subpattern = self.subpattern.pack_characters()
        return self
    def is_empty(self):
        return self.subpattern.is_empty()
    def is_atomic(self):
        return self.subpattern.is_atomic()
    def contains_group(self):
        return True
    def compile(self, reverse=False):
        return [(_OP.GROUP, self.group)] + self.subpattern.compile(reverse) + \
          [(_OP.END, )]
    def remove_captures(self):
        return self.subpattern.remove_captures()
    def dump(self, indent=0, reverse=False):
        print "%sGROUP %s" % (_INDENT * indent, self.group)
        self.subpattern.dump(indent + 1, reverse)
    def firstset(self):
        return self.subpattern.firstset()
    def has_simple_start(self):
        return self.subpattern.has_simple_start()
    def __eq__(self, other):
        return type(self) is type(other) and (self.group, self.subpattern) == \
          (other.group, other.subpattern)

class _LazyRepeat(_GreedyRepeat):
    _opcode = _OP.LAZY_REPEAT
    _op_name = "LAZY_REPEAT"

class _LookAround(_StructureBase):
    _dir_text = {False: "AHEAD", True: "BEHIND"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, behind, positive, subpattern):
        self.behind, self.positive, self.subpattern = bool(behind), \
          bool(positive), subpattern
    def fix_groups(self):
        self.subpattern.fix_groups()
    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)
        if self.positive and subpattern.is_empty():
            return subpattern
        self.subpattern = subpattern
        return self
    def pack_characters(self):
        self.subpattern = self.subpattern.pack_characters()
        return self
    def is_empty(self):
        return self.subpattern.is_empty()
    def is_atomic(self):
        return self.subpattern.is_atomic()
    def contains_group(self):
        return self.subpattern.contains_group()
    def compile(self, reverse=False):
        return [(_OP.LOOKAROUND, int(self.positive), int(not self.behind))] + \
          self.subpattern.compile(self.behind) + [(_OP.END, )]
    def dump(self, indent=0, reverse=False):
        print "%sLOOKAROUND %s %s" % (_INDENT * indent,
          self._dir_text[self.behind], self._pos_text[self.positive])
        self.subpattern.dump(indent + 1, self.behind)
    def firstset(self):
        return set([None])
    def __eq__(self, other):
        return type(self) is type(other) and (self.behind, self.positive,
          self.subpattern) == (other.behind, other.positive, other.subpattern)
    def can_repeat(self):
        return False

class _Property(_RegexBase):
    _opcode = {False: _OP.PROPERTY, True: _OP.PROPERTY_REV}
    _op_name = {False: "PROPERTY", True: "PROPERTY_REV"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    def __init__(self, value, positive=True, zerowidth=False):
        self.value, self.positive, self.zerowidth = value, bool(positive), \
          bool(zerowidth)
        self._key = self.__class__, self.value, self.positive, self.zerowidth
    def compile(self, reverse=False):
        flags = int(self.positive) + _ZEROWIDTH_OP * int(self.zerowidth)
        return [(self._opcode[reverse], flags, self.value)]
    def dump(self, indent=0, reverse=False):
        print "%s%s %s %s" % (_INDENT * indent, self._op_name[reverse],
          self._pos_text[self.positive], self.value)
    def firstset(self):
        try:
            return set([self])
        except TypeError:
            print("_key of property is {}".format(self._key))
            raise
    def has_simple_start(self):
        return True
    def is_case_sensitive(self, info):
        return True

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
        print "%s%s %s" % (_INDENT * indent, self._op_name[reverse],
          self.group)

class _RefGroupIgn(_RefGroup):
    _opcode = {False: _OP.REF_GROUP_IGN, True: _OP.REF_GROUP_IGN_REV}
    _op_name = {False: "REF_GROUP_IGN", True: "REF_GROUP_IGN_REV"}

class _SearchAnchor(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.SEARCH_ANCHOR, )]
    def dump(self, indent=0, reverse=False):
        print "%sSEARCH_ANCHOR" % (_INDENT * indent)

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
        self.sequence = sequence
        return self
    def pack_characters(self):
        sequence = []
        char_type, characters = _Character, []
        for subpattern in self.sequence:
            if type(subpattern) is char_type and subpattern.positive:
                characters.append(subpattern.value)
            else:
                if characters:
                    _Sequence._flush_characters(char_type, characters, sequence)
                    characters = []
                if type(subpattern) in _all_char_types and subpattern.positive:
                    char_type = type(subpattern)
                    characters.append(subpattern.value)
                else:
                    sequence.append(subpattern.pack_characters())
        if characters:
            _Sequence._flush_characters(char_type, characters, sequence)
        if len(sequence) == 1:
            return sequence[0]
        self.sequence = sequence
        return self
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
            seq = list(reversed(self.sequence))
        else:
            seq = self.sequence
        code = []
        for subpattern in seq:
            code.extend(subpattern.compile(reverse))
        return code
    def remove_captures(self):
        self.sequence = [subpattern.remove_captures() for subpattern in
          self.sequence]
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
    def __eq__(self, other):
        return type(self) is type(other) and self.sequence == other.sequence
    @staticmethod
    def _flush_characters(char_type, characters, sequence):
        if not characters:
            return
        if len(characters) == 1:
            sequence.append(char_type(characters[0]))
        else:
            sequence.append(_string_classes[char_type](characters))

class _Set(_RegexBase):
    _opcode = {False: _OP.SET, True: _OP.SET_REV}
    _op_name = {False: "SET", True: "SET_REV"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    _big_bitset_opcode = {False: _OP.BIG_BITSET, True: _OP.BIG_BITSET_REV}
    _small_bitset_opcode = {False: _OP.SMALL_BITSET, True: _OP.SMALL_BITSET_REV}
    def __init__(self, characters, others, positive=True, zerowidth=False):
        characters = tuple(set(characters))
        others = tuple(set(others))
        self.characters, self.others, self.positive, self.zerowidth = \
          characters, others, bool(positive), zerowidth
        self._key = self.__class__, self.characters, self.others, \
          self.positive, self.zerowidth
    def optimise(self, info):
        # If the set contains complementary properties then the set itself is
        # equivalent to _AnyAll.
        prop = {False: set(), True: set()}
        for o in self.others:
            if isinstance(o, _Property):
                prop[o.positive].add(o.value)
        if self.positive and (prop[False] & prop[True]) and not self.zerowidth:
            return _AnyAll()

        # We can simplify if the set contains just a single character.
        if not self.others and len(self.characters) == 1:
            return _Character(self.characters[0], positive=self.positive,
              zerowidth=self.zerowidth)

        # We can simplify if the set contains just a single property (with
        # certain exceptions).
        if not self.characters and len(self.others) == 1:
            o = self.others[0]
            if type(o) is _Any and self.positive and not self.zerowidth:
                return o
            if isinstance(o, _Property):
                return _Property(o.value, positive=self.positive == o.positive,
                  zerowidth=self.zerowidth)

        return self
    def compile(self, reverse=False):
        # If there are only characters then compile to a character or bitset.
        if not self.others:
            if len(self.characters) == 1:
                return _Character(self.characters[0],
                  positive=self.positive).compile(reverse)
            return self._make_bitset(self.characters, self.positive, reverse)

        # If there's one property and no characters then compile to that.
        if not self.characters and len(self.others) == 1:
            o = self.others[0]
            o.positive = o.positive == self.positive
            o.zerowidth = self.zerowidth
            return o.compile(reverse)

        # Compile a compound set.
        flags = int(self.positive) + _ZEROWIDTH_OP * int(self.zerowidth)
        code = [(self._opcode[reverse], flags)]
        if self.characters:
            code.extend(self._make_bitset(self.characters, True, False))
        for o in self.others:
            code.extend(o.compile())
        code.append((_OP.END, ))
        return code
    def dump(self, indent=0, reverse=False):
        print "%s%s %s" % (_INDENT * indent, self._op_name[reverse],
          self._pos_text[self.positive])
        if self.characters:
            characters = sorted(self.characters)
            c = characters[0]
            start, end = c, c - 1
            for c in characters:
                if c > end + 1:
                    if start == end:
                        print "%sCHARACTER %s" % (_INDENT * (indent + 1), start)
                    else:
                        print "%sRANGE %s %s" % (_INDENT * (indent + 1), start,
                          end)
                    start = c
                end = c
            if start == end:
                print "%sCHARACTER %s" % (_INDENT * (indent + 1), start)
            else:
                print "%sRANGE %s %s" % (_INDENT * (indent + 1), start, end)
        for o in self.others:
            o.dump(indent + 1)
    def firstset(self):
        try:
            return set([self])
        except TypeError:
            print("_key of set is {}".format(self._key))
            raise
    def has_simple_start(self):
        return True
    BITS_PER_INDEX = 16
    INDEXES_PER_CODE = _BITS_PER_CODE // BITS_PER_INDEX
    CODE_MASK = (1 << _BITS_PER_CODE) - 1
    CODES_PER_SUBSET = 256 // _BITS_PER_CODE
    SUBSET_MASK = (1 << 256) - 1
    def _make_bitset(self, characters, positive, reverse):
        code = []
        # values for big bitset are: max_char indexes... subsets...
        # values for small bitset are: top_bits bitset
        bitset_dict = defaultdict(int)
        for c in characters:
            bitset_dict[c >> 8] |= 1 << (c & 0xFF)
        if len(bitset_dict) > 1:
            # Build a big bitset.
            indexes = []
            subset_index = {}
            for top in range(max(bitset_dict.keys()) + 1):
                subset = bitset_dict.get(top, 0)
                ind = subset_index.setdefault(subset, len(subset_index))
                indexes.append(ind)
            remainder = len(indexes) % self.INDEXES_PER_CODE
            if remainder:
                indexes.extend([0] * (self.INDEXES_PER_CODE - remainder))
            data = []
            for i in range(0, len(indexes), self.INDEXES_PER_CODE):
                ind = 0
                for s in range(self.INDEXES_PER_CODE):
                    ind |= indexes[i + s] << (_Set.BITS_PER_INDEX * s)
                data.append(ind)
            for subset, ind in sorted(subset_index.items(), key=lambda pair:
              pair[1]):
                data.extend(_Set._bitset_to_codes(subset))
            flags = int(positive) + _ZEROWIDTH_OP * int(self.zerowidth)
            code.append((self._big_bitset_opcode[reverse], flags,
              max(characters)) + tuple(data))
        else:
            # Build a small bitset.
            flags = int(positive) + _ZEROWIDTH_OP * int(self.zerowidth)
            top_bits, bitset = bitset_dict.items()[0]
            code.append((self._small_bitset_opcode[reverse], flags, top_bits) +
              tuple(_Set._bitset_to_codes(bitset)))
        return code
    @staticmethod
    def _bitset_to_codes(bitset):
        codes = []
        for i in range(_Set.CODES_PER_SUBSET):
            codes.append(bitset & _Set.CODE_MASK)
            bitset >>= _BITS_PER_CODE
        return codes

class _StartOfLine(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.START_OF_LINE, )]
    def dump(self, indent=0, reverse=False):
        print "%sSTART_OF_LINE" % (_INDENT * indent)

class _StartOfString(_ZeroWidthBase):
    def compile(self, reverse=False):
        return [(_OP.START_OF_STRING, )]
    def dump(self, indent=0, reverse=False):
        print "%sSTART_OF_STRING" % (_INDENT * indent)

class _String(_RegexBase):
    _opcode = {False: _OP.STRING, True: _OP.STRING_REV}
    _op_name = {False: "STRING", True: "STRING_REV"}
    def __init__(self, characters):
        self.characters = characters
        self._key = self.__class__, self.characters
    def compile(self, reverse=False):
        return [(self._opcode[reverse], len(self.characters)) +
          tuple(self.characters)]
    def dump(self, indent=0, reverse=False):
        print "%s%s %s" % (_INDENT * indent, self._op_name[reverse],
          " ".join(map(str, self.characters)))
    def firstset(self):
        return set([_Character(self.characters[0])])
    def has_simple_start(self):
        return True
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
    "a": "\a",
    "b": "\b",
    "f": "\f",
    "n": "\n",
    "r": "\r",
    "t": "\t",
    "v": "\v",
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

class _Source(object):
    "Scanner for the regular expression source string."
    def __init__(self, string):
        if isinstance(string, unicode):
            self.string = string
            self.char_type = unichr
        else:
            self.string = string
            self.char_type = chr
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
            return self.string[ : 0]
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
            raise error("missing %s" % substring)
    def at_end(self):
        pos = self.pos
        try:
            if self.ignore_space:
                while self.string[pos].isspace():
                    pos += 1
            return pos >= len(self.string)
        except IndexError:
            return True

class _Info(object):
    "Info about the regular expression."
    OPEN = "OPEN"
    CLOSED = "CLOSED"
    def __init__(self, flags=0, char_type=None):
        self.global_flags = flags & _GLOBAL_FLAGS
        self.scoped_flags = flags & _SCOPED_FLAGS
        self.all_flags = self.global_flags | self.scoped_flags
        if not (self.global_flags & NEW):
            self.global_flags = self.all_flags
        self.group_count = 0
        self.group_index = {}
        self.group_name = {}
        self.used_groups = set()
        self.group_state = {}
        self.char_type = char_type
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
        self.group_state[group] = self.OPEN
        return group
    def close_group(self, group):
        self.group_state[group] = self.CLOSED
    def is_open_group(self, name):
        if name.isdigit():
            group = int(name)
        else:
            group = self.group_index.get(name)
        return self.group_state.get(group) == self.OPEN

class Scanner:
    def __init__(self, lexicon, flags=0):
        self.lexicon = lexicon

        # Combine phrases into a compound pattern.
        patterns = []
        for phrase, action in lexicon:
            # Parse the regular expression.
            source = _Source(phrase)
            info = _Info(flags, source.char_type)
            source.ignore_space = bool(info.all_flags & VERBOSE)
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
        parsed = parsed.pack_characters()

        reverse = bool(info.global_flags & REVERSE)

        # Compile the compound pattern. The result is a list of tuples.
        code = parsed.compile(reverse) + [(_OP.SUCCESS, )]
        if parsed.has_simple_start():
            fs_code = []
        else:
            fs_code = _compile_firstset(info, parsed.firstset())
        fs_code = _flatten_code(fs_code)

        # Flatten the code into a list of ints.
        code = _flatten_code(code)

        code = fs_code + code

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
