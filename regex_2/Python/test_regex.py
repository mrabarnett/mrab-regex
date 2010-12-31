import regex
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
            self.record_failure("Expected %s but got %s instead" % (expected_result,
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
        self.expect(lambda: regex.search('a*', 'xxx').span(0), "(0, 0)")
        self.expect(lambda: regex.search('x*', 'axx').span(), "(0, 0)")
        self.expect(lambda: regex.search('x+', 'axx').span(0), "(1, 3)")
        self.expect(lambda: regex.search('x+', 'axx').span(), "(1, 3)")
        self.expect(lambda: regex.search('x', 'aaa'), "None")
        self.expect(lambda: regex.match('a*', 'xxx').span(0), "(0, 0)")
        self.expect(lambda: regex.match('a*', 'xxx').span(), "(0, 0)")
        self.expect(lambda: regex.match('x*', 'xxxa').span(0), "(0, 3)")
        self.expect(lambda: regex.match('x*', 'xxxa').span(), "(0, 3)")
        self.expect(lambda: regex.match('a+', 'xxx'), "None")

    def bump_num(self, matchobj):
        int_value = int(matchobj[0])
        return str(int_value + 1)

    def test_basic_regex_sub(self):
        self.expect(lambda: regex.sub("(?i)b+", "x", "bbbb BBBB"), "'x x'")
        self.expect(lambda: regex.sub(r'\d+', self.bump_num, '08.2 -2 23x99y'),
          "'9.3 -3 24x100y'")
        self.expect(lambda: regex.sub(r'\d+', self.bump_num, '08.2 -2 23x99y',
          3), "'9.3 -3 23x99y'")

        self.expect(lambda: regex.sub('.', lambda m: r"\n", 'x'), repr("\\n"))
        self.expect(lambda: regex.sub('.', r"\n", 'x'), repr("\n"))

        s = r"\1\1"
        self.expect(lambda: regex.sub('(.)', s, 'x'), "'xx'")
        self.expect(lambda: regex.sub('(.)', regex.escape(s), 'x'), repr(s))
        self.expect(lambda: regex.sub('(.)', lambda m: s, 'x'), repr(s))

        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a>\g<a>', 'xx'),
          "'xxxx'")
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<a>\g<1>', 'xx'),
          "'xxxx'")
        self.expect(lambda: regex.sub('(?P<unk>x)', r'\g<unk>\g<unk>', 'xx'),
          "'xxxx'")
        self.expect(lambda: regex.sub('(?P<unk>x)', r'\g<1>\g<1>', 'xx'),
          "'xxxx'")

        self.expect(lambda: regex.sub('a',
          r'\t\n\v\r\f\a\b\B\Z\a\A\w\W\s\S\d\D', 'a'),
          repr("\t\n\v\r\f\a\b\\B\\Z\a\\A\\w\\W\\s\\S\\d\\D"))
        self.expect(lambda: regex.sub('a', '\t\n\v\r\f\a', 'a'),
          repr("\t\n\v\r\f\a"))
        self.expect(lambda: regex.sub('a', '\t\n\v\r\f\a', 'a'), repr(chr(9) +
          chr(10) + chr(11) + chr(13) + chr(12) + chr(7)))

        self.expect(lambda: regex.sub(r'^\s*', 'X', 'test'), "'Xtest'")

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

        # In Python2.3 (etc), these loop endlessly in sre_parser.py.
        self.expect(lambda: regex.sub('(((((((((((x)))))))))))', r'\11', 'x'),
          "'x'")
        self.expect(lambda: regex.sub('((((((((((y))))))))))(.)', r'\118',
          'xyz'), "'xz8'")
        self.expect(lambda: regex.sub('((((((((((y))))))))))(.)', r'\11a',
          'xyz'), "'xza'")

    def test_qualified_re_sub(self):
        self.expect(lambda: regex.sub('a', 'b', 'aaaaa'), "'bbbbb'")
        self.expect(lambda: regex.sub('a', 'b', 'aaaaa', 1), "'baaaa'")

    def test_bug_114660(self):
        self.expect(lambda: regex.sub(r'(\S)\s+(\S)', r'\1 \2', 'hello  there'),
          "'hello there'")

    def test_bug_462270(self):
        # Test for empty sub() behaviour, see SF bug #462270
        self.expect(lambda: regex.sub('x*', '-', 'abxd'), "'-a-b-d-'")
        self.expect(lambda: regex.sub('x+', '-', 'abxd'), "'ab-d'")

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
          "''")
        self.expect(lambda: regex.sub('(?P<a>x)|(?P<b>y)', r'\2', 'xx'), "''")

        # The old behaviour was to raise it as an IndexError.
        self.expect(lambda: regex.sub('(?P<a>x)', r'\g<-1>', 'xx'),
          self.BAD_GROUP_NAME)

    def test_re_subn(self):
        self.expect(lambda: regex.subn("(?i)b+", "x", "bbbb BBBB"),
          "('x x', 2)")
        self.expect(lambda: regex.subn("b+", "x", "bbbb BBBB"),
         "('x BBBB', 1)")
        self.expect(lambda: regex.subn("b+", "x", "xyz"), "('xyz', 0)")
        self.expect(lambda: regex.subn("b*", "x", "xyz"), "('xxxyxzx', 4)")
        self.expect(lambda: regex.subn("b*", "x", "xyz", 2), "('xxxyz', 2)")

    def test_re_split(self):
        self.expect(lambda: regex.split(":", ":a:b::c"),
          "['', 'a', 'b', '', 'c']")
        self.expect(lambda: regex.split(":*", ":a:b::c"),
          "['', 'a', 'b', 'c']")
        self.expect(lambda: regex.split("(:*)", ":a:b::c"),
          "['', ':', 'a', ':', 'b', '::', 'c']")
        self.expect(lambda: regex.split("(?::*)", ":a:b::c"),
          "['', 'a', 'b', 'c']")
        self.expect(lambda: regex.split("(:)*", ":a:b::c"),
          "['', ':', 'a', ':', 'b', ':', 'c']")
        self.expect(lambda: regex.split("([b:]+)", ":a:b::c"),
          "['', ':', 'a', ':b::', 'c']")
        self.expect(lambda: regex.split("(b)|(:+)", ":a:b::c"),
          "['', None, ':', 'a', None, ':', '', 'b', None, '', None, '::', 'c']")
        self.expect(lambda: regex.split("(?:b)|(?::+)", ":a:b::c"),
          "['', 'a', '', '', 'c']")

        self.expect(lambda: regex.split("x", "xaxbxc"), "['', 'a', 'b', 'c']")
        self.expect(lambda: [m for m in regex.splititer("x", "xaxbxc")],
          "['', 'a', 'b', 'c']")

        self.expect(lambda: regex.split("(?r)x", "xaxbxc"),
          "['c', 'b', 'a', '']")
        self.expect(lambda: [m for m in regex.splititer("(?r)x", "xaxbxc")],
          "['c', 'b', 'a', '']")

        self.expect(lambda: regex.split("(x)|(y)", "xaxbxc"),
          "['', 'x', None, 'a', 'x', None, 'b', 'x', None, 'c']")
        self.expect(lambda: [m for m in regex.splititer("(x)|(y)", "xaxbxc")],
          "['', 'x', None, 'a', 'x', None, 'b', 'x', None, 'c']")

        self.expect(lambda: regex.split("(?r)(x)|(y)", "xaxbxc"),
          "['c', 'x', None, 'b', 'x', None, 'a', 'x', None, '']")
        self.expect(lambda: [m for m in regex.splititer("(?r)(x)|(y)",
          "xaxbxc")], "['c', 'x', None, 'b', 'x', None, 'a', 'x', None, '']")

    def test_qualified_re_split(self):
        self.expect(lambda: regex.split(":", ":a:b::c", 2), "['', 'a', 'b::c']")
        self.expect(lambda: regex.split(':', 'a:b:c:d', 2), "['a', 'b', 'c:d']")
        self.expect(lambda: regex.split("(:)", ":a:b::c", 2),
          "['', ':', 'a', ':', 'b::c']")
        self.expect(lambda: regex.split("(:*)", ":a:b::c", 2),
          "['', ':', 'a', ':', 'b::c']")

    def test_re_findall(self):
        self.expect(lambda: regex.findall(":+", "abc"), "[]")
        self.expect(lambda: regex.findall(":+", "a:b::c:::d"),
          "[':', '::', ':::']")
        self.expect(lambda: regex.findall("(:+)", "a:b::c:::d"),
          "[':', '::', ':::']")
        self.expect(lambda: regex.findall("(:)(:*)", "a:b::c:::d"),
          "[(':', ''), (':', ':'), (':', '::')]")

    def test_bug_117612(self):
        self.expect(lambda: regex.findall(r"(a|(b))", "aba"),
          "[('a', ''), ('b', 'b'), ('a', '')]")

    def test_re_match(self):
        self.expect(lambda: regex.match('a', 'a')[:], "('a',)")
        self.expect(lambda: regex.match('(a)', 'a')[:], "('a', 'a')")
        self.expect(lambda: regex.match(r'(a)', 'a')[0], "'a'")
        self.expect(lambda: regex.match(r'(a)', 'a')[1], "'a'")
        self.expect(lambda: regex.match(r'(a)', 'a').group(1, 1), "('a', 'a')")

        pat = regex.compile('((a)|(b))(c)?')
        self.expect(lambda: pat.match('a')[:], "('a', 'a', 'a', None, None)")
        self.expect(lambda: pat.match('b')[:], "('b', 'b', None, 'b', None)")
        self.expect(lambda: pat.match('ac')[:], "('ac', 'a', 'a', None, 'c')")
        self.expect(lambda: pat.match('bc')[:], "('bc', 'b', None, 'b', 'c')")
        self.expect(lambda: pat.match('bc')[:], "('bc', 'b', None, 'b', 'c')")

        # A single group.
        m = regex.match('(a)', 'a')
        self.expect(lambda: m.group(), "'a'")
        self.expect(lambda: m.group(0), "'a'")
        self.expect(lambda: m.group(1), "'a'")
        self.expect(lambda: m.group(1, 1), "('a', 'a')")

        pat = regex.compile('(?:(?P<a1>a)|(?P<b2>b))(?P<c3>c)?')
        self.expect(lambda: pat.match('a').group(1, 2, 3), "('a', None, None)")
        self.expect(lambda: pat.match('b').group('a1', 'b2', 'c3'),
          "(None, 'b', None)")
        self.expect(lambda: pat.match('ac').group(1, 'b2', 3),
          "('a', None, 'c')")

    def test_re_groupref_exists(self):
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', '(a)')[:],
          "('(a)', '(', 'a')")
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', 'a')[:],
          "('a', None, 'a')")
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', 'a)'),
          "None")
        self.expect(lambda: regex.match(r'^(\()?([^()]+)(?(1)\))$', '(a'),
          "None")
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)b|d))$', 'ab')[:],
          "('ab', 'a', 'b')")
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)b|d))$', 'cd')[:],
          "('cd', None, 'd')")
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)|d))$', 'cd')[:],
          "('cd', None, 'd')")
        self.expect(lambda: regex.match('^(?:(a)|c)((?(1)|d))$', 'a')[:],
          "('a', 'a', '')")

        # Tests for bug #1177831: exercise groups other than the first group.
        p = regex.compile('(?P<g1>a)(?P<g2>b)?((?(g2)c|d))')
        self.expect(lambda: p.match('abc')[:], "('abc', 'a', 'b', 'c')")
        self.expect(lambda: p.match('ad')[:], "('ad', 'a', None, 'd')")
        self.expect(lambda: p.match('abd'), "None")
        self.expect(lambda: p.match('ac'), "None")

    def test_re_groupref(self):
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1$', '|a|')[:],
          "('|a|', '|', 'a')")
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1?$', 'a')[:],
          "('a', None, 'a')")
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1$', 'a|'), "None")
        self.expect(lambda: regex.match(r'^(\|)?([^()]+)\1$', '|a'), "None")
        self.expect(lambda: regex.match(r'^(?:(a)|c)(\1)$', 'aa')[:],
          "('aa', 'a', 'a')")
        self.expect(lambda: regex.match(r'^(?:(a)|c)(\1)?$', 'c')[:],
          "('c', None, None)")

    def test_groupdict(self):
        self.expect(lambda: regex.match('(?P<first>first) (?P<second>second)',
          'first second').groupdict(),
           repr({'first': 'first', 'second': 'second'}))

    def test_expand(self):
        self.expect(lambda: regex.match("(?P<first>first) (?P<second>second)",
          "first second").expand(r"\2 \1 \g<second> \g<first>"),
          "'second first second first'")

    def test_repeat_minmax(self):
        self.expect(lambda: regex.match(r"^(\w){1}$", "abc"), "None")
        self.expect(lambda: regex.match(r"^(\w){1}?$", "abc"), "None")
        self.expect(lambda: regex.match(r"^(\w){1,2}$", "abc"), "None")
        self.expect(lambda: regex.match(r"^(\w){1,2}?$", "abc"), "None")

        self.expect(lambda: regex.match(r"^(\w){3}$", "abc")[1], "'c'")
        self.expect(lambda: regex.match(r"^(\w){1,3}$", "abc")[1], "'c'")
        self.expect(lambda: regex.match(r"^(\w){1,4}$", "abc")[1], "'c'")
        self.expect(lambda: regex.match(r"^(\w){3,4}?$", "abc")[1], "'c'")
        self.expect(lambda: regex.match(r"^(\w){3}?$", "abc")[1], "'c'")
        self.expect(lambda: regex.match(r"^(\w){1,3}?$", "abc")[1], "'c'")
        self.expect(lambda: regex.match(r"^(\w){1,4}?$", "abc")[1], "'c'")
        self.expect(lambda: regex.match(r"^(\w){3,4}?$", "abc")[1], "'c'")

        self.expect(lambda: regex.match("^x{1}$", "xxx"), "None")
        self.expect(lambda: regex.match("^x{1}?$", "xxx"), "None")
        self.expect(lambda: regex.match("^x{1,2}$", "xxx"), "None")
        self.expect(lambda: regex.match("^x{1,2}?$", "xxx"), "None")

        self.expect(lambda: regex.match("^x{1}", "xxx")[0], "'x'")
        self.expect(lambda: regex.match("^x{1}?", "xxx")[0], "'x'")
        self.expect(lambda: regex.match("^x{0,1}", "xxx")[0], "'x'")
        self.expect(lambda: regex.match("^x{0,1}?", "xxx")[0], "''")

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

        self.expect(lambda: regex.match("^x{}$", "xxx"), "None")
        self.expect(lambda: type(regex.match("^x{}$", "x{}")), self.MATCH_CLASS)

    def test_getattr(self):
        self.expect(lambda: regex.compile("(?i)(a)(b)").pattern,
          "'(?i)(a)(b)'")
        self.expect(lambda: regex.compile("(?i)(a)(b)").flags, repr(regex.I))
        self.expect(lambda: regex.compile(u"(?i)(a)(b)").flags, repr(regex.I))
        self.expect(lambda: regex.compile("(?i)(a)(b)").groups, "2")
        self.expect(lambda: regex.compile("(?i)(a)(b)").groupindex, "{}")
        self.expect(lambda:
          regex.compile("(?i)(?P<first>a)(?P<other>b)").groupindex,
          repr({'first': 1, 'other': 2}))

        self.expect(lambda: regex.match("(a)", "a").pos, "0")
        self.expect(lambda: regex.match("(a)", "a").endpos, "1")
        self.expect(lambda: regex.match("(a)", "a").string, "'a'")
        self.expect(lambda: regex.match("(a)", "a").regs, "((0, 1), (0, 1))")
        self.expect(lambda: type(regex.match("(a)", "a").re),
          self.PATTERN_CLASS)

    def test_special_escapes(self):
        self.expect(lambda: regex.search(r"\b(b.)\b", "abcd abc bcd bx")[1],
          "'bx'")
        self.expect(lambda: regex.search(r"\B(b.)\B", "abc bcd bc abxd")[1],
          "'bx'")
        self.expect(lambda: regex.search(r"\b(b.)\b", "abcd abc bcd bx",
          regex.LOCALE)[1], "'bx'")
        self.expect(lambda: regex.search(r"\B(b.)\B", "abc bcd bc abxd",
          regex.LOCALE)[1], "'bx'")
        self.expect(lambda: regex.search(ur"\b(b.)\b", u"abcd abc bcd bx",
          regex.UNICODE)[1], "u'bx'")
        self.expect(lambda: regex.search(ur"\B(b.)\B", u"abc bcd bc abxd",
          regex.UNICODE)[1], "u'bx'")
        self.expect(lambda: regex.search(r"^abc$", "\nabc\n", regex.M)[0],
          "'abc'")
        self.expect(lambda: regex.search(r"^\Aabc\Z$", "abc", regex.M)[0],
          "'abc'")
        self.expect(lambda: regex.search(r"^\Aabc\Z$", "\nabc\n", regex.M),
          "None")
        self.expect(lambda: regex.search(ur"\b(b.)\b",
          u"abcd abc bcd bx")[1], "u'bx'")
        self.expect(lambda: regex.search(ur"\B(b.)\B",
          u"abc bcd bc abxd")[1], "u'bx'")
        self.expect(lambda: regex.search(ur"^abc$", u"\nabc\n",
          regex.M)[0], "u'abc'")
        self.expect(lambda: regex.search(ur"^\Aabc\Z$", u"abc",
          regex.M)[0], "u'abc'")
        self.expect(lambda: regex.search(ur"^\Aabc\Z$", u"\nabc\n",
          regex.M), "None")
        self.expect(lambda: regex.search(r"\d\D\w\W\s\S", "1aa! a")[0],
          "'1aa! a'")
        self.expect(lambda: regex.search(r"\d\D\w\W\s\S", "1aa! a",
          regex.LOCALE)[0], "'1aa! a'")
        self.expect(lambda: regex.search(ur"\d\D\w\W\s\S", u"1aa! a",
          regex.UNICODE)[0], "u'1aa! a'")

    def test_bigcharset(self):
        self.expect(lambda: regex.match(ur"(?u)([\u2222\u2223])",
          u"\u2222")[1], "u'\u2222'")
        self.expect(lambda: regex.match(ur"(?u)([\u2222\u2223])", u"\u2222",
          regex.UNICODE)[1], "u'\u2222'")
        self.expect(lambda: u"".join(regex.findall(u".",
          u"e\xe8\xe9\xea\xeb\u0113\u011b\u0117", flags=regex.UNICODE)),
          r"u'e\xe8\xe9\xea\xeb\u0113\u011b\u0117'")
        self.expect(lambda:
          u"".join(regex.findall(ur"[e\xe8\xe9\xea\xeb\u0113\u011b\u0117]",
          u"e\xe8\xe9\xea\xeb\u0113\u011b\u0117", flags=regex.UNICODE)),
          r"u'e\xe8\xe9\xea\xeb\u0113\u011b\u0117'")
        self.expect(lambda:
          u"".join(regex.findall(ur"e|\xe8|\xe9|\xea|\xeb|\u0113|\u011b|\u0117",
          u"e\xe8\xe9\xea\xeb\u0113\u011b\u0117", flags=regex.UNICODE)),
          r"u'e\xe8\xe9\xea\xeb\u0113\u011b\u0117'")

    def test_anyall(self):
        self.expect(lambda: regex.match("a.b", "a\nb", regex.DOTALL)[0],
          repr("a\nb"))
        self.expect(lambda: regex.match("a.*b", "a\n\nb", regex.DOTALL)[0],
          repr("a\n\nb"))

    def test_non_consuming(self):
        self.expect(lambda: regex.match(r"(a(?=\s[^a]))", "a b")[1], "'a'")
        self.expect(lambda: regex.match(r"(a(?=\s[^a]*))", "a b")[1], "'a'")
        self.expect(lambda: regex.match(r"(a(?=\s[abc]))", "a b")[1], "'a'")
        self.expect(lambda: regex.match(r"(a(?=\s[abc]*))", "a bc")[1], "'a'")
        self.expect(lambda: regex.match(r"(a)(?=\s\1)", "a a")[1], "'a'")
        self.expect(lambda: regex.match(r"(a)(?=\s\1*)", "a aa")[1], "'a'")
        self.expect(lambda: regex.match(r"(a)(?=\s(abc|a))", "a a")[1], "'a'")

        self.expect(lambda: regex.match(r"(a(?!\s[^a]))", "a a")[1], "'a'")
        self.expect(lambda: regex.match(r"(a(?!\s[abc]))", "a d")[1], "'a'")
        self.expect(lambda: regex.match(r"(a)(?!\s\1)", "a b")[1], "'a'")
        self.expect(lambda: regex.match(r"(a)(?!\s(abc|a))", "a b")[1], "'a'")

    def test_ignore_case(self):
        self.expect(lambda: regex.match("abc", "ABC", regex.I)[0], "'ABC'")
        self.expect(lambda: regex.match(u"abc", u"ABC", regex.I)[0],
          "u'ABC'")

        self.expect(lambda: regex.match(r"(a\s[^a]*)", "a bb", regex.I)[1],
          "'a bb'")
        self.expect(lambda: regex.match(r"(a\s[abc])", "a b", regex.I)[1],
          "'a b'")
        self.expect(lambda: regex.match(r"(a\s[abc]*)", "a bb", regex.I)[1],
          "'a bb'")
        self.expect(lambda: regex.match(r"((a)\s\2)", "a a", regex.I)[1],
          "'a a'")
        self.expect(lambda: regex.match(r"((a)\s\2*)", "a aa", regex.I)[1],
          "'a aa'")
        self.expect(lambda: regex.match(r"((a)\s(abc|a))", "a a", regex.I)[1],
          "'a a'")
        self.expect(lambda: regex.match(r"((a)\s(abc|a)*)", "a aa", regex.I)[1],
          "'a aa'")

        # Issue #3511.
        self.expect(lambda: regex.match(r"[Z-a]", "_").span(), "(0, 1)")
        self.expect(lambda: regex.match(r"(?i)[Z-a]", "_").span(), "(0, 1)")

    def test_category(self):
        self.expect(lambda: regex.match(r"(\s)", " ")[1], "' '")

    def test_not_literal(self):
        self.expect(lambda: regex.search(r"\s([^a])", " b")[1], "'b'")
        self.expect(lambda: regex.search(r"\s([^a]*)", " bb")[1], "'bb'")

    def test_search_coverage(self):
        self.expect(lambda: regex.search(r"\s(b)", " b")[1], "'b'")
        self.expect(lambda: regex.search(r"a\s", "a ")[0], "'a '")

    def test_re_escape(self):
        p = ""
        self.expect(lambda: regex.escape(p), repr(p))
        for i in range(0, 256):
            p += chr(i)
            self.expect(lambda: type(regex.match(regex.escape(chr(i)), chr(i))),
              self.MATCH_CLASS)
            self.expect(lambda: regex.match(regex.escape(chr(i)),
              chr(i)).span(), "(0, 1)")

        pat = regex.compile(regex.escape(p))
        self.expect(lambda: pat.match(p).span(), "(0, 256)")

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
        self.expect(lambda: regex.match(r'(a)|(b)', 'b').start(1), "-1")
        self.expect(lambda: regex.match(r'(a)|(b)', 'b').end(1), "-1")
        self.expect(lambda: regex.match(r'(a)|(b)', 'b').span(1), "(-1, -1)")

    def test_bug_527371(self):
        # Bug described in patches 527371/672491.
        self.expect(lambda: regex.match(r'(a)?a','a').lastindex, "None")
        self.expect(lambda: regex.match(r'(a)(b)?b','ab').lastindex, "1")
        self.expect(lambda: regex.match(r'(?P<a>a)(?P<b>b)?b','ab').lastgroup,
          "'a'")
        self.expect(lambda: regex.match("(?P<a>a(b))", "ab").lastgroup, "'a'")
        self.expect(lambda: regex.match("((a))", "a").lastindex, "1")

    def test_bug_545855(self):
        # Bug 545855 -- This pattern failed to cause a compile error as it
        # should, instead provoking a TypeError.
        self.expect(lambda: regex.compile('foo[a-'), self.BAD_SET)

    def test_bug_418626(self):
        # Bugs 418626 at al. -- Testing Greg Chapman's addition of op code
        # SRE_OP_MIN_REPEAT_ONE for eliminating recursion on simple uses of
        # pattern '*?' on a long string.
        self.expect(lambda: regex.match('.*?c', 10000 * 'ab' + 'cd').end(0),
          "20001")
        self.expect(lambda: regex.match('.*?cd', 5000 * 'ab' + 'c' + 5000 * 'ab'
          + 'cde').end(0), "20003")
        self.expect(lambda: regex.match('.*?cd', 20000 * 'abc' + 'de').end(0),
          "60001")
        # Non-simple '*?' still used to hit the recursion limit, before the
        # non-recursive scheme was implemented.
        self.expect(lambda: regex.search('(a|b)*?c', 10000 * 'ab' +
          'cd').end(0), "20001")

    def test_bug_612074(self):
        pat = u"[" + regex.escape(u"\u2039") + u"]"
        self.expect(lambda: regex.compile(pat) and 1, "1")

    def test_stack_overflow(self):
        # Nasty cases that used to overflow the straightforward recursive
        # implementation of repeated groups.
        self.expect(lambda: regex.match('(x)*', 50000 * 'x')[1], "'x'")
        self.expect(lambda: regex.match('(x)*y', 50000 * 'x' + 'y')[1], "'x'")
        self.expect(lambda: regex.match('(x)*?y', 50000 * 'x' + 'y')[1], "'x'")

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
          "(['sum', 'op=', 3, 'op*', 'foo', 'op+', 312.5, 'op+', 'bar'], '')")

    def test_bug_448951(self):
        # Bug 448951 (similar to 429357, but with single char match).
        # (Also test greedy matches.)
        for op in '', '?', '*':
            self.expect(lambda: regex.match(r'((.%s):)?z' % op, 'z')[:],
              "('z', None, None)")
            self.expect(lambda: regex.match(r'((.%s):)?z' % op, 'a:z')[:],
              "('a:z', 'a:', 'a')")

    def test_bug_725106(self):
        # Capturing groups in alternatives in repeats.
        self.expect(lambda: regex.match('^((a)|b)*', 'abc')[:],
          "('ab', 'b', 'a')")
        self.expect(lambda: regex.match('^(([ab])|c)*', 'abc')[:],
          "('abc', 'c', 'b')")
        self.expect(lambda: regex.match('^((d)|[ab])*', 'abc')[:],
          "('ab', 'b', None)")
        self.expect(lambda: regex.match('^((a)c|[ab])*', 'abc')[:],
          "('ab', 'b', None)")
        self.expect(lambda: regex.match('^((a)|b)*?c', 'abc')[:],
          "('abc', 'b', 'a')")
        self.expect(lambda: regex.match('^(([ab])|c)*?d', 'abcd')[:],
          "('abcd', 'c', 'b')")
        self.expect(lambda: regex.match('^((d)|[ab])*?c', 'abc')[:],
          "('abc', 'b', None)")
        self.expect(lambda: regex.match('^((a)c|[ab])*?c', 'abc')[:],
          "('abc', 'b', None)")

    def test_bug_725149(self):
        # Mark_stack_base restoring before restoring marks.
        self.expect(lambda: regex.match('(a)(?:(?=(b)*)c)*', 'abb')[:],
          "('a', 'a', None)")
        self.expect(lambda: regex.match('(a)((?!(b)*))*', 'abb')[:],
          "('a', 'a', None, None)")

    def test_bug_764548(self):
        # Bug 764548, regex.compile() barfs on str/unicode subclasses.
        class my_unicode(unicode): pass
        pat = regex.compile(my_unicode("abc"))
        self.expect(lambda: pat.match("xyz"), "None")

    def test_finditer(self):
        it = regex.finditer(r":+", "a:b::c:::d")
        self.expect(lambda: [item[0] for item in it], "[':', '::', ':::']")

    def test_bug_926075(self):
        self.index += 1
        if regex.compile('bug_926075') is regex.compile(u'bug_926075'):
            self.record_failure()

    def test_bug_931848(self):
        pattern = u"[\u002E\u3002\uFF0E\uFF61]"
        self.expect(lambda: regex.compile(pattern).split("a.b.c"),
          "['a', 'b', 'c']")

    def test_bug_581080(self):
        it = regex.finditer(r"\s", "a b")
        self.expect(lambda: it.next().span(), "(1, 2)")
        self.expect(lambda: it.next(), "StopIteration()")

        scanner = regex.compile(r"\s").scanner("a b")
        self.expect(lambda: scanner.search().span(), "(1, 2)")
        self.expect(lambda: scanner.search(), "None")

    def test_bug_817234(self):
        it = regex.finditer(r".*", "asdf")
        self.expect(lambda: it.next().span(), "(0, 4)")
        self.expect(lambda: it.next().span(), "(4, 4)")
        self.expect(lambda: it.next(), "StopIteration()")

    def test_empty_array(self):
        # SF buf 1647541.
        import array
        for typecode in 'cbBuhHiIlLfd':
            a = array.array(typecode)
            self.expect(lambda: regex.compile("bla").match(a), "None")
            self.expect(lambda: regex.compile("").match(a)[1 : ], "()")

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
        self.expect(lambda: pat.match(u'\xe0'), "None")
        pat = regex.compile(u'(?a)\xc0', regex.IGNORECASE)
        self.expect(lambda: pat.match(u'\xe0'), "None")
        pat = regex.compile(u'\w', regex.ASCII)
        self.expect(lambda: pat.match(u'\xe0'), "None")
        pat = regex.compile(u'(?a)\w')
        self.expect(lambda: pat.match(u'\xe0'), "None")
        # String patterns.
        for flags in (0, regex.ASCII):
            pat = regex.compile('\xc0', flags | regex.IGNORECASE)
            self.expect(lambda: pat.match('\xe0'), "None")
            pat = regex.compile('\w')
            self.expect(lambda: pat.match('\xe0'), "None")
        self.expect(lambda: regex.compile('(?au)\w'), self.MIXED_FLAGS)

    def test_subscripting_match(self):
        self.index += 1
        m = regex.match(r'(?<a>\w)', 'xy')
        if m[0] != m.group(0) or m[1] != m.group(1):
            record_failure("Failed")
        self.index += 1
        if m[:] != ('x', 'x'):
            self.record_failure("Failed: expected \"('x', 'x')\" but got %s instead" % repr(m[:]))

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
          "[u'\\u0939\\u093f\\u0928\\u094d\\u0926\\u0940']")
        self.expect(lambda: regex.findall(ur"(?u)\W+",
          u" \u0939\u093f\u0928\u094d\u0926\u0940,"),
          "[u' ', u',']")
        self.expect(lambda: regex.split(ur"(?uz)\b",
          u" \u0939\u093f\u0928\u094d\u0926\u0940,"),
          "[u' ', u'\\u0939\\u093f\\u0928\\u094d\\u0926\\u0940', u',']")
        self.expect(lambda: regex.split(ur"(?uz)\B",
          u" \u0939\u093f\u0928\u094d\u0926\u0940,"),
          "[u'', u' \\u0939', u'\\u093f', u'\\u0928', u'\\u094d', u'\\u0926', u'\\u0940,', u'']")

    def test_search_anchor(self):
        self.expect(lambda: regex.findall(r"\G\w{2}", "abcd ef"),
          "['ab', 'cd']")

    def test_search_reverse(self):
        self.expect(lambda: regex.findall(r"(?r).", "abc"), "['c', 'b', 'a']")
        self.expect(lambda: regex.findall(r"(?r).", "abc", overlapped=True),
          "['c', 'b', 'a']")
        self.expect(lambda: regex.findall(r"(?r)..", "abcde"), "['de', 'bc']")
        self.expect(lambda: regex.findall(r"(?r)..", "abcde", overlapped=True),
          "['de', 'cd', 'bc', 'ab']")

        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).", "abc")],
          "['c', 'b', 'a']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde",
          overlapped=True)], "['de', 'cd', 'bc', 'ab']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).",
          "abc")],"['c', 'b', 'a']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde",
          overlapped=True)],"['de', 'cd', 'bc', 'ab']")

        self.expect(lambda: regex.findall(r"^|\w+", "foo bar"),
          "['', 'foo', 'bar']")
        self.expect(lambda: regex.findall(r"(?z)^|\w+", "foo bar"),
          "['', 'foo', 'bar']")
        self.expect(lambda: regex.findall(r"(?r)^|\w+", "foo bar"),
          "['bar', 'foo', '']")
        self.expect(lambda: regex.findall(r"(?rz)^|\w+", "foo bar"),
          "['bar', 'foo', '']")

        self.expect(lambda: [m[0] for m in regex.finditer(r"^|\w+", "foo bar")],
          "['', 'foo', 'bar']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?z)^|\w+",
          "foo bar")], "['', 'foo', 'bar']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)^|\w+",
          "foo bar")], "['bar', 'foo', '']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?rz)^|\w+",
          "foo bar")], "['bar', 'foo', '']")

        self.expect(lambda: regex.findall(r"\G\w{2}", "abcd ef"),
          "['ab', 'cd']")
        self.expect(lambda: regex.findall(r".{2}(?<=\G.*)", "abcd"),
          "['ab', 'cd']")
        self.expect(lambda: regex.findall(r"(?r)\G\w{2}", "abcd ef"),
          "[]")
        self.expect(lambda: regex.findall(r"(?r)\w{2}\G", "abcd ef"),
          "['ef']")

        self.expect(lambda: regex.findall(r"q*", "qqwe"),
          "['qq', '', '', '']")
        self.expect(lambda: regex.findall(r"(?z)q*", "qqwe"),
          "['qq', '', '', '']")
        self.expect(lambda: regex.findall(r"(?r)q*", "qqwe"),
          "['', '', 'qq', '']")
        self.expect(lambda: regex.findall(r"(?rz)q*", "qqwe"),
          "['', '', 'qq', '']")

        self.expect(lambda: regex.findall(".", "abcd", pos=1, endpos=3),
          "['b', 'c']")
        self.expect(lambda: regex.findall(".", "abcd", pos=1, endpos=-1),
          "['b', 'c']")
        self.expect(lambda: [m[0] for m in regex.finditer(".", "abcd", pos=1,
          endpos=3)], "['b', 'c']")
        self.expect(lambda: [m[0] for m in regex.finditer(".", "abcd", pos=1,
          endpos=-1)], "['b', 'c']")

        self.expect(lambda: [m[0] for m in regex.finditer("(?r).", "abcd",
          pos=1, endpos=3)], "['c', 'b']")
        self.expect(lambda: [m[0] for m in regex.finditer("(?r).", "abcd",
          pos=1, endpos=-1)], "['c', 'b']")
        self.expect(lambda: regex.findall("(?r).", "abcd", pos=1, endpos=3),
          "['c', 'b']")
        self.expect(lambda: regex.findall("(?r).", "abcd", pos=1, endpos=-1),
          "['c', 'b']")

        self.expect(lambda: regex.findall(r"[ab]", "aB", regex.I), "['a', 'B']")
        self.expect(lambda: regex.findall(r"(?r)[ab]", "aB", regex.I),
          "['B', 'a']")

        self.expect(lambda: regex.findall(r"(?r).{2}", "abc"), "['bc']")
        self.expect(lambda: regex.findall(r"(?r).{2}", "abc", overlapped=True),
          "['bc', 'ab']")
        self.expect(lambda: regex.findall(r"(\w+) (\w+)",
          "first second third fourth fifth"),
          "[('first', 'second'), ('third', 'fourth')]")
        self.expect(lambda: regex.findall(r"(?r)(\w+) (\w+)",
          "first second third fourth fifth"),
          "[('fourth', 'fifth'), ('second', 'third')]")

        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).{2}", "abc")],
          "['bc']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r).{2}", "abc",
          overlapped=True)], "['bc', 'ab']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(\w+) (\w+)",
          "first second third fourth fifth")],
          "['first second', 'third fourth']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)(\w+) (\w+)",
          "first second third fourth fifth")],
          "['fourth fifth', 'second third']")

        self.expect(lambda: regex.search("abcdef", "abcdef").span(), "(0, 6)")
        self.expect(lambda: regex.search("(?r)abcdef", "abcdef").span(),
          "(0, 6)")
        self.expect(lambda: regex.search("(?i)abcdef", "ABCDEF").span(),
          "(0, 6)")
        self.expect(lambda: regex.search("(?ir)abcdef", "ABCDEF").span(),
          "(0, 6)")

        self.expect(lambda: regex.sub(r"(.)", r"\1", "abc"), "'abc'")
        self.expect(lambda: regex.sub(r"(?r)(.)", r"\1", "abc"), "'abc'")

    def test_atomic(self):
        # Issue 433030.
        self.expect(lambda: regex.search(r"(?>a*)a", "aa"), "None")

    def test_possessive(self):
        # Single-character non-possessive.
        self.expect(lambda: regex.search(r"a?a", "a").span(), "(0, 1)")
        self.expect(lambda: regex.search(r"a*a", "aaa").span(), "(0, 3)")
        self.expect(lambda: regex.search(r"a+a", "aaa").span(), "(0, 3)")
        self.expect(lambda: regex.search(r"a{1,3}a", "aaa").span(), "(0, 3)")

        # Multiple-character non-possessive.
        self.expect(lambda: regex.search(r"(?:ab)?ab", "ab").span(), "(0, 2)")
        self.expect(lambda: regex.search(r"(?:ab)*ab", "ababab").span(),
          "(0, 6)")
        self.expect(lambda: regex.search(r"(?:ab)+ab", "ababab").span(),
          "(0, 6)")
        self.expect(lambda: regex.search(r"(?:ab){1,3}ab", "ababab").span(),
          "(0, 6)")

        # Single-character possessive.
        self.expect(lambda: regex.search(r"a?+a", "a"), "None")
        self.expect(lambda: regex.search(r"a*+a", "aaa"), "None")
        self.expect(lambda: regex.search(r"a++a", "aaa"), "None")
        self.expect(lambda: regex.search(r"a{1,3}+a", "aaa"), "None")

        # Multiple-character possessive.
        self.expect(lambda: regex.search(r"(?:ab)?+ab", "ab"), "None")
        self.expect(lambda: regex.search(r"(?:ab)*+ab", "ababab"), "None")
        self.expect(lambda: regex.search(r"(?:ab)++ab", "ababab"), "None")
        self.expect(lambda: regex.search(r"(?:ab){1,3}+ab", "ababab"), "None")

    def test_zerowidth(self):
        # Issue 3262.
        self.expect(lambda: regex.split(r"\b", "a b"), "['a b']")
        self.expect(lambda: regex.split(r"(?z)\b", "a b"),
          "['', 'a', ' ', 'b', '']")

        # Issue 1647489.
        self.expect(lambda: regex.findall(r"^|\w+", "foo bar"),
          "['', 'foo', 'bar']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"^|\w+", "foo bar")],
          "['', 'foo', 'bar']")
        self.expect(lambda: regex.findall(r"(?r)^|\w+", "foo bar"),
          "['bar', 'foo', '']")
        self.expect(lambda: [m[0] for m in
          regex.finditer(r"(?r)^|\w+", "foo bar")], "['bar', 'foo', '']")
        self.expect(lambda: regex.findall(r"(?z)^|\w+", "foo bar"),
          "['', 'foo', 'bar']")
        self.expect(lambda: [m[0] for m in
          regex.finditer(r"(?z)^|\w+", "foo bar")], "['', 'foo', 'bar']")
        self.expect(lambda: regex.findall(r"(?rz)^|\w+", "foo bar"),
          "['bar', 'foo', '']")
        self.expect(lambda: [m[0] for m in
          regex.finditer(r"(?rz)^|\w+", "foo bar")], "['bar', 'foo', '']")

        self.expect(lambda: regex.split("", "xaxbxc"), "['xaxbxc']")
        self.expect(lambda: [m for m in regex.splititer("", "xaxbxc")],
          "['xaxbxc']")

        self.expect(lambda: regex.split("(?r)", "xaxbxc"), "['xaxbxc']")
        self.expect(lambda: [m for m in regex.splititer("(?r)", "xaxbxc")],
          "['xaxbxc']")

        self.expect(lambda: regex.split("(?z)", "xaxbxc"),
          "['', 'x', 'a', 'x', 'b', 'x', 'c', '']")
        self.expect(lambda: [m for m in regex.splititer("(?z)", "xaxbxc")],
          "['', 'x', 'a', 'x', 'b', 'x', 'c', '']")

        self.expect(lambda: regex.split("(?rz)", "xaxbxc"),
          "['', 'c', 'x', 'b', 'x', 'a', 'x', '']")
        self.expect(lambda: [m for m in regex.splititer("(?rz)", "xaxbxc")],
          "['', 'c', 'x', 'b', 'x', 'a', 'x', '']")

    def test_scoped_and_inline_flags(self):
        # Issues 433028, #433024, #433027.
        self.expect(lambda: regex.search(r"(?i)Ab", "ab").span(), "(0, 2)")
        self.expect(lambda: regex.search(r"(?i:A)b", "ab").span(), "(0, 2)")
        self.expect(lambda: regex.search(r"A(?i)b", "ab"), "None")

        self.expect(lambda: regex.search(r"(?-i)Ab", "ab", flags=regex.I),
          "None")
        self.expect(lambda: regex.search(r"(?-i:A)b", "ab", flags=regex.I),
          "None")
        self.expect(lambda: regex.search(r"A(?-i)b", "ab",
          flags=regex.I).span(), "(0, 2)")

    def test_repeated_repeats(self):
        # Issue 2537.
        self.expect(lambda: regex.search(r"(?:a+)+", "aaa").span(), "(0, 3)")
        self.expect(lambda: regex.search(r"(?:(?:ab)+c)+", "abcabc").span(),
          "(0, 6)")

    def test_lookbehind(self):
        self.expect(lambda: regex.search(r"123(?<=a\d+)", "a123").span(),
          "(1, 4)")
        self.expect(lambda: regex.search(r"123(?<=a\d+)", "b123"), "None")
        self.expect(lambda: regex.search(r"123(?<!a\d+)", "a123"), "None")
        self.expect(lambda: regex.search(r"123(?<!a\d+)", "b123").span(),
          "(1, 4)")

        self.expect(lambda: type(regex.match("(a)b(?<=b)(c)", "abc")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match("(a)b(?<=c)(c)", "abc"), "None")
        self.expect(lambda: type(regex.match("(a)b(?=c)(c)", "abc")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match("(a)b(?=b)(c)", "abc"), "None")

        self.expect(lambda: regex.match("(?:(a)|(x))b(?<=(?(2)x|c))c", "abc"),
          "None")
        self.expect(lambda: regex.match("(?:(a)|(x))b(?<=(?(2)b|x))c", "abc"),
          "None")
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?<=(?(2)x|b))c",
          "abc")), self.MATCH_CLASS)
        self.expect(lambda: regex.match("(?:(a)|(x))b(?<=(?(1)c|x))c", "abc"),
          "None")
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?<=(?(1)b|x))c",
          "abc")), self.MATCH_CLASS)

        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?=(?(2)x|c))c",
          "abc")), self.MATCH_CLASS)
        self.expect(lambda: regex.match("(?:(a)|(x))b(?=(?(2)c|x))c", "abc"),
          "None")
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?=(?(2)x|c))c",
          "abc")), self.MATCH_CLASS)
        self.expect(lambda: regex.match("(?:(a)|(x))b(?=(?(1)b|x))c", "abc"),
          "None")
        self.expect(lambda: type(regex.match("(?:(a)|(x))b(?=(?(1)c|x))c",
          "abc")), self.MATCH_CLASS)

        self.expect(lambda: regex.match("(a)b(?<=(?(2)x|c))(c)", "abc"), "None")
        self.expect(lambda: regex.match("(a)b(?<=(?(2)b|x))(c)", "abc"), "None")
        self.expect(lambda: regex.match("(a)b(?<=(?(1)c|x))(c)", "abc"), "None")
        self.expect(lambda: type(regex.match("(a)b(?<=(?(1)b|x))(c)", "abc")),
          self.MATCH_CLASS)

        self.expect(lambda: type(regex.match("(a)b(?=(?(2)x|c))(c)", "abc")),
          self.MATCH_CLASS)
        self.expect(lambda: regex.match("(a)b(?=(?(2)b|x))(c)", "abc"), "None")
        self.expect(lambda: type(regex.match("(a)b(?=(?(1)c|x))(c)", "abc")),
          self.MATCH_CLASS)

        self.expect(lambda: type(regex.compile(r"(a)\2(b)")), self.PATTERN_CLASS)

    def test_unmatched_in_sub(self):
        # Issue 1519638.
        self.expect(lambda: regex.sub(r"(x)?(y)?", r"\2-\1", "xy"), "'y-x'")
        self.expect(lambda: regex.sub(r"(x)?(y)?", r"\2-\1", "x"), "'-x'")
        self.expect(lambda: regex.sub(r"(x)?(y)?", r"\2-\1", "y"), "'y-'")

    def test_overlapped(self):
        self.expect(lambda: regex.findall(r"..", "abcde"), "['ab', 'cd']")
        self.expect(lambda: regex.findall(r"..", "abcde", overlapped=True),
          "['ab', 'bc', 'cd', 'de']")
        self.expect(lambda: regex.findall(r"(?r)..", "abcde"), "['de', 'bc']")
        self.expect(lambda: regex.findall(r"(?r)..", "abcde", overlapped=True),
          "['de', 'cd', 'bc', 'ab']")

        self.expect(lambda: [m[0] for m in regex.finditer(r"..", "abcde")],
          "['ab', 'cd']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"..", "abcde",
          overlapped=True)], "['ab', 'bc', 'cd', 'de']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde")],
          "['de', 'bc']")
        self.expect(lambda: [m[0] for m in regex.finditer(r"(?r)..", "abcde",
          overlapped=True)], "['de', 'cd', 'bc', 'ab']")

    def test_splititer(self):
        self.expect(lambda: regex.split(r",", "a,b,,c,"),
          "['a', 'b', '', 'c', '']")
        self.expect(lambda: [m for m in regex.splititer(r",", "a,b,,c,")],
          "['a', 'b', '', 'c', '']")

    def test_grapheme(self):
        self.expect(lambda: regex.match(ur"(?u)\X", u"\xE0").span(), "(0, 1)")
        self.expect(lambda: regex.match(ur"(?u)\X", u"a\u0300").span(),
          "(0, 2)")

        self.expect(lambda: regex.findall(ur"(?u)\X",
          u"a\xE0a\u0300e\xE9e\u0301"),
          "[u'a', u'\\xe0', u'a\\u0300', u'e', u'\\xe9', u'e\\u0301']")
        self.expect(lambda: regex.findall(ur"(?u)\X{3}",
          u"a\xE0a\u0300e\xE9e\u0301"),
          "[u'a\\xe0a\\u0300', u'e\\xe9e\\u0301']")

    def test_word_boundary(self):
        text = u'The quick ("brown") fox can\'t jump 32.3 feet, right?'
        self.expect(lambda: regex.split(ur'(?z)\b', text), repr([u'', u'The',
          u' ', u'quick', u' ("', u'brown', u'") ', u'fox', u' ', u'can', u"'",
          u't', u' ', u'jump', u' ', u'32', u'.', u'3', u' ', u'feet', u', ',
          u'right', u'?']))
        self.expect(lambda: regex.split(ur'(?wz)\b', text), repr([u'', u'The',
          u' ', u'quick', u' ', u'(', u'"', u'brown', u'"', u')', u' ', u'fox',
          u' ', u"can't", u' ', u'jump', u' ', u'32.3', u' ', u'feet', u',',
          u' ', u'right', u'?', u'']))

        text = u"The  fox"
        self.expect(lambda: regex.split(ur'(?z)\b', text), repr([u'', u'The',
          u'  ', u'fox', u'']))
        self.expect(lambda: regex.split(ur'(?wz)\b', text), repr([u'', u'The',
          u' ', u' ', u'fox', u'']))

        text = u"can't aujourd'hui l'objectif"
        self.expect(lambda: regex.split(ur'(?z)\b', text), repr([u'', u'can',
          u"'", u't', u' ', u'aujourd', u"'", u'hui', u' ', u'l', u"'",
          u'objectif', u'']))
        self.expect(lambda: regex.split(ur'(?wz)\b', text), repr([u'',
          u"can't", u' ', u"aujourd'hui", u' ', u"l'", u'objectif', u'']))

    def test_branch_reset(self):
        self.expect(lambda: regex.match(r"(?:(a)|(b))(c)", "ac").groups(),
          "('a', None, 'c')")
        self.expect(lambda: regex.match(r"(?:(a)|(b))(c)", "bc").groups(),
          "(None, 'b', 'c')")
        self.expect(lambda:
          regex.match(r"(?:(?<a>a)|(?<b>b))(?<c>c)", "ac").groups(),
          "('a', None, 'c')")
        self.expect(lambda:
          regex.match(r"(?:(?<a>a)|(?<b>b))(?<c>c)", "bc").groups(),
            "(None, 'b', 'c')")
        self.expect(lambda:
          regex.match(r"(?<a>a)(?:(?<b>b)|(?<c>c))(?<d>d)", "abd").groups(),
          "('a', 'b', None, 'd')")
        self.expect(lambda:
          regex.match(r"(?<a>a)(?:(?<b>b)|(?<c>c))(?<d>d)", "acd").groups(),
          "('a', None, 'c', 'd')")
        self.expect(lambda: regex.match(r"(a)(?:(b)|(c))(d)", "abd").groups(),
         "('a', 'b', None, 'd')")
        self.expect(lambda: regex.match(r"(a)(?:(b)|(c))(d)", "acd").groups(),
          "('a', None, 'c', 'd')")
        self.expect(lambda: regex.match(r"(a)(?|(b)|(b))(d)", "abd").groups(),
          "('a', 'b', 'd')")
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)|(?<b>b))(c)", "ac").groups(),
          "('a', None, 'c')")
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)|(?<b>b))(c)", "bc").groups(),
          "(None, 'b', 'c')")
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)|(?<a>b))(c)", "ac").groups(), "('a', 'c')")
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)|(?<a>b))(c)", "bc").groups(), "('b', 'c')")
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(?<a>d))(e)", "abe").groups(),
          "('a', 'b', 'e')")
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(?<a>d))(e)", "cde").groups(),
          "('d', 'c', 'e')")
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(d))(e)", "abe").groups(),
          "('a', 'b', 'e')")
        self.expect(lambda: regex.match(r"(?|(?<a>a)(?<b>b)|(?<b>c)(d))(e)",
          "cde").groups(), "('d', 'c', 'e')")
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(c)(d))(e)", "abe").groups(),
          "('a', 'b', 'e')")
        self.expect(lambda:
          regex.match(r"(?|(?<a>a)(?<b>b)|(c)(d))(e)", "cde").groups(),
          "('c', 'd', 'e')")
        self.expect(lambda: regex.match(r"(?|(?<a>a)(?<b>b)|(c)(?<a>d))(e)",
          "abe"), self.DUPLICATE_GROUP)
        self.expect(lambda: regex.match(r"(?|(?<a>a)(?<b>b)|(c)(?<a>d))(e)",
          "cde"), self.DUPLICATE_GROUP)

    def test_set(self):
        self.expect(lambda: regex.match(r"[a]", "a").span(), "(0, 1)")
        self.expect(lambda: regex.match(r"(?i)[a]", "A").span(), "(0, 1)")
        self.expect(lambda: regex.match(r"[a-b]", r"a").span(), "(0, 1)")
        self.expect(lambda: regex.match(r"(?i)[a-b]", r"A").span(), "(0, 1)")

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

            ('(?P<foo_123>a)', 'a', '1', "'a'"),
            ('(?P<foo_123>a)(?P=foo_123)', 'aa', '1', "'a'"),

            # Mal-formed \g in pattern treated as literal for compatibility.
            (r'(?<foo_123>a)\g<foo_123', 'aa', '', 'None'),
            (r'(?<foo_123>a)\g<1>', 'aa', '1', "'a'"),
            (r'(?<foo_123>a)\g<!>', 'aa', '', 'None'),
            (r'(?<foo_123>a)\g<foo_124>', 'aa', '', self.UNKNOWN_GROUP),  # Backref to undefined group.

            ('(?<foo_123>a)', 'a', '1', "'a'"),
            (r'(?<foo_123>a)\g<foo_123>', 'aa', '1', "'a'"),

            # Test octal escapes.
            ('\\1', 'a', '', self.UNKNOWN_GROUP),    # Backreference.
            ('[\\1]', '\1', '0', "'\\x01'"),  # Character.
            ('\\09', chr(0) + '9', '0', repr(chr(0) + '9')),
            ('\\141', 'a', '0', "'a'"),
            ('(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)\\119', 'abcdefghijklk9',
              '0,11', "('abcdefghijklk9', 'k')"),

            # Test \0 is handled everywhere.
            (r'\0', '\0', '0', repr('\0')),
            (r'[\0a]', '\0', '0', repr('\0')),
            (r'[a\0]', '\0', '0', repr('\0')),
            (r'[^a\0]', '\0', '', "None"),

            # Test various letter escapes.
            (r'\a[\b]\f\n\r\t\v', '\a\b\f\n\r\t\v', '0',
              repr('\a\b\f\n\r\t\v')),
            (r'[\a][\b][\f][\n][\r][\t][\v]', '\a\b\f\n\r\t\v', '0',
              repr('\a\b\f\n\r\t\v')),
            (r'\c\e\g\h\i\j\k\m\o\p\q\y\z', 'ceghijkmopqyz', '0',
              "'ceghijkmopqyz'"),
            (r'\xff', '\377', '0', repr(chr(255))),
            # New \x semantics.
            (r'\x00ffffffffffffff', '\377', '', "None"),
            (r'\x00f', '\017', '', "None"),
            (r'\x00fe', '\376', '', "None"),

            (r'\x00ff', '\377', '', "None"),
            (r'\t\n\v\r\f\a\g', '\t\n\v\r\f\ag', '0', repr('\t\n\v\r\f\ag')),
            ('\t\n\v\r\f\a\g', '\t\n\v\r\f\ag', '0', repr('\t\n\v\r\f\ag')),
            (r'\t\n\v\r\f\a', '\t\n\v\r\f\a', '0', repr(chr(9) + chr(10) + chr(11)
              + chr(13) + chr(12) + chr(7))),
            (r'[\t][\n][\v][\r][\f][\b]', '\t\n\v\r\f\b', '0',
              repr('\t\n\v\r\f\b')),

            (r"^\w+=(\\[\000-\277]|[^\n\\])*",
              "SRC=eval.c g.c blah blah blah \\\\\n\tapes.c", '0',
              repr("SRC=eval.c g.c blah blah blah \\\\")),

            # Test that . only matches \n in DOTALL mode.
            ('a.b', 'acb', '0', "'acb'"),
            ('a.b', 'a\nb', '', "None"),
            ('a.*b', 'acc\nccb', '', "None"),
            ('a.{4,5}b', 'acc\nccb', '', "None"),
            ('a.b', 'a\rb', '0', repr('a\rb')),
            # The new behaviour is that the inline flag affects only what follows.
            ('a.b(?s)', 'a\nb', '', "None"),
            ('(?s)a.b', 'a\nb', '0', repr('a\nb')),
            ('a.*(?s)b', 'acc\nccb', '', "None"),
            ('(?s)a.*b', 'acc\nccb', '0', repr('acc\nccb')),
            ('(?s)a.{4,5}b', 'acc\nccb', '0', repr('acc\nccb')),

            (')', '', '', self.TRAILING_CHARS),           # Unmatched right bracket.
            ('', '', '0', "''"),    # Empty pattern.
            ('abc', 'abc', '0', "'abc'"),
            ('abc', 'xbc', '', "None"),
            ('abc', 'axc', '', "None"),
            ('abc', 'abx', '', "None"),
            ('abc', 'xabcy', '0', "'abc'"),
            ('abc', 'ababc', '0', "'abc'"),
            ('ab*c', 'abc', '0', "'abc'"),
            ('ab*bc', 'abc', '0', "'abc'"),
            ('ab*bc', 'abbc', '0', "'abbc'"),
            ('ab*bc', 'abbbbc', '0', "'abbbbc'"),
            ('ab+bc', 'abbc', '0', "'abbc'"),
            ('ab+bc', 'abc', '', "None"),
            ('ab+bc', 'abq', '', "None"),
            ('ab+bc', 'abbbbc', '0', "'abbbbc'"),
            ('ab?bc', 'abbc', '0', "'abbc'"),
            ('ab?bc', 'abc', '0', "'abc'"),
            ('ab?bc', 'abbbbc', '', "None"),
            ('ab?c', 'abc', '0', "'abc'"),
            ('^abc$', 'abc', '0', "'abc'"),
            ('^abc$', 'abcc', '', "None"),
            ('^abc', 'abcc', '0', "'abc'"),
            ('^abc$', 'aabc', '', "None"),
            ('abc$', 'aabc', '0', "'abc'"),
            ('^', 'abc', '0', "''"),
            ('$', 'abc', '0', "''"),
            ('a.c', 'abc', '0', "'abc'"),
            ('a.c', 'axc', '0', "'axc'"),
            ('a.*c', 'axyzc', '0', "'axyzc'"),
            ('a.*c', 'axyzd', '', "None"),
            ('a[bc]d', 'abc', '', "None"),
            ('a[bc]d', 'abd', '0', "'abd'"),
            ('a[b-d]e', 'abd', '', "None"),
            ('a[b-d]e', 'ace', '0', "'ace'"),
            ('a[b-d]', 'aac', '0', "'ac'"),
            ('a[-b]', 'a-', '0', "'a-'"),
            ('a[\\-b]', 'a-', '0', "'a-'"),
            ('a[b-]', 'a-', '0', "'a-'"),
            ('a[]b', '-', '', self.BAD_SET),
            ('a[', '-', '', self.BAD_SET),
            ('a\\', '-', '', self.BAD_ESCAPE),
            ('abc)', '-', '', self.TRAILING_CHARS),
            ('(abc', '-', '', self.MISSING_RPAREN),
            ('a]', 'a]', '0', "'a]'"),
            ('a[]]b', 'a]b', '0', "'a]b'"),
            ('a[\]]b', 'a]b', '0', "'a]b'"),
            ('a[^bc]d', 'aed', '0', "'aed'"),
            ('a[^bc]d', 'abd', '', "None"),
            ('a[^-b]c', 'adc', '0', "'adc'"),
            ('a[^-b]c', 'a-c', '', "None"),
            ('a[^]b]c', 'a]c', '', "None"),
            ('a[^]b]c', 'adc', '0', "'adc'"),
            ('\\ba\\b', 'a-', '0', "'a'"),
            ('\\ba\\b', '-a', '0', "'a'"),
            ('\\ba\\b', '-a-', '0', "'a'"),
            ('\\by\\b', 'xy', '', "None"),
            ('\\by\\b', 'yz', '', "None"),
            ('\\by\\b', 'xyz', '', "None"),
            ('x\\b', 'xyz', '', "None"),
            ('x\\B', 'xyz', '0', "'x'"),
            ('\\Bz', 'xyz', '0', "'z'"),
            ('z\\B', 'xyz', '', "None"),
            ('\\Bx', 'xyz', '', "None"),
            ('\\Ba\\B', 'a-', '', "None"),
            ('\\Ba\\B', '-a', '', "None"),
            ('\\Ba\\B', '-a-', '', "None"),
            ('\\By\\B', 'xy', '', "None"),
            ('\\By\\B', 'yz', '', "None"),
            ('\\By\\b', 'xy', '0', "'y'"),
            ('\\by\\B', 'yz', '0', "'y'"),
            ('\\By\\B', 'xyz', '0', "'y'"),
            ('ab|cd', 'abc', '0', "'ab'"),
            ('ab|cd', 'abcd', '0', "'ab'"),
            ('()ef', 'def', '0,1', "('ef', '')"),
            ('$b', 'b', '', "None"),
            ('a\\(b', 'a(b', '', "('a(b',)"),
            ('a\\(*b', 'ab', '0', "'ab'"),
            ('a\\(*b', 'a((b', '0', "'a((b'"),
            ('a\\\\b', 'a\\b', '0', repr('a\\b')),
            ('((a))', 'abc', '0,1,2', "('a', 'a', 'a')"),
            ('(a)b(c)', 'abc', '0,1,2', "('abc', 'a', 'c')"),
            ('a+b+c', 'aabbabc', '0', "'abc'"),
            ('(a+|b)*', 'ab', '0,1', "('ab', 'b')"),
            ('(a+|b)+', 'ab', '0,1', "('ab', 'b')"),
            ('(a+|b)?', 'ab', '0,1', "('a', 'a')"),
            (')(', '-', '', self.TRAILING_CHARS),
            ('[^ab]*', 'cde', '0', "'cde'"),
            ('abc', '', '', "None"),
            ('a*', '', '0', "''"),
            ('a|b|c|d|e', 'e', '0', "'e'"),
            ('(a|b|c|d|e)f', 'ef', '0,1', "('ef', 'e')"),
            ('abcd*efg', 'abcdefg', '0', "'abcdefg'"),
            ('ab*', 'xabyabbbz', '0', "'ab'"),
            ('ab*', 'xayabbbz', '0', "'a'"),
            ('(ab|cd)e', 'abcde', '0,1', "('cde', 'cd')"),
            ('[abhgefdc]ij', 'hij', '0', "'hij'"),
            ('^(ab|cd)e', 'abcde', '', "None"),
            ('(abc|)ef', 'abcdef', '0,1', "('ef', '')"),
            ('(a|b)c*d', 'abcd', '0,1', "('bcd', 'b')"),
            ('(ab|ab*)bc', 'abc', '0,1', "('abc', 'a')"),
            ('a([bc]*)c*', 'abc', '0,1', "('abc', 'bc')"),
            ('a([bc]*)(c*d)', 'abcd', '0,1,2', "('abcd', 'bc', 'd')"),
            ('a([bc]+)(c*d)', 'abcd', '0,1,2', "('abcd', 'bc', 'd')"),
            ('a([bc]*)(c+d)', 'abcd', '0,1,2', "('abcd', 'b', 'cd')"),
            ('a[bcd]*dcdcde', 'adcdcde', '0', "'adcdcde'"),
            ('a[bcd]+dcdcde', 'adcdcde', '', "None"),
            ('(ab|a)b*c', 'abc', '0,1', "('abc', 'ab')"),
            ('((a)(b)c)(d)', 'abcd', '1,2,3,4', "('abc', 'a', 'b', 'd')"),
            ('[a-zA-Z_][a-zA-Z0-9_]*', 'alpha', '0', "'alpha'"),
            ('^a(bc+|b[eh])g|.h$', 'abh', '0,1', "('bh', None)"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effgz', '0,1,2',
              "('effgz', 'effgz', None)"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'ij', '0,1,2', "('ij', 'ij', 'j')"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effg', '', "None"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'bcdd', '', "None"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'reffgz', '0,1,2',
              "('effgz', 'effgz', None)"),
            ('(((((((((a)))))))))', 'a', '0', "'a'"),
            ('multiple words of text', 'uh-uh', '', "None"),
            ('multiple words', 'multiple words, yeah', '0', "'multiple words'"),
            ('(.*)c(.*)', 'abcde', '0,1,2', "('abcde', 'ab', 'de')"),
            ('\\((.*), (.*)\\)', '(a, b)', '2,1', "('b', 'a')"),
            ('[k]', 'ab', '', "None"),
            ('a[-]?c', 'ac', '0', "'ac'"),
            ('(abc)\\1', 'abcabc', '1', "'abc'"),
            ('([a-c]*)\\1', 'abcabc', '1', "'abc'"),
            ('^(.+)?B', 'AB', '1', "'A'"),
            ('(a+).\\1$', 'aaaaa', '0,1', "('aaaaa', 'aa')"),
            ('^(a+).\\1$', 'aaaa', '', "None"),
            ('(abc)\\1', 'abcabc', '0,1', "('abcabc', 'abc')"),
            ('([a-c]+)\\1', 'abcabc', '0,1', "('abcabc', 'abc')"),
            ('(a)\\1', 'aa', '0,1', "('aa', 'a')"),
            ('(a+)\\1', 'aa', '0,1', "('aa', 'a')"),
            ('(a+)+\\1', 'aa', '0,1', "('aa', 'a')"),
            ('(a).+\\1', 'aba', '0,1', "('aba', 'a')"),
            ('(a)ba*\\1', 'aba', '0,1', "('aba', 'a')"),
            ('(aa|a)a\\1$', 'aaa', '0,1', "('aaa', 'a')"),
            ('(a|aa)a\\1$', 'aaa', '0,1', "('aaa', 'a')"),
            ('(a+)a\\1$', 'aaa', '0,1', "('aaa', 'a')"),
            ('([abc]*)\\1', 'abcabc', '0,1', "('abcabc', 'abc')"),
            ('(a)(b)c|ab', 'ab', '0,1,2', "('ab', None, None)"),
            ('(a)+x', 'aaax', '0,1', "('aaax', 'a')"),
            ('([ac])+x', 'aacx', '0,1', "('aacx', 'c')"),
            ('([^/]*/)*sub1/', 'd:msgs/tdir/sub1/trial/away.cpp', '0,1',
              "('d:msgs/tdir/sub1/', 'tdir/')"),
            ('([^.]*)\\.([^:]*):[T ]+(.*)', 'track1.title:TBlah blah blah',
              '0,1,2,3',
              "('track1.title:TBlah blah blah', 'track1', 'title', 'Blah blah blah')"),
            ('([^N]*N)+', 'abNNxyzN', '0,1', "('abNNxyzN', 'xyzN')"),
            ('([^N]*N)+', 'abNNxyz', '0,1', "('abNN', 'N')"),
            ('([abc]*)x', 'abcx', '0,1', "('abcx', 'abc')"),
            ('([abc]*)x', 'abc', '', "None"),
            ('([xyz]*)x', 'abcx', '0,1', "('x', '')"),
            ('(a)+b|aac', 'aac', '0,1', "('aac', None)"),

            # Test symbolic groups.

            ('(?P<i d>aaa)a', 'aaaa', '', self.MISSING_GT),
            ('(?P<id>aaa)a', 'aaaa', '0,id', "('aaaa', 'aaa')"),
            ('(?P<id>aa)(?P=id)', 'aaaa', '0,id', "('aaaa', 'aa')"),
            ('(?P<id>aa)(?P=xd)', 'aaaa', '', self.UNKNOWN_GROUP),

            # Character properties.
            (ur"\g", u"g", '0', "u'g'"),
            (ur"\g<1>", u"g", '', self.UNKNOWN_GROUP),
            (ur"(.)\g<1>", u"gg", '0', "u'gg'"),
            (ur"(.)\g<1>", u"gg", '', "(u'gg', u'g')"),
            (ur"\N", u"N", '0', "u'N'"),
            (ur"\N{LATIN SMALL LETTER A}", u"a", '0', "u'a'"),
            (ur"\p", u"p", '0', "u'p'"),
            (ur"\p{Ll}", u"a", '0', "u'a'"),
            (ur"\P", u"P", '0', "u'P'"),
            (ur"\P{Lu}", u"p", '0', "u'p'"),

            # All tests from Perl.

            ('abc', 'abc', '0', "'abc'"),
            ('abc', 'xbc', '', "None"),
            ('abc', 'axc', '', "None"),
            ('abc', 'abx', '', "None"),
            ('abc', 'xabcy', '0', "'abc'"),
            ('abc', 'ababc', '0', "'abc'"),
            ('ab*c', 'abc', '0', "'abc'"),
            ('ab*bc', 'abc', '0', "'abc'"),
            ('ab*bc', 'abbc', '0', "'abbc'"),
            ('ab*bc', 'abbbbc', '0', "'abbbbc'"),
            ('ab{0,}bc', 'abbbbc', '0', "'abbbbc'"),
            ('ab+bc', 'abbc', '0', "'abbc'"),
            ('ab+bc', 'abc', '', "None"),
            ('ab+bc', 'abq', '', "None"),
            ('ab{1,}bc', 'abq', '', "None"),
            ('ab+bc', 'abbbbc', '0', "'abbbbc'"),
            ('ab{1,}bc', 'abbbbc', '0', "'abbbbc'"),
            ('ab{1,3}bc', 'abbbbc', '0', "'abbbbc'"),
            ('ab{3,4}bc', 'abbbbc', '0', "'abbbbc'"),
            ('ab{4,5}bc', 'abbbbc', '', "None"),
            ('ab?bc', 'abbc', '0', "'abbc'"),
            ('ab?bc', 'abc', '0', "'abc'"),
            ('ab{0,1}bc', 'abc', '0', "'abc'"),
            ('ab?bc', 'abbbbc', '', "None"),
            ('ab?c', 'abc', '0', "'abc'"),
            ('ab{0,1}c', 'abc', '0', "'abc'"),
            ('^abc$', 'abc', '0', "'abc'"),
            ('^abc$', 'abcc', '', "None"),
            ('^abc', 'abcc', '0', "'abc'"),
            ('^abc$', 'aabc', '', "None"),
            ('abc$', 'aabc', '0', "'abc'"),
            ('^', 'abc', '0', "''"),
            ('$', 'abc', '0', "''"),
            ('a.c', 'abc', '0', "'abc'"),
            ('a.c', 'axc', '0', "'axc'"),
            ('a.*c', 'axyzc', '0', "'axyzc'"),
            ('a.*c', 'axyzd', '', "None"),
            ('a[bc]d', 'abc', '', "None"),
            ('a[bc]d', 'abd', '0', "'abd'"),
            ('a[b-d]e', 'abd', '', "None"),
            ('a[b-d]e', 'ace', '0', "'ace'"),
            ('a[b-d]', 'aac', '0', "'ac'"),
            ('a[-b]', 'a-', '0', "'a-'"),
            ('a[b-]', 'a-', '0', "'a-'"),
            ('a[b-a]', '-', '', self.BAD_CHAR_RANGE),
            ('a[]b', '-', '', self.BAD_SET),
            ('a[', '-', '', self.BAD_SET),
            ('a]', 'a]', '0', "'a]'"),
            ('a[]]b', 'a]b', '0', "'a]b'"),
            ('a[^bc]d', 'aed', '0', "'aed'"),
            ('a[^bc]d', 'abd', '', "None"),
            ('a[^-b]c', 'adc', '0', "'adc'"),
            ('a[^-b]c', 'a-c', '', "None"),
            ('a[^]b]c', 'a]c', '', "None"),
            ('a[^]b]c', 'adc', '0', "'adc'"),
            ('ab|cd', 'abc', '0', "'ab'"),
            ('ab|cd', 'abcd', '0', "'ab'"),
            ('()ef', 'def', '0,1', "('ef', '')"),
            ('*a', '-', '', self.NOTHING_TO_REPEAT),
            ('(*)b', '-', '', self.NOTHING_TO_REPEAT),
            ('$b', 'b', '', "None"),
            ('a\\', '-', '', self.BAD_ESCAPE),
            ('a\\(b', 'a(b', '', "('a(b',)"),
            ('a\\(*b', 'ab', '0', "'ab'"),
            ('a\\(*b', 'a((b', '0', "'a((b'"),
            ('a\\\\b', 'a\\b', '0', repr('a\\b')),
            ('abc)', '-', '', self.TRAILING_CHARS),
            ('(abc', '-', '', self.MISSING_RPAREN),
            ('((a))', 'abc', '0,1,2', "('a', 'a', 'a')"),
            ('(a)b(c)', 'abc', '0,1,2', "('abc', 'a', 'c')"),
            ('a+b+c', 'aabbabc', '0', "'abc'"),
            ('a{1,}b{1,}c', 'aabbabc', '0', "'abc'"),
            ('a**', '-', '', self.NOTHING_TO_REPEAT),
            ('a.+?c', 'abcabc', '0', "'abc'"),
            ('(a+|b)*', 'ab', '0,1', "('ab', 'b')"),
            ('(a+|b){0,}', 'ab', '0,1', "('ab', 'b')"),
            ('(a+|b)+', 'ab', '0,1', "('ab', 'b')"),
            ('(a+|b){1,}', 'ab', '0,1', "('ab', 'b')"),
            ('(a+|b)?', 'ab', '0,1', "('a', 'a')"),
            ('(a+|b){0,1}', 'ab', '0,1', "('a', 'a')"),
            (')(', '-', '', self.TRAILING_CHARS),
            ('[^ab]*', 'cde', '0', "'cde'"),
            ('abc', '', '', "None"),
            ('a*', '', '0', "''"),
            ('([abc])*d', 'abbbcd', '0,1', "('abbbcd', 'c')"),
            ('([abc])*bcd', 'abcd', '0,1', "('abcd', 'a')"),
            ('a|b|c|d|e', 'e', '0', "'e'"),
            ('(a|b|c|d|e)f', 'ef', '0,1', "('ef', 'e')"),
            ('abcd*efg', 'abcdefg', '0', "'abcdefg'"),
            ('ab*', 'xabyabbbz', '0', "'ab'"),
            ('ab*', 'xayabbbz', '0', "'a'"),
            ('(ab|cd)e', 'abcde', '0,1', "('cde', 'cd')"),
            ('[abhgefdc]ij', 'hij', '0', "'hij'"),
            ('^(ab|cd)e', 'abcde', '', "None"),
            ('(abc|)ef', 'abcdef', '0,1', "('ef', '')"),
            ('(a|b)c*d', 'abcd', '0,1', "('bcd', 'b')"),
            ('(ab|ab*)bc', 'abc', '0,1', "('abc', 'a')"),
            ('a([bc]*)c*', 'abc', '0,1', "('abc', 'bc')"),
            ('a([bc]*)(c*d)', 'abcd', '0,1,2', "('abcd', 'bc', 'd')"),
            ('a([bc]+)(c*d)', 'abcd', '0,1,2', "('abcd', 'bc', 'd')"),
            ('a([bc]*)(c+d)', 'abcd', '0,1,2', "('abcd', 'b', 'cd')"),
            ('a[bcd]*dcdcde', 'adcdcde', '0', "'adcdcde'"),
            ('a[bcd]+dcdcde', 'adcdcde', '', "None"),
            ('(ab|a)b*c', 'abc', '0,1', "('abc', 'ab')"),
            ('((a)(b)c)(d)', 'abcd', '1,2,3,4', "('abc', 'a', 'b', 'd')"),
            ('[a-zA-Z_][a-zA-Z0-9_]*', 'alpha', '0', "'alpha'"),
            ('^a(bc+|b[eh])g|.h$', 'abh', '0,1', "('bh', None)"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effgz', '0,1,2',
              "('effgz', 'effgz', None)"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'ij', '0,1,2', "('ij', 'ij', 'j')"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'effg', '', "None"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'bcdd', '', "None"),
            ('(bc+d$|ef*g.|h?i(j|k))', 'reffgz', '0,1,2',
              "('effgz', 'effgz', None)"),
            ('((((((((((a))))))))))', 'a', '10', "'a'"),
            ('((((((((((a))))))))))\\10', 'aa', '0', "'aa'"),
            # Python does not have the same rules for \\41 so this is a syntax error
            #    ('((((((((((a))))))))))\\41', 'aa', '', "None"),
            #    ('((((((((((a))))))))))\\41', 'a!', '0', "'a!'"),
            ('((((((((((a))))))))))\\41', '', '', self.UNKNOWN_GROUP),
            ('(?i)((((((((((a))))))))))\\41', '', '', self.UNKNOWN_GROUP),
            ('(((((((((a)))))))))', 'a', '0', "'a'"),
            ('multiple words of text', 'uh-uh', '', "None"),
            ('multiple words', 'multiple words, yeah', '0', "'multiple words'"),
            ('(.*)c(.*)', 'abcde', '0,1,2', "('abcde', 'ab', 'de')"),
            ('\\((.*), (.*)\\)', '(a, b)', '2,1', "('b', 'a')"),
            ('[k]', 'ab', '', "None"),
            ('a[-]?c', 'ac', '0', "'ac'"),
            ('(abc)\\1', 'abcabc', '1', "'abc'"),
            ('([a-c]*)\\1', 'abcabc', '1', "'abc'"),
            ('(?i)abc', 'ABC', '0', "'ABC'"),
            ('(?i)abc', 'XBC', '', "None"),
            ('(?i)abc', 'AXC', '', "None"),
            ('(?i)abc', 'ABX', '', "None"),
            ('(?i)abc', 'XABCY', '0', "'ABC'"),
            ('(?i)abc', 'ABABC', '0', "'ABC'"),
            ('(?i)ab*c', 'ABC', '0', "'ABC'"),
            ('(?i)ab*bc', 'ABC', '0', "'ABC'"),
            ('(?i)ab*bc', 'ABBC', '0', "'ABBC'"),
            ('(?i)ab*?bc', 'ABBBBC', '0', "'ABBBBC'"),
            ('(?i)ab{0,}?bc', 'ABBBBC', '0', "'ABBBBC'"),
            ('(?i)ab+?bc', 'ABBC', '0', "'ABBC'"),
            ('(?i)ab+bc', 'ABC', '', "None"),
            ('(?i)ab+bc', 'ABQ', '', "None"),
            ('(?i)ab{1,}bc', 'ABQ', '', "None"),
            ('(?i)ab+bc', 'ABBBBC', '0', "'ABBBBC'"),
            ('(?i)ab{1,}?bc', 'ABBBBC', '0', "'ABBBBC'"),
            ('(?i)ab{1,3}?bc', 'ABBBBC', '0', "'ABBBBC'"),
            ('(?i)ab{3,4}?bc', 'ABBBBC', '0', "'ABBBBC'"),
            ('(?i)ab{4,5}?bc', 'ABBBBC', '', "None"),
            ('(?i)ab??bc', 'ABBC', '0', "'ABBC'"),
            ('(?i)ab??bc', 'ABC', '0', "'ABC'"),
            ('(?i)ab{0,1}?bc', 'ABC', '0', "'ABC'"),
            ('(?i)ab??bc', 'ABBBBC', '', "None"),
            ('(?i)ab??c', 'ABC', '0', "'ABC'"),
            ('(?i)ab{0,1}?c', 'ABC', '0', "'ABC'"),
            ('(?i)^abc$', 'ABC', '0', "'ABC'"),
            ('(?i)^abc$', 'ABCC', '', "None"),
            ('(?i)^abc', 'ABCC', '0', "'ABC'"),
            ('(?i)^abc$', 'AABC', '', "None"),
            ('(?i)abc$', 'AABC', '0', "'ABC'"),
            ('(?i)^', 'ABC', '0', "''"),
            ('(?i)$', 'ABC', '0', "''"),
            ('(?i)a.c', 'ABC', '0', "'ABC'"),
            ('(?i)a.c', 'AXC', '0', "'AXC'"),
            ('(?i)a.*?c', 'AXYZC', '0', "'AXYZC'"),
            ('(?i)a.*c', 'AXYZD', '', "None"),
            ('(?i)a[bc]d', 'ABC', '', "None"),
            ('(?i)a[bc]d', 'ABD', '0', "'ABD'"),
            ('(?i)a[b-d]e', 'ABD', '', "None"),
            ('(?i)a[b-d]e', 'ACE', '0', "'ACE'"),
            ('(?i)a[b-d]', 'AAC', '0', "'AC'"),
            ('(?i)a[-b]', 'A-', '0', "'A-'"),
            ('(?i)a[b-]', 'A-', '0', "'A-'"),
            ('(?i)a[b-a]', '-', '', self.BAD_CHAR_RANGE),
            ('(?i)a[]b', '-', '', self.BAD_SET),
            ('(?i)a[', '-', '', self.BAD_SET),
            ('(?i)a]', 'A]', '0', "'A]'"),
            ('(?i)a[]]b', 'A]B', '0', "'A]B'"),
            ('(?i)a[^bc]d', 'AED', '0', "'AED'"),
            ('(?i)a[^bc]d', 'ABD', '', "None"),
            ('(?i)a[^-b]c', 'ADC', '0', "'ADC'"),
            ('(?i)a[^-b]c', 'A-C', '', "None"),
            ('(?i)a[^]b]c', 'A]C', '', "None"),
            ('(?i)a[^]b]c', 'ADC', '0', "'ADC'"),
            ('(?i)ab|cd', 'ABC', '0', "'AB'"),
            ('(?i)ab|cd', 'ABCD', '0', "'AB'"),
            ('(?i)()ef', 'DEF', '0,1', "('EF', '')"),
            ('(?i)*a', '-', '', self.NOTHING_TO_REPEAT),
            ('(?i)(*)b', '-', '', self.NOTHING_TO_REPEAT),
            ('(?i)$b', 'B', '', "None"),
            ('(?i)a\\', '-', '', self.BAD_ESCAPE),
            ('(?i)a\\(b', 'A(B', '', "('A(B',)"),
            ('(?i)a\\(*b', 'AB', '0', "'AB'"),
            ('(?i)a\\(*b', 'A((B', '0', "'A((B'"),
            ('(?i)a\\\\b', 'A\\B', '0', repr('A\\B')),
            ('(?i)abc)', '-', '', self.TRAILING_CHARS),
            ('(?i)(abc', '-', '', self.MISSING_RPAREN),
            ('(?i)((a))', 'ABC', '0,1,2', "('A', 'A', 'A')"),
            ('(?i)(a)b(c)', 'ABC', '0,1,2', "('ABC', 'A', 'C')"),
            ('(?i)a+b+c', 'AABBABC', '0', "'ABC'"),
            ('(?i)a{1,}b{1,}c', 'AABBABC', '0', "'ABC'"),
            ('(?i)a**', '-', '', self.NOTHING_TO_REPEAT),
            ('(?i)a.+?c', 'ABCABC', '0', "'ABC'"),
            ('(?i)a.*?c', 'ABCABC', '0', "'ABC'"),
            ('(?i)a.{0,5}?c', 'ABCABC', '0', "'ABC'"),
            ('(?i)(a+|b)*', 'AB', '0,1', "('AB', 'B')"),
            ('(?i)(a+|b){0,}', 'AB', '0,1', "('AB', 'B')"),
            ('(?i)(a+|b)+', 'AB', '0,1', "('AB', 'B')"),
            ('(?i)(a+|b){1,}', 'AB', '0,1', "('AB', 'B')"),
            ('(?i)(a+|b)?', 'AB', '0,1', "('A', 'A')"),
            ('(?i)(a+|b){0,1}', 'AB', '0,1', "('A', 'A')"),
            ('(?i)(a+|b){0,1}?', 'AB', '0,1', "('', None)"),
            ('(?i))(', '-', '', self.TRAILING_CHARS),
            ('(?i)[^ab]*', 'CDE', '0', "'CDE'"),
            ('(?i)abc', '', '', "None"),
            ('(?i)a*', '', '0', "''"),
            ('(?i)([abc])*d', 'ABBBCD', '0,1', "('ABBBCD', 'C')"),
            ('(?i)([abc])*bcd', 'ABCD', '0,1', "('ABCD', 'A')"),
            ('(?i)a|b|c|d|e', 'E', '0', "'E'"),
            ('(?i)(a|b|c|d|e)f', 'EF', '0,1', "('EF', 'E')"),
            ('(?i)abcd*efg', 'ABCDEFG', '0', "'ABCDEFG'"),
            ('(?i)ab*', 'XABYABBBZ', '0', "'AB'"),
            ('(?i)ab*', 'XAYABBBZ', '0', "'A'"),
            ('(?i)(ab|cd)e', 'ABCDE', '0,1', "('CDE', 'CD')"),
            ('(?i)[abhgefdc]ij', 'HIJ', '0', "'HIJ'"),
            ('(?i)^(ab|cd)e', 'ABCDE', '', "None"),
            ('(?i)(abc|)ef', 'ABCDEF', '0,1', "('EF', '')"),
            ('(?i)(a|b)c*d', 'ABCD', '0,1', "('BCD', 'B')"),
            ('(?i)(ab|ab*)bc', 'ABC', '0,1', "('ABC', 'A')"),
            ('(?i)a([bc]*)c*', 'ABC', '0,1', "('ABC', 'BC')"),
            ('(?i)a([bc]*)(c*d)', 'ABCD', '0,1,2', "('ABCD', 'BC', 'D')"),
            ('(?i)a([bc]+)(c*d)', 'ABCD', '0,1,2', "('ABCD', 'BC', 'D')"),
            ('(?i)a([bc]*)(c+d)', 'ABCD', '0,1,2', "('ABCD', 'B', 'CD')"),
            ('(?i)a[bcd]*dcdcde', 'ADCDCDE', '0', "'ADCDCDE'"),
            ('(?i)a[bcd]+dcdcde', 'ADCDCDE', '', "None"),
            ('(?i)(ab|a)b*c', 'ABC', '0,1', "('ABC', 'AB')"),
            ('(?i)((a)(b)c)(d)', 'ABCD', '1,2,3,4', "('ABC', 'A', 'B', 'D')"),
            ('(?i)[a-zA-Z_][a-zA-Z0-9_]*', 'ALPHA', '0', "'ALPHA'"),
            ('(?i)^a(bc+|b[eh])g|.h$', 'ABH', '0,1', "('BH', None)"),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'EFFGZ', '0,1,2', "('EFFGZ', 'EFFGZ', None)"),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'IJ', '0,1,2', "('IJ', 'IJ', 'J')"),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'EFFG', '', "None"),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'BCDD', '', "None"),
            ('(?i)(bc+d$|ef*g.|h?i(j|k))', 'REFFGZ', '0,1,2', "('EFFGZ', 'EFFGZ', None)"),
            ('(?i)((((((((((a))))))))))', 'A', '10', "'A'"),
            ('(?i)((((((((((a))))))))))\\10', 'AA', '0', "'AA'"),
            #('(?i)((((((((((a))))))))))\\41', 'AA', '', "None"),
            #('(?i)((((((((((a))))))))))\\41', 'A!', '0', "'A!'"),
            ('(?i)(((((((((a)))))))))', 'A', '0', "'A'"),
            ('(?i)(?:(?:(?:(?:(?:(?:(?:(?:(?:(a))))))))))', 'A', '1', "'A'"),
            ('(?i)(?:(?:(?:(?:(?:(?:(?:(?:(?:(a|b|c))))))))))', 'C', '1', "'C'"),
            ('(?i)multiple words of text', 'UH-UH', '', "None"),
            ('(?i)multiple words', 'MULTIPLE WORDS, YEAH', '0', "'MULTIPLE WORDS'"),
            ('(?i)(.*)c(.*)', 'ABCDE', '0,1,2', "('ABCDE', 'AB', 'DE')"),
            ('(?i)\\((.*), (.*)\\)', '(A, B)', '2,1', "('B', 'A')"),
            ('(?i)[k]', 'AB', '', "None"),
        #    ('(?i)abcd', 'ABCD', SUCCEED, 'found+"-"+\\found+"-"+\\\\found', 'ABCD-$&-\\ABCD'),
        #    ('(?i)a(bc)d', 'ABCD', SUCCEED, 'g1+"-"+\\g1+"-"+\\\\g1', 'BC-$1-\\BC'),
            ('(?i)a[-]?c', 'AC', '0', "'AC'"),
            ('(?i)(abc)\\1', 'ABCABC', '1', "'ABC'"),
            ('(?i)([a-c]*)\\1', 'ABCABC', '1', "'ABC'"),
            ('a(?!b).', 'abad', '0', "'ad'"),
            ('a(?=d).', 'abad', '0', "'ad'"),
            ('a(?=c|d).', 'abad', '0', "'ad'"),
            ('a(?:b|c|d)(.)', 'ace', '1', "'e'"),
            ('a(?:b|c|d)*(.)', 'ace', '1', "'e'"),
            ('a(?:b|c|d)+?(.)', 'ace', '1', "'e'"),
            ('a(?:b|(c|e){1,2}?|d)+?(.)', 'ace', '1,2', "('c', 'e')"),

            # Lookbehind: split by : but not if it is escaped by -.
            ('(?<!-):(.*?)(?<!-):', 'a:bc-:de:f', '1', "'bc-:de'"),
            # Escaping with \ as we know it.
            ('(?<!\\\):(.*?)(?<!\\\):', 'a:bc\\:de:f', '1', repr('bc\\:de')),
            # Terminating with ' and escaping with ? as in edifact.
            ("(?<!\\?)'(.*?)(?<!\\?)'", "a'bc?'de'f", '1', repr("bc?'de")),

            # Comments using the (?#...) syntax.

            ('w(?# comment', 'w', '', self.MISSING_RPAREN),
            ('w(?# comment 1)xy(?# comment 2)z', 'wxyz', '0', "'wxyz'"),

            # Check odd placement of embedded pattern modifiers.

            # Not an error under PCRE/PRE:
            # New behaviour is that inline flags affect only what follows.
            ('w(?i)', 'W', '0', "None"),
            ('w(?i)', 'w', '0', "'w'"),
            ('(?i)w', 'W', '0', "'W'"),

            # Comments using the x embedded pattern modifier.

            ("""(?x)w# comment 1
x y
# comment 2
z""", 'wxyz', '0', "'wxyz'"),

            # Using the m embedded pattern modifier.

            ('^abc', """jkl
abc
xyz""", '', "None"),
            ('(?m)^abc', """jkl
abc
xyz""", '0', "'abc'"),

            ('(?m)abc$', """jkl
xyzabc
123""", '0', "'abc'"),

            # Using the s embedded pattern modifier.

            ('a.b', 'a\nb', '', "None"),
            ('(?s)a.b', 'a\nb', '0', repr('a\nb')),

            # Test \w, etc. both inside and outside character classes.

            ('\\w+', '--ab_cd0123--', '0', "'ab_cd0123'"),
            ('[\\w]+', '--ab_cd0123--', '0', "'ab_cd0123'"),
            ('\\D+', '1234abc5678', '0', "'abc'"),
            ('[\\D]+', '1234abc5678', '0', "'abc'"),
            ('[\\da-fA-F]+', '123abc', '0', "'123abc'"),
            # Not an error under PCRE/PRE:
            # ('[\\d-x]', '-', '', self.SYNTAX_ERROR),
            (r'([\s]*)([\S]*)([\s]*)', ' testing!1972', '3,2,1',
              "('', 'testing!1972', ' ')"),
            (r'(\s*)(\S*)(\s*)', ' testing!1972', '3,2,1',
              "('', 'testing!1972', ' ')"),

            #
            # Post-1.5.2 additions.

            # xmllib problem.
            (r'(([a-z]+):)?([a-z]+)$', 'smil', '1,2,3', "(None, None, 'smil')"),
            # Bug 110866: reference to undefined group.
            (r'((.)\1+)', '', '', self.OPEN_GROUP),
            # Bug 111869: search (PRE/PCRE fails on this one, SRE doesn't).
            (r'.*d', 'abc\nabd', '0', "'abd'"),
            # Bug 112468: various expected syntax errors.
            (r'(', '', '', self.MISSING_RPAREN),
            (r'[\41]', '!', '0', "'!'"),
            # Bug 114033: nothing to repeat.
            (r'(x?)?', 'x', '0', "'x'"),
            # Bug 115040: rescan if flags are modified inside pattern.
            # New behaviour is that inline flags affect only what follows.
            (r' (?x)foo ', 'foo', '0', "None"),
            (r'(?x) foo ', 'foo', '0', "'foo'"),
            (r'(?x)foo ', 'foo', '0', "'foo'"),
            # Bug 115618: negative lookahead.
            (r'(?<!abc)(d.f)', 'abcdefdof', '0', "'dof'"),
            # Bug 116251: character class bug.
            (r'[\w-]+', 'laser_beam', '0', "'laser_beam'"),
            # Bug 123769+127259: non-greedy backtracking bug.
            (r'.*?\S *:', 'xx:', '0', "'xx:'"),
            (r'a[ ]*?\ (\d+).*', 'a   10', '0', "'a   10'"),
            (r'a[ ]*?\ (\d+).*', 'a    10', '0', "'a    10'"),
            # Bug 127259: \Z shouldn't depend on multiline mode.
            (r'(?ms).*?x\s*\Z(.*)','xx\nx\n', '1', "''"),
            # Bug 128899: uppercase literals under the ignorecase flag.
            (r'(?i)M+', 'MMM', '0', "'MMM'"),
            (r'(?i)m+', 'MMM', '0', "'MMM'"),
            (r'(?i)[M]+', 'MMM', '0', "'MMM'"),
            (r'(?i)[m]+', 'MMM', '0', "'MMM'"),
            # Bug 130748: ^* should be an error (nothing to repeat).
            (r'^*', '', '', self.NOTHING_TO_REPEAT),
            # Bug 133283: minimizing repeat problem.
            (r'"(?:\\"|[^"])*?"', r'"\""', '0', repr(r'"\""')),
            # Bug 477728: minimizing repeat problem.
            (r'^.*?$', 'one\ntwo\nthree\n', '', "None"),
            # Bug 483789: minimizing repeat problem.
            (r'a[^>]*?b', 'a>b', '', "None"),
            # Bug 490573: minimizing repeat problem.
            (r'^a*?$', 'foo', '', "None"),
            # Bug 470582: nested groups problem.
            (r'^((a)c)?(ab)$', 'ab', '1,2,3', "(None, None, 'ab')"),
            # Another minimizing repeat problem (capturing groups in assertions).
            ('^([ab]*?)(?=(b)?)c', 'abc', '1,2', "('ab', None)"),
            ('^([ab]*?)(?!(b))c', 'abc', '1,2', "('ab', None)"),
            ('^([ab]*?)(?<!(a))c', 'abc', '1,2', "('ab', None)"),
            # Bug 410271: \b broken under locales.
            (r'\b.\b', 'a', '0', "'a'"),
            (ur'(?u)\b.\b', u'\N{LATIN CAPITAL LETTER A WITH DIAERESIS}',
              '0', "u'\\xc4'"),
            (ur'(?u)\w', u'\N{LATIN CAPITAL LETTER A WITH DIAERESIS}',
              '0', "u'\\xc4'"),
        ]

        for t in tests:
            self.index += 1
            try:
                pattern, string, groups, expected = t
            except ValueError:
                fields = ", ".join([repr(f) for f in t[ : 3]] + ["..."])
                self.record_failure("Incorrect number of test fields: (%s)" % fields)
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
                    self.record_failure("Expected %s but got %s instead: (%s)" % (expected,
                      actual, fields))

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
