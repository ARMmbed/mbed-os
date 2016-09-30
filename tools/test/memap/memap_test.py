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
            "Misc": {
                "unknown": 0,
                ".ARM": 8,
                ".ARM.extab": 0,
                ".init": 12,
                "OUTPUT": 0,
                ".stack": 0,
                ".eh_frame": 0,
                ".fini_array": 4,
                ".heap": 0,
                ".stabstr": 0,
                ".interrupts_ram": 0,
                ".init_array": 0,
                ".stab": 0,
                ".ARM.attributes": 7347,
                ".bss": 8517,
                ".flash_config": 16,
                ".interrupts": 1024,
                ".data": 2325,
                ".ARM.exidx": 0,
                ".text": 59906,
                ".jcr": 0
            },
            "Fill": {
                "unknown": 12,
                ".ARM": 0,
                ".ARM.extab": 0,
                ".init": 0,
                "OUTPUT": 0,
                ".stack": 0,
                ".eh_frame": 0,
                ".fini_array": 0,
                ".heap": 65536,
                ".stabstr": 0,
                ".interrupts_ram": 1024,
                ".init_array": 0,
                ".stab": 0,
                ".ARM.attributes": 0,
                ".bss": 2235,
                ".flash_config": 0,
                ".interrupts": 0,
                ".data": 3,
                ".ARM.exidx": 0,
                ".text": 136,
                ".jcr": 0
            }
        }
        
        self.memap_parser.compute_report()

    def tearDown(self):
        """
        Called after each test case

        :return:
        """
        pass
    
    def generate_test_helper(self, output_type, file_output=None):
        """
        Helper that ensures that the member variables "modules", "mem_report",
        and "mem_summary" are unchanged after calling "generate_output"
        
        :param output_type: type string that is passed to "generate_output"
        :param file_output: path to output file that is passed to "generate_output"      
        :return:
        """
        
        old_modules = deepcopy(self.memap_parser.modules)
        old_report = deepcopy(self.memap_parser.mem_report)
        old_summary = deepcopy(self.memap_parser.mem_summary)
        self.memap_parser.generate_output(output_type, file_output)
        self.assertEqual(self.memap_parser.modules, old_modules,
                         "generate_output modified the 'modules' property")
        self.assertEqual(self.memap_parser.mem_report, old_report,
                         "generate_output modified the 'mem_report' property")
        self.assertEqual(self.memap_parser.mem_summary, old_summary,
                         "generate_output modified the 'mem_summary' property")

    def test_report_computed(self):
        """
        Test ensures the report and summary are computed
        
        :return:
        """
        self.assertTrue(self.memap_parser.mem_report)
        self.assertTrue(self.memap_parser.mem_summary)
        self.assertEqual(self.memap_parser.mem_report[-1]['summary'],
                         self.memap_parser.mem_summary,
                         "mem_report did not contain a correct copy of mem_summary")
    
    def test_generate_output_table(self):
        """
        Test ensures that an output of type "table" can be generated correctly
        
        :return:
        """
        self.generate_test_helper('table')
    
    def test_generate_output_json(self):
        """
        Test ensures that an output of type "json" can be generated correctly
        
        :return:
        """
        file_name = '.json_test_output.json'
        self.generate_test_helper('json', file_output=file_name)
        self.assertTrue(os.path.exists(file_name), "Failed to create json file")
        os.remove(file_name)
    
    def test_generate_output_csv_ci(self):
        """
        Test ensures that an output of type "csv-ci" can be generated correctly
        
        :return:
        """
        file_name = '.csv_ci_test_output.csv'
        self.generate_test_helper('csv-ci', file_output=file_name)
        self.assertTrue(os.path.exists(file_name), "Failed to create csv-ci file")
        os.remove(file_name)
    

if __name__ == '__main__':
    unittest.main()
