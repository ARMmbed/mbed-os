# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Add our CMake list files to CMake default module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

find_program(CCACHE "ccache")
if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

include(${MBED_CONFIG_PATH}/mbed_config.cmake)
include(mbed_set_post_build)
include(mbed_generate_config_header)
include(mbed_create_distro)

# Load toolchain file
if(NOT CMAKE_TOOLCHAIN_FILE OR MBED_TOOLCHAIN_FILE_USED)
    set(MBED_TOOLCHAIN_FILE_USED TRUE CACHE INTERNAL "")
    # We want to bring CMP0123 we set in mbed_toolchain.cmake
    # to the whole Mbed OS.
    include(mbed_toolchain NO_POLICY_SCOPE)
endif()

# Specify available build profiles and add options for the selected build profile
include(mbed_profile)

enable_language(C CXX ASM)

# set executable suffix (has to be done after enabling languages)
# Note: This is nice to have, but is also required because STM32Cube will only work on files with a .elf extension
set(CMAKE_EXECUTABLE_SUFFIX .elf)

# Find Python
find_package(Python3 COMPONENTS Interpreter)
include(CheckPythonPackage)

# Check python packages
set(PYTHON_PACKAGES_TO_CHECK intelhex prettytable future jinja2 mbed_tools)
foreach(PACKAGE_NAME ${PYTHON_PACKAGES_TO_CHECK})
    string(TOUPPER ${PACKAGE_NAME} PACKAGE_NAME_UCASE) # Ucase name needed for CMake variable
    string(TOLOWER ${PACKAGE_NAME} PACKAGE_NAME_LCASE) # Lcase name needed for import statement

    check_python_package(${PACKAGE_NAME_LCASE} HAVE_PYTHON_${PACKAGE_NAME_UCASE})
    if(NOT HAVE_PYTHON_${PACKAGE_NAME_UCASE})
        message(WARNING "Missing Python dependency ${PACKAGE_NAME}")
    endif()
endforeach()

# Check deps for memap
if(Python3_FOUND AND HAVE_PYTHON_INTELHEX AND HAVE_PYTHON_PRETTYTABLE)
    set(HAVE_MEMAP_DEPS TRUE)
else()
    set(HAVE_MEMAP_DEPS FALSE)
    message(STATUS "Missing Python dependencies (at least one of: python3, intelhex, prettytable) so the memory map cannot be printed")
endif()

# load mbed_create_distro
include(${CMAKE_CURRENT_LIST_DIR}/mbed_create_distro.cmake)

# load upload method configuration defaults for this target
set(EXPECTED_MBED_UPLOAD_CFG_FILE_PATH targets/upload_method_cfg/${MBED_TARGET}.cmake)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../${EXPECTED_MBED_UPLOAD_CFG_FILE_PATH})
    message(STATUS "Mbed: Loading default upload method configuration from ${EXPECTED_MBED_UPLOAD_CFG_FILE_PATH}")
    include(${CMAKE_CURRENT_LIST_DIR}/../../${EXPECTED_MBED_UPLOAD_CFG_FILE_PATH})
else()
    message(STATUS "Mbed: Target does not have any upload method configuration.  'make flash-' commands will not be available unless configured by the upper-level project.")
    set(UPLOAD_METHOD_DEFAULT "NONE")
endif()
