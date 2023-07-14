# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# File containing various functions for operating on library and executable targets.

include_guard(GLOBAL)

#
# Converts output file of `target` to binary file and to Intel HEX file.
#
function(mbed_generate_bin_hex target)
    get_property(elf_to_bin GLOBAL PROPERTY ELF2BIN)

    set(artifact_name $<TARGET_FILE_BASE_NAME:${target}>)

    # The first condition is quoted in case MBED_OUTPUT_EXT is unset
    if ("${MBED_OUTPUT_EXT}" STREQUAL "" OR MBED_OUTPUT_EXT STREQUAL "bin")
        list(APPEND CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} -O binary $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.bin
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.bin"
        )
    endif()
    if ("${MBED_OUTPUT_EXT}" STREQUAL "" OR MBED_OUTPUT_EXT STREQUAL "hex")
        list(APPEND CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} -O ihex $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.hex
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.hex"
        )
    endif()

    add_custom_command(
        TARGET
            ${target}
        POST_BUILD
        COMMAND
            ${CMAKE_POST_BUILD_COMMAND}
        COMMENT
            "executable:"
        VERBATIM
    )
endfunction()

#
# Parse toolchain generated map file of `target` and display a readable table format.
#
function(mbed_generate_map_file target)
    # set default args for memap.py
    set(MBED_MEMAP_DEPTH "2" CACHE STRING "directory depth level to display report")
    set(MBED_MEMAP_CREATE_JSON FALSE CACHE BOOL "create report in json file")
    set(MBED_MEMAP_CREATE_HTML FALSE CACHE BOOL "create report in html file")

    # generate table for screen
    add_custom_command(
        TARGET
            ${target}
        POST_BUILD
        COMMAND ${Python3_EXECUTABLE} ${mbed-os_SOURCE_DIR}/tools/memap.py 
            -t ${MBED_TOOLCHAIN} ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map 
            --depth ${MBED_MEMAP_DEPTH} 
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
    )

    # generate json file
    if (${MBED_MEMAP_CREATE_JSON})
        add_custom_command(
            TARGET
                ${target}
            POST_BUILD
            COMMAND ${Python3_EXECUTABLE} ${mbed-os_SOURCE_DIR}/tools/memap.py 
            -t ${MBED_TOOLCHAIN} ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map 
            --depth ${MBED_MEMAP_DEPTH} 
            -e json
            -o ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.memmap.json
            WORKING_DIRECTORY
                ${CMAKE_CURRENT_BINARY_DIR}
    )
    endif()

    # generate html file
    if (${MBED_MEMAP_CREATE_HTML})
        add_custom_command(
            TARGET
                ${target}
            POST_BUILD
            COMMAND ${Python3_EXECUTABLE} ${mbed-os_SOURCE_DIR}/tools/memap.py 
            -t ${MBED_TOOLCHAIN} ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map 
            --depth ${MBED_MEMAP_DEPTH} 
            -e html
            -o ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.memmap.html
            WORKING_DIRECTORY
                ${CMAKE_CURRENT_BINARY_DIR}
    )
    endif()
endfunction()

#
# Validate selected application profile.
#
function(mbed_validate_application_profile target)
    get_target_property(app_link_libraries ${target} LINK_LIBRARIES)
    string(FIND "${app_link_libraries}" "mbed-baremetal" string_found_position)
    if(${string_found_position} GREATER_EQUAL 0)
        if(NOT "bare-metal" IN_LIST MBED_TARGET_SUPPORTED_APPLICATION_PROFILES)
            message(FATAL_ERROR
                "Use full profile as baremetal profile is not supported for this Mbed target")
        endif()
    elseif(NOT "full" IN_LIST MBED_TARGET_SUPPORTED_APPLICATION_PROFILES)
        message(FATAL_ERROR
            "The full profile is not supported for this Mbed target")
    endif()
endfunction()

#
# Set post build operations
#
# target: the affected target
# 2nd arg: optional, linker script path 
#
function(mbed_set_post_build target)
    # add linker script. Skip for greentea test code, there the linker script is set in mbed_setup_linker_script()
    if (NOT MBED_IS_STANDALONE)
        if("${ARGN}" STREQUAL "")
            if(TARGET mbed-os)
                get_target_property(LINKER_SCRIPT_PATH mbed-os LINKER_SCRIPT_PATH)
                target_link_options(${target}
                PRIVATE
                    "-T" "${LINKER_SCRIPT_PATH}"
            )
            elseif(TARGET mbed-baremetal)
                get_target_property(LINKER_SCRIPT_PATH mbed-baremetal LINKER_SCRIPT_PATH)
                target_link_options(${target}
                PRIVATE
                    "-T" "${LINKER_SCRIPT_PATH}"
            )
            endif()
        else()
            message(STATUS "${target} uses custom linker script  ${ARGV1}")
            mbed_set_custom_linker_script(${target} ${ARGV1})
        endif()
    endif()

    # The mapfile name includes the top-level target name and the
    # executable suffix for all toolchains as CMake hardcodes the name of the
    # diagnostic output file for some toolchains.

    # copy mapfile .map to .map.old for ram/rom statistics diff in memap.py
    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map)
        add_custom_command(
            TARGET
                ${target}
            PRE_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E rename "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map" "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map.old"
        )
    endif()
    
    mbed_configure_memory_map(${target} "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map")
    mbed_validate_application_profile(${target})
    mbed_generate_bin_hex(${target})

    if(COMMAND mbed_post_build_function)
        mbed_post_build_function(${target})
    endif()

    if(HAVE_MEMAP_DEPS)
        mbed_generate_map_file(${target})
    endif()

    mbed_generate_upload_target(${target})
    mbed_generate_ide_debug_configuration(${target})
