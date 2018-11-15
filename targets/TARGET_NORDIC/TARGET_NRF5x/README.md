# Nordic NRF5x

## Adding New Targets Based On Nordic NRF52832 And NRF52840 MCUs

First, create a new entry in `mbed-os/targets/targets.json` using the template below:

```json
{
    "target_name": {
        "inherits": [""],
        "release_versions": ["5"],
        "device_name": ""
    }
}
```

Where `target_name` is the name of the new target, `inherits` can be either `MCU_NRF52832` or `MCU_NRF52840`, and `device_name` is the ID specifying actual RAM and flash size, for example, `nRF52832_xxAA` and `nRF52840_xxAA`. The `release_version` specifies that the target is compatible with Mbed OS 5.

This entry enables the new target in the Mbed OS build system.

Next, add optional target specific configuration in `mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/mbed_lib.json`.

```json
{
    "target_overrides": {
        "target_name": {
            "target.macros_add": [
                "optional macros"
            ],
            "optional configuration"
        }
    }
}
```

The optional configuration includes specifying errata fixes specific for the MCU release used on the target, configuring the low-frequency clock source, and configuring the UART behavior. See targets `NRF52_DK` and `NRF52840_DK` for examples.

### LF Clock Configuration

LF clock source configuration is used for MCU startup initialization and the BLE SoftDevice LF clock configuration (if BLE libraries is used). Advanced configurations are used only for the BLE SoftDevice LF clock configuration.

Default clock source is XTAL oscillator. There are three options that can be configured as the clock source:
    - NRF_LF_SRC_XTAL
    - NRF_LF_SRC_RC
    - NRF_LF_SRC_SYNTH

Set `lf_clock_src` to what is most suitable for the target. This value can later be overridden by the user application if necessary.


## Mbed HAL Implementation Details

### SPI and I2C

The TWI/TWIM (I2C) and SPI/SPIM module shares the same underlying hardware and each instance can only provide one functionality at a time. Both the NRF52832 and NRF52840 have 2 TWI/TWIM modules and 3 SPI/SPIM:

| Instance 0 | Instance 1 | Instance 2 |
| :---:      | :---:      | :---:      |
| SPI0/SPIM0 | SPI1/SPIM1 | SPI2/SPIM2 |
| TWI0/TWIM0 | TWI1/TWIM1 |            |

When instantiating a new Mbed SPI object or I2C object, the object will be assigned a hardware instance. By default, the HAL implementation will automatically pick a hardware instance based on the assigned pins in the following order:

1. The driver will look up the pins in the configuration table and pick a pre-assigned instance.
1. If the pins can't be found in the configuration table, the driver will check if a hardware instance has already been assigned to those pins so that objects using the same pins will share the same instance.
1. If no instance has been assigned, the driver will look for a free instane. For I2C objects instances are assigned from 0 to 1. For SPI objects instances are assigned from 2 to 0. This ensures that no matter the order of instantiation the first three objects can be guaranteed to be on separate instances no matter the pins being used.
1. If no unused instance can be found, objects not sharing any pins will be assigned to the same default instance, which is `Instance 0` for I2C and `Instance 2` for SPI.

#### Customization

A custom configuration table can be provided by overriding the weakly defined default empty table. In the example below, I2C objects using pins `p1` and `p2` for `SDA` and `SCL` will always be assigned to `Instance 1` and SPI objects using pins `p3`, `p4`, `p5` for `MOSI`, `MISO`, and `CLOCK` will be assigned to `Instance 2` and SPI objects using pins `p6`, `p7`, and `p8` will be assigned to `Instance 0`. The custom configuration table must always be terminated with a row of `NC`.

```
const PinMapI2C PinMap_I2C[] = {
    {p1, p2, 1},
    {NC, NC, NC}
};

const PinMapSPI PinMap_SPI[] = {
    {p3, p4, p5, 2},
    {p6, p7, p8, 0},
    {NC, NC, NC, NC}
};
```

The tables must be placed in a C compilation file.

#### Concurrency

1. When called from the same thread, it is safe to assign I2C and SPI objects to the same instance.
1. If an instance is being used exclusively for either I2C or SPI, the objects can safely be called from multiple threads.
1. If an instance is being used for both I2C and SPI, the user must provide thread safety between the objects.


### Serial

The serial implementation uses the UARTE module which works exclusively through EasyDMA and RAM buffers.
To ensure no data is lost a FIFO is used to buffer data received. The FIFO buffer is backed by the Nordic atomic fifo, which can be read and written to safely without disabling interrupts.

#### Customization

The FIFOs can be resized to fit the application:

```
    "name": "nordic",
    "config": {
        "uart_0_fifo_size": {
            "help": "UART0 FIFO buffer. FIFO buffer is filled from DMA buffer.",
            "value": 32
        },
        "uart_1_fifo_size": {
            "help": "UART1 FIFO buffer. FIFO buffer is filled from DMA buffer.",
            "value": 32
        }
    }
```

FIFO buffers can be configured per instance and the size should reflect the largest expected burst data. For example, a serial debug port might receive a line of data at a time, so an 80 byte FIFO buffer would be adequate. A serial port connected to a wifi radio should have a FIFO buffer in the kilo byte range.

For the NRF52840, UARTE instances are assigned based on pins and calling order. Serial objects with the same pin configurations will go to the same instance. A custom configuration table can be provided by overriding the weakly defined default empty table. In the example below, serial objects using pins `p1` and `p2` for `Tx` and `Rx` will always be assigned to `Instance 1` and serial objects using pins `p3` and `p4` for `Tx` and `Rx` will be assigned to `Instance 0` regardless of calling order. The custom configuration table must always be terminated with a row of `NC`.

