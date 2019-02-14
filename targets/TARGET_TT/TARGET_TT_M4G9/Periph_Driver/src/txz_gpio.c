/**
 *******************************************************************************
 * @file    txz_gpio.c
 * @brief   This file provides API functions for GPIO driver.
 * @version V1.0.0.2
 * $Date:: 2018-04-11 14:13:48 #$
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
#include "txz_gpio.h"
     
#if defined(__GPIO_H)
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @addtogroup GPIO
 *  @{
 */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup GPIO_Private_define GPIO Private Define
 *  @{
 */
/** 
 *  @name  Parameter Result
 *  @brief Whether the parameter is specified or not.
 *  @{
 */
#define PARAM_OK    ((int32_t)1)    /*!< Parameter is valid(specified).         */
#define PARAM_NG    ((int32_t)0)    /*!< Parameter is invalid(not specified).   */
/**
 *  @}
 */ /* End of name Parameter Result */
 
/** 
 *  @name  Bit Operation Macro
 *  @brief Whether the parameter is specified or not.
 *  @{
 */
#define PORT_BASE (0x400E0000UL) /*!< Port Register Base Adress */
#define BITBAND_PORT_OFFSET (0x0000100UL) /*!< Port Register Offset Value */
#define BITBAND_PORT_BASE(gr) (PORT_BASE + (uint32_t)((BITBAND_PORT_OFFSET) * (uint32_t)(gr)) ) /*!<  Operational target Port Adress */
#define BITBAND_PORT_MODE_BASE(base, pinmode) ((uint32_t)(base) + (uint32_t)(pinmode) )  /*!< Operational target Control Register Adress */
#define BITBAND_PORT_SET(base, bitnum) (*((__IO uint32_t *)(base)) |= (uint32_t)(0x0000001UL<< (bitnum))) /*!< Target Pin Bit set */
#define BITBAND_PORT_CLR(base, bitnum) (*((__IO uint32_t *)(base)) &= ~((uint32_t)(0x0000001UL<< (bitnum)))) /*!< Target Pin Bit clear */
#define BITBAND_PORT_READ(val, base, bitnum) ((val) = ((*((__IO uint32_t *)(base)) & (uint32_t)(0x0000001UL<< (bitnum))) >> (bitnum))) /*!< Target Pin Bit read */
/**
 *  @}
 */ /* End of Bit Operation Macro */
/**
 *  @}
 */ /* End of group GPIO_Private_typedef */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup GPIO_Private_define GPIO Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group GPIO_Private_define */
/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup GPIO_Private_typedef GPIO Private Typedef
 *  @{
 */
/*!
  * @brief  Pin Exist Table
  * @details Bit0 :GPIO_Mode_DATA
  * @details Bit1 :GPIO_Mode_CR
  * @details Bit2 :GPIO_Mode_FR1
  * @details Bit3 :GPIO_Mode_FR2
  * @details Bit4 :GPIO_Mode_FR3
  * @details Bit5 :GPIO_Mode_FR4
  * @details Bit6 :GPIO_Mode_FR5
  * @details Bit7 :GPIO_Mode_FR6
  * @details Bit8 :GPIO_Mode_FR7
  * @details Bit9 :GPIO_Mode_OD
  * @details Bit10 :GPIO_Mode_PUP
  * @details Bit11 :GPIO_Mode_PDN
  * @details Bit12 :GPIO_Mode_IE
  */
#if defined(TMPM4G6)
static uint16_t PinExistTbl[GPIO_GROUP_Max][GPIO_PORT_Max] =
{
   /* Port-0  Port-1  Port-2  Port-3  Port-4  Port-5  Port-6  Port-7 */
    { 0x1FDF, 0x1F57, 0x1F17, 0x1FDF, 0x1FDF, 0x1FD7, 0x1F97, 0x1FDF },     /** GPIO_PORT_A */
    { 0x1E5F, 0x1EDF, 0x1E57, 0x1E17, 0x1E57, 0x1E17, 0x1E5F, 0x1E5F },     /** GPIO_PORT_B */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_C */
    { 0x1FFF, 0x1F7F, 0x1F77, 0x1F37, 0x1F57, 0x1F17, 0x1F5F, 0x1F5F },     /** GPIO_PORT_D */
    { 0x1F7F, 0x1E07, 0x1F77, 0x1F77, 0x1F77, 0x1F77, 0x1F77, 0x1F7F },     /** GPIO_PORT_E */
    { 0x1E07, 0x1E07, 0x1F03, 0x1F03, 0x0000, 0x0000, 0x1E07, 0x1E07 },     /** GPIO_PORT_F */
    { 0x1E57, 0x1E57, 0x1E03, 0x1F7B, 0x1F6B, 0x1F7B, 0x1E67, 0x1E67 },     /** GPIO_PORT_G */
    { 0x1E77, 0x1E77, 0x1E73, 0x1E73, 0x1E57, 0x1E47, 0x1E47, 0x1E47 },     /** GPIO_PORT_H */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_J */
    { 0x1F9F, 0x1E3F, 0x1E8F, 0x1E8F, 0x1EA7, 0x1EA7, 0x1EBF, 0x1DBB },     /** GPIO_PORT_K */
    { 0x1F9B, 0x1F03, 0x1F03, 0x1F9B, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_L */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_M */
    { 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03 },     /** GPIO_PORT_N */
    { 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B },     /** GPIO_PORT_P */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_R */
    { 0x1E03, 0x1E03, 0x1F03, 0x1EDF, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_T */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_U */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_V */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_W */
    { 0x1C01, 0x1C01, 0x1C01, 0x1C01, 0x1E27, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_Y */
};
#endif  /* TMPM4G6 */
#if defined(TMPM4G7)
static uint16_t PinExistTbl[GPIO_GROUP_Max][GPIO_PORT_Max] =
{
   /* Port-0  Port-1  Port-2  Port-3  Port-4  Port-5  Port-6  Port-7 */
    { 0x1FDF, 0x1F57, 0x1F17, 0x1FDF, 0x1FDF, 0x1FD7, 0x1F97, 0x1FDF },     /** GPIO_PORT_A */
    { 0x1E5F, 0x1EDF, 0x1E57, 0x1E17, 0x1E57, 0x1E17, 0x1E5F, 0x1E5F },     /** GPIO_PORT_B */
    { 0x1E57, 0x1E57, 0x1E57, 0x1E17, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_C */
    { 0x1FFF, 0x1F7F, 0x1F77, 0x1F37, 0x1F57, 0x1F17, 0x1F5F, 0x1F5F },     /** GPIO_PORT_D */
    { 0x1F7F, 0x1E07, 0x1F77, 0x1F77, 0x1F77, 0x1F77, 0x1F77, 0x1F7F },     /** GPIO_PORT_E */
    { 0x1E07, 0x1E07, 0x1F03, 0x1F03, 0x1E07, 0x1E07, 0x1E07, 0x1E07 },     /** GPIO_PORT_F */
    { 0x1E57, 0x1E57, 0x1E03, 0x1F7B, 0x1F6B, 0x1F7B, 0x1E67, 0x1E67 },     /** GPIO_PORT_G */
    { 0x1E77, 0x1E77, 0x1E73, 0x1E73, 0x1E57, 0x1E47, 0x1E47, 0x1E47 },     /** GPIO_PORT_H */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_J */
    { 0x1F9F, 0x1E3F, 0x1E8F, 0x1E8F, 0x1EA7, 0x1EA7, 0x1EBF, 0x1DBB },     /** GPIO_PORT_K */
    { 0x1F9B, 0x1F03, 0x1F03, 0x1F9B, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_L */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_M */
    { 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03 },     /** GPIO_PORT_N */
    { 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B },     /** GPIO_PORT_P */
    { 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_R */
    { 0x1E03, 0x1E03, 0x1F03, 0x1EDF, 0x1E23, 0x1E0B, 0x0000, 0x0000 },     /** GPIO_PORT_T */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_U */
    { 0x1FFB, 0x1FFB, 0x1FFB, 0x1FEB, 0x1FEB, 0x1FFB, 0x1F7B, 0x1FEB },     /** GPIO_PORT_V */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_W */
    { 0x1C01, 0x1C01, 0x1C01, 0x1C01, 0x1E27, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_Y */
};
#endif  /* TMPM4G7 */
#if defined(TMPM4G8)
static uint16_t PinExistTbl[GPIO_GROUP_Max][GPIO_PORT_Max] =
{
   /* Port-0  Port-1  Port-2  Port-3  Port-4  Port-5  Port-6  Port-7 */
    { 0x1FDF, 0x1F57, 0x1F17, 0x1FDF, 0x1FDF, 0x1FD7, 0x1F97, 0x1FDF },     /** GPIO_PORT_A */
    { 0x1E5F, 0x1EDF, 0x1E57, 0x1E17, 0x1E57, 0x1E17, 0x1E5F, 0x1E5F },     /** GPIO_PORT_B */
    { 0x1E57, 0x1E57, 0x1E57, 0x1E17, 0x1E57, 0x1E17, 0x1E07, 0x1E07 },     /** GPIO_PORT_C */
    { 0x1FFF, 0x1F7F, 0x1F77, 0x1F37, 0x1F57, 0x1F17, 0x1F5F, 0x1F5F },     /** GPIO_PORT_D */
    { 0x1F7F, 0x1E07, 0x1F77, 0x1F77, 0x1F77, 0x1F77, 0x1F77, 0x1F7F },     /** GPIO_PORT_E */
    { 0x1E07, 0x1E07, 0x1F03, 0x1F03, 0x1E07, 0x1E07, 0x1E07, 0x1E07 },     /** GPIO_PORT_F */
    { 0x1E57, 0x1E57, 0x1E03, 0x1F7B, 0x1F6B, 0x1F7B, 0x1E67, 0x1E67 },     /** GPIO_PORT_G */
    { 0x1E77, 0x1E77, 0x1E73, 0x1E73, 0x1E57, 0x1E47, 0x1E47, 0x1E47 },     /** GPIO_PORT_H */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_J */
    { 0x1F9F, 0x1E3F, 0x1E8F, 0x1E8F, 0x1EA7, 0x1EA7, 0x1EBF, 0x1DBB },     /** GPIO_PORT_K */
    { 0x1F9B, 0x1F03, 0x1F03, 0x1F9B, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_L */
    { 0x1FE3, 0x1FE3, 0x1FDB, 0x1FEB, 0x1FAB, 0x1F9B, 0x1FFB, 0x1FEB },     /** GPIO_PORT_M */
    { 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03 },     /** GPIO_PORT_N */
    { 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B },     /** GPIO_PORT_P */
    { 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B },     /** GPIO_PORT_R */
    { 0x1E03, 0x1E03, 0x1F03, 0x1EDF, 0x1E23, 0x1E0B, 0x0000, 0x0000 },     /** GPIO_PORT_T */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_U */
    { 0x1FFB, 0x1FFB, 0x1FFB, 0x1FEB, 0x1FEB, 0x1FFB, 0x1F7B, 0x1FEB },     /** GPIO_PORT_V */
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_W */
    { 0x1C01, 0x1C01, 0x1C01, 0x1C01, 0x1E27, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_Y */
};
#endif  /* TMPM4G8 */
#if defined(TMPM4G9)
static uint16_t PinExistTbl[GPIO_GROUP_Max][GPIO_PORT_Max] =
{
   /* Port-0  Port-1  Port-2  Port-3  Port-4  Port-5  Port-6  Port-7 */
    { 0x1FDF, 0x1F57, 0x1F17, 0x1FDF, 0x1FDF, 0x1FD7, 0x1F97, 0x1FDF },     /** GPIO_PORT_A */
    { 0x1E5F, 0x1EDF, 0x1E57, 0x1E17, 0x1E57, 0x1E17, 0x1E5F, 0x1E5F },     /** GPIO_PORT_B */
    { 0x1E57, 0x1E57, 0x1E57, 0x1E17, 0x1E57, 0x1E17, 0x1E07, 0x1E07 },     /** GPIO_PORT_C */
    { 0x1FFF, 0x1F7F, 0x1F77, 0x1F37, 0x1F57, 0x1F17, 0x1F5F, 0x1F5F },     /** GPIO_PORT_D */
    { 0x1F7F, 0x1E47, 0x1F77, 0x1F77, 0x1F77, 0x1F77, 0x1F77, 0x1F7F },     /** GPIO_PORT_E */
    { 0x1E07, 0x1E07, 0x1F03, 0x1F03, 0x1E07, 0x1E07, 0x1E07, 0x1E07 },     /** GPIO_PORT_F */
    { 0x1E57, 0x1E57, 0x1F03, 0x1F7B, 0x1F6B, 0x1F7B, 0x1E67, 0x1E67 },     /** GPIO_PORT_G */
    { 0x1E77, 0x1E77, 0x1E73, 0x1E73, 0x1E57, 0x1E47, 0x1E47, 0x1E47 },     /** GPIO_PORT_H */
    { 0x1E53, 0x1E53, 0x1F53, 0x1F53, 0x1E5B, 0x1E5B, 0x1F43, 0x1F43 },     /** GPIO_PORT_J */
    { 0x1F9F, 0x1E3F, 0x1E8F, 0x1E8F, 0x1EA7, 0x1EA7, 0x1EBF, 0x1DBB },     /** GPIO_PORT_K */
    { 0x1F9B, 0x1F03, 0x1F03, 0x1F9B, 0x1E1B, 0x1E0B, 0x1E1B, 0x1E0F },     /** GPIO_PORT_L */
    { 0x1FE3, 0x1FE3, 0x1FDB, 0x1FEB, 0x1FAB, 0x1F9B, 0x1FFB, 0x1FEB },     /** GPIO_PORT_M */
    { 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03, 0x1E03 },     /** GPIO_PORT_N */
    { 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B, 0x1E5B },     /** GPIO_PORT_P */
    { 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B, 0x1E1B },     /** GPIO_PORT_R */
    { 0x1E03, 0x1E03, 0x1F03, 0x1EDF, 0x1E23, 0x1E0B, 0x0000, 0x0000 },     /** GPIO_PORT_T */
    { 0x1F1B, 0x1F0B, 0x1F1B, 0x1F1B, 0x1F1B, 0x1F1B, 0x1F1B, 0x1F0B },     /** GPIO_PORT_U */
    { 0x1FFB, 0x1FFB, 0x1FFB, 0x1FEB, 0x1FEB, 0x1FFB, 0x1F7B, 0x1FEB },     /** GPIO_PORT_V */
    { 0x1EE3, 0x1F63, 0x1F63, 0x1E63, 0x1FD3, 0x1FC3, 0x1FC3, 0x1FD3 },     /** GPIO_PORT_W */
    { 0x1C01, 0x1C01, 0x1C01, 0x1C01, 0x1E27, 0x0000, 0x0000, 0x0000 },     /** GPIO_PORT_Y */
};
#endif  /* TMPM4G9 */

/**
 *  @}
 */ /* End of group GPIO_Private_typedef */
 
/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup GPIO_Private_fuctions GPIO Private Fuctions
 *  @{
 */

static uint8_t change_mode_to_num( uint32_t mode );
static uint8_t change_func_to_num( uint32_t mode );
static int32_t check_param_pin_exist(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode);
static int32_t check_param_func_pin_exist(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode);
/*--------------------------------------------------*/
/*!
  * @fn     static int32_t check_param_pin_exist(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode)
  * @brief  Check the Pin Exist.
  * @param[in] p_obj :GPIO object.
  * @param[in] group :GPIO Port Group.  : Use @ref gpio_gr_t
  * @param[in] num   :GPIO Port Number. : Use @ref gpio_num_t
  * @param[in] mode  :GPIO Port Mode. : Use @ref gpio_mode_t
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  */
/*--------------------------------------------------*/

static uint8_t change_mode_to_num( uint32_t mode )
{
    uint8_t retVal = 0;

    if (mode == GPIO_Mode_DATA) {retVal = 0;}
    else if (mode == GPIO_Mode_CR) {retVal = 1;}
    else if (mode == GPIO_Mode_FR1) {retVal = 2;}
    else if (mode == GPIO_Mode_FR2) {retVal = 3;}
    else if (mode == GPIO_Mode_FR3) {retVal = 4;}
    else if (mode == GPIO_Mode_FR4) {retVal = 5;}
    else if (mode == GPIO_Mode_FR5) {retVal = 6;}
    else if (mode == GPIO_Mode_FR6) {retVal = 7;}
    else if (mode == GPIO_Mode_FR7) {retVal = 8;}
    else if (mode == GPIO_Mode_OD) {retVal = 9;}
    else if (mode == GPIO_Mode_PUP) {retVal = 10;}
    else if (mode == GPIO_Mode_PDN) {retVal = 11;}
    else if (mode == GPIO_Mode_IE) {retVal = 12;}
    else {retVal = 13;}

    return retVal;
}

static uint8_t change_func_to_num( uint32_t mode )
{
    uint8_t retVal = 0;

    if  (mode == GPIO_FR_1) {retVal = 2;}
    else if (mode == GPIO_FR_2) {retVal = 3;}
    else if (mode == GPIO_FR_3) {retVal = 4;}
    else if (mode == GPIO_FR_4) {retVal = 5;}
    else if (mode == GPIO_FR_5) {retVal = 6;}
    else if (mode == GPIO_FR_6) {retVal = 7;}
    else if (mode == GPIO_FR_7) {retVal = 8;}
    else if (mode == GPIO_FR_NA) {retVal = 1;}
    else if (mode == 0) {retVal = 1;}
    else {retVal = 13;}

    return retVal;
}

static int32_t check_param_pin_exist(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode)
{
    int32_t result = PARAM_NG;
    uint8_t chgmode;
    uint16_t tmp;

    chgmode = change_mode_to_num(mode);
    if ((chgmode < 13) && (group < GPIO_GROUP_Max) && (num < GPIO_PORT_Max))
    {
        tmp = (PinExistTbl[group][num] >> chgmode) & 0x01;
        result = PARAM_OK;
        if (tmp == 0){ result = PARAM_NG;}
    }
    else
    {
        result = PARAM_NG;
    }

    return (result);
}
static int32_t check_param_func_pin_exist(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode)
{
    int32_t result = PARAM_NG;
    uint8_t chgfunc;
    uint16_t tmp;

    chgfunc = change_func_to_num(mode);
    /* param check skip if func is INPUT or OUTPUT */
    if(chgfunc == 1)
    {
        return (PARAM_OK);
    }       
    if ((chgfunc < 13) && (group < GPIO_GROUP_Max) && (num < GPIO_PORT_Max))
    {
        tmp = (PinExistTbl[group][num] >> chgfunc) & 0x01;
        result = PARAM_OK;
        if (tmp == 0){ result = PARAM_NG;}
    }
    else
    {
        result = PARAM_NG;
    }

    return (result);
}
/**
 *  @}
 */ /* End of group GPIO_Private_functions */



/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup GPIO_Exported_functions
 *  @{
 */
/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result _gpio_init(_gpio_t *p_obj, uint32_t group)
  * @brief     Initialize the GPIO object.
  * @param[in] p_obj :GPIO object.
  * @param[in] group :GPIO Port Group.  : Use @ref gpio_gr_t
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result _gpio_init(_gpio_t *p_obj, uint32_t group)
{
    TXZ_Result result = TXZ_SUCCESS;

    /* Check the NULL of address. */
    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else
    {
        switch (group)
        {
#if defined(TMPM4G6)
        case GPIO_PORT_A:
            /* PA Clock Enable */
            TSB_CG_FSYSMENB_IPMENB02 = 1U;
            p_obj->p_pa_instance->DATA  =  0x00;
            p_obj->p_pa_instance->CR    =  0x00;
            p_obj->p_pa_instance->FR1   =  0x00;
            p_obj->p_pa_instance->FR2   =  0x00;
            p_obj->p_pa_instance->FR3   =  0x00;
            p_obj->p_pa_instance->FR5   =  0x00;
            p_obj->p_pa_instance->FR6   =  0x00;
            p_obj->p_pa_instance->FR7   =  0x00;
            p_obj->p_pa_instance->OD    =  0x00;
            p_obj->p_pa_instance->PUP   =  0x00;
            p_obj->p_pa_instance->PDN   =  0x00;
            p_obj->p_pa_instance->IE    =  0x00;
            break;
        case GPIO_PORT_B:
            /* PB Clock Enable */
            TSB_CG_FSYSMENB_IPMENB03 = 1U;
            p_obj->p_pb_instance->DATA  =  0x00;
            p_obj->p_pb_instance->CR    =  0x00;
            p_obj->p_pb_instance->FR1   =  0x00;
            p_obj->p_pb_instance->FR2   =  0x00;
            p_obj->p_pb_instance->FR3   =  0x00;
            p_obj->p_pb_instance->FR5   =  0x00;
            p_obj->p_pb_instance->FR6   =  0x00;
            p_obj->p_pb_instance->OD    =  0x00;
            p_obj->p_pb_instance->PUP   =  0x00;
            p_obj->p_pb_instance->PDN   =  0x00;
            p_obj->p_pb_instance->IE    =  0x00;
            break;
        case GPIO_PORT_D:
            /* PD Clock Enable */
            TSB_CG_FSYSMENB_IPMENB05 = 1U;
            p_obj->p_pd_instance->DATA  =  0x00;
            p_obj->p_pd_instance->CR    =  0x00;
            p_obj->p_pd_instance->FR1   =  0x00;
            p_obj->p_pd_instance->FR2   =  0x00;
            p_obj->p_pd_instance->FR3   =  0x00;
            p_obj->p_pd_instance->FR4   =  0x00;
            p_obj->p_pd_instance->FR5   =  0x00;
            p_obj->p_pd_instance->FR6   =  0x00;
            p_obj->p_pd_instance->FR7   =  0x00;
            p_obj->p_pd_instance->OD    =  0x00;
            p_obj->p_pd_instance->PUP   =  0x00;
            p_obj->p_pd_instance->PDN   =  0x00;
            p_obj->p_pd_instance->IE    =  0x00;
            break;
        case GPIO_PORT_E:
            /* PE Clock Enable */
            TSB_CG_FSYSMENB_IPMENB06 = 1U;
            p_obj->p_pe_instance->DATA  =  0x00;
            p_obj->p_pe_instance->CR    =  0x00;
            p_obj->p_pe_instance->FR1   =  0x00;
            p_obj->p_pe_instance->FR2   =  0x00;
            p_obj->p_pe_instance->FR3   =  0x00;
            p_obj->p_pe_instance->FR4   =  0x00;
            p_obj->p_pe_instance->FR5   =  0x00;
            p_obj->p_pe_instance->FR7   =  0x00;
            p_obj->p_pe_instance->OD    =  0x00;
            p_obj->p_pe_instance->PUP   =  0x00;
            p_obj->p_pe_instance->PDN   =  0x00;
            p_obj->p_pe_instance->IE    =  0x00;
            break;
        case GPIO_PORT_F:
            /* PF Clock Enable */
            TSB_CG_FSYSMENB_IPMENB07= 1U;
            p_obj->p_pf_instance->DATA  =  0x00;
            p_obj->p_pf_instance->CR    =  0x00;
            p_obj->p_pf_instance->FR1   =  0x00;
            p_obj->p_pf_instance->FR7   =  0x00;
            p_obj->p_pf_instance->OD    =  0x00;
            p_obj->p_pf_instance->PUP   =  0x00;
            p_obj->p_pf_instance->PDN   =  0x00;
            p_obj->p_pf_instance->IE    =  0x00;
            break;
        case GPIO_PORT_G:
            /* PG Clock Enable */
            TSB_CG_FSYSMENB_IPMENB08 = 1U;
            p_obj->p_pg_instance->DATA  =  0x00;
            p_obj->p_pg_instance->CR    =  0x00;
            p_obj->p_pg_instance->FR1   =  0x00;
            p_obj->p_pg_instance->FR2   =  0x00;
            p_obj->p_pg_instance->FR3   =  0x00;
            p_obj->p_pg_instance->FR4   =  0x00;
            p_obj->p_pg_instance->FR5   =  0x00;
            p_obj->p_pg_instance->FR7   =  0x00;
            p_obj->p_pg_instance->OD    =  0x00;
            p_obj->p_pg_instance->PUP   =  0x00;
            p_obj->p_pg_instance->PDN   =  0x00;
            p_obj->p_pg_instance->IE    =  0x00;
            break;
        case GPIO_PORT_H:
            /* PH Clock Enable */
            TSB_CG_FSYSMENB_IPMENB09 = 1U;
            p_obj->p_ph_instance->DATA  =  0x00;
            p_obj->p_ph_instance->CR    =  0x50;
            p_obj->p_ph_instance->FR1   =  0xF8;
            p_obj->p_ph_instance->FR3   =  0x00;
            p_obj->p_ph_instance->FR4   =  0x00;
            p_obj->p_ph_instance->FR5   =  0x00;
            p_obj->p_ph_instance->OD    =  0x00;
            p_obj->p_ph_instance->PUP   =  0x98;
            p_obj->p_ph_instance->PDN   =  0x20;
            p_obj->p_ph_instance->IE    =  0xB8;
            break;
        case GPIO_PORT_K:
            /* PK Clock Enable */
            TSB_CG_FSYSMENB_IPMENB11 = 1U;
            p_obj->p_pk_instance->DATA  =  0x00;
            p_obj->p_pk_instance->CR    =  0x00;
            p_obj->p_pk_instance->FR1   =  0x00;
            p_obj->p_pk_instance->FR2   =  0x00;
            p_obj->p_pk_instance->FR3   =  0x00;
            p_obj->p_pk_instance->FR4   =  0x00;
            p_obj->p_pk_instance->FR6   =  0x00;
            p_obj->p_pk_instance->FR7   =  0x00;
            p_obj->p_pk_instance->OD    =  0x00;
            p_obj->p_pk_instance->PUP   =  0x00;
            p_obj->p_pk_instance->PDN   =  0x00;
            p_obj->p_pk_instance->IE    =  0x00;
            break;
        case GPIO_PORT_L:
            /* PL Clock Enable */
            TSB_CG_FSYSMENB_IPMENB12 = 1U;
            p_obj->p_pl_instance->DATA  =  0x00;
            p_obj->p_pl_instance->CR    =  0x00;
            p_obj->p_pl_instance->FR1   =  0x00;
            p_obj->p_pl_instance->FR2   =  0x00;
            p_obj->p_pl_instance->FR3   =  0x00;
            p_obj->p_pl_instance->FR6   =  0x00;
            p_obj->p_pl_instance->FR7   =  0x00;
            p_obj->p_pl_instance->OD    =  0x00;
            p_obj->p_pl_instance->PUP   =  0x00;
            p_obj->p_pl_instance->PDN   =  0x00;
            p_obj->p_pl_instance->IE    =  0x00;
            break;
        case GPIO_PORT_N:
            /* PN Clock Enable */
            TSB_CG_FSYSMENB_IPMENB14 = 1U;
            p_obj->p_pn_instance->DATA  =  0x00;
            p_obj->p_pn_instance->CR    =  0x00;
            p_obj->p_pn_instance->OD    =  0x00;
            p_obj->p_pn_instance->PUP   =  0x00;
            p_obj->p_pn_instance->PDN   =  0x00;
            p_obj->p_pn_instance->IE    =  0x00;
            break;
        case GPIO_PORT_P:
            /* PP Clock Enable */
            TSB_CG_FSYSMENB_IPMENB15 = 1U;
            p_obj->p_pp_instance->DATA  =  0x00;
            p_obj->p_pp_instance->CR    =  0x00;
            p_obj->p_pp_instance->FR2   =  0x00;
            p_obj->p_pp_instance->FR3   =  0x00;
            p_obj->p_pp_instance->FR5   =  0x00;
            p_obj->p_pp_instance->OD    =  0x00;
            p_obj->p_pp_instance->PUP   =  0x00;
            p_obj->p_pp_instance->PDN   =  0x00;
            p_obj->p_pp_instance->IE    =  0x00;
            break;
        case GPIO_PORT_T:
            /* PT Clock Enable */
            TSB_CG_FSYSMENB_IPMENB16 = 1U;
            p_obj->p_pt_instance->DATA  =  0x00;
            p_obj->p_pt_instance->CR    =  0x00;
            p_obj->p_pt_instance->FR1   =  0x00;
            p_obj->p_pt_instance->FR2   =  0x00;
            p_obj->p_pt_instance->FR3   =  0x00;
            p_obj->p_pt_instance->FR6   =  0x00;
            p_obj->p_pt_instance->FR7   =  0x00;
            p_obj->p_pt_instance->OD    =  0x00;
            p_obj->p_pt_instance->PUP   =  0x00;
            p_obj->p_pt_instance->PDN   =  0x00;
            p_obj->p_pt_instance->IE    =  0x00;
            break;
        case GPIO_PORT_Y:
            /* PY Clock Enable */
            TSB_CG_FSYSMENB_IPMENB21 = 1U;
            p_obj->p_py_instance->DATA  =  0x00;
            p_obj->p_py_instance->CR    =  0x00;
            p_obj->p_py_instance->FR1   =  0x00;
            p_obj->p_py_instance->FR4   =  0x00;
            p_obj->p_py_instance->OD    =  0x00;
            p_obj->p_py_instance->PUP   =  0x00;
            p_obj->p_py_instance->PDN   =  0x00;
            p_obj->p_py_instance->IE    =  0x00;
            break;
#endif  /* TMPM4G6 */
#if defined(TMPM4G7)
        case GPIO_PORT_A:
            /* PA Clock Enable */
            TSB_CG_FSYSMENB_IPMENB02 = 1U;
            p_obj->p_pa_instance->DATA  =  0x00;
            p_obj->p_pa_instance->CR    =  0x00;
            p_obj->p_pa_instance->FR1   =  0x00;
            p_obj->p_pa_instance->FR2   =  0x00;
            p_obj->p_pa_instance->FR3   =  0x00;
            p_obj->p_pa_instance->FR5   =  0x00;
            p_obj->p_pa_instance->FR6   =  0x00;
            p_obj->p_pa_instance->FR7   =  0x00;
            p_obj->p_pa_instance->OD    =  0x00;
            p_obj->p_pa_instance->PUP   =  0x00;
            p_obj->p_pa_instance->PDN   =  0x00;
            p_obj->p_pa_instance->IE    =  0x00;
            break;
        case GPIO_PORT_B:
            /* PB Clock Enable */
            TSB_CG_FSYSMENB_IPMENB03 = 1U;
            p_obj->p_pb_instance->DATA  =  0x00;
            p_obj->p_pb_instance->CR    =  0x00;
            p_obj->p_pb_instance->FR1   =  0x00;
            p_obj->p_pb_instance->FR2   =  0x00;
            p_obj->p_pb_instance->FR3   =  0x00;
            p_obj->p_pb_instance->FR5   =  0x00;
            p_obj->p_pb_instance->FR6   =  0x00;
            p_obj->p_pb_instance->OD    =  0x00;
            p_obj->p_pb_instance->PUP   =  0x00;
            p_obj->p_pb_instance->PDN   =  0x00;
            p_obj->p_pb_instance->IE    =  0x00;
            break;
        case GPIO_PORT_C:
            /* PC Clock Enable */
            TSB_CG_FSYSMENB_IPMENB04 = 1U;
            p_obj->p_pc_instance->DATA  =  0x00;
            p_obj->p_pc_instance->CR    =  0x00;
            p_obj->p_pc_instance->FR1   =  0x00;
            p_obj->p_pc_instance->FR3   =  0x00;
            p_obj->p_pc_instance->FR5   =  0x00;
            p_obj->p_pc_instance->OD    =  0x00;
            p_obj->p_pc_instance->PUP   =  0x00;
            p_obj->p_pc_instance->PDN   =  0x00;
            p_obj->p_pc_instance->IE    =  0x00;
            break;
        case GPIO_PORT_D:
            /* PD Clock Enable */
            TSB_CG_FSYSMENB_IPMENB05 = 1U;
            p_obj->p_pd_instance->DATA  =  0x00;
            p_obj->p_pd_instance->CR    =  0x00;
            p_obj->p_pd_instance->FR1   =  0x00;
            p_obj->p_pd_instance->FR2   =  0x00;
            p_obj->p_pd_instance->FR3   =  0x00;
            p_obj->p_pd_instance->FR4   =  0x00;
            p_obj->p_pd_instance->FR5   =  0x00;
            p_obj->p_pd_instance->FR6   =  0x00;
            p_obj->p_pd_instance->FR7   =  0x00;
            p_obj->p_pd_instance->OD    =  0x00;
            p_obj->p_pd_instance->PUP   =  0x00;
            p_obj->p_pd_instance->PDN   =  0x00;
            p_obj->p_pd_instance->IE    =  0x00;
            break;
        case GPIO_PORT_E:
            /* PE Clock Enable */
            TSB_CG_FSYSMENB_IPMENB06 = 1U;
            p_obj->p_pe_instance->DATA  =  0x00;
            p_obj->p_pe_instance->CR    =  0x00;
            p_obj->p_pe_instance->FR1   =  0x00;
            p_obj->p_pe_instance->FR2   =  0x00;
            p_obj->p_pe_instance->FR3   =  0x00;
            p_obj->p_pe_instance->FR4   =  0x00;
            p_obj->p_pe_instance->FR5   =  0x00;
            p_obj->p_pe_instance->FR7   =  0x00;
            p_obj->p_pe_instance->OD    =  0x00;
            p_obj->p_pe_instance->PUP   =  0x00;
            p_obj->p_pe_instance->PDN   =  0x00;
            p_obj->p_pe_instance->IE    =  0x00;
            break;
        case GPIO_PORT_F:
            /* PF Clock Enable */
            TSB_CG_FSYSMENB_IPMENB07= 1U;
            p_obj->p_pf_instance->DATA  =  0x00;
            p_obj->p_pf_instance->CR    =  0x00;
            p_obj->p_pf_instance->FR1   =  0x00;
            p_obj->p_pf_instance->FR7   =  0x00;
            p_obj->p_pf_instance->OD    =  0x00;
            p_obj->p_pf_instance->PUP   =  0x00;
            p_obj->p_pf_instance->PDN   =  0x00;
            p_obj->p_pf_instance->IE    =  0x00;
            break;
        case GPIO_PORT_G:
            /* PG Clock Enable */
            TSB_CG_FSYSMENB_IPMENB08 = 1U;
            p_obj->p_pg_instance->DATA  =  0x00;
            p_obj->p_pg_instance->CR    =  0x00;
            p_obj->p_pg_instance->FR1   =  0x00;
            p_obj->p_pg_instance->FR2   =  0x00;
            p_obj->p_pg_instance->FR3   =  0x00;
            p_obj->p_pg_instance->FR4   =  0x00;
            p_obj->p_pg_instance->FR5   =  0x00;
            p_obj->p_pg_instance->FR7   =  0x00;
            p_obj->p_pg_instance->OD    =  0x00;
            p_obj->p_pg_instance->PUP   =  0x00;
            p_obj->p_pg_instance->PDN   =  0x00;
            p_obj->p_pg_instance->IE    =  0x00;
            break;
        case GPIO_PORT_H:
            /* PH Clock Enable */
            TSB_CG_FSYSMENB_IPMENB09 = 1U;
            p_obj->p_ph_instance->DATA  =  0x00;
            p_obj->p_ph_instance->CR    =  0x50;
            p_obj->p_ph_instance->FR1   =  0xF8;
            p_obj->p_ph_instance->FR3   =  0x00;
            p_obj->p_ph_instance->FR4   =  0x00;
            p_obj->p_ph_instance->FR5   =  0x00;
            p_obj->p_ph_instance->OD    =  0x00;
            p_obj->p_ph_instance->PUP   =  0x98;
            p_obj->p_ph_instance->PDN   =  0x20;
            p_obj->p_ph_instance->IE    =  0xB8;
            break;
        case GPIO_PORT_K:
            /* PK Clock Enable */
            TSB_CG_FSYSMENB_IPMENB11 = 1U;
            p_obj->p_pk_instance->DATA  =  0x00;
            p_obj->p_pk_instance->CR    =  0x00;
            p_obj->p_pk_instance->FR1   =  0x00;
            p_obj->p_pk_instance->FR2   =  0x00;
            p_obj->p_pk_instance->FR3   =  0x00;
            p_obj->p_pk_instance->FR4   =  0x00;
            p_obj->p_pk_instance->FR6   =  0x00;
            p_obj->p_pk_instance->FR7   =  0x00;
            p_obj->p_pk_instance->OD    =  0x00;
            p_obj->p_pk_instance->PUP   =  0x00;
            p_obj->p_pk_instance->PDN   =  0x00;
            p_obj->p_pk_instance->IE    =  0x00;
            break;
        case GPIO_PORT_L:
            /* PL Clock Enable */
            TSB_CG_FSYSMENB_IPMENB12 = 1U;
            p_obj->p_pl_instance->DATA  =  0x00;
            p_obj->p_pl_instance->CR    =  0x00;
            p_obj->p_pl_instance->FR1   =  0x00;
            p_obj->p_pl_instance->FR2   =  0x00;
            p_obj->p_pl_instance->FR3   =  0x00;
            p_obj->p_pl_instance->FR6   =  0x00;
            p_obj->p_pl_instance->FR7   =  0x00;
            p_obj->p_pl_instance->OD    =  0x00;
            p_obj->p_pl_instance->PUP   =  0x00;
            p_obj->p_pl_instance->PDN   =  0x00;
            p_obj->p_pl_instance->IE    =  0x00;
            break;
        case GPIO_PORT_N:
            /* PN Clock Enable */
            TSB_CG_FSYSMENB_IPMENB14 = 1U;
            p_obj->p_pn_instance->DATA  =  0x00;
            p_obj->p_pn_instance->CR    =  0x00;
            p_obj->p_pn_instance->OD    =  0x00;
            p_obj->p_pn_instance->PUP   =  0x00;
            p_obj->p_pn_instance->PDN   =  0x00;
            p_obj->p_pn_instance->IE    =  0x00;
            break;
        case GPIO_PORT_P:
            /* PP Clock Enable */
            TSB_CG_FSYSMENB_IPMENB15 = 1U;
            p_obj->p_pp_instance->DATA  =  0x00;
            p_obj->p_pp_instance->CR    =  0x00;
            p_obj->p_pp_instance->FR2   =  0x00;
            p_obj->p_pp_instance->FR3   =  0x00;
            p_obj->p_pp_instance->FR5   =  0x00;
            p_obj->p_pp_instance->OD    =  0x00;
            p_obj->p_pp_instance->PUP   =  0x00;
            p_obj->p_pp_instance->PDN   =  0x00;
            p_obj->p_pp_instance->IE    =  0x00;
            break;
        case GPIO_PORT_R:
            /* PR Clock Enable */
            TSB_CG_FSYSMENB_IPMENB16 = 1U;
            p_obj->p_pr_instance->DATA  =  0x00;
            p_obj->p_pr_instance->CR    =  0x00;
            p_obj->p_pr_instance->FR2   =  0x00;
            p_obj->p_pr_instance->FR3   =  0x00;
            p_obj->p_pr_instance->OD    =  0x00;
            p_obj->p_pr_instance->PUP   =  0x00;
            p_obj->p_pr_instance->PDN   =  0x00;
            p_obj->p_pr_instance->IE    =  0x00;
            break;
        case GPIO_PORT_T:
            /* PT Clock Enable */
            TSB_CG_FSYSMENB_IPMENB17 = 1U;
            p_obj->p_pt_instance->DATA  =  0x00;
            p_obj->p_pt_instance->CR    =  0x00;
            p_obj->p_pt_instance->FR1   =  0x00;
            p_obj->p_pt_instance->FR2   =  0x00;
            p_obj->p_pt_instance->FR3   =  0x00;
            p_obj->p_pt_instance->FR6   =  0x00;
            p_obj->p_pt_instance->FR7   =  0x00;
            p_obj->p_pt_instance->OD    =  0x00;
            p_obj->p_pt_instance->PUP   =  0x00;
            p_obj->p_pt_instance->PDN   =  0x00;
            p_obj->p_pt_instance->IE    =  0x00;
            break;
        case GPIO_PORT_V:
            /* PV Clock Enable */
            TSB_CG_FSYSMENB_IPMENB19 = 1U;
            p_obj->p_pv_instance->DATA  =  0x00;
            p_obj->p_pv_instance->CR    =  0x00;
            p_obj->p_pv_instance->FR2   =  0x00;
            p_obj->p_pv_instance->FR3   =  0x00;
            p_obj->p_pv_instance->FR4   =  0x00;
            p_obj->p_pv_instance->FR5   =  0x00;
            p_obj->p_pv_instance->FR6   =  0x00;
            p_obj->p_pv_instance->FR7   =  0x00;
            p_obj->p_pv_instance->OD    =  0x00;
            p_obj->p_pv_instance->PUP   =  0x00;
            p_obj->p_pv_instance->PDN   =  0x00;
            p_obj->p_pv_instance->IE    =  0x00;
            break;
        case GPIO_PORT_Y:
            /* PY Clock Enable */
            TSB_CG_FSYSMENB_IPMENB21 = 1U;
            p_obj->p_py_instance->DATA  =  0x00;
            p_obj->p_py_instance->CR    =  0x00;
            p_obj->p_py_instance->FR1   =  0x00;
            p_obj->p_py_instance->FR4   =  0x00;
            p_obj->p_py_instance->OD    =  0x00;
            p_obj->p_py_instance->PUP   =  0x00;
            p_obj->p_py_instance->PDN   =  0x00;
            p_obj->p_py_instance->IE    =  0x00;
            break;
#endif  /* TMPM4G7 */
#if defined(TMPM4G8)
        case GPIO_PORT_A:
            /* PA Clock Enable */
            TSB_CG_FSYSMENB_IPMENB02 = 1U;
            p_obj->p_pa_instance->DATA  =  0x00;
            p_obj->p_pa_instance->CR    =  0x00;
            p_obj->p_pa_instance->FR1   =  0x00;
            p_obj->p_pa_instance->FR2   =  0x00;
            p_obj->p_pa_instance->FR3   =  0x00;
            p_obj->p_pa_instance->FR5   =  0x00;
            p_obj->p_pa_instance->FR6   =  0x00;
            p_obj->p_pa_instance->FR7   =  0x00;
            p_obj->p_pa_instance->OD    =  0x00;
            p_obj->p_pa_instance->PUP   =  0x00;
            p_obj->p_pa_instance->PDN   =  0x00;
            p_obj->p_pa_instance->IE    =  0x00;
            break;
        case GPIO_PORT_B:
            /* PB Clock Enable */
            TSB_CG_FSYSMENB_IPMENB03 = 1U;
            p_obj->p_pb_instance->DATA  =  0x00;
            p_obj->p_pb_instance->CR    =  0x00;
            p_obj->p_pb_instance->FR1   =  0x00;
            p_obj->p_pb_instance->FR2   =  0x00;
            p_obj->p_pb_instance->FR3   =  0x00;
            p_obj->p_pb_instance->FR5   =  0x00;
            p_obj->p_pb_instance->FR6   =  0x00;
            p_obj->p_pb_instance->OD    =  0x00;
            p_obj->p_pb_instance->PUP   =  0x00;
            p_obj->p_pb_instance->PDN   =  0x00;
            p_obj->p_pb_instance->IE    =  0x00;
            break;
        case GPIO_PORT_C:
            /* PC Clock Enable */
            TSB_CG_FSYSMENB_IPMENB04 = 1U;
            p_obj->p_pc_instance->DATA  =  0x00;
            p_obj->p_pc_instance->CR    =  0x00;
            p_obj->p_pc_instance->FR1   =  0x00;
            p_obj->p_pc_instance->FR3   =  0x00;
            p_obj->p_pc_instance->FR5   =  0x00;
            p_obj->p_pc_instance->OD    =  0x00;
            p_obj->p_pc_instance->PUP   =  0x00;
            p_obj->p_pc_instance->PDN   =  0x00;
            p_obj->p_pc_instance->IE    =  0x00;
            break;
        case GPIO_PORT_D:
            /* PD Clock Enable */
            TSB_CG_FSYSMENB_IPMENB05 = 1U;
            p_obj->p_pd_instance->DATA  =  0x00;
            p_obj->p_pd_instance->CR    =  0x00;
            p_obj->p_pd_instance->FR1   =  0x00;
            p_obj->p_pd_instance->FR2   =  0x00;
            p_obj->p_pd_instance->FR3   =  0x00;
            p_obj->p_pd_instance->FR4   =  0x00;
            p_obj->p_pd_instance->FR5   =  0x00;
            p_obj->p_pd_instance->FR6   =  0x00;
            p_obj->p_pd_instance->FR7   =  0x00;
            p_obj->p_pd_instance->OD    =  0x00;
            p_obj->p_pd_instance->PUP   =  0x00;
            p_obj->p_pd_instance->PDN   =  0x00;
            p_obj->p_pd_instance->IE    =  0x00;
            break;
        case GPIO_PORT_E:
            /* PE Clock Enable */
            TSB_CG_FSYSMENB_IPMENB06 = 1U;
            p_obj->p_pe_instance->DATA  =  0x00;
            p_obj->p_pe_instance->CR    =  0x00;
            p_obj->p_pe_instance->FR1   =  0x00;
            p_obj->p_pe_instance->FR2   =  0x00;
            p_obj->p_pe_instance->FR3   =  0x00;
            p_obj->p_pe_instance->FR4   =  0x00;
            p_obj->p_pe_instance->FR5   =  0x00;
            p_obj->p_pe_instance->FR7   =  0x00;
            p_obj->p_pe_instance->OD    =  0x00;
            p_obj->p_pe_instance->PUP   =  0x00;
            p_obj->p_pe_instance->PDN   =  0x00;
            p_obj->p_pe_instance->IE    =  0x00;
            break;
        case GPIO_PORT_F:
            /* PF Clock Enable */
            TSB_CG_FSYSMENB_IPMENB07= 1U;
            p_obj->p_pf_instance->DATA  =  0x00;
            p_obj->p_pf_instance->CR    =  0x00;
            p_obj->p_pf_instance->FR1   =  0x00;
            p_obj->p_pf_instance->FR7   =  0x00;
            p_obj->p_pf_instance->OD    =  0x00;
            p_obj->p_pf_instance->PUP   =  0x00;
            p_obj->p_pf_instance->PDN   =  0x00;
            p_obj->p_pf_instance->IE    =  0x00;
            break;
        case GPIO_PORT_G:
            /* PG Clock Enable */
            TSB_CG_FSYSMENB_IPMENB08 = 1U;
            p_obj->p_pg_instance->DATA  =  0x00;
            p_obj->p_pg_instance->CR    =  0x00;
            p_obj->p_pg_instance->FR1   =  0x00;
            p_obj->p_pg_instance->FR2   =  0x00;
            p_obj->p_pg_instance->FR3   =  0x00;
            p_obj->p_pg_instance->FR4   =  0x00;
            p_obj->p_pg_instance->FR5   =  0x00;
            p_obj->p_pg_instance->FR7   =  0x00;
            p_obj->p_pg_instance->OD    =  0x00;
            p_obj->p_pg_instance->PUP   =  0x00;
            p_obj->p_pg_instance->PDN   =  0x00;
            p_obj->p_pg_instance->IE    =  0x00;
            break;
        case GPIO_PORT_H:
            /* PH Clock Enable */
            TSB_CG_FSYSMENB_IPMENB09 = 1U;
            p_obj->p_ph_instance->DATA  =  0x00;
            p_obj->p_ph_instance->CR    =  0x50;
            p_obj->p_ph_instance->FR1   =  0xF8;
            p_obj->p_ph_instance->FR3   =  0x00;
            p_obj->p_ph_instance->FR4   =  0x00;
            p_obj->p_ph_instance->FR5   =  0x00;
            p_obj->p_ph_instance->OD    =  0x00;
            p_obj->p_ph_instance->PUP   =  0x98;
            p_obj->p_ph_instance->PDN   =  0x20;
            p_obj->p_ph_instance->IE    =  0xB8;
            break;
        case GPIO_PORT_K:
            /* PK Clock Enable */
            TSB_CG_FSYSMENB_IPMENB11 = 1U;
            p_obj->p_pk_instance->DATA  =  0x00;
            p_obj->p_pk_instance->CR    =  0x00;
            p_obj->p_pk_instance->FR1   =  0x00;
            p_obj->p_pk_instance->FR2   =  0x00;
            p_obj->p_pk_instance->FR3   =  0x00;
            p_obj->p_pk_instance->FR4   =  0x00;
            p_obj->p_pk_instance->FR6   =  0x00;
            p_obj->p_pk_instance->FR7   =  0x00;
            p_obj->p_pk_instance->OD    =  0x00;
            p_obj->p_pk_instance->PUP   =  0x00;
            p_obj->p_pk_instance->PDN   =  0x00;
            p_obj->p_pk_instance->IE    =  0x00;
            break;
        case GPIO_PORT_L:
            /* PL Clock Enable */
            TSB_CG_FSYSMENB_IPMENB12 = 1U;
            p_obj->p_pl_instance->DATA  =  0x00;
            p_obj->p_pl_instance->CR    =  0x00;
            p_obj->p_pl_instance->FR1   =  0x00;
            p_obj->p_pl_instance->FR2   =  0x00;
            p_obj->p_pl_instance->FR3   =  0x00;
            p_obj->p_pl_instance->FR6   =  0x00;
            p_obj->p_pl_instance->FR7   =  0x00;
            p_obj->p_pl_instance->OD    =  0x00;
            p_obj->p_pl_instance->PUP   =  0x00;
            p_obj->p_pl_instance->PDN   =  0x00;
            p_obj->p_pl_instance->IE    =  0x00;
            break;
        case GPIO_PORT_M:
            /* PM Clock Enable */
            TSB_CG_FSYSMENB_IPMENB13 = 1U;
            p_obj->p_pm_instance->DATA  =  0x00;
            p_obj->p_pm_instance->CR    =  0x00;
            p_obj->p_pm_instance->FR2   =  0x00;
            p_obj->p_pm_instance->FR3   =  0x00;
            p_obj->p_pm_instance->FR4   =  0x00;
            p_obj->p_pm_instance->FR5   =  0x00;
            p_obj->p_pm_instance->FR6   =  0x00;
            p_obj->p_pm_instance->FR7   =  0x00;
            p_obj->p_pm_instance->OD    =  0x00;
            p_obj->p_pm_instance->PUP   =  0x00;
            p_obj->p_pm_instance->PDN   =  0x00;
            p_obj->p_pm_instance->IE    =  0x00;
            break;
        case GPIO_PORT_N:
            /* PN Clock Enable */
            TSB_CG_FSYSMENB_IPMENB14 = 1U;
            p_obj->p_pn_instance->DATA  =  0x00;
            p_obj->p_pn_instance->CR    =  0x00;
            p_obj->p_pn_instance->OD    =  0x00;
            p_obj->p_pn_instance->PUP   =  0x00;
            p_obj->p_pn_instance->PDN   =  0x00;
            p_obj->p_pn_instance->IE    =  0x00;
            break;
        case GPIO_PORT_P:
            /* PP Clock Enable */
            TSB_CG_FSYSMENB_IPMENB15 = 1U;
            p_obj->p_pp_instance->DATA  =  0x00;
            p_obj->p_pp_instance->CR    =  0x00;
            p_obj->p_pp_instance->FR2   =  0x00;
            p_obj->p_pp_instance->FR3   =  0x00;
            p_obj->p_pp_instance->FR5   =  0x00;
            p_obj->p_pp_instance->OD    =  0x00;
            p_obj->p_pp_instance->PUP   =  0x00;
            p_obj->p_pp_instance->PDN   =  0x00;
            p_obj->p_pp_instance->IE    =  0x00;
            break;
        case GPIO_PORT_R:
            /* PR Clock Enable */
            TSB_CG_FSYSMENB_IPMENB16 = 1U;
            p_obj->p_pr_instance->DATA  =  0x00;
            p_obj->p_pr_instance->CR    =  0x00;
            p_obj->p_pr_instance->FR2   =  0x00;
            p_obj->p_pr_instance->FR3   =  0x00;
            p_obj->p_pr_instance->OD    =  0x00;
            p_obj->p_pr_instance->PUP   =  0x00;
            p_obj->p_pr_instance->PDN   =  0x00;
            p_obj->p_pr_instance->IE    =  0x00;
            break;
        case GPIO_PORT_T:
            /* PT Clock Enable */
            TSB_CG_FSYSMENB_IPMENB17 = 1U;
            p_obj->p_pt_instance->DATA  =  0x00;
            p_obj->p_pt_instance->CR    =  0x00;
            p_obj->p_pt_instance->FR1   =  0x00;
            p_obj->p_pt_instance->FR2   =  0x00;
            p_obj->p_pt_instance->FR3   =  0x00;
            p_obj->p_pt_instance->FR6   =  0x00;
            p_obj->p_pt_instance->FR7   =  0x00;
            p_obj->p_pt_instance->OD    =  0x00;
            p_obj->p_pt_instance->PUP   =  0x00;
            p_obj->p_pt_instance->PDN   =  0x00;
            p_obj->p_pt_instance->IE    =  0x00;
            break;
        case GPIO_PORT_V:
            /* PV Clock Enable */
            TSB_CG_FSYSMENB_IPMENB19 = 1U;
            p_obj->p_pv_instance->DATA  =  0x00;
            p_obj->p_pv_instance->CR    =  0x00;
            p_obj->p_pv_instance->FR2   =  0x00;
            p_obj->p_pv_instance->FR3   =  0x00;
            p_obj->p_pv_instance->FR4   =  0x00;
            p_obj->p_pv_instance->FR5   =  0x00;
            p_obj->p_pv_instance->FR6   =  0x00;
            p_obj->p_pv_instance->FR7   =  0x00;
            p_obj->p_pv_instance->OD    =  0x00;
            p_obj->p_pv_instance->PUP   =  0x00;
            p_obj->p_pv_instance->PDN   =  0x00;
            p_obj->p_pv_instance->IE    =  0x00;
            break;
        case GPIO_PORT_Y:
            /* PY Clock Enable */
            TSB_CG_FSYSMENB_IPMENB21 = 1U;
            p_obj->p_py_instance->DATA  =  0x00;
            p_obj->p_py_instance->CR    =  0x00;
            p_obj->p_py_instance->FR1   =  0x00;
            p_obj->p_py_instance->FR4   =  0x00;
            p_obj->p_py_instance->OD    =  0x00;
            p_obj->p_py_instance->PUP   =  0x00;
            p_obj->p_py_instance->PDN   =  0x00;
            p_obj->p_py_instance->IE    =  0x00;
            break;
#endif  /* TMPM4G8 */
#if defined(TMPM4G9)
        case GPIO_PORT_A:
            /* PA Clock Enable */
            TSB_CG_FSYSMENB_IPMENB02 = 1U;
            p_obj->p_pa_instance->DATA  =  0x00;
            p_obj->p_pa_instance->CR    =  0x00;
            p_obj->p_pa_instance->FR1   =  0x00;
            p_obj->p_pa_instance->FR2   =  0x00;
            p_obj->p_pa_instance->FR3   =  0x00;
            p_obj->p_pa_instance->FR5   =  0x00;
            p_obj->p_pa_instance->FR6   =  0x00;
            p_obj->p_pa_instance->FR7   =  0x00;
            p_obj->p_pa_instance->OD    =  0x00;
            p_obj->p_pa_instance->PUP   =  0x00;
            p_obj->p_pa_instance->PDN   =  0x00;
            p_obj->p_pa_instance->IE    =  0x00;
            break;
        case GPIO_PORT_B:
            /* PB Clock Enable */
            TSB_CG_FSYSMENB_IPMENB03 = 1U;
            p_obj->p_pb_instance->DATA  =  0x00;
            p_obj->p_pb_instance->CR    =  0x00;
            p_obj->p_pb_instance->FR1   =  0x00;
            p_obj->p_pb_instance->FR2   =  0x00;
            p_obj->p_pb_instance->FR3   =  0x00;
            p_obj->p_pb_instance->FR5   =  0x00;
            p_obj->p_pb_instance->FR6   =  0x00;
            p_obj->p_pb_instance->OD    =  0x00;
            p_obj->p_pb_instance->PUP   =  0x00;
            p_obj->p_pb_instance->PDN   =  0x00;
            p_obj->p_pb_instance->IE    =  0x00;
            break;
        case GPIO_PORT_C:
            /* PC Clock Enable */
            TSB_CG_FSYSMENB_IPMENB04 = 1U;
            p_obj->p_pc_instance->DATA  =  0x00;
            p_obj->p_pc_instance->CR    =  0x00;
            p_obj->p_pc_instance->FR1   =  0x00;
            p_obj->p_pc_instance->FR3   =  0x00;
            p_obj->p_pc_instance->FR5   =  0x00;
            p_obj->p_pc_instance->OD    =  0x00;
            p_obj->p_pc_instance->PUP   =  0x00;
            p_obj->p_pc_instance->PDN   =  0x00;
            p_obj->p_pc_instance->IE    =  0x00;
            break;
        case GPIO_PORT_D:
            /* PD Clock Enable */
            TSB_CG_FSYSMENB_IPMENB05 = 1U;
            p_obj->p_pd_instance->DATA  =  0x00;
            p_obj->p_pd_instance->CR    =  0x00;
            p_obj->p_pd_instance->FR1   =  0x00;
            p_obj->p_pd_instance->FR2   =  0x00;
            p_obj->p_pd_instance->FR3   =  0x00;
            p_obj->p_pd_instance->FR4   =  0x00;
            p_obj->p_pd_instance->FR5   =  0x00;
            p_obj->p_pd_instance->FR6   =  0x00;
            p_obj->p_pd_instance->FR7   =  0x00;
            p_obj->p_pd_instance->OD    =  0x00;
            p_obj->p_pd_instance->PUP   =  0x00;
            p_obj->p_pd_instance->PDN   =  0x00;
            p_obj->p_pd_instance->IE    =  0x00;
            break;
        case GPIO_PORT_E:
            /* PE Clock Enable */
            TSB_CG_FSYSMENB_IPMENB06 = 1U;
            p_obj->p_pe_instance->DATA  =  0x00;
            p_obj->p_pe_instance->CR    =  0x00;
            p_obj->p_pe_instance->FR1   =  0x00;
            p_obj->p_pe_instance->FR2   =  0x00;
            p_obj->p_pe_instance->FR3   =  0x00;
            p_obj->p_pe_instance->FR4   =  0x00;
            p_obj->p_pe_instance->FR5   =  0x00;
            p_obj->p_pe_instance->FR7   =  0x00;
            p_obj->p_pe_instance->OD    =  0x00;
            p_obj->p_pe_instance->PUP   =  0x00;
            p_obj->p_pe_instance->PDN   =  0x00;
            p_obj->p_pe_instance->IE    =  0x00;
            break;
        case GPIO_PORT_F:
            /* PF Clock Enable */
            TSB_CG_FSYSMENB_IPMENB07= 1U;
            p_obj->p_pf_instance->DATA  =  0x00;
            p_obj->p_pf_instance->CR    =  0x00;
            p_obj->p_pf_instance->FR1   =  0x00;
            p_obj->p_pf_instance->FR7   =  0x00;
            p_obj->p_pf_instance->OD    =  0x00;
            p_obj->p_pf_instance->PUP   =  0x00;
            p_obj->p_pf_instance->PDN   =  0x00;
            p_obj->p_pf_instance->IE    =  0x00;
            break;
        case GPIO_PORT_G:
            /* PG Clock Enable */
            TSB_CG_FSYSMENB_IPMENB08 = 1U;
            p_obj->p_pg_instance->DATA  =  0x00;
            p_obj->p_pg_instance->CR    =  0x00;
            p_obj->p_pg_instance->FR1   =  0x00;
            p_obj->p_pg_instance->FR2   =  0x00;
            p_obj->p_pg_instance->FR3   =  0x00;
            p_obj->p_pg_instance->FR4   =  0x00;
            p_obj->p_pg_instance->FR5   =  0x00;
            p_obj->p_pg_instance->FR7   =  0x00;
            p_obj->p_pg_instance->OD    =  0x00;
            p_obj->p_pg_instance->PUP   =  0x00;
            p_obj->p_pg_instance->PDN   =  0x00;
            p_obj->p_pg_instance->IE    =  0x00;
            break;
        case GPIO_PORT_H:
            /* PH Clock Enable */
            TSB_CG_FSYSMENB_IPMENB09 = 1U;
            p_obj->p_ph_instance->DATA  =  0x00;
            p_obj->p_ph_instance->CR    =  0x50;
            p_obj->p_ph_instance->FR1   =  0xF8;
            p_obj->p_ph_instance->FR3   =  0x00;
            p_obj->p_ph_instance->FR4   =  0x00;
            p_obj->p_ph_instance->FR5   =  0x00;
            p_obj->p_ph_instance->OD    =  0x00;
            p_obj->p_ph_instance->PUP   =  0x98;
            p_obj->p_ph_instance->PDN   =  0x20;
            p_obj->p_ph_instance->IE    =  0xB8;
            break;
        case GPIO_PORT_J:
            /* PJ Clock Enable */
            TSB_CG_FSYSMENB_IPMENB10 = 1U;
            p_obj->p_pj_instance->DATA  =  0x00;
            p_obj->p_pj_instance->CR    =  0x00;
            p_obj->p_pj_instance->FR2   =  0x00;
            p_obj->p_pj_instance->FR3   =  0x00;
            p_obj->p_pj_instance->FR5   =  0x00;
            p_obj->p_pj_instance->FR7   =  0x00;
            p_obj->p_pj_instance->OD    =  0x00;
            p_obj->p_pj_instance->PUP   =  0x00;
            p_obj->p_pj_instance->PDN   =  0x00;
            p_obj->p_pj_instance->IE    =  0x00;
            break;
        case GPIO_PORT_K:
            /* PK Clock Enable */
            TSB_CG_FSYSMENB_IPMENB11 = 1U;
            p_obj->p_pk_instance->DATA  =  0x00;
            p_obj->p_pk_instance->CR    =  0x00;
            p_obj->p_pk_instance->FR1   =  0x00;
            p_obj->p_pk_instance->FR2   =  0x00;
            p_obj->p_pk_instance->FR3   =  0x00;
            p_obj->p_pk_instance->FR4   =  0x00;
            p_obj->p_pk_instance->FR6   =  0x00;
            p_obj->p_pk_instance->FR7   =  0x00;
            p_obj->p_pk_instance->OD    =  0x00;
            p_obj->p_pk_instance->PUP   =  0x00;
            p_obj->p_pk_instance->PDN   =  0x00;
            p_obj->p_pk_instance->IE    =  0x00;
            break;
        case GPIO_PORT_L:
            /* PL Clock Enable */
            TSB_CG_FSYSMENB_IPMENB12 = 1U;
            p_obj->p_pl_instance->DATA  =  0x00;
            p_obj->p_pl_instance->CR    =  0x00;
            p_obj->p_pl_instance->FR1   =  0x00;
            p_obj->p_pl_instance->FR2   =  0x00;
            p_obj->p_pl_instance->FR3   =  0x00;
            p_obj->p_pl_instance->FR6   =  0x00;
            p_obj->p_pl_instance->FR7   =  0x00;
            p_obj->p_pl_instance->OD    =  0x00;
            p_obj->p_pl_instance->PUP   =  0x00;
            p_obj->p_pl_instance->PDN   =  0x00;
            p_obj->p_pl_instance->IE    =  0x00;
            break;
        case GPIO_PORT_M:
            /* PM Clock Enable */
            TSB_CG_FSYSMENB_IPMENB13 = 1U;
            p_obj->p_pm_instance->DATA  =  0x00;
            p_obj->p_pm_instance->CR    =  0x00;
            p_obj->p_pm_instance->FR2   =  0x00;
            p_obj->p_pm_instance->FR3   =  0x00;
            p_obj->p_pm_instance->FR4   =  0x00;
            p_obj->p_pm_instance->FR5   =  0x00;
            p_obj->p_pm_instance->FR6   =  0x00;
            p_obj->p_pm_instance->FR7   =  0x00;
            p_obj->p_pm_instance->OD    =  0x00;
            p_obj->p_pm_instance->PUP   =  0x00;
            p_obj->p_pm_instance->PDN   =  0x00;
            p_obj->p_pm_instance->IE    =  0x00;
            break;
        case GPIO_PORT_N:
            /* PN Clock Enable */
            TSB_CG_FSYSMENB_IPMENB14 = 1U;
            p_obj->p_pn_instance->DATA  =  0x00;
            p_obj->p_pn_instance->CR    =  0x00;
            p_obj->p_pn_instance->OD    =  0x00;
            p_obj->p_pn_instance->PUP   =  0x00;
            p_obj->p_pn_instance->PDN   =  0x00;
            p_obj->p_pn_instance->IE    =  0x00;
            break;
        case GPIO_PORT_P:
            /* PP Clock Enable */
            TSB_CG_FSYSMENB_IPMENB15 = 1U;
            p_obj->p_pp_instance->DATA  =  0x00;
            p_obj->p_pp_instance->CR    =  0x00;
            p_obj->p_pp_instance->FR2   =  0x00;
            p_obj->p_pp_instance->FR3   =  0x00;
            p_obj->p_pp_instance->FR5   =  0x00;
            p_obj->p_pp_instance->OD    =  0x00;
            p_obj->p_pp_instance->PUP   =  0x00;
            p_obj->p_pp_instance->PDN   =  0x00;
            p_obj->p_pp_instance->IE    =  0x00;
            break;
        case GPIO_PORT_R:
            /* PR Clock Enable */
            TSB_CG_FSYSMENB_IPMENB16 = 1U;
            p_obj->p_pr_instance->DATA  =  0x00;
            p_obj->p_pr_instance->CR    =  0x00;
            p_obj->p_pr_instance->FR2   =  0x00;
            p_obj->p_pr_instance->FR3   =  0x00;
            p_obj->p_pr_instance->OD    =  0x00;
            p_obj->p_pr_instance->PUP   =  0x00;
            p_obj->p_pr_instance->PDN   =  0x00;
            p_obj->p_pr_instance->IE    =  0x00;
            break;
        case GPIO_PORT_T:
            /* PT Clock Enable */
            TSB_CG_FSYSMENB_IPMENB17 = 1U;
            p_obj->p_pt_instance->DATA  =  0x00;
            p_obj->p_pt_instance->CR    =  0x00;
            p_obj->p_pt_instance->FR1   =  0x00;
            p_obj->p_pt_instance->FR2   =  0x00;
            p_obj->p_pt_instance->FR3   =  0x00;
            p_obj->p_pt_instance->FR6   =  0x00;
            p_obj->p_pt_instance->FR7   =  0x00;
            p_obj->p_pt_instance->OD    =  0x00;
            p_obj->p_pt_instance->PUP   =  0x00;
            p_obj->p_pt_instance->PDN   =  0x00;
            p_obj->p_pt_instance->IE    =  0x00;
            break;
        case GPIO_PORT_U:
            /* PU Clock Enable */
            TSB_CG_FSYSMENB_IPMENB18 = 1U;
            p_obj->p_pu_instance->DATA  =  0x00;
            p_obj->p_pu_instance->CR    =  0x00;
            p_obj->p_pu_instance->FR2   =  0x00;
            p_obj->p_pu_instance->FR3   =  0x00;
            p_obj->p_pu_instance->FR7   =  0x00;
            p_obj->p_pu_instance->OD    =  0x00;
            p_obj->p_pu_instance->PUP   =  0x00;
            p_obj->p_pu_instance->PDN   =  0x00;
            p_obj->p_pu_instance->IE    =  0x00;
            break;
        case GPIO_PORT_V:
            /* PV Clock Enable */
            TSB_CG_FSYSMENB_IPMENB19 = 1U;
            p_obj->p_pv_instance->DATA  =  0x00;
            p_obj->p_pv_instance->CR    =  0x00;
            p_obj->p_pv_instance->FR2   =  0x00;
            p_obj->p_pv_instance->FR3   =  0x00;
            p_obj->p_pv_instance->FR4   =  0x00;
            p_obj->p_pv_instance->FR5   =  0x00;
            p_obj->p_pv_instance->FR6   =  0x00;
            p_obj->p_pv_instance->FR7   =  0x00;
            p_obj->p_pv_instance->OD    =  0x00;
            p_obj->p_pv_instance->PUP   =  0x00;
            p_obj->p_pv_instance->PDN   =  0x00;
            p_obj->p_pv_instance->IE    =  0x00;
            break;
        case GPIO_PORT_W:
            /* PW Clock Enable */
            TSB_CG_FSYSMENB_IPMENB20 = 1U;
            p_obj->p_pw_instance->DATA  =  0x00;
            p_obj->p_pw_instance->CR    =  0x00;
            p_obj->p_pw_instance->FR3   =  0x00;
            p_obj->p_pw_instance->FR4   =  0x00;
            p_obj->p_pw_instance->FR5   =  0x00;
            p_obj->p_pw_instance->FR6   =  0x00;
            p_obj->p_pw_instance->FR7   =  0x00;
            p_obj->p_pw_instance->OD    =  0x00;
            p_obj->p_pw_instance->PUP   =  0x00;
            p_obj->p_pw_instance->PDN   =  0x00;
            p_obj->p_pw_instance->IE    =  0x00;
            break;
        case GPIO_PORT_Y:
            /* PY Clock Enable */
            TSB_CG_FSYSMENB_IPMENB21 = 1U;
            p_obj->p_py_instance->DATA  =  0x00;
            p_obj->p_py_instance->CR    =  0x00;
            p_obj->p_py_instance->FR1   =  0x00;
            p_obj->p_py_instance->FR4   =  0x00;
            p_obj->p_py_instance->OD    =  0x00;
            p_obj->p_py_instance->PUP   =  0x00;
            p_obj->p_py_instance->PDN   =  0x00;
            p_obj->p_py_instance->IE    =  0x00;
            break;
#endif  /* TMPM4G9 */
        default:
            result = TXZ_ERROR;
            return (result);
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result gpio_deinit(_gpio_t *p_obj, uint32_t group)
  * @brief     Release the GPIO object.
  * @param     p_obj :GPIO object.
  * @param     group  :GPIO Port Group.: Use @ref gpio_gr_t
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result gpio_deinit(_gpio_t *p_obj, uint32_t group)
{
    TXZ_Result result = TXZ_SUCCESS;

    /* Check the NULL of address. */
    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else
    {
        /* Disable the selected GPIO peripheral  */
        switch (group)
        {
#if defined(TMPM4G6)
        case GPIO_PORT_A:
            p_obj->p_pa_instance->DATA  =  0x00;
            p_obj->p_pa_instance->CR    =  0x00;
            p_obj->p_pa_instance->FR1   =  0x00;
            p_obj->p_pa_instance->FR2   =  0x00;
            p_obj->p_pa_instance->FR3   =  0x00;
            p_obj->p_pa_instance->FR5   =  0x00;
            p_obj->p_pa_instance->FR6   =  0x00;
            p_obj->p_pa_instance->FR7   =  0x00;
            p_obj->p_pa_instance->OD    =  0x00;
            p_obj->p_pa_instance->PUP   =  0x00;
            p_obj->p_pa_instance->PDN   =  0x00;
            p_obj->p_pa_instance->IE    =  0x00;
            /* PA Clock Disable */
            TSB_CG_FSYSMENB_IPMENB02 = 0U;
            break;
        case GPIO_PORT_B:
            p_obj->p_pb_instance->DATA  =  0x00;
            p_obj->p_pb_instance->CR    =  0x00;
            p_obj->p_pb_instance->FR1   =  0x00;
            p_obj->p_pb_instance->FR2   =  0x00;
            p_obj->p_pb_instance->FR3   =  0x00;
            p_obj->p_pb_instance->FR5   =  0x00;
            p_obj->p_pb_instance->FR6   =  0x00;
            p_obj->p_pb_instance->OD    =  0x00;
            p_obj->p_pb_instance->PUP   =  0x00;
            p_obj->p_pb_instance->PDN   =  0x00;
            p_obj->p_pb_instance->IE    =  0x00;
            /* PB Clock Disable */
            TSB_CG_FSYSMENB_IPMENB03 = 0U;
            break;
        case GPIO_PORT_D:
            p_obj->p_pd_instance->DATA  =  0x00;
            p_obj->p_pd_instance->CR    =  0x00;
            p_obj->p_pd_instance->FR1   =  0x00;
            p_obj->p_pd_instance->FR2   =  0x00;
            p_obj->p_pd_instance->FR3   =  0x00;
            p_obj->p_pd_instance->FR4   =  0x00;
            p_obj->p_pd_instance->FR5   =  0x00;
            p_obj->p_pd_instance->FR6   =  0x00;
            p_obj->p_pd_instance->FR7   =  0x00;
            p_obj->p_pd_instance->OD    =  0x00;
            p_obj->p_pd_instance->PUP   =  0x00;
            p_obj->p_pd_instance->PDN   =  0x00;
            p_obj->p_pd_instance->IE    =  0x00;
            /* PD Clock Disable */
            TSB_CG_FSYSMENB_IPMENB05 = 0U;
            break;
        case GPIO_PORT_E:
            p_obj->p_pe_instance->DATA  =  0x00;
            p_obj->p_pe_instance->CR    =  0x00;
            p_obj->p_pe_instance->FR1   =  0x00;
            p_obj->p_pe_instance->FR2   =  0x00;
            p_obj->p_pe_instance->FR3   =  0x00;
            p_obj->p_pe_instance->FR4   =  0x00;
            p_obj->p_pe_instance->FR5   =  0x00;
            p_obj->p_pe_instance->FR7   =  0x00;
            p_obj->p_pe_instance->OD    =  0x00;
            p_obj->p_pe_instance->PUP   =  0x00;
            p_obj->p_pe_instance->PDN   =  0x00;
            p_obj->p_pe_instance->IE    =  0x00;
            /* PE Clock Disable */
            TSB_CG_FSYSMENB_IPMENB06 = 0U;
            break;
        case GPIO_PORT_F:
            p_obj->p_pf_instance->DATA  =  0x00;
            p_obj->p_pf_instance->CR    =  0x00;
            p_obj->p_pf_instance->FR1   =  0x00;
            p_obj->p_pf_instance->FR7   =  0x00;
            p_obj->p_pf_instance->OD    =  0x00;
            p_obj->p_pf_instance->PUP   =  0x00;
            p_obj->p_pf_instance->PDN   =  0x00;
            p_obj->p_pf_instance->IE    =  0x00;
            /* PF Clock Disable */
            TSB_CG_FSYSMENB_IPMENB07= 0U;
            break;
        case GPIO_PORT_G:
            p_obj->p_pg_instance->DATA  =  0x00;
            p_obj->p_pg_instance->CR    =  0x00;
            p_obj->p_pg_instance->FR1   =  0x00;
            p_obj->p_pg_instance->FR2   =  0x00;
            p_obj->p_pg_instance->FR3   =  0x00;
            p_obj->p_pg_instance->FR4   =  0x00;
            p_obj->p_pg_instance->FR5   =  0x00;
            p_obj->p_pg_instance->FR7   =  0x00;
            p_obj->p_pg_instance->OD    =  0x00;
            p_obj->p_pg_instance->PUP   =  0x00;
            p_obj->p_pg_instance->PDN   =  0x00;
            p_obj->p_pg_instance->IE    =  0x00;
            /* PG Clock Disable */
            TSB_CG_FSYSMENB_IPMENB08 = 0U;
            break;
        case GPIO_PORT_H:
            p_obj->p_ph_instance->DATA  =  0x00;
            p_obj->p_ph_instance->CR    =  0x50;
            p_obj->p_ph_instance->FR1   =  0xF0;
            p_obj->p_ph_instance->FR3   =  0x00;
            p_obj->p_ph_instance->FR4   =  0x00;
            p_obj->p_ph_instance->FR5   =  0x00;
            p_obj->p_ph_instance->OD    =  0x00;
            p_obj->p_ph_instance->PUP   =  0x98;
            p_obj->p_ph_instance->PDN   =  0x20;
            p_obj->p_ph_instance->IE    =  0xB8;
            /* PH Clock Disable */
            TSB_CG_FSYSMENB_IPMENB09 = 0U;
            break;
        case GPIO_PORT_K:
            p_obj->p_pk_instance->DATA  =  0x00;
            p_obj->p_pk_instance->CR    =  0x00;
            p_obj->p_pk_instance->FR1   =  0x00;
            p_obj->p_pk_instance->FR2   =  0x00;
            p_obj->p_pk_instance->FR3   =  0x00;
            p_obj->p_pk_instance->FR4   =  0x00;
            p_obj->p_pk_instance->FR6   =  0x00;
            p_obj->p_pk_instance->FR7   =  0x00;
            p_obj->p_pk_instance->OD    =  0x00;
            p_obj->p_pk_instance->PUP   =  0x00;
            p_obj->p_pk_instance->PDN   =  0x00;
            p_obj->p_pk_instance->IE    =  0x00;
            /* PK Clock Disable */
            TSB_CG_FSYSMENB_IPMENB11 = 0U;
            break;
        case GPIO_PORT_L:
            p_obj->p_pl_instance->DATA  =  0x00;
            p_obj->p_pl_instance->CR    =  0x00;
            p_obj->p_pl_instance->FR1   =  0x00;
            p_obj->p_pl_instance->FR2   =  0x00;
            p_obj->p_pl_instance->FR3   =  0x00;
            p_obj->p_pl_instance->FR6   =  0x00;
            p_obj->p_pl_instance->FR7   =  0x00;
            p_obj->p_pl_instance->OD    =  0x00;
            p_obj->p_pl_instance->PUP   =  0x00;
            p_obj->p_pl_instance->PDN   =  0x00;
            p_obj->p_pl_instance->IE    =  0x00;
            /* PL Clock Disable */
            TSB_CG_FSYSMENB_IPMENB12 = 0U;
            break;
        case GPIO_PORT_N:
            p_obj->p_pn_instance->DATA  =  0x00;
            p_obj->p_pn_instance->CR    =  0x00;
            p_obj->p_pn_instance->OD    =  0x00;
            p_obj->p_pn_instance->PUP   =  0x00;
            p_obj->p_pn_instance->PDN   =  0x00;
            p_obj->p_pn_instance->IE    =  0x00;
            /* PN Clock Disable */
            TSB_CG_FSYSMENB_IPMENB14 = 0U;
            break;
        case GPIO_PORT_P:
            p_obj->p_pp_instance->DATA  =  0x00;
            p_obj->p_pp_instance->CR    =  0x00;
            p_obj->p_pp_instance->FR2   =  0x00;
            p_obj->p_pp_instance->FR3   =  0x00;
            p_obj->p_pp_instance->FR5   =  0x00;
            p_obj->p_pp_instance->OD    =  0x00;
            p_obj->p_pp_instance->PUP   =  0x00;
            p_obj->p_pp_instance->PDN   =  0x00;
            p_obj->p_pp_instance->IE    =  0x00;
            /* PP Clock Disable */
            TSB_CG_FSYSMENB_IPMENB15 = 0U;
            break;
        case GPIO_PORT_T:
            p_obj->p_pt_instance->DATA  =  0x00;
            p_obj->p_pt_instance->CR    =  0x00;
            p_obj->p_pt_instance->FR1   =  0x00;
            p_obj->p_pt_instance->FR2   =  0x00;
            p_obj->p_pt_instance->FR3   =  0x00;
            p_obj->p_pt_instance->FR6   =  0x00;
            p_obj->p_pt_instance->FR7   =  0x00;
            p_obj->p_pt_instance->OD    =  0x00;
            p_obj->p_pt_instance->PUP   =  0x00;
            p_obj->p_pt_instance->PDN   =  0x00;
            p_obj->p_pt_instance->IE    =  0x00;
            /* PT Clock Disable */
            TSB_CG_FSYSMENB_IPMENB16 = 0U;
            break;
        case GPIO_PORT_Y:
            p_obj->p_py_instance->DATA  =  0x00;
            p_obj->p_py_instance->CR    =  0x00;
            p_obj->p_py_instance->FR1   =  0x00;
            p_obj->p_py_instance->FR4   =  0x00;
            p_obj->p_py_instance->OD    =  0x00;
            p_obj->p_py_instance->PUP   =  0x00;
            p_obj->p_py_instance->PDN   =  0x00;
            p_obj->p_py_instance->IE    =  0x00;
            /* PY Clock Disable */
            TSB_CG_FSYSMENB_IPMENB21 = 0U;
            break;
#endif  /* TMPM4G6 */
#if defined(TMPM4G7)
        case GPIO_PORT_A:
            p_obj->p_pa_instance->DATA  =  0x00;
            p_obj->p_pa_instance->CR    =  0x00;
            p_obj->p_pa_instance->FR1   =  0x00;
            p_obj->p_pa_instance->FR2   =  0x00;
            p_obj->p_pa_instance->FR3   =  0x00;
            p_obj->p_pa_instance->FR5   =  0x00;
            p_obj->p_pa_instance->FR6   =  0x00;
            p_obj->p_pa_instance->FR7   =  0x00;
            p_obj->p_pa_instance->OD    =  0x00;
            p_obj->p_pa_instance->PUP   =  0x00;
            p_obj->p_pa_instance->PDN   =  0x00;
            p_obj->p_pa_instance->IE    =  0x00;
            /* PA Clock Disable */
            TSB_CG_FSYSMENB_IPMENB02 = 0U;
            break;
        case GPIO_PORT_B:
            p_obj->p_pb_instance->DATA  =  0x00;
            p_obj->p_pb_instance->CR    =  0x00;
            p_obj->p_pb_instance->FR1   =  0x00;
            p_obj->p_pb_instance->FR2   =  0x00;
            p_obj->p_pb_instance->FR3   =  0x00;
            p_obj->p_pb_instance->FR5   =  0x00;
            p_obj->p_pb_instance->FR6   =  0x00;
            p_obj->p_pb_instance->OD    =  0x00;
            p_obj->p_pb_instance->PUP   =  0x00;
            p_obj->p_pb_instance->PDN   =  0x00;
            p_obj->p_pb_instance->IE    =  0x00;
            /* PB Clock Disable */
            TSB_CG_FSYSMENB_IPMENB03 = 0U;
            break;
        case GPIO_PORT_C:
            p_obj->p_pc_instance->DATA  =  0x00;
            p_obj->p_pc_instance->CR    =  0x00;
            p_obj->p_pc_instance->FR1   =  0x00;
            p_obj->p_pc_instance->FR3   =  0x00;
            p_obj->p_pc_instance->FR5   =  0x00;
            p_obj->p_pc_instance->OD    =  0x00;
            p_obj->p_pc_instance->PUP   =  0x00;
            p_obj->p_pc_instance->PDN   =  0x00;
            p_obj->p_pc_instance->IE    =  0x00;
            /* PC Clock Disable */
            TSB_CG_FSYSMENB_IPMENB04 = 0U;
            break;
        case GPIO_PORT_D:
            p_obj->p_pd_instance->DATA  =  0x00;
            p_obj->p_pd_instance->CR    =  0x00;
            p_obj->p_pd_instance->FR1   =  0x00;
            p_obj->p_pd_instance->FR2   =  0x00;
            p_obj->p_pd_instance->FR3   =  0x00;
            p_obj->p_pd_instance->FR4   =  0x00;
            p_obj->p_pd_instance->FR5   =  0x00;
            p_obj->p_pd_instance->FR6   =  0x00;
            p_obj->p_pd_instance->FR7   =  0x00;
            p_obj->p_pd_instance->OD    =  0x00;
            p_obj->p_pd_instance->PUP   =  0x00;
            p_obj->p_pd_instance->PDN   =  0x00;
            p_obj->p_pd_instance->IE    =  0x00;
            /* PD Clock Disable */
            TSB_CG_FSYSMENB_IPMENB05 = 0U;
            break;
        case GPIO_PORT_E:
            p_obj->p_pe_instance->DATA  =  0x00;
            p_obj->p_pe_instance->CR    =  0x00;
            p_obj->p_pe_instance->FR1   =  0x00;
            p_obj->p_pe_instance->FR2   =  0x00;
            p_obj->p_pe_instance->FR3   =  0x00;
            p_obj->p_pe_instance->FR4   =  0x00;
            p_obj->p_pe_instance->FR5   =  0x00;
            p_obj->p_pe_instance->FR7   =  0x00;
            p_obj->p_pe_instance->OD    =  0x00;
            p_obj->p_pe_instance->PUP   =  0x00;
            p_obj->p_pe_instance->PDN   =  0x00;
            p_obj->p_pe_instance->IE    =  0x00;
            /* PE Clock Disable */
            TSB_CG_FSYSMENB_IPMENB06 = 0U;
            break;
        case GPIO_PORT_F:
            p_obj->p_pf_instance->DATA  =  0x00;
            p_obj->p_pf_instance->CR    =  0x00;
            p_obj->p_pf_instance->FR1   =  0x00;
            p_obj->p_pf_instance->FR7   =  0x00;
            p_obj->p_pf_instance->OD    =  0x00;
            p_obj->p_pf_instance->PUP   =  0x00;
            p_obj->p_pf_instance->PDN   =  0x00;
            p_obj->p_pf_instance->IE    =  0x00;
            /* PF Clock Disable */
            TSB_CG_FSYSMENB_IPMENB07= 0U;
            break;
        case GPIO_PORT_G:
            p_obj->p_pg_instance->DATA  =  0x00;
            p_obj->p_pg_instance->CR    =  0x00;
            p_obj->p_pg_instance->FR1   =  0x00;
            p_obj->p_pg_instance->FR2   =  0x00;
            p_obj->p_pg_instance->FR3   =  0x00;
            p_obj->p_pg_instance->FR4   =  0x00;
            p_obj->p_pg_instance->FR5   =  0x00;
            p_obj->p_pg_instance->FR7   =  0x00;
            p_obj->p_pg_instance->OD    =  0x00;
            p_obj->p_pg_instance->PUP   =  0x00;
            p_obj->p_pg_instance->PDN   =  0x00;
            p_obj->p_pg_instance->IE    =  0x00;
            /* PG Clock Disable */
            TSB_CG_FSYSMENB_IPMENB08 = 0U;
            break;
        case GPIO_PORT_H:
            p_obj->p_ph_instance->DATA  =  0x00;
            p_obj->p_ph_instance->CR    =  0x50;
            p_obj->p_ph_instance->FR1   =  0xF0;
            p_obj->p_ph_instance->FR3   =  0x00;
            p_obj->p_ph_instance->FR4   =  0x00;
            p_obj->p_ph_instance->FR5   =  0x00;
            p_obj->p_ph_instance->OD    =  0x00;
            p_obj->p_ph_instance->PUP   =  0x98;
            p_obj->p_ph_instance->PDN   =  0x20;
            p_obj->p_ph_instance->IE    =  0xB8;
            /* PH Clock Disable */
            TSB_CG_FSYSMENB_IPMENB09 = 0U;
            break;
        case GPIO_PORT_K:
            p_obj->p_pk_instance->DATA  =  0x00;
            p_obj->p_pk_instance->CR    =  0x00;
            p_obj->p_pk_instance->FR1   =  0x00;
            p_obj->p_pk_instance->FR2   =  0x00;
            p_obj->p_pk_instance->FR3   =  0x00;
            p_obj->p_pk_instance->FR4   =  0x00;
            p_obj->p_pk_instance->FR6   =  0x00;
            p_obj->p_pk_instance->FR7   =  0x00;
            p_obj->p_pk_instance->OD    =  0x00;
            p_obj->p_pk_instance->PUP   =  0x00;
            p_obj->p_pk_instance->PDN   =  0x00;
            p_obj->p_pk_instance->IE    =  0x00;
            /* PK Clock Disable */
            TSB_CG_FSYSMENB_IPMENB11 = 0U;
            break;
        case GPIO_PORT_L:
            p_obj->p_pl_instance->DATA  =  0x00;
            p_obj->p_pl_instance->CR    =  0x00;
            p_obj->p_pl_instance->FR1   =  0x00;
            p_obj->p_pl_instance->FR2   =  0x00;
            p_obj->p_pl_instance->FR3   =  0x00;
            p_obj->p_pl_instance->FR6   =  0x00;
            p_obj->p_pl_instance->FR7   =  0x00;
            p_obj->p_pl_instance->OD    =  0x00;
            p_obj->p_pl_instance->PUP   =  0x00;
            p_obj->p_pl_instance->PDN   =  0x00;
            p_obj->p_pl_instance->IE    =  0x00;
            /* PL Clock Disable */
            TSB_CG_FSYSMENB_IPMENB12 = 0U;
            break;
        case GPIO_PORT_N:
            p_obj->p_pn_instance->DATA  =  0x00;
            p_obj->p_pn_instance->CR    =  0x00;
            p_obj->p_pn_instance->OD    =  0x00;
            p_obj->p_pn_instance->PUP   =  0x00;
            p_obj->p_pn_instance->PDN   =  0x00;
            p_obj->p_pn_instance->IE    =  0x00;
            /* PN Clock Disable */
            TSB_CG_FSYSMENB_IPMENB14 = 0U;
            break;
        case GPIO_PORT_P:
            p_obj->p_pp_instance->DATA  =  0x00;
            p_obj->p_pp_instance->CR    =  0x00;
            p_obj->p_pp_instance->FR2   =  0x00;
            p_obj->p_pp_instance->FR3   =  0x00;
            p_obj->p_pp_instance->FR5   =  0x00;
            p_obj->p_pp_instance->OD    =  0x00;
            p_obj->p_pp_instance->PUP   =  0x00;
            p_obj->p_pp_instance->PDN   =  0x00;
            p_obj->p_pp_instance->IE    =  0x00;
            /* PP Clock Disable */
            TSB_CG_FSYSMENB_IPMENB15 = 0U;
            break;
        case GPIO_PORT_R:
            p_obj->p_pr_instance->DATA  =  0x00;
            p_obj->p_pr_instance->CR    =  0x00;
            p_obj->p_pr_instance->FR2   =  0x00;
            p_obj->p_pr_instance->FR3   =  0x00;
            p_obj->p_pr_instance->OD    =  0x00;
            p_obj->p_pr_instance->PUP   =  0x00;
            p_obj->p_pr_instance->PDN   =  0x00;
            p_obj->p_pr_instance->IE    =  0x00;
            /* PR Clock Disable */
            TSB_CG_FSYSMENB_IPMENB16 = 0U;
            break;
        case GPIO_PORT_T:
            p_obj->p_pt_instance->DATA  =  0x00;
            p_obj->p_pt_instance->CR    =  0x00;
            p_obj->p_pt_instance->FR1   =  0x00;
            p_obj->p_pt_instance->FR2   =  0x00;
            p_obj->p_pt_instance->FR3   =  0x00;
            p_obj->p_pt_instance->FR6   =  0x00;
            p_obj->p_pt_instance->FR7   =  0x00;
            p_obj->p_pt_instance->OD    =  0x00;
            p_obj->p_pt_instance->PUP   =  0x00;
            p_obj->p_pt_instance->PDN   =  0x00;
            p_obj->p_pt_instance->IE    =  0x00;
            /* PT Clock Disable */
            TSB_CG_FSYSMENB_IPMENB17 = 0U;
            break;
        case GPIO_PORT_V:
            p_obj->p_pv_instance->DATA  =  0x00;
            p_obj->p_pv_instance->CR    =  0x00;
            p_obj->p_pv_instance->FR2   =  0x00;
            p_obj->p_pv_instance->FR3   =  0x00;
            p_obj->p_pv_instance->FR4   =  0x00;
            p_obj->p_pv_instance->FR5   =  0x00;
            p_obj->p_pv_instance->FR6   =  0x00;
            p_obj->p_pv_instance->FR7   =  0x00;
            p_obj->p_pv_instance->OD    =  0x00;
            p_obj->p_pv_instance->PUP   =  0x00;
            p_obj->p_pv_instance->PDN   =  0x00;
            p_obj->p_pv_instance->IE    =  0x00;
            /* PV Clock Disable */
            TSB_CG_FSYSMENB_IPMENB19 = 0U;
            break;
        case GPIO_PORT_Y:
            p_obj->p_py_instance->DATA  =  0x00;
            p_obj->p_py_instance->CR    =  0x00;
            p_obj->p_py_instance->FR1   =  0x00;
            p_obj->p_py_instance->FR4   =  0x00;
            p_obj->p_py_instance->OD    =  0x00;
            p_obj->p_py_instance->PUP   =  0x00;
            p_obj->p_py_instance->PDN   =  0x00;
            p_obj->p_py_instance->IE    =  0x00;
            /* PY Clock Disable */
            TSB_CG_FSYSMENB_IPMENB21 = 0U;
            break;
#endif  /* TMPM4G7 */
#if defined(TMPM4G8)
        case GPIO_PORT_A:
            p_obj->p_pa_instance->DATA  =  0x00;
            p_obj->p_pa_instance->CR    =  0x00;
            p_obj->p_pa_instance->FR1   =  0x00;
            p_obj->p_pa_instance->FR2   =  0x00;
            p_obj->p_pa_instance->FR3   =  0x00;
            p_obj->p_pa_instance->FR5   =  0x00;
            p_obj->p_pa_instance->FR6   =  0x00;
            p_obj->p_pa_instance->FR7   =  0x00;
            p_obj->p_pa_instance->OD    =  0x00;
            p_obj->p_pa_instance->PUP   =  0x00;
            p_obj->p_pa_instance->PDN   =  0x00;
            p_obj->p_pa_instance->IE    =  0x00;
            /* PA Clock Disable */
            TSB_CG_FSYSMENB_IPMENB02 = 0U;
            break;
        case GPIO_PORT_B:
            p_obj->p_pb_instance->DATA  =  0x00;
            p_obj->p_pb_instance->CR    =  0x00;
            p_obj->p_pb_instance->FR1   =  0x00;
            p_obj->p_pb_instance->FR2   =  0x00;
            p_obj->p_pb_instance->FR3   =  0x00;
            p_obj->p_pb_instance->FR5   =  0x00;
            p_obj->p_pb_instance->FR6   =  0x00;
            p_obj->p_pb_instance->OD    =  0x00;
            p_obj->p_pb_instance->PUP   =  0x00;
            p_obj->p_pb_instance->PDN   =  0x00;
            p_obj->p_pb_instance->IE    =  0x00;
            /* PB Clock Disable */
            TSB_CG_FSYSMENB_IPMENB03 = 0U;
            break;
        case GPIO_PORT_C:
            p_obj->p_pc_instance->DATA  =  0x00;
            p_obj->p_pc_instance->CR    =  0x00;
            p_obj->p_pc_instance->FR1   =  0x00;
            p_obj->p_pc_instance->FR3   =  0x00;
            p_obj->p_pc_instance->FR5   =  0x00;
            p_obj->p_pc_instance->OD    =  0x00;
            p_obj->p_pc_instance->PUP   =  0x00;
            p_obj->p_pc_instance->PDN   =  0x00;
            p_obj->p_pc_instance->IE    =  0x00;
            /* PC Clock Disable */
            TSB_CG_FSYSMENB_IPMENB04 = 0U;
            break;
        case GPIO_PORT_D:
            p_obj->p_pd_instance->DATA  =  0x00;
            p_obj->p_pd_instance->CR    =  0x00;
            p_obj->p_pd_instance->FR1   =  0x00;
            p_obj->p_pd_instance->FR2   =  0x00;
            p_obj->p_pd_instance->FR3   =  0x00;
            p_obj->p_pd_instance->FR4   =  0x00;
            p_obj->p_pd_instance->FR5   =  0x00;
            p_obj->p_pd_instance->FR6   =  0x00;
            p_obj->p_pd_instance->FR7   =  0x00;
            p_obj->p_pd_instance->OD    =  0x00;
            p_obj->p_pd_instance->PUP   =  0x00;
            p_obj->p_pd_instance->PDN   =  0x00;
            p_obj->p_pd_instance->IE    =  0x00;
            /* PD Clock Disable */
            TSB_CG_FSYSMENB_IPMENB05 = 0U;
            break;
        case GPIO_PORT_E:
            p_obj->p_pe_instance->DATA  =  0x00;
            p_obj->p_pe_instance->CR    =  0x00;
            p_obj->p_pe_instance->FR1   =  0x00;
            p_obj->p_pe_instance->FR2   =  0x00;
            p_obj->p_pe_instance->FR3   =  0x00;
            p_obj->p_pe_instance->FR4   =  0x00;
            p_obj->p_pe_instance->FR5   =  0x00;
            p_obj->p_pe_instance->FR7   =  0x00;
            p_obj->p_pe_instance->OD    =  0x00;
            p_obj->p_pe_instance->PUP   =  0x00;
            p_obj->p_pe_instance->PDN   =  0x00;
            p_obj->p_pe_instance->IE    =  0x00;
            /* PE Clock Disable */
            TSB_CG_FSYSMENB_IPMENB06 = 0U;
            break;
        case GPIO_PORT_F:
            p_obj->p_pf_instance->DATA  =  0x00;
            p_obj->p_pf_instance->CR    =  0x00;
            p_obj->p_pf_instance->FR1   =  0x00;
            p_obj->p_pf_instance->FR7   =  0x00;
            p_obj->p_pf_instance->OD    =  0x00;
            p_obj->p_pf_instance->PUP   =  0x00;
            p_obj->p_pf_instance->PDN   =  0x00;
            p_obj->p_pf_instance->IE    =  0x00;
            /* PF Clock Disable */
            TSB_CG_FSYSMENB_IPMENB07= 0U;
            break;
        case GPIO_PORT_G:
            p_obj->p_pg_instance->DATA  =  0x00;
            p_obj->p_pg_instance->CR    =  0x00;
            p_obj->p_pg_instance->FR1   =  0x00;
            p_obj->p_pg_instance->FR2   =  0x00;
            p_obj->p_pg_instance->FR3   =  0x00;
            p_obj->p_pg_instance->FR4   =  0x00;
            p_obj->p_pg_instance->FR5   =  0x00;
            p_obj->p_pg_instance->FR7   =  0x00;
            p_obj->p_pg_instance->OD    =  0x00;
            p_obj->p_pg_instance->PUP   =  0x00;
            p_obj->p_pg_instance->PDN   =  0x00;
            p_obj->p_pg_instance->IE    =  0x00;
            /* PG Clock Disable */
            TSB_CG_FSYSMENB_IPMENB08 = 0U;
            break;
        case GPIO_PORT_H:
            p_obj->p_ph_instance->DATA  =  0x00;
            p_obj->p_ph_instance->CR    =  0x50;
            p_obj->p_ph_instance->FR1   =  0xF0;
            p_obj->p_ph_instance->FR3   =  0x00;
            p_obj->p_ph_instance->FR4   =  0x00;
            p_obj->p_ph_instance->FR5   =  0x00;
            p_obj->p_ph_instance->OD    =  0x00;
            p_obj->p_ph_instance->PUP   =  0x98;
            p_obj->p_ph_instance->PDN   =  0x20;
            p_obj->p_ph_instance->IE    =  0xB8;
            /* PH Clock Disable */
            TSB_CG_FSYSMENB_IPMENB09 = 0U;
            break;
        case GPIO_PORT_K:
            p_obj->p_pk_instance->DATA  =  0x00;
            p_obj->p_pk_instance->CR    =  0x00;
            p_obj->p_pk_instance->FR1   =  0x00;
            p_obj->p_pk_instance->FR2   =  0x00;
            p_obj->p_pk_instance->FR3   =  0x00;
            p_obj->p_pk_instance->FR4   =  0x00;
            p_obj->p_pk_instance->FR6   =  0x00;
            p_obj->p_pk_instance->FR7   =  0x00;
            p_obj->p_pk_instance->OD    =  0x00;
            p_obj->p_pk_instance->PUP   =  0x00;
            p_obj->p_pk_instance->PDN   =  0x00;
            p_obj->p_pk_instance->IE    =  0x00;
            /* PK Clock Disable */
            TSB_CG_FSYSMENB_IPMENB11 = 0U;
            break;
        case GPIO_PORT_L:
            p_obj->p_pl_instance->DATA  =  0x00;
            p_obj->p_pl_instance->CR    =  0x00;
            p_obj->p_pl_instance->FR1   =  0x00;
            p_obj->p_pl_instance->FR2   =  0x00;
            p_obj->p_pl_instance->FR3   =  0x00;
            p_obj->p_pl_instance->FR6   =  0x00;
            p_obj->p_pl_instance->FR7   =  0x00;
            p_obj->p_pl_instance->OD    =  0x00;
            p_obj->p_pl_instance->PUP   =  0x00;
            p_obj->p_pl_instance->PDN   =  0x00;
            p_obj->p_pl_instance->IE    =  0x00;
            /* PL Clock Disable */
            TSB_CG_FSYSMENB_IPMENB12 = 0U;
            break;
        case GPIO_PORT_M:
            p_obj->p_pm_instance->DATA  =  0x00;
            p_obj->p_pm_instance->CR    =  0x00;
            p_obj->p_pm_instance->FR2   =  0x00;
            p_obj->p_pm_instance->FR3   =  0x00;
            p_obj->p_pm_instance->FR4   =  0x00;
            p_obj->p_pm_instance->FR5   =  0x00;
            p_obj->p_pm_instance->FR6   =  0x00;
            p_obj->p_pm_instance->FR7   =  0x00;
            p_obj->p_pm_instance->OD    =  0x00;
            p_obj->p_pm_instance->PUP   =  0x00;
            p_obj->p_pm_instance->PDN   =  0x00;
            p_obj->p_pm_instance->IE    =  0x00;
            /* PM Clock Disable */
            TSB_CG_FSYSMENB_IPMENB13 = 0U;
            break;
        case GPIO_PORT_N:
            p_obj->p_pn_instance->DATA  =  0x00;
            p_obj->p_pn_instance->CR    =  0x00;
            p_obj->p_pn_instance->OD    =  0x00;
            p_obj->p_pn_instance->PUP   =  0x00;
            p_obj->p_pn_instance->PDN   =  0x00;
            p_obj->p_pn_instance->IE    =  0x00;
            /* PN Clock Disable */
            TSB_CG_FSYSMENB_IPMENB14 = 0U;
            break;
        case GPIO_PORT_P:
            p_obj->p_pp_instance->DATA  =  0x00;
            p_obj->p_pp_instance->CR    =  0x00;
            p_obj->p_pp_instance->FR2   =  0x00;
            p_obj->p_pp_instance->FR3   =  0x00;
            p_obj->p_pp_instance->FR5   =  0x00;
            p_obj->p_pp_instance->OD    =  0x00;
            p_obj->p_pp_instance->PUP   =  0x00;
            p_obj->p_pp_instance->PDN   =  0x00;
            p_obj->p_pp_instance->IE    =  0x00;
            /* PP Clock Disable */
            TSB_CG_FSYSMENB_IPMENB15 = 0U;
            break;
        case GPIO_PORT_R:
            p_obj->p_pr_instance->DATA  =  0x00;
            p_obj->p_pr_instance->CR    =  0x00;
            p_obj->p_pr_instance->FR2   =  0x00;
            p_obj->p_pr_instance->FR3   =  0x00;
            p_obj->p_pr_instance->OD    =  0x00;
            p_obj->p_pr_instance->PUP   =  0x00;
            p_obj->p_pr_instance->PDN   =  0x00;
            p_obj->p_pr_instance->IE    =  0x00;
            /* PR Clock Disable */
            TSB_CG_FSYSMENB_IPMENB16 = 0U;
            break;
        case GPIO_PORT_T:
            p_obj->p_pt_instance->DATA  =  0x00;
            p_obj->p_pt_instance->CR    =  0x00;
            p_obj->p_pt_instance->FR1   =  0x00;
            p_obj->p_pt_instance->FR2   =  0x00;
            p_obj->p_pt_instance->FR3   =  0x00;
            p_obj->p_pt_instance->FR6   =  0x00;
            p_obj->p_pt_instance->FR7   =  0x00;
            p_obj->p_pt_instance->OD    =  0x00;
            p_obj->p_pt_instance->PUP   =  0x00;
            p_obj->p_pt_instance->PDN   =  0x00;
            p_obj->p_pt_instance->IE    =  0x00;
            /* PT Clock Disable */
            TSB_CG_FSYSMENB_IPMENB17 = 0U;
            break;
        case GPIO_PORT_V:
            p_obj->p_pv_instance->DATA  =  0x00;
            p_obj->p_pv_instance->CR    =  0x00;
            p_obj->p_pv_instance->FR2   =  0x00;
            p_obj->p_pv_instance->FR3   =  0x00;
            p_obj->p_pv_instance->FR4   =  0x00;
            p_obj->p_pv_instance->FR5   =  0x00;
            p_obj->p_pv_instance->FR6   =  0x00;
            p_obj->p_pv_instance->FR7   =  0x00;
            p_obj->p_pv_instance->OD    =  0x00;
            p_obj->p_pv_instance->PUP   =  0x00;
            p_obj->p_pv_instance->PDN   =  0x00;
            p_obj->p_pv_instance->IE    =  0x00;
            /* PV Clock Disable */
            TSB_CG_FSYSMENB_IPMENB19 = 0U;
            break;
        case GPIO_PORT_Y:
            p_obj->p_py_instance->DATA  =  0x00;
            p_obj->p_py_instance->CR    =  0x00;
            p_obj->p_py_instance->FR1   =  0x00;
            p_obj->p_py_instance->FR4   =  0x00;
            p_obj->p_py_instance->OD    =  0x00;
            p_obj->p_py_instance->PUP   =  0x00;
            p_obj->p_py_instance->PDN   =  0x00;
            p_obj->p_py_instance->IE    =  0x00;
            /* PY Clock Disable */
            TSB_CG_FSYSMENB_IPMENB21 = 0U;
            break;
#endif  /* TMPM4G8 */
#if defined(TMPM4G9)
        case GPIO_PORT_A:
            p_obj->p_pa_instance->DATA  =  0x00;
            p_obj->p_pa_instance->CR    =  0x00;
            p_obj->p_pa_instance->FR1   =  0x00;
            p_obj->p_pa_instance->FR2   =  0x00;
            p_obj->p_pa_instance->FR3   =  0x00;
            p_obj->p_pa_instance->FR5   =  0x00;
            p_obj->p_pa_instance->FR6   =  0x00;
            p_obj->p_pa_instance->FR7   =  0x00;
            p_obj->p_pa_instance->OD    =  0x00;
            p_obj->p_pa_instance->PUP   =  0x00;
            p_obj->p_pa_instance->PDN   =  0x00;
            p_obj->p_pa_instance->IE    =  0x00;
            /* PA Clock Disable */
            TSB_CG_FSYSMENB_IPMENB02 = 0U;
            break;
        case GPIO_PORT_B:
            p_obj->p_pb_instance->DATA  =  0x00;
            p_obj->p_pb_instance->CR    =  0x00;
            p_obj->p_pb_instance->FR1   =  0x00;
            p_obj->p_pb_instance->FR2   =  0x00;
            p_obj->p_pb_instance->FR3   =  0x00;
            p_obj->p_pb_instance->FR5   =  0x00;
            p_obj->p_pb_instance->FR6   =  0x00;
            p_obj->p_pb_instance->OD    =  0x00;
            p_obj->p_pb_instance->PUP   =  0x00;
            p_obj->p_pb_instance->PDN   =  0x00;
            p_obj->p_pb_instance->IE    =  0x00;
            /* PB Clock Disable */
            TSB_CG_FSYSMENB_IPMENB03 = 0U;
            break;
        case GPIO_PORT_C:
            p_obj->p_pc_instance->DATA  =  0x00;
            p_obj->p_pc_instance->CR    =  0x00;
            p_obj->p_pc_instance->FR1   =  0x00;
            p_obj->p_pc_instance->FR3   =  0x00;
            p_obj->p_pc_instance->FR5   =  0x00;
            p_obj->p_pc_instance->OD    =  0x00;
            p_obj->p_pc_instance->PUP   =  0x00;
            p_obj->p_pc_instance->PDN   =  0x00;
            p_obj->p_pc_instance->IE    =  0x00;
            /* PC Clock Disable */
            TSB_CG_FSYSMENB_IPMENB04 = 0U;
            break;
        case GPIO_PORT_D:
            p_obj->p_pd_instance->DATA  =  0x00;
            p_obj->p_pd_instance->CR    =  0x00;
            p_obj->p_pd_instance->FR1   =  0x00;
            p_obj->p_pd_instance->FR2   =  0x00;
            p_obj->p_pd_instance->FR3   =  0x00;
            p_obj->p_pd_instance->FR4   =  0x00;
            p_obj->p_pd_instance->FR5   =  0x00;
            p_obj->p_pd_instance->FR6   =  0x00;
            p_obj->p_pd_instance->FR7   =  0x00;
            p_obj->p_pd_instance->OD    =  0x00;
            p_obj->p_pd_instance->PUP   =  0x00;
            p_obj->p_pd_instance->PDN   =  0x00;
            p_obj->p_pd_instance->IE    =  0x00;
            /* PD Clock Disable */
            TSB_CG_FSYSMENB_IPMENB05 = 0U;
            break;
        case GPIO_PORT_E:
            p_obj->p_pe_instance->DATA  =  0x00;
            p_obj->p_pe_instance->CR    =  0x00;
            p_obj->p_pe_instance->FR1   =  0x00;
            p_obj->p_pe_instance->FR2   =  0x00;
            p_obj->p_pe_instance->FR3   =  0x00;
            p_obj->p_pe_instance->FR4   =  0x00;
            p_obj->p_pe_instance->FR5   =  0x00;
            p_obj->p_pe_instance->FR7   =  0x00;
            p_obj->p_pe_instance->OD    =  0x00;
            p_obj->p_pe_instance->PUP   =  0x00;
            p_obj->p_pe_instance->PDN   =  0x00;
            p_obj->p_pe_instance->IE    =  0x00;
            /* PE Clock Disable */
            TSB_CG_FSYSMENB_IPMENB06 = 0U;
            break;
        case GPIO_PORT_F:
            p_obj->p_pf_instance->DATA  =  0x00;
            p_obj->p_pf_instance->CR    =  0x00;
            p_obj->p_pf_instance->FR1   =  0x00;
            p_obj->p_pf_instance->FR7   =  0x00;
            p_obj->p_pf_instance->OD    =  0x00;
            p_obj->p_pf_instance->PUP   =  0x00;
            p_obj->p_pf_instance->PDN   =  0x00;
            p_obj->p_pf_instance->IE    =  0x00;
            /* PF Clock Disable */
            TSB_CG_FSYSMENB_IPMENB07= 0U;
            break;
        case GPIO_PORT_G:
            p_obj->p_pg_instance->DATA  =  0x00;
            p_obj->p_pg_instance->CR    =  0x00;
            p_obj->p_pg_instance->FR1   =  0x00;
            p_obj->p_pg_instance->FR2   =  0x00;
            p_obj->p_pg_instance->FR3   =  0x00;
            p_obj->p_pg_instance->FR4   =  0x00;
            p_obj->p_pg_instance->FR5   =  0x00;
            p_obj->p_pg_instance->FR7   =  0x00;
            p_obj->p_pg_instance->OD    =  0x00;
            p_obj->p_pg_instance->PUP   =  0x00;
            p_obj->p_pg_instance->PDN   =  0x00;
            p_obj->p_pg_instance->IE    =  0x00;
            /* PG Clock Disable */
            TSB_CG_FSYSMENB_IPMENB08 = 0U;
            break;
        case GPIO_PORT_H:
            p_obj->p_ph_instance->DATA  =  0x00;
            p_obj->p_ph_instance->CR    =  0x50;
            p_obj->p_ph_instance->FR1   =  0xF0;
            p_obj->p_ph_instance->FR3   =  0x00;
            p_obj->p_ph_instance->FR4   =  0x00;
            p_obj->p_ph_instance->FR5   =  0x00;
            p_obj->p_ph_instance->OD    =  0x00;
            p_obj->p_ph_instance->PUP   =  0x98;
            p_obj->p_ph_instance->PDN   =  0x20;
            p_obj->p_ph_instance->IE    =  0xB8;
            /* PH Clock Disable */
            TSB_CG_FSYSMENB_IPMENB09 = 0U;
            break;
        case GPIO_PORT_J:
            p_obj->p_pj_instance->DATA  =  0x00;
            p_obj->p_pj_instance->CR    =  0x00;
            p_obj->p_pj_instance->FR2   =  0x00;
            p_obj->p_pj_instance->FR3   =  0x00;
            p_obj->p_pj_instance->FR5   =  0x00;
            p_obj->p_pj_instance->FR7   =  0x00;
            p_obj->p_pj_instance->OD    =  0x00;
            p_obj->p_pj_instance->PUP   =  0x00;
            p_obj->p_pj_instance->PDN   =  0x00;
            p_obj->p_pj_instance->IE    =  0x00;
            /* PJ Clock Disable */
            TSB_CG_FSYSMENB_IPMENB10 = 0U;
            break;
        case GPIO_PORT_K:
            p_obj->p_pk_instance->DATA  =  0x00;
            p_obj->p_pk_instance->CR    =  0x00;
            p_obj->p_pk_instance->FR1   =  0x00;
            p_obj->p_pk_instance->FR2   =  0x00;
            p_obj->p_pk_instance->FR3   =  0x00;
            p_obj->p_pk_instance->FR4   =  0x00;
            p_obj->p_pk_instance->FR6   =  0x00;
            p_obj->p_pk_instance->FR7   =  0x00;
            p_obj->p_pk_instance->OD    =  0x00;
            p_obj->p_pk_instance->PUP   =  0x00;
            p_obj->p_pk_instance->PDN   =  0x00;
            p_obj->p_pk_instance->IE    =  0x00;
            /* PK Clock Disable */
            TSB_CG_FSYSMENB_IPMENB11 = 0U;
            break;
        case GPIO_PORT_L:
            p_obj->p_pl_instance->DATA  =  0x00;
            p_obj->p_pl_instance->CR    =  0x00;
            p_obj->p_pl_instance->FR1   =  0x00;
            p_obj->p_pl_instance->FR2   =  0x00;
            p_obj->p_pl_instance->FR3   =  0x00;
            p_obj->p_pl_instance->FR6   =  0x00;
            p_obj->p_pl_instance->FR7   =  0x00;
            p_obj->p_pl_instance->OD    =  0x00;
            p_obj->p_pl_instance->PUP   =  0x00;
            p_obj->p_pl_instance->PDN   =  0x00;
            p_obj->p_pl_instance->IE    =  0x00;
            /* PL Clock Disable */
            TSB_CG_FSYSMENB_IPMENB12 = 0U;
            break;
        case GPIO_PORT_M:
            p_obj->p_pm_instance->DATA  =  0x00;
            p_obj->p_pm_instance->CR    =  0x00;
            p_obj->p_pm_instance->FR2   =  0x00;
            p_obj->p_pm_instance->FR3   =  0x00;
            p_obj->p_pm_instance->FR4   =  0x00;
            p_obj->p_pm_instance->FR5   =  0x00;
            p_obj->p_pm_instance->FR6   =  0x00;
            p_obj->p_pm_instance->FR7   =  0x00;
            p_obj->p_pm_instance->OD    =  0x00;
            p_obj->p_pm_instance->PUP   =  0x00;
            p_obj->p_pm_instance->PDN   =  0x00;
            p_obj->p_pm_instance->IE    =  0x00;
            /* PM Clock Disable */
            TSB_CG_FSYSMENB_IPMENB13 = 0U;
            break;
        case GPIO_PORT_N:
            p_obj->p_pn_instance->DATA  =  0x00;
            p_obj->p_pn_instance->CR    =  0x00;
            p_obj->p_pn_instance->OD    =  0x00;
            p_obj->p_pn_instance->PUP   =  0x00;
            p_obj->p_pn_instance->PDN   =  0x00;
            p_obj->p_pn_instance->IE    =  0x00;
            /* PN Clock Disable */
            TSB_CG_FSYSMENB_IPMENB14 = 0U;
            break;
        case GPIO_PORT_P:
            p_obj->p_pp_instance->DATA  =  0x00;
            p_obj->p_pp_instance->CR    =  0x00;
            p_obj->p_pp_instance->FR2   =  0x00;
            p_obj->p_pp_instance->FR3   =  0x00;
            p_obj->p_pp_instance->FR5   =  0x00;
            p_obj->p_pp_instance->OD    =  0x00;
            p_obj->p_pp_instance->PUP   =  0x00;
            p_obj->p_pp_instance->PDN   =  0x00;
            p_obj->p_pp_instance->IE    =  0x00;
            /* PP Clock Disable */
            TSB_CG_FSYSMENB_IPMENB15 = 0U;
            break;
        case GPIO_PORT_R:
            p_obj->p_pr_instance->DATA  =  0x00;
            p_obj->p_pr_instance->CR    =  0x00;
            p_obj->p_pr_instance->FR2   =  0x00;
            p_obj->p_pr_instance->FR3   =  0x00;
            p_obj->p_pr_instance->OD    =  0x00;
            p_obj->p_pr_instance->PUP   =  0x00;
            p_obj->p_pr_instance->PDN   =  0x00;
            p_obj->p_pr_instance->IE    =  0x00;
            /* PR Clock Disable */
            TSB_CG_FSYSMENB_IPMENB16 = 0U;
            break;
        case GPIO_PORT_T:
            p_obj->p_pt_instance->DATA  =  0x00;
            p_obj->p_pt_instance->CR    =  0x00;
            p_obj->p_pt_instance->FR1   =  0x00;
            p_obj->p_pt_instance->FR2   =  0x00;
            p_obj->p_pt_instance->FR3   =  0x00;
            p_obj->p_pt_instance->FR6   =  0x00;
            p_obj->p_pt_instance->FR7   =  0x00;
            p_obj->p_pt_instance->OD    =  0x00;
            p_obj->p_pt_instance->PUP   =  0x00;
            p_obj->p_pt_instance->PDN   =  0x00;
            p_obj->p_pt_instance->IE    =  0x00;
            /* PT Clock Disable */
            TSB_CG_FSYSMENB_IPMENB17 = 0U;
            break;
        case GPIO_PORT_U:
            p_obj->p_pu_instance->DATA  =  0x00;
            p_obj->p_pu_instance->CR    =  0x00;
            p_obj->p_pu_instance->FR2   =  0x00;
            p_obj->p_pu_instance->FR3   =  0x00;
            p_obj->p_pu_instance->FR7   =  0x00;
            p_obj->p_pu_instance->OD    =  0x00;
            p_obj->p_pu_instance->PUP   =  0x00;
            p_obj->p_pu_instance->PDN   =  0x00;
            p_obj->p_pu_instance->IE    =  0x00;
            /* PU Clock Disable */
            TSB_CG_FSYSMENB_IPMENB18 = 0U;
            break;
        case GPIO_PORT_V:
            p_obj->p_pv_instance->DATA  =  0x00;
            p_obj->p_pv_instance->CR    =  0x00;
            p_obj->p_pv_instance->FR2   =  0x00;
            p_obj->p_pv_instance->FR3   =  0x00;
            p_obj->p_pv_instance->FR4   =  0x00;
            p_obj->p_pv_instance->FR5   =  0x00;
            p_obj->p_pv_instance->FR6   =  0x00;
            p_obj->p_pv_instance->FR7   =  0x00;
            p_obj->p_pv_instance->OD    =  0x00;
            p_obj->p_pv_instance->PUP   =  0x00;
            p_obj->p_pv_instance->PDN   =  0x00;
            p_obj->p_pv_instance->IE    =  0x00;
            /* PV Clock Disable */
            TSB_CG_FSYSMENB_IPMENB19 = 0U;
            break;
        case GPIO_PORT_W:
            p_obj->p_pw_instance->DATA  =  0x00;
            p_obj->p_pw_instance->CR    =  0x00;
            p_obj->p_pw_instance->FR3   =  0x00;
            p_obj->p_pw_instance->FR4   =  0x00;
            p_obj->p_pw_instance->FR5   =  0x00;
            p_obj->p_pw_instance->FR6   =  0x00;
            p_obj->p_pw_instance->FR7   =  0x00;
            p_obj->p_pw_instance->OD    =  0x00;
            p_obj->p_pw_instance->PUP   =  0x00;
            p_obj->p_pw_instance->PDN   =  0x00;
            p_obj->p_pw_instance->IE    =  0x00;
            /* PW Clock Disable */
            TSB_CG_FSYSMENB_IPMENB20 = 0U;
            break;
        case GPIO_PORT_Y:
            p_obj->p_py_instance->DATA  =  0x00;
            p_obj->p_py_instance->CR    =  0x00;
            p_obj->p_py_instance->FR1   =  0x00;
            p_obj->p_py_instance->FR4   =  0x00;
            p_obj->p_py_instance->OD    =  0x00;
            p_obj->p_py_instance->PUP   =  0x00;
            p_obj->p_py_instance->PDN   =  0x00;
            p_obj->p_py_instance->IE    =  0x00;
            /* PY Clock Disable */
            TSB_CG_FSYSMENB_IPMENB21 = 0U;
            break;
#endif  /* TMPM4G9 */
        default:
            result = TXZ_ERROR;
            return (result);
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result gpio_write_mode(_gpio_t *p_obj, uint32_t group, uint32_t mode, uint32_t val)
  * @brief  Port Mode Write
  * @param[in] p_obj :GPIO object.
  * @param[in] group :GPIO Port Group. : Use @ref gpio_gr_t
  * @param[in] mode  :GPIO Port Mode. : Use @ref gpio_num_t
  * @param[in] val   :value
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   When p_obj is NULL, no processing.
  */
/*--------------------------------------------------*/
TXZ_Result gpio_write_mode(_gpio_t *p_obj, uint32_t group, uint32_t mode, uint32_t val)
{
    TXZ_Result result = TXZ_SUCCESS;
    int32_t i;
    int32_t param_result = PARAM_NG;

    /* Check the parameters, the NULL of address */
    for (i = GPIO_PORT_0; i < GPIO_PORT_Max; i++)
    {
        param_result = check_param_pin_exist(p_obj, group, (uint32_t)i, mode);
        if(param_result == PARAM_OK)
        { 
            break;
        }
        else
        {
             result = TXZ_ERROR;
        }
    }
    if (((void*)(p_obj) == (void*)0) || (param_result == PARAM_NG))
    {
        result = TXZ_ERROR;
    }
    else
    {
        switch (group)
        {
#if defined(TMPM4G6)
        case GPIO_PORT_A:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pa_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pa_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pa_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pa_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pa_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pa_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pa_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pa_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pa_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pa_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pa_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pa_instance->IE   = val;}
            break;
        case GPIO_PORT_B:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pb_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pb_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pb_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pb_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pb_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pb_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pb_instance->FR6  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pb_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pb_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pb_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pb_instance->IE   = val;}
            break;
        case GPIO_PORT_D:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pd_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pd_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pd_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pd_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pd_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pd_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pd_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pd_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pd_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pd_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pd_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pd_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pd_instance->IE   = val;}
            break;
        case GPIO_PORT_E:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pe_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pe_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pe_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pd_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pd_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pd_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pd_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pe_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pe_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pe_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pe_instance->IE   = val;}
            break;
        case GPIO_PORT_F:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pf_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pf_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pf_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pf_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pf_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pf_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pf_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pf_instance->IE   = val;}
            break;
        case GPIO_PORT_G:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pg_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pg_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pg_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pg_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pg_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pg_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pg_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pg_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pg_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pg_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pg_instance->IE  = val;}
            break;
        case GPIO_PORT_H:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_ph_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_ph_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_ph_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_ph_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_ph_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_ph_instance->FR5 = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_ph_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_ph_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_ph_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_ph_instance->IE   = val;}
            break;
        case GPIO_PORT_K:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pk_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pk_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pk_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pk_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pk_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pk_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pk_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pk_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pk_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pk_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pk_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pk_instance->IE   = val;}
            break;
        case GPIO_PORT_L:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pl_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pl_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pl_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pl_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pl_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pl_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pl_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pl_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pl_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pl_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pl_instance->IE   = val;}
            break;
        case GPIO_PORT_N:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pn_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pn_instance->CR   = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pn_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pn_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pn_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pn_instance->IE   = val;}
            break;
        case GPIO_PORT_P:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pp_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pp_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pp_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pp_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pp_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pp_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pp_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pp_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pp_instance->IE   = val;}
            break;
        case GPIO_PORT_T:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pt_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pt_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pt_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pt_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pt_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pt_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pt_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pt_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pt_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pt_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pt_instance->IE   = val;}
            break;
        case GPIO_PORT_Y:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_py_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_py_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_py_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_py_instance->FR4  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_py_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_py_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_py_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_py_instance->IE   = val;}
            break;
#endif  /* TMPM4G6 */
#if defined(TMPM4G7)
        case GPIO_PORT_A:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pa_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pa_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pa_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pa_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pa_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pa_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pa_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pa_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pa_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pa_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pa_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pa_instance->IE   = val;}
            break;
        case GPIO_PORT_B:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pb_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pb_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pb_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pb_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pb_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pb_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pb_instance->FR6  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pb_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pb_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pb_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pb_instance->IE   = val;}
            break;
        case GPIO_PORT_C:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pc_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pc_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pc_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pc_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pc_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pc_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pc_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pc_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pc_instance->IE   = val;}
            break; 
        case GPIO_PORT_D:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pd_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pd_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pd_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pd_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pd_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pd_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pd_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pd_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pd_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pd_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pd_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pd_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pd_instance->IE   = val;}
            break;
        case GPIO_PORT_E:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pe_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pe_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pe_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pd_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pd_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pd_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pd_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pe_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pe_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pe_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pe_instance->IE   = val;}
            break;
        case GPIO_PORT_F:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pf_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pf_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pf_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pf_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pf_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pf_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pf_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pf_instance->IE   = val;}
            break;
        case GPIO_PORT_G:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pg_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pg_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pg_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pg_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pg_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pg_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pg_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pg_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pg_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pg_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pg_instance->IE  = val;}
            break;
        case GPIO_PORT_H:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_ph_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_ph_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_ph_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_ph_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_ph_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_ph_instance->FR5 = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_ph_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_ph_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_ph_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_ph_instance->IE   = val;}
            break;
        case GPIO_PORT_K:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pk_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pk_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pk_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pk_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pk_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pk_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pk_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pk_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pk_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pk_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pk_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pk_instance->IE   = val;}
            break;
        case GPIO_PORT_L:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pl_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pl_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pl_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pl_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pl_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pl_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pl_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pl_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pl_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pl_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pl_instance->IE   = val;}
            break;
        case GPIO_PORT_N:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pn_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pn_instance->CR   = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pn_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pn_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pn_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pn_instance->IE   = val;}
            break;
        case GPIO_PORT_P:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pp_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pp_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pp_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pp_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pp_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pp_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pp_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pp_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pp_instance->IE   = val;}
            break;
        case GPIO_PORT_R:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pr_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pr_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pr_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pr_instance->FR3  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pr_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pr_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pr_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pr_instance->IE   = val;}
            break;
        case GPIO_PORT_T:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pt_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pt_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pt_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pt_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pt_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pt_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pt_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pt_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pt_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pt_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pt_instance->IE   = val;}
            break;
        case GPIO_PORT_V:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pv_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pv_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pv_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pv_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pv_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pv_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pv_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pv_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pv_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pv_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pv_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pv_instance->IE   = val;}
            break;
        case GPIO_PORT_Y:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_py_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_py_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_py_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_py_instance->FR4  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_py_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_py_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_py_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_py_instance->IE   = val;}
            break;
