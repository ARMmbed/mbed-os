/**************************************************************************//**
 * @file     tamper.c
 * @version  V3.00
 * @brief    M2354 series Tamper driver source file
 *
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "NuMicro.h"


/** @cond HIDDEN_SYMBOLS */


/** @endcond HIDDEN_SYMBOLS */


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup Tamper_Driver Tamper Driver
  @{
*/

/** @addtogroup Tamper_EXPORTED_FUNCTIONS Tamper Exported Functions
  @{
*/

/**
  * @brief      Enable Tamper Interrupt
  *
  * @param[in]  u32IntFlagMask      Specify the interrupt source. It consists of:
  *                                     - \ref TAMP_INTEVEN_TAMP0EN_Msk   : Tamper 0 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP1EN_Msk   : Tamper 1 or Pair 0 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP2EN_Msk   : Tamper 2 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP3EN_Msk   : Tamper 3 or Pair 1 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP4EN_Msk   : Tamper 4 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP5EN_Msk   : Tamper 5 or Pair 2 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_CLKFEN_Msk    : LXT Clock Frequency Monitor Fail interrupt
  *                                     - \ref TAMP_INTEVEN_CLKSTOPEN_Msk : LXT Clock Frequency Monitor Stop interrupt
  *                                     - \ref TAMP_INTEVEN_OVPEN_Msk     : Core_SB Over Voltage Detection interrupt
  *                                     - \ref TAMP_INTEVEN_PWRPEN_Msk    : Core Power Postive Glitch Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_PWRNEN_Msk    : Core Power Negative Glitch Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_ACTSEN_Msk    : Active Shiled including VDD, GND and Tamper Pin Event interrupt
  *                                     - \ref TAMP_INTEVEN_RTCLVREN_Msk  : RTC Low Voltage Detection interrupt
  *                                     - \ref TAMP_INTEVEN_RTCIOEN_Msk   : RTC Tamper I/O Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_RTCLKEN_Msk   : RTC Clock Monitor Fail or Stop interrupt
  *                                     - \ref TAMP_INTEVEN_HXTERREN_Msk  : Core HXT Error interrupt
  *                                     - \ref TAMP_INTEVEN_VLOSSEN_Msk   : VBAT Power Loss interrupt
  *                                     - \ref TAMP_INTEVEN_WDTEN_Msk     : Watch Dog Event interrupt
  *                                     - \ref TAMP_INTEVEN_BODEN_Msk     : Brown Out Event interrupt
  *
  * @return     None
  *
  * @details    This API is used to enable the specify Tamper interrupt function.
  */
void TAMP_EnableInt(uint32_t u32IntFlagMask)
{
    TAMPER->INTEVEN |= u32IntFlagMask;
}

/**
  * @brief      Disable Tamper Interrupt
  *
  * @param[in]  u32IntFlagMask      Specify the interrupt source. It consists of:
  *                                     - \ref TAMP_INTEVEN_TAMP0EN_Msk   : Tamper 0 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP1EN_Msk   : Tamper 1 or Pair 0 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP2EN_Msk   : Tamper 2 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP3EN_Msk   : Tamper 3 or Pair 1 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP4EN_Msk   : Tamper 4 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_TAMP5EN_Msk   : Tamper 5 or Pair 2 Pin Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_CLKFEN_Msk    : LXT Clock Frequency Monitor Fail interrupt
  *                                     - \ref TAMP_INTEVEN_CLKSTOPEN_Msk : LXT Clock Frequency Monitor Stop interrupt
  *                                     - \ref TAMP_INTEVEN_OVPEN_Msk     : Core_SB Over Voltage Detection interrupt
  *                                     - \ref TAMP_INTEVEN_PWRPEN_Msk    : Core Power Postive Glitch Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_PWRNEN_Msk    : Core Power Negative Glitch Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_ACTSEN_Msk    : Active Shiled including VDD, GND and Tamper Pin Event interrupt
  *                                     - \ref TAMP_INTEVEN_RTCLVREN_Msk  : RTC Low Voltage Detection interrupt
  *                                     - \ref TAMP_INTEVEN_RTCIOEN_Msk   : RTC Tamper I/O Event Detection interrupt
  *                                     - \ref TAMP_INTEVEN_RTCLKEN_Msk   : RTC Clock Monitor Fail or Stop interrupt
  *                                     - \ref TAMP_INTEVEN_HXTERREN_Msk  : Core HXT Error interrupt
  *                                     - \ref TAMP_INTEVEN_VLOSSEN_Msk   : VBAT Power Loss interrupt
  *                                     - \ref TAMP_INTEVEN_WDTEN_Msk     : Watch Dog Event interrupt
  *                                     - \ref TAMP_INTEVEN_BODEN_Msk     : Brown Out Event interrupt
  *
  * @return     None
  *
  * @details    This API is used to disable the specify Tamper interrupt function.
  */
void TAMP_DisableInt(uint32_t u32IntFlagMask)
{
    TAMPER->INTEVEN &= ~u32IntFlagMask;
}

/**
  * @brief      Static Tamper Detect
  *
  * @param[in]  u32TamperSelect     Tamper pin select. Possible options are
  *                                 - \ref TAMP_TAMPER0_SELECT
  *                                 - \ref TAMP_TAMPER1_SELECT
  *                                 - \ref TAMP_TAMPER2_SELECT
  *                                 - \ref TAMP_TAMPER3_SELECT
  *                                 - \ref TAMP_TAMPER4_SELECT
  *                                 - \ref TAMP_TAMPER5_SELECT
  *
  * @param[in]  u32DetecLevel       Tamper pin detection level select. Possible options are
  *                                 - \ref TAMP_TAMPER_HIGH_LEVEL_DETECT
  *                                 - \ref TAMP_TAMPER_LOW_LEVEL_DETECT
  *
  * @param[in]  u32DebounceEn       Tamper pin de-bounce enable
  *                                 - \ref TAMP_TAMPER_DEBOUNCE_ENABLE
  *                                 - \ref TAMP_TAMPER_DEBOUNCE_DISABLE
  *
  * @return     None
  *
  * @details    This API is used to enable the tamper pin detect function with specify trigger condition.
  *             User needs to disable dynamic tamper function before use this API.
  */
void TAMP_StaticTamperEnable(uint32_t u32TamperSelect, uint32_t u32DetecLevel, uint32_t u32DebounceEn)
{
    uint32_t i;
    uint32_t u32Reg;
    uint32_t u32TmpReg;

    u32Reg = TAMPER->TIOCTL;

    u32TmpReg = (TAMP_TIOCTL_TAMP0EN_Msk | (u32DetecLevel << TAMP_TIOCTL_TAMP0LV_Pos) |
                 (u32DebounceEn << TAMP_TIOCTL_TAMP0DBEN_Pos));

    for(i = 0UL; i < (uint32_t)MAX_TAMPER_PIN_NUM; i++)
    {
        if(u32TamperSelect & (0x1UL << i))
        {
            u32Reg &= ~((TAMP_TIOCTL_TAMP0EN_Msk | TAMP_TIOCTL_TAMP0LV_Msk | TAMP_TIOCTL_TAMP0DBEN_Msk) << (i * 4UL));
            u32Reg |= (u32TmpReg << (i * 4UL));
        }
    }

    TAMPER->TIOCTL = u32Reg;
}

/**
  * @brief      Static Tamper Disable
  *
  * @param[in]  u32TamperSelect     Tamper pin select. Possible options are
  *                                 - \ref TAMP_TAMPER0_SELECT
  *                                 - \ref TAMP_TAMPER1_SELECT
  *                                 - \ref TAMP_TAMPER2_SELECT
  *                                 - \ref TAMP_TAMPER3_SELECT
  *                                 - \ref TAMP_TAMPER4_SELECT
  *                                 - \ref TAMP_TAMPER5_SELECT
  *
  * @return     None
  *
  * @details    This API is used to disable the static tamper pin detect.
  */
void TAMP_StaticTamperDisable(uint32_t u32TamperSelect)
{
    uint32_t i;
    uint32_t u32Reg;
    uint32_t u32TmpReg;

    u32Reg = TAMPER->TIOCTL;

    u32TmpReg = TAMP_TIOCTL_TAMP0EN_Msk;

    for(i = 0UL; i < (uint32_t)MAX_TAMPER_PIN_NUM; i++)
    {
        if(u32TamperSelect & (0x1UL << i))
        {
            u32Reg &= ~(u32TmpReg << (i * 4UL));
        }
    }

    TAMPER->TIOCTL = u32Reg;
}

/**
  * @brief      Dynamic Tamper Detect
  *
  * @param[in]  u32PairSel          Tamper pin detection enable. Possible options are
  *                                 - \ref TAMP_PAIR0_SELECT
  *                                 - \ref TAMP_PAIR1_SELECT
  *                                 - \ref TAMP_PAIR2_SELECT
  *
  * @param[in]  u32DebounceEn       Tamper pin de-bounce enable
  *                                 - \ref TAMP_TAMPER_DEBOUNCE_ENABLE
  *                                 - \ref TAMP_TAMPER_DEBOUNCE_DISABLE
  *
  *  @param[in]  u32Pair1Source     Dynamic pair 1 input source select
  *                                 0: Pair 1 source select tamper 2
  *                                 1: Pair 1 source select tamper 0
  *
  *  @param[in]  u32Pair2Source     Dynamic pair 2 input source select
  *                                 0: Pair 2 source select tamper 4
  *                                 1: Pair 2 source select tamper 0
  *
  * @return     None
  *
  * @details    This API is used to enable the dynamic tamper.
  */
void TAMP_DynamicTamperEnable(uint32_t u32PairSel, uint32_t u32DebounceEn, uint32_t u32Pair1Source, uint32_t u32Pair2Source)
{
    uint32_t i;
    uint32_t u32Reg;
    uint32_t u32TmpReg;
    uint32_t u32Tamper2Debounce, u32Tamper4Debounce;

    u32Reg = TAMPER->TIOCTL;
    u32Reg &= ~(TAMP_TIOCTL_TAMP0EN_Msk | TAMP_TIOCTL_TAMP1EN_Msk | TAMP_TIOCTL_TAMP2EN_Msk |
                TAMP_TIOCTL_TAMP3EN_Msk | TAMP_TIOCTL_TAMP4EN_Msk | TAMP_TIOCTL_TAMP5EN_Msk);

    u32Tamper2Debounce = u32Reg & TAMP_TIOCTL_TAMP2DBEN_Msk;
    u32Tamper4Debounce = u32Reg & TAMP_TIOCTL_TAMP4DBEN_Msk;

    u32Reg &= ~(TAMP_TIOCTL_TAMP0EN_Msk | TAMP_TIOCTL_TAMP1EN_Msk | TAMP_TIOCTL_TAMP2EN_Msk |
                TAMP_TIOCTL_TAMP3EN_Msk | TAMP_TIOCTL_TAMP4EN_Msk | TAMP_TIOCTL_TAMP5EN_Msk);
    u32Reg &= ~(TAMP_TIOCTL_DYN1ISS_Msk | TAMP_TIOCTL_DYN2ISS_Msk);
    u32Reg |= ((u32Pair1Source & 0x1UL) << TAMP_TIOCTL_DYN1ISS_Pos) | ((u32Pair2Source & 0x1UL) << TAMP_TIOCTL_DYN2ISS_Pos);

    if(u32DebounceEn)
    {
        u32TmpReg = (TAMP_TIOCTL_TAMP0EN_Msk | TAMP_TIOCTL_TAMP1EN_Msk |
                     TAMP_TIOCTL_TAMP0DBEN_Msk | TAMP_TIOCTL_TAMP1DBEN_Msk | TAMP_TIOCTL_DYNPR0EN_Msk);
    }
    else
    {
        u32TmpReg = (TAMP_TIOCTL_TAMP0EN_Msk | TAMP_TIOCTL_TAMP1EN_Msk | TAMP_TIOCTL_DYNPR0EN_Msk);
    }

    for(i = 0UL; i < (uint32_t)MAX_PAIR_NUM; i++)
    {
        if(u32PairSel & (0x1UL << i))
        {
            u32Reg &= ~((TAMP_TIOCTL_TAMP0DBEN_Msk | TAMP_TIOCTL_TAMP1DBEN_Msk) << (i * 8UL));
            u32Reg |= (u32TmpReg << (i * 8UL));
        }
    }

    if((u32Pair1Source) && (u32PairSel & (uint32_t)TAMP_PAIR1_SELECT))
    {
        u32Reg &= ~TAMP_TIOCTL_TAMP2EN_Msk;
        u32Reg |= u32Tamper2Debounce;
    }

    if((u32Pair2Source) && (u32PairSel & (uint32_t)TAMP_PAIR2_SELECT))
    {
        u32Reg &= ~TAMP_TIOCTL_TAMP4EN_Msk;
        u32Reg |= u32Tamper4Debounce;
    }

    TAMPER->TIOCTL = u32Reg;
}

/**
  * @brief      Dynamic Tamper Disable
  *
  * @param[in]  u32PairSel          Tamper pin detection disable. Possible options are
  *                                 - \ref TAMP_PAIR0_SELECT
  *                                 - \ref TAMP_PAIR1_SELECT
  *                                 - \ref TAMP_PAIR2_SELECT
  *
  * @return     None
  *
  * @details    This API is used to disable the dynamic tamper.
  */
void TAMP_DynamicTamperDisable(uint32_t u32PairSel)
{
    uint32_t i;
    uint32_t u32Reg;
    uint32_t u32TmpReg;
    uint32_t u32Tamper2En = 0UL, u32Tamper4En = 0UL;

    u32Reg = TAMPER->TIOCTL;

    if((u32Reg & (uint32_t)TAMP_TIOCTL_DYN1ISS_Msk) && (u32PairSel & (uint32_t)TAMP_PAIR1_SELECT))
    {
        u32Tamper2En = u32Reg & TAMP_TIOCTL_TAMP2EN_Msk;
    }

    if((u32Reg & (uint32_t)TAMP_TIOCTL_DYN2ISS_Msk) && (u32PairSel & (uint32_t)TAMP_PAIR2_SELECT))
    {
        u32Tamper4En = u32Reg & TAMP_TIOCTL_TAMP4EN_Msk;
    }

    u32TmpReg = (TAMP_TIOCTL_TAMP0EN_Msk | TAMP_TIOCTL_TAMP1EN_Msk | TAMP_TIOCTL_DYNPR0EN_Msk);

    for(i = 0UL; i < (uint32_t)MAX_PAIR_NUM; i++)
    {
        if(u32PairSel & (0x1UL << i))
        {
            u32Reg &= ~(u32TmpReg << ((i * 8UL)));
        }
    }

    u32Reg |= (u32Tamper2En | u32Tamper4En);

    TAMPER->TIOCTL = u32Reg;
}

/**
  * @brief      Config Dynamic Tamper
  *
  * @param[in]  u32ChangeRate       The dynamic tamper output change rate
  *                                 - \ref TAMP_2POW6_CLK
  *                                 - \ref TAMP_2POW7_CLK
  *                                 - \ref TAMP_2POW8_CLK
  *                                 - \ref TAMP_2POW9_CLK
  *                                 - \ref TAMP_2POW10_CLK
  *                                 - \ref TAMP_2POW11_CLK
  *                                 - \ref TAMP_2POW12_CLK
  *                                 - \ref TAMP_2POW13_CLK
  *
  * @param[in]  u32SeedReload       Reload new seed or not
  *                                 0: not reload new seed
  *                                 1: reload new seed
  *
  * @param[in]  u32RefPattern       Reference pattern
  *                                 - \ref REF_RANDOM_PATTERN
  *                                 - \ref REF_SEED
  *
  * @param[in]  u32Seed             Seed Value (0x0 ~ 0xFFFFFFFF)
  *
  * @return     None
  *
  * @details    This API is used to config dynamic tamper setting.
  */
void TAMP_DynamicTamperConfig(uint32_t u32ChangeRate, uint32_t u32SeedReload, uint32_t u32RefPattern, uint32_t u32Seed)
{
    uint32_t u32Reg;

    u32Reg = TAMPER->TIOCTL;

    u32Reg &= ~(TAMP_TIOCTL_DYNSRC_Msk | TAMP_TIOCTL_SEEDRLD_Msk | TAMP_TIOCTL_DYNRATE_Msk);

    u32Reg |= (u32ChangeRate) | ((u32SeedReload & 0x1UL) << TAMP_TIOCTL_SEEDRLD_Pos) |
              ((u32RefPattern & 0x3UL) << TAMP_TIOCTL_DYNSRC_Pos);

    TAMPER->SEED = u32Seed; /* Need to set seed value before re-loade seed */
    TAMPER->TIOCTL = u32Reg;
}




/**
  * @brief      Active Shield Dynamic Tamper Detect
  *
  * @param[in]  u32PairSel1         Active shield 1 tamper pin detection enable. Possible options are
  *                                 - \ref TAMP_PAIR0_SELECT
  *                                 - \ref TAMP_PAIR1_SELECT
  *
  *  @param[in]  u32Pair1Source1    Active shield 1 dynamic pair 1 input source select
  *                                 0: Pair 1 source select tamper 2
  *                                 1: Pair 1 source select tamper 0
  *
  * @param[in]  u32PairSel2         Active shield 2 tamper pin detection enable. Possible options are
  *                                 - \ref TAMP_PAIR0_SELECT
  *                                 - \ref TAMP_PAIR1_SELECT
  *
  *  @param[in]  u32Pair1Source2    Active shield 2 dynamic pair 1 input source select
  *                                 0: Pair 1 source select tamper 2
  *                                 1: Pair 1 source select tamper 0
  *
  * @return     None
  *
  * @details    This API is used to enable the active shield dynamic tamper.
  */
void TAMP_ActiveShieldDynamicTamperEnable(uint32_t u32PairSel1, uint32_t u32Pair1Source1, uint32_t u32PairSel2, uint32_t u32Pair1Source2)
{
    uint32_t i;
    uint32_t u32Reg1, u32Reg2;
    uint32_t u32TmpReg1, u32TmpReg2;

    u32Reg1 = TAMPER->ACTSTIOCTL1;
    u32Reg1 &= ~(TAMP_ACTSTIOCTL1_ATAMP0EN_Msk | TAMP_ACTSTIOCTL1_ATAMP1EN_Msk | TAMP_ACTSTIOCTL1_ATAMP2EN_Msk |
                 TAMP_ACTSTIOCTL1_ATAMP3EN_Msk);
    u32Reg2 = TAMPER->ACTSTIOCTL2;
    u32Reg2 &= ~(TAMP_ACTSTIOCTL2_ATAMP0EN2_Msk | TAMP_ACTSTIOCTL2_ATAMP1EN2_Msk | TAMP_ACTSTIOCTL2_ATAMP2EN2_Msk |
                 TAMP_ACTSTIOCTL2_ATAMP3EN2_Msk);

    u32Reg1 &= ~(TAMP_ACTSTIOCTL1_ADYN1ISS_Msk);
    u32Reg1 |= ((u32Pair1Source1 & 0x1UL) << TAMP_ACTSTIOCTL1_ADYN1ISS_Pos);
    u32Reg2 &= ~(TAMP_ACTSTIOCTL2_ADYN1ISS2_Msk);
    u32Reg2 |= ((u32Pair1Source2 & 0x1UL) << TAMP_ACTSTIOCTL2_ADYN1ISS2_Pos);

    u32TmpReg1 = (TAMP_ACTSTIOCTL1_ATAMP0EN_Msk | TAMP_ACTSTIOCTL1_ATAMP1EN_Msk | TAMP_ACTSTIOCTL1_ADYNPR0EN_Msk);
    u32TmpReg2 = (TAMP_ACTSTIOCTL2_ATAMP0EN2_Msk | TAMP_ACTSTIOCTL2_ATAMP1EN2_Msk | TAMP_ACTSTIOCTL2_ADYNPR0EN2_Msk);

    for(i = 0UL; i < (uint32_t)ACTS_MAX_PAIR_NUM; i++)
    {
        if(u32PairSel1 & (0x1UL << i))
        {
            u32Reg1 |= (u32TmpReg1 << (i * 8UL));
        }

        if(u32PairSel2 & (0x1UL << i))
        {
            u32Reg2 |= (u32TmpReg2 << (i * 8UL));
        }
    }

    if((u32Pair1Source1) && (u32PairSel1 & (uint32_t)TAMP_PAIR1_SELECT))
    {
        u32Reg1 &= ~TAMP_ACTSTIOCTL1_ATAMP2EN_Msk;
    }

    if((u32Pair1Source2) && (u32PairSel2 & (uint32_t)TAMP_PAIR1_SELECT))
    {
        u32Reg2 &= ~TAMP_ACTSTIOCTL2_ATAMP2EN2_Msk;
    }

    TAMPER->ACTSTIOCTL1 = u32Reg1;
    TAMPER->ACTSTIOCTL2 = u32Reg2;
}

/**
  * @brief      Active Shield Dynamic Tamper Disable
  *
  * @param[in]  u32PairSel1          Active shield 1 tamper pin detection disable. Possible options are
  *                                 - \ref TAMP_PAIR0_SELECT
  *                                 - \ref TAMP_PAIR1_SELECT
  *
  * @param[in]  u32PairSel2          Active shield 2 tamper pin detection disable. Possible options are
  *                                 - \ref TAMP_PAIR0_SELECT
  *                                 - \ref TAMP_PAIR1_SELECT
  *
  * @return     None
  *
  * @details    This API is used to disable the active shield dynamic tamper.
  */
void TAMP_ActiveShieldDynamicTamperDisable(uint32_t u32PairSel1, uint32_t u32PairSel2)
{
    uint32_t i;
    uint32_t u32Reg1, u32Reg2;
    uint32_t u32TmpReg1, u32TmpReg2;
    uint32_t u32Tamper2En1 = 0UL, u32Tamper2En2 = 0UL;

    u32Reg1 = TAMPER->ACTSTIOCTL1;
    u32Reg2 = TAMPER->ACTSTIOCTL2;

    if((u32Reg1 & (uint32_t)TAMP_ACTSTIOCTL1_ADYN1ISS_Msk) && (u32PairSel1 & (uint32_t)TAMP_PAIR1_SELECT))
    {
        u32Tamper2En1 = u32Reg1 & TAMP_ACTSTIOCTL1_ATAMP2EN_Msk;
    }

    if((u32Reg2 & (uint32_t)TAMP_ACTSTIOCTL2_ADYN1ISS2_Msk) && (u32PairSel2 & (uint32_t)TAMP_PAIR1_SELECT))
    {
        u32Tamper2En2 = u32Reg2 & TAMP_ACTSTIOCTL2_ATAMP2EN2_Msk;
    }

    u32TmpReg1 = (TAMP_ACTSTIOCTL1_ATAMP0EN_Msk | TAMP_ACTSTIOCTL1_ATAMP1EN_Msk | TAMP_ACTSTIOCTL1_ADYNPR0EN_Msk);
    u32TmpReg2 = (TAMP_ACTSTIOCTL2_ATAMP0EN2_Msk | TAMP_ACTSTIOCTL2_ATAMP1EN2_Msk | TAMP_ACTSTIOCTL2_ADYNPR0EN2_Msk);

    for(i = 0UL; i < (uint32_t)MAX_PAIR_NUM; i++)
    {
        if(u32PairSel1 & (0x1UL << i))
        {
            u32Reg1 &= ~(u32TmpReg1 << ((i * 8UL)));
        }

        if(u32PairSel2 & (0x1UL << i))
        {
            u32Reg2 &= ~(u32TmpReg2 << ((i * 8UL)));
        }
    }

    u32Reg1 |= u32Tamper2En1;
    u32Reg2 |= u32Tamper2En2;

    TAMPER->ACTSTIOCTL1 = u32Reg1;
    TAMPER->ACTSTIOCTL2 = u32Reg2;
}

/**
  * @brief      Config Active Shield Dynamic Tamper
  *
  * @param[in]  u32ChangeRate1/2    The dynamic tamper output change rate
  *                                 - \ref TAMP_ACTS_2POW6_CLK
  *                                 - \ref TAMP_ACTS_2POW7_CLK
  *                                 - \ref TAMP_ACTS_2POW8_CLK
  *                                 - \ref TAMP_ACTS_2POW9_CLK
  *                                 - \ref TAMP_ACTS_2POW10_CLK
  *                                 - \ref TAMP_ACTS_2POW11_CLK
  *                                 - \ref TAMP_ACTS_2POW12_CLK
  *                                 - \ref TAMP_ACTS_2POW13_CLK
  *
  * @param[in]  u32SeedReload1/2    Reload new seed or not
  *                                 0: not reload new seed
  *                                 1: reload new seed
  *
  * @param[in]  u32RefPattern1/2    Reference pattern
  *                                 - \ref REF_RANDOM_PATTERN
  *                                 - \ref REF_SEED
  *
  * @param[in]  u32Seed/2           Seed Value (0x0 ~ 0xFFFFFFFF)
  *
  * @return     None
  *
  * @details    This API is used to config active shield dynamic tamper setting.
  */
void TAMP_ActiveShieldDynamicTamperConfig(uint32_t u32ChangeRate1, uint32_t u32SeedReload1, uint32_t u32RefPattern1, uint32_t u32Seed,
        uint32_t u32ChangeRate2, uint32_t u32SeedReload2, uint32_t u32RefPattern2, uint32_t u32Seed2)
{
    uint32_t u32Reg1, u32Reg2;

    u32Reg1 = TAMPER->ACTSTIOCTL1;
    u32Reg2 = TAMPER->ACTSTIOCTL2;

    u32Reg1 &= ~(TAMP_ACTSTIOCTL1_ADYNSRC_Msk | TAMP_ACTSTIOCTL1_ADYNRATE_Msk);
    u32Reg2 &= ~(TAMP_ACTSTIOCTL2_ADYNSRC2_Msk | TAMP_ACTSTIOCTL2_SEEDRLD2_Msk | TAMP_ACTSTIOCTL2_ADYNRATE2_Msk);

    u32Reg1 |= (u32ChangeRate1) | ((u32RefPattern1 & 0x3UL) << TAMP_ACTSTIOCTL1_ADYNSRC_Pos);
    u32Reg2 |= (u32ChangeRate2) | ((u32SeedReload2 & 0x1UL) << TAMP_ACTSTIOCTL2_SEEDRLD2_Pos) |
               ((u32RefPattern2 & 0x3UL) << TAMP_ACTSTIOCTL2_ADYNSRC2_Pos);

    TAMPER->SEED = u32Seed; /* Need to set seed value before re-loade seed */
    TAMPER->SEED2 = u32Seed2; /* Need to set seed value before re-loade seed */

    TAMPER->ACTSTIOCTL1 = u32Reg1;
    TAMPER->TIOCTL |= ((u32SeedReload1 & 0x1UL) << TAMP_TIOCTL_SEEDRLD_Pos);
    TAMPER->ACTSTIOCTL2 = u32Reg2;
}

/*@}*/ /* end of group Tamper_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group Tamper_Driver */

/*@}*/ /* end of group Standard_Driver */

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
