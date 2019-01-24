/**
  ******************************************************************************
  * @file    BlueNRG1_sysCtrl.c
  * @author  VMA Application Team
  * @version V2.0.1
  * @date    21-March-2016
  * @brief   This file provides all the System Controller firmware functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../../Periph_Driver/inc/BlueNRG1_sysCtrl.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @defgroup SysCtrl System Controller Driver
  * @{
  */ 

/** @defgroup SysCtrl_Private_TypesDefinitions Private Types Definitions
  * @{
  */

/**
  * @}
  */ 

/** @defgroup SysCtrl_Private_Defines Private Defines
  * @{
  */ 


#define SOC_REASON_SYSREQ        (0x00000002)
#define SOC_REASON_WDG           (0x00000004)
#define SOC_REASON_LOCKUP        (0x00000008)

#define BLE_REASON_WAKEUP_RST    (0x00000001)
#define BLE_REASON_BOR_RST       (0x00000002 | BLE_REASON_WAKEUP_RST)
#define BLE_REASON_POR_RST       (0x00000004 | BLE_REASON_WAKEUP_RST)
#define BLE_REASON_FROM_IO9      (0x00000008 | BLE_REASON_WAKEUP_RST)
#define BLE_REASON_FROM_IO10     (0x00000010 | BLE_REASON_WAKEUP_RST)
#define BLE_REASON_FROM_IO11     (0x00000020 | BLE_REASON_WAKEUP_RST)
#define BLE_REASON_FROM_IO12     (0x00000040 | BLE_REASON_WAKEUP_RST)
#define BLE_REASON_FROM_IO13     (0x00000080 | BLE_REASON_WAKEUP_RST)
#define BLE_REASON_FROM_TIMER1   (0x00000100 | BLE_REASON_WAKEUP_RST)
#define BLE_REASON_FROM_TIMER2   (0x00000400 | BLE_REASON_WAKEUP_RST)


/**
  * @}
  */ 

/** @defgroup SysCtrl_Private_Macros Private Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup SysCtrl_Private_Variables Private Variables
  * @{
  */

/**
  * @}
  */ 

/** @defgroup SysCtrl_Private_FunctionPrototypes Private Function Prototypes
  * @{
  */
  
/**
  * @}
  */
  
/** @defgroup SysCtrl_Public_Functions Public Functions
  * @{
  */


/**
  * @brief  De init function
  * @param  None
  * @retval None
  */
void SysCtrl_DeInit(void)
{
  SYSTEM_CTRL->WKP_IO_IS  = 0x00;
  SYSTEM_CTRL->WKP_IO_IE  = 0x03;
  CKGEN_SOC->CLOCK_EN     = 0x0003FFFF;
  CKGEN_BLE->CLK32K_COUNT = 0x0000000F;
  CKGEN_BLE->CLK32K_IT    = 0x00000000;
}


/**
  * @brief  Enables the clock for the specified peripheral.
  * @param  PeriphClock: Peripheral to be enabled, this value can be
  *         @arg CLOCK_PERIPH_GPIO Clock of GPIO peripheral
  *         @arg CLOCK_PERIPH_NVM Clock of NVM peripheral
  *         @arg CLOCK_PERIPH_SYS_CONTROL Clock of SYSTEM CONTROL peripheral
  *         @arg CLOCK_PERIPH_UART Clock of UART peripheral
  *         @arg CLOCK_PERIPH_SPI Clock of SPI peripheral
  *         @arg CLOCK_PERIPH_WDG Clock of WATCHDOG peripheral
  *         @arg CLOCK_PERIPH_ADC Clock of ADC peripheral
  *         @arg CLOCK_PERIPH_I2C1 Clock of I2C1 peripheral
  *         @arg CLOCK_PERIPH_I2C2 Clock of I2C2 peripheral
  *         @arg CLOCK_PERIPH_MTFX1 Clock of MFTX1 peripheral
  *         @arg CLOCK_PERIPH_MTFX2 Clock of MFTX2 peripheral
  *         @arg CLOCK_PERIPH_RTC Clock of RTC peripheral
  *         @arg CLOCK_PERIPH_DMA Clock of DMA peripheral
  *         @arg CLOCK_PERIPH_RNG Clock of RNG peripheral
  *         @arg CLOCK_PERIPH_PKA Clock of PKA peripheral
  * @param NewState: New state of the peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SysCtrl_PeripheralClockCmd(uint32_t PeriphClock, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_CLOCK_PERIPH(PeriphClock));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState == ENABLE) {
    SET_BIT(CKGEN_SOC->CLOCK_EN, PeriphClock);
  }
  else {
    CLEAR_BIT(CKGEN_SOC->CLOCK_EN, PeriphClock);
  }
}

/**
  * @brief  Get the wakeup reason.
  * @param  None
  * @retval RESET_REASON_Type: wakeup reason @ref RESET_REASON_Type
  */
