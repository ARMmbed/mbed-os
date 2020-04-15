"""
mbed SDK
Copyright (c) 2011-2019 ARM Limited
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
from __future__ import print_function, absolute_import
from builtins import str  # noqa: F401

import re
from copy import copy
from os.path import join, dirname, splitext, basename, exists, isfile, relpath, sep
from os import makedirs, write, remove
from tempfile import mkstemp
from shutil import rmtree
from distutils.version import LooseVersion

from tools.toolchains.mbed_toolchain import (
    mbedToolchain, TOOLCHAIN_PATHS, should_replace_small_c_lib
)
from tools.utils import mkdir, NotSupportedException, run_cmd
from tools.resources import FileRef

UARM_TOOLCHAIN_WARNING = (
    "Warning: We noticed that you are using uARM Toolchain either via --toolchain command line or default_toolchain option. "
    "We are deprecating the use of the uARM Toolchain. "
    "For more information on how to use the ARM toolchain with small C libraries, "
    "please visit https://os.mbed.com/docs/mbed-os/latest/reference/using-small-c-libraries.html"
)

class ARM(mbedToolchain):
    LINKER_EXT = '.sct'
    LIBRARY_EXT = '.ar'

    STD_LIB_NAME = "%s.ar"
    DIAGNOSTIC_PATTERN = re.compile('"(?P<file>[^"]+)", line (?P<line>\d+)( \(column (?P<column>\d+)\)|): (?P<severity>Warning|Error|Fatal error): (?P<message>.+)')
    INDEX_PATTERN = re.compile('(?P<col>\s*)\^')
    DEP_PATTERN = re.compile('\S+:\s(?P<file>.+)\n')
    SHEBANG = "#! armcc -E"
    SUPPORTED_CORES = [
        "Cortex-M0", "Cortex-M0+", "Cortex-M3", "Cortex-M4", "Cortex-M4F",
        "Cortex-M7", "Cortex-M7F", "Cortex-M7FD", "Cortex-A9"
    ]
    ARMCC_RANGE = (LooseVersion("5.06"), LooseVersion("5.07"))
    ARMCC_PRODUCT_RE = re.compile(b"Product: (.*)")
    ARMCC_VERSION_RE = re.compile(b"Component: ARM Compiler (\d+\.\d+)")

    @staticmethod
    def check_executable():
        """Returns True if the executable (armcc) location specified by the
         user exists OR the executable can be found on the PATH.
         Returns False otherwise."""
        return mbedToolchain.generic_check_executable("ARM", 'armcc', 2, 'bin')

    def __init__(self, target, notify=None, macros=None,
                 build_profile=None, build_dir=None, coverage_patterns=None):
        mbedToolchain.__init__(
            self, target, notify, macros, build_dir=build_dir,
            build_profile=build_profile)
        if target.core not in self.SUPPORTED_CORES:
            raise NotSupportedException(
                "this compiler does not support the core %s" % target.core)

        toolchain = "arm"

        if should_replace_small_c_lib(target, toolchain):
            target.c_lib = "std"

        self.check_c_lib_supported(target, toolchain)

        if (
            getattr(target, "default_toolchain", "ARM") == "uARM"
            or getattr(target, "c_lib", "std") == "small"
        ):
            if "-DMBED_RTOS_SINGLE_THREAD" not in self.flags['common']:
                self.flags['common'].append("-DMBED_RTOS_SINGLE_THREAD")
            if "-D__MICROLIB" not in self.flags['common']:
                self.flags['common'].append("-D__MICROLIB")
            if "--library_type=microlib" not in self.flags['ld']:
                self.flags['ld'].append("--library_type=microlib")
            if "--library_type=microlib" not in self.flags['common']:
                self.flags['common'].append("--library_type=microlib")

        self.check_and_add_minimal_printf(target)

        cpu = {
            "Cortex-M0+": "Cortex-M0plus",
            "Cortex-M4F": "Cortex-M4.fp.sp",
            "Cortex-M7F": "Cortex-M7.fp.sp",
            "Cortex-M7FD": "Cortex-M7.fp.dp"}.get(target.core, target.core)

        ARM_BIN = join(TOOLCHAIN_PATHS['ARM'], "bin")

        main_cc = join(ARM_BIN, "armcc")

        self.flags['common'] += ["--cpu=%s" % cpu]

        self.asm = [main_cc] + self.flags['common'] + self.flags['asm']
        self.cc = [main_cc] + self.flags['common'] + self.flags['c']
        self.cppc = (
            [main_cc] + self.flags['common'] +
            self.flags['c'] + self.flags['cxx']
        )

        self.ld = [join(ARM_BIN, "armlink")] + self.flags['ld']

        self.ar = join(ARM_BIN, "armar")
        self.elf2bin = join(ARM_BIN, "fromelf")

        self.SHEBANG += " --cpu=%s" % cpu

        self.product_name = None

    def version_check(self):
        # The --ide=mbed removes an instability with checking the version of
        # the ARMC6 binary that comes with Mbed Studio.
        # NOTE: the --ide=mbed argument is only for use with Mbed OS
        stdout, _, retcode = run_cmd(
            [self.cc[0], "--vsn", "--ide=mbed"],
            redirect=True
        )
        msg = None
        min_ver, max_ver = self.ARMCC_RANGE
        output = stdout.encode("utf-8")
        match = self.ARMCC_VERSION_RE.search(output)
        if match:
            found_version = LooseVersion(match.group(1).decode("utf-8"))
        else:
            found_version = None
        min_ver, max_ver = self.ARMCC_RANGE
        if found_version and (found_version < min_ver
                              or found_version >= max_ver):
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
                "severity": "WARNING",
            })

        msg = None
        match = self.ARMCC_PRODUCT_RE.search(output)
        if match:
            self.product_name = match.group(1).decode("utf-8")
        else:
            self.product_name = None

        if not match or len(match.groups()) != 1:
            msg = (
                "Could not detect product name: defaulting to professional "
                "version of ARMC6"
            )

    def _get_toolchain_labels(self):
        if getattr(self.target, "default_toolchain", "ARM") == "uARM":
            return ["ARM", "ARM_MICRO", "ARMC5"]
        else:
            return ["ARM", "ARM_STD", "ARMC5"]

    def parse_dependencies(self, dep_path):
        dependencies = []
        for line in open(dep_path).readlines():
            match = ARM.DEP_PATTERN.match(line)
            if match is not None:
                # we need to append chroot, because when the .d files are
                # generated the compiler is chrooted
                dependencies.append(
                    (self.CHROOT if self.CHROOT else '') + match.group('file')
                )
        return dependencies

    def parse_output(self, output):
        msg = None
        for line in output.splitlines():
            match = self.DIAGNOSTIC_PATTERN.match(line)
            if match is not None:
                if msg is not None:
                    self.notify.cc_info(msg)
                    msg = None
                msg = {
                    'severity': match.group('severity').lower(),
                    'file': match.group('file'),
                    'line': match.group('line'),
                    'message': match.group('message'),
                    'text': '',
                    'target_name': self.target.name,
                    'toolchain_name': self.name
                }
                if match.group('column'):
                    msg['col'] = match.group('column')
                else:
                    msg['col'] = 0
            elif msg is not None:
                # Determine the warning/error column by calculating the '^'
                # position
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
        config_header = self.get_config_header()
        if config_header is not None:
            opts = opts + self.get_config_option(config_header)
        if for_asm:
            return opts
        if self.RESPONSE_FILES:
            opts += ['--via', self.get_inc_file(includes)]
        else:
            opts += ["-I%s" % i for i in includes if i]

        return opts

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

        # Return command array, don't execute
        return [cmd_pre, cmd]

    def compile(self, cc, source, object, includes):
        # Build compile command
        cmd = cc + self.get_compile_options(self.get_symbols(), includes)

        cmd.extend(self.get_dep_option(object))

        cmd.extend(["-o", object, source])

        return [cmd]

    def compile_c(self, source, object, includes):
        return self.compile(self.cc, source, object, includes)

    def compile_cpp(self, source, object, includes):
        return self.compile(self.cppc, source, object, includes)

    def correct_scatter_shebang(self, sc_fileref, cur_dir_name=None):
        """Correct the shebang at the top of a scatter file.

        The shebang line is the line at the top of the file starting with '#!'. If this line is present
        then the linker will execute the command on that line on the content of the scatter file prior
        to consuming the content into the link. Typically the shebang line will contain an instruction
        to run the C-preprocessor (either 'armcc -E' or 'armclang -E') which allows for macro expansion,
        inclusion of headers etc. Other options are passed to the preprocessor to specify aspects of the
        system such as the processor architecture and cpu type.

        The build system (at this point) will have constructed what it considers to be a correct shebang
        line for this build. If this differs from the line in the scatter file then the scatter file
        will be rewritten by this function to contain the build-system-generated shebang line. Note
        that the rewritten file will be placed in the BUILD output directory.

        Include processing

        If the scatter file runs the preprocessor, and contains #include statements then the pre-processor
        include path specifies where the #include files are to be found. Typically, #include files
        are specified with a path relative to the location of the original scatter file. When the
        preprocessor runs, the system automatically passes the location of the scatter file into the
        include path through an implicit '-I' option to the preprocessor, and this works fine in the
        offline build system.
        Unfortunately this approach does not work in the online build, because the preprocessor
        command runs in a chroot. The true (non-chroot) path to the file as known by the build system
        looks something like this:
            /tmp/chroots/ch-eefd72fb-2bcb-4e99-9043-573d016618bb/extras/mbed-os.lib/...
        whereas the path known by the preprocessor will be:
            /extras/mbed-os.lib/...
        Consequently, the chroot path has to be explicitly passed to the preprocessor through an
        explicit -I/path/to/chroot/file option in the shebang line.

        *** THERE IS AN ASSUMPTION THAT THE CHROOT PATH IS THE REAL FILE PATH WITH THE FIRST
        *** THREE ELEMENTS REMOVED. THIS ONLY HOLDS TRUE UNTIL THE ONLINE BUILD SYSTEM CHANGES

        If the include path manipulation as described above does change, then any scatter file
        containing a #include statement is likely to fail on the online compiler.

        Positional arguments:
        sc_fileref -- FileRef object of the scatter file

        Keyword arguments:
        cur_dir_name -- the name (not path) of the directory containing the
                        scatter file

        Return:
        The FileRef of the correct scatter file

        Side Effects:
        This method MAY write a new scatter file to disk
        """
        with open(sc_fileref.path, "r") as input:
            lines = input.readlines()

            # If the existing scatter file has no shebang line, or the line that it does have
            # matches the desired line then the existing scatter file is used directly without rewriting.
            if (lines[0].startswith(self.SHEBANG) or
                    not lines[0].startswith("#!")):
                return sc_fileref

            new_scatter = join(self.build_dir, ".link_script.sct")
            if cur_dir_name is None:
                cur_dir_name = dirname(sc_fileref.path)

                # For a chrooted system, adjust the path to the scatter file to be a valid
                # chroot location by removing the first three elements of the path.
                if cur_dir_name.startswith("/tmp/chroots"):
                    cur_dir_name = sep + join(*(cur_dir_name.split(sep)[4:]))

            # Add the relocated scatter file path to the include path.
            self.SHEBANG += " -I%s" % cur_dir_name

            # Only rewrite if doing a full build...
            if self.need_update(new_scatter, [sc_fileref.path]):
                with open(new_scatter, "w") as out:
                    # Write the new shebang line...
                    out.write(self.SHEBANG + "\n")
                    # ...followed by the unmolested remaining content from the original scatter file.
                    out.write("".join(lines[1:]))

            return FileRef(".link_script.sct", new_scatter)

    def get_link_command(
            self,
            output,
            objects,
            libraries,
            lib_dirs,
            scatter_file
    ):
        base, _ = splitext(output)
        map_file = base + ".map"
        args = ["-o", output, "--info=totals", "--map", "--list=%s" % map_file]
        args.extend(objects)
        args.extend(libraries)
        if lib_dirs:
            args.extend(["--userlibpath", ",".join(lib_dirs)])
        if scatter_file:
            scatter_name = relpath(scatter_file)
            new_scatter = self.correct_scatter_shebang(FileRef(scatter_name, scatter_file))
            args.extend(["--scatter", new_scatter.path])

        cmd = self.ld + args

        if self.RESPONSE_FILES:
            cmd_linker = cmd[0]
            link_files = self.get_link_file(cmd[1:])
            cmd = [cmd_linker, '--via', link_files]

        return cmd

    def link(self, output, objects, libraries, lib_dirs, scatter_file):
        cmd = self.get_link_command(
            output, objects, libraries, lib_dirs, scatter_file
        )

        self.notify.cc_verbose("Link: %s" % ' '.join(cmd))
        self.default_cmd(cmd)

    def archive(self, objects, lib_path):
        if self.RESPONSE_FILES:
            param = ['--via', self.get_arch_file(objects)]
        else:
            param = objects
        self.default_cmd([self.ar, '-r', lib_path] + param)

    def get_binary_commands(self, bin_arg, bin, elf):
        return [self.elf2bin, bin_arg, '-o', bin, elf]

    def binary(self, resources, elf, bin):
        _, fmt = splitext(bin)
        # On .hex format, combine multiple .hex files (for multiple load
        # regions) into one
        bin_arg = {".bin": "--bin", ".hex": "--i32combined"}[fmt]
        cmd = self.get_binary_commands(bin_arg, bin, elf)

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
        return "--predefine=\"-D%s=%s\"" % (name, value)

    @staticmethod
    def redirect_symbol(source, sync, build_dir):
        if not exists(build_dir):
            makedirs(build_dir)
        handle, filename = mkstemp(prefix=".redirect-symbol.", dir=build_dir)
        write(handle, "RESOLVE %s AS %s\n" % (source, sync))
        return "--edit=%s" % filename


class ARM_STD(ARM):

    OFFICIALLY_SUPPORTED = True

    def __init__(
            self,
            target,
            notify=None,
            macros=None,
            build_profile=None,
            build_dir=None,
            coverage_patterns=None
    ):
        ARM.__init__(
            self,
            target,
            notify,
            macros,
            build_dir=build_dir,
            build_profile=build_profile,
            coverage_patterns=None
        )
        if int(target.build_tools_metadata["version"]) > 0:
            # check only for ARMC5 because ARM_STD means using ARMC5, and thus
            # supported_toolchains must include ARMC5
            if not set(target.supported_toolchains).intersection(
                    set(("ARMC5", "ARM"))
            ):
                raise NotSupportedException(
                    "ARM compiler 5 support is required for ARM build"
                )
        else:
            if not set(("ARM", "uARM")).intersection(set(
                    target.supported_toolchains
            )):
                raise NotSupportedException(
                    "ARM/uARM compiler support is required for ARM build"
                )


class ARM_MICRO(ARM):

    PATCHED_LIBRARY = False

    OFFICIALLY_SUPPORTED = True

    def __init__(
            self,
            target,
            notify=None,
            macros=None,
            silent=False,
            extra_verbose=False,
            build_profile=None,
            build_dir=None
    ):
        target.default_toolchain = "uARM"
        if int(target.build_tools_metadata["version"]) > 0:
            # At this point we already know that we want to use ARMC5+Microlib
            # so check for if they are supported For, AC6+Microlib we still
            # use ARMC6 class
            if not set(("ARMC5", "uARM")).issubset(set(
                    target.supported_toolchains
            )):
                raise NotSupportedException(
                    "ARM/uARM compiler support is required for ARM build"
                )
        else:
            if not set(("ARM", "uARM")).intersection(set(
                    target.supported_toolchains
            )):
                raise NotSupportedException(
                    "ARM/uARM compiler support is required for ARM build"
                )
        ARM.__init__(
            self,
            target,
            notify,
            macros,
            build_dir=build_dir,
            build_profile=build_profile
        )


class ARMC6(ARM_STD):

    OFFICIALLY_SUPPORTED = False
    SHEBANG = "#! armclang -E --target=arm-arm-none-eabi -x c"
    SUPPORTED_CORES = [
        "Cortex-M0", "Cortex-M0+", "Cortex-M3", "Cortex-M4",
        "Cortex-M4F", "Cortex-M7", "Cortex-M7F", "Cortex-M7FD",
        "Cortex-M23", "Cortex-M23-NS", "Cortex-M33", "Cortex-M33F",
        "Cortex-M33-NS", "Cortex-M33F-NS", "Cortex-M33FE-NS", "Cortex-M33FE",
        "Cortex-A9"
    ]
    ARMCC_RANGE = (LooseVersion("6.10"), LooseVersion("7.0"))
    LD_DIAGNOSTIC_PATTERN = re.compile(
        '(?P<severity>Warning|Error): (?P<message>.+)'
    )
    DIAGNOSTIC_PATTERN = re.compile('((?P<file>[^:]+):(?P<line>\d+):)(?P<col>\d+):? (?P<severity>warning|[eE]rror|fatal error): (?P<message>.+)')

    @staticmethod
    def check_executable():
        return mbedToolchain.generic_check_executable("ARMC6", "armclang", 1)

    def __init__(self, target, *args, **kwargs):
        mbedToolchain.__init__(self, target, *args, **kwargs)
        if target.core not in self.SUPPORTED_CORES:
            raise NotSupportedException(
                "this compiler does not support the core %s" % target.core)

        if int(target.build_tools_metadata["version"]) > 0:
            if not set(("ARM", "ARMC6", "uARM")).intersection(set(
                    target.supported_toolchains
            )):
                raise NotSupportedException(
                    "ARM/ARMC6 compiler support is required for ARMC6 build"
                )
        else:
            if not set(("ARM", "ARMC6")).intersection(set(
                    target.supported_toolchains
            )):
                raise NotSupportedException(
                    "ARM/ARMC6 compiler support is required for ARMC6 build"
                )

        toolchain = "arm"

        if should_replace_small_c_lib(target, toolchain):
            target.c_lib = "std"

        self.check_c_lib_supported(target, toolchain)

        if (
            getattr(target, "default_toolchain", "ARMC6") == "uARM"
            or getattr(target, "c_lib", "std") == "small"
        ):
            if "-DMBED_RTOS_SINGLE_THREAD" not in self.flags['common']:
                self.flags['common'].append("-DMBED_RTOS_SINGLE_THREAD")
            if "-D__MICROLIB" not in self.flags['common']:
                self.flags['common'].append("-D__MICROLIB")
            if "--library_type=microlib" not in self.flags['ld']:
                self.flags['ld'].append("--library_type=microlib")
            if "--library_type=microlib" not in self.flags['asm']:
                self.flags['asm'].append("--library_type=microlib")

        self.check_and_add_minimal_printf(target)

        if target.is_TrustZone_secure_target:
            if kwargs.get('build_dir', False):
                # Output secure import library
                build_dir = kwargs['build_dir']
                secure_file = join(build_dir, "cmse_lib.o")
                self.flags["ld"] += ["--import_cmse_lib_out=%s" % secure_file]

            # Enable compiler security extensions
            self.flags['cxx'].append("-mcmse")
            self.flags['c'].append("-mcmse")

        if target.is_TrustZone_non_secure_target:
            # Add linking time preprocessor macro DOMAIN_NS
            # (DOMAIN_NS is passed to compiler and assembler via CORTEX_SYMBOLS
            # in mbedToolchain.get_symbols)
            define_string = self.make_ld_define("DOMAIN_NS", "0x1")
            self.flags["ld"].append(define_string)

        core = target.core_without_NS
        cpu = {
            "Cortex-M0+": "cortex-m0plus",
            "Cortex-M4F": "cortex-m4",
            "Cortex-M7F": "cortex-m7",
            "Cortex-M7FD": "cortex-m7",
            "Cortex-M33": "cortex-m33+nodsp",
            "Cortex-M33F": "cortex-m33+nodsp",
            "Cortex-M33E": "cortex-m33",
            "Cortex-M33FE": "cortex-m33"}.get(core, core)

        cpu = cpu.lower()
        self.flags['common'].append("-mcpu=%s" % cpu)
        self.SHEBANG += " -mcpu=%s" % cpu

        # FPU handling
        if core in ["Cortex-M4", "Cortex-M7", "Cortex-M33", "Cortex-M33E"]:
            self.flags['common'].append("-mfpu=none")
        elif core == "Cortex-M4F":
            self.flags['common'].append("-mfpu=fpv4-sp-d16")
            self.flags['common'].append("-mfloat-abi=hard")
        elif core == "Cortex-M7F" or core.startswith("Cortex-M33F"):
            self.flags['common'].append("-mfpu=fpv5-sp-d16")
            self.flags['common'].append("-mfloat-abi=hard")
        elif core == "Cortex-M7FD":
            self.flags['common'].append("-mfpu=fpv5-d16")
            self.flags['common'].append("-mfloat-abi=hard")

        asm_ld_cpu = {
            "Cortex-M0+": "Cortex-M0plus",
            "Cortex-M4": "Cortex-M4.no_fp",
            "Cortex-M4F": "Cortex-M4",
            "Cortex-M7": "Cortex-M7.no_fp",
            "Cortex-M7F": "Cortex-M7.fp.sp",
            "Cortex-M7FD": "Cortex-M7",
            "Cortex-M33": "Cortex-M33.no_dsp.no_fp",
            "Cortex-M33E": "Cortex-M33.no_fp",
            "Cortex-M33F": "Cortex-M33.no_dsp",
            "Cortex-M33FE": "Cortex-M33"}.get(core, core)

        self.flags['asm'].append("--cpu=%s" % asm_ld_cpu)
        self.flags['ld'].append("--cpu=%s" % asm_ld_cpu)

        self.cc = ([join(TOOLCHAIN_PATHS["ARMC6"], "armclang")] +
                   self.flags['common'] + self.flags['c'])
        self.cppc = ([join(TOOLCHAIN_PATHS["ARMC6"], "armclang")] +
                     self.flags['common'] + self.flags['cxx'])
        self.asm = [join(TOOLCHAIN_PATHS["ARMC6"], "armasm")]
        self.asm += self.flags['asm']
        self.ld = [join(TOOLCHAIN_PATHS["ARMC6"], "armlink")]
        self.ld += self.flags['ld']
        self.ar = join(TOOLCHAIN_PATHS["ARMC6"], "armar")
        self.elf2bin = join(TOOLCHAIN_PATHS["ARMC6"], "fromelf")

        # Adding this for safety since this inherits the `version_check`
        # function but does not call the constructor of ARM_STD, so the
        # `product_name` variable is not initialized.
        self.product_name = None

    def _get_toolchain_labels(self):
        if getattr(self.target, "default_toolchain", "ARM") == "uARM":
            return ["ARM", "ARM_MICRO", "ARMC6"]
        else:
            return ["ARM", "ARM_STD", "ARMC6"]

    @property
    def is_mbed_studio_armc6(self):
        return self.product_name and "Mbed Studio" in self.product_name

    def parse_dependencies(self, dep_path):
        return mbedToolchain.parse_dependencies(self, dep_path)

    def is_not_supported_error(self, output):
        return "#error [NOT_SUPPORTED]" in output

    def parse_output(self, output):
        for line in output.splitlines():
            match = self.LD_DIAGNOSTIC_PATTERN.match(line)
            if match is not None:
                self.notify.cc_info({
                    'severity': match.group('severity').lower(),
                    'message': match.group('message'),
                    'text': '',
                    'target_name': self.target.name,
                    'toolchain_name': self.name,
                    'col': 0,
                    'file': "",
                    'line': 0
                })
            match = self.DIAGNOSTIC_PATTERN.search(line)
            if match is not None:
                self.notify.cc_info({
                    'severity': match.group('severity').lower(),
                    'file': match.group('file'),
                    'line': match.group('line'),
                    'col': match.group('col'),
                    'message': match.group('message'),
                    'text': '',
                    'target_name': self.target.name,
                    'toolchain_name': self.name
                })

    def get_config_option(self, config_header):
        return ["-include", config_header]

    def get_compile_options(self, defines, includes, for_asm=False):

        opts = ['-D%s' % d for d in defines]

        if self.RESPONSE_FILES:
            opts += ['@{}'.format(self.get_inc_file(includes))]
        else:
            opts += ["-I%s" % i for i in includes if i]

        config_header = self.get_config_header()
        if config_header:
            opts.extend(self.get_config_option(config_header))
        if for_asm:
            opts = [
                "--cpreproc",
                "--cpreproc_opts=%s" % ",".join(self.flags['common'] + opts)
            ]

        if self.is_mbed_studio_armc6:
            # NOTE: the --ide=mbed argument is only for use with Mbed OS
            opts.insert(0, "--ide=mbed")

        return opts

    def assemble(self, source, object, includes):
        # Preprocess first, then assemble
        root, _ = splitext(object)
        tempfile = root + '.E'

        # Build preprocess assemble command
        cmd_pre = copy(self.cc)
        cmd_pre.extend(self.get_compile_options(
            self.get_symbols(True), includes, for_asm=False))
        cmd_pre.extend(["-E", "-MT", object, "-o", tempfile, source])

        # Build main assemble command
        cmd = self.asm + ["-o", object, tempfile]
        if self.is_mbed_studio_armc6:
            # NOTE: the --ide=mbed argument is only for use with Mbed OS
            cmd.insert(1, "--ide=mbed")

        # Return command array, don't execute
        return [cmd_pre, cmd]

    def compile(self, cc, source, object, includes):
        cmd = copy(cc)
        cmd.extend(self.get_compile_options(self.get_symbols(), includes))
        cmd.extend(["-o", object, source])
        return [cmd]

    def get_link_command(
            self,
            output,
            objects,
            libraries,
            lib_dirs,
            scatter_file
    ):
        cmd = ARM.get_link_command(
            self, output, objects, libraries, lib_dirs, scatter_file
        )

        if self.is_mbed_studio_armc6:
            # NOTE: the --ide=mbed argument is only for use with Mbed OS
            cmd.insert(1, "--ide=mbed")

        return cmd

    def get_binary_commands(self, bin_arg, bin, elf):
        cmd = ARM.get_binary_commands(self, bin_arg, bin, elf)

        if self.is_mbed_studio_armc6:
            # NOTE: the --ide=mbed argument is only for use with Mbed OS
            cmd.insert(1, "--ide=mbed")

        return cmd

