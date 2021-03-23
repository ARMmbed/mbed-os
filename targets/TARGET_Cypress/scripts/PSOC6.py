#
# Copyright (c) 2017-2018 Future Electronics
# Copyright (c) 2018-2019 Cypress Semiconductor Corporation
# Copyright (c) 2020-2021 Arm Limited
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

import argparse
import pathlib
import logging
import os
import sys
import subprocess
from array import array
from struct import (pack, unpack)
from shutil import copy2
import json
from intelhex import IntelHex, hex2bin, bin2hex
from pathlib import Path

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MBED_OS_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, os.pardir, os.pardir, os.pardir))

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


class ArtefactsError(Exception):
    """An exception to indicate that the artefact(s) needed for processing 
    ave not been found."""


# Base class for all configuration exceptions
class ConfigException(Exception):
    """Config system only exception. Makes it easier to distinguish config
    errors"""


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
    sign_hex(
        toolchain.build_dir,
        toolchain.target.hex_filename,
        toolchain.target.name,
        toolchain.target.policy_file,
        toolchain.notify,
        toolchain.target.boot_scheme,
        toolchain.target.cm0_img_id,
        toolchain.target.cm4_img_id,
        elf,
        binf,
        m0hexf
    )

def sign_hex(
    build_dir, m0hex_filename, target_name, policy, notification, boot_scheme,
    cm0_img_id, cm4_img_id, elf, m4hex, m0hex
):
    """
    Adds signature to a binary file being built,
    using cysecuretools python package.
    :param build_dir: The build directory
    :param m0hex_filename: The file name of the Cortex-M0 hex
    :param target_name: The name of the Mbed target
    :param policy: The path to the policy file
    :param notification: The object to output notification with
    :param boot_scheme: The boot scheme
    :param cm0_img_id: The Cortex-M0 image identifier
    :param cm4_img_id: The Cortex-M4 image identifier
    :param elf: An ELF file
    :param m4hex: The path to the Cortex-M4 HEX file
    :param m0hex: The path to the Cortex-M0 HEX file
    """
    # Keep module import here so it is required only if building PSOC6 targets
    # that need to be signed

    if m0hex != '':
        m0hex_build = os.path.join(build_dir, m0hex_filename)
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
        secure_target = TARGET_MAPPING[target_name]
    except KeyError:
        raise ConfigException("[PSOC6.sign_image] Target " + target_name + " is not supported in cysecuretools.")

    policy_file = find_policy(
        policy,
        notification.info,
        target_name
    )

    notification.info("[PSOC6.sign_image] Using policy file: " + str(policy_file))

    import cysecuretools
    tools = cysecuretools.CySecureTools(secure_target, str(policy_file))

    if str(boot_scheme) == 'single_image':
        notification.info("[PSOC6.sign_image] single image signing")
        sign_application(notification.debug, tools, m4hex, image_id=cm0_img_id)

    elif str(boot_scheme) == 'multi_image':
        sign_application(notification.debug, tools, m0hex, image_id=cm0_img_id)
        sign_application(notification.debug, tools, m4hex, image_id=cm4_img_id)

        complete(notification.debug, elf, hexf0=m4hex, hexf1=m0hex)

    else:
        raise ConfigException("[PSOC6.sign_image] Boot scheme " + str(boot_scheme) + \
            "is not supported. Supported boot schemes are 'single_image' and 'multi_image' ")


def sign_application(message_func, tools, binary, image_id):
    """
    Helper function for adding signature to binary
    :param message_func: Function to print a status information
    :param tools: CySecureTools object
    :param binary: Path to binary file to add signature
    :param image_id: ID of image slot in which binary will be flashed
    """
    
    # Get address and size of image slot from policy for passed image_id
    # UPGRADE image will be generated automatically by cysecuretools
    address, size = tools.flash_map(image_id=image_id, image_type="BOOT")

    if not address or not size:
        raise AddSignatureError(
            f"Cannot find image with id {image_id} and type BOOT in the policy file"
        )

    tools.sign_image(binary, image_id)
    message_func("[PSOC6.sign_image] Slot start address and size for image ID " \
                                + str(image_id) + " is " + hex(address) + ", " + hex(size))


