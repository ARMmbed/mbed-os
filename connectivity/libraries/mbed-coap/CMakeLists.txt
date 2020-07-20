# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/mbed-coap
        ${CMAKE_CURRENT_SOURCE_DIR}/source/include
)

target_sources(mbed-os
    PRIVATE
        source/sn_coap_builder.c
        source/sn_coap_header_check.c
        source/sn_coap_parser.c
        source/sn_coap_protocol.c
)
