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


def target_cross_toolchain(allowed_toolchains,
                           features=[], targets=TARGET_MAP.keys()):
    """Generate pairs of target and toolchains

    Args:
    allowed_toolchains - a list of all possible toolchains

    Kwargs:
    features - the features that must be in the features array of a
               target
    targets - a list of available targets
    """
    for target, toolchains in get_mbed_official_release("5"):
        for toolchain in toolchains:
            if (toolchain in allowed_toolchains and
                target in targets and
                all(feature in TARGET_MAP[target].features
                    for feature in features)):
                yield target, toolchain


def main():
    """Entry point"""
    parser = ArgumentParser()
    subparsers = parser.add_subparsers()
    import_cmd = subparsers.add_parser("import")
    import_cmd.set_defaults(fn=do_import)
    compile_cmd = subparsers.add_parser("compile")
    compile_cmd.set_defaults(fn=do_compile)
    compile_cmd.add_argument(
        "toolchains", nargs="*", default=SUPPORTED_TOOLCHAINS,
        type=argparse_force_uppercase_type(SUPPORTED_TOOLCHAINS,
                                           "toolchain"))
    args = parser.parse_args()
    return args.fn(args)


def do_import(_):
    """Do the import step of this process"""
    for example, _ in EXAMPLES.iteritems():
        subprocess.call(["mbed-cli", "import", example])
    return 0


def do_compile(args):
    """Do the compile step"""
    failures = []
    sucesses = []
    for example, requirements in EXAMPLES.iteritems():
        os.chdir(basename(example))
        for target, toolchain in target_cross_toolchain(args.toolchains,
                                                        **requirements):
            proc = subprocess.Popen(["mbed-cli", "compile", "-t", toolchain,
                                     "-m", target, "--silent"])
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
