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
        line = format(value, '0%dx' %(width))
        if len(line) > width:
            print "[ERROR] value 0x%s cannot fit width %d" %(line, width)
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

# ----------------------------
#       main function
# ----------------------------
def prepend(image, sym_addr, image_prepend):

    # parse input arguments
    offset = 44

    output_file = image_prepend
    output = open(output_file, "wb")

    write_fixed_width_value(os.stat(image).st_size, 8, output)
    write_fixed_width_value(int(sym_addr), 8, output)
    write_fixed_width_value(RAM2_RSVD, 16, output)

    append_image_file(image, output)
    output.close()

def find_section(toolchain, elf, section):
    ret = None
    mapfile = elf.rsplit(".", 1)[0] + ".map"

    if toolchain == "GCC_ARM":
        # .image2.table   0x[00000000]30000000       0x18
        #                 0x[00000000]30000000       __image2_start__ = .
        #                 0x[00000000]30000000       __image2_entry_func__ = .
        with open(mapfile, 'r') as infile:
            # Search area to parse
            for line in infile:
                if line.startswith(section):
                    match = re.match(r'^' + section + \
                            r'\s+0x0{,8}(?P<addr>[0-9A-Fa-f]{8})\s+.*$', line)
                    if match:
                        ret = match.group("addr")
                        break
    elif toolchain in ["ARM_STD", "ARM", "ARM_MICRO"]:
        # Memory Map of the image
        #   Load Region LR_DRAM (Base: 0x30000000, Size: 0x00006a74, Max: 0x00200000, ABSOLUTE)
        #     Execution Region IMAGE2_TABLE (Base: 0x30000000, Size: 0x00000018, Max: 0xffffffff, ABSOLUTE, FIXED)
        #     Base Addr    Size         Type   Attr      Idx    E Section Name        Object
        #     0x30000000   0x00000004   Data   RO         5257    .image2.ram.data    rtl8195a_init.o
        with open(mapfile, 'r') as infile:
            # Search area to parse
            for line in infile:
                if line.startswith('Memory Map of the image'):
                    break
            # Search section(block)
            for line in infile:
                match = re.match(r'^.*Region\s+' + section + \
                        r'\s+\(Base: 0x(?P<addr>[0-9A-Fa-f]{8}),\s+Size:.*\)$', line)
                if match:
                    ret = match.group("addr")
                    break
    elif toolchain == "IAR":
        #   Section                 Kind        Address     Size  Object
        #   -------                 ----        -------     ----  ------
        # "A3":                                           0x8470
        #   IMAGE2                           0x10006000   0x5d18  <Block>
        #     .ram_image2.text               0x10006000   0x5bbc  <Block>
        #       .rodata             const    0x10006000     0x14  retarget.o [17]
        with open(mapfile, 'r') as infile:
            # Search area to parse
            for line in infile:
                if line.startswith('  Section  '):
                    break
            # Search section(block)
            for line in infile:
                match = re.match(r'^\s+' + section + \
                        r'\s+0x(?P<addr>[0-9A-Fa-f]{8})\s+.*<Block>$', line)
                if match:
                    ret = match.group("addr")
                    break
    else:
        print "[ERROR] unsupported toolchain " + toolchain
        sys.exit(-1)

    if not ret:
        print "[ERROR] cannot find the address of section " + section
        return 0

    return int(ret,16)

# ----------------------------
#       main function
# ----------------------------
def rtl8195a_elf2bin(toolchain, image_elf, image_bin):
    if toolchain == "GCC_ARM":
        img2_section = ".image2.table"
    elif toolchain in ["ARM_STD", "ARM", "ARM_MICRO"]:
        img2_section = ".image2.table"
    elif toolchain == "IAR":
        # actually it's block
        img2_section = "IMAGE2"
    else:
	return

    image_name = os.path.splitext(image_elf)[0]

    ram1_prepend_bin = os.path.join(TOOLS_BOOTLOADERS, "REALTEK_RTL8195AM", "ram_1_prepend.bin")
    ram2_prepend_bin = image_name + '-ram_2_prepend.bin'
    ram2_bin = image_name + '.bin'
    ram2_addr = find_section(toolchain, image_elf, img2_section)
    #print("toolchain = %s, ram2_bin = %s, ram2_addr = 0x%x" % (toolchain, ram2_bin, ram2_addr))

    prepend(ram2_bin, ram2_addr, ram2_prepend_bin)

    # write output file
    output = open(image_bin, "wb")
    append_image_file(ram1_prepend_bin, output)
    append_image_file(ram2_prepend_bin, output)
    output.close()
