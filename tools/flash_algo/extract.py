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

from __future__ import print_function
import sys
import os
import argparse
from os.path import join, abspath, dirname
from flash_algo import PackFlashAlgo

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), "..", ".."))
sys.path.insert(0, ROOT)

from tools.targets import TARGETS
from tools.arm_pack_manager import Cache

TEMPLATE_PATH = "c_blob_mbed.tmpl"


def main():
    """Generate flash algorithms"""
    parser = argparse.ArgumentParser(description='Flash generator')
    parser.add_argument("--rebuild_all", action="store_true",
                        help="Rebuild entire cache")
    parser.add_argument("--rebuild_descriptors", action="store_true",
                        help="Rebuild descriptors")
    parser.add_argument("--target", default=None,
                        help="Name of target to generate algo for")
    parser.add_argument("--all", action="store_true",
                        help="Build all flash algos for devcies")
    args = parser.parse_args()

    cache = Cache(True, True)
    if args.rebuild_all:
        cache.cache_everything()
        print("Cache rebuilt")
        return

    if args.rebuild_descriptors:
        cache.cache_descriptors()
        print("Descriptors rebuilt")
        return

    if args.target is None:
        device_and_filenames = [(target.device_name, target.name) for target
                                in TARGETS if hasattr(target, "device_name")]
    else:
        device_and_filenames = [(args.target, args.target.replace("/", "-"))]

    try:
        os.mkdir("output")
    except OSError:
        # Directory already exists
        pass

    for device, filename in device_and_filenames:
        dev = cache.index[device]
        binaries = cache.get_flash_algorthim_binary(device, all=True)
        algos = [PackFlashAlgo(binary.read()) for binary in binaries]
        filtered_algos = algos if args.all else filter_algos(dev, algos)
        for idx, algo in enumerate(filtered_algos):
            file_name = ("%s_%i.c" % (filename, idx)
                         if args.all or len(filtered_algos) != 1
                         else "%s.c" % filename)
            output_path = join("output", file_name)
            algo.process_template(TEMPLATE_PATH, output_path)
        print("%s: %s      \r" % (device, filename))


def filter_algos(dev, algos):
    if "memory" not in dev:
        return algos
    if "IROM1" not in dev["memory"]:
        return algos
    if "IROM2" in dev["memory"]:
        return algos

    rom_rgn = dev["memory"]["IROM1"]
    try:
        start = int(rom_rgn["start"], 0)
        size = int(rom_rgn["size"], 0)
    except ValueError:
        return algos

    matching_algos = [algo for algo in algos if 
                      algo.flash_start == start and algo.flash_size == size]
    return matching_algos if len(matching_algos) == 1 else algos


if __name__ == '__main__':
    main()
