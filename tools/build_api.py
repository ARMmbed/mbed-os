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

import re
import tempfile
import colorama

from copy import copy
from types import ListType
from shutil import rmtree
from os.path import join, exists, basename, abspath, normpath
from os import getcwd, walk, linesep
from time import time
import fnmatch

from tools.utils import mkdir, run_cmd, run_cmd_ext, NotSupportedException, ToolException, InvalidReleaseTargetException
from tools.paths import MBED_TARGETS_PATH, MBED_LIBRARIES, MBED_API, MBED_HAL, MBED_COMMON
from tools.targets import TARGET_NAMES, TARGET_MAP
from tools.libraries import Library
from tools.toolchains import TOOLCHAIN_CLASSES
from jinja2 import FileSystemLoader
from jinja2.environment import Environment
from tools.config import Config

RELEASE_VERSIONS = ['2', '5']

def prep_report(report, target_name, toolchain_name, id_name):
    # Setup report keys
    if not target_name in report:
        report[target_name] = {}

    if not toolchain_name in report[target_name]:
        report[target_name][toolchain_name] = {}

    if not id_name in report[target_name][toolchain_name]:
        report[target_name][toolchain_name][id_name] = []

def prep_properties(properties, target_name, toolchain_name, vendor_label):
    # Setup test properties
    if not target_name in properties:
        properties[target_name] = {}

    if not toolchain_name in properties[target_name]:
        properties[target_name][toolchain_name] = {}

    properties[target_name][toolchain_name]["target"] = target_name
    properties[target_name][toolchain_name]["vendor"] = vendor_label
    properties[target_name][toolchain_name]["toolchain"] = toolchain_name

def create_result(target_name, toolchain_name, id_name, description):
    cur_result = {}
    cur_result["target_name"] = target_name
    cur_result["toolchain_name"] = toolchain_name
    cur_result["id"] = id_name
    cur_result["description"] = description
    cur_result["elapsed_time"] = 0
    cur_result["output"] = ""

    return cur_result

def add_result_to_report(report, result):
    target = result["target_name"]
    toolchain = result["toolchain_name"]
    id_name = result['id']
    result_wrap = { 0: result }
    report[target][toolchain][id_name].append(result_wrap)

def get_config(src_paths, target, toolchain_name):
    # Convert src_paths to a list if needed
    if type(src_paths) != ListType:
        src_paths = [src_paths]

    # Pass all params to the unified prepare_resources()
    toolchain = prepare_toolchain(src_paths, target, toolchain_name)

    # Scan src_path for config files
    resources = toolchain.scan_resources(src_paths[0])
    for path in src_paths[1:]:
        resources.add(toolchain.scan_resources(path))

    # Update configuration files until added features creates no changes
    prev_features = set()
    while True:
        # Update the configuration with any .json files found while scanning
        toolchain.config.add_config_files(resources.json_files)

        # Add features while we find new ones
        features = toolchain.config.get_features()
        if features == prev_features:
            break

        for feature in features:
            if feature in resources.features:
                resources += resources.features[feature]

        prev_features = features
    toolchain.config.validate_config()

    cfg, macros = toolchain.config.get_config_data()
    features = toolchain.config.get_features()
    return cfg, macros, features

def is_official_target(target_name, version):
    """ Returns True, None if a target is part of the official release for the
    given version. Return False, 'reason' if a target is not part of the
    official release for the given version.

    target_name: Name if the target (ex. 'K64F')
    version: The release version string. Should be a string contained within RELEASE_VERSIONS
    """
    
    result = True
    reason = None
    target = TARGET_MAP[target_name]
    
    if hasattr(target, 'release_versions') and version in target.release_versions:
        if version == '2':
            # For version 2, either ARM or uARM toolchain support is required
            required_toolchains = set(['ARM', 'uARM'])
            
            if not len(required_toolchains.intersection(set(target.supported_toolchains))) > 0:
                result = False           
                reason = ("Target '%s' must support " % target.name) + \
                    ("one of the folowing toolchains to be included in the mbed 2.0 ") + \
                    (("official release: %s" + linesep) % ", ".join(required_toolchains)) + \
                    ("Currently it is only configured to support the ") + \
                    ("following toolchains: %s" % ", ".join(target.supported_toolchains))
                    
        elif version == '5':
            # For version 5, ARM, GCC_ARM, and IAR toolchain support is required
            required_toolchains = set(['ARM', 'GCC_ARM', 'IAR'])
            required_toolchains_sorted = list(required_toolchains)
            required_toolchains_sorted.sort()
            supported_toolchains = set(target.supported_toolchains)
            supported_toolchains_sorted = list(supported_toolchains)
            supported_toolchains_sorted.sort()
            
            if not required_toolchains.issubset(supported_toolchains):
                result = False
                reason = ("Target '%s' must support " % target.name) + \
                    ("ALL of the folowing toolchains to be included in the mbed OS 5.0 ") + \
                    (("official release: %s" + linesep) % ", ".join(required_toolchains_sorted)) + \
                    ("Currently it is only configured to support the ") + \
                    ("following toolchains: %s" % ", ".join(supported_toolchains_sorted))

            elif not target.default_build == 'standard':
                result = False
                reason = ("Target '%s' must set the 'default_build' " % target.name) + \
                    ("to 'standard' to be included in the mbed OS 5.0 ") + \
                    ("official release." + linesep) + \
                    ("Currently it is set to '%s'" % target.default_build)

        else:
            result = False
            reason = ("Target '%s' has set an invalid release version of '%s'" % version) + \
                ("Please choose from the following release versions: %s" + ', '.join(RELEASE_VERSIONS))

    else:
        result = False
        if not hasattr(target, 'release_versions'):
            reason = "Target '%s' does not have the 'release_versions' key set" % target.name
        elif not version in target.release_versions:
            reason = "Target '%s' does not contain the version '%s' in its 'release_versions' key" % (target.name, version)
    
    return result, reason

