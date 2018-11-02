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
from __future__ import print_function, absolute_import
from builtins import str

from os.path import splitext, basename, relpath, join, abspath, dirname,\
    exists
from os import remove
import sys
from subprocess import check_output, CalledProcessError, Popen, PIPE
import shutil
from jinja2.exceptions import TemplateNotFound
from tools.resources import FileType
from tools.export.exporters import Exporter, apply_supported_whitelist
from tools.utils import NotSupportedException
from tools.targets import TARGET_MAP

SHELL_ESCAPE_TABLE = {
    "(": "\(",
    ")": "\)",
}


def shell_escape(string):
    return "".join(SHELL_ESCAPE_TABLE.get(char, char) for char in string)


class Makefile(Exporter):
    """Generic Makefile template that mimics the behavior of the python build
    system
    """

    DOT_IN_RELATIVE_PATH = True

    MBED_CONFIG_HEADER_SUPPORTED = True

    PREPROCESS_ASM = False

    POST_BINARY_WHITELIST = set([
        "MCU_NRF51Code.binary_hook",
        "TEENSY3_1Code.binary_hook",
        "LPCTargetCode.lpc_patch",
        "LPC4088Code.binary_hook"
    ])

    @classmethod
    def is_target_supported(cls, target_name):
        target = TARGET_MAP[target_name]
        return apply_supported_whitelist(
            cls.TOOLCHAIN, cls.POST_BINARY_WHITELIST, target)

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
                     in self.libraries]
        sys_libs = [self.prepare_sys_lib(lib) for lib
                    in self.toolchain.sys_libs]

        ctx = {
            'name': self.project_name,
            'to_be_compiled': to_be_compiled,
            'object_files': self.resources.objects,
            'include_paths': list(set(self.resources.inc_dirs)),
            'library_paths': self.resources.lib_dirs,
            'linker_script': self.resources.linker_script,
            'libraries': libraries,
            'ld_sys_libs': sys_libs,
            'hex_files': self.resources.hex_files,
            'vpath': (["../../.."]
                      if (basename(dirname(dirname(self.export_dir)))
                          == "projectfiles")
                      else [".."]),
            'cc_cmd': basename(self.toolchain.cc[0]),
            'cppc_cmd': basename(self.toolchain.cppc[0]),
            'asm_cmd': basename(self.toolchain.asm[0]),
            'ld_cmd': basename(self.toolchain.ld[0]),
            'elf2bin_cmd': basename(self.toolchain.elf2bin),
            'link_script_ext': self.toolchain.LINKER_EXT,
            'link_script_option': self.LINK_SCRIPT_OPTION,
            'user_library_flag': self.USER_LIBRARY_FLAG,
            'needs_asm_preproc': self.PREPROCESS_ASM,
            'shell_escape': shell_escape,
        }

        if hasattr(self.toolchain, "preproc"):
            ctx['pp_cmd'] = " ".join(
                [basename(self.toolchain.preproc[0])] +
                self.toolchain.preproc[1:] +
                self.toolchain.ld[1:]
            )
        else:
            ctx['pp_cmd'] = None

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
        ctx.update(self.format_flags())
        ctx['asm_flags'].extend(self.toolchain.asm[1:])
        ctx['c_flags'].extend(self.toolchain.cc[1:])
        ctx['cxx_flags'].extend(self.toolchain.cppc[1:])

        # Add the virtual path the the include option in the ASM flags
        new_asm_flags = []
        for flag in ctx['asm_flags']:
            if flag.startswith('-I'):
                new_asm_flags.append("-I{}/{}".format(ctx['vpath'][0], flag[2:]))
            elif flag.startswith('--preinclude='):
                new_asm_flags.append("--preinclude={}/{}".format(ctx['vpath'][0], flag[13:]))
            else:
                new_asm_flags.append(flag)
        ctx['asm_flags'] = new_asm_flags

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

    def format_flags(self):
        """Format toolchain flags for Makefile"""
        flags = {}
        for k, v in self.flags.items():
            if k in ['c_flags', 'cxx_flags']:
                flags[k] = map(lambda x: x.replace('"', '\\"'), v)
            else:
                flags[k] = v

        return flags

    @staticmethod
    def clean(_):
        remove("Makefile")
        # legacy .build directory cleaned if exists
        if exists('.build'):
            shutil.rmtree('.build')
        if exists('BUILD'):
            shutil.rmtree('BUILD')

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

        print(out_string)

        if log_name:
            # Write the output to the log file
            with open(log_name, 'w+') as f:
                f.write(out_string)

        # Cleanup the exported and built files
        if cleanup:
            remove(log_name)
            Makefile.clean(project_name)

        if ret_code != 0:
            # Seems like something went wrong.
            return -1
        else:
            return 0


class GccArm(Makefile):
    """GCC ARM specific makefile target"""
    NAME = 'Make-GCC-ARM'
    TEMPLATE = 'make-gcc-arm'
    TOOLCHAIN = "GCC_ARM"
    LINK_SCRIPT_OPTION = "-T"
    USER_LIBRARY_FLAG = "-L"

    @staticmethod
    def prepare_lib(libname):
        if "lib" == libname[:3]:
            libname = libname[3:-2]
        return "-l" + libname

    @staticmethod
    def prepare_sys_lib(libname):
        return "-l" + libname


class Arm(Makefile):
    """ARM Compiler generic makefile target"""
    LINK_SCRIPT_OPTION = "--scatter"
    USER_LIBRARY_FLAG = "--userlibpath "
    TEMPLATE = 'make-arm'

    @staticmethod
    def prepare_lib(libname):
        return libname

    @staticmethod
    def prepare_sys_lib(libname):
        return libname

    def generate(self):
        if self.resources.linker_script:
            sct_file = self.resources.get_file_refs(FileType.LD_SCRIPT)[-1]
            new_script = self.toolchain.correct_scatter_shebang(
                sct_file.path, join("..", dirname(sct_file.name)))
            if new_script is not sct_file:
                self.resources.add_files_to_type(
                    FileType.LD_SCRIPT, [new_script])
                self.generated_files.append(new_script)
        return super(Arm, self).generate()

class Armc5(Arm):
    """ARM Compiler 5 (armcc) specific makefile target"""
    NAME = 'Make-ARMc5'
    TOOLCHAIN = "ARM"
    PREPROCESS_ASM = True

class Armc6(Arm):
    """ARM Compiler 6 (armclang) specific generic makefile target"""
    NAME = 'Make-ARMc6'
    TOOLCHAIN = "ARMC6"


class IAR(Makefile):
    """IAR specific makefile target"""
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

    @staticmethod
    def prepare_sys_lib(libname):
        if "lib" == libname[:3]:
            libname = libname[3:]
        return "-l" + splitext(libname)[0]
