
/**
 * @file xmc_sdmmc.h
 * @date 2017-02-14
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial version
 *     - Documentation updates
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *
 * 2016-01-16:
 *     - Added the following APIs to the XMC_SDMMC low level driver <br>
 *         1) XMC_SDMMC_EnableDelayCmdDatLines <br>
 *         2) XMC_SDMMC_DisableDelayCmdDatLines <br>
 *         3) XMC_SDMMC_SetDelay <br>
 *         4) XMC_SDMMC_EnableHighSpeed <br>
 *         5) XMC_SDMMC_DisableHighSpeed <br>
 *
 * 2016-04-07:
 *     - Added XMC_SDMMC_COMMAND_RESPONSE_t <br>
 *
 * 2016-07-11:
 *     - Adjust masks for the following functions: <br>
 *       1) XMC_SDMMC_SetBusVoltage <br>
 *       2) XMC_SDMMC_SetDataLineTimeout <br>
 *       3) XMC_SDMMC_SDClockFreqSelect <br>
 *
 * 2017-02-14:
 *     - Added: <br>
 *       1) XMC_SDMMC_SetCardDetectionStatus() <br>
 *       2) XMC_SDMMC_SetCardDetectionSource() <br>

 * @endcond
 */

#ifndef XMC_SDMMC_H
#define XMC_SDMMC_H

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/
 
#include "xmc_common.h"

#if defined (SDMMC)

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup SDMMC
 * @brief Secure Digital/Multi Media Card (SDMMC) driver for the XMC4500 microcontroller
 *
 * The SDMMC peripheral provides an interface between SD/SDIO/MMC cards and the AHB. It handles
 * the SD/SDIO protocol at transmission level. It automatically packs data and checks for CRC,
 * start/end bits and format correctness. For SD cards, a maximum transfer rate of 24MB/sec is
 * supported and for MMC cards, 48MB/sec.
 *
 * The peripheral can be used for applications that require large storage memory; e.g. Data logging,
 * firmware updates or an embedded database.
 *
 * The SDMMC low level driver provides functions to configure and initialize the SDMMC hardware
 * peripheral.
 * @{
 */

/*******************************************************************************
 * MACROS
 *******************************************************************************/

/**
 * A convenient symbol for the SDMMC peripheral base address
 */
#if defined (SDMMC)
# define XMC_SDMMC ((XMC_SDMMC_t *)SDMMC_BASE)
#else
# error 'SDMMC' base peripheral pointer not defined
#endif

/*
 * Check for valid ACMD errors <br>
 *
 * This macro is used in the LLD for assertion checks (XMC_ASSERT).
 */
#define XMC_SDMMC_CHECK_MODULE_PTR(p) ((p) == XMC_SDMMC)

/*
 * Check for valid ACMD errors <br>
 *
 * This macro is used in the LLD for assertion checks (XMC_ASSERT).
 */
#define XMC_SDMMC_CHECK_ACMD_ERR(v)\
  ((v == XMC_SDMMC_ACMD12_NOT_EXEC_ERR) ||\
   (v == XMC_SDMMC_ACMD_TIMEOUT_ERR)    ||\
   (v == XMC_SDMMC_ACMD_CRC_ERR)        ||\
   (v == XMC_SDMMC_ACMD_END_BIT_ERR)    ||\
   (v == XMC_SDMMC_ACMD_IND_ERR)        ||\
   (v == XMC_SDMMC_CMD_NOT_ISSUED_BY_ACMD12_ERR))

/*
 * Check for valid SDCLK divider frequency <br>
 *
 * This macro is used in the LLD for assertion checks (XMC_ASSERT).
 */
#define XMC_SDMMC_CHECK_SDCLK_FREQ(f)\
  ((f == XMC_SDMMC_CLK_DIV_1)           ||\
   (f == XMC_SDMMC_CLK_DIV_2)           ||\
   (f == XMC_SDMMC_CLK_DIV_4)           ||\
   (f == XMC_SDMMC_CLK_DIV_8)           ||\
   (f == XMC_SDMMC_CLK_DIV_16)          ||\
   (f == XMC_SDMMC_CLK_DIV_32)          ||\
   (f == XMC_SDMMC_CLK_DIV_64)          ||\
   (f == XMC_SDMMC_CLK_DIV_128)         ||\
   (f == XMC_SDMMC_CLK_DIV_256))

/*
 * Check for valid bus voltage levels <br>
 *
 * This macro is used in the LLD for assertion checks (XMC_ASSERT).
 */
#define XMC_SDMMC_CHECK_BUS_VOLTAGE(v)\
  (v == XMC_SDMMC_BUS_VOLTAGE_3_3_VOLTS)

/*
 * Check for valid data timeout counter values <br>
 *
 * This macro is used in the LLD for assertion checks (XMC_ASSERT).
 */
#define XMC_SDMMC_CHECK_DAT_TIMEOUT_COUNTER(c)\
  ((c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_13) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_14) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_15) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_16) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_17) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_18) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_19) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_20) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_21) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_22) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_23) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_24) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_25) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_26) ||\
   (c == XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_27))

/*
 * Valid number of data lines <br>
 *
 * This macro is used in the LLD for assertion checks (XMC_ASSERT).
 */
#define XMC_SDMMC_CHECK_DATA_LINES(l)\
  ((l == XMC_SDMMC_DATA_LINES_1)        ||\
   (l == XMC_SDMMC_DATA_LINES_4)        ||\
   (l == XMC_SDMMC_DATA_LINES_8))

/*
 * Check data transfer dir: Host to card and vice-versa <br>
 *
 * This macro is used in the LLD for assertion checks (XMC_ASSERT).
 */
#define XMC_SDMMC_CHECK_DATA_TRANSFER_DIR(d)\
  ((d == XMC_SDMMC_DATA_TRANSFER_HOST_TO_CARD)   ||\
   (d == XMC_SDMMC_DATA_TRANSFER_CARD_TO_HOST))

/*
 * Min and max number of delay elements <br>
 *
 * This macro is used in the LLD for assertion checks (XMC_ASSERT).
 */
#define XMC_SDMMC_MIN_DELAY_ELEMENTS (0U)
#define XMC_SDMMC_MAX_DELAY_ELEMENTS (15U)

/*******************************************************************************
 * ENUMS
 *******************************************************************************/

/**
 * Number of data lines for SDMMC data transfer
 */
typedef enum
{
  XMC_SDMMC_DATA_LINES_1 = 0x00U, /**< Single data line mode */
  XMC_SDMMC_DATA_LINES_4 = 0x02U, /**< 4-bit mode */
  XMC_SDMMC_DATA_LINES_8 = 0x20U  /**< SD 8-bit mode */
} XMC_SDMMC_DATA_LINES_t;

/**
 * Valid SD clock frequency divider selection
 */
typedef enum
{
  XMC_SDMMC_CLK_DIV_1   = 0x00U, /**< Base clock (10 Mhz -> 63 Mhz) */
  XMC_SDMMC_CLK_DIV_2   = 0x01U, /**< Base clock divided by 2 */
  XMC_SDMMC_CLK_DIV_4   = 0x02U, /**< Base clock divided by 4 */
  XMC_SDMMC_CLK_DIV_8   = 0x04U, /**< Base clock divided by 8 */
  XMC_SDMMC_CLK_DIV_16  = 0x08U, /**< Base clock divided by 16 */
  XMC_SDMMC_CLK_DIV_32  = 0x10U, /**< Base clock divided by 32 */
  XMC_SDMMC_CLK_DIV_64  = 0x20U, /**< Base clock divided by 64 */
  XMC_SDMMC_CLK_DIV_128 = 0x40U, /**< Base clock divided by 128 */
  XMC_SDMMC_CLK_DIV_256 = 0x80U  /**< Base clock divided by 256 */
} XMC_SDMMC_SDCLK_FREQ_SEL_t;

