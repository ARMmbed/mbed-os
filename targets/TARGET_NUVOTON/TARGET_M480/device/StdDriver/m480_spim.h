/**************************************************************************//**
 * @file    spim.h
 * @version V1.00
 * @brief   M480 series SPIM driver header file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef __SPIM_H__
#define __SPIM_H__

/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup M480_Device_Driver M480 Device Driver
  @{
*/

/** @addtogroup M480_SPIM_Driver SPIM Driver
  @{
*/


/** @addtogroup M480_SPIM_EXPORTED_CONSTANTS SPIM Exported Constants
  @{
*/

#define SPIM_DMM_MAP_ADDR      0x8000000         /*!< DMM mode memory map base address     */
#define SPIM_DMM_SIZE          0x2000000         /*!< DMM mode memory mapping size         */
#define SPIM_CCM_ADDR          0x10020000        /*!< CCM mode memory map base address     */
#define SPIM_CCM_SIZE          0x8000            /*!< CCM mode memory size                 */

/*---------------------------------------------------------------------------------------------------------*/
/* SPIM_CTL0 constant definitions                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define SPIM_CTL0_RW_IN(x)              ((! (x)) << SPIM_CTL0_QDIODIR_Pos)      /*!< SPIM_CTL0: SPI Interface Direction Select */
#define SPIM_CTL0_BITMODE_SING          (0UL << SPIM_CTL0_BITMODE_Pos)          /*!< SPIM_CTL0: One bit mode (SPI Interface including DO, DI, HOLD, WP) */
#define SPIM_CTL0_BITMODE_DUAL          (1UL << SPIM_CTL0_BITMODE_Pos)          /*!< SPIM_CTL0: Two bits mode (SPI Interface including D0, D1, HOLD, WP) */
#define SPIM_CTL0_BITMODE_QUAD          (2UL << SPIM_CTL0_BITMODE_Pos)          /*!< SPIM_CTL0: Four bits mode (SPI Interface including D0, D1, D2, D3) */
#define SPIM_CTL0_OPMODE_IO             (0UL << SPIM_CTL0_OPMODE_Pos)           /*!< SPIM_CTL0: I/O Mode */
#define SPIM_CTL0_OPMODE_PAGEWRITE      (1UL << SPIM_CTL0_OPMODE_Pos)           /*!< SPIM_CTL0: Page Write Mode */
#define SPIM_CTL0_OPMODE_PAGEREAD       (2UL << SPIM_CTL0_OPMODE_Pos)           /*!< SPIM_CTL0: Page Read Mode */
#define SPIM_CTL0_OPMODE_DIRECTMAP      (3UL << SPIM_CTL0_OPMODE_Pos)           /*!< SPIM_CTL0: Direct Map Mode */

#define CMD_NORMAL_PAGE_PROGRAM         (0x02UL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Page Program (Page Write Mode Use) */
#define CMD_NORMAL_PAGE_PROGRAM_4B      (0x12UL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Page Program (Page Write Mode Use) */
#define CMD_QUAD_PAGE_PROGRAM_WINBOND   (0x32UL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Quad Page program (for Winbond) (Page Write Mode Use) */
#define CMD_QUAD_PAGE_PROGRAM_MXIC      (0x38UL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Quad Page program (for MXIC) (Page Write Mode Use) */
#define CMD_QUAD_PAGE_PROGRAM_EON       (0x40UL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Quad Page Program (for EON) (Page Write Mode Use) */

#define CMD_DMA_NORMAL_READ             (0x03UL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Read Data (Page Read Mode Use) */
#define CMD_DMA_FAST_READ               (0x0BUL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Fast Read (Page Read Mode Use) */
#define CMD_DMA_NORMAL_DUAL_READ        (0x3BUL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Fast Read Dual Output (Page Read Mode Use) */
#define CMD_DMA_FAST_DUAL_READ          (0xBBUL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Fast Read Dual Output (Page Read Mode Use) */
#define CMD_DMA_NORMAL_QUAD_READ        (0xE7UL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Fast Read Quad I/O (Page Read Mode Use) */
#define CMD_DMA_FAST_QUAD_READ          (0xEBUL << SPIM_CTL0_CMDCODE_Pos)       /*!< SPIM_CTL0: Fast Read Quad I/O (Page Read Mode Use) */

