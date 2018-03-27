"""
mbed SDK
Copyright (c) 2014-2017 ARM Limited
Copyright (c) 2018 Code::Blocks

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
from os.path import splitext, basename
from os import remove
from shutil import rmtree
from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter
from tools.export.makefile import GccArm

class CodeBlocks(GccArm):
    NAME = 'Code::Blocks'

    DOT_IN_RELATIVE_PATH = True

    MBED_CONFIG_HEADER_SUPPORTED = True

    PREPROCESS_ASM = False

    POST_BINARY_WHITELIST = set([
        "NCS36510TargetCode.ncs36510_addfib"
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

    def generate(self):
        self.resources.win_to_unix()

        comp_flags = []
        debug_flags = []
        next_is_include = False
        for f in self.flags['c_flags'] + self.flags['cxx_flags'] + self.flags['common_flags']:
            f = f.strip()
            if f == "-include":
                next_is_include = True
                continue
            if f == 'c':
                continue
            if next_is_include:
                f = '-include ' + f
            next_is_include = False
            if f.startswith('-O') or f.startswith('-g'):
                debug_flags.append(f)
            else:
                comp_flags.append(f)
        comp_flags = list(set(comp_flags))
        inc_dirs = [self.filter_dot(s) for s in self.resources.inc_dirs];
        inc_dirs = [x for x in inc_dirs if (x is not None and
                                            x != '' and x != '.' and
                                            not x.startswith('bin') and
                                            not x.startswith('obj'))];

        c_sources = [self.filter_dot(s) for s in self.resources.c_sources]
        ncs36510fib = (hasattr(self.toolchain.target, 'post_binary_hook') and
                       self.toolchain.target.post_binary_hook['function'] == 'NCS36510TargetCode.ncs36510_addfib')
        if ncs36510fib:
            c_sources.append('ncs36510fib.c')
            c_sources.append('ncs36510trim.c')

        ctx = {
            'project_name': self.project_name,
            'debug_flags': debug_flags,
            'comp_flags': comp_flags,
            'ld_flags': self.flags['ld_flags'],
            'headers': list(set([self.filter_dot(s) for s in self.resources.headers])),
            'c_sources': c_sources,
            's_sources': [self.filter_dot(s) for s in self.resources.s_sources],
            'cpp_sources': [self.filter_dot(s) for s in self.resources.cpp_sources],
            'include_paths': inc_dirs,
            'linker_script': self.filter_dot(self.resources.linker_script),
            'libraries': self.resources.libraries,
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
            'NRF51_DK': 'board/nordic_nrf51_dk.cfg'
            }

        if self.target in openocd_board:
            ctx['openocdboard'] = openocd_board[self.target]

        self.gen_file('codeblocks/cbp.tmpl', ctx, "%s.%s" % (self.project_name, 'cbp'))

        if ncs36510fib:
            ctx = {
                'mac_addr_low': 0xFFFFFFFF,
                'mac_addr_high': 0xFFFFFFFF,
                'clk_32k_trim': 0x39,
                'clk_32m_trim': 0x17,
                'rssi': 0x3D,
                'txtune': 0xFFFFFFFF
            }
            if hasattr(self.toolchain.target, 'config'):
                for an, cn in [ ['mac-addr-low', 'mac_addr_low'],
                                ['mac-addr-high', 'mac_addr_high'],
                                ['32KHz-clk-trim', 'clk_32k_trim'],
                                ['32MHz-clk-trim', 'clk_32m_trim'],
                                ['rssi-trim', 'rssi'],
                                ['txtune-trim', 'txtune'] ]:
                    if an in self.toolchain.target.config:
                        if 'value' in self.toolchain.target.config[an]:
                            ctx[cn] = int(self.toolchain.target.config[an]['value'], 0)
            for f in [ 'ncs36510fib.c', 'ncs36510trim.c' ]:
                self.gen_file("codeblocks/%s" % f, ctx, f)

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