/**
 * Status return values for the SDMMC low level driver
 */
typedef enum
{
  XMC_SDMMC_STATUS_SUCCESS       = 0U, /**< Operation successful */
  XMC_SDMMC_STATUS_CMD_LINE_BUSY,      /**< Command line busy */
  XMC_SDMMC_STATUS_DAT_LINE_BUSY       /**< Data line busy */
} XMC_SDMMC_STATUS_t;

/**
 * SDMMC events (Normal and error events)
 */
typedef enum
{
  XMC_SDMMC_CMD_COMPLETE       = 0x01U, /**< Command complete event */
  XMC_SDMMC_TX_COMPLETE        = 0x02U, /**< Transmit complete event */
  XMC_SDMMC_BLOCK_GAP_EVENT    = 0x04U, /**< Block gap event */
  XMC_SDMMC_BUFFER_WRITE_READY = 0x10U, /**< Buffer write ready event */
  XMC_SDMMC_BUFFER_READ_READY  = 0x20U, /**< Buffer read ready event */
  XMC_SDMMC_CARD_INS           = 0x40U, /**< Card insert event */
  XMC_SDMMC_CARD_REMOVAL       = 0x80U, /**< Card removal event */
  XMC_SDMMC_CARD_INT           = 0x100U, /**< Card INT event */
  XMC_SDMMC_CARD_ERR           = 0x8000U, /**< Card error interrupt */
  XMC_SDMMC_CMD_TIMEOUT_ERR    = ((uint32_t)0x01 << 16U),   /**< Command time-out error */
  XMC_SDMMC_CMD_CRC_ERR        = ((uint32_t)0x02U << 16U),  /**< Command CRC error */
  XMC_SDMMC_CMD_END_BIT_ERR    = ((uint32_t)0x04U << 16U),  /**< Command end bit error */
  XMC_SDMMC_CMD_IND_ERR        = ((uint32_t)0x08U << 16U),  /**< Command index error */
  XMC_SDMMC_DATA_TIMEOUT_ERR   = ((uint32_t)0x10U << 16U),  /**< Data time-out error */
  XMC_SDMMC_DATA_CRC_ERR       = ((uint32_t)0x20U << 16U),  /**< Data CRC error */
  XMC_SDMMC_DATA_END_BIT_ERR   = ((uint32_t)0x40U << 16U),  /**< Data end bit error */
  XMC_SDMMC_CURRENT_LIMIT_ERR  = ((uint32_t)0x80U << 16U),  /**< Current limit error */
  XMC_SDMMC_ACMD_ERR           = ((uint32_t)0x100U << 16U), /**< ACMD error */
  XMC_SDMMC_TARGET_RESP_ERR    = ((uint32_t)0x1000U << 16U) /**< Target response error */
} XMC_SDMMC_EVENT_t;

/**
 * SDMMC wakeup events
 */
typedef enum
{
  XMC_SDMMC_WAKEUP_EN_CARD_INT = SDMMC_WAKEUP_CTRL_WAKEUP_EVENT_EN_INT_Msk, /**< Wakeup on card interrupt */
  XMC_SDMMC_WAKEUP_EN_CARD_INS = SDMMC_WAKEUP_CTRL_WAKEUP_EVENT_EN_INS_Msk, /**< Wakeup on SD card insertion */
  XMC_SDMMC_WAKEUP_EN_CARD_REM = SDMMC_WAKEUP_CTRL_WAKEUP_EVENT_EN_REM_Msk  /**< Wakeup SD card removal */
} XMC_SDMMC_WAKEUP_EVENT_t;

/**
 * SDMMC software reset modes
 */
typedef enum
{
  XMC_SDMMC_SW_RESET_ALL    = SDMMC_SW_RESET_SW_RST_ALL_Msk,      /**< Software reset all */
  XMC_SDMMC_SW_RST_CMD_LINE = SDMMC_SW_RESET_SW_RST_CMD_LINE_Msk, /**< Software reset command line */
  XMC_SDMMC_SW_RST_DAT_LINE = SDMMC_SW_RESET_SW_RST_DAT_LINE_Msk  /**< Software reset data line */
} XMC_SDMMC_SW_RESET_t;

/**
 * CMD12 response errors of Auto CMD12
 */
typedef enum
{
  XMC_SDMMC_ACMD12_NOT_EXEC_ERR = SDMMC_ACMD_ERR_STATUS_ACMD12_NOT_EXEC_ERR_Msk, /**< ACMD12 not executed error */
  XMC_SDMMC_ACMD_TIMEOUT_ERR    = SDMMC_ACMD_ERR_STATUS_ACMD_TIMEOUT_ERR_Msk,    /**< ACMD timeout error */
  XMC_SDMMC_ACMD_CRC_ERR        = SDMMC_ACMD_ERR_STATUS_ACMD_CRC_ERR_Msk,        /**< ACMD CRC error */
  XMC_SDMMC_ACMD_END_BIT_ERR    = SDMMC_ACMD_ERR_STATUS_ACMD_END_BIT_ERR_Msk,    /**< ACMD end bit error */
  XMC_SDMMC_ACMD_IND_ERR        = SDMMC_ACMD_ERR_STATUS_ACMD_IND_ERR_Msk,        /**< ACMD IND error */
  XMC_SDMMC_CMD_NOT_ISSUED_BY_ACMD12_ERR = SDMMC_ACMD_ERR_STATUS_CMD_NOT_ISSUED_BY_ACMD12_ERR_Msk /**< CMD not issued by ACMD12 */
} XMC_SDMMC_ACMD_ERR_t;

/**
 * SDMMC response types
 */
typedef enum
{
  XMC_SDMMC_RESPONSE_TYPE_NO_RESPONSE = 0U, /**< No response */
  XMC_SDMMC_RESPONSE_TYPE_R1,               /**< Response type: R1 */
  XMC_SDMMC_RESPONSE_TYPE_R1b,              /**< Response type: R1b */
  XMC_SDMMC_RESPONSE_TYPE_R2,               /**< Response type: R2 */
  XMC_SDMMC_RESPONSE_TYPE_R3,               /**< Response type: R3 */
  XMC_SDMMC_RESPONSE_TYPE_R6,               /**< Response type: R6 */
  XMC_SDMMC_RESPONSE_TYPE_R7                /**< Response type: R7 */
} XMC_SDMMC_RESPONSE_TYPE_t;

/**
* Command response selection
*/
typedef enum XMC_SDMMC_COMMAND_RESPONSE
{
  XMC_SDMMC_COMMAND_RESPONSE_NONE = 0, /**< No Response */
  XMC_SDMMC_COMMAND_RESPONSE_LONG = 1, /**< Response length 136 */
  XMC_SDMMC_COMMAND_RESPONSE_SHORT = 2, /**< Response length 48 */
  XMC_SDMMC_COMMAND_RESPONSE_SHORT_BUSY = 3, /**< Response length 48 check Busy after response */
} XMC_SDMMC_COMMAND_RESPONSE_t;

/**
 * Types of SDMMC commands
 */
typedef enum
{
  XMC_SDMMC_COMMAND_TYPE_NORMAL  = 0U, /**< Command normal */
  XMC_SDMMC_COMMAND_TYPE_SUSPEND,      /**< Command suspend */
  XMC_SDMMC_COMMAND_TYPE_RESUME,       /**< Command resume */
  XMC_SDMMC_COMMAND_TYPE_ABORT         /**< Command abort */
} XMC_SDMMC_COMMAND_TYPE_t;

/**
 * SDMMC transfer modes
 */