```
const PinMapI2C PinMap_UART[] = {
    {p1, p2, 1},
    {p3, p4, 0},
    {NC, NC, NC}
};
```

The table must be placed in a C compilation file.

#### Flow Control (RTS/CTS)

When hardware flow control is enabled the FIFO buffers can be reduced to save RAM. Flow control ensures that bytes cannot be dropped due to poor interrupt latency.

#### SWI0

To minimize the time spend in the highest priority interrupt handler all callbacks to the user provided IRQ handlers are deferred through Software Interrupts running at lowest priority. SWI0 is reserved by the serial implementation.


#### Asserts

The nordic asserts have been redirected to mbed error handling when building in debug mode.
The SDK file `mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_14_2/libraries/util/nrf_assert.h` was modified to enable the asserts when NDEBUG is not defined.

The assert handler is defined in mbed-os/features/FEATURE_BLE/targets/TARGET_NORDIC/TARGET_NRF5x/source/btle/btle.cpp : assert_nrf_callback() which forwards assert failures to thye mbed error() handler.


#### Limitations

 * The UARTE hardware only supports 8-bit, None/Even parity, and 1 stop bit.
 * The asynchronous read and write implementation currently only support 255 byte transfers.
 * The EasyDMA hardware can only read from RAM, which means all Tx buffers must reside in RAM. If a Tx buffer residing in flash is passed to the asynchronous write function, the function will try to copy the Tx buffer to a temporary internal buffer and transmit the data from there.
 * It is not possible to do an asynchronous write from flash and receive non-asynchronously at the same time since the non-asynchronous receive buffer is being used as the temporary transmission buffer.
 * The driver will flush the DMA buffer after a configurable timeout. During this process the UART will be halted and therefor unable to receive data. Hardware flow control should be enabled to avoid missing any data during this window.

#### Serial Wire Output (SWO)

On the nRF52832 pin 18 (p18 or p0_18) is the SWO pin and a GPIO pin.  On the nRF52_DK and DELTA_DFBM_NQ620 targets p18 is also mapped to LED2, so the ITM has been removed from these targets to avoid contention.  If you need SWO capability instead of LED2, add the ITM through ```mbed_app.json```:
```
    "target_overrides": {
        "*": {
            "target.device_has_add": ["ITM"]
        }
    }
```

## SoftDevice

SoftDevices are treated as bootloaders and automatically combined by the tools. Available SoftDevices:

 * TARGET_SOFTDEVICE_COMMON
 * TARGET_SOFTDEVICE_S112
 * TARGET_SOFTDEVICE_S132_FULL (MBR + SoftDevice, default)
 * TARGET_SOFTDEVICE_S132_OTA (SoftDevice only, for firmware updates)
 * TARGET_SOFTDEVICE_S132_MBR (MBR only, for bootloader builds)
 * TARGET_SOFTDEVICE_S140_FULL (MBR + SoftDevice, default)
 * TARGET_SOFTDEVICE_S140_OTA (SoftDevice only, for firmware updates)
 * TARGET_SOFTDEVICE_S140_MBR (MBR only, for bootloader builds)
 * TARGET_SOFTDEVICE_NONE

NRF52832 uses the S132 SoftDevice and NRF52840 uses the S140 SoftDevice.

The OTA and MBR binaries are obtained from the original FULL SoftDevice by splitting it in an MBR part and a SoftDevice part. The MBR is needed for the bootloader and the SoftDevice for firmware updates.

### Changing SoftDevice

By default, all applications are built with the FULL SoftDevice. This can be changed by modifying the application's `mbed_app.json` configuration file. Examples for the NRF52_DK and NRF52840_DK boards are shown below.

Build application with no MBR or SoftDevice:

```
    "target_overrides": {
        "NRF52_DK": {
            "target.extra_labels_remove": ["SOFTDEVICE_COMMON", "SOFTDEVICE_S132_FULL"],
            "target.extra_labels_add": ["SOFTDEVICE_NONE"]
        },
        "NRF52840_DK": {
            "target.extra_labels_remove": ["SOFTDEVICE_COMMON", "SOFTDEVICE_S140_FULL"],
            "target.extra_labels_add": ["SOFTDEVICE_NONE"]
        }       
    }
```


Build application with MBR only:

```
    "target_overrides": {
        "NRF52_DK": {
            "target.extra_labels_remove": ["SOFTDEVICE_COMMON", "SOFTDEVICE_S132_FULL"],
            "target.extra_labels_add": ["SOFTDEVICE_S132_MBR"]
        },
        "NRF52840_DK": {
            "target.extra_labels_remove": ["SOFTDEVICE_S140_FULL"],
            "target.extra_labels_add": ["SOFTDEVICE_S140_MBR"]
        }              
    }
```

## NRF52840 CryptoCell310 Support 

By default, all NRF52840 applications will use the CryptoCell310 subsystem which is built into the NRF52840.  This provides hardware support for random number generation and encryption which are utilized by Mbed TLS.  If using the CryptoCell310 subsystem is not desired, it can be replaced with a software implementation.  This can be done by modifying the application's `mbed_app.json` configuration file as shown below.
 
```
    "target_overrides": {
        "NRF52840_DK": {
            "target.features_remove": ["CRYPTOCELL310"],
            "target.macros_remove": ["MBEDTLS_CONFIG_HW_SUPPORT"],
            "target.macros_add": ["NRFX_RNG_ENABLED=1", "RNG_ENABLED=1", "NRF_QUEUE_ENABLED=1"]
        }
    }
```

