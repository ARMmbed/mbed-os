# mbed SDK
# Copyright (c) 2011-2016 ARM Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
from __future__ import print_function, absolute_import
from builtins import str


from os.path import join, exists, realpath, relpath, basename, isfile, splitext
from os import makedirs, listdir, remove, rmdir
import json

from tools.export.makefile import Makefile, GccArm, Armc5, IAR

class VSCode(Makefile):
    """Generic VSCode project. Intended to be subclassed by classes that
    specify a type of Makefile.
    """
    def generate(self):
        """Generate Makefile and VSCode launch and task files
        """
        super(VSCode, self).generate()

        # So.... I want all .h and .hpp files in self.resources.inc_dirs
        all_directories = []

        for directory in self.resources.inc_dirs:
            if not directory:
                continue

            if directory == ".":
                all_directories.append("${workspaceFolder}/*")
            else:
                all_directories.append(directory.replace("./", "${workspaceFolder}/") + "/*")

        cpp_props = {
            "configurations": [
                {
                    "name": "Windows",
                    "forcedInclude": [
                        "${workspaceFolder}/mbed_config.h"
                    ],
                    "compilerPath": self.toolchain.cppc[0],
                    "intelliSenseMode": "gcc-x64",
                    "includePath": all_directories,
                    "defines": [symbol for symbol in self.toolchain.get_symbols()]
                },
                {
                    "name": "Mac",
                    "forcedInclude": [
                        "${workspaceFolder}/mbed_config.h"
                    ],
                    "compilerPath": self.toolchain.cppc[0],
                    "includePath": all_directories,
                    "defines": [symbol for symbol in self.toolchain.get_symbols()]
                },
                {
                    "name": "Linux",
                    "forcedInclude": [
                        "${workspaceFolder}/mbed_config.h"
                    ],
                    "compilerPath": self.toolchain.cppc[0],
                    "includePath": all_directories,
                    "defines": [symbol for symbol in self.toolchain.get_symbols()]
                }
            ]
        }

        ctx = {
            'name': self.project_name,
            'elf_location': join('BUILD', self.project_name)+'.elf',
            'c_symbols': self.toolchain.get_symbols(),
            'asm_symbols': self.toolchain.get_symbols(True),
            'target': self.target,
            'include_paths': self.resources.inc_dirs,
            'load_exe': str(self.LOAD_EXE).lower(),
            'cpp_props': json.dumps(cpp_props, indent=4, separators=(',', ': '))
        }

        if not exists(join(self.export_dir, '.vscode')):
            makedirs(join(self.export_dir, '.vscode'))

        config_files = ['launch', 'settings', 'tasks', 'c_cpp_properties']
        for file in config_files:
            if not exists('.vscode/%s.json' % file):
                self.gen_file('vscode/%s.tmpl' % file, ctx,
                              '.vscode/%s.json' % file)
            else:
                print('Keeping existing %s.json' % file)

    @staticmethod
    def clean(_):
        for f in ['launch', 'settings', 'tasts', 'c_cpp_properties']:
            remove(".vscode/%s.json" % f)
        rmdir(".vscode")

class VSCodeGcc(VSCode, GccArm):
    LOAD_EXE = True
    NAME = "VSCode-GCC-ARM"

class VSCodeArmc5(VSCode, Armc5):
    LOAD_EXE = True
    NAME = "VSCode-Armc5"

class VSCodeIAR(VSCode, IAR):
    LOAD_EXE = True
    NAME = "VSCode-IAR"
