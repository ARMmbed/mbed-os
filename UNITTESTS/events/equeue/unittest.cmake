
####################
# UNIT TESTS
####################

list(REMOVE_ITEM unittest-includes ${PROJECT_SOURCE_DIR}/../events/test/unity/target_h ${PROJECT_SOURCE_DIR}/../events/test/unity/target_h/equeue)

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
  ../events/test/unity/equeue/test_equeue.cpp
  ../events/test/unity/stubs/EqueuePosix_stub.c
)

set(unittest-test-flags
  -pthread
  -DEQUEUE_PLATFORM_POSIX
)

