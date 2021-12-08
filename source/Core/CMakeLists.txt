# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
        ./include
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
        buffer_dyn.c
        ns_address_internal.c
        ns_monitor.c
        ns_socket.c
        sockbuf.c
)
