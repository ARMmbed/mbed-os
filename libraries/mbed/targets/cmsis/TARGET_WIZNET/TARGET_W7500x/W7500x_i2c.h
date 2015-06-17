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

#include "W7500x.h"

typedef enum
{
    I2C_WRITE_SA7=0,
    I2C_READ_SA7,
    I2C_CTRWRITE_SA7,
    I2C_CTRREAD_SA7,
} I2C_CTR;

typedef enum
{
    I2C_ACKR=0,
    I2C_ACKT,
    I2C_OACKR,
    I2C_SACKR,
    I2C_BT,
} I2C_SR;


typedef enum
{
    INT_ACKR=-1,
    INT_ACKT=-2,
    INT_NACKR=-3,
    INT_NACKT=-4,
    INT_BT=-5,
} I2C_ERROR;
    
    
/**
*@
*/
/** @defgroup I2C_registers 
  * @{
  */

#define I2C_Register_PRER               ((uint8_t)0x00)
#define I2C_Register_CTR                ((uint8_t)0x04)
#define I2C_Register_CMDR               ((uint8_t)0x08)
#define I2C_Register_SR                 ((uint8_t)0x0C)
#define I2C_Register_TSR                ((uint8_t)0x10)
#define I2C_Register_SADDR              ((uint8_t)0x14)
#define I2C_Register_TXR                ((uint8_t)0x18)
#define I2C_Register_RXR                ((uint8_t)0x1C)
#define I2C_Register_ISR                ((uint8_t)0x20)
#define I2C_Register_ISCR               ((uint8_t)0x24)
#define I2C_Register_ISMR               ((uint8_t)0x28)
#define IS_I2C_REGISTER(REGISTER) (((REGISTER) == I2C_Register_PRER) || \
                                   ((REGISTER) == I2C_Register_CTR) || \
                                   ((REGISTER) == I2C_Register_CMDR) || \
                                   ((REGISTER) == I2C_Register_SR) || \
                                   ((REGISTER) == I2C_Register_TSR) || \
                                   ((REGISTER) == I2C_Register_SADDR) || \
                                   ((REGISTER) == I2C_Register_TXR) || \
                                   ((REGISTER) == I2C_Register_RXR) || \
                                   ((REGISTER) == I2C_Register_ISR)|| \
                                   ((REGISTER) == I2C_Register_ISCR)| \
                                   ((REGISTER) == I2C_Register_ISMR))
  
/** @defgroup I2C_Private_Defines
  * @{
  */
/* I2C COREEN mask */
#define I2C_CTR_COREEN_DIS       ((uint16_t)0xFF7F)

/* I2C INTEREN mask */
#define I2C_CTR_INTEREN_DIS      ((uint16_t)0xFFBF)

/* I2C MODE(M/SN) mask */
#define I2C_CTR_MODE_SLAVE      ((uint16_t)0xFFDF)

/* I2C ADDR10(10/7N) mask */
#define I2C_CTR_ADDR10_7BIT     ((uint16_t)0xFFEF)

/* I2C CTRRWN(R/WN) mask */
#define I2C_CTR_CTRRWN_DIS      ((uint16_t)0xFFF7)

/* I2C CTREN mask */
#define I2C_CTR_CTEN_DIS        ((uint16_t)0xFFFB)


/* I2C START mask */
#define I2C_CMDR_START_DIS      ((uint16_t)0xFF7F)

/* I2C STOP mask */
#define I2C_CMDR_STOP_DIS       ((uint16_t)0xFFBF)
                                
/* I2C ACK mask */              
#define I2C_CMDR_ACK_NAK        ((uint16_t)0xFFDF)
                                
/* I2C RESTART mask */          
#define I2C_CMDR_RESTA_DIS      ((uint16_t)0xFFEF)

/* I2C INTERRUPT RESET mask */          
#define I2C_ISCR_RST_DIS      ((uint16_t)0xFFFE)
/**
  * @}
 */
  
  
  
#define I2C_WRITE               0
#define I2C_READ                1
#define I2C_RWSEL(NewState)    (((NewState) ==    I2C_WRITE)|| \
                            ((NewState) ==    I2C_READ))

                            
#define I2C_Ack_Enable                  (0x01ul  << 5)
#define I2C_Ack_Disable                 (0x00ul  << 5)
#define IS_I2C_ACK_NewState(NewState) (((NewState) == I2C_Ack_Enable) || \
                                 ((NewState) == I2C_Ack_Disable))              
                         
