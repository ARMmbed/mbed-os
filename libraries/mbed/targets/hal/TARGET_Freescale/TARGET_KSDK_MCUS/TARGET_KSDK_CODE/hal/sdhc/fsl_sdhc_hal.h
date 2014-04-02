/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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
#ifndef __FSL_SDHC_HAL_H__
#define __FSL_SDHC_HAL_H__

#include <assert.h>
#include <stdbool.h>
#include "fsl_device_registers.h"

/*! @addtogroup sdhc_hal */
/*! @{ */

/* PRSSTA */
#define SDHC_HAL_DAT0_LEVEL             (BM_SDHC_PRSSTAT_DLSL & (1 << 24))

/* XFERTYP */
#define SDHC_HAL_MAX_BLOCK_COUNT        (BS_SDHC_BLKATTR_BLKCNT - 1)
#define SDHC_HAL_ENABLE_DMA             BM_SDHC_XFERTYP_DMAEN
#define SDHC_HAL_DISABLE_DMA            (0U)

#define SDHC_HAL_CMD_TYPE_SUSPEND       (BF_SDHC_XFERTYP_CMDTYP(1))
#define SDHC_HAL_CMD_TYPE_RESUME        (BF_SDHC_XFERTYP_CMDTYP(2))
#define SDHC_HAL_CMD_TYPE_ABORT         (BF_SDHC_XFERTYP_CMDTYP(3))

#define SDHC_HAL_ENABLE_BLOCK_COUNT     BM_SDHC_XFERTYP_BCEN
#define SDHC_HAL_DISABLE_BLOCK_COUNT    (0U)

#define SDHC_HAL_ENABLE_AUTO_CMD12      BM_SDHC_XFERTYP_AC12EN
#define SDHC_HAL_DISABLE_AUTO_CMD12     (0U)

#define SDHC_HAL_ENABLE_DATA_READ       BM_SDHC_XFERTYP_DTDSEL
#define SDHC_HAL_DISABLE_AUTO_CMD12     (0U)

#define SDHC_HAL_MULTIPLE_BLOCK         BM_SDHC_XFERTYP_MSBSEL
#define SDHC_HAL_SINGLE_BLOCK           (0U)

#define SDHC_HAL_NO_RESPONE             (0U)
#define SDHC_HAL_RESP_LEN_136           ((0x1 << BP_SDHC_XFERTYP_RSPTYP) & BM_SDHC_XFERTYP_RSPTYP)
#define SDHC_HAL_RESP_LEN_48            ((0x2 << BP_SDHC_XFERTYP_RSPTYP) & BM_SDHC_XFERTYP_RSPTYP)
#define SDHC_HAL_RESP_LEN_48_BC         ((0x3 << BP_SDHC_XFERTYP_RSPTYP) & BM_SDHC_XFERTYP_RSPTYP)

#define SDHC_HAL_ENABLE_CRC_CHECK       BM_SDHC_XFERTYP_CCCEN
#define SDHC_HAL_DISABLE_CRC_CHECK      (0U)

#define SDHC_HAL_ENABLE_INDEX_CHECK     BM_SDHC_XFERTYP_CICEN
#define SDHC_HAL_DISABLE_INDEX_CHECK    (0U)

#define SDHC_HAL_DATA_PRESENT           BM_SDHC_XFERTYP_DPSEL
#define SDHC_HAL_NO_DATA_PRESENT        (0U)

/* SYSCTL */
#define SDHC_HAL_MAX_DVS                (16U)
#define SDHC_HAL_INITIAL_DVS            (1U)            /* initial value of divisor to calculate clock rate */
#define SDHC_HAL_INITIAL_CLKFS          (2U)            /* initial value of clock selector to calculate clock rate */
#define SDHC_HAL_NEXT_DVS(x)            do { ((x) += 1); } while(0)
#define SDHC_HAL_PREV_DVS(x)            do { ((x) -= 1); } while(0)
#define SDHC_HAL_MAX_CLKFS              (256U)
#define SDHC_HAL_NEXT_CLKFS(x)          do { ((x) <<= 1); } while(0)
#define SDHC_HAL_PREV_CLKFS(x)          do { ((x) >>= 1); } while(0)

