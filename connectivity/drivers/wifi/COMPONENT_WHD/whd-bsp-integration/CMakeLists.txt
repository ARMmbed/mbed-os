# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy-psoc6-common-network INTERFACE)

target_include_directories(mbed-cy-psoc6-common-network
    INTERFACE
        .
)

target_sources(mbed-cy-psoc6-common-network
    INTERFACE
        cy_network_buffer.c
        cybsp_wifi.c
)

target_link_libraries(mbed-cy-psoc6-common-network
    INTERFACE
        mbed-lwipstack
        mbed-emac
)

target_compile_definitions(mbed-cy-psoc6-common-network
    INTERFACE
        MBED_CONF_cy-psoc6-common-network_PRESENT=1
)
