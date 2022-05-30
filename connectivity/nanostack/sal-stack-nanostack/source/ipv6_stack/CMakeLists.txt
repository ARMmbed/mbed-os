# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    PUBLIC
        .
)

target_sources(mbed-nanostack-sal_stack
    PRIVATE
        ipv6_routing_table.c
        protocol_ipv6.c
)
