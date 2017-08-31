/**
 *******************************************************************************
 * @file    tmpm066_cg.c
 * @brief   This file provides API functions for CG driver 
 * @version V2.0.2.1    
 * @date    2015/09/22
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm066_cg.h"

#if defined(__TMPM066_CG_H)
/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @defgroup CG
  * @brief CG driver modules
  * @{
  */

/** @defgroup CG_Private_Defines
  * @{
  */

#define CG_FC_GEAR_MASK                      ((uint32_t)0xFFFFFFF8)
#define CG_PRCK_MASK                         ((uint32_t)0xFFFFF0FF)
#define CG_WUP_TIME_MASK                     ((uint32_t)0x000FFFFF)
#define CG_WUP_COUNTER_MASK                  ((uint32_t)0xFFFFFEFF)
#define CG_WUP_START_SET                     ((uint32_t)0x00000001)
#define CG_WUEF_VALUE_MASK                   ((uint32_t)0x00000002)
#define CG_OSCCR_IOSCEN_SET                  ((uint32_t)0x00000001)
#define CG_OSCCR_IOSCEN_CLEAR                ((uint32_t)0xFFFFFFFE)
#define CG_OSCCR_EOSCEN_EHOSC                ((uint32_t)0x00000002)
#define CG_OSCCR_EOSCEN_NOUSE                ((uint32_t)0xFFFFFFF9)
#define CG_OSCCR_EOSCEN_MASK                 ((uint32_t)0x00000006)
#define CG_OSCCR_EOSCEN_CLKIN                ((uint32_t)0x00000004)
#define CG_OSCCR_OSCSEL_SET                  ((uint32_t)0x00000100)
#define CG_OSCCR_OSCSEL_CLEAR                ((uint32_t)0xFFFFFEFF)
#define CG_OSCCR_OSCF_SET                    ((uint32_t)0x00000200)
#define CG_OSCCR_WUPT_MASK                   ((uint32_t)0x00FFFFFF)

#define CG_PLL0SEL_PLL0ON_SET                ((uint32_t)0x00000001)
#define CG_PLL0SEL_PLL0ON_CLEAR              ((uint32_t)0xFFFFFFFE)
#define CG_PLL0SEL_PLL0SEL_SET               ((uint32_t)0x00000002)
#define CG_PLL0SEL_PLL0SEL_CLEAR             ((uint32_t)0xFFFFFFFD)
#define CG_PLL0SEL_PLLST_SET                 ((uint32_t)0x00000004)

#define CG_PLL0SET_VALUE_MASK                 ((uint32_t)0xFFFFFF00)
#define CG_PLL0SEL_PLL0SET_MASK               ((uint32_t)0xFFFFFF00)

#define CG_STBY_MODE_MASK                    ((uint32_t)0xFFFFFFFC)
//#define CG_NMIFLG_MASK                       ((uint32_t)0xFFF8FFFF)

#define FC_GEAR_1_1                          ((uint32_t)0x00000000)
#define FC_GEAR_1_2                          ((uint32_t)0x00000001)
#define FC_GEAR_1_4                          ((uint32_t)0x00000002)
#define FC_GEAR_1_8                          ((uint32_t)0x00000003)
#define FC_GEAR_1_16                         ((uint32_t)0x00000004)

#define WARM_UP_SEL_OSC_INT_HIGH             ((uint32_t)0xFFFFFEFF)
#define WARM_UP_SEL_OSC_EXT_HIGH             ((uint32_t)0x00000100)

#define CG_PROTECT_SET                       ((uint32_t)0x000000C1)
#define CG_PROTECT_CLEAR                     ((uint32_t)0x0000003E)

//#define INT_NCLR_PCLR_CLEAR                  ((uint8_t)0x01)
#define ADC_MOD0_BUSY_MASK                   ((uint32_t)0x00000040)
#define CG_SPCLKEN_ADCKEN_CLEAR                ((uint32_t)0xFFFEFFFF)
#define CG_SPCLKEN_ADCKEN_SET                  ((uint32_t)0x00010000)


static CG_DivideLevel numToDivideLevel_table[CG_DIVIDE_MAX] = {
    CG_DIVIDE_1,
    CG_DIVIDE_2,
    CG_DIVIDE_4,
    CG_DIVIDE_8,
    CG_DIVIDE_16,
    CG_DIVIDE_32,
    CG_DIVIDE_64,
    CG_DIVIDE_128,
    CG_DIVIDE_256,
    CG_DIVIDE_512,
    CG_DIVIDE_UNKNOWN,
};

