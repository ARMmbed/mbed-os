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

import os
from os.path import abspath, basename, dirname, splitext
from os.path import join as path_join
import tempfile

from tools.psa.tfm.bin_utils.assemble import Assembly

SCRIPT_DIR = dirname(abspath(__file__))
MBED_OS_ROOT = abspath(path_join(SCRIPT_DIR, os.pardir, os.pardir))
LPC55S69_BASE = path_join(MBED_OS_ROOT, 'targets', 'TARGET_NXP', 'TARGET_MCUXpresso_MCUS', 'TARGET_LPC55S69')

def lpc55s69_tfm_bin(t_self, non_secure_bin, secure_bin):
    tempdir = tempfile.mkdtemp()
    flash_layout = path_join(LPC55S69_BASE, 'partition', 'flash_layout.h')
    ns_bin_name, ns_bin_ext = splitext(basename(non_secure_bin))
    concatenated_bin = path_join(tempdir, 'tfm_' + ns_bin_name + ns_bin_ext)

    assert os.path.isfile(flash_layout)
    assert os.path.isfile(secure_bin)
    assert os.path.isfile(non_secure_bin)

    #1. Concatenate secure TFM and non-secure mbed binaries
    output = Assembly(flash_layout, concatenated_bin)
    output.add_image(secure_bin, "SECURE")
    output.add_image(non_secure_bin, "NON_SECURE")

    #2. Delete the original binary file name
    os.remove(non_secure_bin)

    #3. Rename to new concatenated binary to the original binary file name
    os.rename(concatenated_bin, non_secure_bin)

