/** 
 *******************************************************************************
 * @file    bsp_i2c.c
 * @brief   This file provides API functions for BSP I2C driver.
 * @version V1.0.0.1
 * $Date:: 2017-10-03 #$
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
#include "txz_i2c_api.h"

#if defined(__BSP_I2C_H)

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

/** 
 *  @name  Parameter Result
 *  @brief Whether the parameter is specified or not.
 *  @{
 */
#define I2C_PARAM_OK              ((int32_t)1)              /*!< Parameter is valid(specified).         */
#define I2C_PARAM_NG              ((int32_t)0)              /*!< Parameter is invalid(not specified).   */
/**
 *  @}
 */ /* End of name Parameter Result */

/** 
 *  @name  timeout 
 *  @brief This timeouts are not based on accurate values, this just guarantee that 
           the application will not remain stuck if the I2C communication is corrupted.
 *  @{
 */
#define I2C_TIMEOUT     (100000)    /*>! fail safe. */

/**
 *  @}
 */ /* End of name timeout */

#define I2CxSR_AL               ((uint32_t)0x00000008)     /*!< AL                      */
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
#define I2C_CH0                     (0)         /*!< I2C Channel 0.            */
#define I2C_CH1                     (1)         /*!< I2C Channel 1.            */
#define I2C_CH2                     (2)         /*!< I2C Channel 2.            */
#define I2C_CH3                     (3)         /*!< I2C Channel 3.            */
#define I2C_CH4                     (4)         /*!< I2C Channel 3.            */
#define I2C_CH_NUM                  (5)         /*!< Number of I2C Channel.    */

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
/*----------------------------------*/
/** 
 * @brief  Transfer State.
*/
/*----------------------------------*/
enum {
    I2C_TRANSFER_STATE_IDLE = 0U,   /*!< Idle.                      */
    I2C_TRANSFER_STATE_BUSY         /*!< Busy.                      */
} TransferState;

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

/*----------------------------------*/
/** 
 * @brief  For IRQn_Type number definition.
*/
/*----------------------------------*/
typedef struct
{
    IRQn_Type i2c;
    IRQn_Type al;
    IRQn_Type bf;
    IRQn_Type na;
} i2c_irq_t;

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
 * @brief  Channel 0 IRQn_Type number table.
*/
/*----------------------------------*/
static const i2c_irq_t I2C_CH0_IRQN_TBL[1] = 
{
    { INTI2C0_IRQn, INTI2C0AL_IRQn, INTI2C0BF_IRQn, INTI2C0NACK_IRQn }
};

/*----------------------------------*/
/** 
 * @brief  Channel 1 IRQn_Type number table.
*/
/*----------------------------------*/
static const i2c_irq_t I2C_CH1_IRQN_TBL[1] = 
{
    { INTI2C1_IRQn, INTI2C1AL_IRQn, INTI2C1BF_IRQn, INTI2C1NACK_IRQn }
};

/*----------------------------------*/
/** 
 * @brief  Channel 2 IRQn_Type number table.
*/
/*----------------------------------*/
static const i2c_irq_t I2C_CH2_IRQN_TBL[1] = 
{
    { INTI2C2_IRQn, INTI2C2AL_IRQn, INTI2C2BF_IRQn, INTI2C2NACK_IRQn }
};

/*----------------------------------*/
/** 
 * @brief  Channel 3 IRQn_Type number table.
*/
/*----------------------------------*/
static const i2c_irq_t I2C_CH3_IRQN_TBL[1] = 
{
    { INTI2C3_IRQn, INTI2C3AL_IRQn, INTI2C3BF_IRQn, INTI2C3NACK_IRQn }
};

/*----------------------------------*/
/** 
 * @brief  Channel 4 IRQn_Type number table.
*/
/*----------------------------------*/
static const i2c_irq_t I2C_CH4_IRQN_TBL[1] = 
{
    { INTI2C4_IRQn, INTI2C4AL_IRQn, INTI2C4BF_IRQn, INTI2C4NACK_IRQn }
};
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
#ifdef DEBUG
__STATIC_INLINE int32_t check_param_irqn(uint32_t irqn);
__STATIC_INLINE int32_t check_param_address(int32_t address);
#endif
__STATIC_INLINE void enable_irq(uint32_t irqn);
__STATIC_INLINE void disable_irq(uint32_t irqn);
__STATIC_INLINE void clear_irq(uint32_t irqn);
__STATIC_INLINE void set_port_ch0(i2c_port_t sda, i2c_port_t scl);
__STATIC_INLINE void set_port_ch1(i2c_port_t sda, i2c_port_t scl);
__STATIC_INLINE void set_port_ch2(i2c_port_t sda, i2c_port_t scl);
__STATIC_INLINE void set_port_ch3(i2c_port_t sda, i2c_port_t scl);
__STATIC_INLINE void set_port_ch4(i2c_port_t sda, i2c_port_t scl);
__STATIC_INLINE uint32_t set_i2c(uint8_t ch, uint32_t *p_irqn);
__STATIC_INLINE void reset_asynch(_i2c_t *p_obj);
__STATIC_INLINE int32_t wait_status(_i2c_t *p_obj);
static void i2c_irq_handler(_i2c_t *p_obj);
static void i2c_slave_irq_handler(_i2c_t *p_obj);

