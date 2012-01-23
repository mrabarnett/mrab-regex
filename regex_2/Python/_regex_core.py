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

__all__ = ["A", "ASCII", "B", "BESTMATCH", "D", "DEBUG", "F", "FULLCASE", "I",
  "IGNORECASE", "L", "LOCALE", "M", "MULTILINE", "R", "REVERSE", "S", "DOTALL",
  "T", "TEMPLATE", "U", "UNICODE", "V0", "VERSION0", "V1", "VERSION1", "W",
  "WORD", "X", "VERBOSE", "error", "ALNUM", "NONLITERAL", "Info", "Source",
  "FirstSetError", "UnscopedFlagSet", "OP", "Scanner", "compile_firstset",
  "compile_repl_escape", "count_ones", "flatten_code", "fold_case",
  "parse_pattern", "shrink_cache", "REGEX_FLAGS"]

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
A = ASCII = 0x80        # Assume ASCII locale.
B = BESTMATCH = 0x1000  # Best fuzzy match.
D = DEBUG = 0x200       # Print parsed pattern.
F = FULLCASE = 0x4000   # Unicode Full case-folding.
I = IGNORECASE = 0x2    # Ignore case.
L = LOCALE = 0x4        # Assume current 8-bit locale.
M = MULTILINE = 0x8     # Make anchors look for newline.
R = REVERSE = 0x400     # Search backwards.
S = DOTALL = 0x10       # Make dot match newline.
U = UNICODE = 0x20      # Assume Unicode locale.
V0 = VERSION0 = 0x2000  # Old legacy behaviour.
V1 = VERSION1 = 0x100   # New enhanced behaviour.
W = WORD = 0x800        # Default Unicode word breaks.
X = VERBOSE = 0x40      # Ignore whitespace and comments.
T = TEMPLATE = 0x1      # Template (present because re module has it).

DEFAULT_VERSION = VERSION1

ALL_VERSIONS = VERSION0 | VERSION1
ALL_ENCODINGS = ASCII | LOCALE | UNICODE

# The default flags for the various versions.
DEFAULT_FLAGS = {VERSION0: 0, VERSION1: FULLCASE}

# The mask for the flags.
GLOBAL_FLAGS = ALL_ENCODINGS | ALL_VERSIONS | BESTMATCH | DEBUG | REVERSE
SCOPED_FLAGS = FULLCASE | IGNORECASE | MULTILINE | DOTALL | WORD | VERBOSE

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
REGEX_FLAGS = {"a": ASCII, "b": BESTMATCH, "f": FULLCASE, "i": IGNORECASE, "L":
  LOCALE, "m": MULTILINE, "r": REVERSE, "s": DOTALL, "u": UNICODE, "V0":
  VERSION0, "V1": VERSION1, "w": WORD, "x": VERBOSE}

# The case flags.
CASE_FLAGS = FULLCASE | IGNORECASE
NOCASE = 0
FULLIGNORECASE = FULLCASE | IGNORECASE

FULL_CASE_FOLDING = UNICODE | FULLIGNORECASE

# The number of digits in hexadecimal escapes.
HEX_ESCAPES = {"x": 2, "u": 4, "U": 8}

# A singleton which indicates a comment within a pattern.
COMMENT = object()

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
BOUNDARY
BRANCH
CHARACTER
CHARACTER_IGN
CHARACTER_IGN_REV
CHARACTER_REV
DEFAULT_BOUNDARY
DEFAULT_END_OF_WORD
DEFAULT_START_OF_WORD
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
END_OF_WORD
FUZZY
GRAPHEME_BOUNDARY
GREEDY_REPEAT
GREEDY_REPEAT_ONE
GROUP
GROUP_CALL
GROUP_EXISTS
GROUP_RETURN
LAZY_REPEAT
LAZY_REPEAT_ONE
LOOKAROUND
NEXT
NOT_GROUP_CALL
PROPERTY
PROPERTY_IGN
PROPERTY_IGN_REV
PROPERTY_REV
RANGE
RANGE_IGN
RANGE_IGN_REV
RANGE_REV
REF_GROUP
REF_GROUP_FLD
REF_GROUP_FLD_REV
REF_GROUP_IGN
REF_GROUP_IGN_REV
REF_GROUP_REV
SEARCH_ANCHOR
SET_DIFF
SET_DIFF_IGN
SET_DIFF_IGN_REV
SET_DIFF_REV
SET_INTER
SET_INTER_IGN
SET_INTER_IGN_REV
SET_INTER_REV
SET_SYM_DIFF
SET_SYM_DIFF_IGN
SET_SYM_DIFF_IGN_REV
SET_SYM_DIFF_REV
SET_UNION
SET_UNION_IGN
SET_UNION_IGN_REV
SET_UNION_REV
START_GROUP
START_OF_LINE
START_OF_LINE_U
START_OF_STRING
START_OF_WORD
STRING
STRING_FLD
STRING_FLD_REV
STRING_IGN
STRING_IGN_REV
STRING_REV
STRING_SET
STRING_SET_FLD
STRING_SET_FLD_REV
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

def fold_case(info, string):
    "Folds the case of a string."
    flags = info.flags
    if (flags & ALL_ENCODINGS) == 0:
        flags |= info.guess_encoding
    return _regex.fold_case(flags, string)

def is_cased(info, char):
    "Checks whether a character is cased."
    return len(_regex.get_all_cases(info.flags, char)) > 1

def compile_firstset(info, fs):
    "Compiles the firstset for the pattern."
    if not fs or None in fs:
        return []

    # If we ignore the case, for simplicity we won't build a firstset.
    members = set()
    for i in fs:
        if i.case_flags:
            if isinstance(i, Character):
                if is_cased(info, i.value):
                    return []
            elif isinstance(i, SetBase):
                return []

        members.add(i.with_flags(case_flags=NOCASE))

    # Build the firstset.
    fs = SetUnion(list(members), zerowidth=True)
    fs = fs.optimise(info, in_set=True)

    # Compile the firstset.
    return fs.compile(bool(info.flags & REVERSE))

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

def make_character(info, value, in_set=False):
    "Makes a character literal."
    if in_set:
        # A character set is built case-sensitively.
        return Character(value)

    return Character(value, case_flags=info.flags & CASE_FLAGS)

def make_ref_group(info, name):
    "Makes a group reference."
    return RefGroup(info, name, case_flags=info.flags & CASE_FLAGS)

def make_string_set(info, name):
    "Makes a string set."
    return StringSet(info, name, case_flags=info.flags & CASE_FLAGS)

def make_property(info, prop, in_set):
    "Makes a property."
    if in_set:
        return prop

    return prop.with_flags(case_flags=info.flags & CASE_FLAGS)

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

    return make_sequence(sequence)

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

        if element.is_empty() or min_count == max_count == 1:
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
    while True:
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
        if ch == "(" and source.match("?#"):
            # A comment.
            parse_comment(source)
            continue
        if ch == "#" and (info.flags & VERBOSE):
            parse_hash_comment(source)
            continue

        # Neither a quantifier nor a comment.
        break

    # Parse it later, perhaps as a literal.
    source.pos = here
    return None

def parse_hash_comment(source):
    "Parses a single-line 'hash' comment."
    source.ignore_space = False
    try:
        # Ignore characters until a newline or the end of the
        # pattern.
        while source.get() not in "\n":
            pass
    finally:
        source.ignore_space = True

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

        if (min_count >= UNLIMITED or max_count is not None and max_count >=
          UNLIMITED):
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
    try:
        parse_cost_constraint(source, constraints)
    except ParseError:
        source.pos = here

        parse_cost_equation(source, constraints)

def parse_cost_constraint(source, constraints):
    "Parses a cost constraint."
    here = source.pos
    ch = source.get()
    if ch in ALPHA:
        # Syntax: constraint [("<=" | "<") cost]
        constraint = parse_constraint(source, constraints, ch)

        max_inc = parse_fuzzy_compare(source)

        if max_inc is None:
            # No maximum cost.
            constraints[constraint] = 0, None
        else:
            # There's a maximum cost.
            max_cost = int(parse_count(source))

            # Inclusive or exclusive limit?
            if not max_inc:
                max_cost -= 1

            if max_cost < 0:
                raise error("bad fuzzy cost limit")

            constraints[constraint] = 0, max_cost
    elif ch in DIGITS:
        # Syntax: cost ("<=" | "<") constraint ("<=" | "<") cost
        source.pos = here
        try:
            # Minimum cost.
            min_cost = int(parse_count(source))

            min_inc = parse_fuzzy_compare(source)
            if min_inc is None:
                raise ParseError()

            constraint = parse_constraint(source, constraints, source.get())

            max_inc = parse_fuzzy_compare(source)
            if max_inc is None:
                raise ParseError()

            # Maximum cost.
            max_cost = int(parse_count(source))

            # Inclusive or exclusive limits?
            if not min_inc:
                min_cost += 1
            if not max_inc:
                max_cost -= 1

            if not 0 <= min_cost <= max_cost:
                raise error("bad fuzzy cost limit")

            constraints[constraint] = min_cost, max_cost
        except ValueError:
            raise ParseError()
    else:
        raise ParseError()

def parse_constraint(source, constraints, ch):
    "Parses a constraint."
    if ch not in "deis":
        raise error("bad fuzzy constraint")

    if ch in constraints:
        raise error("repeated fuzzy constraint")

    return ch

def parse_fuzzy_compare(source):
    "Parses a cost comparator."
    if source.match("<="):
        return True
    elif source.match("<"):
        return False
    else:
        return None

def parse_cost_equation(source, constraints):
    "Parses a cost equation."
    if "cost" in constraints:
        raise error("more than one cost equation")

    cost = {}

    parse_cost_term(source, cost)
    while source.match("+"):
        parse_cost_term(source, cost)

    max_inc = parse_fuzzy_compare(source)
    if max_inc is None:
        raise error("missing fuzzy cost limit")

    max_cost = int(parse_count(source))

    if not max_inc:
        max_cost -= 1

    if max_cost < 0:
        raise error("bad fuzzy cost limit")

    cost["max"] = max_cost

    constraints["cost"] = cost

