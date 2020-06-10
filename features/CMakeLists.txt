# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(cellular/framework)
add_subdirectory(frameworks/mbed-trace)
add_subdirectory(lwipstack)
add_subdirectory(mbedtls)
add_subdirectory(frameworks/mbed-client-randlib)
add_subdirectory(frameworks/nanostack-libservice)
add_subdirectory(netsocket)
add_subdirectory(nanostack)
add_subdirectory(storage)
add_subdirectory(device_key)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
