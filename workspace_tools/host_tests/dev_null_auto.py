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

    def check_readline(self, text):
        """ Reads line from serial port and checks if text was part of read string
        """
        result = False
        c = self.mbed.serial_readline()
        if c is None:
            self.print_result("ioerr_serial")
            return None
        if text in c:
            result = True
        return result

    def run(self):
        result = True

        # Test should print some text and later stop printing
        res = self.check_readline("re-routing stdout to /null")   # 'MBED: re-routing stdout to /null'
        if not res:
            # We haven't read preamble line
            result = False
        else:
            # Check if there are printed characters
            str = ''
            for i in range(3):
                c = self.mbed.serial_read(32)
                if c is None:
                    self.print_result("ioerr_serial")
                    return
                else:
                    str += c
                if len(str) > 0:
                    result = False
                    break
            print "Received %d bytes: %s"% (len(str), str)
            stdout.flush()
        if result:
            self.print_result('success')
        else:
            self.print_result('failure')

if __name__ == '__main__':
    DevNullTest().run()
