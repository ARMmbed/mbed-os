# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        # add greentea test
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

# Note: The event queue library gets a different implementation when the RTOS is or is not included.
# So, we add it to the core Mbed lib so that it will be built once for each Mbed version.
target_include_directories(mbed-core-flags
    INTERFACE
        .
        ./include
        ./include/events
        ./include/events/internal
)

target_compile_definitions(mbed-core-flags
    INTERFACE
        MBED_CONF_EVENTS_PRESENT=1
)

target_sources(mbed-core-sources
    INTERFACE
        source/EventQueue.cpp
        source/equeue.c
        source/equeue_mbed.cpp
        source/equeue_posix.c
        source/mbed_shared_queues.cpp
)