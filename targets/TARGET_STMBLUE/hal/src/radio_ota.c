/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : radio_ota.c 
* Author             : RF Application Team
* Version            : V1.0.0
* Date               : May-2018
* Description        : OTA library for user application
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "radio_ota.h"
//#include "BlueNRG1_conf.h"

/** @addtogroup BlueNRG1_StdPeriph_Examples
* @{
*/

/** @addtogroup RADIO_Examples RADIO Examples
* @{
*/

/** @addtogroup RADIO_TxRx RADIO TxRx Example
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#define OTA_NO_OPERATION                                 (0x11)
#define OTA_APP_SWITCH_OP_CODE_NO_OPERATION              (0xB0014211)
#define OTA_SWITCH_TO_OTA_RESET_MANAGER (OTA_APP_SWITCH_OP_CODE_NO_OPERATION + (OTA_NO_OPERATION*4)) 

void OTA_Jump_To_Reset_Manager()
{
  extern volatile uint32_t ota_sw_activation;
  ota_sw_activation = OTA_SWITCH_TO_OTA_RESET_MANAGER; 
  
  NVIC_SystemReset();
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

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
