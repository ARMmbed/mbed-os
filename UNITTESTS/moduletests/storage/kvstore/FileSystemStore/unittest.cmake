####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
  ../features/frameworks/mbed-trace/mbed-trace
)

set(unittest-sources
  ../storage/blockdevice/source/HeapBlockDevice.cpp
  ../storage/kvstore/filesystemstore/FileSystemStore.cpp
  ../storage/filesystem/littlefs/LittleFileSystem.cpp
  ../storage/filesystem/Dir.cpp
  ../storage/filesystem/File.cpp
  ../storage/filesystem/FileSystem.cpp
  ../features/frameworks/mbed-trace/source/mbed_trace.c
  ../storage/filesystem/littlefs/littlefs/lfs_util.c
  ../storage/filesystem/littlefs/littlefs/lfs.c
  ../platform/source/FileBase.cpp
  ../platform/source/FileSystemHandle.cpp
  ../platform/source/FileHandle.cpp
)

set(unittest-test-sources
  moduletests/storage/kvstore/FileSystemStore/moduletest.cpp
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