"""
mbed SDK
Copyright (c) 2014 ARM Limited

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
from exporters import Exporter
from os.path import splitext, basename


class CoIDE(Exporter):
    NAME = 'CoIDE'
    TARGETS = ['KL25Z']
    TOOLCHAIN = 'GCC_ARM'

    def generate(self):
        to_be_compiled = []
        for r_type in ['s_sources', 'c_sources', 'cpp_sources']:
            r = getattr(self.resources, r_type)
            if r:
                for source in r:
                    base, ext = splitext(source)
                    to_be_compiled.append(base + '.o')

        libraries = []
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])

        ctx = {
            'name': self.program_name,
            'to_be_compiled': to_be_compiled,
            'include_paths': self.resources.inc_dirs,
            'library_paths': self.resources.lib_dirs,
            'scatter_file': self.resources.linker_script,
            'object_files': self.resources.objects,
            'symbols': self.toolchain.get_symbols()
        }
        target = self.target.lower()

        # Project file
        self.gen_file('coide_%s.coproj.tmpl' % target, ctx, '.coproj')
