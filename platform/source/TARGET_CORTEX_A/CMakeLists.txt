# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

function(_mbed_get_system_init)
    if(${MBED_TOOLCHAIN} STREQUAL "IAR")
        target_sources(mbed-core PRIVATE TOOLCHAIN_IAR/cmain.S)
    endif()
endfunction()

_mbed_get_system_init()
