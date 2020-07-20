# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


function(_mbed_set_assembly_source)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        target_sources(mbed-os PRIVATE TOOLCHAIN_GCC/except.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        target_sources(mbed-os PRIVATE TOOLCHAIN_ARM/except.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        target_sources(mbed-os
            PRIVATE
                TOOLCHAIN_IAR/except.S
                TOOLCHAIN_IAR/cmain.S
        )
    endif()
endfunction()

_mbed_set_assembly_source()

target_sources(mbed-os
    PRIVATE
        mbed_fault_handler.c
)
