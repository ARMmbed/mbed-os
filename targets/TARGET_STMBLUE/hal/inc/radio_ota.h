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
/**
  ******************************************************************************
  * @file    radio_ota.h
  * @author  AMS - RF Application team
  * @version V1.0.0
  * @date    29-May-2018
  * @brief   Header file for 2,4 GHz proprietary radio Over-the-Air FW upgrade APIs
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
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
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

/** 
 * @brief This function allows to give control to the OTA Reset Manager who handles
 *        the 2,4 GHz proprietary radio Over-the-Air FW upgrade procedure and the jump
 *        to the valid application to be run.
 * 
 * @param None
 *
 * @retval None
 */
void OTA_Jump_To_Reset_Manager();


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
