
####################
# UNIT TESTS
####################

set(unittest-sources
  ../features/netsocket/SocketAddress.cpp
  ../features/netsocket/NetworkInterface.cpp
  ../features/netsocket/NetworkInterfaceDefaults.cpp
  ../features/netsocket/NetworkStack.cpp #nsapi_create_stack
  ../features/netsocket/InternetSocket.cpp
  ../features/netsocket/TCPSocket.cpp
  ../features/netsocket/InternetDatagramSocket.cpp
  ../features/netsocket/UDPSocket.cpp
  ../features/netsocket/SocketStats.cpp
  ../features/netsocket/EthernetInterface.cpp
  ../features/netsocket/EMACInterface.cpp
  ../features/netsocket/nsapi_dns.cpp
  ../features/frameworks/nanostack-libservice/source/libip4string/ip4tos.c
  ../features/frameworks/nanostack-libservice/source/libip6string/ip6tos.c
  ../features/frameworks/nanostack-libservice/source/libip4string/stoip4.c
  ../features/frameworks/nanostack-libservice/source/libip6string/stoip6.c
  ../features/frameworks/nanostack-libservice/source/libBits/common_functions.c
  ../features/frameworks/nanostack-libservice/source/libBits/common_functions.c
  ../features/frameworks/nanostack-libservice/source/libList/ns_list.c
)

set(unittest-test-sources
  moduletests/features/netsocket/IfaceDnsSocket/moduletest.cpp
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