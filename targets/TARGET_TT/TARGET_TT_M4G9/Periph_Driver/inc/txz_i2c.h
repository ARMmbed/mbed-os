/** 
 *******************************************************************************
 * @file    txz_i2c.h
 * @brief   This file provides all the functions prototypes for I2C Class.
 * @version V1.0.0.4
 * $Date:: 2016-11-24 00:00:00 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"

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

#ifdef DEBUG
/** 
 *  @name  I2C_NULL Pointer
 *  @brief NULL Pointer.
 *  @{
 */
#define I2C_NULL                ((void *)0)
/**
 *  @}
 */ /* End of name I2C_NULL Pointer */
#endif

/** 
 *  @name  I2CxST Macro Definition.
 *  @brief I2CxST Register Macro Definition.
 *  @{
 */
#define I2CxST_NACK             ((uint32_t)0x00000008)    /*!< NACK Interrupt Status.   */
#define I2CxST_I2CBF            ((uint32_t)0x00000004)    /*!< I2CBF Interrupt Status.  */
#define I2CxST_I2CAL            ((uint32_t)0x00000002)    /*!< I2CAL Interrupt Status.  */
#define I2CxST_I2C              ((uint32_t)0x00000001)    /*!< I2C Interrupt Status.    */
#define I2CxST_CLEAR            ((uint32_t)0x0000000F)    /*!< All Bits Clear.          */
/**
 *  @}
 */ /* End of name I2CxST Macro Definition */

/** 
 *  @name  I2CxCR1 Macro Definition.
 *  @brief I2CxCR1 Register Macro Definition.
 *  @{
 */
#define I2CxCR1_ACK             ((uint32_t)0x00000010)     /*!< ACK         */
#define I2CxCR1_NOACK           ((uint32_t)0x00000008)     /*!< NOACK       */
#define I2CxCR1_BC              ((uint32_t)0x000000E0)     /*!< BC          */

/**
 *  @}
 */ /* End of name I2CxCR1 Macro Definition */

/** 
 *  @name  I2CxDBR Macro Definition.
 *  @brief I2CxDBR Register Macro Definition.
 *  @{
 */
#define I2CxDBR_DB_MASK         ((uint32_t)0x000000FF)      /* !< DB 7-0 bits mask.     */
/**
 *  @}
 */ /* End of name I2CxDBR Macro Definition */


/** 
 *  @name  I2CxCR2 Macro Definition.
 *  @brief I2CxCR2 Register Macro Definition.
 *  @{
 */
#define I2CxCR2_PIN_CLEAR       ((uint32_t)0x00000010)     /*!< PIN=1                                  */
#define I2CxCR2_I2CM_DISABLE    ((uint32_t)0x00000000)     /*!< I2CM=0                                 */
#define I2CxCR2_I2CM_ENABLE     ((uint32_t)0x00000008)     /*!< I2CM=1                                 */
#define I2CxCR2_SWRES_10        ((uint32_t)0x00000002)     /*!< SWRES=10                               */
#define I2CxCR2_SWRES_01        ((uint32_t)0x00000001)     /*!< SWRES=01                               */
#define I2CxCR2_START_CONDITION ((uint32_t)0x000000F8)     /*!< MST=1,TRX=1,BB=1,PIN=1,I2CM=1          */
#define I2CxCR2_STOP_CONDITION  ((uint32_t)0x000000D8)     /*!< MST=1,TRX=1,BB=0,PIN=1,I2CM=1          */
#define I2CxCR2_INIT            ((uint32_t)0x00000008)     /*!< MST=0,TRX=0,BB=0,PIN=0,I2CM=1,SWRES=00 */

/**
 *  @}
 */ /* End of name I2CxCR2 Macro Definition */

/** 
 *  @name  I2CxSR Macro Definition.
 *  @brief I2CxSR Register Macro Definition.
 *  @{
 */
#define I2CxSR_MST              ((uint32_t)0x00000080)     /*!< MST                     */
#define I2CxSR_TRX              ((uint32_t)0x00000040)     /*!< TRX                     */
#define I2CxSR_BB               ((uint32_t)0x00000020)     /*!< BB                      */
#define I2CxSR_PIN              ((uint32_t)0x00000010)     /*!< PIN                     */
#define I2CxSR_AL               ((uint32_t)0x00000008)     /*!< AL                      */
#define I2CxSR_AAS              ((uint32_t)0x00000004)     /*!< AAS                     */
#define I2CxSR_AD0              ((uint32_t)0x00000002)     /*!< AD0                     */
#define I2CxSR_LRB              ((uint32_t)0x00000001)     /*!< LRB                     */
/**
 *  @}
 */ /* End of name I2CxSR Macro Definition */

