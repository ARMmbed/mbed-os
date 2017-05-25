from __future__ import print_function
import sys
import argparse
import json
from os.path import abspath, join, dirname

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.build_api import prepare_toolchain, scan_resources
from tools.toolchains import mbedToolchain, TOOLCHAIN_CLASSES, TOOLCHAIN_PATHS
from tools.targets import TARGET_MAP
from tools.config import ConfigException
from tools.arm_pack_manager import do_queue, Reader

def should_be_built(target_name, toolchain_name, to_find):
    if toolchain_name not in TARGET_MAP[target_name].supported_toolchains:
        return False
    k = prepare_toolchain(ROOT, "BUILD", target_name, toolchain_name,
                            notify=lambda _, __: True)
    try:
        r = scan_resources([ROOT], k)
        combo_files = set(
            r.headers + r.c_sources + r.cpp_sources + r.s_sources +
            r.objects + r.libraries + r.hex_files + r.bin_files +
            r.json_files)
        if combo_files.intersection(to_find):
            return True
    except ConfigException:
        pass
    return False

def main():
    to_build = {}
    to_find = set([abspath(f.strip()) for f in sys.stdin.readlines()])
    for target_name in TARGET_MAP:
        for toolchain_name in TOOLCHAIN_CLASSES:
            if should_be_built(target_name, toolchain_name, to_find):
                to_build.setdefault(target_name, [])
                to_build[target_name].append(toolchain_name)
    if not to_build:
        to_build = {t_name: TOOLCHAIN_CLASSES.keys() for t_name in TARGET_MAP}
    json.dump(to_build, sys.stdout, indent=True)
    print()

if __name__ == "__main__":
    main()
