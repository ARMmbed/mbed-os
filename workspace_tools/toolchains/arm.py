import re
from os.path import join

from workspace_tools.toolchains import mbedToolchain
from workspace_tools.settings import ARM_BIN, ARM_INC, ARM_LIB, MY_ARM_CLIB, ARM_CPPLIB


class ARM(mbedToolchain):
    LINKER_EXT = '.sct'
    LIBRARY_EXT = '.ar'
    
    STD_LIB_NAME = "%s.ar"
    DIAGNOSTIC_PATTERN  = re.compile('"(?P<file>[^"]+)", line (?P<line>\d+): (?P<severity>Warning|Error): (?P<message>.+)')
    DEP_PATTERN = re.compile('\S+:\s(?P<file>.+)\n')
    
    def __init__(self, target, options=None, notify=None):
        mbedToolchain.__init__(self, target, options, notify)
        
        if   target.core == "Cortex-M0+":
            cpu = "Cortex-M0"
        elif target.core == "Cortex-M4":
            cpu = "Cortex-M4.fp"
        else:
            cpu = target.core
        
        common = [join(ARM_BIN, "armcc"), "-c",
            "--cpu=%s" % cpu, "--gnu",
            "-Ospace", "--split_sections", "--apcs=interwork",
            "--brief_diagnostics", "--restrict"
        ]
        
        if "save-asm" in self.options:
            common.extend(["--asm", "--interleave"])
        elif "debug-info" in self.options:
            common.extend(["--debug"])
        
        common_c = [
            "--md", "--no_depend_system_headers",
            '-I%s' % ARM_INC
        ]
        
        self.asm = common
        self.cc = common + common_c + ["--c99"]
        self.cppc = common + common_c + ["--cpp", "--no_rtti"]
        
        self.ld = [join(ARM_BIN, "armlink")]
        self.sys_libs = []
        
        self.ar = join(ARM_BIN, "armar")
        self.elf2bin = join(ARM_BIN, "fromelf")
    
    def remove_option(self, option):
        for tool in [self.asm, self.cc, self.cppc]:
            if option in tool:
                tool.remove(option)
    
    def assemble(self, source, object):
        self.default_cmd(self.cc + ["-o", object, source])
    
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
                    match.group('message')
                )
    
    def archive(self, objects, lib_path):
        self.default_cmd([self.ar, '-r', lib_path] + objects)
    
    def link(self, output, objects, libraries, lib_dirs, mem_map):
        args = ["-o", output, "--userlibpath", ",".join(lib_dirs), "--info=totals", "--list=.link_totals.txt"]
        if mem_map:
            args.extend(["--scatter", mem_map])
        
        self.default_cmd(self.ld + args + objects + libraries + self.sys_libs)
    
    def binary(self, elf, bin):
        self.default_cmd([self.elf2bin, '--bin', '-o', bin, elf])


class ARM_STD(ARM):
    def __init__(self, target, options=None, notify=None):
        ARM.__init__(self, target, options, notify)
        self.ld.append("--libpath=%s" % ARM_LIB)


class ARM_MICRO(ARM):
    PATCHED_LIBRARY = True
    
    def __init__(self, target, options=None, notify=None):
        ARM.__init__(self, target, notify)
        
        # Compiler
        self.asm  += ["-D__MICROLIB"]
        self.cc   += ["--library_type=microlib", "-D__MICROLIB"]
        self.cppc += ["--library_type=microlib", "-D__MICROLIB"]
        
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
            self.ld.append("--libpath=%s" % ARM_LIB)
