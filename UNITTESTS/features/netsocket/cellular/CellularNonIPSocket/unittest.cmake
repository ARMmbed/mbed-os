
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  ../features/netsocket/cellular
)

set(unittest-sources
  ../features/netsocket/cellular/CellularNonIPSocket.cpp
)

set(unittest-test-sources
  features/netsocket/cellular/CellularNonIPSocket/test_CellularNonIPSocket.cpp
  stubs/NetworkInterface_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/NetworkStack_stub.cpp
  stubs/EventFlags_stub.cpp
  stubs/Mutex_stub.cpp
  stubs/CellularContext_stub.cpp
  stubs/mbed_assert_stub.c
)
