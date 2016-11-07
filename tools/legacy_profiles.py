"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

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

from tools.paths import PROFILES_LEGACY


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


MBED_SDK_REV_MAP = {
    '6c34061e7c34': 121,
    '7c328cabac7e': 120,
    'aae6fcc7d9bb': 119,
    '082adc85693f': 118,
    '99a22ba036c9': 117,
    'c0f6e94411f5': 116,
    '87f2f5183dfb': 115,
    '252557024ec3': 114,
    'f141b2784e32': 113,
    '6f327212ef96': 112,
    '4336505e4b1c': 111,
    '165afa46840b': 110,
    '9296ab0bfc11': 109,
    '34e6b704fe68': 108,
    '4f6c30876dfa': 107,
    'ba1f97679dad': 106,
    '8ed44a420e5c': 105,
    'b9ad9a133dc7': 104,
    'bad568076d81': 103,
    'da0ca467f8b5': 102,
    '7cff1c4259d7': 101,
    'cbbeb26dbd92': 100,
    'dbbf35b96557': 99,
    '8ab26030e058': 98,
    '433970e64889': 97,
    '487b796308b0': 96,
    '7e07b6fb45cf': 95,
    '9ad691361fac': 94,
    'e188a91d3eaa': 93,
    '4fc01daae5a5': 92,
    '031413cf7a89': 91,
    'cb3d968589d8': 90,
    '552587b429a1': 89,
    '9327015d4013': 88,
    '6213f644d804': 87,
    '04dd9b1680ae': 86,
    '024bf7f99721': 85,
    '0b3ab51c8877': 84,
    '8a40adfe8776': 83,
    '6473597d706e': 82,
    '7d30d6019079': 81,
    '8e73be2a2ac1': 80,
    '0c05e21ae27e': 79,
    'ed8466a608b4': 78,
    '869cf507173a': 77,
    '824293ae5e43': 76,
    'dc225afb6914': 75,
    'a842253909c9': 74,
    '1efda918f0ba': 73,
    '4096f863f923': 72,
    '8fabd470bb6e': 71,
    '673126e12c73': 70,
    '4a7918f48478': 69,
    'f37f3b9c9f0b': 68,
    'a9913a65894f': 67,
    '9c8f0e3462fb': 66,
    '5798e58a58b1': 65,
    'e3affc9e7238': 64,
    'b3110cd2dd17': 63,
    '7e6c9f46b3bd': 62,
    '5e5da4a5990b': 61,
    '3d0ef94e36ec': 60,
    '0883845fe643': 59,
    '0954ebd79f59': 58,
    '0480438fc29c': 57,
    '3753e96f3c8b': 56,
    'd722ed6a4237': 55,
    '71b101360fb9': 54,
    '63cdd78b2dc1': 53,
    '09236a68d21b': 52,
    'a076018f59af': 51,
    'b60934f96c0c': 50,
    'eeb8a2a33ec9': 49,
    '49c296715c73': 48,
    '134def52cfa0': 47,
    '890817bdcffb': 46,
    '3d775a932e1d': 45,
    '24d45a770a51': 44,
    'e2ed12d17f06': 43,
    'cd19af002ccc': 42,
    '10b9abbe79a6': 41,
    '976df7c37ad5': 40,
    '737756e0b479': 39,
    '4c0c40fd0593': 38,
    '14f4805c468c': 37,
    'b4b9f287a47e': 36,
    '5284544d04b6': 35,
    '7495d544864f': 34,
    '5364839841bd': 33,
    '3b05dd009342': 32,
    'a7ef757f598c': 31,
    '3991a86798e3': 30,
    '078e4b97a13e': 29,
    '667d61c9177b': 28,
    '7110ebee3484': 27,
    '63bcd7ba4912': 26,
    '9a9732ce53a1': 25,
    'e2ac27c8e93e': 24,
    '74b8d43b5817': 23,
    '9114680c05da': 22,
    '3944f1e2fa4f': 21,
    '029aa53d7323': 20,
    'e6be4cd80aad': 19,
    'b3c9f16cbb96': 18,
    '49a220cc26e0': 17,
    '32af5db564d4': 16,
    'd1a9de3f4fe0': 15,
    '20a79241b4a0': 14,
    'a0336ede94ce': 13,
    'f63353af7be8': 12,
    '1c1ebd0324fa': 11,
    'fcb9359f0959': 10,
    'cf0d45ce28a6': 9,
    '00a04e5cd407': 8,
    '15d74db76485': 7,
    '3fd6a337c7cc': 6,
    '62573be585e9': 5,
    '5d1359a283bc': 4,
    'aefd12a1f1c5': 3,
    '969fc1867111': 2,
    '6b7f447ca868': 1,
    '82220227f4fa': 0,
}
