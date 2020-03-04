
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../features/storage/blockdevice/ObservingBlockDevice.cpp
  ../features/storage/blockdevice/ReadOnlyBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
  stubs/mbed_error.c
)

set(unittest-test-sources
  features/storage/blockdevice/ObservingBlockDevice/test_ObservingBlockDevice.cpp
  stubs/BlockDevice_mock.h
)
