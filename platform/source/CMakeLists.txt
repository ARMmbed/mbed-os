# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")
add_subdirectory(minimal-printf)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        ATCmdParser.cpp
        CThunkBase.cpp
        CriticalSectionLock.cpp
        DeepSleepLock.cpp
        FileBase.cpp
        FileHandle.cpp
        FilePath.cpp
        FileSystemHandle.cpp
        LocalFileSystem.cpp
        Stream.cpp
        SysTimer.cpp
        mbed_alloc_wrappers.cpp
        mbed_application.c
        mbed_assert.c
        mbed_atomic_impl.c
        mbed_board.c
        mbed_critical.c
        mbed_error.c
        mbed_error_hist.c
        mbed_interface.c
        mbed_mem_trace.cpp
        mbed_mktime.c
        mbed_mpu_mgmt.c
        mbed_os_timer.cpp
        mbed_poll.cpp
        mbed_power_mgmt.c
        mbed_retarget.cpp
        mbed_rtc_time.cpp
        mbed_sdk_boot.c
        mbed_semihost_api.c
        mbed_stats.c
        mbed_thread.cpp
        mbed_wait_api_no_rtos.c
)
