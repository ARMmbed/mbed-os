
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
  ../features/cellular/framework/AT/AT_CellularSMS.cpp
)

# Test files
set(unittest-test-sources
  features/cellular/framework/AT/at_cellularsms/at_cellularsmstest.cpp
  stubs/ATHandler_stub.cpp
  stubs/AT_CellularBase_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/CellularUtil_stub.cpp
  stubs/us_ticker_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/ThisThread_stub.cpp
  stubs/mbed_wait_api_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMBED_CONF_CELLULAR_USE_SMS=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMBED_CONF_CELLULAR_USE_SMS=1")


