# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    equeue.h
    Event.h
    EventQueue.h
    mbed_events.h
    mbed_shared_queues.h
    UserAllocatedEvent.h

    source/equeue.c
    source/equeue_mbed.cpp
    source/equeue_posix.c
    source/EventQueue.cpp
    source/mbed_shared_queues.cpp
)

target_sources(mbed-os PRIVATE internal/equeue_platform.h)

target_include_directories(mbed-os PUBLIC .)
