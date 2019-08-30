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
import sys
import subprocess
from array import array
from struct import (pack, unpack)
from shutil import copy2
import json
from intelhex import IntelHex, hex2bin, bin2hex

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
    if toolchain.target.is_PSA_non_secure_target:
        m0hexf = next((f for f in hex_files if os.path.basename(f) == os.path.basename(hexf)), m0hexf)

    if m0hexf:
        toolchain.notify.info("M0 core image file found: %s." % m0hexf)
    else:
        toolchain.notify.info("M0 core hex image file %s not found. Aborting." % hex_filename)
        raise ConfigException("Required M0 core hex image not found.")

    return m0hexf


# check if policy parameters are consistent
def check_slots_integrity(toolchain, fw_cyb, target_data, fw_spe=None, fw_nspe=None):
    """
    Function checks consistency of parameters presented in
    policy file used for build of Secure Boot enabled target.
    :param toolchain: Toolchain object of current build session
    :param fw_cyb: CyBootloader firmware description from policy
    :param target_data: Object contains description of
                        processing target from target.json
    :param fw_spe: CM0p firmware descpription object from policy
    :param fw_nspe: CM4 firmware descpription object from policy
    :return: List of slots and image id corresponding to them
    """
    slot0 = None
    slot1 = None

    if fw_spe is None:
        img_id = fw_nspe["id"]

        # check single stage scheme
        if not (fw_cyb["launch"] == img_id):
            # may be PSA NSPE part
            if not fw_cyb["launch"] == SPE_IMAGE_ID:
                toolchain.notify.debug("[PSOC6.sign_image] WARNING: ID of build image " + str(img_id) +
                                       " does not correspond launch ID in CyBootloader - " + str(fw_cyb["launch"]))
            else:
                toolchain.notify.info("[PSOC6.sign_image] INFO: NSPE image ID is " + str(img_id) +
                                      ". It will be launched by SPE part.")

        # check slots addresses and sizes if upgrade is set to True
        for slot in fw_nspe["resources"]:
            if slot["type"] == "BOOT":
                slot0 = slot

            if fw_nspe["upgrade"] is True:
                slot1 = slot
                if slot["type"] == "UPGRADE":
                    if fw_nspe.get("encrypt") is True:
                        # mark slot1 image as one, that should be encrypted
                        slot1.update({'encrypt': True})
                        toolchain.notify.info("[PSOC6.sign_image] INFO: Image for UPGRADE NSPE will"
                                              " be ENCRYPTED per policy settings.")
                    else:
                        pass
            else:
                toolchain.notify.info("[PSOC6.sign_image] INFO: Image for UPGRADE will not"
                                    " be built per policy settings.")
                break
        if slot0 is None:
            toolchain.notify.debug("[PSOC6.sign_image] WARNING: BOOT section not found in policy resources")
            raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

    else:
        # check if PSA targets flash map correspond to slots addresses and sizes in policy
        if not (int(target_data["overrides"]["secure-rom-start"], 16) - MCUBOOT_HEADER_SIZE) ==\
                int(fw_spe["resources"][0]["address"]):
            toolchain.notify.debug("[PSOC6.sign_image] WARNING: SPE start address "
                                   "does not correspond BOOT slot start address defined in policy. "
                                   "Check if MCUboot header offset 0x400 is included in SPE flash start")

        if not (int(target_data["overrides"]["non-secure-rom-start"], 16) - MCUBOOT_HEADER_SIZE) ==\
                int(fw_nspe["resources"][0]["address"]):
            toolchain.notify.debug("[PSOC6.sign_image] WARNING: NSPE start address "
                                   "does not correspond BOOT slot start address defined in policy. "
                                   "Check if MCUboot header offset 0x400 is included in NSPE flash start")

        if (int(target_data["overrides"]["secure-rom-size"], 16) + MCUBOOT_HEADER_SIZE) >\
                int(fw_spe["resources"][0]["size"]):
            toolchain.notify.debug("[PSOC6.sign_image] WARNING: SPE flash size "
                                   "does not fit in BOOT slot size defined in policy.")

        if (int(target_data["overrides"]["non-secure-rom-size"], 16) + MCUBOOT_HEADER_SIZE) >\
                int(fw_nspe["resources"][0]["size"]):
            toolchain.notify.debug("[PSOC6.sign_image] WARNING: NSPE flash size "
                                   "does not fit in BOOT slot size defined in policy.")

        img_id = fw_spe["id"]
        # check dual stage scheme
        if img_id != 1:
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: Image ID of SPE image"
                                   " is not equal to 1!")
            raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

        if not (fw_cyb["launch"] == img_id):
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: ID of build image"
                                   " does not correspond launch ID in CyBootloader!")
            raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

        if not (fw_spe["launch"] == fw_nspe["id"]):
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: ID of NSPE image"
                                   " does not correspond launch ID in SPE part!")
            raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

        # check slots addresses and sizes if upgrade is set to True
        for slot in fw_spe["resources"]:
            if slot["type"] == "BOOT":
                slot0 = slot
            if fw_spe["upgrade"] is True:
                if slot["type"] == "UPGRADE":
                    slot1 = slot
                    if fw_spe.get("encrypt") is True:
                        # mark slot1 image as one, that should be encrypted
                        slot1.update({'encrypt': True})
                        toolchain.notify.info("[PSOC6.sign_image] INFO: Image for UPGRADE SPE will"
                                            " be ENCRYPTED per policy settings.")
                    else:
                        pass
            else:
                toolchain.notify.info("[PSOC6.sign_image] INFO: Image for UPGRADE will not"
                                      " be produced per policy settings.")
    if slot0 is None:
        toolchain.notify.debug("[PSOC6.sign_image] WARNING: BOOT section not found in policy resources")
        raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

    if slot1 is not None:
        # bigger or equal to 0x18000000 in hex is a start of SMIF memory
        if slot1["address"] >= SMIF_MEM_MAP_START:
            toolchain.notify.info("[PSOC6.sign_image] INFO: UPGRADE slot will be resided in external flash")

        if slot0["size"] != slot1["size"]:
            toolchain.notify.debug("[PSOC6.sign_image] WARNING: BOOT and UPGRADE slots sizes are not equal")

        return (slot0, slot1, img_id)
    else:
        return (slot0, None, img_id)


