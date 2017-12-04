"""
<<<<<<< HEAD
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
=======
Realtek Semiconductor Corp.

RTL8195A elf2bin script
>>>>>>> upstream/master
"""

import sys, array, struct, os, re, subprocess
import hashlib
<<<<<<< HEAD

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
=======
import shutil
import time
import binascii

from tools.paths import TOOLS_BOOTLOADERS
from tools.toolchains import TOOLCHAIN_PATHS

# Constant Variables
TAG = 0x81950001
VER = 0x81950001
CAMPAIGN = binascii.hexlify('FFFFFFFFFFFFFFFF')

RAM2_HEADER = {
    'tag': 0,
    'ver': 0,
    'timestamp': 0,
    'size': 72,
    'hash': 'FF',
    'campaign': 'FF',
    'crc32': 0xFFFFFFFF,
}

def format_number(number, width):
    # convert to string
    line = format(number, '0%dx' % (width))
    if len(line) > width:
        print "[ERROR] 0x%s cannot fit in width %d" % (line, width)
>>>>>>> upstream/master
        sys.exit(-1)
    # cut string to list & reverse
    line = [line[i:i+2] for i in range(0, len(line), 2)]
    line.reverse()
<<<<<<< HEAD
    # convert to write buffer
    output.write("".join([chr(long(b, 16)) for b in line]))
=======
    return binascii.a2b_hex("".join(line))

def format_string(string):
    return binascii.a2b_hex(string)

def write_number(value, width, output):
    output.write(format_number(value, width))

def write_string(value, width, output):
    output.write(format_string(value))
>>>>>>> upstream/master

def append_image_file(image, output):
    input = open(image, "rb")
    output.write(input.read())
    input.close()

<<<<<<< HEAD
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
        printf("[error] unsupported toolchain") + toolchain
        return
    ram2_info = {'addr':None, 'size':0}
    image_name = os.path.splitext(image_elf)[0]

    ram1_prepend_bin = os.path.join(TOOLS_BOOTLOADERS, "REALTEK_RTL8195AM", "ram_1_prepend.bin")
    ram2_prepend_bin = image_name + '-ram_2_prepend.bin'
    
    old_bin = image_name + '.bin'
    for section in img2_sections:
        section_info = parse_section(toolchain, image_elf, section)
        #print("addr 0x%x, size 0x%x" % (section_info['addr'], section_info['size']))
        if ram2_info['addr'] is None or ram2_info['addr'] > section_info['addr']:
            ram2_info['addr'] = section_info['addr']
        ram2_info['size'] = ram2_info['size'] + section_info['size']

    #print("toolchain = %s, bin name = \"%s, ram2_addr = 0x%x, ram2_size = 0x%x" % (toolchain, old_bin, ram2_info['addr'], ram2_info['size']))

    prepend(old_bin, ram2_prepend_bin, toolchain, ram2_info)
    # write output file
    output = open(image_bin, "wb")
    append_image_file(ram1_prepend_bin, output)
    append_image_file(ram2_prepend_bin, output)
    output.close()
    #print("post built done")

=======
def write_padding_bytes(output_name, size):
    current_size = os.stat(output_name).st_size
    padcount = size - current_size
    if padcount < 0:
        print "[ERROR] image is larger than expected size"
        sys.exit(-1)
    output = open(output_name, "ab")
    output.write('\377' * padcount)
    output.close()

def crc32_checksum(string):
    return binascii.crc32(string) & 0xFFFFFFFF

def sha256_checksum(filename, block_size=65536):
    sha256 = hashlib.sha256()
    with open(filename, 'rb') as f:
        for block in iter(lambda: f.read(block_size), b''):
            sha256.update(block)
    return sha256.hexdigest()

def epoch_timestamp():
    epoch = int(time.time())
    return epoch

