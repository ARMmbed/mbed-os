# Interfaces and utils for host test plugin architecture

import os
from host_test_plugins import HostTestPluginBase

# Note: This plugin is not fully functional, needs improvements

class HostTestPluginResetMethod_MPS2(HostTestPluginBase):
    """ Plugin used to reset ARM_MPS2 platform
    Supports:
         reboot.txt   - startup from standby state, reboots when in run mode.
         shutdown.txt - shutdown from run mode.
         reset.txt    - reset FPGA during run mode.
    """
    def touch_file(self, path):
        """ Touch file and set timestamp to items
        """
        with open(path, 'a'):
            os.utime(path, None)

    # Plugin interface
    name = 'HostTestPluginResetMethod_MPS2'
    type = 'ResetMethod'
    capabilities = ['reboot.txt', 'shutdown.txt', 'reset.txt']
    required_parameters = ['disk']

    def setup(self, *args, **kwargs):
        """ Prepare / configure plugin to work.
            This method can receive plugin specific parameters by kwargs and
            ignore other parameters which may affect other plugins.
        """
        pass

    def execute(self, capabilitity, *args, **kwargs):
        """ Executes capability by name.
            Each capability may directly just call some command line
            program or execute building pythonic function
        """
        for parameter in self.required_parameters:
            if parameter not in kwargs:
                print "%s. Plugin parameter '%s' missing"% (self.name, parameter)
                return False

        if capabilitity == 'reboot.txt':
            # TODO: Implement touch file for reboot
            pass

        elif capabilitity == 'shutdown.txt':
            # TODO: Implement touch file for shutdown
            pass

        elif capabilitity == 'reset.txt':
            # TODO: Implement touch file for reset
            pass

        else:
            return False
        return True

def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginResetMethod_MPS2()