static CG_STBYMode numToSTBYMode_table[CG_STBY_MODE_MAX] = {
    CG_STBY_MODE_IDLE,
    CG_STBY_MODE_STOP1,
    CG_STBY_MODE_UNKNOWN,
    CG_STBY_MODE_UNKNOWN,
};

/** @} */
/* End of group CG_Private_Defines */

/** @defgroup CG_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group CG_Private_FunctionPrototypes */

/** @defgroup CG_Private_Functions
  * @{
  */

/** @} */
/* End of group CG_Private_Functions */

/** @defgroup CG_Exported_Functions
  * @{
  */

/**
  * @brief  Set dividing level between clock fgear and fc.
  * @param  DivideFgearFromFc: Dividing level between fgear and fc.
  *   This parameter can be one of the following values:
  *   CG_DIVIDE_1, CG_DIVIDE_2, CG_DIVIDE_4, CG_DIVIDE_8, CG_DIVIDE_16
  * @retval None
  */
void CG_SetFgearLevel(CG_DivideLevel DivideFgearFromFc)
{
    uint32_t gear = FC_GEAR_1_1;
    uint32_t regval = TSB_CG->SYSCR;

    /* Check the parameters */
    assert_param(IS_CG_GEAR_DIVIDE_LEVEL(DivideFgearFromFc));

    /* Set the value of fgear */
    switch (DivideFgearFromFc) {
    case CG_DIVIDE_1:
        gear = FC_GEAR_1_1;
        break;
    case CG_DIVIDE_2:
        gear = FC_GEAR_1_2;
        break;
    case CG_DIVIDE_4:
        gear = FC_GEAR_1_4;
        break;
    case CG_DIVIDE_8:
        gear = FC_GEAR_1_8;
        break;
    case CG_DIVIDE_16:
        gear = FC_GEAR_1_16;
        break;
    default:
        /* Do nothing */
        break;
    }

    regval &= CG_FC_GEAR_MASK;
    regval |= gear;
    TSB_CG->SYSCR = regval;
}

/**
  * @brief  Get dividing level between clock fgear and fc.
  * @param  None
  * @retval The dividing level between clock fgear and fc
  *   The value returned can be one of the following values:
  *   CG_DIVIDE_1, CG_DIVIDE_2, CG_DIVIDE_4,
  *   CG_DIVIDE_8, CG_DIVIDE_16 or CG_DIVIDE_UNKNOWN
  */
CG_DivideLevel CG_GetFgearLevel(void)
{
    CG_DivideLevel DivideFgearFromFc = CG_DIVIDE_UNKNOWN;
    uint32_t syscr = TSB_CG->SYSCR;
    syscr &= (~CG_FC_GEAR_MASK);

    switch (syscr) {
    case FC_GEAR_1_1:
        DivideFgearFromFc = CG_DIVIDE_1;
        break;
    case FC_GEAR_1_2:
        DivideFgearFromFc = CG_DIVIDE_2;
        break;
    case FC_GEAR_1_4:
        DivideFgearFromFc = CG_DIVIDE_4;
        break;
    case FC_GEAR_1_8:
        DivideFgearFromFc = CG_DIVIDE_8;
        break;
    case FC_GEAR_1_16:
        DivideFgearFromFc = CG_DIVIDE_16;
        break;
    default:
        /* Do nothing */
        break;
    }

    return DivideFgearFromFc;
}

/**
  * @brief  Set dividing level between clock PhiT0 and fc.
  * @param  DividePhiT0FromFc: Dividing level between PhiT0 and fc.
  *   This parameter can be one of the following values:
  *   CG_DIVIDE_1, CG_DIVIDE_2, CG_DIVIDE_4, CG_DIVIDE_8,
  *   CG_DIVIDE_16, CG_DIVIDE_32, CG_DIVIDE_64, CG_DIVIDE_128,
  *   CG_DIVIDE_256 or CG_DIVIDE_512
  * @retval  Success or not
  *   The value returned can be one of the following values:
  *   SUCCESS or ERROR
  */