endfunction()

#
# Call this at the very end of the build script.  Does some final cleanup tasks such as
# writing out debug configurations.
#
function(mbed_finalize_build)
    mbed_finalize_ide_debug_configurations()
endfunction(mbed_finalize_build)

# Lists that mbed_disable_mcu_target_file stores data in
set(MBED_DISABLE_MCU_TARGET_FILE_TARGETS "" CACHE INTERNAL "" FORCE)
set(MBED_DISABLE_MCU_TARGET_FILE_FILES "" CACHE INTERNAL "" FORCE)

# Use this function to disable a source file from one of the MCU targets in the targets/ directory.
# This allows you to override this file from a custom target.
# This function may only be used with a target in the mbed-os/targets directory, and may only be
# called after including app.cmake and before adding mbed-os as a subdirectory.
function(mbed_disable_mcu_target_file TARGET FILENAME)

    # Record this file for later disablement
    set(MBED_DISABLE_MCU_TARGET_FILE_TARGETS ${MBED_DISABLE_MCU_TARGET_FILE_TARGETS} ${TARGET} CACHE INTERNAL "" FORCE)
    set(MBED_DISABLE_MCU_TARGET_FILE_FILES ${MBED_DISABLE_MCU_TARGET_FILE_FILES} ${FILENAME} CACHE INTERNAL "" FORCE)

endfunction(mbed_disable_mcu_target_file)

# Called later, midway through the Mbed configure, to apply the file disables that were recorded earlier.
function(mbed_apply_mcu_target_file_disables)

    # Iterate through each disable request
    list(LENGTH MBED_DISABLE_MCU_TARGET_FILE_TARGETS NUM_DISABLES)
    set(DISABLE_IDX 0)
    while(DISABLE_IDX LESS NUM_DISABLES)

        # Get the target and the file
        list(GET MBED_DISABLE_MCU_TARGET_FILE_TARGETS ${DISABLE_IDX} CURR_TARGET)
        list(GET MBED_DISABLE_MCU_TARGET_FILE_FILES ${DISABLE_IDX} CURR_FILE)

        if(TARGET ${CURR_TARGET})
            get_property(CURR_TARGET_TYPE TARGET ${CURR_TARGET} PROPERTY TYPE)
            if("${CURR_TARGET_TYPE}" STREQUAL "INTERFACE_LIBRARY")

                # Iterate through the list of sources and remove the target one
                get_property(CURR_TARGET_IFACE_SOURCES TARGET ${CURR_TARGET} PROPERTY INTERFACE_SOURCES)
                set(FOUND FALSE)
                foreach(SOURCE_FILE ${CURR_TARGET_IFACE_SOURCES})
                    get_filename_component(SOURCE_FILE_NAME ${SOURCE_FILE} NAME)
                    if("${SOURCE_FILE_NAME}" STREQUAL CURR_FILE)
                        set(FOUND TRUE)
                        list(REMOVE_ITEM CURR_TARGET_IFACE_SOURCES "${SOURCE_FILE}")
                    endif()
                endforeach()

                if(FOUND)
                    message(STATUS "Disabled file '${CURR_FILE}' in target '${CURR_TARGET}'")
                else()
                    message(WARNING "mbed_disable_mcu_target_file(): File '${CURR_FILE}' not found in target '${CURR_TARGET}'")
                endif()

                set_property(TARGET ${CURR_TARGET} PROPERTY INTERFACE_SOURCES ${CURR_TARGET_IFACE_SOURCES})

            else()
                message(FATAL_ERROR "mbed_disable_mcu_target_file(): Target '${CURR_TARGET}' is not an interface target.")
            endif()
        else()
            message(FATAL_ERROR "mbed_disable_mcu_target_file(): Failed to find target '${CURR_TARGET}'.")
        endif()

        math(EXPR DISABLE_IDX "${DISABLE_IDX} + 1")
    endwhile()

endfunction(mbed_apply_mcu_target_file_disables)