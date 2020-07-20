# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

function(_mbed_get_libcc_310_core)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LIBCC_310_CORE TOOLCHAIN_GCC_ARM/libcc_310_core.a)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LIBCC_310_CORE TOOLCHAIN_ARM/libcc_310_core.ar)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(LIBCC_310_CORE TOOLCHAIN_IAR/lib_cc310_ext.a)
    endif()
    target_sources(mbed-os PRIVATE ${LIBCC_310_CORE})
endfunction()

_mbed_get_libcc_310_core()