def parse_cost_term(source, cost):
    "Parses a cost equation term."
    coeff = parse_count(source)
    ch = source.get()
    if ch not in "dis":
        raise ParseError()

    if ch in cost:
        raise error("repeated fuzzy cost")

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
    """Parses an element. An element might actually be a flag, eg. '(?i)', in
    which case it returns None.
    """
    while True:
        here = source.pos
        ch = source.get()
        if ch in SPECIAL_CHARS:
            if ch in ")|":
                # The end of a sequence. At the end of the pattern ch is "".
                source.pos = here
                return None
            elif ch == "\\":
                # An escape sequence outside a set.
                return parse_escape(source, info, False)
            elif ch == "(":
                # A parenthesised subpattern or a flag.
                element = parse_paren(source, info)
                if element and element is not COMMENT:
                    return element
            elif ch == ".":
                # Any character.
                if info.flags & DOTALL:
                    return AnyAll()
                elif info.flags & WORD:
                    return AnyU()
                else:
                    return Any()
            elif ch == "[":
                # A character set.
                return parse_set(source, info)
            elif ch == "^":
                # The start of a line or the string.
                if info.flags & MULTILINE:
                    if info.flags & WORD:
                        return StartOfLineU()
                    else:
                        return StartOfLine()
                else:
                    return StartOfString()
            elif ch == "$":
                # The end of a line or the string.
                if info.flags & MULTILINE:
                    if info.flags & WORD:
                        return EndOfLineU()
                    else:
                        return EndOfLine()
                else:
                    if info.flags & WORD:
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
                return make_character(info, ord(ch))
            elif ch in "?*+":
                # A quantifier where we expected an element.
                raise error("nothing to repeat")
            elif ch == "#" and (info.flags & VERBOSE):
                # A comment.
                parse_hash_comment(source)
            else:
                # A literal.
                return make_character(info, ord(ch))
        else:
            # A literal.
            return make_character(info, ord(ch))

def parse_paren(source, info):
    "Parses a parenthesised subpattern or a flag."
    here = source.pos
    ch = source.get()
    if ch == "?":
        # (?...
        here2 = source.pos
        ch = source.get()
        if ch == "<":
            # (?<...
            here3 = source.pos
            ch = source.get()
            if ch == "=":
                # (?<=...: positive lookbehind.
                return parse_lookaround(source, info, True, True)
            if ch == "!":
                # (?<!...: negative lookbehind.
                return parse_lookaround(source, info, True, False)

            # (?<...: a named capture group.
            source.pos = here3
            name = parse_name(source)
            group = info.new_group(name)
            source.expect(">")
            saved_flags = info.flags
            saved_ignore = source.ignore_space
            try:
                subpattern = parse_pattern(source, info)
            finally:
                source.ignore_space = saved_ignore
                info.flags = saved_flags

            source.expect(")")
            info.close_group(group)
            return Group(info, group, subpattern)
        if ch == "=":
            # (?=...: positive lookahead.
            return parse_lookaround(source, info, False, True)
        if ch == "!":
            # (?!...: negative lookahead.
            return parse_lookaround(source, info, False, False)
        if ch == "P":
            # (?P...: a Python extension.
            return parse_extension(source, info)
        if ch == "#":
            # (?#...: a comment.
            return parse_comment(source)
        if ch == "(":
            # (?(...: a conditional subpattern.
            return parse_conditional(source, info)
        if ch == ">":
            # (?>...: an atomic subpattern.
            return parse_atomic(source, info)
        if ch == "|":
            # (?|...: a common/reset groups branch.
            return parse_common(source, info)
        if ch == "R" or "0" <= ch <= "9":
            # (?R...: probably a call to a group.
            return parse_call_group(source, info, ch)
        if ch == "&":
            # (?&...: a call to a named group.
            return parse_call_named_group(source, info)

        # (?...: probably a flags subpattern.
        source.pos = here2
        return parse_flags_subpattern(source, info)

    # (...: an unnamed capture group.
    source.pos = here
    group = info.new_group()
    saved_flags = info.flags
    saved_ignore = source.ignore_space
    try:
        subpattern = parse_pattern(source, info)
    finally:
        source.ignore_space = saved_ignore
        info.flags = saved_flags

    source.expect(")")
    info.close_group(group)

    return Group(info, group, subpattern)

def parse_extension(source, info):
    "Parses a Python extension."
    here = source.pos
    ch = source.get()
    if ch == "<":
        # (?P<...: a named capture group.
        name = parse_name(source)
        group = info.new_group(name)
        source.expect(">")
        saved_flags = info.flags
        saved_ignore = source.ignore_space
        try:
            subpattern = parse_pattern(source, info)
        finally:
            source.ignore_space = saved_ignore
            info.flags = saved_flags

        source.expect(")")
        info.close_group(group)

        return Group(info, group, subpattern)
    if ch == "=":
        # (?P=...: a named group reference.
        name = parse_name(source)
        source.expect(")")
        if info.is_open_group(name):
            raise error("can't refer to an open group")

        return make_ref_group(info, name)
    if ch == ">" or ch == "&":
        # (?P>...: a call to a group.
        return parse_call_named_group(source, info)

    source.pos = here
    raise error("unknown extension")

def parse_comment(source):
    "Parses a comment."
    ch = source.get()
    while ch not in ")":
        ch = source.get()

    if not ch:
        raise error("missing )")

    return COMMENT

def parse_lookaround(source, info, behind, positive):
    "Parses a lookaround."
    saved_flags = info.flags
    saved_ignore = source.ignore_space
    try:
        subpattern = parse_pattern(source, info)
    finally:
        source.ignore_space = saved_ignore
        info.flags = saved_flags

    source.expect(")")

    return LookAround(behind, positive, subpattern)

def parse_conditional(source, info):
    "Parses a conditional subpattern."
    saved_flags = info.flags
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
        info.flags = saved_flags

    source.expect(")")

    return Conditional(info, group, yes_branch, no_branch)

def parse_atomic(source, info):
    "Parses an atomic subpattern."
    saved_flags = info.flags
    saved_ignore = source.ignore_space
    try:
        subpattern = parse_pattern(source, info)
    finally:
        source.ignore_space = saved_ignore
        info.flags = saved_flags

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

def parse_call_group(source, info, ch):
    "Parses a call to a group."
    if ch == "R":
        source.expect(")")
        group = "0"
    else:
        group = [ch]

        ch = source.get()
        while "0" <= ch <= "9":
            group.append(ch)
            ch = source.get()

        if ch != ")":
            raise error("expected )")

        group = "".join(group)

    return CallGroup(info, group)

def parse_call_named_group(source, info):
    "Parses a call to a named group."
    group = parse_name(source)
    source.expect(")")

    return CallGroup(info, group)

def parse_flags_subpattern(source, info):
    "Parses a flags subpattern."
    # It could be inline flags or a subpattern possibly with local flags.
    # Parse the flags.
    flags_on, flags_off = 0, 0
    try:
        while True:
            here = source.pos
            ch = source.get()
            if ch == "V":
                ch += source.get()
            flags_on |= REGEX_FLAGS[ch]
    except KeyError:
        pass

    flags_on |= DEFAULT_FLAGS.get(flags_on & ALL_VERSIONS, 0)

    if ch == "-":
        try:
            while True:
                here = source.pos
                ch = source.get()
                if ch == "V":
                    ch += source.get()
                flags_off |= REGEX_FLAGS[ch]
        except KeyError:
            pass

        if not flags_off:
            raise error("bad inline flags: no flags after '-'")

        if (flags_off & GLOBAL_FLAGS):
            raise error("bad inline flags: can't turn off global flag")

    # Separate the global and scoped flags.
    source.pos = here
    saved_flags = info.flags
    info.flags = (info.flags | flags_on) & ~(flags_off & SCOPED_FLAGS)
    saved_ignore = source.ignore_space
    source.ignore_space = bool(info.flags & VERBOSE)
    if source.match(":"):
        # A subpattern with local flags.
        if flags_on & GLOBAL_FLAGS:
            raise error("bad inline flags: can't scope global flag")

        try:
            subpattern = parse_pattern(source, info)

            # Consume trailing whitespace if VERBOSE.
            if source.get():
                source.pos -= 1
        finally:
            source.ignore_space = saved_ignore
            info.flags = saved_flags

        source.expect(")")

        return subpattern
    else:
        # Positional flags.
        if not source.match(")"):
            raise error("bad inline flags: " + repr(source.get()))

        version = (info.flags & ALL_VERSIONS) or DEFAULT_VERSION
        if version == VERSION0:
            # Positional flags are global and can only be turned on.
            if flags_off:
                raise error("bad inline flags: can't turn flags off")

            info.global_flags = info.flags
        else:
            info.global_flags = info.flags & GLOBAL_FLAGS

        if info.global_flags & ~saved_flags:
            # A global has been turned on, so reparse the pattern.
            raise UnscopedFlagSet(info.global_flags)

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
    if not name or name[0].isdigit() and not name.isdigit():
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
    saved_ignore = source.ignore_space
    source.ignore_space = False
    ch = source.get()
    source.ignore_space = saved_ignore
    if not ch:
        # A backslash at the end of the pattern.
        raise error("bad escape")
    if ch in HEX_ESCAPES:
        # A hexadecimal escape sequence.
        return parse_hex_escape(source, info, HEX_ESCAPES[ch], in_set)
    elif ch == "g" and not in_set:
        # A group reference.
        here = source.pos
        try:
            return parse_group_ref(source, info)
        except error:
            # Invalid as a group reference, so assume it's a literal.
            source.pos = here

        return make_character(info, ord(ch), in_set)
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
        # A Unicode property, positive or negative.
        return parse_property(source, info, ch == "p", in_set)
    elif ch == "X" and not in_set:
        # A grapheme cluster.
        return Grapheme()
    elif ch in ALPHA:
        # An alphabetic escape sequence.
        # Positional escapes aren't allowed inside a character set.
        if not in_set:
            if info.flags & WORD:
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
            return Character(ord(value))

        return make_character(info, ord(ch), in_set)
    elif ch in DIGITS:
        # A numeric escape sequence.
        return parse_numeric_escape(source, info, ch, in_set)
    else:
        # A literal.
        return make_character(info, ord(ch), in_set)

