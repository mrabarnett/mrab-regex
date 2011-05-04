Introduction
------------

This new regex implementation is intended eventually to replace Python's current re module implementation.

For testing and comparison with the current 're' module the new implementation is in the form of a module called 'regex'.

Also included are the compiled binary .pyd files for Python 2.5-2.7 and Python 3.1-3.2 on 32-bit Windows.


Flags
-----

There are 2 kinds of flag: scoped and global. Scoped flags can apply to only part of a pattern and can be turned on or off; global flags apply to the entire pattern and can only be turned on.

The scoped flags are: ``IGNORECASE``, ``MULTILINE``, ``DOTALL``, ``VERBOSE``, ``WORD``.

The global flags are: ``ASCII``, ``LOCALE``, ``NEW``, ``REVERSE``, ``UNICODE``.

If neither the ASCII, LOCALE nor UNICODE flag is specified, the default is UNICODE if the regex pattern is a Unicode string and ASCII if it's a bytestring.

The ``NEW`` flag turns on the new behaviour of this module, which can differ from that of the 're' module, such as splitting on zero-width matches, inline flags affecting only what follows, and being able to turn inline flags off.


Notes on named capture groups
-----------------------------

All capture groups have a group number, starting from 1.

Groups with the same group name will have the same group number, and groups with a different group name will have a different group number.

The same group name can be used on different branches of an alternation because they are mutually exclusive, eg. ``(?P<foo>first)|(?P<foo>second)``. They will, of course, have the same group number.

Group numbers will be reused, where possible, across different branches of a branch reset, eg. ``(?|(first)|(second))`` has only group 1. If capture groups have different group names then they will, of course, have different group numbers, eg. ``(?|(?P<foo>first)|(?P<bar>second))`` has group 1 ("foo") and group 2 ("bar").


Multithreading
--------------

The regex module releases the GIL during matching on instances of the built-in (immutable) string classes, enabling other Python threads to run concurrently. It is also possible to force the regex module to release the GIL during matching by calling the matching methods with the keyword argument ``concurrent=True``. The behaviour is undefined if the string changes during matching, so use it *only* when it is guaranteed that that won't happen.


Building for 64-bits
--------------------

If the source files are built for a 64-bit target then the string positions will also be 64-bit. (The 're' module appears to limit string positions to 32 bits, even on a 64-bit build.)


Unicode
-------

This module supports Unicode 6.0.0.


Additional features
-------------------

The issue numbers relate to the Python bug tracker.

