
####################
# UNIT TESTS
####################

set(unittest-sources
  ../connectivity/netsocket/source/SocketAddress.cpp
  ../connectivity/netsocket/source/NetworkInterface.cpp
  ../connectivity/netsocket/source/NetworkInterfaceDefaults.cpp
  ../connectivity/netsocket/source/NetworkStack.cpp #nsapi_create_stack
  ../connectivity/netsocket/source/InternetSocket.cpp
  ../connectivity/netsocket/source/TCPSocket.cpp
  ../connectivity/netsocket/source/InternetDatagramSocket.cpp
  ../connectivity/netsocket/source/UDPSocket.cpp
  ../connectivity/netsocket/source/SocketStats.cpp
  ../connectivity/netsocket/source/EthernetInterface.cpp
  ../connectivity/netsocket/source/EMACInterface.cpp
  ../connectivity/netsocket/source/nsapi_dns.cpp
  ../connectivity/libraries/nanostack-libservice/source/libip4string/ip4tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/ip6tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip4string/stoip4.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/stoip6.c
  ../connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c
  ../connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c
  ../connectivity/libraries/nanostack-libservice/source/libList/ns_list.c
)

set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/moduletest.cpp
  stubs/MeshInterface_stub.cpp
  stubs/CellularInterface_stub.cpp
  stubs/Mutex_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_critical_stub.c
  stubs/mbed_rtos_rtx_stub.c
  stubs/equeue_stub.c
  stubs/EventQueue_stub.cpp
  stubs/Kernel_stub.cpp
  stubs/mbed_error.c
  stubs/mbed_shared_queues_stub.cpp
  stubs/rtx_mutex_stub.c
  stubs/EventFlags_stub.cpp
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEVICE_EMAC -DMBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=ETHERNET -DMBED_CONF_NSAPI_DNS_RESPONSE_WAIT_TIME=10000 -DMBED_CONF_NSAPI_DNS_RETRIES=1 -DMBED_CONF_NSAPI_DNS_TOTAL_ATTEMPTS=10 -DMBED_CONF_NSAPI_DNS_CACHE_SIZE=5")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_EMAC -DMBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=ETHERNET -DMBED_CONF_NSAPI_DNS_RESPONSE_WAIT_TIME=10000 -DMBED_CONF_NSAPI_DNS_RETRIES=1 -DMBED_CONF_NSAPI_DNS_TOTAL_ATTEMPTS=10 -DMBED_CONF_NSAPI_DNS_CACHE_SIZE=5")