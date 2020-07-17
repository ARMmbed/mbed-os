
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../storage/blockdevice/source/ProfilingBlockDevice.cpp
  ../storage/blockdevice/source/HeapBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/test_ProfilingBlockDevice.cpp
  stubs/mbed_error.c
)
