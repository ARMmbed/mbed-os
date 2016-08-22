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
from os.path import splitext, basename, relpath, join, abspath, dirname,\
    exists
from os import curdir, getcwd
from tools.export.exporters import Exporter
from tools.utils import NotSupportedException
from tools.targets import TARGET_MAP
from jinja2.exceptions import TemplateNotFound


class Makefile(Exporter):

    DOT_IN_RELATIVE_PATH = True

    MBED_CONFIG_HEADER_SUPPORTED = True

    def generate(self):
        # "make" wants Unix paths
        self.resources.win_to_unix()

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
            'name': self.project_name,
            'to_be_compiled': to_be_compiled,
            'object_files': self.resources.objects,
            'include_paths': self.resources.inc_dirs,
            'library_paths': self.resources.lib_dirs,
            'linker_script': self.resources.linker_script,
            'libraries': libraries,
            'symbols': self.toolchain.get_symbols(),
            'hex_files': self.resources.hex_files,
            'vpath': (["../../.."]
                      if basename(dirname(dirname(self.export_dir))) == "projectfiles"
                      else [".."]),
            'cc_cmd': " ".join(self.toolchain.cc),
            'cppc_cmd': " ".join(self.toolchain.cppc),
            'asm_cmd': " ".join(self.toolchain.asm),
            'ld_cmd': " ".join(self.toolchain.ld),
            'elf2bin_cmd': self.toolchain.elf2bin,
            'link_script_ext': self.toolchain.LINKER_EXT,
            'link_script_option': self.LINK_SCRIPT_OPTION,
        }

        for key in ['include_paths', 'library_paths', 'linker_script', 'hex_files']:
            if isinstance(ctx[key], list):
                ctx[key] = [ctx['vpath'][0] + "/" + t for t in ctx[key]]
            else:
                ctx[key] = ctx['vpath'][0] + "/" + ctx[key]
        if "../." not in ctx["include_paths"]:
            ctx["include_paths"] += ['../.']
        ctx["include_paths"] = list(set(ctx["include_paths"]))
        for key in ['include_paths', 'library_paths', 'hex_files', 'to_be_compiled', 'symbols']:
            ctx[key] = sorted(ctx[key])
        ctx.update(self.flags)

        for templatefile in \
            ['makefile/%s_%s.tmpl' % (self.NAME.lower(),
                                      self.target.lower())] + \
            ['makefile/%s_%s.tmpl' % (self.NAME.lower(),
                                      label.lower()) for label
             in self.toolchain.target.extra_labels] +\
            ['makefile/%s.tmpl' % self.NAME.lower()]:
            try:
                self.gen_file(templatefile, ctx, 'Makefile')
                break
            except TemplateNotFound:
                pass
        else:
            raise NotSupportedException("This make tool is in development")


class GccArm_Exporter(Makefile):
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if "GCC_ARM" in obj.supported_toolchains]
    NAME = 'Make-GCC-ARM'
    TOOLCHAIN = "GCC_ARM"
    LINK_SCRIPT_OPTION = "-T"


class Armc5_Exporter(Makefile):
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if "ARM" in obj.supported_toolchains]
    NAME = 'Make-ARMc5'
    TOOLCHAIN = "ARM"
    LINK_SCRIPT_OPTION = "--scatter"


class IAR_Exporter(Makefile):
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if "IAR" in obj.supported_toolchains]
    NAME = 'Make-IAR'
    TOOLCHAIN = "IAR"
    LINK_SCRIPT_OPTION = "--config"
