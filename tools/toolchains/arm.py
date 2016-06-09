"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" tools.,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
import re
from os.path import join
import copy

from tools.toolchains import mbedToolchain
from tools.settings import ARM_BIN, ARM_INC, ARM_LIB, MY_ARM_CLIB, ARM_CPPLIB
from tools.hooks import hook_tool
from tools.settings import GOANNA_PATH

class ARM(mbedToolchain):
    LINKER_EXT = '.sct'
    LIBRARY_EXT = '.ar'

    STD_LIB_NAME = "%s.ar"
    DIAGNOSTIC_PATTERN  = re.compile('"(?P<file>[^"]+)", line (?P<line>\d+)( \(column (?P<column>\d+)\)|): (?P<severity>Warning|Error): (?P<message>.+)')
    DEP_PATTERN = re.compile('\S+:\s(?P<file>.+)\n')

    DEFAULT_FLAGS = {
        'common': ["--apcs=interwork",
            "--brief_diagnostics"],
        'asm': ['-I"%s"' % ARM_INC],
        'c': ["-c", "--gnu", "-Otime", "--restrict", "--multibyte_chars", "--split_sections", "--md", "--no_depend_system_headers", '-I"%s"' % ARM_INC,
            "--c99", "-D__ASSERT_MSG" ],
        'cxx': ["--cpp", "--no_rtti", "-D__ASSERT_MSG"],
        'ld': [],
    }

    def __init__(self, target, options=None, notify=None, macros=None, silent=False, extra_verbose=False):
        mbedToolchain.__init__(self, target, options, notify, macros, silent, extra_verbose=extra_verbose)

        if target.core == "Cortex-M0+":
            cpu = "Cortex-M0"
        elif target.core == "Cortex-M4F":
            cpu = "Cortex-M4.fp"
        elif target.core == "Cortex-M7F":
            cpu = "Cortex-M7.fp.sp"
        else:
            cpu = target.core

        main_cc = join(ARM_BIN, "armcc")

        self.flags = copy.deepcopy(self.DEFAULT_FLAGS)
        self.flags['common'] += ["--cpu=%s" % cpu]
        if "save-asm" in self.options:
            self.flags['common'].extend(["--asm", "--interleave"])

        if "debug-info" in self.options:
            self.flags['common'].append("-g")
            self.flags['c'].append("-O0")
        else:
            self.flags['c'].append("-O3")

        self.asm = [main_cc] + self.flags['common'] + self.flags['asm'] + self.flags['c']
        if not "analyze" in self.options:
            self.cc = [main_cc] + self.flags['common'] + self.flags['c']
            self.cppc = [main_cc] + self.flags['common'] + self.flags['c'] + self.flags['cxx']
        else:
            self.cc  = [join(GOANNA_PATH, "goannacc"), "--with-cc=" + main_cc.replace('\\', '/'), "--dialect=armcc", '--output-format="%s"' % self.GOANNA_FORMAT] + self.flags['common'] + self.flags['c'] 
            self.cppc= [join(GOANNA_PATH, "goannac++"), "--with-cxx=" + main_cc.replace('\\', '/'), "--dialect=armcc", '--output-format="%s"' % self.GOANNA_FORMAT] + self.flags['common'] + self.flags['c'] + self.flags['cxx']

        self.ld = [join(ARM_BIN, "armlink")]
        self.sys_libs = []

        self.ar = join(ARM_BIN, "armar")
        self.elf2bin = join(ARM_BIN, "fromelf")

    def remove_option(self, option):
        for tool in [self.asm, self.cc, self.cppc]:
            if option in tool:
                tool.remove(option)

    def assemble(self, source, object, includes):
        # Preprocess first, then assemble
        tempfile = object + '.E.s'
        return [
            self.asm + ['-D%s' % s for s in self.get_symbols() + self.macros] + ["-I%s" % i for i in includes] + ["-E", "-o", tempfile, source],
            self.hook.get_cmdline_assembler(self.asm + ["-o", object, tempfile])
        ]

    def parse_dependencies(self, dep_path):
        dependencies = []
        for line in open(dep_path).readlines():
            match = ARM.DEP_PATTERN.match(line)
            if match is not None:
                dependencies.append(match.group('file'))
        return dependencies

    def parse_output(self, output):
        for line in output.splitlines():
            match = ARM.DIAGNOSTIC_PATTERN.match(line)
            if match is not None:
                self.cc_info(
                    match.group('severity').lower(),
                    match.group('file'),
                    match.group('line'),
                    match.group('message'),
                    target_name=self.target.name,
                    toolchain_name=self.name
                )
            match = self.goanna_parse_line(line)
            if match is not None:
                self.cc_info(
                    match.group('severity').lower(),
                    match.group('file'),
                    match.group('line'),
                    match.group('message')
                )

    def get_dep_opt(self, dep_path):
        return ["--depend", dep_path]

    def archive(self, objects, lib_path):
        self.default_cmd([self.ar, '-r', lib_path] + objects)

    def link(self, output, objects, libraries, lib_dirs, mem_map):
        if len(lib_dirs):
            args = ["-o", output, "--userlibpath", ",".join(lib_dirs), "--info=totals", "--list=.link_totals.txt"]
        else:
            args = ["-o", output, "--info=totals", "--list=.link_totals.txt"]

        if mem_map:
            args.extend(["--scatter", mem_map])

        if hasattr(self.target, "link_cmdline_hook"):
            args = self.target.link_cmdline_hook(self.__class__.__name__, args)

        self.default_cmd(self.ld + args + objects + libraries + self.sys_libs)

    @hook_tool
    def binary(self, resources, elf, bin):
        args = [self.elf2bin, '--bin', '-o', bin, elf]

        if hasattr(self.target, "binary_cmdline_hook"):
            args = self.target.binary_cmdline_hook(self.__class__.__name__, args)

        self.default_cmd(args)

