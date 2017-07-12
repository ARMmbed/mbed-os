## mbed wrapper for the [little filesystem](https://github.com/geky/littlefs)

This is the mbed wrapper for [littlefs](https://github.com/geky/littlefs),
a little fail-safe filesystem designed for embedded systems.

```
   | | |     .---._____
  .-----.   |          |
--|o    |---| littlefs |
--|     |---|          |
  '-----'   '----------'
   | | |
```

The littlefs comes with the following features:

- **Power-loss resilient** - The littlefs is designed to handle random power
  failures and has strong copy-on-write guaruntees. During all filesystem
  operations, the storage on disk is always kept in a valid state. When
  updating a file, the original file will remain unmodified until the file is
  closed, or sync is called.

- **Error detection** - While the littlefs does not implement static wear
  leveling, the littlefs detects write error and uses a limited form of dynamic
  wear leveling to work around blocks that go bad during the lifetime of the
  filesystem.

- **Low RAM/ROM usage** - The littlefs is designed to work in a limited amount
  of memory, recursion is avoided, and dynamic memory is limited to
  configurable buffers that can be provided statically.

The littlefs also comes with the following limitations:

- **Not portable to host OSs** - While the littlefs can use an SD card, no
  littlefs driver has been ported to host OSs such as Linux or Windows. If
  you need to support host OSs consider using the [FATFileSystem](https://github.com/ARMmbed/mbed-os/blob/mbed-os-5.5/features/filesystem/fat/FATFileSystem.h)
  found in mbed OS.

- **No static wear leveling** - While the littlefs does extend the lifetime
  of storage through a form of dynamic wear leveling, the littlefs does not
  evict static blocks. If you need to maximize the longevity of the underlying
  storage consider using the [SPIFFileSystem](https://github.com/armmbed/mbed-spiffs).

## Usage

If you are already using a filesystem in mbed, adopting the littlefs should
just require a name change to use the [LittleFileSystem](LittleFileSystem.h)
class.

Here is a simple example that updates a file named "boot_count" every time
the application runs:
``` c++
#include "LittleFileSystem.h"
#include "SPIFBlockDevice.h"

// Physical block device, can be any device that supports the BlockDevice API
SPIFBlockDevice bd(PTE2, PTE4, PTE1, PTE5);

// Storage for the littlefs
LittleFileSystem fs("fs");

// Entry point
int main() {
    // Mount the filesystem
    int err = fs.mount(&bd);
    if (err) {
        // Reformat if we can't mount the filesystem,
        // this should only happen on the first boot
        LittleFileSystem::format(&bd);
        fs.mount(&bd);
    }

    // Read the boot count
    uint32_t boot_count = 0;
    FILE *f = fopen("/fs/boot_count", "r+");
    if (!f) {
        // Create the file if it doesn't exist
        f = fopen("/fs/boot_count", "w+");
    }
    fread(&boot_count, sizeof(boot_count), 1, f);

    // Update the boot count
    boot_count += 1;
    rewind(f);
    fwrite(&boot_count, sizeof(boot_count), 1, f);

    // Remember that storage may not be updated until the file
    // is closed successfully
    fclose(f);

    // Release any resources we were using
    fs.unmount();

    // Print the boot count
    printf("boot_count: %ld\n", boot_count);
}
```
