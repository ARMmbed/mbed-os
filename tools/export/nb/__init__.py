from __future__ import print_function, absolute_import
from builtins import str

import os
import copy
import shutil

from os.path import relpath, join, exists, dirname, basename
from os import makedirs, remove
from json import load

from tools.export.exporters import Exporter, apply_supported_whitelist
from tools.targets import TARGET_MAP
from tools.utils import NotSupportedException
from tools.build_api import prepare_toolchain

POST_BINARY_WHITELIST = set([
    "TEENSY3_1Code.binary_hook",
    "MCU_NRF51Code.binary_hook",
    "LPCTargetCode.lpc_patch",
    "LPC4088Code.binary_hook"
])


class GNUARMNetbeans(Exporter):
    NAME = 'GNU ARM Netbeans'
    TOOLCHAIN = 'GCC_ARM'

    @classmethod
    def is_target_supported(cls, target_name):
        target = TARGET_MAP[target_name]
        return apply_supported_whitelist(
            cls.TOOLCHAIN, POST_BINARY_WHITELIST, target)

    @staticmethod
    def prepare_sys_lib(libname):
        return "-l" + libname

    @staticmethod
    def get_defines_and_remove_from_flags(flags_in, str_key):
        defines = []
        flags_temp = copy.deepcopy(flags_in)
        for f in flags_temp[str_key]:
            f = f.strip()
            if f.startswith('-D'):
                defines.append(f[2:])
                flags_in[str_key].remove(f)

        return defines

    @staticmethod
    def get_includes_and_remove_from_flags(flags_in, str_key):
        includes = []
        flags_temp = copy.deepcopy(flags_in)
        next_is_include = False
        for f in flags_temp[str_key]:
            f = f.strip()
            if next_is_include:
                includes.append(f)
                flags_in[str_key].remove(f)
                next_is_include = False
                continue
            if f == "-include":
                flags_in[str_key].remove(f)
                next_is_include = True

        return includes

    @staticmethod
    def get_c_std_and_remove_from_flag(flags_in, str_key):
        comp_std = ''
        c_std = {
            'c90': 'c90', 'c89': 'c90', 'gnu90': 'gnu90', 'gnu89': 'gnu90',
            'c99': 'c99', 'c9x': 'c99', 'gnu99': 'gnu99', 'gnu9x': 'gnu98',
            'c11': 'c11', 'c1x': 'c11', 'gnu11': 'gnu11', 'gnu1x': 'gnu11'
        }
        cpp_std = {
            'c++98': 'cpp98', 'c++03': 'cpp98',
            'gnu++98': 'gnucpp98', 'gnu++03': 'gnucpp98',
            'c++0x': 'cpp0x', 'gnu++0x': 'gnucpp0x',
            'c++11': 'cpp11', 'gnu++11': 'gnucpp11',
            'c++1y': 'cpp1y', 'gnu++1y': 'gnucpp1y',
            'c++14': 'cpp14', 'gnu++14': 'gnucpp14',
            'c++1z': 'cpp1z', 'gnu++1z': 'gnucpp1z',
        }

        flags_temp = copy.deepcopy(flags_in)
        for f in flags_temp[str_key]:
            f = f.strip()
            if f.startswith('-std='):
                comp_std = f[len('-std='):]
                flags_in[str_key].remove(f)
            elif f.startswith('-'):
                std = f[len('-'):]
                if std in c_std or std in cpp_std:
                    comp_std = std
                    flags_in[str_key].remove(f)
        return comp_std

    def validate_resources(self):
        if not self.resources.linker_script:
            raise NotSupportedException("No linker script found.")

    def create_jinja_ctx(self):
        self.options = {}
        flags = {}
        self.validate_resources()
        # Convert all Backslashes to Forward Slashes
        self.resources.win_to_unix()

        self.ld_script = self.filter_dot(
            self.resources.linker_script)

        # Read in all profiles, we'll extract compiler options.
        profiles = self.get_all_profiles()

        profile_ids = [s.lower() for s in profiles]
        profile_ids.sort()
        for prof_id in profile_ids:
            # There are 4 categories of options, a category common too
            # all tools and a specific category for each of the tools.
            opts = {}
            opts['defines'] = {}
            opts['common'] = {}
            opts['as'] = {}
            opts['c'] = {}
            opts['cpp'] = {}
            opts['ld'] = {}

            opts['id'] = prof_id
            opts['name'] = opts['id'].capitalize()

            profile = profiles[prof_id]

            # A small hack, do not bother with src_path again,
            # pass an empty string to avoid crashing.
            src_paths = ['']
            target_name = self.toolchain.target.name

            toolchain = prepare_toolchain(
                src_paths, "", target_name, self.TOOLCHAIN, build_profile=[profile])

            flags = self.toolchain_flags(toolchain)

            opts['defines'] = self.get_defines_and_remove_from_flags(flags, 'common_flags')
            opts['forced_includes'] = self.get_includes_and_remove_from_flags(flags, 'common_flags')
            opts['common'] = flags['common_flags']
            opts['as'] = flags['asm_flags']
            opts['c'] = flags['c_flags']
            opts['cpp'] = flags['cxx_flags']
            opts['ld'] = flags['ld_flags']

            self.options[prof_id] = opts

        sources = []  # list of strings

        forced_includes = self.get_includes_and_remove_from_flags(flags, 'c_flags')
        forced_includes += self.get_includes_and_remove_from_flags(flags, 'cxx_flags')

        # Remove Duplicates
        forced_includes = list(set(forced_includes))

        c_std = self.get_c_std_and_remove_from_flag(flags, 'c_flags')
        cpp_std = self.get_c_std_and_remove_from_flag(flags, 'cxx_flags')

        # Make one list of all resources
        for r_type in ['c_sources', 's_sources', 'cpp_sources']:
            sources.extend(getattr(self.resources, r_type))

        # Remove all leading './'
        c_sources = [self.filter_dot(field) for field in self.resources.c_sources]
        cpp_sources = [self.filter_dot(field) for field in self.resources.cpp_sources]
        s_sources = [self.filter_dot(field) for field in self.resources.s_sources]
        headers = [self.filter_dot(field) for field in self.resources.headers]
        sources = [self.filter_dot(field) for field in sources]
        include_paths = [self.filter_dot(field) for field in self.resources.inc_dirs]

        sys_libs = [self.prepare_sys_lib(lib) for lib
                    in self.toolchain.sys_libs]
        preproc = " ".join([basename(self.toolchain.preproc[0])] +
                           self.toolchain.preproc[1:] +
                           self.toolchain.ld[1:])

        if 'nbproject' in include_paths:
            include_paths.remove('nbproject')

        jinja_ctx = {
            'name': self.project_name,
            'target': self.toolchain.target.name,
            'elf_location': join('BUILD', self.project_name) + '.elf',
            'c_symbols': self.toolchain.get_symbols(),
            'asm_symbols': self.toolchain.get_symbols(True),
            'c_flags': flags['c_flags'],
            'cxx_flags': flags['cxx_flags'],
            'ld_flags': self.flags['ld_flags'],
            'asm_flags': self.flags['asm_flags'],
            'common_flags': self.flags['common_flags'],
            'include_paths': include_paths,
            'forced_includes': forced_includes,
            'c_sources': c_sources,
            'cpp_sources': cpp_sources,
            's_sources': s_sources,
            'headers': headers,
            'headers_folder': self.get_netbeans_file_list(sorted(headers)),
            'sources_folder': self.get_netbeans_file_list(sorted(sources)),
            'options': self.options,
            'c_std': self.get_netbeans_c_std(c_std),
            'cpp_std': self.get_netbeans_cpp_std(cpp_std),
            'linker_script': self.ld_script,
            'linker_libs': sys_libs,
            'pp_cmd': preproc,
            'cc_cmd': self.toolchain.cc[0],
            'cppc_cmd': self.toolchain.cppc[0],
            'asm_cmd': self.toolchain.asm[0],
            'ld_cmd': self.toolchain.ld[0],
            'elf2bin_cmd': self.toolchain.elf2bin
        }
        return jinja_ctx

    def generate(self):
        """Generate Makefile, configurations.xml & project.xml Netbeans project file
        """
        jinja_ctx = self.create_jinja_ctx()

        if not exists(join(self.export_dir, 'nbproject')):
            makedirs(join(self.export_dir, 'nbproject'))

        self.gen_file('nb/configurations.tmpl', jinja_ctx, 'nbproject/configurations.xml')
        self.gen_file('nb/project.tmpl', jinja_ctx, 'nbproject/project.xml')
        self.gen_file_nonoverwrite('nb/mbedignore.tmpl', jinja_ctx,
                                   '.mbedignore')
        self.gen_file('nb/Makefile.tmpl', jinja_ctx, 'Makefile')

        print('Done. Import the \'{0}\' project in Netbeans.'.format(self.project_name))

    @staticmethod
    def clean(_):
        shutil.rmtree("nbproject")
        remove("Makefile")

    # -------------------------------------------------------------------------

    @staticmethod
    def filter_dot(str_in):
        """
        Remove the './' prefix, if present.
        This function assumes that resources.win_to_unix()
        replaced all windows backslashes with slashes.
        """
        if str_in is None:
            return None
        if str_in[:2] == './':
            return str_in[2:]
        return str_in

    # -------------------------------------------------------------------------

    @staticmethod
    def get_all_profiles():
        tools_path = dirname(dirname(dirname(__file__)))
        file_names = [join(tools_path, "profiles", fn) for fn in os.listdir(
            join(tools_path, "profiles")) if fn.endswith(".json")]

        profiles = {}

        for fn in file_names:
            content = load(open(fn))
            profile_name = basename(fn).replace(".json", "")
            profiles[profile_name] = content

        return profiles

    @staticmethod
    def get_netbeans_file_list(file_list):
        cur_dir = ''
        prev_dir = ''
        output = []
        folder_count = 1
        dir_depth = 0
        for item in file_list:
            cur_dir = os.path.dirname(item)
            dir_temp = os.path.normpath(cur_dir)
            prev_dir_temp = os.path.normpath(prev_dir)
            dir_list = dir_temp.split(os.sep)
            prev_dir_list = prev_dir_temp.split(os.sep)
            dir_depth = len(dir_list)

            # Current File is in Directory: Compare the given dir with previous Dir
            if cur_dir and prev_dir != cur_dir:
                # evaluate all matched items (from current and previous list)
                matched = []
                # Compare the Element in Previous Dir with the Elements in Current Dir
                # and add the equal Elements to the match-List
                for elem_prev_dir, elem_cur_dir in zip(prev_dir_list, dir_list):
                    if elem_prev_dir == elem_cur_dir:
                        matched.append(elem_cur_dir)

                # calculate difference between matched and length
                diff = dir_depth - len(matched)

                # if previous dir was not root
                if prev_dir != '':
                    # if the elements count is not equal we calculate the difference
                    if len(dir_list) != len(prev_dir_list):
                        dir_depth_prev = len(prev_dir_list)
                        delta = dir_depth_prev - len(matched)

                        for i in range(dir_depth_prev - delta, dir_depth_prev):
                            output.append('</logicalFolder>')

                    # if the elements count is equal, we subtract the matched length from the total length
                    else:
                        for i in range(len(matched), len(dir_list)):
                            output.append('</logicalFolder>')

                for i in range(dir_depth - diff, dir_depth):
                    output.append('<logicalFolder name="f' + str(folder_count) + '" displayName="' + str(
                        dir_list[i]) + '" projectFiles="true">')
                    folder_count += 1

            # Current File is in root
            else:
                # Close Tag if we are in root and the previous dir wasn't
                if cur_dir == '' and prev_dir != '':
                    for i in range(0, len(prev_dir_list)):
                        output.append('</logicalFolder>')

            # Save the Current Dir
            prev_dir = cur_dir
            output.append('<itemPath>' + str(item) + '</itemPath>')

        if cur_dir != '':
            # close all open tags
            output.append('</logicalFolder>' * dir_depth)

        return output

    @staticmethod
    def get_netbeans_c_std(c_std):
        c_std_netbeans = 0
        if '89' in c_std:
            c_std_netbeans = 2
        elif '99' in c_std:
            c_std_netbeans = 3
        elif '11' in c_std:
            c_std_netbeans = 10
        return c_std_netbeans

    @staticmethod
    def get_netbeans_cpp_std(cpp_std):
        cpp_std_netbeans = 0
        if '98' in cpp_std:
            cpp_std_netbeans = 4
        elif '11' in cpp_std:
            cpp_std_netbeans = 8
        elif '14' in cpp_std:
            cpp_std_netbeans = 11
        return cpp_std_netbeans
