/**
  ******************************************************************************
  * @file    BlueNRG1_sysCtrl.h
  * @author  VMA Application Team
  * @version V2.0.0
  * @date    21-March-2016
  * @brief   This file contains all the functions prototypes for the System Controller 
  *          firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_SYSCTRL_H
#define BLUENRG1_SYSCTRL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup SystemControl  System Control
  * @{
  */

/** @defgroup SystemControl_Exported_Types Exported Types
  * @{
  */


/** 
  * @brief System Reset and Wakeup Sources
  */ 
typedef enum
{
  RESET_NONE,
  RESET_SYSREQ,
  RESET_WDG,
  RESET_LOCKUP,
  RESET_BLE_BOR,
  RESET_BLE_POR,
  RESET_BLE_WAKEUP_FROM_IO9,
  RESET_BLE_WAKEUP_FROM_IO10,
  RESET_BLE_WAKEUP_FROM_IO11,
  RESET_BLE_WAKEUP_FROM_IO12,
  RESET_BLE_WAKEUP_FROM_IO13,
  RESET_BLE_WAKEUP_FROM_TIMER1,
  RESET_BLE_WAKEUP_FROM_TIMER2,
} RESET_REASON_Type;


/**
  * @}
  */

/** @defgroup SystemControl_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup System_Clock_Frequency System Clock Frequency
  * @{
  */
#if (HS_SPEED_XTAL == HS_SPEED_XTAL_32MHZ)
  #define SYST_CLOCK 	32000000            /* System clock frequency */ 
#elif (HS_SPEED_XTAL == HS_SPEED_XTAL_16MHZ)
   #define SYST_CLOCK 	16000000            /* System clock frequency */ 
#else
#error "No definition for SYST_CLOCK"
#endif

/**
  * @}
  */


/** @defgroup Peripherals_Clock Peripherals Clock
  * @{
  */
#define CLOCK_PERIPH_GPIO                ((uint32_t)0x00000001)
#define CLOCK_PERIPH_NVM                 ((uint32_t)0x00000002)
#define CLOCK_PERIPH_SYS_CONTROL         ((uint32_t)0x00000004)
#define CLOCK_PERIPH_UART                ((uint32_t)0x00000008)
#define CLOCK_PERIPH_SPI                 ((uint32_t)0x00000010)
#define CLOCK_PERIPH_WDG                 ((uint32_t)0x00000080)
#define CLOCK_PERIPH_ADC                 ((uint32_t)0x00000100)
#define CLOCK_PERIPH_I2C1                ((uint32_t)0x00000200)
#define CLOCK_PERIPH_I2C2                ((uint32_t)0x00000400)
#define CLOCK_PERIPH_MTFX1               ((uint32_t)0x00000800)
#define CLOCK_PERIPH_MTFX2               ((uint32_t)0x00001000)
#define CLOCK_PERIPH_RTC                 ((uint32_t)0x00002000)
#define CLOCK_PERIPH_DMA                 ((uint32_t)0x00010000)
#define CLOCK_PERIPH_RNG                 ((uint32_t)0x00020000)
#define CLOCK_PERIPH_PKA                 ((uint32_t)0x000C0000)

#define IS_CLOCK_PERIPH(PERIPH) (((PERIPH) & 0x000F3F9F) != 0)
/**
  * @}
  */


  
/** @defgroup IO_sources_for_wakeup   IO sources for wakeup
  * @{
  */
  
#define WAKEUP_ON_IO9       ((uint8_t)0x01)    /* Wakeup source is IO9 */
#define WAKEUP_ON_IO10      ((uint8_t)0x02)    /* Wakeup source is IO10 */
#define WAKEUP_ON_IO11      ((uint8_t)0x04)    /* Wakeup source is IO11 */
#define WAKEUP_ON_IO12      ((uint8_t)0x08)    /* Wakeup source is IO12 */
#define WAKEUP_ON_IO13      ((uint8_t)0x10)    /* Wakeup source is IO13 */

#define WAKEUP_ON_HIGH_STATE    ((uint8_t)0)   /* Wakeup on high state */
#define WAKEUP_ON_LOW_STATE     ((uint8_t)1)   /* Wakeup on low state */

#define IS_WAKEUP_ON_IO(IOS) (((IOS) & 0x1F) != 0)

#define IS_WAKEUP_ON_STATE(STATE) ( ((STATE) == WAKEUP_ON_HIGH_STATE) || ((STATE) == WAKEUP_ON_LOW_STATE) )

/**
  * @}
  */
  

/** @defgroup External_Clock_Selection   External Clock Selection
  * @{
  */

#define XO_32MHZ     ((uint8_t)1)    /* The external clock XO used is a 32 MHz */
#define XO_16MHZ     ((uint8_t)0)    /* The external clock XO used is a 16 MHz */

#define IS_XO_FREQ(XO) ( ((XO) == XO_32MHZ) || ((XO) == XO_16MHZ) )

/**
  * @}
  */


/**
  * @}
  */



/** @defgroup SystemControl_Exported_Functions Exported Functions
  * @{
  */

void SysCtrl_DeInit(void);
void SysCtrl_PeripheralClockCmd(uint32_t PeriphClock, FunctionalState NewState);
RESET_REASON_Type SysCtrl_GetWakeupResetReason(void);

void SysCtrl_WakeupFromIo(uint8_t IO, uint8_t LevelState, FunctionalState NewState);
void SysCtrl_SelectXO(uint8_t XOFreq);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* BLUENRG1_SYSCTRL_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
