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

from os.path import splitext, basename, join
from tools.utils import mkdir
from tools.export.gnuarmeclipse import GNUARMEclipse
from tools.export.gnuarmeclipse import UID
from tools.build_api import prepare_toolchain
from sys import flags, platform

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
        'NUCLEO_L486RG':
        {
            'name': 'NUCLEO-L486RG',
            'mcuId': 'STM32L486RGTx'
        },
        'NUCLEO_L496ZG':
        {
            'name': 'NUCLEO-L496ZG',
            'mcuId': 'STM32L496ZGTx'
        },        
    }

    TARGETS = BOARDS.keys()

    def __gen_dir(self, dir_name):
        """
        Method that creates directory
        """
        settings = join(self.export_dir, dir_name)
        mkdir(settings)

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
        opts['c']['slowflashdata'] = False
        if '-mslow-flash-data' in flags_in['c_flags']:
            opts['c']['slowflashdata'] = True
        opts['cpp']['slowflashdata'] = False
        if '-mslow-flash-data' in flags_in['cxx_flags']:
            opts['cpp']['slowflashdata'] = True
        if opts['common']['optimization.messagelength']:
            opts['common']['optimization.other'] += ' -fmessage-length=0'
        if opts['common']['optimization.signedchar']:
            opts['common']['optimization.other'] += ' -fsigned-char'
        if opts['common']['optimization.nocommon']:
            opts['common']['optimization.other'] += ' -fno-common'
        if opts['common']['optimization.noinlinefunctions']:
            opts['common']['optimization.other'] += ' -fno-inline-functions'
        if opts['common']['optimization.freestanding']:
            opts['common']['optimization.other'] += ' -ffreestanding'
        if opts['common']['optimization.nobuiltin']:
            opts['common']['optimization.other'] += ' -fno-builtin'
        if opts['common']['optimization.spconstant']:
            opts['common']['optimization.other'] += ' -fsingle-precision-constant'
        if opts['common']['optimization.nomoveloopinvariants']:
            opts['common']['optimization.other'] += ' -fno-move-loop-invariants'
        if opts['common']['warnings.unused']:
            opts['common']['warnings.other'] += ' -Wunused'
        if opts['common']['warnings.uninitialized']:
            opts['common']['warnings.other'] += ' -Wuninitialized'
        if opts['common']['warnings.missingdeclaration']:
            opts['common']['warnings.other'] += ' -Wmissing-declarations'
        if opts['common']['warnings.pointerarith']:
            opts['common']['warnings.other'] += ' -Wpointer-arith'
        if opts['common']['warnings.padded']:
            opts['common']['warnings.other'] += ' -Wpadded'
        if opts['common']['warnings.shadow']:
            opts['common']['warnings.other'] += ' -Wshadow'
        if opts['common']['warnings.logicalop']:
            opts['common']['warnings.other'] += ' -Wlogical-op'
        if opts['common']['warnings.agreggatereturn']:
            opts['common']['warnings.other'] += ' -Waggregate-return'
        if opts['common']['warnings.floatequal']:
            opts['common']['warnings.other'] += ' -Wfloat-equal'
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
        # Strange System Workbench feature: If first parameter in Other flags is a
        # define (-D...), Other flags will be replaced by defines and other flags
        # are completely ignored. Moving -D parameters to defines.
        for compiler in ['c', 'cpp', 'as']:
            tmpList = opts[compiler]['other'].split(' ')
            otherList = []
            for item in tmpList:
                if item.startswith('-D'):
                    opts[compiler]['defines'].append(str(item[2:]))
                else:
                    otherList.append(item)
            opts[compiler]['other'] = ' '.join(otherList)
        # Assembler options
        for as_def in opts['as']['defines']:
            if '=' in as_def:
                opts['as']['other'] += ' --defsym ' + as_def
            else:
                opts['as']['other'] += ' --defsym ' + as_def + '=1'

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
        self.as_defines = [s.replace('"', '&quot;')
                           for s in self.toolchain.get_symbols(True)]
        self.c_defines = [s.replace('"', '&quot;')
                          for s in self.toolchain.get_symbols()]
        self.cpp_defines = self.c_defines
        print 'Symbols: {0}'.format(len(self.c_defines))

        self.include_path = []
        for s in self.resources.inc_dirs:
            self.include_path.append("../" + self.filter_dot(s))
        print ('Include folders: {0}'.format(len(self.include_path)))

        self.compute_exclusions()

        print ('Exclude folders: {0}'.format(len(self.excluded_folders)))

        ld_script = self.filter_dot(self.resources.linker_script)
        print ('Linker script:   {0}'.format(ld_script))

        lib_dirs = [self.filter_dot(s) for s in self.resources.lib_dirs]

        preproc_cmd = basename(self.toolchain.preproc[0]) + " " + " ".join(self.toolchain.preproc[1:])

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

            opts['c']['defines'] = self.c_defines
            opts['cpp']['defines'] = self.cpp_defines
            opts['as']['defines'] = self.as_defines

            self.process_sw_options(opts, flags)

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
            'platform': platform,
            'include_paths': self.include_path,
            'config_header': config_header,
            'exclude_paths': '|'.join(self.excluded_folders),
            'ld_script': ld_script,
            'library_paths': lib_dirs,
            'object_files': self.resources.objects,
            'libraries': libraries,
            'board_name': self.BOARDS[self.target.upper()]['name'],
            'mcu_name': self.BOARDS[self.target.upper()]['mcuId'],
            'cpp_cmd': preproc_cmd,
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
        self.gen_file('sw4stm32/launch.tmpl', ctx, self.project_name +
                      ' ' + options['debug']['name'] + '.launch')
