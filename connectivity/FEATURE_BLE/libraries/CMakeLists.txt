# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-ble-cordio OBJECT)

if("CORDIO_LL" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CORDIO_LL)
endif()

add_subdirectory(cordio_stack)

target_include_directories(mbed-os-ble-cordio
    PUBLIC
        .
)

mbed_link_component(mbed-os-ble mbed-os-ble-cordio)
