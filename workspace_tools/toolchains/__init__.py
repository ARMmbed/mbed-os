"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
from os import stat, walk, remove
from os.path import join, splitext, exists, relpath, dirname, basename, split
from shutil import copyfile
from copy import copy
from types import ListType
from inspect import getmro

from workspace_tools.utils import run_cmd, mkdir, rel_path, ToolException, split_path
from workspace_tools.patch import patch
from workspace_tools.settings import BUILD_OPTIONS

import workspace_tools.hooks as hooks
import re

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


# Support legacy build conventions: the original mbed build system did not have
# standard labels for the "TARGET_" and "TOOLCHAIN_" specific directories, but
# had the knowledge of a list of these directories to be ignored.
LEGACY_IGNORE_DIRS = set([
    'LPC11U24', 'LPC1768', 'LPC2368', 'LPC4088', 'LPC812', 'KL25Z',
    'ARM', 'GCC_ARM', 'GCC_CR', 'GCC_CS', 'IAR', 'uARM'
])
LEGACY_TOOLCHAIN_NAMES = {
    'ARM_STD':'ARM', 'ARM_MICRO': 'uARM',
    'GCC_ARM': 'GCC_ARM', 'GCC_CR': 'GCC_CR', 'GCC_CS': 'GCC_CS',
    'IAR': 'IAR',
}


