/**
******************************************************************************
* @file    hal_radio.c
* @author  AMG - RF Application team
* @version V1.1.0
* @date    3-April-2018
* @brief   BlueNRG-1,2 HAL radio APIs 
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
* <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
******************************************************************************
*/
#include "hal_radio.h"

static ActionPacket aPacket[2]; 
static uint32_t networkID = 0x88DF88DF;

static uint8_t CondRoutineTrue(ActionPacket* p)
{
  return TRUE;
}

static uint8_t dataRoutineNull(ActionPacket* current_action_packet, ActionPacket* next)
{
  return TRUE;
}

static uint8_t CondRoutineRxTrue(ActionPacket* p)
{
  /* received a packet */
  if( ((p->status & BIT_TX_MODE) == 0) && ((p->status & IRQ_RCV_OK) != 0) ) {
    /* packet received without CRC error */ 
    return TRUE;
  }
  return FALSE; 
}


/**
* @brief  This routine sets the network ID field for packet transmission and filtering for the receiving.
*         Only two devices with same networkID can communicate with each other.
* @param  ID: network ID based on bluetooth specification:
*           1. It shall have no more than six consecutive zeros or ones.
*           2. It shall not have all four octets equal.
*           3. It shall have no more than 24 transitions.
*           4. It shall have a minimum of two transitions in the most significant six bits.
*
* @retval uint8_t: return value
*           - 0x00 : Success.
*           - 0xC0 : Invalid parameter.
*/
uint8_t HAL_RADIO_SetNetworkID(uint32_t ID)
{
  networkID = ID;
  return 0;
}


/**
* @brief  This routine sends a packet on a specific channel and at a specific time.
* @param  channel: Frequency channel between 0 to 39.
* @param  wakeup_time: Time of transmission in us. This is relative time regarding now.
*         The relative time cannot be less than 100 us otherwise it might wrap around after 32.7 second.
* @param  txBuffer: Pointer to TX data buffer. Second byte of this buffer must be the length of the data.
* @param  Callback: This function is being called as data routine.
*         First ActionPacket is current action packet and the second one is next action packet.
* @retval uint8_t return value
*           - 0x00 : Success.
*           - 0xC0 : Invalid parameter.
*           - 0xC4 : Radio is busy, receiving has not been triggered.
*/
uint8_t HAL_RADIO_SendPacket(uint8_t channel, 
                             uint32_t wakeup_time, 
                             uint8_t* txBuffer, 
                             uint8_t (*Callback)(ActionPacket*, ActionPacket*) )
{
  uint8_t returnValue = SUCCESS_0;
  uint32_t dummy;
  
  if(channel > 39) {
    returnValue = INVALID_PARAMETER_C0;
  }
  
  if(RADIO_GetStatus(&dummy) != BLUE_IDLE_0) {
    returnValue = RADIO_BUSY_C4;
  }
  
  if(returnValue == SUCCESS_0) {
    uint8_t map[5]= {0xFF,0xFF,0xFF,0xFF,0xFF};
    RADIO_SetChannelMap(0, &map[0]);
    RADIO_SetChannel(0, channel, 0);
    RADIO_SetTxAttributes(0, networkID, 0x555555, SCA_DEFAULTVALUE);   
      
    aPacket[0].StateMachineNo = STATE_MACHINE_0;
    aPacket[0].ActionTag =  TXRX | PLL_TRIG | TIMER_WAKEUP | RELATIVE;
    aPacket[0].WakeupTime = wakeup_time;
    aPacket[0].ReceiveWindowLength = 0; /* does not affect for Tx */
    aPacket[0].data = txBuffer;
    aPacket[0].next_true = NULL_0;
    aPacket[0].next_false = NULL_0;
    aPacket[0].condRoutine = CondRoutineTrue;
    aPacket[0].dataRoutine = Callback;
    
    RADIO_SetReservedArea(&aPacket[0]); 
    returnValue = RADIO_MakeActionPacketPending(&aPacket[0]);
  }
  
  return returnValue; 
}


