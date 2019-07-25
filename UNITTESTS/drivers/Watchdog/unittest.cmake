
####################
# UNIT TESTS
####################
set(TEST_SUITE_NAME "Watchdog")

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  .
  ../hal
)

# Source files
set(unittest-sources
  ../drivers/source/Watchdog.cpp
)

# Test files
set(unittest-test-sources
  drivers/Watchdog/test_watchdog.cpp  
  stubs/mbed_critical_stub.c  
  stubs/mbed_assert_stub.c
  stubs/watchdog_api_stub.c
)

# defines
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEVICE_WATCHDOG -DMBED_WDOG_ASSERT=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_WATCHDOG -DMBED_WDOG_ASSERT=1")