typedef enum {
    MFGID_UNKNOW    = 0x00,
    MFGID_SPANSION  = 0x01,
    MFGID_EON       = 0x1C,
    MFGID_ISSI      = 0x7F,
    MFGID_MXIC      = 0xC2,
    MFGID_WINBOND   = 0xEF
}
E_MFGID;

/* Flash opcodes. */
#define OPCODE_WREN             0x06    /* Write enable */
#define OPCODE_RDSR             0x05    /* Read status register #1*/
#define OPCODE_WRSR             0x01    /* Write status register #1 */
#define OPCODE_RDSR2            0x35    /* Read status register #2*/
#define OPCODE_WRSR2            0x31    /* Write status register #2 */
#define OPCODE_RDSR3            0x15    /* Read status register #3*/
#define OPCODE_WRSR3            0x11    /* Write status register #3 */
#define OPCODE_NORM_READ        0x03    /* Read data bytes (low frequency) */
#define OPCODE_FAST_READ        0x0b    /* Read data bytes (high frequency) */
#define OPCODE_PP               0x02    /* Page program (up to 256 bytes) */
#define OPCODE_SE_4K            0x20    /* Erase 4KB sector */
#define OPCODE_BE_32K           0x52    /* Erase 32KB block */
#define OPCODE_CHIP_ERASE       0xc7    /* Erase whole flash chip */
#define OPCODE_BE_64K           0xd8    /* Erase 64KB block */
#define OPCODE_READ_ID          0x90    /* Read ID */
#define OPCODE_RDID             0x9f    /* Read JEDEC ID */
#define OPCODE_BRRD             0x16    /* SPANSION flash - Bank Register Read command  */
#define OPCODE_BRWR             0x17    /* SPANSION flash - Bank Register write command */

/* Used for SST flashes only. */
#define OPCODE_BP               0x02    /* Byte program */
#define OPCODE_WRDI             0x04    /* Write disable */
#define OPCODE_AAI_WP           0xad    /* Auto u32Address increment word program */

/* Used for Macronix flashes only. */
#define OPCODE_EN4B             0xb7    /* Enter 4-byte mode */
#define OPCODE_EX4B             0xe9    /* Exit 4-byte mode */

#define OPCODE_RDSCUR           0x2b
#define OPCODE_WRSCUR           0x2f

#define OPCODE_RSTEN            0x66
#define OPCODE_RST              0x99

#define OPCODE_ENQPI            0x38
#define OPCODE_EXQPI            0xFF

/* Status Register bits. */
#define SR_WIP                  1       /* Write in progress */
#define SR_WEL                  2       /* Write enable latch */
#define SR_QE                   0x40    /* Quad Enable for MXIC */
/* Status Register #2 bits. */
#define SR2_QE                  2       /* Quad Enable for Winbond */
/* meaning of other SR_* bits may differ between vendors */
#define SR_BP0                  4       /* Block protect 0 */
#define SR_BP1                  8       /* Block protect 1 */
#define SR_BP2                  0x10    /* Block protect 2 */
#define SR_SRWD                 0x80    /* SR write protect */
#define SR3_ADR                 0x01    /* 4-byte u32Address mode */

#define SCUR_4BYTE              0x04    /* 4-byte u32Address mode */

/*@}*/ /* end of group M480_SPIM_EXPORTED_CONSTANTS */


/** @addtogroup M480_SPIM_EXPORTED_MACROS SPIM Exported Macros
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Macros and functions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/

/**
 * @details    Enable cipher.
 */
#define _SPIM_ENABLE_CIPHER()       (SPIM->CTL0 &= ~SPIM_CTL0_CIPHOFF_Msk)

/**
 * @details    Disable cipher.
 */
#define _SPIM_DISABLE_CIPHER()      (SPIM->CTL0 |= SPIM_CTL0_CIPHOFF_Msk)

/**
 * @details    Enable cipher balance
 */
#define _SPIM_ENABLE_BALEN()        (SPIM->CTL0 |= SPIM_CTL0_BALEN_Msk)

/**
 * @details    Disable cipher balance
 */
#define _SPIM_DISABLE_BALEN()       (SPIM->CTL0 &= ~SPIM_CTL0_BALEN_Msk)

/**
 * @details    Set 4-byte address to be enabled/disabled.
 */
#define _SPIM_SET_4BYTE_ADDR_EN(x) \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~SPIM_CTL0_B4ADDREN_Msk)) | ((x) << SPIM_CTL0_B4ADDREN_Pos); \
    } while (0)

