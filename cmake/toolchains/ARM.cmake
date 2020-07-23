# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ASM_COMPILER    "armasm")
set(CMAKE_C_COMPILER      "armclang")
set(CMAKE_CXX_COMPILER    "armclang")
set(CMAKE_AR              "armar")
set(ELF2BIN               "fromelf")

list(APPEND common_options
    "${MBED_STUDIO_ARM_COMPILER}"
    "-c"
    "--target=arm-arm-none-eabi"
    "-Oz"
    "-Wno-armcc-pragma-push-pop"
    "-Wno-armcc-pragma-anon-unions"
    "-Wno-reserved-user-defined-literal"
    "-Wno-deprecated-register"
    "-fdata-sections"
    "-fno-exceptions"
    "-fshort-enums"
    "-fshort-wchar"
)

target_compile_options(mbed-os
    PUBLIC
        $<$<COMPILE_LANGUAGE:C>:${common_options}>
)

target_compile_options(mbed-os
    PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:${common_options}>
)

target_compile_options(mbed-os
    PUBLIC
        $<$<COMPILE_LANGUAGE:ASM>:${MBED_STUDIO_ARM_COMPILER}>
        $<$<COMPILE_LANGUAGE:ASM>:--cpreproc>
        $<$<COMPILE_LANGUAGE:ASM>:--cpreproc_opts=--target=arm-arm-none-eabi,-mcpu=cortex-m4,-D,__FPU_PRESENT,-D,MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED>
)

target_compile_definitions(mbed-os
    PUBLIC
        TOOLCHAIN_ARM
)

target_link_options(mbed-os
    PUBLIC
        ${MBED_STUDIO_ARM_COMPILER}
)
