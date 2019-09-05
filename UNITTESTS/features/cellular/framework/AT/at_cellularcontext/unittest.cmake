
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  features/cellular/framework/common/util
  ../features/cellular/framework/common
  ../features/cellular/framework/AT
  ../features/cellular/framework/device
  ../features/netsocket/cellular
)

# Source files
set(unittest-sources
  ../features/cellular/framework/AT/AT_CellularContext.cpp
)

# Test files
set(unittest-test-sources
  features/cellular/framework/AT/at_cellularcontext/at_cellularcontexttest.cpp
  stubs/ATHandler_stub.cpp
  stubs/AT_CellularBase_stub.cpp
  stubs/AT_CellularDevice_stub.cpp
  stubs/AT_CellularStack_stub.cpp
  stubs/AT_CellularNetwork_stub.cpp
  stubs/CellularDevice_stub.cpp
  stubs/CellularStateMachine_stub.cpp
  stubs/equeue_stub.c
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/mbed_assert_stub.c
  stubs/NetworkInterface_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/NetworkStack_stub.cpp
  stubs/randLIB_stub.cpp
  stubs/Semaphore_stub.cpp
  stubs/us_ticker_stub.cpp
  stubs/UARTSerial_stub.cpp
  stubs/SerialBase_stub.cpp
  stubs/CellularContext_stub.cpp
  stubs/CellularUtil_stub.cpp
  stubs/SocketAddress_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
)