def find_symbol(toolchain, mapfile, symbol):
    ret = None

    HEX = '0x0{,8}(?P<addr>[0-9A-Fa-f]{8})'
    if toolchain == "GCC_ARM":
        SYM = re.compile(r'^\s+' + HEX + r'\s+' + symbol + '\r?$')
    elif toolchain in ["ARM_STD", "ARM", "ARM_MICRO"]:
        SYM = re.compile(r'^\s+' + HEX + r'\s+0x[0-9A-Fa-f]{8}\s+Code.*\s+i\.' + symbol + r'\s+.*$')
    elif toolchain == "IAR":
        SYM = re.compile(r'^' + symbol + r'\s+' + HEX + '\s+.*$')

    with open(mapfile, 'r') as infile:
        for line in infile:
            match = re.match(SYM, line)
            if match:
                ret = match.group("addr")

    if not ret:
        print "[ERROR] cannot find the address of symbol " + symbol
        return 0

    return int(ret,16) | 1

def parse_load_segment_gcc(image_elf):
    # Program Headers:
    #   Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
    #   LOAD           0x000034 0x10006000 0x10006000 0x026bc 0x026bc RW  0x8
    #   LOAD           0x0026f0 0x30000000 0x30000000 0x06338 0x06338 RWE 0x4
    segment_list = []
    cmd = os.path.join(TOOLCHAIN_PATHS['GCC_ARM'], 'arm-none-eabi-readelf')
    cmd = '"' + cmd + '"' + ' -l ' + image_elf
    for line in subprocess.check_output(cmd, shell=True, universal_newlines=True).split("\n"):
        if not line.startswith("  LOAD"):
            continue
        segment = line.split()
        if len(segment) != 8:
            continue
        offset = int(segment[1][2:], 16)
        addr   = int(segment[2][2:], 16)
        size   = int(segment[4][2:], 16)
        if addr != 0 and size != 0:
            segment_list.append((offset, addr, size))
    return segment_list

def parse_load_segment_armcc(image_elf):
    # ====================================
    #
    # ** Program header #2
    #
    #     Type          : PT_LOAD (1)
    #     File Offset   : 52 (0x34)
    #     Virtual Addr  : 0x30000000
    #     Physical Addr : 0x30000000
    #     Size in file  : 27260 bytes (0x6a7c)
    #     Size in memory: 42168 bytes (0xa4b8)
    #     Flags         : PF_X + PF_W + PF_R + PF_ARM_ENTRY (0x80000007)
    #     Alignment     : 8
    #
    (offset, addr, size) = (0, 0, 0)
    segment_list = []
    in_segment = False
    cmd = os.path.join(TOOLCHAIN_PATHS['ARM'], 'bin', 'fromelf')
    cmd = '"' + cmd + '"' + ' --text -v --only=none ' + image_elf
    for line in subprocess.check_output(cmd, shell=True, universal_newlines=True).split("\n"):
        if line == "":
            pass
        elif line.startswith("** Program header"):
            in_segment = True
        elif in_segment == False:
            pass
        elif line.startswith("============"):
            if addr != 0 and size != 0:
                segment_list.append((offset, addr, size))
            in_segment = False
            (offset, addr, size) = (0, 0, 0)
        elif line.startswith("    Type"):
            if not re.match(r'\s+Type\s+:\s+PT_LOAD\s.*$', line):
                in_segment = False
        elif line.startswith("    File Offset"):
            match = re.match(r'^\s+File Offset\s+:\s+(?P<offset>\d+).*$', line)
            if match:
                offset = int(match.group("offset"))
        elif line.startswith("    Virtual Addr"):
            match = re.match(r'^\s+Virtual Addr\s+:\s+0x(?P<addr>[0-9a-f]+).*$', line)
            if match:
                addr = int(match.group("addr"), 16)
        elif line.startswith("    Size in file"):
            match = re.match(r'^\s+Size in file\s+:.*\(0x(?P<size>[0-9a-f]+)\).*$', line)
            if match:
                size = int(match.group("size"), 16)
    return segment_list


