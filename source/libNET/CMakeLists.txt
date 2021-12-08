# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
        ./src
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
        src/multicast_api.c
        src/net_6lowpan_parameter_api.c
        src/net_dns.c
        src/net_dns_internal.h
        src/net_ipv6.c
        src/net_load_balance.c
        src/net_load_balance_internal.h
        src/net_mle.c
        src/net_rpl.c
        src/net_short_address_extension.c
        src/net_test.c
        src/ns_net.c
        src/socket_api.c
)
