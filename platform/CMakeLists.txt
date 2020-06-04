# SPDX-License-Identifier: Apache-2.0

# internal helper functions
function(_mbed_set_handler)
    if(${MBED_OS_TOOLCHAIN} STREQUAL "GCC ARM")
        target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/TOOLCHAIN_GCC/except.S)
    elseif(${MBED_OS_TOOLCHAIN} STREQUAL ARM)
        target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/TOOLCHAIN_ARM/except.S)
    elseif(${MBED_OS_TOOLCHAIN} STREQUAL IAR)
        target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/TOOLCHAIN_IAR/except.S)
        target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/TOOLCHAIN_IAR/cmain.S)
    endif()
endfunction()

target_sources(mbed-os PRIVATE
    ATCmdParser.h
    Callback.h
    CircularBuffer.h
    CMakeLists.txt
    CriticalSectionLock.h
    CThunk.h
    DeepSleepLock.h
    DirHandle.h
    FileBase.h
    FileHandle.h
    FileLike.h
    FilePath.h
    FileSystemHandle.h
    FileSystemLike.h
    LocalFileSystem.h
    mbed_application.h
    mbed_assert.h
    mbed_atomic.h
    mbed_critical.h
    mbed_debug.h
    mbed_error.h
    mbed_interface.h
    mbed_mem_trace.h
    mbed_mktime.h
    mbed_mpu_mgmt.h
    mbed_poll.h
    mbed_power_mgmt.h
    mbed_preprocessor.h
    mbed_retarget.h
    mbed_rtc_time.h
    mbed_semihost_api.h
    mbed_stats.h
    mbed_thread.h
    mbed_toolchain.h
    mbed_version.h
    mbed_wait_api.h
    NonCopyable.h
    platform.h
    PlatformMutex.h
    ScopedLock.h
    ScopedRamExecutionLock.h
    ScopedRomWriteLock.h
    SharedPtr.h
    SingletonPtr.h
    Span.h
    Stream.h
    Transaction.h

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
    source/mbed_crash_data_offsets.h
    source/mbed_critical.c
    source/mbed_error.c
    source/mbed_error_hist.c
    source/mbed_error_hist.h
    source/mbed_interface.c
    source/mbed_mem_trace.cpp
    source/mbed_mktime.c
    source/mbed_mpu_mgmt.c
    source/mbed_os_timer.cpp
    source/mbed_os_timer.h
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
    source/SysTimer.h
)

_mbed_set_handler()

# Currently only Cortex-M anyway, so this is hardcoded here
target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/mbed_fault_handler.c)
target_sources(mbed-os PRIVATE source/TARGET_CORTEX_M/mbed_fault_handler.h)

target_sources(mbed-os PRIVATE source/minimal-printf/mbed_printf_armlink_overrides.c)
target_sources(mbed-os PRIVATE source/minimal-printf/mbed_printf_implementation.c)
target_sources(mbed-os PRIVATE source/minimal-printf/mbed_printf_implementation.h)
target_sources(mbed-os PRIVATE source/minimal-printf/mbed_printf_wrapper.c)

target_sources(mbed-os PRIVATE cxxsupport)

target_include_directories(mbed-os PUBLIC .)
target_include_directories(mbed-os PUBLIC cxxsupport)
target_include_directories(mbed-os PUBLIC source)
