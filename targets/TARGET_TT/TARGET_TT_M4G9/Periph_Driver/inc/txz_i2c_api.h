/** 
 *******************************************************************************
 * @file    bsp_i2c.h
 * @brief   This file provides all the functions prototypes for I2C Driver.
 * @version V1.0.0.2
 * $Date:: 2017-10-03 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_i2c.h"

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
 *  @addtogroup UTILITIES_Exported_macro
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group UTILITIES_Exported_macro */


/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Exported_define
 *  @{
 */
/** 
 *  @defgroup I2C_NullPointer  Null Pointer
 *  @brief    I2C NULL Pointer.
 *  @{
 */
#define I2C_NULL        ((void *)0)
/**
 *  @}
 */ /* End of group I2C_NullPointer */

/** 
 *  @}
 */ /* End of group UTILITIES_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Exported_define
 *  @{
 */

/**
 * @defgroup I2C_ACK I2C ACK Macros
 * @brief    I2C Type of Acknowledge.
 * @{
 */
#define I2C_NACK                    (0)         /*!< NACK Received.             */
#define I2C_ACK                     (1)         /*!< ACK Received.              */
/**
 *  @}
 */ /* End of group I2C_ACK */


/**
 * @defgroup I2C_ERROR I2C ERROR Macros
 * @brief    I2C Error definitions.
 * @{
 */
#define I2C_ERROR_NO_ERROR          (0)         /*!< No Error.                          */
#if 0
#define I2C_ERROR_NO_SLAVE          (-1)        /*!< No Slave Error.                    */
#define I2C_ERROR_BUS_BUSY          (-2)        /*!< Bus Busy Error.(now, not support)  */
#endif
#define I2C_ERROR_PARAM             (-3)        /*!< Parameter Error.                   */
#define I2C_ERROR_OTHERS            (-4)        /*!< Others Error.                      */
#define I2C_ERROR_ARBITRATION       (-5)        /*!< Arbitration Error.                 */
/**
 *  @}
 */ /* End of group I2C_ERROR */


/**
 * @defgroup I2C_Events I2C Events Macros
 * @brief    I2C Asynch Events.
 * @{
 */
#define I2C_EVENT_ERROR               (1 << 1)  /*!< Error.                     */
#define I2C_EVENT_ERROR_NO_SLAVE      (1 << 2)  /*!< No Slave.                  */
#define I2C_EVENT_TRANSFER_COMPLETE   (1 << 3)  /*!< Transfer Complete.         */
#define I2C_EVENT_TRANSFER_EARLY_NACK (1 << 4)  /*!< End of Transfer.           */
#define I2C_EVENT_ALL                 (I2C_EVENT_ERROR |  I2C_EVENT_TRANSFER_COMPLETE | I2C_EVENT_ERROR_NO_SLAVE | I2C_EVENT_TRANSFER_EARLY_NACK)
/**
 *  @}
 */ /* End of group I2C_Events */

/**
 * @defgroup I2C_SlaveReceive I2C Slave Receive Return Macros
 * @brief    I2C Received Contents of Slave.
 * @{
 */
#define I2C_NO_DATA             (0)     /*!< the slave has not been addressed. */
#define I2C_READ_ADDRESSED      (1)     /*!< the master has requested a read from this slave. */
#define I2C_WRITE_GENERAL       (2)     /*!< the master is writing to all slave.(now, not support) */
#define I2C_WRITE_ADDRESSED     (3)     /*!< the master is writing to this slave. */
/**
 *  @}
 */ /* End of group I2C_SlaveReceive */

/** 
 *  @}
 */ /* End of group UTILITIES_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Exported_define
 *  @{
 */

/*----------------------------------*/
/** 
 * @brief  i2c Port Enumerated Type Definition.
*/
/*----------------------------------*/
typedef enum {
  I2C_PORT_PG2 = 0,     /*!< 0: PG2 I2C0 */
  I2C_PORT_PG3,         /*!< 1: PG3 I2C0 */
  I2C_PORT_PF2,         /*!< 2: PF2 I2C1 */
  I2C_PORT_PF3,         /*!< 3: PF3 I2C1 */
  I2C_PORT_PG4,         /*!< 4: PG4 I2C2 */
  I2C_PORT_PG5,         /*!< 5: PG5 I2C2 */
  I2C_PORT_PJ6,         /*!< 6: PJ6 I2C3 */
  I2C_PORT_PJ7,         /*!< 7: PJ7 I2C3 */
  I2C_PORT_PJ2,         /*!< 8: PJ2 I2C4 */
  I2C_PORT_PJ3,         /*!< 9: PJ3 I2C4 */}
i2c_port_t;

/*----------------------*/
/*  I2C Setting         */
/*----------------------*/
/* #define I2C_CHANNEL0 */
#define I2C_CHANNEL3
/* #define I2C_CHANNEL2 */
/* #define I2C_CHANNEL3 */
/* #define I2C_CHANNEL4 */
#if defined(I2C_CHANNEL0)
 #define I2Cx_TEXT          "I2C0"
 #define I2C_CFG_PORT_SCL     (I2C_PORT_PG3)        /*!<  SCL Port.                  */
 #define I2C_CFG_PORT_SDA     (I2C_PORT_PG2)        /*!<  SDA Port.                  */
#elif defined(I2C_CHANNEL1)
 #define I2Cx_TEXT          "I2C1"
 #define I2C_CFG_PORT_SCL     (I2C_PORT_PF3)        /*!<  SCL Port.                  */
 #define I2C_CFG_PORT_SDA     (I2C_PORT_PF2)        /*!<  SDA Port.                  */
#elif defined(I2C_CHANNEL2)
 #define I2Cx_TEXT          "I2C2"
 #define I2C_CFG_PORT_SCL     (I2C_PORT_PG5)        /*!<  SCL Port.                  */
 #define I2C_CFG_PORT_SDA     (I2C_PORT_PG4)        /*!<  SDA Port.                  */
#elif defined(I2C_CHANNEL3)
 #define I2Cx_TEXT          "I2C3"
 #define I2C_CFG_PORT_SCL     (I2C_PORT_PJ7)        /*!<  SCL Port.                  */
 #define I2C_CFG_PORT_SDA     (I2C_PORT_PJ6)        /*!<  SDA Port.                  */
#elif defined(I2C_CHANNEL4)
 #define I2Cx_TEXT          "I2C4"
 #define I2C_CFG_PORT_SCL     (I2C_PORT_PJ3)        /*!<  SCL Port.                  */
 #define I2C_CFG_PORT_SDA     (I2C_PORT_PJ2)        /*!<  SDA Port.                  */ 
#else
 #error "target channel is non-select."
#endif

/** 
 *  @}
 */ /* End of group UTILITIES_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Exported_typedef
 *  @{
 */
/*----------------------------------*/
/** 
 * @brief  I2C internal information structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint8_t         bus_free;       /*!< Bus free information.          */
    uint8_t         start;          /*!< Start condition information.   */
    uint32_t        irqn;           /*!< IRQ number table pointer.      */
    struct
    {
        uint32_t    address;        /*!< Slave address.                 */
        uint32_t    stop;           /*!< Stop control                   */
        uint32_t    event;          /*!< I2C Event information.         */
        uint32_t    state;          /*!< Transfer State.                */
    } asynch;
} i2c_internal_info_t;

