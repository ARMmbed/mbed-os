# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-nanostack OBJECT)

mbed_configure_component(mbed-os-nanostack)

add_subdirectory(coap-service)
add_subdirectory(mbed-mesh-api)
add_subdirectory(nanostack-hal-mbed-cmsis-rtos)
add_subdirectory(sal-stack-nanostack)
add_subdirectory(sal-stack-nanostack-eventloop)

target_include_directories(mbed-os-nanostack
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nanostack-interface
)

target_sources(mbed-os-nanostack
    PRIVATE
        source/Nanostack.cpp
)

# TODO: needs fixing, will complain if added 2x
add_subdirectory(../libraries/mbed-coap ${CMAKE_CURRENT_BINARY_DIR}/mbed-coap)

target_link_libraries(mbed-os-nanostack mbed-os-nanostack-libservice mbed-os-netsocket mbed-os-coap mbed-os)
