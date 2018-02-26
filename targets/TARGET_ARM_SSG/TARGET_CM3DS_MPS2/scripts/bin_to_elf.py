#!/usr/bin/env python

"""
Copyright (c) 2017 ARM Limited

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

"""
This script parses the test_spec.json file and replace the .bin extension of
test binary paths to .elf. It allows you to use .elf file for greentea testing.
For testing, the MPS2+ should be configured to program the file mbed.elf at
root. Motherboard firmware version 2.2.6 or greater is required.
Example to launch tests with .elf files (compiled with ARM), at the root of mbed-os folder:
$ mbed test --compile -m ARM_CM3DS_MPS2 -t ARM
$ ./targets/TARGET_ARM_SSG/TARGET_CM3DS_MPS2/scripts/bin_to_elf.py BUILD/tests/ARM_CM3DS_MPS2/ARM/test_spec.json
$ mbed test --run -m ARM_CM3DS_MPS2 -t ARM
"""

import json
import sys
import os

def show_help_exit():
    print("bin_to_elf.py")
    print("---------------------------------------------------------------")
    print("This script parses the test_spec.json file and replace the .bin")
    print("extension of test binary paths to .elf")
    print("---------------------------------------------------------------")
    print("usage: ./bin_to_elf.py PATH_TO_TEST_SPEC")
    exit(1)

def bin_to_elf(test_spec):
    tests = test_spec["builds"]
    for device in tests:
        for test in tests[device]["tests"]:
            path_dict = tests[device]["tests"][test]["binaries"][0]
            old_path = path_dict["path"]
            name, extension = os.path.splitext(old_path)
            if extension == ".bin":
                path_dict["path"] = name + ".elf"

def main():
    arg_len = len(sys.argv)

    if arg_len != 2:
        show_help_exit()

    filename = sys.argv[1]

    if filename == "-h" or filename == "--help":
        show_help_exit()

    if not os.path.isfile(filename):
        print("File %s does not exist." % filename)
        exit(2)

    try:
        with open(filename, "r+") as f:
            test_spec = json.load(f)
            bin_to_elf(test_spec)
            # These two lines delete the contents of the file
            f.seek(0)
            f.truncate()
            # Write the same file with the new JSON content
            json.dump(test_spec, f, indent=2)
            print("Paths successfully changed.")
    except IOError as err:
        print("File %s can not be opened or written to (%s)." % (filename, err))
    except Exception as err:
        print("Error while parsing JSON file (%s)." % err)

if __name__ == "__main__":
    main()
