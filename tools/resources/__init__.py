# mbed SDK
# Copyright (c) 2011-2013 ARM Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
# The scanning rules and Resources object.

A project in Mbed OS contains metadata in the file system as directory names.
These directory names adhere to a set of rules referred to as scanning rules.
The following are the English version of the scanning rules:

Directory names starting with "TEST_", "TARGET_", "TOOLCHAIN_" and "FEATURE_"
are excluded from a build unless one of the following is true:
 * The suffix after "TARGET_" is a target label (see target.labels).
 * The suffix after "TOOLCHAIN_" is a toolchain label, defined by the
   inheritance hierarchy of the toolchain class.
 * The suffix after "FEATURE_" is a member of `target.features`.


"""

from __future__ import print_function, division, absolute_import

import fnmatch
import re
from copy import copy
from itertools import chain
from os import walk
from os.path import (join, splitext, dirname, relpath, basename, split, normcase,
                     abspath, exists)

from ..toolchains import TOOLCHAINS

# Support legacy build conventions: the original mbed build system did not have
# standard labels for the "TARGET_" and "TOOLCHAIN_" specific directories, but
# had the knowledge of a list of these directories to be ignored.
LEGACY_IGNORE_DIRS = set([
    # Legacy Targets
    'LPC11U24',
    'LPC1768',
    'LPC2368',
    'LPC4088',
    'LPC812',
    'KL25Z',

    # Legacy Toolchains
    'ARM',
    'uARM',
    'IAR',
    'GCC_ARM',
    'GCC_CS',
    'GCC_CR',
    'GCC_CW',
    'GCC_CW_EWL',
    'GCC_CW_NEWLIB',
    'ARMC6',

    # Tests, here for simplicity
    'TESTS',
])
LEGACY_TOOLCHAIN_NAMES = {
    'ARM_STD':'ARM',
    'ARM_MICRO': 'uARM',
    'GCC_ARM': 'GCC_ARM',
    'GCC_CR': 'GCC_CR',
    'IAR': 'IAR',
    'ARMC6': 'ARMC6',
}


class Resources(object):
    def __init__(self, notify, base_path=None, collect_ignores=False):
        self.notify = notify
        self.base_path = base_path
        self.collect_ignores = collect_ignores
        self._label_paths = []

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

        self.ignored_dirs = []

        self.labels = {
            "TARGET": [],
            "TOOLCHAIN": [],
            "FEATURE": []
        }

        # Pre-mbed 2.0 ignore dirs
        self.legacy_ignore_dirs = (LEGACY_IGNORE_DIRS | TOOLCHAINS)

        # Ignore patterns from .mbedignore files
        self.ignore_patterns = []
        self._ignore_regex = re.compile("$^")

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
        for f,p in resources.file_basepath.items():
            self.file_basepath[f] = p

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

        self.ignored_dirs += resources.ignored_dirs
        self._label_paths += resources._label_paths

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
                toolchain.tool_error(
                    "Object file %s.o is not unique! It could be made from: %s"\
                    % (objname, " ".join(filenames)))
        for headername, locations in dupe_headers.items():
            if len(locations) > 1:
                count+=1
                toolchain.tool_error(
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

        if self.linker_script is not None:
            self.linker_script = rel_path(self.linker_script, base, dot)

    def win_to_unix(self):
        for field in ['inc_dirs', 'headers', 's_sources', 'c_sources',
                      'cpp_sources', 'lib_dirs', 'objects', 'libraries',
                      'lib_builds', 'lib_refs', 'repo_dirs', 'repo_files',
                      'hex_files', 'bin_files', 'json_files']:
            v = [f.replace('\\', '/') for f in getattr(self, field)]
            setattr(self, field, v)

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
            ):
            if resources:
                s.append('%s:\n  ' % label + '\n  '.join(resources))

        if self.linker_script:
            s.append('Linker Script: ' + self.linker_script)

        return '\n'.join(s)


    def _add_labels(self, prefix, labels):
        self.labels.setdefault(prefix, [])
        self.labels[prefix].extend(labels)
        prefixed_labels = set("%s_%s" % (prefix, label) for label in labels)
        for path, base_path in self._label_paths:
            if basename(path) in prefixed_labels:
                self.add_directory(path, base_path)
        self._label_paths = [(p, b) for p, b in self._label_paths
                             if basename(p) not in prefixed_labels]

    def add_target_labels(self, target):
        self._add_labels("TARGET", target.labels)

    def add_features(self, features):
        self._add_labels("FEATURE", features)

    def add_toolchain_labels(self, toolchain):
        for prefix, value in toolchain.get_labels().items():
            self._add_labels(prefix, value)
        self.legacy_ignore_dirs -= set(
            [toolchain.target.name, LEGACY_TOOLCHAIN_NAMES[toolchain.name]])

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

    def _not_current_label(self, dirname, label_type):
        return (dirname.startswith(label_type + "_") and
                dirname[len(label_type) + 1:] not in self.labels[label_type])

    def add_directory(self, path, base_path=None, exclude_paths=None):
        """ Scan a directory and include its resources in this resources obejct

        Positional arguments:
        path - the path to search for resources

        Keyword arguments
        base_path - If this is part of an incremental scan, include the origin
                    directory root of the scan here
        exclude_paths - A list of paths that are to be excluded from a build
        """
        self.notify.progress("scan", abspath(path))

        if base_path is None:
            base_path = path
        if self.collect_ignores and path in self.ignored_dirs:
            self.ignored_dirs.remove(path)
        if exclude_paths:
            self.add_ignore_patterns(
                path, base_path, [join(e, "*") for e in exclude_paths])

        for root, dirs, files in walk(path, followlinks=True):
            # Check if folder contains .mbedignore
            if ".mbedignore" in files:
                with open (join(root,".mbedignore"), "r") as f:
                    lines=f.readlines()
                    lines = [l.strip() for l in lines
                             if l.strip() != "" and not l.startswith("#")]
                    self.add_ignore_patterns(root, base_path, lines)

            root_path =join(relpath(root, base_path))
            if self.is_ignored(join(root_path,"")):
                self.ignore_dir(root_path)
                dirs[:] = []
                continue

            for d in copy(dirs):
                dir_path = join(root, d)
                if d == '.hg' or d == '.git':
                    self.repo_dirs.append(dir_path)

                if (any(self._not_current_label(d, t) for t
                        in ['TARGET', 'TOOLCHAIN', 'FEATURE'])):
                    self._label_paths.append((dir_path, base_path))
                    self.ignore_dir(dir_path)
                    dirs.remove(d)
                elif (d.startswith('.') or d in self.legacy_ignore_dirs or
                      self.is_ignored(join(root_path, d, ""))):
                    self.ignore_dir(dir_path)
                    dirs.remove(d)

            # Add root to include paths
            root = root.rstrip("/")
            self.inc_dirs.append(root)
            self.file_basepath[root] = base_path

            for file in files:
                file_path = join(root, file)
                self._add_file(file_path, base_path)

    def _add_file(self, file_path, base_path):
        """ Add a single file into the resources object that was found by
        scanning starting as base_path
        """

        if  (self.is_ignored(relpath(file_path, base_path)) or
             basename(file_path).startswith(".")):
            self.ignore_dir(relpath(file_path, base_path))
            return

        self.file_basepath[file_path] = base_path
        _, ext = splitext(file_path)
        ext = ext.lower()

        if   ext == '.s':
            self.s_sources.append(file_path)

        elif ext == '.c':
            self.c_sources.append(file_path)

        elif ext == '.cpp' or ext == '.cc':
            self.cpp_sources.append(file_path)

        elif ext == '.h' or ext == '.hpp' or ext == '.hh':
            self.headers.append(file_path)

        elif ext == '.o':
            self.objects.append(file_path)

        elif ext in ('.a', '.ar'):
            self.libraries.append(file_path)
            self.lib_dirs.add(dirname(file_path))

        elif ext in ('.sct', '.icf', '.ld'):
            if self.linker_script is not None:
                self.notify.info("Warning: Multiple linker scripts detected: %s and %s" % (self.linker_script, file_path))
            else:
                self.linker_script = file_path

        elif ext == '.lib':
            self.lib_refs.append(file_path)

        elif ext == '.bld':
            self.lib_builds.append(file_path)

        elif basename(file_path) == '.hgignore':
            self.repo_files.append(file_path)

        elif basename(file_path) == '.gitignore':
            self.repo_files.append(file_path)

        elif ext == '.hex':
            self.hex_files.append(file_path)

        elif ext == '.bin':
            self.bin_files.append(file_path)

        elif ext == '.json':
            self.json_files.append(file_path)


    def scan_with_toolchain(self, src_paths, toolchain, dependencies_paths=None,
                            inc_dirs=None, exclude=True):
        """ Scan resources using initialized toolcain

        Positional arguments
        src_paths - the paths to source directories
        toolchain - valid toolchain object

        Keyword arguments
        dependencies_paths - dependency paths that we should scan for include dirs
        inc_dirs - additional include directories which should be added to
                   the scanner resources
        exclude - Exclude the toolchain's build directory from the resources
        """
        self.add_toolchain_labels(toolchain)
        for path in src_paths:
            if exists(path):
                if exclude:
                    self.add_directory(path, exclude_paths=[toolchain.build_dir])
                else:
                    self.add_directory(path)

        # Scan dependency paths for include dirs
        if dependencies_paths is not None:
            toolchain.progress("dep", dependencies_paths)
            for dep in dependencies_paths:
                lib_self = self.__class__(self.base_path, self.collect_ignores)\
                               .scan_with_toolchain([dep], toolchain)
                self.inc_dirs.extend(lib_self.inc_dirs)

        # Add additional include directories if passed
        if inc_dirs:
            if isinstance(inc_dirs, list):
                self.inc_dirs.extend(inc_dirs)
            else:
                self.inc_dirs.append(inc_dirs)

        # Load self into the config system which might expand/modify self
        # based on config data
        toolchain.config.load_resources(self)

        # Set the toolchain's configuration data
        toolchain.set_config_data(toolchain.config.get_config_data())

        return self

    def scan_with_config(self, src_paths, config):
        if config.target:
            self.add_target_labels(config.target)
        for path in src_paths:
            if exists(path):
                self.add_directory(path)
        config.load_resources(self)
        return self
