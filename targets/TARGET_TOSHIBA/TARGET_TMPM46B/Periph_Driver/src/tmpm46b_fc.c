/**
 *******************************************************************************
 * @file    tmpm46b_fc.c
 * @brief   This file provides API functions for FC driver.
 * @version V2.0.2.1
 * @date    2015/02/27
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm46b_fc.h"

#if defined(__TMPM46B_FC_H)
/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @defgroup FC 
  * @brief FC driver modules
  * @{
  */

/** @defgroup FC_Private_Defines
  * @{
  */

#define BASE_ADDR_H                         ((uint32_t)0x00080000)

#define SECBIT_SECBIT_PASSWORD              ((uint32_t)0xA74A9D23)
#define SECBIT_SECBIT_SET                   ((uint32_t)0x00000001)
#define SECBIT_SECBIT_CLEAR                 ((uint32_t)0xFFFFFFFE)

#define SECBIT_SECURITY_MASK                ((uint32_t)0x00000001)
#define SECBIT_DISABLE_FLAG                 ((uint32_t)0x00000000)

#define FLCS_BUSY_MASK                      ((uint32_t)0x00000001)
#define FLCS_BUSY_FLAG                      ((uint32_t)0x00000000)

#define FC_PROTECT_MASK                     ((uint32_t)0x00000001)

#define FC_BLOCK_FLAG                       ((uint32_t)0x00000000)

#define FC_PAGE_FLAG                        ((uint32_t)0x00000000)

#define FC_ABORT_MASK                       ((uint32_t)0x01000000)
#define FC_ABORT_FLAG                       ((uint32_t)0x00000000)

#define FC_SWAP_SIZE_MASK                   ((uint32_t)0x00000700)

#define FC_SWAP_STATE_MASK                  ((uint32_t)0x00000003)

#define FC_SWAP_BIT_VALUE_MASK              ((uint32_t)0x00000001)

#define FC_AREA_CLEAR                       ((uint32_t)0x00000000)
#define FC_AREA_0_SET                       ((uint32_t)0x00000007)
#define FC_AREA_1_SET                       ((uint32_t)0x00000070)
#define FC_AREA_ALL_SET                     ((uint32_t)0x00000077)

#define FC_CR_ABORT_SET                     ((uint32_t)0x00000007)
#define FC_STSCLR_ABORT_CLEAR               ((uint32_t)0x00000007)

#define FLASH_AREA                          ((uint32_t)(FLASH_START_ADDR & 0xFFF00000U))

#define FC_CMD_BC1_ADDR                     ((uint32_t)(0x5E005400))
#define FC_CMD_BC2_ADDR                     ((uint32_t)(0x5E00AA00))
#define FC_CMD_BC3_ADDR                     ((uint32_t)(0x5E000000))

#define FC_CMD_BC1_ADDR_H                   ((uint32_t)(FC_CMD_BC1_ADDR + BASE_ADDR_H))
#define FC_CMD_BC2_ADDR_H                   ((uint32_t)(FC_CMD_BC2_ADDR + BASE_ADDR_H))

#define FC_ERASE_CHIP_OVER_TIME             ((uint32_t)0x005FFFFF)      /* FC_ERASE_CHIP_OVER_TIME > 345ms */

#define FC_ERASE_BLOCK_OVER_TIME            ((uint32_t)0x00CFFFFF)      /* FC_ERASE_BLOCK_OVER_TIME > 920ms */
#define FC_ERASE_PAGE_OVER_TIME             ((uint32_t)0x0016FFFF)      /* FC_ERASE_PAGE_OVER_TIME > 115ms */
#define FC_ERASE_AREA_OVER_TIME             ((uint32_t)0x0016FFFF)
#define FC_WRITE_PAGE_OVER_TIME             ((uint32_t)0x0007FFFF)      /* FC_WRITE_PAGE_OVER_TIME > 30.7ms */
#define FC_SET_PROTECT_STATE_OVER_TIME      ((uint32_t)0x000FFFFF)
#define FC_SWAP_SET_OVER_TIME               ((uint32_t)0x0016FFFF)      /* FC_SWAP_SET_OVER_TIME > 115ms */


#define FC_PBA_ADDR_SHIFT                   4U  /*PBA address shift 4 Bits */

#define FC_SWPSR_BIT_START                  ((uint32_t)0x5E000880)

#define FC_PSR0_MASK                        ((uint32_t)0xFFFFFF00)
#define FC_PSR1_PSR2_MASK                   ((uint32_t)0xFFFF0000)

#define FC_BLOCK_ADDR_MASK                  ((uint32_t)0x001FE000)
#define FC_PAGE_ADDR_MASK                   ((uint32_t)0x001FF000)

#define FC_AREA_1_START_ADDR                ((uint32_t)(FLASH_AREA + 0x00080000UL))
#define FC_PAGE_SIZE                        ((uint32_t)(FLASH_PAGE_SIZE/4U))

