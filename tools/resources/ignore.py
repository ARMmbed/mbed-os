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
        self._ignore_regexes = []
        self._unignore_patterns = []
        self._unignore_regexes = []

    def is_ignored(self, file_path):
        """Check if file path is ignored by any .mbedignore thus far"""

        # find longest ignore and unignore pattern that matches the path
        ignore_match_pattern = None
        unignore_match_pattern = None

        filepath_normcase = normcase(file_path)

        for regex_index in range(0, len(self._ignore_regexes)):
            this_regex_match = self._ignore_regexes[regex_index].match(filepath_normcase)
            if this_regex_match:
                if ignore_match_pattern is None:
                    # no previous match
                    ignore_match_pattern = self._ignore_patterns[regex_index]
                elif len(self._ignore_patterns[regex_index]) > len(ignore_match_pattern):
                    # found a longer match
                    ignore_match_pattern = self._ignore_patterns[regex_index]

        for regex_index in range(0, len(self._unignore_regexes)):
            this_regex_match = self._unignore_regexes[regex_index].match(filepath_normcase)
            if this_regex_match:
                if unignore_match_pattern is None:
                    # no previous match
                    unignore_match_pattern = self._unignore_patterns[regex_index]
                elif len(self._unignore_patterns[regex_index]) > len(unignore_match_pattern):
                    # found a longer match
                    unignore_match_pattern = self._unignore_patterns[regex_index]

        if ignore_match_pattern is None:
            return False

        if unignore_match_pattern is not None and len(unignore_match_pattern) >= len(ignore_match_pattern):
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
            patterns_normpath = list(normcase(p) for p in patterns)
        else:
            patterns_normpath = list(normcase(join(in_name, pat)) for pat in patterns)

        self._ignore_patterns.extend(patterns_normpath)
        self._ignore_regexes.extend(re.compile(fnmatch.translate(p)) for p in patterns_normpath)

        #print("New ignore regex: " + str(self._ignore_regexes))


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
            patterns_normpath = list(normcase(p) for p in patterns)
        else:
            patterns_normpath = list(normcase(join(in_name, pat)) for pat in patterns)

        self._unignore_patterns.extend(patterns_normpath)
        self._unignore_regexes.extend(re.compile(fnmatch.translate(p)) for p in patterns_normpath)

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


