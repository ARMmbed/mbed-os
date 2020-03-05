# SPDX-License-Identifier: Apache-2.0

target_sources(mbed_os PRIVATE ConditionVariable.h)
target_sources(mbed_os PRIVATE EventFlags.h)
target_sources(mbed_os PRIVATE Kernel.h)
target_sources(mbed_os PRIVATE Mail.h)
target_sources(mbed_os PRIVATE mbed_rtos1_types.h)
target_sources(mbed_os PRIVATE mbed_rtos_storage.h)
target_sources(mbed_os PRIVATE mbed_rtos_types.h)
target_sources(mbed_os PRIVATE MemoryPool.h)
target_sources(mbed_os PRIVATE Mutex.h)
target_sources(mbed_os PRIVATE Queue.h)
target_sources(mbed_os PRIVATE rtos.h)
target_sources(mbed_os PRIVATE RtosTimer.h)
target_sources(mbed_os PRIVATE Semaphore.h)
target_sources(mbed_os PRIVATE ThisThread.h)
target_sources(mbed_os PRIVATE Thread.h)

target_sources(mbed_os PRIVATE source/ConditionVariable.cpp)
target_sources(mbed_os PRIVATE source/EventFlags.cpp)
target_sources(mbed_os PRIVATE source/Kernel.cpp)
target_sources(mbed_os PRIVATE source/Mutex.cpp)
target_sources(mbed_os PRIVATE source/RtosTimer.cpp)
target_sources(mbed_os PRIVATE source/rtos_handlers.h)
target_sources(mbed_os PRIVATE source/rtos_idle.h)
target_sources(mbed_os PRIVATE source/Semaphore.cpp)
target_sources(mbed_os PRIVATE source/ThisThread.cpp)
target_sources(mbed_os PRIVATE source/Thread.cpp)


add_subdirectory(source/TARGET_CORTEX)

target_include_directories(mbed_os PUBLIC .)

target_include_directories(mbed_os PRIVATE source)
