# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(mbed-os
    PRIVATE
        buffer_dyn.c
        ns_address_internal.c
        ns_monitor.c
        ns_socket.c
        sockbuf.c
)
