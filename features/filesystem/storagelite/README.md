## StorageLite

StorageLite is a storage solution, providing a key value store like API of set/get data or a reduced POSIX API of open-write-close and open-read-close. It provides fast write and read functions, power failure resilience, security and built-in backup and factory reset support.

Uses: keeping credentials and cryptographic keys, application settings, application states and small application data to be sent to servers.

Hardware: designed for external NOR flash, it can work also on internal flash and with additional block devices on SD cards.

StorageLite is a lightweight file system storage module for files in external memory. Files are stored and accessed by file-name. StorageLite has the following attributes:

- High performance through a small RAM table.
- Security:
  - Integrity through file authentication.
  - Confidentiality through file encryption.
  - Rollback protection.
- Wear level: StorageLite stores files in a sequential order. This concept uses the entire external memory space. It is optimized for NOR flash that supports a limited number of erase per sector.
- Backup: built-in backup and factory reset support.
- Low RAM memory footprint: indexing each file uses 8 bytes of RAM.

Please see the [StorageLite design document](./StorageLiteDesign.md) for a detailed design description of StorageLite.

#### Flash memory

StorageLite is optimized for NOR flash memory. It is based on dividing the allocated memory storage to areas: active and standby. Data is written to the active area until it becomes full. When the active area becomes full, an internal garbage collection mechanism moves only the updated relevant files to the standby area and switches between the active and standby areas. The new standby area is then erased.

This concept is ideal for low wear leveling of the memory, but it comes with a price of using half of the external memory size.

#### APIs

You can either use StorageLite with set- and get-based APIs or it can be defined as StorageLiteFS and work with a limited POSIX open-write or read-close based API. (Only a **single** write or read operation is allowed.)

##### StorageLite

- `init`: binds StorageLite to a specific block device and constructs and initializes StorageLite module elements.
- `deinit`: deinitialize the StorageLite module.
- `set`: saves data into persistent storage by file name.
- `get`: retrieves file data from storage after authenticating it.
- `remove`: removes an existing file from storage.
- `get_file_size`: returns the file size after authenticating the data.
- `file_exist`: verifies that file exists and authenticates it.
- `get_file_flags`: returns files supported features (such as rollback protection, encryption and factory reset).
- `get_first/next`: provides the ability to iterate through existing files, starting from the first one.
- `factory_reset`: returns storage to contain only files that are backed up and returns those files to their backup version.

##### StorageLiteFS

- Mount or unmount: attaches or detaches a block device to or from the file system.
- Open or close: establishes the connection between a file and a file descriptor.
- Write: Writing data to the file associated with the opened file descriptor (replacing existing data - if exists). You can only call write once.
- Read: reads data from the file associated with the open file descriptor. Read can be called only once.
- Remove: deletes a file name from the file system.
- Reformat: clears a file system, resulting in an empty and mounted file system.

#### Usage

##### Using StorageLite

First, define a block device, and define the maximum number of files you want StorageLite to support. Next, create an instance of StorageLite, and initialize it with the above block device and max files setup. Then, you can start saving new files and getting their data.

``` c++
    #define MAX_NUM_OF_FILES 50
    SPIFBlockDevice spif(PTE2, PTE4, PTE1, PTE5);
    StorageLite sl;
    sl.init(&spif, MAX_NUM_OF_FILES);
```

##### Using StorageLiteFS

First, define the block device you want StorageLiteFS to use, and create an instance of StorageLite. Next, create a StorageLiteFS instance, giving it the name under which it is cataloged in the filesystem tree, the StorageLite instance it uses and its feature flags. Finally, mount the selected block device onto the StorageLiteFS instance. Now you can start opening new files for write and read.

``` c++
    SPIFBlockDevice spif(PTE2, PTE4, PTE1, PTE5);
    StorageLite sl;
    StorageLiteFS slfs("/sl", &sl, StorageLite::encrypt_flag);
    slfs.mount(&spif);
```

#### Testing StorageLite

Run any of the StorageLite tests with the `mbed` command:

```mbed test -n features-storagelite-tests-storagelite-whitebox```
```mbed test -n features-storagelite-tests-storagelite-fs_tests```
```mbed test -n features-storagelite-tests-storagelite-general_tests```

### StorageLite API

TODO: Link to StorageLite class reference once code merges.

### SotrageLite example

TODO: Link to transcluded example once it exists.