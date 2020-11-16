# General Pin Names design document

# Table of contents

1. [Table of contents](#table-of-contents).
    1. [Revision history](#revision-history).
1. [Introduction](#introduction).
    1. [Overview and assumptions](#overview-and-assumptions)
1. [Detailed design](#detailed-design).
    1. [LEDs](#leds).
    1. [Buttons](#buttons).
1. [Non-valid definitions](#non-valid-definitions).
1. [Testing compliance](#testing-compliance).


### Revision history

Initial revision: 1.0

Authors: Malavika Sajikumar (ADI), Sean Doyle (ADI), Kyle Jansen (ADI), Jerome Coutant (ST), Marcelo Salazar (Arm).

This document is written for Mbed OS 6.

## Introduction

### Overview and assumptions

Mbed OS is designed so that application code written in the platform is portable across different Mbed supported boards with the same hardware capabilities or interfaces. However, the code may not be truly portable due to the differences in pin name definitions for the same kind of interfaces across different boards. 

This document provides general guidelines on best practices for defining pin names that could apply to all boards but it's not specific to any type of connector.

Note there might be separate documents for pin names that apply to specific connectors such as the Arduino Uno. These are available in the [HAL design documents](./) folder.

## Detailed design

To achieve meaningful portability of application code across various Mbed Enabled boards, certain pin names of commonly used interfaces and board components should be common across these boards. This document describes a set of rules on how to define components found on actual boards, using the PinNames.h file as part of the BSP of the board.

### LEDs

**Definition of LEDs**

Only add LEDs that are available on the board. This is an example on how to define LEDs in PinNames.h:

    // Px_xx relates to the processor pin connected to the LED
    #define LED1 Px_xx  // LED1
    #define LED2 Px_xx  // LED2
    #define LED3 Px_xx  // LED3
    #define LED4 Px_xx  // LED4
    .  
    .  
    #define LEDN Px_xx  // LEDN

Note this document is proposing changing LEDs from `enums` to `define`, which causes a minor change in the compile preprocessor. From application point of view, there are no implications.

**Using LEDs at application**

The detection of available LEDs at application level can be done as follow:

    #ifdef LED1
        DigitalOut myLED(LED1);
        myLED = 1;
    #endif 

Alternatively, if the usage of an LED is required, then the application can detect its availability and generate an error accordingly:

    #ifndef LED1
        #error This application requires the availability of an LED
    #endif 

It's possible to define new names of LEDs related to its properties, like color or functionality. They can be defined as aliases in the application as shown below:

    #define RED_LED    LED1
    #define STATUS_LED LED2 

However, these names do not apply to all boards and hence should not be used in official example applications developed by Arm nor Partners, as these are considered platform agnostic.

### Buttons

**Definition of buttons**

Only add buttons that are available on the board. This is an example on how to define buttons in PinNames.h:

    // Px_xx relates to the processor pin connected to the button  
    #define BUTTON1 Px_xx  // BUTTON1
    #define BUTTON2 Px_xx  // BUTTON2
    #define BUTTON3 Px_xx  // BUTTON3
    #define BUTTON4 Px_xx  // BUTTON4
    .  
    .  
    #define BUTTONN Px_xx  // BUTTONN

Note this document is proposing changing buttons from `enums` to `define`, which causes a minor change in the compile preprocessor. From application point of view, there are no implications.

**Using Buttons at application**

The detection of available buttons at application level can be done as follow:

    #ifdef BUTTON1
        DigitalIn myBUTTON(BUTTON1);
        int input = myBUTTON.read();
    #endif 

Alternatively, if the usage of a button is required, then the application can detect its availability and generate an error accordingly:

    #ifndef BUTTON1
        #error This application requires the availability of a button
    #endif 

It's possible to define new names of buttons related to its properties. It's recommended to add a comment with a description. They can be defined as aliases at application level as shown below:

    #define PUSH_BUTTON BUTTON1 // Momentary push Button

However, these names do not apply to all boards and hence should not be used in official example applications developed by Arm nor Partners, as these are considered platform agnostic.

### UART

Every Mbed board includes a serial interface to the host PC, which allows the console to print useful information about the application.

The UART pins must be defined to be able to run automated tests using Greentea, as the communication between the host PC and the MCU is done over serial.

This is an example on how to define UART names in PinNames.h:

    typedef enum {
    ...
    // Px_xx relates to the processor pin connected to the UART
    USBTX       = Px_xx,
    USBRX       = Px_xx,
    ...
    } PinName;

Note this document is proposing unifying the pin names used for UART communication between the MCU and the host PC.

### Non-valid definitions

If either LEDs or buttons are not available, they should not be defined.
This allows for unavailable LEDs or BUTTONs to be caught in Mbed OS allowing the corresponding errors to be generated.
   
    #define LED1 PB_0  // LED1 is valid
    #define LED2 LED1  // Not valid as it's duplicate and LED2 does not exist on the board
    #define LED3 PB_0  // Not valid as it's duplicate and LED3 does not exist on the board
    #define LED4 NC    // Not valid definition as LED4 does not exist

    #define BUTTON1 PB_1    // BUTTON1 is valid
    #define BUTTON2 BUTTON1 // Not valid as it's duplicate and BUTTON2 does not exist on the board
    #define BUTTON3 PB_1    // Not valid as it's duplicate and BUTTON3 does not exist on the board
    #define BUTTON4 NC      // Not valid as BUTTON4 does not exist


### Testing compliance

A python script could check, during CI process, whether a board has valid LEDs and BUTTONS defined (none equal to NC, and no duplicated pin values).

A Greentea test could check if LED and BUTTON pins are valid:

    mbed test -t <toolchain> -m <target> -n hal-tests-tests-pin_names-generic

Note the testing of UART is implicit when running Greentea tests.

LEDs or buttons should not be defined if they don't exist on the board. If they are defined but contain no valid pinnames, then a warning should be displayed.

In a new version of Mbed OS, the tests should be enabled to check compliance and generate errors when non-valid LEDs or buttons are being used.
