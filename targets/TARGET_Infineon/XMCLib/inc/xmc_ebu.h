/**
 * @file xmc_ebu.h
 * @date 2017-06-24
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
 * 2015-06-20:
 *     - Initial
 *
 * 2016-03-30:
 *     - Added ebu_data_hold_cycles_for_read_accesses to XMC_EBU_BUS_READ_CONFIG_t
 *     - Added ebu_device_addressing_mode and ebu_data_hold_cycles_for_write_accesses to XMC_EBU_BUS_WRITE_CONFIG_t
 *      
 * 2017-06-24:
 *     - Changed XMC_EBU_AddressSelectEnable() to make sure arbitartion mode is active
 *
 * @endcond 
 *
 */
 
#ifndef XMC_EBU_H
#define XMC_EBU_H

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
 
#include <xmc_common.h>
#if defined (EBU)
#include <xmc_scu.h>

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup EBU
 * @brief External Bus Unit (EBU) driver for the XMC45/XMC47/XMC48 microcontroller
 *
 * The External Bus Unit (EBU) controls the transactions between external memories or 
 * peripheral units, and the internal memories and peripheral units. Several external 
 * device configurations are supported; e.g. Asynchronous static memories, SDRAM 
 * and various flash memory types. It supports multiple programmable address regions. 
 *
 * The EBU low level driver provides functions to configure and initialize the EBU
 * hardware peripheral. 
 * @{
 */
 
/**********************************************************************************************************************
 * MACROS
**********************************************************************************************************************/

/**
 * A convenient symbol for the EBU peripheral base address
 */
#if defined (EBU)
# define XMC_EBU ((XMC_EBU_t *)EBU_BASE)
#else
# error 'EBU' base peripheral pointer not defined
#endif


/*
 * This macro is used in the LLD for assertion checks (XMC_ASSERT).
 */
#define XMC_EBU_CHECK_MODULE_PTR(p) ((p) == XMC_EBU)

/**********************************************************************************************************************
 * ENUMS
**********************************************************************************************************************/
 
/**
 * Status return values for EBU low level driver
 */ 
typedef enum XMC_EBU_STATUS
{
  XMC_EBU_STATUS_OK    = 0U, /**< Operation successful */
  XMC_EBU_STATUS_BUSY  = 1U, /**< Busy with a previous request */
  XMC_EBU_STATUS_ERROR = 3U  /**< Operation unsuccessful */
} XMC_EBU_STATUS_t;

/**
 * EBU clock divide ratio
 */
typedef enum  XMC_EBU_CLOCK_DIVIDE_RATIO
{
  XMC_EBU_CLOCK_DIVIDED_BY_1 = 0U, /**< Clock divided by 1 */
  XMC_EBU_CLOCK_DIVIDED_BY_2 = 1U, /**< Clock divided by 2 */
  XMC_EBU_CLOCK_DIVIDED_BY_3 = 2U, /**< Clock divided by 3 */
  XMC_EBU_CLOCK_DIVIDED_BY_4 = 3U  /**< Clock divided by 4 */
} XMC_EBU_CLOCK_DIVIDE_RATIO_t;

/**
 * EBU DIV2 clocking mode
 */
typedef enum  XMC_EBU_DIV2_CLK_MODE
{
  XMC_EBU_DIV2_CLK_MODE_OFF = 0U, /**< Divider 2 clock mode OFF */
  XMC_EBU_DIV2_CLK_MODE_ON = 1U   /**< Divider 2 clock mode ON */
} XMC_EBU_DIV2_CLK_MODE_t;

/**
 * EBU clocking mode
 */
typedef enum  XMC_EBU_CLK_MODE
{
  XMC_EBU_CLK_MODE_ASYNCHRONOUS_TO_AHB = 0U, /**< EBU is using standard clocking mode */
  XMC_EBU_CLK_MODE_SYNCHRONOUS_TO_CPU = 1U   /**< EBU is running at AHB bus clock divided by 2 */
} XMC_EBU_CLK_MODE_t;

/**
 * EBU arbitration mode
 */
typedef enum  XMC_EBU_ARB_MODE
{
  XMC_EBU_ARB_MODE_NOT_SELECTED     = 0U, /**< No Bus arbitration mode selected */
  XMC_EBU_ARB_MODE_ARBITER_MODE     = 1U, /**< Arbiter Mode arbitration mode selected */
  XMC_EBU_ARB_MODE_PARTICIPANT_MODE = 2U, /**< Participant arbitration mode selected */
  XMC_EBU_ARB_MODE_SOLE_MASTER_MODE = 3U  /**< Sole Master arbitration mode selected */
} XMC_EBU_ARB_MODE_t;

/**
 * EBU ALE mode
 */
typedef enum  XMC_EBU_ALE_MODE
{
  XMC_EBU_ALE_OUTPUT_IS_INV_ADV = 0U, /**< Output is ADV */
  XMC_EBU_ALE_OUTPUT_IS_ALE     = 1U  /**< Output is ALE */
} XMC_EBU_ALE_MODE_t;

/**
 * EBU clock status
 */
typedef enum XMC_EBU_CLK_STATUS
{
  XMC_EBU_CLK_STATUS_DISABLE_BIT = EBU_CLC_DISS_Msk,     /**< EBU Disable Status Bit */
  XMC_EBU_CLK_STATUS_MODE        = EBU_CLC_SYNCACK_Msk,  /**< EBU Clocking Mode Status */
  XMC_EBU_CLK_STATUS_DIV2_MODE   = EBU_CLC_DIV2ACK_Msk,  /**< DIV2 Clocking Mode Status */
  XMC_EBU_CLK_STATUS_DIV_RATIO   = EBU_CLC_EBUDIVACK_Msk /**< EBU Clock Divide Ratio Status */
} XMC_EBU_CLK_STATUS_t;

/**
 * EBU address selection
 */
typedef enum XMC_EBU_ADDRESS_SELECT
{
  XMC_EBU_ADDRESS_SELECT_MEMORY_REGION_ENABLE        = EBU_ADDRSEL0_REGENAB_Msk, /**< Memory Region Enable */
  XMC_EBU_ADDRESS_SELECT_ALTERNATE_REGION_ENABLE     = EBU_ADDRSEL0_ALTENAB_Msk, /**< Alternate Region Enable */
  XMC_EBU_ADDRESS_SELECT_MEMORY_REGION_WRITE_PROTECT = EBU_ADDRSEL0_WPROT_Msk    /**< Memory Region Write Protect */
} XMC_EBU_ADDRESS_SELECT_t;

/**
 * EBU bus write configuration status
 */
typedef enum XMC_EBU_BUSWCON_SELECT
{
  XMC_EBU_BUSWCON_SELECT_NAN_WORKAROUND = EBU_BUSWCON0_NAA_Msk, /**< Enable flash non-array access workaround */
  XMC_EBU_BUSWCON_SELECT_DEVICE_ADDRESSING_MODE = EBU_BUSWCON0_PORTW_Msk, /**< Device Addressing Mode */
} XMC_EBU_BUSWCON_SELECT_t;

/**
 * EBU burst length for synchronous burst
 */
typedef enum XMC_EBU_BURST_LENGTH_SYNC
{
  XMC_EBU_BURST_LENGTH_SYNC_1_DATA_ACCESS   = 0U, /**< 1 data access (default after reset) */
  XMC_EBU_BURST_LENGTH_SYNC_2_DATA_ACCESSES = 1U, /**< 2 data access */
  XMC_EBU_BURST_LENGTH_SYNC_4_DATA_ACCESSES = 2U, /**< 3 data access */
  XMC_EBU_BURST_LENGTH_SYNC_8_DATA_ACCESSES = 3U, /**< 4 data access */
} XMC_EBU_BURST_LENGTH_SYNC_t;

/**
 * EBU burst buffer mode
 */
typedef enum XMC_EBU_BURST_BUFFER_SYNC_MODE
{
  XMC_EBU_BURST_BUFFER_SYNC_LENGTH_SYNC_ENABLE = 0U, /**< Burst buffer length defined by value in FETBLEN */
  XMC_EBU_BURST_BUFFER_SYNC_SINGLE_MODE        = 1U  /**< All data required for transaction (single burst transfer) */
} XMC_EBU_BURST_BUFFER_SYNC_MODE_t;

/**
 * Read single stage synchronization
 */
typedef enum XMC_EBU_READ_STAGES_SYNC
{
  XMC_EBU_READ_STAGES_SYNC_TWO = 0U, /**< Two stages of synchronization used (maximum margin) */
  XMC_EBU_READ_STAGES_SYNC_ONE = 1U  /**< One stage of synchronization used (minimum latency) */
} XMC_EBU_READ_STAGES_SYNC_t;

/**
 * EBU burst flash clock feedback enable/disable
 */
typedef enum XMC_EBU_BURST_FLASH_CLOCK_FEEDBACK
{
  XMC_EBU_BURST_FLASH_CLOCK_FEEDBACK_DISABLE = 0U, /**< BFCLK feedback not used */
  XMC_EBU_BURST_FLASH_CLOCK_FEEDBACK_ENABLE  = 1U  /**< BFCLK feedback used */
} XMC_EBU_BURST_FLASH_CLOCK_FEEDBACK_t;

/**
 * EBU burst flash clock mode select
 */
typedef enum XMC_EBU_BURST_FLASH_CLOCK_MODE
{
  XMC_EBU_BURST_FLASH_CLOCK_MODE_RUN_CONTINUOSLY = 0U, /**< Burst flash clock runs continuously */
  XMC_EBU_BURST_FLASH_CLOCK_MODE_DISABLED_BETWEEN_ACCESSES = 1U /**< Burst flash clock disabled */
} XMC_EBU_BURST_FLASH_CLOCK_MODE_t;

/**
 * EBU flash non-array access
 */
typedef enum XMC_EBU_FLASH_NON_ARRAY_ACCESS
{
  XMC_EBU_FLASH_NON_ARRAY_ACCESS_DISNABLE = 0U, /**< Disable non-array access */
  XMC_EBU_FLASH_NON_ARRAY_ACCESS_ENABLE   = 1U  /**< Enable non-array access */
} XMC_EBU_FLASH_NON_ARRAY_ACCESS_t;

/**
 * EBU early chip select for synchronous burst
 */
typedef enum XMC_EBU_EARLY_CHIP_SELECT_SYNC_BURST
{
  XMC_EBU_EARLY_CHIP_SELECT_DELAYED     = 0U, /**< Chip select delayed */
  XMC_EBU_EARLY_CHIP_SELECT_NOT_DELAYED = 1U  /**< Chip select not delayed */
} XMC_EBU_EARLY_CHIP_SELECT_SYNC_BURST_t;

/**
 * EBU early burst signal enable for synchronous burst
 */
typedef enum XMC_EBU_BURST_SIGNAL_SYNC_BURST
{
  XMC_EBU_BURST_SIGNAL_SYNC_BURST_ADV_DELAYED     = 0U, /**< Chip select delayed */
  XMC_EBU_BURST_SIGNAL_SYNC_BURST_ADV_NOT_DELAYED = 1U  /**< Chip select not delayed */
} XMC_EBU_BURST_SIGNAL_SYNC_BURST_t;

/**
 * EBU burst address wrapping
 */
typedef enum XMC_EBU_BURST_ADDRESS_WRAPPING
{
  XMC_EBU_BURST_ADDRESS_WRAPPING_DISABLED = 0U, /**< Automatically re-aligns any non-aligned synchronous burst access */
  XMC_EBU_BURST_ADDRESS_WRAPPING_ENABLED  = 1U  /**< Starts any burst access at address specified by the AHB request */
} XMC_EBU_BURST_ADDRESS_WRAPPING_t;

/**
 * EBU reversed polarity at WAIT
 */
typedef enum XMC_EBU_WAIT_SIGNAL_POLARITY
{
  XMC_EBU_WAIT_SIGNAL_POLARITY_PIN_ACTIVE_LOW  = 0U, /**< OFF, input at WAIT pin is active low */
  XMC_EBU_WAIT_SIGNAL_POLARITY_PIN_ACTIVE_HIGH = 1U  /**< Polarity reversed, input at WAIT pin is active high */
} XMC_EBU_WAIT_SIGNAL_POLARITY_t;

/**
 * EBU byte control signal control
 */
typedef enum XMC_EBU_BYTE_CONTROL
{
  XMC_EBU_BYTE_CONTROL_FOLLOWS_CHIP_SELECT_TIMMING         = 0U, /**< Control signals follow chip select timing */
  XMC_EBU_BYTE_CONTROL_FOLLOWS_CONTROL_SIGNAL_TIMMING      = 1U, /**< Control signals follow control signal timing */
  XMC_EBU_BYTE_CONTROL_FOLLOWS_WRITE_ENABLE_SIGNAL_TIMMING = 2U  /**< Control signals follow write enable timing */
} XMC_EBU_BYTE_CONTROL_t;

/**
 * EBU device addressing mode
 */
typedef enum XMC_EBU_DEVICE_ADDRESSING_MODE
{
  XMC_EBU_DEVICE_ADDRESSING_MODE_16_BITS                  = 1U, /**< Address will only be driven onto AD[15:0] */
  XMC_EBU_DEVICE_ADDRESSING_MODE_TWIN_16_BITS_MULTIPLEXED = 2U, /**< Lower 16b will be driven onto A[15:0] & AD[15:0] */
  XMC_EBU_DEVICE_ADDRESSING_MODE_32_BITS_MULTIPLEXED      = 3U  /**< Full address driven onto A[15:0] & AD[15:0] */
} XMC_EBU_DEVICE_ADDRESSING_MODE_t;

/**
 * EBU external wait control
 */
typedef enum XMC_EBU_WAIT_CONTROL
{
  XMC_EBU_WAIT_CONTROL_OFF = 0U, /**< Default after reset; Wait control off */  
  XMC_EBU_WAIT_CONTROL_SYNC_EARLY_WAIT_ASYNC_ASYNC_INPUT_AT_WAIT = 1U,    /**< SYNC: Wait for page load (Early WAIT);
                                                                               ASYNC: Asynchronous input at WAIT */ 
  XMC_EBU_WAIT_CONTROL_SYNC_WAIT_WITH_DATA_ASYNC_SYNC_INPUT_AT_WAIT = 2U, /**< SYNC: Wait for page load (WAIT with data);
                                                                               ASYNC: Synchronous input at WAIT; */  
  XMC_EBU_WAIT_CONTROL_SYNC_ABORT_AND_RETRY_ACCESS = 3U                   /**< SYNC: Abort and retry access; */
} XMC_EBU_WAIT_CONTROL_t;

/**
 * EBU asynchronous address phase
 */
typedef enum XMC_EBU_ASYNCHRONOUS_ADDRESS_PHASE
{
  XMC_EBU_ASYNCHRONOUS_ADDRESS_PHASE_CLOCK_ENABLED_AT_BEGINNING_OF_ACCESS = 0U, /**< Enabled at beginning of access */
  XMC_EBU_ASYNCHRONOUS_ADDRESS_PHASE_CLOCK_ENABLED_AFTER_ADDRESS_PHASE    = 1U  /**< Enabled after address phase */
} XMC_EBU_ASYNCHRONOUS_ADDRESS_PHASE_t;

/**
 * EBU device type for region
 */
typedef enum XMC_EBU_DEVICE_TYPE
{
  XMC_EBU_DEVICE_TYPE_MUXED_ASYNCHRONOUS_TYPE   = 0U, /**< Device type muxed asynchronous */
  XMC_EBU_DEVICE_TYPE_MUXED_BURST_TYPE          = 1U, /**< Device type muxed burst */
  XMC_EBU_DEVICE_TYPE_NAND_FLASH                = 2U, /**< Device type NAND flash */
  XMC_EBU_DEVICE_TYPE_MUXED_CELLULAR_RAM        = 3U, /**< Device type muxed cellular RAM  */
  XMC_EBU_DEVICE_TYPE_DEMUXED_ASYNCHRONOUS_TYPE = 4U, /**< Device type de-muxed asynchronous */
  XMC_EBU_DEVICE_TYPE_DEMUXED_BURST_TYPE        = 5U, /**< Device type de-muxed burst */
  XMC_EBU_DEVICE_TYPE_DEMUXED_PAGE_MODE         = 6U, /**< Device type de-muxed page mode */
  XMC_EBU_DEVICE_TYPE_DEMUXED_CELLULAR_RAM      = 7U, /**< Device type de-muxed cellular RAM  */
  XMC_EBU_DEVICE_TYPE_SDRAM                     = 8U  /**< Device type SDRAM  */
} XMC_EBU_DEVICE_TYPE_t;

/**
 * EBU lock chip select
 */
typedef enum XMC_EBU_LOCK_CHIP_SELECT
{
  XMC_EBU_LOCK_CHIP_SELECT_DISABLED = 0U, /**< Chip select cannot be locked */
  XMC_EBU_LOCK_CHIP_SELECT_ENABLED  = 1U  /**< Chip select automatically locked after a write operation */
} XMC_EBU_LOCK_CHIP_SELECT_t;

/**
 * EBU Frequency of external clock at pin BFCLKO
 */
typedef enum XMC_EBU_FREQUENCY_EXT_CLK_PIN
{
  XMC_EBU_FREQ_EXT_CLK_PIN_EQUAL_TO_INT_CLK   = 0U, /**< Equal to INT_CLK frequency */
  XMC_EBU_FREQ_EXT_CLK_PIN_HALF_OF_INT_CLK    = 1U, /**< 1/2 of INT_CLK frequency */
  XMC_EBU_FREQ_EXT_CLK_PIN_THIRD_OF_INT_CLK   = 2U, /**< 1/3 of INT_CLK frequency */
  XMC_EBU_FREQ_EXT_CLK_PIN_QUARTER_OF_INT_CLK = 3U  /**< 1/4 of INT_CLK frequency */
} XMC_EBU_FREQ_EXT_CLK_PIN_t;

/**
 * EBU extended data
 */
typedef enum XMC_EBU_EXT_DATA
{
  XMC_EBU_EXT_DATA_OUTPUT_EVERY_1_BFCLK_CYCLES = 0U, /**< External memory outputs data every BFCLK cycle */
  XMC_EBU_EXT_DATA_OUTPUT_EVERY_2_BFCLK_CYCLES = 1U, /**< External memory outputs data every two BFCLK cycles */
  XMC_EBU_EXT_DATA_OUTPUT_EVERY_4_BFCLK_CYCLES = 2U, /**< External memory outputs data every four BFCLK cycles */
  XMC_EBU_EXT_DATA_OUTPUT_EVERY_8_BFCLK_CYCLES = 3U  /**< External memory outputs data every eight BFCLK cycles */
} XMC_EBU_EXT_DATA_t;

/**
 * EBU SDRAM clock mode select
 */
typedef enum XMC_EBU_SDRAM_CLK_MODE
{
  XMC_EBU_SDRAM_CLK_MODE_CONTINUOUSLY_RUNS         = 0U, /**< Clock continuously running */
  XMC_EBU_SDRAM_CLK_MODE_DISABLED_BETWEEN_ACCESSES = 1U  /**< Clock disabled between accesses */
} XMC_EBU_SDRAM_CLK_MODE_t;

/**
 * EBU power save mode used for gated clock mode
 */
typedef enum XMC_EBU_SDRAM_PWR_MODE
{
  XMC_EBU_SDRAM_PWR_MODE_PRECHARGE_BEFORE_CLK_STOP      = 0U, /**< Precharge before clock stop */
  XMC_EBU_SDRAM_PWR_MODE_AUTO_PRECHARGE_BEFORE_CLK_STOP = 1U, /**< Auto-precharge before clock stop */
  XMC_EBU_SDRAM_PWR_MODE_ACTIVE_PWR_DOWN                = 2U, /**< Active power down (stop clock without precharge) */
  XMC_EBU_SDRAM_PWR_MODE_CLK_STOP_PWR_DOWN              = 3U  /**< Clock stop power down */
} XMC_EBU_SDRAM_PWR_MODE_t;

/**
 * EBU disable SDRAM clock output
 */
typedef enum XMC_EBU_SDRAM_CLK_OUTPUT
{
  XMC_EBU_SDRAM_CLK_OUTPUT_ENABLED  = 0U, /**< Clock output enabled */
  XMC_EBU_SDRAM_CLK_OUTPUT_DISABLED = 1U  /**< Clock output disabled */
} XMC_EBU_SDRAM_CLK_OUTPUT_t;

/**
 *  EBU mask for bank tag
 */
typedef enum XMC_EBU_SDRAM_MASK_FOR_BANKM_TAG
{
  XMC_EBU_SDRAM_MASK_FOR_BANK_TAG_ADDRESS_21_to_20 = 1U, /**< Mask for bank tag addresses 21 to 20 */
  XMC_EBU_SDRAM_MASK_FOR_BANK_TAG_ADDRESS_22_to_21 = 2U, /**< Mask for bank tag addresses 22 to 21 */
  XMC_EBU_SDRAM_MASK_FOR_BANK_TAG_ADDRESS_23_to_22 = 3U, /**< Mask for bank tag addresses 23 to 22 */
  XMC_EBU_SDRAM_MASK_FOR_BANK_TAG_ADDRESS_24_to_23 = 4U, /**< Mask for bank tag addresses 24 to 23 */
  XMC_EBU_SDRAM_MASK_FOR_BANK_TAG_ADDRESS_25_to_24 = 5U, /**< Mask for bank tag addresses 25 to 24 */
  XMC_EBU_SDRAM_MASK_FOR_BANK_TAG_ADDRESS_26_to_25 = 6U, /**< Mask for bank tag addresses 26 to 25 */
  XMC_EBU_SDRAM_MASK_FOR_BANK_TAG_ADDRESS_26       = 7U  /**< Mask for bank tag addresses 26 */
} XMC_EBU_SDRAM_MASK_FOR_BANK_TAG_t;

/**
 * EBU Mask for row tag
 */
typedef enum XMC_EBU_SDRAM_MASK_FOR_ROW_TAG
{
  XMC_EBU_SDRAM_MASK_FOR_ROW_TAG_ADDRESS_26_to_9  = 1U, /**< Mask for row tag addresses 26 to 9 */
  XMC_EBU_SDRAM_MASK_FOR_ROW_TAG_ADDRESS_26_to_10 = 2U, /**< Mask for row tag addresses 26 to 10 */
  XMC_EBU_SDRAM_MASK_FOR_ROW_TAG_ADDRESS_26_to_11 = 3U, /**< Mask for row tag addresses 26 to 11 */
  XMC_EBU_SDRAM_MASK_FOR_ROW_TAG_ADDRESS_26_to_12 = 4U, /**< Mask for row tag addresses 26 to 12 */
  XMC_EBU_SDRAM_MASK_FOR_ROW_TAG_ADDRESS_26_to_13 = 5U  /**< Mask for row tag addresses 26 to 13 */
} XMC_EBU_SDRAM_MASK_FOR_ROW_TAG_t;

/**
 * Number of address bits from bit 0 to be used for column address
 */
typedef enum XMC_EBU_SDRAM_WIDTH_OF_COLUMN_ADDRESS
{
  XMC_EBU_SDRAM_WIDTH_OF_COLUMN_ADDRESS_8_to_0  = 1U, /**< Address [8:0] */
  XMC_EBU_SDRAM_WIDTH_OF_COLUMN_ADDRESS_9_to_0  = 2U, /**< Address [9:0] */
  XMC_EBU_SDRAM_WIDTH_OF_COLUMN_ADDRESS_10_to_0 = 3U  /**< Address [10:0] */
} XMC_EBU_SDRAM_WIDTH_OF_COLUMN_ADDRESS_t;

/**
 * Number of clocks between a READ command and the availability of data
 */
typedef enum XMC_EBU_SDRAM_CAS_LATENCY
{
  XMC_EBU_SDRAM_CAS_LATENCY_2_CLKS = 2U, /**< 2 clocks between a READ command and the availability of data */
  XMC_EBU_SDRAM_CAS_LATENCY_3_CLKS = 3U  /**< 3 clocks between a READ command and the availability of data */
} XMC_EBU_SDRAM_CAS_LATENCY_t;

/**
 * Number of locations can be accessed with a single command
 */
typedef enum XMC_EBU_SDRAM_BURST_LENGTH
{
  XMC_EBU_SDRAM_BURST_LENGTH_1_LOCATION  = 0U, /**< One location accessed with a single command */
  XMC_EBU_SDRAM_BURST_LENGTH_2_LOCATION  = 1U, /**< Two location accessed with a single command */
  XMC_EBU_SDRAM_BURST_LENGTH_4_LOCATION  = 2U, /**< Four location accessed with a single command */
  XMC_EBU_SDRAM_BURST_LENGTH_8_LOCATION  = 3U, /**< Eight location accessed with a single command */
  XMC_EBU_SDRAM_BURST_LENGTH_16_LOCATION = 4U  /**< Sixteen location accessed with a single command */
} XMC_EBU_SDRAM_BURST_LENGTH_t;

/**
 * EBU SDRAM status
 */
typedef enum XMC_EBU_SDRAM_STATUS
{
  XMC_EBU_SDRAM_STATUS_RX_ERROR = EBU_SDRSTAT_SDERR_Msk, /**< Detected an error when returning read data */
  XMC_EBU_SDRAM_STATUS_BUSY = EBU_SDRSTAT_SDRMBUSY_Msk, /**< The status of power-up initialization sequence */
  XMC_EBU_SDRAM_STATUS_REFRESH_ERROR = EBU_SDRSTAT_REFERR_Msk /**< Failed previous refresh req collides with new req */
} XMC_EBU_SDRAM_STATUS_t;

/**
 * SDRAM refresh status
 */
typedef enum XMC_EBU_SDRAM_RFRSH_STATUS
{
  XMC_EBU_SDRAM_RFRSH_STATUS_SELF_REFRESH_ENTRY_STATUS = EBU_SDRMREF_SELFRENST_Msk, /**< Self refresh entry command issue successful */
  XMC_EBU_SDRAM_RFRSH_STATUS_SELF_REFRESH_EXIT_STATUS = EBU_SDRMREF_SELFREXST_Msk   /**< Self refresh exit command issue successful */
} XMC_EBU_SDRAM_RFRSH_STATUS_t;


/**********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/
 
/* Anonymous structure/union guard start */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__TASKING__)
  #pragma warning 586