/** 
 *  @name  I2CxPRS Macro Definition.
 *  @brief I2CxPRS Register Macro Definition.
 *  @{
 */
#define I2CxPRS_PRCK            ((uint32_t)0x0000001F)     /*!< PRCK                    */
/**
 *  @}
 */ /* End of name I2CxPRS Macro Definition */

/** 
 *  @name  I2CxIE Macro Definition.
 *  @brief I2CxIE Register Macro Definition.
 *  @{
 */
#define I2CxIE_SELPINCD         ((uint32_t)0x00000040)     /*!< SELPINCD                */
#define I2CxIE_DMARI2CTX        ((uint32_t)0x00000020)     /*!< DMARI2CTX               */
#define I2CxIE_DMARI2CRX        ((uint32_t)0x00000010)     /*!< DMARI2CRX               */
#define I2CxIE_I2C              ((uint32_t)0x00000001)     /*!< INTI2C                  */
#define I2CxIE_CLEAR            ((uint32_t)0x00000000)     /*!< All Clear Setting       */

/**
 *  @}
 */ /* End of name I2CxIE Macro Definition */


/** 
 *  @name  I2CxOP Macro Definition.
 *  @brief I2CxOP Register Macro Definition.
 *  @{
 */
#define I2CxOP_DISAL            ((uint32_t)0x00000080)     /*!< DISAL                   */
#define I2CxOP_SA2ST            ((uint32_t)0x00000040)     /*!< SA2ST                   */
#define I2CxOP_SAST             ((uint32_t)0x00000020)     /*!< SAST                    */
#define I2CxOP_NFSEL            ((uint32_t)0x00000010)     /*!< NFSEL                   */
#define I2CxOP_RSTA             ((uint32_t)0x00000008)     /*!< RSTA                    */
#define I2CxOP_GCDI             ((uint32_t)0x00000004)     /*!< GDDI                    */
#define I2CxOP_SREN             ((uint32_t)0x00000002)     /*!< SREN                    */
#define I2CxOP_MFACK            ((uint32_t)0x00000001)     /*!< MFACK                   */
#ifndef I2C_MULTI_MASTER
  #define I2CxOP_INIT             ((uint32_t)0x00000084)     /*!< Initial Settings.       */
#else
  #define I2CxOP_INIT             ((uint32_t)0x00000004)     /*!< Initial Settings.       */
#endif
#define I2CxOP_SLAVE_INIT       ((uint32_t)0x00000084)     /*!< Slave Initial Settings. */
/**
 *  @}
 */ /* End of name I2CxOP Macro Definition */
     
/** 
 *  @name  I2CxAR Macro Definition.
 *  @brief I2CxAR Register Macro Definition.
 *  @{
 */
#define I2CxAR_ALS              ((uint32_t)0x00000001)      /*!< ALS.                   */
#define I2CxAR_INIT             ((uint32_t)0x00000000)      /*!< Initial Settings.      */
#define I2CxAR2_INIT            ((uint32_t)0x00000000)      /*!< Initial Settings.      */

/**
 *  @}
 */ /* End of name I2CxAR Macro Definition */


/** 
 *  @name  I2CxPM Macro Definition.
 *  @brief I2CxPM Register Macro Definition.
 *  @{
 */
#define I2CxPM_SDA_SCL           ((uint32_t)0x00000003)      /* SDA and SCL level. */
/**
 *  @}
 */ /* End of name I2CxPM Macro Definition */
 
/** 
 *  @name  I2CxWUPCR_INT Macro Definition.
 *  @brief I2CxWUPCR_INT Register Macro Definition.
 *  @{
 */
#define I2CxWUPCR_INT_RELESE     ((uint32_t)0x00000001)      /* Interrupt Release.         */
#define I2CxWUPCR_INT_HOLD       ((uint32_t)0x00000000)      /* Interrupt setting keep it. */
/**
 *  @}
 */ /* End of name I2CxWUPCR_INT Macro Definition */

/** 
 *  @name  I2CxWUPCR_RST Macro Definition.
 *  @brief I2CxWUPCR_RST Register Macro Definition.
 *  @{
 */
