"""
mbed SDK
Copyright (c) 2011-2017 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Title: GNU ARM Eclipse (http://gnuarmeclipse.github.io) exporter.

Description: Creates a managed build project that can be imported by 
the GNU ARM Eclipse plug-ins.

Author: Liviu Ionescu <ilg@livius.net>
"""
from __future__ import print_function, absolute_import
from builtins import str

import os
import copy
import tempfile
import shutil
import copy

from subprocess import call, Popen, PIPE
from os.path import splitext, basename, relpath, dirname, exists, join, dirname
from random import randint
from json import load

from tools.export.exporters import Exporter, apply_supported_whitelist
from tools.options import list_profiles
from tools.targets import TARGET_MAP
from tools.utils import NotSupportedException
from tools.build_api import prepare_toolchain

# =============================================================================


class UID:
    """
    Helper class, used to generate unique ids required by .cproject symbols.
    """
    @property
    def id(self):
        return "%0.9u" % randint(0, 999999999)

# Global UID generator instance.
# Passed to the template engine, and referred as {{u.id}}.
# Each invocation generates a new number.
u = UID()

# =============================================================================


POST_BINARY_WHITELIST = set([
    "TEENSY3_1Code.binary_hook",
    "MCU_NRF51Code.binary_hook",
    "LPCTargetCode.lpc_patch",
    "LPC4088Code.binary_hook"
])

