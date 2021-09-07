# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ASM_COMPILER "armclang")
set(CMAKE_C_COMPILER "armclang")
set(CMAKE_CXX_COMPILER "armclang")
set(ARM_ELF2BIN "fromelf")
set_property(GLOBAL PROPERTY ELF2BIN ${ARM_ELF2BIN})

# tell cmake about compiler targets.
# This will cause it to add the --target flag.
set(CMAKE_C_COMPILER_TARGET arm-arm-none-eabi)
set(CMAKE_CXX_COMPILER_TARGET arm-arm-none-eabi)

# Sets toolchain options
list(APPEND common_options
    "-mthumb"
    "-Wno-armcc-pragma-push-pop"
    "-Wno-armcc-pragma-anon-unions"
    "-Wno-reserved-user-defined-literal"
    "-Wno-deprecated-register"
    "-fdata-sections"
    "-fno-exceptions"
    "-fshort-enums"
    "-fshort-wchar"
)

list(APPEND asm_compile_options
    -masm=auto
    --target=arm-arm-none-eabi
)

# Add linking time preprocessor macro for TFM targets
if(MBED_CPU_CORE MATCHES "-NS$")
    list(APPEND link_options
        "--predefine=\"-DDOMAIN_NS=0x1\""
    )
endif()

# Configure the toolchain to select the selected C library
function(mbed_set_c_lib target lib_type)
    if (${lib_type} STREQUAL "small")
        target_compile_definitions(${target}
            INTERFACE
                MBED_RTOS_SINGLE_THREAD
                __MICROLIB
        )

        target_link_options(${target}
            INTERFACE
                "--library_type=microlib"
        )
    endif()
endfunction()

# Configure the toolchain to select the selected printf library
function(mbed_set_printf_lib target lib_type)
    if (${lib_type} STREQUAL "minimal-printf")
        target_compile_definitions(${target}
            INTERFACE
                MBED_MINIMAL_PRINTF
        )
    endif()
endfunction()

# Add linker flags to generate a mapfile with a given name
# `mapfile` is overridden as CMake provides the name of the diagnostic output
# file by providing armlink with the --list command line option.
# See https://gitlab.kitware.com/cmake/cmake/-/issues/21538
function(mbed_configure_memory_map target mapfile)
    target_link_options(${target}
        PRIVATE
            "--map"
            "--list=${mapfile}"
    )
endfunction()
