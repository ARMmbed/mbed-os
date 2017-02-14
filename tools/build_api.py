"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited

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
import tempfile
import datetime
import uuid
import struct
import zlib
import hashlib
from shutil import rmtree
from os.path import join, exists, dirname, basename, abspath, normpath, splitext
from os.path import relpath
from os import linesep, remove, makedirs
from time import time
from intelhex import IntelHex
from json import load, dump
from jinja2 import FileSystemLoader
from jinja2.environment import Environment

from .arm_pack_manager import Cache
from .utils import (mkdir, run_cmd, run_cmd_ext, NotSupportedException,
                    ToolException, InvalidReleaseTargetException,
                    intelhex_offset, integer)
from .paths import (MBED_CMSIS_PATH, MBED_TARGETS_PATH, MBED_LIBRARIES,
                    MBED_HEADER, MBED_DRIVERS, MBED_PLATFORM, MBED_HAL,
                    MBED_CONFIG_FILE, MBED_LIBRARIES_DRIVERS,
                    MBED_LIBRARIES_PLATFORM, MBED_LIBRARIES_HAL,
                    BUILD_DIR)
from .resources import Resources, FileType, FileRef
from .notifier.mock import MockNotifier
from .targets import TARGET_NAMES, TARGET_MAP, CORE_ARCH, set_targets_json_location
from .libraries import Library
from .toolchains import TOOLCHAIN_CLASSES, mbedToolchain
from .config import Config
from .build_profiles import find_build_profile, get_toolchain_profile, find_targets_json

RELEASE_VERSIONS = ['2', '5']

def prep_report(report, target_name, toolchain_name, id_name):
    """Setup report keys

    Positional arguments:
    report - the report to fill
    target_name - the target being used
    toolchain_name - the toolchain being used
    id_name - the name of the executable or library being built
    """
    if not target_name in report:
        report[target_name] = {}

    if not toolchain_name in report[target_name]:
        report[target_name][toolchain_name] = {}

    if not id_name in report[target_name][toolchain_name]:
        report[target_name][toolchain_name][id_name] = []

def prep_properties(properties, target_name, toolchain_name, vendor_label):
    """Setup test properties

    Positional arguments:
    properties - the dict to fill
    target_name - the target the test is targeting
    toolchain_name - the toolchain that will compile the test
    vendor_label - the vendor
    """
    if not target_name in properties:
        properties[target_name] = {}

    if not toolchain_name in properties[target_name]:
        properties[target_name][toolchain_name] = {}

    properties[target_name][toolchain_name]["target"] = target_name
    properties[target_name][toolchain_name]["vendor"] = vendor_label
    properties[target_name][toolchain_name]["toolchain"] = toolchain_name

def create_result(target_name, toolchain_name, id_name, description):
    """Create a result dictionary

    Positional arguments:
    target_name - the target being built for
    toolchain_name - the toolchain doing the building
    id_name - the name of the executable or library being built
    description - a human readable description of what's going on
    """
    cur_result = {}
    cur_result["target_name"] = target_name
    cur_result["toolchain_name"] = toolchain_name
    cur_result["id"] = id_name
    cur_result["description"] = description
    cur_result["elapsed_time"] = 0
    cur_result["output"] = ""

    return cur_result

def add_result_to_report(report, result):
    """Add a single result to a report dictionary

    Positional arguments:
    report - the report to append to
    result - the result to append
    """
    result["date"] = datetime.datetime.utcnow().isoformat()
    result["uuid"] = str(uuid.uuid1())
    target = result["target_name"]
    toolchain = result["toolchain_name"]
    id_name = result['id']
    result_wrap = {0: result}
    report[target][toolchain][id_name].append(result_wrap)

def get_config(src_paths, target, toolchain_name=None, app_config=None):
    """Get the configuration object for a target-toolchain combination

    Positional arguments:
    src_paths - paths to scan for the configuration files
    target - the device we are building for
    toolchain_name - the string that identifies the build tools
    """
    # Convert src_paths to a list if needed
    if not isinstance(src_paths, list):
        src_paths = [src_paths]

    res = Resources(MockNotifier())
    if toolchain_name:
        toolchain = prepare_toolchain(src_paths, None, target, toolchain_name,
                                      app_config=app_config)
        config = toolchain.config
        res.scan_with_toolchain(src_paths, toolchain, exclude=False)
    else:
        config = Config(target, src_paths, app_config=app_config)
        res.scan_with_config(src_paths, config)
    if config.has_regions:
        _ = list(config.regions)

    cfg, macros = config.get_config_data()
    features = config.get_features()
    return cfg, macros, features

