# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import unittest
from mbed_os_tools.test.host_tests_runner.host_test_default import DefaultTestSelector


class HostTestDefaultTestCase(unittest.TestCase):

    def test_os_info(self):
        expected = {
            "grm_module" : "module_name",
            "grm_host" : "10.2.123.43",
            "grm_port" : "3334",
        }

        # Case that includes an IP address but no protocol
        arg = [expected["grm_module"], expected["grm_host"], expected["grm_port"]]
        result = DefaultTestSelector._parse_grm(":".join(arg))
        self.assertEqual(result, expected)

        # Case that includes an IP address but no protocol nor a no port
        expected["grm_port"] = None
        arg = [expected["grm_module"], expected["grm_host"]]
        result = DefaultTestSelector._parse_grm(":".join(arg))
        self.assertEqual(result, expected)

        # Case that includes an IP address and a protocol
        expected["grm_host"] = "https://10.2.123.43"
        expected["grm_port"] = "443"
        arg = [expected["grm_module"], expected["grm_host"], expected["grm_port"]]
        result = DefaultTestSelector._parse_grm(":".join(arg))
        self.assertEqual(result, expected)

        # Case that includes an IP address and a protocol, but no port
        expected["grm_port"] = None
        arg = [expected["grm_module"], expected["grm_host"]]
        result = DefaultTestSelector._parse_grm(":".join(arg))
        self.assertEqual(result, expected)

if __name__ == '__main__':
    unittest.main()
