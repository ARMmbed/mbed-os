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

import random
import re
from host_test import Test, DefaultTest
from time import time
from sys import stdout

class StdioTest(DefaultTest):

    pattern_int_value = "^Your value was: (-?\d+)"
    re_detect_int_value = re.compile(pattern_int_value)

    def print_result(self, result):
       print "\n{%s}\n{end}" % result

    def run(self):
        
        test_result = True
        
        for i in range(1, 5):
            random_integer = random.randint(-10000, 10000)
            print "Generated number: " + str(random_integer)
            self.mbed.serial.write(str(random_integer) + "\n")
            serial_stdio_msg = ""
            
            ip_msg_timeout = self.mbed.options.timeout
            start_serial_pool = time();
            while (time() - start_serial_pool) < ip_msg_timeout:
                c = self.mbed.serial.read(512)
                stdout.write(c)
                stdout.flush()
                serial_stdio_msg += c
                # Searching for reply with scanned values
                m = self.re_detect_int_value.search(serial_stdio_msg)
                if m and len(m.groups()):
                    duration = time() - start_serial_pool
                    print "Number: " + str(m.groups()[0])
                    test_result = test_result and (random_integer == int(m.groups()[0]))
                    stdout.flush()
                    break
            else:
                print "Error: No IP and port information sent from server"
                self.print_result('error')
                exit(-2)

        if test_result: # All numbers are the same
            self.print_result('success')
        else:
            self.print_result('failure')
        stdout.flush()

if __name__ == '__main__':
    StdioTest().run()
