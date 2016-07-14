import sys
from os.path import join, abspath, dirname, exists, basename
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from shutil import move, rmtree
from argparse import ArgumentParser
from os import path

from tools.paths import EXPORT_DIR, EXPORT_WORKSPACE, EXPORT_TMP
from tools.paths import MBED_BASE, MBED_LIBRARIES
from tools.export import export, setup_user_prj, EXPORTERS, mcu_ide_matrix
from tools.utils import args_error, mkdir
from tools.tests import TESTS, Test, TEST_MAP
from tools.tests import test_known, test_name_known
from tools.targets import TARGET_NAMES
from tools.libraries import LIBRARIES
from utils import argparse_lowercase_type, argparse_uppercase_type, argparse_filestring_type, argparse_many
from utils import argparse_force_lowercase_type, argparse_force_uppercase_type



if __name__ == '__main__':
    # Parse Options
    parser = ArgumentParser()

    targetnames = TARGET_NAMES
    targetnames.sort()
    toolchainlist = EXPORTERS.keys()
    toolchainlist.sort()

    parser.add_argument("-m", "--mcu",
                      metavar="MCU",
                      default='LPC1768',
                      type=argparse_many(argparse_force_uppercase_type(targetnames, "MCU")),
                      help="generate project for the given MCU (%s)"% ', '.join(targetnames))

    parser.add_argument("-i",
                      dest="ide",
                      default='uvision',
                      type=argparse_force_lowercase_type(toolchainlist, "toolchain"),
                      help="The target IDE: %s"% str(toolchainlist))

    parser.add_argument("-c", "--clean",
                      action="store_true",
                      default=False,
                      help="clean the export directory")

    group = parser.add_mutually_exclusive_group(required=False)
    group.add_argument("-p",
                      type=test_known,
                      dest="program",
                      help="The index of the desired test program: [0-%d]"% (len(TESTS)-1))

    group.add_argument("-n",
                      type=test_name_known,
                      dest="program",
                      help="The name of the desired test program")

    parser.add_argument("-b",
                      dest="build",
                      action="store_true",
                      default=False,
                      help="use the mbed library build, instead of the sources")

    group.add_argument("-L", "--list-tests",
                      action="store_true",
                      dest="list_tests",
                      default=False,
                      help="list available programs in order and exit")

    group.add_argument("-S", "--list-matrix",
                      action="store_true",
                      dest="supported_ides",
                      default=False,
                      help="displays supported matrix of MCUs and IDEs")

    parser.add_argument("-E",
                      action="store_true",
                      dest="supported_ides_html",
                      default=False,
                      help="writes tools/export/README.md")

    parser.add_argument("--source",
                      action="append",
                      type=argparse_filestring_type,
                      dest="source_dir",
                      default=[],
                      help="The source (input) directory")

    parser.add_argument("-D",
                      action="append",
                      dest="macros",
                      help="Add a macro definition")

    options = parser.parse_args()

    # Print available tests in order and exit
    if options.list_tests is True:
        print '\n'.join(map(str, sorted(TEST_MAP.values())))
        sys.exit()

    # Only prints matrix of supported IDEs
    if options.supported_ides:
        print mcu_ide_matrix()
        exit(0)

    # Only prints matrix of supported IDEs
    if options.supported_ides_html:
        html = mcu_ide_matrix(verbose_html=True)
        try:
            with open("./export/README.md","w") as f:
                f.write("Exporter IDE/Platform Support\n")
                f.write("-----------------------------------\n")
                f.write("\n")
                f.write(html)
        except IOError as e:
            print "I/O error({0}): {1}".format(e.errno, e.strerror)
        except:
            print "Unexpected error:", sys.exc_info()[0]
            raise
        exit(0)

    # Clean Export Directory
    if options.clean:
        if exists(EXPORT_DIR):
            rmtree(EXPORT_DIR)

    # Export results
    successes = []
    failures = []
    zip = True
    clean = True

    # source_dir = use relative paths, otherwise sources are copied
    sources_relative = True if options.source_dir else False

    for mcu in options.mcu:
        # Program Number or name
        p, src, ide = options.program, options.source_dir, options.ide

        if src:
            # --source is used to generate IDE files to toolchain directly in the source tree and doesn't generate zip file
            project_dir = options.source_dir
            project_name = TESTS[p] if p else "Unnamed_project"
            project_temp = path.join(options.source_dir[0], 'projectfiles', '%s_%s' % (ide, mcu))
            mkdir(project_temp)
            lib_symbols = []
            if options.macros:
                lib_symbols += options.macros
            zip = False   # don't create zip
            clean = False # don't cleanup because we use the actual source tree to generate IDE files
        else:
            test = Test(p)

            # Some libraries have extra macros (called by exporter symbols) to we need to pass
            # them to maintain compilation macros integrity between compiled library and
            # header files we might use with it
            lib_symbols = []
            if options.macros:
                lib_symbols += options.macros
            for lib in LIBRARIES:
                if lib['build_dir'] in test.dependencies:
                    lib_macros = lib.get('macros', None)
                    if lib_macros is not None:
                        lib_symbols.extend(lib_macros)

            if not options.build:
                # Substitute the library builds with the sources
                # TODO: Substitute also the other library build paths
                if MBED_LIBRARIES in test.dependencies:
                    test.dependencies.remove(MBED_LIBRARIES)
                    test.dependencies.append(MBED_BASE)

            # Build the project with the same directory structure of the mbed online IDE
            project_name = test.id
            project_dir = [join(EXPORT_WORKSPACE, project_name)]
            project_temp = EXPORT_TMP
            setup_user_prj(project_dir[0], test.source_dir, test.dependencies)

        # Export to selected toolchain
        tmp_path, report = export(project_dir, project_name, ide, mcu, project_dir[0], project_temp, clean=clean, make_zip=zip, extra_symbols=lib_symbols, sources_relative=sources_relative)
        if report['success']:
            if not zip:
                zip_path = join(project_temp, project_name)
            else:
                zip_path = join(EXPORT_DIR, "%s_%s_%s.zip" % (project_name, ide, mcu))
                move(tmp_path, zip_path)
            successes.append("%s::%s\t%s"% (mcu, ide, zip_path))
        else:
            failures.append("%s::%s\t%s"% (mcu, ide, report['errormsg']))

    # Prints export results
    print
    if len(successes) > 0:
        print "Successful exports:"
        for success in successes:
            print "  * %s"% success
    if len(failures) > 0:
        print "Failed exports:"
        for failure in failures:
            print "  * %s"% failure
