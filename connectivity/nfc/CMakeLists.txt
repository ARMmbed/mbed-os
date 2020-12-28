# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(libraries)

target_include_directories(mbed-nfc
    INTERFACE
        .
        ./include
        ./include/nfc
        ./include/nfc/ndef
        ./include/nfc/ndef/common
)

target_sources(mbed-nfc
    INTERFACE
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
    INTERFACE
        MBED_CONF_NFC_PRESENT=1
)

target_link_libraries(mbed-nfc
    INTERFACE
        mbed-events
)
