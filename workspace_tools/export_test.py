import sys
from os.path import join, abspath, dirname, exists
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.append(ROOT)

from workspace_tools.paths import *
from workspace_tools.utils import mkdir, cmd, copy_file
from workspace_tools.export import export

from shutil import copytree


USR_PRJ_NAME = "export_test"
USER_PRJ = join(BUILD_DIR, USR_PRJ_NAME)
USER_LIB = join(USER_PRJ, "lib")
USER_SRC = join(USER_PRJ, "src")
TEMP     = join(USER_PRJ, ".temp")

def setup_test_user_prj():
    if exists(USER_PRJ):
        print 'Test user project already generated...'
        return
    
    # Build project directory structure
    for d in [USER_LIB, USER_SRC]:
        mkdir(d)
    
    # Sources
    print 'Coping sources...'
    copy_file(join(TEST_DIR, "rtos", "mbed", "basic", "main.cpp"), join(USER_SRC, "main.cpp"))
    copytree(join(LIB_DIR, "rtos"), join(USER_LIB, "rtos"))
    
    # FAKE BUILD URL
    open(join(USER_SRC, "mbed.bld"), 'w').write("http://mbed.org/users/mbed_official/code/mbed/builds/976df7c37ad5\n")


def fake_build_url_resolver(url):
    # FAKE BUILD URL: Ignore the URL, always return the path to the mbed library
    return {'path':MBED_LIBRARIES, 'name':'mbed'}


def test_export(toolchain, target, expected_error=None):
    if toolchain is None and target is None:
        base_dir = join(TEMP, "zip")
    else:
        base_dir = join(TEMP, toolchain, target)
    temp_dir = join(base_dir, "temp")
    mkdir(temp_dir)
    
    zip_path, report = export(USER_PRJ, USR_PRJ_NAME, toolchain, target, base_dir, temp_dir, False, fake_build_url_resolver)
    
    if report['success']:
        export_name = join(TEMP, "export_%s_%s.zip" % (toolchain, target))
        cmd(["mv", zip_path, export_name])
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
    # import logging as log
    # log.getLogger().setLevel(log.DEBUG)
    
    setup_test_user_prj()
    
    for toolchain, target in [
            ('uvision', 'LPC1768'), ('uvision', 'LPC11U24'), ('uvision', 'KL25Z'),
            
            ('codered', 'LPC1768'),
            
            # Linux path: /home/emimon01/bin/gcc-cs/bin/
            # Windows path: "C:/Program Files (x86)/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI/bin/"
            ('codesourcery', 'LPC1768'),
            
            # Linux path: /home/emimon01/bin/gcc-arm/bin/
            # Windows path: C:/arm-none-eabi-gcc-4_7/bin/
            ('gcc_arm', 'LPC1768'),
            
            ('ds5_5', 'LPC1768'), ('ds5_5', 'LPC11U24'),
            
            ('iar', 'LPC1768'),
            
            (None, None)
        ]:
        print '\n=== Exporting to "%s::%s" ===' % (toolchain, target)
        test_export(toolchain, target)
    
    print "\n=== Test error messages ==="
    test_export('codered', 'LPC11U24', expected_error='codered')