#define I2CxWUPCR_RST_RESET   ((uint32_t)0x00000010)         /* I2C BUS Reset.             */
#define I2CxWUPCR_RST_RELEASE ((uint32_t)0x00000000)         /* I2C BUS Reset Release.     */
/**
 *  @}
 */ /* End of name I2CxWUPCR_RST Macro Definition */


/** 
 *  @name  I2CxWUPCR_ACK Macro Definition.
 *  @brief I2CxWUPCR_ACK Register Macro Definition.
 *  @{
 */
#define I2CxWUPCR_ACK            ((uint32_t)0x00000020)         /* ACK Output. Output "0"                */
#define I2CxWUPCR_NACK           ((uint32_t)0x00000000)         /* ACL No Output. Output "1" NACK Output */
/**
 *  @}
 */ /* End of name I2CxWUPCR_RST Macro Definition */
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

/*----------------------------------*/
/** 
 * @brief  Clock setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t               sck;            /*!< Select internal SCL output clock frequency.                 */
    uint32_t               prsck;          /*!< Prescaler clock frequency for generating the Serial clock.  */
} I2C_clock_setting_t;

/*----------------------------------*/
/** 
 * @brief  Wakeup Control setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t               sgcdi;          /*!< Select general call detect ON/OFF.    */
    uint32_t               ack;            /*!< Select ACK output.                    */
    uint32_t               reset;          /*!< I2C BUS Rest.                         */
    uint32_t               intend;         /*!< Interrupt release.                    */
} I2CS_wup_setting_t;

/*----------------------------------*/
/** 
 * @brief  Initial setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    I2C_clock_setting_t  clock;            /*!< Serial clock setting.            */
} I2C_initial_setting_t;

/*----------------------------------*/
/** 
 * @brief  Initial setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    I2CS_wup_setting_t    wup;              /*!< Wakeup Control setting.          */
} I2CS_initial_setting_t;

/*----------------------------------*/
/** 
 * @brief  I2C handle structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    TSB_I2C_TypeDef       *p_instance;     /*!< Registers base address.           */
    I2C_initial_setting_t init;            /*!< Initial setting.                  */
} I2C_t;
#if defined(I2CSxWUP_EN)
/*----------------------------------*/
/** 
 * @brief  I2CS handle structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    TSB_I2CS_TypeDef       *p_instance;     /*!< Registers base address.           */
    I2CS_initial_setting_t init;            /*!< Initial setting.                  */
} I2CS_t;
#endif
/**
 *  @}
 */ /* End of group UTILITIES_Private_typedef */

/*------------------------------------------------------------------------------*/
/*  Inline Functions                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_functions
 *  @{
 */
__STATIC_INLINE void I2C_reset(I2C_t *p_obj);
__STATIC_INLINE int32_t I2C_port_high(I2C_t *p_obj);
__STATIC_INLINE void I2C_stop_condition(I2C_t *p_obj);
__STATIC_INLINE uint32_t I2C_read_data(I2C_t *p_obj);
__STATIC_INLINE void I2C_write_data(I2C_t *p_obj, uint32_t data);
__STATIC_INLINE int32_t I2C_restart(I2C_t *p_obj);
__STATIC_INLINE void I2C_set_ack(I2C_t *p_obj, int32_t nack);
__STATIC_INLINE int32_t I2C_get_ack(I2C_t *p_obj);
__STATIC_INLINE int32_t I2C_status_busy(I2C_t *p_obj);
__STATIC_INLINE int32_t I2C_master(I2C_t *p_obj);
__STATIC_INLINE int32_t I2C_transmitter(I2C_t *p_obj);
__STATIC_INLINE int32_t I2C_int_status(I2C_t *p_obj);
__STATIC_INLINE void I2C_clear_int_status(I2C_t *p_obj);
__STATIC_INLINE void I2C_enable_interrupt(I2C_t *p_obj);
__STATIC_INLINE void I2C_enable_interrupt_dma(I2C_t *p_obj, int32_t tx);
__STATIC_INLINE void I2C_disable_interrupt(I2C_t *p_obj);
__STATIC_INLINE void I2C_set_address(I2C_t *p_obj, uint32_t addr);
__STATIC_INLINE int32_t I2C_slave_detected(I2C_t *p_obj);
 
