#!/usr/bin/env python

# CMSIS-DAP Interface Firmware
# Copyright (c) 2009-2013 ARM Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
from __future__ import absolute_import
from __future__ import print_function

import argparse
import itertools
import binascii
import intelhex
import os

FIB_BASE = 0x2000
FLASH_BASE = 0x3000
FW_REV = 0x01000100  #todo: determine if revision can be passed from yotta

def ranges(i):
    for _, b in itertools.groupby(enumerate(i), lambda x_y: x_y[1] - x_y[0]):
        b = list(b)
        yield b[0][1], b[-1][1]


def add_fib_at_start(arginput):
    #parser = argparse.ArgumentParser(description='Firmware Information Block generation script')
    #parser.add_argument("input", type=str, help="bin file to read from.")

    #args = parser.parse_args()
    #input_file = args.input + "_orig.bin"
    #output_file = args.input #use same name, does not include extension
    #file_name_hex = args.input + ".hex"
    #file_name_bin = args.input + ".bin"

    print("inputfile", arginput)
    input_file = arginput + ".bin"
    output_file = arginput #use same name, does not include extension
    file_name_hex = arginput + "_fib.hex"
    file_name_bin = arginput + ".bin"
    print("inputfile", input_file)
    print("output_file", output_file)
    print("file_name_hex", file_name_hex)
    print("file_name_bin", file_name_bin)


    # Import intelhex if avaialable, otherwise fail
    try:
        from intelhex import IntelHex
    except:
        return fail('error: You do not have \'intelhex\' installed. Please run \'pip install intelhex\' then retry.')

    # Read in hex file
    input_hex_file = intelhex.IntelHex()
    input_hex_file.padding = 0x00
    input_hex_file.loadbin(input_file, offset=FLASH_BASE)

    output_hex_file = intelhex.IntelHex()
    output_hex_file.padding = 0x00

    # Get the starting and ending address
    addresses = input_hex_file.addresses()
    addresses.sort()
    start_end_pairs = list(ranges(addresses))
    regions = len(start_end_pairs)

    if regions == 1:
        start, end = start_end_pairs[0]
    else:
        start = min(min(start_end_pairs))
        end = max(max(start_end_pairs))

    assert start >= FLASH_BASE, ("Error - start 0x%x less than begining of user flash area" %start)
    # Compute checksum over the range (don't include data at location of crc)
    size = end - start + 1
    data = input_hex_file.tobinarray(start=start, size=size)
    crc32 = binascii.crc32(data) & 0xFFFFFFFF

    fw_rev = FW_REV

    checksum = (start + size + crc32 + fw_rev) & 0xFFFFFFFF

    print("Writing FIB: base 0x%08X, size 0x%08X, crc32 0x%08X, fw rev 0x%08X, checksum 0x%08X" % (start, size, crc32, fw_rev, checksum))

#expected initial values used by daplink to validate that it is a valid bin file
#added as dummy values in this file because the fib area preceeds the application area
#the bootloader will ignore these dummy values
#  00 is stack pointer (RAM address)
#  04 is Reset vector  (FLASH address)
#  08 NMI_Handler      (FLASH address)
#  0C HardFault_Handler(FLASH address)
#  10 dummy
    dummy_sp = 0x3FFFFC00
    dummy_reset_vector = 0x00003625
    dummy_nmi_handler = 0x00003761
    dummy_hardfault_handler = 0x00003691
    dummy_blank = 0x00000000

#expected fib structure
#typedef struct fib{
	#uint32_t base;		/**< Base offset of firmware, indicating what flash the firmware is in. (will never be 0x11111111) */
	#uint32_t size;		/**< Size of the firmware */
	#uint32_t crc;		/**< CRC32 for firmware correctness check */
	#uint32_t rev;		/**< Revision number */
	#uint32_t checksum;	/**< Check-sum of information block */
