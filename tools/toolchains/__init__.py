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
from __future__ import print_function, division, absolute_import

import re
import sys
from os import stat, walk, getcwd, sep, remove
from copy import copy
from time import time, sleep
from shutil import copyfile
from os.path import (join, splitext, exists, relpath, dirname, basename, split,
                     abspath, isfile, isdir, normcase)
from itertools import chain
from inspect import getmro
from copy import deepcopy
from abc import ABCMeta, abstractmethod
from distutils.spawn import find_executable
from multiprocessing import Pool, cpu_count
from hashlib import md5
import fnmatch

from ..utils import (run_cmd, mkdir, rel_path, ToolException,
                    NotSupportedException, split_path, compile_worker)
from ..settings import MBED_ORG_USER, PRINT_COMPILER_OUTPUT_AS_LINK
from .. import hooks
from ..notifier.term import TerminalNotifier
from ..memap import MemapParser


#Disables multiprocessing if set to higher number than the host machine CPUs
CPU_COUNT_MIN = 1
CPU_COEF = 1

class LazyDict(object):
    def __init__(self):
        self.eager = {}
        self.lazy = {}

    def add_lazy(self, key, thunk):
        if key in self.eager:
            del self.eager[key]
        self.lazy[key] = thunk

    def __getitem__(self, key):
        if  (key not in self.eager
             and key in self.lazy):
            self.eager[key] = self.lazy[key]()
            del self.lazy[key]
        return self.eager[key]

    def __setitem__(self, key, value):
        self.eager[key] = value

    def __delitem__(self, key):
        if key in self.eager:
            del self.eager[key]
        else:
            del self.lazy[key]

    def __contains__(self, key):
        return key in self.eager or key in self.lazy

    def __iter__(self):
        return chain(iter(self.eager), iter(self.lazy))

    def __len__(self):
        return len(self.eager) + len(self.lazy)

    def __str__(self):
        return "Lazy{%s}" % (
            ", ".join("%r: %r" % (k, v) for k, v in
                      chain(self.eager.items(), ((k, "not evaluated")
                                                     for k in self.lazy))))

    def update(self, other):
        if isinstance(other, LazyDict):
            self.eager.update(other.eager)
            self.lazy.update(other.lazy)
        else:
            self.eager.update(other)

    def items(self):
        """Warning: This forces the evaluation all of the items in this LazyDict
        that are iterated over."""
        for k, v in self.eager.items():
            yield k, v
        for k in self.lazy.keys():
            yield k, self[k]

    def apply(self, fn):
        """Delay the application of a computation to all items of the lazy dict.
        Does no computation now. Instead the comuptation is performed when a
        consumer attempts to access a value in this LazyDict"""
        new_lazy = {}
        for k, f in self.lazy.items():
            def closure(f=f):
                return fn(f())
            new_lazy[k] = closure
        for k, v in self.eager.items():
            def closure(v=v):
                return fn(v)
            new_lazy[k] = closure
        self.lazy = new_lazy
        self.eager = {}

class Resources:
    def __init__(self, base_path=None, collect_ignores=False):
        self.base_path = base_path
        self.collect_ignores = collect_ignores

        self.file_basepath = {}

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
        self.lib_refs = []

        self.repo_dirs = []
        self.repo_files = []

        self.linker_script = None

        # Other files
        self.hex_files = []
        self.bin_files = []
        self.json_files = []

        # Features
        self.features = LazyDict()
        self.ignored_dirs = []

    def __add__(self, resources):
        if resources is None:
            return self
        else:
            return self.add(resources)

    def __radd__(self, resources):
        if resources is None:
            return self
        else:
            return self.add(resources)

    def ignore_dir(self, directory):
        if self.collect_ignores:
            self.ignored_dirs.append(directory)

    def add(self, resources):
        self.file_basepath.update(resources.file_basepath)

        self.inc_dirs += resources.inc_dirs
        self.headers += resources.headers

        self.s_sources += resources.s_sources
        self.c_sources += resources.c_sources
        self.cpp_sources += resources.cpp_sources

        self.lib_dirs |= resources.lib_dirs
        self.objects += resources.objects
        self.libraries += resources.libraries

        self.lib_builds += resources.lib_builds
        self.lib_refs += resources.lib_refs

        self.repo_dirs += resources.repo_dirs
        self.repo_files += resources.repo_files

        if resources.linker_script is not None:
            self.linker_script = resources.linker_script

        self.hex_files += resources.hex_files
        self.bin_files += resources.bin_files
        self.json_files += resources.json_files

        self.features.update(resources.features)
        self.ignored_dirs += resources.ignored_dirs

        return self

    def rewrite_basepath(self, file_name, export_path, loc):
        """ Replace the basepath of filename with export_path

        Positional arguments:
        file_name - the absolute path to a file
        export_path - the final destination of the file after export
        """
        new_f = join(loc, relpath(file_name, self.file_basepath[file_name]))
        self.file_basepath[new_f] = export_path
        return new_f

    def subtract_basepath(self, export_path, loc=""):
        """ Rewrite all of the basepaths with the export_path

        Positional arguments:
        export_path - the final destination of the resources with respect to the
        generated project files
        """
        keys = ['s_sources', 'c_sources', 'cpp_sources', 'hex_files',
                'objects', 'libraries', 'inc_dirs', 'headers', 'linker_script',
                'lib_dirs']
        for key in keys:
            vals = getattr(self, key)
            if isinstance(vals, set):
                vals = list(vals)
            if isinstance(vals, list):
                new_vals = []
                for val in vals:
                    new_vals.append(self.rewrite_basepath(
                        val, export_path, loc))
                if isinstance(getattr(self, key), set):
                    setattr(self, key, set(new_vals))
                else:
                    setattr(self, key, new_vals)
            elif vals:
                setattr(self, key, self.rewrite_basepath(
                    vals, export_path, loc))
        def closure(res, export_path=export_path, loc=loc):
            res.subtract_basepath(export_path, loc)
            return res
        self.features.apply(closure)

    def _collect_duplicates(self, dupe_dict, dupe_headers):
        for filename in self.s_sources + self.c_sources + self.cpp_sources:
            objname, _ = splitext(basename(filename))
            dupe_dict.setdefault(objname, set())
            dupe_dict[objname] |= set([filename])
        for filename in self.headers:
            headername = basename(filename)
            dupe_headers.setdefault(headername, set())
            dupe_headers[headername] |= set([headername])
        return dupe_dict, dupe_headers

    def detect_duplicates(self, toolchain):
        """Detect all potential ambiguities in filenames and report them with
        a toolchain notification

        Positional Arguments:
        toolchain - used for notifications
        """
        count = 0
        dupe_dict, dupe_headers = self._collect_duplicates(dict(), dict())
        for objname, filenames in dupe_dict.items():
            if len(filenames) > 1:
                count+=1
                toolchain.notify.tool_error(
                    "Object file %s.o is not unique! It could be made from: %s"\
                    % (objname, " ".join(filenames)))
        for headername, locations in dupe_headers.items():
            if len(locations) > 1:
                count+=1
                toolchain.notify.tool_error(
                    "Header file %s is not unique! It could be: %s" %\
                    (headername, " ".join(locations)))
        return count


    def relative_to(self, base, dot=False):
        for field in ['inc_dirs', 'headers', 's_sources', 'c_sources',
                      'cpp_sources', 'lib_dirs', 'objects', 'libraries',
                      'lib_builds', 'lib_refs', 'repo_dirs', 'repo_files',
                      'hex_files', 'bin_files', 'json_files']:
            v = [rel_path(f, base, dot) for f in getattr(self, field)]
            setattr(self, field, v)

        def to_apply(feature, base=base, dot=dot):
            feature.relative_to(base, dot)
        self.features.apply(to_apply)

        if self.linker_script is not None:
            self.linker_script = rel_path(self.linker_script, base, dot)

    def win_to_unix(self):
        for field in ['inc_dirs', 'headers', 's_sources', 'c_sources',
                      'cpp_sources', 'lib_dirs', 'objects', 'libraries',
                      'lib_builds', 'lib_refs', 'repo_dirs', 'repo_files',
                      'hex_files', 'bin_files', 'json_files']:
            v = [f.replace('\\', '/') for f in getattr(self, field)]
            setattr(self, field, v)

        def to_apply(feature):
            feature.win_to_unix()
        self.features.apply(to_apply)

        if self.linker_script is not None:
            self.linker_script = self.linker_script.replace('\\', '/')

    def __str__(self):
        s = []

        for (label, resources) in (
                ('Include Directories', self.inc_dirs),
                ('Headers', self.headers),

                ('Assembly sources', self.s_sources),
                ('C sources', self.c_sources),
                ('C++ sources', self.cpp_sources),

                ('Library directories', self.lib_dirs),
                ('Objects', self.objects),
                ('Libraries', self.libraries),

                ('Hex files', self.hex_files),
                ('Bin files', self.bin_files),

                ('Features', self.features),
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
    'ARM', 'uARM', 'IAR',
    'GCC_ARM', 'GCC_CS', 'GCC_CR', 'GCC_CW', 'GCC_CW_EWL', 'GCC_CW_NEWLIB',
    'ARMC6'
])
LEGACY_TOOLCHAIN_NAMES = {
    'ARM_STD':'ARM', 'ARM_MICRO': 'uARM',
    'GCC_ARM': 'GCC_ARM', 'GCC_CR': 'GCC_CR',
    'IAR': 'IAR',
    'ARMC6': 'ARMC6',
}


