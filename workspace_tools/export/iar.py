from exporters import Exporter


class IAREmbeddedWorkbench(Exporter):
    NAME = 'IAR'
    TARGETS = ['LPC1768']
    TOOLCHAIN = 'IAR'
    
    def generate(self):
        ctx = {
            'name': self.program_name,
            'include_paths': self.resources.inc_dirs,
            'linker_script': self.resources.linker_script,
            'object_files': self.resources.objects,
            'libraries': self.resources.libraries,
        }
        self.gen_file('iar.ewp.tmpl', ctx, '%s.ewp' % self.program_name)
        self.gen_file('iar.eww.tmpl', ctx, '%s.eww' % self.program_name)
