# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ASM_COMPILER "armclang")
set(CMAKE_C_COMPILER "armclang")
set(CMAKE_CXX_COMPILER "armclang")
set(CMAKE_AR "armar")
set(ARM_ELF2BIN "fromelf")
set_property(GLOBAL PROPERTY ELF2BIN ${ARM_ELF2BIN})

# Sets toolchain options
function(mbed_set_toolchain_options target)
    list(
        APPEND
        common_options
        "-c"
        "--target=arm-arm-none-eabi"
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

    target_compile_options(
        ${target} INTERFACE $<$<COMPILE_LANGUAGE:C>:${common_options}>
    )

    target_compile_options(
        ${target} INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${common_options}>
    )

    target_compile_options(
        ${target} INTERFACE $<$<COMPILE_LANGUAGE:ASM>:--target=arm-arm-none-eabi
                            -masm=auto>
    )

    # Add linking time preprocessor macro for TFM targets
    if(MBED_CPU_CORE MATCHES "\-NS$")
        list(APPEND link_options "--predefine=\"-DDOMAIN_NS=0x1\"")
    endif()

    target_link_options(${target} INTERFACE ${link_options})
endfunction()

# Configure the toolchain to select the selected C library
function(mbed_set_c_lib target lib_type)
    if(${lib_type} STREQUAL "small")
        target_compile_definitions(
            ${target} INTERFACE MBED_RTOS_SINGLE_THREAD __MICROLIB
        )

        target_link_options(${target} INTERFACE "--library_type=microlib")
    endif()
endfunction()

# Configure the toolchain to select the selected printf library
function(mbed_set_printf_lib target lib_type)
    if(${lib_type} STREQUAL "minimal-printf")
        target_compile_definitions(${target} INTERFACE MBED_MINIMAL_PRINTF)
    endif()
endfunction()
