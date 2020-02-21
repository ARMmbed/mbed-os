"""
SPDX-License-Identifier: Apache-2.0

Copyright (c) 2020 Arm Limited. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations 
"""

# Asumptions for this script: 
# 1. directory_name is scanned directory.
#  Files are copied to this directory with full tree. As result, if we find
#  license offender, we can have full path (just scrape directory_name). We do this
#  magic because scancode allows to scan directories/one file.
# 2. SPDX and license text is a must for all code files

import json
import argparse
import sys
import os.path
import logging
import re

userlog = logging.getLogger("scancode-evaluate")
userlog.setLevel(logging.INFO)
logfile = os.path.join(os.getcwd(), 'scancode-evaluate.log')
log_file_handler = logging.FileHandler(logfile, mode='w')
userlog.addHandler(log_file_handler)

MISSING_LICENSE_TEXT = "Missing license header"
MISSING_PERMISIVE_LICENSE_TEXT = "Non-permissive license"
MISSING_SPDX_TEXT = "Missing SPDX license identifier"

def license_check(directory_name, file):
    """ Check licenses in the scancode json file for specified directory

    This function does not verify if file exists, should be done prior the call.

    Args:  
    directory_name - where scancode was run, used to scrape this from paths   
    file - scancode json output file (output from scancode --license --json-pp)

    Returns:    
    0 if nothing found
    >0 - count how many license isses found
    -1 if any error in file licenses found
    """

    offenders = []
    try:
        # find all licenses in the files, must be licensed and permissive
        with open(file, 'r') as scancode_output:
            results = json.load(scancode_output)
    except ValueError:
        userlog.warning("JSON could not be decoded")
        return -1

    try:
        for file in results['files']:
            license_offender = {}
            license_offender['file'] = file
            # ignore directory, not relevant here
            if license_offender['file']['type'] == 'directory':
                continue
            if not license_offender['file']['licenses']:
                license_offender['reason'] = MISSING_LICENSE_TEXT
                offenders.append(license_offender)
                continue

            found_spdx = False
            for i in range(len(license_offender['file']['licenses'])):
                if license_offender['file']['licenses'][i]['category'] != 'Permissive':
                    license_offender['reason'] = MISSING_PERMISIVE_LICENSE_TEXT
                    offenders.append(license_offender)
                # find SPDX, it shall be one of licenses found
                if license_offender['file']['licenses'][i]['matched_rule']['identifier'].find("spdx") != -1:
                    found_spdx = True

            if not found_spdx:
                try:
                    # Issue reported here https://github.com/nexB/scancode-toolkit/issues/1913
                    # We verify here if SPDX is not really there as SDPX is part of the license text
                    # scancode has some problems detecting it properly
                    with open(os.path.join(os.path.abspath(license_offender['file']['path'])), 'r') as spdx_file_check:
                        filetext = spdx_file_check.read()
                    matches = re.findall("SPDX-License-Identifier:?", filetext)
                    if matches:
                        continue
                    license_offender['reason'] = MISSING_SPDX_TEXT
                    offenders.append(license_offender)
                except UnicodeDecodeError:
                    # not valid file for license check
                    continue
    except KeyError:
        userlog.warning("Invalid scancode json file")
        return -1

    if offenders:
        userlog.warning("Found files with missing license details, please review and fix")
        for offender in offenders:
            userlog.warning("File: " + offender['file']['path'][len(directory_name):] + " " + "reason: " + offender['reason'])
    return len(offenders)

def parse_args():
    parser = argparse.ArgumentParser(
        description="License check.")
    parser.add_argument('-f', '--file',
                        help="scancode-toolkit output json file")
    parser.add_argument('-d', '--directory_name', default="SCANCODE",
                        help='Directory name where are files being checked')
    return parser.parse_args()

if __name__ == "__main__":

    args = parse_args()
    if args.file and os.path.isfile(args.file):
        count = license_check(args.directory_name, args.file)
        if count == 0:
            sys.exit(0)
        else:
            sys.exit(-1)
    else:
        userlog.warning("Could not find the scancode json file")
        sys.exit(-1)
