/**
 *******************************************************************************
 * @file    tmpm46b_i2c.c
 * @brief   This file provides API functions for I2C driver.
 * @version V2.0.2.1
 * @date    2015/02/13
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm46b_i2c.h"

#if defined(__TMPM46B_I2C_H)

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @defgroup I2C 
  * @brief I2C driver modules
  * @{
  */

/** @defgroup I2C_Private_Defines
  * @{
  */

#define I2CCR1_BC_MASK                  ((uint32_t)0x0000001F)
#define I2CCR1_ACK_SET                  ((uint32_t)0x00000010)
#define I2CCR1_ACK_CLEAR                ((uint32_t)0x000000EF)
#define I2CCR1_SCK_MASK                 ((uint32_t)0x000000F8)
#define I2CCR1_NOACK_MASK               ((uint32_t)0x00000008)
#define I2CCR1_NOACK_ENABLE             ((uint32_t)0x00000000)

#define I2CCR2_PIN_SET                  ((uint32_t)0x00000010)
#define I2CCR2_I2CM_I2C                 ((uint32_t)0x00000008)
#define I2CCR2_SWRST_MASK               ((uint32_t)0xFFFFFFFC)
#define I2CCR2_SWRST_CMD1               ((uint32_t)0x00000002)
#define I2CCR2_SWRST_CMD2               ((uint32_t)0x00000001)
#define I2CCR2_START_CONDITION          ((uint32_t)0x000000F0)
#define I2CCR2_STOP_CONDITION           ((uint32_t)0x000000D0)


#define I2CAR_SA_MASK                   ((uint32_t)0x000000FE)
#define I2CAR_ALS_SET                   ((uint32_t)0x00000001)
#define I2CAR_ALS_CLEAR                 ((uint32_t)0xFFFFFFFE)
#define I2C_DATA_MASK                   ((uint32_t)0x000000FF)

#define I2CIE_IE_SET                    ((uint32_t)0x00000001)
#define I2CIE_IE_CLEAR                  ((uint32_t)0xFFFFFFFE)

#define I2CIR_ISIC_MASK                 ((uint32_t)0x00000001)
#define I2CIR_ISIC_SET                  ((uint32_t)0x00000001)

/** @} */
/* End of group I2C_Private_Defines */

/** @defgroup I2C_Exported_Functions
  * @{
  */

/**
  * @brief  Enable or disable the generation of ACK clock. 
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @param  NewState: New state of ACK clock.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void I2C_SetACK(TSB_I2C_TypeDef * I2Cx, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    tmp = I2Cx->CR1;
    if (NewState == ENABLE) {
        /* Set I2CxCR1<ACK> to enable generation of ACK clock */
        tmp |= I2CCR1_ACK_SET;
    } else {
        /* Clear I2CxCR1<ACK> to disable generation of ACK clock */
        tmp &= I2CCR1_ACK_CLEAR;
    }
    I2Cx->CR1 = tmp;
}

/**
  * @brief  Initialize the specified I2C channel in I2C mode.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @param  InitI2CStruct: The structure containing I2C in I2C mode configuration.
  * @retval None
  */
void I2C_Init(TSB_I2C_TypeDef * I2Cx, I2C_InitTypeDef * InitI2CStruct)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));
    assert_param(IS_POINTER_NOT_NULL(InitI2CStruct));
    assert_param(IS_I2C_ADDR(InitI2CStruct->I2CSelfAddr));
    assert_param(IS_I2C_BIT_NUM(InitI2CStruct->I2CDataLen));
    assert_param(IS_I2C_SCK_CLK_DIV(InitI2CStruct->I2CClkDiv));
    assert_param(IS_FUNCTIONAL_STATE(InitI2CStruct->I2CACKState));

    /* Get the system clock frequency */
    SystemCoreClockUpdate();

    /* Check the prescaler clock in the range between 50ns and 150ns */
    assert_param(IS_PRESCALER_CLK_VALID(InitI2CStruct->PrescalerClkDiv, SystemCoreClock));

    /* Set prescaler clock */
    I2Cx->PRS = InitI2CStruct->PrescalerClkDiv % I2C_PRESCALER_DIV_32;

    /* Set selfaddress for I2Cx */
    I2Cx->AR = InitI2CStruct->I2CSelfAddr & I2CAR_SA_MASK;

    /* Set I2C bit length of transfer data  */
    tmp = I2Cx->CR1 & I2CCR1_BC_MASK;
    tmp |= (InitI2CStruct->I2CDataLen << 5U);
    /* Set I2C clock division */
    tmp &= I2CCR1_SCK_MASK;
    tmp |= InitI2CStruct->I2CClkDiv;
    if (InitI2CStruct->I2CACKState) {
        /* Set I2CxCR1<ACK> to enable generation of ACK clock */
        tmp |= I2CCR1_ACK_SET;
    } else {
        /* Clear I2CxCR1<ACK> to disable generation of ACK clock */
        tmp &= I2CCR1_ACK_CLEAR;
    }
    I2Cx->CR1 = tmp;

    /* Initialize I2C to I2C Slave-Rx mode  */
    I2Cx->CR2 = I2CCR2_PIN_SET | I2CCR2_I2CM_I2C;
}

