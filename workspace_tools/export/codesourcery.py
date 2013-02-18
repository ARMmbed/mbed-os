from exporters import Exporter
from os.path import splitext, basename


class CodeSourcery(Exporter):
    NAME = 'CodeSourcery'
    TARGETS = ['LPC1768']
    TOOLCHAIN = 'GCC_CS'
    DOT_IN_RELATIVE_PATH = True
    
    def generate(self):
        # "make" wants Unix paths
        self.resources.win_to_unix()
        
        to_be_compiled = []
        for r_type in ['s_sources', 'c_sources', 'cpp_sources']:
            r = getattr(self.resources, r_type)
            if r:
                for source in r:
                    base, ext = splitext(source)
                    to_be_compiled.append(base + '.o')
        
        libraries = []
        for lib in self.resources.libraries:
            l, _ = splitext(basename(lib))
            libraries.append(l[3:])
        
        ctx = {
            'name': self.program_name,
            'to_be_compiled': to_be_compiled,
            'object_files': self.resources.objects,
            'include_paths': self.resources.inc_dirs,
            'library_paths': self.resources.lib_dirs,
            'linker_script': self.resources.linker_script,
            'libraries': libraries,
        }
        self.gen_file('codesourcery_%s.tmpl' % self.target.lower(), ctx, 'Makefile')
