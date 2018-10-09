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

    def is_ignored(self, file_path):
        """Check if file path is ignored by any .mbedignore thus far"""
        return self._ignore_regex.match(normcase(file_path))

    def add_ignore_patterns(self, in_name, patterns):
        """Ignore all files and directories matching the paterns in 
        directories named by in_name.

        Positional arguments:
        in_name - the filename prefix that this ignore will apply to
        patterns - the list of patterns we will ignore in the future
        """
        if in_name == ".":
            self._ignore_patterns.extend(normcase(p) for p in patterns)
        else:
            self._ignore_patterns.extend(
                normcase(join(in_name, pat)) for pat in patterns)
        if self._ignore_patterns:
            self._ignore_regex = re.compile("|".join(
                fnmatch.translate(p) for p in self._ignore_patterns))

    def add_mbedignore(self, in_name, filepath):
        """Add a series of patterns to the ignored paths

        Positional arguments:
        in_name - the filename prefix that this ignore will apply to
        patterns - the list of patterns we will ignore in the future
        """
        with open (filepath) as f:
            patterns = [l.strip() for l in f
                        if l.strip() != "" and not l.startswith("#")]
            self.add_ignore_patterns(in_name, patterns)


