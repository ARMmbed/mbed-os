####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
    ../connectivity/FEATURE_BLE/include/ble
    ../connectivity/FEATURE_BLE/include
    ../connectivity
)

# Source files
set(unittest-sources
    ../connectivity/FEATURE_BLE/source/gap/AdvertisingDataBuilder.cpp
    ../connectivity/FEATURE_BLE/source/gap/AdvertisingParameters.cpp
    ../connectivity/FEATURE_BLE/source/gap/ConnectionParameters.cpp
    ../connectivity/FEATURE_BLE/source/gatt/DiscoveredCharacteristic.cpp
    ../connectivity/FEATURE_BLE/source/Gap.cpp
    ../connectivity/FEATURE_BLE/source/GattClient.cpp
    ../connectivity/FEATURE_BLE/source/GattServer.cpp
    ../connectivity/FEATURE_BLE/source/SecurityManager.cpp
)

# Test files
set(unittest-test-sources
    ${CMAKE_CURRENT_LIST_DIR}/ble_template.cpp
    stubs/BLE.cpp
)

set(unittest-test-flags
    -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
)
