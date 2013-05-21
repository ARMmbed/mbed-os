from os import stat, walk, remove
from os.path import splitext, exists, relpath, dirname, basename
from shutil import copyfile
from copy import copy
from types import ListType
import re

from workspace_tools.settings import *
from workspace_tools.utils import run_cmd, mkdir, rel_path, ToolException, split_path
from workspace_tools.patch import patch
from workspace_tools.targets import TARGET_NAMES

"""
We made the unfortunate choice of calling the ARM standard library toolchain "ARM"
and the ARM microlib "uARM".

Now we want to define toolchain types. For example:
    ARM: [ARM_STD, ARM_MICRO]
    GCC: [GCC_CR, GCC_CS, GCC_ARM]

We are going to introduce the "GCC" type directory, but we cannot use the "ARM"
type directory, because it would get confused with the legacy "ARM" toolchain.

[TODO] Rename ARM toolchains:
  * ARM  -> ARM_STD
  * uARM -> ARM_MICRO
"""

# List of ignored directories (all the hidden directories are ignored by default)
IGNORE_DIRECTORIES = set(['CVS'])


def print_notify(event):
    # Default command line notification
    if event['type'] in ['info', 'debug']:
        print event['message']
    
    elif event['type'] == 'cc':
        event['severity'] = event['severity'].title()
        event['file'] = basename(event['file'])
        print '[%(severity)s] %(file)s@%(line)s: %(message)s' % event
    
    elif event['type'] == 'progress':
        print '%s: %s' % (event['action'].title(), basename(event['file']))


class Resources:
    def __init__(self, base_path=None):
        self.base_path = base_path
        
        self.inc_dirs = []
        self.headers = []
        
        self.s_sources = []
        self.c_sources = []
        self.cpp_sources = []
        
        self.lib_dirs = set([])
        self.objects = []
        self.libraries = []
        
        # mbed special files
        self.lib_builds = []
        
        self.linker_script = None
    
    def add(self, resources):
        self.inc_dirs += resources.inc_dirs
        self.headers += resources.headers
        
        self.s_sources += resources.s_sources
        self.c_sources += resources.c_sources
        self.cpp_sources += resources.cpp_sources
        
        self.lib_dirs |= resources.lib_dirs
        self.objects += resources.objects
        self.libraries += resources.libraries
        
        self.lib_builds += resources.lib_builds
        
        if resources.linker_script is not None:
            self.linker_script = resources.linker_script
    
    def relative_to(self, base, dot=False):
        for field in ['inc_dirs', 'headers', 's_sources', 'c_sources',
                      'cpp_sources', 'lib_dirs', 'objects', 'libraries']:
            v = [rel_path(f, base, dot) for f in getattr(self, field)]
            setattr(self, field, v)
        if self.linker_script is not None:
            self.linker_script = rel_path(self.linker_script, base, dot)
    
    def win_to_unix(self):
        for field in ['inc_dirs', 'headers', 's_sources', 'c_sources',
                      'cpp_sources', 'lib_dirs', 'objects', 'libraries']:
            v = [f.replace('\\', '/') for f in getattr(self, field)]
            setattr(self, field, v)
        if self.linker_script is not None:
            self.linker_script = self.linker_script.replace('\\', '/')
    
    def __str__(self):
        s  = []
        
        for (label, resources) in (
                ('Include Directories', self.inc_dirs),
                ('Headers', self.headers),
                
                ('Assembly sources', self.s_sources),
                ('C sources', self.c_sources),
                ('C++ sources', self.cpp_sources),
                
                ('Library directories', self.lib_dirs),
                ('Objects', self.objects),
                ('Libraries', self.libraries)
            ):
            if resources:
                s.append('%s:\n  ' % label + '\n  '.join(resources))
        
        if self.linker_script:
            s.append('Linker Script: ' + self.linker_script)
        
        return '\n'.join(s)


