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
    target_link_libraries(mbed-mbedtls-cryptocell310
        INTERFACE
            ./${LIBCC_310_EXT}
    )
endfunction()

function(_mbed_get_libcc_310_trng)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LIBCC_310_TRNG TOOLCHAIN_GCC_ARM/libcc_310_trng.a)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LIBCC_310_TRNG TOOLCHAIN_ARM/libcc_310_trng.ar)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(LIBCC_310_TRNG TOOLCHAIN_IAR/lib_cc310_trng.a)
    endif()
    target_link_libraries(mbed-mbedtls-cryptocell310
        INTERFACE
            ./${LIBCC_310_TRNG}
    )
endfunction()

_mbed_get_libcc_310_ext()
_mbed_get_libcc_310_trng()

target_include_directories(mbed-mbedtls-cryptocell310
    INTERFACE
        .
)

target_sources(mbed-mbedtls-cryptocell310
    INTERFACE
        crypto_device_platform.c
)