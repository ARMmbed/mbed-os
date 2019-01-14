import re
import os
import json
from collections import namedtuple
from tools.targets import TARGET_MAP
from os.path import join, exists
from os import makedirs, remove
import shutil

from tools.export.makefile import Makefile, GccArm, Armc5, IAR

_eclipse_defs = os.path.join(
    os.path.dirname(os.path.abspath(__file__)), 'cdt_definitions.json')

with open(_eclipse_defs, 'r') as f:
    _CONFIGS_OPTIONS = json.load(f)

supported_launches = ['debug', 'program']

class Eclipse(Makefile):
    """Generic Eclipse project. Intended to be subclassed by classes that
    specify a type of Makefile.
    """
    def get_target_config(self, ctx, configuration):
        """Retrieve info from cdt_definitions.json"""
        tgt = TARGET_MAP[self.target]
        defaults = _CONFIGS_OPTIONS['default']
        eclipse_config = defaults['generic']
        if configuration in defaults:
            eclipse_config.update(defaults[configuration])

        target_specific = _CONFIGS_OPTIONS['targets']
        if tgt.name in target_specific:
            target_info = target_specific[tgt.name]['generic']
            if configuration in target_specific[tgt.name]:
                target_info.update(target_specific[tgt.name][configuration])
            
            eclipse_config.update(target_info)

        #special case for gdbClientOtherOptions param - in some cases it may contain dynamical values, fill in it here
        eclipse_config['gdbClientOtherOptions'] = eclipse_config['gdbClientOtherOptions'].format(
                                                    elf_location=ctx['elf_location'])

        Eclipsedevice = namedtuple('Eclipsedevice', eclipse_config.keys())
        return Eclipsedevice(**eclipse_config)

    def generate(self):
        """Generate Makefile, .cproject & .project Eclipse project file,
        py_ocd_settings launch file, and software link .p2f file
        """
        super(Eclipse, self).generate()
        starting_dot = re.compile(r'(^[.]/|^[.]$)')
        ctx = {
            'name': self.project_name,
            'elf_location': join('BUILD',self.project_name)+'.elf',
            'c_symbols': self.toolchain.get_symbols(),
            'asm_symbols': self.toolchain.get_symbols(True),
            'target': self.target,
            'include_paths': [starting_dot.sub('%s/' % self.project_name, inc) for inc in self.resources.inc_dirs],
            'load_exe': str(self.LOAD_EXE).lower()
        }
        
        launch_cfgs = {}
        for launch_name in supported_launches:
            launch = dict(ctx.items() + {'device': self.get_target_config(ctx, launch_name)}.items())
            launch_cfgs.update({launch_name: launch})
            
        if not exists(join(self.export_dir,'eclipse-extras')):
            makedirs(join(self.export_dir,'eclipse-extras'))

        for launch_name, ctx in launch_cfgs.items():
            self.gen_file('cdt/%s' % 'pyocd_settings.tmpl', ctx, join('eclipse-extras',
                            '{target}_{project}_{conf}_{launch}.launch'.format(
                                                                        target=self.target,
                                                                        project=self.project_name,
                                                                        conf=launch_name,
                                                                        launch='pyocd_settings')))

        self.gen_file('cdt/necessary_software.tmpl', ctx,
                      join('eclipse-extras','necessary_software.p2f'))

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

class EclipseIAR(Eclipse, IAR):
    LOAD_EXE = True
    NAME = "Eclipse-IAR"


