# mbed Bluetooth Low Energy Stack

This is the Github repository for the `BLE_API`. Please see the [Mbed OS Example BLE GitHub Repo](https://github.com/ARMmbed/mbed-os-example-ble) for all code examples, and the [Mbed OS BLE API page](https://os.mbed.com/docs/latest/apis/bluetooth.html) for all code documentation and general help.

## Getting Started

* [Mbed OS examples](https://os.mbed.com/teams/mbed-os-examples/) for all Mbed OS and BLE examples.
* [Mbed OS example BLE GitHub repo](https://github.com/ARMmbed/mbed-os-example-ble) for all Mbed OS BLE examples.
* [Mbed OS BLE introduction](https://os.mbed.com/docs/latest/apis/ble.html) for an introduction to Mbed BLE.
* [Mbed OS BLE API page](https://os.mbed.com/docs/latest/apis/bluetooth.html) for the Mbed BLE API documentation.

## Privacy notice

The Cordio Bluetooth stack only stores one single signing key. This key is then 
shared across all bonded devices. If a malicious device bonds with the Mbed OS 
application it then gains knowledge of the shared signing key of the Mbed OS device. 
The malicious device can then track the Mbed OS device whenever a signing write 
is issued from it. 

To overcome this privacy issue do not issue signed writes from the Mbed OS device.
A signed write occurs when the member function `write` of `GattClient` is called 
with its `cmd` argument set to `GATT_OP_SIGNED_WRITE_CMD`.

Instead of using signed writes, enable encryption on the connection. This is achieved
 by calling the function `setLinkEncryption` of the `SecurityManager`. Set the encryption 
to at least `ENCRYPTED`. 
