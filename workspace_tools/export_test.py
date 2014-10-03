"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
import sys
from os.path import join, abspath, dirname, exists
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from shutil import move

from workspace_tools.paths import *
from workspace_tools.utils import mkdir, cmd
from workspace_tools.export import export, setup_user_prj
from optparse import OptionParser


# List of project which can be exported
USER_PROJECTS = {
    'RTOS_MBED_BASIC' : (join(TEST_DIR, "rtos", "mbed", "basic"), [join(LIB_DIR, "rtos")]),
    'MBED_BLINKY' : (join(TEST_DIR, "mbed", "blinky"), [])
}

# Tuple of toolchain and target with possible export projects
EXPORTS = (
    ('coide', 'KL05Z', ['RTOS_MBED_BASIC']),
    ('coide', 'KL25Z', ['RTOS_MBED_BASIC']),
    ('coide', 'LPC1768', ['RTOS_MBED_BASIC']),
    ('coide', 'ARCH_PRO', ['RTOS_MBED_BASIC']),
    ('coide', 'DISCO_F407VG', ['RTOS_MBED_BASIC', 'MBED_BLINKY']),
    ('coide', 'NUCLEO_F401RE', ['RTOS_MBED_BASIC']),
    ('coide', 'NUCLEO_F411RE', ['RTOS_MBED_BASIC']),
    # ('coide', 'NUCLEO_F334R8', ['MBED_BLINKY']),

    ('uvision', 'LPC1768', ['RTOS_MBED_BASIC']),
    ('uvision', 'LPC11U24', ['RTOS_MBED_BASIC']),
    ('uvision', 'KL25Z', ['RTOS_MBED_BASIC']),
    ('uvision', 'LPC1347', ['RTOS_MBED_BASIC']),
    ('uvision', 'LPC1114', ['RTOS_MBED_BASIC']),
    ('uvision', 'LPC4088', ['RTOS_MBED_BASIC']),
    ('uvision', 'LPC4337', ['RTOS_MBED_BASIC']),

    ('uvision', 'NUCLEO_F030R8', ['RTOS_MBED_BASIC']),
    ('uvision', 'NUCLEO_F072RB', ['RTOS_MBED_BASIC']),
    ('uvision', 'NUCLEO_F103RB', ['RTOS_MBED_BASIC']),
    ('uvision', 'NUCLEO_F302R8', ['RTOS_MBED_BASIC']),
    ('uvision', 'NUCLEO_F334R8', ['RTOS_MBED_BASIC']),
    ('uvision', 'NUCLEO_F401RE', ['RTOS_MBED_BASIC']),
    ('uvision', 'NUCLEO_F411RE', ['RTOS_MBED_BASIC']),
    ('uvision', 'NUCLEO_L053R8', ['RTOS_MBED_BASIC']),
    ('uvision', 'NUCLEO_L152RE', ['RTOS_MBED_BASIC']),

    ('lpcxpresso', 'LPC1768', ['RTOS_MBED_BASIC']),
    ('lpcxpresso', 'LPC4088', ['RTOS_MBED_BASIC']),
    ('lpcxpresso', 'LPC1114', ['RTOS_MBED_BASIC']),
    ('lpcxpresso', 'LPC11U35_401', ['RTOS_MBED_BASIC']),
    ('lpcxpresso', 'LPC11U35_501', ['RTOS_MBED_BASIC']),
    ('lpcxpresso', 'LPCCAPPUCCINO', ['RTOS_MBED_BASIC']),
    ('lpcxpresso', 'LPC1549', ['RTOS_MBED_BASIC']),
    ('lpcxpresso', 'LPC11U68', ['RTOS_MBED_BASIC']),
    ('codesourcery', 'LPC1768', ['RTOS_MBED_BASIC']),

    ('gcc_arm', 'LPC1768', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'LPC1549', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'LPC1114', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'LPC11U35_401', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'LPC11U35_501', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'LPCCAPPUCCINO', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'LPC2368', ['RTOS_MBED_BASIC']),

    ('gcc_arm', 'STM32F407', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'DISCO_F100RB', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'DISCO_F051R8', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'DISCO_F407VG', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'DISCO_F303VC', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'NRF51822', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'NUCLEO_F401RE', ['RTOS_MBED_BASIC']),
    ('gcc_arm', 'NUCLEO_F411RE', ['RTOS_MBED_BASIC']),
    # ('gcc_arm', 'NUCLEO_F334R8', ['MBED_BLINKY']),


    ('ds5_5', 'LPC1768', ['RTOS_MBED_BASIC']), 
    ('ds5_5', 'LPC11U24', ['RTOS_MBED_BASIC']),

    ('iar', 'LPC1768', ['RTOS_MBED_BASIC'])
)

USR_PRJ_NAME = "usr_prj"