#endif

/**
 * Clock configuration values of EBU  <br>
 *
 * The structure presents a convenient way to set/obtain the clock configuration
 * values for clock mode, div2 clock mode and clock divide ratio of EBU.
 * The XMC_EBU_Init() can be used to populate the structure with the clock
 * configuration values of the EBU module.
 */ 
typedef struct XMC_EBU_CLK_CONFIG
{
  union
  { 
    uint32_t raw0;
    struct
    {
      uint32_t                        : 16;
      uint32_t ebu_clk_mode           : 1; /**< Clocking mode (::XMC_EBU_CLK_MODE_t) */
      uint32_t ebu_div2_clk_mode      : 1; /**< DIV2 clocking mode (::XMC_EBU_DIV2_CLK_MODE_t) */
      uint32_t ebu_clock_divide_ratio : 2; /**< Clock divide ratio (::XMC_EBU_CLOCK_DIVIDE_RATIO_t) */
      uint32_t                        : 12;
    };
  };
} XMC_EBU_CLK_CONFIG_t;

/**
 * Mode configuration values for EBU <br>
 *
 * The structure presents a convenient way to set/obtain the mode configuration,
 * SDRAM tristate, external clock, arbitration, timeout control and ALE mode for
 * EBU. The XMC_EBU_Init() can be used to populate the structure with the
 * clock configuration values of the EBU module.
 */ 
