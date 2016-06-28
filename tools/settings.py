"""
mbed SDK
Copyright (c) 2016 ARM Limited

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

from os import getenv, pathsep
from os.path import join, abspath, dirname, exists, isfile, basename, normpath
import sys
import logging

ROOT = abspath(join(dirname(__file__), ".."))


##############################################################################
# Toolchains and Build System Settings
##############################################################################
BUILD_DIR = abspath(join(ROOT, ".build"))

# ARM Compiler 5
ARM_PATH = ""

# GCC ARM
GCC_ARM_PATH = ""

# GCC CodeRed
GCC_CR_PATH = ""

# IAR
IAR_PATH = ""

# Goanna static analyser. Please overload it in mbed_settings.py
GOANNA_PATH = "c:/Program Files (x86)/RedLizards/Goanna Central 3.2.3/bin"

# cppcheck path (command) and output message format
CPPCHECK_CMD = ["cppcheck", "--enable=all"]
CPPCHECK_MSG_FORMAT = ["--template=[{severity}] {file}@{line}: {id}:{message}"]

BUILD_OPTIONS = []

# mbed.org username
MBED_ORG_USER = ""


##############################################################################
# Path Binary discovery
##############################################################################

# Binaries for which to search in the PATH
# No need to search for GCC-based toolchains at the moment
PATH_BINARY_SEARCH = {
    "ARM_PATH": "bin/armcc",
    "IAR_PATH": "bin/iccarm"
}

# Function that accepts a binary/command name (ex. "armcc") and returns its
# location in the system's PATH variable if it exists.
def find_bin_in_path(bin_name):        
    executable_name = bin_name
    
    # Add .exe for Windows platforms
    if sys.platform in ["win32", "cygwin"] and not executable_name.endswith('.exe'):
        executable_name = executable_name + ".exe"
    
    # Check the PATH
    for path in getenv("PATH").split(pathsep):
        path = path.strip('"')
        tool_bin_full_path = join(path, executable_name)
        if exists(tool_bin_full_path) and isfile(tool_bin_full_path):
            # If found, return the path to the binary
            return join(path, bin_name)
    
    # If not found, return None
    return None

# Search for each of the toolchains that require a known path
for toolchain_name, bin_path in PATH_BINARY_SEARCH.iteritems():
    # Normalize the path the binary path
    bin_path = normpath(bin_path)
    
    # Find the path to the binary in the system's PATH
    bin_path_in_path = find_bin_in_path(basename(bin_path))
    
    if bin_path_in_path:
        # Normalize the path and set the base path for the toolchain
        bin_path_in_path = normpath(bin_path_in_path)
        substr_index = bin_path_in_path.rfind(bin_path)
        globals()[toolchain_name] = normpath(bin_path_in_path[:substr_index])


##############################################################################
# User Settings (file)
##############################################################################
try:
    # Allow to overwrite the default settings without the need to edit the
    # settings file stored in the repository
    from mbed_settings import *
except ImportError:
    pass


##############################################################################
# User Settings (env vars)
##############################################################################
_ENV_PATHS = ['ARM_PATH', 'GCC_ARM_PATH', 'GCC_CR_PATH', 'IAR_PATH']

for _n in _ENV_PATHS:
    if getenv('MBED_'+_n):
        globals()[_n] = getenv('MBED_'+_n)
        if not exists(getenv('MBED_'+_n)):
            print "WARNING: MBED_%s set as environment variable but doesn't exist" % _n


##############################################################################
# ARM Compiler Paths
##############################################################################

ARM_BIN = join(ARM_PATH, "bin")
ARM_INC = join(ARM_PATH, "include")
ARM_LIB = join(ARM_PATH, "lib")
ARM_CPPLIB = join(ARM_LIB, "cpplib")
MY_ARM_CLIB = join(ARM_LIB, "lib", "microlib")


##############################################################################
# Test System Settings
##############################################################################
SERVER_PORT = 59432
SERVER_ADDRESS = "10.2.200.94"
LOCALHOST = "10.2.200.94"

MUTs = {
    "1" : {"mcu": "LPC1768",
        "port":"COM41", "disk":'E:\\',
        "peripherals": ["TMP102", "digital_loop", "port_loop", "analog_loop", "SD"]
    },
    "2": {"mcu": "LPC11U24",
        "port":"COM42", "disk":'F:\\',
        "peripherals":  ["TMP102", "digital_loop", "port_loop", "SD"]
    },
    "3" : {"mcu": "KL25Z",
        "port":"COM43", "disk":'G:\\',
        "peripherals":  ["TMP102", "digital_loop", "port_loop", "analog_loop", "SD"]
    },
}
