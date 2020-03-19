####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
  ../features/frameworks/mbed-trace/mbed-trace
)

set(unittest-sources
  ../features/storage/blockdevice/HeapBlockDevice.cpp
  ../features/storage/kvstore/filesystemstore/FileSystemStore.cpp
  ../features/storage/filesystem/littlefs/LittleFileSystem.cpp
  ../features/storage/filesystem/Dir.cpp
  ../features/storage/filesystem/File.cpp
  ../features/storage/filesystem/FileSystem.cpp
  ../features/frameworks/mbed-trace/source/mbed_trace.c
  ../features/storage/filesystem/littlefs/littlefs/lfs_util.c
  ../features/storage/filesystem/littlefs/littlefs/lfs.c
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