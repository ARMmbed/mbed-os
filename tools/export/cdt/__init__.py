"""
mbed SDK
Copyright (c) 2016-2019 ARM Limited
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
import re
import os
import json
from collections import namedtuple
from os.path import join, exists
from os import makedirs, remove
import shutil
from copy import deepcopy
from tools.targets import TARGET_MAP

from tools.export.makefile import Makefile, GccArm, Armc5, IAR

_eclipse_defs = os.path.join(
    os.path.dirname(os.path.abspath(__file__)), 'cdt_definitions.json')

with open(_eclipse_defs, 'r') as f:
    _CONFIGS_OPTIONS = json.load(f)

supported_launches = ['debug', 'program', 'erase']

class Eclipse(Makefile):
    """Generic Eclipse project. Intended to be subclassed by classes that
    specify a type of Makefile.
    """
    def get_target_config(self, configuration):
        """Retrieve info from cdt_definitions.json"""
        defaults = _CONFIGS_OPTIONS['default']
        eclipse_config = deepcopy(defaults['generic'])
        if configuration in defaults:
            eclipse_config.update(defaults[configuration])

        # Get target-specific options. Use labels to find alias
        # for target. This allows to define options within inheritance path
        target_specific = _CONFIGS_OPTIONS['targets']
        for target_alias in self.toolchain.target.labels:
            if target_alias in target_specific:
                eclipse_config.update(target_specific[target_alias]['generic'])
                if configuration in target_specific[target_alias]:
                    eclipse_config.update(target_specific[target_alias][configuration])
                break

        return eclipse_config

    def generate(self):
        """Generate Makefile, .cproject & .project Eclipse project file,
        pyocd_settings launch files for both GNU ARM Eclipse and
        GNU MCU Eclipse plug-ins, and software link .p2f file
        """
        super(Eclipse, self).generate()
        starting_dot = re.compile(r'(^[.]/|^[.]$)')
        ctx = {
            'name': self.project_name,
            'elf_location': join('BUILD', self.project_name)+'.elf',
            'c_symbols': self.toolchain.get_symbols(),
            'asm_symbols': self.toolchain.get_symbols(True),
            'target': self.target,
            'include_paths': [starting_dot.sub('%s/' % self.project_name, inc) for inc in self.resources.inc_dirs],
            'load_exe': str(self.LOAD_EXE).lower()
        }
        
        launch_cfgs = {}
        for launch_name in supported_launches:
            launch = deepcopy(ctx)
            launch.update({'device': self.get_target_config(launch_name)})
            launch_cfgs[launch_name] = launch
            
        if not exists(join(self.export_dir, 'eclipse-extras')):
            makedirs(join(self.export_dir, 'eclipse-extras'))

        for launch_name, ctx in launch_cfgs.items():
            # Generate launch configurations for former GNU ARM Eclipse plug-in
            self.gen_file('cdt/%s' % 'pyocd_settings_gnu_arm.tmpl', ctx, join('eclipse-extras',
                            '{target}_{project}_{conf}_pyocd_settings.launch'.format(
                                                                        target=self.target,
                                                                        project=self.project_name,
                                                                        conf=launch_name)))
            # Generate launch configurations for GNU MCU Eclipse plug-in
            self.gen_file('cdt/%s' % 'pyocd_settings_gnu_mcu.tmpl', ctx, join('eclipse-extras',
                            '{target}_{project}_{conf}.launch'.format(
                                                                        target=self.target,
                                                                        project=self.project_name,
                                                                        conf=launch_name)))

        self.gen_file('cdt/necessary_software.tmpl', ctx,
                      join('eclipse-extras', 'necessary_software.p2f'))

        self.gen_file('cdt/.cproject.tmpl', ctx, '.cproject')
        self.gen_file('cdt/.project.tmpl', ctx, '.project')

    @staticmethod
    def clean(project_name):
        shutil.rmtree("eclipse-extras")
        remove(".cproject")
        remove(".project")


class EclipseGcc(Eclipse, GccArm):
    LOAD_EXE = True
    NAME = "Eclipse-GCC-ARM"

class EclipseArmc5(Eclipse, Armc5):
    LOAD_EXE = False
    NAME = "Eclipse-Armc5"
    
    @classmethod
    def is_target_supported(cls, target_name):
        target = TARGET_MAP[target_name]
        if int(target.build_tools_metadata["version"]) > 0:
            return "ARMC5" in target.supported_toolchains
        else:
            return True

class EclipseIAR(Eclipse, IAR):
    LOAD_EXE = True
    NAME = "Eclipse-IAR"


