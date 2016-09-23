/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _FSL_SDRAMC_H_
#define _FSL_SDRAMC_H_

#include "fsl_common.h"

/*!
 * @addtogroup sdramc
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief SDRAMC driver version 2.0.0. */
#define FSL_SDRAMC_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*! @brief SDRAM controller auto-refresh timing. */
typedef enum _sdramc_refresh_time
{
    kSDRAMC_RefreshThreeClocks = 0x0U, /*!< The refresh timing with three bus clocks. */
    kSDRAMC_RefreshSixClocks,          /*!< The refresh timing with six bus clocks. */
    kSDRAMC_RefreshNineClocks          /*!< The refresh timing with nine bus clocks. */
} sdramc_refresh_time_t;

/*!
 * @brief Setting latency for SDRAM controller timing specifications.
 *
 * The latency setting will affects the following SDRAM timing specifications:
 *       - trcd: SRAS assertion to SCAS assertion \n
 *       - tcasl: SCAS assertion to data out \n
 *       - tras: ACTV command to Precharge command \n
 *       - trp: Precharge command to ACTV command \n
 *       - trwl, trdl: Last data input to Precharge command \n
 *       - tep: Last data out to Precharge command \n
 * the details of the latency setting and timing specifications are shown on the following table list: \n
 *   latency      trcd:          tcasl         tras           trp        trwl,trdl        tep   \n
 *    0       1 bus clock    1 bus clock   2 bus clocks   1 bus clock   1 bus clock   1 bus clock  \n
 *    1       2 bus clock    2 bus clock   4 bus clocks   2 bus clock   1 bus clock   1 bus clock  \n
 *    2       3 bus clock    3 bus clock   6 bus clocks   3 bus clock   1 bus clock   1 bus clock  \n
 *    3       3 bus clock    3 bus clock   6 bus clocks   3 bus clock   1 bus clock   1 bus clock  \n
 */
typedef enum _sdramc_latency
{
    kSDRAMC_LatencyZero = 0x0U, /*!< Latency  0. */
    kSDRAMC_LatencyOne,         /*!< Latency  1. */
    kSDRAMC_LatencyTwo,         /*!< Latency  2. */
    kSDRAMC_LatencyThree,       /*!< Latency  3. */
} sdramc_latency_t;

/*! @brief SDRAM controller command bit location. */
typedef enum _sdramc_command_bit_location
{
    kSDRAMC_Commandbit17 = 0x0U, /*!< Command bit location is bit 17. */
    kSDRAMC_Commandbit18,        /*!< Command bit location is bit 18. */
    kSDRAMC_Commandbit19,        /*!< Command bit location is bit 19. */
    kSDRAMC_Commandbit20,        /*!< Command bit location is bit 20. */
    kSDRAMC_Commandbit21,        /*!< Command bit location is bit 21. */
    kSDRAMC_Commandbit22,        /*!< Command bit location is bit 22. */
    kSDRAMC_Commandbit23,        /*!< Command bit location is bit 23. */
    kSDRAMC_Commandbit24         /*!< Command bit location is bit 24. */
} sdramc_command_bit_location_t;

/*! @brief SDRAM controller command. */
typedef enum _sdramc_command
{
    kSDRAMC_ImrsCommand = 0x0U,        /*!< Initiate MRS command. */
    kSDRAMC_PrechargeCommand,          /*!< Initiate precharge command.  */
    kSDRAMC_SelfrefreshEnterCommand,   /*!< Enter self-refresh command. */
    kSDRAMC_SelfrefreshExitCommand,    /*!< Exit self-refresh command. */
    kSDRAMC_AutoRefreshEnableCommand,  /*!< Enable Auto refresh command. */
    kSDRAMC_AutoRefreshDisableCommand, /*!< Disable Auto refresh command.  */
} sdramc_command_t;

/*! @brief SDRAM port size. */
typedef enum _sdramc_port_size
{
    kSDRAMC_PortSize32Bit = 0x0U, /*!< 32-Bit port size. */
    kSDRAMC_PortSize8Bit,         /*!< 8-Bit port size. */
    kSDRAMC_PortSize16Bit         /*!< 16-Bit port size. */
} sdramc_port_size_t;

/*! @brief SDRAM controller block selection. */
typedef enum _sdramc_block_selection
{
    kSDRAMC_Block0 = 0x0U, /*!< Select SDRAM block 0. */
    kSDRAMC_Block1,        /*!< Select SDRAM block 1. */
} sdramc_block_selection_t;

/*! @brief SDRAM controller block control configuration structure. */
typedef struct _sdramc_blockctl_config
{
    sdramc_block_selection_t block;         /*!< The block number. */
    sdramc_port_size_t portSize;            /*!< The port size of the associated SDRAM block. */
    sdramc_command_bit_location_t location; /*!< The command bit location. */
    sdramc_latency_t latency;               /*!< The latency for some timing specifications. */
    uint32_t address;                       /*!< The base address of the SDRAM block. */
    uint32_t addressMask;                   /*!< The base address mask of the SDRAM block. */
} sdramc_blockctl_config_t;