/* IRQSTAT */
#define SDHC_HAL_CMD_COMPLETE_INT       BM_SDHC_IRQSTAT_CC
#define SDHC_HAL_TRANS_COMPLETE_INT     BM_SDHC_IRQSTAT_TC
#define SDHC_HAL_BLOCK_GAP_EVENT_INT    BM_SDHC_IRQSTAT_BGE
#define SDHC_HAL_DMA_INT                BM_SDHC_IRQSTAT_DMAE
#define SDHC_HAL_BUF_WRITE_READY_INT    BM_SDHC_IRQSTAT_BWR
#define SDHC_HAL_BUF_READ_READY_INT     BM_SDHC_IRQSTAT_BRR
#define SDHC_HAL_CARD_INSERTION_INT     BM_SDHC_IRQSTAT_CINS
#define SDHC_HAL_CARD_REMOVAL_INT       BM_SDHC_IRQSTAT_CRM
#define SDHC_HAL_CARD_INT               BM_SDHC_IRQSTAT_CINT
#define SDHC_HAL_CMD_TIMEOUT_ERR_INT    BM_SDHC_IRQSTAT_CTOE
#define SDHC_HAL_CMD_CRC_ERR_INT        BM_SDHC_IRQSTAT_CCE
#define SDHC_HAL_CMD_END_BIT_ERR_INT    BM_SDHC_IRQSTAT_CEBE
#define SDHC_HAL_CMD_INDEX_ERR_INT      BM_SDHC_IRQSTAT_CIE
#define SDHC_HAL_DATA_TIMEOUT_ERR_INT   BM_SDHC_IRQSTAT_DTOE
#define SDHC_HAL_DATA_CRC_ERR_INT       BM_SDHC_IRQSTAT_CCE
#define SDHC_HAL_DATA_END_BIT_ERR_INT   BM_SDHC_IRQSTAT_DEBE
#define SDHC_HAL_AUTO_CMD12_ERR_INT     BM_SDHC_IRQSTAT_AC12E
#define SDHC_HAL_DMA_ERR_INT            BM_SDHC_IRQSTAT_DMAE

/* AC12ERR */
#define SDHC_HAL_ACMD12_NOT_EXEC_ERR    BM_SDHC_AC12ERR_AC12NE
#define SDHC_HAL_ACMD12_TIMEOUT_ERR     BM_SDHC_AC12ERR_AC12TOE
#define SDHC_HAL_ACMD12_END_BIT_ERR     BM_SDHC_AC12ERR_AC12EBE
#define SDHC_HAL_ACMD12_CRC_ERR         BM_SDHC_AC12ERR_AC12CE
#define SDHC_HAL_ACMD12_INDEX_ERR       BM_SDHC_AC12ERR_AC12IE
#define SDHC_HAL_ACMD12_NOT_ISSUE_ERR   BM_SDHC_AC12ERR_CNIBAC12E

/* HTCAPBLT */
#define SDHC_HAL_SUPPORT_ADMA           BM_SDHC_HTCAPBLT_ADMAS
#define SDHC_HAL_SUPPORT_HIGHSPEED     BM_SDHC_HTCAPBLT_HSS
#define SDHC_HAL_SUPPORT_DMA            BM_SDHC_HTCAPBLT_DMAS
#define SDHC_HAL_SUPPORT_SUSPEND_RESUME BM_SDHC_HTCAPBLT_SRS
#define SDHC_HAL_SUPPORT_3_3_V          BM_SDHC_HTCAPBLT_VS33
#define SDHC_HAL_SUPPORT_3_0_V          BM_SDHC_HTCAPBLT_VS30
#define SDHC_HAL_SUPPORT_1_8_V          BM_SDHC_HTCAPBLT_VS18