#define I2C_MASTER_MODE          (0x01ul  << 5 )   //  0x20
#define I2C_SLAVE_MODE          (0x00ul  << 5 )   //  0x20
#define IS_I2C_MODE(MODE)  ((MODE) ==   I2C_MASTER_MODE)|| \
                            (MODE) ==   I2C_SLAVE_MODE))  

#define I2C_CTR_MODE          (0x01ul  << 5 )   //  0x20             

#define     SLAVE_ADDR10     0x208


typedef enum
 {
    I2C_Master = I2C_MASTER_MODE,
    I2C_Slave = I2C_SLAVE_MODE
 }I2C_MODE;
 
 
typedef struct
{
    uint8_t    prescale;
    uint16_t   timeout;
    I2C_CTR     control;
}I2C_MasterConfStruct;
 

typedef struct
{
    uint32_t mode;
    uint16_t slave_address;         // only on slave mode
    I2C_MasterConfStruct master;
}I2C_ConfigStruct;


/** @defgroup I2C_Exported_Functions
  * @{
 */
 
uint32_t    I2C_Init        (I2C_TypeDef* I2Cx, I2C_ConfigStruct conf);
void        setFrequency    (I2C_TypeDef* I2Cx, uint8_t prescale);
void        I2C_DeInit      (I2C_TypeDef* I2Cx);

ErrorStatus I2C_Start       (I2C_TypeDef* I2Cx, uint16_t slave_address, I2C_CTR ctr);
void        I2C_Stop        (I2C_TypeDef* I2Cx);
void        I2C_Reset       (I2C_TypeDef* I2Cx);

void    I2C_SendData    (I2C_TypeDef* I2Cx,uint16_t Data);
int8_t  I2C_SendDataAck (I2C_TypeDef* I2Cx,uint16_t Data);
int     I2C_ReceiveData (I2C_TypeDef* I2Cx, int last);

int     I2C_Burst_Read  (I2C_TypeDef* I2Cx, uint16_t address, uint8_t *data, int length, int stop);
int     I2C_Burst_Write (I2C_TypeDef* I2Cx, uint16_t address, uint8_t *data, int length, int stop);

void I2C_GenerateSTART  (I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GenerateSTOP   (I2C_TypeDef* I2Cx, FunctionalState NewState);

void I2C_AcknowledgeConfig  (I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_RESTART            (I2C_TypeDef * I2Cx, FunctionalState NewState);

void I2C_CoreEn     (I2C_TypeDef* I2Cx,FunctionalState NewState);
void I2C_InterEn    (I2C_TypeDef* I2Cx,FunctionalState NewState);
void I2C_MasterSlave(I2C_TypeDef* I2Cx,FunctionalState NewState);
void I2C_ControlRW  (I2C_TypeDef* I2Cx,FunctionalState NewState);
void I2C_ControlEn  (I2C_TypeDef* I2Cx,FunctionalState NewState);

void I2C_InterRst       (I2C_TypeDef* I2Cx,FunctionalState NewState);
void I2C_Prescale       (I2C_TypeDef* I2Cx,uint16_t Data);
void I2C_TimeoutSet     (I2C_TypeDef* I2Cx,uint16_t Data);
void I2C_SetSlavAddress (I2C_TypeDef* I2Cx,uint16_t Data);
uint8_t I2C_StatusRead  (I2C_TypeDef* I2Cx);

ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx,I2C_SR sr);

void I2C_MasterInit         (I2C_TypeDef *  I2Cx,uint8_t Prescale,uint16_t Timeout,I2C_CTR Ctr);
void I2C_SlaveInit          (I2C_TypeDef * I2Cx,FunctionalState NewState, uint16_t data);
void I2C_SendSlaveAddress   (I2C_TypeDef* I2Cx, uint8_t SlaveAddress,I2C_CTR Ctr);

int8_t I2C_Restart_Structure(I2C_TypeDef * I2Cx,uint32_t SlaveAddress,I2C_CTR Ctr);
uint16_t I2C_ReadRegister   (I2C_TypeDef* I2Cx, uint8_t I2C_Register);

void I2C_GPIO(void);
void GPIO_I2C(void );

void WriteByte(uint8_t val);

void digitalWrite(GPIO_TypeDef* GPIOx,uint16_t pin, uint16_t val);
uint16_t digitalRead(GPIO_TypeDef* GPIOx,uint16_t pin);
void i2c_loop_us(int us);
void i2c_loop_ms(int count) ;

/**
  * @}
 */