def is_official_target(target_name, version):
    """ Returns True, None if a target is part of the official release for the
    given version. Return False, 'reason' if a target is not part of the
    official release for the given version.

    Positional arguments:
    target_name - Name if the target (ex. 'K64F')
    version - The release version string. Should be a string contained within
              RELEASE_VERSIONS
    """

    result = True
    reason = None
    target = TARGET_MAP[target_name]

    if hasattr(target, 'release_versions') \
       and version in target.release_versions:
        if version == '2':
            # For version 2, either ARM or uARM toolchain support is required
            required_toolchains = set(['ARM', 'uARM'])

            if not len(required_toolchains.intersection(
                    set(target.supported_toolchains))) > 0:
                result = False
                reason = ("Target '%s' must support " % target.name) + \
                    ("one of the folowing toolchains to be included in the") + \
                    ((" mbed 2.0 official release: %s" + linesep) %
                     ", ".join(required_toolchains)) + \
                    ("Currently it is only configured to support the ") + \
                    ("following toolchains: %s" %
                     ", ".join(target.supported_toolchains))

        elif version == '5':
            # For version 5, ARM, GCC_ARM, and IAR toolchain support is required
            required_toolchains = [
                set(['ARM', 'GCC_ARM', 'IAR']),
                set(['ARMC6'])
            ]
            supported_toolchains = set(target.supported_toolchains)

            if not any(r.issubset(supported_toolchains)
                       for r in required_toolchains):
                result = False
                reason = ("Target '%s' must support " % target.name) + \
                    ("ALL of the folowing toolchains to be included in the") + \
                    ((" mbed OS 5.0 official release: %s" + linesep) %
                     ", ".join(sorted(required_toolchains[0]))) + \
                    ("Currently it is only configured to support the ") + \
                    ("following toolchains: %s" %
                     ", ".join(sorted(supported_toolchains)))

            elif not target.default_lib == 'std':
                result = False
                reason = ("Target '%s' must set the " % target.name) + \
                    ("'default_lib' to 'std' to be included in the ") + \
                    ("mbed OS 5.0 official release." + linesep) + \
                    ("Currently it is set to '%s'" % target.default_lib)

        else:
            result = False
            reason = ("Target '%s' has set an invalid release version of '%s'" %
                      version) + \
                ("Please choose from the following release versions: %s" %
                 ', '.join(RELEASE_VERSIONS))

    else:
        result = False
        if not hasattr(target, 'release_versions'):
            reason = "Target '%s' " % target.name
            reason += "does not have the 'release_versions' key set"
        elif not version in target.release_versions:
            reason = "Target '%s' does not contain the version '%s' " % \
                     (target.name, version)
            reason += "in its 'release_versions' key"

    return result, reason

def transform_release_toolchains(toolchains, version):
    """ Given a list of toolchains and a release version, return a list of
    only the supported toolchains for that release

    Positional arguments:
    toolchains - The list of toolchains
    version - The release version string. Should be a string contained within
              RELEASE_VERSIONS
    """
    if version == '5':
        return ['ARM', 'GCC_ARM', 'IAR']
    else:
        return toolchains


def get_mbed_official_release(version):
    """ Given a release version string, return a tuple that contains a target
    and the supported toolchains for that release.
    Ex. Given '2', return (('LPC1768', ('ARM', 'GCC_ARM')),
                           ('K64F', ('ARM', 'GCC_ARM')), ...)

    Positional arguments:
    version - The version string. Should be a string contained within
              RELEASE_VERSIONS
    """

    mbed_official_release = (
        tuple(
            tuple(
                [
                    TARGET_MAP[target].name,
                    tuple(transform_release_toolchains(
                        TARGET_MAP[target].supported_toolchains, version))
                ]
            ) for target in TARGET_NAMES \
            if (hasattr(TARGET_MAP[target], 'release_versions')
                and version in TARGET_MAP[target].release_versions)
        )
    )

    for target in mbed_official_release:
        is_official, reason = is_official_target(target[0], version)

        if not is_official:
            raise InvalidReleaseTargetException(reason)

    return mbed_official_release

ARM_COMPILERS = ("ARM", "ARMC6", "uARM")
def target_supports_toolchain(target, toolchain_name):
    if toolchain_name in ARM_COMPILERS:
        return any(tc in target.supported_toolchains for tc in ARM_COMPILERS)
    else:
        return toolchain_name in target.supported_toolchains


def prepare_toolchain(src_paths, build_dir, target, toolchain_name,
                      macros=None, clean=False, jobs=1,
                      notify=None, config=None, app_config=None,
                      build_profile=None, ignore=None):
    """ Prepares resource related objects - toolchain, target, config

    Positional arguments:
    src_paths - the paths to source directories
    target - ['LPC1768', 'LPC11U24', etc.]
    toolchain_name - ['ARM', 'uARM', 'GCC_ARM', 'GCC_CR']

    Keyword arguments:
    macros - additional macros
    clean - Rebuild everything if True
    jobs - how many compilers we can run at once
    notify - Notify function for logs
    config - a Config object to use instead of creating one
    app_config - location of a chosen mbed_app.json file
    build_profile - a list of mergeable build profiles
    ignore - list of paths to add to mbedignore
    """

    # We need to remove all paths which are repeated to avoid
    # multiple compilations and linking with the same objects
    src_paths = [src_paths[0]] + list(set(src_paths[1:]))

    # If the configuration object was not yet created, create it now
    config = config or Config(target, src_paths, app_config=app_config)
    target = config.target
    if not target_supports_toolchain(target, toolchain_name):
        raise NotSupportedException(
            "Target {} is not supported by toolchain {}".format(
                target.name, toolchain_name))
    if (toolchain_name == "ARM" and CORE_ARCH[target.core] == 8):
        toolchain_name = "ARMC6"

    try:
        cur_tc = TOOLCHAIN_CLASSES[toolchain_name]
    except KeyError:
        raise KeyError("Toolchain %s not supported" % toolchain_name)

    profile = {'c': [], 'cxx': [], 'common': [], 'asm': [], 'ld': []}
    for contents in build_profile or []:
        for key in profile:
            profile[key].extend(contents[toolchain_name].get(key, []))

    toolchain = cur_tc(
        target, notify, macros, build_dir=build_dir, build_profile=profile)

    toolchain.config = config
    toolchain.jobs = jobs
    toolchain.build_all = clean

    if ignore:
        toolchain.add_ignore_patterns(root=".", base_path=".", patterns=ignore)

    return toolchain

def _printihex(ihex):
    import pprint
    pprint.PrettyPrinter().pprint(ihex.todict())

def _real_region_size(region):
    try:
        part = intelhex_offset(region.filename, offset=region.start)
        return (part.maxaddr() - part.minaddr()) + 1
    except AttributeError:
        return region.size


