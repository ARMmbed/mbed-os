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
