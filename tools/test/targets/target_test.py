#!/usr/bin/env python
"""
 mbed
 Copyright (c) 2017-2017 ARM Limited

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

import sys
from os.path import join, abspath, dirname
import unittest

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), "..", "..", ".."))
sys.path.insert(0, ROOT)

from tools.targets import TARGETS
from tools.arm_pack_manager import Cache

class TestTargets(unittest.TestCase):

    def test_device_name(self):
        """Assert device name is in a pack"""
        cache = Cache(True, True)
        named_targets = (target for target in TARGETS if
                         hasattr(target, "device_name"))
        for target in named_targets:
            self.assertTrue(target.device_name in cache.index,
                            "Target %s contains invalid device_name %s" %
                            (target.name, target.device_name))

if __name__ == '__main__':
    unittest.main()