def setup_test_user_prj():
    for project_name in USER_PROJECTS:
        (source_dir, libraries) = USER_PROJECTS[project_name]
        user_project_name = USR_PRJ_NAME + '_' + project_name
        user_project = join(EXPORT_WORKSPACE, user_project_name)
        if exists(user_project):
            print 'Test user project ' + user_project_name + 'already generated...'
            continue

        setup_user_prj(user_project, source_dir, libraries)

        # FAKE BUILD URL
        open(join(join(user_project, "src"), "mbed.bld"), 'w').write("http://mbed.org/users/mbed_official/code/mbed/builds/976df7c37ad5\n")


def fake_build_url_resolver(url):
    # FAKE BUILD URL: Ignore the URL, always return the path to the mbed library
    return {'path':MBED_LIBRARIES, 'name':'mbed'}


def test_export(toolchain, target, project, expected_error=None):
    if toolchain is None and target is None and project is None:
        base_dir = join(EXPORT_TMP, "zip")
    else:
        base_dir = join(EXPORT_TMP, toolchain, target, project)
    temp_dir = join(base_dir, "temp")
    mkdir(temp_dir)

    (source_dir, libraries) = USER_PROJECTS[project]
    user_project_name = USR_PRJ_NAME + '_' + project
    user_project = join(EXPORT_WORKSPACE, user_project_name)
    zip_path, report = export(user_project, user_project_name, toolchain, target, base_dir, temp_dir, False, fake_build_url_resolver)

    if report['success']:
        move(zip_path, join(EXPORT_DIR, "export_%s_%s_%s.zip" % (toolchain, target, project)))
        print "[OK]"
    else:
        if expected_error is None:
            print '[ERRROR] %s' % report['errormsg']
        else:
            if (zip_path is None) and (expected_error in report['errormsg']):
                print '[OK]'
            else:
                print '[ERROR]'
                print '    zip:', zip_path
                print '    msg:', report['errormsg']


if __name__ == '__main__':
    toolchainlist = []
    targetlist = []
    projectlist = []
    for toolchain, target, projects in EXPORTS:
        if not toolchainlist.__contains__(toolchain):
            toolchainlist.append(toolchain)
        if not targetlist.__contains__(target):
            targetlist.append(target)
        for project in projects:
            if not projectlist.__contains__(project):
                projectlist.append(project)

    targetlist.sort()
    toolchainlist.sort()
    projectlist.sort()

    # Parse Options
    parser = OptionParser()

    # Default option: with this option or without any option all possible exports are generated
    parser.add_option("-a", "--all",
                  action="store_true",
                  default=False,
                  help="export all projects and MCU to all toolchains",
                  metavar="ALL")

    parser.add_option("-m", "--mcu",
                  help="export for the given MCU (%s)" % ', '.join(targetlist),
                  metavar="MCU")

    parser.add_option("-t", "--tool",
                  help="export to the given TOOLCHAIN (%s)" % ', '.join(toolchainlist),
                  metavar="TOOLCHAIN")

    parser.add_option("-p", "--project",
                  help="export the given PROJECT (%s)" % ', '.join(projectlist),
                  metavar="PROJECT")

    (options, args) = parser.parse_args()

    # Get target list
    if options.mcu:
        mcu_list = (options.mcu).split(",")
        for mcu in mcu_list:
            if mcu not in targetlist:
                print "Given MCU '%s' not into the supported list:\n%s" % (mcu, targetlist)
                sys.exit(1)
        targets = mcu_list
    else:
        targets = targetlist

    # Get toolchains list
    if options.tool:
        toolchain_list = (options.tool).split(",")
        for tc in toolchain_list:
            if tc not in toolchainlist:
                print "Given toolchain '%s' not into the supported list:\n%s" % (tc, toolchainlist)
                sys.exit(1)
        toolchains = toolchain_list
    else:
        toolchains = toolchainlist

    # Get projects list
    if options.project:
        project_list = (options.project).split(",")
        for prj in project_list:
            if prj not in projectlist:
                print "Given project '%s' not into the configured list:\n%s" % (prj, projectlist)
                sys.exit(1)
        projects = project_list
    else:
        projects = projectlist
        
    # Generates all possible exports
    if options.all:
        targets = targetlist
        toolchains = toolchainlist
        projects = projectlist

    setup_test_user_prj()

    for toolchain, target, project in EXPORTS:
        for prj in project:
            if toolchains.__contains__(toolchain) and targets.__contains__(target) and projects.__contains__(prj):
                print '\n=== Exporting to "%s::%s::%s" ===' % (toolchain, target, prj)
                test_export(toolchain, target, prj)

    # print "\n=== Test error messages ==="
    # test_export('lpcxpresso', 'LPC11U24', 'RTOS_MBED_BASIC', expected_error='lpcxpresso')
