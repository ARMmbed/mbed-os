# Pin Names design document

# Table of contents

1. [Table of contents](#table-of-contents).
    * [Revision history](#revision-history).
1. [Introduction](#introduction).
    1. [Overview and background](#overview-and-background).
    1. [Requirements and assumptions](#requirements-and-assumptions).
1. [Detailed design](#detailed-design).
    1. [Arduino Uno (Rev 3) Header Pins](#arduino-uno-rev-3-header-pins).
        1. [I2C Definition for Arduino Uno (Rev3) Pins](#i2c-definition-for-arduino-uno-rev3-pins)
        1. [SPI Definition for Arduino Uno (Rev3) Pins](#spi-definition-for-arduino-uno-rev3-pins)
    1. [Custom/Vendor Header Pins](#custom/vendor-header-pins).
        1. [Custom/Vendor Connector I2C Definition](#custom/vendor-connector-i2c-definition).
        1. [Custom/Vendor Connector SPI Definition](#custom/vendor-connector-spi-definition).
    1. [Board Components Pins](#board-components-pins).
        1. [LED Definition](#led-definition)
        1. [BUTTON Definition](#button-definition)
1. [Other information](#other-information).
    1. [Avoid Unnecessary Definitions](#avoid-unnecessary-definitions).


### Revision history

1.0 - Initial revision - Malavika Sajikumar - 3/5/2020.  
This document was written for MBED OS version 5.15 (released on 11th of February 2020).

# Introduction

### Overview and background

Mbed is designed so that application code written in the platform is portable across different Mbed supported boards with the same hardware capabilities or interfaces. However, the code, in most cases, is not truly portable due to the differences in pin name definitions for the same kind of interfaces across different boards. 

This document provides a standard for defining pin names for the commonly used Mbed interfaces like I2C, SPI, etc. and board components like buttons and LEDs in a board support package.

### Requirements and assumptions

This document mainly applies to the pin standards required for Arduino Uno header which is shared among multiple Mbed enabled boards. The Arduino Uno header has been stable since 2012 at its current revision, which is the Arduino Uno Rev3. All design choices in this document for the Arduino Uno header are based on the Arduino Uno Rev3 header implementation.

# Detailed design

To achieve meaningful portability of application code across various Mbed enabled boards, the pin names of commonly used interfaces and board components should be common across these boards. This document describes a set of rules on how to name these pins in the board support package, specifically in PinNames.h file of the board.

### Arduino Uno (Rev3) Header Pins

All Arduino Uno (Rev3) form factor Mbed controller boards should define D0-D15 & A0-A5 pins as a default standard.

    // Arduino Uno (Rev3) header pin connection naming  
    // Px_xx relates to the processor pin connected to the Arduino Uno (Rev3) header pin
    
    D0 = Px_xx,  // UART RX / GPIO 0  
    D1 = Px_xx,  // UART TX / GPIO 1  
    D2 = Px_xx,  // GPIO 2  
    D3 = Px_xx,  // TMR / PWM / GPIO 3  
    D4 = Px_xx,  // GPIO 4  
    D5 = Px_xx,  // TMR / PWM / GPIO 5  
    D6 = Px_xx,  // TMR / PWM / GPIO 6  
    D7 = Px_xx,  // GPIO 7  
    D8 = Px_xx,  // GPIO 8  
    D9 = Px_xx,  // TMR / PWM / GPIO 9  
    D10 = Px_xx, // SPI CS / TMR / PWM / GPIO 10  
    D11 = Px_xx, // SPI MOSI / TMR / PWM / GPIO 11  
    D12 = Px_xx, // SPI MISO / GPIO 12  
    D13 = Px_xx, // SPI SCK / GPIO 13  
    D14 = Px_xx, // I2C SDA / GPIO 14  
    D15 = Px_xx, // I2C SCL / GPIO 15  
    
    A0 = Px_xx,  // ADC 0 / GPIO 16  
    A1 = Px_xx,  // ADC 1 / GPIO 17  
    A2 = Px_xx,  // ADC 2 / GPIO 18  
    A3 = Px_xx,  // ADC 3 / GPIO 19  
    A4 = Px_xx,  // ADC 4 / GPIO 20  
    A5 = Px_xx,  // ADC 5 / GPIO 21

**I2C Definition for Arduino Uno (Rev3) Pins**

All I2C definitions for the Arduino Uno (Rev3) header pins should be defined as follows:

    // Arduino Uno (Rev3) header pin connection naming   
    // Px_xx relates to the processor pin connected to the Arduino Uno (Rev3) header pin
    
    D14 = Px_xx, // I2C SDA / GPIO 14  
    D15 = Px_xx, // I2C SCL / GPIO 15

    // I2C signals are aliased as below  
    I2C_SDA = D14, // I2C SDA  
    I2C_SCL = D15, // I2C SCL

**SPI definition for Arduino Uno (Rev3) Pins**  

All SPI definitions for the Arduino Uno (Rev3) header pins should be defined as follows:

    // Arduino Uno (Rev3) header pin connection naming  
    // Px_xx relates to the processor pin connected to the Arduino Uno (Rev3) header pin
    
    D10 = Px_xx,    // SPI CS / TMR / PWM / GPIO 10  
    D11 = Px_xx,    // SPI MOSI / TMR / PWM / GPIO 11  
    D12 = Px_xx,    // SPI MISO / GPIO 12  
    D13 = Px_xx,    // SPI SCK / GPIO 13  

    // SPI signals are aliased as below  
    
    SPI_CS = D10,   // SPI CS  
    SPI_MOSI = D11, // SPI MOSI  
    SPI_MISO = D12, // SPI MISO  
    SPI_SCK = D13,  // SPI SCK

### Custom/Vendor Header Pins

**Custom/Vendor Connector I2C Definition**

All I2C definitions for a new connector should be defined as follows:

    // ConnectorName is the name of the specific connector you are implementing signals for here  
    // Px_xx relates to the processor pin connected to the custom/vendor connector pin  
    
    ConnectorName_I2C0_SCL  = Px_xx,  // Connector I2C0 SCL  
    ConnectorName_I2C0_SDA  = Px_xx,  // Connector I2C0 SDA  
    ConnectorName_I2C1_SCL  = Px_xx,  // Connector I2C1 SCL  
    ConnectorName_I2C1_SDA  = Px_xx,  // Connector I2C1 SDA  
    .  
    .  
    .  
    ConnectorName_I2CN_SCL  = Px_xx,  // Connector I2CN SCL  
    ConnectorName_I2CN_SDA  = Px_xx,  // Connector I2CN SDA  

**Custom/Vendor Connector SPI Definition**

All SPI definitions for a new connector should be defined as follows:

    // ConnectorName is the name of the specific connector you are implementing signals for here  
    // Px_xx relates to your processors pin connected to the custom/vendor connector pin
    
    ConnectorName_SPI0_CS   = Px_xx,  // Connector SPI0 CS  
    ConnectorName_SPI0_MOSI = Px_xx,  // Connector SPI0 MOSI  
    ConnectorName_SPI0_MISO = Px_xx,  // Connector SPI0 MISO  
    ConnectorName_SPI0_SCK  = Px_xx,  // Connector SPI0 SCK  
    
    ConnectorName_SPI1_CS   = Px_xx,  // Connector SPI1 CS  
    ConnectorName_SPI1_MOSI = Px_xx,  // Connector SPI1 MOSI  
    ConnectorName_SPI1_MISO = Px_xx,  // Connector SPI1 MISO  
    ConnectorName_SPI1_SCK  = Px_xx,  // Connector SPI1 SCK  
    .  
    .  
    .  
    
    ConnectorName_SPIN_CS   = Px_xx,  // Connector SPIN CS  
    ConnectorName_SPIN_MOSI = Px_xx,  // Connector SPIN MOSI  
    ConnectorName_SPIN_MISO = Px_xx,  // Connector SPIN MISO  
    ConnectorName_SPIN_SCK  = Px_xx,  // Connector SPIN SCK  

### Board Components Pins

**LED Definition**

All LED definitions should be defined as follows:

    // Px_xx relates to the processor pin connected to the LED
    
    LED1 = Px_xx,  // LED1  
    LED2 = Px_xx,  // LED2  
    LED3 = Px_xx,  // LED3  
    LED4 = Px_xx,  // LED4  
    .  
    .  
    LEDN = Px_xx   // LEDN

**BUTTON Definition**

All BUTTON definitions should be defined as follows:

    // Px_xx relates to the processor pin connected to the Button  
    BUTTON1 = Px_xx,  // BUTTON1  
    BUTTON2 = Px_xx,  // BUTTON2  
    BUTTON3 = Px_xx,  // BUTTON3  
    BUTTON4 = Px_xx,  // BUTTON4   
    .  
    .  
    BUTTONN = Px_xx   // BUTTONN  

# Other information

### Avoid Unnecessary Definitions

If only 1 LED/BUTTON is on board, do not define additional LEDs/BUTTONs in BSP. This allows for unavailable LEDs/BUTTONs to be caught as a compile error.

    // Do not do the following
    
    LED1 = Px_xx,  // LED1  
    LED2 = LED1,   // LED2  
    LED3 = LED1,   // LED3  
    LED4 = LED1,   // LED4   
    
    BUTTON1 = Px_xx,     // BUTTON1  
    BUTTON2 = BUTTON1,   // BUTTON2  
    BUTTON3 = BUTTON1,   // BUTTON3  
    BUTTON4 = BUTTON1,   // BUTTON4  