def _fill_header(region_list, current_region):
    """Fill an application header region

    This is done it three steps:
     * Fill the whole region with zeros
     * Fill const, timestamp and size entries with their data
     * Fill the digests using this header as the header region
    """
    region_dict = {r.name: r for r in region_list}
    header = IntelHex()
    header.puts(current_region.start, b'\x00' * current_region.size)
    start = current_region.start
    for member in current_region.filename:
        _, type, subtype, data = member
        member_size = Config.header_member_size(member)
        if type == "const":
            fmt = {
                "8le": ">B", "16le": "<H", "32le": "<L", "64le": "<Q",
                "8be": "<B", "16be": ">H", "32be": ">L", "64be": ">Q"
            }[subtype]
            header.puts(start, struct.pack(fmt, integer(data, 0)))
        elif type == "timestamp":
            fmt = {"32le": "<L", "64le": "<Q",
                   "32be": ">L", "64be": ">Q"}[subtype]
            header.puts(start, struct.pack(fmt, time()))
        elif type == "size":
            fmt = {"32le": "<L", "64le": "<Q",
                   "32be": ">L", "64be": ">Q"}[subtype]
            size = sum(_real_region_size(region_dict[r]) for r in data)
            header.puts(start, struct.pack(fmt, size))
        elif type  == "digest":
            if data == "header":
                ih = header[:start]
            else:
                ih = intelhex_offset(region_dict[data].filename, offset=region_dict[data].start)
            if subtype.startswith("CRCITT32"):
                fmt = {"CRCITT32be": ">l", "CRCITT32le": "<l"}[subtype]
                header.puts(start, struct.pack(fmt, zlib.crc32(ih.tobinarray())))
            elif subtype.startswith("SHA"):
                if subtype == "SHA256":
                    hash = hashlib.sha256()
                elif subtype == "SHA512":
                    hash = hashlib.sha512()
                hash.update(ih.tobinarray())
                header.puts(start, hash.digest())
        start += Config.header_member_size(member)
    return header

def merge_region_list(region_list, destination, notify, padding=b'\xFF'):
    """Merge the region_list into a single image

    Positional Arguments:
    region_list - list of regions, which should contain filenames
    destination - file name to write all regions to
    padding - bytes to fill gapps with
    """
    merged = IntelHex()
    _, format = splitext(destination)

    notify.info("Merging Regions")

    for region in region_list:
        if region.active and not region.filename:
            raise ToolException("Active region has no contents: No file found.")
        if isinstance(region.filename, list):
            header_basename, _ = splitext(destination)
            header_filename = header_basename + "_header.hex"
            _fill_header(region_list, region).tofile(header_filename, format='hex')
            region = region._replace(filename=header_filename)
        if region.filename:
            notify.info("  Filling region %s with %s" % (region.name, region.filename))
            part = intelhex_offset(region.filename, offset=region.start)
            part.start_addr = None
            part_size = (part.maxaddr() - part.minaddr()) + 1
            if part_size > region.size:
                raise ToolException("Contents of region %s does not fit"
                                    % region.name)
            merged.merge(part)
            pad_size = region.size - part_size
            if pad_size > 0 and region != region_list[-1]:
                notify.info("  Padding region %s with 0x%x bytes" %
                            (region.name, pad_size))
                if format is ".hex":
                    """The offset will be in the hex file generated when we're done,
                    so we can skip padding here"""
                else:
                    merged.puts(merged.maxaddr() + 1, padding * pad_size)

    if not exists(dirname(destination)):
        makedirs(dirname(destination))
    notify.info("Space used after regions merged: 0x%x" %
                (merged.maxaddr() - merged.minaddr() + 1))
    merged.tofile(destination, format=format.strip("."))


UPDATE_WHITELIST = (
    "application",
)