* regex.escape (issue #2650)

    regex.escape has an additional keyword parameter ``special_only``. When True, only 'special' regex characters, such as '?', are escaped.

    Examples:

        >>> regex.escape("foo!?")
        'foo\\!\\?'
        >>> regex.escape("foo!?", special_only=True)
        'foo!\\?'

* Repeated captures (issue #7132)

    A match object has additional methods which return information on all the successful matches of a repeated capture group. These methods are:

    ``matchobject.captures([group1, ...])``

        Returns a list of the strings matched in a group or groups. Compare with ``matchobject.group([group1, ...])``.

    ``matchobject.starts([group])``

        Returns a list of the start positions. Compare with ``matchobject.start([group])``.

    ``matchobject.ends([group])``

        Returns a list of the end positions. Compare with ``matchobject.end([group])``.

    ``matchobject.spans([group])``

        Returns a list of the spans. Compare with ``matchobject.span([group])``.

    Examples:

        >>> m = regex.search(r"(\w{3})+", "123456789")
        >>> m.group(1)
        '789'
        >>> m.captures(1)
        ['123', '456', '789']
        >>> m.start(1)
        6
        >>> m.starts(1)
        [0, 3, 6]
        >>> m.end(1)
        9
        >>> m.ends(1)
        [3, 6, 9]
        >>> m.span(1)
        (6, 9)
        >>> m.spans(1)
        [(0, 3), (3, 6), (6, 9)]

* Atomic grouping (issue #433030)

    ``(?>...)``

    If the following pattern subsequently fails, then the subpattern as a whole will fail.

* Possessive quantifiers.

    ``(?:...)?+`` ; ``(?:...)*+`` ; ``(?:...)++`` ; ``(?:...){min,max}+``

    The subpattern is matched up to 'max' times. If the following pattern subsequently fails, then all of the repeated subpatterns will fail as a whole. For example, ``(?:...)++`` is equivalent to ``(?>(?:...)+)``.

* Scoped flags (issue #433028)

    ``(?flags-flags:...)``

    The flags will apply only to the subpattern. Flags can be turned on or off.

* Inline flags (issue #433024, issue #433027)

    ``(?flags-flags)``

    If the ``NEW`` flag is turned on then the flags will apply to the end of the group or pattern and can be turned on or off. If the ``NEW`` flag isn't turned on then the flags will be global and can't be turned off.

* Repeated repeats (issue #2537)

    A regex like ``((x|y+)*)*`` will be accepted and will work correctly, but should complete more quickly.

* Definition of 'word' character (issue #1693050)

    The definition of a 'word' character has been expanded for Unicode. It now conforms to the Unicode specification at ``http://www.unicode.org/reports/tr29/``. This applies to ``\w``, ``\W``, ``\b`` and ``\B``.

* Groups in lookahead and lookbehind (issue #814253)

    Groups and group references are permitted in both lookahead and lookbehind.

* Variable-length lookbehind

    A lookbehind can match a variable-length string.

* Correct handling of charset with ignore case flag (issue #3511)

    Ranges within charsets are handled correctly when the ignore-case flag is turned on.

* Unmatched group in replacement (issue #1519638)

    An unmatched group is treated as an empty string in a replacement template.

* 'Pathological' patterns (issue #1566086, issue #1662581, issue #1448325, issue #1721518, issue #1297193)

    'Pathological' patterns should complete more quickly.

* Flags argument for regex.split, regex.sub and regex.subn (issue #3482)

    ``regex.split``, ``regex.sub`` and ``regex.subn`` support a 'flags' argument.

* Pos and endpos arguments for regex.sub and regex.subn

    ``regex.sub`` and ``regex.subn`` support 'pos' and 'endpos' arguments.

* 'Overlapped' argument for regex.findall and regex.finditer

    ``regex.findall`` and ``regex.finditer`` support an 'overlapped' flag which permits overlapped matches.

* Unicode escapes (issue #3665)

    The Unicode escapes ``\uxxxx`` and ``\Uxxxxxxxx`` are supported.

* Large patterns (issue #1160)

    Patterns can be much larger.

* Zero-width match with regex.finditer (issue #1647489)

    ``regex.finditer`` behaves correctly when it splits at a zero-width match.

* Zero-width split with regex.split (issue #3262)

    ``regex.split`` can split at a zero-width match if the ``NEW`` flag is turned on. When the flag is turned off the current behaviour is unchanged because the BDFL thinks that some existing software might depend on it.

* Splititer

    ``regex.splititer`` has been added. It's a generator equivalent of ``regex.split``.

* Subscripting for groups

    A match object accepts access to the captured groups via subscripting and slicing:

    >>> m = regex.search(r"(?P<before>.*?)(?P<num>\d+)(?P<after>.*)", "pqr123stu")
    >>> print m["before"]
    pqr
    >>> print m["num"]
    123
    >>> print m["after"]
    stu
    >>> print len(m)
    4
    >>> print m[:]
    ('pqr123stu', 'pqr', '123', 'stu')

* Named groups

    Groups can be named with ``(?<name>...)`` as well as the current ``(?P<name>...)``.

* Group references

    Groups can be referenced within a pattern with ``\g<name>``. This also allows there to be more than 99 groups.

* Named characters

    ``\N{name}``

    Named characters are supported. (Note: only those known by Python's Unicode database are supported.)

* Unicode codepoint properties, including scripts and blocks

    ``\p{property=value}``; ``\P{property=value}``; ``\p{value}`` ; ``\P{value}``

    Many Unicode properties are supported, including blocks and scripts. ``\p{property=value}`` or ``\p{property:value}`` matches a character whose property ``property`` has value ``value``. The inverse of ``\p{property=value}`` is ``\P{property=value}`` or ``\p{^property=value}``.

    If the short form ``\p{value}`` is used, the properties are checked in the order: ``General_Category``, ``Script``, ``Block``, binary property:

    1. ``Latin``, the 'Latin' script (``Script=Latin``).

    2. ``Cyrillic``, the 'Cyrillic' script (``Script=Cyrillic``).

    3. ``BasicLatin``, the 'BasicLatin' block (``Block=BasicLatin``).

    4. ``Alphabetic``, the 'Alphabetic' binary property (``Alphabetic=Yes``).

    A short form starting with ``Is`` indicates a script or binary property:

    1. ``IsLatin``, the 'Latin' script (``Script=Latin``).

    2. ``IsCyrillic``, the 'Cyrillic' script (``Script=Cyrillic``).

    3. ``IsAlphabetic``, the 'Alphabetic' binary property (``Alphabetic=Yes``).

    A short form starting with ``In`` indicates a block property:

    1. ``InBasicLatin``, the 'BasicLatin' block (``Block=BasicLatin``).

    2. ``InCyrillic``, the 'Cyrillic' block (``Block=Cyrillic``).

* POSIX character classes

    ``[[:alpha:]]``; ``[[:^alpha:]]``

    POSIX character classes are supported. This is actually treated as an alternative form of ``\p{...}``.

* Search anchor

    ``\G``

    A search anchor has been added. It matches at the position where each search started/continued and can be used for contiguous matches or in negative variable-length lookbehinds to limit how far back the lookbehind goes:

    >>> regex.findall(r"\w{2}", "abcd ef")
    ['ab', 'cd', 'ef']
    >>> regex.findall(r"\G\w{2}", "abcd ef")
    ['ab', 'cd']

    1. The search starts at position 0 and matches 2 letters 'ab'.

    2. The search continues at position 2 and matches 2 letters 'cd'.

    3. The search continues at position 4 and fails to match any letters.

    4. The anchor stops the search start position from being advanced, so there are no more results.

* Reverse searching

    Searches can now work backwards:

    >>> regex.findall(r".", "abc")
    ['a', 'b', 'c']
    >>> regex.findall(r"(?r).", "abc")
    ['c', 'b', 'a']

    Note: the result of a reverse search is not necessarily the reverse of a forward search:

    >>> regex.findall(r"..", "abcde")
    ['ab', 'cd']
    >>> regex.findall(r"(?r)..", "abcde")
    ['de', 'bc']

* Matching a single grapheme

    ``\X``

    The grapheme matcher is supported. It now conforms to the Unicode specification at ``http://www.unicode.org/reports/tr29/``.

* Branch reset

    (?|...|...)

    Capture group numbers will be reused across the alternatives.

* Default Unicode word boundary

    The ``WORD`` flag changes the definition of a 'word boundary' to that of a default Unicode word boundary. This applies to ``\b`` and ``\B``.

* SRE engine do not release the GIL (issue #1366311)

    The regex module can release the GIL during matching (see the above section on multithreading).

    Iterators can be safely shared across threads.