#endif  /* TMPM4G7 */
#if defined(TMPM4G8)
        case GPIO_PORT_A:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pa_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pa_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pa_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pa_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pa_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pa_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pa_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pa_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pa_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pa_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pa_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pa_instance->IE   = val;}
            break;
        case GPIO_PORT_B:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pb_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pb_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pb_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pb_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pb_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pb_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pb_instance->FR6  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pb_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pb_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pb_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pb_instance->IE   = val;}
            break;
        case GPIO_PORT_C:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pc_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pc_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pc_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pc_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pc_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pc_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pc_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pc_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pc_instance->IE   = val;}
            break; 
        case GPIO_PORT_D:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pd_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pd_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pd_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pd_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pd_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pd_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pd_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pd_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pd_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pd_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pd_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pd_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pd_instance->IE   = val;}
            break;
        case GPIO_PORT_E:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pe_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pe_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pe_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pd_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pd_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pd_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pd_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pe_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pe_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pe_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pe_instance->IE   = val;}
            break;
        case GPIO_PORT_F:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pf_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pf_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pf_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pf_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pf_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pf_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pf_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pf_instance->IE   = val;}
            break;
        case GPIO_PORT_G:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pg_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pg_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pg_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pg_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pg_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pg_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pg_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pg_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pg_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pg_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pg_instance->IE  = val;}
            break;
        case GPIO_PORT_H:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_ph_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_ph_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_ph_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_ph_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_ph_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_ph_instance->FR5 = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_ph_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_ph_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_ph_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_ph_instance->IE   = val;}
            break;
        case GPIO_PORT_K:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pk_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pk_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pk_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pk_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pk_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pk_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pk_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pk_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pk_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pk_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pk_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pk_instance->IE   = val;}
            break;
        case GPIO_PORT_L:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pl_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pl_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pl_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pl_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pl_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pl_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pl_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pl_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pl_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pl_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pl_instance->IE   = val;}
            break;
        case GPIO_PORT_M:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pm_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pm_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pm_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pm_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pm_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pm_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pm_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pm_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pm_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pm_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pm_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pm_instance->IE   = val;}
            break;
        case GPIO_PORT_N:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pn_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pn_instance->CR   = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pn_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pn_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pn_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pn_instance->IE   = val;}
            break;
        case GPIO_PORT_P:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pp_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pp_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pp_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pp_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pp_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pp_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pp_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pp_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pp_instance->IE   = val;}
            break;
        case GPIO_PORT_R:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pr_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pr_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pr_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pr_instance->FR3  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pr_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pr_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pr_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pr_instance->IE   = val;}
            break;
        case GPIO_PORT_T:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pt_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pt_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pt_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pt_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pt_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pt_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pt_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pt_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pt_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pt_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pt_instance->IE   = val;}
            break;
        case GPIO_PORT_V:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pv_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pv_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pv_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pv_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pv_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pv_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pv_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pv_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pv_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pv_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pv_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pv_instance->IE   = val;}
            break;
        case GPIO_PORT_Y:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_py_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_py_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_py_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_py_instance->FR4  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_py_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_py_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_py_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_py_instance->IE   = val;}
            break;
