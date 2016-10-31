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

from tools.utils import argparse_force_uppercase_type
import examples_lib as lib
from examples_lib import SUPPORTED_TOOLCHAINS
from tools.export import EXPORTERS
from tools.build_api import get_mbed_official_release
from tools.targets import TARGET_MAP

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
SUPPORTED_IDES = ["iar", "uvision", "make_gcc_arm", "make_iar", "make_armc5"]


def target_cross_toolchain(allowed_toolchains,
                           features=[], targets=TARGET_MAP.keys(),
                           toolchains=SUPPORTED_TOOLCHAINS):
    """Generate pairs of target and toolchains

    Args:
    allowed_toolchains - a list of all possible toolchains

    Kwargs:
    features - the features that must be in the features array of a
               target
    targets - a list of available targets
    toolchains - a list of available toolchains
    """
    for release_target, release_toolchains in get_mbed_official_release("5"):
        for toolchain in release_toolchains:
            if (toolchain in allowed_toolchains and
                toolchain in toolchains and
                release_target in targets and
                all(feature in TARGET_MAP[release_target].features
                    for feature in features)):
                yield release_target, toolchain


def target_cross_ide(allowed_ides,
                    targets=TARGET_MAP.keys()):
    """Generate pairs of target and ides

    Args:
    allowed_ides - a list of all possible IDEs

    """
    for release_target, release_toolchains in get_mbed_official_release("5"):
        for ide in allowed_ides:
            if release_target in EXPORTERS[ide].TARGETS:
                yield release_target, ide


def main():
    """Entry point"""
    parser = ArgumentParser()
    parser.add_argument("-c", dest="config", default="examples.json")
    subparsers = parser.add_subparsers()
    import_cmd = subparsers.add_parser("import")
    import_cmd.set_defaults(fn=do_import)
    version_cmd = subparsers.add_parser("tag")
    version_cmd.add_argument("tag")
    version_cmd.set_defaults(fn=do_versionning)
    compile_cmd = subparsers.add_parser("compile")
    compile_cmd.set_defaults(fn=do_compile),
    compile_cmd.add_argument(
        "toolchains", nargs="*", default=SUPPORTED_TOOLCHAINS,
        type=argparse_force_uppercase_type(SUPPORTED_TOOLCHAINS,
                                           "toolchain")),
    export_cmd = subparsers.add_parser("export")
    export_cmd.set_defaults(fn=do_export),
    export_cmd.add_argument(
        "ide", nargs="*", default=SUPPORTED_IDES,
        type=argparse_force_uppercase_type(SUPPORTED_IDES,
                                           "ide"))
    args = parser.parse_args()
    config = json.load(open(os.path.join(os.path.dirname(__file__),
                               args.config)))
    return args.fn(args, config)


def do_export(args):

    def print_message(message, name):
        print(message+ " %s"%name)
        sys.stdout.flush()

    export_failures = []
    build_failures = []
    sucesses = []
    for example, requirements in EXAMPLES.iteritems():
        ex_name = basename(example)
        if ex_name != "mbed-os-example-blinky":
            continue
        os.chdir(ex_name)
        for target, ide in target_cross_ide(args.ide):
            example_name = "{} {} {}".format(ex_name, target,
                                             ide)
            print_message("Export:",example_name)
            proc = subprocess.Popen(["mbed-cli", "export", "-i", ide,
                                     "-m", target])
            proc.wait()
            if proc.returncode:
                export_failures.append(example_name)
                print_message("FAILURE Export:", example_name)
            else:
                print_message("SUCCESS Export:", example_name)
                print_message("Build:", example_name)
                if EXPORTERS[ide].build(ex_name):
                    print_message("FAILURE Build:", example_name)
                    build_failures.append(example_name)
                else:
                    print_message("SUCCESS Build:", example_name)
                    sucesses.append(example_name)
    print_stuff("Passed", sucesses)
    print_stuff("Failed Export", export_failures)
    print_stuff("Failed Building", build_failures)
    return len(export_failures+build_failures)

    
def do_import(_, config):
    """Do the import step of this process"""
    lib.source_repos(config)
    return 0

def do_compile(args, config):
    """Do the compile step"""
    results = {}
    results = lib.compile_repos(config, args.toolchains)
    
    lib.print_compilation_summary(results)
    failures = lib.get_num_failures(results)
    print("Number of failures = %d" % failures)
    return failures 
    
def do_versionning(args, config):
    """ Test update the mbed-os to the version specified by the tag """
    lib.update_mbedos_version(config, args.tag)        
    return 0


if __name__ == "__main__":
    sys.exit(main())