typedef struct XMC_EBU_MODE_CONFIG
{ 
  union
  {
    uint32_t raw0;
    struct
    {
      uint32_t                      : 2;
      uint32_t ebu_sdram_tristate   : 1; /**< 0 - SDRAM cannot be shared; 1 - SDRAM can be shared */
      uint32_t                      : 1;
      uint32_t ebu_extlock          : 1; /**< 0 - ext bus is not locked after the EBU gains ownership; 1 - ext bus is not locked */
      uint32_t ebu_arbsync          : 1; /**< 0 - arbitration inputs are sync; 1 - arbitration inputs are async */
      uint32_t ebu_arbitration_mode : 2; /**< Arbitration mode selection (::XMC_EBU_ARB_MODE_t) */
      uint32_t bus_timeout_control  : 8; /**< Determines num of inactive cycles leading to a bus timeout after the EBU gains ownership <br>
                                              00H: Timeout is disabled <br>
                                              01H: Timeout is generated after 1 x 8 clock cycles <br>
                                              FFH: Timeout is generated after 255 x 8 clock cycles <br> */
      uint32_t                      : 15;
      uint32_t ebu_ale_mode         : 1; /**< ALE mode. Switch the ADV output to be an active high ALE signal instead of active low ADV. (::XMC_EBU_ALE_MODE_t) */
    };
  };
} XMC_EBU_MODE_CONFIG_t;

