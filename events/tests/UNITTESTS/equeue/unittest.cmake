
####################
# UNIT TESTS
####################

list(REMOVE_ITEM unittest-includes ${PROJECT_SOURCE_DIR}/../events/tests/UNITTESTS/target_h ${PROJECT_SOURCE_DIR}/../events/test/UNITTESTS/target_h/equeue)

set(unittest-includes ${unittest-includes}
  ../events/source
  ../events/include/events
  ../events/include/events/internal
)

set(unittest-sources
  ../events/source/equeue.c
)

set(unittest-test-sources
  ../events/tests/UNITTESTS/equeue/test_equeue.cpp
  ../events/tests/UNITTESTS/stubs/EqueuePosix_stub.c
)

set(unittest-test-flags
  -pthread
  -DEQUEUE_PLATFORM_POSIX
)

