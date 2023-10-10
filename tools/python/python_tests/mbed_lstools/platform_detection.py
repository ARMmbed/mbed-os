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
import copy
from mock import patch, mock_open, DEFAULT

from mbed_lstools.lstools_base import MbedLsToolsBase

TEST_DATA_PATH = 'test_data'

class DummyLsTools(MbedLsToolsBase):
    return_value = []
    def find_candidates(self):
        return self.return_value

try:
    basestring
except NameError:
    # Python 3
    basestring = str


def get_case_insensitive_path(path, file_name):
    for entry in os.listdir(path):
        if entry.lower() == file_name.lower():
            return os.path.join(path, entry)

    raise Exception('No matching file for %s found in $s' % (file_name, path))


class PlatformDetectionTestCase(unittest.TestCase):
    """ Basic test cases checking trivial asserts
    """

    def setUp(self):
        self.base = DummyLsTools()

    def tearDown(self):
        pass

    def run_test(self, test_data_case, candidate_data, expected_data):
        # Add necessary candidate data
        candidate_data['mount_point'] = 'dummy_mount_point'

        # Find the test data in the test_data folder
        test_script_path = os.path.dirname(os.path.abspath(__file__))
        test_data_path = os.path.join(test_script_path, TEST_DATA_PATH)
        test_data_cases = os.listdir(test_data_path)
        self.assertTrue(test_data_case in test_data_cases, 'Expected %s to be present in %s folder' % (test_data_case, test_data_path))
        test_data_case_path = os.path.join(test_data_path, test_data_case)

        # NOTE a limitation of this mocked test is that it only allows mocking of one directory level.
        # This is enough at the moment because all firmwares seem to use a flat file structure.
        # If this changes in the future, this mocking framework can be extended to support this.

        test_data_case_file_names = os.listdir(test_data_case_path)
        mocked_open_file_paths = [os.path.join(candidate_data['mount_point'], file_name ) for file_name in test_data_case_file_names]

        # Setup all the mocks
        self.base.return_value = [candidate_data]

        def do_open(path, mode='r'):
            file_name = os.path.basename(path)
            try:
                with open(get_case_insensitive_path(test_data_case_path, file_name), 'r') as test_data_file:
                    test_data_file_data = test_data_file.read()
            except OSError:
                raise OSError("(mocked open) No such file or directory: '%s'" % (path))

            file_object = mock_open(read_data=test_data_file_data).return_value
            file_object.__iter__.return_value = test_data_file_data.splitlines(True)
            return file_object

        with patch("mbed_lstools.lstools_base.MbedLsToolsBase.mount_point_ready") as _mpr,\
             patch('mbed_os_tools.detect.lstools_base.open', do_open) as _,\
             patch("mbed_os_tools.detect.lstools_base.listdir") as _listdir:
            _mpr.return_value = True
            _listdir.return_value = test_data_case_file_names
            results = self.base.list_mbeds(read_details_txt=True)

            # There should only ever be one result
            self.assertEqual(len(results), 1)
            actual = results[0]

            expected_keys = list(expected_data.keys())
            actual_keys = list(actual.keys())
            differing_map = {}
            for key in expected_keys:
                actual_value = actual.get(key)
                if actual_value != expected_data[key]:
                    differing_map[key] = (actual_value, expected_data[key])


            if differing_map:
                differing_string = ''
                for differing_key in sorted(list(differing_map.keys())):
                    actual, expected = differing_map[differing_key]
                    differing_string += '    "%s": "%s" (expected "%s")\n' % (differing_key, actual, expected)

                assert_string = 'Expected data mismatch:\n\n{\n%s}' % (differing_string)
                self.assertTrue(False, assert_string)



    def test_efm32pg_stk3401_jlink(self):
        self.run_test('efm32pg_stk3401_jlink', {
            'target_id_usb_id': u'000440074453',
            'vendor_id': '1366',
            'product_id': '1015'
        }, {
            'platform_name': 'EFM32PG_STK3401',
            'device_type': 'jlink',
            'target_id': '2035022D000122D5D475113A',
            'target_id_usb_id': '000440074453',
            'target_id_mbed_htm': '2035022D000122D5D475113A'
        })

    def test_lpc1768(self):
        self.run_test('lpc1768', {
            'target_id_usb_id': u'101000000000000000000002F7F20DF3',
            'vendor_id': '0d28',
            'product_id': '0204'
        }, {
            'platform_name': 'LPC1768',
            'device_type': 'daplink',
            'target_id': '101000000000000000000002F7F20DF3d51e6be5ac41795761dc44148e3b7000',
            'target_id_usb_id': '101000000000000000000002F7F20DF3',
            'target_id_mbed_htm': '101000000000000000000002F7F20DF3d51e6be5ac41795761dc44148e3b7000'
        })

    def test_nucleo_f411re_stlink(self):
        self.run_test('nucleo_f411re_stlink', {
            'target_id_usb_id': u'0671FF554856805087112815',
            'vendor_id': '0483',
            'product_id': '374b'
        }, {
            'platform_name': 'NUCLEO_F411RE',
            'device_type': 'stlink',
            'target_id': '07400221076061193824F764',
            'target_id_usb_id': '0671FF554856805087112815',
            'target_id_mbed_htm': '07400221076061193824F764'
        })

    def test_nrf51_microbit(self):
        self.run_test('nrf51_microbit', {
            'target_id_usb_id': u'9900007031324e45000f9019000000340000000097969901',
            'vendor_id': '0d28',
            'product_id': '0204'
        }, {
            'platform_name': 'NRF51_MICROBIT',
            'device_type': 'daplink',
            'target_id': '9900007031324e45000f9019000000340000000097969901',
            'target_id_usb_id': '9900007031324e45000f9019000000340000000097969901'
        })

    def test_k64f_daplink(self):
        self.run_test('k64f_daplink', {
            'target_id_usb_id': u'0240000032044e45000a700a997b00356781000097969900',
            'vendor_id': '0d28',
            'product_id': '0204'
        }, {
            'platform_name': 'K64F',
            'device_type': 'daplink',
            'target_id': '0240000032044e45000a700a997b00356781000097969900',
            'target_id_usb_id': '0240000032044e45000a700a997b00356781000097969900',
            'target_id_mbed_htm': '0240000032044e45000a700a997b00356781000097969900'
        })

    def test_nrf52_dk_daplink(self):
        self.run_test('nrf52_dk_daplink', {
            'target_id_usb_id': u'110100004420312043574641323032203233303397969903',
            'vendor_id': '0d28',
            'product_id': '0204'
        }, {
            'platform_name': 'NRF52_DK',
            'device_type': 'daplink',
            'target_id': '110100004420312043574641323032203233303397969903',
            'target_id_usb_id': '110100004420312043574641323032203233303397969903',
            'target_id_mbed_htm': '110100004420312043574641323032203233303397969903'
        })

    def test_nrf52_dk_jlink(self):
        self.run_test('nrf52_dk_jlink', {
            'target_id_usb_id': u'000682546728',
            'vendor_id': '1366',
            'product_id': '1015'
        }, {
            'platform_name': 'NRF52_DK',
            'device_type': 'jlink',
            'target_id': '000682546728',
            'target_id_usb_id': '000682546728'
        })



if __name__ == '__main__':
    unittest.main()
