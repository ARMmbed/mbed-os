from __future__ import print_function, absolute_import
from builtins import str

import os
from os.path import normpath, exists, dirname
import ntpath
import copy
from collections import namedtuple
import shutil
from subprocess import Popen, PIPE
import re

from tools.resources import FileType
from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter
from tools.export.cmsis import DeviceCMSIS


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
        UVDebug = namedtuple('UVDebug', ['bin_loc', 'core_flag', 'key'])

        # CortexMXn => pCMX
        cpu = self.core.replace("Cortex-", "C")
        cpu = cpu.replace("+", "")
        cpu = cpu.replace("F", "")
        cpu = cpu.replace("-NS", "")
        cpu_flag = "p"+cpu

        # Locations found in Keil_v5/TOOLS.INI
        debuggers = {
            "st-link": ('STLink\\ST-LINKIII-KEIL_SWO.dll',
                        'ST-LINKIII-KEIL_SWO'),
            "j-link": ('Segger\\JL2CM3.dll', 'JL2CM3'),
            "cmsis-dap": ('BIN\\CMSIS_AGDI.dll', 'CMSIS_AGDI'),
            "nulink": ('NULink\\Nu_Link.dll', 'Nu_Link')
        }
        res = debuggers[self.debug.lower()]
        binary = res[0]
        key = res[1]

        return UVDebug(binary, cpu_flag, key)

    def generate_flash_dll(self):
        '''Flash DLL string from uvision
        S = SW/JTAG Clock ID
        C = CPU index in JTAG chain
        P = Access Port
        For the Options for Target -> Debug -> settings -> "Flash" dialog:
        FD = RAM Start for Flash Functions
        FC = RAM Size for Flash Functions
        FN = Number of Flash types
        FF = Flash File Name (without an extension)
        FS = Start Address of the Flash Device
        FL = Size of the Flash Device
        FP = Full path to the Device algorithm (RTE)

        Necessary to flash some targets.
        '''
        fl_count = 0

        def get_mem_no_x(mem_str):
            mem_reg = "\dx(\w+)"
            m = re.search(mem_reg, mem_str)
            return m.group(1) if m else None

        RAMS = [
            (get_mem_no_x(info["start"]), get_mem_no_x(info["size"]))
            for mem, info in self.target_info["memory"].items() if "RAM" in mem
        ]
        format_str = (
            "UL2CM3(-S0 -C0 -P0 -FD{ramstart}"
            " -FC{ramsize} -FN{num_algos} {extra_flags})"
        )
        ramstart = ''
        # Default according to Keil developer
        ramsize = '1000'
        if len(RAMS) >= 1:
            ramstart = RAMS[0][0]
        extra_flags = []
        for name, info in self.target_info["algorithm"].items():
            if not name or not info:
                continue
            if int(info["default"]) == 0:
                continue
            name_reg = "\w*/([\w_]+)\.flm"
            m = re.search(name_reg, name.lower())
            fl_name = m.group(1) if m else None
            name_flag = "-FF" + str(fl_count) + fl_name

            start = get_mem_no_x(info["start"])
            size = get_mem_no_x(info["size"])
            rom_start_flag = "-FS" + str(fl_count) + str(start)
            rom_size_flag = "-FL" + str(fl_count) + str(size)

            if info["ramstart"] is not None and info["ramsize"] is not None:
                ramstart = get_mem_no_x(info["ramstart"])
                ramsize = get_mem_no_x(info["ramsize"])

            path_flag = "-FP{}($$Device:{}${})".format(
                str(fl_count), self.dname, name
            )

            extra_flags.extend([
                name_flag, rom_start_flag, rom_size_flag, path_flag
            ])
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

    POST_BINARY_WHITELIST = set([
        "MCU_NRF51Code.binary_hook",
        "TEENSY3_1Code.binary_hook",
        "LPCTargetCode.lpc_patch",
        "LPC4088Code.binary_hook",
        "MTSCode.combine_bins_mts_dot",
        "MTSCode.combine_bins_mts_dragonfly",
        "NCS36510TargetCode.ncs36510_addfib"
    ])

    # File associations within .uvprojx file
    file_types = {'.cpp': 8, '.c': 1, '.s': 2,
                  '.obj': 3, '.o': 3, '.lib': 4,
                  '.ar': 4, '.h': 5, '.hpp': 5, '.sct': 4}

    def uv_files(self, files):
        """An generator containing Uvision specific information about project files
        Positional Arguments:
        files - the location of source files

        .uvprojx XML for project file:
        <File>
            <FileType>{{file.type}}</FileType>
            <FileName>{{file.name}}</FileName>
            <FilePath>{{file.loc}}</FilePath>
        </File>
        """
        for loc in files:
            # Encapsulates the information necessary for template entry above
            UVFile = namedtuple('UVFile', ['type', 'loc', 'name'])
            _, ext = os.path.splitext(loc)
            if ext.lower() in self.file_types:
                type = self.file_types[ext.lower()]
                name = ntpath.basename(normpath(loc))
                yield UVFile(type, loc, name)

    def format_flags(self):
        """Format toolchain flags for Uvision"""
        flags = copy.deepcopy(self.flags)
        asm_flag_string = (
            '--cpreproc --cpreproc_opts=-D__ASSERT_MSG,' +
            ",".join("-D{}".format(s) for s in
                     self.toolchain.get_symbols(for_asm=True)))
        flags['asm_flags'] = asm_flag_string

        config_header = self.config_header_ref
        config_option = self.toolchain.get_config_option(config_header.name)
        c_flags = set(
            flags['c_flags'] + flags['cxx_flags'] + flags['common_flags']
        )
        in_template = set(
            ["--no_vla", "--cpp", "--c99", "-MMD"] + config_option
        )

        def valid_flag(x):
            return (
                x not in in_template and
                not x.startswith("-O") and
                not x.startswith("-std") and
                not x.startswith("-D")
            )

        def is_define(s):
            return s.startswith("-D") and "(" not in s

        flags['c_flags'] = " ".join(
            f.replace('"', '\\"') for f in c_flags if valid_flag(f)
        )
        flags['c_flags'] += " "
        flags['c_flags'] += " ".join(config_option)
        flags['c_defines'] = " ".join(f[2:].replace('"', '\\"')
                                      for f in c_flags if is_define(f))
        flags['ld_flags'] = " ".join(set(flags['ld_flags']))
        return flags

    def format_src(self, srcs):
        """Make sources into the named tuple for use in the template"""
        grouped = self.group_project_files(srcs)
        for group, files in grouped.items():
            grouped[group] = sorted(list(self.uv_files(files)),
                                    key=lambda tuple: tuple[2].lower())
        return grouped

    @staticmethod
    def format_fpu(core):
        """Generate a core's FPU string"""
        if core.endswith("FD"):
            return "FPU3(DFPU)"
        elif core.endswith("F"):
            return "FPU2"
        else:
            return ""

    def generate(self):
        """Generate the .uvproj file"""
        srcs = (
            self.resources.headers + self.resources.s_sources +
            self.resources.c_sources + self.resources.cpp_sources +
            self.resources.objects + self.libraries
        )
        ctx = {
            'name': self.project_name,
            # project_files => dict of generators - file group to generator of
            # UVFile tuples defined above
            'project_files': sorted(list(self.format_src(srcs).items()),
                                    key=lambda tuple: tuple[0].lower()),
            'include_paths': ';'.join(self.filter_dot(d) for d in
                                      self.resources.inc_dirs).encode('utf-8'),
            'device': DeviceUvision(self.target),
        }
        sct_name, sct_path = self.resources.get_file_refs(
            FileType.LD_SCRIPT)[0]
        ctx['linker_script'] = self.toolchain.correct_scatter_shebang(
            sct_path, dirname(sct_name))
        if ctx['linker_script'] != sct_path:
            self.generated_files.append(ctx['linker_script'])
        ctx['cputype'] = ctx['device'].core.rstrip("FD").replace("-NS", "")
        if ctx['device'].core.endswith("FD"):
            ctx['fpu_setting'] = 3
        elif ctx['device'].core.endswith("F"):
            ctx['fpu_setting'] = 2
        else:
            ctx['fpu_setting'] = 1
        ctx['fputype'] = self.format_fpu(ctx['device'].core)
        ctx['armc6'] = int(self.TOOLCHAIN is 'ARMC6')
        ctx['toolchain_name'] = self.TOOLCHAIN_NAME
        ctx.update(self.format_flags())
        self.gen_file(
            'uvision/uvision.tmpl', ctx, self.project_name + ".uvprojx"
        )
        self.gen_file(
            'uvision/uvision_debug.tmpl', ctx, self.project_name + ".uvoptx"
        )

    @staticmethod
    def clean(project_name):
        os.remove(project_name + ".uvprojx")
        os.remove(project_name + ".uvoptx")
        # legacy .build directory cleaned if exists
        if exists('.build'):
            shutil.rmtree('.build')
        if exists('BUILD'):
            shutil.rmtree('BUILD')

    @staticmethod
    def build(project_name, log_name='build_log.txt', cleanup=True):
        """ Build Uvision project """
        # > UV4 -r -j0 -o [log_name] [project_name].uvprojx
        proj_file = project_name + ".uvprojx"
        cmd = ['UV4', '-r', '-j0', '-o', log_name, proj_file]

        # Build the project
        p = Popen(cmd, stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        ret_code = p.returncode

        # Print the log file to stdout
        with open(log_name, 'r') as f:
            print(f.read())

        # Cleanup the exported and built files
        if cleanup:
            os.remove(log_name)
            Uvision.clean(project_name)

        # Returns 0 upon success, 1 upon a warning, and neither upon an error
        if ret_code != 0 and ret_code != 1:
            # Seems like something went wrong.
            return -1
        else:
            return 0


class UvisionArmc5(Uvision):
    NAME = 'uvision5-armc5'
    TOOLCHAIN = 'ARM'
    TOOLCHAIN_NAME = ''

    @classmethod
    def is_target_supported(cls, target_name):
        target = TARGET_MAP[target_name]
        if not (set(target.supported_toolchains).intersection(
                set(["ARM", "uARM"]))):
            return False
        if not DeviceCMSIS.check_supported(target_name):
            return False
        if "Cortex-A" in target.core:
            return False
        if not hasattr(target, "post_binary_hook"):
            return True
        if target.post_binary_hook['function'] in cls.POST_BINARY_WHITELIST:
            return True
        else:
            return False


class UvisionArmc6(Uvision):
    NAME = 'uvision5-armc6'
    TOOLCHAIN = 'ARMC6'
    TOOLCHAIN_NAME = '6070000::V6.7::.\ARMCLANG'

    @classmethod
    def is_target_supported(cls, target_name):
        target = TARGET_MAP[target_name]
        if not (set(target.supported_toolchains).intersection(
                set(["ARMC6"]))):
            return False
        if not DeviceCMSIS.check_supported(target_name):
            return False
        if "Cortex-A" in target.core:
            return False
        if not hasattr(target, "post_binary_hook"):
            return True
        if target.post_binary_hook['function'] in cls.POST_BINARY_WHITELIST:
            return True
        else:
            return False
