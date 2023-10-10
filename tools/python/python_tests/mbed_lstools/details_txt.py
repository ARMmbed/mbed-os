"""
mbed SDK
Copyright (c) 2011-2018 ARM Limited

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

import unittest
import os
import errno
import logging

from mbed_lstools.main import create



class ParseMbedHTMTestCase(unittest.TestCase):
    """ Unit tests checking HTML parsing code for 'mbed.htm' files
    """

    details_txt_0226 = """Version: 0226
Build:   Aug 24 2015 17:06:30
Git Commit SHA: 27a236b9fe39c674a703c5c89655fbd26b8e27e1
Git Local mods: Yes
"""

    details_txt_0240 = """# DAPLink Firmware - see https://mbed.com/daplink
Unique ID: 0240000029164e45002f0012706e0006f301000097969900
HIF ID: 97969900
Auto Reset: 0
Automation allowed: 0
Daplink Mode: Interface
Interface Version: 0240
Git SHA: c765cbb590f57598756683254ca38b211693ae5e
Local Mods: 0
USB Interfaces: MSD, CDC, HID
Interface CRC: 0x26764ebf
"""

    def setUp(self):
        self.mbeds = create()

    def tearDown(self):
        pass

    def test_simplified_daplink_txt_content(self):
        # Fetch lines from DETAILS.TXT
        lines = self.details_txt_0226.splitlines()
        self.assertEqual(4, len(lines))

        # Check parsing content
        result = self.mbeds._parse_details(lines)
        self.assertEqual(4, len(result))
        self.assertIn('Version', result)
        self.assertIn('Build', result)
        self.assertIn('Git Commit SHA', result)
        self.assertIn('Git Local mods', result)

        # Check for daplink_version
        self.assertEqual(result['Version'], "0226")

    def test_extended_daplink_txt_content(self):
        # Fetch lines from DETAILS.TXT
        lines = self.details_txt_0240.splitlines()
        self.assertEqual(11, len(lines))

        # Check parsing content
        result = self.mbeds._parse_details(lines)
        self.assertEqual(11, len(result))   # 12th would be comment
        self.assertIn('Unique ID', result)
        self.assertIn('HIF ID', result)
        self.assertIn('Auto Reset', result)
        self.assertIn('Automation allowed', result)
        self.assertIn('Daplink Mode', result)
        self.assertIn('Interface Version', result)
        self.assertIn('Git SHA', result)
        self.assertIn('Local Mods', result)
        self.assertIn('USB Interfaces', result)
        self.assertIn('Interface CRC', result)

        # Check if we parsed comment line:
        # "# DAPLink Firmware - see https://mbed.com/daplink"
        for key in result:
            # Check if we parsed comment
            self.assertFalse(key.startswith('#'))
            # Check if we parsed
            self.assertFalse('https://mbed.com/daplink' in result[key])

        # Check for daplink_version
        # DAPlink <240 compatibility
        self.assertEqual(result['Interface Version'], "0240")
        self.assertEqual(result['Version'], "0240")

    def test_(self):
        pass

    def test_(self):
        pass

if __name__ == '__main__':
    unittest.main()