#endif  /* TMPM4G8 */
#if defined(TMPM4G9)
        case GPIO_PORT_A:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pa_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pa_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pa_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pa_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pa_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pa_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pa_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pa_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pa_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pa_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pa_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pa_instance->IE   = val;}
            break;
        case GPIO_PORT_B:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pb_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pb_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pb_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pb_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pb_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pb_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pb_instance->FR6  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pb_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pb_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pb_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pb_instance->IE   = val;}
            break;
        case GPIO_PORT_C:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pc_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pc_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pc_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pc_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pc_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pc_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pc_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pc_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pc_instance->IE   = val;}
            break; 
        case GPIO_PORT_D:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pd_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pd_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pd_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pd_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pd_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pd_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pd_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pd_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pd_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pd_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pd_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pd_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pd_instance->IE   = val;}
            break;
        case GPIO_PORT_E:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pe_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pe_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pe_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pd_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pd_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pd_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pd_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pe_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pe_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pe_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pe_instance->IE   = val;}
            break;
        case GPIO_PORT_F:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pf_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pf_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pf_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pf_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pf_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pf_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pf_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pf_instance->IE   = val;}
            break;
        case GPIO_PORT_G:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pg_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pg_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pg_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pg_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pg_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pg_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pg_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pg_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pg_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pg_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pg_instance->IE  = val;}
            break;
        case GPIO_PORT_H:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_ph_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_ph_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_ph_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_ph_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_ph_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_ph_instance->FR5 = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_ph_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_ph_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_ph_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_ph_instance->IE   = val;}
            break;
        case GPIO_PORT_J:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pj_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pj_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pj_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pj_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pj_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pj_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pj_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pj_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pj_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pj_instance->IE   = val;}
            break;
        case GPIO_PORT_K:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pk_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pk_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pk_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pk_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pk_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pk_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pk_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pk_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pk_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pk_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pk_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pk_instance->IE   = val;}
            break;
        case GPIO_PORT_L:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pl_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pl_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pl_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pl_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pl_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pl_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pl_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pl_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pl_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pl_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pl_instance->IE   = val;}
            break;
        case GPIO_PORT_M:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pm_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pm_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pm_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pm_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pm_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pm_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pm_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pm_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pm_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pm_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pm_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pm_instance->IE   = val;}
            break;
        case GPIO_PORT_N:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pn_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pn_instance->CR   = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pn_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pn_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pn_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pn_instance->IE   = val;}
            break;
        case GPIO_PORT_P:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pp_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pp_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pp_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pp_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pp_instance->FR5  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pp_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pp_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pp_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pp_instance->IE   = val;}
            break;
        case GPIO_PORT_R:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pr_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pr_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pr_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pr_instance->FR3  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pr_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pr_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pr_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pr_instance->IE   = val;}
            break;
        case GPIO_PORT_T:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pt_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pt_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_pt_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pt_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pt_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pt_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pt_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pt_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pt_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pt_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pt_instance->IE   = val;}
            break;
        case GPIO_PORT_U:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pu_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pu_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pu_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pu_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pu_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pu_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pu_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pu_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pu_instance->IE   = val;}
            break;
        case GPIO_PORT_V:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pv_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pv_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR2) {p_obj->p_pv_instance->FR2  = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pv_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pv_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pv_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pv_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pv_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pv_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pv_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pv_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pv_instance->IE   = val;}
            break;
        case GPIO_PORT_W:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_pw_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_pw_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR3) {p_obj->p_pw_instance->FR3  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_pw_instance->FR4  = val;}
            else if(mode == GPIO_Mode_FR5) {p_obj->p_pw_instance->FR5  = val;}
            else if(mode == GPIO_Mode_FR6) {p_obj->p_pw_instance->FR6  = val;}
            else if(mode == GPIO_Mode_FR7) {p_obj->p_pw_instance->FR7  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_pw_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_pw_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_pw_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_pw_instance->IE   = val;}
            break;
        case GPIO_PORT_Y:
            if(mode == GPIO_Mode_DATA)     {p_obj->p_py_instance->DATA = val;}
            else if(mode == GPIO_Mode_CR)  {p_obj->p_py_instance->CR   = val;}
            else if(mode == GPIO_Mode_FR1) {p_obj->p_py_instance->FR1  = val;}
            else if(mode == GPIO_Mode_FR4) {p_obj->p_py_instance->FR4  = val;}
            else if(mode == GPIO_Mode_OD)  {p_obj->p_py_instance->OD   = val;}
            else if(mode == GPIO_Mode_PUP) {p_obj->p_py_instance->PUP  = val;}
            else if(mode == GPIO_Mode_PDN) {p_obj->p_py_instance->PDN  = val;}
            else if(mode == GPIO_Mode_IE)  {p_obj->p_py_instance->IE   = val;}
            break;
