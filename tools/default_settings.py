"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited
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

from os.path import join, abspath, dirname
import logging

ROOT = abspath(join(dirname(__file__), ".."))

# These default settings have two purposes:
#    1) Give a template for writing local "private_settings.py"
#    2) Give default initialization fields for the "toolchains.py" constructors

##############################################################################
# Build System Settings
##############################################################################
BUILD_DIR = abspath(join(ROOT, "build"))

# ARM
ARM_PATH = "C:/Program Files/ARM"
ARM_BIN = join(ARM_PATH, "bin")
ARM_INC = join(ARM_PATH, "include")
ARM_LIB = join(ARM_PATH, "lib")

ARM_CPPLIB = join(ARM_LIB, "cpplib")
MY_ARM_CLIB = join(ARM_PATH, "lib", "microlib")

# GCC ARM
GCC_ARM_PATH = ""

# GCC CodeRed
GCC_CR_PATH = "C:/code_red/RedSuite_4.2.0_349/redsuite/Tools/bin"

# IAR
IAR_PATH = "C:/Program Files (x86)/IAR Systems/Embedded Workbench 7.0/arm"

# Goanna static analyser. Please overload it in private_settings.py
GOANNA_PATH = "c:/Program Files (x86)/RedLizards/Goanna Central 3.2.3/bin"

# cppcheck path (command) and output message format
CPPCHECK_CMD = ["cppcheck", "--enable=all"]
CPPCHECK_MSG_FORMAT = ["--template=[{severity}] {file}@{line}: {id}:{message}"]

BUILD_OPTIONS = []

# mbed.org username
MBED_ORG_USER = ""

##############################################################################
# Private Settings
##############################################################################
try:
    # Allow to overwrite the default settings without the need to edit the
    # settings file stored in the repository
    from mbed_settings import *
except ImportError:
    print '[WARNING] Using default settings. Define your settings in the file "./mbed_settings.py"'