class mbedToolchain:
    VERBOSE = True
    
    CORTEX_SYMBOLS = {
        "Cortex-M3" : ["__CORTEX_M3", "ARM_MATH_CM3"],
        "Cortex-M0" : ["__CORTEX_M0", "ARM_MATH_CM0"],
        "Cortex-M0+": ["__CORTEX_M0PLUS", "ARM_MATH_CM0"],
        "Cortex-M4" : ["__CORTEX_M4", "ARM_MATH_CM4", "__FPU_PRESENT=1"],        
    }
    
    def __init__(self, target, notify=None):
        self.target = target
        
        if notify is not None:
            self.notify = notify
        else:
            self.notify = print_notify
        
        self.COMPILE_C_AS_CPP = False
        self.CHROOT = None

        bin_tuple = (target.name, self.NAME)
        self.obj_path = join(*bin_tuple)
        self.IGNORE_DIR = (IGNORE_DIRECTORIES | set(TARGET_NAMES) | TOOLCHAINS) - set(bin_tuple)
        
        # Target and Toolchain symbols
        self.symbols = [
            "TARGET_" + target.name, "TOOLCHAIN_" + self.NAME,
        ]
        
        # Cortex CPU symbols
        if target.core in mbedToolchain.CORTEX_SYMBOLS:
            self.symbols.extend(mbedToolchain.CORTEX_SYMBOLS[target.core])
        
        self.IGNORE_FILES = []
        
        self.build_all = False
        self.mbed_libs = False
    
    def need_update(self, target, dependencies):
        if self.build_all:
            return True
        
        if not exists(target):
            return True

        target_mod_time = stat(target).st_mtime

        for d in dependencies:
            # Some objects are not provided with full path and here we do not have
            # information about the library paths. Safe option: assume an update
            if not d or not exists(d):
                return True
            
            if stat(d).st_mtime >= target_mod_time:
                return True
        
        return False
    
    def set_options(self, options):
        # Generic options
        if 'symbols' in options:
            self.symbols.extend(options['symbols'])
        
        # Toolchain specific options
        if self.NAME in options:
            to = options[self.NAME]
            if 'source_dir' in to:
                self.IGNORE_DIR -= set(to['source_dir'])
            
            if 'remove_options' in to:
                for option in to['remove_options']:
                    self.remove_option(option)
        
        # Target specific options
        if self.target.name in options:
            to = options[self.target.name]
            if 'ignore_files' in to:
                self.IGNORE_FILES.extend(to['ignore_files'])
    
    def scan_resources(self, path):
        resources = Resources(path)
        """ os.walk(top[, topdown=True[, onerror=None[, followlinks=False]]])
        When topdown is True, the caller can modify the dirnames list in-place
        (perhaps using del or slice assignment), and walk() will only recurse into
        the subdirectories whose names remain in dirnames; this can be used to prune
        the search, impose a specific order of visiting, or even to inform walk()
        about directories the caller creates or renames before it resumes walk()
        again. Modifying dirnames when topdown is False is ineffective, because in
        bottom-up mode the directories in dirnames are generated before dirpath
        itself is generated.
        """
        for root, dirs, files in walk(path):
            # Remove ignored directories
            for d in copy(dirs):
                if d.startswith('.') or d in self.IGNORE_DIR:
                    dirs.remove(d)
            
            # Add root to include paths
            resources.inc_dirs.append(root)
            
            for file in files:
                if file in self.IGNORE_FILES: continue
                
                file_path = join(root, file)
                _, ext = splitext(file)
                ext = ext.lower()
                
                if   ext == '.s':
                    resources.s_sources.append(file_path)
                
                elif ext == '.c':
                    resources.c_sources.append(file_path)
                
                elif ext == '.cpp':
                    resources.cpp_sources.append(file_path)
                
                elif ext == '.h':
                    resources.headers.append(file_path)
                    
                    # Check if there are the mbed libraries for this target/toolchain
                    if file == "mbed.h":
                        if self.NAME == 'ARM': # Legacy default toolchain
                            self.mbed_libs = True
                        else:
                            self.mbed_libs = exists(join(root, self.target.name, self.NAME))
                
                elif ext == '.o':
                    resources.objects.append(file_path)
                
                elif ext ==  self.LIBRARY_EXT:
                    resources.libraries.append(file_path)
                    resources.lib_dirs.add(root)
                
                elif ext == self.LINKER_EXT:
                    resources.linker_script = file_path
                
                elif ext == '.bld':
                    resources.lib_builds.append(file_path)
        
        return resources
    
    def copy_files(self, src_path, trg_path, files_paths):
        # Handle a single file
        if type(files_paths) != ListType: files_paths = [files_paths]
        
        for source in files_paths:
            
            relative_path = relpath(source, src_path)
            target = join(trg_path, relative_path)
            
            if (target != source) and (self.need_update(target, [source])):
                self.progress("copy", relative_path)
                mkdir(dirname(target))
                copyfile(source, target)
    
    def compile_sources(self, resources, build_path, inc_dirs=None):
        # Web IDE progress bar for project build
        self.to_be_compiled = len(resources.s_sources) + len(resources.c_sources) + len(resources.cpp_sources)
        self.compiled = 0
        
        objects = []
        inc_paths = resources.inc_dirs
        if inc_dirs is not None:
            inc_paths.extend(inc_dirs)
        
        for source in resources.s_sources:
            self.compiled += 1
            _, name, _ = split_path(source)
            object = join(build_path, name + '.o')
            if self.need_update(object, [source]):
                self.progress("assemble", source, build_update=True)
                self.assemble(source, object)
            objects.append(object)
        
        # The dependency checking for C/C++ is delegated to the specific compiler
        for source in resources.c_sources:
            _, name, _ = split_path(source)
            object = join(build_path, name + '.o')
            if self.COMPILE_C_AS_CPP:
                self.compile_cpp(source, object, inc_paths)
            else:
                self.compile_c(source, object, inc_paths)
            objects.append(object)
        
        for source in resources.cpp_sources:
            _, name, _ = split_path(source)
            object = join(build_path, name + '.o')
            self.compile_cpp(source, object, inc_paths)
            objects.append(object)
        
        return objects
    
    def compile(self, cc, source, object, includes):
        # Check dependencies
        base, _ = splitext(object)
        dep_path = base + '.d'
        
        self.compiled += 1
        
        if (not exists(dep_path) or
            self.need_update(object, self.parse_dependencies(dep_path))):
            
            self.progress("compile", source, build_update=True)
            
            # Compile
            command = cc + ['-D%s' % s for s in self.symbols] + ["-I%s" % i for i in includes] + ["-o", object, source]
            if hasattr(self, "get_dep_opt"):
                command.extend(self.get_dep_opt(dep_path))
            
            if hasattr(self, "cc_extra"):
                command.extend(self.cc_extra(base))
            
            self.debug(command)
            _, stderr, rc = run_cmd(command, dirname(object), chroot=self.CHROOT)
            
            # Parse output for Warnings and Errors
            self.parse_output(stderr)
            
            # Check return code
            if rc != 0:
                raise ToolException(stderr)
    
    def compile_c(self, source, object, includes):
        self.compile(self.cc, source, object, includes)
    
    def compile_cpp(self, source, object, includes):
        self.compile(self.cppc, source, object, includes)
    
    def build_library(self, objects, dir, name):
        lib = self.STD_LIB_NAME % name
        fout = join(dir, lib)
        if self.need_update(fout, objects):
            self.info("Library: %s" % lib)
            self.archive(objects, fout)
    
    def build_program(self, r, tmp_path, name):
        objects = self.compile_sources(r, tmp_path) + r.objects
        
        elf = join(tmp_path, name + '.elf')
        bin = join(tmp_path, name + '.bin')
        
        if self.need_update(elf, objects + r.libraries + [r.linker_script]):
            self.progress("link", name)
            self.link(elf, objects, r.libraries, r.lib_dirs, r.linker_script)
        
        if self.need_update(bin, [elf]):
            self.progress("elf2bin", name)
            self.binary(elf, bin)
            
            if self.target.vendor == 'NXP':
                self.debug("LPC Patch %s" % (name + '.bin'))
                patch(bin)
            
        self.var("compile_succeded", True)
        self.var("binary", name+'.bin')
        
        return bin
    
    def default_cmd(self, command):
        self.debug(command)
        stdout, stderr, rc = run_cmd(command, chroot=self.CHROOT)
        self.debug(stdout)
        if rc != 0:
            for line in stderr.splitlines():
                self.tool_error(line)
            raise ToolException(stderr)
    
    ### NOTIFICATIONS ###
    def info(self, message):
        self.notify({'type': 'info', 'message': message})
    
    def debug(self, message):
        if self.VERBOSE:
            if type(message) is ListType:
                message = ' '.join(message)
            self.notify({'type': 'debug', 'message': message})
    
    def cc_info(self, severity, file, line, message):
        self.notify({'type': 'cc', 'severity': severity, 'file': file, 'line': line, 'message': message})
    
    def progress(self, action, file, build_update=False):
        msg = {'type': 'progress', 'action': action, 'file': file}
        if build_update:
            msg['percent'] = 100. * float(self.compiled) / float(self.to_be_compiled)
        self.notify(msg)
   
    def tool_error(self, message):
        self.notify({'type': 'tool_error', 'message': message})
    
    def var(self, key, value):
        self.notify({'type': 'var', 'key': key, 'val': value})


