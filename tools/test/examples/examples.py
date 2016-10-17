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
    compile_cmd.set_defaults(fn=do_compile)
    compile_cmd.add_argument(
        "toolchains", nargs="*", default=SUPPORTED_TOOLCHAINS,
        type=argparse_force_uppercase_type(SUPPORTED_TOOLCHAINS,
                                           "toolchain"))
    args = parser.parse_args()
    config = json.load(open(os.path.join(os.path.dirname(__file__),
                               args.config)))

    return args.fn(args, config)

    
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
