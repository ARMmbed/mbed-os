# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Generate a file containing compile definitions and include paths
function(mbed_generate_options_for_linker target output_response_file_path)
    set(_compile_definitions
        "$<TARGET_PROPERTY:${target},INTERFACE_COMPILE_DEFINITIONS>"
    )
    set(_include_directories
        "$<TARGET_PROPERTY:${target},INTERFACE_INCLUDE_DIRECTORIES>"
    )

    # Append -D to all macros and quote them as we pass these as response file to cxx compiler
    set(_compile_definitions
        "$<$<BOOL:${_compile_definitions}>:'-D$<JOIN:${_compile_definitions},' '-D>'>"
    )
    # Prepend -I to all macros and quote them as we pass these as response file to cxx compiler
    set(_include_directories
        "$<$<BOOL:${_compile_definitions}>:'-I$<JOIN:${_include_directories},' '-I>'>"
    )
    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/linker_script_preprocess_flags.txt" CONTENT
"${_compile_definitions}
${_include_directories}
")
    set(${output_response_file_path} ${CMAKE_CURRENT_BINARY_DIR}/linker_script_preprocess_flags.txt PARENT_SCOPE)
endfunction()

# Compiler setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)

# Clear toolchains options for all languages as Mbed OS uses
# different initialisation options (such as for optimization and debug symbols)
set(CMAKE_C_FLAGS_DEBUG "" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE STRING "" FORCE)

set(CMAKE_ASM_FLAGS_DEBUG "" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO "" CACHE STRING "" FORCE)

# Use response files always
set(CMAKE_ASM_USE_RESPONSE_FILE_FOR_INCLUDES 1)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 1)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)

set(CMAKE_ASM_USE_RESPONSE_FILE_FOR_OBJECTS 1)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)

set(CMAKE_ASM_USE_RESPONSE_FILE_FOR_LIBRARIES 1)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_LIBRARIES 1)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES 1)

# Set basic compiler and linker flags.
# Also load some utility functions for setting the toolchain options.
set(link_options "")
set(common_options "")
set(c_cxx_compile_options "") # compile options only for C/CXX
set(cxx_compile_options "") # compile options only for CXX
set(asm_compile_options "") # compile options only for ASM

include(toolchains/${MBED_TOOLCHAIN})

set(MBED_CPU_CORE_TAG ${MBED_CPU_CORE})
if (MBED_CPU_CORE MATCHES "Cortex-A[0-9]+$")
    set(MBED_CPU_CORE_TAG "Cortex-A")
    string(REPLACE "Cortex-" "" MBED_CPU_CORE_CODE "${MBED_CPU_CORE}")
endif()

include(cores/${MBED_CPU_CORE_TAG})

#converts a list into a string with each of its elements seperated by a space
macro(list_to_space_separated OUTPUT_VAR)# 2nd arg: LIST...
    string(REPLACE ";" " " ${OUTPUT_VAR} "${ARGN}")
endmacro(list_to_space_separated)

# set toolchain flags with CMake (INIT variables will be picked up on first run)
list_to_space_separated(CMAKE_C_FLAGS_INIT ${common_options} ${c_cxx_compile_options})
list_to_space_separated(CMAKE_CXX_FLAGS_INIT ${common_options} ${c_cxx_compile_options} ${cxx_compile_options})
list_to_space_separated(CMAKE_ASM_FLAGS_INIT ${common_options} ${asm_compile_options})
list_to_space_separated(CMAKE_EXE_LINKER_FLAGS_INIT ${link_options})

# due to a bug in ARMClang CMake compiler file, all _INIT flags must end with a space.
# (this line: https://github.com/Kitware/CMake/blob/7d4a6ce714a9213ffd34b13a3debcb31a2430e04/Modules/Compiler/ARMClang.cmake#L97)
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} ")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} ")
set(CMAKE_ASM_FLAGS_INIT "${CMAKE_ASM_FLAGS_INIT} ")

# Set language standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS TRUE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS TRUE)
