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
        result = True
        str = ''
        for i in range(3):
            c = self.mbed.serial_read(128)
            if c is None:
                self.print_result("ioerr_serial")
                return
            else:
                str += c
            # Check for expected and unexpected prints in Mbed output
            if "re-routing stdout to /null" not in str:
                result = False
            if "printf redirected to /null" in str:
                result = False
            if "{failure}" in str:
                result = False
            if not result:
                break
        # Data from serial received correctly
        print "Received %d bytes:"% len(str)
        print str
        stdout.flush()
        if result:
            self.print_result('success')
        else:
            self.print_result('failure')

if __name__ == '__main__':
    DevNullTest().run()
