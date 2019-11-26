
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "features_netsocket_nsapi_dns")

# Source files
set(unittest-sources
  ../features/netsocket/nsapi_dns.cpp
  ../features/frameworks/nanostack-libservice/source/libip4string/ip4tos.c
  ../features/frameworks/nanostack-libservice/source/libip6string/ip6tos.c
  ../features/frameworks/nanostack-libservice/source/libip4string/stoip4.c
  ../features/frameworks/nanostack-libservice/source/libip6string/stoip6.c
  ../features/frameworks/nanostack-libservice/source/libBits/common_functions.c  
  ../features/frameworks/nanostack-libservice/source/libList/ns_list.c
)

# Test files
set(unittest-test-sources
  stubs/Mutex_stub.cpp
  ../features/netsocket/SocketAddress.cpp
  stubs/mbed_assert_stub.cpp
  stubs/NetworkStack_stub.cpp
  stubs/NetworkInterfaceDefaults_stub.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_critical_stub.c
  stubs/mbed_rtos_rtx_stub.c
  stubs/Kernel_stub.cpp
  stubs/EventFlags_stub.cpp
  stubs/rtx_mutex_stub.c
  stubs/SocketStats_Stub.cpp
  features/netsocket/nsapi_dns/test_nsapi_dns.cpp
  ../features/netsocket/NetworkInterface.cpp
  ../features/netsocket/InternetSocket.cpp
  ../features/netsocket/InternetDatagramSocket.cpp
  ../features/netsocket/UDPSocket.cpp
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMBED_CONF_NSAPI_DNS_RESPONSE_WAIT_TIME=10000 -DMBED_CONF_NSAPI_DNS_RETRIES=1 -DMBED_CONF_NSAPI_DNS_TOTAL_ATTEMPTS=10 -DMBED_CONF_NSAPI_DNS_CACHE_SIZE=5")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMBED_CONF_NSAPI_DNS_RESPONSE_WAIT_TIME=10000 -DMBED_CONF_NSAPI_DNS_RETRIES=1 -DMBED_CONF_NSAPI_DNS_TOTAL_ATTEMPTS=10 -DMBED_CONF_NSAPI_DNS_CACHE_SIZE=5")