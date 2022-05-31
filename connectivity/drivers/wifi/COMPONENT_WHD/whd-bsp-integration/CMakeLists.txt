# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy-psoc6-common-network STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-cy-psoc6-common-network
    PUBLIC
        .
)

target_sources(mbed-cy-psoc6-common-network
    PRIVATE
        cy_network_buffer.c
        cybsp_wifi.c
)

target_link_libraries(mbed-cy-psoc6-common-network
    PUBLIC
        mbed-lwipstack
        mbed-emac
)

target_compile_definitions(mbed-cy-psoc6-common-network
    PUBLIC
        MBED_CONF_cy-psoc6-common-network_PRESENT=1
)
