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
import os
import sys
import shutil
import tempfile
from os.path import join, abspath, dirname
from contextlib import contextmanager
import pytest

from tools.targets import TARGETS, TARGET_MAP, Target, update_target_data
from tools.arm_pack_manager import Cache


def test_device_name():
    """Assert device name is in a pack"""
    cache = Cache(True, True)
    named_targets = (target for target in TARGETS if
                     hasattr(target, "device_name"))
    for target in named_targets:
        assert target.device_name in cache.index,\
            ("Target %s contains invalid device_name %s" %
             (target.name, target.device_name))

@contextmanager
def temp_target_file(extra_target, json_filename='custom_targets.json'):
    """Create an extra targets temp file in a context manager

    :param extra_target: the contents of the extra targets temp file
    """
    tempdir = tempfile.mkdtemp()
    try:
        targetfile = os.path.join(tempdir, json_filename)
        with open(targetfile, 'w') as f:
            f.write(extra_target)
        yield tempdir
    finally:
        # Reset extra targets
        Target.set_targets_json_location()
        # Delete temp files
        shutil.rmtree(tempdir)

def test_add_extra_targets():
    """Search for extra targets json in a source folder"""
    test_target_json = """
    { 
        "Test_Target": {
            "inherits": ["Target"]
        }
    }
    """
    with temp_target_file(test_target_json) as source_dir:
        Target.add_extra_targets(source_dir=source_dir)
        update_target_data()

        assert 'Test_Target' in TARGET_MAP
        assert TARGET_MAP['Test_Target'].core is None, \
            "attributes should be inherited from Target"

def test_modify_existing_target():
    """Set default targets file, then override base Target definition"""
    initial_target_json = """
    {
        "Target": {
            "core": null,
            "default_toolchain": "ARM",
            "supported_toolchains": null,
            "extra_labels": [],
            "is_disk_virtual": false,
            "macros": [],
            "device_has": [],
            "features": [],
            "detect_code": [],
            "public": false,
            "default_lib": "std",
            "bootloader_supported": false
        },
        "Test_Target": {
            "inherits": ["Target"],
            "core": "Cortex-M4",
            "supported_toolchains": ["ARM"]
        }
    }"""

    test_target_json = """
    { 
        "Target": {
            "core": "Cortex-M0",
            "default_toolchain": "GCC_ARM",
            "supported_toolchains": null,
            "extra_labels": [],
            "is_disk_virtual": false,
            "macros": [],
            "device_has": [],
            "features": [],
            "detect_code": [],
            "public": false,
            "default_lib": "std",
            "bootloader_supported": true
        }
    }
    """

    with temp_target_file(initial_target_json, json_filename="targets.json") as targets_dir:
        Target.set_targets_json_location(os.path.join(targets_dir, "targets.json"))
        update_target_data()
        assert TARGET_MAP["Test_Target"].core == "Cortex-M4"
        assert TARGET_MAP["Test_Target"].default_toolchain == 'ARM'
        assert TARGET_MAP["Test_Target"].bootloader_supported == False

        with temp_target_file(test_target_json) as source_dir:
            Target.add_extra_targets(source_dir=source_dir)
            update_target_data()

            assert TARGET_MAP["Test_Target"].core == "Cortex-M4"
            # The existing target should not be modified by custom targets
            assert TARGET_MAP["Test_Target"].default_toolchain != 'GCC_ARM'
            assert TARGET_MAP["Test_Target"].bootloader_supported != True
