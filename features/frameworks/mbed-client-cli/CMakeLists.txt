# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-client-cli
    INTERFACE
        .
        ./mbed-client-cli
)

target_sources(mbed-os-client-cli
    INTERFACE
        source/ns_cmdline.c
)

target_link_libraries(mbed-os-client-cli
    INTERFACE
        mbed-os-nanostack-libservice
)
