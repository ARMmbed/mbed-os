/**
 ******************************************************************************
 * @file    tl_mbox.c
 * @author  MCD Application Team
 * @brief   Transport layer for the mailbox interface
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
#include "hw.h"

#include "stm_list.h"
#include "tl.h"
#include "mbox_def.h"
#include "tl_dbg_conf.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  TL_MB_MM_RELEASE_BUFFER,
  TL_MB_BLE_CMD,
  TL_MB_BLE_CMD_RSP,
  TL_MB_BLE_ASYNCH_EVT,
  TL_MB_SYS_CMD,
  TL_MB_SYS_CMD_RSP,
  TL_MB_SYS_ASYNCH_EVT,
} TL_MB_PacketType_t;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**< reference table */
PLACE_IN_SECTION("MAPPING_TABLE") static volatile MB_RefTable_t TL_RefTable;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_DeviceInfoTable_t TL_DeviceInfoTable;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_BleTable_t TL_BleTable;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_ThreadTable_t TL_ThreadTable;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_LldTestsTable_t TL_LldTestsTable;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_BleLldTable_t TL_BleLldTable;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_SysTable_t TL_SysTable;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_MemManagerTable_t TL_MemManagerTable;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_TracesTable_t TL_TracesTable;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_Mac_802_15_4_t TL_Mac_802_15_4_Table;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static MB_ZigbeeTable_t TL_Zigbee_Table;

/**< tables */
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static tListNode  FreeBufQueue;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static tListNode  TracesEvtQueue;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t    CsBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + sizeof(TL_CsEvt_t)];
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static tListNode  EvtQueue;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static tListNode  SystemEvtQueue;


static tListNode  LocalFreeBufQueue;
static void (* BLE_IoBusEvtCallBackFunction) (TL_EvtPacket_t *phcievt);
static void (* BLE_IoBusAclDataTxAck) ( void );
static void (* SYS_CMD_IoBusCallBackFunction) (TL_EvtPacket_t *phcievt);
static void (* SYS_EVT_IoBusCallBackFunction) (TL_EvtPacket_t *phcievt);


/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SendFreeBuf( void );
static void OutputDbgTrace(TL_MB_PacketType_t packet_type, uint8_t* buffer);

/* Public Functions Definition ------------------------------------------------------*/

/******************************************************************************
 * GENERAL
 ******************************************************************************/
void TL_Enable( void )
{
  HW_IPCC_Enable();

  return;
}


void TL_Init( void )
{
  TL_RefTable.p_device_info_table = &TL_DeviceInfoTable;
  TL_RefTable.p_ble_table = &TL_BleTable;
  TL_RefTable.p_thread_table = &TL_ThreadTable;
  TL_RefTable.p_lld_tests_table = &TL_LldTestsTable;
  TL_RefTable.p_ble_lld_table = &TL_BleLldTable;
  TL_RefTable.p_sys_table = &TL_SysTable;
  TL_RefTable.p_mem_manager_table = &TL_MemManagerTable;
  TL_RefTable.p_traces_table = &TL_TracesTable;
  TL_RefTable.p_mac_802_15_4_table = &TL_Mac_802_15_4_Table;
  TL_RefTable.p_zigbee_table = &TL_Zigbee_Table;
  HW_IPCC_Init();

  return;
}

/******************************************************************************
 * BLE
 ******************************************************************************/
int32_t TL_BLE_Init( void* pConf )
{
  MB_BleTable_t  * p_bletable;

  TL_BLE_InitConf_t *pInitHciConf = (TL_BLE_InitConf_t *) pConf;

  LST_init_head (&EvtQueue);

  p_bletable = TL_RefTable.p_ble_table;

  p_bletable->pcmd_buffer = pInitHciConf->p_cmdbuffer;
  p_bletable->phci_acl_data_buffer = pInitHciConf->p_AclDataBuffer;
  p_bletable->pcs_buffer  = (uint8_t*)CsBuffer;
  p_bletable->pevt_queue  = (uint8_t*)&EvtQueue;

  HW_IPCC_BLE_Init();

  BLE_IoBusEvtCallBackFunction = pInitHciConf->IoBusEvtCallBack;
  BLE_IoBusAclDataTxAck = pInitHciConf->IoBusAclDataTxAck;

  return 0;
}

