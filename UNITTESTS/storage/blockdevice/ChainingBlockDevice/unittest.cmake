
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../storage/blockdevice/ChainingBlockDevice.cpp
  ../storage/blockdevice/HeapBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  storage/blockdevice/ChainingBlockDevice/test_ChainingBlockDevice.cpp
)
