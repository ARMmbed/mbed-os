#!/usr/bin/env python
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

from tools.paths import *
from tools.utils import mkdir, cmd
from tools.export import export, setup_user_prj


USR_PRJ_NAME = "usr_prj"
USER_PRJ = join(EXPORT_WORKSPACE, USR_PRJ_NAME)
USER_SRC = join(USER_PRJ, "src")


def setup_test_user_prj():
    if exists(USER_PRJ):
        print 'Test user project already generated...'
        return

    setup_user_prj(USER_PRJ, join(TEST_DIR, "rtos", "mbed", "basic"), [join(ROOT, "rtos"), join(LIB_DIR, "tests", "mbed", "env")])

    # FAKE BUILD URL
    open(join(USER_SRC, "mbed.bld"), 'w').write("http://mbed.org/users/mbed_official/code/mbed/builds/976df7c37ad5\n")


def fake_build_url_resolver(url):
    # FAKE BUILD URL: Ignore the URL, always return the path to the mbed library
    return {'path':MBED_LIBRARIES, 'name':'mbed'}


def test_export(toolchain, target, expected_error=None):
    if toolchain is None and target is None:
        base_dir = join(EXPORT_TMP, "zip")
    else:
        base_dir = join(EXPORT_TMP, toolchain, target)
    temp_dir = join(base_dir, "temp")
    mkdir(temp_dir)

    zip_path, report = export(USER_PRJ, USR_PRJ_NAME, toolchain, target, base_dir, temp_dir, False, None, fake_build_url_resolver)

    if report['success']:
        move(zip_path, join(EXPORT_DIR, "export_%s_%s.zip" % (toolchain, target)))
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
    setup_test_user_prj()

    for toolchain, target in [
            ('zip', 'LPC1768'),

            ('emblocks', 'LPC1768'),
            ('emblocks', 'LPC1549'),
            ('emblocks', 'LPC1114'),
            ('emblocks', 'LPC11U35_401'),
            ('emblocks', 'LPC11U35_501'),
            ('emblocks', 'LPCCAPPUCCINO'),
            ('emblocks', 'LPC2368'),
            ('emblocks', 'STM32F407'),
            ('emblocks', 'DISCO_F100RB'),
            ('emblocks', 'DISCO_F051R8'),
            ('emblocks', 'DISCO_F407VG'),
            ('emblocks', 'DISCO_F303VC'),
            ('emblocks', 'NRF51822'),
            ('emblocks', 'NUCLEO_F401RE'),
            ('emblocks', 'NUCLEO_F410RB'),
            ('emblocks', 'NUCLEO_F411RE'),
            ('emblocks', 'MTS_MDOT_F405RG'),
            ('emblocks', 'MTS_MDOT_F411RE'),

            ('coide', 'KL05Z'),
            ('coide', 'KL25Z'),
            ('coide', 'LPC1768'),
            ('coide', 'ARCH_PRO'),
            ('coide', 'DISCO_F407VG'),
            ('coide', 'NUCLEO_F401RE'),
            ('coide', 'NUCLEO_F410RB'),
            ('coide', 'NUCLEO_F411RE'),
            ('coide', 'DISCO_F429ZI'),
            ('coide', 'NUCLEO_F429ZI'),
            #('coide', 'DISCO_F469NI'), removed because template not available
            ('coide', 'NUCLEO_F334R8'),
            ('coide', 'NUCLEO_F303ZE'),
            ('coide', 'MTS_MDOT_F405RG'),
            ('coide', 'MTS_MDOT_F411RE'),

            ('uvision', 'LPC1768'),
            ('uvision', 'LPC11U24'),
            ('uvision', 'LPC11U35_401'),
            ('uvision', 'LPC11U35_501'),
            ('uvision', 'KL25Z'),
            ('uvision', 'LPC1347'),
            ('uvision', 'LPC1114'),
            ('uvision', 'LPC4088'),
            ('uvision', 'LPC4088_DM'),
            ('uvision', 'LPC4337'),
            ('uvision', 'LPC824'),
            ('uvision', 'SSCI824'),
            ('uvision', 'HRM1017'),

            ('uvision', 'B96B_F446VE'),
            ('uvision', 'NUCLEO_F030R8'),
            ('uvision', 'NUCLEO_F031K6'),
            ('uvision', 'NUCLEO_F042K6'),
            ('uvision', 'NUCLEO_F070RB'),
            ('uvision', 'NUCLEO_F072RB'),
            ('uvision', 'NUCLEO_F091RC'),
            ('uvision', 'NUCLEO_F103RB'),
            ('uvision', 'NUCLEO_F302R8'),
            ('uvision', 'NUCLEO_F303K8'),
            ('uvision', 'NUCLEO_F303RE'),
            ('uvision', 'NUCLEO_F334R8'),
            ('uvision', 'NUCLEO_F303ZE'),
            ('uvision', 'NUCLEO_F401RE'),
            ('uvision', 'NUCLEO_F410RB'),
            ('uvision', 'NUCLEO_F411RE'),
            ('uvision', 'NUCLEO_F412ZG'),
            ('uvision', 'NUCLEO_F429ZI'),
            ('uvision', 'NUCLEO_F446RE'),
            ('uvision', 'NUCLEO_F446ZE'),
            ('uvision', 'NUCLEO_L011K4'),
            ('uvision', 'NUCLEO_L031K6'),
            ('uvision', 'NUCLEO_L053R8'),
            ('uvision', 'NUCLEO_L073RZ'),
            ('uvision', 'NUCLEO_L152RE'),
            ('uvision', 'NUCLEO_L432KC'),
            ('uvision', 'NUCLEO_L476RG'),
            ('uvision', 'MTS_MDOT_F405RG'),
            ('uvision', 'MAXWSNENV'),
            ('uvision', 'MAX32600MBED'),
            ('uvision', 'MAX32620HSP'),
            ('uvision', 'DISCO_F051R8'),
            ('uvision', 'DISCO_F103RB'),
            ('uvision', 'DISCO_F303VC'),
            ('uvision', 'DISCO_L053C8'),
            ('uvision', 'DISCO_F334C8'),
            ('uvision', 'DISCO_F407VG'),
            ('uvision', 'DISCO_F429ZI'),
            ('uvision', 'DISCO_F746NG'),
            ('uvision', 'DISCO_F469NI'),
            ('uvision', 'DISCO_L476VG'),
            ('uvision', 'MOTE_L152RC'),
            ('uvision', 'ARM_BEETLE_SOC'),

            ('lpcxpresso', 'LPC1768'),
            ('lpcxpresso', 'LPC4088'),
            ('lpcxpresso', 'LPC4088_DM'),
            ('lpcxpresso', 'LPC1114'),
            ('lpcxpresso', 'LPC11U35_401'),
            ('lpcxpresso', 'LPC11U35_501'),
            ('lpcxpresso', 'LPCCAPPUCCINO'),
            ('lpcxpresso', 'LPC1549'),
            ('lpcxpresso', 'LPC11U68'),

            # Linux path: /home/emimon01/bin/gcc-arm/bin/
            # Windows path: C:/arm-none-eabi-gcc-4_7/bin/
            ('gcc_arm', 'LPC1768'),
            ('gcc_arm', 'LPC4088_DM'),
            ('gcc_arm', 'LPC1549'),
            ('gcc_arm', 'LPC1114'),
            ('gcc_arm', 'LPC11U35_401'),
            ('gcc_arm', 'LPC11U35_501'),
            ('gcc_arm', 'LPCCAPPUCCINO'),
            ('gcc_arm', 'LPC2368'),
            ('gcc_arm', 'LPC2460'),
            ('gcc_arm', 'LPC824'),
            ('gcc_arm', 'SSCI824'),

            ('gcc_arm', 'B96B_F446VE'),
            ('gcc_arm', 'STM32F407'),
            ('gcc_arm', 'DISCO_F100RB'),
            ('gcc_arm', 'DISCO_F051R8'),
            ('gcc_arm', 'DISCO_F407VG'),
            ('gcc_arm', 'DISCO_F303VC'),
            ('gcc_arm', 'DISCO_L053C8'),
            ('gcc_arm', 'DISCO_F334C8'),
            ('gcc_arm', 'DISCO_L053C8'),
            ('gcc_arm', 'DISCO_F429ZI'),
            ('gcc_arm', 'DISCO_F746NG'),
            ('gcc_arm', 'NUCLEO_F031K6'),
            ('gcc_arm', 'NUCLEO_F042K6'),
            ('gcc_arm', 'NRF51822'),
            ('gcc_arm', 'RBLAB_BLENANO'),
            ('gcc_arm', 'HRM1017'),
            ('gcc_arm', 'NUCLEO_F401RE'),
            ('gcc_arm', 'NUCLEO_F410RB'),
            ('gcc_arm', 'NUCLEO_F411RE'),
            ('gcc_arm', 'NUCLEO_F429ZI'),
            ('gcc_arm', 'NUCLEO_F446RE'),
            ('gcc_arm', 'NUCLEO_F446ZE'),
            ('gcc_arm', 'NUCLEO_F303ZE'),
            ('gcc_arm', 'ELMO_F411RE'),
            ('gcc_arm', 'DISCO_F469NI'),
            ('gcc_arm', 'NUCLEO_F334R8'),
            ('gcc_arm', 'NUCLEO_L011K4'),
            ('gcc_arm', 'NUCLEO_L031K6'),
            ('gcc_arm', 'NUCLEO_L432KC'),
            ('gcc_arm', 'MAX32600MBED'),
            ('gcc_arm', 'MTS_MDOT_F405RG'),
            ('gcc_arm', 'MTS_MDOT_F411RE'),
            ('gcc_arm', 'RZ_A1H'),
            ('gcc_arm', 'MAXWSNENV'),
            ('gcc_arm', 'MAX32600MBED'),
            ('gcc_arm', 'MAX32620HSP'),
            ('gcc_arm', 'ARCH_BLE'),
            ('gcc_arm', 'ARCH_MAX'),
            ('gcc_arm', 'ARCH_PRO'),
            ('gcc_arm', 'DELTA_DFCM_NNN40'),
            ('gcc_arm', 'K20D50M'),
            ('gcc_arm', 'K22F'),
            ('gcc_arm', 'K64F'),
            ('gcc_arm', 'KL05Z'),
            ('gcc_arm', 'KL25Z'),
            ('gcc_arm', 'KL43Z'),
            ('gcc_arm', 'KL46Z'),
            ('gcc_arm', 'EFM32GG_STK3700'),
            ('gcc_arm', 'EFM32LG_STK3600'),
            ('gcc_arm', 'EFM32WG_STK3800'),
            ('gcc_arm', 'EFM32ZG_STK3200'),
            ('gcc_arm', 'EFM32HG_STK3400'),
            ('gcc_arm', 'EFM32PG_STK3401'),

            ('ds5_5', 'LPC1768'),
            ('ds5_5', 'LPC11U24'),
            ('ds5_5', 'RZ_A1H'),

            ('iar', 'LPC1768'),
            ('iar', 'LPC4088_DM'),
            ('iar', 'LPC1347'),

            ('iar', 'B96B_F446VE'),
            ('iar', 'NUCLEO_F030R8'),
            ('iar', 'NUCLEO_F031K6'),
            ('iar', 'NUCLEO_F042K6'),
            ('iar', 'NUCLEO_F070RB'),
            ('iar', 'NUCLEO_F072RB'),
            ('iar', 'NUCLEO_F091RC'),
            ('iar', 'NUCLEO_F302R8'),
            ('iar', 'NUCLEO_F303K8'),
            ('iar', 'NUCLEO_F303RE'),
            ('iar', 'NUCLEO_F334R8'),
            ('iar', 'NUCLEO_F303ZE'),
            ('iar', 'NUCLEO_F401RE'),
            ('iar', 'NUCLEO_F410RB'),
            ('iar', 'NUCLEO_F411RE'),
            ('iar', 'NUCLEO_F429ZI'),
            ('iar', 'NUCLEO_F446RE'),
            ('iar', 'NUCLEO_F446ZE'),
            ('iar', 'NUCLEO_L011K4'),
            ('iar', 'NUCLEO_L031K6'),
            ('iar', 'NUCLEO_L053R8'),
            ('iar', 'NUCLEO_L073RZ'),
            ('iar', 'NUCLEO_L152RE'),
            ('iar', 'NUCLEO_L432KC'),
            ('iar', 'NUCLEO_L476RG'),
            ('iar', 'DISCO_L053C8'),
            ('iar', 'DISCO_F334C8'),
            ('iar', 'DISCO_F429ZI'),
            ('iar', 'DISCO_F469NI'),
            ('iar', 'DISCO_F746NG'),
            ('iar', 'DISCO_L476VG'),
            ('iar', 'STM32F407'),
            ('iar', 'MTS_MDOT_F405RG'),
            ('iar', 'MTS_MDOT_F411RE'),
            ('iar', 'MAXWSNENV'),
            ('iar', 'MAX32600MBED'),
            ('iar', 'MAX32620HSP'),
            ('iar', 'MOTE_L152RC'),
            ('iar', 'RZ_A1H'),

            # ('sw4stm32', 'DISCO_F051R8'),
            # ('sw4stm32', 'DISCO_F100RB'),
            ('sw4stm32', 'DISCO_F303VC'),
            ('sw4stm32', 'DISCO_F334C8'),
            # ('sw4stm32', 'DISCO_F401VC'),
            ('sw4stm32', 'DISCO_F407VG'),
            ('sw4stm32', 'DISCO_F429ZI'),
            ('sw4stm32', 'DISCO_F469NI'),
            ('sw4stm32', 'DISCO_F746NG'),
            ('sw4stm32', 'DISCO_L053C8'),
            ('sw4stm32', 'DISCO_L476VG'),
            ('sw4stm32', 'NUCLEO_F030R8'),
            ('sw4stm32', 'NUCLEO_F031K6'),
            ('sw4stm32', 'NUCLEO_F042K6'),
            ('sw4stm32', 'NUCLEO_F070RB'),
            ('sw4stm32', 'NUCLEO_F072RB'),
            ('sw4stm32', 'NUCLEO_F091RC'),
            ('sw4stm32', 'NUCLEO_F103RB'),
            ('sw4stm32', 'NUCLEO_F302R8'),
            ('sw4stm32', 'NUCLEO_F303K8'),
            ('sw4stm32', 'NUCLEO_F303RE'),
            ('sw4stm32', 'NUCLEO_F334R8'),
            ('sw4stm32', 'NUCLEO_F401RE'),
            ('sw4stm32', 'NUCLEO_F410RB'),
            ('sw4stm32', 'NUCLEO_F411RE'),
            ('sw4stm32', 'NUCLEO_F429ZI'),
            ('sw4stm32', 'NUCLEO_F446RE'),
            ('sw4stm32', 'NUCLEO_F446ZE'),
            ('sw4stm32', 'NUCLEO_L011K4'),
            ('sw4stm32', 'NUCLEO_L053R8'),
            ('sw4stm32', 'NUCLEO_L073RZ'),
            ('sw4stm32', 'NUCLEO_L152RE'),
            ('sw4stm32', 'NUCLEO_L432KC'),
            ('sw4stm32', 'NUCLEO_L476RG'),
            ('sw4stm32', 'NUCLEO_F031K6'),
            ('sw4stm32', 'NUCLEO_F042K6'),
            ('sw4stm32', 'NUCLEO_F303ZE'),
            ('sw4stm32', 'NUCLEO_F410RB'),

            ('e2studio', 'RZ_A1H'),
            # Removed following item to avoid script error
            #(None, None),
        ]:
        print '\n=== Exporting to "%s::%s" ===' % (toolchain, target)
        test_export(toolchain, target)

    print "\n=== Test error messages ==="
    test_export('lpcxpresso', 'LPC11U24', expected_error='lpcxpresso')