int32_t TL_BLE_SendCmd( uint8_t* buffer, uint16_t size )
{
  (void)(buffer);
  (void)(size);

  ((TL_CmdPacket_t*)(TL_RefTable.p_ble_table->pcmd_buffer))->cmdserial.type = TL_BLECMD_PKT_TYPE;

  OutputDbgTrace(TL_MB_BLE_CMD, TL_RefTable.p_ble_table->pcmd_buffer);

  HW_IPCC_BLE_SendCmd();

  return 0;
}

void HW_IPCC_BLE_RxEvtNot(void)
{
  TL_EvtPacket_t *phcievt;

  while(LST_is_empty(&EvtQueue) == FALSE)
  {
    LST_remove_head (&EvtQueue, (tListNode **)&phcievt);

    if ( ((phcievt->evtserial.evt.evtcode) == TL_BLEEVT_CS_OPCODE) || ((phcievt->evtserial.evt.evtcode) == TL_BLEEVT_CC_OPCODE ) )
    {
      OutputDbgTrace(TL_MB_BLE_CMD_RSP, (uint8_t*)phcievt);
    }
    else
    {
      OutputDbgTrace(TL_MB_BLE_ASYNCH_EVT, (uint8_t*)phcievt);
    }

    BLE_IoBusEvtCallBackFunction(phcievt);
  }

  return;
}

int32_t TL_BLE_SendAclData( uint8_t* buffer, uint16_t size )
{
  (void)(buffer);
  (void)(size);

  ((TL_AclDataPacket_t *)(TL_RefTable.p_ble_table->phci_acl_data_buffer))->AclDataSerial.type = TL_ACL_DATA_PKT_TYPE;

  HW_IPCC_BLE_SendAclData();

  return 0;
}

void HW_IPCC_BLE_AclDataAckNot(void)
{
  BLE_IoBusAclDataTxAck( );

  return;
}

/******************************************************************************
 * SYSTEM
 ******************************************************************************/
int32_t TL_SYS_Init( void* pConf  )
{
  MB_SysTable_t  * p_systable;

  TL_SYS_InitConf_t *pInitHciConf = (TL_SYS_InitConf_t *) pConf;

  LST_init_head (&SystemEvtQueue);
  p_systable = TL_RefTable.p_sys_table;
  p_systable->pcmd_buffer = pInitHciConf->p_cmdbuffer;
  p_systable->sys_queue = (uint8_t*)&SystemEvtQueue;

  HW_IPCC_SYS_Init();

  SYS_CMD_IoBusCallBackFunction = pInitHciConf->IoBusCallBackCmdEvt;
  SYS_EVT_IoBusCallBackFunction = pInitHciConf->IoBusCallBackUserEvt;

  return 0;
}

int32_t TL_SYS_SendCmd( uint8_t* buffer, uint16_t size )
{
  (void)(buffer);
  (void)(size);

  ((TL_CmdPacket_t *)(TL_RefTable.p_sys_table->pcmd_buffer))->cmdserial.type = TL_SYSCMD_PKT_TYPE;

  OutputDbgTrace(TL_MB_SYS_CMD, TL_RefTable.p_sys_table->pcmd_buffer);

  HW_IPCC_SYS_SendCmd();

  return 0;
}

void HW_IPCC_SYS_CmdEvtNot(void)
{
  OutputDbgTrace(TL_MB_SYS_CMD_RSP, (uint8_t*)(TL_RefTable.p_sys_table->pcmd_buffer) );

  SYS_CMD_IoBusCallBackFunction( (TL_EvtPacket_t*)(TL_RefTable.p_sys_table->pcmd_buffer) );

  return;
}

void HW_IPCC_SYS_EvtNot( void )
{
  TL_EvtPacket_t *p_evt;

  while(LST_is_empty(&SystemEvtQueue) == FALSE)
  {
    LST_remove_head (&SystemEvtQueue, (tListNode **)&p_evt);

    OutputDbgTrace(TL_MB_SYS_ASYNCH_EVT, (uint8_t*)p_evt );

    SYS_EVT_IoBusCallBackFunction( p_evt );
  }

  return;
}

/******************************************************************************
 * THREAD
 ******************************************************************************/
