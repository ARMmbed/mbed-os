# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    mbed_rtx.h
    USBEndpoints_Kinetis.h
    USBPhyHw.h
    USBPhy_Kinetis.cpp
)

mbed_add_cmake_directory_if_labels("")

target_include_directories(mbed-os PUBLIC .)
