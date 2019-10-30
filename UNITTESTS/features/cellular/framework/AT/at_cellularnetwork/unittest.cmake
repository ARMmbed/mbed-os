
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  features/cellular/framework/common/util
  ../features/cellular/framework/common
  ../features/cellular/framework/AT
  ../features/frameworks/mbed-client-randlib/mbed-client-randlib
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
  stubs/AT_CellularBase_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/us_ticker_stub.cpp
  stubs/mbed_assert_stub.c
  stubs/SocketAddress_stub.cpp
  stubs/randLIB_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
)