def transform_release_toolchains(toolchains, version):
    """ Given a list of toolchains and a release version, return a list of
    only the supported toolchains for that release

    toolchains: The list of toolchains
    version: The release version string. Should be a string contained within RELEASE_VERSIONS
    """
    toolchains_set = set(toolchains)

    if version == '5':
        return ['ARM', 'GCC_ARM', 'IAR']
    else:
        return toolchains


def get_mbed_official_release(version):
    """ Given a release version string, return a tuple that contains a target
    and the supported toolchains for that release.
    Ex. Given '2', return (('LPC1768', ('ARM', 'GCC_ARM')), ('K64F', ('ARM', 'GCC_ARM')), ...)

    version: The version string. Should be a string contained within RELEASE_VERSIONS
    """

    MBED_OFFICIAL_RELEASE = (
        tuple(
            tuple(
                [
                    TARGET_MAP[target].name,
                    tuple(transform_release_toolchains(TARGET_MAP[target].supported_toolchains, version))
                ]
            ) for target in TARGET_NAMES if (hasattr(TARGET_MAP[target], 'release_versions') and version in TARGET_MAP[target].release_versions)
        )
    )
    
    for target in MBED_OFFICIAL_RELEASE:
        is_official, reason = is_official_target(target[0], version)
        
        if not is_official:
            raise InvalidReleaseTargetException(reason)
            
    return MBED_OFFICIAL_RELEASE


def prepare_toolchain(src_paths, target, toolchain_name,
        macros=None, options=None, clean=False, jobs=1,
        notify=None, silent=False, verbose=False, extra_verbose=False, config=None):
    """ Prepares resource related objects - toolchain, target, config
    src_paths: the paths to source directories
    target: ['LPC1768', 'LPC11U24', 'LPC2368']
    toolchain_name: ['ARM', 'uARM', 'GCC_ARM', 'GCC_CR']
    clean: Rebuild everything if True
    notify: Notify function for logs
    verbose: Write the actual tools command lines if True
    """

    # We need to remove all paths which are repeated to avoid
    # multiple compilations and linking with the same objects
    src_paths = [src_paths[0]] + list(set(src_paths[1:]))

    # If the configuration object was not yet created, create it now
    config = config or Config(target, src_paths)

    # If the 'target' argument is a string, convert it to a target instance
    if isinstance(target, basestring):
        try:
            target = TARGET_MAP[target]
        except KeyError:
            raise KeyError("Target '%s' not found" % target)

    # Toolchain instance
    try:
        toolchain = TOOLCHAIN_CLASSES[toolchain_name](target, options, notify, macros, silent, extra_verbose=extra_verbose)
    except KeyError as e:
        raise KeyError("Toolchain %s not supported" % toolchain_name)

    toolchain.config = config
    toolchain.jobs = jobs
    toolchain.build_all = clean
    toolchain.VERBOSE = verbose

    return toolchain

def scan_resources(src_paths, toolchain, dependencies_paths=None, inc_dirs=None):
    """ Scan resources using initialized toolcain
    src_paths: the paths to source directories
    toolchain: valid toolchain object
    dependencies_paths: dependency paths that we should scan for include dirs
    inc_dirs: additional include directories which should be added to thescanner resources
    """

    # Scan src_path
    resources = toolchain.scan_resources(src_paths[0])
    for path in src_paths[1:]:
        resources.add(toolchain.scan_resources(path))

    # Scan dependency paths for include dirs
    if dependencies_paths is not None:
        for path in dependencies_paths:
            lib_resources = toolchain.scan_resources(path)
            resources.inc_dirs.extend(lib_resources.inc_dirs)

    # Add additional include directories if passed
    if inc_dirs:
        if type(inc_dirs) == ListType:
            resources.inc_dirs.extend(inc_dirs)
        else:
            resources.inc_dirs.append(inc_dirs)

    # Load resources into the config system which might expand/modify resources based on config data
    resources = toolchain.config.load_resources(resources)

    # Set the toolchain's configuration data
    toolchain.set_config_data(toolchain.config.get_config_data())

    return resources

