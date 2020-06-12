/**
 ******************************************************************************
 * @file    shci.c
 * @author  MCD Application Team
 * @brief   HCI command for the system channel
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
#include "stm32_wpan_common.h"

#include "shci_tl.h"
#include "shci.h"
#include "stm32wbxx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Local Functions Definition ------------------------------------------------------*/
/* Public Functions Definition ------------------------------------------------------*/

/**
 *  C2 COMMAND
 *  These commands are sent to the CPU2
 */
uint8_t SHCI_C2_FUS_GetState( SHCI_FUS_GetState_ErrorCode_t *p_error_code )
{
  /**
   * A command status event + payload has the same size than the expected command complete
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE + 1];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_FUS_GET_STATE,
             0,
             0,
             p_rsp );

  if(p_error_code != 0)
  {
    *p_error_code = (SHCI_FUS_GetState_ErrorCode_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[1]);
  }

  return (((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FUS_FwUpgrade( uint32_t fw_src_add,  uint32_t fw_dest_add )
{
  /**
   * TL_BLEEVT_CS_BUFFER_SIZE is 15 bytes so it is large enough to hold the 8 bytes of command parameters
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;
  uint32_t *p_cmd;
  uint8_t cmd_length;

  p_cmd = (uint32_t*)local_buffer;
  cmd_length = 0;

  if(fw_src_add != 0)
  {
    *p_cmd = fw_src_add;
    cmd_length += 4;
  }

  if(fw_dest_add != 0)
  {
    *(p_cmd+1) = fw_dest_add;
    cmd_length += 4;
  }

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_FUS_FW_UPGRADE,
             cmd_length,
             local_buffer,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FUS_FwDelete( void )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_FUS_FW_DELETE,
             0,
             0,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FUS_UpdateAuthKey( SHCI_C2_FUS_UpdateAuthKey_Cmd_Param_t *pParam )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_FUS_UPDATE_AUTH_KEY,
             sizeof( SHCI_C2_FUS_UpdateAuthKey_Cmd_Param_t ),
             (uint8_t*)pParam,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FUS_LockAuthKey( void )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_FUS_LOCK_AUTH_KEY,
             0,
             0,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FUS_StoreUsrKey( SHCI_C2_FUS_StoreUsrKey_Cmd_Param_t *pParam, uint8_t *p_key_index )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE + 1];
  TL_EvtPacket_t * p_rsp;
  uint8_t local_payload_len;

  if(pParam->KeyType == KEYTYPE_ENCRYPTED)
  {
    /**
     * When the key is encrypted, the 12 bytes IV Key is included in the payload as well
     * The IV key is always 12 bytes
     */
    local_payload_len = pParam->KeySize + 2 + 12;
  }
  else
  {
    local_payload_len = pParam->KeySize + 2;
  }

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_FUS_STORE_USR_KEY,
             local_payload_len ,
             (uint8_t*)pParam,
             p_rsp );

  *p_key_index = (((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[1]);

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FUS_LoadUsrKey( uint8_t key_index )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  local_buffer[0] = key_index;

  shci_send( SHCI_OPCODE_C2_FUS_LOAD_USR_KEY,
             1,
             local_buffer,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FUS_StartWs( void )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_FUS_START_WS,
             0,
             0,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}


SHCI_CmdStatus_t SHCI_C2_FUS_LockUsrKey( uint8_t key_index )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  local_buffer[0] = key_index;

  shci_send( SHCI_OPCODE_C2_FUS_LOCK_USR_KEY,
             1,
             local_buffer,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_BLE_Init( SHCI_C2_Ble_Init_Cmd_Packet_t *pCmdPacket )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

 shci_send( SHCI_OPCODE_C2_BLE_INIT,
            sizeof( SHCI_C2_Ble_Init_Cmd_Param_t ),
            (uint8_t*)&pCmdPacket->Param,
            p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_THREAD_Init( void )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_THREAD_INIT,
             0,
             0,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_LLDTESTS_Init( uint8_t param_size, uint8_t * p_param )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_LLD_TESTS_INIT,
             param_size,
             p_param,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_ZIGBEE_Init( void )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_ZIGBEE_INIT,
             0,
             0,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}
SHCI_CmdStatus_t SHCI_C2_DEBUG_Init( SHCI_C2_DEBUG_Init_Cmd_Packet_t *pCmdPacket  )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_DEBUG_INIT,
             sizeof( SHCI_C2_DEBUG_init_Cmd_Param_t ),
             (uint8_t*)&pCmdPacket->Param,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FLASH_EraseActivity( SHCI_EraseActivity_t erase_activity )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  local_buffer[0] = erase_activity;

  shci_send( SHCI_OPCODE_C2_FLASH_ERASE_ACTIVITY,
             1,
             local_buffer,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_CONCURRENT_SetMode( SHCI_C2_CONCURRENT_Mode_Param_t Mode )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  local_buffer[0] = Mode;

  shci_send( SHCI_OPCODE_C2_CONCURRENT_SET_MODE,
             1,
             local_buffer,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FLASH_StoreData( SHCI_C2_FLASH_Ip_t Ip )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  local_buffer[0] = Ip;

  shci_send( SHCI_OPCODE_C2_FLASH_STORE_DATA,
             1,
             local_buffer,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_FLASH_EraseData( SHCI_C2_FLASH_Ip_t Ip )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  local_buffer[0] = Ip;

  shci_send( SHCI_OPCODE_C2_FLASH_ERASE_DATA,
             1,
             local_buffer,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_RADIO_AllowLowPower( SHCI_C2_FLASH_Ip_t Ip,uint8_t  FlagRadioLowPowerOn)
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  local_buffer[0] = Ip;
  local_buffer[1] = FlagRadioLowPowerOn;

  shci_send( SHCI_OPCODE_C2_RADIO_ALLOW_LOW_POWER,
             2,
             local_buffer,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_MAC_802_15_4_Init( void )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_MAC_802_15_4_INIT,
             0,
             0,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_Reinit( void )
{
  /**
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  shci_send( SHCI_OPCODE_C2_REINIT,
             0,
             0,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

SHCI_CmdStatus_t SHCI_C2_ExtpaConfig(uint32_t gpio_port, uint16_t gpio_pin_number, uint8_t gpio_polarity, uint8_t gpio_status)
{
  /**
   * TL_BLEEVT_CS_BUFFER_SIZE is 15 bytes so it is large enough to hold the 8 bytes of command parameters
   * Buffer is large enough to hold command complete without payload
   */
  uint8_t local_buffer[TL_BLEEVT_CS_BUFFER_SIZE];
  TL_EvtPacket_t * p_rsp;

  p_rsp = (TL_EvtPacket_t *)local_buffer;

  ((SHCI_C2_EXTPA_CONFIG_Cmd_Param_t*)local_buffer)->gpio_port = gpio_port;
  ((SHCI_C2_EXTPA_CONFIG_Cmd_Param_t*)local_buffer)->gpio_pin_number = gpio_pin_number;
  ((SHCI_C2_EXTPA_CONFIG_Cmd_Param_t*)local_buffer)->gpio_polarity = gpio_polarity;
  ((SHCI_C2_EXTPA_CONFIG_Cmd_Param_t*)local_buffer)->gpio_status = gpio_status;

  shci_send( SHCI_OPCODE_C2_EXTPA_CONFIG,
             8,
             local_buffer,
             p_rsp );

  return (SHCI_CmdStatus_t)(((TL_CcEvt_t*)(p_rsp->evtserial.evt.payload))->payload[0]);
}

/**
 *  Local System COMMAND
 *  These commands are NOT sent to the CPU2
 */

SHCI_CmdStatus_t SHCI_GetWirelessFwInfo( WirelessFwInfo_t* pWirelessInfo )
{
  uint32_t ipccdba = 0;
  MB_RefTable_t * p_RefTable = NULL;
  uint32_t version = 0;
  uint32_t memorySize = 0;
  uint32_t infoStack = 0;

  ipccdba = READ_BIT( FLASH->IPCCBR, FLASH_IPCCBR_IPCCDBA );
  p_RefTable = (MB_RefTable_t*)((ipccdba<<2) + SRAM2A_BASE);

  /**
   *  Retrieve the WirelessFwInfoTable
   *  This table is stored in RAM at startup during the TL (transport layer) initialization
   */
  version =  p_RefTable->p_device_info_table->WirelessFwInfoTable.Version;
  pWirelessInfo->VersionMajor       = ((version & INFO_VERSION_MAJOR_MASK) >> INFO_VERSION_MAJOR_OFFSET);
  pWirelessInfo->VersionMinor       = ((version & INFO_VERSION_MINOR_MASK) >> INFO_VERSION_MINOR_OFFSET);
  pWirelessInfo->VersionSub         = ((version & INFO_VERSION_SUB_MASK) >> INFO_VERSION_SUB_OFFSET);
  pWirelessInfo->VersionBranch      = ((version & INFO_VERSION_BRANCH_MASK) >> INFO_VERSION_BRANCH_OFFSET);
  pWirelessInfo->VersionReleaseType = ((version & INFO_VERSION_TYPE_MASK) >> INFO_VERSION_TYPE_OFFSET);

  memorySize =  p_RefTable->p_device_info_table->WirelessFwInfoTable.MemorySize;
  pWirelessInfo->MemorySizeSram2B   = ((memorySize & INFO_SIZE_SRAM2B_MASK) >> INFO_SIZE_SRAM2B_OFFSET);
  pWirelessInfo->MemorySizeSram2A   = ((memorySize & INFO_SIZE_SRAM2A_MASK) >> INFO_SIZE_SRAM2A_OFFSET);
  pWirelessInfo->MemorySizeSram1    = ((memorySize & INFO_SIZE_SRAM1_MASK) >> INFO_SIZE_SRAM1_OFFSET);
  pWirelessInfo->MemorySizeFlash    = ((memorySize & INFO_SIZE_FLASH_MASK) >> INFO_SIZE_FLASH_OFFSET);

  infoStack =  p_RefTable->p_device_info_table->WirelessFwInfoTable.InfoStack;
  pWirelessInfo->StackType          = ((infoStack & INFO_STACK_TYPE_MASK) >> INFO_STACK_TYPE_OFFSET);

  /**
   *  Retrieve the FusInfoTable
   *  This table is stored in RAM at startup during the TL (transport layer) initialization
   */
  version =  p_RefTable->p_device_info_table->FusInfoTable.Version;
  pWirelessInfo->FusVersionMajor       = ((version & INFO_VERSION_MAJOR_MASK) >> INFO_VERSION_MAJOR_OFFSET);
  pWirelessInfo->FusVersionMinor       = ((version & INFO_VERSION_MINOR_MASK) >> INFO_VERSION_MINOR_OFFSET);
  pWirelessInfo->FusVersionSub         = ((version & INFO_VERSION_SUB_MASK) >> INFO_VERSION_SUB_OFFSET);

  memorySize =  p_RefTable->p_device_info_table->FusInfoTable.MemorySize;
  pWirelessInfo->FusMemorySizeSram2B   = ((memorySize & INFO_SIZE_SRAM2B_MASK) >> INFO_SIZE_SRAM2B_OFFSET);
  pWirelessInfo->FusMemorySizeSram2A   = ((memorySize & INFO_SIZE_SRAM2A_MASK) >> INFO_SIZE_SRAM2A_OFFSET);
  pWirelessInfo->FusMemorySizeFlash    = ((memorySize & INFO_SIZE_FLASH_MASK) >> INFO_SIZE_FLASH_OFFSET);

  return (SHCI_Success);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
