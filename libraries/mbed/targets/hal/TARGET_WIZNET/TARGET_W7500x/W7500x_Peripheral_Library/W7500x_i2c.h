/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of ARM Limited nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
/*include -------------------------------------*/
#ifndef __W7500X_I2C_H
#define __W7500X_I2C_H

#include "W7500x.h"


typedef enum {
    I2C_PA_5    = 0x05,
    I2C_PA_6    = 0x06,
    I2C_PA_9    = 0x09,
    I2C_PA_10   = 0x0A,
    I2C_PC_4    = 0x24,
    I2C_PC_5    = 0x25,
    I2C_PC_8    = 0x28,
    // Not connected
    I2C_NC = (int)0xFFFFFFFF
} I2C_PinName;

typedef struct
{
    I2C_PinName scl;
    I2C_PinName sda;
}I2C_ConfigStruct;


#define I2C_PORT(X) (((uint32_t)(X) >> 4) & 0xF)    // port number (0=A, 1=B, 2=C, 3=D)
#define I2C_PIN_INDEX(X)  (1 << ((uint32_t)(X) & 0xF))    // pin index : flag bit 
 
uint32_t I2C_Init(I2C_ConfigStruct* conf);

void I2C_WriteBitSDA(I2C_ConfigStruct* conf, uint8_t data);
void I2C_WriteBitSCL(I2C_ConfigStruct* conf, uint8_t data);
uint8_t I2C_ReadBitSDA(I2C_ConfigStruct* conf);

void I2C_SendACK(I2C_ConfigStruct* conf);
void I2C_SendNACK(I2C_ConfigStruct* conf);

uint8_t I2C_WriteByte(I2C_ConfigStruct* conf, uint8_t data);
uint8_t I2C_ReadByte(I2C_ConfigStruct* conf);

void I2C_Start(I2C_ConfigStruct* conf);
void I2C_Stop(I2C_ConfigStruct* conf);

int I2C_Write(I2C_ConfigStruct* conf, uint8_t addr, uint8_t* data, uint32_t len);
int I2C_WriteRepeated(I2C_ConfigStruct* conf, uint8_t addr, uint8_t* data, uint32_t len);
int I2C_Read(I2C_ConfigStruct* conf, uint8_t addr, uint8_t* data, uint32_t len);
int I2C_ReadRepeated(I2C_ConfigStruct* conf, uint8_t addr, uint8_t* data, uint32_t len);


 #endif //__W7500X_I2C_H

