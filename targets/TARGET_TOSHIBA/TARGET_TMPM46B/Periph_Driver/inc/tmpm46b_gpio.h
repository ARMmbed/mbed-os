/**
 *******************************************************************************
 * @file    tmpm46b_gpio.h
 * @brief   This file provides all the functions prototypes for GPIO driver. 
 * @version V2.0.2.1
 * @date    2015/02/09
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_GPIO_H
#define __TMPM46B_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"

/** @addtogroup TX04_Periph_Driver
  * @{
  */
/** @addtogroup GPIO
  * @{
  */
/** @addtogroup  GPIO_Parameter_Definition
  * @{
  */

/** @brief :The maximum number of the Function Register
  * Note for porting:
  * If function register 6 is the maximum number in 
  * all the GPIO port,then define FRMAX (6U)
  */
#define FRMAX  (6U)             /* the max number of Port I/O function register is 6 */

/** @brief: define for function register
  * Note for porting:
  * If the maximum number of the function Register is 6,
  * then you need to define 6 GPIO_FUNC_REG_x ,
  * the value should be increased from 0 to 5
  */
#define GPIO_FUNC_REG_1                 ((uint8_t)0x00)
#define GPIO_FUNC_REG_2                 ((uint8_t)0x01)
#define GPIO_FUNC_REG_3                 ((uint8_t)0x02)
#define GPIO_FUNC_REG_4                 ((uint8_t)0x03)
#define GPIO_FUNC_REG_5                 ((uint8_t)0x04)
#define GPIO_FUNC_REG_6                 ((uint8_t)0x05)

/** @brief :The GPIO_Port enum
  * Note for porting:
  * the port value order from low to high with '1' step
  * and begin with "0".
  */
    typedef enum {
        GPIO_PA = 0U,
        GPIO_PB = 1U,
        GPIO_PC = 2U,
        GPIO_PD = 3U,
        GPIO_PE = 4U,
        GPIO_PF = 5U,
        GPIO_PG = 6U,
        GPIO_PH = 7U,
        GPIO_PJ = 8U,
        GPIO_PK = 9U,
        GPIO_PL = 10U
    } GPIO_Port;

#define IS_GPIO_PORT(param)             ((param) <= GPIO_PL)    /* parameter checking for port number */

#define RESER  (8U-(FRMAX))

    typedef struct {
        __IO uint32_t DATA;
        __IO uint32_t CR;
        __IO uint32_t FR[FRMAX];
        uint32_t RESERVED0[RESER];
        __IO uint32_t OD;
        __IO uint32_t PUP;
        __IO uint32_t PDN;
        uint32_t RESERVED1;
        __IO uint32_t IE;
    } TSB_Port_TypeDef;

    typedef struct {
        uint8_t PinDATA;
        uint8_t PinCR;
        uint8_t PinFR[FRMAX];
        uint8_t PinOD;
        uint8_t PinPUP;
        uint8_t PinPDN;
        uint8_t PinIE;
    } GPIO_RegTypeDef;

    typedef struct {
        uint8_t IOMode;        /* Set the port input or output mode     */
        uint8_t PullUp;        /* Enable or disable Pull-up function    */
        uint8_t OpenDrain;     /* Enable or disable open drain function */
        uint8_t PullDown;      /* Enable or disable Pull-down function  */
    } GPIO_InitTypeDef;

#define GPIO_INPUT_MODE                 ((uint8_t)0x00)
#define GPIO_OUTPUT_MODE                ((uint8_t)0x01)
#define GPIO_IO_MODE_NONE               ((uint8_t)0x02)
#define IS_GPIO_IO_MODE_STATE(param)    (((param) == GPIO_INPUT_MODE) || \
                                         ((param) == GPIO_OUTPUT_MODE) || \
                                         ((param) == GPIO_IO_MODE_NONE))

#define GPIO_PULLUP_DISABLE             ((uint8_t)0x00)
#define GPIO_PULLUP_ENABLE              ((uint8_t)0x01)
#define GPIO_PULLUP_NONE                ((uint8_t)0x02)
#define IS_GPIO_PULLUP_STATE(param)     (((param) == GPIO_PULLUP_ENABLE) || \
                                         ((param) == GPIO_PULLUP_DISABLE) || \
                                         ((param) == GPIO_PULLUP_NONE))

#define GPIO_PULLDOWN_DISABLE           ((uint8_t)0x00)
#define GPIO_PULLDOWN_ENABLE            ((uint8_t)0x01)
#define GPIO_PULLDOWN_NONE              ((uint8_t)0x02)
#define IS_GPIO_PULLDOWN_STATE(param)   (((param) == GPIO_PULLDOWN_ENABLE) || \
                                         ((param) == GPIO_PULLDOWN_DISABLE) || \
                                         ((param) == GPIO_PULLDOWN_NONE))

#define GPIO_OPEN_DRAIN_DISABLE         ((uint8_t)0x00)
#define GPIO_OPEN_DRAIN_ENABLE          ((uint8_t)0x01)
#define GPIO_OPEN_DRAIN_NONE            ((uint8_t)0x02)
#define IS_GPIO_OPEN_DRAIN_STATE(param) (((param) == GPIO_OPEN_DRAIN_ENABLE) || \
                                         ((param) == GPIO_OPEN_DRAIN_DISABLE) || \
                                         ((param) == GPIO_OPEN_DRAIN_NONE))

#define GPIO_BIT_VALUE_1                ((uint8_t)0x01)
#define GPIO_BIT_VALUE_0                ((uint8_t)0x00)

#define IS_GPIO_BIT_VALUE(BitValue)     (((BitValue) == GPIO_BIT_VALUE_1)|| \
                                         ((BitValue) == GPIO_BIT_VALUE_0))