#endif  /* TMPM4G9 */
        default:
            result = TXZ_ERROR;
            return (result);
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result gpio_read_mode(_gpio_t *p_obj, uint32_t group, uint32_t mode, uint32_t *val)
  * @brief  Port Mode Read
  * @param[in]  p_obj :GPIO object.
  * @param[in]  group :GPIO Port Group.  : Use @ref gpio_gr_t
  * @param[in]  mode  :GPIO Port Mode. : Use @ref gpio_num_t
  * @param[out] val   :Store of value
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   When p_obj is NULL, no processing.
  */
/*--------------------------------------------------*/
TXZ_Result gpio_read_mode(_gpio_t *p_obj, uint32_t group, uint32_t mode, uint32_t *val)
{
    TXZ_Result result = TXZ_SUCCESS;
    int32_t param_result = PARAM_NG;
    int32_t i;

    /* Check the parameters, the NULL of address */
    for (i = GPIO_PORT_0; i < GPIO_PORT_Max; i++ )
    {
        param_result = check_param_pin_exist(p_obj, group, (uint32_t)i, mode);
        if(param_result == PARAM_OK)
        { 
            break;
        }
        else
        {
             result = TXZ_ERROR;
        }
    }
    if (((void*)(p_obj) == (void*)0) || (param_result == PARAM_NG))
    {
        result = TXZ_ERROR;
    }
    else
    {
        switch (group)
        {
#if defined(TMPM4G6)
        case GPIO_PORT_A:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pa_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pa_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pa_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pa_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pa_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pa_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pa_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pa_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pa_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pa_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pa_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pa_instance->IE;}
            break;
        case GPIO_PORT_B:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pb_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pb_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pb_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pb_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pb_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pb_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pb_instance->FR6;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pb_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pb_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pb_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pb_instance->IE;}
            break;
        case GPIO_PORT_D:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pd_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pd_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pd_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pd_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pd_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pd_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pd_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pd_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pd_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pd_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pd_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pd_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pd_instance->IE;}
            break;
        case GPIO_PORT_E:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pe_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pe_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pe_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pe_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pe_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pe_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pe_instance->FR5;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pe_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pe_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pe_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pe_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pe_instance->IE;}
            break;
        case GPIO_PORT_F:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pf_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pf_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pf_instance->FR1;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pf_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pf_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pf_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pf_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pf_instance->IE;}
            break;
        case GPIO_PORT_G:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pg_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pg_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pg_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pg_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pg_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pg_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pg_instance->FR5;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pg_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pg_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pg_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pg_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pg_instance->IE;}
            break;
        case GPIO_PORT_H:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_ph_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_ph_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_ph_instance->FR1;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_ph_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_ph_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_ph_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_ph_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_ph_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_ph_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_ph_instance->IE;}
            break;
        case GPIO_PORT_K:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pk_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pk_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pk_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pk_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pk_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pk_instance->FR4;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pk_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pk_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pk_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pk_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pk_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pk_instance->IE;}
            break;
        case GPIO_PORT_L:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pl_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pl_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pl_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pl_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pl_instance->FR3;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pl_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pl_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pl_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pl_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pl_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pl_instance->IE;}
            break;
        case GPIO_PORT_N:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pn_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pn_instance->CR;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pn_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pn_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pn_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pn_instance->IE;}
            break;
        case GPIO_PORT_P:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pp_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pp_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pp_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pp_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pp_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pp_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pp_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pp_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pp_instance->IE;}
            break;
        case GPIO_PORT_T:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pt_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pt_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pt_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pt_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pt_instance->FR3;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pt_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pt_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pt_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pt_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pt_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pt_instance->IE;}
            break;
        case GPIO_PORT_Y:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_py_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_py_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_py_instance->FR1;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_py_instance->FR4;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_py_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_py_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_py_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_py_instance->IE;}
            break;
