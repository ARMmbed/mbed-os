from exporters import Exporter
from os.path import basename


class DS5_5(Exporter):
    NAME = 'DS5'
    TOOLCHAIN = 'ARM'
    TARGETS = ['LPC1768', 'LPC11U24']
    FILE_TYPES = {
        'c_sources':'1',
        'cpp_sources':'8',
        's_sources':'2'
    }
    
    def generate(self):
        source_files = []
        for r_type, n in DS5_5.FILE_TYPES.iteritems():
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
        self.gen_file('ds5_5_%s.project.tmpl' % target, ctx, '.project')
        self.gen_file('ds5_5_%s.cproject.tmpl' % target, ctx, '.cproject')
        self.gen_file('ds5_5_%s.launch.tmpl' % target, ctx, 'ds5_%s.launch' % target)
