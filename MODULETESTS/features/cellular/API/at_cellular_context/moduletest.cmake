
####################
# MODULE TESTS
####################

# Add test specific include paths
set(moduletest-includes ${moduletest-includes}
  features/cellular/framework/common/util
  ../features/cellular/framework/common
  ../features/cellular/framework/AT
  ../features/cellular/framework/device
  ../features/netsocket/cellular
)

# Source files
set(moduletest-sources
  ../features/cellular/framework/AT/AT_CellularContext.cpp
  ../features/cellular/framework/common/APN_db.cpp
  ../features/cellular/framework/common/CellularLog.cpp
  ../features/cellular/framework/common/CellularUtil.cpp
  ../features/cellular/framework/device/CellularContext.cpp
  ../features/cellular/framework/device/CellularDevice.cpp
  ../features/cellular/framework/device/CellularStateMachine.cpp
  ../features/cellular/framework/AT/AT_CellularBase.cpp
  ../features/cellular/framework/AT/AT_CellularContext.cpp
  ../features/cellular/framework/AT/AT_CellularDevice.cpp
  ../features/cellular/framework/AT/AT_CellularInformation.cpp
  ../features/cellular/framework/AT/AT_CellularNetwork.cpp
  ../features/cellular/framework/AT/AT_CellularSMS.cpp
  ../features/cellular/framework/AT/AT_CellularStack.cpp
  ../features/cellular/framework/AT/AT_ControlPlane_netif.cpp
  ../features/cellular/framework/AT/ATHandler.cpp
)

# Test files
set(moduletest-test-sources
  features/cellular/API/at_cellular_context/atcellularcontexttest.cpp
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
  stubs/SocketAddress_stub.cpp
  stubs/Thread_stub.cpp
  stubs/Mutex_stub.cpp
  stubs/ThisThread_stub.cpp
  stubs/Kernel_stub.cpp
  stubs/mbed_poll_stub.cpp
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMBED_CONF_CELLULAR_CONTROL_PLANE_OPT=0 -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200 -DMBED_CONF_CELLULAR_DEBUG_AT=0 -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_APN=0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMBED_CONF_CELLULAR_CONTROL_PLANE_OPT=0 -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200 -DMBED_CONF_CELLULAR_DEBUG_AT=0 -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_APN=0")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEVICE_SERIAL=1 -DDEVICE_INTERRUPTIN=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_SERIAL=1 -DDEVICE_INTERRUPTIN=1")

