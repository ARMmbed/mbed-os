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
from shutil import copy
from .host_test_plugins import HostTestPluginBase


class HostTestPluginCopyMethod_Mbed(HostTestPluginBase):
    """ Generic flashing method for mbed-enabled devices (by copy)
    """

    def __init__(self):
        """ ctor
        """
        HostTestPluginBase.__init__(self)

    def generic_mbed_copy(self, image_path, destination_disk):
        """! Generic mbed copy method for "mbed enabled" devices.

        @param image_path Path to binary file to be flashed
        @param destination_disk Path to destination (mbed mount point)

        @details It uses standard python shutil function to copy image_file (target specific binary) to device's disk.

        @return Returns True if copy (flashing) was successful
        """
        result = True
        if not destination_disk.endswith('/') and not destination_disk.endswith('\\'):
            destination_disk += '/'
        try:
            copy(image_path, destination_disk)
        except Exception as e:
            self.print_plugin_error("shutil.copy('%s', '%s')"% (image_path, destination_disk))
            self.print_plugin_error("Error: %s"% str(e))
            result = False
        return result

    # Plugin interface
    name = 'HostTestPluginCopyMethod_Mbed'
    type = 'CopyMethod'
    stable = True
    capabilities = ['shutil', 'default']
    required_parameters = ['image_path', 'destination_disk']

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        return True

    def execute(self, capability, *args, **kwargs):
        """! Executes capability by name.
        @details Each capability may directly just call some command line program or execute building pythonic function
        @return Returns True if 'capability' operation was successful
        """
        if not kwargs['image_path']:
            self.print_plugin_error("Error: image path not specified")
            return False

        if not kwargs['destination_disk']:
            self.print_plugin_error("Error: destination disk not specified")
            return False

        # This optional parameter can be used if TargetID is provided (-t switch)
        target_id = kwargs.get('target_id', None)
        pooling_timeout = kwargs.get('polling_timeout', 60)

        result = False
        if self.check_parameters(capability, *args, **kwargs):
            # Capability 'default' is a dummy capability
            if kwargs['image_path'] and kwargs['destination_disk']:
                if capability == 'shutil':
                    image_path = os.path.normpath(kwargs['image_path'])
                    destination_disk = os.path.normpath(kwargs['destination_disk'])
                    # Wait for mount point to be ready
                    # if mount point changed according to target_id use new mount point
                    # available in result (_, destination_disk) of check_mount_point_ready
                    mount_res, destination_disk = self.check_mount_point_ready(destination_disk, target_id=self.target_id, timeout=pooling_timeout)  # Blocking
                    if mount_res:
                        result = self.generic_mbed_copy(image_path, destination_disk)
        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_Mbed()
