# SPDX-License-Identifier: Apache-2.0

target_sources(mbed_os PRIVATE equeue.h)
target_sources(mbed_os PRIVATE Event.h)
target_sources(mbed_os PRIVATE EventQueue.h)
target_sources(mbed_os PRIVATE mbed_events.h)
target_sources(mbed_os PRIVATE mbed_shared_queues.h)
target_sources(mbed_os PRIVATE UserAllocatedEvent.h)

target_sources(mbed_os PRIVATE source/equeue.c)
target_sources(mbed_os PRIVATE source/equeue_mbed.cpp)
target_sources(mbed_os PRIVATE source/equeue_posix.c)
target_sources(mbed_os PRIVATE source/EventQueue.cpp)
target_sources(mbed_os PRIVATE source/mbed_shared_queues.cpp)

target_sources(mbed_os PRIVATE internal/equeue_platform.h)

target_include_directories(mbed_os PUBLIC .)