RESET_REASON_Type SysCtrl_GetWakeupResetReason(void)
{
  uint32_t tmpSoc, tmpBle;
  
  tmpSoc = CKGEN_SOC->REASON_RST;
  tmpBle = CKGEN_BLE->REASON_RST;

  if ((tmpSoc == 0) &&
      (tmpBle >= BLE_REASON_FROM_IO9) && 
      (tmpBle <= BLE_REASON_FROM_IO13)) {
    if ((tmpBle & BLE_REASON_FROM_IO9) == BLE_REASON_FROM_IO9) {
      return RESET_BLE_WAKEUP_FROM_IO9;
    }
    if ((tmpBle & BLE_REASON_FROM_IO10) == BLE_REASON_FROM_IO10) {
      return RESET_BLE_WAKEUP_FROM_IO10;
    }
    if ((tmpBle & BLE_REASON_FROM_IO11) == BLE_REASON_FROM_IO11) {
      return RESET_BLE_WAKEUP_FROM_IO11;
    }
    if ((tmpBle & BLE_REASON_FROM_IO12) == BLE_REASON_FROM_IO12) {
      return RESET_BLE_WAKEUP_FROM_IO12;
    }
    if ((tmpBle & BLE_REASON_FROM_IO13) == BLE_REASON_FROM_IO13) {
      return RESET_BLE_WAKEUP_FROM_IO13;
    }
  }

  if ((tmpSoc == 0) &&
      ((tmpBle & BLE_REASON_FROM_TIMER1) == BLE_REASON_FROM_TIMER1)) {
    return RESET_BLE_WAKEUP_FROM_TIMER1;
  }
  if ((tmpSoc == 0) &&
      ((tmpBle & BLE_REASON_FROM_TIMER2) == BLE_REASON_FROM_TIMER2)) {
    return RESET_BLE_WAKEUP_FROM_TIMER2;
  }
  if ((tmpSoc == 0) &&
      ((tmpBle & BLE_REASON_POR_RST) == BLE_REASON_POR_RST)) {
    return RESET_BLE_POR;
  }
  if ((tmpSoc == 0) &&
      ((tmpBle & BLE_REASON_BOR_RST) == BLE_REASON_BOR_RST)) {
    return RESET_BLE_BOR;
  }
  if (tmpSoc == SOC_REASON_SYSREQ) {
    return RESET_SYSREQ;
  }
  if (tmpSoc == SOC_REASON_WDG) {
    return RESET_WDG;
  }
  if((tmpSoc == SOC_REASON_LOCKUP)) {
    return RESET_LOCKUP;
  }

  return RESET_NONE;
}


/**
* @brief  Configure the wakeup source from IO.
* @param  IO: the IO selected as wakeup source, the value can be 
*         @arg WAKEUP_ON_IO9 Wakeup source is IO9
*         @arg WAKEUP_ON_IO10 Wakeup source is IO10
*         @arg WAKEUP_ON_IO11 Wakeup source is IO11
*         @arg WAKEUP_ON_IO12 Wakeup source is IO12
*         @arg WAKEUP_ON_IO13 Wakeup source is IO13
* @param  LevelState: select the level of the wakeup source, the value can be 
*         @arg WAKEUP_ON_HIGH_STATE Wakeup on high state
*         @arg WAKEUP_ON_LOW_STATE Wakeup on low state
* @param  NewState: functional state @ref FunctionalState
*         This parameter can be: ENABLE or DISABLE.
* @retval  None
*/
void SysCtrl_WakeupFromIo(uint8_t IO, uint8_t LevelState, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_WAKEUP_ON_IO(IO));
  assert_param(IS_WAKEUP_ON_STATE(LevelState));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(LevelState == WAKEUP_ON_HIGH_STATE) {
    SET_BIT(SYSTEM_CTRL->WKP_IO_IS, IO);
  }
  else {
    CLEAR_BIT(SYSTEM_CTRL->WKP_IO_IS, IO);
  }
  
  if(NewState == ENABLE) {
    SET_BIT(SYSTEM_CTRL->WKP_IO_IE, IO);
  }
  else {    
    CLEAR_BIT(SYSTEM_CTRL->WKP_IO_IE, IO);
  }
  
}

/**
  * @brief  Enable disable the XO clock divider for the system control.
  * @param  XOFreq: configure the system for the correct XO external clock used, the value can be
*         @arg XO_32MHz enable the XO divider and expect a 32 MHz external clock.
*         @arg XO_16MHz disable the XO divider and expect a 16 MHz external clock.
  * @retval None
  */
void SysCtrl_SelectXO(uint8_t XOFreq)
{  
  /* Check the parameters */
  assert_param(IS_XO_FREQ(XOFreq));

  if(XOFreq == XO_32MHZ) {
    SYSTEM_CTRL->CTRL_b.MHZ32_SEL = SET;
  }
  else if(XOFreq == XO_16MHZ) {   
    SYSTEM_CTRL->CTRL_b.MHZ32_SEL = RESET;
  }
}



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
