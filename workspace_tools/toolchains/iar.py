import re
from os import remove
from os.path import join, exists

from workspace_tools.toolchains import mbedToolchain
from workspace_tools.settings import IAR_PATH


class IAR(mbedToolchain):
    LIBRARY_EXT = '.a'
    LINKER_EXT = '.icf'
    STD_LIB_NAME = "%s.a"
    
    DIAGNOSTIC_PATTERN = re.compile('"(?P<file>[^"]+)",(?P<line>[\d]+)\s+(?P<severity>Warning|Error)(?P<message>.+)')
    
    def __init__(self, target, options=None, notify=None):
        mbedToolchain.__init__(self, target, options, notify)
        
        c_flags = [
            "-Oh",
            "--cpu=%s" % target.core, "--thumb",
            "--dlib_config", join(IAR_PATH, "inc", "c", "DLib_Config_Full.h"),
            "-e", # Enable IAR language extension
            "--no_wrap_diagnostics",
            # Pa050: No need to be notified about "non-native end of line sequence"
            # Pa084: Pointless integer comparison -> checks for the values of an enum, but we use values outside of the enum to notify errors (ie: NC).
            # Pa093: Implicit conversion from float to integer (ie: wait_ms(85.4) -> wait_ms(85))
            # Pa082: Operation involving two values from two registers (ie: (float)(*obj->MR)/(float)(LPC_PWM1->MR0))
            "--diag_suppress=Pa050,Pa084,Pa093,Pa082",
        ]

        if "debug-info" in self.options:
            c_flags.append("-r")
        
        IAR_BIN = join(IAR_PATH, "bin")
        self.asm  = [join(IAR_BIN, "iasmarm")] + ["--cpu", target.core]
        self.cc   = [join(IAR_BIN, "iccarm")] + c_flags
        self.cppc = [join(IAR_BIN, "iccarm"), "--c++",  "--no_rtti", "--no_exceptions"] + c_flags
        
        self.ld   = join(IAR_BIN, "ilinkarm")
        self.ar = join(IAR_BIN, "iarchive")
        self.elf2bin = join(IAR_BIN, "ielftool")
    
    def parse_output(self, output):
        for line in output.splitlines():
            match = IAR.DIAGNOSTIC_PATTERN.match(line)
            if match is not None:
                self.cc_info(
                    match.group('severity').lower(),
                    match.group('file'),
                    match.group('line'),
                    match.group('message'),
                )
    
    def get_dep_opt(self, dep_path):
        return ["--dependencies", dep_path]
    
    def cc_extra(self, base):
        return ["-l", base + '.s']
    
    def parse_dependencies(self, dep_path):
        return [path.strip() for path in open(dep_path).readlines()
                if (path and not path.isspace())]
    
    def assemble(self, source, object):
        self.default_cmd(self.asm + ["-o", object, source])
    
    def archive(self, objects, lib_path):
        if exists(lib_path):
            remove(lib_path)
        self.default_cmd([self.ar, lib_path] + objects)
    
    def link(self, output, objects, libraries, lib_dirs, mem_map):
        args = [self.ld, "-o", output, "--config", mem_map]
        self.default_cmd(args + objects + libraries)
    
    def binary(self, elf, bin):
        self.default_cmd([self.elf2bin, '--bin', elf, bin])