def parse_numeric_escape(source, info, ch, in_set):
    "Parses a numeric escape sequence."
    if in_set or ch == "0":
        # Octal escape sequence, max 3 digits.
        return parse_octal_escape(source, info, [ch], in_set)

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
            encoding = info.flags & ALL_ENCODINGS
            if encoding == ASCII or encoding == LOCALE:
                octal_mask = 0xFF
            else:
                octal_mask = 0x1FF

            value = int(digits + ch, 8) & octal_mask
            return make_character(info, value)

    # Group reference.
    source.pos = here
    if info.is_open_group(digits):
        raise error("can't refer to an open group")

    return make_ref_group(info, digits)

def parse_octal_escape(source, info, digits, in_set):
    "Parses an octal escape sequence."
    here = source.pos
    ch = source.get()
    while len(digits) < 3 and ch in OCT_DIGITS:
        digits.append(ch)
        here = source.pos
        ch = source.get()

    source.pos = here
    try:
        value = int("".join(digits), 8)
        return make_character(info, value, in_set)
    except ValueError:
        raise error("bad octal escape")

def parse_hex_escape(source, info, expected_len, in_set):
    "Parses a hex escape sequence."
    digits = []
    for i in range(expected_len):
        ch = source.get()
        if ch not in HEX_DIGITS:
            raise error("bad hex escape")
        digits.append(ch)

    value = int("".join(digits), 16)
    return make_character(info, value, in_set)

def parse_group_ref(source, info):
    "Parses a group reference."
    source.expect("<")
    name = parse_name(source, True)
    source.expect(">")
    if info.is_open_group(name):
        raise error("can't refer to an open group")

    return make_ref_group(info, name)

def parse_string_set(source, info):
    "Parses a string set reference."
    source.expect("<")
    name = parse_name(source, True)
    source.expect(">")
    if name is None or name not in info.kwargs:
        raise error("undefined named list")

    return make_string_set(info, name)

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
                return make_character(info, ord(value), in_set)
            except KeyError:
                raise error("undefined character name")

    source.pos = here
    return make_character(info, ord("N"), in_set)

def parse_property(source, info, positive, in_set):
    "Parses a Unicode property."
    here = source.pos
    ch = source.get()
    if ch == "{":
        negate = source.match("^")
        prop_name, name = parse_property_name(source)
        if source.match("}"):
            # It's correctly delimited.
            prop = lookup_property(prop_name, name, positive != negate)
            return make_property(info, prop, in_set)
    elif ch and ch in "CLMNPSZ":
        # An abbreviated property, eg \pL.
        prop = lookup_property(None, ch, positive)
        return make_property(info, prop, in_set)

    # Not a property, so treat as a literal "p" or "P".
    source.pos = here
    ch = "p" if positive else "P"
    return make_character(info, ord(ch), in_set)

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
    version = (info.flags & ALL_VERSIONS) or DEFAULT_VERSION

    saved_ignore = source.ignore_space
    source.ignore_space = False
    # Negative set?
    negate = source.match("^")
    try:
        if version == VERSION0:
            item = parse_set_imp_union(source, info)
        else:
            item = parse_set_union(source, info)

        if not source.match("]"):
            raise error("missing ]")
    finally:
        source.ignore_space = saved_ignore

    if negate:
        item = item.with_flags(positive=not item.positive)

    item = item.with_flags(case_flags=info.flags & CASE_FLAGS)

    return item

def parse_set_union(source, info):
    "Parses a set union ([x||y])."
    items = [parse_set_symm_diff(source, info)]
    while source.match("||"):
        items.append(parse_set_symm_diff(source, info))

    if len(items) == 1:
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
    version = (info.flags & ALL_VERSIONS) or DEFAULT_VERSION

    items = [parse_set_member(source, info)]
    while True:
        here = source.pos
        if source.match("]"):
            # End of the set.
            source.pos = here
            break

        if version == VERSION1 and any(source.match(op) for op in SET_OPS):
            # The new behaviour has set operators.
            source.pos = here
            break

        items.append(parse_set_member(source, info))

    if len(items) == 1:
        return items[0]
    return SetUnion(items)

def parse_set_member(source, info):
    "Parses a member in a character set."
    # Parse a set item.
    start = parse_set_item(source, info)
    if (not isinstance(start, Character) or not start.positive or not
      source.match("-")):
        # It's not the start of a range.
        return start

    # It looks like the start of a range of characters.
    here = source.pos
    if source.match("]"):
        # We've reached the end of the set, so return both the character and
        # hyphen.
        source.pos = here
        return SetUnion([start, Character(ord("-"))])

    # Parse a set item.
    end = parse_set_item(source, info)
    if not isinstance(end, Character) or not end.positive:
        # It's not a range, so return the character, hyphen and property.
        return SetUnion([start, Character(ord("-")), end])

    # It _is_ a range.
    if start.value > end.value:
        raise error("bad character range")

    if start.value == end.value:
        return start

    return Range(start.value, end.value)

def parse_set_item(source, info):
    "Parses an item in a character set."
    version = (info.flags & ALL_VERSIONS) or DEFAULT_VERSION

    if source.match("\\"):
        # An escape sequence in a set.
        return parse_escape(source, info, True)

    here = source.pos
    if source.match("[:"):
        # Looks like a POSIX character class.
        try:
            return parse_posix_class(source, info)
        except ParseError:
            # Not a POSIX character class.
            source.pos = here

    if version == VERSION1 and source.match("["):
        # It's the start of a nested set.

        # Negative set?
        negate = source.match("^")
        item = parse_set_union(source, info)

        if not source.match("]"):
            raise error("missing ]")

        if negate:
            item = item.with_flags(positive=not item.positive)

        return item

    ch = source.get()
    if not ch:
        raise error("bad set", True)

    return Character(ord(ch))

def parse_posix_class(source, info):
    "Parses a POSIX character class."
    negate = source.match("^")
    prop_name, name = parse_property_name(source)
    if not source.match(":]"):
        raise ParseError()

    return lookup_property(prop_name, name, positive=not negate)

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

        if "YES" in value_dict and val_id == 0:
            positive, val_id = not positive, 1

        return Property((prop_id << 16) | val_id, positive)

    # Only the value is provided.
    # It might be the name of a GC, script or block value.
    for property in ("GC", "SCRIPT", "BLOCK"):
        prop_id, value_dict = PROPERTIES.get(property)
        val_id = value_dict.get(value)
        if val_id is not None:
            return Property((prop_id << 16) | val_id, positive)

    # It might be the name of a binary property.
    prop = PROPERTIES.get(value)
    if prop:
        prop_id, value_dict = prop

        if "YES" in value_dict:
            return Property((prop_id << 16) | 1, positive)

    # It might be the name of a binary property starting with a prefix.
    if value.startswith("IS"):
        prop = PROPERTIES.get(value[2 : ])
        if prop:
            prop_id, value_dict = prop
            if "YES" in value_dict:
                return Property((prop_id << 16) | 1, positive)

    # It might be the name of a script or block starting with a prefix.
    for prefix, property in (("IS", "SCRIPT"), ("IN", "BLOCK")):
        if value.startswith(prefix):
            prop_id, value_dict = PROPERTIES.get(property)
            val_id = value_dict.get(value[2 : ])
            if val_id is not None:
                return Property((prop_id << 16) | val_id, positive)

    # Unknown property.
    raise error("unknown property")

def compile_repl_escape(source, pattern, is_unicode):
    "Compiles a replacement template escape sequence."
    ch = source.get()
    if ch in ALPHA:
        # An alphabetic escape sequence.
        value = CHARACTER_ESCAPES.get(ch)
        if value:
            return False, [ord(value)]

        if ch in HEX_ESCAPES and (ch == "x" or is_unicode):
            # A hexadecimal escape sequence.
            return False, [parse_repl_hex_escape(source, HEX_ESCAPES[ch])]

        if ch == "g":
            # A group preference.
            return True, [compile_repl_group(source, pattern)]

        if ch == "N" and is_unicode:
            # A named character.
            value = parse_repl_named_char(source)
            if value is not None:
                return False, [value]

        return False, [ord("\\"), ord(ch)]

    if isinstance(source.sep, str):
        octal_mask = 0xFF
    else:
        octal_mask = 0x1FF

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

        return False, [int(digits, 8) & octal_mask]

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
                return False, [int(digits + ch, 8) & octal_mask]

        # A group reference.
        source.pos = here
        return True, [int(digits)]

    if ch == "\\":
        # An escaped backslash is a backslash.
        return False, [ord("\\")]

    # An escaped non-backslash is a backslash followed by the literal.
    return False, [ord("\\"), ord(ch)]

def parse_repl_hex_escape(source, expected_len):
    "Parses a hex escape sequence in a replacement string."
    digits = []
    for i in range(expected_len):
        ch = source.get()
        if ch not in HEX_DIGITS:
            raise error("bad hex escape")
        digits.append(ch)

    return int("".join(digits), 16)


def parse_repl_named_char(source):
    "Parses a named character in a replacement string."
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
                return ord(value)
            except KeyError:
                raise error("undefined character name")

    source.pos = here
    return None

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

POS_TEXT = {False: "NON-MATCH", True: "MATCH"}
CASE_TEXT = {NOCASE: "", IGNORECASE: " SIMPLE_IGNORE_CASE", FULLCASE: "",
  FULLIGNORECASE: " FULL_IGNORE_CASE"}

def make_sequence(items):
    if len(items) == 1:
        return items[0]
    return Sequence(items)