def build_project(src_paths, build_path, target, toolchain_name,
                  libraries_paths=None, linker_script=None, clean=False,
                  notify=None, name=None, macros=None, inc_dirs=None, jobs=1,
                  report=None, properties=None, project_id=None,
                  project_description=None, config=None,
                  app_config=None, build_profile=None, stats_depth=None, ignore=None):
    """ Build a project. A project may be a test or a user program.

    Positional arguments:
    src_paths - a path or list of paths that contain all files needed to build
                the project
    build_path - the directory where all of the object files will be placed
    target - the MCU or board that the project will compile for
    toolchain_name - the name of the build tools

    Keyword arguments:
    libraries_paths - The location of libraries to include when linking
    linker_script - the file that drives the linker to do it's job
    clean - Rebuild everything if True
    notify - Notify function for logs
    name - the name of the project
    macros - additional macros
    inc_dirs - additional directories where include files may be found
    jobs - how many compilers we can run at once
    report - a dict where a result may be appended
    properties - UUUUHHHHH beats me
    project_id - the name put in the report
    project_description - the human-readable version of what this thing does
    config - a Config object to use instead of creating one
    app_config - location of a chosen mbed_app.json file
    build_profile - a dict of flags that will be passed to the compiler
    stats_depth - depth level for memap to display file/dirs
    ignore - list of paths to add to mbedignore
    """
    # Convert src_path to a list if needed
    if not isinstance(src_paths, list):
        src_paths = [src_paths]
    # Extend src_paths wiht libraries_paths
    if libraries_paths is not None:
        src_paths.extend(libraries_paths)
        inc_dirs.extend(map(dirname, libraries_paths))

    if clean and exists(build_path):
        rmtree(build_path)
    mkdir(build_path)

    ###################################
    # mbed Classic/2.0/libary support #

    # Find build system profile
    profile = None
    targets_json = None
    for path in src_paths:
        profile = find_build_profile(path) or profile
        if profile:
            targets_json = join(dirname(abspath(__file__)), 'legacy_targets.json')
        else:
            targets_json = find_targets_json(path) or targets_json

    # Apply targets.json to active targets
    if targets_json:
        notify.info("Using targets from %s" % targets_json)
        set_targets_json_location(targets_json)

    # Apply profile to toolchains
    if profile:
        def init_hook(self):
            profile_data = get_toolchain_profile(self.name, profile)
            if not profile_data:
                return
            notify.info("Using toolchain %s profile %s" % (self.name, profile))

            for k,v in profile_data.items():
                if self.flags.has_key(k):
                    self.flags[k] = v
                else:
                    setattr(self, k, v)

        mbedToolchain.init = init_hook

    # mbed Classic/2.0/libary support #
    ###################################

    toolchain = prepare_toolchain(
        src_paths, build_path, target, toolchain_name, macros=macros,
        clean=clean, jobs=jobs, notify=notify, config=config,
        app_config=app_config, build_profile=build_profile, ignore=ignore)
    toolchain.version_check()

    # The first path will give the name to the library
    name = (name or toolchain.config.name or
            basename(normpath(abspath(src_paths[0]))))
    notify.info("Building project %s (%s, %s)" %
                (name, toolchain.target.name, toolchain_name))

    # Initialize reporting
    if report != None:
        start = time()
        # If project_id is specified, use that over the default name
        id_name = project_id.upper() if project_id else name.upper()
        description = project_description if project_description else name
        vendor_label = toolchain.target.extra_labels[0]
        prep_report(report, toolchain.target.name, toolchain_name, id_name)
        cur_result = create_result(toolchain.target.name, toolchain_name,
                                   id_name, description)
        if properties != None:
            prep_properties(properties, toolchain.target.name, toolchain_name,
                            vendor_label)

    try:
        resources = Resources(notify).scan_with_toolchain(
            src_paths, toolchain, inc_dirs=inc_dirs)

        # Change linker script if specified
        if linker_script is not None:
            resources.add_file_ref(linker_script, linker_script)

        # Compile Sources
        objects = toolchain.compile_sources(resources, sorted(resources.get_file_paths(FileType.INC_DIR)))
        resources.add_files_to_type(FileType.OBJECT, objects)

        # Link Program
        if toolchain.config.has_regions:
            binary, _ = toolchain.link_program(resources, build_path, name + "_application")
            region_list = list(toolchain.config.regions)
            region_list = [r._replace(filename=binary) if r.active else r
                           for r in region_list]
            res = "%s.%s" % (join(build_path, name),
                             getattr(toolchain.target, "OUTPUT_EXT", "bin"))
            merge_region_list(region_list, res, notify)
            update_regions = [
                r for r in region_list if r.name in UPDATE_WHITELIST
            ]
            if update_regions:
                update_res = "%s_update.%s" % (
                    join(build_path, name),
                    getattr(toolchain.target, "OUTPUT_EXT", "bin")
                )
                merge_region_list(update_regions, update_res, notify)
                res = (res, update_res)
            else:
                res = (res, None)
        else:
            res, _ = toolchain.link_program(resources, build_path, name)
            res = (res, None)

        memap_instance = getattr(toolchain, 'memap_instance', None)
        memap_table = ''
        if memap_instance:
            # Write output to stdout in text (pretty table) format
            memap_table = memap_instance.generate_output('table', stats_depth)
            notify.info(memap_table)

            # Write output to file in JSON format
            map_out = join(build_path, name + "_map.json")
            memap_instance.generate_output('json', stats_depth, map_out)

            # Write output to file in CSV format for the CI
            map_csv = join(build_path, name + "_map.csv")
            memap_instance.generate_output('csv-ci', stats_depth, map_csv)

            map_html = join(build_path, name + "_map.html")
            memap_instance.generate_output('html', stats_depth, map_html)

        resources.detect_duplicates()

        if report != None:
            end = time()
            cur_result["elapsed_time"] = end - start
            cur_result["result"] = "OK"
            cur_result["memory_usage"] = (memap_instance.mem_report
                                          if memap_instance is not None else None)
            cur_result["bin"] = res[0]
            cur_result["elf"] = splitext(res[0])[0] + ".elf"
            cur_result.update(toolchain.report)

            add_result_to_report(report, cur_result)

        return res

    except Exception as exc:
        if report != None:
            end = time()

            if isinstance(exc, NotSupportedException):
                cur_result["result"] = "NOT_SUPPORTED"
            else:
                cur_result["result"] = "FAIL"

            cur_result["elapsed_time"] = end - start

            add_result_to_report(report, cur_result)
        # Let Exception propagate
        raise

