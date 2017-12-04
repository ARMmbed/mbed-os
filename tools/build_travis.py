#!/usr/bin/env python2

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
# "libs" can contain "dsp", "usb"

build_list = (
    { "target": "LPC1768",           "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "LPC11U24",          "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "OC_MBUINO",         "toolchains": "GCC_ARM", "libs": []  },

    { "target": "LPC11U24_301",      "toolchains": "GCC_ARM", "libs": [] },

    { "target": "B96B_F446VE",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_L053R8",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_L152RE",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F030R8",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F031K6",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F042K6",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F070RB",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F072RB",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F091RC",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F103RB",     "toolchains": "GCC_ARM" },
    { "target": "NUCLEO_F207ZG",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "NUCLEO_F302R8",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F303K8",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F303RE",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F303ZE",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "NUCLEO_F334R8",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F401RE",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "NUCLEO_F410RB",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F411RE",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "NUCLEO_F412ZG",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_L432KC",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_L476RG",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_L011K4",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_L031K6",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_L073RZ",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NUCLEO_F429ZI",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "NUCLEO_F446RE",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "NUCLEO_F446ZE",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "NUCLEO_F746ZG",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "NUCLEO_F767ZI",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "NUCLEO_L496ZG",     "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "MOTE_L152RC",     "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "ELMO_F411RE",       "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "MTS_MDOT_F405RG",   "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "MTS_MDOT_F411RE",   "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "MTS_DRAGONFLY_F411RE", "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "ARCH_MAX",          "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "DISCO_F051R8",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "DISCO_F303VC",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "DISCO_F334C8",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "DISCO_F401VC",      "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "DISCO_F407VG",      "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "DISCO_F413ZH",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "DISCO_F429ZI",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "DISCO_F469NI",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "DISCO_F746NG",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "DISCO_F769NI",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "DISCO_L475VG_IOT01A", "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "DISCO_L476VG",        "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "DISCO_L072CZ_LRWAN1", "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },

    { "target": "LPC1114",           "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "LPC11U35_401",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "UBLOX_C027",        "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "LPC11U35_501",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "LPC11U68",          "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "LPC11U37H_401",     "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "KL05Z",             "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "KL25Z",             "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "KL27Z",             "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "KL43Z",             "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "KL46Z",             "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "K20D50M",           "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "TEENSY3_1",         "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "K64F",              "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "K22F",              "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "LPC4088",           "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "ARCH_PRO",          "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "LPC1549",           "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NRF51822",          "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "DELTA_DFCM_NNN40",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NRF51_DK",          "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "NRF51_MICROBIT",    "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "EFM32ZG_STK3200",   "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "EFM32HG_STK3400",   "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "EFM32LG_STK3600",   "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "EFM32GG_STK3700",   "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "EFM32WG_STK3800",   "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
    { "target": "EFM32PG_STK3401",   "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "MAXWSNENV",    "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "MAX32600MBED", "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "MAX32620HSP",  "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "SAMR21G18A",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "SAMD21J18A",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "SAMD21G18A",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
    { "target": "SAML21J18A",  "toolchains": "GCC_ARM", "libs": ["dsp"] },

    { "target": "NUMAKER_PFM_NUC472",   "toolchains": "GCC_ARM",    "libs": ["dsp", "usb"] },
    { "target": "NUMAKER_PFM_M453",     "toolchains": "GCC_ARM",    "libs": ["dsp", "usb"] },
    { "target": "NUMAKER_PFM_M487",     "toolchains": "GCC_ARM",    "libs": ["dsp", "usb"] },
 )

################################################################################
# Configure example test building (linking against external mbed SDK libraries)

linking_list = [
    {"target": "LPC1768",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_15", "MBED_16", "MBED_17"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "K64F",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "K22F",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "KL43Z",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUCLEO_F446RE",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUCLEO_F446ZE",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUCLEO_F401RE",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"], 
             }
    },
    {"target": "NUCLEO_F411RE",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUCLEO_F412ZG",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
             }
    },
    {"target": "NUCLEO_F429ZI",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUCLEO_F207ZG",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUCLEO_F746ZG",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUCLEO_F767ZI",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUCLEO_L476RG",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
             }
    },
    {"target": "DISCO_F429ZI",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
             }
    },
    {"target": "DISCO_F407VG",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
     {"target": "DISCO_F413ZH",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               }
     },
    {"target": "NUCLEO_F303ZE",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
             }
    },
    {"target": "DISCO_L475VG_IOT01A",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "DISCO_L476VG",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "DISCO_L072CZ_LRWAN1",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
              }
    },
    {"target": "NUMAKER_PFM_NUC472",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUMAKER_PFM_M453",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     },
    {"target": "NUMAKER_PFM_M487",
     "toolchains": "GCC_ARM",
     "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
               "usb"  : ["USB_1", "USB_2" ,"USB_3"],
               }
     }
    ]

################################################################################

# Driver

def run_builds(dry_run):
    for build in build_list:
        toolchain_list = build["toolchains"]
        if type(toolchain_list) != type([]): toolchain_list = [toolchain_list]
        for toolchain in toolchain_list:
            cmdline = "python tools/build.py -m %s -t %s -j 4 -c --silent "% (build["target"], toolchain)
            libs = build.get("libs", [])
            if libs:
                cmdline = cmdline + " ".join(["--" + l for l in libs])
            print "Executing: " + cmdline
            if not dry_run:
                if os.system(cmdline) != 0:
                    sys.exit(1)


def run_test_linking(dry_run):
    """ Function run make.py commands to build and link simple mbed SDK
        tests against few libraries to make sure there are no simple linking errors.
    """
    for link in linking_list:
        toolchain_list = link["toolchains"]
        if type(toolchain_list) != type([]):
            toolchain_list = [toolchain_list]
        for toolchain in toolchain_list:
            tests = link["tests"]
            # Call make.py for each test group for particular library
            for test_lib in tests:
                test_names = tests[test_lib]
                test_lib_switch = "--" + test_lib if test_lib else ""
                cmdline = "python tools/make.py -m %s -t %s -c --silent %s -n %s " % (link["target"], toolchain, test_lib_switch, ",".join(test_names))
                print "Executing: " + cmdline
                if not dry_run:
                    if os.system(cmdline) != 0:
                        sys.exit(1)

def run_test_testsuite(dry_run):
    cmdline = "python tools/singletest.py --version"
    print "Executing: " + cmdline
    if not dry_run:
        if os.system(cmdline) != 0:
            sys.exit(1)

if __name__ == "__main__":
    run_builds("-s" in sys.argv)
    run_test_linking("-s" in sys.argv)
    run_test_testsuite("-s" in sys.argv)
