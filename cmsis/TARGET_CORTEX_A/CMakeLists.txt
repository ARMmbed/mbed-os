# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        irq_ctrl_gic.c
)

if(${MBED_OS_TOOLCHAIN} STREQUAL "IAR")
    target_sources(mbed-os
        PRIVATE
            TOOLCHAIN_IAR/cmain.S
    )
endif()

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
