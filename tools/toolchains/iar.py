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
import re
from os import remove
from os.path import join, splitext, exists

from tools.toolchains import mbedToolchain, TOOLCHAIN_PATHS
from tools.hooks import hook_tool

class IAR(mbedToolchain):
    LIBRARY_EXT = '.a'
    LINKER_EXT = '.icf'
    STD_LIB_NAME = "%s.a"

    DIAGNOSTIC_PATTERN = re.compile('"(?P<file>[^"]+)",(?P<line>[\d]+)\s+(?P<severity>Warning|Error|Fatal error)(?P<message>.+)')
    INDEX_PATTERN  = re.compile('(?P<col>\s*)\^')

    @staticmethod
    def check_executable():
        """Returns True if the executable (arm-none-eabi-gcc) location
        specified by the user exists OR the executable can be found on the PATH.
        Returns False otherwise."""
        return mbedToolchain.generic_check_executable("IAR", 'iccarm', 2, "bin")

    def __init__(self, target, notify=None, macros=None,
                 silent=False, extra_verbose=False, build_profile=None,
                 build_dir=None):
        mbedToolchain.__init__(self, target, notify, macros, silent,
                               build_dir=build_dir,
                               extra_verbose=extra_verbose,
                               build_profile=build_profile)
        if target.core == "Cortex-M7F" or target.core == "Cortex-M7FD":
            cpuchoice = "Cortex-M7"
        elif target.core.startswith("Cortex-M23"):
            cpuchoice = "8-M.baseline"
        elif target.core.startswith("Cortex-M33"):
            cpuchoice = "8-M.mainline"
        else:
            cpuchoice = target.core

        # flags_cmd are used only by our scripts, the project files have them already defined,
        # using this flags results in the errors (duplication)
        # asm accepts --cpu Core or --fpu FPU, not like c/c++ --cpu=Core
        asm_flags_cmd = ["--cpu", cpuchoice]
        # custom c flags
        c_flags_cmd = ["--cpu", cpuchoice]

        c_flags_cmd.extend([
            "--thumb", "--dlib_config", "DLib_Config_Full.h"
        ])
        # custom c++ cmd flags
        cxx_flags_cmd = [
            "--c++", "--no_rtti", "--no_exceptions"
        ]
        if target.core == "Cortex-M7FD":
            asm_flags_cmd += ["--fpu", "VFPv5"]
            c_flags_cmd.append("--fpu=VFPv5")
        elif target.core == "Cortex-M7F":
            asm_flags_cmd += ["--fpu", "VFPv5_sp"]
            c_flags_cmd.append("--fpu=VFPv5_sp")
        elif target.core == "Cortex-M23" or target.core == "Cortex-M33":
            self.flags["asm"] += ["--cmse"]

        IAR_BIN = join(TOOLCHAIN_PATHS['IAR'], "bin")
        main_cc = join(IAR_BIN, "iccarm")

        self.asm  = [join(IAR_BIN, "iasmarm")] + asm_flags_cmd + self.flags["asm"]
        self.cc   = [main_cc]
        self.cppc = [main_cc]
        self.cc += self.flags["common"] + c_flags_cmd + self.flags["c"]
        self.cppc += self.flags["common"] + c_flags_cmd + cxx_flags_cmd + self.flags["cxx"]
        
        self.ld   = [join(IAR_BIN, "ilinkarm")] + self.flags['ld']
        self.ar = join(IAR_BIN, "iarchive")
        self.elf2bin = join(IAR_BIN, "ielftool")

    def parse_dependencies(self, dep_path):
        return [(self.CHROOT if self.CHROOT else '')+path.strip() for path in open(dep_path).readlines()
                if (path and not path.isspace())]

    def parse_output(self, output):
        msg = None
        for line in output.splitlines():
            match = IAR.DIAGNOSTIC_PATTERN.match(line)
            if match is not None:
                if msg is not None:
                    self.cc_info(msg)
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
                # Determine the warning/error column by calculating the ^ position
                match = IAR.INDEX_PATTERN.match(line)
                if match is not None:
                    msg['col'] = len(match.group('col'))
                    self.cc_info(msg)
                    msg = None
                else:
                    msg['text'] += line+"\n"

        if msg is not None:
            self.cc_info(msg)

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
        if for_asm :
            return opts
        if self.RESPONSE_FILES:
            opts += ['-f', self.get_inc_file(includes)]
        else:
            opts += ["-I%s" % i for i in includes]

        config_header = self.get_config_header()
        if config_header is not None:
            opts = opts + self.get_config_option(config_header)
        return opts

    @hook_tool
    def assemble(self, source, object, includes):
        # Build assemble command
        cmd = self.asm + self.get_compile_options(self.get_symbols(True), includes, True) + ["-o", object, source]

        # Call cmdline hook
        cmd = self.hook.get_cmdline_assembler(cmd)

        # Return command array, don't execute
        return [cmd]

    @hook_tool
    def compile(self, cc, source, object, includes):
        # Build compile command
        cmd = cc +  self.get_compile_options(self.get_symbols(), includes)

        cmd.extend(self.get_dep_option(object))

        cmd.extend(self.cc_extra(object))
        
        cmd.extend(["-o", object, source])

        # Call cmdline hook
        cmd = self.hook.get_cmdline_compiler(cmd)

        return [cmd]

    def compile_c(self, source, object, includes):
        return self.compile(self.cc, source, object, includes)

    def compile_cpp(self, source, object, includes):
        return self.compile(self.cppc, source, object, includes)

    @hook_tool
    def link(self, output, objects, libraries, lib_dirs, mem_map):
        # Build linker command
        map_file = splitext(output)[0] + ".map"
        cmd = self.ld + [ "-o", output, "--map=%s" % map_file] + objects + libraries

        if mem_map:
            cmd.extend(["--config", mem_map])

        # Call cmdline hook
        cmd = self.hook.get_cmdline_linker(cmd)

        if self.RESPONSE_FILES:
            # Split link command to linker executable + response file
            cmd_linker = cmd[0]
            link_files = self.get_link_file(cmd[1:])
            cmd = [cmd_linker, '-f', link_files]

        # Exec command
        self.cc_verbose("Link: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    @hook_tool
    def archive(self, objects, lib_path):
        if self.RESPONSE_FILES:
            param = ['-f', self.get_arch_file(objects)]
        else:
            param = objects

        if exists(lib_path):
            remove(lib_path)

        self.default_cmd([self.ar, lib_path] + param)

    @hook_tool
    def binary(self, resources, elf, bin):
        _, fmt = splitext(bin)
        bin_arg = {".bin": "--bin", ".hex": "--ihex"}[fmt]
        # Build binary command
        cmd = [self.elf2bin, bin_arg, elf, bin]

        # Call cmdline hook
        cmd = self.hook.get_cmdline_binary(cmd)

        # Exec command
        self.cc_verbose("FromELF: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    @staticmethod
    def name_mangle(name):
        return "_Z%i%sv" % (len(name), name)

    @staticmethod
    def make_ld_define(name, value):
        return "--config_def %s=0x%x" % (name, value)

    @staticmethod
    def redirect_symbol(source, sync, build_dir):
        return "--redirect %s=%s" % (source, sync)
