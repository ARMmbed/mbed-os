import re

from os.path import join, exists
from os import makedirs

from tools.export.makefile import Makefile, GccArm, Armc5, IAR

class Eclipse(Makefile):
    """Generic Eclipse project. Intended to be subclassed by classes that
    specify a type of Makefile.
    """
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

        if not exists(join(self.export_dir,'eclipse-extras')):
            makedirs(join(self.export_dir,'eclipse-extras'))


        self.gen_file('cdt/pyocd_settings.tmpl', ctx,
                      join('eclipse-extras',
                           '{target}_pyocd_{project}_settings.launch'.format(target=self.target,
                                                                             project=self.project_name)))
        self.gen_file('cdt/necessary_software.tmpl', ctx,
                      join('eclipse-extras','necessary_software.p2f'))

        self.gen_file('cdt/.cproject.tmpl', ctx, '.cproject')
        self.gen_file('cdt/.project.tmpl', ctx, '.project')


class EclipseGcc(Eclipse, GccArm):
    LOAD_EXE = True
    NAME = "Eclipse-GCC-ARM"

class EclipseArmc5(Eclipse, Armc5):
    LOAD_EXE = False
    NAME = "Eclipse-Armc5"

class EclipseIAR(Eclipse, IAR):
    LOAD_EXE = True
    NAME = "Eclipse-IAR"


