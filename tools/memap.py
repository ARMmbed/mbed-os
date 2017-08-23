#!/usr/bin/env python

"""Memory Map File Analyser for ARM mbed"""

import sys
import os
import re
import csv
import json
import argparse
from copy import deepcopy
from prettytable import PrettyTable

from utils import argparse_filestring_type, \
    argparse_lowercase_hyphen_type, argparse_uppercase_type

RE_ARMCC = re.compile(
    r'^\s+0x(\w{8})\s+0x(\w{8})\s+(\w+)\s+(\w+)\s+(\d+)\s+[*]?.+\s+(.+)$')
RE_IAR = re.compile(
    r'^\s+(.+)\s+(zero|const|ro code|inited|uninit)\s'
    r'+0x(\w{8})\s+0x(\w+)\s+(.+)\s.+$')

class MemapParser(object):
    """An object that represents parsed results, parses the memory map files,
    and writes out different file types of memory results
    """

    print_sections = ('.text', '.data', '.bss')

    misc_flash_sections = ('.interrupts', '.flash_config')

    other_sections = ('.interrupts_ram', '.init', '.ARM.extab',
                      '.ARM.exidx', '.ARM.attributes', '.eh_frame',
                      '.init_array', '.fini_array', '.jcr', '.stab',
                      '.stabstr', '.ARM.exidx', '.ARM')

    # sections to print info (generic for all toolchains)
    sections = ('.text', '.data', '.bss', '.heap', '.stack')

    def __init__(self):
        """ General initialization
        """

        # list of all modules and their sections
        self.modules = dict()       # full list - doesn't change with depth
        self.short_modules = dict() # short version with specific depth

        # sections must be defined in this order to take irrelevant out
        self.all_sections = self.sections + self.other_sections + \
                            self.misc_flash_sections + ('unknown', 'OUTPUT')

        # Memory report (sections + summary)
        self.mem_report = []

        # Just the memory summary section
        self.mem_summary = dict()

        self.subtotal = dict()

        self.misc_flash_mem = 0


    def remove_unused_modules(self):
        """ Removes modules/objects that were compiled but are not used
        """

        # Using keys to be able to remove entry
        for i in self.modules.keys():
            size = 0
            for k in self.print_sections:
                size += self.modules[i][k]
            if size == 0:
                del self.modules[i]

    def module_init(self, object_name):
        """ Initialize a module. Just adds the name of the module

        Positional arguments:
        object_name - name of the entry to add
        """

        if object_name not in self.modules:
            temp_dic = dict()
            for section_idx in self.all_sections:
                temp_dic[section_idx] = 0
            self.modules[object_name] = temp_dic

    def module_add(self, object_name, size, section):
        """ Adds a module / section to the list

        Positional arguments:
        object_name - name of the entry to add
        size - the size of the module being added
        section - the section the module contributes to
        """

        # Check if object is a sub-string of key
        for module_path in self.modules:

            # this is required to differenciate: main.o vs xxxmain.o
            module_split = os.path.basename(module_path)
            obj_split = os.path.basename(object_name)

            if module_split == obj_split:
                self.modules[module_path][section] += size
                return

        new_module = dict()
        for section_idx in self.all_sections:
            new_module[section_idx] = 0
        new_module[section] = size
        self.modules[object_name] = new_module

    def module_replace(self, old_object, new_object):
        """ Replaces an object name with a new one
        """

        # Check if object is a sub-string of key
        if old_object in self.modules:
            self.modules[new_object] = self.modules[old_object]
            del self.modules[old_object]

    def check_new_section_gcc(self, line):
        """ Check whether a new section in a map file has been detected (only
        applies to gcc)

        Positional arguments:
        line - the line to check for a new section
        """

        for i in self.all_sections:
            if line.startswith(i):
                # should name of the section (assuming it's a known one)
                return i

        if line.startswith('.'):
            return 'unknown'     # all others are classified are unknown
        else:
            return False         # everything else, means no change in section


    def parse_object_name_gcc(self, line):
        """ Parse a path to object file

        Positional arguments:
        txt - the path to parse the object and module name from
        """

        line = line.replace('\\', '/')
        RE_OBJECT_FILE = r'^.+\/(.+\.o)$'
        test_re_mbed_os_name = re.match(RE_OBJECT_FILE, line)

        if test_re_mbed_os_name:

            object_name = test_re_mbed_os_name.group(1)

            # corner case: certain objects are provided by the GCC toolchain
            if 'arm-none-eabi' in line:
                object_name = '[lib]/misc/' + object_name

            return object_name

        else:

            RE_LIBRARY_OBJECT_FILE = r'^.+\/(lib.+\.a)\((.+\.o)\)$'
            test_re_obj_name = re.match(RE_LIBRARY_OBJECT_FILE, line)

            if test_re_obj_name:
                object_name = test_re_obj_name.group(1) + '/' + \
                              test_re_obj_name.group(2)

                return '[lib]/' + object_name

            else:
                print "Malformed input found when parsing GCC map: %s" % line
                return '[misc]'

    def parse_section_gcc(self, line):
        """ Parse data from a section of gcc map file

        examples:
                        0x00004308       0x7c ./BUILD/K64F/GCC_ARM/mbed-os/hal/targets/hal/TARGET_Freescale/TARGET_KPSDK_MCUS/spi_api.o
         .text          0x00000608      0x198 ./BUILD/K64F/GCC_ARM/mbed-os/core/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_RTOS_M4_M7/TOOLCHAIN_GCC/HAL_CM4.o

        Positional arguments:
        line - the line to parse a section from
        """

        RE_STD_SECTION_GCC = re.compile(
            r'^\s+.*0x(\w{8,16})\s+0x(\w+)\s(.+)$')

        test_address_len_name = re.match(RE_STD_SECTION_GCC, line)

        if test_address_len_name:

            if int(test_address_len_name.group(2), 16) == 0: # size == 0
                return ["", 0] # no valid entry
            else:
                o_name = self.parse_object_name_gcc(\
                    test_address_len_name.group(3))
                o_size = int(test_address_len_name.group(2), 16)

                return [o_name, o_size]

        else: # special corner case for *fill* sections
            #  example
            # *fill*         0x0000abe4        0x4

            RE_FILL_SECTION_GCC = r'^\s+\*fill\*\s+0x(\w{8,16})\s+0x(\w+).*$'

            test_address_len = re.match(RE_FILL_SECTION_GCC, line)

            if test_address_len:
                if int(test_address_len.group(2), 16) == 0: # size == 0
                    return ["", 0] # no valid entry
                else:
                    o_name = '[fill]'
                    o_size = int(test_address_len.group(2), 16)
                    return [o_name, o_size]
            else:
                return ["", 0] # no valid entry


    def parse_map_file_gcc(self, file_desc):
        """ Main logic to decode gcc map files

        Positional arguments:
        file_desc - a stream object to parse as a gcc map file
        """

        current_section = 'unknown'

        with file_desc as infile:

            # Search area to parse
            for line in infile:
                if line.startswith('Linker script and memory map'):
                    current_section = "unknown"
                    break

            # Start decoding the map file
            for line in infile:

                change_section = self.check_new_section_gcc(line)

                if change_section == "OUTPUT": # finish parsing file: exit
                    break
                elif change_section != False:
                    current_section = change_section

                [object_name, object_size] = self.parse_section_gcc(line)

                if object_size == 0 or object_name == "":
                    pass
                else:
                    self.module_add(object_name, object_size,\
                                        current_section)

    def parse_object_name_armcc(self, line):
        """ Parse object file

        Positional arguments:
        line - the line containing the object or library
        """

        # simple object (not library)
        if line[-2] == '.' and line[-1] == 'o':
            return line

        else:

            RE_OBJECT_ARMCC = r'(.+\.l)\((.+\.o)\)'
            test_re_obj_name = re.match(RE_OBJECT_ARMCC, line)

            if test_re_obj_name:
                object_name = test_re_obj_name.group(1) + '/' + \
                              test_re_obj_name.group(2)

                return '[lib]/' + object_name
            else:
                print "Malformed input found when parsing ARMCC map: %s" % line
                return '[misc]'



    def parse_section_armcc(self, line):
        """ Parse data from an armcc map file

        Examples of armcc map file:
            Base_Addr    Size         Type   Attr      Idx    E Section Name        Object
            0x00000000   0x00000400   Data   RO        11222    RESET               startup_MK64F12.o
            0x00000410   0x00000008   Code   RO        49364  * !!!main             c_w.l(__main.o)

        Positional arguments:
        line - the line to parse the section data from
        """

        test_re_armcc = re.match(RE_ARMCC, line)

        if test_re_armcc:

            size = int(test_re_armcc.group(2), 16)

            if test_re_armcc.group(4) == 'RO':
                section = '.text'
            else:
                if test_re_armcc.group(3) == 'Data':
                    section = '.data'
                elif test_re_armcc.group(3) == 'Zero':
                    section = '.bss'
                else:
                    print "Malformed input found when parsing armcc map: %s" %\
                          line

            # check name of object or library
            object_name = self.parse_object_name_armcc(\
                test_re_armcc.group(6))

            return [object_name, size, section]

        else:
            return ["", 0, ""]

    def parse_object_name_iar(self, line):
        """ Parse object file

        Positional arguments:
        line - the line containing the object or library
        """

        # simple object (not library)
        if line[-2] == '.' and line[-1] == 'o':
            object_name = line
            return object_name

        else:
            return '[misc]'


    def parse_section_iar(self, line):
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
        """

        test_re_iar = re.match(RE_IAR, line)

        if test_re_iar:

            size = int(test_re_iar.group(4), 16)

            if test_re_iar.group(2) == 'const' or \
               test_re_iar.group(2) == 'ro code':
                section = '.text'
            elif test_re_iar.group(2) == 'zero' or \
            test_re_iar.group(2) == 'uninit':
                if test_re_iar.group(1)[0:4] == 'HEAP':
                    section = '.heap'
                elif test_re_iar.group(1)[0:6] == 'CSTACK':
                    section = '.stack'
                else:
                    section = '.bss' #  default section

            elif test_re_iar.group(2) == 'inited':
                section = '.data'
            else:
                print "Malformed input found when parsing IAR map: %s" % line

            # lookup object in dictionary and return module name
            temp = test_re_iar.group(5)
            object_name = self.parse_object_name_iar(temp)

            return [object_name, size, section]

        else:
            return ["", 0, ""] # no valid entry

    def parse_map_file_armcc(self, file_desc):
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

                [object_name, object_size, section] = \
                                self.parse_section_armcc(line)

                if object_size == 0 or object_name == "" or section == "":
                    pass
                else:
                    self.module_add(object_name, object_size, section)


    def check_new_library_iar(self, line):
        """
        Searches for libraries and returns name. Example:
        m7M_tls.a: [43]

        """

        RE_LIBRARY_IAR = re.compile(r'^(.+\.a)\:.+$')

        test_address_line = re.match(RE_LIBRARY_IAR, line)

        if test_address_line:
            return test_address_line.group(1)
        else:
            return ""

    def check_new_object_lib_iar(self, line):
        """
        Searches for objects within a library section and returns name. Example:
        rt7M_tl.a: [44]
            ABImemclr4.o                 6
            ABImemcpy_unaligned.o      118
            ABImemset48.o               50
            I64DivMod.o                238
            I64DivZer.o                  2

        """

        RE_OBJECT_LIBRARY_IAR = re.compile(r'^\s+(.+\.o)\s.*')

        test_address_line = re.match(RE_OBJECT_LIBRARY_IAR, line)

        if test_address_line:
            return test_address_line.group(1)
        else:
            return ""

    def parse_map_file_iar(self, file_desc):
        """ Main logic to decode IAR map files

        Positional arguments:
        file_desc - a file like object to parse as an IAR map file
        """

        # first round, search for objects
        with file_desc as infile:
            # Search area to parse
            for line in infile:
                if line.startswith('  Section  '):
                    break

            # Start decoding the map file
            for line in infile:

                [name, size, section] = self.parse_section_iar(line)

                if size == 0 or name == "" or section == "":
                    pass
                else:
                    self.module_add(name, size, section)

                if line.startswith('*** MODULE SUMMARY'): # finish section
                    break

            # Start decoding the map file
            current_library = ""
            for line in infile:

                library = self.check_new_library_iar(line)

                if library != "":
                    current_library = library

                object_name = self.check_new_object_lib_iar(line)

                if object_name != "" and current_library != "":
                    temp = '[lib]' + '/'+ current_library + '/'+ object_name
                    self.module_replace(object_name, temp)


    export_formats = ["json", "csv-ci", "table"]

    def list_dir_obj(self, path):
        """ Searches all objects in BUILD directory and creates list

        Positional arguments:
        path - the path to a map file
        """

        path = path.replace('\\', '/')

        # check location of map file
        RE_PATH_MAP_FILE = r'^(.+)\/.+\.map$'
        test_re = re.match(RE_PATH_MAP_FILE, path)

        if test_re:
            search_path = test_re.group(1)
        else:
            print "Warning: this doesn't look like an mbed project"
            return

        # create empty disctionary
        self.modules = dict()

        # search for object files
        for root, _, obj_files in os.walk(search_path):
            for obj_file in obj_files:
                if obj_file.endswith(".o"):

                    txt = os.path.join(root, obj_file)

                    txt = txt.replace('\\', '/')

                    # add relative path + object to list
                    self.module_init(txt[len(search_path)+1:])

        # The code below is a special case for TESTS.
        # mbed-os lives in a separate location and we need to explicitly search
        # their object files skiping the TESTS folder (already scanned above)

        # check location of mbed-os
        RE_PATH_MAP_FILE = r'^(.+)\/mbed-os\/.*TESTS\/.+\.map$'
        test_re = re.match(RE_PATH_MAP_FILE, path)

        if test_re == None:
            return

        search_path = test_re.group(1)

        # search for object files
        for root, _, obj_files in os.walk(search_path):
            for obj_file in obj_files:
                if 'TESTS' not in root and obj_file.endswith(".o"):

                    txt = os.path.join(root, obj_file)
                    txt = txt.replace('\\', '/')

                    # add relative path + object to list
                    self.module_init(txt[len(search_path)+1:])


    def reduce_depth(self, depth):
        """
        prints list of directories and objects. Examples:

        (1) depth = 1:
        main.o
        mbed-os

        (2) depth = 2:
        main.o
        mbed-os/test.o
        mbed-os/drivers

        """

        # depth 0 or None shows all entries
        if depth == 0 or depth == None:
            self.short_modules = deepcopy(self.modules)
            return

        self.short_modules = dict()

        # create reduced list
        for line in self.modules:

            data = line.split('/')
            ndir = len(data)

            temp = ''
            count = 0

            # iterate until the max depth level
            max_level = min(depth, ndir)

            # rebuild the path based on depth level
            while count < max_level:
                if count > 0:    # ignore '/' from first entry
                    temp = temp + '/'

                temp = temp + data[count]
                count += 1

            if temp not in self.short_modules:
                temp_dic = dict()
                for section_idx in self.all_sections:
                    temp_dic[section_idx] = 0
                self.short_modules[temp] = temp_dic

            for section_idx in self.all_sections:
                self.short_modules[temp][section_idx] += \
                    self.modules[line][section_idx]


    export_formats = ["json", "csv-ci", "table"]

    def generate_output(self, export_format, depth, file_output=None):
        """ Generates summary of memory map data

        Positional arguments:
        export_format - the format to dump

        Keyword arguments:
        file_desc - descriptor (either stdout or file)
        depth - directory depth on report

        Returns: generated string for the 'table' format, otherwise None
        """

        self.reduce_depth(depth)
        self.compute_report()

        try:
            if file_output:
                file_desc = open(file_output, 'wb')
            else:
                file_desc = sys.stdout
        except IOError as error:
            print "I/O error({0}): {1}".format(error.errno, error.strerror)
            return False

        to_call = {'json': self.generate_json,
                   'csv-ci': self.generate_csv,
                   'table': self.generate_table}[export_format]
        output = to_call(file_desc)

        if file_desc is not sys.stdout:
            file_desc.close()

        return output

    def generate_json(self, file_desc):
        """Generate a json file from a memory map

        Positional arguments:
        file_desc - the file to write out the final report to
        """
        file_desc.write(json.dumps(self.mem_report, indent=4))
        file_desc.write('\n')

        return None

    def generate_csv(self, file_desc):
        """Generate a CSV file from a memoy map

        Positional arguments:
        file_desc - the file to write out the final report to
        """
        csv_writer = csv.writer(file_desc, delimiter=',',
                                quoting=csv.QUOTE_MINIMAL)

        csv_module_section = []
        csv_sizes = []
        for i in sorted(self.short_modules):
            for k in self.print_sections:
                csv_module_section += [i+k]
                csv_sizes += [self.short_modules[i][k]]

        csv_module_section += ['static_ram']
        csv_sizes += [self.mem_summary['static_ram']]

        csv_module_section += ['total_flash']
        csv_sizes += [self.mem_summary['total_flash']]

        csv_writer.writerow(csv_module_section)
        csv_writer.writerow(csv_sizes)

        return None

    def generate_table(self, file_desc):
        """Generate a table from a memoy map

        Returns: string of the generated table
        """
        # Create table
        columns = ['Module']
        columns.extend(self.print_sections)

        table = PrettyTable(columns)
        table.align["Module"] = "l"
        for col in self.print_sections:
            table.align[col] = 'r'

        for i in list(self.print_sections):
            table.align[i] = 'r'

        for i in sorted(self.short_modules):
            row = [i]

            for k in self.print_sections:
                row.append(self.short_modules[i][k])

            table.add_row(row)

        subtotal_row = ['Subtotals']
        for k in self.print_sections:
            subtotal_row.append(self.subtotal[k])

        table.add_row(subtotal_row)

        output = table.get_string()
        output += '\n'

        output += "Total Static RAM memory (data + bss): %s bytes\n" % \
                        str(self.mem_summary['static_ram'])
        output += "Total Flash memory (text + data): %s bytes\n" % \
                        str(self.mem_summary['total_flash'])

        return output

    toolchains = ["ARM", "ARM_STD", "ARM_MICRO", "GCC_ARM", "GCC_CR", "IAR"]

    def compute_report(self):
        """ Generates summary of memory usage for main areas
        """

        for k in self.sections:
            self.subtotal[k] = 0

        for i in sorted(self.short_modules):
            for k in self.sections:
                self.subtotal[k] += self.short_modules[i][k]

        self.mem_summary = {
            'static_ram': (self.subtotal['.data'] + self.subtotal['.bss']),
            'total_flash': (self.subtotal['.text'] + self.subtotal['.data']),
        }

        self.mem_report = []
        for i in sorted(self.short_modules):
            self.mem_report.append({
                "module":i,
                "size":{
                    k:self.short_modules[i][k] for k in self.print_sections
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

        result = True
        try:
            with open(mapfile, 'r') as file_input:

                # Common to all toolchains: first search for objects in BUILD
                self.list_dir_obj(os.path.abspath(mapfile))

                if toolchain == "ARM" or toolchain == "ARM_STD" or\
                  toolchain == "ARM_MICRO":
                    self.parse_map_file_armcc(file_input)
                elif toolchain == "GCC_ARM" or toolchain == "GCC_CR":
                    self.parse_map_file_gcc(file_input)
                elif toolchain == "IAR":
                    self.parse_map_file_iar(file_input)
                else:
                    result = False

                self.remove_unused_modules()

        except IOError as error:
            print "I/O error({0}): {1}".format(error.errno, error.strerror)
            result = False
        return result

def main():
    """Entry Point"""

    version = '0.4.0'

    # Parser handling
    parser = argparse.ArgumentParser(
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
    if len(sys.argv) <= 1:
        parser.print_help()
        sys.exit(1)

    args = parser.parse_args()

    # Create memap object
    memap = MemapParser()

    # Parse and decode a map file
    if args.file and args.toolchain:
        if memap.parse(args.file, args.toolchain) is False:
            sys.exit(0)

    if args.depth is None:
        depth = 2  # default depth level
    else:
        depth = args.depth

    returned_string = None
    # Write output in file
    if args.output != None:
        returned_string = memap.generate_output(args.export, \
            depth, args.output)
    else: # Write output in screen
        returned_string = memap.generate_output(args.export, depth)

    if args.export == 'table' and returned_string:
        print returned_string

    sys.exit(0)

if __name__ == "__main__":
    main()
