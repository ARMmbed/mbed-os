# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
        dhcp_service_api.c
        libDHCPv6.c
        libDHCPv6_server.c
        libDHCPv6_vendordata.c
)