#endif  /* TMPM4G6 */
#if defined(TMPM4G7)
        case GPIO_PORT_A:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pa_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pa_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pa_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pa_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pa_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pa_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pa_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pa_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pa_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pa_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pa_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pa_instance->IE;}
            break;
        case GPIO_PORT_B:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pb_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pb_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pb_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pb_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pb_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pb_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pb_instance->FR6;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pb_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pb_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pb_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pb_instance->IE;}
            break;
        case GPIO_PORT_C:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pc_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pc_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pc_instance->FR1;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pc_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pc_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pc_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pc_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pc_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pc_instance->IE;}
            break; 
        case GPIO_PORT_D:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pd_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pd_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pd_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pd_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pd_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pd_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pd_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pd_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pd_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pd_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pd_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pd_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pd_instance->IE;}
            break;
        case GPIO_PORT_E:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pe_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pe_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pe_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pe_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pe_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pe_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pe_instance->FR5;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pe_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pe_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pe_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pe_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pe_instance->IE;}
            break;
        case GPIO_PORT_F:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pf_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pf_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pf_instance->FR1;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pf_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pf_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pf_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pf_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pf_instance->IE;}
            break;
        case GPIO_PORT_G:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pg_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pg_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pg_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pg_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pg_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pg_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pg_instance->FR5;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pg_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pg_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pg_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pg_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pg_instance->IE;}
            break;
        case GPIO_PORT_H:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_ph_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_ph_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_ph_instance->FR1;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_ph_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_ph_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_ph_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_ph_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_ph_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_ph_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_ph_instance->IE;}
            break;
        case GPIO_PORT_K:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pk_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pk_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pk_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pk_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pk_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pk_instance->FR4;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pk_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pk_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pk_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pk_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pk_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pk_instance->IE;}
            break;
        case GPIO_PORT_L:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pl_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pl_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pl_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pl_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pl_instance->FR3;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pl_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pl_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pl_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pl_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pl_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pl_instance->IE;}
            break;
        case GPIO_PORT_N:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pn_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pn_instance->CR;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pn_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pn_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pn_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pn_instance->IE;}
            break;
        case GPIO_PORT_P:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pp_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pp_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pp_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pp_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pp_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pp_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pp_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pp_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pp_instance->IE;}
            break;
        case GPIO_PORT_R:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pr_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pr_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pr_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pr_instance->FR3;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pr_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pr_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pr_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pr_instance->IE;}
            break;
        case GPIO_PORT_T:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pt_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pt_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pt_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pt_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pt_instance->FR3;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pt_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pt_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pt_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pt_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pt_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pt_instance->IE;}
            break;
        case GPIO_PORT_V:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pv_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pv_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pv_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pv_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pv_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pv_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pv_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pv_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pv_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pv_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pv_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pv_instance->IE;}
            break;
        case GPIO_PORT_Y:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_py_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_py_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_py_instance->FR1;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_py_instance->FR4;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_py_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_py_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_py_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_py_instance->IE;}
            break;
