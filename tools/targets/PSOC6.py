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
from distutils.spawn import find_executable
from shutil import copyfile
from intelhex import IntelHex
from intelhex.compat import asbytes

from ..config import ConfigException


class HookError(Exception):
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return self.msg


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
        segment = ihex.tobinarray(start = start, end = end)
        checksum += sum(segment)

    lowchecksum = checksum & 0x0FFFF
    message_func("Calculated checksum for %s is 0x%04x" % (hexf, checksum))

    # update checksum
    checksum_bytes = array('B', asbytes('\0'*2))
    checksum_bytes[0] = lowchecksum >> 8
    checksum_bytes[1] = lowchecksum & 0xFF
    ihex.frombytes(checksum_bytes, offset=0x90300000)

    # update metadata
    sig_bytes = ihex.tobinarray(start=0x90500002, size=4)
    signature = (sig_bytes[0] << 24) | (sig_bytes[1]  << 16) | (sig_bytes[2] << 8) | sig_bytes[3]
    sigcheck = checksum + signature
    sigcheck_bytes = array('B', asbytes('\0'*4))
    sigcheck_bytes[0] = (sigcheck >> 24) & 0xff
    sigcheck_bytes[1] = (sigcheck >> 16) & 0xff
    sigcheck_bytes[2] = (sigcheck >> 8) & 0xff
    sigcheck_bytes[3] = sigcheck & 0xFF
    ihex.frombytes(sigcheck_bytes, offset=0x90500008)

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

def check_matching_features(image_features, target_features):
    for feature in image_features:
        if not (feature in target_features):
            return False
    return True

# Find Cortex M0 boot image proper for the application image.
def find_cm0_images(toolchain, resources, elf, hexf):
    # Scan to find the actual paths of m0 image
    # First check if user-compiled image exists.
    params, _ = toolchain.config.get_config_data()
    dual_core_enabled = params['target.sub-target-build-enable'].value
    if dual_core_enabled:
        if "BLE" in toolchain.target.features:
            raise ConfigException("Feature 'BLE' not compatible with dual core configuration.")
        m0target = toolchain.target.sub_target
        m4target = toolchain.target.name
        m0hexf = hexf.replace(m4target, m0target)
        if not os.path.isfile(m0hexf):
            raise ConfigException("Matching M0 core hex image not found.")
    else:
        # Try default image.
        m0hexf = None
        m0_images = toolchain.target.M0_CORE_IMAGE
        # convert into a list if needed
        if not isinstance(m0_images, list):
            m0_images = [m0_images]

        # find an image with matching features
        try:
            target_features = toolchain.target.features
        except AttributeError:
            target_features = []
            pass

        for image in m0_images:
            features = image['features']
            if check_matching_features(image['features'], target_features):
                for hexf in resources.hex_files:
                    if hexf.find(image['name']) != -1:
                        m0hexf = hexf
                        break
            if m0hexf:
                break

    if m0hexf:
        toolchain.notify.info("M0 core image file found %s." % os.path.basename(m0hexf))
    else:
        toolchain.notify.debug("M0 core hex image file not found. Aborting.")
        raise ConfigException("Matching M0 core hex image not found.")

    m0elf = m0hexf.replace(".hex", ".elf")
    return m0elf, m0hexf


def complete(toolchain, elf0, hexf0, hexf1=None):
    complete_func(toolchain.notify.debug, elf0, hexf0, hexf1)


