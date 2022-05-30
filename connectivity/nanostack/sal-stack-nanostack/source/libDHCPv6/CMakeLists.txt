# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    PUBLIC
        .
)

target_sources(mbed-nanostack-sal_stack
    PRIVATE
        dhcp_service_api.c
        libDHCPv6.c
        libDHCPv6_server.c
        libDHCPv6_vendordata.c
)
