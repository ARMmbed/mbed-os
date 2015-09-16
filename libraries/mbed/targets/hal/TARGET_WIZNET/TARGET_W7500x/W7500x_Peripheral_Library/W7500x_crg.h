/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   This file contains all the functions prototypes for the Clock Reset Generator 
  *          firmware library.
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __W7500X_CRG_H
#define __W7500X_CRG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "W7500x.h"

// It will be in W7500x_crg.h
typedef enum { CRG_CLK_DIS = 0, CRG_MCLK, CRG_RCLK, CRG_OCLK } CRG_CLK_SOURCE;
typedef enum { CRG_CLK_HIGH = 0, CRG_CLK_LOW }  CRG_CLK_LOW_SOURCE;
typedef enum { CRG_PREDIV1 = 0, CRG_PREDIV2, CRG_PREDIV4, CRG_PREDIV8, CRG_PREDIV16, \
               CRG_PREDIV32, CRG_PREDIV64, CRG_PREDIV128 } CRG_PREDIV;
typedef enum { CRG_TIMER0 = 0, CRG_TIMER1 } CRG_TIMER;
typedef enum { CRG_PWM0 = 0, CRG_PWM1, CRG_PWM2, CRG_PWM3, CRG_PWM4, CRG_PWM5, CRG_PWM6, CRG_PWM7 } CRG_PWM;



#define IS_CRG_PLL_SRC(SRC)          (((SRC) == CRG_RCLK) || ((SRC) == CRG_OCLK))
#define IS_CRG_FCLK_SRC(SRC)         (((SRC) == CRG_MCLK) || ((SRC) == CRG_RCLK) || ((SRC) == CRG_OCLK))

#define CRG_CLK_SRC_DEFAULT(SRC)     (((SRC) == CRG_CLK_DIS) || ((SRC) == CRG_MCLK) || \
                                      ((SRC) == CRG_RCLK) || ((SRC) == CRG_OCLK))
#define CRG_CLK_SRC_LOW(SRC)         (((SRC) == CRG_CLK_HIGH) || ((SRC) == CRG_CLK_LOW))

#define CRG_CLK_PREDIV_DEFAULT(DIV)  (((DIV) == CRG_PREDIV1) || ((DIV) == CRG_PREDIV2) || \
                                      ((DIV) == CRG_PREDIV4) || ((DIV) == CRG_PREDIV8))
#define CRG_CLK_PREDIV_ADVANCE(DIV)  (((DIV) == CRG_PREDIV1)  || ((DIV) == CRG_PREDIV2)  || \
                                      ((DIV) == CRG_PREDIV4)  || ((DIV) == CRG_PREDIV8)  || \
                                      ((DIV) == CRG_PREDIV16) || ((DIV) == CRG_PREDIV32) || \
                                      ((DIV) == CRG_PREDIV64)  || ((DIV) == CRG_PREDIV128))


#define IS_CRG_FCLK_PREDIV(DIV)     CRG_CLK_PREDIV_DEFAULT(DIV) 

#define IS_CRG_SSPCLK_SRC(SRC)      CRG_CLK_SRC_DEFAULT(SRC)
#define IS_CRG_SSPCLK_PREDIV(DIV)   CRG_CLK_PREDIV_DEFAULT(DIV)

#define IS_CRG_ADCCLK_PREDIV(DIV)   CRG_CLK_PREDIV_DEFAULT(DIV)
#define IS_CRG_ADCCLK_SRC(SRC)      CRG_CLK_SRC_DEFAULT(SRC)





#define IS_CRG_TIMERCLK_NUM(NUM)            (((NUM) == CRG_TIMER0) || ((NUM) == CRG_TIMER1))
#define IS_CRG_TIMERCLK_SRC(SRC)            CRG_CLK_SRC_DEFAULT(SRC)
#define IS_CRG_TIMERCLK_PREDIV(DIV)         CRG_CLK_PREDIV_ADVANCE(DIV)

#define CRG_SET_TIMERCLK_SSR(NUM,VALUE) \
            (*((volatile uint32_t *)(CRG->TIMER0CLK_SSR) + (0x10ul * NUM)) = VALUE)
#define CRG_SET_TIMERCLK_PREDIV(NUM,VALUE) \
            (*((volatile uint32_t *)(CRG->TIMER0CLK_PVSR) + (0x10ul * NUM)) = VALUE)
//#define CRG_SET_TIMERCLK_SSR(num,value)     CRG->TIMER##num##CLK_SSR = value
//#define CRG_SET_TIMERCLK_PREDIV(num,value)  CRG->TIMER##num##CLK_PVSR = value


