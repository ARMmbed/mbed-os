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
from __future__ import print_function

import os
from os.path import join, basename
from time import sleep

from .host_test_plugins import HostTestPluginBase

class HostTestPluginCopyMethod_Shell(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginCopyMethod_Shell'
    type = 'CopyMethod'
    stable = True
    capabilities = ['shell', 'cp', 'copy', 'xcopy']
    required_parameters = ['image_path', 'destination_disk', 'program_cycle_s']

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
            program_cycle_s = kwargs['program_cycle_s']
            # Wait for mount point to be ready
            self.check_mount_point_ready(destination_disk)  # Blocking
            # Prepare correct command line parameter values
            image_base_name = basename(image_path)
            destination_path = join(destination_disk, image_base_name)
            if capability == 'shell':
                if os.name == 'nt': capability = 'copy'
                elif os.name == 'posix': capability = 'cp'
            if capability == 'cp' or capability == 'copy' or capability == 'copy':
                copy_method = capability
                cmd = [copy_method, image_path, destination_path]
                if os.name == 'posix':
                    result = self.run_command(cmd, shell=False)
                    result = self.run_command(["sync"])
                else:
                    result = self.run_command(cmd)

            # Allow mbed to cycle
            sleep(program_cycle_s)

        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_Shell()
