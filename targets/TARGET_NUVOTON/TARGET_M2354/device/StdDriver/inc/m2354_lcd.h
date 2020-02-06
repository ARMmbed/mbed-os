/**************************************************************************//**
 * @file     lcd.h
 * @version  V3.00
 * @brief    Liquid-Crystal Display(LCD) driver header file
 *
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup LCD_Driver LCD Driver
  @{
*/

/** @addtogroup LCD_EXPORTED_CONSTANTS LCD Exported Constants
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/*  LCD Bias Voltage Level Constant Definitions                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define LCD_BIAS_LV_DIV_2           (2) /*!< LCD Bias Voltage Level Selection - 1/2 Bias \hideinitializer */
#define LCD_BIAS_LV_DIV_3           (3) /*!< LCD Bias Voltage Level Selection - 1/3 Bias \hideinitializer */
#define LCD_BIAS_LV_DIV_4           (4) /*!< LCD Bias Voltage Level Selection - 1/4 Bias \hideinitializer */

/*---------------------------------------------------------------------------------------------------------*/
/*  LCD COM Number (Duty Selection) Constant Definitions                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define LCD_COM_NUM_1               (1) /*!< LCD COM Number Selection - 1/1 Duty \hideinitializer */
#define LCD_COM_NUM_2               (2) /*!< LCD COM Number Selection - 1/2 Duty \hideinitializer */
#define LCD_COM_NUM_3               (3) /*!< LCD COM Number Selection - 1/3 Duty \hideinitializer */
#define LCD_COM_NUM_4               (4) /*!< LCD COM Number Selection - 1/4 Duty \hideinitializer */
#define LCD_COM_NUM_5               (5) /*!< LCD COM Number Selection - 1/5 Duty \hideinitializer */
#define LCD_COM_NUM_6               (6) /*!< LCD COM Number Selection - 1/6 Duty \hideinitializer */
#define LCD_COM_NUM_7               (7) /*!< LCD COM Number Selection - 1/7 Duty \hideinitializer */
#define LCD_COM_NUM_8               (8) /*!< LCD COM Number Selection - 1/8 Duty \hideinitializer */

/*---------------------------------------------------------------------------------------------------------*/
/*  LCD Waveform Type Constant Definitions                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define LCD_TYPE_A                  (0x0UL) /*!< LCD waveform Type-A \hideinitializer */
#define LCD_TYPE_B                  (0x1UL) /*!< LCD waveform Type-B \hideinitializer */

/*---------------------------------------------------------------------------------------------------------*/
/*  LCD Interrupt Source Constant Definitions                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define LCD_DISABLE_INT             (0x0UL) /*!< Disable frame count & frame end interrupt \hideinitializer */
#define LCD_FRAMECOUNT_INT          (0x1UL) /*!< Indicate frame count interrupt \hideinitializer */
#define LCD_FRAMEEND_INT            (0x2UL) /*!< Indicate frame end interrupt \hideinitializer */
#define LCD_ALL_INT                 (0x3UL) /*!< Indicate frame count & frame end interrupt \hideinitializer */

/*---------------------------------------------------------------------------------------------------------*/
/*  LCD VLCD Source Constant Definitions                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define LCD_VLCD_EXTERNAL_POWER     (0x0UL) /*!< VLCD from external power \hideinitializer */
#define LCD_VLCD_INTERNAL_VDD       (0x1UL) /*!< VLCD from internal VDD \hideinitializer */
#define LCD_VLCD_CHARGE_PUMP        (0x2UL) /*!< VLCD from charge pump \hideinitializer */

/*@}*/ /* end of group LCD_EXPORTED_CONSTANTS */


/** @addtogroup LCD_EXPORTED_STRUCTS LCD Exported Structs
  @{
*/
/**
  * @details    LCD define Time Data Struct
  */
typedef struct
{
    uint32_t u32SrcFreq;         /*!< LCD clock source frequency */
    uint32_t u32ComNum;          /*!< COM number */
    uint32_t u32BiasLevel;       /*!< Bias level */
    uint32_t u32Framerate;       /*!< Operation frame rate */
    uint32_t u32WaveformType;    /*!< Waveform type */
    uint32_t u32ReverseWaveform; /*!< Reverse waveform or not */
    uint32_t u32IntSrc;          /*!< Interrupt source */
    uint32_t u32VLCDSrc;         /*!< VLCD source */
} S_LCD_CONFIGURATION_T;

/*@}*/ /* end of group LCD_EXPORTED_STRUCTS */


/** @addtogroup LCD_EXPORTED_FUNCTIONS LCD Exported Functions
  @{
*/

/**
  * @brief      Enable LCD Display
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to enable LCD display.
  */
#define LCD_ENABLE_DISPLAY()        do{ LCD->CTL |= LCD_CTL_EN_Msk; while(LCD->CTL & LCD_CTL_SYNC_Msk) {} }while(0)

