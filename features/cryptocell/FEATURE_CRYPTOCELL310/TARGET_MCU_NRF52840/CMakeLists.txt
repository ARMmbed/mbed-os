# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

function(_mbed_get_libcc_310_ext)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LIBCC_310_EXT TOOLCHAIN_GCC_ARM/libcc_310_ext.a)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LIBCC_310_EXT TOOLCHAIN_ARM/libcc_310_ext.ar)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(LIBCC_310_EXT TOOLCHAIN_IAR/lib_cc310_ext.a)
    endif()
    target_sources(mbed-os PRIVATE ${LIBCC_310_EXT})
endfunction()

function(_mbed_get_libcc_310_trng)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LIBCC_310_TRNG TOOLCHAIN_GCC_ARM/libcc_310_trng.a)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LIBCC_310_TRNG TOOLCHAIN_ARM/libcc_310_trng.ar)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(LIBCC_310_TRNG TOOLCHAIN_IAR/lib_cc310_trng.a)
    endif()
    target_sources(mbed-os PRIVATE ${LIBCC_310_TRNG})
endfunction()

_mbed_get_libcc_310_ext()
_mbed_get_libcc_310_trng()

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        crypto_device_platform.c
)
