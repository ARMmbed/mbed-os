## How to commission a Thread device in practise

### Building the node application

By default, the mbed Thread applications/examples use the static network link configuration defined in the [mesh-api configuration file](https://github.com/ARMmbed/mbed-os/blob/master/features/nanostack/FEATURE_NANOSTACK/mbed-mesh-api/mbed_lib.json).
If you want to use the Thread commissioning, add the following lines to your `.json` file. You can use the [mesh-minimal](https://github.com/ARMmbed/mbed-os-example-mesh-minimal) application as an example.

* `"mbed-mesh-api.thread-use-static-link-config": false`
* `"macros": ["MBEDTLS_USER_CONFIG_FILE=\"mbedtls_config.h\""]`

Setting `thread-use-static-link-config` to `false` prevents the usage of the predefined link configuration values and allows the device to start network scanning.

### QR code generation

You can use [a free online tool](http://www.qr-code-generator.com/) to generate a QR code.

In the tool, fill in the URL field: `v=1&eui=000b57fffe07a8be&cc=PV7TUCB0`.
These are just example values. Fill in the correct values for your device and make sure `v=1` is always there.

* `cc` is the PSKd, which is configured in the `.json` file (see the mesh-api configuration).
* `eui` is equal to the RF MAC address by default. 

There are four additional query parameters you can put into this field: 

* `vn`    Vendor name
* `vm`    Vendor model
* `vv`    Product/Vendor version
* `vs`    Product/Vendor serial number

To print the MAC address, you need to connect the node to the Thread network with static configuration enabled, unless you have your own configuration for the MAC address. For example, in the **mesh-minimal** application, place this `printf("MAC address = %s\n", mesh.get_mac_address());` after `printf("connected. IP = %s\n", mesh.get_ip_address());`

### Using the Thread commissioning application

You can use the [Thread Android application](https://play.google.com/store/apps/details?id=org.threadgroup.commissioner) for commissioning:

1. Set up a connection to the Wi-Fi access point to which the Thread border router is connected. When connected, the application lists your Thread border routers(s). 
2. Select a border router from the list.
3. Enter the passphrase asked by the application. It is `Thread Network` (when using the default mesh-api Thread configuration). After successful connection, the application is ready to scan the QR code.
4. Power on your Thread device if it is not already on. 
5. Scan the QR code. The Thread device should join the network. If you are using the mesh-minimal application you should see the IP address printed: `connected. IP = ...`.
