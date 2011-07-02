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

import string
import sys
import unicodedata
from collections import defaultdict

import _regex

__all__ = ["A", "ASCII", "D", "DEBUG", "I", "IGNORECASE", "L", "LOCALE", "M",
  "MULTILINE", "N", "NEW", "R", "REVERSE", "S", "DOTALL", "U", "UNICODE", "W",
  "WORD", "X", "VERBOSE", "T", "TEMPLATE", "error", "ALNUM", "NONLITERAL",
  "Info", "Source", "FirstSetError", "UnscopedFlagSet", "OP", "Scanner",
  "compile_firstset", "compile_repl_escape", "count_ones", "flatten_code",
  "fold_string_case", "parse_pattern", "shrink_cache"]

# The regex exception.
class error(Exception):
    def __init__(self, message, set_error=False):
        Exception.__init__(self, message)
        self.set_error = set_error

# The exception for when a positional flag has been turned on in the old
# behaviour.
class UnscopedFlagSet(Exception):
    def __init__(self, global_flags):
        Exception.__init__(self)
        self.global_flags = global_flags

# The exception for when parsing fails and we want to try something else.
class ParseError(Exception):
    pass

# The exception for when there isn't a valid first set.
class FirstSetError(Exception):
    pass

# Flags.
A = ASCII = 0x80     # Assume ASCII locale.
D = DEBUG = 0x200    # Print parsed pattern.
I = IGNORECASE = 0x2 # Ignore case.
L = LOCALE = 0x4     # Assume current 8-bit locale.
M = MULTILINE = 0x8  # Make anchors look for newline.
N = NEW = 0x100      # Scoped inline flags and correct handling of zero-width
                     # matches.
R = REVERSE = 0x400  # Search backwards.
S = DOTALL = 0x10    # Make dot match newline.
U = UNICODE = 0x20   # Assume Unicode locale.
W = WORD = 0x800     # Default Unicode word breaks.
X = VERBOSE = 0x40   # Ignore whitespace and comments.
T = TEMPLATE = 0x1   # Template (present because re module has it).

# The mask for the flags.
GLOBAL_FLAGS = ASCII | DEBUG | LOCALE | NEW | REVERSE | UNICODE
SCOPED_FLAGS = IGNORECASE | MULTILINE | DOTALL | WORD | VERBOSE

ALPHA = frozenset(string.ascii_letters)
DIGITS = frozenset(string.digits)
ALNUM = ALPHA | DIGITS
OCT_DIGITS = frozenset(string.octdigits)
HEX_DIGITS = frozenset(string.hexdigits)
NONLITERAL = frozenset("()[]{}?*+|^$\\")
SPECIAL_CHARS = set("()|?*+{^$.[\\#") | set([""])
SET_OPS = ("||", "~~", "&&", "--")

# The width of the code words inside the regex engine.
BYTES_PER_CODE = _regex.get_code_size()
BITS_PER_CODE = BYTES_PER_CODE * 8

# The repeat count which represents infinity.
UNLIMITED = (1 << BITS_PER_CODE) - 1

# The regular expression flags.
REGEX_FLAGS = {"a": ASCII, "i": IGNORECASE, "L": LOCALE, "m": MULTILINE, "n":
  NEW, "r": REVERSE, "s": DOTALL, "u": UNICODE, "w": WORD, "x": VERBOSE}

# The number of digits in hexadecimal escapes.
HEX_ESCAPES = {"x": 2, "u": 4, "U": 8}

# The names of the opcodes.
OPCODES = """
FAILURE
SUCCESS
ANY
ANY_ALL
ANY_ALL_REV
ANY_REV
ANY_U
ANY_U_REV
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
END_FUZZY
END_GREEDY_REPEAT
END_GROUP
END_LAZY_REPEAT
END_OF_LINE
END_OF_LINE_U
END_OF_STRING
END_OF_STRING_LINE
END_OF_STRING_LINE_U
FUZZY
GRAPHEME_BOUNDARY
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
SET_DIFF
SET_DIFF_REV
SET_INTER
SET_INTER_REV
SET_SYM_DIFF
SET_SYM_DIFF_REV
SET_UNION
SET_UNION_REV
SMALL_BITSET
SMALL_BITSET_REV
START_GROUP
START_OF_LINE
START_OF_LINE_U
START_OF_STRING
STRING
STRING_IGN
STRING_IGN_REV
STRING_REV
STRING_SET
STRING_SET_IGN
STRING_SET_IGN_REV
STRING_SET_REV
"""

# Define the opcodes in a namespace.
class Namespace(object):
    pass

OP = Namespace()
for i, op in enumerate(OPCODES.split()):
    setattr(OP, op, i)