/**
 * GPIO mode configuration for the allocated EBU ports <br>
 * Configuring this structure frees the allocated EBU ports for GPIO
 * functionality. The XMC_EBU_Init() is used to populate the structure
 * with the GPIO mode for the allocated EBU ports.
 */ 
typedef struct XMC_EBU_FREE_PINS_TO_GPIO
{
  union
  {
    uint32_t raw0;
    struct
    {
      uint32_t                   : 16;
      uint32_t address_pins_gpio : 9; /**< 0 - Address bit required for addressing memory; 1 - Address bit available as GPIO */
      uint32_t adv_pin_gpio      : 1; /**< Adv pin to GPIO mode */
      uint32_t                   : 6;
    };
  };
} XMC_EBU_FREE_PINS_TO_GPIO_t;

/**
 * Read configuration for a region of EBU <br>
 *
 * The structure presents a convenient way to set/obtain the read and read timing
 * configuration for a region for EBU. The XMC_EBU_ConfigureRegion() can be
 * used to populate the structure with the read configuration values for EBU.
 */ 
typedef struct XMC_EBU_BUS_READ_CONFIG
{
  /* EBU read configuration parameters */
  union
  {
    uint32_t raw0;
    struct
    {
      uint32_t ebu_burst_length_sync                         : 3; /**< Burst length for synchronous burst (::XMC_EBU_BURST_LENGTH_SYNC_t) */
      uint32_t ebu_burst_buffer_sync_mode                    : 1; /**< Burst buffer mode (::XMC_EBU_BURST_BUFFER_SYNC_MODE_t) */
      uint32_t ebu_read_stages_synch                         : 1; /**< Read single stage synchronization */
      uint32_t ebu_burst_flash_clock_feedback                : 1; /**< Burst flash clock feedback enable/disable (::XMC_EBU_BURST_FLASH_CLOCK_FEEDBACK_t) */
      uint32_t ebu_burst_flash_clock_mode                    : 1; /**< Burst flash clock mode select (::XMC_EBU_BURST_FLASH_CLOCK_MODE_t) */
      uint32_t ebu_flash_non_array_access                    : 1; /**< flash non-array access (::XMC_EBU_FLASH_NON_ARRAY_ACCESS_t) */
      uint32_t                                               : 8;
      uint32_t ebu_early_chip_select_sync_burst              : 1; /**< Early chip select for sync burst (::XMC_EBU_EARLY_CHIP_SELECT_SYNC_BURST_t) */
      uint32_t ebu_burst_signal_sync                         : 1; /**< Early burst signal enable for synchronous burst (::XMC_EBU_BURST_SIGNAL_SYNC_BURST_t) */
      uint32_t ebu_burst_address_wrapping                    : 1; /**< Burst address wrapping (::XMC_EBU_BURST_ADDRESS_WRAPPING_t) */
      uint32_t ebu_wait_signal_polarity                      : 1; /**< Reversed polarity at WAIT (::XMC_EBU_WAIT_SIGNAL_POLARITY_t) */
      uint32_t ebu_byte_control                              : 2; /**< Byte control signal control (::XMC_EBU_BYTE_CONTROL_t) */
      uint32_t ebu_device_addressing_mode                    : 2; /**< Device addressing mode (::XMC_EBU_DEVICE_ADDRESSING_MODE_t) */
      uint32_t ebu_wait_control                              : 2; /**< External wait control (::XMC_EBU_WAIT_CONTROL_t) */
      uint32_t ebu_asynchronous_address_phase                : 1; /**< Asynchronous address phase (::XMC_EBU_ASYNCHRONOUS_ADDRESS_PHASE_t) */
      uint32_t                                               : 1;
      uint32_t ebu_device_type                               : 4; /**< Device type for region (::XMC_EBU_DEVICE_TYPE_t) */
    };
  };  
  /* EBU read access parameters */
  union
  {
    uint32_t raw1;
    struct
    {
      /**
       * Recovery Cycles between Different Regions:     <br>
       * This bit field determines the number of clock cycles of the Recovery Phase between consecutive accesses directed to different regions or different types of access.<br>
       * 0000B: No recovery phase clock cycles available <br>
       * 0001B: 1 clock cycle selected                   <br>
       * ...                                            <br>
       * 1110B: 14 clock cycles selected                <br>
       * 1111B: 15 clock cycles selected                <br>
       */
      uint32_t ebu_recovery_cycles_between_different_regions : 4;
      /**
       * Recovery cycles after read accesses:           <br>
       * This bit field determines the basic number of clock cycles of the Recovery Phase at the end of read accesses.<br>
       * 000B: No recovery phase clock cycles available <br>
       * 001B: 1 clock cycle selected                   <br>
       * ...                                            <br>
       * 110B: 6 clock cycles selected                  <br>
       * 111B: 7 clock cycles selected                  <br>
       */
      uint32_t ebu_recovery_cycles_after_read_accesses       : 3;
      /**
       * Programmed wait states for read accesses: <br>
       * Number of programmed wait states for read accesses. For synchronous accesses, this will always be adjusted so that the phase exits on a rising edge of the external clock.<br>
       * 00000B:  1 wait state                     <br>
       * 00001B:  1 wait state                     <br>
       * 00010B:  2 wait state                     <br>
       * ...                                       <br>
       * 11110B:  30 wait states                   <br>
       * 11111B:  31 wait states                   <br>
       */
      uint32_t ebu_programmed_wait_states_for_read_accesses  : 5;
      /**
       * Data Hold Cycles for Read Accesses <br>
       * This bit field determines the basic number of Data Hold phase clock cycles during read accesses. It has no effect in the current implementation<br>
       */
      uint32_t ebu_data_hold_cycles_for_read_accesses: 4;
      /**
       * Frequency of external clock at pin BFCLKO (::XMC_EBU_FREQ_EXT_CLK_PIN_t)
       */
      uint32_t ebu_freq_ext_clk_pin                          : 2;
      /**
       * EBU Extended data (::XMC_EBU_EXT_DATA_t)
       */
      uint32_t ebu_ext_data                                  : 2;
      /**
       * Command delay cycles:            <br>
       * This bit field determines the basic number of Command Delay phase clock cycles.<br>
       * 0000B: 0 clock cycle selected    <br>
       * 0001B: 1 clock cycle selected    <br>
       * ...                              <br>
       * 1110B: 14 clock cycles selected  <br>
       * 1111B: 15 clock cycles selected  <br>
       */
       uint32_t command_delay_lines                          : 4;
       /**
        * Address hold cycles:            <br>
        * This bit field determines the number of clock cycles of the address hold phase.<br>
        * 0000B: 0 clock cycle selected   <br>
        * 0001B: 1 clock cycle selected   <br>
        * ...                             <br>
        * 1110B: 14 clock cycles selected <br>
        * 1111B: 15 clock cycles selected <br>
        */
       uint32_t address_hold_cycles                          : 4;
        /**
         * Address Cycles:
         * This bit field determines the number of clock cycles of the address phase.<br>
         * 0000B: 1 clock cycle selected   <br>
         * 0001B: 1 clock cycle selected   <br>
         * ...                             <br>
         * 1110B: 14 clock cycles selected <br>
         * 1111B: 15 clock cycles selected <br>
         */
       uint32_t address_cycles                               : 4;
    };
  };
} XMC_EBU_BUS_READ_CONFIG_t;

