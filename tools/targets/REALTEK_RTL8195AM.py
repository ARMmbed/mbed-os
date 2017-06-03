"""
mbed REALTEK_RTL8195AM elf2bin script
Copyright (c) 2011-2016 Realtek Semiconductor Corp.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

LIBRARIES BUILD
"""

import sys, array, struct, os, re, subprocess
import hashlib

from tools.paths import TOOLS_BOOTLOADERS
from datetime import datetime

# Constant Variables
RAM2_RSVD = 0x3131373835393138

def write_fixed_width_string(value, width, output):
    # cut string to list & reverse
    line = [value[i:i+2] for i in range(0, len(value), 2)]
    output.write("".join([chr(long(b, 16)) for b in line]))

def write_fixed_width_value(value, width, output):
    # convert to string
    line = format(value, '0%dx' % (width))
    if len(line) > width:
        print "[ERROR] value 0x%s cannot fit width %d" % (line, width)
        sys.exit(-1)
    # cut string to list & reverse
    line = [line[i:i+2] for i in range(0, len(line), 2)]
    line.reverse()
    # convert to write buffer
    output.write("".join([chr(long(b, 16)) for b in line]))

def append_image_file(image, output):
    input = open(image, "rb")
    output.write(input.read())
    input.close()

def prepend(image, image_prepend, toolchain, info):
    output = open(image_prepend, "wb")  
    write_fixed_width_value(info['size'], 8, output)
    write_fixed_width_value(info['addr'], 8, output)
    write_fixed_width_value(RAM2_RSVD, 16, output)
    with open(image, "rb") as input:
        if toolchain == "IAR":
            input.seek(info['addr'])
        output.write(input.read(info['size']))
    output.close()

def parse_section(toolchain, elf, section):
    info = {'addr':None, 'size':0};
    if toolchain not in ["GCC_ARM", "ARM_STD", "ARM", "ARM_MICRO", "IAR"]:
        print "[ERROR] unsupported toolchain " + toolchain
        sys.exit(-1)
    
    mapfile = elf.rsplit(".", 1)[0] + ".map"
    
    with open(mapfile, 'r') as infile:
        # Search area to parse
        for line in infile:
            if toolchain == "GCC_ARM":
                # .image2.table   0x[00000000]30000000       0x18
                #                 0x[00000000]30000000       __image2_start__ = .
                #                 0x[00000000]30000000       __image2_entry_func__ = .
                match = re.match(r'^' + section + \
                        r'\s+0x0{,8}(?P<addr>[0-9A-Fa-f]{8})\s+0x(?P<size>[0-9A-Fa-f]+).*$', line)
            elif toolchain in ["ARM_STD", "ARM", "ARM_MICRO"]:
                # Memory Map of the image
                #   Load Region LR_DRAM (Base: 0x30000000, Size: 0x00006a74, Max: 0x00200000, ABSOLUTE)
                #     Execution Region IMAGE2_TABLE (Base: 0x30000000, Size: 0x00000018, Max: 0xffffffff, ABSOLUTE, FIXED)
                #     Base Addr    Size         Type   Attr      Idx    E Section Name        Object
                #     0x30000000   0x00000004   Data   RO         5257    .image2.ram.data    rtl8195a_init.o
                match = re.match(r'^.*Region\s+' + section + \
                        r'\s+\(Base: 0x(?P<addr>[0-9A-Fa-f]{8}),\s+Size: 0x(?P<size>[0-9A-Fa-f]+), .*\)$', line)
            elif toolchain == "IAR":
                #   Section                 Kind        Address     Size  Object
                #   -------                 ----        -------     ----  ------
                # "A3":                                           0x8470
                #   IMAGE2                           0x10006000   0x5d18  <Block>
                #     .ram_image2.text               0x10006000   0x5bbc  <Block>
                #       .rodata             const    0x10006000     0x14  retarget.o [17]
                match = re.match(r'^\s+' + section + \
                        r'\s+0x(?P<addr>[0-9A-Fa-f]{8})\s+0x(?P<size>[0-9A-Fa-f]+)\s+.*<Block>$', line)
            if match:
                info['addr'] = int(match.group("addr"), 16)
                try:
                    info['size'] = int(match.group("size"), 16)
                except IndexError:
                    print "[WARNING] cannot find the size of section " + section
                return info

    print "[ERROR] cannot find the address of section " + section    
    return info

# ----------------------------
#       main function
# ----------------------------
def rtl8195a_elf2bin(toolchain, image_elf, image_bin):
    if toolchain == "GCC_ARM":
        img2_sections = [".image2.table", ".text", ".data"]
    elif toolchain in ["ARM_STD", "ARM", "ARM_MICRO"]:
        img2_sections = [".image2.table", ".text", ".data"]
    elif toolchain == "IAR":
        # actually it's block
        img2_sections = ["IMAGE2"]
    else:
        print("[error] unsupported toolchain") + toolchain
        return
    ram2_info = {'addr':None, 'size':0}
    image_name = os.path.splitext(image_elf)[0]

    ram1_prepend_bin = os.path.join(TOOLS_BOOTLOADERS, "REALTEK_RTL8195AM", "ram_1_prepend.bin")
    ram2_prepend_bin = image_name + '-ram_2_prepend.bin'
    
    old_bin = image_name + '.bin'
    for section in img2_sections:
        section_info = parse_section(toolchain, image_elf, section)
        if ram2_info['addr'] is None or ram2_info['addr'] > section_info['addr']:
            ram2_info['addr'] = section_info['addr']
        ram2_info['size'] = ram2_info['size'] + section_info['size']

    prepend(old_bin, ram2_prepend_bin, toolchain, ram2_info)
    # write output file
    output = open(image_bin, "wb")
    append_image_file(ram1_prepend_bin, output)
    append_image_file(ram2_prepend_bin, output)
    output.close()
    # post built done