def shrink_cache(cache_dict, args_dict, max_length, divisor=5):
    """Make room in the given cache.

    Args:
        cache_dict: The cache dictionary to modify.
        args_dict: The dictionary of named list args used by patterns.
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

    # Rebuild the arguments dictionary.
    args_dict.clear()
    for pattern, pattern_type, flags, args in cache_dict:
        args_dict[pattern, pattern_type, flags] = args

def all_cases(info, ch):
    "Gets all the cases of a character."
    encoding = info.global_flags & (ASCII | LOCALE | UNICODE)
    return _regex.all_cases(encoding or info.guess_encoding, ch)

def fold_string_case(info, string):
    "Folds the cases of a string."
    encoding = info.global_flags & (ASCII | LOCALE | UNICODE)
    encoding = encoding or info.guess_encoding
    if isinstance(string, unicode):
        return u"".join(unichr(_regex.folded_case(encoding, ord(c))) for c in
          string)
    else:
        return "".join(chr(_regex.folded_case(encoding, ord(c))) for c in
          string)

def unique(items):
    "Removes duplicate items."
    new_items = []
    seen = set()
    for i in items:
        if i not in seen:
            new_items.append(i)
            seen.add(i)

    return type(items)(new_items)

def compile_firstset(info, fs):
    "Compiles the firstset for the pattern."
    items = set()
    for m in fs:
        if isinstance(m, Character) and m.positive or isinstance(m, Property):
            items.add(m)
        elif isinstance(m, SetUnion) and m.positive:
            for i in m.items:
                if isinstance(i, (Character, Property)):
                    items.add(i)
                else:
                    return []
        else:
            return []

    fs = SetUnion(list(items), zerowidth=True).optimise(info)
    if not fs:
        # No firstset.
        return []

    # Compile the firstset.
    return fs.compile(bool(info.global_flags & REVERSE))

def count_ones(n):
    "Counts the number of set bits in an int."
    count = 0
    while n:
        count += 1
        n &= n - 1

    return count

def flatten_code(code):
    "Flattens the code from a list of tuples."
    flat_code = []
    for c in code:
        flat_code.extend(c)

    return flat_code

def char_literal(info, value):
    "Creates a character literal."
    if info.all_flags & IGNORECASE:
        return CharacterIgn(info, value)

    return Character(info, value)

def ref_group(info, name):
    "Creates a group reference."
    if info.all_flags & IGNORECASE:
        return RefGroupIgn(info, name)

    return RefGroup(info, name)

def string_set(info, name):
    "Creates a string set."
    if info.all_flags & IGNORECASE:
        return StringSetIgn(info, name)

    return StringSet(info, name)

def parse_pattern(source, info):
    "Parses a pattern, eg. 'a|b|c'."
    # Capture group names can be duplicated provided that their matching is
    # mutually exclusive.
    previous_groups = info.used_groups.copy()
    branches = [parse_sequence(source, info)]
    all_groups = info.used_groups
    while source.match("|"):
        info.used_groups = previous_groups.copy()
        branches.append(parse_sequence(source, info))
        all_groups |= info.used_groups

    info.used_groups = all_groups

    if len(branches) == 1:
        return branches[0]
    return Branch(branches)

def parse_sequence(source, info):
    "Parses a sequence, eg. 'abc'."
    sequence = []
    item = parse_item(source, info)
    while item:
        sequence.append(item)
        item = parse_item(source, info)

    if len(sequence) == 1:
        return sequence[0]
    return Sequence(sequence)

def PossessiveRepeat(element, min_count, max_count):
    "Builds a possessive repeat."
    return Atomic(GreedyRepeat(element, min_count, max_count))

def parse_item(source, info):
    "Parses an item, which might be repeated. Returns None if there's no item."
    element = parse_element(source, info)
    counts = parse_quantifier(source, info)
    if counts:
        # Is there an element to repeat?
        if not element or not element.can_repeat():
            raise error("nothing to repeat")

        min_count, max_count = counts
        here = source.pos
        ch = source.get()
        if ch == "?":
            # The "?" suffix that means it's a lazy repeat.
            repeated = LazyRepeat
        elif ch == "+":
            # The "+" suffix that means it's a possessive repeat.
            repeated = PossessiveRepeat
        else:
            # No suffix means that it's a greedy repeat.
            source.pos = here
            repeated = GreedyRepeat

        if not element or min_count == max_count == 1:
            return element

        return repeated(element, min_count, max_count)

    # No quantifier, but maybe there's a fuzzy constraint.
    constraints = parse_fuzzy(source)
    if not constraints:
        # No fuzzy constraint.
        return element

    # If a group is marked as fuzzy then put all of the fuzzy part in the
    # group.
    if isinstance(element, Group):
        element.subpattern = Fuzzy(element.subpattern, constraints)
        return element

    return Fuzzy(element, constraints)

def parse_quantifier(source, info):
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
        try:
            return parse_limited_quantifier(source)
        except ParseError:
            # Not a limited quantifier.
            pass

    # Parse it later, perhaps as a literal.
    source.pos = here
    return None

def parse_limited_quantifier(source):
    "Parses a limited quantifier."
    min_count = parse_count(source)
    ch = source.get()
    if ch == ",":
        max_count = parse_count(source)
        if not source.match("}"):
            raise ParseError()

        # No minimum means 0 and no maximum means unlimited.
        min_count = int(min_count) if min_count else 0
        max_count = int(max_count) if max_count else None
        if max_count is not None and min_count > max_count:
            raise error("min repeat greater than max repeat")

        if min_count >= UNLIMITED or max_count is not None and max_count >= \
          UNLIMITED:
            raise error("repeat count too big")

        return min_count, max_count

    if ch != "}":
        raise ParseError()

    if not min_count:
        # Not a quantifier.
        raise ParseError()

    min_count = max_count = int(min_count)
    if min_count >= UNLIMITED:
        raise error("repeat count too big")

    return min_count, max_count

def parse_fuzzy(source):
    "Parses a fuzzy setting, if present."
    here = source.pos
    if not source.match("{"):
        source.pos = here
        return None

    saved_ignore = source.ignore_space
    source.ignore_space = True

    constraints = {}
    try:
        parse_fuzzy_item(source, constraints)
        while source.match(","):
            parse_fuzzy_item(source, constraints)
    except ParseError:
        source.pos = here

        return None
    finally:
        source.ignore_space = saved_ignore

    if not source.match("}"):
        raise error("expected }")

    return constraints

def parse_fuzzy_item(source, constraints):
    "Parses a fuzzy setting item."
    here = source.pos
    ch = source.get()
    if not ch:
        raise ParseError()

    if ch in "deis":
        # It's a single error constraint.
        if ch in constraints:
            raise error("repeated fuzzy constraint")

        if source.match("<="):
            try:
                constraints[ch] = int(parse_count(source))
            except ValueError:
                raise error("missing maximum")
        elif source.match("<"):
            try:
                constraints[ch] = max(int(parse_count(source)) - 1, 0)
            except ValueError:
                raise error("missing maximum")
        else:
            # There's no maximum.
            constraints[ch] = None
    else:
        # It's a cost equation.
        source.pos = here
        cost = parse_cost_equation(source)
        if "cost" in constraints:
            raise error("more than one cost equation")

        constraints["cost"] = cost

def parse_cost_equation(source):
    "Parses a cost equation."
    cost = {}
    parse_cost_term(source, cost)
    while source.match("+"):
        parse_cost_term(source, cost)

    if source.match("<="):
        try:
            cost["max"] = int(parse_count(source))
        except ValueError:
            raise error("missing maximum")
    elif source.match("<"):
        try:
            cost["max"] = max(int(parse_count(source)) - 1, 0)
        except ValueError:
            raise error("missing maximum")
    else:
        raise error("missing maximum")

    return cost

def parse_cost_term(source, cost):
    "Parses a cost equation term."
    coeff = parse_count(source)
    ch = source.get()
    if ch not in "dis":
        raise ParseError()

    if ch in cost:
        raise error("repeated cost")

    cost[ch] = int(coeff) if coeff else 1

def parse_count(source):
    "Parses a quantifier's count, which can be empty."
    count = []
    here = source.pos
    ch = source.get()
    while ch in DIGITS:
        count.append(ch)
        here = source.pos
        ch = source.get()

    source.pos = here
    return "".join(count)

def parse_element(source, info):
    "Parses an element. An element might actually be a flag, eg. '(?i)'."
    while True:
        here = source.pos
        ch = source.get()
        if ch in SPECIAL_CHARS:
            if ch in ")|":
                # The end of a sequence. At the end of the pattern ch is "".
                source.pos = here
                return None
            elif ch == "\\":
                # An escape sequence.
                return parse_escape(source, info, False)
            elif ch == "(":
                # A parenthesised subpattern or a flag.
                element = parse_paren(source, info)
                if element:
                    return element
            elif ch == ".":
                # Any character.
                if info.all_flags & DOTALL:
                    return AnyAll()
                elif info.all_flags & WORD:
                    return AnyU()
                else:
                    return Any()
            elif ch == "[":
                # A character set.
                return parse_set(source, info)
            elif ch == "^":
                # The start of a line or the string.
                if info.all_flags & MULTILINE:
                    if info.all_flags & WORD:
                        return StartOfLineU()
                    else:
                        return StartOfLine()
                else:
                    return StartOfString()
            elif ch == "$":
                # The end of a line or the string.
                if info.all_flags & MULTILINE:
                    if info.all_flags & WORD:
                        return EndOfLineU()
                    else:
                        return EndOfLine()
                else:
                    if info.all_flags & WORD:
                        return EndOfStringLineU()
                    else:
                        return EndOfStringLine()
            elif ch == "{":
                # Looks like a limited quantifier.
                here2 = source.pos
                source.pos = here
                counts = parse_quantifier(source, info)
                if counts:
                    # A quantifier where we expected an element.
                    raise error("nothing to repeat")

                # Not a quantifier, so it's a literal.
                source.pos = here2
                return Character(info, ord(ch))
            elif ch in "?*+":
                # A quantifier where we expected an element.
                raise error("nothing to repeat")
            elif info.all_flags & VERBOSE:
                if ch == "#":
                    # A comment.
                    source.ignore_space = False
                    try:
                        # Ignore characters until a newline or the end of the
                        # pattern.
                        while source.get() not in "\n":
                            pass
                    finally:
                        source.ignore_space = True
                else:
                    # A literal.
                    return char_literal(info, ord(ch))
            else:
                # A literal.
                return char_literal(info, ord(ch))
        else:
            # A literal.
            return char_literal(info, ord(ch))

def parse_paren(source, info):
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
                return parse_lookaround(source, info, True, True)
            if ch == "!":
                # Negative lookbehind.
                return parse_lookaround(source, info, True, False)

            # A named capture group.
            source.pos = here3
            name = parse_name(source)
            group = info.new_group(name)
            source.expect(">")
            saved_scoped_flags = info.scoped_flags
            saved_ignore = source.ignore_space
            try:
                subpattern = parse_pattern(source, info)
            finally:
                source.ignore_space = saved_ignore
                info.scoped_flags = saved_scoped_flags

            source.expect(")")
            info.close_group(group)
            return Group(info, group, subpattern)
        if ch == "=":
            # Positive lookahead.
            return parse_lookaround(source, info, False, True)
        if ch == "!":
            # Negative lookahead.
            return parse_lookaround(source, info, False, False)
        if ch == "P":
            # A Python extension.
            return parse_extension(source, info)
        if ch == "#":
            # A comment.
            return parse_comment(source)
        if ch == "(":
            # A conditional subpattern.
            return parse_conditional(source, info)
        if ch == ">":
            # An atomic subpattern.
            return parse_atomic(source, info)
        if ch == "|":
            # A common groups branch.
            return parse_common(source, info)

        # A flags subpattern.
        source.pos = here2
        return parse_flags_subpattern(source, info)

    # An unnamed capture group.
    source.pos = here
    group = info.new_group()
    saved_scoped_flags = info.scoped_flags
    saved_ignore = source.ignore_space
    try:
        subpattern = parse_pattern(source, info)
    finally:
        source.ignore_space = saved_ignore
        info.scoped_flags = saved_scoped_flags
        info.all_flags = info.global_flags | info.scoped_flags

    source.expect(")")
    info.close_group(group)

    return Group(info, group, subpattern)

def parse_extension(source, info):
    "Parses a Python extension."
    here = source.pos
    ch = source.get()
    if ch == "<":
        # A named capture group.
        name = parse_name(source)
        group = info.new_group(name)
        source.expect(">")
        saved_scoped_flags = info.scoped_flags
        saved_ignore = source.ignore_space
        try:
            subpattern = parse_pattern(source, info)
        finally:
            source.ignore_space = saved_ignore
            info.scoped_flags = saved_scoped_flags

        source.expect(")")
        info.close_group(group)

        return Group(info, group, subpattern)

    if ch == "=":
        # A named group reference.
        name = parse_name(source)
        source.expect(")")
        if info.is_open_group(name):
            raise error("can't refer to an open group")

        return ref_group(info, name)

    source.pos = here
    raise error("unknown extension")

def parse_comment(source):
    "Parses a comment."
    ch = source.get()
    while ch not in ")":
        ch = source.get()

    if not ch:
        raise error("missing )")

    return None

def parse_lookaround(source, info, behind, positive):
    "Parses a lookaround."
    saved_scoped_flags = info.scoped_flags
    saved_ignore = source.ignore_space
    try:
        subpattern = parse_pattern(source, info)
    finally:
        source.ignore_space = saved_ignore
        info.scoped_flags = saved_scoped_flags

    source.expect(")")

    return LookAround(behind, positive, subpattern)

def parse_conditional(source, info):
    "Parses a conditional subpattern."
    saved_scoped_flags = info.scoped_flags
    saved_ignore = source.ignore_space
    try:
        group = parse_name(source, True)
        source.expect(")")
        previous_groups = info.used_groups.copy()
        yes_branch = parse_sequence(source, info)
        if source.match("|"):
            yes_groups = info.used_groups
            info.used_groups = previous_groups
            no_branch = parse_sequence(source, info)
            info.used_groups |= yes_groups
        else:
            no_branch = Sequence()
    finally:
        source.ignore_space = saved_ignore
        info.scoped_flags = saved_scoped_flags

    source.expect(")")

    return Conditional(info, group, yes_branch, no_branch)

def parse_atomic(source, info):
    "Parses an atomic subpattern."
    saved_scoped_flags = info.scoped_flags
    saved_ignore = source.ignore_space
    try:
        subpattern = parse_pattern(source, info)
    finally:
        source.ignore_space = saved_ignore
        info.scoped_flags = saved_scoped_flags

    source.expect(")")

    return Atomic(subpattern)

def parse_common(source, info):
    "Parses a common groups branch."
    # Capture group numbers in different branches can reuse the group numbers.
    previous_groups = info.used_groups.copy()
    initial_group_count = info.group_count
    branches = [parse_sequence(source, info)]
    final_group_count = info.group_count
    all_groups = info.used_groups
    while source.match("|"):
        info.used_groups = previous_groups.copy()
        info.group_count = initial_group_count
        branches.append(parse_sequence(source, info))
        final_group_count = max(final_group_count, info.group_count)
        all_groups |= info.used_groups

    info.used_groups = all_groups
    info.group_count = final_group_count
    source.expect(")")

    if len(branches) == 1:
        return branches[0]
    return Branch(branches)

def parse_flags_subpattern(source, info):
    "Parses a flags subpattern."
    # It could be inline flags or a subpattern possibly with local flags.
    # Parse the flags.
    flags_on, flags_off = 0, 0
    try:
        while True:
            here = source.pos
            ch = source.get()
            flags_on |= REGEX_FLAGS[ch]
    except KeyError:
        pass

    if ch == "-":
        try:
            while True:
                here = source.pos
                ch = source.get()
                flags_off |= REGEX_FLAGS[ch]
        except KeyError:
            pass

        if not flags_off or (flags_off & GLOBAL_FLAGS):
            error("bad inline flags")

    # Separate the global and scoped flags.
    source.pos = here
    old_global_flags = info.global_flags
    info.global_flags |= flags_on & GLOBAL_FLAGS
    flags_on &= SCOPED_FLAGS
    flags_off &= SCOPED_FLAGS
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
            subpattern = parse_pattern(source, info)
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
                info.global_flags &= GLOBAL_FLAGS

            raise UnscopedFlagSet(info.global_flags)

        info.all_flags = info.global_flags | info.scoped_flags

        return None

def parse_name(source, allow_numeric=False):
    "Parses a name."
    name = []
    here = source.pos
    saved_ignore = source.ignore_space
    source.ignore_space = False
    try:
        ch = source.get()
        while ch in ALNUM or ch == "_":
            name.append(ch)
            here = source.pos
            ch = source.get()
    finally:
        source.ignore_space = saved_ignore

    source.pos = here
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

def is_octal(string):
    "Checks whether a string is octal."
    return all(ch in OCT_DIGITS for ch in string)

def is_decimal(string):
    "Checks whether a string is decimal."
    return all(ch in DIGITS for ch in string)

def is_hexadecimal(string):
    "Checks whether a string is hexadecimal."
    return all(ch in HEX_DIGITS for ch in string)

def parse_escape(source, info, in_set):
    "Parses an escape sequence."
    ch = source.get()
    if not ch:
        # A backslash at the end of the pattern.
        raise error("bad escape")
    if ch in HEX_ESCAPES:
        # A hexadecimal escape sequence.
        return parse_hex_escape(source, info, HEX_ESCAPES[ch])
    elif ch == "g" and not in_set:
        # A group reference.
        here = source.pos
        try:
            return parse_group_ref(source, info)
        except error:
            # Invalid as a group reference, so assume it's a literal.
            source.pos = here

        return char_literal(info, ord(ch))
    elif ch == "G" and not in_set:
        # A search anchor.
        return SearchAnchor()
    elif ch == "L" and not in_set:
        # A string set.
        return parse_string_set(source, info)
    elif ch == "N":
        # A named codepoint.
        return parse_named_char(source, info, in_set)
    elif ch in "pP":
        # A Unicode property.
        return parse_property(source, info, in_set, ch == "p")
    elif ch == "X" and not in_set:
        # A grapheme cluster.
        return Grapheme()
    elif ch in ALPHA:
        # An alphabetic escape sequence.
        # Positional escapes aren't allowed inside a character set.
        if not in_set:
            if info.all_flags & WORD:
                value = WORD_POSITION_ESCAPES.get(ch)
            else:
                value = POSITION_ESCAPES.get(ch)

            if value:
                return value

        value = CHARSET_ESCAPES.get(ch)
        if value:
            return value

        value = CHARACTER_ESCAPES.get(ch)
        if value:
            return Character(info, ord(value))

        return char_literal(info, ord(ch))
    elif ch in DIGITS:
        # A numeric escape sequence.
        return parse_numeric_escape(source, info, ch, in_set)
    else:
        # A literal.
        return char_literal(info, ord(ch))

def parse_numeric_escape(source, info, ch, in_set):
    "Parses a numeric escape sequence."
    if in_set or ch == "0":
        # Octal escape sequence, max 3 digits.
        return parse_octal_escape(source, info, [ch])

    # At least 1 digit, so either octal escape or group.
    digits = ch
    here = source.pos
    ch = source.get()
    if ch in DIGITS:
        # At least 2 digits, so either octal escape or group.
        digits += ch
        here = source.pos
        ch = source.get()
        if is_octal(digits) and ch in OCT_DIGITS:
            # 3 octal digits, so octal escape sequence.
            value = int(digits + ch, 8) & 0xFF
            return char_literal(info, value)

    # Group reference.
    source.pos = here
    if info.is_open_group(digits):
        raise error("can't refer to an open group")

    return ref_group(info, digits)

def parse_octal_escape(source, info, digits):
    "Parses an octal escape sequence."
    here = source.pos
    ch = source.get()
    while len(digits) < 3 and ch in OCT_DIGITS:
        digits.append(ch)
        here = source.pos
        ch = source.get()

    source.pos = here
    try:
        value = int("".join(digits), 8) & 0xFF
        return char_literal(info, value)
    except ValueError:
        raise error("bad escape")

def parse_hex_escape(source, info, expected_len):
    "Parses a hex escape sequence."
    digits = []
    for i in range(expected_len):
        ch = source.get()
        if ch not in HEX_DIGITS:
            raise error("bad hex escape")
        digits.append(ch)

    value = int("".join(digits), 16)
    return char_literal(info, value)

def parse_group_ref(source, info):
    "Parses a group reference."
    source.expect("<")
    name = parse_name(source, True)
    source.expect(">")
    if info.is_open_group(name):
        raise error("can't refer to an open group")

    return ref_group(info, name)

def parse_string_set(source, info):
    source.expect("<")
    name = parse_name(source, True)
    source.expect(">")
    if name is None or name not in info.kwargs:
        raise error("undefined named list")

    return string_set(info, name)

def parse_named_char(source, info, in_set):
    "Parses a named character."
    here = source.pos
    ch = source.get()
    if ch == "{":
        name = []
        ch = source.get()
        while ch in ALPHA or ch == " ":
            name.append(ch)
            ch = source.get()

        if ch == "}":
            try:
                value = unicodedata.lookup("".join(name))
                return char_literal(info, ord(value))
            except KeyError:
                raise error("undefined character name")

    source.pos = here
    return char_literal(info, ord("N"))

def parse_property(source, info, in_set, positive):
    "Parses a Unicode property."
    here = source.pos
    if source.match("{"):
        negate = source.match("^")
        prop_name, name = parse_property_name(source)
        if source.match("}"):
            # It's correctly delimited.
            return lookup_property(prop_name, name, positive != negate)

    # Not a property, so treat as a literal "p" or "P".
    source.pos = here
    ch = "p" if positive else "P"
    return char_literal(info, ord(ch))

def parse_property_name(source):
    "Parses a property name, which may be qualified."
    name = []
    here = source.pos
    ch = source.get()
    while ch and (ch in ALNUM or ch in " &_-."):
        name.append(ch)
        here = source.pos
        ch = source.get()

    here2 = here
    if ch and ch in ":=":
        prop_name = name
        name = []
        here = source.pos
        ch = source.get()
        while ch and (ch in ALNUM or ch in " &_-."):
            name.append(ch)
            here = source.pos
            ch = source.get()

        if all(ch == " " for ch in name):
            # No name after the ":" or "=", so assume it's an unqualified name.
            prop_name, name = None, prop_name
            here = here2
    else:
        prop_name = None

    source.pos = here
    return prop_name, name

def parse_set(source, info):
    "Parses a character set."
    saved_ignore = source.ignore_space
    source.ignore_space = False
    # Negative set?
    negate = source.match("^")
    try:
        item = parse_set_union(source, info)
        if negate:
            item = SetUnion([item], positive=False)
    finally:
        source.ignore_space = saved_ignore

    return item

def parse_set_union(source, info):
    "Parses a set union ([x||y])."
    items = [parse_set_symm_diff(source, info)]
    while source.match("||"):
        items.append(parse_set_symm_diff(source, info))

    if not source.match("]"):
        raise error("missing ]")

    if len(items) == 1 and not isinstance(items[0], Range):
        return items[0]
    return SetUnion(items)

def parse_set_symm_diff(source, info):
    "Parses a set symmetric difference ([x~~y])."
    items = [parse_set_inter(source, info)]
    while source.match("~~"):
        items.append(parse_set_inter(source, info))

    if len(items) == 1:
        return items[0]
    return SetSymDiff(items)

def parse_set_inter(source, info):
    "Parses a set intersection ([x&&y])."
    items = [parse_set_diff(source, info)]
    while source.match("&&"):
        items.append(parse_set_diff(source, info))

    if len(items) == 1:
        return items[0]
    return SetInter(items)

def parse_set_diff(source, info):
    "Parses a set difference ([x--y])."
    items = [parse_set_imp_union(source, info)]
    while source.match("--"):
        items.append(parse_set_imp_union(source, info))

    if len(items) == 1:
        return items[0]
    return SetDiff(items)

def parse_set_imp_union(source, info):
    "Parses a set implicit union ([xy])."
    items = [parse_set_member(source, info)]
    while True:
        here = source.pos
        if source.match("]") or any(source.match(op) for op in SET_OPS):
            break
        items.append(parse_set_member(source, info))

    source.pos = here

    if len(items) == 1:
        return items[0]
    return SetUnion(items)

def parse_set_member(source, info):
    "Parses a member in a character set."
    # Parse a character or property.
    start = parse_set_item(source, info)
    if not isinstance(start, Character) or not source.match("-"):
        # Not the start of a range.
        return start

    # It looks like the start of a range of characters.
    here = source.pos
    if source.match("]"):
        # We've reached the end of the set, so return both the character and
        # hyphen.
        source.pos = here
        return SetUnion([start, Character(info, ord("-"))])

    # Parse a character or property.
    end = parse_set_item(source, info)
    if not isinstance(end, Character):
        # It's not a range, so return the character, hyphen and property.
        return SetUnion([start, Character(info, ord("-")), end])

    # It _is_ a range.
    if start.value > end.value:
        raise error("bad character range")

    if info.all_flags & IGNORECASE:
        return RangeIgn(info, start.value, end.value)

    return Range(info, start.value, end.value)

def parse_set_item(source, info):
    "Parses an item in a character set."
    if source.match("\\"):
        return parse_escape(source, info, True)

    here = source.pos
    if source.match("[:"):
        # Looks like a POSIX character class.
        try:
            return parse_posix_class(source, info)
        except ParseError:
            # Not a POSIX character class.
            source.pos = here

    ch = source.get()
    if ch == "[" and info.nested_sets:
        # Looks like the start of a nested set.
        here = source.pos
        try:
            return parse_set_union(source, info)
        except error:
            # Failed to parse a nested set, so treat it as a literal.
            source.pos = here

    if not ch:
        raise error("bad set", True)

    return char_literal(info, ord(ch))

def parse_posix_class(source, info):
    "Parses a POSIX character class."
    negate = source.match("^")
    prop_name, name = parse_property_name(source)
    if not source.match(":]"):
        raise ParseError()

    return lookup_property(prop_name, name, not negate)

def float_to_rational(flt):
    "Converts a float to a rational pair."
    int_part = int(flt)
    error = flt - int_part
    if abs(error) < 0.0001:
        return int_part, 1

    den, num = float_to_rational(1.0 / error)

    return int_part * den + num, den

def numeric_to_rational(numeric):
    "Converts a numeric string to a rational string, if possible."
    if numeric[0] == "-":
        sign, numeric = numeric[0], numeric[1 : ]
    else:
        sign = ""

    parts = numeric.split("/")
    if len(parts) == 2:
        num, den = float_to_rational(float(parts[0]) / float(parts[1]))
    elif len(parts) == 1:
        num, den = float_to_rational(float(parts[0]))
    else:
        raise ValueError()

    format = "{}{}" if den == 1 else "{}{}/{}"

    return format.format(sign, num, den)

def standardise_name(name):
    "Standardises a property or value name."
    try:
        return numeric_to_rational("".join(name))
    except (ValueError, ZeroDivisionError):
        return "".join(ch for ch in name if ch not in "_- ").upper()

def lookup_property(property, value, positive):
    "Looks up a property."
    # Normalise the names (which may still be lists).
    property = standardise_name(property) if property else None
    value = standardise_name(value)
    if property:
        # Both the property and the value are provided.
        prop = PROPERTIES.get(property)
        if not prop:
            raise error("unknown property")

        prop_id, value_dict = prop
        val_id = value_dict.get(value)
        if val_id is None:
            raise error("unknown property value")

        return Property((prop_id << 16) | val_id, positive)

    # Only the value is provided.
    # It might be the name of a GC, script or block value.
    for property in ("GC", "SCRIPT", "BLOCK"):
        prop_id, value_dict = PROPERTIES.get(property)
        val_id = value_dict.get(value)
        if val_id is not None:
            return Property((prop_id << 16) | val_id, positive)

    # It might be the name of a property.
    prop = PROPERTIES.get(value)
    if prop:
        prop_id, value_dict = prop
        return Property(prop_id << 16, not positive)

    # It might be the name of a binary property.
    if value.startswith("IS"):
        prop = PROPERTIES.get(value[2 : ])
        if prop:
            prop_id, value_dict = prop
            return Property(prop_id << 16, not positive)

    # It might be the prefixed name of a script or block.
    for prefix, property in (("IS", "SCRIPT"), ("IN", "BLOCK")):
        if value.startswith(prefix):
            prop_id, value_dict = PROPERTIES.get(property)
            val_id = value_dict.get(value[2 : ])
            if val_id is not None:
                return Property((prop_id << 16) | val_id, positive)

    # Unknown property.
    raise error("unknown property")

def compile_repl_escape(source, pattern):
    "Compiles a replacement template escape sequence."
    ch = source.get()
    if ch in ALPHA:
        # An alphabetic escape sequence.
        value = CHARACTER_ESCAPES.get(ch)
        if value:
            return False, [ord(value)]

        if ch == "g":
            # A group preference.
            return True, [compile_repl_group(source, pattern)]

        return False, [ord("\\"), ord(ch)]
    if ch == "0":
        # An octal escape sequence.
        digits = ch
        while len(digits) < 3:
            here = source.pos
            ch = source.get()
            if ch not in OCT_DIGITS:
                source.pos = here
                break
            digits += ch

        return False, [int(digits, 8) & 0xFF]
    if ch in DIGITS:
        # Either an octal escape sequence (3 digits) or a group reference (max
        # 2 digits).
        digits = ch
        here = source.pos
        ch = source.get()
        if ch in DIGITS:
            digits += ch
            here = source.pos
            ch = source.get()
            if ch and is_octal(digits + ch):
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

def compile_repl_group(source, pattern):
    "Compiles a replacement template group reference."
    source.expect("<")
    name = parse_name(source, True)
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

INDENT = "  "
POSITIVE_OP = 0x1
ZEROWIDTH_OP = 0x2
FUZZY_OP = 0x4
REVERSE_OP = 0x8

# Common base for all nodes.
class RegexBase(object):
    def __init__(self):
        self._key = self.__class__

    def fix_groups(self):
        pass

    def optimise(self, info):
        return self

    def pack_characters(self, info):
        return self

    def remove_captures(self):
        return self

    def is_atomic(self):
        return True

    def contains_group(self):
        return False

    def get_first(self):
        return self

    def drop_first(self):
        return Sequence()

    def get_last(self):
        return self

    def drop_last(self):
        return Sequence()

    def can_repeat(self):
        return True

    def firstset(self):
        raise FirstSetError()

    def has_simple_start(self):
        return False

    def __hash__(self):
        return hash(self._key)

    def __eq__(self, other):
        return type(self) is type(other) and self._key == other._key

    def __ne__(self, other):
        return not self.__eq__(other)

    def __nonzero__(self):
        return True

# Base for zero-width nodes.
class ZeroWidthBase(RegexBase):
    _pos_text = {False: "NON-MATCH", True: "MATCH"}

    def __init__(self, positive=True):
        RegexBase.__init__(self)
        self.positive = bool(positive)
        self._key = self.__class__, self.positive

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if fuzzy:
            flags |= FUZZY_OP
        if reverse:
            flags |= REVERSE_OP
        return [(self._opcode, flags)]

    def dump(self, indent=0, reverse=False):
        print "%s%s %s" % (INDENT * indent, self._op_name,
          self._pos_text[self.positive])

    def firstset(self):
        return set([None])

    def can_repeat(self):
        return False

# Base for 'structure' nodes, ie those containing subpatterns.
class StructureBase(RegexBase):
    def get_first(self):
        return None

    def drop_first(self):
        raise error("internal error")

    def get_last(self):
        return None

    def drop_last(self):
        raise error("internal error")

class Any(RegexBase):
    _opcode = {False: OP.ANY, True: OP.ANY_REV}
    _op_name = {False: "ANY", True: "ANY_REV"}

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[reverse], flags)]

    def dump(self, indent=0, reverse=False):
        print "%s%s" % (INDENT * indent, self._op_name[reverse])

    def has_simple_start(self):
        return True

class AnyAll(Any):
    _opcode = {False: OP.ANY_ALL, True: OP.ANY_ALL_REV}
    _op_name = {False: "ANY_ALL", True: "ANY_ALL_REV"}

class AnyU(Any):
    _opcode = {False: OP.ANY_U, True: OP.ANY_U_REV}
    _op_name = {False: "ANY_U", True: "ANY_U_REV"}

class Atomic(StructureBase):
    def __init__(self, subpattern):
        StructureBase.__init__(self)
        self.subpattern = subpattern

    def fix_groups(self):
        self.subpattern.fix_groups()

    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)

        # Move any atomic prefix or suffix out of the subpattern.
        prefix, subpattern = Atomic._split_atomic_prefix(subpattern)
        suffix, subpattern = Atomic._split_atomic_suffix(subpattern)

        if subpattern:
            sequence = prefix + [Atomic(subpattern)] + suffix
        else:
            sequence = prefix + suffix

        if len(sequence) == 1:
            return sequence[0]
        return Sequence(sequence)

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def contains_group(self):
        return self.subpattern.contains_group()

    def compile(self, reverse=False, fuzzy=False):
        return [(OP.ATOMIC, )] + self.subpattern.compile(reverse, fuzzy) + \
          [(OP.END, )]

    def dump(self, indent=0, reverse=False):
        print "%sATOMIC" % (INDENT * indent)
        self.subpattern.dump(indent + 1, reverse)

    def firstset(self):
        return self.subpattern.firstset()

    def has_simple_start(self):
        return self.subpattern.has_simple_start()

    def __eq__(self, other):
        return type(self) is type(other) and self.subpattern == \
          other.subpattern

    def __nonzero__(self):
        return bool(self.subpattern)

    @staticmethod
    def _split_atomic_prefix(subpattern):
        # Leading atomic items can be moved out of an atomic subpattern.
        prefix = []
        while True:
            item = subpattern.get_first()
            if not item or not item.is_atomic():
                break
            prefix.append(item)
            subpattern = subpattern.drop_first()

        return prefix, subpattern

    @staticmethod
    def _split_atomic_suffix(subpattern):
        # Trailing atomic items can be moved out of an atomic subpattern.
        suffix = []
        while True:
            item = subpattern.get_last()
            if not item or not item.is_atomic():
                break
            suffix.append(item)
            subpattern = subpattern.drop_last()

        suffix.reverse()

        return suffix, subpattern

class Boundary(ZeroWidthBase):
    _opcode = OP.BOUNDARY
    _op_name = "BOUNDARY"

class Branch(StructureBase):
    def __init__(self, branches):
        StructureBase.__init__(self)
        self.branches = branches

    def fix_groups(self):
        for b in self.branches:
            b.fix_groups()

    def optimise(self, info):
        # Flatten branches within branches.
        branches = Branch._flatten_branches(info, self.branches)

        # Move any common prefix or suffix out of the branches.
        prefix, branches = Branch._split_common_prefix(branches)
        suffix, branches = Branch._split_common_suffix(branches)

        # Merge branches starting with the same character. (If a character
        # prefix doesn't match in one branch, it won't match in any of the
        # others starting with that same character.)
        branches = Branch._merge_common_prefixes(info, branches)

        # Try to reduce adjacent single-character branches to sets.
        branches = Branch._reduce_to_set(info, branches)

        if len(branches) > 1:
            sequence = prefix + [Branch(branches)] + suffix
        else:
            sequence = prefix + branches + suffix

        if len(sequence) == 1:
            return sequence[0]
        return Sequence(sequence)

    def pack_characters(self, info):
        self.branches = [b.pack_characters(info) for b in self.branches]
        return self

    def is_atomic(self):
        return all(b.is_atomic() for b in self.branches)

    def contains_group(self):
        return any(b.contains_group() for b in self.branches)

    def compile(self, reverse=False, fuzzy=False):
        code = [(OP.BRANCH, )]
        for b in self.branches:
            code.extend(b.compile(reverse, fuzzy))
            code.append((OP.NEXT, ))

        code[-1] = (OP.END, )

        return code

    def remove_captures(self):
        self.branches = [b.remove_captures() for b in self.branches]
        return self

    def dump(self, indent=0, reverse=False):
        print "%sBRANCH" % (INDENT * indent)
        self.branches[0].dump(indent + 1, reverse)
        for b in self.branches[1 : ]:
            print "%sOR" % (INDENT * indent)
            b.dump(indent + 1, reverse)

    def firstset(self):
        fs = set()
        for b in self.branches:
            fs |= b.firstset()

        return fs or set([None])

    def __eq__(self, other):
        return type(self) is type(other) and self.branches == other.branches

    def __nonzero__(self):
        return any(self.branches)

    @staticmethod
    def _flatten_branches(info, branches):
        # Flatten the branches so that there aren't branches of branches.
        new_branches = []
        for b in branches:
            b = b.optimise(info)
            if isinstance(b, Branch):
                new_branches.extend(b.branches)
            else:
                new_branches.append(b)

        return new_branches

    @staticmethod
    def _split_common_prefix(branches):
        # Common leading items can be moved out of the branches.
        prefix = []
        while True:
            item = branches[0].get_first()
            if not item:
                break
            if any(b.get_first() != item for b in branches[1 : ]):
                break
            prefix.append(item)
            branches = [b.drop_first() for b in branches]

        return prefix, branches

    @staticmethod
    def _split_common_suffix(branches):
        # Common trailing items can be moved out of the branches.
        suffix = []
        while True:
            item = branches[0].get_last()
            if not item:
                break
            if any(b.get_last() != item for b in branches[1 : ]):
                break
            suffix.append(item)
            branches = [b.drop_last() for b in branches]

        suffix.reverse()

        return suffix, branches

    @staticmethod
    def _merge_common_prefixes(info, branches):
        # Branches with the same character prefix can be grouped together if
        # they are separated only by other branches with a character prefix.
        char_type = None
        char_prefixes = defaultdict(list)
        order = {}
        new_branches = []
        for b in branches:
            first = b.get_first()
            if isinstance(first, Character) and first.positive:
                if type(first) is not char_type:
                    if char_prefixes:
                        Branch._flush_char_prefix(info, char_type,
                          char_prefixes, order, new_branches)
                        char_prefixes.clear()
                        order.clear()

                    char_type = type(first)

                char_prefixes[first.value].append(b)
                order.setdefault(first.value, len(order))
            else:
                if char_prefixes:
                    Branch._flush_char_prefix(info, char_type, char_prefixes,
                      order, new_branches)
                    char_prefixes.clear()
                    order.clear()

                char_type = None
                new_branches.append(b)

        if char_prefixes:
            Branch._flush_char_prefix(info, char_type, char_prefixes, order,
              new_branches)

        return new_branches

    @staticmethod
    def _reduce_to_set(info, branches):
        # Can the branches be reduced to a set?
        new_branches = []
        members = set()
        for b in branches:
            if isinstance(b, Character) and b.positive or isinstance(b,
              Property):
                members.add(b)
            elif isinstance(b, SetUnion) and b.positive:
                for m in b.items:
                    if isinstance(m, (Character, Property)):
                        members.add(m)
                    else:
                        Branch._flush_set_members(info, members, new_branches)
                        members.clear()
                        new_branches.append(b)
            else:
                Branch._flush_set_members(info, members, new_branches)
                members.clear()
                new_branches.append(b)

        Branch._flush_set_members(info, members, new_branches)

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
                for b in branches:
                    b = b.drop_first()
                    if b:
                        subbranches.append(b)
                    elif not optional:
                        subbranches.append(Sequence())
                        optional = True

                sequence = [char_type(info, value)]
                if len(subbranches) > 1:
                    sequence.append(Branch(subbranches))
                else:
                    sequence.extend(subbranches)
                new_branches.append(Sequence(sequence).optimise(info))

    @staticmethod
    def _flush_set_members(info, members, new_branches):
        if members:
            new_branches.append(SetUnion(list(members)).optimise(info))

class Character(RegexBase):
    _opcode = {False: OP.CHARACTER, True: OP.CHARACTER_REV}
    _op_name = {False: "CHARACTER", True: "CHARACTER_REV"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}

    def __init__(self, info, value, positive=True, zerowidth=False):
        RegexBase.__init__(self)
        self.info, self.value, self.positive, self.zerowidth = info, value, \
          bool(positive), bool(zerowidth)
        self._key = self.__class__, self.value, self.positive, self.zerowidth

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[reverse], flags, self.value)]

    def dump(self, indent=0, reverse=False):
        print "%s%s %s %s" % (INDENT * indent, self._op_name[reverse],
          self._pos_text[self.positive], self.value)

    def firstset(self):
        return set([self])

    def has_simple_start(self):
        return True

    def change_flags(self, positive=True, zerowidth=False):
        return type(self)(self.info, self.value, self.positive == positive,
          self.zerowidth or zerowidth)

class CharacterIgn(Character):
    _opcode = {False: OP.CHARACTER_IGN, True: OP.CHARACTER_IGN_REV}
    _op_name = {False: "CHARACTER_IGN", True: "CHARACTER_IGN_REV"}

    def optimise(self, info):
        # Case-sensitive matches are faster, so convert to a case-sensitive
        # instance if the character is case-insensitive.
        if len(all_cases(info, self.value)) == 1:
            return Character(info, self.value, self.positive, self.zerowidth)

        return self

class Conditional(StructureBase):
    def __new__(cls, info, group, yes_item, no_item):
        if not yes_item and not no_item:
            return Sequence()

        return StructureBase.__new__(cls)

    def __init__(self, info, group, yes_item, no_item):
        StructureBase.__init__(self)
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
        self.no_item.fix_groups()

    def optimise(self, info):
        yes_item = self.yes_item.optimise(info)
        no_item = self.no_item.optimise(info)

        return Conditional(info, self.group, yes_item, no_item)

    def pack_characters(self, info):
        self.yes_item = self.yes_item.pack_characters(info)
        self.no_item = self.no_item.pack_characters(info)
        return self

    def is_atomic(self):
        return self.yes_item.is_atomic() and self.no_item.is_atomic()

    def contains_group(self):
        return self.yes_item.contains_group() or self.no_item.contains_group()

    def compile(self, reverse=False, fuzzy=False):
        code = [(OP.GROUP_EXISTS, self.group)]
        code.extend(self.yes_item.compile(reverse, fuzzy))
        add_code = self.no_item.compile(reverse, fuzzy)
        if add_code:
            code.append((OP.NEXT, ))
            code.extend(add_code)

        code.append((OP.END, ))

        return code

    def remove_captures(self):
        self.yes_item = self.yes_item.remove_captures()
        self.no_item = self.no_item.remove_captures()

    def dump(self, indent=0, reverse=False):
        print "%sGROUP_EXISTS %s" % (INDENT * indent, self.group)
        self.yes_item.dump(indent + 1, reverse)
        if self.no_item:
            print "%sOR" % (INDENT * indent)
            self.no_item.dump(indent + 1, reverse)

    def firstset(self):
        return self.yes_item.firstset() | self.no_item.firstset()

    def __eq__(self, other):
        return type(self) is type(other) and (self.group, self.yes_item,
          self.no_item) == (other.group, other.yes_item, other.no_item)

    def __nonzero__(self):
        return bool(self.yes_item or self.no_item)

class DefaultBoundary(ZeroWidthBase):
    _opcode = OP.DEFAULT_BOUNDARY
    _op_name = "DEFAULT_BOUNDARY"

class EndOfLine(ZeroWidthBase):
    _opcode = OP.END_OF_LINE
    _op_name = "END_OF_LINE"

class EndOfLineU(EndOfLine):
    _opcode = OP.END_OF_LINE_U
    _op_name = "END_OF_LINE_U"

class EndOfString(ZeroWidthBase):
    _opcode = OP.END_OF_STRING
    _op_name = "END_OF_STRING"

class EndOfStringLine(ZeroWidthBase):
    _opcode = OP.END_OF_STRING_LINE
    _op_name = "END_OF_STRING_LINE"

class EndOfStringLineU(EndOfStringLine):
    _opcode = OP.END_OF_STRING_LINE_U
    _op_name = "END_OF_STRING_LINE_U"

class Fuzzy(StructureBase):
    def __init__(self, subpattern, constraints=None):
        StructureBase.__init__(self)
        if constraints is None:
            constraints = {}
        self.subpattern = subpattern
        self.constraints = constraints

        # If an error type is mentioned in the cost equation, then its maximum
        # defaults to unlimited.
        if "cost" in constraints:
            for e in "dis":
                if e in constraints["cost"]:
                    constraints.setdefault(e, None)

        # If any error type is mentioned then the maxima default to 0,
        # they default to unlimited.
        if set(constraints) & set("dis"):
            for e in "dis":
                constraints.setdefault(e, 0)
        else:
            for e in "dis":
                constraints.setdefault(e, None)

        # The maximum of the generic error type defaults to unlimited.
        constraints.setdefault("e", None)

        # The cost equation defaults to equal costs.
        # Also, the cost of any error type not mentioned in the cost equation
        # defaults to 0.
        if "cost" in constraints:
            for e in "dis":
                constraints["cost"].setdefault(e, 0)
        else:
            constraints["cost"] = {"d": 1, "i": 1, "s": 1, "max":
              constraints["e"]}

    def fix_groups(self):
        self.subpattern.fix_groups()

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def contains_group(self):
        return self.subpattern.contains_group()

    def compile(self, reverse=False, fuzzy=False):
        # The individual maxima.
        arguments = []
        for e in "dise":
            v = self.constraints[e]
            arguments.append(UNLIMITED if v is None else v)

        # The coeffs of the cost equation.
        for e in "dis":
            arguments.append(self.constraints["cost"][e])

        # The maximum of the cost equation.
        v = self.constraints["cost"]["max"]
        arguments.append(UNLIMITED if v is None else v)

        flags = 0
        if reverse:
            flags |= REVERSE_OP

        return [(OP.FUZZY, flags) + tuple(arguments)] + \
          self.subpattern.compile(reverse, True) + [(OP.END,)]

    def dump(self, indent=0, reverse=False):
        print "%sFUZZY" % (INDENT * indent)
        self.subpattern.dump(indent + 1, reverse)

    def has_simple_start(self):
        return self.subpattern.has_simple_start()

    def __eq__(self, other):
        return type(self) is type(other) and self.subpattern == \
          other.subpattern

    def __nonzero__(self):
        return bool(self.subpattern)

class Grapheme(RegexBase):
    _op_name = {False: "GRAPHEME", True: "GRAPHEME_REV"}

    def compile(self, reverse=False, fuzzy=False):
        # Match at least 1 character until a grapheme boundary is reached.
        # Note that this is the same whether matching forwards or backwards.
        character_matcher = LazyRepeat(AnyAll(), 1, None).compile(reverse,
          fuzzy)
        boundary_matcher = [(OP.GRAPHEME_BOUNDARY, 1)]

        return character_matcher + boundary_matcher

    def dump(self, indent=0, reverse=False):
        print "%s%s" % (INDENT * indent, self._op_name[reverse])

class GreedyRepeat(StructureBase):
    _opcode = OP.GREEDY_REPEAT
    _op_name = "GREEDY_REPEAT"

    def __init__(self, subpattern, min_count, max_count):
        StructureBase.__init__(self)
        self.subpattern, self.min_count, self.max_count = subpattern, \
          min_count, max_count

    def fix_groups(self):
        self.subpattern.fix_groups()

    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)

        return type(self)(subpattern, self.min_count, self.max_count)

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def is_atomic(self):
        return self.min_count == self.max_count and self.subpattern.is_atomic()

    def contains_group(self):
        return self.subpattern.contains_group()

    def compile(self, reverse=False, fuzzy=False):
        repeat = [self._opcode, self.min_count]
        if self.max_count is None:
            repeat.append(UNLIMITED)
        else:
            repeat.append(self.max_count)

        return [tuple(repeat)] + self.subpattern.compile(reverse, fuzzy) + \
          [(OP.END, )]

    def remove_captures(self):
        self.subpattern = self.subpattern.remove_captures()
        return self

    def dump(self, indent=0, reverse=False):
        if self.max_count is None:
            print "%s%s %s INF" % (INDENT * indent, self._op_name,
              self.min_count)
        else:
            print "%s%s %s %s" % (INDENT * indent, self._op_name,
              self.min_count, self.max_count)

        self.subpattern.dump(indent + 1, reverse)

    def firstset(self):
        fs = self.subpattern.firstset()
        if self.min_count == 0:
            fs.add(None)

        return fs

    def __eq__(self, other):
        return type(self) is type(other) and (self.subpattern, self.min_count,
          self.max_count) == (other.subpattern, other.min_count,
          other.max_count)

    def __nonzero__(self):
        return bool(self.subpattern)

class Group(StructureBase):
    def __init__(self, info, group, subpattern):
        StructureBase.__init__(self)
        self.info, self.group, self.subpattern = info, group, subpattern

    def fix_groups(self):
        self.subpattern.fix_groups()

    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)

        return Group(self.info, self.group, subpattern)

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def is_atomic(self):
        return self.subpattern.is_atomic()

    def contains_group(self):
        return True

    def compile(self, reverse=False, fuzzy=False):
        return [(OP.GROUP, self.group)] + self.subpattern.compile(reverse,
          fuzzy) + [(OP.END, )]

    def remove_captures(self):
        return self.subpattern.remove_captures()

    def dump(self, indent=0, reverse=False):
        print "%sGROUP %s" % (INDENT * indent, self.group)
        self.subpattern.dump(indent + 1, reverse)

    def firstset(self):
        return self.subpattern.firstset()

    def has_simple_start(self):
        return self.subpattern.has_simple_start()

    def __eq__(self, other):
        return type(self) is type(other) and (self.group, self.subpattern) == \
          (other.group, other.subpattern)

    def __nonzero__(self):
        return True

class LazyRepeat(GreedyRepeat):
    _opcode = OP.LAZY_REPEAT
    _op_name = "LAZY_REPEAT"

class LookAround(StructureBase):
    _dir_text = {False: "AHEAD", True: "BEHIND"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}

    def __new__(cls, behind, positive, subpattern):
        if positive and not subpattern:
            return subpattern

        return StructureBase.__new__(cls)

    def __init__(self, behind, positive, subpattern):
        StructureBase.__init__(self)
        self.behind, self.positive, self.subpattern = bool(behind), \
          bool(positive), subpattern

    def fix_groups(self):
        self.subpattern.fix_groups()

    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)

        return LookAround(self.behind, self.positive, subpattern)

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def is_atomic(self):
        return self.subpattern.is_atomic()

    def contains_group(self):
        return self.subpattern.contains_group()

    def compile(self, reverse=False, fuzzy=False):
        return [(OP.LOOKAROUND, int(self.positive), int(not self.behind))] + \
          self.subpattern.compile(self.behind) + [(OP.END, )]

    def dump(self, indent=0, reverse=False):
        print "%sLOOKAROUND %s %s" % (INDENT * indent,
          self._dir_text[self.behind], self._pos_text[self.positive])
        self.subpattern.dump(indent + 1, self.behind)

    def __eq__(self, other):
        return type(self) is type(other) and (self.behind, self.positive,
          self.subpattern) == (other.behind, other.positive, other.subpattern)

    def __nonzero__(self):
        return bool(self.subpattern)

    def can_repeat(self):
        return False

class Property(RegexBase):
    _opcode = {False: OP.PROPERTY, True: OP.PROPERTY_REV}
    _op_name = {False: "PROPERTY", True: "PROPERTY_REV"}
    _pos_text = {False: "NON-MATCH", True: "MATCH"}

    def __init__(self, value, positive=True, zerowidth=False):
        RegexBase.__init__(self)
        self.value, self.positive, self.zerowidth = value, bool(positive), \
          bool(zerowidth)
        self._key = self.__class__, self.value, self.positive, self.zerowidth

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[reverse], flags, self.value)]

    def dump(self, indent=0, reverse=False):
        print "%s%s %s %s=%s" % (INDENT * indent, self._op_name[reverse],
          self._pos_text[self.positive], self.value >> 16, self.value &
          0xFFFF)

    def firstset(self):
        return set([self])

    def has_simple_start(self):
        return True

    def change_flags(self, positive=True, zerowidth=False):
        return type(self)(self.value, self.positive == positive, self.zerowidth
          or zerowidth)

class Range(RegexBase):
    _op_name = "RANGE"

    def __new__(cls, info, lower, upper):
        if lower == upper:
            return Character(info, lower)

        return RegexBase.__new__(cls)

    def __init__(self, info, lower, upper):
        RegexBase.__init__(self)
        self.info, self.lower, self.upper = info, lower, upper
        self._key = self.__class__, self.lower, self.upper

    def dump(self, indent=0, reverse=False):
        print "%s%s %s %s" % (INDENT * indent, self._op_name, self.lower,
          self.upper)

    def firstset(self):
        return set([self])

    def has_simple_start(self):
        return True

class RangeIgn(Range):
    _op_name = "RANGE_IGN"

    def __new__(cls, info, lower, upper):
        if lower == upper:
            return CharacterIgn(info, lower)

        return Range.__new__(cls, info, lower, upper)

class RefGroup(RegexBase):
    _opcode = {False: OP.REF_GROUP, True: OP.REF_GROUP_REV}
    _op_name = {False: "REF_GROUP", True: "REF_GROUP_REV"}

    def __init__(self, info, group):
        RegexBase.__init__(self)
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

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[reverse], flags, self.group)]

    def remove_captures(self):
        raise error("group reference not allowed")

    def dump(self, indent=0, reverse=False):
        print "%s%s %s" % (INDENT * indent, self._op_name[reverse], self.group)

class RefGroupIgn(RefGroup):
    _opcode = {False: OP.REF_GROUP_IGN, True: OP.REF_GROUP_IGN_REV}
    _op_name = {False: "REF_GROUP_IGN", True: "REF_GROUP_IGN_REV"}

class SearchAnchor(ZeroWidthBase):
    _opcode = OP.SEARCH_ANCHOR
    _op_name = "SEARCH_ANCHOR"

class Info(object):
    "Info about the regular expression."
    OPEN = "OPEN"
    CLOSED = "CLOSED"

    def __init__(self, flags=0, char_type=None, nested_sets=True, kwargs={}):
        self.global_flags = flags & GLOBAL_FLAGS
        self.scoped_flags = flags & SCOPED_FLAGS
        self.all_flags = self.global_flags | self.scoped_flags
        if not (self.global_flags & NEW):
            self.global_flags = self.all_flags

        self.kwargs = kwargs

        self.group_count = 0
        self.group_index = {}
        self.group_name = {}
        self.used_groups = set()
        self.group_state = {}
        self.char_type = char_type
        self.named_lists = {}
        self.nested_sets = nested_sets

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

class Sequence(StructureBase):
    def __init__(self, sequence=None):
        StructureBase.__init__(self)
        if sequence is None:
            sequence = []

        self.sequence = sequence

    def fix_groups(self):
        for s in self.sequence:
            s.fix_groups()

    def optimise(self, info):
        # Flatten the sequences.
        sequence = []
        for s in self.sequence:
            s = s.optimise(info)
            if isinstance(s, Sequence):
                sequence.extend(s.sequence)
            else:
                sequence.append(s)

        if len(sequence) == 1:
            return sequence[0]
        return Sequence(sequence)

    def pack_characters(self, info):
        "Packs sequences of characters into strings."
        sequence = []
        char_type, characters = Character, []
        for s in self.sequence:
            if type(s) is char_type and s.positive:
                characters.append(s.value)
            else:
                if characters:
                    Sequence._flush_characters(info, char_type, characters,
                      sequence)
                    characters = []

                if type(s) in ALL_CHAR_TYPES and s.positive:
                    char_type = type(s)
                    characters.append(s.value)
                else:
                    sequence.append(s.pack_characters(info))

        if characters:
            Sequence._flush_characters(info, char_type, characters, sequence)

        if len(sequence) == 1:
            return sequence[0]
        return Sequence(sequence)

    def is_atomic(self):
        return all(s.is_atomic() for s in self.sequence)

    def contains_group(self):
        return any(s.contains_group() for s in self.sequence)

    def get_first(self):
        if self.sequence:
            return self.sequence[0]

        return None

    def drop_first(self):
        if len(self.sequence) == 2:
            return self.sequence[1]

        return Sequence(self.sequence[1 : ])

    def get_last(self):
        if self.sequence:
            return self.sequence[-1]

        return None

    def drop_last(self):
        if len(self.sequence) == 2:
            return self.sequence[-2]

        return Sequence(self.sequence[ : -1])

    def compile(self, reverse=False, fuzzy=False):
        if reverse:
            seq = list(reversed(self.sequence))
        else:
            seq = self.sequence

        code = []
        for s in seq:
            code.extend(s.compile(reverse, fuzzy))

        return code

    def remove_captures(self):
        self.sequence = [s.remove_captures() for s in self.sequence]
        return self

    def dump(self, indent=0, reverse=False):
        for s in self.sequence:
            s.dump(indent, reverse)

    def firstset(self):
        fs = set()
        for s in self.sequence:
            fs |= s.firstset()
            if None not in fs:
                return fs
            fs.discard(None)

        return fs | set([None])

    def has_simple_start(self):
        return self.sequence and self.sequence[0].has_simple_start()

    def __eq__(self, other):
        return type(self) is type(other) and self.sequence == other.sequence

    def __nonzero__(self):
        return any(self.sequence)

    @staticmethod
    def _flush_characters(info, char_type, characters, sequence):
        if not characters:
            return

        if len(characters) == 1:
            sequence.append(char_type(info, characters[0]))
        else:
            sequence.append(STRING_CLASSES[char_type](info, characters))

class SetBase(RegexBase):
    _pos_text = {False: "NON-MATCH", True: "MATCH"}
    _big_bitset_opcode = {False: OP.BIG_BITSET, True: OP.BIG_BITSET_REV}
    _small_bitset_opcode = {False: OP.SMALL_BITSET, True: OP.SMALL_BITSET_REV}

    def __init__(self, items, positive=True, zerowidth=False):
        RegexBase.__init__(self)
        items = tuple(items)
        self.items, self.positive, self.zerowidth = items, positive, zerowidth
        self._key = self.__class__, self.items, self.positive, self.zerowidth

    def dump(self, indent=0, reverse=False):
        print "%s%s %s %s" % (INDENT * indent, self._op_name[reverse],
          self._pos_text[self.positive])
        for i in self.items:
            i.dump(indent + 1)

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP
        code = [(self._opcode[reverse], flags)]
        for m in self.items:
            if isinstance(m, Range):
                code.extend(SetUnion([m]).compile())
            else:
                code.extend(m.compile())

        code.append((OP.END, ))

        return code

    def change_flags(self, positive=True, zerowidth=False):
        return type(self)(self.items, self.positive == positive, self.zerowidth
          or zerowidth)

    def _rebuild(self, items):
        return type(self)(items, self.positive, self.zerowidth)

    BITS_PER_INDEX = 16
    INDEXES_PER_CODE = BITS_PER_CODE // BITS_PER_INDEX
    CODE_MASK = (1 << BITS_PER_CODE) - 1
    CODES_PER_SUBSET = 256 // BITS_PER_CODE

    def _make_bitset(self, characters, positive, reverse, fuzzy):
        code = []
        # values for big bitset are: max_char indexes... subsets...
        # values for small bitset are: top_bits bitset
        bitset_dict = defaultdict(int)
        for c in characters:
            bitset_dict[c >> 8] |= 1 << (c & 0xFF)

        flags = 0
        if positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP

        if len(bitset_dict) > 1:
            # Build a big bitset.
            indexes = []
            subset_index = {}
            for top in range(max(bitset_dict.keys()) + 1):
                subset = bitset_dict.get(top, 0)
                ind = subset_index.setdefault(subset, len(subset_index))
                indexes.append(ind)

            remainder = len(indexes) % SetBase.INDEXES_PER_CODE
            if remainder:
                indexes.extend([0] * (SetBase.INDEXES_PER_CODE - remainder))

            data = []
            for i in range(0, len(indexes), SetBase.INDEXES_PER_CODE):
                ind = 0
                for s in range(SetBase.INDEXES_PER_CODE):
                    ind |= indexes[i + s] << (SetBase.BITS_PER_INDEX * s)

                data.append(ind)

            for subset, ind in sorted(subset_index.items(), key=lambda pair:
              pair[1]):
                data.extend(SetBase._bitset_to_codes(subset))

            code.append((self._big_bitset_opcode[reverse], flags,
              max(characters)) + tuple(data))
        else:
            # Build a small bitset.
            top_bits, bitset = bitset_dict.items()[0]
            code.append((self._small_bitset_opcode[reverse], flags, top_bits) +
              tuple(SetBase._bitset_to_codes(bitset)))

        return code

    @staticmethod
    def _bitset_to_codes(bitset):
        codes = []
        for i in range(SetBase.CODES_PER_SUBSET):
            codes.append(bitset & SetBase.CODE_MASK)
            bitset >>= BITS_PER_CODE

        return codes

class SetDiff(SetBase):
    _opcode = {False: OP.SET_DIFF, True: OP.SET_DIFF_REV}
    _op_name = {False: "SET_DIFF", True: "SET_DIFF_REV"}

    def optimise(self, info):
        items = []
        # First item is inclusive, following items are exclusive.
        for m in self.items:
            m = m.optimise(info)
            if isinstance(m, SetDiff) and m.positive:
                if items:
                    # Following item, so it's exclusive.
                    items.append(m)
                else:
                    # First item, so its first item is inclusive and its
                    # following items are exclusive.
                    items.extend(m.items)
            elif isinstance(m, SetUnion) and m.positive:
                if items:
                    # Following item, so its items are exclusive.
                    items.extend(m.items)
                else:
                    # First item, so its inclusive.
                    items.append(m)
            else:
                items.append(m)

        return self._rebuild(items)

class SetInter(SetBase):
    _opcode = {False: OP.SET_INTER, True: OP.SET_INTER_REV}
    _op_name = {False: "SET_INTER", True: "SET_INTER_REV"}

    def optimise(self, info):
        items = []
        for m in self.items:
            m = m.optimise(info)
            if isinstance(m, SetInter) and m.positive:
                # Intersection in intersection.
                items.extend(m.items)
            else:
                items.append(m)

        return self._rebuild(items)

class SetSymDiff(SetBase):
    _opcode = {False: OP.SET_SYM_DIFF, True: OP.SET_SYM_DIFF_REV}
    _op_name = {False: "SET_SYM_DIFF", True: "SET_SYM_DIFF_REV"}

    def optimise(self, info):
        items = []
        for m in self.items:
            m = m.optimise(info)
            if isinstance(m, SetSymDiff) and m.positive:
                # Symmetric difference in symmetric difference.
                items.extend(m.items)
            else:
                items.append(m)

        return self._rebuild(items)

class SetUnion(SetBase):
    _opcode = {False: OP.SET_UNION, True: OP.SET_UNION_REV}
    _op_name = {False: "SET_UNION", True: "SET_UNION_REV"}

    def optimise(self, info):
        items = []
        for m in self.items:
            m = m.optimise(info)
            if isinstance(m, SetUnion) and m.positive:
                # Union in union.
                items.extend(m.items)
            else:
                items.append(m)

        return self._rebuild(items)

    def compile(self, reverse=False, fuzzy=False):
        characters, others = set(), set()
        for m in self.items:
            if type(m) is Character:
                characters.add(m.value)
            elif type(m) is CharacterIgn:
                characters.update(all_cases(m.info, m.value))
            elif type(m) is Range:
                characters.update(range(m.lower, m.upper + 1))
            elif type(m) is RangeIgn:
                for c in range(m.lower, m.upper + 1):
                    characters.update(all_cases(m.info, c))
            else:
                others.add(m)

        # If there are only characters then compile to a bitset.
        if not others:
            return self._make_bitset(characters, self.positive, reverse, fuzzy)

        # Compile a compound set.
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP
        code = [(self._opcode[reverse], flags)]
        if characters:
            code.extend(self._make_bitset(characters, True, False, False))

        for m in others:
            code.extend(m.compile())

        code.append((OP.END, ))

        return code

    def firstset(self):
        return set([self])

    def has_simple_start(self):
        return True

class StartOfLine(ZeroWidthBase):
    _opcode = OP.START_OF_LINE
    _op_name = "START_OF_LINE"

class StartOfLineU(StartOfLine):
    _opcode = OP.START_OF_LINE_U
    _op_name = "START_OF_LINE_U"

class StartOfString(ZeroWidthBase):
    _opcode = OP.START_OF_STRING
    _op_name = "START_OF_STRING"

class String(RegexBase):
    _opcode = {False: OP.STRING, True: OP.STRING_REV}
    _op_name = {False: "STRING", True: "STRING_REV"}

    def __init__(self, info, characters):
        self.info, self.characters = info, characters
        self._key = self.__class__, self.characters

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[reverse], flags, len(self.characters)) +
          tuple(self.characters)]

    def dump(self, indent=0, reverse=False):
        print "%s%s %s" % (INDENT * indent, self._op_name[reverse],
          " ".join(map(str, self.characters)))

    def firstset(self):
        return set([Character(self.info, self.characters[0])])

    def has_simple_start(self):
        return True

    def get_first_char(self):
        raise error("internal error")

    def drop_first_char(self):
        raise error("internal error")

class StringIgn(String):
    _opcode = {False: OP.STRING_IGN, True: OP.STRING_IGN_REV}
    _op_name = {False: "STRING_IGN", True: "STRING_IGN_REV"}

    def firstset(self):
        return set([CharacterIgn(self.info, self.characters[0])])

class StringSet(RegexBase):
    _opcode = {False: OP.STRING_SET, True: OP.STRING_SET_REV}
    _op_name = {False: "STRING_SET", True: "STRING_SET_REV"}

    _char_type = Character
    _str_type = String

    def __init__(self, info, name):
        self.info, self.name = info, name
        self._key = self.__class__, self.name

        self.set_key = (name, False)
        if self.set_key not in info.named_lists:
            info.named_lists[self.set_key] = len(info.named_lists)

    def compile(self, reverse=False, fuzzy=False):
        index = self.info.named_lists[self.set_key]
        items = self.info.kwargs[self.name]
        if fuzzy:
            strings = []
            for i in items:
                strings.append(tuple(ord(c) for c in i))
            s = self._build_string_set(strings, self._char_type,
              self._str_type)
            self._flatten(s, String)
            return s.compile(reverse, fuzzy)
        else:
            min_len = min(len(i) for i in items)
            max_len = max(len(i) for i in items)
            return [(self._opcode[reverse], index, min_len, max_len)]

    def dump(self, indent=0, reverse=False):
        print "%s%s %s" % (INDENT * indent, self._op_name[reverse], self.name)

    def _build_string_set(self, strings, char_type, str_type):
        # Divide the strings according to their first character.
        optional = False
        prefixed = defaultdict(set)
        for s in strings:
            try:
                prefixed[s[0]].add(s)
            except IndexError:
                optional = True

        # Collect lone characters and lone strings.
        char_set = set()
        string_set = set()
        others = {}
        for c, subset in prefixed.items():
            if len(subset) == 1:
                s = list(subset)[0]
                if len(s) == 1:
                    # It's a lone character.
                    char_set.add(s[0])
                else:
                    # It's a lone string.
                    string_set.add(s)
            else:
                # The 'normal' case.
                others[c] = self._build_string_set([s[1 : ] for s in subset],
                  char_type, str_type)

        branches = []

        # Add the character set for the lone characters.
        if char_set:
            if len(char_set) == 1:
                branches.append(char_type(self.info, list(char_set)[0]))
            else:
                branches.append(SetUnion([char_type(self.info, c) for c in
                  char_set]))

        # Add the string subsets, complete with their character prefixes.
        for c, s in others.items():
            s = Sequence([char_type(self.info, c), s])
            branches.append(s)

        # Add the lone strings.
        for s in string_set:
            branches.append(str_type(self.info, list(s)))

        # Is this string set optional?
        if optional:
            branches.append(Sequence())

        if not branches:
            return Sequence()

        if len(branches) == 1:
            return branches[0]

        return Branch(branches)

    def _flatten(self, s, str_type):
        if isinstance(s, Branch):
            for b in s.branches:
                self._flatten(b, str_type)
        elif isinstance(s, Sequence) and s.sequence:
            seq = s.sequence

            while isinstance(seq[-1], Sequence):
                seq[-1 : ] = seq[-1].sequence

            n = 0
            while n < len(seq) and isinstance(seq[n], Character):
                n += 1

            if n > 1:
                seq[ : n] = [str_type(self.info, [c.value for c in seq[ : n]])]

            self._flatten(seq[-1], str_type)

class StringSetIgn(StringSet):
    _opcode = {False: OP.STRING_SET_IGN, True: OP.STRING_SET_IGN_REV}
    _op_name = {False: "STRING_SET_IGN", True: "STRING_SET_IGN_REV"}

    _char_type = CharacterIgn
    _str_type = StringIgn

    def __init__(self, info, name):
        self.info, self.name = info, name
        self._key = self.__class__, self.name

        self.set_key = (name, True)
        if self.set_key not in info.named_lists:
            info.named_lists[self.set_key] = len(info.named_lists)

class Source(object):
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

class Scanner:
    def __init__(self, lexicon, flags=0):
        self.lexicon = lexicon

        # Combine phrases into a compound pattern.
        patterns = []
        for phrase, action in lexicon:
            # Parse the regular expression.
            source = Source(phrase)
            info = Info(flags, source.char_type)
            source.ignore_space = bool(info.all_flags & VERBOSE)
            parsed = parse_pattern(source, info)
            if not source.at_end():
                raise error("trailing characters")

            # We want to forbid capture groups within each phrase.
            patterns.append(parsed.remove_captures())

        # Combine all the subpatterns into one pattern.
        info = Info(flags)
        patterns = [Group(info, g + 1, p) for g, p in enumerate(patterns)]
        parsed = Branch(patterns)

        # Optimise the compound pattern.
        parsed = parsed.optimise(info)
        parsed = parsed.pack_characters(info)

        reverse = bool(info.global_flags & REVERSE)

        # Compile the compound pattern. The result is a list of tuples.
        code = parsed.compile(reverse) + [(OP.SUCCESS, )]

        # Flatten the code into a list of ints.
        code = flatten_code(code)

        if not parsed.has_simple_start():
            # Get the first set, if possible.
            try:
                fs_code = compile_firstset(info, parsed.firstset())
                fs_code = flatten_code(fs_code)
                code = fs_code + code
            except FirstSetError:
                pass

        # Create the PatternObject.
        #
        # Local flags like IGNORECASE affect the code generation, but aren't
        # needed by the PatternObject itself. Conversely, global flags like
        # LOCALE _don't_ affect the code generation but _are_ needed by the
        # PatternObject.
        self.scanner = _regex.compile(None, flags & GLOBAL_FLAGS, code, {}, {},
          {})

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

ALL_CHAR_TYPES = (Character, CharacterIgn)
STRING_CLASSES = {Character: String, CharacterIgn: StringIgn}

PROPERTIES = _regex.get_properties()

# Character escape sequences.
CHARACTER_ESCAPES = {
    "a": "\a",
    "b": "\b",
    "f": "\f",
    "n": "\n",
    "r": "\r",
    "t": "\t",
    "v": "\v",
}

# Predefined character set escape sequences.
CHARSET_ESCAPES = {
    "d": lookup_property(None, "DIGIT", True),
    "D": lookup_property(None, "DIGIT", False),
    "s": lookup_property(None, "SPACE", True),
    "S": lookup_property(None, "SPACE", False),
    "w": lookup_property(None, "WORD", True),
    "W": lookup_property(None, "WORD", False),
}

# Positional escape sequences.
POSITION_ESCAPES = {
    "A": StartOfString(),
    "b": Boundary(),
    "B": Boundary(False),
    "Z": EndOfString(),
}

# Positional escape sequences when WORD flag set.
WORD_POSITION_ESCAPES = dict(POSITION_ESCAPES)
WORD_POSITION_ESCAPES.update({
    "b": DefaultBoundary(),
    "B": DefaultBoundary(False),
})