class ARM(mbedToolchain):
    LINKER_EXT = '.sct'
    LIBRARY_EXT = '.ar'
    
    STD_LIB_NAME = "%s.ar"
    DIAGNOSTIC_PATTERN  = re.compile('"(?P<file>[^"]+)", line (?P<line>\d+): (?P<severity>Warning|Error): (?P<message>.+)')
    DEP_PATTERN = re.compile('\S+:\s(?P<file>.+)\n')
    
    def __init__(self, target, notify):
        mbedToolchain.__init__(self, target, notify)
        
        if target.core == "Cortex-M0+":
            cpu = "Cortex-M0"
        else:
            cpu = target.core         
            
        if target.core == "Cortex-M4":
            cpu = "Cortex-M4.fp"
           
        common = [join(ARM_BIN, "armcc"), "-c",
            "--cpu=%s" % cpu, "--gnu",
            "-Ospace", "--split_sections", "--apcs=interwork",
            "--brief_diagnostics", "--restrict"
        ] # "--asm" "--interleave"
        common_c = [
            "--md", "--no_depend_system_headers",
            '-I%s' % ARM_INC
        ]
        
        self.asm = common
        self.cc = common + common_c + ["--c99"]
        self.cppc = common + common_c + ["--cpp", "--no_rtti"]
        
        self.ld = [join(ARM_BIN, "armlink")]
        self.sys_libs = []
        
        self.ar = join(ARM_BIN, "armar")
        self.elf2bin = join(ARM_BIN, "fromelf")
    
    def remove_option(self, option):
        for tool in [self.asm, self.cc, self.cppc]:
            if option in tool:
                tool.remove(option)
    
    def assemble(self, source, object):
        self.default_cmd(self.cc + ["-o", object, source])
    
    def parse_dependencies(self, dep_path):
        dependencies = []
        for line in open(dep_path).readlines():
            match = ARM.DEP_PATTERN.match(line)
            if match is not None:
                if self.CHROOT:
                    # mbed.org build system: We need to append chroot here, 
                    # because when the .d files are generated, armcc is chrooted
                    dependencies.append(self.CHROOT + match.group('file'))
                else:
                    dependencies.append(match.group('file'))
        return dependencies
    
    def parse_output(self, output):
        for line in output.splitlines():
            match = ARM.DIAGNOSTIC_PATTERN.match(line)
            if match is not None:
                self.cc_info(
                    match.group('severity').lower(),
                    match.group('file'),
                    match.group('line'),
                    match.group('message')
                )
    
    def archive(self, objects, lib_path):
        self.default_cmd([self.ar, '-r', lib_path] + objects)
    
    def link(self, output, objects, libraries, lib_dirs, mem_map):
        args = ["-o", output, "--userlibpath", ",".join(lib_dirs), "--info=totals", "--list=.link_totals.txt"]
        if mem_map:
            args.extend(["--scatter", mem_map])
        
        self.default_cmd(self.ld + args + objects + libraries + self.sys_libs)
    
    def binary(self, elf, bin):
        self.default_cmd([self.elf2bin, '--bin', '-o', bin, elf])


