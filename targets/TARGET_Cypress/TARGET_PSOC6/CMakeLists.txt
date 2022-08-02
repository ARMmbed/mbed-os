# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(COMPONENT_SCL EXCLUDE_FROM_ALL)

add_library(mbed-cat1a INTERFACE)

target_include_directories(mbed-cat1a
    INTERFACE
        mtb-pdl-cat1/devices/COMPONENT_CAT1A/include
        mtb-pdl-cat1/devices/COMPONENT_CAT1A/include/ip
)

add_subdirectory(TARGET_CY8CKIT064B0S2_4343W EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_CY8CKIT_062S2_43012 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_CY8CKIT_062_BLE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_CY8CKIT_062_WIFI_BT EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_CY8CPROTO_062S3_4343W EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_CY8CPROTO_062_4343W EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_CYSBSYSKIT_01 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_CYTFM_064B0S2_4343W EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_CYW9P62S1_43012EVB_01 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_CYW9P62S1_43438EVB_01 EXCLUDE_FROM_ALL)

add_subdirectory(abstraction-rtos EXCLUDE_FROM_ALL)

add_library(mbed-cm0p-bless INTERFACE)
target_sources(mbed-cm0p-bless
    INTERFACE
        psoc6cm0p/COMPONENT_CM0P_BLESS/psoc6_cm0p_bless.c
)

add_library(mbed-cm0p-crypto INTERFACE)
target_sources(mbed-cm0p-crypto
    INTERFACE
        psoc6cm0p/COMPONENT_CM0P_CRYPTO/psoc6_01_cm0p_crypto.c
        psoc6cm0p/COMPONENT_CM0P_CRYPTO/psoc6_02_cm0p_crypto.c
        psoc6cm0p/COMPONENT_CM0P_CRYPTO/psoc6_03_cm0p_crypto.c
        psoc6cm0p/COMPONENT_CM0P_CRYPTO/psoc6_04_cm0p_crypto.c
)

add_library(mbed-cm0p-secure INTERFACE)
target_sources(mbed-cm0p-secure
    INTERFACE
        psoc6cm0p/COMPONENT_CM0P_SECURE/psoc6_02_cm0p_secure.c
        psoc6cm0p/COMPONENT_CM0P_SECURE/psoc6_03_cm0p_secure.c
)

if("CM0P_SLEEP" IN_LIST MBED_TARGET_LABELS)
    add_library(mbed-cm0p-sleep INTERFACE)
    target_sources(mbed-cm0p-sleep
        INTERFACE
            psoc6cm0p/COMPONENT_CM0P_SLEEP/psoc6_01_cm0p_sleep.c
            psoc6cm0p/COMPONENT_CM0P_SLEEP/psoc6_02_cm0p_sleep.c
            psoc6cm0p/COMPONENT_CM0P_SLEEP/psoc6_03_cm0p_sleep.c
            psoc6cm0p/COMPONENT_CM0P_SLEEP/psoc6_04_cm0p_sleep.c
    )
endif()

add_library(mbed-udb-sdio-p12 INTERFACE)
target_include_directories(mbed-udb-sdio-p12
    INTERFACE
        udb-sdio-whd/COMPONENT_UDB_SDIO_P12
)
target_sources(mbed-udb-sdio-p12
    INTERFACE
        udb-sdio-whd/COMPONENT_UDB_SDIO_P12/SDIO_HOST_cfg.c
)

add_library(mbed-udb-sdio-p2 INTERFACE)
target_include_directories(mbed-udb-sdio-p2
    INTERFACE
        udb-sdio-whd/COMPONENT_UDB_SDIO_P2
)
target_sources(mbed-udb-sdio-p2
    INTERFACE
        udb-sdio-whd/COMPONENT_UDB_SDIO_P2/SDIO_HOST_cfg.c
)

add_library(mbed-udb-sdio-p9 INTERFACE)
target_include_directories(mbed-udb-sdio-p9
    INTERFACE
        udb-sdio-whd/COMPONENT_UDB_SDIO_P9
)
target_sources(mbed-udb-sdio-p9
    INTERFACE
        udb-sdio-whd/COMPONENT_UDB_SDIO_P9/SDIO_HOST_cfg.c
)

