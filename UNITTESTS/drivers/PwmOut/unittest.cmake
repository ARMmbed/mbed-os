
####################
# UNIT TESTS
####################
set(TEST_SUITE_NAME "PwmOut")

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  .
  ../hal
)

# Source files
set(unittest-sources
  ../drivers/source/PwmOut.cpp
)

# Test files
set(unittest-test-sources
  drivers/PwmOut/test_pwmout.cpp
  stubs/mbed_critical_stub.c
  stubs/mbed_assert_stub.cpp
  stubs/pwmout_api_stub.c
)

# defines
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEVICE_PWMOUT")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_PWMOUT")
