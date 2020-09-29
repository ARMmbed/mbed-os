# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-nanostack INTERFACE)

add_subdirectory(coap-service)
add_subdirectory(mbed-mesh-api)
add_subdirectory(nanostack-hal-mbed-cmsis-rtos)
add_subdirectory(sal-stack-nanostack)
add_subdirectory(sal-stack-nanostack-eventloop)

target_include_directories(mbed-os-nanostack
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nanostack-interface
)

target_sources(mbed-os-nanostack
    INTERFACE
        source/Nanostack.cpp
)

target_compile_definitions(mbed-os-nanostack
    INTERFACE
        MBED_CONF_NANOSTACK_PRESENT=1
)

target_link_libraries(mbed-os-nanostack INTERFACE mbed-os-nanostack-libservice mbed-os-netsocket mbed-os-coap)
