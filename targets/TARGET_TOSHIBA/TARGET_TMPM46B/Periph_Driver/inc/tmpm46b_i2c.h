/**
 *******************************************************************************
 * @file    tmpm46b_i2c.h
 * @brief   This file provides all the functions prototypes for I2C driver.
 * @version V2.0.2.1
 * @date    2015/02/13
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_I2C_H
#define __TMPM46B_I2C_H

#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */

/** @defgroup I2C_Exported_Types
  * @{
  */
    typedef struct {
        uint32_t I2CSelfAddr;           /*!< Specify self-address of the I2C channel in I2C mode */
        uint32_t I2CDataLen;            /*!< Specify data length of the I2C channel in I2C mode */
        FunctionalState I2CACKState;    /*!< Enable or disable the generation of ACK clock */
        uint32_t I2CClkDiv;             /*!< Select the division of the prescaler clock for generating the serial clock */
        uint32_t PrescalerClkDiv;       /*!< Select the division of fsys for generating the fprsck */
    } I2C_InitTypeDef;

    typedef union {
        uint32_t All;
        struct {
            uint32_t LastRxBit:1;
            uint32_t GeneralCall:1;
            uint32_t SlaveAddrMatch:1;
            uint32_t ArbitrationLost:1;
            uint32_t INTReq:1;
            uint32_t BusState:1;
            uint32_t TRx:1;
            uint32_t MasterSlave:1;
        } Bit;
    } I2C_State;

#define I2C_CHANNEL_NUMBER              3U
#define IS_I2C_PERIPH(param)      (((param) == TSB_I2C0) || \
                                   ((param) == TSB_I2C1) || \
                                   ((param) == TSB_I2C2))

#define I2C_DATA_LEN_8              ((uint32_t)0x00000000)
#define I2C_DATA_LEN_1              ((uint32_t)0x00000001)
#define I2C_DATA_LEN_2              ((uint32_t)0x00000002)
#define I2C_DATA_LEN_3              ((uint32_t)0x00000003)
#define I2C_DATA_LEN_4              ((uint32_t)0x00000004)
#define I2C_DATA_LEN_5              ((uint32_t)0x00000005)
#define I2C_DATA_LEN_6              ((uint32_t)0x00000006)
#define I2C_DATA_LEN_7              ((uint32_t)0x00000007)

#define I2C_SCK_CLK_DIV_20              ((uint32_t)0x00000000)
#define I2C_SCK_CLK_DIV_24              ((uint32_t)0x00000001)
#define I2C_SCK_CLK_DIV_32              ((uint32_t)0x00000002)
#define I2C_SCK_CLK_DIV_48              ((uint32_t)0x00000003)
#define I2C_SCK_CLK_DIV_80              ((uint32_t)0x00000004)
#define I2C_SCK_CLK_DIV_144             ((uint32_t)0x00000005)
#define I2C_SCK_CLK_DIV_272             ((uint32_t)0x00000006)
#define I2C_SCK_CLK_DIV_528             ((uint32_t)0x00000007)
#define IS_I2C_SCK_CLK_DIV(param)       (((param) == I2C_SCK_CLK_DIV_20) || \
                                         ((param) == I2C_SCK_CLK_DIV_24) || \
                                         ((param) == I2C_SCK_CLK_DIV_32) || \
                                         ((param) == I2C_SCK_CLK_DIV_48) || \
                                         ((param) == I2C_SCK_CLK_DIV_80) || \
                                         ((param) == I2C_SCK_CLK_DIV_144) || \
                                         ((param) == I2C_SCK_CLK_DIV_272) || \
                                         ((param) == I2C_SCK_CLK_DIV_528))

