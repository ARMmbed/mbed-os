# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    ConditionVariable.h
    EventFlags.h
    Kernel.h
    Mail.h
    mbed_rtos1_types.h
    mbed_rtos_storage.h
    mbed_rtos_types.h
    MemoryPool.h
    Mutex.h
    Queue.h
    rtos.h
    Semaphore.h
    ThisThread.h
    Thread.h
    source/ConditionVariable.cpp
    source/EventFlags.cpp
    source/Kernel.cpp
    source/Mutex.cpp
    source/rtos_handlers.h
    source/rtos_idle.h
    source/Semaphore.cpp
    source/ThisThread.cpp
    source/Thread.cpp
)


add_subdirectory(source/TARGET_CORTEX)

target_include_directories(mbed-os PUBLIC .)

target_include_directories(mbed-os PRIVATE source)
