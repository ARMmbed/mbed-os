# mbed SDK
# Copyright (c) 2011-2013 ARM Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import fnmatch
import re
from os.path import normcase, join


IGNORE_FILENAME = ".mbedignore"


class MbedIgnoreSet(object):
    """
    # The mbedignore rules as an object

    A project in Mbed OS contains metadata files that exclude files from a build.
    These rules are stored as `fnmatch` patterns in text in a files named `.mbedignore`.
    """

    def __init__(self):
        self._ignore_patterns = []
        self._ignore_regex = re.compile("$^")
        self._unignore_patterns = []
        self._unignore_regex = re.compile("$^")

    def is_ignored(self, file_path):
        """Check if file path is ignored by any .mbedignore thus far"""
        ignore_match = self._ignore_regex.match(normcase(file_path))
        unignore_match = self._unignore_regex.match(normcase(file_path))

        if not ignore_match:
            return False

        if unignore_match is not None and len(unignore_match.group(0)) >= len(ignore_match.group(0)):
            return False

        return True

    def add_ignore_patterns(self, in_name, patterns):
        """Ignore all files and directories matching the paterns in 
        directories named by in_name.

        Positional arguments:
        in_name - the filename prefix that this ignore will apply to
        patterns - the list of patterns we will ignore in the future
        """

        if len(patterns) == 0:
            return

        if in_name == ".":
            self._ignore_patterns.extend(normcase(p) for p in patterns)
        else:
            self._ignore_patterns.extend(
                normcase(join(in_name, pat)) for pat in patterns)

        self._ignore_regex = re.compile("|".join(
            fnmatch.translate(p) for p in self._ignore_patterns))

    def add_unignore_patterns(self, in_name, patterns):
        """Un-ignore all files and directories matching the patterns in
        directories named by in_name.

        Un-ignore rules take precedence based on depth.  So ignoring
        a/b/* then unignoring a/b/c.cpp would allow c.cpp to build.
        But unignoring a/* then ignoring a/b/* would prevent c.cpp from building.

        Positional arguments:
        in_name - the filename prefix that this unignore will apply to
        patterns - the list of patterns we will unignore in the future
        """

        if len(patterns) == 0:
            return

        if in_name == ".":
            self._unignore_patterns.extend(normcase(p) for p in patterns)
        else:
            self._unignore_patterns.extend(
                normcase(join(in_name, pat)) for pat in patterns)

        self._unignore_regex = re.compile("|".join(
            fnmatch.translate(p) for p in self._unignore_patterns))

    def add_mbedignore(self, in_name, filepath):
        """Add a series of patterns to the ignored paths

        Positional arguments:
        in_name - the filename prefix that this ignore will apply to
        patterns - the list of patterns we will ignore in the future
        """
        with open (filepath) as f:

            ignore_patterns = []
            unignore_patterns = []

            for line in f:
                line_text = line.strip()

                if line_text == "" or line_text.startswith("#"):
                    # no content on this line
                    continue

                if line_text.startswith("!"):
                    # unignore rule
                    unignore_patterns.append(line_text[1:])
                else:
                    # ignore rule
                    ignore_patterns.append(line_text)

            self.add_ignore_patterns(in_name, ignore_patterns)
            self.add_unignore_patterns(in_name, unignore_patterns)


