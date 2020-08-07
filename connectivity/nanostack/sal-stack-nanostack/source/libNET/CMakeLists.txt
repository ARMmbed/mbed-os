# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(mbed-os
    PRIVATE
        src/multicast_api.c
        src/net_6lowpan_parameter_api.c
        src/net_ipv6.c
        src/net_load_balance.c
        src/net_mle.c
        src/net_rpl.c
        src/net_short_address_extension.c
        src/net_test.c
        src/ns_net.c
        src/socket_api.c
)
