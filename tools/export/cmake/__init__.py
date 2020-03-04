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
from __future__ import print_function, absolute_import
from builtins import str

import re
import shutil
from os import remove, getcwd, chdir, mkdir
from os.path import basename, exists
from subprocess import Popen, PIPE

from jinja2.exceptions import TemplateNotFound

from tools.export.exporters import Exporter, apply_supported_whitelist
from tools.targets import TARGET_MAP


class CMake(Exporter):
    """Generic CMake template that mimics the behavior of the python build
    system
    """

    TEMPLATE = 'CMakeLists.txt'

    MBED_CONFIG_HEADER_SUPPORTED = True

    PREPROCESS_ASM = False

    POST_BINARY_WHITELIST = set([
        "MCU_NRF51Code.binary_hook",
        "TEENSY3_1Code.binary_hook",
        "LPCTargetCode.lpc_patch",
        "LPC4088Code.binary_hook",
        "PSOC6Code.complete"
    ])

    @classmethod
    def is_target_supported(cls, target_name):
        target = TARGET_MAP[target_name]
        return apply_supported_whitelist(
            cls.TOOLCHAIN, cls.POST_BINARY_WHITELIST, target)

    def generate(self):
        """Generate the CMakefiles.txt
        """
        self.resources.win_to_unix()

        # get all source files including headers, adding headers allows IDEs to detect which files
        # belong to the project, otherwise headers may be greyed out and not work with inspection
        # (that is true for CLion and definitely for Visual Code)
        srcs = set(self.resources.c_sources +
                   self.resources.cpp_sources +
                   self.resources.s_sources +
                   self.resources.headers)
        srcs = [re.sub(r'^[.]/', '', f) for f in srcs]

        # additional libraries
        libraries = [self.prepare_lib(basename(lib)) for lib in self.libraries]
        sys_libs = [self.prepare_sys_lib(lib) for lib in self.toolchain.sys_libs]

        # sort includes reverse, so the deepest dir comes first (ensures short includes)
        includes = sorted([re.sub(r'^[.]/', '', l) for l in self.resources.inc_dirs], reverse=True)

        ctx = {
            'name': self.project_name,
            'target': self.target,
            'sources': sorted(srcs),
            'libraries': libraries,
            'ld_sys_libs': sys_libs,
            'include_paths': includes,
            'library_paths': sorted([re.sub(r'^[.]/', '', l) for l in self.resources.lib_dirs]),
            'linker_script': self.resources.linker_script,
            'hex_files': self.hex_files,
            'ar': basename(self.toolchain.ar),
            'cc': basename(self.toolchain.cc[0]),
            'cc_flags': " ".join(flag for flag in self.toolchain.cc[1:] if not flag == "-c"),
            'cxx': basename(self.toolchain.cppc[0]),
            'cxx_flags': " ".join(flag for flag in self.toolchain.cppc[1:] if not flag == "-c"),
            'asm': basename(self.toolchain.asm[0]),
            'asm_flags': " ".join(flag for flag in self.toolchain.asm[1:] if not flag == "-c"),
            'symbols': sorted(self.toolchain.get_symbols()),
            'ld': basename(self.toolchain.ld[0]),
            # fix the missing underscore '_' (see
            'ld_flags': re.sub("--wrap,_(?!_)", "--wrap,__", " ".join(self.toolchain.ld[1:])),
            'elf2bin': basename(self.toolchain.elf2bin),
            'link_script_ext': self.toolchain.LINKER_EXT,
            'link_script_option': self.LINK_SCRIPT_OPTION,
            'user_library_flag': self.USER_LIBRARY_FLAG,
            'needs_asm_preproc': self.PREPROCESS_ASM,
        }

        if hasattr(self.toolchain, "preproc"):
            ctx['pp'] = basename(self.toolchain.preproc[0])
            ctx['pp_flags'] = " ".join(self.toolchain.preproc[1:] +
                                       self.toolchain.ld[1:])
        else:
            ctx['pp'] = None
            ctx['pp_flags'] = None

        try:
            self.gen_file('cmake/%s.tmpl' % self.TEMPLATE, ctx, 'CMakeLists.txt')
        except TemplateNotFound:
            pass

    @staticmethod
    def clean(_):
        remove("CMakeLists.txt")
        # legacy .build directory cleaned if exists
        if exists('.build'):
            shutil.rmtree('.build')
        if exists('BUILD'):
            shutil.rmtree('BUILD')

    @staticmethod
    def build(project_name, log_name="build_log.txt", cleanup=True):
        """ Build Make project """

        # change into our build directory
        current_dir = getcwd()
        if not exists("BUILD"):
            mkdir("BUILD")
        chdir("BUILD")

        # > run cmake initial command
        cmd = ["cmake", ".."]

        # Build the project
        p = Popen(cmd, stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        ret_code = p.returncode

        if ret_code == 0:
            # we create the cmake files inside BUILD, change into and run cmake

            # > run make -j
            cmd = ["make", "-j"]

            p = Popen(cmd, stdout=PIPE, stderr=PIPE)
            out, err = p.communicate()
            ret_code = p.returncode

        # go back to the original directory
        chdir(current_dir)

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
            CMake.clean(project_name)

        if ret_code != 0:
            # Seems like something went wrong.
            return -1
        else:
            return 0


class GccArm(CMake):
    """GCC ARM specific cmake target"""
    NAME = 'CMake-GCC-ARM'
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

# class Arm(CMake):
#     """ARM Compiler generic cmake target"""
#     LINK_SCRIPT_OPTION = "--scatter"
#     USER_LIBRARY_FLAG = "--userlibpath "
#
#     @staticmethod
#     def prepare_lib(libname):
#         return libname
#
#     @staticmethod
#     def prepare_sys_lib(libname):
#         return libname
#
#     def generate(self):
#         if self.resources.linker_script:
#             new_script = self.toolchain.correct_scatter_shebang(
#                 self.resources.linker_script)
#             if new_script is not self.resources.linker_script:
#                 self.resources.linker_script = new_script
#                 self.generated_files.append(new_script)
#         return super(Arm, self).generate()
#
#
# class Armc5(Arm):
#     """ARM Compiler 5 (armcc) specific makefile target"""
#     NAME = 'CMake-ARMc5'
#     TOOLCHAIN = "ARM"
#     PREPROCESS_ASM = True
#
#
# class Armc6(Arm):
#     """ARM Compiler 6 (armclang) specific generic makefile target"""
#     NAME = 'CMake-ARMc6'
#     TOOLCHAIN = "ARMC6"
#
#
# class IAR(CMake):
#     """IAR specific cmake target"""
#     NAME = 'CMake-IAR'
#     TOOLCHAIN = "IAR"
#     LINK_SCRIPT_OPTION = "--config"
#     USER_LIBRARY_FLAG = "-L"
#
#     @staticmethod
#     def prepare_lib(libname):
#         if "lib" == libname[:3]:
#             libname = libname[3:]
#         return "-l" + splitext(libname)[0]
#
#     @staticmethod
#     def prepare_sys_lib(libname):
#         if "lib" == libname[:3]:
#             libname = libname[3:]
#         return "-l" + splitext(libname)[0]
