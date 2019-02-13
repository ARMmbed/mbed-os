# mbed SDK
# Copyright (c) 2019 ARM Limited
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import unittest
from os.path import dirname, join
from tools.resources import Resources, FileType
from tools.notifier.mock import MockNotifier

SRC_PATHS = {
    '': join(dirname(__file__), 'source'),
    # The online compiler uses a similar mapping, with the `.lib` suffix.
    'mbed-os': join(dirname(__file__), 'mbed-os.lib'),
}


class ResourcesTest(unittest.TestCase):
    """
    Tests for Resources objects
    """

    def setUp(self):
        """
        Called before each test case

        :return:
        """
    def test_basic_scan(self):
        """
        Verify that the ordering of Target info addition and directory addition
        does not matter, so long as all the Target info and all directories are
        added.
        """
        first = Resources(MockNotifier())
        first._add_labels('TARGET', ['K64F'])
        first._add_labels('TARGET', ['FRDM'])
        for name, loc in SRC_PATHS.items():
            print(name, loc)
            first.add_directory(loc, into_path=name)
        assert("main.cpp" in first.get_file_names(FileType.CPP_SRC))

    def test_add_target_info(self):
        """
        Verify that the ordering of Target info addition and directory addition
        does not matter, so long as all the Target info and all directories are
        added.
        """
        first = Resources(MockNotifier())
        middle = Resources(MockNotifier())
        last = Resources(MockNotifier())
        first._add_labels('TARGET', ['K64F'])
        first._add_labels('TARGET', ['FRDM'])
        middle._add_labels('TARGET', ['FRDM'])
        for name, loc in SRC_PATHS.items():
            first.add_directory(loc, into_path=name)
            middle.add_directory(loc, into_path=name)
            last.add_directory(loc, into_path=name)
        middle._add_labels('TARGET', ['K64F'])
        last._add_labels('TARGET', ['K64F'])
        last._add_labels('TARGET', ['FRDM'])
        for ftype in Resources.ALL_FILE_TYPES:
            assert(set(first.get_file_refs(ftype))
                   == set(middle.get_file_refs(ftype)))
            assert(set(last.get_file_refs(ftype))
                   == set(middle.get_file_refs(ftype)))

    def test_detect_duplicates(self):
        """
        Verify that detect_duplicates finds all of the duplicate object files
        in the scanned tree.
        """
        notifier = MockNotifier()
        first = Resources(notifier)
        first._add_labels('TARGET', ['K64F'])
        for name, loc in SRC_PATHS.items():
            first.add_directory(loc, into_path=name)
        notifier.messages = []
        first.detect_duplicates()
        error_messages = "\n".join(
            m['message'] for m in notifier.messages if m['type'] == 'tool_error'
        )
        assert(" eggs.o " in error_messages)
        first._add_labels('TARGET', ['FRDM'])
        first.detect_duplicates()
        error_messages = "\n".join(
            m['message'] for m in notifier.messages if m['type'] == 'tool_error'
        )
        assert(" eggs.o " in error_messages)
        assert(" not-main.o " in error_messages)
        assert(" main.o " in error_messages)

    def test_filter_by_all_libraries(self):
        """
        Assert something
        """
        res = Resources(MockNotifier())
        res._add_labels('TARGET', ['K64F', 'FRDM'])
        for name, loc in SRC_PATHS.items():
            res.add_directory(loc, into_path=name)
        res.filter_by_libraries(res.get_file_refs(FileType.JSON))
        assert("main.cpp" in res.get_file_names(FileType.CPP_SRC))

    def test_filter_by_bm_lib(self):
        res = Resources(MockNotifier())
        res._add_labels('TARGET', ['K64F', 'FRDM'])
        for name, loc in SRC_PATHS.items():
            res.add_directory(loc, into_path=name)
        filter_by = [
            ref for ref in res.get_file_refs(FileType.JSON)
            if join("platform", "bm", "mbed_lib.json") in ref.name
        ]
        res.filter_by_libraries(filter_by)
        assert("main.cpp" not in res.get_file_names(FileType.CPP_SRC))
        assert(
            join("mbed-os", "platform", "bm", "bm.cpp")
            in res.get_file_names(FileType.CPP_SRC)
        )
        assert(
            join("mbed-os", "TARGET_FRDM", "not-main.cpp")
            in res.get_file_names(FileType.CPP_SRC)
        )


if __name__ == '__main__':
    unittest.main()