def process_target(toolchain, target):
    """
    Gathers and process information about target being built
    :param toolchain: Toolchain object of current build session
    :param target: Name of target being built
    :return: List with all data needed for adding signature
    """
    from pathlib import Path

    targets_json = Path("targets/targets.json")
    cy_targets = Path("targets/TARGET_Cypress/TARGET_PSOC6/")
    sb_params_file_name = Path("secure_image_parameters.json")
    root_dir = Path(os.getcwd())

    mbed_os_targets = root_dir / targets_json

    if not os.path.isfile(str(mbed_os_targets)):
        # try location for tests
        mbed_os_targets = root_dir / 'mbed-os' / targets_json
        root_dir = root_dir / 'mbed-os'
        if not os.path.isfile(str(mbed_os_targets)):
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: targets.json not found!")
            raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

    with open(str(mbed_os_targets)) as j:
        json_str = j.read()
        all_targets = json.loads(json_str)
        j.close()

    processing_target = all_targets[target]
    sb_params_file_path = root_dir / cy_targets / Path("TARGET_" + str(target)) / sb_params_file_name

    if os.path.isfile(str(sb_params_file_path)):
        with open(str(sb_params_file_path)) as f:
            json_str = f.read()
            sb_config = json.loads(json_str)
            f.close()
    else:
        toolchain.notify.debug("[PSOC6.sign_image] ERROR: secure_image_parametest.json not found!")
        raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

    sdk_path = root_dir / sb_config["sdk_path"]

    priv_key_path = sdk_path / Path(sb_config["priv_key_file"])

    if not os.path.isfile(str(priv_key_path)):
        toolchain.notify.debug("[PSOC6.sign_image] ERROR: Private key file not found in " + str(priv_key_path))
        raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

    if "_PSA" in target:
        # assume dual stage bootloading scheme
        with open(sdk_path / Path(sb_config["policy_file"])) as p:
            policy_str = p.read()
            policy_file = json.loads(policy_str)
            p.close()

            firmware_list = policy_file["boot_upgrade"]["firmware"]

        # collect firmware descriptions from policy for corresponding images
        firmware_cyb_cm0p = firmware_list[0]

        if "_M0_" in target:
            firmware_spe_cm0p = firmware_list[1]
            firmware_nspe_cm4 = firmware_list[2]

            slots = check_slots_integrity(toolchain, fw_cyb=firmware_cyb_cm0p, fw_spe=firmware_spe_cm0p,
                                          fw_nspe=firmware_nspe_cm4, target_data=processing_target)
        else:
            firmware_nspe_cm4 = firmware_list[2]
            slots = check_slots_integrity(toolchain, fw_cyb=firmware_cyb_cm0p, fw_nspe=firmware_nspe_cm4,
                                          target_data=processing_target)
    else:
        # consider single stage bootloading scheme
        with open(sdk_path / Path(sb_config["policy_file"])) as p:
            policy_str = p.read()
            policy_file = json.loads(policy_str)
            p.close()

        firmware_list = policy_file["boot_upgrade"]["firmware"]
        firmware_cyb_cm0p = firmware_list[0]
        firmware_nspe_cm4 = firmware_list[1]
        slots = check_slots_integrity(toolchain, fw_cyb=firmware_cyb_cm0p,
                                      fw_nspe=firmware_nspe_cm4, target_data=processing_target)

    target_sig_data = [{"img_data": sb_config["boot0"], "slot_data": slots[0],
                        "key_file": sb_config["priv_key_file"], "sdk_path": sdk_path, "id": slots[2]}]

    if slots[1] is not None:
        target_sig_data.append({"img_data": sb_config["boot1"], "slot_data": slots[1],
                                "key_file": sb_config["priv_key_file"], "sdk_path": sdk_path, "id": slots[2]})
        # check if slot1 image sould be encrypted
        if slots[1].get("encrypt") is True:

            dev_pub_key = sdk_path / Path(sb_config["dev_pub_key_file"])
            if not os.path.isfile(str(dev_pub_key)):
                toolchain.notify.debug("[PSOC6.sign_image] ERROR: Device public key file not found in " + str(dev_pub_key))
                raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

            aes_key_file = sdk_path / Path(sb_config["aes_key_file"])
            if not os.path.isfile(str(aes_key_file)):
                toolchain.notify.debug("[PSOC6.sign_image] ERROR: AES-128 key file not found in " + str(aes_key_file))
                raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

            target_sig_data[1].update({"aes_key": sb_config["aes_key_file"], "dev_pub_key": sb_config["dev_pub_key_file"]})

        else:
            toolchain.notify.info("[PSOC6.sign_image] INFO: Image for slot UPGRADE would not be encrypted per policy settings")

    return target_sig_data


