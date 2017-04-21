"""
@copyright (c) 2012 ON Semiconductor. All rights reserved.
ON Semiconductor is supplying this software for use with ON Semiconductor
processor based microcontrollers only.
THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
"""

from __future__ import absolute_import
from __future__ import print_function

import itertools
import binascii
import intelhex
from tools.config import Config

FIB_BASE = 0x2000
FLASH_BASE = 0x3000
FW_REV = 0x01000100
TRIM_BASE = 0x2800

def ranges(i):
    for _, b in itertools.groupby(enumerate(i), lambda x_y: x_y[1] - x_y[0]):
        b = list(b)
        yield b[0][1], b[-1][1]


def add_fib_at_start(arginput):
    input_file = arginput + ".bin"
    file_name_hex = arginput + "_fib.hex"
    file_name_bin = arginput + ".bin"

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

    assert start >= FLASH_BASE, ("Error - start 0x%x less than begining of user\
    flash area" %start)
    # Compute checksum over the range (don't include data at location of crc)
    size = end - start + 1
    data = input_hex_file.tobinarray(start=start, size=size)
    crc32 = binascii.crc32(data) & 0xFFFFFFFF

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

    fib_start = FIB_BASE
    dummy_fib_size = 20
    fib_size = 20
    trim_size = 24
    user_code_start = FLASH_BASE
    trim_area_start = TRIM_BASE

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
    for i in range(fib_start + dummy_fib_size + fib_size, trim_area_start):
        output_hex_file[i] = 0xFF

    # Read in configuration data from the config parameter in targets.json
    configData = Config('NCS36510')
    paramData = configData.get_target_config_data()
    for v in paramData.values():
        if (v.name == "target.mac-addr-high"):
            mac_addr_high = int(v.value, 16)
        elif (v.name == "target.mac-addr-low"):
            mac_addr_low = int(v.value,16)
        elif (v.name == "target.32KHz-clk-trim"):
            clk_32k_trim = int(v.value,16)
        elif (v.name == "target.32MHz-clk-trim"):
            clk_32m_trim = int(v.value,16)
        elif (v.name == "target.rssi-trim"):
            rssi = int(v.value,16)
        elif (v.name == "target.txtune-trim"):
            txtune = int(v.value,16)
        else:
            print("Not a valid param")

    output_hex_file[trim_area_start + 0] = mac_addr_low & 0xFF
    output_hex_file[trim_area_start + 1] = (mac_addr_low >> 8)  & 0xFF
    output_hex_file[trim_area_start + 2] = (mac_addr_low >> 16) & 0xFF
    output_hex_file[trim_area_start + 3] = (mac_addr_low >> 24) & 0xFF
    
    output_hex_file[trim_area_start + 4] = mac_addr_high & 0xFF
    output_hex_file[trim_area_start + 5] = (mac_addr_high >> 8)  & 0xFF
    output_hex_file[trim_area_start + 6] = (mac_addr_high >> 16) & 0xFF
    output_hex_file[trim_area_start + 7] = (mac_addr_high >> 24) & 0xFF

    output_hex_file[trim_area_start + 8] = clk_32k_trim & 0xFF
    output_hex_file[trim_area_start + 9] = (clk_32k_trim >> 8)  & 0xFF
    output_hex_file[trim_area_start + 10] = (clk_32k_trim >> 16) & 0xFF
    output_hex_file[trim_area_start + 11] = (clk_32k_trim >> 24) & 0xFF

    output_hex_file[trim_area_start + 12] = clk_32m_trim & 0xFF
    output_hex_file[trim_area_start + 13] = (clk_32m_trim >> 8)  & 0xFF
    output_hex_file[trim_area_start + 14] = (clk_32m_trim >> 16) & 0xFF
    output_hex_file[trim_area_start + 15] = (clk_32m_trim >> 24) & 0xFF

    output_hex_file[trim_area_start + 16] = rssi & 0xFF
    output_hex_file[trim_area_start + 17] = (rssi >> 8)  & 0xFF
    output_hex_file[trim_area_start + 18] = (rssi >> 16) & 0xFF
    output_hex_file[trim_area_start + 19] = (rssi >> 24) & 0xFF

    output_hex_file[trim_area_start + 20] = txtune & 0xFF
    output_hex_file[trim_area_start + 21] = (txtune >> 8)  & 0xFF
    output_hex_file[trim_area_start + 22] = (txtune >> 16) & 0xFF
    output_hex_file[trim_area_start + 23] = (txtune >> 24) & 0xFF
    
    # pad the rest of the area with 0xFF
    for i in range(trim_area_start + trim_size, user_code_start):
        output_hex_file[i] = 0xFF

    #merge two hex files
    output_hex_file.merge(input_hex_file, overlap='error')

    # Write out file(s)
    output_hex_file.tofile(file_name_hex, 'hex')
    output_hex_file.tofile(file_name_bin, 'bin')
