"""
mbed SDK
Copyright (c) 2014-2017 ARM Limited
Copyright (c) 2018 ON Semiconductor

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
import stat
import os
from os.path import splitext, basename, dirname, abspath, isdir
from os import remove, mkdir
from shutil import rmtree, copyfile
from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter
from tools.export.makefile import GccArm

class CodeBlocks(GccArm):
    NAME = 'Code::Blocks'

    DOT_IN_RELATIVE_PATH = True

    MBED_CONFIG_HEADER_SUPPORTED = True

    PREPROCESS_ASM = False

    POST_BINARY_WHITELIST = set([
        "NCS36510TargetCode.ncs36510_addfib",
        "PSOC6Code.complete"
    ])

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

    @staticmethod
    def prepare_lib(libname):
        if "lib" == libname[:3]:
            libname = libname[3:-2]
        return "-l" + libname

    @staticmethod
    def prepare_sys_lib(libname):
        return "-l" + libname

    def generate(self):
        self.resources.win_to_unix()

        comp_flags = []
        debug_flags = []
        release_flags = [ '-Os', '-g1' ]
        next_is_include = False
        for f in self.flags['c_flags'] + self.flags['cxx_flags'] + self.flags['common_flags']:
            f = f.strip()
            if f == "-include":
                next_is_include = True
                continue
            if f == '-c':
                continue
            if next_is_include:
                f = '-include ' + f
            next_is_include = False
            if f.startswith('-O') or f.startswith('-g'):
                debug_flags.append(f)
            else:
                comp_flags.append(f)
        comp_flags = sorted(list(set(comp_flags)))
        inc_dirs = [self.filter_dot(s) for s in self.resources.inc_dirs];
        inc_dirs = [x for x in inc_dirs if (x is not None and
                                            x != '' and x != '.' and
                                            not x.startswith('bin') and
                                            not x.startswith('obj'))];

        c_sources = sorted([self.filter_dot(s) for s in self.resources.c_sources])
        libraries = [self.prepare_lib(basename(lib)) for lib in self.libraries]
        sys_libs = [self.prepare_sys_lib(lib) for lib
                    in self.toolchain.sys_libs]
        ncs36510fib = (hasattr(self.toolchain.target, 'post_binary_hook') and
                       self.toolchain.target.post_binary_hook['function'] == 'NCS36510TargetCode.ncs36510_addfib')
        if ncs36510fib:
            c_sources.append('ncs36510fib.c')
            c_sources.append('ncs36510trim.c')

        ctx = {
            'project_name': self.project_name,
            'debug_flags': debug_flags,
            'release_flags': release_flags,
            'comp_flags': comp_flags,
            'ld_flags': self.flags['ld_flags'],
            'headers': sorted(list(set([self.filter_dot(s) for s in self.resources.headers]))),
            'c_sources': c_sources,
            's_sources': sorted([self.filter_dot(s) for s in self.resources.s_sources]),
            'cpp_sources': sorted([self.filter_dot(s) for s in self.resources.cpp_sources]),
            'include_paths': inc_dirs,
            'linker_script': self.filter_dot(self.resources.linker_script),
            'libraries': libraries,
            'sys_libs': sys_libs,
            'ncs36510addfib': ncs36510fib,
            'openocdboard': ''
            }

        openocd_board = {
            'NCS36510': 'board/ncs36510_axdbg.cfg',
            'DISCO_F429ZI': 'board/stm32f429discovery.cfg',
            'DISCO_F469NI': 'board/stm32f469discovery.cfg',
            'DISCO_L053C8': 'board/stm32l0discovery.cfg',
            'DISCO_L072CZ_LRWAN1': 'board/stm32l0discovery.cfg',
            'DISCO_F769NI': 'board/stm32f7discovery.cfg',
            'DISCO_L475VG_IOT01A': 'board/stm32l4discovery.cfg',
            'DISCO_L476VG': 'board/stm32l4discovery.cfg',
            'NRF51822': 'board/nordic_nrf51822_mkit.cfg',
            'NRF51822_BOOT': 'board/nordic_nrf51822_mkit.cfg',
            'NRF51822_OTA': 'board/nordic_nrf51822_mkit.cfg',
            'NRF51_DK_LEGACY': 'board/nordic_nrf51_dk.cfg',
            'NRF51_DK_BOOT': 'board/nordic_nrf51_dk.cfg',
            'NRF51_DK_OTA': 'board/nordic_nrf51_dk.cfg',
            'NRF51_DK': 'board/nordic_nrf51_dk.cfg',
            'CY8CKIT_062_WIFI_BT': 'board/cy8ckit_062_ble.cfg'
            }

        if self.target in openocd_board:
            ctx['openocdboard'] = openocd_board[self.target]

        self.gen_file('codeblocks/cbp.tmpl', ctx, "%s.%s" % (self.project_name, 'cbp'))
        for f in [ 'obj', 'bin' ]:
            if not isdir(f):
                mkdir(f)
            self.gen_file_nonoverwrite('codeblocks/mbedignore.tmpl',
                                       ctx, f + '/.mbedignore')

        if ncs36510fib:
            genaddfiles = [ 'ncs36510fib.c', 'ncs36510trim.c' ]
            for f in genaddfiles:
                copyfile(os.path.join(dirname(abspath(__file__)), f),
                         self.gen_file_dest(f))
            ignorefiles = genaddfiles
            try:
                with open(self.gen_file_dest('.mbedignore'), 'r') as f:
                    l = set(map(lambda x: x.strip(), f.readlines()))
                    ignorefiles = [x for x in genaddfiles if x not in l]
            except IOError as e:
                pass
            except:
                raise
            if ignorefiles:
                with open(self.gen_file_dest('.mbedignore'), 'a') as f:
                    for fi in ignorefiles:
                        f.write("%s\n" % fi)

        # finally, generate the project file
        super(CodeBlocks, self).generate()

    @staticmethod
    def clean(project_name):
        for ext in ['cbp', 'depend', 'layout']:
            remove("%s.%s" % (project_name, ext))
        for f in ['openocd.log', 'ncs36510fib.c', 'ncs36510trim.c']:
            remove(f)
        for d in ['bin', 'obj']:
            rmtree(d, ignore_errors=True)
