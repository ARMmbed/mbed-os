
####################
# UNIT TESTS
####################

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
  features/cellular/framework/AT/at_cellularbase/at_cellularbasetest.cpp
  stubs/mbed_assert_stub.c
  stubs/ATHandler_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/FileHandle_stub.cpp
  stubs/ConditionVariable_stub.cpp
  stubs/Mutex_stub.cpp
)
