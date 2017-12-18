"""
mbed SDK
Copyright (c) 2017 ARM Limited

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
import time
import serial
import sys
import mbed_lstools
from subprocess import Popen, PIPE, STDOUT
import argparse
import json
import os
import logging
import atexit
import threading

"""! US TICKER TEST FOR GREEN TEA

  TEST SCENARIO
  This part of the test is executed on the host (PC).
  
  Actions performed by the script:
  - build mbed part of the US TICKER test,
  - request user to flash and reset the mbed board,
  - perform us ticker test,
  - report test status.
  
  For basic functionality of the Grean-Tea and utest frameworks us ticker is not
  required. Advanced usage (using of asynchronous test cases) requires
  running us ticker since utest framework uses Timeout class to execute call-backs
  after the specified time.  
  
  Test cases:
  - verify if us ticker counts ticks.
  - verify if us ticker interrupt scheduling works.
  - verify if us ticker interrupt re-scheduling works.
  - verify if us ticker frequency and counter width is valid (consistent with the new standards)
  - verify if us ticker frequency is consistent with declared frequency.
  - verify if attached call-back to Timeout object is called in the specified point in time.
  
  Note:
  This test verified enough of us ticker to enable greentea/utest.
  This is not a full unit/integration tests for us ticker.
  
  Example usage:
      test.py -m K64F -t GCC_ARM
  This command will run test on K64F platform using GCC_ARM toolchain.
  
  For more options type:
      test.py -h
