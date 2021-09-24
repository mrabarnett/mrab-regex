#! python3.9
# -*- coding: utf-8 -*-
#
# This Python script parses the Unicode data files and generates the C files
# for the regex module.
#
# Written by MRAB.
#
from contextlib import suppress
from itertools import chain
from os import listdir, mkdir
from os.path import basename, dirname, exists, join, normpath
from urllib.parse import urljoin
from urllib.request import urlretrieve
from time import time

import codecs
import sys
sys.stdout = codecs.getwriter('utf-8')(sys.stdout.detach())

class Timed:
    def __init__(self, message=None):
        self._message = message

    def __enter__(self):
        self._start = time()

    def __exit__(self, exc_type, exc_val, exc_tb):
        finish = time()
        elapsed = finish - self._start

        if self._message is None:
            print(f'Took {elapsed:0.2f} secs')
        else:
            print(f'{self._message} took {elapsed:0.2f} secs')

        return False

def unique(iterable, key=None):

    if key is None:
        def key(item):
            return item

    seen = set()

    for item in iterable:
        k = key(item)

        if k not in seen:
            seen.add(k)
            yield item

class IterRanges:
    def __init__(self, ranges):
        self._ranges = ranges
        self._pos = 0
        self._update()

    def next(self):
        if self._pos >= len(self._ranges):
            return

        self._pos += 1
        self._update()

    def _update(self):
        if self._pos < len(self._ranges):
            self.lower, self.upper = self._ranges[self._pos]
        else:
            self.lower = self.upper = NUM_CODEPOINTS

class Ranges:
    def __init__(self, initial=None):
        self._ranges = []

        if initial is not None:
            self._ranges.extend(initial)

        self._is_normalised = initial is None

    def add(self, lower, upper=None):
        if upper is None:
            self._ranges.append((lower, lower))
        else:
            self._ranges.append((lower, upper))

        self._is_normalised = False

    def __or__(self, other):
        return Ranges(self._ranges + other._ranges)

    def __sub__(self, other):
        self._normalise()
        other._normalise()

        include = IterRanges(self._ranges)
        exclude = IterRanges(other._ranges)
        new_ranges = []

        lower = include.lower

        while lower < NUM_CODEPOINTS:
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

        return Ranges(new_ranges)

    def __iter__(self):
        self._normalise()

        return iter(self._ranges)

    def __len__(self):
        self._normalise()

        return len(self._ranges)

    def lowest(self):
        self._normalise()

        return self._ranges[0][0]

    def __repr__(self):
        self._normalise()

        return 'Ranges({!r})'.format(self._ranges)

    def _normalise(self):
        if self._is_normalised:
            return

        if len(self._ranges) >= 2:
            self._ranges.sort()

            new_ranges = []
            lower, upper = self._ranges[0]

            for l, u in self._ranges[1 : ]:
                if l - upper > 1:
                    new_ranges.append((lower, upper))
                    lower, upper = l, u
                else:
                    lower = min(lower, l)
                    upper = max(upper, u)

            new_ranges.append((lower, upper))

            self._ranges = new_ranges

        self._is_normalised = True

munge_dict = str.maketrans({'-': '', '_': '', ' ': ''})

def munge(value):
    munged_value = value.translate(munge_dict).upper()

    if value.startswith('-'):
        munged_value = '-' + munged_value

    return munged_value

def download_unicode_files(unicode_data_base, data_files, data_folder):
    for section in data_files.values():
        for rel_path in section:
            path = normpath(join(data_folder, basename(rel_path)))

            if not exists(path):
                url = urljoin(unicode_data_base, rel_path)
                print('Downloading {} from {}'.format(rel_path, url),
                  flush=True)
                urlretrieve(url, path)

def parse_property_aliases(data_folder):
    properties = {}

    path = join(data_folder, 'PropertyAliases.txt')

    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            if not line or line.startswith('#'):
                continue

            fields = [field.strip() for field in line.split(';')]
            prop_name = fields.pop(1)

            property = {'names': list(unique([prop_name] + fields, key=munge))}

            for name in property['names']:
                properties[munge(name)] = property

    return properties

def parse_value_aliases(data_folder, properties):
    path = join(data_folder, 'PropertyValueAliases.txt')

    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            if not line or line.startswith('#'):
                continue

            line = line.partition('#')[0]
            fields = [field.strip() for field in line.split(';')]
            prop_name = fields.pop(0)
            val_name = fields.pop(2 if prop_name == 'ccc' else 1)

            property = properties[munge(prop_name)]
            value = {'names': list(unique([val_name] + fields, key=munge))}
            values = property.setdefault('values', {})

            for name in value['names']:
                values[munge(name)] = value

def parse_binary(properties, path):
    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            if not line or line.startswith('#'):
                continue

            line = line.partition('#')[0]
            fields = [field.strip() for field in line.split(';')]
            codepoints = [int(part, 16) for part in fields[0].split('..')]
            prop_name = fields[1]
            property = properties[munge(prop_name)]
            property.setdefault('default', munge('No'))
            value = property['values'][munge('Yes')]
            value.setdefault('codepoints', Ranges()).add(codepoints[0],
              codepoints[-1])

def parse_emoji(properties, path):
    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            if not line:
                continue

            if line.startswith('# @missing:'):
                fields = line.split()
                prop_name = fields[-3]

                try:
                    property = properties[munge(prop_name)]
                except KeyError:
                    property = {'names': [prop_name], 'values': {}}
                    value = {'names': ['No', 'N']}
                    property['values'][munge(value['names'][0])] = value
                    value = {'names': ['Yes', 'Y']}
                    property['values'][munge(value['names'][0])] = value
                    properties[munge(prop_name)] = property

                default = fields[-1]
                property['default'] = munge(default)
            elif not line.startswith('#'):
                line = line.partition('#')[0]
                fields = [field.strip() for field in line.split(';')]
                codepoints = [int(part, 16) for part in fields[0].split('..')]
                prop_name = fields[1]
                property = properties[munge(prop_name)]
                property.setdefault('default', munge('No'))

                try:
                    value = property['values'][munge('Yes')]
                except KeyError:
                    value = {'names': ['Yes']}
                    property['values'][munge('Yes')] = value

                value.setdefault('codepoints', Ranges()).add(codepoints[0],
                  codepoints[-1])