#endif  /* TMPM4G7 */
#if defined(TMPM4G8)
        case GPIO_PORT_A:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pa_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pa_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pa_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pa_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pa_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pa_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pa_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pa_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pa_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pa_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pa_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pa_instance->IE;}
            break;
        case GPIO_PORT_B:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pb_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pb_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pb_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pb_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pb_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pb_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pb_instance->FR6;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pb_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pb_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pb_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pb_instance->IE;}
            break;
        case GPIO_PORT_C:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pc_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pc_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pc_instance->FR1;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pc_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pc_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pc_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pc_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pc_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pc_instance->IE;}
            break; 
        case GPIO_PORT_D:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pd_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pd_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pd_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pd_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pd_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pd_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pd_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pd_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pd_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pd_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pd_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pd_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pd_instance->IE;}
            break;
        case GPIO_PORT_E:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pe_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pe_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pe_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pe_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pe_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pe_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pe_instance->FR5;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pe_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pe_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pe_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pe_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pe_instance->IE;}
            break;
        case GPIO_PORT_F:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pf_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pf_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pf_instance->FR1;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pf_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pf_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pf_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pf_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pf_instance->IE;}
            break;
        case GPIO_PORT_G:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pg_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pg_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pg_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pg_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pg_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pg_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pg_instance->FR5;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pg_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pg_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pg_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pg_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pg_instance->IE;}
            break;
        case GPIO_PORT_H:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_ph_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_ph_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_ph_instance->FR1;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_ph_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_ph_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_ph_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_ph_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_ph_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_ph_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_ph_instance->IE;}
            break;
        case GPIO_PORT_K:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pk_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pk_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pk_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pk_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pk_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pk_instance->FR4;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pk_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pk_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pk_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pk_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pk_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pk_instance->IE;}
            break;
        case GPIO_PORT_L:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pl_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pl_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pl_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pl_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pl_instance->FR3;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pl_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pl_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pl_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pl_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pl_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pl_instance->IE;}
            break;
        case GPIO_PORT_M:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pm_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pm_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pm_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pm_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pm_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pm_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pm_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pm_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pm_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pm_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pm_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pm_instance->IE;}
            break;
        case GPIO_PORT_N:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pn_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pn_instance->CR;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pn_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pn_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pn_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pn_instance->IE;}
            break;
        case GPIO_PORT_P:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pp_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pp_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pp_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pp_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pp_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pp_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pp_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pp_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pp_instance->IE;}
            break;
        case GPIO_PORT_R:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pr_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pr_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pr_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pr_instance->FR3;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pr_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pr_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pr_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pr_instance->IE;}
            break;
        case GPIO_PORT_T:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pt_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pt_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pt_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pt_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pt_instance->FR3;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pt_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pt_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pt_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pt_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pt_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pt_instance->IE;}
            break;
        case GPIO_PORT_V:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pv_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pv_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pv_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pv_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pv_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pv_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pv_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pv_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pv_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pv_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pv_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pv_instance->IE;}
            break;
        case GPIO_PORT_Y:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_py_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_py_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_py_instance->FR1;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_py_instance->FR4;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_py_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_py_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_py_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_py_instance->IE;}
            break;
