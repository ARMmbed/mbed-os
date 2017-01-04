/*!
 *****************************************************************************
 @file:    adi_i2c_def_v1.h
 @brief:   Internal I2C device driver definitions and macros
 @version: $Revision: 33205 $
 @date:    $Date: 2016-01-11 10:46:07 +0000 (Mon, 11 Jan 2016) $
 -----------------------------------------------------------------------------
Copyright (c) 2014-2015 Analog Devices, Inc.

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

#ifndef __ADI_I2C_DEF_H__
#define __ADI_I2C_DEF_H__

/*! \cond PRIVATE */

#include <drivers/i2c/adi_i2c.h>
#include <services/dma/adi_dma.h>

#if defined(__ECC__)
#define ALIGN4  _Pragma("align(4)")
#elif defined(__ICCARM__)
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__GNUC__)
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__CC_ARM)
#define ALIGN4 __attribute__((aligned (4)))
#endif


#define NUM_I2C_BUFFERS      (2)

/*
 * The blocking mode is set depending on the specific API's called
 * by the application.
 */
typedef enum
{
    BLOCK_MODE_NONE,
    BLOCK_MODE_BLOCKING,
    BLOCK_MODE_NON_BLOCKING
} BLOCK_MODE;

/*
 * Enum for the different buffer info or errors that occurred
 */
typedef uint16_t BUFF_INFO;

#define BUFF_INFO_NONE       (0x0000u)
#define BUFF_INFO_ERR        (0x0001u)
#define BUFF_INFO_NACK_ADDR  (BUFF_INFO_ERR | 0x0100u)
#define BUFF_INFO_NACK_DATA  (BUFF_INFO_ERR | 0x0200u)
#define BUFF_INFO_NACK_ALOST (BUFF_INFO_ERR | 0x0400u)


/*
 * I2C configuration structure
 */
typedef struct {
    ADI_I2C_MODE     mode;              /* dataflow mode/method (Control call) */
    uint32_t         frequency;         /* the frequency of the I2C in Hz (default: 40 KHz) */
    uint16_t         slaveAddr;         /* Address to be used by the I2C */
    uint16_t         duty_cycle;        /* the duty cycle of the twi in % (default=50) */
    bool_t           b10bitSlaveAddr;   /* true if the slave address is 10-bit wide */
    bool_t           bDmaEnabled;       /* Flag to determine whether the DMA is enabled or not */
    bool_t           bDmaInitialized;   /* Flag to determine whether the DMA is initialized or not */
                                        /* Store the first master address byte which is required for repeat start */
    uint16_t         firstMasterAddressByte;
    uint32_t         MRxCountRemaining; /* Master RX Count Remaining (in bytes) */

    uint16_t         nRegControl;       /* Control register value */
    uint32_t         nGlobalEvent;      /* Global Events */
} ADI_I2C_CONFIG_STRUCT;

/*
 * I2C buffer structure
 */
typedef struct __ADI_I2C_BUFFER_STRUCT{
	struct __ADI_I2C_BUFFER_STRUCT  *pNext;             /* Pointer to the next buffer structure */
	uint8_t                         *pData;             /* Pointer to the data buffer */
	uint8_t                         *pCurData;          /* Pointer to the current position in the data buffer */
	uint32_t                         BytesRemaining;    /* Number of bytes remaining */
	uint16_t                         Info;              /* Info structure for the buffer */
	uint8_t                          Direction;         /* Direction of buffer (OUTBOUND or INBOUND) */
	bool_t                           Restart;           /* Boolean flag to determine whether repeat start is enabled or not */
	bool_t                           FastProcess;       /* If fast process of buffer is enabled */
} ADI_I2C_BUFFER_STRUCT;
/* Pre-declaration of _ADI_I2C_DRIVER_STRUCT structure */
struct _ADI_I2C_DRIVER_STRUCT;

/* Manage FIFO function */
typedef ADI_I2C_RESULT(*ManageFIFO)(struct _ADI_I2C_DRIVER_STRUCT const *drv, bool_t bStart);

/*
 * I2C data structure
 */
typedef struct _ADI_I2C_DATA_STRUCT{
        bool_t                bDeviceEnabled;       /* Boolean flag to determine whether the
                                                       device is enabled or not */
	ADI_I2C_CONFIG_STRUCT config;               /* Pointer to the configuration structure */

	ADI_I2C_BUFFER_STRUCT *activeBuffer;        /* Pointer to active buffer */
	ADI_I2C_BUFFER_STRUCT *freeBuffers;         /* Free Buffer List */
	ADI_I2C_BUFFER_STRUCT *pendingBuffers;      /* Pending buffer list */
	ADI_I2C_BUFFER_STRUCT *completedBuffers;    /* Completed buffer list */

	PEND_VAR_DECLR                              /* Current semaphore count */

#if (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1)
        ADI_DMA_CHANNEL_HANDLE hDMAHandle;          /* DMA Handle */
                                                    /* DMA Memory */
        ALIGN4
        uint8_t DMAMemory[ADI_DMA_MEMORY_SIZE];
#endif

	ADI_CALLBACK           Callback;            /* Callback function pointer */
	void                  *pCallParam;          /* Application callback parameter */
	BLOCK_MODE             eBlockMode;          /* Blocking mode to be used */

    ManageFIFO             pfManageFIFO;        /* Manage FIFO function */

	ADI_I2C_BUFFER_STRUCT  Buffers[NUM_I2C_BUFFERS];  /* Internal buffers */
} ADI_I2C_DATA_STRUCT;

/*
 *  I2C Device Structure
 */
typedef struct _ADI_I2C_DRIVER_STRUCT
{
	ADI_I2C_TypeDef       *pReg;        /* Pointer to register base */
	ADI_I2C_DATA_STRUCT   *pData;       /* Pointer to device structure */
} ADI_I2C_DRIVER_STRUCT;

/* alias for the actual device structure */
typedef struct _ADI_I2C_DRIVER_STRUCT  ADI_I2C_DRIVER;

/* The amount of memory required by the application */
#define DATA_MEM_SIZE (sizeof(ADI_I2C_DATA_STRUCT))

/*! \endcond */

#endif
