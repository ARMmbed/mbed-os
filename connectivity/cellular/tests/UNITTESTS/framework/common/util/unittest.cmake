
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  ../connectivity/cellular/tests/UNITTESTS/framework/common/util
  ../connectivity/cellular/include/cellular/framework/common
  ../platform/randlib/include/mbed-client-randlib
)

# Source files
set(unittest-sources
  ../connectivity/cellular/source/framework/common/CellularUtil.cpp
)

# Test files
set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/utiltest.cpp
  stubs/randLIB_stub.cpp
)
