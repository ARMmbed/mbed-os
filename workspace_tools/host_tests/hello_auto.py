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

class HelloTest(DefaultTest):
    HELLO_WORLD = "Hello World\n"

    def run(self):
        c = self.mbed.serial_read(1)
        if c is None:
           self.print_result("ioerr_serial")
           return
        data_to_read = len(self.HELLO_WORLD)
        read_buffer = ''
        if c == '$': # target will printout TargetID e.g.: $$$$1040e649d5c09a09a3f6bc568adef61375c6
            #Read additional 39 bytes of TargetID
            if self.mbed.serial_read(39) is None:
                self.print_result("ioerr_serial")
                return
        else:
            data_to_read -= 1
            read_buffer += c
        c = self.mbed.serial_read(data_to_read)
        read_buffer += c
        if c is None:
            self.print_result("ioerr_serial")
            return
        stdout.write(read_buffer)
        if read_buffer == self.HELLO_WORLD: # Hello World received
            self.print_result('success')
        else:
            self.print_result('failure')
        stdout.flush()

if __name__ == '__main__':
    HelloTest().run()
