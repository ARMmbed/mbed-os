import os
from os.path import sep, join, exists
from collections import namedtuple
from subprocess import Popen, PIPE
from distutils.spawn import find_executable
import re

from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter, FailedBuildException
import json
class IAR(Exporter):
    NAME = 'iar'
    TOOLCHAIN = 'IAR'

    def_loc = os.path.join(
        os.path.dirname(os.path.abspath(__file__)), '..', '..', '..',
        'tools','export', 'iar', 'iar_definitions.json')

    with open(def_loc, 'r') as f:
        IAR_DEFS = json.load(f)
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if hasattr(obj, 'device_name') and
               obj.device_name in IAR_DEFS.keys()]

    SPECIAL_TEMPLATES = {
        'rz_a1h'  : 'iar/iar_rz_a1h.ewp.tmpl',
        'nucleo_f746zg' : 'iar/nucleo_f746zg.ewp.tmpl'
    }

    def iar_groups(self, grouped_src):
        """Return a namedtuple of group info
        Positional Arguments:
        grouped_src: dictionary mapping a group(str) to sources
            within it (list of file names)
        Relevant part of IAR template
        {% for group in groups %}
	    <group>
	        <name>group.name</name>
	        {% for file in group.files %}
	        <file>
	        <name>$PROJ_DIR${{file}}</name>
	        </file>
	        {% endfor %}
	    </group>
	    {% endfor %}
        """
        IARgroup = namedtuple('IARgroup', ['name','files'])
        groups = []
        for name, files in grouped_src.items():
            groups.append(IARgroup(name,files))
        return groups

    def iar_device(self):
        device_name =  TARGET_MAP[self.target].device_name
        device_info = self.IAR_DEFS[device_name]
        iar_defaults ={
            "OGChipSelectEditMenu": "",
            "CoreVariant": '',
            "GFPUCoreSlave": '',
            "GFPUCoreSlave2": 40,
            "GBECoreSlave": 35
        }

        iar_defaults.update(device_info)
        IARdevice = namedtuple('IARdevice', iar_defaults.keys())
        return IARdevice(**iar_defaults)

    def format_file(self, file):
        return join('$PROJ_DIR$',file)

    def format_src(self, srcs):
        grouped = self.group_project_files(srcs)
        for group, files in grouped.items():
            grouped[group] = [self.format_file(src) for src in files]
        return grouped

    def get_ewp_template(self):
        return self.SPECIAL_TEMPLATES.get(self.target.lower(), 'iar/ewp.tmpl')

    def generate(self):
        """Generate the .ww and .ewp files"""

        srcs = self.resources.headers + self.resources.s_sources + \
               self.resources.c_sources + self.resources.cpp_sources + \
               self.resources.objects + self.resources.libraries
        flags = self.flags
        flags['c_flags'] = list(set(flags['common_flags']
                                    + flags['c_flags']
                                    + flags['cxx_flags']))
        flags['c_flags'].remove('--vla')
        ctx = {
            'name': self.project_name,
            'groups': self.iar_groups(self.format_src(srcs)),
            'linker_script': self.format_file(self.resources.linker_script),
            'include_paths': [self.format_file(src) for src in self.resources.inc_dirs],
            'device': self.iar_device(),
            'ewp': sep+self.project_name + ".ewp"
        }
        ctx.update(flags)

        self.gen_file('iar/eww.tmpl', ctx, self.project_name+".eww")
        self.gen_file(self.get_ewp_template(), ctx, self.project_name + ".ewp")

    def _parse_subprocess_output(self, output):
        num_errors = 0
        lines = output.split("\n")
        error_re = '\s*Total number of errors:\s*(\d+)\s*'
        for line in lines:
            m = re.match(error_re, line)
            if m is not None:
                num_errors = m.group(1)
        return int(num_errors)

    def build(self):
        """ Build IAR project """
        # > IarBuild [project_path] -build [project_name]
        proj_file = join(self.export_dir, self.project_name + ".ewp")

        if find_executable("IarBuild"):
            iar_exe = "IarBuild.exe"
        else:
            iar_exe = join('C:', sep,
                          'Program Files  (x86)', 'IAR Systems',
                          'Embedded Workbench 7.5', 'common', 'bin',
                          'IarBuild.exe')
            if not exists(iar_exe):
                raise Exception("UV4.exe not found. Add to path.")

        cmd = [iar_exe, proj_file, '-build', self.project_name]
        p = Popen(cmd, stdin=PIPE, stdout=PIPE, stderr=PIPE)
        output, err = p.communicate()
        num_errors = self._parse_subprocess_output(output)
        if num_errors !=0:
            # Seems like something went wrong.
            raise FailedBuildException("Project: %s build failed with %s erros" % (
            proj_file, num_errors))
