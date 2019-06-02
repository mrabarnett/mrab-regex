#! python3.7
# -*- coding: utf-8 -*-
#
# This Python script parses the Unicode data files and generates the C files
# for the regex module.
#
# Written by MRAB.
#
from collections import defaultdict
from json import dump, load
from os import remove, rename
from os.path import dirname, exists, isfile, join, splitext
from shutil import copy2, move
from urllib.error import HTTPError
from urllib.parse import urljoin
from urllib.request import urlretrieve

def report(*args, **kwargs):
    print(*args, **kwargs, flush=True)

class UnicodeDataError(Exception):
    pass

def download_unicode_file(url, path):
    'Downloads a Unicode file.'

    report('Downloading {}'.format(url))

    new_path = path + '.new'

    try:
        urlretrieve(url, new_path)
    except ValueError:
        # The download failed. Clean up.
        try:
            remove(new_path)
        except OSError:
            pass

        raise

    # Make a copy of the downloaded file with a versioned filename.
    with open(new_path, encoding='utf-8') as file:
        # The 1st line of the file should be a comment with a version filename.
        line = file.readline()

    if line.startswith('#') and line.endswith('.txt\n'):
        versioned_name = line.strip('# \n')
        versioned_path = join(unicode_folder, versioned_name)
        if isfile(versioned_path):
            # We already have this version.
            remove(new_path)
        else:
            # It's a new version.
            move(new_path, path)
            copy2(path, versioned_path)

            version = splitext(versioned_name)[0].partition('-')[2]
            report('Updated to version {}'.format(versioned_name))

