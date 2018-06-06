#!/usr/bin/env python

"""
Copyright (c) 2018 ARM Limited

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
This script removes the files inside the TZ-TRNG driver that are unused by
Mbed OS.
The TZ-TRNG driver is an open-source driver (Apache 2.0) available here:
https://github.com/ARM-software/TZ-TRNG

The driver is used in Mbed OS in 800-90B TRNG mode.
It removes Makefiles, PDF, test files and FE-TRNG C files (the other TRNG mode
not used).

After the script is executed, this driver can be succesfully integrated and
compiled in a Mbed OS project and the CC_TrngGetSource function can be called,
for example in a Mbed OS HAL implementation for a new target.

This script needs to be executed at the root of the TZ-TRNG directory.
The .git folder and .gitignore file are not deleted by this script and should be
deleted manually.
"""

import os
import sys

# Files that this script will delete, if one of this files do not exist, the
# script will exit and do nothing. If all files of a directory have been
# deleted, then the directory will also be deleted.
FILES_TO_DELETE = ["TRNG_test.c",
                   "trustzone_trng_characterization_application_note_100685_0000_06_en.pdf",
                   "host/src/tests/tztrng_test/tztrng_test.c",
                   "host/src/tests/tztrng_test/Makefile",
                   "host/src/tests/tztrng_test/pal/tztrng_test_pal_api.h",
                   "host/src/tests/tztrng_test/pal/linux/tztrng_test_pal.c",
                   "host/src/tests/tztrng_test/pal/linux/tztrng_test_pal.h",
                   "host/src/tests/tztrng_test/pal/freertos/tztrng_test_pal.c",
                   "host/src/tests/tztrng_test/pal/freertos/tztrng_test_pal.h",
                   "host/src/tests/tztrng_test/tztrng_test.h",
                   "host/src/tests/tztrng_test/tztrng_test_arm.c",
                   "host/src/tztrng_lib/Makefile",
                   "host/src/tztrng_lib/llf_rnd_fetrng.c",
                   "host/Makefile.defs",
                   "host/Makefile.rules",
                   "host/Makefile.pal",
                   "host/Makefile.freertos",
                   "host/proj.cfg",
                   "build.props",
                   "proj.ext.cfg",
                   "trustzone_trng_software_integrators_manual_101049_0000_00_en.pdf"]

def show_help_exit():
    print("mbed-os.py")
    print("---------------------------------------------------------------")
    print("This script removes the files inside the TZ-TRNG driver that")
    print("are unused by Mbed OS. It needs to be executed at the root of")
    print("the TZ-TRNG driver directory.")
    print("---------------------------------------------------------------")
    print("usage: ./mbed-os.py")
    exit(1)

def main():

    # Check if the help flag has been entered
    if len(sys.argv) > 1:
        option = sys.argv[1]
        if option == "-h" or option == "--help":
            show_help_exit()

    # Check if all files exist
    for file_to_delete in FILES_TO_DELETE:
        if not os.path.exists(file_to_delete):
            print("Error: file %s does not exist." % file_to_delete)
            exit(2)

    # Delete all the files and the the empty directories
    for file_to_delete in FILES_TO_DELETE:
        dir_path = os.path.dirname(file_to_delete)
        os.remove(file_to_delete)
        # Check if the directory containing the file we just deleted has become
        # empty and delete it if it is the case
        # If dir_path is empty the file to delete is at the root
        if dir_path != "" and os.listdir(dir_path) == []:
            os.rmdir(dir_path)

if __name__ == "__main__":
    main()
