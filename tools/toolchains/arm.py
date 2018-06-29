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
from __future__ import print_function, absolute_import
from builtins import str

import re
from copy import copy
from os.path import join, dirname, splitext, basename, exists, relpath, isfile
from os import makedirs, write, curdir, remove
from tempfile import mkstemp
from shutil import rmtree
from distutils.version import LooseVersion

from tools.toolchains import mbedToolchain, TOOLCHAIN_PATHS
from tools.hooks import hook_tool
from tools.utils import mkdir, NotSupportedException, run_cmd

class ARM(mbedToolchain):
    LINKER_EXT = '.sct'
    LIBRARY_EXT = '.ar'

    STD_LIB_NAME = "%s.ar"
    DIAGNOSTIC_PATTERN  = re.compile('"(?P<file>[^"]+)", line (?P<line>\d+)( \(column (?P<column>\d+)\)|): (?P<severity>Warning|Error|Fatal error): (?P<message>.+)')
    INDEX_PATTERN  = re.compile('(?P<col>\s*)\^')
    DEP_PATTERN = re.compile('\S+:\s(?P<file>.+)\n')
    SHEBANG = "#! armcc -E"
    SUPPORTED_CORES = ["Cortex-M0", "Cortex-M0+", "Cortex-M3", "Cortex-M4",
                       "Cortex-M4F", "Cortex-M7", "Cortex-M7F", "Cortex-M7FD", "Cortex-A9"]
    ARMCC_RANGE = (LooseVersion("5.06"), LooseVersion("5.07"))
    ARMCC_VERSION_RE = re.compile("Product: ARM Compiler (\d+\.\d+)")

    @staticmethod
    def check_executable():
        """Returns True if the executable (armcc) location specified by the
         user exists OR the executable can be found on the PATH.
         Returns False otherwise."""
        return mbedToolchain.generic_check_executable("ARM", 'armcc', 2, 'bin')

    def __init__(self, target, notify=None, macros=None,
                 build_profile=None, build_dir=None):
        mbedToolchain.__init__(
            self, target, notify, macros, build_dir=build_dir,
            build_profile=build_profile)
        if target.core not in self.SUPPORTED_CORES:
            raise NotSupportedException(
                "this compiler does not support the core %s" % target.core)

        if getattr(target, "default_lib", "std") == "small":
            if "-DMBED_RTOS_SINGLE_THREAD" not in self.flags['common']:
                self.flags['common'].append("-DMBED_RTOS_SINGLE_THREAD")
            if "--library_type=microlib" not in self.flags['ld']:
                self.flags['ld'].append("--library_type=microlib")

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

        self.ld = [join(ARM_BIN, "armlink")] + self.flags['ld']

        self.ar = join(ARM_BIN, "armar")
        self.elf2bin = join(ARM_BIN, "fromelf")

        self.SHEBANG += " --cpu=%s" % cpu

    def version_check(self):
        stdout, _, retcode = run_cmd([self.cc[0], "--vsn"], redirect=True)
        msg = None
        min_ver, max_ver = self.ARMCC_RANGE
        match = self.ARMCC_VERSION_RE.search(stdout)
        found_version = LooseVersion(match.group(1)) if match else None
        min_ver, max_ver = self.ARMCC_RANGE
        if found_version and (found_version < min_ver or found_version >= max_ver):
            msg = ("Compiler version mismatch: Have {}; "
                   "expected version >= {} and < {}"
                   .format(found_version, min_ver, max_ver))
        elif not match or len(match.groups()) != 1:
            msg = ("Compiler version mismatch: Could not detect version; "
                   "expected version >= {} and < {}"
                   .format(min_ver, max_ver))

        if msg:
            self.notify.cc_info({
                "message": msg,
                "file": "",
                "line": "",
                "col": "",
                "severity": "ERROR",
            })

    def _get_toolchain_labels(self):
        if getattr(self.target, "default_lib", "std") == "small":
            return ["ARM", "ARM_MICRO"]
        else:
            return ["ARM", "ARM_STD"]

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
                    self.notify.cc_info(msg)
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
                    self.notify.cc_info(msg)
                    msg = None
                else:
                    msg['text'] += line+"\n"
        
        if msg is not None:
            self.notify.cc_info(msg)

    def get_dep_option(self, object):
        base, _ = splitext(object)
        dep_path = base + '.d'
        return ["--depend", dep_path]

    def get_config_option(self, config_header):
        return ['--preinclude=' + config_header]

    def get_compile_options(self, defines, includes, for_asm=False):
        opts = ['-D%s' % d for d in defines]
        if for_asm:
            return opts
        if self.RESPONSE_FILES:
            opts += ['--via', self.get_inc_file(includes)]
        else:
            opts += ["-I%s" % i for i in includes]

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
        cmd_pre = copy(self.asm)
        cmd_pre.extend(self.get_compile_options(
            self.get_symbols(True), includes, True))
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

    def correct_scatter_shebang(self, scatter_file, base_path=curdir):
        """Correct the shebang at the top of a scatter file.

        Positional arguments:
        scatter_file -- the scatter file to correct

        Return:
        The location of the correct scatter file

        Side Effects:
        This method MAY write a new scatter file to disk
        """
        with open(scatter_file, "r") as input:
            lines = input.readlines()
            if (lines[0].startswith(self.SHEBANG) or
                not lines[0].startswith("#!")):
                return scatter_file
            else:
                new_scatter = join(self.build_dir, ".link_script.sct")
                self.SHEBANG += " -I %s" % relpath(dirname(scatter_file),
                                                   base_path)
                if self.need_update(new_scatter, [scatter_file]):
                    with open(new_scatter, "w") as out:
                        out.write(self.SHEBANG)
                        out.write("\n")
                        out.write("".join(lines[1:]))

                return new_scatter

    @hook_tool
    def link(self, output, objects, libraries, lib_dirs, scatter_file):
        base, _ = splitext(output)
        map_file = base + ".map"
        args = ["-o", output, "--info=totals", "--map", "--list=%s" % map_file]
        args.extend(objects)
        args.extend(libraries)
        if lib_dirs:
            args.extend(["--userlibpath", ",".join(lib_dirs)])
        if scatter_file:
            new_scatter = self.correct_scatter_shebang(scatter_file)
            args.extend(["--scatter", new_scatter])

        cmd_pre = self.ld + args
        cmd = self.hook.get_cmdline_linker(cmd_pre)

        if self.RESPONSE_FILES:
            cmd_linker = cmd[0]
            link_files = self.get_link_file(cmd[1:])
            cmd = [cmd_linker, '--via', link_files]

        self.notify.cc_verbose("Link: %s" % ' '.join(cmd))
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
        # On .hex format, combine multiple .hex files (for multiple load regions) into one 
        bin_arg = {".bin": "--bin", ".hex": "--i32combined"}[fmt]
        cmd = [self.elf2bin, bin_arg, '-o', bin, elf]
        cmd = self.hook.get_cmdline_binary(cmd)

        # remove target binary file/path
        if exists(bin):
            if isfile(bin):
                remove(bin)
            else:
                rmtree(bin)

        self.notify.cc_verbose("FromELF: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    @staticmethod
    def name_mangle(name):
        return "_Z%i%sv" % (len(name), name)

    @staticmethod
    def make_ld_define(name, value):
        return "--predefine=\"-D%s=0x%x\"" % (name, value)

    @staticmethod
    def redirect_symbol(source, sync, build_dir):
        if not exists(build_dir):
            makedirs(build_dir)
        handle, filename = mkstemp(prefix=".redirect-symbol.", dir=build_dir)
        write(handle, "RESOLVE %s AS %s\n" % (source, sync))
        return "--edit=%s" % filename


class ARM_STD(ARM):
    def __init__(self, target, notify=None, macros=None,
                 build_profile=None, build_dir=None):
        ARM.__init__(self, target, notify, macros, build_dir=build_dir,
                     build_profile=build_profile)
        if not set(("ARM", "uARM")).intersection(set(target.supported_toolchains)):
            raise NotSupportedException("ARM/uARM compiler support is required for ARM build")


class ARM_MICRO(ARM):
    PATCHED_LIBRARY = False
    def __init__(self, target, notify=None, macros=None,
                 silent=False, extra_verbose=False, build_profile=None,
                 build_dir=None):
        target.default_lib = "small"
        ARM.__init__(self, target, notify, macros, build_dir=build_dir,
                     build_profile=build_profile)
        if not set(("ARM", "uARM")).intersection(set(target.supported_toolchains)):
            raise NotSupportedException("ARM/uARM compiler support is required for ARM build")

class ARMC6(ARM_STD):
    SHEBANG = "#! armclang -E --target=arm-arm-none-eabi -x c"
    SUPPORTED_CORES = ["Cortex-M0", "Cortex-M0+", "Cortex-M3", "Cortex-M4",
                       "Cortex-M4F", "Cortex-M7", "Cortex-M7F", "Cortex-M7FD",
                       "Cortex-M23", "Cortex-M23-NS", "Cortex-M33",
                       "CortexM33-NS", "Cortex-A9"]
    ARMCC_RANGE = (LooseVersion("6.10"), LooseVersion("7.0"))

    @staticmethod
    def check_executable():
        return mbedToolchain.generic_check_executable("ARMC6", "armclang", 1)

    def __init__(self, target, *args, **kwargs):
        mbedToolchain.__init__(self, target, *args, **kwargs)
        if target.core not in self.SUPPORTED_CORES:
            raise NotSupportedException(
                "this compiler does not support the core %s" % target.core)

        if not set(("ARM", "ARMC6")).intersection(set(target.supported_toolchains)):
            raise NotSupportedException("ARM/ARMC6 compiler support is required for ARMC6 build")

        if target.core.lower().endswith("fd"):
            self.flags['common'].append("-mcpu=%s" % target.core.lower()[:-2])
            self.flags['ld'].append("--cpu=%s" % target.core.lower()[:-2])
            self.SHEBANG += " -mcpu=%s" % target.core.lower()[:-2]
        elif target.core.lower().endswith("f"):
            self.flags['common'].append("-mcpu=%s" % target.core.lower()[:-1])
            self.flags['ld'].append("--cpu=%s" % target.core.lower()[:-1])
            self.SHEBANG += " -mcpu=%s" % target.core.lower()[:-1]
        elif target.core.lower().endswith("ns"):
            self.flags['common'].append("-mcpu=%s" % target.core.lower()[:-3])
            self.flags['ld'].append("--cpu=%s" % target.core.lower()[:-3])
            self.SHEBANG += " -mcpu=%s" % target.core.lower()[:-3]
        else:
            self.flags['common'].append("-mcpu=%s" % target.core.lower())
            self.flags['ld'].append("--cpu=%s" % target.core.lower())
            self.SHEBANG += " -mcpu=%s" % target.core.lower()

        if target.core == "Cortex-M4F":
            self.flags['common'].append("-mfpu=fpv4-sp-d16")
            self.flags['common'].append("-mfloat-abi=hard")
        elif target.core == "Cortex-M7F":
            self.flags['common'].append("-mfpu=fpv5-sp-d16")
            self.flags['common'].append("-mfloat-abi=softfp")
        elif target.core == "Cortex-M7FD":
            self.flags['common'].append("-mfpu=fpv5-d16")
            self.flags['common'].append("-mfloat-abi=softfp")
        elif target.core.startswith("Cortex-M23"):
            self.flags['common'].append("-march=armv8-m.base")
        elif target.core.startswith("Cortex-M33"):
            self.flags['common'].append("-march=armv8-m.main")

        if target.core == "Cortex-M23" or target.core == "Cortex-M33":
            self.flags['common'].append("-mcmse")

        # Create Secure library
        if ((target.core == "Cortex-M23" or self.target.core == "Cortex-M33") and
            kwargs.get('build_dir', False)):
            build_dir = kwargs['build_dir']
            secure_file = join(build_dir, "cmse_lib.o")
            self.flags["ld"] += ["--import_cmse_lib_out=%s" % secure_file]
        # Add linking time preprocessor macro __DOMAIN_NS
        if target.core == "Cortex-M23-NS" or self.target.core == "Cortex-M33-NS":
            define_string = self.make_ld_define("__DOMAIN_NS", 1)
            self.flags["ld"].append(define_string)

        asm_cpu = {
            "Cortex-M0+": "Cortex-M0",
            "Cortex-M4F": "Cortex-M4.fp",
            "Cortex-M7F": "Cortex-M7.fp.sp",
            "Cortex-M7FD": "Cortex-M7.fp.dp",
            "Cortex-M23-NS": "Cortex-M23",
            "Cortex-M33-NS": "Cortex-M33" }.get(target.core, target.core)

        self.flags['asm'].append("--cpu=%s" % asm_cpu)

        self.cc = ([join(TOOLCHAIN_PATHS["ARMC6"], "armclang")] +
                   self.flags['common'] + self.flags['c'])
        self.cppc = ([join(TOOLCHAIN_PATHS["ARMC6"], "armclang")] +
                     self.flags['common'] + self.flags['cxx'])
        self.asm = [join(TOOLCHAIN_PATHS["ARMC6"], "armasm")] + self.flags['asm']
        self.ld = [join(TOOLCHAIN_PATHS["ARMC6"], "armlink")] + self.flags['ld']
        self.ar = [join(TOOLCHAIN_PATHS["ARMC6"], "armar")]
        self.elf2bin = join(TOOLCHAIN_PATHS["ARMC6"], "fromelf")

    def _get_toolchain_labels(self):
        return ["ARM", "ARM_STD", "ARMC6"]

    def parse_dependencies(self, dep_path):
        return mbedToolchain.parse_dependencies(self, dep_path)

    def is_not_supported_error(self, output):
        return "#error [NOT_SUPPORTED]" in output

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
