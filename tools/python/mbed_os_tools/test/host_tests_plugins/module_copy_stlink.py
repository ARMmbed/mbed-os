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


class HostTestPluginCopyMethod_Stlink(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginCopyMethod_Stlink'
    type = 'CopyMethod'
    capabilities = ['stlink']
    required_parameters = ['image_path']

    def __init__(self):
        """ ctor
        """
        HostTestPluginBase.__init__(self)

    def is_os_supported(self, os_name=None):
        """! In this implementation this plugin only is supporeted under Windows machines
        """
        # If no OS name provided use host OS name
        if not os_name:
            os_name = self.mbed_os_support()

        # This plugin only works on Windows
        if os_name and os_name.startswith('Windows'):
            return True
        return False

    def setup(self, *args, **kwargs):
        """! Configure plugin, this function should be called before plugin execute() method is used.
        """
        self.ST_LINK_CLI = 'ST-LINK_CLI.exe'
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
            if capability == 'stlink':
                # Example:
                # ST-LINK_CLI.exe -p "C:\Work\mbed\build\test\DISCO_F429ZI\GCC_ARM\MBED_A1\basic.bin"
                cmd = [self.ST_LINK_CLI,
                       '-p', image_path, '0x08000000',
                       '-V'
                       ]
                result = self.run_command(cmd)
        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginCopyMethod_Stlink()
