#!/usr/bin/env python
# Copyright (c) 2020 Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
import importlib
import os
import sys
from unittest import TestCase

# TODO: fix scancode to match python naming conventROOTi
SCANCODE_EVALUATE = importlib.import_module("scancode-evaluate")
license_check = SCANCODE_EVALUATE.license_check

ROOT = os.path.abspath(
	os.path.join(os.path.dirname(__file__))
)

# path to stub files
stub_path = ROOT + "/scancode_test/"

# template copyright notices
invalid_header_1 = "/* Copyright (C) Arm Limited, Inc - All Rights Reserved\
 * Unauthorized copying of this. file, via any medium is strictly prohibited\
 * Proprietary and confidential\
 */"

invalid_header_2 = "/* mbed Microcontroller Library\
 * Copyright (c) 2006-2013 ARM Limited\
 *\
 * Licensed under the Apache License, Version 2.0 (the \"License\");\
 * you may not use this file except in compliance with the License.\
 * You may obtain a copy of the License at\
 *\
 *     http://www.apache.org/licenses/LICENSE-2.0\
 *\
 * Unless required by applicable law or agreed to in writing, software\
 * distributed under the License is distributed on an \"AS IS\" BASIS,\
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\
 * See the License for the specific language governing permissions and\
 * limitations under the License.\
 */"


# implement test class
class TestScancodeEvaluate(TestCase):
	""" Test scancode evaluation script """

	def test_scancode_case_1(self):
		""" Test Case 1 -- faulty json file
			@inputs scancode_test_1.json
			@outputs -1 if any error in file licenses found
		"""
		expected_result = -1
		test_json = ROOT + "/scancode_test/scancode_test_1.json"

		# pass json path to test function
		result = license_check(ROOT, test_json)

		self.assertEqual(expected_result, result)

	def test_scancode_case_2(self):
		""" Test Case 2 -- no errors in license headers, try multiple types i.e Apache-2.0, BSD3
			@inputs scancode_test_2.json [4 Apache-2.0, 4 BSD-3.0]
			@outputs 0
		"""
		expected_result = 0
		test_json = ROOT + "/scancode_test/scancode_test_2.json"

		result = license_check(ROOT, test_json)
		self.assertEqual(expected_result, result, "False Negative(s)")

	def test_scancode_case_3(self):
		""" Test Case 3 -- all files containing errors
			@inputs scancode_test_3.json [2 no header, 2 non-permissive + spdx, 1 missing SPDX]
			@output 5
		"""
		# create stub files with a non-permissive license and missing spdx
		for i in range(3, 6):
			with open(stub_path + "test" + str(i) + ".h", "w") as file:
				if i == 5:
					file.write(invalid_header_2)
				else:
					file.write(invalid_header_1)

		expected_result = 7
		test_json = ROOT + "/scancode_test/scancode_test_3.json"

		result = license_check(ROOT, test_json)

		self.assertEqual(expected_result, result, "False Positive(s)")
		# delete stub files
		os.remove(stub_path + "test3.h")
		os.remove(stub_path + "test4.h")
		os.remove(stub_path + "test5.h")

	def test_scancode_case_4(self):
		""" Test Case 4 -- license header permissive and non-permissive 'license' [FP]
			@inputs scancode_test_4.json
			@outputs 0
		"""
		expected_result = 0
		test_json = ROOT + "/scancode_test/scancode_test_4.json"

		result = license_check(ROOT, test_json)

		self.assertEqual(expected_result, result, "Non-Permissive Header False Positive")
