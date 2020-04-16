from __future__ import print_function, absolute_import
from builtins import str

import os
from os.path import normpath, exists, dirname, join, abspath, relpath
import ntpath
import copy
from collections import namedtuple
import shutil
from subprocess import Popen, PIPE
import re
import json

from tools.resources import FileType
from tools.targets import TARGET_MAP, CORE_ARCH
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

        RAMS = [
            (info["start"], info["size"])
            for mem, info in self.target_info["memories"].items()
            if "RAM" in mem
        ]
        format_str = (
            "UL2CM3(-S0 -C0 -P0 -FD{ramstart}"
            " -FC{ramsize} -FN{num_algos} {extra_flags})"
        )
        ramstart = ''
        # Default according to Keil developer
        ramsize = '1000'
        if len(RAMS) >= 1:
            ramstart = '{:x}'.format(RAMS[0][0])
        extra_flags = []
        for info in self.target_info["algorithms"]:
            if not info:
                continue
            if not info["default"]:
                continue
            name = info['file_name']
            name_reg = "\w*[/\\\\]([\w_]+)\.flm"
            m = re.search(name_reg, name.lower())
            fl_name = m.group(1) if m else None
            name_flag = "-FF" + str(fl_count) + fl_name

            start = '{:x}'.format(info["start"])
            size = '{:x}'.format(info["size"])
            rom_start_flag = "-FS" + str(fl_count) + str(start)
            rom_size_flag = "-FL" + str(fl_count) + str(size)

            if info["ram_start"] is not None and info["ram_size"] is not None:
                ramstart = '{:x}'.format(info["ram_start"])
                ramsize = '{:x}'.format(info["ram_size"])

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
        "MTSCode.combine_bins_mts_dragonfly",
        "NCS36510TargetCode.ncs36510_addfib",
        "LPC55S69Code.binary_hook",
        "M2351Code.merge_secure"
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
            ["--no_vla", "--cpp", "--cpp11", "--c99", "-MMD"] + config_option
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
        fpu_core_name = core.replace("-NS", "").rstrip("E")
        if fpu_core_name.endswith("FD"):
            return "FPU3(DFPU)"
        elif fpu_core_name.endswith("F"):
            if CORE_ARCH[core] == 8:
                return "FPU3(SFPU)"
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
                                      self.resources.inc_dirs),
            'device': DeviceUvision(self.target),
            'postbuild_step_active': 0,
        }

        if self.toolchain.config.has_regions and not self.zip:
            # Serialize region information
            export_info = {}
            restrict_size = getattr(self.toolchain.config.target, "restrict_size")
            if restrict_size:
                export_info["target"] = {
                    "restrict_size": restrict_size
                }

            binary_path = "BUILD/{}.hex".format(self.project_name)
            region_list = list(self.toolchain.config.regions)
            export_info["region_list"] = [
                r._replace(filename=binary_path) if r.active else r for r in region_list
            ]
            # Enable the post build step
            postbuild_script_path = join(relpath(dirname(__file__)), "postbuild.py")
            ctx['postbuild_step'] = (
                'python {} "$K\\" "#L"'.format(postbuild_script_path)
            )
            ctx['postbuild_step_active'] = 1
            ctx['export_info'] = json.dumps(export_info, indent=4)

        sct_file_ref = self.resources.get_file_refs(FileType.LD_SCRIPT)[0]
        sct_file_ref = self.toolchain.correct_scatter_shebang(
            sct_file_ref, dirname(sct_file_ref.name)
        )
        self.resources.add_file_ref(
            FileType.LD_SCRIPT, sct_file_ref.name, sct_file_ref.path
        )
        ctx['linker_script'] = sct_file_ref.name
        fpu_included_core_name = ctx['device'].core.replace("-NS", "")
        ctx['cputype'] = fpu_included_core_name.rstrip("FDE")
        if fpu_included_core_name.endswith("FD"):
            ctx['fpu_setting'] = 3
        elif fpu_included_core_name.rstrip("E").endswith("F"):
            ctx['fpu_setting'] = 2
        else:
            ctx['fpu_setting'] = 1
        ctx['fputype'] = self.format_fpu(ctx['device'].core)
        ctx['armc6'] = int(self.TOOLCHAIN is 'ARMC6')
        ctx['toolchain_name'] = self.TOOLCHAIN_NAME

        std = [flag for flag in self.flags['c_flags'] if flag.startswith("-std=")]
        if len(std) >= 1:
            std = std[-1][len('-std='):]
        else:
            std = None
        c_std = {
            'c89': 1, 'gnu89': 2,
            'c90': 1, 'gnu90': 2,
            'c99': 3, 'gnu99': 4,
            'c11': 5, 'gnu11': 6,
        }
        ctx['v6_lang'] = c_std.get(std, 0)

        std = [flag for flag in self.flags['cxx_flags'] if flag.startswith("-std=")]
        if len(std) >= 1:
            std = std[-1][len('-std='):]
        else:
            std = None
        cpp_std = {
            'c++98': 1, 'gnu++98': 2,
            'c++03': 5, 'gnu++03': 2, # UVision 5.28 does not support gnu++03 - fall back to gnu++98
            'c++11': 3, 'gnu++11': 4,
            'c++14': 6, 'gnu++14': 7, # UVision 5.28 or later is required for gnu++14, c++14 or gnu++17
            'c++17': 8, 'gnu++17': 9,
        }
        ctx['v6_lang_p'] = cpp_std.get(std, 0)

        ctx.update(self.format_flags())
        self.gen_file(
            'uvision/uvision.tmpl', ctx, self.project_name + ".uvprojx"
        )
        self.gen_file(
            'uvision/uvision_debug.tmpl', ctx, self.project_name + ".uvoptx"
        )

        if ctx['postbuild_step_active']:
            self.gen_file(
                'uvision/debug_init.ini', ctx, 'debug_init.ini'
            )
            self.gen_file(
                'uvision/flash_init.ini', ctx, 'flash_init.ini'
            )
            self.gen_file(
                'uvision/export_info.tmpl', ctx, 'export_info.json'
            )

    @staticmethod
    def clean(project_name):
        os.remove(project_name + ".uvprojx")
        os.remove(project_name + ".uvoptx")

        if exists("export_info.json"):
            os.remove("export_info.json")
        if exists("debug_init.ini"):
            os.remove("debug_init.ini")
        if exists("flash_init.ini"):
            os.remove("flash_init.ini")

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
        if int(target.build_tools_metadata["version"]) > 0:
            #Just check for ARMC5 as ARMC5 must be there irrespective of whether uARM is there or not if the target is staying with ARMC5
            if "ARMC5" not in target.supported_toolchains:
                return False
        else:
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
        if int(target.build_tools_metadata["version"]) > 0:
            if not len(set(target.supported_toolchains).intersection(
                    set(["ARM", "ARMC6"]))) > 0:
                return False
        else:
            if "ARMC6" not in target.supported_toolchains:
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
