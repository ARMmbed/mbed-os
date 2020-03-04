# SPDX-License-Identifier: Apache-2.0

target_sources(${PROJECT_NAME} PUBLIC mbed_rtx.h)
target_sources(${PROJECT_NAME} PUBLIC USBEndpoints_Kinetis.h)
target_sources(${PROJECT_NAME} PUBLIC USBPhyHw.h)
target_sources(${PROJECT_NAME} PUBLIC USBPhy_Kinetis.cpp)

# TODO: select proper target conditionally
add_subdirectory(TARGET_MCUXpresso_MCUS)