# Common base class for all nodes.
class RegexBase(object):
    def __init__(self):
        self._key = self.__class__

    def with_flags(self, positive=None, case_flags=None, zerowidth=None):
        if positive is None:
            positive = self.positive
        else:
            positive = bool(positive)
        if case_flags is None:
            case_flags = self.case_flags
        else:
            case_flags = case_flags & CASE_FLAGS
        if zerowidth is None:
            zerowidth = self.zerowidth
        else:
            zerowidth = bool(zerowidth)

        if (positive == self.positive and case_flags == self.case_flags and
          zerowidth == self.zerowidth):
            return self

        return self.rebuild(positive, case_flags, zerowidth)

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

    def can_be_affix(self):
        return True

    def contains_group(self):
        return False

    def can_repeat(self):
        return True

    def get_firstset(self, reverse):
        raise FirstSetError()

    def has_simple_start(self):
        return False

    def is_empty(self):
        return False

    def __hash__(self):
        return hash(self._key)

    def __eq__(self, other):
        return type(self) is type(other) and self._key == other._key

    def __ne__(self, other):
        return not self.__eq__(other)

# Base class for zero-width nodes.
class ZeroWidthBase(RegexBase):
    def __init__(self, positive=True):
        RegexBase.__init__(self)
        self.positive = bool(positive)

        self._key = self.__class__, self.positive

    def can_repeat(self):
        return False

    def get_firstset(self, reverse):
        return set([None])

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
          POS_TEXT[self.positive])

class Any(RegexBase):
    _opcode = {False: OP.ANY, True: OP.ANY_REV}
    _op_name = {False: "ANY", True: "ANY_REV"}

    def has_simple_start(self):
        return True

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[reverse], flags)]

    def dump(self, indent=0, reverse=False):
        print "%s%s" % (INDENT * indent, self._op_name[reverse])

class AnyAll(Any):
    _opcode = {False: OP.ANY_ALL, True: OP.ANY_ALL_REV}
    _op_name = {False: "ANY_ALL", True: "ANY_ALL_REV"}

class AnyU(Any):
    _opcode = {False: OP.ANY_U, True: OP.ANY_U_REV}
    _op_name = {False: "ANY_U", True: "ANY_U_REV"}

class Atomic(RegexBase):
    def __init__(self, subpattern):
        RegexBase.__init__(self)
        self.subpattern = subpattern

    def fix_groups(self):
        self.subpattern.fix_groups()

    def optimise(self, info):
        self.subpattern = self.subpattern.optimise(info)

        if self.subpattern.is_empty():
            return self.subpattern
        return self

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def remove_captures(self):
        self.subpattern = self.subpattern.remove_captures()
        return self

    def can_be_affix(self):
        return self.subpattern.can_be_affix()

    def contains_group(self):
        return self.subpattern.contains_group()

    def get_firstset(self, reverse):
        return self.subpattern.get_firstset(reverse)

    def has_simple_start(self):
        return self.subpattern.has_simple_start()

    def compile(self, reverse=False, fuzzy=False):
        return ([(OP.ATOMIC, )] + self.subpattern.compile(reverse, fuzzy) +
          [(OP.END, )])

    def dump(self, indent=0, reverse=False):
        print "%sATOMIC" % (INDENT * indent)
        self.subpattern.dump(indent + 1, reverse)

    def is_empty(self):
        return self.subpattern.is_empty()

    def __eq__(self, other):
        return (type(self) is type(other) and self.subpattern ==
          other.subpattern)

class Boundary(ZeroWidthBase):
    _opcode = OP.BOUNDARY
    _op_name = "BOUNDARY"

class Branch(RegexBase):
    def __init__(self, branches):
        RegexBase.__init__(self)
        self.branches = branches

    def fix_groups(self):
        for b in self.branches:
            b.fix_groups()

    def optimise(self, info):
        # Flatten branches within branches.
        branches = Branch._flatten_branches(info, self.branches)

        # Move any common prefix or suffix out of the branches.
        prefix, branches = Branch._split_common_prefix(info, branches)
        suffix, branches = Branch._split_common_suffix(info, branches)

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

        return make_sequence(sequence)

    def pack_characters(self, info):
        self.branches = [b.pack_characters(info) for b in self.branches]
        return self

    def remove_captures(self):
        self.branches = [b.remove_captures() for b in self.branches]
        return self

    def is_atomic(self):
        return all(b.is_atomic() for b in self.branches)

    def can_be_affix(self):
        return all(b.can_be_affix() for b in self.branches)

    def contains_group(self):
        return any(b.contains_group() for b in self.branches)

    def get_firstset(self, reverse):
        fs = set()
        for b in self.branches:
            fs |= b.get_firstset(reverse)

        return fs or set([None])

    def compile(self, reverse=False, fuzzy=False):
        code = [(OP.BRANCH, )]
        for b in self.branches:
            code.extend(b.compile(reverse, fuzzy))
            code.append((OP.NEXT, ))

        code[-1] = (OP.END, )

        return code

    def dump(self, indent=0, reverse=False):
        print "%sBRANCH" % (INDENT * indent)
        self.branches[0].dump(indent + 1, reverse)
        for b in self.branches[1 : ]:
            print "%sOR" % (INDENT * indent)
            b.dump(indent + 1, reverse)

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
    def _split_common_prefix(info, branches):
        # Common leading items can be moved out of the branches.
        # Get the items in the branches.
        alternatives = []
        for b in branches:
            if isinstance(b, Sequence):
                alternatives.append(b.items)
            else:
                alternatives.append([b])

        # What is the maximum possible length of the prefix?
        max_count = min(len(a) for a in alternatives)

        # What is the longest common prefix?
        prefix = alternatives[0]
        pos = 0
        end_pos = max_count
        while pos < end_pos and prefix[pos].can_be_affix() and all(a[pos] ==
          prefix[pos] for a in alternatives):
            pos += 1
        count = pos

        if (info.flags & UNICODE) == UNICODE:
            # We need to check that we're not splitting a sequence of
            # characters which could form part of full case-folding.
            while count > 0 and not all(Branch._can_split(a, count) for a in
              alternatives):
                count -= 1

        # No common prefix is possible.
        if count == 0:
            return [], branches

        # Rebuild the branches.
        new_branches = []
        for a in alternatives:
            new_branches.append(make_sequence(a[count : ]))

        return prefix[ : count], new_branches

    @staticmethod
    def _split_common_suffix(info, branches):
        # Common trailing items can be moved out of the branches.
        # Get the items in the branches.
        alternatives = []
        for b in branches:
            if isinstance(b, Sequence):
                alternatives.append(b.items)
            else:
                alternatives.append([b])

        # What is the maximum possible length of the suffix?
        max_count = min(len(a) for a in alternatives)

        # What is the longest common suffix?
        suffix = alternatives[0]
        pos = -1
        end_pos = -1 - max_count
        while pos > end_pos and suffix[pos].can_be_affix() and all(a[pos] ==
          suffix[pos] for a in alternatives):
            pos -= 1
        count = -1 - pos

        if (info.flags & UNICODE) == UNICODE:
            # We need to check that we're not splitting a sequence of
            # characters which could form part of full case-folding.
            while count > 0 and not all(Branch._can_split_rev(a, count) for a
              in alternatives):
                count -= 1

        # No common suffix is possible.
        if count == 0:
            return [], branches

        # Rebuild the branches.
        new_branches = []
        for a in alternatives:
            new_branches.append(make_sequence(a[ : -count]))

        return suffix[-count : ], new_branches

    @staticmethod
    def _can_split(items, count):
        # Check the characters either side of the proposed split.
        if not Branch._is_full_case(items, count - 1):
            return True

        if not Branch._is_full_case(items, count):
            return True

        # Check whether a 1-1 split would be OK.
        if Branch._is_folded(items[count - 1 : count + 1]):
            return False

        # Check whether a 1-2 split would be OK.
        if (Branch._is_full_case(items, count + 2) and
          Branch._is_folded(items[count - 1 : count + 2])):
            return False

        # Check whether a 2-1 split would be OK.
        if (Branch._is_full_case(items, count - 2) and
          Branch._is_folded(items[count - 2 : count + 1])):
            return False

        return True

    @staticmethod
    def _can_split_rev(items, count):
        end = len(items)

        # Check the characters either side of the proposed split.
        if not Branch._is_full_case(items, end - count):
            return True

        if not Branch._is_full_case(items, end - count - 1):
            return True

        # Check whether a 1-1 split would be OK.
        if Branch._is_folded(items[end - count - 1 : end - count + 1]):
            return False

        # Check whether a 1-2 split would be OK.
        if (Branch._is_full_case(items, end - count + 2) and
          Branch._is_folded(items[end - count - 1 : end - count + 2])):
            return False

        # Check whether a 2-1 split would be OK.
        if (Branch._is_full_case(items, end - count - 2) and
          Branch._is_folded(items[end - count - 2 : end - count + 1])):
            return False

        return True

    @staticmethod
    def _merge_common_prefixes(info, branches):
        # Branches with the same case-sensitive character prefix can be grouped
        # together if they are separated only by other branches with a
        # character prefix.
        prefixed = defaultdict(list)
        order = {}
        new_branches = []
        for b in branches:
            if Branch._is_simple_character(b):
                # Branch starts with a simple character.
                prefixed[b.value].append([b])
                order.setdefault(b.value, len(order))
            elif (isinstance(b, Sequence) and b.items and
              Branch._is_simple_character(b.items[0])):
                # Branch starts with a simple character.
                prefixed[b.items[0].value].append(b.items)
                order.setdefault(b.items[0].value, len(order))
            else:
                Branch._flush_char_prefix(info, prefixed, order, new_branches)

                new_branches.append(b)

        Branch._flush_char_prefix(info, prefixed, order, new_branches)

        return new_branches

    @staticmethod
    def _is_simple_character(c):
        return isinstance(c, Character) and c.positive and not c.case_flags

    @staticmethod
    def _reduce_to_set(info, branches):
        # Can the branches be reduced to a set?
        new_branches = []
        items = set()
        case_flags = NOCASE
        for b in branches:
            if isinstance(b, (Character, Property, SetBase)):
                # Branch starts with a single character.
                if b.case_flags != case_flags:
                    # Different case sensitivity, so flush.
                    Branch._flush_set_members(info, items, case_flags,
                      new_branches)

                    case_flags = b.case_flags

                items.add(b.with_flags(case_flags=NOCASE))
            else:
                Branch._flush_set_members(info, items, case_flags,
                  new_branches)

                new_branches.append(b)

        Branch._flush_set_members(info, items, case_flags, new_branches)

        return new_branches

    @staticmethod
    def _flush_char_prefix(info, prefixed, order, new_branches):
        # Flush the prefixed branches.
        if not prefixed:
            return

        for value, branches in sorted(prefixed.items(), key=lambda pair:
          order[pair[0]]):
            if len(branches) == 1:
                new_branches.append(make_sequence(branches[0]))
            else:
                subbranches = []
                optional = False
                for b in branches:
                    if len(b) > 1:
                        subbranches.append(make_sequence(b[1 : ]))
                    elif not optional:
                        subbranches.append(Sequence())
                        optional = True

                sequence = Sequence([Character(value), Branch(subbranches)])
                new_branches.append(sequence.optimise(info))

        prefixed.clear()
        order.clear()

    @staticmethod
    def _flush_set_members(info, items, case_flags, new_branches):
        # Flush the set members.
        if not items:
            return

        if len(items) == 1:
            item = list(items)[0]
        else:
            item = SetUnion(list(items)).optimise(info)

        new_branches.append(item.with_flags(case_flags=case_flags))

        items.clear()

    @staticmethod
    def _is_full_case(items, i):
        if not 0 <= i < len(items):
            return False

        return isinstance(i, Character) and i.positive and (i.case_flags &
          FULLIGNORECASE) == FULLIGNORECASE

    @staticmethod
    def _is_folded(items):
        if len(items) < 2:
            return False

        for i in items:
            if (not isinstance(i, Character) or not i.positive or not
              i.case_flags):
                return False

        folded = u"".join(unichr(i.value) for i in items)
        folded = _regex.fold_case(FULL_CASE_FOLDING, folded)

        # Get the characters which expand to multiple codepoints on folding.
        expanding_chars = _regex.get_expand_on_folding()

        for c in expanding_chars:
            if folded == _regex.fold_case(FULL_CASE_FOLDING, c):
                return True

        return False

    def is_empty(self):
        return all(b.is_empty() for b in self.branches)

    def __eq__(self, other):
        return type(self) is type(other) and self.branches == other.branches

