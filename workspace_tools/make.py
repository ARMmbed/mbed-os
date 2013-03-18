#! /usr/bin/env python
"""
TEST BUILD & RUN
"""
import sys
from os.path import join, abspath, dirname
from shutil import copy
from time import sleep
from serial import Serial

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.append(ROOT)

from workspace_tools.options import get_default_options_parser
from workspace_tools.build_api import build_project
from workspace_tools.tests import TESTS, Test, TEST_MAP
from workspace_tools.paths import BUILD_DIR, RTOS_LIBRARIES


def args_error(parser, message):
    print "\n\n%s\n\n" % message
    parser.print_help()
    sys.exit()


if __name__ == '__main__':
    # Parse Options
    parser = get_default_options_parser()
    parser.add_option("-p", type="int", dest="program",
                      help="The index of the desired test program: [0-%d]" % (len(TESTS)-1))
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose",
                      default=False, help="Verbose diagnostic output")
    
    # Local run
    parser.add_option("-d", "--disk", dest="disk",
                      default=None, help="The mbed disk")
    parser.add_option("-s", "--serial", dest="serial",
                      default=None, help="The mbed serial port")
    parser.add_option("-b", "--baud", type="int", dest="baud",
                      default=None, help="The mbed serial baud rate")
    
    # Ideally, all the tests with a single "main" thread can be run with, or
    # without the rtos
    parser.add_option("--rtos", action="store_true", dest="rtos",
                      default=False, help="Link to the rtos")
    
    (options, args) = parser.parse_args()
    
    # Program Number
    p = options.program
    if p is None or (p < 0) or (p > (len(TESTS)-1)):
        message = "[ERROR] You have to specify one of the following tests:\n"
        descriptions = [(test.n, test.description) for test in TEST_MAP.values()]
        descriptions.sort()
        message += '\n'.join(["  [%2d] %s " % d for d in descriptions])
        args_error(parser, message)
    
    # Target
    if options.mcu is None :
        args_error(parser, "[ERROR] You should specify an MCU")
    mcu = options.mcu
    
    # Toolchain
    if options.tool is None:
        args_error(parser, "[ERROR] You should specify a TOOLCHAIN")
    toolchain = options.tool
    
    # Test
    test = Test(p)
    if not test.is_supported(mcu, toolchain):
        print 'The selected test is not supported on target %s with toolchain %s' % (mcu, toolchain)
        sys.exit()
    
    # RTOS
    if options.rtos:
        test.dependencies.append(RTOS_LIBRARIES)
    
    build_dir = join(BUILD_DIR, "test", mcu, toolchain, test.id)
    
    try:
        bin = build_project(test.source_dir, build_dir, mcu, toolchain,
                            test.dependencies, clean=options.clean, verbose=options.verbose)
        print 'Image: %s' % bin
        
        if options.disk:
            # Simple copy to the mbed disk
            copy(bin, options.disk)
        
        if options.serial:
            if options.mcu in ["KL25Z", "LPC812"]:
                # We do not have a flash disk where to store the image, we write
                # it directly on the target chip, therefore we need to
                # disconnect the interface: wait for the device to enumerate
                # again
                copy_time = 4
            else:
                copy_time = 1.5 
            sleep(copy_time)
            serial = Serial(options.serial, timeout = 1)
            if options.baud:
                serial.setBaudrate(options.baud)
            serial.flushInput()
            serial.flushOutput()
            serial.sendBreak()
            
            while True:
                c = serial.read(512)
                sys.stdout.write(c)
                sys.stdout.flush()
    
    except KeyboardInterrupt, e:
        print "\n[CTRL+c] exit"
    except Exception,e:
        print "[ERROR] %s" % str(e)
