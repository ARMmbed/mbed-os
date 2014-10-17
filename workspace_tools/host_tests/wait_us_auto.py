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
from time import time
from sys import stdout

class WaitusTest(DefaultTest):
    def run(self):
        test_result = True
        # First character to start test (to know after reset when test starts)
        if self.mbed.serial_timeout(None) is None:
            self.print_result("ioerr_serial")
            return
        c = self.mbed.serial_read(1)
        if c is None:
            self.print_result("ioerr_serial")
            return
        if c == '$': # target will printout TargetID e.g.: $$$$1040e649d5c09a09a3f6bc568adef61375c6
            #Read additional 39 bytes of TargetID
            if self.mbed.serial_read(39) is None:
                self.print_result("ioerr_serial")
                return
            c = self.mbed.serial_read(1) # Re-read first 'tick'
            if c is None:
                self.print_result("ioerr_serial")
                return
        print "Test started"
        start_serial_pool = time()
        start = time()
        for i in range(0, 10):
            c = self.mbed.serial_read(1)
            if c is None:
                self.print_result("ioerr_serial")
                return
            if i > 2: # we will ignore first few measurements
                delta = time() - start
                deviation = abs(delta - 1)
                # Round values
                delta = round(delta, 2)
                deviation = round(deviation, 2)
                # Check if time measurements are in given range
                deviation_ok = True if delta > 0 and deviation <= 0.10 else False # +/-10%
                test_result = test_result and deviation_ok
                msg = "OK" if deviation_ok else "FAIL"
                print ". in %.2f sec (%.2f) [%s]" % (delta, deviation, msg)
            else:
                print ". skipped"
            stdout.flush()
            start = time()
        measurement_time = time() - start_serial_pool
        print "Completed in %.2f sec" % (measurement_time)

        if test_result: # All numbers are the same
            self.print_result('success')
        else:
            self.print_result('failure')

if __name__ == '__main__':
    WaitusTest().run()
