import os
from os.path import sep, normpath, join, exists
import ntpath
import copy
from collections import namedtuple
from distutils.spawn import find_executable
import subprocess
import re

from tools.arm_pack_manager import Cache
from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter, FailedBuildException
from tools.export.cmsis import DeviceCMSIS

cache_d = False


class DeviceUvision(DeviceCMSIS):
    """Uvision Device class, inherits CMSIS Device class

    Encapsulates information necessary for uvision project targets"""
    def __init__(self, target):
        DeviceCMSIS.__init__(self, target)
        dev_format = "$$Device:{0}${1}"
        self.svd = ''
        if self.debug_svd:
            self.svd = dev_format.format(self.dname, self.debug_svd)
        self.reg_file = dev_format.format(self.dname, self.compile_header)
        self.debug_interface = self.uv_debug()
        self.flash_dll = self.generate_flash_dll()

    def uv_debug(self):
        """Return a namedtuple of information about uvision debug settings"""
        UVDebug = namedtuple('UVDebug',['bin_loc','core_flag', 'key'])

        # CortexMXn => pCMX
        cpu = self.core.replace("Cortex-", "C")
        cpu = cpu.replace("+", "")
        cpu = cpu.replace("F", "")
        cpu_flag = "p"+cpu

        # Locations found in Keil_v5/TOOLS.INI
        debuggers = {"st-link": ('STLink\\ST-LINKIII-KEIL_SWO.dll', 'ST-LINKIII-KEIL_SWO'),
                     "j-link":('Segger\\JL2CM3.dll', 'JL2CM3'),
                     "cmsis-dap":('BIN\\CMSIS_AGDI.dll', 'CMSIS_AGDI'),
                     "nulink":('NULink\\Nu_Link.dll','Nu_Link')}
        res = debuggers[self.debug.lower()]
        binary = res[0]
        key = res[1]

        return UVDebug(binary, cpu_flag, key)

    def generate_flash_dll(self):
        '''Flash DLL string from uvision
        S = SW/JTAG Clock ID
        C = CPU index in JTAG chain
        P = Access Port
        For the Options for Target -> Debug tab -> settings -> "Flash" tab in the dialog:
        FD = RAM Start for Flash Functions
        FC = RAM Size for Flash Functions
        FN = Number of Flash types
        FF = Flash File Name (without an extension)
        FS = Start Address of the Flash Device
        FL = Size of the Flash Device
        FP = Full path to the Device algorithm (RTE)

        Necessary to flash some targets. Info gathered from algorithms field of pdsc file.
        '''
        fl_count = 0
        def get_mem_no_x(mem_str):
            mem_reg = "\dx(\w+)"
            m = re.search(mem_reg, mem_str)
            return m.group(1) if m else None

        RAMS = [(get_mem_no_x(info["start"]), get_mem_no_x(info["size"]))
                for mem, info in self.target_info["memory"].items() if "RAM" in mem]
        format_str = "UL2CM3(-S0 -C0 -P0 -FD{ramstart}"+" -FC{ramsize} "+"-FN{num_algos} {extra_flags})"
        ramstart = ''
        #Default according to Keil developer
        ramsize = '1000'
        if len(RAMS)>=1:
            ramstart = RAMS[0][0]
        extra_flags = []
        for name, info in self.target_info["algorithm"].items():
            if int(info["default"])==0:
                continue
            name_reg = "\w*/([\w_]+)\.flm"
            m = re.search(name_reg, name.lower())
            fl_name = m.group(1) if m else None
            name_flag = "-FF" + str(fl_count) + fl_name

            start, size = get_mem_no_x(info["start"]), get_mem_no_x(info["size"])
            rom_start_flag = "-FS"+str(fl_count)+str(start)
            rom_size_flag = "-FL" + str(fl_count) + str(size)

            if info["ramstart"] is not None and info["ramsize"] is not None:
                ramstart = get_mem_no_x(info["ramstart"])
                ramsize = get_mem_no_x(info["ramsize"])

            path_flag = "-FP" + str(fl_count) + "($$Device:"+self.dname+"$"+name+")"

            extra_flags.extend([name_flag, rom_start_flag, rom_size_flag, path_flag])
            fl_count += 1

        extra = " ".join(extra_flags)
        return format_str.format(ramstart=ramstart,
                                 ramsize=ramsize,
                                 extra_flags=extra, num_algos=fl_count)


