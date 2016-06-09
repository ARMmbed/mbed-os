#! /usr/bin/env python

# Memory Map File Analyser for ARM mbed OS

import argparse
import sys
import string
import os
import re
import json
import time
import string
import StringIO 
from prettytable import PrettyTable

debug = False

class MemmapParser(object):

    def __init__(self):
        """
        General initialization
        """

        # list of all modules and their sections
        self.modules = dict()

        self.misc_flash_sections = ('.interrupts', '.flash_config')

        self.other_sections = ('.interrupts_ram', '.init', '.ARM.extab', '.ARM.exidx', '.ARM.attributes', \
                 '.eh_frame', '.init_array', '.fini_array', '.jcr', '.stab', '.stabstr', \
                 '.ARM.exidx','.ARM' )

        # sections to print info (generic for all toolchains)
        self.sections = ('.text', '.data', '.bss', '.heap', '.stack',) 

        # need to have sections merged in this order ()
        self.all_sections = self.sections + self.other_sections + \
                            self.misc_flash_sections + ('unknown', 'OUTPUT')

        self.print_sections = ('.text', '.data', '.bss')

        # list of all object files and mappting to module names
        self.object_to_module = dict() 


    def generate_output(self, file, json_mode):
        """
        Generates summary of memory map data

        Parameters
            file: descriptor (either stdout or file)
            json_mode: generates output in json formal (True/False)
        """

        buf = StringIO.StringIO()

        # Calculate misc flash sections
        misc_flash_mem = 0
        for i in self.modules:
            for k in self.misc_flash_sections:
                if self.modules[i][k]:
                    misc_flash_mem += self.modules[i][k]

        # Create table
        colums = ['Module']
        for i in list(self.print_sections):
            colums.append(i) 

        table = PrettyTable(colums)
        table.align["Module"] = "l"

        subtotal = dict()
        for k in self.sections:
            subtotal[k] = 0

        json_obj = []
        for i in sorted(self.modules):
            
            row = []
            row.append(i)

            for k in self.sections: 
                subtotal[k] += self.modules[i][k]

            for k in self.print_sections:
                row.append(self.modules[i][k])
            
            json_obj.append({ "module":i, "size":{k:self.modules[i][k] for k in self.print_sections}})
            table.add_row(row)

        subtotal_row = ['Subtotals']
        for k in self.print_sections:
            subtotal_row.append(subtotal[k]) 

        table.add_row(subtotal_row)

        if json_mode:
            json_obj.append({ "summary":{'static_ram':(subtotal['.data']+subtotal['.bss']),
                                               'heap':(subtotal['.heap']),
                                              'stack':(subtotal['.stack']),
                                          'total_ram':(subtotal['.data']+subtotal['.bss']+subtotal['.heap']+subtotal['.stack']),
                                        'total_flash':(subtotal['.text']+subtotal['.data']+misc_flash_mem),}})

            file.write(json.dumps(json_obj, indent=4))
            file.write('\n')
        else:
            file.write(table.get_string())
            file.write('\n')
            file.write("Static RAM memory (data + bss): %s\n" % (str(subtotal['.data']+subtotal['.bss'])))
            file.write("Heap: %s\n" % str(subtotal['.heap']))
            file.write("Stack: %s\n" % str(subtotal['.stack']))
            file.write("Total RAM memory (data + bss + heap + stack): %s\n" % (str(subtotal['.data']+subtotal['.bss']+subtotal['.heap']+subtotal['.stack'])))
            file.write("Total Flash memory (text + data + misc): %s\n" % (str(subtotal['.text']+subtotal['.data']+misc_flash_mem)))
        return

    def module_add(self, module_name, size, section):
        """
        Adds a module / section to the list
        """

        if module_name in self.modules:
                self.modules[module_name][section] += size
        else:
            temp_dic = dict()
            for x in self.all_sections:
                temp_dic[x] = 0
            temp_dic[section] = size
            self.modules[module_name] = temp_dic

    def find_start_gcc(self,line):
        """
        Checks location of gcc map file to start parsing map file
        """
        if line.startswith('Linker script and memory map'):
            return True
        else:
            return False

    def find_start_armcc(self,line):
        """
        Checks location of armcc map file to start parsing map file
        """
        if line.startswith('    Base Addr    Size'):
            return True
        else:
            return False

    def find_start_iar(self,line):
        """
        Checks location of armcc map file to start parsing map file
        """
        if line.startswith('  Section  '):
            return True
        else:
            return False

    def check_new_section_gcc(self,line):
        """
        Check whether a new section in a map file has been detected (only applies to gcc)
        """

        for i in self.all_sections:
            if line.startswith(i):
                return i  # should name of the section (assuming it's a known one)

        if line.startswith('.'):
            return 'unknown'     # all others are clasified are unknown
        else:
            return False         # everything else, means no change in section

    def path_object_to_module_name(self,txt):
        """
        Parses path to object file and extracts module / object data 
        """

        txt = txt.replace('\\','/')
        rex_mbed_os_name = r'^.+mbed-os\/(.+)\/(.+\.o)$'
        test_rex_mbed_os_name = re.match(rex_mbed_os_name,txt)
 
        if test_rex_mbed_os_name:

            object_name = test_rex_mbed_os_name.group(2)
            data = test_rex_mbed_os_name.group(1).split('/')
            ndata = len(data)

            if ndata == 1:
                module_name = data[0]
            else:
                module_name = data[0] + '/' + data[1]

            return [module_name, object_name]
        else:
            return ['Misc', ""]
               

    def parse_section_gcc(self,line):
        """
        Parse data from a section of gcc map file
        """
        # examples
        #                 0x00004308       0x7c ./.build/K64F/GCC_ARM/mbed-os/hal/targets/hal/TARGET_Freescale/TARGET_KPSDK_MCUS/spi_api.o
        #  .text          0x00000608      0x198 ./.build/K64F/GCC_ARM/mbed-os/core/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_RTOS_M4_M7/TOOLCHAIN_GCC/HAL_CM4.o
        rex_address_len_name = r'^\s+.*0x(\w{8,16})\s+0x(\w+)\s(.+)$'

        test_address_len_name = re.match(rex_address_len_name,line)

        if test_address_len_name:

            if int(test_address_len_name.group(2),16) == 0: # size == 0
                return ["",0] # no valid entry
            else:
                m_name, m_object = self.path_object_to_module_name(test_address_len_name.group(3))
                m_size = int(test_address_len_name.group(2),16) 
                return [m_name,m_size]

        else: # special cortner case for *fill* sections
            #  example
            # *fill*         0x0000abe4        0x4
            rex_address_len = r'^\s+\*fill\*\s+0x(\w{8,16})\s+0x(\w+).*$'
            test_address_len = re.match(rex_address_len,line)

            if test_address_len:
                if int(test_address_len.group(2),16) == 0: # size == 0
                    return ["",0] # no valid entry
                else:
                    m_name = 'Misc'
                    m_size = int(test_address_len.group(2),16)
                    return [m_name,m_size]
            else:
                return ["",0] # no valid entry

    def parse_map_file_gcc(self, file):
        """
        Main logic to decode gcc map files
        """

        current_section = 'unknown'

        with file as infile:

            # Search area to parse
            for line in infile:
                if self.find_start_gcc(line) == True:
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

                if debug:
                    print "Line: %s" % line,
                    print "Module: %s\tSection: %s\tSize: %s" % (module_name,current_section,module_size)
                    raw_input("----------")

    def parse_section_armcc(self,line):
        """
        Parse data from an armcc map file
        """
        # Examples of armcc map file:
        #     Base_Addr    Size         Type   Attr      Idx    E Section Name        Object
        #     0x00000000   0x00000400   Data   RO        11222    RESET               startup_MK64F12.o
        #     0x00000410   0x00000008   Code   RO        49364  * !!!main             c_w.l(__main.o)
        rex_armcc = r'^\s+0x(\w{8})\s+0x(\w{8})\s+(\w+)\s+(\w+)\s+(\d+)\s+[*]?.+\s+(.+)$'

        test_rex_armcc = re.match(rex_armcc,line)

        if test_rex_armcc:

            size = int(test_rex_armcc.group(2),16)

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

            return [module_name,size,section]

        else:
            return ["",0,""] # no valid entry

    def parse_section_iar(self,line):
        """
        Parse data from an IAR map file
        """
        # Examples of IAR map file:
        #  Section             Kind        Address     Size  Object
        #  .intvec             ro code  0x00000000    0x198  startup_MK64F12.o [15]
        #  .rodata             const    0x00000198      0x0  zero_init3.o [133]
        #  .iar.init_table     const    0x00008384     0x2c  - Linker created -
        #  Initializer bytes   const    0x00000198     0xb2  <for P3 s0>
        #  .data               inited   0x20000000     0xd4  driverAtmelRFInterface.o [70]
        #  .bss                zero     0x20000598    0x318  RTX_Conf_CM.o [4]
        #  .iar.dynexit        uninit   0x20001448    0x204  <Block tail>
        #    HEAP              uninit   0x20001650  0x10000  <Block tail>
        rex_iar = r'^\s+(.+)\s+(zero|const|ro code|inited|uninit)\s+0x(\w{8})\s+0x(\w+)\s+(.+)\s.+$'

        test_rex_iar = re.match(rex_iar,line)

        if test_rex_iar:

            size = int(test_rex_iar.group(4),16)

            if test_rex_iar.group(2) == 'const' or test_rex_iar.group(2) == 'ro code':
                section = '.text'
            elif test_rex_iar.group(2) == 'zero' or test_rex_iar.group(2) == 'uninit':
               
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

            return [module_name,size,section]

        else:
            return ["",0,""] # no valid entry

    def parse_map_file_armcc(self, file):
        """
        Main logic to decode armcc map files
        """

        with file as infile:

            # Search area to parse
            for line in infile:
                if self.find_start_armcc(line) == True:
                    break

            # Start decoding the map file
            for line in infile:

                [name, size, section] = self.parse_section_armcc(line)

                if size == 0 or name == "" or section == "":
                    pass
                else:
                    self.module_add(name, size, section)

    def parse_map_file_iar(self, file):
        """
        Main logic to decode armcc map files
        """

        with file as infile:

            # Search area to parse
            for line in infile:
                if self.find_start_iar(line) == True:
                    break

            # Start decoding the map file
            for line in infile:

                [name, size, section] = self.parse_section_iar(line)

                if size == 0 or name == "" or section == "":
                    pass
                else:
                    self.module_add(name, size, section)

    def search_objects(self,path,toolchain):
        """
        Check whether the specified map file matches with the toolchain.
        Searches for object files and creates mapping: object --> module
        """

        path = path.replace('\\','/')

        # check location of map file
        rex = r'^(.+\/)' + re.escape(toolchain) + r'\/(.+\.map)$'
        test_rex = re.match(rex,path)
        
        if test_rex:
            search_path = test_rex.group(1) + toolchain + '/mbed-os/'
        else:
            # It looks this is not an mbed OS project
            # object-to-module mapping cannot be generated
            print "Warning: specified toolchain doesn't match with path to the memory map file."
            return

        for root, dirs, files in os.walk(search_path):
            for file in files:
                if file.endswith(".o"):
                    module_name, object_name = self.path_object_to_module_name(os.path.join(root, file))

                    if object_name in self.object_to_module:
                        print "WARNING: multiple usages of object file: %s" % object_name
                        print "    Current: %s" % self.object_to_module[object_name]
                        print "    New:     %s" % module_name
                        print " "
                        
                    else:
                        self.object_to_module.update({object_name:module_name})