/*! @brief SDRAM controller refresh timing configuration structure. */
typedef struct _sdramc_refresh_config
{
    sdramc_refresh_time_t refreshTime; /*!< Trc:The number of bus clocks inserted
                                            between a REF and next ACTIVE command. */
    uint32_t sdramRefreshRow;          /*!< The SDRAM refresh time each row: ns/row. */
    uint32_t busClock_Hz;              /*!< The bus clock for SDRAMC. */
} sdramc_refresh_config_t;

/*!
 * @brief SDRAM controller configuration structure.
 *
 * Defines a configure structure and uses the SDRAMC_Configure() function to make necessary
 * initializations.
 */
typedef struct _sdramc_config_t
{
    sdramc_refresh_config_t *refreshConfig; /*!< Refresh timing configure structure pointer. */
    sdramc_blockctl_config_t *blockConfig;  /*!< Block configure structure pointer. If both SDRAM
                                                 blocks are used, use the two continuous blockConfig. */
    uint8_t numBlockConfig;                 /*!< SDRAM block numbers for configuration. */
} sdramc_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name SDRAM Controller Initialization and De-initialization
 * @{
 */

/*!
 * @brief Initializes the SDRAM controller.
 * This function ungates the SDRAM controller clock and initializes the SDRAM controller.
 * This function must be called before calling any other SDRAM controller driver functions.
 * Example
   @code
    sdramc_refresh_config_t refreshConfig;
    sdramc_blockctl_config_t blockConfig;
    sdramc_config_t config;

    refreshConfig.refreshTime  = kSDRAM_RefreshThreeClocks;
    refreshConfig.sdramRefreshRow = 15625;
    refreshConfig.busClock = 60000000;

    blockConfig.block = kSDRAMC_Block0;
    blockConfig.portSize = kSDRAMC_PortSize16Bit;
    blockConfig.location = kSDRAMC_Commandbit19;
    blockConfig.latency = kSDRAMC_RefreshThreeClocks;
    blockConfig.address = SDRAM_START_ADDRESS;
    blockConfig.addressMask = 0x7c0000;

    config.refreshConfig = &refreshConfig,
    config.blockConfig = &blockConfig,
    config.totalBlocks = 1;

    SDRAMC_Init(SDRAM, &config);
   @endcode
 *
 * @param base SDRAM controller peripheral base address.
 * @param configure The SDRAM configuration structure pointer.
 */
void SDRAMC_Init(SDRAM_Type *base, sdramc_config_t *configure);

/*!
 * @brief Deinitializes the SDRAM controller module and gates the clock.
 * This function gates the SDRAM controller clock. As a result, the SDRAM
 * controller module doesn't work after calling this function.
 *
 * @param base SDRAM controller peripheral base address.
 */
void SDRAMC_Deinit(SDRAM_Type *base);

/* @} */

/*!
 * @name SDRAM Controller Basic Operation
 * @{
 */

/*!
 * @brief Sends the SDRAM command.
 * This function sends the command to SDRAM. There are precharge command, initialize MRS command,
 * auto-refresh enable/disable command, and self-refresh enter/exit commands.
 * Note the self-refresh enter/exit commands are all blocks setting and "block"
 * are ignored. Ensure to set the right "block" when send other commands.
 *
 * @param base SDRAM controller peripheral base address.
 * @param block The block selection.
 * @param command The SDRAM command, see "sdramc_command_t".
 *        kSDRAMC_ImrsCommand -  Initialize MRS command   \n
 *        kSDRAMC_PrechargeCommand  - Initialize precharge command   \n
 *        kSDRAMC_SelfrefreshEnterCommand - Enter self-refresh command \n
 *        kSDRAMC_SelfrefreshExitCommand  -  Exit self-refresh command \n
 *        kSDRAMC_AutoRefreshEnableCommand  - Enable auto refresh command \n
 *        kSDRAMC_AutoRefreshDisableCommand  - Disable auto refresh command
 * @return Command execution status.
 * All commands except the "initialize MRS command" and "precharge command"
 * return kStatus_Success directly.
 * For "initialize MRS command" and "precharge command"
 * return kStatus_Success when the command success else return kStatus_Fail.
 */
status_t SDRAMC_SendCommand(SDRAM_Type *base, sdramc_block_selection_t block, sdramc_command_t command);

/*!
 * @brief Enables/disables the write protection.
 *
 * @param base SDRAM peripheral base address.
 * @param block The block which is selected.
 * @param enable True enable write protection, false disable write protection.
 */
static inline void SDRAMC_EnableWriteProtect(SDRAM_Type *base, sdramc_block_selection_t block, bool enable)
{
    if (enable)
    {
        base->BLOCK[block].CM |= SDRAM_CM_WP_MASK;
    }
    else
    {
        base->BLOCK[block].CM &= ~SDRAM_CM_WP_MASK;
    }
}

/*!
 * @brief Enables/disables the operation valid.
 *
 * @param base SDRAM peripheral base address.
 * @param block The block which is selected.
 * @param enable True enable the operation valid, false disable the operation valid.
 */
static inline void SDRAMC_EnableOperateValid(SDRAM_Type *base, sdramc_block_selection_t block, bool enable)
{
    if (enable)
    {
        base->BLOCK[block].CM |= SDRAM_CM_V_MASK;
    }
    else
    {
        base->BLOCK[block].CM &= ~SDRAM_CM_V_MASK;
    }
}

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_SDRAMC_H_*/