/* FEVT */
#define SDHC_HAL_ACMD12_NOT_EXEC_ERR_EVENT  BM_SDHC_FEVT_AC12NE
#define SDHC_HAL_ACMD12_TIMEOUT_ERR_EVENT   BM_SDHC_FEVT_AC12TOE
#define SDHC_HAL_ACMD12_CRC_ERR_EVENT       BM_SDHC_FEVT_AC12CE
#define SDHC_HAL_ACMD12_END_BIT_ERR_EVENT   BM_SDHC_FEVT_AC12EBE
#define SDHC_HAL_ACMD12_INDEX_ERR_EVENT     BM_SDHC_FEVT_AC12IE
#define SDHC_HAL_ACMD12_NOT_ISSUE_ERR_EVENT BM_SDHC_FEVT_CNIBAC12E
#define SDHC_HAL_CMD_TIMEOUT_ERR_EVENT      BM_SDHC_FEVT_CTOE
#define SDHC_HAL_CMD_CRC_ERR_EVENT          BM_SDHC_FEVT_CCE
#define SDHC_HAL_CMD_END_BIT_ERR_EVENT      BM_SDHC_FEVT_CEBE
#define SDHC_HAL_CMD_INDEX_ERR_EVENT        BM_SDHC_FEVT_CIE
#define SDHC_HAL_DATA_TIMEOUT_ERR_EVENT     BM_SDHC_FEVT_DTOE
#define SDHC_HAL_DATA_CRC_ERR_EVENT         BM_SDHC_FEVT_DCE
#define SDHC_HAL_DATA_END_BIT_ERR_EVENT     BM_SDHC_FEVT_DEBE
#define SDHC_HAL_ACMD12_ERR_EVENT           BM_SDHC_FEVT_AC12E
#define SDHC_HAL_CARD_INT_EVENT             BM_SDHC_FEVT_CINT
#define SDHC_HAL_DMA_ERROR_EVENT            BM_SDHC_FEVT_DMAE

/* MMCBOOT */
typedef enum _sdhc_hal_mmcboot
{
    kSDHC_HAL_MMCBOOT_Normal = 0,
    kSDHC_HAL_MMCBOOT_Alter = 1,
} sdhc_hal_mmcboot_t;

/* PROCTL */
typedef enum _sdhc_hal_led
{
    kSDHC_HAL_LED_Off = 0,
    kSDHC_HAL_LED_On = 1,
} sdhc_hal_led_t;

typedef enum _sdhc_hal_dtw
{
    kSDHC_HAL_DTW_1Bit = 0,
    kSDHC_HAL_DTW_4Bit = 1,
    kSDHC_HAL_DTW_8Bit = 2,
} sdhc_hal_dtw_t;

typedef enum _sdhc_hal_endian
{
    kSDHC_HAL_ENDIAN_Big = 0,
    kSDHC_HAL_ENDIAN_HalfWordBig = 1,
    kSDHC_HAL_ENDIAN_Little = 2,
} sdhc_hal_endian_t;

typedef enum _sdhc_hal_dma_mode
{
    kSDHC_HAL_DMA_NoOrSimple = 0,
    kSDHC_HAL_DMA_Adma1 = 1,
    kSDHC_HAL_DMA_Adma2 = 2,
} sdhc_hal_dma_mode_t;

#define SDHC_HAL_RST_TYPE_ALL               BM_SDHC_SYSCTL_RSTA
#define SDHC_HAL_RST_TYPE_CMD               BM_SDHC_SYSCTL_RSTC
#define SDHC_HAL_RST_TYPE_DATA              BM_SDHC_SYSCTL_RSTD

#define SDHC_HAL_MAX_BLKLEN_512B            (0U)
#define SDHC_HAL_MAX_BLKLEN_1024B           (1U)
#define SDHC_HAL_MAX_BLKLEN_2048B           (2U)
#define SDHC_HAL_MAX_BLKLEN_4096B           (3U)

/*************************************************************************************************
 * API
 ************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! @name SDHC HAL FUNCTION */
/*@{ */

/*!
 * @brief Checks whether the given instance is valid.
 *
 * @param instance SDHC instance ID
 * @return true if valid
 */
static inline bool sdhc_hal_is_valid_instance(uint8_t instance)
{
    return (instance < HW_SDHC_INSTANCE_COUNT);
}

/*!
 * @brief Configures the DMA address.
 *
 * @param instance SDHC instance ID
 * @param address the DMA address
 */
static inline void sdhc_hal_set_dma_addr(uint8_t instance, uint32_t address)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    HW_SDHC_DSADDR_WR(BF_SDHC_DSADDR_DSADDR(address));
}

/*!
 * @brief Gets the DMA address.
 *
 * @param instance SDHC instance ID
 * @return the DMA address
 */
static inline uint32_t sdhc_hal_get_dma_addr(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return HW_SDHC_DSADDR_RD();
}