#ifdef DEBUG
/*--------------------------------------------------*/
/** 
  * @brief  Compare the IRQn's parameter.
  * @param  irqn         :I2C IRQn List.
  * @retval I2C_PARAM_OK :Available.
  * @retval I2C_PARAM_NG :Not Available.
  * @note   -.
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_irqn(uint32_t irqn)
{
    int32_t result = I2C_PARAM_NG;

    if (irqn == (uint32_t)&I2C_CH0_IRQN_TBL)
    {
        result = I2C_PARAM_OK;
    }
    if (irqn == (uint32_t)&I2C_CH1_IRQN_TBL)
    {
        result = I2C_PARAM_OK;
    }
    if (irqn == (uint32_t)&I2C_CH2_IRQN_TBL)
    {
        result = I2C_PARAM_OK;
    }
    if (irqn == (uint32_t)&I2C_CH3_IRQN_TBL)
    {
        result = I2C_PARAM_OK;
    }
    if (irqn == (uint32_t)&I2C_CH4_IRQN_TBL)
    {
        result = I2C_PARAM_OK;
    }
    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Compare the Slave address's parameter.
  * @param  address      :Address.
  * @retval I2C_PARAM_OK :Available.
  * @retval I2C_PARAM_NG :Not Available.
  * @note   Here, 10bit address has not supported.
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_address(int32_t address)
{
    int32_t result = I2C_PARAM_NG;

    if ((address >= 0) && (address <= 255))
    {
        result = I2C_PARAM_OK;
    }
    return (result);
}
#endif

/*--------------------------------------------------*/
/**
  * @brief     Enable I2C IRQ
  * @param     irqn     :I2C IRQn List.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void enable_irq(uint32_t irqn)
{
    i2c_irq_t *p_irqn = (i2c_irq_t *)irqn;
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(check_param_irqn(irqn));
#endif /* #ifdef DEBUG */
    NVIC_EnableIRQ(p_irqn->i2c);
}

/*--------------------------------------------------*/
/**
  * @brief     Disable I2C IRQ
  * @param     irqn     :I2C IRQn List.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void disable_irq(uint32_t irqn)
{
    i2c_irq_t *p_irqn = (i2c_irq_t *)irqn;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(check_param_irqn(irqn));
#endif /* #ifdef DEBUG */
    NVIC_DisableIRQ(p_irqn->i2c);
    NVIC_DisableIRQ(p_irqn->al);
    NVIC_DisableIRQ(p_irqn->bf);
    NVIC_DisableIRQ(p_irqn->na);
}

/*--------------------------------------------------*/
/**
  * @brief     ClearPending I2C IRQ
  * @param     irqn     :I2C IRQn List.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void clear_irq(uint32_t irqn)
{
    i2c_irq_t *p_irqn = (i2c_irq_t *)irqn;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(check_param_irqn(irqn));
#endif /* #ifdef DEBUG */
    NVIC_ClearPendingIRQ(p_irqn->i2c);
    NVIC_ClearPendingIRQ(p_irqn->al);
    NVIC_ClearPendingIRQ(p_irqn->bf);
    NVIC_ClearPendingIRQ(p_irqn->na);
}

/*--------------------------------------------------*/
/**
  * @brief     I2C Port Setting (PG2, PG3)
  * @param     sda         :SDA port.
  * @param     scl         :SCL port.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void set_port_ch0(i2c_port_t sda, i2c_port_t scl)
{   
    if ((sda == I2C_PORT_PG2) && (scl == I2C_PORT_PG3))
    {
        /* Port G  */

        /* SCL */
        TSB_PG_IE_PG3IE   = 0;    /* Input       :Disable */
        TSB_PG_CR_PG3C    = 0;    /* Output      :Disable */
        TSB_PG_OD_PG3OD   = 1;    /* OD Control  :Open Drain */
        TSB_PG_PUP_PG3UP  = 0;    /* Pull-up     :Disable */
        TSB_PG_PDN_PG3DN  = 0;    /* Pull-down   :Disable */
        TSB_PG_DATA_PG3   = 0;    /* Data        :0       */
        TSB_PG_FR7_PG3F7  = 1;    /* Function    :I2C0SCL */
        TSB_PG_IE_PG3IE   = 1;    /* Input       :Enable  */
        TSB_PG_CR_PG3C    = 1;    /* Output      :Enable  */

        /* SDA */
        TSB_PG_IE_PG2IE   = 0;    /* Input      :Disable  */
        TSB_PG_CR_PG2C    = 0;    /* Output     :Disable  */
        TSB_PG_OD_PG2OD   = 1;    /* OD Control :Open Drain */
        TSB_PG_PUP_PG2UP  = 0;    /* Pull-up    :Disable  */
        TSB_PG_PDN_PG2DN  = 0;    /* Pull-down  :Disable  */
        TSB_PG_DATA_PG2   = 0;    /* Data       :0        */
        TSB_PG_FR7_PG2F7  = 1;    /* Function   :I2C0SDA  */
        TSB_PG_IE_PG2IE   = 1;    /* Input      :Enable   */
        TSB_PG_CR_PG2C    = 1;    /* Output     :Enable   */
    }
}

/*--------------------------------------------------*/
/**
  * @brief     I2C Port Setting (PF2, PF3)
  * @param     sda         :SDA port.
  * @param     scl         :SCL port.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void set_port_ch1(i2c_port_t sda, i2c_port_t scl)
{
    if ((sda == I2C_PORT_PF2) && (scl == I2C_PORT_PF3))
    {
        /* Port F  */
        
        /* SCL */
        TSB_PF_IE_PF3IE   = 0;    /* Input       :Disable */
        TSB_PF_CR_PF3C    = 0;    /* Output      :Disable */
        TSB_PF_OD_PF3OD   = 1;    /* OD Control  :Open Drain */
        TSB_PF_PUP_PF3UP  = 0;    /* Pull-up     :Disable */
        TSB_PF_PDN_PF3DN  = 0;    /* Pull-down   :Disable */
        TSB_PF_DATA_PF3   = 0;    /* Data        :0       */
        TSB_PF_FR7_PF3F7  = 1;    /* Function    :I2C0SCL */
        TSB_PF_IE_PF3IE   = 1;    /* Input       :Enable  */
        TSB_PF_CR_PF3C    = 1;    /* Output      :Enable  */

        /* SDA */
        TSB_PF_IE_PF2IE   = 0;    /* Input      :Disable  */
        TSB_PF_CR_PF2C    = 0;    /* Output     :Disable  */
        TSB_PF_OD_PF2OD   = 1;    /* OD Control :Open Drain */
        TSB_PF_PUP_PF2UP  = 0;    /* Pull-up    :Disable  */
        TSB_PF_PDN_PF2DN  = 0;    /* Pull-down  :Disable  */
        TSB_PF_DATA_PF2   = 0;    /* Data       :0        */
        TSB_PF_FR7_PF2F7  = 1;    /* Function   :I2C0SDA  */
        TSB_PF_IE_PF2IE   = 1;    /* Input      :Enable   */
        TSB_PF_CR_PF2C    = 1;    /* Output     :Enable   */
    }
}

