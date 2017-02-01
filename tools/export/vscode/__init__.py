from os.path import join, exists, realpath, relpath, basename
from os import makedirs

from tools.export.makefile import Makefile, GccArm, Armc5, IAR

class VSCode(Makefile):
    """Generic VSCode project. Intended to be subclassed by classes that
    specify a type of Makefile.
    """
    def generate(self):
        """Generate Makefile and VSCode launch and task files
        """
        super(VSCode, self).generate()
        ctx = {
            'name': self.project_name,
            'elf_location': join('BUILD', self.project_name)+'.elf',
            'c_symbols': self.toolchain.get_symbols(),
            'asm_symbols': self.toolchain.get_symbols(True),
            'target': self.target,
            'include_paths': self.resources.inc_dirs,
            'load_exe': str(self.LOAD_EXE).lower()
        }

        if not exists(join(self.export_dir, '.vscode')):
            makedirs(join(self.export_dir, '.vscode'))


        self.gen_file('vscode/tasks.tmpl', ctx,
                      join('.vscode', 'tasks.json'))
        self.gen_file('vscode/launch.tmpl', ctx,
                      join('.vscode', 'launch.json'))


class VSCodeGcc(VSCode, GccArm):
    LOAD_EXE = True
    NAME = "VSCode-GCC-ARM"

class VSCodeArmc5(VSCode, Armc5):
    LOAD_EXE = True
    NAME = "VSCode-Armc5"

class VSCodeIAR(VSCode, IAR):
    LOAD_EXE = True
    NAME = "VSCode-IAR"


