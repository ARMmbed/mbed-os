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

from tools.build_api import get_config
from tools.targets import set_targets_json_location, Target
from tools.config import ConfigException, Config
import os, sys

# Compare the output of config against a dictionary of known good results
def compare_config(cfg, expected):
    try:
        for k in cfg:
            if cfg[k].value != expected[k]:
                return "'%s': expected '%s', got '%s'" % (k, expected[k], cfg[k].value)
    except KeyError:
        raise
        return "Unexpected key '%s' in configuration data" % k
    for k in expected:
        if k not in ["desc", "expected_macros", "expected_features"] + cfg.keys():
            return "Expected key '%s' was not found in configuration data" % k
    return ""

def test_tree(full_name, name):
    failed = 0
    sys.path.append(full_name)
    if "test_data" in sys.modules:
       del sys.modules["test_data"]
    import test_data
    # If the test defines custom targets, they must exist in a file called
    # "targets.json" in the test's directory.
    if os.path.isfile(os.path.join(full_name, "targets.json")):
        set_targets_json_location(os.path.join(full_name, "targets.json"))
    else: # uset the regular set of targets
        set_targets_json_location()
    for target, expected in test_data.expected_results.items():
        sys.stdout.write("%s:'%s'(%s) " % (name, expected["desc"], target))
        sys.stdout.flush()
        err_msg = None
        try:
            cfg, macros, features = get_config(full_name, target, "GCC_ARM")
            macros = Config.config_macros_to_macros(macros)
        except ConfigException as e:
            err_msg = e.message
        if err_msg:
            if expected.has_key("exception_msg"):
                if err_msg.find(expected["exception_msg"]) == -1:
                    print "FAILED!"
                    sys.stderr.write("    Unexpected error message!\n")
                    sys.stderr.write("    Expected: '%s'\n" % expected["exception_msg"])
                    sys.stderr.write("    Got: '%s'\n" % err_msg)
                    failed += 1
                else:
                    print "OK"
            else:
                print "FAILED!"
                sys.stderr.write("    Error while getting configuration!\n")
                sys.stderr.write("    " + err_msg + "\n")
                failed += 1
        else:
            res = compare_config(cfg, expected)
            expected_macros = expected.get("expected_macros", None)
            expected_features = expected.get("expected_features", None)

            if res:
                print "FAILED!"
                sys.stdout.write("    " + res + "\n")
                failed += 1
            elif expected_macros is not None:
                if sorted(expected_macros) != sorted(macros):
                    print "FAILED!"
                    sys.stderr.write("    List of macros doesn't match\n")
                    sys.stderr.write("    Expected: '%s'\n" % ",".join(sorted(expected_macros)))
                    sys.stderr.write("    Got: '%s'\n" % ",".join(sorted(expected_macros)))
                    failed += 1
                else:
                    print "OK"
            elif expected_features is not None:
                if sorted(expected_features) != sorted(features):
                    print "FAILED!"
                    sys.stderr.write("    List of features doesn't match\n")
                    sys.stderr.write("    Expected: '%s'\n" % ",".join(sorted(expected_features)))
                    sys.stderr.write("    Got: '%s'\n" % ",".join(sorted(expected_features)))
                    failed += 1
                else:
                    print "OK"
            else:
                print "OK"
    sys.path.remove(full_name)
    return failed

failed = 0
root_dir = os.path.abspath(os.path.dirname(__file__))
tlist = sorted(os.listdir(root_dir), key = lambda e: int(e[4:]) if e.startswith('test') else -1)
for test_name in tlist:
    full_name = os.path.join(root_dir, test_name)
    if os.path.isdir(full_name) and test_name.startswith('test'):
        failed += test_tree(full_name, test_name)
sys.exit(failed)

