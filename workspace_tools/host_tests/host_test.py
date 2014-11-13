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
from sys import stdout
from time import sleep, time
from optparse import OptionParser

import host_tests_plugins

# This is a little tricky. We need to add upper directory to path so
# we can find packages we want from the same level as other files do
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..')))


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
                    break
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
        """ Calls proper reset plugin to do the job.
            Please refer to host_test_plugins functionality
        """
        # Flush serials to get only input after reset
        self.flush()
        if self.options.forced_reset_type:
            host_tests_plugins.call_plugin('ResetMethod', self.options.forced_reset_type, disk=self.disk)
        else:
            host_tests_plugins.call_plugin('ResetMethod', 'default', serial=self.serial)
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


class TestResults:
    """ Test results set by host tests
    """
    def __init__(self):
        self.RESULT_SUCCESS = 'success'
        self.RESULT_FAILURE = 'failure'
        self.RESULT_ERROR = 'error'
        self.RESULT_IO_SERIAL = 'ioerr_serial'


class Test(TestResults):
    """ Base class for host test's test runner
    """
    def __init__(self):
        self.mbed = Mbed()

    def run(self):
        """ Test runner for host test. This function will start executing
            test and forward test result via serial port to test suite
        """
        try:
            result = self.test()
            self.print_result(self.RESULT_SUCCESS if result else self.RESULT_FAILURE)
        except Exception, e:
            print str(e)
            self.print_result(self.RESULT_ERROR)

    def setup(self):
        """ Setup and check if configuration for test is correct. E.g. if serial port can be opened
        """
        result = True
        if not self.mbed.serial:
            result = False
            self.print_result(self.RESULT_IO_SERIAL)
        return result

    def notify(self, message):
        """ On screen notification function
        """
        print message
        stdout.flush()

    def print_result(self, result):
        """ Test result unified printing function
        """
        self.notify("\n{{%s}}\n{{end}}" % result)


class DefaultTest(Test):
    """ Test class with serial port initialization
    """
    def __init__(self):
        TestResults.__init__(self)
        Test.__init__(self)
        serial_init_res = self.mbed.init_serial()
        if not serial_init_res:
            self.print_result(self.RESULT_IO_SERIAL)
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
                    self.print_result(self.RESULT_IO_SERIAL)
                    break
                stdout.write(c)
                stdout.flush()
        except KeyboardInterrupt, _:
            self.notify("\r\n[CTRL+C] exit")


if __name__ == '__main__':
    Simple().run()