typedef struct {
    uint32_t Start_Addr;
    uint32_t End_Addr;
} Block_AddrDef;

Block_AddrDef block_addr[] = {
    {0U, 8U}, {8U, 16U}, {16U, 24U}, {24U, 32U}, {32U, 40U},
    {40U, 48U}, {48U, 56U}, {56U, 64U}, {64U, 72U}, {72U, 80U},
    {80U, 88U}, {88U, 96U}, {96U, 104U}, {104U, 112U}, {112U, 120U},
    {120U, 128U}, {128U, 136U}, {136U, 144U}, {144U, 152U}, {152U, 160U},
    {160U, 168U}, {168U, 176U}, {176U, 184U}, {184U, 192U}, {192U, 200U},
    {200U, 208U}, {208U, 216U}, {216U, 224U}, {224U, 232U}, {232U, 240U},
    {240U, 248U}, {248U, 256U},
};

/** @} */
/* End of group FC_Private_Defines */

/** @defgroup FC_Private_FunctionPrototypes
  * @{
  */
static uint8_t FC_AddrToBlockNum(uint32_t Addr);
static void FC_SetAreaSelection(uint8_t BlockNum);
static void FC_ClearAreaSelection(uint8_t BlockNum);
/** @} */
/* End of group FC_Private_FunctionPrototypes */

/** @defgroup FC_Private_Functions
  * @{
  */

/**
  * @brief  Convert address to block number.
  * @param  Addr
  * @retval BlockNum
   */
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static uint8_t FC_AddrToBlockNum(uint32_t Addr)
{
    uint8_t BlockNum = 0U;
    uint8_t i = 0U;
    uint32_t temp = (Addr - FLASH_START_ADDR) / FLASH_PAGE_SIZE;

    for (i = 0U; i <= FC_BLOCK_MAX; ++i) {
        if ((temp >= block_addr[i].Start_Addr) && (temp < block_addr[i].End_Addr)) {
            BlockNum = i;
            break;
        }
    }

    return BlockNum;
}

/**
  * @brief  Set an area selection bit according to block number.
  * @param  BlockNum
  * @retval None
   */
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static void FC_SetAreaSelection(uint8_t BlockNum)
{
    uint32_t tmp = 0U;
    tmp = TSB_FC->AREASEL;
    if (BlockNum <= 15U) {
        tmp |= FC_AREA_0_SET;
    } else if (BlockNum <= 31U) {
        tmp |= FC_AREA_1_SET;
    } else {
        /* Do nothing */
    }
    TSB_FC->AREASEL = tmp;
    while (TSB_FC->AREASEL != tmp) {
        /* Do nothing */
    }
}

/**
  * @brief  Clear an area selection bit according to block number.
  * @param  BlockNum
  * @retval None
   */
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static void FC_ClearAreaSelection(uint8_t BlockNum)
{
    uint32_t tmp = 0U;
    tmp = TSB_FC->AREASEL;
    if (BlockNum <= 15U) {
        tmp &= ~FC_AREA_0_SET;
    } else if (BlockNum <= 31U) {
        tmp &= ~FC_AREA_1_SET;
    } else {
        /* Do nothing */
    }
    TSB_FC->AREASEL = tmp;
    while (TSB_FC->AREASEL != tmp) {
        /* Do nothing */
    }
}

/** @} */
/* End of group FC_Private_Functions */

/** @defgroup FC_Exported_Functions
  * @{
  */

/**
  * @brief  Set the value of SECBIT register.
  * @param  NewState: The value of SECBIT register.
  *   This parameter can be one of the following values:
  *   DISABLE or ENABLE.
  * @retval None.
  * @register The used register:
  *   FCSECBIT <SECBIT>
  */
void FC_SetSecurityBit(FunctionalState NewState)
{
    uint32_t tmp = TSB_FC->SECBIT;
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        /* Set FCSECBIT<SECBIT> to "1" that enable security function  */
        tmp |= SECBIT_SECBIT_SET;
    } else {
        /* Set FCSECBIT<SECBIT> to "0" that disable security function  */
        tmp &= SECBIT_SECBIT_CLEAR;
    }
    TSB_FC->SECBIT = SECBIT_SECBIT_PASSWORD;
    TSB_FC->SECBIT = tmp;
}

/**
  * @brief  Get the value of SECBIT register.
  * @param  None.
  * @retval DISABLE or ENABLE.
  * @register The used register:
  *   FCSECBIT <SECBIT>
  */
FunctionalState FC_GetSecurityBit(void)
{
    uint32_t tmp = 0U;
    FunctionalState retval = ENABLE;

    tmp = TSB_FC->SECBIT & SECBIT_SECURITY_MASK;

    /* Check disable flag */
    if (tmp == SECBIT_DISABLE_FLAG) {
        retval = DISABLE;
    } else {                    /* Check enable flag */
        retval = ENABLE;
    }

    return retval;
}

