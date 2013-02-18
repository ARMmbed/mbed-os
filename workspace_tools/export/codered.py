from exporters import Exporter
from os.path import splitext, basename


class CodeRed(Exporter):
    NAME = 'CodeRed'
    TARGETS = ['LPC1768']
    TOOLCHAIN = 'GCC_CR'
    
    def generate(self):
        libraries = []
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])
        
        ctx = {
            'name': self.program_name,
            'include_paths': self.resources.inc_dirs,
            'linker_script': self.resources.linker_script,
            'object_files': self.resources.objects,
            'libraries': libraries,
        }
        self.gen_file('codered_%s_project.tmpl' % self.target.lower(), ctx, '.project')
        self.gen_file('codered_%s_cproject.tmpl' % self.target.lower(), ctx, '.cproject')
