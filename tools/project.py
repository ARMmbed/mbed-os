""" The CLI entry point for exporting projects from the mbed tools to any of the
supported IDEs or project structures.
"""
from __future__ import print_function, absolute_import
from builtins import str

import sys
from os.path import (join, abspath, dirname, exists, basename, normpath,
                     realpath, relpath, basename)
from os import remove
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from shutil import move, rmtree
from argparse import ArgumentParser

from tools.paths import EXPORT_DIR, MBED_HAL, MBED_LIBRARIES, MBED_TARGETS_PATH
from tools.settings import BUILD_DIR
from tools.export import (
    EXPORTERS,
    mcu_ide_matrix,
    mcu_ide_list,
    export_project,
    get_exporter_toolchain,
)
from tools.tests import TESTS, TEST_MAP
from tools.tests import test_known, test_name_known, Test
from tools.targets import TARGET_NAMES, set_targets_json_location
from tools.utils import (
    argparse_filestring_type,
    argparse_profile_filestring_type,
    argparse_many,
    args_error,
)
from tools.utils import argparse_force_lowercase_type
from tools.utils import argparse_force_uppercase_type
from tools.utils import print_large_string
from tools.utils import NotSupportedException
from tools.options import extract_profile, list_profiles, extract_mcus
from tools.notifier.term import TerminalNotifier
from tools.build_profiles import find_targets_json, find_build_profile, get_toolchain_profile
from tools.toolchains import mbedToolchain

EXPORTER_ALIASES = {
    u'gcc_arm': u'make_gcc_arm',
    u'uvision': u'uvision5',
}


def resolve_exporter_alias(ide):
    if ide in EXPORTER_ALIASES:
        return EXPORTER_ALIASES[ide]
    else:
        return ide


def setup_project(
        ide,
        target,
        zip,
        program,
        source_dir,
        build,
        export_path,
):
    """Generate a name, if not provided, and find dependencies

    Positional arguments:
    ide - IDE or project structure that will soon be exported to
    target - MCU that the project will build for

    Keyword arguments:
    program - the index of a test program
    source_dir - the directory, or directories that contain all of the sources
    build - a directory that will contain the result of the export
    """
    # Some libraries have extra macros (called by exporter symbols) to we need
    # to pass them to maintain compilation macros integrity between compiled
    # library and header files we might use with it
    if source_dir:
        # --source is used to generate IDE files to toolchain directly
        # in the source tree and doesn't generate zip file
        project_dir = export_path or source_dir[0]
        if program:
            project_name = TESTS[program]
        else:
            project_name = basename(normpath(realpath(source_dir[0])))
        if zip:
            src_paths = {path.strip(".\\/"): [path] for path in source_dir}
        else:
            src_paths = {relpath(path, project_dir): [path] for path in source_dir}
        lib_paths = None
    else:
        test = Test(program)
        if not build:
            # Substitute the mbed library builds with their sources
            if MBED_LIBRARIES in test.dependencies:
                test.dependencies.remove(MBED_LIBRARIES)
                test.dependencies.append(MBED_HAL)
                test.dependencies.append(MBED_TARGETS_PATH)

        src_paths = [test.source_dir]
        lib_paths = test.dependencies
        project_name = "_".join([test.id, ide, target])
        project_dir = join(EXPORT_DIR, project_name)

    return project_dir, project_name, src_paths, lib_paths


def export(target, ide, build=None, src=None, macros=None, project_id=None,
           zip_proj=False, build_profile=None, export_path=None, notify=None,
           app_config=None, ignore=None):
    """Do an export of a project.

    Positional arguments:
    target - MCU that the project will compile for
    ide - the IDE or project structure to export to

    Keyword arguments:
    build - to use the compiled mbed libraries or not
    src - directory or directories that contain the source to export
    macros - extra macros to add to the project
    project_id - the name of the project
    clean - start from a clean state before exporting
    zip_proj - create a zip file or not
    ignore - list of paths to add to mbedignore

    Returns an object of type Exporter (tools/exports/exporters.py)
    """
    ###################################
    # mbed Classic/2.0/libary support #

    # Find build system profile
    profile = None
    targets_json = None
    for path in src:
        profile = find_build_profile(path) or profile
        if profile:
            targets_json = join(dirname(dirname(abspath(__file__))), 'legacy_targets.json')
        else:
            targets_json = find_targets_json(path) or targets_json

    # Apply targets.json to active targets
    if targets_json:
        notify.info("Using targets from %s" % targets_json)
        set_targets_json_location(targets_json)

    # Apply profile to toolchains
    if profile:
        def init_hook(self):
            profile_data = get_toolchain_profile(self.name, profile)
            if not profile_data:
                return
            notify.info("Using toolchain %s profile %s" % (self.name, profile))

            for k,v in profile_data.items():
                if self.flags.has_key(k):
                    self.flags[k] = v
                else:
                    setattr(self, k, v)

        mbedToolchain.init = init_hook

    # mbed Classic/2.0/libary support #
    ###################################

    project_dir, name, src, lib = setup_project(
        ide,
        target,
        bool(zip_proj),
        program=project_id,
        source_dir=src,
        build=build,
        export_path=export_path,
    )

    zip_name = name+".zip" if zip_proj else None

    return export_project(
        src,
        project_dir,
        target,
        ide,
        name=name,
        macros=macros,
        libraries_paths=lib,
        zip_proj=zip_name,
        build_profile=build_profile,
        notify=TerminalNotifier(),
        app_config=app_config,
        ignore=ignore
    )

