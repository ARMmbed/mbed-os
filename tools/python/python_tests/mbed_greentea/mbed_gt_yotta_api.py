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
import os
import shutil
import tempfile
import unittest
from mbed_greentea import mbed_yotta_api

def generate_paths_and_write(data):
    # Generate some dummy temp directories
    curr_dir = os.getcwd()
    temp0_dir = tempfile.mkdtemp()
    temp1_dir = os.mkdir(os.path.join(temp0_dir, "yotta_targets"))
    temp2_dir = os.mkdir(os.path.join(temp0_dir, "yotta_targets", "target_name"))

    with open(os.path.join(os.path.join(temp0_dir, "yotta_targets", "target_name"), "target.json"), "w") as f:
        f.write(data)

    return temp0_dir

class YottaApiFunctionality(unittest.TestCase):
    def setUp(self):
        self.curr_dir = os.getcwd()

    def tearDown(self):
        pass

    def test_build_with_yotta_invalid_target_name(self):
        res, ret = mbed_yotta_api.build_with_yotta("invalid_name", True, build_to_debug=True)
        self.assertEqual(res, False)

        res, ret = mbed_yotta_api.build_with_yotta("invalid_name", True, build_to_release=True)
        self.assertEqual(res, False)

    def test_get_platform_name_from_yotta_target_invalid_target_file(self):
        temp0_dir = generate_paths_and_write("test")

        os.chdir(temp0_dir)
        result = mbed_yotta_api.get_platform_name_from_yotta_target("target_name")
        self.assertEqual(result, None)
        os.chdir(self.curr_dir)

        shutil.rmtree(temp0_dir)

    def test_get_platform_name_from_yotta_target_missing_keywords(self):
        temp0_dir = generate_paths_and_write("{}")

        os.chdir(temp0_dir)
        result = mbed_yotta_api.get_platform_name_from_yotta_target("target_name")
        self.assertEqual(result, None)
        os.chdir(self.curr_dir)

        shutil.rmtree(temp0_dir)

    def test_get_platform_name_from_yotta_target_missing_targets(self):
        temp0_dir = generate_paths_and_write('{"keywords": []}')

        os.chdir(temp0_dir)
        result = mbed_yotta_api.get_platform_name_from_yotta_target("target_name")
        self.assertEqual(result, None)
        os.chdir(self.curr_dir)

        shutil.rmtree(temp0_dir)

    def test_get_platform_name_from_yotta_target_valid_targets(self):
        temp0_dir = generate_paths_and_write('{"keywords": ["mbed-target:K64F"]}')

        os.chdir(temp0_dir)
        result = mbed_yotta_api.get_platform_name_from_yotta_target("target_name")
        self.assertEqual(result, "K64F")
        os.chdir(self.curr_dir)

        shutil.rmtree(temp0_dir)
