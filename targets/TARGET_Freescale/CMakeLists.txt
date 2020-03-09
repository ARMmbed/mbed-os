# SPDX-License-Identifier: Apache-2.0

target_sources(mbed_os PRIVATE mbed_rtx.h)
target_sources(mbed_os PRIVATE USBEndpoints_Kinetis.h)
target_sources(mbed_os PRIVATE USBPhyHw.h)
target_sources(mbed_os PRIVATE USBPhy_Kinetis.cpp)

mbed_os_add_cmake_directory("TARGET"
"${MBED_OS_TARGET_KEYS}")

target_include_directories(mbed_os PUBLIC .)
