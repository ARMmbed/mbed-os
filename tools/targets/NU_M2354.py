#!/usr/bin/python
# Copyright (c) 2017-2021 Arm Limited
#
# SPDX-License-Identifier: Apache-2.0
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

import os
from os.path import abspath, basename, dirname, splitext, isdir
from os.path import join as path_join
import re
import subprocess
import shutil
from intelhex import IntelHex
from datetime import datetime


SCRIPT_DIR = dirname(abspath(__file__))
MBED_OS_ROOT = abspath(path_join(SCRIPT_DIR, os.pardir, os.pardir))
SECURE_ROOT = path_join(MBED_OS_ROOT, 'targets', 'TARGET_NUVOTON', 'TARGET_M2354', 'TARGET_TFM', 'TARGET_NU_M2354', 'COMPONENT_TFM_S_FW')

def m2354_tfm_bin(t_self, non_secure_image, secure_bin):

    assert os.path.isfile(secure_bin)
    assert os.path.isfile(non_secure_image)

    secure_bin = abspath(secure_bin)
    non_secure_image = abspath(non_secure_image)

    build_dir = dirname(non_secure_image)
    tempdir = path_join(build_dir, 'temp')
    if not isdir(tempdir):
        os.makedirs(tempdir)

    flash_layout = path_join(SECURE_ROOT, 'partition', 'flash_layout.h')

    bl2_bin = path_join(SECURE_ROOT, 'bl2.bin')
    s_bin_basename = splitext(basename(secure_bin))[0]
    ns_bin_basename, output_ext = splitext(basename(non_secure_image))

    # Convert NS image to BIN format if it is HEX
    if output_ext == ".hex":
        non_secure_bin = abspath(path_join(tempdir, ns_bin_basename + ".bin"))
        ns_ih = IntelHex(non_secure_image)
        ns_ih.tobinfile(non_secure_bin)
    else:
        non_secure_bin = non_secure_image

    signing_key = path_join(SCRIPT_DIR, 'nuvoton_m2354-root-rsa-3072.pem')
    assert os.path.isfile(signing_key)

    # Find Python 3 command name across platforms
    python3_cmd = "python3" if shutil.which("python3") is not None else "python"

    # Specify image version
    #
    # MCUboot image version format: Major.Minor.Revision+Build
    #
    # Requirements for image version:
    # 1. Major.Minor.Revision must be non-decremental when used to derive security
    #    counter (-s 'auto').
    # 2. Make Major.Minor.Revision+Build incremental to identify the firmware
    #    itself uniquely through psa_fwu_query().
    # 3. Get around MCUboot failure with:
    #    [INF] Starting bootloader
    #    [INF] Swap type: none
    #    [ERR] Failed to add Image 0 data to shared memory area
    #    [ERR] Unable to find bootable image
    #    This is because TF-M underestimates MAX_BOOT_RECORD_SZ for boot record
    #    where Major.Minor.Revision will pack into during signing. The more digits
    #    of the Major.Minor.Revision, the larger the needed boot record size. And
    #    then MCUboot errors in boot_save_boot_status().
    #
    # To meet all the above requirements, we apply the following policy:
    # 1. To not change MAX_BOOT_RECORD_SZ in TF-M, specify Major.Minor.Revision
    #    with TF-M version instead of modified Unix timestamp. This needs less digits to
    #    fit into MAX_BOOT_RECORD_SZ.
    # 2. To make Major.Minor.Revision+Build incremental, specify the Build part with
    #    modified Unix timestamp.
    # 3. To make security counter non-decremental, we can derive it from
    #    Major.Minor.Revision (-s 'auto') or explicitly specify it with modified
    #    Unix timestamp, depending on security consideration.
    #
    # NOTE: To get around Y2038 problem, we modify Unix timestamp by setting new base
    #       point. Using 32-bit unsigned integer to hold the modified Unix timestamp,
    #       it will break (wrap around) after Y2156 (2106 + 2020 - 1970).
    #       https://en.wikipedia.org/wiki/Year_2038_problem
    #
    modified_timestamp = int(datetime.now().timestamp()) - int(datetime(2020, 1, 1).timestamp())
    img_ver_major = 1       # Instead of (modified_timestamp >> 24) & 0xFF
    img_ver_minor = 4       # Instead of (modified_timestamp >> 16) & 0xFF
    img_ver_revision = 0    # Instead of modified_timestamp & 0xFFFF
    img_ver_build = modified_timestamp

    # wrapper.py command template
    cmd_wrapper = [
        python3_cmd,
        path_join(MBED_OS_ROOT, "tools", "psa", "tfm", "bin_utils", "wrapper.py"),
        "-v",
        "{}.{}.{}+{}".format(img_ver_major, img_ver_minor, img_ver_revision, img_ver_build),
        "-k",
        "SIGNING_KEY_PATH",
        "--layout",
        "IMAGE_MACRO_PATH",
        "--public-key-format",
        'full',
        "--align",
        '1',
        # Reasons for removing padding and boot magic option "--pad":
        # 1. PSA FWU API psa_fwu_install() will be responsible for writing boot magic to enable upgradeable.
        # 2. The image size gets smaller instead of slot size.
        #"--pad",
        "--pad-header",
        "-H",
        '0x400',
        "--overwrite-only",
        "-s",
        'auto', # Or modified_timestamp
        "-d",
        '(IMAGE_ID,MAJOR.MINOR.REVISION+BUILD)',
        "RAW_BIN_PATH",
        "SIGNED_BIN_PATH",
    ]
    pos_wrapper_signing_key = cmd_wrapper.index("-k") + 1
    pos_wrapper_layout = cmd_wrapper.index("--layout") + 1
    pos_wrapper_dependency = cmd_wrapper.index("-d") + 1
    pos_wrapper_raw_bin = len(cmd_wrapper) - 2
    pos_wrapper_signed_bin = len(cmd_wrapper) - 1

    # assemble.py command template
    cmd_assemble = [
        python3_cmd,
        path_join(MBED_OS_ROOT, "tools", "psa", "tfm", "bin_utils", "assemble.py"),
        "--layout",
        "IMAGE_MACRO_PATH",
        "-s",
        "SECURE_BIN_PATH",
        "-n",
        "NONSECURE_BIN_PATH",
        "-o",
        "CONCATENATED_BIN_PATH",
    ]
    pos_assemble_layout = cmd_assemble.index("--layout") + 1
    pos_assemble_secure_bin = cmd_assemble.index("-s") + 1
    pos_assemble_nonsecure_bin = cmd_assemble.index("-n") + 1
    pos_assemble_concat_bin = cmd_assemble.index("-o") + 1

    # If second signing key is passed down, go signing separately; otherwise, go signing together.
    if os.path.isfile(path_join(SECURE_ROOT, 'partition', 'signing_layout_ns_preprocessed.h')):
        signing_key_1 = 'nuvoton_m2354-root-rsa-3072_1.pem'
    else:
        signing_key_1 = None

    if signing_key_1 is not None:
        signing_key_1 = path_join(SCRIPT_DIR, signing_key_1)
        assert os.path.isfile(signing_key_1)

        image_macros_s = path_join(SECURE_ROOT, 'partition', 'signing_layout_s_preprocessed.h')
        image_macros_ns = path_join(SECURE_ROOT, 'partition', 'signing_layout_ns_preprocessed.h')
        assert os.path.isfile(image_macros_s)
        assert os.path.isfile(image_macros_ns)

        s_signed_bin = abspath(path_join(tempdir, 'tfm_s_signed' + '.bin'))
        ns_signed_bin = abspath(path_join(tempdir, 'tfm_' + ns_bin_basename + '_signed' + '.bin'))
        signed_concat_bin = abspath(path_join(tempdir, 'tfm_s_signed_' + ns_bin_basename + '_signed_concat' + '.bin'))
        s_update_bin = abspath(path_join(build_dir, s_bin_basename + '_update' + '.bin'))
        ns_update_bin = abspath(path_join(build_dir, ns_bin_basename + '_update' + '.bin'))

        #1. Run wrapper to sign the secure TF-M binary
        cmd_wrapper[pos_wrapper_signing_key] = signing_key
        cmd_wrapper[pos_wrapper_layout] = image_macros_s
        cmd_wrapper[pos_wrapper_dependency] = '(1,0.0.0+0)' # Minimum version of non-secure image required for upgrading to the secure image
        cmd_wrapper[pos_wrapper_raw_bin] = secure_bin
        cmd_wrapper[pos_wrapper_signed_bin] = s_signed_bin

        retcode = run_cmd(cmd_wrapper, MBED_OS_ROOT)
        if retcode:
            raise Exception("Unable to sign " + "TF-M Secure" +
                            " binary, Error code: " + str(retcode))
            return

        #2. Run wrapper to sign the non-secure mbed binary
        cmd_wrapper[pos_wrapper_signing_key] = signing_key_1
        cmd_wrapper[pos_wrapper_layout] = image_macros_ns
        cmd_wrapper[pos_wrapper_dependency] = '(0,0.0.0+0)' # Minimum version of secure image required for upgrading to the non-secure image
        cmd_wrapper[pos_wrapper_raw_bin] = non_secure_bin
        cmd_wrapper[pos_wrapper_signed_bin] = ns_signed_bin

        retcode = run_cmd(cmd_wrapper, MBED_OS_ROOT)
        if retcode:
            raise Exception("Unable to sign " + "TF-M Secure" +
                            " binary, Error code: " + str(retcode))
            return

        #3. Concatenate signed secure TF-M binary and signed non-secure mbed binary
        cmd_assemble[pos_assemble_layout] = image_macros_s
        cmd_assemble[pos_assemble_secure_bin] = s_signed_bin
        cmd_assemble[pos_assemble_nonsecure_bin] = ns_signed_bin
        cmd_assemble[pos_assemble_concat_bin] = signed_concat_bin

        retcode = run_cmd(cmd_assemble, MBED_OS_ROOT)
        if retcode:
            raise Exception("Unable to concatenate " + "Secure TF-M (signed)/Non-secure Mbed (signed)" +
                            " binaries, Error code: " + str(retcode))
            return

        #4. Concatenate MCUboot and concatenated signed secure TF-M binary/signed non-secure mbed binary
        flash_area_0_offset = find_flash_area_0_offset(flash_layout)
        out_ih = IntelHex()
        out_ih.loadbin(bl2_bin)
        out_ih.loadbin(signed_concat_bin, flash_area_0_offset)
        out_ih.tofile(non_secure_image, 'hex' if output_ext == ".hex" else "bin")

        # Generate firmware update file for PSA Firmware Update
        shutil.copy(s_signed_bin, s_update_bin)
        shutil.copy(ns_signed_bin, ns_update_bin)
    else:
        image_macros_s_ns = path_join(SECURE_ROOT, 'partition', 'signing_layout_preprocessed.h')
        assert os.path.isfile(image_macros_s_ns)

        concat_bin = abspath(path_join(tempdir, 'tfm_s_' + ns_bin_basename + ".bin"))
        concat_signed_bin = abspath(path_join(tempdir, 'tfm_s_' + ns_bin_basename + '_signed' + ".bin"))
        update_bin = abspath(path_join(build_dir, ns_bin_basename + '_update' + '.bin'))

        #1. Concatenate secure TFM and non-secure mbed binaries
        cmd_assemble[pos_assemble_layout] = image_macros_s_ns
        cmd_assemble[pos_assemble_secure_bin] = secure_bin
        cmd_assemble[pos_assemble_nonsecure_bin] = non_secure_bin
        cmd_assemble[pos_assemble_concat_bin] = concat_bin

        retcode = run_cmd(cmd_assemble, MBED_OS_ROOT)
        if retcode:
            raise Exception("Unable to concatenate " + "Secure TF-M/Non-secure Mbed" +
                            " binaries, Error code: " + str(retcode))
            return

        #2. Run wrapper to sign the concatenated binary
        cmd_wrapper[pos_wrapper_signing_key] = signing_key
        cmd_wrapper[pos_wrapper_layout] = image_macros_s_ns
        cmd_wrapper[pos_wrapper_dependency] = '(1,0.0.0+0)' # No effect for single image boot
        cmd_wrapper[pos_wrapper_raw_bin] = concat_bin
        cmd_wrapper[pos_wrapper_signed_bin] = concat_signed_bin

        retcode = run_cmd(cmd_wrapper, MBED_OS_ROOT)
        if retcode:
            raise Exception("Unable to sign " + "concatenated" +
                            " binary, Error code: " + str(retcode))
            return

        #3. Concatenate MCUboot and signed binary
        flash_area_0_offset = find_flash_area_0_offset(flash_layout)
        out_ih = IntelHex()
        out_ih.loadbin(bl2_bin)
        out_ih.loadbin(concat_signed_bin, flash_area_0_offset)
        out_ih.tofile(non_secure_image, 'hex' if output_ext == ".hex" else "bin")

        # Generate firmware update file for PSA Firmware Update
        shutil.copy(concat_signed_bin, update_bin)

