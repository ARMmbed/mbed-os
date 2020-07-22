# Pin Names design document

# Table of contents

1. [Table of contents](#table-of-contents).
    * [Revision history](#revision-history).
1. [Introduction](#introduction).
    1. [Overview and background](#overview-and-background).
    1. [Requirements and assumptions](#requirements-and-assumptions).
1. [Detailed design](#detailed-design).
    1. [Arduino Uno (Rev 3) connector Pins](#arduino-uno-rev-3-connector-pins).
        1. [I2C and SPI Definition for Arduino Uno (Rev3) Pins](#i2c-definition-for-arduino-uno-rev3-pins)
    1. [Board Components Pins](#board-components-pins).
        1. [LED Definition](#led-definition)
        1. [BUTTON Definition](#button-definition)
1. [Other information](#other-information).
    1. [Avoid Unnecessary Definitions](#avoid-unnecessary-definitions).


### Revision history

1.0 - Initial revision - Malavika Sajikumar - 7/20/2020.  
This document was written for MBED OS version 5.15 (released on 11th of February 2020).

# Introduction

### Overview and background

Mbed is designed so that application code written in the platform is portable across different Mbed supported boards with the same hardware capabilities or interfaces. However, the code, in most cases, is not truly portable due to the differences in pin name definitions for the same kind of interfaces across different boards. 

This design document provides rules and guidelines on how to define Arduino Uno connector pins in the board support package of development boards to achieve true code portability across various boards with Arduino Uno connectors and code targeting pins on the same. 

### Requirements and assumptions

This document applies to the pin standards required for Arduino Uno connector which is used on multiple Mbed enabled boards. The Arduino Uno connector has been stable since 2012 at its current revision, which is the Arduino Uno Rev3. All design choices in this document for the Arduino Uno connector are based on the Arduino Uno Rev3 connector implementation.

If the developement board is defined as Arduino Uno compliant, the Arduino Uno connector standard has to be followed and ARDUINO_UNO should be defined as a supported form factor in targets.json file.

    "supported_form_factors": [
            "ARDUINO_UNO"
        ],

The Arduino Uno connector pins and pin aliases are defined in two files - PinNames.h and PinNamesTypes.h. These files reside in the following locations:

    targets/MCU_VENDOR/MCU_FAMILY/MCU/Board/PinNames.h
    targets/MCU_VENDOR/PinNamesTypes.h


# Detailed design

To achieve meaningful portability of application code across various Mbed enabled boards that are Arduino Uno compliant, the pin names used for the connector pins should be common across these boards. This document describes a set of rules on how and where to define these pins in the board support package.

### Arduino Uno (Rev3) connector Pins

All Arduino Uno (Rev3) form factor Mbed controller boards should define D0-D15 & A0-A5 pins as a default standard. These pins should be defined in PinNames.h file within a PinName enum. The prefix ARDUINO_UNO_ distinguishes these pins from pins defined for other custom/common connectors that may have similar pin names. 

    // Arduino Uno (Rev3) connector pin connection naming  
    // Px_xx relates to the processor pin connected to the Arduino Uno (Rev3) connector pin

    ARDUINO_UNO_D0 = Px_xx,
    ARDUINO_UNO_D1 = Px_xx,
    ARDUINO_UNO_D2 = Px_xx,
    ARDUINO_UNO_D3 = Px_xx,
    ARDUINO_UNO_D4 = Px_xx,
    ARDUINO_UNO_D5 = Px_xx,
    ARDUINO_UNO_D6 = Px_xx,
    ARDUINO_UNO_D7 = Px_xx,
    ARDUINO_UNO_D8 = Px_xx,
    ARDUINO_UNO_D9 = Px_xx,
    ARDUINO_UNO_D10 = Px_xx,
    ARDUINO_UNO_D11 = Px_xx,
    ARDUINO_UNO_D12 = Px_xx,
    ARDUINO_UNO_D13 = Px_xx,
    ARDUINO_UNO_D14 = Px_xx,
    ARDUINO_UNO_D15 = Px_xx,

    ARDUINO_UNO_A0 = Px_xx,
    ARDUINO_UNO_A1 = Px_xx,
    ARDUINO_UNO_A2 = Px_xx,
    ARDUINO_UNO_A3 = Px_xx,
    ARDUINO_UNO_A4 = Px_xx,
    ARDUINO_UNO_A5 = Px_xx,

>>TODO: Should the Arduino Uno standard description be provided here?

If the development board has the Arduino Uno connector in hardware, but does not comply with the Arduino Uno standard, whether it be with alternate functionality pins or no connect pins, the board should not be defined as Arduino Uno compiant and ARDUINO_UNO should not be added as a supported form factor in targets.json. The pins with alternate functions have to be referenced using processor pin names (Px_xx) or pin name aliases in application code. 

>>TODO: Should adding D pin names still be an option? A comment describing alternative functionality for each pin should be provided in that case. 

>>TODO: How should the implementation of vendor defined drivers be modified so that NC pins can be caught as an error? For example: Have to redefine the vendor drivers (i2c_api.c, spi_api.c) to catch NC using MBED_ASSERT. Have to define gpio_is_connected() in gpio_api.c for all targets?

**I2C and SPI Definition for Arduino Uno (Rev3) Pins**

All I2C and SPI pin name alias definitions for the Arduino Uno (Rev3) connector pins should be defined within PinNamesTypes.h as follows:

    #ifdef TARGET_FF_ARDUINO_UNO
    // Arduino Uno I2C signals aliases
    #define ARDUINO_UNO_I2C_SDA D14
    #define ARDUINO_UNO_I2C_SCL D15

    // Arduino Uno SPI signals aliases
    #define ARDUINO_UNO_SPI_CS   D10
    #define ARDUINO_UNO_SPI_MOSI D11
    #define ARDUINO_UNO_SPI_MISO D12
    #define ARDUINO_UNO_SPI_SCK  D13
    #endif // TARGET_FF_ARDUINO_UNO

>>TODO: Should I2C and SPI pin name aliases for Arduino Uno connector be defined without the ARDUINO_UNO_ prefix? If so, how can this standardisation be distinguished from legacy non-Arduino-compliant board definitions?
