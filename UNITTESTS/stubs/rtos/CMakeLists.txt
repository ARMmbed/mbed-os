# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-rtos)

target_sources(mbed-stubs-rtos
    PRIVATE
        ConditionVariable_stub.cpp
        Kernel_stub.cpp
        mbed_rtos_rtx_stub.c
        Mutex_stub.cpp
        rtx_mutex_stub.c
        Semaphore_stub.cpp
        ThisThread_stub.cpp
        Thread_stub.cpp
)

target_link_libraries(mbed-stubs-rtos
    PRIVATE
        mbed-headers
        mbed-stubs-headers
)
