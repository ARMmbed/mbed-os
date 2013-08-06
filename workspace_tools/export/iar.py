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


class IAREmbeddedWorkbench(Exporter):
    NAME = 'IAR'
    TARGETS = ['LPC1768']
    TOOLCHAIN = 'IAR'
    
    def generate(self):
        ctx = {
            'name': self.program_name,
            'include_paths': self.resources.inc_dirs,
            'linker_script': self.resources.linker_script,
            'object_files': self.resources.objects,
            'libraries': self.resources.libraries,
        }
        self.gen_file('iar.ewp.tmpl', ctx, '%s.ewp' % self.program_name)
        self.gen_file('iar.eww.tmpl', ctx, '%s.eww' % self.program_name)