/**
* @brief  This routine sends a packet on a specific channel and at a certain time then wait for receiving acknowledge.
* @param  channel: Frequency channel between 0 to 39.
* @param  wakeup_time: Time of transmission based on us. This is relative time regarding now.
*         The relative time cannot be less than 100 us otherwise it might wrap around after 32.7 second.
* @param  txBuffer: Pointer to TX data buffer. Secound byte of this buffer must be the length of the data.
* @param  rxBuffer: Pointer to RX data buffer. Secound byte of this buffer must be the length of the data.
* @param  receive_timeout: Time of RX window used to wait for the packet on us.
* @param  callback: This function is being called as data routine.
*         First ActionPacket is current action packet and the second one is next action packet.
* @retval uint8_t return value
*           - 0x00 : Success.
*           - 0xC0 : Invalid parameter.
*           - 0xC4 : Radio is busy, receiving has not been triggered.
*/
uint8_t HAL_RADIO_SendPacketWithAck(uint8_t channel, 
                                    uint32_t wakeup_time, 
                                    uint8_t* txBuffer, 
                                    uint8_t* rxBuffer, 
                                    uint32_t receive_timeout,
                                    uint8_t (*Callback)(ActionPacket*, ActionPacket*) )
{
  uint8_t returnValue = SUCCESS_0;
  uint32_t dummy;
  
  if(channel > 39) {
    returnValue = INVALID_PARAMETER_C0;      
  }

  if(RADIO_GetStatus(&dummy) != BLUE_IDLE_0) {
    returnValue = RADIO_BUSY_C4;
  }
  
  if(returnValue == SUCCESS_0) {
    uint8_t map[5]= {0xFF,0xFF,0xFF,0xFF,0xFF};
    RADIO_SetChannelMap(0, &map[0]);
    RADIO_SetChannel(0, channel, 0);
    
    RADIO_SetTxAttributes(0, networkID, 0x555555, SCA_DEFAULTVALUE);   

    aPacket[0].StateMachineNo = STATE_MACHINE_0;   
    aPacket[0].ActionTag =  TXRX | PLL_TRIG | TIMER_WAKEUP | RELATIVE;  
    aPacket[0].WakeupTime = wakeup_time;
    aPacket[0].ReceiveWindowLength = 0; /* does not affect for Tx */
    aPacket[0].data = txBuffer;
    aPacket[0].next_true = &aPacket[1];
    aPacket[0].next_false = &aPacket[1];   
    aPacket[0].condRoutine = CondRoutineTrue;
    aPacket[0].dataRoutine = dataRoutineNull;
    
    aPacket[1].StateMachineNo = STATE_MACHINE_0;   
    aPacket[1].ActionTag =  0;   
    aPacket[1].WakeupTime = wakeup_time;
    aPacket[1].ReceiveWindowLength = receive_timeout; 
    aPacket[1].data = rxBuffer; 
    aPacket[1].next_true = NULL_0;
    aPacket[1].next_false = NULL_0;    
    aPacket[1].condRoutine = CondRoutineTrue;
    aPacket[1].dataRoutine = Callback;
    
    RADIO_SetReservedArea(&aPacket[0]);
    RADIO_SetReservedArea(&aPacket[1]);
    returnValue = RADIO_MakeActionPacketPending(&aPacket[0]);    
  }
  
  return returnValue; 
}


/**
* @brief  This routine receives a packet on a specific channel and at a certain time.
* @param  channel: Frequency channel between 0 to 39.
* @param  wakeup_time: Time of transmission based on us. This is relative time regarding now.
*         The relative time cannot be less than 100 us otherwise it might wrap around after 32.7 second.
* @param  rxBuffer: Pointer to RX data buffer. Secound byte of this buffer must be the length of the data.
* @param  receive_timeout: Time of RX window used to wait for the packet on us.
* @param  callback: This function is being called as data routine.
*         First ActionPacket is current action packet and the second one is next action packet.
* @retval uint8_t return value
*           - 0x00 : Success.
*           - 0xC0 : Invalid parameter.
*           - 0xC4 : Radio is busy, receiving has not been triggered.
*/
uint8_t HAL_RADIO_ReceivePacket(uint8_t channel, 
                                uint32_t wakeup_time, 
                                uint8_t* rxBuffer,                       
                                uint32_t receive_timeout, 
                                uint8_t (*Callback)(ActionPacket*, ActionPacket*) )
{
  uint8_t returnValue = SUCCESS_0;
  uint32_t dummy;
  
  if(channel > 39) {
    returnValue = INVALID_PARAMETER_C0;
  }
  
  if(RADIO_GetStatus(&dummy) != BLUE_IDLE_0) {
    returnValue = RADIO_BUSY_C4;
  }
  
  if(returnValue == SUCCESS_0) {
    uint8_t map[5]= {0xFF,0xFF,0xFF,0xFF,0xFF};
    RADIO_SetChannelMap(0, &map[0]);
    RADIO_SetChannel(0, channel, 0);
    
    RADIO_SetTxAttributes(0, networkID, 0x555555, SCA_DEFAULTVALUE);
    
    aPacket[0].StateMachineNo = STATE_MACHINE_0;
    aPacket[0].ActionTag =  PLL_TRIG | TIMER_WAKEUP | RELATIVE;
    aPacket[0].WakeupTime = wakeup_time;
    aPacket[0].ReceiveWindowLength = receive_timeout;
    aPacket[0].data = rxBuffer;
    aPacket[0].next_true = NULL_0;
    aPacket[0].next_false = NULL_0;
    aPacket[0].condRoutine = CondRoutineTrue;
    aPacket[0].dataRoutine = Callback;
    
    RADIO_SetReservedArea(&aPacket[0]);
    returnValue = RADIO_MakeActionPacketPending(&aPacket[0]);
  }
  
  return returnValue;
}