/*!
 * @brief Gets the block size configured.
 *
 * @param instance SDHC instance ID
 * @return the block size already configured
 */
static inline uint32_t sdhc_hal_get_blksz(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_BLKATTR_BLKSIZE;
}

/*!
 * @brief Sets the block size.
 *
 * @param instance SDHC instance ID
 * @param blockSize the block size
 */
static inline void sdhc_hal_set_blksz(uint8_t instance, uint32_t blockSize)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_BLKATTR_BLKSIZE(blockSize);
}

/*!
 * @brief Sets the block count.
 *
 * @param instance SDHC instance ID
 * @param blockCount the block count
 */
static inline void sdhc_hal_set_blkcnt(uint8_t instance, uint32_t blockCount)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_BLKATTR_BLKCNT(blockCount);
}

/*!
 * @brief Gets the block count configured.
 *
 * @param instance SDHC instance id
 * @return the block count already configured
 */
static inline uint32_t sdhc_hal_get_blkcnt(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_BLKATTR_BLKCNT;
}

/*!
 * @brief Configures the command argument.
 *
 * @param instance SDHC instance ID
 * @param arg the command argument
 */
static inline void sdhc_hal_set_cmd_arg(uint8_t instance, uint32_t arg)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_CMDARG_CMDARG(arg);
}

/*!
 * @brief Sends a command.
 *
 * @param instance SDHC instance ID
 * @param index command index
 * @param flags transfer type flags
 */
static inline void sdhc_hal_send_cmd(uint8_t instance, uint32_t index, uint32_t flags)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    HW_SDHC_XFERTYP_WR(((index << BP_SDHC_XFERTYP_CMDINX) & BM_SDHC_XFERTYP_CMDINX)
            | (flags & ( BM_SDHC_XFERTYP_DMAEN | BM_SDHC_XFERTYP_MSBSEL | BM_SDHC_XFERTYP_DPSEL
                | BM_SDHC_XFERTYP_CMDTYP | BM_SDHC_XFERTYP_BCEN | BM_SDHC_XFERTYP_CICEN
                | BM_SDHC_XFERTYP_CCCEN | BM_SDHC_XFERTYP_RSPTYP | BM_SDHC_XFERTYP_DTDSEL
                | BM_SDHC_XFERTYP_AC12EN)));
}

/*!
 * @brief Fills the the data port.
 *
 * @param instance SDHC instance ID
 * @param data the data about to be sent
 */
static inline void sdhc_hal_set_data(uint8_t instance, uint32_t data)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    HW_SDHC_DATPORT_WR(data);
}

/*!
 * @brief Retrieves the data from the data port.
 *
 * @param instance SDHC instance ID
 * @return data the data read
 */
static inline uint32_t sdhc_hal_get_data(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_DATPORT_DATCONT;
}

/*!
 * @brief Checks whether the command inhibit bit is set or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if command inhibit, 0 if not.
 */
static inline uint32_t sdhc_hal_is_cmd_inhibit(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_CIHB;
}

/*!
 * @brief Checks whether data inhibit bit is set or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if data inhibit, 0 if not.
 */
static inline uint32_t sdhc_hal_is_data_inhibit(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_CDIHB;
}

/*!
 * @brief Checks whether data line is active.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's active, 0 if not.
 */
static inline uint32_t sdhc_hal_is_data_line_active(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_DLA;
}

/*!
 * @brief Checks whether the SD clock is stable or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's stable, 0 if not.
 */
static inline uint32_t sdhc_hal_is_sd_clk_stable(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_SDSTB;
}

/*!
 * @brief Checks whether the  IPG clock is off or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's off, 0 if not.
 */
static inline uint32_t sdhc_hal_is_ipg_clk_off(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_IPGOFF;
}

/*!
 * @brief Checks whether the system clock is off or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's off, 0 if not.
 */
static inline uint32_t sdhc_hal_is_sys_clk_off(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_HCKOFF;
}

/*!
 * @brief Checks whether the peripheral clock is off or not.
 *
 * @param instance SDHC instance ID.
 * @return 1 if it's off, 0 if not.
 */
static inline uint32_t sdhc_hal_is_per_clk_off(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_PEROFF;
}

/*!
 * @brief Checks whether  the  SD clock is off or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's off, 0 if not.
 */
