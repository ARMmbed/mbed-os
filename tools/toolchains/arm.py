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
from copy import copy
from os.path import join, dirname, splitext

from tools.settings import ARMC6_PATH

from tools.toolchains import mbedToolchain, TOOLCHAIN_PATHS
from tools.hooks import hook_tool
from tools.utils import mkdir

class ARM(mbedToolchain):
    LINKER_EXT = '.sct'
    LIBRARY_EXT = '.ar'

    STD_LIB_NAME = "%s.ar"

    # ANY changes to these default flags is backwards incompatible and require
    # an update to the mbed-sdk-tools and website that introduces a profile
    # for the previous version of these flags
    DEFAULT_FLAGS = {
        'common': ["-c", "--target=arm-arm-none-eabi", "-mthumb", "-g"],
        'asm': [],
        'c': ["-D__ASSERT_MSG"],
        'cxx': [],
        'ld': [],
    }

    @staticmethod
    def check_executable():
        """Returns True if the executable (armclang) location specified by the
         user exists OR the executable can be found on the PATH.
         Returns False otherwise."""
        return mbedToolchain.generic_check_executable("ARM", "armclang", 1)

    def __init__(self, target, *args, **kwargs):
        mbedToolchain.__init__(self, target, *args, **kwargs)

        if target.core.lower()[-1] == 'f':
            self.flags['common'].append("-mcpu=%s" % target.core.lower()[:-1])
            self.flags['ld'].append("--cpu=%s" % target.core.lower()[:-1])
        else:
            self.flags['common'].append("-mcpu=%s" % target.core.lower())
            self.flags['ld'].append("--cpu=%s" % target.core.lower())

        if target.core == "Cortex-M4F":
            self.flags['common'].append("-mfpu=fpv4-sp-d16")
            self.flags['common'].append("-mfloat-abi=hard")
        elif target.core == "Cortex-M7F":
            self.flags['common'].append("-mfpu=fpv5-sp-d16")
            self.flags['common'].append("-mfloat-abi=softfp")
        elif target.core == "Cortex-M7FD":
            self.flags['common'].append("-mfpu=fpv5-d16")
            self.flags['common'].append("-mfloat-abi=softfp")

        asm_cpu = {
            "Cortex-M0+": "Cortex-M0",
            "Cortex-M4F": "Cortex-M4.fp",
            "Cortex-M7F": "Cortex-M7.fp.sp",
            "Cortex-M7DF": "Cortex-M7.fp.dp"}.get(target.core, target.core)

        self.flags['asm'].append("--cpu=%s" % asm_cpu)

        self.cc = ([join(ARMC6_PATH, "armclang")] + self.flags['common'] +
                   self.flags['c'])
        self.cppc = ([join(ARMC6_PATH, "armclang")] + self.flags['common'] +
                     self.flags['cxx'])
        self.asm = [join(ARMC6_PATH, "armasm")] + self.flags['asm']
        self.ld = [join(ARMC6_PATH, "armlink")] + self.flags['ld']
        self.ar = [join(ARMC6_PATH, "armar")]
        self.elf2bin = join(ARMC6_PATH, "fromelf")


    def parse_dependencies(self, dep_path):
        return []

    def parse_output(self, output):
        pass

    def get_config_option(self, config_header):
        return ["-include", config_header]

    def get_compile_options(self, defines, includes, for_asm=False):
        opts = ['-D%s' % d for d in defines]
        opts.extend(["-I%s" % i for i in includes])
        if for_asm:
            return ["--cpreproc",
                    "--cpreproc_opts=%s" % ",".join(self.flags['common'] + opts)]
        else:
            config_header = self.get_config_header()
            if config_header:
                opts.extend(self.get_config_option(config_header))
            return opts

    @hook_tool
    def assemble(self, source, object, includes):
        cmd_pre = copy(self.asm)
        cmd_pre.extend(self.get_compile_options(
            self.get_symbols(True), includes, for_asm=True))
        cmd_pre.extend(["-o", object, source])
        return [self.hook.get_cmdline_assembler(cmd_pre)]

    @hook_tool
    def compile(self, cc, source, object, includes):
        cmd = copy(cc)
        cmd.extend(self.get_compile_options(self.get_symbols(), includes))
        cmd.extend(["-o", object, source])
        cmd = self.hook.get_cmdline_compiler(cmd)
        return [cmd]

    def compile_c(self, source, object, includes):
        return self.compile(self.cc, source, object, includes)

    def compile_cpp(self, source, object, includes):
        return self.compile(self.cppc, source, object, includes)

    @hook_tool
    def link(self, output, objects, libraries, lib_dirs, scatter_file):
        base, _ = splitext(output)
        map_file = base + ".map"
        args = ["-o", output, "--info=totals", "--map", "--list=%s" % map_file]
        args.extend(objects)
        args.extend(libraries)
        if lib_dirs:
            args.append("--userlibpath", ",".join(lib_dirs))
        if scatter_file:
            args.extend(["--scatter", scatter_file])

        cmd_pre = self.ld + args
        cmd = self.hook.get_cmdline_linker(cmd_pre)

        if self.RESPONSE_FILES:
            cmd_linker = cmd[0]
            link_files = self.get_link_file(cmd[1:])
            cmd = [cmd_linker, '--via', link_files]

        self.cc_verbose("Link: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    @hook_tool
    def archive(self, objects, lib_path):
        if self.RESPONSE_FILES:
            param = ['--via', self.get_arch_file(objects)]
        else:
            param = objects
        self.default_cmd([self.ar, '-r', lib_path] + param)

    @hook_tool
    def binary(self, resources, elf, bin):
        _, fmt = splitext(bin)
        bin_arg = {".bin": "--bin", ".hex": "--i32"}[fmt]
        cmd_pre = [self.elf2bin, bin_arg, '-o', bin, elf]
        cmd = self.hook.get_cmdline_binary(cmd_pre)
        self.cc_verbose("FromELF: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

class ARM_STD(ARM):
    pass
