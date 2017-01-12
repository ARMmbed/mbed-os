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
from os.path import splitext, basename, relpath, dirname
from random import randint
import os
import copy

from tools.targets import TARGET_MAP
from tools.utils import NotSupportedException


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


class GNUARMEclipse(Exporter):
    NAME = 'GNU ARM Eclipse'
    TOOLCHAIN = 'GCC_ARM'

    # Indirectly support all GCC_ARM targets.
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if 'GCC_ARM' in obj.supported_toolchains]

    @staticmethod
    def filter_dot(s):
        """
        Remove the './' prefix, if present.
        """
        if s == None:
            return None
        if s[:2] == './':
            return s[2:]
        return s

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

    def dump_tree(self, node, depth=0):
        for k in node.keys():
            n = node[k]
            pn = n['parent']['name'] if 'parent' in n.keys() else ''
            print '  ' * depth, n['name'], n['is_used'], pn
            if len(n['children'].keys()) != 0:
                self.dump_tree(n['children'], depth + 1)

    def dump_paths(self, node, depth=0):
        for k in node.keys():
            n = node[k]
            x = []
            ni = n
            while True:
                x.insert(0, ni['name'])
                if 'parent' not in ni:
                    break
                ni = ni['parent']
            path = '/'.join(x)
            print path, n['is_used']
            self.dump_paths(n['children'], depth + 1)

    def recurse_excludings(self, node):
        """
        Recurse the tree and collect all unused folders; descend
        the hierarchy only for used nodes.
        """
        for k in node.keys():
            n = node[k]
            if n['is_used'] == False:
                x = []
                ni = n
                while True:
                    x.insert(0, ni['name'])
                    if 'parent' not in ni:
                        break
                    ni = ni['parent']
                path = '/'.join(x)
                # print path
                self.excluded_folders.append(path)
            else:
                self.recurse_excludings(n['children'])

    def add_source_folder_to_tree(self, path, is_used=False):
        """
        Decompose a path in an array of folder names and create the tree.
        On the second pass the nodes should be already there; mark them
        as used.
        """
        # print path, is_used
        a = path.split(os.sep)
        n = self.source_tree
        p = None
        for s in a:
            if s[0] == '.':
                continue
            if s not in n.keys():
                nn = {}
                nn['name'] = s
                nn['children'] = {}
                if p != None:
                    nn['parent'] = p
                n[s] = nn
            n[s]['is_used'] = is_used
            p = n[s]
            n = n[s]['children']

    def compute_exclusions(self):
        """
        With the project root as the only source folder known to CDT,
        based on the list of source files, compute the folders to not
        be included in the build.

        The steps are:
        - get the list of source folders, as dirname(source_file)
        - compute the top folders (subfolders of the project folders)
        - iterate all subfolders and add them to a tree, with all 
        nodes markes as 'not used'
        - iterate the source folders and mark them as 'used' in the
        tree, including all intermediate nodes
        - recurse the tree and collect all unused folders; descend
        the hierarchy only for used nodes
        """
        source_folders = [self.filter_dot(s) for s in set(dirname(
            src) for src in self.resources.c_sources + self.resources.cpp_sources + self.resources.s_sources)]
        source_folders.remove('.')
        # print 'source folders'
        # print source_folders

        top_folders = [f for f in set(s.split(os.sep)[0]
                                      for s in source_folders)]
        # print 'top folders'
        # print top_folders

        self.source_tree = {}
        for top_folder in top_folders:
            for root, dirs, files in os.walk(top_folder):
                if len(dirs) == 0:
                    self.add_source_folder_to_tree(root)

        for folder in source_folders:
            self.add_source_folder_to_tree(folder, True)

        # print
        # print self.source_tree
        # self.dump_paths(self.source_tree)

        # print 'excludings'
        self.excluded_folders = []
        self.recurse_excludings(self.source_tree)

    def generate(self):
        """
        Override the parent method with code generation.
        """
        if not self.resources.linker_script:
            raise NotSupportedException("No linker script found.")

        # TODO: Check if this is needed.
        # self.resources.win_to_unix()

        libraries = []
        print self.resources.libraries
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])

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

        core = self.toolchain.target.core

        try:
            # cortex-m0, cortex-m0-small-multiply, cortex-m0plus,
            # cortex-m0plus-small-multiply, cortex-m1, cortex-m1-small-multiply,
            # cortex-m3, cortex-m4, cortex-m7.
            target_mcpu = MCPUS[core]['mcpu']

            # default, fpv4spd16, fpv5d16, fpv5spd16
            target_fpu_unit = MCPUS[core]['fpu_unit']

            # soft, softfp, hard.
            target_fpu_abi = 'soft'

            if target_fpu_unit != None:
                target_fpu_abi = 'hard'

        except AttributeError:
            # TODO filter out projects with toolchain core not supported,
            # instead of raising an exception.
            raise NotSupportedException(
                'Target core {0} not supported.'.format(core))

        # TODO: get the list from existing .cproject
        build_folders = ['Debug', 'Release']

        objects = [self.filter_dot(s) for s in self.resources.objects]
        for bf in build_folders:
            objects = [o for o in objects if not o.startswith(bf + '/')]
        print 'objects'
        print objects

        f = self.flags
        print 'common_flags'
        print f['common_flags']
        print 'asm_flags'
        print f['asm_flags']
        print 'c_flags'
        print f['c_flags']
        print 'cxx_flags'
        print f['cxx_flags']
        print 'ld_flags'
        print f['ld_flags']

        source_folders = [self.filter_dot(s) for s in set(dirname(
            src) for src in self.resources.c_sources + self.resources.cpp_sources + self.resources.s_sources)]
        source_folders.remove('.')

        self.compute_exclusions()

        asm_defines = self.toolchain.get_symbols(True)
        c_defines = self.toolchain.get_symbols()
        cpp_defines = self.toolchain.get_symbols()

        include_paths = [self.filter_dot(s) for s in self.resources.inc_dirs]

        library_paths = [self.filter_dot(s) for s in self.resources.lib_dirs]

        linker_script = self.filter_dot(self.resources.linker_script)

        ctx = {
            'name': self.project_name,
            'excluded_folders': '|'.join(self.excluded_folders),
            'include_paths': include_paths,
            'library_paths': library_paths,
            'object_files': objects,
            'libraries': libraries,
            'linker_script': linker_script,
            'asm_defines': asm_defines,
            'c_defines': c_defines,
            'cpp_defines': cpp_defines,
            'target_mcpu': target_mcpu,
            'target_fpu_abi': target_fpu_abi,
            'target_fpu_unit': target_fpu_unit,

            # Unique IDs used each in multiple places.
            # Add more if needed.
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

        self.gen_file('gnuarmeclipse/.project.tmpl', ctx, '.project')
        self.gen_file('gnuarmeclipse/.cproject.tmpl', ctx, '.cproject')

    @staticmethod
    def build(project_name, log_name="build_log.txt", cleanup=True):
        """
        Build GNU ARM Eclipse project.
        """

        ret_code = 0

        # TODO: add code to run the build in a headless configuration.

        # Cleanup the exported and built files
        if cleanup:
            os.remove('.project')
            os.remove('.cproject')

        if ret_code !=0:
            # Seems like something went wrong.
            return -1
        else:
            return 0
