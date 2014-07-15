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

from host_test import DefaultTest
from sys import stdout

class DevNullTest(DefaultTest):
    def run(self):
        c = self.mbed.serial_read(512)
        if c is None:
            self.print_result("ioerr_serial")
            return
        # Data from serial received correctly
        print "Received %d bytes" % len(c)
        if "{failure}" not in c:
            self.print_result('success')
        else:
            self.print_result('failure')
        stdout.flush()

if __name__ == '__main__':
    DevNullTest().run()
