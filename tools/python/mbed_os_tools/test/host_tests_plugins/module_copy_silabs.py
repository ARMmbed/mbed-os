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
from .host_test_plugins import HostTestPluginBase


class HostTestPluginCopyMethod_Silabs(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginCopyMethod_Silabs'
    type = 'CopyMethod'
    capabilities = ['eACommander', 'eACommander-usb']
    required_parameters = ['image_path', 'destination_disk']
    stable = True

    def __init__(self):
        """ ctor
        """
        HostTestPluginBase.__init__(self)

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        self.EACOMMANDER_CMD = 'eACommander.exe'
        return True

    def execute(self, capability, *args, **kwargs):
        """! Executes capability by name

        @param capability Capability name
        @param args Additional arguments
        @param kwargs Additional arguments

        @details Each capability e.g. may directly just call some command line program or execute building pythonic function

        @return Capability call return value
        """
        result = False
        if self.check_parameters(capability, *args, **kwargs) is True:
            image_path = os.path.normpath(kwargs['image_path'])
            destination_disk = os.path.normpath(kwargs['destination_disk'])
            if capability == 'eACommander':
                cmd = [self.EACOMMANDER_CMD,
                       '--serialno', destination_disk,
                       '--flash', image_path,
                       '--resettype', '2', '--reset']
                result = self.run_command(cmd)
            elif capability == 'eACommander-usb':
                cmd = [self.EACOMMANDER_CMD,
                       '--usb', destination_disk,
                       '--flash', image_path]
                result = self.run_command(cmd)
        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_Silabs()