typedef enum
{
  XMC_SDMMC_TRANSFER_MODE_TYPE_SINGLE        = 0x00U, /**< Transfer mode type: single */
  XMC_SDMMC_TRANSFER_MODE_TYPE_INFINITE      = 0x20U, /**< Transfer mode type: infinite */
  XMC_SDMMC_TRANSFER_MODE_TYPE_MULTIPLE      = 0x22U, /**< Transfer mode type: multiple */
  XMC_SDMMC_TRANSFER_MODE_TYPE_STOP_MULTIPLE = 0x22U  /**< Transfer mode type: multiple stop */
} XMC_SDMMC_TRANSFER_MODE_TYPE_t;

/**
 * Auto command transfer modes
 */
typedef enum
{
  XMC_SDMMC_TRANSFER_MODE_AUTO_CMD_DISABLED = 0x00U, /**< ACMD mode disabled */
  XMC_SDMMC_TRANSFER_MODE_AUTO_CMD_12                /**< ACMD12 mode */
} XMC_SDMMC_TRANSFER_MODE_AUTO_CMD_t;

/**
 * SDMMC bus voltage level
 */
typedef enum
{
  XMC_SDMMC_BUS_VOLTAGE_3_3_VOLTS = 0x07U
} XMC_SDMMC_BUS_VOLTAGE_t;

/**
 * Data line timeout counter values
 */
typedef enum
{
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_13 = 0U, /** SDCLK * (2 ^ 13) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_14 = 1U, /** SDCLK * (2 ^ 14) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_15 = 2U, /** SDCLK * (2 ^ 15) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_16 = 3U, /** SDCLK * (2 ^ 16) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_17 = 4U, /** SDCLK * (2 ^ 17) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_18 = 5U, /** SDCLK * (2 ^ 18) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_19 = 6U, /** SDCLK * (2 ^ 19) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_20 = 7U, /** SDCLK * (2 ^ 20) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_21 = 8U, /** SDCLK * (2 ^ 21) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_22 = 9U, /** SDCLK * (2 ^ 22) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_23 = 10U, /** SDCLK * (2 ^ 23) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_24 = 11U, /** SDCLK * (2 ^ 24) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_25 = 12U, /** SDCLK * (2 ^ 25) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_26 = 13U, /** SDCLK * (2 ^ 26) */
  XMC_SDMMC_DAT_TIMEOUT_COUNTER_2_POW_27 = 14U, /** SDCLK * (2 ^ 27) */
} XMC_SDMMC_DAT_TIMEOUT_COUNTER_t;

/**
 * SDMMC data transfer direction
 */
typedef enum
{
  XMC_SDMMC_DATA_TRANSFER_HOST_TO_CARD = 0U, /** Host to card */
  XMC_SDMMC_DATA_TRANSFER_CARD_TO_HOST       /** Card to host */
} XMC_SDMMC_DATA_TRANSFER_DIR_t;

/**
 * SDMMC card detection signal source
 */
typedef enum XMC_SDMMC_CD_SOURCE
{
  XMC_SDMMC_CD_SOURCE_PIN = 0,
  XMC_SDMMC_CD_SOURCE_SW = 1 << SDMMC_HOST_CTRL_CARD_DET_SIGNAL_DETECT_Pos
} XMC_SDMMC_CD_SOURCE_t;

/**
 * SDMMC card detection status
 */
typedef enum XMC_SDMMC_CD_STATUS
{
  XMC_SDMMC_CD_STATUS_NO_CARD = 0,
  XMC_SDMMC_CD_STATUS_INSERTED = 1 << SDMMC_HOST_CTRL_CARD_DETECT_TEST_LEVEL_Pos
} XMC_SDMMC_CD_STATUS_t;

/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/

/**
 * SDMMC device structure <br>
 *
 * The structure represents a collection of all hardware registers used
 * to configure the SDMMC peripheral on the XMC4500 microcontroller. The
 * registers can be accessed with ::XMC_SDMMC.
 */
typedef struct
{
  __I  uint32_t  RESERVED0;
  __IO uint16_t  BLOCK_SIZE;
  __IO uint16_t  BLOCK_COUNT;
  __IO uint32_t  ARGUMENT1;
  __IO uint16_t  TRANSFER_MODE;
  __IO uint16_t  COMMAND;
  __I  uint32_t  RESPONSE[4];
  __IO uint32_t  DATA_BUFFER;
  __I  uint32_t  PRESENT_STATE;
  __IO uint8_t   HOST_CTRL;
  __IO uint8_t   POWER_CTRL;
  __IO uint8_t   BLOCK_GAP_CTRL;
  __IO uint8_t   WAKEUP_CTRL;
  __IO uint16_t  CLOCK_CTRL;
  __IO uint8_t   TIMEOUT_CTRL;
  __IO uint8_t   SW_RESET;
  __IO uint16_t  INT_STATUS_NORM;
  __IO uint16_t  INT_STATUS_ERR;
  __IO uint16_t  EN_INT_STATUS_NORM;
  __IO uint16_t  EN_INT_STATUS_ERR;
  __IO uint16_t  EN_INT_SIGNAL_NORM;
  __IO uint16_t  EN_INT_SIGNAL_ERR;
  __I  uint16_t  ACMD_ERR_STATUS;
  __I  uint16_t  RESERVED1[9];
  __O  uint16_t  FORCE_EVENT_ACMD_ERR_STATUS;
  __O  uint16_t  FORCE_EVENT_ERR_STATUS;
  __I  uint32_t  RESERVED2[8];
  __O  uint32_t  DEBUG_SEL;
  __I  uint32_t  RESERVED3[30];
  __IO uint32_t  SPI;
  __I  uint32_t  RESERVED4[2];
  __I  uint16_t  SLOT_INT_STATUS;
} XMC_SDMMC_t;

/* Anonymous structure/union guard start */
#if defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__TASKING__)
  #pragma warning 586
#endif

/**
 * Present state of the SDMMC host controller <br>
 *
 * The structure presents a convenient way to obtain the SDMMC peripheral's
 * present state information (for example, the write protect pin level). The
 * XMC_SDMMC_GetPresentState() API can be used to populate the structure
 * with the state of the SD host controller.
 */
typedef union
{
  struct
  {
    uint32_t command_inihibit_cmd 	 : 1; /**< Command: Inhibit command */
	uint32_t command_inihibit_dat 	 : 1; /**< Command: Inhibit data */
	uint32_t dat_line_active 		 : 1; /**< Data line active */
	uint32_t 						 : 5;
	uint32_t write_transfer_active 	 : 1; /**< Write transfer active */
	uint32_t read_transfer_active 	 : 1; /**< Read transfer active */
	uint32_t buffer_write_enable 	 : 1; /**< Buffer write enable */
	uint32_t buffer_read_enable 	 : 1; /**< Buffer read enable */
	uint32_t 						 : 4;
	uint32_t card_inserted 			 : 1; /**< Card inserted */
	uint32_t card_state_stable 		 : 1; /**< Card state stable */
	uint32_t card_detect_pin_level 	 : 1; /**< Card detect pin level */
	uint32_t write_protect_pin_level : 1; /**< Write protect pin level */
	uint32_t dat_3_0_pin_level 		 : 4; /**< Data 3_0 pin level */
	uint32_t cmd_line_level          : 1; /**< Command line level */
	uint32_t dat7_4_pin_level        : 4; /**< Data 7_4 pin level */
	uint32_t                         : 3;
  };
  uint32_t b32;
} XMC_SDMMC_PRESENT_STATE_t;

/**
 * SDMMC transfer mode configuration
 */
typedef struct
{
  uint32_t block_size;
  uint32_t num_blocks;
  XMC_SDMMC_TRANSFER_MODE_TYPE_t type;
  XMC_SDMMC_TRANSFER_MODE_AUTO_CMD_t auto_cmd;
  XMC_SDMMC_DATA_TRANSFER_DIR_t direction;
} XMC_SDMMC_TRANSFER_MODE_t;

