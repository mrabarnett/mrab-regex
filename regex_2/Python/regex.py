#
# Secret Labs' Regular Expression Engine
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

r"""Support for regular expressions (RE).

This module provides regular expression matching operations similar to
those found in Perl.  It supports both 8-bit and Unicode strings; both
the pattern and the strings being processed can contain null bytes and
characters outside the US ASCII range.

Regular expressions can contain both special and ordinary characters.
Most ordinary characters, like "A", "a", or "0", are the simplest
regular expressions; they simply match themselves.  You can
concatenate ordinary characters, so last matches the string 'last'.

The special characters are:
    "."                Matches any character except a newline.
    "^"                Matches the start of the string.
    "$"                Matches the end of the string or just before the newline
                       at the end of the string.
    "*"                Matches 0 or more (greedy) repetitions of the preceding
                       RE. Greedy means that it will match as many repetitions
                       as possible.
    "+"                Matches 1 or more (greedy) repetitions of the preceding
                       RE.
    "?"                Matches 0 or 1 (greedy) of the preceding RE.
    *?,+?,??           Non-greedy versions of the previous three special
                       characters.
    *+,++,?+           Possessive versions of the previous three special
                       characters.
    {m,n}              Matches from m to n repetitions of the preceding RE.
    {m,n}?             Non-greedy version of the above.
    {m,n}+             Possessive version of the above.
    "\\"               Either escapes special characters or signals a special
                       sequence.
    []                 Indicates a set of characters. A "^" as the first
                       character indicates a complementing set.
    "|"                A|B, creates an RE that will match either A or B.
    (...)              Matches the RE inside the parentheses. The contents are
                       captured and can be retrieved or matched later in the
                       string.
    (?flags-flags)     Sets/clears the flags for the entire RE, or the
                       remainder of the RE if the 'NEW' flag is set.
    (?:...)            Non-capturing version of regular parentheses.
    (?>...)            Atomic non-capturing version of regular parentheses.
    (?flags-flags:...) Non-capturing version of regular parentheses with local
                       flags.
    (?P<name>...)      The substring matched by the group is accessible by
                       name.
    (?<name>...)       The substring matched by the group is accessible by
                       name.
    (?P=name)          Matches the text matched earlier by the group named
                       name.
    (?#...)            A comment; ignored.
    (?=...)            Matches if ... matches next, but doesn't consume the
                       string.
    (?!...)            Matches if ... doesn't match next.
    (?<=...)           Matches if preceded by ... (must be fixed length).
    (?<!...)           Matches if not preceded by ... (must be fixed length).
    (?(id)yes|no)      Matches yes pattern if group id matched, the (optional)
                       no pattern otherwise.
    (?|...|...)        (?|A|B), creates an RE that will match either A or B,
                       but reuses capture group numbers across the
                       alternatives.

The special sequences consist of "\\" and a character from the list
below.  If the ordinary character is not on the list, then the
resulting RE will match the second character.
    \number    Matches the contents of the group of the same number if number
               is no more than 2 digits, otherwise the character with the
               3-digit octal code.
    \a         Matches the bell character.
    \A         Matches only at the start of the string.
    \b         Matches the empty string, but only at the start or end of a
               word.
    \B         Matches the empty string, but not at the start or end of a word.
    \d         Matches any decimal digit; equivalent to the set [0-9] when
               matching a bytestring or a Unicode string with the ASCII flag,
               or the whole range of Unicode digits when matching a Unicode
               string.
    \D         Matches any non-digit character; equivalent to [^\d].
    \f         Matches the formfeed character.
    \g<name>   Matches the text matched by the group named name.
    \G         Matches the empty string, but only at the position where the
               search started.
    \n         Matches the newline character.
    \N{name}   Matches the named character.
    \p{name}   Matches the character if it has the specified property.
    \P{name}   Matches the complement of \p<name>.
    \r         Matches the carriage-return character.
    \s         Matches any whitespace character; equivalent to [ \t\n\r\f\v].
    \S         Matches any non-whitespace character; equivalent to [^\s].
    \t         Matches the tab character.
    \uXXXX     Matches the Unicode codepoint with 4-digit hex code XXXX.
    \UXXXXXXXX Matches the Unicode codepoint with 8-digit hex code XXXXXXXX.
    \v         Matches the vertical tab character.
    \w         Matches any alphanumeric character; equivalent to [a-zA-Z0-9_]
               when matching a bytestring or a Unicode string with the ASCII
               flag, or the whole range of Unicode alphanumeric characters
               (letters plus digits plus underscore) when matching a Unicode
               string. With LOCALE, it will match the set [0-9_] plus
               characters defined as letters for the current locale.
    \W         Matches the complement of \w; equivalent to [^\w].
    \xXX       Matches the character with 2-digit hex code XX.
    \X         Matches a grapheme; equivalent to \P{M}\p{M}*.
    \Z         Matches only at the end of the string.
    \\         Matches a literal backslash.

This module exports the following functions:
    match     Match a regular expression pattern at the beginning of a string.
    search    Search a string for the presence of a pattern.
    sub       Substitute occurrences of a pattern found in a string.
    subn      Same as sub, but also return the number of substitutions made.
    split     Split a string by the occurrences of a pattern.
    splititer Return an iterator yielding the parts of a split string.
    findall   Find all occurrences of a pattern in a string.
    finditer  Return an iterator yielding a match object for each match.
    compile   Compile a pattern into a Pattern object.
    purge     Clear the regular expression cache.
    escape    Backslash all non-alphanumerics or special characters in a
              string.

Most of the functions support a concurrent parameter: if True, the GIL will be
released during matching, allowing other Python threads to run concurrently. If
the string changes during matching, the behaviour is undefined. This parameter
is not needed when working on the builtin (immutable) string classes.

Some of the functions in this module take flags as optional parameters. Most of
these flags can also be set within an RE:
    A  a  ASCII      Make \w, \W, \b, \B, \d, and \D match the corresponding
                     ASCII character categories when matching a Unicode string.
                     Default when matching a bytestring.
    D     DEBUG      Print the parsed pattern.
    I  i  IGNORECASE Perform case-insensitive matching.
    L  L  LOCALE     Make \w, \W, \b, \B, \d, and \D dependent on the current
                     locale.
    M  m  MULTILINE  "^" matches the beginning of lines (after a newline) as
                     well as the string. "$" matches the end of lines (before a
                     newline) as well as the end of the string.
    N  n  NEW        Turn on scoped flags and correct handling of zero-width
                     matches.
    R  r  REVERSE    Searches backwards.
    S  s  DOTALL     "." matches any character at all, including the newline.
    U  u  UNICODE    Make \w, \W, \b, \B, \d, and \D dependent on the Unicode
                     locale. Default when matching a Unicode string.
    W  w  WORD       Make \b and \B work with default Unicode word breaks.
    X  x  VERBOSE    Ignore whitespace and comments for nicer looking RE's.

This module also defines an exception 'error'.

"""

