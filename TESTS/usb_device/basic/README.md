# Testing the USB device data toggle reset with a Linux host

When you run the `tests-usb_device-basic` test suite on a Linux machine, please make
sure that at least one of the following prerequisites are met:
* using the Linux kernel ***4.17* or newer**,
* using the ***eHCI*** USB driver instead of *xHCI*.

Implementations of the *xHCI* driver prior to version 4.17 of the Linux kernel did
not have the functionality necessary to test `"endpoint test data toggle reset"`.
Even though the data toggle is correctly reset on the device side, the host side will
not be synchronized and the test will falsely fail.

Further reading:
1. [the Linux kernel patch adding missing xHCI behavior](https://github.com/torvalds/linux/commit/f5249461b504d35aa1a40140983b7ec415807d9e),
1. [LKML discussion explaining the details of this issue](https://lkml.org/lkml/2016/12/15/388).