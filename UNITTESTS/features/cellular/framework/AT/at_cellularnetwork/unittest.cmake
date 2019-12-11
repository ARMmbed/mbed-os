
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  features/cellular/framework/common/util
  ../features/cellular/framework/common
  ../features/cellular/framework/AT
  ../features/frameworks/mbed-client-randlib/mbed-client-randlib
  ../features/netsocket/cellular
)

# Source files
set(unittest-sources
  ../features/cellular/framework/AT/AT_CellularNetwork.cpp
  ../features/cellular/framework/common/CellularUtil.cpp
)

# Test files
set(unittest-test-sources
  features/cellular/framework/AT/at_cellularnetwork/at_cellularnetworktest.cpp
  stubs/ATHandler_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/us_ticker_stub.cpp
  stubs/CellularDevice_stub.cpp
  stubs/AT_CellularDevice_stub.cpp
  stubs/CellularContext_stub.cpp
  stubs/AT_CellularContext_stub.cpp
  stubs/NetworkInterface_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/SocketAddress_stub.cpp
  stubs/randLIB_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
  stubs/Semaphore_stub.cpp
)

set(unittest-test-flags
  -DDEVICE_SERIAL=1
  -DDEVICE_INTERRUPTIN=1
  -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
)
