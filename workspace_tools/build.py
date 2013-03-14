#! /usr/bin/env python
"""
LIBRARIES BUILD
"""
import sys
from time import time
from os.path import join, abspath, dirname

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.append(ROOT)

from workspace_tools.toolchains import TARGETS, TOOLCHAINS
from workspace_tools.options import get_default_options_parser
from workspace_tools.build_api import build_mbed_libs, build_lib


if __name__ == '__main__':
    start = time()
    
    # Parse Options
    parser = get_default_options_parser()
    
    # Extra libraries
    parser.add_option("-r", "--rtos", action="store_true", dest="rtos",
                      default=False, help="Compile the rtos")
    parser.add_option("-b", "--debug", action="store_true", dest="debug",
                      default=False, help="Compile the debugging library")
    parser.add_option("-f", "--fatfs", action="store_true", dest="fatfs",
                      default=False, help="Compile the fatfs")
    parser.add_option("-e", "--eth", action="store_true", dest="eth",
                      default=False, help="Compile the ethernet library")
    parser.add_option("-V", "--vodafone", action="store_true", dest="vodafone",
                      default=False, help="Compile the Vodafone library")
    parser.add_option("-U", "--usb_host", action="store_true", dest="usb_host",
                      default=False, help="Compile the USB Host library")
    parser.add_option("-u", "--usb", action="store_true", dest="usb",
                      default=False, help="Compile the USB Device library")
    parser.add_option("-d", "--dsp", action="store_true", dest="dsp",
                      default=False, help="Compile the DSP library")
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose",
                      default=False, help="Verbose diagnostic output")
    
    (options, args) = parser.parse_args()
    
    # Get target list
    if options.mcu:
        targets = [options.mcu]
    else:
        targets = TARGETS
    
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
    if options.debug:
        libraries.append("debug")
    if options.fatfs:
        libraries.append("fatfs")
    if options.usb_host:
        libraries.append("usb_host")
    if options.eth:
        libraries.append("eth")
    if options.vodafone:
        libraries.append("vodafone")
    if options.usb:
        libraries.append("usb")
    if options.dsp:
        libraries.extend(["cmsis_dsp", "dsp"])
    
    # Build
    failures = []
    successes = []
    for toolchain in toolchains:
        for target in targets:
            id = "%s::%s" % (toolchain, target)
            try:
                build_mbed_libs(target, toolchain, verbose=options.verbose)
                for lib_id in libraries:
                    build_lib(lib_id, target, toolchain, verbose=options.verbose)
                successes.append(id)
            except Exception, e:
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
