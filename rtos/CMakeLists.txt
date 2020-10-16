# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/rtos
        ${CMAKE_CURRENT_SOURCE_DIR}/include/rtos/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_sources(mbed-core
    PRIVATE
        source/EventFlags.cpp
        source/Kernel.cpp
        source/Mutex.cpp
        source/Semaphore.cpp
        source/ThisThread.cpp
)

target_sources(mbed-os
    PRIVATE
        source/ConditionVariable.cpp
        source/Thread.cpp
)

target_compile_definitions(mbed-core
    PUBLIC
        MBED_CONF_RTOS_API_PRESENT=1
)
