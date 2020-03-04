# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PUBLIC mbed_rtx.h)
target_sources(mbed-os PUBLIC USBEndpoints_Kinetis.h)
target_sources(mbed-os PUBLIC USBPhyHw.h)
target_sources(mbed-os PUBLIC USBPhy_Kinetis.cpp)

# TODO: select proper target conditionally
add_subdirectory(TARGET_MCUXpresso_MCUS)