Result CG_SetPhiT0Level(CG_DivideLevel DividePhiT0FromFc)
{
    uint32_t fprclk = 0U;
    Result retval = ERROR;
    uint32_t regval = TSB_CG->SYSCR;

    /* Check the parameters */
    assert_param(IS_CG_DIVIDE_FC_LEVEL(DividePhiT0FromFc));
  
    fprclk = (uint32_t) DividePhiT0FromFc;
    regval &= CG_PRCK_MASK;
    fprclk <<= 8U;
    regval |= fprclk;
    TSB_CG->SYSCR = regval;
    retval = SUCCESS;
    
    return retval;
}

/**
  * @brief  Get dividing level between clock phiT0 and fc.
  * @param  None
  * @retval The divide level between clock phiT0 and fc
  *   The value returned can be one of the following values:
  *   CG_DIVIDE_1, CG_DIVIDE_2, CG_DIVIDE_4, CG_DIVIDE_8,
  *   CG_DIVIDE_16, CG_DIVIDE_32, CG_DIVIDE_64, CG_DIVIDE_128,
  *   CG_DIVIDE_256, CG_DIVIDE_512 or CG_DIVIDE_UNKNOWN
  */
CG_DivideLevel CG_GetPhiT0Level(void)
{
    uint32_t fprclk = 0U;
    CG_DivideLevel phiT0Level = CG_DIVIDE_UNKNOWN;

    fprclk = TSB_CG->SYSCR & (~CG_PRCK_MASK);
    fprclk = (uint32_t) (fprclk >> 8U);

    phiT0Level = numToDivideLevel_table[fprclk];
    
    return phiT0Level;
}

/**
  * @brief  Set the warm up time 
  * @param  Source: Select source of warm-up counter
  *   This parameter can be one of the following values:
  *   CG_WARM_UP_SRC_OSC_INT_HIGH, CG_WARM_UP_SRC_OSC_EXT_HIGH.
  *   Warm-up function is not necessary when using stable external clock.
  * @param  Time: Set number of warm-up cycle. It is between 0x0000 and 0xFFFF.
  * @retval None
  */
void CG_SetWarmUpTime(CG_WarmUpSrc Source, uint16_t Time)
{
    uint32_t wupt = 0U;
    uint32_t regval = TSB_CG->WUPHCR;

    /* Check the parameters */
    assert_param(IS_CG_WARM_UP_SRC(Source));

    /* Get high 12 bits of warm-up time */
    wupt = (((uint32_t) Time) & CG_OSCCR_WUPT_MASK) << 20U;

    regval &= CG_WUP_TIME_MASK;
    regval &= CG_WUP_COUNTER_MASK;

    switch (Source) {
    case CG_WARM_UP_SRC_OSC_INT_HIGH:
        regval |= wupt;
        regval &= WARM_UP_SEL_OSC_INT_HIGH;
        break;
    case CG_WARM_UP_SRC_OSC_EXT_HIGH:
        regval |= wupt;
        regval |= WARM_UP_SEL_OSC_EXT_HIGH;
        break;
    default:
        /* Do nothing */
        break;
    }

    TSB_CG->WUPHCR = regval;
}

/**
  * @brief  Start operation of warm up timer for oscillator.
  * @param  None
  * @retval None
  */
void CG_StartWarmUp(void)
{
    uint32_t regval = TSB_CG->WUPHCR;
    regval |= CG_WUP_START_SET;
    TSB_CG->WUPHCR = regval;
}

/**
  * @brief Check whether warm up is completed or not.
  * @param  None
  * @retval The state of warm-up
  *   The value returned can be one of the following values:
  *   DONE or BUSY
  */
WorkState CG_GetWarmUpState(void)
{
    WorkState state = BUSY;
    uint32_t wuef = 0U;
    uint32_t regval = TSB_CG->WUPHCR;

    wuef = regval & CG_WUEF_VALUE_MASK;
    if (wuef == 0U) {
        state = DONE;
    } else {
        /* Do nothing */
    }

    return state;
}

/**
  * @brief  Set PLL multiplying value
  * @param  NewValue: PLL multiplying value
  *   This parameter can be one of the following values:
  *   CG_8M_MUL_12_FPLL, CG_10M_MUL_8_FPLL, CG_12M_MUL_8_FPLL,
  *   CG_16M_MUL_6_FPLL.
  * @retval Success or not
  *   The value returned can be one of the following values:
  *   SUCCESS or ERROR
  */