/**
 * @details    Enable SPIM interrupt
 */
#define _SPIM_ENABLE_INT()       (SPIM->CTL0 |= SPIM_CTL0_IEN_Msk)

/**
 * @details    Disable SPIM interrupt
 */
#define _SPIM_DISABLE_INT()      (SPIM->CTL0 &= ~SPIM_CTL0_IEN_Msk)

/**
 * @details    Is interrupt flag on.
 */
#define _SPIM_IS_IF_ON()    ((SPIM->CTL0 & SPIM_CTL0_IF_Msk) != 0)

/**
 * @details    Clear interrupt flag.
 */
#define _SPIM_CLR_INT() \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~SPIM_CTL0_IF_Msk)) | (1 << SPIM_CTL0_IF_Pos);  \
    } while (0)

/**
 * @details    Set transmit/receive bit length
 */
#define _SPIM_SET_DATA_WIDTH(x)   \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~SPIM_CTL0_DWIDTH_Msk)) | (((x) - 1) << SPIM_CTL0_DWIDTH_Pos);  \
    } while (0)

/**
 * @details    Get data transmit/receive bit length setting
 */
#define _SPIM_GET_DATA_WIDTH()   \
    (((SPIM->CTL0 & SPIM_CTL0_DWIDTH_Msk) >> SPIM_CTL0_DWIDTH_Pos)+1)

/**
 * @details    Set data transmit/receive burst number
 */
#define _SPIM_SET_DATA_NUM(x) \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~SPIM_CTL0_BURSTNUM_Msk)) | (((x) - 1) << SPIM_CTL0_BURSTNUM_Pos);  \
    } while (0)

/**
 * @details    Get data transmit/receive burst number
 */
#define _SPIM_GET_DATA_NUM() \
    (((SPIM->CTL0 & SPIM_CTL0_BURSTNUM_Msk) >> SPIM_CTL0_BURSTNUM_Pos)+1)

/**
 * @details    Enable Single Input mode.
 */
#define _SPIM_ENABLE_SING_INPUT_MODE()  \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~(SPIM_CTL0_BITMODE_Msk | SPIM_CTL0_QDIODIR_Msk))) | (SPIM_CTL0_BITMODE_SING | SPIM_CTL0_RW_IN(1));    \
    } while (0)

/**
 * @details    Enable Single Output mode.
 */
#define _SPIM_ENABLE_SING_OUTPUT_MODE() \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~(SPIM_CTL0_BITMODE_Msk | SPIM_CTL0_QDIODIR_Msk))) | (SPIM_CTL0_BITMODE_SING | SPIM_CTL0_RW_IN(0));    \
    } while (0)

/**
 * @details    Enable Dual Input mode.
 */
#define _SPIM_ENABLE_DUAL_INPUT_MODE()  \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~(SPIM_CTL0_BITMODE_Msk | SPIM_CTL0_QDIODIR_Msk))) | (SPIM_CTL0_BITMODE_DUAL | SPIM_CTL0_RW_IN(1));    \
    } while (0)

/**
 * @details    Enable Dual Output mode.
 */
#define _SPIM_ENABLE_DUAL_OUTPUT_MODE() \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~(SPIM_CTL0_BITMODE_Msk | SPIM_CTL0_QDIODIR_Msk))) | (SPIM_CTL0_BITMODE_DUAL | SPIM_CTL0_RW_IN(0));    \
    } while (0)

/**
 * @details    Enable Quad Input mode.
 */
#define _SPIM_ENABLE_QUAD_INPUT_MODE()  \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~(SPIM_CTL0_BITMODE_Msk | SPIM_CTL0_QDIODIR_Msk))) | (SPIM_CTL0_BITMODE_QUAD | SPIM_CTL0_RW_IN(1));    \
    } while (0)

/**
 * @details    Enable Quad Output mode.
 */
#define _SPIM_ENABLE_QUAD_OUTPUT_MODE() \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~(SPIM_CTL0_BITMODE_Msk | SPIM_CTL0_QDIODIR_Msk))) | (SPIM_CTL0_BITMODE_QUAD | SPIM_CTL0_RW_IN(0));    \
    } while (0)

/**
 * @details    Set suspend interval which ranges between 0 and 15.
 */
#define _SPIM_SET_SUSP_INTVL(x) \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~SPIM_CTL0_SUSPITV_Msk)) | ((x) << SPIM_CTL0_SUSPITV_Pos);    \
    } while (0)

