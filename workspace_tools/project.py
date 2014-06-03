import sys
from os.path import join, abspath, dirname, exists
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from shutil import move
from optparse import OptionParser

from workspace_tools.paths import EXPORT_DIR, EXPORT_WORKSPACE, EXPORT_TMP
from workspace_tools.paths import MBED_BASE, MBED_LIBRARIES
from workspace_tools.export import export, setup_user_prj, EXPORTERS
from workspace_tools.utils import args_error
from workspace_tools.tests import TESTS, Test, TEST_MAP
from workspace_tools.targets import TARGET_NAMES


if __name__ == '__main__':
    # Parse Options
    parser = OptionParser()

    parser.add_option("-m", "--mcu", metavar="MCU", default='LPC1768',
        help="generate project for the given MCU (%s)" % ', '.join(TARGET_NAMES))

    parser.add_option("-p", type="int", dest="program",
        help="The index of the desired test program: [0-%d]" % (len(TESTS)-1))

    parser.add_option("-i", dest="ide", default='uvision',
        help="The target IDE: %s" % str(EXPORTERS.keys()))

    parser.add_option("-b", dest="build", action="store_true", default=False,
        help="Use the mbed library build, instead of the sources")

    (options, args) = parser.parse_args()

    # Target
    if options.mcu is None :
        args_error(parser, "[ERROR] You should specify an MCU")
    mcu = options.mcu

    # IDE
    if options.ide is None:
        args_error(parser, "[ERROR] You should specify an IDE")
    ide = options.ide

    # Project
    if options.program is None or (options.program < 0) or (options.program > (len(TESTS)-1)):
        message = "[ERROR] You have to specify one of the following tests:\n"
        message += '\n'.join(map(str, sorted(TEST_MAP.values())))
        args_error(parser, message)
    test = Test(options.program)

    if not options.build:
        # Substitute the library builds with the sources
        # TODO: Substitute also the other library build paths
        if MBED_LIBRARIES in test.dependencies:
            test.dependencies.remove(MBED_LIBRARIES)
            test.dependencies.append(MBED_BASE)

    # Build the projectwith the same directory structure of the mbed online IDE
    project_dir = join(EXPORT_WORKSPACE, test.id)
    setup_user_prj(project_dir, test.source_dir, test.dependencies)

    # Export to selected toolchain
    tmp_path, report = export(project_dir, test.id, ide, mcu, EXPORT_WORKSPACE, EXPORT_TMP)
    if report['success']:
        zip_path = join(EXPORT_DIR, "%s_%s_%s.zip" % (test.id, ide, mcu))
        move(tmp_path, zip_path)
        print "[OK]"
    else:
        print '[ERRROR] %s' % report['errormsg']
