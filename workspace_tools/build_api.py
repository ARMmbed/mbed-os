from os.path import join, exists, basename
from shutil import copyfile, rmtree
from copy import copy
import json
from types import ListType

from workspace_tools.utils import mkdir
from workspace_tools.toolchains import TOOLCHAIN_CLASSES, Resources

"""
src_path: the path of the source directory
build_path: the path of the build directory
target: ['LPC1768', 'LPC11U24', 'LPC2368']
toolchain: ['ARM', 'uARM', 'GCC_ARM', 'GCC_CS', 'GCC_CR']
library_paths: List of paths to additional libraries
clean: Rebuild everything if True
notify: Notify function for logs
verbose: Write the actual tools command lines if True
"""
def build_library(src_paths, build_path, target='LPC1768', toolchain_name='ARM',
        libraries_paths=None, name=None, clean=False, notify=None, verbose=False):
    if type(src_paths) != ListType: src_paths = [src_paths]
    
    for src_path in src_paths:
        if not exists(src_path):
            raise Exception("The library source folder does not exist: %s", src_path)
    
    # Toolchain instance
    toolchain = TOOLCHAIN_CLASSES[toolchain_name](target, notify)
    toolchain.VERBOSE = verbose
    toolchain.build_all = clean
    
    # Options
    # By convention the first path should contain the build configuration
    main_src = src_paths[0]
    options_file = join(main_src, 'options.json')
    if exists(options_file):
        try:
            options = json.load(open(options_file))
        except Exception, e:
            raise Exception("Error parsing option file: %s\n\t%s" % (options_file, e))
    else:
        options = {}
    toolchain.set_options(options)
    
    # Library name can be set by: function parameter, option file, or directory name (in that order)
    if name is None:
        if 'lib_name' in options:
            name = options['lib_name']
        else:
            name = basename(main_src)
    
    toolchain.info("\n>>> BUILD LIBRARY %s (%s, %s)" % (name.upper(), target, toolchain_name))
    
    # Scan Resources
    resources = []
    for src_path in src_paths:
        resources.append(toolchain.scan_resources(src_path))
    
    # Just for the include paths
    lib_resources = Resources()
    if libraries_paths is not None:
        for path in libraries_paths:
            lib_resources.add(toolchain.scan_resources(path))
    
    # Create the desired build directory structure
    bin_path = join(build_path, toolchain.obj_path)
    mkdir(bin_path)
    
    tmp_path = join(build_path, '.temp', toolchain.obj_path)
    mkdir(tmp_path)
    
    # Copy Files
    for resource in resources:
        files_to_be_copied = resource.headers
        if resource.linker_script is not None:
            # Linker script
            files_to_be_copied.append(resource.linker_script)
        toolchain.copy_files(resource.base_path, build_path, files_to_be_copied)
    
    # Compile sources
    objects = []
    for resource in resources:
        objects.extend(toolchain.compile_sources(resource, tmp_path, lib_resources.inc_dirs))
    
    # Same objects need to remain stand alone
    if 'objects' in options:
        for object in copy(objects):
            filename = basename(object)
            if filename in options['objects']:
                objects.remove(object)
                target = join(bin_path, filename)
                if toolchain.need_update(target, [object]):
                    toolchain.info("Copy: %s" % filename)
                    copyfile(object, target)
    
    # Archive objects in a library
    if objects:
        toolchain.build_library(objects, bin_path, name)


def build_project(src_path, build_path, target='LPC1768', toolchain_name='ARM',
        libraries_paths=None, clean=False, notify=None, verbose=False, name=None):
    # Toolchain instance
    toolchain = TOOLCHAIN_CLASSES[toolchain_name](target, notify)
    toolchain.VERBOSE = verbose
    toolchain.build_all = clean
    
    if name is None:
        name = basename(src_path)
    toolchain.info("\n>>> BUILD PROJECT: %s (%s, %s)" % (name.upper(), target, toolchain_name))
    
    # Scan src_path and libraries_paths for resources
    resources = toolchain.scan_resources(src_path)
    if libraries_paths is not None:
        for path in libraries_paths:
            resources.add(toolchain.scan_resources(path))
    
    # Build Directory
    if clean:
        if exists(build_path):
            rmtree(build_path)
    mkdir(build_path)
    
    # Build Program
    return toolchain.build_program(resources, build_path, name)


# Core mbed libraries 
from workspace_tools.libraries import Library


def build_lib(lib_id, target, toolchain, verbose=False):
    lib = Library(lib_id)
    if lib.is_supported(target, toolchain):
        build_library(lib.source_dir, lib.build_dir, target, toolchain, lib.dependencies, lib.name, verbose=verbose)
    else:
        print '\n\nLibrary "%s" is not yet supported on target %s with toolchain %s' % (lib_id, target, toolchain)

CHIP_VENDOR = {
    "LPC2368" : "nxp",
    "LPC1768" : "nxp",
    "LPC11U24": "nxp",
    "LPC812"  : "nxp",
    
    "KL25Z"   : "freescale",
}

def build_mbed_libs(target, toolchain, verbose=False):
    vendor = CHIP_VENDOR[target]
    
    for lib_name in ["%s_cmsis"%vendor, "%s_mbed"%vendor]:
        build_lib(lib_name, target, toolchain, verbose=verbose)
