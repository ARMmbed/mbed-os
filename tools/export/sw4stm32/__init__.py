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

import copy

from os import walk, sep
from os.path import splitext, basename, join, dirname, relpath
from random import randint
from tools.utils import mkdir
from tools.export.exporters import Exporter


class Sw4STM32(Exporter):
    """
    Sw4STM32 class
    """
    NAME = 'Sw4STM32'
    TOOLCHAIN = 'GCC_ARM'

    BOARDS = {
        'B96B_F446VE':
        {
            'name': 'B96B-F446VE',
            'mcuId': 'STM32F446VETx'
        },
        'DISCO_F051R8':
        {
            'name': 'STM32F0DISCOVERY',
            'mcuId': 'STM32F051R8Tx'
        },
        'DISCO_F303VC':
        {
            'name': 'STM32F3DISCOVERY',
            'mcuId': 'STM32F303VCTx'
        },
        'DISCO_F334C8':
        {
            'name': 'STM32F3348DISCOVERY',
            'mcuId': 'STM32F334C8Tx'
        },
        'DISCO_F401VC':
        {
            'name': 'STM32F401C-DISCO',
            'mcuId': 'STM32F401VCTx'
        },
        'DISCO_F407VG':
        {
            'name': 'STM32F4DISCOVERY',
            'mcuId': 'STM32F407VGTx'
        },
        'DISCO_F413ZH':
        {
            'name': 'DISCO_F413',
            'mcuId': 'STM32F413ZHTx'
        },
        'DISCO_F429ZI':
        {
            'name': 'STM32F429I-DISCO',
            'mcuId': 'STM32F429ZITx'
        },
        'DISCO_F469NI':
        {
            'name': 'DISCO-F469NI',
            'mcuId': 'STM32F469NIHx'
        },
        'DISCO_F746NG':
        {
            'name': 'STM32F746G-DISCO',
            'mcuId': 'STM32F746NGHx'
        },
        'DISCO_F769NI':
        {
            'name': 'DISCO-F769NI',
            'mcuId': 'STM32F769NIHx'
        },
        'DISCO_L053C8':
        {
            'name': 'STM32L0538DISCOVERY',
            'mcuId': 'STM32L053C8Tx'
        },
        'DISCO_L072CZ_LRWAN1':
        {
            'name': 'DISCO-L072CZ-LRWAN1',
            'mcuId': 'STM32L072CZTx'
        },
        'DISCO_L475VG_IOT01A':
        {
            'name': 'STM32L475G-DISCO',
            'mcuId': 'STM32L475VGTx'
        },
        'DISCO_L476VG':
        {
            'name': 'STM32L476G-DISCO',
            'mcuId': 'STM32L476VGTx'
        },
        'NUCLEO_F030R8':
        {
            'name': 'NUCLEO-F030R8',
            'mcuId': 'STM32F030R8Tx'
        },
        'NUCLEO_F031K6':
        {
            'name': 'NUCLEO-F031K6',
            'mcuId': 'STM32F031K6Tx'
        },
        'NUCLEO_F042K6':
        {
            'name': 'NUCLEO-F042K6',
            'mcuId': 'STM32F042K6Tx'
        },
        'NUCLEO_F070RB':
        {
            'name': 'NUCLEO-F070RB',
            'mcuId': 'STM32F070RBTx'
        },
        'NUCLEO_F072RB':
        {
            'name': 'NUCLEO-F072RB',
            'mcuId': 'STM32F072RBTx'
        },
        'NUCLEO_F091RC':
        {
            'name': 'NUCLEO-F091RC',
            'mcuId': 'STM32F091RCTx'
        },
        'NUCLEO_F103RB':
        {
            'name': 'NUCLEO-F103RB',
            'mcuId': 'STM32F103RBTx'
        },
        'NUCLEO_F207ZG':
        {
            'name': 'NUCLEO-F207ZG',
            'mcuId': 'STM32F207ZGTx'
        },
        'NUCLEO_F302R8':
        {
            'name': 'NUCLEO-F302R8',
            'mcuId': 'STM32F302R8Tx'
        },
        'NUCLEO_F303K8':
        {
            'name': 'NUCLEO-F303K8',
            'mcuId': 'STM32F303K8Tx'
        },
        'NUCLEO_F303RE':
        {
            'name': 'NUCLEO-F303RE',
            'mcuId': 'STM32F303RETx'
        },
        'NUCLEO_F303ZE':
        {
            'name': 'NUCLEO-F303ZE',
            'mcuId': 'STM32F303ZETx'
        },
        'NUCLEO_F334R8':
        {
            'name': 'NUCLEO-F334R8',
            'mcuId': 'STM32F334R8Tx'
        },
        'NUCLEO_F401RE':
        {
            'name': 'NUCLEO-F401RE',
            'mcuId': 'STM32F401RETx'
        },
        'NUCLEO_F410RB':
        {
            'name': 'NUCLEO-F410RB',
            'mcuId': 'STM32F410RBTx'
        },
        'NUCLEO_F411RE':
        {
            'name': 'NUCLEO-F411RE',
            'mcuId': 'STM32F411RETx'
        },
        'NUCLEO_F429ZI':
        {
            'name': 'NUCLEO-F429ZI',
            'mcuId': 'STM32F429ZITx'
        },
        'NUCLEO_F446RE':
        {
            'name': 'NUCLEO-F446RE',
            'mcuId': 'STM32F446RETx'
        },
        'NUCLEO_F446ZE':
        {
            'name': 'NUCLEO-F446ZE',
            'mcuId': 'STM32F446ZETx'
        },
        'NUCLEO_F746ZG':
        {
            'name': 'NUCLEO-F746ZG',
            'mcuId': 'STM32F746ZGTx'
        },
        'NUCLEO_F767ZI':
        {
            'name': 'NUCLEO-F767ZI',
            'mcuId': 'STM32F767ZITx'
        },
        'NUCLEO_L011K4':
        {
            'name': 'NUCLEO-L011K4',
            'mcuId': 'STM32L011K4Tx'
        },
        'NUCLEO_L031K6':
        {
            'name': 'NUCLEO-L031K6',
            'mcuId': 'STM32L031K6Tx'
        },
        'NUCLEO_L053R8':
        {
            'name': 'NUCLEO-L053R8',
            'mcuId': 'STM32L053R8Tx'
        },
        'NUCLEO_L073RZ':
        {
            'name': 'NUCLEO-L073RZ',
            'mcuId': 'STM32L073RZTx'
        },
        'NUCLEO_L152RE':
        {
            'name': 'NUCLEO-L152RE',
            'mcuId': 'STM32L152RETx'
        },
        'NUCLEO_L432KC':
        {
            'name': 'NUCLEO-L432KC',
            'mcuId': 'STM32L432KCUx'
        },
        'NUCLEO_L476RG':
        {
            'name': 'NUCLEO-L476RG',
            'mcuId': 'STM32L476RGTx'
        },
    }

    TARGETS = BOARDS.keys()

    def __gen_dir(self, dir_name):
        """
        Method that creates directory
        """
        settings = join(self.export_dir, dir_name)
        mkdir(settings)

    def __generate_uid(self):
        """
        Method to create random int
        """
        return "%0.9u" % randint(0, 999999999)

    @staticmethod
    def filter_dot(path):
        """
        This function removes ./ from str.
        str must be converted with win_to_unix() before using this function.
        """
        if not path:
            return path
        if path.startswith('./'):
            return path[2:]
        return path

    def build_excludelist(self):
        """
        This method creates list for excluded directories.
        """
        self.source_folders = [self.filter_dot(s)
                               for s in set(dirname(src) for src in
                                            self.resources.c_sources +
                                            self.resources.cpp_sources +
                                            self.resources.s_sources)]
        if '.' in self.source_folders:
            self.source_folders.remove('.')

        top_folders = [f for f in set(s.split('/')[0]
                                      for s in self.source_folders)]

        for top_folder in top_folders:
            for root, dirs, files in walk(top_folder, topdown=True):
                # Paths returned by os.walk() must be split with os.dep
                # to accomodate Windows weirdness.
                parts = root.split(sep)
                self.remove_unused('/'.join(parts))

    def remove_unused(self, path):
        """
        Method for checking if path is needed.
        Method adds path to excluded list if not needed
        and is not subdirectory of already excluded directory
        """
        found = path in self.include_path
        needtoadd = True
        if not found:
            for directory in self.exclude_dirs:
                # Do not exclude subfolders from excluded folder
                if directory + '/' in path:
                    needtoadd = False
            if needtoadd:
                self.exclude_dirs.append(path)

    def generate(self):
        """
        Generate the .project and .cproject files.
        """
        opts = {}

        if not self.resources.linker_script:
            raise NotSupportedException("No linker script found.")

        print
        print 'Create a System Workbench for STM32 managed project'
        print 'Project name: {0}'.format(self.project_name)
        print 'Target: {0}'.format(self.toolchain.target.name)
        print 'Toolchain: {0}'.format(self.TOOLCHAIN)
        print

        self.resources.win_to_unix()

        fp_hardware = "no"
        fp_abi = "soft"
        core = self.toolchain.target.core
        if core == "Cortex-M4F" or core == "Cortex-M7F":
            fp_hardware = "fpv4-sp-d16"
            fp_abi = "softfp"
        elif core == "Cortex-M7FD":
            fp_hardware = "fpv5-d16"
            fp_abi = "softfp"
        print 'FP hardware: ' + fp_hardware

        config_header = self.filter_dot(self.toolchain.get_config_header())

        self.resources.win_to_unix()

        libraries = []
        for lib in self.resources.libraries:
            library, _ = splitext(basename(lib))
            libraries.append(library[3:])

        self.include_path = [self.filter_dot(s)
                             for s in self.resources.inc_dirs]
        print 'Include folders: %d' % len(self.include_path)

        self.exclude_dirs = []
        self.build_excludelist()

        print 'Exclude folders: %d' % len(self.exclude_dirs)

        self.exclude_dirs = '|'.join(self.exclude_dirs)

        ld_script = self.filter_dot(self.resources.linker_script)
        print ('Linker script: {0}'.format(ld_script))

        lib_dirs = [self.filter_dot(s) for s in self.resources.lib_dirs]

        symbols = [s.replace('"', '&quot;')
                   for s in self.toolchain.get_symbols()]
        # TODO: We need to fetch all flags from CDT
        opts['ld'] = {}
        opts['ld']['extra_flags'] = ''

        ctx = {
            'name': self.project_name,
            'include_paths': self.include_path,
            'config_header': config_header,
            'exclude_paths': self.exclude_dirs,
            'ld_script': ld_script,
            'linker_script': 'linker-script-' + self.project_name + '.ld',
            'library_paths': lib_dirs,
            'object_files': self.resources.objects,
            'libraries': libraries,
            'symbols': symbols,
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
            'floating_point_abi': fp_abi,
            'cpp_cmd': " ".join(self.toolchain.preproc),
            'options': opts
        }

        self.__gen_dir('.settings')
        self.gen_file('sw4stm32/language_settings_commom.tmpl',
                      ctx, '.settings/language.settings.xml')
        self.gen_file('sw4stm32/project_common.tmpl', ctx, '.project')
        self.gen_file('sw4stm32/cproject_common.tmpl', ctx, '.cproject')
        self.gen_file('sw4stm32/makefile.targets.tmpl', ctx,
                      'makefile.targets', trim_blocks=True, lstrip_blocks=True)
