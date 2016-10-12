/**
 ******************************************************************************
 * @file i2c_ipc7208_map.h
 * @brief I2C IPC 7208 HW register map
 * @internal
 * @author ON Semiconductor
 * $Rev: 3324 $
 * $Date: 2015-03-27 17:00:28 +0530 (Fri, 27 Mar 2015) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup i2c_ipc7208
 *
 * @details
 * <p>
 * I2C IPC 7208 HW register map description
 * </p>
 *
 * <h1> Reference document(s) </h1>
 * <p>
 * <a href="../pdf/IPC7208_I2C_APB_DS_v1P3.pdf" target="_blank">
 * IPC7208 APB I2C Master Design Specification v1.3 </a>
 * </p>
 */

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#ifndef I2C_IPC7208_MAP_H_
#define I2C_IPC7208_MAP_H_

#include "architecture.h"

/** I2C HW Structure Overlay */
typedef struct {
    union {
        struct {
            __IO uint32_t CMD_FIFO_EMPTY :1; /**< 1 = Command FIFO is empty , 0 = Command FIFO is empty */
            __IO uint32_t RD_FIFO_NOT_EMPTY :1; /**< 0 = Read data is not ready , 1 = Read data is ready  */
            __IO uint32_t I2C_BUS_ERR :1; /**< 0 = No buss error occurred , 1 = buss error */
            __IO uint32_t RD_FIFO_UFL :1; /**< 0 = Read data FIFO is not underflowed , 1 = Read data FIFO is underflowed */
            __IO uint32_t CMD_FIFO_OFL :1;/**< 0 = Command FIFO is not overflowed 1 = Command FIFO is overflowed */
            __IO uint32_t CMD_FIFO_FULL :1; /**< 0 = Command FIFO not full , 1 = Command FIFO full */
            __IO uint32_t PAD :2; /**< Reserved . Always reads back 0. */
        } BITS;
        __IO uint32_t WORD;
    } STATUS;
    __IO uint32_t RD_FIFO_REG;/**< Data from the I2C Slave to be read by the processor. */
    __IO uint32_t CMD_REG; /**< I2C Command Programming interface */
    union {
        struct {
            __IO uint32_t CMD_FIFO_INT :1; /**< Command FIFO empty interrupt : 0 = disable , 1 = enable */
            __IO uint32_t RD_FIFO_INT :1;  /**< Read Data FIFO Not Empty Interrupt :  0 = disable , 1 = enable */
            __IO uint32_t I2C_ERR_INT :1;  /**< I2C Error Interrupt : 0 = disable , 1 = enable */
            //    __IO uint32_t PAD :4;    /**< Reserved. Writes have no effect; Read as 0x00. */
        } BITS;
        __IO uint32_t WORD;
    } IER;
    union {
        struct {
            __IO uint32_t CD_VAL :5; /**< I2C APB Clock Divider Value (low 5 bits). */
            __IO uint32_t I2C_APB_CD_EN :1; /**< 0 = I2C clock divider disable 1 = I2C clock divider enable */
            __IO uint32_t I2C_CLK_SRC :1; /**< I2C clock source : 0 =  external clock , 1 = APB clock */
            __IO uint32_t I2C_MODULE_EN :1; /**< 0 = I2C disable , 1 = I2C enable */
        } BITS;
        __IO uint32_t WORD;
    } CR;
    __IO uint32_t PRE_SCALE_REG; /* I2C APB Clock Divider Value (upper 8 bits). */
} I2cIpc7208Reg_t, *I2cIpc7208Reg_pt;

#endif /* I2C_IPC7208_MAP_H_ */
