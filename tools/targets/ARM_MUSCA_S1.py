#!/usr/bin/python
# Copyright (c) 2017-2020 Arm Limited
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
from argparse import Namespace
from tools.psa.tfm.bin_utils.assemble import Assembly
from tools.psa.tfm.bin_utils.imgtool import do_sign
from tools.psa.tfm.bin_utils.imgtool_lib import version

SCRIPT_DIR = dirname(abspath(__file__))
MBED_OS_ROOT = abspath(path_join(SCRIPT_DIR, os.pardir, os.pardir))
MUSCA_S1_BASE = path_join(MBED_OS_ROOT, 'targets', 'TARGET_ARM_SSG', 'TARGET_MUSCA_S1')


def musca_tfm_bin(t_self, non_secure_bin, secure_bin):

    assert os.path.isfile(secure_bin)
    assert os.path.isfile(non_secure_bin)

    build_dir = dirname(non_secure_bin)
    tempdir = path_join(build_dir, 'temp')
    if not isdir(tempdir):
        os.makedirs(tempdir)
    flash_layout = path_join(MUSCA_S1_BASE, 'partition', 'flash_layout.h')
    mcuboot_bin = path_join(MUSCA_S1_BASE, 'mcuboot.bin')
    image_macros_s = path_join(MUSCA_S1_BASE, 'partition', 'image_macros_preprocessed_s.c')
    image_macros_ns = path_join(MUSCA_S1_BASE, 'partition', 'image_macros_preprocessed_ns.c')
    s_bin_name, s_bin_ext = splitext(basename(secure_bin))
    s_signed_bin = path_join(tempdir, s_bin_name + '_signed' + s_bin_ext)
    ns_bin_name, ns_bin_ext = splitext(basename(non_secure_bin))
    ns_signed_bin = path_join(tempdir, 'tfm_' + ns_bin_name + '_signed' + ns_bin_ext)
    concatenated_bin = path_join(tempdir, s_bin_name + '_' + ns_bin_name + '_concat' + ns_bin_ext)

    assert os.path.isfile(image_macros_s)
    assert os.path.isfile(image_macros_ns)

    #1. Run imgtool to sign the secure binary
    sign_args = Namespace(
        layout=image_macros_s,
        key=path_join(SCRIPT_DIR, 'musca_s1-root-rsa-3072.pem'),
        public_key_format=None,
        align=1,
        dependencies=None,
        version=version.decode_version('1.0'),
        header_size=0x400,
        security_counter=None,
        rsa_pkcs1_15=False,
        included_header=False,
        infile=secure_bin,
        outfile=s_signed_bin
        )
    do_sign(sign_args)

    #2. Run imgtool to sign the non-secure mbed binary
    sign_args = Namespace(
        layout=image_macros_ns,
        key=path_join(SCRIPT_DIR, 'musca_s1-root-rsa-3072_1.pem'),
        public_key_format=None,
        align=1,
        dependencies=None,
        version=version.decode_version('1.0'),
        header_size=0x400,
        security_counter=None,
        rsa_pkcs1_15=False,
        included_header=False,
        infile=non_secure_bin,
        outfile=ns_signed_bin
        )
    do_sign(sign_args)

    #1. Concatenate signed secure TFM and non-secure mbed binaries
    output = Assembly(image_macros_s, concatenated_bin)
    output.add_image(s_signed_bin, "SECURE")
    output.add_image(ns_signed_bin, "NON_SECURE")

    #3. Concatenate mcuboot and signed binary and overwrite mbed built binary file
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
