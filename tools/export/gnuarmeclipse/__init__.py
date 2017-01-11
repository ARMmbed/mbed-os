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
from os.path import splitext, basename, relpath
from random import randint
import copy

from tools.targets import TARGET_MAP
from tools.utils import NotSupportedException

class UID:
    @property
    def id(self):
        return "%0.9u" % randint(0, 999999999)

u = UID()

class GNUARMEclipse(Exporter):
    NAME = 'GNU ARM Eclipse'
    TOOLCHAIN = 'GCC_ARM'

    # Indirectly support all GCC_ARM targets.
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if 'GCC_ARM' in obj.supported_toolchains]

    @staticmethod
    def filter_dot(s):
        if s == None:
            return None
        if s[:2] == './':
            return s[2:]
        return s

    @property
    def flags(self):
        """Returns a dictionary of toolchain flags.
        Keys of the dictionary are:
        cxx_flags    - c++ flags
        c_flags      - c flags
        ld_flags     - linker flags
        asm_flags    - assembler flags
        common_flags - common options
        """
        config_header = self.toolchain.get_config_header()
        flags = {key + "_flags": copy.deepcopy(value) for key, value
                 in self.toolchain.flags.iteritems()}
        if config_header:
            config_header = relpath(config_header,
                                    self.resources.file_basepath[config_header])
            flags['c_flags'] += self.toolchain.get_config_option(config_header)
            flags['cxx_flags'] += self.toolchain.get_config_option(
                config_header)
        return flags


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
            raise NotSupportedException('Target core {0} not supported.'.format(core))

        # TODO: clarify how to use objects; enabling this adds another
        # object 'main.o'.
        objects = [] # self.resources.objects
        print self.resources.objects

        f = self.flags
        print 'common_flags'
        print f['common_flags']
        print 'asm_flags'
        print f['asm_flags']
        print 'c_flags'
        print f['c_flags']
        print 'cxx_flags'
        print f['cxx_flags']
        print 'ld_flags'
        print f['ld_flags']

        asm_defines = self.toolchain.get_symbols(True)
        c_defines = self.toolchain.get_symbols()
        cpp_defines = self.toolchain.get_symbols()
        
        include_paths = [self.filter_dot(s) for s in self.resources.inc_dirs]

        library_paths = [self.filter_dot(s) for s in self.resources.lib_dirs]

        linker_script = self.filter_dot(self.resources.linker_script)

        ctx = {
            'name': self.project_name,
            'include_paths': include_paths,
            'library_paths': library_paths,
            'object_files': objects,
            'libraries': libraries,
            'linker_script': linker_script,
            'asm_defines': asm_defines,
            'c_defines': c_defines,
            'cpp_defines': cpp_defines,
            'target_mcpu': target_mcpu,
            'target_fpu_abi': target_fpu_abi,
            'target_fpu_unit': target_fpu_unit,

            # Unique IDs used each in multiple places.
            # Add more if needed.
            'debug_config_uid': u.id,
            'debug_tool_c_compiler_uid': u.id,
            'debug_tool_c_compiler_input_uid': u.id,
            'debug_tool_cpp_compiler_uid': u.id,
            'debug_tool_cpp_compiler_input_uid': u.id,
            'release_config_uid': u.id,
            'release_tool_c_compiler_uid': u.id,
            'release_tool_c_compiler_input_uid': u.id,
            'release_tool_cpp_compiler_uid': u.id,
            'release_tool_cpp_compiler_input_uid': u.id,

            # Must be an object with an `id` property, which
            # will be called repeatedly, to generate multiple UIDs.
            'u': u,
        }

        # ctx.update(f)

        self.gen_file('gnuarmeclipse/.project.tmpl', ctx, '.project')
        self.gen_file('gnuarmeclipse/.cproject.tmpl', ctx, '.cproject')