/*--------------------------------------------------*/
/**
  * @brief     I2C software reset.
  * @param     p_obj       :I2C object.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void I2C_reset(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        p_obj->p_instance->CR2 = I2CxCR2_SWRES_10;
        p_obj->p_instance->CR2 = I2CxCR2_SWRES_01;
    }
#else
    p_obj->p_instance->CR2 = I2CxCR2_SWRES_10;
    p_obj->p_instance->CR2 = I2CxCR2_SWRES_01;
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     I2C bus port high
  * @param     p_obj       :I2C object.
  * @retval    true        :SDA and SCL Port High.
  * @retval    false       :Bus Error.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t I2C_port_high(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        return (((p_obj->p_instance->PM & I2CxPM_SDA_SCL) == I2CxPM_SDA_SCL));
    }
    return (0);
#else
    return (((p_obj->p_instance->PM & I2CxPM_SDA_SCL) == I2CxPM_SDA_SCL));
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Generate stop condition.
  * @param     p_obj       :I2C object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void I2C_stop_condition(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        p_obj->p_instance->CR2 = I2CxCR2_STOP_CONDITION;
    }
#else
    p_obj->p_instance->CR2 = I2CxCR2_STOP_CONDITION;
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Read from Data buffer
  * @param     p_obj       :I2C object.
  * @retval    result      :Read data.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE uint32_t I2C_read_data(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        return (p_obj->p_instance->DBR & I2CxDBR_DB_MASK);
    }
    return (0);
#else
    return (p_obj->p_instance->DBR & I2CxDBR_DB_MASK);
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Write to Data buffer.
  * @param     p_obj       :I2C object.
  * @param     data        :Write data.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void I2C_write_data(I2C_t *p_obj, uint32_t data)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        p_obj->p_instance->DBR = (data & I2CxDBR_DB_MASK);
    }
#else
    p_obj->p_instance->DBR = (data & I2CxDBR_DB_MASK);
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Return restart condition
  * @param     p_obj    :I2C object.
  * @retval    true     :Restart Detected.
  * @retval    false    :Restart Non-Detected.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t I2C_restart(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        __IO uint32_t opreg = p_obj->p_instance->OP;
        p_obj->p_instance->OP &= ~I2CxOP_RSTA;
        return ((opreg & I2CxOP_RSTA) == I2CxOP_RSTA);
    }
    return (0);
#else
    __IO uint32_t opreg = p_obj->p_instance->OP;
    p_obj->p_instance->OP &= ~I2CxOP_RSTA;
    return ((opreg & I2CxOP_RSTA) == I2CxOP_RSTA);
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Set Ack condition
  * @param     p_obj       :I2C object.
  * @param     nack        :1 NACK, 0 ACK.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void I2C_set_ack(I2C_t *p_obj, int32_t nack)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        if (nack)
        {
            p_obj->p_instance->OP |= I2CxOP_MFACK;
        }
        else
        {
            p_obj->p_instance->OP &= ~I2CxOP_MFACK;
        }
    }
#else
    if (nack)
    {
        p_obj->p_instance->OP |= I2CxOP_MFACK;
    }
    else
    {
        p_obj->p_instance->OP &= ~I2CxOP_MFACK;
    }
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Return received Ack condition
  * @param     p_obj    :I2C object.
  * @retval    true     :NACK Received.
  * @retval    false    :ACK Received.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t I2C_get_ack(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        return ((p_obj->p_instance->SR & I2CxSR_LRB) == I2CxSR_LRB);
    }
    return (0);
#else
    return ((p_obj->p_instance->SR & I2CxSR_LRB) == I2CxSR_LRB);
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Return Busy condition
  * @param     p_obj       :I2C object.
  * @retval    true        :I2C bus busy.
  * @retval    false       :I2C bus free.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t I2C_status_busy(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        return ((p_obj->p_instance->SR & I2CxSR_BB) == I2CxSR_BB);
    }
    return (0);
#else
    return ((p_obj->p_instance->SR & I2CxSR_BB) == I2CxSR_BB);
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Return The Master status
  * @param     p_obj       :I2C object.
  * @retval    true        :Master mode.
  * @retval    false       :Slave mode.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t I2C_master(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        return ((p_obj->p_instance->SR & I2CxSR_MST) == I2CxSR_MST);
    }
    return (0);
#else
    return ((p_obj->p_instance->SR & I2CxSR_MST) == I2CxSR_MST);
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Return The Transmitter
  * @param     p_obj       :I2C object.
  * @retval    true        :Transmitter.
  * @retval    false       :Receiver.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t I2C_transmitter(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        return ((p_obj->p_instance->SR & I2CxSR_TRX) == I2CxSR_TRX);
    }
    return (0);
#else
    return ((p_obj->p_instance->SR & I2CxSR_TRX) == I2CxSR_TRX);
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Interrupt Status
  * @param     p_obj       :I2C object.
  * @retval    true        :Interruput Occured.
  * @retval    false       :No Interruput Occured.
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t I2C_int_status(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        return ((p_obj->p_instance->ST & I2CxST_I2C) == I2CxST_I2C);
    }
    return (0);
#else
    return ((p_obj->p_instance->ST & I2CxST_I2C) == I2CxST_I2C);
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Interrupt Status Clear
  * @param     p_obj       :I2C object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void I2C_clear_int_status(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        p_obj->p_instance->ST = I2CxST_CLEAR;
    }
#else
    p_obj->p_instance->ST = I2CxST_CLEAR;
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Enable Interrupt setting.
  * @param     p_obj       :I2C object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void I2C_enable_interrupt(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        p_obj->p_instance->IE = I2CxIE_I2C;
    }
#else
    p_obj->p_instance->IE = I2CxIE_I2C;
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Enable Interrupt setting.
  * @param     p_obj       :I2C object.
  * @param     tx          :Direction of transfer(1=tx 0=rx).
  * @retval    -
  * @note      For DMA transfer.
  */
