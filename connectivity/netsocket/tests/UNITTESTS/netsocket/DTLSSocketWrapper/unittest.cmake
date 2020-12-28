
####################
# UNIT TESTS
####################

set(unittest-sources
  ../connectivity/netsocket/source/SocketAddress.cpp
  ../connectivity/netsocket/source/NetworkStack.cpp
  ../connectivity/netsocket/source/InternetSocket.cpp
  ../connectivity/netsocket/source/InternetDatagramSocket.cpp
  ../connectivity/netsocket/source/UDPSocket.cpp
  ../connectivity/netsocket/source/DTLSSocketWrapper.cpp
  ../connectivity/netsocket/source/TLSSocketWrapper.cpp
  ../connectivity/libraries/nanostack-libservice/source/libip4string/ip4tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/ip6tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip4string/stoip4.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/stoip6.c
  ../connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c
)

set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/test_DTLSSocketWrapper.cpp
  stubs/Mutex_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_critical_stub.c
  stubs/equeue_stub.c
  ../connectivity/nanostack/coap-service/test/coap-service/unittest/stub/mbedtls_stub.c
  stubs/EventQueue_stub.cpp
  stubs/mbed_shared_queues_stub.cpp
  stubs/nsapi_dns_stub.cpp
  stubs/EventFlags_stub.cpp
  stubs/stoip4_stub.c
  stubs/ip4tos_stub.c
  stubs/Kernel_stub.cpp
  stubs/SocketStats_Stub.cpp
)

set(MBEDTLS_USER_CONFIG_FILE_PATH "\"${CMAKE_CURRENT_LIST_DIR}/dtls_test_config.h\"")
set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/test_DTLSSocketWrapper.cpp PROPERTIES COMPILE_DEFINITIONS MBEDTLS_USER_CONFIG_FILE=${MBEDTLS_USER_CONFIG_FILE_PATH})
set_source_files_properties(../connectivity/netsocket/source/DTLSSocketWrapper.cpp PROPERTIES COMPILE_DEFINITIONS MBEDTLS_USER_CONFIG_FILE=${MBEDTLS_USER_CONFIG_FILE_PATH})
