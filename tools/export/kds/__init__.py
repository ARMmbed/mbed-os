"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited
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
from os.path import splitext, basename
from os import remove

from tools.export.exporters import Exporter, deprecated_exporter


@deprecated_exporter
class KDS(Exporter):
    NAME = 'Kinetis Design Studio'
    TOOLCHAIN = 'GCC_ARM'

    TARGETS = [
        'K64F',
        'HEXIWEAR',
        'K22F',
    ]

    def generate(self):
        libraries = []
        for lib in self.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])

        ctx = {
            'name': self.project_name,
            'include_paths': self.resources.inc_dirs,
            'linker_script': self.resources.linker_script,
            'object_files': self.resources.objects,
            'libraries': libraries,
            'symbols': self.toolchain.get_symbols()
        }
        self.gen_file('kds/%s_project.tmpl' % self.target.lower(), ctx, '.project')
        self.gen_file('kds/%s_cproject.tmpl' % self.target.lower(), ctx, '.cproject')
        self.gen_file('kds/launch.tmpl', ctx, '%s.launch' % self.project_name)

    @staticmethod
    def clean(project_name):
        remove('%s.launch' % project_name)
