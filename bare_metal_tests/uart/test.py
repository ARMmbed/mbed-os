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

"""! UART TEST FOR GREEN TEA

  TEST SCENARIO
  This part of the test is executed on the host (PC).
  
  Actions performed by the script:
  - build mbed part of the UART test,
  - request user to flash and reset the mbed board,
  - perform uart communication test,
  - report test status.
  
  Test scenario:
  After startup script will try to communicate via UART with the mbed device.
  This part of the test waits for the expected message and responds with
  the same message. 
  4 test messages should be received. Last message ("PASSED") informs
  that on the mbed side also all messages have been received.
 
  To perform this test with serial port baud rate different than default (9600),
  call host test with appropriate --baudrate parameter.
  
  Example usage:
      test.py -m K64F -t GCC_ARM -b 9600
  This command will run test on K64F platfom using GCC_ARM toolchain and
  serial connection speed equal to 9600 b/s.
  
  For more options type:
      test.py -h
"""

RETRY_LIMIT = 10

def serial_cleanup(serial_port):
    """! Serial port cleanup handler.
         This function closes opened serial port.
         
    @param serial_port serial port.
    """
    if (serial_port != None and serial_port.isOpen() == True):
        serial_port.close()

def run_command(cmd):
    """! Runs command and prints proc stdout on screen.
         It is used to build the test.
         
    @param cmd command to be executed.
    @return image path.
    """
    image_path = str()
    
    # Remove build log file if exists
    try:
        os.remove('build.log')
    except OSError:
        pass
    
    try:
        p = Popen(cmd, stdout=sys.stdout)
    except OSError:
        logging.exception ('Command can not be executed.')
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

def build_test(target, toolchain, clean_flag, baudrate):
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
          '--build-data build.log %s -D BAUD_RATE=%s' \
          % (toolchain, target, clean, baudrate)
    
    return run_command(cmd)

def serial_write_line(serial_port, line):
    """Write line to the serial port.
    
    @param line line to be written.
    """
    serial_port.write(line)
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
            line += c
            if (c == '\0'):
                break
    logging.debug('[RECV] %s' % line)
    return line

def wait_for_message_and_respond(serial_port, msg):
    """! This function waits for the message from the mbed device.
         If valid message has been obtained, then it responds with
         the same message. If invalid message has been received, then
         it responds with 'RETRY\0'.
         Note: First received message may be incomplete and is used for
             synchronisation.
             
         @param msg expected message
    """
    for x in range(0, RETRY_LIMIT):
        line = serial_read_line(serial_port)
        if (line == msg):
            logging.info('Expected string has been received from the mbed device.')
            serial_write_line(serial_port, line)
            return True
        else:
            # if the line is unexpected (corrupted) and nul terminator and send,
            # this will trigger mbed part to resend the test message.
            if not line.endswith('\0'):
                line += '\0'
            serial_write_line(serial_port, line)
        
    return False

def main():
    serial_port = None
    sel_platform_id = None
    muts_list = None

    # List of expected messages from the mbed device.
    message_list = ['x\0', 
                    'mbed\0',
                    'Green Tea UART bare metal test.\0', 
                    'Long Message-Long Message-Long Message-Long Message-Long Message-Long Message-Long Message-Long Message-Long Message--.\0',
                    'PASSED\0']

    # Prepare required/optional arguments.
    parser = argparse.ArgumentParser(description='Execute UART bare metal test to check if platform is ready to use green tea.')
    parser.add_argument("-m", "--target", help="Target id", required=True)
    parser.add_argument("-t", "--toolchain", help="Compile toolchain. Example: ARM, GCC_ARM, IAR.", default='GCC_ARM')
    parser.add_argument("-b", "--baudrate", help="Serial port baud rate.", default=9600)
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
    logging.info('Selected serial port baud rate: %s b/s.' % (args.baudrate))

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
    build_test(args.target, args.toolchain, args.clean, args.baudrate)

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
        serial_port = serial.Serial(port=muts_list[sel_platform_id]['serial_port'],
                                    baudrate=args.baudrate,
                                    parity=serial.PARITY_NONE,
                                    stopbits=serial.STOPBITS_ONE,
                                    bytesize=serial.EIGHTBITS)

        # Register cleanup exit handler
        atexit.register(serial_cleanup, serial_port)
    
    except (ValueError, serial.SerialException):
        logging.exception ('Could not open serial port.')
        sys.exit()

    # Perform serial comunication test with the mbed device.
    for msg in message_list:
        if not wait_for_message_and_respond(serial_port, msg):
            logging.error("Expected message has NOT been received by the host.")
            sys.exit()

    # If we are here, then all expected messages have been received from the mbed device.
    # Assume that last message - "PASSED" only inform that on mbed device side also
    # all responses from host were successfully received.
    logging.info('Test passed!')
    sys.exit()

if  __name__ =='__main__':
    main()
    