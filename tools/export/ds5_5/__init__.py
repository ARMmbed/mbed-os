"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited

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
from os.path import basename

from tools.export.exporters import Exporter


class DS5_5(Exporter):
    NAME = 'DS5'

    TARGETS = [
        'LPC1768',
        'LPC11U24',
        'LPC812',
        'UBLOX_C027',
        'ARCH_PRO',
        'RZ_A1H',
        'VK_RZ_A1H',
        'GR_LYCHEE',
    ]

    USING_MICROLIB = [
        'LPC812',
    ]

    FILE_TYPES = {
        'c_sources':'1',
        'cpp_sources':'8',
        's_sources':'2'
    }

    TOOLCHAIN = "ARM"

    def get_toolchain(self):
        return 'uARM' if (self.target in self.USING_MICROLIB) else 'ARM'

    def generate(self):
        source_files = []
        for r_type, n in DS5_5.FILE_TYPES.items():
            for file in getattr(self.resources, r_type):
                source_files.append({
                    'name': basename(file), 'type': n, 'path': file
                })

        ctx = {
            'name': self.project_name,
            'include_paths': self.resources.inc_dirs,
            'scatter_file': self.resources.linker_script,
            'object_files': self.resources.objects + self.libraries,
            'source_files': source_files,
            'symbols': self.toolchain.get_symbols()
        }
        target = self.target.lower()

        # Project file
        self.gen_file('ds5_5/%s.project.tmpl' % target, ctx, '.project')
        self.gen_file('ds5_5/%s.cproject.tmpl' % target, ctx, '.cproject')
        self.gen_file('ds5_5/%s.launch.tmpl' % target, ctx, 'ds5_%s.launch' % target)
