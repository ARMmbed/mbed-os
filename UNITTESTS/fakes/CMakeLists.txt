# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(ble)

add_library(mbed-os-fakes-event-queue)

target_sources(mbed-os-fakes-event-queue
    PRIVATE
        ${MBED_PATH}/UNITTESTS/fakes/events/EventQueue.cpp
)

target_include_directories(mbed-os-fakes-event-queue
    PUBLIC
        ${MBED_PATH}/UNITTESTS/fakes
)

target_link_libraries(mbed-os-fakes-event-queue
    PRIVATE
        mbed-os-headers
)