class MultistageTable:
    'A multi-stage table.'

    def __init__(self, block_sizes, stages, is_binary):
        self.block_sizes = block_sizes
        self.stages = stages
        self.is_binary = is_binary

        self.num_stages = len(self.block_sizes) + 1

        # How many bytes of storage are needed for this table?
        if self.is_binary:
            self.bytesize = (sum(smallest_datatype(min(stage), max(stage))[1] *
              len(stage) for stage in self.stages[ : -1]) +
              (len(self.stages[-1]) + 7) // 8)
        else:
            self.bytesize = sum(smallest_datatype(min(stage), max(stage))[1] *
              len(stage) for stage in self.stages)

        # Calculate the block-size products for lookup.
        self._size_products = [product(self.block_sizes[stage : ]) for stage in
          range(self.num_stages - 1)]

class PropertySet:
    'A collection of Unicode properties.'

    def __init__(self):
        self._property_list = []
        self._property_dict = {}
        self._munge_property_name = None

    def add(self, property):
        'Adds a property.'

        if not self._munge_property_name:
            self._munge_property_name = property.munge_name

        if property.name in self:
            raise ValueError('property set already has property {!a}'.format(property.name))

        self._property_list.append(property)

        for name in [property.name] + property.aliases:
            self._property_dict[self._munge_property_name(name)] = property

    def get(self, name, default=None):
        if not self._property_dict:
            return default

        return self._property_dict.get(self._munge_property_name(name),
          default)

    def __len__(self):
        return len(self._property_list)

    def __getitem__(self, name):
        property = self.get(self._munge_property_name(name))
        if property is None:
            raise KeyError(name)

        return property

    def __delitem__(self, name):
        property = self[name]

        self._property_list = [p for p in self._property_list if p is not
          property]
        self._property_dict = {n: p for n, p in self._property_dict.items() if
          p is not property}

    def __iter__(self):
        for property in self._property_list:
            yield property

class Property:
    'A Unicode property.'

    def __init__(self, name, aliases=[]):
        self.name = name
        self.aliases = []
        self.default = None
        self.id = None
        self._value_list = []
        self._value_dict = {}
        self._munge_value_name = None

        # Ignore any duplicate names.
        seen = {self.munge_name(self.name)}

        for name in aliases:
            munged_name = self.munge_name(name)
            if munged_name not in seen:
                self.aliases.append(name)
                seen.add(munged_name)

    def add(self, value):
        'Adds a value.'

        if not self._munge_value_name:
            self._munge_value_name = value.munge_name

        if value.name in self:
            raise KeyError('property {!a} already has value {!a}'.format(self.name,
              value.name))

        self._value_list.append(value)

        for munged_name in [value.name] + value.aliases:
            self._value_dict[self._munge_value_name(munged_name)] = value

    def munge_name(self, name):
        'Munges a name into a standard form.'

        return name.translate(self._munge_trans).upper()

    _munge_trans = str.maketrans('', '', ' -_')

    def get(self, name, default=None):
        if not self._value_dict:
            return default

        return self._value_dict.get(self._munge_value_name(name), default)

    def __len__(self):
        return len(self._value_list)

    def __getitem__(self, name):
        value = self.get(name)
        if value is None:
            raise KeyError(name)

        return value

    def __iter__(self):
        for value in self._value_list:
            yield value

    def _expand_entries(self):
        'Expands the value dict into a list of entries.'

        entries = [0] * NUM_CODEPOINTS

        if self.name == 'Script_Extensions':
            script = properties['Script']

            for value in script._value_list:
                for lower, upper in value.ranges:
                    entries[lower : upper + 1] = [value.id] * (upper - lower +
                      1)

        for value in self._value_list:
            if not value.mask:
                for lower, upper in value.ranges:
                    entries[lower : upper + 1] = [value.id] * (upper - lower +
                      1)

        return entries

    def generate_code(self, h_file, c_file):
        'Generates the code for a property.'

        # Build the tables.
        self._build_tables(MAX_STAGES)

        report('Generating code for {!a}'.format(self.name))

        table = self.table

        # Write the property tables.
        c_file.write('''
/* {name}. */
'''.format(name=self.name))

        self.generate_tables(c_file)

        if self.name == 'Script_Extensions':
            rows = []
            scripts = properties['Script']

            for value in sorted(self, key=lambda value: value.id):
                rows.append(sorted(scripts[name].id for name in
                  value.name.split()))

            max_cols = max(len(row) for row in rows)
            fmt = '    {{' + ', '.join(['%3d'] * max_cols) + '}},'

            rows = [fmt % tuple(row + [0] * (max_cols - len(row))) for row in
              rows]
            rows[-1] = rows[-1].rstrip(',')

            c_file.write('''
static RE_ScriptExt re_scripts_extensions_table[] = {
''')

            for row in rows:
                c_file.write(row + '\n')

            c_file.write('''\
};
''')

        # Write the lookup function.
        if self.name == 'Script_Extensions':
            prototype = 'int re_get_script_extensions(RE_UINT32 ch, RE_UINT8* scripts)'
        else:
            prototype = 'RE_UINT32 re_get_{name}(RE_UINT32 ch)'.format(name=self.name.lower())

        h_file.write('{prototype};\n'.format(prototype=prototype))

        c_file.write('''
{prototype} {{
'''.format(prototype=prototype))

        self._generate_locals(c_file)

        c_file.write('\n')

        self._generate_lookup(c_file)

        if self.name == 'Script_Extensions':
            c_file.write('''
    scr = re_scripts_extensions_table[value].scripts;
    scripts[0] = scr[0];
    count = 1;

    if (scr[0] != 0) {
        while (count < RE_MAX_SCX && scr[count] != 0) {
            scripts[count] = scr[count];
            ++count;
        }
    }

    return count;
}
''')
        else:
            c_file.write('''
    return value;
}
''')

    def generate_tables(self, c_file):
        'Generates the tables.'

        table = self.table

        for stage in range(table.num_stages):
            # The contents of this table.
            entries = table.stages[stage]

            # What data type should we use for the entries?
            if self.is_binary and stage == table.num_stages - 1:
                data_type = 'RE_UINT8'

                entries = self._pack_to_bitflags(entries)
            else:
                data_type = smallest_datatype(min(entries), max(entries))[0]

            # The entries will be stored in an array.
            c_file.write('''
static {data_type} re_{name}_stage_{stage}[] = {{
'''.format(data_type=data_type, name=self.name.lower(), stage=stage + 1))

            # Write the entries, nicely aligned in columns.
            entries = ['{},'.format(e) for e in entries]

            entry_width = max(len(e) for e in entries)
            entries = [e.rjust(entry_width) for e in entries]

            for start in range(0, len(entries), COLUMNS):
                c_file.write('    {}\n'.format(' '.join(entries[start : start +
                  COLUMNS])))

            c_file.write('};\n')

        # Write how much storage will be used by all of the tables.
        c_file.write('''
/* {name}: {bytesize} bytes. */
'''.format(name=self.name, bytesize=table.bytesize))

    def _pack_to_bitflags(self, entries):
        entries = tuple(entries)
        new_entries = []

        for start in range(0, len(entries), 8):
            new_entries.append(self.bitflag_dict[entries[start : start + 8]])

        return new_entries

    # Build a dict for converting 8-tuples into bytes.
    bitflag_dict = {}

    for byte in range(0x100):
        bitflag_dict[tuple(map(int, '{:08b}'.format(byte)[ : : -1]))] = byte

    def _generate_locals(self, c_file):
        c_file.write('''\
    RE_UINT32 code;
    RE_UINT32 f;
    RE_UINT32 pos;
    RE_UINT32 value;
''')

        if self.name == 'Script_Extensions':
            c_file.write('''\
    RE_UINT8* scr;
    int count;
''')

    def _generate_lookup(self, c_file):
        'Generates the C lookup function.'

        table = self.table
        name = self.name.lower()

        # Convert the block sizes into shift values.
        shifts = [mul_to_shift(size) for size in table.block_sizes]

        c_file.write('''\
    f = ch >> {field_shift};
    code = ch ^ (f << {field_shift});
    pos = (RE_UINT32)re_{name}_stage_1[f] << {block_shift};
'''.format(field_shift=sum(shifts), name=name, block_shift=shifts[0]))

        for stage in range(1, table.num_stages - 1):
            c_file.write('''\
    f = code >> {field_shift};
    code ^= f << {field_shift};
    pos = (RE_UINT32)re_{name}_stage_{stage}[pos + f] << {block_shift};
'''.format(field_shift=sum(shifts[stage : ]), name=name, stage=stage + 1,
  block_shift=shifts[stage]))

        # If it's a binary property, we're using bitflags.
        if self.is_binary:
            c_file.write('''\
    pos += code;
    value = (re_{name}_stage_{stage}[pos >> 3] >> (pos & 0x7)) & 0x1;
'''.format(name=self.name.lower(), stage=table.num_stages))
        else:
            c_file.write('''\
    value = re_{name}_stage_{stage}[pos + code];
'''.format(name=self.name.lower(), stage=table.num_stages))

    def _build_tables(self, max_stages):
        'Calculates the block sizes and build the tables.'

        entries = self._expand_entries()

        self.is_binary = len(self._value_list) == 2

        if self.name not in storage:
            smallest_size = float('inf')

            report('Determining smallest storage size')

            # Try different numbers and sizes of blocks.
            for block_sizes, bytesize in self._table_sizes(entries, 1,
              max_stages, self.is_binary):
                if bytesize < smallest_size:
                    best_block_sizes = block_sizes
                    smallest_size = bytesize

            storage[self.name] = best_block_sizes, smallest_size
        else:
            best_block_sizes, smallest_size = storage[self.name]

        report('The storage size is {} bytes'.format(smallest_size))

        self._build_multistage_table(entries, best_block_sizes, smallest_size)

    def _table_sizes(self, entries, num_stages, max_stages, binary):
        '''Yields different numbers and sizes of blocks, up to max_stages.

        All the sizes are powers of 2 and for a binary property the final block
        size is at least 8 because the final stage of the table will be using
        bitflags.
        '''

        # What if this is the top stage?
        if binary:
            bytesize = len(entries) // 8
        else:
            bytesize = (smallest_datatype(min(entries), max(entries))[1] *
              len(entries))

        yield [], bytesize

        if num_stages >= max_stages:
            return

        entries = tuple(entries)

        # Initialise the block size and double it on each iteration. Usually an
        # index entry is 1 byte, so a data block should be at least 2 bytes.
        size = 16 if binary else 2

        # There should be at least 2 blocks.
        while size * 2 <= len(entries) and len(entries) % size == 0:
            # Group the entries into blocks.
            indexes = []
            block_dict = {}
            for start in range(0, len(entries), size):
                block = entries[start : start + size]
                indexes.append(block_dict.setdefault(block, len(block_dict)))

            # Collect all the blocks.
            blocks = []
            for block in sorted(block_dict, key=lambda block:
              block_dict[block]):
                blocks.extend(block)

            # How much storage will the blocks stage need?
            if binary:
                block_bytesize = len(blocks) // 8
            else:
                block_bytesize = (smallest_datatype(min(blocks),
                  max(blocks))[1] * len(blocks))

            # Yield the higher stages for the indexes.
            for block_sizes, total_bytesize in self._table_sizes(indexes,
              num_stages + 1, max_stages, False):
                yield block_sizes + [size], total_bytesize + block_bytesize

            # Next size up.
            size *= 2

    def _build_multistage_table(self, entries, block_sizes, bytesize):
        'Builds a multi-stage table.'

        if product(block_sizes) > len(entries):
            raise UnicodeDataError('product of block sizes greater than number of entries')

        # Build the stages from the bottom one up.
        stages = []

        for block_size in reversed(block_sizes):
            entries = tuple(entries)

            # Group the entries into blocks.
            block_dict = {}
            indexes = []
            for start in range(0, len(entries), block_size):
                block = entries[start : start + block_size]
                indexes.append(block_dict.setdefault(block, len(block_dict)))

            # Collect all the blocks.
            blocks = []
            for block in sorted(block_dict, key=lambda block:
              block_dict[block]):
                blocks.extend(block)

            # We have a new stage.
            stages.append(blocks)

            # Prepare for the next higher stage.
            entries = indexes

        # We have the top stage.
        stages.append(entries)

        # Put the stages into the correct order (top-down).
        stages.reverse()

        self.table = MultistageTable(block_sizes, stages, self.is_binary)

def product(numbers):
    'Returns the product of a series of numbers.'

    if not product:
        raise ValueError('product of empty sequence')

    result = 1
    for n in numbers:
        result *= n

    return result

def mul_to_shift(number):
    'Converts a multiplier into a shift.'

    shift = number.bit_length() - 1
    if shift < 0 or (1 << shift) != number:
        raise ValueError("can't convert multiplier into shift")

    return shift

class AnyProperty(Property):
    'The Any property.'

    def __init__(self):
        Property.__init__(self, 'Any')

    def generate_code(self, h_file, c_file):
        'Generates the code for a property.'

        report('Generating code for Any')

        # Write the lookup function.
        prototype = 'RE_UINT32 re_get_any(RE_UINT32 ch)'

        h_file.write('{prototype};\n'.format(prototype=prototype))

        c_file.write('''
/* Any. */

{prototype} {{
    return 1;
}}
'''.format(prototype=prototype))

class Value:
    'A Unicode value.'

    def __init__(self, name, aliases=[], mask=0, id=None):
        self.name = name
        self.aliases = []
        self.id = id
        self.mask = mask
        self._ranges = []
        self._modified = False

        # Ignore any duplicate names.
        seen = {self.munge_name(self.name)}

        for name in aliases:
            munged_name = self.munge_name(name)
            if munged_name not in seen:
                self.aliases.append(name)
                seen.add(munged_name)

    def add(self, ranges):
        'Adds a codepoint range.'

        if isinstance(ranges, tuple):
            self._ranges.append(ranges)
        else:
            self._ranges.extend(ranges)

        self._modified = True

    @property
    def ranges(self):
        if self._modified:
            self._ranges = merge_ranges(self._ranges)
            self._modified = False

        return self._ranges

    def munge_name(self, name):
        'Munges a name into a standard form.'

        return name.translate(self._munge_trans).upper()

    _munge_trans = str.maketrans('', '', ' -_')

    def __iter__(self):
        for codepoints in self.ranges:
            yield codepoints

class NumericValue(Value):
    'A Unicode numeric value.'

    _munge_trans = str.maketrans('', '', ' _')

class CaseProperty(Property):
    'A case-folding property.'

    def _expand_entries(self):
        entries = [0] * NUM_CODEPOINTS

        for id, key in enumerate(self._value_list):
            for codepoint in self._value_dict[key]:
                entries[codepoint] = id

        return entries

class AllCases(CaseProperty):
    'All Unicode cases.'

    def __init__(self, name, other_cases):
        self.name = name

        # How many other cases can there be?
        max_other_cases = max(map(len, other_cases))
        self.max_cases = max_other_cases + 1

        extra = tuple([0] * max_other_cases)

        self._value_list = [extra]
        self._value_dict = {extra: []}

        for diff, codepoints in sorted(other_cases.items()):
            if diff:
                key = (diff + extra)[ : max_other_cases]
                self._value_list.append(key)
                self._value_dict[key] = codepoints

        self._value_list.sort(key=lambda diff: sorted(self._value_dict[diff]))

    def generate_code(self, h_file, c_file):
        'Generates the code for a property.'

        report('Generating code for {}'.format(self.name))

        # Build the tables.
        self._build_tables(MAX_CASE_STAGES)

        # Write the all-cases tables.
        c_file.write('''
/* {name}. */
'''.format(name=self.name))

        self.generate_tables(c_file)

        # Calculate the size of the struct.
        entry_size = (self.max_cases - 1) * 4

        # Write the entries, nicely aligned in columns.
        rows = [[str(e) for e in r] for r in self._value_list]
        entry_widths = [max(len(e) for e in c) for c in zip(*rows)]
        rows = [[e.rjust(w) for e, w in zip(r, entry_widths)] for r in rows]

        c_file.write('''
static RE_AllCases re_all_cases_table[] = {
''')
        for r in rows:
            c_file.write('    {{%s}},\n' % ', '.join(r))

        c_file.write('};\n')

        # Write how much storage will be used by the table.
        c_file.write('''
/* {name}: {bytesize} bytes. */
'''.format(name=self.name, bytesize=entry_size * len(rows)))

        # Write the lookup function.
        prototype = 'int re_get_{name}(RE_UINT32 ch, RE_UINT32* codepoints)'.format(name=self.name.lower())

        h_file.write('{prototype};\n'.format(prototype=prototype))

        c_file.write('''
{prototype} {{
'''.format(name=self.name, prototype=prototype))

        self._generate_locals(c_file)

        c_file.write('''\
    RE_AllCases* all_cases;
    int count;

''')

        self._generate_lookup(c_file)

        c_file.write('''
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
''')

class SimpleCaseFolding(CaseProperty):
    'Unicode simple case-folding.'

    def __init__(self, name, folding):
        self.name = name
        self._value_dict = dict(folding)
        self._value_dict.setdefault(0, [])
        self._value_list = list(self._value_dict)

        self._value_list.sort(key=lambda diff: sorted(self._value_dict[diff]))

    def generate_code(self, h_file, c_file):
        'Generates the code for a property.'

        report('Generating code for {}'.format(self.name))

        # Build the tables.
        self._build_tables(MAX_CASE_STAGES)

        # Write the case-folding tables.
        c_file.write('''
/* {name}. */
'''.format(name=self.name))

        self.generate_tables(c_file)

        # Calculate the size of an entry, including alignment.
        entry_size = 4

        # Write the entries, nicely aligned in columns.
        rows = [str(r) for r in self._value_list]
        entry_width = max(len(r) for r in rows)
        rows = [r.rjust(entry_width) for r in rows]

        c_file.write('''
static RE_INT32 re_simple_case_folding_table[] = {
''')
        for r in rows:
            c_file.write('    {},\n'.format(r))

        c_file.write('};\n')

        # Write how much storage will be used by the table.
        c_file.write('''
/* {name}: {bytesize} bytes. */
'''.format(name=self.name, bytesize=entry_size * len(rows)))

        # Write the lookup function.
        prototype = 'RE_UINT32 re_get_{name}(RE_UINT32 ch)'.format(name=self.name.lower())

        h_file.write('{prototype};\n'.format(prototype=prototype))

        c_file.write('''
{prototype} {{
'''.format(name=self.name, prototype=prototype))

        self._generate_locals(c_file)

        c_file.write('''\
    RE_INT32 diff;

''')

        self._generate_lookup(c_file)

        c_file.write('''
    diff = re_simple_case_folding_table[value];

    return (RE_UINT32)((RE_INT32)ch + diff);
}
''')

class FullCaseFolding(CaseProperty):
    'Unicode full case-folding.'

    def __init__(self, name, folding):
        self.name = name
        self.max_folded = max(map(len, folding))

        extra = tuple([0] * self.max_folded)

        self._value_dict = {extra: []}

        for folded in folding:
            key = (folded + extra)[ : self.max_folded]
            self._value_dict[key] = folding[folded]

        self._value_list = list(self._value_dict)
        self._value_list.sort(key=lambda diff: sorted(self._value_dict[diff]))

    def generate_code(self, h_file, c_file):
        'Generates the code for a property.'

        report('Generating code for {}'.format(self.name))

        # Build the tables.
        self._build_tables(MAX_CASE_STAGES)

        # Write the case-folding tables.
        c_file.write('''
/* {name}. */
'''.format(name=self.name))

        self.generate_tables(c_file)

        # What data type should we use for the case-folding entries?
        rows = self._value_list

        # The diff entry needs to be signed 32-bit, the others should be OK
        # with unsigned 16-bit.
        data = [e for r in rows for e in r[1 : ]]

        # Verify that unsigned 16-bit is OK.
        data_type = smallest_datatype(min(data), max(data))[0]
        if data_type != 'RE_UINT16':
            raise UnicodeDataError('full case-folding table entry too big')

        # Calculate the size of an entry, including alignment.
        entry_size = 4 + 2 * (self.max_folded - 1)
        excess = entry_size % 4
        if excess > 0:
            entry_size += 4 - excess

        # Write the entries, nicely aligned in columns.
        rows = [[str(e) for e in r] for r in rows]
        entry_widths = [max(len(e) for e in c) for c in zip(*rows)]
        rows = [[e.rjust(w) for e, w in zip(r, entry_widths)] for r in rows]

        c_file.write('''
static RE_FullCaseFolding re_full_case_folding_table[] = {
''')
        for r in rows:
            c_file.write('    {%s, {%s}},\n' % (r[0], ', '.join(r[1 : ])))

        c_file.write('};\n')

        # Write how much storage will be used by the table.
        c_file.write('''
/* {name}: {bytesize} bytes. */
'''.format(name=self.name, bytesize=entry_size * len(rows)))

        # Write the lookup function.
        prototype = 'int re_get_{name}(RE_UINT32 ch, RE_UINT32* codepoints)'.format(name=self.name.lower())

        h_file.write('{prototype};\n'.format(prototype=prototype))

        c_file.write('''
{prototype} {{
'''.format(name=self.name, prototype=prototype))

        self._generate_locals(c_file)

        c_file.write('''\
    RE_FullCaseFolding* case_folding;
    int count;

''')

        self._generate_lookup(c_file)

        c_file.write('''
    case_folding = &re_full_case_folding_table[value];

    codepoints[0] = (RE_UINT32)((RE_INT32)ch + case_folding->diff);
    count = 1;

    while (count < RE_MAX_FOLDED && case_folding->codepoints[count - 1] != 0) {
        codepoints[count] = case_folding->codepoints[count - 1];
        ++count;
    }

    return count;
}
''')

def parse_property_files(unicode_folder, unicode_data_files, properties):
    'Parses the Unicode data files.'

    # Parse the property and value aliases files.
    parse_property_aliases(join(unicode_folder, 'PropertyAliases.txt'),
      properties)

    parse_value_aliases(join(unicode_folder, 'PropertyValueAliases.txt'),
      properties)

    # Parse the other data files.
    next_property_id = 0
    it = iter(unicode_data_files.splitlines())

    # Find the 'codepoints' section.
    for line in it:
        if line.startswith('[properties]'):
            break

    # Process the 'properties' section.
    for line in it:
        if line.startswith('['):
            break

        if line and not line.startswith('#'):
            filename = line.rpartition('/')[2]
            if filename == 'DerivedNumericValues.txt':
                # Treat the numeric values file specially.
                next_property_id = parse_numeric_values_file(unicode_folder,
                  filename, next_property_id, properties)
            elif filename == 'emoji-data.txt':
                next_property_id = parse_emoji_file(unicode_folder, filename,
                  next_property_id, properties)
            elif filename == 'LineBreak.txt':
                next_property_id = parse_linebreak_file(unicode_folder,
                  filename, next_property_id, properties)
            elif filename == 'ScriptExtensions.txt':
                next_property_id = parse_script_extensions_file(unicode_folder,
                  filename, next_property_id, properties)
            else:
                next_property_id = parse_unicode_data_file(unicode_folder,
                  filename, next_property_id, properties)

    if line.startswith('[normalisation]'):
        filename = next(it).rpartition('/')[2]
        next_property_id = parse_unicode_normalisation_file(unicode_folder,
          filename, next_property_id, properties)

    # Add the additional properties.
    next_property_id = make_additional_properties(properties, next_property_id)

    # Remove all the properties that have been named but not defined.
    undefined = [property for property in properties  if property.id is None]

    for property in undefined:
        del properties[property.name]

    report()

def parse_property_aliases(path, properties):
    'Parses the file listing the property aliases.'

    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.rstrip()
            if line and not line.startswith('#'):
                # It's a data line.
                #
                # Ignore anything after the comment, if any.
                line = line.partition('#')[0]

                fields = [field.strip() for field in line.split(';')]

                # The canonical name of the property is in field 2. The other
                # fields contain other aliases.
                properties.add(Property(fields[1], aliases=fields))

def parse_value_aliases(path, properties):
    'Parses the file listing the value aliases for each property.'

    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.rstrip()
            if line and not line.startswith('#'):
                # It's a data line.
                #
                # Ignore anything after the comment, if any.
                line = line.partition('#')[0]

                fields = [field.strip() for field in line.split(';')]

                # The name of the property is in field 1 and the canonical name
                # of the value is in either field 3 or field 4. The other
                # fields contain other aliases.
                value_name = fields[3] if fields[0] == 'ccc' else fields[2]
                properties[fields[0]].add(Value(value_name, aliases=fields[1 :
                  ]))

def parse_unicode_data_file(unicode_folder, filename, next_property_id,
  properties):
    'Parses a Unicode data file.'

    report('Parsing {}'.format(filename))
    property = None
    value = None
    is_binary = True
    next_value_id = 0

    with open(join(unicode_folder, filename), encoding='utf-8') as file:
        for line in file:
            line = line.rstrip()
            if line.startswith('#'):
                # It's a comment line.
                if line.startswith(('# Property:',
                  '#  All code points not explicitly listed for')):
                    # This line is giving the name of the property.
                    is_binary = False
                    property = properties[line.split()[-1]]
                    if property.id is None:
                        property.id = next_property_id
                        next_property_id += 1

                    if property.name == 'General_Category':
                        property.default = property['Unassigned']
                elif line.startswith('# @missing:'):
                    # This line is giving the name of the default value.
                    is_binary = False
                    value = property[line.split()[-1]]
                    value.id = 0
                    next_value_id = 1
                    property.default = value
            elif line:
                # It's a data line.
                fields = line.partition('#')[0].split(';')
                codepoints = [int(c, 16) for c in fields[0].split('..')]
                name = fields[1].strip()

                if is_binary:
                    # It's a binary property.
                    property = properties[name]
                    if property.id is None:
                        property.id = next_property_id
                        next_property_id += 1

                    property.default = property['No']
                    property['No'].id = 0
                    property['Yes'].id = 1

                    value = property['Yes']
                else:
                    # It's a multi-value property.
                    value = property[name]
                    if value.id is None:
                        value.id = next_value_id
                        next_value_id += 1

                value.add((codepoints[0], codepoints[-1]))

    return next_property_id

def parse_numeric_values_file(unicode_folder, filename, next_property_id,
  properties):
    'Parses the Unicode numeric value data file.'

    report('Parsing {}'.format(filename))
    property = None
    value = None
    next_value_id = 0

    with open(join(unicode_folder, filename), encoding='utf-8') as file:
        for line in file:
            line = line.rstrip()
            if line.startswith('#'):
                # It's a comment line.
                if line.startswith('# Derived Property:'):
                    # This line is giving the name of the property.
                    property = properties[line.split()[-1]]
                    if property.id is None:
                        property.id = next_property_id
                        next_property_id += 1

                    if property.default is None:
                        name= 'NaN'
                        value = property.get(name)
                        if value is None:
                            value = NumericValue(name, id=0)
                            next_value_id = 1
                            property.add(value)

                        property.default = value
                elif line.startswith('# @missing:'):
                    # This line is giving the name of the default value.
                    name = line.split()[-1]
                    value = property.get(name)
                    if value is None:
                        value = NumericValue(name, id=0)
                        next_value_id = 1
                        property.add(value)

                    property.default = value
            elif line:
                # It's a data line.
                fields = line.partition('#')[0].split(';')
                codepoints = [int(c, 16) for c in fields[0].split('..')]
                name = fields[3].strip()

                value = property.get(name)
                if value is None:
                    value = NumericValue(name, id=next_value_id)
                    next_value_id += 1
                    property.add(value)

                value.add((codepoints[0], codepoints[-1]))

    return next_property_id

def parse_emoji_file(unicode_folder, filename, next_property_id, properties):
    'Parses the Unicode emoji data file.'

    report('Parsing {}'.format(filename))
    property = None
    value = None
    next_value_id = 0

    with open(join(unicode_folder, filename), encoding='utf-8') as file:
        for line in file:
            line = line.rstrip()
            if line.startswith('#'):
                # It's a comment line.
                if line.startswith('# @missing:'):
                    # This line is giving the name of the property.
                    fields = line.split()
                    property = Property(fields[-3])
                    properties.add(property)
                    assert fields[-1] == 'No'
                    if property.id is None:
                        property.id = next_property_id
                        next_property_id += 1

                    if property.default is None:
                        name = 'No'
                        value = property.get(name)
                        if value is None:
                            value = Value(name, id=0)
                            next_value_id = 1
                            property.add(value)

                        property.default = value
            elif line:
                # It's a data line.
                fields = line.partition('#')[0].split(';')
                codepoints = [int(c, 16) for c in fields[0].split('..')]
                name = 'Yes'
                value = property.get(name)
                if value is None:
                    value = Value(name, id=next_value_id)
                    next_value_id += 1
                    property.add(value)

                value.add((codepoints[0], codepoints[-1]))

    return next_property_id

def parse_linebreak_file(unicode_folder, filename, next_property_id, properties):
    'Parses the Unicode linebreak data file.'

    report('Parsing {}'.format(filename))
    property = Property('Line_Break')
    properties.add(property)
    if property.id is None:
        property.id = next_property_id
        next_property_id += 1

    value = None
    next_value_id = 0

    with open(join(unicode_folder, filename), encoding='utf-8') as file:
        for line in file:
            line = line.rstrip()
            if line.startswith('#'):
                # It's a comment line.
                if line.startswith('# @missing:'):
                    # This line is giving the name of the default value.
                    name = line.split(';')[-1].strip()
                    value = property.get(name)
                    if value is None:
                        value = NumericValue(name, id=0)
                        next_value_id = 1
                        property.add(value)

                    property.default = value
            elif line:
                # It's a data line.
                fields = line.partition('#')[0].split(';')
                codepoints = [int(c, 16) for c in fields[0].split('..')]
                name = fields[1].strip()
                value = property.get(name)
                if value is None:
                    value = Value(name, id=next_value_id)
                    next_value_id += 1
                    property.add(value)

                value.add((codepoints[0], codepoints[-1]))

    return next_property_id

def parse_unicode_normalisation_file(unicode_folder, filename,
  next_property_id, properties):
    'Parses a Unicode normalisation file.'

    report('Parsing {}'.format(filename))
    property = None
    value = None
    next_value_id = 0

    with open(join(unicode_folder, filename), encoding='utf-8') as file:
        for line in file:
            line = line.rstrip()
            if line.startswith('#'):
                # It's a comment line.
                if line.startswith('# Derived Property:'):
                    property = None
                elif line.startswith('# Property:'):
                    # This line is giving the name of the property.
                    property = properties[line.split()[-1]]
                    if property.id is None:
                        property.id = next_property_id
                        next_property_id += 1
                elif property and line.startswith('# @missing:'):
                    # This line is giving the name of the default value.
                    value = property[line.split()[-1]]
                    value.id = 0
                    next_value_id = 1
                    property.default = value
            elif property and line:
                # It's a data line.
                fields = line.partition('#')[0].split(';')
                codepoints = [int(c, 16) for c in fields[0].split('..')]
                name = fields[2].strip()

                # It's a multi-value property.
                value = property[name]
                if value.id is None:
                    value.id = next_value_id
                    next_value_id += 1

                value.add((codepoints[0], codepoints[-1]))

    return next_property_id

def parse_script_extensions_file(unicode_folder, filename, next_property_id,
  properties):
    'Parses the Unicode Script Extensions file.'

    report('Parsing {}'.format(filename))

    scripts = properties['Script']

    property = properties['ScriptExtensions']
    if property.id is None:
        property.id = next_property_id
        property.default = scripts.default
        next_property_id += 1

    for value in scripts:
        if value.id is not None:
            property.add(Value(value.name, aliases=value.aliases, id=value.id))

    next_value_id = max(value.id for value in property) + 1

    with open(join(unicode_folder, filename), encoding='utf-8') as file:
        for line in file:
            line = line.rstrip()
            if line.startswith('#'):
                pass
            elif line:
                # It's a data line.
                fields = line.partition('#')[0].split(';')
                codepoints = [int(c, 16) for c in fields[0].split('..')]
                script_names = fields[1].strip()

                # Ensure that the script names are known.
                for name in script_names.split():
                    scripts[name]

                try:
                    value = property[script_names]
                except KeyError:
                    value = Value(script_names, id=next_value_id)
                    property.add(value)
                    next_value_id += 1

                value.add((codepoints[0], codepoints[-1]))

    return next_property_id

def make_additional_properties(properties, next_property_id):
    'Makes the additional properties.'

    general_category = properties['General_Category']

    # Make the 'Alphanumeric' property.
    alphabetic = properties['Alphabetic']['Yes'].ranges
    decimal_number = general_category['Decimal_Number'].ranges

    next_property_id = make_binary_property(properties, next_property_id,
      'Alphanumeric', alphabetic + decimal_number, aliases=['AlNum'])

    # Make the 'Any' property.
    next_property_id = make_Any_property(properties, next_property_id)

    # General_Category has a compound value called 'Assigned'.
    mask = 0
    ranges = []

    for value in general_category:
        if value.id is not None:
            mask |= 1 << value.id

        if value.name != 'Unassigned':
            ranges.extend(value.ranges)

    value = Value('Assigned', mask=mask)
    value.add(ranges)
    general_category.add(value)

    # Make the 'Blank' property.
    assigned = []

    for value in general_category:
        if value is not general_category.default:
            assigned.extend(value.ranges)

    space_separator = general_category['Space_Separator'].ranges
    blank = [(0x09, 0x09)] + space_separator

    next_property_id = make_binary_property(properties, next_property_id,
      'Blank', blank)

    # Make the 'Graph' property.
    whitespace = properties['White_Space']['Yes'].ranges
    control = general_category['Control'].ranges
    surrogate = general_category['Surrogate'].ranges

    unassigned = range_difference([(0, MAX_CODEPOINT)], assigned)

    graph = range_difference([(0, MAX_CODEPOINT)], whitespace + control +
      surrogate + unassigned)

    next_property_id = make_binary_property(properties, next_property_id,
      'Graph', graph)

    # Make the 'Print' property.
    print_ = range_difference(graph + blank, control)

    next_property_id = make_binary_property(properties, next_property_id,
      'Print', print_)

    # Make the 'Word' property.
    enclosing_mark = general_category['Enclosing_Mark'].ranges
    nonspacing_mark = general_category['Nonspacing_Mark'].ranges
    spacing_mark = general_category['Spacing_Mark'].ranges
    connector_punctuation = general_category['Connector_Punctuation'].ranges
    join_control = properties['Join_Control']['Yes'].ranges

    word = (alphabetic + enclosing_mark + nonspacing_mark + spacing_mark +
      decimal_number + connector_punctuation + join_control)

    next_property_id = make_binary_property(properties, next_property_id,
      'Word', word)

    # Make the 'XDigit' property.
    hex_digit = properties['Hex_Digit']['Yes'].ranges

    xdigit = decimal_number + hex_digit

    next_property_id = make_binary_property(properties, next_property_id,
      'XDigit', xdigit)

    # Make the 'Posix_Digit' property.
    posix_digit = [(ord('0'), ord('9'))]

    next_property_id = make_binary_property(properties, next_property_id,
      'Posix_Digit', posix_digit)

    # Make the 'Posix_AlNum' property.
    posix_alnum = alphabetic + posix_digit

    next_property_id = make_binary_property(properties, next_property_id,
      'Posix_AlNum', posix_alnum)

    # Make the 'Posix_Punct' property.
    punctuation = []
    for name in 'Pd Ps Pe Pc Po Pi Pf'.split():
        punctuation.extend(general_category[name].ranges)

    symbol = general_category['Symbol'].ranges
    for name in 'Sm Sc Sk So '.split():
        symbol.extend(general_category[name].ranges)

    posix_punct = range_difference(punctuation + symbol, alphabetic)

    next_property_id = make_binary_property(properties, next_property_id,
      'Posix_Punct', posix_punct)

    # Make the 'Posix_XDigit' property.
    posix_xdigit = [(ord('0'), ord('9')), (ord('A'), ord('F')), (ord('a'),
     ord('f'))]

    next_property_id = make_binary_property(properties, next_property_id,
      'Posix_XDigit', posix_xdigit)

    # The 'General_Category' property has a ompound value called 'Cased_Letter'.
    lc_value = general_category['Cased_Letter']
    ranges = []

    for name in 'Lu Ll Lt'.split():
        lc_value.mask |= 1 << general_category[name].id
        ranges.extend(general_category[name].ranges)

    lc_value.add(ranges)

    # The 'General_Category' property has some compound values called 'Letter',
    # 'Number', etc.
    compound = []
    ids = []

    for value in general_category:
        for alias in list(value.aliases):
            if len(alias) == 1:
                # Add the additional alias for the compound value.
                value.aliases.append(alias + '&')
                compound.append(alias)
            elif len(alias) == 2 and alias.istitle():
                # Build the mask for the compound value.
                c_value = general_category[alias[0]]
                c_value.mask |= 1 << value.id
                c_value.add(value.ranges)
                ids.append(value.id)

    last_value_id = max(ids)

    for id, name in enumerate(sorted(compound), start=last_value_id + 1):
        general_category[name].id = id

    last_value_id += len(compound)

    for value in general_category:
        if value.id is None:
            last_value_id += 1
            value.id = last_value_id

    for property in properties:
        used = []
        not_defined = []
        has_codepoints = False

        for value in property:
            if value.id is None:
                not_defined.append(value)
            else:
                used.append(value.id)

            if value.ranges:
                has_codepoints = True

        if used:
            for id, value in enumerate(not_defined, start=max(used) + 1):
                value.id = id
        elif has_codepoints:
            for id, value in enumerate(not_defined):
                value.id = id

    return next_property_id

def make_binary_property(properties, next_property_id, name, ranges,
  aliases=[]):
    'Makes a binary property.'

    yes_aliases = properties['Alphabetic']['Yes'].aliases
    no_aliases = properties['Alphabetic']['No'].aliases

    property = Property(name, aliases=aliases)
    property.id = next_property_id

    value = Value('No', aliases=no_aliases)
    property.add(value)

    property.default = value

    value = Value('Yes', aliases=yes_aliases)
    value.add(merge_ranges(ranges))
    property.add(value)

    properties.add(property)

    return next_property_id + 1

def make_Any_property(properties, next_property_id):
    'Makes the Any property.'

    yes_aliases = properties['Alphabetic']['Yes'].aliases
    no_aliases = properties['Alphabetic']['No'].aliases

    property = AnyProperty()
    property.id = next_property_id

    value = Value('No', aliases=no_aliases)
    property.add(value)

    property.default = value

    value = Value('Yes', aliases=yes_aliases)
    value.add((0x0000, MAX_CODEPOINT))
    property.add(value)

    properties.add(property)

    return next_property_id + 1

def parse_case_folding_file(path):
    'Parses the Unicode CaseFolding file.'

    simple_folding = defaultdict(list)
    full_folding = defaultdict(list)
    equivalent = defaultdict(set)
    turkic_set = set()
    empty_set = set()

    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.rstrip()
            if line and not line.startswith('#'):
                # It's a data line.
                fields = line.partition('#')[0].split(';')
                codepoint = int(fields[0], 16)
                fold_type = fields[1].strip()
                folded = [int(f, 16) for f in fields[2].split()]

                if fold_type in 'CFS':
                    # We want to determine which codepoints are equivalent to
                    # each other under case-folding.

                    # These are equivalent to each other.
                    equiv_set = {(codepoint, ), tuple(folded)}

                    # First collect the existing equivalences.
                    for x in list(equiv_set):
                        equiv_set.update(equivalent.get(x, empty_set))

                    # Then make them all refer to the same equivalence set.
                    for x in equiv_set:
                        equivalent[x] = equiv_set

                # The first entry is the delta.
                entries = tuple([folded[0] - codepoint] + folded[1 : ])

                if fold_type in 'CS':
                    # Common and simple case-folding.
                    simple_folding[entries[0]].append(codepoint)

                if fold_type in 'CF':
                    # Common and fall case-folding.
                    full_folding[entries].append(codepoint)

                if fold_type == 'T':
                    # Turkic case-folding.
                    turkic_set.add((codepoint, tuple(folded)))

    # Is the Turkic set what we expected?
    if turkic_set != {(0x49, (0x131, )), (0x130, (0x69, ))}:
        raise UnicodeDataError('Turkic set has changed')

    # Which codepoints expand on full case-folding?
    expand_set = set()

    for expanded in full_folding:
        if len(expanded) > 1:
            expand_set.update(full_folding[expanded])

    # Add the Turkic set to the equivalences. Note that:
    #
    #    dotted_capital == dotted_small
    #
    # and:
    #
    #    dotted_small == dotless_capital
    #
    # but:
    #
    #    dotted_capital != dotless_capital
    #
    for codepoint, folded in turkic_set:
        char1, char2 = (codepoint, ), folded
        equivalent[char1] = equivalent[char1] | {char2}
        equivalent[char2] = equivalent[char2] | {char1}

    # Collect and sort the equivalences. We want to know all of the possible
    # case-forms for each codepoint.
    other_cases = defaultdict(list)

    for codepoint, equiv_set in equivalent.items():
        if len(codepoint) == 1:
            diff_list = []

            for e in equiv_set - {codepoint}:
                if len(e) == 1:
                    diff_list.append(e[0] - codepoint[0])

            other_cases[tuple(sorted(diff_list))].append(codepoint[0])

    # Save the all-cases property.
    all_prop = AllCases('All_Cases', other_cases)

    # Save the simple case-folding property.
    simple_folding_prop = SimpleCaseFolding('Simple_Case_Folding',
      simple_folding)

    # Save the full case-folding property.
    full_folding_prop = FullCaseFolding('Full_Case_Folding', full_folding)

    return all_prop, simple_folding_prop, full_folding_prop, expand_set

def tabulate(rows):
    'Creates a table with right-justified columns.'

    # Convert all the entries to strings.
    rows = [[str(e) for e in row] for row in rows]

    # Determine the widths of the columns.
    widths = [max(len(e) for e in column) for column in zip(*rows)]

    # Pad all the entries.
    rows = [[e.rjust(w) for e, w in zip(row, widths)] for row in rows]

    return rows

def write_properties_description(properties, properties_path):
    'Writes the Unicode properties description file.'

    with open(properties_path, 'w', encoding='utf-8', newline='\n') as file:
        file.write('The following is a list of the {} properties which are supported by this module:\n'.format(len(properties)))

        for property in sorted(properties, key=lambda property: property.name.upper()):
            file.write('\n')

            name = property.name
            aliases = sorted(property.aliases, key=str.upper)
            if aliases:
                file.write('{} [{}]\n'.format(name, ', '.join(aliases)))
            else:
                file.write('{}\n'.format(name))

            for value in sorted(property, key=lambda value: value.name.upper()):
                name = value.name
                aliases = sorted(value.aliases, key=str.upper)
                if aliases:
                    file.write('    {} [{}]\n'.format(name,
                      ', '.join(aliases)))
                else:
                    file.write('    {}\n'.format(name))

def store_properties(unicode_version, properties, path):
    'Stores the Unicode properties in a file.'

    with open(path, 'w', encoding='ascii') as file:
        file.write('Version {}\n'.format(unicode_version))
        for property in sorted(properties, key=lambda p: p.name.upper()):
            aliases = [property.name] + sorted(property.aliases, key=str.upper)
            file.write('Property {}\n'.format(', '.join('"{}"'.format(name) for
              name in aliases)))

            value = property.default
            if value is None:
                raise UnicodeDataError('property {!a} has no default value'.format(property.name))

            aliases = [value.name] + sorted(value.aliases, key=str.upper)
            file.write('DefaultValue {}\n'.format(', '.join('"{}"'.format(name)
              for name in aliases)))

            for value in sorted(property, key=lambda v: v.name.upper()):
                if value is not property.default:
                    aliases = [value.name] + sorted(value.aliases, key=str.upper)
                    file.write('Value {}'.format(', '.join('"{}"'.format(name)
                      for name in aliases)))

                    if value.mask:
                        file.write(' : 0x{:08X}\n'.format(value.mask))
                    else:
                        file.write('\n')

                    for lower, upper in value.ranges:
                        if lower == upper:
                            file.write('{:04X}\n'.format(lower))
                        else:
                            file.write('{:04X}..{:04X}\n'.format(lower, upper))

def check_unicode_version(unicode_data_files):
    'Checks the Unicode version in the data files.'

    versions = defaultdict(set)

    # Read the version and filename from the first line of each data file.
    for line in unicode_data_files.splitlines():
        if line and line[0] != '#' and line[0] != '[':
            name = line.rpartition('/')[-1]
            path = join(unicode_folder, name)

            with open(path, encoding='utf-8') as file:
                line = file.readline()

                if line == '# emoji-data.txt\n':
                    for line in file:
                        if line.startswith('# Version:'):
                            ver = line.split()[-1]
                            ver += '.0' * (2 - ver.count('.'))
                            versions[ver].add(name)
                            break
                elif line.endswith('.txt\n'):
                    ver = line[ : -5].rpartition('-')[2]
                    versions[ver].add(name)

    if len(versions) != 1:
        if set(versions) == {'12.0.0', '12.1.0'} and versions['12.0.0'] == {'emoji-data.txt'}:
            # Cannot find emoji-data.txt for version 12.1.0.
            pass
        else:
            for ver, names in versions.items():
                print(ver, '=>', [name.strip('# \n') for name in names])

            raise ValueError('expected 1 version of Unicode, but found {}'.format(len(versions)))

    def make_key(ver):
        return tuple(map(int, ver.split('.')))

    return max(versions, key=make_key)

def download_files(unicode_version, unicode_data_files):
    'Downloads the Unicode data files from the website.'

    for line in unicode_data_files.splitlines():
        if line and line[0] != '#' and line[0] != '[':
            base, ext = splitext(line.rpartition('/')[-1])
            versioned_path = join(unicode_folder, '{}-{}{}'.format(base, unicode_version, ext))

            if not isfile(versioned_path):
                url = urljoin(unicode_data_base, line)
                path = join(unicode_folder, line.rpartition('/')[-1])

                try:
                    download_unicode_file(url, path)
                except HTTPError:
                    print('{} not found'.format(url))

def merge_ranges(ranges):
    'Sorts and merges a list of codepoint ranges.'

    ranges = sorted(ranges)
    new_ranges = ranges[ : 1]

    for lower, upper in ranges:
        if lower <= new_ranges[-1][1] + 1:
            new_ranges[-1] = (new_ranges[-1][0], max(upper, new_ranges[-1][1]))
        else:
            new_ranges.append((lower, upper))

    return new_ranges

def range_difference(include_ranges, exclude_ranges):
    'Returns the difference between a list of inclusive ranges and a list of exclusive ranges.'

    new_ranges = []

    include = IterRanges(merge_ranges(include_ranges))
    exclude = IterRanges(merge_ranges(exclude_ranges))

    lower = include.lower

    while lower <= MAX_CODEPOINT:
        if lower < include.lower:
            # We're below the current include range.
            # Advance into the range.
            lower = include.lower
        elif lower > include.upper:
            # We're above the current include range.
            # Advance into the next include range.
            include.next()
            lower = max(lower, include.lower)
        elif lower < exclude.lower:
            # We're below the current exclude range.
            # Accept codepoints as far as the end of the include range.
            upper = min(include.upper, exclude.lower - 1)
            new_ranges.append((lower, upper))
            lower = upper + 1
        elif lower > exclude.upper:
            # We're above the current exclude range.
            exclude.next()
        else:
            # We're within both the include and exclude ranges.
            # Advance out of the overlap.
            upper = min(include.upper, exclude.upper)
            lower = upper + 1

    return new_ranges

class IterRanges:
    'Class for iterating over a list of codepoint ranges.'

    def __init__(self, ranges):
        self._ranges = ranges
        self._index = 0

        if self._index < len(self._ranges):
            self.lower, self.upper = self._ranges[self._index]
        else:
            self.lower, self.upper = MAX_CODEPOINT + 1, MAX_CODEPOINT + 1

    def next(self):
        if self._index < len(self._ranges):
            self._index += 1

        if self._index < len(self._ranges):
            self.lower, self.upper = self._ranges[self._index]
        else:
            self.lower, self.upper = MAX_CODEPOINT + 1, MAX_CODEPOINT + 1

def generate_code():
    'Generates the code.'

    h_path = join(code_folder, '_regex_unicode.h')
    c_path = join(code_folder, '_regex_unicode.c')

    # Create the list of standardised strings.
    strings = []

    for property in properties:
        strings.append(property.name)
        strings.extend(property.aliases)

        if property.name != 'Script_Extensions':
            for value in property:
                strings.append(value.name)
                strings.extend(value.aliases)

    def reduce_name(name):
        return name.replace('_', '').upper()

    def by_id(obj):
        return obj.id

    strings = sorted(set(map(reduce_name, strings)))

    # The maximum number of scripts/codepoint in the Script_Extensions property.
    max_scx = max(len(val.name.split()) for val in
      properties['Script_Extensions'])

    with open(h_path , 'w', encoding='ascii', newline='\n') as h_file, \
      open(c_path, 'w', encoding='ascii', newline='\n') as c_file:
        # Useful definitions.
        h_file.write('''\
typedef unsigned char RE_UINT8;
typedef signed char RE_INT8;
typedef unsigned short RE_UINT16;
typedef signed short RE_INT16;
typedef unsigned int RE_UINT32;
typedef signed int RE_INT32;

typedef unsigned char BOOL;
#if !defined(FALSE) || !defined(TRUE)
#define FALSE 0
#define TRUE 1
#endif

#define RE_ASCII_MAX 0x7F
#define RE_LOCALE_MAX 0xFF
#define RE_UNICODE_MAX 0x10FFFF

#define RE_MAX_CASES {max_cases}
#define RE_MAX_FOLDED {max_folded}
#define RE_MAX_SCX {max_scx}

typedef struct RE_Property {{
    RE_UINT16 name;
    RE_UINT8 id;
    RE_UINT8 value_set;
}} RE_Property;

typedef struct RE_PropertyValue {{
    RE_UINT16 name;
    RE_UINT8 value_set;
    RE_UINT16 id;
}} RE_PropertyValue;

typedef RE_UINT32 (*RE_GetPropertyFunc)(RE_UINT32 ch);

'''.format(max_cases=all_cases.max_cases,
  max_folded=full_case_folding.max_folded, max_scx=max_scx))

        RE_Property_size = 4
        RE_PropertyValue_size = 4

        for name in ('GC', 'Cased', 'Uppercase', 'Lowercase', 'SCX'):
            h_file.write('#define RE_PROP_{} 0x{:X}\n'.format(name.upper(),
              properties[name].id))

        h_file.write('\n')

        values = []
        compound = []

        for value in properties['GC']:
            if value.mask:
                for alias in value.aliases:
                    if len(alias) == 1:
                        compound.append((alias, value))
                        break
                else:
                    compound.append((reduce_name(value.name.upper()), value))
            else:
                for alias in value.aliases:
                    if len(alias) == 2 and alias.isalpha():
                        values.append((alias, value))

        values.sort(key=lambda pair: pair[1].id)
        compound.sort(key=lambda pair: (len(pair[0]) > 1, pair[0]))

        for alias, value in compound:
            h_file.write('#define RE_PROP_{} {}\n'.format(alias, value.id))

        h_file.write('\n')

        for alias, value in values:
            h_file.write('#define RE_PROP_{} {}\n'.format(alias.upper(),
              value.id))

        h_file.write('\n')

        for alias, value in compound:
            if len(alias) == 1:
                h_file.write('#define RE_PROP_{}_MASK 0x{:08X}\n'.format(alias,
                  value.mask))

        h_file.write('\n')

        # Common abbreviated properties.
        common_properties = 'AlNum Alpha Any ASCII Blank Cntrl Digit Graph Lower Print Space Upper Word XDigit Posix_AlNum Posix_Digit Posix_Punct Posix_XDigit'

        for name in common_properties.split():
            property = properties.get(name)
            if property is not None:
                h_file.write('#define RE_PROP_{} 0x{:06X}\n'.format(name.upper(),
                  property.id << 16 | 1))
            else:
                for prop_name in 'GC Script Block'.split():
                    property = properties[prop_name]
                    value = property.get(name)
                    if value is not None:
                        h_file.write('#define RE_PROP_{} 0x{:06X}\n'.format(name.upper(),
                          property.id << 16 | value.id))
                        break
                else:
                    raise UnicodeDataError('unknown abbreviated property: {!a}'.format(name))

        h_file.write('\n')

        for value in sorted(properties['Word_Break'], key=by_id):
            h_file.write('#define RE_WBREAK_{} {}\n'.format(reduce_name(value.name),
              value.id))

        h_file.write('\n')

        for value in sorted(properties['Grapheme_Cluster_Break'], key=by_id):
            h_file.write('#define RE_GBREAK_{} {}\n'.format(reduce_name(value.name),
              value.id))

        h_file.write('\n')

        for value in sorted(properties['Line_Break'], key=by_id):
            h_file.write('#define RE_LBREAK_{} {}\n'.format(reduce_name(value.name),
              value.id))

        c_file.write('''/* For Unicode version {version} */

#include "_regex_unicode.h"
'''.format(version=unicode_version))

        # Write the standardised strings.
        c_file.write('''
#define RE_BLANK_MASK ((1 << RE_PROP_ZL) | (1 << RE_PROP_ZP))
#define RE_GRAPH_MASK ((1 << RE_PROP_CC) | (1 << RE_PROP_CS) | (1 << RE_PROP_CN))
#define RE_WORD_MASK (RE_PROP_M_MASK | (1 << RE_PROP_ND) | (1 << RE_PROP_PC))

typedef struct RE_ScriptExt {
    RE_UINT8 scripts[RE_MAX_SCX];
} RE_ScriptExt;

typedef struct RE_AllCases {
    RE_INT32 diffs[RE_MAX_CASES - 1];
} RE_AllCases;

typedef struct RE_FullCaseFolding {
    RE_INT32 diff;
    RE_UINT16 codepoints[RE_MAX_FOLDED - 1];
} RE_FullCaseFolding;

/* strings. */

char* re_strings[] = {
''')

        bytesize = 0

        for string in strings:
            s = reduce_name(string)
            c_file.write('    "{}",\n'.format(s))
            bytesize += len(s) + 1

        h_file.write('\nextern char* re_strings[{}];\n'.format(len(strings)))

        c_file.write('''}};

/* strings: {bytesize} bytes. */
'''.format(bytesize=bytesize))

        # Write the property name tables.
        #
        # Properties which are aliases have the same property id, and
        # properties, such as binary properties, which have the same set of
        # values have the same value set id.

        # The rows of the property and value tables.
        property_rows = []
        value_rows = []

        # The value sets.
        value_sets = {}

        # Give an id to each distinct property or value name.
        strings = {string: id for id, string in enumerate(strings)}

        for property in sorted(properties, key=by_id):
            prop = property

            if prop.name == 'Script_Extensions':
                # The Script Extensions property name the same values as the
                # Script property.
                prop = properties['Script']

            value_set = tuple((value.name, tuple(value.aliases)) for value in
              prop)
            new_value_set = value_set not in value_sets
            value_set_id = value_sets.setdefault(value_set, len(value_sets))

            # name of property, id of property, id of value set
            property_rows.append((strings[reduce_name(property.name)],
              property.id, value_set_id))
            for alias in property.aliases:
                property_rows.append((strings[reduce_name(alias)], property.id,
                  value_set_id))

            # We don't want to duplicate value sets.
            if new_value_set:
                for value in sorted(prop, key=by_id):
                    # name of value, id of value set, value
                    value_rows.append((strings[reduce_name(value.name)],
                      value_set_id, value.id))
                    for alias in value.aliases:
                        value_rows.append((strings[reduce_name(alias)],
                          value_set_id, value.id))

        # Columns of RE_Property are:
        #     RE_UINT16 name
        #     RE_UINT8 id
        #     RE_UINT8 value_set
        fields = [('name', 16), ('id', 8), ('value_set', 8)]
        max_values = [max(column) for column in zip(*property_rows)]

        for (name, width), max_value in zip(fields, max_values):
            if max_value >> width:
                raise ValueError("'{}' field of RE_Property struct is too small".format(name))

        # Columns of RE_PropertyValue are:
        #     RE_UINT16 name
        #     RE_UINT8 value_set
        #     RE_UINT16 id
        fields = [('name', 16), ('value_set', 8), ('id', 16)]
        max_values = [max(column) for column in zip(*value_rows)]

        for (name, width), max_value in zip(fields, max_values):
            if max_value >> width:
                raise ValueError("'{}' field of RE_PropertyValue struct is too small".format(name))

        # Fix the column widths of the tables.
        property_rows = tabulate(property_rows)
        value_rows = tabulate(value_rows)

        expand_data_type, expand_data_size = smallest_datatype(min(expand_set),
          max(expand_set))

        c_file.write('''
/* properties. */

RE_Property re_properties[] = {
''')

        h_file.write('''\
extern RE_Property re_properties[{prop_rows}];
extern RE_PropertyValue re_property_values[{val_rows}];
extern {data_type} re_expand_on_folding[{expand_rows}];
extern RE_GetPropertyFunc re_get_property[{func_count}];

'''.format(prop_rows=len(property_rows), val_rows=len(value_rows),
  data_type=expand_data_type, expand_rows=len(expand_set),
  func_count=len(properties)))

        for row in property_rows:
            c_file.write('    {{{}}},\n'.format(', '.join(row)))

        c_file.write('''\
}};

/* properties: {bytesize} bytes. */

/* property values. */

RE_PropertyValue re_property_values[] = {{
'''.format(bytesize=RE_Property_size * len(property_rows)))

        for row in value_rows:
            c_file.write('    {{{}}},\n'.format(', '.join(row)))

        c_file.write('''\
}};

/* property values: {bytesize} bytes. */

/* Codepoints which expand on full case-folding. */

{data_type} re_expand_on_folding[] = {{
'''.format(bytesize=RE_PropertyValue_size * len(value_rows),
  data_type=expand_data_type))

        items = ['{},'.format(c) for c in sorted(expand_set)]
        width = max(len(i) for i in items)
        items = [i.rjust(width) for i in items]

        columns = 8
        for start in range(0, len(items), columns):
            c_file.write('    {}\n'.format(' '.join(items[start : start +
              columns])))

        c_file.write('''}};

/* expand_on_folding: {bytesize} bytes. */
'''.format(bytesize=len(items) * expand_data_size))

        # Build and write the property data tables.
        for index, property in enumerate(sorted(properties, key=by_id)):
            report('{} of {}: {}'.format(index + 1, len(properties),
              property.name))
            property.generate_code(h_file, c_file)
            report()

        all_cases.generate_code(h_file, c_file)
        report()
        simple_case_folding.generate_code(h_file, c_file)
        report()
        full_case_folding.generate_code(h_file, c_file)
        report()

        # Write the property function array.
        c_file.write('''
/* Property function table. */

RE_GetPropertyFunc re_get_property[] = {
''')

        for property in sorted(properties, key=by_id):
            report('{} is {}'.format(property.name, property.id))

            if property.name == 'Script_Extensions':
                c_file.write('    0,\n')
                continue

            c_file.write('    re_get_{},\n'.format(property.name.lower()))

        c_file.write('};\n')

def smallest_datatype(min_value, max_value):
    'Determines the smallest C data type which can store values in a range.'

    # 1 byte, unsigned and signed.
    if 0 <= min_value <= max_value <= 0xFF:
        return 'RE_UINT8', 1
    if -0x80 <= min_value <= max_value <= 0x7F:
        return 'RE_INT8', 1

    # 2 bytes, unsigned and signed.
    if 0 <= min_value <= max_value <= 0xFFFF:
        return 'RE_UINT16', 2
    if -0x8000 <= min_value <= max_value <= 0x7FFF:
        return 'RE_INT16', 2

    # 4 bytes, unsigned and signed.
    if 0 <= min_value <= max_value <= 0xFFFFFFFF:
        return 'RE_UINT32', 4
    if -0x80000000 <= min_value <= max_value <= 0x7FFFFFFF:
        return 'RE_INT32', 4

    raise ValueError('value range too big for 32 bits')

# Whether to update the Unicode data files from the Unicode website.
UPDATE = False
#UPDATE = True
UNICODE_VERSION = '12.1.0'

# Whether to recalculate the best block sizes for the tables.
RECALC = False

# The URL from which the Unicode data files can be obtained.
unicode_data_base = 'http://www.unicode.org/Public/UNIDATA/'

# The local folder in which the Unicode data files are stored.
unicode_folder = r'D:\projects\Unicode\Data'

# The local folder in which the generated C files should be written.
code_folder = join(dirname(__file__), 'regex')

# The path of the file in which the block sizes are stored.
json_path = splitext(__file__)[0] + '.json'

COLUMNS = 16
MAX_CODEPOINT = 0x10FFFF
NUM_CODEPOINTS = 0x110000
MAX_STAGES = 4
MAX_CASE_STAGES = 4 # Fewer stages for case info because it's used frequently.

# The Unicode data files. The file names are given relative to the website URL.
unicode_data_files = '''\
[properties]
extracted/DerivedGeneralCategory.txt
Blocks.txt
Scripts.txt
ScriptExtensions.txt
auxiliary/WordBreakProperty.txt
auxiliary/GraphemeBreakProperty.txt
auxiliary/SentenceBreakProperty.txt
DerivedCoreProperties.txt
PropList.txt
HangulSyllableType.txt
extracted/DerivedBidiClass.txt
extracted/DerivedCombiningClass.txt
extracted/DerivedDecompositionType.txt
extracted/DerivedEastAsianWidth.txt
extracted/DerivedJoiningGroup.txt
extracted/DerivedJoiningType.txt
extracted/DerivedLineBreak.txt
extracted/DerivedNumericType.txt
extracted/DerivedNumericValues.txt
extracted/DerivedBinaryProperties.txt
IndicPositionalCategory.txt
IndicSyllabicCategory.txt
emoji-data.txt
#LineBreak.txt
[normalisation]
DerivedNormalizationProps.txt
[others]
PropertyAliases.txt
PropertyValueAliases.txt
PropList.txt
CaseFolding.txt
#UnicodeData.txt
'''

# Ensure that we have downloaded the Unicode data files for UNICODE_VERSION
# from the Unicode website.
#
# Returns True if any we get any new ones.
if UPDATE:
    download_files(UNICODE_VERSION, unicode_data_files)

# Which version of Unicode is it?
unicode_version = check_unicode_version(unicode_data_files)

# The set of properties.
properties = PropertySet()

# Parse the Unicode data files.
parse_property_files(unicode_folder, unicode_data_files, properties)

storage_path = join(dirname(__file__), 'codepoint_properties.txt')

store_properties(unicode_version, properties, storage_path)
base, ext = splitext(storage_path)
copy2(storage_path, '{}-{}{}'.format(base, unicode_version, ext))

# Parse the case-folding data specially.
path = join(unicode_folder, 'CaseFolding.txt')
all_cases, simple_case_folding, full_case_folding, expand_set = parse_case_folding_file(path)

properties_path = join(dirname(__file__), 'UnicodeProperties.txt')
write_properties_description(properties, properties_path)

# Load the sizes of the table blocks.
try:
    with open(json_path, encoding='utf-8') as json_file:
        storage = load(json_file)
except (FileNotFoundError, ValueError):
    storage = {}

# If it's a different version of Unicode, forget the saved block sizes.
if RECALC or storage.get('version') != unicode_version:
    storage.clear()

# Generate the _regex_unicode.h and _regex_unicode.c files.
generate_code()

# Store the sizes of the table blocks.
storage['version'] = unicode_version

with open(json_path, 'w', encoding='utf-8') as json_file:
    dump(storage, json_file)

report('\nThere are {} properties'.format(len(properties)))

# Collect and summarise a few statistics.
import re
c_path = join(code_folder, '_regex_unicode.c')
code = open(c_path).read()
sizes = defaultdict(int)
for n, s in re.findall(r'(\w+(?: \w+)*): (\d+) bytes', code):
    sizes[n] += int(s)
sizes = sorted(sizes.items(), key=lambda pair: pair[1], reverse=True)
total_size = sum(s for n, s in sizes)
report('\nTotal: {} bytes\n'.format(total_size))
prop_width = max(len(row[0]) for row in sizes)
prop_width = max(prop_width, 8)
storage_width = max(len(str(row[1])) for row in sizes)
storage_width = max(storage_width, 7)
report('{:{}}  {:{}}  {}'.format('Property', prop_width, 'Storage',
  storage_width, 'Percentage'))
report('{:{}}  {:{}}  {}'.format('--------', prop_width, '-------',
  storage_width, '----------'))
format = '{{:<{}}}  {{:>{}}}    {{:>5.1%}}'.format(prop_width, storage_width)
for n, s in sizes:
    report(format.format(n, s, s / total_size))

report('\nFinished!')
