"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.Wirkus@arm.com>
"""

from host_test_plugins import HostTestPluginBase


class HostTestPluginResetMethod_ublox(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginResetMethod_ublox'
    type = 'ResetMethod'
    capabilities = ['ublox']
    required_parameters = []
    stable = False

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
        # Note you need to have jlink.exe on your system path!
        self.JLINK = 'jlink.exe'
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
            if capability == 'ublox':
                # Example:
                # JLINK.exe --CommanderScript aCommandFile
                cmd = [self.JLINK,
                       '-CommanderScript',
                       r'reset.jlink']
                result = self.run_command(cmd)
        return result


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginResetMethod_ublox()