/**
  * @brief  Specify the number of bits per transfer.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @param  I2CBitNum: Specify the number of bits.
  *   This parameter can be one of the following values:
  *   I2C_DATA_LEN_8, I2C_DATA_LEN_1,I2C_DATA_LEN_2,I2C_DATA_LEN_3,
  *   I2C_DATA_LEN_4, I2C_DATA_LEN_5,I2C_DATA_LEN_6 and I2C_DATA_LEN_7.
  * @retval None
  */
void I2C_SetBitNum(TSB_I2C_TypeDef * I2Cx, uint32_t I2CBitNum)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));
    assert_param(IS_I2C_BIT_NUM(I2CBitNum));

    /* Write number of bits per transfer into I2CxCR1<BC> */
    tmp = I2Cx->CR1 & I2CCR1_BC_MASK;
    tmp |= ((uint32_t) I2CBitNum << 5U);

    I2Cx->CR1 = tmp;
}

/**
  * @brief  Reset the state of the specified I2C channel.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @retval None
  */
void I2C_SWReset(TSB_I2C_TypeDef * I2Cx)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    tmp = I2Cx->CR2 & I2CCR2_SWRST_MASK;
    I2Cx->CR2 = tmp | I2CCR2_SWRST_CMD1;
    I2Cx->CR2 = tmp | I2CCR2_SWRST_CMD2;
}

/**
  * @brief  Clear I2C interrupt request in I2C mode.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @retval None
  */
void I2C_ClearINTReq(TSB_I2C_TypeDef * I2Cx)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    /* Set I2CxCR2<PIN> to clear request, and Set I2CxCR2<I2CM> to enable  I2C operation */
    tmp = I2Cx->SR;
    tmp &= (uint32_t) 0x000000E0;
    tmp |= (I2CCR2_PIN_SET | I2CCR2_I2CM_I2C);
    I2Cx->CR2 = tmp;
}

/**
  * @brief  Set I2c bus to Master mode and Generate start condition in I2C mode.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @retval None
  */
void I2C_GenerateStart(TSB_I2C_TypeDef * I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    /* Set I2CxCR2<MST>, <TRX>, <BB> and <PIN> to generate start condition */
    I2Cx->CR2 = I2CCR2_START_CONDITION | I2CCR2_I2CM_I2C;
}

/**
  * @brief  Set I2c bus to Master mode and Generate stop condition in I2C mode.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @retval None
  */
void I2C_GenerateStop(TSB_I2C_TypeDef * I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    /* Set I2CxCR2<MST>, <TRX>, <PIN> and clear <BB> to generate stop condition */
    I2Cx->CR2 = I2CCR2_STOP_CONDITION | I2CCR2_I2CM_I2C;
}

/**
  * @brief  Get the I2C channel state in I2C mode
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @retval The state of the I2C channel in I2C bus.
  */
I2C_State I2C_GetState(TSB_I2C_TypeDef * I2Cx)
{
    I2C_State state;
    state.All = 0U;

    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    state.All = I2Cx->SR;
    state.All &= I2C_DATA_MASK;
    return state;
}

/**
  * @brief  Set data to be sent and MCU starts transmission.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @param  Data: The data to be sent, max 0xFF.
  * @retval None
  */