#ifdef THREAD_WB
void TL_THREAD_Init( TL_TH_Config_t *p_Config )
{
  MB_ThreadTable_t  * p_thread_table;

  p_thread_table = TL_RefTable.p_thread_table;

  p_thread_table->clicmdrsp_buffer = p_Config->p_ThreadCliRspBuffer;
  p_thread_table->otcmdrsp_buffer = p_Config->p_ThreadOtCmdRspBuffer;
  p_thread_table->notack_buffer = p_Config->p_ThreadNotAckBuffer;

  HW_IPCC_THREAD_Init();

  return;
}

void TL_OT_SendCmd( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_thread_table->otcmdrsp_buffer))->cmdserial.type = TL_OTCMD_PKT_TYPE;

  HW_IPCC_OT_SendCmd();

  return;
}

void TL_CLI_SendCmd( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_thread_table->clicmdrsp_buffer))->cmdserial.type = TL_CLICMD_PKT_TYPE;

  HW_IPCC_CLI_SendCmd();

  return;
}

void TL_THREAD_SendAck ( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_thread_table->notack_buffer))->cmdserial.type = TL_OTACK_PKT_TYPE;

  HW_IPCC_THREAD_SendAck();

  return;
}

void TL_THREAD_CliSendAck ( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_thread_table->notack_buffer))->cmdserial.type = TL_OTACK_PKT_TYPE;

  HW_IPCC_THREAD_CliSendAck();

  return;
}

void HW_IPCC_OT_CmdEvtNot(void)
{
  TL_OT_CmdEvtReceived( (TL_EvtPacket_t*)(TL_RefTable.p_thread_table->otcmdrsp_buffer) );

  return;
}

void HW_IPCC_THREAD_EvtNot( void )
{
  TL_THREAD_NotReceived( (TL_EvtPacket_t*)(TL_RefTable.p_thread_table->notack_buffer) );

  return;
}

void HW_IPCC_THREAD_CliEvtNot( void )
{
  TL_THREAD_CliNotReceived( (TL_EvtPacket_t*)(TL_RefTable.p_thread_table->clicmdrsp_buffer) );

  return;
}

__WEAK void TL_OT_CmdEvtReceived( TL_EvtPacket_t * Otbuffer  ){};
__WEAK void TL_THREAD_NotReceived( TL_EvtPacket_t * Notbuffer ){};
__WEAK void TL_THREAD_CliNotReceived( TL_EvtPacket_t * Notbuffer ){};

#endif /* THREAD_WB */

/******************************************************************************
 * LLD TESTS
 ******************************************************************************/
#ifdef LLD_TESTS_WB
void TL_LLDTESTS_Init( TL_LLD_tests_Config_t *p_Config )
{
  MB_LldTestsTable_t  * p_lld_tests_table;

  p_lld_tests_table = TL_RefTable.p_lld_tests_table;
  p_lld_tests_table->clicmdrsp_buffer = p_Config->p_LldTestsCliCmdRspBuffer;
  p_lld_tests_table->m0cmd_buffer = p_Config->p_LldTestsM0CmdBuffer;
  HW_IPCC_LLDTESTS_Init();
  return;
}

void TL_LLDTESTS_SendCliCmd( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_lld_tests_table->clicmdrsp_buffer))->cmdserial.type = TL_CLICMD_PKT_TYPE;
  HW_IPCC_LLDTESTS_SendCliCmd();
  return;
}

void HW_IPCC_LLDTESTS_ReceiveCliRsp( void )
{
  TL_LLDTESTS_ReceiveCliRsp( (TL_CmdPacket_t*)(TL_RefTable.p_lld_tests_table->clicmdrsp_buffer) );
  return;
}

void TL_LLDTESTS_SendCliRspAck( void )
{
  HW_IPCC_LLDTESTS_SendCliRspAck();
  return;
}

void HW_IPCC_LLDTESTS_ReceiveM0Cmd( void )
{
  TL_LLDTESTS_ReceiveM0Cmd( (TL_CmdPacket_t*)(TL_RefTable.p_lld_tests_table->m0cmd_buffer) );
  return;
}


void TL_LLDTESTS_SendM0CmdAck( void )
{
  HW_IPCC_LLDTESTS_SendM0CmdAck();
  return;
}

