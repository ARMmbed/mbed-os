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

import json
import os
from time import sleep
from .. import host_tests_plugins as ht_plugins
from ... import detect
from .. import DEFAULT_BAUD_RATE
from ..host_tests_logger import HtrunLogger


class Mbed:
    """! Base class for a host driven test
    @details This class stores information about things like disk, port, serial speed etc.
             Class is also responsible for manipulation of serial port between host and mbed device
    """
    def __init__(self, options):
        """ ctor
        """
        # For compatibility with old mbed. We can use command line options for Mbed object
        # or we can pass options directly from .
        self.options = options
        self.logger = HtrunLogger('MBED')
        # Options related to copy / reset mbed device
        self.port = self.options.port
        self.mcu = self.options.micro
        self.disk = self.options.disk
        self.target_id = self.options.target_id
        self.image_path = self.options.image_path.strip('"') if self.options.image_path is not None else ''
        self.copy_method = self.options.copy_method
        self.retry_copy = self.options.retry_copy
        self.program_cycle_s = float(self.options.program_cycle_s if self.options.program_cycle_s is not None else 2.0)
        self.polling_timeout = self.options.polling_timeout

        # Serial port settings
        self.serial_baud = DEFAULT_BAUD_RATE
        self.serial_timeout = 1

        # Users can use command to pass port speeds together with port name. E.g. COM4:115200:1
        # Format if PORT:SPEED:TIMEOUT
        port_config = self.port.split(':') if self.port else ''
        if len(port_config) == 2:
            # -p COM4:115200
            self.port = port_config[0]
            self.serial_baud = int(port_config[1])
        elif len(port_config) == 3:
            # -p COM4:115200:0.5
            self.port = port_config[0]
            self.serial_baud = int(port_config[1])
            self.serial_timeout = float(port_config[2])

        # Overriding baud rate value with command line specified value
        self.serial_baud = self.options.baud_rate if self.options.baud_rate else self.serial_baud

        # Test configuration in JSON format
        self.test_cfg = None
        if self.options.json_test_configuration is not None:
            # We need to normalize path before we open file
            json_test_configuration_path = self.options.json_test_configuration.strip("\"'")
            try:
                self.logger.prn_inf("Loading test configuration from '%s'..." % json_test_configuration_path)
                with open(json_test_configuration_path) as data_file:
                    self.test_cfg = json.load(data_file)
            except IOError as e:
                self.logger.prn_err("Test configuration JSON file '{0}' I/O error({1}): {2}"
                                    .format(json_test_configuration_path, e.errno, e.strerror))
            except:
                self.logger.prn_err("Test configuration JSON Unexpected error:", str(e))
                raise

    def copy_image(self, image_path=None, disk=None, copy_method=None, port=None, mcu=None, retry_copy=5):
        """! Closure for copy_image_raw() method.
        @return Returns result from copy plugin
        """
        def get_remount_count(disk_path, tries=2):
            """! Get the remount count from 'DETAILS.TXT' file
            @return Returns count, None if not-available
            """

            #In case of no disk path, nothing to do
            if disk_path is None:
                return None
                
            for cur_try in range(1, tries + 1):
                try:
                    files_on_disk = [x.upper() for x in os.listdir(disk_path)]
                    if 'DETAILS.TXT' in files_on_disk:
                        with open(os.path.join(disk_path, 'DETAILS.TXT'), 'r') as details_txt:
                            for line in details_txt.readlines():
                                if 'Remount count:' in line:
                                    return int(line.replace('Remount count: ', ''))
                            # Remount count not found in file
                            return None
                    # 'DETAILS.TXT file not found
                    else:
                        return None

                except OSError as e:
                    self.logger.prn_err("Failed to get remount count due to OSError.", str(e))
                    self.logger.prn_inf("Retrying in 1 second (try %s of %s)" % (cur_try, tries))
                    sleep(1)
            # Failed to get remount count
            return None

        def check_flash_error(target_id, disk, initial_remount_count):
            """! Check for flash errors
            @return Returns false if FAIL.TXT present, else true
            """
            if not target_id:
                self.logger.prn_wrn("Target ID not found: Skipping flash check and retry")
                return True

            if not copy_method in ["shell", "default"]:
                # We're using a "copy method" that may not necessarily require
                # an "Mbed Enabled" device. In this case we shouldn't use
                # mbedls.detect to attempt to rediscover the mount point, as
                # mbedls.detect is only compatible with Mbed Enabled devices.
                # It's best just to return `True` and continue here. This will
                # avoid the inevitable 2.5s delay caused by us repeatedly
                # attempting to enumerate Mbed Enabled devices in the code
                # below when none are connected. The user has specified a
                # non-Mbed plugin copy method, so we shouldn't delay them by
                # trying to check for Mbed Enabled devices.
                return True

            bad_files = set(['FAIL.TXT'])
            # Re-try at max 5 times with 0.5 sec in delay
            for i in range(5):
                # mbed_os_tools.detect.create() should be done inside the loop. Otherwise it will loop on same data.
                mbeds = detect.create()
                mbed_list = mbeds.list_mbeds() #list of mbeds present
                # get first item in list with a matching target_id, if present
                mbed_target = next((x for x in mbed_list if x['target_id']==target_id), None)

                if mbed_target is not None:
                    if 'mount_point' in mbed_target and mbed_target['mount_point'] is not None:
                        if not initial_remount_count is None:
                            new_remount_count = get_remount_count(disk)
                            if not new_remount_count is None and new_remount_count == initial_remount_count:
                                sleep(0.5)
                                continue

                        common_items = []
                        try:
                            items = set([x.upper() for x in os.listdir(mbed_target['mount_point'])])
                            common_items = bad_files.intersection(items)
                        except OSError as e:
                            print("Failed to enumerate disk files, retrying")
                            continue

                        for common_item in common_items:
                            full_path = os.path.join(mbed_target['mount_point'], common_item)
                            self.logger.prn_err("Found %s"% (full_path))
                            bad_file_contents = "[failed to read bad file]"
                            try:
                                with open(full_path, "r") as bad_file:
                                    bad_file_contents = bad_file.read()
                            except IOError as error:
                                self.logger.prn_err("Error opening '%s': %s" % (full_path, error))

                            self.logger.prn_err("Error file contents:\n%s" % bad_file_contents)
                        if common_items:
                            return False
                sleep(0.5)
            return True

        # Set-up closure environment
        if not image_path:
            image_path = self.image_path
        if not disk:
            disk = self.disk
        if not copy_method:
            copy_method = self.copy_method
        if not port:
            port = self.port
        if not mcu:
            mcu = self.mcu
        if not retry_copy:
            retry_copy = self.retry_copy
        target_id = self.target_id

        if not image_path:
            self.logger.prn_err("Error: image path not specified")
            return False

        if not os.path.isfile(image_path):
            self.logger.prn_err("Error: image file (%s) not found" % image_path)
            return False

        for count in range(0, retry_copy):
            initial_remount_count = get_remount_count(disk)
            # Call proper copy method
            result = self.copy_image_raw(image_path, disk, copy_method, port, mcu)
            sleep(self.program_cycle_s)
            if not result:
                continue
            result = check_flash_error(target_id, disk, initial_remount_count)
            if result:
                break
        return result

    def copy_image_raw(self, image_path=None, disk=None, copy_method=None, port=None, mcu=None):
        """! Copy file depending on method you want to use. Handles exception
             and return code from shell copy commands.
        @return Returns result from copy plugin
        @details Method which is actually copying image to mbed
        """
        # image_path - Where is binary with target's firmware

        # Select copy_method
        # We override 'default' method with 'shell' method
        copy_method = {
            None : 'shell',
            'default' : 'shell',
        }.get(copy_method, copy_method)

        result = ht_plugins.call_plugin('CopyMethod',
                                        copy_method,
                                        image_path=image_path,
                                        mcu=mcu,
                                        serial=port,
                                        destination_disk=disk,
                                        target_id=self.target_id,
                                        pooling_timeout=self.polling_timeout,
                                        format=self.options.format
                                        )
        return result

    def hw_reset(self):
        """
        Performs hardware reset of target ned device.

        :return:
        """
        device_info = {}
        result = ht_plugins.call_plugin('ResetMethod',
                                        'power_cycle',
                                        target_id=self.target_id,
                                        device_info=device_info,
                                        format=self.options.format)
        if result:
            self.port = device_info['serial_port']
            self.disk = device_info['mount_point']
        return result
