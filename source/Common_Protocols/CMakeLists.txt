# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
        icmpv6.c
        icmpv6_prefix.c
        icmpv6_radv.c
        ipv6.c
        ipv6_flow.c
        ipv6_fragmentation.c
        ipv6_resolution.c
        mld.c
        tcp.c
        udp.c
)
