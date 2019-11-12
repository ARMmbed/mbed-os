# SDIO API

# Table of contents

- [SDIO API](#sdio-api)
- [Table of contents](#table-of-contents)
    - [Revision history](#revision-history)
- [Introduction](#introduction)
    - [Overview and background](#overview-and-background)
    - [Requirements and assumptions](#requirements-and-assumptions)
- [System architecture and high-level design](#system-architecture-and-high-level-design)
    - [System architecture and component interaction](#system-architecture-and-component-interaction)
- [Detailed design](#detailed-design)
    - [Detailed design 1 (For example, `Configuring the device`)](#detailed-design-1-for-example-configuring-the-device)
    - [Detailed design 2 (For example, `Starting and stopping the device`)](#detailed-design-2-for-example-starting-and-stopping-the-device)
    - [Detailed design 3 (For example, `Reading from and Writing to the device`)](#detailed-design-3-for-example-reading-from-and-writing-to-the-device)
    - [Detailed design N (For example, `Resetting the device`)](#detailed-design-n-for-example-resetting-the-device)
- [Usage scenarios and examples](#usage-scenarios-and-examples)
    - [Scenario 1 (For example, `Reading from the device`)](#scenario-1-for-example-reading-from-the-device)
    - [Scenario 2 (For example, `Writing to the device`)](#scenario-2-for-example-writing-to-the-device)
- [Tools and configuration changes](#tools-and-configuration-changes)
    - [Tool change 1](#tool-change-1)
    - [Configuration changes](#configuration-changes)
- [Other information](#other-information)
    - [Reusability](#reusability)
    - [Deprecations](#deprecations)
    - [References](#references)
    - [Custom headings](#custom-headings)

### Revision history

1.0 - Initial revision - Mahesh Mahadevan - 11/12/2019.

# Introduction

### Overview and background

SDIO (Secure Digital I/O) is and extension to SD standard that provides high speed I/O interface with low power consumption. SDIO can be used for both I/O functionality as well as for standard storage. Currently Mbed OS supports SD memory cards only in the SPI mode.

### Requirements and assumptions

This feature requires a SDIO interface on the target as this feature implements a block device driver over the SDIO interface.

# System architecture and high-level design

A SDIOBlockDevice driver is implememted to interface with the mbed-os storage framework. This driver will interact with the target
hardware using the HAL API. The HAL driver should provide the following

- `Configuring the device` - The driver should implement the interfaces to intialize the device at start and deinitialize on close.
- `Reading from and Writing to the device` - Read and Write interfaces should be implemented to support blocking mode.
- `Asynchronous operations` - Non-blocking read and write interfaces
- `Read and Write status` - Interface to read the read & write status which is used during non-blocking operations.
- `Erase` - Interface to erase specified memory area
- `Get Card State` - Gets the current data status of the card
- `Get Card Info` - Gets information about a SD card


# Detailed design

### API description

The HAL driver API is as follows:

### Configuring the device

int sdio_init(void): Initializes the SD card device. Returns the status

int sdio_deinit(void): DeInitializes the SD card device. Returns the status


### Reading from and Writing to the device

int sdio_readblocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks):  Reads block(s) from a specified address
in a SD card in polling mode. pData is a pointer to the buffer where the read data will be stored, ReadAddr is an address
from where data is to be read and NumOfBlocks indicates the number of SD blocks to read. The function returns the read status.

int sdio_writeblocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks): Writes block(s) to a specified address in
an SD card in polling mode. pData is a pointer to the buffer that will contain the data to transmit, WriteAddr is an address
to where data is to be written and NumOfBlocks indicates the number of SD blocks to write. The function returns the write status.


### Asynchronous operations

Below are asynchronous API's that will work in DMA mode.

int sdio_readblocks_async(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks): Reads block(s) from a specified address
in an SD card in non-blocking mode. This API will use DMA to transfer data. pData is a pointer to the buffer where the read
data will be stored, ReadAddr is an address from where data is to be read and NumOfBlocks indicates the number of SD blocks
to read. The function returns the read status.


int sdio_writeblocks_async(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks): Writes block(s) to a specified address
in an SD card in non-blocking mode. This API will use DMA to transfer data. pData is a pointer to the buffer that will contain
the data to transmit, WriteAddr is an address to where data is to be written and NumOfBlocks indicates the number of SD blocks
to write. The function returns the write status.

int sdio_read_pending(void): Check if a DMA read operation is pending. The return values can be one of the following:
   SD_TRANSFER_OK: No data transfer is active
   SD_TRANSFER_BUSY: Data transfer is active

int sdio_write_pending(void): Check if a DMA write operation is pending. The return values can be one of the following:
   SD_TRANSFER_OK: No data transfer is active
   SD_TRANSFER_BUSY: Data transfer is active


### Erase operation

int sdio_erase(uint32_t StartAddr, uint32_t EndAddr): Erases the specified memory area of the given SD card. StartAddr is the
start byte address and EndAddr is the end byte address. The function return erase status


### Get Card State

int sdio_get_card_state(void): Gets the current SD card data status. The return values can be one of the following:
   SD_TRANSFER_OK: Card is read for data transfer
   SD_TRANSFER_ERROR: Card is not ready


### Get Card Info
void sdio_get_card_info(SDIO_Cardinfo_t *CardInfo): Get information about specific SD card. CardInfo is a pointer to
HAL SD_CardInfo structure

typedef struct {
    uint32_t CardType;     /* Specifies the card Type                         */
    uint32_t CardVersion;  /* Specifies the card version                      */
    uint32_t Class;        /* Specifies the class of the card class           */
    uint32_t RelCardAdd;   /* Specifies the Relative Card Address             */
    uint32_t BlockNbr;     /* Specifies the Card Capacity in blocks           */
    uint32_t BlockSize;    /* Specifies one block size in bytes               */
    uint32_t LogBlockNbr;  /* Specifies the Card logical Capacity in blocks   */
    uint32_t LogBlockSize; /* Specifies logical block size in bytes           */
} SDIO_Cardinfo_t;
