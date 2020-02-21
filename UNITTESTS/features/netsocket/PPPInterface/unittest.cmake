
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "features_netsocket_PPPInterface")

# Source files
set(unittest-sources
  ../features/netsocket/SocketAddress.cpp
  ../features/netsocket/PPPInterface.cpp
  ../features/netsocket/EMACInterface.cpp
  ../features/netsocket/NetworkInterface.cpp
  ../features/netsocket/NetworkStack.cpp
  ../features/frameworks/nanostack-libservice/source/libip4string/ip4tos.c
  ../features/frameworks/nanostack-libservice/source/libip6string/ip6tos.c
  ../features/frameworks/nanostack-libservice/source/libip4string/stoip4.c
  ../features/frameworks/nanostack-libservice/source/libip6string/stoip6.c
  ../features/frameworks/nanostack-libservice/source/libBits/common_functions.c
  ../features/frameworks/nanostack-libservice/source/libList/ns_list.c
)

# Test files
set(unittest-test-sources
  features/netsocket/PPPInterface/test_PPPInterface.cpp
  stubs/Mutex_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/equeue_stub.c
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/mbed_shared_queues_stub.cpp
  stubs/nsapi_dns_stub.cpp
  stubs/EventFlags_stub.cpp
  stubs/NetworkStack_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/SocketStats_Stub.cpp
  stubs/mbed_error.c
)
