
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
  ../storage/blockdevice/source
)

set(unittest-sources
  ../storage/blockdevice/source/ChainingBlockDevice.cpp
  ../storage/blockdevice/source/HeapBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/test_ChainingBlockDevice.cpp
)
