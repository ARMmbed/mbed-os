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

from .host_test_plugins import HostTestPluginBase


class HostTestPluginResetMethod_JN51xx(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginResetMethod_JN51xx'
    type = 'ResetMethod'
    capabilities = ['jn51xx']
    required_parameters = ['serial']
    stable = False

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
        # Note you need to have eACommander.exe on your system path!
        self.JN51XX_PROGRAMMER = 'JN51xxProgrammer.exe'
        return True

    def execute(self, capability, *args, **kwargs):
        """! Executes capability by name

        @param capability Capability name
        @param args Additional arguments
        @param kwargs Additional arguments
        @details Each capability e.g. may directly just call some command line program or execute building pythonic function
        @return Capability call return value
        """
        if not kwargs['serial']:
            self.print_plugin_error("Error: serial port not set (not opened?)")
            return False

        result = False
        if self.check_parameters(capability, *args, **kwargs):
            if kwargs['serial']:
                if capability == 'jn51xx':
                    # Example:
                    # The device should be automatically reset before the programmer disconnects.
                    # Issuing a command with no file to program or read will put the device into
                    # programming mode and then reset it. E.g.
                    # $ JN51xxProgrammer.exe -s COM5 -V0
                    # COM5: Detected JN5179 with MAC address 00:15:8D:00:01:24:E0:37
                    serial_port = kwargs['serial']
                    cmd = [self.JN51XX_PROGRAMMER,
                           '-s', serial_port,
                           '-V0'
                          ]
                    result = self.run_command(cmd)
        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginResetMethod_JN51xx()