__WEAK void TL_LLDTESTS_ReceiveCliRsp( TL_CmdPacket_t * Notbuffer ){};
__WEAK void TL_LLDTESTS_ReceiveM0Cmd( TL_CmdPacket_t * Notbuffer ){};
#endif /* LLD_TESTS_WB */

/******************************************************************************
 * BLE LLD
 ******************************************************************************/
#ifdef BLE_LLD_WB
void TL_BLE_LLD_Init( TL_BLE_LLD_Config_t *p_Config )
{
  MB_BleLldTable_t  * p_ble_lld_table;

  p_ble_lld_table = TL_RefTable.p_ble_lld_table;
  p_ble_lld_table->cmdrsp_buffer = p_Config->p_BleLldCmdRspBuffer;
  p_ble_lld_table->m0cmd_buffer = p_Config->p_BleLldM0CmdBuffer;
  HW_IPCC_BLE_LLD_Init();
  return;
}

void TL_BLE_LLD_SendCliCmd( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_ble_lld_table->cmdrsp_buffer))->cmdserial.type = TL_CLICMD_PKT_TYPE;
  HW_IPCC_BLE_LLD_SendCliCmd();
  return;
}

void HW_IPCC_BLE_LLD_ReceiveCliRsp( void )
{
  TL_BLE_LLD_ReceiveCliRsp( (TL_CmdPacket_t*)(TL_RefTable.p_ble_lld_table->cmdrsp_buffer) );
  return;
}

void TL_BLE_LLD_SendCliRspAck( void )
{
  HW_IPCC_BLE_LLD_SendCliRspAck();
  return;
}

void HW_IPCC_BLE_LLD_ReceiveM0Cmd( void )
{
  TL_BLE_LLD_ReceiveM0Cmd( (TL_CmdPacket_t*)(TL_RefTable.p_ble_lld_table->m0cmd_buffer) );
  return;
}


void TL_BLE_LLD_SendM0CmdAck( void )
{
  HW_IPCC_BLE_LLD_SendM0CmdAck();
  return;
}

__WEAK void TL_BLE_LLD_ReceiveCliRsp( TL_CmdPacket_t * Notbuffer ){};
__WEAK void TL_BLE_LLD_ReceiveM0Cmd( TL_CmdPacket_t * Notbuffer ){};

/* Transparent Mode */
void TL_BLE_LLD_SendCmd( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_ble_lld_table->cmdrsp_buffer))->cmdserial.type = TL_CLICMD_PKT_TYPE;
  HW_IPCC_BLE_LLD_SendCmd();
  return;
}

void HW_IPCC_BLE_LLD_ReceiveRsp( void )
{
  TL_BLE_LLD_ReceiveRsp( (TL_CmdPacket_t*)(TL_RefTable.p_ble_lld_table->cmdrsp_buffer) );
  return;
}

void TL_BLE_LLD_SendRspAck( void )
{
  HW_IPCC_BLE_LLD_SendRspAck();
  return;
}
#endif /* BLE_LLD_WB */

#ifdef MAC_802_15_4_WB
/******************************************************************************
 * MAC 802.15.4
 ******************************************************************************/
void TL_MAC_802_15_4_Init( TL_MAC_802_15_4_Config_t *p_Config )
{
  MB_Mac_802_15_4_t  * p_mac_802_15_4_table;

  p_mac_802_15_4_table = TL_RefTable.p_mac_802_15_4_table;

  p_mac_802_15_4_table->p_cmdrsp_buffer = p_Config->p_Mac_802_15_4_CmdRspBuffer;
  p_mac_802_15_4_table->p_notack_buffer = p_Config->p_Mac_802_15_4_NotAckBuffer;

  HW_IPCC_MAC_802_15_4_Init();

  return;
}

void TL_MAC_802_15_4_SendCmd( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_mac_802_15_4_table->p_cmdrsp_buffer))->cmdserial.type = TL_OTCMD_PKT_TYPE;

  HW_IPCC_MAC_802_15_4_SendCmd();

  return;
}

void TL_MAC_802_15_4_SendAck ( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_mac_802_15_4_table->p_notack_buffer))->cmdserial.type = TL_OTACK_PKT_TYPE;

  HW_IPCC_MAC_802_15_4_SendAck();

  return;
}

