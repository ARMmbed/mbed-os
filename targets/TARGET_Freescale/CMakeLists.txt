# SPDX-License-Identifier: Apache-2.0

target_sources(mbed_os PRIVATE mbed_rtx.h)
target_sources(mbed_os PRIVATE USBEndpoints_Kinetis.h)
target_sources(mbed_os PRIVATE USBPhyHw.h)
target_sources(mbed_os PRIVATE USBPhy_Kinetis.cpp)

# TODO: select proper target conditionally
add_subdirectory(TARGET_MCUXpresso_MCUS)

target_include_directories(mbed_os PUBLIC .)
