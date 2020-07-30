# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(GCC_ELF2BIN "arm-none-eabi-objcopy")
set_property(GLOBAL PROPERTY ELF2BIN ${GCC_ELF2BIN})

list(APPEND link_options
    "-Wl,--start-group"
    "-lstdc++"
    "-lsupc++"
    "-lm"
    "-lc"
    "-lgcc"
    "-lnosys"
    "-Wl,--end-group"
    "-T"
    "${CMAKE_BINARY_DIR}/${APP_TARGET}.link_script.ld"
)

list(APPEND common_options
    "-Wall"
    "-Wextra"
    "-Wno-unused-parameter"
    "-Wno-missing-field-initializers"
    "-fmessage-length=0"
    "-fno-exceptions"
    "-ffunction-sections"
    "-fdata-sections"
    "-funsigned-char"
    "-MMD"
    "-fomit-frame-pointer"
    "-g3"
)

target_compile_options(mbed-os
    PUBLIC
        ${common_options}
)

target_compile_definitions(mbed-os
    PUBLIC
        TOOLCHAIN_GCC_ARM
        TOOLCHAIN_GCC
)

target_link_options(mbed-os
    PUBLIC
        ${common_options}
        ${link_options}
)