/**
 * @details    Get suspend interval setting
 */
#define _SPIM_GET_SUSP_INTVL() \
    ((SPIM->CTL0 & SPIM_CTL0_SUSPITV_Msk) >> SPIM_CTL0_SUSPITV_Pos)

/**
 * @details    Set operation mode.
 */
#define _SPIM_SET_OPMODE(x)  \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~SPIM_CTL0_OPMODE_Msk)) | x; \
    } while (0)

/**
 * @details    Get operation mode.
 */
#define _SPIM_GET_OP_MODE()         (SPIM->CTL0 & SPIM_CTL0_OPMODE_Msk)

/**
 * @details    Set SPIM mode.
 */
#define _SPIM_SET_SPIM_MODE(x)    \
    do {    \
        SPIM->CTL0 = (SPIM->CTL0 & (~SPIM_CTL0_CMDCODE_Msk)) | x;   \
    } while (0)

/**
 * @details    Get SPIM mode.
 */
#define _SPIM_GET_SPIM_MODE()       (SPIM->CTL0 & SPIM_CTL0_CMDCODE_Msk)

/**
 * @details    Start operation.
 */
#define _SPIM_SET_GO()              (SPIM->CTL1 |= SPIM_CTL1_SPIMEN_Msk)

/**
 * @details    Is engine busy.
 */
#define _SPIM_IS_BUSY()             (SPIM->CTL1 & SPIM_CTL1_SPIMEN_Msk)

/**
 * @details    Wait for free.
 */
#define _SPIM_WAIT_FREE()   \
    do {    \
        while (SPIM->CTL1 & SPIM_CTL1_SPIMEN_Msk);   \
    } while (0)

/**
 * @details    Enable cache.
 */
#define _SPIM_ENABLE_CACHE()        (SPIM->CTL1 &= ~SPIM_CTL1_CACHEOFF_Msk)

/**
 * @details    Disable cache.
 */
#define _SPIM_DISABLE_CACHE()       (SPIM->CTL1 |= SPIM_CTL1_CACHEOFF_Msk)

/**
 * @details    Is cache enabled.
 */
#define _SPIM_IS_CACHE_EN()         ((SPIM->CTL1 & SPIM_CTL1_CACHEOFF_Msk) ? 0 : 1)

/**
 * @details    Enable CCM
 */
#define _SPIM_ENABLE_CCM()          (SPIM->CTL1 |= SPIM_CTL1_CCMEN_Msk)

/**
 * @details    Disable CCM.
 */
#define _SPIM_DISABLE_CCM()         (SPIM->CTL1 &= ~SPIM_CTL1_CCMEN_Msk)

/**
 * @details    Is CCM enabled.
 */
#define _SPIM_IS_CCM_EN()           ((SPIM->CTL1 & SPIM_CTL1_CCMEN_Msk) >> SPIM_CTL1_CCMEN_Pos)

/**
 * @details    Invalidate cache.
 */
#define _SPIM_INVALID_CACHE()       (SPIM->CTL1 |= SPIM_CTL1_CDINVAL_Msk)

/**
 * @details    Set SS(Select Active) to active level.
 */
#define _SPIM_SET_SS_EN(x) \
    do {    \
        (SPIM->CTL1 = (SPIM->CTL1 & (~SPIM_CTL1_SS_Msk)) |  ((! (x)) << SPIM_CTL1_SS_Pos)); \
    } while (0)

/**
 * @details    Is SS(Select Active) in active level.
 */
#define _SPI_GET_SS_EN() \
    (!(SPIM->CTL1 & SPIM_CTL1_SS_Msk))

/**
 * @details    Set active level of slave select to be high/low.
 */
#define _SPI_SET_SS_ACTLVL(x) \
    do {    \
        (SPIM->CTL1 = (SPIM->CTL1 & (~SPIM_CTL1_SSACTPOL_Msk)) | ((!! (x)) << SPIM_CTL1_SSACTPOL_Pos));   \
    } while (0)

/**
 * @details    Set idle time interval
 */
#define _SPI_SET_IDL_INTVL(x) \
    do {    \
       SPIM->CTL1 = (SPIM->CTL1 & (~SPIM_CTL1_IDLETIME_Msk)) | ((x) << SPIM_CTL1_IDLETIME_Pos);  \
    } while (0)

/**
 * @details    Get idle time interval setting
 */
