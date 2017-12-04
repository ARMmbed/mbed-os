"""
mbed SDK
Copyright (c) 2014-2017 ARM Limited

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
from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter
from tools.export.makefile import GccArm

class QtCreator(GccArm):
    NAME = 'QtCreator'

    MBED_CONFIG_HEADER_SUPPORTED = True

    def generate(self):
        self.resources.win_to_unix()

        defines         = [] # list of tuples ('D'/'U', [key, value]) (value is optional)
        forced_includes = [] # list of strings
        sources         = [] # list of strings
        include_paths   = [] # list of strings

        next_is_include = False
        for f in self.flags['c_flags'] + self.flags['cxx_flags']:
            f=f.strip()
            if next_is_include:
                forced_includes.append(f)
                next_is_include = False
                continue
            if f.startswith('-D'):
                defines.append(('D', f[2:].split('=', 1)))
            elif f.startswith('-U'):
                defines.append(('U', [f[2:]]))
            elif f == "-include":
                next_is_include = True

        for r_type in ['headers', 'c_sources', 's_sources', 'cpp_sources']:
            sources.extend(getattr(self.resources, r_type))

        include_paths = self.resources.inc_dirs

        ctx = {
            'defines': defines,
            'forced_includes': forced_includes,
            'sources': sources,
            'include_paths': self.resources.inc_dirs
            }

        for ext in ['creator', 'files', 'includes', 'config']:
            self.gen_file('qtcreator/%s.tmpl' % ext, ctx, "%s.%s" % (self.project_name, ext))

        # finally, generate the Makefile
        super(QtCreator, self).generate()