def clean(source_dir):
    if exists(EXPORT_DIR):
        rmtree(EXPORT_DIR)
    for cls in EXPORTERS.values():
        try:
            cls.clean(basename(abspath(source_dir[0])))
        except (NotImplementedError, IOError, OSError):
            pass
    for f in list(EXPORTERS.values())[0].CLEAN_FILES:
        try:
            remove(f)
        except (IOError, OSError):
            pass

    return export_project(src, project_dir, target, ide, name=name,
                          macros=macros, libraries_paths=lib, zip_proj=zip_name,
                          build_profile=build_profile, notify=notify,
                          app_config=app_config, ignore=ignore)


def get_args(argv):
    parser = ArgumentParser()

    targetnames = TARGET_NAMES
    targetnames.sort()
    toolchainlist = list(EXPORTERS.keys()) + list(EXPORTER_ALIASES.keys())
    toolchainlist.sort()

    parser.add_argument(
        "-m", "--mcu",
        metavar="MCU",
        help="generate project for the given MCU ({})".format(
            ', '.join(targetnames))
    )

    parser.add_argument(
        "-i",
        dest="ide",
        type=argparse_force_lowercase_type(
            toolchainlist, "toolchain"),
        help="The target IDE: %s" % str(toolchainlist)
    )

    parser.add_argument(
        "-c", "--clean",
        action="store_true",
        default=False,
        help="clean the export directory"
    )

    group = parser.add_mutually_exclusive_group(required=False)
    group.add_argument(
        "-p",
        type=test_known,
        dest="program",
        help="The index of the desired test program: [0-%s]" % (len(TESTS) - 1)
    )

    group.add_argument(
        "-n",
        type=test_name_known,
        dest="program",
        help="The name of the desired test program"
    )

    parser.add_argument(
        "-b",
        dest="build",
        default=False,
        action="store_true",
        help="use the mbed library build, instead of the sources"
    )

    group.add_argument(
        "-L", "--list-tests",
        action="store_true",
        dest="list_tests",
        default=False,
        help="list available programs in order and exit"
    )

    group.add_argument(
        "-S", "--list-matrix",
        dest="supported_ides",
        default=False,
        const="matrix",
        choices=["matrix", "ides"],
        nargs="?",
        help="displays supported matrix of MCUs and IDEs"
    )

    group.add_argument(
        "--update-packs",
        dest="update_packs",
        action="store_true",
        default=False
    )

    parser.add_argument(
        "-E",
        action="store_true",
        dest="supported_ides_html",
        default=False,
        help="Generate a markdown version of the results of -S in README.md"
    )

    parser.add_argument(
        "--build",
        type=argparse_filestring_type,
        dest="build_dir",
        default=None,
        help="Directory for the exported project files"
    )

    parser.add_argument(
        "--source",
        action="append",
        type=argparse_filestring_type,
        dest="source_dir",
        default=[],
        help="The source (input) directory"
    )

    parser.add_argument(
        "-D",
        action="append",
        dest="macros",
        help="Add a macro definition"
    )

    parser.add_argument(
        "--profile",
        dest="profile",
        action="append",
        type=argparse_profile_filestring_type,
        help=("Build profile to use. Can be either path to json"
              "file or one of the default one ({})".format(
                  ", ".join(list_profiles()))),
        default=[]
    )

    parser.add_argument(
        "--app-config",
        dest="app_config",
        default=None
    )

    parser.add_argument(
        "-z",
        action="store_true",
        default=None,
        dest="zip",
    )

    parser.add_argument(
        "--ignore",
        dest="ignore",
        type=argparse_many(str),
        default=None,
        help=("Comma separated list of patterns to add to mbedignore "
              "(eg. ./main.cpp)")
    )

    return parser.parse_args(argv), parser


def main():
    """Entry point"""
    # Parse Options
    options, parser = get_args(sys.argv[1:])

    # Print available tests in order and exit
    if options.list_tests:
        print('\n'.join(str(test) for test in sorted(TEST_MAP.values())))
    elif options.supported_ides:
        if options.supported_ides == "matrix":
            print_large_string(mcu_ide_matrix())
        elif options.supported_ides == "ides":
            print(mcu_ide_list())
    elif options.supported_ides_html:
        html = mcu_ide_matrix(verbose_html=True)
        with open("README.md", "w") as readme:
            readme.write("Exporter IDE/Platform Support\n")
            readme.write("-----------------------------------\n")
            readme.write("\n")
            readme.write(html)
    elif options.update_packs:
        from tools.arm_pack_manager import Cache
        cache = Cache(True, True)
        cache.cache_everything()
    else:
        # Check required arguments
        if not options.mcu:
            args_error(parser, "argument -m/--mcu is required")
        if not options.ide:
            args_error(parser, "argument -i is required")
        if (options.program is None) and (not options.source_dir):
            args_error(parser, "one of -p, -n, or --source is required")

        if options.clean:
            clean(options.source_dir)

        ide = resolve_exporter_alias(options.ide)
        exporter, toolchain_name = get_exporter_toolchain(ide)
        profile = extract_profile(parser, options, toolchain_name, fallback="debug")
        mcu = extract_mcus(parser, options)[0]
        if not exporter.is_target_supported(mcu):
            args_error(parser, "%s not supported by %s" % (mcu, ide))

        try:
            export(
                mcu,
                ide,
                build=options.build,
                src=options.source_dir,
                macros=options.macros,
                project_id=options.program,
                zip_proj=not bool(options.source_dir) or options.zip,
                build_profile=profile,
                app_config=options.app_config,
                export_path=options.build_dir,
                ignore=options.ignore
            )
        except NotSupportedException as exc:
            args_error(parser, "%s not supported by %s" % (mcu, ide))
            print("[Not Supported] %s" % str(exc))
    exit(0)

if __name__ == "__main__":
    main()
