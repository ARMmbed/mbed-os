"""
Realtek Semiconductor Corp.

RTL8195A elf2bin script
"""

import sys, array, struct, os, re, subprocess
import hashlib
import shutil
import time
import binascii
import elftools

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

def _parse_load_segment_inner(image_elf):
    with open(image_elf, "rb") as fd:
        elffile = elftools.elf.elffile.ELFFile(fd)
        for segment in elffile.iter_segments():
            offset = segment['p_offset']
            addr = segment['p_vaddr']
            size = segment['p_filesz']
            if (addr != 0 and size != 0 and segment['p_type'] == 'PT_LOAD'):
                yield offset, addr, size

def parse_load_segment(toolchain, image_elf):
    return list(_parse_load_segment_inner(image_elf))

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
    append_image_file(ram2_bin, output)

    output.seek(0xb000)
    line = ""
    for key in ['tag', 'ver', 'timestamp', 'size', 'hash', 'campaign']:
        line += RAM2_HEADER[key]
        output.write(RAM2_HEADER[key])

    RAM2_HEADER['crc32'] = format_number(crc32_checksum(line), 8)

    output.write(RAM2_HEADER['crc32'])
    output.close()

# ----------------------------
#       main function
# ----------------------------
def rtl8195a_elf2bin(t_self, image_elf, image_bin):

    image_name = list(os.path.splitext(image_elf))[:-1]
    image_map = ".".join(image_name + ['map'])

    ram1_bin = os.path.join(TOOLS_BOOTLOADERS, "REALTEK_RTL8195AM", "ram_1.bin")
    ram2_bin = ".".join(image_name) + '-payload.bin'

    entry = find_symbol(t_self.name, image_map, "PLAT_Start")
    segment = parse_load_segment(t_self.name, image_elf)

    create_payload(image_elf, ram2_bin, entry, segment)
    create_daplink(image_bin, ram1_bin, ram2_bin)
