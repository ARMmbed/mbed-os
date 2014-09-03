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
import uuid
from sys import stdout
from host_test import Test


class EchoTest(Test):
    def __init__(self):
        Test.__init__(self)
        self.mbed.init_serial(115200)
        self.mbed.reset()

    def test(self):
        # Let's wait for Mbed to print its readiness, usually "{{start}}"
        if self.mbed.serial_timeout(None) is None:
            self.print_result("ioerr_serial")
            return

        c = self.mbed.serial_read(len('{{start}}'))
        if c is None:
            self.print_result("ioerr_serial")
            return
        print c
        stdout.flush()

        if self.mbed.serial_timeout(1) is None:
            self.print_result("ioerr_serial")
            return

        self.mbed.flush()
        self.notify("Starting the ECHO test")
        check = True
        for i in range(1, 100):
            TEST = str(uuid.uuid4())
            self.mbed.serial_write(TEST + "\n")
            l = self.mbed.serial.readline().strip()
            if not l: continue

            if l != TEST:
                check = False
                self.notify('"%s" != "%s"' % (l, TEST))
            else:
                if (i % 10) == 0:
                    self.notify(TEST)
        return check


if __name__ == '__main__':
    EchoTest().run()
