# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ASM_COMPILER "armclang")
set(CMAKE_C_COMPILER "armclang")
set(CMAKE_CXX_COMPILER "armclang")
set(CMAKE_AR "armar")
set(ARM_ELF2BIN "fromelf")
set_property(GLOBAL PROPERTY ELF2BIN ${ARM_ELF2BIN})

option(MBEDIDE "Use Arm compiler from Mbed Studio" OFF)
if(MBEDIDE)
    set_property(GLOBAL PROPERTY MBED_STUDIO_ARM_COMPILER "--ide=mbed")
endif()


# Sets toolchain options
function(mbed_set_toolchain_options target)
    get_property(mbed_studio_arm_compiler GLOBAL PROPERTY MBED_STUDIO_ARM_COMPILER)
    list(APPEND common_options
        "${mbed_studio_arm_compiler}"
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

    target_compile_options(${target}
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${common_options}>
    )

    target_compile_options(${target}
        PUBLIC
            $<$<COMPILE_LANGUAGE:CXX>:${common_options}>
    )

    target_compile_options(${target}
        PUBLIC
            $<$<COMPILE_LANGUAGE:ASM>:--target=arm-arm-none-eabi -masm=auto>
            $<$<COMPILE_LANGUAGE:ASM>:${MBED_STUDIO_ARM_COMPILER}>
    )

    target_compile_definitions(${target}
        PUBLIC
            TOOLCHAIN_ARM
    )

    list(APPEND link_options
        "${MBED_STUDIO_ARM_COMPILER}"
        "--map"
    )

    # Add linking time preprocessor macro for TFM targets
    if(MBED_CPU_CORE MATCHES "\-NS$")
        list(APPEND link_options
            "--predefine=\"-DDOMAIN_NS=0x1\""
        )
    endif()

    target_link_options(${target}
        PUBLIC
            ${link_options}
    )
endfunction()

# Configure the toolchain to select the selected C library
function(mbed_set_c_lib target lib_type)
    if (${lib_type} STREQUAL "small")
        target_compile_definitions(${target}
            PUBLIC
                MBED_RTOS_SINGLE_THREAD
                __MICROLIB
        )

        target_compile_options(${target}
            PUBLIC
                $<$<COMPILE_LANGUAGE:ASM>:"--library_type=microlib">
        )

        target_link_options(${target}
            PUBLIC
                "--library_type=microlib"
        )
    endif()
endfunction()

# Configure the toolchain to select the selected printf library
function(mbed_set_printf_lib target lib_type)
    if (${lib_type} STREQUAL "minimal-printf")
        target_compile_definitions(${target}
            PUBLIC
                MBED_MINIMAL_PRINTF
        )
    endif()
endfunction()
