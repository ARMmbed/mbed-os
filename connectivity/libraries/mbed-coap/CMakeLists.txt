# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-coap OBJECT)

target_include_directories(mbed-os-coap
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/mbed-coap
        ${CMAKE_CURRENT_SOURCE_DIR}/source/include
)

target_sources(mbed-os-coap
    PRIVATE
        source/sn_coap_builder.c
        source/sn_coap_header_check.c
        source/sn_coap_parser.c
        source/sn_coap_protocol.c
)

target_link_libraries(mbed-os-coap mbed-os-nanostack-libservice mbed-os)
