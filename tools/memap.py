#!/usr/bin/env python

"""
Copyright (c) 2016-2019 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
from __future__ import print_function, division, absolute_import

from abc import abstractmethod, ABCMeta
from sys import stdout, exit, argv, path
from os import sep
from os.path import (basename, dirname, join, relpath, abspath, commonprefix,
                     splitext)
import re
import csv
import json
from argparse import ArgumentParser
from copy import deepcopy
from collections import defaultdict
from prettytable import PrettyTable, HEADER
from jinja2 import FileSystemLoader, StrictUndefined
from jinja2.environment import Environment
from future.utils import with_metaclass


# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
path.insert(0, ROOT)

from tools.utils import (
    argparse_filestring_type,
    argparse_lowercase_hyphen_type,
    argparse_uppercase_type
)  # noqa: E402


class _Parser(with_metaclass(ABCMeta, object)):
    """Internal interface for parsing"""
    SECTIONS = ('.text', '.data', '.bss', '.heap', '.stack')
    MISC_FLASH_SECTIONS = ('.interrupts', '.flash_config')
    OTHER_SECTIONS = ('.interrupts_ram', '.init', '.ARM.extab',
                      '.ARM.exidx', '.ARM.attributes', '.eh_frame',
                      '.init_array', '.fini_array', '.jcr', '.stab',
                      '.stabstr', '.ARM.exidx', '.ARM')

    def __init__(self):
        self.modules = dict()

    def module_add(self, object_name, size, section):
        """ Adds a module or section to the list

        Positional arguments:
        object_name - name of the entry to add
        size - the size of the module being added
        section - the section the module contributes to
        """
        if not object_name or not size or not section:
            return

        if object_name in self.modules:
            self.modules[object_name].setdefault(section, 0)
            self.modules[object_name][section] += size
            return

        obj_split = sep + basename(object_name)
        for module_path, contents in self.modules.items():
            if module_path.endswith(obj_split) or module_path == object_name:
                contents.setdefault(section, 0)
                contents[section] += size
                return

        new_module = defaultdict(int)
        new_module[section] = size
        self.modules[object_name] = new_module

    def module_replace(self, old_object, new_object):
        """ Replaces an object name with a new one
        """
        if old_object in self.modules:
            self.modules[new_object] = self.modules[old_object]
            del self.modules[old_object]

    @abstractmethod
    def parse_mapfile(self, mapfile):
        """Parse a given file object pointing to a map file

        Positional arguments:
        mapfile - an open file object that reads a map file

        return value - a dict mapping from object names to section dicts,
                       where a section dict maps from sections to sizes
        """
        raise NotImplemented


class _GccParser(_Parser):
    RE_OBJECT_FILE = re.compile(r'^(.+\/.+\.o(bj)?)$')
    RE_LIBRARY_OBJECT = re.compile(
        r'^.+' + r''.format(sep) + r'lib((.+\.a)\((.+\.o(bj)?)\))$'
    )
    RE_STD_SECTION = re.compile(r'^\s+.*0x(\w{8,16})\s+0x(\w+)\s(.+)$')
    RE_FILL_SECTION = re.compile(r'^\s*\*fill\*\s+0x(\w{8,16})\s+0x(\w+).*$')
    RE_TRANS_FILE = re.compile(r'^(.+\/|.+\.ltrans.o(bj)?)$')
    OBJECT_EXTENSIONS = (".o", ".obj")

    ALL_SECTIONS = (
        _Parser.SECTIONS
        + _Parser.OTHER_SECTIONS
        + _Parser.MISC_FLASH_SECTIONS
        + ('unknown', 'OUTPUT')
    )

    def check_new_section(self, line):
        """ Check whether a new section in a map file has been detected

        Positional arguments:
        line - the line to check for a new section

        return value - A section name, if a new section was found, None
                       otherwise
        """
        line_s = line.strip()
        for i in self.ALL_SECTIONS:
            if line_s.startswith(i):
                return i
        if line.startswith('.'):
            return 'unknown'
        else:
            return None

    def parse_object_name(self, line):
        """ Parse a path to object file

        Positional arguments:
        line - the path to parse the object and module name from

        return value - an object file name
        """
        if re.match(self.RE_TRANS_FILE, line):
            return '[misc]'

        test_re_mbed_os_name = re.match(self.RE_OBJECT_FILE, line)

        if test_re_mbed_os_name:
            object_name = test_re_mbed_os_name.group(1)

            # corner case: certain objects are provided by the GCC toolchain
            if 'arm-none-eabi' in line:
                return join('[lib]', 'misc', basename(object_name))
            return object_name

        else:
            test_re_obj_name = re.match(self.RE_LIBRARY_OBJECT, line)

            if test_re_obj_name:
                return join('[lib]', test_re_obj_name.group(2),
                            test_re_obj_name.group(3))
            else:
                if (
                    not line.startswith("LONG") and
                    not line.startswith("linker stubs")
                ):
                    print("Unknown object name found in GCC map file: %s"
                          % line)
                return '[misc]'

    def parse_section(self, line):
        """ Parse data from a section of gcc map file

        examples:
                        0x00004308       0x7c ./BUILD/K64F/GCC_ARM/spi_api.o
         .text          0x00000608      0x198 ./BUILD/K64F/HAL_CM4.o

        Positional arguments:
        line - the line to parse a section from
        """
        is_fill = re.match(self.RE_FILL_SECTION, line)
        if is_fill:
            o_name = '[fill]'
            o_size = int(is_fill.group(2), 16)
            return [o_name, o_size]

        is_section = re.match(self.RE_STD_SECTION, line)
        if is_section:
            o_size = int(is_section.group(2), 16)
            if o_size:
                o_name = self.parse_object_name(is_section.group(3))
                return [o_name, o_size]

        return ["", 0]

    def parse_mapfile(self, file_desc):
        """ Main logic to decode gcc map files

        Positional arguments:
        file_desc - a stream object to parse as a gcc map file
        """
        current_section = 'unknown'

        with file_desc as infile:
            for line in infile:
                if line.startswith('Linker script and memory map'):
                    current_section = "unknown"
                    break

            for line in infile:
                next_section = self.check_new_section(line)

                if next_section == "OUTPUT":
                    break
                elif next_section:
                    current_section = next_section

                object_name, object_size = self.parse_section(line)
                self.module_add(object_name, object_size, current_section)

        common_prefix = dirname(commonprefix([
            o for o in self.modules.keys()
            if (
                    o.endswith(self.OBJECT_EXTENSIONS)
                    and not o.startswith("[lib]")
            )]))
        new_modules = {}
        for name, stats in self.modules.items():
            if name.startswith("[lib]"):
                new_modules[name] = stats
            elif name.endswith(self.OBJECT_EXTENSIONS):
                new_modules[relpath(name, common_prefix)] = stats
            else:
                new_modules[name] = stats
        return new_modules


class _ArmccParser(_Parser):
    RE = re.compile(
        r'^\s+0x(\w{8})\s+0x(\w{8})\s+(\w+)\s+(\w+)\s+(\d+)\s+[*]?.+\s+(.+)$')
    RE_OBJECT = re.compile(r'(.+\.(l|ar))\((.+\.o(bj)?)\)')
    OBJECT_EXTENSIONS = (".o", ".obj")

    def parse_object_name(self, line):
        """ Parse object file

        Positional arguments:
        line - the line containing the object or library
        """
        if line.endswith(self.OBJECT_EXTENSIONS):
            return line

        else:
            is_obj = re.match(self.RE_OBJECT, line)
            if is_obj:
                return join(
                    '[lib]', basename(is_obj.group(1)), is_obj.group(3)
                )
            else:
                print(
                    "Malformed input found when parsing ARMCC map: %s" % line
                )
                return '[misc]'

    def parse_section(self, line):
        """ Parse data from an armcc map file

        Examples of armcc map file:
            Base_Addr    Size         Type   Attr      Idx    E Section Name        Object
            0x00000000   0x00000400   Data   RO        11222    self.RESET               startup_MK64F12.o
            0x00000410   0x00000008   Code   RO        49364  * !!!main             c_w.l(__main.o)

        Positional arguments:
        line - the line to parse the section data from
        """  # noqa: E501
        test_re = re.match(self.RE, line)

        if (
            test_re
            and "ARM_LIB_HEAP" not in line
            ):
            size = int(test_re.group(2), 16)

            if test_re.group(4) == 'RO':
                section = '.text'
            else:
                if test_re.group(3) == 'Data':
                    section = '.data'
                elif test_re.group(3) == 'Zero':
                    section = '.bss'
                elif test_re.group(3) == 'Code':
                    section = '.text'
                else:
                    print(
                        "Malformed input found when parsing armcc map: %s, %r"
                        % (line, test_re.groups())
                    )

                    return ["", 0, ""]

            # check name of object or library
            object_name = self.parse_object_name(
                test_re.group(6))

            return [object_name, size, section]

        else:
            return ["", 0, ""]

    def parse_mapfile(self, file_desc):
        """ Main logic to decode armc5 map files

        Positional arguments:
        file_desc - a file like object to parse as an armc5 map file
        """
        with file_desc as infile:
            # Search area to parse
            for line in infile:
                if line.startswith('    Base Addr    Size'):
                    break

            # Start decoding the map file
            for line in infile:
                self.module_add(*self.parse_section(line))

        common_prefix = dirname(commonprefix([
            o for o in self.modules.keys()
            if (
                o.endswith(self.OBJECT_EXTENSIONS)
                and o != "anon$$obj.o"
                and o != "anon$$obj.obj"
                and not o.startswith("[lib]")
            )]))
        new_modules = {}
        for name, stats in self.modules.items():
            if (
                name == "anon$$obj.o"
                or name == "anon$$obj.obj"
                or name.startswith("[lib]")
            ):
                new_modules[name] = stats
            elif name.endswith(self.OBJECT_EXTENSIONS):
                new_modules[relpath(name, common_prefix)] = stats
            else:
                new_modules[name] = stats
        return new_modules


class _IarParser(_Parser):
    RE = re.compile(
        r'^\s+(.+)\s+(zero|const|ro code|inited|uninit)\s'
        r'+0x([\'\w]+)\s+0x(\w+)\s+(.+)\s.+$')

    RE_CMDLINE_FILE = re.compile(r'^#\s+(.+\.o(bj)?)')
    RE_LIBRARY = re.compile(r'^(.+\.a)\:.+$')
    RE_OBJECT_LIBRARY = re.compile(r'^\s+(.+\.o(bj)?)\s.*')
    OBJECT_EXTENSIONS = (".o", ".obj")

    def __init__(self):
        _Parser.__init__(self)
        # Modules passed to the linker on the command line
        # this is a dict because modules are looked up by their basename
        self.cmd_modules = {}

    def parse_object_name(self, object_name):
        """ Parse object file

        Positional arguments:
        line - the line containing the object or library
        """
        if object_name.endswith(self.OBJECT_EXTENSIONS):
            try:
                return self.cmd_modules[object_name]
            except KeyError:
                return object_name
        else:
            return '[misc]'

    def parse_section(self, line):
        """ Parse data from an IAR map file

        Examples of IAR map file:
         Section             Kind        Address     Size  Object
         .intvec             ro code  0x00000000    0x198  startup_MK64F12.o [15]
         .rodata             const    0x00000198      0x0  zero_init3.o [133]
         .iar.init_table     const    0x00008384     0x2c  - Linker created -
         Initializer bytes   const    0x00000198     0xb2  <for P3 s0>
         .data               inited   0x20000000     0xd4  driverAtmelRFInterface.o [70]
         .bss                zero     0x20000598    0x318  RTX_Conf_CM.o [4]
         .iar.dynexit        uninit   0x20001448    0x204  <Block tail>
           HEAP              uninit   0x20001650  0x10000  <Block tail>

        Positional_arguments:
        line - the line to parse section data from
        """  # noqa: E501
        test_re = re.match(self.RE, line)
        if test_re:
            if (
                test_re.group(2) == 'const' or
                test_re.group(2) == 'ro code'
            ):
                section = '.text'
            elif (test_re.group(2) == 'zero' or
                  test_re.group(2) == 'uninit'):
                if test_re.group(1)[0:4] == 'HEAP':
                    section = '.heap'
                elif test_re.group(1)[0:6] == 'CSTACK':
                    section = '.stack'
                else:
                    section = '.bss'  # default section

            elif test_re.group(2) == 'inited':
                section = '.data'
            else:
                print("Malformed input found when parsing IAR map: %s" % line)
                return ["", 0, ""]

            # lookup object in dictionary and return module name
            object_name = self.parse_object_name(test_re.group(5))

            size = int(test_re.group(4), 16)
            return [object_name, size, section]

        else:
            return ["", 0, ""]

    def check_new_library(self, line):
        """
        Searches for libraries and returns name. Example:
        m7M_tls.a: [43]

        """
        test_address_line = re.match(self.RE_LIBRARY, line)
        if test_address_line:
            return test_address_line.group(1)
        else:
            return ""

    def check_new_object_lib(self, line):
        """
        Searches for objects within a library section and returns name.
        Example:
        rt7M_tl.a: [44]
            ABImemclr4.o                 6
            ABImemcpy_unaligned.o      118
            ABImemset48.o               50
            I64DivMod.o                238
            I64DivZer.o                  2

        """
        test_address_line = re.match(self.RE_OBJECT_LIBRARY, line)
        if test_address_line:
            return test_address_line.group(1)
        else:
            return ""

    def parse_command_line(self, lines):
        """Parse the files passed on the command line to the iar linker

        Positional arguments:
        lines -- an iterator over the lines within a file
        """
        for line in lines:
            if line.startswith("*"):
                break
            for arg in line.split(" "):
                arg = arg.rstrip(" \n")
                if (
                    not arg.startswith("-")
                    and arg.endswith(self.OBJECT_EXTENSIONS)
                ):
                    self.cmd_modules[basename(arg)] = arg

        common_prefix = dirname(commonprefix(list(self.cmd_modules.values())))
        self.cmd_modules = {s: relpath(f, common_prefix)
                            for s, f in self.cmd_modules.items()}

    def parse_mapfile(self, file_desc):
        """ Main logic to decode IAR map files

        Positional arguments:
        file_desc - a file like object to parse as an IAR map file
        """
        with file_desc as infile:
            self.parse_command_line(infile)

            for line in infile:
                if line.startswith('  Section  '):
                    break

            for line in infile:
                self.module_add(*self.parse_section(line))

                if line.startswith('*** MODULE SUMMARY'):  # finish section
                    break

            current_library = ""
            for line in infile:
                library = self.check_new_library(line)

                if library:
                    current_library = library

                object_name = self.check_new_object_lib(line)

                if object_name and current_library:
                    temp = join('[lib]', current_library, object_name)
                    self.module_replace(object_name, temp)
        return self.modules


class MemapParser(object):
    """An object that represents parsed results, parses the memory map files,
    and writes out different file types of memory results
    """

    print_sections = ('.text', '.data', '.bss')
    delta_sections = ('.text-delta', '.data-delta', '.bss-delta')

    # sections to print info (generic for all toolchains)
    sections = _Parser.SECTIONS
    misc_flash_sections = _Parser.MISC_FLASH_SECTIONS
    other_sections = _Parser.OTHER_SECTIONS

    def __init__(self):
        # list of all modules and their sections
        # full list - doesn't change with depth
        self.modules = dict()
        self.old_modules = None
        # short version with specific depth
        self.short_modules = dict()

        # Memory report (sections + summary)
        self.mem_report = []

        # Memory summary
        self.mem_summary = dict()

        # Totals of ".text", ".data" and ".bss"
        self.subtotal = dict()

        # Flash no associated with a module
        self.misc_flash_mem = 0

        # Name of the toolchain, for better headings
        self.tc_name = None

    def reduce_depth(self, depth):
        """
        populates the short_modules attribute with a truncated module list

        (1) depth = 1:
        main.o
        mbed-os

        (2) depth = 2:
        main.o
        mbed-os/test.o
        mbed-os/drivers

        """
        if depth == 0 or depth is None:
            self.short_modules = deepcopy(self.modules)
        else:
            self.short_modules = dict()
            for module_name, v in self.modules.items():
                split_name = module_name.split(sep)
                if split_name[0] == '':
                    split_name = split_name[1:]
                new_name = join(*split_name[:depth])
                self.short_modules.setdefault(new_name, defaultdict(int))
                for section_idx, value in v.items():
                    self.short_modules[new_name][section_idx] += value
                    delta_name = section_idx + '-delta'
                    self.short_modules[new_name][delta_name] += value
            if self.old_modules:
                for module_name, v in self.old_modules.items():
                    split_name = module_name.split(sep)
                    if split_name[0] == '':
                        split_name = split_name[1:]
                    new_name = join(*split_name[:depth])
                    self.short_modules.setdefault(new_name, defaultdict(int))
                    for section_idx, value in v.items():
                        delta_name = section_idx + '-delta'
                        self.short_modules[new_name][delta_name] -= value

    export_formats = ["json", "csv-ci", "html", "table"]

    def generate_output(self, export_format, depth, file_output=None):
        """ Generates summary of memory map data

        Positional arguments:
        export_format - the format to dump

        Keyword arguments:
        file_desc - descriptor (either stdout or file)
        depth - directory depth on report

        Returns: generated string for the 'table' format, otherwise None
        """
        if depth is None or depth > 0:
            self.reduce_depth(depth)
        self.compute_report()
        try:
            if file_output:
                file_desc = open(file_output, 'w')
            else:
                file_desc = stdout
        except IOError as error:
            print("I/O error({0}): {1}".format(error.errno, error.strerror))
            return False

        to_call = {'json': self.generate_json,
                   'html': self.generate_html,
                   'csv-ci': self.generate_csv,
                   'table': self.generate_table}[export_format]
        output = to_call(file_desc)

        if file_desc is not stdout:
            file_desc.close()

        return output

    @staticmethod
    def _move_up_tree(tree, next_module):
        tree.setdefault("children", [])
        for child in tree["children"]:
            if child["name"] == next_module:
                return child
        else:
            new_module = {"name": next_module, "value": 0, "delta": 0}
            tree["children"].append(new_module)
            return new_module

    def generate_html(self, file_desc):
        """Generate a json file from a memory map for D3

        Positional arguments:
        file_desc - the file to write out the final report to
        """
        tree_text = {"name": ".text", "value": 0, "delta": 0}
        tree_bss = {"name": ".bss", "value": 0, "delta": 0}
        tree_data = {"name": ".data", "value": 0, "delta": 0}
        for name, dct in self.modules.items():
            cur_text = tree_text
            cur_bss = tree_bss
            cur_data = tree_data
            modules = name.split(sep)
            while True:
                try:
                    cur_text["value"] += dct['.text']
                    cur_text["delta"] += dct['.text']
                except KeyError:
                    pass
                try:
                    cur_bss["value"] += dct['.bss']
                    cur_bss["delta"] += dct['.bss']
                except KeyError:
                    pass
                try:
                    cur_data["value"] += dct['.data']
                    cur_data["delta"] += dct['.data']
                except KeyError:
                    pass
                if not modules:
                    break
                next_module = modules.pop(0)
                cur_text = self._move_up_tree(cur_text, next_module)
                cur_data = self._move_up_tree(cur_data, next_module)
                cur_bss = self._move_up_tree(cur_bss, next_module)
        if self.old_modules:
            for name, dct in self.old_modules.items():
                cur_text = tree_text
                cur_bss = tree_bss
                cur_data = tree_data
                modules = name.split(sep)
                while True:
                    try:
                        cur_text["delta"] -= dct['.text']
                    except KeyError:
                        pass
                    try:
                        cur_bss["delta"] -= dct['.bss']
                    except KeyError:
                        pass
                    try:
                        cur_data["delta"] -= dct['.data']
                    except KeyError:
                        pass
                    if not modules:
                        break
                    next_module = modules.pop(0)
                    if not any(
                        cld['name'] == next_module
                        for cld in cur_text['children']
                    ):
                        break
                    cur_text = self._move_up_tree(cur_text, next_module)
                    cur_data = self._move_up_tree(cur_data, next_module)
                    cur_bss = self._move_up_tree(cur_bss, next_module)

        tree_rom = {
            "name": "ROM",
            "value": tree_text["value"] + tree_data["value"],
            "delta": tree_text["delta"] + tree_data["delta"],
            "children": [tree_text, tree_data]
        }
        tree_ram = {
            "name": "RAM",
            "value": tree_bss["value"] + tree_data["value"],
            "delta": tree_bss["delta"] + tree_data["delta"],
            "children": [tree_bss, tree_data]
        }

        jinja_loader = FileSystemLoader(dirname(abspath(__file__)))
        jinja_environment = Environment(loader=jinja_loader,
                                        undefined=StrictUndefined)

        template = jinja_environment.get_template("memap_flamegraph.html")
        name, _ = splitext(basename(file_desc.name))
        if name.endswith("_map"):
            name = name[:-4]
        if self.tc_name:
            name = "%s %s" % (name, self.tc_name)
        data = {
            "name": name,
            "rom": json.dumps(tree_rom),
            "ram": json.dumps(tree_ram),
        }
        file_desc.write(template.render(data))
        return None

    def generate_json(self, file_desc):
        """Generate a json file from a memory map

        Positional arguments:
        file_desc - the file to write out the final report to
        """
        file_desc.write(json.dumps(self.mem_report, indent=4))
        file_desc.write('\n')
        return None

    RAM_FORMAT_STR = (
        "Total Static RAM memory (data + bss): {}({:+}) bytes\n"
    )

    ROM_FORMAT_STR = (
        "Total Flash memory (text + data): {}({:+}) bytes\n"
    )

    def generate_csv(self, file_desc):
        """Generate a CSV file from a memoy map

        Positional arguments:
        file_desc - the file to write out the final report to
        """
        writer = csv.writer(file_desc, delimiter=',',
                            quoting=csv.QUOTE_MINIMAL)

        module_section = []
        sizes = []
        for i in sorted(self.short_modules):
            for k in self.print_sections + self.delta_sections:
                module_section.append((i + k))
                sizes += [self.short_modules[i][k]]

        module_section.append('static_ram')
        sizes.append(self.mem_summary['static_ram'])

        module_section.append('total_flash')
        sizes.append(self.mem_summary['total_flash'])

        writer.writerow(module_section)
        writer.writerow(sizes)
        return None

    def generate_table(self, file_desc):
        """Generate a table from a memoy map

        Returns: string of the generated table
        """
        # Create table
        columns = ['Module']
        columns.extend(self.print_sections)

        table = PrettyTable(columns, junction_char="|", hrules=HEADER)
        table.align["Module"] = "l"
        for col in self.print_sections:
            table.align[col] = 'r'

        for i in list(self.print_sections):
            table.align[i] = 'r'

        for i in sorted(self.short_modules):
            row = [i]

            for k in self.print_sections:
                row.append("{}({:+})".format(
                    self.short_modules[i][k],
                    self.short_modules[i][k + "-delta"]
                ))

            table.add_row(row)

        subtotal_row = ['Subtotals']
        for k in self.print_sections:
            subtotal_row.append("{}({:+})".format(
                self.subtotal[k], self.subtotal[k + '-delta']))

        table.add_row(subtotal_row)

        output = table.get_string()
        output += '\n'

        output += self.RAM_FORMAT_STR.format(
            self.mem_summary['static_ram'],
            self.mem_summary['static_ram_delta']
        )
        output += self.ROM_FORMAT_STR.format(
            self.mem_summary['total_flash'],
            self.mem_summary['total_flash_delta']
        )

        return output

    toolchains = ["ARM", "ARM_STD", "ARM_MICRO", "GCC_ARM", "IAR"]

    def compute_report(self):
        """ Generates summary of memory usage for main areas
        """
        self.subtotal = defaultdict(int)

        for mod in self.modules.values():
            for k in self.sections:
                self.subtotal[k] += mod[k]
                self.subtotal[k + '-delta'] += mod[k]
        if self.old_modules:
            for mod in self.old_modules.values():
                for k in self.sections:
                    self.subtotal[k + '-delta'] -= mod[k]

        self.mem_summary = {
            'static_ram': self.subtotal['.data'] + self.subtotal['.bss'],
            'static_ram_delta':
            self.subtotal['.data-delta'] + self.subtotal['.bss-delta'],
            'total_flash': (self.subtotal['.text'] + self.subtotal['.data']),
            'total_flash_delta':
            self.subtotal['.text-delta'] + self.subtotal['.data-delta'],
        }

        self.mem_report = []
        if self.short_modules:
            for name, sizes in sorted(self.short_modules.items()):
                self.mem_report.append({
                    "module": name,
                    "size": {
                        k: sizes.get(k, 0) for k in (self.print_sections +
                                                     self.delta_sections)
                    }
                })

        self.mem_report.append({
            'summary': self.mem_summary
        })

    def parse(self, mapfile, toolchain):
        """ Parse and decode map file depending on the toolchain

        Positional arguments:
        mapfile - the file name of the memory map file
        toolchain - the toolchain used to create the file
        """
        self.tc_name = toolchain.title()
        if toolchain in ("ARM", "ARM_STD", "ARM_MICRO", "ARMC6"):
            parser = _ArmccParser
        elif toolchain == "GCC_ARM":
            parser = _GccParser
        elif toolchain == "IAR":
            parser = _IarParser
        else:
            return False
        try:
            with open(mapfile, 'r') as file_input:
                self.modules = parser().parse_mapfile(file_input)
            try:
                with open("%s.old" % mapfile, 'r') as old_input:
                    self.old_modules = parser().parse_mapfile(old_input)
            except IOError:
                self.old_modules = None
            return True

        except IOError as error:
            print("I/O error({0}): {1}".format(error.errno, error.strerror))
            return False


def main():
    """Entry Point"""
    version = '0.4.0'

    # Parser handling
    parser = ArgumentParser(
        description="Memory Map File Analyser for ARM mbed\nversion %s" %
        version)

    parser.add_argument(
        'file', type=argparse_filestring_type, help='memory map file')

    parser.add_argument(
        '-t', '--toolchain', dest='toolchain',
        help='select a toolchain used to build the memory map file (%s)' %
        ", ".join(MemapParser.toolchains),
        required=True,
        type=argparse_uppercase_type(MemapParser.toolchains, "toolchain"))

    parser.add_argument(
        '-d', '--depth', dest='depth', type=int,
        help='specify directory depth level to display report', required=False)

    parser.add_argument(
        '-o', '--output', help='output file name', required=False)

    parser.add_argument(
        '-e', '--export', dest='export', required=False, default='table',
        type=argparse_lowercase_hyphen_type(MemapParser.export_formats,
                                            'export format'),
        help="export format (examples: %s: default)" %
        ", ".join(MemapParser.export_formats))

    parser.add_argument('-v', '--version', action='version', version=version)

    # Parse/run command
    if len(argv) <= 1:
        parser.print_help()
        exit(1)

    args = parser.parse_args()

    # Create memap object
    memap = MemapParser()

    # Parse and decode a map file
    if args.file and args.toolchain:
        if memap.parse(args.file, args.toolchain) is False:
            exit(0)

    if args.depth is None:
        depth = 2  # default depth level
    else:
        depth = args.depth

    returned_string = None
    # Write output in file
    if args.output is not None:
        returned_string = memap.generate_output(
            args.export,
            depth,
            args.output
        )
    else:  # Write output in screen
        returned_string = memap.generate_output(args.export, depth)

    if args.export == 'table' and returned_string:
        print(returned_string)

    exit(0)


if __name__ == "__main__":
    main()