#endif  /* TMPM4G8 */
#if defined(TMPM4G9)
        case GPIO_PORT_A:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pa_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pa_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pa_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pa_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pa_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pa_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pa_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pa_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pa_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pa_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pa_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pa_instance->IE;}
            break;
        case GPIO_PORT_B:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pb_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pb_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pb_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pb_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pb_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pb_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pb_instance->FR6;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pb_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pb_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pb_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pb_instance->IE;}
            break;
        case GPIO_PORT_C:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pc_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pc_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pc_instance->FR1;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pc_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pc_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pc_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pc_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pc_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pc_instance->IE;}
            break; 
        case GPIO_PORT_D:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pd_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pd_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pd_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pd_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pd_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pd_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pd_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pd_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pd_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pd_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pd_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pd_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pd_instance->IE;}
            break;
        case GPIO_PORT_E:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pe_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pe_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pe_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pe_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pe_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pe_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pe_instance->FR5;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pe_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pe_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pe_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pe_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pe_instance->IE;}
            break;
        case GPIO_PORT_F:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pf_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pf_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pf_instance->FR1;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pf_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pf_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pf_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pf_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pf_instance->IE;}
            break;
        case GPIO_PORT_G:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pg_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pg_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pg_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pg_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pg_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pg_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pg_instance->FR5;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pg_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pg_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pg_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pg_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pg_instance->IE;}
            break;
        case GPIO_PORT_H:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_ph_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_ph_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_ph_instance->FR1;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_ph_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_ph_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_ph_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_ph_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_ph_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_ph_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_ph_instance->IE;}
            break;
        case GPIO_PORT_J:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pj_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pj_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pj_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pj_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pj_instance->FR5;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pj_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pj_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pj_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pj_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pj_instance->IE;}
            break;
        case GPIO_PORT_K:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pk_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pk_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pk_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pk_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pk_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pk_instance->FR4;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pk_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pk_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pk_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pk_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pk_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pk_instance->IE;}
            break;
        case GPIO_PORT_L:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pl_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pl_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pl_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pl_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pl_instance->FR3;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pl_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pl_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pl_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pl_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pl_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pl_instance->IE;}
            break;
        case GPIO_PORT_M:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pm_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pm_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pm_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pm_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pm_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pm_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pm_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pm_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pm_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pm_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pm_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pm_instance->IE;}
            break;
        case GPIO_PORT_N:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pn_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pn_instance->CR;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pn_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pn_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pn_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pn_instance->IE;}
            break;
        case GPIO_PORT_P:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pp_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pp_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pp_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pp_instance->FR3;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pp_instance->FR5;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pp_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pp_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pp_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pp_instance->IE;}
            break;
        case GPIO_PORT_R:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pr_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pr_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pr_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pr_instance->FR3;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pr_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pr_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pr_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pr_instance->IE;}
            break;
        case GPIO_PORT_T:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pt_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pt_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_pt_instance->FR1;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pt_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pt_instance->FR3;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pt_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pt_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pt_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pt_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pt_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pt_instance->IE;}
            break;
        case GPIO_PORT_U:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pu_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pu_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pu_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pu_instance->FR3;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pu_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pu_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pu_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pu_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pu_instance->IE;}
            break;
        case GPIO_PORT_V:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pv_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pv_instance->CR;}
            else if(mode == GPIO_Mode_FR2) {*val = p_obj->p_pv_instance->FR2;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pv_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pv_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pv_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pv_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pv_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pv_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pv_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pv_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pv_instance->IE;}
            break;
        case GPIO_PORT_W:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_pw_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_pw_instance->CR;}
            else if(mode == GPIO_Mode_FR3) {*val = p_obj->p_pw_instance->FR3;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_pw_instance->FR4;}
            else if(mode == GPIO_Mode_FR5) {*val = p_obj->p_pw_instance->FR5;}
            else if(mode == GPIO_Mode_FR6) {*val = p_obj->p_pw_instance->FR6;}
            else if(mode == GPIO_Mode_FR7) {*val = p_obj->p_pw_instance->FR7;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_pw_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_pw_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_pw_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_pw_instance->IE;}
            break;
        case GPIO_PORT_Y:
            if(mode == GPIO_Mode_DATA)     {*val = p_obj->p_py_instance->DATA;}
            else if(mode == GPIO_Mode_CR)  {*val = p_obj->p_py_instance->CR;}
            else if(mode == GPIO_Mode_FR1) {*val = p_obj->p_py_instance->FR1;}
            else if(mode == GPIO_Mode_FR4) {*val = p_obj->p_py_instance->FR4;}
            else if(mode == GPIO_Mode_OD)  {*val = p_obj->p_py_instance->OD;}
            else if(mode == GPIO_Mode_PUP) {*val = p_obj->p_py_instance->PUP;}
            else if(mode == GPIO_Mode_PDN) {*val = p_obj->p_py_instance->PDN;}
            else if(mode == GPIO_Mode_IE)  {*val = p_obj->p_py_instance->IE;}
            break;