def parse_multivalue(properties, path):
    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            if not line:
                continue

            if line.startswith('# Property:'):
                prop_name = line.split()[-1]
                property = properties[munge(prop_name)]
            elif line.startswith('#  All code points not explicitly listed for'):
                prop_name = line.split()[-1]
                property = properties[munge(prop_name)]
            elif line.startswith('# @missing:'):
                default = line.split()[-1]
                property['default'] = munge(default)
            elif not line.startswith('#'):
                line = line.partition('#')[0]
                fields = [field.strip() for field in line.split(';')]
                codepoints = [int(part, 16) for part in fields[0].split('..')]
                val_name = fields[1]
                value = property['values'][munge(val_name)]
                value.setdefault('codepoints', Ranges()).add(codepoints[0],
                  codepoints[-1])

def parse_normalisation(properties, path):
    property = None

    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            if not line:
                continue

            if line.startswith('# Derived Property:'):
                property = None
            elif line.startswith('# Property:'):
                prop_name = line.split()[-1]
                property = properties[munge(prop_name)]
            elif property:
                if line.startswith('# @missing:'):
                    default = line.split()[-1]
                    property['default'] = munge(default)
                elif not line.startswith('#'):
                    line = line.partition('#')[0]
                    fields = [field.strip() for field in line.split(';')]
                    codepoints = [int(part, 16) for part in
                      fields[0].split('..')]
                    val_name = fields[2]

                    value = property['values'][munge(val_name)]
                    value.setdefault('codepoints', Ranges()).add(codepoints[0],
                      codepoints[-1])

def parse_numeric_values(properties, path):
    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            if not line:
                continue

            if line.startswith('# Derived Property:'):
                prop_name = line.split()[-1]
                property = properties[munge(prop_name)]
                default = {'names': ['NaN']}
                property['values'] = {munge('NaN'): default}
                property['default'] = munge('NaN')
            elif line.startswith('# @missing:'):
                default = line.split()[-1]
                property['default'] = munge(default)
            elif not line.startswith('#'):
                line = line.partition('#')[0]
                fields = [field.strip() for field in line.split(';')]
                codepoints = [int(part, 16) for part in fields[0].split('..')]
                val_name = fields[3]

                try:
                    value = property['values'][munge(val_name)]
                except KeyError:
                    value = {'names': [val_name]}
                    property['values'][munge(val_name)] = value

                value.setdefault('codepoints', Ranges()).add(codepoints[0],
                  codepoints[-1])

def parse_script_extensions(properties, path):
    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            if not line:
                continue

            if line.startswith('# Property:'):
                prop_name = line.split()[-1]
                property = properties[munge(prop_name)]
                property['values'] = {}
            elif not line.startswith('#'):
                line = line.partition('#')[0]
                fields = [field.strip() for field in line.split(';')]
                codepoints = [int(part, 16) for part in fields[0].split('..')]

                key = tuple(sorted(fields[1].split(), key=str.lower))

                try:
                    value = property['values'][key]
                except KeyError:
                    value = {'codepoints': Ranges()}
                    property['values'][key] = value

                value['codepoints'].add(codepoints[0], codepoints[-1])

