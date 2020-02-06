/**************************************************************************//**
 * @file     lcd.c
 * @version  V3.00
 * @brief    Liquid-Crystal Display(LCD) driver source file
 *
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "NuMicro.h"


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup LCD_Driver LCD Driver
  @{
*/


/// @cond HIDDEN_SYMBOLS

/** @addtogroup LCD_EXPORTED_VARIABLES LCD Exported Variables
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static uint32_t g_LCDFrameRate; /* LCD source:32768Hz, COM:4, FREQ Div:64, frame-rate = ((32768/(64*2))/4) = 64Hz */

/*@}*/ /* end of group LCD_EXPORTED_VARIABLES */

/// @endcond /* HIDDEN_SYMBOLS */


/** @addtogroup LCD_EXPORTED_FUNCTIONS LCD Exported Functions
  @{
*/

/**
  * @brief      LCD Initialization routine
  *
  * @param[in]  pLCDCfg Specify the LCD property. It includes:
  *                     u32SrcFreq: Clock source frequency of LCD controller.
  *                     u32ComNum: LCD COM duty selection. Valid values are:
  *                                 - \ref LCD_COM_NUM_1
  *                                 - \ref LCD_COM_NUM_2
  *                                 - \ref LCD_COM_NUM_3
  *                                 - \ref LCD_COM_NUM_4
  *                                 - \ref LCD_COM_NUM_5
  *                                 - \ref LCD_COM_NUM_6
  *                                 - \ref LCD_COM_NUM_7
  *                                 - \ref LCD_COM_NUM_8
  *                     u32BiasLevel: LCD Bias level selection. Valid values are:
  *                                     - \ref LCD_BIAS_LV_DIV_2
  *                                     - \ref LCD_BIAS_LV_DIV_3
  *                                     - \ref LCD_BIAS_LV_DIV_4
  *                     u32Framerate: Specify the LCD operating frame.
  *                     u32WaveformType: Specify the LCD waveform type. Valid values are:
  *                                         - \ref LCD_TYPE_A
  *                                         - \ref LCD_TYPE_B
  *                     u32ReverseWaveform: Set waveform reverse.
  *                                         0 : Waveform is not reversed
  *                                         1 : Waveform is reversed
  *                     u32IntSrc: Interrupt Source:
  *                                 - \ref LCD_DISABLE_INT
  *                                 - \ref LCD_FRAMECOUNT_INT
  *                                 - \ref LCD_FRAMEEND_INT
  *                                 - \ref LCD_ALL_INT
  *                     u32VLCDSrc: VLCD source:
  *                                 - \ref LCD_VLCD_EXTERNAL_POWER
  *                                 - \ref LCD_VLCD_INTERNAL_VDD
  *                                 - \ref LCD_VLCD_CHARGE_PUMP
  *
  * @return     LCD operating frame rate
  *
  * @details    This function will configure the LCD properties for driving the LCD display well.
  *             After that, user can perform \ref LCD_ENABLE_DISPLAY() to enable LCD controller for LCD display.
  */
