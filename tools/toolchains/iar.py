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
from os import remove
from os.path import join, splitext, exists
from distutils.version import LooseVersion

from tools.toolchains.mbed_toolchain import (
    mbedToolchain, TOOLCHAIN_PATHS, should_replace_small_c_lib
)
from tools.utils import run_cmd

class IAR(mbedToolchain):
    OFFICIALLY_SUPPORTED = True
    LIBRARY_EXT = '.a'
    LINKER_EXT = '.icf'
    STD_LIB_NAME = "%s.a"

    DIAGNOSTIC_PATTERN = re.compile('"(?P<file>[^"]+)",(?P<line>[\d]+)\s+(?P<severity>Warning|Error|Fatal error)(?P<message>.+)')
    INDEX_PATTERN = re.compile('(?P<col>\s*)\^')
    IAR_VERSION_RE = re.compile(b"IAR ANSI C/C\+\+ Compiler V(\d+\.\d+)")
    IAR_VERSION = LooseVersion("8.32")

    @staticmethod
    def check_executable():
        """Returns True if the executable (arm-none-eabi-gcc) location
        specified by the user exists OR the executable can be found on the
        PATH. Returns False otherwise."""
        return mbedToolchain.generic_check_executable(
            "IAR", 'iccarm', 2, "bin"
        )

    def __init__(self, target, notify=None, macros=None, build_profile=None,
                 build_dir=None, coverage_patterns=None):
        mbedToolchain.__init__(
            self,
            target,
            notify,
            macros,
            build_dir=build_dir,
            build_profile=build_profile
        )

        toolchain = "iar"

        if should_replace_small_c_lib(target, toolchain):
            target.c_lib = "std"

        self.check_c_lib_supported(target, "iar")

        if target.is_TrustZone_secure_target:
            # Enable compiler security extensions
            self.flags["asm"] += ["--cmse"]
            self.flags["common"] += ["--cmse"]
            # Output secure import library
            secure_file = join(build_dir, "cmse_lib.o")
            self.flags["ld"] += ["--import_cmse_lib_out=%s" % secure_file]

        if target.is_TrustZone_non_secure_target:
            # Add linking time preprocessor macro DOMAIN_NS
            # (DOMAIN_NS is passed to compiler and assembler via CORTEX_SYMBOLS
            # in mbedToolchain.get_symbols)
            define_string = self.make_ld_define("DOMAIN_NS", "0x1")
            self.flags["ld"].append(define_string)

        self.check_and_add_minimal_printf(target)

        core = target.core_without_NS
        cpu = {
            "Cortex-M7F": "Cortex-M7.fp.sp",
            "Cortex-M7FD": "Cortex-M7.fp.dp",
            "Cortex-M33": "Cortex-M33.no_dsp",
            "Cortex-M33E": "Cortex-M33",
            "Cortex-M33F": "Cortex-M33.fp.no_dsp",
            "Cortex-M33FE": "Cortex-M33.fp"}.get(core, core)

        # flags_cmd are used only by our scripts, the project files have them
        # already defined, using this flags results in the errors (duplication)
        # asm accepts --cpu Core or --fpu FPU, not like c/c++ --cpu=Core
        asm_flags_cmd = ["--cpu", cpu]
        # custom c flags
        c_flags_cmd = ["--cpu", cpu]

        c_flags_cmd.extend([
            "--thumb", "--dlib_config", "DLib_Config_Full.h"
        ])
        # custom c++ cmd flags
        cxx_flags_cmd = [
            "--c++", "--no_rtti", "--no_exceptions"
        ]

        IAR_BIN = join(TOOLCHAIN_PATHS['IAR'], "bin")
        main_cc = join(IAR_BIN, "iccarm")

        self.asm = [join(IAR_BIN, "iasmarm")]
        self.asm += asm_flags_cmd
        self.asm += self.flags["asm"]

        self.cc = [main_cc]
        self.cc += self.flags["common"]
        self.cc += c_flags_cmd
        self.cc += self.flags["c"]

        self.cppc = [main_cc]
        self.cppc += self.flags["common"]
        self.cppc += c_flags_cmd
        self.cppc += cxx_flags_cmd
        self.cppc += self.flags["cxx"]

        self.ld = [join(IAR_BIN, "ilinkarm")] + self.flags['ld']
        self.ar = join(IAR_BIN, "iarchive")
        self.elf2bin = join(IAR_BIN, "ielftool")

    def version_check(self):
        stdout, _, retcode = run_cmd([self.cc[0], "--version"], redirect=True)
        msg = None
        match = self.IAR_VERSION_RE.search(stdout.encode("utf-8"))
        found_version = match.group(1).decode("utf-8") if match else None
        if found_version and LooseVersion(found_version) != self.IAR_VERSION:
            msg = "Compiler version mismatch: Have {}; expected {}".format(
                found_version, self.IAR_VERSION)
        elif not match or len(match.groups()) != 1:
            msg = ("Compiler version mismatch: Could Not detect compiler "
                   "version; expected {}".format(self.IAR_VERSION))
        if msg:
            self.notify.cc_info({
                "message": msg,
                "file": "",
                "line": "",
                "col": "",
                "severity": "Warning",
            })

    def _inner_parse_deps(self, dep_path):
        for path in open(dep_path).readlines():
            if path and not path.isspace():
                if self.CHROOT:
                    yield self.CHROOT + path.strip()
                else:
                    yield path.strip()

    def parse_dependencies(self, dep_path):
        return list(self._inner_parse_deps(dep_path))

    def parse_output(self, output):
        msg = None
        for line in output.splitlines():
            match = IAR.DIAGNOSTIC_PATTERN.match(line)
            if match is not None:
                if msg is not None:
                    self.notify.cc_info(msg)
                    msg = None
                msg = {
                    'severity': match.group('severity').lower(),
                    'file': match.group('file'),
                    'line': match.group('line'),
                    'col': 0,
                    'message': match.group('message'),
                    'text': '',
                    'target_name': self.target.name,
                    'toolchain_name': self.name
                }
            elif msg is not None:
                # Determine the warning/error column by calculating the '^'
                # position
                match = IAR.INDEX_PATTERN.match(line)
                if match is not None:
                    msg['col'] = len(match.group('col'))
                    self.notify.cc_info(msg)
                    msg = None
                else:
                    msg['text'] += line+"\n"

        if msg is not None:
            self.notify.cc_info(msg)

    def get_dep_option(self, object):
        base, _ = splitext(object)
        dep_path = base + '.d'
        return ["--dependencies", dep_path]

    def cc_extra(self, object):
        base, _ = splitext(object)
        return ["-l", base + '.s.txt']

    def get_config_option(self, config_header):
        return ['--preinclude=' + config_header]

    def get_compile_options(self, defines, includes, for_asm=False):
        opts = ['-D%s' % d for d in defines]
        if for_asm:
            config_macros = self.config.get_config_data_macros()
            macros_cmd = ['"-D%s"' % d for d in config_macros if '"' not in d]
            if self.RESPONSE_FILES:
                via_file = self.make_option_file(
                    macros_cmd, "asm_macros_{}.xcl")
                opts += ['-f', via_file]
            else:
                opts += macros_cmd
            return opts
        else:
            if self.RESPONSE_FILES:
                opts += ['-f', self.get_inc_file(includes)]
            else:
                opts += ["-I%s" % i for i in includes]
            config_header = self.get_config_header()
            if config_header is not None:
                opts = opts + self.get_config_option(config_header)

            return opts

    def assemble(self, source, object, includes):
        # Build assemble command
        cmd = self.asm + self.get_compile_options(
            self.get_symbols(True), includes, True
        ) + ["-o", object, source]

        # Return command array, don't execute
        return [cmd]

    def compile(self, cc, source, object, includes):
        # Build compile command
        cmd = cc + self.get_compile_options(self.get_symbols(), includes)
        cmd.extend(self.get_dep_option(object))
        cmd.extend(self.cc_extra(object))
        cmd.extend(["-o", object, source])
        return [cmd]

    def compile_c(self, source, object, includes):
        return self.compile(self.cc, source, object, includes)

    def compile_cpp(self, source, object, includes):
        return self.compile(self.cppc, source, object, includes)

    def link(self, output, objects, libraries, lib_dirs, mem_map):
        # Build linker command
        map_file = splitext(output)[0] + ".map"
        cmd = self.ld + ["-o", output, "--map=%s" % map_file]
        cmd += objects
        cmd += libraries

        if mem_map:
            cmd.extend(["--config", mem_map])

        if self.RESPONSE_FILES:
            # Split link command to linker executable + response file
            cmd_linker = cmd[0]
            link_files = self.get_link_file(cmd[1:])
            cmd = [cmd_linker, '-f', link_files]

        # Exec command
        self.notify.cc_verbose("Link: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    def archive(self, objects, lib_path):
        if self.RESPONSE_FILES:
            param = ['-f', self.get_arch_file(objects)]
        else:
            param = objects

        if exists(lib_path):
            remove(lib_path)

        self.default_cmd([self.ar, lib_path] + param)

    def binary(self, resources, elf, bin):
        _, fmt = splitext(bin)
        bin_arg = {".bin": "--bin", ".hex": "--ihex"}[fmt]
        # Build binary command
        cmd = [self.elf2bin, bin_arg, elf, bin]

        # Exec command
        self.notify.cc_verbose("FromELF: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    @staticmethod
    def name_mangle(name):
        return "_Z%i%sv" % (len(name), name)

    @staticmethod
    def make_ld_define(name, value):
        return "--config_def %s=%s" % (name, value)

    @staticmethod
    def redirect_symbol(source, sync, build_dir):
        return "--redirect %s=%s" % (source, sync)
