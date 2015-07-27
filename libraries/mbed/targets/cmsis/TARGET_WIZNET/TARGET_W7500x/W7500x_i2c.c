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
#include <stdio.h>
#include "W7500x.h"

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

    if(scl_port_num == 0)
    {
        HAL_GPIO_Init(GPIOA, &GPIO_InitDef);
        HAL_GPIO_SetBits(GPIOA, scl_pin_index);
    }
    else if(scl_port_num == 1)
    {
        HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
        HAL_GPIO_SetBits(GPIOB, scl_pin_index);
    }
    else if(scl_port_num == 2)
    {
        HAL_GPIO_Init(GPIOC, &GPIO_InitDef);
        HAL_GPIO_SetBits(GPIOC, scl_pin_index);
    }
    else if(scl_port_num == 3)
    {
        HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
        HAL_GPIO_SetBits(GPIOD, scl_pin_index);
    }
    else
    {
        printf("SCL pin Port number error\r\n");
        return 1;
    }
    
    //SDA setting
    GPIO_InitDef.GPIO_Pin = sda_pin_index;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
    if(sda_port_num == 0)
    {
        HAL_GPIO_Init(GPIOA, &GPIO_InitDef);
        HAL_GPIO_ResetBits(GPIOA, sda_pin_index);
    }
    else if(sda_port_num == 1)
    {
        HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
        HAL_GPIO_ResetBits(GPIOB, sda_pin_index);
    }
    else if(sda_port_num == 2)
    {
        HAL_GPIO_Init(GPIOC, &GPIO_InitDef);
        HAL_GPIO_ResetBits(GPIOC, sda_pin_index);
    }
    else if(sda_port_num == 3)
    {
        HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
        HAL_GPIO_ResetBits(GPIOD, sda_pin_index);
    }
    else
    {
        printf("SDA pin Port number error\r\n");
        return 1;
    }
    
    HAL_PAD_AFConfig(scl_port_num, scl_pin_index, PAD_AF1);
    HAL_PAD_AFConfig(sda_port_num, sda_pin_index, PAD_AF1);

    return 0;
}

void I2C_WriteBitSCL(I2C_ConfigStruct* conf, uint8_t data)
{
    uint32_t scl_port_num = I2C_PORT(conf->scl);
    uint32_t scl_pin_index = I2C_PIN_INDEX(conf->scl);

    if(scl_port_num == 0)
    {
        if(data == 1)
            HAL_GPIO_SetBits(GPIOA, scl_pin_index);
        else
            HAL_GPIO_ResetBits(GPIOA, scl_pin_index);
    }
    else if(scl_port_num == 1)
    {
        if(data == 1)
            HAL_GPIO_SetBits(GPIOB, scl_pin_index);
        else
            HAL_GPIO_ResetBits(GPIOB, scl_pin_index);
    } 
    else if(scl_port_num == 2)
    {
        if(data == 1)
            HAL_GPIO_SetBits(GPIOC, scl_pin_index);
        else
            HAL_GPIO_ResetBits(GPIOC, scl_pin_index);
    }
    else if(scl_port_num == 3)
    {
        if(data == 1)
            HAL_GPIO_SetBits(GPIOD, scl_pin_index);
        else
            HAL_GPIO_ResetBits(GPIOD, scl_pin_index);
    }
}

void I2C_WriteBitSDA(I2C_ConfigStruct* conf, uint8_t data)
{
    uint32_t sda_port_num = I2C_PORT(conf->sda);
    uint32_t sda_pin_index = I2C_PIN_INDEX(conf->sda);

    if(sda_port_num == 0)
    {
        if(data == 1)
            GPIOA->OUTENCLR = sda_pin_index;
        else
            GPIOA->OUTENSET = sda_pin_index;
    }
    else if(sda_port_num == 1)
    {
        if(data == 1)
            GPIOB->OUTENCLR = sda_pin_index;
        else
            GPIOB->OUTENSET = sda_pin_index;
    } 
    else if(sda_port_num == 2)
    {
        if(data == 1)
            GPIOC->OUTENCLR = sda_pin_index;
        else
            GPIOC->OUTENSET = sda_pin_index;
    }
    else if(sda_port_num == 3)
    {
        if(data == 1)
            GPIOD->OUTENCLR = sda_pin_index;
        else
            GPIOD->OUTENSET = sda_pin_index;
    }
}

uint8_t I2C_ReadBitSDA(I2C_ConfigStruct* conf)
{
    uint32_t sda_port_num = I2C_PORT(conf->sda);
    uint32_t sda_pin_index = I2C_PIN_INDEX(conf->sda);
    
    if(sda_port_num == 0)
    {
        if(GPIOA->DATA & sda_pin_index)
            return 1;
        else
            return 0;
    }
    else if(sda_port_num == 1)
    {
        if(GPIOB->DATA & sda_pin_index)
            return 1;
        else
            return 0;
    } 
    else if(sda_port_num == 2)
    {
        if(GPIOC->DATA & sda_pin_index)
            return 1;
        else
            return 0;
    }
    else if(sda_port_num == 3)
    {
        if(GPIOD->DATA & sda_pin_index)
            return 1;
        else
            return 0;
    }
    
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
        printf("Received NACK at address phase!!\r\n");
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
        printf("Received NACK at address phase!!\r\n");
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
        printf("Received NACK at address phase!!\r\n");
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
        printf("Received NACK at address phase!!\r\n");
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