class mbedToolchain:
    # Verbose logging
    VERBOSE = True

    # Compile C files as CPP
    COMPILE_C_AS_CPP = False

    # Response files for compiling, includes, linking and archiving.
    # Not needed on posix systems where the typical arg limit is 2 megabytes
    RESPONSE_FILES = True

    CORTEX_SYMBOLS = {
        "Cortex-M0" : ["__CORTEX_M0", "ARM_MATH_CM0", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M0+": ["__CORTEX_M0PLUS", "ARM_MATH_CM0PLUS", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M1" : ["__CORTEX_M3", "ARM_MATH_CM1", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M3" : ["__CORTEX_M3", "ARM_MATH_CM3", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M4" : ["__CORTEX_M4", "ARM_MATH_CM4", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M4F" : ["__CORTEX_M4", "ARM_MATH_CM4", "__FPU_PRESENT=1", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M7" : ["__CORTEX_M7", "ARM_MATH_CM7", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M7F" : ["__CORTEX_M7", "ARM_MATH_CM7", "__FPU_PRESENT=1", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M7FD" : ["__CORTEX_M7", "ARM_MATH_CM7", "__FPU_PRESENT=1", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-A9" : ["__CORTEX_A9", "ARM_MATH_CA9", "__FPU_PRESENT", "__CMSIS_RTOS", "__EVAL", "__MBED_CMSIS_RTOS_CA9"],
        "Cortex-M23-NS": ["__CORTEX_M23", "ARM_MATH_ARMV8MBL", "DOMAIN_NS=1", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M23": ["__CORTEX_M23", "ARM_MATH_ARMV8MBL", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M33-NS": ["__CORTEX_M33", "ARM_MATH_ARMV8MML", "DOMAIN_NS=1", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M33": ["__CORTEX_M33", "ARM_MATH_ARMV8MML", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M33F-NS": ["__CORTEX_M33", "ARM_MATH_ARMV8MML", "DOMAIN_NS=1", "__FPU_PRESENT", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
        "Cortex-M33F": ["__CORTEX_M33", "ARM_MATH_ARMV8MML", "__FPU_PRESENT", "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
    }

    MBED_CONFIG_FILE_NAME="mbed_config.h"

    PROFILE_FILE_NAME = ".profile"

    __metaclass__ = ABCMeta

    profile_template = {'common':[], 'c':[], 'cxx':[], 'asm':[], 'ld':[]}

    def __init__(self, target, notify=None, macros=None, build_profile=None,
                 build_dir=None):
        self.target = target
        self.name = self.__class__.__name__

        # compile/assemble/link/binary hooks
        self.hook = hooks.Hook(target, self)

        # Toolchain flags
        self.flags = deepcopy(build_profile or self.profile_template)

        # System libraries provided by the toolchain
        self.sys_libs = []

        # User-defined macros
        self.macros = macros or []

        # Macros generated from toolchain and target rules/features
        self.asm_symbols = None
        self.cxx_symbols = None

        # Labels generated from toolchain and target rules/features (used for selective build)
        self.labels = None

        # This will hold the initialized config object
        self.config = None

        # This will hold the configuration data (as returned by Config.get_config_data())
        self.config_data = None

        # This will hold the location of the configuration file or None if there's no configuration available
        self.config_file = None

        # Call guard for "get_config_data" (see the comments of get_config_data for details)
        self.config_processed = False

        # Non-incremental compile
        self.build_all = False

        # Build output dir
        self.build_dir = build_dir
        self.timestamp = time()

        # Number of concurrent build jobs. 0 means auto (based on host system cores)
        self.jobs = 0

        # Ignore patterns from .mbedignore files
        self.ignore_patterns = []
        self._ignore_regex = re.compile("$^")

        # Pre-mbed 2.0 ignore dirs
        self.legacy_ignore_dirs = (LEGACY_IGNORE_DIRS | TOOLCHAINS) - set([target.name, LEGACY_TOOLCHAIN_NAMES[self.name]])

        # Output notify function
        # This function is passed all events, and expected to handle notification of the
        # user, emit the events to a log, etc.
        # The API for all notify methods passed into the notify parameter is as follows:
        # def notify(Event, Silent)
        # Where *Event* is a dict representing the toolchain event that was generated
        #            e.g.: a compile succeeded, or a warning was emitted by the compiler
        #                  or an application was linked
        #       *Silent* is a boolean
        if notify:
            self.notify = notify
        else:
            self.notify = TerminalNotifier()

        # uVisor spepcific rules
        if 'UVISOR' in self.target.features and 'UVISOR_SUPPORTED' in self.target.extra_labels:
            self.target.core = re.sub(r"F$", '', self.target.core)

        # Stats cache is used to reduce the amount of IO requests to stat
        # header files during dependency change. See need_update()
        self.stat_cache = {}

        # Used by the mbed Online Build System to build in chrooted environment
        self.CHROOT = None

        # Call post __init__() hooks before the ARM/GCC_ARM/IAR toolchain __init__() takes over
        self.init()

    # Used for post __init__() hooks
    # THIS METHOD IS BEING OVERRIDDEN BY THE MBED ONLINE BUILD SYSTEM
    # ANY CHANGE OF PARAMETERS OR RETURN VALUES WILL BREAK COMPATIBILITY
    def init(self):
        return True

    def get_output(self):
        return self.notifier.get_output()

    def get_symbols(self, for_asm=False):
        if for_asm:
            if self.asm_symbols is None:
                self.asm_symbols = []

                # Cortex CPU symbols
                if self.target.core in mbedToolchain.CORTEX_SYMBOLS:
                    self.asm_symbols.extend(mbedToolchain.CORTEX_SYMBOLS[self.target.core])

                # Add target's symbols
                self.asm_symbols += self.target.macros
                # Add extra symbols passed via 'macros' parameter
                self.asm_symbols += self.macros
            return list(set(self.asm_symbols))  # Return only unique symbols
        else:
            if self.cxx_symbols is None:
                # Target and Toolchain symbols
                labels = self.get_labels()
                self.cxx_symbols = ["TARGET_%s" % t for t in labels['TARGET']]
                self.cxx_symbols.extend(["TOOLCHAIN_%s" % t for t in labels['TOOLCHAIN']])

                # Cortex CPU symbols
                if self.target.core in mbedToolchain.CORTEX_SYMBOLS:
                    self.cxx_symbols.extend(mbedToolchain.CORTEX_SYMBOLS[self.target.core])

                # Symbols defined by the on-line build.system
                self.cxx_symbols.extend(['MBED_BUILD_TIMESTAMP=%s' % self.timestamp, 'TARGET_LIKE_MBED', '__MBED__=1'])
                if MBED_ORG_USER:
                    self.cxx_symbols.append('MBED_USERNAME=' + MBED_ORG_USER)

                # Add target's symbols
                self.cxx_symbols += self.target.macros
                # Add target's hardware
                self.cxx_symbols += ["DEVICE_" + data + "=1" for data in self.target.device_has]
                # Add target's features
                self.cxx_symbols += ["FEATURE_" + data + "=1" for data in self.target.features]
                # Add extra symbols passed via 'macros' parameter
                self.cxx_symbols += self.macros

                # Form factor variables
                if hasattr(self.target, 'supported_form_factors'):
                    self.cxx_symbols.extend(["TARGET_FF_%s" % t for t in self.target.supported_form_factors])

            return list(set(self.cxx_symbols))  # Return only unique symbols

    # Extend the internal list of macros
    def add_macros(self, new_macros):
        self.macros.extend(new_macros)

    def get_labels(self):
        if self.labels is None:
            toolchain_labels = self._get_toolchain_labels()
            self.labels = {
                'TARGET': self.target.labels,
                'FEATURE': self.target.features,
                'TOOLCHAIN': toolchain_labels
            }

            # This is a policy decision and it should /really/ be in the config system
            # ATM it's here for backward compatibility
            if ((("-g" in self.flags['common'] or "-g3" in self.flags['common']) and
                 "-O0" in self.flags['common']) or
                ("-r" in self.flags['common'] and
                 "-On" in self.flags['common'])):
                self.labels['TARGET'].append("DEBUG")
            else:
                self.labels['TARGET'].append("RELEASE")
        return self.labels

    def _get_toolchain_labels(self):
        toolchain_labels = [c.__name__ for c in getmro(self.__class__)]
        toolchain_labels.remove('mbedToolchain')
        toolchain_labels.remove('object')
        return toolchain_labels


    # Determine whether a source file needs updating/compiling
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

            if d not in self.stat_cache:
                self.stat_cache[d] = stat(d).st_mtime

            if self.stat_cache[d] >= target_mod_time:
                return True

        return False

    def is_ignored(self, file_path):
        """Check if file path is ignored by any .mbedignore thus far"""
        return self._ignore_regex.match(normcase(file_path))

    def add_ignore_patterns(self, root, base_path, patterns):
        """Add a series of patterns to the ignored paths

        Positional arguments:
        root - the directory containing the ignore file
        base_path - the location that the scan started from
        patterns - the list of patterns we will ignore in the future
        """
        real_base = relpath(root, base_path)
        if real_base == ".":
            self.ignore_patterns.extend(normcase(p) for p in patterns)
        else:
            self.ignore_patterns.extend(normcase(join(real_base, pat)) for pat in patterns)
        if self.ignore_patterns:
            self._ignore_regex = re.compile("|".join(fnmatch.translate(p) for p in self.ignore_patterns))

    # Create a Resources object from the path pointed to by *path* by either traversing a
    # a directory structure, when *path* is a directory, or adding *path* to the resources,
    # when *path* is a file.
    # The parameter *base_path* is used to set the base_path attribute of the Resources
    # object and the parameter *exclude_paths* is used by the directory traversal to
    # exclude certain paths from the traversal.
    def scan_resources(self, path, exclude_paths=None, base_path=None,
                       collect_ignores=False):
        self.progress("scan", path)

        resources = Resources(path, collect_ignores=collect_ignores)
        if not base_path:
            if isfile(path):
                base_path = dirname(path)
            else:
                base_path = path
        resources.base_path = base_path

        if isfile(path):
            self._add_file(path, resources, base_path, exclude_paths=exclude_paths)
        else:
            self._add_dir(path, resources, base_path, exclude_paths=exclude_paths)
        return resources

    # A helper function for scan_resources. _add_dir traverses *path* (assumed to be a
    # directory) and heeds the ".mbedignore" files along the way. _add_dir calls _add_file
    # on every file it considers adding to the resources object.
    def _add_dir(self, path, resources, base_path, exclude_paths=None):
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
        labels = self.get_labels()
        for root, dirs, files in walk(path, followlinks=True):
            # Check if folder contains .mbedignore
            if ".mbedignore" in files:
                with open (join(root,".mbedignore"), "r") as f:
                    lines=f.readlines()
                    lines = [l.strip() for l in lines] # Strip whitespaces
                    lines = [l for l in lines if l != ""] # Strip empty lines
                    lines = [l for l in lines if not re.match("^#",l)] # Strip comment lines
                    # Append root path to glob patterns and append patterns to ignore_patterns
                    self.add_ignore_patterns(root, base_path, lines)

            # Skip the whole folder if ignored, e.g. .mbedignore containing '*'
            root_path =join(relpath(root, base_path))
            if  (self.is_ignored(join(root_path,"")) or
                 self.build_dir == root_path):
                resources.ignore_dir(root_path)
                dirs[:] = []
                continue

            for d in copy(dirs):
                dir_path = join(root, d)
                # Add internal repo folders/files. This is needed for exporters
                if d == '.hg' or d == '.git':
                    resources.repo_dirs.append(dir_path)

                if ((d.startswith('.') or d in self.legacy_ignore_dirs) or
                    # Ignore targets that do not match the TARGET in extra_labels list
                    (d.startswith('TARGET_') and d[7:] not in labels['TARGET']) or
                    # Ignore toolchain that do not match the current TOOLCHAIN
                    (d.startswith('TOOLCHAIN_') and d[10:] not in labels['TOOLCHAIN']) or
                    # Ignore .mbedignore files
                    self.is_ignored(join(relpath(root, base_path), d,"")) or
                    # Ignore TESTS dir
                    (d == 'TESTS')):
                        resources.ignore_dir(dir_path)
                        dirs.remove(d)
                elif d.startswith('FEATURE_'):
                    # Recursively scan features but ignore them in the current scan.
                    # These are dynamically added by the config system if the conditions are matched
                    def closure (dir_path=dir_path, base_path=base_path):
                        return self.scan_resources(dir_path, base_path=base_path,
                                                   collect_ignores=resources.collect_ignores)
                    resources.features.add_lazy(d[8:], closure)
                    resources.ignore_dir(dir_path)
                    dirs.remove(d)
                elif exclude_paths:
                    for exclude_path in exclude_paths:
                        rel_path = relpath(dir_path, exclude_path)
                        if not (rel_path.startswith('..')):
                            resources.ignore_dir(dir_path)
                            dirs.remove(d)
                            break

            # Add root to include paths
            root = root.rstrip("/")
            resources.inc_dirs.append(root)
            resources.file_basepath[root] = base_path

            for file in files:
                file_path = join(root, file)
                self._add_file(file_path, resources, base_path)

    # A helper function for both scan_resources and _add_dir. _add_file adds one file
    # (*file_path*) to the resources object based on the file type.
    def _add_file(self, file_path, resources, base_path, exclude_paths=None):

        if  (self.is_ignored(relpath(file_path, base_path)) or
             basename(file_path).startswith(".")):
            resources.ignore_dir(relpath(file_path, base_path))
            return

        resources.file_basepath[file_path] = base_path
        _, ext = splitext(file_path)
        ext = ext.lower()

        if   ext == '.s':
            resources.s_sources.append(file_path)

        elif ext == '.c':
            resources.c_sources.append(file_path)

        elif ext == '.cpp':
            resources.cpp_sources.append(file_path)

        elif ext == '.h' or ext == '.hpp':
            resources.headers.append(file_path)

        elif ext == '.o':
            resources.objects.append(file_path)

        elif ext == self.LIBRARY_EXT:
            resources.libraries.append(file_path)
            resources.lib_dirs.add(dirname(file_path))

        elif ext == self.LINKER_EXT:
            if resources.linker_script is not None:
                self.notify.info("Warning: Multiple linker scripts detected: %s -> %s" % (resources.linker_script, file_path))
            resources.linker_script = file_path

        elif ext == '.lib':
            resources.lib_refs.append(file_path)

        elif ext == '.bld':
            resources.lib_builds.append(file_path)

        elif basename(file_path) == '.hgignore':
            resources.repo_files.append(file_path)

        elif basename(file_path) == '.gitignore':
            resources.repo_files.append(file_path)

        elif ext == '.hex':
            resources.hex_files.append(file_path)

        elif ext == '.bin':
            resources.bin_files.append(file_path)

        elif ext == '.json':
            resources.json_files.append(file_path)


    def scan_repository(self, path):
        resources = []

        for root, dirs, files in walk(path):
            # Remove ignored directories
            for d in copy(dirs):
                if d == '.' or d == '..':
                    dirs.remove(d)

            for file in files:
                file_path = join(root, file)
                resources.append(file_path)

        return resources

    def copy_files(self, files_paths, trg_path, resources=None, rel_path=None):
        # Handle a single file
        if not isinstance(files_paths, list):
            files_paths = [files_paths]

        for source in files_paths:
            if source is None:
                files_paths.remove(source)

        for source in files_paths:
            if resources is not None and source in resources.file_basepath:
                relative_path = relpath(source, resources.file_basepath[source])
            elif rel_path is not None:
                relative_path = relpath(source, rel_path)
            else:
                _, relative_path = split(source)

            target = join(trg_path, relative_path)

            if (target != source) and (self.need_update(target, [source])):
                self.progress("copy", relative_path)
                mkdir(dirname(target))
                copyfile(source, target)

    # THIS METHOD IS BEING OVERRIDDEN BY THE MBED ONLINE BUILD SYSTEM
    # ANY CHANGE OF PARAMETERS OR RETURN VALUES WILL BREAK COMPATIBILITY
    def relative_object_path(self, build_path, base_dir, source):
        source_dir, name, _ = split_path(source)

        obj_dir = relpath(join(build_path, relpath(source_dir, base_dir)))
        if obj_dir is not self.prev_dir:
            self.prev_dir = obj_dir
            mkdir(obj_dir)
        return join(obj_dir, name + '.o')

    # Generate response file for all includes.
    # ARM, GCC, IAR cross compatible
    def get_inc_file(self, includes):
        include_file = join(self.build_dir, ".includes_%s.txt" % self.inc_md5)
        if not exists(include_file):
            with open(include_file, "w") as f:
                cmd_list = []
                for c in includes:
                    if c:
                        c = c.replace("\\", "/")
                        if self.CHROOT:
                            c = c.replace(self.CHROOT, '')
                        cmd_list.append('"-I%s"' % c)
                string = " ".join(cmd_list)
                f.write(string)
        return include_file

    # Generate response file for all objects when linking.
    # ARM, GCC, IAR cross compatible
    def get_link_file(self, cmd):
        link_file = join(self.build_dir, ".link_files.txt")
        with open(link_file, "w") as f:
            cmd_list = []
            for c in cmd:
                if c:
                    c = c.replace("\\", "/")
                    if self.CHROOT:
                        c = c.replace(self.CHROOT, '')
                    cmd_list.append(('"%s"' % c) if not c.startswith('-') else c)
            string = " ".join(cmd_list)
            f.write(string)
        return link_file

    # Generate response file for all objects when archiving.
    # ARM, GCC, IAR cross compatible
    def get_arch_file(self, objects):
        archive_file = join(self.build_dir, ".archive_files.txt")
        with open(archive_file, "w") as f:
            o_list = []
            for o in objects:
                o_list.append('"%s"' % o)
            string = " ".join(o_list).replace("\\", "/")
            f.write(string)
        return archive_file

    # THIS METHOD IS BEING CALLED BY THE MBED ONLINE BUILD SYSTEM
    # ANY CHANGE OF PARAMETERS OR RETURN VALUES WILL BREAK COMPATIBILITY
    def compile_sources(self, resources, inc_dirs=None):
        # Web IDE progress bar for project build
        files_to_compile = resources.s_sources + resources.c_sources + resources.cpp_sources
        self.to_be_compiled = len(files_to_compile)
        self.compiled = 0

        self.notify.cc_verbose("Macros: "+' '.join(['-D%s' % s for s in self.get_symbols()]))

        inc_paths = resources.inc_dirs
        if inc_dirs is not None:
            if isinstance(inc_dirs, list):
                inc_paths.extend(inc_dirs)
            else:
                inc_paths.append(inc_dirs)
        # De-duplicate include paths
        inc_paths = set(inc_paths)
        # Sort include paths for consistency
        inc_paths = sorted(set(inc_paths))
        # Unique id of all include paths
        self.inc_md5 = md5(' '.join(inc_paths).encode('utf-8')).hexdigest()

        objects = []
        queue = []
        work_dir = getcwd()
        self.prev_dir = None

        # Generate configuration header (this will update self.build_all if needed)
        self.get_config_header()
        self.dump_build_profile()

        # Sort compile queue for consistency
        files_to_compile.sort()
        for source in files_to_compile:
            object = self.relative_object_path(
                self.build_dir, resources.file_basepath[source], source)

            # Queue mode (multiprocessing)
            commands = self.compile_command(source, object, inc_paths)
            if commands is not None:
                queue.append({
                    'source': source,
                    'object': object,
                    'commands': commands,
                    'work_dir': work_dir,
                    'chroot': self.CHROOT
                })
            else:
                self.compiled += 1
                objects.append(object)

        # Use queues/multiprocessing if cpu count is higher than setting
        jobs = self.jobs if self.jobs else cpu_count()
        if jobs > CPU_COUNT_MIN and len(queue) > jobs:
            return self.compile_queue(queue, objects)
        else:
            return self.compile_seq(queue, objects)

    # Compile source files queue in sequential order
    def compile_seq(self, queue, objects):
        for item in queue:
            result = compile_worker(item)

            self.compiled += 1
            self.progress("compile", item['source'], build_update=True)
            for res in result['results']:
                self.notify.cc_verbose("Compile: %s" % ' '.join(res['command']), result['source'])
                self.compile_output([
                    res['code'],
                    res['output'],
                    res['command']
                ])
            objects.append(result['object'])
        return objects

    # Compile source files queue in parallel by creating pool of worker threads
    def compile_queue(self, queue, objects):
        jobs_count = int(self.jobs if self.jobs else cpu_count() * CPU_COEF)
        p = Pool(processes=jobs_count)

        results = []
        for i in range(len(queue)):
            results.append(p.apply_async(compile_worker, [queue[i]]))
        p.close()

        itr = 0
        while len(results):
            itr += 1
            if itr > 180000:
                p.terminate()
                p.join()
                raise ToolException("Compile did not finish in 5 minutes")

            sleep(0.01)
            pending = 0
            for r in results:
                if r.ready():
                    try:
                        result = r.get()
                        results.remove(r)

                        self.compiled += 1
                        self.progress("compile", result['source'], build_update=True)
                        for res in result['results']:
                            self.notify.cc_verbose("Compile: %s" % ' '.join(res['command']), result['source'])
                            self.compile_output([
                                res['code'],
                                res['output'],
                                res['command']
                            ])
                        objects.append(result['object'])
                    except ToolException as err:
                        if p._taskqueue.queue:
                            p._taskqueue.queue.clear()
                            sleep(0.5)
                        p.terminate()
                        p.join()
                        raise ToolException(err)
                else:
                    pending += 1
                    if pending >= jobs_count:
                        break

        results = None
        p.join()

        return objects

    # Determine the compile command based on type of source file
    def compile_command(self, source, object, includes):
        # Check dependencies
        _, ext = splitext(source)
        ext = ext.lower()

        if ext == '.c' or  ext == '.cpp':
            base, _ = splitext(object)
            dep_path = base + '.d'
            try:
                deps = self.parse_dependencies(dep_path) if (exists(dep_path)) else []
            except (IOError, IndexError):
                deps = []
            config_file = ([self.config.app_config_location]
                           if self.config.app_config_location else [])
            deps.extend(config_file)
            if ext == '.cpp' or self.COMPILE_C_AS_CPP:
                deps.append(join(self.build_dir, self.PROFILE_FILE_NAME + "-cxx"))
            else:
                deps.append(join(self.build_dir, self.PROFILE_FILE_NAME + "-c"))
            if len(deps) == 0 or self.need_update(object, deps):
                if ext == '.cpp' or self.COMPILE_C_AS_CPP:
                    return self.compile_cpp(source, object, includes)
                else:
                    return self.compile_c(source, object, includes)
        elif ext == '.s':
            deps = [source]
            deps.append(join(self.build_dir, self.PROFILE_FILE_NAME + "-asm"))
            if self.need_update(object, deps):
                return self.assemble(source, object, includes)
        else:
            return False

        return None

    def parse_dependencies(self, dep_path):
        """Parse the dependency information generated by the compiler.

        Positional arguments:
        dep_path -- the path to a file generated by a previous run of the compiler

        Return value:
        A list of all source files that the dependency file indicated were dependencies

        Side effects:
        None

        Note: A default implementation is provided for make-like file formats
        """
        dependencies = []
        buff = open(dep_path).readlines()
        if buff:
            buff[0] = re.sub('^(.*?)\: ', '', buff[0])
            for line in buff:
                filename = line.replace('\\\n', '').strip()
                if filename:
                    filename = filename.replace('\\ ', '\a')
                    dependencies.extend(((self.CHROOT if self.CHROOT else '') +
                                         f.replace('\a', ' '))
                                        for f in filename.split(" "))
        return list(filter(None, dependencies))

    def is_not_supported_error(self, output):
        return "#error directive: [NOT_SUPPORTED]" in output

    @abstractmethod
    def parse_output(self, output):
        """Take in compiler output and extract sinlge line warnings and errors from it.

        Positional arguments:
        output -- a string of all the messages emitted by a run of the compiler

        Return value:
        None

        Side effects:
        call self.cc_info or self.notify with a description of the event generated by the compiler
        """
        raise NotImplemented

    def compile_output(self, output=[]):
        _rc = output[0]
        _stderr = output[1].decode("utf-8")
        command = output[2]

        # Parse output for Warnings and Errors
        self.parse_output(_stderr)
        self.notify.debug("Return: %s"% _rc)
        for error_line in _stderr.splitlines():
            self.notify.debug("Output: %s"% error_line)

        # Check return code
        if _rc != 0:
            if self.is_not_supported_error(_stderr):
                raise NotSupportedException(_stderr)
            else:
                raise ToolException(_stderr)

    def build_library(self, objects, dir, name):
        needed_update = False
        lib = self.STD_LIB_NAME % name
        fout = join(dir, lib)
        if self.need_update(fout, objects):
            self.notify.info("Library: %s" % lib)
            self.archive(objects, fout)
            needed_update = True

        return needed_update

    def link_program(self, r, tmp_path, name):
        needed_update = False
        ext = 'bin'
        if hasattr(self.target, 'OUTPUT_EXT'):
            ext = self.target.OUTPUT_EXT

        if hasattr(self.target, 'OUTPUT_NAMING'):
            self.notify.var("binary_naming", self.target.OUTPUT_NAMING)
            if self.target.OUTPUT_NAMING == "8.3":
                name = name[0:8]
                ext = ext[0:3]

        # Create destination directory
        head, tail =  split(name)
        new_path = join(tmp_path, head)
        mkdir(new_path)

        filename = name+'.'+ext
        # Absolute path of the final linked file
        full_path = join(tmp_path, filename)
        elf = join(tmp_path, name + '.elf')
        bin = None if ext == 'elf' else full_path
        map = join(tmp_path, name + '.map')

        r.objects = sorted(set(r.objects))
        config_file = ([self.config.app_config_location]
                       if self.config.app_config_location else [])
        dependencies = r.objects + r.libraries + [r.linker_script] + config_file
        dependencies.append(join(self.build_dir, self.PROFILE_FILE_NAME + "-ld"))
        if self.need_update(elf, dependencies):
            needed_update = True
            self.progress("link", name)
            self.link(elf, r.objects, r.libraries, r.lib_dirs, r.linker_script)

        if bin and self.need_update(bin, [elf]):
            needed_update = True
            self.progress("elf2bin", name)
            self.binary(r, elf, bin)

        # Initialize memap and process map file. This doesn't generate output.
        self.mem_stats(map)

        self.notify.var("compile_succeded", True)
        self.notify.var("binary", filename)

        return full_path, needed_update

    # THIS METHOD IS BEING OVERRIDDEN BY THE MBED ONLINE BUILD SYSTEM
    # ANY CHANGE OF PARAMETERS OR RETURN VALUES WILL BREAK COMPATIBILITY
    def default_cmd(self, command):
        _stdout, _stderr, _rc = run_cmd(command, work_dir=getcwd(), chroot=self.CHROOT)
        self.notify.debug("Return: %s"% _rc)

        for output_line in _stdout.splitlines():
            self.notify.debug("Output: %s"% output_line)
        for error_line in _stderr.splitlines():
            self.notify.debug("Errors: %s"% error_line)

        if _rc != 0:
            for line in _stderr.splitlines():
                self.notify.tool_error(line)
            raise ToolException(_stderr)

    def progress(self, action, file, build_update=False):
        if build_update:
            percent = 100. * float(self.compiled) / float(self.to_be_compiled)
        else:
            percent = None
        self.notify.progress(action, file, percent)

    # THIS METHOD IS BEING OVERRIDDEN BY THE MBED ONLINE BUILD SYSTEM
    # ANY CHANGE OF PARAMETERS OR RETURN VALUES WILL BREAK COMPATIBILITY
    def mem_stats(self, map):
        """! Creates parser object
        @param map Path to linker map file to parse and decode
        @return None
        """
        toolchain = self.__class__.__name__

        # Create memap object
        memap = MemapParser()

        # Parse and decode a map file
        if memap.parse(abspath(map), toolchain) is False:
            self.notify.info("Unknown toolchain for memory statistics %s" % toolchain)
            return None

        # Store the memap instance for later use
        self.memap_instance = memap

        # Note: memory statistics are not returned.
        # Need call to generate_output later (depends on depth & output format)

        return None

    def add_regions(self):
        """Add regions to the build profile, if there are any.
        """
        regions = list(self.config.regions)
        self.notify.info("Using regions %s in this build."
                         % ", ".join(region.name for region in regions))
        for region in regions:
            for define in [(region.name.upper() + "_ADDR", region.start),
                           (region.name.upper() + "_SIZE", region.size)]:
                define_string = "-D%s=0x%x" %  define
                self.cc.append(define_string)
                self.cppc.append(define_string)
                self.flags["common"].append(define_string)
            if region.active:
                for define in [("MBED_APP_START", region.start),
                               ("MBED_APP_SIZE", region.size)]:
                    define_string = self.make_ld_define(*define)
                    self.ld.append(define_string)
                    self.flags["ld"].append(define_string)
            self.notify.info("  Region %s: size 0x%x, offset 0x%x"
                             % (region.name, region.size, region.start))

    # Set the configuration data
    def set_config_data(self, config_data):
        self.config_data = config_data
        if self.config.has_regions:
            self.add_regions()

    # Creates the configuration header if needed:
    # - if there is no configuration data, "mbed_config.h" is not create (or deleted if it exists).
    # - if there is configuration data and "mbed_config.h" does not exist, it is created.
    # - if there is configuration data similar to the previous configuration data,
    #   "mbed_config.h" is left untouched.
    # - if there is new configuration data, "mbed_config.h" is overriden.
    # The function needs to be called exactly once for the lifetime of this toolchain instance.
    # The "config_processed" variable (below) ensures this behaviour.
    # The function returns the location of the configuration file, or None if there is no
    # configuration data available (and thus no configuration file)
    def get_config_header(self):
        if self.config_processed: # this function was already called, return its result
            return self.config_file
        # The config file is located in the build directory
        self.config_file = join(self.build_dir, self.MBED_CONFIG_FILE_NAME)
        # If the file exists, read its current content in prev_data
        if exists(self.config_file):
            with open(self.config_file, "r") as f:
                prev_data = f.read()
        else:
            prev_data = None
        # Get the current configuration data
        crt_data = self.config.config_to_header(self.config_data) if self.config_data else None
        # "changed" indicates if a configuration change was detected
        changed = False
        if prev_data is not None: # a previous mbed_config.h exists
            if crt_data is None: # no configuration data, so "mbed_config.h" needs to be removed
                remove(self.config_file)
                self.config_file = None # this means "config file not present"
                changed = True
            elif crt_data != prev_data: # different content of config file
                with open(self.config_file, "w") as f:
                    f.write(crt_data)
                changed = True
        else: # a previous mbed_config.h does not exist
            if crt_data is not None: # there's configuration data available
                with open(self.config_file, "w") as f:
                    f.write(crt_data)
                changed = True
            else:
                self.config_file = None # this means "config file not present"
        # If there was a change in configuration, rebuild everything
        self.build_all = changed
        # Make sure that this function will only return the location of the configuration
        # file for subsequent calls, without trying to manipulate its content in any way.
        self.config_processed = True
        return self.config_file

    def dump_build_profile(self):
        """Dump the current build profile and macros into the `.profile` file
        in the build directory"""
        for key in ["cxx", "c", "asm", "ld"]:
            to_dump = (str(self.flags[key]) + str(sorted(self.macros)))
            if key in ["cxx", "c"]:
                to_dump += str(self.flags['common'])
            where = join(self.build_dir, self.PROFILE_FILE_NAME + "-" + key)
            self._overwrite_when_not_equal(where, to_dump)

    @staticmethod
    def _overwrite_when_not_equal(filename, content):
        if not exists(filename) or content != open(filename).read():
            with open(filename, "w") as out:
                out.write(content)

    @staticmethod
    def generic_check_executable(tool_key, executable_name, levels_up,
                                 nested_dir=None):
        """
        Positional args:
        tool_key: the key to index TOOLCHAIN_PATHS
        executable_name: the toolchain's named executable (ex. armcc)
        levels_up: each toolchain joins the toolchain_path, some
        variable directories (bin, include), and the executable name,
        so the TOOLCHAIN_PATH value must be appropriately distanced

        Keyword args:
        nested_dir: the directory within TOOLCHAIN_PATHS where the executable
          is found (ex: 'bin' for ARM\bin\armcc (necessary to check for path
          that will be used by toolchain's compile)

        Returns True if the executable location specified by the user
        exists and is valid OR the executable can be found on the PATH.
        Returns False otherwise.
        """
        # Search PATH if user did not specify a path or specified path doesn't
        # exist.
        if not TOOLCHAIN_PATHS[tool_key] or not exists(TOOLCHAIN_PATHS[tool_key]):
            exe = find_executable(executable_name)
            if not exe:
                return False
            for level in range(levels_up):
                # move up the specified number of directories
                exe = dirname(exe)
            TOOLCHAIN_PATHS[tool_key] = exe
        if nested_dir:
            subdir = join(TOOLCHAIN_PATHS[tool_key], nested_dir,
                          executable_name)
        else:
            subdir = join(TOOLCHAIN_PATHS[tool_key],executable_name)
        # User could have specified a path that exists but does not contain exe
        return exists(subdir) or exists(subdir +'.exe')

    @abstractmethod
    def check_executable(self):
        """Returns True if the executable (armcc) location specified by the
         user exists OR the executable can be found on the PATH.
         Returns False otherwise."""
        raise NotImplemented

    @abstractmethod
    def get_config_option(self, config_header):
        """Generate the compiler option that forces the inclusion of the configuration
        header file.

        Positional arguments:
        config_header -- The configuration header that will be included within all source files

        Return value:
        A list of the command line arguments that will force the inclusion the specified header

        Side effects:
        None
        """
        raise NotImplemented

    @abstractmethod
    def get_compile_options(self, defines, includes, for_asm=False):
        """Generate the compiler options from the defines and includes

        Positional arguments:
        defines -- The preprocessor macros defined on the command line
        includes -- The include file search paths

        Keyword arguments:
        for_asm -- generate the assembler options instead of the compiler options

        Return value:
        A list of the command line arguments that will force the inclusion the specified header

        Side effects:
        None
        """
        raise NotImplemented

    @abstractmethod
    def assemble(self, source, object, includes):
        """Generate the command line that assembles.

        Positional arguments:
        source -- a file path that is the file to assemble
        object -- a file path that is the destination object
        includes -- a list of all directories where header files may be found

        Return value:
        The complete command line, as a list, that would invoke the assembler
        on the source file, include all the include paths, and generate
        the specified object file.

        Side effects:
        None

        Note:
        This method should be decorated with @hook_tool.
        """
        raise NotImplemented

    @abstractmethod
    def compile_c(self, source, object, includes):
        """Generate the command line that compiles a C source file.

        Positional arguments:
        source -- the C source file to compile
        object -- the destination object file
        includes -- a list of all the directories where header files may be found

        Return value:
        The complete command line, as a list, that would invoke the C compiler
        on the source file, include all the include paths, and generate the
        specified object file.

        Side effects:
        None

        Note:
        This method should be decorated with @hook_tool.
        """
        raise NotImplemented

    @abstractmethod
    def compile_cpp(self, source, object, includes):
        """Generate the command line that compiles a C++ source file.

        Positional arguments:
        source -- the C++ source file to compile
        object -- the destination object file
        includes -- a list of all the directories where header files may be found

        Return value:
        The complete command line, as a list, that would invoke the C++ compiler
        on the source file, include all the include paths, and generate the
        specified object file.

        Side effects:
        None

        Note:
        This method should be decorated with @hook_tool.
        """
        raise NotImplemented

    @abstractmethod
    def link(self, output, objects, libraries, lib_dirs, mem_map):
        """Run the linker to create an executable and memory map.

        Positional arguments:
        output -- the file name to place the executable in
        objects -- all of the object files to link
        libraries -- all of the required libraries
        lib_dirs -- where the required libraries are located
        mem_map -- the location where the memory map file should be stored

        Return value:
        None

        Side effect:
        Runs the linker to produce the executable.

        Note:
        This method should be decorated with @hook_tool.
        """
        raise NotImplemented

    @abstractmethod
    def archive(self, objects, lib_path):
        """Run the command line that creates an archive.

        Positional arguhments:
        objects -- a list of all the object files that should be archived
        lib_path -- the file name of the resulting library file

        Return value:
        None

        Side effect:
        Runs the archiving tool to produce the library file.

        Note:
        This method should be decorated with @hook_tool.
        """
        raise NotImplemented

    @abstractmethod
    def binary(self, resources, elf, bin):
        """Run the command line that will Extract a simplified binary file.

        Positional arguments:
        resources -- A resources object (Is not used in any of the toolchains)
        elf -- the executable file that is to be converted
        bin -- the file name of the to be created simplified binary file

        Return value:
        None

        Side effect:
        Runs the elf2bin tool to produce the simplified binary file.

        Note:
        This method should be decorated with @hook_tool.
        """
        raise NotImplemented

    @staticmethod
    @abstractmethod
    def name_mangle(name):
        """Mangle a name based on the conventional name mangling of this toolchain

        Positional arguments:
        name -- the name to mangle

        Return:
        the mangled name as a string
        """
        raise NotImplemented

    @staticmethod
    @abstractmethod
    def make_ld_define(name, value):
        """Create an argument to the linker that would define a symbol

        Positional arguments:
        name -- the symbol to define
        value -- the value to give the symbol

        Return:
        The linker flag as a string
        """
        raise NotImplemented

    @staticmethod
    @abstractmethod
    def redirect_symbol(source, sync, build_dir):
        """Redirect a symbol at link time to point at somewhere else

        Positional arguments:
        source -- the symbol doing the pointing
        sync -- the symbol being pointed to
        build_dir -- the directory to put "response files" if needed by the toolchain

        Side Effects:
        Possibly create a file in the build directory

        Return:
        The linker flag to redirect the symbol, as a string
        """
        raise NotImplemented

    # Return the list of macros geenrated by the build system
    def get_config_macros(self):
        return self.config.config_to_macros(self.config_data) if self.config_data else []

    @abstractmethod
    def version_check(self):
        """Check the version of a compiler being used and raise a
        NotSupportedException when it's incorrect.
        """
        raise NotImplemented

    @property
    def report(self):
        to_ret = {}
        to_ret['c_compiler'] = {'flags': copy(self.flags['c']),
                                'symbols': self.get_symbols()}
        to_ret['cxx_compiler'] = {'flags': copy(self.flags['cxx']),
                                  'symbols': self.get_symbols()}
        to_ret['assembler'] = {'flags': copy(self.flags['asm']),
                               'symbols': self.get_symbols(True)}
        to_ret['linker'] = {'flags': copy(self.flags['ld'])}
        to_ret.update(self.config.report)
        return to_ret

from tools.settings import ARM_PATH, ARMC6_PATH, GCC_ARM_PATH, IAR_PATH

TOOLCHAIN_PATHS = {
    'ARM': ARM_PATH,
    'uARM': ARM_PATH,
    'ARMC6': ARMC6_PATH,
    'GCC_ARM': GCC_ARM_PATH,
    'IAR': IAR_PATH
}

from tools.toolchains.arm import ARM_STD, ARM_MICRO, ARMC6
from tools.toolchains.gcc import GCC_ARM
from tools.toolchains.iar import IAR

TOOLCHAIN_CLASSES = {
    u'ARM': ARM_STD,
    u'uARM': ARM_MICRO,
    u'ARMC6': ARMC6,
    u'GCC_ARM': GCC_ARM,
    u'IAR': IAR
}

TOOLCHAINS = set(TOOLCHAIN_CLASSES.keys())