add_library(mbed-psoc6 INTERFACE)
target_include_directories(mbed-psoc6
    INTERFACE
        .
        udb-sdio-whd
        serial-flash
        core-lib/include
        mtb-hal-cat1/include
        mtb-hal-cat1/include/pin_packages
        mtb-hal-cat1/include/triggers
        mtb-pdl-cat1/devices/COMPONENT_CAT1A/include
        mtb-pdl-cat1/devices/COMPONENT_CAT1A/include/ip
        mtb-pdl-cat1/drivers/include
)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(ASSEMBLY_ROUTINES mtb-pdl-cat1/drivers/source/TOOLCHAIN_ARM/cy_syslib_mdk.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(ASSEMBLY_ROUTINES mtb-pdl-cat1/drivers/source/TOOLCHAIN_GCC_ARM/cy_syslib_gcc.S)
endif()

target_sources(mbed-psoc6
    INTERFACE
        cy_analogin_api.c
        cy_analogout_api.c
        cy_crc_api.c
        cy_flash_api.c
        cy_gpio_api.c
        cy_gpio_irq_api.c
        cy_i2c_api.c
        cy_lp_ticker_api.c
        cy_port_api.c
        cy_pwmout_api.c
        cy_qspi_api.c
        cy_reset_reason_api.c
        cy_rtc_api.c
        cy_serial_api.c
        cy_sleep_api.c
        cy_spi_api.c
        cy_trng_api.c
        cy_us_ticker_api.c
        cy_usb_phy.cpp
        cy_watchdog_api.c
        mbed_overrides.c
        pinmap.c

        serial-flash/cy_serial_flash_prog.c
        serial-flash/cy_serial_flash_qspi.c

        udb-sdio-whd/SDIO_HOST.c

        mtb-hal-cat1/source/cyhal_adc.c
        mtb-hal-cat1/source/cyhal_analog_common.c
        mtb-hal-cat1/source/cyhal_clock.c
        mtb-hal-cat1/source/cyhal_comp.c
        mtb-hal-cat1/source/cyhal_comp_ctb.c
        mtb-hal-cat1/source/cyhal_comp_lp.c
        mtb-hal-cat1/source/cyhal_crc.c
        mtb-hal-cat1/source/cyhal_crypto_common.c
        mtb-hal-cat1/source/cyhal_dac.c
        mtb-hal-cat1/source/cyhal_deprecated.c
        mtb-hal-cat1/source/cyhal_dma.c
        mtb-hal-cat1/source/cyhal_dma_dmac.c
        mtb-hal-cat1/source/cyhal_dma_dw.c
        mtb-hal-cat1/source/cyhal_ezi2c.c
        mtb-hal-cat1/source/cyhal_flash.c
        mtb-hal-cat1/source/cyhal_gpio.c
        mtb-hal-cat1/source/cyhal_hwmgr.c
        mtb-hal-cat1/source/cyhal_i2c.c
        mtb-hal-cat1/source/cyhal_i2s.c
        mtb-hal-cat1/source/cyhal_interconnect.c
        mtb-hal-cat1/source/cyhal_lptimer.c
        mtb-hal-cat1/source/cyhal_opamp.c
        mtb-hal-cat1/source/cyhal_pdmpcm.c
        mtb-hal-cat1/source/cyhal_pwm.c
        mtb-hal-cat1/source/cyhal_qspi.c
        mtb-hal-cat1/source/cyhal_quaddec.c
        mtb-hal-cat1/source/cyhal_rtc.c
        mtb-hal-cat1/source/cyhal_scb_common.c
        mtb-hal-cat1/source/cyhal_sdhc.c
        mtb-hal-cat1/source/cyhal_spi.c
        mtb-hal-cat1/source/cyhal_syspm.c
        mtb-hal-cat1/source/cyhal_system.c
        mtb-hal-cat1/source/cyhal_tcpwm_common.c
        mtb-hal-cat1/source/cyhal_timer.c
        mtb-hal-cat1/source/cyhal_trng.c
        mtb-hal-cat1/source/cyhal_uart.c
        mtb-hal-cat1/source/cyhal_udb_sdio.c
        mtb-hal-cat1/source/cyhal_usb_dev.c
        mtb-hal-cat1/source/cyhal_utils.c
        mtb-hal-cat1/source/cyhal_wdt.c

        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_01_104_m_csp_ble.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_01_104_m_csp_ble_usb.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_01_116_bga_ble.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_01_116_bga_usb.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_01_124_bga.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_01_124_bga_sip.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_01_43_smt.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_01_68_qfn_ble.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_01_80_wlcsp.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_02_100_wlcsp.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_02_124_bga.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_02_128_tqfp.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_02_68_qfn.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_03_100_tqfp.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_03_49_wlcsp.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_03_68_qfn.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_04_64_tqfp.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_04_68_qfn.c
        mtb-hal-cat1/source/pin_packages/cyhal_psoc6_04_80_tqfp.c

        mtb-hal-cat1/source/triggers/cyhal_triggers_psoc6_01.c
        mtb-hal-cat1/source/triggers/cyhal_triggers_psoc6_02.c
        mtb-hal-cat1/source/triggers/cyhal_triggers_psoc6_03.c
        mtb-hal-cat1/source/triggers/cyhal_triggers_psoc6_04.c

        mtb-pdl-cat1/devices/COMPONENT_CAT1A/source/cy_device.c
        mtb-pdl-cat1/drivers/source/cy_ble_clk.c
        mtb-pdl-cat1/drivers/source/cy_canfd.c
        mtb-pdl-cat1/drivers/source/cy_crypto.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_aes_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_aes_v2.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_cmac_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_cmac_v2.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_crc_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_crc_v2.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_des_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_des_v2.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_ecc_domain_params.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_ecc_ecdsa.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_ecc_key_gen.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_ecc_nist_p.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_hmac_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_hmac_v2.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_hw.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_hw_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_mem_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_mem_v2.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_prng_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_prng_v2.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_rsa.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_sha_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_sha_v2.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_trng_v1.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_trng_v2.c
        mtb-pdl-cat1/drivers/source/cy_crypto_core_vu.c
        mtb-pdl-cat1/drivers/source/cy_crypto_server.c
        mtb-pdl-cat1/drivers/source/cy_csd.c
        mtb-pdl-cat1/drivers/source/cy_ctb.c
        mtb-pdl-cat1/drivers/source/cy_ctdac.c
        mtb-pdl-cat1/drivers/source/cy_dma.c
        mtb-pdl-cat1/drivers/source/cy_dmac.c
        mtb-pdl-cat1/drivers/source/cy_efuse.c
        mtb-pdl-cat1/drivers/source/cy_flash.c
        mtb-pdl-cat1/drivers/source/cy_gpio.c
        mtb-pdl-cat1/drivers/source/cy_i2s.c
        mtb-pdl-cat1/drivers/source/cy_ipc_drv.c
        mtb-pdl-cat1/drivers/source/cy_ipc_pipe.c
        mtb-pdl-cat1/drivers/source/cy_ipc_sema.c
        mtb-pdl-cat1/drivers/source/cy_lpcomp.c
        mtb-pdl-cat1/drivers/source/cy_lvd.c
        mtb-pdl-cat1/drivers/source/cy_mcwdt.c
        mtb-pdl-cat1/drivers/source/cy_pdm_pcm.c
        mtb-pdl-cat1/drivers/source/cy_pra.c
        mtb-pdl-cat1/drivers/source/cy_pra_cfg.c
        mtb-pdl-cat1/drivers/source/cy_profile.c
        mtb-pdl-cat1/drivers/source/cy_prot.c
        mtb-pdl-cat1/drivers/source/cy_rtc.c
        mtb-pdl-cat1/drivers/source/cy_sar.c
        mtb-pdl-cat1/drivers/source/cy_scb_common.c
        mtb-pdl-cat1/drivers/source/cy_scb_ezi2c.c
        mtb-pdl-cat1/drivers/source/cy_scb_i2c.c
        mtb-pdl-cat1/drivers/source/cy_scb_spi.c
        mtb-pdl-cat1/drivers/source/cy_scb_uart.c
        mtb-pdl-cat1/drivers/source/cy_sd_host.c
        mtb-pdl-cat1/drivers/source/cy_seglcd.c
        mtb-pdl-cat1/drivers/source/cy_smartio.c
        mtb-pdl-cat1/drivers/source/cy_smif.c
        mtb-pdl-cat1/drivers/source/cy_smif_memslot.c
        mtb-pdl-cat1/drivers/source/cy_sysanalog.c
        mtb-pdl-cat1/drivers/source/cy_sysclk.c
        mtb-pdl-cat1/drivers/source/cy_sysint.c
        mtb-pdl-cat1/drivers/source/cy_syslib.c
        mtb-pdl-cat1/drivers/source/cy_syspm.c
        mtb-pdl-cat1/drivers/source/cy_systick.c
        mtb-pdl-cat1/drivers/source/cy_tcpwm_counter.c
        mtb-pdl-cat1/drivers/source/cy_tcpwm_pwm.c
        mtb-pdl-cat1/drivers/source/cy_tcpwm_quaddec.c
        mtb-pdl-cat1/drivers/source/cy_tcpwm_shiftreg.c
        mtb-pdl-cat1/drivers/source/cy_trigmux.c
        mtb-pdl-cat1/drivers/source/cy_usbfs_dev_drv.c
        mtb-pdl-cat1/drivers/source/cy_usbfs_dev_drv_io.c
        mtb-pdl-cat1/drivers/source/cy_usbfs_dev_drv_io_dma.c
        mtb-pdl-cat1/drivers/source/cy_wdt.c

        ${ASSEMBLY_ROUTINES}
)

target_link_libraries(mbed-psoc6 INTERFACE mbed-cmsis-cortex-m)

# TODO: Include only if mbed-baremetal is not used
if("CY_RTOS_AWARE" IN_LIST MBED_CONFIG_DEFINITIONS)
    target_link_libraries(mbed-psoc6 INTERFACE mbed-cy-rtos mbed-cy-rtx)
endif()
