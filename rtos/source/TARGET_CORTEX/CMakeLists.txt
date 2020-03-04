# SPDX-License-Identifier: Apache-2.0

target_sources(${PROJECT_NAME} PUBLIC mbed_boot.c)
target_sources(${PROJECT_NAME} PUBLIC mbed_boot.h)
target_sources(${PROJECT_NAME} PUBLIC mbed_rtos_rtx.c)
target_sources(${PROJECT_NAME} PUBLIC mbed_rtx_conf.h)
target_sources(${PROJECT_NAME} PUBLIC mbed_rtx_handlers.c)
target_sources(${PROJECT_NAME} PUBLIC mbed_rtx_idle.cpp)
target_sources(${PROJECT_NAME} PUBLIC mbed_rtx_storage.h)

# TODO: rtx5 should have own CMake?

target_sources(${PROJECT_NAME} PUBLIC rtx5/Include/cmsis_os2.h)
target_sources(${PROJECT_NAME} PUBLIC rtx5/Include/os_tick.h)

# TODO: Cortex-A missing here
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Config/RTX_Config.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Config/RTX_Config.h)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Include/rtx_evr.h)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Include/rtx_os.h)

# TODO: toolchain selection
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/TOOLCHAIN_GCC/TARGET_RTOS_M4_M7/irq_cm4f.S)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/TOOLCHAIN_GCC/TARGET_RTOS_M4_M7/irq_cm4f.S)

target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_core_ca.h)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_core_cm.h)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_delay.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_evflags.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_evr.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_kernel.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_lib.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_lib.h)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_memory.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_mempool.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_msgqueue.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_mutex.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_semaphore.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_system.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_thread.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/RTX/Source/rtx_timer.c)

target_sources(${PROJECT_NAME} PUBLIC rtx5/Source/os_systick.c)
target_sources(${PROJECT_NAME} PUBLIC rtx5/Source/os_tick_ptim.c)

# TODO: toolchain selection
target_sources(${PROJECT_NAME} PUBLIC TOOLCHAIN_GCC_ARM mbed_boot_gcc_arm.c)
