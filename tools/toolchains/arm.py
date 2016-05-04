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
from os.path import join, dirname, splitext, basename, exists
from hashlib import md5

from tools.toolchains import mbedToolchain
from tools.settings import ARM_BIN, ARM_INC, ARM_LIB, MY_ARM_CLIB, ARM_CPPLIB, GOANNA_PATH
from tools.hooks import hook_tool
from tools.utils import mkdir

class ARM(mbedToolchain):
    LINKER_EXT = '.sct'
    LIBRARY_EXT = '.ar'

    STD_LIB_NAME = "%s.ar"
    DIAGNOSTIC_PATTERN  = re.compile('"(?P<file>[^"]+)", line (?P<line>\d+)( \(column (?P<column>\d+)\)|): (?P<severity>Warning|Error): (?P<message>.+)')
    DEP_PATTERN = re.compile('\S+:\s(?P<file>.+)\n')

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
        common = ["-c",
            "--cpu=%s" % cpu, "--gnu",
            "-Otime", "--split_sections", "--apcs=interwork",
            "--brief_diagnostics", "--restrict", "--multibyte_chars"
        ]

        if "save-asm" in self.options:
            common.extend(["--asm", "--interleave"])

        if "debug-info" in self.options:
            common.append("-O0")
        else:
            common.append("-O3")
        # add debug symbols for all builds
        common.append("-g")

        common_c = [
            "--md", "--no_depend_system_headers",
            '-I%s' % ARM_INC
        ]

        self.asm = [main_cc] + common + ['-I%s' % ARM_INC]
        if not "analyze" in self.options:
            self.cc = [main_cc] + common + common_c + ["--c99"]
            self.cppc = [main_cc] + common + common_c + ["--cpp", "--no_rtti"]
        else:
            self.cc  = [join(GOANNA_PATH, "goannacc"), "--with-cc=" + main_cc.replace('\\', '/'), "--dialect=armcc", '--output-format="%s"' % self.GOANNA_FORMAT] + common + common_c + ["--c99"]
            self.cppc= [join(GOANNA_PATH, "goannac++"), "--with-cxx=" + main_cc.replace('\\', '/'), "--dialect=armcc", '--output-format="%s"' % self.GOANNA_FORMAT] + common + common_c + ["--cpp", "--no_rtti"]

        self.ld = [join(ARM_BIN, "armlink")]
        self.sys_libs = []

        self.ar = join(ARM_BIN, "armar")
        self.elf2bin = join(ARM_BIN, "fromelf")

    def remove_option(self, option):
        for tool in [self.asm, self.cc, self.cppc]:
            if option in tool:
                tool.remove(option)

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

    @hook_tool
    def assemble(self, source, object, includes):
        # Preprocess first, then assemble
        dir = join(dirname(object), '.temp')
        mkdir(dir)
        tempfile = join(dir, basename(object) + '.E.s')
        
        # Build preprocess assemble command
        cmd_pre = self.asm + ['-D%s' % s for s in self.get_symbols() + self.macros]

        # Response file
        inc_str = ' '.join(includes)
        if len(inc_str) > 16000:
            sum = md5(inc_str).hexdigest()
            include_files = join(self.temp_dir, "includes_%s.txt" % sum)
            if not exists(include_files):
                with open(include_files, "wb") as f:
                    cmd_list = []
                    for c in includes:
                        if c:
                            cmd_list.append(('-I"%s"' % c) if not c.startswith('-') else c)
                    string = " ".join(cmd_list).replace("\\", "/")
                    f.write(string)
            cmd_pre.extend(['--via', include_files])
        else:
            cmd_pre.extend(['-I"%s"' % i for i in includes])

        cmd_pre.extend(["-E", "-o", tempfile, source])

        # Build main assemble command
        cmd = self.asm + ["-o", object, tempfile]

        # Call cmdline hook
        cmd_pre = self.hook.get_cmdline_assembler(cmd_pre)
        cmd = self.hook.get_cmdline_assembler(cmd)
       
        # Return command array, don't execute
        return [cmd_pre, cmd]

    @hook_tool
    def compile(self, cc, source, object, includes):
        cmd = cc + ['-D %s' % s for s in self.get_symbols()]

        inc_str = ' '.join(includes)
        if len(inc_str) > 16000:
            sum = md5(inc_str).hexdigest()
            include_files = join(self.temp_dir, "includes_%s.txt" % sum)
            if not exists(include_files):
                with open(include_files, "wb") as f:
                    cmd_list = []
                    for c in includes:
                        if c:
                            cmd_list.append(('-I"%s"' % c) if not c.startswith('-') else c)
                    string = " ".join(cmd_list).replace("\\", "/")
                    f.write(string)
            cmd.extend(['--via', include_files])
        else:
            cmd.extend(['-I"%s"' % i for i in includes])

        
        base, _ = splitext(object)
        dep_path = base + '.d'
        cmd.extend(self.get_dep_opt(dep_path))
            
        cmd.extend(["-o", object, source])

        return [cmd]

    def compile_c(self, source, object, includes):
        return self.compile(self.cc, source, object, includes)

    def compile_cpp(self, source, object, includes):
        return self.compile(self.cppc, source, object, includes)

    @hook_tool
    def link(self, output, objects, libraries, lib_dirs, mem_map):
        if len(lib_dirs):
            args = ["-o", output, "--userlibpath", ",".join(lib_dirs), "--info=totals", "--list=.link_totals.txt"]
        else:
            args = ["-o", output, "--info=totals", "--list=.link_totals.txt"]

        if mem_map:
            args.extend(["--scatter", mem_map])

        # Build linker command
        cmd = self.ld + args + objects + libraries + self.sys_libs

        # Call cmdline hook
        cmd = self.hook.get_cmdline_linker(cmd)

        # Split link command to linker executable + response file
        link_files = join(dirname(output), ".link_files.txt")
        with open(link_files, "wb") as f:
            cmd_linker = cmd[0]
            cmd_list = []
            for c in cmd[1:]:
                if c:
                    cmd_list.append(('"%s"' % c) if not c.startswith('-') else c)                    
            string = " ".join(cmd_list).replace("\\", "/")
            f.write(string)

        # Exec command
        self.default_cmd([cmd_linker, '--via', link_files])

    @hook_tool
    def archive(self, objects, lib_path):
        archive_files = join(dirname(lib_path), ".archive_files.txt")
        with open(archive_files, "wb") as f:
            o_list = []
            for o in objects:
                o_list.append('"%s"' % o)                    
            string = " ".join(o_list).replace("\\", "/")
            f.write(string)

        # Exec command
        self.default_cmd([self.ar, '-r', lib_path, '--via', archive_files])

    @hook_tool
    def binary(self, resources, elf, bin):
        # Build binary command
        cmd = [self.elf2bin, '--bin', '-o', bin, elf]

        # Call cmdline hook
        cmd = self.hook.get_cmdline_binary(cmd)

        # Exec command
        self.default_cmd(cmd)


