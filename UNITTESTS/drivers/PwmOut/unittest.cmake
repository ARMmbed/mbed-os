
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

set(unittest-test-flags
  -DDEVICE_PWMOUT
)

