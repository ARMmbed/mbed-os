# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy_psoc6_whd INTERFACE)

if("43012" IN_LIST MBED_TARGET_LABELS)
    set(MFGTEST_BIN_C resources/firmware/COMPONENT_43012/43012C0-mfgtest_bin.c)
    set(MFGTEST_CLM_BLOB_C resources/firmware/COMPONENT_43012/43012C0-mfgtest_clm_blob.c)
    set(BIN_C resources/firmware/COMPONENT_43012/43012C0_bin.c)
    set(CLM_BLOB_C resources/firmware/COMPONENT_43012/43012C0_clm_blob.c)
    set(RESOURCE_INC_DIR resources/firmware/COMPONENT_43012)
elseif("43438" IN_LIST MBED_TARGET_LABELS)
    set(MFGTEST_BIN_C resources/firmware/COMPONENT_43438/43438A1-mfgtest_bin.c)
    set(MFGTEST_CLM_BLOB_C resources/firmware/COMPONENT_43438/43438A1-mfgtest_clm_blob.c)
    set(BIN_C resources/firmware/COMPONENT_43438/43438A1_bin.c)
    set(CLM_BLOB_C resources/firmware/COMPONENT_43438/43438A1_clm_blob.c)
    set(RESOURCE_INC_DIR resources/firmware/COMPONENT_43438)
elseif("4343W" IN_LIST MBED_TARGET_LABELS)
    set(MFGTEST_BIN_C resources/firmware/COMPONENT_4343W/4343WA1-mfgtest_bin.c)
    set(MFGTEST_CLM_BLOB_C resources/firmware/COMPONENT_4343W/4343WA1-mfgtest_clm_blob.c)
    set(BIN_C resources/firmware/COMPONENT_4343W/4343WA1_bin.c)
    set(CLM_BLOB_C resources/firmware/COMPONENT_4343W/4343WA1_clm_blob.c)
    set(RESOURCE_INC_DIR resources/firmware/COMPONENT_4343W)
endif()

if("CY8CKIT064B0S2_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CKIT064B0S2_4343W)
elseif("CY8CKIT_062S2_43012" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CKIT_062S2_43012)
elseif("CY8CKIT_062S2_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CKIT_062S2_4343W)
elseif("CY8CKIT_062_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CKIT_062_4343W)
elseif("CY8CKIT_062_WIFI_BT" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CKIT_062_WIFI_BT)
elseif("CY8CKIT_064S1_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CKIT_064S1_4343W)
elseif("CY8CKIT_064S2_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CKIT_064S2_4343W)
elseif("CY8CMOD_062S2_43012" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CMOD_062S2_43012)
elseif("CY8CMOD_062S3_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CMOD_062S3_4343W)
elseif("CY8CMOD_062_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CMOD_062_4343W)
elseif("CY8CPROTO_062S3_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CPROTO_062S3_4343W)
elseif("CY8CPROTO_062_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CY8CPROTO_062_4343W)
elseif("CYTFM_064B0S2_4343W" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CYTFM_064B0S2_4343W)
elseif("CYW943012P6EVB_01" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CYW943012P6EVB_01)
elseif("CYW943012WCD2" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CYW943012WCD2)
elseif("CYW9P62S1_43012CAR_01" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CYW9P62S1_43012CAR_01)
elseif("CYW9P62S1_43012EVB_01" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CYW9P62S1_43012EVB_01)
elseif("CYW9P62S1_43438EVB_01" IN_LIST MBED_TARGET_LABELS)
    set(WIFI_NVRAM_IMAGE_INC_DIR resources/nvram/TARGET_CYW9P62S1_43438EVB_01)
endif()

target_include_directories(mbed-cy_psoc6_whd
    INTERFACE
        .
        inc
        resources/resource_imp
        src
        src/include
        src/bus_protocols
        ${RESOURCE_INC_DIR}
        ${WIFI_NVRAM_IMAGE_INC_DIR}
)

target_sources(mbed-cy_psoc6_whd
    INTERFACE
        resources/resource_imp/whd_resources.c

        src/whd_ap.c
        src/whd_buffer_api.c
        src/whd_cdc_bdc.c
        src/whd_chip.c
        src/whd_chip_constants.c
        src/whd_clm.c
        src/whd_debug.c
        src/whd_events.c
        src/whd_logging.c
        src/whd_management.c
        src/whd_network_if.c
        src/whd_resource_if.c
        src/whd_sdpcm.c
        src/whd_thread.c
        src/whd_utils.c
        src/whd_wifi.c
        src/whd_wifi_api.c
        src/whd_wifi_p2p.c

        src/bus_protocols/whd_bus.c
        src/bus_protocols/whd_bus_common.c
        src/bus_protocols/whd_bus_sdio_protocol.c
        src/bus_protocols/whd_bus_spi_protocol.c

        ${MFGTEST_BIN_C}
        ${MFGTEST_CLM_BLOB_C}
        ${BIN_C}
        ${CLM_BLOB_C}
)

target_compile_definitions(mbed-cy_psoc6_whd
    INTERFACE
        MBED_CONF_CY_PSOC6_WHD_PRESENT=1
)
