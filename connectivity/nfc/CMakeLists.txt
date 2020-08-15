# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(libraries)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nfc
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nfc/ndef
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nfc/ndef/common
)

target_sources(mbed-os
    PRIVATE
        source/NFCController.cpp
        source/NFCControllerDriver.cpp
        source/NFCEEPROM.cpp
        source/NFCEEPROMDriver.cpp
        source/NFCNDEFCapable.cpp
        source/NFCRemoteEndpoint.cpp
        source/NFCRemoteInitiator.cpp
        source/NFCTarget.cpp
        source/Type4RemoteInitiator.cpp

        source/ndef/MessageBuilder.cpp
        source/ndef/MessageParser.cpp
        source/ndef/RecordParser.cpp

        source/ndef/common/Mime.cpp
        source/ndef/common/SimpleMessageParser.cpp
        source/ndef/common/Text.cpp
        source/ndef/common/URI.cpp
        source/ndef/common/util.cpp
)
