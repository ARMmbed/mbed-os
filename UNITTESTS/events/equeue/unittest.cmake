
####################
# UNIT TESTS
####################

list(REMOVE_ITEM unittest-includes ${PROJECT_SOURCE_DIR}/../events/test/unit/target_h ${PROJECT_SOURCE_DIR}/../events/test/unit/target_h/equeue)

set(unittest-includes ${unittest-includes}
  ../events/source
  ../events
  ../events/events
  ../events/internal
)

set(unittest-sources
  ../events/source/equeue.c
)

set(unittest-test-sources
  ../events/test/unit/equeue/test_equeue.cpp
  ../events/test/unit/stubs/EqueuePosix_stub.c
)

set(unittest-test-flags
  -pthread
  -DEQUEUE_PLATFORM_POSIX
)

