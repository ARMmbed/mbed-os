# Mbed OS BLE services

BLE services are available in the https://github.com/ARMmbed/mbed-os-experimental-ble-services repository. It's a
community effort and contains services created by mbed-os team, our partners and mbed-os users. 

To use the services there add the repository to your project by adding a 'mbed-os-experimental-ble-services.lib' file
containing the link to the repo like so:
```
https://github.com/ARMmbed/mbed-os-experimental-ble-services
```

Each BLE service is an Mbed OS library.  To use it with new cmake based mbed-tools you will need to add the library
to your link libraries for your application.
