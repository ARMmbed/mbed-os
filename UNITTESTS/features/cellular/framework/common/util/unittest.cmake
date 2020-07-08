
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  connectivity/cellular/framework/common/util
  ../connectivity/cellular/framework/common
  ../features/frameworks/mbed-client-randlib/mbed-client-randlib
)

# Source files
set(unittest-sources
  ../connectivity/cellular/framework/common/CellularUtil.cpp
)

# Test files
set(unittest-test-sources
  connectivity/cellular/framework/common/util/utiltest.cpp
  stubs/randLIB_stub.cpp
)
