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
import sys

# Add PSA TF-M binary utility scripts in system path
TFM_SCRIPTS = abspath(path_join(dirname(__file__), '..', 'psa', 'tfm', 'bin_utils'))
sys.path.insert(0, TFM_SCRIPTS)

SCRIPT_DIR = dirname(abspath(__file__))
MBED_OS_ROOT = abspath(path_join(SCRIPT_DIR, os.pardir, os.pardir))

def musca_tfm_bin(t_self, non_secure_bin, secure_bin, target_name):

    MUSCA_BASE = path_join(MBED_OS_ROOT, 'targets', 'TARGET_ARM_SSG', ('TARGET_' + target_name))
    assert os.path.isfile(secure_bin)
    assert os.path.isfile(non_secure_bin)

    build_dir = dirname(non_secure_bin)
    tempdir = path_join(build_dir, 'temp')
    if not isdir(tempdir):
        os.makedirs(tempdir)
    flash_layout = path_join(MUSCA_BASE, 'partition', 'flash_layout.h')
    mcuboot_bin = path_join(MUSCA_BASE, 'bl2.bin')
    image_macros_s = path_join(MUSCA_BASE, 'partition', 'signing_layout_s.c')
    image_macros_ns = path_join(MUSCA_BASE, 'partition', 'signing_layout_ns.c')
    s_bin_name, s_bin_ext = splitext(basename(secure_bin))
    s_signed_bin = abspath(path_join(tempdir, s_bin_name + '_signed' + s_bin_ext))
    ns_bin_name, ns_bin_ext = splitext(basename(non_secure_bin))
    ns_signed_bin = abspath(path_join(tempdir, 'tfm_' + ns_bin_name + '_signed' + ns_bin_ext))
    concatenated_bin = abspath(path_join(tempdir, s_bin_name + '_' + ns_bin_name + '_concat' + ns_bin_ext))

    assert os.path.isfile(image_macros_s)
    assert os.path.isfile(image_macros_ns)

    #1. Run wrapper to sign the TF-M secure binary
    cmd = [
        "python3",
        path_join(MBED_OS_ROOT, "tools", "psa","tfm", "bin_utils","wrapper.py"),
        "-v",
        '1.2.0',
        "-k",
        path_join(SCRIPT_DIR, (target_name.lower() + '-root-rsa-3072.pem')),
        "--layout",
        image_macros_s,
        "--public-key-format",
        'full',
        "--align",
        '1',
        "--pad",
        "--pad-header",
        "-H",
        '0x400',
        "--overwrite-only",
        "-s",
        'auto',
        "-d",
        '(0,0.0.0+0)',
        abspath(secure_bin),
        s_signed_bin,
    ]

    retcode = run_cmd(cmd, MBED_OS_ROOT)
    if retcode:
        raise Exception("Unable to sign " + target_name +
                            " secure binary, Error code: " + retcode)
        return

    #2. Run wrapper to sign the non-secure mbed binary
    cmd = [
        "python3",
        path_join(MBED_OS_ROOT, "tools", "psa","tfm", "bin_utils","wrapper.py"),
        "-v",
        '1.2.0',
        "-k",
        path_join(SCRIPT_DIR, (target_name.lower() + '-root-rsa-3072_1.pem')),
        "--layout",
        image_macros_ns,
        "--public-key-format",
        'full',
        "--align",
        '1',
        "--pad",
        "--pad-header",
        "-H",
        '0x400',
        "--overwrite-only",
        "-s",
        'auto',
        "-d",
        '(1,0.0.0+0)',
        abspath(non_secure_bin),
        ns_signed_bin,
    ]

    retcode = run_cmd(cmd, MBED_OS_ROOT)
    if retcode:
        raise Exception("Unable to sign " + target_name +
                            " non-secure binary, Error code: " + retcode)
        return

    #3. Concatenate signed secure TFM and non-secure mbed binaries
    cmd = [
        "python3",
        path_join(MBED_OS_ROOT, "tools", "psa","tfm", "bin_utils","assemble.py"),
        "--layout",
        image_macros_s,
        "-s",
        s_signed_bin,
        "-n",
        ns_signed_bin,
        "-o",
        concatenated_bin,
    ]

    retcode = run_cmd(cmd, MBED_OS_ROOT)
    if retcode:
        raise Exception("Unable to concatenate " + target_name +
                            " binaries, Error code: " + retcode)
        return

    #4. Concatenate mcuboot and signed binary and overwrite mbed built binary file
    mcuboot_image_size = find_bl2_size(flash_layout)
    with open(mcuboot_bin, "rb") as mcuboot_fh, open(concatenated_bin, "rb") as concat_fh:
        with open(non_secure_bin, "w+b") as out_fh:
            out_fh.write(mcuboot_fh.read())
            out_fh.seek(mcuboot_image_size)
            out_fh.write(concat_fh.read())


def find_bl2_size(configFile):
    bl2_size_re = re.compile(r"^#define\s+FLASH_AREA_BL2_SIZE\s+\({0,1}(0x[0-9a-fA-F]+)\){0,1}")
    bl2_size = None
    with open(configFile, 'r') as flash_layout_file:
        for line in flash_layout_file:
            m = bl2_size_re.match(line)
            if m is not None:
                bl2_size = int(m.group(1), 0)
                break
    return bl2_size

def run_cmd(cmd, directory):
    POPEN_INSTANCE = subprocess.Popen(
        cmd,
        stderr=subprocess.STDOUT,
        cwd=directory,
    )
    POPEN_INSTANCE.communicate()
    return POPEN_INSTANCE.returncode