#define I2C_PRESCALER_DIV_1              ((uint32_t)0x00000001)
#define I2C_PRESCALER_DIV_2              ((uint32_t)0x00000002)
#define I2C_PRESCALER_DIV_3              ((uint32_t)0x00000003)
#define I2C_PRESCALER_DIV_4              ((uint32_t)0x00000004)
#define I2C_PRESCALER_DIV_5              ((uint32_t)0x00000005)
#define I2C_PRESCALER_DIV_6              ((uint32_t)0x00000006)
#define I2C_PRESCALER_DIV_7              ((uint32_t)0x00000007)
#define I2C_PRESCALER_DIV_8              ((uint32_t)0x00000008)
#define I2C_PRESCALER_DIV_9              ((uint32_t)0x00000009)
#define I2C_PRESCALER_DIV_10             ((uint32_t)0x0000000A)
#define I2C_PRESCALER_DIV_11             ((uint32_t)0x0000000B)
#define I2C_PRESCALER_DIV_12             ((uint32_t)0x0000000C)
#define I2C_PRESCALER_DIV_13             ((uint32_t)0x0000000D)
#define I2C_PRESCALER_DIV_14             ((uint32_t)0x0000000E)
#define I2C_PRESCALER_DIV_15             ((uint32_t)0x0000000F)
#define I2C_PRESCALER_DIV_16             ((uint32_t)0x00000010)
#define I2C_PRESCALER_DIV_17             ((uint32_t)0x00000011)
#define I2C_PRESCALER_DIV_18             ((uint32_t)0x00000012)
#define I2C_PRESCALER_DIV_19             ((uint32_t)0x00000013)
#define I2C_PRESCALER_DIV_20             ((uint32_t)0x00000014)
#define I2C_PRESCALER_DIV_21             ((uint32_t)0x00000015)
#define I2C_PRESCALER_DIV_22             ((uint32_t)0x00000016)
#define I2C_PRESCALER_DIV_23             ((uint32_t)0x00000017)
#define I2C_PRESCALER_DIV_24             ((uint32_t)0x00000018)
#define I2C_PRESCALER_DIV_25             ((uint32_t)0x00000019)
#define I2C_PRESCALER_DIV_26             ((uint32_t)0x0000001A)
#define I2C_PRESCALER_DIV_27             ((uint32_t)0x0000001B)
#define I2C_PRESCALER_DIV_28             ((uint32_t)0x0000001C)
#define I2C_PRESCALER_DIV_29             ((uint32_t)0x0000001D)
#define I2C_PRESCALER_DIV_30             ((uint32_t)0x0000001E)
#define I2C_PRESCALER_DIV_31             ((uint32_t)0x0000001F)
#define I2C_PRESCALER_DIV_32             ((uint32_t)0x00000020)

/** @} */
/* End of group I2C_Exported_Types */

/** @defgroup I2C_Exported_Macros 
  * @{
  */
#define IS_PRESCALER_CLK_VALID(param1, param2)     (((param1) >= I2C_PRESCALER_DIV_1) && \
                                                    ((param1) <= I2C_PRESCALER_DIV_32) && \
                                                    (((param2) / (param1)) > 666666U) && \
                                                    (((param2) / (param1)) < 20000000U))

#define IS_I2C_DATA(param)              ((param) <= (uint32_t)0x000000FF)

#define IS_I2C_BIT_NUM(param)           ((param) <= (uint32_t)0x00000007)

#define IS_I2C_ADDR(param)              (((param) < (uint32_t)0x000000FF) && \
                                         (!((param) & (uint32_t)0x00000001)))

/** @} */
/* End of group I2C_Exported_Macros */

/** @defgroup I2C_Exported_FunctionPrototypes
  * @{
  */

    void I2C_SetACK(TSB_I2C_TypeDef * I2Cx, FunctionalState NewState);
    void I2C_Init(TSB_I2C_TypeDef * I2Cx, I2C_InitTypeDef * InitI2CStruct);
    void I2C_SetBitNum(TSB_I2C_TypeDef * I2Cx, uint32_t I2CBitNum);
    void I2C_SWReset(TSB_I2C_TypeDef * I2Cx);
    void I2C_ClearINTReq(TSB_I2C_TypeDef * I2Cx);
    void I2C_GenerateStart(TSB_I2C_TypeDef * I2Cx);
    void I2C_GenerateStop(TSB_I2C_TypeDef * I2Cx);
    I2C_State I2C_GetState(TSB_I2C_TypeDef * I2Cx);
    void I2C_SetSendData(TSB_I2C_TypeDef * I2Cx, uint32_t Data);
    uint32_t I2C_GetReceiveData(TSB_I2C_TypeDef * I2Cx);
    void I2C_SetFreeDataMode(TSB_I2C_TypeDef * I2Cx, FunctionalState NewState);
    FunctionalState I2C_GetSlaveAddrMatchState(TSB_I2C_TypeDef * I2Cx);
    void I2C_SetPrescalerClock(TSB_I2C_TypeDef * I2Cx, uint32_t PrescalerClock);
    void I2C_SetINTReq(TSB_I2C_TypeDef * I2Cx, FunctionalState NewState);
    FunctionalState I2C_GetINTStatus(TSB_I2C_TypeDef * I2Cx);
    void I2C_ClearINTOutput(TSB_I2C_TypeDef * I2Cx);
/** @} */
/* End of group I2C_Exported_FunctionPrototypes */

/** @} */
/* End of group I2C */

/** @} */
/* End of group TX04_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM46B_I2C_H */
