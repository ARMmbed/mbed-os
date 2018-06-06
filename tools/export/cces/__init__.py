"""
mbed SDK
Copyright (c) 2011-2017 ARM Limited
Portions Copyright (c) 2017-2018 Analog Devices, Inc.

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

from collections import namedtuple


# Container for CCES option type and value
Option = namedtuple('Option', ['type', 'value'])

"""
Tuple of supported device names
"""
SUPPORTED_DEVICES = ("ADuCM3027", "ADuCM3029", "ADuCM360", "ADuCM4050")

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
        return (cls.TOOLCHAIN in target.supported_toolchains) \
            and hasattr(target, "device_name") \
            and (target.device_name in SUPPORTED_DEVICES)

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
        flags = {key + "_flags": copy.deepcopy(value) for key, value \
                    in self.toolchain.flags.iteritems()}
        if config_header:
            config_header = os.path.relpath(config_header, \
                                self.resources.file_basepath[config_header])
            config_header = "\\\"" + self.format_inc_path(config_header) \
                                + "\\\""
            header_options = self.toolchain.get_config_option(config_header)
            flags['c_flags'] += header_options
            flags['cxx_flags'] += header_options
        return flags

    @staticmethod
    def format_path(path, prefix):
        """
        Formats the given source path relative to the project directory
        using the prefix
        """
        new_path = path
        if new_path.startswith("./"):
            new_path = new_path[2:]
        return prefix + new_path.replace("\\", "\\\\")

    @staticmethod
    def format_inc_path(path):
        """
        Formats the given include path relative to the project directory
        """
        return CCES.format_path(path, "${ProjDirPath}/../")

    @staticmethod
    def format_src_path(path):
        """
        Formats the given source path relative to the project directory
        """
        return CCES.format_path(path, "PARENT-1-PROJECT_LOC/")

    @staticmethod
    def clean_flags(container, flags_to_remove):
        """
        Some flags are handled by CCES already, so there's no need
        to include them twice.
        """
        for flag in flags_to_remove:
            if flag in container:
                container.remove(flag)

    @staticmethod
    def parse_flags(flags, options, booleans):
        """
        Parse the values in `booleans`, insert them into the
        `options` dictionary and remove them from `flags`
        """
        for flag, flag_id in booleans.items():
            value = "false"
            if flag in flags:
                value = "true"
                flags.remove(flag)
            options[flag_id] = Option("baseId", value)

    @staticmethod
    def convert_common_options(prefix, options, flags):
        """
        Converts the common flags into CCES options and removes them
        from the flags list
        """
        # remove these flags without converting to option
        # since they are added by CCES
        remove = ["-c"]
        CCES.clean_flags(flags, remove)

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
        remove = ["-x", "assembler-with-cpp"]
        CCES.clean_flags(flags, remove)

        booleans = {"-v": "arm.assembler.option.verbose",
                    "-g": "arm.assembler.option.debuginfo"}

        CCES.parse_flags(flags, options, booleans)

        CCES.convert_common_options("arm.assembler.", options, flags)

        return options

    @staticmethod
    def convert_compiler_options(flags):
        """
        Converts the compiler flags into CCES options and removes them
        from the flags list
        """
        options = {}

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
                    "-save-temps": \
                        "arm.base.compiler.option.savetemps",
                    "-ffunction-sections": \
                        "arm.c.compiler.option.elimination.code",
                    "-fdata-sections": \
                        "arm.c.compiler.option.elimination.data",
                    "-pedantic": "arm.base.compiler.option.pedantic",
                    "-pedantic-errors": \
                        "arm.base.compiler.option.pedanticerrors",
                    "-w": "arm.base.compiler.option.inhibitallwarnings",
                    "-Wall": "arm.base.compiler.option.allwarnings",
                    "-Wextra": "arm.base.compiler.option.extrawarnings",
                    "-Werror": "arm.base.compiler.option.warningaserror",
                    "-Wconversion": \
                        "arm.base.compiler.option.conversionwarning"}

        CCES.parse_flags(flags, options, booleans)

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

        CCES.parse_flags(flags, options, booleans)

        return options

    @staticmethod
    def get_cces_path(root):
        """
        Returns the path to the CCES executable
        """
        cces_path = None

        if sys.platform == 'win32' or sys.platform == 'cygwin':
            cces_path = os.path.join(root, "Eclipse", "ccesc.exe")
        elif sys.platform.startswith('linux'):
            cces_path = os.path.join(root, "Eclipse", "cces")
        elif sys.platform == 'darwin':
            cces_path = os.path.join(root, "MacOS", "cces")
        else:
            print("Unsupported operating system '%s'" % sys.platform)
            return None

        return cces_path

    @staticmethod
    def get_project_create_command(cces_path, workspace, project_name):
        """
        Generate the headless tools projectcreate command string
        with the given parameters
        """
        cmd = [
            "\"%s\"" % cces_path,
            "-nosplash",
            "-consoleLog",
            "-application com.analog.crosscore.headlesstools",
            "-data", workspace,
            "-project", project_name,
            "-command projectcreate",
            "-input-file", "cces.json"
        ]
        return ' '.join(cmd)

    @staticmethod
    def get_project_build_command(cces_path, workspace, project_name):
        """
        Generate the headless tools build command string
        with the given parameters
        """
        cmd = [
            "\"%s\"" % cces_path,
            "-nosplash",
            "-consoleLog",
            "-application com.analog.crosscore.headlesstools",
            "-data", workspace,
            "-project", project_name,
            "-cleanBuild all"
        ]
        return ' '.join(cmd)

    # override
    def generate(self):
        """
        Generate the CCES project files using headless builder.
        """

        self.resources.win_to_unix()

        asm_defines = self.toolchain.get_symbols(True)
        c_defines = self.toolchain.get_symbols()

        include_dirs = [self.format_inc_path(d) for d \
                        in self.resources.inc_dirs if d]

        srcs = self.resources.s_sources + \
                self.resources.c_sources + \
                self.resources.cpp_sources + \
                self.resources.headers

        srcs_dict = {}
        for src in srcs:
            srcs_dict[src] = self.format_src_path(src)

        ld_script = self.format_inc_path(self.resources.linker_script)

        asm_flags = self.flags['asm_flags']
        c_flags = self.flags['c_flags'] + self.flags['common_flags']
        cxx_flags = self.flags['cxx_flags'] + self.flags['common_flags']

        libs = []
        for libpath in self.resources.libraries:
            lib = os.path.splitext(os.path.basename(libpath))[0]
            libs.append(lib[3:]) # skip 'lib' prefix

        ld_flags = self.flags['ld_flags'] + ["-l" + lib for lib \
                    in self.toolchain.sys_libs]

        proc = self.toolchain.target.device_name
        cpu = self.toolchain.target.core.lower()
        fpu = None
        float_abi = None

        # parse toolchain CPU flags
        for flag in self.toolchain.cpu:
            if flag.startswith("-mcpu="):
                cpu = flag[len("-mcpu="):]
            elif flag.startswith("-mfpu="):
                fpu = flag[len("-mfpu="):]
            elif flag.startswith("-mfloat-abi="):
                float_abi = flag[len("-mfloat-abi="):]

        # remove toolchain CPU flags. We'll handle them separately
        # in the generated .json file
        self.clean_flags(c_flags, self.toolchain.cpu)
        self.clean_flags(cxx_flags, self.toolchain.cpu)
        self.clean_flags(ld_flags, self.toolchain.cpu)

        ld_opts = self.convert_linker_options(ld_flags)
        asm_opts = self.convert_assembler_options(asm_flags)
        c_opts = self.convert_compiler_options(c_flags)
        cxx_opts = self.convert_compiler_options(cxx_flags)

        project = "cces"
        json = "cces.json"
        local_location = project

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

        # generate a readme on how to create the CCES project
        # using the generated .json file

        cces_paths = {
            "Windows" : "%CCES_HOME%\\Eclipse\\ccesc.exe",
            "Linux" : "${CCES_HOME}/Eclipse/cces",
            "MacOS" : "${CCES_HOME}/MacOS/cces"
        }

        commands = {"create":{}, "build":{}}
        for operating_system, path in cces_paths.items():
            commands["create"][operating_system] = \
                                CCES.get_project_create_command(path, \
                                "WORKSPACE", project)
            commands["build"][operating_system] = \
                                CCES.get_project_build_command(path, \
                                "WORKSPACE", project)

        jinja_ctx = {
            'commands' : commands
        }

        self.gen_file('cces/README.md.tmpl', jinja_ctx, "README.md")

        print("CCES files generated.")


    @staticmethod
    def clean(_):
        os.remove('cces.json')
        os.remove('README.md')

    @staticmethod
    def build(project_name, log_name='build_log.txt', cleanup=True):
        """
        Build the generated CCES project using headless builder.
        """
        # create the project by importing .json file using CCES headless builder
        cces_home = os.getenv("CCES_HOME")
        if cces_home is None:
            print("Failed to build project: " + \
                "'CCES_HOME' environment variable not defined.")
            return -1

        cces_path = CCES.get_cces_path(cces_home)
        if cces_path is None:
            return -1

        workspace = tempfile.mkdtemp()

        cmd = CCES.get_project_create_command(cces_path, workspace, \
                project_name)
        print(cmd)
        process = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
        out, err = process.communicate()
        ret_code = process.returncode

        # cleanup workspace
        if os.path.exists(workspace):
            shutil.rmtree(workspace, True)
            CCES.clean(project_name)

        # check return code for failure
        if ret_code != 0:
            for line in out.split("\n"):
                print(line)
            for line in err.split("\n"):
                print(line)

            print("Failed to create project. Return code: %d" % ret_code)
            return -1

        # build the project
        workspace = tempfile.mkdtemp()

        cmd = CCES.get_project_build_command(cces_path, workspace, project_name)
        print(cmd)
        process = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
        out, err = process.communicate()
        ret_code = process.returncode

        if log_name:
            with open(log_name, 'w+') as log_file:
                log_file.write(out)
                log_file.write(err)
                if ret_code != 0:
                    log_file.write("Failed to build project. Return code: %d"\
                                    % ret_code)

        # cleanup workspace
        if os.path.exists(workspace):
            shutil.rmtree(workspace)

        # check return code for failure
        if ret_code == 0:
            return 0

        for line in out.split("\n"):
            print(line)
        for line in err.split("\n"):
            print(line)

        print("Failed to build project. Return code: %d" % ret_code)
        return -1
