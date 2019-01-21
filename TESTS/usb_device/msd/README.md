# USB mass storage test user guide

To run the tests-usb_device-msd test device with at least *70kB* of RAM is required.
Test creates 64kB `HeapBlockDevice` as block device and mounts FAT32 filesystem on it.
64kB block device is the smallest one that can mount FAT32 filesystem.

Test can be easily extended to use any block device available in Mbed

Test run command:
```bash
mbed test -t COMPILER -m TARGET -n tests-usb_device-msd
```
