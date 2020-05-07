**NOTE!** `mbed-coap` has been removed from within Mbed OS.

The repository is still available for use via its own public repo. Please see [mbed-coap](https://github.com/ARMmbed/mbed-coap).
You can easily include it still in your project by adding a `.lib` file referring to it, for example:

filename: `mbed-coap.lib`
contents: `https://github.com/armmbed/mbed-coap/#4ee0eb3b18267b5be0d42f00dc4cc0660679686b`

If you want your project to be compatible with Mbed OS 5 and 6, we recommend you add a
`.mbedignore` file which exludes the Mbed OS `mbed-coap` always and you have the `mbed-coap.lib`
in your project instead (i.e. you always use `mbed-coap` via its repository).

Pelion Device Management Client will include `mbed-coap` by default starting from version 4.5.0.