def sign_image(toolchain, binf):
    """
    Adds signature to a binary file being built,
    prepares some intermediate binary artifacts.
    :param toolchain: Toolchain object of current build session
    :param binf: Binary file created for target
    """
    from pathlib import PurePath

    target_sig_data = None
    # reserve name for separate NSPE image
    out_cm4_hex = binf[:-4] + "_cm4.hex"

    # preserve original hex file from mbed-os build
    mbed_hex = binf[:-4] + "_unsigned.hex"
    copy2(binf, mbed_hex)

    # find target name and type before processing
    for part in PurePath(binf).parts:
        if "CY" in part:
            target_sig_data = process_target(toolchain=toolchain, target=part)

    if target_sig_data is None:
        toolchain.notify.debug("[PSOC6.sign_image] ERROR: Target not found!")
        raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")

    for slot in target_sig_data:
        # first check if image for slot under processing should be encrypted
        if slot["slot_data"].get("encrypt") is True:
            # call encrypt_img to perform encryption
            args = [sys.executable, str(slot["sdk_path"] / "encrypted_image_runner.py"),
                                    "--sdk-path", str(slot["sdk_path"]), "--hex-file", os.getcwd() + '/' + mbed_hex,
                                    "--key-priv", str(slot["sdk_path"] / slot["key_file"]),
                                    "--key-pub", str(slot["sdk_path"] / slot["dev_pub_key"]),
                                    "--key-aes", str(slot["sdk_path"] / slot["aes_key"]),
                                    "--ver", str(slot["img_data"]["VERSION"]), "--img-id", str(slot["id"]),
                                    "--rlb-count", str(slot["img_data"]["ROLLBACK_COUNTER"]),
                                    "--slot-size", str(hex(slot["slot_data"]["size"])),
                                    "--img-offset", str(slot["slot_data"]["address"])]
            if slot["slot_data"]["type"] != "BOOT":
                args.append("--pad")
            process = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # catch standard process pipes outputs
            stderr = process.communicate()[1]
            stdout = process.communicate()[0]
            rc = process.wait()
            toolchain.notify.info(stdout.decode("utf-8"))

            if rc != 0:
                toolchain.notify.debug("[PSOC6.sign_image] ERROR: Encryption script ended with error!")
                toolchain.notify.debug("[PSOC6.sign_image] Message from encryption script: " + stderr.decode("utf-8"))
                raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")
            else:
                toolchain.notify.info("[PSOC6.sign_image] SUCCESS: Image for slot " +
                                    slot["slot_data"]["type"] + " is signed and encrypted with no errors!")
        # all non ecrypted images take this path
        else:
            if slot["slot_data"]["type"] == "UPGRADE":
                out_hex_name = binf[:-4] + "_upgrade.hex"
            else:
                out_hex_name = binf
            
            out_bin_name = out_hex_name[:-4] + "_signed.bin"

            # call imgtool for signature
            args = [sys.executable, str(slot["sdk_path"] / "imgtool/imgtool.py"),
                                    "sign", "--key", str(slot["sdk_path"] / slot["key_file"]),
                                    "--header-size", str(hex(MCUBOOT_HEADER_SIZE)), "--pad-header", "--align", "8",
                                    "--version", str(slot["img_data"]["VERSION"]), "--image-id",
                                    str(slot["id"]), "--rollback_counter", str(slot["img_data"]["ROLLBACK_COUNTER"]),
                                    "--slot-size", str(hex(slot["slot_data"]["size"])), "--overwrite-only",
                                    mbed_hex, out_hex_name]
            if slot["slot_data"]["type"] != "BOOT":
                args.append("--pad")
            process = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # catch stderr outputs
            stderr = process.communicate()[1]
            rc = process.wait()

            if rc != 0:
                toolchain.notify.debug("[PSOC6.sign_image] ERROR: Signature is not added!")
                toolchain.notify.debug("[PSOC6.sign_image] Message from imgtool: " + stderr.decode("utf-8"))
                raise AddSignatureError("PSOC6.sign_image finished execution with errors! Signature is not added.")
            else:
                toolchain.notify.info("[PSOC6.sign_image] SUCCESS: Image for slot " +
                                    slot["slot_data"]["type"] + " is signed with no errors!")
            # preserve signed binary file
            hex2bin(out_hex_name, out_bin_name)

            # preserve separate hex for cm4
            # 16 is image ID for NSPE image
            if slot["id"] == NSPE_IMAGE_ID:
                copy2(out_hex_name, out_cm4_hex)

            # produce hex file for slot1
            if slot["slot_data"]["type"] == "UPGRADE":
                bin2hex(out_bin_name, out_hex_name, offset=int(slot["slot_data"]["address"]))
                toolchain.notify.info("Image UPGRADE: " + out_hex_name + "\n")


def complete(toolchain, elf0, hexf0, hexf1=None):
    """
    Merge CM4 and CM0 images to a single binary
    """
    complete_func(toolchain.notify.debug, elf0, hexf0, hexf1)
