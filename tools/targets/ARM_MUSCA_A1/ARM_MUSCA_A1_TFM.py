 #
 # Copyright (c) 2018 Arm Limited
 #
 # Licensed under the Apache License Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing software
 # distributed under the License is distributed on an "AS IS" BASIS
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

try:
 import Crypto
except ImportError, e:
 print('Please install "pycryptodome" Python module! ("pip install --user pycryptodome")')
 quit()

import sys
import os.path
from os.path import(
    join,
    dirname
)
run_dir = dirname(os.path.realpath(__file__))
sys.path.append(run_dir)

import assemble
import imgtool
import re

def musca_tfm_hex(t_self, mbed_ns_binary_file):
    #Input files and paths
    mcuboot_binary_file_name = "mcuboot.bin"
    tfm_binary_file_name = "tfm_s.bin"
    flash_layout_file_name = "flash_layout.h"
    musca_tfm_rel_path = join("targets", "TARGET_ARM_SSG", "TARGET_MUSCA_A1", "TFM")

    #Temporary files
    mbed_ns_file_name, ext = os.path.splitext(os.path.basename(mbed_ns_binary_file))
    concatenated_binary_file_name = mbed_ns_file_name + "_tfm.bin"
    signed_binary_file_name = mbed_ns_file_name + "_signed.bin"

    build_dir = dirname(mbed_ns_binary_file)
    mbed_os_dir = dirname(dirname(dirname(run_dir)))
    tfm_musca_dir = join(mbed_os_dir, musca_tfm_rel_path)
    mcuboot_binary_file_name = join(tfm_musca_dir, mcuboot_binary_file_name)
    tfm_binary_file = join(tfm_musca_dir, tfm_binary_file_name)
    flash_layout_file = join(tfm_musca_dir, flash_layout_file_name)

    concatenated_binary_file = join(build_dir, concatenated_binary_file_name)
    signed_binary_file = join(build_dir, signed_binary_file_name)
    temporary_files = [concatenated_binary_file, signed_binary_file]

    #1. Run assemble.py to concatenate secure TFM and non-secure mbed binaries
    sys.argv = ['assemble.py',
                '-l', flash_layout_file,
                '-s', tfm_binary_file,
                '-n', mbed_ns_binary_file,
                '-o', concatenated_binary_file]
    assemble.main()

    #2. Run imgtool.py to sign the concatenated binary
    sys.argv = ['imgtool.py',
                'sign', '--layout', flash_layout_file,
                '-k', join(run_dir, 'root-rsa-2048.pem'),
                '--align', '1', '-v', '1.0', '-H', '0x400', '--pad', '0x100000',
                concatenated_binary_file, signed_binary_file]
    imgtool.args()

    #3. Concatenate mcuboot and signed binary and overwrite mbed built binary file
    mcuboot_image_size = find_bl2_size(flash_layout_file)

    with open(mcuboot_binary_file_name, 'rb') as fh:
        mcuboot_bin_file = fh.read()

    with open(signed_binary_file, 'rb') as fh:
        tfm_mbed_signed_bin_file = fh.read()

    with open(mbed_ns_binary_file, 'w+b') as fh:
        fh.write(mcuboot_bin_file)
        fh.seek(mcuboot_image_size)
        fh.write(tfm_mbed_signed_bin_file)
        fh.close

    #4. Delete temporary files
    for file in temporary_files:
        os.remove(file)

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