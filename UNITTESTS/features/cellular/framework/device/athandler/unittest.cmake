
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
  ../features/cellular/framework/device/ATHandler.cpp
)

# Test files
set(unittest-test-sources
  features/cellular/framework/device/athandler/athandlertest.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/us_ticker_stub.cpp
  stubs/BufferedSerial_stub.cpp
  stubs/SerialBase_stub.cpp
  stubs/mbed_assert_stub.cpp
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

set(unittest-test-flags
  -DMBED_CONF_CELLULAR_DEBUG_AT=true
  -DOS_STACK_SIZE=2048
  -DDEVICE_SERIAL=1
  -DDEVICE_INTERRUPTIN=1
  -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
)
