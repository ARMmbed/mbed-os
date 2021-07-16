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

execute_process(
    COMMAND armclang --version
    ERROR_VARIABLE mbed_studio_check
)

if(mbed_studio_check MATCHES "Mbed Studio")
    if(deprecated_system_processor)
        # In order for the Arm Compiler from Mbed Studio to pass CMake's built-in
        # try_compile() test, CMake >= 3.21.0 with policy CMP0123 set to NEW (i.e. default)
        # is required.
        message(FATAL_ERROR
            "Detected Arm Compiler from Mbed Studio. To use it with CMake, "
            "policy CMP0123 is required and provided by CMake 3.21.0 and above."
        )
    else()
        # NOTE: The --ide=mbed flag is only licensed for use with Mbed OS.
        list(APPEND common_options
            "--ide=mbed"
        )

        list(APPEND link_options
            "--ide=mbed"
        )
        set_property(GLOBAL PROPERTY ELF2BIN ${ARM_ELF2BIN} --ide=mbed)

        # CMake doesn't support passing extra archiver flags to compiler detection
        # tests, so we need to wrap `armar` into a script that appends `--ide=mbed`.
        if(WIN32) # covers WIN64 too
            set(CMAKE_AR "${CMAKE_CURRENT_LIST_DIR}/mbed_studio_armar.bat")
        else()
            set(CMAKE_AR "${CMAKE_CURRENT_LIST_DIR}/mbed_studio_armar.sh")
        endif()

        # CMake's built-in compiler detection test is based on a dummy C program
        # that doesn't contain any "mbed" symbols, which are required by the version
        # of `armlink` licensed by Mbed Studio. So we need to disable the linker test.
        set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
    endif()
endif()

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
