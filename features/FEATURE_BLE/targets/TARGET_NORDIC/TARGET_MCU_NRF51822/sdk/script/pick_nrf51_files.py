#!/usr/bin/env python

# Copyright (c) 2015-2016 ARM Limited
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os, shutil, json, pprint, sys
from collections import OrderedDict

help_text = """
Usage: python {} [options] <full-noridc-sdk-path> <nrf51-sdk-yotta-module-path>
options: --purge   : to delete all existing files and start again
         --dry-run : to list the files to be copied but not actually copy them
""".format(os.path.basename(__file__))

# exclude path to avoid confusion over files of the same name
exclude_path = ["examples", "SVD", "s110", "s120", "s210", "s310", "nrf_soc_nosd", "serialization/connectivity",
                'components/libraries/hci/config', 'components/libraries/bootloader_dfu/ble_transport']

def find(name, path):
    paths = []
    for root, dirs, files in os.walk(path):
        if True not in [x in root for x in exclude_path]:
            if name in files:
                paths.append(os.path.join(root, name))

    if len(paths) == 0:
        print "-"*30
        print "Warning! No {} found!!!!".format(name)
        print "-"*30
        return None
    elif len(paths) > 1:
        print "-"*30
        print "Warning! More than one {} found!!!!".format(name)
        print paths
        print "-"*30
        return None
    else:
        return paths[0]

def find_dir(dir_name, path):
    paths = []
    for root, dirs, files in os.walk(path):
        if dir_name in root:
            for fn in files:
                paths.append(os.path.join(root, fn))
    return paths

if __name__ == "__main__":
    # define source and destination of copy
    arg_valid = True
    if len(sys.argv) not in [3, 4]:
        arg_valid = False
    else:
        src_folder = sys.argv[-2]
        yt_module_dir = sys.argv[-1]

        for d in [src_folder, yt_module_dir]:
            if not os.path.isdir(d):
                arg_valid = False
                print src_folder, "is not a folder"

    purge = ("--purge" in sys.argv)
    dry_run = ("--dry-run" in sys.argv)

    if not arg_valid:
        print help_text
        sys.exit(1)

    dst_folder = os.path.join(yt_module_dir, "source/nordic_sdk")

    # build a file_list from required_files.txt
    file_list = []
    with open("required_files.txt", "r") as fd:
        for line in fd:
            line = line.strip()
            if line.startswith("D "):
                directory = line.split(" ")[-1]
                file_list += find_dir(directory, src_folder)
            elif not line.startswith("#") and line != '':
                fn = os.path.basename(line).strip()
                fn = find(fn, src_folder)
                file_list.append(fn)

    # remove everything from the destination folder
    if purge and not dry_run and os.path.exists(dst_folder):
        shutil.rmtree(dst_folder)

    # copy files
    extra_includes = []
    for src in file_list:
        if src:
            rel_dst = os.path.relpath(src, src_folder)
            dst = os.path.join(dst_folder, rel_dst)
            print src, "->", dst

            directory = os.path.dirname(dst)
            if not os.path.exists(directory):
                print "Creating directory:", directory
                if not dry_run:
                    os.makedirs(directory)
            if not os.path.isfile(dst):
                print "Copying file", dst
                if not dry_run:
                    shutil.copyfile(src, dst)

            # build a list of extra includes to be added to module.json
            if dst.endswith(".h"):
                inc_rel_path = os.path.relpath(dst, yt_module_dir)
                inc_dir_path = os.path.dirname(inc_rel_path)
                if inc_dir_path not in extra_includes:
                    extra_includes.append(inc_dir_path)

    # write extraIncludes in the module.json file
    mod_json = os.path.join(yt_module_dir, "module.json")
    print "-"*30
    print "Writing extra_includes to {}".format(mod_json)
    print "-"*30
    for n in sorted(extra_includes):
        print n

    if not dry_run:
        with open(mod_json, 'r+') as fd:
            jobj = json.loads(fd.read(), object_pairs_hook=OrderedDict)
            jobj['extraIncludes'] = sorted(extra_includes)
            jdump = json.dumps(jobj, indent=2, separators=(',', ': '))
            fd.seek(0)
            fd.write(jdump)
            fd.write("\n")
            fd.truncate()
