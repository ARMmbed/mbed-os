
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../storage/blockdevice/ReadOnlyBlockDevice.cpp
  ../storage/blockdevice/HeapBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  storage/blockdevice/ReadOnlyBlockDevice/test_ReadOnlyBlockDevice.cpp
  stubs/mbed_error.c
)