/**
 * Write configuration for a region of EBU <br>
 *
 * The structure presents a convenient way to set/obtain the write and write timing
 * configurations for a region of EBU. The XMC_EBU_ConfigureRegion() can be used
 * to populate the structure with the write configuration values of EBU.
 */ 
typedef struct XMC_EBU_BUS_WRITE_CONFIG
{
  /* EBU write configuration parameters */
  union
  {
    uint32_t raw0;
    struct
    {
      uint32_t ebu_burst_length_sync                         : 3;  /**< Burst length for synchronous burst (::XMC_EBU_BURST_LENGTH_SYNC_t) */
      uint32_t ebu_burst_buffer_sync_mode                    : 1;  /**< Burst buffer mode (::XMC_EBU_BURST_BUFFER_SYNC_MODE_t) */
      uint32_t                                               : 12;
      uint32_t ebu_early_chip_select_sync_burst              : 1;  /**< Early chip select for sync burst (::XMC_EBU_EARLY_CHIP_SELECT_SYNC_BURST_t) */
      uint32_t ebu_burst_signal_sync                         : 1;  /**< Early burst signal enable for synchronous burst  (::XMC_EBU_BURST_SIGNAL_SYNC_BURST_t) */
      uint32_t                                               : 1;
      uint32_t ebu_wait_signal_polarity                      : 1;  /**< Reversed polarity at WAIT (::XMC_EBU_WAIT_SIGNAL_POLARITY_t) */
      uint32_t ebu_byte_control                              : 2;  /**< Byte control signal control (::XMC_EBU_BYTE_CONTROL_t) */
      uint32_t ebu_device_addressing_mode                    : 2;  /**< Device addressing mode (::XMC_EBU_DEVICE_ADDRESSING_MODE_t) */
      uint32_t ebu_wait_control                              : 2;  /**< External wait control (::XMC_EBU_WAIT_CONTROL_t) */
      uint32_t ebu_asynchronous_address_phase                : 1;  /**< Asynchronous address phase (::XMC_EBU_ASYNCHRONOUS_ADDRESS_PHASE_t) */
      uint32_t ebu_lock_chip_select                          : 1;  /**< Lock chip select (::XMC_EBU_LOCK_CHIP_SELECT_t) */
      uint32_t ebu_device_type                               : 4;  /**< Device type for region (::XMC_EBU_DEVICE_TYPE_t) */
    };
  };
  /* EBU write access parameters */
  union
  {
    uint32_t raw1;
    struct
    {
      /**
       * Recovery cycles between different regions:       <br>
       * This bit field determines the number of clock cycles of the Recovery Phase between consecutive accesses directed to different regions or different types of access.<br>
       * 0000B: No recovery phase clock cycles available  <br>
       * 0001B: 1 clock cycle selected                    <br>
       * ...                                              <br>
       * 1110B: 14 clock cycles selected                  <br>
       * 1111B: 15 clock cycles selected                  <br>
       */
      uint32_t ebu_recovery_cycles_between_different_regions : 4;

      /**
       * Recovery cycles after write accesses:          <br>
       * This bit field determines the basic number of clock cycles of the Recovery Phase at the end of write accesses.<br>
       * 000B: No recovery phase clock cycles available <br>
       * 001B: 1 clock cycle selected                   <br> 
       * ...                                            <br>
       * 110B: 6 clock cycles selected                  <br>
       * 111B: 7 clock cycles selected                  <br>
       */
      uint32_t ebu_recovery_cycles_after_write_accesses      : 3;

      /**
       * Programmed wait states for write accesses: <br>
       * Number of programmed wait states for write accesses. For synchronous accesses, this will always be adjusted so that the phase exits on a rising edge of the external clock.<br>
       * 00000B:  1 wait state                      <br>
       * 00001B:  1 wait state                      <br>
       * 00010B:  2 wait state                      <br>
       * ...                                        <br>
       * 11110B:  30 wait states                    <br>
       * 11111B:  31 wait states                    <br>
      */
      uint32_t ebu_programmed_wait_states_for_write_accesses : 5;

      /**
       * Data Hold Cycles for Write Accesses
       * This bit field determines the basic number of Data Hold phase clock cycles during write accesses.<br>
       * 0000B: No recovery phase clock cycles available  <br>
       * 0001B: 1 clock cycle selected                    <br>
       * ...                                              <br>
       * 1110B: 14 clock cycles selected                  <br>
       * 1111B: 15 clock cycles selected                  <br>
       */
      uint32_t ebu_data_hold_cycles_for_write_accesses : 4;
      /**<
       * Frequency of external clock at pin BFCLKO (::XMC_EBU_FREQ_EXT_CLK_PIN_t)
       */
      uint32_t ebu_freq_ext_clk_pin                          : 2;
      /**
       * EBU extended data (::XMC_EBU_EXT_DATA_t)
       */
      uint32_t ebu_ext_data                                  : 2;
      /**
       * Command delay cycles:           <br>
       * This bit field determines the basic number of Command Delay phase clock cycles.<br>
       * 0000B: 0 clock cycle selected   <br>
       * 0001B: 1 clock cycle selected   <br>
       * ...                             <br>
       * 1110B: 14 clock cycles selected <br>
       * 1111B: 15 clock cycles selected <br>
       */
      uint32_t command_delay_lines                           : 4;
      /** Address hold cycles:           <br>
       * This bit field determines the number of clock cycles of the address hold phase.<br>
       * 0000B: 0 clock cycle selected   <br>
       * 0001B: 1 clock cycle selected   <br>
       * ...                             <br>
       * 1110B: 14 clock cycles selected <br>
       * 1111B: 15 clock cycles selected <br>
       */
      uint32_t address_hold_cycles                           : 4;
      /**
       * Address cycles:                 <br>
       * This bit field determines the number of clock cycles of the address phase.<br>
       * 0000B: 1 clock cycle selected   <br>
       * 0001B: 1 clock cycle selected   <br>
       * ...                             <br>
       * 1110B: 14 clock cycles selected <br>
       * 1111B: 15 clock cycles selected <br>
       */
     uint32_t address_cycles                                 : 4;
    };
  };
}XMC_EBU_BUS_WRITE_CONFIG_t;