#endif  /* TMPM4G9 */
        default:
            result = TXZ_ERROR;
        }
    }

    return result;
}

/*--------------------------------------------------*/
/**
  * @brief  Port Function switching
  * @param  p_obj  :GPIO object.
  * @param  group  :GPIO Port Group.  : Use @ref gpio_gr_t
  * @param  num    :GPIO Port Number. : Use @ref gpio_num_t
  * @param  func   :GPIO Portxx Func. : Use @ref gpio_pa0_func_t - @ref gpio_pl4_func_t
  * @param  inout  :GPIO Input/Output.: Use @ref gpio_pininout_t
  * @retval GPIO_RESULT_SUCCESS :Success.
  * @retval GPIO_RESULT_FAILURE :Failure.
  * @note   When p_obj is NULL, no processing.
  */
/*--------------------------------------------------*/
TXZ_Result gpio_func(_gpio_t *p_obj, gpio_gr_t group, gpio_num_t num, uint32_t func, gpio_pininout_t inout)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t port_base;
    uint32_t mode_base;
    int32_t param_result = PARAM_NG;
    
    /* Check the parameters, the NULL of address */
    param_result = check_param_func_pin_exist(p_obj, group, num, func);
    if (((void*)(p_obj) == (void*)0) || (param_result == PARAM_NG))
    {
        result = TXZ_ERROR;
    }
    else
    {
        switch (group)
        {
        #if defined(TMPM4G6)
        case GPIO_PORT_A:
            port_base = (uint32_t)p_obj->p_pa_instance;
            break;
        case GPIO_PORT_B:
            port_base = (uint32_t)p_obj->p_pb_instance;
            break;
        case GPIO_PORT_D:
            port_base = (uint32_t)p_obj->p_pd_instance;
            break;
        case GPIO_PORT_E:
            port_base = (uint32_t)p_obj->p_pe_instance;
            break;
        case GPIO_PORT_F:
            port_base = (uint32_t)p_obj->p_pf_instance;
            break;
        case GPIO_PORT_G:
            port_base = (uint32_t)p_obj->p_pg_instance;
            break;
        case GPIO_PORT_H:
            port_base = (uint32_t)p_obj->p_ph_instance;
            break;
        case GPIO_PORT_K:
            port_base = (uint32_t)p_obj->p_pk_instance;
            break;
        case GPIO_PORT_L:
            port_base = (uint32_t)p_obj->p_pl_instance;
            break;
        case GPIO_PORT_N:
            port_base = (uint32_t)p_obj->p_pn_instance;
            break;
        case GPIO_PORT_P:
            port_base = (uint32_t)p_obj->p_pp_instance;
            break;
        case GPIO_PORT_T:
            port_base = (uint32_t)p_obj->p_pt_instance;
            break;
        case GPIO_PORT_Y:
            port_base = (uint32_t)p_obj->p_py_instance;
            break;
        #endif  /* TMPM4G6 */
        #if defined(TMPM4G7)
        case GPIO_PORT_A:
            port_base = (uint32_t)p_obj->p_pa_instance;
            break;
        case GPIO_PORT_B:
            port_base = (uint32_t)p_obj->p_pb_instance;
            break;
        case GPIO_PORT_C:
            port_base = (uint32_t)p_obj->p_pc_instance;
            break; 
        case GPIO_PORT_D:
            port_base = (uint32_t)p_obj->p_pd_instance;
            break;
        case GPIO_PORT_E:
            port_base = (uint32_t)p_obj->p_pe_instance;
            break;
        case GPIO_PORT_F:
            port_base = (uint32_t)p_obj->p_pf_instance;
            break;
        case GPIO_PORT_G:
            port_base = (uint32_t)p_obj->p_pg_instance;
            break;
        case GPIO_PORT_H:
            port_base = (uint32_t)p_obj->p_ph_instance;
            break;
        case GPIO_PORT_K:
            port_base = (uint32_t)p_obj->p_pk_instance;
            break;
        case GPIO_PORT_L:
            port_base = (uint32_t)p_obj->p_pl_instance;
            break;
        case GPIO_PORT_N:
            port_base = (uint32_t)p_obj->p_pn_instance;
            break;
        case GPIO_PORT_P:
            port_base = (uint32_t)p_obj->p_pp_instance;
            break;
        case GPIO_PORT_R:
            port_base = (uint32_t)p_obj->p_pr_instance;
            break;
        case GPIO_PORT_T:
            port_base = (uint32_t)p_obj->p_pt_instance;
            break;
        case GPIO_PORT_V:
            port_base = (uint32_t)p_obj->p_pv_instance;
            break;
        case GPIO_PORT_Y:
            port_base = (uint32_t)p_obj->p_py_instance;
            break;
        #endif  /* TMPM4G7 */
        #if defined(TMPM4G8)
        case GPIO_PORT_A:
            port_base = (uint32_t)p_obj->p_pa_instance;
            break;
        case GPIO_PORT_B:
            port_base = (uint32_t)p_obj->p_pb_instance;
            break;
        case GPIO_PORT_C:
            port_base = (uint32_t)p_obj->p_pc_instance;
            break; 
        case GPIO_PORT_D:
            port_base = (uint32_t)p_obj->p_pd_instance;
            break;
        case GPIO_PORT_E:
            port_base = (uint32_t)p_obj->p_pe_instance;
            break;
        case GPIO_PORT_F:
            port_base = (uint32_t)p_obj->p_pf_instance;
            break;
        case GPIO_PORT_G:
            port_base = (uint32_t)p_obj->p_pg_instance;
            break;
        case GPIO_PORT_H:
            port_base = (uint32_t)p_obj->p_ph_instance;
            break;
        case GPIO_PORT_K:
            port_base = (uint32_t)p_obj->p_pk_instance;
            break;
        case GPIO_PORT_L:
            port_base = (uint32_t)p_obj->p_pl_instance;
            break;
        case GPIO_PORT_M:
            port_base = (uint32_t)p_obj->p_pm_instance;
            break;
        case GPIO_PORT_N:
            port_base = (uint32_t)p_obj->p_pn_instance;
            break;
        case GPIO_PORT_P:
            port_base = (uint32_t)p_obj->p_pp_instance;
            break;
        case GPIO_PORT_R:
            port_base = (uint32_t)p_obj->p_pr_instance;
            break;
        case GPIO_PORT_T:
            port_base = (uint32_t)p_obj->p_pt_instance;
            break;
        case GPIO_PORT_V:
            port_base = (uint32_t)p_obj->p_pv_instance;
            break;
        case GPIO_PORT_Y:
            port_base = (uint32_t)p_obj->p_py_instance;
            break;
        #endif  /* TMPM4G8 */
        #if defined(TMPM4G9)
        case GPIO_PORT_A:
            port_base = (uint32_t)p_obj->p_pa_instance;
            break;
        case GPIO_PORT_B:
            port_base = (uint32_t)p_obj->p_pb_instance;
            break;
        case GPIO_PORT_C:
            port_base = (uint32_t)p_obj->p_pc_instance;
            break; 
        case GPIO_PORT_D:
            port_base = (uint32_t)p_obj->p_pd_instance;
            break;
        case GPIO_PORT_E:
            port_base = (uint32_t)p_obj->p_pe_instance;
            break;
        case GPIO_PORT_F:
            port_base = (uint32_t)p_obj->p_pf_instance;
            break;
        case GPIO_PORT_G:
            port_base = (uint32_t)p_obj->p_pg_instance;
            break;
        case GPIO_PORT_H:
            port_base = (uint32_t)p_obj->p_ph_instance;
            break;
        case GPIO_PORT_J:
            port_base = (uint32_t)p_obj->p_pj_instance;
            break;
        case GPIO_PORT_K:
            port_base = (uint32_t)p_obj->p_pk_instance;
            break;
        case GPIO_PORT_L:
            port_base = (uint32_t)p_obj->p_pl_instance;
            break;
        case GPIO_PORT_M:
            port_base = (uint32_t)p_obj->p_pm_instance;
            break;
        case GPIO_PORT_N:
            port_base = (uint32_t)p_obj->p_pn_instance;
            break;
        case GPIO_PORT_P:
            port_base = (uint32_t)p_obj->p_pp_instance;
            break;
        case GPIO_PORT_R:
            port_base = (uint32_t)p_obj->p_pr_instance;
            break;
        case GPIO_PORT_T:
            port_base = (uint32_t)p_obj->p_pt_instance;
            break;
        case GPIO_PORT_U:
            port_base = (uint32_t)p_obj->p_pu_instance;
            break;
        case GPIO_PORT_V:
            port_base = (uint32_t)p_obj->p_pv_instance;
            break;
        case GPIO_PORT_W:
            port_base = (uint32_t)p_obj->p_pw_instance;
            break;
        case GPIO_PORT_Y:
            port_base = (uint32_t)p_obj->p_py_instance;
            break;
        #endif  /* TMPM4G9 */
        default:
            result = TXZ_ERROR;
        }
        if(result == TXZ_ERROR)
        {
            return (result);
        }

        /* Initialization PxFR OFF */    
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR1);
        BITBAND_PORT_CLR(mode_base, num);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR2);
        BITBAND_PORT_CLR(mode_base, num);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR3);
        BITBAND_PORT_CLR(mode_base, num);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR4);
        BITBAND_PORT_CLR(mode_base, num);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR5);
        BITBAND_PORT_CLR(mode_base, num);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR6);
        BITBAND_PORT_CLR(mode_base, num);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR7);
        BITBAND_PORT_CLR(mode_base, num);
        
        /* Initialize Input/Output */
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
        BITBAND_PORT_CLR(mode_base, num);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
        BITBAND_PORT_CLR(mode_base, num);
    
        switch (func) {
            case 0:
                if(inout== GPIO_PIN_OUTPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_INPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                }else if(inout== GPIO_PIN_INOUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_NOTINOUT){
                    /* No Process */
                }
                break;
            case GPIO_FR_1:
                mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR1);
                BITBAND_PORT_SET(mode_base, num);
                if(inout== GPIO_PIN_OUTPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_INPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                }else if(inout== GPIO_PIN_INOUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_NOTINOUT){
                    /* No Process */
                }
                break;
            case GPIO_FR_2:
                mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR2);
                BITBAND_PORT_SET(mode_base, num);
                if(inout== GPIO_PIN_OUTPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_INPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                }else if(inout== GPIO_PIN_INOUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_NOTINOUT){
                    /* No Process */
                }
                break;
            case GPIO_FR_3:
                mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR3);
                BITBAND_PORT_SET(mode_base, num);
                if(inout== GPIO_PIN_OUTPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_INPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                }else if(inout== GPIO_PIN_INOUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_NOTINOUT){
                    /* No Process */
                }
                break;
             case GPIO_FR_4:
                mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR4);
                BITBAND_PORT_SET(mode_base, num);
                if(inout== GPIO_PIN_OUTPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_INPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                }else if(inout== GPIO_PIN_INOUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_NOTINOUT){
                    /* No Process */
                }
                break;
             case GPIO_FR_5:
                mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR5);
                BITBAND_PORT_SET(mode_base, num);
                if(inout== GPIO_PIN_OUTPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_INPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                }else if(inout== GPIO_PIN_INOUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }
                break;
             case GPIO_FR_6:
                mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR6);
                BITBAND_PORT_SET(mode_base, num);
                if(inout== GPIO_PIN_OUTPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_INPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                }else if(inout== GPIO_PIN_INOUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_NOTINOUT){
                    /* No Process */
                }
                break;
             case GPIO_FR_7:
                mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR7);
                BITBAND_PORT_SET(mode_base, num);
                if(inout== GPIO_PIN_OUTPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_INPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                }else if(inout== GPIO_PIN_INOUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_NOTINOUT){
                    /* No Process */
                }
                break;
            case GPIO_FR_NA:
                if(inout== GPIO_PIN_OUTPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_INPUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                }else if(inout== GPIO_PIN_INOUT){
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }else if(inout== GPIO_PIN_NOTINOUT){
                    /* No Process */
                }
                break;
            default:
                result = TXZ_ERROR;
                return (result);
         }
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Set Pull up mode
  * @param  p_obj  :GPIO object.
  * @param  group  :GPIO Port Group.  : Use @ref gpio_gr_t
  * @param  num    :GPIO Port Number. : Use @ref gpio_num_t
  * @param  val    :GPIO Pin Reset/Set. : Use @ref gpio_pinstate_t
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   When p_obj is NULL, no processing.
  */
/*--------------------------------------------------*/
TXZ_Result gpio_SetPullUp(_gpio_t *p_obj, gpio_gr_t group, gpio_num_t num, gpio_pinstate_t val)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t port_base;
    uint32_t mode_base;

    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else if (check_param_pin_exist(p_obj, group, num, GPIO_Mode_PUP) == PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    else
    {
        port_base = BITBAND_PORT_BASE(group);

        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_PUP);
        if (val == GPIO_PIN_SET)
        {
            BITBAND_PORT_SET(mode_base, num);
        }
        else if (val == GPIO_PIN_RESET)
        {
            BITBAND_PORT_CLR(mode_base, num);
        }
        else{ result = TXZ_ERROR;}
    }

    return result;
}

/*--------------------------------------------------*/
/**
  * @brief  Set Pull down mode
  * @param  p_obj  :GPIO object.
  * @param  group  :GPIO Port Group.  : Use @ref gpio_gr_t
  * @param  num    :GPIO Port Number. : Use @ref gpio_num_t
  * @param  val    :GPIO Pin Reset/Set. : Use @ref gpio_pinstate_t
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   When p_obj is NULL, no processing.
  */
/*--------------------------------------------------*/
TXZ_Result gpio_SetPullDown(_gpio_t *p_obj, gpio_gr_t group, gpio_num_t num, gpio_pinstate_t val)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t port_base;
    uint32_t mode_base;

    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else if (check_param_pin_exist(p_obj, group, num, GPIO_Mode_PDN) == PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    else
    {
        port_base = BITBAND_PORT_BASE(group);

        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_PDN);
        if (val == GPIO_PIN_SET)
        {
            BITBAND_PORT_SET(mode_base, num);
        }
        else if (val == GPIO_PIN_RESET)
        {
            BITBAND_PORT_CLR(mode_base, num);
        }
        else{ result = TXZ_ERROR;}
    }

    return result;
}

/*--------------------------------------------------*/
/**
  * @brief  Set Open drain mode
  * @param  p_obj  :GPIO object.
  * @param  group  :GPIO Port Group.  : Use @ref gpio_gr_t
  * @param  num    :GPIO Port Number. : Use @ref gpio_num_t
  * @param  val    :GPIO Pin Reset/Set. : Use @ref gpio_pinstate_t
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   When p_obj is NULL, no processing.
  */
/*--------------------------------------------------*/
TXZ_Result gpio_SetOpenDrain(_gpio_t *p_obj, gpio_gr_t group, gpio_num_t num, gpio_pinstate_t val)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t port_base;
    uint32_t mode_base;

    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else if (check_param_pin_exist(p_obj, group, num, GPIO_Mode_OD) == PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    else
    {
        port_base = BITBAND_PORT_BASE(group);

        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_OD);
        if (val == GPIO_PIN_SET)
        {
            BITBAND_PORT_SET(mode_base, num);
        }
        else if (val == GPIO_PIN_RESET) 
        {
            BITBAND_PORT_CLR(mode_base, num);
        }
        else{ result = TXZ_ERROR;}
    }

    return result;
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result gpio_write_bit(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode, uint32_t val)
  * @brief  Port Bit Write
  * @param  p_obj  :GPIO object.
  * @param  group  :GPIO Port Group.  : Use @ref gpio_gr_t
  * @param  num    :GPIO Port Number. : Use @ref gpio_num_t
  * @param  mode   :GPIO Port Mode. : Use @ref gpio_mode_t
  * @param  val    :GPIO Pin Reset/Set.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   When p_obj is NULL, no processing.
  */
/*--------------------------------------------------*/
TXZ_Result gpio_write_bit(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode, uint32_t val)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t base;

    /* Check the parameters */
    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else if (check_param_pin_exist(p_obj, group, num, mode) == PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    else
    {
        base = BITBAND_PORT_BASE(group);
        base = BITBAND_PORT_MODE_BASE(base, mode);
        if (val == GPIO_PIN_SET){ BITBAND_PORT_SET(base, num);}
        else if (val == GPIO_PIN_RESET){ BITBAND_PORT_CLR(base, num);}
        else{ result = TXZ_ERROR;}
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result gpio_read_bit(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode, gpio_pinstate_t *pinstate)
  * @brief  Port Bit Read
  * @param[in]  p_obj  :GPIO object.
  * @param[in]  group  :GPIO Port Group.  : Use @ref gpio_gr_t
  * @param[in]  num    :GPIO Port Number. : Use @ref gpio_num_t
  * @param[in]  mode   :GPIO Port Mode. : Use @ref gpio_mode_t
  * @param[out] *pinstate  : store  Value of GPIO BitPin. : Use @ref gpio_pinstate_t
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   When p_obj is NULL, no processing.: Use @ref gpio_pinstate_t
  */
/*--------------------------------------------------*/
TXZ_Result gpio_read_bit(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode, gpio_pinstate_t *pinstate)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t base;
    uint32_t val;

    /* Check the parameters */   
    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else if (check_param_pin_exist(p_obj, group, num, mode) == PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    else
    {
        base = BITBAND_PORT_BASE(group);
        base = BITBAND_PORT_MODE_BASE(base, mode);
        BITBAND_PORT_READ(val, base, num);
        if(val == GPIO_PIN_RESET){ *pinstate = GPIO_PIN_RESET;}
        else if (val == GPIO_PIN_SET){ *pinstate = GPIO_PIN_SET;}
        else{ result = TXZ_ERROR;}
    }

    return result;
}

/**
 *  @}
 */ /* End of group GPIO_Exported_functions */

/**
 *  @} 
 */ /* End of group GPIO */

/**
 *  @} 
 */ /* End of group Periph_Driver */

#endif /* defined(__GPIO_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