def main():

    version = '0.3.7'
    time_start = time.clock()

    # Parser handling
    parser = argparse.ArgumentParser(description="Memory Map File Analyser for ARM mbed OS\nversion %s" % version)

    parser.add_argument('file', help='memory map file')

    parser.add_argument('-t','--toolchain', dest='toolchain', help='select a toolchain that corresponds to the memory map file (ARM, GCC_ARM, IAR)',
                        required=True)

    parser.add_argument('-o','--output',help='output file name', required=False)

    parser.add_argument('-j', '--json', dest='json', required=False, action="store_true",
                      help='output in JSON formatted list')

    parser.add_argument('-v', '--version', action='version', version=version)

    # Parse/run command
    if len(sys.argv) <= 1:
        parser.print_help()
        sys.exit(1)

    args, remainder = parser.parse_known_args()

    try:
        file_input = open(args.file,'rt')
    except IOError as e:
        print "I/O error({0}): {1}".format(e.errno, e.strerror)
        sys.exit(0)

    # Creates parser object
    t = MemmapParser()
  
    # Decode map file depending on the toolchain
    if args.toolchain == "ARM":
        t.search_objects(os.path.abspath(args.file),args.toolchain)
        t.parse_map_file_armcc(file_input)
    elif args.toolchain == "GCC_ARM":
        t.parse_map_file_gcc(file_input)
    elif args.toolchain == "IAR":
        print "WARNING: IAR Compiler not fully supported (yet)"
        print " "
        t.search_objects(os.path.abspath(args.file),args.toolchain)
        t.parse_map_file_iar(file_input)
    else:
        print "Invalid toolchain. Options are: ARM, GCC_ARM, IAR"
        sys.exit(0)

    # Write output in file
    if args.output != None:
        try:
            file_output = open(args.output,'w')
            t.generate_output(file_output,args.json)
            file_output.close()
        except IOError as e:
            print "I/O error({0}): {1}".format(e.errno, e.strerror)
            sys.exit(0)
    else: # Write output in screen
        t.generate_output(sys.stdout,args.json)
    
    file_input.close()

    print "Elapsed time: %smS" %int(round((time.clock()-time_start)*1000))
    
    sys.exit(0)

if __name__ == "__main__":
    main()