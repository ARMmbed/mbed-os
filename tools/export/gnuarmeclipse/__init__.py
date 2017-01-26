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
from tools.export.exporters import Exporter
from os.path import splitext, basename, relpath, dirname, exists
from random import randint
import os
import copy
import tempfile
import shutil
from subprocess import call, Popen, PIPE

# import logging

from tools.targets import TARGET_MAP
from tools.utils import NotSupportedException

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


class GNUARMEclipse(Exporter):
    NAME = 'GNU ARM Eclipse'
    TOOLCHAIN = 'GCC_ARM'

    # Indirectly support all GCC_ARM targets.
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if 'GCC_ARM' in obj.supported_toolchains]

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
                 in self.toolchain.flags.iteritems()}
        if config_header:
            config_header = relpath(config_header,
                                    self.resources.file_basepath[config_header])
            flags['c_flags'] += self.toolchain.get_config_option(config_header)
            flags['cxx_flags'] += self.toolchain.get_config_option(
                config_header)
        return flags

    # override
    def generate(self):
        """
        Generate the .project and .cproject files.
        """
        if not self.resources.linker_script:
            raise NotSupportedException("No linker script found.")

        print
        print '[Create a GNU ARM Eclipse C++ managed project]'
        print 'Project name: {0}'.format(self.project_name)
        print 'Build configurations: Debug & Release'

        self.resources.win_to_unix()

        # TODO: use some logger to display additional info if verbose

        # There are 4 categories of options, a category common too
        # all tools and a specific category for each of the tools.
        self.options = {}
        self.options['common'] = {}
        self.options['as'] = {}
        self.options['c'] = {}
        self.options['cpp'] = {}
        self.options['ld'] = {}

        libraries = []
        # print 'libraries'
        # print self.resources.libraries
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])

        self.system_libraries = [
            'stdc++', 'supc++', 'm', 'c', 'gcc', 'nosys'
        ]

        # TODO: get the list from existing .cproject
        build_folders = ['Debug', 'Release', 'BUILD']

        objects = [self.filter_dot(s) for s in self.resources.objects]
        for bf in build_folders:
            objects = [o for o in objects if not o.startswith(bf + '/')]
        # print 'objects'
        # print objects

        self.compute_exclusions()

        self.process_options()

        self.options['as']['defines'] = self.toolchain.get_symbols(True)
        self.options['c']['defines'] = self.toolchain.get_symbols()
        self.options['cpp']['defines'] = self.toolchain.get_symbols()
        print 'Symbols: {0}'.format(len(self.options['cpp']['defines']))

        self.options['common']['include_paths'] = [
            self.filter_dot(s) for s in self.resources.inc_dirs]
        print 'Include folders: {0}'.format(len(self.options['common']['include_paths']))
        self.options['common']['excluded_folders'] = '|'.join(
            self.excluded_folders)

        self.options['ld']['library_paths'] = [
            self.filter_dot(s) for s in self.resources.lib_dirs]

        self.options['ld']['object_files'] = objects

        self.options['ld']['user_libraries'] = libraries

        self.options['ld']['system_libraries'] = self.system_libraries

        self.options['ld']['script'] = self.filter_dot(
            self.resources.linker_script)
        print 'Linker script: {0}'.format(self.options['ld']['script'])

        ctx = {
            'name': self.project_name,

            # Compiler & linker command line options
            'options': self.options,

            # Unique IDs used in multiple places.
            # Those used only once are implemented with {{u.id}}.
            'debug_config_uid': u.id,
            'debug_tool_c_compiler_uid': u.id,
            'debug_tool_c_compiler_input_uid': u.id,
            'debug_tool_cpp_compiler_uid': u.id,
            'debug_tool_cpp_compiler_input_uid': u.id,
            'release_config_uid': u.id,
            'release_tool_c_compiler_uid': u.id,
            'release_tool_c_compiler_input_uid': u.id,
            'release_tool_cpp_compiler_uid': u.id,
            'release_tool_cpp_compiler_input_uid': u.id,

            # Must be an object with an `id` property, which
            # will be called repeatedly, to generate multiple UIDs.
            'u': u,
        }

        # TODO: it would be good to have jinja stop if one of the
        # expected context values is not defined.
        self.gen_file('gnuarmeclipse/.project.tmpl', ctx, '.project')
        self.gen_file('gnuarmeclipse/.cproject.tmpl', ctx, '.cproject')
        self.gen_file('gnuarmeclipse/makefile.targets.tmpl', ctx, 'makefile.targets')

        print 'Done.'

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
            '-data', relpath(tmp_folder, os.getcwd()),
            '-import', '.',
            '-cleanBuild', 'all'
        ]

        p = Popen(' '.join(cmd), stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        ret_code = p.returncode

        stdout_string = "=" * 10 + "STDOUT" + "=" * 10 + "\n"
        err_string = "=" * 10 + "STDERR" + "=" * 10 + "\n"
        err_string += err


        ret_string = "SUCCESS\n"
        if ret_code != 0:
            ret_string += "FAILURE\n"

        print "%s\nSee %s for STDOUT\n%s\n%s" % (stdout_string, log_name, err_string, ret_string)

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
            os.remove('.project')
            os.remove('.cproject')
            if exists('Debug'):
                shutil.rmtree('Debug')
            if exists('Release'):
                shutil.rmtree('Release')

        # Always remove the temporary folder.
        if exists(tmp_folder):
            shutil.rmtree(tmp_folder)

        if ret_code == 0:
            # Return Success
            return 0

        # Seems like something went wrong.
        return -1

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
        if '.' in source_folders:
            source_folders.remove('.')

        # print 'source folders'
        # print source_folders

        # Source folders were converted before and are guaranteed to
        # use the POSIX separator.
        top_folders = [f for f in set(s.split('/')[0]
                                      for s in source_folders)]
        # print 'top folders'
        # print top_folders

        self.source_tree = {}
        for top_folder in top_folders:
            for root, dirs, files in os.walk(top_folder, topdown=True):
                # print root, dirs, files

                # Paths returned by os.walk() must be split with os.dep
                # to accomodate Windows weirdness.
                parts = root.split(os.sep)

                # Ignore paths that include parts starting with dot.
                skip = False
                for part in parts:
                    if part.startswith('.'):
                        skip = True
                        break
                if skip:
                    continue

                # Further process only leaf paths, (that do not have 
                # sub-folders).
                if len(dirs) == 0:
                    # The path is reconstructed using POSIX separators.
                    self.add_source_folder_to_tree('/'.join(parts))

        for folder in source_folders:
            self.add_source_folder_to_tree(folder, True)

        # print
        # print self.source_tree
        # self.dump_paths(self.source_tree)
        # self.dump_tree(self.source_tree)

        # print 'excludings'
        self.excluded_folders = ['BUILD']
        self.recurse_excludings(self.source_tree)

        print 'Source folders: {0}, with {1} exclusions'.format(len(source_folders), len(self.excluded_folders))

    def add_source_folder_to_tree(self, path, is_used=False):
        """
        Decompose a path in an array of folder names and create the tree.
        On the second pass the nodes should be already there; mark them
        as used.
        """
        # print path, is_used

        # All paths arriving here are guaranteed to use the POSIX
        # separators, os.walk() paths were also explicitly converted.
        parts = path.split('/')
        # print parts
        node = self.source_tree
        prev = None
        for part in parts:
            if part not in node.keys():
                new_node = {}
                new_node['name'] = part
                new_node['children'] = {}
                if prev != None:
                    new_node['parent'] = prev
                node[part] = new_node
            node[part]['is_used'] = is_used
            prev = node[part]
            node = node[part]['children']

    def recurse_excludings(self, nodes):
        """
        Recurse the tree and collect all unused folders; descend
        the hierarchy only for used nodes.
        """
        for k in nodes.keys():
            node = nodes[k]
            if node['is_used'] == False:
                parts = []
                cnode = node
                while True:
                    parts.insert(0, cnode['name'])
                    if 'parent' not in cnode:
                        break
                    cnode = cnode['parent']

                # Compose a POSIX path.
                path = '/'.join(parts)
                # print path
                self.excluded_folders.append(path)
            else:
                self.recurse_excludings(node['children'])

    # -------------------------------------------------------------------------

    @staticmethod
    def filter_dot(str):
        """
        Remove the './' prefix, if present.
        This function assumes that resources.win_to_unix()
        replaced all windows backslashes with slashes.
        """
        if str == None:
            return None
        if str[:2] == './':
            return str[2:]
        return str

    # -------------------------------------------------------------------------

    def dump_tree(self, nodes, depth=0):
        for k in nodes.keys():
            node = nodes[k]
            parent_name = node['parent']['name'] if 'parent' in node.keys() else ''
            print '  ' * depth, node['name'], node['is_used'], parent_name
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
            print path, nodes[k]['is_used']
            self.dump_paths(nodes[k]['children'], depth + 1)

    # -------------------------------------------------------------------------

    def process_options(self):
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
        flags = self.flags

        if False:
            print
            print 'common_flags', flags['common_flags']
            print 'asm_flags', flags['asm_flags']
            print 'c_flags', flags['c_flags']
            print 'cxx_flags', flags['cxx_flags']
            print 'ld_flags', flags['ld_flags']

        # Initialise the 'last resort' options where all unrecognised
        # options will be collected.
        self.options['as']['other'] = ''
        self.options['c']['other'] = ''
        self.options['cpp']['other'] = ''
        self.options['ld']['other'] = ''

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
        }

        # Remove options that are supplied by CDT
        self.remove_option(flags['common_flags'], '-c')
        self.remove_option(flags['common_flags'], '-MMD')

        # As 'plan B', get the CPU from the target definition.
        core = self.toolchain.target.core

        self.options['common']['arm.target.family'] = None

        # cortex-m0, cortex-m0-small-multiply, cortex-m0plus,
        # cortex-m0plus-small-multiply, cortex-m1, cortex-m1-small-multiply,
        # cortex-m3, cortex-m4, cortex-m7.
        str = self.find_options(flags['common_flags'], '-mcpu=')
        if str != None:
            self.options['common']['arm.target.family'] = str[len('-mcpu='):]
            self.remove_option(flags['common_flags'], str)
            self.remove_option(flags['ld_flags'], str)
        else:
            if core not in MCPUS:
                raise NotSupportedException(
                    'Target core {0} not supported.'.format(core))
            self.options['common']['arm.target.family'] = MCPUS[core]['mcpu']

        self.options['common']['arm.target.arch'] = 'none'
        str = self.find_options(flags['common_flags'], '-march=')
        arch = str[len('-march='):]
        archs = {'armv6-m': 'armv6-m', 'armv7-m': 'armv7-m'}
        if arch in archs:
            self.options['common']['arm.target.arch'] = archs[arh]
            self.remove_option(flags['common_flags'], str)

        self.options['common']['arm.target.instructionset'] = 'thumb'
        if '-mthumb' in flags['common_flags']:
            self.remove_option(flags['common_flags'], '-mthumb')
            self.remove_option(flags['ld_flags'], '-mthumb')
        elif '-marm' in flags['common_flags']:
            self.options['common']['arm.target.instructionset'] = 'arm'
            self.remove_option(flags['common_flags'], '-marm')
            self.remove_option(flags['ld_flags'], '-marm')

        self.options['common']['arm.target.thumbinterwork'] = False
        if '-mthumb-interwork' in flags['common_flags']:
            self.options['common']['arm.target.thumbinterwork'] = True
            self.remove_option(flags['common_flags'], '-mthumb-interwork')

        self.options['common']['arm.target.endianness'] = None
        if '-mlittle-endian' in flags['common_flags']:
            self.options['common']['arm.target.endianness'] = 'little'
            self.remove_option(flags['common_flags'], '-mlittle-endian')
        elif '-mbig-endian' in flags['common_flags']:
            self.options['common']['arm.target.endianness'] = 'big'
            self.remove_option(flags['common_flags'], '-mbig-endian')

        self.options['common']['arm.target.fpu.unit'] = None
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
                self.options['common']['arm.target.fpu.unit'] = fpus[fpu]

                self.remove_option(flags['common_flags'], str)
                self.remove_option(flags['ld_flags'], str)
        if self.options['common']['arm.target.fpu.unit'] == None:
            if core not in MCPUS:
                raise NotSupportedException(
                    'Target core {0} not supported.'.format(core))
            if MCPUS[core]['fpu_unit']:
                self.options['common'][
                    'arm.target.fpu.unit'] = MCPUS[core]['fpu_unit']

        # soft, softfp, hard.
        str = self.find_options(flags['common_flags'], '-mfloat-abi=')
        if str != None:
            self.options['common']['arm.target.fpu.abi'] = str[
                len('-mfloat-abi='):]
            self.remove_option(flags['common_flags'], str)
            self.remove_option(flags['ld_flags'], str)

        self.options['common']['arm.target.unalignedaccess'] = None
        if '-munaligned-access' in flags['common_flags']:
            self.options['common']['arm.target.unalignedaccess'] = 'enabled'
            self.remove_option(flags['common_flags'], '-munaligned-access')
        elif '-mno-unaligned-access' in flags['common_flags']:
            self.options['common']['arm.target.unalignedaccess'] = 'disabled'
            self.remove_option(flags['common_flags'], '-mno-unaligned-access')

        # Default optimisation level for Release.
        self.options['common']['optimization.level'] = '-Os'

        # If the project defines an optimisation level, it is used
        # only for the Release configuration, the Debug one used '-Og'.
        str = self.find_options(flags['common_flags'], '-O')
        if str != None:
            levels = {
                '-O0': 'none', '-O1': 'optimize', '-O2': 'more',
                '-O3': 'most', '-Os': 'size', '-Og': 'debug'
            }
            if str in levels:
                self.options['common']['optimization.level'] = levels[str]
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

        self.options['common']['include_files'] = include_files

        if '-ansi' in flags['c_flags']:
            self.options['c']['compiler.std'] = '-ansi'
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
                self.options['c']['compiler.std'] = c_std[std]
                self.remove_option(flags['c_flags'], str)

        if '-ansi' in flags['cxx_flags']:
            self.options['cpp']['compiler.std'] = '-ansi'
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
                self.options['cpp']['compiler.std'] = cpp_std[std]
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
            self.options['common'][optimization_options[option]] = False
            if option in flags['common_flags']:
                self.options['common'][optimization_options[option]] = True
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
            self.options['common'][warning_options[option]] = False
            if option in flags['common_flags']:
                self.options['common'][warning_options[option]] = True
                self.remove_option(flags['common_flags'], option)

        # Common debug options.
        debug_levels = {
            '-g': 'default',
            '-g1': 'minimal',
            '-g3': 'max',
        }
        self.options['common']['debugging.level'] = 'none'
        for option in debug_levels:
            if option in flags['common_flags']:
                self.options['common'][
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

        self.options['common']['debugging.format'] = ''
        for option in debug_levels:
            if option in flags['common_flags']:
                self.options['common'][
                    'debugging.format'] = debug_formats[option]
                self.remove_option(flags['common_flags'], option)

        self.options['common']['debugging.prof'] = False
        if '-p' in flags['common_flags']:
            self.options['common']['debugging.prof'] = True
            self.remove_option(flags['common_flags'], '-p')

        self.options['common']['debugging.gprof'] = False
        if '-pg' in flags['common_flags']:
            self.options['common']['debugging.gprof'] = True
            self.remove_option(flags['common_flags'], '-gp')

        # Assembler options.
        self.options['as']['usepreprocessor'] = False
        while '-x' in flags['asm_flags']:
            ix = flags['asm_flags'].index('-x')
            str = flags['asm_flags'][ix + 1]

            if str == 'assembler-with-cpp':
                self.options['as']['usepreprocessor'] = True
            else:
                # Collect all other assembler options.
                self.options['as']['other'] += ' -x ' + str

            self.remove_option(flags['asm_flags'], '-x')
            self.remove_option(flags['asm_flags'], 'assembler-with-cpp')

        self.options['as']['nostdinc'] = False
        if '-nostdinc' in flags['asm_flags']:
            self.options['as']['nostdinc'] = True
            self.remove_option(flags['asm_flags'], '-nostdinc')

        self.options['as']['verbose'] = False
        if '-v' in flags['asm_flags']:
            self.options['as']['verbose'] = True
            self.remove_option(flags['asm_flags'], '-v')

        # C options.
        self.options['c']['nostdinc'] = False
        if '-nostdinc' in flags['c_flags']:
            self.options['c']['nostdinc'] = True
            self.remove_option(flags['c_flags'], '-nostdinc')

        self.options['c']['verbose'] = False
        if '-v' in flags['c_flags']:
            self.options['c']['verbose'] = True
            self.remove_option(flags['c_flags'], '-v')

        warning_options = {
            '-Wmissing-prototypes': 'warnings.missingprototypes',
            '-Wstrict-prototypes': 'warnings.strictprototypes',
            '-Wbad-function-cast': 'warnings.badfunctioncast',
        }

        for option in warning_options:
            self.options['c'][warning_options[option]] = False
            if option in flags['common_flags']:
                self.options['c'][warning_options[option]] = True
                self.remove_option(flags['common_flags'], option)

        # C++ options.
        self.options['cpp']['nostdinc'] = False
        if '-nostdinc' in flags['cxx_flags']:
            self.options['cpp']['nostdinc'] = True
            self.remove_option(flags['cxx_flags'], '-nostdinc')

        self.options['cpp']['nostdincpp'] = False
        if '-nostdinc++' in flags['cxx_flags']:
            self.options['cpp']['nostdincpp'] = True
            self.remove_option(flags['cxx_flags'], '-nostdinc++')

        optimization_options = {
            '-fno-exceptions': 'optimization.noexceptions',
            '-fno-rtti': 'optimization.nortti',
            '-fno-use-cxa-atexit': 'optimization.nousecxaatexit',
            '-fno-threadsafe-statics': 'optimization.nothreadsafestatics',
        }

        for option in optimization_options:
            self.options['cpp'][optimization_options[option]] = False
            if option in flags['cxx_flags']:
                self.options['cpp'][optimization_options[option]] = True
                self.remove_option(flags['cxx_flags'], option)
            if option in flags['common_flags']:
                self.options['cpp'][optimization_options[option]] = True
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
            self.options['cpp'][warning_options[option]] = False
            if option in flags['cxx_flags']:
                self.options['cpp'][warning_options[option]] = True
                self.remove_option(flags['cxx_flags'], option)
            if option in flags['common_flags']:
                self.options['cpp'][warning_options[option]] = True
                self.remove_option(flags['common_flags'], option)

        self.options['cpp']['verbose'] = False
        if '-v' in flags['cxx_flags']:
            self.options['cpp']['verbose'] = True
            self.remove_option(flags['cxx_flags'], '-v')

        # Linker options.
        linker_options = {
            '-nostartfiles': 'nostart',
            '-nodefaultlibs': 'nodeflibs',
            '-nostdlib': 'nostdlibs',
        }

        for option in linker_options:
            self.options['ld'][linker_options[option]] = False
            if option in flags['ld_flags']:
                self.options['ld'][linker_options[option]] = True
                self.remove_option(flags['ld_flags'], option)

        self.options['ld']['gcsections'] = False
        if '-Wl,--gc-sections' in flags['ld_flags']:
            self.options['ld']['gcsections'] = True
            self.remove_option(flags['ld_flags'], '-Wl,--gc-sections')

        self.options['ld']['flags'] = []
        to_remove = []
        for opt in flags['ld_flags']:
            if opt.startswith('-Wl,--wrap,'):
                self.options['ld']['flags'].append('--wrap='+opt[len('-Wl,--wrap,'):])
                to_remove.append(opt)
        for opt in to_remove:
                self.remove_option(flags['ld_flags'], opt)

        # Other tool remaining options are separated by category.
        self.options['as']['otherwarnings'] = self.find_options(
            flags['asm_flags'], '-W')

        self.options['c']['otherwarnings'] = self.find_options(
            flags['c_flags'], '-W')
        self.options['c']['otheroptimizations'] = self.find_options(flags[
                                                                    'c_flags'], '-f')

        self.options['cpp']['otherwarnings'] = self.find_options(
            flags['cxx_flags'], '-W')
        self.options['cpp']['otheroptimizations'] = self.find_options(
            flags['cxx_flags'], '-f')

        # Other common remaining options are separated by category.
        self.options['common']['optimization.other'] = self.find_options(
            flags['common_flags'], '-f')
        self.options['common']['warnings.other'] = self.find_options(
            flags['common_flags'], '-W')

        # Remaining common flags are added to each tool.
        self.options['as']['other'] += ' ' + \
            ' '.join(flags['common_flags']) + ' ' + \
            ' '.join(flags['asm_flags'])
        self.options['c']['other'] += ' ' + \
            ' '.join(flags['common_flags']) + ' ' + ' '.join(flags['c_flags'])
        self.options['cpp']['other'] += ' ' + \
            ' '.join(flags['common_flags']) + ' ' + \
            ' '.join(flags['cxx_flags'])
        self.options['ld']['other'] += ' ' + \
            ' '.join(flags['common_flags']) + ' ' + ' '.join(flags['ld_flags'])

        if len(self.system_libraries) > 0:
            self.options['ld']['other'] += ' -Wl,--start-group '
            self.options['ld'][
                'other'] += ' '.join('-l' + s for s in self.system_libraries)
            self.options['ld']['other'] += ' -Wl,--end-group '

        # Strip all 'other' flags, since they might have leading spaces.
        self.options['as']['other'] = self.options['as']['other'].strip()
        self.options['c']['other'] = self.options['c']['other'].strip()
        self.options['cpp']['other'] = self.options['cpp']['other'].strip()
        self.options['ld']['other'] = self.options['ld']['other'].strip()

        if False:
            print
            print self.options

            print
            print 'common_flags', flags['common_flags']
            print 'asm_flags', flags['asm_flags']
            print 'c_flags', flags['c_flags']
            print 'cxx_flags', flags['cxx_flags']
            print 'ld_flags', flags['ld_flags']

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
