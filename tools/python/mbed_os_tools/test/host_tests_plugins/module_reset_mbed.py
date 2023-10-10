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

import re
import pkg_resources
from .host_test_plugins import HostTestPluginBase


class HostTestPluginResetMethod_Mbed(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginResetMethod_Mbed'
    type = 'ResetMethod'
    stable = True
    capabilities = ['default']
    required_parameters = ['serial']

    def __init__(self):
        """! ctor
        @details We can check module version by referring to version attribute
        import pkg_resources
        print pkg_resources.require("mbed-host-tests")[0].version
        '2.7'
        """
        HostTestPluginBase.__init__(self)
        self.re_float = re.compile("^\d+\.\d+")
        pyserial_version = pkg_resources.require("pyserial")[0].version
        self.pyserial_version = self.get_pyserial_version(pyserial_version)
        self.is_pyserial_v3 = float(self.pyserial_version) >= 3.0

    def get_pyserial_version(self, pyserial_version):
        """! Retrieve pyserial module version
        @return Returns float with pyserial module number
        """
        version = 3.0
        m = self.re_float.search(pyserial_version)
        if m:
            try:
                version = float(m.group(0))
            except ValueError:
                version = 3.0   # We will assume you've got latest (3.0+)
        return version

    def safe_sendBreak(self, serial):
        """! Closure for pyserial version dependant API calls
        """
        if self.is_pyserial_v3:
            return self._safe_sendBreak_v3_0(serial)
        return self._safe_sendBreak_v2_7(serial)

    def _safe_sendBreak_v2_7(self, serial):
        """! pyserial 2.7 API implementation of sendBreak/setBreak
        @details
        Below API is deprecated for pyserial 3.x versions!
        http://pyserial.readthedocs.org/en/latest/pyserial_api.html#serial.Serial.sendBreak
        http://pyserial.readthedocs.org/en/latest/pyserial_api.html#serial.Serial.setBreak
        """
        result = True
        try:
            serial.sendBreak()
        except:
            # In Linux a termios.error is raised in sendBreak and in setBreak.
            # The following setBreak() is needed to release the reset signal on the target mcu.
            try:
                serial.setBreak(False)
            except:
                result = False
        return result

    def _safe_sendBreak_v3_0(self, serial):
        """! pyserial 3.x API implementation of send_brea / break_condition
        @details
        http://pyserial.readthedocs.org/en/latest/pyserial_api.html#serial.Serial.send_break
        http://pyserial.readthedocs.org/en/latest/pyserial_api.html#serial.Serial.break_condition
        """
        result = True
        try:
            serial.send_break()
        except:
            # In Linux a termios.error is raised in sendBreak and in setBreak.
            # The following break_condition = False is needed to release the reset signal on the target mcu.
            try:
                serial.break_condition = False
            except Exception as e:
                self.print_plugin_error("Error while doing 'serial.break_condition = False' : %s"% str(e))
                result = False
        return result

    def setup(self, *args, **kwargs):
        """! Configure plugin, this function should be called before plugin execute() method is used.
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
        if not kwargs['serial']:
            self.print_plugin_error("Error: serial port not set (not opened?)")
            return False

        result = False
        if self.check_parameters(capability, *args, **kwargs) is True:
            if kwargs['serial']:
                if capability == 'default':
                    serial = kwargs['serial']
                    result = self.safe_sendBreak(serial)
        return result


def load_plugin():
    """! Returns plugin available in this module
    """
    return HostTestPluginResetMethod_Mbed()
