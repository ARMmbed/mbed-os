# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE equeue.h)
target_sources(mbed-os PRIVATE Event.h)
target_sources(mbed-os PRIVATE EventQueue.h)
target_sources(mbed-os PRIVATE mbed_events.h)
target_sources(mbed-os PRIVATE mbed_shared_queues.h)
target_sources(mbed-os PRIVATE UserAllocatedEvent.h)

target_sources(mbed-os PRIVATE source/equeue.c)
target_sources(mbed-os PRIVATE source/equeue_mbed.cpp)
target_sources(mbed-os PRIVATE source/equeue_posix.c)
target_sources(mbed-os PRIVATE source/EventQueue.cpp)
target_sources(mbed-os PRIVATE source/mbed_shared_queues.cpp)

target_sources(mbed-os PRIVATE internal/equeue_platform.h)

target_include_directories(mbed-os PUBLIC .)
