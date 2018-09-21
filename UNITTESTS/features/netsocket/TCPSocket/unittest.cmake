
####################
# UNIT TESTS
####################

set(unittest-sources
  ../features/netsocket/SocketAddress.cpp
  ../features/netsocket/InternetSocket.cpp
  ../features/netsocket/TCPSocket.cpp
  ../features/frameworks/nanostack-libservice/source/libip4string/ip4tos.c
  ../features/frameworks/nanostack-libservice/source/libip4string/stoip4.c
)

set(unittest-test-sources
  features/netsocket/TCPSocket/test_TCPSocket.cpp
  stubs/Mutex_stub.cpp
  stubs/mbed_assert_stub.c
  stubs/EventFlags_stub.cpp
  stubs/stoip4_stub.c
  stubs/ip4tos_stub.c
)
