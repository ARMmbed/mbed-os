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


def calculate_build_profile(path):
    """Find the build profile version by scanning

    Positional arguments:
    path - the path to search for the mbed library
    """
    with open(path) as fd:
        try:
            url = open(join(path), 'r').read().strip()
            build = re.sub(r'^.+/(.*?)$', r'\1', url)
        except:
            return None
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


def find_legacy_files(path, max_depth=3):
    """Find a targets.json file within a path using depth first search

    Positional Arguments:
    path - the path to search for targets.json in

    Keyword Arguments:
    max_depth - the depth to stop searching at

    """
    fs = ['targets.json', 'mbed.profile']
    targets, profile = None, None
    if max_depth <= 0:
        return targets, profile
    if exists(join(path, 'targets.json')):
        targets = join(path, 'targets.json')
    if exists(join(path, 'mbed.profile')):
        profile = join(path, 'mbed.profile')
    for name in listdir(path):
        if (name.endswith(".bld") or
            name.endswith(".lib")) and (not isdir(join(path, name))):
            profile = profile or calculate_build_profile(join(path, name))
        if not name.startswith(".") and isdir(join(path, name)):
            t, p = find_legacy_files(join(path, name), max_depth - 1)
            targets = targets or t
            profile = profile or p

    return targets, profile


MBED_SDK_REV_MAP = load(open(join(TOOLS, "mbed-sdk-revs.json")))
