#! /usr/bin/env python
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

LIBRARIES BUILD
"""
import sys
from time import time
from os.path import join, abspath, dirname

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.append(ROOT)

from workspace_tools.toolchains import TOOLCHAINS
from workspace_tools.targets import TARGET_NAMES, TARGET_MAP
from workspace_tools.options import get_default_options_parser
from workspace_tools.build_api import build_mbed_libs, build_lib


if __name__ == '__main__':
    start = time()
    
    # Parse Options
    parser = get_default_options_parser()
    
    # Extra libraries
    parser.add_option("-r", "--rtos", action="store_true", dest="rtos",
                      default=False, help="Compile the rtos")
    parser.add_option("-e", "--eth", action="store_true", dest="eth",
                      default=False, help="Compile the ethernet library")
    parser.add_option("-U", "--usb_host", action="store_true", dest="usb_host",
                      default=False, help="Compile the USB Host library")
    parser.add_option("-u", "--usb", action="store_true", dest="usb",
                      default=False, help="Compile the USB Device library")
    parser.add_option("-d", "--dsp", action="store_true", dest="dsp",
                      default=False, help="Compile the DSP library")
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose",
                      default=False, help="Verbose diagnostic output")
    parser.add_option("-b", "--ublox", action="store_true", dest="ublox",
                      default=False, help="Compile the u-blox library")
    (options, args) = parser.parse_args()
    
    # Get target list
    if options.mcu:
        targets = [options.mcu]
    else:
        targets = TARGET_NAMES
    
    # Get toolchains list
    if options.tool:
        toolchains = [options.tool]
    else:
        toolchains = TOOLCHAINS
    
    # Get libraries list
    libraries = []
    
    # Additional Libraries
    if options.rtos:
        libraries.extend(["rtx", "rtos"])
    if options.eth:
        libraries.append("eth")
    if options.usb:
        libraries.append("usb")
    if options.usb_host:
        libraries.append("usb_host")
    if options.dsp:
        libraries.extend(["cmsis_dsp", "dsp"])
    if options.ublox:
        libraries.extend(["rtx", "rtos", "usb_host", "ublox"])
    
    # Build
    failures = []
    successes = []
    for toolchain in toolchains:
        for target in targets:
            id = "%s::%s" % (toolchain, target)
            try:
                mcu = TARGET_MAP[target]
                build_mbed_libs(mcu, toolchain, options=options.options,
                                verbose=options.verbose, clean=options.clean)
                for lib_id in libraries:
                    build_lib(lib_id, mcu, toolchain, options=options.options,
                              verbose=options.verbose, clean=options.clean)
                successes.append(id)
            except Exception, e:
                if options.verbose:
                    import sys, traceback
                    traceback.print_exc(file=sys.stdout)
                    sys.exit(1)
                
                failures.append(id)
                print e
    
    # Write summary of the builds
    print "\n\nCompleted in: (%.2f)s" % (time() - start)
    
    if successes:
        print "\n\nBuild successes:"
        print "\n".join(["  * %s" % s for s in successes])
    
    if failures:
        print "\n\nBuild failures:"
        print "\n".join(["  * %s" % f for f in failures])
