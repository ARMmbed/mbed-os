/*
 * Copyright (c) 2019 STMicroelectronics. All Rights Reserved.
 *
 * The information contained herein is property of STMicroelectronics.
 * Terms and conditions of usage are described in detail in SOFTWARE
 * LICENSE AGREEMENT:
 *
 * (SLA0068 - PRODUCTION LIMITED LICENSE AGREEMENT FOR ST MATERIALS)
 *
 * You may obtain a copy of the License here:
 * LICENSE-SLA0068.txt and at STMicroelectronics website.
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 */

#ifndef __HAL_RADIO_H_
#define __HAL_RADIO_H_

#include "../../Periph_Driver/inc/BlueNRG1_radio.h"


uint8_t HAL_RADIO_SendPacket(uint8_t channel, 
                    uint32_t wakeup_time, 
                    uint8_t* txBuffer, 
                    uint8_t (*Callback)(ActionPacket*, ActionPacket*) );
                          
uint8_t HAL_RADIO_SendPacketWithAck(uint8_t channel, 
                                 uint32_t wakeup_time, 
                                 uint8_t* txBuffer, 
                                 uint8_t* rxBuffer, 
                                 uint32_t receive_timeout,
                                 uint8_t (*Callback)(ActionPacket*, ActionPacket*) 
                                );
                                
uint8_t HAL_RADIO_ReceivePacket(uint8_t channel, 
                      uint32_t wakeup_time, 
                      uint8_t* rxBuffer,                       
                      uint32_t receive_timeout, 
                      uint8_t (*Callback)(ActionPacket*, ActionPacket*) );

uint8_t HAL_RADIO_ReceivePacketWithAck(uint8_t channel, 
                             uint32_t wakeup_time,
                             uint8_t* rxBuffer, 
                             uint8_t* txBuffer,
                             uint32_t receive_timeout, 
                             uint8_t (*Callback)(ActionPacket*, ActionPacket*)
                             );
                             
uint8_t HAL_RADIO_SetNetworkID(uint32_t ID);


#endif /* __HAL_RADIO_H_ */