/**
 * Represent an SDMMC command <br>
 *
 * The structure holds the configuration for an SDMMC command. The SDMMC
 * COMMAND register is a 16-bit register which is responsible for enabling
 * configuration parameters like command type, response type, index check
 * enable (and a few more). Once SDMMC.COMMAND is configured, the
 * XMC_SDMMC_SendCommand() function can be used to send the command.
 */
typedef union
{
  struct
  {
    uint16_t response_type_sel : 2; /**< Response type select ::XMC_SDMMC_COMMAND_RESPONSE_t */
    uint16_t                   : 1;
    uint16_t crc_check_en      : 1; /**< Command CRC check enable */
    uint16_t index_check_en    : 1; /**< Command index check enable */
    uint16_t dat_present_sel   : 1; /**< Data present select */
    uint16_t cmd_type          : 2; /**< Command type ::XMC_SDMMC_COMMAND_TYPE_t */
    uint16_t cmd_index         : 6; /**< Command index */
    uint16_t                   : 2;
  };
  uint16_t cmd;
} XMC_SDMMC_COMMAND_t;

/* Anonymous structure/union guard end */
#if defined (__CC_ARM)
  #pragma pop
#elif defined (__TASKING__)
  #pragma warning restore
#endif

/**
 * Card response structure
 */
typedef struct
{
  uint32_t response_0;
  uint32_t response_2;
  uint32_t response_4;
  uint32_t response_6;
} XMC_SDMMC_RESPONSE_t;

/**
 * SDMMC configuration data structure <br>
 *
 * The structure is used to configure the bus width and the clock divider.
 */
typedef struct
{
  uint8_t bus_width; /**< SDMMC bus width */
  XMC_SDMMC_SDCLK_FREQ_SEL_t clock_divider; /**< SDMMC clock divider */
} XMC_SDMMC_CONFIG_t;

/*******************************************************************************
 * API PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Get power status of the SDMMC peripheral <br>
 *
 * \par
 * The function checks the SD_BUS_POWER bit-field of the POWER_CTRL register and returns
 * a boolean value - "on" or "off".
 */
bool XMC_SDMMC_GetPowerStatus(XMC_SDMMC_t *const sdmmc);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable SDMMC peripheral <br>
 *
 * \par
 * The function de-asserts the peripheral reset. The peripheral needs to be initialized.
 */
void XMC_SDMMC_Enable(XMC_SDMMC_t *const sdmmc);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable SDMMC peripheral <br>
 *
 * \par
 * The function asserts the peripheral reset.
 */
void XMC_SDMMC_Disable(XMC_SDMMC_t *const sdmmc);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param config A pointer to a constant XMC_SDMMC_CONFIG_t structure containing the
 *               bus width and clock divider configuration
 * @return ::XMC_SDMMC_STATUS_SUCCESS
 *
 * \par<b>Description: </b><br>
 * Initialize the SDMMC peripheral <br>
 *
 * \par
 * The function enables the SDMMC peripheral, sets the internal clock divider register
 * and sets the bus width.
 */
