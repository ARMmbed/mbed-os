# SPDX-License-Identifier: Apache-2.0

target_sources(${PROJECT_NAME} PUBLIC equeue.h)
target_sources(${PROJECT_NAME} PUBLIC Event.h)
target_sources(${PROJECT_NAME} PUBLIC EventQueue.h)
target_sources(${PROJECT_NAME} PUBLIC mbed_events.h)
target_sources(${PROJECT_NAME} PUBLIC mbed_shared_queues.h)
target_sources(${PROJECT_NAME} PUBLIC UserAllocatedEvent.h)

target_sources(${PROJECT_NAME} PUBLIC sources/equeue.c)
target_sources(${PROJECT_NAME} PUBLIC sources/equeue_mbed.cpp)
target_sources(${PROJECT_NAME} PUBLIC sources/equeue_posix.c)
target_sources(${PROJECT_NAME} PUBLIC sources/EventQueue.cpp)
target_sources(${PROJECT_NAME} PUBLIC sources/mbed_shared_queues.cpp)

target_sources(${PROJECT_NAME} PRIVATE internal/equeue_platform.h)

