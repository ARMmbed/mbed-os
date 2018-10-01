
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "features_netsocket_SocketAddress")

# We want to get rid of ip6string.h fake include to use the real definitions.
list(REMOVE_ITEM unittest-includes "${PROJECT_SOURCE_DIR}/target_h")

# Source files
set(unittest-sources
  ../features/netsocket/SocketAddress.cpp
  ../features/frameworks/nanostack-libservice/source/libip4string/ip4tos.c
  ../features/frameworks/nanostack-libservice/source/libip4string/stoip4.c
# Adding real ip6-related features, to test the relevant functions fully.
  ../features/frameworks/nanostack-libservice/source/libip6string/ip6tos.c
  ../features/frameworks/nanostack-libservice/source/libip6string/stoip6.c
  ../features/frameworks/nanostack-libservice/source/libBits/common_functions.c
  
)

# Test files
set(unittest-test-sources
  features/netsocket/SocketAddress/test_SocketAddress.cpp
)