void HW_IPCC_MAC_802_15_4_CmdEvtNot(void)
{
  TL_MAC_802_15_4_CmdEvtReceived( (TL_EvtPacket_t*)(TL_RefTable.p_mac_802_15_4_table->p_cmdrsp_buffer) );

  return;
}

void HW_IPCC_MAC_802_15_4_EvtNot( void )
{
  TL_MAC_802_15_4_NotReceived( (TL_EvtPacket_t*)(TL_RefTable.p_mac_802_15_4_table->p_notack_buffer) );

  return;
}

__WEAK void TL_MAC_802_15_4_CmdEvtReceived( TL_EvtPacket_t * Otbuffer  ){};
__WEAK void TL_MAC_802_15_4_NotReceived( TL_EvtPacket_t * Notbuffer ){};
#endif

#ifdef ZIGBEE_WB
/******************************************************************************
 * ZIGBEE
 ******************************************************************************/
void TL_ZIGBEE_Init( TL_ZIGBEE_Config_t *p_Config )
{
  MB_ZigbeeTable_t  * p_zigbee_table;

  p_zigbee_table = TL_RefTable.p_zigbee_table;
  p_zigbee_table->appliCmdM4toM0_buffer = p_Config->p_ZigbeeOtCmdRspBuffer;
  p_zigbee_table->notifM0toM4_buffer = p_Config->p_ZigbeeNotAckBuffer;
  p_zigbee_table->requestM0toM4_buffer = p_Config->p_ZigbeeNotifRequestBuffer;

  HW_IPCC_ZIGBEE_Init();

  return;
}

/* Zigbee M4 to M0 Request */
void TL_ZIGBEE_SendM4RequestToM0( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_zigbee_table->appliCmdM4toM0_buffer))->cmdserial.type = TL_OTCMD_PKT_TYPE;

  HW_IPCC_ZIGBEE_SendM4RequestToM0();

  return;
}

/* Used to receive an ACK from the M0 */
void HW_IPCC_ZIGBEE_RecvAppliAckFromM0(void)
{
  TL_ZIGBEE_CmdEvtReceived( (TL_EvtPacket_t*)(TL_RefTable.p_zigbee_table->appliCmdM4toM0_buffer) );

  return;
}

/* Zigbee notification from M0 to M4 */
void HW_IPCC_ZIGBEE_RecvM0NotifyToM4( void )
{
  TL_ZIGBEE_NotReceived( (TL_EvtPacket_t*)(TL_RefTable.p_zigbee_table->notifM0toM4_buffer) );

  return;
}

/* Send an ACK to the M0 for a Notification */
void TL_ZIGBEE_SendM4AckToM0Notify ( void )
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_zigbee_table->notifM0toM4_buffer))->cmdserial.type = TL_OTACK_PKT_TYPE;

  HW_IPCC_ZIGBEE_SendM4AckToM0Notify();

  return;
}

/* Zigbee M0 to M4 Request */
void HW_IPCC_ZIGBEE_RecvM0RequestToM4( void )
{
  TL_ZIGBEE_M0RequestReceived( (TL_EvtPacket_t*)(TL_RefTable.p_zigbee_table->requestM0toM4_buffer) );

  return;
}

/* Send an ACK to the M0 for a Request */
void TL_ZIGBEE_SendM4AckToM0Request(void)
{
  ((TL_CmdPacket_t *)(TL_RefTable.p_zigbee_table->requestM0toM4_buffer))->cmdserial.type = TL_OTACK_PKT_TYPE;

  HW_IPCC_ZIGBEE_SendM4AckToM0Request();

  return;
}


__WEAK void TL_ZIGBEE_CmdEvtReceived( TL_EvtPacket_t * Otbuffer  ){};
__WEAK void TL_ZIGBEE_NotReceived( TL_EvtPacket_t * Notbuffer ){};
#endif



/******************************************************************************
 * MEMORY MANAGER
 ******************************************************************************/
