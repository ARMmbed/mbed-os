# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    if(BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/TESTS)
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

target_include_directories(mbed-core
    INTERFACE
        .
        ./include
        ./include/rtos
        ./include/rtos/internal
        ./source
)

target_sources(mbed-core
    INTERFACE
        source/EventFlags.cpp
        source/Kernel.cpp
        source/Mutex.cpp
        source/Semaphore.cpp
        source/ThisThread.cpp
        source/ConditionVariable.cpp
        source/Thread.cpp
)


target_compile_definitions(mbed-core
    INTERFACE
        MBED_CONF_RTOS_API_PRESENT=1
)
