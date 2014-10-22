# Interfaces and utils for host test plugin architecture

from subprocess import call
from host_test_plugins import HostTestPluginBase


class HostTestPluginResetMethod_Mbed(HostTestPluginBase):

    def safe_sendBreak(self, serial):
        """ Wraps serial.sendBreak() to avoid serial::serialposix.py exception on Linux
            Traceback (most recent call last):
              File "make.py", line 189, in <module>
                serial.sendBreak()
              File "/usr/lib/python2.7/dist-packages/serial/serialposix.py", line 511, in sendBreak
                termios.tcsendbreak(self.fd, int(duration/0.25))
            error: (32, 'Broken pipe')
        """
        result = True
        try:
            serial.sendBreak()
        except:
            # In linux a termios.error is raised in sendBreak and in setBreak.
            # The following setBreak() is needed to release the reset signal on the target mcu.
            try:
                serial.setBreak(False)
            except:
                result = False
        return result

    # Plugin interface
    name = 'HostTestPluginResetMethod_Mbed'
    type = 'ResetMethod'
    capabilities = ['default']
    required_parameters = ['serial']

    def setup(self, *args, **kwargs):
        pass

    def execute(self, capabilitity, *args, **kwargs):
        """ Executes capability by name.
            Each capability may directly just call some command line
            program or execute building pythonic function
        """
        for parameter in self.required_parameters:
            if parameter not in kwargs:
                print "Plugin parameter '%s' missing"% parameter
                return False

        if capabilitity == 'default':
            serial = kwargs['serial']
            self.safe_sendBreak(serial)
        else:
            return False
        return True


def load_plugin():
    """ Returns plugin available in this module
    """
    return HostTestPluginResetMethod_Mbed()
