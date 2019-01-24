/**
  ******************************************************************************
  * @file    hal_types.h
  * @author  AMS - VMA RF Application team
  * @version V1.0.1
  * @date    3-April-2018
  * @brief   Header file that includes hal types for BlueNRG-1 SoC
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
  */
#ifndef __HAL_TYPES_H__
#define __HAL_TYPES_H__

#include <stdint.h>

/**@brief RESET,SET definition */
typedef enum {RESET = 0, SET} FlagStatus, ITStatus;
/**@brief Macro that checks if STATE is a FlagStatus / ITStatus */
#define IS_FLAGSTATUS(STATE) (((STATE) == RESET) || ((STATE) == SET))
#define IS_ITSTATUS(STATE)   IS_FLAGSTATUS(STATE)

/**@brief DISABLE, ENABLE  definition */
typedef enum {DISABLE = 0, ENABLE} FunctionalState;
/** @brief Macro that checks if STATE is a FunctionalState */
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {SUCCESS = 0, ERROR} ErrorStatus;
/** @brief Macro that checks if STATE is a ErrorStatus */
#define IS_ERROR_STATE(STATE) (((STATE) == SUCCESS) || ((STATE) == ERROR))


/** @brief Macro that stores Value into a buffer in Little Endian Format (2 bytes)*/
#define HOST_TO_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )

/** @brief Macro that stores  Little Endian Format into a buffer value */
#define LE_TO_HOST_32(ptr)   (uint32_t) ( ((uint32_t) \
                                           *((uint8_t *)ptr)) | \
                                           ((uint32_t) \
                                            *((uint8_t *)ptr + 1) << 8)  | \
                                           ((uint32_t) \
                                            *((uint8_t *)ptr + 2) << 16) | \
                                           ((uint32_t) \
                                            *((uint8_t *)ptr + 3) << 24) )

/** @brief Macro that stores Value into a buffer in Little Endian Format (4 bytes) */
#define HOST_TO_LE_32(buf, val)    ( ((buf)[0] =  (uint8_t) (val)     ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8)  ) , \
                                   ((buf)[2] =  (uint8_t) (val>>16) ) , \
                                   ((buf)[3] =  (uint8_t) (val>>24) ) ) 

/** @brief Booelan definition */
typedef uint8_t BOOL;

/**@brief TRUE, FALSE definition */
//#define TRUE  ((BOOL)1U)
//#define FALSE ((BOOL)0U)

#define TRUE  1 //antonio
#define FALSE 0 //antonio

#endif /* __HAL_TYPES_H__ */