class GNUARMEclipse(Exporter):
    NAME = 'GNU ARM Eclipse'
    TOOLCHAIN = 'GCC_ARM'

    @classmethod
    def is_target_supported(cls, target_name):
        target = TARGET_MAP[target_name]
        return apply_supported_whitelist(
            cls.TOOLCHAIN, POST_BINARY_WHITELIST, target)

    # override
    @property
    def flags(self):
        """Returns a dictionary of toolchain flags.
        Keys of the dictionary are:
        cxx_flags    - c++ flags
        c_flags      - c flags
        ld_flags     - linker flags
        asm_flags    - assembler flags
        common_flags - common options

        The difference from the parent function is that it does not
        add macro definitions, since they are passed separately.
        """

        config_header = self.toolchain.get_config_header()
        flags = {key + "_flags": copy.deepcopy(value) for key, value
                 in self.toolchain.flags.items()}
        if config_header:
            config_header = relpath(config_header,
                                    self.resources.file_basepath[config_header])
            flags['c_flags'] += self.toolchain.get_config_option(config_header)
            flags['cxx_flags'] += self.toolchain.get_config_option(
                config_header)
        return flags

    def toolchain_flags(self, toolchain):
        """Returns a dictionary of toolchain flags.
        Keys of the dictionary are:
        cxx_flags    - c++ flags
        c_flags      - c flags
        ld_flags     - linker flags
        asm_flags    - assembler flags
        common_flags - common options

        The difference from the above is that it takes a parameter.
        """

        # Note: use the config options from the currently selected toolchain.
        config_header = self.toolchain.get_config_header()

        flags = {key + "_flags": copy.deepcopy(value) for key, value
                 in toolchain.flags.items()}
        if config_header:
            config_header = relpath(config_header,
                                    self.resources.file_basepath[config_header])
            header_options = self.toolchain.get_config_option(config_header)
            flags['c_flags'] += header_options
            flags['cxx_flags'] += header_options
        return flags

    def validate_resources(self):
        if not self.resources.linker_script:
            raise NotSupportedException("No linker script found.")

    def create_jinja_ctx(self):

        self.validate_resources()

        self.resources.win_to_unix()

        # TODO: use some logger to display additional info if verbose

        libraries = []
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])

        self.system_libraries = [
            'stdc++', 'supc++', 'm', 'c', 'gcc', 'nosys'
        ]

        # Read in all profiles, we'll extract compiler options.
        profiles = self.get_all_profiles()

        profile_ids = [s.lower() for s in profiles]
        profile_ids.sort()

        # TODO: get the list from existing .cproject
        build_folders = [s.capitalize() for s in profile_ids]
        build_folders.append('BUILD')

        objects = [self.filter_dot(s) for s in self.resources.objects]
        for bf in build_folders:
            objects = [o for o in objects if not o.startswith(bf + '/')]

        self.compute_exclusions()

        self.include_path = [
            self.filter_dot(s) for s in self.resources.inc_dirs]

        self.as_defines = self.toolchain.get_symbols(True)
        self.c_defines = self.toolchain.get_symbols()
        self.cpp_defines = self.c_defines

        self.ld_script = self.filter_dot(
            self.resources.linker_script)

        self.options = {}
        for id in profile_ids:

            # There are 4 categories of options, a category common too
            # all tools and a specific category for each of the tools.
            opts = {}
            opts['common'] = {}
            opts['as'] = {}
            opts['c'] = {}
            opts['cpp'] = {}
            opts['ld'] = {}

            opts['id'] = id
            opts['name'] = opts['id'].capitalize()


            profile = profiles[id]

            # A small hack, do not bother with src_path again,
            # pass an empty string to avoid crashing.
            src_paths = ['']
            target_name = self.toolchain.target.name
            toolchain = prepare_toolchain(
                src_paths, "", target_name, self.TOOLCHAIN, build_profile=[profile])

            # Hack to fill in build_dir
            toolchain.build_dir = self.toolchain.build_dir
            toolchain.config = self.toolchain.config
            toolchain.set_config_data(self.toolchain.config.get_config_data())

            flags = self.toolchain_flags(toolchain)

            # Most GNU ARM Eclipse options have a parent,
            # either debug or release.
            if '-O0' in flags['common_flags'] or '-Og' in flags['common_flags']:
                opts['parent_id'] = 'debug'
            else:
                opts['parent_id'] = 'release'

            self.process_options(opts, flags)

            opts['as']['defines'] = self.as_defines
            opts['c']['defines'] = self.c_defines
            opts['cpp']['defines'] = self.cpp_defines

            opts['common']['include_paths'] = self.include_path
            opts['common']['excluded_folders'] = '|'.join(
                self.excluded_folders)

            opts['ld']['library_paths'] = [
                self.filter_dot(s) for s in self.resources.lib_dirs]

            opts['ld']['object_files'] = objects
            opts['ld']['user_libraries'] = libraries
            opts['ld']['system_libraries'] = self.system_libraries
            opts['ld']['script'] = join(id.capitalize(),
                                        "linker-script-%s.ld" % id)
            opts['cpp_cmd'] = '"{}"'.format(toolchain.preproc[0]) + " " + " ".join(toolchain.preproc[1:])

            # Unique IDs used in multiple places.
            # Those used only once are implemented with {{u.id}}.
            uid = {}
            uid['config'] = u.id
            uid['tool_c_compiler'] = u.id
            uid['tool_c_compiler_input'] = u.id
            uid['tool_cpp_compiler'] = u.id
            uid['tool_cpp_compiler_input'] = u.id

            opts['uid'] = uid

            self.options[id] = opts

        jinja_ctx = {
            'name': self.project_name,
            'ld_script': self.ld_script,

            # Compiler & linker command line options
            'options': self.options,

            # Must be an object with an `id` property, which
            # will be called repeatedly, to generate multiple UIDs.
            'u': u,
        }
        return jinja_ctx

    # override
    def generate(self):
        """
        Generate the .project and .cproject files.
        """
        jinja_ctx = self.create_jinja_ctx()


        self.gen_file('gnuarmeclipse/.project.tmpl', jinja_ctx,
                      '.project', trim_blocks=True, lstrip_blocks=True)
        self.gen_file('gnuarmeclipse/.cproject.tmpl', jinja_ctx,
                      '.cproject', trim_blocks=True, lstrip_blocks=True)
        self.gen_file('gnuarmeclipse/makefile.targets.tmpl', jinja_ctx,
                      'makefile.targets', trim_blocks=True, lstrip_blocks=True)
        self.gen_file_nonoverwrite('gnuarmeclipse/mbedignore.tmpl', jinja_ctx,
                                   '.mbedignore')

        print('Done. Import the \'{0}\' project in Eclipse.'.format(self.project_name))

    @staticmethod
    def clean(_):
        os.remove('.project')
        os.remove('.cproject')
        if exists('Debug'):
            shutil.rmtree('Debug')
        if exists('Release'):
            shutil.rmtree('Release')
        if exists('makefile.targets'):
            os.remove('makefile.targets')

    # override
    @staticmethod
    def build(project_name, log_name="build_log.txt", cleanup=True):
        """
        Headless build an Eclipse project.

        The following steps are performed:
        - a temporary workspace is created, 
        - the project is imported,
        - a clean build of all configurations is performed and 
        - the temporary workspace is removed.

        The build results are in the Debug & Release folders.

        All executables (eclipse & toolchain) must be in the PATH.

        The general method to start a headless Eclipse build is:

        $ eclipse \
        --launcher.suppressErrors \
        -nosplash \
        -application org.eclipse.cdt.managedbuilder.core.headlessbuild \
        -data /path/to/workspace \
        -import /path/to/project \
        -cleanBuild "project[/configuration] | all"
        """

        # TODO: possibly use the log file.

        # Create a temporary folder for the workspace.
        tmp_folder = tempfile.mkdtemp()

        cmd = [
            'eclipse',
            '--launcher.suppressErrors',
            '-nosplash',
            '-application org.eclipse.cdt.managedbuilder.core.headlessbuild',
            '-data', tmp_folder,
            '-import', os.getcwd(),
            '-cleanBuild', project_name
        ]

        p = Popen(' '.join(cmd), shell=True, stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        ret_code = p.returncode
        stdout_string = "=" * 10 + "STDOUT" + "=" * 10 + "\n"
        err_string = "=" * 10 + "STDERR" + "=" * 10 + "\n"
        err_string += err

        ret_string = "SUCCESS\n"
        if ret_code != 0:
            ret_string += "FAILURE\n"

        print("%s\n%s\n%s\n%s" % (stdout_string, out, err_string, ret_string))

        if log_name:
            # Write the output to the log file
            with open(log_name, 'w+') as f:
                f.write(stdout_string)
                f.write(out)
                f.write(err_string)
                f.write(ret_string)

        # Cleanup the exported and built files
        if cleanup:
            if exists(log_name):
                os.remove(log_name)

        # Always remove the temporary folder.
        if exists(tmp_folder):
            shutil.rmtree(tmp_folder)

        if ret_code == 0:
            # Return Success
            return 0

        # Seems like something went wrong.
        return -1

   # -------------------------------------------------------------------------

    @staticmethod
    def get_all_profiles():
        tools_path = dirname(dirname(dirname(__file__)))
        file_names = [join(tools_path, "profiles", fn) for fn in os.listdir(
            join(tools_path, "profiles")) if fn.endswith(".json")]


        profile_names = [basename(fn).replace(".json", "")
                         for fn in file_names]

        profiles = {}

        for fn in file_names:
            content = load(open(fn))
            profile_name = basename(fn).replace(".json", "")
            profiles[profile_name] = content

        return profiles

    # -------------------------------------------------------------------------
    # Process source files/folders exclusions.

    def compute_exclusions(self):
        """
        With the project root as the only source folder known to CDT,
        based on the list of source files, compute the folders to not
        be included in the build.

        The steps are:
        - get the list of source folders, as dirname(source_file)
        - compute the top folders (subfolders of the project folder)
        - iterate all subfolders and add them to a tree, with all 
        nodes markes as 'not used'
        - iterate the source folders and mark them as 'used' in the
        tree, including all intermediate nodes
        - recurse the tree and collect all unused folders; descend
        the hierarchy only for used nodes
        """
        source_folders = [self.filter_dot(s) for s in set(dirname(
            src) for src in self.resources.c_sources + self.resources.cpp_sources + self.resources.s_sources)]

        self.excluded_folders = set(self.resources.ignored_dirs) - set(self.resources.inc_dirs)


    # -------------------------------------------------------------------------

    def dump_tree(self, nodes, depth=0):
        for k in nodes.keys():
            node = nodes[k]
            parent_name = node['parent'][
                'name'] if 'parent' in node.keys() else ''
            if len(node['children'].keys()) != 0:
                self.dump_tree(node['children'], depth + 1)

    def dump_paths(self, nodes, depth=0):
        for k in nodes.keys():
            node = nodes[k]
            parts = []
            while True:
                parts.insert(0, node['name'])
                if 'parent' not in node:
                    break
                node = node['parent']
            path = '/'.join(parts)
            self.dump_paths(nodes[k]['children'], depth + 1)

    # -------------------------------------------------------------------------

    def process_options(self, opts, flags_in):
        """
        CDT managed projects store lots of build options in separate
        variables, with separate IDs in the .cproject file.
        When the CDT build is started, all these options are brought
        together to compose the compiler and linker command lines.

        Here the process is reversed, from the compiler and linker
        command lines, the options are identified and various flags are
        set to control the template generation process.

        Once identified, the options are removed from the command lines.

        The options that were not identified are options that do not 
        have CDT equivalents and will be passed in the 'Other options' 
        categories.

        Although this process does not have a very complicated logic,
        given the large number of explicit configuration options
        used by the GNU ARM Eclipse managed build plug-in, it is tedious...
        """

        # Make a copy of the flags, to be one by one removed after processing.
        flags = copy.deepcopy(flags_in)

        # Initialise the 'last resort' options where all unrecognised
        # options will be collected.
        opts['as']['other'] = ''
        opts['c']['other'] = ''
        opts['cpp']['other'] = ''
        opts['ld']['other'] = ''

        MCPUS = {
            'Cortex-M0': {'mcpu': 'cortex-m0', 'fpu_unit': None},
            'Cortex-M0+': {'mcpu': 'cortex-m0plus', 'fpu_unit': None},
            'Cortex-M1': {'mcpu': 'cortex-m1', 'fpu_unit': None},
            'Cortex-M3': {'mcpu': 'cortex-m3', 'fpu_unit': None},
            'Cortex-M4': {'mcpu': 'cortex-m4', 'fpu_unit': None},
            'Cortex-M4F': {'mcpu': 'cortex-m4', 'fpu_unit': 'fpv4spd16'},
            'Cortex-M7': {'mcpu': 'cortex-m7', 'fpu_unit': None},
            'Cortex-M7F': {'mcpu': 'cortex-m7', 'fpu_unit': 'fpv4spd16'},
            'Cortex-M7FD': {'mcpu': 'cortex-m7', 'fpu_unit': 'fpv5d16'},
            'Cortex-A9': {'mcpu': 'cortex-a9', 'fpu_unit': 'vfpv3'}
        }

        # Remove options that are supplied by CDT
        self.remove_option(flags['common_flags'], '-c')
        self.remove_option(flags['common_flags'], '-MMD')

        # As 'plan B', get the CPU from the target definition.
        core = self.toolchain.target.core

        opts['common']['arm.target.family'] = None

        # cortex-m0, cortex-m0-small-multiply, cortex-m0plus,
        # cortex-m0plus-small-multiply, cortex-m1, cortex-m1-small-multiply,
        # cortex-m3, cortex-m4, cortex-m7.
        str = self.find_options(flags['common_flags'], '-mcpu=')
        if str != None:
            opts['common']['arm.target.family'] = str[len('-mcpu='):]
            self.remove_option(flags['common_flags'], str)
            self.remove_option(flags['ld_flags'], str)
        else:
            if core not in MCPUS:
                raise NotSupportedException(
                    'Target core {0} not supported.'.format(core))
            opts['common']['arm.target.family'] = MCPUS[core]['mcpu']

        opts['common']['arm.target.arch'] = 'none'
        str = self.find_options(flags['common_flags'], '-march=')
        arch = str[len('-march='):]
        archs = {'armv6-m': 'armv6-m', 'armv7-m': 'armv7-m', 'armv7-a': 'armv7-a'}
        if arch in archs:
            opts['common']['arm.target.arch'] = archs[arch]
            self.remove_option(flags['common_flags'], str)

        opts['common']['arm.target.instructionset'] = 'thumb'
        if '-mthumb' in flags['common_flags']:
            self.remove_option(flags['common_flags'], '-mthumb')
            self.remove_option(flags['ld_flags'], '-mthumb')
        elif '-marm' in flags['common_flags']:
            opts['common']['arm.target.instructionset'] = 'arm'
            self.remove_option(flags['common_flags'], '-marm')
            self.remove_option(flags['ld_flags'], '-marm')

        opts['common']['arm.target.thumbinterwork'] = False
        if '-mthumb-interwork' in flags['common_flags']:
            opts['common']['arm.target.thumbinterwork'] = True
            self.remove_option(flags['common_flags'], '-mthumb-interwork')

        opts['common']['arm.target.endianness'] = None
        if '-mlittle-endian' in flags['common_flags']:
            opts['common']['arm.target.endianness'] = 'little'
            self.remove_option(flags['common_flags'], '-mlittle-endian')
        elif '-mbig-endian' in flags['common_flags']:
            opts['common']['arm.target.endianness'] = 'big'
            self.remove_option(flags['common_flags'], '-mbig-endian')

        opts['common']['arm.target.fpu.unit'] = None
        # default, fpv4spd16, fpv5d16, fpv5spd16
        str = self.find_options(flags['common_flags'], '-mfpu=')
        if str != None:
            fpu = str[len('-mfpu='):]
            fpus = {
                'fpv4-sp-d16': 'fpv4spd16',
                'fpv5-d16': 'fpv5d16',
                'fpv5-sp-d16': 'fpv5spd16'
            }
            if fpu in fpus:
                opts['common']['arm.target.fpu.unit'] = fpus[fpu]

                self.remove_option(flags['common_flags'], str)
                self.remove_option(flags['ld_flags'], str)
        if opts['common']['arm.target.fpu.unit'] == None:
            if core not in MCPUS:
                raise NotSupportedException(
                    'Target core {0} not supported.'.format(core))
            if MCPUS[core]['fpu_unit']:
                opts['common'][
                    'arm.target.fpu.unit'] = MCPUS[core]['fpu_unit']

        # soft, softfp, hard.
        str = self.find_options(flags['common_flags'], '-mfloat-abi=')
        if str != None:
            opts['common']['arm.target.fpu.abi'] = str[
                len('-mfloat-abi='):]
            self.remove_option(flags['common_flags'], str)
            self.remove_option(flags['ld_flags'], str)

        opts['common']['arm.target.unalignedaccess'] = None
        if '-munaligned-access' in flags['common_flags']:
            opts['common']['arm.target.unalignedaccess'] = 'enabled'
            self.remove_option(flags['common_flags'], '-munaligned-access')
        elif '-mno-unaligned-access' in flags['common_flags']:
            opts['common']['arm.target.unalignedaccess'] = 'disabled'
            self.remove_option(flags['common_flags'], '-mno-unaligned-access')

        # Default optimisation level for Release.
        opts['common']['optimization.level'] = '-Os'

        # If the project defines an optimisation level, it is used
        # only for the Release configuration, the Debug one used '-Og'.
        str = self.find_options(flags['common_flags'], '-O')
        if str != None:
            levels = {
                '-O0': 'none', '-O1': 'optimize', '-O2': 'more',
                '-O3': 'most', '-Os': 'size', '-Og': 'debug'
            }
            if str in levels:
                opts['common']['optimization.level'] = levels[str]
                self.remove_option(flags['common_flags'], str)

        include_files = []
        for all_flags in [flags['common_flags'], flags['c_flags'], flags['cxx_flags']]:
            while '-include' in all_flags:
                ix = all_flags.index('-include')
                str = all_flags[ix + 1]
                if str not in include_files:
                    include_files.append(str)
                self.remove_option(all_flags, '-include')
                self.remove_option(all_flags, str)

        opts['common']['include_files'] = include_files

        if '-ansi' in flags['c_flags']:
            opts['c']['compiler.std'] = '-ansi'
            self.remove_option(flags['c_flags'], str)
        else:
            str = self.find_options(flags['c_flags'], '-std')
            std = str[len('-std='):]
            c_std = {
                'c90': 'c90', 'c89': 'c90', 'gnu90': 'gnu90', 'gnu89': 'gnu90',
                'c99': 'c99', 'c9x': 'c99', 'gnu99': 'gnu99', 'gnu9x': 'gnu98',
                'c11': 'c11', 'c1x': 'c11', 'gnu11': 'gnu11', 'gnu1x': 'gnu11'
            }
            if std in c_std:
                opts['c']['compiler.std'] = c_std[std]
                self.remove_option(flags['c_flags'], str)

        if '-ansi' in flags['cxx_flags']:
            opts['cpp']['compiler.std'] = '-ansi'
            self.remove_option(flags['cxx_flags'], str)
        else:
            str = self.find_options(flags['cxx_flags'], '-std')
            std = str[len('-std='):]
            cpp_std = {
                'c++98': 'cpp98', 'c++03': 'cpp98',
                'gnu++98': 'gnucpp98', 'gnu++03': 'gnucpp98',
                'c++0x': 'cpp0x', 'gnu++0x': 'gnucpp0x',
                'c++11': 'cpp11', 'gnu++11': 'gnucpp11',
                'c++1y': 'cpp1y', 'gnu++1y': 'gnucpp1y',
                'c++14': 'cpp14', 'gnu++14': 'gnucpp14',
                'c++1z': 'cpp1z', 'gnu++1z': 'gnucpp1z',
            }
            if std in cpp_std:
                opts['cpp']['compiler.std'] = cpp_std[std]
                self.remove_option(flags['cxx_flags'], str)

        # Common optimisation options.
        optimization_options = {
            '-fmessage-length=0': 'optimization.messagelength',
            '-fsigned-char': 'optimization.signedchar',
            '-ffunction-sections': 'optimization.functionsections',
            '-fdata-sections': 'optimization.datasections',
            '-fno-common': 'optimization.nocommon',
            '-fno-inline-functions': 'optimization.noinlinefunctions',
            '-ffreestanding': 'optimization.freestanding',
            '-fno-builtin': 'optimization.nobuiltin',
            '-fsingle-precision-constant': 'optimization.spconstant',
            '-fPIC': 'optimization.PIC',
            '-fno-move-loop-invariants': 'optimization.nomoveloopinvariants',
        }

        for option in optimization_options:
            opts['common'][optimization_options[option]] = False
            if option in flags['common_flags']:
                opts['common'][optimization_options[option]] = True
                self.remove_option(flags['common_flags'], option)

        # Common warning options.
        warning_options = {
            '-fsyntax-only': 'warnings.syntaxonly',
            '-pedantic': 'warnings.pedantic',
            '-pedantic-errors': 'warnings.pedanticerrors',
            '-w': 'warnings.nowarn',
            '-Wunused': 'warnings.unused',
            '-Wuninitialized': 'warnings.uninitialized',
            '-Wall': 'warnings.allwarn',
            '-Wextra': 'warnings.extrawarn',
            '-Wmissing-declarations': 'warnings.missingdeclaration',
            '-Wconversion': 'warnings.conversion',
            '-Wpointer-arith': 'warnings.pointerarith',
            '-Wpadded': 'warnings.padded',
            '-Wshadow': 'warnings.shadow',
            '-Wlogical-op': 'warnings.logicalop',
            '-Waggregate-return': 'warnings.agreggatereturn',
            '-Wfloat-equal': 'warnings.floatequal',
            '-Werror': 'warnings.toerrors',
        }

        for option in warning_options:
            opts['common'][warning_options[option]] = False
            if option in flags['common_flags']:
                opts['common'][warning_options[option]] = True
                self.remove_option(flags['common_flags'], option)

        # Common debug options.
        debug_levels = {
            '-g': 'default',
            '-g1': 'minimal',
            '-g3': 'max',
        }
        opts['common']['debugging.level'] = 'none'
        for option in debug_levels:
            if option in flags['common_flags']:
                opts['common'][
                    'debugging.level'] = debug_levels[option]
                self.remove_option(flags['common_flags'], option)

        debug_formats = {
            '-ggdb': 'gdb',
            '-gstabs': 'stabs',
            '-gstabs+': 'stabsplus',
            '-gdwarf-2': 'dwarf2',
            '-gdwarf-3': 'dwarf3',
            '-gdwarf-4': 'dwarf4',
            '-gdwarf-5': 'dwarf5',
        }

        opts['common']['debugging.format'] = ''
        for option in debug_levels:
            if option in flags['common_flags']:
                opts['common'][
                    'debugging.format'] = debug_formats[option]
                self.remove_option(flags['common_flags'], option)

        opts['common']['debugging.prof'] = False
        if '-p' in flags['common_flags']:
            opts['common']['debugging.prof'] = True
            self.remove_option(flags['common_flags'], '-p')

        opts['common']['debugging.gprof'] = False
        if '-pg' in flags['common_flags']:
            opts['common']['debugging.gprof'] = True
            self.remove_option(flags['common_flags'], '-gp')

        # Assembler options.
        opts['as']['usepreprocessor'] = False
        while '-x' in flags['asm_flags']:
            ix = flags['asm_flags'].index('-x')
            str = flags['asm_flags'][ix + 1]

            if str == 'assembler-with-cpp':
                opts['as']['usepreprocessor'] = True
            else:
                # Collect all other assembler options.
                opts['as']['other'] += ' -x ' + str

            self.remove_option(flags['asm_flags'], '-x')
            self.remove_option(flags['asm_flags'], 'assembler-with-cpp')

        opts['as']['nostdinc'] = False
        if '-nostdinc' in flags['asm_flags']:
            opts['as']['nostdinc'] = True
            self.remove_option(flags['asm_flags'], '-nostdinc')

        opts['as']['verbose'] = False
        if '-v' in flags['asm_flags']:
            opts['as']['verbose'] = True
            self.remove_option(flags['asm_flags'], '-v')

        # C options.
        opts['c']['nostdinc'] = False
        if '-nostdinc' in flags['c_flags']:
            opts['c']['nostdinc'] = True
            self.remove_option(flags['c_flags'], '-nostdinc')

        opts['c']['verbose'] = False
        if '-v' in flags['c_flags']:
            opts['c']['verbose'] = True
            self.remove_option(flags['c_flags'], '-v')

        warning_options = {
            '-Wmissing-prototypes': 'warnings.missingprototypes',
            '-Wstrict-prototypes': 'warnings.strictprototypes',
            '-Wbad-function-cast': 'warnings.badfunctioncast',
        }

        for option in warning_options:
            opts['c'][warning_options[option]] = False
            if option in flags['common_flags']:
                opts['c'][warning_options[option]] = True
                self.remove_option(flags['common_flags'], option)

        # C++ options.
        opts['cpp']['nostdinc'] = False
        if '-nostdinc' in flags['cxx_flags']:
            opts['cpp']['nostdinc'] = True
            self.remove_option(flags['cxx_flags'], '-nostdinc')

        opts['cpp']['nostdincpp'] = False
        if '-nostdinc++' in flags['cxx_flags']:
            opts['cpp']['nostdincpp'] = True
            self.remove_option(flags['cxx_flags'], '-nostdinc++')

        optimization_options = {
            '-fno-exceptions': 'optimization.noexceptions',
            '-fno-rtti': 'optimization.nortti',
            '-fno-use-cxa-atexit': 'optimization.nousecxaatexit',
            '-fno-threadsafe-statics': 'optimization.nothreadsafestatics',
        }

        for option in optimization_options:
            opts['cpp'][optimization_options[option]] = False
            if option in flags['cxx_flags']:
                opts['cpp'][optimization_options[option]] = True
                self.remove_option(flags['cxx_flags'], option)
            if option in flags['common_flags']:
                opts['cpp'][optimization_options[option]] = True
                self.remove_option(flags['common_flags'], option)

        warning_options = {
            '-Wabi': 'warnabi',
            '-Wctor-dtor-privacy': 'warnings.ctordtorprivacy',
            '-Wnoexcept': 'warnings.noexcept',
            '-Wnon-virtual-dtor': 'warnings.nonvirtualdtor',
            '-Wstrict-null-sentinel': 'warnings.strictnullsentinel',
            '-Wsign-promo': 'warnings.signpromo',
            '-Weffc++': 'warneffc',
        }

        for option in warning_options:
            opts['cpp'][warning_options[option]] = False
            if option in flags['cxx_flags']:
                opts['cpp'][warning_options[option]] = True
                self.remove_option(flags['cxx_flags'], option)
            if option in flags['common_flags']:
                opts['cpp'][warning_options[option]] = True
                self.remove_option(flags['common_flags'], option)

        opts['cpp']['verbose'] = False
        if '-v' in flags['cxx_flags']:
            opts['cpp']['verbose'] = True
            self.remove_option(flags['cxx_flags'], '-v')

        # Linker options.
        linker_options = {
            '-nostartfiles': 'nostart',
            '-nodefaultlibs': 'nodeflibs',
            '-nostdlib': 'nostdlibs',
        }

        for option in linker_options:
            opts['ld'][linker_options[option]] = False
            if option in flags['ld_flags']:
                opts['ld'][linker_options[option]] = True
                self.remove_option(flags['ld_flags'], option)

        opts['ld']['gcsections'] = False
        if '-Wl,--gc-sections' in flags['ld_flags']:
            opts['ld']['gcsections'] = True
            self.remove_option(flags['ld_flags'], '-Wl,--gc-sections')

        opts['ld']['flags'] = []
        to_remove = []
        for opt in flags['ld_flags']:
            if opt.startswith('-Wl,--wrap,'):
                opts['ld']['flags'].append(
                    '--wrap=' + opt[len('-Wl,--wrap,'):])
                to_remove.append(opt)
        for opt in to_remove:
            self.remove_option(flags['ld_flags'], opt)

        # Other tool remaining options are separated by category.
        opts['as']['otherwarnings'] = self.find_options(
            flags['asm_flags'], '-W')

        opts['c']['otherwarnings'] = self.find_options(
            flags['c_flags'], '-W')
        opts['c']['otheroptimizations'] = self.find_options(flags[
            'c_flags'], '-f')

        opts['cpp']['otherwarnings'] = self.find_options(
            flags['cxx_flags'], '-W')
        opts['cpp']['otheroptimizations'] = self.find_options(
            flags['cxx_flags'], '-f')

        # Other common remaining options are separated by category.
        opts['common']['optimization.other'] = self.find_options(
            flags['common_flags'], '-f')
        opts['common']['warnings.other'] = self.find_options(
            flags['common_flags'], '-W')

        # Remaining common flags are added to each tool.
        opts['as']['other'] += ' ' + \
            ' '.join(flags['common_flags']) + ' ' + \
            ' '.join(flags['asm_flags'])
        opts['c']['other'] += ' ' + \
            ' '.join(flags['common_flags']) + ' ' + ' '.join(flags['c_flags'])
        opts['cpp']['other'] += ' ' + \
            ' '.join(flags['common_flags']) + ' ' + \
            ' '.join(flags['cxx_flags'])
        opts['ld']['other'] += ' ' + \
            ' '.join(flags['common_flags']) + ' ' + ' '.join(flags['ld_flags'])

        if len(self.system_libraries) > 0:
            opts['ld']['other'] += ' -Wl,--start-group '
            opts['ld'][
                'other'] += ' '.join('-l' + s for s in self.system_libraries)
            opts['ld']['other'] += ' -Wl,--end-group '

        # Strip all 'other' flags, since they might have leading spaces.
        opts['as']['other'] = opts['as']['other'].strip()
        opts['c']['other'] = opts['c']['other'].strip()
        opts['cpp']['other'] = opts['cpp']['other'].strip()
        opts['ld']['other'] = opts['ld']['other'].strip()

    @staticmethod
    def find_options(lst, option):
        tmp = [str for str in lst if str.startswith(option)]
        if len(tmp) > 0:
            return tmp[0]
        else:
            return None

    @staticmethod
    def find_options(lst, prefix):
        other = ''
        opts = [str for str in lst if str.startswith(prefix)]
        if len(opts) > 0:
            for opt in opts:
                other += ' ' + opt
                GNUARMEclipse.remove_option(lst, opt)
        return other.strip()

    @staticmethod
    def remove_option(lst, option):
        if option in lst:
            lst.remove(option)

# =============================================================================
