"""
mbed SDK
Copyright (c) 2014-2016 ARM Limited

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
from os.path import split,splitext, basename
from os import remove

from tools.export.exporters import Exporter, deprecated_exporter

class Folder:
    def __init__(self, name):
        self.name = name
        self.children = []

    def contains(self, folderName):
        for child in self.children:
            if child.name == folderName:
                return True
        return False

    def __str__(self):
        retval = self.name + " "
        if len(self.children) > 0:
            retval += "[ "
            for child in self.children:
                retval += child.__str__()
            retval += " ]"

        return retval

    def findChild(self, folderName):
        for child in self.children:
            if child.name == folderName:
                return child
        return None

    def addChild(self, folderName):
        if folderName == '':
            return None

        if not self.contains(folderName):
            self.children.append(Folder(folderName))

        return self.findChild(folderName)

@deprecated_exporter
class SimplicityV3(Exporter):
    NAME = 'SimplicityV3'
    TOOLCHAIN = 'GCC_ARM'

    TARGETS = [
        'EFM32GG_STK3700',
        'EFM32ZG_STK3200',
        'EFM32LG_STK3600',
        'EFM32WG_STK3800',
        'EFM32HG_STK3400',
        'EFM32PG_STK3401'
    ]

    PARTS = {
        'EFM32GG_STK3700': 'com.silabs.mcu.si32.efm32.efm32gg.efm32gg990f1024',
        'EFM32ZG_STK3200': 'com.silabs.mcu.si32.efm32.efm32zg.efm32zg222f32',
        'EFM32LG_STK3600': 'com.silabs.mcu.si32.efm32.efm32lg.efm32lg990f256',
        'EFM32WG_STK3800': 'com.silabs.mcu.si32.efm32.efm32wg.efm32wg990f256',
        'EFM32HG_STK3400': 'com.silabs.mcu.si32.efm32.efm32hg.efm32hg322f64',
        'EFM32PG_STK3401': 'com.silabs.mcu.si32.efm32.efm32pg1b.efm32pg1b200f256gm48'
    }

    KITS = {
        'EFM32GG_STK3700': 'com.silabs.kit.si32.efm32.efm32gg.stk3700',
        'EFM32ZG_STK3200': 'com.silabs.kit.si32.efm32.efm32zg.stk3200',
        'EFM32LG_STK3600': 'com.silabs.kit.si32.efm32.efm32lg.stk3600',
        'EFM32WG_STK3800': 'com.silabs.kit.si32.efm32.efm32wg.stk3800',
        'EFM32HG_STK3400': 'com.silabs.kit.si32.efm32.efm32hg.slstk3400a',
		'EFM32PG_STK3401': 'com.silabs.kit.si32.efm32.efm32pg.slstk3401a'
    }

    FILE_TYPES = {
        'c_sources':'1',
        'cpp_sources':'1',
        's_sources':'1'
    }

    EXCLUDED_LIBS = [
        'm',
        'c',
        'gcc',
        'nosys',
        'supc++',
        'stdc++'
    ]

    DOT_IN_RELATIVE_PATH = False

    MBED_CONFIG_HEADER_SUPPORTED = True

    orderedPaths = Folder("Root")

    def check_and_add_path(self, path):
        levels = path.split('/')
        base = self.orderedPaths
        for level in levels:
            if base.contains(level):
                base = base.findChild(level)
            else:
                base.addChild(level)
                base = base.findChild(level)


    def generate(self):
        # "make" wants Unix paths
        self.resources.win_to_unix()

        main_files = []

        EXCLUDED_LIBS = [
            'm',
            'c',
            'gcc',
            'nosys',
            'supc++',
            'stdc++'
        ]

        for r_type in ['s_sources', 'c_sources', 'cpp_sources']:
            r = getattr(self.resources, r_type)
            if r:
                for source in r:
                    self.check_and_add_path(split(source)[0])

                    if not ('/' in source):
                        main_files.append(source)

        libraries = []
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            if l[3:] not in EXCLUDED_LIBS:
                libraries.append(l[3:])

        defines = []
        for define in self.toolchain.get_symbols():
            if '=' in define:
                keyval = define.split('=')
                defines.append( (keyval[0], keyval[1]) )
            else:
                defines.append( (define, '') )

        self.check_and_add_path(split(self.resources.linker_script)[0])

        ctx = {
            'name': self.project_name,
            'main_files': main_files,
            'recursiveFolders': self.orderedPaths,
            'object_files': self.resources.objects,
            'include_paths': self.resources.inc_dirs,
            'library_paths': self.resources.lib_dirs,
            'linker_script': self.resources.linker_script,
            'libraries': libraries,
            'defines': defines,
            'part': self.PARTS[self.target],
            'kit': self.KITS[self.target],
            'loopcount': 0
        }
        ctx.update(self.flags)

        ## Strip main folder from include paths because ssproj is not capable of handling it
        if '.' in ctx['include_paths']:
            ctx['include_paths'].remove('.')
            ctx['include_root'] = True

        '''
        Suppress print statements
        print('\n')
        print(self.target)
        print('\n')
        print(ctx)
        print('\n')
        print(self.orderedPaths)
        for path in self.orderedPaths.children:
            print(path.name + "\n")
            for bpath in path.children:
                print("\t" + bpath.name + "\n")
        '''

        self.gen_file('simplicity/slsproj.tmpl', ctx, '%s.slsproj' % self.project_name)

    @staticmethod
    def clean(project_name):
        remove('%s.slsproj' % project_name)