def parse_case_folding(properties, path):
    simple_folding = {}
    full_folding = {}
    turkic_set = set()

    with open(path, encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            if not line or line.startswith('#'):
                continue

            line = line.partition('#')[0]
            fields = line.split(';')
            codepoint = int(fields[0], 16)
            kind = fields[1].strip()
            folded = [int(part, 16) for part in fields[2].split()]
            delta = folded[0] ^ codepoint

            if kind in {'S', 'C', 'T'}:
                simple_folding.setdefault(delta, Ranges()).add(codepoint,
                  codepoint)

            if kind in {'F', 'C', 'T'}:
                key = tuple([delta] + folded[1 : ])
                full_folding.setdefault(key, Ranges()).add(codepoint,
                  codepoint)

            if kind == 'T':
                turkic_set.add((codepoint, tuple(folded)))

    # Is the Turkic set what we expected?
    if turkic_set != {(0x49, (0x131, )), (0x130, (0x69, ))}:
        raise ValueError('Turkic set has changed')

    properties['simple_folding'] = simple_folding
    properties['full_folding'] = full_folding

def parse_unicode_files(data_files, data_folder):
    properties = parse_property_aliases(data_folder)
    parse_value_aliases(data_folder, properties)

    def ignore(*args):
        pass

    parsers = {
        'aliases': ignore,
        'binary': parse_binary,
        'emoji': parse_emoji,
        'multivalue': parse_multivalue,
        'normalisation': parse_normalisation,
        'numeric_values': parse_numeric_values,
        'script_extensions': parse_script_extensions,
        'case_folding': parse_case_folding,
    }

    for section, rel_paths in data_files.items():
        parse = parsers[section]

        for rel_path in rel_paths:
            data_file = basename(rel_path)

            print('Parsing {}'.format(data_file), flush=True)
            parse(properties, join(data_folder, data_file))

    unicode_data = {'properties': {}}

    for prop_name, property in properties.items():
        if has_codepoints(property):
            unicode_data['properties'][prop_name] = property
        elif prop_name in {'simple_folding', 'full_folding'}:
            unicode_data[prop_name] = property

    properties = unicode_data['properties']
    property = properties[munge('General_Category')]
    property['default'] = munge('Unassigned')

    values = property['values']

    for val_name, value in list(values.items()):
        if len(val_name) == 1:
            new_name = val_name.upper() + '&'
            values[munge(new_name)] = value
            value['names'].append(new_name)

    return unicode_data

def make_binary_property(properties, names, codepoints):
    no_value = {'names': ['No', 'N', 'F', 'False']}
    yes_value = {'names': ['Yes', 'Y', 'T', 'True'], 'codepoints': codepoints}
    values = {}

    for value in [no_value, yes_value]:
        for name in value['names']:
            values[munge(name)] = value

    property = {'names': names, 'values': values, 'default': munge('No')}

    for name in names:
        properties[munge(name)] = property

def make_additional_properties(unicode_data):

    def get_values(prop_name):
        return properties[munge(prop_name)]['values']

    def get_codepoints(prop_name, val_name):
        return get_values(prop_name)[munge(val_name)]['codepoints']

    properties = unicode_data['properties']

    # Make the 'Alphanumeric' property.
    alphabetic = get_codepoints('Alphabetic', 'Yes')
    decimal_number = get_codepoints('General_Category', 'Decimal_Number')

    make_binary_property(properties, ['Alphanumeric', 'AlNum'], alphabetic |
      decimal_number)

    # Make the 'Any' property.
    make_binary_property(properties, ['Any'], Ranges([(0, NUM_CODEPOINTS -
      1)]))

    # General_Category has a compound value called 'Assigned'.
    assigned = Ranges()

    for value in unique(get_values('General_Category').values(), key=id):
        if value['names'][0] != 'Unassigned':
            try:
                assigned |= value['codepoints']
            except KeyError:
                pass

    value = {'names': ['Assigned']}
    properties[munge('General_Category')]['values'][munge('Assigned')] = value

    # Make the 'Blank' property.
    space_separator = get_codepoints('General_Category', 'Space_Separator')
    blank = Ranges([(0x09, 0x09)]) | space_separator

    make_binary_property(properties, ['Blank'], blank)

    # Make the 'Graph' property.
    whitespace = get_codepoints('White_Space', 'Yes')
    control = get_codepoints('General_Category', 'Control')
    surrogate = get_codepoints('General_Category', 'Surrogate')

    graph = assigned - (whitespace | control | surrogate)

    make_binary_property(properties, ['Graph'], graph)

    # Make the 'Print' property.
    print_ = (graph | blank) - control

    make_binary_property(properties, ['Print'], print_)

    # Make the 'Word' property.
    enclosing_mark = get_codepoints('General_Category', 'Enclosing_Mark')
    nonspacing_mark = get_codepoints('General_Category', 'Nonspacing_Mark')
    spacing_mark = get_codepoints('General_Category', 'Spacing_Mark')
    connector_punctuation = get_codepoints('General_Category',
      'Connector_Punctuation')
    join_control = get_codepoints('Join_Control', 'Yes')

    word = (alphabetic | enclosing_mark | nonspacing_mark | spacing_mark |
      decimal_number | connector_punctuation | join_control)

    make_binary_property(properties, ['Word'], word)

    # Make the 'XDigit' property.
    hex_digit = get_codepoints('Hex_Digit', 'Yes')

    xdigit = decimal_number | hex_digit

    make_binary_property(properties, ['XDigit'], xdigit)

    # Make the 'Posix_Digit' property.
    posix_digit = Ranges([(ord('0'), ord('9'))])

    make_binary_property(properties, ['Posix_Digit'], posix_digit)

    # Make the 'Posix_AlNum' property.
    posix_alnum = alphabetic | posix_digit

    make_binary_property(properties, ['Posix_AlNum'], posix_alnum)

    # Make the 'Posix_Punct' property.
    punctuation = Ranges()

    for name in 'Pd Ps Pe Pc Po Pi Pf'.split():
        punctuation |= get_codepoints('General_Category', name)

    symbol = Ranges()

    for name in 'Sm Sc Sk So '.split():
        symbol |= get_codepoints('General_Category', name)

    posix_punct = (punctuation | symbol) - alphabetic

    make_binary_property(properties, ['Posix_Punct'], posix_punct)

    # Make the 'Posix_XDigit' property.
    posix_xdigit = Ranges([(ord('0'), ord('9')), (ord('A'), ord('F')),
      (ord('a'), ord('f'))])

    make_binary_property(properties, ['Posix_XDigit'], posix_xdigit)

def preferred(d):
    return munge(d['names'][0])

def has_codepoints(property):
    if 'values' not in property:
        return False

    return any('codepoints' in value for value in property['values'].values())

def write_summary(unicode_data, tools_folder):
    print('Writing summary')

    properties = unicode_data['properties']

    path = join(tools_folder, 'Unicode.txt')

    with open(path, 'w', encoding='ascii') as file:
        file.write('Version {}\n'.format(UNICODE_VERSION))

        for property in sorted(unique(properties.values(), key=id),
          key=preferred):
            if not has_codepoints(property):
                print(property['names'][0])
                continue

            file.write('Property {}\n'.format(' '.join(property['names'])))

            values = property['values']

            if property['names'][0] == 'Script_Extensions':
                for key in sorted(values):
                    value = values[key]
                    file.write('Value {}\n'.format(' '.join(key)))

                    for lower, upper in value.get('codepoints', []):
                        if lower == upper:
                            file.write('{:04X}\n'.format(lower))
                        else:
                            file.write('{:04X}..{:04X}\n'.format(lower, upper))
            else:
                if 'default' in property:
                    default = values[property['default']]
                    file.write('DefaultValue {}\n'.format(default['names'][0]))

                for value in sorted(unique(values.values(), key=id),
                  key=preferred):
                    file.write('Value {}\n'.format(' '.join(value['names'])))

                    for lower, upper in value.get('codepoints', []):
                        if lower == upper:
                            file.write('{:04X}\n'.format(lower))
                        else:
                            file.write('{:04X}..{:04X}\n'.format(lower, upper))

        file.write('SimpleFolding\n')

        for delta, ranges in unicode_data['simple_folding'].items():
            file.write('Value {:04X}\n'.format(delta))

            for lower, upper in ranges:
                if lower == upper:
                    file.write('{:04X}\n'.format(lower))
                else:
                    file.write('{:04X}..{:04X}\n'.format(lower, upper))

        file.write('FullFolding\n')

        for key, ranges in unicode_data['full_folding'].items():
            file.write('Value {}\n'.format(' '.join('{:04X}'.format(value) for
              value in key)))

            for lower, upper in ranges:
                if lower == upper:
                    file.write('{:04X}\n'.format(lower))
                else:
                    file.write('{:04X}..{:04X}\n'.format(lower, upper))

def make_binary_dict():
    binary_dict = {}

    for n in range(0x100):
        key = tuple(map(int, format(n, '08b')[ : : -1]))
        binary_dict[key] = n

    return binary_dict

def collect_strings(properties):
    strings = []

    for property in properties.values():
        try:
            strings.extend(property['names'])

            for value in property['values'].values():
                strings.extend(value['names'])
        except KeyError:
            pass

    return sorted(set(munge(string) for string in strings))

def chunked(iterable, chunk_size):
    sequence = iterable
    count = len(sequence)

    for start in range(0, count, chunk_size):
        chunk = sequence[start : start +  chunk_size]
        yield chunk

def determine_entry_type(iterable):
    lower, upper = min(iterable), max(iterable)

    if 0 <= lower <= upper <= 0xFF:
        return 'RE_UINT8'

    if 0 <= lower <= upper <= 0xFFFF:
        return 'RE_UINT16'

    raise ValueError('cannot determine C type for {}..{}'.format(lower, upper))

def is_binary(property):
    return sum(1 for val in val_list if val['id'] != 0) == 1

def count_ranges(property):
    count = 0
    default_id = property['values'][munge(property['default'])]['id']

    for value in unique(property['values'].values(), key=id):
        if value['id'] != default_id:
            count += len(value.get('codepoints', []))

    return count

def generate_small_lookup(property, c_file):
    c_file.write('''
/* {}. */
RE_UINT32 re_get_{}(RE_UINT32 codepoint) {{
'''.format(property['names'][0], property['names'][0].lower()))

    default_id = property['values'][munge(property['default'])]['id']
    ranges = []

    for value in unique(property['values'].values(), key=id):
        if value['id'] != default_id:
            val_id = value['id']

            for lower, upper in value.get('codepoints', []):
                ranges.append((lower, upper, val_id))

    if len(ranges) == 1 and ranges[0][ : 2] == (0, NUM_CODEPOINTS - 1):
        c_file.write('    return {};\n}}\n'.format(ranges[0][2]))
    else:
        for lower, upper, val_id in ranges:
            width = 2 if upper <= 0xFF else 4 if upper <= 0xFFFF else 6

            if lower == upper:
                c_file.write('''\
    if (codepoint == 0x{:0{width}X})
        return {};
'''.format(lower, val_id, width=width))
            else:
                c_file.write('''\
    if (0x{:0{width}X} <= codepoint && codepoint <= 0x{:0{width}X})
        return {};
'''.format(lower, upper, val_id, width=width))

        c_file.write('\n    return {};\n}}\n'.format(default_id))

def generate_table(table_name, values, c_file, max_columns=16, public=False):
    entry_type = determine_entry_type(values)

    if public:
        c_file.write('{} {}[] = {{\n'.format(entry_type, table_name))
    else:
        c_file.write('static {} {}[] = {{\n'.format(entry_type, table_name))

    entries = [str(value) for value in values]
    max_width = max(len(entry) for entry in entries)
    entries = [entry.rjust(max_width) + ',' for entry in entries]
    entries[-1] = entries[-1].rstrip(',')

    for chunk in chunked(entries, max_columns):
        c_file.write('    %s\n' % ' '.join(chunk))

    c_file.write('};\n')

def generate_lookup(property, c_file):
    val_list = list(unique(property['values'].values(), key=id))

    if count_ranges(property) <= 8:
        generate_small_lookup(property, c_file)
        return

    default_id = property['values'][munge(property['default'])]['id']
    entries = [default_id] * NUM_CODEPOINTS

    for value in val_list:
        val_id = value['id']

        for lower, upper in value.get('codepoints', []):
            entries[lower : upper + 1] = [val_id] * (upper - lower + 1)

    CHUNK_SIZE = 32

    indexes = []
    chunks = {}

    for chunk in chunked(tuple(entries), CHUNK_SIZE):
        indexes.append(chunks.setdefault(chunk, len(chunks)))

    table_2 = list(chain(*sorted(chunks, key=chunks.get)))

    entries = indexes
    indexes = []
    chunks = {}

    for start in range(0, len(entries), CHUNK_SIZE):
        chunk = tuple(entries[start : start + CHUNK_SIZE])
        indexes.append(chunks.setdefault(chunk, len(chunks)))

    table_1 = list(chain(*sorted(chunks, key=chunks.get)))

    table_0 = indexes

    c_file.write('\n/* {}. */\n'.format(property['names'][0]))

    prop_name = property['names'][0].lower()
    binary = set(table_2) == {0, 1}

    for i, table in enumerate([table_0, table_1, table_2]):
        if i == 2 and binary:
            binary = True
            entries = []

            for start in range(0, len(table), 8):
                entries.append(binary_dict[tuple(table[start : start + 8])])

            table = entries

        if i > 0:
            c_file.write('\n')

        generate_table('re_{}_table_{}'.format(prop_name, 1 + i), table,
          c_file)

    if binary:
        c_file.write('''
RE_UINT32 re_get_{0}(RE_UINT32 codepoint) {{
    RE_UINT32 field_2;
    RE_UINT32 field_1;
    RE_UINT32 field_0;
    RE_UINT32 offset;
    RE_UINT32 v;

    field_2 = codepoint >> 10;
    field_1 = (codepoint >> 5) & 0x1F;
    field_0 = (codepoint >> 3) & 0x3;
    offset = codepoint & 0x7;

    v = re_{0}_table_1[field_2];
    v = re_{0}_table_2[(v << 5) | field_1];
    v = re_{0}_table_3[(v << 2) | field_0];

    return (v >> offset) & 0x1;
}}
'''.format(prop_name))
    else:
        c_file.write('''
RE_UINT32 re_get_{0}(RE_UINT32 codepoint) {{
    RE_UINT32 field_2;
    RE_UINT32 field_1;
    RE_UINT32 field_0;
    RE_UINT32 v;

    field_2 = codepoint >> 10;
    field_1 = (codepoint >> 5) & 0x1F;
    field_0 = codepoint & 0x1F;

    v = re_{0}_table_1[field_2];
    v = re_{0}_table_2[(v << 5) | field_1];
    v = re_{0}_table_3[(v << 5) | field_0];

    return v;
}}
'''.format(prop_name))

def generate_script_extensions_lookup(properties, property, c_file):
    entries = [0] * NUM_CODEPOINTS

    # Initialise with script.
    val_list = unique(properties[munge('Script')]['values'].values(), key=id)

    for value in val_list:
        val_id = value['id']

        for lower, upper in value.get('codepoints', []):
            entries[lower : upper + 1] = [val_id] * (upper - lower + 1)

    script_count = 1 + max(value['id'] for value in
      properties[munge('Script')]['values'].values())

    val_list = unique(property['values'].values(), key=id)

    for value in val_list:
        val_id = value['id']

        for lower, upper in value.get('codepoints', []):
            entries[lower : upper + 1] = [val_id] * (upper - lower + 1)

    CHUNK_SIZE = 32

    indexes = []
    chunks = {}

    for chunk in chunked(entries, CHUNK_SIZE):
        indexes.append(chunks.setdefault(tuple(chunk), len(chunks)))

    table_2 = list(chain(*sorted(chunks, key=chunks.get)))

    entries = indexes
    indexes = []
    chunks = {}

    for start in range(0, len(entries), CHUNK_SIZE):
        chunk = tuple(entries[start : start + CHUNK_SIZE])
        indexes.append(chunks.setdefault(chunk, len(chunks)))

    table_1 = list(chain(*sorted(chunks, key=chunks.get)))

    table_0 = indexes

    c_file.write('\n/* {}. */\n'.format(property['names'][0]))

    prop_name = property['names'][0].lower()

    for i, table in enumerate([table_0, table_1, table_2]):
        generate_table('{}_table_{}'.format(prop_name, 1 + i), table, c_file)

    script_values = properties[munge('Script')]['values']
    ext_dict = {}

    for key, value in property['values'].items():
        ext_dict[value['id']] = [script_values[munge(name)]['id'] for name in
          key]

    offsets = []
    entries = []

    for key, value in sorted(ext_dict.items()):
        offsets.append(len(entries))
        entries.extend(value + [0])

    generate_table('{}_table_4'.format(prop_name), offsets, c_file)

    generate_table('{}_table_5'.format(prop_name), entries, c_file)

    c_file.write('''
int re_get_{0}(RE_UINT32 codepoint, RE_UINT8* scripts) {{
    RE_UINT32 field_2;
    RE_UINT32 field_1;
    RE_UINT32 field_0;
    RE_UINT32 v;
    int offset;
    int count;

    field_2 = codepoint >> 10;
    field_1 = (codepoint >> 5) & 0x1F;
    field_0 = codepoint & 0x1F;

    v = {0}_table_1[field_2];
    v = {0}_table_2[(v << 5) | field_1];
    v = {0}_table_3[(v << 5) | field_0];

    if (v < {1}) {{
        scripts[0] = v;

        return 1;
    }}

    offset = {0}_table_4[v - {1}];
    count = 0;

    do {{
        scripts[count] = {0}_table_5[offset + count];
        ++count;
    }} while ({0}_table_5[offset + count] != 0);

    return count;
}}
'''.format(prop_name, script_count))

def generate_all_cases(unicode_data, c_file):
    simple_folding = unicode_data['simple_folding']

    all_cases = {}

    for delta, ranges in simple_folding.items():
        for lower, upper in ranges:
            for codepoint in range(lower, upper + 1):
                folded = codepoint ^ delta
                all_cases.setdefault(folded, set()).update({codepoint, folded})

    for codepoint in list(all_cases):
        cases = {codepoint} | all_cases.get(codepoint, set())

        for c in list(cases):
            cases |= all_cases.get(c, set())

        for c in cases:
            all_cases[c] = cases

    all_cases[0x49] = {0x49, 0x69, 0x131} # Dotless capital I.
    all_cases[0x69] = {0x69, 0x49, 0x130} # Dotted small I.
    all_cases[0x130] = {0x130, 0x69} # Dotted capital I.
    all_cases[0x131] = {0x131, 0x49} # Dotless small I.

    entries = [0] * NUM_CODEPOINTS
    others_dict = {(0, ): 0}

    for codepoint, cases in all_cases.items():
        others = sorted(cases - {codepoint})
        key = tuple([others[0] ^ codepoint] + others[1 : ])
        entries[codepoint] = others_dict.setdefault(key, len(others_dict))

    CHUNK_SIZE = 32

    indexes = []
    chunks = {}

    for chunk in chunked(entries, CHUNK_SIZE):
        indexes.append(chunks.setdefault(tuple(chunk), len(chunks)))

    table_2 = list(chain(*sorted(chunks, key=chunks.get)))

    entries = indexes
    indexes = []
    chunks = {}

    for start in range(0, len(entries), CHUNK_SIZE):
        chunk = tuple(entries[start : start + CHUNK_SIZE])
        indexes.append(chunks.setdefault(chunk, len(chunks)))

    table_1 = list(chain(*sorted(chunks, key=chunks.get)))

    table_0 = indexes

    c_file.write('\n/* All cases. */\n')

    for i, table in enumerate([table_0, table_1, table_2]):
        if i > 0:
            c_file.write('\n')

        generate_table('re_all_cases_table_{}'.format(1 + i), table, c_file)

    c_file.write('\nstatic RE_AllCases re_all_cases_table_4[] = {\n')

    max_columns = max(len(value) for value in others_dict)

    max_width = max(len(str(item)) for value in others_dict for item in value)
    fmt = '    {{{:%d}, {{' % max_width + ', '.join(['{:%d}' % max_width] *
      (max_columns -1)) + '}}}},\n'

    lines = []

    for values in sorted(others_dict, key=others_dict.get):
        values = list(values) + [0] * max_columns
        lines.append(fmt.format(*values))

    lines[-1] = lines[-1].rstrip(',\n') + '\n'

    c_file.writelines(lines)

    c_file.write('};\n')

    c_file.write('''
int re_get_all_cases(RE_UINT32 codepoint, RE_UINT32* cases) {
    RE_UINT32 field_2;
    RE_UINT32 field_1;
    RE_UINT32 field_0;
    RE_UINT32 v;

    field_2 = codepoint >> 10;
    field_1 = (codepoint >> 5) & 0x1F;
    field_0 = codepoint & 0x1F;

    v = re_all_cases_table_1[field_2];
    v = re_all_cases_table_2[(v << 5) | field_1];
    v = re_all_cases_table_3[(v << 5) | field_0];

    cases[0] = codepoint;

    if (re_all_cases_table_4[v].delta == 0)
        return 1;

    cases[1] = codepoint ^ re_all_cases_table_4[v].delta;

    if (re_all_cases_table_4[v].others[0] == 0)
        return 2;

    cases[2] = re_all_cases_table_4[v].others[0];

    if (re_all_cases_table_4[v].others[1] == 0)
        return 3;

    cases[3] = re_all_cases_table_4[v].others[1];

    return 4;
}
''')

def generate_simple_case_folding(unicode_data, c_file):
    simple_folding = unicode_data['simple_folding']

    entries = [0] * NUM_CODEPOINTS
    value_dict = {0: 0}

    for delta, ranges in sorted(simple_folding.items()):
        val_id = value_dict.setdefault(delta, len(value_dict))

        for lower, upper in ranges:
            entries[lower : upper + 1] = [val_id] * (upper - lower + 1)

    CHUNK_SIZE = 32

    indexes = []
    chunks = {}

    for chunk in chunked(entries, CHUNK_SIZE):
        indexes.append(chunks.setdefault(tuple(chunk), len(chunks)))

    table_2 = list(chain(*sorted(chunks, key=chunks.get)))

    entries = indexes
    indexes = []
    chunks = {}

    for start in range(0, len(entries), CHUNK_SIZE):
        chunk = tuple(entries[start : start + CHUNK_SIZE])
        indexes.append(chunks.setdefault(chunk, len(chunks)))

    table_1 = list(chain(*sorted(chunks, key=chunks.get)))

    table_0 = indexes

    c_file.write('\n/* Simple case folding. */\n')

    for i, table in enumerate([table_0, table_1, table_2]):
        if i > 0:
            c_file.write('\n')

        generate_table('re_simple_folding_table_{}'.format(1 + i), table, c_file)

    c_file.write('\nstatic RE_UINT16 re_simple_folding_table_4[] = {\n')

    entries = [str(value) for value in sorted(value_dict, key=value_dict.get)]
    max_width = max(len(entry) for entry in entries)
    entries = [entry.rjust(max_width) + ',' for  entry in entries]
    entries[-1] = entries[-1].rstrip(',')

    for chunk in chunked(entries, 8):
        c_file.write('    %s\n' % ' '.join(chunk))

    c_file.write('};\n')

    c_file.write('''
RE_UINT32 re_get_simple_case_folding(RE_UINT32 codepoint) {
    RE_UINT32 field_2;
    RE_UINT32 field_1;
    RE_UINT32 field_0;
    RE_UINT32 v;

    field_2 = codepoint >> 10;
    field_1 = (codepoint >> 5) & 0x1F;
    field_0 = codepoint & 0x1F;

    v = re_simple_folding_table_1[field_2];
    v = re_simple_folding_table_2[(v << 5) | field_1];
    v = re_simple_folding_table_3[(v << 5) | field_0];

    return codepoint ^ re_simple_folding_table_4[v];
}
''')

def generate_full_case_folding(unicode_data, c_file):
    full_folding = unicode_data['full_folding']

    entries = [0] * NUM_CODEPOINTS
    value_dict = {(0, ): 0}

    for delta, ranges in sorted(full_folding.items()):
        val_id = value_dict.setdefault(delta, len(value_dict))

        for lower, upper in ranges:
            entries[lower : upper + 1] = [val_id] * (upper - lower + 1)

    CHUNK_SIZE = 32

    indexes = []
    chunks = {}

    for chunk in chunked(entries, CHUNK_SIZE):
        indexes.append(chunks.setdefault(tuple(chunk), len(chunks)))

    table_2 = list(chain(*sorted(chunks, key=chunks.get)))

    entries = indexes
    indexes = []
    chunks = {}

    for start in range(0, len(entries), CHUNK_SIZE):
        chunk = tuple(entries[start : start + CHUNK_SIZE])
        indexes.append(chunks.setdefault(chunk, len(chunks)))

    table_1 = list(chain(*sorted(chunks, key=chunks.get)))

    table_0 = indexes

    c_file.write('\n/* Full case folding. */\n')

    for i, table in enumerate([table_0, table_1, table_2]):
        if i > 0:
            c_file.write('\n')

        generate_table('re_full_folding_table_{}'.format(1 + i), table, c_file)

    c_file.write('\nstatic RE_FullCaseFolding re_full_folding_table_4[] = {\n')

    max_folded = max(len(value) for value in value_dict)
    max_width = max(len(str(item)) for value in value_dict for item in value)
    rows = [(value + (0, ) * max_folded)[ : max_folded] for value in
      sorted(value_dict, key=value_dict.get)]
    fmt = ('    {{{{' + ', '.join(['{:%d}' % max_width] * max_folded) +
      '}}}},\n').format
    lines = []

    for row in rows:
        lines.append(fmt(*row))

    lines[-1] = lines[-1].rstrip(',\n') + '\n'

    c_file.writelines(lines)

    c_file.write('};\n')

    c_file.write('''
int re_get_full_case_folding(RE_UINT32 codepoint, RE_UINT32* folded) {
    RE_UINT32 field_2;
    RE_UINT32 field_1;
    RE_UINT32 field_0;
    RE_UINT32 v;
    RE_UINT16* data;

    field_2 = codepoint >> 10;
    field_1 = (codepoint >> 5) & 0x1F;
    field_0 = codepoint & 0x1F;

    v = re_full_folding_table_1[field_2];
    v = re_full_folding_table_2[(v << 5) | field_1];
    v = re_full_folding_table_3[(v << 5) | field_0];

    data = re_full_folding_table_4[v].data;
    folded[0] = codepoint ^ data[0];

    if (data[1] == 0)
        return 1;

    folded[1] = data[1];

    if (data[2] == 0)
        return 2;

    folded[2] = data[2];

    return 3;
}
''')

def generate_code(unicode_data, tools_folder):
    print('Generating code')

    # Codepoints that expand on full casefolding.
    expanded = []

    for key, ranges in unicode_data['full_folding'].items():
        if len(key) > 1:
            for lower, upper in ranges:
                expanded.extend(range(lower, upper + 1))

    expanded.sort()

    # Assign the property and value IDs.
    properties = unicode_data['properties']
    prop_list = list(unique(properties.values(), key=id))
    prop_list.sort(key=preferred)

    unicode_data['property_tablee_count'] = len(properties)
    unicode_data['property_count'] = len(prop_list)

    no_yes_maybe = {
        'NO', 'N', 'FALSE', 'F',
        'YES', 'Y', 'TRUE', 'T',
        'MAYBE', 'M',
    }

    yes_no_maybe_dict = {'No': 0, 'Yes': 1, 'Maybe': 2}

    for prop_id, property in enumerate(prop_list):
        property['id'] = prop_id

        if property['names'][0] == 'Script_Extensions':
            script_count = 1 + max(val['id'] for val in
              properties[munge('Script')]['values'].values())

            def make_key(value):
                return value['codepoints'].lowest()

            val_list = list(unique(property['values'].values(), key=id))
            val_list.sort(key=make_key)

            for val_id, value in enumerate(val_list):
                value['id'] = script_count + val_id
        else:
            default = property['default']

            if not (set(property['values']) - no_yes_maybe):

                def make_key(value):
                    return yes_no_maybe_dict[value['names'][0]]

            else:

                def make_key(value):
                    if munge(value['names'][0]) == default:
                        return (0, )

                    if 'codepoints' not in value:
                        return (2, )

                    return 1, value['codepoints'].lowest()

            val_list = list(unique(property['values'].values(), key=id))
            val_list.sort(key=make_key)

            def make_key(val):
                name_list = [name for name in val['names'] if '&' in name]

                if name_list:
                    return 1, name_list[0][0]

                return 0

            if property['names'][0] == 'General_Category':

                def make_key(value):
                    for name in value['names']:
                        if '&' in name:
                            return (1, name)

                    if value.get('codepoints'):
                        return (0, )

                    return (2, munge(value['names'][0]))

                for val_id, value in enumerate(sorted(val_list, key=make_key)):
                    value['id'] = val_id
            else:
                for val_id, value in enumerate(val_list):
                    value['id'] = val_id

    # Collect the value sets.
    valueset_dict = {}

    for property in sorted(prop_list, key=lambda prop: prop['id']):
        prop_name = property['names'][0]

        if prop_name == 'Script_Extensions':
            property['valueset_id'] = properties[munge('Script')]['valueset_id']
        else:
            valueset = []

            val_list = list(unique(property['values'].values(), key=id))

            for value in sorted(val_list, key=lambda val: val['id']):
                valueset.append(tuple(value['names']))

            valueset_id = valueset_dict.setdefault(tuple(valueset),
              len(valueset_dict))
            property['valueset_id'] = valueset_id

    strings = collect_strings(properties)

    c_path = join(tools_folder, 'unicode.c')
    h_path = join(tools_folder, 'unicode.h')

    with open(c_path, 'w', newline='\n', encoding='ascii') as c_file:
        c_file.write('''\
/* For Unicode version {} */

#include "_regex_unicode.h"

#define RE_BLANK_MASK ((1 << RE_PROP_ZL) | (1 << RE_PROP_ZP))
#define RE_GRAPH_MASK ((1 << RE_PROP_CC) | (1 << RE_PROP_CS) | (1 << RE_PROP_CN))
#define RE_WORD_MASK (RE_PROP_M_MASK | (1 << RE_PROP_ND) | (1 << RE_PROP_PC))

typedef struct {{
    RE_UINT8 scripts[RE_MAX_SCX];
}} RE_ScriptExt;

typedef struct {{
    RE_UINT32 delta;
    RE_UINT16 others[RE_MAX_CASES - 1];
}} RE_AllCases;

typedef struct {{
    RE_UINT16 data[RE_MAX_FOLDED];
}} RE_FullCaseFolding;

/* Strings. */
char* re_strings[] = {{
'''.format(UNICODE_VERSION))

        lines = []

        for string in strings:
            lines.append('    "{}",\n'.format(string))

        strings_dict = {string: i for i, string in enumerate(strings)}

        unicode_data['string_count'] = len(strings_dict)

        c_file.writelines(lines)
        c_file.write('''\
};

/* Properties. */
RE_Property re_properties[] = {
''')

        for prop_id, property in enumerate(sorted(prop_list, key=lambda prop:
          prop['id'])):
            for name in property['names']:
                c_file.write('    {{{:4}, {:2}, {:2}}}, /* {} */\n'.format(strings_dict[munge(name)],
                  prop_id, property['valueset_id'], munge(name)))

        c_file.write('''\
};

/* Property values. */
RE_PropertyValue re_property_values[] = {
''')

        def make_key(names):
            if any(len(name) == 2 for name in names):
                return 0

            return 1

        gc_valset_id = properties[munge('General_Category')]['valueset_id']
        count = 0

        for valset, valset_id in sorted(valueset_dict.items(), key=lambda pair:
          pair[1]):
            if valset_id == gc_valset_id:
                valset = sorted(valset, key=make_key)

            for val_id, names in enumerate(valset):
                for name in names:
                    c_file.write('''    {{{:4}, {:2}, {:3}}}, /* {} */\n'''.format(strings_dict[munge(name)],
                      valset_id, val_id, munge(name)))

                count += len(names)

        unicode_data['valueset_table_count'] = count

        c_file.write('};\n')

        c_file.write('''\n/* Codepoints which expand on full case-folding. */\n''')

        unicode_data['expanded_count'] = len(expanded)
        generate_table('re_expand_on_folding', expanded, c_file, max_columns=8, public=True)

        for property in prop_list:
            print('    {}'.format(property['names'][0]), flush=True)

            if property['names'][0] == 'Script_Extensions':
                generate_script_extensions_lookup(properties, property, c_file)
            else:
                generate_lookup(property, c_file)

        print('    All cases', flush=True)
        generate_all_cases(unicode_data, c_file)

        print('    Simple case folding', flush=True)
        generate_simple_case_folding(unicode_data, c_file)

        print('    Full case folding', flush=True)
        generate_full_case_folding(unicode_data, c_file)

        c_file.write('''
/* Property function table. */
RE_GetPropertyFunc re_get_property[] = {
''')

        lines = []

        for property in prop_list:
            prop_name = property['names'][0].lower()

            if prop_name == 'script_extensions':
                lines.append('    0,\n')
            else:
                lines.append('    re_get_{},\n'.format(prop_name))

        lines[-1] = lines[-1].rstrip(',\n') + '\n'

        c_file.writelines(lines)

        c_file.write('};\n')

    with open(h_path, 'w', newline='\n', encoding='ascii') as h_file:
        property = unicode_data['properties'][munge('Script_Extensions')]
        max_scx = max(len(key) for key in property['values'])

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

#define RE_MAX_CASES 4
#define RE_MAX_FOLDED 3
#define RE_MAX_SCX {}

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

typedef RE_UINT32 (*RE_GetPropertyFunc)(RE_UINT32 codepoint);
'''.format(max_scx))

        gc_id = properties[munge('General_Category')]['id']
        cased_id = properties[munge('Cased')]['id']
        upper_id = properties[munge('Uppercase')]['id']
        lower_id = properties[munge('Lowercase')]['id']
        scx_id = properties[munge('Script_Extensions')]['id']

        h_file.write('''
#define RE_PROP_GC 0x{:X}
#define RE_PROP_CASED 0x{:X}
#define RE_PROP_UPPERCASE 0x{:X}
#define RE_PROP_LOWERCASE 0x{:X}
#define RE_PROP_SCX 0x{:X}

'''.format(gc_id, cased_id, upper_id, lower_id, scx_id))

        gc_values = properties[munge('General_Category')]['values']
        group_names = set('C L M N P S Z Assigned Cased_Letter'.split())

        names = set(gc_values) & set(munge(name) for name in group_names)

        for name in sorted(names, key=lambda name: gc_values[name]['id']):
            h_file.write('#define RE_PROP_{} {}\n'.format(name,
              gc_values[name]['id']))

        h_file.write('\n')

        val_list = []
        masks = {}

        for name in gc_values:
            if len(name) != 2 or not name.isalpha():
                continue

            if not gc_values[name].get('codepoints'):
                continue

            val_id = gc_values[name]['id']
            val_list.append((val_id, name))
            masks.setdefault(name[0], 0)
            masks[name[0]] |= 1 << val_id

        for val_id, name in sorted(val_list):
            h_file.write('#define RE_PROP_{} {}\n'.format(name, val_id))

        h_file.write('\n')

        for name, mask in sorted(masks.items()):
            h_file.write('#define RE_PROP_{}_MASK 0x{:08X}\n'.format(name,
              mask))

        h_file.write('\n')

        common = '''
            Alnum Alpha Any Ascii Blank Cntrl Digit Graph Lower Print Space
            Upper Word Xdigit Posix_Alnum Posix_Digit Posix_Punct Posix_Xdigit
        '''

        for name in common.split():
            property = properties.get(munge(name))

            if property is not None:
                h_file.write('#define RE_PROP_{} 0x{:06X}\n'.format(name.upper(),
                  (property['id'] << 16) | 1))
            else:
                for prop_name in ['GC', 'Script', 'Block']:
                    property = properties[munge(prop_name)]
                    value = property['values'].get(munge(name))

                    if value is not None:
                        h_file.write('#define RE_PROP_{} 0x{:06X}\n'.format(name.upper(),
                          (property['id'] << 16) | value['id']))
                        break

        h_file.write('\n')

        val_list = unique(properties[munge('Word_Break')]['values'].values(),
          key=id)
        values = [(value['id'], value['names'][0]) for value in val_list]

        for val_id, name in sorted(values):
            h_file.write('#define RE_WBREAK_{} {}\n'.format(munge(name),
              val_id))

        h_file.write('\n')

        val_list = unique(properties[munge('Grapheme_Cluster_Break')]['values'].values(),
          key=id)
        values = [(value['id'], value['names'][0]) for value in val_list]

        for val_id, name in sorted(values):
            h_file.write('#define RE_GBREAK_{} {}\n'.format(munge(name),
              val_id))

        h_file.write('\n')

        val_list = unique(properties[munge('Line_Break')]['values'].values(),
          key=id)
        values = [(value['id'], value['names'][0]) for value in val_list]

        for val_id, name in sorted(values):
            h_file.write('#define RE_LBREAK_{} {}\n'.format(munge(name),
              val_id))

        h_file.write('\n')

        h_file.write('extern char* re_strings[{}];\n'.format(unicode_data['string_count']))
        h_file.write('extern RE_Property re_properties[{}];\n'.format(unicode_data['property_tablee_count']))
        h_file.write('extern RE_PropertyValue re_property_values[{}];\n'.format(unicode_data['valueset_table_count']))
        h_file.write('extern RE_UINT16 re_expand_on_folding[{}];\n'.format(unicode_data['expanded_count']))
        h_file.write('extern RE_GetPropertyFunc re_get_property[{}];\n'.format(unicode_data['property_count']))

        h_file.write('\n')

        for property in prop_list:
            prop_name = property['names'][0]

            if prop_name == 'Script_Extensions':
                h_file.write('int re_get_{}(RE_UINT32 codepoint, RE_UINT8* scripts);\n'.format(prop_name.lower()))
            else:
                h_file.write('RE_UINT32 re_get_{}(RE_UINT32 codepoint);\n'.format(prop_name.lower()))

        h_file.write('int re_get_all_cases(RE_UINT32 codepoint, RE_UINT32* cases);\n')
        h_file.write('RE_UINT32 re_get_simple_case_folding(RE_UINT32 codepoint);\n')
        h_file.write('int re_get_full_case_folding(RE_UINT32 codepoint, RE_UINT32* folded);\n')

# Whether to update the Unicode data files from the Unicode website.
UNICODE_VERSION = '14.0.0'

# The URL from which the Unicode data files can be obtained.
unicode_data_base = 'http://www.unicode.org/Public/UNIDATA/'

NUM_CODEPOINTS = 0x110000

# The Unicode data files. The file names are relative to the website URL.
unicode_data_files = '''
[aliases]
PropertyAliases.txt
PropertyValueAliases.txt
[binary]
PropList.txt
extracted/DerivedBinaryProperties.txt
DerivedCoreProperties.txt
[emoji]
emoji/emoji-data.txt
[normalisation]
DerivedNormalizationProps.txt
[multivalue]
auxiliary/GraphemeBreakProperty.txt
auxiliary/SentenceBreakProperty.txt
auxiliary/WordBreakProperty.txt
Blocks.txt
extracted/DerivedBidiClass.txt
extracted/DerivedCombiningClass.txt
extracted/DerivedDecompositionType.txt
extracted/DerivedEastAsianWidth.txt
extracted/DerivedGeneralCategory.txt
extracted/DerivedJoiningGroup.txt
extracted/DerivedJoiningType.txt
extracted/DerivedLineBreak.txt
extracted/DerivedNumericType.txt
HangulSyllableType.txt
IndicPositionalCategory.txt
IndicSyllabicCategory.txt
Scripts.txt
[numeric_values]
extracted/DerivedNumericValues.txt
[case_folding]
CaseFolding.txt
[script_extensions]
ScriptExtensions.txt
'''

data_files = {}
section = ''

for line in unicode_data_files.splitlines():
    if line[ : 1] + line[-1 : ] == '[]':
        section = line[1 : -1]
    elif line:
        data_files.setdefault(section, []).append(line)

# The generated C files will be written into this folder.
tools_folder = dirname(__file__)

# The local folder in which the Unicode data files are stored.
data_folder = join(tools_folder, 'unicode_data')

with suppress(FileExistsError):
    mkdir(data_folder)

download_unicode_files(unicode_data_base, data_files, data_folder)

unicode_data = parse_unicode_files(data_files, data_folder)
make_additional_properties(unicode_data)
write_summary(unicode_data, tools_folder)

binary_dict = make_binary_dict()

generate_code(unicode_data, tools_folder)

print('\nFinished!')
