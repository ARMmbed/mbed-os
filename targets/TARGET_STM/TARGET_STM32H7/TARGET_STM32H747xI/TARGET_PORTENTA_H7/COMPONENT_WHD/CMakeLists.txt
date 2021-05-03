# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy-psoc6-whd-43012 INTERFACE)
target_include_directories(mbed-cy-psoc6-whd-43012
    INTERFACE
        resources/firmware/COMPONENT_43012
)
target_sources(mbed-cy-psoc6-whd-43012 
    INTERFACE
        resources/firmware/COMPONENT_43012/43012C0-mfgtest_bin.c
        resources/firmware/COMPONENT_43012/43012C0-mfgtest_clm_blob.c
        resources/firmware/COMPONENT_43012/43012C0_bin.c
        resources/firmware/COMPONENT_43012/43012C0_clm_blob.c
)

add_library(mbed-cy-psoc6-whd-43438 INTERFACE)
target_include_directories(mbed-cy-psoc6-whd-43438
    INTERFACE
        resources/firmware/COMPONENT_43438
)
target_sources(mbed-cy-psoc6-whd-43438 
    INTERFACE
        resources/firmware/COMPONENT_43438/43438A1-mfgtest_bin.c
        resources/firmware/COMPONENT_43438/43438A1-mfgtest_clm_blob.c
        resources/firmware/COMPONENT_43438/43438A1_bin.c
        resources/firmware/COMPONENT_43438/43438A1_clm_blob.c
)

add_library(mbed-cy-psoc6-whd-4343w INTERFACE)
target_include_directories(mbed-cy-psoc6-whd-4343w
    INTERFACE
        resources/firmware/COMPONENT_4343W
)
target_sources(mbed-cy-psoc6-whd-4343w 
    INTERFACE
        resources/firmware/COMPONENT_4343W/4343WA1-mfgtest_bin.c
        resources/firmware/COMPONENT_4343W/4343WA1-mfgtest_clm_blob.c
        resources/firmware/COMPONENT_4343W/4343WA1_bin.c
        resources/firmware/COMPONENT_4343W/4343WA1_clm_blob.c
)

add_library(mbed-cy8ckit-062s2-43012-whd INTERFACE)
target_include_directories(mbed-cy8ckit-062s2-43012-whd INTERFACE resources/nvram/TARGET_CY8CKIT_062S2_43012)

add_library(mbed-cy8ckit-062s2-4343w-whd INTERFACE)
target_include_directories(mbed-cy8ckit-062s2-4343w-whd INTERFACE resources/nvram/TARGET_CY8CKIT_062S2_4343W)

add_library(mbed-cy8ckit-062-4343w-whd INTERFACE)
target_include_directories(mbed-cy8ckit-062-4343w-whd INTERFACE resources/nvram/TARGET_CY8CKIT_062_4343W)

add_library(mbed-cy8ckit-062-wifi-bt-whd INTERFACE)
target_include_directories(mbed-cy8ckit-062-wifi-bt-whd INTERFACE resources/nvram/TARGET_CY8CKIT_062_WIFI_BT)

add_library(mbed-cy8ckit-064s1-4343w-whd INTERFACE)
target_include_directories(mbed-cy8ckit-064s1-4343w-whd INTERFACE resources/nvram/TARGET_CY8CKIT_064S1_4343W)

add_library(mbed-cy8ckit-064s2-4343w-whd INTERFACE)
target_include_directories(mbed-cy8ckit-064s2-4343w-whd INTERFACE resources/nvram/TARGET_CY8CKIT_064S2_4343W)

add_library(mbed-cy8cmod-062s2-43012-whd INTERFACE)
target_include_directories(mbed-cy8cmod-062s2-43012-whd INTERFACE resources/nvram/TARGET_CY8CMOD_062S2_43012)

add_library(mbed-cy8cmod-062s3-4343w-whd INTERFACE)
target_include_directories(mbed-cy8cmod-062s3-4343w-whd INTERFACE resources/nvram/TARGET_CY8CMOD_062S3_4343W)

add_library(mbed-cy8cmod-062-4343w-whd INTERFACE)
target_include_directories(mbed-cy8cmod-062-4343w-whd INTERFACE resources/nvram/TARGET_CY8CMOD_062_4343W)

add_library(mbed-cy8cproto-062s3-4343w-whd INTERFACE)
target_include_directories(mbed-cy8cproto-062s3-4343w-whd INTERFACE resources/nvram/TARGET_CY8CPROTO_062S3_4343W)

add_library(mbed-cy8cproto-062-4343w-whd INTERFACE)
target_include_directories(mbed-cy8cproto-062-4343w-whd INTERFACE resources/nvram/TARGET_CY8CPROTO_062_4343W)

add_library(mbed-cytfm-064b0s2-4343w-whd INTERFACE)
target_include_directories(mbed-cytfm-064b0s2-4343w-whd INTERFACE resources/nvram/TARGET_CYTFM_064B0S2_4343W)

add_library(mbed-cyw943012p6evb-01-whd INTERFACE)
target_include_directories(mbed-cyw943012p6evb-01-whd INTERFACE resources/nvram/TARGET_CYW943012P6EVB_01)

add_library(mbed-cyw943012wcd2-whd INTERFACE)
target_include_directories(mbed-cyw943012wcd2-whd INTERFACE resources/nvram/TARGET_CYW943012WCD2)

add_library(mbed-cyw9p62s1-43012car-01-whd INTERFACE)
target_include_directories(mbed-cyw9p62s1-43012car-01-whd INTERFACE resources/nvram/TARGET_CYW9P62S1_43012CAR_01)

add_library(mbed-cyw9p62s1-43012evb-01-whd INTERFACE)
target_include_directories(mbed-cyw9p62s1-43012evb-01-whd INTERFACE resources/nvram/TARGET_CYW9P62S1_43012EVB_01)

add_library(mbed-cyw9p62s1-43438evb-01-whd INTERFACE)
target_include_directories(mbed-cyw9p62s1-43438evb-01-whd INTERFACE resources/nvram/TARGET_CYW9P62S1_43438EVB_01)

add_library(mbed-cy8ckit-064b0s2-4343w-whd INTERFACE)
target_include_directories(mbed-cy8ckit-064b0s2-4343w-whd INTERFACE resources/nvram/TARGET_CY8CKIT_064B0S2_4343W)



target_include_directories(mbed-cy-psoc6-whd
    INTERFACE
        .
        resources/resource_imp
)

target_sources(mbed-cy-psoc6-whd
    INTERFACE
        resources/resource_imp/whd_resources.c

)

