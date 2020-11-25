# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(coap-service)
add_subdirectory(mbed-mesh-api)
add_subdirectory(nanostack-hal-mbed-cmsis-rtos)
add_subdirectory(sal-stack-nanostack)
add_subdirectory(sal-stack-nanostack-eventloop)

target_include_directories(mbed-nanostack
    INTERFACE
        .
        ./include
        ./include/nanostack-interface
)

target_sources(mbed-nanostack
    INTERFACE
        source/Nanostack.cpp
)

target_compile_definitions(mbed-nanostack
    INTERFACE
        MBED_CONF_NANOSTACK_PRESENT=1
)

target_link_libraries(mbed-nanostack
    INTERFACE
        mbed-nanostack-mbed_mesh_api
        mbed-nanostack-sal_stack-event_loop
        mbed-nanostack-sal_stack
        mbed-netsocket
)