class CallGroup(RegexBase):
    def __init__(self, info, group):
        RegexBase.__init__(self)
        self.info = info
        self.group = group

        self._key = self.__class__, self.group

    def fix_groups(self):
        try:
            self.group = int(self.group)
        except ValueError:
            try:
                self.group = self.info.group_index[self.group]
            except KeyError:
                raise error("unknown group")

        if not 0 <= self.group <= self.info.group_count:
            raise error("unknown group")

        self._key = self.__class__, self.group

    def remove_captures(self):
        raise error("group reference not allowed")

    def compile(self, reverse=False, fuzzy=False):
        return [(OP.GROUP_CALL, self.group)]

    def dump(self, indent=0, reverse=False):
        print "%sGROUP_CALL %s" % (INDENT * indent, self.group)

    def __eq__(self, other):
        return type(self) is type(other) and self.group == other.group

class Character(RegexBase):
    _opcode = {(NOCASE, False): OP.CHARACTER, (IGNORECASE, False):
      OP.CHARACTER_IGN, (FULLCASE, False): OP.CHARACTER, (FULLIGNORECASE,
      False): OP.CHARACTER_IGN, (NOCASE, True): OP.CHARACTER_REV, (IGNORECASE,
      True): OP.CHARACTER_IGN_REV, (FULLCASE, True): OP.CHARACTER_REV,
      (FULLIGNORECASE, True): OP.CHARACTER_IGN_REV}
    _op_name = {False: "CHARACTER", True: "CHARACTER_REV"}

    def __init__(self, value, positive=True, case_flags=NOCASE,
      zerowidth=False):
        RegexBase.__init__(self)
        self.value = value
        self.positive = bool(positive)
        self.case_flags = case_flags
        self.zerowidth = bool(zerowidth)

        self._key = (self.__class__, self.value, self.positive,
          self.case_flags, self.zerowidth)

    def rebuild(self, positive, case_flags, zerowidth):
        return Character(self.value, positive, case_flags, zerowidth)

    def optimise(self, info, in_set=False):
        return self

    def get_firstset(self, reverse):
        return set([self])

    def has_simple_start(self):
        return True

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP

        code = PrecompiledCode([self._opcode[self.case_flags, reverse], flags,
          self.value])

        if (self.positive and (self.case_flags & FULLIGNORECASE) ==
          FULLIGNORECASE):
            # Get the folded character.
            folded = _regex.fold_case(FULL_CASE_FOLDING, unichr(self.value))
            if len(folded) > 1:
                # The character expands on full case-folding.
                code = Branch([code, String([ord(c) for c in folded],
                  case_flags=self.case_flags)])

        return code.compile(reverse, fuzzy)

    def dump(self, indent=0, reverse=False):
        print "%s%s %s %s%s" % (INDENT * indent, self._op_name[reverse],
          POS_TEXT[self.positive], self.value, CASE_TEXT[self.case_flags])

    def matches(self, ch):
        return (ch == self.value) == self.positive

class Conditional(RegexBase):
    def __new__(cls, info, group, yes_item, no_item):
        if yes_item.is_empty() and no_item.is_empty():
            return Sequence()

        return RegexBase.__new__(cls)

    def __init__(self, info, group, yes_item, no_item):
        RegexBase.__init__(self)
        self.info = info
        self.group = group
        self.yes_item = yes_item
        self.no_item = no_item

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

    def remove_captures(self):
        self.yes_item = self.yes_item.remove_captures()
        self.no_item = self.no_item.remove_captures()

    def is_atomic(self):
        return self.yes_item.is_atomic() and self.no_item.is_atomic()

    def can_be_affix(self):
        return self.yes_item.can_be_affix() and self.no_item.can_be_affix()

    def contains_group(self):
        return self.yes_item.contains_group() or self.no_item.contains_group()

    def get_firstset(self, reverse):
        return (self.yes_item.get_firstset(reverse) |
          self.no_item.get_firstset(reverse))

    def compile(self, reverse=False, fuzzy=False):
        code = [(OP.GROUP_EXISTS, self.group)]
        code.extend(self.yes_item.compile(reverse, fuzzy))
        add_code = self.no_item.compile(reverse, fuzzy)
        if add_code:
            code.append((OP.NEXT, ))
            code.extend(add_code)

        code.append((OP.END, ))

        return code

    def dump(self, indent=0, reverse=False):
        print "%sGROUP_EXISTS %s" % (INDENT * indent, self.group)
        self.yes_item.dump(indent + 1, reverse)
        if self.no_item:
            print "%sOR" % (INDENT * indent)
            self.no_item.dump(indent + 1, reverse)

    def is_empty(self):
        return self.yes_item.is_empty() and self.no_item.is_empty()

    def __eq__(self, other):
        return type(self) is type(other) and (self.group, self.yes_item,
          self.no_item) == (other.group, other.yes_item, other.no_item)

class DefaultBoundary(ZeroWidthBase):
    _opcode = OP.DEFAULT_BOUNDARY
    _op_name = "DEFAULT_BOUNDARY"

class DefaultEndOfWord(ZeroWidthBase):
    _opcode = OP.DEFAULT_END_OF_WORD
    _op_name = "DEFAULT_END_OF_WORD"

class DefaultStartOfWord(ZeroWidthBase):
    _opcode = OP.DEFAULT_START_OF_WORD
    _op_name = "DEFAULT_START_OF_WORD"

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

class EndOfWord(ZeroWidthBase):
    _opcode = OP.END_OF_WORD
    _op_name = "END_OF_WORD"

class Fuzzy(RegexBase):
    def __init__(self, subpattern, constraints=None):
        RegexBase.__init__(self)
        if constraints is None:
            constraints = {}
        self.subpattern = subpattern
        self.constraints = constraints

        # If an error type is mentioned in the cost equation, then its maximum
        # defaults to unlimited.
        if "cost" in constraints:
            for e in "dis":
                if e in constraints["cost"]:
                    constraints.setdefault(e, (0, None))

        # If any error type is mentioned, then all the error maxima default to
        # 0, otherwise they default to unlimited.
        if set(constraints) & set("dis"):
            for e in "dis":
                constraints.setdefault(e, (0, 0))
        else:
            for e in "dis":
                constraints.setdefault(e, (0, None))

        # The maximum of the generic error type defaults to unlimited.
        constraints.setdefault("e", (0, None))

        # The cost equation defaults to equal costs. Also, the cost of any
        # error type not mentioned in the cost equation defaults to 0.
        if "cost" in constraints:
            for e in "dis":
                constraints["cost"].setdefault(e, 0)
        else:
            constraints["cost"] = {"d": 1, "i": 1, "s": 1, "max":
              constraints["e"][1]}

    def fix_groups(self):
        self.subpattern.fix_groups()

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def remove_captures(self):
        self.subpattern = self.subpattern.remove_captures()
        return self

    def is_atomic(self):
        return self.subpattern.is_atomic()

    def contains_group(self):
        return self.subpattern.contains_group()

    def compile(self, reverse=False, fuzzy=False):
        # The individual limits.
        arguments = []
        for e in "dise":
            v = self.constraints[e]
            arguments.append(v[0])
            arguments.append(UNLIMITED if v[1] is None else v[1])

        # The coeffs of the cost equation.
        for e in "dis":
            arguments.append(self.constraints["cost"][e])

        # The maximum of the cost equation.
        v = self.constraints["cost"]["max"]
        arguments.append(UNLIMITED if v is None else v)

        flags = 0
        if reverse:
            flags |= REVERSE_OP

        return ([(OP.FUZZY, flags) + tuple(arguments)] +
          self.subpattern.compile(reverse, True) + [(OP.END,)])

    def dump(self, indent=0, reverse=False):
        print "%sFUZZY" % (INDENT * indent)
        self.subpattern.dump(indent + 1, reverse)

    def is_empty(self):
        return self.subpattern.is_empty()

    def __eq__(self, other):
        return (type(self) is type(other) and self.subpattern ==
          other.subpattern)

