
####################
# UNIT TESTS
####################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMBED_CONF_PLATFORM_CALLBACK_COMPARABLE")

# Source files
set(unittest-sources
  ../connectivity/netsocket/source/SocketAddress.cpp
  ../connectivity/netsocket/source/NetworkStack.cpp
  ../connectivity/netsocket/source/NetworkInterface.cpp
  ../connectivity/libraries/nanostack-libservice/source/libip4string/ip4tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/ip6tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip4string/stoip4.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/stoip6.c
  ../connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c
  ../connectivity/libraries/nanostack-libservice/source/libList/ns_list.c
)

# Test files
set(unittest-test-sources
  stubs/Mutex_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/equeue_stub.c
  stubs/EventQueue_stub.cpp
  stubs/mbed_shared_queues_stub.cpp
  stubs/nsapi_dns_stub.cpp
  stubs/EventFlags_stub.cpp
  ${CMAKE_CURRENT_LIST_DIR}/test_NetworkInterface.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/SocketStats_Stub.cpp
  stubs/mbed_error.c
)