XMC_SDMMC_STATUS_t XMC_SDMMC_Init(XMC_SDMMC_t *const sdmmc, const XMC_SDMMC_CONFIG_t *config);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param event A valid SDMMC event (XMC_SDMMC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable SDMMC normal and error event(s) <br>
 *
 * \par
 * The function first sets the bit-fields of EN_INT_STATUS_NORM and EN_INT_STATUS_ERR
 * registers to enable interrupt status for requested normal/error SDMMC events. It then
 * sets the bit-fields of EN_INT_SIGNAL_NORM and EN_INT_SIGNAL_ERR to enable the
 * interrupt generation for the requested events.
 */
void XMC_SDMMC_EnableEvent(XMC_SDMMC_t *const sdmmc, uint32_t event);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param event A valid SDMMC event (XMC_SDMMC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable normal and error SDMMC event(s) <br>
 *
 * \par
 * The function disables the interrupt generation for the requested events by clearing
 * the bit-fields of EN_INT_SIGNAL_NORM and EN_INT_SIGNAL_ERR registers.
 *
 * \par<b>Note:</b><br>
 * The XMC_SDMMC_DisableEvent() function doesn't reset the the interrupt status. One
 * may still use XMC_SDMMC_GetEvent() to check the status of requested events even if
 * the interrupt generation is already disabled.
 */
void XMC_SDMMC_DisableEvent(XMC_SDMMC_t *const sdmmc, uint32_t event);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param event A valid SDMMC event (XMC_SDMMC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clear SDMMC event(s) <br>
 *
 * \par
 * The function clears requested normal/error events by settings the bit-fields of
 * the INT_STATUS register. Please check SDMMC_INT_STATUS_NORM in the XMC45000
 * manual for more details.
 */
void XMC_SDMMC_ClearEvent(XMC_SDMMC_t *const sdmmc, uint32_t event);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param event A valid SDMMC event (XMC_SDMMC_EVENT_t)
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Get SDMMC event status <br>
 *
 * \par
 * The function returns the status of a single requested (normal/error) event by
 * reading the appropriate bit-fields of the INT_STATUS register.
 */
bool XMC_SDMMC_GetEvent(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_EVENT_t event);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param event A valid SDMMC event (XMC_SDMMC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable event status <br>
 *
 * \par
 * The function sets the bit-fields of EN_INT_STATUS_NORM and EN_INT_STATUS_ERR
 * registers to enable interrupt status for requested normal/error SDMMC events.
 */
void XMC_SDMMC_EnableEventStatus(XMC_SDMMC_t *const sdmmc, uint32_t event);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param event A valid SDMMC event (XMC_SDMMC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable event status <br>
 *
 * \par
 * The function resets the bit-fields of EN_INT_STATUS_NORM and EN_INT_STATUS_ERR
 * registers to disable interrupt status for requested normal/error SDMMC events.
 */
void XMC_SDMMC_DisableEventStatus(XMC_SDMMC_t *const sdmmc, uint32_t event);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param event A valid SDMMC event (::XMC_SDMMC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Trigger SDMMC error events <br>
 *
 * \par
 * The SDMMC peripheral supports triggering of following error events: <br>
 *
 * ::XMC_SDMMC_CMD_TIMEOUT_ERR, ::XMC_SDMMC_CMD_CRC_ERR, ::XMC_SDMMC_CMD_END_BIT_ERR,
 * ::XMC_SDMMC_CMD_IND_ERR, ::XMC_SDMMC_DATA_TIMEOUT_ERR, ::XMC_SDMMC_DATA_CRC_ERR,
 * ::XMC_SDMMC_DATA_END_BIT_ERR, ::XMC_SDMMC_CURRENT_LIMIT_ERR, ::XMC_SDMMC_ACMD_ERR,
 * ::XMC_SDMMC_TARGET_RESP_ERR
 *
 * For triggering Auto CMD12 error, see XMC_SDMMC_TriggerACMDErr()
 */
__STATIC_INLINE void XMC_SDMMC_TriggerEvent(XMC_SDMMC_t *const sdmmc, uint32_t event)
{
  XMC_ASSERT("XMC_SDMMC_TriggerEvent: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  
  sdmmc->FORCE_EVENT_ERR_STATUS |= (uint16_t)(event >> 16U);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param source A valid SDMMC card detection signal source (::XMC_SDMMC_CD_SOURCE_t)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Selects source for card detection
 */
__STATIC_INLINE void XMC_SDMMC_SetCardDetectionSource(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_CD_SOURCE_t source)
{
  XMC_ASSERT("XMC_SDMMC_TriggerEvent: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->HOST_CTRL |= (sdmmc->HOST_CTRL & (uint32_t)~SDMMC_HOST_CTRL_CARD_DET_SIGNAL_DETECT_Msk) | source;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param status A valid SDMMC card detection status (::XMC_SDMMC_CD_STATUS_t)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the card detection status indicating whether card is inserted or not.
 * Generates (card ins or card removal) interrupt when the normal interrupt is enabled.
 * @note Only valid if SDMMC card detection signal source is set to XMC_SDMMC_CD_SOURCE_SW <br>
 *
 */
__STATIC_INLINE void XMC_SDMMC_SetCardDetectionStatus(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_CD_STATUS_t status)
{
  XMC_ASSERT("XMC_SDMMC_TriggerEvent: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->HOST_CTRL |= (sdmmc->HOST_CTRL & (uint32_t)~SDMMC_HOST_CTRL_CARD_DETECT_TEST_LEVEL_Msk) | status;
}


/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Check if any error event has occured <br>
 *
 * \par
 * The function can typically be used for writing an error interrupt recovery routine.
 * Should any error be indicated (If XMC_SDMMC_IsAnyErrorEvent() returns true), the
 * routine may then clear the event after indicating the error event and reset the
 * SDMMC command and data lines.
 */
__STATIC_INLINE bool XMC_SDMMC_IsAnyErrorEvent(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_IsAnyErrorEvent: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  return (bool)(sdmmc->INT_STATUS_ERR);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param event A valid SDMMC wakeup event (::XMC_SDMMC_WAKEUP_EVENT_t) or a valid combination
 *              of logically OR'd wakeup events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable wakeup event(s) <br>
 *
 * \par
 * The function enables SDMMC wakeup events by setting appropriate bit-fields of the WAKEUP_CTRL
 * register. <br>
 *
 * List of supported wakeup events -> Wakeup on: <br>
 * 1) Card interrupt <br>
 * 2) SD card insertion <br>
 * 3) SD card removal <br>
 */
__STATIC_INLINE void XMC_SDMMC_EnableWakeupEvent(XMC_SDMMC_t *const sdmmc, uint32_t event)
{
  XMC_ASSERT("XMC_SDMMC_EnableWakeupEvent: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  
  sdmmc->WAKEUP_CTRL |= (uint8_t)event;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param event A valid SDMMC wakeup event (::XMC_SDMMC_WAKEUP_EVENT_t) or a valid combination
 *              of logically OR'd wakeup events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable wakeup event(s) <br>
 *
 * \par
 * The function disables SDMMC wakeup events by clearing appropriate bit-fields of the WAKEUP_CTRL
 * register. <br>
 *
 * List of supported wakeup events -> Wakeup on: <br>
 * 1) Card interrupt <br>
 * 2) SD card insertion <br>
 * 3) SD card removal <br>
 */
__STATIC_INLINE void XMC_SDMMC_DisableWakeupEvent(XMC_SDMMC_t *const sdmmc, uint32_t event)
{
  XMC_ASSERT("XMC_SDMMC_DisableWakeupEvent: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  
  sdmmc->WAKEUP_CTRL &= (uint8_t)~event;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param error A valid SDMMC ACMD error (::XMC_SDMMC_ACMD_ERR_t)
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Get status of Auto CMD12 errors <br>
 *
 * \par
 * The function detects the presence of an Auto CMD12 error. A boolean is returned to
 * indicate if an error is detected.
 */
__STATIC_INLINE bool XMC_SDMMC_GetACMDErrStatus(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_ACMD_ERR_t error)
{
  XMC_ASSERT("XMC_SDMMC_GetACMDErrStatus: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  XMC_ASSERT("XMC_SDMMC_GetACMDErrStatus: Invalid ACMD response error", XMC_SDMMC_CHECK_ACMD_ERR(error));
  
  return (bool)(sdmmc->ACMD_ERR_STATUS & (uint16_t)error);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param error A valid SDMMC ACMD error (::XMC_SDMMC_ACMD_ERR_t) or a valid combination
 *              of logically OR'd ACMD error events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Triggers Auto CMD12 error(s) <br>
 *
 * \par
 * This function triggers Auto CMD12 error(s) by setting appropriate bit-fields of the
 * FORCE_EVENT_ACMD_ERR_STATUS register.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_SDMMC_TriggerEvent()
 */
__STATIC_INLINE void XMC_SDMMC_TriggerACMDErr(XMC_SDMMC_t *const sdmmc, uint32_t error)
{
  XMC_ASSERT("XMC_SDMMC_TriggerACMDErr: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  
  sdmmc->FORCE_EVENT_ACMD_ERR_STATUS |= (uint16_t)error;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return uint32_t The value held in the SDMMC FIFO
 *
 * \par<b>Description: </b><br>
 * Use this function to read a single word (32 bits) from the SDMMC FIFO. <br>
 */
__STATIC_INLINE uint32_t XMC_SDMMC_ReadFIFO(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_ReadFIFO: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  return (sdmmc->DATA_BUFFER);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param data Pointer to a data word (32 bits) that needs to be written to the FIFO
 * @return None
 *
 * \par<b>Description: </b><br>
 * Use this function to write a single word (32 bits) to the SDMMC FIFO. <br>
 */
__STATIC_INLINE void XMC_SDMMC_WriteFIFO(XMC_SDMMC_t *const sdmmc, uint32_t *data)
{
  XMC_ASSERT("XMC_SDMMC_WriteFIFO: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->DATA_BUFFER = *data;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable SDMMC bus power <br>
 *
 * \par
 * The function sets the SD_BUS_POWER bit-field in the POWER_CTRL register, enabling the
 * bus power. It may be invoked after enabling the SD clock (XMC_SDMMC_SDClockEnable()).
 */
__STATIC_INLINE void XMC_SDMMC_BusPowerOn(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_BusPowerOn: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->POWER_CTRL |= (uint8_t)(SDMMC_POWER_CTRL_SD_BUS_POWER_Msk);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable SDMMC bus power <br>
 *
 * \par
 * The function resets the SD_BUS_POWER bit-field in the POWER_CTRL register, disabling the
 * bus power.
 */
__STATIC_INLINE void XMC_SDMMC_BusPowerOff(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_BusPowerOff: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->POWER_CTRL &= (uint8_t)~SDMMC_POWER_CTRL_SD_BUS_POWER_Msk;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable the internal SDMMC clock <br>
 *
 * \par
 * The function enables the internal clock of the SDMMC peripheral. To check if the
 * clock is stable, use XMC_SDMMC_GetClockStability().
 *
 * \par<b>Note: </b><br>
 * Invoke XMC_SDMMC_Init() before using this function.
 */
__STATIC_INLINE void XMC_SDMMC_Start(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_Start: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  
  /* Enable internal clock */
  sdmmc->CLOCK_CTRL |= (uint16_t)SDMMC_CLOCK_CTRL_INTERNAL_CLOCK_EN_Msk;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Check internal clock stability <br>
 *
 * \par
 * Use this function to check the internal SDMMC clock stability. The function returns a
 * boolean value indicating internal clock stability (true = stable)
 */
__STATIC_INLINE bool XMC_SDMMC_GetClockStability(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_GetClockStability: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  
  /* Return clock stability */
  return (bool)(sdmmc->CLOCK_CTRL & SDMMC_CLOCK_CTRL_INTERNAL_CLOCK_STABLE_Msk);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable internal SDMMC clock <br>
 *
 * \par
 * The function disables the internal clock of the SDMMC peripheral. The SDMMC registers
 * can still be read and written even if the internal clock is disabled.
 */
__STATIC_INLINE void XMC_SDMMC_Stop(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_Stop: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->CLOCK_CTRL &= (uint16_t)~SDMMC_CLOCK_CTRL_INTERNAL_CLOCK_EN_Msk;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable the SD clock <br>
 *
 * \par
 * The function sets the SDCLOCK_EN bit-field of the CLOCK_CTRL register, enabling the
 * SD clock. It can be invoked after the internal clock has achieved stability. SD card
 * initialization process may then follow.
 */
__STATIC_INLINE void XMC_SDMMC_SDClockEnable(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_SDClockEnable: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->CLOCK_CTRL |= (uint16_t)SDMMC_CLOCK_CTRL_SDCLOCK_EN_Msk;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable the SD clock <br>
 *
 * \par
 * The function resets the SDCLOCK_EN bit-field of the CLOCK_CTRL register, disabling the
 * SD clock. It can be used alongside a SD card information reset routine (if required).
 */
__STATIC_INLINE void XMC_SDMMC_SDClockDisable(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_SDClockDisable: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->CLOCK_CTRL &= (uint16_t)~SDMMC_CLOCK_CTRL_SDCLOCK_EN_Msk;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param reset_mode Reset mode or a bitwise combination of modes
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set SDMMC software reset request <br>
 *
 * \par
 * The function sets in the SDMMC SW_RESET register: <br>
 * 1) bit 0 to reset all <br>
 * 2) bit 1 to reset CMD line <br>
 * 3) bit 2 reset DAT line <br>
 *
 * It is typically used to reset the SD HOST controller's registers.
 */
__STATIC_INLINE void XMC_SDMMC_SetSWReset(XMC_SDMMC_t *const sdmmc, uint32_t reset_mode)
{
  XMC_ASSERT("XMC_SDMMC_SetSWReset: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->SW_RESET |= (uint8_t)reset_mode;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return Software reset status
 *
 * \par<b>Description: </b><br>
 * Get SDMMC software reset status <br>
 *
 * \par
 * The SD host takes some time to reset its registers after invoking XMC_SDMMC_SetSWReset().
 * Since XMC_SDMMC_SetSWReset() is a non-blocking function, XMC_SDMMC_GetSWResetStatus() has
 * been provided to check the software reset status. The return value needs to be masked
 * with the reset mode (XMC_SDMMC_SW_RESET_t) to get a specific software reset status value.
 */
__STATIC_INLINE uint32_t XMC_SDMMC_GetSWResetStatus(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_GetSWResetStatus: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  
  return (uint32_t)(sdmmc->SW_RESET);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return XMC_SDMMC_PRESENT_STATE_t A structure storing the present state of the host controller
 *
 * \par<b>Description: </b><br>
 * Get the present state of the SDMMC host controller <br>
 *
 * \par
 * Get the values of each bit-field in SDMMC_PRESENT_STATE register
 * The function call populates an instance of the XMC_SDMMC_PRESENT_STATE_t structure with
 * the state of the SD host controller and returns it to the caller.
 */
__STATIC_INLINE XMC_SDMMC_PRESENT_STATE_t XMC_SDMMC_GetPresentState(const XMC_SDMMC_t *const sdmmc)
{
  XMC_SDMMC_PRESENT_STATE_t result;

  XMC_ASSERT("XMC_SDMMC_GetPresentState: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  result.b32 = (uint32_t)sdmmc->PRESENT_STATE;

  return result;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return bool SDMMC command line status
 *
 * \par<b>Description: </b><br>
 * Check if the command line is busy <br>
 *
 * \par
 * The function reads the SDMMC PRESENT_STATE register and returns "true" if the command
 * line is busy ("false" otherwise). The command line must be free before sending an SDMMC
 * command with XMC_SDMMC_SendCommand().
 */
__STATIC_INLINE bool XMC_SDMMC_IsCommandLineBusy(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_IsCommandLineBusy: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  return (bool)(sdmmc->PRESENT_STATE & SDMMC_PRESENT_STATE_COMMAND_INHIBIT_CMD_Msk);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return bool SDMMC data line status
 *
 * \par<b>Description: </b><br>
 * Check if the data line is busy <br>
 *
 * \par
 * The function reads the SDMMC PRESENT_STATE register and returns "true" if the data
 * line is busy ("false" otherwise). The data line must be free before sending an SDMMC
 * command with XMC_SDMMC_SendCommand().
 */
__STATIC_INLINE bool XMC_SDMMC_IsDataLineBusy(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_IsDataLineBusy: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  return (bool)(sdmmc->PRESENT_STATE & SDMMC_PRESENT_STATE_COMMAND_INHIBIT_DAT_Msk);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return bool Status of all data lines
 *
 * \par<b>Description: </b><br>
 * Check if all data line are high <br>
 *
 * \par
 * The function reads the SDMMC PRESENT_STATE register and returns "true" if all data
 * lines are high. It can be used to handle SDMMC error conditions. For example, if an
 * error event (XMC_SDMMC_IsAnyErrorEvent()) is detected and all data lines are high,
 * the user code can conclude that the error is of a "recoverable" type.
 */
__STATIC_INLINE bool XMC_SDMMC_IsAllDataLinesHigh(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_IsAllDataLinesHigh: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  return ((((sdmmc->PRESENT_STATE & SDMMC_PRESENT_STATE_DAT_3_0_PIN_LEVEL_Msk) >>
		    SDMMC_PRESENT_STATE_DAT_3_0_PIN_LEVEL_Pos) == 0x0FU) ? true : false);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param command A pointer to a constant of type XMC_SDMMC_COMMAND_t, pointing to the command configuration
 * @param arg Command argument
 * @return ::XMC_SDMMC_STATUS_SUCCESS
 *
 * \par<b>Description: </b><br>
 * Send normal SDMMC command <br>
 *
 * \par
 * Use this function to send a normal SDMMC command. This non-blocking function sets the
 * ARGUMENT1 and COMMAND registers. It is the user's responsibility to check if the command
 * and data lines are busy (XMC_SDMMC_IsDataLineBusy(), XMC_SDMMC_IsCommandLineBusy()).
 */
XMC_SDMMC_STATUS_t XMC_SDMMC_SendCommand(XMC_SDMMC_t *const sdmmc, const XMC_SDMMC_COMMAND_t *command, uint32_t arg);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return uint32_t SDMMC command response
 *
 * \par<b>Description: </b><br>
 * Get card response (no Auto command) <br>
 *
 * \par
 * This function returns [39:8] bits of the card response. The others are checked automatically
 * by the peripheral. This function can be used with response type R1, R1b, R3, R4, R5, R5b, R6
 * but it doesn't support the retrieving of R1 of Auto CMD 23 and R1b of Auto CMD 12. To get
 * these responses, use XMC_SDMMC_GetAutoCommandResponse().
 */
__STATIC_INLINE uint32_t XMC_SDMMC_GetCommandResponse(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_GetCommandResponse: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  
  return (sdmmc->RESPONSE[0]);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return uint32_t Auto command response value
 *
 * \par<b>Description: </b><br>
 * Get card response of Auto commands <br>
 *
 * \par
 * This function returns card response [39:8] bits of auto commands: R1 of Auto CMD 23 and
 * R1b of Auto CMD 12.
 */
__STATIC_INLINE uint32_t XMC_SDMMC_GetAutoCommandResponse(const XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_GetAutoCommandResponse: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  return (sdmmc->RESPONSE[3]);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param response Pointer to structure type XMC_SDMMC_RESPONSE_t to store the full response
 * @return None
 *
 * \par<b>Description: </b><br>
 * Get card R2 response <br>
 *
 * \par
 * The R2 response is 120 bits wide. The function reads all peripheral registers and store in
 * the response data structure.
 */
void XMC_SDMMC_GetR2Response(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_RESPONSE_t *const response);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param transfer_mode Transfer mode configuration
 * @return None
 *
 * \par<b>Description: </b><br>
 * Configure data transfer mode <br>
 *
 * \par
 * The function configures block size, block count, type of data transfer, response type
 * and sets the auto command configuration. Use this function to configure a multi-block
 * SDMMC transfer.
 */
void XMC_SDMMC_SetDataTransferMode(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_TRANSFER_MODE_t *const transfer_mode);

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return uint32_t Number of blocks that need to be transferred
 *
 * \par<b>Description: </b><br>
 * Get the number of blocks that need to be transferred <br>
 *
 * \par
 * This function is valid only for multiple block transfers. The host controller
 * decrements the block count after each block transfer and stops when the count reaches
 * zero. It can only be accessed when no transaction is happening (i.e after a transaction
 * has stopped). This function returns an invalid value during the transfer. <br>
 *
 * When saving transfer context as a result of the suspend command, the number of blocks
 * yet to be transferred can be determined by using this function.
 */
__STATIC_INLINE uint32_t XMC_SDMMC_GetTransferBlocksNum(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_GetTransferBlocksNum: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  return (uint32_t)(sdmmc->BLOCK_COUNT);
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param enabled "true" to enable read wait control, "false" to disable read wait control.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Configure read wait control <br>
 *
 * \par
 * The read wait function is optional for SDIO cards. If the card supports read wait and
 * XMC_SDMMC_GetTransferBlocksNum() is executed, the SDMMC peripheral will stop read data
 * using DAT[2] line. If this feature is not enabled the peripheral has to stop the SD
 * clock to hold read data, restricting commands generation. <br>
 *
 * When the host driver detects an SD card insertion, it sets this bit according to the
 * CCCR of the SDIO card. If the card does not support read wait, this feature shall
 * never be enabled otherwise a DAT line conflict may occur. If this feature is disabled,
 * Suspend/Resume cannot be supported.
 */
__STATIC_INLINE void XMC_SDMMC_SetReadWaitControl(XMC_SDMMC_t *const sdmmc, bool enabled)
{
  XMC_ASSERT("XMC_SDMMC_SetReadWaitControl: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->BLOCK_GAP_CTRL = (uint8_t)((sdmmc->BLOCK_GAP_CTRL & (uint8_t)~SDMMC_BLOCK_GAP_CTRL_READ_WAIT_CTRL_Msk) |
                                    (uint8_t)((uint8_t)enabled << SDMMC_BLOCK_GAP_CTRL_READ_WAIT_CTRL_Pos));
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param enabled "true" to set stop at block gap, "false" for transfer
 * @return None
 *
 * \par<b>Description: </b><br>
 * Stop at block gap request <br>
 *
 * \par
 * The function is used to terminate a transaction execution at the next block gap for
 * non-DMA transfers.
 */
__STATIC_INLINE void XMC_SDMMC_SetStopAtBlockGap(XMC_SDMMC_t *const sdmmc, bool enabled)
{
  XMC_ASSERT("XMC_SDMMC_SetStopAtBlockGap: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->BLOCK_GAP_CTRL = (uint8_t)((sdmmc->BLOCK_GAP_CTRL & (uint8_t)~SDMMC_BLOCK_GAP_CTRL_STOP_AT_BLOCK_GAP_Msk) |
                                    (uint8_t)((uint8_t)enabled << SDMMC_BLOCK_GAP_CTRL_STOP_AT_BLOCK_GAP_Pos));
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param enabled "true" to restart transaction, "false" is ignored
 * @return None
 *
 * \par<b>Description: </b><br>
 * Issue a continue request <br>
 *
 * \par
 * The function is used to restart a transaction which was stopped using the "Stop at
 * block gap" request. (XMC_SDMMC_SetStopAtBlockGap())
 */
__STATIC_INLINE void XMC_SDMMC_SetContinueRequest(XMC_SDMMC_t *const sdmmc, bool enabled)
{
  XMC_ASSERT("XMC_SDMMC_SetContinueRequest: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->BLOCK_GAP_CTRL = ((sdmmc->BLOCK_GAP_CTRL & (uint8_t)~SDMMC_BLOCK_GAP_CTRL_CONTINUE_REQ_Msk) |
                           (uint8_t)((uint8_t)enabled << SDMMC_BLOCK_GAP_CTRL_CONTINUE_REQ_Pos));
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Get continue request <br>
 *
 * \par
 * The function returns the status of the BLOCK_GAP_CTRL.CONTINUE_REQ bit-field. It
 * returns "true" if the transaction is restarted after a "stop at block gap" request.
 */
__STATIC_INLINE bool XMC_SDMMC_GetContinueRequest(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_GetContinueRequest: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  return (bool)(sdmmc->BLOCK_GAP_CTRL & (uint8_t)(1U << SDMMC_BLOCK_GAP_CTRL_CONTINUE_REQ_Pos));
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param config A pointer to the SDMMC configuration structure (::XMC_SDMMC_CONFIG_t)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable interrupt at block gap <br>
 *
 * \par
 * The function sets the BLOCK_GAP_CTRL.INT_AT_BLOCK_GAP bit-field to enable interrupt
 * at block gap for a multi-block transfer. This bit is only valid in a 4-bit mode of
 * the SDIO card.
 */
__STATIC_INLINE void XMC_SDMMC_EnableInterruptAtBlockGap(XMC_SDMMC_t *const sdmmc, const XMC_SDMMC_CONFIG_t *config)
{
  XMC_ASSERT("XMC_SDMMC_EnableInterruptAtBlockGap: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  XMC_ASSERT("XMC_SDMMC_EnableInterruptAtBlockGap: This operation is only valid in 4-bit mode",
             (config->bus_width == XMC_SDMMC_DATA_LINES_1));

  sdmmc->BLOCK_GAP_CTRL |= (uint8_t)SDMMC_BLOCK_GAP_CTRL_INT_AT_BLOCK_GAP_Msk;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param config A pointer to the SDMMC configuration structure (::XMC_SDMMC_CONFIG_t)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable interrupt at block gap <br>
 *
 * \par
 * The function resets the BLOCK_GAP_CTRL.INT_AT_BLOCK_GAP bit-field to disable interrupt
 * at block gap. This bit is only valid in a 4-bit mode of the SDIO card.
 */
__STATIC_INLINE void XMC_SDMMC_DisableInterruptAtBlockGap(XMC_SDMMC_t *const sdmmc,
                                                          const XMC_SDMMC_CONFIG_t *config)

{
  XMC_ASSERT("XMC_SDMMC_EnableInterruptAtBlockGap: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  XMC_ASSERT("XMC_SDMMC_EnableInterruptAtBlockGap: This operation is only valid in 4-bit mode",
             (config->bus_width == XMC_SDMMC_DATA_LINES_1));

  sdmmc->BLOCK_GAP_CTRL &= (uint8_t)~SDMMC_BLOCK_GAP_CTRL_INT_AT_BLOCK_GAP_Msk;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param clk Desired clock frequency (::XMC_SDMMC_SDCLK_FREQ_SEL_t)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set SD clock frequency <br>
 *
 * \par
 * The function sets the CLOCK_CTRL register to configure the frequency of the SD clock
 * pin. The register is programmed with the divisor of the base clock frequency (clk).
 *
 * The following settings are permitted (8-bit divided clock mode): <br>
 * 00H: base clock (10MHz->63MHz) <br>
 * 01H: base clock divided by 2   <br>
 * 10H: base clock divided by 32  <br>
 * 02H: base clock divided by 4   <br>
 * 04H: base clock divided by 8   <br>
 * 08H: base clock divided by 16  <br>
 * 20H: base clock divided by 64  <br>
 * 40H: base clock divided by 128 <br>
 * 80H: base clock divided by 256 <br>
 *
 * \par<b>Note: </b><br>
 * The internal clock should be disabled before updating frequency clock select. Please
 * see section 2.2.14 -> "Clock Control Register" in the SD HOST specification for more
 * information.
 */
__STATIC_INLINE void XMC_SDMMC_SDClockFreqSelect(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_SDCLK_FREQ_SEL_t clk)
{
  XMC_ASSERT("XMC_SDMMC_SDClockFreqSelect: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  XMC_ASSERT("XMC_SDMMC_SDClockFreqSelect: Invalid clock frequency selection", XMC_SDMMC_CHECK_SDCLK_FREQ(clk));

  sdmmc->CLOCK_CTRL = (uint16_t)((sdmmc->CLOCK_CTRL & (uint32_t)~SDMMC_CLOCK_CTRL_SDCLK_FREQ_SEL_Msk) |
                                 (uint32_t)(clk << SDMMC_CLOCK_CTRL_SDCLK_FREQ_SEL_Pos));
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param bus_voltage Desired bus voltage (::XMC_SDMMC_BUS_VOLTAGE_t)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set SDMMC bus voltage <br>
 *
 * \par
 * The function sets the CLOCK_CTRL register to configure the bus voltage. Currently,
 * 3.3 volts is the supported voltage level. This function is relevant within the host
 * controller initialization routine.
 */
__STATIC_INLINE void XMC_SDMMC_SetBusVoltage(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_BUS_VOLTAGE_t bus_voltage)
{
  XMC_ASSERT("XMC_SDMMC_SetBusVoltage: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  XMC_ASSERT("XMC_SDMMC_SetBusVoltage: Invalid bus voltage", XMC_SDMMC_CHECK_BUS_VOLTAGE(bus_voltage));

  sdmmc->POWER_CTRL = (uint8_t)((sdmmc->POWER_CTRL & (uint32_t)~SDMMC_POWER_CTRL_SD_BUS_VOLTAGE_SEL_Msk) |
                                (uint32_t)(bus_voltage << SDMMC_POWER_CTRL_SD_BUS_VOLTAGE_SEL_Pos));
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param timeout Data line timeout value
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set data line timeout <br>
 *
 * \par
 * Use the function to set the interval by which the data line timeouts are detected. The
 * timeout clock frequency is generated by dividing the SD clock (TMCLK) by the timeout argument.
 * This function must be called before setting the bus voltage (XMC_SDMMC_SetBusVoltage()).
 */
__STATIC_INLINE void XMC_SDMMC_SetDataLineTimeout(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_DAT_TIMEOUT_COUNTER_t timeout)
{
  XMC_ASSERT("XMC_SDMMC_SetDataLineTimeout: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  XMC_ASSERT("XMC_SDMMC_SetDataLineTimeout: Invalid timeout", XMC_SDMMC_CHECK_DAT_TIMEOUT_COUNTER(timeout));

  sdmmc->TIMEOUT_CTRL = (uint8_t)((sdmmc->TIMEOUT_CTRL & (uint32_t)~SDMMC_TIMEOUT_CTRL_DAT_TIMEOUT_CNT_VAL_Msk) |
                                  (uint32_t)(timeout << SDMMC_TIMEOUT_CTRL_DAT_TIMEOUT_CNT_VAL_Pos));
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param lines Number of data lines to use  (::XMC_SDMMC_DATA_LINES_t)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set data transfer width <br>
 *
 * \par
 * Use the function to set the data transfer width. Before using this function, an ACMD6
 * command (with R1 response type) must be sent to switch the bus width.
 */
__STATIC_INLINE void XMC_SDMMC_SetDataTransferWidth(XMC_SDMMC_t *const sdmmc, XMC_SDMMC_DATA_LINES_t lines)
{
  XMC_ASSERT("XMC_SDMMC_SetDataTransferWidth: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  XMC_ASSERT("XMC_SDMMC_SetDataTransferWidth: Invalid no. of data lines", XMC_SDMMC_CHECK_DATA_LINES(lines));

  sdmmc->HOST_CTRL &= (uint8_t)~(XMC_SDMMC_DATA_LINES_1 | XMC_SDMMC_DATA_LINES_4 | XMC_SDMMC_DATA_LINES_8);
  sdmmc->HOST_CTRL |= (uint8_t)lines;
}

/**
 * @param sdmmc A constant pointer to XMC_SDMMC_t, pointing to the SDMMC base address
 * @param dir Transfer direction (::XMC_SDMMC_DATA_TRANSFER_DIR_t)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set data transfer direction <br>
 *
 * \par
 * Use the function to set the data transfer direction: host to card OR card to host. It
 * is typically used to configure block operations (read/write) on the SD card. For
 * example, XMC_SDMMC_DATA_TRANSFER_HOST_TO_CARD must be used for a write block operation.
 */
__STATIC_INLINE void XMC_SDMMC_SetDataTransferDirection(XMC_SDMMC_t *const sdmmc,
                                                        XMC_SDMMC_DATA_TRANSFER_DIR_t dir)
{
  XMC_ASSERT("XMC_SDMMC_SetDataTransferDirection: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));
  XMC_ASSERT("XMC_SDMMC_SetDataTransferDirection: Invalid direction", XMC_SDMMC_CHECK_DATA_TRANSFER_DIR(dir));

  sdmmc->TRANSFER_MODE = (uint16_t)((sdmmc->TRANSFER_MODE & (uint16_t)~SDMMC_TRANSFER_MODE_TX_DIR_SELECT_Msk) |
	                                (uint16_t)((uint16_t)dir << SDMMC_TRANSFER_MODE_TX_DIR_SELECT_Pos));
}

/**
 * @param None
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable delay on the command/data out lines <br>
 *
 * \par
 * Use the function to enable delay on the command/data out lines. Invoke this function
 * before selecting the number of delay elements.
 */
__STATIC_INLINE void XMC_SDMMC_EnableDelayCmdDatLines(void)
{
  SCU_GENERAL->SDMMCDEL |= (uint32_t)SCU_GENERAL_SDMMCDEL_TAPEN_Msk;
}

/**
 * @param None
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable delay on the command/data out lines <br>
 *
 * \par
 * Use the function to disable delay on the command/data out lines.
 */
__STATIC_INLINE void XMC_SDMMC_DisableDelayCmdDatLines(void)
{
  SCU_GENERAL->SDMMCDEL &= (uint32_t)~SCU_GENERAL_SDMMCDEL_TAPEN_Msk;
}

/**
 * @param tapdel Number of delay elements to select
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set number of delay elements on the command/data out lines <br>
 *
 * \par
 * Use the function to set the number of delay elements on the command/data out lines.
 * The function writes the delay value to the SDMMC delay control register (SDMMCDEL)
 * within the realm of the SCU peripheral. A delay of tapdel + 1 is considered as the
 * final selected number of delay elements.
 */
__STATIC_INLINE void XMC_SDMMC_SetDelay(uint8_t tapdel)
{
  SCU_GENERAL->SDMMCDEL = (uint32_t)((SCU_GENERAL->SDMMCDEL & (uint32_t)~SCU_GENERAL_SDMMCDEL_TAPDEL_Msk) |
	                                (uint32_t)(tapdel << SCU_GENERAL_SDMMCDEL_TAPDEL_Pos));
}

/**
 * @param None
 * @return None
 *
 * \par<b>Description: </b><br>
 * High speed enable <br>
 *
 * \par
 * Use the function to enable high speed operation. The default is a normal speed operation.
 * Once enabled, the host controller outputs command and data lines at the rising edge of the
 * SD clock (up to 50 MHz for SD).
 */
__STATIC_INLINE void XMC_SDMMC_EnableHighSpeed(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_EnableHighSpeed: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->HOST_CTRL |= (uint8_t)SDMMC_HOST_CTRL_HIGH_SPEED_EN_Msk;
}

/**
 * @param None
 * @return None
 *
 * \par<b>Description: </b><br>
 * High speed disable <br>
 *
 * \par
 * Use the function to disable high speed operation. The host controller will switch back
 * to a normal speed mode. In this mode, the host controller outputs command and data lines
 * at 25 MHz for SD.
 */
__STATIC_INLINE void XMC_SDMMC_DisableHighSpeed(XMC_SDMMC_t *const sdmmc)
{
  XMC_ASSERT("XMC_SDMMC_DisableHighSpeed: Invalid module pointer", XMC_SDMMC_CHECK_MODULE_PTR(sdmmc));

  sdmmc->HOST_CTRL &= (uint8_t)~SDMMC_HOST_CTRL_HIGH_SPEED_EN_Msk;
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* #if defined (SDMMC) */

#endif
