/**
 *******************************************************************************
 * @file    tmpm46b_fc.h
 * @brief   This file provides all the functions prototypes for FC driver.
 * @version V2.0.2.1
 * @date    2015/02/27
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_FC_H
#define __TMPM46B_FC_H


/* #define SINGLE_BOOT_MODE */

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @addtogroup FC
  * @{
  */

/** @defgroup FC_Exported_Types
  * @{
  */
    typedef enum {
        FC_SUCCESS = 0U,
        FC_ERROR_PROTECTED = 1U,
        FC_ERROR_OVER_TIME = 2U
    } FC_Result;

/** @} */
/* End of group FC_Exported_Types */

/** @defgroup FC_Exported_Constants
  * @{
  */

#define FLASH_PAGE_SIZE                 ((uint32_t)0x00001000)  /* Page Size 4096 Bytes/1024 words */
#define PROGRAM_UNIT                    4U      /* Page program could be written 16 bytes/4 words once */

#define FC_PAGE_0                       ((uint8_t)0x00)
#define FC_PAGE_1                       ((uint8_t)0x01)
#define FC_PAGE_2                       ((uint8_t)0x02)
#define FC_PAGE_3                       ((uint8_t)0x03)
#define FC_PAGE_4                       ((uint8_t)0x04)
#define FC_PAGE_5                       ((uint8_t)0x05)
#define FC_PAGE_6                       ((uint8_t)0x06)
#define FC_PAGE_7                       ((uint8_t)0x07)

#define FC_PAGE_MAX                     FC_PAGE_7
#define IS_FC_PAGE_NUM(param)           ((param) <= FC_PAGE_MAX)

#define FC_BLOCK_0                      ((uint8_t)0x00)
#define FC_BLOCK_1                      ((uint8_t)0x01)
#define FC_BLOCK_2                      ((uint8_t)0x02)
#define FC_BLOCK_3                      ((uint8_t)0x03)
#define FC_BLOCK_4                      ((uint8_t)0x04)
#define FC_BLOCK_5                      ((uint8_t)0x05)
#define FC_BLOCK_6                      ((uint8_t)0x06)
#define FC_BLOCK_7                      ((uint8_t)0x07)
#define FC_BLOCK_8                      ((uint8_t)0x08)
#define FC_BLOCK_9                      ((uint8_t)0x09)
#define FC_BLOCK_10                     ((uint8_t)0x0A)
#define FC_BLOCK_11                     ((uint8_t)0x0B)
#define FC_BLOCK_12                     ((uint8_t)0x0C)
#define FC_BLOCK_13                     ((uint8_t)0x0D)
#define FC_BLOCK_14                     ((uint8_t)0x0E)
#define FC_BLOCK_15                     ((uint8_t)0x0F)
#define FC_BLOCK_16                     ((uint8_t)0x10)
#define FC_BLOCK_17                     ((uint8_t)0x11)
#define FC_BLOCK_18                     ((uint8_t)0x12)
#define FC_BLOCK_19                     ((uint8_t)0x13)
#define FC_BLOCK_20                     ((uint8_t)0x14)
#define FC_BLOCK_21                     ((uint8_t)0x15)
#define FC_BLOCK_22                     ((uint8_t)0x16)
#define FC_BLOCK_23                     ((uint8_t)0x17)
#define FC_BLOCK_24                     ((uint8_t)0x18)
#define FC_BLOCK_25                     ((uint8_t)0x19)
#define FC_BLOCK_26                     ((uint8_t)0x1A)
#define FC_BLOCK_27                     ((uint8_t)0x1B)
#define FC_BLOCK_28                     ((uint8_t)0x1C)
#define FC_BLOCK_29                     ((uint8_t)0x1D)
#define FC_BLOCK_30                     ((uint8_t)0x1E)
#define FC_BLOCK_31                     ((uint8_t)0x1F)

#define FLASH_CHIP_SIZE                 ((uint32_t)0x00100000)  /* Flash chip size is 1024KByte */
#define FC_BLOCK_MAX                    FC_BLOCK_31

#define IS_FC_BLOCK_NUM(param)          (((param) <= FC_BLOCK_MAX) && ((param) >= FC_BLOCK_1))

#define FC_AREA_ALL                     ((uint8_t)0x00)
#define FC_AREA_0                       ((uint8_t)0x01)
#define FC_AREA_1                       ((uint8_t)0x02)

#define IS_FC_AREA(param)               ((param) <= FC_AREA_1)

#define FC_SWAP_SIZE_4K                 ((uint32_t)0x00000000)
#define FC_SWAP_SIZE_8K                 ((uint32_t)0x00000001)
#define FC_SWAP_SIZE_16K                ((uint32_t)0x00000002)
#define FC_SWAP_SIZE_32K                ((uint32_t)0x00000003)
#define FC_SWAP_SIZE_512K               ((uint32_t)0x00000004)

#define FC_SWAP_INITIAL                 ((uint32_t)0x00000000)
#define FC_SWAPPING                     ((uint32_t)0x00000001)
#define FC_SWAP_PROHIBIT                ((uint32_t)0x00000002)
#define FC_SWAP_RELEASE                 ((uint32_t)0x00000003)

#define FC_Clk_Div_1                    ((uint8_t) 0x00)
#define FC_Clk_Div_2                    ((uint8_t) 0x01)
#define FC_Clk_Div_3                    ((uint8_t) 0x02)
#define FC_Clk_Div_4                    ((uint8_t) 0x03)
#define FC_Clk_Div_5                    ((uint8_t) 0x04)
#define FC_Clk_Div_6                    ((uint8_t) 0x05)
#define FC_Clk_Div_7                    ((uint8_t) 0x06)
#define FC_Clk_Div_8                    ((uint8_t) 0x07)
#define FC_Clk_Div_9                    ((uint8_t) 0x08)
#define FC_Clk_Div_10                   ((uint8_t) 0x09)
#define FC_Clk_Div_11                   ((uint8_t) 0x0A)
#define FC_Clk_Div_12                   ((uint8_t) 0x0B)
#define FC_Clk_Div_13                   ((uint8_t) 0x0C)
#define FC_Clk_Div_14                   ((uint8_t) 0x0D)
#define FC_Clk_Div_15                   ((uint8_t) 0x0E)
#define FC_Clk_Div_16                   ((uint8_t) 0x0F)
#define FC_Clk_Div_17                   ((uint8_t) 0x10)
#define FC_Clk_Div_18                   ((uint8_t) 0x11)
#define FC_Clk_Div_19                   ((uint8_t) 0x12)
#define FC_Clk_Div_20                   ((uint8_t) 0x13)
#define FC_Clk_Div_21                   ((uint8_t) 0x14)
#define FC_Clk_Div_22                   ((uint8_t) 0x15)
#define FC_Clk_Div_23                   ((uint8_t) 0x16)
#define FC_Clk_Div_24                   ((uint8_t) 0x17)
#define FC_Clk_Div_25                   ((uint8_t) 0x18)
#define FC_Clk_Div_26                   ((uint8_t) 0x19)
#define FC_Clk_Div_27                   ((uint8_t) 0x1A)
#define FC_Clk_Div_28                   ((uint8_t) 0x1B)
#define FC_Clk_Div_29                   ((uint8_t) 0x1C)
#define FC_Clk_Div_30                   ((uint8_t) 0x1D)
#define FC_Clk_Div_31                   ((uint8_t) 0x1E)
#define FC_Clk_Div_32                   ((uint8_t) 0x1F)
#define IS_FC_WCLK_DIV(param)           ((param) <= FC_Clk_Div_32)

#define FC_PROG_CNT_250                 ((uint8_t) 0x00)
#define FC_PROG_CNT_300                 ((uint8_t) 0x01)
#define FC_PROG_CNT_350                 ((uint8_t) 0x02)
#define IS_FC_PROG_CNT(param)           ((param) <= (FC_PROG_CNT_350))

#define FC_ERAS_CNT_85                  ((uint8_t) 0x00)
#define FC_ERAS_CNT_90                  ((uint8_t) 0x01)
#define FC_ERAS_CNT_95                  ((uint8_t) 0x02)
#define FC_ERAS_CNT_100                 ((uint8_t) 0x03)
#define FC_ERAS_CNT_105                 ((uint8_t) 0x04)
#define FC_ERAS_CNT_110                 ((uint8_t) 0x05)
#define FC_ERAS_CNT_115                 ((uint8_t) 0x06)
#define FC_ERAS_CNT_120                 ((uint8_t) 0x07)
#define FC_ERAS_CNT_125                 ((uint8_t) 0x08)
#define FC_ERAS_CNT_130                 ((uint8_t) 0x09)
#define FC_ERAS_CNT_135                 ((uint8_t) 0x0A)
#define FC_ERAS_CNT_140                 ((uint8_t) 0x0B)
#define IS_FC_ERASE_CNT(param)          ((param) <= FC_ERAS_CNT_140)

#define FC_SWPSR_BIT_0                  ((uint8_t) 0x00)
#define FC_SWPSR_BIT_1                  ((uint8_t) 0x01)
#define FC_SWPSR_BIT_2                  ((uint8_t) 0x02)
#define FC_SWPSR_BIT_3                  ((uint8_t) 0x03)
#define FC_SWPSR_BIT_4                  ((uint8_t) 0x04)
#define FC_SWPSR_BIT_5                  ((uint8_t) 0x05)
#define FC_SWPSR_BIT_6                  ((uint8_t) 0x06)
#define FC_SWPSR_BIT_7                  ((uint8_t) 0x07)
#define FC_SWPSR_BIT_8                  ((uint8_t) 0x08)
#define FC_SWPSR_BIT_9                  ((uint8_t) 0x09)
#define FC_SWPSR_BIT_10                 ((uint8_t) 0x0A)
#define IS_FC_SWPSR_BIT_NUM(param)      ((param) <= FC_SWPSR_BIT_10)

#define FC_BIT_VALUE_0                  ((uint32_t)0x00000000)
#define FC_BIT_VALUE_1                  ((uint32_t)0x00000001)

#ifdef SINGLE_BOOT_MODE
#define FLASH_START_ADDR                ((uint32_t)0x5E000000)  /* SINGLE_BOOT_MODE */
#else
#define FLASH_START_ADDR                ((uint32_t)0x00000000)  /* User Boot Mode As Default */
#endif

#define FLASH_END_ADDR                  (FLASH_START_ADDR + FLASH_CHIP_SIZE - 1U)

#ifdef SINGLE_BOOT_MODE
#define IS_FC_ADDR(param)               (((param) >= FLASH_START_ADDR) && \
                                         ((param) <= FLASH_END_ADDR))
