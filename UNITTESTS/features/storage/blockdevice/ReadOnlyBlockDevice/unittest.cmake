
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../features/storage/blockdevice/ReadOnlyBlockDevice.cpp
  ../features/storage/blockdevice/HeapBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  features/storage/blockdevice/ReadOnlyBlockDevice/test_ReadOnlyBlockDevice.cpp
  stubs/mbed_error.c
)
