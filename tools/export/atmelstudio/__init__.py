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
import uuid
from os.path import splitext, basename, dirname
from os import remove

from tools.export.exporters import Exporter, deprecated_exporter


@deprecated_exporter
class AtmelStudio(Exporter):
    NAME = 'AtmelStudio'
    TOOLCHAIN = 'GCC_ARM'

    TARGETS = [
        'SAMD21J18A',
        'SAMR21G18A',
        'SAMD21G18A',
        'SAML21J18A',
        'SAMG55J19',
    ]

    DOT_IN_RELATIVE_PATH = True

    MBED_CONFIG_HEADER_SUPPORTED = True

    @classmethod
    def is_target_supported(cls, maybe_supported):
        return maybe_supported in cls.TARGETS

    def generate(self):

        source_files = []
        dirs = []
        for r_type in ['s_sources', 'c_sources', 'cpp_sources']:
            r = getattr(self.resources, r_type)
            if r:
                for source in r:
                    source_files.append(source[2:])
                    dirs.append(dirname(source[2:]))

        source_folders = []
        for e in dirs:
            if e and e not in source_folders:
                source_folders.append(e)

        libraries = []
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])
		
        solution_uuid = '{' + str(uuid.uuid4()) + '}'
        project_uuid = '{' + str(uuid.uuid4()) + '}'

        ctx = {
            'target': self.target,
            'name': self.project_name,
            'source_files': source_files,
            'source_folders': source_folders,
            'object_files': self.resources.objects,
            'include_paths': self.resources.inc_dirs,
            'library_paths': self.resources.lib_dirs,
            'linker_script': self.resources.linker_script,
            'libraries': libraries,
            'symbols': self.toolchain.get_symbols(),
            'solution_uuid': solution_uuid.upper(),
            'project_uuid': project_uuid.upper()
        }
        ctx.update(self.flags)
        target = self.target.lower()
        self.gen_file('atmelstudio/atsln.tmpl', ctx, '%s.atsln' % self.project_name)
        self.gen_file('atmelstudio/cppproj.tmpl', ctx, '%s.cppproj' % self.project_name)

    @staticmethod
    def clean(project_name):
        remove('%s.atsln' % project_name)
        remove('%s.cppproj' % project_name)