/**
  * @brief  Get the status of the flash auto operation.
  * @param  None.
  * @retval BUSY or DONE.
  * @register The used register:
  *   FCPSR0<RDY_BSY>
  */
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
WorkState FC_GetBusyState(void)
{
    uint32_t tmp = 0U;
    WorkState retval = DONE;

    tmp = TSB_FC->PSR0 & FLCS_BUSY_MASK;

    /* Check busy flag */
    if (tmp == FLCS_BUSY_FLAG) {
        retval = BUSY;
    } else {                    /* Check ready flag */
        retval = DONE;
    }

    return retval;
}

/**
  * @brief  Get the specified block protection state.
  * @param  BlockNum: The flash block number.
  *   This parameter can be one of the following values:
  *   FC_BLOCK_1 to FC_BLOCK_31
  * @retval DISABLE or ENABLE.
  * @register The used registers:
  *   FCPSR0<31:17>
  *   FCPSR1<31:16>
  *   FCPSR2<31:16>
  */
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
FunctionalState FC_GetBlockProtectState(uint8_t BlockNum)
{
    uint32_t tmp = FC_PROTECT_MASK;
    FunctionalState retval = ENABLE;

    /* Check the parameters */
    assert_param(IS_FC_BLOCK_NUM(BlockNum));
    if (BlockNum <= FC_BLOCK_15) {
        tmp = tmp << ((uint32_t) BlockNum + 16U);
        tmp &= TSB_FC->PSR0;
    } else {
        tmp = tmp << BlockNum;
        tmp &= TSB_FC->PSR1;
    }
    if (tmp == FC_BLOCK_FLAG) {
        /* Return protection status of each block */
        retval = DISABLE;
    } else {
        retval = ENABLE;
    }

    return retval;
}

/**
  * @brief  Get the specified page protection state.
  * @param  PageNum: The flash page number.
  *   This parameter can be one of the following values:
  *   FC_PAGE_0 to FC_PAGE_7
  * @retval DISABLE or ENABLE.
  * @register The used register:
  *   FCPSR0<15:8>
  */
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
FunctionalState FC_GetPageProtectState(uint8_t PageNum)
{
    uint32_t tmp = FC_PROTECT_MASK;
    FunctionalState retval = ENABLE;

    /* Check the parameters */
    assert_param(IS_FC_PAGE_NUM(PageNum));
    tmp = tmp << ((uint32_t) PageNum + 8U);
    tmp &= TSB_FC->PSR0;

    if (tmp == FC_PAGE_FLAG) {
        /* Return protection status of each block */
        retval = DISABLE;
    } else {
        retval = ENABLE;
    }

    return retval;
}

/**
  * @brief  Get the status of the auto operation is aborted or not.
  * @param  None.
  * @retval DISABLE or ENABLE.
  *   DISABLE: Aborted is disable.
  *   ENABLE: Aborted is enable.
  * @register The used register:
  *   FCSR<WEABORT>
  */
FunctionalState FC_GetAbortState(void)
{
    uint32_t tmp = 0U;
    FunctionalState retval = DISABLE;

    tmp = TSB_FC->SR & FC_ABORT_MASK;

    /* Check  flag */
    if (tmp == FC_ABORT_FLAG) {
        retval = DISABLE;
    } else {                    /* Check flag */
        retval = ENABLE;
    }

    return retval;
}

/**
  * @brief  Get the swap size.
  * @param  None.
  * @retval the swap size.
  *   The value returned can be one of the following values:
  *   FC_SWAP_SIZE_4K, FC_SWAP_SIZE_8K, FC_SWAP_SIZE_16K,
  *   FC_SWAP_SIZE_32K, FC_SWAP_SIZE_512K
  * @register The used register:
  *   FCSWPSR <SIZE[2:0]>
  */
uint32_t FC_GetSwapSize(void)
{
    uint32_t tmp = 0U;

    tmp = TSB_FC->SWPSR & FC_SWAP_SIZE_MASK;
    tmp = (tmp >> 8U);

    return tmp;
}

/**
  * @brief  Get the swap state.
  * @param  None.
  * @retval the swap state.
  *   The value returned can be one of the following values:
  *   FC_SWAP_RELEASE, FC_SWAP_PROHIBIT,
  *   FC_SWAPPING, FC_SWAP_INITIAL.
  * @register The used register:
  *   FCSWPSR <SWP[1:0]>
  */
uint32_t FC_GetSwapState(void)
{
    uint32_t tmp = 0U;

    tmp = TSB_FC->SWPSR & FC_SWAP_STATE_MASK;

    return tmp;
}

/**
  * @brief  Specifies an "area" in the Flash memory that is targeted by Flash memory 
  *  operation command.
  * @param  AreaNum: the flash area number.
  *   This parameter can be one of the following values:
  *   FC_AREA_0, FC_AREA_1, FC_AREA_ALL.
  * @param  NewState: Specify area state.
  *   This parameter can be one of the following values:
  *   ENABLE:Select the area.
  *   DISABLE:Unselect the area.
  * @retval None
  * @register The used register:
  *   FCAREASEL <AREA0><AREA1>
  */
