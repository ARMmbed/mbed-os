""" import and bulid a bunch of example programs """

from argparse import ArgumentParser
import os
from os.path import dirname, abspath, basename
import os.path
import sys
import subprocess
import json

ROOT = abspath(dirname(dirname(dirname(dirname(__file__)))))
sys.path.insert(0, ROOT)

from tools.build_api import get_mbed_official_release
from tools.targets import TARGET_MAP
from tools.utils import argparse_force_uppercase_type


EXAMPLES = json.load(open(os.path.join(os.path.dirname(__file__),
                                       "examples.json")))

def print_stuff(name, lst):
    if lst:
        print("#"*80)
        print("# {} example combinations".format(name))
        print("#")
    for thing in lst:
        print(thing)


SUPPORTED_TOOLCHAINS = ["ARM", "IAR", "GCC_ARM"]


def target_cross_toolchain(required_features, allowed_toolchains):
    """Generate pairs of target and toolchains

    Args:
    required_features - the features that must be in the features array of a
                        target
    allowed_toolchains - a list of all possible toolchains
    """
    for target, toolchains in get_mbed_official_release("5"):
        for toolchain in toolchains:
            if (toolchain in allowed_toolchains and
                all(feature in TARGET_MAP[target].features
                    for feature in required_features)):
                yield target, toolchain



def main():
    """Entry point"""
    parser = ArgumentParser()
    parser.add_argument(
        "toolchains", nargs="*", default=SUPPORTED_TOOLCHAINS,
        type=argparse_force_uppercase_type(SUPPORTED_TOOLCHAINS,
                                           "toolchain"))
    args = parser.parse_args()

    failures = []
    sucesses = []
    for example, build_features in EXAMPLES.iteritems():
        subprocess.call(["mbed-cli", "import", example])
        os.chdir(basename(example))
        for target, toolchain in target_cross_toolchain(build_features,
                                                        args.toolchains):
            proc = subprocess.Popen(["mbed-cli", "compile", "-t",
                                     toolchain, "-m", target])
            proc.wait()
            example_name = "{} {} {}".format(basename(example), target,
                                             toolchain)
            if proc.returncode:
                failures.append(example_name)
            else:
                sucesses.append(example_name)
        os.chdir("..")

    print_stuff("Passed", sucesses)
    print_stuff("Failed", failures)
    return len(failures)

if __name__ == "__main__":
    sys.exit(main())
