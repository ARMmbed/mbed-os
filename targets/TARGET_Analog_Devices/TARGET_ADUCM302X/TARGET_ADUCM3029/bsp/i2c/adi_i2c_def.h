/*!
 *****************************************************************************
 @file:    adi_i2c_def.h
 @brief:   Internal I2C device driver definitions and macros
 -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.
THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/
#ifndef ADI_I2C_DEF_H
#define ADI_I2C_DEF_H

/*! \cond PRIVATE */

#include <drivers/i2c/adi_i2c.h>

#define ADI_I2C_NUM_INSTANCES       (1u)
#define ADI_I2C_STATUS_ERROR_MASK ( (1u << BITP_I2C_MSTAT_NACKADDR) \
                                  | (1u << BITP_I2C_MSTAT_NACKDATA) \
                                  | (1u << BITP_I2C_MSTAT_ALOST)    )

/* Internal Actions */
static void           submitTransaction  (ADI_I2C_HANDLE const hDevice, ADI_I2C_TRANSACTION* const pTransaction);
static void           commenceTransmit   (ADI_I2C_HANDLE const hDevice);
static void           commenceReceive    (ADI_I2C_HANDLE const hDevice);
static ADI_I2C_RESULT i2cReset           (ADI_I2C_HANDLE const hDevice);

/* interrupt event handlers */
static void           transmitHandler    (ADI_I2C_HANDLE const hDevice);
static void           receiveHandler     (ADI_I2C_HANDLE const hDevice);
static void           completeHandler    (ADI_I2C_HANDLE const hDevice);
static void           errorHandler       (ADI_I2C_HANDLE const hDevice);

#if defined(__ADUCM302x__)
/*
 *  SYS Device Structure
 */
typedef struct _ADI_SYS_STRUCT
{
        ADI_SYS_TypeDef       *pReg;      /* Pointer to register base */
} ADI_SYS_STRUCT;

/* alias for the actual device structure */
typedef struct _ADI_SYS_STRUCT  ADI_SYS_REGISTERS;

#endif
/*
 *****************************************************************************
 * I2C Configuration structure.
 *****************************************************************************/
typedef struct __ADI_I2C_CONFIG {
    uint16_t MasterControlRegister;  /* I2C_MCTL register configuration.      */
    uint16_t ClockDividerRegister;   /* I2C_DIV  register.                    */
    uint16_t SharedControlRegister;  /* I2C_DIV  register.                    */
    uint16_t TimingControlRegister;  /* I2C_TCTL register.                    */
    uint16_t ClockStretchRegister;   /* I2C_ASTRETCH register.                */
    uint16_t TargetSlaveAddress;     /* slave address value (not a register). */
} ADI_I2C_CONFIG;


/* I2C physical device instance data */
typedef struct __ADI_I2C_DEVICE_INFO {
    IRQn_Type             pioIRQn;  /* PIO interrupt number                */
    ADI_I2C_TypeDef      *pDev;     /* pointer to i2c controller           */
    ADI_I2C_CONFIG       *pConfig;  /* pointer to user config info         */
    ADI_I2C_HANDLE        hDevice;  /* I2C handle or NULL if uninitialized */
} ADI_I2C_DEVICE_INFO;

/* I2C driver instance data structure */
typedef struct __ADI_I2C_DEV_DATA_TYPE {

    /* make sure to synchronize ANY size changes with ADI_I2C_MEMORY_SIZE macro in adi_i2c.h */

    /* device attributes */
    ADI_I2C_TypeDef      *pDev;
    ADI_I2C_DEVICE_INFO  *pDevInfo;


    /* driver state */
    uint16_t              hwStatus;
    bool                  bRepeatStart;
    uint16_t              i2cDeviceAddress;
    uint16_t              i2cEncodedDeviceAddress;    /* encoded as 7-bit device address + r/w LSB */
    bool                  bSubmitCalled;

    /* prologue data */
    volatile uint8_t     *pNextPrologueByte;
    volatile uint16_t     remainingPrologueCount;

    /* write data */
    volatile uint8_t     *pNextWriteByte;
    volatile uint16_t     remainingWriteCount;

    /* read data */
    volatile uint8_t     *pNextReadByte;
    volatile uint16_t     remainingReadCount;

    ADI_I2C_RESULT        result;    /* collector for return status */
    ADI_I2C_HW_ERRORS     hwErrors;  /* collector for error status */

    SEM_VAR_DECLR        /* blocking object: "Semaphore" for rtos,  "nLowPowerExitFlag" for non-rtos */

} ADI_I2C_DEV_DATA_TYPE;

/*! \endcond */

#endif    /* end of ifndef ADI_I2C_DEF_H */

