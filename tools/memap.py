#!/usr/bin/env python

"""Memory Map File Analyser for ARM mbed"""

import sys
import os
import re
import csv
import json
import argparse
from prettytable import PrettyTable

from utils import argparse_filestring_type, \
    argparse_lowercase_hyphen_type, argparse_uppercase_type

DEBUG = False

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

    def __init__(self, detailed_misc=False):
        """ General initialization
        """
        # 
        self.detailed_misc = detailed_misc
        
        # list of all modules and their sections
        self.modules = dict()

        # sections must be defined in this order to take irrelevant out
        self.all_sections = self.sections + self.other_sections + \
                            self.misc_flash_sections + ('unknown', 'OUTPUT')

        # list of all object files and mappting to module names
        self.object_to_module = dict()

        # Memory report (sections + summary)
        self.mem_report = []

        # Just the memory summary section
        self.mem_summary = dict()

        self.subtotal = dict()

    def module_add(self, module_name, size, section):
        """ Adds a module / section to the list

        Positional arguments:
        module_name - name of the module to add
        size - the size of the module being added
        section - the section the module contributes to
        """

        if module_name in self.modules:
            self.modules[module_name][section] += size
        else:
            temp_dic = dict()
            for section_idx in self.all_sections:
                temp_dic[section_idx] = 0
            temp_dic[section] = size
            self.modules[module_name] = temp_dic

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

    
    def path_object_to_module_name(self, txt):
        """ Parse a path to object file to extract it's module and object data

        Positional arguments:
        txt - the path to parse the object and module name from
        """

        txt = txt.replace('\\', '/')
        rex_mbed_os_name = r'^.+mbed-os\/(.+)\/(.+\.o)$'
        test_rex_mbed_os_name = re.match(rex_mbed_os_name, txt)

        if test_rex_mbed_os_name:

            object_name = test_rex_mbed_os_name.group(2)
            data = test_rex_mbed_os_name.group(1).split('/')
            ndata = len(data)

            if ndata == 1:
                module_name = data[0]
            else:
                module_name = data[0] + '/' + data[1]

            return [module_name, object_name]
            
        elif self.detailed_misc:           
            rex_obj_name = r'^.+\/(.+\.o\)*)$'
            test_rex_obj_name = re.match(rex_obj_name, txt)
            if test_rex_obj_name:
                object_name = test_rex_obj_name.group(1)
                return ['Misc/' + object_name, ""]        
                
            return ['Misc', ""]
        else: 
            return ['Misc', ""]

    def parse_section_gcc(self, line):
        """ Parse data from a section of gcc map file

        examples:
                        0x00004308       0x7c ./BUILD/K64F/GCC_ARM/mbed-os/hal/targets/hal/TARGET_Freescale/TARGET_KPSDK_MCUS/spi_api.o
         .text          0x00000608      0x198 ./BUILD/K64F/GCC_ARM/mbed-os/core/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_RTOS_M4_M7/TOOLCHAIN_GCC/HAL_CM4.o

        Positional arguments:
        line - the line to parse a section from
        """
        rex_address_len_name = re.compile(
            r'^\s+.*0x(\w{8,16})\s+0x(\w+)\s(.+)$')

        test_address_len_name = re.match(rex_address_len_name, line)

        if test_address_len_name:

            if int(test_address_len_name.group(2), 16) == 0: # size == 0
                return ["", 0] # no valid entry
            else:
                m_name, _ = self.path_object_to_module_name(
                    test_address_len_name.group(3))
                m_size = int(test_address_len_name.group(2), 16)
                return [m_name, m_size]

        else: # special corner case for *fill* sections
            #  example
            # *fill*         0x0000abe4        0x4
            rex_address_len = r'^\s+\*fill\*\s+0x(\w{8,16})\s+0x(\w+).*$'
            test_address_len = re.match(rex_address_len, line)

            if test_address_len:
                if int(test_address_len.group(2), 16) == 0: # size == 0
                    return ["", 0] # no valid entry
                else:
                    m_name = 'Fill'
                    m_size = int(test_address_len.group(2), 16)
                    return [m_name, m_size]
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

                [module_name, module_size] = self.parse_section_gcc(line)

                if module_size == 0 or module_name == "":
                    pass
                else:
                    self.module_add(module_name, module_size, current_section)

                if DEBUG:
                    print "Line: %s" % line,
                    print "Module: %s\tSection: %s\tSize: %s" % \
                        (module_name, current_section, module_size)
                    raw_input("----------")

    def parse_section_armcc(self, line):
        """ Parse data from an armcc map file

        Examples of armcc map file:
            Base_Addr    Size         Type   Attr      Idx    E Section Name        Object
            0x00000000   0x00000400   Data   RO        11222    RESET               startup_MK64F12.o
            0x00000410   0x00000008   Code   RO        49364  * !!!main             c_w.l(__main.o)

        Positional arguments:
        line - the line to parse the section data from
        """

        test_rex_armcc = re.match(RE_ARMCC, line)

        if test_rex_armcc:

            size = int(test_rex_armcc.group(2), 16)

            if test_rex_armcc.group(4) == 'RO':
                section = '.text'
            else:
                if test_rex_armcc.group(3) == 'Data':
                    section = '.data'
                elif test_rex_armcc.group(3) == 'Zero':
                    section = '.bss'
                else:
                    print "BUG armcc map parser"
                    raw_input()

            # lookup object in dictionary and return module name
            object_name = test_rex_armcc.group(6)
            if object_name in self.object_to_module:
                module_name = self.object_to_module[object_name]
            else:
                module_name = 'Misc'

            return [module_name, size, section]

        else:
            return ["", 0, ""] # no valid entry

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

        test_rex_iar = re.match(RE_IAR, line)

        if test_rex_iar:

            size = int(test_rex_iar.group(4), 16)

            if test_rex_iar.group(2) == 'const' or \
               test_rex_iar.group(2) == 'ro code':
                section = '.text'
            elif test_rex_iar.group(2) == 'zero' or \
            test_rex_iar.group(2) == 'uninit':
                if test_rex_iar.group(1)[0:4] == 'HEAP':
                    section = '.heap'
                elif test_rex_iar.group(1)[0:6] == 'CSTACK':
                    section = '.stack'
                else:
                    section = '.bss' #  default section

            elif test_rex_iar.group(2) == 'inited':
                section = '.data'
            else:
                print "BUG IAR map parser"
                raw_input()

            # lookup object in dictionary and return module name
            object_name = test_rex_iar.group(5)
            if object_name in self.object_to_module:
                module_name = self.object_to_module[object_name]
            else:
                module_name = 'Misc'

            return [module_name, size, section]

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

                [name, size, section] = self.parse_section_armcc(line)

                if size == 0 or name == "" or section == "":
                    pass
                else:
                    self.module_add(name, size, section)

    def parse_map_file_iar(self, file_desc):
        """ Main logic to decode IAR map files

        Positional arguments:
        file_desc - a file like object to parse as an IAR map file
        """

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

    def search_objects(self, path):
        """ Searches for object files and creates mapping: object --> module

        Positional arguments:
        path - the path to an object file
        """

        path = path.replace('\\', '/')

        # check location of map file
        rex = r'^(.+)' + r'\/(.+\.map)$'
        test_rex = re.match(rex, path)

        if test_rex:
            search_path = test_rex.group(1) + '/mbed-os/'
        else:
            print "Warning: this doesn't look like an mbed project"
            return

        for root, _, obj_files in os.walk(search_path):
            for obj_file in obj_files:
                if obj_file.endswith(".o"):
                    module_name, object_name = self.path_object_to_module_name(
                        os.path.join(root, obj_file))

                    if object_name in self.object_to_module:
                        if DEBUG:
                            print "WARNING: multiple usages of object file: %s"\
                                % object_name
                            print "    Current: %s" % \
                                self.object_to_module[object_name]
                            print "    New:     %s" % module_name
                            print " "
                    else:
                        self.object_to_module.update({object_name:module_name})

    export_formats = ["json", "csv-ci", "table"]

    def generate_output(self, export_format, file_output=None):
        """ Generates summary of memory map data

        Positional arguments:
        export_format - the format to dump

        Keyword arguments:
        file_desc - descriptor (either stdout or file)

        Returns: generated string for the 'table' format, otherwise None
        """

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
        for i in sorted(self.modules):
            for k in self.print_sections:
                csv_module_section += [i+k]
                csv_sizes += [self.modules[i][k]]

        csv_module_section += ['static_ram']
        csv_sizes += [self.mem_summary['static_ram']]

        csv_module_section += ['heap']
        if self.mem_summary['heap'] == 0:
            csv_sizes += ['unknown']
        else:
            csv_sizes += [self.mem_summary['heap']]

        csv_module_section += ['stack']
        if self.mem_summary['stack'] == 0:
            csv_sizes += ['unknown']
        else:
            csv_sizes += [self.mem_summary['stack']]

        csv_module_section += ['total_ram']
        csv_sizes += [self.mem_summary['total_ram']]

        csv_module_section += ['total_flash']
        csv_sizes += [self.mem_summary['total_flash']]

        csv_writer.writerow(csv_module_section)
        csv_writer.writerow(csv_sizes)

        return None

    def generate_table(self, file_desc):
        """Generate a table from a memoy map

        Positional arguments:
        file_desc - the file to write out the final report to

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

        for i in sorted(self.modules):
            row = [i]

            for k in self.print_sections:
                row.append(self.modules[i][k])

            table.add_row(row)

        subtotal_row = ['Subtotals']
        for k in self.print_sections:
            subtotal_row.append(self.subtotal[k])

        table.add_row(subtotal_row)

        output = table.get_string()
        output += '\n'

        if self.mem_summary['heap'] == 0:
            output += "Allocated Heap: unknown\n"
        else:
            output += "Allocated Heap: %s bytes\n" % \
                        str(self.mem_summary['heap'])

        if self.mem_summary['stack'] == 0:
            output += "Allocated Stack: unknown\n"
        else:
            output += "Allocated Stack: %s bytes\n" % \
                        str(self.mem_summary['stack'])

        output += "Total Static RAM memory (data + bss): %s bytes\n" % \
                        str(self.mem_summary['static_ram'])
        output += "Total RAM memory (data + bss + heap + stack): %s bytes\n" % \
                        str(self.mem_summary['total_ram'])
        output += "Total Flash memory (text + data + misc): %s bytes\n" % \
                        str(self.mem_summary['total_flash'])

        return output

    toolchains = ["ARM", "ARM_STD", "ARM_MICRO", "GCC_ARM", "IAR"]

    def compute_report(self):
        for k in self.sections:
            self.subtotal[k] = 0

        for i in sorted(self.modules):
            for k in self.sections:
                self.subtotal[k] += self.modules[i][k]

        # Calculate misc flash sections
        self.misc_flash_mem = 0
        for i in self.modules:
            for k in self.misc_flash_sections:
                if self.modules[i][k]:
                    self.misc_flash_mem += self.modules[i][k]

        self.mem_summary = {
            'static_ram': (self.subtotal['.data'] + self.subtotal['.bss']),
            'heap': (self.subtotal['.heap']),
            'stack': (self.subtotal['.stack']),
            'total_ram': (self.subtotal['.data'] + self.subtotal['.bss'] +
                          self.subtotal['.heap']+self.subtotal['.stack']),
            'total_flash': (self.subtotal['.text'] + self.subtotal['.data'] +
                            self.misc_flash_mem),
        }

        self.mem_report = []
        for i in sorted(self.modules):
            self.mem_report.append({
                "module":i,
                "size":{
                    k:self.modules[i][k] for k in self.print_sections
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
                if toolchain == "ARM" or toolchain == "ARM_STD" or\
                   toolchain == "ARM_MICRO":
                    self.search_objects(os.path.abspath(mapfile))
                    self.parse_map_file_armcc(file_input)
                elif toolchain == "GCC_ARM":
                    self.parse_map_file_gcc(file_input)
                elif toolchain == "IAR":
                    self.search_objects(os.path.abspath(mapfile))
                    self.parse_map_file_iar(file_input)
                else:
                    result = False
            
            self.compute_report()
        
        except IOError as error:
            print "I/O error({0}): {1}".format(error.errno, error.strerror)
            result = False
        return result

def main():
    """Entry Point"""

    version = '0.3.12'

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
        '-o', '--output', help='output file name', required=False)

    parser.add_argument(
        '-e', '--export', dest='export', required=False, default='table',
        type=argparse_lowercase_hyphen_type(MemapParser.export_formats,
                                            'export format'),
        help="export format (examples: %s: default)" %
        ", ".join(MemapParser.export_formats))

    parser.add_argument('-v', '--version', action='version', version=version)
    
    parser.add_argument('-d', '--detailed', action='store_true', help='Displays the elements in "Misc" in a detailed fashion', required=False)

    # Parse/run command
    if len(sys.argv) <= 1:
        parser.print_help()
        sys.exit(1)


    args = parser.parse_args()

    # Create memap object
    memap = MemapParser(detailed_misc=args.detailed)

    # Parse and decode a map file
    if args.file and args.toolchain:
        if memap.parse(args.file, args.toolchain) is False:
            sys.exit(0)

    returned_string = None
    # Write output in file
    if args.output != None:
        returned_string = memap.generate_output(args.export, args.output)
    else: # Write output in screen
        returned_string = memap.generate_output(args.export)

    if args.export == 'table' and returned_string:
        print returned_string

    sys.exit(0)

if __name__ == "__main__":
    main()