class Grapheme(RegexBase):
    _op_name = {False: "GRAPHEME", True: "GRAPHEME_REV"}

    def compile(self, reverse=False, fuzzy=False):
        # Match at least 1 character until a grapheme boundary is reached. Note
        # that this is the same whether matching forwards or backwards.
        character_matcher = LazyRepeat(AnyAll(), 1, None).compile(reverse,
          fuzzy)
        boundary_matcher = [(OP.GRAPHEME_BOUNDARY, 1)]

        return character_matcher + boundary_matcher

    def dump(self, indent=0, reverse=False):
        print "%s%s" % (INDENT * indent, self._op_name[reverse])

class GreedyRepeat(RegexBase):
    _opcode = OP.GREEDY_REPEAT
    _op_name = "GREEDY_REPEAT"

    def __init__(self, subpattern, min_count, max_count):
        RegexBase.__init__(self)
        self.subpattern = subpattern
        self.min_count = min_count
        self.max_count = max_count

    def fix_groups(self):
        self.subpattern.fix_groups()

    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)

        return type(self)(subpattern, self.min_count, self.max_count)

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def remove_captures(self):
        self.subpattern = self.subpattern.remove_captures()
        return self

    def is_atomic(self):
        return self.min_count == self.max_count and self.subpattern.is_atomic()

    def contains_group(self):
        return self.subpattern.contains_group()

    def get_firstset(self, reverse):
        fs = self.subpattern.get_firstset(reverse)
        if self.min_count == 0:
            fs.add(None)

        return fs

    def compile(self, reverse=False, fuzzy=False):
        repeat = [self._opcode, self.min_count]
        if self.max_count is None:
            repeat.append(UNLIMITED)
        else:
            repeat.append(self.max_count)

        return ([tuple(repeat)] + self.subpattern.compile(reverse, fuzzy) +
          [(OP.END, )])

    def dump(self, indent=0, reverse=False):
        if self.max_count is None:
            limit = "INF"
        else:
            limit = self.max_count
        print "%s%s %s %s" % (INDENT * indent, self._op_name, self.min_count,
          limit)

        self.subpattern.dump(indent + 1, reverse)

    def is_empty(self):
        return self.subpattern.is_empty()

    def __eq__(self, other):
        return type(self) is type(other) and (self.subpattern, self.min_count,
          self.max_count) == (other.subpattern, other.min_count,
          other.max_count)

class Group(RegexBase):
    def __init__(self, info, group, subpattern):
        RegexBase.__init__(self)
        self.info = info
        self.group = group
        self.subpattern = subpattern

    def fix_groups(self):
        self.subpattern.fix_groups()

    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)

        return Group(self.info, self.group, subpattern)

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def remove_captures(self):
        return self.subpattern.remove_captures()

    def is_atomic(self):
        return self.subpattern.is_atomic()

    def can_be_affix(self):
        return False

    def contains_group(self):
        return True

    def get_firstset(self, reverse):
        return self.subpattern.get_firstset(reverse)

    def has_simple_start(self):
        return self.subpattern.has_simple_start()

    def compile(self, reverse=False, fuzzy=False):
        return [(OP.GROUP, self.group)] + self.subpattern.compile(reverse,
          fuzzy) + [(OP.END, )]

    def dump(self, indent=0, reverse=False):
        print "%sGROUP %s" % (INDENT * indent, self.group)
        self.subpattern.dump(indent + 1, reverse)

    def __eq__(self, other):
        return (type(self) is type(other) and (self.group, self.subpattern) ==
          (other.group, other.subpattern))

class LazyRepeat(GreedyRepeat):
    _opcode = OP.LAZY_REPEAT
    _op_name = "LAZY_REPEAT"

class LookAround(RegexBase):
    _dir_text = {False: "AHEAD", True: "BEHIND"}

    def __new__(cls, behind, positive, subpattern):
        if positive and subpattern.is_empty():
            return subpattern

        return RegexBase.__new__(cls)

    def __init__(self, behind, positive, subpattern):
        RegexBase.__init__(self)
        self.behind = bool(behind)
        self.positive = bool(positive)
        self.subpattern = subpattern

    def fix_groups(self):
        self.subpattern.fix_groups()

    def optimise(self, info):
        subpattern = self.subpattern.optimise(info)

        return LookAround(self.behind, self.positive, subpattern)

    def pack_characters(self, info):
        self.subpattern = self.subpattern.pack_characters(info)
        return self

    def remove_captures(self):
        return self.subpattern.remove_captures()

    def is_atomic(self):
        return self.subpattern.is_atomic()

    def can_be_affix(self):
        return self.subpattern.can_be_affix()

    def contains_group(self):
        return self.subpattern.contains_group()

    def compile(self, reverse=False, fuzzy=False):
        return ([(OP.LOOKAROUND, int(self.positive), int(not self.behind))] +
          self.subpattern.compile(self.behind) + [(OP.END, )])

    def dump(self, indent=0, reverse=False):
        print "%sLOOKAROUND %s %s" % (INDENT * indent,
          self._dir_text[self.behind], POS_TEXT[self.positive])
        self.subpattern.dump(indent + 1, self.behind)

    def is_empty(self):
        return self.subpattern.is_empty()

    def __eq__(self, other):
        return type(self) is type(other) and (self.behind, self.positive,
          self.subpattern) == (other.behind, other.positive, other.subpattern)

class PrecompiledCode(RegexBase):
    def __init__(self, code):
        self.code = code

    def compile(self, reverse=False, fuzzy=False):
        return [tuple(self.code)]

class Property(RegexBase):
    _opcode = {(NOCASE, False): OP.PROPERTY, (IGNORECASE, False):
      OP.PROPERTY_IGN, (FULLCASE, False): OP.PROPERTY, (FULLIGNORECASE, False):
      OP.PROPERTY_IGN, (NOCASE, True): OP.PROPERTY_REV, (IGNORECASE, True):
      OP.PROPERTY_IGN_REV, (FULLCASE, True): OP.PROPERTY_REV, (FULLIGNORECASE,
      True): OP.PROPERTY_IGN_REV}
    _op_name = {False: "PROPERTY", True: "PROPERTY_REV"}

    def __init__(self, value, positive=True, case_flags=NOCASE,
      zerowidth=False):
        RegexBase.__init__(self)
        self.value = value
        self.positive = bool(positive)
        self.case_flags = case_flags
        self.zerowidth = bool(zerowidth)

        self._key = (self.__class__, self.value, self.positive,
          self.case_flags, self.zerowidth)

    def rebuild(self, positive, case_flags, zerowidth):
        return Property(self.value, positive, case_flags, zerowidth)

    def optimise(self, info, in_set=False):
        return self

    def get_firstset(self, reverse):
        return set([self])

    def has_simple_start(self):
        return True

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[self.case_flags, reverse], flags, self.value)]

    def dump(self, indent=0, reverse=False):
        print "%s%s %s %s=%s%s" % (INDENT * indent, self._op_name[reverse],
          POS_TEXT[self.positive], self.value >> 16, self.value & 0xFFFF,
          CASE_TEXT[self.case_flags])

    def matches(self, ch):
        return _regex.has_property_value(self.value, ch) == self.positive

class Range(RegexBase):
    _opcode = {(NOCASE, False): OP.RANGE, (IGNORECASE, False): OP.RANGE_IGN,
      (FULLCASE, False): OP.RANGE, (FULLIGNORECASE, False): OP.RANGE_IGN,
      (NOCASE, True): OP.RANGE_REV, (IGNORECASE, True): OP.RANGE_IGN_REV,
      (FULLCASE, True): OP.RANGE_REV, (FULLIGNORECASE, True): OP.RANGE_IGN_REV}
    _op_name = {False: "RANGE", True: "RANGE_REV"}

    def __init__(self, lower, upper, positive=True, case_flags=NOCASE,
      zerowidth=False):
        RegexBase.__init__(self)
        self.lower = lower
        self.upper = upper
        self.positive = bool(positive)
        self.case_flags = case_flags
        self.zerowidth = bool(zerowidth)

        self._key = (self.__class__, self.lower, self.upper, self.positive,
          self.case_flags, self.zerowidth)

    def rebuild(self, positive, case_flags, zerowidth):
        return Range(self.lower, self.upper, positive, case_flags, zerowidth)

    def optimise(self, info, in_set=False):
        # Is the range case-sensitive?
        if not self.positive or not (self.case_flags & IGNORECASE) or in_set:
            return self

        # Is full case-folding possible?
        if (not (info.flags & UNICODE) or (self.case_flags & FULLIGNORECASE) !=
          FULLIGNORECASE):
            return self

        # Get the characters which expand to multiple codepoints on folding.
        expanding_chars = _regex.get_expand_on_folding()

        # Get the folded characters in the range.
        items = []
        for ch in expanding_chars:
            if self.lower <= ord(ch) <= self.upper:
                folded = _regex.fold_case(FULL_CASE_FOLDING, ch)
                items.append(String([ord(c) for c in folded],
                  case_flags=self.case_flags))

        if not items:
            # We can fall back to simple case-folding.
            return self

        if len(items) < self.upper - self.lower + 1:
            # Not all the characters are covered by the full case-folding.
            items.insert(0, self)

        return Branch(items)

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[self.case_flags, reverse], flags, self.lower,
          self.upper)]

    def dump(self, indent=0, reverse=False):
        print "%sRANGE %s %s..%s%s" % (INDENT * indent,
          POS_TEXT[self.positive], self.lower, self.upper,
          CASE_TEXT[self.case_flags])

    def matches(self, ch):
        return (self.lower <= ch <= self.upper) == self.positive