/**
 * SDRAM configuration structure <br>
 *
 * The structure is a placeholder for setting (and obtaining) the SDRAM configuration,
 * operation mode configuration and the right refresh parameters. The XMC_EBU_ConfigureSdram()
 * can be used to populate the structure with the SDRAM operation mode and
 * refresh parameters configuration.
 */
typedef struct XMC_EBU_SDRAM_CONFIG
{
  /* EBU SDRAM control parameters */
  union
  {
    uint32_t raw0;
    struct
    {
      /**
       * Number of clock cycles between row activate command and a precharge
       * command 
       */
      uint32_t ebu_row_precharge_delay_counter            : 4;
      /** 
       * (CRFSH) Number of refresh commands issued during powerup init sequence:
       * Perform CRFSH + 1 refresh cycles
       */
      uint32_t ebu_init_refresh_commands_counter          : 4;
      /**
       * (CRSC) Number of NOP cycles after a mode register set command:
       * Insert CRSC + 1 NOP cycles 
       */
      uint32_t ebu_mode_register_set_up_time              : 2;
      /**
       * (CRP) Number of NOP cycles inserted after a precharge command:
       * Insert CRP + 1 NOP cycles 
       */
      uint32_t ebu_row_precharge_time_counter             : 2;
      /**
       * Number of address bits from bit 0 to be used for column address
       */
      uint32_t ebu_sdram_width_of_column_address          : 2;
      /**
       * (CRCD) Number of NOP cycles between a row address and a column
       * address: Insert CRCD + 1 NOP cycles
       */
      uint32_t ebu_sdram_row_to_column_delay_counter      : 2;
      /**
       * Row cycle time counter: Insert (CRCE * 8) + CRC + 1 NOP cycles
       */
      uint32_t ebu_sdram_row_cycle_time_counter           : 3;
      /**
       * Mask for row tag
       */
      uint32_t ebu_sdram_mask_for_row_tag                 : 3;
      /**
       * Mask for bank tag
       */
      uint32_t ebu_sdram_mask_for_bank_tag                : 3;
      /**
       * Extension to the Row cycle time counter (CRCE)
       */
      uint32_t ebu_sdram_row_cycle_time_counter_extension : 3;
      /**
       * Disable SDRAM clock output
       */
      uint32_t ebu_sdram_clk_output                       : 1;
      /**
       * Power Save Mode used for gated clock mode
       */
      uint32_t ebu_sdram_pwr_mode                         : 2;
      /**
       * SDRAM clock mode select
       */
      uint32_t ebu_sdram_clk_mode                         : 1;
    };
  };
  /* EBU SDRAM mode parameters */
  union
  {
    uint32_t raw1;
    struct
    {
      /**
       * Number of locations can be accessed with a single command
       */
      uint32_t ebu_sdram_burst_length                     : 3;
      uint32_t                                            : 1;
      /**
       * Number of clocks between a READ command and the availability
       * of data
       */
      uint32_t ebu_sdram_casclk_mode                      : 3;
      uint32_t                                            : 8;
      /**
       * Cold start
       */
      uint32_t ebu_sdram_cold_start: 1;
      /**
       * Value to be written to the extended mode register of a mobile
       * SDRAM device
       */
      uint32_t ebu_sdram_extended_operation_mode          : 12;
      /**
       * Value to be written to the bank select pins of a mobile SDRAM
       * device during an extended mode register write operation
       */
      uint32_t ebu_sdram_extended_operation_bank_select   : 4;
    };
  };
  /* EBU SDRAM refresh parameters */
  union
    {
    uint32_t raw2;
    struct
    {
      /**
       * Number of refresh counter period:
       * Refresh period is 'num_refresh_counter_period' x 64 clock cycles
       */
      uint32_t ebu_sdram_num_refresh_counter_period       : 6;
      /**
       * Number of refresh commands
       */
      uint32_t ebu_sdram_num_refresh_cmnds                : 3;
      uint32_t                                            : 1;
      /**
       * If 1, the self refresh exit command is issued to all SDRAM devices
       * regardless of their attachment to type 0 or type 1
       */
      uint32_t ebu_sdram_self_refresh_exit                : 1;
      uint32_t                                            : 1;
      /**
       * If "1", the self refresh entry command is issued to all SDRAM devices,
       * regardless regardless of their attachment to type 0 or type 1
       */
      uint32_t ebu_sdram_self_refresh_entry               : 1;
      /**
       * If 1, memory controller will automatically issue the "self refresh
       * entry" command to all SDRAM devices when it gives up control of the
       * external bus. It will also automatically issue "self refresh exit"
       * when it regains control of the bus
       */
      uint32_t ebu_sdram_auto_self_refresh                : 1;
      /**
       * Extended number of refresh counter period
       */
      uint32_t ebu_sdram_extended_refresh_counter_period  : 2;
      /**
       * Number of NOP cycles inserted after a self refresh exit before a
       * command is permitted to the SDRAM/DDRAM
       */
      uint32_t ebu_sdram_self_refresh_exit_delay          : 8;
      /**
       * If 1, an auto refresh cycle will be performed; If 0, no refresh will
       * be performed
       */
      uint32_t ebu_sdram_auto_refresh                     : 1;
      /**
       * Number of NOPs after the SDRAM controller exits power down before an
       * active command is permitted
       */
      uint32_t ebu_sdram_delay_on_power_down_exit         : 3;
      uint32_t : 4;
    };
  };
} XMC_EBU_SDRAM_CONFIG_t;

/* Anonymous structure/union guard end */
#if defined (__CC_ARM)
  #pragma pop
#elif defined (__TASKING__)
  #pragma warning restore
#endif

/**
 * EBU region initialization with read configurations <br>
 *
 * The structure presents a convenient way to set/obtain the read and read timing
 * configurations for a region of EBU. The XMC_EBU_ConfigureRegion() can be
 * used to populate the structure with the read configuration values of EBU  
 */  
typedef struct XMC_EBU_REGION_READ_CONFIG
{
  const uint32_t ebu_region_no;                  /**< Number of region*/
  XMC_EBU_BUS_READ_CONFIG_t ebu_bus_read_config; /**< Read configuration and access parameters structure */
} XMC_EBU_REGION_READ_CONFIG_t;

/**
 * EBU region initialization with write configurations <br>
 *
 * The structure presents a convenient way to set/obtain the write and write
 * timing configurations for a region of EBU. The XMC_EBU_ConfigureRegion()
 * can be used to populate the structure with the write configuration
 * values of EBU.
 */  
typedef struct XMC_EBU_REGION_WRITE_CONFIG
{
  const uint32_t ebu_region_no;                    /**< Number of refresh counter period */
  XMC_EBU_BUS_WRITE_CONFIG_t ebu_bus_write_config; /**< Write configuration and access parameters structure */
} XMC_EBU_REGION_WRITE_CONFIG_t;

/**
 * EBU region initialization with read and write configurations <br>
 *
 * The structure presents a convenient way to set/obtain the read, read timing,
 * write and write timing configurations for a region of EBU. The
 * XMC_EBU_ConfigureRegion() can be used to populate the structure with the
 * region read and write configuration values of EBU.
 */  
typedef struct XMC_EBU_REGION
{
  XMC_EBU_REGION_READ_CONFIG_t read_config;
  XMC_EBU_REGION_WRITE_CONFIG_t write_config;
} XMC_EBU_REGION_t;

/**
 * EBU global configurations <br>
 *
 * The structure presents a convenient way to set/obtain the global configurations
 * of the EBU like clock, mode and GPIO mode. The XMC_EBU_Init() can be
 * used to populate the structure with the region read and write configuration
 * values of EBU.
 */  
