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
#ifndef __W7500X_EXTI_H
#define __W7500X_EXTI_H

#ifdef __cplusplus
 extern "C" {
#endif

 /* Includes ------------------------------------------------------------------*/
#include "W7500x.h"


 /**
  * @brief  EXTI mode enumeration  
  */

typedef enum
{
  EXTI_Mode_Disable = 0x00,
  EXTI_Mode_Interrupt = 0x02
}EXTIMode_TypeDef;

#define IS_EXTI_MODE(MODE) (((MODE) == EXTI_Mode_Rising) || \
                            ((MODE) == EXTI_Mode_Interrupt)) 

/** 
  * @brief  EXTI Trigger enumeration  
  */

typedef enum
{
  EXTI_Trigger_Rising = 0x00,
  EXTI_Trigger_Falling = 0x01  
}EXTITrigger_TypeDef;

#define IS_EXTI_TRIGGER(TRIGGER) (((TRIGGER) == EXTI_Trigger_Rising) || \
                                  ((TRIGGER) == EXTI_Trigger_Falling))

/** 
  * @brief  EXTI Init Structure definition  
  */

typedef struct
{
  uint32_t EXTI_Line;               /*!< Specifies the EXTI lines to be enabled or disabled.
                                         This parameter can be any combination of @ref EXTI_Lines */
   
  EXTIMode_TypeDef EXTI_Mode;       /*!< Specifies the mode for the EXTI lines.This parameter can be a value of @ref EXTIMode_TypeDef */

  EXTITrigger_TypeDef EXTI_Trigger; /*!< Specifies the trigger signal active edge for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */

}EXTI_InitTypeDef;

/**
  * @}
  */

void EXTI_DeInit(void);
void EXTI_Init(PAD_Type Px, EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_Polarity_Set(PAD_Type Px, uint16_t GPIO_Pin, uint16_t Polarity );
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);
uint32_t EXTI_GetEXTIConfig(PAD_Type Px, uint16_t GPIO_Pin);
uint16_t EXTI_Px_GetEXTEN(PAD_Type Px);
uint16_t EXTI_Px_GetEXTINTPOL(PAD_Type Px);


#ifdef __cplusplus
}
#endif


#endif //__W7500X_EXTI_H
