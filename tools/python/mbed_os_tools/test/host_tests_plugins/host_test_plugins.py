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

import os
import sys
import platform

from os import access, F_OK
from sys import stdout
from time import sleep
from subprocess import call

from ... import detect
from ..host_tests_logger import HtrunLogger


class HostTestPluginBase:
    """! Base class for all plugins used with host tests
    """
    ###########################################################################
    # Interface:
    ###########################################################################

    ###########################################################################
    # Interface attributes defining plugin name, type etc.
    ###########################################################################
    name = "HostTestPluginBase" # Plugin name, can be plugin class name
    type = "BasePlugin"         # Plugin type: ResetMethod, CopyMethod etc.
    capabilities = []           # Capabilities names: what plugin can achieve
                                # (e.g. reset using some external command line tool)
    required_parameters = []    # Parameters required for 'kwargs' in plugin APIs: e.g. self.execute()
    stable = False              # Determine if plugin is stable and can be used

    def __init__(self):
        """ ctor
        """
        # Setting Host Test Logger instance
        ht_loggers = {
            'BasePlugin' : HtrunLogger('PLGN'),
            'CopyMethod' : HtrunLogger('COPY'),
            'ResetMethod' : HtrunLogger('REST'),
        }
        self.plugin_logger = ht_loggers.get(self.type, ht_loggers['BasePlugin'])

    ###########################################################################
    # Interface methods
    ###########################################################################

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        return False

    def execute(self, capability, *args, **kwargs):
        """! Executes capability by name
        @param capability Capability name
        @param args Additional arguments
        @param kwargs Additional arguments
        @details Each capability e.g. may directly just call some command line program or execute building pythonic function
        @return Capability call return value
        """
        return False

    def is_os_supported(self, os_name=None):
        """!
        @return Returns true if plugin works (supportes) under certain OS
        @os_name String describing OS.
                 See self.mbed_os_support() and self.mbed_os_info()
        @details In some cases a plugin will not work under particular OS
                 mainly because command / software used to implement plugin
                 functionality is not available e.g. on MacOS or Linux.
        """
        return True

    ###########################################################################
    # Interface helper methods - overload only if you need to have custom behaviour
    ###########################################################################
    def print_plugin_error(self, text):
        """! Function prints error in console and exits always with False
        @param text Text to print
        """
        self.plugin_logger.prn_err(text)
        return False

    def print_plugin_info(self, text, NL=True):
        """! Function prints notification in console and exits always with True
        @param text Text to print
        @param NL Deprecated! Newline will be added behind text if this flag is True
        """

        self.plugin_logger.prn_inf(text)
        return True

    def print_plugin_char(self, char):
        """ Function prints char on stdout
        """
        stdout.write(char)
        stdout.flush()
        return True

    def check_mount_point_ready(self, destination_disk, init_delay=0.2, loop_delay=0.25, target_id=None, timeout=60):
        """! Waits until destination_disk is ready and can be accessed by e.g. copy commands
        @return True if mount point was ready in given time, False otherwise
        @param destination_disk Mount point (disk) which will be checked for readiness
        @param init_delay - Initial delay time before first access check
        @param loop_delay - polling delay for access check
        @param timeout Mount point pooling timeout in seconds
        """

        if target_id:
            # Wait for mount point to appear with mbed-ls
            # and if it does check if mount point for target_id changed
            # If mount point changed, use new mount point and check if its ready (os.access)
            new_destination_disk = destination_disk

            # Sometimes OSes take a long time to mount devices (up to one minute).
            # Current pooling time: 120x 500ms = 1 minute
            self.print_plugin_info("Waiting up to %d sec for '%s' mount point (current is '%s')..."% (timeout, target_id, destination_disk))
            timeout_step = 0.5
            timeout = int(timeout / timeout_step)
            for i in range(timeout):
                # mbed_os_tools.detect.create() should be done inside the loop.
                # Otherwise it will loop on same data.
                mbeds = detect.create()
                mbed_list = mbeds.list_mbeds() #list of mbeds present
                # get first item in list with a matching target_id, if present
                mbed_target = next((x for x in mbed_list if x['target_id']==target_id), None)

                if mbed_target is not None:
                    # Only assign if mount point is present and known (not None)
                    if 'mount_point' in mbed_target and mbed_target['mount_point'] is not None:
                        new_destination_disk = mbed_target['mount_point']
                        break
                sleep(timeout_step)

            if new_destination_disk != destination_disk:
                # Mount point changed, update to new mount point from mbed-ls
                self.print_plugin_info("Mount point for '%s' changed from '%s' to '%s'..."% (target_id, destination_disk, new_destination_disk))
                destination_disk = new_destination_disk

        result = True
        # Check if mount point we've promoted to be valid one (by optional target_id check above)
        # Let's wait for 30 * loop_delay + init_delay max
        if not access(destination_disk, F_OK):
            self.print_plugin_info("Waiting for mount point '%s' to be ready..."% destination_disk, NL=False)
            sleep(init_delay)
            for i in range(30):
                if access(destination_disk, F_OK):
                    result = True
                    break
                sleep(loop_delay)
                self.print_plugin_char('.')
            else:
                self.print_plugin_error("mount {} is not accessible ...".format(destination_disk))
                result = False
        return (result, destination_disk)

    def check_serial_port_ready(self, serial_port, target_id=None, timeout=60):
        """! Function checks (using mbed-ls) and updates serial port name information for DUT with specified target_id.
        If no target_id is specified function returns old serial port name.
        @param serial_port Current serial port name
        @param target_id Target ID of a device under test which serial port will be checked and updated if needed
        @param timeout Serial port pooling timeout in seconds
        @return Tuple with result (always True) and serial port read from mbed-ls
        """
        # If serial port changed (check using mbed-ls), use new serial port
        new_serial_port = None

        if target_id:
            # Sometimes OSes take a long time to mount devices (up to one minute).
            # Current pooling time: 120x 500ms = 1 minute
            self.print_plugin_info("Waiting up to %d sec for '%s' serial port (current is '%s')..."% (timeout, target_id, serial_port))
            timeout_step = 0.5
            timeout = int(timeout / timeout_step)
            for i in range(timeout):
                # mbed_os_tools.detect.create() should be done inside the loop. Otherwise it will loop on same data.
                mbeds = detect.create()
                mbed_list = mbeds.list_mbeds() #list of mbeds present
                # get first item in list with a matching target_id, if present
                mbed_target = next((x for x in mbed_list if x['target_id']==target_id), None)

                if mbed_target is not None:
                    # Only assign if serial port is present and known (not None)
                    if 'serial_port' in mbed_target and mbed_target['serial_port'] is not None:
                        new_serial_port = mbed_target['serial_port']
                        if new_serial_port != serial_port:
                            # Serial port changed, update to new serial port from mbed-ls
                            self.print_plugin_info("Serial port for tid='%s' changed from '%s' to '%s'..." % (target_id, serial_port, new_serial_port))
                        break
                sleep(timeout_step)
        else:
            new_serial_port = serial_port

        return new_serial_port

    def check_parameters(self, capability, *args, **kwargs):
        """! This function should be ran each time we call execute() to check if none of the required parameters is missing
        @param capability Capability name
        @param args Additional parameters
        @param kwargs Additional parameters
        @return Returns True if all parameters are passed to plugin, else return False
        """
        missing_parameters = []
        for parameter in self.required_parameters:
            if parameter not in kwargs:
                missing_parameters.append(parameter)
        if len(missing_parameters):
            self.print_plugin_error("execute parameter(s) '%s' missing!"% (', '.join(missing_parameters)))
            return False
        return True

    def run_command(self, cmd, shell=True, stdin = None):
        """! Runs command from command line.
        @param cmd Command to execute
        @param shell True if shell command should be executed (eg. ls, ps)
        @param stdin A custom stdin for the process running the command (defaults to None)
        @details Function prints 'cmd' return code if execution failed
        @return True if command successfully executed
        """
        result = True
        try:
            ret = call(cmd, shell=shell, stdin=stdin)
            if ret:
                self.print_plugin_error("[ret=%d] Command: %s"% (int(ret), cmd))
                return False
        except Exception as e:
            result = False
            self.print_plugin_error("[ret=%d] Command: %s"% (int(ret), cmd))
            self.print_plugin_error(str(e))
        return result

    def mbed_os_info(self):
        """! Returns information about host OS
        @return Returns tuple with information about OS and host platform
        """
        result = (os.name,
                  platform.system(),
                  platform.release(),
                  platform.version(),
                  sys.platform)
        return result

    def mbed_os_support(self):
        """! Function used to determine host OS
        @return Returns None if host OS is unknown, else string with name
        @details This function should be ported for new OS support
        """
        result = None
        os_info = self.mbed_os_info()
        if (os_info[0] == 'nt' and os_info[1] == 'Windows'):
            result = 'Windows7'
        elif (os_info[0] == 'posix' and os_info[1] == 'Linux' and ('Ubuntu' in os_info[3])):
            result = 'Ubuntu'
        elif (os_info[0] == 'posix' and os_info[1] == 'Linux'):
            result = 'LinuxGeneric'
        elif (os_info[0] == 'posix' and os_info[1] == 'Darwin'):
            result = 'Darwin'
        return result
