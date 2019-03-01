#
# Copyright (c) 2017-2018 Future Electronics
# Copyright (c) 2018-2019 Cypress Semiconductor Corporation
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

import os
import platform
import subprocess
import errno
from array import array
from struct import (pack, unpack)
from shutil import copyfile
from intelhex import IntelHex
from intelhex.compat import asbytes

from ..config import ConfigException

# The size of the program data in Cypress HEX files is limited to 0x80000000
# Higher addresses contain additional metadata (chip protection, eFuse data, etc..)
CY_PROGRAM_SIZE = 0x80000000

# The starting address of the program data checksum section
CY_CHECKSUM_ADDR = 0x90300000

# The starting address of the .cymeta section (12 bytes)
# Additional metadata include silicon revision, Silicon/JTAG ID, etc.
CY_META_ADDR = 0x90500000

# The address of the silicon ID (4 bytes)
CY_META_SILICON_ID_ADDR = 0x90500002

# The address of the metadata checksum (4 bytes)
CY_META_CHECKSUM_ADDR = 0x90500008


# Patch Cypress hex file:
# - update checksum
# - update metadata
# - align regions to page (256 bytes) boundary
def patch(message_func, ihex, hexf, align=256):
    update_checksum = False
    update_metadata = False

    # calculate checksum of the program section, detect metadata
    checksum = 0
    for start, end in ihex.segments():
        if start == CY_CHECKSUM_ADDR:
            # checksum section found in the original hex
            update_checksum = True
        if start == CY_META_ADDR:
            # metadata section found in the original hex
            update_metadata = True
        if start  >= CY_PROGRAM_SIZE:
            continue
        segment = ihex.tobinarray(start, end)
        checksum += sum(segment)

    # only update checksum if it was found in the original hex
    if update_checksum:
        lowchecksum = checksum & 0x0FFFF
        message_func("Calculated checksum for %s is 0x%04x" % (hexf, lowchecksum))

        checksum_str = pack('>H', lowchecksum)
        ihex.frombytes(array('B', checksum_str), offset=CY_CHECKSUM_ADDR)

    # only update metadata if it was found in the original hex
    if update_metadata:
        signature = unpack('>L', ihex.tobinstr(start=CY_META_SILICON_ID_ADDR, size=4))[0]
        sigcheck = pack('>L', (checksum + signature) & 0x0FFFF)
        ihex.frombytes(array('B',sigcheck), offset=CY_META_CHECKSUM_ADDR)

    # align flash segments
    align_mask = align - 1
    alignments = IntelHex()
    for start, end in ihex.segments():
        if start >= CY_PROGRAM_SIZE:
            continue
        aligned_start = start & ~align_mask
        if start != aligned_start:
            message_func("Aligning start from 0x%x to 0x%x" % (start, aligned_start))
            alignments.frombytes(ihex.tobinarray(aligned_start, start - 1), aligned_start)
        aligned_end = end & ~align_mask
        if end != aligned_end:
            aligned_end += align
            message_func("Aligning end from 0x%x to 0x%x" % (end, aligned_end))
            alignments.frombytes(ihex.tobinarray(end, aligned_end - 1), end)
    ihex.merge(alignments, 'ignore')

def merge_images(hexf0, hexf1=None):
    ihex = IntelHex()
    ihex.padding = 0x00
    ihex.loadfile(hexf0, "hex")
    if  hexf1 is not None:
        # Merge the CM0+ image
        ihex1 = IntelHex(hexf1)
        ihex.merge(ihex1, 'ignore')
    return ihex

def complete_func(message_func, elf0, hexf0, hexf1=None, dest=None):
    message_func("Postprocessing %s -> %s" % (elf0, hexf0))
    ihex = merge_images(hexf0, hexf1)
    patch(message_func, ihex, hexf0)
    ihex.write_hex_file(dest if dest else hexf0, write_start_addr=False, byte_count=16)

# Find Cortex M0 image.
def find_cm0_image(toolchain, resources, elf, hexf, hex_filename):
    # Locate user-specified image
    from tools.resources import FileType
    hex_files = resources.get_file_paths(FileType.HEX)
    m0hexf = next((f for f in hex_files if os.path.basename(f) == hex_filename), None)
    if toolchain.target.is_PSA_non_secure_target:
        m0hexf = next((f for f in hex_files if os.path.basename(f) == os.path.basename(hexf)), m0hexf)

    if m0hexf:
        toolchain.notify.info("M0 core image file found: %s." % m0hexf)
    else:
        toolchain.notify.info("M0 core hex image file %s not found. Aborting." % hex_filename)
        raise ConfigException("Required M0 core hex image not found.")

    return m0hexf

def complete(toolchain, elf0, hexf0, hexf1=None):
    complete_func(toolchain.notify.debug, elf0, hexf0, hexf1)