class ARM_STD(ARM):
    NAME = 'ARM' # In the future we want to rename it ARM_STD
    
    def __init__(self, target, notify=None):
        ARM.__init__(self, target, notify)
        self.ld.append("--libpath=%s" % ARM_LIB)


class ARM_MICRO(ARM):
    NAME = 'uARM' # In the future we want to rename it ARM_MICRO
    PATCHED_LIBRARY = True
    
    def __init__(self, target, notify=None):
        ARM.__init__(self, target, notify)
        
        # Compiler
        self.asm  += ["-D__MICROLIB"]
        self.cc   += ["--library_type=microlib", "-D__MICROLIB"]
        self.cppc += ["--library_type=microlib", "-D__MICROLIB"]
        
        # Linker
        self.ld.append("--library_type=microlib")
        
        # We had to patch microlib to add C++ support
        # In later releases this patch should have entered mainline
        if ARM_MICRO.PATCHED_LIBRARY:
            self.ld.append("--noscanlib")
            
            # System Libraries
            self.sys_libs.extend([join(MY_ARM_CLIB, lib+".l") for lib in ["mc_p", "mf_p", "m_ps"]])
            
            if target.core == "Cortex-M3":
                self.sys_libs.extend([join(ARM_CPPLIB, lib+".l") for lib in ["cpp_ws", "cpprt_w"]])
            
            elif target.core in ["Cortex-M0", "Cortex-M0+"]:
                self.sys_libs.extend([join(ARM_CPPLIB, lib+".l") for lib in ["cpp_ps", "cpprt_p"]])
        else:
            self.ld.append("--libpath=%s" % ARM_LIB)


