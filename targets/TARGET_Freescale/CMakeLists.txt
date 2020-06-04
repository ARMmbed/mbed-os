# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE mbed_rtx.h)
target_sources(mbed-os PRIVATE USBEndpoints_Kinetis.h)
target_sources(mbed-os PRIVATE USBPhyHw.h)
target_sources(mbed-os PRIVATE USBPhy_Kinetis.cpp)

mbed_add_cmake_directory("" "${MBED_OS_TARGET_LABELS}")

target_include_directories(mbed-os PUBLIC .)
