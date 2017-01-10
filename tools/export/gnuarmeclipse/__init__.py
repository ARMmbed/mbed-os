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
from tools.export.exporters import Exporter
from os.path import splitext, basename
from random import randint

from tools.targets import TARGET_MAP


class GNUARMEclipse(Exporter):
    NAME = 'GNU ARM Eclipse'
    TOOLCHAIN = 'GCC_ARM'

    # Indirectly support all GCC_ARM targets.
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if 'GCC_ARM' in obj.supported_toolchains]

    def __generate_uid(self):
        return "%0.9u" % randint(0, 999999999)



    def generate(self):
        libraries = []
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])

        MCPUS = {
            'Cortex-M0': {'mcpu': 'cortex-m0', 'fpu_unit': None},
            'Cortex-M0+': {'mcpu': 'cortex-m0plus', 'fpu_unit': None},
            'Cortex-M1': {'mcpu': 'cortex-m1', 'fpu_unit': None},
            'Cortex-M3': {'mcpu': 'cortex-m3', 'fpu_unit': None},
            'Cortex-M4': {'mcpu': 'cortex-m4', 'fpu_unit': None},
            'Cortex-M4F': {'mcpu': 'cortex-m4', 'fpu_unit': 'fpv4spd16'},
            'Cortex-M7': {'mcpu': 'cortex-m7', 'fpu_unit': None},
            'Cortex-M7F': {'mcpu': 'cortex-m7', 'fpu_unit': 'fpv4spd16'},
            'Cortex-M7FD': {'mcpu': 'cortex-m7', 'fpu_unit': 'fpv5d16'},
        }

        core = self.toolchain.target.core

        try:
            # cortex-m0, cortex-m0-small-multiply, cortex-m0plus, 
            # cortex-m0plus-small-multiply, cortex-m1, cortex-m1-small-multiply,
            # cortex-m3, cortex-m4, cortex-m7.
            target_mcpu = MCPUS[core]['mcpu']

            # default, fpv4spd16, fpv5d16, fpv5spd16
            target_fpu_unit = MCPUS[core]['fpu_unit']

            # soft, softfp, hard.
            target_fpu_abi = 'soft'

            if target_fpu_unit != None:
                target_fpu_abi = 'hard'
                
        except AttributeError:
            # TODO filter out projects with toolchain core not supported,
            # instead of raising an exception.
            raise Exception('Target core {0} not supported.'.format(core))

        # TODO: clarify how to use objects; enabling this adds another
        # object 'main.o'.
        objects = [] # self.resources.objects

        ctx = {
            'name': self.project_name,
            'include_paths': self.resources.inc_dirs,
            'linker_script': self.resources.linker_script,
            'library_paths': self.resources.lib_dirs,
            'object_files': objects,
            'libraries': libraries,
            'symbols': self.toolchain.get_symbols(),
            'target_mcpu': target_mcpu,
            'target_fpu_abi': target_fpu_abi,
            'target_fpu_unit': target_fpu_unit,
            # 
            'debug_config_uid': self.__generate_uid(),
            'debug_tool_c_compiler_uid': self.__generate_uid(),
            'debug_tool_c_compiler_input_uid': self.__generate_uid(),
            'debug_tool_cpp_compiler_uid': self.__generate_uid(),
            'debug_tool_cpp_compiler_input_uid': self.__generate_uid(),
            'release_config_uid': self.__generate_uid(),
            'release_tool_c_compiler_uid': self.__generate_uid(),
            'release_tool_c_compiler_input_uid': self.__generate_uid(),
            'release_tool_cpp_compiler_uid': self.__generate_uid(),
            'release_tool_cpp_compiler_input_uid': self.__generate_uid(),
            'uid': self.__generate_uid()
        }
        self.gen_file('gnuarmeclipse/.project.tmpl', ctx, '.project')
        self.gen_file('gnuarmeclipse/.cproject.tmpl', ctx, '.cproject')
        # self.gen_file('kds_launch.tmpl', ctx, '%s.launch' % self.project_name)
