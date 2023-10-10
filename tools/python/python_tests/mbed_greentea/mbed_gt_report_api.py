"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited
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

import json
import os
import six
import sys
import tempfile
import unittest

from mock import patch
from mbed_greentea import mbed_report_api

class GreenteaReportApiFunctionality(unittest.TestCase):
    def setUp(self):
        self.test_case_data = {
            "K64F-ARM": {
                "suite": {
                    "testcase_result": {
                        "case-1": {
                            "duration": 1.00,
                            "time_start": 2.00,
                            "time_end": 3.00,
                            "result_text": "FAIL",
                            "passed": 10,
                            "failed": 10
                        },
                        "case-2": {
                            "duration": 1.00,
                            "time_start": 2.00,
                            "time_end": 3.00,
                            "result_text": "SKIPPED",
                            "passed": 10,
                            "failed": 10
                        },
                        "case-3": {
                            "duration": 1.00,
                            "time_start": 2.00,
                            "time_end": 3.00,
                            "result_text": "?",
                            "passed": 10,
                            "failed": 10
                        },
                    },
                    "single_test_output": "OK",
                    "platform_name": "K64F"
                }
            }
        }

        self.test_suite_data = {
            "K64F-ARM": {
                "test-1": {
                    "testcase_result": {
                        "build_path": ".."
                    },
                    "single_test_output": "OK",
                    "single_test_result": "OK",
                    "platform_name": "K64F",
                    "elapsed_time": 1.2,
                    "copy_method": "shell"
                },
                "test-2": {
                    "testcase_result": {
                        "build_path": ".."
                    },
                    "single_test_output": "OK",
                    "single_test_result": "OK",
                    "platform_name": "K64F",
                    "elapsed_time": 1.2,
                    "copy_method": "shell"
                }
            },
            "N32F-ARM": {
                "test-3": {
                    "testcase_result": {
                        "build_path": ".."
                    },
                    "single_test_output": "OK",
                    "single_test_result": "OK",
                    "platform_name": "N32F",
                    "elapsed_time": 1.2,
                    "copy_method": "shell"
                }
            }
        }

    def tearDown(self):
        pass

    def test_export_to_file(self):
        # Invalid filepath
        payload  = "PAYLOAD"
        filepath = "."

        old_stdout = sys.stdout
        sys.stdout = stdout_capture = six.StringIO()
        result = mbed_report_api.export_to_file(filepath, payload)
        sys.stdout = old_stdout

        command_output = stdout_capture.getvalue().splitlines()[0]
        self.assertIn("file failed:", command_output)
        self.assertEqual(result, False)

        # Valid filepath
        temp_file = tempfile.mkstemp("test_file")
        result = mbed_report_api.export_to_file(temp_file[1], payload)
        self.assertTrue(result)

        with open(temp_file[1], 'r') as f:
            read_data = f.read()

        self.assertEqual(read_data, payload)
        os.close(temp_file[0])
        os.remove(temp_file[1])


    def test_exporter_text(self):
        result, result_ = mbed_report_api.exporter_text(self.test_suite_data)

        lines = result.splitlines()
        self.assertIn("target", lines[0])
        self.assertIn("platform_name", lines[0])
        self.assertIn("test suite", lines[0])
        self.assertIn("result", lines[0])
        self.assertIn("K64F", lines[2])
        self.assertIn("test-1", lines[2])
        self.assertIn("test-2", lines[3])
        self.assertIn("test-3", lines[4])
        self.assertIn("OK", lines[2])

    def test_exporter_testcase_test(self):
        result, result_ = mbed_report_api.exporter_testcase_text(self.test_case_data)

        lines = result.splitlines()
        self.assertIn("target", lines[0])
        self.assertIn("platform_name", lines[0])
        self.assertIn("test suite", lines[0])
        self.assertIn("result", lines[0])

        line = lines[2]
        self.assertIn("K64F-ARM", line)
        self.assertIn("suite", line)
        self.assertIn("case-1", line)

    def test_exporter_testcase_junit(self):
        result = mbed_report_api.exporter_testcase_junit(self.test_case_data)
        self.assertIsNotNone(result)

        from xml.etree import ElementTree as ET
        xml = ET.fromstring(result)

        self.assertEqual(xml.tag, "testsuites")
        self.assertEqual(xml.attrib["failures"], "1")
        self.assertEqual(xml.attrib["tests"], "3")
        self.assertEqual(xml.attrib["errors"], "1")
        self.assertEqual(xml.attrib["time"], "3.0")

        child = xml[0]
        self.assertEqual(child.tag, "testsuite")
        self.assertEqual(child.attrib["failures"], "1")
        self.assertEqual(child.attrib["tests"], "3")
        self.assertEqual(child.attrib["errors"], "1")
        self.assertEqual(child.attrib["time"], "3.0")
