/**
  ******************************************************************************
  * @file    otp.c
  * @author  MCD Application Team
  * @brief   OTP manager
  ******************************************************************************
   * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "utilities_common.h"

#include "otp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

uint8_t * OTP_Read( uint8_t id )
{
  uint8_t *p_id;

  p_id = (uint8_t*)(CFG_OTP_END_ADRESS - 7) ;

  while( ((*( p_id + 7 )) != id) && ( p_id != (uint8_t*)CFG_OTP_BASE_ADDRESS) )
  {
    p_id -= 8 ;
  }

  if((*( p_id + 7 )) != id)
  {
    p_id = 0 ;
  }

  return p_id ;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