def build_library(src_paths, build_path, target, toolchain_name,
                  dependencies_paths=None, name=None, clean=False,
                  archive=True, notify=None, macros=None, inc_dirs=None, jobs=1,
                  report=None, properties=None, project_id=None,
                  remove_config_header_file=False, app_config=None,
                  build_profile=None, ignore=None):
    """ Build a library

    Positional arguments:
    src_paths - a path or list of paths that contain all files needed to build
                the library
    build_path - the directory where all of the object files will be placed
    target - the MCU or board that the project will compile for
    toolchain_name - the name of the build tools

    Keyword arguments:
    dependencies_paths - The location of libraries to include when linking
    name - the name of the library
    clean - Rebuild everything if True
    archive - whether the library will create an archive file
    notify - Notify function for logs
    macros - additional macros
    inc_dirs - additional directories where include files may be found
    jobs - how many compilers we can run at once
    report - a dict where a result may be appended
    properties - UUUUHHHHH beats me
    project_id - the name that goes in the report
    remove_config_header_file - delete config header file when done building
    app_config - location of a chosen mbed_app.json file
    build_profile - a dict of flags that will be passed to the compiler
    ignore - list of paths to add to mbedignore
    """

    # Convert src_path to a list if needed
    if not isinstance(src_paths, list):
        src_paths = [src_paths]
    src_paths = [relpath(s) for s in src_paths]

    # Build path
    if archive:
        # Use temp path when building archive
        tmp_path = join(build_path, '.temp')
        mkdir(tmp_path)
    else:
        tmp_path = build_path

    # Clean the build directory
    if clean and exists(tmp_path):
        rmtree(tmp_path)
    mkdir(tmp_path)

    # Pass all params to the unified prepare_toolchain()
    toolchain = prepare_toolchain(
        src_paths, build_path, target, toolchain_name, macros=macros,
        clean=clean, jobs=jobs, notify=notify, app_config=app_config,
        build_profile=build_profile, ignore=ignore)

    # The first path will give the name to the library
    if name is None:
        name = basename(normpath(abspath(src_paths[0])))
    notify.info("Building library %s (%s, %s)" %
                   (name, toolchain.target.name, toolchain_name))

    # Initialize reporting
    if report != None:
        start = time()
        # If project_id is specified, use that over the default name
        id_name = project_id.upper() if project_id else name.upper()
        description = name
        vendor_label = toolchain.target.extra_labels[0]
        prep_report(report, toolchain.target.name, toolchain_name, id_name)
        cur_result = create_result(toolchain.target.name, toolchain_name,
                                   id_name, description)
        cur_result['type'] = 'library'
        if properties != None:
            prep_properties(properties, toolchain.target.name, toolchain_name,
                            vendor_label)

    for src_path in src_paths:
        if not exists(src_path):
            error_msg = "The library source folder does not exist: %s", src_path
            if report != None:
                cur_result["output"] = error_msg
                cur_result["result"] = "FAIL"
                add_result_to_report(report, cur_result)
            raise Exception(error_msg)

    try:
        res = Resources(notify).scan_with_toolchain(
            src_paths, toolchain, dependencies_paths, inc_dirs=inc_dirs)

        # Copy headers, objects and static libraries - all files needed for
        # static lib
        to_copy = (
            res.get_file_refs(FileType.HEADER) +
            res.get_file_refs(FileType.OBJECT) +
            res.get_file_refs(FileType.LIB) +
            res.get_file_refs(FileType.JSON) +
            res.get_file_refs(FileType.LD_SCRIPT) +
            res.get_file_refs(FileType.HEX) +
            res.get_file_refs(FileType.BIN)
        )
        toolchain.copy_files(to_copy, build_path)
        # Compile Sources
        objects = toolchain.compile_sources(
            res, res.get_file_paths(FileType.INC_DIR))
        res.add_files_to_type(FileType.OBJECT, objects)

        if archive:
            toolchain.build_library(objects, build_path, name)

        if remove_config_header_file:
            config_header_path = toolchain.get_config_header()
            if config_header_path:
                remove(config_header_path)

        if report != None:
            end = time()
            cur_result["elapsed_time"] = end - start
            cur_result["result"] = "OK"
            add_result_to_report(report, cur_result)
        return True

    except Exception as exc:
        if report != None:
            end = time()

            if isinstance(exc, ToolException):
                cur_result["result"] = "FAIL"
            elif isinstance(exc, NotSupportedException):
                cur_result["result"] = "NOT_SUPPORTED"

            cur_result["elapsed_time"] = end - start

            add_result_to_report(report, cur_result)

        # Let Exception propagate
        raise

######################
### Legacy methods ###
######################

def mbed2_obj_path(target_name, toolchain_name):
    real_tc_name = TOOLCHAIN_CLASSES[toolchain_name].__name__
    return join("TARGET_" + target_name, "TOOLCHAIN_" + real_tc_name)