def build_project(src_paths, build_path, target, toolchain_name,
        libraries_paths=None, options=None, linker_script=None,
        clean=False, notify=None, verbose=False, name=None, macros=None, inc_dirs=None,
        jobs=1, silent=False, report=None, properties=None, project_id=None, project_description=None,
        extra_verbose=False, config=None):
    """ This function builds project. Project can be for example one test / UT
    """

    # Convert src_path to a list if needed
    if type(src_paths) != ListType:
        src_paths = [src_paths]
    # Extend src_paths wiht libraries_paths
    if libraries_paths is not None:
        src_paths.extend(libraries_paths)

    # Build Directory
    if clean:
        if exists(build_path):
            rmtree(build_path)
    mkdir(build_path)

    # Pass all params to the unified prepare_toolchain()
    toolchain = prepare_toolchain(src_paths, target, toolchain_name,
        macros=macros, options=options, clean=clean, jobs=jobs,
        notify=notify, silent=silent, verbose=verbose, extra_verbose=extra_verbose, config=config)

    # The first path will give the name to the library
    if name is None:
        name = basename(normpath(abspath(src_paths[0])))
    toolchain.info("Building project %s (%s, %s)" % (name, toolchain.target.name, toolchain_name))

    # Initialize reporting
    if report != None:
        start = time()
        # If project_id is specified, use that over the default name
        id_name = project_id.upper() if project_id else name.upper()
        description = project_description if project_description else name
        vendor_label = toolchain.target.extra_labels[0]
        prep_report(report, toolchain.target.name, toolchain_name, id_name)
        cur_result = create_result(toolchain.target.name, toolchain_name, id_name, description)
        if properties != None:
            prep_properties(properties, toolchain.target.name, toolchain_name, vendor_label)

    try:
        # Call unified scan_resources
        resources = scan_resources(src_paths, toolchain, inc_dirs=inc_dirs)

        # Change linker script if specified
        if linker_script is not None:
            resources.linker_script = linker_script

        # Compile Sources
        objects = toolchain.compile_sources(resources, build_path, resources.inc_dirs)
        resources.objects.extend(objects)

        # Link Program
        res, _ = toolchain.link_program(resources, build_path, name)

        if report != None:
            end = time()
            cur_result["elapsed_time"] = end - start
            cur_result["output"] = toolchain.get_output()
            cur_result["result"] = "OK"
            cur_result["memory_usage"] = toolchain.map_outputs

            add_result_to_report(report, cur_result)

        return res

    except Exception, e:
        if report != None:
            end = time()

            if isinstance(e, NotSupportedException):
                cur_result["result"] = "NOT_SUPPORTED"
            else:
                cur_result["result"] = "FAIL"

            cur_result["elapsed_time"] = end - start

            toolchain_output = toolchain.get_output()
            if toolchain_output:
                cur_result["output"] += toolchain_output

            add_result_to_report(report, cur_result)

        # Let Exception propagate
        raise