Result CG_SetFPLLValue(uint32_t NewValue)
{
    Result retval = SUCCESS;

    /* read PLL0SEL and clear PLL0SET(bit14:0) */
    uint32_t tmp = TSB_CG->PLL0SEL & CG_PLL0SEL_PLL0SET_MASK;

    /* Check the parameters */
    assert_param(IS_CG_FPLL_VALUE(NewValue));

    /* Don't use the PLL when internal high-speed oscillator (IHOSC) is used as system clock */
    /* When PLL is on, don't change the PLL setting value */
    if (CG_FOSC_OSC_INT == CG_GetFoscSrc()) {
        retval = ERROR;
    } else if (ENABLE == CG_GetPLLState()) {
        retval = ERROR;
    } else {
        tmp |= NewValue;
        TSB_CG->PLL0SEL = tmp;
    }

    return retval;
}

/**
  * @brief  Get the value of PLL setting
  * @param  None
  * @retval Get the value of PLL setting.  
  *   CG_8M_MUL_12_FPLL, CG_10M_MUL_8_FPLL, CG_12M_MUL_8_FPLL,
  *   CG_16M_MUL_6_FPLL.
  */
uint32_t CG_GetFPLLValue(void)
{
    uint32_t PLL0SEL = TSB_CG->PLL0SEL & CG_PLL0SET_VALUE_MASK;

    return PLL0SEL;
}

/**
  * @brief  Enable PLL or disable it.
  * @param  NewState: New state of PLL
  *   This parameter can be one of the following values:
  *   DISABLE or ENABLE
  * @retval Success or not
  *   The value returned can be one of the following values:
  *   SUCCESS or ERROR
  */
Result CG_SetPLL(FunctionalState NewState)
{
    Result retval = ERROR;
    uint32_t regval = TSB_CG->PLL0SEL;
    CG_FcSrc fcsrc = CG_FC_SRC_FPLL;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        regval |= CG_PLL0SEL_PLL0ON_SET;
        retval = SUCCESS;
    } else {
        fcsrc = CG_GetFcSrc();
        if (fcsrc == CG_FC_SRC_FOSC) {  /* PLL is not used. So it can be shut off */
            /* Set register to disable PLL */
            regval &= CG_PLL0SEL_PLL0ON_CLEAR;
            retval = SUCCESS;
        } else {                /*fcsrc == CG_FC_SRC_FPLL. PLL is in use, so disabling it should be invalid */
            /* Do nothing */
        }
    }
    TSB_CG->PLL0SEL = regval;
    return retval;
}

/**
  * @brief  Get the status of PLL operation
  * @param  None
  * @retval Run or stop
  *   The value returned can be one of the following values:
  *   ENABLE or DISABLE
  */
FunctionalState CG_GetPLLState(void)
{
    FunctionalState pllstate = DISABLE;
    uint32_t regval = TSB_CG->PLL0SEL;

    regval &= CG_PLL0SEL_PLL0ON_SET;
    if (regval == CG_PLL0SEL_PLL0ON_SET) {
        pllstate = ENABLE;
    } else {
        /* Do nothing */
    }

    return pllstate;
}


/**
  * @brief  Set the source of high-speed oscillator (fosc)
  * @param  Source: Select clock source for fosc
  *   This parameter can be one of the following values:
  *   CG_FOSC_OSC_EXT , CG_FOSC_CLKIN_EXT or CG_FOSC_OSC_INT
  * @retval None
  */
void CG_SetFoscSrc(CG_FoscSrc Source)
{
    /* Check the parameters */
    assert_param(IS_CG_FOSC_SRC(Source));

    if (Source == CG_FOSC_OSC_INT) {
        /*Selects internal high-speed oscillator */
        TSB_CG->OSCCR |= CG_OSCCR_IOSCEN_SET;
        TSB_CG->OSCCR &= CG_OSCCR_OSCSEL_CLEAR;        
    } else {
        /*Selects external high-speed oscillator */
        TSB_CG->OSCCR |= CG_OSCCR_OSCSEL_SET;
        TSB_CG->OSCCR &= CG_OSCCR_IOSCEN_CLEAR | CG_OSCCR_EOSCEN_NOUSE;
        if (Source == CG_FOSC_OSC_EXT) {
            /*Selects external high-speed oscillator */
            TSB_CG->OSCCR |= CG_OSCCR_EOSCEN_EHOSC;
        } else {
            /*Selects an external clock input */
            TSB_CG->OSCCR |= CG_OSCCR_EOSCEN_CLKIN;
        }
    }
}

