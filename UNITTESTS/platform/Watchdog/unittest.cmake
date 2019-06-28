
####################
# UNIT TESTS
####################
set(TEST_SUITE_NAME "mbed_watchdog_mgr")
# Add test specific include paths
set(unittest-includes ${unittest-includes}
  .
  ../hal
)

# Source files
set(unittest-sources
  ../platform/mbed_watchdog_mgr.cpp
  
)

# Test files
set(unittest-test-sources
  platform/mbed_watchdog_mgr/test_mbed_watchdog_mgr.cpp
  platform/mbed_watchdog_mgr/Watchdog.cpp
  stubs/mbed_assert_stub.c
  stubs/mbed_critical_stub.c  
  stubs/watchdog_api_stub.c
)

# defines
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEVICE_WATCHDOG -DHW_WATCHDOG_TIMEOUT=500")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_WATCHDOG -DHW_WATCHDOG_TIMEOUT=500")
