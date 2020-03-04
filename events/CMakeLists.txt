# SPDX-License-Identifier: Apache-2.0

target_sources(mbed_os PUBLIC equeue.h)
target_sources(mbed_os PUBLIC Event.h)
target_sources(mbed_os PUBLIC EventQueue.h)
target_sources(mbed_os PUBLIC mbed_events.h)
target_sources(mbed_os PUBLIC mbed_shared_queues.h)
target_sources(mbed_os PUBLIC UserAllocatedEvent.h)

target_sources(mbed_os PUBLIC source/equeue.c)
target_sources(mbed_os PUBLIC source/equeue_mbed.cpp)
target_sources(mbed_os PUBLIC source/equeue_posix.c)
target_sources(mbed_os PUBLIC source/EventQueue.cpp)
target_sources(mbed_os PUBLIC source/mbed_shared_queues.cpp)

target_sources(mbed_os PRIVATE internal/equeue_platform.h)

