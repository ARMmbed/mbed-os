"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

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
from os.path import basename


class Uvision4(Exporter):
    NAME = 'uVision4'
    TOOLCHAIN = 'ARM'
    TARGETS = ['LPC1768', 'LPC11U24', 'KL25Z', 'LPC1347', 'LPC1114', 'LPC11C24']
    FILE_TYPES = {
        'c_sources':'1',
        'cpp_sources':'8',
        's_sources':'2'
    }
    # By convention uVision projects do not show header files in the editor:
    # 'headers':'5',
    
    def generate(self):
        source_files = []
        for r_type, n in Uvision4.FILE_TYPES.iteritems():
            for file in getattr(self.resources, r_type):
                source_files.append({
                    'name': basename(file), 'type': n, 'path': file
                })
        ctx = {
            'name': self.program_name,
            'include_paths': self.resources.inc_dirs,
            'scatter_file': self.resources.linker_script,
            'object_files': self.resources.objects + self.resources.libraries,
            'source_files': source_files,
            'symbols': self.toolchain.get_symbols()
        }
        target = self.target.lower()
        
        # Project file
        self.gen_file('uvision4_%s.uvproj.tmpl' % target, ctx, '%s.uvproj' % self.program_name)
        self.gen_file('uvision4_%s.uvopt.tmpl' % target, ctx, '%s.uvopt' % self.program_name)
