"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited
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
from os import remove
from tools.export.gnuarmeclipse import GNUARMEclipse

class E2Studio(GNUARMEclipse):
    NAME = 'e2 studio'
    TOOLCHAIN = 'GCC_ARM'

    TARGETS = [
        'RZ_A1H',
        'VK_RZ_A1H',
        'GR_LYCHEE',
    ]

    # override
    def generate(self):

        jinja_ctx = self.create_jinja_ctx()

        self.gen_file('e2studio/.cproject.tmpl', jinja_ctx, '.cproject', trim_blocks=True, lstrip_blocks=True)
        self.gen_file('e2studio/.gdbinit.tmpl', jinja_ctx, '.gdbinit')
        self.gen_file('e2studio/launch5x.tmpl', jinja_ctx, '%s OpenOCD 5x.launch' % self.project_name, trim_blocks=True, lstrip_blocks=True)
        self.gen_file('e2studio/launch.tmpl', jinja_ctx, '%s OpenOCD.launch' % self.project_name, trim_blocks=True, lstrip_blocks=True)

        self.gen_file('gnuarmeclipse/.project.tmpl', jinja_ctx, '.project', trim_blocks=True, lstrip_blocks=True)
        self.gen_file_nonoverwrite('gnuarmeclipse/mbedignore.tmpl', jinja_ctx, '.mbedignore')
        self.gen_file('gnuarmeclipse/makefile.targets.tmpl', jinja_ctx, 'makefile.targets', trim_blocks=True, lstrip_blocks=True)

    @staticmethod
    def clean(project_name):
        remove('%s OpenOCD 5x.launch' % project_name)
        remove('%s OpenOCD.launch' % project_name)