class RefGroup(RegexBase):
    _opcode = {(NOCASE, False): OP.REF_GROUP, (IGNORECASE, False):
      OP.REF_GROUP_IGN, (FULLCASE, False): OP.REF_GROUP, (FULLIGNORECASE,
      False): OP.REF_GROUP_FLD, (NOCASE, True): OP.REF_GROUP_REV, (IGNORECASE,
      True): OP.REF_GROUP_IGN_REV, (FULLCASE, True): OP.REF_GROUP_REV,
      (FULLIGNORECASE, True): OP.REF_GROUP_FLD_REV}
    _op_name = {False: "REF_GROUP", True: "REF_GROUP_REV"}

    def __init__(self, info, group, case_flags=NOCASE):
        RegexBase.__init__(self)
        self.info = info
        self.group = group
        self.case_flags = case_flags

        self._key = self.__class__, self.group, self.case_flags

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

        self._key = self.__class__, self.group, self.case_flags

    def remove_captures(self):
        raise error("group reference not allowed")

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[self.case_flags, reverse], flags, self.group)]

    def dump(self, indent=0, reverse=False):
        print "%s%s %s%s" % (INDENT * indent, self._op_name[reverse],
          self.group, CASE_TEXT[self.case_flags])

class SearchAnchor(ZeroWidthBase):
    _opcode = OP.SEARCH_ANCHOR
    _op_name = "SEARCH_ANCHOR"

class Sequence(RegexBase):
    def __init__(self, items=None):
        RegexBase.__init__(self)
        if items is None:
            items = []

        self.items = items

    def fix_groups(self):
        for s in self.items:
            s.fix_groups()

    def optimise(self, info):
        # Flatten the sequences.
        items = []
        for s in self.items:
            s = s.optimise(info)
            if isinstance(s, Sequence):
                items.extend(s.items)
            else:
                items.append(s)

        return make_sequence(items)

    def pack_characters(self, info):
        "Packs sequences of characters into strings."
        items = []
        characters = []
        case_flags = NOCASE
        for s in self.items:
            if type(s) is Character and s.positive:
                if s.case_flags != case_flags:
                    # Different case sensitivity, so flush, unless neither the
                    # previous nor the new character are cased.
                    if case_flags or is_cased(info, s.value):
                        Sequence._flush_characters(info, characters,
                          case_flags, items)

                        case_flags = s.case_flags

                characters.append(s.value)
            elif type(s) is String:
                if s.case_flags != case_flags:
                    # Different case sensitivity, so flush, unless the neither
                    # the previous nor the new string are cased.
                    if not s.case_flags or any(is_cased(info, c) for c in
                      characters):
                        Sequence._flush_characters(info, characters,
                          case_flags, items)

                        case_flags = s.case_flags

                characters.extend(s.characters)
            else:
                Sequence._flush_characters(info, characters, case_flags, items)

                items.append(s.pack_characters(info))

        Sequence._flush_characters(info, characters, case_flags, items)

        return make_sequence(items)

    def remove_captures(self):
        self.items = [s.remove_captures() for s in self.items]
        return self

    def is_atomic(self):
        return all(s.is_atomic() for s in self.items)

    def can_be_affix(self):
        return False

    def contains_group(self):
        return any(s.contains_group() for s in self.items)

    def get_firstset(self, reverse):
        fs = set()
        items = self.items
        if reverse:
            items.reverse()
        for s in items:
            fs |= s.get_firstset(reverse)
            if None not in fs:
                return fs
            fs.discard(None)

        return fs | set([None])

    def has_simple_start(self):
        return self.items and self.items[0].has_simple_start()

    def compile(self, reverse=False, fuzzy=False):
        seq = self.items
        if reverse:
            seq = seq[::-1]

        code = []
        for s in seq:
            code.extend(s.compile(reverse, fuzzy))

        return code

    def dump(self, indent=0, reverse=False):
        for s in self.items:
            s.dump(indent, reverse)

    @staticmethod
    def _flush_characters(info, characters, case_flags, items):
        if not characters:
            return

        # Disregard case_flags if all of the characters are case-less.
        if case_flags & IGNORECASE:
            if not any(is_cased(info, c) for c in characters):
                case_flags = NOCASE

        if len(characters) == 1:
            items.append(Character(characters[0], case_flags=case_flags))
        else:
            items.append(String(characters, case_flags=case_flags))

        characters[:] = []

    def is_empty(self):
        return all(i.is_empty() for i in self.items)

    def __eq__(self, other):
        return type(self) is type(other) and self.items == other.items

class SetBase(RegexBase):
    def __init__(self, items, positive=True, case_flags=NOCASE,
      zerowidth=False):
        RegexBase.__init__(self)
        self.items = tuple(items)
        self.positive = bool(positive)
        self.case_flags = case_flags
        self.zerowidth = bool(zerowidth)

        self._key = (self.__class__, self.items, self.positive,
          self.case_flags, self.zerowidth)

    def rebuild(self, positive, case_flags, zerowidth):
        return type(self)(self.items, positive, case_flags, zerowidth)

    def get_firstset(self, reverse):
        return set([self])

    def has_simple_start(self):
        return True

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP
        code = [(self._opcode[self.case_flags, reverse], flags)]
        for m in self.items:
            code.extend(m.compile())

        code.append((OP.END, ))

        return code

    def dump(self, indent=0, reverse=False):
        print "%s%s %s %s%s" % (INDENT * indent, self._op_name[reverse],
          POS_TEXT[self.positive], CASE_TEXT[self.case_flags])
        for i in self.items:
            i.dump(indent + 1)

    def _handle_case_folding(self, info, in_set):
        # Is the set case-sensitive?
        if not self.positive or not (self.case_flags & IGNORECASE) or in_set:
            return self

        # Is full case-folding possible?
        if (not (info.flags & UNICODE) or (self.case_flags & FULLIGNORECASE) !=
          FULLIGNORECASE):
            return self

        # Get the characters which expand to multiple codepoints on folding.
        expanding_chars = _regex.get_expand_on_folding()

        # Get the folded characters in the set.
        items = []
        seen = set()
        for ch in expanding_chars:
            if self.matches(ord(ch)):
                folded = _regex.fold_case(FULL_CASE_FOLDING, ch)
                if folded not in seen:
                    items.append(String([ord(c) for c in folded],
                      case_flags=self.case_flags))
                    seen.add(folded)

        if not items:
            # We can fall back to simple case-folding.
            return self

        return Branch([self] + items)

class SetDiff(SetBase):
    _opcode = {(NOCASE, False): OP.SET_DIFF, (IGNORECASE, False):
      OP.SET_DIFF_IGN, (FULLCASE, False): OP.SET_DIFF, (FULLIGNORECASE, False):
      OP.SET_DIFF_IGN, (NOCASE, True): OP.SET_DIFF_REV, (IGNORECASE, True):
      OP.SET_DIFF_IGN_REV, (FULLCASE, True): OP.SET_DIFF_REV, (FULLIGNORECASE,
      True): OP.SET_DIFF_IGN_REV}
    _op_name = {False: "SET_DIFF", True: "SET_DIFF_REV"}

    def optimise(self, info, in_set=False):
        items = self.items
        if len(items) > 2:
            items = [items[0], SetUnion(items[1 : ])]

        if len(items) == 1:
            return items[0].with_flags(case_flags=self.case_flags,
              zerowidth=self.zerowidth).optimise(info, in_set)

        self.items = tuple(m.optimise(info, in_set=True) for m in items)

        return self._handle_case_folding(info, in_set)

    def matches(self, ch):
        m = self.items[0].matches(ch) and not self.items[1].matches(ch)
        return m == self.positive

class SetInter(SetBase):
    _opcode = {(NOCASE, False): OP.SET_INTER, (IGNORECASE, False):
      OP.SET_INTER_IGN, (FULLCASE, False): OP.SET_INTER, (FULLIGNORECASE,
      False): OP.SET_INTER_IGN, (NOCASE, True): OP.SET_INTER_REV, (IGNORECASE,
      True): OP.SET_INTER_IGN_REV, (FULLCASE, True): OP.SET_INTER_REV,
      (FULLIGNORECASE, True): OP.SET_INTER_IGN_REV}
    _op_name = {False: "SET_INTER", True: "SET_INTER_REV"}

    def optimise(self, info, in_set=False):
        items = []
        for m in self.items:
            m = m.optimise(info, in_set=True)
            if isinstance(m, SetInter) and m.positive:
                # Intersection in intersection.
                items.extend(m.items)
            else:
                items.append(m)

        if len(items) == 1:
            return items[0].with_flags(case_flags=self.case_flags,
              zerowidth=self.zerowidth).optimise(info, in_set)

        self.items = tuple(items)

        return self._handle_case_folding(info, in_set)

    def matches(self, ch):
        m = all(i.matches(ch) for i in self.items)
        return m == self.positive

class SetSymDiff(SetBase):
    _opcode = {(NOCASE, False): OP.SET_SYM_DIFF, (IGNORECASE, False):
      OP.SET_SYM_DIFF_IGN, (FULLCASE, False): OP.SET_SYM_DIFF, (FULLIGNORECASE,
      False): OP.SET_SYM_DIFF_IGN, (NOCASE, True): OP.SET_SYM_DIFF_REV,
      (IGNORECASE, True): OP.SET_SYM_DIFF_IGN_REV, (FULLCASE, True):
      OP.SET_SYM_DIFF_REV, (FULLIGNORECASE, True): OP.SET_SYM_DIFF_IGN_REV}
    _op_name = {False: "SET_SYM_DIFF", True: "SET_SYM_DIFF_REV"}

    def optimise(self, info, in_set=False):
        items = []
        for m in self.items:
            m = m.optimise(info, in_set=True)
            if isinstance(m, SetSymDiff) and m.positive:
                # Symmetric difference in symmetric difference.
                items.extend(m.items)
            else:
                items.append(m)

        if len(items) == 1:
            return items[0].with_flags(case_flags=self.case_flags,
              zerowidth=self.zerowidth).optimise(info, in_set)

        self.items = tuple(items)

        return self._handle_case_folding(info, in_set)

    def matches(self, ch):
        m = False
        for i in self.items:
            m = m != i.matches(ch)

        return m == self.positive