void FC_SelectArea(uint8_t AreaNum, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FC_AREA(AreaNum));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    switch (AreaNum) {
    case FC_AREA_0:
        (NewState == ENABLE) ? (TSB_FC->AREASEL |= FC_AREA_0_SET) : (TSB_FC->AREASEL &=
                                                                     ~FC_AREA_0_SET);
        break;
    case FC_AREA_1:
        (NewState == ENABLE) ? (TSB_FC->AREASEL |= FC_AREA_1_SET) : (TSB_FC->AREASEL &=
                                                                     ~FC_AREA_1_SET);
        break;
    case FC_AREA_ALL:
        (NewState == ENABLE) ? (TSB_FC->AREASEL |= FC_AREA_ALL_SET) : (TSB_FC->AREASEL &=
                                                                       ~FC_AREA_ALL_SET);
        break;
    default:
        /* Do nothing */
        break;
    }
}

/**
  * @brief  Set abortion of auto operation command
  * @param  None
  * @retval None
  * @register The used register:
  *   FCCR <WEABORT>
  */
void FC_SetAbortion(void)
{
    /* Set abortion of auto operation command */
    TSB_FC->CR |= FC_CR_ABORT_SET;
}

/**
  * @brief  Clear FCSR<WEABORT> to "0" command
  * @param  None
  * @retval None
  * @register The used register:
  *   FCSTSCLR <WEABORT>
  */
void FC_ClearAbortion(void)
{
    /* Clear FCSR<WEABORT> to "0" command */
    TSB_FC->STSCLR |= FC_STSCLR_ABORT_CLEAR;
}

/**
  * @brief  Set Frequency division ratio to change the clock (WCLK: fsys/(DIV+1))
  *  in automatic operation to 8 to 12MHz
  * @param  ClkDiv: the divisor of the system clock.
  *   the parameter is can be one of the following values:
  *   FC_Clk_Div_1, FC_Clk_Div_2, FC_Clk_Div_3, FC_Clk_Div_4, 
  *   FC_Clk_Div_5, FC_Clk_Div_6, FC_Clk_Div_7, FC_Clk_Div_8, 
  *   FC_Clk_Div_9, FC_Clk_Div_10, FC_Clk_Div_11, FC_Clk_Div_12, 
  *   FC_Clk_Div_13, FC_Clk_Div_14, FC_Clk_Div_15, FC_Clk_Div_16,
  *   FC_Clk_Div_17, FC_Clk_Div_18, FC_Clk_Div_19, FC_Clk_Div_20, 
  *   FC_Clk_Div_21, FC_Clk_Div_22, FC_Clk_Div_23, FC_Clk_Div_24, 
  *   FC_Clk_Div_25, FC_Clk_Div_26, FC_Clk_Div_27, FC_Clk_Div_28, 
  *   FC_Clk_Div_29, FC_Clk_Div_30, FC_Clk_Div_31, FC_Clk_Div_32.
  * @retval None
  * @register The used register:
  *   FCWCLKCR <DIV>
  */
void FC_SetClkDiv(uint8_t ClkDiv)
{
    /* Check the parameters */
    assert_param(IS_FC_WCLK_DIV(ClkDiv));

    TSB_FC->WCLKCR = (uint32_t) ClkDiv;
}

/**
  * @brief  Set the number of counts that makes a programming time (CNT/WCLK) by automatic program
  *  execution command be within the range of 20 to 40 micro-sec.
  * @param  ProgramCount: the counter of the divided system clock for flash program
  *   This parameter can be one of the following values:
  *   FC_PROG_CNT_250, FC_PROG_CNT_300, FC_PROG_CNT_350.
  * @retval None
  * @register The used register:
  *   FCPROGCR <CNT>
  */
void FC_SetProgramCount(uint8_t ProgramCount)
{
    /* Check the parameters */
    assert_param(IS_FC_PROG_CNT(ProgramCount));

    TSB_FC->PROGCR = (uint32_t) ProgramCount;
}

/**
  * @brief  Set the number of counts until erase time (CNT/WCLK) will be 100 ~ 130msec 
  *  using each auto erase command
  * @param  EraseCounter: the counter of the divided system clock for flash program
  *   This parameter can be one of the following values:
  *   FC_ERAS_CNT_85, FC_ERAS_CNT_90, FC_ERAS_CNT_95, FC_ERAS_CNT_100, 
  *   FC_ERAS_CNT_105, FC_ERAS_CNT_110, FC_ERAS_CNT_115, FC_ERAS_CNT_120, 
  *   FC_ERAS_CNT_125, FC_ERAS_CNT_130, FC_ERAS_CNT_135, FC_ERAS_CNT_140.
  * @retval None
  * @register The used register:
  *   FCERASECR <CNT>
  */
