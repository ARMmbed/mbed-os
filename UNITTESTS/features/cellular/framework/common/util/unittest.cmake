
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "cellular-framework-common-util")

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  features/cellular/framework/common/util
  ../features/cellular/framework/common
  ../features/frameworks/mbed-client-randlib/mbed-client-randlib
)

# Source files
set(unittest-sources
  ../features/cellular/framework/common/CellularUtil.cpp
)

# Test files
set(unittest-test-sources
  features/cellular/framework/common/util/utiltest.cpp
  stubs/randLIB_stub.cpp
)
