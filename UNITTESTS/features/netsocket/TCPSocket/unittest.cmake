
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "features_netsocket_TCPSocket")

set(unittest-sources
  stubs/Mutex.cpp
  stubs/mbed_assert.c
  stubs/EventFlags.cpp
  ../features/netsocket/SocketAddress.cpp
  ../features/netsocket/InternetSocket.cpp
  ../features/netsocket/TCPSocket.cpp
)

set(unittest-test-sources
  features/netsocket/TCPSocket/test_TCPSocket.cpp
)