void FC_SetEraseCounter(uint8_t EraseCounter)
{
    /* Check the parameters */
    assert_param(IS_FC_ERASE_CNT(EraseCounter));

    TSB_FC->ERASECR = (uint32_t) EraseCounter;
}

/**
  * @brief  Program the protection bit to make the specified block protected.
  * @param  BlockNum: The flash block number.
  *   This parameter can be one of the following values:
  *   FC_BLOCK_1 to FC_BLOCK_31
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  * @register The used registers:
  *   FCPSR0<31:17>
  *   FCPSR1<31:16>
  *   FCPSR2<31:16>
  */
FC_Result FC_ProgramBlockProtectState(uint8_t BlockNum)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) FC_CMD_BC1_ADDR;
    volatile uint32_t *addr2 = (uint32_t *) FC_CMD_BC2_ADDR;
    volatile uint32_t *PBA;
    uint32_t counter = FC_SET_PROTECT_STATE_OVER_TIME;
    volatile uint32_t PBA_data;

    /* Check the parameters */
    assert_param(IS_FC_BLOCK_NUM(BlockNum));

    PBA_data = ((uint32_t) BlockNum + 7U) << FC_PBA_ADDR_SHIFT;

    PBA_data += (uint32_t) 0x5E000000;
    PBA = (uint32_t *) PBA_data;

    if (ENABLE == FC_GetBlockProtectState(BlockNum)) {
        retval = FC_ERROR_PROTECTED;
    } else {
        TSB_FC->AREASEL |= FC_AREA_ALL_SET;
        while (TSB_FC->AREASEL != FC_AREA_ALL_SET) {
            /* Do nothing */
        }
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x0000009A; /* bus cycle 3 */
        *PBA = (uint32_t) 0x0000009A;   /* bus cycle 4 */
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
        TSB_FC->AREASEL &= ~FC_AREA_ALL_SET;
        while (TSB_FC->AREASEL != FC_AREA_CLEAR) {
            /* Do nothing */
        }
    }

    return retval;
}

/**
  * @brief  Program the protection bit to make the specified page protected.
  * @param  PageNum: The flash page number.
  *   This parameter can be one of the following values:
  *   FC_PAGE_0 to FC_PAGE_7
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  * @register The used register:
  *   FCPSR0<15:8>
  */
FC_Result FC_ProgramPageProtectState(uint8_t PageNum)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) FC_CMD_BC1_ADDR;
    volatile uint32_t *addr2 = (uint32_t *) FC_CMD_BC2_ADDR;
    volatile uint32_t *PBA = 0U;
    uint32_t counter = FC_SET_PROTECT_STATE_OVER_TIME;
    volatile uint32_t PBA_data;

    /* Check the parameters */
    assert_param(IS_FC_PAGE_NUM(PageNum));

    PBA_data = (uint32_t) PageNum << FC_PBA_ADDR_SHIFT;

    PBA_data += (uint32_t) 0x5E000000;
    PBA = (uint32_t *) PBA_data;

    if (ENABLE == FC_GetPageProtectState(PageNum)) {
        retval = FC_ERROR_PROTECTED;
    } else {
        TSB_FC->AREASEL |= FC_AREA_ALL_SET;
        while (TSB_FC->AREASEL != FC_AREA_ALL_SET) {
            /* Do nothing */
        }
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x0000009A; /* bus cycle 3 */
        *PBA = (uint32_t) 0x0000009A;   /* bus cycle 4 */
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
        TSB_FC->AREASEL &= ~FC_AREA_ALL_SET;
        while (TSB_FC->AREASEL != FC_AREA_CLEAR) {
            /* Do nothing */
        }
    }

    return retval;
}

/**
  * @brief  Erase the protection bits to make the whole flash unprotected.
  * @param  None   
  * @retval FC_SUCCESS, FC_ERROR_OVER_TIME.
  * @register The used registers:
  *   FCPSR0<15:8><31:17>
  *   FCPSR1<31:16>
  */
FC_Result FC_EraseProtectState(void)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) FC_CMD_BC1_ADDR;
    volatile uint32_t *addr2 = (uint32_t *) FC_CMD_BC2_ADDR;
    volatile uint32_t *addr3 = (uint32_t *) FC_CMD_BC3_ADDR;
    uint32_t counter = FC_SET_PROTECT_STATE_OVER_TIME;

    TSB_FC->AREASEL |= FC_AREA_ALL_SET;
    while (TSB_FC->AREASEL != FC_AREA_ALL_SET) {
        /* Do nothing */
    }
    *addr1 = (uint32_t) 0x000000AA;     /* bus cycle 1 */
    *addr2 = (uint32_t) 0x00000055;     /* bus cycle 2 */
    *addr1 = (uint32_t) 0x0000006A;     /* bus cycle 3 */
    *addr3 = (uint32_t) 0x0000006A;     /* bus cycle 4 */
    __DSB();

    while (BUSY == FC_GetBusyState()) { /* check if FLASH is busy with overtime counter */
        if (!(counter--)) {     /* check overtime */
            retval = FC_ERROR_OVER_TIME;
            break;
        } else {
            /* Do nothing */
        }
    }
    TSB_FC->AREASEL &= ~FC_AREA_ALL_SET;
    while (TSB_FC->AREASEL != FC_AREA_CLEAR) {
        /* Do nothing */
    }

    return retval;
}

