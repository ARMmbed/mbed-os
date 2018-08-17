
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "features_netsocket_TCPSocket")

set(unittest-sources
  ../features/netsocket/SocketAddress.cpp
  ../features/netsocket/InternetSocket.cpp
  ../features/netsocket/TCPSocket.cpp
)

set(unittest-test-sources
  stubs/Mutex_stub.cpp
  stubs/mbed_assert_stub.c
  stubs/EventFlags_stub.cpp
  features/netsocket/TCPSocket/test_TCPSocket.cpp
)
