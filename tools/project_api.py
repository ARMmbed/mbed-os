import sys
from os.path import join, abspath, dirname, exists, basename
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.paths import EXPORT_WORKSPACE, EXPORT_TMP
from tools.paths import MBED_BASE, MBED_LIBRARIES
from tools.export import export, setup_user_prj
from tools.utils import mkdir
from tools.tests import Test, TEST_MAP, TESTS
from tools.libraries import LIBRARIES

try:
    import tools.private_settings as ps
except:
    ps = object()


def get_program(n):
    p = TEST_MAP[n].n
    return p


def get_test(p):
    return Test(p)


def get_test_from_name(n):
    if not n in TEST_MAP.keys():
        # Check if there is an alias for this in private_settings.py
        if getattr(ps, "test_alias", None) is not None:
            alias = ps.test_alias.get(n, "")
            if not alias in TEST_MAP.keys():
                return None
            else:
                n = alias
        else:
            return None
    return get_program(n)


def get_lib_symbols(macros, src, program):
    # Some libraries have extra macros (called by exporter symbols) to we need to pass
    # them to maintain compilation macros integrity between compiled library and
    # header files we might use with it
    lib_symbols = []
    if macros:
        lib_symbols += macros
    if src:
        return lib_symbols
    test = get_test(program)
    for lib in LIBRARIES:
        if lib['build_dir'] in test.dependencies:
            lib_macros = lib.get('macros', None)
            if lib_macros is not None:
                lib_symbols.extend(lib_macros)


def setup_project(mcu, ide, program=None, source_dir=None, build=None):

    # Some libraries have extra macros (called by exporter symbols) to we need to pass
    # them to maintain compilation macros integrity between compiled library and
    # header files we might use with it
    if source_dir:
        # --source is used to generate IDE files to toolchain directly in the source tree and doesn't generate zip file
        project_dir = source_dir
        project_name = TESTS[program] if program else "Unnamed_Project"
        project_temp = join(source_dir[0], 'projectfiles', '%s_%s' % (ide, mcu))
        mkdir(project_temp)
    else:
        test = get_test(program)
        if not build:
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

    return project_dir, project_name, project_temp


def perform_export(dir, name, ide, mcu, temp, clean=False, zip=False, lib_symbols='',
                   sources_relative=False, progen_build=False):

    tmp_path, report = export(dir, name, ide, mcu, dir[0], temp, clean=clean,
                              make_zip=zip, extra_symbols=lib_symbols, sources_relative=sources_relative,
                              progen_build=progen_build)
    return tmp_path, report


def print_results(successes, failures, skips = []):
    print
    if len(successes) > 0:
        print "Successful: "
        for success in successes:
            print "  * %s" % success
    if len(failures) > 0:
        print "Failed: "
        for failure in failures:
            print "  * %s" % failure
    if len(skips) > 0:
        print "Skipped: "
        for skip in skips:
            print "  * %s" % skip