void TL_MM_Init( TL_MM_Config_t *p_Config )
{
  static MB_MemManagerTable_t  * p_mem_manager_table;

  LST_init_head (&FreeBufQueue);
  LST_init_head (&LocalFreeBufQueue);

  p_mem_manager_table = TL_RefTable.p_mem_manager_table;

  p_mem_manager_table->blepool = p_Config->p_AsynchEvtPool;
  p_mem_manager_table->blepoolsize = p_Config->AsynchEvtPoolSize;
  p_mem_manager_table->pevt_free_buffer_queue = (uint8_t*)&FreeBufQueue;
  p_mem_manager_table->spare_ble_buffer = p_Config->p_BleSpareEvtBuffer;
  p_mem_manager_table->spare_sys_buffer = p_Config->p_SystemSpareEvtBuffer;
  p_mem_manager_table->traces_evt_pool = p_Config->p_TracesEvtPool;
  p_mem_manager_table->tracespoolsize = p_Config->TracesEvtPoolSize;

  return;
}

void TL_MM_EvtDone(TL_EvtPacket_t * phcievt)
{
  LST_insert_tail(&LocalFreeBufQueue, (tListNode *)phcievt);

  OutputDbgTrace(TL_MB_MM_RELEASE_BUFFER, (uint8_t*)phcievt);

  HW_IPCC_MM_SendFreeBuf( SendFreeBuf );

  return;
}

static void SendFreeBuf( void )
{
  tListNode *p_node;

  while ( FALSE == LST_is_empty (&LocalFreeBufQueue) )
  {
    LST_remove_head( &LocalFreeBufQueue, (tListNode **)&p_node );
    LST_insert_tail( (tListNode*)(TL_RefTable.p_mem_manager_table->pevt_free_buffer_queue), p_node );
  }

  return;
}

/******************************************************************************
 * TRACES
 ******************************************************************************/
void TL_TRACES_Init( void )
{
  LST_init_head (&TracesEvtQueue);

  TL_RefTable.p_traces_table->traces_queue = (uint8_t*)&TracesEvtQueue;

  HW_IPCC_TRACES_Init();

  return;
}

void HW_IPCC_TRACES_EvtNot(void)
{
  TL_EvtPacket_t *phcievt;

  while(LST_is_empty(&TracesEvtQueue) == FALSE)
  {
    LST_remove_head (&TracesEvtQueue, (tListNode **)&phcievt);
    TL_TRACES_EvtReceived( phcievt );
  }

  return;
}

__WEAK void TL_TRACES_EvtReceived( TL_EvtPacket_t * hcievt )
{
  (void)(hcievt);
}

/******************************************************************************
 * DEBUG INFORMATION
 ******************************************************************************/
