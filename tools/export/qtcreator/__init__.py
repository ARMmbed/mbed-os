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
from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter, filter_supported


class QtCreator(Exporter):
    NAME = 'QtCreator'
    TOOLCHAIN = 'GCC_ARM'

    TARGETS = filter_supported("GCC_ARM", set())

    MBED_CONFIG_HEADER_SUPPORTED = True

    def generate(self):
        self.resources.win_to_unix()

        with open("%s.creator" % self.project_name, "w") as fd:
            fd.write("[General]\n")

        next_is_include = False
        includes = []
        with open("%s.config" % self.project_name, "w") as fd:
            for f in self.flags['c_flags'] + self.flags['cxx_flags']:
                f=f.strip()
                if next_is_include:
                    includes.append(f)
                    next_is_include = False
                    continue
                if f.startswith('-D'):
                    fd.write("#define %s\n" % (f[2:].replace('=', ' ')))
                elif f.startswith('-U'):
                    fd.write("#undef %s\n" % f[2:])
                elif f == "-include":
                    next_is_include = True
            for i in includes:
                fd.write("#include \"%s\"\n" % i)

        with open("%s.files" % self.project_name, "w") as fd:
            for r_type in ['headers', 'c_sources', 's_sources', 'cpp_sources']:
                for f in getattr(self.resources, r_type):
                    fd.write(f + "\n")

        with open("%s.includes" % self.project_name, "w") as fd:
            for i in self.resources.inc_dirs:
                fd.write(i + "\n")