class GCC(mbedToolchain):
    LINKER_EXT = '.ld'
    LIBRARY_EXT = '.a'
    
    STD_LIB_NAME = "lib%s.a"
    CIRCULAR_DEPENDENCIES = True
    DIAGNOSTIC_PATTERN = re.compile('((?P<line>\d+):)(\d+:)? (?P<severity>warning|error): (?P<message>.+)')
    
    def __init__(self, target, notify, tool_path):
        mbedToolchain.__init__(self, target, notify)
        
        if target.core == "Cortex-M0+":
            cpu = "cortex-m0"
        else:
            cpu = target.core.lower()
        
        self.cpu = ["-mcpu=%s" % cpu]
        if target.core.startswith("Cortex"):
            self.cpu.append("-mthumb")
            
        if target.core == "Cortex-M4":
            self.cpu.append("-mfpu=vfp")
        
        # Note: We are using "-O2" instead of "-Os" to avoid this known GCC bug:
        # http://gcc.gnu.org/bugzilla/show_bug.cgi?id=46762
        common_flags = ["-c", "-O2", "-Wall",
            "-fmessage-length=0", "-fno-exceptions", "-fno-builtin",
            "-ffunction-sections", "-fdata-sections",
            "-MMD", "-save-temps"
            ] + self.cpu
        
        self.asm = [join(tool_path, "arm-none-eabi-as")] + self.cpu
        
        self.cc  = [join(tool_path, "arm-none-eabi-gcc"), "-std=gnu99"] + common_flags
        self.cppc =[join(tool_path, "arm-none-eabi-g++"), "-std=gnu++98"] + common_flags
        
        self.ld = [join(tool_path, "arm-none-eabi-gcc"), "-Wl,--gc-sections"] + self.cpu
        self.sys_libs = ["stdc++", "supc++", "m", "c", "gcc"]
        
        self.ar = join(tool_path, "arm-none-eabi-ar")
        self.elf2bin = join(tool_path, "arm-none-eabi-objcopy")
    
    def assemble(self, source, object):
        self.default_cmd(self.asm + ["-o", object, source])
    
    def parse_dependencies(self, dep_path):
        dependencies = []
        for line in open(dep_path).readlines()[1:]:
            file = line.replace('\\\n', '').strip()
            if file:
                dependencies.append(file)
        return dependencies
    
    def parse_output(self, output):
        # The warning/error notification is multiline
        WHERE, WHAT = 0, 1
        state, file, message = WHERE, None, None
        for line in output.splitlines():
            # Each line should start with the file information: "filepath: ..."
            # i should point past the file path                          ^
            # avoid the first column in Windows (C:\)
            i = line.find(':', 2)
            if i == -1: continue
            
            if state == WHERE:
                file = line[:i]
                message = line[i+1:].strip() + ' '
                state = WHAT
            
            elif state == WHAT:
                match = GCC.DIAGNOSTIC_PATTERN.match(line[i+1:])
                if match is None:
                    state = WHERE
                    continue
                
                self.cc_info(
                    match.group('severity'),
                    file, match.group('line'),
                    message + match.group('message')
                )
    
    def archive(self, objects, lib_path):
        self.default_cmd([self.ar, "rcs", lib_path] + objects)
    
    def link(self, output, objects, libraries, lib_dirs, mem_map):
        libs = []
        for l in libraries:
            name, _ = splitext(basename(l))
            libs.append("-l%s" % name[3:])
        libs.extend(["-l%s" % l for l in self.sys_libs])
        
        # NOTE: There is a circular dependency between the mbed library and the clib
        # We could define a set of week symbols to satisfy the clib dependencies in "sys.o",
        # but if an application uses only clib symbols and not mbed symbols, then the final
        # image is not correctly retargeted 
        if self.CIRCULAR_DEPENDENCIES:
            libs.extend(libs)
        
        self.default_cmd(self.ld + ["-T%s" % mem_map, "-o", output] +
            objects + ["-L%s" % L for L in lib_dirs] + libs)
    
    def binary(self, elf, bin):
        self.default_cmd([self.elf2bin, "-O", "binary", elf, bin])


