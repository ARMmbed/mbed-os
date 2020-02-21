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
from time import sleep
from .host_test_plugins import HostTestPluginBase

# Note: This plugin is not fully functional, needs improvements

class HostTestPluginResetMethod_MPS2(HostTestPluginBase):
    """ Plugin used to reset ARM_MPS2 platform
    Supports:
         reboot.txt   - startup from standby state, reboots when in run mode.
         shutdown.txt - shutdown from run mode.
         reset.txt    - reset FPGA during run mode.
    """
    def touch_file(self, file):
        """ Touch file and set timestamp to items
        """
        tfile = file+'.tmp'
        fhandle = open(tfile, 'a')
        try:
            fhandle.close()
        finally:
            os.rename(tfile, file)
            return True

    # Plugin interface
    name = 'HostTestPluginResetMethod_MPS2'
    type = 'ResetMethod'
    capabilities = ['mps2-reboot', 'mps2-reset']
    required_parameters = ['disk']

    def setup(self, *args, **kwargs):
        """ Prepare / configure plugin to work.
            This method can receive plugin specific parameters by kwargs and
            ignore other parameters which may affect other plugins.
        """
        return True

    def execute(self, capabilitity, *args, **kwargs):
        """ Executes capability by name.
            Each capability may directly just call some command line
            program or execute building pythonic function
        """
        return True
        result = False
        if self.check_parameters(capabilitity, *args, **kwargs) is True:
            disk = kwargs['disk']
            
            if capabilitity == 'mps2-reboot' and self.touch_file(disk + 'reboot.txt'):
                sleep(20)
                result = True

            elif capabilitity == 'mps2-reset' and self.touch_file(disk + 'reboot.txt'):
                sleep(20)
                result = True

        return result

def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginResetMethod_MPS2()
