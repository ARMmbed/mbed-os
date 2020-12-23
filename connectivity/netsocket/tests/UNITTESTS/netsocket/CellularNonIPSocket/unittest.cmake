
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
)

set(unittest-sources
  ../connectivity/netsocket/source/CellularNonIPSocket.cpp
)

set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/test_CellularNonIPSocket.cpp
  stubs/NetworkInterface_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/NetworkStack_stub.cpp
  stubs/EventFlags_stub.cpp
  stubs/Mutex_stub.cpp
  stubs/CellularContext_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/mbed_atomic_stub.c
)

set(unittest-test-flags
  -DMBED_CONF_CELLULAR_PRESENT=1
  -DDEVICE_SERIAL=1
  -DDEVICE_INTERRUPTIN=1
  -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
)