#}fib_t, *fib_pt;

    fib_start = FIB_BASE
    dummy_fib_size = 20
    fib_size = 20
    user_code_start = FLASH_BASE

    # Write FIB to the file in little endian
    output_hex_file[fib_start + 0] = (dummy_sp >> 0) & 0xFF
    output_hex_file[fib_start + 1] = (dummy_sp >> 8) & 0xFF
    output_hex_file[fib_start + 2] = (dummy_sp >> 16) & 0xFF
    output_hex_file[fib_start + 3] = (dummy_sp >> 24) & 0xFF

    output_hex_file[fib_start + 4] = (dummy_reset_vector >> 0) & 0xFF
    output_hex_file[fib_start + 5] = (dummy_reset_vector >> 8) & 0xFF
    output_hex_file[fib_start + 6] = (dummy_reset_vector >> 16) & 0xFF
    output_hex_file[fib_start + 7] = (dummy_reset_vector >> 24) & 0xFF

    output_hex_file[fib_start + 8] = (dummy_nmi_handler >> 0) & 0xFF
    output_hex_file[fib_start + 9] = (dummy_nmi_handler >> 8) & 0xFF
    output_hex_file[fib_start + 10] = (dummy_nmi_handler >> 16) & 0xFF
    output_hex_file[fib_start + 11] = (dummy_nmi_handler >> 24) & 0xFF

    output_hex_file[fib_start + 12] = (dummy_hardfault_handler >> 0) & 0xFF
    output_hex_file[fib_start + 13] = (dummy_hardfault_handler >> 8) & 0xFF
    output_hex_file[fib_start + 14] = (dummy_hardfault_handler >> 16) & 0xFF
    output_hex_file[fib_start + 15] = (dummy_hardfault_handler >> 24) & 0xFF

    output_hex_file[fib_start + 16] = (dummy_blank >> 0) & 0xFF
    output_hex_file[fib_start + 17] = (dummy_blank >> 8) & 0xFF
    output_hex_file[fib_start + 18] = (dummy_blank >> 16) & 0xFF
    output_hex_file[fib_start + 19] = (dummy_blank >> 24) & 0xFF

    # Write FIB to the file in little endian
    output_hex_file[fib_start + 20] = (start >> 0) & 0xFF
    output_hex_file[fib_start + 21] = (start >> 8) & 0xFF
    output_hex_file[fib_start + 22] = (start >> 16) & 0xFF
    output_hex_file[fib_start + 23] = (start >> 24) & 0xFF

    output_hex_file[fib_start + 24] = (size >> 0) & 0xFF
    output_hex_file[fib_start + 25] = (size >> 8) & 0xFF
    output_hex_file[fib_start + 26] = (size >> 16) & 0xFF
    output_hex_file[fib_start + 27] = (size >> 24) & 0xFF

    output_hex_file[fib_start + 28] = (crc32 >> 0) & 0xFF
    output_hex_file[fib_start + 29] = (crc32 >> 8) & 0xFF
    output_hex_file[fib_start + 30] = (crc32 >> 16) & 0xFF
    output_hex_file[fib_start + 31] = (crc32 >> 24) & 0xFF

    output_hex_file[fib_start + 32] = (fw_rev >> 0) & 0xFF
    output_hex_file[fib_start + 33] = (fw_rev >> 8) & 0xFF
    output_hex_file[fib_start + 34] = (fw_rev >> 16) & 0xFF
    output_hex_file[fib_start + 35] = (fw_rev >> 24) & 0xFF

    output_hex_file[fib_start + 36] = (checksum >> 0) & 0xFF
    output_hex_file[fib_start + 37] = (checksum >> 8) & 0xFF
    output_hex_file[fib_start + 38] = (checksum >> 16) & 0xFF
    output_hex_file[fib_start + 39] = (checksum >> 24) & 0xFF

    #pad the rest of the file
    for i in range(fib_start + dummy_fib_size + fib_size, user_code_start):
        output_hex_file[i] =  0xFF

    #merge two hex files
    output_hex_file.merge(input_hex_file, overlap='error')

    # Write out file(s)
    output_hex_file.tofile(file_name_hex, 'hex')
    output_hex_file.tofile(file_name_bin, 'bin')
