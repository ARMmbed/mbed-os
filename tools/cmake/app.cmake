# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

find_program(CCACHE "ccache")
if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

set(MBED_TOOLCHAIN_FILE_USED FALSE)

# Set default toolchain file
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(MBED_TOOLCHAIN_FILE_USED TRUE)

    set(CMAKE_TOOLCHAIN_FILE "${MBED_PATH}/tools/cmake/toolchain.cmake" CACHE INTERNAL "")

    # Specify locations for toolchains and generic options
    include(${MBED_PATH}/tools/cmake/toolchains/${MBED_TOOLCHAIN}.cmake)

    # Specify available build profiles and add options for the selected build profile
    include(${MBED_PATH}/tools/cmake/profile.cmake)
endif()

enable_language(C CXX ASM)