typedef struct XMC_EBU_CONFIG
{
  XMC_EBU_CLK_CONFIG_t ebu_clk_config;               /**< Clock configuration structure */
  XMC_EBU_MODE_CONFIG_t ebu_mode_config;             /**< Mode configuration structure */
  XMC_EBU_FREE_PINS_TO_GPIO_t ebu_free_pins_to_gpio; /**< Free allocated EBU ports for GPIO */
} XMC_EBU_CONFIG_t;
  
/**
 * External Bus Unit (EBU) device structure <br>
 *
 * The structure represents a collection of all hardware registers
 * used to configure the EBU peripheral on the XMC4500 microcontroller.
 * The registers can be accessed with ::XMC_EBU.
 */
typedef struct
{
  __IO uint32_t  CLC;
  __IO uint32_t  MODCON;
  __I  uint32_t  ID;
  __IO uint32_t  USERCON;
  __I  uint32_t  RESERVED0[2];
  __IO uint32_t  ADDRSEL[4];
  struct
  {
    __IO uint32_t  RDCON;
    __IO uint32_t  RDAPR;
    __IO uint32_t  WRCON;
    __IO uint32_t  WRAPR;
  } BUS[4];
  __IO uint32_t  SDRMCON;
  __IO uint32_t  SDRMOD;
  __IO uint32_t  SDRMREF;
  __I  uint32_t  SDRSTAT;
} XMC_EBU_t;

/**********************************************************************************************************************
 * API PROTOTYPES
**********************************************************************************************************************/
 
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address
 * @param config Constant pointer to a constant ::XMC_EBU_CONFIG_t structure containing the
 *               clock mode and clock configuration.
 * @return XMC_EBU_STATUS_t Always returns XMC_EBU_STATUS_OK (Only register assignment statements)
 *
 * \par<b>Description: </b><br>
 * Initialize the EBU peripheral <br>
 *
 * \par
 * The function enables the EBU peripheral, configures time values for clock mode, div2
 * clock mode, mode configuration, SDRAM tristate, external clock, arbitration, timeout
 * control, ALE mode and configuration to free up the allocated EBU ports for GPIO
 * functionality (if required).
 */
XMC_EBU_STATUS_t XMC_EBU_Init(XMC_EBU_t *const ebu, const XMC_EBU_CONFIG_t *const config);

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address
 * @param config Constant pointer to a constant ::XMC_EBU_SDRAM_CONFIG_t structure containing
 *               the SDRAM configuration, operation mode configuration and right refresh
 *               parameters
 * @return None
 *
 * \par<b>Description: </b><br>
 * Configures the SDRAM <br>
 *
 * \par
 * The function enables the SDRAM, sets SDRAM configuration parameters such as operation
 * mode and refresh parameters. Please see  ::XMC_EBU_SDRAM_CONFIG_t for more information.
 */
void XMC_EBU_ConfigureSdram(XMC_EBU_t *const ebu, const XMC_EBU_SDRAM_CONFIG_t *const config);

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address
 * @param region Constant pointer to a constant ::XMC_EBU_REGION_t structure containing the
 *               read, read timing, write and write timing configurations for a region of
 *               EBU
 * @return None
 *
 * \par<b>Description: </b><br>
 * Configures the SDRAM <br>
 *
 * \par
 * The function configures the EBU region read, read timing, write and write timing parameter
 * configuration. It also configures the region registers for read and write accesses. Please
 * see ::XMC_EBU_REGION_t for more information.
 *
 */
void XMC_EBU_ConfigureRegion(XMC_EBU_t *const ebu, const XMC_EBU_REGION_t *const region);

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable EBU peripheral <br>
 *
 * \par
 * The function de-asserts the peripheral reset. The peripheral needs to be initialized.
 * It also enables the control of the EBU.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_EBU_Disable(), XMC_SCU_RESET_AssertPeripheralReset()
 */
__STATIC_INLINE void XMC_EBU_Enable(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_Enable: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_EBU);
  ebu->CLC &= ~EBU_CLC_DISR_Msk;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable EBU peripheral <br>
 *
 * \par
 * The function asserts the peripheral reset. It also disables the control of the EBU.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_EBU_Enable(), XMC_SCU_RESET_DeassertPeripheralReset()
 */
__STATIC_INLINE void XMC_EBU_Disable(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_Disable: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->CLC |= EBU_CLC_DISR_Msk;
  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_EBU);
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address
 * @param clk_status Constant structure ::XMC_EBU_CLK_STATUS_t, containing the 
 *                   disable status, clock mode status, DIV2 clock mode status
 *                   and clock divide ratio
 * @return Status Returns clock status, disable status, clock mode status, DIV2 clock
 *                mode status and clock divide ratio
 *
 * \par<b>Description: </b><br>
 * Gets the clock status of EBU peripheral <br>
 *
 * \par
 * The function returns the clock staus of the EBU peripheral. The return value will
 * indicate the following parameters: <br>
 * 1) Is EBU disabled?                <br>
 * 2) Clocking mode                   <br>
 * 3) DIV2 clocking mode              <br>
 * 4) Clock divide ratio              <br>
 *
 * \par<b>Related APIs:</b><br>
 * XMC_EBU_Enable(), XMC_EBU_Disable() and XMC_EBU_CLKDivideRatio()
 */   
