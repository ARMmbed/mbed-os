# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import sys
import json
from time import sleep
from serial import Serial, SerialException
from .. import host_tests_plugins, DEFAULT_BAUD_RATE


def flash_dev(disk=None,
              image_path=None,
              copy_method='default',
              port=None,
              program_cycle_s=4):
    """! Flash device using pythonic interface
    @param disk Switch -d <disk>
    @param image_path Switch -f <image_path>
    @param copy_method Switch -c <copy_method> (default: shell)
    @param port Switch -p <port>
    """
    if copy_method == 'default':
        copy_method = 'shell'
    result = False
    result = host_tests_plugins.call_plugin('CopyMethod',
                                            copy_method,
                                            image_path=image_path,
                                            serial=port,
                                            destination_disk=disk)
    sleep(program_cycle_s)
    return result

def reset_dev(port=None,
              disk=None,
              reset_type='default',
              reset_timeout=1,
              serial_port=None,
              baudrate=DEFAULT_BAUD_RATE,
              timeout=1,
              verbose=False):
    """! Reset device using pythonic interface
    @param port Switch -p <port>
    @param disk Switch -d <disk>
    @param reset_type Switch -r <reset_type>
    @param reset_timeout Switch -R <reset_timeout>
    @param serial_port Serial port handler, set to None if you want this function to open serial

    @param baudrate Serial port baudrate
    @param timeout Serial port timeout
    @param verbose Verbose mode
    """

    result = False
    if not serial_port:
        try:
            with Serial(port, baudrate=baudrate, timeout=timeout) as serial_port:
                result = host_tests_plugins.call_plugin('ResetMethod',
                                                        reset_type,
                                                        serial=serial_port,
                                                        disk=disk)
            sleep(reset_timeout)
        except SerialException as e:
            if verbose:
                print("%s" % (str(e)))
            result = False
    return result

def handle_send_break_cmd(port,
                          disk,
                          reset_type=None,
                          baudrate=None,
                          timeout=1,
                          verbose=False):
    """! Resets platforms and prints serial port output
        @detail Mix with switch -r RESET_TYPE and -p PORT for versatility
    """
    if not reset_type:
        reset_type = 'default'

    port_config = port.split(':')
    if len(port_config) == 2:
        # -p COM4:115200
        port = port_config[0]
        baudrate = int(port_config[1]) if not baudrate else baudrate
    elif len(port_config) == 3:
        # -p COM4:115200:0.5
        port = port_config[0]
        baudrate = int(port_config[1]) if not baudrate else baudrate
        timeout = float(port_config[2])

    # Use default baud rate value if not set
    if not baudrate:
        baudrate = DEFAULT_BAUD_RATE

    if verbose:
        print("mbedhtrun: serial port configuration: %s:%s:%s"% (port, str(baudrate), str(timeout)))

    try:
        serial_port = Serial(port, baudrate=baudrate, timeout=timeout)
    except Exception as e:
        print("mbedhtrun: %s" % (str(e)))
        print(json.dumps({
            "port" : port,
            "disk" : disk,
            "baudrate" : baudrate,
            "timeout" : timeout,
            "reset_type" : reset_type,
            }, indent=4))
        return False

    serial_port.flush()
    # Reset using one of the plugins
    result = host_tests_plugins.call_plugin('ResetMethod', reset_type, serial=serial_port, disk=disk)
    if not result:
        print("mbedhtrun: reset plugin failed")
        print(json.dumps({
            "port" : port,
            "disk" : disk,
            "baudrate" : baudrate,
            "timeout" : timeout,
            "reset_type" : reset_type
            }, indent=4))
        return False

    print("mbedhtrun: serial dump started (use ctrl+c to break)")
    try:
        while True:
            test_output = serial_port.read(512)
            if test_output:
                sys.stdout.write('%s'% test_output)
            if "{end}" in test_output:
                if verbose:
                    print()
                    print("mbedhtrun: stopped (found '{end}' terminator)")
                break
    except KeyboardInterrupt:
        print("ctrl+c break")

    serial_port.close()
    return True
