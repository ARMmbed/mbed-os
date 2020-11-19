# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("SCL" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_SCL EXCLUDE_FROM_ALL)
endif()

if("WHD" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_WHD EXCLUDE_FROM_ALL)
    add_subdirectory(common/COMPONENT_WHD EXCLUDE_FROM_ALL)
endif()

if("CY8CKIT064B0S2_4343W" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CY8CKIT064B0S2_4343W)
elseif("CY8CKIT_062S2_43012" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CY8CKIT_062S2_43012)
elseif("CY8CKIT_062_BLE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CY8CKIT_062_BLE)
elseif("CY8CKIT_062_WIFI_BT" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CY8CKIT_062_WIFI_BT)
elseif("CY8CPROTO_062S3_4343W" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CY8CPROTO_062S3_4343W)
elseif("CY8CPROTO_062_4343W" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CY8CPROTO_062_4343W)
elseif("CYSBSYSKIT_01" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CYSBSYSKIT_01)
elseif("CYTFM_064B0S2_4343W" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CYTFM_064B0S2_4343W)
elseif("CYW9P62S1_43012EVB_01" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CYW9P62S1_43012EVB_01)
elseif("CYW9P62S1_43438EVB_01" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CYW9P62S1_43438EVB_01)
endif()

add_subdirectory(psoc6csp/abstraction/rtos EXCLUDE_FROM_ALL)

if("CM0P_BLESS" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-core
        INTERFACE
            psoc6cm0p/COMPONENT_CM0P_BLESS/psoc6_cm0p_bless.c
    )
endif()

if("CM0P_CRYPTO" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-core
        INTERFACE
            psoc6cm0p/COMPONENT_CM0P_CRYPTO/psoc6_01_cm0p_crypto.c
            psoc6cm0p/COMPONENT_CM0P_CRYPTO/psoc6_02_cm0p_crypto.c
            psoc6cm0p/COMPONENT_CM0P_CRYPTO/psoc6_03_cm0p_crypto.c
            psoc6cm0p/COMPONENT_CM0P_CRYPTO/psoc6_04_cm0p_crypto.c
    )
endif()

if("CM0P_SECURE" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-core
        INTERFACE
            psoc6cm0p/COMPONENT_CM0P_SECURE/psoc6_02_cm0p_secure.c
            psoc6cm0p/COMPONENT_CM0P_SECURE/psoc6_03_cm0p_secure.c
    )
endif()

if("CM0P_SLEEP" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-core
        INTERFACE
            psoc6cm0p/COMPONENT_CM0P_SLEEP/psoc6_01_cm0p_sleep.c
            psoc6cm0p/COMPONENT_CM0P_SLEEP/psoc6_02_cm0p_sleep.c
            psoc6cm0p/COMPONENT_CM0P_SLEEP/psoc6_03_cm0p_sleep.c
            psoc6cm0p/COMPONENT_CM0P_SLEEP/psoc6_04_cm0p_sleep.c
    )
endif()

if("UDB_SDIO_P12" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            common/udb-sdio-whd/COMPONENT_UDB_SDIO_P12
    )

    target_sources(mbed-core
        INTERFACE
            common/udb-sdio-whd/COMPONENT_UDB_SDIO_P12/SDIO_HOST_cfg.c
    )
elseif("UDB_SDIO_P2" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            common/udb-sdio-whd/COMPONENT_UDB_SDIO_P2
    )

    target_sources(mbed-core
        INTERFACE
            common/udb-sdio-whd/COMPONENT_UDB_SDIO_P2/SDIO_HOST_cfg.c
    )
elseif("UDB_SDIO_P9" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            common/udb-sdio-whd/COMPONENT_UDB_SDIO_P9
    )

    target_sources(mbed-core
        INTERFACE
            common/udb-sdio-whd/COMPONENT_UDB_SDIO_P9/SDIO_HOST_cfg.c
    )
endif()

target_include_directories(mbed-core
    INTERFACE
        .
        common
        common/udb-sdio-whd
        psoc6csp/abstraction/resource/include
        psoc6csp/core_lib/include
        psoc6csp/hal/include
        psoc6csp/hal/include/pin_packages
        psoc6csp/hal/include/triggers
        psoc6pdl/devices/include
        psoc6pdl/devices/include/ip
        psoc6pdl/drivers/include
)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(ASSEMBLY_ROUTINES psoc6pdl/drivers/source/TOOLCHAIN_ARM/cy_syslib_mdk.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(ASSEMBLY_ROUTINES psoc6pdl/drivers/source/TOOLCHAIN_GCC_ARM/cy_syslib_gcc.S)
endif()

target_sources(mbed-core
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

        common/cy_serial_flash_prog.c
        common/cy_serial_flash_qspi.c

        common/udb-sdio-whd/SDIO_HOST.c

        psoc6csp/abstraction/resource/source/cyabs_resource.c

        psoc6csp/hal/source/cyhal_adc.c
        psoc6csp/hal/source/cyhal_analog_common.c
        psoc6csp/hal/source/cyhal_clock.c
        psoc6csp/hal/source/cyhal_crc.c
        psoc6csp/hal/source/cyhal_crypto_common.c
        psoc6csp/hal/source/cyhal_dac.c
        psoc6csp/hal/source/cyhal_deprecated.c
        psoc6csp/hal/source/cyhal_dma.c
        psoc6csp/hal/source/cyhal_dma_dmac.c
        psoc6csp/hal/source/cyhal_dma_dw.c
        psoc6csp/hal/source/cyhal_ezi2c.c
        psoc6csp/hal/source/cyhal_flash.c
        psoc6csp/hal/source/cyhal_gpio.c
        psoc6csp/hal/source/cyhal_hwmgr.c
        psoc6csp/hal/source/cyhal_i2c.c
        psoc6csp/hal/source/cyhal_i2s.c
        psoc6csp/hal/source/cyhal_interconnect.c
        psoc6csp/hal/source/cyhal_lptimer.c
        psoc6csp/hal/source/cyhal_not_implemented.c
        psoc6csp/hal/source/cyhal_pdmpcm.c
        psoc6csp/hal/source/cyhal_pwm.c
        psoc6csp/hal/source/cyhal_qspi.c
        psoc6csp/hal/source/cyhal_rtc.c
        psoc6csp/hal/source/cyhal_scb_common.c
        psoc6csp/hal/source/cyhal_sdhc.c
        psoc6csp/hal/source/cyhal_spi.c
        psoc6csp/hal/source/cyhal_syspm.c
        psoc6csp/hal/source/cyhal_system.c
        psoc6csp/hal/source/cyhal_tcpwm_common.c
        psoc6csp/hal/source/cyhal_timer.c
        psoc6csp/hal/source/cyhal_trng.c
        psoc6csp/hal/source/cyhal_uart.c
        psoc6csp/hal/source/cyhal_udb_sdio.c
        psoc6csp/hal/source/cyhal_usb_dev.c
        psoc6csp/hal/source/cyhal_utils.c
        psoc6csp/hal/source/cyhal_wdt.c

        psoc6csp/hal/source/pin_packages/cyhal_psoc6_01_104_m_csp_ble.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_01_104_m_csp_ble_usb.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_01_116_bga_ble.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_01_116_bga_usb.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_01_124_bga.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_01_124_bga_sip.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_01_43_smt.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_01_68_qfn_ble.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_01_80_wlcsp.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_02_100_wlcsp.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_02_124_bga.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_02_128_tqfp.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_02_68_qfn.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_03_100_tqfp.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_03_49_wlcsp.c
        psoc6csp/hal/source/pin_packages/cyhal_psoc6_03_68_qfn.c

        psoc6csp/hal/source/triggers/cyhal_triggers_psoc6_01.c
        psoc6csp/hal/source/triggers/cyhal_triggers_psoc6_02.c
        psoc6csp/hal/source/triggers/cyhal_triggers_psoc6_03.c

        psoc6pdl/drivers/source/cy_ble_clk.c
        psoc6pdl/drivers/source/cy_canfd.c
        psoc6pdl/drivers/source/cy_crypto.c
        psoc6pdl/drivers/source/cy_crypto_core_aes_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_aes_v2.c
        psoc6pdl/drivers/source/cy_crypto_core_cmac_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_cmac_v2.c
        psoc6pdl/drivers/source/cy_crypto_core_crc_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_crc_v2.c
        psoc6pdl/drivers/source/cy_crypto_core_des_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_des_v2.c
        psoc6pdl/drivers/source/cy_crypto_core_ecc_domain_params.c
        psoc6pdl/drivers/source/cy_crypto_core_ecc_ecdsa.c
        psoc6pdl/drivers/source/cy_crypto_core_ecc_key_gen.c
        psoc6pdl/drivers/source/cy_crypto_core_ecc_nist_p.c
        psoc6pdl/drivers/source/cy_crypto_core_hmac_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_hmac_v2.c
        psoc6pdl/drivers/source/cy_crypto_core_hw.c
        psoc6pdl/drivers/source/cy_crypto_core_hw_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_mem_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_mem_v2.c
        psoc6pdl/drivers/source/cy_crypto_core_prng_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_prng_v2.c
        psoc6pdl/drivers/source/cy_crypto_core_rsa.c
        psoc6pdl/drivers/source/cy_crypto_core_sha_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_sha_v2.c
        psoc6pdl/drivers/source/cy_crypto_core_trng_v1.c
        psoc6pdl/drivers/source/cy_crypto_core_trng_v2.c
        psoc6pdl/drivers/source/cy_crypto_core_vu.c
        psoc6pdl/drivers/source/cy_crypto_server.c
        psoc6pdl/drivers/source/cy_csd.c
        psoc6pdl/drivers/source/cy_ctb.c
        psoc6pdl/drivers/source/cy_ctdac.c
        psoc6pdl/drivers/source/cy_device.c
        psoc6pdl/drivers/source/cy_dma.c
        psoc6pdl/drivers/source/cy_dmac.c
        psoc6pdl/drivers/source/cy_efuse.c
        psoc6pdl/drivers/source/cy_flash.c
        psoc6pdl/drivers/source/cy_gpio.c
        psoc6pdl/drivers/source/cy_i2s.c
        psoc6pdl/drivers/source/cy_ipc_drv.c
        psoc6pdl/drivers/source/cy_ipc_pipe.c
        psoc6pdl/drivers/source/cy_ipc_sema.c
        psoc6pdl/drivers/source/cy_lpcomp.c
        psoc6pdl/drivers/source/cy_lvd.c
        psoc6pdl/drivers/source/cy_mcwdt.c
        psoc6pdl/drivers/source/cy_pdm_pcm.c
        psoc6pdl/drivers/source/cy_pra.c
        psoc6pdl/drivers/source/cy_pra_cfg.c
        psoc6pdl/drivers/source/cy_profile.c
        psoc6pdl/drivers/source/cy_prot.c
        psoc6pdl/drivers/source/cy_rtc.c
        psoc6pdl/drivers/source/cy_sar.c
        psoc6pdl/drivers/source/cy_scb_common.c
        psoc6pdl/drivers/source/cy_scb_ezi2c.c
        psoc6pdl/drivers/source/cy_scb_i2c.c
        psoc6pdl/drivers/source/cy_scb_spi.c
        psoc6pdl/drivers/source/cy_scb_uart.c
        psoc6pdl/drivers/source/cy_sd_host.c
        psoc6pdl/drivers/source/cy_seglcd.c
        psoc6pdl/drivers/source/cy_smartio.c
        psoc6pdl/drivers/source/cy_smif.c
        psoc6pdl/drivers/source/cy_smif_memslot.c
        psoc6pdl/drivers/source/cy_sysanalog.c
        psoc6pdl/drivers/source/cy_sysclk.c
        psoc6pdl/drivers/source/cy_sysint.c
        psoc6pdl/drivers/source/cy_syslib.c
        psoc6pdl/drivers/source/cy_syspm.c
        psoc6pdl/drivers/source/cy_systick.c
        psoc6pdl/drivers/source/cy_tcpwm_counter.c
        psoc6pdl/drivers/source/cy_tcpwm_pwm.c
        psoc6pdl/drivers/source/cy_tcpwm_quaddec.c
        psoc6pdl/drivers/source/cy_trigmux.c
        psoc6pdl/drivers/source/cy_usbfs_dev_drv.c
        psoc6pdl/drivers/source/cy_usbfs_dev_drv_io.c
        psoc6pdl/drivers/source/cy_usbfs_dev_drv_io_dma.c
        psoc6pdl/drivers/source/cy_wdt.c

        ${ASSEMBLY_ROUTINES}
)