#define IS_CRG_PWMCLK_NUM(NUM)              (((NUM) == CRG_PWM0) || ((NUM) == CRG_PWM1) || \
                                             ((NUM) == CRG_PWM2) || ((NUM) == CRG_PWM3) || \
                                             ((NUM) == CRG_PWM4) || ((NUM) == CRG_PWM5) || \
                                             ((NUM) == CRG_PWM6) || ((NUM) == CRG_PWM7) )
#define IS_CRG_PWMCLK_SRC(SRC)              CRG_CLK_SRC_DEFAULT(SRC)
#define IS_CRG_PWMCLK_PREDIV(DIV)           CRG_CLK_PREDIV_ADVANCE(DIV)

#define CRG_SET_PWMCLK_SSR(NUM,VALUE)       \
            (*((volatile uint32_t *)(CRG->PWM0CLK_SSR) + (0x10ul * NUM)) = VALUE)
#define CRG_SET_PWMCLK_PREDIV(NUM,VALUE)    \
            (*((volatile uint32_t *)(CRG->PWM0CLK_PVSR) + (0x10ul * NUM)) = VALUE)
//#define CRG_SET_PWMCLK_SSR(num,value)       CRG->PWM##num##CLK_SSR = value
//#define CRG_SET_PWMCLK_PREDIV(num,value)    CRG->PWM##num##CLK_PVSR = value

#define IS_CRG_RTC_HS_SRC(SRC)              CRG_CLK_SRC_DEFAULT(SRC)
#define IS_CRG_RTC_HS_PREDIV(DIV)           CRG_CLK_PREDIV_ADVANCE(DIV)
#define IS_CRG_RTC_LOW_SRC(SRC)             CRG_CLK_SRC_LOW(SRC) 

#define IS_CRG_WDOGCLK_HS_SRC(SRC)          CRG_CLK_SRC_DEFAULT(SRC)
#define IS_CRG_WDOGCLK_HS_PREDIV(DIV)       CRG_CLK_PREDIV_ADVANCE(DIV)
#define IS_CRG_WDOGCLK_LOW_SRC(SRC)         CRG_CLK_SRC_LOW(SRC)

#define IS_CRG_UARTCLK_SRC(SRC)             CRG_CLK_SRC_DEFAULT(SRC)
#define IS_CRG_UARTCLK_PREDIV(DIV)          CRG_CLK_PREDIV_DEFAULT(DIV)

#define IS_CRG_MONCLK_SSR(value)            (((value) >= 0x00ul) || ((value) <= 0x13ul))

void CRG_DeInit(void);
void CRG_OSC_PowerDownEnable        (FunctionalState NewState);
void CRG_PLL_PowerDownEnable        (FunctionalState NewState);
void CRG_PLL_OutputEnable           (FunctionalState NewState);
void CRG_PLL_BypassEnable           (FunctionalState NewState);
void CRG_PLL_InputFrequencySelect   (CRG_CLK_SOURCE src);

void CRG_FCLK_SourceSelect          (CRG_CLK_SOURCE src);
void CRG_FCLK_SetPrescale           (CRG_PREDIV prediv);

void CRG_SSPCLK_SourceSelect    (CRG_CLK_SOURCE src);
void CRG_SSPCLK_SetPrescale     (CRG_PREDIV prediv);

void CRG_ADCCLK_SourceSelect    (CRG_CLK_SOURCE src);
void CRG_ADCCLK_SetPrescale     (CRG_PREDIV prediv);

void CRG_TIMERCLK_SourceSelect  (CRG_TIMER num, CRG_CLK_SOURCE src);
void CRG_TIMERCLK_SetPrescale   (CRG_TIMER num, CRG_PREDIV prediv);

void CRG_PWMCLK_SourceSelect    (CRG_PWM num, CRG_CLK_SOURCE src);
void CRG_PWMCLK_SetPrescale     (CRG_PWM num, CRG_PREDIV prediv);

void CRG_RTC_HS_SourceSelect    (CRG_CLK_SOURCE src);
void CRG_RTC_HS_SetPrescale     (CRG_PREDIV prediv);
void CRG_RTC_SourceSelect       (CRG_CLK_LOW_SOURCE src);

void CRG_WDOGCLK_HS_SourceSelect    (CRG_CLK_SOURCE src);
void CRG_WDOGCLK_HS_SetPrescale     (CRG_PREDIV prediv);
void CRG_WDOGCLK_SourceSelect       (CRG_CLK_LOW_SOURCE src);

void CRG_UARTCLK_SourceSelect   (CRG_CLK_SOURCE src);
void CRG_UARTCLK_SetPrescale    (CRG_PREDIV prediv);

void CRG_MII_Enable (FunctionalState rx_clk, FunctionalState tx_clk);

void    CRG_SetMonitoringClock  (uint32_t value);
uint32_t CRG_GetMonitoringClock  (void);


#ifdef __cplusplus
}
#endif

#endif

