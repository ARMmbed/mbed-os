# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-coap OBJECT)

mbed_configure_component(mbed-os-coap)

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

mbed_enable_components(mbed-os-nanostack-libservice)

target_link_libraries(mbed-os-coap mbed-os-nanostack-libservice mbed-os)
