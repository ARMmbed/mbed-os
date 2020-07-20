# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
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
