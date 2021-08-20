# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build_nxp)

add_library(mbed-mbed-lpc1768 INTERFACE)

target_include_directories(mbed-mbed-lpc1768
    INTERFACE
        .
)

target_link_libraries(mbed-mbed-lpc1768 INTERFACE mbed-lpc176x)

add_library(mbed-lpc1768 INTERFACE)

target_link_libraries(mbed-lpc1768 INTERFACE mbed-mbed-lpc1768)

mbed_post_build_lpc_patch_vtable("LPC1768")
