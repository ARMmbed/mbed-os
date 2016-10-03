from os.path import join, exists, realpath, relpath, basename
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
        ctx = {
            'name': self.project_name,
            'elf_location': join('.build',self.project_name)+'.elf',
            'c_symbols': self.toolchain.get_symbols(),
            'asm_symbols': self.toolchain.get_symbols(True),
            'target': self.target,
            'include_paths': self.resources.inc_dirs,
            'load_exe': str(self.LOAD_EXE).lower()
        }

        if not exists(join(self.export_dir,'eclipse-extras')):
            makedirs(join(self.export_dir,'eclipse-extras'))


        self.gen_file('cdt/pyocd_settings.tmpl', ctx,
                      join('eclipse-extras',self.target+'_pyocd_settings.launch'))
        self.gen_file('cdt/necessary_software.tmpl', ctx,
                      join('eclipse-extras','necessary_software.p2f'))

        cproj = relpath('.cproject',self.export_dir)
        self.gen_file('cdt/.cproject.tmpl', ctx,
                      cproj)
        proj = relpath('.project',self.export_dir)
        self.gen_file('cdt/.project.tmpl', ctx,
                      proj)


class EclipseGcc(Eclipse, GccArm):
    LOAD_EXE = True

class EclipseArmc5(Eclipse, Armc5):
    LOAD_EXE = False

class EclipseIAR(Eclipse, IAR):
    LOAD_EXE = True