/**
  * @brief  Get the source of high-speed oscillator (fosc)
  * @param  None
  * @retval Source of fosc
  *   The value returned can be one of the following values:
  *   CG_FOSC_OSC_EXT , CG_FOSC_CLKIN_EXT or CG_FOSC_OSC_INT
  */
CG_FoscSrc CG_GetFoscSrc(void)
{
    uint32_t regval = TSB_CG->OSCCR;
    uint32_t oscf = 0U;
    uint32_t hoscon = 0U;
    CG_FoscSrc fosc_src = CG_FOSC_CLKIN_EXT;

    oscf = regval & CG_OSCCR_OSCF_SET;
    hoscon = regval & CG_OSCCR_EOSCEN_MASK;
    if (oscf == 0U) {
        fosc_src = CG_FOSC_OSC_INT;
    } else {
        if (hoscon == CG_OSCCR_EOSCEN_EHOSC) {
            fosc_src = CG_FOSC_OSC_EXT;
        } else {
            /* Do nothing */
        }
    }

    return fosc_src;
}

/**
  * @brief  Set to the specified low-power mode
  * @param  Low power mode
  *   The value can be one of the following values:
  *   CG_STBY_MODE_STOP1, CG_STBY_MODE_IDLE.
  * @retval None
  */
void CG_SetSTBYMode(CG_STBYMode Mode)
{
    uint32_t regval = TSB_CG->STBYCR;

    /* Check the parameter */
    assert_param(IS_CG_STBY_MODE(Mode));

    regval &= CG_STBY_MODE_MASK;
    regval |= (uint32_t) Mode;

    TSB_CG->STBYCR = regval;
}

/**
  * @brief  Get the low-power consumption mode
  * @param  None
  * @retval Low power mode
  *   The value returned can be one of the following values:
  *   CG_STBY_MODE_STOP1, CG_STBY_MODE_IDLE, CG_STBY_MODE_UNKNOWN. 
  */
CG_STBYMode CG_GetSTBYMode(void)
{
    CG_STBYMode stby_mode = CG_STBY_MODE_UNKNOWN;
    uint8_t regval = (uint8_t) (TSB_CG->STBYCR & (~CG_STBY_MODE_MASK));

    stby_mode = numToSTBYMode_table[regval];

    return stby_mode;
}

/**
  * @brief  Set the source of fc
  * @param  Source:  The source of fc
  *   This parameter can be one of the following values:
  *   CG_FC_SRC_FOSC or CG_FC_SRC_FPLL
  * @retval None
  */
void CG_SetFcSrc(CG_FcSrc Source)
{
    uint32_t regval = TSB_CG->PLL0SEL;

    /* Check the parameters */
    assert_param(IS_CG_FC_SRC(Source));

    if (Source == CG_FC_SRC_FOSC) {
       regval &= CG_PLL0SEL_PLL0SEL_CLEAR;
    } else {
       regval |= CG_PLL0SEL_PLL0SEL_SET;
    }
    TSB_CG->PLL0SEL = regval;
}

/**
  * @brief  Get the source of fc
  * @param  None
  * @retval The source of fc
  *   The value returned can be one of the following values:
  *   CG_FC_SRC_FOSC or CG_FC_SRC_FPLL
  */
CG_FcSrc CG_GetFcSrc(void)
{
    uint32_t regval = TSB_CG->PLL0SEL;
    CG_FcSrc fcsrc = CG_FC_SRC_FOSC;

    regval &= CG_PLL0SEL_PLLST_SET;
    if (regval == CG_PLL0SEL_PLLST_SET) {
        fcsrc = CG_FC_SRC_FPLL;
    } else {
        /* Do nothing */
    }

    return fcsrc;
}

/**
  * @brief  Enable or disable to protect CG registers
  * @param  NewState: New state of the CG protect register
  *   This parameter can be one of the following values:
  *   DISABLE or ENABLE
  * @retval None
  */
void CG_SetProtectCtrl(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        TSB_CG->PROTECT = CG_PROTECT_SET;
    } else {
        TSB_CG->PROTECT = CG_PROTECT_CLEAR;
    }
}