class GCC_ARM(GCC):
    NAME = 'GCC_ARM'
    
    def __init__(self, target, notify=None):
        GCC.__init__(self, target, notify, GCC_ARM_PATH)
        
        # Use latest gcc nanolib
        self.ld.append("--specs=nano.specs")
        if target in ["LPC1768"]:
            self.ld.extend(["-u", "_printf_float", "-u", "_scanf_float"])
        
        self.sys_libs.append("nosys")


class GCC_CR(GCC):
    NAME = 'GCC_CR'
    
    def __init__(self, target, notify=None):
        GCC.__init__(self, target, notify, GCC_CR_PATH)
        
        additional_compiler_flags = [
            "-D__NEWLIB__", "-D__CODE_RED", "-D__USE_CMSIS", "-DCPP_USE_HEAP",
        ]
        self.cc += additional_compiler_flags
        self.cppc += additional_compiler_flags
        
        self.ld += ["-nostdlib"]


class GCC_CS(GCC):
    NAME = 'GCC_CS'
    
    def __init__(self, target, notify=None):
        GCC.__init__(self, target, notify, GCC_CS_PATH)


class GCC_CW(GCC):
    ARCH_LIB = {
        "Cortex-M0+": "armv6-m",
    }
    
    def __init__(self, target, notify=None):
        GCC.__init__(self, target, notify, CW_GCC_PATH)


class GCC_CW_EWL(GCC_CW):
    NAME = 'GCC_CW_EWL'
    
    def __init__(self, target, notify=None):
        GCC_CW.__init__(self, target, notify)
        
        # Compiler
        common = [
            '-mfloat-abi=soft',
            '-nostdinc', '-I%s' % join(CW_EWL_PATH, "EWL_C", "include"),
        ]
        self.cc += common + [
            '-include', join(CW_EWL_PATH, "EWL_C", "include", 'lib_c99.prefix')
        ]
        self.cppc += common + [
            '-nostdinc++', '-I%s' % join(CW_EWL_PATH, "EWL_C++", "include"),
            '-include', join(CW_EWL_PATH, "EWL_C++", "include", 'lib_ewl_c++.prefix')
        ]
        
        # Linker
        self.sys_libs = []
        self.CIRCULAR_DEPENDENCIES = False
        self.ld = [join(GCC_CW_PATH, "arm-none-eabi-g++"),
            "-Xlinker", "--gc-sections",
            "-L%s" % join(CW_EWL_PATH, "lib", GCC_CW.ARCH_LIB[target.core]),
            "-n", "-specs=ewl_c++.specs", "-mfloat-abi=soft",
            "-Xlinker", "--undefined=__pformatter_", "-Xlinker", "--defsym=__pformatter=__pformatter_",
            "-Xlinker", "--undefined=__sformatter", "-Xlinker", "--defsym=__sformatter=__sformatter",
        ] + self.cpu


