# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy_psoc6_common_network INTERFACE)

target_include_directories(mbed-cy_psoc6_common_network
    INTERFACE
        .
)

target_sources(mbed-cy_psoc6_common_network
    INTERFACE
        cy_network_buffer.c
        cybsp_wifi.c
)

target_link_libraries(mbed-cy_psoc6_common_network
    INTERFACE
        mbed-lwipstack
        mbed-emac
)

target_compile_definitions(mbed-cy_psoc6_common_network
    INTERFACE
        MBED_CONF_CY_PSOC6_COMMON_NETWORK_PRESENT=1
)
