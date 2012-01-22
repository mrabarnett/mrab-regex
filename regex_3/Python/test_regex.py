import regex
import string
import sys
import unicodedata
from weakref import proxy

class Test:
    MATCH_CLASS = "<class '_regex.Match'>"
    PATTERN_CLASS = "<class '_regex.Pattern'>"
    FLAGS_WITH_COMPILED_PAT = """ValueError("can't process flags argument with a compiled pattern",)"""
    INVALID_GROUP_REF = "error('invalid group reference',)"
    MISSING_GT = "error('missing >',)"
    BAD_GROUP_NAME = "error('bad group name',)"
    MISSING_LT = "error('missing <',)"
    UNKNOWN_GROUP_I = "IndexError('unknown group',)"
    UNKNOWN_GROUP = "error('unknown group',)"
    BAD_ESCAPE = "error('bad escape',)"
    BAD_OCTAL_ESCAPE = "error('bad octal escape',)"
    BAD_SET = "error('bad set',)"
    STR_PAT_ON_BYTES = """TypeError("can't use a string pattern on a bytes-like object",)"""
    BYTES_PAT_ON_STR = """TypeError("can't use a bytes pattern on a string-like object",)"""
    STR_PAT_BYTES_TEMPL = """TypeError('sequence item 0: expected str instance, bytes found',)"""
    BYTES_PAT_STR_TEMPL = """TypeError('sequence item 0: expected bytes, str found',)"""
    BYTES_PAT_UNI_FLAG = """ValueError("can't use UNICODE flag with a bytes pattern",)"""
    MIXED_FLAGS = """ValueError('ASCII, LOCALE and UNICODE flags are mutually incompatible',)"""
    SYNTAX_ERROR = "SYNTAX_ERROR"
    MISSING_RPAREN = "error('missing )',)"
    TRAILING_CHARS = "error('trailing characters in pattern',)"
    BAD_CHAR_RANGE = "error('bad character range',)"
    NOTHING_TO_REPEAT = "error('nothing to repeat',)"
    OPEN_GROUP = """error("can't refer to an open group",)"""
    DUPLICATE_GROUP = "error('duplicate group',)"
    CANT_TURN_OFF = """error("bad inline flags: can't turn flags off",)"""

    def __init__(self):
        pass

    def expect(self, code, expected_result):
        self.index += 1
        try:
            actual_result = ascii(code())
        except Exception as e:
            actual_result = ascii(e)
        if actual_result != expected_result:
            self.record_failure("Expected {} but got {} instead".format(expected_result,
              actual_result))

    def record_failure(self, message="Failed"):
        self.failed_tests.append((self.performing, self.index, message))

    def test_weakref(self):
        self.index += 1
        s = 'QabbbcR'
        x = regex.compile('ab+c')
        y = proxy(x)
        if x.findall('QabbbcR') != y.findall('QabbbcR'):
            self.record_failure()

    def test_search_star_plus(self):
        # 1..10
        self.expect(lambda: regex.search('a*', 'xxx').span(0), ascii((0, 0)))
        self.expect(lambda: regex.search('x*', 'axx').span(), ascii((0, 0)))
        self.expect(lambda: regex.search('x+', 'axx').span(0), ascii((1, 3)))
        self.expect(lambda: regex.search('x+', 'axx').span(), ascii((1, 3)))
        self.expect(lambda: regex.search('x', 'aaa'), ascii(None))
        self.expect(lambda: regex.match('a*', 'xxx').span(0), ascii((0, 0)))
        self.expect(lambda: regex.match('a*', 'xxx').span(), ascii((0, 0)))
        self.expect(lambda: regex.match('x*', 'xxxa').span(0), ascii((0, 3)))
        self.expect(lambda: regex.match('x*', 'xxxa').span(), ascii((0, 3)))
        self.expect(lambda: regex.match('a+', 'xxx'), ascii(None))

    def bump_num(self, matchobj):
        int_value = int(matchobj[0])
        return str(int_value + 1)

    def test_basic_regex_sub(self):
        # 1..3
        self.expect(lambda: regex.sub("(?i)b+", "x", "bbbb BBBB"), ascii('x x'))
        self.expect(lambda: regex.sub(r'\d+', self.bump_num, '08.2 -2 23x99y'),
          ascii('9.3 -3 24x100y'))
        self.expect(lambda: regex.sub(r'\d+', self.bump_num, '08.2 -2 23x99y',
          3), ascii('9.3 -3 23x99y'))

        # 4..5
        self.expect(lambda: regex.sub('.', lambda m: r"\n", 'x'), ascii("\\n"))
        self.expect(lambda: regex.sub('.', r"\n", 'x'), ascii("\n"))

        # 6..9
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a>\g<a>', 'xx'),
          ascii('xxxx'))
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a>\g<1>', 'xx'),
          ascii('xxxx'))
        self.expect(lambda: regex.sub('(?P<unk>x)', r'\g<unk>\g<unk>', 'xx'),
          ascii('xxxx'))
        self.expect(lambda: regex.sub('(?P<unk>x)', r'\g<1>\g<1>', 'xx'),
          ascii('xxxx'))

        # 10..12
        self.expect(lambda: regex.sub('a',
          r'\t\n\v\r\f\a\b\B\Z\a\A\w\W\s\S\d\D', 'a'),
          ascii("\t\n\v\r\f\a\b\\B\\Z\a\\A\\w\\W\\s\\S\\d\\D"))
        self.expect(lambda: regex.sub('a', '\t\n\v\r\f\a', 'a'),
          ascii("\t\n\v\r\f\a"))
        self.expect(lambda: regex.sub('a', '\t\n\v\r\f\a', 'a'), ascii(chr(9) +
          chr(10) + chr(11) + chr(13) + chr(12) + chr(7)))

        # 13
        self.expect(lambda: regex.sub(r'^\s*', 'X', 'test'), ascii('Xtest'))

        # 14..17
        self.expect(lambda: regex.sub(r"x", r"\x0A", "x"), ascii("\n"))
        self.expect(lambda: regex.sub(r"x", r"\u000A", "x"), ascii("\n"))
        self.expect(lambda: regex.sub(r"x", r"\U0000000A", "x"), ascii("\n"))
        self.expect(lambda: regex.sub(r"x", r"\N{LATIN CAPITAL LETTER A}",
          "x"), ascii("A"))

        # 18..21
        self.expect(lambda: regex.sub(br"x", br"\x0A", b"x"), ascii(b"\n"))
        self.expect(lambda: regex.sub(br"x", br"\u000A", b"x"),
          ascii(b"\\u000A"))
        self.expect(lambda: regex.sub(br"x", br"\U0000000A", b"x"),
          ascii(b"\\U0000000A"))
        self.expect(lambda: regex.sub(br"x", br"\N{LATIN CAPITAL LETTER A}",
          b"x"), ascii(b"\\N{LATIN CAPITAL LETTER A}"))

    def test_bug_449964(self):
        # Fails for group followed by other escape.
        self.expect(lambda: regex.sub(r'(?P<unk>x)', r'\g<1>\g<1>\b', 'xx'),
          ascii("xx\bxx\b"))

    def test_bug_449000(self):
        # Test for sub() on escaped characters.
        # 1..4
        self.expect(lambda: regex.sub(r'\r\n', r'\n', 'abc\r\ndef\r\n'),
          ascii("abc\ndef\n"))
        self.expect(lambda: regex.sub('\r\n', r'\n', 'abc\r\ndef\r\n'),
          ascii("abc\ndef\n"))
        self.expect(lambda: regex.sub(r'\r\n', '\n', 'abc\r\ndef\r\n'),
          ascii("abc\ndef\n"))
        self.expect(lambda: regex.sub('\r\n', '\n', 'abc\r\ndef\r\n'),
          ascii("abc\ndef\n"))

    def test_bug_1661(self):
        # Verify that flags do not get silently ignored with compiled patterns
        pattern = regex.compile('.')
        # 1..4
        self.expect(lambda: regex.match(pattern, 'A', regex.I),
          self.FLAGS_WITH_COMPILED_PAT)
        self.expect(lambda: regex.search(pattern, 'A', regex.I),
          self.FLAGS_WITH_COMPILED_PAT)
        self.expect(lambda: regex.findall(pattern, 'A', regex.I),
          self.FLAGS_WITH_COMPILED_PAT)
        self.expect(lambda: regex.compile(pattern, regex.I),
          self.FLAGS_WITH_COMPILED_PAT)

    def test_bug_3629(self):
        # A regex that triggered a bug in the sre-code validator
        self.expect(lambda: type(regex.compile("(?P<quote>)(?(quote))")),
          self.PATTERN_CLASS)

    def test_sub_template_numeric_escape(self):
        # Bug 776311 and friends.
        # 1..7
        self.expect(lambda: regex.sub('x', r'\0', 'x'), ascii("\0"))
        self.expect(lambda: regex.sub('x', r'\000', 'x'), ascii("\000"))
        self.expect(lambda: regex.sub('x', r'\001', 'x'), ascii("\001"))
        self.expect(lambda: regex.sub('x', r'\008', 'x'), ascii("\0" + "8"))
        self.expect(lambda: regex.sub('x', r'\009', 'x'), ascii("\0" + "9"))
        self.expect(lambda: regex.sub('x', r'\111', 'x'), ascii("\111"))
        self.expect(lambda: regex.sub('x', r'\117', 'x'), ascii("\117"))

        # 8..9
        self.expect(lambda: regex.sub('x', r'\1111', 'x'), ascii("\1111"))
        self.expect(lambda: regex.sub('x', r'\1111', 'x'), ascii("\111" + "1"))

        # 10..14
        self.expect(lambda: regex.sub('x', r'\00', 'x'), ascii('\x00'))
        self.expect(lambda: regex.sub('x', r'\07', 'x'), ascii('\x07'))
        self.expect(lambda: regex.sub('x', r'\08', 'x'), ascii("\0" + "8"))
        self.expect(lambda: regex.sub('x', r'\09', 'x'), ascii("\0" + "9"))
        self.expect(lambda: regex.sub('x', r'\0a', 'x'), ascii("\0" + "a"))

        # 15..18
        self.expect(lambda: regex.sub('x', r'\400', 'x'), ascii("\u0100"))
        self.expect(lambda: regex.sub('x', r'\777', 'x'), ascii("\u01FF"))
        self.expect(lambda: regex.sub(b'x', br'\400', b'x'), ascii(b"\x00"))
        self.expect(lambda: regex.sub(b'x', br'\777', b'x'), ascii(b"\xFF"))

        # 19..30
        self.expect(lambda: regex.sub('x', r'\1', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\8', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\9', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\11', 'x'),
          self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\18', 'x'),
          self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\1a', 'x'),
          self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\90', 'x'),
          self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\99', 'x'),
          self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\118', 'x'),
          self.INVALID_GROUP_REF) # r'\11' + '8'
        self.expect(lambda: regex.sub('x', r'\11a', 'x'),
          self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\181', 'x'),
          self.INVALID_GROUP_REF) # r'\18' + '1'
        self.expect(lambda: regex.sub('x', r'\800', 'x'),
          self.INVALID_GROUP_REF) # r'\80' + '0'

        # In Python 2.3 (etc), these loop endlessly in sre_parser.py.
        # 31..33
        self.expect(lambda: regex.sub('(((((((((((x)))))))))))', r'\11', 'x'),
          ascii('x'))
        self.expect(lambda: regex.sub('((((((((((y))))))))))(.)', r'\118',
          'xyz'), ascii('xz8'))
        self.expect(lambda: regex.sub('((((((((((y))))))))))(.)', r'\11a',
          'xyz'), ascii('xza'))

    def test_qualified_re_sub(self):
        # 1..2
        self.expect(lambda: regex.sub('a', 'b', 'aaaaa'), ascii('bbbbb'))
        self.expect(lambda: regex.sub('a', 'b', 'aaaaa', 1), ascii('baaaa'))

    def test_bug_114660(self):
        self.expect(lambda: regex.sub(r'(\S)\s+(\S)', r'\1 \2',
          'hello  there'), ascii('hello there'))

    def test_bug_462270(self):
        # Test for empty sub() behaviour, see SF bug #462270
        # 1..2
        self.expect(lambda: regex.sub('x*', '-', 'abxd'), ascii('-a-b--d-'))
        self.expect(lambda: regex.sub('x+', '-', 'abxd'), ascii('ab-d'))

    def test_symbolic_refs(self):
        # 1..6
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a', 'xx'),
          self.MISSING_GT)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<', 'xx'),
          self.BAD_GROUP_NAME)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g', 'xx'),
          self.MISSING_LT)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a a>', 'xx'),
          self.MISSING_GT)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<1a1>', 'xx'),
          self.BAD_GROUP_NAME)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<ab>', 'xx'),
          self.UNKNOWN_GROUP_I)

        # The new behaviour of unmatched but valid groups is to treat them like
        # empty matches in the replacement template, like in Perl.
        # 7..8
        self.expect(lambda: regex.sub('(?P<a>x)|(?P<b>y)', r'\g<b>', 'xx'),
          ascii(''))
        self.expect(lambda: regex.sub('(?P<a>x)|(?P<b>y)', r'\2', 'xx'),
          ascii(''))

        # The old behaviour was to raise it as an IndexError.
        # 9
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<-1>', 'xx'),
          self.BAD_GROUP_NAME)

    def test_re_subn(self):
        # 1..5
        self.expect(lambda: regex.subn("(?i)b+", "x", "bbbb BBBB"),
          ascii(('x x', 2)))
        self.expect(lambda: regex.subn("b+", "x", "bbbb BBBB"), ascii(('x BBBB',
          1)))
        self.expect(lambda: regex.subn("b+", "x", "xyz"), ascii(('xyz', 0)))
        self.expect(lambda: regex.subn("b*", "x", "xyz"), ascii(('xxxyxzx', 4)))
        self.expect(lambda: regex.subn("b*", "x", "xyz", 2), ascii(('xxxyz',
          2)))

    def test_re_split(self):
        # 1..8
        self.expect(lambda: regex.split(":", ":a:b::c"), ascii(['', 'a', 'b',
          '', 'c']))
        self.expect(lambda: regex.split(":*", ":a:b::c"), ascii(['', 'a', 'b',
          'c']))
        self.expect(lambda: regex.split("(:*)", ":a:b::c"), ascii(['', ':', 'a',
          ':', 'b', '::', 'c']))
        self.expect(lambda: regex.split("(?::*)", ":a:b::c"), ascii(['', 'a',
          'b', 'c']))
        self.expect(lambda: regex.split("(:)*", ":a:b::c"), ascii(['', ':', 'a',
          ':', 'b', ':', 'c']))
        self.expect(lambda: regex.split("([b:]+)", ":a:b::c"), ascii(['', ':',
          'a', ':b::', 'c']))
        self.expect(lambda: regex.split("(b)|(:+)", ":a:b::c"), ascii(['', None,
          ':', 'a', None, ':', '', 'b', None, '', None, '::', 'c']))
        self.expect(lambda: regex.split("(?:b)|(?::+)", ":a:b::c"), ascii(['',
          'a', '', '', 'c']))

        # 9..10
        self.expect(lambda: regex.split("x", "xaxbxc"), ascii(['', 'a', 'b',
          'c']))
        self.expect(lambda: [m for m in regex.splititer("x", "xaxbxc")],
          ascii(['', 'a', 'b', 'c']))

        # 11..12
        self.expect(lambda: regex.split("(?r)x", "xaxbxc"), ascii(['c', 'b',
          'a', '']))
        self.expect(lambda: [m for m in regex.splititer("(?r)x", "xaxbxc")],
          ascii(['c', 'b', 'a', '']))

        # 13..14
        self.expect(lambda: regex.split("(x)|(y)", "xaxbxc"), ascii(['', 'x',
          None, 'a', 'x', None, 'b', 'x', None, 'c']))
        self.expect(lambda: [m for m in regex.splititer("(x)|(y)", "xaxbxc")],
          ascii(['', 'x', None, 'a', 'x', None, 'b', 'x', None, 'c']))

        # 15..16
        self.expect(lambda: regex.split("(?r)(x)|(y)", "xaxbxc"), ascii(['c',
          'x', None, 'b', 'x', None, 'a', 'x', None, '']))
        self.expect(lambda: [m for m in regex.splititer("(?r)(x)|(y)",
          "xaxbxc")], ascii(['c', 'x', None, 'b', 'x', None, 'a', 'x', None,
          '']))

        # 17..19
        self.expect(lambda: regex.split(r"(?V1)\b", "a b c"), ascii(['', 'a',
          ' ', 'b', ' ', 'c', '']))
        self.expect(lambda: regex.split(r"(?V1)\m", "a b c"), ascii(['', 'a ',
          'b ', 'c']))
        self.expect(lambda: regex.split(r"(?V1)\M", "a b c"), ascii(['a', ' b',
          ' c', '']))

    def test_qualified_re_split(self):
        # 1..4
        self.expect(lambda: regex.split(":", ":a:b::c", 2), ascii(['', 'a',
          'b::c']))
        self.expect(lambda: regex.split(':', 'a:b:c:d', 2), ascii(['a', 'b',
          'c:d']))
        self.expect(lambda: regex.split("(:)", ":a:b::c", 2), ascii(['', ':',
          'a', ':', 'b::c']))
        self.expect(lambda: regex.split("(:*)", ":a:b::c", 2), ascii(['', ':',
          'a', ':', 'b::c']))

    def test_re_findall(self):
        # 1..4
        self.expect(lambda: regex.findall(":+", "abc"), ascii([]))
        self.expect(lambda: regex.findall(":+", "a:b::c:::d"), ascii([':', '::',
          ':::']))
        self.expect(lambda: regex.findall("(:+)", "a:b::c:::d"), ascii([':',
          '::', ':::']))
        self.expect(lambda: regex.findall("(:)(:*)", "a:b::c:::d"), ascii([(':',
          ''), (':', ':'), (':', '::')]))

        # 5..7
        self.expect(lambda: regex.findall(r"\((?P<test>.{0,5}?TEST)\)",
          "(MY TEST)"), ascii(["MY TEST"]))
        self.expect(lambda: regex.findall(r"\((?P<test>.{0,3}?TEST)\)",
          "(MY TEST)"), ascii(["MY TEST"]))
        self.expect(lambda: regex.findall(r"\((?P<test>.{0,3}?T)\)",
          "(MY T)"), ascii(["MY T"]))

        # 8..10
        self.expect(lambda: regex.findall(r"[^a]{2}[A-Z]", "\n  S"),
          ascii(['  S']))
        self.expect(lambda: regex.findall(r"[^a]{2,3}[A-Z]", "\n  S"),
          ascii(['\n  S']))
        self.expect(lambda: regex.findall(r"[^a]{2,3}[A-Z]", "\n   S"),
          ascii(['   S']))

        # 11..12
        self.expect(lambda: regex.findall(r"X(Y[^Y]+?){1,2}( |Q)+DEF",
          "XYABCYPPQ\nQ DEF"), ascii([('YPPQ\n', ' ')]))

        self.expect(lambda: regex.findall(r"(\nTest(\n+.+?){0,2}?)?\n+End",
          "\nTest\nxyz\nxyz\nEnd"), ascii([('\nTest\nxyz\nxyz', '\nxyz')]))

    def test_bug_117612(self):
        self.expect(lambda: regex.findall(r"(a|(b))", "aba"), ascii([('a', ''),
          ('b', 'b'), ('a', '')]))

    def test_re_match(self):
        # 1..5
        self.expect(lambda: regex.match('a', 'a')[:], ascii(('a',)))
        self.expect(lambda: regex.match('(a)', 'a')[:], ascii(('a', 'a')))
        self.expect(lambda: regex.match(r'(a)', 'a')[0], ascii('a'))
        self.expect(lambda: regex.match(r'(a)', 'a')[1], ascii('a'))
        self.expect(lambda: regex.match(r'(a)', 'a').group(1, 1), ascii(('a',
          'a')))

        pat = regex.compile('((a)|(b))(c)?')
        # 6..10
        self.expect(lambda: pat.match('a')[:], ascii(('a', 'a', 'a', None,
          None)))
        self.expect(lambda: pat.match('b')[:], ascii(('b', 'b', None, 'b',
          None)))
        self.expect(lambda: pat.match('ac')[:], ascii(('ac', 'a', 'a', None,
          'c')))
        self.expect(lambda: pat.match('bc')[:], ascii(('bc', 'b', None, 'b',
          'c')))
        self.expect(lambda: pat.match('bc')[:], ascii(('bc', 'b', None, 'b',
          'c')))

        # A single group.
        m = regex.match('(a)', 'a')
        # 11..14
        self.expect(lambda: m.group(), ascii('a'))
        self.expect(lambda: m.group(0), ascii('a'))
        self.expect(lambda: m.group(1), ascii('a'))
        self.expect(lambda: m.group(1, 1), ascii(('a', 'a')))

        pat = regex.compile('(?:(?P<a1>a)|(?P<b2>b))(?P<c3>c)?')
        # 15..17
        self.expect(lambda: pat.match('a').group(1, 2, 3), ascii(('a', None,
          None)))
        self.expect(lambda: pat.match('b').group('a1', 'b2', 'c3'), ascii((None,
          'b', None)))
        self.expect(lambda: pat.match('ac').group(1, 'b2', 3), ascii(('a', None,
          'c')))

    def test_re_groupref_exists(self):
        # 1..8
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', '(a)')[:],
          ascii(('(a)', '(', 'a')))
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', 'a')[:],
          ascii(('a', None, 'a')))
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', 'a)'),
          ascii(None))
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', '(a'),
          ascii(None))
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)b|d))$', 'ab')[:],
          ascii(('ab', 'a', 'b')))
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)b|d))$', 'cd')[:],
          ascii(('cd', None, 'd')))
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)|d))$', 'cd')[:],
          ascii(('cd', None, 'd')))
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)|d))$', 'a')[:],
          ascii(('a', 'a', '')))

        # Tests for bug #1177831: exercise groups other than the first group.
        p = regex.compile('(?P<g1>a)(?P<g2>b)?((?(g2)c|d))')
        # 9..12
        self.expect(lambda: p.match('abc')[:], ascii(('abc', 'a', 'b', 'c')))
        self.expect(lambda: p.match('ad')[:], ascii(('ad', 'a', None, 'd')))
        self.expect(lambda: p.match('abd'), ascii(None))
        self.expect(lambda: p.match('ac'), ascii(None))

    def test_re_groupref(self):
        # 1..6
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1$', '|a|')[:],
          ascii(('|a|', '|', 'a')))
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1?$', 'a')[:],
          ascii(('a', None, 'a')))
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1$', 'a|'),
          ascii(None))
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1$', '|a'),
          ascii(None))
        self.expect(lambda: regex.match(r'^(?:(a)|c)(\1)$', 'aa')[:],
          ascii(('aa', 'a', 'a')))
        self.expect(lambda: regex.match(r'^(?:(a)|c)(\1)?$', 'c')[:],
          ascii(('c', None, None)))

        self.expect(lambda:
          regex.findall("(?i)(.{1,40}?),(.{1,40}?)(?:;)+(.{1,80}).{1,40}?\\3(\ |;)+(.{1,80}?)\\1",
          "TEST, BEST; LEST ; Lest 123 Test, Best"), ascii([('TEST', ' BEST',
          ' LEST', ' ', '123 ')]))

    def test_groupdict(self):
        self.expect(lambda: regex.match('(?P<first>first) (?P<second>second)',
          'first second').groupdict(), ascii({'first': 'first', 'second':
          'second'}))

    def test_expand(self):
        self.expect(lambda: regex.match("(?P<first>first) (?P<second>second)",
          "first second").expand(r"\2 \1 \g<second> \g<first>"),
          ascii('second first second first'))

    def test_repeat_minmax(self):
        # 1..4
        self.expect(lambda: regex.match(r"^(\w){1}$", "abc"), ascii(None))
        self.expect(lambda: regex.match(r"^(\w){1}?$", "abc"), ascii(None))
        self.expect(lambda: regex.match(r"^(\w){1,2}$", "abc"), ascii(None))
        self.expect(lambda: regex.match(r"^(\w){1,2}?$", "abc"), ascii(None))

        # 5..12
        self.expect(lambda: regex.match(r"^(\w){3}$", "abc")[1], ascii('c'))
        self.expect(lambda: regex.match(r"^(\w){1,3}$", "abc")[1], ascii('c'))
        self.expect(lambda: regex.match(r"^(\w){1,4}$", "abc")[1], ascii('c'))
        self.expect(lambda: regex.match(r"^(\w){3,4}?$", "abc")[1], ascii('c'))
        self.expect(lambda: regex.match(r"^(\w){3}?$", "abc")[1], ascii('c'))
        self.expect(lambda: regex.match(r"^(\w){1,3}?$", "abc")[1], ascii('c'))
        self.expect(lambda: regex.match(r"^(\w){1,4}?$", "abc")[1], ascii('c'))
        self.expect(lambda: regex.match(r"^(\w){3,4}?$", "abc")[1], ascii('c'))

        # 13..16
        self.expect(lambda: regex.match("^x{1}$", "xxx"), ascii(None))
        self.expect(lambda: regex.match("^x{1}?$", "xxx"), ascii(None))
        self.expect(lambda: regex.match("^x{1,2}$", "xxx"), ascii(None))
        self.expect(lambda: regex.match("^x{1,2}?$", "xxx"), ascii(None))

        # 17..20
        self.expect(lambda: regex.match("^x{1}", "xxx")[0], ascii('x'))
        self.expect(lambda: regex.match("^x{1}?", "xxx")[0], ascii('x'))
        self.expect(lambda: regex.match("^x{0,1}", "xxx")[0], ascii('x'))
        self.expect(lambda: regex.match("^x{0,1}?", "xxx")[0], ascii(''))

        # 21..28
        self.expect(lambda: type(regex.match("^x{3}$", "xxx")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match("^x{1,3}$", "xxx")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match("^x{1,4}$", "xxx")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match("^x{3,4}?$", "xxx")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match("^x{3}?$", "xxx")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match("^x{1,3}?$", "xxx")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match("^x{1,4}?$", "xxx")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match("^x{3,4}?$", "xxx")),
          self.MATCH_CLASS)

        # 29..30
        self.expect(lambda: regex.match("^x{}$", "xxx"), ascii(None))
        self.expect(lambda: type(regex.match("^x{}$", "x{}")),
          self.MATCH_CLASS)

    def test_getattr(self):
        # 1..6
        self.expect(lambda: regex.compile("(?i)(a)(b)").pattern,
          ascii('(?i)(a)(b)'))
        self.expect(lambda: regex.compile("(?i)(a)(b)").flags, ascii(regex.I |
          regex.U | regex.DEFAULT_VERSION))
        self.expect(lambda: regex.compile(b"(?i)(a)(b)").flags, ascii(regex.A |
          regex.I | regex.DEFAULT_VERSION))
        self.expect(lambda: regex.compile("(?i)(a)(b)").groups, ascii(2))
        self.expect(lambda: regex.compile("(?i)(a)(b)").groupindex, ascii({}))
        self.expect(lambda:
          regex.compile("(?i)(?P<first>a)(?P<other>b)").groupindex,
          ascii({'first': 1, 'other': 2}))

        # 7..8
        self.expect(lambda: regex.match("(a)", "a").pos, ascii(0))
        self.expect(lambda: regex.match("(a)", "a").endpos, ascii(1))

        # 9..12
        self.expect(lambda: regex.search("b(c)", "abcdef").pos, ascii(0))
        self.expect(lambda: regex.search("b(c)", "abcdef").endpos, ascii(6))
        self.expect(lambda: regex.search("b(c)", "abcdef").span(), ascii((1,
          3)))
        self.expect(lambda: regex.search("b(c)", "abcdef").span(1), ascii((2,
          3)))

        # 13..15
        self.expect(lambda: regex.match("(a)", "a").string, ascii('a'))
        self.expect(lambda: regex.match("(a)", "a").regs, ascii(((0, 1), (0,
          1))))
        self.expect(lambda: type(regex.match("(a)", "a").re),
          self.PATTERN_CLASS)

    def test_special_escapes(self):
        # 1..6
        self.expect(lambda: regex.search(r"\b(b.)\b", "abcd abc bcd bx")[1],
          ascii('bx'))
        self.expect(lambda: regex.search(r"\B(b.)\B", "abc bcd bc abxd")[1],
          ascii('bx'))
        self.expect(lambda: regex.search(br"\b(b.)\b", b"abcd abc bcd bx",
          regex.LOCALE)[1], ascii(b'bx'))
        self.expect(lambda: regex.search(br"\B(b.)\B", b"abc bcd bc abxd",
          regex.LOCALE)[1], ascii(b'bx'))
        self.expect(lambda: regex.search(r"\b(b.)\b", "abcd abc bcd bx",
          regex.UNICODE)[1], ascii('bx'))
        self.expect(lambda: regex.search(r"\B(b.)\B", "abc bcd bc abxd",
          regex.UNICODE)[1], ascii('bx'))

        # 7..9
        self.expect(lambda: regex.search(r"^abc$", "\nabc\n", regex.M)[0],
          ascii('abc'))
        self.expect(lambda: regex.search(r"^\Aabc\Z$", "abc", regex.M)[0],
          ascii('abc'))
        self.expect(lambda: regex.search(r"^\Aabc\Z$", "\nabc\n", regex.M),
          ascii(None))

        # 10..14
        self.expect(lambda: regex.search(br"\b(b.)\b", b"abcd abc bcd bx")[1],
          ascii(b'bx'))
        self.expect(lambda: regex.search(br"\B(b.)\B", b"abc bcd bc abxd")[1],
          ascii(b'bx'))
        self.expect(lambda: regex.search(br"^abc$", b"\nabc\n", regex.M)[0],
          ascii(b'abc'))
        self.expect(lambda: regex.search(br"^\Aabc\Z$", b"abc", regex.M)[0],
          ascii(b'abc'))
        self.expect(lambda: regex.search(br"^\Aabc\Z$", b"\nabc\n", regex.M),
          ascii(None))

        # 15..17
        self.expect(lambda: regex.search(r"\d\D\w\W\s\S", "1aa! a")[0],
          ascii('1aa! a'))
        self.expect(lambda: regex.search(br"\d\D\w\W\s\S", b"1aa! a",
          regex.LOCALE)[0], ascii(b'1aa! a'))
        self.expect(lambda: regex.search(r"\d\D\w\W\s\S", "1aa! a",
          regex.UNICODE)[0], ascii('1aa! a'))

    def test_bigcharset(self):
        # 1..5
        self.expect(lambda: regex.match(r"([\u2222\u2223])", "\u2222")[1],
          ascii('\u2222'))
        self.expect(lambda: regex.match(r"([\u2222\u2223])", "\u2222",
          regex.UNICODE)[1], ascii('\u2222'))
        self.expect(lambda: "".join(regex.findall(".",
          "e\xe8\xe9\xea\xeb\u0113\u011b\u0117", flags=regex.UNICODE)),
          ascii('e\xe8\xe9\xea\xeb\u0113\u011b\u0117'))
        self.expect(lambda:
          "".join(regex.findall(r"[e\xe8\xe9\xea\xeb\u0113\u011b\u0117]",
          "e\xe8\xe9\xea\xeb\u0113\u011b\u0117", flags=regex.UNICODE)),
          ascii('e\xe8\xe9\xea\xeb\u0113\u011b\u0117'))
        self.expect(lambda:
          "".join(regex.findall(r"e|\xe8|\xe9|\xea|\xeb|\u0113|\u011b|\u0117",
          "e\xe8\xe9\xea\xeb\u0113\u011b\u0117", flags=regex.UNICODE)),
          ascii('e\xe8\xe9\xea\xeb\u0113\u011b\u0117'))

    def test_anyall(self):
        # 1..2
        self.expect(lambda: regex.match("a.b", "a\nb", regex.DOTALL)[0],
          ascii("a\nb"))
        self.expect(lambda: regex.match("a.*b", "a\n\nb", regex.DOTALL)[0],
          ascii("a\n\nb"))

    def test_non_consuming(self):
        # 1..7
        self.expect(lambda: regex.match(r"(a(?=\s[^a]))", "a b")[1], ascii('a'))
        self.expect(lambda: regex.match(r"(a(?=\s[^a]*))", "a b")[1],
          ascii('a'))
        self.expect(lambda: regex.match(r"(a(?=\s[abc]))", "a b")[1],
          ascii('a'))
        self.expect(lambda: regex.match(r"(a(?=\s[abc]*))", "a bc")[1],
          ascii('a'))
        self.expect(lambda: regex.match(r"(a)(?=\s\1)", "a a")[1], ascii('a'))
        self.expect(lambda: regex.match(r"(a)(?=\s\1*)", "a aa")[1], ascii('a'))
        self.expect(lambda: regex.match(r"(a)(?=\s(abc|a))", "a a")[1],
          ascii('a'))

        # 8..11
        self.expect(lambda: regex.match(r"(a(?!\s[^a]))", "a a")[1], ascii('a'))
        self.expect(lambda: regex.match(r"(a(?!\s[abc]))", "a d")[1],
          ascii('a'))
        self.expect(lambda: regex.match(r"(a)(?!\s\1)", "a b")[1], ascii('a'))
        self.expect(lambda: regex.match(r"(a)(?!\s(abc|a))", "a b")[1],
          ascii('a'))

    def test_ignore_case(self):
        # 1..2
        self.expect(lambda: regex.match("abc", "ABC", regex.I)[0], ascii('ABC'))
        self.expect(lambda: regex.match(b"abc", b"ABC", regex.I)[0],
          ascii(b'ABC'))

        # 3..9
        self.expect(lambda: regex.match(r"(a\s[^a]*)", "a bb", regex.I)[1],
          ascii('a bb'))
        self.expect(lambda: regex.match(r"(a\s[abc])", "a b", regex.I)[1],
          ascii('a b'))
        self.expect(lambda: regex.match(r"(a\s[abc]*)", "a bb", regex.I)[1],
          ascii('a bb'))
        self.expect(lambda: regex.match(r"((a)\s\2)", "a a", regex.I)[1],
          ascii('a a'))
        self.expect(lambda: regex.match(r"((a)\s\2*)", "a aa", regex.I)[1],
          ascii('a aa'))
        self.expect(lambda: regex.match(r"((a)\s(abc|a))", "a a", regex.I)[1],
          ascii('a a'))
        self.expect(lambda: regex.match(r"((a)\s(abc|a)*)", "a aa",
          regex.I)[1], ascii('a aa'))

        # Issue #3511.
        # 10..11
        self.expect(lambda: regex.match(r"[Z-a]", "_").span(), ascii((0, 1)))
        self.expect(lambda: regex.match(r"(?i)[Z-a]", "_").span(), ascii((0,
          1)))

        # 12..15
        self.expect(lambda: type(regex.match(r"(?i)nao", "nAo")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r"(?i)n\xE3o", "n\xC3o")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r"(?i)n\xE3o", "N\xC3O")),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r"(?i)s", "\u017F")),
          self.MATCH_CLASS)

    def test_case_folding(self):
        # 1..4
        self.expect(lambda: regex.search(r"(?fi)ss", "SS").span(), ascii((0,
          2)))
        self.expect(lambda: regex.search(r"(?fi)SS", "ss").span(), ascii((0,
          2)))
        self.expect(lambda: regex.search(r"(?fi)SS",
          "\N{LATIN SMALL LETTER SHARP S}").span(), ascii((0, 1)))
        self.expect(lambda:
          regex.search(r"(?fi)\N{LATIN SMALL LETTER SHARP S}", "SS").span(),
          ascii((0, 2)))

        # 5..7
        self.expect(lambda: regex.search(r"(?fi)\N{LATIN SMALL LIGATURE ST}",
          "ST").span(), ascii((0, 2)))
        self.expect(lambda: regex.search(r"(?fi)ST",
          "\N{LATIN SMALL LIGATURE ST}").span(), ascii((0, 1)))
        self.expect(lambda: regex.search(r"(?fi)ST",
          "\N{LATIN SMALL LIGATURE LONG S T}").span(), ascii((0, 1)))

        # 8..12
        self.expect(lambda: regex.search(r"(?fi)SST",
          "\N{LATIN SMALL LETTER SHARP S}t").span(), ascii((0, 2)))
        self.expect(lambda: regex.search(r"(?fi)SST",
          "s\N{LATIN SMALL LIGATURE LONG S T}").span(), ascii((0, 2)))
        self.expect(lambda: regex.search(r"(?fi)SST",
          "s\N{LATIN SMALL LIGATURE ST}").span(), ascii((0, 2)))
        self.expect(lambda: regex.search(r"(?fi)\N{LATIN SMALL LIGATURE ST}",
          "SST").span(), ascii((1, 3)))
        self.expect(lambda: regex.search(r"(?fi)SST",
          "s\N{LATIN SMALL LIGATURE ST}").span(), ascii((0, 2)))

        # 13..18
        self.expect(lambda: regex.search(r"(?fi)FFI",
          "\N{LATIN SMALL LIGATURE FFI}").span(), ascii((0, 1)))
        self.expect(lambda: regex.search(r"(?fi)FFI",
          "\N{LATIN SMALL LIGATURE FF}i").span(), ascii((0, 2)))
        self.expect(lambda: regex.search(r"(?fi)FFI",
          "f\N{LATIN SMALL LIGATURE FI}").span(), ascii((0, 2)))
        self.expect(lambda: regex.search(r"(?fi)\N{LATIN SMALL LIGATURE FFI}",
          "FFI").span(), ascii((0, 3)))
        self.expect(lambda: regex.search(r"(?fi)\N{LATIN SMALL LIGATURE FF}i",
          "FFI").span(), ascii((0, 3)))
        self.expect(lambda: regex.search(r"(?fi)f\N{LATIN SMALL LIGATURE FI}",
          "FFI").span(), ascii((0, 3)))

        # 19..27
        sigma = "\u03A3\u03C3\u03C2"
        for ch1 in sigma:
            for ch2 in sigma:
                self.index += 1
                if not regex.match(r"(?fi)" + ch1, ch2):
                    self.record_failure()

        # 28..39
        self.expect(lambda: bool(regex.search(r"(?iV1)ff", "\uFB00\uFB01")),
          ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)ff", "\uFB01\uFB00")),
          ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)fi", "\uFB00\uFB01")),
          ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)fi", "\uFB01\uFB00")),
          ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)fffi", "\uFB00\uFB01")),
          ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)f\uFB03",
          "\uFB00\uFB01")), ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)ff", "\uFB00\uFB01")),
          ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)fi", "\uFB00\uFB01")),
          ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)fffi", "\uFB00\uFB01")),
          ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)f\uFB03",
          "\uFB00\uFB01")), ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)f\uFB01", "\uFB00i")),
          ascii(True))
        self.expect(lambda: bool(regex.search(r"(?iV1)f\uFB01", "\uFB00i")),
          ascii(True))

        # 40..41
        self.expect(lambda:
          regex.findall(r"(?iV0)\m(?:word){e<=3}\M(?<!\m(?:word){e<=1}\M)",
          "word word2 word word3 word word234 word23 word"), ascii(["word234",
          "word23"]))
        self.expect(lambda:
          regex.findall(r"(?iV1)\m(?:word){e<=3}\M(?<!\m(?:word){e<=1}\M)",
          "word word2 word word3 word word234 word23 word"), ascii(["word234",
          "word23"]))

    def test_category(self):
        self.expect(lambda: regex.match(r"(\s)", " ")[1], ascii(' '))

    def test_not_literal(self):
        # 1..2
        self.expect(lambda: regex.search(r"\s([^a])", " b")[1], ascii('b'))
        self.expect(lambda: regex.search(r"\s([^a]*)", " bb")[1], ascii('bb'))

    def test_search_coverage(self):
        # 1..2
        self.expect(lambda: regex.search(r"\s(b)", " b")[1], ascii('b'))
        self.expect(lambda: regex.search(r"a\s", "a ")[0], ascii('a '))

    def test_re_escape(self):
        p = ""
        self.expect(lambda: regex.escape(p), ascii(p))
        for i in range(0, 256):
            p += chr(i)
            self.expect(lambda: type(regex.match(regex.escape(chr(i)),
              chr(i))), self.MATCH_CLASS)
            self.expect(lambda: regex.match(regex.escape(chr(i)),
              chr(i)).span(), ascii((0, 1)))

        pat = regex.compile(regex.escape(p))
        self.expect(lambda: pat.match(p).span(), ascii((0, 256)))

    def test_re_escape_byte(self):
        p = b""
        self.expect(lambda: regex.escape(p), ascii(p))
        for i in range(0, 256):
            b = bytes([i])
            p += b
            self.expect(lambda: type(regex.match(regex.escape(b), b)),
              self.MATCH_CLASS)
            self.expect(lambda: regex.match(regex.escape(b), b).span(),
              ascii((0, 1)))

        pat = regex.compile(regex.escape(p))
        self.expect(lambda: pat.match(p).span(), ascii((0, 256)))

    def test_constants(self):
        self.index += 1
        if regex.I != regex.IGNORECASE:
            self.record_failure()
        self.index += 1
        if regex.L != regex.LOCALE:
            self.record_failure()
        self.index += 1
        if regex.M != regex.MULTILINE:
            self.record_failure()
        self.index += 1
        if regex.S != regex.DOTALL:
            self.record_failure()
        self.index += 1
        if regex.X != regex.VERBOSE:
            self.record_failure()

    def test_flags(self):
        for flag in [regex.I, regex.M, regex.X, regex.S, regex.L]:
            self.expect(lambda: type(regex.compile('^pattern$', flag)),
              self.PATTERN_CLASS)

    def test_sre_character_literals(self):
        for i in [0, 8, 16, 32, 64, 127, 128, 255]:
            self.expect(lambda: type(regex.match(r"\%03o" % i, chr(i))),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"\%03o0" % i, chr(i) + "0")),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"\%03o8" % i, chr(i) + "8")),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"\x%02x" % i, chr(i))),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"\x%02x0" % i, chr(i) +
              "0")), self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"\x%02xz" % i, chr(i) +
              "z")), self.MATCH_CLASS)
        self.expect(lambda: regex.match(r"\911", ""), self.UNKNOWN_GROUP)

    def test_sre_character_class_literals(self):
        for i in [0, 8, 16, 32, 64, 127, 128, 255]:
            self.expect(lambda: type(regex.match(r"[\%03o]" % i, chr(i))),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"[\%03o0]" % i, chr(i))),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"[\%03o8]" % i, chr(i))),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"[\x%02x]" % i, chr(i))),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"[\x%02x0]" % i, chr(i))),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"[\x%02xz]" % i, chr(i))),
              self.MATCH_CLASS)
        self.expect(lambda: regex.match(r"[\911]", ""), self.BAD_OCTAL_ESCAPE)

    def test_bug_113254(self):
        # 1..3
        self.expect(lambda: regex.match(r'(a)|(b)', 'b').start(1), ascii(-1))
        self.expect(lambda: regex.match(r'(a)|(b)', 'b').end(1), ascii(-1))
        self.expect(lambda: regex.match(r'(a)|(b)', 'b').span(1), ascii((-1,
          -1)))

    def test_bug_527371(self):
        # Bug described in patches 527371/672491.
        # 1..5
        self.expect(lambda: regex.match(r'(a)?a','a').lastindex, ascii(None))
        self.expect(lambda: regex.match(r'(a)(b)?b','ab').lastindex, ascii(1))
        self.expect(lambda: regex.match(r'(?P<a>a)(?P<b>b)?b','ab').lastgroup,
          ascii('a'))
        self.expect(lambda: regex.match("(?P<a>a(b))", "ab").lastgroup,
          ascii('a'))
        self.expect(lambda: regex.match("((a))", "a").lastindex, ascii(1))

    def test_bug_545855(self):
        # Bug 545855 -- This pattern failed to cause a compile error as it
        # should, instead provoking a TypeError.
        self.expect(lambda: regex.compile('foo[a-'), self.BAD_SET)

    def test_bug_418626(self):
        # Bugs 418626 at al. -- Testing Greg Chapman's addition of op code
        # SRE_OP_MIN_REPEAT_ONE for eliminating recursion on simple uses of
        # pattern '*?' on a long string.
        # 1..3
        self.expect(lambda: regex.match('.*?c', 10000 * 'ab' + 'cd').end(0),
          ascii(20001))
        self.expect(lambda: regex.match('.*?cd', 5000 * 'ab' + 'c' + 5000 *
          'ab' + 'cde').end(0), ascii(20003))
        self.expect(lambda: regex.match('.*?cd', 20000 * 'abc' + 'de').end(0),
          ascii(60001))
        # Non-simple '*?' still used to hit the recursion limit, before the
        # non-recursive scheme was implemented.
        # 4
        self.expect(lambda: regex.search('(a|b)*?c', 10000 * 'ab' +
          'cd').end(0), ascii(20001))

    def test_bug_612074(self):
        pat = "[" + regex.escape("\u2039") + "]"
        self.expect(lambda: regex.compile(pat) and 1, ascii(1))

    def test_stack_overflow(self):
        # Nasty cases that used to overflow the straightforward recursive
        # implementation of repeated groups.
        # 1..3
        self.expect(lambda: regex.match('(x)*', 50000 * 'x')[1], ascii('x'))
        self.expect(lambda: regex.match('(x)*y', 50000 * 'x' + 'y')[1],
          ascii('x'))
        self.expect(lambda: regex.match('(x)*?y', 50000 * 'x' + 'y')[1],
          ascii('x'))

    def test_scanner(self):
        def s_ident(scanner, token): return token
        def s_operator(scanner, token): return "op%s" % token
        def s_float(scanner, token): return float(token)
        def s_int(scanner, token): return int(token)

        scanner = regex.Scanner([
          (r"[a-zA-Z_]\w*", s_ident), (r"\d+\.\d*", s_float), (r"\d+", s_int),
          (r"=|\+|-|\*|/", s_operator), (r"\s+", None),
        ])

        # 1
        self.expect(lambda: type(scanner.scanner.scanner("").pattern),
          self.PATTERN_CLASS)

        # 2
        self.expect(lambda: scanner.scan("sum = 3*foo + 312.50 + bar"),
          ascii((['sum', 'op=', 3, 'op*', 'foo', 'op+', 312.5, 'op+', 'bar'],
          '')))

    def test_bug_448951(self):
        # Bug 448951 (similar to 429357, but with single char match).
        # (Also test greedy matches.)
        for op in '', '?', '*':
            self.expect(lambda: regex.match(r'((.%s):)?z' % op, 'z')[:],
              ascii(('z', None, None)))
            self.expect(lambda: regex.match(r'((.%s):)?z' % op, 'a:z')[:],
              ascii(('a:z', 'a:', 'a')))

    def test_bug_725106(self):
        # Capturing groups in alternatives in repeats.
        # 1..8
        self.expect(lambda: regex.match('^((a)|b)*', 'abc')[:], ascii(('ab',
          'b', 'a')))
        self.expect(lambda: regex.match('^(([ab])|c)*', 'abc')[:], ascii(('abc',
          'c', 'b')))
        self.expect(lambda: regex.match('^((d)|[ab])*', 'abc')[:], ascii(('ab',
          'b', None)))
        self.expect(lambda: regex.match('^((a)c|[ab])*', 'abc')[:], ascii(('ab',
          'b', None)))
        self.expect(lambda: regex.match('^((a)|b)*?c', 'abc')[:], ascii(('abc',
          'b', 'a')))
        self.expect(lambda: regex.match('^(([ab])|c)*?d', 'abcd')[:],
          ascii(('abcd', 'c', 'b')))
        self.expect(lambda: regex.match('^((d)|[ab])*?c', 'abc')[:],
          ascii(('abc', 'b', None)))
        self.expect(lambda: regex.match('^((a)c|[ab])*?c', 'abc')[:],
          ascii(('abc', 'b', None)))

    def test_bug_725149(self):
        # Mark_stack_base restoring before restoring marks.
        # 1..2
        self.expect(lambda: regex.match('(a)(?:(?=(b)*)c)*', 'abb')[:],
          ascii(('a', 'a', None)))
        self.expect(lambda: regex.match('(a)((?!(b)*))*', 'abb')[:], ascii(('a',
          'a', None, None)))

    def test_bug_764548(self):
        # Bug 764548, regex.compile() barfs on str/unicode subclasses.
        class my_unicode(str): pass
        pat = regex.compile(my_unicode("abc"))
        self.expect(lambda: pat.match("xyz"), ascii(None))

    def test_finditer(self):
        it = regex.finditer(r":+", "a:b::c:::d")
        self.expect(lambda: [item[0] for item in it], ascii([':', '::', ':::']))

    def test_bug_926075(self):
        self.index += 1
        if regex.compile('bug_926075') is regex.compile(b'bug_926075'):
            self.record_failure()

    def test_bug_931848(self):
        pattern = "[\u002E\u3002\uFF0E\uFF61]"
        self.expect(lambda: regex.compile(pattern).split("a.b.c"), ascii(['a',
          'b', 'c']))

    def test_bug_581080(self):
        it = regex.finditer(r"\s", "a b")
        # 1..2
        self.expect(lambda: next(it).span(), ascii((1, 2)))
        self.expect(lambda: next(it), ascii(StopIteration()))

        scanner = regex.compile(r"\s").scanner("a b")
        # 3..4
        self.expect(lambda: scanner.search().span(), ascii((1, 2)))
        self.expect(lambda: scanner.search(), ascii(None))

    def test_bug_817234(self):
        it = regex.finditer(r".*", "asdf")
        # 1..3
        self.expect(lambda: next(it).span(), ascii((0, 4)))
        self.expect(lambda: next(it).span(), ascii((4, 4)))
        self.expect(lambda: next(it), ascii(StopIteration()))

    def test_empty_array(self):
        # SF buf 1647541.
        import array
        for typecode in 'bBuhHiIlLfd':
            a = array.array(typecode)
            self.expect(lambda: regex.compile(b"bla").match(a), ascii(None))
            self.expect(lambda: regex.compile(b"").match(a)[1 : ], ascii(()))

    def test_inline_flags(self):
        # Bug #1700.
        upper_char = chr(0x1ea0) # Latin Capital Letter A with Dot Below
        lower_char = chr(0x1ea1) # Latin Small Letter A with Dot Below

        p = regex.compile(upper_char, regex.I | regex.U)
        # 1
        self.expect(lambda: type(p.match(lower_char)), self.MATCH_CLASS)

        # 2
        p = regex.compile(lower_char, regex.I | regex.U)
        self.expect(lambda: type(p.match(upper_char)), self.MATCH_CLASS)

        p = regex.compile('(?i)' + upper_char, regex.U)
        # 3
        self.expect(lambda: type(p.match(lower_char)), self.MATCH_CLASS)

        p = regex.compile('(?i)' + lower_char, regex.U)
        # 4
        self.expect(lambda: type(p.match(upper_char)), self.MATCH_CLASS)

        p = regex.compile('(?iu)' + upper_char)
        # 5
        self.expect(lambda: type(p.match(lower_char)), self.MATCH_CLASS)

        p = regex.compile('(?iu)' + lower_char)
        # 6
        self.expect(lambda: type(p.match(upper_char)), self.MATCH_CLASS)

        # 7..10
        self.expect(lambda: type(regex.match(r"(?i)a", "A")), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r"a(?i)", "A")), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r"(?iV1)a", "A")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match(r"a(?iV1)", "A"), ascii(None))

    def test_dollar_matches_twice(self):
        # $ matches the end of string, and just before the terminating \n.
        pattern = regex.compile('$')
        # 1..3
        self.expect(lambda: pattern.sub('#', 'a\nb\n'), ascii('a\nb#\n#'))
        self.expect(lambda: pattern.sub('#', 'a\nb\nc'), ascii('a\nb\nc#'))
        self.expect(lambda: pattern.sub('#', '\n'), ascii('#\n#'))

        pattern = regex.compile('$', regex.MULTILINE)
        # 4..6
        self.expect(lambda: pattern.sub('#', 'a\nb\n' ), ascii('a#\nb#\n#'))
        self.expect(lambda: pattern.sub('#', 'a\nb\nc'), ascii('a#\nb#\nc#'))
        self.expect(lambda: pattern.sub('#', '\n'), ascii('#\n#'))

    def test_bytes_str_mixing(self):
        # Mixing str and bytes is disallowed.
        pat = regex.compile('.')
        bpat = regex.compile(b'.')
        # 1..8
        self.expect(lambda: pat.match(b'b'), self.STR_PAT_ON_BYTES)
        self.expect(lambda: bpat.match('b'), self.BYTES_PAT_ON_STR)
        self.expect(lambda: pat.sub(b'b', 'c'), self.STR_PAT_BYTES_TEMPL)
        self.expect(lambda: pat.sub('b', b'c'), self.STR_PAT_ON_BYTES)
        self.expect(lambda: pat.sub(b'b', b'c'), self.STR_PAT_ON_BYTES)
        self.expect(lambda: bpat.sub(b'b', 'c'), self.BYTES_PAT_ON_STR)
        self.expect(lambda: bpat.sub('b', b'c'), self.BYTES_PAT_STR_TEMPL)
        self.expect(lambda: bpat.sub('b', 'c'), self.BYTES_PAT_ON_STR)

        # 9..14
        self.expect(lambda: regex.compile(b'\w', regex.UNICODE),
          self.BYTES_PAT_UNI_FLAG)
        self.expect(lambda: regex.compile(b'(?u)\w'), self.BYTES_PAT_UNI_FLAG)
        self.expect(lambda: regex.compile('\w', regex.UNICODE | regex.ASCII),
          self.MIXED_FLAGS)
        self.expect(lambda: regex.compile('(?u)\w', regex.ASCII),
          self.MIXED_FLAGS)
        self.expect(lambda: regex.compile('(?a)\w', regex.UNICODE),
          self.MIXED_FLAGS)
        self.expect(lambda: regex.compile('(?au)\w'), self.MIXED_FLAGS)

    def test_ascii_and_unicode_flag(self):
        # String patterns.
        # 1..4
        for flags in (0, regex.UNICODE):
            pat = regex.compile('\xc0', flags | regex.IGNORECASE)
            self.expect(lambda: type(pat.match('\xe0')), self.MATCH_CLASS)
            pat = regex.compile('\w', flags)
            self.expect(lambda: type(pat.match('\xe0')), self.MATCH_CLASS)

        pat = regex.compile('\xc0', regex.ASCII | regex.IGNORECASE)
        # 5..8
        self.expect(lambda: pat.match('\xe0'), ascii(None))
        pat = regex.compile('(?a)\xc0', regex.IGNORECASE)
        self.expect(lambda: pat.match('\xe0'), ascii(None))
        pat = regex.compile('\w', regex.ASCII)
        self.expect(lambda: pat.match('\xe0'), ascii(None))
        pat = regex.compile('(?a)\w')
        self.expect(lambda: pat.match('\xe0'), ascii(None))

        # Bytes patterns.
        # 9..12
        for flags in (0, regex.ASCII):
            pat = regex.compile(b'\xc0', flags | regex.IGNORECASE)
            self.expect(lambda: pat.match(b'\xe0'), ascii(None))
            pat = regex.compile(b'\w')
            self.expect(lambda: pat.match(b'\xe0'), ascii(None))

        # 13
        self.expect(lambda: regex.compile('(?au)\w'), self.MIXED_FLAGS)

    def test_subscripting_match(self):
        self.index += 1
        m = regex.match(r'(?<a>\w)', 'xy')
        if not m:
            self.record_failure("Failed: expected match but returned None")
        elif not m or m[0] != m.group(0) or m[1] != m.group(1):
            self.record_failure("Failed")
        self.index += 1
        if not m:
            self.record_failure("Failed: expected match but returned None")
        elif m[:] != ('x', 'x'):
            self.record_failure("Failed: expected \"('x', 'x')\" but got {} instead".format(ascii(m[:])))

    def test_new_named_groups(self):
        self.index += 1
        m0 = regex.match(r'(?P<a>\w)', 'x')
        m1 = regex.match(r'(?<a>\w)', 'x')
        if not (m0 and m1 and m0[:] == m1[:]):
            self.record_failure("Failed")

    def test_properties(self):
        # 1..4
        self.expect(lambda: regex.match(b'(?ai)\xC0', b'\xE0'), ascii(None))
        self.expect(lambda: regex.match(br'(?ai)\xC0', b'\xE0'), ascii(None))
        self.expect(lambda: regex.match(br'(?a)\w', b'\xE0'), ascii(None))
        self.expect(lambda: type(regex.match(r'\w', '\xE0')), self.MATCH_CLASS)

        # 5
        self.expect(lambda: regex.match(br'(?L)\w', b'\xE0'), ascii(None))

        # 6..11
        self.expect(lambda: type(regex.match(br'(?L)\d', b'0')),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(br'(?L)\s', b' ')),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(br'(?L)\w', b'a')),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match(br'(?L)\d', b'?'), ascii(None))
        self.expect(lambda: regex.match(br'(?L)\s', b'?'), ascii(None))
        self.expect(lambda: regex.match(br'(?L)\w', b'?'), ascii(None))

        # 12..17
        self.expect(lambda: regex.match(br'(?L)\D', b'0'), ascii(None))
        self.expect(lambda: regex.match(br'(?L)\S', b' '), ascii(None))
        self.expect(lambda: regex.match(br'(?L)\W', b'a'), ascii(None))
        self.expect(lambda: type(regex.match(br'(?L)\D', b'?')),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(br'(?L)\S', b'?')),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(br'(?L)\W', b'?')),
          self.MATCH_CLASS)

        # 18..27
        self.expect(lambda: type(regex.match(r'\p{Cyrillic}',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{IsCyrillic}',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{Script=Cyrillic}',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{InCyrillic}',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{Block=Cyrillic}',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:Cyrillic:]]',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:IsCyrillic:]]',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:Script=Cyrillic:]]',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:InCyrillic:]]',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:Block=Cyrillic:]]',
          '\N{CYRILLIC CAPITAL LETTER A}')), self.MATCH_CLASS)

        # 28..42
        self.expect(lambda: type(regex.match(r'\P{Cyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\P{IsCyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\P{Script=Cyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\P{InCyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\P{Block=Cyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{^Cyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{^IsCyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{^Script=Cyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{^InCyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{^Block=Cyrillic}',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:^Cyrillic:]]',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:^IsCyrillic:]]',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:^Script=Cyrillic:]]',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:^InCyrillic:]]',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'[[:^Block=Cyrillic:]]',
          '\N{LATIN CAPITAL LETTER A}')), self.MATCH_CLASS)

        # 43..54
        self.expect(lambda: type(regex.match(r'\d', '0')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\s', ' ')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\w', 'A')), self.MATCH_CLASS)
        self.expect(lambda: regex.match(r"\d", "?"), ascii(None))
        self.expect(lambda: regex.match(r"\s", "?"), ascii(None))
        self.expect(lambda: regex.match(r"\w", "?"), ascii(None))
        self.expect(lambda: regex.match(r"\D", "0"), ascii(None))
        self.expect(lambda: regex.match(r"\S", " "), ascii(None))
        self.expect(lambda: regex.match(r"\W", "A"), ascii(None))
        self.expect(lambda: type(regex.match(r'\D', '?')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\S', '?')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\W', '?')), self.MATCH_CLASS)

        # 55..58
        self.expect(lambda: type(regex.match(r'\p{L}', 'A')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{L}', 'a')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{Lu}', 'A')),
          self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\p{Ll}', 'a')),
          self.MATCH_CLASS)

        # 59..60
        self.expect(lambda: type(regex.match(r'(?i)a', 'a')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'(?i)a', 'A')), self.MATCH_CLASS)

        # 61..63
        self.expect(lambda: type(regex.match(r'\w', '0')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\w', 'a')), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r'\w', '_')), self.MATCH_CLASS)

        # 64..68
        self.expect(lambda: regex.match(r"\X", "\xE0").span(), ascii((0, 1)))
        self.expect(lambda: regex.match(r"\X", "a\u0300").span(), ascii((0, 2)))
        self.expect(lambda: regex.findall(r"\X", "a\xE0a\u0300e\xE9e\u0301"),
          ascii(['a', '\xe0', 'a\u0300', 'e', '\xe9', 'e\u0301']))
        self.expect(lambda: regex.findall(r"\X{3}",
          "a\xE0a\u0300e\xE9e\u0301"), ascii(['a\xe0a\u0300', 'e\xe9e\u0301']))
        self.expect(lambda: regex.findall(r"\X", "\r\r\n\u0301A\u0301"),
          ascii(['\r', '\r\n', '\u0301', 'A\u0301']))

        # 69
        self.expect(lambda: type(regex.match(r'\p{Ll}', 'a')),
          self.MATCH_CLASS)

        chars_u = "-09AZaz_\u0393\u03b3"
        chars_b = b"-09AZaz_"
        word_set = set("Ll Lm Lo Lt Lu Mc Me Mn Nd Nl No Pc".split())

        tests = [
            (r"\w", chars_u, "09AZaz_\u0393\u03b3"),
            (r"[[:word:]]", chars_u, "09AZaz_\u0393\u03b3"),
            (r"\W", chars_u, "-"),
            (r"[[:^word:]]", chars_u, "-"),
            (r"\d", chars_u, "09"),
            (r"[[:digit:]]", chars_u, "09"),
            (r"\D", chars_u, "-AZaz_\u0393\u03b3"),
            (r"[[:^digit:]]", chars_u, "-AZaz_\u0393\u03b3"),
            (r"[[:alpha:]]", chars_u, "AZaz\u0393\u03b3"),
            (r"[[:^alpha:]]", chars_u, "-09_"),
            (r"[[:alnum:]]", chars_u, "09AZaz\u0393\u03b3"),
            (r"[[:^alnum:]]", chars_u, "-_"),
            (r"[[:xdigit:]]", chars_u, "09Aa"),
            (r"[[:^xdigit:]]", chars_u, "-Zz_\u0393\u03b3"),
            (r"\p{InBasicLatin}", "a\xE1", "a"),
            (r"\P{InBasicLatin}", "a\xE1", "\xE1"),
            (r"(?i)\p{InBasicLatin}", "a\xE1", "a"),
            (r"(?i)\P{InBasicLatin}", "a\xE1", "\xE1"),

            (br"(?L)\w", chars_b, b"09AZaz_"),
            (br"(?L)[[:word:]]", chars_b, b"09AZaz_"),
            (br"(?L)\W", chars_b, b"-"),
            (br"(?L)[[:^word:]]", chars_b, b"-"),
            (br"(?L)\d", chars_b, b"09"),
            (br"(?L)[[:digit:]]", chars_b, b"09"),
            (br"(?L)\D", chars_b, b"-AZaz_"),
            (br"(?L)[[:^digit:]]", chars_b, b"-AZaz_"),
            (br"(?L)[[:alpha:]]", chars_b, b"AZaz"),
            (br"(?L)[[:^alpha:]]", chars_b, b"-09_"),
            (br"(?L)[[:alnum:]]", chars_b, b"09AZaz"),
            (br"(?L)[[:^alnum:]]", chars_b, b"-_"),
            (br"(?L)[[:xdigit:]]", chars_b, b"09Aa"),
            (br"(?L)[[:^xdigit:]]", chars_b, b"-Zz_"),

            (br"(?a)\w", chars_b, b"09AZaz_"),
            (br"(?a)[[:word:]]", chars_b, b"09AZaz_"),
            (br"(?a)\W", chars_b, b"-"),
            (br"(?a)[[:^word:]]", chars_b, b"-"),
            (br"(?a)\d", chars_b, b"09"),
            (br"(?a)[[:digit:]]", chars_b, b"09"),
            (br"(?a)\D", chars_b, b"-AZaz_"),
            (br"(?a)[[:^digit:]]", chars_b, b"-AZaz_"),
            (br"(?a)[[:alpha:]]", chars_b, b"AZaz"),
            (br"(?a)[[:^alpha:]]", chars_b, b"-09_"),
            (br"(?a)[[:alnum:]]", chars_b, b"09AZaz"),
            (br"(?a)[[:^alnum:]]", chars_b, b"-_"),
            (br"(?a)[[:xdigit:]]", chars_b, b"09Aa"),
            (br"(?a)[[:^xdigit:]]", chars_b, b"-Zz_"),
        ]
        for pattern, chars, expected in tests:
            self.index += 1
            try:
                if chars[ : 0].join(regex.findall(pattern, chars)) != expected:
                    self.record_failure("Failed: {}".format(pattern))
            except Exception as e:
                self.record_failure("Failed: {} raised {}".format(pattern,
                  ascii(e)))

    def test_word_class(self):
        # 1..4
        self.expect(lambda: regex.findall(r"\w+",
          " \u0939\u093f\u0928\u094d\u0926\u0940,"),
          ascii(['\u0939\u093f\u0928\u094d\u0926\u0940']))
        self.expect(lambda: regex.findall(r"\W+",
          " \u0939\u093f\u0928\u094d\u0926\u0940,"), ascii([' ', ',']))
        self.expect(lambda: regex.split(r"(?V1)\b",
          " \u0939\u093f\u0928\u094d\u0926\u0940,"), ascii([' ',
          '\u0939\u093f\u0928\u094d\u0926\u0940', ',']))
        self.expect(lambda: regex.split(r"(?V1)\B",
          " \u0939\u093f\u0928\u094d\u0926\u0940,"), ascii(['', ' \u0939',
          '\u093f', '\u0928', '\u094d', '\u0926', '\u0940,', '']))

    def test_search_anchor(self):
        self.expect(lambda: regex.findall(r"\G\w{2}", "abcd ef"), ascii(['ab',
          'cd']))

    def test_search_reverse(self):
        # 1..5
        self.expect(lambda: regex.findall(r"(?r).", "abc"), ascii(['c', 'b',
          'a']))
        self.expect(lambda: regex.findall(r"(?r).", "abc", overlapped=True),
          ascii(['c', 'b', 'a']))
        self.expect(lambda: regex.findall(r"(?r)..", "abcde"), ascii(['de',
          'bc']))
        self.expect(lambda: regex.findall(r"(?r)..", "abcde", overlapped=True),
          ascii(['de', 'cd', 'bc', 'ab']))
        self.expect(lambda: regex.findall(r"(?r)(.)(-)(.)", "a-b-c",
          overlapped=True), ascii([("b", "-", "c"), ("a", "-", "b")]))

        # 6..9
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).", "abc")],
          ascii(['c', 'b', 'a']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde",
          overlapped=True)], ascii(['de', 'cd', 'bc', 'ab']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).", "abc")],
         ascii(['c', 'b', 'a']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde",
          overlapped=True)],ascii(['de', 'cd', 'bc', 'ab']))

        # 10..13
        self.expect(lambda: regex.findall(r"^|\w+", "foo bar"), ascii(['',
          'foo', 'bar']))
        self.expect(lambda: regex.findall(r"(?V1)^|\w+", "foo bar"), ascii(['',
          'foo', 'bar']))
        self.expect(lambda: regex.findall(r"(?r)^|\w+", "foo bar"),
          ascii(['bar', 'foo', '']))
        self.expect(lambda: regex.findall(r"(?rV1)^|\w+", "foo bar"),
          ascii(['bar', 'foo', '']))

        # 14..17
        self.expect(lambda: [m[0] for m in regex.finditer(r"^|\w+",
          "foo bar")], ascii(['', 'foo', 'bar']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?V1)^|\w+",
          "foo bar")], ascii(['', 'foo', 'bar']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)^|\w+",
          "foo bar")], ascii(['bar', 'foo', '']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?rV1)^|\w+",
          "foo bar")], ascii(['bar', 'foo', '']))

        # 18..21
        self.expect(lambda: regex.findall(r"\G\w{2}", "abcd ef"), ascii(['ab',
          'cd']))
        self.expect(lambda: regex.findall(r".{2}(?<=\G.*)", "abcd"),
          ascii(['ab', 'cd']))
        self.expect(lambda: regex.findall(r"(?r)\G\w{2}", "abcd ef"), ascii([]))
        self.expect(lambda: regex.findall(r"(?r)\w{2}\G", "abcd ef"),
          ascii(['ef']))

        # 22..25
        self.expect(lambda: regex.findall(r"q*", "qqwe"), ascii(['qq', '', '',
          '']))
        self.expect(lambda: regex.findall(r"(?V1)q*", "qqwe"), ascii(['qq', '',
          '', '']))
        self.expect(lambda: regex.findall(r"(?r)q*", "qqwe"), ascii(['', '',
          'qq', '']))
        self.expect(lambda: regex.findall(r"(?rV1)q*", "qqwe"), ascii(['', '',
          'qq', '']))

        # 26..29
        self.expect(lambda: regex.findall(".", "abcd", pos=1, endpos=3),
          ascii(['b', 'c']))
        self.expect(lambda: regex.findall(".", "abcd", pos=1, endpos=-1),
          ascii(['b', 'c']))
        self.expect(lambda: [m[0] for m in regex.finditer(".", "abcd", pos=1,
          endpos=3)], ascii(['b', 'c']))
        self.expect(lambda: [m[0] for m in regex.finditer(".", "abcd", pos=1,
          endpos=-1)], ascii(['b', 'c']))

        # 30..33
        self.expect(lambda: [m[0] for m in regex.finditer("(?r).", "abcd",
          pos=1, endpos=3)], ascii(['c', 'b']))
        self.expect(lambda: [m[0] for m in regex.finditer("(?r).", "abcd",
          pos=1, endpos=-1)], ascii(['c', 'b']))
        self.expect(lambda: regex.findall("(?r).", "abcd", pos=1, endpos=3),
          ascii(['c', 'b']))
        self.expect(lambda: regex.findall("(?r).", "abcd", pos=1, endpos=-1),
          ascii(['c', 'b']))

        # 34..35
        self.expect(lambda: regex.findall(r"[ab]", "aB", regex.I), ascii(['a',
          'B']))
        self.expect(lambda: regex.findall(r"(?r)[ab]", "aB", regex.I),
          ascii(['B', 'a']))

        # 36..39
        self.expect(lambda: regex.findall(r"(?r).{2}", "abc"), ascii(['bc']))
        self.expect(lambda: regex.findall(r"(?r).{2}", "abc", overlapped=True),
          ascii(['bc', 'ab']))
        self.expect(lambda: regex.findall(r"(\w+) (\w+)",
          "first second third fourth fifth"), ascii([('first', 'second'),
          ('third', 'fourth')]))
        self.expect(lambda: regex.findall(r"(?r)(\w+) (\w+)",
          "first second third fourth fifth"), ascii([('fourth', 'fifth'),
          ('second', 'third')]))

        # 40..43
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).{2}", "abc")],
          ascii(['bc']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).{2}", "abc",
          overlapped=True)], ascii(['bc', 'ab']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(\w+) (\w+)",
          "first second third fourth fifth")], ascii(['first second',
          'third fourth']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)(\w+) (\w+)",
          "first second third fourth fifth")], ascii(['fourth fifth',
          'second third']))

        # 44..47
        self.expect(lambda: regex.search("abcdef", "abcdef").span(), ascii((0,
          6)))
        self.expect(lambda: regex.search("(?r)abcdef", "abcdef").span(),
          ascii((0, 6)))
        self.expect(lambda: regex.search("(?i)abcdef", "ABCDEF").span(),
          ascii((0, 6)))
        self.expect(lambda: regex.search("(?ir)abcdef", "ABCDEF").span(),
          ascii((0, 6)))

        # 48..49
        self.expect(lambda: regex.sub(r"(.)", r"\1", "abc"), ascii('abc'))
        self.expect(lambda: regex.sub(r"(?r)(.)", r"\1", "abc"), ascii('abc'))

    def test_atomic(self):
        # Issue 433030.
        self.expect(lambda: regex.search(r"(?>a*)a", "aa"), ascii(None))

    def test_possessive(self):
        # Single-character non-possessive.
        # 1..4
        self.expect(lambda: regex.search(r"a?a", "a").span(), ascii((0, 1)))
        self.expect(lambda: regex.search(r"a*a", "aaa").span(), ascii((0, 3)))
        self.expect(lambda: regex.search(r"a+a", "aaa").span(), ascii((0, 3)))
        self.expect(lambda: regex.search(r"a{1,3}a", "aaa").span(), ascii((0,
          3)))

        # Multiple-character non-possessive.
        # 5..8
        self.expect(lambda: regex.search(r"(?:ab)?ab", "ab").span(), ascii((0,
          2)))
        self.expect(lambda: regex.search(r"(?:ab)*ab", "ababab").span(),
          ascii((0, 6)))
        self.expect(lambda: regex.search(r"(?:ab)+ab", "ababab").span(),
          ascii((0, 6)))
        self.expect(lambda: regex.search(r"(?:ab){1,3}ab", "ababab").span(),
          ascii((0, 6)))

        # Single-character possessive.
        # 9..12
        self.expect(lambda: regex.search(r"a?+a", "a"), ascii(None))
        self.expect(lambda: regex.search(r"a*+a", "aaa"), ascii(None))
        self.expect(lambda: regex.search(r"a++a", "aaa"), ascii(None))
        self.expect(lambda: regex.search(r"a{1,3}+a", "aaa"), ascii(None))

        # Multiple-character possessive.
        # 13..16
        self.expect(lambda: regex.search(r"(?:ab)?+ab", "ab"), ascii(None))
        self.expect(lambda: regex.search(r"(?:ab)*+ab", "ababab"), ascii(None))
        self.expect(lambda: regex.search(r"(?:ab)++ab", "ababab"), ascii(None))
        self.expect(lambda: regex.search(r"(?:ab){1,3}+ab", "ababab"),
          ascii(None))

    def test_zerowidth(self):
        # Issue 3262.
        # 1..2
        self.expect(lambda: regex.split(r"\b", "a b"), ascii(['a b']))
        self.expect(lambda: regex.split(r"(?V1)\b", "a b"), ascii(['', 'a', ' ',
          'b', '']))

        # Issue 1647489.
        # 3..10
        self.expect(lambda: regex.findall(r"^|\w+", "foo bar"), ascii(['',
          'foo', 'bar']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"^|\w+",
          "foo bar")], ascii(['', 'foo', 'bar']))
        self.expect(lambda: regex.findall(r"(?r)^|\w+", "foo bar"),
          ascii(['bar', 'foo', '']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)^|\w+",
          "foo bar")], ascii(['bar', 'foo', '']))
        self.expect(lambda: regex.findall(r"(?V1)^|\w+", "foo bar"), ascii(['',
          'foo', 'bar']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?V1)^|\w+",
          "foo bar")], ascii(['', 'foo', 'bar']))
        self.expect(lambda: regex.findall(r"(?rV1)^|\w+", "foo bar"),
          ascii(['bar', 'foo', '']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?rV1)^|\w+",
          "foo bar")], ascii(['bar', 'foo', '']))

        # 11..12
        self.expect(lambda: regex.split("", "xaxbxc"), ascii(['xaxbxc']))
        self.expect(lambda: [m for m in regex.splititer("", "xaxbxc")],
          ascii(['xaxbxc']))

        # 13..14
        self.expect(lambda: regex.split("(?r)", "xaxbxc"), ascii(['xaxbxc']))
        self.expect(lambda: [m for m in regex.splititer("(?r)", "xaxbxc")],
          ascii(['xaxbxc']))

        # 15..16
        self.expect(lambda: regex.split("(?V1)", "xaxbxc"), ascii(['', 'x', 'a',
          'x', 'b', 'x', 'c', '']))
        self.expect(lambda: [m for m in regex.splititer("(?V1)", "xaxbxc")],
          ascii(['', 'x', 'a', 'x', 'b', 'x', 'c', '']))

        # 17..18
        self.expect(lambda: regex.split("(?rV1)", "xaxbxc"), ascii(['', 'c',
          'x', 'b', 'x', 'a', 'x', '']))
        self.expect(lambda: [m for m in regex.splititer("(?rV1)", "xaxbxc")],
          ascii(['', 'c', 'x', 'b', 'x', 'a', 'x', '']))

    def test_scoped_and_inline_flags(self):
        # Issues 433028, #433024, #433027.
        # 1..4
        self.expect(lambda: regex.search(r"(?i)Ab", "ab").span(), ascii((0, 2)))
        self.expect(lambda: regex.search(r"(?i:A)b", "ab").span(), ascii((0,
          2)))
        self.expect(lambda: regex.search(r"A(?i)b", "ab").span(), ascii((0, 2)))
        self.expect(lambda: regex.search(r"A(?iV1)b", "ab"), ascii(None))

        # 5..8
        self.expect(lambda: regex.search(r"(?V0-i)Ab", "ab",
          flags=regex.I), self.CANT_TURN_OFF)
        self.expect(lambda: regex.search(r"(?V1-i)Ab", "ab", flags=regex.I),
          ascii(None))
        self.expect(lambda: regex.search(r"(?-i:A)b", "ab", flags=regex.I),
          ascii(None))
        self.expect(lambda: regex.search(r"A(?V1-i)b", "ab",
          flags=regex.I).span(), ascii((0, 2)))

    def test_repeated_repeats(self):
        # Issue 2537.
        # 1..2
        self.expect(lambda: regex.search(r"(?:a+)+", "aaa").span(), ascii((0,
          3)))
        self.expect(lambda: regex.search(r"(?:(?:ab)+c)+", "abcabc").span(),
          ascii((0, 6)))

    def test_lookbehind(self):
        # 1..4
        self.expect(lambda: regex.search(r"123(?<=a\d+)", "a123").span(),
          ascii((1, 4)))
        self.expect(lambda: regex.search(r"123(?<=a\d+)", "b123"), ascii(None))
        self.expect(lambda: regex.search(r"123(?<!a\d+)", "a123"), ascii(None))
        self.expect(lambda: regex.search(r"123(?<!a\d+)", "b123").span(),
          ascii((1, 4)))

        # 5..8
        self.expect(lambda: type(regex.match("(a)b(?<=b)(c)", "abc")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match("(a)b(?<=c)(c)", "abc"), ascii(None))
        self.expect(lambda: type(regex.match("(a)b(?=c)(c)", "abc")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match("(a)b(?=b)(c)", "abc"), ascii(None))

        # 9..13
        self.expect(lambda: regex.match("(?:(a)|(x))b(?<=(?(2)x|c))c", "abc"),
          ascii(None))
        self.expect(lambda: regex.match("(?:(a)|(x))b(?<=(?(2)b|x))c", "abc"),
          ascii(None))
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?<=(?(2)x|b))c",
          "abc")), self.MATCH_CLASS)
        self.expect(lambda: regex.match("(?:(a)|(x))b(?<=(?(1)c|x))c", "abc"),
          ascii(None))
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?<=(?(1)b|x))c",
          "abc")), self.MATCH_CLASS)

        # 14..18
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?=(?(2)x|c))c",
          "abc")), self.MATCH_CLASS)
        self.expect(lambda: regex.match("(?:(a)|(x))b(?=(?(2)c|x))c", "abc"),
          ascii(None))
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?=(?(2)x|c))c",
          "abc")), self.MATCH_CLASS)
        self.expect(lambda: regex.match("(?:(a)|(x))b(?=(?(1)b|x))c", "abc"),
          ascii(None))
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?=(?(1)c|x))c",
          "abc")), self.MATCH_CLASS)

        # 19..22
        self.expect(lambda: regex.match("(a)b(?<=(?(2)x|c))(c)", "abc"),
          ascii(None))
        self.expect(lambda: regex.match("(a)b(?<=(?(2)b|x))(c)", "abc"),
          ascii(None))
        self.expect(lambda: regex.match("(a)b(?<=(?(1)c|x))(c)", "abc"),
          ascii(None))
        self.expect(lambda: type(regex.match("(a)b(?<=(?(1)b|x))(c)", "abc")),
          self.MATCH_CLASS)

        # 23..25
        self.expect(lambda: type(regex.match("(a)b(?=(?(2)x|c))(c)", "abc")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match("(a)b(?=(?(2)b|x))(c)", "abc"),
          ascii(None))
        self.expect(lambda: type(regex.match("(a)b(?=(?(1)c|x))(c)", "abc")),
          self.MATCH_CLASS)

        # 26
        self.expect(lambda: type(regex.compile(r"(a)\2(b)")),
          self.PATTERN_CLASS)

    def test_unmatched_in_sub(self):
        # Issue 1519638.
        # 1..3
        self.expect(lambda: regex.sub(r"(x)?(y)?", r"\2-\1", "xy"),
          ascii('y-x-'))
        self.expect(lambda: regex.sub(r"(x)?(y)?", r"\2-\1", "x"), ascii('-x-'))
        self.expect(lambda: regex.sub(r"(x)?(y)?", r"\2-\1", "y"), ascii('y--'))

    def test_bug_10328 (self):
        # Issue 10328.
        pat = regex.compile(r'(?m)(?P<trailing_ws>[ \t]+\r*$)|(?P<no_final_newline>(?<=[^\n])\Z)')
        # 1..2
        self.expect(lambda: pat.subn(lambda m: '<' + m.lastgroup + '>',
          'foobar '), ascii(('foobar<trailing_ws><no_final_newline>', 2)))
        self.expect(lambda: [m.group() for m in pat.finditer('foobar ')],
          ascii([' ', '']))

    def test_overlapped(self):
        # 1..5
        self.expect(lambda: regex.findall(r"..", "abcde"), ascii(['ab', 'cd']))
        self.expect(lambda: regex.findall(r"..", "abcde", overlapped=True),
          ascii(['ab', 'bc', 'cd', 'de']))
        self.expect(lambda: regex.findall(r"(?r)..", "abcde"), ascii(['de',
          'bc']))
        self.expect(lambda: regex.findall(r"(?r)..", "abcde", overlapped=True),
          ascii(['de', 'cd', 'bc', 'ab']))
        self.expect(lambda: regex.findall(r"(.)(-)(.)", "a-b-c",
          overlapped=True), ascii([("a", "-", "b"), ("b", "-", "c")]))

        # 6..9
        self.expect(lambda: [m[0] for m in regex.finditer(r"..", "abcde")],
          ascii(['ab', 'cd']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"..", "abcde",
          overlapped=True)], ascii(['ab', 'bc', 'cd', 'de']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde")],
          ascii(['de', 'bc']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde",
          overlapped=True)], ascii(['de', 'cd', 'bc', 'ab']))

        # 10..11
        self.expect(lambda: [m.groups() for m in regex.finditer(r"(.)(-)(.)",
          "a-b-c", overlapped=True)], ascii([("a", "-", "b"), ("b", "-", "c")]))
        self.expect(lambda: [m.groups() for m in
          regex.finditer(r"(?r)(.)(-)(.)", "a-b-c", overlapped=True)],
          ascii([("b", "-", "c"), ("a", "-", "b")]))

    def test_splititer(self):
        # 1..2
        self.expect(lambda: regex.split(r",", "a,b,,c,"), ascii(['a', 'b', '',
          'c', '']))
        self.expect(lambda: [m for m in regex.splititer(r",", "a,b,,c,")],
          ascii(['a', 'b', '', 'c', '']))

    def test_grapheme(self):
        # 1..2
        self.expect(lambda: regex.match(r"\X", "\xE0").span(), ascii((0, 1)))
        self.expect(lambda: regex.match(r"\X", "a\u0300").span(), ascii((0,
          2)))

        # 3..5
        self.expect(lambda: regex.findall(r"\X", "a\xE0a\u0300e\xE9e\u0301"),
          ascii(['a', '\xe0', 'a\u0300', 'e', '\xe9', 'e\u0301']))
        self.expect(lambda: regex.findall(r"\X{3}",
          "a\xE0a\u0300e\xE9e\u0301"), ascii(['a\xe0a\u0300', 'e\xe9e\u0301']))
        self.expect(lambda: regex.findall(r"\X", "\r\r\n\u0301A\u0301"),
          ascii(['\r', '\r\n', '\u0301', 'A\u0301']))

    def test_word_boundary(self):
        text = 'The quick ("brown") fox can\'t jump 32.3 feet, right?'
        # 1..2
        self.expect(lambda: regex.split(r'(?V1)\b', text), ascii(['', 'The',
          ' ', 'quick', ' ("', 'brown', '") ', 'fox', ' ', 'can', "'", 't',
          ' ', 'jump', ' ', '32', '.', '3', ' ', 'feet', ', ', 'right', '?']))
        self.expect(lambda: regex.split(r'(?V1w)\b', text), ascii(['', 'The',
          ' ', 'quick', ' ', '(', '"', 'brown', '"', ')', ' ', 'fox', ' ',
          "can't", ' ', 'jump', ' ', '32.3', ' ', 'feet', ',', ' ', 'right',
          '?', '']))

        text = "The  fox"
        # 3..4
        self.expect(lambda: regex.split(r'(?V1)\b', text), ascii(['', 'The',
          '  ', 'fox', '']))
        self.expect(lambda: regex.split(r'(?V1w)\b', text), ascii(['', 'The',
          ' ', ' ', 'fox', '']))

        text = "can't aujourd'hui l'objectif"
        # 5..6
        self.expect(lambda: regex.split(r'(?V1)\b', text), ascii(['', 'can',
          "'", 't', ' ', 'aujourd', "'", 'hui', ' ', 'l', "'", 'objectif',
          '']))
        self.expect(lambda: regex.split(r'(?V1w)\b', text), ascii(['', "can't",
          ' ', "aujourd'hui", ' ', "l'", 'objectif', '']))

    def test_line_boundary(self):
        # 1..6
        self.expect(lambda: regex.findall(r".+", "Line 1\nLine 2\n"),
          ascii(["Line 1", "Line 2"]))
        self.expect(lambda: regex.findall(r".+", "Line 1\rLine 2\r"),
          ascii(["Line 1\rLine 2\r"]))
        self.expect(lambda: regex.findall(r".+", "Line 1\r\nLine 2\r\n"),
          ascii(["Line 1\r", "Line 2\r"]))
        self.expect(lambda: regex.findall(r"(?w).+", "Line 1\nLine 2\n"),
          ascii(["Line 1", "Line 2"]))
        self.expect(lambda: regex.findall(r"(?w).+", "Line 1\rLine 2\r"),
          ascii(["Line 1", "Line 2"]))
        self.expect(lambda: regex.findall(r"(?w).+", "Line 1\r\nLine 2\r\n"),
          ascii(["Line 1", "Line 2"]))

        # 7..12
        self.expect(lambda: regex.search(r"^abc", "abc").start(), ascii(0))
        self.expect(lambda: regex.search(r"^abc", "\nabc"), ascii(None))
        self.expect(lambda: regex.search(r"^abc", "\rabc"), ascii(None))
        self.expect(lambda: regex.search(r"(?w)^abc", "abc").start(), ascii(0))
        self.expect(lambda: regex.search(r"(?w)^abc", "\nabc"), ascii(None))
        self.expect(lambda: regex.search(r"(?w)^abc", "\rabc"), ascii(None))

        # 13..18
        self.expect(lambda: regex.search(r"abc$", "abc").start(), ascii(0))
        self.expect(lambda: regex.search(r"abc$", "abc\n").start(), ascii(0))
        self.expect(lambda: regex.search(r"abc$", "abc\r"), ascii(None))
        self.expect(lambda: regex.search(r"(?w)abc$", "abc").start(), ascii(0))
        self.expect(lambda: regex.search(r"(?w)abc$", "abc\n").start(),
          ascii(0))
        self.expect(lambda: regex.search(r"(?w)abc$", "abc\r").start(),
          ascii(0))

        # 19..24
        self.expect(lambda: regex.search(r"(?m)^abc", "abc").start(), ascii(0))
        self.expect(lambda: regex.search(r"(?m)^abc", "\nabc").start(),
          ascii(1))
        self.expect(lambda: regex.search(r"(?m)^abc", "\rabc"), ascii(None))
        self.expect(lambda: regex.search(r"(?mw)^abc", "abc").start(), ascii(0))
        self.expect(lambda: regex.search(r"(?mw)^abc", "\nabc").start(),
          ascii(1))
        self.expect(lambda: regex.search(r"(?mw)^abc", "\rabc").start(),
          ascii(1))

        # 25..30
        self.expect(lambda: regex.search(r"(?m)abc$", "abc").start(), ascii(0))
        self.expect(lambda: regex.search(r"(?m)abc$", "abc\n").start(),
          ascii(0))
        self.expect(lambda: regex.search(r"(?m)abc$", "abc\r"), ascii(None))
        self.expect(lambda: regex.search(r"(?mw)abc$", "abc").start(),
          ascii(0))
        self.expect(lambda: regex.search(r"(?mw)abc$", "abc\n").start(),
          ascii(0))
        self.expect(lambda: regex.search(r"(?mw)abc$", "abc\r").start(),
          ascii(0))

    def test_branch_reset(self):
        # 1..4
        self.expect(lambda: regex.match(r"(?:(a)|(b))(c)", "ac").groups(),
          ascii(('a', None, 'c')))
        self.expect(lambda: regex.match(r"(?:(a)|(b))(c)", "bc").groups(),
          ascii((None, 'b', 'c')))
        self.expect(lambda: regex.match(r"(?:(?<a>a)|(?<b>b))(?<c>c)",
          "ac").groups(), ascii(('a', None, 'c')))
        self.expect(lambda: regex.match(r"(?:(?<a>a)|(?<b>b))(?<c>c)",
          "bc").groups(), ascii((None, 'b', 'c')))

        # 5..7
        self.expect(lambda: regex.match(r"(?<a>a)(?:(?<b>b)|(?<c>c))(?<d>d)",
          "abd").groups(), ascii(('a', 'b', None, 'd')))
        self.expect(lambda: regex.match(r"(?<a>a)(?:(?<b>b)|(?<c>c))(?<d>d)",
          "acd").groups(), ascii(('a', None, 'c', 'd')))
        self.expect(lambda: regex.match(r"(a)(?:(b)|(c))(d)", "abd").groups(),
         ascii(('a', 'b', None, 'd')))

        # 8..12
        self.expect(lambda: regex.match(r"(a)(?:(b)|(c))(d)", "acd").groups(),
          ascii(('a', None, 'c', 'd')))
        self.expect(lambda: regex.match(r"(a)(?|(b)|(b))(d)", "abd").groups(),
          ascii(('a', 'b', 'd')))
        self.expect(lambda: regex.match(r"(?|(?<a>a)|(?<b>b))(c)",
          "ac").groups(), ascii(('a', None, 'c')))
        self.expect(lambda: regex.match(r"(?|(?<a>a)|(?<b>b))(c)",
          "bc").groups(), ascii((None, 'b', 'c')))
        self.expect(lambda: regex.match(r"(?|(?<a>a)|(?<a>b))(c)",
          "ac").groups(), ascii(('a', 'c')))

        # 13
        self.expect(lambda: regex.match(r"(?|(?<a>a)|(?<a>b))(c)",
          "bc").groups(), ascii(('b', 'c')))

        # 14..21
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(?<a>d))(e)", "abe").groups(),
          ascii(('a', 'b', 'e')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(?<a>d))(e)", "cde").groups(),
          ascii(('d', 'c', 'e')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(d))(e)", "abe").groups(),
          ascii(('a', 'b', 'e')))
        self.expect(lambda: regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(d))(e)",
          "cde").groups(), ascii(('d', 'c', 'e')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(c)(d))(e)", "abe").groups(),
          ascii(('a', 'b', 'e')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(c)(d))(e)", "cde").groups(),
          ascii(('c', 'd', 'e')))
        self.expect(lambda: regex.match(r"(?|(?<a>a)(?<b>b)|(c)(?<a>d))(e)",
          "abe"), self.DUPLICATE_GROUP)
        self.expect(lambda: regex.match(r"(?|(?<a>a)(?<b>b)|(c)(?<a>d))(e)",
          "cde"), self.DUPLICATE_GROUP)

    def test_set(self):
        # 1..4
        self.expect(lambda: regex.match(r"[a]", "a").span(), ascii((0, 1)))
        self.expect(lambda: regex.match(r"(?i)[a]", "A").span(), ascii((0, 1)))
        self.expect(lambda: regex.match(r"[a-b]", r"a").span(), ascii((0, 1)))
        self.expect(lambda: regex.match(r"(?i)[a-b]", r"A").span(), ascii((0,
          1)))

        # 5
        self.expect(lambda: regex.sub(r"(?V0)([][])", r"-", "a[b]c"),
          ascii("a-b-c"))

        # 6..7
        self.expect(lambda: regex.findall(r"[\p{Alpha}]", "a0"), ascii(["a"]))
        self.expect(lambda: regex.findall(r"(?i)[\p{Alpha}]", "A0"),
          ascii(["A"]))

        # 8..11
        self.expect(lambda: regex.findall(r"[a\p{Alpha}]", "ab0"), ascii(["a",
          "b"]))
        self.expect(lambda: regex.findall(r"[a\P{Alpha}]", "ab0"), ascii(["a",
          "0"]))
        self.expect(lambda: regex.findall(r"(?i)[a\p{Alpha}]", "ab0"),
          ascii(["a", "b"]))
        self.expect(lambda: regex.findall(r"(?i)[a\P{Alpha}]", "ab0"),
          ascii(["a", "0"]))

        # 12..13
        self.expect(lambda: regex.findall(r"[a-b\p{Alpha}]", "abC0"),
          ascii(["a", "b", "C"]))
        self.expect(lambda: regex.findall(r"(?i)[a-b\p{Alpha}]", "AbC0"),
          ascii(["A", "b", "C"]))

        # 14..17
        self.expect(lambda: regex.findall(r"[\p{Alpha}]", "a0"), ascii(["a"]))
        self.expect(lambda: regex.findall(r"[\P{Alpha}]", "a0"), ascii(["0"]))
        self.expect(lambda: regex.findall(r"[^\p{Alpha}]", "a0"), ascii(["0"]))
        self.expect(lambda: regex.findall(r"[^\P{Alpha}]", "a0"), ascii(["a"]))

        # 18..23
        self.expect(lambda: "".join(regex.findall(r"[^\d-h]", "a^b12c-h")),
          ascii('a^bc'))
        self.expect(lambda: "".join(regex.findall(r"[^\dh]", "a^b12c-h")),
          ascii('a^bc-'))
        self.expect(lambda: "".join(regex.findall(r"[^h\s\db]", "a^b 12c-h")),
          ascii('a^c-'))
        self.expect(lambda: "".join(regex.findall(r"[^b\w]", "a b")),
          ascii(' '))
        self.expect(lambda: "".join(regex.findall(r"[^b\S]", "a b")),
          ascii(' '))
        self.expect(lambda: "".join(regex.findall(r"[^8\d]", "a 1b2")),
          ascii('a b'))

        all_chars = "".join(chr(c) for c in range(0x100))
        # 24..26
        self.expect(lambda: len(regex.findall(r"\p{ASCII}", all_chars)),
          ascii(128))
        self.expect(lambda: len(regex.findall(r"\p{Letter}", all_chars)),
          ascii(117))
        self.expect(lambda: len(regex.findall(r"\p{Digit}", all_chars)),
          ascii(10))

        # Set operators
        # 27..37
        self.expect(lambda: len(regex.findall(r"(?V1)[\p{ASCII}&&\p{Letter}]",
          all_chars)), ascii(52))
        self.expect(lambda:
          len(regex.findall(r"(?V1)[\p{ASCII}&&\p{Alnum}&&\p{Letter}]",
          all_chars)), ascii(52))
        self.expect(lambda:
         len(regex.findall(r"(?V1)[\p{ASCII}&&\p{Alnum}&&\p{Digit}]",
         all_chars)), ascii(10))
        self.expect(lambda: len(regex.findall(r"(?V1)[\p{ASCII}&&\p{Cc}]",
          all_chars)), ascii(33))
        self.expect(lambda: len(regex.findall(r"(?V1)[\p{ASCII}&&\p{Graph}]",
          all_chars)), ascii(94))
        self.expect(lambda: len(regex.findall(r"(?V1)[\p{ASCII}--\p{Cc}]",
          all_chars)), ascii(95))
        self.expect(lambda: len(regex.findall(r"[\p{Letter}\p{Digit}]",
          all_chars)), ascii(127))
        self.expect(lambda: len(regex.findall(r"(?V1)[\p{Letter}||\p{Digit}]",
          all_chars)), ascii(127))
        self.expect(lambda: len(regex.findall(r"\p{HexDigit}", all_chars)),
          ascii(22))
        self.expect(lambda:
          len(regex.findall(r"(?V1)[\p{HexDigit}~~\p{Digit}]", all_chars)),
          ascii(12))
        self.expect(lambda:
          len(regex.findall(r"(?V1)[\p{Digit}~~\p{HexDigit}]", all_chars)),
          ascii(12))

        # 38..42
        self.expect(lambda: type(regex.compile(r"(?V0)([][-])")),
          self.PATTERN_CLASS)
        self.expect(lambda: regex.findall(r"(?V1)[[a-z]--[aei]]", "abc"),
          ascii(["b", "c"]))
        self.expect(lambda: regex.findall(r"(?iV1)[[a-z]--[aei]]", "abc"),
          ascii(["b", "c"]))
        self.expect(lambda: regex.findall("(?V1)[\w--a]","abc"), ascii(["b",
          "c"]))
        self.expect(lambda: regex.findall("(?iV1)[\w--a]","abc"), ascii(["b",
          "c"]))

    def test_various(self):
        tests = [
            # Test ?P< and ?P= extensions.
            # 1..4
            ('(?P<foo_123', '', '', self.MISSING_GT),      # Unterminated group identifier.
            ('(?P<1>a)', '', '', self.BAD_GROUP_NAME),     # Begins with a digit.
            ('(?P<!>a)', '', '', self.BAD_GROUP_NAME),     # Begins with an illegal char.
            ('(?P<foo!>a)', '', '', self.MISSING_GT),      # Begins with an illegal char.

            # Same tests, for the ?P= form.
            # 5..8
            ('(?P<foo_123>a)(?P=foo_123', 'aa', '', self.MISSING_RPAREN),
            ('(?P<foo_123>a)(?P=1)', 'aa', '', self.BAD_GROUP_NAME),
            ('(?P<foo_123>a)(?P=!)', 'aa', '', self.BAD_GROUP_NAME),
            ('(?P<foo_123>a)(?P=foo_124)', 'aa', '', self.UNKNOWN_GROUP),  # Backref to undefined group.

            # 9..10
            ('(?P<foo_123>a)', 'a', '1', ascii('a')),
            ('(?P<foo_123>a)(?P=foo_123)', 'aa', '1', ascii('a')),

            # Mal-formed \g in pattern treated as literal for compatibility.
            # 11..14
            (r'(?<foo_123>a)\g<foo_123', 'aa', '', ascii(None)),
            (r'(?<foo_123>a)\g<1>', 'aa', '1', ascii('a')),
            (r'(?<foo_123>a)\g<!>', 'aa', '', ascii(None)),
            (r'(?<foo_123>a)\g<foo_124>', 'aa', '', self.UNKNOWN_GROUP),  # Backref to undefined group.

            # 15..16
            ('(?<foo_123>a)', 'a', '1', ascii('a')),
            (r'(?<foo_123>a)\g<foo_123>', 'aa', '1', ascii('a')),

            # Test octal escapes.
            # 17..21
            ('\\1', 'a', '', self.UNKNOWN_GROUP),    # Backreference.
            ('[\\1]', '\1', '0', "'\\x01'"),  # Character.
            ('\\09', chr(0) + '9', '0', ascii(chr(0) + '9')),
            ('\\141', 'a', '0', ascii('a')),
            ('(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)\\119', 'abcdefghijklk9',
              '0,11', ascii(('abcdefghijklk9', 'k'))),

            # Test \0 is handled everywhere.
            # 22..25
            (r'\0', '\0', '0', ascii('\0')),
            (r'[\0a]', '\0', '0', ascii('\0')),
            (r'[a\0]', '\0', '0', ascii('\0')),
            (r'[^a\0]', '\0', '', ascii(None)),

            # Test various letter escapes.
            # 26..29
            (r'\a[\b]\f\n\r\t\v', '\a\b\f\n\r\t\v', '0',
              ascii('\a\b\f\n\r\t\v')),
            (r'[\a][\b][\f][\n][\r][\t][\v]', '\a\b\f\n\r\t\v', '0',
              ascii('\a\b\f\n\r\t\v')),
            (r'\c\e\g\h\i\j\k\o\p\q\y\z', 'ceghijkopqyz', '0',
              ascii('ceghijkopqyz')),
            (r'\xff', '\377', '0', ascii(chr(255))),
            # New \x semantics.
            # 30..32
            (r'\x00ffffffffffffff', '\377', '', ascii(None)),
            (r'\x00f', '\017', '', ascii(None)),
            (r'\x00fe', '\376', '', ascii(None)),

            # 33..37
            (r'\x00ff', '\377', '', ascii(None)),
            (r'\t\n\v\r\f\a\g', '\t\n\v\r\f\ag', '0', ascii('\t\n\v\r\f\ag')),
            ('\t\n\v\r\f\a\g', '\t\n\v\r\f\ag', '0', ascii('\t\n\v\r\f\ag')),
            (r'\t\n\v\r\f\a', '\t\n\v\r\f\a', '0', ascii(chr(9) + chr(10) +
              chr(11) + chr(13) + chr(12) + chr(7))),
            (r'[\t][\n][\v][\r][\f][\b]', '\t\n\v\r\f\b', '0',
              ascii('\t\n\v\r\f\b')),

            # 38
            (r"^\w+=(\\[\000-\277]|[^\n\\])*",
              "SRC=eval.c g.c blah blah blah \\\\\n\tapes.c", '0',
              ascii("SRC=eval.c g.c blah blah blah \\\\")),

            # Test that . only matches \n in DOTALL mode.
            # 39..43
            ('a.b', 'acb', '0', ascii('acb')),
            ('a.b', 'a\nb', '', ascii(None)),
            ('a.*b', 'acc\nccb', '', ascii(None)),
            ('a.{4,5}b', 'acc\nccb', '', ascii(None)),
            ('a.b', 'a\rb', '0', ascii('a\rb')),
            # The new behaviour is that the inline flag affects only what follows.
            # 44..50
            ('a.b(?s)', 'a\nb', '0', ascii('a\nb')),
            ('a.b(?sV1)', 'a\nb', '', ascii(None)),
            ('(?s)a.b', 'a\nb', '0', ascii('a\nb')),
            ('a.*(?s)b', 'acc\nccb', '0', ascii('acc\nccb')),
            ('a.*(?sV1)b', 'acc\nccb', '', ascii(None)),
            ('(?s)a.*b', 'acc\nccb', '0', ascii('acc\nccb')),
            ('(?s)a.{4,5}b', 'acc\nccb', '0', ascii('acc\nccb')),

            # 51..60
            (')', '', '', self.TRAILING_CHARS),           # Unmatched right bracket.
            ('', '', '0', "''"),    # Empty pattern.
            ('abc', 'abc', '0', ascii('abc')),
            ('abc', 'xbc', '', ascii(None)),
            ('abc', 'axc', '', ascii(None)),
            ('abc', 'abx', '', ascii(None)),
            ('abc', 'xabcy', '0', ascii('abc')),
            ('abc', 'ababc', '0', ascii('abc')),
            ('ab*c', 'abc', '0', ascii('abc')),
            ('ab*bc', 'abc', '0', ascii('abc')),

            # 61..70
            ('ab*bc', 'abbc', '0', ascii('abbc')),
            ('ab*bc', 'abbbbc', '0', ascii('abbbbc')),
            ('ab+bc', 'abbc', '0', ascii('abbc')),
            ('ab+bc', 'abc', '', ascii(None)),
            ('ab+bc', 'abq', '', ascii(None)),
            ('ab+bc', 'abbbbc', '0', ascii('abbbbc')),
            ('ab?bc', 'abbc', '0', ascii('abbc')),
            ('ab?bc', 'abc', '0', ascii('abc')),
            ('ab?bc', 'abbbbc', '', ascii(None)),
            ('ab?c', 'abc', '0', ascii('abc')),

            # 71..80
            ('^abc$', 'abc', '0', ascii('abc')),
            ('^abc$', 'abcc', '', ascii(None)),
            ('^abc', 'abcc', '0', ascii('abc')),
            ('^abc$', 'aabc', '', ascii(None)),
            ('abc$', 'aabc', '0', ascii('abc')),
            ('^', 'abc', '0', ascii('')),
            ('$', 'abc', '0', ascii('')),
            ('a.c', 'abc', '0', ascii('abc')),
            ('a.c', 'axc', '0', ascii('axc')),
            ('a.*c', 'axyzc', '0', ascii('axyzc')),

            # 81..90
            ('a.*c', 'axyzd', '', ascii(None)),
            ('a[bc]d', 'abc', '', ascii(None)),
            ('a[bc]d', 'abd', '0', ascii('abd')),
            ('a[b-d]e', 'abd', '', ascii(None)),
            ('a[b-d]e', 'ace', '0', ascii('ace')),
            ('a[b-d]', 'aac', '0', ascii('ac')),
            ('a[-b]', 'a-', '0', ascii('a-')),
            ('a[\\-b]', 'a-', '0', ascii('a-')),
            ('a[b-]', 'a-', '0', ascii('a-')),
            ('a[]b', '-', '', self.BAD_SET),

            # 91..100
            ('a[', '-', '', self.BAD_SET),
            ('a\\', '-', '', self.BAD_ESCAPE),
            ('abc)', '-', '', self.TRAILING_CHARS),
            ('(abc', '-', '', self.MISSING_RPAREN),
            ('a]', 'a]', '0', ascii('a]')),
            ('a[]]b', 'a]b', '0', ascii('a]b')),
            ('a[]]b', 'a]b', '0', ascii('a]b')),
            ('a[^bc]d', 'aed', '0', ascii('aed')),
            ('a[^bc]d', 'abd', '', ascii(None)),
            ('a[^-b]c', 'adc', '0', ascii('adc')),

            # 101..110
            ('a[^-b]c', 'a-c', '', ascii(None)),
            ('a[^]b]c', 'a]c', '', ascii(None)),
            ('a[^]b]c', 'adc', '0', ascii('adc')),
            ('\\ba\\b', 'a-', '0', ascii('a')),
            ('\\ba\\b', '-a', '0', ascii('a')),
            ('\\ba\\b', '-a-', '0', ascii('a')),
            ('\\by\\b', 'xy', '', ascii(None)),
            ('\\by\\b', 'yz', '', ascii(None)),
            ('\\by\\b', 'xyz', '', ascii(None)),
            ('x\\b', 'xyz', '', ascii(None)),

            # 111..120
            ('x\\B', 'xyz', '0', ascii('x')),
            ('\\Bz', 'xyz', '0', ascii('z')),
            ('z\\B', 'xyz', '', ascii(None)),
            ('\\Bx', 'xyz', '', ascii(None)),
            ('\\Ba\\B', 'a-', '', ascii(None)),
            ('\\Ba\\B', '-a', '', ascii(None)),
            ('\\Ba\\B', '-a-', '', ascii(None)),
            ('\\By\\B', 'xy', '', ascii(None)),
            ('\\By\\B', 'yz', '', ascii(None)),
            ('\\By\\b', 'xy', '0', ascii('y')),

            # 121..130
            ('\\by\\B', 'yz', '0', ascii('y')),
            ('\\By\\B', 'xyz', '0', ascii('y')),
            ('ab|cd', 'abc', '0', ascii('ab')),
            ('ab|cd', 'abcd', '0', ascii('ab')),
            ('()ef', 'def', '0,1', ascii(('ef', ''))),
            ('$b', 'b', '', ascii(None)),
            ('a\\(b', 'a(b', '', ascii(('a(b',))),
            ('a\\(*b', 'ab', '0', ascii('ab')),
            ('a\\(*b', 'a((b', '0', ascii('a((b')),
            ('a\\\\b', 'a\\b', '0', ascii('a\\b')),

            # 131..140
            ('((a))', 'abc', '0,1,2', ascii(('a', 'a', 'a'))),
            ('(a)b(c)', 'abc', '0,1,2', ascii(('abc', 'a', 'c'))),
            ('a+b+c', 'aabbabc', '0', ascii('abc')),
            ('(a+|b)*', 'ab', '0,1', ascii(('ab', 'b'))),
            ('(a+|b)+', 'ab', '0,1', ascii(('ab', 'b'))),
            ('(a+|b)?', 'ab', '0,1', ascii(('a', 'a'))),
            (')(', '-', '', self.TRAILING_CHARS),
            ('[^ab]*', 'cde', '0', ascii('cde')),
            ('abc', '', '', ascii(None)),
            ('a*', '', '0', ascii('')),

            # 141..150
            ('a|b|c|d|e', 'e', '0', ascii('e')),
            ('(a|b|c|d|e)f', 'ef', '0,1', ascii(('ef', 'e'))),
            ('abcd*efg', 'abcdefg', '0', ascii('abcdefg')),
            ('ab*', 'xabyabbbz', '0', ascii('ab')),
            ('ab*', 'xayabbbz', '0', ascii('a')),
            ('(ab|cd)e', 'abcde', '0,1', ascii(('cde', 'cd'))),
            ('[abhgefdc]ij', 'hij', '0', ascii('hij')),
            ('^(ab|cd)e', 'abcde', '', ascii(None)),
            ('(abc|)ef', 'abcdef', '0,1', ascii(('ef', ''))),
            ('(a|b)c*d', 'abcd', '0,1', ascii(('bcd', 'b'))),

            # 151..160
            ('(ab|ab*)bc', 'abc', '0,1', ascii(('abc', 'a'))),
            ('a([bc]*)c*', 'abc', '0,1', ascii(('abc', 'bc'))),
            ('a([bc]*)(c*d)', 'abcd', '0,1,2', ascii(('abcd', 'bc', 'd'))),
            ('a([bc]+)(c*d)', 'abcd', '0,1,2', ascii(('abcd', 'bc', 'd'))),
            ('a([bc]*)(c+d)', 'abcd', '0,1,2', ascii(('abcd', 'b', 'cd'))),
            ('a[bcd]*dcdcde', 'adcdcde', '0', ascii('adcdcde')),
            ('a[bcd]+dcdcde', 'adcdcde', '', ascii(None)),
            ('(ab|a)b*c', 'abc', '0,1', ascii(('abc', 'ab'))),
            ('((a)(b)c)(d)', 'abcd', '1,2,3,4', ascii(('abc', 'a', 'b', 'd'))),
            ('[a-zA-Z_][a-zA-Z0-9_]*', 'alpha', '0', ascii('alpha')),

            # 161..170
            ('^a(bc+|b[eh])g|.h$', 'abh', '0,1', ascii(('bh', None))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effgz', '0,1,2', ascii(('effgz',
              'effgz', None))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'ij', '0,1,2', ascii(('ij', 'ij', 'j'))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effg', '', ascii(None)),
            ('(bc+d$|ef*g.|h?i(j|k))', 'bcdd', '', ascii(None)),
            ('(bc+d$|ef*g.|h?i(j|k))', 'reffgz', '0,1,2', ascii(('effgz',
              'effgz', None))),
            ('(((((((((a)))))))))', 'a', '0', ascii('a')),
            ('multiple words of text', 'uh-uh', '', ascii(None)),
            ('multiple words', 'multiple words, yeah', '0',
              ascii('multiple words')),
            ('(.*)c(.*)', 'abcde', '0,1,2', ascii(('abcde', 'ab', 'de'))),

            # 171..180
            ('\\((.*), (.*)\\)', '(a, b)', '2,1', ascii(('b', 'a'))),
            ('[k]', 'ab', '', ascii(None)),
            ('a[-]?c', 'ac', '0', ascii('ac')),
            ('(abc)\\1', 'abcabc', '1', ascii('abc')),
            ('([a-c]*)\\1', 'abcabc', '1', ascii('abc')),
            ('^(.+)?B', 'AB', '1', ascii('A')),
            ('(a+).\\1$', 'aaaaa', '0,1', ascii(('aaaaa', 'aa'))),
            ('^(a+).\\1$', 'aaaa', '', ascii(None)),
            ('(abc)\\1', 'abcabc', '0,1', ascii(('abcabc', 'abc'))),
            ('([a-c]+)\\1', 'abcabc', '0,1', ascii(('abcabc', 'abc'))),

            # 181..190
            ('(a)\\1', 'aa', '0,1', ascii(('aa', 'a'))),
            ('(a+)\\1', 'aa', '0,1', ascii(('aa', 'a'))),
            ('(a+)+\\1', 'aa', '0,1', ascii(('aa', 'a'))),
            ('(a).+\\1', 'aba', '0,1', ascii(('aba', 'a'))),
            ('(a)ba*\\1', 'aba', '0,1', ascii(('aba', 'a'))),
            ('(aa|a)a\\1$', 'aaa', '0,1', ascii(('aaa', 'a'))),
            ('(a|aa)a\\1$', 'aaa', '0,1', ascii(('aaa', 'a'))),
            ('(a+)a\\1$', 'aaa', '0,1', ascii(('aaa', 'a'))),
            ('([abc]*)\\1', 'abcabc', '0,1', ascii(('abcabc', 'abc'))),
            ('(a)(b)c|ab', 'ab', '0,1,2', ascii(('ab', None, None))),

            # 191..200
            ('(a)+x', 'aaax', '0,1', ascii(('aaax', 'a'))),
            ('([ac])+x', 'aacx', '0,1', ascii(('aacx', 'c'))),
            ('([^/]*/)*sub1/', 'd:msgs/tdir/sub1/trial/away.cpp', '0,1',
              ascii(('d:msgs/tdir/sub1/', 'tdir/'))),
            ('([^.]*)\\.([^:]*):[T ]+(.*)', 'track1.title:TBlah blah blah',
              '0,1,2,3', ascii(('track1.title:TBlah blah blah', 'track1',
              'title', 'Blah blah blah'))),
            ('([^N]*N)+', 'abNNxyzN', '0,1', ascii(('abNNxyzN', 'xyzN'))),
            ('([^N]*N)+', 'abNNxyz', '0,1', ascii(('abNN', 'N'))),
            ('([abc]*)x', 'abcx', '0,1', ascii(('abcx', 'abc'))),
            ('([abc]*)x', 'abc', '', ascii(None)),
            ('([xyz]*)x', 'abcx', '0,1', ascii(('x', ''))),
            ('(a)+b|aac', 'aac', '0,1', ascii(('aac', None))),

            # Test symbolic groups.
            # 201..204
            ('(?P<i d>aaa)a', 'aaaa', '', self.MISSING_GT),
            ('(?P<id>aaa)a', 'aaaa', '0,id', ascii(('aaaa', 'aaa'))),
            ('(?P<id>aa)(?P=id)', 'aaaa', '0,id', ascii(('aaaa', 'aa'))),
            ('(?P<id>aa)(?P=xd)', 'aaaa', '', self.UNKNOWN_GROUP),

            # Character properties.
            # 205..214
            (r"\g", "g", '0', ascii('g')),
            (r"\g<1>", "g", '', self.UNKNOWN_GROUP),
            (r"(.)\g<1>", "gg", '0', ascii('gg')),
            (r"(.)\g<1>", "gg", '', ascii(('gg', 'g'))),
            (r"\N", "N", '0', ascii('N')),
            (r"\N{LATIN SMALL LETTER A}", "a", '0', ascii('a')),
            (r"\p", "p", '0', ascii('p')),
            (r"\p{Ll}", "a", '0', ascii('a')),
            (r"\P", "P", '0', ascii('P')),
            (r"\P{Lu}", "p", '0', ascii('p')),

            # All tests from Perl.
            # 215..220
            ('abc', 'abc', '0', ascii('abc')),
            ('abc', 'xbc', '', ascii(None)),
            ('abc', 'axc', '', ascii(None)),
            ('abc', 'abx', '', ascii(None)),
            ('abc', 'xabcy', '0', ascii('abc')),
            ('abc', 'ababc', '0', ascii('abc')),

            # 221..230
            ('ab*c', 'abc', '0', ascii('abc')),
            ('ab*bc', 'abc', '0', ascii('abc')),
            ('ab*bc', 'abbc', '0', ascii('abbc')),
            ('ab*bc', 'abbbbc', '0', ascii('abbbbc')),
            ('ab{0,}bc', 'abbbbc', '0', ascii('abbbbc')),
            ('ab+bc', 'abbc', '0', ascii('abbc')),
            ('ab+bc', 'abc', '', ascii(None)),
            ('ab+bc', 'abq', '', ascii(None)),
            ('ab{1,}bc', 'abq', '', ascii(None)),
            ('ab+bc', 'abbbbc', '0', ascii('abbbbc')),

            # 231..240
            ('ab{1,}bc', 'abbbbc', '0', ascii('abbbbc')),
            ('ab{1,3}bc', 'abbbbc', '0', ascii('abbbbc')),
            ('ab{3,4}bc', 'abbbbc', '0', ascii('abbbbc')),
            ('ab{4,5}bc', 'abbbbc', '', ascii(None)),
            ('ab?bc', 'abbc', '0', ascii('abbc')),
            ('ab?bc', 'abc', '0', ascii('abc')),
            ('ab{0,1}bc', 'abc', '0', ascii('abc')),
            ('ab?bc', 'abbbbc', '', ascii(None)),
            ('ab?c', 'abc', '0', ascii('abc')),
            ('ab{0,1}c', 'abc', '0', ascii('abc')),

            # 241..250
            ('^abc$', 'abc', '0', ascii('abc')),
            ('^abc$', 'abcc', '', ascii(None)),
            ('^abc', 'abcc', '0', ascii('abc')),
            ('^abc$', 'aabc', '', ascii(None)),
            ('abc$', 'aabc', '0', ascii('abc')),
            ('^', 'abc', '0', ascii('')),
            ('$', 'abc', '0', ascii('')),
            ('a.c', 'abc', '0', ascii('abc')),
            ('a.c', 'axc', '0', ascii('axc')),
            ('a.*c', 'axyzc', '0', ascii('axyzc')),

            # 251..260
            ('a.*c', 'axyzd', '', ascii(None)),
            ('a[bc]d', 'abc', '', ascii(None)),
            ('a[bc]d', 'abd', '0', ascii('abd')),
            ('a[b-d]e', 'abd', '', ascii(None)),
            ('a[b-d]e', 'ace', '0', ascii('ace')),
            ('a[b-d]', 'aac', '0', ascii('ac')),
            ('a[-b]', 'a-', '0', ascii('a-')),
            ('a[b-]', 'a-', '0', ascii('a-')),
            ('a[b-a]', '-', '', self.BAD_CHAR_RANGE),
            ('a[]b', '-', '', self.BAD_SET),

            # 261..270
            ('a[', '-', '', self.BAD_SET),
            ('a]', 'a]', '0', ascii('a]')),
            ('a[]]b', 'a]b', '0', ascii('a]b')),
            ('a[^bc]d', 'aed', '0', ascii('aed')),
            ('a[^bc]d', 'abd', '', ascii(None)),
            ('a[^-b]c', 'adc', '0', ascii('adc')),
            ('a[^-b]c', 'a-c', '', ascii(None)),
            ('a[^]b]c', 'a]c', '', ascii(None)),
            ('a[^]b]c', 'adc', '0', ascii('adc')),
            ('ab|cd', 'abc', '0', ascii('ab')),

            # 271..280
            ('ab|cd', 'abcd', '0', ascii('ab')),
            ('()ef', 'def', '0,1', ascii(('ef', ''))),
            ('*a', '-', '', self.NOTHING_TO_REPEAT),
            ('(*)b', '-', '', self.NOTHING_TO_REPEAT),
            ('$b', 'b', '', ascii(None)),
            ('a\\', '-', '', self.BAD_ESCAPE),
            ('a\\(b', 'a(b', '', ascii(('a(b',))),
            ('a\\(*b', 'ab', '0', ascii('ab')),
            ('a\\(*b', 'a((b', '0', ascii('a((b')),
            ('a\\\\b', 'a\\b', '0', ascii('a\\b')),

            # 281..290
            ('abc)', '-', '', self.TRAILING_CHARS),
            ('(abc', '-', '', self.MISSING_RPAREN),
            ('((a))', 'abc', '0,1,2', ascii(('a', 'a', 'a'))),
            ('(a)b(c)', 'abc', '0,1,2', ascii(('abc', 'a', 'c'))),
            ('a+b+c', 'aabbabc', '0', ascii('abc')),
            ('a{1,}b{1,}c', 'aabbabc', '0', ascii('abc')),
            ('a**', '-', '', self.NOTHING_TO_REPEAT),
            ('a.+?c', 'abcabc', '0', ascii('abc')),
            ('(a+|b)*', 'ab', '0,1', ascii(('ab', 'b'))),
            ('(a+|b){0,}', 'ab', '0,1', ascii(('ab', 'b'))),

            # 291..300
            ('(a+|b)+', 'ab', '0,1', ascii(('ab', 'b'))),
            ('(a+|b){1,}', 'ab', '0,1', ascii(('ab', 'b'))),
            ('(a+|b)?', 'ab', '0,1', ascii(('a', 'a'))),
            ('(a+|b){0,1}', 'ab', '0,1', ascii(('a', 'a'))),
            (')(', '-', '', self.TRAILING_CHARS),
            ('[^ab]*', 'cde', '0', ascii('cde')),
            ('abc', '', '', ascii(None)),
            ('a*', '', '0', ascii('')),
            ('([abc])*d', 'abbbcd', '0,1', ascii(('abbbcd', 'c'))),
            ('([abc])*bcd', 'abcd', '0,1', ascii(('abcd', 'a'))),

            # 301..310
            ('a|b|c|d|e', 'e', '0', ascii('e')),
            ('(a|b|c|d|e)f', 'ef', '0,1', ascii(('ef', 'e'))),
            ('abcd*efg', 'abcdefg', '0', ascii('abcdefg')),
            ('ab*', 'xabyabbbz', '0', ascii('ab')),
            ('ab*', 'xayabbbz', '0', ascii('a')),
            ('(ab|cd)e', 'abcde', '0,1', ascii(('cde', 'cd'))),
            ('[abhgefdc]ij', 'hij', '0', ascii('hij')),
            ('^(ab|cd)e', 'abcde', '', ascii(None)),
            ('(abc|)ef', 'abcdef', '0,1', ascii(('ef', ''))),
            ('(a|b)c*d', 'abcd', '0,1', ascii(('bcd', 'b'))),

            # 311..320
            ('(ab|ab*)bc', 'abc', '0,1', ascii(('abc', 'a'))),
            ('a([bc]*)c*', 'abc', '0,1', ascii(('abc', 'bc'))),
            ('a([bc]*)(c*d)', 'abcd', '0,1,2', ascii(('abcd', 'bc', 'd'))),
            ('a([bc]+)(c*d)', 'abcd', '0,1,2', ascii(('abcd', 'bc', 'd'))),
            ('a([bc]*)(c+d)', 'abcd', '0,1,2', ascii(('abcd', 'b', 'cd'))),
            ('a[bcd]*dcdcde', 'adcdcde', '0', ascii('adcdcde')),
            ('a[bcd]+dcdcde', 'adcdcde', '', ascii(None)),
            ('(ab|a)b*c', 'abc', '0,1', ascii(('abc', 'ab'))),
            ('((a)(b)c)(d)', 'abcd', '1,2,3,4', ascii(('abc', 'a', 'b', 'd'))),
            ('[a-zA-Z_][a-zA-Z0-9_]*', 'alpha', '0', ascii('alpha')),

            # 321..328
            ('^a(bc+|b[eh])g|.h$', 'abh', '0,1', ascii(('bh', None))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effgz', '0,1,2', ascii(('effgz',
              'effgz', None))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'ij', '0,1,2', ascii(('ij', 'ij', 'j'))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effg', '', ascii(None)),
            ('(bc+d$|ef*g.|h?i(j|k))', 'bcdd', '', ascii(None)),
            ('(bc+d$|ef*g.|h?i(j|k))', 'reffgz', '0,1,2', ascii(('effgz',
              'effgz', None))),
            ('((((((((((a))))))))))', 'a', '10', ascii('a')),
            ('((((((((((a))))))))))\\10', 'aa', '0', ascii('aa')),

            # Python does not have the same rules for \\41 so this is a syntax error
            #    ('((((((((((a))))))))))\\41', 'aa', '', ascii(None)),
            #    ('((((((((((a))))))))))\\41', 'a!', '0', ascii('a!')),
            # 329..330
            ('((((((((((a))))))))))\\41', '', '', self.UNKNOWN_GROUP),
            ('(?i)((((((((((a))))))))))\\41', '', '', self.UNKNOWN_GROUP),

            # 331..340
            ('(((((((((a)))))))))', 'a', '0', ascii('a')),
            ('multiple words of text', 'uh-uh', '', ascii(None)),
            ('multiple words', 'multiple words, yeah', '0',
              ascii('multiple words')),
            ('(.*)c(.*)', 'abcde', '0,1,2', ascii(('abcde', 'ab', 'de'))),
            ('\\((.*), (.*)\\)', '(a, b)', '2,1', ascii(('b', 'a'))),
            ('[k]', 'ab', '', ascii(None)),
            ('a[-]?c', 'ac', '0', ascii('ac')),
            ('(abc)\\1', 'abcabc', '1', ascii('abc')),
            ('([a-c]*)\\1', 'abcabc', '1', ascii('abc')),
            ('(?i)abc', 'ABC', '0', ascii('ABC')),

            # 341..350
            ('(?i)abc', 'XBC', '', ascii(None)),
            ('(?i)abc', 'AXC', '', ascii(None)),
            ('(?i)abc', 'ABX', '', ascii(None)),
            ('(?i)abc', 'XABCY', '0', ascii('ABC')),
            ('(?i)abc', 'ABABC', '0', ascii('ABC')),
            ('(?i)ab*c', 'ABC', '0', ascii('ABC')),
            ('(?i)ab*bc', 'ABC', '0', ascii('ABC')),
            ('(?i)ab*bc', 'ABBC', '0', ascii('ABBC')),
            ('(?i)ab*?bc', 'ABBBBC', '0', ascii('ABBBBC')),
            ('(?i)ab{0,}?bc', 'ABBBBC', '0', ascii('ABBBBC')),

            # 351..360
            ('(?i)ab+?bc', 'ABBC', '0', ascii('ABBC')),
            ('(?i)ab+bc', 'ABC', '', ascii(None)),
            ('(?i)ab+bc', 'ABQ', '', ascii(None)),
            ('(?i)ab{1,}bc', 'ABQ', '', ascii(None)),
            ('(?i)ab+bc', 'ABBBBC', '0', ascii('ABBBBC')),
            ('(?i)ab{1,}?bc', 'ABBBBC', '0', ascii('ABBBBC')),
            ('(?i)ab{1,3}?bc', 'ABBBBC', '0', ascii('ABBBBC')),
            ('(?i)ab{3,4}?bc', 'ABBBBC', '0', ascii('ABBBBC')),
            ('(?i)ab{4,5}?bc', 'ABBBBC', '', ascii(None)),
            ('(?i)ab??bc', 'ABBC', '0', ascii('ABBC')),

            # 361..370
            ('(?i)ab??bc', 'ABC', '0', ascii('ABC')),
            ('(?i)ab{0,1}?bc', 'ABC', '0', ascii('ABC')),
            ('(?i)ab??bc', 'ABBBBC', '', ascii(None)),
            ('(?i)ab??c', 'ABC', '0', ascii('ABC')),
            ('(?i)ab{0,1}?c', 'ABC', '0', ascii('ABC')),
            ('(?i)^abc$', 'ABC', '0', ascii('ABC')),
            ('(?i)^abc$', 'ABCC', '', ascii(None)),
            ('(?i)^abc', 'ABCC', '0', ascii('ABC')),
            ('(?i)^abc$', 'AABC', '', ascii(None)),
            ('(?i)abc$', 'AABC', '0', ascii('ABC')),

            # 371..380
            ('(?i)^', 'ABC', '0', ascii('')),
            ('(?i)$', 'ABC', '0', ascii('')),
            ('(?i)a.c', 'ABC', '0', ascii('ABC')),
            ('(?i)a.c', 'AXC', '0', ascii('AXC')),
            ('(?i)a.*?c', 'AXYZC', '0', ascii('AXYZC')),
            ('(?i)a.*c', 'AXYZD', '', ascii(None)),
            ('(?i)a[bc]d', 'ABC', '', ascii(None)),
            ('(?i)a[bc]d', 'ABD', '0', ascii('ABD')),
            ('(?i)a[b-d]e', 'ABD', '', ascii(None)),
            ('(?i)a[b-d]e', 'ACE', '0', ascii('ACE')),

            # 381..390
            ('(?i)a[b-d]', 'AAC', '0', ascii('AC')),
            ('(?i)a[-b]', 'A-', '0', ascii('A-')),
            ('(?i)a[b-]', 'A-', '0', ascii('A-')),
            ('(?i)a[b-a]', '-', '', self.BAD_CHAR_RANGE),
            ('(?i)a[]b', '-', '', self.BAD_SET),
            ('(?i)a[', '-', '', self.BAD_SET),
            ('(?i)a]', 'A]', '0', ascii('A]')),
            ('(?i)a[]]b', 'A]B', '0', ascii('A]B')),
            ('(?i)a[^bc]d', 'AED', '0', ascii('AED')),
            ('(?i)a[^bc]d', 'ABD', '', ascii(None)),

            # 391..400
            ('(?i)a[^-b]c', 'ADC', '0', ascii('ADC')),
            ('(?i)a[^-b]c', 'A-C', '', ascii(None)),
            ('(?i)a[^]b]c', 'A]C', '', ascii(None)),
            ('(?i)a[^]b]c', 'ADC', '0', ascii('ADC')),
            ('(?i)ab|cd', 'ABC', '0', ascii('AB')),
            ('(?i)ab|cd', 'ABCD', '0', ascii('AB')),
            ('(?i)()ef', 'DEF', '0,1', ascii(('EF', ''))),
            ('(?i)*a', '-', '', self.NOTHING_TO_REPEAT),
            ('(?i)(*)b', '-', '', self.NOTHING_TO_REPEAT),
            ('(?i)$b', 'B', '', ascii(None)),

            # 401..410
            ('(?i)a\\', '-', '', self.BAD_ESCAPE),
            ('(?i)a\\(b', 'A(B', '', ascii(('A(B',))),
            ('(?i)a\\(*b', 'AB', '0', ascii('AB')),
            ('(?i)a\\(*b', 'A((B', '0', ascii('A((B')),
            ('(?i)a\\\\b', 'A\\B', '0', ascii('A\\B')),
            ('(?i)abc)', '-', '', self.TRAILING_CHARS),
            ('(?i)(abc', '-', '', self.MISSING_RPAREN),
            ('(?i)((a))', 'ABC', '0,1,2', ascii(('A', 'A', 'A'))),
            ('(?i)(a)b(c)', 'ABC', '0,1,2', ascii(('ABC', 'A', 'C'))),
            ('(?i)a+b+c', 'AABBABC', '0', ascii('ABC')),

            # 411..420
            ('(?i)a{1,}b{1,}c', 'AABBABC', '0', ascii('ABC')),
            ('(?i)a**', '-', '', self.NOTHING_TO_REPEAT),
            ('(?i)a.+?c', 'ABCABC', '0', ascii('ABC')),
            ('(?i)a.*?c', 'ABCABC', '0', ascii('ABC')),
            ('(?i)a.{0,5}?c', 'ABCABC', '0', ascii('ABC')),
            ('(?i)(a+|b)*', 'AB', '0,1', ascii(('AB', 'B'))),
            ('(?i)(a+|b){0,}', 'AB', '0,1', ascii(('AB', 'B'))),
            ('(?i)(a+|b)+', 'AB', '0,1', ascii(('AB', 'B'))),
            ('(?i)(a+|b){1,}', 'AB', '0,1', ascii(('AB', 'B'))),
            ('(?i)(a+|b)?', 'AB', '0,1', ascii(('A', 'A'))),

            # 421..430
            ('(?i)(a+|b){0,1}', 'AB', '0,1', ascii(('A', 'A'))),
            ('(?i)(a+|b){0,1}?', 'AB', '0,1', ascii(('', None))),
            ('(?i))(', '-', '', self.TRAILING_CHARS),
            ('(?i)[^ab]*', 'CDE', '0', ascii('CDE')),
            ('(?i)abc', '', '', ascii(None)),
            ('(?i)a*', '', '0', ascii('')),
            ('(?i)([abc])*d', 'ABBBCD', '0,1', ascii(('ABBBCD', 'C'))),
            ('(?i)([abc])*bcd', 'ABCD', '0,1', ascii(('ABCD', 'A'))),
            ('(?i)a|b|c|d|e', 'E', '0', ascii('E')),
            ('(?i)(a|b|c|d|e)f', 'EF', '0,1', ascii(('EF', 'E'))),

            # 431..440
            ('(?i)abcd*efg', 'ABCDEFG', '0', ascii('ABCDEFG')),
            ('(?i)ab*', 'XABYABBBZ', '0', ascii('AB')),
            ('(?i)ab*', 'XAYABBBZ', '0', ascii('A')),
            ('(?i)(ab|cd)e', 'ABCDE', '0,1', ascii(('CDE', 'CD'))),
            ('(?i)[abhgefdc]ij', 'HIJ', '0', ascii('HIJ')),
            ('(?i)^(ab|cd)e', 'ABCDE', '', ascii(None)),
            ('(?i)(abc|)ef', 'ABCDEF', '0,1', ascii(('EF', ''))),
            ('(?i)(a|b)c*d', 'ABCD', '0,1', ascii(('BCD', 'B'))),
            ('(?i)(ab|ab*)bc', 'ABC', '0,1', ascii(('ABC', 'A'))),
            ('(?i)a([bc]*)c*', 'ABC', '0,1', ascii(('ABC', 'BC'))),

            # 441..450
            ('(?i)a([bc]*)(c*d)', 'ABCD', '0,1,2', ascii(('ABCD', 'BC', 'D'))),
            ('(?i)a([bc]+)(c*d)', 'ABCD', '0,1,2', ascii(('ABCD', 'BC', 'D'))),
            ('(?i)a([bc]*)(c+d)', 'ABCD', '0,1,2', ascii(('ABCD', 'B', 'CD'))),
            ('(?i)a[bcd]*dcdcde', 'ADCDCDE', '0', ascii('ADCDCDE')),
            ('(?i)a[bcd]+dcdcde', 'ADCDCDE', '', ascii(None)),
            ('(?i)(ab|a)b*c', 'ABC', '0,1', ascii(('ABC', 'AB'))),
            ('(?i)((a)(b)c)(d)', 'ABCD', '1,2,3,4', ascii(('ABC', 'A', 'B',
              'D'))),
            ('(?i)[a-zA-Z_][a-zA-Z0-9_]*', 'ALPHA', '0', ascii('ALPHA')),
            ('(?i)^a(bc+|b[eh])g|.h$', 'ABH', '0,1', ascii(('BH', None))),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'EFFGZ', '0,1,2', ascii(('EFFGZ',
              'EFFGZ', None))),

            # 451..456
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'IJ', '0,1,2', ascii(('IJ', 'IJ',
              'J'))),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'EFFG', '', ascii(None)),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'BCDD', '', ascii(None)),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'REFFGZ', '0,1,2', ascii(('EFFGZ',
              'EFFGZ', None))),
            ('(?i)((((((((((a))))))))))', 'A', '10', ascii('A')),
            ('(?i)((((((((((a))))))))))\\10', 'AA', '0', ascii('AA')),
            #('(?i)((((((((((a))))))))))\\41', 'AA', '', ascii(None)),
            #('(?i)((((((((((a))))))))))\\41', 'A!', '0', ascii('A!')),
            # 457..460
            ('(?i)(((((((((a)))))))))', 'A', '0', ascii('A')),
            ('(?i)(?:(?:(?:(?:(?:(?:(?:(?:(?:(a))))))))))', 'A', '1',
              ascii('A')),
            ('(?i)(?:(?:(?:(?:(?:(?:(?:(?:(?:(a|b|c))))))))))', 'C', '1',
              ascii('C')),
            ('(?i)multiple words of text', 'UH-UH', '', ascii(None)),

            # 461..464
            ('(?i)multiple words', 'MULTIPLE WORDS, YEAH', '0',
              ascii('MULTIPLE WORDS')),
            ('(?i)(.*)c(.*)', 'ABCDE', '0,1,2', ascii(('ABCDE', 'AB', 'DE'))),
            ('(?i)\\((.*), (.*)\\)', '(A, B)', '2,1', ascii(('B', 'A'))),
            ('(?i)[k]', 'AB', '', ascii(None)),
        #    ('(?i)abcd', 'ABCD', SUCCEED, 'found+"-"+\\found+"-"+\\\\found', ascii(ABCD-$&-\\ABCD)),
        #    ('(?i)a(bc)d', 'ABCD', SUCCEED, 'g1+"-"+\\g1+"-"+\\\\g1', ascii(BC-$1-\\BC)),
            # 465..470
            ('(?i)a[-]?c', 'AC', '0', ascii('AC')),
            ('(?i)(abc)\\1', 'ABCABC', '1', ascii('ABC')),
            ('(?i)([a-c]*)\\1', 'ABCABC', '1', ascii('ABC')),
            ('a(?!b).', 'abad', '0', ascii('ad')),
            ('a(?=d).', 'abad', '0', ascii('ad')),
            ('a(?=c|d).', 'abad', '0', ascii('ad')),

            # 471..474
            ('a(?:b|c|d)(.)', 'ace', '1', ascii('e')),
            ('a(?:b|c|d)*(.)', 'ace', '1', ascii('e')),
            ('a(?:b|c|d)+?(.)', 'ace', '1', ascii('e')),
            ('a(?:b|(c|e){1,2}?|d)+?(.)', 'ace', '1,2', ascii(('c', 'e'))),

            # Lookbehind: split by : but not if it is escaped by -.
            # 475
            ('(?<!-):(.*?)(?<!-):', 'a:bc-:de:f', '1', ascii('bc-:de')),
            # Escaping with \ as we know it.
            # 476
            ('(?<!\\\):(.*?)(?<!\\\):', 'a:bc\\:de:f', '1', ascii('bc\\:de')),
            # Terminating with ' and escaping with ? as in edifact.
            # 477
            ("(?<!\\?)'(.*?)(?<!\\?)'", "a'bc?'de'f", '1', ascii("bc?'de")),

            # Comments using the (?#...) syntax.

            # 478..479
            ('w(?# comment', 'w', '', self.MISSING_RPAREN),
            ('w(?# comment 1)xy(?# comment 2)z', 'wxyz', '0', ascii('wxyz')),

            # Check odd placement of embedded pattern modifiers.

            # Not an error under PCRE/PRE:
            # When the new behaviour is turned on positional inline flags affect
            # only what follows.
            # 480..485
            ('w(?i)', 'W', '0', ascii('W')),
            ('w(?iV1)', 'W', '0', ascii(None)),
            ('w(?i)', 'w', '0', ascii('w')),
            ('w(?iV1)', 'w', '0', ascii('w')),
            ('(?i)w', 'W', '0', ascii('W')),
            ('(?iV1)w', 'W', '0', ascii('W')),

            # Comments using the x embedded pattern modifier.
            # 486
            ("""(?x)w# comment 1
x y
# comment 2
z""", 'wxyz', '0', ascii('wxyz')),

            # Using the m embedded pattern modifier.
            # 487..489
            ('^abc', """jkl
abc
xyz""", '', ascii(None)),
            ('(?m)^abc', """jkl
abc
xyz""", '0', ascii('abc')),

            ('(?m)abc$', """jkl
xyzabc
123""", '0', ascii('abc')),

            # Using the s embedded pattern modifier.
            # 490..491
            ('a.b', 'a\nb', '', ascii(None)),
            ('(?s)a.b', 'a\nb', '0', ascii('a\nb')),

            # Test \w, etc. both inside and outside character classes.
            # 492..496
            ('\\w+', '--ab_cd0123--', '0', ascii('ab_cd0123')),
            ('[\\w]+', '--ab_cd0123--', '0', ascii('ab_cd0123')),
            ('\\D+', '1234abc5678', '0', ascii('abc')),
            ('[\\D]+', '1234abc5678', '0', ascii('abc')),
            ('[\\da-fA-F]+', '123abc', '0', ascii('123abc')),
            # Not an error under PCRE/PRE:
            # ('[\\d-x]', '-', '', self.SYNTAX_ERROR),
            # 497..498
            (r'([\s]*)([\S]*)([\s]*)', ' testing!1972', '3,2,1', ascii(('',
              'testing!1972', ' '))),
            (r'(\s*)(\S*)(\s*)', ' testing!1972', '3,2,1', ascii(('',
              'testing!1972', ' '))),

            #
            # Post-1.5.2 additions.

            # xmllib problem.
            # 499
            (r'(([a-z]+):)?([a-z]+)$', 'smil', '1,2,3', ascii((None, None,
              'smil'))),
            # Bug 110866: reference to undefined group.
            # 500
            (r'((.)\1+)', '', '', self.OPEN_GROUP),
            # Bug 111869: search (PRE/PCRE fails on this one, SRE doesn't).
            # 501
            (r'.*d', 'abc\nabd', '0', ascii('abd')),
            # Bug 112468: various expected syntax errors.
            # 502..503
            (r'(', '', '', self.MISSING_RPAREN),
            (r'[\41]', '!', '0', ascii('!')),
            # Bug 114033: nothing to repeat.
            # 504
            (r'(x?)?', 'x', '0', ascii('x')),
            # Bug 115040: rescan if flags are modified inside pattern.
            # If the new behaviour is turned on then positional inline flags
            # affect only what follows.
            # 505..510
            (r' (?x)foo ', 'foo', '0', ascii('foo')),
            (r' (?V1x)foo ', 'foo', '0', ascii(None)),
            (r'(?x) foo ', 'foo', '0', ascii('foo')),
            (r'(?V1x) foo ', 'foo', '0', ascii('foo')),
            (r'(?x)foo ', 'foo', '0', ascii('foo')),
            (r'(?V1x)foo ', 'foo', '0', ascii('foo')),
            # Bug 115618: negative lookahead.
            # 511
            (r'(?<!abc)(d.f)', 'abcdefdof', '0', ascii('dof')),
            # Bug 116251: character class bug.
            # 512
            (r'[\w-]+', 'laser_beam', '0', ascii('laser_beam')),
            # Bug 123769+127259: non-greedy backtracking bug.
            # 513..515
            (r'.*?\S *:', 'xx:', '0', ascii('xx:')),
            (r'a[ ]*?\ (\d+).*', 'a   10', '0', ascii('a   10')),
            (r'a[ ]*?\ (\d+).*', 'a    10', '0', ascii('a    10')),
            # Bug 127259: \Z shouldn't depend on multiline mode.
            # 516
            (r'(?ms).*?x\s*\Z(.*)','xx\nx\n', '1', ascii('')),
            # Bug 128899: uppercase literals under the ignorecase flag.
            # 517..520
            (r'(?i)M+', 'MMM', '0', ascii('MMM')),
            (r'(?i)m+', 'MMM', '0', ascii('MMM')),
            (r'(?i)[M]+', 'MMM', '0', ascii('MMM')),
            (r'(?i)[m]+', 'MMM', '0', ascii('MMM')),
            # Bug 130748: ^* should be an error (nothing to repeat).
            # 521
            (r'^*', '', '', self.NOTHING_TO_REPEAT),
            # Bug 133283: minimizing repeat problem.
            # 522
            (r'"(?:\\"|[^"])*?"', r'"\""', '0', ascii(r'"\""')),
            # Bug 477728: minimizing repeat problem.
            # 523
            (r'^.*?$', 'one\ntwo\nthree\n', '', ascii(None)),
            # Bug 483789: minimizing repeat problem.
            # 524
            (r'a[^>]*?b', 'a>b', '', ascii(None)),
            # Bug 490573: minimizing repeat problem.
            # 525
            (r'^a*?$', 'foo', '', ascii(None)),
            # Bug 470582: nested groups problem.
            # 526
            (r'^((a)c)?(ab)$', 'ab', '1,2,3', ascii((None, None, 'ab'))),
            # Another minimizing repeat problem (capturing groups in assertions).
            # 527..529
            ('^([ab]*?)(?=(b)?)c', 'abc', '1,2', ascii(('ab', None))),
            ('^([ab]*?)(?!(b))c', 'abc', '1,2', ascii(('ab', None))),
            ('^([ab]*?)(?<!(a))c', 'abc', '1,2', ascii(('ab', None))),
            # Bug 410271: \b broken under locales.
            # 530..532
            (r'\b.\b', 'a', '0', ascii('a')),
            (r'\b.\b', '\N{LATIN CAPITAL LETTER A WITH DIAERESIS}', '0',
              ascii('\xc4')),
            (r'\w', '\N{LATIN CAPITAL LETTER A WITH DIAERESIS}', '0',
              ascii('\xc4')),
        ]

        for t in tests:
            self.index += 1
            try:
                pattern, string, groups, expected = t
            except ValueError:
                fields = ", ".join([ascii(f) for f in t[ : 3]] + ["..."])
                self.record_failure("Incorrect number of test fields: ({})".format(fields))
            else:
                group_list = []
                if groups:
                    for group in groups.split(","):
                        try:
                            group_list.append(int(group))
                        except ValueError:
                            group_list.append(group)

                try:
                    m = regex.search(pattern, string)
                    if m:
                        if group_list:
                            actual = ascii(m.group(*group_list))
                        else:
                            actual = ascii(m[:])
                    else:
                        actual = ascii(m)
                except Exception as e:
                    actual = ascii(e)

                if actual != expected:
                    fields = ", ".join([ascii(f) for f in t[ : 3]] + ["..."])
                    self.record_failure("Expected {} but got {} instead: ({})".format(expected,
                      actual, fields))

    def test_replacement(self):
        # 1..2
        self.expect(lambda: regex.sub("test\?", "result\?\.\a\q\m\n", "test?"),
          ascii("result\?\.\a\q\m\n"))
        self.expect(lambda: regex.sub(r"test\?", "result\?\.\a\q\m\n",
          "test?"), ascii("result\?\.\a\q\m\n"))

        # 3..6
        self.expect(lambda: regex.sub('(.)', r"\1\1", 'x'), ascii('xx'))
        self.expect(lambda: regex.sub('(.)', regex.escape(r"\1\1"), 'x'),
          ascii(r"\1\1"))
        self.expect(lambda: regex.sub('(.)', r"\\1\\1", 'x'), ascii(r"\1\1"))
        self.expect(lambda: regex.sub('(.)', lambda m: r"\1\1", 'x'),
          ascii(r"\1\1"))

    def test_common_prefix(self):
        # Very long common prefix
        all = string.ascii_lowercase + string.digits + string.ascii_uppercase
        side = all * 4
        regexp = '(' + side + '|' + side + ')'
        self.expect(lambda: type(regex.compile(regexp)), self.PATTERN_CLASS)

    def test_captures(self):
        # 1..5
        self.expect(lambda: regex.search(r"(\w)+", "abc").captures(1),
          ascii(['a', 'b', 'c']))
        self.expect(lambda: regex.search(r"(\w{3})+", "abcdef").captures(0, 1),
          ascii((['abcdef'], ['abc', 'def'])))
        self.expect(lambda: regex.search(r"^(\d{1,3})(?:\.(\d{1,3})){3}$",
          "192.168.0.1").captures(1, 2), ascii((['192', ], ['168', '0', '1'])))
        self.expect(lambda: regex.match(r"^([0-9A-F]{2}){4} ([a-z]\d){5}$",
          "3FB52A0C a2c4g3k9d3").captures(1, 2), ascii((['3F', 'B5', '2A',
          '0C'], ['a2', 'c4', 'g3', 'k9', 'd3'])))
        self.expect(lambda: regex.match("([a-z]W)([a-z]X)+([a-z]Y)",
          "aWbXcXdXeXfY").captures(1, 2, 3), ascii((['aW'], ['bX', 'cX', 'dX',
          'eX'], ['fY'])))

        # 6..8
        self.expect(lambda: regex.search(r".*?(?=(.)+)b", "ab").captures(1),
          ascii(['b']))
        self.expect(lambda: regex.search(r".*?(?>(.){0,2})d",
          "abcd").captures(1), ascii(['b', 'c']))
        self.expect(lambda: regex.search(r"(.)+", "a").captures(1),
          ascii(['a']))

    def test_guards(self):
        m = regex.search(r"(X.*?Y\s*){3}(X\s*)+AB:",
          "XY\nX Y\nX  Y\nXY\nXX AB:")
        # 1
        self.expect(lambda: m.span(0, 1, 2), ascii(((3, 21), (12, 15), (16,
          18))))

        m = regex.search(r"(X.*?Y\s*){3,}(X\s*)+AB:",
          "XY\nX Y\nX  Y\nXY\nXX AB:")
        # 2
        self.expect(lambda: m.span(0, 1, 2), ascii(((0, 21), (12, 15), (16,
          18))))

        m = regex.search(r'\d{4}(\s*\w)?\W*((?!\d)\w){2}', "9999XX")
        # 3
        self.expect(lambda: m.span(0, 1, 2), ascii(((0, 6), (-1, -1), (5, 6))))

        m = regex.search(r'A\s*?.*?(\n+.*?\s*?){0,2}\(X', 'A\n1\nS\n1 (X')
        # 4
        self.expect(lambda: m.span(0, 1), ascii(((0, 10), (5, 8))))

        m = regex.search('Derde\s*:', 'aaaaaa:\nDerde:')
        # 5
        self.expect(lambda: m.span(), ascii((8, 14)))
        m = regex.search('Derde\s*:', 'aaaaa:\nDerde:')
        # 6
        self.expect(lambda: m.span(), ascii((7, 13)))

    def test_turkic(self):
        # Turkish has dotted and dotless I/i.
        pairs = "I=i;I=\u0131;i=\u0130"

        all_chars = set()
        matching = set()
        for pair in pairs.split(";"):
            ch1, ch2 = pair.split("=")
            all_chars.update((ch1, ch2))
            matching.add((ch1, ch1))
            matching.add((ch1, ch2))
            matching.add((ch2, ch1))
            matching.add((ch2, ch2))

        for ch1 in all_chars:
            for ch2 in all_chars:
                self.index += 1
                m = regex.match(r"(?i)\A" + ch1 + r"\Z", ch2)
                if m:
                    if (ch1, ch2) not in matching:
                        self.record_failure("{} matching {}".format(ascii(ch1),
                          ascii(ch2)))
                else:
                    if (ch1, ch2) in matching:
                        self.record_failure("{} not matching {}".format(ascii(ch1),
                          ascii(ch2)))

    def test_named_lists(self):
        options = ["one", "two", "three"]
        # 1..3
        self.expect(lambda: regex.match(r"333\L<bar>444", "333one444",
          bar=options).group(), ascii("333one444"))
        self.expect(lambda: regex.match(r"(?i)333\L<bar>444", "333TWO444",
          bar=options).group(), ascii("333TWO444"))
        self.expect(lambda: regex.match(r"333\L<bar>444", "333four444",
          bar=options), ascii(None))

        options = [b"one", b"two", b"three"]
        # 4..6
        self.expect(lambda: regex.match(br"333\L<bar>444", b"333one444",
          bar=options).group(), ascii(b"333one444"))
        self.expect(lambda: regex.match(br"(?i)333\L<bar>444", b"333TWO444",
          bar=options).group(), ascii(b"333TWO444"))
        self.expect(lambda: regex.match(br"333\L<bar>444", b"333four444",
          bar=options), ascii(None))

        # 7
        self.expect(lambda: type(regex.compile(r"3\L<bar>4\L<bar>+5",
          bar=["one", "two", "three"])), self.PATTERN_CLASS)

        # 8..9
        self.expect(lambda: regex.findall(r"^\L<options>", "solid QWERT",
          options=set(['good', 'brilliant', '+s\\ol[i}d'])), ascii([]))
        self.expect(lambda: regex.findall(r"^\L<options>", "+solid QWERT",
          options=set(['good', 'brilliant', '+solid'])), ascii(['+solid']))

        options = ["STRASSE"]
        # 10
        self.expect(lambda: regex.match(r"(?fi)\L<words>",
          "stra\N{LATIN SMALL LETTER SHARP S}e", words=options).span(),
          ascii((0, 6)))

        options = ["STRASSE", "stress"]
        # 11
        self.expect(lambda: regex.match(r"(?fi)\L<words>",
          "stra\N{LATIN SMALL LETTER SHARP S}e", words=options).span(),
          ascii((0, 6)))

        options = ["stra\N{LATIN SMALL LETTER SHARP S}e"]
        # 12
        self.expect(lambda: regex.match(r"(?fi)\L<words>", "STRASSE",
          words=options).span(), ascii((0, 7)))

        options = ["kit"]
        # 13..14
        self.expect(lambda: regex.search(r"(?i)\L<words>", "SKITS",
          words=options).span(), ascii((1, 4)))
        self.expect(lambda: regex.search(r"(?i)\L<words>",
          "SK\N{LATIN CAPITAL LETTER I WITH DOT ABOVE}TS",
          words=options).span(), ascii((1, 4)))

        # 15..16
        self.expect(lambda: regex.search(r"(?fi)\b(\w+) +\1\b",
          " stra\N{LATIN SMALL LETTER SHARP S}e STRASSE ").span(), ascii((1,
          15)))
        self.expect(lambda: regex.search(r"(?fi)\b(\w+) +\1\b",
          " STRASSE stra\N{LATIN SMALL LETTER SHARP S}e ").span(), ascii((1,
          15)))

    def test_fuzzy(self):
        # Some tests borrowed from TRE library tests.
        # 1..6
        self.expect(lambda: type(regex.compile('(fou){s,e<=1}')),
          self.PATTERN_CLASS)
        self.expect(lambda: type(regex.compile('(fuu){s}')),
          self.PATTERN_CLASS)
        self.expect(lambda: type(regex.compile('(fuu){s,e}')),
          self.PATTERN_CLASS)
        self.expect(lambda: type(regex.compile('(anaconda){1i+1d<1,s<=1}')),
          self.PATTERN_CLASS)
        self.expect(lambda:
          type(regex.compile('(anaconda){1i+1d<1,s<=1,e<=10}')),
          self.PATTERN_CLASS)
        self.expect(lambda:
          type(regex.compile('(anaconda){s<=1,e<=1,1i+1d<1}')),
          self.PATTERN_CLASS)

        text = 'molasses anaconda foo bar baz smith anderson '
        # 7..11
        self.expect(lambda: regex.search('(znacnda){s<=1,e<=3,1i+1d<1}', text),
          ascii(None))
        self.expect(lambda: regex.search('(znacnda){s<=1,e<=3,1i+1d<2}',
          text).span(0, 1), ascii(((9, 17), (9, 17))))
        self.expect(lambda: regex.search('(ananda){1i+1d<2}', text),
          ascii(None))
        self.expect(lambda: regex.search(r"(?:\bznacnda){e<=2}", text)[0],
          ascii("anaconda"))
        self.expect(lambda: regex.search(r"(?:\bnacnda){e<=2}", text)[0],
          ascii("anaconda"))

        text = 'anaconda foo bar baz smith anderson'
        # 12..16
        self.expect(lambda: regex.search('(fuu){i<=3,d<=3,e<=5}', text).span(0,
          1), ascii(((0, 0), (0, 0))))
        self.expect(lambda: regex.search('(?b)(fuu){i<=3,d<=3,e<=5}',
          text).span(0, 1), ascii(((9, 10), (9, 10))))
        self.expect(lambda: regex.search('(fuu){i<=2,d<=2,e<=5}', text).span(0,
          1), ascii(((9, 10), (9, 10))))
        self.expect(lambda: regex.search('(fuu){i<=3,d<=3,e}', text).span(0,
          1), ascii(((0, 0), (0, 0))))
        self.expect(lambda: regex.search('(?b)(fuu){i<=3,d<=3,e}',
          text).span(0, 1), ascii(((9, 10), (9, 10))))

        # 17
        self.expect(lambda: type(regex.compile('(approximate){s<=3,1i+1d<3}')),
          self.PATTERN_CLASS)

        # No cost limit.
        # 18..19
        self.expect(lambda: regex.search('(foobar){e}',
          'xirefoabralfobarxie').span(0, 1), ascii(((4, 7), (4, 7))))
        self.expect(lambda: regex.search('(?b)(foobar){e}',
          'xirefoabralfobarxie').span(0, 1), ascii(((11, 16), (11, 16))))

        # At most two errors.
        # 20..21
        self.expect(lambda: regex.search('(foobar){e<=2}',
          'xirefoabrzlfd').span(0, 1), ascii(((4, 9), (4, 9))))
        self.expect(lambda: regex.search('(foobar){e<=2}', 'xirefoabzlfd'),
          ascii(None))

        # At most two inserts or substitutions and max two errors total.
        # 22
        self.expect(lambda: regex.search('(foobar){i<=2,s<=2,e<=2}',
          'oobargoobaploowap').span(0, 1), ascii(((5, 11), (5, 11))))

        # Find best whole word match for "foobar".
        # 23..25
        self.expect(lambda: regex.search('\\b(foobar){e}\\b',
          'zfoobarz').span(0, 1), ascii(((0, 8), (0, 8))))
        self.expect(lambda: regex.search('\\b(foobar){e}\\b',
          'boing zfoobarz goobar woop').span(0, 1), ascii(((0, 6), (0, 6))))
        self.expect(lambda: regex.search('(?b)\\b(foobar){e}\\b',
          'boing zfoobarz goobar woop').span(0, 1), ascii(((15, 21), (15, 21))))

        # Match whole string, allow only 1 error.
        # 26..40
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'foobar').span(0,
          1), ascii(((0, 6), (0, 6))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'xfoobar').span(0,
          1), ascii(((0, 7), (0, 7))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$',
         'foobarx').span(0, 1), ascii(((0, 7), (0, 7))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'fooxbar').span(0,
          1), ascii(((0, 7), (0, 7))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'foxbar').span(0,
          1), ascii(((0, 6), (0, 6))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'xoobar').span(0,
          1), ascii(((0, 6), (0, 6))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'foobax').span(0,
          1), ascii(((0, 6), (0, 6))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'oobar').span(0,
          1), ascii(((0, 5), (0, 5))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'fobar').span(0,
          1), ascii(((0, 5), (0, 5))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'fooba').span(0,
          1), ascii(((0, 5), (0, 5))))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'xfoobarx'),
          ascii(None))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'foobarxx'),
          ascii(None))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'xxfoobar'),
          ascii(None))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'xfoxbar'),
          ascii(None))
        self.expect(lambda: regex.search('^(foobar){e<=1}$', 'foxbarx'),
          ascii(None))

        # At most one insert, two deletes, and three substitutions.
        # Additionally, deletes cost two and substitutes one, and total
        # cost must be less than 4.
        # 41..42
        self.expect(lambda: regex.search('(foobar){i<=1,d<=2,s<=3,2d+1s<4}',
          '3oifaowefbaoraofuiebofasebfaobfaorfeoaro').span(0, 1), ascii(((6,
          13), (6, 13))))
        self.expect(lambda:
          regex.search('(?b)(foobar){i<=1,d<=2,s<=3,2d+1s<4}',
          '3oifaowefbaoraofuiebofasebfaobfaorfeoaro').span(0, 1), ascii(((26,
          33), (26, 33))))

        # Partially fuzzy matches.
        # 43..45
        self.expect(lambda: regex.search('foo(bar){e<=1}zap',
          'foobarzap').span(0, 1), ascii(((0, 9), (3, 6))))
        self.expect(lambda: regex.search('foo(bar){e<=1}zap', 'fobarzap'),
          ascii(None))
        self.expect(lambda: regex.search('foo(bar){e<=1}zap',
          'foobrzap').span(0, 1), ascii(((0, 8), (3, 5))))

        # 46..47
        text = ('www.cnn.com 64.236.16.20\nwww.slashdot.org 66.35.250.150\n'
          'For useful information, use www.slashdot.org\nthis is demo data!\n')
        self.expect(lambda: regex.search(r'(?s)^.*(dot.org){e}.*$',
          text).span(0, 1), ascii(((0, 120), (93, 100))))
        self.expect(lambda: regex.search(r'^.*(dot.org){e}.*$', text).span(0,
          1), ascii(((0, 119), (24, 101))))

        # 48..55
        # Behaviour is unexpectd, but arguably not wrong. It first finds the
        # best match, then the best in what follows, etc.
        self.expect(lambda: regex.findall(r"\b\L<words>{e<=1}\b",
          " book cot dog desk ", words="cat dog".split()),
          ascii(["cot", "dog"]))
        self.expect(lambda: regex.findall(r"\b\L<words>{e<=1}\b",
          " book dog cot desk ", words="cat dog".split()),
          ascii(["dog", "cot"]))
        self.expect(lambda: regex.findall(r"(?r)\b\L<words>{e<=1}\b",
          " book cot dog desk ", words="cat dog".split()),
          ascii(["dog", "cot"]))
        self.expect(lambda: regex.findall(r"(?r)\b\L<words>{e<=1}\b",
          " book dog cot desk ", words="cat dog".split()),
          ascii(["cot", "dog"]))
        self.expect(lambda: regex.findall(br"\b\L<words>{e<=1}\b",
          b" book cot dog desk ", words=b"cat dog".split()),
          ascii([b"cot", b"dog"]))
        self.expect(lambda: regex.findall(br"\b\L<words>{e<=1}\b",
          b" book dog cot desk ", words=b"cat dog".split()),
          ascii([b"dog", b"cot"]))
        self.expect(lambda: regex.findall(br"(?r)\b\L<words>{e<=1}\b",
          b" book cot dog desk ", words=b"cat dog".split()),
          ascii([b"dog", b"cot"]))
        self.expect(lambda: regex.findall(br"(?r)\b\L<words>{e<=1}\b",
          b" book dog cot desk ", words=b"cat dog".split()),
          ascii([b"cot", b"dog"]))

        # 56..58
        self.expect(lambda: regex.search(r"(\w+) (\1{e<=1})",
          "foo fou").groups(), ascii(("foo", "fou")))
        self.expect(lambda: regex.search(r"(?r)(\2{e<=1}) (\w+)",
          "foo fou").groups(), ascii(("foo", "fou")))
        self.expect(lambda: regex.search(br"(\w+) (\1{e<=1})",
          b"foo fou").groups(), ascii((b"foo", b"fou")))

        # 59..60
        self.expect(lambda: regex.findall(r"(?:(?:QR)+){e}","abcde"),
          ascii(["abcde", ""]))
        self.expect(lambda: regex.findall(r"(?:Q+){e}","abc"), ascii(["abc",
          ""]))

        # Hg issue 41
        # 61..65
        self.expect(lambda: regex.match(r"(?:service detection){0<e<5}",
          "servic detection").span(), ascii((0, 16)))
        self.expect(lambda: regex.match(r"(?:service detection){0<e<5}",
          "service detect").span(), ascii((0, 14)))
        self.expect(lambda: regex.match(r"(?:service detection){0<e<5}",
          "service detecti").span(), ascii((0, 15)))
        self.expect(lambda: regex.match(r"(?:service detection){0<e<5}",
          "service detection"), ascii(None))
        self.expect(lambda: regex.match(r"(?:service detection){0<e<5}",
          "in service detection").span(), ascii((0, 20)))

    def test_recursive(self):
        # 1..6
        self.expect(lambda: regex.search(r"(\w)(?:(?R)|(\w?))\1",
          "xx")[ : ], ascii(("xx", "x", "")))
        self.expect(lambda: regex.search(r"(\w)(?:(?R)|(\w?))\1",
          "aba")[ : ], ascii(("aba", "a", "b")))
        self.expect(lambda: regex.search(r"(\w)(?:(?R)|(\w?))\1",
          "abba")[ : ], ascii(("abba", "a", None)))
        self.expect(lambda: regex.search(r"(\w)(?:(?R)|(\w?))\1",
          "kayak")[ : ], ascii(("kayak", "k", None)))
        self.expect(lambda: regex.search(r"(\w)(?:(?R)|(\w?))\1",
          "paper")[ : ], ascii(("pap", "p", "a")))
        self.expect(lambda: regex.search(r"(\w)(?:(?R)|(\w?))\1",
          "dontmatchme"), ascii(None))

        # 7..12
        self.expect(lambda: regex.search(r"(?r)\2(?:(\w?)|(?R))(\w)",
          "xx")[ : ], ascii(("xx", "", "x")))
        self.expect(lambda: regex.search(r"(?r)\2(?:(\w?)|(?R))(\w)",
          "aba")[ : ], ascii(("aba", "b", "a")))
        self.expect(lambda: regex.search(r"(?r)\2(?:(\w?)|(?R))(\w)",
          "abba")[ : ], ascii(("abba", None, "a")))
        self.expect(lambda: regex.search(r"(?r)\2(?:(\w?)|(?R))(\w)",
          "kayak")[ : ], ascii(("kayak", None, "k")))
        self.expect(lambda: regex.search(r"(?r)\2(?:(\w?)|(?R))(\w)",
          "paper")[ : ], ascii(("pap", "a", "p")))
        self.expect(lambda: regex.search(r"(?r)\2(?:(\w?)|(?R))(\w)",
          "dontmatchme"), ascii(None))

        # 13..14
        self.expect(lambda: regex.search(r"\(((?>[^()]+)|(?R))*\)",
          "(ab(cd)ef)")[ : ], ascii(("(ab(cd)ef)", "ef")))
        self.expect(lambda: regex.search(r"\(((?>[^()]+)|(?R))*\)",
          "(ab(cd)ef)").captures(1), ascii(["ab", "(cd)", "ef"]))

        # 15..16
        self.expect(lambda: regex.search(r"(?r)\(((?R)|(?>[^()]+))*\)",
          "(ab(cd)ef)")[ : ], ascii(("(ab(cd)ef)", "ab")))
        self.expect(lambda: regex.search(r"(?r)\(((?R)|(?>[^()]+))*\)",
          "(ab(cd)ef)").captures(1), ascii(["ef", "(cd)", "ab"]))

        # 17
        self.expect(lambda: regex.search(r"\(([^()]+|(?R))*\)",
          "some text (a(b(c)d)e) more text")[ : ], ascii(("(a(b(c)d)e)",  "e")))

        # 18
        self.expect(lambda: regex.search(r"(?r)\(((?R)|[^()]+)*\)",
          "some text (a(b(c)d)e) more text")[ : ], ascii(("(a(b(c)d)e)",  "a")))

        # 19
        self.expect(lambda: regex.search(r"(foo(\(((?:(?>[^()]+)|(?2))*)\)))",
          "foo(bar(baz)+baz(bop))")[ : ], ascii(("foo(bar(baz)+baz(bop))",
          "foo(bar(baz)+baz(bop))", "(bar(baz)+baz(bop))",
          "bar(baz)+baz(bop)")))

        # 20
        self.expect(lambda: regex.search(r"(?r)(foo(\(((?:(?2)|(?>[^()]+))*)\)))",
          "foo(bar(baz)+baz(bop))")[ : ], ascii(("foo(bar(baz)+baz(bop))",
          "foo(bar(baz)+baz(bop))", "(bar(baz)+baz(bop))",
          "bar(baz)+baz(bop)")))

        # 21..25
        rgx = regex.compile(r"""^\s*(<\s*([a-zA-Z:]+)(?:\s*[a-zA-Z:]*\s*=\s*(?:'[^']*'|"[^"]*"))*\s*(/\s*)?>(?:[^<>]*|(?1))*(?(3)|<\s*/\s*\2\s*>))\s*$""")
        self.expect(lambda: bool(rgx.search('<foo><bar></bar></foo>')),
          ascii(True))
        self.expect(lambda: bool(rgx.search('<foo><bar></foo></bar>')),
          ascii(False))
        self.expect(lambda: bool(rgx.search('<foo><bar/></foo>')), ascii(True))
        self.expect(lambda: bool(rgx.search('<foo><bar></foo>')), ascii(False))
        self.expect(lambda: bool(rgx.search('<foo bar=baz/>')), ascii(False))

        # 26..30
        self.expect(lambda: bool(rgx.search('<foo bar="baz">')), ascii(False))
        self.expect(lambda: bool(rgx.search('<foo bar="baz"/>')), ascii(True))
        self.expect(lambda: bool(rgx.search('<    fooo   /  >')), ascii(True))
        # The next regex should and does match. Perl 5.14 agrees.
        #self.expect(lambda: bool(rgx.search('<foo/>foo')), ascii(False))
        self.expect(lambda: bool(rgx.search('foo<foo/>')), ascii(False))

        # 30..32
        self.expect(lambda: bool(rgx.search('<foo>foo</foo>')), ascii(True))
        self.expect(lambda: bool(rgx.search('<foo><bar/>foo</foo>')),
          ascii(True))
        self.expect(lambda: bool(rgx.search('<a><b><c></c></b></a>')),
          ascii(True))

    def test_hg_bugs(self):
        # Hg issue 28
        # 1
        self.expect(lambda: bool(regex.compile("(?>b)", flags=regex.V1)),
          ascii(True))

        # Hg issue 29
        # 2
        self.expect(lambda: bool(regex.compile("^((?>\w+)|(?>\s+))*$",
          flags=regex.V1)), ascii(True))

        # Hg issue 31
        # 3..8
        self.expect(lambda: regex.findall(r"\((?:(?>[^()]+)|(?R))*\)",
          "a(bcd(e)f)g(h)"), ascii(['(bcd(e)f)', '(h)']))
        self.expect(lambda: regex.findall(r"\((?:(?:[^()]+)|(?R))*\)",
          "a(bcd(e)f)g(h)"), ascii(['(bcd(e)f)', '(h)']))
        self.expect(lambda: regex.findall(r"\((?:(?>[^()]+)|(?R))*\)",
          "a(b(cd)e)f)g)h"), ascii(['(b(cd)e)']))
        self.expect(lambda: regex.findall(r"\((?:(?>[^()]+)|(?R))*\)",
          "a(bc(d(e)f)gh"), ascii(['(d(e)f)']))
        self.expect(lambda: regex.findall(r"(?r)\((?:(?>[^()]+)|(?R))*\)",
          "a(bc(d(e)f)gh"), ascii(['(d(e)f)']))
        self.expect(lambda: [m.group() for m in
          regex.finditer(r"\((?:[^()]*+|(?0))*\)", "a(b(c(de)fg)h")],
          ascii(['(c(de)fg)']))

        # Hg issue 32
        # 9
        self.expect(lambda: regex.search("a(bc)d", "abcd", regex.I |
          regex.V1).group(0), ascii("abcd"))

        # Hg issue 33
        # 10..11
        self.expect(lambda: regex.search("([\da-f:]+)$", "E", regex.I |
          regex.V1).group(0), ascii("E"))
        self.expect(lambda: regex.search("([\da-f:]+)$", "e", regex.I |
          regex.V1).group(0), ascii("e"))

        # Hg issue 34
        # 12
        self.expect(lambda: regex.search("^(?=ab(de))(abd)(e)",
          "abde").groups(), ascii(('de', 'abd', 'e')))

        # Hg issue 35
        # 13
        self.expect(lambda: bool(regex.match(r"\ ", " ", flags=regex.X)),
          ascii(True))

        # Hg issue 36
        # 14
        self.expect(lambda: regex.search(r"^(a|)\1{2}b", "b").group(0, 1),
          ascii(('b', '')))

        # Hg issue 37
        # 15
        self.expect(lambda: regex.search("^(a){0,0}", "abc").group(0, 1),
          ascii(('', None)))

        # Hg issue 38
        # 16
        self.expect(lambda: regex.search("(?>.*/)b", "a/b").group(0),
          ascii("a/b"))

        # Hg issue 39
        # 17..18
        self.expect(lambda: regex.search(r"(?V0)((?i)blah)\s+\1",
          "blah BLAH").group(0, 1), ascii(("blah BLAH", "blah")))
        self.expect(lambda: regex.search(r"(?V1)((?i)blah)\s+\1", "blah BLAH"),
          ascii(None))

        # Hg issue 40
        # 19
        self.expect(lambda: regex.search(r"(\()?[^()]+(?(1)\)|)",
          "(abcd").group(0), ascii("abcd"))

        # Hg issue 42
        # 20..22
        self.expect(lambda: regex.search("(a*)*", "a").span(1), ascii((1, 1)))
        self.expect(lambda: regex.search("(a*)*", "aa").span(1), ascii((2, 2)))
        self.expect(lambda: regex.search("(a*)*", "aaa").span(1), ascii((3, 3)))

        # Hg issue 43
        # 23
        self.expect(lambda: regex.search("a(?#xxx)*", "aaa").group(),
          ascii("aaa"))

        # Hg issue 44
        # 24
        self.expect(lambda: regex.search("(?=abc){3}abc", "abcabcabc").span(),
          ascii((0, 3)))

        # Hg issue 45
        # 25..26
        self.expect(lambda: regex.search("^(?:a(?:(?:))+)+", "a").span(),
          ascii((0, 1)))
        self.expect(lambda: regex.search("^(?:a(?:(?:))+)+", "aa").span(),
          ascii((0, 2)))

    def run(self):
        print("Performing tests")
        print("================")

        def make_key(name):
            pos = len(name)
            while name[pos - 1].isdigit():
                pos -= 1
            return (name[ : pos], int(name[pos : ] or "0"))

        funcs = [name for name in dir(self) if name.startswith("test_")]
        funcs.sort(key=make_key)

        self.failed_tests = []
        performed_count = 0

        for name in funcs:
            obj = getattr(self, name)
            if hasattr(obj, "__call__"):
                self.performing = name
                performed_count += 1
                self.index = 0
                print(self.performing)
                sys.stdout.flush()
                obj()

        print()
        print("Performed {}".format(performed_count))
        print()

        if self.failed_tests:
            print("Failures")
            print("========")
            current = ""
            failed_count = 0
            for name, index, message in self.failed_tests:
                if name != current:
                    if current:
                        print()
                    current = name
                    failed_count += 1
                    print(current)
                print("\t{}) {}".format(index, message))
            print()
            print("Failed {}".format(failed_count))
        else:
            print("Passed")

if __name__ == "__main__":
    Test().run()