#define _SPI_GET_IDL_INTVL() \
    ((SPIM->CTL1 & SPIM_CTL1_IDLETIME_Msk) >> SPIM_CTL1_IDLETIME_Pos)

/**
 * @details    Set SPIM clock divider
 */
#define _SPIM_SET_CLOCK_DIVIDER(x)    \
    do {    \
       SPIM->CTL1 = (SPIM->CTL1 & (~SPIM_CTL1_DIVIDER_Msk)) | ((x) << SPIM_CTL1_DIVIDER_Pos);    \
    } while (0)

/**
 * @details    Get SPIM current clock divider setting
 */
#define _SPIM_GET_CLOCK_DIVIDER()   \
    ((SPIM->CTL1 & SPIM_CTL1_DIVIDER_Msk) >> SPIM_CTL1_DIVIDER_Pos)

/**
 * @details    Set SPI flash deselect time interval of DMA write mode
 */
#define _SPIM_SET_RXCLKDLY_DWDELSEL(x) \
    do {    \
        (SPIM->RXCLKDLY = (SPIM->RXCLKDLY & (~SPIM_RXCLKDLY_DWDELSEL_Msk)) |  ((x) << SPIM_RXCLKDLY_DWDELSEL_Pos)); \
    } while (0)

/**
 * @details    Get SPI flash deselect time interval of DMA write mode
 */
#define _SPIM_GET_RXCLKDLY_DWDELSEL() \
    ((SPIM->RXCLKDLY & SPIM_RXCLKDLY_DWDELSEL_Msk) >> SPIM_RXCLKDLY_DWDELSEL_Pos)

/**
 * @details    Set sampling clock delay selection for received data
 */
#define _SPIM_SET_RXCLKDLY_RDDLYSEL(x) \
    do {    \
        (SPIM->RXCLKDLY = (SPIM->RXCLKDLY & (~SPIM_RXCLKDLY_RDDLYSEL_Msk)) |  (x << SPIM_RXCLKDLY_RDDLYSEL_Pos)); \
    } while (0)

/**
 * @details    Get sampling clock delay selection for received data
 */
#define _SPIM_GET_RXCLKDLY_RDDLYSEL() \
    ((SPIM->RXCLKDLY & SPIM_RXCLKDLY_RDDLYSEL_Msk) >> SPIM_RXCLKDLY_RDDLYSEL_Pos)

/**
 * @details    Set sampling clock edge selection for received data
 */
#define _SPIM_SET_RXCLKDLY_RDEDGE() \
    (SPIM->RXCLKDLY |= SPIM_RXCLKDLY_RDEDGE_Msk); \

/**
 * @details    Get sampling clock edge selection for received data
 */
#define _SPIM_CLR_RXCLKDLY_RDEDGE() \
    (SPIM->RXCLKDLY &= ~SPIM_RXCLKDLY_RDEDGE_Msk)

/**
 * @details    Set mode bits data for continuous read mode
 */
#define _SPIM_SET_DMMCTL_CRMDAT(x) \
    do {    \
        (SPIM->DMMCTL = (SPIM->DMMCTL & (~SPIM_DMMCTL_CRMDAT_Msk)) |  ((x) << SPIM_DMMCTL_CRMDAT_Pos)) | SPIM_DMMCTL_CREN_Msk; \
    } while (0)

/**
 * @details    Get mode bits data for continuous read mode
 */
#define _SPIM_GET_DMMCTL_CRMDAT() \
    ((SPIM->DMMCTL & SPIM_DMMCTL_CRMDAT_Msk) >> SPIM_DMMCTL_CRMDAT_Pos)

/**
 * @details    Set DMM mode SPI flash deselect time
 */
#define _SPIM_DMM_SET_DESELTIM(x) \
    do {    \
        SPIM->DMMCTL = (SPIM->DMMCTL & ~SPIM_DMMCTL_DESELTIM_Msk) | ((x&0x1F) << SPIM_DMMCTL_DESELTIM_Pos);   \
    } while (0)

/**
 * @details    Get current DMM mode SPI flash deselect time setting
 */
#define _SPIM_DMM_GET_DESELTIM() \
        ((SPIM->DMMCTL & SPIM_DMMCTL_DESELTIM_Msk) >> SPIM_DMMCTL_DESELTIM_Pos)

/**
 * @details    Enable DMM mode burst wrap mode
 */
#define _SPIM_DMM_ENABLE_BWEN()         (SPIM->DMMCTL |= SPIM_DMMCTL_BWEN_Msk)

