
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "features_netsocket_TCPServer")

set(unittest-sources
  ../features/netsocket/SocketAddress.cpp
  ../features/netsocket/NetworkStack.cpp
  ../features/netsocket/InternetSocket.cpp
  ../features/netsocket/TCPSocket.cpp
  ../features/netsocket/TCPServer.cpp
  ../features/frameworks/nanostack-libservice/source/libip4string/ip4tos.c
  ../features/frameworks/nanostack-libservice/source/libip6string/ip6tos.c
  ../features/frameworks/nanostack-libservice/source/libip4string/stoip4.c
  ../features/frameworks/nanostack-libservice/source/libip6string/stoip6.c
  ../features/frameworks/nanostack-libservice/source/libBits/common_functions.c  
)

set(unittest-test-sources
  stubs/Mutex_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_critical_stub.c
  stubs/equeue_stub.c
  stubs/EventQueue_stub.cpp
  stubs/mbed_error.c
  stubs/mbed_shared_queues_stub.cpp
  stubs/nsapi_dns_stub.cpp
  stubs/EventFlags_stub.cpp
  features/netsocket/TCPServer/test_TCPServer.cpp
  stubs/SocketStats_Stub.cpp
)
