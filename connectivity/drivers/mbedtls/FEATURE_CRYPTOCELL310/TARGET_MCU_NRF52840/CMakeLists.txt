# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-mbedtls-cryptocell310-nrf52840 OBJECT)

function(_mbed_get_libcc_310_ext)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LIBCC_310_EXT TOOLCHAIN_GCC_ARM/libcc_310_ext.a)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LIBCC_310_EXT TOOLCHAIN_ARM/libcc_310_ext.ar)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(LIBCC_310_EXT TOOLCHAIN_IAR/lib_cc310_ext.a)
    endif()
    mbed_link_component(mbed-os-mbedtls-cryptocell310-nrf52840
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/${LIBCC_310_EXT}
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
    mbed_link_component(mbed-os-mbedtls-cryptocell310-nrf52840
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/${LIBCC_310_TRNG}
    )
endfunction()

_mbed_get_libcc_310_ext()
_mbed_get_libcc_310_trng()

target_include_directories(mbed-os-mbedtls-cryptocell310-nrf52840
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-mbedtls-cryptocell310-nrf52840
    PRIVATE
        crypto_device_platform.c
)

mbed_link_component(mbed-os-mbedtls-cryptocell310-nrf52840 mbed-os-mbedtls)