static inline uint32_t sdhc_hal_is_sd_clk_off(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_SDOFF;
}

/*!
 * @brief Checks whether the write transfer is active or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's active, 0 if not.
 */
static inline uint32_t sdhc_hal_is_write_trans_active(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_WTA;
}

/*!
 * @brief Checks whether the read transfer is active or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's off, 0 if not.
 */
static inline uint32_t sdhc_hal_is_read_trans_active(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_RTA;
}

/*!
 * @brief Check whether the buffer write is enabled or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's isEnabledd, 0 if not.
 */
static inline uint32_t sdhc_hal_is_buf_write_enabled(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_BWEN;
}

/*!
 * @brief Checks whether the buffer read is enabled or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's isEnabledd, 0 if not.
 */
static inline uint32_t sdhc_hal_is_buf_read_enabled(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_BREN;
}

/*!
 * @brief Checks whether the  card is inserted or not.
 *
 * @param instance SDHC instance ID.
 * @return 1 if it's inserted, 0 if not.
 */
static inline uint32_t sdhc_hal_is_card_inserted(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_CINS;
}

/*!
 * @brief Checks whether the command line signal is high or not.
 *
 * @param instance SDHC instance ID
 * @return 1 if it's high, 0 if not.
 */
static inline uint32_t sdhc_hal_is_cmd_line_level_high(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_CLSL;
}

/*!
 * @brief Gets the data line signal level or not.
 *
 * @param instance SDHC instance ID
 * @return [7:0] data line signal level
 */
static inline uint32_t sdhc_hal_get_data_line_level(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PRSSTAT_DLSL;
}

/*!
 * @brief Sets the LED state.
 *
 * @param instance SDHC instance ID
 * @param state the LED state
 */
static inline void sdhc_hal_set_led_state(uint8_t instance, sdhc_hal_led_t state)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_LCTL(state);
}

/*!
 * @brief Sets the data transfer width.
 *
 * @param instance SDHC instance ID
 * @param dtw data transfer width
 */
static inline void sdhc_hal_set_data_trans_width(uint8_t instance, sdhc_hal_dtw_t dtw)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_DTW(dtw);
}

/*!
 * @brief Checks whether the DAT3 is taken as card detect pin.
 *
 * @param instance SDHC instance ID
 */
static inline bool sdhc_hal_is_d3cd_enabled(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PROCTL_D3CD;
}

/*!
 * @brief Enables the DAT3 as a card detect pin.
 *
 * @param instance SDHC instance ID
 * @param isEnabled isEnabled the feature
 */
static inline void sdhc_hal_enable_d3cd(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_D3CD(isEnabled ? 1 : 0);
}

/*!
 * @brief Configures the endian mode.
 *
 * @param instance SDHC instance ID
 * @param endianMode endian mode
 */
static inline void sdhc_hal_set_endian(uint8_t instance, sdhc_hal_endian_t endianMode)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_EMODE(endianMode);
}

/*!
* @brief Gets the card detect test level.
*
* @param instance SDHC instance ID
* @return card detect test level
*/
static inline uint32_t sdhc_hal_get_cd_test_level(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_PROCTL_CDTL;
}

/*!
* @brief Enables the card detect test.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_cd_test(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_CDSS(isEnabled ? 1 : 0);
}

/*!
* @brief Sets the DMA mode.
*
* @param instance SDHC instance ID
* @param dmaMode the DMA mode
*/
static inline void sdhc_hal_set_dma_mode(uint8_t instance, sdhc_hal_dma_mode_t dmaMode)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_DMAS(dmaMode);
}

/*!
* @brief Enables stop at the block gap.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_stop_at_blkgap(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_SABGREQ(isEnabled ? 1 : 0);
}

/*!
* @brief Restarts a transaction which has stopped at the block gap.
*
* @param instance SDHC instance ID
*/
static inline void sdhc_hal_continue_req(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_CREQ(1);
}