"""

RETRY_LIMIT = 10

SYNC_OK = 0
SYNC_ERR = -1

def serial_cleanup(serial_port):
    """! Serial port clean-up handler.
         This function closes opened serial port.
         
    @param serial_port serial port.
    """
    if (serial_port != None and serial_port.isOpen() == True):
        serial_port.close()

def build_test(target, toolchain, clean_flag):
    """Build the mbed test.
    
    @param target target id.
    @param toolchain compile toolchain.
    @return image path.
    """
    logging.info('Building mbed UART test for green tea.')
    
    clean = ''
    if (clean_flag):
        clean = '-c'
    
    cmd = 'mbed compile -t %s -m %s --source . --source ../.. ' \
          '--build-data build.log %s' \
          % (toolchain, target, clean)
    
    image_path = str()
    
    # Remove build log file if exists
    try:
        os.remove('build.log')
    except OSError:
        pass
    
    try:
        p = Popen(cmd, stdout=sys.stdout)
    except OSError:
        logging.exception ('Command cannot be executed.')
        sys.exit()

    # Check process status.
    returncode = p.wait()
    if returncode != 0:
        logging.error('Error while executing command.')
        sys.exit()
    
    # Get image path   
    try:
        with open('build.log') as f:
            build_data = json.load(f)
            image_path = build_data['builds'][0]['bin']
    # Image path can not be found.
    except (KeyError, IndexError, IOError, AttributeError): 
        # If we are here, then build process has failed.
        logging.exception ('Unable to locate image file.')
        sys.exit()
       
    return image_path

def serial_write_line(serial_port, line):
    """Write line to the serial port.
    
    @param line line to be written.
    """
    serial_port.write(line+'\0')
    logging.debug('[SEND] %s' % line)

def serial_read_line(serial_port):
    """Read line from the serial port with timeout.
    
    @return string read from the serial port.
    """
    limit = 0
    line = str()
    while (True):
        if (serial_port.in_waiting == 0):
            time.sleep(0.001)
            limit += 1
            if (limit == RETRY_LIMIT):
                break
        else:
            limit = 0
            c = serial_port.read()
            if (c == '\0' or c == '\n'):
                break
            line += c
    logging.debug('[RECV] -%s-' % line)
    return line

def sync_host_dev(serial_port):
    """Synchronize host part with mbed part of the test.
    
    @param serial_port serial port.
    """
    for i in range(0, RETRY_LIMIT):
        serial_write_line(serial_port, '__sync')
        time.sleep(0.1)
        resp = serial_read_line(serial_port)
        if (resp == '__ready'):
            return SYNC_OK
    
    return SYNC_ERR

def test_commander(serial_port, test_support_functions):
    """Function handles test execution. Decode command from mbed part of the test
       and perform specific action. Command is encoded in JSON format. 
    
    @param serial_port serial port.
    @param test_support_functions dictionary with test case support functions.
    """
    timeout_time = -1
    
    while (True):

        line = serial_read_line(serial_port)

        if (timeout_time > 0 and int(time.time()) > timeout_time):
            logging.info('TEST TIMEOUT')
            sys.exit()

        # Assume that we received command (if reception timeout occurs, then we get empty string)
        try:
            # remove null terminator
            cmd = json.loads(line)
        except (ValueError):
            # If this is not a command in JSON format, then print it out
            if (line != ''):
                logging.info(line)
            continue
            
        if (cmd['cmd'] == 'timeout'):
            timeout_time = int(time.time() + 1) + int(cmd['val'])
            logging.info('Timeout: %s sec.\n' % cmd['val'])
            
        if (cmd['cmd'] == 'description'):
            logging.info('--- Test case ---')
            logging.info('Description: %s' % cmd['val'])
                
        if (cmd['cmd'] == 'host_test' and cmd['val'] != '(null)'):
            test_support_functions[cmd['val']](serial_port)

        if (cmd['cmd'] == 'assert'):
            logging.info(cmd['val'])

        if (cmd['cmd'] == 'status'):
            if(cmd['val'] == "PASSED"):
                logging.info('Result: PASSED\n')
            else:
                logging.info('Result: FAILED\n')
            
        if (cmd['cmd'] == 'done'):
            logging.info('--- Test execution status ---\nTOTAL EXECUTED TEST CASES: %s\nPASSED: %s\nFAILED: %s\n', 
                         cmd['val']['total'], cmd['val']['passed'], cmd['val']['failed'])
                
            if (int(cmd['val']['failed']) == 0 and int(cmd['val']['total']) == int(cmd['val']['passed'])):
                logging.info('All test cases executed successfully!')
                logging.info('TEST PASSED')
            else:
                logging.info('TEST FAILED')
            break

def support_freq_test(serial_port):
    """Support function for frequency test.
    
    @param serial_port serial port.
    """
    
    time.sleep(0.1)
    
    serial_write_line(serial_port, 'start')
    time.sleep(1)
    serial_write_line(serial_port, 'stop_')

def main():
    serial_port = None
    sel_platform_id = None
    muts_list = None
    
    test_support_functions = {
        "freq_test": support_freq_test,
    }

    # Prepare required/optional arguments.
    parser = argparse.ArgumentParser(description='Execute US ticker bare metal test to check if platform is ready to use green tea.')
    parser.add_argument("-m", "--target", help="Target id", required=True)
    parser.add_argument("-t", "--toolchain", help="Compile toolchain. Example: ARM, GCC_ARM, IAR.", default='GCC_ARM')
    parser.add_argument("-c", "--clean", action='store_true', help="Perform a clean build.")
    parser.add_argument("-v", "--verbose", action='store_true', help="Increase output verbosity.", default=0)
    args = parser.parse_args()
    
    # Set verbosity level (print transmission details or not)
    if (args.verbose):
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.INFO)

    # Create list of connected mbed devices.
    mbeds = mbed_lstools.create()
    muts_list = mbeds.list_mbeds()

    # Print platform and baud rate info.
    logging.info('Selected platform: %s.' % (args.target))

    # Print list of devices with details.
    logging.info('Available platforms: ')
    for i, mut in enumerate(muts_list):
        sel_str = ''
        if (mut['platform_name'] == args.target):
            sel_platform_id = i
            sel_str = '  --- SELECTED ---'
        logging.info('%-20s %-10s %-10s %-10s' % (mut['platform_name'], mut['serial_port'], mut['mount_point'], sel_str))

    # If our device is not on the list of connected mbed devices, then quit.
    if (sel_platform_id == None):
        logging.info('Provided platform - %s has not been found.' % (args.target))
        sys.exit()

    # Build the test.
    build_test(args.target, args.toolchain, args.clean)

    # Inform what to do next.
    logging.info('Perform the following steps: \n'
                 '1. Copy the binary file to the board.\n'
                 '2. Press the reset button on the mbed board to start the program.\n'
                 '3. Press Enter to start the test...\n')

    # Wait for signal to continue.
    raw_input('')

    # Indicate start of the communication test.
    logging.info('Starting test, please wait...')
    
    # Open serial port.
    try:
        serial_port = serial.Serial(port = muts_list[sel_platform_id]['serial_port'],
                                    baudrate = 9600,
                                    parity = serial.PARITY_NONE,
                                    stopbits = serial.STOPBITS_ONE,
                                    bytesize = serial.EIGHTBITS)

        # Register clean-up exit handler
        atexit.register(serial_cleanup, serial_port)
    
    except (ValueError, serial.SerialException):
        logging.exception ('Could not open serial port.')
        sys.exit()
        
    
    # Sync both parts of the test
    if(sync_host_dev(serial_port) == SYNC_ERR):
        logging.error('Host - Device synchronisation error.')
        sys.exit()
    
    logging.info('Host - Device synchronisation complete.')
    
    logging.info('Starting test.')
    
    # Start the test
    test_commander(serial_port, test_support_functions)
    
    sys.exit()

if  __name__ =='__main__':
    main()
    
