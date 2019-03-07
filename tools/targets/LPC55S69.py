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
from os.path import abspath, dirname
from os.path import join as path_join

from intelhex import IntelHex

SCRIPT_DIR = dirname(abspath(__file__))
MBED_OS_ROOT = abspath(path_join(SCRIPT_DIR, os.pardir, os.pardir))
LPC55S69_BASE = path_join(MBED_OS_ROOT, 'targets', 'TARGET_NXP', 'TARGET_MCUXpresso_MCUS', 'TARGET_LPC55S69')


def lpc55s69_complete(t_self, non_secure_bin, secure_bin):
    if secure_bin is None:
        return None

    assert os.path.isfile(secure_bin)
    assert os.path.isfile(non_secure_bin)

    ns_hex = IntelHex(non_secure_bin)
    s_hex = IntelHex()
    s_hex.loadbin(secure_bin)

    s_hex.merge(ns_hex, overlap='ignore')
    s_hex.tofile(non_secure_bin, 'hex')
