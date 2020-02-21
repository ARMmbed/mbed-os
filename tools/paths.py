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
from os.path import join, commonprefix, realpath
from os import getenv

# Conventions about the directory structure
from tools.settings import ROOT, BUILD_DIR

# Allow overriding some of the build parameters using environment variables
BUILD_DIR = getenv("MBED_BUILD_DIR") or BUILD_DIR

# Embedded Libraries Sources
LIB_DIR = join(ROOT, "features/unsupported")

TOOLS = join(ROOT, "tools")
TOOLS_DATA = join(TOOLS, "data")
TOOLS_BOOTLOADERS = join(TOOLS, "bootloaders")

# mbed libraries
MBED_HEADER = join(ROOT, "mbed.h")
MBED_DRIVERS = join(ROOT, "drivers")
MBED_PLATFORM = join(ROOT, "platform")
MBED_HAL = join(ROOT, "hal")

MBED_CMSIS_PATH = join(ROOT, "cmsis")
MBED_TARGETS_PATH = join(ROOT, "targets")

MBED_LIBRARIES = join(BUILD_DIR, "mbed")
MBED_LIBRARIES_DRIVERS = join(MBED_LIBRARIES, "drivers")
MBED_LIBRARIES_PLATFORM = join(MBED_LIBRARIES, "platform")
MBED_LIBRARIES_HAL = join(MBED_LIBRARIES, "hal")

MBED_CONFIG_FILE = join(ROOT, "platform/mbed_lib.json")

# Tests
TEST_DIR = join(LIB_DIR, "tests")
HOST_TESTS = join(ROOT, "tools", "host_tests")

# mbed RPC
MBED_RPC = join(LIB_DIR, "rpc")

RPC_LIBRARY = join(BUILD_DIR, "rpc")

# DSP
DSP = join(LIB_DIR, "dsp")
DSP_CMSIS = join(DSP, "cmsis_dsp")
DSP_ABSTRACTION = join(DSP, "dsp")
DSP_LIBRARIES = join(BUILD_DIR, "dsp")

# USB Device
USB = join(LIB_DIR, "USBDevice")
USB_LIBRARIES = join(BUILD_DIR, "usb")

# Export
EXPORT_DIR = join(BUILD_DIR, "export")
EXPORT_WORKSPACE = join(EXPORT_DIR, "workspace")
EXPORT_TMP = join(EXPORT_DIR, ".temp")

# CppUtest library
CPPUTEST_DIR = join(ROOT, "..")
CPPUTEST_SRC = join(CPPUTEST_DIR, "cpputest", "src", "CppUTest")
CPPUTEST_INC = join(CPPUTEST_DIR, "cpputest", "include")
CPPUTEST_INC_EXT = join(CPPUTEST_DIR, "cpputest", "include", "CppUTest")
# Platform dependant code is here (for armcc compiler)
CPPUTEST_PLATFORM_SRC = join(CPPUTEST_DIR, "cpputest", "src", "Platforms",
                             "armcc")
CPPUTEST_PLATFORM_INC = join(CPPUTEST_DIR, "cpputest", "include", "Platforms",
                             "armcc")
# Function 'main' used to run all compiled UTs
CPPUTEST_TESTRUNNER_SCR = join(TEST_DIR, "utest", "testrunner")
CPPUTEST_TESTRUNNER_INC = join(TEST_DIR, "utest", "testrunner")

CPPUTEST_LIBRARY = join(BUILD_DIR, "cpputest")
