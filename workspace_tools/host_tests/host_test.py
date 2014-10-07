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

# Check if 'serial' module is installed
try:
    from serial import Serial
except ImportError, e:
    print "Error: Can't import 'serial' module: %s"% e
    exit(-1)

import os
from optparse import OptionParser
from time import sleep, time
from sys import stdout

# This is a little tricky. We need to add upper directory to path so
# we can find packages we want from the same level as other files do
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..')))
from workspace_tools.settings import EACOMMANDER_CMD


class Mbed:
    """ Base class for a host driven test
    """
    def __init__(self):
        parser = OptionParser()

        parser.add_option("-m", "--micro",
                          dest="micro",
                          help="The target microcontroller",
                          metavar="MICRO")

        parser.add_option("-p", "--port",
                          dest="port",
                          help="The serial port of the target mbed (ie: COM3)",
                          metavar="PORT")

        parser.add_option("-d", "--disk",
                          dest="disk",
                          help="The target disk path",
                          metavar="DISK_PATH")

        parser.add_option("-t", "--timeout",
                          dest="timeout",
                          help="Timeout",
                          metavar="TIMEOUT")

        parser.add_option("-e", "--extra",
                          dest="extra",
                          help="Extra serial port (used by some tests)",
                          metavar="EXTRA")

        parser.add_option("-r", "--reset",
                          dest="forced_reset_type",
                          help="Forces different type of reset")

        parser.add_option("-R", "--reset-timeout",
                          dest="forced_reset_timeout",
                          metavar="NUMBER",
                          type="int",
                          help="When forcing a reset using option -r you can set up after reset timeout in seconds")

        (self.options, _) = parser.parse_args()

        self.DEFAULT_RESET_TOUT = 0
        self.DEFAULT_TOUT = 10

        if self.options.port is None:
            raise Exception("The serial port of the target mbed have to be provided as command line arguments")

        self.port = self.options.port
        self.disk = self.options.disk
        self.extra_port = self.options.extra
        self.extra_serial = None
        self.serial = None
        self.timeout = self.DEFAULT_TOUT if self.options.timeout is None else self.options.timeout
        print 'Host test instrumentation on port: "%s" and disk: "%s"' % (self.port, self.disk)

    def init_serial(self, baud=9600, extra_baud=9600):
        """ Initialize serial port. Function will return error is port can't be opened or initialized
        """
        result = True
        try:
            self.serial = Serial(self.port, timeout=1)
        except Exception as e:
            result = False
        # Port can be opened
        if result:
            self.serial.setBaudrate(baud)
            if self.extra_port:
                self.extra_serial = Serial(self.extra_port, timeout = 1)
                self.extra_serial.setBaudrate(extra_baud)
            self.flush()
        return result

    def serial_timeout(self, timeout):
        """ Wraps self.mbed.serial object timeout property
        """
        result = None
        if self.serial:
            self.serial.timeout = timeout
            result = True
        return result

    def serial_read(self, count=1):
        """ Wraps self.mbed.serial object read method
        """
        result = None
        if self.serial:
            try:
                result = self.serial.read(count)
            except:
                result = None
        return result

    def serial_readline(self, timeout=5):
        """ Wraps self.mbed.serial object read method to read one line from serial port
        """
        result = ''
        start = time()
        while (time() - start) < timeout:
            if self.serial:
                try:
                    c = self.serial.read(1)
                    result += c
                except:
                    result = None
                if c == '\n':
                    break
        return result

    def serial_write(self, write_buffer):
        """ Wraps self.mbed.serial object write method
        """
        result = None
        if self.serial:
            try:
                result = self.serial.write(write_buffer)
            except:
               result = None
        return result

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

    def touch_file(self, path):
        """ Touch file and set timestamp to items
        """
        with open(path, 'a'):
            os.utime(path, None)

    def reset_timeout(self, timeout):
        """ Timeout executed just after reset command is issued
        """
        for n in range(0, timeout):
            sleep(1)

    def reset(self):
        """ Reset function.
            Supports:
            - 'standard' send break command via Mbed's CDC,
            - also handles other reset modes:
              -  E.g. reset by touching file with specific file name:
                 reboot.txt   - startup from standby state, reboots when in run mode.
                 shutdown.txt - shutdown from run mode
                 reset.txt    - reset FPGA during run mode
              - eACommander for reset of SiLabs Gecko baords.
        """
        if self.options.forced_reset_type:
            if self.options.forced_reset_type == 'eACommander':
                # For this copy method 'disk' will be 'serialno' for eACommander command line parameters
                # Note: Commands are executed in the order they are specified on the command line
                cmd = [EACOMMANDER_CMD,
                       '--serialno', self.disk.rstrip('/\\'),
                       '--resettype', '2', '--reset',]
                try:
                    self.flush()
                    ret = call(cmd, shell=True)
                    if ret:
                        resutl_msg = "Return code: %d. Command: "% ret + " ".join(cmd)
                        result = False
                except Exception, e:
                    resutl_msg = e
                    result = False
            elif self.options.forced_reset_type == 'eACommander-usb':
                # For this copy method 'disk' will be 'usb address' for eACommander command line parameters
                # Note: Commands are executed in the order they are specified on the command line
                cmd = [EACOMMANDER_CMD,
                       '--usb', self.disk.rstrip('/\\'),
                       '--resettype', '2', '--reset',]
                try:
                    self.flush()
                    ret = call(cmd, shell=True)
                    if ret:
                        resutl_msg = "Return code: %d. Command: "% ret + " ".join(cmd)
                        result = False
                except Exception, e:
                    resutl_msg = e
                    result = False
            elif self.options.forced_reset_type.endswith('.txt'):
                reset_file_path = os.path.join(self.disk, self.options.forced_reset_type.lower())
                self.touch_file(reset_file_path)
                self.flush()
        else:
            self.safe_sendBreak(self.serial)  # Instead of serial.sendBreak()
            self.flush()
        # Flush serials to get only input after reset
        #self.flush()
        # Give time to wait for the image loading
        reset_tout_s = self.options.forced_reset_timeout if self.options.forced_reset_timeout is not None else self.DEFAULT_RESET_TOUT
        self.reset_timeout(reset_tout_s)

    def flush(self):
        """ Flush serial ports
        """
        if self.serial:
            self.serial.flushInput()
            self.serial.flushOutput()
        if self.extra_serial:
            self.extra_serial.flushInput()
            self.extra_serial.flushOutput()


