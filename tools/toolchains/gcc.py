"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited
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
import re
import fnmatch
from os.path import join, basename, splitext, dirname, exists
from os import getcwd, getenv
from distutils.spawn import find_executable
from distutils.version import LooseVersion

from tools.toolchains.mbed_toolchain import (
    mbedToolchain, TOOLCHAIN_PATHS, should_replace_small_c_lib
)
from tools.utils import run_cmd


class GCC(mbedToolchain):
    OFFICIALLY_SUPPORTED = True
    LINKER_EXT = '.ld'
    LIBRARY_EXT = '.a'

    STD_LIB_NAME = "lib%s.a"
    DIAGNOSTIC_PATTERN = re.compile('((?P<file>[^:]+):(?P<line>\d+):)(?P<col>\d+):? (?P<severity>warning|[eE]rror|fatal error): (?P<message>.+)')

    GCC_RANGE = (LooseVersion("9.0.0"), LooseVersion("10.0.0"))
    GCC_VERSION_RE = re.compile(b"\d+\.\d+\.\d+")
    DWARF_PRODUCER_RE = re.compile(r'(DW_AT_producer)(.*:\s*)(?P<producer>.*)')

    def __init__(self, target,  notify=None, macros=None, build_profile=None,
                 build_dir=None, coverage_patterns=None):
        mbedToolchain.__init__(
            self,
            target,
            notify,
            macros,
            build_profile=build_profile,
            build_dir=build_dir,
            coverage_patterns=coverage_patterns
        )

        tool_path = TOOLCHAIN_PATHS['GCC_ARM']
        # Add flags for current size setting
        c_lib = "std"
        if hasattr(target, "c_lib"):
            toolchain = "gcc_arm"

            if should_replace_small_c_lib(target, toolchain):
                target.c_lib = "std"

            self.check_c_lib_supported(target, toolchain)
            c_lib = target.c_lib
        elif hasattr(target, "default_build"):
            c_lib = target.default_build

        if c_lib == "small":
            common_flags = ["-DMBED_RTOS_SINGLE_THREAD", "-D__NEWLIB_NANO"]
            self.flags["common"].extend(common_flags)
            self.flags["ld"].append("--specs=nano.specs")

        self.check_and_add_minimal_printf(target)

        if getattr(target, "printf_lib", "std") == "minimal-printf":
            minimal_printf_wraps = [
                "-Wl,--wrap,printf",
                "-Wl,--wrap,sprintf",
                "-Wl,--wrap,snprintf",
                "-Wl,--wrap,vprintf",
                "-Wl,--wrap,vsprintf",
                "-Wl,--wrap,vsnprintf",
                "-Wl,--wrap,fprintf",
                "-Wl,--wrap,vfprintf",
            ]

            # Add the linker option to wrap the f\v\s\printf functions if not
            # already added.
            for minimal_printf_wrap in minimal_printf_wraps:
                if minimal_printf_wrap not in self.flags["ld"]:
                    self.flags["ld"].append(minimal_printf_wrap)

        self.cpu = []
        if target.is_TrustZone_secure_target:
            # Enable compiler security extensions
            self.cpu.append("-mcmse")
            # Output secure import library
            self.flags["ld"].extend([
                "-Wl,--cmse-implib",
                "-Wl,--out-implib=%s" % join(build_dir, "cmse_lib.o")
            ])

        if target.is_TrustZone_non_secure_target:
            # Add linking time preprocessor macro DOMAIN_NS
            # (DOMAIN_NS is passed to compiler and assembler via CORTEX_SYMBOLS
            # in mbedToolchain.get_symbols)
            self.flags["ld"].append("-DDOMAIN_NS=1")

        core = target.core_without_NS
        cpu = {
            "Cortex-M0+": "cortex-m0plus",
            "Cortex-M4F": "cortex-m4",
            "Cortex-M7F": "cortex-m7",
            "Cortex-M7FD": "cortex-m7",
            "Cortex-M33": "cortex-m33+nodsp",
            "Cortex-M33E": "cortex-m33",
            "Cortex-M33F": "cortex-m33+nodsp",
            "Cortex-M33FE": "cortex-m33"}.get(core, core)

        if cpu == "cortex-m33+nodsp":
            self.cpu.append("-march=armv8-m.main")
        elif cpu == "cortex-m33":
            self.cpu.append("-march=armv8-m.main+dsp")
        else:
            self.cpu.append("-mcpu={}".format(cpu.lower()))

        if target.core.startswith("Cortex-M"):
            self.cpu.append("-mthumb")

        # FPU handling, M7 possibly to have double FPU
        if core == "Cortex-M4F":
            self.cpu.append("-mfpu=fpv4-sp-d16")
            self.cpu.append("-mfloat-abi=softfp")
        elif core == "Cortex-M7F" or core.startswith("Cortex-M33F"):
            self.cpu.append("-mfpu=fpv5-sp-d16")
            self.cpu.append("-mfloat-abi=softfp")
        elif core == "Cortex-M7FD":
            self.cpu.append("-mfpu=fpv5-d16")
            self.cpu.append("-mfloat-abi=softfp")

        if target.core == "Cortex-A9":
            self.cpu.append("-mthumb-interwork")
            self.cpu.append("-marm")
            self.cpu.append("-march=armv7-a")
            self.cpu.append("-mfpu=vfpv3")
            self.cpu.append("-mfloat-abi=hard")
            self.cpu.append("-mno-unaligned-access")

        self.flags["common"] += self.cpu
        self.coverage_supported = True
        main_cc = join(tool_path, "arm-none-eabi-gcc")
        main_cppc = join(tool_path, "arm-none-eabi-g++")
        self.asm = [main_cc] + self.flags['asm'] + self.flags["common"]
        self.cc = [main_cc]
        self.cppc = [main_cppc]
        self.cc += self.flags['c'] + self.flags['common']
        self.cppc += self.flags['cxx'] + self.flags['common']

        self.flags['ld'] += self.cpu
        self.ld = [join(tool_path, "arm-none-eabi-gcc")]
        self.ld += self.flags['ld'] + self.flags['common']
        self.sys_libs = ["stdc++", "supc++", "m", "c", "gcc", "nosys"]
        self.preproc = [join(tool_path, "arm-none-eabi-cpp"), "-E", "-P"]

        self.ar = join(tool_path, "arm-none-eabi-ar")
        self.elf2bin = join(tool_path, "arm-none-eabi-objcopy")
        self.objdump = join(tool_path, "arm-none-eabi-objdump")

        self.use_distcc = (bool(getenv("DISTCC_POTENTIAL_HOSTS", False))
                           and not getenv("MBED_DISABLE_DISTCC", False))

        # create copies of gcc/ld options as coverage build options, and injects extra coverage options 
        self.coverage_cc = self.cc + ["--coverage", "-DENABLE_LIBGCOV_PORT"]
        self.coverage_cppc = self.cppc + ["--coverage", "-DENABLE_LIBGCOV_PORT"]
        self.coverage_ld = self.ld + ['--coverage', '-Wl,--wrap,GREENTEA_SETUP', '-Wl,--wrap,_Z25GREENTEA_TESTSUITE_RESULTi']

        # for gcc coverage options remove MBED_DEBUG macro (this is required by code coverage function)
        for flag in ["-DMBED_DEBUG"]:
            if flag in self.coverage_cc:
                self.coverage_cc.remove(flag)
            if flag in self.coverage_cppc:
                self.coverage_cppc.remove(flag)
        #  for lg coverage options remove exit wrapper (this is required by code coverage function)
        for flag in ['-Wl,--wrap,exit', '-Wl,--wrap,atexit']:
            if flag in self.coverage_ld:
                self.coverage_ld.remove(flag)

    def version_check(self):
        stdout, _, retcode = run_cmd([self.cc[0], "--version"], redirect=True)
        msg = None
        match = self.GCC_VERSION_RE.search(stdout.encode("utf-8"))
        if match:
            found_version = LooseVersion(match.group(0).decode('utf-8'))
        else:
            found_version = None
        min_ver, max_ver = self.GCC_RANGE
        if found_version and (found_version < min_ver
                              or found_version >= max_ver):
            msg = ("Compiler version mismatch: Have {}; "
                   "expected version >= {} and < {}"
                   .format(found_version, min_ver, max_ver))
        elif not match:
            msg = ("Compiler version mismatch: Could not detect version; "
                   "expected version >= {} and < {}"
                   .format(min_ver, max_ver))
        if msg:
            self.notify.cc_info({
                "message": msg,
                "file": "",
                "line": "",
                "col": "",
                "severity": "Warning",
            })

    def is_not_supported_error(self, output):
        return "error: #error [NOT_SUPPORTED]" in output

    def parse_output(self, output):
        # The warning/error notification is multiline
        msg = None
        for line in output.splitlines():
            match = self.DIAGNOSTIC_PATTERN.search(line)
            if match is not None:
                if msg is not None:
                    self.notify.cc_info(msg)
                    msg = None
                msg = {
                    'severity': match.group('severity').lower(),
                    'file': match.group('file'),
                    'line': match.group('line'),
                    'col': match.group('col'),
                    'message': match.group('message'),
                    'text': '',
                    'target_name': self.target.name,
                    'toolchain_name': self.name
                }

        if msg is not None:
            self.notify.cc_info(msg)

    def get_dep_option(self, object):
        base, _ = splitext(object)
        dep_path = base + '.d'
        return ["-MD", "-MF", dep_path]

    def get_config_option(self, config_header):
        return ['-include', config_header]

    def get_compile_options(self, defines, includes, for_asm=False):
        opts = ['-D%s' % d for d in defines]
        if self.RESPONSE_FILES:
            opts += ['@%s' % self.get_inc_file(includes)]
        else:
            opts += ["-I%s" % i for i in includes]

        config_header = self.get_config_header()
        if config_header is not None:
            opts = opts + self.get_config_option(config_header)
        return opts

    def match_coverage_patterns(self, source):
        """Check whether the give source file match with coverage patterns, if so return True. """
        for pattern in self.coverage_patterns:
            if fnmatch.fnmatch(source, pattern):
                return True
        return False

    def assemble(self, source, object, includes):
        # Build assemble command
        cmd = self.asm + self.get_compile_options(
            self.get_symbols(True), includes
        ) + ["-o", object, source]

        # Return command array, don't execute
        return [cmd]

    def compile(self, cc, source, object, includes):
        # Build compile command
        cmd = cc + self.get_compile_options(self.get_symbols(), includes)

        cmd.extend(self.get_dep_option(object))

        cmd.extend(["-o", object, source])

        if self.use_distcc:
            cmd = ["distcc"] + cmd

        return [cmd]

    def compile_c(self, source, object, includes):
        if self.coverage_patterns and self.match_coverage_patterns(source):
            return self.compile(self.coverage_cc, source, object, includes)
        return self.compile(self.cc, source, object, includes)

    def compile_cpp(self, source, object, includes):
        if self.coverage_patterns and self.match_coverage_patterns(source):
             return self.compile(self.coverage_cppc, source, object, includes)
        return self.compile(self.cppc, source, object, includes)

    def link(self, output, objects, libraries, lib_dirs, mem_map):
        libs = []
        for l in libraries:
            name, _ = splitext(basename(l))
            libs.append("-l%s" % name[3:])
        libs.extend(["-l%s" % l for l in self.sys_libs])

        # Preprocess
        if mem_map:
            preproc_output = join(dirname(output), ".link_script.ld")
            cmd = (
                self.preproc + [mem_map] + self.ld[1:] + ["-o", preproc_output] +
                self.get_compile_options(self.get_symbols(), [])
            )
            self.notify.cc_verbose("Preproc: %s" % ' '.join(cmd))
            self.default_cmd(cmd)
            mem_map = preproc_output

        # NOTE: GCC_ARM_LTO_WORKAROUND
        # This is a workaround for the GCC not using the strong symbols from
        # C files to override the weak symbols from ASM files. This GCC bug is only
        # present when building with the link-time optimizer (LTO) enabled. For
        # more details please see:
        #   https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83967
        #
        # This can be fixed by changing the order of object files in the linker
        # command; objects providing the weak symbols and compiled from assembly
        # must be listed before the objects providing the strong symbols.
        # To keep things simple, ALL object files from ASM are listed before
        # other object files.
        asm_objects = []
        if '-flto' in self.ld:
            asm_objects = self.get_asm_objects(objects)
        reorg_objects = (
            [o for o in objects if o in asm_objects] +
            [o for o in objects if o not in asm_objects]
        )
        # Build linker command
        map_file = splitext(output)[0] + ".map"
        cmd = (
            (self.coverage_ld if self.coverage_patterns else self.ld) +
            ["-o", output, "-Wl,-Map=%s" % map_file] +
            reorg_objects +
            ["-Wl,--start-group"] +
            libs +
            ["-Wl,--end-group"]
        )

        if mem_map:
            cmd.extend(['-T', mem_map])

        for L in lib_dirs:
            cmd.extend(['-L', L])
        cmd.extend(libs)

        if self.RESPONSE_FILES:
            # Split link command to linker executable + response file
            cmd_linker = cmd[0]
            link_files = self.get_link_file(cmd[1:])
            cmd = [cmd_linker, "@%s" % link_files]

        # Exec command
        self.notify.cc_verbose("Link: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    def archive(self, objects, lib_path):
        if self.RESPONSE_FILES:
            param = ["@%s" % self.get_arch_file(objects)]
        else:
            param = objects

        # Exec command
        self.default_cmd([self.ar, 'rcs', lib_path] + param)

    def binary(self, resources, elf, bin):
        # Build binary command
        _, fmt = splitext(bin)
        bin_arg = {'.bin': 'binary', '.hex': 'ihex'}[fmt]
        cmd = [self.elf2bin, "-O", bin_arg, elf, bin]

        # Exec command
        self.notify.cc_verbose("FromELF: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    @staticmethod
    def name_mangle(name):
        return "_Z%i%sv" % (len(name), name)

    @staticmethod
    def make_ld_define(name, value):
        return "-D%s=%s" % (name, value)

    @staticmethod
    def redirect_symbol(source, sync, build_dir):
        return "-Wl,--defsym=%s=%s" % (source, sync)

    @staticmethod
    def check_executable():
        """Returns True if the executable (arm-none-eabi-gcc) location
        specified by the user exists OR the executable can be found on the
        PATH. Returns False otherwise."""
        if (
            not TOOLCHAIN_PATHS['GCC_ARM'] or
            not exists(TOOLCHAIN_PATHS['GCC_ARM'])
        ):
            if find_executable('arm-none-eabi-gcc'):
                TOOLCHAIN_PATHS['GCC_ARM'] = ''
                return True
            else:
                return False
        else:
            exec_name = join(TOOLCHAIN_PATHS['GCC_ARM'], 'arm-none-eabi-gcc')
            return exists(exec_name) or exists(exec_name + '.exe')

    def check_if_obj_from_asm(self, obj_file):
        """Check if obj_file was build by the GNU Assembler."""
        dw_producer = ''
        cmd = [self.objdump, '--dwarf=info', obj_file]
        stdout, stderr, rc = run_cmd(cmd, work_dir=getcwd(), chroot=self.CHROOT)
        if rc != 0:
            return False
        match = self.DWARF_PRODUCER_RE.search(stdout)
        if match:
            dw_producer = match.group('producer')
        return 'GNU AS' in dw_producer

    def get_asm_objects(self, objects):
        """Return a list of object files built from ASM."""
        return [o for o in objects if self.check_if_obj_from_asm(o)]

class GCC_ARM(GCC):
    pass
