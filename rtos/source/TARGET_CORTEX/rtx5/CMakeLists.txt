# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE Include/cmsis_os2.h)
target_sources(mbed-os PRIVATE Include/os_tick.h)

# TODO: Cortex-A missing here
target_sources(mbed-os PRIVATE RTX/Config/RTX_Config.c)
target_sources(mbed-os PRIVATE RTX/Config/RTX_Config.h)
target_sources(mbed-os PRIVATE RTX/Include/rtx_evr.h)
target_sources(mbed-os PRIVATE RTX/Include/rtx_os.h)

if(${MBED_OS_TOOLCHAIN} STREQUAL "GCC ARM")
add_subdirectory(mbed-os PRIVATE RTX/Source/TOOLCHAIN_GCC)
elif(${MBED_OS_TOOLCHAIN} STREQUAL "ARM")
add_subdirectory(mbed-os PRIVATE RTX/Source/TOOLCHAIN_ARM)
elif(${MBED_OS_TOOLCHAIN} STREQUAL "IAR")
add_subdirectory(mbed-os PRIVATE RTX/Source/TOOLCHAIN_IAR)
endif()

target_sources(mbed-os PRIVATE RTX/Source/rtx_core_ca.h)
target_sources(mbed-os PRIVATE RTX/Source/rtx_core_cm.h)
target_sources(mbed-os PRIVATE RTX/Source/rtx_delay.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_evflags.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_evr.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_kernel.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_lib.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_lib.h)
target_sources(mbed-os PRIVATE RTX/Source/rtx_memory.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_mempool.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_msgqueue.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_mutex.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_semaphore.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_system.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_thread.c)
target_sources(mbed-os PRIVATE RTX/Source/rtx_timer.c)

target_sources(mbed-os PRIVATE Source/os_systick.c)
target_sources(mbed-os PRIVATE Source/os_tick_ptim.c)

target_include_directories(mbed-os PUBLIC Include)
target_include_directories(mbed-os PUBLIC RTX/Include)
target_include_directories(mbed-os PUBLIC RTX/Source)
target_include_directories(mbed-os PUBLIC RTX/Config)
