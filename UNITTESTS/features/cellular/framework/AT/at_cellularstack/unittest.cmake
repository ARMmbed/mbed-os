
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
  ../features/cellular/framework/AT/AT_CellularStack.cpp
  ../features/frameworks/nanostack-libservice/source/libip4string/ip4tos.c
  ../features/frameworks/nanostack-libservice/source/libip6string/ip6tos.c
  ../features/frameworks/nanostack-libservice/source/libip4string/stoip4.c
  ../features/frameworks/nanostack-libservice/source/libip6string/stoip6.c
  ../features/frameworks/nanostack-libservice/source/libBits/common_functions.c
  ../features/netsocket/SocketAddress.cpp
)

# Test files
set(unittest-test-sources
  features/cellular/framework/AT/at_cellularstack/at_cellularstacktest.cpp
  stubs/ATHandler_stub.cpp
  stubs/AT_CellularBase_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/CellularUtil_stub.cpp
  stubs/us_ticker_stub.cpp
  stubs/NetworkStack_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/ThisThread_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
)
