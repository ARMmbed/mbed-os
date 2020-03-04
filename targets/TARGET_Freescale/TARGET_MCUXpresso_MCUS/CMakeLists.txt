# SPDX-License-Identifier: Apache-2.0

target_sources(mbed_os PUBLIC fsl_common.c)

target_sources(mbed_os PUBLIC api/analogin_api.c)
target_sources(mbed_os PUBLIC api/analogout_api.c)
target_sources(mbed_os PUBLIC api/dma_api.c)
target_sources(mbed_os PUBLIC api/dma_api_hal.h)
target_sources(mbed_os PUBLIC api/flash_api.c)
target_sources(mbed_os PUBLIC api/gpio_api.c)
target_sources(mbed_os PUBLIC api/gpio_irq_api.c)
target_sources(mbed_os PUBLIC api/gpio_object.h)
target_sources(mbed_os PUBLIC api/i2c_api.c)
target_sources(mbed_os PUBLIC api/lp_ticker.c)
target_sources(mbed_os PUBLIC api/objects.h)
target_sources(mbed_os PUBLIC api/PeripheralPins.h)
target_sources(mbed_os PUBLIC api/pinmap.c)
target_sources(mbed_os PUBLIC api/PortNames.h)
target_sources(mbed_os PUBLIC api/port_api.c)
target_sources(mbed_os PUBLIC api/qspi_api.c)
target_sources(mbed_os PUBLIC api/rtc_api.c)
target_sources(mbed_os PUBLIC api/sleep.c)

# TODO: select target in Freescale conditionally
add_subdirectory(TARGET_MCU_K64F)

target_include_directories(mbed_os PUBLIC api)
target_include_directories(mbed_os PUBLIC TARGET_MCU_K64F)