def build_library(src_paths, build_path, target, toolchain_name,
         dependencies_paths=None, options=None, name=None, clean=False, archive=True,
         notify=None, verbose=False, macros=None, inc_dirs=None,
         jobs=1, silent=False, report=None, properties=None, extra_verbose=False,
         project_id=None):
    """ Prepares resource related objects - toolchain, target, config
    src_paths: the paths to source directories
    build_path: the path of the build directory
    target: ['LPC1768', 'LPC11U24', 'LPC2368']
    toolchain_name: ['ARM', 'uARM', 'GCC_ARM', 'GCC_CR']
    clean: Rebuild everything if True
    notify: Notify function for logs
    verbose: Write the actual tools command lines if True
    inc_dirs: additional include directories which should be included in build
    """

    # Convert src_path to a list if needed
    if type(src_paths) != ListType:
        src_paths = [src_paths]

    # Build path
    if archive:
        # Use temp path when building archive
        tmp_path = join(build_path, '.temp')
        mkdir(tmp_path)
    else:
        tmp_path = build_path

    # Clean the build directory
    if clean:
        if exists(tmp_path):
            rmtree(tmp_path)
    mkdir(tmp_path)

    # Pass all params to the unified prepare_toolchain()
    toolchain = prepare_toolchain(src_paths, target, toolchain_name,
        macros=macros, options=options, clean=clean, jobs=jobs,
        notify=notify, silent=silent, verbose=verbose, extra_verbose=extra_verbose)

    # The first path will give the name to the library
    if name is None:
        name = basename(normpath(abspath(src_paths[0])))
    toolchain.info("Building library %s (%s, %s)" % (name, toolchain.target.name, toolchain_name))

    # Initialize reporting
    if report != None:
        start = time()
        # If project_id is specified, use that over the default name
        id_name = project_id.upper() if project_id else name.upper()
        description = name
        vendor_label = toolchain.target.extra_labels[0]
        prep_report(report, toolchain.target.name, toolchain_name, id_name)
        cur_result = create_result(toolchain.target.name, toolchain_name, id_name, description)
        if properties != None:
            prep_properties(properties, toolchain.target.name, toolchain_name, vendor_label)

    for src_path in src_paths:
        if not exists(src_path):
            error_msg = "The library source folder does not exist: %s", src_path
            if report != None:
                cur_result["output"] = error_msg
                cur_result["result"] = "FAIL"
                add_result_to_report(report, cur_result)
            raise Exception(error_msg)

    try:
        # Call unified scan_resources
        resources = scan_resources(src_paths, toolchain, dependencies_paths=dependencies_paths, inc_dirs=inc_dirs)


        # Copy headers, objects and static libraries - all files needed for static lib
        toolchain.copy_files(resources.headers, build_path, resources=resources)
        toolchain.copy_files(resources.objects, build_path, resources=resources)
        toolchain.copy_files(resources.libraries, build_path, resources=resources)
        if resources.linker_script:
            toolchain.copy_files(resources.linker_script, build_path, resources=resources)

        if resources.hex_files:
            toolchain.copy_files(resources.hex_files, build_path, resources=resources)

        # Compile Sources
        objects = toolchain.compile_sources(resources, abspath(tmp_path), resources.inc_dirs)
        resources.objects.extend(objects)

        if archive:
            toolchain.build_library(objects, build_path, name)

        if report != None:
            end = time()
            cur_result["elapsed_time"] = end - start
            cur_result["output"] = toolchain.get_output()
            cur_result["result"] = "OK"

            add_result_to_report(report, cur_result)
        return True

    except Exception, e:
        if report != None:
            end = time()

            if isinstance(e, ToolException):
                cur_result["result"] = "FAIL"
            elif isinstance(e, NotSupportedException):
                cur_result["result"] = "NOT_SUPPORTED"

            cur_result["elapsed_time"] = end - start

            toolchain_output = toolchain.get_output()
            if toolchain_output:
                cur_result["output"] += toolchain_output

            add_result_to_report(report, cur_result)

        # Let Exception propagate
        raise e

######################
### Legacy methods ###
######################

def build_lib(lib_id, target, toolchain_name, options=None, verbose=False, clean=False, macros=None, notify=None, jobs=1, silent=False, report=None, properties=None, extra_verbose=False):
    """ Legacy method for building mbed libraries
        Function builds library in proper directory using all dependencies and macros defined by user.
    """
    lib = Library(lib_id)
    if not lib.is_supported(target, toolchain_name):
        print 'Library "%s" is not yet supported on target %s with toolchain %s' % (lib_id, target.name, toolchain)
        return False

    # We need to combine macros from parameter list with macros from library definition
    MACROS = lib.macros if lib.macros else []
    if macros:
        macros.extend(MACROS)
    else:
        macros = MACROS

    src_paths = lib.source_dir
    build_path = lib.build_dir
    dependencies_paths = lib.dependencies
    inc_dirs = lib.inc_dirs
    inc_dirs_ext = lib.inc_dirs_ext

    """ src_path: the path of the source directory
    build_path: the path of the build directory
    target: ['LPC1768', 'LPC11U24', 'LPC2368']
    toolchain: ['ARM', 'uARM', 'GCC_ARM', 'GCC_CR']
    library_paths: List of paths to additional libraries
    clean: Rebuild everything if True
    notify: Notify function for logs
    verbose: Write the actual tools command lines if True
    inc_dirs: additional include directories which should be included in build
    inc_dirs_ext: additional include directories which should be copied to library directory
    """
    if type(src_paths) != ListType:
        src_paths = [src_paths]

    # The first path will give the name to the library
    name = basename(src_paths[0])

    if report != None:
        start = time()
        id_name = name.upper()
        description = name
        vendor_label = target.extra_labels[0]
        cur_result = None
        prep_report(report, target.name, toolchain_name, id_name)
        cur_result = create_result(target.name, toolchain_name, id_name, description)

        if properties != None:
            prep_properties(properties, target.name, toolchain_name, vendor_label)

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
        toolchain = TOOLCHAIN_CLASSES[toolchain_name](target, options, macros=macros, notify=notify, silent=silent, extra_verbose=extra_verbose)
        toolchain.VERBOSE = verbose
        toolchain.jobs = jobs
        toolchain.build_all = clean

        toolchain.info("Building library %s (%s, %s)" % (name.upper(), target.name, toolchain_name))

        # Scan Resources
        resources = []
        for src_path in src_paths:
            resources.append(toolchain.scan_resources(src_path))

        # Add extra include directories / files which are required by library
        # This files usually are not in the same directory as source files so
        # previous scan will not include them
        if inc_dirs_ext is not None:
            for inc_ext in inc_dirs_ext:
                resources.append(toolchain.scan_resources(inc_ext))

        # Dependencies Include Paths
        dependencies_include_dir = []
        if dependencies_paths is not None:
            for path in dependencies_paths:
                lib_resources = toolchain.scan_resources(path)
                dependencies_include_dir.extend(lib_resources.inc_dirs)

        if inc_dirs:
            dependencies_include_dir.extend(inc_dirs)

        # Create the desired build directory structure
        bin_path = join(build_path, toolchain.obj_path)
        mkdir(bin_path)
        tmp_path = join(build_path, '.temp', toolchain.obj_path)
        mkdir(tmp_path)

        # Copy Headers
        for resource in resources:
            toolchain.copy_files(resource.headers, build_path, resources=resource)

        dependencies_include_dir.extend(toolchain.scan_resources(build_path).inc_dirs)

        # Compile Sources
        objects = []
        for resource in resources:
            objects.extend(toolchain.compile_sources(resource, tmp_path, dependencies_include_dir))

        needed_update = toolchain.build_library(objects, bin_path, name)

        if report != None and needed_update:
            end = time()
            cur_result["elapsed_time"] = end - start
            cur_result["output"] = toolchain.get_output()
            cur_result["result"] = "OK"

            add_result_to_report(report, cur_result)
        return True

    except Exception, e:
        if report != None:
            end = time()
            cur_result["result"] = "FAIL"
            cur_result["elapsed_time"] = end - start

            toolchain_output = toolchain.get_output()
            if toolchain_output:
                cur_result["output"] += toolchain_output

            add_result_to_report(report, cur_result)

        # Let Exception propagate
        raise e