class Uvision(Exporter):
    """Keil Uvision class

    This class encapsulates information to be contained in a Uvision
    project file (.uvprojx).
    The needed information can be viewed in uvision.tmpl
    """
    NAME = 'cmsis'
    TOOLCHAIN = 'ARM'
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if "ARM" in obj.supported_toolchains]
    #File associations within .uvprojx file
    file_types = {'.cpp': 8, '.c': 1, '.s': 2,
                  '.obj': 3, '.o': 3, '.lib': 4,
                  '.ar': 4, '.h': 5, '.sct': 4}

    def uv_file(self, loc):
        """Return a namedtuple of information about project file
        Positional Arguments:
        loc - the file's location

        .uvprojx XML for project file:
        <File>
            <FileType>{{file.type}}</FileType>
            <FileName>{{file.name}}</FileName>
            <FilePath>{{file.loc}}</FilePath>
        </File>
        """
        UVFile = namedtuple('UVFile', ['type','loc','name'])
        _, ext = os.path.splitext(loc)
        type = self.file_types[ext.lower()]
        name = ntpath.basename(normpath(loc))
        return UVFile(type, loc, name)

    def format_flags(self):
        """Format toolchain flags for Uvision"""
        flags = copy.deepcopy(self.flags)
        asm_flag_string = '--cpreproc --cpreproc_opts=-D__ASSERT_MSG,' + \
                          ",".join(flags['asm_flags'])
        # asm flags only, common are not valid within uvision project,
        # they are armcc specific
        flags['asm_flags'] = asm_flag_string
        # cxx flags included, as uvision have them all in one tab
        flags['c_flags'] = list(set(['-D__ASSERT_MSG']
                                        + flags['common_flags']
                                        + flags['c_flags']
                                        + flags['cxx_flags']))
        # not compatible with c99 flag set in the template
        try: flags['c_flags'].remove("--c99")
        except ValueError: pass
        # cpp is not required as it's implicit for cpp files
        try: flags['c_flags'].remove("--cpp")
        except ValueError: pass
        # we want no-vla for only cxx, but it's also applied for C in IDE,
        #  thus we remove it
        try: flags['c_flags'].remove("--no_vla")
        except ValueError: pass
        flags['c_flags'] =" ".join(flags['c_flags'])
        return flags

    def format_src(self, srcs):
        """Make sources into the named tuple for use in the template"""
        grouped = self.group_project_files(srcs)
        for group, files in grouped.items():
            grouped[group] = [self.uv_file(src) for src in files]
        return grouped

    def generate(self):
        """Generate the .uvproj file"""
        cache = Cache(True, False)
        if cache_d:
            cache.cache_descriptors()

        srcs = self.resources.headers + self.resources.s_sources + \
               self.resources.c_sources + self.resources.cpp_sources + \
               self.resources.objects + self.resources.libraries
        ctx = {
            'name': self.project_name,
            'project_files': self.format_src(srcs),
            'linker_script':self.resources.linker_script,
            'include_paths': '; '.join(self.resources.inc_dirs).encode('utf-8'),
            'device': DeviceUvision(self.target),
        }
        # Turn on FPU optimizations if the core has an FPU
        ctx['fpu_setting'] = 1 if 'f' not in ctx['device'].core.lower() \
                                  or 'd' in ctx['device'].core.lower() else 2
        ctx.update(self.format_flags())
        self.gen_file('uvision/uvision.tmpl', ctx, self.project_name+".uvprojx")
        self.gen_file('uvision/uvision_debug.tmpl', ctx, self.project_name + ".uvoptx")

    def build(self):
        ERRORLEVEL = {
            0: 'success (0 warnings, 0 errors)',
            1: 'warnings',
            2: 'errors',
            3: 'fatal errors',
            11: 'cant write to project file',
            12: 'device error',
            13: 'error writing',
            15: 'error reading xml file',
        }
        success = 0
        warn  = 1
        if find_executable("UV4"):
            uv_exe = "UV4.exe"
        else:
            uv_exe = join('C:', sep,
            'Keil_v5', 'UV4', 'UV4.exe')
            if not exists(uv_exe):
                raise Exception("UV4.exe not found. Add to path.")
        cmd = [uv_exe, '-r', '-j0', '-o', join(self.export_dir,'build_log.txt'), join(self.export_dir,self.project_name+".uvprojx")]
        ret_code = subprocess.call(cmd)
        with open(join(self.export_dir, 'build_log.txt'), 'r') as build_log:
            print build_log.read()

        if ret_code != success and ret_code != warn:
            # Seems like something went wrong.
            raise FailedBuildException("Project: %s build failed with the status: %s" % (
                self.project_name, ERRORLEVEL.get(ret_code, "Unknown")))
        else:
            return "Project: %s build succeeded with the status: %s" % (
            self.project_name, ERRORLEVEL.get(ret_code, "Unknown"))
