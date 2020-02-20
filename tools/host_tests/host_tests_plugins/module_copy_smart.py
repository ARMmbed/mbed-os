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

import os
import sys
from os.path import join, basename, exists, abspath, dirname
from time import sleep
from host_test_plugins import HostTestPluginBase

sys.path.append(abspath(join(dirname(__file__), "../../../")))
import tools.test_api

class HostTestPluginCopyMethod_Smart(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginCopyMethod_Smart'
    type = 'CopyMethod'
    stable = True
    capabilities = ['smart']
    required_parameters = ['image_path', 'destination_disk', 'target_mcu']

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        return True

    def execute(self, capability, *args, **kwargs):
        """ Executes capability by name.
            Each capability may directly just call some command line
            program or execute building pythonic function
        """
        result = False
        if self.check_parameters(capability, *args, **kwargs) is True:
            image_path = kwargs['image_path']
            destination_disk = kwargs['destination_disk']
            target_mcu = kwargs['target_mcu']
            # Wait for mount point to be ready
            self.check_mount_point_ready(destination_disk)  # Blocking
            # Prepare correct command line parameter values
            image_base_name = basename(image_path)
            destination_path = join(destination_disk, image_base_name)
            if capability == 'smart':
                if os.name == 'posix':
                    cmd = ['cp', image_path, destination_path]
                    result = self.run_command(cmd, shell=False)

                    cmd = ['sync']
                    result = self.run_command(cmd, shell=False)
                elif os.name == 'nt':
                    cmd = ['copy', image_path, destination_path]
                    result = self.run_command(cmd, shell=True)

                # Give the OS and filesystem time to settle down
                sleep(3)

                platform_name_filter = [target_mcu]
                muts_list = {}

                remount_complete = False

                for i in range(0, 60):
                    print('Looking for %s with MBEDLS' % target_mcu)
                    muts_list = tools.test_api.get_autodetected_MUTS_list(platform_name_filter=platform_name_filter)

                    if 1 in muts_list:
                        mut = muts_list[1]
                        destination_disk = mut['disk']
                        destination_path = join(destination_disk, image_base_name)

                        if mut['mcu'] == 'LPC1768' or mut['mcu'] == 'LPC11U24':
                            if exists(destination_disk) and exists(destination_path):
                                remount_complete = True
                                break;
                        else:
                            if exists(destination_disk) and not exists(destination_path):
                                remount_complete = True
                                break;

                    sleep(1)

                if remount_complete:
                    print('Remount complete')
                else:
                    print('Remount FAILED')

                    if exists(destination_disk):
                        print('Disk exists')
                    else:
                        print('Disk does not exist')

                    if exists(destination_path):
                        print('Image exists')
                    else:
                        print('Image does not exist')

                    result = None


        return result

def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_Smart()
