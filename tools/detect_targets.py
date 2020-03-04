#! /usr/bin/env python2
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
from __future__ import print_function
import sys
import os
import re

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.options import get_default_options_parser

# Check: Extra modules which are required by core test suite
from tools.utils import check_required_modules
check_required_modules(['prettytable'])

# Imports related to mbed build api
from tools.build_api import mcu_toolchain_matrix
from tools.test_api import get_autodetected_MUTS_list
from tools.test_api import get_module_avail
from argparse import ArgumentParser

try:
    import mbed_lstools
except:
    pass

def main():
    """Entry Point"""
    try:
        # Parse Options
        parser = ArgumentParser()

        parser.add_argument("-S", "--supported-toolchains",
                            action="store_true",
                            dest="supported_toolchains",
                            default=False,
                            help="Displays supported matrix of"
                            " targets and toolchains")

        parser.add_argument('-f', '--filter',
                            dest='general_filter_regex',
                            default=None,
                            help='Filter targets')

        parser.add_argument("-v", "--verbose",
                            action="store_true",
                            dest="verbose",
                            default=False,
                            help="Verbose diagnostic output")

        options = parser.parse_args()

        # Only prints matrix of supported toolchains
        if options.supported_toolchains:
            print(mcu_toolchain_matrix(
                platform_filter=options.general_filter_regex))
            exit(0)

        # If auto_detect attribute is present, we assume other auto-detection
        # parameters like 'toolchains_filter' are also set.
        muts = get_autodetected_MUTS_list()

        mcu_filter = options.general_filter_regex or ".*"

        count = 0
        for mut in muts.values():
            if re.match(mcu_filter, mut['mcu'] or "Unknown"):
                interface_version = get_interface_version(mut['disk'])
                print("")
                print("[mbed] Detected %s, port %s, mounted %s, interface "
                      "version %s:" %
                      (mut['mcu'], mut['port'], mut['disk'], interface_version))
                print("[mbed] Supported toolchains for %s" % mut['mcu'])
                print(mcu_toolchain_matrix(platform_filter=mut['mcu']))
                count += 1

        if count == 0:
            print("[mbed] No mbed targets were detected on your system.")

    except KeyboardInterrupt:
        print("\n[CTRL+c] exit")
    except Exception as exc:
        import traceback
        traceback.print_exc(file=sys.stdout)
        print("[ERROR] %s" % str(exc))
        sys.exit(1)
        
def get_interface_version(mount_point):
    """ Function returns interface version from the target mounted on the specified mount point
    
        mount_point can be acquired via the following:
            muts = get_autodetected_MUTS_list()
            for mut in muts.values():
                mount_point = mut['disk']
                    
        @param mount_point Name of disk where platform is connected to host machine.
    """
    if get_module_avail('mbed_lstools'):
        try :
            mbedls = mbed_lstools.create()
            mbeds = mbedls.list_mbeds(unique_names=True, read_details_txt=True)
            
            for mbed in mbeds:
                if mbed['mount_point'] == mount_point:
            
                    if 'daplink_version' in mbed:
                        return mbed['daplink_version']           
        except :
            return 'unknown'
        
    return 'unknown'

if __name__ == '__main__':
    main()
