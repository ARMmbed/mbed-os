# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Script responsible for generating the Mbed OS CMake configuration file (mbed_config.cmake) automatically
# when needed

# Input variables:
#  MBED_TARGET - Mbed OS target name that you want to build for
#  MBED_APP_JSON_PATH - Path to mbed_app.json.  If relative, will be interpreted relative to CMAKE_SOURCE_DIR.
#  CUSTOM_TARGETS_JSON_PATH - Full path to custom_targets.json, may be set to empty/undefined if not using it.  If relative, will be interpreted relative to CMAKE_SOURCE_DIR.

set(MBED_NEED_TO_RECONFIGURE FALSE)

# First, verify that MBED_TARGET has not changed
if(DEFINED MBED_INTERNAL_LAST_MBED_TARGET)
    if(NOT "${MBED_INTERNAL_LAST_MBED_TARGET}" STREQUAL "${MBED_TARGET}")
        message(FATAL_ERROR "MBED_TARGET has changed since this directory was originally configured. (previously was: ${MBED_INTERNAL_LAST_MBED_TARGET}).  Due to CMake limitations, this build dir must be deleted and recreated from scratch (or use cmake --fresh [CMake 3.24 or newer]).")
    endif()
else()
    if("${MBED_TARGET}" STREQUAL "")
        message(FATAL_ERROR "You must define MBED_TARGET to the Mbed target you wish to build for!")
    endif()

    # First run of CMake
    message(STATUS "Mbed: First CMake run detected, generating configs...")
    set(MBED_NEED_TO_RECONFIGURE TRUE)
endif()
set(MBED_INTERNAL_LAST_MBED_TARGET "${MBED_TARGET}" CACHE INTERNAL "Previous mbed target this dir was configured with" FORCE)

# Convert all relative paths to absolute paths, rooted at CMAKE_SOURCE_DIR.
# This makes sure that they are interpreted the same way everywhere.
get_filename_component(MBED_APP_JSON_PATH "${MBED_APP_JSON_PATH}" ABSOLUTE BASE_DIR ${CMAKE_SOURCE_DIR})
get_filename_component(CUSTOM_TARGETS_JSON_PATH "${CUSTOM_TARGETS_JSON_PATH}" ABSOLUTE BASE_DIR ${CMAKE_SOURCE_DIR})

# Check if mbed_app.json was modified
# Note: if the path is an empty string, get_filename_component(ABSOLUTE) will convert it to a directory,
# so we have to verify that the path we have is a file, not a dir.
if(NOT EXISTS "${MBED_APP_JSON_PATH}" OR IS_DIRECTORY "${MBED_APP_JSON_PATH}")
    message(FATAL_ERROR "Failed to find mbed_app.json at ${MBED_APP_JSON_PATH}")
endif()

file(TIMESTAMP "${MBED_APP_JSON_PATH}" MBED_APP_JSON_TIMESTAMP "%s" UTC)
if(NOT MBED_NEED_TO_RECONFIGURE)
    if(NOT "${MBED_INTERNAL_LAST_MBED_APP_JSON_TIMESTAMP}" STREQUAL "${MBED_APP_JSON_TIMESTAMP}")
        message(STATUS "Mbed: mbed_app.json modified, regenerating configs...")
        set(MBED_NEED_TO_RECONFIGURE TRUE)
    endif()
endif()
set(MBED_INTERNAL_LAST_MBED_APP_JSON_TIMESTAMP "${MBED_APP_JSON_TIMESTAMP}" CACHE INTERNAL "Previous UTC modification timestamp for mbed_app.json" FORCE)

# Check if custom_targets.json was modified
if(EXISTS "${CUSTOM_TARGETS_JSON_PATH}" AND (NOT IS_DIRECTORY "${CUSTOM_TARGETS_JSON_PATH}"))
    file(TIMESTAMP "${CUSTOM_TARGETS_JSON_PATH}" CUSTOM_TARGETS_JSON_TIMESTAMP "%s" UTC)