/*--------------------------------------------------*/
/**
  * @brief     I2C Port Setting (PG4, PG5)
  * @param     sda         :SDA port.
  * @param     scl         :SCL port.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void set_port_ch2(i2c_port_t sda, i2c_port_t scl)
{   
    if ((sda == I2C_PORT_PG4) && (scl == I2C_PORT_PG5))
    {
        /* Port G  */

        /* SCL */
        TSB_PG_IE_PG5IE   = 0;    /* Input       :Disable */
        TSB_PG_CR_PG5C    = 0;    /* Output      :Disable */
        TSB_PG_OD_PG5OD   = 1;    /* OD Control  :Open Drain */
        TSB_PG_PUP_PG5UP  = 0;    /* Pull-up     :Disable */
        TSB_PG_PDN_PG5DN  = 0;    /* Pull-down   :Disable */
        TSB_PG_DATA_PG5   = 0;    /* Data        :0       */
        TSB_PG_FR7_PG5F7  = 1;    /* Function    :I2C0SCL */
        TSB_PG_IE_PG5IE   = 1;    /* Input       :Enable  */
        TSB_PG_CR_PG5C    = 1;    /* Output      :Enable  */

        /* SDA */
        TSB_PG_IE_PG4IE   = 0;    /* Input      :Disable  */
        TSB_PG_CR_PG4C    = 0;    /* Output     :Disable  */
        TSB_PG_OD_PG4OD   = 1;    /* OD Control :Open Drain */
        TSB_PG_PUP_PG4UP  = 0;    /* Pull-up    :Disable  */
        TSB_PG_PDN_PG4DN  = 0;    /* Pull-down  :Disable  */
        TSB_PG_DATA_PG4   = 0;    /* Data       :0        */
        TSB_PG_FR7_PG4F7  = 1;    /* Function   :I2C0SDA  */
        TSB_PG_IE_PG4IE   = 1;    /* Input      :Enable   */
        TSB_PG_CR_PG4C    = 1;    /* Output     :Enable   */
    }
}

/*--------------------------------------------------*/
/**
  * @brief     I2C Port Setting (PM0, PM1)
  * @param     sda         :SDA port.
  * @param     scl         :SCL port.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void set_port_ch3(i2c_port_t sda, i2c_port_t scl)
{   
    if ((sda == I2C_PORT_PJ6) && (scl == I2C_PORT_PJ7))
    {
        /* Port M  */

        /* SCL */
        TSB_PJ_IE_PJ6IE   = 0;    /* Input       :Disable */
        TSB_PJ_CR_PJ6C    = 0;    /* Output      :Disable */
        TSB_PJ_OD_PJ6OD   = 1;    /* OD Control  :Open Drain */
        TSB_PJ_PUP_PJ6UP  = 0;    /* Pull-up     :Disable */
        TSB_PJ_PDN_PJ6DN  = 0;    /* Pull-down   :Disable */
        TSB_PJ_DATA_PJ6   = 0;    /* Data        :0       */
        TSB_PJ_FR7_PJ6F7  = 1;    /* Function    :I2C3SCL */
        TSB_PJ_IE_PJ6IE   = 1;    /* Input       :Enable  */
        TSB_PJ_CR_PJ6C    = 1;    /* Output      :Enable  */

        /* SDA */
        TSB_PJ_IE_PJ7IE   = 0;    /* Input      :Disable  */
        TSB_PJ_CR_PJ7C    = 0;    /* Output     :Disable  */
        TSB_PJ_OD_PJ7OD   = 1;    /* OD Control :Open Drain */
        TSB_PJ_PUP_PJ7UP  = 0;    /* Pull-up    :Disable  */
        TSB_PJ_PDN_PJ7DN  = 0;    /* Pull-down  :Disable  */
        TSB_PJ_DATA_PJ7   = 0;    /* Data       :0        */
        TSB_PJ_FR7_PJ7F7  = 1;    /* Function   :I2C3SDA  */
        TSB_PJ_IE_PJ7IE   = 1;    /* Input      :Enable   */
        TSB_PJ_CR_PJ7C    = 1;    /* Output     :Enable   */
    }
}

/*--------------------------------------------------*/
/**
  * @brief     I2C Port Setting (PM6, PM7)
  * @param     sda         :SDA port.
  * @param     scl         :SCL port.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void set_port_ch4(i2c_port_t sda, i2c_port_t scl)
{   
    if ((sda == I2C_PORT_PJ2) && (scl == I2C_PORT_PJ3))
    {
        /* Port M  */

        /* SCL */
        TSB_PJ_IE_PJ2IE   = 0;    /* Input       :Disable */
        TSB_PJ_CR_PJ2C    = 0;    /* Output      :Disable */
        TSB_PJ_OD_PJ2OD   = 1;    /* OD Control  :Open Drain */
        TSB_PJ_PUP_PJ2UP  = 0;    /* Pull-up     :Disable */
        TSB_PJ_PDN_PJ2DN  = 0;    /* Pull-down   :Disable */
        TSB_PJ_DATA_PJ2   = 0;    /* Data        :0       */
        TSB_PJ_FR7_PJ2F7  = 1;    /* Function    :I2C3SCL */
        TSB_PJ_IE_PJ2IE   = 1;    /* Input       :Enable  */
        TSB_PJ_CR_PJ2C    = 1;    /* Output      :Enable  */

        /* SDA */
        TSB_PJ_IE_PJ3IE   = 0;    /* Input      :Disable  */
        TSB_PJ_CR_PJ3C    = 0;    /* Output     :Disable  */
        TSB_PJ_OD_PJ3OD   = 1;    /* OD Control :Open Drain */
        TSB_PJ_PUP_PJ3UP  = 0;    /* Pull-up    :Disable  */
        TSB_PJ_PDN_PJ3DN  = 0;    /* Pull-down  :Disable  */
        TSB_PJ_DATA_PJ3   = 0;    /* Data       :0        */
        TSB_PJ_FR7_PJ3F7  = 1;    /* Function   :I2C3SDA  */
        TSB_PJ_IE_PJ3IE   = 1;    /* Input      :Enable   */
        TSB_PJ_CR_PJ3C    = 1;    /* Output     :Enable   */
    }
}

