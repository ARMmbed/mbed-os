# SPDX-License-Identifier: Apache-2.0

# internal helper functions
function(_mbed_set_handler)
    if(${MBED_OS_TOOLCHAIN} STREQUAL "GCC_ARM")
        target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/TOOLCHAIN_GCC/except.S)
    elseif(${MBED_OS_TOOLCHAIN} STREQUAL "ARM")
        target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/TOOLCHAIN_ARM/except.S)
    elseif(${MBED_OS_TOOLCHAIN} STREQUAL "IAR")
        target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/TOOLCHAIN_IAR/except.S)
        target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/TOOLCHAIN_IAR/cmain.S)
    endif()
endfunction()

target_sources(mbed-os
    PRIVATE
        source/ATCmdParser.cpp
        source/CriticalSectionLock.cpp
        source/CThunkBase.cpp
        source/DeepSleepLock.cpp
        source/FileBase.cpp
        source/FileHandle.cpp
        source/FilePath.cpp
        source/FileSystemHandle.cpp
        source/LocalFileSystem.cpp
        source/mbed_alloc_wrappers.cpp
        source/mbed_application.c
        source/mbed_assert.c
        source/mbed_atomic_impl.c
        source/mbed_board.c
        source/mbed_critical.c
        source/mbed_error.c
        source/mbed_error_hist.c
        source/mbed_interface.c
        source/mbed_mem_trace.cpp
        source/mbed_mktime.c
        source/mbed_mpu_mgmt.c
        source/mbed_os_timer.cpp
        source/mbed_poll.cpp
        source/mbed_power_mgmt.c
        source/mbed_retarget.cpp
        source/mbed_rtc_time.cpp
        source/mbed_sdk_boot.c
        source/mbed_semihost_api.c
        source/mbed_stats.c
        source/mbed_thread.cpp
        source/mbed_wait_api_no_rtos.c
        source/Stream.cpp
        source/SysTimer.cpp
)

_mbed_set_handler()

# Currently only Cortex-M anyway, so this is hardcoded here
target_sources(mbed-os
    PRIVATE
        cxxsupport/mstd_mutex.cpp
        source/TARGET_CORTEX_M/mbed_fault_handler.c
        source/minimal-printf/mbed_printf_armlink_overrides.c
        source/minimal-printf/mbed_printf_implementation.c
        source/minimal-printf/mbed_printf_wrapper.c
)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        cxxsupport
        internal
        source
        source/TARGET_CORTEX_M
        source/minimal-printf
)