/*----------------------------------*/
/** 
 * @brief  I2C buffer structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint8_t     *p_buffer;          /*!< Buffer address.      */
    uint32_t    length;             /*!< Buffer length.       */
    uint32_t    pos;                /*!< Buffer pointer.      */
} i2c_buffer_t;

/*----------------------------------*/
/** 
 * @brief  I2C handle structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    I2C_t                 i2c;      /*!< I2C class structure.   */
    i2c_internal_info_t   info;     /*!< Internal Information.  */
    i2c_buffer_t          tx_buff;  /*!< Tx buffer structure.   */
    i2c_buffer_t          rx_buff;  /*!< Rx buffer structure.   */
} _i2c_t;

/**
 *  @}
 */ /* End of group UTILITIES_Exported_typedef */


/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UTILITIES_Exported_functions
 *  @{
 */
TXZ_Result i2c_init_t(_i2c_t *p_obj, i2c_port_t sda, i2c_port_t scl);
TXZ_Result i2c_frequency_t(_i2c_t *p_obj, int32_t hz);
void i2c_reset_t(_i2c_t *p_obj);
TXZ_Result i2c_check_bus_free_t(_i2c_t *p_obj);
TXZ_Result i2c_start_t(_i2c_t *p_obj);
TXZ_Result i2c_stop_t(_i2c_t *p_obj);
int32_t i2c_read_t(_i2c_t *p_obj, int32_t address, uint8_t *p_data, int32_t length, int32_t stop);
int32_t i2c_write_t(_i2c_t *p_obj, int32_t address, uint8_t *p_data, int32_t length, int32_t stop);
int32_t i2c_byte_read_t(_i2c_t *p_obj, int32_t last);
int32_t i2c_byte_write_t(_i2c_t *p_obj, int32_t data);
uint8_t i2c_active_t(_i2c_t *p_obj);
TXZ_Result i2c_transfer_asynch_t(_i2c_t *p_obj, uint8_t *p_tx, int32_t tx_length, uint8_t *p_rx, int32_t rx_length, int32_t address, int32_t stop);
uint32_t i2c_irq_handler_asynch_t(_i2c_t *p_obj);
void i2c_abort_asynch_t(_i2c_t *p_obj);

/* For slave */
void i2c_slave_mode_t(_i2c_t *p_obj, int32_t enable_slave);
int32_t i2c_slave_receive_t(_i2c_t *p_obj);
int32_t i2c_slave_read_t(_i2c_t *p_obj, uint8_t *p_data, int32_t length);
int32_t i2c_slave_write_t(_i2c_t *p_obj, uint8_t *p_data, int32_t length);
void i2c_slave_address_t(_i2c_t *p_obj, uint32_t address);
TXZ_Result i2c_slave_transfer_asynch_t(_i2c_t *p_obj, uint8_t *p_tx, int32_t tx_length, uint8_t *p_rx, int32_t rx_length);
uint32_t i2c_slave_irq_handler_asynch_t(_i2c_t *p_obj);
void i2c_slave_abort_asynch_t(_i2c_t *p_obj);

/** 
 *  @}
 */ /* End of group UTILITIES_Exported_functions */

/**
 *  @}
 */ /* End of group UTILITIES */

/**
 *  @} 
 */ /* End of group Example */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __BSP_I2C_H */


