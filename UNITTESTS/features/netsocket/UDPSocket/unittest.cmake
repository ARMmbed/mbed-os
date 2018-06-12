
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "features_netsocket_UDPSocket")

set(unittest-sources
  stubs/Mutex.cpp
  stubs/mbed_assert.c
  stubs/equeue.c
  stubs/EventQueue_stub.cpp
  stubs/mbed_shared_queues.cpp
  stubs/EventFlags.cpp
  stubs/nsapi_dns.cpp
  ../features/netsocket/SocketAddress.cpp
  ../features/netsocket/NetworkStack.cpp
  ../features/netsocket/InternetSocket.cpp
  ../features/netsocket/UDPSocket.cpp
)

set(unittest-test-sources
  features/netsocket/UDPSocket/test_UDPSocket.cpp
)