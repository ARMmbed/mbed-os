# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE ConditionVariable.h)
target_sources(mbed-os PRIVATE EventFlags.h)
target_sources(mbed-os PRIVATE Kernel.h)
target_sources(mbed-os PRIVATE Mail.h)
target_sources(mbed-os PRIVATE mbed_rtos1_types.h)
target_sources(mbed-os PRIVATE mbed_rtos_storage.h)
target_sources(mbed-os PRIVATE mbed_rtos_types.h)
target_sources(mbed-os PRIVATE MemoryPool.h)
target_sources(mbed-os PRIVATE Mutex.h)
target_sources(mbed-os PRIVATE Queue.h)
target_sources(mbed-os PRIVATE rtos.h)
target_sources(mbed-os PRIVATE RtosTimer.h)
target_sources(mbed-os PRIVATE Semaphore.h)
target_sources(mbed-os PRIVATE ThisThread.h)
target_sources(mbed-os PRIVATE Thread.h)

target_sources(mbed-os PRIVATE source/ConditionVariable.cpp)
target_sources(mbed-os PRIVATE source/EventFlags.cpp)
target_sources(mbed-os PRIVATE source/Kernel.cpp)
target_sources(mbed-os PRIVATE source/Mutex.cpp)
target_sources(mbed-os PRIVATE source/RtosTimer.cpp)
target_sources(mbed-os PRIVATE source/rtos_handlers.h)
target_sources(mbed-os PRIVATE source/rtos_idle.h)
target_sources(mbed-os PRIVATE source/Semaphore.cpp)
target_sources(mbed-os PRIVATE source/ThisThread.cpp)
target_sources(mbed-os PRIVATE source/Thread.cpp)


add_subdirectory(source/TARGET_CORTEX)

target_include_directories(mbed-os PUBLIC .)

target_include_directories(mbed-os PRIVATE source)
