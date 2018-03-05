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
from os.path import splitext, basename
from os import remove
from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter, apply_supported_whitelist


POST_BINARY_WHITELIST = set([
    "TEENSY3_1Code.binary_hook",
    "LPCTargetCode.lpc_patch",
    "LPC4088Code.binary_hook"
])


class EmBitz(Exporter):
    NAME = 'EmBitz'
    TOOLCHAIN = 'GCC_ARM'

    MBED_CONFIG_HEADER_SUPPORTED = True

    FILE_TYPES = {
        'headers': 'h',
        'c_sources': 'c',
        's_sources': 'a',
        'cpp_sources': 'cpp'
    }

    @classmethod
    def is_target_supported(cls, target_name):
        target = TARGET_MAP[target_name]
        return apply_supported_whitelist(
            cls.TOOLCHAIN, POST_BINARY_WHITELIST, target)

    @staticmethod
    def _remove_symbols(sym_list):
        return [s for s in sym_list if not s.startswith("-D")]

    def generate(self):
        self.resources.win_to_unix()
        source_files = []
        for r_type, n in self.FILE_TYPES.items():
            for file in getattr(self.resources, r_type):
                source_files.append({
                    'name': file, 'type': n
                })

        libraries = []
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])


        if self.resources.linker_script is None:
            self.resources.linker_script = ''

        ctx = {
            'name': self.project_name,
            'target': self.target,
            'toolchain': self.toolchain.name,
            'source_files': source_files,
            'include_paths': self.resources.inc_dirs,
            'script_file': self.resources.linker_script,
            'library_paths': self.resources.lib_dirs,
            'libraries': libraries,
            'symbols': self.toolchain.get_symbols(),
            'object_files': self.resources.objects,
            'sys_libs': self.toolchain.sys_libs,
            'cc_org': (self.flags['common_flags'] +
                       self._remove_symbols(self.flags['c_flags'])),
            'ld_org': self.flags['ld_flags'],
            'cppc_org': (self.flags['common_flags'] +
                         self._remove_symbols(self.flags['cxx_flags']))
        }

        self.gen_file('embitz/eix.tmpl', ctx, '%s.eix' % self.project_name)

    @staticmethod
    def clean(project_name):
        remove("%s.eix" % project_name)
