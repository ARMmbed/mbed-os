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
from time import sleep
from sys import stdout

class Mbed:
    """
    Base class for a host driven test
    """
    def __init__(self):
        parser = OptionParser()

        parser.add_option("-m", "--micro",
                          dest="micro",
                          help="The target microcontroller ",
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

        (self.options, _) = parser.parse_args()

        if self.options.port is None:
            raise Exception("The serial port of the target mbed have to be provided as command line arguments")

        self.port = self.options.port
        self.disk = self.options.disk
        self.extra_port = self.options.extra
        self.extra_serial = None
        self.serial = None
        self.timeout = 10 if self.options.timeout is None else self.options.timeout
        print 'Mbed: "%s" "%s"' % (self.port, self.disk)

    def init_serial(self, baud=9600, extra_baud=9600):
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

    def serial_read(self, count=1):
        """ Wraps self.mbed.serial object read method """
        result = None
        if self.serial:
            try:
                result = self.serial.read(count)
            except:
                result = None
        return result

    def serial_write(self, write_buffer):
        """ Wraps self.mbed.serial object write method """
        result = -1
        if self.serial:
            try:
                result = self.serial.write(write_buffer)
            except:
               result = -1
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

    def touch_file(self, path, name):
        with os.open(path, 'a'):
            os.utime(path, None)

    def reset(self):
        """ reboot.txt   - startup from standby state, reboots when in run mode.
            shutdown.txt - shutdown from run mode
            reset.txt    - reset fpga during run mode """
        if self.options.forced_reset_type:
            path = os.path.join([self.disk, self.options.forced_reset_type.lower()])
            if self.options.forced_reset_type.endswith('.txt'):
                self.touch_file(path)
        else:
            self.safe_sendBreak(self.serial)  # Instead of serial.sendBreak()
            # Give time to wait for the image loading
        sleep(2)

    def flush(self):
        self.serial.flushInput()
        self.serial.flushOutput()
        if self.extra_serial:
            self.extra_serial.flushInput()
            self.extra_serial.flushOutput()


class Test:
    def __init__(self):
        self.mbed = Mbed()

    def run(self):
        try:
            result = self.test()
            self.print_result("success" if result else "failure")
        except Exception, e:
            print str(e)
            self.print_result("error")

    def setup(self):
        """ Setup and check if configuration for test is correct. E.g. if serial port can be opened """
        result = True
        if not self.mbed.serial:
            result = False
            self.print_result("ioerr_serial")
        return result

    def notify(self, message):
        """ On screen notification function """
        print message
        stdout.flush()

    def print_result(self, result):
        """ Test result unified printing function """
        self.notify("\n{%s}\n{end}" % result)


class DefaultTest(Test):
    def __init__(self):
        Test.__init__(self)
        serial_init_res = self.mbed.init_serial()
        self.mbed.reset()


class Simple(DefaultTest):
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
