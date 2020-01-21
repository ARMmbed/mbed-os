
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
    /features/cellular/framework/device/cellulardevice
  ../features/cellular/framework/device
  ../features/cellular/framework/common
  ../features/netsocket/cellular
)

# Source files
set(unittest-sources
  ../features/cellular/framework/device/CellularContext.cpp
  ../features/frameworks/nanostack-libservice/source/libip4string/ip4tos.c
  ../features/frameworks/nanostack-libservice/source/libip6string/ip6tos.c
  ../features/frameworks/nanostack-libservice/source/libip4string/stoip4.c
  ../features/frameworks/nanostack-libservice/source/libip6string/stoip6.c
  ../features/frameworks/nanostack-libservice/source/libBits/common_functions.c
  ../features/netsocket/SocketAddress.cpp
)

# Test files
set(unittest-test-sources
  features/cellular/framework/device/cellularcontext/cellularcontexttest.cpp
  stubs/FileHandle_stub.cpp
  stubs/CellularStateMachine_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/BufferedSerial_stub.cpp
  stubs/SerialBase_stub.cpp
  stubs/ATHandler_stub.cpp
  stubs/AT_CellularNetwork_stub.cpp
  stubs/AT_CellularContext_stub.cpp
  stubs/AT_CellularDevice_stub.cpp
  stubs/Semaphore_stub.cpp
  stubs/NetworkInterface_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/CellularDevice_stub.cpp
  stubs/equeue_stub.c
  stubs/ThisThread_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
)

set(unittest-test-flags
  -DMDMRTS=PTC0
  -DMDMCTS=PTC1
  -DMDMTXD=NC
  -DMDMRXD=NC
  -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
  -DCELLULAR_DEVICE=myCellularDevice
  -DDEVICE_SERIAL_FC=1
  -DMBED_CONF_CELLULAR_CONTROL_PLANE_OPT=0
  -DDEVICE_SERIAL=1
  -DDEVICE_INTERRUPTIN=1
  -DMBED_CONF_CELLULAR_USE_SMS=1
)
