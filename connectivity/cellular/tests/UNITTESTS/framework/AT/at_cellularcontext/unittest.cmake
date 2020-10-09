
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  ../connectivity/cellular/tests/UNITTESTS/framework/common/util
  ../connectivity/cellular/include/cellular/framework/common
  ../connectivity/cellular/include/cellular/framework/AT
  ../connectivity/cellular/include/cellular/framework/device
)

# Source files
set(unittest-sources
  ../connectivity/cellular/source/framework/AT/AT_CellularContext.cpp
)

# Test files
set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/at_cellularcontexttest.cpp
  stubs/ATHandler_stub.cpp
  stubs/AT_CellularDevice_stub.cpp
  stubs/AT_CellularStack_stub.cpp
  stubs/AT_CellularNetwork_stub.cpp
  stubs/AT_ControlPlane_netif_stub.cpp
  stubs/CellularDevice_stub.cpp
  stubs/CellularStateMachine_stub.cpp
  stubs/equeue_stub.c
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/NetworkInterface_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/NetworkStack_stub.cpp
  stubs/randLIB_stub.cpp
  stubs/Semaphore_stub.cpp
  stubs/us_ticker_stub.cpp
  stubs/BufferedSerial_stub.cpp
  stubs/SerialBase_stub.cpp
  stubs/CellularContext_stub.cpp
  stubs/CellularUtil_stub.cpp
  stubs/SocketAddress_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
)

set(unittest-test-flags
  -DDEVICE_SERIAL=1
  -DDEVICE_INTERRUPTIN=1
  -DMBED_CONF_CELLULAR_USE_SMS=1
  -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_APN=NULL
  -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
)
