
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  features/cellular/framework/common/util
  ../features/cellular/framework/common
  ../features/cellular/framework/AT
  ../features/frameworks/mbed-client-randlib/mbed-client-randlib
)

# Source files
set(unittest-sources
  stubs/randLIB_stub.c
  ../features/cellular/framework/AT/AT_CellularDevice.cpp
)

# Test files
set(unittest-test-sources
  features/cellular/framework/AT/at_cellulardevice/at_cellulardevicetest.cpp
  stubs/AT_CellularNetwork_stub.cpp 
  stubs/ATHandler_stub.cpp 
  stubs/AT_CellularSMS_stub.cpp 
  stubs/AT_CellularSIM_stub.cpp 
  stubs/AT_CellularPower_stub.cpp 
  stubs/AT_CellularInformation_stub.cpp 
  stubs/CellularUtil_stub.cpp 
  stubs/AT_CellularBase_stub.cpp 
  stubs/NetworkInterface_stub.cpp 
  stubs/EventQueue_stub.cpp 
  stubs/FileHandle_stub.cpp 
  stubs/mbed_assert_stub.c
)
