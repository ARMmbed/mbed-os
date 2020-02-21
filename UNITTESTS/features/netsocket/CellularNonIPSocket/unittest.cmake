
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  ../features/netsocket/cellular
)

set(unittest-sources
  ../features/netsocket/CellularNonIPSocket.cpp
)

set(unittest-test-sources
  features/netsocket/CellularNonIPSocket/test_CellularNonIPSocket.cpp
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
  -DDEVICE_SERIAL=1
  -DDEVICE_INTERRUPTIN=1
  -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
)
