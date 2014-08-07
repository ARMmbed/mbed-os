#!/usr/bin/env python

"""
Travis-CI build script

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

import os
import sys

################################################################################
# Configure builds here
# "libs" can contain "dsp", "rtos", "eth", "usb_host", "usb", "ublox", "fat"

build_list = (
    { "target": "LPC1768",       "toolchains": "GCC_ARM", "libs": ["dsp", "rtos", "eth", "usb_host", "usb", "ublox"] },
    { "target": "LPC2368",       "toolchains": "GCC_ARM" },
    { "target": "LPC11U24",      "toolchains": "GCC_ARM", "libs": ["dsp", "rtos"]},
    { "target": "GHI_MBUINO",    "toolchains": "GCC_ARM"},

    { "target": "LPC11U24_301",  "toolchains": "GCC_ARM" },
    { "target": "NUCLEO_F103RB", "toolchains": "GCC_ARM"},

    { "target": "NUCLEO_F401RE", "toolchains": "GCC_ARM", "libs": ["dsp", "rtos"]},
    { "target": "LPC1114",       "toolchains": "GCC_ARM", "libs": ["dsp", "rtos"] },
    { "target": "LPC11U35_401",  "toolchains": "GCC_ARM", "libs": ["dsp", "rtos"] },
    { "target": "UBLOX_C027",    "toolchains": "GCC_ARM", "libs": ["dsp", "rtos"] },
    { "target": "LPC11U35_501",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "LPC11U68",      "toolchains": "GCC_ARM", "libs": ["dsp", "rtos"] },

    { "target": "KL05Z",         "toolchains": "GCC_ARM", "libs": ["dsp", "rtos"] },
    { "target": "KL25Z",         "toolchains": "GCC_ARM", "libs": ["dsp", "rtos", "usb"] },
    { "target": "KL46Z",         "toolchains": "GCC_ARM", "libs": ["dsp", "rtos", "usb"] },
    { "target": "K20D50M",       "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "K64F",          "toolchains": "GCC_ARM", "libs": ["dsp", "rtos", "usb"] },
    { "target": "LPC4088",       "toolchains": "GCC_ARM", "libs": ["dsp", "rtos", "usb"] },
    { "target": "ARCH_PRO",      "toolchains": "GCC_ARM", "libs": ["dsp", "rtos"] },
)

################################################################################
# Driver

def run_builds(dry_run):
    for build in build_list:
        toolchain_list = build["toolchains"]
        if type(toolchain_list) != type([]): toolchain_list = [toolchain_list]
        for toolchain in toolchain_list:
            cmdline = "python workspace_tools/build.py -m %s -t %s -c " % (build["target"], toolchain)
            libs = build.get("libs", [])
            if libs:
                cmdline = cmdline + " ".join(["--" + l for l in libs])
            print "Executing: " + cmdline
            if not dry_run:
                if os.system(cmdline) != 0:
                    sys.exit(1)

if __name__ == "__main__":
    run_builds("-s" in sys.argv)
