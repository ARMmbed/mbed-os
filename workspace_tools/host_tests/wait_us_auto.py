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

from host_test import Test, DefaultTest
from time import time
from sys import stdout

class WaitusTest(DefaultTest):
    def print_result(self, result):
       print "\n{%s}\n{end}" % result

    def run(self):
        test_result = True
        # First character to start test (to know after reset when test starts)
        self.mbed.serial.timeout = None
        c = self.mbed.serial.read(1)
        print "Test started"
        start_serial_pool = start = time();
        for i in range(0, 10):
            c = self.mbed.serial.read(1)
            if i > 2: # we will ignore first few measurements
                delta = time() - start
                deviation = abs(delta - 1) 
                deviation_ok = True if delta > 0 and deviation <= 0.5 else False # +/-5%
                test_result = test_result and deviation_ok
                msg = "OK" if deviation_ok else "FAIL"
                print ". in %.2f sec (%.2f) [%s]" % (delta, abs(delta - 0.95), msg)
            else:
                print "."
            start = time();
        measurement_time = time() - start_serial_pool
        print "Completed in %.2f sec" % (measurement_time)
        
        if test_result: # All numbers are the same
            self.print_result('success')
        else:
            self.print_result('failure')
        stdout.flush()

if __name__ == '__main__':
    WaitusTest().run()
