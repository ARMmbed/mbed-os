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
import re

class DetectPlatformTest(DefaultTest):
    PATTERN_MICRO_NAME = "Target '(\w+)'"
    re_detect_micro_name = re.compile(PATTERN_MICRO_NAME)

    def run(self):
        result = True

        c = self.mbed.serial_readline() # {{start}} preamble
        if c is None:
           self.print_result("ioerr_serial")
           return
        print c.strip()
        stdout.flush()

        print "HOST: Detecting target name..."
        stdout.flush()

        c = self.mbed.serial_readline()
        if c is None:
           self.print_result("ioerr_serial")
           return
        print c.strip()
        stdout.flush()

        # Check for target name
        m = self.re_detect_micro_name.search(c)
        if m and len(m.groups()):
            micro_name = m.groups()[0]
            micro_cmp = self.mbed.options.micro == micro_name
            result = result and micro_cmp
            print "HOST: MUT Target name '%s', expected '%s'... [%s]"% (micro_name, self.mbed.options.micro, "OK" if micro_cmp else "FAIL")
            stdout.flush()

        for i in range(0, 2):
            c = self.mbed.serial_readline()
            if c is None:
               self.print_result("ioerr_serial")
               return
            print c.strip()
            stdout.flush()

        if result: # Hello World received
            self.print_result('success')
        else:
            self.print_result('failure')
        stdout.flush()

if __name__ == '__main__':
    DetectPlatformTest().run()
