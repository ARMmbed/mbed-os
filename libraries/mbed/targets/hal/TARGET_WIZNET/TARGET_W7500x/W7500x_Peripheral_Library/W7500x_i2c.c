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
#include "W7500x.h"
#include "W7500x_i2c.h"
#include "W7500x_gpio.h"

GPIO_InitTypeDef GPIO_InitDef;

uint32_t I2C_Init(I2C_ConfigStruct* conf)
{
    uint32_t scl_port_num;
    uint32_t scl_pin_index;
    uint32_t sda_port_num;
    uint32_t sda_pin_index;
    
    scl_port_num = I2C_PORT(conf->scl);
    scl_pin_index = I2C_PIN_INDEX(conf->scl);
    
    sda_port_num = I2C_PORT(conf->sda);
    sda_pin_index = I2C_PIN_INDEX(conf->sda);
    
    //SCL setting
    GPIO_InitDef.GPIO_Pin = scl_pin_index;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;

    HAL_GPIO_Init((GPIO_TypeDef*)(GPIOA_BASE + (scl_port_num << 24)), &GPIO_InitDef);
    HAL_GPIO_SetBits((GPIO_TypeDef*)(GPIOA_BASE + (scl_port_num << 24)), scl_pin_index);
    
    //SDA setting
    GPIO_InitDef.GPIO_Pin = sda_pin_index;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
    
    HAL_GPIO_Init((GPIO_TypeDef*)(GPIOA_BASE + (sda_port_num << 24)), &GPIO_InitDef);
    HAL_GPIO_ResetBits((GPIO_TypeDef*)(GPIOA_BASE + (sda_port_num << 24)), sda_pin_index);
    
    //Pin muxing
    HAL_PAD_AFConfig((PAD_Type)scl_port_num, (uint16_t)scl_pin_index, PAD_AF1);
    HAL_PAD_AFConfig((PAD_Type)sda_port_num, (uint16_t)sda_pin_index, PAD_AF1);

    return 0;
}

void I2C_WriteBitSCL(I2C_ConfigStruct* conf, uint8_t data)
{
    uint32_t scl_port_num = I2C_PORT(conf->scl);
    uint32_t scl_pin_index = I2C_PIN_INDEX(conf->scl);

    if(data == 1)
        HAL_GPIO_SetBits((GPIO_TypeDef*)(GPIOA_BASE + (scl_port_num << 24)), scl_pin_index);
    else
        HAL_GPIO_ResetBits((GPIO_TypeDef*)(GPIOA_BASE + (scl_port_num << 24)), scl_pin_index);
    
}

void I2C_WriteBitSDA(I2C_ConfigStruct* conf, uint8_t data)
{
    uint32_t sda_port_num = I2C_PORT(conf->sda);
    uint32_t sda_pin_index = I2C_PIN_INDEX(conf->sda);

    if(data == 1)
        GPIO_OutEnClr((GPIO_TypeDef*)(GPIOA_BASE + (sda_port_num << 24)), sda_pin_index);
    else
        GPIO_OutEnSet((GPIO_TypeDef*)(GPIOA_BASE + (sda_port_num << 24)), sda_pin_index);
    
}

uint8_t I2C_ReadBitSDA(I2C_ConfigStruct* conf)
{
    uint32_t sda_port_num = I2C_PORT(conf->sda);
    uint32_t sda_pin_index = I2C_PIN_INDEX(conf->sda);
    
    if(HAL_GPIO_ReadInputDataBit((GPIO_TypeDef*)(GPIOA_BASE + (sda_port_num << 24)), sda_pin_index))
        return 1;
    
    return 0;
}

void I2C_Start(I2C_ConfigStruct* conf)
{
    I2C_WriteBitSCL(conf, 1);
    I2C_WriteBitSDA(conf, 1);
    
    I2C_WriteBitSDA(conf, 0);
    I2C_WriteBitSCL(conf, 0);
}

void I2C_Stop(I2C_ConfigStruct* conf)
{
    I2C_WriteBitSCL(conf, 0);
    I2C_WriteBitSDA(conf, 0);
    
    I2C_WriteBitSCL(conf, 1);
    I2C_WriteBitSDA(conf, 1);
}

