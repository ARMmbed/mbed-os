# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nfc STATIC EXCLUDE_FROM_ALL)

add_subdirectory(libraries)

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/TESTS)
    endif()
endif()

target_include_directories(mbed-nfc
    PUBLIC
        .
        ./include
        ./include/nfc
        ./include/nfc/ndef
        ./include/nfc/ndef/common
)

target_sources(mbed-nfc
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

target_compile_definitions(mbed-nfc
    PUBLIC
        MBED_CONF_NFC_PRESENT=1
)