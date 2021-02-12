# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

#
# Preprocesses and sets the linker script for an Mbed target.
# Called once for each MCU target in the build system.
#
function(mbed_set_linker_script input_target raw_linker_script_path)
    set(LINKER_SCRIPT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${input_target}.link_script.ld)
    # To avoid path limits on Windows, we create a "response file" and set the path to it as a
    # global property. We need this solely to pass the compile definitions to GCC's preprocessor,
    # so it can expand any macro definitions in the linker script.
    get_property(_linker_preprocess_definitions GLOBAL PROPERTY COMPILE_DEFS_RESPONSE_FILE)
    if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
        add_custom_command(
            OUTPUT
                ${LINKER_SCRIPT_PATH}
            PRE_LINK
            COMMAND
                ${CMAKE_C_COMPILER} ${_linker_preprocess_definitions}
                -E -x assembler-with-cpp
                -P ${raw_linker_script_path}
                -o ${LINKER_SCRIPT_PATH}
            WORKING_DIRECTORY
                ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT
                "Link line:"
            VERBATIM
        )
        # CMake will not let us add PRE_LINK commands to INTERFACE targets, and input_target could
        # be an INTERFACE target.
        # To get around this we create an intermediate custom target depending on the preprocessed
        # linker script output by CPP. We add this custom target as a dependency of input_target.
        # This ensures CMake runs our custom command to preprocess the linker script before trying
        # to build input_target.
        set(LinkerScriptTarget ${input_target}LinkerScript)
        add_custom_target(${LinkerScriptTarget} DEPENDS ${LINKER_SCRIPT_PATH} VERBATIM)
        add_dependencies(${input_target} ${LinkerScriptTarget})
        target_link_options(${input_target}
            INTERFACE
                "-T" "${LINKER_SCRIPT_PATH}"
        )
    elseif(MBED_TOOLCHAIN STREQUAL "ARM")
        target_link_options(${input_target}
            INTERFACE
                "--scatter=${raw_linker_script_path}"
                "--predefine=${_linker_preprocess_definitions}"
                "--map"
        )
    endif()
endfunction()
