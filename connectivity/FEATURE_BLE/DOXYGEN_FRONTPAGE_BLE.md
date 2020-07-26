# BLE API {#mainpage}

The BLE module within mbed OS offers a high abstraction level for using
Bluetooth Low Energy on multiple platforms.

This documentation describes the internal structure of the mbed
[BLE API](https://github.com/armmbed/ble).

For getting started with BLE on mbed, check our [introduction
page](https://docs.mbed.com/docs/ble-intros/en/latest/).

For mbed OS examples using BLE, check [this
repository](https://github.com/armmbed/ble-examples). For mbed-classic
examples, please refer to [code under mbed.org](https://developer.mbed.org/teams/Bluetooth-Low-Energy/code/).

## Supported Services

Currently supported reference services include:

* [Battery](@ref BatteryService)
* [Device Firmware Update (DFU)](@ref DFUService)
* [Device Information](@ref DeviceInformationService)
* [Health Thermometer](@ref HealthThermometerService)
* [Heart Rate](@ref HeartRateService)
* [UART](@ref UARTService)
* [UriBeacon](@ref URIBeaconConfigService)
* [iBeacon](@ref iBeacon)

The [documentation](https://docs.mbed.com/docs/ble-intros/en/latest/AdvSamples/Overview/)
contains an overview on how to create new, application-specific services.
