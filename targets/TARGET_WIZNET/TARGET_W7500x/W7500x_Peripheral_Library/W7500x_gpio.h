/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   This file contains all the functions prototypes for the GPIO
  *          firmware library.
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __W7500X_HAL_GPIO_H
#define __W7500X_HAL_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "W7500x.h"

#define GPIO_Pin_0      (uint16_t)(0x01 <<  0)     // Pin  0 Selected
#define GPIO_Pin_1      (uint16_t)(0x01 <<  1)     // Pin  1 Selected
#define GPIO_Pin_2      (uint16_t)(0x01 <<  2)     // Pin  2 Selected
#define GPIO_Pin_3      (uint16_t)(0x01 <<  3)     // Pin  3 Selected
#define GPIO_Pin_4      (uint16_t)(0x01 <<  4)     // Pin  4 Selected
#define GPIO_Pin_5      (uint16_t)(0x01 <<  5)     // Pin  5 Selected
#define GPIO_Pin_6      (uint16_t)(0x01 <<  6)     // Pin  6 Selected
#define GPIO_Pin_7      (uint16_t)(0x01 <<  7)     // Pin  7 Selected
#define GPIO_Pin_8      (uint16_t)(0x01 <<  8)     // Pin  8 Selected
#define GPIO_Pin_9      (uint16_t)(0x01 <<  9)     // Pin  9 Selected
#define GPIO_Pin_10     (uint16_t)(0x01 << 10)     // Pin 10 Selected
#define GPIO_Pin_11     (uint16_t)(0x01 << 11)     // Pin 11 Selected
#define GPIO_Pin_12     (uint16_t)(0x01 << 12)     // Pin 12 Selected
#define GPIO_Pin_13     (uint16_t)(0x01 << 13)     // Pin 13 Selected
#define GPIO_Pin_14     (uint16_t)(0x01 << 14)     // Pin 14 Selected
#define GPIO_Pin_15     (uint16_t)(0x01 << 15)     // Pin 15 Selected
#define GPIO_Pin_All    (uint16_t)(0xFFFF)         // All pins Selected


#define IS_GPIO_PIN(PIN)        ((PIN) != (uint16_t)0x00)
#define IS_GET_GPIO_PIN(PIN)    (((PIN) == GPIO_Pin_0)  || ((PIN) == GPIO_Pin_1)  || \
                                (((PIN) == GPIO_Pin_2)  || ((PIN) == GPIO_Pin_3)  || \
                                (((PIN) == GPIO_Pin_4)  || ((PIN) == GPIO_Pin_5)  || \
                                (((PIN) == GPIO_Pin_6)  || ((PIN) == GPIO_Pin_7)  || \
                                (((PIN) == GPIO_Pin_8)  || ((PIN) == GPIO_Pin_9)  || \
                                (((PIN) == GPIO_Pin_10) || ((PIN) == GPIO_Pin_11) || \
                                (((PIN) == GPIO_Pin_12) || ((PIN) == GPIO_Pin_13) || \
                                (((PIN) == GPIO_Pin_14) || ((PIN) == GPIO_Pin_15) )


#define IS_GPIO_ALL_PERIPH(PERIPH)  (((PERIPH) == GPIOA) || ((PERIPH) == GPIOB) || \
                                     ((PERIPH) == GPIOC) || ((PERIPH) == GPIOD) )

typedef enum
{
    GPIO_Mode_IN    = 0x00, /*!< GPIO Input Mode                 */
    GPIO_Mode_OUT   = 0x01, /*!< GPIO Output Mode                */
    GPIO_Mode_AF    = 0x02 /*!< GPIO Alternate function Mode    */
}GPIOMode_TypeDef;

typedef enum
{
    GPIO_NO_PUPD = 0x0ul,
    GPIO_PuPd_UP = Px_PCR_PUPD_UP,
    GPIO_PuPd_DOWN = Px_PCR_PUPD_DOWN,
    GPIO_PuPd_Default = 0x0ul,
}GPIOPad_TypeDef;

typedef struct
{
    uint32_t GPIO_Pin;
    GPIOMode_TypeDef GPIO_Mode;
    GPIOPad_TypeDef GPIO_Pad;
}GPIO_InitTypeDef;

typedef enum
{
    Bit_RESET = 0,
    Bit_SET
}BitAction;


#define IS_PAD_TYPE(Px)  (((Px) == PAD_PA) || ((Px) == PAD_PB) \
                          ((Px) == PAD_PC) || ((Px) == PAD_PD))

#define IS_PA_NUM(NUM) (((NUM)>=0) && ((NUM)<16))
#define IS_PB_NUM(NUM) (((NUM)>=0) && ((NUM)<16))
#define IS_PC_NUM(NUM) (((NUM)>=0) && ((NUM)<16))
#define IS_PD_NUM(NUM) (((NUM)>=0) && ((NUM)< 5))

#define IS_PAD_AF(AF)       (((AF) == PAD_AF0) || ((AF) == PAD_AF1) || \
                             ((AF) == PAD_AF2) || ((AF) == PAD_AF3))


#define IS_GPIO_BIT_ACTION(ACTION)  (((ACTION_ == Bit_RESET) || (ACTION) ==Bit_SET))

void HAL_GPIO_DeInit(GPIO_TypeDef* GPIOx);
void HAL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void HAL_GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
uint8_t HAL_GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t HAL_GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t HAL_GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t HAL_GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void HAL_GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void HAL_GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void HAL_PAD_AFConfig(PAD_Type Px, uint16_t Pnum, PAD_AF_TypeDef P_AF);

void GPIO_OutEnClr(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_OutEnSet(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


#ifdef __cplusplus
}
#endif

#endif // __W7500X_HAL_GPIO_H
