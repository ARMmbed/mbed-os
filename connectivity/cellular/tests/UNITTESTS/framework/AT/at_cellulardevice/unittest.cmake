
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  ../connectivity/cellular/tests/UNITTESTS/framework/common/util
  ../connectivity/cellular/include/cellular/framework/common
  ../connectivity/cellular/include/cellular/framework/AT
  ../connectivity/cellular/include/cellular/framework/device
  ../platform/randlib/include/mbed-client-randlib
  ../drivers
  ../hal
)

# Source files
set(unittest-sources
  stubs/randLIB_stub.c
  ../connectivity/cellular/source/framework/AT/AT_CellularDevice.cpp
)

# Test files
set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/at_cellulardevicetest.cpp
  stubs/AT_CellularNetwork_stub.cpp
  stubs/ATHandler_stub.cpp
  stubs/AT_CellularSMS_stub.cpp
  stubs/AT_CellularInformation_stub.cpp
  stubs/CellularUtil_stub.cpp
  stubs/NetworkInterface_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/CellularDevice_stub.cpp
  stubs/NetworkStack_stub.cpp
  stubs/AT_CellularContext_stub.cpp
  stubs/Semaphore_stub.cpp
  stubs/BufferedSerial_stub.cpp
  stubs/SerialBase_stub.cpp
  stubs/CellularStateMachine_stub.cpp
  stubs/CellularContext_stub.cpp
  stubs/ThisThread_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
)

set(unittest-test-flags
  -DDEVICE_SERIAL=1
  -DDEVICE_INTERRUPTIN=1
  -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_APN=NULL
  -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_USERNAME=NULL
  -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_PASSWORD=NULL
  -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_PLMN=NULL
  -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_SIM_PIN=NULL
  -DMDMTXD=NC
  -DMDMRXD=NC
  -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
  -DMBED_CONF_CELLULAR_USE_SMS=1
)
