# SPDX-License-Identifier: Apache-2.0

target_sources(mbed_os PUBLIC ConditionVariable.h)
target_sources(mbed_os PUBLIC EventFlags.h)
target_sources(mbed_os PUBLIC Kernel.h)
target_sources(mbed_os PUBLIC Mail.h)
target_sources(mbed_os PUBLIC mbed_rtos1_types.h)
target_sources(mbed_os PUBLIC mbed_rtos_storage.h)
target_sources(mbed_os PUBLIC mbed_rtos_types.h)
target_sources(mbed_os PUBLIC MemoryPool.h)
target_sources(mbed_os PUBLIC Mutex.h)
target_sources(mbed_os PUBLIC Queue.h)
target_sources(mbed_os PUBLIC rtos.h)
target_sources(mbed_os PUBLIC RtosTimer.h)
target_sources(mbed_os PUBLIC Semaphore.h)
target_sources(mbed_os PUBLIC ThisThread.h)
target_sources(mbed_os PUBLIC Thread.h)

target_sources(mbed_os PUBLIC source/ConditionVariable.cpp)
target_sources(mbed_os PUBLIC source/EventFlags.cpp)
target_sources(mbed_os PUBLIC source/Kernel.cpp)
target_sources(mbed_os PUBLIC source/Mutex.cpp)
target_sources(mbed_os PUBLIC source/RtosTimer.cpp)
target_sources(mbed_os PUBLIC source/rtos_handlers.h)
target_sources(mbed_os PUBLIC source/rtos_idle.h)
target_sources(mbed_os PUBLIC source/Semaphore.cpp)
target_sources(mbed_os PUBLIC source/ThisThread.cpp)
target_sources(mbed_os PUBLIC source/Thread.cpp)


add_subdirectory(source/TARGET_CORTEX)

target_include_directories(mbed_os PUBLIC .)

target_include_directories(mbed_os PRIVATE source)
