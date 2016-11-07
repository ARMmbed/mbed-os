"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited

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

import re
import tempfile
import colorama
from json import load

from copy import copy
from os.path import join, abspath, exists, isdir
from os import listdir, walk
import fnmatch

from tools.paths import PROFILES_LEGACY, TOOLS


def get_toolchain_profile(toolchain, profile):
    """Get the profile for use by the toolchain

    Positional Arguments:
    toolchain - Toolchoin name as a string
    profile - build profile version
    """
    if profile and exists(join(PROFILES_LEGACY, profile + ".json")):
        contents = load(open(join(PROFILES_LEGACY, profile + ".json")))
        if contents.get(toolchain, None):
            return contents[toolchain]

def find_build_profile(path):
    """Find the build profile version by scanning

    Positional arguments:
    path - the path to search for the mbed library
    """
    for build in find_build_ids(path):
        if MBED_SDK_REV_MAP.has_key(build):
            idx = MBED_SDK_REV_MAP[build]

            if idx <= 43:
                return 'v1'
            elif idx <= 68:
                return 'v2'
            elif idx <= 76:
                return 'v3'
            elif idx <= 105:
                return 'v4'
            elif idx <= 121:
                return 'v5'


def find_build_ids(path):
    """Find the hash of all libraries within the path from their .lib or .bld
    files

    Positional Arguments:
    path - the path to search for hashes
    """
    for (root, dirs, files) in walk(path):
        for d in copy(dirs):
            if d.startswith('.'):
                dirs.remove(d)

        for filename in \
            filter(lambda s: s.endswith(".bld") or s.endswith(".lib"), files):
            try:
                url = open(join(root, filename), 'r').read().strip()
                yield re.sub(r'^.+/(.*?)$', r'\1', url)
            except:
                pass


def find_targets_json(path, max_depth=2, min_depth=1):
    """Find a targets.json file within a path using depth first search

    Positional Arguments:
    path - the path to search for targets.json in

    Keyword Arguments:
    max_depth - the depth to stop searching at
    min_depth - the minimum depth to start searching at

    """
    f = 'targets.json'
    if max_depth <= 0:
        return None
    if min_depth <= 0 and exists(join(path, f)):
        return os.path.abspath(join(path, f))

    for dir in filter(isdir, listdir(path)):
        for d in filter(lambda d: d.startswith("."), dirs):
            find_targets_json(join(path, dir), max_depth - 1, min_depth - 1)

    return None


MBED_SDK_REV_MAP = load(open(join(TOOLS, "mbed-sdk-revs.json")))
