"""
mbed SDK
Copyright (c) 2016 ARM Limited

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
import os
import json

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
sys.path.insert(0, ROOT)

import unittest
from tools.memap import MemapParser
from copy import deepcopy

"""
Tests for test_api.py
"""

class MemapParserTests(unittest.TestCase):
    """
    Test cases for Test Api
    """

    def setUp(self):
        """
        Called before each test case

        :return:
        """
        self.memap_parser = MemapParser()

        self.memap_parser.modules = {
            "mbed-os/targets/TARGET/TARGET_MCUS/api/pinmap.o": {
                ".text": 1,
                ".data": 2,
                ".bss": 3,
                ".heap": 0,
                ".stack": 0,
                ".interrupts_ram":0,
                ".init":0,
                ".ARM.extab":0,
                ".ARM.exidx":0,
                ".ARM.attributes":0,
                ".eh_frame":0,
                ".init_array":0,
                ".fini_array":0,
                ".jcr":0,
                ".stab":0,
                ".stabstr":0,
                ".ARM.exidx":0,
                ".ARM":0,
                ".interrupts":0,
                ".flash_config":0,
                "unknown":0,
                "OUTPUT":0,
                },
            "[lib]/libc.a/lib_a-printf.o": {
                ".text": 4,
                ".data": 5,
                ".bss": 6,
                ".heap": 0,
                ".stack": 0,
                ".interrupts_ram":0,
                ".init":0,
                ".ARM.extab":0,
                ".ARM.exidx":0,
                ".ARM.attributes":0,
                ".eh_frame":0,
                ".init_array":0,
                ".fini_array":0,
                ".jcr":0,
                ".stab":0,
                ".stabstr":0,
                ".ARM.exidx":0,
                ".ARM":0,
                ".interrupts":0,
                ".flash_config":0,
                "unknown":0,
                "OUTPUT":0,
                },
            "main.o": {
                ".text": 7,
                ".data": 8,
                ".bss": 0,
                ".heap": 0,
                ".stack": 0,
                ".interrupts_ram":0,
                ".init":0,
                ".ARM.extab":0,
                ".ARM.exidx":0,
                ".ARM.attributes":0,
                ".eh_frame":0,
                ".init_array":0,
                ".fini_array":0,
                ".jcr":0,
                ".stab":0,
                ".stabstr":0,
                ".ARM.exidx":0,
                ".ARM":0,
                ".interrupts":0,
                ".flash_config":0,
                "unknown":0,
                "OUTPUT":0,
                },
            "test.o": {
                ".text": 0,
                ".data": 0,
                ".bss": 0,
                ".heap": 0,
                ".stack": 0,
                ".interrupts_ram":0,
                ".init":0,
                ".ARM.extab":0,
                ".ARM.exidx":0,
                ".ARM.attributes":0,
                ".eh_frame":0,
                ".init_array":0,
                ".fini_array":0,
                ".jcr":0,
                ".stab":0,
                ".stabstr":0,
                ".ARM.exidx":0,
                ".ARM":0,
                ".interrupts":0,
                ".flash_config":0,
                "unknown":0,
                "OUTPUT":0,
                },
        }

    def tearDown(self):
        """
        Called after each test case

        :return:
        """
        pass

    def generate_test_helper(self, output_type, depth, file_output=None):
        """
        Helper that ensures that the member variables "modules" is
        unchanged after calling "generate_output"

        :param output_type: type string that is passed to "generate_output"
        :param file_output: path to output file that is passed to "generate_output"
        :return:
        """

        old_modules = deepcopy(self.memap_parser.modules)

        self.memap_parser.generate_output(output_type, depth, file_output)

        self.assertEqual(self.memap_parser.modules, old_modules,
                        "generate_output modified the 'modules' property")


    def test_report_computed(self):
        """
        Test ensures the report and summary are computed

        :return:
        """

        self.memap_parser.generate_output('table', 2)

        # Report is created after generating output
        self.assertTrue(self.memap_parser.mem_summary)
        self.assertTrue(self.memap_parser.mem_report)

    def test_generate_output_table(self):
        """
        Test ensures that an output of type "table" can be generated correctly

        :return:
        """
        depth = 2
        self.generate_test_helper('table', depth)

    def test_generate_output_json(self):
        """
        Test ensures that an output of type "json" can be generated correctly

        :return:
        """
        file_name = '.json_test_output.json'
        depth = 2
        self.generate_test_helper('json', depth, file_output=file_name)
        self.assertTrue(os.path.exists(file_name), "Failed to create json file")
        os.remove(file_name)

    def test_generate_output_csv_ci(self):
        """
        Test ensures that an output of type "csv-ci" can be generated correctly

        :return:
        """
        file_name = '.csv_ci_test_output.csv'
        depth = 2
        self.generate_test_helper('csv-ci', depth, file_output=file_name)
        self.assertTrue(os.path.exists(file_name), "Failed to create csv-ci file")
        os.remove(file_name)


if __name__ == '__main__':
    unittest.main()