/**
  * @brief  Write data to the specified page.
  * @param  PageAddr: The page start address.
  * @param  Data: The pointer to data buffer to be written into the page.
  *   The data size should be FC_PAGE_SIZE.
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  * @register The used register:
  *   None
  */
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
FC_Result FC_WritePage(uint32_t PageAddr, uint32_t * Data)
{
    uint32_t tmpAddr = 0U;
    uint32_t i = 0U;
    uint32_t *source = Data;
    uint8_t BlockNum = 0U;
    volatile uint8_t PageNum = 0U;
    volatile uint32_t *addr1 = (uint32_t *) FC_CMD_BC1_ADDR;
    volatile uint32_t *addr2 = (uint32_t *) FC_CMD_BC2_ADDR;
    volatile uint32_t *PA = 0U;
    uint32_t counter = FC_WRITE_PAGE_OVER_TIME;
    FC_Result retval = FC_SUCCESS;
    FunctionalState state1 = DISABLE;
    FunctionalState state2 = DISABLE;

    BlockNum = FC_AddrToBlockNum(PageAddr);

    tmpAddr = (PageAddr & (uint32_t) 0x00FFFFF8) | (uint32_t) 0x5E000000;
    PA = (uint32_t *) tmpAddr;

    assert_param(IS_FC_PAGE_ADDR(PageAddr));
    assert_param(IS_POINTER_NOT_NULL(Data));

    if (BlockNum >= FC_BLOCK_16) {
        addr1 = (uint32_t *) FC_CMD_BC1_ADDR_H;
        addr2 = (uint32_t *) FC_CMD_BC2_ADDR_H;
    } else {
        /* Do nothing */
    }
    if (BlockNum == 0U) {
        PageNum = (uint8_t) ((PageAddr - FLASH_START_ADDR) / FLASH_PAGE_SIZE);
        state1 = FC_GetPageProtectState(PageNum);
    } else {
        state2 = FC_GetBlockProtectState(BlockNum);
    }
    if ((ENABLE == state1) || (ENABLE == state2)) {
        retval = FC_ERROR_PROTECTED;
    } else {
        FC_SetAreaSelection(BlockNum);
        /* page program */
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x000000A0; /* bus cycle 3 */
        for (i = 0U; i < PROGRAM_UNIT; i++) {
            *PA = *source;
            source++;
        }
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
        FC_ClearAreaSelection(BlockNum);
    }

    return retval;
}

/**
  * @brief  Erase the contents of the specified block.
  * @param  BlockAddr: The block start address.
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  * @register The used register:
  *   None
  */
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
FC_Result FC_EraseBlock(uint32_t BlockAddr)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) FC_CMD_BC1_ADDR;
    volatile uint32_t *addr2 = (uint32_t *) FC_CMD_BC2_ADDR;
    volatile uint32_t *BA = 0U;
    uint32_t counter = FC_ERASE_BLOCK_OVER_TIME;
    uint8_t BlockNum = 0U;
    FunctionalState state1 = DISABLE;

    BlockNum = FC_AddrToBlockNum(BlockAddr);

    assert_param(IS_FC_ADDR(BlockAddr));

    if (BlockNum >= FC_BLOCK_16) {
        addr1 = (uint32_t *) FC_CMD_BC1_ADDR_H;
        addr2 = (uint32_t *) FC_CMD_BC2_ADDR_H;
    } else {
        /* Do nothing */
    }
    BlockAddr &= FC_BLOCK_ADDR_MASK;
    BlockAddr |= (uint32_t) 0x5E000000;

    BA = (uint32_t *) BlockAddr;
    if (BlockNum >= FC_BLOCK_1) {
        state1 = FC_GetBlockProtectState(BlockNum);
    } else {
        state1 = FC_GetPageProtectState(FC_PAGE_0);
        state1 |= FC_GetPageProtectState(FC_PAGE_1);
        state1 |= FC_GetPageProtectState(FC_PAGE_2);
        state1 |= FC_GetPageProtectState(FC_PAGE_3);
        state1 |= FC_GetPageProtectState(FC_PAGE_4);
        state1 |= FC_GetPageProtectState(FC_PAGE_5);
        state1 |= FC_GetPageProtectState(FC_PAGE_6);
        state1 |= FC_GetPageProtectState(FC_PAGE_7);
    }
    if (ENABLE == state1) {
        retval = FC_ERROR_PROTECTED;
    } else {
        FC_SetAreaSelection(BlockNum);
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x00000080; /* bus cycle 3 */
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 4 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 5 */
        *BA = (uint32_t) 0x00000030;    /* bus cycle 6 */
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
        FC_ClearAreaSelection(BlockNum);
    }

    return retval;
}

