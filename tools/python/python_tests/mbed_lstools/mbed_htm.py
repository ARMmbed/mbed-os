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

    # DAPlink <0240
    test_mbed_htm_k64f_url_str = '<meta http-equiv="refresh" content="0; url=http://mbed.org/device/?code=02400203D94B0E7724B7F3CF"/>'
    test_mbed_htm_l152re_url_str = '<meta http-equiv="refresh" content="0; url=http://mbed.org/device/?code=07100200656A9A955A0F0CB8"/>'
    test_mbed_htm_lpc1768_url_str = '<meta http-equiv="refresh" content="0; url=http://mbed.org/start?auth=101000000000000000000002F7F1869557200730298d254d3ff3509e3fe4722d&loader=11972&firmware=16457&configuration=4" />'
    test_mbed_htm_nucleo_l031k6_str = '<meta http-equiv="refresh" content="0; url=http://mbed.org/device/?code=07900221461663077952F5AA"/>'
    test_mbed_htm_nrf51_url_str = '<meta http-equiv="refresh" content="0; url=http://mbed.org/device/?code=1100021952333120353935373130313232323032AFD5DFD8"/>'

    # DAPLink 0240
    test_daplink_240_mbed_html_str = 'window.location.replace("https://mbed.org/device/?code=0240000029164e45002f0012706e0006f301000097969900?version=0240?target_id=0007ffffffffffff4e45315450090023");'

    def setUp(self):
        self.mbeds = create()

    def tearDown(self):
        pass

    def test_mbed_htm_k64f_url(self):
        target_id = self.mbeds._target_id_from_htm(self.test_mbed_htm_k64f_url_str)
        self.assertEqual('02400203D94B0E7724B7F3CF', target_id)

    def test_mbed_htm_l152re_url(self):
        target_id = self.mbeds._target_id_from_htm(self.test_mbed_htm_l152re_url_str)
        self.assertEqual('07100200656A9A955A0F0CB8', target_id)

    def test_mbed_htm_lpc1768_url(self):
        target_id = self.mbeds._target_id_from_htm(self.test_mbed_htm_lpc1768_url_str)
        self.assertEqual('101000000000000000000002F7F1869557200730298d254d3ff3509e3fe4722d', target_id)

    def test_daplink_nucleo_l031k6_url(self):
        target_id = self.mbeds._target_id_from_htm(self.test_mbed_htm_nucleo_l031k6_str)
        self.assertEqual('07900221461663077952F5AA', target_id)

    def test_daplink_240_mbed_html(self):
        target_id = self.mbeds._target_id_from_htm(self.test_daplink_240_mbed_html_str)
        self.assertEqual('0240000029164e45002f0012706e0006f301000097969900', target_id)

    def test_mbed_htm_nrf51_url(self):
        target_id = self.mbeds._target_id_from_htm(self.test_mbed_htm_nrf51_url_str)
        self.assertEqual('1100021952333120353935373130313232323032AFD5DFD8', target_id)

    def get_mbed_htm_comment_section_ver_build(self):
        # Incorrect data
        ver_bld = self.mbeds._mbed_htm_comment_section_ver_build('<!-- Version: XXXX Build: Mar 26 2014 13:22:20 -->')
        self.assertIsNone(ver_bld)

        ver_bld = self.mbeds._mbed_htm_comment_section_ver_build('<!-- mbed Platform Website and Authentication Shortcut -->')
        self.assertIsNone(ver_bld)

        ver_bld = self.mbeds._mbed_htm_comment_section_ver_build('')
        self.assertIsNone(ver_bld)

        # Correct data
        ver_bld = self.mbeds._mbed_htm_comment_section_ver_build('<!-- Version: 0200 Build: Mar 26 2014 13:22:20 -->')
        self.assertIsNotNone(ver_bld)
        self.assertEqual(('0200', 'Mar 26 2014 13:22:20'), ver_bld)

        ver_bld = self.mbeds._mbed_htm_comment_section_ver_build('<!-- Version: 0200 Build: Aug 27 2014 13:29:28 -->')
        self.assertIsNotNone(ver_bld)
        self.assertEqual(('0200', 'Aug 27 2014 13:29:28'), ver_bld)

        ver_bld = self.mbeds._mbed_htm_comment_section_ver_build('<!-- Version: 0219 Build: Feb  2 2016 15:20:54 Git Commit SHA: 0853ba0cdeae2436c52efcba0ba76a6434c200ff Git local mods:No-->')
        self.assertIsNotNone(ver_bld)
        self.assertEqual(('0219', 'Feb  2 2016 15:20:54'), ver_bld)

        ver_bld = self.mbeds._mbed_htm_comment_section_ver_build('<!-- Version: 0.14.3. build 471 -->')
        self.assertIsNotNone(ver_bld)
        self.assertEqual(('0.14.3', '471'), ver_bld)

    def test_(self):
        pass

if __name__ == '__main__':
    unittest.main()
