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
from os import remove
import sys
from subprocess import check_output, CalledProcessError, Popen, PIPE
import shutil
from jinja2.exceptions import TemplateNotFound
from tools.export.exporters import Exporter
from tools.utils import NotSupportedException
from tools.targets import TARGET_MAP


class Makefile(Exporter):
    """Generic Makefile template that mimics the behavior of the python build
    system
    """

    DOT_IN_RELATIVE_PATH = True

    MBED_CONFIG_HEADER_SUPPORTED = True

    def generate(self):
        """Generate the makefile

        Note: subclasses should not need to override this method
        """
        if not self.resources.linker_script:
            raise NotSupportedException("No linker script found.")

        self.resources.win_to_unix()

        to_be_compiled = [splitext(src)[0] + ".o" for src in
                          self.resources.s_sources +
                          self.resources.c_sources +
                          self.resources.cpp_sources]

        libraries = [self.prepare_lib(basename(lib)) for lib
                     in self.resources.libraries]

        ctx = {
            'name': self.project_name,
            'to_be_compiled': to_be_compiled,
            'object_files': self.resources.objects,
            'include_paths': list(set(self.resources.inc_dirs)),
            'library_paths': self.resources.lib_dirs,
            'linker_script': self.resources.linker_script,
            'libraries': libraries,
            'hex_files': self.resources.hex_files,
            'vpath': (["../../.."]
                      if (basename(dirname(dirname(self.export_dir)))
                          == "projectfiles")
                      else [".."]),
            'cc_cmd': " ".join(["\'" + part + "\'" for part
                                in ([basename(self.toolchain.cc[0])] +
                                    self.toolchain.cc[1:])]),
            'cppc_cmd': " ".join(["\'" + part + "\'" for part
                                  in ([basename(self.toolchain.cppc[0])] +
                                      self.toolchain.cppc[1:])]),
            'asm_cmd': " ".join(["\'" + part + "\'" for part
                                in ([basename(self.toolchain.asm[0])] +
                                    self.toolchain.asm[1:])]),
            'ld_cmd': " ".join(["\'" + part + "\'" for part
                                in ([basename(self.toolchain.ld[0])] +
                                    self.toolchain.ld[1:])]),
            'elf2bin_cmd': "\'" + basename(self.toolchain.elf2bin) + "\'",
            'link_script_ext': self.toolchain.LINKER_EXT,
            'link_script_option': self.LINK_SCRIPT_OPTION,
            'user_library_flag': self.USER_LIBRARY_FLAG,
        }

        for key in ['include_paths', 'library_paths', 'linker_script',
                    'hex_files']:
            if isinstance(ctx[key], list):
                ctx[key] = [ctx['vpath'][0] + "/" + t for t in ctx[key]]
            else:
                ctx[key] = ctx['vpath'][0] + "/" + ctx[key]
        if "../." not in ctx["include_paths"]:
            ctx["include_paths"] += ['../.']
        for key in ['include_paths', 'library_paths', 'hex_files',
                    'to_be_compiled']:
            ctx[key] = sorted(ctx[key])
        ctx.update(self.flags)

        for templatefile in \
            ['makefile/%s_%s.tmpl' % (self.TEMPLATE,
                                      self.target.lower())] + \
            ['makefile/%s_%s.tmpl' % (self.TEMPLATE,
                                      label.lower()) for label
             in self.toolchain.target.extra_labels] +\
            ['makefile/%s.tmpl' % self.TEMPLATE]:
            try:
                self.gen_file(templatefile, ctx, 'Makefile')
                break
            except TemplateNotFound:
                pass
        else:
            raise NotSupportedException("This make tool is in development")

    @staticmethod
    def build(project_name, log_name="build_log.txt", cleanup=True):
        """ Build Make project """
        # > Make -j
        cmd = ["make", "-j"]

        # Build the project
        p = Popen(cmd, stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        ret_code = p.returncode

        out_string = "=" * 10 + "STDOUT" + "=" * 10 + "\n"
        out_string += out
        out_string += "=" * 10 + "STDERR" + "=" * 10 + "\n"
        out_string += err

        if ret_code == 0:
            out_string += "SUCCESS"
        else:
            out_string += "FAILURE"

        print out_string

        if log_name:
            # Write the output to the log file
            with open(log_name, 'w+') as f:
                f.write(out_string)

        # Cleanup the exported and built files
        if cleanup:
            remove("Makefile")
            remove(log_name)
            # legacy .build directory cleaned if exists
            if exists('.build'):
                shutil.rmtree('.build')
            if exists('BUILD'):
                shutil.rmtree('BUILD')

        if ret_code != 0:
            # Seems like something went wrong.
            return -1
        else:
            return 0


class GccArm(Makefile):
    """GCC ARM specific makefile target"""
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if "GCC_ARM" in obj.supported_toolchains]
    NAME = 'Make-GCC-ARM'
    TEMPLATE = 'make-gcc-arm'
    TOOLCHAIN = "GCC_ARM"
    LINK_SCRIPT_OPTION = "-T"
    USER_LIBRARY_FLAG = "-L"

    @staticmethod
    def prepare_lib(libname):
        return "-l:" + libname


class Armc5(Makefile):
    """ARM Compiler 5 specific makefile target"""
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if "ARM" in obj.supported_toolchains]
    NAME = 'Make-ARMc5'
    TEMPLATE = 'make-armc5'
    TOOLCHAIN = "ARM"
    LINK_SCRIPT_OPTION = "--scatter"
    USER_LIBRARY_FLAG = "--userlibpath "

    @staticmethod
    def prepare_lib(libname):
        return libname


class IAR(Makefile):
    """IAR specific makefile target"""
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if "IAR" in obj.supported_toolchains]
    NAME = 'Make-IAR'
    TEMPLATE = 'make-iar'
    TOOLCHAIN = "IAR"
    LINK_SCRIPT_OPTION = "--config"
    USER_LIBRARY_FLAG = "-L"

    @staticmethod
    def prepare_lib(libname):
        if "lib" == libname[:3]:
            libname = libname[3:]
        return "-l" + splitext(libname)[0]
