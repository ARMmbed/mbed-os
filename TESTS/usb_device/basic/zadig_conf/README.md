# Generic USB driver installation on Windows machines

In order to run the Mbed OS USB device test suite (`tests-usb_device-*`)
on Windows hosts you need to install generic USB drivers for two test devices.

1. Download *Zadig* application from https://zadig.akeo.ie/.
1. Unplug the Mbed device.
1. Open *Zadig*.
1. Select *Device -> Load Preset Device*.
1. Open `mbed_os-usb_test_device1.cfg`.
1. Choose `libusb-win32 (v1.2.6.0)` driver.
1. Select `Install Driver` and click it.
1. Select *Device -> Load Preset Device*.
1. Open `mbed_os-usb_test_device2.cfg`.
1. Choose `libusb-win32 (v1.2.6.0)` driver.
1. Select `Install Driver` and click it.
1. Close *Zadig*.
1. Plug both device USB interfaces (*DAPLink* and *USB device*).
