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
from collections import namedtuple, defaultdict
from copy import copy
from itertools import chain
from os import walk, sep
from os.path import (join, splitext, dirname, relpath, basename, split, normcase,
                     abspath, exists)

from .ignore import MbedIgnoreSet, IGNORE_FILENAME

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
    'TEST_APPS',
])
LEGACY_TOOLCHAIN_NAMES = {
    'ARM_STD':'ARM',
    'ARM_MICRO': 'uARM',
    'GCC_ARM': 'GCC_ARM',
    'GCC_CR': 'GCC_CR',
    'IAR': 'IAR',
    'ARMC6': 'ARMC6',
}


FileRef = namedtuple("FileRef", "name path")

class FileType(object):
    C_SRC = "c"
    CPP_SRC = "c++"
    ASM_SRC = "s"
    HEADER = "header"
    INC_DIR = "inc"
    LIB_DIR = "libdir"
    LIB = "lib"
    OBJECT = "o"
    HEX = "hex"
    BIN = "bin"
    JSON = "json"
    LD_SCRIPT = "ld"
    LIB_REF = "libref"
    BLD_REF = "bldref"
    REPO_DIR = "repodir"

    def __init__(self):
        raise NotImplemented

class Resources(object):
    ALL_FILE_TYPES = [
        FileType.C_SRC,
        FileType.CPP_SRC,
        FileType.ASM_SRC,
        FileType.HEADER,
        FileType.INC_DIR,
        FileType.LIB_DIR,
        FileType.LIB,
        FileType.OBJECT,
        FileType.HEX,
        FileType.BIN,
        FileType.JSON,
        FileType.LD_SCRIPT,
        FileType.LIB_REF,
        FileType.BLD_REF,
        FileType.REPO_DIR,
    ]

    def __init__(self, notify, collect_ignores=False):
        # publicly accessible things
        self.ignored_dirs = []

        # Pre-mbed 2.0 ignore dirs
        self._legacy_ignore_dirs = (LEGACY_IGNORE_DIRS)

        # Primate parameters
        self._notify = notify
        self._collect_ignores = collect_ignores

        # Storage for file references, indexed by file type
        self._file_refs = defaultdict(set)

        # Incremental scan related
        self._label_paths = []
        self._labels = {
            "TARGET": [], "TOOLCHAIN": [], "FEATURE": [], "COMPONENT": []
        }
        self._prefixed_labels = set()

        # Path seperator style (defaults to OS-specific seperator)
        self._sep = sep

        self._ignoreset = MbedIgnoreSet()

    def ignore_dir(self, directory):
        if self._collect_ignores:
            self.ignored_dirs.append(directory)

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

    def detect_duplicates(self):
        """Detect all potential ambiguities in filenames and report them with
        a toolchain notification
        """
        count = 0
        dupe_dict, dupe_headers = self._collect_duplicates(dict(), dict())
        for objname, filenames in dupe_dict.items():
            if len(filenames) > 1:
                count+=1
                self._notify.tool_error(
                    "Object file %s.o is not unique! It could be made from: %s"\
                    % (objname, " ".join(filenames)))
        for headername, locations in dupe_headers.items():
            if len(locations) > 1:
                count+=1
                self._notify.tool_error(
                    "Header file %s is not unique! It could be: %s" %\
                    (headername, " ".join(locations)))
        return count

    def win_to_unix(self):
        self._sep = "/"
        if self._sep != sep:
            for file_type in self.ALL_FILE_TYPES:
                v = [f._replace(name=f.name.replace(sep, self._sep)) for
                     f in self.get_file_refs(file_type)]
                self._file_refs[file_type] = v

    def __str__(self):
        s = []

        for (label, file_type) in (
                ('Include Directories', FileType.INC_DIR),
                ('Headers', FileType.HEADER),

                ('Assembly sources', FileType.ASM_SRC),
                ('C sources', FileType.C_SRC),
                ('C++ sources', FileType.CPP_SRC),

                ('Library directories', FileType.LIB_DIR),
                ('Objects', FileType.OBJECT),
                ('Libraries', FileType.LIB),

                ('Hex files', FileType.HEX),
                ('Bin files', FileType.BIN),
                ('Linker script', FileType.LD_SCRIPT)
            ):
            resources = self.get_file_refs(file_type)
            if resources:
                s.append('%s:\n  ' % label + '\n  '.join(
                    "%s -> %s" % (name, path) for name, path in resources))

        return '\n'.join(s)


    def _add_labels(self, prefix, labels):
        self._labels[prefix].extend(labels)
        self._prefixed_labels |= set("%s_%s" % (prefix, label) for label in labels)
        for path, base_path, into_path in self._label_paths:
            if basename(path) in self._prefixed_labels:
                self.add_directory(path, base_path, into_path)
        self._label_paths = [(p, b, i) for p, b, i in self._label_paths
                             if basename(p) not in self._prefixed_labels]

    def add_target_labels(self, target):
        self._add_labels("TARGET", target.labels)
        self._add_labels("COMPONENT", target.components)
        self.add_features(target.features)

    def add_features(self, features):
        self._add_labels("FEATURE", features)

    def add_toolchain_labels(self, toolchain):
        for prefix, value in toolchain.get_labels().items():
            self._add_labels(prefix, value)
        self._legacy_ignore_dirs -= set(
            [toolchain.target.name, LEGACY_TOOLCHAIN_NAMES[toolchain.name]])

    def add_ignore_patterns(self, root, base_path, patterns):
        real_base = relpath(root, base_path)
        self._ignoreset.add_ignore_patterns(real_base, patterns)

    def _not_current_label(self, dirname, label_type):
        return (dirname.startswith(label_type + "_") and
                dirname[len(label_type) + 1:] not in self._labels[label_type])

    def add_file_ref(self, file_type, file_name, file_path):
        if sep != self._sep:
            ref = FileRef(file_name.replace(sep, self._sep), file_path)
        else:
            ref = FileRef(file_name, file_path)
        self._file_refs[file_type].add(ref)

    def get_file_refs(self, file_type):
        """Return a list of FileRef for every file of the given type"""
        return list(self._file_refs[file_type])

    def _all_parents(self, files):
        for name in files:
            components = name.split(self._sep)
            start_at = 2 if components[0] in set(['', '.']) else 1
            for index, directory in reversed(list(enumerate(components))[start_at:]):
                if directory in self._prefixed_labels:
                    start_at = index + 1
                    break
            for n in range(start_at, len(components)):
                parent = self._sep.join(components[:n])
                yield parent

    def _get_from_refs(self, file_type, key):
        if file_type is FileType.INC_DIR:
            parents = set(self._all_parents(self._get_from_refs(
                FileType.HEADER, key)))
            parents.add(".")
        else:
            parents = set()
        return sorted(
            list(parents) + [key(f) for f in self.get_file_refs(file_type)]
        )


    def get_file_names(self, file_type):
        return self._get_from_refs(file_type, lambda f: f.name)

    def get_file_paths(self, file_type):
        return self._get_from_refs(file_type, lambda f: f.path)

    def add_files_to_type(self, file_type, files):
        for f in files:
            self.add_file_ref(file_type, f, f)

    @property
    def inc_dirs(self):
        return self.get_file_names(FileType.INC_DIR)

    @property
    def headers(self):
        return self.get_file_names(FileType.HEADER)

    @property
    def s_sources(self):
        return self.get_file_names(FileType.ASM_SRC)

    @property
    def c_sources(self):
        return self.get_file_names(FileType.C_SRC)

    @property
    def cpp_sources(self):
        return self.get_file_names(FileType.CPP_SRC)

    @property
    def lib_dirs(self):
        return self.get_file_names(FileType.LIB_DIR)

    @property
    def objects(self):
        return self.get_file_names(FileType.OBJECT)

    @property
    def libraries(self):
        return self.get_file_names(FileType.LIB)

    @property
    def lib_builds(self):
        return self.get_file_names(FileType.BLD_REF)

    @property
    def lib_refs(self):
        return self.get_file_names(FileType.LIB_REF)

    @property
    def linker_script(self):
        options = self.get_file_names(FileType.LD_SCRIPT)
        if options:
            return options[0]
        else:
            return None

    @property
    def hex_files(self):
        return self.get_file_names(FileType.HEX)

    @property
    def bin_files(self):
        return self.get_file_names(FileType.BIN)

    @property
    def json_files(self):
        return self.get_file_names(FileType.JSON)

    def add_directory(
            self,
            path,
            base_path=None,
            into_path=None,
            exclude_paths=None,
    ):
        """ Scan a directory and include its resources in this resources obejct

        Positional arguments:
        path - the path to search for resources

        Keyword arguments
        base_path - If this is part of an incremental scan, include the origin
                    directory root of the scan here
        into_path - Pretend that scanned files are within the specified
                    directory within a project instead of using their actual path
        exclude_paths - A list of paths that are to be excluded from a build
        """
        self._notify.progress("scan", abspath(path))

        if base_path is None:
            base_path = path
        if into_path is None:
            into_path = path
        if self._collect_ignores and path in self.ignored_dirs:
            self.ignored_dirs.remove(path)
        if exclude_paths:
            self.add_ignore_patterns(
                path, base_path, [join(e, "*") for e in exclude_paths])

        for root, dirs, files in walk(path, followlinks=True):
            # Check if folder contains .mbedignore
            if IGNORE_FILENAME in files:
                real_base = relpath(root, base_path)
                self._ignoreset.add_mbedignore(
                    real_base, join(root, IGNORE_FILENAME))

            root_path =join(relpath(root, base_path))
            if self._ignoreset.is_ignored(join(root_path,"")):
                self.ignore_dir(root_path)
                dirs[:] = []
                continue

            for d in copy(dirs):
                dir_path = join(root, d)
                if d == '.hg' or d == '.git':
                    fake_path = join(into_path, relpath(dir_path, base_path))
                    self.add_file_ref(FileType.REPO_DIR, fake_path, dir_path)

                if (any(self._not_current_label(d, t) for t
                        in self._labels.keys())):
                    self._label_paths.append((dir_path, base_path, into_path))
                    self.ignore_dir(dir_path)
                    dirs.remove(d)
                elif (d.startswith('.') or d in self._legacy_ignore_dirs or
                      self._ignoreset.is_ignored(join(root_path, d, ""))):
                    self.ignore_dir(dir_path)
                    dirs.remove(d)

            # Add root to include paths
            root = root.rstrip("/")

            for file in files:
                file_path = join(root, file)
                self._add_file(file_path, base_path, into_path)

    _EXT = {
        ".c": FileType.C_SRC,
        ".cc": FileType.CPP_SRC,
        ".cpp": FileType.CPP_SRC,
        ".s": FileType.ASM_SRC,
        ".h": FileType.HEADER,
        ".hh": FileType.HEADER,
        ".hpp": FileType.HEADER,
        ".o": FileType.OBJECT,
        ".hex": FileType.HEX,
        ".bin": FileType.BIN,
        ".json": FileType.JSON,
        ".a": FileType.LIB,
        ".ar": FileType.LIB,
        ".sct": FileType.LD_SCRIPT,
        ".ld": FileType.LD_SCRIPT,
        ".icf": FileType.LD_SCRIPT,
        ".lib": FileType.LIB_REF,
        ".bld": FileType.BLD_REF,
    }

    _DIR_EXT = {
        ".a": FileType.LIB_DIR,
        ".ar": FileType.LIB_DIR,
    }

    def _add_file(self, file_path, base_path, into_path):
        """ Add a single file into the resources object that was found by
        scanning starting as base_path
        """

        if  (self._ignoreset.is_ignored(relpath(file_path, base_path)) or
             basename(file_path).startswith(".")):
            self.ignore_dir(relpath(file_path, base_path))
            return

        fake_path = join(into_path, relpath(file_path, base_path))
        _, ext = splitext(file_path)
        try:
            file_type = self._EXT[ext.lower()]
            self.add_file_ref(file_type, fake_path, file_path)
        except KeyError:
            pass
        try:
            dir_type = self._DIR_EXT[ext.lower()]
            self.add_file_ref(dir_type, dirname(fake_path), dirname(file_path))
        except KeyError:
            pass


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
                into_path = relpath(path).strip(".\\/")
                if exclude:
                    self.add_directory(
                        path,
                        into_path=into_path,
                        exclude_paths=[toolchain.build_dir]
                    )
                else:
                    self.add_directory(path, into_path=into_path)

        # Scan dependency paths for include dirs
        if dependencies_paths is not None:
            toolchain.progress("dep", dependencies_paths)
            for dep in dependencies_paths:
                lib_self = self.__class__(self._notify, self._collect_ignores)\
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