/*--------------------------------------------------*/
/**
  * @brief     I2C Setting
  * @param     ch       :I2C Channel.
  * @param     p_irqn   :Destination Address of a I2C IRQn List.
  * @retval    non-zero :Instance Address.
  * @retval    zero     :Channel not supported.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE uint32_t set_i2c(uint8_t ch, uint32_t *p_irqn)
{   
    uint32_t instance = 0;

    switch (ch)
    {
        case I2C_CH0:
            instance = (uint32_t)TSB_I2C0;
            *p_irqn = (uint32_t)&I2C_CH0_IRQN_TBL;
            break;

        case I2C_CH1:
            instance = (uint32_t)TSB_I2C1;
            *p_irqn = (uint32_t)&I2C_CH1_IRQN_TBL;
            break;

        case I2C_CH2:
            instance = (uint32_t)TSB_I2C2;
            *p_irqn = (uint32_t)&I2C_CH2_IRQN_TBL;
            break;

        case I2C_CH3:
            instance = (uint32_t)TSB_I2C3;
            *p_irqn = (uint32_t)&I2C_CH3_IRQN_TBL;
            break;

        case I2C_CH4:
            instance = (uint32_t)TSB_I2C4;
            *p_irqn = (uint32_t)&I2C_CH4_IRQN_TBL;
            break;
                
        default:
            break;
    }
    return (instance);
}

/*--------------------------------------------------*/
/**
  * @brief     Reset Asynch Transfer
  * @param     p_obj    :i2c object
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void reset_asynch(_i2c_t *p_obj)
{
    disable_irq(p_obj->info.irqn);
    I2C_disable_interrupt(&p_obj->i2c);
}

__STATIC_INLINE int32_t I2C_status_arbitration(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        return ((p_obj->p_instance->SR & I2CxSR_AL) == I2CxSR_AL);
    }
    return (0);
#else
    return ((p_obj->p_instance->SR & I2CxSR_AL) == I2CxSR_AL);
#endif
}
/*--------------------------------------------------*/
/**
  * @brief     Waiting i2c status
  * @param     p_obj    :i2c object
  * @retval    0        :Success.
  * @retval    -1       :Failure.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t wait_status(_i2c_t *p_obj)
{
    int32_t timeout;

    timeout = I2C_TIMEOUT;
    while (!I2C_int_status(&p_obj->i2c))
    {
        if (I2C_status_arbitration(&p_obj->i2c))
        {
            volatile uint32_t dummy = 0;
            dummy = I2C_read_data(&p_obj->i2c);
             return (-5);
        }
        if ((timeout--) == 0)
        {
            return (-1);
        }
    }
    if (I2C_status_arbitration(&p_obj->i2c))
    {
        volatile uint32_t dummy = 0;
        dummy = I2C_read_data(&p_obj->i2c);
        return (-5);
    }
    return (0);
}

/*--------------------------------------------------*/
/**
  * @brief     I2C Transfer handler
  * @param     p_obj     :i2c object.
  * @retval    -
  * @note      Called by i2c_irq_handler_asynch_t.
  */
/*--------------------------------------------------*/
static void i2c_irq_handler(_i2c_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    I2C_clear_int_status(&p_obj->i2c);

    if ((!I2C_master(&p_obj->i2c)) || (p_obj->info.asynch.state != I2C_TRANSFER_STATE_BUSY))
    {
        p_obj->info.asynch.event = I2C_EVENT_ERROR;
        p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
    }
    else
    {
        if (I2C_transmitter(&p_obj->i2c))
        {
            int32_t start = I2C_restart(&p_obj->i2c);

            if (!I2C_get_ack(&p_obj->i2c))
            {
                if (p_obj->tx_buff.pos < p_obj->tx_buff.length)
                {
                    I2C_write_data(&p_obj->i2c, (uint32_t)p_obj->tx_buff.p_buffer[p_obj->tx_buff.pos++]);
                }
                else if (p_obj->rx_buff.length != 0)
                {
                    I2C_start_condition(&p_obj->i2c, (p_obj->info.asynch.address | 1U));
                }
                else
                {
                    if (p_obj->info.asynch.stop)
                    {
                        I2C_stop_condition(&p_obj->i2c);
                    }
                    p_obj->info.asynch.event = I2C_EVENT_TRANSFER_COMPLETE;
                    p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
                }
            }
            else
            {
                if (p_obj->tx_buff.pos < p_obj->tx_buff.length)
                {
                    if (p_obj->tx_buff.pos == 0)
                    {
                        p_obj->info.asynch.event = (I2C_EVENT_ERROR | I2C_EVENT_ERROR_NO_SLAVE);
                        p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
                    }
                    else
                    {
                        p_obj->info.asynch.event = (I2C_EVENT_ERROR | I2C_EVENT_TRANSFER_EARLY_NACK);
                        p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
                    }
                }
                else if (p_obj->rx_buff.length != 0)
                {
                    if (p_obj->tx_buff.pos == 0)
                    {
                        p_obj->info.asynch.event = (I2C_EVENT_ERROR | I2C_EVENT_ERROR_NO_SLAVE);
                        p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
                    }
                    else
                    {
                        p_obj->info.asynch.event = (I2C_EVENT_ERROR | I2C_EVENT_TRANSFER_EARLY_NACK);
                        p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
                    }
                }
                else
                {
                    if (p_obj->info.asynch.stop)
                    {
                       I2C_stop_condition(&p_obj->i2c);
                    }
                    p_obj->info.asynch.event = I2C_EVENT_TRANSFER_COMPLETE;
                    p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
                }
            }
        }
        else
        {
            int32_t start = I2C_restart(&p_obj->i2c);

            if (p_obj->rx_buff.pos < p_obj->rx_buff.length)
            {
                if (!start)
                {
                    p_obj->rx_buff.p_buffer[p_obj->rx_buff.pos++] = (uint8_t)I2C_read_data(&p_obj->i2c);
                }
            }
            if (p_obj->rx_buff.pos < p_obj->rx_buff.length)
            {
                I2C_set_ack(&p_obj->i2c, ((p_obj->rx_buff.pos < (p_obj->rx_buff.length - 1)? 0: 1)));
                I2C_write_data(&p_obj->i2c, 0);
            }
            else
            {
                if (p_obj->info.asynch.stop)
                {
                    I2C_stop_condition(&p_obj->i2c);
                }
                p_obj->info.asynch.event = I2C_EVENT_TRANSFER_COMPLETE;
                p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
            }
        }
    }
    if (p_obj->info.asynch.state == I2C_TRANSFER_STATE_IDLE)
    {
       reset_asynch(p_obj);
    }
}

