"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

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

from subprocess import call
from host_test_plugins import HostTestPluginBase


class HostTestPluginResetMethod_SiLabs(HostTestPluginBase):
    # Plugin interface
    name = 'HostTestPluginResetMethod_SiLabs'
    type = 'ResetMethod'
    capabilities = ['eACommander', 'eACommander-usb']
    required_parameters = ['disk']

    def setup(self, *args, **kwargs):
        """ Configure plugin, this function should be called before plugin execute() method is used.
        """
        self.EACOMMANDER_CMD = 'c:/SiliconLabs/SimplicityStudio/v2/commander/eACommander.exe'

    def execute(self, capabilitity, *args, **kwargs):
        """ Executes capability by name.
            Each capability may directly just call some command line
            program or execute building pythonic function
        """

        for parameter in self.required_parameters:
            if parameter not in kwargs:
                print "Plugin parameter '%s' missing"% parameter
                return False

        disk = kwargs['disk'].rstrip('/\\')

        if capabilitity == 'eACommander':
            # For this copy method 'disk' will be 'serialno' for eACommander command line parameters
            # Note: Commands are executed in the order they are specified on the command line
            cmd = [self.EACOMMANDER_CMD,
                   '--serialno', disk,
                   '--resettype', '2', '--reset',]
            try:
                ret = call(cmd, shell=True)
                if ret:
                    print "Return code: %d. Command: "% ret + " ".join(cmd)
            except Exception, e:
                print str(e)

        elif capabilitity == 'eACommander-usb':
            # For this copy method 'disk' will be 'usb address' for eACommander command line parameters
            # Note: Commands are executed in the order they are specified on the command line
            cmd = [self.EACOMMANDER_CMD,
                   '--usb', disk,
                   '--resettype', '2', '--reset',]
            try:
                ret = call(cmd, shell=True)
                if ret:
                    print "Return code: %d. Command: "% ret + " ".join(cmd)
            except Exception, e:
                print(e)
        else:
            return False
        return True


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginResetMethod_SiLabs()
