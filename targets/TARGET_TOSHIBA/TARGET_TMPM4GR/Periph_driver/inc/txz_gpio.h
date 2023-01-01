/**
 *******************************************************************************
 * @file    txz_gpio.h
 * @brief   This file provides all the functions prototypes for GPIO driver.
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
/** @defgroup Device_Included Device Included
  * @{
  */
#include "txz_gpio_M4GR.h"

/**
  * @}
  */ /* End of group Device_Included */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup GPIO_Exported_define GPIO Exported Define
 *  @{
 */
#define CHECK_MASK(mode, mask) ((1 << ((mode)/4)) & (mask))
#define WRITE_PORT(mode, addr, val, mask) \
    do { \
        if(CHECK_MASK(mode, mask)) *((uint32_t*)(addr) + (mode)/4) = (val); \
    }while(0)
#define READ_PORT(mode, addr, val, mask) \
    do { \
        if(CHECK_MASK(mode, mask)) (*(val)) = (*((uint32_t*)(addr) + (mode)/4)); \
    }while(0)

/**
 *  @defgroup GPIO_Result  Result
 *  @brief    GPIO Result Macro Definition.
 *  @{
 */
#define GPIO_RESULT_SUCCESS     (0)          /*!< Success       */
#define GPIO_RESULT_FAILURE     (-1)         /*!< Failure       */
#define GPIO_READ_FAILURE       (0xFFFFFFFF) /*!< Failure       */
/**
 *  @}
 */ /* End of group GPIO_Result */

/**
 *  @}
 */ /* End of group GPIO_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** @defgroup GPIO_Exported_Typedef GPIO Exported Typedef
  * @{
  */

/**
 *  @enum gpio_pinstate_t
 *  @brief    Pin State Reset/Set Enumerated Type Definition.
 */
typedef enum {
    GPIO_PIN_RESET = 0,  /*!< 0: Clear       */
    GPIO_PIN_SET,        /*!< 1: Set         */
} gpio_pinstate_t;

/**
 *  @enum gpio_pininout_t
 *  @brief    Pin Input/Output Enumerated Type Definition.
 */
typedef enum {
    GPIO_PIN_INPUT = 0,  /*!< 0: Input       */
    GPIO_PIN_OUTPUT,     /*!< 1: Output       */
    GPIO_PIN_INOUT,      /*!< 2: Input/Output */
    GPIO_PIN_NOTINOUT,   /*!< 3: Not Input/Output */
} gpio_pininout_t;

/**
 *  @enum gpio_mode_t
 *  @brief    Port Mode Enumerated Type Definition.
 */
typedef enum {
    GPIO_Mode_DATA = 0x0,  /*!< 0x0: PxDATA       */
    GPIO_Mode_CR  = 0x4,   /*!< 0x4: PxCR         */
    GPIO_Mode_FR1 = 0x8,   /*!< 0x8: PxFR1        */
    GPIO_Mode_FR2 = 0xC,   /*!< 0xC: PxFR2        */
    GPIO_Mode_FR3 = 0x10,  /*!< 0x10: PxFR3        */
    GPIO_Mode_FR4 = 0x14,  /*!< 0x14: PxFR4        */
    GPIO_Mode_FR5 = 0x18,  /*!< 0x18: PxFR5        */
    GPIO_Mode_FR6 = 0x1C,  /*!< 0x1C: PxFR6        */
    GPIO_Mode_FR7 = 0x20,  /*!< 0x20: PxFR7        */
    GPIO_Mode_FR8 = 0x24,  /*!< 0x24: PxFR8        */
    GPIO_Mode_OD  = 0x28,  /*!< 0x28: PxOD         */
    GPIO_Mode_PUP = 0x2C,  /*!< 0x2C: PxPUP        */
    GPIO_Mode_PDN = 0x30,  /*!< 0x30: PxPDN        */
    GPIO_Mode_IE  = 0x38   /*!< 0x38: PxIE         */
} gpio_mode_t;

/**
 *  @enum gpio_mode_num
 *  @brief    Port Mode Enumerated Num(bit num).
 */
typedef enum {
    GPIO_ModeNum_DATA = 0,  /*!< 0: PxDATA       */
    GPIO_ModeNum_CR,        /*!< 1: PxCR         */
    GPIO_ModeNum_FR1,       /*!< 2: PxFR1        */
    GPIO_ModeNum_FR2,       /*!< 3: PxFR2        */
    GPIO_ModeNum_FR3,       /*!< 4: PxFR3        */
    GPIO_ModeNum_FR4,       /*!< 5: PxFR4        */
    GPIO_ModeNum_FR5,       /*!< 6: PxFR5        */
    GPIO_ModeNum_FR6,       /*!< 7: PxFR6        */
    GPIO_ModeNum_FR7,       /*!< 8: PxFR7        */
    GPIO_ModeNum_FR8,       /*!< 9: PxFR8        */
    GPIO_ModeNum_OD,        /*!< 10: PxOD        */
    GPIO_ModeNum_PUP,       /*!< 11: PxPUP       */
    GPIO_ModeNum_PDN,       /*!< 12: PxPDN       */
    GPIO_ModeNum_IE,        /*!< 13: PxIE        */
    GPIO_ModeNum_Max,       /*!< 14: Max         */
} gpio_mode_num;
/**
 *  @}
 */ /* End of group GPIO_Exported_Typedef */
/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup GPIO_Exported_Typedef GPIO Exported Typedef
 *  @{
 */
/**
 *  @}
 */ /* End of group GPIO_Exported_Typedef */
/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup GPIO_Exported_functions GPIO Exported Functions
 *  @{
 */
TXZ_Result _gpio_init(_gpio_t *p_obj, uint32_t group);
TXZ_Result gpio_deinit(_gpio_t *p_obj, uint32_t group);
TXZ_Result gpio_write_mode(_gpio_t *p_obj, uint32_t group, uint32_t mode, uint32_t val);
TXZ_Result gpio_read_mode(_gpio_t *p_obj, uint32_t group, uint32_t mode, uint32_t *val);
TXZ_Result gpio_func(_gpio_t *p_obj, gpio_gr_t group, gpio_num_t num, uint32_t func, gpio_pininout_t inout);
TXZ_Result gpio_SetPullUp(_gpio_t *p_obj, gpio_gr_t group, gpio_num_t num, gpio_pinstate_t val);
TXZ_Result gpio_SetPullDown(_gpio_t *p_obj, gpio_gr_t group, gpio_num_t num, gpio_pinstate_t val);
TXZ_Result gpio_SetOpenDrain(_gpio_t *p_obj, gpio_gr_t group, gpio_num_t num, gpio_pinstate_t val);
TXZ_Result gpio_write_bit(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode, uint32_t val);
TXZ_Result gpio_read_bit(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode, gpio_pinstate_t *pinstate);

/**
 *  @}
 */ /* End of group GPIO_Exported_functions */

/**
 *  @}
 */ /* End of group GPIO */

#ifdef __cplusplus
}
#endif
#endif /* __GPIO_H */
