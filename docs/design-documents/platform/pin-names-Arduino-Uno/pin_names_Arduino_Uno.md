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

All Arduino Uno (Rev3) form factor Mbed controller boards should define D0-D15 & A0-A5 pins as a default standard. These pins should be defined in PinNames.h file within a PinName enum. The prefix Arduino_Uno_ distinguishes these pins from pins defined for other custom/common connectors that may have similar pin names. 

    // Arduino Uno (Rev3) connector pin connection naming  
    // Px_xx relates to the processor pin connected to the Arduino Uno (Rev3) connector pin

    Arduino_Uno_D0 = Px_xx,
    Arduino_Uno_D1 = Px_xx,
    Arduino_Uno_D2 = Px_xx,
    Arduino_Uno_D3 = Px_xx,
    Arduino_Uno_D4 = Px_xx,
    Arduino_Uno_D5 = Px_xx,
    Arduino_Uno_D6 = Px_xx,
    Arduino_Uno_D7 = Px_xx,
    Arduino_Uno_D8 = Px_xx,
    Arduino_Uno_D9 = Px_xx,
    Arduino_Uno_D10 = Px_xx,
    Arduino_Uno_D11 = Px_xx,
    Arduino_Uno_D12 = Px_xx,
    Arduino_Uno_D13 = Px_xx,
    Arduino_Uno_D14 = Px_xx,
    Arduino_Uno_D15 = Px_xx,

    Arduino_Uno_A0 = Px_xx,
    Arduino_Uno_A1 = Px_xx,
    Arduino_Uno_A2 = Px_xx,
    Arduino_Uno_A3 = Px_xx,
    Arduino_Uno_A4 = Px_xx,
    Arduino_Uno_A5 = Px_xx,

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

### Board Components Pins

>>Modifications from previous version: #defines for LEDs,BUTTONs. Decision on NC should be used or not. 
>>Does this make sense or should we stick to Kevin's suggestion? Should the code be able to catch:
static_assert(BUTTON1 != NC && BUTTON2 != NC, "This application needs two buttons");

>>If so, how many LEDs/BUTTONs would you define?

**LED Definition**

All LED definitions should be defined as follows in PinNames.h:

    // Px_xx relates to the processor pin connected to the LED
    
    #define LED1 = Px_xx  // LED1
    #define LED2 = Px_xx  // LED2  
    #define LED3 = Px_xx  // LED3  
    #define LED4 = Px_xx  // LED4  
    .  
    .  
    #define LEDN = Px_xx   // LEDN

>>Should be in applicatiopn code
A comment describing more LED properties, like color or functionality can also be provided. If color descriptive names are required, they can be defined as aliases, as shown below:

    #define RED_LED = LED1
    #define STATUS_LED = LED2

**BUTTON Definition**

All BUTTON definitions should be defined as follows in PinNames.h:

    // Px_xx relates to the processor pin connected to the Button  
    #define BUTTON1 = Px_xx  // BUTTON1  
    #define BUTTON2 = Px_xx  // BUTTON2  
    #define BUTTON3 = Px_xx  // BUTTON3  
    #define BUTTON4 = Px_xx  // BUTTON4   
    .  
    .  
    #define BUTTONN = Px_xx   // BUTTONN  

A comment describing more BUTTON properties can also be provided, as shown below:

    #define RESET_BUTTON = BUTTON1

# Other information

### Avoid Unnecessary Definitions

If only 1 LED/BUTTON is on board, do not define additional LEDs/BUTTONs in BSP. This allows for unavailable LEDs/BUTTONs to be caught as a compile error.

    // Do not do the following
    
    #define LED1 = Px_xx,  // LED1  
    #define LED2 = LED1,   // LED2  
    #define LED3 = LED1,   // LED3  
    #define LED4 = LED1,   // LED4   
    
    #define BUTTON1 = Px_xx,     // BUTTON1  
    #define BUTTON2 = BUTTON1,   // BUTTON2  
    #define BUTTON3 = BUTTON1,   // BUTTON3  
    #define BUTTON4 = BUTTON1,   // BUTTON4  

