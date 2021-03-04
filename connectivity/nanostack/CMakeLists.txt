# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nanostack OBJECT)
add_library(mbed-nanostack-coap_service INTERFACE)
add_library(mbed-nanostack-mbed_mesh_api INTERFACE)
add_library(mbed-nanostack-hal_mbed_cmsis_rtos INTERFACE)
add_library(mbed-nanostack-sal_stack INTERFACE)
add_library(mbed-nanostack-sal_stack-event_loop INTERFACE)

add_subdirectory(coap-service)
add_subdirectory(mbed-mesh-api)
add_subdirectory(nanostack-hal-mbed-cmsis-rtos)
add_subdirectory(sal-stack-nanostack)
add_subdirectory(sal-stack-nanostack-eventloop)

target_include_directories(mbed-nanostack
    PUBLIC
        .
        ./include
        ./include/nanostack-interface
)

target_sources(mbed-nanostack
    PRIVATE
        source/Nanostack.cpp
)

target_compile_definitions(mbed-nanostack
    PUBLIC
        MBED_CONF_NANOSTACK_PRESENT=1
)

target_link_libraries(mbed-nanostack
    PRIVATE
        mbed-rtos
        mbed-core
        mbed-randlib
        mbed-nanostack-mbed_mesh_api
        mbed-nanostack-sal_stack-event_loop
        mbed-nanostack-sal_stack
        mbed-nanostack-hal_mbed_cmsis_rtos
        mbed-nanostack-libservice
)