uint32_t LCD_Open(S_LCD_CONFIGURATION_T *pLCDCfg)
{
    uint32_t u32TargetFR, u32FreqDiv;
    uint32_t u32ATPReg, u32ATPData;
    uint32_t u32ACFGReg, u32ACFGData, u32ATrim0Reg;

    LCD_DISABLE_DISPLAY();

    /* Turn all segments off */
    LCD_SetAllPixels(0);

    /* Set driving waveform, Type-A or Type-B */
    if(pLCDCfg->u32WaveformType == LCD_TYPE_A)
        LCD_WAVEFORM_TYPE_A();
    else
        LCD_WAVEFORM_TYPE_B();

    /* Reverse the waveform or not */
    if(pLCDCfg->u32ReverseWaveform == 0)
        LCD_WAVEFORM_NORMAL();
    else
        LCD_WAVEFORM_REVERSE();

    /* Calculate frequency divider */
    u32FreqDiv = pLCDCfg->u32SrcFreq / ((pLCDCfg->u32Framerate * pLCDCfg->u32ComNum) * 2);

    /* Set LCD operating frequency divider, COM duty cycle and Bias level */
    LCD->CFG = (LCD->CFG & ~(LCD_CFG_FREQDIV_Msk | LCD_CFG_DUTY_Msk | LCD_CFG_BIAS_Msk)) |
               (((u32FreqDiv-1)<<LCD_CFG_FREQDIV_Pos) | ((pLCDCfg->u32ComNum-1)<<LCD_CFG_DUTY_Pos) |((pLCDCfg->u32BiasLevel-1)<<LCD_CFG_BIAS_Pos));

    if(pLCDCfg->u32IntSrc == LCD_DISABLE_INT)
    {
        LCD_DISABLE_FRAME_COUNTING_INT();
        LCD_DISABLE_FRAME_END_INT();
    }
    else
    {
        if((pLCDCfg->u32IntSrc & LCD_FRAMECOUNT_INT) == LCD_FRAMECOUNT_INT )
        {
            LCD_ENABLE_FRAME_COUNTING_INT();
        }

        if((pLCDCfg->u32IntSrc & LCD_FRAMEEND_INT) == LCD_FRAMEEND_INT )
        {
            LCD_ENABLE_FRAME_END_INT();
        }
    }

    /* Calculate target frame rate */
    u32TargetFR = (pLCDCfg->u32SrcFreq / (u32FreqDiv * 2)) / pLCDCfg->u32ComNum;

    /* Get operating frame rate */
    g_LCDFrameRate = u32TargetFR;


    /* Setting Analog Timing Parameter according u32FreqDiv setting (operating frame rate) */
    u32ATPReg  = (uint32_t)((LCD_T *)LCD) + 0x54ul;
    u32ATPData = inpw(u32ATPReg);
    outpw(u32ATPReg, ((u32ATPData&0x00600FFFul) | ((u32FreqDiv-1)<<23) |((u32FreqDiv-1)<<12)));

    /* Configure Driving type */
    u32ACFGReg  = (uint32_t)((LCD_T *)LCD) + 0x50ul;
    u32ACFGData = inpw(u32ACFGReg);
    if(pLCDCfg->u32VLCDSrc == LCD_VLCD_CHARGE_PUMP)
    {
        /* Use LCD module clock as charge pump clock source */
        u32ATrim0Reg  = (uint32_t)((LCD_T *)LCD) + 0x60ul;
        outpw(u32ATrim0Reg, 0x1Eul);
    }
    outpw(u32ACFGReg, (u32ACFGData&0xFFFC3FF0ul) | (pLCDCfg->u32VLCDSrc<<2) | 0x1ul); // Default LCD operation is configured to BUF1 mode


    return g_LCDFrameRate;
}

/**
  * @brief      Disable LCD Controller
  *
  * @param[in]  None
  *
  * @return     None
  *
  * @details    This function is used to disable LCD controller.
  */
void LCD_Close(void)
{
    LCD_DISABLE_DISPLAY();
}

/**
  * @brief      Enables a Segment Display
  *
  * @param[in]  u32Com      Specify COM number. Valid values are from 0~7.
  * @param[in]  u32Seg      Specify Segment number. Valid values are from 0~43.
  * @param[in]  u32OnFlag   0 : Segment not display
  *                         1 : Segment display
  *
  * @return     None
  *
  * @details    This function is used to enable specified segment display on the LCD.
  */
void LCD_SetPixel(uint32_t u32Com, uint32_t u32Seg, uint32_t u32OnFlag)
{
    int32_t seg_num = (u32Seg / 4);
    int32_t seg_shift = (int32_t)(8 * (u32Seg - (4*(uint32_t)seg_num)));

    if (seg_num < 11)
    {
        if(u32OnFlag)
        {
            LCD->SDCTL[seg_num] |= (uint32_t)((1<<u32Com) << seg_shift);
        }
        else
        {
            LCD->SDCTL[seg_num] &= (uint32_t)(~((1<<u32Com) << seg_shift));
        }
    }
}