else()
    set(CUSTOM_TARGETS_JSON_TIMESTAMP "<none>")
endif()

if(NOT MBED_NEED_TO_RECONFIGURE)
    if(NOT "${MBED_INTERNAL_LAST_CUSTOM_TARGETS_JSON_TIMESTAMP}" STREQUAL "${CUSTOM_TARGETS_JSON_TIMESTAMP}")
        message(STATUS "Mbed: custom_targets.json modified, regenerating configs...")
        set(MBED_NEED_TO_RECONFIGURE TRUE)
    endif()
endif()

set(MBED_INTERNAL_LAST_CUSTOM_TARGETS_JSON_TIMESTAMP "${CUSTOM_TARGETS_JSON_TIMESTAMP}" CACHE INTERNAL "Previous UTC modification timestamp for custom_targets.json" FORCE)

# Check if a previous configure failed
if(DEFINED MBED_INTERNAL_CONFIGURE_SUCCEEDED)
    if(NOT MBED_NEED_TO_RECONFIGURE)
        if(NOT MBED_INTERNAL_CONFIGURE_SUCCEEDED)
            message(STATUS "Mbed: previous generation attempt did not succeed, regenerating configs...")
            set(MBED_NEED_TO_RECONFIGURE TRUE)
        endif()
    endif()
endif()

if(MBED_NEED_TO_RECONFIGURE)
    # Generate mbed_config.cmake for this target
    if(EXISTS "${CUSTOM_TARGETS_JSON_PATH}" AND (NOT IS_DIRECTORY "${CUSTOM_TARGETS_JSON_PATH}"))
        set(CUSTOM_TARGET_ARGUMENT --custom-targets-json "${CUSTOM_TARGETS_JSON_PATH}")
    else()
        set(CUSTOM_TARGET_ARGUMENT "")
    endif()

    # Make sure an old config file doesn't stick around
    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/mbed_config.cmake)

    set(MBEDTOOLS_CONFIGURE_COMMAND ${Python3_EXECUTABLE}
        -c "import mbed_tools.cli.main\; mbed_tools.cli.main.cli()" # This is used instead of invoking mbed_tools as a script, because it might not be on the user's PATH.
        configure
        -t GCC_ARM # GCC_ARM is currently the only supported toolchain
        -m "${MBED_TARGET}"
        --mbed-os-path ${CMAKE_CURRENT_LIST_DIR}/../..
        --output-dir ${CMAKE_CURRENT_BINARY_DIR}
        --app-config "${MBED_APP_JSON_PATH}"
        ${CUSTOM_TARGET_ARGUMENT})

    execute_process(
        COMMAND ${MBEDTOOLS_CONFIGURE_COMMAND}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        RESULT_VARIABLE MBEDTOOLS_CONFIGURE_RESULT
        OUTPUT_VARIABLE MBEDTOOLS_CONFIGURE_OUTPUT
        ERROR_VARIABLE MBEDTOOLS_CONFIGURE_ERROR_OUTPUT
        ECHO_OUTPUT_VARIABLE
        ECHO_ERROR_VARIABLE
    )

    if((NOT MBEDTOOLS_CONFIGURE_RESULT EQUAL 0) OR (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/mbed_config.cmake))
        set(MBED_INTERNAL_CONFIGURE_SUCCEEDED FALSE CACHE INTERNAL "Whether the mbedtools configure command succeeded on the most recent try" FORCE)
        string(JOIN " " MBEDTOOLS_COMMAND_SPC_SEP ${MBEDTOOLS_CONFIGURE_COMMAND})
        message(FATAL_ERROR "mbedtools configure failed!  Cannot build this project.  Command was ${MBEDTOOLS_COMMAND_SPC_SEP}")
    else()
        set(MBED_INTERNAL_CONFIGURE_SUCCEEDED TRUE CACHE INTERNAL "Whether the mbedtools configure command succeeded on the most recent try" FORCE)
    endif()

endif()

# Include the generated config file
include(${CMAKE_CURRENT_BINARY_DIR}/mbed_config.cmake)