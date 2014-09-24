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

import re
import random
from sys import stdout
from time import time
from host_test import DefaultTest

class StdioTest(DefaultTest):
    PATTERN_INT_VALUE = "Your value was: (-?\d+)"
    re_detect_int_value = re.compile(PATTERN_INT_VALUE)

    def run(self):
        test_result = True

        c = self.mbed.serial_readline() # {{start}} preamble
        if c is None:
            self.print_result("ioerr_serial")
            return
        print c
        stdout.flush()

        for i in range(0, 10):
            random_integer = random.randint(-99999, 99999)
            print "HOST: Generated number: " + str(random_integer)
            stdout.flush()
            start = time()
            self.mbed.serial_write(str(random_integer) + "\n")

            serial_stdio_msg = self.mbed.serial_readline()
            if c is None:
                self.print_result("ioerr_serial")
                return
            delay_time = time() - start
            print serial_stdio_msg.strip()
            stdout.flush()

            # Searching for reply with scanned values
            m = self.re_detect_int_value.search(serial_stdio_msg)
            if m and len(m.groups()):
                int_value = m.groups()[0]
                int_value_cmp = random_integer == int(int_value)
                test_result = test_result and int_value_cmp
                print "HOST: Number %s read after %.3f sec ... [%s]"% (int_value, delay_time, "OK" if int_value_cmp else "FAIL")
                print
                stdout.flush()
            else:
                test_result = False
                break

        if test_result: # All numbers are the same
            self.print_result('success')
        else:
            self.print_result('failure')

if __name__ == '__main__':
    StdioTest().run()