/*--------------------------------------------------*/
/**
  * @brief     I2C Transfer handler
  * @param     p_obj     :i2c object.
  * @retval    -
  * @note      Called by i2c_slave_irq_handler_asynch_t.
  */
/*--------------------------------------------------*/
static void i2c_slave_irq_handler(_i2c_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    I2C_clear_int_status(&p_obj->i2c);

    if ((I2C_master(&p_obj->i2c)) || (p_obj->info.asynch.state != I2C_TRANSFER_STATE_BUSY))
    {
        p_obj->info.asynch.event = I2C_EVENT_ERROR;
        p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
    }
    else
    {
        int32_t start = I2C_slave_detected(&p_obj->i2c);
        if (start)
        {
            uint8_t sa = (uint8_t)I2C_read_data(&p_obj->i2c);
        }
        if (I2C_transmitter(&p_obj->i2c))
        {
            if (!I2C_get_ack(&p_obj->i2c))
            {
                if (p_obj->tx_buff.pos < p_obj->tx_buff.length)
                {
                    I2C_write_data(&p_obj->i2c, (uint32_t)p_obj->tx_buff.p_buffer[p_obj->tx_buff.pos++]);
                }
                else
                {   /* dummy, wait nack */
                    I2C_write_data(&p_obj->i2c, 0);
                }
            }
            else
            {
                /* error event not be set */
                p_obj->info.asynch.event = I2C_EVENT_TRANSFER_COMPLETE;
                p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
            }
        }
        else
        {
            if (p_obj->rx_buff.pos < p_obj->rx_buff.length)
            {
                if (!start)
                {
                    p_obj->rx_buff.p_buffer[p_obj->rx_buff.pos++] = (uint8_t)I2C_read_data(&p_obj->i2c);
                }
            }
            if (p_obj->rx_buff.pos < p_obj->rx_buff.length)
            {
                I2C_set_ack(&p_obj->i2c, ((p_obj->rx_buff.pos < (p_obj->rx_buff.length - 1)? 0: 1)));
                I2C_write_data(&p_obj->i2c, 0);
            }
            else
            {
                p_obj->info.asynch.event = I2C_EVENT_TRANSFER_COMPLETE;
                p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
            }
        }
    }
    if (p_obj->info.asynch.state == I2C_TRANSFER_STATE_IDLE)
    {
        reset_asynch(p_obj);
        I2C_slave_init(&p_obj->i2c);
    }
}

/*--------------------------------------------------*/
/**
  * @brief     Enable I2C IRQ
  * @param     p_obj     :i2c object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
void i2c_enable_irq(_i2c_t *p_obj)
{
    enable_irq(p_obj->info.irqn);
}

/*--------------------------------------------------*/
/**
  * @brief     Disable I2C IRQ
  * @param     p_obj     :i2c object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
void i2c_disable_irq(_i2c_t *p_obj)
{
    disable_irq(p_obj->info.irqn);
}

/**
 *  @}
 */ /* End of group UTILITIES_Private_functions */