# Public symbols.
__all__ = ["match", "search", "sub", "subn", "split", "splititer", "findall",
    "finditer", "compile", "purge", "template", "escape", "A", "D", "I", "L",
    "M", "N", "R", "S", "T", "U", "W", "X", "ASCII", "DEBUG", "IGNORECASE",
    "LOCALE", "MULTILINE", "NEW", "REVERSE", "DOTALL", "TEMPLATE", "UNICODE",
    "WORD", "VERBOSE", "error"]

__version__ = "2.3.0"

# --------------------------------------------------------------------
# Public interface.

def match(pattern, string, flags=0, pos=None, endpos=None, concurrent=None):
    """Try to apply the pattern at the start of the string, returning a match
    object, or None if no match was found."""
    return _compile(pattern, flags).match(string, pos, endpos, concurrent)

def search(pattern, string, flags=0, pos=None, endpos=None, concurrent=None):
    """Search through string looking for a match to the pattern, returning a
    match object, or None if no match was found."""
    return _compile(pattern, flags).search(string, pos, endpos, concurrent)

def sub(pattern, repl, string, count=0, flags=0, pos=None, endpos=None,
  concurrent=None):
    """Return the string obtained by replacing the leftmost (or rightmost with
    a reverse pattern) non-overlapping occurrences of the pattern in string by
    the replacement repl.  repl can be either a string or a callable; if a
    string, backslash escapes in it are processed; if a callable, it's passed
    the match object and must return a replacement string to be used."""
    return _compile(pattern, flags).sub(repl, string, count, pos, endpos,
      concurrent)

def subn(pattern, repl, string, count=0, flags=0, pos=None, endpos=None,
  concurrent=None):
    """Return a 2-tuple containing (new_string, number).  new_string is the
    string obtained by replacing the leftmost (or rightmost with a reverse
    pattern) non-overlapping occurrences of the pattern in the source string by
    the replacement repl.  number is the number of substitutions that were
    made.  repl can be either a string or a callable; if a string, backslash
    escapes in it are processed; if a callable, it's passed the match object
    and must return a replacement string to be used."""
    return _compile(pattern, flags).subn(repl, string, count, pos, endpos,
      concurrent)

def split(pattern, string, maxsplit=0, flags=0, concurrent=None):
    """Split the source string by the occurrences of the pattern, returning a
    list containing the resulting substrings."""
    return _compile(pattern, flags).split(string, maxsplit, concurrent)

def splititer(pattern, string, maxsplit=0, flags=0, concurrent=None):
    """Return an iterator yielding the parts of a split string."""
    return _compile(pattern, flags).splititer(string, maxsplit, concurrent)

def findall(pattern, string, flags=0, pos=None, endpos=None, overlapped=False,
  concurrent=None):
    """Return a list of all matches in the string.  The matches may be
    overlapped if overlapped is True.  If one or more groups are present in the
    pattern, return a list of groups; this will be a list of tuples if the
    pattern has more than one group.  Empty matches are included in the
    result."""
    return _compile(pattern, flags).findall(string, pos, endpos, overlapped,
      concurrent)

def finditer(pattern, string, flags=0, pos=None, endpos=None, overlapped=False,
  concurrent=None):
    """Return an iterator over all matches in the string.  The matches may be
    overlapped if overlapped is True.  For each match, the iterator returns a
    match object.  Empty matches are included in the result."""
    return _compile(pattern, flags).finditer(string, pos, endpos, overlapped,
      concurrent)

def compile(pattern, flags=0):
    "Compile a regular expression pattern, returning a pattern object."
    return _compile(pattern, flags)

def purge():
    "Clear the regular expression cache"
    _cache.clear()

def template(pattern, flags=0):
    "Compile a template pattern, returning a pattern object."
    return _compile(pattern, flags | TEMPLATE)

def escape(pattern, special_only=False):
    "Escape all non-alphanumeric characters or special characters in pattern."
    if isinstance(pattern, unicode):
        s = []
        if special_only:
            for c in pattern:
                if c in _rc._SPECIAL:
                    s.append(u"\\")
                    s.append(c)
                elif c == u"\x00":
                    s.append(u"\\000")
                else:
                    s.append(c)
        else:
            for c in pattern:
                if c in _rc._ALNUM:
                    s.append(c)
                elif c == u"\x00":
                    s.append(u"\\000")
                else:
                    s.append(u"\\")
                    s.append(c)
        return u"".join(s)
    else:
        s = []
        if special_only:
            for c in pattern:
                if c in _rc._ALNUM:
                    s.append("\\")
                    s.append(c)
                elif c == "\x00":
                    s.append("\\000")
                else:
                    s.append(c)
        else:
            for c in pattern:
                if c in _rc._ALNUM:
                    s.append(c)
                elif c == "\x00":
                    s.append("\\000")
                else:
                    s.append("\\")
                    s.append(c)
        return "".join(s)

# --------------------------------------------------------------------
# Internals.

from _regex_core import *
import _regex_core as _rc

# Caches for the patterns and replacements.
_cache = {}

# Maximum size of the cache.
_MAXCACHE = 500

def _compile(pattern, flags=0):
    "Compiles a regular expression to a PatternObject. "
    # We're checking in this order because _pattern_type isn't defined when
    # _compile() is first called, with a string pattern, but only after the
    # support objects are defined.
    if isinstance(pattern, (unicode, str)):
        pass
    elif isinstance(pattern, _pattern_type):
        if flags:
            raise ValueError("can't process flags argument with a compiled pattern")
        return pattern
    else:
        raise TypeError("first argument must be a string or compiled pattern")

    # Have we already seen this regular expression?
    key = pattern, type(pattern), flags
    p = _cache.get(key)
    if p:
        return p

    # Parse the pattern. If the 'NEW' flag isn't turned on then the positional
    # inline flags will be global and the pattern will need to be reparsed if a
    # positional flag becomes turned on.
    global_flags = flags
    while True:
        try:
            source = _rc._Source(pattern)
            info = _rc._Info(global_flags, source.char_type)
            source.ignore_space = bool(info.all_flags & VERBOSE)
            parsed = _rc._parse_pattern(source, info)
            break
        except _rc._UnscopedFlagSet, e:
            # Remember the global flags for the next attempt.
            global_flags = e.global_flags

    if not source.at_end():
        raise error("trailing characters in pattern")

    # Check the global flags for conflicts.
    all_flags = ASCII | LOCALE | UNICODE
    if _rc._count_ones(info.global_flags & all_flags) > 1:
        raise ValueError("ASCII, LOCALE and UNICODE flags are mutually incompatible")

    # Fix the group references.
    parsed.fix_groups()

    # Optimise the parsed pattern.
    parsed = parsed.optimise(info)
    parsed = parsed.pack_characters()

    reverse = bool(info.global_flags & REVERSE)

    # Should we print the parsed pattern?
    if flags & DEBUG:
        parsed.dump(indent=0, reverse=reverse)

    # Compile the parsed pattern. The result is a list of tuples.
    code = parsed.compile(reverse) + [(_rc._OP.SUCCESS, )]
    if parsed.has_simple_start():
        fs_code = []
    else:
        fs_code = _rc._compile_firstset(info, parsed.firstset())
    fs_code = _rc._flatten_code(fs_code)

    # Flatten the code into a list of ints.
    code = _rc._flatten_code(code)

    code = fs_code + code

    # The named capture groups.
    index_group = dict((v, n) for n, v in info.group_index.items())

    # Create the PatternObject.
    #
    # Local flags like IGNORECASE affect the code generation, but aren't needed
    # by the PatternObject itself. Conversely, global flags like LOCALE _don't_
    # affect the code generation but _are_ needed by the PatternObject.
    p = _regex.compile(pattern, info.global_flags | info.scoped_flags, code,
      info.group_index, index_group)

    # Store the compiled pattern.
    if len(_cache) >= _MAXCACHE:
        _rc._shrink_cache(_cache, _MAXCACHE)
    _cache[key] = p

    return p

def _compile_replacement(pattern, template):
    "Compiles a replacement template."
    # This function is called by the _regex module.
    source = _rc._Source(template)
    if isinstance(template, unicode):
        def make_string(char_codes):
            return u"".join(unichr(c) for c in char_codes)
    else:
        def make_string(char_codes):
            return "".join(chr(c) for c in char_codes)
    compiled = []
    literal = []
    while True:
        ch = source.get()
        if not ch:
            break
        if ch == "\\":
            # '_compile_repl_escape' will return either an int group reference
            # or a string literal. It returns items (plural) in order to handle
            # a 2-character literal (an invalid escape sequence).
            is_group, items = _rc._compile_repl_escape(source, pattern)
            if is_group:
                # It's a group, so first flush the literal.
                if literal:
                    compiled.append(make_string(literal))
                    literal = []
                compiled.extend(items)
            else:
                literal.extend(items)
        else:
            literal.append(ord(ch))
    # Flush the literal.
    if literal:
        compiled.append(make_string(literal))
    return compiled

import _regex
_regex.set_exception(error)

# We define _pattern_type here after all the support objects have been
# defined.
_pattern_type = type(_compile("", 0))

# Register myself for pickling.
import copy_reg

def _pickle(p):
    return _compile, (p.pattern, p.flags)

copy_reg.pickle(_pattern_type, _pickle, _compile)
