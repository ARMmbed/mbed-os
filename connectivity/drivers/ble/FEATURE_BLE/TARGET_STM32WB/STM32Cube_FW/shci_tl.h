/**
 ******************************************************************************
 * @file    shci_tl.h
 * @author  MCD Application Team
 * @brief   System HCI command header for the system channel
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


#ifndef __SHCI_TL_H_
#define __SHCI_TL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tl.h"

/* Exported defines -----------------------------------------------------------*/
typedef enum
{
  SHCI_TL_UserEventFlow_Disable,
  SHCI_TL_UserEventFlow_Enable,
} SHCI_TL_UserEventFlowStatus_t;

typedef enum
{
  SHCI_TL_CmdBusy,
  SHCI_TL_CmdAvailable
} SHCI_TL_CmdStatus_t;

/**
 * @brief Structure used to manage the BUS IO operations.
 *        All the structure fields will point to functions defined at user level.
 * @{
 */ 
typedef struct
{                
  int32_t (* Init)    (void* pConf); /**< Pointer to SHCI TL function for the IO Bus initialization */
  int32_t (* DeInit)  (void); /**< Pointer to SHCI TL function for the IO Bus de-initialization */
  int32_t (* Reset)   (void); /**< Pointer to SHCI TL function for the IO Bus reset */
  int32_t (* Receive) (uint8_t*, uint16_t); /**< Pointer to SHCI TL function for the IO Bus data reception */
  int32_t (* Send)    (uint8_t*, uint16_t); /**< Pointer to SHCI TL function for the IO Bus data transmission */
  int32_t (* DataAck) (uint8_t*, uint16_t* len); /**< Pointer to SHCI TL function for the IO Bus data ack reception */
  int32_t (* GetTick) (void); /**< Pointer to BSP function for getting the HAL time base timestamp */    
} tSHciIO;
/**
 * @}
 */

/**
 * @brief Contain the SHCI context
 * @{
 */
typedef struct
{   
  tSHciIO io; /**< Manage the BUS IO operations */
  void (* UserEvtRx) (void * pData); /**< User System events callback function pointer */
} tSHciContext;

typedef struct
{
  SHCI_TL_UserEventFlowStatus_t status;
  TL_EvtPacket_t *pckt;
} tSHCI_UserEvtRxParam;

typedef struct
{
  uint8_t *p_cmdbuffer;
  void (* StatusNotCallBack) (SHCI_TL_CmdStatus_t status);
} SHCI_TL_HciInitConf_t;

/**
  * shci_send
  * @brief  Send an System HCI Command
  *
  * @param : cmd_code = Opcode of the command
  * @param : len_cmd_payload = Length of the command payload
  * @param : p_cmd_payload = Address of the command payload
  * @param : p_rsp_status = Address of the full buffer holding the command complete event
  * @retval : None
  */
void shci_send( uint16_t cmd_code, uint8_t len_cmd_payload, uint8_t * p_cmd_payload, TL_EvtPacket_t * p_rsp_status );
 
/**
 * @brief  Register IO bus services.
 * @param  fops The SHCI IO structure managing the IO BUS
 * @retval None
 */
void shci_register_io_bus(tSHciIO* fops);

/**
 * @brief  Interrupt service routine that must be called when the system channel
 *         reports a packet has been received
 *
 * @param  pdata Packet or event pointer
 * @retval None
 */
void shci_notify_asynch_evt(void* pdata);

/**
 * @brief  This function resume the User Event Flow which has been stopped on return 
 *         from UserEvtRx() when the User Event has not been processed.
 *
 * @param  None
 * @retval None
 */
void shci_resume_flow(void);


/**
 * @brief  This function is called when an System HCI Command is sent to the CPU2 and the response is waited.
 *         It is called from the same context the System HCI command has been sent.
 *         It shall not return until the command response notified by shci_cmd_resp_release() is received.
 *         A weak implementation is available in shci_tl.c based on polling mechanism
 *         The user may re-implement this function in the application to improve performance :
 *         - It may use UTIL_SEQ_WaitEvt() API when using the Sequencer
 *         - It may use a semaphore when using cmsis_os interface
 *
 * @param  timeout: Waiting timeout
 * @retval None
 */
void shci_cmd_resp_wait(uint32_t timeout);

/**
 * @brief  This function is called when an System HCI command is received from the CPU2.
 *         A weak implementation is available in shci_tl.c based on polling mechanism
 *         The user may re-implement this function in the application to improve performance :
 *         - It may use UTIL_SEQ_SetEvt() API when using the Sequencer
 *         - It may use a semaphore when using cmsis_os interface
 *
 *
 * @param  flag: Release flag
 * @retval None
 */
void shci_cmd_resp_release(uint32_t flag);


/**
 * @brief  This process shall be called each time the shci_notify_asynch_evt notification is received
 *
 * @param  None
 * @retval None
 */

void shci_user_evt_proc(void);

/**
 * @brief Initialize the System Host Controller Interface.
 *        This function must be called before any communication on the System Channel
 *
 * @param  pData: System events callback function pointer
 *         This callback is triggered when an user event is received on
 *         the System Channel from CPU2.
 * @param  pConf: Configuration structure pointer
 * @retval None
 */
void shci_init(void(* UserEvtRx)(void* pData), void* pConf);

#ifdef __cplusplus
}
#endif

#endif /* __SHCI_TL_H_ */
