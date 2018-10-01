"""
Copyright (c) 2018, Arm Limited
SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


UNIT TEST GENERATE NEW UNIT TESTS
"""

from __future__ import print_function
import os
import re
import logging

class UnitTestGeneratorTool(object):
    """
    Generator tool to create new unit tests from template
    """

    def _replace_content(self, template_content, dirname, classname, extension):
        if extension == "h":
            content = re.sub(r"cppfile",
                             "",
                             template_content)
        elif extension == "cpp":
            content = re.sub(r"cppfile",
                             "../dirname/template.cpp",
                             template_content)

        content = re.sub(r"headerfile", "../dirname/template.h", content)
        content = re.sub(r"dirname", dirname, content)
        content = re.sub(r"template", classname, content)

        return content

    def create(self, mbed_os_root=None, filepath=None):
        """
        Generate a unit test for a file to be tested from templates

        Keyword arguments:
        mbed_os_root - Mbed OS root directory
        filepath - Path to the file to be tested
        """

        if filepath is None:
            logging.error("No filepath given. Cannot create a new unit test.")
            return

        if mbed_os_root is None:
            mbed_os_root = \
                os.path.normpath(os.path.join(os.path.dirname(__file__),
                                              "../.."))
        if os.path.isabs(filepath):
            filepath = os.path.join(
                os.path.relpath(os.path.dirname(filepath), mbed_os_root),
                os.path.basename(filepath))

        if not re.match(r"((\w+\/)+)(\w+)\.(c|cpp|h|hpp)$", filepath):
            logging.error(
                "No proper path to source file given.")
            return

        if not os.path.isfile(os.path.join(mbed_os_root, filepath)):
            logging.warning("Cannot find source file: %s from Mbed OS.",
                            filepath)

        dir_name = re.sub(r"(\\|\/)(\w+)\.(cpp|h)$", "", filepath)

        match = re.search(r"(\\|\/)(\w+)\.(cpp|h)$", filepath)

        if match:
            class_name = match.group(2)
            file_extension = match.group(3)

        # Create directory tree if not exist.
        test_dir = os.path.join(mbed_os_root, "UNITTESTS", dir_name, class_name)

        if not os.path.exists(test_dir):
            os.makedirs(test_dir)
        else:
            logging.error("Unit tests exist already for this file.")
            return

        try:
            suite_name = ("%s/%s" % (dir_name, class_name)).replace("/", "_")

            # Test source file template
            template_source_file = \
                os.path.join(mbed_os_root,
                             "UNITTESTS/template/test_template.cpp")

            with open(template_source_file, "r") as input_file:
                template_content = input_file.read()

            output_source_file = os.path.join(test_dir,
                                              "test_%s.cpp" % class_name)

            with open(output_source_file, "w") as output_file:
                content = self._replace_content(template_content,
                                                dir_name,
                                                class_name,
                                                file_extension)

                output_file.writelines(content)

            # Definition file template
            template_source_file = \
                os.path.join(mbed_os_root,
                             "UNITTESTS/template/unittest.cmake.template")

            with open(template_source_file, "r") as input_file:
                template_content = input_file.read()

            output_source_file = os.path.join(test_dir, "unittest.cmake")

            with open(output_source_file, "w") as output_file:
                content = self._replace_content(template_content,
                                                dir_name,
                                                class_name,
                                                file_extension)

                output_file.writelines(content)

            logging.info("""
Unit test %s created.

Generated files into %s directory.
""", suite_name, test_dir)

        except IOError:
            logging.error("Cannot create files for a unit test.")
