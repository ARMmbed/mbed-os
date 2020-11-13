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
  ../storage/kvstore/filesystemstore/source/FileSystemStore.cpp
  ../storage/filesystem/littlefs/source/LittleFileSystem.cpp
  ../storage/filesystem/source/Dir.cpp
  ../storage/filesystem/source/File.cpp
  ../storage/filesystem/source/FileSystem.cpp
  ../platform/mbed-trace/source/mbed_trace.c
  ../storage/filesystem/littlefs/littlefs/lfs_util.c
  ../storage/filesystem/littlefs/littlefs/lfs.c
  ../platform/source/FileBase.cpp
  ../platform/source/FileSystemHandle.cpp
  ../platform/source/FileHandle.cpp
)

set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/moduletest.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
  stubs/mbed_error.c
  stubs/kv_config_stub.cpp
  stubs/mbed_retarget_stub.cpp
)

set(unittest-test-flags
  -DMBED_LFS_READ_SIZE=64
  -DMBED_LFS_PROG_SIZE=64
  -DMBED_LFS_BLOCK_SIZE=512
  -DMBED_LFS_LOOKAHEAD=512
)