/**
  * @brief      Enable/Disable all LCD segments
  *
  * @param[in]  u32OnOff    0 : Disable all segments
  *                         1 : Enable all segments
  *
  * @return     None
  *
  * @details    This function is used to enable/disable all LCD segments.
  */
void LCD_SetAllPixels(uint32_t u32OnOff)
{
    uint32_t i, u32SetValue;

    if(u32OnOff)
    {
        u32SetValue = 0xFFFFFFFF;
    }
    else
    {
        u32SetValue = 0x00000000;
    }

    for(i = 0; i < 11; i++)
        LCD->SDCTL[i] = u32SetValue;
}

/**
  * @brief      Enable LCD Blinking
  *
  * @param[in]  u32ms       Blinking display time(unit: ms)
  *
  * @return     Real blinking delay time(ms)
  *
  * @details    This function is used to enable blink function with specified period in LCD controller.
  */
uint32_t LCD_EnableBlink(uint32_t u32ms)
{
    uint32_t u32FrameTime, u32FrameCount;

    if((1000/u32ms) > g_LCDFrameRate)
        u32ms = (1000 / g_LCDFrameRate);
    u32FrameTime = 1000 / g_LCDFrameRate;

    if(u32FrameTime == 0)
    {
        u32FrameCount = 1;
    }
    else
    {
        u32FrameCount = u32ms / u32FrameTime;
        if(u32FrameCount == 0)
            u32FrameCount = 1;
    }

    LCD_SET_FRAME_COUNTING_VALUE(u32FrameCount);

    /* Enable Blink LCD */
    LCD_BLINKING_ON();

    return (u32FrameTime * u32FrameCount);
}

/**
  * @brief      Disable LCD Blinking
  *
  * @param[in]  None
  *
  * @return     None
  *
  * @details    This function is used to disable blink function in LCD controller.
  */
void LCD_DisableBlink(void)
{
    /* Disable Blink LCD */
    LCD_BLINKING_OFF();
}

/**
  * @brief      Enable LCD Interrupt
  *
  * @param[in]  IntSrc      Interrupt Source: \ref LCD_FRAMECOUNT_INT / \ref LCD_FRAMEEND_INT / \ref LCD_ALL_INT
  *
  * @return     None
  *
  * @details    This function is used to enable LCD specified interrupt.
  */
void LCD_EnableInt(uint32_t u32IntSrc)
{
    if((u32IntSrc & LCD_FRAMECOUNT_INT) == LCD_FRAMECOUNT_INT )
    {
        LCD_ENABLE_FRAME_COUNTING_INT();
    }

    if((u32IntSrc & LCD_FRAMEEND_INT) == LCD_FRAMEEND_INT )
    {
        LCD_ENABLE_FRAME_END_INT();
    }
}

/**
  * @brief      Disable LCD Interrupt
  *
  * @param[in]  IntSrc      Interrupt Source: \ref LCD_FRAMECOUNT_INT / \ref LCD_FRAMEEND_INT / \ref LCD_ALL_INT
  *
  * @return     None
  *
  * @details    This function is used to disable LCD specified interrupt.
  */
void LCD_DisableInt(uint32_t u32IntSrc)
{
    if((u32IntSrc & LCD_FRAMECOUNT_INT) == LCD_FRAMECOUNT_INT )
    {
        LCD_DISABLE_FRAME_COUNTING_INT();
    }

    if((u32IntSrc & LCD_FRAMEEND_INT) == LCD_FRAMEEND_INT )
    {
        LCD_DISABLE_FRAME_END_INT();
    }
}


/*@}*/ /* end of group LCD_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group LCD_Driver */

/*@}*/ /* end of group Standard_Driver */

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
