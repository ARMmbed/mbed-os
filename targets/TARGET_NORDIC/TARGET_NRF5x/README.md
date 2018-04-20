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

The serial implementation uses the UARTE module which works exclusively through EasyDMA and RAM buffers. For optimal performance, each configured instance (NRF52832 has 1, NRF52840 has 2) has three buffers statically assigned:

1. Rx DMA buffer, which EasyDMA is currently writing to.
1. Rx DMA buffer, pre-loaded in EasyDMA for automatic switchover.
1. Rx FIFO buffer, for serving data to the application.

When the first DMA buffer is full or flushed the interrupt handler will automatically copy the DMA buffer to the FIFO buffer. This happens in interrupt context to avoid data loss and with UARTE interrupts set at the highest priority. The FIFO buffer is backed by the Nordic atomic fifo, which can be read and written to safely without disabling interrupts. 

#### Customization

All buffers can be resized to fit the application:

```
    "name": "nordic",
    "config": {
        "uart-dma-size": {
            "help": "UART DMA buffer. 2 buffers per instance. DMA buffer is filled by UARTE",
            "value": 8
        },
        "uart-0-fifo-size": {
            "help": "UART0 FIFO buffer. FIFO buffer is filled from DMA buffer.",
            "value": 32
        },
        "uart-1-fifo-size": {
            "help": "UART1 FIFO buffer. FIFO buffer is filled from DMA buffer.",
            "value": 32
        }
    }
```

All DMA buffers are the same size and must be at least 5 bytes due to hardware restrictions. DMA buffers should be sized to handle the worst expected interrupt latency. FIFO buffers can be configured per instance and the size should reflect the largest expected burst data. For example, a serial debug port might receive a line of data at a time, so an 80 byte FIFO buffer would be adequate. A serial port connected to a wifi radio should have a FIFO buffer in the kilo byte range.

For the NRF52840, UARTE instances are assigned based on pins and calling order. Serial objects with the same pin configurations will go to the same instance. A custom configuration table can be provided by overriding the weakly defined default empty table. In the example below, serial objects using pins `p1` and `p2` for `Tx` and `Rx` will always be assigned to `Instance 1` and serial objects using pins `p3` and `p4` for `Tx` and `Rx` will be assigned to `Instance 0` regardless of calling order. The custom configuration table must always be terminated with a row of `NC`.

```
const PinMapI2C PinMap_UART[] = { 
    {p1, p2, 1},
    {p3, p4, 0},
    {NC, NC, NC}
};
```

The table must be placed in a C compilation file.


#### RTC2

Because each DMA buffer must be at least 5 bytes deep, each buffer is automatically flushed after a certain idle period to ensure low latency and correctness. This idle timeout is implemented using 2 of the 4 channels on RTC instance 2. This leaves RTC0 for the SoftDevice and RTC1 for Mbed tickers.

The RTC2 ISR is set at the lowest interrupt priority to ensure that UARTE interrupts take precedence. The last 2 of the 4 RTC channels are used for decoupling UARTE ISR context from Mbed IRQ events. This ensures that any user code will only delay other user callbacks and idle flushing and puts an upper bound on the interrupt handling time for the UARTE ISR.

#### Limitations

 * The UARTE hardware only supports 8-bit, None/Even parity, and 1 stop bit.
 * The asynchronous read and write implementation currently only support 255 byte transfers.
 * The EasyDMA hardware can only read from RAM, which means all Tx buffers must reside in RAM. If a Tx buffer residing in flash is passed to the asynchronous write function, the function will try to copy the Tx buffer to a temporary internal buffer and transmit the data from there.
 * It is not possible to do an asynchronous write from flash and receive non-asynchronously at the same time since the non-asynchronous receive buffer is being used as the temporary transmission buffer. 

