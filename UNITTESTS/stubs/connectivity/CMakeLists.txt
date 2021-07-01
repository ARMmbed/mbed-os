# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-connectivity)

target_compile_definitions(mbed-stubs-connectivity
    PRIVATE
        DEVICE_SERIAL=1
        DEVICE_INTERRUPTIN=1
        MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
        MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION=true
        MBED_CONF_LORA_AUTOMATIC_UPLINK_MESSAGE=true
        MBED_CONF_LORA_TX_MAX_SIZE=255
        MBED_CONF_CELLULAR_AT_HANDLER_BUFFER_SIZE=32
        MDMTXD=NC
        MDMRXD=NC
)

target_sources(mbed-stubs-connectivity
    PRIVATE
        aes_stub.c
        CellularInterface_stub.cpp
        cipher_stub.c
        cmac_stub.c
        ip4tos_stub.c
        LoRaMacChannelPlan_stub.cpp
        LoRaMacCommand_stub.cpp
        LoRaMacCrypto_stub.cpp
        LoRaMac_stub.cpp
        LoRaPHYEU868_stub.cpp
        LoRaPHY_stub.cpp
        LoRaWANStack_stub.cpp
        LoRaWANTimer_stub.cpp
        MeshInterface_stub.cpp
        NetworkInterfaceDefaults_stub.cpp
        NetworkInterface_stub.cpp
        NetworkStack_stub.cpp
        nsapi_dns_stub.cpp
        SocketAddress_stub.cpp
        SocketStats_Stub.cpp
        stoip4_stub.c
        ${mbed-os_SOURCE_DIR}/connectivity/nanostack/coap-service/test/coap-service/unittest/stub/mbedtls_stub.c
)

target_link_libraries(mbed-stubs-connectivity
    PRIVATE
        mbed-stubs-rtos-headers
        mbed-headers
        mbed-stubs-headers
        mbed-stubs-rtos
        gtest
)
