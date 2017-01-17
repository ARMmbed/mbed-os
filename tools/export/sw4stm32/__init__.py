"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited

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
from os.path import splitext, basename, join
from random import randint
from tools.utils import mkdir
from tools.export.exporters import Exporter


class Sw4STM32(Exporter):
    NAME = 'Sw4STM32'
    TOOLCHAIN = 'GCC_ARM'

    BOARDS = {
        'B96B_F446VE':      {'name': 'B96B-F446VE',           'mcuId': 'STM32F446VETx'},
        'DISCO_F051R8':     {'name': 'STM32F0DISCOVERY',      'mcuId': 'STM32F051R8Tx'},
        'DISCO_F303VC':     {'name': 'STM32F3DISCOVERY',      'mcuId': 'STM32F303VCTx'},
        'DISCO_F334C8':     {'name': 'STM32F3348DISCOVERY',   'mcuId': 'STM32F334C8Tx'},
        'DISCO_F401VC':     {'name': 'STM32F401C-DISCO',      'mcuId': 'STM32F401VCTx'},
        'DISCO_F407VG':     {'name': 'STM32F4DISCOVERY',      'mcuId': 'STM32F407VGTx'},
        'DISCO_F429ZI':     {'name': 'STM32F429I-DISCO',      'mcuId': 'STM32F429ZITx'},
        'DISCO_F469NI':     {'name': 'DISCO-F469NI',          'mcuId': 'STM32F469NIHx'},
        'DISCO_F746NG':     {'name': 'STM32F746G-DISCO',      'mcuId': 'STM32F746NGHx'},
        'DISCO_F769NI':     {'name': 'DISCO-F769NI',          'mcuId': 'STM32F769NIHx'},
        'DISCO_L053C8':     {'name': 'STM32L0538DISCOVERY',   'mcuId': 'STM32L053C8Tx'},
        'DISCO_L476VG':     {'name': 'STM32L476G-DISCO',      'mcuId': 'STM32L476VGTx'},
        'NUCLEO_F030R8':    {'name': 'NUCLEO-F030R8',         'mcuId': 'STM32F030R8Tx'},
        'NUCLEO_F031K6':    {'name': 'NUCLEO-F031K6',         'mcuId': 'STM32F031K6Tx'},
        'NUCLEO_F042K6':    {'name': 'NUCLEO-F042K6',         'mcuId': 'STM32F042K6Tx'},
        'NUCLEO_F070RB':    {'name': 'NUCLEO-F070RB',         'mcuId': 'STM32F070RBTx'},
        'NUCLEO_F072RB':    {'name': 'NUCLEO-F072RB',         'mcuId': 'STM32F072RBTx'},
        'NUCLEO_F091RC':    {'name': 'NUCLEO-F091RC',         'mcuId': 'STM32F091RCTx'},
        'NUCLEO_F103RB':    {'name': 'NUCLEO-F103RB',         'mcuId': 'STM32F103RBTx'},
        'NUCLEO_F207ZG':    {'name': 'NUCLEO-F207ZG',         'mcuId': 'STM32F207ZGTx'},
        'NUCLEO_F302R8':    {'name': 'NUCLEO-F302R8',         'mcuId': 'STM32F302R8Tx'},
        'NUCLEO_F303K8':    {'name': 'NUCLEO-F303K8',         'mcuId': 'STM32F303K8Tx'},
        'NUCLEO_F303RE':    {'name': 'NUCLEO-F303RE',         'mcuId': 'STM32F303RETx'},
        'NUCLEO_F303ZE':    {'name': 'NUCLEO-F303ZE',         'mcuId': 'STM32F303ZETx'},
        'NUCLEO_F334R8':    {'name': 'NUCLEO-F334R8',         'mcuId': 'STM32F334R8Tx'},
        'NUCLEO_F401RE':    {'name': 'NUCLEO-F401RE',         'mcuId': 'STM32F401RETx'},
        'NUCLEO_F410RB':    {'name': 'NUCLEO-F410RB',         'mcuId': 'STM32F410RBTx'},
        'NUCLEO_F411RE':    {'name': 'NUCLEO-F411RE',         'mcuId': 'STM32F411RETx'},
        'NUCLEO_F429ZI':    {'name': 'NUCLEO-F429ZI',         'mcuId': 'STM32F429ZITx'},
        'NUCLEO_F446RE':    {'name': 'NUCLEO-F446RE',         'mcuId': 'STM32F446RETx'},
        'NUCLEO_F446ZE':    {'name': 'NUCLEO-F446ZE',         'mcuId': 'STM32F446ZETx'},
        'NUCLEO_F746ZG':    {'name': 'NUCLEO-F746ZG',         'mcuId': 'STM32F746ZGTx'},
        'NUCLEO_F767ZI':    {'name': 'NUCLEO-F767ZI',         'mcuId': 'STM32F767ZITx'},
        'NUCLEO_L011K4':    {'name': 'NUCLEO-L011K4',         'mcuId': 'STM32L011K4Tx'},
        'NUCLEO_L031K6':    {'name': 'NUCLEO-L031K6',         'mcuId': 'STM32L031K6Tx'},
        'NUCLEO_L053R8':    {'name': 'NUCLEO-L053R8',         'mcuId': 'STM32L053R8Tx'},
        'NUCLEO_L073RZ':    {'name': 'NUCLEO-L073RZ',         'mcuId': 'STM32L073RZTx'},
        'NUCLEO_L152RE':    {'name': 'NUCLEO-L152RE',         'mcuId': 'STM32L152RETx'},
        'NUCLEO_L432KC':    {'name': 'NUCLEO-L432KC',         'mcuId': 'STM32L432KCUx'},
        'NUCLEO_L476RG':    {'name': 'NUCLEO-L476RG',         'mcuId': 'STM32L476RGTx'},
    }

    TARGETS = BOARDS.keys()

    def __gen_dir(self, dirname):
        settings = join(self.export_dir, dirname)
        mkdir(settings)

    def __generate_uid(self):
        return "%0.9u" % randint(0, 999999999)

    def generate(self):
        fp_hardware = "no"
        fp_abi = "soft"
        core = self.toolchain.target.core
        if core == "Cortex-M4F" or core == "Cortex-M7F":
            fp_hardware = "fpv4-sp-d16"
            fp_abi = "soft-fp"
        elif core == "Cortex-M7FD":
            fp_hardware = "fpv5-d16"
            fp_abi = "soft-fp"
            
        libraries = []
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])

        ctx = {
            'name': self.project_name,
            'include_paths': self.resources.inc_dirs,
            'linker_script': self.resources.linker_script,
            'library_paths': self.resources.lib_dirs,
            'object_files': self.resources.objects,
            'libraries': libraries,
            'symbols': self.toolchain.get_symbols(),
            'board_name': self.BOARDS[self.target.upper()]['name'],
            'mcu_name': self.BOARDS[self.target.upper()]['mcuId'],
            'debug_config_uid': self.__generate_uid(),
            'debug_tool_compiler_uid': self.__generate_uid(),
            'debug_tool_compiler_input_uid': self.__generate_uid(),
            'release_config_uid': self.__generate_uid(),
            'release_tool_compiler_uid': self.__generate_uid(),
            'release_tool_compiler_input_uid': self.__generate_uid(),
            'uid': self.__generate_uid(),
            'floating_point_hardware': fp_hardware,
            'floating_point_abi': fp_abi
        }

        self.__gen_dir('.settings')
        self.gen_file('sw4stm32/language_settings_commom.tmpl', ctx, '.settings/language.settings.xml')
        self.gen_file('sw4stm32/project_common.tmpl', ctx, '.project')
        self.gen_file('sw4stm32/cproject_common.tmpl', ctx, '.cproject')