class ARM_STD(ARM):
    def __init__(self, target, options=None, notify=None, macros=None, silent=False, extra_verbose=False):
        ARM.__init__(self, target, options, notify, macros, silent, extra_verbose=extra_verbose)
        self.cc   += ["-D__ASSERT_MSG"]
        self.cppc += ["-D__ASSERT_MSG"]
        self.ld.extend(["--libpath", ARM_LIB])


class ARM_MICRO(ARM):
    PATCHED_LIBRARY = False

    def __init__(self, target, options=None, notify=None, macros=None, silent=False, extra_verbose=False):
        ARM.__init__(self, target, options, notify, macros, silent, extra_verbose=extra_verbose)

        # Compiler
        self.asm  += ["-D__MICROLIB"]
        self.cc   += ["--library_type=microlib", "-D__MICROLIB", "-D__ASSERT_MSG"]
        self.cppc += ["--library_type=microlib", "-D__MICROLIB", "-D__ASSERT_MSG"]

        # Linker
        self.ld.append("--library_type=microlib")

        # We had to patch microlib to add C++ support
        # In later releases this patch should have entered mainline
        if ARM_MICRO.PATCHED_LIBRARY:
            self.ld.append("--noscanlib")

            # System Libraries
            self.sys_libs.extend([join(MY_ARM_CLIB, lib+".l") for lib in ["mc_p", "mf_p", "m_ps"]])

            if target.core == "Cortex-M3":
                self.sys_libs.extend([join(ARM_CPPLIB, lib+".l") for lib in ["cpp_ws", "cpprt_w"]])

            elif target.core in ["Cortex-M0", "Cortex-M0+"]:
                self.sys_libs.extend([join(ARM_CPPLIB, lib+".l") for lib in ["cpp_ps", "cpprt_p"]])
        else:
            self.ld.extend(["--libpath", ARM_LIB])
