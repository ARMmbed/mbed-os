#-------------------------------------------------------------------------------
# Copyright (c) 2020-2023 Macronix International Co. LTD. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0079 NEW)

if (CRYPTO_MBEDTLS AND CRYPTO_PSA)
    Message(FATAL_ERROR "CRYPTO_MBEDTLS and CRYPTO_PSA both enabled!")
endif()
if (NOT CRYPTO_MBEDTLS AND NOT CRYPTO_PSA)
    Message(FATAL_ERROR "CRYPTO_MBEDTLS and CRYPTO_PSA both disabled!")
endif()
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libmx78_armor_lib.a)
    Message(FATAL_ERROR "libmx78_armor_lib.a should be download and put under current directory")
else()
    add_library(mx78_armor_lib STATIC IMPORTED)
    set_target_properties(mx78_armor_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libmx78_armor_lib.a)
    set_target_properties(mx78_armor_lib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_library(mx78_armor STATIC)
target_include_directories(mx78_armor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_compile_definitions(mx78_armor
    PRIVATE
        $<$<BOOL:${SECUREFLASH_PROVISION}>:SECUREFLASH_PROVISION>
        ETSS_PROV_DEVELOPER_MODE=${ETSS_PROV_DEVELOPER_MODE}
        $<$<BOOL:${MULTI_CLIENT_ISOLATION}>:MULTI_CLIENT_ISOLATION>
        $<$<BOOL:${SECUREFLASH_DEBUG}>:SECUREFLASH_DEBUG>
        $<$<BOOL:${MX78_ARMOR_TEST_MODE}>:MX78_ARMOR_TEST_MODE>
        $<$<BOOL:${CRYPTO_MBEDTLS}>:CRYPTO_MBEDTLS>
        $<$<BOOL:${CRYPTO_PSA}>:CRYPTO_PSA>
)

target_sources(mx78_armor
    PRIVATE
        mx78_armor.c
        $<$<BOOL:${CRYPTO_PSA}>:${CMAKE_CURRENT_SOURCE_DIR}/crypto_wrapper/crypto_wrapper_psa.c>
        $<$<BOOL:${CRYPTO_MBEDTLS}>:${CMAKE_CURRENT_SOURCE_DIR}/crypto_wrapper/crypto_wrapper_mbedtls.c>
)

target_link_libraries(mx78_armor
    PRIVATE
        psa_interface
        platform_s
        tfm_sprt
        secureflash_platform
        spi_nor_driver
        mx78_armor_lib
    INTERFACE
        platform_common_interface
)

target_link_libraries(jedec_security_hal
    PRIVATE
        mx78_armor
)
if(SECUREFLASH_PROVISION)
target_include_directories(mx78_armor
    PRIVATE
        provisioning
)


if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/provisioning/libmx78_armor_provision_lib.a)
    Message(FATAL_ERROR "libmx78_armor_provision_lib.a should be download and put under provisioning directory")
else()
    add_library(mx78_armor_provision_lib STATIC IMPORTED)
    set_target_properties(mx78_armor_provision_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/provisioning/libmx78_armor_provision_lib.a)
    set_target_properties(mx78_armor_provision_lib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/provisioning)
endif()

target_link_libraries(mx78_armor
    PRIVATE
        mx78_armor_provision_lib
)
endif()
