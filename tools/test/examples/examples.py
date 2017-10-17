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
from tools.utils import argparse_many
from tools.build_api import get_mbed_official_release
import examples_lib as lib
from examples_lib import SUPPORTED_TOOLCHAINS, SUPPORTED_IDES

def main():
    """Entry point"""

    official_targets = get_mbed_official_release("5")
    official_target_names = [x[0] for x in official_targets]


    parser = ArgumentParser()
    parser.add_argument("-c", dest="config", default="examples.json")
    parser.add_argument("-e", "--example",
                        help=("filter the examples used in the script"),
                        type=argparse_many(lambda x: x),
                        default=[])
    subparsers = parser.add_subparsers()
    import_cmd = subparsers.add_parser("import")
    import_cmd.set_defaults(fn=do_import)
    clone_cmd = subparsers.add_parser("clone")
    clone_cmd.set_defaults(fn=do_clone)
    deploy_cmd = subparsers.add_parser("deploy")
    deploy_cmd.set_defaults(fn=do_deploy)
    version_cmd = subparsers.add_parser("tag")
    version_cmd.add_argument("tag")
    version_cmd.set_defaults(fn=do_versionning)
    compile_cmd = subparsers.add_parser("compile")
    compile_cmd.set_defaults(fn=do_compile),
    compile_cmd.add_argument(
        "toolchains", nargs="*", default=SUPPORTED_TOOLCHAINS,
        type=argparse_force_uppercase_type(SUPPORTED_TOOLCHAINS,
                                           "toolchain")),
    compile_cmd.add_argument("-m", "--mcu",
                             help=("build for the given MCU (%s)" %
                                ', '.join(official_target_names)),
                            metavar="MCU",
                            type=argparse_many(
                                argparse_force_uppercase_type(
                                    official_target_names, "MCU")),
                            default=official_target_names)

    compile_cmd.add_argument("--profile",
                             help=("build profile file"),
                             metavar="profile")

    export_cmd = subparsers.add_parser("export")
    export_cmd.set_defaults(fn=do_export),
    export_cmd.add_argument(
        "ide", nargs="*", default=SUPPORTED_IDES,
        type=argparse_force_uppercase_type(SUPPORTED_IDES,
                                           "ide"))
    export_cmd.add_argument("-m", "--mcu",
                             help=("build for the given MCU (%s)" %
                                ', '.join(official_target_names)),
                            metavar="MCU",
                            type=argparse_many(
                                argparse_force_uppercase_type(
                                    official_target_names, "MCU")),
                            default=official_target_names)
    args = parser.parse_args()
    config = json.load(open(os.path.join(os.path.dirname(__file__),
                               args.config)))

    all_examples = []
    for example in config['examples']:
        all_examples = all_examples + [basename(x['repo']) for x in lib.get_repo_list(example)]
    examples = [x for x in all_examples if x in args.example] if args.example else all_examples
    return args.fn(args, config, examples)


def do_export(args, config, examples):
    """Do export and build step"""
    results = {}
    results = lib.export_repos(config, args.ide, args.mcu, examples)

    lib.print_summary(results, export=True)
    failures = lib.get_num_failures(results, export=True)
    print("Number of failures = %d" % failures)
    return failures


def do_import(_, config, examples):
    """Do the import step of this process"""
    lib.source_repos(config, examples)
    return 0


def do_clone(_, config, examples):
    """Do the clone step of this process"""
    lib.clone_repos(config, examples)
    return 0


def do_deploy(_, config, examples):
    """Do the deploy step of this process"""
    lib.deploy_repos(config, examples)
    return 0


def do_compile(args, config, examples):
    """Do the compile step"""
    results = {}
    results = lib.compile_repos(config, args.toolchains, args.mcu, args.profile, examples)
    
    lib.print_summary(results)
    failures = lib.get_num_failures(results)
    print("Number of failures = %d" % failures)
    return failures 
    
def do_versionning(args, config, examples):
    """ Test update the mbed-os to the version specified by the tag """
    lib.update_mbedos_version(config, args.tag, examples)
    return 0


if __name__ == "__main__":
    sys.exit(main())