class mbedToolchain:
    VERBOSE = True
    
    CORTEX_SYMBOLS = {
        "Cortex-M3" : ["__CORTEX_M3", "ARM_MATH_CM3"],
        "Cortex-M0" : ["__CORTEX_M0", "ARM_MATH_CM0"],
        "Cortex-M0+": ["__CORTEX_M0PLUS", "ARM_MATH_CM0PLUS"],
        "Cortex-M4" : ["__CORTEX_M4", "ARM_MATH_CM4", "__FPU_PRESENT=1"],
    }

    GOANNA_FORMAT = "[Goanna] warning [%FILENAME%:%LINENO%] - [%CHECKNAME%(%SEVERITY%)] %MESSAGE%"
    GOANNA_DIAGNOSTIC_PATTERN = re.compile(r'"\[Goanna\] (?P<severity>warning) \[(?P<file>[^:]+):(?P<line>\d+)\] \- (?P<message>.*)"')

    def __init__(self, target, options=None, notify=None, macros=None):
        self.target = target
        self.name = self.__class__.__name__
        self.hook = hooks.Hook(target, self)
        
        self.legacy_ignore_dirs = LEGACY_IGNORE_DIRS - set([target.name, LEGACY_TOOLCHAIN_NAMES[self.name]])
        
        if notify is not None:
            self.notify = notify
        else:
            self.notify = print_notify
        
        if options is None:
            self.options = []
        else:
            self.options = options
        self.macros = macros or []
        self.options.extend(BUILD_OPTIONS)
        if self.options:
            self.info("Build Options: %s" % (', '.join(self.options)))
        
        self.obj_path = join("TARGET_"+target.name, "TOOLCHAIN_"+self.name)
        
        self.symbols = None
        self.labels = None
        self.has_config = False
        
        self.build_all = False

    def goanna_parse_line(self, line):
        if "analyze" in self.options:
            return self.GOANNA_DIAGNOSTIC_PATTERN.match(line)
        else:
            return None
    
    def get_symbols(self):
        if self.symbols is None:
            # Target and Toolchain symbols
            labels = self.get_labels()
            self.symbols = ["TARGET_%s" % t for t in labels['TARGET']]
            self.symbols.extend(["TOOLCHAIN_%s" % t for t in labels['TOOLCHAIN']])
            if self.has_config:
                self.symbols.append('HAVE_MBED_CONFIG_H')
            
            # Cortex CPU symbols
            if self.target.core in mbedToolchain.CORTEX_SYMBOLS:
                self.symbols.extend(mbedToolchain.CORTEX_SYMBOLS[self.target.core])
        
        return self.symbols
    
    def get_labels(self):
        if self.labels is None:
            toolchain_labels = [c.__name__ for c in getmro(self.__class__)]
            toolchain_labels.remove('mbedToolchain')
            self.labels = {
                'TARGET': self.target.get_labels(),
                'TOOLCHAIN': toolchain_labels
            }
        return self.labels
    
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
    
    def scan_resources(self, path):
        labels = self.get_labels()
        resources = Resources(path)
        self.has_config = False
        
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
                if ((d.startswith('.') or d in self.legacy_ignore_dirs) or
                    (d.startswith('TARGET_') and d[7:] not in labels['TARGET']) or
                    (d.startswith('TOOLCHAIN_') and d[10:] not in labels['TOOLCHAIN'])):
                    dirs.remove(d)
            
            # Add root to include paths
            resources.inc_dirs.append(root)
            
            for file in files:
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
                    if basename(file_path) == "mbed_config.h":
                        self.has_config = True
                    resources.headers.append(file_path)
                
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
    
    def copy_files(self, files_paths, trg_path, rel_path=None):
        # Handle a single file
        if type(files_paths) != ListType: files_paths = [files_paths]
        
        for source in files_paths:
            if rel_path is not None:
                relative_path = relpath(source, rel_path)
            else:
                _, relative_path = split(source)
            
            target = join(trg_path, relative_path)
            
            if (target != source) and (self.need_update(target, [source])):
                self.progress("copy", relative_path)
                mkdir(dirname(target))
                copyfile(source, target)
    
    def relative_object_path(self, build_path, base_dir, source):
        source_dir, name, _ = split_path(source)
        obj_dir = join(build_path, relpath(source_dir, base_dir))
        mkdir(obj_dir)
        return join(obj_dir, name + '.o')
    
    def compile_sources(self, resources, build_path, inc_dirs=None):
        # Web IDE progress bar for project build
        self.to_be_compiled = len(resources.s_sources) + len(resources.c_sources) + len(resources.cpp_sources)
        self.compiled = 0
        
        objects = []
        inc_paths = resources.inc_dirs
        if inc_dirs is not None:
            inc_paths.extend(inc_dirs)
        
        base_path = resources.base_path
        for source in resources.s_sources:
            self.compiled += 1
            object = self.relative_object_path(build_path, base_path, source)
            if self.need_update(object, [source]):
                self.progress("assemble", source, build_update=True)
                self.assemble(source, object, inc_paths)
            objects.append(object)
        
        # The dependency checking for C/C++ is delegated to the specific compiler
        for source in resources.c_sources:
            object = self.relative_object_path(build_path, base_path, source)
            self.compile_c(source, object, inc_paths)
            objects.append(object)
        
        for source in resources.cpp_sources:
            object = self.relative_object_path(build_path, base_path, source)
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
            command = cc + ['-D%s' % s for s in self.get_symbols() + self.macros] + ["-I%s" % i for i in includes] + ["-o", object, source]
            if hasattr(self, "get_dep_opt"):
                command.extend(self.get_dep_opt(dep_path))
            
            if hasattr(self, "cc_extra"):
                command.extend(self.cc_extra(base))
            
            self.debug(command)
            _, stderr, rc = run_cmd(command, dirname(object))
            
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
    
    def link_program(self, r, tmp_path, name):
        elf = join(tmp_path, name + '.elf')
        bin = join(tmp_path, name + '.bin')
        
        if self.need_update(elf, r.objects + r.libraries + [r.linker_script]):
            self.progress("link", name)
            self.link(elf, r.objects, r.libraries, r.lib_dirs, r.linker_script)
        
        if self.need_update(bin, [elf]):
            self.progress("elf2bin", name)
            self.binary(elf, bin)
            
            if self.target.name.startswith('LPC'):
                self.debug("LPC Patch %s" % (name + '.bin'))
                patch(bin)
            
        self.var("compile_succeded", True)
        self.var("binary", name+'.bin')
        
        return bin
    
    def default_cmd(self, command):
        self.debug(command)
        stdout, stderr, rc = run_cmd(command)
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


from workspace_tools.toolchains.arm import ARM_STD, ARM_MICRO
from workspace_tools.toolchains.gcc import GCC_ARM, GCC_CS, GCC_CR, GCC_CW_EWL, GCC_CW_NEWLIB
from workspace_tools.toolchains.iar import IAR

TOOLCHAIN_CLASSES = {
    'ARM': ARM_STD, 'uARM': ARM_MICRO,
    'GCC_ARM': GCC_ARM, 'GCC_CS': GCC_CS, 'GCC_CR': GCC_CR,
    'GCC_CW_EWL': GCC_CW_EWL, 'GCC_CW_NEWLIB': GCC_CW_NEWLIB,
    'IAR': IAR
}

TOOLCHAINS = set(TOOLCHAIN_CLASSES.keys())
