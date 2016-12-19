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
from os.path import join, dirname, splitext, basename

from tools.toolchains import mbedToolchain, TOOLCHAIN_PATHS
from tools.hooks import hook_tool
from tools.utils import mkdir

class ARM(mbedToolchain):
    LINKER_EXT = '.sct'
    LIBRARY_EXT = '.ar'

    STD_LIB_NAME = "%s.ar"
    DIAGNOSTIC_PATTERN  = re.compile('"(?P<file>[^"]+)", line (?P<line>\d+)( \(column (?P<column>\d+)\)|): (?P<severity>Warning|Error): (?P<message>.+)')
    INDEX_PATTERN  = re.compile('(?P<col>\s*)\^')
    DEP_PATTERN = re.compile('\S+:\s(?P<file>.+)\n')

    @staticmethod
    def check_executable():
        """Returns True if the executable (armcc) location specified by the
         user exists OR the executable can be found on the PATH.
         Returns False otherwise."""
        return mbedToolchain.generic_check_executable("ARM", 'armcc', 2, 'bin')

    def __init__(self, target, notify=None, macros=None,
                 silent=False, extra_verbose=False, build_profile=None):
        mbedToolchain.__init__(self, target, notify, macros, silent,
                               extra_verbose=extra_verbose,
                               build_profile=build_profile)

        if target.core == "Cortex-M0+":
            cpu = "Cortex-M0"
        elif target.core == "Cortex-M4F":
            cpu = "Cortex-M4.fp"
        elif target.core == "Cortex-M7FD":
            cpu = "Cortex-M7.fp.dp"
        elif target.core == "Cortex-M7F":
            cpu = "Cortex-M7.fp.sp"
        else:
            cpu = target.core

        ARM_BIN = join(TOOLCHAIN_PATHS['ARM'], "bin")
        ARM_INC = join(TOOLCHAIN_PATHS['ARM'], "include")
        
        main_cc = join(ARM_BIN, "armcc")

        self.flags['common'] += ["--cpu=%s" % cpu]

        self.asm = [main_cc] + self.flags['common'] + self.flags['asm']
        self.cc = [main_cc] + self.flags['common'] + self.flags['c']
        self.cppc = [main_cc] + self.flags['common'] + self.flags['c'] + self.flags['cxx']

        self.ld = [join(ARM_BIN, "armlink")]
        self.sys_libs = []

        self.ar = join(ARM_BIN, "armar")
        self.elf2bin = join(ARM_BIN, "fromelf")

    def parse_dependencies(self, dep_path):
        dependencies = []
        for line in open(dep_path).readlines():
            match = ARM.DEP_PATTERN.match(line)
            if match is not None:
                #we need to append chroot, because when the .d files are generated the compiler is chrooted
                dependencies.append((self.CHROOT if self.CHROOT else '') + match.group('file'))
        return dependencies
        
    def parse_output(self, output):
        msg = None
        for line in output.splitlines():
            match = ARM.DIAGNOSTIC_PATTERN.match(line)
            if match is not None:
                if msg is not None:
                    self.cc_info(msg)
                    msg = None
                msg = {
                    'severity': match.group('severity').lower(),
                    'file': match.group('file'),
                    'line': match.group('line'),
                    'col': match.group('column') if match.group('column') else 0,
                    'message': match.group('message'),
                    'text': '',
                    'target_name': self.target.name,
                    'toolchain_name': self.name
                }
            elif msg is not None:
                # Determine the warning/error column by calculating the ^ position
                match = ARM.INDEX_PATTERN.match(line)
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
        return ["--depend", dep_path]

    def get_config_option(self, config_header):
        return ['--preinclude=' + config_header]

    def get_compile_options(self, defines, includes, for_asm=False):        
        opts = ['-D%s' % d for d in defines]
        if self.RESPONSE_FILES:
            opts += ['--via', self.get_inc_file(includes)]
        else:
            opts += ["-I%s" % i for i in includes]

        if not for_asm:
            config_header = self.get_config_header()
            if config_header is not None:
                opts = opts + self.get_config_option(config_header)
        return opts

    @hook_tool
    def assemble(self, source, object, includes):
        # Preprocess first, then assemble
        dir = join(dirname(object), '.temp')
        mkdir(dir)
        tempfile = join(dir, basename(object) + '.E.s')
        
        # Build preprocess assemble command
        cmd_pre = self.asm + self.get_compile_options(self.get_symbols(True), includes) + ["-E", "-o", tempfile, source]

        # Build main assemble command
        cmd = self.asm + ["-o", object, tempfile]

        # Call cmdline hook
        cmd_pre = self.hook.get_cmdline_assembler(cmd_pre)
        cmd = self.hook.get_cmdline_assembler(cmd)
       
        # Return command array, don't execute
        return [cmd_pre, cmd]

    @hook_tool
    def compile(self, cc, source, object, includes):
        # Build compile command
        cmd = cc + self.get_compile_options(self.get_symbols(), includes)
        
        cmd.extend(self.get_dep_option(object))
            
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
        map_file = splitext(output)[0] + ".map"
        if len(lib_dirs):
            args = ["-o", output, "--userlibpath", ",".join(lib_dirs), "--info=totals", "--map", "--list=%s" % map_file]
        else:
            args = ["-o", output, "--info=totals", "--map", "--list=%s" % map_file]

        if mem_map:
            args.extend(["--scatter", mem_map])

        # Build linker command
        cmd = self.ld + args + objects + libraries + self.sys_libs

        # Call cmdline hook
        cmd = self.hook.get_cmdline_linker(cmd)

        if self.RESPONSE_FILES:
            # Split link command to linker executable + response file
            cmd_linker = cmd[0]
            link_files = self.get_link_file(cmd[1:])
            cmd = [cmd_linker, '--via', link_files]

        # Exec command
        self.cc_verbose("Link: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    @hook_tool
    def archive(self, objects, lib_path):
        if self.RESPONSE_FILES:
            param = ['--via', self.get_arch_file(objects)]
        else:
            param = objects

        # Exec command
        self.default_cmd([self.ar, '-r', lib_path] + param)

    @hook_tool
    def binary(self, resources, elf, bin):
        # Build binary command
        cmd = [self.elf2bin, '--bin', '-o', bin, elf]

        # Call cmdline hook
        cmd = self.hook.get_cmdline_binary(cmd)

        # Exec command
        self.cc_verbose("FromELF: %s" % ' '.join(cmd))
        self.default_cmd(cmd)


class ARM_STD(ARM):
    pass


class ARM_MICRO(ARM):
    PATCHED_LIBRARY = False
