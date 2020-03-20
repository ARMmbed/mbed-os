"""
mbed SDK
SPDX-License-Identifier: Apache-2.0
Copyright (c) 2011-2013 ARM Limited
SPDX-License-Identifier: Apache-2.0

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
import json
from os import stat, getcwd, getenv, rename, remove
from copy import copy
from time import time, sleep
from shutil import copyfile
from os.path import join, splitext, exists, relpath, dirname, split, abspath
from inspect import getmro
from copy import deepcopy
from collections import namedtuple
from abc import ABCMeta, abstractmethod
from distutils.spawn import find_executable
from multiprocessing import Pool, cpu_count
from hashlib import md5

from ..utils import (
    run_cmd,
    mkdir,
    ToolException,
    NotSupportedException,
    split_path,
    compile_worker,
    generate_update_filename,
)
from ..settings import MBED_ORG_USER, PRINT_COMPILER_OUTPUT_AS_LINK
from ..notifier.term import TerminalNotifier
from ..resources import FileType
from ..memap import MemapParser
from ..config import (ConfigException, RAM_ALL_MEMORIES, ROM_ALL_MEMORIES)
from ..regions import (UPDATE_WHITELIST, merge_region_list)
from ..settings import COMPARE_FIXED
from ..settings import ARM_PATH, ARMC6_PATH, GCC_ARM_PATH, IAR_PATH
from future.utils import with_metaclass


TOOLCHAIN_PATHS = {
    'ARM': ARM_PATH,
    'uARM': ARM_PATH,
    'ARMC6': ARMC6_PATH,
    'GCC_ARM': GCC_ARM_PATH,
    'IAR': IAR_PATH
}


# Minimum job count in order to turn on parallel builds
CPU_COUNT_MIN = 1
# Number of jobs to start for each CPU
CPU_COEF = 1

CORTEX_SYMBOLS = {
    "Cortex-M0":       ["__CORTEX_M0", "ARM_MATH_CM0", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M0+":      ["__CORTEX_M0PLUS", "ARM_MATH_CM0PLUS", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M1":       ["__CORTEX_M3", "ARM_MATH_CM1", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M3":       ["__CORTEX_M3", "ARM_MATH_CM3", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M4":       ["__CORTEX_M4", "ARM_MATH_CM4", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M4F":      ["__CORTEX_M4", "ARM_MATH_CM4", "__FPU_PRESENT=1",
                        "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M7":       ["__CORTEX_M7", "ARM_MATH_CM7", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M7F":      ["__CORTEX_M7", "ARM_MATH_CM7", "__FPU_PRESENT=1",
                        "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M7FD":     ["__CORTEX_M7", "ARM_MATH_CM7", "__FPU_PRESENT=1",
                        "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
    "Cortex-A9":       ["__CORTEX_A9", "ARM_MATH_CA9", "__FPU_PRESENT",
                        "__CMSIS_RTOS", "__EVAL", "__MBED_CMSIS_RTOS_CA9"],
    "Cortex-M23-NS":   ["__CORTEX_M23", "ARM_MATH_ARMV8MBL", "DOMAIN_NS=1",
                        "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M23":      ["__CORTEX_M23", "ARM_MATH_ARMV8MBL", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M33-NS":   ["__CORTEX_M33", "ARM_MATH_ARMV8MML", "DOMAIN_NS=1",
                        "__CMSIS_RTOS", "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M33":      ["__CORTEX_M33", "ARM_MATH_ARMV8MML", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M33F-NS":  ["__CORTEX_M33", "ARM_MATH_ARMV8MML", "DOMAIN_NS=1",
                        "__FPU_PRESENT=1U", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M33F":     ["__CORTEX_M33", "ARM_MATH_ARMV8MML",
                        "__FPU_PRESENT=1U", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM"],
    "Cortex-M33FE-NS": ["__CORTEX_M33", "ARM_MATH_ARMV8MML", "DOMAIN_NS=1",
                        "__FPU_PRESENT=1U", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM", "__DSP_PRESENT=1U"],
    "Cortex-M33FE":    ["__CORTEX_M33", "ARM_MATH_ARMV8MML",
                        "__FPU_PRESENT=1U", "__CMSIS_RTOS",
                        "__MBED_CMSIS_RTOS_CM", "__DSP_PRESENT=1U"],
}

UNSUPPORTED_C_LIB_EXCEPTION_STRING = "{} C library option not supported for this target."

class mbedToolchain(with_metaclass(ABCMeta, object)):
    OFFICIALLY_SUPPORTED = False

    # Verbose logging
    VERBOSE = True

    # Compile C files as CPP
    COMPILE_C_AS_CPP = False

    # Response files for compiling, includes, linking and archiving.
    # Response files are files that contain arguments that would
    # normally be passed on the command line.
    RESPONSE_FILES = True

    MBED_CONFIG_FILE_NAME = "mbed_config.h"

    PROFILE_FILE_NAME = ".profile"

    profile_template = {'common': [], 'c': [], 'cxx': [], 'asm': [], 'ld': []}

    def __init__(self, target, notify=None, macros=None, build_profile=None,
                 build_dir=None, coverage_patterns=None):
        self.target = target
        self.name = self.__class__.__name__


        # Toolchain flags
        self.flags = deepcopy(build_profile or self.profile_template)

        # System libraries provided by the toolchain
        self.sys_libs = []

        # User-defined macros
        self.macros = macros or []

        # Macros generated from toolchain and target rules/features
        self.asm_symbols = None
        self.cxx_symbols = None

        # Labels generated from toolchain and target rules/features (used for
        # selective build)
        self.labels = None

        # This will hold the initialized config object
        self.config = None

        self.config_data = None

        self.config_file = None

        # Call guard for "get_config_data" (see the comments of
        # get_config_data for details)
        self.config_processed = False

        # Non-incremental compile
        self.build_all = False

        # Build output dir
        if PRINT_COMPILER_OUTPUT_AS_LINK:
            self.build_dir = abspath(build_dir)
        else:
            self.build_dir = build_dir
        self.timestamp = getenv("MBED_BUILD_TIMESTAMP", time())

        # Number of concurrent build jobs. 0 means host system cores
        self.jobs = 0

        if notify:
            self.notify = notify
        else:
            self.notify = TerminalNotifier()

        # Stats cache is used to reduce the amount of IO requests to stat
        # header files during dependency change. See need_update()
        self.stat_cache = {}

        # Used by the mbed Online Build System to build in chrooted environment
        self.CHROOT = None

        self.coverage_supported = False
        self.coverage_patterns = coverage_patterns

        # post-init hook used by the online compiler TODO: remove this.
        self.init()

    # TODO: This should not be needed, so remove it
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
                if self.target.core in CORTEX_SYMBOLS:
                    self.asm_symbols.extend(CORTEX_SYMBOLS[self.target.core])

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
                self.cxx_symbols.extend(
                    "TOOLCHAIN_%s" % t for t in labels['TOOLCHAIN']
                )

                # Cortex CPU symbols
                if self.target.core in CORTEX_SYMBOLS:
                    self.cxx_symbols.extend(CORTEX_SYMBOLS[self.target.core])

                # Symbols defined by the on-line build.system
                self.cxx_symbols.extend(
                    ['MBED_BUILD_TIMESTAMP=%s' % self.timestamp,
                     'TARGET_LIKE_MBED', '__MBED__=1']
                )
                if MBED_ORG_USER:
                    self.cxx_symbols.append('MBED_USERNAME=' + MBED_ORG_USER)

                # Add target's name
                self.cxx_symbols += ["TARGET_NAME=" + self.target.name]
                # Add target's symbols
                self.cxx_symbols += self.target.macros
                # Add target's hardware
                self.cxx_symbols += [
                    "DEVICE_" + data + "=1" for data in self.target.device_has
                ]
                # Add target's features
                self.cxx_symbols += [
                    "FEATURE_" + data + "=1" for data in self.target.features
                ]
                # Add target's components
                self.cxx_symbols += [
                    "COMPONENT_" + data + "=1"
                    for data in self.target.components
                ]

                # Add extra symbols passed via 'macros' parameter
                self.cxx_symbols += self.macros

                # Form factor variables
                if hasattr(self.target, 'supported_form_factors'):
                    self.cxx_symbols.extend(
                        ["TARGET_FF_%s" % t for t in
                         self.target.supported_form_factors]
                    )

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
                'COMPONENT': self.target.components,
                'TOOLCHAIN': toolchain_labels
            }

            # This is a policy decision and it should /really/ be in the config
            # system ATM it's here for backward compatibility
            if ((("-g" in self.flags['common'] or
                  "-g3" in self.flags['common']) and
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
            if not d or not exists(d):
                return True

            if d not in self.stat_cache:
                self.stat_cache[d] = stat(d).st_mtime

            if self.stat_cache[d] >= target_mod_time:
                return True

        return False

    def copy_files(self, files_paths, trg_path, resources=None):
        # Handle a single file
        if not isinstance(files_paths, list):
            files_paths = [files_paths]

        for dest, source in files_paths:
            target = join(trg_path, dest)
            if (target != source) and (self.need_update(target, [source])):
                self.progress("copy", dest)
                mkdir(dirname(target))
                copyfile(source, target)

    # TODO: the online build system places the target between
    # the file name and file extension. Supporting multiple co-resident
    # builds within a single directory would make this online override
    # obsolite.
    # THIS METHOD IS BEING OVERRIDDEN BY THE MBED ONLINE BUILD SYSTEM
    # ANY CHANGE OF PARAMETERS OR RETURN VALUES WILL BREAK COMPATIBILITY
    def relative_object_path(self, build_path, file_ref):
        source_dir, name, _ = split_path(file_ref.name)

        obj_dir = relpath(join(build_path, source_dir))
        if obj_dir is not self.prev_dir:
            self.prev_dir = obj_dir
            mkdir(obj_dir)
        return join(obj_dir, name + '.o')

    def make_option_file(self, options, naming=".options_{}.txt"):
        """ Generate a via file for a pile of defines
        ARM, GCC, IAR cross compatible
        """
        to_write = " ".join(options).encode('utf-8')
        new_md5 = md5(to_write).hexdigest()
        via_file = join(self.build_dir, naming.format(new_md5))
        try:
            with open(via_file, "r") as fd:
                old_md5 = md5(fd.read().encode('utf-8')).hexdigest()
        except IOError:
            old_md5 = None
        if old_md5 != new_md5:
            with open(via_file, "wb") as fd:
                fd.write(to_write)
        return via_file

    def get_inc_file(self, includes):
        """Generate a via file for all includes.
        ARM, GCC, IAR cross compatible
        """
        cmd_list = (
            "\"-I{}\"".format(c.replace("\\", "/")) for c in includes if c
        )
        if self.CHROOT:
            cmd_list = (c.replace(self.CHROOT, '') for c in cmd_list)
        return self.make_option_file(list(cmd_list), naming=".includes_{}.txt")

    def get_link_file(self, cmd):
        """Generate a via file for all objects when linking.
        ARM, GCC, IAR cross compatible
        """
        cmd_list = (c.replace("\\", "/") for c in cmd if c)
        if self.CHROOT:
            cmd_list = (c.replace(self.CHROOT, '') for c in cmd_list)
        return self.make_option_file(
            list(cmd_list), naming=".link_options.txt"
        )

    def get_arch_file(self, objects):
        """ Generate a via file for all objects when archiving.
        ARM, GCC, IAR cross compatible
        """
        cmd_list = (c.replace("\\", "/") for c in objects if c)
        return self.make_option_file(list(cmd_list), ".archive_files.txt")

    def compile_legacy_sources(
        self, resources, inc_dirs=None, exclude_dirs=None
    ):
        """Compile source files with option to exclude some directories.

        This method only exists to not break API compatibility and provide a
        way to exclude directories for Mbed OS 2 builds.
        """
        return self._compile_sources(
            resources, inc_dirs=inc_dirs, exclude_dirs=exclude_dirs
        )

    # THIS METHOD IS BEING CALLED BY THE MBED ONLINE BUILD SYSTEM
    # ANY CHANGE OF PARAMETERS OR RETURN VALUES WILL BREAK COMPATIBILITY
    def compile_sources(self, resources, inc_dirs=None):
        """Compile source files."""
        return self._compile_sources(resources, inc_dirs=inc_dirs)

    @staticmethod
    def _exclude_files_from_build(files_to_compile, exclude_dirs):
        """Remove files from dirs to be excluded for the build."""
        return [
            file_to_compile
            for file_to_compile in files_to_compile
            if all(
                exclude_dir not in file_to_compile.path
                for exclude_dir in exclude_dirs
            )
        ]

    def _compile_sources(self, resources, inc_dirs=None, exclude_dirs=None):
        # Web IDE progress bar for project build
        files_to_compile = (
            resources.get_file_refs(FileType.ASM_SRC) +
            resources.get_file_refs(FileType.C_SRC) +
            resources.get_file_refs(FileType.CPP_SRC)
        )
        if exclude_dirs:
            compilation_queue = self._exclude_files_from_build(files_to_compile, exclude_dirs)
        else:
            compilation_queue = files_to_compile

        self.to_be_compiled = len(compilation_queue)
        self.compiled = 0

        self.notify.cc_verbose("Macros: " + ' '.join([
            '-D%s' % s for s in self.get_symbols()
        ]))

        inc_paths = resources.get_file_paths(FileType.INC_DIR)
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

        # Generate configuration header and update self.build_all
        self.get_config_header()
        self.dump_build_profile()

        # Sort compile queue for consistency
        compilation_queue.sort()
        for source in compilation_queue:
            object = self.relative_object_path(self.build_dir, source)

            # Queue mode (multiprocessing)
            commands = self.compile_command(source.path, object, inc_paths)
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
            self.progress("compile", item['source'].name, build_update=True)
            for res in result['results']:
                self.notify.cc_verbose(
                    "Compile: %s" % ' '.join(res['command']),
                    result['source'].name
                )
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
                        self.progress(
                            "compile",
                            result['source'].name,
                            build_update=True
                        )
                        for res in result['results']:
                            self.notify.cc_verbose(
                                "Compile: %s" % ' '.join(res['command']),
                                result['source'].name
                            )
                            self.compile_output([
                                res['code'],
                                res['output'],
                                res['command']
                            ])
                        objects.append(result['object'])
                    except ToolException as err:
                        # Stop the worker processes immediately without completing outstanding work
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

        source = abspath(source) if PRINT_COMPILER_OUTPUT_AS_LINK else source

        if ext == '.c' or ext == '.cpp' or ext == '.cc':
            base, _ = splitext(object)
            dep_path = base + '.d'
            try:
                if exists(dep_path):
                    deps = self.parse_dependencies(dep_path)
                else:
                    deps = []
            except (IOError, IndexError):
                deps = []
            config_file = ([self.config.app_config_location]
                           if self.config.app_config_location else [])
            deps.extend(config_file)
            if ext != '.c' or self.COMPILE_C_AS_CPP:
                deps.append(join(
                    self.build_dir, self.PROFILE_FILE_NAME + "-cxx"
                ))
            else:
                deps.append(join(
                    self.build_dir, self.PROFILE_FILE_NAME + "-c"
                ))
            if len(deps) == 0 or self.need_update(object, deps):
                if ext != '.c' or self.COMPILE_C_AS_CPP:
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
        dep_path -- the path to a file generated by a previous run of the
                    compiler

        Return value:
        A list of all source files that the dependency file indicated were
        dependencies

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
        call self.cc_info or self.notify with a description of the event
        generated by the compiler
        """
        raise NotImplemented

    def compile_output(self, output=[]):
        rc = output[0]
        stderr = output[1]

        # Parse output for Warnings and Errors
        self.parse_output(stderr)
        self.notify.debug("Return: %s" % rc)
        for error_line in stderr.splitlines():
            self.notify.debug("Output: %s" % error_line)

        # Check return code
        if rc != 0:
            if self.is_not_supported_error(stderr):
                raise NotSupportedException(stderr)
            else:
                raise ToolException(stderr)

    def build_library(self, objects, dir, name):
        needed_update = False
        lib = self.STD_LIB_NAME % name
        fout = join(dir, lib)
        if self.need_update(fout, objects):
            self.notify.info("Library: %s" % lib)
            self.archive(objects, fout)
            needed_update = True

        return needed_update

    def _do_region_merge(self, name, binary, ext):
        region_list = list(self.config.regions)
        region_list = [r._replace(filename=binary) if r.active else r
                       for r in region_list]
        res = "{}.{}".format(join(self.build_dir, name), ext)
        merge_region_list(
            region_list, res, self.notify,
            restrict_size=self.config.target.restrict_size
        )
        update_regions = [
            r for r in region_list if r.name in UPDATE_WHITELIST
        ]
        if update_regions:
            update_res = join(
                self.build_dir,
                generate_update_filename(name, self.target)
            )
            merge_region_list(
                update_regions,
                update_res,
                self.notify,
                restrict_size=self.config.target.restrict_size
            )
            return res, update_res
        else:
            return res, None

    def link_program(self, r, tmp_path, name):
        ext = getattr(self.target, "OUTPUT_EXT", "bin")

        if hasattr(self.target, 'OUTPUT_NAMING'):
            self.notify.var("binary_naming", self.target.OUTPUT_NAMING)
            if self.target.OUTPUT_NAMING == "8.3":
                name = name[0:8]
                ext = ext[0:3]

        # Create destination directory
        head, tail = split(name)
        new_path = join(tmp_path, head)
        mkdir(new_path)

        # The output file names are derived from the project name, but this can have spaces in it which
        # messes-up later processing. Replace any spaces in the derived names with '_'
        tail = tail.replace(" ", "_")

        # Absolute path of the final linked file
        if self.config.has_regions:
            elf = join(new_path, tail + '_application.elf')
            mapfile = join(new_path, tail + '_application.map')
        else:
            elf = join(new_path, tail + '.elf')
            mapfile = join(new_path, tail + '.map')

        objects = sorted(set(r.get_file_paths(FileType.OBJECT)))
        config_file = ([self.config.app_config_location]
                       if self.config.app_config_location else [])
        try:
            linker_script = [
                path for _, path in r.get_file_refs(FileType.LD_SCRIPT)
                if path.endswith(self.LINKER_EXT)
            ][-1]
        except IndexError:
            raise NotSupportedException("No linker script found")
        lib_dirs = r.get_file_paths(FileType.LIB_DIR)
        libraries = [l for l in r.get_file_paths(FileType.LIB)
                     if l.endswith(self.LIBRARY_EXT)]
        hex_files = r.get_file_paths(FileType.HEX)
        dependencies = (
            objects + libraries + [linker_script] + config_file + hex_files
        )
        dependencies.append(join(
            self.build_dir,
            self.PROFILE_FILE_NAME + "-ld"
        ))
        if self.need_update(elf, dependencies):
            if not COMPARE_FIXED and exists(mapfile):
                old_mapfile = "%s.old" % mapfile
                if exists(old_mapfile):
                    remove(old_mapfile)
                rename(mapfile, old_mapfile)
            self.progress("link", tail)
            self.link(elf, objects, libraries, lib_dirs, linker_script)

        if self.config.has_regions:
            filename = "{}_application.{}".format(tail, ext)
        else:
            filename = "{}.{}".format(tail, ext)
        full_path = join(new_path, filename)
        if ext != 'elf':
            if full_path and self.need_update(full_path, [elf]):
                self.progress("elf2bin", tail)
                self.binary(r, elf, full_path)
            if self.config.has_regions:
                full_path, updatable = self._do_region_merge(
                    tail, full_path, ext
                )
            else:
                updatable = None
        else:
            updatable = None

        # compile/assemble/link/binary hooks
        post_build_hook = self.target.get_post_build_hook(
            self._get_toolchain_labels()
        )
        if post_build_hook:
            self.progress("post-build", tail)
            post_build_hook(self, r, elf, full_path)
        # Initialize memap and process map file. This doesn't generate output.
        self.mem_stats(mapfile)

        self.notify.var("compile_succeded", True)
        self.notify.var("binary", full_path)

        return full_path, updatable

    # THIS METHOD IS BEING OVERRIDDEN BY THE MBED ONLINE BUILD SYSTEM
    # ANY CHANGE OF PARAMETERS OR RETURN VALUES WILL BREAK COMPATIBILITY
    def default_cmd(self, command):
        stdout, stderr, rc = run_cmd(
            command, work_dir=getcwd(), chroot=self.CHROOT
        )
        self.notify.debug("Return: %s" % rc)

        self.parse_output(stderr)
        for output_line in stdout.splitlines():
            self.notify.debug("Output: %s" % output_line)
        for error_line in stderr.splitlines():
            self.notify.debug("Errors: %s" % error_line)

        if rc != 0:
            for line in stderr.splitlines():
                self.notify.tool_error(line)
            raise ToolException(stderr)

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
            self.notify.info(
                "Unknown toolchain for memory statistics %s" % toolchain
            )
            return None

        # Store the memap instance for later use
        self.memap_instance = memap

        # Note: memory statistics are not returned.
        # Need call to generate_output later (depends on depth & output format)

        return None

    def _add_defines_from_region(
            self,
            region,
            linker_define=False,
            suffixes=['_ADDR', '_SIZE']
    ):
        for define in [(region.name.upper() + suffixes[0], region.start),
                       (region.name.upper() + suffixes[1], region.size)]:
            define_string = "-D%s=0x%x" % define
            self.cc.append(define_string)
            self.cppc.append(define_string)
            self.flags["common"].append(define_string)
            if linker_define:
                ld_string = ("%s" % define[0], "0x%x" % define[1])
                ld_string = self.make_ld_define(*ld_string)
                self.ld.append(ld_string)
                self.flags["ld"].append(ld_string)

    def _add_all_regions(self, region_list, active_region_name):
        for region in region_list:
            self._add_defines_from_region(region)
            if region.active and active_region_name:
                for define in [
                        ("%s_START" % active_region_name,
                         "0x%x" % region.start),
                        ("%s_SIZE" % active_region_name, "0x%x" % region.size)
                ]:
                    define_string = self.make_ld_define(*define)
                    self.ld.append(define_string)
                    self.flags["ld"].append(define_string)
            self.notify.info("  Region %s: size 0x%x, offset 0x%x"
                             % (region.name, region.size, region.start))

    def add_regions(self):
        """Add regions to the build profile, if there are any.
        """
        if self.config.has_regions:
            try:
                regions = list(self.config.regions)
                regions.sort(key=lambda x: x.start)
                self.notify.info("Using ROM region%s %s in this build." % (
                    "s" if len(regions) > 1 else "",
                    ", ".join(r.name for r in regions)
                ))
                self._add_all_regions(regions, "MBED_APP")
            except ConfigException as error:
                self.notify.info("Configuration error: %s" % str(error))

        if self.config.has_ram_regions:
            try:
                regions = list(self.config.ram_regions)
                self.notify.info("Using RAM region%s %s in this build." % (
                    "s" if len(regions) > 1 else "",
                    ", ".join(r.name for r in regions)
                ))
                self._add_all_regions(regions, None)
            except ConfigException as error:
                self.notify.info("Configuration error: %s" % str(error))

        Region = namedtuple("Region", "name start size")

        if not getattr(self.target, "static_memory_defines", False):
            self.notify.info("Configuration error: 'static_memory_defines' is not defined.")
            return

        try:
            # Add all available ROM regions to build profile
            rom_available_regions = self.config.get_all_active_memories(
                ROM_ALL_MEMORIES
            )
            for key, value in rom_available_regions.items():
                rom_start, rom_size = value
                self._add_defines_from_region(
                    Region("MBED_" + key, rom_start, rom_size),
                    True,
                    suffixes=["_START", "_SIZE"]
                )
        except ConfigException as error:
            self.notify.info("Configuration error: %s" % str(error))

        try:
            # Add all available RAM regions to build profile
            ram_available_regions = self.config.get_all_active_memories(
                RAM_ALL_MEMORIES
            )
            for key, value in ram_available_regions.items():
                ram_start, ram_size = value
                self._add_defines_from_region(
                    Region("MBED_" + key, ram_start, ram_size),
                    True,
                    suffixes=["_START", "_SIZE"]
                )
        except ConfigException as error:
            self.notify.info("Configuration error: %s" % str(error))

    STACK_PARAM = "target.boot-stack-size"
    TFM_LVL_PARAM = "tfm.level"
    XIP_ENABLE_PARAM = "target.xip-enable"

    def add_linker_defines(self):
        params, _ = self.config_data

        if self.STACK_PARAM in params:
            define_string = self.make_ld_define(
                "MBED_BOOT_STACK_SIZE",
                int(params[self.STACK_PARAM].value, 0)
            )
            self.ld.append(define_string)
            self.flags["ld"].append(define_string)

        # Pass TFM_LVL to linker files, so single linker file can support different TFM security levels.
        if self.TFM_LVL_PARAM in params:
            define_string = self.make_ld_define(
                "TFM_LVL",
                params[self.TFM_LVL_PARAM].value
            )
            self.ld.append(define_string)
            self.flags["ld"].append(define_string)

        if self.XIP_ENABLE_PARAM in params:
            define_string = self.make_ld_define(
                "XIP_ENABLE",
                params[self.XIP_ENABLE_PARAM].value
            )
            self.ld.append(define_string)
            self.flags["ld"].append(define_string)

        if self.target.is_PSA_secure_target:
            for flag, param in [
                ("MBED_PUBLIC_RAM_START", "target.public-ram-start"),
                ("MBED_PUBLIC_RAM_SIZE", "target.public-ram-size")
            ]:
                define_string = self.make_ld_define(flag, params[param].value)
                self.ld.append(define_string)
                self.flags["ld"].append(define_string)

        if hasattr(self.target, 'post_binary_hook'):
            if self.target.post_binary_hook is None:
                define_string = self.make_ld_define(
                    "DISABLE_POST_BINARY_HOOK", 1)
                self.ld.append(define_string)
                self.flags["ld"].append(define_string)

    # Set the configuration data
    def set_config_data(self, config_data):
        self.config_data = config_data
        # new configuration data can change labels, so clear the cache
        self.labels = None
        # pass info about softdevice presence to linker (see NRF52)
        if "SOFTDEVICE_PRESENT" in config_data[1]:
            define_string = self.make_ld_define(
                "SOFTDEVICE_PRESENT",
                config_data[1]["SOFTDEVICE_PRESENT"].macro_value
            )
            self.ld.append(define_string)
            self.flags["ld"].append(define_string)
        self.add_regions()
        self.add_linker_defines()

    def get_config_header(self):
        """ Creates the configuration header as needed.
        The config file is located in the build directory

        - if there is no configuration data, "mbed_config.h" will not exists.
        - if there is configuration data and "mbed_config.h" does not exist,
          it is created.
        - if there is configuration data that is the same as the previous
          configuration data, "mbed_config.h" is left untouched.
        - if there is new configuration data, "mbed_config.h" is overriden.
        The function needs to be called exactly once for the lifetime of this
        toolchain instance.
        The "config_processed" variable (below) ensures this behaviour.
        The function returns the location of the configuration file, or None
        when there is no configuration data and file available.
        """
        if self.config_processed:
            return self.config_file
        self.config_file = join(self.build_dir, self.MBED_CONFIG_FILE_NAME)

        if exists(self.config_file):
            with open(self.config_file, "r") as f:
                prev_data = f.read()
        else:
            prev_data = None
        if self.config_data:
            crt_data = self.config.config_to_header(self.config_data)
        else:
            crt_data = None

        changed = False
        if prev_data is not None:
            if crt_data is None:
                remove(self.config_file)
                self.config_file = None
                changed = True
            elif crt_data != prev_data:
                with open(self.config_file, "w") as f:
                    f.write(crt_data)
                changed = True
        else:
            if crt_data is not None:
                with open(self.config_file, "w") as f:
                    f.write(crt_data)
                changed = True
            else:
                self.config_file = None
        self.build_all = changed
        self.config_processed = True
        return self.config_file

    def dump_build_profile(self):
        """Dump the current build profile and macros into the `.profile` file
        in the build directory"""
        for key in ["cxx", "c", "asm", "ld"]:
            to_dump = {
                "flags": sorted(self.flags[key]),
                "macros": sorted(self.macros),
                "symbols": sorted(self.get_symbols(for_asm=(key == "asm"))),
            }
            if key in ["cxx", "c"]:
                to_dump["symbols"].remove(
                    'MBED_BUILD_TIMESTAMP=%s' % self.timestamp
                )
                to_dump["flags"].extend(sorted(self.flags['common']))
            where = join(self.build_dir, self.PROFILE_FILE_NAME + "-" + key)
            self._overwrite_when_not_equal(where, json.dumps(
                to_dump, sort_keys=True, indent=4))

    def check_and_add_minimal_printf(self, target):
        """Add toolchain flag if minimal-printf is selected."""
        if (
            getattr(target, "printf_lib", "std") == "minimal-printf"
            and "-DMBED_MINIMAL_PRINTF" not in self.flags["common"]
        ):
            self.flags["common"].append("-DMBED_MINIMAL_PRINTF")

    def check_c_lib_supported(self, target, toolchain):
        """
        Check and raise an exception if the requested C library is not supported,

        target.c_lib is modified to have the lowercased string of its original string.
        This is done to be case insensitive when validating.
        """
        if  hasattr(target, "default_lib"):
            raise NotSupportedException(
                   "target.default_lib is no longer supported, please use target.c_lib for C library selection."
                )
        if  hasattr(target, "c_lib"):
            target.c_lib = target.c_lib.lower()
            if (
                hasattr(target, "supported_c_libs") == False
                or toolchain not in target.supported_c_libs
                or target.c_lib not in target.supported_c_libs[toolchain]
            ):
                raise NotSupportedException(
                   UNSUPPORTED_C_LIB_EXCEPTION_STRING.format(target.c_lib)
                )

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
        if (not TOOLCHAIN_PATHS[tool_key] or
                not exists(TOOLCHAIN_PATHS[tool_key])):
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
            subdir = join(TOOLCHAIN_PATHS[tool_key], executable_name)
        # User could have specified a path that exists but does not contain exe
        return exists(subdir) or exists(subdir + '.exe')

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
        config_header -- The configuration header that will be included within
                         all source files

        Return value:
        A list of the command line arguments that will force the inclusion the
        specified header

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
        for_asm -- generate the assembler options instead of the compiler
                   options

        Return value:
        A list of the command line arguments that will force the inclusion the
        specified header

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
        """
        raise NotImplemented

    @abstractmethod
    def compile_c(self, source, object, includes):
        """Generate the command line that compiles a C source file.

        Positional arguments:
        source -- the C source file to compile
        object -- the destination object file
        includes -- a list of all the directories where header files may be
                    found

        Return value:
        The complete command line, as a list, that would invoke the C compiler
        on the source file, include all the include paths, and generate the
        specified object file.

        Side effects:
        None
        """
        raise NotImplemented

    @abstractmethod
    def compile_cpp(self, source, object, includes):
        """Generate the command line that compiles a C++ source file.

        Positional arguments:
        source -- the C++ source file to compile
        object -- the destination object file
        includes -- a list of all the directories where header files may be
                    found

        Return value:
        The complete command line, as a list, that would invoke the C++
        compiler on the source file, include all the include paths, and
        generate the specified object file.

        Side effects:
        None
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
        build_dir -- the directory to put "response files" if needed by the
                     toolchain

        Side Effects:
        Possibly create a file in the build directory

        Return:
        The linker flag to redirect the symbol, as a string
        """
        raise NotImplemented

    def get_config_macros(self):
        """ Return the list of macros generated by the build system """
        if self.config_data:
            return self.config.config_to_macros(self.config_data)
        else:
            return []

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

def should_replace_small_c_lib(target, toolchain):
    """
    Check if the small C lib should be replaced with the standard C lib.
    Return True if the replacement occurs otherwise return False.
    """
    return (
        not is_library_supported("small", target, toolchain)
        and is_library_supported("std", target, toolchain)
        and target.c_lib == "small"
    )


def is_library_supported(lib_type, target, toolchain):
    """
    Check if a library type is supported by a toolchain for a given target.

    Return True if the library type is supported, False if not supported or
    the target does not have an supported_c_libs attribute.
    """
    return (
        hasattr(target, "supported_c_libs")
        and toolchain.lower() in target.supported_c_libs
        and lib_type in target.supported_c_libs[toolchain.lower()]
    )
