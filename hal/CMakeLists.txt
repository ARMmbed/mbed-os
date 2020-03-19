# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    analogin_api.h
    analogout_api.h
    buffer.h
    can_api.h
    can_helper.h
    crc_api.h
    critical_section_api.h
    dma_api.h
    ethernet_api.h
    flash_api.h
    gpio_api.h
    gpio_irq_api.h
    i2c_api.h
    itm_api.h
    LowPowerTickerWrapper.cpp
    LowPowerTickerWrapper.h
    lp_ticker_api.h
    mbed_compat.c
    mbed_critical_section_api.c
    mbed_flash_api.c
    mbed_gpio.c
    mbed_gpio_irq.c
    mbed_itm_api.c
    mbed_lp_ticker_api.c
    mbed_lp_ticker_wrapper.cpp
    mbed_lp_ticker_wrapper.h
    mbed_pinmap_common.c
    mbed_pinmap_default.cpp
    mbed_ticker_api.c
    mbed_us_ticker_api.c
    mpu_api.h
    pinmap.h
    port_api.h
    pwmout_api.h
    qspi_api.h
    reset_reason_api.h
    rtc_api.h
    serial_api.h
    sleep_api.h
    spi_api.h
    spm_api.h
    static_pinmap.cpp
    static_pinmap.h
    ticker_api.h
    trng_api.h
    us_ticker_api.h
    watchdog_api.h

    mpu/mbed_mpu_v7m.c
    mpu/mbed_mpu_v8m.c

    usb/USBPhy.h
    usb/USBPhyEvents.h
    usb/USBPhyTypes.h
    usb/usb_phy_api.h
)

target_include_directories(mbed-os PUBLIC .)
target_include_directories(mbed-os PUBLIC usb)


