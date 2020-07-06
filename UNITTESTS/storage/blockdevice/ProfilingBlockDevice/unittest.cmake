
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../storage/blockdevice/ProfilingBlockDevice.cpp
  ../storage/blockdevice/HeapBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  storage/blockdevice/ProfilingBlockDevice/test_ProfilingBlockDevice.cpp
  stubs/mbed_error.c
)
