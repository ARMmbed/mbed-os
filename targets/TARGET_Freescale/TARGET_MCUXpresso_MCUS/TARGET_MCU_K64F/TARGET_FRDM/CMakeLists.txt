# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE crc.c)
target_sources(mbed-os PRIVATE crc.h)
target_sources(mbed-os PRIVATE device.h)
target_sources(mbed-os PRIVATE fsl_clock_config.c)
target_sources(mbed-os PRIVATE fsl_clock_config.h)
target_sources(mbed-os PRIVATE fsl_phy.c)
target_sources(mbed-os PRIVATE fsl_phy.h)
target_sources(mbed-os PRIVATE mbed_overrides.c)
target_sources(mbed-os PRIVATE PeripheralNames.h)
target_sources(mbed-os PRIVATE PeripheralPinMaps.h)
target_sources(mbed-os PRIVATE PeripheralPins.c)
target_sources(mbed-os PRIVATE PinNames.h)

target_include_directories(mbed-os PUBLIC .)
