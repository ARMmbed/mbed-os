"""
mbed SDK
Copyright (c) 2011-2017 ARM Limited
Portions Copyright (c) 2017 Analog Devices, Inc.

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

import copy
import os
import sys
import shutil
import tempfile

from subprocess import Popen, PIPE

from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter

class Option:
    """
    Container for CCES option type and value
    """
    def __init__(self, opt_type, value):
        self.type = opt_type
        self.value = value


class CCES(Exporter):
    """
    mbed exporter for Analog Devices' CrossCore Embedded Studio(TM)
    """
    NAME = 'CrossCore Embedded Studio'
    TOOLCHAIN = 'GCC_ARM'

    @classmethod
    def is_target_supported(cls, target_name):
        """Query support for a particular target

        Positional Arguments:
        target_name - the name of the target.
        """
        target = TARGET_MAP[target_name]
        return cls.TOOLCHAIN in target.supported_toolchains

    @property
    def flags(self):
        """Returns a dictionary of toolchain flags.
        Keys of the dictionary are:
        cxx_flags    - c++ flags
        c_flags      - c flags
        ld_flags     - linker flags
        asm_flags    - assembler flags
        common_flags - common options

        Skip macros because headless tools handles them separately
        """
        config_header = self.toolchain.get_config_header()
        flags = {key + "_flags": copy.deepcopy(value) for key, value
                    in self.toolchain.flags.iteritems()}
        if config_header:
            config_header = os.path.relpath(config_header,
                                self.resources.file_basepath[config_header])
            config_header = "\\\"" + self.format_path(config_header) + "\\\""
            header_options = self.toolchain.get_config_option(config_header)
            flags['c_flags'] += header_options
            flags['cxx_flags'] += header_options
        return flags

    @staticmethod
    def format_path(path):
        """
        Formats the given path relative to the project directory
        """
        return os.path.abspath(path).replace("\\", "\\\\")

    @staticmethod
    def clean_flags(container, flags):
        """
        Some flags are handled by CCES already, so there's no need
        to include them twice.
        """
        for flag in container:
            if flag in flags:
                flags.remove(flag)

    @staticmethod
    def convert_common_options(prefix, options, flags):
        """
        Converts the common flags into CCES options and removes them
        from the flags list
        """
        # remove these flags without converting to option
        # since they are added by CCES
        remove = [ "-c" ]
        for flag in remove:
            if flag in flags:
                flags.remove(flag)

        value = prefix + "option.dwarfversion.enumerated.v2"
        for flag in flags:
            if flag.startswith("-gdwarf"):
                value = prefix + "option.dwarfversion.enumerated.v" + flag[-1]
                flags.remove(flag)
                break
        option = Option("baseId", value)
        options[prefix + "option.dwarfversion"] = option

    @staticmethod
    def convert_assembler_options(flags):
        """
        Converts the assembler flags into CCES options and removes them
        from the flags list
        """
        options = {}

        # remove these flags without converting to option
        # since they are added by CCES
        remove = [ "-x", "assembler-with-cpp" ]
        for flag in remove:
            if flag in flags:
                flags.remove(flag)

        booleans = {"-v": "arm.assembler.option.verbose",
                    "-g": "arm.assembler.option.debuginfo"}

        for flag in booleans:
            value = "false"
            if flag in flags:
                value = "true"
                flags.remove(flag)
            option = Option("baseId", value)
            options[booleans[flag]] = option

        CCES.convert_common_options("arm.assembler.", options, flags)

        return options

    @staticmethod
    def convert_compiler_options(flags):
        """
        Converts the compiler flags into CCES options and removes them
        from the flags list
        """
        options  = {}

        enable_optimization = "true"
        value = "arm.base.compiler.option.optimization.og"
        for flag in flags:
            if flag.startswith("-O"):
                value = "arm.base.compiler.option.optimization.o" + flag[2:]
                if flag[2:] == "0":
                    enable_optimization = "false"
                flags.remove(flag)
                break
        option = Option("baseId", value)
        options["arm.base.compiler.option.optimization"] = option

        option = Option("baseId", enable_optimization)
        options["arm.base.compiler.option.optimization.enable"] = option

        booleans = {"-g": "arm.base.compiler.option.debug",
                    "-save-temps": "arm.base.compiler.option.savetemps",
                    "-ffunction-sections": "arm.c.compiler.option.elimination.code",
                    "-fdata-sections": "arm.c.compiler.option.elimination.data",
                    "-pedantic": "arm.base.compiler.option.pedantic",
                    "-pedantic-errors": "arm.base.compiler.option.pedanticerrors",
                    "-w": "arm.base.compiler.option.inhibitallwarnings",
                    "-Wall": "arm.base.compiler.option.allwarnings",
                    "-Wextra": "arm.base.compiler.option.extrawarnings",
                    "-Werror": "arm.base.compiler.option.warningaserror",
                    "-Wconversion": "arm.base.compiler.option.conversionwarning"}

        for flag in booleans:
            value = "false"
            if flag in flags:
                value = "true"
                flags.remove(flag)
            option = Option("baseId", value)
            options[booleans[flag]] = option

        CCES.convert_common_options("arm.base.compiler.", options, flags)

        return options

    @staticmethod
    def convert_linker_options(flags):
        """
        Converts the linker flags into CCES options and removes them
        from the flags list
        """
        options = {}

        booleans = {"-nostartfiles": "arm.linker.option.nostart",
                    "-nodefaultlibs": "arm.linker.option.nodeflibs",
                    "-nostdlib": "arm.linker.option.nostdlibs",
                    "-s": "arm.linker.option.strip",
                    "-Wl,--gc-sections": "arm.linker.option.elimination"}

        for flag in booleans:
            value = "false"
            if flag in flags:
                value = "true"
                flags.remove(flag)
            option = Option("baseId", value)
            options[booleans[flag]] = option

        return options

    # override
    def generate(self):
        """
        Generate the CCES project files using headless builder.
        """

        self.resources.win_to_unix()

        t = TARGET_MAP[self.target]

        proc = t.device_name
        fpu = None
        float_abi = None

        if t.core == "Cortex-M4F":
            cpu = "cortex-m4"
            fpu = "fpv4-sp-d16"
            float_abi = "softfp"
        else:
            cpu = t.core.lower()

        asm_defines = self.toolchain.get_symbols(True)
        c_defines = self.toolchain.get_symbols()

        include_dirs = [self.format_path(d) for d in self.resources.inc_dirs if d]

        srcs = self.resources.s_sources + \
                self.resources.c_sources + \
                self.resources.cpp_sources + \
                self.resources.headers

        srcs_dict = {}
        for src in srcs:
            srcs_dict[src] = self.format_path(src)

        ld_script = self.format_path(self.resources.linker_script)

        asm_flags = self.flags['asm_flags']
        c_flags = self.flags['c_flags'] + self.flags['common_flags']
        cxx_flags = self.flags['cxx_flags'] + self.flags['common_flags']

        libs = []
        for libpath in self.resources.libraries:
            lib, ext = os.path.splitext(os.path.basename(libpath))
            libs.append(lib[3:]) # skip 'lib' prefix

        system_libs = [
            '-lstdc++', '-lsupc++', '-lc', '-lgcc'
            ]

        ld_flags = self.flags['ld_flags'] + system_libs

        flags = ["-mcpu="+cpu, "-mthumb"]
        if fpu is not None:
            flags.append("-mfpu="+fpu)
            flags.append("-mfloat-abi="+float_abi)

        self.clean_flags(c_flags, flags)
        self.clean_flags(cxx_flags, flags)
        self.clean_flags(ld_flags, flags)

        ld_opts = self.convert_linker_options(ld_flags)
        asm_opts = self.convert_assembler_options(asm_flags)
        c_opts = self.convert_compiler_options(c_flags)
        cxx_opts = self.convert_compiler_options(cxx_flags)

        workspace = tempfile.mkdtemp()
        project = "cces"
        json = "cces.json"
        local_location = self.format_path(project)

        jinja_ctx = {
            'project' : self.project_name,
            'cpu' : cpu,
            'proc' : proc,
            'family' : "ARM",
            'asm_defines' : asm_defines,
            'c_defines' : c_defines,
            'fpu' : fpu,
            'float_abi' : float_abi,
            'ld_script' : ld_script,
            'local_location' : local_location,
            'srcs': srcs_dict,
            'include_dirs' : include_dirs,
            'ld_opts' : ld_opts,
            'ld_flags' : ld_flags,
            'asm_opts' : asm_opts,
            'asm_flags' : asm_flags,
            'c_opts' : c_opts,
            'c_flags' : c_flags,
            'cxx_opts' : cxx_opts,
            'cxx_flags' : cxx_flags,
        }

        self.gen_file('cces/cces.json.tmpl', jinja_ctx,
                      json, trim_blocks=True, lstrip_blocks=True)

        # import .json file using CCES headless builder
        cces_path = os.getenv("CCES_HOME")
        if cces_path is None:
            print "Failed to export project: 'CCES_HOME' environment variable not defined."
            return -1

        if sys.platform == 'win32' or sys.platform == 'cygwin':
            cces_path = os.path.join(cces_path, "Eclipse", "ccesc.exe")
        elif sys.platform.startswith('linux'):
            cces_path = os.path.join(cces_path, "Eclipse", "cces")
        elif sys.platform == 'darwin':
            cces_path = os.path.join(cces_path, "MacOS", "cces")
        else:
            print "Unsupported operating system '%s'" % sys.platform
            return -1

        cmd = [
            "\"%s\"" % cces_path,
            "-nosplash",
            "-consoleLog",
            "-application com.analog.crosscore.headlesstools",
            "-data", workspace,
            "-project", os.path.join(self.export_dir, project),
            "-command projectcreate",
            "-input-file", os.path.join(self.export_dir, json)
        ]
        print ' '.join(cmd)
        p = Popen(' '.join(cmd), shell=True, stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        rc = p.returncode

        # cleanup workspace
        if os.path.exists(workspace):
            shutil.rmtree(workspace)

        # check return code for failure
        if rc == 0:
            return 0

        for line in out.split("\n"):
            print line
        for line in err.split("\n"):
            print line

        print "Failed to export project. Return code: %d" % rc
        return -1

    @staticmethod
    def build(project_name, log_name='build_log.txt', cleanup=True):
        """
        Build the generated CCES project using headless builder.
        """
        workspace = tempfile.mkdtemp()
        project = "cces"
        cces_path = os.getenv("CCES_HOME")
        if cces_path is None:
            print "Failed to build project: 'CCES_HOME' environment variable not defined."
            return -1

        if sys.platform == 'win32' or sys.platform == 'cygwin':
            cces_path = os.path.join(cces_path, "Eclipse", "ccesc.exe")
        elif sys.platform.startswith('linux'):
            cces_path = os.path.join(cces_path, "Eclipse", "cces")
        elif sys.platform == 'darwin':
            cces_path = os.path.join(cces_path, "MacOS", "cces")
        else:
            print "Unsupported operating system '%s'" % sys.platform
            return -1

        cmd = [
            "\"%s\"" % cces_path,
            "-nosplash",
            "-consoleLog",
            "-application com.analog.crosscore.headlesstools",
            "-data", workspace,
            "-project", project,
            "-cleanBuild all"
        ]
        p = Popen(' '.join(cmd), shell=True, stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        rc = p.returncode

        if log_name:
            with open(log_name, 'w+') as f:
                f.write(out)
                f.write(err)
                if rc != 0:
                    f.write("Failed to build project. Return code: %d" % rc)

        # cleanup workspace
        if os.path.exists(workspace):
            shutil.rmtree(workspace)

        # check return code for failure
        if rc == 0:
            return 0

        print out
        print err

        print "Failed to build project. Return code: %d" % rc
        return -1
