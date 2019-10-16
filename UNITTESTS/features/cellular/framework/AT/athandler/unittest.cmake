
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  ../platform
  ../features/cellular/framework/common/util
  ../features/cellular/framework/common
  ../features/cellular/framework/AT
  ../features/frameworks/mbed-client-randlib/mbed-client-randlib
  
)

# Source files
set(unittest-sources
  ../features/cellular/framework/AT/ATHandler.cpp
)

# Test files
set(unittest-test-sources
  features/cellular/framework/AT/athandler/athandlertest.cpp
  stubs/AT_CellularBase_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/us_ticker_stub.cpp
  stubs/mbed_assert_stub.c
  stubs/mbed_poll_stub.cpp
  stubs/Timer_stub.cpp
  stubs/equeue_stub.c
  stubs/Kernel_stub.cpp
  stubs/ThisThread_stub.cpp
  stubs/randLIB_stub.cpp
  stubs/CellularUtil_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
  stubs/mbed_rtos_rtx_stub.c
  stubs/rtx_mutex_stub.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMBED_CONF_CELLULAR_DEBUG_AT=true -DOS_STACK_SIZE=2048")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMBED_CONF_CELLULAR_DEBUG_AT=true -DOS_STACK_SIZE=2048")
