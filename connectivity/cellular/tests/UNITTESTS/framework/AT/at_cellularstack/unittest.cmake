
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  ../connectivity/cellular/tests/UNITTESTS/framework/common/util
  ../connectivity/cellular/include/cellular/framework/common
  ../connectivity/cellular/include/cellular/framework/AT
  ../platform/randlib/include/mbed-client-randlib
)

# Source files
set(unittest-sources
  ../connectivity/cellular/source/framework/AT/AT_CellularStack.cpp
  ../connectivity/libraries/nanostack-libservice/source/libip4string/ip4tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/ip6tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip4string/stoip4.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/stoip6.c
  ../connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c
  ../connectivity/netsocket/source/SocketAddress.cpp
)

# Test files
set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/at_cellularstacktest.cpp
  stubs/ATHandler_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/CellularUtil_stub.cpp
  stubs/CellularContext_stub.cpp
  stubs/AT_CellularContext_stub.cpp
  stubs/AT_CellularNetwork_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/NetworkInterface_stub.cpp
  stubs/CellularDevice_stub.cpp
  stubs/AT_CellularDevice_stub.cpp
  stubs/us_ticker_stub.cpp
  stubs/NetworkStack_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/ThisThread_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
  stubs/Semaphore_stub.cpp
)

set(unittest-test-flags
  -DDEVICE_SERIAL=1
  -DDEVICE_INTERRUPTIN=1
  -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
)
