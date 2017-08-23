"""
mbed SDK
Copyright (c) 2011-2017 ARM Limited

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
from tools.export.gnuarmeclipse import GNUARMEclipse
from tools.export.gnuarmeclipse import UID
from tools.build_api import prepare_toolchain
from sys import flags

# Global random number generator instance.
u = UID()


class Sw4STM32(GNUARMEclipse):
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

    def get_fpu_hardware(self, fpu_unit):
        """
        Convert fpu unit name into hardware name.
        """
        hw = ''
        fpus = {
            'fpv4spd16': 'fpv4-sp-d16',
            'fpv5d16': 'fpv5-d16',
            'fpv5spd16': 'fpv5-sp-d16'
        }
        if fpu_unit in fpus:
            hw = fpus[fpu_unit]
        return hw

    def process_sw_options(self, opts, flags_in):
        """
        Process System Workbench specific options.

        System Workbench for STM32 has some compile options, which are not recognized by the GNUARMEclipse exporter.
        Those are handled in this method.
        """
        opts['c']['preprocess'] = False
        if '-E' in flags_in['c_flags']:
            opts['c']['preprocess'] = True
        opts['cpp']['preprocess'] = False
        if '-E' in flags_in['cxx_flags']:
            opts['cpp']['preprocess'] = True
        opts['ld']['strip'] = False
        if '-s' in flags_in['ld_flags']:
            opts['ld']['strip'] = True
        opts['ld']['shared'] = False
        if '-shared' in flags_in['ld_flags']:
            opts['ld']['shared'] = True
        opts['ld']['soname'] = ''
        opts['ld']['implname'] = ''
        opts['ld']['defname'] = ''
        for item in flags_in['ld_flags']:
            if item.startswith('-Wl,-soname='):
                opts['ld']['soname'] = item[len('-Wl,-soname='):]
            if item.startswith('-Wl,--out-implib='):
                opts['ld']['implname'] = item[len('-Wl,--out-implib='):]
            if item.startswith('-Wl,--output-def='):
                opts['ld']['defname'] = item[len('-Wl,--output-def='):]
        opts['common']['arm.target.fpu.hardware'] = self.get_fpu_hardware(
            opts['common']['arm.target.fpu.unit'])
        opts['common']['debugging.codecov'] = False
        if '-fprofile-arcs' in flags_in['common_flags'] and '-ftest-coverage' in flags_in['common_flags']:
            opts['common']['debugging.codecov'] = True
        # Passing linker options to linker with '-Wl,'-prefix.
        for index in range(len(opts['ld']['flags'])):
            item = opts['ld']['flags'][index]
            if not item.startswith('-Wl,'):
                opts['ld']['flags'][index] = '-Wl,' + item

    def generate(self):
        """
        Generate the .project and .cproject files.
        """
        options = {}

        if not self.resources.linker_script:
            raise NotSupportedException("No linker script found.")

        print ('\nCreate a System Workbench for STM32 managed project')
        print ('Project name: {0}'.format(self.project_name))
        print ('Target:       {0}'.format(self.toolchain.target.name))
        print ('Toolchain:    {0}'.format(self.TOOLCHAIN) + '\n')

        self.resources.win_to_unix()

        config_header = self.filter_dot(self.toolchain.get_config_header())

        libraries = []
        for lib in self.resources.libraries:
            library, _ = splitext(basename(lib))
            libraries.append(library[3:])

        self.system_libraries = [
            'stdc++', 'supc++', 'm', 'c', 'gcc', 'nosys'
        ]

        profiles = self.get_all_profiles()

        self.as_defines = self.toolchain.get_symbols(True)
        self.c_defines = self.toolchain.get_symbols()
        self.cpp_defines = self.c_defines
        print 'Symbols: {0}'.format(len(self.c_defines))

        self.include_path = [self.filter_dot(s)
                             for s in self.resources.inc_dirs]
        print ('Include folders: {0}'.format(len(self.include_path)))

        self.exclude_dirs = []
        self.build_excludelist()

        print ('Exclude folders: {0}'.format(len(self.exclude_dirs)))

        self.exclude_dirs = '|'.join(self.exclude_dirs)

        ld_script = self.filter_dot(self.resources.linker_script)
        print ('Linker script:   {0}'.format(ld_script))

        lib_dirs = [self.filter_dot(s) for s in self.resources.lib_dirs]

        symbols = [s.replace('"', '&quot;')
                   for s in self.toolchain.get_symbols()]

        for id in ['debug', 'release']:
            opts = {}
            opts['common'] = {}
            opts['as'] = {}
            opts['c'] = {}
            opts['cpp'] = {}
            opts['ld'] = {}

            opts['id'] = id
            opts['name'] = opts['id'].capitalize()

            # TODO: Add prints to log or console in verbose mode.
            #print ('\nBuild configuration: {0}'.format(opts['name']))

            profile = profiles[id]

            # A small hack, do not bother with src_path again,
            # pass an empty string to avoid crashing.
            src_paths = ['']
            toolchain = prepare_toolchain(
                src_paths, "", self.toolchain.target.name, self.TOOLCHAIN, build_profile=[profile])

            # Hack to fill in build_dir
            toolchain.build_dir = self.toolchain.build_dir

            flags = self.toolchain_flags(toolchain)

            # TODO: Add prints to log or console in verbose mode.
            # print 'Common flags:', ' '.join(flags['common_flags'])
            # print 'C++ flags:', ' '.join(flags['cxx_flags'])
            # print 'C flags:', ' '.join(flags['c_flags'])
            # print 'ASM flags:', ' '.join(flags['asm_flags'])
            # print 'Linker flags:', ' '.join(flags['ld_flags'])

            # Most GNU ARM Eclipse options have a parent,
            # either debug or release.
            if '-O0' in flags['common_flags'] or '-Og' in flags['common_flags']:
                opts['parent_id'] = 'debug'
            else:
                opts['parent_id'] = 'release'

            self.process_options(opts, flags)

            self.process_sw_options(opts, flags)

            opts['as']['defines'] = self.as_defines
            opts['c']['defines'] = self.c_defines
            opts['cpp']['defines'] = self.cpp_defines

            opts['ld']['library_paths'] = [
                self.filter_dot(s) for s in self.resources.lib_dirs]

            opts['ld']['user_libraries'] = libraries
            opts['ld']['system_libraries'] = self.system_libraries
            opts['ld']['script'] = "linker-script-" + id + ".ld"

            # Unique IDs used in multiple places.
            uid = {}
            uid['config'] = u.id
            uid['tool_c_compiler'] = u.id
            uid['tool_c_compiler_input'] = u.id
            uid['tool_cpp_compiler'] = u.id
            uid['tool_cpp_compiler_input'] = u.id

            opts['uid'] = uid

            options[id] = opts

        ctx = {
            'name': self.project_name,
            'include_paths': self.include_path,
            'config_header': config_header,
            'exclude_paths': self.exclude_dirs,
            'ld_script': ld_script,
            'library_paths': lib_dirs,
            'object_files': self.resources.objects,
            'libraries': libraries,
            'symbols': symbols,
            'board_name': self.BOARDS[self.target.upper()]['name'],
            'mcu_name': self.BOARDS[self.target.upper()]['mcuId'],
            'cpp_cmd': " ".join(self.toolchain.preproc),
            'options': options,
            # id property of 'u' will generate new random identifier every time
            # when called.
            'u': u
        }

        self.__gen_dir('.settings')
        self.gen_file('sw4stm32/language_settings_commom.tmpl',
                      ctx, '.settings/language.settings.xml')
        self.gen_file('sw4stm32/project_common.tmpl', ctx, '.project')
        self.gen_file('sw4stm32/cproject_common.tmpl', ctx, '.cproject')
        self.gen_file('sw4stm32/makefile.targets.tmpl', ctx,
                      'makefile.targets', trim_blocks=True, lstrip_blocks=True)
