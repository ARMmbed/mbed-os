# HAL PinMap design document

# Table of contents

1. [HAL PinMap design document](#hal-pinmap-design-document).
1. [Table of contents](#table-of-contents).
1. [Introduction](#introduction).
    1. [Overview and background](#overview-and-background).
    1. [Requirements and assumptions](#requirements-and-assumptions).
1. [System architecture and high-level design](#system-architecture-and-high-level-design).
    1. [Pinmap](#pinmap).
    1. [Form Factor](#form-factor).
    1. [Restricted Pins](#restricted-pins).

# Introduction

Adds board introspection capabilities and tools.  
These tools will help designing better tests for the HAL.

### Overview and background

At the time being, drivers are only tested on a single "default" peripheral. However, some targets feature the same peripheral through different blocks' implementations for example the SPI may be provided on a single MCU by its USART, QSPI and SSP peripherals.
To ensure that the driver's implementation is valid for all these peripherals we want the CI to run the test set on each peripheral using at least one set of pin determined at run time (pin may eventually picked randomly).

### Requirements and assumptions

1. We want to list all pins for a function on a peripheral.
   For instance, all pins that can be configured as MOSI for SPI1.
2. We want to list all functions a pin can provide.
3. We want to list all pins for a form-factor regardless of the function they can provide.
4. We want a printable name for each form factor pin and a method to get that name.
5. We want a list of the reserved pins that cannot be tested.

Any of the list mentioned above may be empty if none match the criteria.

# System architecture and high-level design

### Pinmap

All HAL APIs which use pins have functions to get the corresponding pin maps. These functions return a `PinMap` array with each entry containing a pin name, a peripheral and a function. The end of the pin map array is indicated by the presence of a NC pin. Below is an example implementation of the function to get the serial tx pinmap:

```C
const PinMap PinMap_UART_TX[] = {
    {P0_19, UART_0, 1},
    {P1_13, UART_0, 3},
    {P1_27, UART_0, 2},
    { NC  , NC    , 0}
};
const PinMap *serial_tx_pinmap()
{
    return PinMap_UART_TX;
}
```

Targets which don't make use of a pinmap, such as ones with peripherals that can be connected to any pin, must still define pinmaps as these are needed for testing. For these devices the pinmap does not need to be comprehensive. Instead it should list a representative sample of pins and peripherals so they can be tested appropriately.

### Form Factor

A boards form factor determines what can be tested automatically. A board can have one or more form factors, each listed in the `supported_form_factors` entry in targets.json:

```
"supported_form_factors": ["ARDUINO"],
```

The Mbed pinamp code has built in support for common form factors, such as `ARDUINO`. When a known form factor is present in the `supported_form_factors` list then it will be used automatically for testing. Use the target configuration value `default-form-factor` to test a custom form factor or to force the tools to use a given form factor.

Use the function `const PinList *pinmap_ff_default_pins(void)` to get the pins of a form factor for testing. Additionally the name of a given form factor pin can be found with the function `const char *pinmap_ff_default_pin_to_string(PinName pin)`.

### Restricted Pins

Some boards have pins which cannot be tested without causing problems elsewhere. One example of this is the serial TX and RX pins used for communication during testing. If these pins are used during a test then communication with the host PC will be lost. To prevent pins like this from being used a target can override the weak function `pinmap_restricted_pins()` to return a pin list containing all target pins which should be skipped during testing. By default this function only includes the TX and RX pins used for communication during testing:

```c
MBED_WEAK const PinList *pinmap_restricted_pins()
{
    static const PinName pins[] = {
        USBTX, USBRX
    };
    static const PinList pin_list = {
        sizeof(pins) / sizeof(pins[0]),
        pins
    };
    return &pin_list;
}
```
