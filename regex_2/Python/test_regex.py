import regex
import string
import sys
import unicodedata
from weakref import proxy

class Test:
    MATCH_CLASS = "<type '_regex.Match'>"
    PATTERN_CLASS = "<type '_regex.Pattern'>"
    FLAGS_WITH_COMPILED_PAT = """ValueError("can't process flags argument with a compiled pattern",)"""
    INVALID_GROUP_REF = "error('invalid group reference',)"
    MISSING_GT = "error('missing >',)"
    BAD_GROUP_NAME = "error('bad group name',)"
    MISSING_LT = "error('missing <',)"
    UNKNOWN_GROUP_I = "IndexError('unknown group',)"
    UNKNOWN_GROUP = "error('unknown group',)"
    BAD_ESCAPE = "error('bad escape',)"
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

    def __init__(self):
        pass

    def expect(self, code, expected_result):
        self.index += 1
        try:
            actual_result = repr(code())
        except Exception, e:
            actual_result = repr(e)
        if actual_result != expected_result:
            self.record_failure("Expected %s but got %s instead" %
              (expected_result, actual_result))

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
        self.expect(lambda: regex.search('a*', 'xxx').span(0), repr((0, 0)))
        self.expect(lambda: regex.search('x*', 'axx').span(), repr((0, 0)))
        self.expect(lambda: regex.search('x+', 'axx').span(0), repr((1, 3)))
        self.expect(lambda: regex.search('x+', 'axx').span(), repr((1, 3)))
        self.expect(lambda: regex.search('x', 'aaa'), repr(None))
        self.expect(lambda: regex.match('a*', 'xxx').span(0), repr((0, 0)))
        self.expect(lambda: regex.match('a*', 'xxx').span(), repr((0, 0)))
        self.expect(lambda: regex.match('x*', 'xxxa').span(0), repr((0, 3)))
        self.expect(lambda: regex.match('x*', 'xxxa').span(), repr((0, 3)))
        self.expect(lambda: regex.match('a+', 'xxx'), repr(None))

    def bump_num(self, matchobj):
        int_value = int(matchobj[0])
        return str(int_value + 1)

    def test_basic_regex_sub(self):
        self.expect(lambda: regex.sub("(?i)b+", "x", "bbbb BBBB"), repr('x x'))
        self.expect(lambda: regex.sub(r'\d+', self.bump_num, '08.2 -2 23x99y'),
          repr('9.3 -3 24x100y'))
        self.expect(lambda: regex.sub(r'\d+', self.bump_num, '08.2 -2 23x99y',
          3), repr('9.3 -3 23x99y'))

        self.expect(lambda: regex.sub('.', lambda m: r"\n", 'x'), repr("\\n"))
        self.expect(lambda: regex.sub('.', r"\n", 'x'), repr("\n"))

        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a>\g<a>', 'xx'),
          repr('xxxx'))
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a>\g<1>', 'xx'),
          repr('xxxx'))
        self.expect(lambda: regex.sub('(?P<unk>x)', r'\g<unk>\g<unk>', 'xx'),
          repr('xxxx'))
        self.expect(lambda: regex.sub('(?P<unk>x)', r'\g<1>\g<1>', 'xx'),
          repr('xxxx'))

        self.expect(lambda: regex.sub('a',
          r'\t\n\v\r\f\a\b\B\Z\a\A\w\W\s\S\d\D', 'a'),
          repr("\t\n\v\r\f\a\b\\B\\Z\a\\A\\w\\W\\s\\S\\d\\D"))
        self.expect(lambda: regex.sub('a', '\t\n\v\r\f\a', 'a'),
          repr("\t\n\v\r\f\a"))
        self.expect(lambda: regex.sub('a', '\t\n\v\r\f\a', 'a'), repr(chr(9) +
          chr(10) + chr(11) + chr(13) + chr(12) + chr(7)))

        self.expect(lambda: regex.sub(r'^\s*', 'X', 'test'), repr('Xtest'))

    def test_bug_449964(self):
        # Fails for group followed by other escape.
        self.expect(lambda: regex.sub(r'(?P<unk>x)', r'\g<1>\g<1>\b', 'xx'),
          repr("xx\bxx\b"))

    def test_bug_449000(self):
        # Test for sub() on escaped characters.
        self.expect(lambda: regex.sub(r'\r\n', r'\n', 'abc\r\ndef\r\n'),
          repr("abc\ndef\n"))
        self.expect(lambda: regex.sub('\r\n', r'\n', 'abc\r\ndef\r\n'),
          repr("abc\ndef\n"))
        self.expect(lambda: regex.sub(r'\r\n', '\n', 'abc\r\ndef\r\n'),
          repr("abc\ndef\n"))
        self.expect(lambda: regex.sub('\r\n', '\n', 'abc\r\ndef\r\n'),
          repr("abc\ndef\n"))

    def test_bug_1140(self):
        # regex.sub(x, y, u'') should return u'', not '', and
        # regex.sub(x, y, '') should return '', not u''.
        # Also:
        # regex.sub(x, y, unicode(x)) should return unicode(y), and
        # regex.sub(x, y, str(x)) should return
        #     str(y) if isinstance(y, str) else unicode(y).
        for x in 'x', u'x':
            for y in 'y', u'y':
                z = regex.sub(x, y, u'')
                self.expect(lambda: (type(z), z), repr((unicode, u'')))
                z = regex.sub(x, y, '')
                self.expect(lambda: (type(z), z), repr((str, '')))
                z = regex.sub(x, y, unicode(x))
                self.expect(lambda: (type(z), z), repr((unicode, unicode(y))))
                z = regex.sub(x, y, str(x))
                self.expect(lambda: (type(z), z), repr((type(y), y)))

    def test_bug_1661(self):
        # Verify that flags do not get silently ignored with compiled patterns
        pattern = regex.compile('.')
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
        self.expect(lambda: regex.sub('x', r'\0', 'x'), repr("\0"))
        self.expect(lambda: regex.sub('x', r'\000', 'x'), repr("\000"))
        self.expect(lambda: regex.sub('x', r'\001', 'x'), repr("\001"))
        self.expect(lambda: regex.sub('x', r'\008', 'x'), repr("\0" + "8"))
        self.expect(lambda: regex.sub('x', r'\009', 'x'), repr("\0" + "9"))
        self.expect(lambda: regex.sub('x', r'\111', 'x'), repr("\111"))
        self.expect(lambda: regex.sub('x', r'\117', 'x'), repr("\117"))

        self.expect(lambda: regex.sub('x', r'\1111', 'x'), repr("\1111"))
        self.expect(lambda: regex.sub('x', r'\1111', 'x'), repr("\111" + "1"))

        self.expect(lambda: regex.sub('x', r'\00', 'x'), repr('\x00'))
        self.expect(lambda: regex.sub('x', r'\07', 'x'), repr('\x07'))
        self.expect(lambda: regex.sub('x', r'\08', 'x'), repr("\0" + "8"))
        self.expect(lambda: regex.sub('x', r'\09', 'x'), repr("\0" + "9"))
        self.expect(lambda: regex.sub('x', r'\0a', 'x'), repr("\0" + "a"))

        self.expect(lambda: regex.sub('x', r'\400', 'x'), repr("\0"))
        self.expect(lambda: regex.sub('x', r'\777', 'x'), repr("\377"))

        self.expect(lambda: regex.sub('x', r'\1', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\8', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\9', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\11', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\18', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\1a', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\90', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\99', 'x'), self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\118', 'x'),
          self.INVALID_GROUP_REF) # r'\11' + '8'
        self.expect(lambda: regex.sub('x', r'\11a', 'x'),
          self.INVALID_GROUP_REF)
        self.expect(lambda: regex.sub('x', r'\181', 'x'),
          self.INVALID_GROUP_REF) # r'\18' + '1'
        self.expect(lambda: regex.sub('x', r'\800', 'x'),
          self.INVALID_GROUP_REF) # r'\80' + '0'

        # In Python 2.3 (etc), these loop endlessly in sre_parser.py.
        self.expect(lambda: regex.sub('(((((((((((x)))))))))))', r'\11', 'x'),
          repr('x'))
        self.expect(lambda: regex.sub('((((((((((y))))))))))(.)', r'\118',
          'xyz'), repr('xz8'))
        self.expect(lambda: regex.sub('((((((((((y))))))))))(.)', r'\11a',
          'xyz'), repr('xza'))

    def test_qualified_re_sub(self):
        self.expect(lambda: regex.sub('a', 'b', 'aaaaa'), repr('bbbbb'))
        self.expect(lambda: regex.sub('a', 'b', 'aaaaa', 1), repr('baaaa'))

    def test_bug_114660(self):
        self.expect(lambda: regex.sub(r'(\S)\s+(\S)', r'\1 \2', 'hello  there'),
          repr('hello there'))

    def test_bug_462270(self):
        # Test for empty sub() behaviour, see SF bug #462270
        self.expect(lambda: regex.sub('x*', '-', 'abxd'), repr('-a-b--d-'))
        self.expect(lambda: regex.sub('x+', '-', 'abxd'), repr('ab-d'))

    def test_symbolic_refs(self):
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a', 'xx'),
          self.MISSING_GT)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<', 'xx'),
          self.BAD_GROUP_NAME)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g', 'xx'), self.MISSING_LT)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a a>', 'xx'),
          self.MISSING_GT)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<1a1>', 'xx'),
          self.BAD_GROUP_NAME)
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<ab>', 'xx'),
          self.UNKNOWN_GROUP_I)

        # The new behaviour of unmatched but valid groups is to treat them like
        # empty matches in the replacement template, like in Perl.
        self.expect(lambda: regex.sub('(?P<a>x)|(?P<b>y)', r'\g<b>', 'xx'),
          repr(''))
        self.expect(lambda: regex.sub('(?P<a>x)|(?P<b>y)', r'\2', 'xx'),
          repr(''))

        # The old behaviour was to raise it as an IndexError.
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<-1>', 'xx'),
          self.BAD_GROUP_NAME)

    def test_re_subn(self):
        self.expect(lambda: regex.subn("(?i)b+", "x", "bbbb BBBB"), repr(('x x',
          2)))
        self.expect(lambda: regex.subn("b+", "x", "bbbb BBBB"), repr(('x BBBB',
          1)))
        self.expect(lambda: regex.subn("b+", "x", "xyz"), repr(('xyz', 0)))
        self.expect(lambda: regex.subn("b*", "x", "xyz"), repr(('xxxyxzx', 4)))
        self.expect(lambda: regex.subn("b*", "x", "xyz", 2), repr(('xxxyz', 2)))

    def test_re_split(self):
        self.expect(lambda: regex.split(":", ":a:b::c"), repr(['', 'a', 'b', '',
          'c']))
        self.expect(lambda: regex.split(":*", ":a:b::c"), repr(['', 'a', 'b',
          'c']))
        self.expect(lambda: regex.split("(:*)", ":a:b::c"), repr(['', ':', 'a',
          ':', 'b', '::', 'c']))
        self.expect(lambda: regex.split("(?::*)", ":a:b::c"), repr(['', 'a',
          'b', 'c']))
        self.expect(lambda: regex.split("(:)*", ":a:b::c"), repr(['', ':', 'a',
          ':', 'b', ':', 'c']))
        self.expect(lambda: regex.split("([b:]+)", ":a:b::c"), repr(['', ':',
          'a', ':b::', 'c']))
        self.expect(lambda: regex.split("(b)|(:+)", ":a:b::c"), repr(['', None,
          ':', 'a', None, ':', '', 'b', None, '', None, '::', 'c']))
        self.expect(lambda: regex.split("(?:b)|(?::+)", ":a:b::c"), repr(['',
          'a', '', '', 'c']))

        self.expect(lambda: regex.split("x", "xaxbxc"), repr(['', 'a', 'b',
          'c']))
        self.expect(lambda: [m for m in regex.splititer("x", "xaxbxc")],
          repr(['', 'a', 'b', 'c']))

        self.expect(lambda: regex.split("(?r)x", "xaxbxc"), repr(['c', 'b', 'a',
          '']))
        self.expect(lambda: [m for m in regex.splititer("(?r)x", "xaxbxc")],
          repr(['c', 'b', 'a', '']))

        self.expect(lambda: regex.split("(x)|(y)", "xaxbxc"), repr(['', 'x',
          None, 'a', 'x', None, 'b', 'x', None, 'c']))
        self.expect(lambda: [m for m in regex.splititer("(x)|(y)", "xaxbxc")],
          repr(['', 'x', None, 'a', 'x', None, 'b', 'x', None, 'c']))

        self.expect(lambda: regex.split("(?r)(x)|(y)", "xaxbxc"), repr(['c',
          'x', None, 'b', 'x', None, 'a', 'x', None, '']))
        self.expect(lambda: [m for m in regex.splititer("(?r)(x)|(y)",
          "xaxbxc")], repr(['c', 'x', None, 'b', 'x', None, 'a', 'x', None,
          '']))

    def test_qualified_re_split(self):
        self.expect(lambda: regex.split(":", ":a:b::c", 2), repr(['', 'a',
          'b::c']))
        self.expect(lambda: regex.split(':', 'a:b:c:d', 2), repr(['a', 'b',
          'c:d']))
        self.expect(lambda: regex.split("(:)", ":a:b::c", 2), repr(['', ':',
          'a', ':', 'b::c']))
        self.expect(lambda: regex.split("(:*)", ":a:b::c", 2), repr(['', ':',
          'a', ':', 'b::c']))

    def test_re_findall(self):
        self.expect(lambda: regex.findall(":+", "abc"), repr([]))
        self.expect(lambda: regex.findall(":+", "a:b::c:::d"), repr([':', '::',
          ':::']))
        self.expect(lambda: regex.findall("(:+)", "a:b::c:::d"), repr([':',
          '::', ':::']))
        self.expect(lambda: regex.findall("(:)(:*)", "a:b::c:::d"), repr([(':',
          ''), (':', ':'), (':', '::')]))

        self.expect(lambda: regex.findall(r"\((?P<test>.{0,5}?TEST)\)",
          "(MY TEST)"), repr(["MY TEST"]))
        self.expect(lambda: regex.findall(r"\((?P<test>.{0,3}?TEST)\)",
          "(MY TEST)"), repr(["MY TEST"]))
        self.expect(lambda: regex.findall(r"\((?P<test>.{0,3}?T)\)",
          "(MY T)"), repr(["MY T"]))

        self.expect(lambda: regex.findall(r"[^a]{2}[A-Z]", "\n  S"),
          repr(['  S']))
        self.expect(lambda: regex.findall(r"[^a]{2,3}[A-Z]", "\n  S"),
          repr(['\n  S']))
        self.expect(lambda: regex.findall(r"[^a]{2,3}[A-Z]", "\n   S"),
          repr(['   S']))

        self.expect(lambda: regex.findall(r"X(Y[^Y]+?){1,2}( |Q)+DEF",
          "XYABCYPPQ\nQ DEF"), repr([('YPPQ\n', ' ')]))

        self.expect(lambda: regex.findall(r"(\nTest(\n+.+?){0,2}?)?\n+End",
          "\nTest\nxyz\nxyz\nEnd"), repr([('\nTest\nxyz\nxyz', '\nxyz')]))

    def test_bug_117612(self):
        self.expect(lambda: regex.findall(r"(a|(b))", "aba"), repr([('a', ''),
          ('b', 'b'), ('a', '')]))

    def test_re_match(self):
        self.expect(lambda: regex.match('a', 'a')[:], repr(('a',)))
        self.expect(lambda: regex.match('(a)', 'a')[:], repr(('a', 'a')))
        self.expect(lambda: regex.match(r'(a)', 'a')[0], repr('a'))
        self.expect(lambda: regex.match(r'(a)', 'a')[1], repr('a'))
        self.expect(lambda: regex.match(r'(a)', 'a').group(1, 1), repr(('a',
          'a')))

        pat = regex.compile('((a)|(b))(c)?')
        self.expect(lambda: pat.match('a')[:], repr(('a', 'a', 'a', None,
          None)))
        self.expect(lambda: pat.match('b')[:], repr(('b', 'b', None, 'b',
          None)))
        self.expect(lambda: pat.match('ac')[:], repr(('ac', 'a', 'a', None,
          'c')))
        self.expect(lambda: pat.match('bc')[:], repr(('bc', 'b', None, 'b',
          'c')))
        self.expect(lambda: pat.match('bc')[:], repr(('bc', 'b', None, 'b',
          'c')))

        # A single group.
        m = regex.match('(a)', 'a')
        self.expect(lambda: m.group(), repr('a'))
        self.expect(lambda: m.group(0), repr('a'))
        self.expect(lambda: m.group(1), repr('a'))
        self.expect(lambda: m.group(1, 1), repr(('a', 'a')))

        pat = regex.compile('(?:(?P<a1>a)|(?P<b2>b))(?P<c3>c)?')
        self.expect(lambda: pat.match('a').group(1, 2, 3), repr(('a', None,
          None)))
        self.expect(lambda: pat.match('b').group('a1', 'b2', 'c3'), repr((None,
          'b', None)))
        self.expect(lambda: pat.match('ac').group(1, 'b2', 3), repr(('a', None,
          'c')))

    def test_re_groupref_exists(self):
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', '(a)')[:],
          repr(('(a)', '(', 'a')))
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', 'a')[:],
          repr(('a', None, 'a')))
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', 'a)'),
          repr(None))
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', '(a'),
          repr(None))
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)b|d))$', 'ab')[:],
          repr(('ab', 'a', 'b')))
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)b|d))$', 'cd')[:],
          repr(('cd', None, 'd')))
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)|d))$', 'cd')[:],
          repr(('cd', None, 'd')))
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)|d))$', 'a')[:],
          repr(('a', 'a', '')))

        # Tests for bug #1177831: exercise groups other than the first group.
        p = regex.compile('(?P<g1>a)(?P<g2>b)?((?(g2)c|d))')
        self.expect(lambda: p.match('abc')[:], repr(('abc', 'a', 'b', 'c')))
        self.expect(lambda: p.match('ad')[:], repr(('ad', 'a', None, 'd')))
        self.expect(lambda: p.match('abd'), repr(None))
        self.expect(lambda: p.match('ac'), repr(None))

    def test_re_groupref(self):
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1$', '|a|')[:],
          repr(('|a|', '|', 'a')))
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1?$', 'a')[:],
          repr(('a', None, 'a')))
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1$', 'a|'), repr(None))
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1$', '|a'), repr(None))
        self.expect(lambda: regex.match(r'^(?:(a)|c)(\1)$', 'aa')[:],
          repr(('aa', 'a', 'a')))
        self.expect(lambda: regex.match(r'^(?:(a)|c)(\1)?$', 'c')[:], repr(('c',
          None, None)))

        self.expect(lambda:
          regex.findall("(?i)(.{1,40}?),(.{1,40}?)(?:;)+(.{1,80}).{1,40}?\\3(\ |;)+(.{1,80}?)\\1",
          "TEST, BEST; LEST ; Lest 123 Test, Best"),
          repr([('TEST', ' BEST', ' LEST', ' ', '123 ')]))

    def test_groupdict(self):
        self.expect(lambda: regex.match('(?P<first>first) (?P<second>second)',
          'first second').groupdict(), repr({'first': 'first', 'second':
          'second'}))

    def test_expand(self):
        self.expect(lambda: regex.match("(?P<first>first) (?P<second>second)",
          "first second").expand(r"\2 \1 \g<second> \g<first>"),
          repr('second first second first'))

    def test_repeat_minmax(self):
        self.expect(lambda: regex.match(r"^(\w){1}$", "abc"), repr(None))
        self.expect(lambda: regex.match(r"^(\w){1}?$", "abc"), repr(None))
        self.expect(lambda: regex.match(r"^(\w){1,2}$", "abc"), repr(None))
        self.expect(lambda: regex.match(r"^(\w){1,2}?$", "abc"), repr(None))

        self.expect(lambda: regex.match(r"^(\w){3}$", "abc")[1], repr('c'))
        self.expect(lambda: regex.match(r"^(\w){1,3}$", "abc")[1], repr('c'))
        self.expect(lambda: regex.match(r"^(\w){1,4}$", "abc")[1], repr('c'))
        self.expect(lambda: regex.match(r"^(\w){3,4}?$", "abc")[1], repr('c'))
        self.expect(lambda: regex.match(r"^(\w){3}?$", "abc")[1], repr('c'))
        self.expect(lambda: regex.match(r"^(\w){1,3}?$", "abc")[1], repr('c'))
        self.expect(lambda: regex.match(r"^(\w){1,4}?$", "abc")[1], repr('c'))
        self.expect(lambda: regex.match(r"^(\w){3,4}?$", "abc")[1], repr('c'))

        self.expect(lambda: regex.match("^x{1}$", "xxx"), repr(None))
        self.expect(lambda: regex.match("^x{1}?$", "xxx"), repr(None))
        self.expect(lambda: regex.match("^x{1,2}$", "xxx"), repr(None))
        self.expect(lambda: regex.match("^x{1,2}?$", "xxx"), repr(None))

        self.expect(lambda: regex.match("^x{1}", "xxx")[0], repr('x'))
        self.expect(lambda: regex.match("^x{1}?", "xxx")[0], repr('x'))
        self.expect(lambda: regex.match("^x{0,1}", "xxx")[0], repr('x'))
        self.expect(lambda: regex.match("^x{0,1}?", "xxx")[0], repr(''))

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

        self.expect(lambda: regex.match("^x{}$", "xxx"), repr(None))
        self.expect(lambda: type(regex.match("^x{}$", "x{}")), self.MATCH_CLASS)

    def test_getattr(self):
        self.expect(lambda: regex.compile("(?i)(a)(b)").pattern,
          repr('(?i)(a)(b)'))
        self.expect(lambda: regex.compile("(?i)(a)(b)").flags, repr(regex.I))
        self.expect(lambda: regex.compile(u"(?i)(a)(b)").flags, repr(regex.I))
        self.expect(lambda: regex.compile("(?i)(a)(b)").groups, repr(2))
        self.expect(lambda: regex.compile("(?i)(a)(b)").groupindex, repr({}))
        self.expect(lambda:
          regex.compile("(?i)(?P<first>a)(?P<other>b)").groupindex,
          repr({'first': 1, 'other': 2}))

        self.expect(lambda: regex.match("(a)", "a").pos, repr(0))
        self.expect(lambda: regex.match("(a)", "a").endpos, repr(1))

        self.expect(lambda: regex.search("b(c)", "abcdef").pos, repr(0))
        self.expect(lambda: regex.search("b(c)", "abcdef").endpos, repr(6))
        self.expect(lambda: regex.search("b(c)", "abcdef").span(), repr((1,
          3)))
        self.expect(lambda: regex.search("b(c)", "abcdef").span(1), repr((2,
          3)))

        self.expect(lambda: regex.match("(a)", "a").string, repr('a'))
        self.expect(lambda: regex.match("(a)", "a").regs, repr(((0, 1), (0,
          1))))
        self.expect(lambda: type(regex.match("(a)", "a").re),
          self.PATTERN_CLASS)

    def test_special_escapes(self):
        self.expect(lambda: regex.search(r"\b(b.)\b", "abcd abc bcd bx")[1],
          repr('bx'))
        self.expect(lambda: regex.search(r"\B(b.)\B", "abc bcd bc abxd")[1],
          repr('bx'))
        self.expect(lambda: regex.search(r"\b(b.)\b", "abcd abc bcd bx",
          regex.LOCALE)[1], repr('bx'))
        self.expect(lambda: regex.search(r"\B(b.)\B", "abc bcd bc abxd",
          regex.LOCALE)[1], repr('bx'))
        self.expect(lambda: regex.search(ur"\b(b.)\b", u"abcd abc bcd bx",
          regex.UNICODE)[1], repr(u'bx'))
        self.expect(lambda: regex.search(ur"\B(b.)\B", u"abc bcd bc abxd",
          regex.UNICODE)[1], repr(u'bx'))
        self.expect(lambda: regex.search(r"^abc$", "\nabc\n", regex.M)[0],
          repr('abc'))
        self.expect(lambda: regex.search(r"^\Aabc\Z$", "abc", regex.M)[0],
          repr('abc'))
        self.expect(lambda: regex.search(r"^\Aabc\Z$", "\nabc\n", regex.M),
          repr(None))
        self.expect(lambda: regex.search(ur"\b(b.)\b", u"abcd abc bcd bx")[1],
          repr(u'bx'))
        self.expect(lambda: regex.search(ur"\B(b.)\B", u"abc bcd bc abxd")[1],
          repr(u'bx'))
        self.expect(lambda: regex.search(ur"^abc$", u"\nabc\n", regex.M)[0],
          repr(u'abc'))
        self.expect(lambda: regex.search(ur"^\Aabc\Z$", u"abc", regex.M)[0],
          repr(u'abc'))
        self.expect(lambda: regex.search(ur"^\Aabc\Z$", u"\nabc\n", regex.M),
          repr(None))
        self.expect(lambda: regex.search(r"\d\D\w\W\s\S", "1aa! a")[0],
          repr('1aa! a'))
        self.expect(lambda: regex.search(r"\d\D\w\W\s\S", "1aa! a",
          regex.LOCALE)[0], repr('1aa! a'))
        self.expect(lambda: regex.search(ur"\d\D\w\W\s\S", u"1aa! a",
          regex.UNICODE)[0], repr(u'1aa! a'))

    def test_bigcharset(self):
        self.expect(lambda: regex.match(ur"(?u)([\u2222\u2223])", u"\u2222")[1],
          repr(u'\u2222'))
        self.expect(lambda: regex.match(ur"(?u)([\u2222\u2223])", u"\u2222",
          regex.UNICODE)[1], repr(u'\u2222'))
        self.expect(lambda: u"".join(regex.findall(u".",
          u"e\xe8\xe9\xea\xeb\u0113\u011b\u0117", flags=regex.UNICODE)),
          repr(u'e\xe8\xe9\xea\xeb\u0113\u011b\u0117'))
        self.expect(lambda:
          u"".join(regex.findall(ur"[e\xe8\xe9\xea\xeb\u0113\u011b\u0117]",
          u"e\xe8\xe9\xea\xeb\u0113\u011b\u0117", flags=regex.UNICODE)),
          repr(u'e\xe8\xe9\xea\xeb\u0113\u011b\u0117'))
        self.expect(lambda:
          u"".join(regex.findall(ur"e|\xe8|\xe9|\xea|\xeb|\u0113|\u011b|\u0117",
          u"e\xe8\xe9\xea\xeb\u0113\u011b\u0117", flags=regex.UNICODE)),
          repr(u'e\xe8\xe9\xea\xeb\u0113\u011b\u0117'))

    def test_anyall(self):
        self.expect(lambda: regex.match("a.b", "a\nb", regex.DOTALL)[0],
          repr("a\nb"))
        self.expect(lambda: regex.match("a.*b", "a\n\nb", regex.DOTALL)[0],
          repr("a\n\nb"))

    def test_non_consuming(self):
        self.expect(lambda: regex.match(r"(a(?=\s[^a]))", "a b")[1], repr('a'))
        self.expect(lambda: regex.match(r"(a(?=\s[^a]*))", "a b")[1], repr('a'))
        self.expect(lambda: regex.match(r"(a(?=\s[abc]))", "a b")[1], repr('a'))
        self.expect(lambda: regex.match(r"(a(?=\s[abc]*))", "a bc")[1],
          repr('a'))
        self.expect(lambda: regex.match(r"(a)(?=\s\1)", "a a")[1], repr('a'))
        self.expect(lambda: regex.match(r"(a)(?=\s\1*)", "a aa")[1], repr('a'))
        self.expect(lambda: regex.match(r"(a)(?=\s(abc|a))", "a a")[1],
          repr('a'))

        self.expect(lambda: regex.match(r"(a(?!\s[^a]))", "a a")[1], repr('a'))
        self.expect(lambda: regex.match(r"(a(?!\s[abc]))", "a d")[1], repr('a'))
        self.expect(lambda: regex.match(r"(a)(?!\s\1)", "a b")[1], repr('a'))
        self.expect(lambda: regex.match(r"(a)(?!\s(abc|a))", "a b")[1],
          repr('a'))

    def test_ignore_case(self):
        self.expect(lambda: regex.match("abc", "ABC", regex.I)[0], repr('ABC'))
        self.expect(lambda: regex.match(u"abc", u"ABC", regex.I)[0],
          repr(u'ABC'))

        self.expect(lambda: regex.match(r"(a\s[^a]*)", "a bb", regex.I)[1],
          repr('a bb'))
        self.expect(lambda: regex.match(r"(a\s[abc])", "a b", regex.I)[1],
          repr('a b'))
        self.expect(lambda: regex.match(r"(a\s[abc]*)", "a bb", regex.I)[1],
          repr('a bb'))
        self.expect(lambda: regex.match(r"((a)\s\2)", "a a", regex.I)[1],
          repr('a a'))
        self.expect(lambda: regex.match(r"((a)\s\2*)", "a aa", regex.I)[1],
          repr('a aa'))
        self.expect(lambda: regex.match(r"((a)\s(abc|a))", "a a", regex.I)[1],
          repr('a a'))
        self.expect(lambda: regex.match(r"((a)\s(abc|a)*)", "a aa", regex.I)[1],
          repr('a aa'))

        # Issue #3511.
        self.expect(lambda: regex.match(r"[Z-a]", "_").span(), repr((0, 1)))
        self.expect(lambda: regex.match(r"(?i)[Z-a]", "_").span(), repr((0, 1)))

    def test_category(self):
        self.expect(lambda: regex.match(r"(\s)", " ")[1], repr(' '))

    def test_not_literal(self):
        self.expect(lambda: regex.search(r"\s([^a])", " b")[1], repr('b'))
        self.expect(lambda: regex.search(r"\s([^a]*)", " bb")[1], repr('bb'))

    def test_search_coverage(self):
        self.expect(lambda: regex.search(r"\s(b)", " b")[1], repr('b'))
        self.expect(lambda: regex.search(r"a\s", "a ")[0], repr('a '))

    def test_re_escape(self):
        p = ""
        self.expect(lambda: regex.escape(p), repr(p))
        for i in range(0, 256):
            p += chr(i)
            self.expect(lambda: type(regex.match(regex.escape(chr(i)), chr(i))),
              self.MATCH_CLASS)
            self.expect(lambda: regex.match(regex.escape(chr(i)),
              chr(i)).span(), repr((0, 1)))

        pat = regex.compile(regex.escape(p))
        self.expect(lambda: pat.match(p).span(), repr((0, 256)))

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
            self.expect(lambda: type(regex.match(r"\x%02x0" % i, chr(i) + "0")),
              self.MATCH_CLASS)
            self.expect(lambda: type(regex.match(r"\x%02xz" % i, chr(i) + "z")),
              self.MATCH_CLASS)
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
        self.expect(lambda: regex.match(r"[\911]", ""), self.BAD_ESCAPE)

    def test_bug_113254(self):
        self.expect(lambda: regex.match(r'(a)|(b)', 'b').start(1), repr(-1))
        self.expect(lambda: regex.match(r'(a)|(b)', 'b').end(1), repr(-1))
        self.expect(lambda: regex.match(r'(a)|(b)', 'b').span(1), repr((-1,
          -1)))

    def test_bug_527371(self):
        # Bug described in patches 527371/672491.
        self.expect(lambda: regex.match(r'(a)?a','a').lastindex, repr(None))
        self.expect(lambda: regex.match(r'(a)(b)?b','ab').lastindex, repr(1))
        self.expect(lambda: regex.match(r'(?P<a>a)(?P<b>b)?b','ab').lastgroup,
          repr('a'))
        self.expect(lambda: regex.match("(?P<a>a(b))", "ab").lastgroup,
          repr('a'))
        self.expect(lambda: regex.match("((a))", "a").lastindex, repr(1))

    def test_bug_545855(self):
        # Bug 545855 -- This pattern failed to cause a compile error as it
        # should, instead provoking a TypeError.
        self.expect(lambda: regex.compile('foo[a-'), self.BAD_SET)

    def test_bug_418626(self):
        # Bugs 418626 at al. -- Testing Greg Chapman's addition of op code
        # SRE_OP_MIN_REPEAT_ONE for eliminating recursion on simple uses of
        # pattern '*?' on a long string.
        self.expect(lambda: regex.match('.*?c', 10000 * 'ab' + 'cd').end(0),
          repr(20001))
        self.expect(lambda: regex.match('.*?cd', 5000 * 'ab' + 'c' + 5000 * 'ab'
          + 'cde').end(0), repr(20003))
        self.expect(lambda: regex.match('.*?cd', 20000 * 'abc' + 'de').end(0),
          repr(60001))
        # Non-simple '*?' still used to hit the recursion limit, before the
        # non-recursive scheme was implemented.
        self.expect(lambda: regex.search('(a|b)*?c', 10000 * 'ab' +
          'cd').end(0), repr(20001))

    def test_bug_612074(self):
        pat = u"[" + regex.escape(u"\u2039") + u"]"
        self.expect(lambda: regex.compile(pat) and 1, repr(1))

    def test_stack_overflow(self):
        # Nasty cases that used to overflow the straightforward recursive
        # implementation of repeated groups.
        self.expect(lambda: regex.match('(x)*', 50000 * 'x')[1], repr('x'))
        self.expect(lambda: regex.match('(x)*y', 50000 * 'x' + 'y')[1],
          repr('x'))
        self.expect(lambda: regex.match('(x)*?y', 50000 * 'x' + 'y')[1],
          repr('x'))

    def test_scanner(self):
        def s_ident(scanner, token): return token
        def s_operator(scanner, token): return "op%s" % token
        def s_float(scanner, token): return float(token)
        def s_int(scanner, token): return int(token)

        scanner = regex.Scanner([
          (r"[a-zA-Z_]\w*", s_ident), (r"\d+\.\d*", s_float), (r"\d+", s_int),
          (r"=|\+|-|\*|/", s_operator), (r"\s+", None),
        ])

        self.expect(lambda: type(scanner.scanner.scanner("").pattern),
          self.PATTERN_CLASS)

        self.expect(lambda: scanner.scan("sum = 3*foo + 312.50 + bar"),
          repr((['sum', 'op=', 3, 'op*', 'foo', 'op+', 312.5, 'op+', 'bar'],
          '')))

    def test_bug_448951(self):
        # Bug 448951 (similar to 429357, but with single char match).
        # (Also test greedy matches.)
        for op in '', '?', '*':
            self.expect(lambda: regex.match(r'((.%s):)?z' % op, 'z')[:],
              repr(('z', None, None)))
            self.expect(lambda: regex.match(r'((.%s):)?z' % op, 'a:z')[:],
              repr(('a:z', 'a:', 'a')))

    def test_bug_725106(self):
        # Capturing groups in alternatives in repeats.
        self.expect(lambda: regex.match('^((a)|b)*', 'abc')[:], repr(('ab', 'b',
          'a')))
        self.expect(lambda: regex.match('^(([ab])|c)*', 'abc')[:], repr(('abc',
          'c', 'b')))
        self.expect(lambda: regex.match('^((d)|[ab])*', 'abc')[:], repr(('ab',
          'b', None)))
        self.expect(lambda: regex.match('^((a)c|[ab])*', 'abc')[:], repr(('ab',
          'b', None)))
        self.expect(lambda: regex.match('^((a)|b)*?c', 'abc')[:], repr(('abc',
          'b', 'a')))
        self.expect(lambda: regex.match('^(([ab])|c)*?d', 'abcd')[:],
          repr(('abcd', 'c', 'b')))
        self.expect(lambda: regex.match('^((d)|[ab])*?c', 'abc')[:],
          repr(('abc', 'b', None)))
        self.expect(lambda: regex.match('^((a)c|[ab])*?c', 'abc')[:],
          repr(('abc', 'b', None)))

    def test_bug_725149(self):
        # Mark_stack_base restoring before restoring marks.
        self.expect(lambda: regex.match('(a)(?:(?=(b)*)c)*', 'abb')[:],
          repr(('a', 'a', None)))
        self.expect(lambda: regex.match('(a)((?!(b)*))*', 'abb')[:],
          repr(('a', 'a', None, None)))

    def test_bug_764548(self):
        # Bug 764548, regex.compile() barfs on str/unicode subclasses.
        class my_unicode(unicode): pass
        pat = regex.compile(my_unicode("abc"))
        self.expect(lambda: pat.match("xyz"), repr(None))

    def test_finditer(self):
        it = regex.finditer(r":+", "a:b::c:::d")
        self.expect(lambda: [item[0] for item in it], repr([':', '::', ':::']))

    def test_bug_926075(self):
        self.index += 1
        if regex.compile('bug_926075') is regex.compile(u'bug_926075'):
            self.record_failure()

    def test_bug_931848(self):
        pattern = u"[\u002E\u3002\uFF0E\uFF61]"
        self.expect(lambda: regex.compile(pattern).split("a.b.c"),
          repr(['a', 'b', 'c']))

    def test_bug_581080(self):
        it = regex.finditer(r"\s", "a b")
        self.expect(lambda: it.next().span(), repr((1, 2)))
        self.expect(lambda: it.next(), repr(StopIteration()))

        scanner = regex.compile(r"\s").scanner("a b")
        self.expect(lambda: scanner.search().span(), repr((1, 2)))
        self.expect(lambda: scanner.search(), repr(None))

    def test_bug_817234(self):
        it = regex.finditer(r".*", "asdf")
        self.expect(lambda: it.next().span(), repr((0, 4)))
        self.expect(lambda: it.next().span(), repr((4, 4)))
        self.expect(lambda: it.next(), repr(StopIteration()))

    def test_empty_array(self):
        # SF buf 1647541.
        import array
        for typecode in 'cbBuhHiIlLfd':
            a = array.array(typecode)
            self.expect(lambda: regex.compile("bla").match(a), repr(None))
            self.expect(lambda: regex.compile("").match(a)[1 : ], repr(()))

    def test_inline_flags(self):
        # Bug #1700.
        upper_char = unichr(0x1ea0) # Latin Capital Letter A with Dot Below
        lower_char = unichr(0x1ea1) # Latin Small Letter A with Dot Below

        p = regex.compile(upper_char, regex.I | regex.U)
        self.expect(lambda: type(p.match(lower_char)), self.MATCH_CLASS)

        p = regex.compile(lower_char, regex.I | regex.U)
        self.expect(lambda: type(p.match(upper_char)), self.MATCH_CLASS)

        p = regex.compile('(?i)' + upper_char, regex.U)
        self.expect(lambda: type(p.match(lower_char)), self.MATCH_CLASS)

        p = regex.compile('(?i)' + lower_char, regex.U)
        self.expect(lambda: type(p.match(upper_char)), self.MATCH_CLASS)

        p = regex.compile('(?iu)' + upper_char)
        self.expect(lambda: type(p.match(lower_char)), self.MATCH_CLASS)

        p = regex.compile('(?iu)' + lower_char)
        self.expect(lambda: type(p.match(upper_char)), self.MATCH_CLASS)

        self.expect(lambda: type(regex.match(r"(?i)a", "A")), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r"a(?i)", "A")), self.MATCH_CLASS)
        self.expect(lambda: type(regex.match(r"(?in)a", "A")), self.MATCH_CLASS)
        self.expect(lambda: regex.match(r"a(?in)", "A"), repr(None))

    def test_dollar_matches_twice(self):
        # $ matches the end of string, and just before the terminating \n.
        pattern = regex.compile('$')
        self.expect(lambda: pattern.sub('#', 'a\nb\n'), repr('a\nb#\n#'))
        self.expect(lambda: pattern.sub('#', 'a\nb\nc'), repr('a\nb\nc#'))
        self.expect(lambda: pattern.sub('#', '\n'), repr('#\n#'))

        pattern = regex.compile('$', regex.MULTILINE)
        self.expect(lambda: pattern.sub('#', 'a\nb\n' ), repr('a#\nb#\n#'))
        self.expect(lambda: pattern.sub('#', 'a\nb\nc'), repr('a#\nb#\nc#'))
        self.expect(lambda: pattern.sub('#', '\n'), repr('#\n#'))

    def test_ascii_and_unicode_flag(self):
        # Unicode patterns.
        for flags in (0, regex.UNICODE):
            pat = regex.compile(u'\xc0', flags | regex.IGNORECASE)
            self.expect(lambda: type(pat.match(u'\xe0')), self.MATCH_CLASS)
            pat = regex.compile(u'\w', flags)
            self.expect(lambda: type(pat.match(u'\xe0')), self.MATCH_CLASS)
        pat = regex.compile(u'\xc0', regex.ASCII | regex.IGNORECASE)
        self.expect(lambda: pat.match(u'\xe0'), repr(None))
        pat = regex.compile(u'(?a)\xc0', regex.IGNORECASE)
        self.expect(lambda: pat.match(u'\xe0'), repr(None))
        pat = regex.compile(u'\w', regex.ASCII)
        self.expect(lambda: pat.match(u'\xe0'), repr(None))
        pat = regex.compile(u'(?a)\w')
        self.expect(lambda: pat.match(u'\xe0'), repr(None))
        # String patterns.
        for flags in (0, regex.ASCII):
            pat = regex.compile('\xc0', flags | regex.IGNORECASE)
            self.expect(lambda: pat.match('\xe0'), repr(None))
            pat = regex.compile('\w')
            self.expect(lambda: pat.match('\xe0'), repr(None))
        self.expect(lambda: regex.compile('(?au)\w'), self.MIXED_FLAGS)

    def test_subscripting_match(self):
        self.index += 1
        m = regex.match(r'(?<a>\w)', 'xy')
        if m[0] != m.group(0) or m[1] != m.group(1):
            record_failure("Failed")
        self.index += 1
        if m[:] != ('x', 'x'):
            self.record_failure("Failed: expected \"('x', 'x')\" but got %s instead"
             % repr(m[:]))

    def test_new_named_groups(self):
        self.index += 1
        m0 = regex.match(r'(?P<a>\w)', 'x')
        m1 = regex.match(r'(?<a>\w)', 'x')
        if not (m0 and m1 and m0[:] == m1[:]):
            self.record_failure("Failed")

    def test_properties(self):
        chars_u = u"-09AZaz_\u0393\u03b3"
        chars_b = "-09AZaz_"
        word_set = set("Ll Lm Lo Lt Lu Mc Me Mn Nd Nl No Pc".split())

        tests = [
            (ur"(?u)\w", chars_u, u"09AZaz_\u0393\u03b3"),
            (ur"(?u)[[:word:]]", chars_u, u"09AZaz_\u0393\u03b3"),
            (ur"(?u)\W", chars_u, u"-"),
            (ur"(?u)[[:^word:]]", chars_u, u"-"),
            (ur"(?u)\d", chars_u, u"09"),
            (ur"(?u)[[:digit:]]", chars_u, u"09"),
            (ur"(?u)\D", chars_u, u"-AZaz_\u0393\u03b3"),
            (ur"(?u)[[:^digit:]]", chars_u, u"-AZaz_\u0393\u03b3"),
            (ur"(?u)[[:alpha:]]", chars_u, u"AZaz\u0393\u03b3"),
            (ur"(?u)[[:^alpha:]]", chars_u, u"-09_"),
            (ur"(?u)[[:alnum:]]", chars_u, u"09AZaz\u0393\u03b3"),
            (ur"(?u)[[:^alnum:]]", chars_u, u"-_"),
            (ur"(?u)[[:xdigit:]]", chars_u, u"09Aa"),
            (ur"(?u)[[:^xdigit:]]", chars_u, u"-Zz_\u0393\u03b3"),

            (r"(?L)\w", chars_b, "09AZaz_"),
            (r"(?L)[[:word:]]", chars_b, "09AZaz_"),
            (r"(?L)\W", chars_b, "-"),
            (r"(?L)[[:^word:]]", chars_b, "-"),
            (r"(?L)\d", chars_b, "09"),
            (r"(?L)[[:digit:]]", chars_b, "09"),
            (r"(?L)\D", chars_b, "-AZaz_"),
            (r"(?L)[[:^digit:]]", chars_b, "-AZaz_"),
            (r"(?L)[[:alpha:]]", chars_b, "AZaz"),
            (r"(?L)[[:^alpha:]]", chars_b, "-09_"),
            (r"(?L)[[:alnum:]]", chars_b, "09AZaz"),
            (r"(?L)[[:^alnum:]]", chars_b, "-_"),
            (r"(?L)[[:xdigit:]]", chars_b, "09Aa"),
            (r"(?L)[[:^xdigit:]]", chars_b, "-Zz_"),

            (r"\w", chars_b, "09AZaz_"),
            (r"[[:word:]]", chars_b, "09AZaz_"),
            (r"\W", chars_b, "-"),
            (r"[[:^word:]]", chars_b, "-"),
            (r"\d", chars_b, "09"),
            (r"[[:digit:]]", chars_b, "09"),
            (r"\D", chars_b, "-AZaz_"),
            (r"[[:^digit:]]", chars_b, "-AZaz_"),
            (r"[[:alpha:]]", chars_b, "AZaz"),
            (r"[[:^alpha:]]", chars_b, "-09_"),
            (r"[[:alnum:]]", chars_b, "09AZaz"),
            (r"[[:^alnum:]]", chars_b, "-_"),
            (r"[[:xdigit:]]", chars_b, "09Aa"),
            (r"[[:^xdigit:]]", chars_b, "-Zz_"),
        ]
        for pattern, chars, expected in tests:
            self.index += 1
            try:
                if chars[ : 0].join(regex.findall(pattern, chars)) != expected:
                    self.record_failure("Failed: %s" % pattern)
            except Exception, e:
                self.record_failure("Failed: %s raised %s" % (pattern, repr(e)))

    def test_word_class(self):
        self.expect(lambda: regex.findall(ur"(?u)\w+",
          u" \u0939\u093f\u0928\u094d\u0926\u0940,"),
          repr([u'\u0939\u093f\u0928\u094d\u0926\u0940']))
        self.expect(lambda: regex.findall(ur"(?u)\W+",
          u" \u0939\u093f\u0928\u094d\u0926\u0940,"), repr([u' ', u',']))
        self.expect(lambda: regex.split(ur"(?un)\b",
          u" \u0939\u093f\u0928\u094d\u0926\u0940,"), repr([u' ',
          u'\u0939\u093f\u0928\u094d\u0926\u0940', u',']))
        self.expect(lambda: regex.split(ur"(?un)\B",
          u" \u0939\u093f\u0928\u094d\u0926\u0940,"), repr([u'', u' \u0939',
          u'\u093f', u'\u0928', u'\u094d', u'\u0926', u'\u0940,', u'']))

    def test_search_anchor(self):
        self.expect(lambda: regex.findall(r"\G\w{2}", "abcd ef"), repr(['ab',
          'cd']))

    def test_search_reverse(self):
        self.expect(lambda: regex.findall(r"(?r).", "abc"), repr(['c', 'b',
          'a']))
        self.expect(lambda: regex.findall(r"(?r).", "abc", overlapped=True),
          repr(['c', 'b', 'a']))
        self.expect(lambda: regex.findall(r"(?r)..", "abcde"), repr(['de',
          'bc']))
        self.expect(lambda: regex.findall(r"(?r)..", "abcde", overlapped=True),
          repr(['de', 'cd', 'bc', 'ab']))
        self.expect(lambda: regex.findall(r"(?r)(.)(-)(.)", "a-b-c",
          overlapped=True), repr([("b", "-", "c"), ("a", "-", "b")]))

        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).", "abc")],
          repr(['c', 'b', 'a']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde",
          overlapped=True)], repr(['de', 'cd', 'bc', 'ab']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).",
          "abc")],repr(['c', 'b', 'a']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde",
          overlapped=True)],repr(['de', 'cd', 'bc', 'ab']))

        self.expect(lambda: regex.findall(r"^|\w+", "foo bar"), repr(['', 'foo',
          'bar']))
        self.expect(lambda: regex.findall(r"(?n)^|\w+", "foo bar"), repr(['',
          'foo', 'bar']))
        self.expect(lambda: regex.findall(r"(?r)^|\w+", "foo bar"), repr(['bar',
          'foo', '']))
        self.expect(lambda: regex.findall(r"(?nr)^|\w+", "foo bar"),
          repr(['bar', 'foo', '']))

        self.expect(lambda: [m[0] for m in regex.finditer(r"^|\w+", "foo bar")],
          repr(['', 'foo', 'bar']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?n)^|\w+",
          "foo bar")], repr(['', 'foo', 'bar']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)^|\w+",
          "foo bar")], repr(['bar', 'foo', '']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?nr)^|\w+",
          "foo bar")], repr(['bar', 'foo', '']))

        self.expect(lambda: regex.findall(r"\G\w{2}", "abcd ef"), repr(['ab',
          'cd']))
        self.expect(lambda: regex.findall(r".{2}(?<=\G.*)", "abcd"), repr(['ab',
          'cd']))
        self.expect(lambda: regex.findall(r"(?r)\G\w{2}", "abcd ef"), repr([]))
        self.expect(lambda: regex.findall(r"(?r)\w{2}\G", "abcd ef"),
          repr(['ef']))

        self.expect(lambda: regex.findall(r"q*", "qqwe"), repr(['qq', '', '',
          '']))
        self.expect(lambda: regex.findall(r"(?n)q*", "qqwe"), repr(['qq', '',
          '', '']))
        self.expect(lambda: regex.findall(r"(?r)q*", "qqwe"), repr(['', '',
          'qq', '']))
        self.expect(lambda: regex.findall(r"(?nr)q*", "qqwe"), repr(['', '',
          'qq', '']))

        self.expect(lambda: regex.findall(".", "abcd", pos=1, endpos=3),
          repr(['b', 'c']))
        self.expect(lambda: regex.findall(".", "abcd", pos=1, endpos=-1),
          repr(['b', 'c']))
        self.expect(lambda: [m[0] for m in regex.finditer(".", "abcd", pos=1,
          endpos=3)], repr(['b', 'c']))
        self.expect(lambda: [m[0] for m in regex.finditer(".", "abcd", pos=1,
          endpos=-1)], repr(['b', 'c']))

        self.expect(lambda: [m[0] for m in regex.finditer("(?r).", "abcd",
          pos=1, endpos=3)], repr(['c', 'b']))
        self.expect(lambda: [m[0] for m in regex.finditer("(?r).", "abcd",
          pos=1, endpos=-1)], repr(['c', 'b']))
        self.expect(lambda: regex.findall("(?r).", "abcd", pos=1, endpos=3),
          repr(['c', 'b']))
        self.expect(lambda: regex.findall("(?r).", "abcd", pos=1, endpos=-1),
          repr(['c', 'b']))

        self.expect(lambda: regex.findall(r"[ab]", "aB", regex.I), repr(['a',
          'B']))
        self.expect(lambda: regex.findall(r"(?r)[ab]", "aB", regex.I),
          repr(['B', 'a']))

        self.expect(lambda: regex.findall(r"(?r).{2}", "abc"), repr(['bc']))
        self.expect(lambda: regex.findall(r"(?r).{2}", "abc", overlapped=True),
          repr(['bc', 'ab']))
        self.expect(lambda: regex.findall(r"(\w+) (\w+)",
          "first second third fourth fifth"), repr([('first', 'second'),
          ('third', 'fourth')]))
        self.expect(lambda: regex.findall(r"(?r)(\w+) (\w+)",
          "first second third fourth fifth"), repr([('fourth', 'fifth'),
          ('second', 'third')]))

        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).{2}", "abc")],
          repr(['bc']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).{2}", "abc",
          overlapped=True)], repr(['bc', 'ab']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(\w+) (\w+)",
          "first second third fourth fifth")], repr(['first second',
          'third fourth']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)(\w+) (\w+)",
          "first second third fourth fifth")], repr(['fourth fifth',
          'second third']))

        self.expect(lambda: regex.search("abcdef", "abcdef").span(), repr((0,
          6)))
        self.expect(lambda: regex.search("(?r)abcdef", "abcdef").span(),
          repr((0, 6)))
        self.expect(lambda: regex.search("(?i)abcdef", "ABCDEF").span(),
          repr((0, 6)))
        self.expect(lambda: regex.search("(?ir)abcdef", "ABCDEF").span(),
          repr((0, 6)))

        self.expect(lambda: regex.sub(r"(.)", r"\1", "abc"), repr('abc'))
        self.expect(lambda: regex.sub(r"(?r)(.)", r"\1", "abc"), repr('abc'))

    def test_atomic(self):
        # Issue 433030.
        self.expect(lambda: regex.search(r"(?>a*)a", "aa"), repr(None))

    def test_possessive(self):
        # Single-character non-possessive.
        self.expect(lambda: regex.search(r"a?a", "a").span(), repr((0, 1)))
        self.expect(lambda: regex.search(r"a*a", "aaa").span(), repr((0, 3)))
        self.expect(lambda: regex.search(r"a+a", "aaa").span(), repr((0, 3)))
        self.expect(lambda: regex.search(r"a{1,3}a", "aaa").span(), repr((0,
          3)))

        # Multiple-character non-possessive.
        self.expect(lambda: regex.search(r"(?:ab)?ab", "ab").span(), repr((0,
          2)))
        self.expect(lambda: regex.search(r"(?:ab)*ab", "ababab").span(),
          repr((0, 6)))
        self.expect(lambda: regex.search(r"(?:ab)+ab", "ababab").span(),
          repr((0, 6)))
        self.expect(lambda: regex.search(r"(?:ab){1,3}ab", "ababab").span(),
          repr((0, 6)))

        # Single-character possessive.
        self.expect(lambda: regex.search(r"a?+a", "a"), repr(None))
        self.expect(lambda: regex.search(r"a*+a", "aaa"), repr(None))
        self.expect(lambda: regex.search(r"a++a", "aaa"), repr(None))
        self.expect(lambda: regex.search(r"a{1,3}+a", "aaa"), repr(None))

        # Multiple-character possessive.
        self.expect(lambda: regex.search(r"(?:ab)?+ab", "ab"), repr(None))
        self.expect(lambda: regex.search(r"(?:ab)*+ab", "ababab"), repr(None))
        self.expect(lambda: regex.search(r"(?:ab)++ab", "ababab"), repr(None))
        self.expect(lambda: regex.search(r"(?:ab){1,3}+ab", "ababab"),
          repr(None))

    def test_zerowidth(self):
        # Issue 3262.
        self.expect(lambda: regex.split(r"\b", "a b"), repr(['a b']))
        self.expect(lambda: regex.split(r"(?n)\b", "a b"), repr(['', 'a', ' ',
          'b', '']))

        # Issue 1647489.
        self.expect(lambda: regex.findall(r"^|\w+", "foo bar"), repr(['', 'foo',
          'bar']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"^|\w+", "foo bar")],
          repr(['', 'foo', 'bar']))
        self.expect(lambda: regex.findall(r"(?r)^|\w+", "foo bar"), repr(['bar',
          'foo', '']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)^|\w+",
          "foo bar")], repr(['bar', 'foo', '']))
        self.expect(lambda: regex.findall(r"(?n)^|\w+", "foo bar"),
          repr(['', 'foo', 'bar']))
        self.expect(lambda: [m[0] for m in
          regex.finditer(r"(?n)^|\w+", "foo bar")], repr(['', 'foo', 'bar']))
        self.expect(lambda: regex.findall(r"(?nr)^|\w+", "foo bar"),
          repr(['bar', 'foo', '']))
        self.expect(lambda: [m[0] for m in
          regex.finditer(r"(?nr)^|\w+", "foo bar")], repr(['bar', 'foo', '']))

        self.expect(lambda: regex.split("", "xaxbxc"), repr(['xaxbxc']))
        self.expect(lambda: [m for m in regex.splititer("", "xaxbxc")],
          repr(['xaxbxc']))

        self.expect(lambda: regex.split("(?r)", "xaxbxc"), repr(['xaxbxc']))
        self.expect(lambda: [m for m in regex.splititer("(?r)", "xaxbxc")],
          repr(['xaxbxc']))

        self.expect(lambda: regex.split("(?n)", "xaxbxc"),
          repr(['', 'x', 'a', 'x', 'b', 'x', 'c', '']))
        self.expect(lambda: [m for m in regex.splititer("(?n)", "xaxbxc")],
          repr(['', 'x', 'a', 'x', 'b', 'x', 'c', '']))

        self.expect(lambda: regex.split("(?nr)", "xaxbxc"),
          repr(['', 'c', 'x', 'b', 'x', 'a', 'x', '']))
        self.expect(lambda: [m for m in regex.splititer("(?nr)", "xaxbxc")],
          repr(['', 'c', 'x', 'b', 'x', 'a', 'x', '']))

    def test_scoped_and_inline_flags(self):
        # Issues 433028, #433024, #433027.
        self.expect(lambda: regex.search(r"(?i)Ab", "ab").span(), repr((0, 2)))
        self.expect(lambda: regex.search(r"(?i:A)b", "ab").span(), repr((0, 2)))
        self.expect(lambda: regex.search(r"A(?i)b", "ab").span(), repr((0, 2)))
        self.expect(lambda: regex.search(r"A(?in)b", "ab"), repr(None))

        self.expect(lambda: regex.search(r"(?-i)Ab", "ab",
          flags=regex.I).span(), repr((0, 2)))
        self.expect(lambda: regex.search(r"(?n-i)Ab", "ab", flags=regex.I),
          repr(None))
        self.expect(lambda: regex.search(r"(?-i:A)b", "ab", flags=regex.I),
          repr(None))
        self.expect(lambda: regex.search(r"A(?-i)b", "ab",
          flags=regex.I).span(), repr((0, 2)))

    def test_repeated_repeats(self):
        # Issue 2537.
        self.expect(lambda: regex.search(r"(?:a+)+", "aaa").span(),
          repr((0, 3)))
        self.expect(lambda: regex.search(r"(?:(?:ab)+c)+", "abcabc").span(),
          repr((0, 6)))

    def test_lookbehind(self):
        self.expect(lambda: regex.search(r"123(?<=a\d+)", "a123").span(),
          repr((1, 4)))
        self.expect(lambda: regex.search(r"123(?<=a\d+)", "b123"), repr(None))
        self.expect(lambda: regex.search(r"123(?<!a\d+)", "a123"), repr(None))
        self.expect(lambda: regex.search(r"123(?<!a\d+)", "b123").span(),
          repr((1, 4)))

        self.expect(lambda: type(regex.match("(a)b(?<=b)(c)", "abc")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match("(a)b(?<=c)(c)", "abc"), repr(None))
        self.expect(lambda: type(regex.match("(a)b(?=c)(c)", "abc")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match("(a)b(?=b)(c)", "abc"), repr(None))

        self.expect(lambda: regex.match("(?:(a)|(x))b(?<=(?(2)x|c))c", "abc"),
          repr(None))
        self.expect(lambda: regex.match("(?:(a)|(x))b(?<=(?(2)b|x))c", "abc"),
          repr(None))
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?<=(?(2)x|b))c",
          "abc")), self.MATCH_CLASS)
        self.expect(lambda: regex.match("(?:(a)|(x))b(?<=(?(1)c|x))c", "abc"),
          repr(None))
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?<=(?(1)b|x))c",
          "abc")), self.MATCH_CLASS)

        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?=(?(2)x|c))c",
          "abc")), self.MATCH_CLASS)
        self.expect(lambda: regex.match("(?:(a)|(x))b(?=(?(2)c|x))c", "abc"),
          repr(None))
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?=(?(2)x|c))c",
          "abc")), self.MATCH_CLASS)
        self.expect(lambda: regex.match("(?:(a)|(x))b(?=(?(1)b|x))c", "abc"),
          repr(None))
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?=(?(1)c|x))c",
          "abc")), self.MATCH_CLASS)

        self.expect(lambda: regex.match("(a)b(?<=(?(2)x|c))(c)", "abc"),
          repr(None))
        self.expect(lambda: regex.match("(a)b(?<=(?(2)b|x))(c)", "abc"),
          repr(None))
        self.expect(lambda: regex.match("(a)b(?<=(?(1)c|x))(c)", "abc"),
          repr(None))
        self.expect(lambda: type(regex.match("(a)b(?<=(?(1)b|x))(c)", "abc")),
          self.MATCH_CLASS)

        self.expect(lambda: type(regex.match("(a)b(?=(?(2)x|c))(c)", "abc")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match("(a)b(?=(?(2)b|x))(c)", "abc"),
          repr(None))
        self.expect(lambda: type(regex.match("(a)b(?=(?(1)c|x))(c)", "abc")),
          self.MATCH_CLASS)

        self.expect(lambda: type(regex.compile(r"(a)\2(b)")),
          self.PATTERN_CLASS)

    def test_unmatched_in_sub(self):
        # Issue 1519638.
        self.expect(lambda: regex.sub(r"(x)?(y)?", r"\2-\1", "xy"),
          repr('y-x-'))
        self.expect(lambda: regex.sub(r"(x)?(y)?", r"\2-\1", "x"), repr('-x-'))
        self.expect(lambda: regex.sub(r"(x)?(y)?", r"\2-\1", "y"), repr('y--'))

    def test_bug_10328 (self):
        # Issue 10328.
        pat = regex.compile(r'(?m)(?P<trailing_ws>[ \t]+\r*$)|(?P<no_final_newline>(?<=[^\n])\Z)')
        self.expect(lambda: pat.subn(lambda m: '<' + m.lastgroup + '>',
          'foobar '), repr(('foobar<trailing_ws><no_final_newline>', 2)))
        self.expect(lambda: [m.group() for m in pat.finditer('foobar ')],
          repr([' ', '']))

    def test_overlapped(self):
        self.expect(lambda: regex.findall(r"..", "abcde"), repr(['ab', 'cd']))
        self.expect(lambda: regex.findall(r"..", "abcde", overlapped=True),
          repr(['ab', 'bc', 'cd', 'de']))
        self.expect(lambda: regex.findall(r"(?r)..", "abcde"), repr(['de',
          'bc']))
        self.expect(lambda: regex.findall(r"(?r)..", "abcde", overlapped=True),
          repr(['de', 'cd', 'bc', 'ab']))
        self.expect(lambda: regex.findall(r"(.)(-)(.)", "a-b-c",
          overlapped=True), repr([("a", "-", "b"), ("b", "-", "c")]))

        self.expect(lambda: [m[0] for m in regex.finditer(r"..", "abcde")],
          repr(['ab', 'cd']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"..", "abcde",
          overlapped=True)], repr(['ab', 'bc', 'cd', 'de']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde")],
          repr(['de', 'bc']))
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde",
          overlapped=True)], repr(['de', 'cd', 'bc', 'ab']))

        self.expect(lambda: [m.groups() for m in regex.finditer(r"(.)(-)(.)",
          "a-b-c", overlapped=True)], repr([("a", "-", "b"), ("b", "-", "c")]))
        self.expect(lambda: [m.groups() for m in regex.finditer(r"(?r)(.)(-)(.)",
          "a-b-c", overlapped=True)], repr([("b", "-", "c"), ("a", "-", "b")]))

    def test_splititer(self):
        self.expect(lambda: regex.split(r",", "a,b,,c,"),
          repr(['a', 'b', '', 'c', '']))
        self.expect(lambda: [m for m in regex.splititer(r",", "a,b,,c,")],
          repr(['a', 'b', '', 'c', '']))

    def test_grapheme(self):
        self.expect(lambda: regex.match(ur"(?u)\X", u"\xE0").span(), repr((0,
          1)))
        self.expect(lambda: regex.match(ur"(?u)\X", u"a\u0300").span(),
          repr((0, 2)))

        self.expect(lambda: regex.findall(ur"(?u)\X",
          u"a\xE0a\u0300e\xE9e\u0301"),
          repr([u'a', u'\xe0', u'a\u0300', u'e', u'\xe9', u'e\u0301']))
        self.expect(lambda: regex.findall(ur"(?u)\X{3}",
          u"a\xE0a\u0300e\xE9e\u0301"),
          repr([u'a\xe0a\u0300', u'e\xe9e\u0301']))

    def test_word_boundary(self):
        text = u'The quick ("brown") fox can\'t jump 32.3 feet, right?'
        self.expect(lambda: regex.split(ur'(?n)\b', text), repr([u'', u'The',
          u' ', u'quick', u' ("', u'brown', u'") ', u'fox', u' ', u'can', u"'",
          u't', u' ', u'jump', u' ', u'32', u'.', u'3', u' ', u'feet', u', ',
          u'right', u'?']))
        self.expect(lambda: regex.split(ur'(?wn)\b', text), repr([u'', u'The',
          u' ', u'quick', u' ', u'(', u'"', u'brown', u'"', u')', u' ', u'fox',
          u' ', u"can't", u' ', u'jump', u' ', u'32.3', u' ', u'feet', u',',
          u' ', u'right', u'?', u'']))

        text = u"The  fox"
        self.expect(lambda: regex.split(ur'(?n)\b', text), repr([u'', u'The',
          u'  ', u'fox', u'']))
        self.expect(lambda: regex.split(ur'(?wn)\b', text), repr([u'', u'The',
          u' ', u' ', u'fox', u'']))

        text = u"can't aujourd'hui l'objectif"
        self.expect(lambda: regex.split(ur'(?n)\b', text), repr([u'', u'can',
          u"'", u't', u' ', u'aujourd', u"'", u'hui', u' ', u'l', u"'",
          u'objectif', u'']))
        self.expect(lambda: regex.split(ur'(?wn)\b', text), repr([u'',
          u"can't", u' ', u"aujourd'hui", u' ', u"l'", u'objectif', u'']))

    def test_branch_reset(self):
        self.expect(lambda: regex.match(r"(?:(a)|(b))(c)", "ac").groups(),
          repr(('a', None, 'c')))
        self.expect(lambda: regex.match(r"(?:(a)|(b))(c)", "bc").groups(),
          repr((None, 'b', 'c')))
        self.expect(lambda:
          regex.match(r"(?:(?<a>a)|(?<b>b))(?<c>c)", "ac").groups(),
          repr(('a', None, 'c')))
        self.expect(lambda:
          regex.match(r"(?:(?<a>a)|(?<b>b))(?<c>c)", "bc").groups(),
            repr((None, 'b', 'c')))
        self.expect(lambda:
          regex.match(r"(?<a>a)(?:(?<b>b)|(?<c>c))(?<d>d)", "abd").groups(),
          repr(('a', 'b', None, 'd')))
        self.expect(lambda:
          regex.match(r"(?<a>a)(?:(?<b>b)|(?<c>c))(?<d>d)", "acd").groups(),
          repr(('a', None, 'c', 'd')))
        self.expect(lambda: regex.match(r"(a)(?:(b)|(c))(d)", "abd").groups(),
         repr(('a', 'b', None, 'd')))
        self.expect(lambda: regex.match(r"(a)(?:(b)|(c))(d)", "acd").groups(),
          repr(('a', None, 'c', 'd')))
        self.expect(lambda: regex.match(r"(a)(?|(b)|(b))(d)", "abd").groups(),
          repr(('a', 'b', 'd')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)|(?<b>b))(c)", "ac").groups(),
          repr(('a', None, 'c')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)|(?<b>b))(c)", "bc").groups(),
          repr((None, 'b', 'c')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)|(?<a>b))(c)", "ac").groups(), repr(('a',
            'c')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)|(?<a>b))(c)", "bc").groups(), repr(('b',
            'c')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(?<a>d))(e)", "abe").groups(),
          repr(('a', 'b', 'e')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(?<a>d))(e)", "cde").groups(),
          repr(('d', 'c', 'e')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(d))(e)", "abe").groups(),
          repr(('a', 'b', 'e')))
        self.expect(lambda: regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(d))(e)",
          "cde").groups(), repr(('d', 'c', 'e')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(c)(d))(e)", "abe").groups(),
          repr(('a', 'b', 'e')))
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(c)(d))(e)", "cde").groups(),
          repr(('c', 'd', 'e')))
        self.expect(lambda: regex.match(r"(?|(?<a>a)(?<b>b)|(c)(?<a>d))(e)",
          "abe"), self.DUPLICATE_GROUP)
        self.expect(lambda: regex.match(r"(?|(?<a>a)(?<b>b)|(c)(?<a>d))(e)",
          "cde"), self.DUPLICATE_GROUP)

    def test_set(self):
        self.expect(lambda: regex.match(r"[a]", "a").span(), repr((0, 1)))
        self.expect(lambda: regex.match(r"(?i)[a]", "A").span(), repr((0, 1)))
        self.expect(lambda: regex.match(r"[a-b]", r"a").span(), repr((0, 1)))
        self.expect(lambda: regex.match(r"(?i)[a-b]", r"A").span(), repr((0,
          1)))

        self.expect(lambda: regex.findall(ur"[\p{Alpha}]", u"a0"),
          repr([u"a"]))
        self.expect(lambda: regex.findall(ur"(?i)[\p{Alpha}]", u"A0"),
          repr([u"A"]))

        self.expect(lambda: regex.findall(ur"[a\p{Alpha}]", u"ab0"),
          repr([u"a", u"b"]))
        self.expect(lambda: regex.findall(ur"[a\P{Alpha}]", u"ab0"),
          repr([u"a", u"0"]))
        self.expect(lambda: regex.findall(ur"(?i)[a\p{Alpha}]", u"ab0"),
          repr([u"a", u"b"]))
        self.expect(lambda: regex.findall(ur"(?i)[a\P{Alpha}]", u"ab0"),
          repr([u"a", u"0"]))

        self.expect(lambda: regex.findall(ur"[a-b\p{Alpha}]", u"abC0"),
          repr([u"a", u"b", u"C"]))
        self.expect(lambda: regex.findall(ur"(?i)[a-b\p{Alpha}]", u"AbC0"),
          repr([u"A", u"b", u"C"]))

        self.expect(lambda: regex.findall(ur"[\p{Alpha}]", u"a0"),
          repr([u"a"]))
        self.expect(lambda: regex.findall(ur"[\P{Alpha}]", u"a0"),
          repr([u"0"]))
        self.expect(lambda: regex.findall(ur"[^\p{Alpha}]", u"a0"),
          repr([u"0"]))
        self.expect(lambda: regex.findall(ur"[^\P{Alpha}]", u"a0"),
          repr([u"a"]))

        self.expect(lambda: "".join(regex.findall(r"[^\d-h]", "a^b12c-h")),
          repr('a^bc'))
        self.expect(lambda: "".join(regex.findall(r"[^\dh]", "a^b12c-h")),
          repr('a^bc-'))
        self.expect(lambda: "".join(regex.findall(r"[^h\s\db]", "a^b 12c-h")),
          repr('a^c-'))
        self.expect(lambda: "".join(regex.findall(r"[^b\w]", "a b")), repr(' '))
        self.expect(lambda: "".join(regex.findall(r"[^b\S]", "a b")), repr(' '))
        self.expect(lambda: "".join(regex.findall(r"[^8\d]", "a 1b2")),
          repr('a b'))

    def test_various(self):
        tests = [
            # Test ?P< and ?P= extensions.
            ('(?P<foo_123', '', '', self.MISSING_GT),      # Unterminated group identifier.
            ('(?P<1>a)', '', '', self.BAD_GROUP_NAME),     # Begins with a digit.
            ('(?P<!>a)', '', '', self.BAD_GROUP_NAME),     # Begins with an illegal char.
            ('(?P<foo!>a)', '', '', self.MISSING_GT),      # Begins with an illegal char.

            # Same tests, for the ?P= form.
            ('(?P<foo_123>a)(?P=foo_123', 'aa', '', self.MISSING_RPAREN),
            ('(?P<foo_123>a)(?P=1)', 'aa', '', self.BAD_GROUP_NAME),
            ('(?P<foo_123>a)(?P=!)', 'aa', '', self.BAD_GROUP_NAME),
            ('(?P<foo_123>a)(?P=foo_124)', 'aa', '', self.UNKNOWN_GROUP),  # Backref to undefined group.

            ('(?P<foo_123>a)', 'a', '1', repr('a')),
            ('(?P<foo_123>a)(?P=foo_123)', 'aa', '1', repr('a')),

            # Mal-formed \g in pattern treated as literal for compatibility.
            (r'(?<foo_123>a)\g<foo_123', 'aa', '', repr(None)),
            (r'(?<foo_123>a)\g<1>', 'aa', '1', repr('a')),
            (r'(?<foo_123>a)\g<!>', 'aa', '', repr(None)),
            (r'(?<foo_123>a)\g<foo_124>', 'aa', '', self.UNKNOWN_GROUP),  # Backref to undefined group.

            ('(?<foo_123>a)', 'a', '1', repr('a')),
            (r'(?<foo_123>a)\g<foo_123>', 'aa', '1', repr('a')),

            # Test octal escapes.
            ('\\1', 'a', '', self.UNKNOWN_GROUP),    # Backreference.
            ('[\\1]', '\1', '0', "'\\x01'"),  # Character.
            ('\\09', chr(0) + '9', '0', repr(chr(0) + '9')),
            ('\\141', 'a', '0', repr('a')),
            ('(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)\\119', 'abcdefghijklk9',
              '0,11', repr(('abcdefghijklk9', 'k'))),

            # Test \0 is handled everywhere.
            (r'\0', '\0', '0', repr('\0')),
            (r'[\0a]', '\0', '0', repr('\0')),
            (r'[a\0]', '\0', '0', repr('\0')),
            (r'[^a\0]', '\0', '', repr(None)),

            # Test various letter escapes.
            (r'\a[\b]\f\n\r\t\v', '\a\b\f\n\r\t\v', '0',
              repr('\a\b\f\n\r\t\v')),
            (r'[\a][\b][\f][\n][\r][\t][\v]', '\a\b\f\n\r\t\v', '0',
              repr('\a\b\f\n\r\t\v')),
            (r'\c\e\g\h\i\j\k\m\o\p\q\y\z', 'ceghijkmopqyz', '0',
              repr('ceghijkmopqyz')),
            (r'\xff', '\377', '0', repr(chr(255))),
            # New \x semantics.
            (r'\x00ffffffffffffff', '\377', '', repr(None)),
            (r'\x00f', '\017', '', repr(None)),
            (r'\x00fe', '\376', '', repr(None)),

            (r'\x00ff', '\377', '', repr(None)),
            (r'\t\n\v\r\f\a\g', '\t\n\v\r\f\ag', '0', repr('\t\n\v\r\f\ag')),
            ('\t\n\v\r\f\a\g', '\t\n\v\r\f\ag', '0', repr('\t\n\v\r\f\ag')),
            (r'\t\n\v\r\f\a', '\t\n\v\r\f\a', '0', repr(chr(9) + chr(10) +
              chr(11) + chr(13) + chr(12) + chr(7))),
            (r'[\t][\n][\v][\r][\f][\b]', '\t\n\v\r\f\b', '0',
              repr('\t\n\v\r\f\b')),

            (r"^\w+=(\\[\000-\277]|[^\n\\])*",
              "SRC=eval.c g.c blah blah blah \\\\\n\tapes.c", '0',
              repr("SRC=eval.c g.c blah blah blah \\\\")),

            # Test that . only matches \n in DOTALL mode.
            ('a.b', 'acb', '0', repr('acb')),
            ('a.b', 'a\nb', '', repr(None)),
            ('a.*b', 'acc\nccb', '', repr(None)),
            ('a.{4,5}b', 'acc\nccb', '', repr(None)),
            ('a.b', 'a\rb', '0', repr('a\rb')),
            # The new behaviour is that the inline flag affects only what follows.
            ('a.b(?s)', 'a\nb', '0', repr('a\nb')),
            ('a.b(?ns)', 'a\nb', '', repr(None)),
            ('(?s)a.b', 'a\nb', '0', repr('a\nb')),
            ('a.*(?s)b', 'acc\nccb', '0', repr('acc\nccb')),
            ('a.*(?ns)b', 'acc\nccb', '', repr(None)),
            ('(?s)a.*b', 'acc\nccb', '0', repr('acc\nccb')),
            ('(?s)a.{4,5}b', 'acc\nccb', '0', repr('acc\nccb')),

            (')', '', '', self.TRAILING_CHARS),           # Unmatched right bracket.
            ('', '', '0', "''"),    # Empty pattern.
            ('abc', 'abc', '0', repr('abc')),
            ('abc', 'xbc', '', repr(None)),
            ('abc', 'axc', '', repr(None)),
            ('abc', 'abx', '', repr(None)),
            ('abc', 'xabcy', '0', repr('abc')),
            ('abc', 'ababc', '0', repr('abc')),
            ('ab*c', 'abc', '0', repr('abc')),
            ('ab*bc', 'abc', '0', repr('abc')),
            ('ab*bc', 'abbc', '0', repr('abbc')),
            ('ab*bc', 'abbbbc', '0', repr('abbbbc')),
            ('ab+bc', 'abbc', '0', repr('abbc')),
            ('ab+bc', 'abc', '', repr(None)),
            ('ab+bc', 'abq', '', repr(None)),
            ('ab+bc', 'abbbbc', '0', repr('abbbbc')),
            ('ab?bc', 'abbc', '0', repr('abbc')),
            ('ab?bc', 'abc', '0', repr('abc')),
            ('ab?bc', 'abbbbc', '', repr(None)),
            ('ab?c', 'abc', '0', repr('abc')),
            ('^abc$', 'abc', '0', repr('abc')),
            ('^abc$', 'abcc', '', repr(None)),
            ('^abc', 'abcc', '0', repr('abc')),
            ('^abc$', 'aabc', '', repr(None)),
            ('abc$', 'aabc', '0', repr('abc')),
            ('^', 'abc', '0', repr('')),
            ('$', 'abc', '0', repr('')),
            ('a.c', 'abc', '0', repr('abc')),
            ('a.c', 'axc', '0', repr('axc')),
            ('a.*c', 'axyzc', '0', repr('axyzc')),
            ('a.*c', 'axyzd', '', repr(None)),
            ('a[bc]d', 'abc', '', repr(None)),
            ('a[bc]d', 'abd', '0', repr('abd')),
            ('a[b-d]e', 'abd', '', repr(None)),
            ('a[b-d]e', 'ace', '0', repr('ace')),
            ('a[b-d]', 'aac', '0', repr('ac')),
            ('a[-b]', 'a-', '0', repr('a-')),
            ('a[\\-b]', 'a-', '0', repr('a-')),
            ('a[b-]', 'a-', '0', repr('a-')),
            ('a[]b', '-', '', self.BAD_SET),
            ('a[', '-', '', self.BAD_SET),
            ('a\\', '-', '', self.BAD_ESCAPE),
            ('abc)', '-', '', self.TRAILING_CHARS),
            ('(abc', '-', '', self.MISSING_RPAREN),
            ('a]', 'a]', '0', repr('a]')),
            ('a[]]b', 'a]b', '0', repr('a]b')),
            ('a[\]]b', 'a]b', '0', repr('a]b')),
            ('a[^bc]d', 'aed', '0', repr('aed')),
            ('a[^bc]d', 'abd', '', repr(None)),
            ('a[^-b]c', 'adc', '0', repr('adc')),
            ('a[^-b]c', 'a-c', '', repr(None)),
            ('a[^]b]c', 'a]c', '', repr(None)),
            ('a[^]b]c', 'adc', '0', repr('adc')),
            ('\\ba\\b', 'a-', '0', repr('a')),
            ('\\ba\\b', '-a', '0', repr('a')),
            ('\\ba\\b', '-a-', '0', repr('a')),
            ('\\by\\b', 'xy', '', repr(None)),
            ('\\by\\b', 'yz', '', repr(None)),
            ('\\by\\b', 'xyz', '', repr(None)),
            ('x\\b', 'xyz', '', repr(None)),
            ('x\\B', 'xyz', '0', repr('x')),
            ('\\Bz', 'xyz', '0', repr('z')),
            ('z\\B', 'xyz', '', repr(None)),
            ('\\Bx', 'xyz', '', repr(None)),
            ('\\Ba\\B', 'a-', '', repr(None)),
            ('\\Ba\\B', '-a', '', repr(None)),
            ('\\Ba\\B', '-a-', '', repr(None)),
            ('\\By\\B', 'xy', '', repr(None)),
            ('\\By\\B', 'yz', '', repr(None)),
            ('\\By\\b', 'xy', '0', repr('y')),
            ('\\by\\B', 'yz', '0', repr('y')),
            ('\\By\\B', 'xyz', '0', repr('y')),
            ('ab|cd', 'abc', '0', repr('ab')),
            ('ab|cd', 'abcd', '0', repr('ab')),
            ('()ef', 'def', '0,1', repr(('ef', ''))),
            ('$b', 'b', '', repr(None)),
            ('a\\(b', 'a(b', '', repr(('a(b',))),
            ('a\\(*b', 'ab', '0', repr('ab')),
            ('a\\(*b', 'a((b', '0', repr('a((b')),
            ('a\\\\b', 'a\\b', '0', repr('a\\b')),
            ('((a))', 'abc', '0,1,2', repr(('a', 'a', 'a'))),
            ('(a)b(c)', 'abc', '0,1,2', repr(('abc', 'a', 'c'))),
            ('a+b+c', 'aabbabc', '0', repr('abc')),
            ('(a+|b)*', 'ab', '0,1', repr(('ab', 'b'))),
            ('(a+|b)+', 'ab', '0,1', repr(('ab', 'b'))),
            ('(a+|b)?', 'ab', '0,1', repr(('a', 'a'))),
            (')(', '-', '', self.TRAILING_CHARS),
            ('[^ab]*', 'cde', '0', repr('cde')),
            ('abc', '', '', repr(None)),
            ('a*', '', '0', repr('')),
            ('a|b|c|d|e', 'e', '0', repr('e')),
            ('(a|b|c|d|e)f', 'ef', '0,1', repr(('ef', 'e'))),
            ('abcd*efg', 'abcdefg', '0', repr('abcdefg')),
            ('ab*', 'xabyabbbz', '0', repr('ab')),
            ('ab*', 'xayabbbz', '0', repr('a')),
            ('(ab|cd)e', 'abcde', '0,1', repr(('cde', 'cd'))),
            ('[abhgefdc]ij', 'hij', '0', repr('hij')),
            ('^(ab|cd)e', 'abcde', '', repr(None)),
            ('(abc|)ef', 'abcdef', '0,1', repr(('ef', ''))),
            ('(a|b)c*d', 'abcd', '0,1', repr(('bcd', 'b'))),
            ('(ab|ab*)bc', 'abc', '0,1', repr(('abc', 'a'))),
            ('a([bc]*)c*', 'abc', '0,1', repr(('abc', 'bc'))),
            ('a([bc]*)(c*d)', 'abcd', '0,1,2', repr(('abcd', 'bc', 'd'))),
            ('a([bc]+)(c*d)', 'abcd', '0,1,2', repr(('abcd', 'bc', 'd'))),
            ('a([bc]*)(c+d)', 'abcd', '0,1,2', repr(('abcd', 'b', 'cd'))),
            ('a[bcd]*dcdcde', 'adcdcde', '0', repr('adcdcde')),
            ('a[bcd]+dcdcde', 'adcdcde', '', repr(None)),
            ('(ab|a)b*c', 'abc', '0,1', repr(('abc', 'ab'))),
            ('((a)(b)c)(d)', 'abcd', '1,2,3,4', repr(('abc', 'a', 'b', 'd'))),
            ('[a-zA-Z_][a-zA-Z0-9_]*', 'alpha', '0', repr('alpha')),
            ('^a(bc+|b[eh])g|.h$', 'abh', '0,1', repr(('bh', None))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effgz', '0,1,2',
              repr(('effgz', 'effgz', None))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'ij', '0,1,2', repr(('ij', 'ij', 'j'))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effg', '', repr(None)),
            ('(bc+d$|ef*g.|h?i(j|k))', 'bcdd', '', repr(None)),
            ('(bc+d$|ef*g.|h?i(j|k))', 'reffgz', '0,1,2',
              repr(('effgz', 'effgz', None))),
            ('(((((((((a)))))))))', 'a', '0', repr('a')),
            ('multiple words of text', 'uh-uh', '', repr(None)),
            ('multiple words', 'multiple words, yeah', '0',
              repr('multiple words')),
            ('(.*)c(.*)', 'abcde', '0,1,2', repr(('abcde', 'ab', 'de'))),
            ('\\((.*), (.*)\\)', '(a, b)', '2,1', repr(('b', 'a'))),
            ('[k]', 'ab', '', repr(None)),
            ('a[-]?c', 'ac', '0', repr('ac')),
            ('(abc)\\1', 'abcabc', '1', repr('abc')),
            ('([a-c]*)\\1', 'abcabc', '1', repr('abc')),
            ('^(.+)?B', 'AB', '1', repr('A')),
            ('(a+).\\1$', 'aaaaa', '0,1', repr(('aaaaa', 'aa'))),
            ('^(a+).\\1$', 'aaaa', '', repr(None)),
            ('(abc)\\1', 'abcabc', '0,1', repr(('abcabc', 'abc'))),
            ('([a-c]+)\\1', 'abcabc', '0,1', repr(('abcabc', 'abc'))),
            ('(a)\\1', 'aa', '0,1', repr(('aa', 'a'))),
            ('(a+)\\1', 'aa', '0,1', repr(('aa', 'a'))),
            ('(a+)+\\1', 'aa', '0,1', repr(('aa', 'a'))),
            ('(a).+\\1', 'aba', '0,1', repr(('aba', 'a'))),
            ('(a)ba*\\1', 'aba', '0,1', repr(('aba', 'a'))),
            ('(aa|a)a\\1$', 'aaa', '0,1', repr(('aaa', 'a'))),
            ('(a|aa)a\\1$', 'aaa', '0,1', repr(('aaa', 'a'))),
            ('(a+)a\\1$', 'aaa', '0,1', repr(('aaa', 'a'))),
            ('([abc]*)\\1', 'abcabc', '0,1', repr(('abcabc', 'abc'))),
            ('(a)(b)c|ab', 'ab', '0,1,2', repr(('ab', None, None))),
            ('(a)+x', 'aaax', '0,1', repr(('aaax', 'a'))),
            ('([ac])+x', 'aacx', '0,1', repr(('aacx', 'c'))),
            ('([^/]*/)*sub1/', 'd:msgs/tdir/sub1/trial/away.cpp', '0,1',
              repr(('d:msgs/tdir/sub1/', 'tdir/'))),
            ('([^.]*)\\.([^:]*):[T ]+(.*)', 'track1.title:TBlah blah blah',
              '0,1,2,3', repr(('track1.title:TBlah blah blah', 'track1',
              'title', 'Blah blah blah'))),
            ('([^N]*N)+', 'abNNxyzN', '0,1', repr(('abNNxyzN', 'xyzN'))),
            ('([^N]*N)+', 'abNNxyz', '0,1', repr(('abNN', 'N'))),
            ('([abc]*)x', 'abcx', '0,1', repr(('abcx', 'abc'))),
            ('([abc]*)x', 'abc', '', repr(None)),
            ('([xyz]*)x', 'abcx', '0,1', repr(('x', ''))),
            ('(a)+b|aac', 'aac', '0,1', repr(('aac', None))),

            # Test symbolic groups.

            ('(?P<i d>aaa)a', 'aaaa', '', self.MISSING_GT),
            ('(?P<id>aaa)a', 'aaaa', '0,id', repr(('aaaa', 'aaa'))),
            ('(?P<id>aa)(?P=id)', 'aaaa', '0,id', repr(('aaaa', 'aa'))),
            ('(?P<id>aa)(?P=xd)', 'aaaa', '', self.UNKNOWN_GROUP),

            # Character properties.
            (ur"\g", u"g", '0', repr(u'g')),
            (ur"\g<1>", u"g", '', self.UNKNOWN_GROUP),
            (ur"(.)\g<1>", u"gg", '0', repr(u'gg')),
            (ur"(.)\g<1>", u"gg", '', repr((u'gg', u'g'))),
            (ur"\N", u"N", '0', repr(u'N')),
            (ur"\N{LATIN SMALL LETTER A}", u"a", '0', repr(u'a')),
            (ur"\p", u"p", '0', repr(u'p')),
            (ur"\p{Ll}", u"a", '0', repr(u'a')),
            (ur"\P", u"P", '0', repr(u'P')),
            (ur"\P{Lu}", u"p", '0', repr(u'p')),

            # All tests from Perl.

            ('abc', 'abc', '0', repr('abc')),
            ('abc', 'xbc', '', repr(None)),
            ('abc', 'axc', '', repr(None)),
            ('abc', 'abx', '', repr(None)),
            ('abc', 'xabcy', '0', repr('abc')),
            ('abc', 'ababc', '0', repr('abc')),
            ('ab*c', 'abc', '0', repr('abc')),
            ('ab*bc', 'abc', '0', repr('abc')),
            ('ab*bc', 'abbc', '0', repr('abbc')),
            ('ab*bc', 'abbbbc', '0', repr('abbbbc')),
            ('ab{0,}bc', 'abbbbc', '0', repr('abbbbc')),
            ('ab+bc', 'abbc', '0', repr('abbc')),
            ('ab+bc', 'abc', '', repr(None)),
            ('ab+bc', 'abq', '', repr(None)),
            ('ab{1,}bc', 'abq', '', repr(None)),
            ('ab+bc', 'abbbbc', '0', repr('abbbbc')),
            ('ab{1,}bc', 'abbbbc', '0', repr('abbbbc')),
            ('ab{1,3}bc', 'abbbbc', '0', repr('abbbbc')),
            ('ab{3,4}bc', 'abbbbc', '0', repr('abbbbc')),
            ('ab{4,5}bc', 'abbbbc', '', repr(None)),
            ('ab?bc', 'abbc', '0', repr('abbc')),
            ('ab?bc', 'abc', '0', repr('abc')),
            ('ab{0,1}bc', 'abc', '0', repr('abc')),
            ('ab?bc', 'abbbbc', '', repr(None)),
            ('ab?c', 'abc', '0', repr('abc')),
            ('ab{0,1}c', 'abc', '0', repr('abc')),
            ('^abc$', 'abc', '0', repr('abc')),
            ('^abc$', 'abcc', '', repr(None)),
            ('^abc', 'abcc', '0', repr('abc')),
            ('^abc$', 'aabc', '', repr(None)),
            ('abc$', 'aabc', '0', repr('abc')),
            ('^', 'abc', '0', repr('')),
            ('$', 'abc', '0', repr('')),
            ('a.c', 'abc', '0', repr('abc')),
            ('a.c', 'axc', '0', repr('axc')),
            ('a.*c', 'axyzc', '0', repr('axyzc')),
            ('a.*c', 'axyzd', '', repr(None)),
            ('a[bc]d', 'abc', '', repr(None)),
            ('a[bc]d', 'abd', '0', repr('abd')),
            ('a[b-d]e', 'abd', '', repr(None)),
            ('a[b-d]e', 'ace', '0', repr('ace')),
            ('a[b-d]', 'aac', '0', repr('ac')),
            ('a[-b]', 'a-', '0', repr('a-')),
            ('a[b-]', 'a-', '0', repr('a-')),
            ('a[b-a]', '-', '', self.BAD_CHAR_RANGE),
            ('a[]b', '-', '', self.BAD_SET),
            ('a[', '-', '', self.BAD_SET),
            ('a]', 'a]', '0', repr('a]')),
            ('a[]]b', 'a]b', '0', repr('a]b')),
            ('a[^bc]d', 'aed', '0', repr('aed')),
            ('a[^bc]d', 'abd', '', repr(None)),
            ('a[^-b]c', 'adc', '0', repr('adc')),
            ('a[^-b]c', 'a-c', '', repr(None)),
            ('a[^]b]c', 'a]c', '', repr(None)),
            ('a[^]b]c', 'adc', '0', repr('adc')),
            ('ab|cd', 'abc', '0', repr('ab')),
            ('ab|cd', 'abcd', '0', repr('ab')),
            ('()ef', 'def', '0,1', repr(('ef', ''))),
            ('*a', '-', '', self.NOTHING_TO_REPEAT),
            ('(*)b', '-', '', self.NOTHING_TO_REPEAT),
            ('$b', 'b', '', repr(None)),
            ('a\\', '-', '', self.BAD_ESCAPE),
            ('a\\(b', 'a(b', '', repr(('a(b',))),
            ('a\\(*b', 'ab', '0', repr('ab')),
            ('a\\(*b', 'a((b', '0', repr('a((b')),
            ('a\\\\b', 'a\\b', '0', repr('a\\b')),
            ('abc)', '-', '', self.TRAILING_CHARS),
            ('(abc', '-', '', self.MISSING_RPAREN),
            ('((a))', 'abc', '0,1,2', repr(('a', 'a', 'a'))),
            ('(a)b(c)', 'abc', '0,1,2', repr(('abc', 'a', 'c'))),
            ('a+b+c', 'aabbabc', '0', repr('abc')),
            ('a{1,}b{1,}c', 'aabbabc', '0', repr('abc')),
            ('a**', '-', '', self.NOTHING_TO_REPEAT),
            ('a.+?c', 'abcabc', '0', repr('abc')),
            ('(a+|b)*', 'ab', '0,1', repr(('ab', 'b'))),
            ('(a+|b){0,}', 'ab', '0,1', repr(('ab', 'b'))),
            ('(a+|b)+', 'ab', '0,1', repr(('ab', 'b'))),
            ('(a+|b){1,}', 'ab', '0,1', repr(('ab', 'b'))),
            ('(a+|b)?', 'ab', '0,1', repr(('a', 'a'))),
            ('(a+|b){0,1}', 'ab', '0,1', repr(('a', 'a'))),
            (')(', '-', '', self.TRAILING_CHARS),
            ('[^ab]*', 'cde', '0', repr('cde')),
            ('abc', '', '', repr(None)),
            ('a*', '', '0', repr('')),
            ('([abc])*d', 'abbbcd', '0,1', repr(('abbbcd', 'c'))),
            ('([abc])*bcd', 'abcd', '0,1', repr(('abcd', 'a'))),
            ('a|b|c|d|e', 'e', '0', repr('e')),
            ('(a|b|c|d|e)f', 'ef', '0,1', repr(('ef', 'e'))),
            ('abcd*efg', 'abcdefg', '0', repr('abcdefg')),
            ('ab*', 'xabyabbbz', '0', repr('ab')),
            ('ab*', 'xayabbbz', '0', repr('a')),
            ('(ab|cd)e', 'abcde', '0,1', repr(('cde', 'cd'))),
            ('[abhgefdc]ij', 'hij', '0', repr('hij')),
            ('^(ab|cd)e', 'abcde', '', repr(None)),
            ('(abc|)ef', 'abcdef', '0,1', repr(('ef', ''))),
            ('(a|b)c*d', 'abcd', '0,1', repr(('bcd', 'b'))),
            ('(ab|ab*)bc', 'abc', '0,1', repr(('abc', 'a'))),
            ('a([bc]*)c*', 'abc', '0,1', repr(('abc', 'bc'))),
            ('a([bc]*)(c*d)', 'abcd', '0,1,2', repr(('abcd', 'bc', 'd'))),
            ('a([bc]+)(c*d)', 'abcd', '0,1,2', repr(('abcd', 'bc', 'd'))),
            ('a([bc]*)(c+d)', 'abcd', '0,1,2', repr(('abcd', 'b', 'cd'))),
            ('a[bcd]*dcdcde', 'adcdcde', '0', repr('adcdcde')),
            ('a[bcd]+dcdcde', 'adcdcde', '', repr(None)),
            ('(ab|a)b*c', 'abc', '0,1', repr(('abc', 'ab'))),
            ('((a)(b)c)(d)', 'abcd', '1,2,3,4', repr(('abc', 'a', 'b', 'd'))),
            ('[a-zA-Z_][a-zA-Z0-9_]*', 'alpha', '0', repr('alpha')),
            ('^a(bc+|b[eh])g|.h$', 'abh', '0,1', repr(('bh', None))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effgz', '0,1,2',
              repr(('effgz', 'effgz', None))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'ij', '0,1,2', repr(('ij', 'ij', 'j'))),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effg', '', repr(None)),
            ('(bc+d$|ef*g.|h?i(j|k))', 'bcdd', '', repr(None)),
            ('(bc+d$|ef*g.|h?i(j|k))', 'reffgz', '0,1,2',
              repr(('effgz', 'effgz', None))),
            ('((((((((((a))))))))))', 'a', '10', repr('a')),
            ('((((((((((a))))))))))\\10', 'aa', '0', repr('aa')),
            # Python does not have the same rules for \\41 so this is a syntax error
            #    ('((((((((((a))))))))))\\41', 'aa', '', repr(None)),
            #    ('((((((((((a))))))))))\\41', 'a!', '0', repr('a!')),
            ('((((((((((a))))))))))\\41', '', '', self.UNKNOWN_GROUP),
            ('(?i)((((((((((a))))))))))\\41', '', '', self.UNKNOWN_GROUP),
            ('(((((((((a)))))))))', 'a', '0', repr('a')),
            ('multiple words of text', 'uh-uh', '', repr(None)),
            ('multiple words', 'multiple words, yeah', '0',
              repr('multiple words')),
            ('(.*)c(.*)', 'abcde', '0,1,2', repr(('abcde', 'ab', 'de'))),
            ('\\((.*), (.*)\\)', '(a, b)', '2,1', repr(('b', 'a'))),
            ('[k]', 'ab', '', repr(None)),
            ('a[-]?c', 'ac', '0', repr('ac')),
            ('(abc)\\1', 'abcabc', '1', repr('abc')),
            ('([a-c]*)\\1', 'abcabc', '1', repr('abc')),
            ('(?i)abc', 'ABC', '0', repr('ABC')),
            ('(?i)abc', 'XBC', '', repr(None)),
            ('(?i)abc', 'AXC', '', repr(None)),
            ('(?i)abc', 'ABX', '', repr(None)),
            ('(?i)abc', 'XABCY', '0', repr('ABC')),
            ('(?i)abc', 'ABABC', '0', repr('ABC')),
            ('(?i)ab*c', 'ABC', '0', repr('ABC')),
            ('(?i)ab*bc', 'ABC', '0', repr('ABC')),
            ('(?i)ab*bc', 'ABBC', '0', repr('ABBC')),
            ('(?i)ab*?bc', 'ABBBBC', '0', repr('ABBBBC')),
            ('(?i)ab{0,}?bc', 'ABBBBC', '0', repr('ABBBBC')),
            ('(?i)ab+?bc', 'ABBC', '0', repr('ABBC')),
            ('(?i)ab+bc', 'ABC', '', repr(None)),
            ('(?i)ab+bc', 'ABQ', '', repr(None)),
            ('(?i)ab{1,}bc', 'ABQ', '', repr(None)),
            ('(?i)ab+bc', 'ABBBBC', '0', repr('ABBBBC')),
            ('(?i)ab{1,}?bc', 'ABBBBC', '0', repr('ABBBBC')),
            ('(?i)ab{1,3}?bc', 'ABBBBC', '0', repr('ABBBBC')),
            ('(?i)ab{3,4}?bc', 'ABBBBC', '0', repr('ABBBBC')),
            ('(?i)ab{4,5}?bc', 'ABBBBC', '', repr(None)),
            ('(?i)ab??bc', 'ABBC', '0', repr('ABBC')),
            ('(?i)ab??bc', 'ABC', '0', repr('ABC')),
            ('(?i)ab{0,1}?bc', 'ABC', '0', repr('ABC')),
            ('(?i)ab??bc', 'ABBBBC', '', repr(None)),
            ('(?i)ab??c', 'ABC', '0', repr('ABC')),
            ('(?i)ab{0,1}?c', 'ABC', '0', repr('ABC')),
            ('(?i)^abc$', 'ABC', '0', repr('ABC')),
            ('(?i)^abc$', 'ABCC', '', repr(None)),
            ('(?i)^abc', 'ABCC', '0', repr('ABC')),
            ('(?i)^abc$', 'AABC', '', repr(None)),
            ('(?i)abc$', 'AABC', '0', repr('ABC')),
            ('(?i)^', 'ABC', '0', repr('')),
            ('(?i)$', 'ABC', '0', repr('')),
            ('(?i)a.c', 'ABC', '0', repr('ABC')),
            ('(?i)a.c', 'AXC', '0', repr('AXC')),
            ('(?i)a.*?c', 'AXYZC', '0', repr('AXYZC')),
            ('(?i)a.*c', 'AXYZD', '', repr(None)),
            ('(?i)a[bc]d', 'ABC', '', repr(None)),
            ('(?i)a[bc]d', 'ABD', '0', repr('ABD')),
            ('(?i)a[b-d]e', 'ABD', '', repr(None)),
            ('(?i)a[b-d]e', 'ACE', '0', repr('ACE')),
            ('(?i)a[b-d]', 'AAC', '0', repr('AC')),
            ('(?i)a[-b]', 'A-', '0', repr('A-')),
            ('(?i)a[b-]', 'A-', '0', repr('A-')),
            ('(?i)a[b-a]', '-', '', self.BAD_CHAR_RANGE),
            ('(?i)a[]b', '-', '', self.BAD_SET),
            ('(?i)a[', '-', '', self.BAD_SET),
            ('(?i)a]', 'A]', '0', repr('A]')),
            ('(?i)a[]]b', 'A]B', '0', repr('A]B')),
            ('(?i)a[^bc]d', 'AED', '0', repr('AED')),
            ('(?i)a[^bc]d', 'ABD', '', repr(None)),
            ('(?i)a[^-b]c', 'ADC', '0', repr('ADC')),
            ('(?i)a[^-b]c', 'A-C', '', repr(None)),
            ('(?i)a[^]b]c', 'A]C', '', repr(None)),
            ('(?i)a[^]b]c', 'ADC', '0', repr('ADC')),
            ('(?i)ab|cd', 'ABC', '0', repr('AB')),
            ('(?i)ab|cd', 'ABCD', '0', repr('AB')),
            ('(?i)()ef', 'DEF', '0,1', repr(('EF', ''))),
            ('(?i)*a', '-', '', self.NOTHING_TO_REPEAT),
            ('(?i)(*)b', '-', '', self.NOTHING_TO_REPEAT),
            ('(?i)$b', 'B', '', repr(None)),
            ('(?i)a\\', '-', '', self.BAD_ESCAPE),
            ('(?i)a\\(b', 'A(B', '', repr(('A(B',))),
            ('(?i)a\\(*b', 'AB', '0', repr('AB')),
            ('(?i)a\\(*b', 'A((B', '0', repr('A((B')),
            ('(?i)a\\\\b', 'A\\B', '0', repr('A\\B')),
            ('(?i)abc)', '-', '', self.TRAILING_CHARS),
            ('(?i)(abc', '-', '', self.MISSING_RPAREN),
            ('(?i)((a))', 'ABC', '0,1,2', repr(('A', 'A', 'A'))),
            ('(?i)(a)b(c)', 'ABC', '0,1,2', repr(('ABC', 'A', 'C'))),
            ('(?i)a+b+c', 'AABBABC', '0', repr('ABC')),
            ('(?i)a{1,}b{1,}c', 'AABBABC', '0', repr('ABC')),
            ('(?i)a**', '-', '', self.NOTHING_TO_REPEAT),
            ('(?i)a.+?c', 'ABCABC', '0', repr('ABC')),
            ('(?i)a.*?c', 'ABCABC', '0', repr('ABC')),
            ('(?i)a.{0,5}?c', 'ABCABC', '0', repr('ABC')),
            ('(?i)(a+|b)*', 'AB', '0,1', repr(('AB', 'B'))),
            ('(?i)(a+|b){0,}', 'AB', '0,1', repr(('AB', 'B'))),
            ('(?i)(a+|b)+', 'AB', '0,1', repr(('AB', 'B'))),
            ('(?i)(a+|b){1,}', 'AB', '0,1', repr(('AB', 'B'))),
            ('(?i)(a+|b)?', 'AB', '0,1', repr(('A', 'A'))),
            ('(?i)(a+|b){0,1}', 'AB', '0,1', repr(('A', 'A'))),
            ('(?i)(a+|b){0,1}?', 'AB', '0,1', repr(('', None))),
            ('(?i))(', '-', '', self.TRAILING_CHARS),
            ('(?i)[^ab]*', 'CDE', '0', repr('CDE')),
            ('(?i)abc', '', '', repr(None)),
            ('(?i)a*', '', '0', repr('')),
            ('(?i)([abc])*d', 'ABBBCD', '0,1', repr(('ABBBCD', 'C'))),
            ('(?i)([abc])*bcd', 'ABCD', '0,1', repr(('ABCD', 'A'))),
            ('(?i)a|b|c|d|e', 'E', '0', repr('E')),
            ('(?i)(a|b|c|d|e)f', 'EF', '0,1', repr(('EF', 'E'))),
            ('(?i)abcd*efg', 'ABCDEFG', '0', repr('ABCDEFG')),
            ('(?i)ab*', 'XABYABBBZ', '0', repr('AB')),
            ('(?i)ab*', 'XAYABBBZ', '0', repr('A')),
            ('(?i)(ab|cd)e', 'ABCDE', '0,1', repr(('CDE', 'CD'))),
            ('(?i)[abhgefdc]ij', 'HIJ', '0', repr('HIJ')),
            ('(?i)^(ab|cd)e', 'ABCDE', '', repr(None)),
            ('(?i)(abc|)ef', 'ABCDEF', '0,1', repr(('EF', ''))),
            ('(?i)(a|b)c*d', 'ABCD', '0,1', repr(('BCD', 'B'))),
            ('(?i)(ab|ab*)bc', 'ABC', '0,1', repr(('ABC', 'A'))),
            ('(?i)a([bc]*)c*', 'ABC', '0,1', repr(('ABC', 'BC'))),
            ('(?i)a([bc]*)(c*d)', 'ABCD', '0,1,2', repr(('ABCD', 'BC', 'D'))),
            ('(?i)a([bc]+)(c*d)', 'ABCD', '0,1,2', repr(('ABCD', 'BC', 'D'))),
            ('(?i)a([bc]*)(c+d)', 'ABCD', '0,1,2', repr(('ABCD', 'B', 'CD'))),
            ('(?i)a[bcd]*dcdcde', 'ADCDCDE', '0', repr('ADCDCDE')),
            ('(?i)a[bcd]+dcdcde', 'ADCDCDE', '', repr(None)),
            ('(?i)(ab|a)b*c', 'ABC', '0,1', repr(('ABC', 'AB'))),
            ('(?i)((a)(b)c)(d)', 'ABCD', '1,2,3,4', repr(('ABC', 'A', 'B',
              'D'))),
            ('(?i)[a-zA-Z_][a-zA-Z0-9_]*', 'ALPHA', '0', repr('ALPHA')),
            ('(?i)^a(bc+|b[eh])g|.h$', 'ABH', '0,1', repr(('BH', None))),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'EFFGZ', '0,1,2', repr(('EFFGZ',
              'EFFGZ', None))),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'IJ', '0,1,2', repr(('IJ', 'IJ',
              'J'))),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'EFFG', '', repr(None)),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'BCDD', '', repr(None)),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'REFFGZ', '0,1,2', repr(('EFFGZ',
              'EFFGZ', None))),
            ('(?i)((((((((((a))))))))))', 'A', '10', repr('A')),
            ('(?i)((((((((((a))))))))))\\10', 'AA', '0', repr('AA')),
            #('(?i)((((((((((a))))))))))\\41', 'AA', '', repr(None)),
            #('(?i)((((((((((a))))))))))\\41', 'A!', '0', repr('A!')),
            ('(?i)(((((((((a)))))))))', 'A', '0', repr('A')),
            ('(?i)(?:(?:(?:(?:(?:(?:(?:(?:(?:(a))))))))))', 'A', '1',
              repr('A')),
            ('(?i)(?:(?:(?:(?:(?:(?:(?:(?:(?:(a|b|c))))))))))', 'C', '1',
              repr('C')),
            ('(?i)multiple words of text', 'UH-UH', '', repr(None)),
            ('(?i)multiple words', 'MULTIPLE WORDS, YEAH', '0',
              repr('MULTIPLE WORDS')),
            ('(?i)(.*)c(.*)', 'ABCDE', '0,1,2', repr(('ABCDE', 'AB', 'DE'))),
            ('(?i)\\((.*), (.*)\\)', '(A, B)', '2,1', repr(('B', 'A'))),
            ('(?i)[k]', 'AB', '', repr(None)),
        #    ('(?i)abcd', 'ABCD', SUCCEED, 'found+"-"+\\found+"-"+\\\\found', repr(ABCD-$&-\\ABCD)),
        #    ('(?i)a(bc)d', 'ABCD', SUCCEED, 'g1+"-"+\\g1+"-"+\\\\g1', repr(BC-$1-\\BC)),
            ('(?i)a[-]?c', 'AC', '0', repr('AC')),
            ('(?i)(abc)\\1', 'ABCABC', '1', repr('ABC')),
            ('(?i)([a-c]*)\\1', 'ABCABC', '1', repr('ABC')),
            ('a(?!b).', 'abad', '0', repr('ad')),
            ('a(?=d).', 'abad', '0', repr('ad')),
            ('a(?=c|d).', 'abad', '0', repr('ad')),
            ('a(?:b|c|d)(.)', 'ace', '1', repr('e')),
            ('a(?:b|c|d)*(.)', 'ace', '1', repr('e')),
            ('a(?:b|c|d)+?(.)', 'ace', '1', repr('e')),
            ('a(?:b|(c|e){1,2}?|d)+?(.)', 'ace', '1,2', repr(('c', 'e'))),

            # Lookbehind: split by : but not if it is escaped by -.
            ('(?<!-):(.*?)(?<!-):', 'a:bc-:de:f', '1', repr('bc-:de')),
            # Escaping with \ as we know it.
            ('(?<!\\\):(.*?)(?<!\\\):', 'a:bc\\:de:f', '1', repr('bc\\:de')),
            # Terminating with ' and escaping with ? as in edifact.
            ("(?<!\\?)'(.*?)(?<!\\?)'", "a'bc?'de'f", '1', repr("bc?'de")),

            # Comments using the (?#...) syntax.

            ('w(?# comment', 'w', '', self.MISSING_RPAREN),
            ('w(?# comment 1)xy(?# comment 2)z', 'wxyz', '0', repr('wxyz')),

            # Check odd placement of embedded pattern modifiers.

            # Not an error under PCRE/PRE:
            # When the new behaviour is turned on positional inline flags affect
            # only what follows.
            ('w(?i)', 'W', '0', repr('W')),
            ('w(?in)', 'W', '0', repr(None)),
            ('w(?i)', 'w', '0', repr('w')),
            ('w(?in)', 'w', '0', repr('w')),
            ('(?i)w', 'W', '0', repr('W')),
            ('(?in)w', 'W', '0', repr('W')),

            # Comments using the x embedded pattern modifier.

            ("""(?x)w# comment 1
x y
# comment 2
z""", 'wxyz', '0', repr('wxyz')),

            # Using the m embedded pattern modifier.

            ('^abc', """jkl
abc
xyz""", '', repr(None)),
            ('(?m)^abc', """jkl
abc
xyz""", '0', repr('abc')),

            ('(?m)abc$', """jkl
xyzabc
123""", '0', repr('abc')),

            # Using the s embedded pattern modifier.

            ('a.b', 'a\nb', '', repr(None)),
            ('(?s)a.b', 'a\nb', '0', repr('a\nb')),

            # Test \w, etc. both inside and outside character classes.

            ('\\w+', '--ab_cd0123--', '0', repr('ab_cd0123')),
            ('[\\w]+', '--ab_cd0123--', '0', repr('ab_cd0123')),
            ('\\D+', '1234abc5678', '0', repr('abc')),
            ('[\\D]+', '1234abc5678', '0', repr('abc')),
            ('[\\da-fA-F]+', '123abc', '0', repr('123abc')),
            # Not an error under PCRE/PRE:
            # ('[\\d-x]', '-', '', self.SYNTAX_ERROR),
            (r'([\s]*)([\S]*)([\s]*)', ' testing!1972', '3,2,1',
              repr(('', 'testing!1972', ' '))),
            (r'(\s*)(\S*)(\s*)', ' testing!1972', '3,2,1',
              repr(('', 'testing!1972', ' '))),

            #
            # Post-1.5.2 additions.

            # xmllib problem.
            (r'(([a-z]+):)?([a-z]+)$', 'smil', '1,2,3', repr((None, None,
              'smil'))),
            # Bug 110866: reference to undefined group.
            (r'((.)\1+)', '', '', self.OPEN_GROUP),
            # Bug 111869: search (PRE/PCRE fails on this one, SRE doesn't).
            (r'.*d', 'abc\nabd', '0', repr('abd')),
            # Bug 112468: various expected syntax errors.
            (r'(', '', '', self.MISSING_RPAREN),
            (r'[\41]', '!', '0', repr('!')),
            # Bug 114033: nothing to repeat.
            (r'(x?)?', 'x', '0', repr('x')),
            # Bug 115040: rescan if flags are modified inside pattern.
            # If the new behaviour is turned on then positional inline flags
            # affect only what follows.
            (r' (?x)foo ', 'foo', '0', repr('foo')),
            (r' (?nx)foo ', 'foo', '0', repr(None)),
            (r'(?x) foo ', 'foo', '0', repr('foo')),
            (r'(?nx) foo ', 'foo', '0', repr('foo')),
            (r'(?x)foo ', 'foo', '0', repr('foo')),
            (r'(?nx)foo ', 'foo', '0', repr('foo')),
            # Bug 115618: negative lookahead.
            (r'(?<!abc)(d.f)', 'abcdefdof', '0', repr('dof')),
            # Bug 116251: character class bug.
            (r'[\w-]+', 'laser_beam', '0', repr('laser_beam')),
            # Bug 123769+127259: non-greedy backtracking bug.
            (r'.*?\S *:', 'xx:', '0', repr('xx:')),
            (r'a[ ]*?\ (\d+).*', 'a   10', '0', repr('a   10')),
            (r'a[ ]*?\ (\d+).*', 'a    10', '0', repr('a    10')),
            # Bug 127259: \Z shouldn't depend on multiline mode.
            (r'(?ms).*?x\s*\Z(.*)','xx\nx\n', '1', repr('')),
            # Bug 128899: uppercase literals under the ignorecase flag.
            (r'(?i)M+', 'MMM', '0', repr('MMM')),
            (r'(?i)m+', 'MMM', '0', repr('MMM')),
            (r'(?i)[M]+', 'MMM', '0', repr('MMM')),
            (r'(?i)[m]+', 'MMM', '0', repr('MMM')),
            # Bug 130748: ^* should be an error (nothing to repeat).
            (r'^*', '', '', self.NOTHING_TO_REPEAT),
            # Bug 133283: minimizing repeat problem.
            (r'"(?:\\"|[^"])*?"', r'"\""', '0', repr(r'"\""')),
            # Bug 477728: minimizing repeat problem.
            (r'^.*?$', 'one\ntwo\nthree\n', '', repr(None)),
            # Bug 483789: minimizing repeat problem.
            (r'a[^>]*?b', 'a>b', '', repr(None)),
            # Bug 490573: minimizing repeat problem.
            (r'^a*?$', 'foo', '', repr(None)),
            # Bug 470582: nested groups problem.
            (r'^((a)c)?(ab)$', 'ab', '1,2,3', repr((None, None, 'ab'))),
            # Another minimizing repeat problem (capturing groups in assertions).
            ('^([ab]*?)(?=(b)?)c', 'abc', '1,2', repr(('ab', None))),
            ('^([ab]*?)(?!(b))c', 'abc', '1,2', repr(('ab', None))),
            ('^([ab]*?)(?<!(a))c', 'abc', '1,2', repr(('ab', None))),
            # Bug 410271: \b broken under locales.
            (r'\b.\b', 'a', '0', repr('a')),
            (ur'(?u)\b.\b', u'\N{LATIN CAPITAL LETTER A WITH DIAERESIS}',
              '0', repr(u'\xc4')),
            (ur'(?u)\w', u'\N{LATIN CAPITAL LETTER A WITH DIAERESIS}',
              '0', repr(u'\xc4')),
        ]

        for t in tests:
            self.index += 1
            try:
                pattern, string, groups, expected = t
            except ValueError:
                fields = ", ".join([repr(f) for f in t[ : 3]] + ["..."])
                self.record_failure("Incorrect number of test fields: (%s)" %
                  fields)
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
                            actual = repr(m.group(*group_list))
                        else:
                            actual = repr(m[:])
                    else:
                        actual = repr(m)
                except Exception, e:
                    actual = repr(e)

                if actual != expected:
                    fields = ", ".join([repr(f) for f in t[ : 3]] + ["..."])
                    self.record_failure("Expected %s but got %s instead: (%s)" %
                      (expected, actual, fields))

    def test_replacement(self):
        self.expect(lambda: regex.sub("test\?", "result\?\.\a\q\m\n", "test?"),
          repr("result\?\.\a\q\m\n"))
        self.expect(lambda: regex.sub(r"test\?", "result\?\.\a\q\m\n", "test?"),
          repr("result\?\.\a\q\m\n"))

        self.expect(lambda: regex.sub('(.)', r"\1\1", 'x'), repr('xx'))
        self.expect(lambda: regex.sub('(.)', regex.escape(r"\1\1"), 'x'),
          repr(r"\1\1"))
        self.expect(lambda: regex.sub('(.)', r"\\1\\1", 'x'), repr(r"\1\1"))
        self.expect(lambda: regex.sub('(.)', lambda m: r"\1\1", 'x'),
          repr(r"\1\1"))

    def test_common_prefix(self):
        # Very long common prefix
        all = string.ascii_lowercase + string.digits + string.ascii_uppercase
        side = all * 4
        regexp = '(' + side + '|' + side + ')'
        self.expect(lambda: type(regex.compile(regexp)), self.PATTERN_CLASS)

    def test_captures(self):
        self.expect(lambda: regex.search(r"(\w)+", "abc").captures(1),
          repr(['a', 'b', 'c']))
        self.expect(lambda: regex.search(r"(\w{3})+", "abcdef").captures(0, 1),
          repr((['abcdef'], ['abc', 'def'])))
        self.expect(lambda: regex.search(r"^(\d{1,3})(?:\.(\d{1,3})){3}$",
          "192.168.0.1").captures(1, 2), repr((['192', ], ['168', '0', '1'])))
        self.expect(lambda: regex.match(r"^([0-9A-F]{2}){4} ([a-z]\d){5}$",
          "3FB52A0C a2c4g3k9d3").captures(1, 2), repr((['3F', 'B5', '2A', '0C'],
          ['a2', 'c4', 'g3', 'k9', 'd3'])))
        self.expect(lambda: regex.match("([a-z]W)([a-z]X)+([a-z]Y)",
          "aWbXcXdXeXfY").captures(1, 2, 3), repr((['aW'], ['bX', 'cX', 'dX',
          'eX'], ['fY'])))

        self.expect(lambda: regex.search(r".*?(?=(.)+)b", "ab").captures(1),
          repr(['b']))
        self.expect(lambda: regex.search(r".*?(?>(.){0,2})d",
          "abcd").captures(1), repr(['b', 'c']))
        self.expect(lambda: regex.search(r"(.)+", "a").captures(1),
          repr(['a']))

    def test_guards(self):
        m = regex.search(r"(X.*?Y\s*){3}(X\s*)+AB:",
          "XY\nX Y\nX  Y\nXY\nXX AB:")
        self.expect(lambda: m.span(0, 1, 2), repr(((3, 21), (12, 15), (16,
          18))))

        m = regex.search(r"(X.*?Y\s*){3,}(X\s*)+AB:",
          "XY\nX Y\nX  Y\nXY\nXX AB:")
        self.expect(lambda: m.span(0, 1, 2), repr(((0, 21), (12, 15), (16,
          18))))

        m = regex.search(r'\d{4}(\s*\w)?\W*((?!\d)\w){2}', "9999XX")
        self.expect(lambda: m.span(0, 1, 2), repr(((0, 6), (-1, -1), (5, 6))))

        m = regex.search(r'A\s*?.*?(\n+.*?\s*?){0,2}\(X', 'A\n1\nS\n1 (X')
        self.expect(lambda: m.span(0, 1), repr(((0, 10), (5, 8))))

        m = regex.search('Derde\s*:', 'aaaaaa:\nDerde:')
        self.expect(lambda: m.span(), repr((8, 14)))
        m = regex.search('Derde\s*:', 'aaaaa:\nDerde:')
        self.expect(lambda: m.span(), repr((7, 13)))

    def run(self):
        print "Performing tests"
        print "================"

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
                print self.performing
                sys.stdout.flush()
                obj()

        print
        print "Performed %s" % performed_count
        print

        if self.failed_tests:
            print "Failures"
            print "========"
            current = ""
            failed_count = 0
            for name, index, message in self.failed_tests:
                if name != current:
                    if current:
                        print
                    current = name
                    failed_count += 1
                    print current
                print "\t%s) %s" % (index, message)
            print
            print "Failed %s" % failed_count
        else:
            print "Passed"

if __name__ == "__main__":
    Test().run()
