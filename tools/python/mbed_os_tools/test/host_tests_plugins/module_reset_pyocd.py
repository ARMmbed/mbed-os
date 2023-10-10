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

try:
    from pyocd.core.helpers import ConnectHelper
    PYOCD_PRESENT = True
except ImportError:
    PYOCD_PRESENT = False


class HostTestPluginResetMethod_pyOCD(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginResetMethod_pyOCD'
    type = 'ResetMethod'
    stable = True
    capabilities = ['pyocd']
    required_parameters = ['target_id']

    def __init__(self):
        """! ctor
        @details We can check module version by referring to version attribute
        import pkg_resources
        print pkg_resources.require("mbed-host-tests")[0].version
        '2.7'
        """
        HostTestPluginBase.__init__(self)

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
        if not PYOCD_PRESENT:
            self.print_plugin_error(
                'The "pyocd" feature is not installed. Please run '
                '"pip install mbed-os-tools[pyocd]" to enable the "pyocd" reset plugin.'
            )
            return False

        if not kwargs['target_id']:
            self.print_plugin_error("Error: target_id not set")
            return False

        result = False
        if self.check_parameters(capability, *args, **kwargs) is True:
            if kwargs['target_id']:
                if capability == 'pyocd':
                    target_id = kwargs['target_id']
                    with ConnectHelper.session_with_chosen_probe(unique_id=target_id,
                                resume_on_disconnect=False) as session:
                        session.target.reset()
                        session.target.resume()
                        result = True
        return result


def load_plugin():
    """! Returns plugin available in this module
    """
    return HostTestPluginResetMethod_pyOCD()
