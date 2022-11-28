# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

#
# Sets the linker script for an Mbed target.
# Called once by the buildscripts for each MCU target in the build system.
# Note: Linker script path may be absolute or relative.  If relative, it will be interpreted relative to the current source dir.
#
function(mbed_set_linker_script input_target raw_linker_script_path)

    # Make sure that we have an absolute path so that it can be used from a different directory
    get_filename_component(raw_linker_script_path ${raw_linker_script_path} ABSOLUTE)

    # Use a custom property to store the linker script.  This property will be read and passed up by mbed_create_distro()
    set_property(TARGET ${input_target} PROPERTY INTERFACE_MBED_LINKER_SCRIPT ${raw_linker_script_path})

endfunction(mbed_set_linker_script)


#
# Set up the linker script for the top-level Mbed OS targets.
# If needed, this also creates another target to preprocess the linker script.
#
# mbed_os_target: CMake target for Mbed OS
# mbed_baremetal_target: CMake target for Mbed Baremetal
# target_defines_header: the full path to the header containing all of the Mbed target defines
#
function(mbed_setup_linker_script mbed_os_target mbed_baremetal_target target_defines_header)

    # Find the path to the desired linker script
    # (the property should be set on both the OS and baremetal targets in a sane world)
    get_property(RAW_LINKER_SCRIPT_PATHS TARGET ${mbed_baremetal_target} PROPERTY INTERFACE_MBED_LINKER_SCRIPT)

    # Check if two (or more) different linker scripts got used
    list(REMOVE_DUPLICATES RAW_LINKER_SCRIPT_PATHS)
    list(LENGTH RAW_LINKER_SCRIPT_PATHS NUM_RAW_LINKER_SCRIPT_PATHS)
    if(NUM_RAW_LINKER_SCRIPT_PATHS GREATER 1)
        message(FATAL_ERROR "More than one linker script selected for the current MCU target.  Perhaps multiple targets with linker scripts set were linked together?")
    endif()

    # Make sure the linker script exists
    if(NOT EXISTS "${RAW_LINKER_SCRIPT_PATHS}")
        message(FATAL_ERROR "Selected linker script ${RAW_LINKER_SCRIPT_PATHS} does not exist!")
    endif()

    set(LINKER_SCRIPT_PATH ${CMAKE_BINARY_DIR}/${MBED_TARGET_CMAKE_NAME}.link_script.ld)

    # To avoid path limits on Windows, we create a "response file" and set the path to it as a
    # global property. We need this solely to pass the compile definitions to GCC's preprocessor,
    # so it can expand any macro definitions in the linker script.
    get_property(linker_defs_response_file GLOBAL PROPERTY COMPILE_DEFS_RESPONSE_FILE)

    get_filename_component(RAW_LINKER_SCRIPT_NAME ${RAW_LINKER_SCRIPT_PATHS} NAME)
    get_filename_component(LINKER_SCRIPT_NAME ${LINKER_SCRIPT_PATH} NAME)
    add_custom_command(
        OUTPUT
            ${LINKER_SCRIPT_PATH}
        PRE_LINK
        COMMAND 
            ${CMAKE_COMMAND} -E echo "Preprocess linker script: ${RAW_LINKER_SCRIPT_NAME} -> ${LINKER_SCRIPT_NAME}"
        COMMAND
            ${CMAKE_C_COMPILER} @${linker_defs_response_file}
            -E -x assembler-with-cpp
            -include ${target_defines_header}
            -P ${RAW_LINKER_SCRIPT_PATHS}
            -o ${LINKER_SCRIPT_PATH}
        DEPENDS
            ${RAW_LINKER_SCRIPT_PATHS}
            ${linker_defs_response_file}
            ${target_defines_header}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM
    )

    
    # The job to create the linker script gets attached to the mbed-linker-script target,
    # which is then added as a dependency of the MCU target.  This ensures the linker script will exist
    # by the time we need it.
    add_custom_target(mbed-linker-script DEPENDS ${LINKER_SCRIPT_PATH} VERBATIM)
    foreach(TARGET ${mbed_baremetal_target} ${mbed_os_target})
        add_dependencies(${TARGET} mbed-linker-script)

        # When building the Mbed internal tests, the tests get created before the mbed-os target does.  So, the normal logic 
        # in mbed_set_post_build() to set the linker script does not work.  So, we need to instead attach the linker script to 
        # the mbed-os and mbed-baremetal libraries themselves, so it will get picked up automatically.
        # This prevents a custom linker script from being used in STANDALONE mode, but we don't need to do that.
        set_target_properties(${TARGET} PROPERTIES LINKER_SCRIPT_PATH  ${LINKER_SCRIPT_PATH})

        #  add linker script only for tests
        if(MBED_IS_STANDALONE)
            target_link_options(${TARGET}
            INTERFACE
                "-T" "${LINKER_SCRIPT_PATH}"
            )
        endif()
    endforeach()

endfunction(mbed_setup_linker_script)


#
# Change the linker script to a custom supplied script instead of the built in.
# this function is called by mbed_set_post_build(target linker_script)
#
# target: CMake target for Mbed OS
# new_linker_script_path: raw linker script
#
function(mbed_set_custom_linker_script target new_linker_script_path)

    set(RAW_LINKER_SCRIPT_PATHS  ${CMAKE_CURRENT_SOURCE_DIR}/${new_linker_script_path})
    set(CUSTOM_LINKER_SCRIPT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${target}.link_spript.ld)

    # To avoid path limits on Windows, we create a "response file" and set the path to it as a
    # global property. We need this solely to pass the compile definitions to GCC's preprocessor,
    # so it can expand any macro definitions in the linker script.
    get_property(linker_defs_response_file GLOBAL PROPERTY COMPILE_DEFS_RESPONSE_FILE)

    get_filename_component(RAW_LINKER_SCRIPT_NAME ${RAW_LINKER_SCRIPT_PATHS} NAME)
    get_filename_component(LINKER_SCRIPT_NAME ${CUSTOM_LINKER_SCRIPT_PATH} NAME)

    add_custom_command(
        TARGET
            ${target}
        PRE_LINK
        COMMAND 
            ${CMAKE_COMMAND} -E echo "Preprocess custom linker script: ${RAW_LINKER_SCRIPT_NAME} -> ${LINKER_SCRIPT_NAME}"
        COMMAND
            ${CMAKE_C_COMPILER} @${linker_defs_response_file}
            -E -x assembler-with-cpp
            -include ${CMAKE_BINARY_DIR}/mbed-os/mbed-target-config.h
            -P ${RAW_LINKER_SCRIPT_PATHS}
            -o ${CUSTOM_LINKER_SCRIPT_PATH}
        DEPENDS
            ${RAW_LINKER_SCRIPT_PATHS}
            ${linker_defs_response_file}
            ${target_defines_header}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM
    )

    # Add linker flags to the target to pick up the preprocessed linker script
    target_link_options(${target}
        PRIVATE
            "-T" "${CUSTOM_LINKER_SCRIPT_PATH}"
    )

endfunction(mbed_set_custom_linker_script)
