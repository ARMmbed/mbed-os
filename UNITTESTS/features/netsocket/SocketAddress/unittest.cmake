
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "features_netsocket_SocketAddress")

# Source files
set(unittest-sources
  ../features/netsocket/SocketAddress.cpp
  ../connectivity/libraries/nanostack-libservice/source/libip4string/ip4tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/ip6tos.c
  ../connectivity/libraries/nanostack-libservice/source/libip4string/stoip4.c
  ../connectivity/libraries/nanostack-libservice/source/libip6string/stoip6.c
  ../connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c
)

# Test files
set(unittest-test-sources
  features/netsocket/SocketAddress/test_SocketAddress.cpp
)
