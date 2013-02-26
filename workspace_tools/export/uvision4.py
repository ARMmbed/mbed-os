from exporters import Exporter
from os.path import basename


class Uvision4(Exporter):
    NAME = 'uVision4'
    TOOLCHAIN = 'ARM'
    TARGETS = ['LPC1768', 'LPC11U24', 'KL25Z']
    FILE_TYPES = {
        'c_sources':'1',
        'cpp_sources':'8',
        's_sources':'2'
    }
    # By convention uVision projects do not show header files in the editor:
    # 'headers':'5',
    
    def generate(self):
        source_files = []
        for r_type, n in Uvision4.FILE_TYPES.iteritems():
            for file in getattr(self.resources, r_type):
                source_files.append({
                    'name': basename(file), 'type': n, 'path': file
                })
        
        ctx = {
            'name': self.program_name,
            'include_paths': self.resources.inc_dirs,
            'scatter_file': self.resources.linker_script,
            'object_files': self.resources.objects + self.resources.libraries,
            'source_files': source_files
        }
        target = self.target.lower()
        
        # Project file
        self.gen_file('uvision4_%s.uvproj.tmpl' % target, ctx, '%s.uvproj' % self.program_name)
        
        if (target == 'lpc1768' or target == 'lpc11u24' or target == 'kl25z'):
            # Add debug option to target the MCB1700
            self.gen_file('uvision4_%s.uvopt.tmpl' % target, ctx, '%s.uvopt' % self.program_name)
