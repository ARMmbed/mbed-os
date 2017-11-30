"""
Realtek Semiconductor Corp.

RTL8195A elf2bin script
"""

import sys, array, struct, os, re, subprocess
import hashlib
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
        sys.exit(-1)
    # cut string to list & reverse
    line = [line[i:i+2] for i in range(0, len(line), 2)]
    line.reverse()
    return binascii.a2b_hex("".join(line))

def format_string(string):
    return binascii.a2b_hex(string)

def write_number(value, width, output):
    output.write(format_number(value, width))

def write_string(value, width, output):
    output.write(format_string(value))

def append_image_file(image, output):
    input = open(image, "rb")
    output.write(input.read())
    input.close()

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