def build_lib(lib_id, target, toolchain_name, clean=False, macros=None,
              notify=None, jobs=1, report=None, properties=None,
              build_profile=None, ignore=None):
    """ Legacy method for building mbed libraries

    Positional arguments:
    lib_id - the library's unique identifier
    target - the MCU or board that the project will compile for
    toolchain_name - the name of the build tools

    Keyword arguments:
    clean - Rebuild everything if True
    macros - additional macros
    notify - Notify function for logs
    jobs - how many compilers we can run at once
    report - a dict where a result may be appended
    properties - UUUUHHHHH beats me
    build_profile - a dict of flags that will be passed to the compiler
    ignore - list of paths to add to mbedignore
    """
    lib = Library(lib_id)
    if not lib.is_supported(target, toolchain_name):
        print('Library "%s" is not yet supported on target %s with toolchain %s'
              % (lib_id, target.name, toolchain_name))
        return False

    # We need to combine macros from parameter list with macros from library
    # definition
    lib_macros = lib.macros if lib.macros else []
    if macros:
        macros.extend(lib_macros)
    else:
        macros = lib_macros

    src_paths = lib.source_dir
    build_path = lib.build_dir
    dependencies_paths = lib.dependencies
    inc_dirs = lib.inc_dirs

    if not isinstance(src_paths, list):
        src_paths = [src_paths]

    # The first path will give the name to the library
    name = basename(src_paths[0])

    if report is not None:
        start = time()
        id_name = name.upper()
        description = name
        vendor_label = target.extra_labels[0]
        cur_result = None
        prep_report(report, target.name, toolchain_name, id_name)
        cur_result = create_result(target.name, toolchain_name, id_name,
                                   description)

        if properties != None:
            prep_properties(properties, target.name, toolchain_name,
                            vendor_label)

    for src_path in src_paths:
        if not exists(src_path):
            error_msg = "The library source folder does not exist: %s", src_path

            if report != None:
                cur_result["output"] = error_msg
                cur_result["result"] = "FAIL"
                add_result_to_report(report, cur_result)

            raise Exception(error_msg)

    try:
        # Toolchain instance
        # Create the desired build directory structure
        bin_path = join(build_path, mbed2_obj_path(target.name, toolchain_name))
        mkdir(bin_path)
        tmp_path = join(build_path, '.temp', mbed2_obj_path(target.name,
                                                            toolchain_name))
        mkdir(tmp_path)

        toolchain = prepare_toolchain(
            src_paths, tmp_path, target, toolchain_name, macros=macros,
            notify=notify, build_profile=build_profile, jobs=jobs, clean=clean,
            ignore=ignore)

        notify.info("Building library %s (%s, %s)" %
                    (name.upper(), target.name, toolchain_name))

        # Take into account the library configuration (MBED_CONFIG_FILE)
        config = toolchain.config
        config.add_config_files([MBED_CONFIG_FILE])

        # Scan Resources
        resources = Resources(notify).scan_with_toolchain(
            src_paths + (lib.inc_dirs_ext or []), toolchain,
            inc_dirs=inc_dirs, dependencies_paths=dependencies_paths)

        # Copy Headers
        toolchain.copy_files(
            resources.get_file_refs(FileType.HEADER), build_path)

        dependencies_include_dir = Resources(notify).sacn_with_toolchain([build_path], toolchain).inc_dirs

        # Compile Sources
        objects = []
        for resource in resources:
            objects.extend(toolchain.compile_sources(resource, dependencies_include_dir))

        needed_update = toolchain.build_library(objects, bin_path, name)

        if report != None and needed_update:
            end = time()
            cur_result["elapsed_time"] = end - start
            cur_result["result"] = "OK"

            add_result_to_report(report, cur_result)
        return True

    except Exception:
        if report != None:
            end = time()
            cur_result["result"] = "FAIL"
            cur_result["elapsed_time"] = end - start

            add_result_to_report(report, cur_result)

        # Let Exception propagate
        raise


# A number of compiled files need to be copied as objects as the linker
# will not search for weak symbol overrides in archives. These are:
#   - mbed_retarget.o: to make sure that the C standard lib symbols get
#                      overridden
#   - mbed_board.o: `mbed_die` is weak
#   - mbed_overrides.o: this contains platform overrides of various
#                       weak SDK functions
#   - mbed_main.o: this contains main redirection
#   - mbed_sdk_boot.o: this contains the main boot code in
#   - PeripheralPins.o: PinMap can be weak
SEPARATE_NAMES = [
    'PeripheralPins.o',
    'mbed_retarget.o',
    'mbed_board.o',
    'mbed_overrides.o',
    'mbed_main.o',
    'mbed_sdk_boot.o',
]


def build_mbed_libs(target, toolchain_name, clean=False, macros=None,
                    notify=None, jobs=1, report=None, properties=None,
                    build_profile=None, ignore=None):
    """ Build legacy libraries for a target and toolchain pair

    Positional arguments:
    target - the MCU or board that the project will compile for
    toolchain_name - the name of the build tools

    Keyword arguments:
    clean - Rebuild everything if True
    macros - additional macros
    notify - Notify function for logs
    jobs - how many compilers we can run at once
    report - a dict where a result may be appended
    properties - UUUUHHHHH beats me
    build_profile - a dict of flags that will be passed to the compiler
    ignore - list of paths to add to mbedignore

    Return - True if target + toolchain built correctly, False if not supported
    """

    if report is not None:
        start = time()
        id_name = "MBED"
        description = "mbed SDK"
        vendor_label = target.extra_labels[0]
        cur_result = None
        prep_report(report, target.name, toolchain_name, id_name)
        cur_result = create_result(
            target.name, toolchain_name, id_name, description)
        if properties is not None:
            prep_properties(
                properties, target.name, toolchain_name, vendor_label)

    if toolchain_name not in target.supported_toolchains:
        supported_toolchains_text = ", ".join(target.supported_toolchains)
        notify.info('The target {} does not support the toolchain {}'.format(
            target.name,
            toolchain_name
        ))
        notify.info('{} supports {} toolchain{}'.format(
            target.name,
            supported_toolchains_text,
            's' if len(target.supported_toolchains) > 1 else ''
        ))

        if report is not None:
            cur_result["result"] = "SKIP"
            add_result_to_report(report, cur_result)

        return False

    try:
        # Source and Build Paths
        build_toolchain = join(
            MBED_LIBRARIES, mbed2_obj_path(target.name, toolchain_name))
        mkdir(build_toolchain)

        tmp_path = join(
            MBED_LIBRARIES,
            '.temp',
            mbed2_obj_path(target.name, toolchain_name)
        )
        mkdir(tmp_path)

        # Toolchain and config
        toolchain = prepare_toolchain(
            [""], tmp_path, target, toolchain_name, macros=macros, notify=notify,
            build_profile=build_profile, jobs=jobs, clean=clean, ignore=ignore)

        config = toolchain.config
        config.add_config_files([MBED_CONFIG_FILE])
        toolchain.set_config_data(toolchain.config.get_config_data())

        # distribute header files
        toolchain.copy_files(
            [FileRef(basename(MBED_HEADER),MBED_HEADER)], MBED_LIBRARIES)
        library_incdirs = [dirname(MBED_LIBRARIES), MBED_LIBRARIES]

        for dir, dest in [(MBED_DRIVERS, MBED_LIBRARIES_DRIVERS),
                          (MBED_PLATFORM, MBED_LIBRARIES_PLATFORM),
                          (MBED_HAL, MBED_LIBRARIES_HAL)]:
            resources = Resources(notify).scan_with_toolchain([dir], toolchain)
            toolchain.copy_files(
                [FileRef(basename(p), p) for p
                 in resources.get_file_paths(FileType.HEADER)] ,
                dest)
            library_incdirs.append(dest)

        # collect resources of the libs to compile
        cmsis_res = Resources(notify).scan_with_toolchain(
            [MBED_CMSIS_PATH], toolchain)
        hal_res = Resources(notify).scan_with_toolchain(
            [MBED_TARGETS_PATH], toolchain)
        mbed_resources = Resources(notify).scan_with_toolchain(
            [MBED_DRIVERS, MBED_PLATFORM, MBED_HAL], toolchain)

        incdirs = cmsis_res.inc_dirs + hal_res.inc_dirs + library_incdirs

        # Build Things
        notify.info("Building library %s (%s, %s)" %
                    ('MBED', target.name, toolchain_name))
        objects = toolchain.compile_sources(mbed_resources, incdirs)
        separate_objects = []

        for obj in objects:
            for name in SEPARATE_NAMES:
                if obj.endswith(name):
                    separate_objects.append(obj)

        for obj in separate_objects:
            objects.remove(obj)

        toolchain.build_library(objects, build_toolchain, "mbed")
        notify.info("Building library %s (%s, %s)" %
                    ('CMSIS', target.name, toolchain_name))
        cmsis_objects = toolchain.compile_sources(cmsis_res, incdirs + [tmp_path])
        notify.info("Building library %s (%s, %s)" %
                    ('HAL', target.name, toolchain_name))
        hal_objects = toolchain.compile_sources(hal_res, incdirs + [tmp_path])

        # Copy everything into the build directory
        to_copy_paths = [
            hal_res.get_file_paths(FileType.HEADER),
            hal_res.get_file_paths(FileType.HEX),
            hal_res.get_file_paths(FileType.BIN),
            hal_res.get_file_paths(FileType.LIB),
            cmsis_res.get_file_paths(FileType.HEADER),
            cmsis_res.get_file_paths(FileType.BIN),
            cmsis_res.get_file_paths(FileType.LD_SCRIPT),
            hal_res.get_file_paths(FileType.LD_SCRIPT),
            [MBED_CONFIG_FILE],
            cmsis_objects,
            hal_objects,
            separate_objects,
        ]
        to_copy = [FileRef(basename(p), p) for p in sum(to_copy_paths, [])]
        toolchain.copy_files(to_copy, build_toolchain)

        if report is not None:
            end = time()
            cur_result["elapsed_time"] = end - start
            cur_result["result"] = "OK"
            add_result_to_report(report, cur_result)

        return True

    except Exception as exc:
        if report is not None:
            end = time()
            cur_result["result"] = "FAIL"
            cur_result["elapsed_time"] = end - start

            cur_result["output"] += str(exc)

            add_result_to_report(report, cur_result)
        raise


