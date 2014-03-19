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

from optparse import OptionParser
from serial import Serial
from time import sleep
from sys import stdout

class Mbed:
    """
    Base class for a host driven test
    """
    def __init__(self):
        parser = OptionParser()

        parser.add_option("-m", "--micro", dest="micro",
                      help="The target microcontroller ", metavar="MICRO")

        parser.add_option("-p", "--port", dest="port",
                      help="The serial port of the target mbed (ie: COM3)", metavar="PORT")

        parser.add_option("-d", "--disk", dest="disk",
                      help="The target disk path", metavar="DISK_PATH")

        parser.add_option("-t", "--timeout", dest="timeout",
                      help="Timeout", metavar="TIMEOUT")

        parser.add_option("-e", "--extra", dest="extra",
                      help="Extra serial port (used by some tests)", metavar="EXTRA")

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
        self.serial = Serial(self.port, timeout = 1)
        self.serial.setBaudrate(baud)
        if self.extra_port:
            self.extra_serial = Serial(self.extra_port, timeout = 1)
            self.extra_serial.setBaudrate(extra_baud)
        self.flush()

    def reset(self):
        self.serial.sendBreak()
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

    def notify(self, message):
        print message
        stdout.flush()

    def print_result(self, result):
        self.notify("\n{%s}\n{end}" % result)


class DefaultTest(Test):
    def __init__(self):
        Test.__init__(self)
        self.mbed.init_serial()
        self.mbed.reset()


class Simple(DefaultTest):
    def run(self):
        try:
            while True:
                c = self.mbed.serial.read(512)
                stdout.write(c)
                stdout.flush()
        except KeyboardInterrupt, _:
            print "\n[CTRL+c] exit"


if __name__ == '__main__':
    Simple().run()
