# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

function(_mbed_get_libcc_310_core)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LIBCC_310_CORE TOOLCHAIN_GCC_ARM/libcc_310_core.a)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LIBCC_310_CORE TOOLCHAIN_ARM/libcc_310_core.ar)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(LIBCC_310_CORE TOOLCHAIN_IAR/lib_cc310_ext.a)
    endif()
    mbed_link_component(mbed-os-mbedtls-cryptocell310
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/${LIBCC_310_CORE}
    )
endfunction()

_mbed_get_libcc_310_core()