/*--------------------------------------------------*/
__STATIC_INLINE void I2C_enable_interrupt_dma(I2C_t *p_obj, int32_t tx)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        if (tx)
        {
            p_obj->p_instance->IE = (I2CxIE_SELPINCD | I2CxIE_DMARI2CTX);
        }
        else
        {
            p_obj->p_instance->IE = (I2CxIE_SELPINCD | I2CxIE_DMARI2CRX);
        }
    }
#else
    if (tx)
    {
        p_obj->p_instance->IE = (I2CxIE_SELPINCD | I2CxIE_DMARI2CTX);
    }
    else
    {
        p_obj->p_instance->IE = (I2CxIE_SELPINCD | I2CxIE_DMARI2CRX);
    }
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Disable Interrupt setting.
  * @param     p_obj       :I2C object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void I2C_disable_interrupt(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        p_obj->p_instance->IE = I2CxIE_CLEAR;
    }
#else
    p_obj->p_instance->IE = I2CxIE_CLEAR;
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Set slave address.
  * @param     p_obj       :I2C object.
  * @param     addr        :slave address.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE void I2C_set_address(I2C_t *p_obj, uint32_t addr)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        p_obj->p_instance->AR  = (addr & ~I2CxAR_ALS);
        p_obj->p_instance->AR2 = I2CxAR2_INIT;
    }
#else
    p_obj->p_instance->AR  = (addr & ~I2CxAR_ALS);
    p_obj->p_instance->AR2 = I2CxAR2_INIT;
#endif
}

/*--------------------------------------------------*/
/**
  * @brief     Detecting Slave Address
  * @param     p_obj       :I2C object.
  * @retval    -
  * @note      -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t I2C_slave_detected(I2C_t *p_obj)
{
#ifdef DEBUG
    if ((p_obj != I2C_NULL) && (p_obj->p_instance != I2C_NULL))
    {
        return (((p_obj->p_instance->SR & I2CxSR_AAS) == I2CxSR_AAS) 
            && ((p_obj->p_instance->OP & I2CxOP_SAST) == I2CxOP_SAST));
    }
    return (0);
#else
    return (((p_obj->p_instance->SR & I2CxSR_AAS) == I2CxSR_AAS) 
        && ((p_obj->p_instance->OP & I2CxOP_SAST) == I2CxOP_SAST));
#endif
}

/**
 *  @}
 */ /* End of group UTILITIES_Private_functions */

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Private_functions
 *  @{
 */
void I2C_init(I2C_t *p_obj);
void I2C_start_condition(I2C_t *p_obj, uint32_t data);
uint32_t I2C_get_clock_setting(I2C_t *p_obj, uint32_t frequency, uint32_t fsys, I2C_clock_setting_t *p_setting);
void I2C_slave_init(I2C_t *p_obj);
#if defined(I2CSxWUP_EN)
void I2CS_init(I2CS_t *p_obj);
void I2CS_Primary_slave_adr_set(I2CS_t *p_obj, uint32_t adr);
void I2CS_Secondary_slave_adr_set(I2CS_t *p_obj, uint32_t adr);
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

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __I2C_H */