def get_unique_supported_toolchains(release_targets=None):
    """ Get list of all unique toolchains supported by targets

    Keyword arguments:
    release_targets - tuple structure returned from get_mbed_official_release().
                      If release_targets is not specified, then it queries all
                      known targets
    """
    unique_supported_toolchains = []

    if not release_targets:
        for target in TARGET_NAMES:
            for toolchain in TARGET_MAP[target].supported_toolchains:
                if toolchain not in unique_supported_toolchains:
                    unique_supported_toolchains.append(toolchain)
    else:
        for target in release_targets:
            for toolchain in target[1]:
                if toolchain not in unique_supported_toolchains:
                    unique_supported_toolchains.append(toolchain)

    return unique_supported_toolchains


def _lowercase_release_version(release_version):
    try:
        return release_version.lower()
    except AttributeError:
        return 'all'

def mcu_toolchain_list(release_version='5'):
    """  Shows list of toolchains

    """
    release_version = _lowercase_release_version(release_version)
    version_release_targets = {}
    version_release_target_names = {}

    for version in RELEASE_VERSIONS:
        version_release_targets[version] = get_mbed_official_release(version)
        version_release_target_names[version] = [x[0] for x in
                                                 version_release_targets[
                                                     version]]

    if release_version in RELEASE_VERSIONS:
        release_targets = version_release_targets[release_version]
    else:
        release_targets = None

    unique_supported_toolchains = get_unique_supported_toolchains(
        release_targets)
    columns = ["mbed OS %s" % x for x in RELEASE_VERSIONS] + unique_supported_toolchains
    return "\n".join(columns)


def mcu_target_list(release_version='5'):
    """  Shows target list

    """
    release_version = _lowercase_release_version(release_version)
    version_release_targets = {}
    version_release_target_names = {}

    for version in RELEASE_VERSIONS:
        version_release_targets[version] = get_mbed_official_release(version)
        version_release_target_names[version] = [x[0] for x in
                                                 version_release_targets[
                                                     version]]

    if release_version in RELEASE_VERSIONS:
        release_targets = version_release_targets[release_version]
    else:
        release_targets = None

    target_names = []

    if release_targets:
        target_names = [x[0] for x in release_targets]
    else:
        target_names = TARGET_NAMES

    return "\n".join(target_names)