/**
  * @brief  Erase the contents of the specified area.
  * @param  AreaAddr: The area start address.
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  * @register The used register:
  *   None
  */
FC_Result FC_EraseArea(uint32_t AreaAddr)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) FC_CMD_BC1_ADDR;
    volatile uint32_t *addr2 = (uint32_t *) FC_CMD_BC2_ADDR;
    volatile uint32_t *AA = 0U;
    uint32_t tmp = 0U;
    uint8_t BlockNum = 0U;
    uint32_t counter = FC_ERASE_AREA_OVER_TIME;
    BlockNum = FC_AddrToBlockNum(AreaAddr);

    assert_param(IS_FC_ADDR(AreaAddr)); /* Check whether it is in the flash address range */

    if (BlockNum >= FC_BLOCK_16) {
        addr1 = (uint32_t *) FC_CMD_BC1_ADDR_H;
        addr2 = (uint32_t *) FC_CMD_BC2_ADDR_H;
    } else {
        /* Do nothing */
    }

    if (BlockNum < FC_BLOCK_16) {
        AA = (uint32_t *) 0x5E000000;
        tmp = TSB_FC->PSR0;
        tmp &= FC_PSR0_MASK;
    } else {
        AA = (uint32_t *) (FC_AREA_1_START_ADDR | (uint32_t) 0x5E000000);
        tmp = TSB_FC->PSR1;
        tmp &= FC_PSR1_PSR2_MASK;
    }

    if (tmp) {
        retval = FC_ERROR_PROTECTED;
    } else {
        FC_SetAreaSelection(BlockNum);
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x00000080; /* bus cycle 3 */
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 4 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 5 */
        *AA = (uint32_t) 0x00000020;    /* bus cycle 6 */
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
        FC_ClearAreaSelection(BlockNum);
    }

    return retval;
}

/**
  * @brief  Erase the contents of the specified page.
  * @param  PageAddr: The page start address.
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  * @register The used register:
  *   None
  */
FC_Result FC_ErasePage(uint32_t PageAddr)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) FC_CMD_BC1_ADDR;
    volatile uint32_t *addr2 = (uint32_t *) FC_CMD_BC2_ADDR;
    volatile uint32_t *PGA = 0U;
    uint32_t counter = FC_ERASE_PAGE_OVER_TIME;
    uint8_t BlockNum = 0U;
    uint8_t PageNum = 0U;
    FunctionalState state1 = DISABLE;
    FunctionalState state2 = DISABLE;
    BlockNum = FC_AddrToBlockNum(PageAddr);

    assert_param(IS_FC_PAGE_ADDR(PageAddr));

    if (BlockNum >= FC_BLOCK_16) {
        addr1 = (uint32_t *) FC_CMD_BC1_ADDR_H;
        addr2 = (uint32_t *) FC_CMD_BC2_ADDR_H;
    } else {
        /* Do nothing */
    }

    PageAddr &= FC_PAGE_ADDR_MASK;
    PageAddr |= (uint32_t) 0x5E000000;

    PGA = (uint32_t *) PageAddr;
    PageNum = (uint8_t) ((PageAddr - FLASH_START_ADDR) / FLASH_PAGE_SIZE);
    if (BlockNum == 0U) {
        state1 = FC_GetPageProtectState(PageNum);
    } else {
        state2 = FC_GetBlockProtectState(BlockNum);
    }
    if ((ENABLE == state1) || (ENABLE == state2)) {
        retval = FC_ERROR_PROTECTED;
    } else {
        FC_SetAreaSelection(BlockNum);
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x00000080; /* bus cycle 3 */
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 4 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 5 */
        *PGA = (uint32_t) 0x00000040;   /* bus cycle 6 */
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
        FC_ClearAreaSelection(BlockNum);
    }

    return retval;
}

/**
  * @brief  Erase the contents of the entire chip.
  * @param  None.
  * @retval FC_SUCCESS, FC_ERROR_PROTECTED, FC_ERROR_OVER_TIME.
  * @register The used register:
  *   None
  */
