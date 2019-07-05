#! /usr/bin/env python2
"""
mbed SDK
Copyright (c) 2019 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

LIBRARIES BUILD
"""
from __future__ import print_function, division, absolute_import

import sys
from os.path import join, abspath, dirname, normpath
import json
from shutil import copyfile
from argparse import ArgumentParser
from copy import copy

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), "../../../"))
sys.path.insert(0, ROOT)

from tools.regions import merge_region_list, UPDATE_WHITELIST
from tools.notifier.term import TerminalNotifier
from tools.config import Region
from tools.utils import split_path, run_cmd_ext, generate_update_filename


if __name__ == "__main__":
    parser = ArgumentParser()

    parser.add_argument(
        "toolchain_path",
        help="Path to the Keil folder"
    )

    parser.add_argument(
        "linker_output",
        help="Path to the built axf file"
    )

    options = parser.parse_args()
    axf_file = normpath(options.linker_output)
    output_directory, output_name, output_ext = split_path(axf_file)
    hex_file = join(output_directory, output_name + ".hex")
    combined_hex_file = join(output_directory, output_name + "_combined.hex")

    command = [
        join(normpath(options.toolchain_path), "ARM/ARMCC/bin/fromelf.exe"),
        "--i32", "--output", hex_file, axf_file
    ]
    stdout, stderr, retcode = run_cmd_ext(command)

    if retcode:
        err_msg = (
            "Failed to convert axf to hex.\r\n"
            "Command: {}\r\n"
            "retcode: {}\r\n"
            "stdout: {}\r\n"
            "stderr: {}"
        ).format(command, retcode, stdout, stderr)
        raise Exception(err_msg)

    with open(join("export_info.json"), "r") as export_info_file:
        export_info_data = json.load(export_info_file)

    region_list = [Region(*r) for r in export_info_data.get("region_list", [])]

    for index, region in enumerate(copy(region_list)):
        if region.name == "application":
            region_data = region._asdict()
            region_data["filename"] = hex_file
            region_list[index] = Region(**region_data)
            break
    else:
        raise Exception("No application region found")

    notify = TerminalNotifier()
    restrict_size = export_info_data.get("target", {}).get("restrict_size")
    merge_region_list(
        region_list, combined_hex_file, notify, restrict_size=restrict_size
    )

    update_regions = [
        r for r in region_list if r.name in UPDATE_WHITELIST
    ]

    if update_regions:
        update_res = normpath(
            join(
                output_directory,
                generate_update_filename(output_name, None)
            )
        )
        merge_region_list(
            update_regions, update_res, notify, restrict_size=restrict_size
        )

    sys.exit(0)