class SetUnion(SetBase):
    _opcode = {(NOCASE, False): OP.SET_UNION, (IGNORECASE, False):
      OP.SET_UNION_IGN, (FULLCASE, False): OP.SET_UNION, (FULLIGNORECASE,
      False): OP.SET_UNION_IGN, (NOCASE, True): OP.SET_UNION_REV, (IGNORECASE,
      True): OP.SET_UNION_IGN_REV, (FULLCASE, True): OP.SET_UNION_REV,
      (FULLIGNORECASE, True): OP.SET_UNION_IGN_REV}
    _op_name = {False: "SET_UNION", True: "SET_UNION_REV"}

    def optimise(self, info, in_set=False):
        items = []
        for m in self.items:
            m = m.optimise(info, in_set=True)
            if isinstance(m, SetUnion) and m.positive:
                # Union in union.
                items.extend(m.items)
            else:
                items.append(m)

        if len(items) == 1:
            i = items[0]
            return i.with_flags(positive=i.positive == self.positive,
              case_flags=self.case_flags,
              zerowidth=self.zerowidth).optimise(info, in_set)

        self.items = tuple(items)

        return self._handle_case_folding(info, in_set)

    def compile(self, reverse=False, fuzzy=False):
        flags = 0
        if self.positive:
            flags |= POSITIVE_OP
        if self.zerowidth:
            flags |= ZEROWIDTH_OP
        if fuzzy:
            flags |= FUZZY_OP

        characters, others = defaultdict(list), []
        for m in self.items:
            if isinstance(m, Character):
                characters[m.positive].append(m.value)
            else:
                others.append(m)

        code = [(self._opcode[self.case_flags, reverse], flags)]

        for positive, values in characters.items():
            flags = 0
            if positive:
                flags |= POSITIVE_OP
            if len(values) == 1:
                code.append((OP.CHARACTER, flags, values[0]))
            else:
                code.append((OP.STRING, flags, len(values)) + tuple(values))

        for m in others:
            code.extend(m.compile())

        code.append((OP.END, ))

        return code

    def matches(self, ch):
        m = any(i.matches(ch) for i in self.items)
        return m == self.positive

class StartOfLine(ZeroWidthBase):
    _opcode = OP.START_OF_LINE
    _op_name = "START_OF_LINE"

class StartOfLineU(StartOfLine):
    _opcode = OP.START_OF_LINE_U
    _op_name = "START_OF_LINE_U"

class StartOfString(ZeroWidthBase):
    _opcode = OP.START_OF_STRING
    _op_name = "START_OF_STRING"

class StartOfWord(ZeroWidthBase):
    _opcode = OP.START_OF_WORD
    _op_name = "START_OF_WORD"

class String(RegexBase):
    _opcode = {(NOCASE, False): OP.STRING, (IGNORECASE, False): OP.STRING_IGN,
      (FULLCASE, False): OP.STRING, (FULLIGNORECASE, False): OP.STRING_FLD,
      (NOCASE, True): OP.STRING_REV, (IGNORECASE, True): OP.STRING_IGN_REV,
      (FULLCASE, True): OP.STRING_REV, (FULLIGNORECASE, True):
      OP.STRING_FLD_REV}
    _op_name = {False: "STRING", True: "STRING_REV"}

    def __init__(self, characters, case_flags=NOCASE):
        self.characters = tuple(characters)
        self.case_flags = case_flags

        self._key = self.__class__, self.characters, self.case_flags

    def get_firstset(self, reverse):
        if reverse:
            pos = -1
        else:
            pos = 0
        return set([Character(self.characters[pos],
          case_flags=self.case_flags)])

    def has_simple_start(self):
        return True

    def compile(self, reverse=False, fuzzy=False):
        if (self.case_flags & FULLIGNORECASE) == FULLIGNORECASE:
            characters = []
            for char in self.characters:
                folded = _regex.fold_case(FULL_CASE_FOLDING, unichr(char))
                characters.extend(ord(c) for c in folded)
        else:
            characters = self.characters

        flags = 0
        if fuzzy:
            flags |= FUZZY_OP
        return [(self._opcode[self.case_flags, reverse], flags,
          len(characters)) + tuple(characters)]

    def dump(self, indent=0, reverse=False):
        print "%s%s %s%s" % (INDENT * indent, self._op_name[reverse],
          " ".join(map(str, self.characters)), CASE_TEXT[self.case_flags])

class StringSet(RegexBase):
    _opcode = {(NOCASE, False): OP.STRING_SET, (IGNORECASE, False):
      OP.STRING_SET_IGN, (FULLCASE, False): OP.STRING_SET, (FULLIGNORECASE,
      False): OP.STRING_SET_FLD, (NOCASE, True): OP.STRING_SET_REV,
      (IGNORECASE, True): OP.STRING_SET_IGN_REV, (FULLCASE, True):
      OP.STRING_SET_REV, (FULLIGNORECASE, True): OP.STRING_SET_FLD_REV}
    _op_name = {False: "STRING_SET", True: "STRING_SET_REV"}

    def __init__(self, info, name, case_flags=NOCASE):
        self.info = info
        self.name = name
        self.case_flags = case_flags

        self._key = self.__class__, self.name, self.case_flags

        self.set_key = (name, self.case_flags)
        if self.set_key not in info.named_lists_used:
            info.named_lists_used[self.set_key] = len(info.named_lists_used)

    def compile(self, reverse=False, fuzzy=False):
        index = self.info.named_lists_used[self.set_key]
        items = self.info.kwargs[self.name]

        case_flags = self.case_flags

        if fuzzy or (case_flags & IGNORECASE):
            encoding = self.info.flags & ALL_ENCODINGS
            fold_flags = encoding | case_flags

            branches = []
            for i in items:
                string = []
                if isinstance(i, unicode):
                    string = [ord(c) for c in _regex.fold_case(fold_flags, i)]
                else:
                    string = [ord(c) for c in i]

                branches.append(Sequence([Character(c, case_flags=case_flags)
                  for c in string]))

            if len(branches) > 1:
                branch = Branch(branches)
            else:
                branch = branches[0]
            branch = branch.optimise(self.info).pack_characters(self.info)

            return branch.compile(reverse, fuzzy)
        else:
            min_len = min(len(i) for i in items)
            max_len = max(len(i) for i in items)

            return [(self._opcode[case_flags, reverse], index, min_len,
              max_len)]

    def dump(self, indent=0, reverse=False):
        print "%s%s %s%s" % (INDENT * indent, self._op_name[reverse],
          self.name, CASE_TEXT[self.case_flags])

    def _flatten(self, s):
        # Flattens the branches.
        if isinstance(s, Branch):
            for b in s.branches:
                self._flatten(b)
        elif isinstance(s, Sequence) and s.items:
            seq = s.items

            while isinstance(seq[-1], Sequence):
                seq[-1 : ] = seq[-1].items

            n = 0
            while n < len(seq) and isinstance(seq[n], Character):
                n += 1

            if n > 1:
                seq[ : n] = [String([c.value for c in seq[ : n]],
                  case_flags=self.case_flags)]

            self._flatten(seq[-1])

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
                pos = self.pos

                for c in substring:
                    # Ignore any whitespace before the character.
                    while self.string[pos].isspace():
                        pos += 1

                    # Does the character match?
                    if c != self.string[pos]:
                        return False

                    pos += 1

                self.pos = pos
                return True
            else:
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

class Info(object):
    "Info about the regular expression."
    OPEN = "OPEN"
    CLOSED = "CLOSED"

    def __init__(self, flags=0, char_type=None, kwargs={}):
        self.flags = flags
        self.global_flags = flags

        self.kwargs = kwargs

        self.group_count = 0
        self.group_index = {}
        self.group_name = {}
        self.used_groups = set()
        self.group_state = {}
        self.char_type = char_type
        self.named_lists_used = {}

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
        # In version 1, a group reference can refer to an open group. We'll
        # just pretend the group isn't open.
        version = (self.flags & ALL_VERSIONS) or DEFAULT_VERSION
        if version == VERSION1:
            return False

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
            source = Source(phrase)
            info = Info(flags, source.char_type)
            source.ignore_space = bool(info.flags & VERBOSE)
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

        reverse = bool(info.flags & REVERSE)

        # Compile the compound pattern. The result is a list of tuples.
        code = parsed.compile(reverse) + [(OP.SUCCESS, )]

        # Flatten the code into a list of ints.
        code = flatten_code(code)

        if not parsed.has_simple_start():
            # Get the first set, if possible.
            try:
                fs_code = compile_firstset(info, parsed.get_firstset(reverse))
                fs_code = flatten_code(fs_code)
                code = fs_code + code
            except FirstSetError:
                pass

        # Check the global flags for conflicts.
        version = (info.flags & ALL_VERSIONS) or DEFAULT_VERSION
        if count_ones(version) > 1:
            raise ValueError("VERSION0 and VERSION1 flags are mutually incompatible")

        # Create the PatternObject.
        #
        # Local flags like IGNORECASE affect the code generation, but aren't
        # needed by the PatternObject itself. Conversely, global flags like
        # LOCALE _don't_ affect the code generation but _are_ needed by the
        # PatternObject.
        self.scanner = _regex.compile(None, (flags & GLOBAL_FLAGS) | version,
          code, {}, {}, {}, [])

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
    "m": StartOfWord(),
    "M": EndOfWord(),
    "Z": EndOfString(),
}

# Positional escape sequences when WORD flag set.
WORD_POSITION_ESCAPES = dict(POSITION_ESCAPES)
WORD_POSITION_ESCAPES.update({
    "b": DefaultBoundary(),
    "B": DefaultBoundary(False),
    "m": DefaultStartOfWord(),
    "M": DefaultEndOfWord(),
})
