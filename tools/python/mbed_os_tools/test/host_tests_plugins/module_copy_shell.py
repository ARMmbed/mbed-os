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
from os.path import join, basename
from .host_test_plugins import HostTestPluginBase


class HostTestPluginCopyMethod_Shell(HostTestPluginBase):
    # Plugin interface
    name = 'HostTestPluginCopyMethod_Shell'
    type = 'CopyMethod'
    stable = True
    capabilities = ['shell', 'cp', 'copy', 'xcopy']
    required_parameters = ['image_path', 'destination_disk']

    def __init__(self):
        """ ctor
        """
        HostTestPluginBase.__init__(self)

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        return True

    def execute(self, capability, *args, **kwargs):
        """! Executes capability by name

        @param capability Capability name
        @param args Additional arguments
        @param kwargs Additional arguments
        @details Each capability e.g. may directly just call some command line program or execute building pythonic function
        @return Capability call return value
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
            if kwargs['image_path'] and kwargs['destination_disk']:
                image_path = os.path.normpath(kwargs['image_path'])
                destination_disk = os.path.normpath(kwargs['destination_disk'])
                # Wait for mount point to be ready
                # if mount point changed according to target_id use new mount point
                # available in result (_, destination_disk) of check_mount_point_ready
                mount_res, destination_disk = self.check_mount_point_ready(destination_disk, target_id=target_id, timeout=pooling_timeout)  # Blocking
                if not mount_res:
                    return result # mount point is not ready return
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
                        if os.uname()[0] == 'Linux':
                            result = result and self.run_command(["sync", "-f", destination_path])
                        else:
                            result = result and self.run_command(["sync"])
                    else:
                        result = self.run_command(cmd)
        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_Shell()