class GCC_CW_NEWLIB(GCC_CW):
    NAME = 'GCC_CW_NEWLIB'
    
    def __init__(self, target, notify=None):
        GCC_CW.__init__(self, target, notify)


class IAR(mbedToolchain):
    NAME = 'IAR'
    LIBRARY_EXT = '.a'
    LINKER_EXT = '.icf'
    STD_LIB_NAME = "%s.a"
    
    DIAGNOSTIC_PATTERN = re.compile('"(?P<file>[^"]+)",(?P<line>[\d]+)\s+(?P<severity>Warning|Error)(?P<message>.+)')
    
    def __init__(self, target, notify=None):
        mbedToolchain.__init__(self, target, notify)
        
        c_flags = [
            "-Oh",
            "--cpu=%s" % target.core, "--thumb",
            "--dlib_config", join(IAR_PATH, "inc", "c", "DLib_Config_Full.h"),
            "-e", # Enable IAR language extension
            "--no_wrap_diagnostics",
            # Pa050: No need to be notified about "non-native end of line sequence"
            # Pa084: Pointless integer comparison -> checks for the values of an enum, but we use values outside of the enum to notify errors (ie: NC).
            # Pa093: Implicit conversion from float to integer (ie: wait_ms(85.4) -> wait_ms(85))
            # Pa082: Operation involving two values from two registers (ie: (float)(*obj->MR)/(float)(LPC_PWM1->MR0))
            "--diag_suppress=Pa050,Pa084,Pa093,Pa082",
        ]
        
        IAR_BIN = join(IAR_PATH, "bin")
        self.asm  = [join(IAR_BIN, "iasmarm")] + ["--cpu", target.core]
        self.cc   = [join(IAR_BIN, "iccarm")] + c_flags
        self.cppc = [join(IAR_BIN, "iccarm"), "--c++",  "--no_rtti", "--no_exceptions"] + c_flags
        
        self.ld   = join(IAR_BIN, "ilinkarm")
        self.ar = join(IAR_BIN, "iarchive")
        self.elf2bin = join(IAR_BIN, "ielftool")
    
    def parse_output(self, output):
        for line in output.splitlines():
            match = IAR.DIAGNOSTIC_PATTERN.match(line)
            if match is not None:
                self.cc_info(
                    match.group('severity').lower(),
                    match.group('file'),
                    match.group('line'),
                    match.group('message'),
                )
    
    def get_dep_opt(self, dep_path):
        return ["--dependencies", dep_path]
    
    def cc_extra(self, base):
        return ["-l", base + '.s']
    
    def parse_dependencies(self, dep_path):
        return [path.strip() for path in open(dep_path).readlines()
                if (path and not path.isspace())]
    
    def assemble(self, source, object):
        self.default_cmd(self.asm + ["-o", object, source])
    
    def archive(self, objects, lib_path):
        if exists(lib_path):
            remove(lib_path)
        self.default_cmd([self.ar, lib_path] + objects)
    
    def link(self, output, objects, libraries, lib_dirs, mem_map):
        args = [self.ld, "-o", output, "--config", mem_map]
        self.default_cmd(args + objects + libraries)
    
    def binary(self, elf, bin):
        self.default_cmd([self.elf2bin, '--bin', elf, bin])


TOOLCHAIN_CLASSES = {
    'ARM': ARM_STD, 'uARM': ARM_MICRO,
    'GCC_ARM': GCC_ARM, 'GCC_CS': GCC_CS, 'GCC_CR': GCC_CR,
    'GCC_CW_EWL': GCC_CW_EWL, 'GCC_CW_NEWLIB': GCC_CW_NEWLIB,
    'IAR': IAR
}

TOOLCHAINS = set(TOOLCHAIN_CLASSES.keys())
