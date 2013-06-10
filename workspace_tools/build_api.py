from os.path import join, exists, basename
from shutil import rmtree
from types import ListType

from workspace_tools.utils import mkdir
from workspace_tools.toolchains import TOOLCHAIN_CLASSES, Resources
from workspace_tools.paths import VENDOR_PATH, MBED_LIBRARIES, MBED_API, MBED_HAL, MBED_COMMON
from workspace_tools.libraries import Library


def build_project(src_path, build_path, target, toolchain_name,
        libraries_paths=None, linker_script=None,
        clean=False, notify=None, verbose=False, name=None):
    # Toolchain instance
    toolchain = TOOLCHAIN_CLASSES[toolchain_name](target, notify)
    toolchain.VERBOSE = verbose
    toolchain.build_all = clean
    
    if name is None:
        name = basename(src_path)
    toolchain.info("\n>>> BUILD PROJECT: %s (%s, %s)" % (name.upper(), target.name, toolchain_name))
    
    # Scan src_path and libraries_paths for resources
    resources = toolchain.scan_resources(src_path)
    if libraries_paths is not None:
        for path in libraries_paths:
            resources.add(toolchain.scan_resources(path))
    
    if linker_script is not None:
        resources.linker_script = linker_script
    
    # Build Directory
    if clean:
        if exists(build_path):
            rmtree(build_path)
    mkdir(build_path)
    
    # Build Program
    return toolchain.build_program(resources, build_path, name)


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
def build_library(src_paths, build_path, target, toolchain_name,
        dependencies_paths=None, name=None, clean=False, notify=None, verbose=False):
    if type(src_paths) != ListType: src_paths = [src_paths]
    
    for src_path in src_paths:
        if not exists(src_path):
            raise Exception("The library source folder does not exist: %s", src_path)
    
    # Toolchain instance
    toolchain = TOOLCHAIN_CLASSES[toolchain_name](target, notify)
    toolchain.VERBOSE = verbose
    toolchain.build_all = clean
    
    # The first path will give the name to the library
    name = basename(src_paths[0])
    toolchain.info("\n>>> BUILD LIBRARY %s (%s, %s)" % (name.upper(), target.name, toolchain_name))
    
    # Scan Resources
    resources = []
    for src_path in src_paths:
        resources.append(toolchain.scan_resources(src_path))
    
    # Dependencies Include Paths
    dependencies = Resources()
    if dependencies_paths is not None:
        for path in dependencies_paths:
            dependencies.add(toolchain.scan_resources(path))
    
    # Create the desired build directory structure
    bin_path = join(build_path, toolchain.obj_path)
    mkdir(bin_path)
    tmp_path = join(build_path, '.temp', toolchain.obj_path)
    mkdir(tmp_path)
    
    # Copy Headers
    for resource in resources:
        toolchain.copy_files(resource.base_path, build_path, resource.headers)
    
    # Compile Sources
    objects = []
    for resource in resources:
        objects.extend(toolchain.compile_sources(resource, tmp_path, dependencies.inc_dirs))
    
    toolchain.build_library(objects, bin_path, name)


def build_lib(lib_id, target, toolchain, verbose=False):
    lib = Library(lib_id)
    if lib.is_supported(target, toolchain):
        build_library(lib.source_dir, lib.build_dir, target, toolchain, lib.dependencies, verbose=verbose)
    else:
        print '\n\nLibrary "%s" is not yet supported on target %s with toolchain %s' % (lib_id, target.name, toolchain)


# We do have unique legacy conventions about how we build and package the mbed library
def build_mbed_libs(target, toolchain_name, verbose=False):
    # Check toolchain support
    if toolchain_name not in target.supported_toolchains:
        print '\n%s target is not yet supported by toolchain %s' % (target.name, toolchain_name)
        return
    
    # Toolchain
    toolchain = TOOLCHAIN_CLASSES[toolchain_name](target)
    toolchain.VERBOSE = verbose
    
    # Source and Build Paths
    TARGET_SRC = join(VENDOR_PATH, target.vendor, target.name)
    BUILD_TARGET = join(MBED_LIBRARIES, target.name)
    BUILD_TOOLCHAIN = join(BUILD_TARGET, toolchain_name)
    mkdir(BUILD_TOOLCHAIN)
    
    TMP_PATH = join(MBED_LIBRARIES, '.temp', toolchain.obj_path)
    mkdir(TMP_PATH)
    
    # CMSIS
    toolchain.info("\n>>> BUILD LIBRARY %s (%s, %s)" % ('CMSIS', target.name, toolchain_name))
    cmsis_src = join(TARGET_SRC, "cmsis")
    resources = toolchain.scan_resources(cmsis_src)
    
    toolchain.copy_files(cmsis_src, BUILD_TARGET, resources.headers + [resources.linker_script])
    objects = toolchain.compile_sources(resources, TMP_PATH, resources.inc_dirs)
    toolchain.copy_files(TMP_PATH, BUILD_TOOLCHAIN, objects)
    
    # mbed
    toolchain.info("\n>>> BUILD LIBRARY %s (%s, %s)" % ('MBED', target.name, toolchain_name))
    HAL_SRC = join(TARGET_SRC, "hal")
    hal_implementation = toolchain.scan_resources(HAL_SRC)
    
    mbed_resources = toolchain.scan_resources(MBED_COMMON)
    mbed_resources.add(hal_implementation)
    
    toolchain.copy_files(MBED_API, MBED_LIBRARIES, toolchain.scan_resources(MBED_API).headers)
    toolchain.copy_files(MBED_HAL, MBED_LIBRARIES, toolchain.scan_resources(MBED_HAL).headers)
    toolchain.copy_files(HAL_SRC, BUILD_TARGET, hal_implementation.headers)
    
    includes = mbed_resources.inc_dirs + [MBED_LIBRARIES, BUILD_TARGET]
    objects = toolchain.compile_sources(mbed_resources, TMP_PATH, includes)
    toolchain.build_library(objects, BUILD_TOOLCHAIN, "mbed")
