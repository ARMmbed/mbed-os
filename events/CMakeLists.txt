# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PUBLIC equeue.h)
target_sources(mbed-os PUBLIC Event.h)
target_sources(mbed-os PUBLIC EventQueue.h)
target_sources(mbed-os PUBLIC mbed_events.h)
target_sources(mbed-os PUBLIC mbed_shared_queues.h)
target_sources(mbed-os PUBLIC UserAllocatedEvent.h)

target_sources(mbed-os PUBLIC sources/equeue.c)
target_sources(mbed-os PUBLIC sources/equeue_mbed.cpp)
target_sources(mbed-os PUBLIC sources/equeue_posix.c)
target_sources(mbed-os PUBLIC sources/EventQueue.cpp)
target_sources(mbed-os PUBLIC sources/mbed_shared_queues.cpp)

target_sources(mbed-os PRIVATE internal/equeue_platform.h)

