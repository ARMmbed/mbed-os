#
# Copyright (c) 2017-2018 Future Electronics
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
from distutils.spawn import find_executable
from shutil import copyfile
from intelhex import IntelHex
from intelhex.compat import asbytes

from ..config import ConfigException
from ..targets import HookError


# Patch Cypress hex file:
# - update checksum
# - update metadata
# - align regions to page (256 bytes) boundary
def patch(message_func, ihex, hexf, align=256):
    #calculate checksum
    checksum = 0
    for start, end in ihex.segments():
        if start  >= 0x090000000:
            continue
        segment = ihex.tobinarray(start, end)
        checksum += sum(segment)

    lowchecksum = checksum & 0x0FFFF
    message_func("Calculated checksum for %s is 0x%04x" % (hexf, lowchecksum))

    # update checksum
    checksum_str = pack('>H', lowchecksum)
    ihex.frombytes(array('B', checksum_str), offset=0x90300000)

    # update metadata
    signature = unpack('>L', ihex.tobinstr(start=0x90500002, size=4))[0]
    sigcheck = pack('>L', (checksum + signature) & 0x0FFFF)
    ihex.frombytes(array('B',sigcheck), offset=0x90500008)

    # align flash segments
    align_mask = align - 1
    alignments = IntelHex()
    for start, end in ihex.segments():
        if start >= 0x090000000:
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
    ihex.merge(alignments)

def merge_images(hexf0, hexf1=None):
    ihex = IntelHex()
    ihex.padding = 0x00
    ihex.loadfile(hexf0, "hex")
    if  hexf1 is not None:
        # get chip ID from metadata and compare
        ihex1 = IntelHex(hexf1)
        type0 = ihex.tobinarray(start=0x90500002, size=4)
        type1 = ihex1.tobinarray(start=0x90500002, size=4)
        if type0 != type1:
            raise HookError(
                "Incompatible processor type: %s in '%s' and 0x%s in '%s'"
                 % (hexf0, type0, hexf1, type1))
        ihex.merge(ihex1, 'ignore')
    return ihex

def complete_func(message_func, elf0, hexf0, hexf1=None, dest=None):
    message_func("Postprocessing %s -> %s" % (elf0, hexf0))
    ihex = merge_images(hexf0, hexf1)
    patch(message_func, ihex, hexf0)
    ihex.write_hex_file(dest if dest else hexf0, write_start_addr=False, byte_count=64)

# Find Cortex M0 image.
def find_cm0_image(toolchain, resources, elf, hexf):
    # Locate user-specified image
    from tools.resources import FileType
    hex_files = resources.get_file_paths(FileType.HEX)
    m0hexf = next((f for f in hex_files if os.path.basename(f) == toolchain.target.m0_core_img), None)

    if m0hexf:
        toolchain.notify.debug("M0 core image file found %s." % os.path.basename(m0hexf))
    else:
        toolchain.notify.debug("M0 core hex image file %s not found. Aborting." % toolchain.target.m0_core_img)
        raise ConfigException("Required M0 core hex image not found.")

    return m0hexf


def complete(toolchain, elf0, hexf0, hexf1=None):
    complete_func(toolchain.notify.debug, elf0, hexf0, hexf1)