# We do have unique legacy conventions about how we build and package the mbed library
def build_mbed_libs(target, toolchain_name, options=None, verbose=False, clean=False, macros=None, notify=None, jobs=1, silent=False, report=None, properties=None, extra_verbose=False):
    """ Function returns True is library was built and false if building was skipped """

    if report != None:
        start = time()
        id_name = "MBED"
        description = "mbed SDK"
        vendor_label = target.extra_labels[0]
        cur_result = None
        prep_report(report, target.name, toolchain_name, id_name)
        cur_result = create_result(target.name, toolchain_name, id_name, description)

        if properties != None:
            prep_properties(properties, target.name, toolchain_name, vendor_label)

    # Check toolchain support
    if toolchain_name not in target.supported_toolchains:
        supported_toolchains_text = ", ".join(target.supported_toolchains)
        print '%s target is not yet supported by toolchain %s' % (target.name, toolchain_name)
        print '%s target supports %s toolchain%s' % (target.name, supported_toolchains_text, 's' if len(target.supported_toolchains) > 1 else '')

        if report != None:
            cur_result["result"] = "SKIP"
            add_result_to_report(report, cur_result)

        return False

    try:
        # Toolchain
        toolchain = TOOLCHAIN_CLASSES[toolchain_name](target, options, macros=macros, notify=notify, silent=silent, extra_verbose=extra_verbose)
        toolchain.VERBOSE = verbose
        toolchain.jobs = jobs
        toolchain.build_all = clean

        # Source and Build Paths
        BUILD_TARGET = join(MBED_LIBRARIES, "TARGET_" + target.name)
        BUILD_TOOLCHAIN = join(BUILD_TARGET, "TOOLCHAIN_" + toolchain.name)
        mkdir(BUILD_TOOLCHAIN)

        TMP_PATH = join(MBED_LIBRARIES, '.temp', toolchain.obj_path)
        mkdir(TMP_PATH)

        # CMSIS
        toolchain.info("Building library %s (%s, %s)"% ('CMSIS', target.name, toolchain_name))
        cmsis_src = join(MBED_TARGETS_PATH, "cmsis")
        resources = toolchain.scan_resources(cmsis_src)

        toolchain.copy_files(resources.headers, BUILD_TARGET)
        toolchain.copy_files(resources.linker_script, BUILD_TOOLCHAIN)
        toolchain.copy_files(resources.bin_files, BUILD_TOOLCHAIN)

        objects = toolchain.compile_sources(resources, TMP_PATH)
        toolchain.copy_files(objects, BUILD_TOOLCHAIN)

        # mbed
        toolchain.info("Building library %s (%s, %s)" % ('MBED', target.name, toolchain_name))

        # Common Headers
        toolchain.copy_files(toolchain.scan_resources(MBED_API).headers, MBED_LIBRARIES)
        toolchain.copy_files(toolchain.scan_resources(MBED_HAL).headers, MBED_LIBRARIES)

        # Target specific sources
        HAL_SRC = join(MBED_TARGETS_PATH, "hal")
        hal_implementation = toolchain.scan_resources(HAL_SRC)
        toolchain.copy_files(hal_implementation.headers + hal_implementation.hex_files + hal_implementation.libraries, BUILD_TARGET, resources=hal_implementation)
        incdirs = toolchain.scan_resources(BUILD_TARGET).inc_dirs
        objects = toolchain.compile_sources(hal_implementation, TMP_PATH, [MBED_LIBRARIES] + incdirs)

        # Common Sources
        mbed_resources = toolchain.scan_resources(MBED_COMMON)
        objects += toolchain.compile_sources(mbed_resources, TMP_PATH, [MBED_LIBRARIES] + incdirs)

        # A number of compiled files need to be copied as objects as opposed to
        # being part of the mbed library, for reasons that have to do with the way
        # the linker search for symbols in archives. These are:
        #   - retarget.o: to make sure that the C standard lib symbols get overridden
        #   - board.o: mbed_die is weak
        #   - mbed_overrides.o: this contains platform overrides of various weak SDK functions
        separate_names, separate_objects = ['retarget.o', 'board.o', 'mbed_overrides.o'], []

        for o in objects:
            for name in separate_names:
                if o.endswith(name):
                    separate_objects.append(o)

        for o in separate_objects:
            objects.remove(o)

        toolchain.build_library(objects, BUILD_TOOLCHAIN, "mbed")

        for o in separate_objects:
            toolchain.copy_files(o, BUILD_TOOLCHAIN)

        if report != None:
            end = time()
            cur_result["elapsed_time"] = end - start
            cur_result["output"] = toolchain.get_output()
            cur_result["result"] = "OK"

            add_result_to_report(report, cur_result)

        return True

    except Exception, e:
        if report != None:
            end = time()
            cur_result["result"] = "FAIL"
            cur_result["elapsed_time"] = end - start

            toolchain_output = toolchain.get_output()
            if toolchain_output:
                cur_result["output"] += toolchain_output

            cur_result["output"] += str(e)

            add_result_to_report(report, cur_result)

        # Let Exception propagate
        raise e


