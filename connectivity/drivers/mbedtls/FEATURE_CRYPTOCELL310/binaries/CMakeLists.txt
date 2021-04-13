# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LIBCC_310_CORE TOOLCHAIN_GCC_ARM/libcc_310_core.a)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LIBCC_310_CORE TOOLCHAIN_ARM/libcc_310_core.ar)
endif()

target_link_libraries(mbed-mbedtls
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/${LIBCC_310_CORE}
)
