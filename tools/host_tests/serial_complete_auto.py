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

class SerialCompleteTest(object):

    def test(self, selftest):
        strip_chars = string.whitespace + "\0"
        out_str = selftest.mbed.serial_readline()
        selftest.notify("HOST: " + out_str)

        if not out_str:
            selftest.notify("HOST: No output detected")
            return selftest.RESULT_IO_SERIAL

        out_str_stripped = out_str.strip(strip_chars)

        if out_str_stripped != "123456789":
            selftest.notify("HOST: Unexpected output. '123456789' Expected. but received '%s'" % out_str_stripped)
            return selftest.RESULT_FAILURE

        else:
            return selftest.RESULT_SUCCESS