def parse_load_segment_iar(image_elf):
    #   SEGMENTS:
    #
    #      Type Offset    Virtual   Physical File Sz Mem Sz Flags   Align
    #      ---- ------    -------   -------- ------- ------ -----   -----
    #   0: load   0x34 0x10006000 0x10006000  0x26bc 0x26bc 0x6  WR   0x8
    #   1: load 0x26f0 0x30000000 0x30000000  0x6338 0x6338 0x7 XWR   0x4
    #
    #   SECTIONS:
    #
    #       Name            Type     Addr         Offset    Size Aln Lnk Inf  ESz Flags
    #       ----            ----     ----         ------    ---- --- --- ---  --- -----
    #    1: .shstrtab       strtab               0xfc4d8    0x60 0x4
    #    2: .strtab         strtab               0xfc538  0xbb3f 0x4

    segment_list = []
    in_segment = False
    cmd = os.path.join(TOOLCHAIN_PATHS['IAR'], 'bin', 'ielfdumparm')
    cmd = '"' + cmd + '"' + ' ' + image_elf
    for line in subprocess.check_output(cmd, shell=True, universal_newlines=True).split("\n"):
        if line.startswith("  SEGMENTS:"):
            in_segment = True
        elif in_segment == False:
            pass
        elif line.startswith("  SECTIONS:"):
            break
        elif re.match(r'^\s+\w+:\s+load\s+.*$', line):
            segment = line.split()
            offset = int(segment[2][2:], 16)
            addr   = int(segment[3][2:], 16)
            size   = int(segment[5][2:], 16)
            if addr != 0 and size != 0:
                segment_list.append((offset, addr, size))
    return segment_list

def parse_load_segment(toolchain, image_elf):
    if toolchain == "GCC_ARM":
        return parse_load_segment_gcc(image_elf)
    elif toolchain in ["ARM_STD", "ARM", "ARM_MICRO"]:
        return parse_load_segment_armcc(image_elf)
    elif toolchain == "IAR":
        return parse_load_segment_iar(image_elf)
    else:
        return []

def create_payload(image_elf, ram2_bin, entry, segment):
    file_elf = open(image_elf, "rb")
    file_bin = open(ram2_bin, "wb")

    write_number(int(entry), 8, file_bin)
    write_number(int(len(segment)), 8, file_bin)
    write_number(0xFFFFFFFF, 8, file_bin)
    write_number(0xFFFFFFFF, 8, file_bin)

    for (offset, addr, size) in segment:
        file_elf.seek(offset)
        # write image header - size & addr
        write_number(addr, 8, file_bin)
        write_number(size, 8, file_bin)
        # write load segment
        file_bin.write(file_elf.read(size))
        delta = size % 4
        if delta != 0:
            padding = 4 - delta
            write_number(0x0, padding * 2, file_bin)
    file_bin.close()
    file_elf.close()

def create_daplink(image_bin, ram1_bin, ram2_bin):

    # remove target binary file/path
    if os.path.isfile(image_bin):
        os.remove(image_bin)
    else:
        shutil.rmtree(image_bin)

    RAM2_HEADER['tag'] = format_number(TAG, 8)
    RAM2_HEADER['ver'] = format_number(VER, 8)
    RAM2_HEADER['timestamp'] = format_number(epoch_timestamp(), 16)
    RAM2_HEADER['size'] = format_number(os.stat(ram2_bin).st_size + 72, 8)
    RAM2_HEADER['hash'] = format_string(sha256_checksum(ram2_bin))
    RAM2_HEADER['campaign'] = format_string(CAMPAIGN)

    output = open(image_bin, "wb")
    append_image_file(ram1_bin, output)

    line = ""
    for key in ['tag', 'ver', 'timestamp', 'size', 'hash', 'campaign']:
        line += RAM2_HEADER[key]
        output.write(RAM2_HEADER[key])

    RAM2_HEADER['crc32'] = format_number(crc32_checksum(line), 8)

    output.write(RAM2_HEADER['crc32'])
    append_image_file(ram2_bin, output)
    output.close()

# ----------------------------
#       main function
# ----------------------------
def rtl8195a_elf2bin(t_self, image_elf, image_bin):

    image_name = os.path.splitext(image_elf)[0]
    image_map = image_name + '.map'

    ram1_bin = os.path.join(TOOLS_BOOTLOADERS, "REALTEK_RTL8195AM", "ram_1.bin")
    ram2_bin = image_name + '-payload.bin'

    entry = find_symbol(t_self.name, image_map, "PLAT_Start")
    segment = parse_load_segment(t_self.name, image_elf)

    create_payload(image_elf, ram2_bin, entry, segment)
    create_daplink(image_bin, ram1_bin, ram2_bin)
>>>>>>> upstream/master
