# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(acore)
add_subdirectory(stack)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/controllers
        ${CMAKE_CURRENT_SOURCE_DIR}/nfc
        ${CMAKE_CURRENT_SOURCE_DIR}/nfc/ndef
        ${CMAKE_CURRENT_SOURCE_DIR}/nfc/ndef/common
)

target_sources(mbed-os
    PRIVATE
        source/controllers/PN512Driver.cpp
        source/controllers/PN512SPITransportDriver.cpp
        source/controllers/PN512TransportDriver.cpp

        source/nfc/NFCController.cpp
        source/nfc/NFCControllerDriver.cpp
        source/nfc/NFCEEPROM.cpp
        source/nfc/NFCEEPROMDriver.cpp
        source/nfc/NFCNDEFCapable.cpp
        source/nfc/NFCRemoteEndpoint.cpp
        source/nfc/NFCRemoteInitiator.cpp
        source/nfc/NFCTarget.cpp
        source/nfc/Type4RemoteInitiator.cpp

        source/nfc/ndef/MessageBuilder.cpp
        source/nfc/ndef/MessageParser.cpp
        source/nfc/ndef/RecordParser.cpp

        source/nfc/ndef/common/Mime.cpp
        source/nfc/ndef/common/SimpleMessageParser.cpp
        source/nfc/ndef/common/Text.cpp
        source/nfc/ndef/common/URI.cpp
        source/nfc/ndef/common/util.cpp
)
