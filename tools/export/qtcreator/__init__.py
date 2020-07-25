"""
mbed SDK
Copyright (c) 2014-2017 ARM Limited
SPDX-License-Identifier: Apache-2.0

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
import itertools
import os
import os.path

from tools.export.makefile import GccArm


class QtCreator(GccArm):
    NAME = 'QtCreator'

    MBED_CONFIG_HEADER_SUPPORTED = True

    def generate(self):
        self.resources.win_to_unix()

        defines = []  # list of tuples ('D'/'U', (key, value)) (value is optional)
        defines_set = set()  # "set" version of defines for deduplication purposes
        forced_includes = []  # list of strings
        sources = []  # list of strings

        next_is_include = False
        all_compiler_flags_iter = itertools.chain(
            self.flags['common_flags'],
            self.flags['c_flags'],
            self.toolchain.cc[1:],
            self.flags['cxx_flags'],
            self.toolchain.cppc[1:],
        )

        for compiler_flag in all_compiler_flags_iter:
            compiler_flag = compiler_flag.strip()
            if next_is_include:
                forced_includes.append(compiler_flag)
                next_is_include = False
                continue
            if compiler_flag.startswith('-D'):
                macro = tuple(compiler_flag[2:].split('=', 1))
                macro_define = ('D', macro)
                # prevent macro duplication
                if macro_define not in defines_set or ('U', macro[0]) in defines_set:
                    defines.append(macro_define)
                    defines_set.add(macro_define)
            elif compiler_flag.startswith('-U'):
                macro_define = ('U', (compiler_flag[2:],))
                defines.append(macro_define)
                defines_set.add(macro_define)
            elif compiler_flag == "-include":
                next_is_include = True

        # deduplicate includes
        forced_includes = sorted(set(os.path.normpath(p) for p in forced_includes))

        for r_type in ('headers', 'c_sources', 's_sources', 'cpp_sources'):
            sources.extend(getattr(self.resources, r_type))

        ctx = {
            'defines': defines,
            'forced_includes': forced_includes,
            'sources': sources,
            'include_paths': self.resources.inc_dirs
        }

        for ext in ('creator', 'files', 'includes', 'config'):
            self.gen_file('qtcreator/{}.tmpl'.format(ext), ctx, "{}.{}".format(self.project_name, ext))

        # finally, generate the Makefile
        super(QtCreator, self).generate()

    @staticmethod
    def clean(project_name):
        for ext in ('creator', 'files', 'includes', 'config'):
            os.remove("{}.{}".format(project_name, ext))
