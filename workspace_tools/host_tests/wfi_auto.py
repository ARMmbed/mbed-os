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

import sys
import uuid
import time
from sys import stdout

class WFITest():

    def test(self, selftest):
        c = selftest.mbed.serial_readline()

        if c == None:
            selftest.notify("HOST: No output detected")
            return selftest.RESULT_IO_SERIAL

        if c.strip() != "0":
            selftest.notify("HOST: Unexpected output. Expected '0' but received '%s'" % c.strip())
            return selftest.RESULT_FAILURE

        # Wait 10 seconds to allow serial prints (indicating failure)
        selftest.mbed.set_serial_timeout(10)

        # If no characters received, pass the test
        if not selftest.mbed.serial_readline():
            selftest.notify("HOST: No further output detected")
            return selftest.RESULT_SUCCESS
        else:
            selftest.notify("HOST: Extra output detected")
            return selftest.RESULT_FAILURE