/**
* @brief  This routine receives a packet on a specific channel and at a certain time.
*         Then sends a packet as an acknowledgment.
* @param  channel: frequency channel between 0 to 39.
* @param  wakeup_time: time of transmission based on us. This is relative time regarding now.
*         The relative time cannot be less than 100 us otherwise it might wrap around after 32.7 second.
* @param  rxBuffer: points to received data buffer. second byte of this buffer determines the length of the data.
* @param  txBuffer: points to data buffer to send. secound byte of this buffer must be the length of the buffer.
* @param  receive_timeout: Time of RX window used to wait for the packet on us.
* @param  callback: This function is being called as data routine.
*         First ActionPacket is current action packet and the second one is next action packet.
* @retval uint8_t return value
*           - 0x00 : Success.
*           - 0xC0 : Invalid parameter.
*           - 0xC4 : Radio is busy, receiving has not been triggered.
*/
uint8_t HAL_RADIO_ReceivePacketWithAck(uint8_t channel, 
                                       uint32_t wakeup_time,
                                       uint8_t* rxBuffer, 
                                       uint8_t* txBuffer,
                                       uint32_t receive_timeout, 
                                       uint8_t (*Callback)(ActionPacket*, ActionPacket*) )
{
  uint8_t returnValue = SUCCESS_0;
  uint32_t dummy;
  
  if(channel > 39) {
    returnValue = INVALID_PARAMETER_C0;      
  }
  
  if(RADIO_GetStatus(&dummy) != BLUE_IDLE_0) {
    returnValue = RADIO_BUSY_C4;
  }
    
  if(returnValue == SUCCESS_0) {
    uint8_t map[5]= {0xFF,0xFF,0xFF,0xFF,0xFF};
    RADIO_SetChannelMap(0, &map[0]);
    RADIO_SetChannel(0,channel,0);
    RADIO_SetTxAttributes(0, networkID,0x555555,SCA_DEFAULTVALUE);   
    
    aPacket[0].StateMachineNo = STATE_MACHINE_0;
    aPacket[0].ActionTag =  PLL_TRIG | TIMER_WAKEUP | RELATIVE;
    aPacket[0].WakeupTime = wakeup_time;
    aPacket[0].ReceiveWindowLength = receive_timeout;
    aPacket[0].data = rxBuffer;
    aPacket[0].next_true = &aPacket[1];
    aPacket[0].next_false = NULL_0;
    aPacket[0].condRoutine = CondRoutineRxTrue;
    aPacket[0].dataRoutine = Callback;
        
    aPacket[1].StateMachineNo = STATE_MACHINE_0;
    aPacket[1].ActionTag =  TXRX;
    aPacket[1].WakeupTime = wakeup_time;
    aPacket[1].ReceiveWindowLength = 0; /* does not affect for Tx */
    aPacket[1].data = txBuffer;
    aPacket[1].next_true = NULL_0;
    aPacket[1].next_false = NULL_0;
    aPacket[1].condRoutine = CondRoutineTrue;
    aPacket[1].dataRoutine = Callback;
    
    RADIO_SetReservedArea(&aPacket[0]); 
    RADIO_SetReservedArea(&aPacket[1]); 
    returnValue = RADIO_MakeActionPacketPending(&aPacket[0]);
  }
  
  return returnValue; 
}


/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
