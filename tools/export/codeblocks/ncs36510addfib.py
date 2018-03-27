#!/usr/bin/env python

"""
@copyright (c) 2012-2018 ON Semiconductor. All rights reserved.
ON Semiconductor is supplying this software for use with ON Semiconductor
processor based microcontrollers only.
THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
"""

import sys
import itertools
import binascii
import intelhex

FIB_BASE = 0x2000
TRIM_BASE = 0x2800
FLASH_BASE = 0x3000
FLASHA_SIZE = 0x52000
FLASHB_BASE = 0x00102000
FLASHB_SIZE = 0x52000
FW_REV = 0x01000100

def ranges(i):
    for _, b in itertools.groupby(enumerate(i), lambda x_y: x_y[1] - x_y[0]):
        b = list(b)
        yield b[0][1], b[-1][1]

def add_fib(input_file, output_file):
    # Read in hex file
    input_hex_file = intelhex.IntelHex()
    input_hex_file.loadhex(input_file)
    #set padding value to be returned when reading from unspecified address
    input_hex_file.padding = 0xFF
    # Create new binary data array
    output_data = bytearray([0xff]*2048)

    # Get the starting and ending address
    addresses = input_hex_file.addresses()
    addresses.sort()
    start_end_pairs = list(ranges(addresses))
    regions = len(start_end_pairs)

    if regions == 1:
        #single range indicating fits within first flash block (<320K)
        start, end = start_end_pairs[0]
        print("Memory start 0x%08X, end 0x%08X" % (start, end))
        # Compute checksum over the range (don't include data at location of crc)
        size = end - start + 1
        data = input_hex_file.tobinarray(start=start, size=size)
        crc32 = binascii.crc32(data) & 0xFFFFFFFF
    else:
        #multiple ranges indicating requires both flash blocks (>320K)
        start, end = start_end_pairs[0]
        start2, end2 = start_end_pairs[1]
        print("Region 1: memory start 0x%08X, end 0x%08X" % (start, end))
        print("Region 2: memory start 0x%08X, end 0x%08X" % (start2, end2))
        # Compute checksum over the range (don't include data at location of crc)
        # replace end with end of flash block A
        end = FLASHA_SIZE - 1
        size = end - start + 1
        data = input_hex_file.tobinarray(start=start, size=size)

        # replace start2 with base of flash block B
        start2 = FLASHB_BASE
        size2 = end2 - start2 + 1
        data2 = input_hex_file.tobinarray(start=start2, size=size2)

        #concatenate data and data2 arrays together
        data.extend(data2)
        crc32 = binascii.crc32(data) & 0xFFFFFFFF

        #replace size with sum of two memory region sizes
        size = size + size2

    assert start >= FLASH_BASE, ("Error - start 0x%x less than begining of user\
    flash area" %start)

    assert regions <= 2, ("Error - more than 2 memory regions found")

    fw_rev = FW_REV

    checksum = (start + size + crc32 + fw_rev) & 0xFFFFFFFF

    print("Writing FIB: base 0x%08X, size 0x%08X, crc32 0x%08X, fw rev 0x%08X,\
    checksum 0x%08X" % (start, size, crc32, fw_rev, checksum))

#expected initial values used by daplink to validate that it is a valid bin
#file added as dummy values in this file because the fib area preceeds the
#application area the bootloader will ignore these dummy values
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
    #uint32_t base;     /**< Base offset of firmware, indicating what flash the
    #                        firmware is in. (will never be 0x11111111) */
    #uint32_t size;     /**< Size of the firmware */
    #uint32_t crc;      /**< CRC32 for firmware correctness check */
    #uint32_t rev;      /**< Revision number */
    #uint32_t checksum; /**< Check-sum of information block */
#}fib_t, *fib_pt;

    # Write FIB to the file in little endian
    output_data[0] = (dummy_sp >> 0) & 0xFF
    output_data[1] = (dummy_sp >> 8) & 0xFF
    output_data[2] = (dummy_sp >> 16) & 0xFF
    output_data[3] = (dummy_sp >> 24) & 0xFF

    output_data[4] = (dummy_reset_vector >> 0) & 0xFF
    output_data[5] = (dummy_reset_vector >> 8) & 0xFF
    output_data[6] = (dummy_reset_vector >> 16) & 0xFF
    output_data[7] = (dummy_reset_vector >> 24) & 0xFF

    output_data[8] = (dummy_nmi_handler >> 0) & 0xFF
    output_data[9] = (dummy_nmi_handler >> 8) & 0xFF
    output_data[10] = (dummy_nmi_handler >> 16) & 0xFF
    output_data[11] = (dummy_nmi_handler >> 24) & 0xFF

    output_data[12] = (dummy_hardfault_handler >> 0) & 0xFF
    output_data[13] = (dummy_hardfault_handler >> 8) & 0xFF
    output_data[14] = (dummy_hardfault_handler >> 16) & 0xFF
    output_data[15] = (dummy_hardfault_handler >> 24) & 0xFF

    output_data[16] = (dummy_blank >> 0) & 0xFF
    output_data[17] = (dummy_blank >> 8) & 0xFF
    output_data[18] = (dummy_blank >> 16) & 0xFF
    output_data[19] = (dummy_blank >> 24) & 0xFF

    # Write FIB to the file in little endian
    output_data[20] = (start >> 0) & 0xFF
    output_data[21] = (start >> 8) & 0xFF
    output_data[22] = (start >> 16) & 0xFF
    output_data[23] = (start >> 24) & 0xFF

    output_data[24] = (size >> 0) & 0xFF
    output_data[25] = (size >> 8) & 0xFF
    output_data[26] = (size >> 16) & 0xFF
    output_data[27] = (size >> 24) & 0xFF

    output_data[28] = (crc32 >> 0) & 0xFF
    output_data[29] = (crc32 >> 8) & 0xFF
    output_data[30] = (crc32 >> 16) & 0xFF
    output_data[31] = (crc32 >> 24) & 0xFF

    output_data[32] = (fw_rev >> 0) & 0xFF
    output_data[33] = (fw_rev >> 8) & 0xFF
    output_data[34] = (fw_rev >> 16) & 0xFF
    output_data[35] = (fw_rev >> 24) & 0xFF

    output_data[36] = (checksum >> 0) & 0xFF
    output_data[37] = (checksum >> 8) & 0xFF
    output_data[38] = (checksum >> 16) & 0xFF
    output_data[39] = (checksum >> 24) & 0xFF

    f = open(output_file, 'wb')
    f.write(output_data)
    f.close()

def main(argv):
    if len(argv) < 2:
        sys.stderr.write("usage: ncs36510addfib.py input.hex output.bin\n")
        sys.exit()
    add_fib(argv[0], argv[1])

if __name__ == "__main__":
    main(sys.argv[1:])
