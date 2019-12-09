
####################
# UNIT TESTS
####################

list(REMOVE_ITEM unittest-includes ${PROJECT_SOURCE_DIR}/target_h/events ${PROJECT_SOURCE_DIR}/target_h/events/equeue)

set(unittest-includes ${unittest-includes}
  ../events/source
  ../events
  ../events/internal
)

set(unittest-sources
  ../events/source/equeue.c
)

set(unittest-test-sources
  events/equeue/test_equeue.cpp
  stubs/EqueuePosix_stub.c
)

set(unittest-test-flags
  -pthread
  -DEQUEUE_PLATFORM_POSIX
)