#define GPIO_BIT_0                      ((uint8_t)0x01)
#define GPIO_BIT_1                      ((uint8_t)0x02)
#define GPIO_BIT_2                      ((uint8_t)0x04)
#define GPIO_BIT_3                      ((uint8_t)0x08)
#define GPIO_BIT_4                      ((uint8_t)0x10)
#define GPIO_BIT_5                      ((uint8_t)0x20)
#define GPIO_BIT_6                      ((uint8_t)0x40)
#define GPIO_BIT_7                      ((uint8_t)0x80)
#define GPIO_BIT_ALL                    ((uint8_t)0xFF)

#define IS_GPIO_WRITE(GPIO_x)                       (GPIO_SFRs[(GPIO_x)].PinCR)

#define IS_GPIO_BIT_DATA(GPIO_x,Bit_x)              ((((GPIO_SFRs[(GPIO_x)].PinDATA) & (Bit_x))&&\
                                                    (!((uint8_t)(~(GPIO_SFRs[(GPIO_x)].PinDATA))&(Bit_x)))))

#define IS_GPIO_BIT_OUT(GPIO_x,Bit_x)               (((GPIO_SFRs[(GPIO_x)].PinCR &(Bit_x))&&\
                                                    (!((uint8_t)(~GPIO_SFRs[(GPIO_x)].PinCR)&(Bit_x)))))

#define IS_GPIO_BIT_IN(GPIO_x,Bit_x)                (((GPIO_SFRs[(GPIO_x)].PinIE &(Bit_x))&&\
                                                    (!((uint8_t)(~GPIO_SFRs[(GPIO_x)].PinIE)&(Bit_x)))))

#define IS_GPIO_BIT_PUP(GPIO_x,Bit_x)               (((GPIO_SFRs[(GPIO_x)].PinPUP &(Bit_x))&&\
                                                    (!((uint8_t)(~GPIO_SFRs[(GPIO_x)].PinPUP)&(Bit_x)))))

#define IS_GPIO_BIT_PDN(GPIO_x,Bit_x)               (((GPIO_SFRs[(GPIO_x)].PinPDN &(Bit_x))&&\
                                                    (!((uint8_t)(~GPIO_SFRs[(GPIO_x)].PinPDN)&(Bit_x)))))

#define IS_GPIO_BIT_OD(GPIO_x,Bit_x)                (((GPIO_SFRs[(GPIO_x)].PinOD &(Bit_x))&&\
                                                    (!((uint8_t)(~GPIO_SFRs[(GPIO_x)].PinOD)&(Bit_x)))))

#define IS_GPIO_BIT_FR(GPIO_x,FuncReg_x,Bit_x)      (((GPIO_SFRs[(GPIO_x)].PinFR[(FuncReg_x)]&(Bit_x))&&\
                                                    (!((uint8_t)(~GPIO_SFRs[(GPIO_x)].PinFR[(FuncReg_x)])&(Bit_x)))))

#define IS_GPIO_FUNCTION_REG(param)     ((param) < (FRMAX))

#define IS_GPIO_BIT(param)              (((param) == GPIO_BIT_0)|| \
                                         ((param) == GPIO_BIT_1)|| \
                                         ((param) == GPIO_BIT_2)|| \
                                         ((param) == GPIO_BIT_3)|| \
                                         ((param) == GPIO_BIT_4)|| \
                                         ((param) == GPIO_BIT_5)|| \
                                         ((param) == GPIO_BIT_6)|| \
                                         ((param) == GPIO_BIT_7))
/** @} */
/* End of group GPIO_Bit_Define */

/** @defgroup GPIO_Exported_FunctionPrototypes
  * @{
  */

    uint8_t GPIO_ReadData(GPIO_Port GPIO_x);
    uint8_t GPIO_ReadDataBit(GPIO_Port GPIO_x, uint8_t Bit_x);
    void GPIO_WriteData(GPIO_Port GPIO_x, uint8_t Data);
    void GPIO_WriteDataBit(GPIO_Port GPIO_x, uint8_t Bit_x, uint8_t BitValue);
    void GPIO_Init(GPIO_Port GPIO_x, uint8_t Bit_x, GPIO_InitTypeDef * GPIO_InitStruct);
    void GPIO_SetOutput(GPIO_Port GPIO_x, uint8_t Bit_x);
    void GPIO_SetInput(GPIO_Port GPIO_x, uint8_t Bit_x);
    void GPIO_SetInputEnableReg(GPIO_Port GPIO_x, uint8_t Bit_x, FunctionalState NewState);
    void GPIO_SetOutputEnableReg(GPIO_Port GPIO_x, uint8_t Bit_x, FunctionalState NewState);
    void GPIO_SetPullUp(GPIO_Port GPIO_x, uint8_t Bit_x, FunctionalState NewState);
    void GPIO_SetPullDown(GPIO_Port GPIO_x, uint8_t Bit_x, FunctionalState NewState);
    void GPIO_SetOpenDrain(GPIO_Port GPIO_x, uint8_t Bit_x, FunctionalState NewState);
    void GPIO_EnableFuncReg(GPIO_Port GPIO_x, uint8_t FuncReg_x, uint8_t Bit_x);
    void GPIO_DisableFuncReg(GPIO_Port GPIO_x, uint8_t FuncReg_x, uint8_t Bit_x);

/** @} */
/* End of group GPIO_Exported_FunctionPrototypes */
/** @} */
/* End of group GPIO */
/** @} */
/* End of group TX04_Periph_Driver */

#ifdef __cplusplus
}
#endif                          
#endif                          /* __TMPM46B_GPIO_H */