/*!
* @brief Enables teh read wait control for the SDIO cards.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_read_wait_ctrl(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_RWCTL(isEnabled ? 1 : 0);
}

/*!
* @brief Enables  stop at the block gap requests.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_intr_stop_at_blk_gap(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_IABG(isEnabled ? 1 : 0);
}

/*!
* @brief Enables wakeup event on the card interrupt.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_wakeup_on_card_intr(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_WECINT(isEnabled ? 1 : 0);
}

/*!
* @brief Enables wakeup event on the card insertion.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_wakeup_on_card_ins(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_WECINS(isEnabled ? 1 : 0);
}

/*!
* @brief Enables  wakeup event on card removal.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_wakeup_on_card_rm(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_PROCTL_WECRM(isEnabled ? 1 : 0);
}

/*!
* @brief Enables the IPG clock, then no automatic clock gating off.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_ipg_clk(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_SYSCTL_IPGEN(isEnabled ? 1 : 0);
}

/*!
* @brief Enables the system clock, then no automatic clock gating off.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_sys_clk(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_SYSCTL_HCKEN(isEnabled ? 1 : 0);
}

/*!
* @brief Enables the peripheral clock, then no automatic clock gating off.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_per_clk(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_SYSCTL_PEREN(isEnabled ? 1 : 0);
}

/*!
* @brief Enables the SD clock. It should be disabled before changing SD clock
* frequency.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_sd_clk(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_SYSCTL_SDCLKEN(isEnabled ? 1 : 0);
}

/*!
* @brief Sets the SD clock frequency divisor.
*
* @param instance SDHC instance ID
* @param divisor the divisor
*/
static inline void sdhc_hal_set_clk_div(uint8_t instance, uint32_t divisor)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_SYSCTL_DVS(divisor);
}

/*!
* @brief Sets the SD clock frequency select.
*
* @param instance SDHC instance ID
* @param freq the frequency selector
*/
static inline void sdhc_hal_set_clk_freq(uint8_t instance, uint32_t freq)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_SYSCTL_SDCLKFS(freq);
}

/*!
* @brief Sets the data timeout counter value.
*
* @param instance SDHC instance ID
* @param timeout
*/
static inline void sdhc_hal_set_data_timeout(uint8_t instance, uint32_t timeout)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_SYSCTL_DTOCV(timeout);
}

/*!
* @brief Performs the kinds of SDHC reset.
*
* @param instance SDHC instance ID
* @param type the type of reset
*/
static inline void sdhc_hal_reset(uint8_t instance, uint32_t type)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    HW_SDHC_SYSCTL_SET(type & (BM_SDHC_SYSCTL_RSTA | BM_SDHC_SYSCTL_RSTC | BM_SDHC_SYSCTL_RSTD));
}

/*!
* @brief Checks whether   the given SDHC reset is finished.
*
* @param instance SDHC instance ID
* @param type the type of reset
* @return if the given reset is done
*/
static inline uint32_t sdhc_hal_is_reset_done(uint8_t instance, uint32_t type)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return !(HW_SDHC_SYSCTL_RD()
            & (type & (BM_SDHC_SYSCTL_RSTA | BM_SDHC_SYSCTL_RSTC | BM_SDHC_SYSCTL_RSTD)));
}

/*!
* @brief Sends  80 SD clock cycles to the card.
*
* @param instance SDHC instance ID
*/
static inline void sdhc_hal_init_card(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_SYSCTL_INITA(1);
}

/*!
* @brief Checks whether sending 80 SD clock cycles to card is finished.
*
* @param instance SDHC instance ID
* @return if sending 80 SD clock cycles is finished
*/
static inline uint32_t sdhc_hal_is_init_card_done(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return !(BR_SDHC_SYSCTL_INITA);
}

/*!
* @brief Gets the current interrupt status.
*
* @param instance SDHC instance ID
* @return current interrupt flags
*/
static inline uint32_t sdhc_hal_get_intr_flags(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return HW_SDHC_IRQSTAT_RD();
}

/*!
* @brief Clears a specified interrupt status.
*
* @param instance SDHC instance ID
* @param mask to specify interrupts' flags to be cleared
*/
static inline void sdhc_hal_clear_intr_flags(uint8_t instance, uint32_t mask)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    HW_SDHC_IRQSTAT_WR(mask);
}

/*!
* @brief Gets the currently enabled interrupt signal.
*
* @param instance SDHC instance ID
* @return currently enabled interrupt signal
*/
static inline uint32_t sdhc_hal_get_intr_signal(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return HW_SDHC_IRQSIGEN_RD();
}

