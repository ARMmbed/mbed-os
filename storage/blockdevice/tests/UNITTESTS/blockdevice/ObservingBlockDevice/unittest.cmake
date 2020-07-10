
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../storage/blockdevice/source/ObservingBlockDevice.cpp
  ../storage/blockdevice/source/ReadOnlyBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
  stubs/mbed_error.c
)

set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/test_ObservingBlockDevice.cpp
  stubs/BlockDevice_mock.h
)