def find_flash_area_0_offset(configFile):
    # Compiled regular expressions 
    flash_area_bl2_offset_re = re.compile(r"^#define\s+FLASH_AREA_BL2_OFFSET\s+\({0,1}(0x[0-9a-fA-F]+)\){0,1}")
    flash_area_bl2_size_re = re.compile(r"^#define\s+FLASH_AREA_BL2_SIZE\s+\({0,1}(0x[0-9a-fA-F]+)\){0,1}")
    rsvd_stor_size_re = re.compile(r"^#define\s+FLASH_AREA_0_OFFSET\s+\(FLASH_AREA_BL2_OFFSET\s+\+\s+FLASH_AREA_BL2_SIZE\s+\+\s+\({0,1}(0x[0-9a-fA-F]+)\){0,1}\)")

    # Match values
    flash_area_bl2_offset = None
    flash_area_bl2_size = None
    rsvd_stor_size = None
    flash_area_0_offset = None

    with open(configFile, 'r') as configFile_:
        for line in configFile_:
            # Seek "#define FLASH_AREA_BL2_OFFSET..."
            if flash_area_bl2_offset is None:
                m = flash_area_bl2_offset_re.match(line)
                if m is not None:
                    flash_area_bl2_offset = int(m.group(1), 0)
                    continue

            # Seek "#define FLASH_AREA_BL2_SIZE..."
            if flash_area_bl2_size is None:
                m = flash_area_bl2_size_re.match(line)
                if m is not None:
                    flash_area_bl2_size = int(m.group(1), 0)
                    continue

            # Seek "#define FLASH_AREA_0_OFFSET..."
            if rsvd_stor_size is None:
                m = rsvd_stor_size_re.match(line)
                if m is not None:
                    rsvd_stor_size = int(m.group(1), 0)
                    continue

            # FLASH_AREA_0_OFFSET = FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE + Reserved storage area size
            if flash_area_bl2_offset is not None and \
                flash_area_bl2_size is not None and \
                rsvd_stor_size is not None:
                flash_area_0_offset = flash_area_bl2_offset + flash_area_bl2_size + rsvd_stor_size
                break

    return flash_area_0_offset

def run_cmd(cmd, directory):

    # Redirect stdout/stderr to pipe, text mode
    POPEN_INSTANCE = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd=directory,
        universal_newlines=True
    )

    # Command line
    print("COMMAND: {}".format(POPEN_INSTANCE.args))

    stdout_data, stderr_data = POPEN_INSTANCE.communicate()

    # stdout/stderr messages
    if (stdout_data):
        print(stdout_data)
    if (stderr_data):
        print(stderr_data)

    # Return code
    return POPEN_INSTANCE.returncode