/*!
* @brief Gets the currently enabled interrupt state.
*
* @param instance SDHC instance ID
* @return currently enabled interrupts' state
*/
static inline uint32_t sdhc_hal_get_intr_state(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return HW_SDHC_IRQSTATEN_RD();
}

/*!
* @brief Gets the auto cmd12 error.
*
* @param instance SDHC instance ID
* @return auto cmd12 error status
*/
static inline uint32_t sdhc_hal_get_ac12_error(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return HW_SDHC_AC12ERR_RD();
}

/*!
* @brief Gets the maximum block length supported.
*
* @param instance SDHC instance ID
* @return the maximum block length support
*/
static inline uint32_t sdhc_hal_get_max_blklen(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_HTCAPBLT_MBL;
}

/*!
* @brief Checks whether the ADMA is supported.
*
* @param instance SDHC instance ID
* @return if ADMA is supported
*/
static inline uint32_t sdhc_hal_host_can_do_adma(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_HTCAPBLT_ADMAS;
}

/*!
* @brief Checks whether the  high speed is supported.
*
* @param instance SDHC instance ID
* @return if high speed is supported
*/
static inline uint32_t sdhc_hal_host_can_do_highspeed(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_HTCAPBLT_HSS;
}

/*!
* @brief Checks whether the  DMA is supported.
*
* @param instance SDHC instance ID
* @return if high speed is supported
*/
static inline uint32_t sdhc_hal_host_can_do_dma(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_HTCAPBLT_DMAS;
}

/*!
* @brief Checks whether the suspend/resume is supported.
*
* @param instance SDHC instance ID
* @return if suspend and resume is supported
*/
static inline uint32_t sdhc_hal_host_can_do_suspend_resume(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_HTCAPBLT_SRS;
}

/*!
* @brief Checks whether the  voltage 3.3 is supported.
*
* @param instance SDHC instance ID
* @return if voltage 3.3 is supported
*/
static inline uint32_t sdhc_hal_host_supports_v330(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_HTCAPBLT_VS33;
}

/*!
* @brief Checks whether the  voltage 3.0 is supported.
*
* @param instance SDHC instance ID
* @return if voltage 3.0 is supported
*/
static inline uint32_t sdhc_hal_host_supports_v300(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
#if defined(FSL_FEATURE_SDHC_HAS_V300_SUPPORT) && FSL_FEATURE_SDHC_HAS_V300_SUPPORT
    return BR_SDHC_HTCAPBLT_VS30;
#else
    return 0;
#endif
}

/*!
* @brief Checks whether the voltage 1.8 is supported.
*
* @param instance SDHC instance ID
* @return if voltage 1.8 is supported
*/
static inline uint32_t sdhc_hal_host_supports_v180(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
#if defined(FSL_FEATURE_SDHC_HAS_V180_SUPPORT) && FSL_FEATURE_SDHC_HAS_V180_SUPPORT
    return BR_SDHC_HTCAPBLT_VS18;
#else
    return 0;
#endif
}

/*!
* @brief Sets the watermark for writing.
*
* @param instance SDHC instance ID
* @param watermark for writing
*/
static inline void sdhc_hal_set_write_watermark(uint8_t instance, uint32_t watermark)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_WML_WRWML(watermark);
}

/*!
* @brief Sets the watermark for reading.
*
* @param instance SDHC instance ID
* @param watermark for reading
*/
static inline void sdhc_hal_set_read_watermark(uint8_t instance, uint32_t watermark)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_WML_RDWML(watermark);
}

/*!
* @brief Sets the force events according to the given mask.
*
* @param instance SDHC instance ID
* @param mask to specify the force events' flags to be set
*/
static inline void sdhc_hal_set_force_event_flags(uint8_t instance, uint32_t mask)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    HW_SDHC_FEVT_WR(mask);
}

/*!
* @brief Checks whether the ADMA error is length mismatch.
*
* @param instance SDHC instance ID
* @return if ADMA error is length mismatch
*/
static inline uint32_t sdhc_hal_is_adma_len_mismatch_err(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_ADMAES_ADMALME;
}

/*!
* @brief Gets back the state of the ADMA error.
*
* @param instance SDHC instance ID
* @return error state
*/
static inline uint32_t sdhc_hal_get_adma_error_stat(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_ADMAES_ADMAES;
}

