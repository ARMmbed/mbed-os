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
import sys
import uuid
from sys import stdout
from host_test import Test


class EchoTest(Test):
    def __init__(self):
        Test.__init__(self)
        self.mbed.init_serial(115200)
        self.mbed.reset()
        self.TEST_LOOP_COUNT = 50

    def test(self):
        """ Test function, return True or False to get standard test notification on stdout
        """
        c = self.mbed.serial_readline() # '{{start}}'
        if c is None:
            self.print_result("ioerr_serial")
            return
        print c.strip()
        stdout.flush()

        self.mbed.flush()
        self.notify("HOST: Starting the ECHO test")
        result = True
        for i in range(0, self.TEST_LOOP_COUNT):
            TEST = str(uuid.uuid4()) + "\n"
            self.mbed.serial_write(TEST)
            c = self.mbed.serial_readline()
            if c is None:
                self.print_result("ioerr_serial")
                return
            if c.strip() != TEST.strip():
                self.notify('HOST: "%s" != "%s"'% (c, TEST))
                result = False
            else:
                sys.stdout.write('.')
                stdout.flush()
        return result


if __name__ == '__main__':
    EchoTest().run()
