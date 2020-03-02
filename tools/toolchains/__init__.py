"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited
SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
from __future__ import print_function, division, absolute_import

from . import mbed_toolchain, arm, gcc, iar

TOOLCHAIN_CLASSES = {
    u'ARM': arm.ARM_STD,
    u'uARM': arm.ARM_MICRO,
    u'ARMC6': arm.ARMC6,
    u'GCC_ARM': gcc.GCC_ARM,
    u'IAR': iar.IAR
}

EXTRA_TOOLCHAIN_NAMES = [
    u"ARMC5"
]

TOOLCHAINS = set(TOOLCHAIN_CLASSES.keys())

# Top level re-exports
mbedToolchain = mbed_toolchain.mbedToolchain
TOOLCHAIN_PATHS = mbed_toolchain.TOOLCHAIN_PATHS
