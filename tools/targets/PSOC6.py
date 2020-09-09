#
# Copyright (c) 2017-2018 Future Electronics
# Copyright (c) 2018-2019 Cypress Semiconductor Corporation
# Copyright (c) 2020 Arm Limited
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
import sys
import subprocess
from array import array
from struct import (pack, unpack)
from shutil import copy2
import json
from intelhex import IntelHex, hex2bin, bin2hex
from pathlib import Path

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

# Secure Boot defines
MCUBOOT_HEADER_SIZE = 1024
SPE_IMAGE_ID = 1
NSPE_IMAGE_ID = 16
SMIF_MEM_MAP_START = 0x18000000

class AddSignatureError(Exception):
    """ A simple class that represents all the exceptions associated with
    adding signature to Secure Boot image
    """

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
        if start >= CY_PROGRAM_SIZE:
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
        ihex.frombytes(array('B', sigcheck), offset=CY_META_CHECKSUM_ADDR)

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
    if hexf1 is not None:
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
    if hex_filename is None:
        return None
    # Locate user-specified image
    from tools.resources import FileType
    hex_files = resources.get_file_paths(FileType.HEX)
    m0hexf = next((f for f in hex_files if os.path.basename(f) == hex_filename), None)

    if m0hexf:
        toolchain.notify.info("M0 core image file found: %s." % m0hexf)
    else:
        toolchain.notify.info("M0 core hex image file %s not found. Aborting." % hex_filename)
        raise ConfigException("Required M0 core hex image not found.")

    return m0hexf


def sign_image(toolchain, resourses, elf, binf, m0hex):
    """
    Adds signature to a binary file being built,
    using cysecuretools python package.
    :param toolchain: Toolchain object of current build session
    :param binf: Binary file created for target
    """

    if m0hex != '':
        m0hex_build = os.path.join(toolchain.build_dir, toolchain.target.hex_filename)
        copy2(m0hex, m0hex_build)
        m0hex = m0hex_build

    # Mapping from mbed target to cysecuretools target
    TARGET_MAPPING = {
            "CY8CKIT064B0S2_4343W": "cy8ckit-064b0s2-4343w",
            "CYTFM_064B0S2_4343W"  : "cy8ckit-064b0s2-4343w",
            "CY8CPROTO_064B0S1_BLE": "cy8cproto-064b0s1-ble",
            "CY8CPROTO_064S1_SB"   : "cy8cproto-064s1-sb",
            "CY8CPROTO_064B0S3"    : "cy8cproto-064b0s3"
    }

    try:
        secure_target = TARGET_MAPPING[toolchain.target.name]
    except KeyError:
        raise ConfigException("[PSOC6.sign_image] Target " + toolchain.target.name + " is not supported in cysecuretools.")

    policy_file = find_policy(toolchain)

    toolchain.notify.info("[PSOC6.sign_image] Using policy file: " + str(policy_file))

    import cysecuretools

    tools = cysecuretools.CySecureTools(secure_target, str(policy_file))

    if str(toolchain.target.boot_scheme) == 'single_image':
        toolchain.notify.info("[PSOC6.sign_image] single image signing")
        sign_application(toolchain, tools, binf, image_id=toolchain.target.cm0_img_id)

    elif str(toolchain.target.boot_scheme) == 'multi_image':
        sign_application(toolchain, tools, m0hex, image_id=toolchain.target.cm0_img_id)
        sign_application(toolchain, tools, binf, image_id=toolchain.target.cm4_img_id)

        complete(toolchain, elf, hexf0=binf, hexf1=m0hex)

    else:
        raise ConfigException("[PSOC6.sign_image] Boot scheme " + str(toolchain.target.boot_scheme) + \
            "is not supported. Supported boot schemes are 'single_image' and 'multi_image' ")


def sign_application(toolchain, tools, binary, image_id):
    """
    Helper function for adding signature to binary
    :param tools: CySecureTools object
    :param binary: Path to binary file to add signature
    :param image_id: ID of image slot in which binary will be flashed
    """
    
    # Get address and size of image slot from policy for passed image_id
    # UPGRADE image will be generated automatically by cysecuretools
    address, size = tools.flash_map(image_id=image_id, image_type="BOOT")

    tools.sign_image(binary, image_id)
    toolchain.notify.debug("[PSOC6.sign_image] Slot start address and size for image ID " \
                                + str(image_id) + " is " + hex(address) + ", " + hex(size))


def find_policy(toolchain):
    """
    Locate path to policy file, by name defined in targets.json
    :param toolchain: toolchain object from mbed build system
    """

    mbed_os_root = Path(os.getcwd())
    
    policy_path = Path(toolchain.target.policy_file)

    # Absolute path provided
    if policy_path.is_absolute():
        policy_file = policy_path

    # May also be relative to mbed-os file scturcture
    else:
        policy_path = mbed_os_root / policy_path

        if os.path.exists(str(policy_path)):
            policy_file = policy_path

        else:
            default_path = Path("targets/TARGET_Cypress/TARGET_PSOC6/") / \
                                Path("TARGET_" + toolchain.target.name) / Path("policy") / \
                                toolchain.target.policy_file

            # Consider default location
            policy_file = mbed_os_root / default_path
            
            if not os.path.exists(str(policy_file)):
                policy_file = mbed_os_root / "mbed-os" / default_path


    if os.path.exists(str(policy_file)):
        toolchain.notify.info("Policy file found: %s." % policy_file)
    else:
        toolchain.notify.info("Policy file %s not found. Aborting." % policy_path)
        raise ConfigException("Required policy file not found.")

    return policy_file



def complete(toolchain, elf0, hexf0, hexf1=None):
    """
    Merge CM4 and CM0 images to a single binary
    """
    complete_func(toolchain.notify.debug, elf0, hexf0, hexf1)
