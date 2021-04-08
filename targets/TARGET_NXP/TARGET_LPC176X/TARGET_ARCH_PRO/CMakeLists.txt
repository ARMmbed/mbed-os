# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build_nxp)

add_library(mbed-arch-pro INTERFACE)

target_include_directories(mbed-arch-pro
    INTERFACE
        .
)

target_link_libraries(mbed-arch-pro INTERFACE mbed-lpc176x)

mbed_post_build_lpc_patch_vtable("ARCH_PRO")