class ARM_STD(ARM):
    def __init__(self, target, options=None, notify=None, macros=None, silent=False, extra_verbose=False):
        ARM.__init__(self, target, options, notify, macros, silent, extra_verbose=extra_verbose)
        self.ld.append("--libpath=%s" % ARM_LIB)


class ARM_MICRO(ARM):
    PATCHED_LIBRARY = False

    def __init__(self, target, options=None, notify=None, macros=None, silent=False, extra_verbose=False):
        ARM.__init__(self, target, options, notify, macros, silent, extra_verbose=extra_verbose)

        # add microlib to the command line flags
        self.asm  += ["-D__MICROLIB"]
        self.cc += ["--library_type=microlib", "-D__MICROLIB"]
        self.cppc += ["--library_type=microlib", "-D__MICROLIB"]

        # the exporter uses --library_type flag to set microlib
        self.flags['c']   += ["--library_type=microlib"]
        self.flags['cxx'] += ["--library_type=microlib"]
        self.flags['ld'].append("--library_type=microlib")

        # We had to patch microlib to add C++ support
        # In later releases this patch should have entered mainline
        if ARM_MICRO.PATCHED_LIBRARY:
            self.flags['ld'].append("--noscanlib")

            # System Libraries
            self.sys_libs.extend([join(MY_ARM_CLIB, lib+".l") for lib in ["mc_p", "mf_p", "m_ps"]])

            if target.core == "Cortex-M3":
                self.sys_libs.extend([join(ARM_CPPLIB, lib+".l") for lib in ["cpp_ws", "cpprt_w"]])

            elif target.core in ["Cortex-M0", "Cortex-M0+"]:
                self.sys_libs.extend([join(ARM_CPPLIB, lib+".l") for lib in ["cpp_ps", "cpprt_p"]])
        else:
            self.ld.append("--libpath=%s" % ARM_LIB)
