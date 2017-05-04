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
from tools.paths import MBED_LIBRARIES,\
    MBED_RPC, RPC_LIBRARY, USB, USB_LIBRARIES, \
    DSP_ABSTRACTION, DSP_CMSIS, DSP_LIBRARIES,\
    CPPUTEST_SRC,\
    CPPUTEST_PLATFORM_SRC, CPPUTEST_TESTRUNNER_SCR, CPPUTEST_LIBRARY,\
    CPPUTEST_INC, CPPUTEST_PLATFORM_INC, CPPUTEST_TESTRUNNER_INC,\
    CPPUTEST_INC_EXT
from tools.data.support import DEFAULT_SUPPORT
from tools.tests import TEST_MBED_LIB


LIBRARIES = [
    # RPC
    {
        "id": "rpc",
        "source_dir": MBED_RPC,
        "build_dir": RPC_LIBRARY,
        "dependencies": [MBED_LIBRARIES],
    },

    # USB Device libraries
    {
        "id": "usb",
        "source_dir": USB,
        "build_dir": USB_LIBRARIES,
        "dependencies": [MBED_LIBRARIES],
    },

    # DSP libraries
    {
        "id": "dsp",
        "source_dir": [DSP_ABSTRACTION, DSP_CMSIS],
        "build_dir": DSP_LIBRARIES,
        "dependencies": [MBED_LIBRARIES]
    },

    # Unit Testing library
    {
        "id": "cpputest",
        "source_dir": [CPPUTEST_SRC, CPPUTEST_PLATFORM_SRC,
                       CPPUTEST_TESTRUNNER_SCR],
        "build_dir": CPPUTEST_LIBRARY,
        "dependencies": [MBED_LIBRARIES],
        'inc_dirs': [CPPUTEST_INC, CPPUTEST_PLATFORM_INC,
                     CPPUTEST_TESTRUNNER_INC, TEST_MBED_LIB],
        'inc_dirs_ext': [CPPUTEST_INC_EXT],
        'macros': ["CPPUTEST_USE_MEM_LEAK_DETECTION=0",
                   "CPPUTEST_USE_STD_CPP_LIB=0", "CPPUTEST=1"],
    },
]


LIBRARY_MAP = dict([(library['id'], library) for library in LIBRARIES])


class Library(object):
    """A library representation that allows for querying of support"""
    def __init__(self, lib_id):
        lib = LIBRARY_MAP[lib_id]
        self.supported = lib.get("supported", DEFAULT_SUPPORT)
        self.dependencies = lib.get("dependencies", None)
        # Include dirs required by library build
        self.inc_dirs = lib.get("inc_dirs", None)
        # Include dirs required by others to use with this library
        self.inc_dirs_ext = lib.get("inc_dirs_ext", None)
        # Additional macros you want to define when building library
        self.macros = lib.get("macros", None)

        self.source_dir = lib["source_dir"]
        self.build_dir = lib["build_dir"]

    def is_supported(self, target, toolchain):
        """Check if a target toolchain combination is supported

        Positional arguments:
        target - the MCU or board
        toolchain - the compiler
        """
        if not hasattr(self, 'supported'):
            return True
        return (target.name in self.supported) and \
            (toolchain in self.supported[target.name])