/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Exported_functions
 *  @{
 */

/*--------------------------------------------------*/
/**
  * @brief     Initialize the I2C Driver
  * @param     p_obj       :i2c object.
  * @param     sda         :SDA port.
  * @param     scl         :SCL port.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result i2c_init_t(_i2c_t *p_obj, i2c_port_t sda, i2c_port_t scl)
{
    TXZ_Result result = TXZ_ERROR;
    uint32_t instance = 0;
    uint32_t irqn = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    /* ch0 */
    if ((sda == I2C_PORT_PG2) && (scl == I2C_PORT_PG3))
    {
        set_port_ch0(sda, scl);
        instance = set_i2c(I2C_CH0, &irqn);
    }
    /* ch1 */
    if ((sda == I2C_PORT_PF2) && (scl == I2C_PORT_PF3))
    {
        set_port_ch1(sda, scl);
        instance = set_i2c(I2C_CH1, &irqn);
    }
    /* ch2 */
    if ((sda == I2C_PORT_PG4) && (scl == I2C_PORT_PG5))
    {
        set_port_ch2(sda, scl);
        instance = set_i2c(I2C_CH2, &irqn);
    }
    /* ch3 */
    if ((sda == I2C_PORT_PJ6) && (scl == I2C_PORT_PJ7))
    {
        set_port_ch3(sda, scl);
        instance = set_i2c(I2C_CH3, &irqn);
    }
    /* ch4 */
    if ((sda == I2C_PORT_PJ2) && (scl == I2C_PORT_PJ3))
    {
        set_port_ch4(sda, scl);
        instance = set_i2c(I2C_CH3, &irqn);
    }
    
    if ((instance != 0) && (irqn != 0))
    {
        disable_irq(irqn);
        clear_irq(irqn);

        /* Set irqn table */
        p_obj->info.irqn = irqn;

        /* Set instance */
        p_obj->i2c.p_instance = (TSB_I2C_TypeDef *)instance;

        /* I2C Reset */
        i2c_reset_t(p_obj);

        /* Set Frequency Default at 100KHz */
        if (i2c_frequency_t(p_obj, 100000) == TXZ_SUCCESS)
        {
            result = TXZ_SUCCESS;
        }
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Reset I2C peripheral
  * @param     p_obj       :i2c object.
  * @retval    -
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
void i2c_reset_t(_i2c_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    /* Software reset */
    I2C_reset(&p_obj->i2c);
}

/*--------------------------------------------------*/
/**
  * @brief     Configure the I2C frequency
  * @param     p_obj       :i2c object.
  * @param     hz          :frequency in Hz.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result i2c_frequency_t(_i2c_t *p_obj, int32_t hz)
{
    TXZ_Result result = TXZ_ERROR;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    if (I2C_port_high(&p_obj->i2c))
    {
        uint32_t fval;

        SystemCoreClockUpdate();

        fval = I2C_get_clock_setting(&p_obj->i2c, (uint32_t)hz, SystemCoreClock, &p_obj->i2c.init.clock);
        if (fval != 0)
        {
            //I2C_init(&p_obj->i2c);
            p_obj->info.bus_free = 0;
            p_obj->info.start = 0;
            p_obj->info.asynch.address = 0;
            p_obj->info.asynch.stop = 0;
            p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
            p_obj->info.asynch.event = 0;
            p_obj->tx_buff.p_buffer = I2C_NULL;
            p_obj->tx_buff.length = 0;
            p_obj->tx_buff.pos = 0;
            p_obj->rx_buff.p_buffer = I2C_NULL;
            p_obj->rx_buff.length = 0;
            p_obj->rx_buff.pos = 0;
            result = TXZ_SUCCESS;
        }
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Check bus free on the I2C bus.
  * @param     p_obj       :i2c object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result i2c_check_bus_free_t(_i2c_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    p_obj->info.bus_free = 1;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Creates a start condition on the I2C bus.
  * @param     p_obj       :i2c object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.(now, not use)
  * @note      Start condition is not generate yet, after this function returned.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result i2c_start_t(_i2c_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    p_obj->info.start = 1;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Creates a stop condition on the I2C bus.
  * @param     p_obj       :i2c object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      Master and blocking function.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result i2c_stop_t(_i2c_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;
    int32_t timeout;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    I2C_stop_condition(&p_obj->i2c);
    p_obj->info.bus_free = 0;
    p_obj->info.start = 0;

    timeout = I2C_TIMEOUT;
    while (i2c_active_t(p_obj))
    {
        if ((timeout--) == 0) 
        {
            result = TXZ_ERROR;
            break;
        }
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Blocking reading data
  * @param     p_obj       :i2c object.
  * @param     address     :Slave address(7-bit) and last bit is 0. 
  * @param     p_data      :Address of Read data.
  * @param     length      :Number of the bytes to read.
  * @param     stop        :Stop to be generated after the transfer is done.
  * @retval    Number of read bytes.
  * @note      Master and blocking function.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
int32_t i2c_read_t(_i2c_t *p_obj, int32_t address, uint8_t *p_data, int32_t length, int32_t stop)
{
    int32_t result = 0;
    int32_t count = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_data));
    assert_param(check_param_address(address));
#endif /* #ifdef DEBUG */

    if (length > 0)
    {
        /* Start Condition */
        if (i2c_start_t(p_obj) == TXZ_SUCCESS)
        {
            /* no processing */
        }
        result = i2c_byte_write_t(p_obj, (int32_t)((uint32_t)address | 1U));
        if (result == I2C_ACK)
        {
            /* Read all bytes */
            while (count < length)
            {
                int32_t data = i2c_byte_read_t(p_obj, ((count < (length - 1))? 0: 1));
                if (data < 0)
                {
                    result = data;
                    break;
                }
                p_data[count++] = (uint8_t)data;
            }
            result = count;
        }
        else if (result == I2C_ERROR_ARBITRATION)
        {
        }
        else if (result == (-2)) //I2C_ERROR_BUS_BUSY
        {
        }
        else
        {
            stop = 1;
            result = (-1) ;//I2C_ERROR_NO_SLAVE;
        }
        /* Stop Condition */
        if (stop)
        {
            if (i2c_stop_t(p_obj) == TXZ_SUCCESS)
            {
                /* no processing */
            }
        }
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Blocking sending data
  * @param     p_obj       :i2c object.
  * @param     address     :Slave address(7-bit) and last bit is 0.
  * @param     p_data      :Destination address of Write data.
  * @param     length      :Number of the bytes to write.
  * @param     stop        :Stop to be generated after the transfer is done.
  * @retval    Number of write bytes.
  * @note      Master and blocking function.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
int32_t i2c_write_t(_i2c_t *p_obj, int32_t address, uint8_t *p_data, int32_t length, int32_t stop)
{
    int32_t result = 0;
    int32_t count = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_data));
    assert_param(check_param_address(address));
#endif /* #ifdef DEBUG */

    /* Start Condition */
    if (i2c_start_t(p_obj) == TXZ_SUCCESS)
    {
        /* no processing */
    }
    result = i2c_byte_write_t(p_obj, address);
    if (result == I2C_ACK)
    {
        /* Write all bytes */
        while (count < length)
        {
            int32_t data = i2c_byte_write_t(p_obj, (int32_t)p_data[count++]);
            if (data < I2C_ACK)
            {
                result = data;
                break;
            }
        }
        if(result >=0){
            result = count;
        }
    }
    else if (result == I2C_ERROR_ARBITRATION)
    {
    }
    else if (result == (-2)) //I2C_ERROR_BUS_BUSY
    {
    }
    else
    {
        stop = 1;
        result = (-1); //I2C_ERROR_NO_SLAVE;
    }
    /* Stop Condition */
    if (stop)
    {
        if (i2c_stop_t(p_obj) == TXZ_SUCCESS)
        {
            /* no processing */
        }
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Read one byte
  * @param     p_obj       :i2c object.
  * @param     last        :last acknowledge.
  * @retval    The read byte (but -1 is timout error).
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
int32_t i2c_byte_read_t(_i2c_t *p_obj, int32_t last)
{
    int32_t result;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    I2C_clear_int_status(&p_obj->i2c);
    I2C_set_ack(&p_obj->i2c, last);
    I2C_write_data(&p_obj->i2c, 0);
    result = wait_status(p_obj);
    if (result < 0)
    {
    //    result = -1;
    }
    else
    {
        result = (int32_t)I2C_read_data(&p_obj->i2c);
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Write one byte
  * @param     p_obj     :i2c object.
  * @param     data      :Write data.
  * @retval    0         :NACK was received.
  * @retval    1         :ACK was received.
  * @retval    -1        :Timout error.
  * @note      Macro definition of return values is @ref I2C_ACK.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
int32_t i2c_byte_write_t(_i2c_t *p_obj, int32_t data)
{
    int32_t result;
    int32_t timeout;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    I2C_clear_int_status(&p_obj->i2c);
    if (p_obj->info.start == 1)
    {
        p_obj->info.start = 0;
        if (p_obj->info.bus_free == 1)
        {
            timeout = I2C_TIMEOUT;
            while (i2c_active_t(p_obj))
            {
                if ((timeout--) == 0) 
                {
                    p_obj->info.bus_free = 0;
                    return (-1);
                }
            }
        }
        /* Start Condition */
        I2C_start_condition(&p_obj->i2c, (uint32_t)data);
        if ((p_obj->info.bus_free == 1) && (!I2C_master(&p_obj->i2c)))
        {
            p_obj->i2c.p_instance->CR2 = (I2CxCR2_INIT | I2CxCR2_PIN_CLEAR);
            p_obj->info.bus_free = 0;
            if (I2C_status_arbitration(&p_obj->i2c))
            {
                return (-5);
            }
            return (-2);
        }
    }
    else
    {
        I2C_write_data(&p_obj->i2c, (uint32_t)data);
    }
    p_obj->info.bus_free = 0;
    result = wait_status(p_obj);
    if (result < 0)
    {
        return (result);
    }
    if (!I2C_get_ack(&p_obj->i2c))
    {
        result = 1;
    }
    else
    {
        result = 0;
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Attempts to determine if the I2C bus is already in use
  * @param     p_obj     :i2c object.
  * @retval    0         :Non-active.
  * @retval    1         :Active.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
uint8_t i2c_active_t(_i2c_t *p_obj)
{
    uint8_t result;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    if (I2C_status_busy(&p_obj->i2c))
    {
        result = 1;     
    }
    else
    {
        result = 0;
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Start I2C asynchronous transfer
  * @param     p_obj     :i2c object.
  * @param     p_tx      :Buffer of write data.
  * @param     tx_length :Length of write data.
  * @param     p_rx      :Buffer of read data.
  * @param     rx_length :Length of read data.
  * @param     address   :Slave address(7-bit) and last bit is 0.
  * @param     stop      :Stop to be generated after the transfer is done.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      Master and non-blocking function. 
  * @note      Events of this function will be notified on i2c_irq_handler_asynch_t.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result i2c_transfer_asynch_t(_i2c_t *p_obj, uint8_t *p_tx, int32_t tx_length, uint8_t *p_rx, int32_t rx_length, int32_t address, int32_t stop)
{
    TXZ_Result result = TXZ_ERROR;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(check_param_address(address));
#endif /* #ifdef DEBUG */

    if (p_obj->info.asynch.state == I2C_TRANSFER_STATE_IDLE)
    {
        if (((p_tx != I2C_NULL) && (tx_length > 0)) || ((p_rx != I2C_NULL) && (rx_length > 0)))
        {
            reset_asynch(p_obj);
            I2C_clear_int_status(&p_obj->i2c);
            clear_irq(p_obj->info.irqn);
            p_obj->info.asynch.address = (uint32_t)address;
            p_obj->info.asynch.event = 0;
            p_obj->info.asynch.stop = (uint32_t)stop;
            p_obj->tx_buff.p_buffer = p_tx;
            p_obj->tx_buff.length = (uint32_t)tx_length;
            p_obj->tx_buff.pos = 0;
            p_obj->rx_buff.p_buffer = p_rx;
            p_obj->rx_buff.length = (uint32_t)rx_length;
            p_obj->rx_buff.pos = 0;
            p_obj->info.asynch.state = I2C_TRANSFER_STATE_BUSY;
            I2C_enable_interrupt(&p_obj->i2c);
            if ((tx_length == 0) && (rx_length != 0))
            {
                I2C_start_condition(&p_obj->i2c, (uint32_t)((uint32_t)address | 1U));
            }
            else
            {
                I2C_start_condition(&p_obj->i2c, (uint32_t)address);
            }
            p_obj->info.bus_free = 0;
            p_obj->info.start = 0;
            enable_irq(p_obj->info.irqn);
            result = TXZ_SUCCESS;
        }
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     The asynchronous IRQ handler
  * @param     p_obj     :i2c object.
  * @retval    zero      :Transfer in progress.
  * @retval    non-zero  :Event information.
  * @note      Macro definition of return values is @ref I2C_Events.
  * @attention This function should be implement as INTI2Cx_IRQHandler.
  */
/*--------------------------------------------------*/
uint32_t i2c_irq_handler_asynch_t(_i2c_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    i2c_irq_handler(p_obj);

    return (p_obj->info.asynch.event & I2C_EVENT_ALL);
}

/*--------------------------------------------------*/
/**
  * @brief     Abort asynchronous transfer
  * @param     p_obj     :i2c object.
  * @retval    -
  * @note      After error event occurred on i2c_irq_handler_asynch_t, 
  * @note      call this function and clear error status. 
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
void i2c_abort_asynch_t(_i2c_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    reset_asynch(p_obj);
    if (i2c_stop_t(p_obj) == TXZ_SUCCESS)
    {
        /* no processing */
    }
    p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
    i2c_reset_t(p_obj);
    I2C_init(&p_obj->i2c);
    clear_irq(p_obj->info.irqn);
}

/*--------------------------------------------------*/
/**
  * @brief     Configure I2C as slave or master.
  * @param     p_obj        :i2c object.
  * @param     enable_slave :Enable slave mode.
  * @retval    -
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
void i2c_slave_mode_t(_i2c_t *p_obj, int32_t enable_slave)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    disable_irq(p_obj->info.irqn);

    if (enable_slave)
    {
        I2C_slave_init(&p_obj->i2c);
    }
    else
    {
        /* Slave Disable Settings. */
        i2c_reset_t(p_obj);
        I2C_init(&p_obj->i2c);
    }
    p_obj->info.bus_free = 0;
    p_obj->info.start = 0;
    I2C_clear_int_status(&p_obj->i2c);
}

/*--------------------------------------------------*/
/**
  * @brief     Check to see if the I2C slave has been addressed.
  * @param     p_obj               :i2c object.
  * @retval    I2C_NO_DATA         :The slave has not been addressed.
  * @retval    I2C_READ_ADDRESSED  :Read addresses.
  * @retval    I2C_WRITE_GENERAL   :Write to all slaves(now, not support).
  * @retval    I2C_WRITE_ADDRESSED :Write addressed.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
int32_t i2c_slave_receive_t(_i2c_t *p_obj)
{
    int32_t result = I2C_NO_DATA;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    if (I2C_slave_detected(&p_obj->i2c))
    {
        uint32_t sa = I2C_read_data(&p_obj->i2c);

        if (!I2C_transmitter(&p_obj->i2c))
        {
            result = I2C_WRITE_ADDRESSED;
        }
        else
        {
            result = I2C_READ_ADDRESSED;
        }
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Blocking reading data.
  * @param     p_obj     :i2c object.
  * @param     p_data    :Destination address of read data.
  * @param     length    :Number of bytes to read.
  * @retval    Number of read bytes.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
int32_t i2c_slave_read_t(_i2c_t *p_obj, uint8_t *p_data, int32_t length)
{
    int32_t count = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_data));
#endif /* #ifdef DEBUG */

    /* Read all bytes */
    while (count < length)
    {
        I2C_clear_int_status(&p_obj->i2c);
        I2C_set_ack(&p_obj->i2c, ((count < (length - 1))? 0: 1));
        I2C_write_data(&p_obj->i2c, 0);
        if (wait_status(p_obj) < 0)
        {
            break;
        }
        if (I2C_slave_detected(&p_obj->i2c))
        {
            return (count);
        }
        p_data[count++] = (uint8_t)I2C_read_data(&p_obj->i2c);
    }
    I2C_slave_init(&p_obj->i2c);
    return (count);
}

/*--------------------------------------------------*/
/**
  * @brief     Blocking sending data.
  * @param     p_obj     :i2c object.
  * @param     p_data    :Source address of write data.
  * @param     length    :Number of bytes to write.
  * @retval    Number of written bytes.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
int32_t i2c_slave_write_t(_i2c_t *p_obj, uint8_t *p_data, int32_t length)
{
    int32_t count = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_data));
#endif /* #ifdef DEBUG */

    /* Write all bytes */
    while (count < length)
    {
        I2C_clear_int_status(&p_obj->i2c);
        I2C_write_data(&p_obj->i2c, (uint32_t)p_data[count++]);
        if (wait_status(p_obj) < 0)
        {
            break;
        }
        if (!I2C_get_ack(&p_obj->i2c))
        {
            /* continue */
        }
        else
        {
            break;
        }
    }
    I2C_slave_init(&p_obj->i2c);
    return (count);
}

/*--------------------------------------------------*/
/**
  * @brief     Configure I2C slave address.
  * @param     p_obj     :i2c object.
  * @param     address   :Address to be set.
  * @retval    -
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
void i2c_slave_address_t(_i2c_t *p_obj, uint32_t address)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(check_param_address((int32_t)address));
#endif /* #ifdef DEBUG */

    I2C_set_address(&p_obj->i2c, address);
}


/*--------------------------------------------------*/
/**
  * @brief     Start I2C asynchronous transfer
  * @param     p_obj     :i2c object.
  * @param     p_tx      :Buffer of write data.
  * @param     tx_length :Length of write data.
  * @param     p_rx      :Buffer of read data.
  * @param     rx_length :Length of read data.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      Slave and non-blocking function.
  * @note      Events of this function will be notified on i2c_slave_irq_handler_asynch_t.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result i2c_slave_transfer_asynch_t(_i2c_t *p_obj, uint8_t *p_tx, int32_t tx_length, uint8_t *p_rx, int32_t rx_length)
{
    TXZ_Result result = TXZ_ERROR;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    if (p_obj->info.asynch.state == I2C_TRANSFER_STATE_IDLE)
    {
        if (((p_tx != I2C_NULL) && (tx_length > 0)) || ((p_rx != I2C_NULL) && (rx_length > 0)))
        {
            reset_asynch(p_obj);
            I2C_clear_int_status(&p_obj->i2c);
            clear_irq(p_obj->info.irqn);
            p_obj->info.asynch.address = 0;
            p_obj->info.asynch.event = 0;
            p_obj->info.asynch.stop = 0;
            p_obj->tx_buff.p_buffer = p_tx;
            p_obj->tx_buff.length = (uint32_t)tx_length;
            p_obj->tx_buff.pos = 0;
            p_obj->rx_buff.p_buffer = p_rx;
            p_obj->rx_buff.length = (uint32_t)rx_length;
            p_obj->rx_buff.pos = 0;
            p_obj->info.asynch.state = I2C_TRANSFER_STATE_BUSY;
            I2C_enable_interrupt(&p_obj->i2c);
            enable_irq(p_obj->info.irqn);
            result = TXZ_SUCCESS;
        }
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     The asynchronous IRQ handler
  * @param     p_obj     :i2c object.
  * @retval    zero      :Transfer in progress.
  * @retval    non-zero  :Event information.
  * @note      Macro definition of return values is @ref I2C_Events.
  * @attention This function should be implement as INTI2Cx_IRQHandler.
  */
/*--------------------------------------------------*/
uint32_t i2c_slave_irq_handler_asynch_t(_i2c_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    i2c_slave_irq_handler(p_obj);

    return (p_obj->info.asynch.event & I2C_EVENT_ALL);
}

/*--------------------------------------------------*/
/**
  * @brief     Abort asynchronous transfer
  * @param     p_obj     :i2c object.
  * @retval    -
  * @note      For a non-blocking function.
  * @note      After error event occurred on i2c_slave_irq_handler_asynch_t, 
  * @note      call this function and clear error status. 
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
void i2c_slave_abort_asynch_t(_i2c_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */

    reset_asynch(p_obj);
    p_obj->info.asynch.state = I2C_TRANSFER_STATE_IDLE;
    I2C_slave_init(&p_obj->i2c);
    I2C_clear_int_status(&p_obj->i2c);
    clear_irq(p_obj->info.irqn);
}

/**
 *  @}
 */ /* End of group UTILITIES_Exported_functions */

/**
 *  @} 
 */ /* End of group UTILITIES */

/**
 *  @} 
 */ /* End of group Example */

#endif /* defined(__BSP_I2C_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */
