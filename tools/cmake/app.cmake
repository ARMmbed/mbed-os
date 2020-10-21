# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

# Set default toolchain file
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${MBED_ROOT}/tools/cmake/toolchain.cmake" CACHE INTERNAL "")
endif()

# Toolchain setup
include(${MBED_ROOT}/tools/cmake/toolchains/${MBED_TOOLCHAIN}.cmake)
enable_language(C CXX ASM)