class Test:
    """ Baseclass for host test's test runner
    """
    def __init__(self):
        self.mbed = Mbed()

    def run(self):
        """ Test runner for host test. This function will start executing
            test and forward test result via serial port to test suite
        """
        try:
            result = self.test()
            self.print_result("success" if result else "failure")
        except Exception, e:
            print str(e)
            self.print_result("error")

    def setup(self):
        """ Setup and check if configuration for test is correct. E.g. if serial port can be opened
        """
        result = True
        if not self.mbed.serial:
            result = False
            self.print_result("ioerr_serial")
        return result

    def notify(self, message):
        """ On screen notification function
        """
        print message
        stdout.flush()

    def print_result(self, result):
        """ Test result unified printing function
        """
        self.notify("\n{%s}\n{end}" % result)


class DefaultTest(Test):
    """ Test class with serial port initialization
    """
    def __init__(self):
        Test.__init__(self)
        serial_init_res = self.mbed.init_serial()
        self.mbed.reset()


class Simple(DefaultTest):
    """ Simple, basic host test's test runner waiting for serial port
        output from MUT, no supervision over test running in MUT is executed.
        Just waiting for result
    """
    def run(self):
        try:
            while True:
                c = self.mbed.serial_read(512)
                if c is None:
                    self.print_result("ioerr_serial")
                    break
                stdout.write(c)
                stdout.flush()
        except KeyboardInterrupt, _:
            print "\n[CTRL+c] exit"


if __name__ == '__main__':
    Simple().run()
