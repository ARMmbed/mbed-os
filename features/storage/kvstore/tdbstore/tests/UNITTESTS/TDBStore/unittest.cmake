
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
  ../platform/mbed-trace/mbed-trace
)

set(unittest-sources
  ../storage/blockdevice/source/HeapBlockDevice.cpp
  ../storage/blockdevice/source/BufferedBlockDevice.cpp
  ../storage/kvstore/tdbstore/source/TDBStore.cpp
  ../platform/mbed-trace/source/mbed_trace.c
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
  stubs/mbed_error.c
)

set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/moduletest.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
