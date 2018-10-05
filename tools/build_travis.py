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
from __future__ import print_function, division, absolute_import

import os
import sys

from argparse import ArgumentParser

################################################################################
# Configure builds here
# "libs" can contain "dsp", "usb"

build_list = [
    {
    "STM":
        (
        { "target": "B96B_F446VE",       "toolchains": "GCC_ARM", "libs": ["dsp"] },
        { "target": "NUCLEO_L053R8",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
        { "target": "MTB_RAK811",        "toolchains": "GCC_ARM"},
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
        { "target": "STEVAL_3DP001V1",   "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
        { "target": "NUCLEO_F410RB",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
        { "target": "NUCLEO_F411RE",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
        { "target": "NUCLEO_F412ZG",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
        { "target": "NUCLEO_F413ZH",     "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
        { "target": "NUCLEO_L432KC",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
        { "target": "MTB_ADV_WISE_1510", "toolchains": "GCC_ARM", "libs": ["dsp"] },
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

        # module manufacturer : muRata
        { "target": "MTB_MURATA_ABZ",    "toolchains": "GCC_ARM", "libs": [] },
        ),
    },

    {
        "NXP":
        (
            { "target": "LPC1768",           "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
            { "target": "LPC11U24",          "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "OC_MBUINO",         "toolchains": "GCC_ARM", "libs": []  },

            { "target": "LPC11U24_301",      "toolchains": "GCC_ARM", "libs": [] },
            { "target": "LPC1114",           "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "LPC11U35_401",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "UBLOX_C027",        "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "LPC11U35_501",      "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "LPC11U68",          "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "LPC11U37H_401",     "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "LPC1549",           "toolchains": "GCC_ARM", "libs": ["dsp"] },
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
        )
    },

    {
        "NORDIC":
        (
            { "target": "NRF51822",          "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "DELTA_DFCM_NNN40",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "NRF51_DK",          "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "NRF51_MICROBIT",    "toolchains": "GCC_ARM", "libs": ["dsp"] },
        )
    },

    {
        "SILICON_LABS":
        (
            { "target": "EFM32ZG_STK3200",   "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "EFM32HG_STK3400",   "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
            { "target": "EFM32LG_STK3600",   "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
            { "target": "EFM32GG_STK3700",   "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
            { "target": "EFM32WG_STK3800",   "toolchains": "GCC_ARM", "libs": ["dsp", "usb"] },
            { "target": "EFM32PG_STK3401",   "toolchains": "GCC_ARM", "libs": ["dsp"] },
        )
    },

    {
        "ATMEL":
        (
            { "target": "SAMR21G18A",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "SAMD21J18A",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "SAMD21G18A",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
            { "target": "SAML21J18A",  "toolchains": "GCC_ARM", "libs": ["dsp"] },
        )
    },


    {
        "NUVOTON":
        (
            { "target": "NUMAKER_PFM_NUC472",   "toolchains": "GCC_ARM",    "libs": ["dsp", "usb"] },
            { "target": "NUMAKER_PFM_M453",     "toolchains": "GCC_ARM",    "libs": ["dsp", "usb"] },
            { "target": "NUMAKER_PFM_M487",     "toolchains": "GCC_ARM",    "libs": ["dsp", "usb"] },
        )
    },


    {
        "RENESAS":
        (
            { "target": "RZ_A1H",    "toolchains": "GCC_ARM" },
            { "target": "GR_LYCHEE", "toolchains": "GCC_ARM" },
        )
    }
]

################################################################################
# Configure example test building (linking against external mbed SDK libraries)

linking_list = [
    {
    "NXP": (
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
         )
    },

    {
        "STM": (
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
            {"target": "NUCLEO_F413ZH",
             "toolchains": "GCC_ARM",
             "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
                       "usb"  : ["USB_1", "USB_2" ,"USB_3"],
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
            {"target": "MTB_MURATA_ABZ",
             "toolchains": "GCC_ARM",
             "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
                      }
            },
        )
    },
    {
        "NUVOTON": (
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
        )
    },
    {
        "RENESAS":
        (
            {
             "target": "RZ_A1H",
             "toolchains": "GCC_ARM",
             "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
                      }
            },
            {
             "target": "GR_LYCHEE",
             "toolchains": "GCC_ARM",
             "tests": {""     : ["MBED_2", "MBED_10", "MBED_11", "MBED_16"],
                      }
            },
        )
    }
]

################################################################################

# Driver

def run_builds(dry_run, vendor):
    for vendor_list in build_list:
        if vendor in vendor_list:
            for build in vendor_list[vendor]:
                toolchain_list = build["toolchains"]
                if type(toolchain_list) != type([]): toolchain_list = [toolchain_list]
                for toolchain in toolchain_list:
                    cmdline = ("%s tools/build.py -m %s -t %s -c --silent "%
                               (sys.executable, build["target"], toolchain))
                    libs = build.get("libs", [])
                    if libs:
                        cmdline = cmdline + " ".join(["--" + l for l in libs])
                    print("Executing: %s" % cmdline)
                    if not dry_run:
                        if os.system(cmdline) != 0:
                            sys.exit(1)


def run_test_linking(dry_run, vendor):
    """ Function run make.py commands to build and link simple mbed SDK
        tests against few libraries to make sure there are no simple linking errors.
    """
    for vendor_list in linking_list:
        if vendor in vendor_list:
            for link in vendor_list[vendor]:
                toolchain_list = link["toolchains"]
                if type(toolchain_list) != type([]):
                    toolchain_list = [toolchain_list]
                for toolchain in toolchain_list:
                    tests = link["tests"]
                    # Call make.py for each test group for particular library
                    for test_lib in tests:
                        test_names = tests[test_lib]
                        test_lib_switch = "--" + test_lib if test_lib else ""
                        cmdline = ("%s tools/make.py -m %s -t %s -c --silent %s "
                                   "-n %s" % (sys.executable, link["target"],
                                               toolchain, test_lib_switch,
                                               ",".join(test_names)))
                        print("Executing: %s" % cmdline)
                        if not dry_run:
                            if os.system(cmdline) != 0:
                                sys.exit(1)

if __name__ == "__main__":
    parser = ArgumentParser()

    parser.add_argument("--vendor",
                        metavar="vendor",
                        type=str.upper,
                        help="Select a vendor to run travis tests"
                        )

    options = parser.parse_args()

    run_builds("-s" in sys.argv, options.vendor)
    run_test_linking("-s" in sys.argv, options.vendor)
