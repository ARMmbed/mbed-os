"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited
SPDX-License-Identifier: Apache-2.0

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
import string
from sys import stdout

class SerialNCRXTest(object):

    def test(self, selftest):
        selftest.mbed.flush();
        # Wait 0.5 seconds to ensure mbed is listening
        time.sleep(0.5)

        #handshake with target to sync test start
        selftest.mbed.serial_write("S");

        strip_chars = string.whitespace + "\0"

        out_str = selftest.mbed.serial_readline()

        if not out_str:
            selftest.notify("HOST: No output detected")
            return selftest.RESULT_IO_SERIAL

        out_str_stripped = out_str.strip(strip_chars)

        if out_str_stripped != "RX OK - Start NC test":
            selftest.notify("HOST: Unexpected output. Expected 'RX OK - Expected' but received '%s'" % out_str_stripped)
            return selftest.RESULT_FAILURE

        # Wait 0.5 seconds to ensure mbed is listening
        time.sleep(0.5)

        selftest.mbed.serial_write("E");

        strip_chars = string.whitespace + "\0"

        out_str = selftest.mbed.serial_readline()

        if not out_str:
            selftest.notify("HOST: No output detected")
            return selftest.RESULT_IO_SERIAL

        out_str_stripped = out_str.strip(strip_chars)

        if out_str_stripped != "RX OK - Expected":
            selftest.notify("HOST: Unexpected output. Expected 'RX OK - Expected' but received '%s'" % out_str_stripped)
            return selftest.RESULT_FAILURE

        # Wait 0.5 seconds to ensure mbed is listening
        time.sleep(0.5)

        # Send character, mbed shouldn't receive
        selftest.mbed.serial_write("U");

        out_str = selftest.mbed.serial_readline()

        # If no characters received, pass the test
        if not out_str:
            selftest.notify("HOST: No further output detected")
            return selftest.RESULT_SUCCESS
        else:
            out_str_stripped = out_str.strip(strip_chars)

            if out_str_stripped == "RX OK - Unexpected":
                selftest.notify("HOST: Unexpected output returned indicating RX still functioning")
            else:
                selftest.notify("HOST: Extraneous output '%s' detected indicating unknown error" % out_str_stripped)

            return selftest.RESULT_FAILURE