void I2C_SetSendData(TSB_I2C_TypeDef * I2Cx, uint32_t Data)
{
    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));
    assert_param(IS_I2C_DATA(Data));

    /* Write data into I2CxDBR */
    I2Cx->DBR = Data;
}

/**
  * @brief  Get data having been received.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @retval The data having been received
  */
uint32_t I2C_GetReceiveData(TSB_I2C_TypeDef * I2Cx)
{
    uint32_t retval = 0U;

    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    /* Save the received data */
    retval = I2Cx->DBR;
    retval &= I2C_DATA_MASK;

    return retval;
}

/**
  * @brief  Enable or disable I2C free data mode of the I2C channel.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @param  NewState: New state of free data mode.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void I2C_SetFreeDataMode(TSB_I2C_TypeDef * I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        /* Set I2CxI2CAR<ALS> to use free data mode transfer in I2C mode */
        I2Cx->AR |= I2CAR_ALS_SET;
    } else {
        /* Clear I2CxI2CAR<ALS> to not use free data mode transfer in I2C mode */
        I2Cx->AR &= I2CAR_ALS_CLEAR;
    }
}

/**
  * @brief  Get slave address match detection state.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @retval DISABLE or ENABLE.
  */
FunctionalState I2C_GetSlaveAddrMatchState(TSB_I2C_TypeDef * I2Cx)
{
    uint32_t tmp = 0U;
    FunctionalState retval = DISABLE;

    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    tmp = I2Cx->CR1 & I2CCR1_NOACK_MASK;
    if (tmp == I2CCR1_NOACK_ENABLE) {
        /* the slave address match or general call detection are enabled. */
        retval = ENABLE;
    } else {
        /* Do nothing */
    }
    return retval;
}

/**
  * @brief  Set prescaler clock of the specified I2C channel.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @param  PrescalerClock: the prescaler clock value.
  *   This parameter can be one of the following values:
  *   I2C_PRESCALER_DIV_1 to I2C_PRESCALER_DIV_32
  * @retval None
  */
void I2C_SetPrescalerClock(TSB_I2C_TypeDef * I2Cx, uint32_t PrescalerClock)
{
    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    /* Get the system clock frequency */
    SystemCoreClockUpdate();

    /* Check the prescaler clock in the range between 50ns and 150ns */
    assert_param(IS_PRESCALER_CLK_VALID(PrescalerClock, SystemCoreClock));

    /* Write prescaler clock into I2CxPRS<PRSCK> */
    I2Cx->PRS = PrescalerClock % I2C_PRESCALER_DIV_32;
}

/**
  * @brief  Enable or disable interrupt request of the I2C channel.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @param  NewState: Specify I2C interrupt setting.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void I2C_SetINTReq(TSB_I2C_TypeDef * I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        /* Set I2CxIE<IE> to enable interrupt request */
        I2Cx->IE |= I2CIE_IE_SET;
    } else {
        /* Clear I2CxIE<IE> to disable interrupt request */
        I2Cx->IE &= I2CIE_IE_CLEAR;
    }
}

/**
  * @brief  Get interrupt generation state.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @retval DISABLE or ENABLE.
  */
FunctionalState I2C_GetINTStatus(TSB_I2C_TypeDef * I2Cx)
{
    uint32_t tmp = 0U;
    FunctionalState retval = DISABLE;

    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    tmp = I2Cx->IR & I2CIR_ISIC_MASK;
    if (tmp == I2CIR_ISIC_SET) {
        /* the I2C interrupt has been generated */
        retval = ENABLE;
    } else {
        /* Do nothing */
    }
    return retval;
}

/**
  * @brief  Clear the I2C interrupt output.
  * @param  I2Cx: Select the I2C channel.
  *   This parameter can be one of the following values:
  *   TSB_I2C0,TSB_I2C1,TSB_I2C2
  * @retval None
  */
void I2C_ClearINTOutput(TSB_I2C_TypeDef * I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_PERIPH(I2Cx));

    /* Set I2CxIR<ISIC> to clear the I2C interrupt output(INTI2Cx) */
    I2Cx->IR = I2CIR_ISIC_SET;
}

/** @} */
/* End of group I2C_Exported_Functions */

/** @} */
/* End of group I2C */

/** @} */
/* End of group TX04_Periph_Driver */

#endif                          /*   defined(__TMPM46B_I2C_H) */