def mcu_toolchain_matrix(verbose_html=False, platform_filter=None,
                         release_version='5'):
    """  Shows target map using prettytable

    Keyword arguments:
    verbose_html - emit html instead of a simple table
    platform_filter - remove results that match the string
    release_version - get the matrix for this major version number
    """
    # Only use it in this function so building works without extra modules
    from prettytable import PrettyTable, HEADER
    release_version = _lowercase_release_version(release_version)
    version_release_targets = {}
    version_release_target_names = {}

    for version in RELEASE_VERSIONS:
        version_release_targets[version] = get_mbed_official_release(version)
        version_release_target_names[version] = [x[0] for x in
                                                 version_release_targets[
                                                     version]]

    if release_version in RELEASE_VERSIONS:
        release_targets = version_release_targets[release_version]
    else:
        release_targets = None

    unique_supported_toolchains = get_unique_supported_toolchains(
        release_targets)
    prepend_columns = ["Target"] + ["mbed OS %s" % x for x in RELEASE_VERSIONS]

    # All tests status table print
    columns = prepend_columns + unique_supported_toolchains
    table_printer = PrettyTable(columns, junction_char="|", hrules=HEADER)
    # Align table
    for col in columns:
        table_printer.align[col] = "c"
    table_printer.align["Target"] = "l"

    perm_counter = 0
    target_counter = 0

    target_names = []

    if release_targets:
        target_names = [x[0] for x in release_targets]
    else:
        target_names = TARGET_NAMES

    for target in sorted(target_names):
        if platform_filter is not None:
            # FIlter out platforms using regex
            if re.search(platform_filter, target) is None:
                continue
        target_counter += 1

        row = [target]  # First column is platform name

        for version in RELEASE_VERSIONS:
            if target in version_release_target_names[version]:
                text = "Supported"
            else:
                text = "-"
            row.append(text)

        for unique_toolchain in unique_supported_toolchains:
            tgt_obj = TARGET_MAP[target]
            if (unique_toolchain in tgt_obj.supported_toolchains or
                (unique_toolchain == "ARMC6" and
                 "ARM" in tgt_obj.supported_toolchains) or
                (unique_toolchain == "ARM" and
                 "ARMC6" in tgt_obj.supported_toolchains and
                 CORE_ARCH[tgt_obj.core] == 8)):
                text = "Supported"
                perm_counter += 1
            else:
                text = "-"

            row.append(text)
        table_printer.add_row(row)

    result = table_printer.get_html_string() if verbose_html \
             else table_printer.get_string()
    result += "\n"
    result += "Supported targets: %d\n"% (target_counter)
    if target_counter == 1:
        result += "Supported toolchains: %d"% (perm_counter)
    return result


def get_target_supported_toolchains(target):
    """ Returns target supported toolchains list

    Positional arguments:
    target - the target to get the supported toolchains of
    """
    return TARGET_MAP[target].supported_toolchains if target in TARGET_MAP \
        else None


def print_build_results(result_list, build_name):
    """ Generate result string for build results

    Positional arguments:
    result_list - the list of results to print
    build_name - the name of the build we are printing result for
    """
    result = ""
    if len(result_list) > 0:
        result += build_name + "\n"
        result += "\n".join(["  * %s" % f for f in result_list])
        result += "\n"
    return result

def print_build_memory_usage(report):
    """ Generate result table with memory usage values for build results
    Aggregates (puts together) reports obtained from self.get_memory_summary()

    Positional arguments:
    report - Report generated during build procedure.
    """
    from prettytable import PrettyTable, HEADER
    columns_text = ['name', 'target', 'toolchain']
    columns_int = ['static_ram', 'total_flash']
    table = PrettyTable(columns_text + columns_int, junction_char="|", hrules=HEADER)

    for col in columns_text:
        table.align[col] = 'l'

    for col in columns_int:
        table.align[col] = 'r'

    for target in report:
        for toolchain in report[target]:
            for name in report[target][toolchain]:
                for dlist in report[target][toolchain][name]:
                    for dlistelem in dlist:
                        # Get 'memory_usage' record and build table with
                        # statistics
                        record = dlist[dlistelem]
                        if 'memory_usage' in record and record['memory_usage']:
                            # Note that summary should be in the last record of
                            # 'memory_usage' section. This is why we are
                            # grabbing last "[-1]" record.
                            row = [
                                record['description'],
                                record['target_name'],
                                record['toolchain_name'],
                                record['memory_usage'][-1]['summary'][
                                    'static_ram'],
                                record['memory_usage'][-1]['summary'][
                                    'total_flash'],
                            ]
                            table.add_row(row)

    result = "Memory map breakdown for built projects (values in Bytes):\n"
    result += table.get_string(sortby='name')
    return result

def write_build_report(build_report, template_filename, filename):
    """Write a build report to disk using a template file

    Positional arguments:
    build_report - a report generated by the build system
    template_filename - a file that contains the template for the style of build
                        report
    filename - the location on disk to write the file to
    """
    build_report_failing = []
    build_report_passing = []

    for report in build_report:
        if len(report["failing"]) > 0:
            build_report_failing.append(report)
        else:
            build_report_passing.append(report)

    env = Environment(extensions=['jinja2.ext.with_'])
    env.loader = FileSystemLoader('ci_templates')
    template = env.get_template(template_filename)

    with open(filename, 'w+') as placeholder:
        placeholder.write(template.render(
            failing_builds=build_report_failing,
            passing_builds=build_report_passing))


def merge_build_data(filename, toolchain_report, app_type):
    path_to_file = dirname(abspath(filename))
    try:
        build_data = load(open(filename))
    except (IOError, ValueError):
        build_data = {'builds': []}
    for tgt in toolchain_report.values():
        for tc in tgt.values():
            for project in tc.values():
                for build in project:
                    try:
                        build[0]['bin_fullpath'] = build[0]['bin']
                        build[0]['elf_fullpath'] = build[0]['elf']
                        build[0]['elf'] = relpath(build[0]['elf'], path_to_file)
                        build[0]['bin'] = relpath(build[0]['bin'], path_to_file)
                    except KeyError:
                        pass
                    if 'type' not in build[0]:
                        build[0]['type'] = app_type
                    build_data['builds'].insert(0, build[0])
    dump(build_data, open(filename, "wb"), indent=4, separators=(',', ': '))
