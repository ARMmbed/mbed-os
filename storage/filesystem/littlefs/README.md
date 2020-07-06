## Mbed OS API for the little filesystem

This is the Mbed OS API for littlefs, a little fail-safe filesystem
designed for embedded systems.

```
   | | |     .---._____
  .-----.   |          |
--|o    |---| littlefs |
--|     |---|          |
  '-----'   '----------'
   | | |
```

**Bounded RAM/ROM** - The littlefs is designed to work with a limited amount
of memory. Recursion is avoided, and dynamic memory is limited to configurable
buffers that can be provided statically.

**Power-loss resilient** - The littlefs is designed for systems that may have
random power failures. The littlefs has strong copy-on-write guarantees, and
storage on disk is always kept in a valid state.

**Wear leveling** - Because the most common form of embedded storage is erodible
flash memories, littlefs provides a form of dynamic wear leveling for systems
that cannot fit a full flash translation layer.

## Usage

If you are already using a filesystem in Mbed, adopting the littlefs should
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

## Reference material

[DESIGN.md](littlefs/DESIGN.md) - DESIGN.md contains a fully detailed dive into
how littlefs actually works. We encourage you to read it because the
solutions and tradeoffs at work here are quite interesting.

[SPEC.md](littlefs/SPEC.md) - SPEC.md contains the on-disk specification of
littlefs with all the nitty-gritty details. This can be useful for developing
tooling.

## Related projects

[littlefs](https://github.com/geky/littlefs) - Where the core of littlefs
currently lives.

[littlefs-fuse](https://github.com/geky/littlefs-fuse) - A [FUSE](https://github.com/libfuse/libfuse)
wrapper for littlefs. The project allows you to mount littlefs directly in a
Linux machine. This can be useful for debugging littlefs if you have an SD card
handy.

[littlefs-js](https://github.com/geky/littlefs-js) - A JavaScript wrapper for
littlefs. I'm not sure why you would want this, but it is handy for demos.
You can see it in action [here](http://littlefs.geky.net/demo.html).