#else
#define IS_FC_ADDR(param)               ((param) <= FLASH_END_ADDR)
#endif

#define IS_FC_PAGE_ADDR(param)          ((((param) > FLASH_START_ADDR) || ((param) == FLASH_START_ADDR)) && \
                                         ((param) <= (FLASH_END_ADDR - PROGRAM_UNIT)))


/** @} */
/* End of group FC_Exported_Constants */


/** @defgroup FC_Exported_FunctionPrototypes
  * @{
  */

    void FC_SetSecurityBit(FunctionalState NewState);
    FunctionalState FC_GetSecurityBit(void);
    WorkState FC_GetBusyState(void);
    FunctionalState FC_GetBlockProtectState(uint8_t BlockNum);
    FunctionalState FC_GetPageProtectState(uint8_t PageNum);
    FunctionalState FC_GetAbortState(void);
    uint32_t FC_GetSwapSize(void);
    uint32_t FC_GetSwapState(void);
    void FC_SelectArea(uint8_t AreaNum, FunctionalState NewState);
    void FC_SetAbortion(void);
    void FC_ClearAbortion(void);
    void FC_SetClkDiv(uint8_t ClkDiv);
    void FC_SetProgramCount(uint8_t ProgramCount);
    void FC_SetEraseCounter(uint8_t EraseCounter);
    FC_Result FC_ProgramBlockProtectState(uint8_t BlockNum);
    FC_Result FC_ProgramPageProtectState(uint8_t PageNum);
    FC_Result FC_EraseProtectState(void);
    FC_Result FC_WritePage(uint32_t PageAddr, uint32_t * Data);
    FC_Result FC_EraseBlock(uint32_t BlockAddr);
    FC_Result FC_EraseArea(uint32_t AreaAddr);
    FC_Result FC_ErasePage(uint32_t PageAddr);
    FC_Result FC_EraseChip(void);
    FC_Result FC_SetSwpsrBit(uint8_t BitNum);
    uint32_t FC_GetSwpsrBitValue(uint8_t BitNum);

/** @} */
/* End of group FC_Exported_FunctionPrototypes */

/** @} */
/* End of group FC */

/** @} */
/* End of group TX04_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM46B_FC_H */