/**
  * @brief      Disable LCD Display
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to disable LCD display.
  */
#define LCD_DISABLE_DISPLAY()       do{ LCD->CTL &= ~LCD_CTL_EN_Msk; while(LCD->CTL & LCD_CTL_SYNC_Msk) {} }while(0)

/**
  * @brief      Set LCD Frame Counting Value
  *
  * @param[in]  u32Value    Frame counting value. Valid value is between 1 to 1024.
  *
  * @return     None
  *
  * @details    This macro is used to set the LCD frame counting value to configure the blink interval.
  */
#define LCD_SET_FRAME_COUNTING_VALUE(u32Value)      (LCD->CTL = (LCD->CTL & ~LCD_CTL_FCV_Msk) | (((u32Value)-1) << LCD_CTL_FCV_Pos))

/**
  * @brief      Set LCD Blinking ON
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to enable LCD blinking.
  */
#define LCD_BLINKING_ON()       (LCD->CTL |= LCD_CTL_BLINK_Msk)
    
/**
  * @brief      Set LCD Blinking OFF
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to disable LCD blinking.
  */
#define LCD_BLINKING_OFF()      (LCD->CTL &= ~LCD_CTL_BLINK_Msk)

/**
  * @brief      Set Normal LCD Waveform
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to set the normal LCD driving wafevorm.
  */
#define LCD_WAVEFORM_NORMAL()       (LCD->CTL &= ~LCD_CTL_WVREV_Msk)

/**
  * @brief      Reverse LCD Waveform
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to reverse the LCD driving wafevorm.
  */
#define LCD_WAVEFORM_REVERSE()      (LCD->CTL |= LCD_CTL_WVREV_Msk)

/**
  * @brief      Set LCD Waveform Type-A
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to set the LCD driving waveform in Type-A.
  */
#define LCD_WAVEFORM_TYPE_A()       (LCD->CFG &= ~LCD_CFG_TYPE_Msk)

/**
  * @brief      Set LCD Waveform Type-B
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to set the LCD driving waveform in Type-B.
  */
#define LCD_WAVEFORM_TYPE_B()       (LCD->CFG |= LCD_CFG_TYPE_Msk)

/**
  * @brief      Enable LCD Frame Counting Interrupt
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to enable frame count interrupt function.
  */
#define LCD_ENABLE_FRAME_COUNTING_INT()     (LCD->INTEN |= LCD_INTEN_FCIEN_Msk)

/**
  * @brief      Enable LCD Frame End Interrupt
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to enable frame end interrupt function.
  */
#define LCD_ENABLE_FRAME_END_INT()          (LCD->INTEN |= LCD_INTEN_FEIEN_Msk)

/**
  * @brief      Disable LCD Frame Counting Interrupt
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to disable frame count interrupt function.
  */
#define LCD_DISABLE_FRAME_COUNTING_INT()     (LCD->INTEN &= ~LCD_INTEN_FCIEN_Msk)

/**
  * @brief      Disable LCD Frame End Interrupt
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro is used to disable frame end interrupt function.
  */
#define LCD_DISABLE_FRAME_END_INT()          (LCD->INTEN &= ~LCD_INTEN_FEIEN_Msk)

/**
  * @brief      Get LCD Frame Counting Flag
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro gets frame count event flag.
  */
#define LCD_GET_FRAME_COUNTING_FLAG()       ((LCD->INTSTS & LCD_INTSTS_FCIF_Msk)? 1UL : 0UL)

/**
  * @brief      Get LCD Frame End Flag
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro gets frame end event flag.
  */
#define LCD_GET_FRAME_END_FLAG()            ((LCD->INTSTS & LCD_INTSTS_FEIF_Msk)? 1UL : 0UL)

/**
  * @brief      Clear LCD Frame Counting Flag
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro clears frame count event flag.
  */
#define LCD_CLEAR_FRAME_COUNTING_FLAG()     (LCD->INTSTS = LCD_INTSTS_FCIF_Msk)

/**
  * @brief      Clear LCD Frame End Flag
  *
  * @param      None
  *
  * @return     None
  *
  * @details    This macro clears frame end event flag.
  */
#define LCD_CLEAR_FRAME_END_FLAG()          (LCD->INTSTS = LCD_INTSTS_FEIF_Msk)


uint32_t LCD_Open(S_LCD_CONFIGURATION_T *pLCDCfg);
void LCD_Close(void);
void LCD_SetPixel(uint32_t u32Com, uint32_t u32Seg, uint32_t u32OnFlag);
void LCD_SetAllPixels(uint32_t u32OnOff);
uint32_t LCD_EnableBlink(uint32_t u32ms);
void LCD_DisableBlink(void);
void LCD_EnableInt(uint32_t u32IntSrc);
void LCD_DisableInt(uint32_t u32IntSrc);

/*@}*/ /* end of group LCD_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group LCD_Driver */

/*@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __LCD_H__ */

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
