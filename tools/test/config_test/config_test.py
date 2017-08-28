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

import os
import sys
import json
import pytest
from os.path import join, isfile, dirname, abspath
from tools.build_api import get_config
from tools.targets import set_targets_json_location, Target
from tools.config import ConfigException, Config

# Compare the output of config against a dictionary of known good results
def compare_config(cfg, expected):
    try:
        for k in cfg:
            if cfg[k].value != expected[k]:
                return "'%s': expected '%s', got '%s'" % (k, expected[k], cfg[k].value)
    except KeyError:
        return "Unexpected key '%s' in configuration data" % k
    for k in expected:
        if k not in ["expected_macros", "expected_features"] + cfg.keys():
            return "Expected key '%s' was not found in configuration data" % k
    return ""

def data_path(path):
    return join(path, "test_data.json")

def is_test(path):
    return isfile(data_path(path))

root_dir = abspath(dirname(__file__))

@pytest.mark.parametrize("name", filter(lambda d: is_test(join(root_dir, d)),
                                             os.listdir(root_dir)))
def test_config(name):
    test_dir = join(root_dir, name)
    test_data = json.load(open(data_path(test_dir)))
    targets_json = os.path.join(test_dir, "targets.json")
    set_targets_json_location(targets_json if isfile(targets_json) else None)
    for target, expected in test_data.items():
        try:
            cfg, macros, features = get_config(test_dir, target, "GCC_ARM")
            res = compare_config(cfg, expected)
            assert not(res), res
            expected_macros = expected.get("expected_macros", None)
            expected_features = expected.get("expected_features", None)

            if expected_macros is not None:
                macros = Config.config_macros_to_macros(macros)
                assert sorted(expected_macros) == sorted(macros)
            if expected_features is not None:
                assert sorted(expected_features) == sorted(features)
        except ConfigException as e:
            err_msg = e.message
            if "exception_msg" not in expected:
                assert not(err_msg), "Unexpected Error: %s" % e
            else:
                assert expected["exception_msg"] in err_msg
