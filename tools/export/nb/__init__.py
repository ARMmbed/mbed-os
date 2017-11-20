import re
import os
import copy

from os.path import relpath, join, exists
from os import makedirs

from tools.export.makefile import Makefile, GccArm, Armc5, IAR


class Netbeans(Makefile):
    """Generic Netbeans project. Intended to be subclassed by classes that
    specify a type of Makefile.
    """

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

    def generate(self):
        """Generate Makefile, configurations.xml & project.xml Netbeans project file
        """
        super(Netbeans, self).generate()

        defines = []  # list of tuples ('D'/'U', [key, value]) (value is optional)
        forced_includes = []  # list of strings
        sources = []  # list of strings
        headers = []  # list of strings

        next_is_include = False
        for f in self.flags['c_flags'] + self.flags['cxx_flags']:
            f = f.strip()
            if next_is_include:
                forced_includes.append(f)
                next_is_include = False
                continue
            if f.startswith('-D'):
                defines.append(('D', f[2:].split('=', 1)))
            elif f.startswith('-U'):
                defines.append(('U', [f[2:]]))
            elif f == "-include":
                next_is_include = True

        # Convert all Backslashes to Forward Slashes
        self.resources.win_to_unix()

        for r_type in ['c_sources', 's_sources', 'cpp_sources']:
            sources.extend(getattr(self.resources, r_type))

        for r_type in ['headers']:
            headers.extend(getattr(self.resources, r_type))

        starting_dot = re.compile(r'(^[.]/|^[.]$)')
        sources = [starting_dot.sub('', field) for field in sources]
        headers = [starting_dot.sub('', field) for field in headers]
        include_paths = [starting_dot.sub('', field) for field in self.resources.inc_dirs]

        headers.sort()
        sources.sort()

        headers_output = create_netbeans_file_list(headers)
        sources_output = create_netbeans_file_list(sources)
        ctx = {
            'name': self.project_name,
            'elf_location': join('BUILD', self.project_name) + '.elf',
            'c_symbols': self.toolchain.get_symbols(),
            'asm_symbols': self.toolchain.get_symbols(True),
            'c_flags': self.flags['c_flags'],
            'cxx_flags': self.flags['cxx_flags'],
            'ld_flags': self.flags['ld_flags'],
            'asm_flags': self.flags['asm_flags'],
            'common_flags': self.flags['common_flags'],
            'target': self.target,
            'include_paths': include_paths,
            'sources': sources,
            'headers': headers,
            'headers_folder': headers_output,
            'sources_folder': sources_output,
            'load_exe': str(self.LOAD_EXE).lower()
        }

        if not exists(join(self.export_dir, 'nbproject')):
            makedirs(join(self.export_dir, 'nbproject'))

        self.gen_file('nb/configurations.tmpl', ctx, 'nbproject/configurations.xml')
        self.gen_file('nb/project.tmpl', ctx, 'nbproject/project.xml')
        self.gen_file('nb/mbedignore.tmpl', ctx, '.mbedignore')


def create_netbeans_file_list(file_list):
    output = []
    prev_dir = ''
    folder_count = 1
    for idx, item in enumerate(file_list):
        cur_dir = os.path.dirname(item)
        dir_temp = os.path.normpath(cur_dir)
        prev_dir_temp = os.path.normpath(prev_dir)
        dir_list = dir_temp.split(os.sep)
        prev_dir_list = prev_dir_temp.split(os.sep)
        dir_depth = len(dir_list)

        # print 'PREV_DIR: ' + str(prev_dir_list)

        # print 'CURR_DIR: ' + str(dir_list)

        # Current File is in Directory: Compare the given dir with previous Dir
        if cur_dir and prev_dir != cur_dir:
            # evaluate all matched items (from current and previous list)
            matched = []
            for element in dir_list:
                if element in prev_dir_list:
                    matched.append(element)

            # calculate difference between matched and length
            diff = dir_depth - len(matched)

            # print 'MATCHED:  ' + str(matched)

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
        # if last iteration close all open tags
        if idx == len(file_list) - 1 and cur_dir != '':
            for i in range(0, len(dir_list)):
                output.append('</logicalFolder>')

    return output


class NetbeansGcc(Netbeans, GccArm):
    LOAD_EXE = True
    NAME = "Netbeans-GCC-ARM"


class NetbeansArmc5(Netbeans, Armc5):
    LOAD_EXE = False
    NAME = "Netbeans-Armc5"


class NetbeansIAR(Netbeans, IAR):
    LOAD_EXE = True
    NAME = "Netbeans-IAR"
