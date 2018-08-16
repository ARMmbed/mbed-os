
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "features_netsocket_InternetSocket")

set(unittest-sources
  stubs/Mutex.cpp
  stubs/mbed_assert.c
  stubs/equeue.c
  stubs/EventQueue_stub.cpp
  stubs/mbed_shared_queues.cpp
  stubs/nsapi_dns.cpp
  stubs/EventFlags.cpp
  ../features/netsocket/SocketAddress.cpp
  ../features/netsocket/NetworkStack.cpp
  ../features/netsocket/InternetSocket.cpp
)

set(unittest-test-sources
  features/netsocket/InternetSocket/test_InternetSocket.cpp
)