static void OutputDbgTrace(TL_MB_PacketType_t packet_type, uint8_t* buffer)
{
  TL_EvtPacket_t *p_evt_packet;
  TL_CmdPacket_t *p_cmd_packet;

  switch(packet_type)
  {
    case TL_MB_MM_RELEASE_BUFFER:
      p_evt_packet = (TL_EvtPacket_t*)buffer;
      switch(p_evt_packet->evtserial.evt.evtcode)
      {
        case TL_BLEEVT_CS_OPCODE:
          TL_MM_DBG_MSG("mm evt released: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
          TL_MM_DBG_MSG(" cmd opcode: 0x%04X", ((TL_CsEvt_t*)(p_evt_packet->evtserial.evt.payload))->cmdcode);
          TL_MM_DBG_MSG(" buffer addr: 0x%08X", p_evt_packet);
          break;

        case TL_BLEEVT_CC_OPCODE:
          TL_MM_DBG_MSG("mm evt released: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
          TL_MM_DBG_MSG(" cmd opcode: 0x%04X", ((TL_CcEvt_t*)(p_evt_packet->evtserial.evt.payload))->cmdcode);
          TL_MM_DBG_MSG(" buffer addr: 0x%08X", p_evt_packet);
          break;

        case TL_BLEEVT_VS_OPCODE:
          TL_MM_DBG_MSG("mm evt released: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
          TL_MM_DBG_MSG(" subevtcode: 0x%04X", ((TL_AsynchEvt_t*)(p_evt_packet->evtserial.evt.payload))->subevtcode);
          TL_MM_DBG_MSG(" buffer addr: 0x%08X", p_evt_packet);
          break;

        default:
          TL_MM_DBG_MSG("mm evt released: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
          TL_MM_DBG_MSG(" buffer addr: 0x%08X", p_evt_packet);
          break;
      }

      TL_MM_DBG_MSG("\r\n");
      break;

    case TL_MB_BLE_CMD:
      p_cmd_packet = (TL_CmdPacket_t*)buffer;
      TL_HCI_CMD_DBG_MSG("ble cmd: 0x%04X", p_cmd_packet->cmdserial.cmd.cmdcode);
      if(p_cmd_packet->cmdserial.cmd.plen != 0)
      {
        TL_HCI_CMD_DBG_MSG(" payload:");
        TL_HCI_CMD_DBG_BUF(p_cmd_packet->cmdserial.cmd.payload, p_cmd_packet->cmdserial.cmd.plen, "");
      }
      TL_HCI_CMD_DBG_MSG("\r\n");

      TL_HCI_CMD_DBG_RAW(&p_cmd_packet->cmdserial, p_cmd_packet->cmdserial.cmd.plen+TL_CMD_HDR_SIZE);
      break;

    case TL_MB_BLE_CMD_RSP:
      p_evt_packet = (TL_EvtPacket_t*)buffer;
      switch(p_evt_packet->evtserial.evt.evtcode)
      {
        case TL_BLEEVT_CS_OPCODE:
          TL_HCI_CMD_DBG_MSG("ble rsp: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
          TL_HCI_CMD_DBG_MSG(" cmd opcode: 0x%04X", ((TL_CsEvt_t*)(p_evt_packet->evtserial.evt.payload))->cmdcode);
          TL_HCI_CMD_DBG_MSG(" numhci: 0x%02X", ((TL_CsEvt_t*)(p_evt_packet->evtserial.evt.payload))->numcmd);
          TL_HCI_CMD_DBG_MSG(" status: 0x%02X", ((TL_CsEvt_t*)(p_evt_packet->evtserial.evt.payload))->status);
          break;

        case TL_BLEEVT_CC_OPCODE:
          TL_HCI_CMD_DBG_MSG("ble rsp: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
          TL_HCI_CMD_DBG_MSG(" cmd opcode: 0x%04X", ((TL_CcEvt_t*)(p_evt_packet->evtserial.evt.payload))->cmdcode);
          TL_HCI_CMD_DBG_MSG(" numhci: 0x%02X", ((TL_CcEvt_t*)(p_evt_packet->evtserial.evt.payload))->numcmd);
          TL_HCI_CMD_DBG_MSG(" status: 0x%02X", ((TL_CcEvt_t*)(p_evt_packet->evtserial.evt.payload))->payload[0]);
          if((p_evt_packet->evtserial.evt.plen-4) != 0)
          {
            TL_HCI_CMD_DBG_MSG(" payload:");
            TL_HCI_CMD_DBG_BUF(&((TL_CcEvt_t*)(p_evt_packet->evtserial.evt.payload))->payload[1], p_evt_packet->evtserial.evt.plen-4, "");
          }
          break;

        default:
          TL_HCI_CMD_DBG_MSG("unknown ble rsp received: %02X", p_evt_packet->evtserial.evt.evtcode);
          break;
      }

      TL_HCI_CMD_DBG_MSG("\r\n");

      TL_HCI_CMD_DBG_RAW(&p_evt_packet->evtserial, p_evt_packet->evtserial.evt.plen+TL_EVT_HDR_SIZE);
      break;

    case TL_MB_BLE_ASYNCH_EVT:
      p_evt_packet = (TL_EvtPacket_t*)buffer;
      if(p_evt_packet->evtserial.evt.evtcode != TL_BLEEVT_VS_OPCODE)
      {
        TL_HCI_EVT_DBG_MSG("ble evt: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
        if((p_evt_packet->evtserial.evt.plen) != 0)
        {
          TL_HCI_EVT_DBG_MSG(" payload:");
          TL_HCI_EVT_DBG_BUF(p_evt_packet->evtserial.evt.payload, p_evt_packet->evtserial.evt.plen, "");
        }
      }
      else
      {
        TL_HCI_EVT_DBG_MSG("ble evt: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
        TL_HCI_EVT_DBG_MSG(" subevtcode: 0x%04X", ((TL_AsynchEvt_t*)(p_evt_packet->evtserial.evt.payload))->subevtcode);
        if((p_evt_packet->evtserial.evt.plen-2) != 0)
        {
          TL_HCI_EVT_DBG_MSG(" payload:");
          TL_HCI_EVT_DBG_BUF(((TL_AsynchEvt_t*)(p_evt_packet->evtserial.evt.payload))->payload, p_evt_packet->evtserial.evt.plen-2, "");
        }
      }

      TL_HCI_EVT_DBG_MSG("\r\n");

      TL_HCI_EVT_DBG_RAW(&p_evt_packet->evtserial, p_evt_packet->evtserial.evt.plen+TL_EVT_HDR_SIZE);
      break;

    case TL_MB_SYS_CMD:
      p_cmd_packet = (TL_CmdPacket_t*)buffer;

      TL_SHCI_CMD_DBG_MSG("sys cmd: 0x%04X", p_cmd_packet->cmdserial.cmd.cmdcode);

      if(p_cmd_packet->cmdserial.cmd.plen != 0)
      {
        TL_SHCI_CMD_DBG_MSG(" payload:");
        TL_SHCI_CMD_DBG_BUF(p_cmd_packet->cmdserial.cmd.payload, p_cmd_packet->cmdserial.cmd.plen, "");
      }
      TL_SHCI_CMD_DBG_MSG("\r\n");

      TL_SHCI_CMD_DBG_RAW(&p_cmd_packet->cmdserial, p_cmd_packet->cmdserial.cmd.plen+TL_CMD_HDR_SIZE);
      break;

    case TL_MB_SYS_CMD_RSP:
      p_evt_packet = (TL_EvtPacket_t*)buffer;
      switch(p_evt_packet->evtserial.evt.evtcode)
      {
        case TL_BLEEVT_CC_OPCODE:
          TL_SHCI_CMD_DBG_MSG("sys rsp: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
          TL_SHCI_CMD_DBG_MSG(" cmd opcode: 0x%02X", ((TL_CcEvt_t*)(p_evt_packet->evtserial.evt.payload))->cmdcode);
          TL_SHCI_CMD_DBG_MSG(" status: 0x%02X", ((TL_CcEvt_t*)(p_evt_packet->evtserial.evt.payload))->payload[0]);
          if((p_evt_packet->evtserial.evt.plen-4) != 0)
          {
            TL_SHCI_CMD_DBG_MSG(" payload:");
            TL_SHCI_CMD_DBG_BUF(&((TL_CcEvt_t*)(p_evt_packet->evtserial.evt.payload))->payload[1], p_evt_packet->evtserial.evt.plen-4, "");
          }
          break;

        default:
          TL_SHCI_CMD_DBG_MSG("unknown sys rsp received: %02X", p_evt_packet->evtserial.evt.evtcode);
          break;
      }

      TL_SHCI_CMD_DBG_MSG("\r\n");

      TL_SHCI_CMD_DBG_RAW(&p_evt_packet->evtserial, p_evt_packet->evtserial.evt.plen+TL_EVT_HDR_SIZE);
      break;

    case  TL_MB_SYS_ASYNCH_EVT:
      p_evt_packet = (TL_EvtPacket_t*)buffer;
      if(p_evt_packet->evtserial.evt.evtcode != TL_BLEEVT_VS_OPCODE)
      {
        TL_SHCI_EVT_DBG_MSG("unknown sys evt received: %02X", p_evt_packet->evtserial.evt.evtcode);
      }
      else
      {
        TL_SHCI_EVT_DBG_MSG("sys evt: 0x%02X", p_evt_packet->evtserial.evt.evtcode);
        TL_SHCI_EVT_DBG_MSG(" subevtcode: 0x%04X", ((TL_AsynchEvt_t*)(p_evt_packet->evtserial.evt.payload))->subevtcode);
        if((p_evt_packet->evtserial.evt.plen-2) != 0)
        {
          TL_SHCI_EVT_DBG_MSG(" payload:");
          TL_SHCI_EVT_DBG_BUF(((TL_AsynchEvt_t*)(p_evt_packet->evtserial.evt.payload))->payload, p_evt_packet->evtserial.evt.plen-2, "");
        }
      }

      TL_SHCI_EVT_DBG_MSG("\r\n");

      TL_SHCI_EVT_DBG_RAW(&p_evt_packet->evtserial, p_evt_packet->evtserial.evt.plen+TL_EVT_HDR_SIZE);
      break;

    default:
      break;
  }

  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
