/**
 *******************************************************************************
 * @file    txz_i2c.c
 * @brief   This file provides API functions for I2C Class.
 * @version V1.0.0.2
 * $Date:: 2016-11-08 00:00:00 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

#ifdef __cplusplus
 extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_i2c.h"

#if defined(__I2C_H)

/**
 *  @addtogroup Example
 *  @{
 */

/** 
 *  @addtogroup UTILITIES
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_macro
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group UTILITIES_Private_macro */

/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_define 
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group UTILITIES_Private_define */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_define 
 *  @{
 */

/** 
 *  @}
 */ /* End of group UTILITIES_Private_define */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group UTILITIES_Private_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_typedef
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group UTILITIES_Private_typedef */

/*------------------------------------------------------------------------------*/
/*  Private Member                                                              */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_variables
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group UTILITIES_Private_variables */

/*------------------------------------------------------------------------------*/
/*  Const Table                                                                 */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_const
 *  @{
 */
/*----------------------------------*/
/** 
 * @brief   SCK Divider value table.
 * @details SCK  = b000 - b111.
 * @note    NFSEL=0 (Digital Setting) Divider value.
*/
/*----------------------------------*/
static const uint32_t I2C_SCK_DIVIDER_TBL[8] = { 20,24,32,48,80,144,272,528 };
static const uint32_t I2C_SCK_LOW_MUL_TBL[8] = { 12, 14, 18, 26, 42, 74, 138, 266 };

/**
 *  @}
 */ /* End of group UTILITIES_Private_const */

/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_functions
 *  @{
 */
 
/* no define */

/**
 *  @}
 */ /* End of group UTILITIES_Private_functions */

/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_functions
 *  @{
 */

/*--------------------------------------------------*/
/**
  * @brief     Initializing I2C Regester
  * @param     p_obj       :I2C object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
void I2C_init(I2C_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    p_obj->p_instance->CR2 = I2CxCR2_I2CM_ENABLE;
    p_obj->p_instance->OP  = I2CxOP_INIT;
    p_obj->p_instance->CR1 = (I2CxCR1_ACK | /* I2CxCR1_NOACK | */ p_obj->init.clock.sck);
    p_obj->p_instance->AR  = I2CxAR_INIT;
    p_obj->p_instance->AR2 = I2CxAR2_INIT;
    p_obj->p_instance->CR2 = I2CxCR2_INIT;
    p_obj->p_instance->PRS = (I2CxPRS_PRCK & p_obj->init.clock.prsck);
    p_obj->p_instance->IE  = I2CxIE_CLEAR;
}

/*--------------------------------------------------*/
/**
  * @brief     Generate start condition
  * @param     p_obj       :I2C object.
  * @param     data        :Slave address.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
void I2C_start_condition(I2C_t *p_obj, uint32_t data)
{
    __IO uint32_t opreg;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    opreg = p_obj->p_instance->OP;
    opreg &= ~(I2CxOP_RSTA | I2CxOP_SREN);
    if(I2C_master(p_obj)){
        if ((p_obj->p_instance->SR & I2CxSR_BB))
        {
            opreg |= I2CxOP_SREN;
        }
    }
    p_obj->p_instance->CR1 = (I2CxCR1_ACK | I2CxCR1_NOACK | p_obj->init.clock.sck);
    p_obj->p_instance->OP  = opreg;
    p_obj->p_instance->DBR = (data & I2CxDBR_DB_MASK);
    p_obj->p_instance->CR2 = I2CxCR2_START_CONDITION;
}

/*--------------------------------------------------*/
/**
  * @brief     Return the I2c clock setting
  * @param     p_obj       :I2C object.
  * @param     frequency   :Maximum frequency.
  * @param     fsys        :SystemCoreClock.
  * @param     p_setting   :Clock data pointer.
  * @retval    Non-zero    :Scl frequency.
  * @retval    0           :Error.
  * @note      -
  */
/*--------------------------------------------------*/
uint32_t I2C_get_clock_setting(I2C_t *p_obj, uint32_t frequency, uint32_t fsys, I2C_clock_setting_t *p_setting)
{
    uint32_t result = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_setting));
#endif /* #ifdef DEBUG */

    if (frequency <= 1000000)
    {
        uint64_t sck, tmp_sck;
        uint64_t prsck, tmp_prsck;
        uint64_t fscl, tmp_fscl;
        uint64_t fx;
        uint64_t max_fx, min_fx;
        uint64_t low_width,low_width_min;

        sck   = tmp_sck   = 0;
        prsck = tmp_prsck = 1;
        fscl  = tmp_fscl  = 0;
        
        if(frequency <= 400000)
        {
            max_fx = 11428572U;     /* Tpresck: 87.5ns 1/87.5 = 0.0114285714 */
            min_fx =  6666666U;     /* Tpresck:150.0ns 1/150  = 0.0066666667 */
            low_width_min = 1600;
        }
        else
        {
            max_fx = 26666667U;     /* Tpresck:37.5ns 1/37.5  = 0.0266666667 */
            min_fx = 15384615U;     /* Tpresck:65.0ns 1/65    = 0.0153846154 */
            low_width_min = 675;
        }
        for (prsck = 1; prsck <= 32; prsck++)
        {
            fx = ((uint64_t)fsys / prsck);

            if ((fx < max_fx) && (fx >= min_fx))
            {
                for (sck = 0; sck <= 7; sck++)
                {
                    low_width = (uint64_t)(1000000000 * prsck * I2C_SCK_LOW_MUL_TBL[sck]) / fsys;
                    if(low_width < low_width_min)
                    {
                        continue;
                    }
                    fscl = (fx / (uint64_t)I2C_SCK_DIVIDER_TBL[sck]);

                    if ((fscl <= frequency) && (fscl > tmp_fscl))
                    {
                        tmp_fscl = fscl;
                        tmp_sck = sck;
                        tmp_prsck = (prsck < 32)? prsck: 0;
                    }
                }
            }
        }
        result = (uint32_t)tmp_fscl;
        p_setting->sck = (uint32_t)tmp_sck;
        p_setting->prsck = (tmp_prsck < 32)? (uint32_t)tmp_prsck: 0;
    }
    else
    {
        result = 0;
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Slave mode setting.
  * @param     p_obj       :I2C object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
void I2C_slave_init(I2C_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    p_obj->p_instance->OP  = I2CxOP_SLAVE_INIT;
    p_obj->p_instance->CR1 = (I2CxCR1_ACK | p_obj->init.clock.sck);
    p_obj->p_instance->CR2 = (I2CxCR2_INIT | I2CxCR2_PIN_CLEAR);
    p_obj->p_instance->CR2 = I2CxCR2_INIT;
    p_obj->p_instance->PRS = (I2CxPRS_PRCK & p_obj->init.clock.prsck);
    p_obj->p_instance->IE  = 1;
}
#if defined(I2CSxWUP_EN)
/*--------------------------------------------------*/
/**
  * @brief     I2C Wakeup Control setting.
  * @param     p_obj       :I2CS object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
void I2CS_init(I2CS_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    p_obj->p_instance->WUPCR1 = (p-obj->init.wup.sgcdi | p_obj->init.wup.ack | p_obj->init.wup.reset | p_obj->init.wup.intend);
}

/*--------------------------------------------------*/
/**
  * @brief     Primary Slave Address setting.
  * @param     p_obj       :I2CS object.
  * @param     addr        :Primary Slave Address.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
void I2CS_Primary_slave_adr_set(I2CS_t *p_obj, uint32_t adr)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    p_obj->p_instance->WUPCR2 = (0x0000000E & adr);
}

/*--------------------------------------------------*/
/**
  * @brief     Secondary Slave Address setting.
  * @param     p_obj       :I2CS object.
  * @param     addr        :Secondary Slave Address.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
void I2CS_Secondary_slave_adr_set(I2CS_t *p_obj, uint32_t adr)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    p_obj->p_instance->WUPCR3 = (0x0000000E & adr);
    p_obj->p_instance->WUPCR3 |= 0x00000001;        /* WUPSA2EN: Secondary Slave Address Use Setting */
}
#endif
/**
 *  @}
 */ /* End of group UTILITIES_Private_functions */

/**
 *  @} 
 */ /* End of group UTILITIES */

/**
 *  @} 
 */ /* End of group Example */

#endif /* defined(__I2C_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