/**
 * @details    Disable DMM mode burst wrap mode
 */
#define _SPIM_DMM_DISABLE_BWEN()        (SPIM->DMMCTL &= ~SPIM_DMMCTL_BWEN_Msk)

/**
 * @details    Enable DMM mode continuous read mode
 */
#define _SPIM_DMM_ENABLE_CREN()         (SPIM->DMMCTL |= SPIM_DMMCTL_CREN_Msk)

/**
 * @details    Disable DMM mode continuous read mode
 */
#define _SPIM_DMM_DISABLE_CREN()        (SPIM->DMMCTL &= ~SPIM_DMMCTL_CREN_Msk)

/**
 * @details    Set DMM mode SPI flash active SCLK time
 */
#define _SPIM_DMM_SET_ACTSCLKT(x) \
    do {    \
        SPIM->DMMCTL = (SPIM->DMMCTL & ~SPIM_DMMCTL_ACTSCLKT_Msk) | ((x&0xF) << SPIM_DMMCTL_ACTSCLKT_Pos) | SPIM_DMMCTL_UACTSCLK_Msk;   \
    } while (0)

/**
 * @details    Set SPI flash active SCLK time as SPIM default
 */
#define _SPIM_DMM_SET_DEFAULT_ACTSCLK()     (SPIM->DMMCTL &= ~SPIM_DMMCTL_UACTSCLK_Msk)

/**
 * @details    Set dummy cycle number (Only for DMM mode and DMA mode)
 */
#define _SPIM_SET_DCNUM(x) \
    do {    \
        SPIM->CTL2 = (SPIM->CTL2 & ~SPIM_CTL2_DCNUM_Msk) | ((x&0x1F) << SPIM_CTL2_DCNUM_Pos) | SPIM_CTL2_USETEN_Msk;   \
    } while (0)

/**
 * @details    Set dummy cycle number (Only for DMM mode and DMA mode) as SPIM default
 */
#define _SPIM_SET_DEFAULT_DCNUM(x)           (SPIM->CTL2 &= ~SPIM_CTL2_USETEN_Msk)


/*@}*/ /* end of group M480_SPIM_EXPORTED_MACROS */


/** @addtogroup M480_SPIM_EXPORTED_FUNCTIONS SPIM Exported Functions
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototypes                                                                              */
/*---------------------------------------------------------------------------------------------------------*/


/**
  * @brief      Get SPIM serial clock.
  * @return     SPI serial clock.
  * @details    This function calculates the serial clock of SPI in Hz.
  */
static __INLINE uint32_t SPIM_GetSClkFreq()
{
    uint32_t clkDiv = _SPIM_GET_CLOCK_DIVIDER();

    return clkDiv ? SystemCoreClock / (clkDiv * 2) : SystemCoreClock;
}


int  SPIM_InitFlash(int clrWP);
void SPIM_ReadJedecId(uint8_t *idBuf, unsigned u32NRx, uint32_t u32NBit);
void SPIM_Enable_4Bytes_Mode(int isEn, uint32_t u32NBit);

void SPIM_ChipErase(uint32_t u32NBit, int isSync);
void SPIM_EraseBlock(uint32_t u32Addr, int is4ByteAddr, uint32_t u32ErsCmd, uint32_t u32NBit, int isSync);

void SPIM_IO_Write(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NTx, uint8_t *pu8TxBuf, uint32_t wrCmd, uint32_t u32NBitCmd, uint32_t u32NBitAddr, uint32_t u32NBitDat);
void SPIM_IO_Read(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NRx, uint8_t *pu8RxBuf, uint32_t u32RdCmd, uint32_t u32NBitCmd, uint32_t u32NBitAddr, uint32_t u32NBitDat, int u32NDummy);

void SPIM_DMA_Write(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NTx, uint8_t *pu8TxBuf, uint32_t wrCmd);
void SPIM_DMA_Read(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NRx, uint8_t *pu8RxBuf, uint32_t u32RdCmd, int isSync);

void SPIM_EnterDirectMapMode(int is4ByteAddr, uint32_t u32RdCmd, uint32_t u32IdleIntvl);
void SPIM_ExitDirectMapMode(void);

void SPIM_SetQuadEnable(int isEn, uint32_t u32NBit);

/*@}*/ /* end of group M480_SPIM_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M480_SPIM_Driver */

/*@}*/ /* end of group M480_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__SPIM_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
