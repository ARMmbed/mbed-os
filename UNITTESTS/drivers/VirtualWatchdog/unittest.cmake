
####################
# UNIT TESTS
####################
set(TEST_SUITE_NAME "VirtualWatchdog")

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  .
  ../hal
)

# Source files
set(unittest-sources
  ../drivers/VirtualWatchdog.cpp
)

# Test files
set(unittest-test-sources
  drivers/VirtualWatchdog/test_virtualwatchdog.cpp
  drivers/VirtualWatchdog/Watchdog.cpp
  stubs/mbed_critical_stub.c  
)

# defines
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEVICE_WATCHDOG -DMBED_WDOG_ASSERT=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_WATCHDOG -DMBED_WDOG_ASSERT=1")