FC_Result FC_EraseChip(void)
{
    FC_Result retval = FC_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *) FC_CMD_BC1_ADDR;
    volatile uint32_t *addr2 = (uint32_t *) FC_CMD_BC2_ADDR;
    uint32_t counter = FC_ERASE_CHIP_OVER_TIME;
    FunctionalState result;
    uint8_t PageNum = FC_PAGE_0;
    uint8_t BlockNum = FC_BLOCK_1;
    for (PageNum = FC_PAGE_0; PageNum <= FC_PAGE_MAX; ++PageNum) {
        result = FC_GetPageProtectState(PageNum);
        if (result == ENABLE) {
            break;
        } else {
            /* Do nothing */
        }
    }

    for (BlockNum = FC_BLOCK_1; BlockNum <= FC_BLOCK_MAX; ++BlockNum) {
        if (result == ENABLE) {
            break;
        } else {
            result = FC_GetBlockProtectState(BlockNum);
        }
    }

    if (ENABLE == result) {
        retval = FC_ERROR_PROTECTED;
    } else {
        TSB_FC->AREASEL |= FC_AREA_ALL_SET;
        while (TSB_FC->AREASEL != FC_AREA_ALL_SET) {
            /* Do nothing */
        }
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 1 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 2 */
        *addr1 = (uint32_t) 0x00000080; /* bus cycle 3 */
        *addr1 = (uint32_t) 0x000000AA; /* bus cycle 4 */
        *addr2 = (uint32_t) 0x00000055; /* bus cycle 5 */
        *addr1 = (uint32_t) 0x00000010; /* bus cycle 6 */
        __DSB();

        while (BUSY == FC_GetBusyState()) {     /* check if FLASH is busy with overtime counter */
            if (!(counter--)) { /* check overtime */
                retval = FC_ERROR_OVER_TIME;
                break;
            } else {
                /* Do nothing */
            }
        }
        TSB_FC->AREASEL &= ~FC_AREA_ALL_SET;
        while (TSB_FC->AREASEL != FC_AREA_CLEAR) {
            /* Do nothing */
        }
    }

    return retval;
}

/**
  * @brief  Setting values of FCSWPSR[10:0] by memory swap command.
  * @param  BitNum: The FCSWPSR bit number to be set.
  *   This parameter can be one of the following values:
  *   FC_SWPSR_BIT_0 to FC_SWPSR_BIT_10
  * @retval FC_SUCCESS, FC_ERROR_OVER_TIME.
  * @register The used register:
  *   FCSWPSR<SIZE><FLG><SWP>
  */
FC_Result FC_SetSwpsrBit(uint8_t BitNum)
{
    FC_Result retval = FC_SUCCESS;
    uint32_t counter = FC_SWAP_SET_OVER_TIME;
    volatile uint32_t *addr1 = (uint32_t *) FC_CMD_BC1_ADDR;
    volatile uint32_t *addr2 = (uint32_t *) FC_CMD_BC2_ADDR;
    volatile uint32_t *MSA = 0U;
    uint32_t MSA_data;

    /* Check the parameters */
    assert_param(IS_FC_SWPSR_BIT_NUM(BitNum));

    MSA_data = (uint32_t) BitNum << FC_PBA_ADDR_SHIFT;
    MSA_data += FC_SWPSR_BIT_START;
    MSA = (uint32_t *) MSA_data;

    TSB_FC->AREASEL |= FC_AREA_ALL_SET;
    while (TSB_FC->AREASEL != FC_AREA_ALL_SET) {
        /* Do nothing */
    }
    *addr1 = (uint32_t) 0x000000AA;     /* bus cycle 1 */
    *addr2 = (uint32_t) 0x00000055;     /* bus cycle 2 */
    *addr1 = (uint32_t) 0x0000009A;     /* bus cycle 3 */
    *MSA = (uint32_t) 0x0000009A;       /* bus cycle 4 */
    __DSB();
    while (BUSY == FC_GetBusyState()) { /* check if FLASH is busy with overtime counter */
        if (!(counter--)) {     /* check overtime */
            retval = FC_ERROR_OVER_TIME;
            break;
        } else {
            /* Do nothing */
        }
    }
    TSB_FC->AREASEL &= ~FC_AREA_ALL_SET;
    while (TSB_FC->AREASEL != FC_AREA_CLEAR) {
        /* Do nothing */
    }

    return retval;
}

/**
* @brief  Get the value of the special bit of FCSWPSR[10:0].
  * @param  BitNum: The special bit of SWPSR.
  *   This parameter can be one of the following values:
  *   FC_SWPSR_BIT_0 to FC_SWPSR_BIT_10.
  * @retval The value of the special bit.
  *   The value returned can be one of the following values:
  *   FC_BIT_VALUE_0, FC_BIT_VALUE_1.
  * @register The used register:
  *   FCSWPSR<SIZE><FLG><SWP>
  */
uint32_t FC_GetSwpsrBitValue(uint8_t BitNum)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_FC_SWPSR_BIT_NUM(BitNum));

    tmp = TSB_FC->SWPSR;
    tmp = (tmp >> (uint32_t) BitNum);
    tmp &= FC_SWAP_BIT_VALUE_MASK;

    return tmp;
}

/** @} */
/* End of group FC_Exported_Functions */

/** @} */
/* End of group FC */

/** @} */
/* End of group TX04_Periph_Driver */

#endif                          /*(__TMPM46B_FC_H) */
