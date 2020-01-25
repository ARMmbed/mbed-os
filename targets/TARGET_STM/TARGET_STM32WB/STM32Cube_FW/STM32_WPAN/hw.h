/**
  ******************************************************************************
  * @file    hw.h
  * @author  MCD Application Team
  * @brief   Hardware
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_H
#define __HW_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/

  /******************************************************************************
   * HW IPCC
   ******************************************************************************/
  void HW_IPCC_Enable( void );
  void HW_IPCC_Init( void );
  void HW_IPCC_Rx_Handler( void );
  void HW_IPCC_Tx_Handler( void );

  void HW_IPCC_BLE_Init( void );
  void HW_IPCC_BLE_SendCmd( void );
  void HW_IPCC_MM_SendFreeBuf( void (*cb)( void ) );
  void HW_IPCC_BLE_RxEvtNot( void );
  void HW_IPCC_BLE_SendAclData( void );
  void HW_IPCC_BLE_AclDataAckNot( void );

  void HW_IPCC_SYS_Init( void );
  void HW_IPCC_SYS_SendCmd( void );
  void HW_IPCC_SYS_CmdEvtNot( void );
  void HW_IPCC_SYS_EvtNot( void );

  void HW_IPCC_THREAD_Init( void );
  void HW_IPCC_OT_SendCmd( void );
  void HW_IPCC_CLI_SendCmd( void );
  void HW_IPCC_THREAD_SendAck( void );
  void HW_IPCC_OT_CmdEvtNot( void );
  void HW_IPCC_CLI_CmdEvtNot( void );
  void HW_IPCC_THREAD_EvtNot( void );
  void HW_IPCC_THREAD_CliSendAck( void );
  void HW_IPCC_THREAD_CliEvtNot( void );

  void HW_IPCC_LLDTESTS_Init( void );
  void HW_IPCC_LLDTESTS_SendCliCmd( void );
  void HW_IPCC_LLDTESTS_ReceiveCliRsp( void );
  void HW_IPCC_LLDTESTS_SendCliRspAck( void );
  void HW_IPCC_LLDTESTS_ReceiveM0Cmd( void );
  void HW_IPCC_LLDTESTS_SendM0CmdAck( void );

  void HW_IPCC_TRACES_Init( void );
  void HW_IPCC_TRACES_EvtNot( void );

  void HW_IPCC_MAC_802_15_4_Init( void );
  void HW_IPCC_MAC_802_15_4_SendCmd( void );
  void HW_IPCC_MAC_802_15_4_SendAck( void );
  void HW_IPCC_MAC_802_15_4_CmdEvtNot( void );
  void HW_IPCC_MAC_802_15_4_EvtNot( void );

  void HW_IPCC_ZIGBEE_Init( void );

  void HW_IPCC_ZIGBEE_SendAppliCmd(void);
  void HW_IPCC_ZIGBEE_AppliCmdNotification(void);

  void HW_IPCC_ZIGBEE_AppliAsyncEvtNotification(void);
  void HW_IPCC_ZIGBEE_SendAppliCmdAck(void);
  void HW_IPCC_ZIGBEE_AppliAsyncLoggingNotification( void );
  void HW_IPCC_ZIGBEE_SendLoggingAck(void);


#ifdef __cplusplus
}
#endif

#endif /*__HW_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