/**
   * @brief  Enable or disable supplying clock fsys to peripheries
   * @param  Periph: The target peripheral of CG supplies clock
   *   This parameter can be one of the following values or their combination:
   *   CG_FC_PERIPH_PORTH,  CG_FC_PERIPH_PORTJ,  CG_FC_PERIPH_TMRB0_3,          
   *   CG_FC_PERIPH_TMRB4_6,  CG_FC_PERIPH_TMR16A,  CG_FC_PERIPH_I2C0,         
   *   CG_FC_PERIPH_SIO0,  CG_FC_PERIPH_TSPI,  CG_FC_PERIPH_DMAC,          
   *   CG_FC_PERIPH_ADC,  CG_FC_PERIPH_USBD,  CG_FC_PERIPH_TMRD,
   *   CG_FC_PERIPHA_ALL.      
   * @param  NewState: New state of clock supply setting.
   *   This parameter can be one of the following values:
   *   DISABLE or ENABLE
   * @retval None
   */
void CG_SetFcPeriphA(uint32_t Periph, FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_CG_FC_PERIPHA(Periph));
    if (NewState == ENABLE) {   /* write '1' to enable */
        TSB_CG->FSYSENA |= Periph;
    } else {                    /* clear to '0' to disable */
        TSB_CG->FSYSENA &= ~Periph;
    }
}

 /**
   * @brief  Enable or disable supplying clock fsys to peripheries
   * @param  Periph: The target peripheral of CG supplies clock
   *   This parameter can be one of the following values or their combination:   
   *   CG_FC_PERIPH_TMRB7, CG_FC_PERIPH_SIO1, CG_FC_PERIPH_WDT,      
   *   CG_FC_PERIPH_I2C1, CG_FC_PERIPHB_ALL.                  
   * @param  NewState: New state of clock supply setting.
   *   This parameter can be one of the following values:
   *   DISABLE or ENABLE
   * @retval None
   */
void CG_SetFcPeriphB(uint32_t Periph, FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_CG_FC_PERIPHB(Periph));
    if (NewState == ENABLE) {   /* write '1' to enable */
        TSB_CG->FSYSENB |= Periph;
    } else {                    /* clear to '0' to disable */
        TSB_CG->FSYSENB &= ~Periph;
    }
}

 /**
   * @brief  Enable or disable supplying clock fsys to peripheries
   * @param  Periph: The target peripheral of CG supplies clock
   *   This parameter can be one of the following values or their combination:   
   *   CG_FPLL_PERIPH_TMRD, CG_EHCLKSEL_8_24_48MHZ, CG_USBSEL_PLL_CLOCKIN,
   *   CG_FC_PERIPH_I2C1, CG_FC_PERIPHB_ALL.  
   * @param  NewState: New state of clock supply setting.
   *   This parameter can be one of the following values:
   *   DISABLE or ENABLE
   * @retval None
   */
void CG_SetFcOptional(uint32_t Periph, FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_CG_FPLL_OPTIONAL(Periph));
    if (NewState == ENABLE) {   /* write '1' to enable */
        TSB_CG->EXTENDO0 |= Periph;
    } else {                    /* clear to '0' to disable */
        TSB_CG->EXTENDO0 &= ~Periph;
    }
}

/**
  * @brief  Enable or disable supplying clock fsys for ADC.
  * @param  NewState: New state of clock fsys supply setting for ADC.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void CG_SetADCClkSupply(FunctionalState NewState)
{
    volatile uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Check ADC is not during conversion */
    do {
        tmp = TSB_AD->MOD0 & ADC_MOD0_BUSY_MASK;
    }
    while (tmp);

    /* Set CGSPCLKEN<ADCKEN>  */
    tmp = TSB_CG->SPCLKEN;
    if (NewState == ENABLE) {
        tmp |= CG_SPCLKEN_ADCKEN_SET;        
    } else {
        tmp &= CG_SPCLKEN_ADCKEN_CLEAR;
    }
    TSB_CG->SPCLKEN = tmp;
}

/** @} */
/* End of group CG_Exported_Functions */

/** @} */
/* End of group CG */

/** @} */
/* End of group TX00_Periph_Driver */

#endif                          /* defined(__TMPM066_CG_H) */
