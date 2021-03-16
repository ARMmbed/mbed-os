# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LIBCC_310_EXT TOOLCHAIN_GCC_ARM/libcc_310_ext.a)
    set(LIBCC_310_TRNG TOOLCHAIN_GCC_ARM/libcc_310_trng.a)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LIBCC_310_EXT TOOLCHAIN_ARM/libcc_310_ext.ar)
    set(LIBCC_310_TRNG TOOLCHAIN_ARM/libcc_310_trng.ar)
endif()

target_include_directories(mbed-mbedtls
    INTERFACE
        .
)

target_sources(mbed-mbedtls
    INTERFACE
        crypto_device_platform.c
)

target_link_libraries(mbed-mbedtls
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/${LIBCC_310_EXT}
        ${CMAKE_CURRENT_SOURCE_DIR}/${LIBCC_310_TRNG}
)
