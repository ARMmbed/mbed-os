
####################
# UNIT TESTS
####################

# Unit test suite name
set(TEST_SUITE_NAME "cellular-framework-AT-AT_CellularBase")

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  features/cellular/framework/AT/AT_CellularBase
  ../features/cellular/framework/AT
  ../features/cellular/framework/common
  ../features/frameworks/mbed-trace
  ../features/frameworks/nanostack-libservice/mbed-client-libservice
  ../features/netsocket
)

# Source files
set(unittest-sources
  ../features/cellular/framework/AT/AT_CellularBase.cpp
)

# Test files
set(unittest-test-sources
  stubs/mbed_assert_stub.c
  stubs/ATHandler_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  features/cellular/framework/AT/AT_CellularBase/test_at_cellularbase.cpp
  features/cellular/framework/AT/AT_CellularBase/at_cellularbasetest.cpp
)
