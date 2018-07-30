/** mbed Microcontroller Library
  ******************************************************************************
  * @file    sleep_ex_api.h
  * @author 
  * @version V1.0.0
  * @brief   This file provides mbed API for SLEEP.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015, Realtek Semiconductor Corp.
  * All rights reserved.
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  ****************************************************************************** 
  */
#ifndef MBED_SLEEP_EX_API_H
#define MBED_SLEEP_EX_API_H
 
#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup sleep_ex SLEEP_EX
 *  @ingroup    hal
 *  @brief      sleep extended functions
 *  @{
 */

#if defined(CONFIG_PLATFORM_8195A) && (CONFIG_PLATFORM_8195A == 1)
///@name Ameba1 Only 
///@{
/* Sleep Eake Up event, the User application also 
   need to config the peripheral to trigger wake up event */
#define SLEEP_WAKEUP_BY_STIMER            (SLP_STIMER)    // wake up by system timer
#define SLEEP_WAKEUP_BY_GTIMER            (SLP_GTIMER)    // wake up by General purpose timer timeout
#define SLEEP_WAKEUP_BY_GPIO_INT          (SLP_GPIO)      // wake up by GPIO Port A[7:0] Interrupt
#define SLEEP_WAKEUP_BY_WLAN              (SLP_WL)        // wake up by WLan event
#define SLEEP_WAKEUP_BY_NFC               (SLP_NFC)       // wake up by NFC event
#define SLEEP_WAKEUP_BY_SDIO              (SLP_SDIO)      // wake up by SDIO event
#define SLEEP_WAKEUP_BY_USB               (SLP_USB)       // wake up by USB event

// Deep Standby Wakeup event
#define STANDBY_WAKEUP_BY_STIMER      (BIT0)     // wake up by system timer   
#define STANDBY_WAKEUP_BY_NFC         (BIT1)     // wake up by NFC event
//#define SLEEP_WAKEUP_BY_DS_TIMER    (BIT2)     // The timer to wakeup from Deep Sleep timer
// Do not modify these definition, or need to modify the code also.
#define STANDBY_WAKEUP_BY_PA5         (BIT4)     // GPIO Port A[5] 
#define STANDBY_WAKEUP_BY_PC7         (BIT5)     // GPIO Port C[7] 
#define STANDBY_WAKEUP_BY_PD5         (BIT6)     // GPIO Port D[5] 
#define STANDBY_WAKEUP_BY_PE3         (BIT7)     // GPIO Port E[3] 

// Deep Sleep Wakeup event
#define DSLEEP_WAKEUP_BY_TIMER     (DS_TIMER33)
#define DSLEEP_WAKEUP_BY_GPIO      (DS_GPIO)    // GPIO Port B[1] 

typedef struct _SLEEP_WKUP_EVENT_ {
    u8 wakeup_event;    // Wake up event: Timer, NFC, GPIO
    u8 gpio_option;     // GPIO Wakeup setting: [3:0]: Pin 3~0 enable, [7:4]: pin3~0 active high/low
    u32 timer_duration; // the sleep duration and then wakeup
} SLEEP_WAKEUP_EVENT, *PSLEEP_WAKEUP_EVENT;
///@}
#endif //CONFIG_PLATFORM_8195A

#if defined(CONFIG_PLATFORM_8711B) && (CONFIG_PLATFORM_8711B == 1)
///@name AmebaZ Only 
///@{
/* Sleep Eake Up event, the User application also 
   need to config the peripheral to trigger wake up event */
#define SLEEP_WAKEUP_BY_STIMER			(BIT_SYSON_WEVT_SYSTIM_MSK)    // wake up by system timer
#define SLEEP_WAKEUP_BY_GPIO_INT		(BIT_SYSON_WEVT_GPIO_MSK)      // wake up by GPIO Port A[31:0] Interrupt
#define SLEEP_WAKEUP_BY_WLAN			(BIT_SYSON_WEVT_WLAN_MSK)        // wake up by WLan event
#define SLEEP_WAKEUP_BY_SDIO			(BIT_SYSON_WEVT_SDIO_MSK)      // wake up by SDIO event
#define SLEEP_WAKEUP_BY_USB				(BIT_SYSON_WEVT_USB_MSK)       // wake up by USB event
#define SLEEP_WAKEUP_BY_GPIO			(BIT_SYSON_WEVT_GPIO_DSTBY_MSK)       // GPIO Port(PA_18, PA_5, PA_22, PA_23)
#define SLEEP_WAKEUP_BY_UART			(BIT_SYSON_WEVT_UART0_MSK | BIT_SYSON_WEVT_UART1_MSK)       // wake up by UART event
#define SLEEP_WAKEUP_BY_I2C				(BIT_SYSON_WEVT_I2C0_MSK | BIT_SYSON_WEVT_I2C1_MSK)		 // wake up by I2C event
#define SLEEP_WAKEUP_BY_RTC				(BIT_SYSON_WEVT_RTC_MSK)       // wake up by RTC event

// Deep Standby Wakeup event
#define STANDBY_WAKEUP_BY_STIMER		(BIT_SYSON_WEVT_A33_AND_A33GPIO_MSK)     // wake up by 1K timer   
#define STANDBY_WAKEUP_BY_GPIO			(BIT_SYSON_WEVT_GPIO_DSTBY_MSK)     // GPIO Port(PA_18, PA_5, PA_22, PA_23)
#define STANDBY_WAKEUP_BY_RTC			(BIT_SYSON_WEVT_RTC_MSK)       // wake up by RTC event

// Deep Sleep Wakeup event
#define DSLEEP_WAKEUP_BY_TIMER		BIT(0)
#define DSLEEP_WAKEUP_BY_GPIO			BIT(2)    // GPIO Port(PA_18, PA_5, PA_22, PA_23)

typedef struct _SLEEP_WKUP_EVENT_ {
    u32 wakeup_event;    // Wake up event: Timer, NFC, GPIO
    u32 gpio_option;     // GPIO Wakeup setting: [3:0]: Pin 3~0 enable, [11:8]: pin3~0 active high/low
    u32 timer_duration; // the sleep duration and then wakeup
} SLEEP_WAKEUP_EVENT, *PSLEEP_WAKEUP_EVENT;
///@}
#endif //CONFIG_PLATFORM_8711B

///@name Ameba Common
///@{

/**
  * @brief  To make the system entering the Clock Gated power saving.
  *         This function just make the system to enter the clock gated 
  *         power saving mode and pending on wake up event waitting.
  *         The user application need to configure the peripheral to 
  *         generate system wake up event, like GPIO interrupt, 
  *         G-Timer timeout, etc. befor entering power saving mode.
  * @param  wakeup_event: A bit map of wake up event.
  *   This parameter can be any combination of the following values:
  *		 @arg SLEEP_WAKEUP_BY_STIMER
  *		 @arg SLEEP_WAKEUP_BY_GTIMER
  *		 @arg SLEEP_WAKEUP_BY_GPIO_INT
  *		 @arg SLEEP_WAKEUP_BY_WLAN
  *		 @arg SLEEP_WAKEUP_BY_SDIO
  *		 @arg SLEEP_WAKEUP_BY_USB
  *		 @arg SLEEP_WAKEUP_BY_GPIO
  *		 @arg SLEEP_WAKEUP_BY_UART
  *		 @arg SLEEP_WAKEUP_BY_I2C
  *		 @arg SLEEP_WAKEUP_BY_RTC
  *		 @arg SLEEP_WAKEUP_BY_RESETPIN
  * @param  sleep_duration: the system sleep duration in ms, only valid
  *         for SLEEP_WAKEUP_BY_STIMER wake up event.
  * @retval None
  */
void sleep_ex(uint32_t wakeup_event,  uint32_t sleep_duration);

/**
  * @brief  To make the system entering the Clock Gated power saving.
  *         This function just make the system to enter the clock gated 
  *         power saving mode and pending on wake up event waitting.
  *         The user application need to configure the peripheral to 
  *         generate system wake up event, like GPIO interrupt
  *         , G-Timer timeout, etc. befor entering power saving mode.
  * @param  wakeup_event: A bit map of wake up event. 
  *   This parameter can be any combination of the following values:
  *		 @arg SLEEP_WAKEUP_BY_STIMER
  *		 @arg SLEEP_WAKEUP_BY_GTIMER
  *		 @arg SLEEP_WAKEUP_BY_GPIO_INT
  *		 @arg SLEEP_WAKEUP_BY_WLAN
  *		 @arg SLEEP_WAKEUP_BY_SDIO
  *		 @arg SLEEP_WAKEUP_BY_USB
  *		 @arg SLEEP_WAKEUP_BY_GPIO
  *		 @arg SLEEP_WAKEUP_BY_UART
  *		 @arg SLEEP_WAKEUP_BY_I2C
  *		 @arg SLEEP_WAKEUP_BY_RTC
  *		 @arg SLEEP_WAKEUP_BY_RESETPIN
  * @param  sleep_duration: the system sleep duration in ms, only valid
  *         for SLEEP_WAKEUP_BY_STIMER wake up event.
  * @param  clk_sourec_enable: the option for SCLK on(1)/off(0)
  * @param  sdr_enable: the option for turn off the SDR controller (1:off, 0:on)
  * @retval None
  */
void sleep_ex_selective(uint32_t wakeup_event,  uint32_t sleep_duration, uint32_t clk_sourec_enable, uint32_t sdr_enable);

#if defined(CONFIG_PLATFORM_8195A) && (CONFIG_PLATFORM_8195A == 1)
///@name Ameba1 Only 
///@{
/**
  * @brief  To make the system entering the Deep Standby power saving.
  *         The CPU, memory and part fo peripheral power is off when 
  *         entering deep standby power saving mode. The program needs 
  *         to be reload from the flash at system resume.
  * @retval None
  */
void deepstandby_ex(void);
///@}
#endif //CONFIG_PLATFORM_8195A

#if defined(CONFIG_PLATFORM_8711B) && (CONFIG_PLATFORM_8711B == 1)
///@name AmebaZ Only 
///@{
/**
  * @brief  To make the system entering the Deep Standby power saving.
  *         The CPU, memory and part fo peripheral power is off when 
  *         entering deep standby power saving mode. The program needs 
  *         to be reload from the flash at system resume.
  * @param  sleep_duration_ms: the system sleep duration in ms, only valid
  *             for STANDBY_WAKEUP_BY_STIMER wake up event.
  * @retval None
  */
void deepstandby_ex(uint32_t sleep_duration_ms);
///@}
#endif //CONFIG_PLATFORM_8711B

/**
  * @brief  To make the system entering the Deep Sleep power saving mode.
  *         The CPU, memory and peripheral power is off when entering
  *         deep sleep power saving mode. The program needs to be reload
  *         and all peripheral needs be re-configure when system resume.
  * @param  wakeup_event: A bit map of wake up event.
  *   This parameter can be any combination of the following values:
  *		 @arg DSLEEP_WAKEUP_BY_TIMER
  *		 @arg DSLEEP_WAKEUP_BY_GPIO
  * @param  sleep_duration: the system sleep duration in ms, only valid
  *         for DSLEEP_WAKEUP_BY_TIMER wake up event.
  * @retval None
  */
void deepsleep_ex(uint32_t wakeup_event, uint32_t sleep_duration);

#if defined(CONFIG_PLATFORM_8195A) && (CONFIG_PLATFORM_8195A == 1)
///@name Ameba1 Only 
///@{
/**
  * @brief  To add a wake up event to wake up the system from the
  *         deep standby power saving mode.
  * @param  wakeup_event: A bit map of wake up event.
  *   This parameter can be any combination of the following values:
  *		 @arg STANDBY_WAKEUP_BY_STIMER
  *		 @arg STANDBY_WAKEUP_BY_GPIO
  *		 @arg STANDBY_WAKEUP_BY_RTC
  *		 @arg STANDBY_WAKEUP_BY_RESETPIN
  * @param  sleep_duration_ms: the system sleep duration in ms, only valid
  *             for STANDBY_WAKEUP_BY_STIMER wake up event.
  * @param  gpio_option: for a GPIO pin to wake up the system by goes high or low
  *   This parameter can be any combination of the following values:
  *		 @arg WAKEUP_BY_GPIO_NONE	
  *		 @arg WAKEUP_BY_GPIO_WAKEUP0_LOW
  *		 @arg WAKEUP_BY_GPIO_WAKEUP0_HIG
  *		 @arg WAKEUP_BY_GPIO_WAKEUP1_LOW
  *		 @arg WAKEUP_BY_GPIO_WAKEUP1_HIG
  *		 @arg WAKEUP_BY_GPIO_WAKEUP2_LOW
  *		 @arg WAKEUP_BY_GPIO_WAKEUP2_HIG
  *		 @arg WAKEUP_BY_GPIO_WAKEUP3_LOW
  *		 @arg WAKEUP_BY_GPIO_WAKEUP3_HIG
  * @retval None
  */
void standby_wakeup_event_add(uint32_t wakeup_event, uint32_t sleep_duration_ms, uint32_t gpio_active);
///@}
#endif //CONFIG_PLATFORM_8195A

#if defined(CONFIG_PLATFORM_8711B) && (CONFIG_PLATFORM_8711B == 1)
///@name AmebaZ Only 
///@{
/**
  * @brief  To add a wake up event to wake up the system from the
  *         deep standby power saving mode.
  * @param  wakeup_event: A bit map of wake up event.
  *   This parameter can be any combination of the following values:
  *		 @arg STANDBY_WAKEUP_BY_STIMER
  *		 @arg STANDBY_WAKEUP_BY_GPIO
  *		 @arg STANDBY_WAKEUP_BY_RTC
  *		 @arg STANDBY_WAKEUP_BY_RESETPIN
  * @param  gpio_option: for a GPIO pin to wake up the system by goes high or low
  *   This parameter can be any combination of the following values:
  *		 @arg WAKEUP_BY_GPIO_NONE	
  *		 @arg WAKEUP_BY_GPIO_WAKEUP0_LOW
  *		 @arg WAKEUP_BY_GPIO_WAKEUP0_HIG
  *		 @arg WAKEUP_BY_GPIO_WAKEUP1_LOW
  *		 @arg WAKEUP_BY_GPIO_WAKEUP1_HIG
  *		 @arg WAKEUP_BY_GPIO_WAKEUP2_LOW
  *		 @arg WAKEUP_BY_GPIO_WAKEUP2_HIG
  *		 @arg WAKEUP_BY_GPIO_WAKEUP3_LOW
  *		 @arg WAKEUP_BY_GPIO_WAKEUP3_HIG
  * @retval None
  */
void standby_wakeup_event_add(uint32_t wakeup_event, uint32_t gpio_active);
///@}
#endif //CONFIG_PLATFORM_8711B

/**
  * @brief  To delete a wake up event for wakeing up the system from the
  *         deep standby power saving mode.
  * @param  wakeup_event: A bit map of wake up event.
  *   This parameter can be any combination of the following values:
  *		 @arg STANDBY_WAKEUP_BY_STIMER
  *		 @arg STANDBY_WAKEUP_BY_GPIO
  *		 @arg STANDBY_WAKEUP_BY_RTC
  *		 @arg STANDBY_WAKEUP_BY_RESETPIN
  * @retval None
  */
void standby_wakeup_event_del(uint32_t wakeup_event);

///@}


#ifdef __cplusplus
}
#endif

//#endif
/*\@}*/
#endif