def find_policy(policy, message_func, target_name=None):
    """
    Locate path to policy file, by name defined in targets.json
    :param policy: Path to the policy file
    :param message_func: Function to print a status information
    :param target_name: Name of the Mbed target
    """
    mbed_os_root = Path(MBED_OS_ROOT)

    policy_path = Path(policy)

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
                                Path("TARGET_" + target_name) / Path("policy") / \
                                policy

            # Consider default location
            policy_file = mbed_os_root / default_path
            
            if not os.path.exists(str(policy_file)):
                policy_file = mbed_os_root / "mbed-os" / default_path


    if os.path.exists(str(policy_file)):
        message_func("Policy file found: %s." % policy_file)
    else:
        message_func("Policy file %s not found. Aborting." % policy_path)
        raise ConfigException("Required policy file not found.")

    return policy_file


def complete(message_func, elf0, hexf0, hexf1=None):
    """
    Merge CM4 and CM0 images to a single binary
    """
    complete_func(message_func, elf0, hexf0, hexf1)


def merge_action(args):
    """Entry point for the "merge" CLI command."""
    complete_func(
        print, args.elf, args.m4hex, args.m0hex
    )


def sign_action(args):
    """Entry point for the "sign" CLI command."""
    sign_hex(
        args.build_dir,
        args.m0hex_filename,
        args.target_name,
        args.policy_file_name,
        logging.getLogger(__name__),
        args.boot_scheme,
        args.cm0_img_id,
        args.cm4_img_id,
        args.elf,
        args.m4hex,
        args.m0hex
    )


def parse_args():
    """Parse the command line arguments."""
    parser = argparse.ArgumentParser(
        description="PSOC6 post build application."
    )

    subcommands = parser.add_subparsers(description="The action to perform.")

    merge_subcommand = subcommands.add_parser(
        "merge", help="Merge Cortex-M4 and Cortex-M0 HEX files."
    )
    merge_subcommand.add_argument(
        "--elf", required=True, help="the application ELF file."
    )
    merge_subcommand.add_argument(
        "--m4hex", required=True, help="the path to the Cortex-M4 HEX to merge."
    )
    merge_subcommand.add_argument(
        "--m0hex", help="the path to the Cortex-M0 HEX to merge."
    )
    merge_subcommand.set_defaults(func=merge_action)

    sign_subcommand = subcommands.add_parser(
        "sign", help="Sign a Cortex-M4 HEX files."
    )
    sign_subcommand.add_argument(
        "--build-dir", required=True, help="the build directory."
    )
    sign_subcommand.add_argument(
        "--m0hex-filename", required=True, help="the name of the HEX file."
    )
    sign_subcommand.add_argument(
        "--target-name", help="the Mbed target name."
    )
    sign_subcommand.add_argument(
        "--policy-file-name", help="the name of the policy file."
    )
    sign_subcommand.add_argument(
        "--boot-scheme", help="the boot scheme."
    )
    sign_subcommand.add_argument(
        "--cm0-img-id", type=int, help="the Cortex-M0 image ID."
    )
    sign_subcommand.add_argument(
        "--cm4-img-id", type=int, help="the Cortex-M4 image ID."
    )
    sign_subcommand.add_argument(
        "--elf", required=True, help="the application ELF file."
    )
    sign_subcommand.add_argument(
        "--m4hex", required=True, help="the path to the Cortex-M4 HEX to merge."
    )
    sign_subcommand.add_argument(
        "--m0hex", help="the path to the Cortex-M0 HEX to merge."
    )
    sign_subcommand.set_defaults(func=sign_action)

    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    args.func(args)