__STATIC_INLINE uint32_t XMC_EBU_GetCLKStatus(XMC_EBU_t *const ebu, const XMC_EBU_CLK_STATUS_t clk_status)
{
  XMC_ASSERT("XMC_EBU_GetCLKStatus: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  return (uint32_t)(ebu->CLC & clk_status);
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address
 * @param clock_divide_ratio Structure ::XMC_EBU_CLOCK_DIVIDE_RATIO_t, containing the 
 *                           clock division factors of 1, 2, 3 and 4 respectively 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the clock divide ratio for EBU peripheral <br>
 *
 * \par
 * The function sets the CLC.EBUDIV bit-field to configure the clock divide ratio
 * value (input clock divide by factor).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_EBU_Enable(), XMC_EBU_Disable() and XMC_EBU_GetCLKStatus()
 */
__STATIC_INLINE void XMC_EBU_CLKDivideRatio(XMC_EBU_t *ebu, XMC_EBU_CLOCK_DIVIDE_RATIO_t clock_divide_ratio)
{
  XMC_ASSERT("XMC_EBU_CLKDivideRatio: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->CLC |= ((clock_divide_ratio << EBU_CLC_EBUDIV_Pos) & EBU_CLC_EBUDIV_Msk);
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the SDRAM self refresh exit (Power up) <br>
 *
 * \par
 * The function sets the SDRMREF.SELFREX bit to issue the self refresh command to
 * all the SDRAM units. This ensures that the SDRAM units come out of  the power down
 * mode. The function also resets the bit SDRMREF.SELFRENST(Self refresh entry status).
 */
__STATIC_INLINE void XMC_EBU_SdramSetSelfRefreshExit(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_SdramSetSelfRefreshExit: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->SDRMREF |= EBU_SDRMREF_SELFREX_Msk;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the SDRAM self refresh entry <br>
 *
 * \par
 * The function sets the SDRMREF.SELFREN bit-field to issue the self refresh command
 * to all the SDRAM units. This ensures that the SDRAM units enter the power down mode
 * after pre-charge. The function also resets the bit SDRMREF.SELFREXST(Self refresh
 * exit status).
 */
__STATIC_INLINE void XMC_EBU_SdramSetSelfRefreshEntry(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_SdramSetSelfRefreshEntry: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->SDRMREF |= EBU_SDRMREF_SELFREN_Msk;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Resets the SDRAM self refresh exit (Power up) <br>
 *
 * \par
 * The function resets the SDRMREF.SELFREX bit-field to stop issuing the self
 * refresh command to all the SDRAM units connected to the bus. This ensures that
 * the SDRAM units don't come out of the power down mode.
 *
 */
__STATIC_INLINE void XMC_EBU_SdramResetSelfRefreshExit(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_SdramResetSelfRefreshExit: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->SDRMREF &= ~EBU_SDRMREF_SELFREX_Msk;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Resets the SDRAM self refresh entry <br>
 *
 * \par
 * The function resets the SDRMREF.SELFREN bit-field to stop issuing the self
 * refresh command to all the SDRAM units.  This ensures that the SDRAM units
 * don't go into the power down mode after the pre-charge is all done.
 *
 */
__STATIC_INLINE void XMC_EBU_SdramResetSelfRefreshEntry(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_SdramResetSelfRefreshEntry: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->SDRMREF &= ~EBU_SDRMREF_SELFREN_Msk;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the SDRAM auto refresh on self refresh exit <br>
 *
 * \par
 * The function sets the SDRMREF.ARFSH bit-field to enable an auto refresh cycle
 * on existing self refresh before the self refresh exit delay.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_SdramDisableAutoRefreshSelfRefreshExit()
 */
__STATIC_INLINE void XMC_EBU_SdramEnableAutoRefreshSelfRefreshExit(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_SdramEnableAutoRefreshSelfRefreshExit: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->SDRMREF |= EBU_SDRMREF_ARFSH_Msk;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Resets the SDRAM auto refresh on self refresh exit <br>
 *
 * \par
 * The function resets the SDRMREF.ARFSH bit to disable an auto refresh cycle
 * on existing self refresh before the self refresh exit delay. No refresh will be
 * performed.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_SdramEnableAutoRefreshSelfRefreshExit()
 */
__STATIC_INLINE void XMC_EBU_SdramDisableAutoRefreshSelfRefreshExit(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_SdramDisableAutoRefreshSelfRefreshExit: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->SDRMREF &= ~EBU_SDRMREF_ARFSH_Msk;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the SDRAM automatic self refresh <br>
 *
 * \par
 * The function sets the SDRMREF.AUTOSELFR bit-field. When set, the memory controller
 * automatically issues the self refresh entry command to all SDRAM units 
 * devices when it gives up control of the external bus. It will also automatically
 * issue the self refresh exit command when it regains control of the bus.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_SdramDisableAutomaticSelfRefresh()
 */  
__STATIC_INLINE void XMC_EBU_SdramEnableAutomaticSelfRefresh(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_SdramEnableAutomaticSelfRefresh: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->SDRMREF |= EBU_SDRMREF_AUTOSELFR_Msk;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Resets the SDRAM automatic self refresh <br>
 *
 * \par
 * The function resets the SDRMREF.AUTOSELFR bit-field. When reset, the memory controller
 * doesn't issue the self refresh entry command when it gives up control of the external
 * bus. It will also not issue the self refresh exit command when it regains control of
 * the bus.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_SdramEnableAutomaticSelfRefresh()
 */  
__STATIC_INLINE void XMC_EBU_SdramDisableAutomaticSelfRefresh(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_SdramDisableAutomaticSelfRefresh: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->SDRMREF &= ~EBU_SDRMREF_AUTOSELFR_Msk;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return bool Returns if the arbitration mode is selected or not
 *
 * \par<b>Description: </b><br>
 * Check if arbitration mode of EBU peripheral is selected
 *
 * \par
 * The bit field ARBMODE of MODCON indicates the selected arbitration mode of the
 * EBU. The follwing are the supported arbitration modes: <br>
 * 1) Arbiter Mode arbitration mode <br>
 * 2) Participant arbitration mode  <br>
 * 3) Sole Master arbitration mode  <br>
 *
 * If any of the above modes are selected, the function returns "true". It returns
 * false otherwise.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_Init() \n\n\n
 */
__STATIC_INLINE bool XMC_EBU_IsBusAribitrationSelected(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_IsBusAribitrationSelected: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  return (bool)((ebu->MODCON & EBU_MODCON_ARBMODE_Msk) != 0);
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address
 * @param ebu_addr_select_en Choose between a memory region enable or an alternate region enable
 * @param ebu_region_n A valid region number for which enable and protection settings
 *                     need to be configured
 * @return None
 *
 * \par<b>Description: </b><br>
 * Controls the enable and protection settings of a region <br>
 *
 * \par
 * The function controls the enable and protection settings of a memory or alternate
 * region. It configures the memory region enable, alternate region enable and the memory
 * region's write protection. The bit-fields ADDRSEL.REGENAB, ADDRSEL.ALTENAB and
 * ADDRSEL.WPROT are configured.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_AddressSelectDisable()
 */   
__STATIC_INLINE void XMC_EBU_AddressSelectEnable(XMC_EBU_t *const ebu,
                                                 uint32_t ebu_addr_select_en,
                                                 const uint32_t ebu_region_n)
{
  XMC_ASSERT("XMC_EBU_AddressSelectEnable: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->ADDRSEL[ebu_region_n] |= ebu_addr_select_en;
  while (!XMC_EBU_IsBusAribitrationSelected(ebu));
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @param ebu_addr_select_dis Choose between a memory region disable or an alternate
 *                            region disable
 * @param ebu_region_n A valid region number for which disable and protection
 *                     settings configured
 * @return None
 *
 * \par<b>Description: </b><br>
 * Controls the disable and protection settings of a region <br>
 *
 * \par
 * The function controls the disable and protection settings of a memory or alternate
 * region. It configures the memory region disable, alternate region disable and the
 * memory region write protect disable for write accesses. The bits ADDRSEL.REGENAB,
 * ADDRSEL.ALTENAB and ADDRSEL.WPROT are configured.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_AddressSelectEnable()
 */  
__STATIC_INLINE void XMC_EBU_AddressSelectDisable(XMC_EBU_t *const ebu,
                                                  uint32_t ebu_addr_select_dis,
                                                  const uint32_t ebu_region_n)
{
  XMC_ASSERT("XMC_EBU_AddressSelectDisable: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  ebu->ADDRSEL[ebu_region_n] &= ~ebu_addr_select_dis;
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @param ebu_buswcon_status Enumeration of type ::XMC_EBU_BUSWCON_SELECT_t, representing
 *                           values for non-array access and device addressing modes.
 * @param ebu_region_n A valid region number for which status pertaining to WRITE is required
 * @return Status Status of non-array access and device addressing mode
 *
 * \par<b>Description: </b><br>
 * Gets WRITE specific status for a region <br>
 *
 * \par
 * The function gets status of the various WRITE specific settings for a region. Status for
 * non-array access enable and device addressing mode are obtained. The status bits of the
 * BUSWCON register are returned.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_ConfigureRegion()
 */  
__STATIC_INLINE uint32_t XMC_EBU_GetBusWriteConfStatus(XMC_EBU_t *const ebu,
                                                       const XMC_EBU_BUSWCON_SELECT_t ebu_buswcon_status,
                                                       const uint32_t ebu_region_n)
{
  XMC_ASSERT("XMC_EBU_GetBusWriteConfStatus: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  return (ebu->BUS[ebu_region_n].WRCON & ebu_buswcon_status);
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @return Status SDRAM error or busy states
 *
 * \par<b>Description: </b><br>
 * Gets SDRAM error or busy states <br>
 * 
 * \par
 * The function gets SDRAM read error, refresh error and busy states. The bit-fields of SDRSTAT
 * indicate the various states. REFERR reflects a failed previous refresh request collision
 * with a new request. SDRMBUSY indicates the status of power-up initialization sequence. It
 * indicates if it is running or not running. SDERR indicates if the SDRAM controller has
 * detected an error when returning the read data.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_ConfigureSdram()
 */  
__STATIC_INLINE uint32_t XMC_EBU_SdramGetStatus(XMC_EBU_t *const ebu)
{
  XMC_ASSERT("XMC_EBU_SdramGetStatus: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  return (uint32_t)(ebu->SDRSTAT);
}

/**
 * @param ebu Constant pointer to ::XMC_EBU_t, pointing to the EBU base address 
 * @param sdram_rfrsh_status Constant enum of type ::XMC_EBU_SDRAM_RFRSH_STATUS_t
 * @return Status Status of self refresh entry and exit command issue
 *
 * \par<b>Description: </b><br>
 * Gets SDRAM refresh status <br>
 * 
 * \par
 * The function gets SDRAM refresh status for self refresh entry/exit command successful issue.
 * The bit-fields of SDRMREF indicate various states:                <br>
 * SELFRENST reflects successful issue of self refresh entry command <br>
 * SELFREXST reflects successful issue of self refresh exit command  <br>
 *
 * \par<b>Related APIs: </b><br>
 * XMC_EBU_SdramResetSelfRefreshEntry(), XMC_EBU_SdramResetSelfRefreshExit()
 */  
__STATIC_INLINE uint32_t XMC_EBU_SdramGetRefreshStatus(XMC_EBU_t *const ebu,
                                                       const XMC_EBU_SDRAM_RFRSH_STATUS_t sdram_rfrsh_status)
{
  XMC_ASSERT("XMC_EBU_SdramGetRefreshStatus: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  return (uint32_t)(ebu->SDRMREF & sdram_rfrsh_status);
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

#endif /* defined (EBU) */

#endif /* XMC_EBU_H */