uint8_t I2C_WriteByte(I2C_ConfigStruct* conf, uint8_t data)
{
    int i;
    uint8_t ret;
    
    //Write byte
    for(i=0; i<8; i++)
    {
        if((data << i) & 0x80)
            I2C_WriteBitSDA(conf, 1);
        else
            I2C_WriteBitSDA(conf, 0);
        
        I2C_WriteBitSCL(conf, 1);
        I2C_WriteBitSCL(conf, 0);
    }
    //Make clk for receiving ack
    I2C_WriteBitSDA(conf, 1);
    I2C_WriteBitSCL(conf, 1);
    //Read Ack/Nack
    ret = I2C_ReadBitSDA(conf);
    
    I2C_WriteBitSCL(conf, 0);
    
    return ret;
}

void I2C_SendACK(I2C_ConfigStruct* conf)
{
    I2C_WriteBitSDA(conf, 0);
    I2C_WriteBitSCL(conf, 1);
    
    I2C_WriteBitSCL(conf, 0);
}
    
void I2C_SendNACK(I2C_ConfigStruct* conf)
{
    I2C_WriteBitSDA(conf, 1);
    I2C_WriteBitSCL(conf, 1);
    
    I2C_WriteBitSCL(conf, 0);
}

uint8_t I2C_ReadByte(I2C_ConfigStruct* conf)
{
    int i;
    uint8_t ret = 0;
    
    I2C_WriteBitSDA(conf, 1); //out enable clear(GPIO is input)
    
    //Read byte
    for(i=0; i<8; i++)
    {
        I2C_WriteBitSCL(conf, 1);
        ret = (ret << 1) | (I2C_ReadBitSDA(conf));
        I2C_WriteBitSCL(conf, 0);
    }
        
    return ret;
}

int I2C_Write(I2C_ConfigStruct* conf, uint8_t addr, uint8_t* data, uint32_t len)
{
    int i;
    
    I2C_Start(conf);
    
    //Write addr
    if(I2C_WriteByte(conf, addr) != 0)
    {
        //printf("Received NACK at address phase!!\r\n");
        return -1;
    }

    //Write data
    for(i=0; i<len; i++)
    {
        if(I2C_WriteByte(conf, data[i]))
            return -1;
    }
    
    I2C_Stop(conf);
    
    return 0;//success
}

int I2C_WriteRepeated(I2C_ConfigStruct* conf, uint8_t addr, uint8_t* data, uint32_t len)
{
    int i;
    
    I2C_Start(conf);
    
    //Write addr
    if(I2C_WriteByte(conf, addr) != 0)
    {
        //printf("Received NACK at address phase!!\r\n");
        return -1;
    }

    //Write data
    for(i=0; i<len; i++)
    {
        if(I2C_WriteByte(conf, data[i]))
            return -1;
    }
    
    return 0;//success
}

int I2C_Read(I2C_ConfigStruct* conf, uint8_t addr, uint8_t* data, uint32_t len)
{
    int i;
    
    I2C_Start(conf);
    
    //Write addr | read command
    if(I2C_WriteByte(conf, (addr | 1)) != 0)
    {
        //printf("Received NACK at address phase!!\r\n");
        return -1;
    }
    
    //Read data
    for(i=0; i<len; i++)
    {
        data[i] = I2C_ReadByte(conf);
        
        if( i == (len - 1) )
            I2C_SendNACK(conf);
        else
            I2C_SendACK(conf);
    }
    
    I2C_Stop(conf);
    
    return 0;//success
}

int I2C_ReadRepeated(I2C_ConfigStruct* conf, uint8_t addr, uint8_t* data, uint32_t len)
{
    int i;
    
    I2C_Start(conf);
    
    //Write addr | read command
    if(I2C_WriteByte(conf, (addr | 1)) != 0)
    {
        //printf("Received NACK at address phase!!\r\n");
        return -1;
    }
    
    //Read data
    for(i=0; i<len; i++)
    {
        data[i] = I2C_ReadByte(conf);
        
        if( i == (len - 1) )
            I2C_SendNACK(conf);
        else
            I2C_SendACK(conf);
    }
    
    return 0;//success
}