/*!
* @brief Checks whether the  ADMA error is a descriptor error.
*
* @param instance SDHC instance ID
* @return if ADMA error is descriptor error
*/
static inline uint32_t sdhc_hal_is_adma_desc_err(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_ADMAES_ADMADCE;
}

/*!
* @brief Sets the ADMA address.
*
* @param instance SDHC instance ID
* @param address for ADMA transfer
*/
static inline void sdhc_hal_set_adma_addr(uint8_t instance, uint32_t address)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    HW_SDHC_ADSADDR_WR(BF_SDHC_ADSADDR_ADSADDR(address));
}

/*!
* @brief Enables the external DMA request.
*
* @param instance SDHC instance ID
* @param isEnabled or not
*/
static inline void sdhc_hal_enable_ext_dma_req(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_VENDOR_EXTDMAEN(isEnabled ? 1 : 0);
}

/*!
* @brief Enables the exact block number for the SDIO CMD53.
*
* @param instance SDHC instance ID
* @param isEnabled or not
*/
static inline void sdhc_hal_enable_exact_blk_num(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_VENDOR_EXBLKNU(isEnabled ? 1 : 0);
}

/*!
* @brief Sets the timeout value for the boot ACK.
*
* @param instance SDHC instance ID
* @param timeout
*/
static inline void sdhc_hal_set_boot_ack_timeout(uint8_t instance, uint32_t timeout)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_MMCBOOT_DTOCVACK(timeout);
}

/*!
* @brief Enables the boot ACK.
*
* @param instance SDHC instance ID
* @param isEnabled
*/
static inline void sdhc_hal_enable_boot_ack(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_MMCBOOT_BOOTACK(isEnabled ? 1 : 0);
}

/*!
* @brief Configures the boot mode.
*
* @param instance SDHC instance ID
* @param mode the boot mode
*/
static inline void sdhc_hal_set_boot_mode(uint8_t instance, sdhc_hal_mmcboot_t mode)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_MMCBOOT_BOOTMODE(mode);
}

/*!
* @brief Enables the fast boot.
*
* @param instance SDHC instance ID
* @param isEnabled or not
*/
static inline void sdhc_hal_enable_fastboot(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_MMCBOOT_BOOTEN(isEnabled ? 1 : 0);
}

/*!
* @brief Enables the automatic stop at the block gap.
*
* @param instance SDHC instance ID
* @param isEnabled or not
*/
static inline void sdhc_hal_enable_auto_stop_at_blkgap(uint8_t instance, bool isEnabled)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_MMCBOOT_AUTOSABGEN(isEnabled ? 1 : 0);
}

/*!
* @brief Configures the the block count for the boot.
*
* @param instance SDHC instance ID
* @param blockCount the block count for boot
*/
static inline void sdhc_hal_set_boot_blkcnt(uint8_t instance, uint32_t blockCount)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    BW_SDHC_MMCBOOT_BOOTBLKCNT(blockCount);
}

/*!
* @brief Gets a specification version.
*
* @param instance SDHC instance ID
* @return specification version
*/
static inline uint32_t sdhc_hal_get_spec_ver(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_HOSTVER_SVN;
}

/*!
* @brief Gets the vendor version.
*
* @param instance SDHC instance ID
* @return vendor version
*/
static inline uint32_t sdhc_hal_get_vendor_ver(uint8_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return BR_SDHC_HOSTVER_VVN;
}

/*!
 * @brief Gets the command response.
 *
 * @param instance SDHC instance ID
 * @param resp an array of response, 4 bytes
 */
void sdhc_hal_get_resp(uint8_t instance, uint32_t * resp);

/*!
* @brief Enables the specified interrupts.
*
* @param instance SDHC instance ID
* @param isEnabled enable or disable
* @param mask to specify interrupts to be isEnabledd
*/
void sdhc_hal_enable_intr_signal(uint8_t instance, bool isEnabled, uint32_t mask);

/*!
* @brief Enables the specified interrupt state.
*
* @param instance SDHC instance ID
* @param isEnabled enable or disable
* @param mask to specify interrupts' state to be enabled
*/
void sdhc_hal_enable_intr_state(uint8_t instance, bool isEnabled, uint32_t mask);

/*@} */

#if defined(__cplusplus)
}
#endif
/*! @} */

#endif

/*************************************************************************************************
 * EOF
 ************************************************************************************************/

