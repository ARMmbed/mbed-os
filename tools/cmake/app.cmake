# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

find_program(CCACHE "ccache")
if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

# Load toolchain file
if(NOT CMAKE_TOOLCHAIN_FILE OR MBED_TOOLCHAIN_FILE_USED)
    set(MBED_TOOLCHAIN_FILE_USED TRUE CACHE INTERNAL "")
    include(${MBED_PATH}/tools/cmake/toolchain.cmake)
endif()

# Specify available build profiles and add options for the selected build profile
include(${MBED_PATH}/tools/cmake/profile.cmake)

enable_language(C CXX ASM)

# set executable suffix (has to be done after enabling languages)
# Note: This is nice to have, but is also required because STM32Cube will only work on files with a .elf extension
set(CMAKE_EXECUTABLE_SUFFIX .elf)