def get_unique_supported_toolchains(release_targets=None):
    """ Get list of all unique toolchains supported by targets
    If release_targets is not specified, then it queries all known targets
    release_targets: tuple structure returned from get_mbed_official_release()
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


def mcu_toolchain_matrix(verbose_html=False, platform_filter=None, release_version='5'):
    """  Shows target map using prettytable """
    from prettytable import PrettyTable # Only use it in this function so building works without extra modules

    if isinstance(release_version, basestring):
        # Force release_version to lowercase if it is a string
        release_version = release_version.lower()
    else:
        # Otherwise default to printing all known targets and toolchains
        release_version = 'all'


    version_release_targets = {}
    version_release_target_names = {}

    for version in RELEASE_VERSIONS:
        version_release_targets[version] = get_mbed_official_release(version)
        version_release_target_names[version] = [x[0] for x in version_release_targets[version]]

    if release_version in RELEASE_VERSIONS:
        release_targets = version_release_targets[release_version]
    else:
        release_targets = None

    unique_supported_toolchains = get_unique_supported_toolchains(release_targets)
    prepend_columns = ["Target"] + ["mbed OS %s" % x for x in RELEASE_VERSIONS]

    # All tests status table print
    columns = prepend_columns + unique_supported_toolchains
    pt = PrettyTable(columns)
    # Align table
    for col in columns:
        pt.align[col] = "c"
    pt.align["Target"] = "l"

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
            if unique_toolchain in TARGET_MAP[target].supported_toolchains:
                text = "Supported"
                perm_counter += 1
            else:
                text = "-"

            row.append(text)
        pt.add_row(row)

    result = pt.get_html_string() if verbose_html else pt.get_string()
    result += "\n"
    result += "Supported targets: %d\n"% (target_counter)
    if target_counter == 1:
        result += "Supported toolchains: %d"% (perm_counter)
    return result


def get_target_supported_toolchains(target):
    """ Returns target supported toolchains list """
    return TARGET_MAP[target].supported_toolchains if target in TARGET_MAP else None


def static_analysis_scan(target, toolchain_name, CPPCHECK_CMD, CPPCHECK_MSG_FORMAT, options=None, verbose=False, clean=False, macros=None, notify=None, jobs=1, extra_verbose=False):
    # Toolchain
    toolchain = TOOLCHAIN_CLASSES[toolchain_name](target, options, macros=macros, notify=notify, extra_verbose=extra_verbose)
    toolchain.VERBOSE = verbose
    toolchain.jobs = jobs
    toolchain.build_all = clean

    # Source and Build Paths
    BUILD_TARGET = join(MBED_LIBRARIES, "TARGET_" + target.name)
    BUILD_TOOLCHAIN = join(BUILD_TARGET, "TOOLCHAIN_" + toolchain.name)
    mkdir(BUILD_TOOLCHAIN)

    TMP_PATH = join(MBED_LIBRARIES, '.temp', toolchain.obj_path)
    mkdir(TMP_PATH)

    # CMSIS
    toolchain.info("Static analysis for %s (%s, %s)" % ('CMSIS', target.name, toolchain_name))
    cmsis_src = join(MBED_TARGETS_PATH, "cmsis")
    resources = toolchain.scan_resources(cmsis_src)

    # Copy files before analysis
    toolchain.copy_files(resources.headers, BUILD_TARGET)
    toolchain.copy_files(resources.linker_script, BUILD_TOOLCHAIN)

    # Gather include paths, c, cpp sources and macros to transfer to cppcheck command line
    includes = ["-I%s"% i for i in resources.inc_dirs]
    includes.append("-I%s"% str(BUILD_TARGET))
    c_sources = " ".join(resources.c_sources)
    cpp_sources = " ".join(resources.cpp_sources)
    macros = ["-D%s"% s for s in toolchain.get_symbols() + toolchain.macros]

    includes = map(str.strip, includes)
    macros = map(str.strip, macros)

    check_cmd = CPPCHECK_CMD
    check_cmd += CPPCHECK_MSG_FORMAT
    check_cmd += includes
    check_cmd += macros

    # We need to pass some params via file to avoid "command line too long in some OSs"
    tmp_file = tempfile.NamedTemporaryFile(delete=False)
    tmp_file.writelines(line + '\n' for line in c_sources.split())
    tmp_file.writelines(line + '\n' for line in cpp_sources.split())
    tmp_file.close()
    check_cmd += ["--file-list=%s"% tmp_file.name]

    _stdout, _stderr, _rc = run_cmd(check_cmd)
    if verbose:
        print _stdout
    print _stderr

    # =========================================================================

    # MBED
    toolchain.info("Static analysis for %s (%s, %s)" % ('MBED', target.name, toolchain_name))

    # Common Headers
    toolchain.copy_files(toolchain.scan_resources(MBED_API).headers, MBED_LIBRARIES)
    toolchain.copy_files(toolchain.scan_resources(MBED_HAL).headers, MBED_LIBRARIES)

    # Target specific sources
    HAL_SRC = join(MBED_TARGETS_PATH, "hal")
    hal_implementation = toolchain.scan_resources(HAL_SRC)

    # Copy files before analysis
    toolchain.copy_files(hal_implementation.headers + hal_implementation.hex_files, BUILD_TARGET, resources=hal_implementation)
    incdirs = toolchain.scan_resources(BUILD_TARGET)

    target_includes = ["-I%s" % i for i in incdirs.inc_dirs]
    target_includes.append("-I%s"% str(BUILD_TARGET))
    target_includes.append("-I%s"% str(HAL_SRC))
    target_c_sources = " ".join(incdirs.c_sources)
    target_cpp_sources = " ".join(incdirs.cpp_sources)
    target_macros = ["-D%s"% s for s in toolchain.get_symbols() + toolchain.macros]

    # Common Sources
    mbed_resources = toolchain.scan_resources(MBED_COMMON)

    # Gather include paths, c, cpp sources and macros to transfer to cppcheck command line
    mbed_includes = ["-I%s" % i for i in mbed_resources.inc_dirs]
    mbed_includes.append("-I%s"% str(BUILD_TARGET))
    mbed_includes.append("-I%s"% str(MBED_COMMON))
    mbed_includes.append("-I%s"% str(MBED_API))
    mbed_includes.append("-I%s"% str(MBED_HAL))
    mbed_c_sources = " ".join(mbed_resources.c_sources)
    mbed_cpp_sources = " ".join(mbed_resources.cpp_sources)

    target_includes = map(str.strip, target_includes)
    mbed_includes = map(str.strip, mbed_includes)
    target_macros = map(str.strip, target_macros)

    check_cmd = CPPCHECK_CMD
    check_cmd += CPPCHECK_MSG_FORMAT
    check_cmd += target_includes
    check_cmd += mbed_includes
    check_cmd += target_macros

    # We need to pass some parames via file to avoid "command line too long in some OSs"
    tmp_file = tempfile.NamedTemporaryFile(delete=False)
    tmp_file.writelines(line + '\n' for line in target_c_sources.split())
    tmp_file.writelines(line + '\n' for line in target_cpp_sources.split())
    tmp_file.writelines(line + '\n' for line in mbed_c_sources.split())
    tmp_file.writelines(line + '\n' for line in mbed_cpp_sources.split())
    tmp_file.close()
    check_cmd += ["--file-list=%s"% tmp_file.name]

    _stdout, _stderr, _rc = run_cmd_ext(check_cmd)
    if verbose:
        print _stdout
    print _stderr


def static_analysis_scan_lib(lib_id, target, toolchain, cppcheck_cmd, cppcheck_msg_format,
                             options=None, verbose=False, clean=False, macros=None, notify=None, jobs=1, extra_verbose=False):
    lib = Library(lib_id)
    if lib.is_supported(target, toolchain):
        static_analysis_scan_library(lib.source_dir, lib.build_dir, target, toolchain, cppcheck_cmd, cppcheck_msg_format,
                      lib.dependencies, options,
                      verbose=verbose, clean=clean, macros=macros, notify=notify, jobs=jobs, extra_verbose=extra_verbose)
    else:
        print 'Library "%s" is not yet supported on target %s with toolchain %s'% (lib_id, target.name, toolchain)


def static_analysis_scan_library(src_paths, build_path, target, toolchain_name, cppcheck_cmd, cppcheck_msg_format,
         dependencies_paths=None, options=None, name=None, clean=False,
         notify=None, verbose=False, macros=None, jobs=1, extra_verbose=False):
    """ Function scans library (or just some set of sources/headers) for staticly detectable defects """
    if type(src_paths) != ListType:
        src_paths = [src_paths]

    for src_path in src_paths:
        if not exists(src_path):
            raise Exception("The library source folder does not exist: %s", src_path)

    # Toolchain instance
    toolchain = TOOLCHAIN_CLASSES[toolchain_name](target, options, macros=macros, notify=notify, extra_verbose=extra_verbose)
    toolchain.VERBOSE = verbose
    toolchain.jobs = jobs

    # The first path will give the name to the library
    name = basename(src_paths[0])
    toolchain.info("Static analysis for library %s (%s, %s)" % (name.upper(), target.name, toolchain_name))

    # Scan Resources
    resources = []
    for src_path in src_paths:
        resources.append(toolchain.scan_resources(src_path))

    # Dependencies Include Paths
    dependencies_include_dir = []
    if dependencies_paths is not None:
        for path in dependencies_paths:
            lib_resources = toolchain.scan_resources(path)
            dependencies_include_dir.extend(lib_resources.inc_dirs)

    # Create the desired build directory structure
    bin_path = join(build_path, toolchain.obj_path)
    mkdir(bin_path)
    tmp_path = join(build_path, '.temp', toolchain.obj_path)
    mkdir(tmp_path)

    # Gather include paths, c, cpp sources and macros to transfer to cppcheck command line
    includes = ["-I%s" % i for i in dependencies_include_dir + src_paths]
    c_sources = " "
    cpp_sources = " "
    macros = ['-D%s' % s for s in toolchain.get_symbols() + toolchain.macros]

    # Copy Headers
    for resource in resources:
        toolchain.copy_files(resource.headers, build_path, resources=resource)
        includes += ["-I%s" % i for i in resource.inc_dirs]
        c_sources += " ".join(resource.c_sources) + " "
        cpp_sources += " ".join(resource.cpp_sources) + " "

    dependencies_include_dir.extend(toolchain.scan_resources(build_path).inc_dirs)

    includes = map(str.strip, includes)
    macros = map(str.strip, macros)

    check_cmd = cppcheck_cmd
    check_cmd += cppcheck_msg_format
    check_cmd += includes
    check_cmd += macros

    # We need to pass some parameters via file to avoid "command line too long in some OSs"
    # Temporary file is created to store e.g. cppcheck list of files for command line
    tmp_file = tempfile.NamedTemporaryFile(delete=False)
    tmp_file.writelines(line + '\n' for line in c_sources.split())
    tmp_file.writelines(line + '\n' for line in cpp_sources.split())
    tmp_file.close()
    check_cmd += ["--file-list=%s"% tmp_file.name]

    # This will allow us to grab result from both stdio and stderr outputs (so we can show them)
    # We assume static code analysis tool is outputting defects on STDERR
    _stdout, _stderr, _rc = run_cmd_ext(check_cmd)
    if verbose:
        print _stdout
    print _stderr


def print_build_results(result_list, build_name):
    """ Generate result string for build results """
    result = ""
    if len(result_list) > 0:
        result += build_name + "\n"
        result += "\n".join(["  * %s" % f for f in result_list])
        result += "\n"
    return result

def print_build_memory_usage_results(report):
    """ Generate result table with memory usage values for build results
        Agregates (puts together) reports obtained from self.get_memory_summary()
        @param report Report generated during build procedure. See
    """
    from prettytable import PrettyTable
    columns_text = ['name', 'target', 'toolchain']
    columns_int = ['static_ram', 'stack', 'heap', 'total_ram', 'total_flash']
    table = PrettyTable(columns_text + columns_int)

    for col in columns_text:
        table.align[col] = 'l'

    for col in columns_int:
        table.align[col] = 'r'

    for target in report:
        for toolchain in report[target]:
            for name in report[target][toolchain]:
                for dlist in report[target][toolchain][name]:
                    for dlistelem in dlist:
                        # Get 'memory_usage' record and build table with statistics
                        record = dlist[dlistelem]
                        if 'memory_usage' in record and record['memory_usage']:
                            # Note that summary should be in the last record of
                            # 'memory_usage' section. This is why we are grabbing
                            # last "[-1]" record.
                            row = [
                                record['description'],
                                record['target_name'],
                                record['toolchain_name'],
                                record['memory_usage'][-1]['summary']['static_ram'],
                                record['memory_usage'][-1]['summary']['stack'],
                                record['memory_usage'][-1]['summary']['heap'],
                                record['memory_usage'][-1]['summary']['total_ram'],
                                record['memory_usage'][-1]['summary']['total_flash'],
                            ]
                            table.add_row(row)

    result = "Memory map breakdown for built projects (values in Bytes):\n"
    result += table.get_string(sortby='name')
    return result

def write_build_report(build_report, template_filename, filename):
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

    with open(filename, 'w+') as f:
        f.write(template.render(failing_builds=build_report_failing, passing_builds=build_report_passing))
