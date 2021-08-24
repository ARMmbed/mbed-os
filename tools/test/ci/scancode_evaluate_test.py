#!/usr/bin/env python
# Copyright (c) 2020 Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
import importlib
import os
import pytest

license_check = importlib.import_module("scancode-evaluate").license_check

STUBS_PATH = os.path.join(
    os.path.abspath(os.path.join(os.path.dirname(__file__))), "scancode_test"
)

HEADER_WITHOUT_SPDX = "/* Copyright (C) Arm Limited, Inc - All Rights Reserved\
 * Unauthorized copying of this. file, via any medium is strictly prohibited\
 * Proprietary and confidential\
 */"

HEADER_WITH_SPDX = "/* mbed Microcontroller Library\
 * Copyright (c) 2006-2013 ARM Limited\
 *\
 * SPDX-License-Identifier: Apache-2.0\
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

HEADER_WITH_BINARY_LICENSE = "/*\
 * Copyright (c) 2019, Arm Limited, All Rights Reserved\
 * SPDX-License-Identifier: LicenseRef-PBL\
 *\
 * This file and the related binary are licensed under the\
 * Permissive Binary License, Version 1.0 (the \"License\");\
 * you may not use these files except in compliance with the License.\
 *\
 */"

@pytest.fixture()
def create_scanned_files():
    """Create stub files.
    test3.h missing license notice
    test4.h with license notice
    test5.h with license notice
    test6.h with permissive binary license
    """
    file_paths = [
        os.path.join(STUBS_PATH, "test3.h"),
        os.path.join(STUBS_PATH, "test4.h"),
        os.path.join(STUBS_PATH, "test5.h"),
        os.path.join(STUBS_PATH, "test6.h")
    ]
    for file_path in file_paths:
        with open(file_path, "w") as new_file:
            if file_path in [os.path.join(STUBS_PATH, "test3.h")]:
                new_file.write(HEADER_WITHOUT_SPDX)
            elif file_path in [os.path.join(STUBS_PATH, "test6.h")]:
                new_file.write(HEADER_WITH_BINARY_LICENSE)
            else:
                new_file.write(HEADER_WITH_SPDX)
    yield
    for file_path in file_paths:
        os.remove(file_path)


class TestScancodeEvaluate:

    def test_missing_files_attribute(self):
        """ Missing `files` attribute in JSON.
            @inputs scancode_test/scancode_test_1.json
            @outputs -1
        """
        assert license_check(os.path.join(STUBS_PATH, "scancode_test_1.json")) == -1

    def test_various_combinations_permissive_license_with_spdx(self):
        """ Various combinations where at least one license in
            a file is permissive and has spdx in the match.identifier
            attribute.
            @inputs scancode_test/scancode_test_2.json
            @outputs 0
        """
        assert license_check(os.path.join(STUBS_PATH, "scancode_test_2.json")) == 0

    def test_missing_license_permissive_license_and_spdx(self, create_scanned_files):
        """ Test four files scanned with various issues.
            test.h: Missing license text (error count += 1)
            test3.h: Missing `Permissive` license text and `spdx` in match.identifier and not in file tested by ScanCode (error count += 1)
            test4.h: Missing `Permissive` license text and `spdx` in match.identifier but found in file tested by ScanCode (error count += 1)
            test5.h: Missing `spdx` in match.identifier but found in file tested by ScanCode. (error count += 0)
            test6.h: Matching `spdx` in match.identifier but Permissive Binary License header (error count += 0)
            @inputs scancode_test/scancode_test_2.json
            @output 3
        """
        assert license_check(os.path.join(STUBS_PATH, "scancode_test_3.json")) == 3

    def test_permissive_license_no_spdx(self, create_scanned_files):
        """ Multiple `Permissive` licenses in one file but none with `spdx` in
            match.identifier and not in file tested by ScanCode (error count += 0)
            @inputs scancode_test/scancode_test_2.json
            @outputs 0
        """
        assert license_check(os.path.join(STUBS_PATH, "scancode_test_4.json")) == 0