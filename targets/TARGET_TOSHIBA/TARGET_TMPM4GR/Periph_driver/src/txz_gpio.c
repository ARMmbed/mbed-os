/**
 *******************************************************************************
 * @file    txz_gpio.c
 * @brief   This file provides API functions for GPIO driver.
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
  * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
#define BITBAND_PORT_SET(base, bitnum) (*((__IO uint32_t *)base) |= (uint32_t)(0x0000001UL<< bitnum)) /*!< Target Pin Bit set */
#define BITBAND_PORT_CLR(base, bitnum) (*((__IO uint32_t *)base) &= ~((uint32_t)(0x0000001UL<< bitnum))) /*!< Target Pin Bit clear */
#define BITBAND_PORT_READ(val, base, bitnum) val = ((*((__IO uint32_t *)base) & (uint32_t)(0x0000001UL<< bitnum)) >> bitnum) /*!< Target Pin Bit read */
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

/* no define */

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

static uint8_t change_mode_to_num(uint32_t mode);
static uint8_t change_func_to_num(uint32_t func);
static int32_t check_param_pin_exist(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode);
static int32_t check_param_func_pin_exist(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t func);

static uint8_t change_mode_to_num(uint32_t mode)
{
    uint8_t retVal = 0;

    if (mode == GPIO_Mode_DATA)     {
        retVal = GPIO_ModeNum_DATA;
    } else if (mode == GPIO_Mode_CR)  {
        retVal = GPIO_ModeNum_CR;
    } else if (mode == GPIO_Mode_FR1) {
        retVal = GPIO_ModeNum_FR1;
    } else if (mode == GPIO_Mode_FR2) {
        retVal = GPIO_ModeNum_FR2;
    } else if (mode == GPIO_Mode_FR3) {
        retVal = GPIO_ModeNum_FR3;
    } else if (mode == GPIO_Mode_FR4) {
        retVal = GPIO_ModeNum_FR4;
    } else if (mode == GPIO_Mode_FR5) {
        retVal = GPIO_ModeNum_FR5;
    } else if (mode == GPIO_Mode_FR6) {
        retVal = GPIO_ModeNum_FR6;
    } else if (mode == GPIO_Mode_FR7) {
        retVal = GPIO_ModeNum_FR7;
    } else if (mode == GPIO_Mode_FR8) {
        retVal = GPIO_ModeNum_FR8;
    } else if (mode == GPIO_Mode_OD)  {
        retVal = GPIO_ModeNum_OD;
    } else if (mode == GPIO_Mode_PUP) {
        retVal = GPIO_ModeNum_PUP;
    } else if (mode == GPIO_Mode_PDN) {
        retVal = GPIO_ModeNum_PDN;
    } else if (mode == GPIO_Mode_IE)  {
        retVal = GPIO_ModeNum_IE;
    } else {
        retVal = GPIO_ModeNum_Max;
    }

    return retVal;
}

static uint8_t change_func_to_num(uint32_t func)
{
    uint8_t retVal = 0;

    if (func == GPIO_FR_1)     {
        retVal = GPIO_ModeNum_FR1;
    } else if (func == GPIO_FR_2) {
        retVal = GPIO_ModeNum_FR2;
    } else if (func == GPIO_FR_3) {
        retVal = GPIO_ModeNum_FR3;
    } else if (func == GPIO_FR_4) {
        retVal = GPIO_ModeNum_FR4;
    } else if (func == GPIO_FR_5) {
        retVal = GPIO_ModeNum_FR5;
    } else if (func == GPIO_FR_6) {
        retVal = GPIO_ModeNum_FR6;
    } else if (func == GPIO_FR_7) {
        retVal = GPIO_ModeNum_FR7;
    } else if (func == GPIO_FR_8) {
        retVal = GPIO_ModeNum_FR8;
    } else if (func == GPIO_FR_NA) {
        retVal = GPIO_ModeNum_CR;
    } else if (func == 0)         {
        retVal = GPIO_ModeNum_CR;
    } else {
        retVal = GPIO_ModeNum_Max;
    }

    return retVal;
}

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
static int32_t check_param_pin_exist(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode)
{
    int32_t result = PARAM_NG;
    uint8_t chgmode;
    uint16_t tmp;

    chgmode = change_mode_to_num(mode);
    if ((chgmode < GPIO_ModeNum_Max) && (group < GPIO_GROUP_Max) && (num < GPIO_PORT_Max)) {
        tmp = (PinExistTbl[group][num] >> chgmode) & 0x01;
        result = PARAM_OK;
        if (tmp == 0) {
            result = PARAM_NG;
        }
    } else {
        result = PARAM_NG;
    }

    return (result);
}
static int32_t check_param_func_pin_exist(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t func)
{
    int32_t result = PARAM_NG;
    uint8_t chgfunc;
    uint16_t tmp;

    chgfunc = change_func_to_num(func);
    /* param check skip if func is INPUT or OUTPUT */
    if (chgfunc == GPIO_ModeNum_CR) {
        return (PARAM_OK);
    }
    if ((chgfunc < GPIO_ModeNum_Max) && (group < GPIO_GROUP_Max) && (num < GPIO_PORT_Max)) {
        tmp = (PinExistTbl[group][num] >> chgfunc) & 0x01;
        result = PARAM_OK;
        if (tmp == 0) {
            result = PARAM_NG;
        }
    } else {
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
    if ((void *)(p_obj) == (void *)0) {
        result = TXZ_ERROR;
    } else {
        switch (group) {
            case GPIO_PORT_A:
                /* PA Clock Enable */
                PORT_CLOCK_ENABLE_PORTA(TSB_CG_FSYSMENB_IPMENB02);
                INIT_GPIO_PORT_A_DATA(p_obj->p_pa_instance->DATA);
                INIT_GPIO_PORT_A_CR(p_obj->p_pa_instance->CR);
                INIT_GPIO_PORT_A_FR1(p_obj->p_pa_instance->FR1);
                INIT_GPIO_PORT_A_FR2(p_obj->p_pa_instance->FR2);
                INIT_GPIO_PORT_A_FR3(p_obj->p_pa_instance->FR3);
                INIT_GPIO_PORT_A_FR4(p_obj->p_pa_instance->FR4);
                INIT_GPIO_PORT_A_FR5(p_obj->p_pa_instance->FR5);
                INIT_GPIO_PORT_A_FR6(p_obj->p_pa_instance->FR6);
                INIT_GPIO_PORT_A_FR7(p_obj->p_pa_instance->FR7);
                INIT_GPIO_PORT_A_FR8(p_obj->p_pa_instance->FR8);
                INIT_GPIO_PORT_A_OD(p_obj->p_pa_instance->OD);
                INIT_GPIO_PORT_A_PUP(p_obj->p_pa_instance->PUP);
                INIT_GPIO_PORT_A_PDN(p_obj->p_pa_instance->PDN);
                INIT_GPIO_PORT_A_IE(p_obj->p_pa_instance->IE);
                break;
            case GPIO_PORT_B:
                /* PB Clock Enable */
                PORT_CLOCK_ENABLE_PORTB(TSB_CG_FSYSMENB_IPMENB03);
                INIT_GPIO_PORT_B_DATA(p_obj->p_pb_instance->DATA);
                INIT_GPIO_PORT_B_CR(p_obj->p_pb_instance->CR);
                INIT_GPIO_PORT_B_FR1(p_obj->p_pb_instance->FR1);
                INIT_GPIO_PORT_B_FR2(p_obj->p_pb_instance->FR2);
                INIT_GPIO_PORT_B_FR3(p_obj->p_pb_instance->FR3);
                INIT_GPIO_PORT_B_FR4(p_obj->p_pb_instance->FR4);
                INIT_GPIO_PORT_B_FR5(p_obj->p_pb_instance->FR5);
                INIT_GPIO_PORT_B_FR6(p_obj->p_pb_instance->FR6);
                INIT_GPIO_PORT_B_FR7(p_obj->p_pb_instance->FR7);
                INIT_GPIO_PORT_B_FR8(p_obj->p_pb_instance->FR8);
                INIT_GPIO_PORT_B_OD(p_obj->p_pb_instance->OD);
                INIT_GPIO_PORT_B_PUP(p_obj->p_pb_instance->PUP);
                INIT_GPIO_PORT_B_PDN(p_obj->p_pb_instance->PDN);
                INIT_GPIO_PORT_B_IE(p_obj->p_pb_instance->IE);
                break;
            case GPIO_PORT_C:
                /* PC Clock Enable */
                PORT_CLOCK_ENABLE_PORTC(TSB_CG_FSYSMENB_IPMENB04);
                INIT_GPIO_PORT_C_DATA(p_obj->p_pc_instance->DATA);
                INIT_GPIO_PORT_C_CR(p_obj->p_pc_instance->CR);
                INIT_GPIO_PORT_C_FR1(p_obj->p_pc_instance->FR1);
                INIT_GPIO_PORT_C_FR2(p_obj->p_pc_instance->FR2);
                INIT_GPIO_PORT_C_FR3(p_obj->p_pc_instance->FR3);
                INIT_GPIO_PORT_C_FR4(p_obj->p_pc_instance->FR4);
                INIT_GPIO_PORT_C_FR5(p_obj->p_pc_instance->FR5);
                INIT_GPIO_PORT_C_FR6(p_obj->p_pc_instance->FR6);
                INIT_GPIO_PORT_C_FR7(p_obj->p_pc_instance->FR7);
                INIT_GPIO_PORT_C_FR8(p_obj->p_pc_instance->FR8);
                INIT_GPIO_PORT_C_OD(p_obj->p_pc_instance->OD);
                INIT_GPIO_PORT_C_PUP(p_obj->p_pc_instance->PUP);
                INIT_GPIO_PORT_C_PDN(p_obj->p_pc_instance->PDN);
                INIT_GPIO_PORT_C_IE(p_obj->p_pc_instance->IE);
                break;
            case GPIO_PORT_D:
                /* PD Clock Enable */
                PORT_CLOCK_ENABLE_PORTD(TSB_CG_FSYSMENB_IPMENB05);
                INIT_GPIO_PORT_D_DATA(p_obj->p_pd_instance->DATA);
                INIT_GPIO_PORT_D_CR(p_obj->p_pd_instance->CR);
                INIT_GPIO_PORT_D_FR1(p_obj->p_pd_instance->FR1);
                INIT_GPIO_PORT_D_FR2(p_obj->p_pd_instance->FR2);
                INIT_GPIO_PORT_D_FR3(p_obj->p_pd_instance->FR3);
                INIT_GPIO_PORT_D_FR4(p_obj->p_pd_instance->FR4);
                INIT_GPIO_PORT_D_FR5(p_obj->p_pd_instance->FR5);
                INIT_GPIO_PORT_D_FR6(p_obj->p_pd_instance->FR6);
                INIT_GPIO_PORT_D_FR7(p_obj->p_pd_instance->FR7);
                INIT_GPIO_PORT_D_FR8(p_obj->p_pd_instance->FR8);
                INIT_GPIO_PORT_D_OD(p_obj->p_pd_instance->OD);
                INIT_GPIO_PORT_D_PUP(p_obj->p_pd_instance->PUP);
                INIT_GPIO_PORT_D_PDN(p_obj->p_pd_instance->PDN);
                INIT_GPIO_PORT_D_IE(p_obj->p_pd_instance->IE);
                break;
            case GPIO_PORT_E:
                /* PE Clock Enable */
                PORT_CLOCK_ENABLE_PORTE(TSB_CG_FSYSMENB_IPMENB06);
                INIT_GPIO_PORT_E_DATA(p_obj->p_pe_instance->DATA);
                INIT_GPIO_PORT_E_CR(p_obj->p_pe_instance->CR);
                INIT_GPIO_PORT_E_FR1(p_obj->p_pe_instance->FR1);
                INIT_GPIO_PORT_E_FR2(p_obj->p_pe_instance->FR2);
                INIT_GPIO_PORT_E_FR3(p_obj->p_pe_instance->FR3);
                INIT_GPIO_PORT_E_FR4(p_obj->p_pe_instance->FR4);
                INIT_GPIO_PORT_E_FR5(p_obj->p_pe_instance->FR5);
                INIT_GPIO_PORT_E_FR6(p_obj->p_pe_instance->FR6);
                INIT_GPIO_PORT_E_FR7(p_obj->p_pe_instance->FR7);
                INIT_GPIO_PORT_E_FR8(p_obj->p_pe_instance->FR8);
                INIT_GPIO_PORT_E_OD(p_obj->p_pe_instance->OD);
                INIT_GPIO_PORT_E_PUP(p_obj->p_pe_instance->PUP);
                INIT_GPIO_PORT_E_PDN(p_obj->p_pe_instance->PDN);
                INIT_GPIO_PORT_E_IE(p_obj->p_pe_instance->IE);
                break;
            case GPIO_PORT_F:
                /* PF Clock Enable */
                PORT_CLOCK_ENABLE_PORTF(TSB_CG_FSYSMENB_IPMENB07);
                INIT_GPIO_PORT_F_DATA(p_obj->p_pf_instance->DATA);
                INIT_GPIO_PORT_F_CR(p_obj->p_pf_instance->CR);
                INIT_GPIO_PORT_F_FR1(p_obj->p_pf_instance->FR1);
                INIT_GPIO_PORT_F_FR2(p_obj->p_pf_instance->FR2);
                INIT_GPIO_PORT_F_FR3(p_obj->p_pf_instance->FR3);
                INIT_GPIO_PORT_F_FR4(p_obj->p_pf_instance->FR4);
                INIT_GPIO_PORT_F_FR5(p_obj->p_pf_instance->FR5);
                INIT_GPIO_PORT_F_FR6(p_obj->p_pf_instance->FR6);
                INIT_GPIO_PORT_F_FR7(p_obj->p_pf_instance->FR7);
                INIT_GPIO_PORT_F_FR8(p_obj->p_pf_instance->FR8);
                INIT_GPIO_PORT_F_OD(p_obj->p_pf_instance->OD);
                INIT_GPIO_PORT_F_PUP(p_obj->p_pf_instance->PUP);
                INIT_GPIO_PORT_F_PDN(p_obj->p_pf_instance->PDN);
                INIT_GPIO_PORT_F_IE(p_obj->p_pf_instance->IE);
                break;
            case GPIO_PORT_G:
                /* PG Clock Enable */
                PORT_CLOCK_ENABLE_PORTG(TSB_CG_FSYSMENB_IPMENB08);
                INIT_GPIO_PORT_G_DATA(p_obj->p_pg_instance->DATA);
                INIT_GPIO_PORT_G_CR(p_obj->p_pg_instance->CR);
                INIT_GPIO_PORT_G_FR1(p_obj->p_pg_instance->FR1);
                INIT_GPIO_PORT_G_FR2(p_obj->p_pg_instance->FR2);
                INIT_GPIO_PORT_G_FR3(p_obj->p_pg_instance->FR3);
                INIT_GPIO_PORT_G_FR4(p_obj->p_pg_instance->FR4);
                INIT_GPIO_PORT_G_FR6(p_obj->p_pg_instance->FR6);
                INIT_GPIO_PORT_G_FR5(p_obj->p_pg_instance->FR5);
                INIT_GPIO_PORT_G_FR7(p_obj->p_pg_instance->FR7);
                INIT_GPIO_PORT_G_FR8(p_obj->p_pg_instance->FR8);
                INIT_GPIO_PORT_G_OD(p_obj->p_pg_instance->OD);
                INIT_GPIO_PORT_G_PUP(p_obj->p_pg_instance->PUP);
                INIT_GPIO_PORT_G_PDN(p_obj->p_pg_instance->PDN);
                INIT_GPIO_PORT_G_IE(p_obj->p_pg_instance->IE);
                break;
            case GPIO_PORT_H:
                /* PH Clock Enable */
                PORT_CLOCK_ENABLE_PORTH(TSB_CG_FSYSMENB_IPMENB09);
                INIT_GPIO_PORT_H_DATA(p_obj->p_ph_instance->DATA);
                INIT_GPIO_PORT_H_CR(p_obj->p_ph_instance->CR);
                INIT_GPIO_PORT_H_FR1(p_obj->p_ph_instance->FR1);
                INIT_GPIO_PORT_H_FR2(p_obj->p_ph_instance->FR2);
                INIT_GPIO_PORT_H_FR3(p_obj->p_ph_instance->FR3);
                INIT_GPIO_PORT_H_FR4(p_obj->p_ph_instance->FR4);
                INIT_GPIO_PORT_H_FR5(p_obj->p_ph_instance->FR5);
                INIT_GPIO_PORT_H_FR6(p_obj->p_ph_instance->FR6);
                INIT_GPIO_PORT_H_FR7(p_obj->p_ph_instance->FR7);
                INIT_GPIO_PORT_H_FR8(p_obj->p_ph_instance->FR8);
                INIT_GPIO_PORT_H_OD(p_obj->p_ph_instance->OD);
                INIT_GPIO_PORT_H_PUP(p_obj->p_ph_instance->PUP);
                INIT_GPIO_PORT_H_PDN(p_obj->p_ph_instance->PDN);
                INIT_GPIO_PORT_H_IE(p_obj->p_ph_instance->IE);
                break;
            case GPIO_PORT_J:
                /* PJ Clock Enable */
                PORT_CLOCK_ENABLE_PORTJ(TSB_CG_FSYSMENB_IPMENB10);
                INIT_GPIO_PORT_J_DATA(p_obj->p_pj_instance->DATA);
                INIT_GPIO_PORT_J_CR(p_obj->p_pj_instance->CR);
                INIT_GPIO_PORT_J_FR1(p_obj->p_pj_instance->FR1);
                INIT_GPIO_PORT_J_FR2(p_obj->p_pj_instance->FR2);
                INIT_GPIO_PORT_J_FR3(p_obj->p_pj_instance->FR3);
                INIT_GPIO_PORT_J_FR4(p_obj->p_pj_instance->FR4);
                INIT_GPIO_PORT_J_FR5(p_obj->p_pj_instance->FR5);
                INIT_GPIO_PORT_J_FR6(p_obj->p_pj_instance->FR6);
                INIT_GPIO_PORT_J_FR7(p_obj->p_pj_instance->FR7);
                INIT_GPIO_PORT_J_FR8(p_obj->p_pj_instance->FR8);
                INIT_GPIO_PORT_J_OD(p_obj->p_pj_instance->OD);
                INIT_GPIO_PORT_J_PUP(p_obj->p_pj_instance->PUP);
                INIT_GPIO_PORT_J_PDN(p_obj->p_pj_instance->PDN);
                INIT_GPIO_PORT_J_IE(p_obj->p_pj_instance->IE);
                break;
            case GPIO_PORT_K:
                /* PK Clock Enable */
                PORT_CLOCK_ENABLE_PORTK(TSB_CG_FSYSMENB_IPMENB11);
                INIT_GPIO_PORT_K_DATA(p_obj->p_pk_instance->DATA);
                INIT_GPIO_PORT_K_CR(p_obj->p_pk_instance->CR);
                INIT_GPIO_PORT_K_FR1(p_obj->p_pk_instance->FR1);
                INIT_GPIO_PORT_K_FR2(p_obj->p_pk_instance->FR2);
                INIT_GPIO_PORT_K_FR3(p_obj->p_pk_instance->FR3);
                INIT_GPIO_PORT_K_FR4(p_obj->p_pk_instance->FR4);
                INIT_GPIO_PORT_K_FR5(p_obj->p_pk_instance->FR5);
                INIT_GPIO_PORT_K_FR6(p_obj->p_pk_instance->FR6);
                INIT_GPIO_PORT_K_FR7(p_obj->p_pk_instance->FR7);
                INIT_GPIO_PORT_K_FR8(p_obj->p_pk_instance->FR8);
                INIT_GPIO_PORT_K_OD(p_obj->p_pk_instance->OD);
                INIT_GPIO_PORT_K_PUP(p_obj->p_pk_instance->PUP);
                INIT_GPIO_PORT_K_PDN(p_obj->p_pk_instance->PDN);
                INIT_GPIO_PORT_K_IE(p_obj->p_pk_instance->IE);
                break;
            case GPIO_PORT_L:
                /* PL Clock Enable */
                PORT_CLOCK_ENABLE_PORTL(TSB_CG_FSYSMENB_IPMENB12);
                INIT_GPIO_PORT_L_DATA(p_obj->p_pl_instance->DATA);
                INIT_GPIO_PORT_L_CR(p_obj->p_pl_instance->CR);
                INIT_GPIO_PORT_L_FR1(p_obj->p_pl_instance->FR1);
                INIT_GPIO_PORT_L_FR2(p_obj->p_pl_instance->FR2);
                INIT_GPIO_PORT_L_FR3(p_obj->p_pl_instance->FR3);
                INIT_GPIO_PORT_L_FR4(p_obj->p_pl_instance->FR4);
                INIT_GPIO_PORT_L_FR5(p_obj->p_pl_instance->FR5);
                INIT_GPIO_PORT_L_FR6(p_obj->p_pl_instance->FR6);
                INIT_GPIO_PORT_L_FR7(p_obj->p_pl_instance->FR7);
                INIT_GPIO_PORT_L_FR8(p_obj->p_pl_instance->FR8);
                INIT_GPIO_PORT_L_OD(p_obj->p_pl_instance->OD);
                INIT_GPIO_PORT_L_PUP(p_obj->p_pl_instance->PUP);
                INIT_GPIO_PORT_L_PDN(p_obj->p_pl_instance->PDN);
                INIT_GPIO_PORT_L_IE(p_obj->p_pl_instance->IE);
                break;
            case GPIO_PORT_M:
                /* PM Clock Enable */
                PORT_CLOCK_ENABLE_PORTM(TSB_CG_FSYSMENB_IPMENB13);
                INIT_GPIO_PORT_M_DATA(p_obj->p_pm_instance->DATA);
                INIT_GPIO_PORT_M_CR(p_obj->p_pm_instance->CR);
                INIT_GPIO_PORT_M_FR1(p_obj->p_pm_instance->FR1);
                INIT_GPIO_PORT_M_FR2(p_obj->p_pm_instance->FR2);
                INIT_GPIO_PORT_M_FR3(p_obj->p_pm_instance->FR3);
                INIT_GPIO_PORT_M_FR4(p_obj->p_pm_instance->FR4);
                INIT_GPIO_PORT_M_FR5(p_obj->p_pm_instance->FR5);
                INIT_GPIO_PORT_M_FR6(p_obj->p_pm_instance->FR6);
                INIT_GPIO_PORT_M_FR7(p_obj->p_pm_instance->FR7);
                INIT_GPIO_PORT_M_FR8(p_obj->p_pm_instance->FR8);
                INIT_GPIO_PORT_M_OD(p_obj->p_pm_instance->OD);
                INIT_GPIO_PORT_M_PUP(p_obj->p_pm_instance->PUP);
                INIT_GPIO_PORT_M_PDN(p_obj->p_pm_instance->PDN);
                INIT_GPIO_PORT_M_IE(p_obj->p_pm_instance->IE);
                break;
            case GPIO_PORT_N:
                /* PN Clock Enable */
                PORT_CLOCK_ENABLE_PORTN(TSB_CG_FSYSMENB_IPMENB14);
                INIT_GPIO_PORT_N_DATA(p_obj->p_pn_instance->DATA);
                INIT_GPIO_PORT_N_CR(p_obj->p_pn_instance->CR);
                INIT_GPIO_PORT_N_FR1(p_obj->p_pn_instance->FR1);
                INIT_GPIO_PORT_N_FR2(p_obj->p_pn_instance->FR2);
                INIT_GPIO_PORT_N_FR3(p_obj->p_pn_instance->FR3);
                INIT_GPIO_PORT_N_FR4(p_obj->p_pn_instance->FR4);
                INIT_GPIO_PORT_N_FR5(p_obj->p_pn_instance->FR5);
                INIT_GPIO_PORT_N_FR6(p_obj->p_pn_instance->FR6);
                INIT_GPIO_PORT_N_FR7(p_obj->p_pn_instance->FR7);
                INIT_GPIO_PORT_N_FR8(p_obj->p_pn_instance->FR8);
                INIT_GPIO_PORT_N_OD(p_obj->p_pn_instance->OD);
                INIT_GPIO_PORT_N_PUP(p_obj->p_pn_instance->PUP);
                INIT_GPIO_PORT_N_PDN(p_obj->p_pn_instance->PDN);
                INIT_GPIO_PORT_N_IE(p_obj->p_pn_instance->IE);
                break;
            case GPIO_PORT_P:
                /* PP Clock Enable */
                PORT_CLOCK_ENABLE_PORTP(TSB_CG_FSYSMENB_IPMENB15);
                INIT_GPIO_PORT_P_DATA(p_obj->p_pp_instance->DATA);
                INIT_GPIO_PORT_P_CR(p_obj->p_pp_instance->CR);
                INIT_GPIO_PORT_P_FR1(p_obj->p_pp_instance->FR1);
                INIT_GPIO_PORT_P_FR2(p_obj->p_pp_instance->FR2);
                INIT_GPIO_PORT_P_FR3(p_obj->p_pp_instance->FR3);
                INIT_GPIO_PORT_P_FR4(p_obj->p_pp_instance->FR4);
                INIT_GPIO_PORT_P_FR5(p_obj->p_pp_instance->FR5);
                INIT_GPIO_PORT_P_FR6(p_obj->p_pp_instance->FR6);
                INIT_GPIO_PORT_P_FR7(p_obj->p_pp_instance->FR7);
                INIT_GPIO_PORT_P_FR8(p_obj->p_pp_instance->FR8);
                INIT_GPIO_PORT_P_OD(p_obj->p_pp_instance->OD);
                INIT_GPIO_PORT_P_PUP(p_obj->p_pp_instance->PUP);
                INIT_GPIO_PORT_P_PDN(p_obj->p_pp_instance->PDN);
                INIT_GPIO_PORT_P_IE(p_obj->p_pp_instance->IE);
                break;
            case GPIO_PORT_R:
                /* PR Clock Enable */
                PORT_CLOCK_ENABLE_PORTR(TSB_CG_FSYSMENB_IPMENB16);
                INIT_GPIO_PORT_R_DATA(p_obj->p_pr_instance->DATA);
                INIT_GPIO_PORT_R_CR(p_obj->p_pr_instance->CR);
                INIT_GPIO_PORT_R_FR1(p_obj->p_pr_instance->FR1);
                INIT_GPIO_PORT_R_FR2(p_obj->p_pr_instance->FR2);
                INIT_GPIO_PORT_R_FR3(p_obj->p_pr_instance->FR3);
                INIT_GPIO_PORT_R_FR4(p_obj->p_pr_instance->FR4);
                INIT_GPIO_PORT_R_FR5(p_obj->p_pr_instance->FR5);
                INIT_GPIO_PORT_R_FR6(p_obj->p_pr_instance->FR6);
                INIT_GPIO_PORT_R_FR7(p_obj->p_pr_instance->FR7);
                INIT_GPIO_PORT_R_FR8(p_obj->p_pr_instance->FR8);
                INIT_GPIO_PORT_R_OD(p_obj->p_pr_instance->OD);
                INIT_GPIO_PORT_R_PUP(p_obj->p_pr_instance->PUP);
                INIT_GPIO_PORT_R_PDN(p_obj->p_pr_instance->PDN);
                INIT_GPIO_PORT_R_IE(p_obj->p_pr_instance->IE);
                break;
            case GPIO_PORT_T:
                /* PT Clock Enable */
                PORT_CLOCK_ENABLE_PORTT(TSB_CG_FSYSMENB_IPMENB17);
                INIT_GPIO_PORT_T_DATA(p_obj->p_pt_instance->DATA);
                INIT_GPIO_PORT_T_CR(p_obj->p_pt_instance->CR);
                INIT_GPIO_PORT_T_FR1(p_obj->p_pt_instance->FR1);
                INIT_GPIO_PORT_T_FR2(p_obj->p_pt_instance->FR2);
                INIT_GPIO_PORT_T_FR3(p_obj->p_pt_instance->FR3);
                INIT_GPIO_PORT_T_FR4(p_obj->p_pt_instance->FR4);
                INIT_GPIO_PORT_T_FR5(p_obj->p_pt_instance->FR5);
                INIT_GPIO_PORT_T_FR6(p_obj->p_pt_instance->FR6);
                INIT_GPIO_PORT_T_FR7(p_obj->p_pt_instance->FR7);
                INIT_GPIO_PORT_T_FR8(p_obj->p_pt_instance->FR8);
                INIT_GPIO_PORT_T_OD(p_obj->p_pt_instance->OD);
                INIT_GPIO_PORT_T_PUP(p_obj->p_pt_instance->PUP);
                INIT_GPIO_PORT_T_PDN(p_obj->p_pt_instance->PDN);
                INIT_GPIO_PORT_T_IE(p_obj->p_pt_instance->IE);
                break;
            case GPIO_PORT_U:
                /* PU Clock Enable */
                PORT_CLOCK_ENABLE_PORTU(TSB_CG_FSYSMENB_IPMENB18);
                INIT_GPIO_PORT_U_DATA(p_obj->p_pu_instance->DATA);
                INIT_GPIO_PORT_U_CR(p_obj->p_pu_instance->CR);
                INIT_GPIO_PORT_U_FR1(p_obj->p_pu_instance->FR1);
                INIT_GPIO_PORT_U_FR2(p_obj->p_pu_instance->FR2);
                INIT_GPIO_PORT_U_FR3(p_obj->p_pu_instance->FR3);
                INIT_GPIO_PORT_U_FR4(p_obj->p_pu_instance->FR4);
                INIT_GPIO_PORT_U_FR5(p_obj->p_pu_instance->FR5);
                INIT_GPIO_PORT_U_FR6(p_obj->p_pu_instance->FR6);
                INIT_GPIO_PORT_U_FR7(p_obj->p_pu_instance->FR7);
                INIT_GPIO_PORT_U_FR8(p_obj->p_pu_instance->FR8);
                INIT_GPIO_PORT_U_OD(p_obj->p_pu_instance->OD);
                INIT_GPIO_PORT_U_PUP(p_obj->p_pu_instance->PUP);
                INIT_GPIO_PORT_U_PDN(p_obj->p_pu_instance->PDN);
                INIT_GPIO_PORT_U_IE(p_obj->p_pu_instance->IE);
                break;
            case GPIO_PORT_V:
                /* PV Clock Enable */
                PORT_CLOCK_ENABLE_PORTV(TSB_CG_FSYSMENB_IPMENB19);
                INIT_GPIO_PORT_V_DATA(p_obj->p_pv_instance->DATA);
                INIT_GPIO_PORT_V_CR(p_obj->p_pv_instance->CR);
                INIT_GPIO_PORT_V_FR1(p_obj->p_pv_instance->FR1);
                INIT_GPIO_PORT_V_FR2(p_obj->p_pv_instance->FR2);
                INIT_GPIO_PORT_V_FR3(p_obj->p_pv_instance->FR3);
                INIT_GPIO_PORT_V_FR4(p_obj->p_pv_instance->FR4);
                INIT_GPIO_PORT_V_FR5(p_obj->p_pv_instance->FR5);
                INIT_GPIO_PORT_V_FR6(p_obj->p_pv_instance->FR6);
                INIT_GPIO_PORT_V_FR7(p_obj->p_pv_instance->FR7);
                INIT_GPIO_PORT_V_FR8(p_obj->p_pv_instance->FR8);
                INIT_GPIO_PORT_V_OD(p_obj->p_pv_instance->OD);
                INIT_GPIO_PORT_V_PUP(p_obj->p_pv_instance->PUP);
                INIT_GPIO_PORT_V_PDN(p_obj->p_pv_instance->PDN);
                INIT_GPIO_PORT_V_IE(p_obj->p_pv_instance->IE);
                break;
            case GPIO_PORT_W:
                /* PW Clock Enable */
                PORT_CLOCK_ENABLE_PORTW(TSB_CG_FSYSMENB_IPMENB20);
                INIT_GPIO_PORT_W_DATA(p_obj->p_pw_instance->DATA);
                INIT_GPIO_PORT_W_CR(p_obj->p_pw_instance->CR);
                INIT_GPIO_PORT_W_FR1(p_obj->p_pw_instance->FR1);
                INIT_GPIO_PORT_W_FR2(p_obj->p_pw_instance->FR2);
                INIT_GPIO_PORT_W_FR3(p_obj->p_pw_instance->FR3);
                INIT_GPIO_PORT_W_FR4(p_obj->p_pw_instance->FR4);
                INIT_GPIO_PORT_W_FR5(p_obj->p_pw_instance->FR5);
                INIT_GPIO_PORT_W_FR6(p_obj->p_pw_instance->FR6);
                INIT_GPIO_PORT_W_FR7(p_obj->p_pw_instance->FR7);
                INIT_GPIO_PORT_W_FR8(p_obj->p_pw_instance->FR8);
                INIT_GPIO_PORT_W_OD(p_obj->p_pw_instance->OD);
                INIT_GPIO_PORT_W_PUP(p_obj->p_pw_instance->PUP);
                INIT_GPIO_PORT_W_PDN(p_obj->p_pw_instance->PDN);
                INIT_GPIO_PORT_W_IE(p_obj->p_pw_instance->IE);
                break;
            case GPIO_PORT_Y:
                /* PY Clock Enable */
                PORT_CLOCK_ENABLE_PORTY(TSB_CG_FSYSMENB_IPMENB21);
                INIT_GPIO_PORT_Y_DATA(p_obj->p_py_instance->DATA);
                INIT_GPIO_PORT_Y_CR(p_obj->p_py_instance->CR);
                INIT_GPIO_PORT_Y_FR1(p_obj->p_py_instance->FR1);
                INIT_GPIO_PORT_Y_FR2(p_obj->p_py_instance->FR2);
                INIT_GPIO_PORT_Y_FR3(p_obj->p_py_instance->FR3);
                INIT_GPIO_PORT_Y_FR4(p_obj->p_py_instance->FR4);
                INIT_GPIO_PORT_Y_FR5(p_obj->p_py_instance->FR5);
                INIT_GPIO_PORT_Y_FR6(p_obj->p_py_instance->FR6);
                INIT_GPIO_PORT_Y_FR7(p_obj->p_py_instance->FR7);
                INIT_GPIO_PORT_Y_FR8(p_obj->p_py_instance->FR8);
                INIT_GPIO_PORT_Y_OD(p_obj->p_py_instance->OD);
                INIT_GPIO_PORT_Y_PUP(p_obj->p_py_instance->PUP);
                INIT_GPIO_PORT_Y_PDN(p_obj->p_py_instance->PDN);
                INIT_GPIO_PORT_Y_IE(p_obj->p_py_instance->IE);
                break;
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
    if ((void *)(p_obj) == (void *)0) {
        result = TXZ_ERROR;
    } else {
        /* Disable the selected GPIO peripheral  */
        switch (group) {
            case GPIO_PORT_A:
                INIT_GPIO_PORT_A_DATA(p_obj->p_pa_instance->DATA);
                INIT_GPIO_PORT_A_CR(p_obj->p_pa_instance->CR);
                INIT_GPIO_PORT_A_FR1(p_obj->p_pa_instance->FR1);
                INIT_GPIO_PORT_A_FR2(p_obj->p_pa_instance->FR2);
                INIT_GPIO_PORT_A_FR3(p_obj->p_pa_instance->FR3);
                INIT_GPIO_PORT_A_FR4(p_obj->p_pa_instance->FR4);
                INIT_GPIO_PORT_A_FR5(p_obj->p_pa_instance->FR5);
                INIT_GPIO_PORT_A_FR6(p_obj->p_pa_instance->FR6);
                INIT_GPIO_PORT_A_FR7(p_obj->p_pa_instance->FR7);
                INIT_GPIO_PORT_A_FR8(p_obj->p_pa_instance->FR8);
                INIT_GPIO_PORT_A_OD(p_obj->p_pa_instance->OD);
                INIT_GPIO_PORT_A_PUP(p_obj->p_pa_instance->PUP);
                INIT_GPIO_PORT_A_PDN(p_obj->p_pa_instance->PDN);
                INIT_GPIO_PORT_A_IE(p_obj->p_pa_instance->IE);
                /* PA Clock Disable */
                PORT_CLOCK_DISABLE_PORTA(TSB_CG_FSYSMENB_IPMENB02);
                break;
            case GPIO_PORT_B:
                INIT_GPIO_PORT_B_DATA(p_obj->p_pb_instance->DATA);
                INIT_GPIO_PORT_B_CR(p_obj->p_pb_instance->CR);
                INIT_GPIO_PORT_B_FR1(p_obj->p_pb_instance->FR1);
                INIT_GPIO_PORT_B_FR2(p_obj->p_pb_instance->FR2);
                INIT_GPIO_PORT_B_FR3(p_obj->p_pb_instance->FR3);
                INIT_GPIO_PORT_B_FR4(p_obj->p_pb_instance->FR4);
                INIT_GPIO_PORT_B_FR5(p_obj->p_pb_instance->FR5);
                INIT_GPIO_PORT_B_FR6(p_obj->p_pb_instance->FR6);
                INIT_GPIO_PORT_B_FR7(p_obj->p_pb_instance->FR7);
                INIT_GPIO_PORT_B_FR8(p_obj->p_pb_instance->FR8);
                INIT_GPIO_PORT_B_OD(p_obj->p_pb_instance->OD);
                INIT_GPIO_PORT_B_PUP(p_obj->p_pb_instance->PUP);
                INIT_GPIO_PORT_B_PDN(p_obj->p_pb_instance->PDN);
                INIT_GPIO_PORT_B_IE(p_obj->p_pb_instance->IE);
                /* PB Clock Disable */
                PORT_CLOCK_DISABLE_PORTB(TSB_CG_FSYSMENB_IPMENB03);
                break;
            case GPIO_PORT_C:
                INIT_GPIO_PORT_C_DATA(p_obj->p_pc_instance->DATA);
                INIT_GPIO_PORT_C_CR(p_obj->p_pc_instance->CR);
                INIT_GPIO_PORT_C_FR1(p_obj->p_pc_instance->FR1);
                INIT_GPIO_PORT_C_FR2(p_obj->p_pc_instance->FR2);
                INIT_GPIO_PORT_C_FR3(p_obj->p_pc_instance->FR3);
                INIT_GPIO_PORT_C_FR4(p_obj->p_pc_instance->FR4);
                INIT_GPIO_PORT_C_FR5(p_obj->p_pc_instance->FR5);
                INIT_GPIO_PORT_C_FR6(p_obj->p_pc_instance->FR6);
                INIT_GPIO_PORT_C_FR7(p_obj->p_pc_instance->FR7);
                INIT_GPIO_PORT_C_FR8(p_obj->p_pc_instance->FR8);
                INIT_GPIO_PORT_C_OD(p_obj->p_pc_instance->OD);
                INIT_GPIO_PORT_C_PUP(p_obj->p_pc_instance->PUP);
                INIT_GPIO_PORT_C_PDN(p_obj->p_pc_instance->PDN);
                INIT_GPIO_PORT_C_IE(p_obj->p_pc_instance->IE);
                /* PC Clock Disable */
                PORT_CLOCK_DISABLE_PORTC(TSB_CG_FSYSMENB_IPMENB04);
                break;
            case GPIO_PORT_D:
                INIT_GPIO_PORT_D_DATA(p_obj->p_pd_instance->DATA);
                INIT_GPIO_PORT_D_CR(p_obj->p_pd_instance->CR);
                INIT_GPIO_PORT_D_FR1(p_obj->p_pd_instance->FR1);
                INIT_GPIO_PORT_D_FR2(p_obj->p_pd_instance->FR2);
                INIT_GPIO_PORT_D_FR3(p_obj->p_pd_instance->FR3);
                INIT_GPIO_PORT_D_FR4(p_obj->p_pd_instance->FR4);
                INIT_GPIO_PORT_D_FR5(p_obj->p_pd_instance->FR5);
                INIT_GPIO_PORT_D_FR6(p_obj->p_pd_instance->FR6);
                INIT_GPIO_PORT_D_FR7(p_obj->p_pd_instance->FR7);
                INIT_GPIO_PORT_D_FR8(p_obj->p_pd_instance->FR8);
                INIT_GPIO_PORT_D_OD(p_obj->p_pd_instance->OD);
                INIT_GPIO_PORT_D_PUP(p_obj->p_pd_instance->PUP);
                INIT_GPIO_PORT_D_PDN(p_obj->p_pd_instance->PDN);
                INIT_GPIO_PORT_D_IE(p_obj->p_pd_instance->IE);
                /* PD Clock Disable */
                PORT_CLOCK_DISABLE_PORTD(TSB_CG_FSYSMENB_IPMENB05);
                break;
            case GPIO_PORT_E:
                INIT_GPIO_PORT_E_DATA(p_obj->p_pe_instance->DATA);
                INIT_GPIO_PORT_E_CR(p_obj->p_pe_instance->CR);
                INIT_GPIO_PORT_E_FR1(p_obj->p_pe_instance->FR1);
                INIT_GPIO_PORT_E_FR2(p_obj->p_pe_instance->FR2);
                INIT_GPIO_PORT_E_FR3(p_obj->p_pe_instance->FR3);
                INIT_GPIO_PORT_E_FR4(p_obj->p_pe_instance->FR4);
                INIT_GPIO_PORT_E_FR5(p_obj->p_pe_instance->FR5);
                INIT_GPIO_PORT_E_FR6(p_obj->p_pe_instance->FR6);
                INIT_GPIO_PORT_E_FR7(p_obj->p_pe_instance->FR7);
                INIT_GPIO_PORT_E_FR8(p_obj->p_pe_instance->FR8);
                INIT_GPIO_PORT_E_OD(p_obj->p_pe_instance->OD);
                INIT_GPIO_PORT_E_PUP(p_obj->p_pe_instance->PUP);
                INIT_GPIO_PORT_E_PDN(p_obj->p_pe_instance->PDN);
                INIT_GPIO_PORT_E_IE(p_obj->p_pe_instance->IE);
                /* PE Clock Disable */
                PORT_CLOCK_DISABLE_PORTE(TSB_CG_FSYSMENB_IPMENB06);
                break;
            case GPIO_PORT_F:
                INIT_GPIO_PORT_F_DATA(p_obj->p_pf_instance->DATA);
                INIT_GPIO_PORT_F_CR(p_obj->p_pf_instance->CR);
                INIT_GPIO_PORT_F_FR1(p_obj->p_pf_instance->FR1);
                INIT_GPIO_PORT_F_FR2(p_obj->p_pf_instance->FR2);
                INIT_GPIO_PORT_F_FR3(p_obj->p_pf_instance->FR3);
                INIT_GPIO_PORT_F_FR4(p_obj->p_pf_instance->FR4);
                INIT_GPIO_PORT_F_FR5(p_obj->p_pf_instance->FR5);
                INIT_GPIO_PORT_F_FR6(p_obj->p_pf_instance->FR6);
                INIT_GPIO_PORT_F_FR7(p_obj->p_pf_instance->FR7);
                INIT_GPIO_PORT_F_FR8(p_obj->p_pf_instance->FR8);
                INIT_GPIO_PORT_F_OD(p_obj->p_pf_instance->OD);
                INIT_GPIO_PORT_F_PUP(p_obj->p_pf_instance->PUP);
                INIT_GPIO_PORT_F_PDN(p_obj->p_pf_instance->PDN);
                INIT_GPIO_PORT_F_IE(p_obj->p_pf_instance->IE);
                /* PF Clock Disable */
                PORT_CLOCK_DISABLE_PORTF(TSB_CG_FSYSMENB_IPMENB07);
                break;
            case GPIO_PORT_G:
                INIT_GPIO_PORT_G_DATA(p_obj->p_pg_instance->DATA);
                INIT_GPIO_PORT_G_CR(p_obj->p_pg_instance->CR);
                INIT_GPIO_PORT_G_FR1(p_obj->p_pg_instance->FR1);
                INIT_GPIO_PORT_G_FR2(p_obj->p_pg_instance->FR2);
                INIT_GPIO_PORT_G_FR3(p_obj->p_pg_instance->FR3);
                INIT_GPIO_PORT_G_FR4(p_obj->p_pg_instance->FR4);
                INIT_GPIO_PORT_G_FR5(p_obj->p_pg_instance->FR5);
                INIT_GPIO_PORT_G_FR6(p_obj->p_pg_instance->FR6);
                INIT_GPIO_PORT_G_FR7(p_obj->p_pg_instance->FR7);
                INIT_GPIO_PORT_G_FR8(p_obj->p_pg_instance->FR8);
                INIT_GPIO_PORT_G_OD(p_obj->p_pg_instance->OD);
                INIT_GPIO_PORT_G_PUP(p_obj->p_pg_instance->PUP);
                INIT_GPIO_PORT_G_PDN(p_obj->p_pg_instance->PDN);
                INIT_GPIO_PORT_G_IE(p_obj->p_pg_instance->IE);
                /* PG Clock Disable */
                PORT_CLOCK_DISABLE_PORTG(TSB_CG_FSYSMENB_IPMENB08);
                break;
            case GPIO_PORT_H:
                INIT_GPIO_PORT_H_DATA(p_obj->p_ph_instance->DATA);
                INIT_GPIO_PORT_H_CR(p_obj->p_ph_instance->CR);
                INIT_GPIO_PORT_H_FR1(p_obj->p_ph_instance->FR1);
                INIT_GPIO_PORT_H_FR2(p_obj->p_ph_instance->FR2);
                INIT_GPIO_PORT_H_FR3(p_obj->p_ph_instance->FR3);
                INIT_GPIO_PORT_H_FR4(p_obj->p_ph_instance->FR4);
                INIT_GPIO_PORT_H_FR5(p_obj->p_ph_instance->FR5);
                INIT_GPIO_PORT_H_FR6(p_obj->p_ph_instance->FR6);
                INIT_GPIO_PORT_H_FR7(p_obj->p_ph_instance->FR7);
                INIT_GPIO_PORT_H_FR8(p_obj->p_ph_instance->FR8);
                INIT_GPIO_PORT_H_OD(p_obj->p_ph_instance->OD);
                INIT_GPIO_PORT_H_PUP(p_obj->p_ph_instance->PUP);
                INIT_GPIO_PORT_H_PDN(p_obj->p_ph_instance->PDN);
                INIT_GPIO_PORT_H_IE(p_obj->p_ph_instance->IE);
                /* PH Clock Disable */
                PORT_CLOCK_DISABLE_PORTH(TSB_CG_FSYSMENB_IPMENB09);
                break;
            case GPIO_PORT_J:
                INIT_GPIO_PORT_J_DATA(p_obj->p_pj_instance->DATA);
                INIT_GPIO_PORT_J_CR(p_obj->p_pj_instance->CR);
                INIT_GPIO_PORT_J_FR1(p_obj->p_pj_instance->FR1);
                INIT_GPIO_PORT_J_FR2(p_obj->p_pj_instance->FR2);
                INIT_GPIO_PORT_J_FR3(p_obj->p_pj_instance->FR3);
                INIT_GPIO_PORT_J_FR4(p_obj->p_pj_instance->FR4);
                INIT_GPIO_PORT_J_FR5(p_obj->p_pj_instance->FR5);
                INIT_GPIO_PORT_J_FR6(p_obj->p_pj_instance->FR6);
                INIT_GPIO_PORT_J_FR7(p_obj->p_pj_instance->FR7);
                INIT_GPIO_PORT_J_FR8(p_obj->p_pj_instance->FR8);
                INIT_GPIO_PORT_J_OD(p_obj->p_pj_instance->OD);
                INIT_GPIO_PORT_J_PUP(p_obj->p_pj_instance->PUP);
                INIT_GPIO_PORT_J_PDN(p_obj->p_pj_instance->PDN);
                INIT_GPIO_PORT_J_IE(p_obj->p_pj_instance->IE);
                /* PJ Clock Disable */
                PORT_CLOCK_DISABLE_PORTJ(TSB_CG_FSYSMENB_IPMENB10);
                break;
            case GPIO_PORT_K:
                INIT_GPIO_PORT_K_DATA(p_obj->p_pk_instance->DATA);
                INIT_GPIO_PORT_K_CR(p_obj->p_pk_instance->CR);
                INIT_GPIO_PORT_K_FR1(p_obj->p_pk_instance->FR1);
                INIT_GPIO_PORT_K_FR2(p_obj->p_pk_instance->FR2);
                INIT_GPIO_PORT_K_FR3(p_obj->p_pk_instance->FR3);
                INIT_GPIO_PORT_K_FR4(p_obj->p_pk_instance->FR4);
                INIT_GPIO_PORT_K_FR5(p_obj->p_pk_instance->FR5);
                INIT_GPIO_PORT_K_FR6(p_obj->p_pk_instance->FR6);
                INIT_GPIO_PORT_K_FR7(p_obj->p_pk_instance->FR7);
                INIT_GPIO_PORT_K_FR8(p_obj->p_pk_instance->FR8);
                INIT_GPIO_PORT_K_OD(p_obj->p_pk_instance->OD);
                INIT_GPIO_PORT_K_PUP(p_obj->p_pk_instance->PUP);
                INIT_GPIO_PORT_K_PDN(p_obj->p_pk_instance->PDN);
                INIT_GPIO_PORT_K_IE(p_obj->p_pk_instance->IE);
                /* PK Clock Disable */
                PORT_CLOCK_DISABLE_PORTK(TSB_CG_FSYSMENB_IPMENB11);
                break;
            case GPIO_PORT_L:
                INIT_GPIO_PORT_L_DATA(p_obj->p_pl_instance->DATA);
                INIT_GPIO_PORT_L_CR(p_obj->p_pl_instance->CR);
                INIT_GPIO_PORT_L_FR1(p_obj->p_pl_instance->FR1);
                INIT_GPIO_PORT_L_FR2(p_obj->p_pl_instance->FR2);
                INIT_GPIO_PORT_L_FR3(p_obj->p_pl_instance->FR3);
                INIT_GPIO_PORT_L_FR4(p_obj->p_pl_instance->FR4);
                INIT_GPIO_PORT_L_FR5(p_obj->p_pl_instance->FR5);
                INIT_GPIO_PORT_L_FR6(p_obj->p_pl_instance->FR6);
                INIT_GPIO_PORT_L_FR7(p_obj->p_pl_instance->FR7);
                INIT_GPIO_PORT_L_FR8(p_obj->p_pl_instance->FR8);
                INIT_GPIO_PORT_L_OD(p_obj->p_pl_instance->OD);
                INIT_GPIO_PORT_L_PUP(p_obj->p_pl_instance->PUP);
                INIT_GPIO_PORT_L_PDN(p_obj->p_pl_instance->PDN);
                INIT_GPIO_PORT_L_IE(p_obj->p_pl_instance->IE);
                /* PL Clock Disable */
                PORT_CLOCK_DISABLE_PORTL(TSB_CG_FSYSMENB_IPMENB12);
                break;
            case GPIO_PORT_M:
                INIT_GPIO_PORT_M_DATA(p_obj->p_pm_instance->DATA);
                INIT_GPIO_PORT_M_CR(p_obj->p_pm_instance->CR);
                INIT_GPIO_PORT_M_FR1(p_obj->p_pm_instance->FR1);
                INIT_GPIO_PORT_M_FR2(p_obj->p_pm_instance->FR2);
                INIT_GPIO_PORT_M_FR3(p_obj->p_pm_instance->FR3);
                INIT_GPIO_PORT_M_FR4(p_obj->p_pm_instance->FR4);
                INIT_GPIO_PORT_M_FR5(p_obj->p_pm_instance->FR5);
                INIT_GPIO_PORT_M_FR6(p_obj->p_pm_instance->FR6);
                INIT_GPIO_PORT_M_FR7(p_obj->p_pm_instance->FR7);
                INIT_GPIO_PORT_M_FR8(p_obj->p_pm_instance->FR8);
                INIT_GPIO_PORT_M_OD(p_obj->p_pm_instance->OD);
                INIT_GPIO_PORT_M_PUP(p_obj->p_pm_instance->PUP);
                INIT_GPIO_PORT_M_PDN(p_obj->p_pm_instance->PDN);
                INIT_GPIO_PORT_M_IE(p_obj->p_pm_instance->IE);
                /* PM Clock Disable */
                PORT_CLOCK_DISABLE_PORTM(TSB_CG_FSYSMENB_IPMENB13);
                break;
            case GPIO_PORT_N:
                INIT_GPIO_PORT_N_DATA(p_obj->p_pn_instance->DATA);
                INIT_GPIO_PORT_N_CR(p_obj->p_pn_instance->CR);
                INIT_GPIO_PORT_N_FR1(p_obj->p_pn_instance->FR1);
                INIT_GPIO_PORT_N_FR2(p_obj->p_pn_instance->FR2);
                INIT_GPIO_PORT_N_FR3(p_obj->p_pn_instance->FR3);
                INIT_GPIO_PORT_N_FR4(p_obj->p_pn_instance->FR4);
                INIT_GPIO_PORT_N_FR5(p_obj->p_pn_instance->FR5);
                INIT_GPIO_PORT_N_FR6(p_obj->p_pn_instance->FR6);
                INIT_GPIO_PORT_N_FR7(p_obj->p_pn_instance->FR7);
                INIT_GPIO_PORT_N_FR8(p_obj->p_pn_instance->FR8);
                INIT_GPIO_PORT_N_OD(p_obj->p_pn_instance->OD);
                INIT_GPIO_PORT_N_PUP(p_obj->p_pn_instance->PUP);
                INIT_GPIO_PORT_N_PDN(p_obj->p_pn_instance->PDN);
                INIT_GPIO_PORT_N_IE(p_obj->p_pn_instance->IE);
                /* PN Clock Disable */
                PORT_CLOCK_DISABLE_PORTN(TSB_CG_FSYSMENB_IPMENB14);
                break;
            case GPIO_PORT_P:
                INIT_GPIO_PORT_P_DATA(p_obj->p_pp_instance->DATA);
                INIT_GPIO_PORT_P_CR(p_obj->p_pp_instance->CR);
                INIT_GPIO_PORT_P_FR1(p_obj->p_pp_instance->FR1);
                INIT_GPIO_PORT_P_FR2(p_obj->p_pp_instance->FR2);
                INIT_GPIO_PORT_P_FR3(p_obj->p_pp_instance->FR3);
                INIT_GPIO_PORT_P_FR4(p_obj->p_pp_instance->FR4);
                INIT_GPIO_PORT_P_FR5(p_obj->p_pp_instance->FR5);
                INIT_GPIO_PORT_P_FR6(p_obj->p_pp_instance->FR6);
                INIT_GPIO_PORT_P_FR7(p_obj->p_pp_instance->FR7);
                INIT_GPIO_PORT_P_FR8(p_obj->p_pp_instance->FR8);
                INIT_GPIO_PORT_P_OD(p_obj->p_pp_instance->OD);
                INIT_GPIO_PORT_P_PUP(p_obj->p_pp_instance->PUP);
                INIT_GPIO_PORT_P_PDN(p_obj->p_pp_instance->PDN);
                INIT_GPIO_PORT_P_IE(p_obj->p_pp_instance->IE);
                /* PP Clock Disable */
                PORT_CLOCK_DISABLE_PORTP(TSB_CG_FSYSMENB_IPMENB15);
                break;
            case GPIO_PORT_R:
                INIT_GPIO_PORT_R_DATA(p_obj->p_pr_instance->DATA);
                INIT_GPIO_PORT_R_CR(p_obj->p_pr_instance->CR);
                INIT_GPIO_PORT_R_FR1(p_obj->p_pr_instance->FR1);
                INIT_GPIO_PORT_R_FR2(p_obj->p_pr_instance->FR2);
                INIT_GPIO_PORT_R_FR3(p_obj->p_pr_instance->FR3);
                INIT_GPIO_PORT_R_FR4(p_obj->p_pr_instance->FR4);
                INIT_GPIO_PORT_R_FR5(p_obj->p_pr_instance->FR5);
                INIT_GPIO_PORT_R_FR6(p_obj->p_pr_instance->FR6);
                INIT_GPIO_PORT_R_FR7(p_obj->p_pr_instance->FR7);
                INIT_GPIO_PORT_R_FR8(p_obj->p_pr_instance->FR8);
                INIT_GPIO_PORT_R_OD(p_obj->p_pr_instance->OD);
                INIT_GPIO_PORT_R_PUP(p_obj->p_pr_instance->PUP);
                INIT_GPIO_PORT_R_PDN(p_obj->p_pr_instance->PDN);
                INIT_GPIO_PORT_R_IE(p_obj->p_pr_instance->IE);
                /* PR Clock Disable */
                PORT_CLOCK_DISABLE_PORTR(TSB_CG_FSYSMENB_IPMENB16);
                break;
            case GPIO_PORT_T:
                INIT_GPIO_PORT_T_DATA(p_obj->p_pt_instance->DATA);
                INIT_GPIO_PORT_T_CR(p_obj->p_pt_instance->CR);
                INIT_GPIO_PORT_T_FR1(p_obj->p_pt_instance->FR1);
                INIT_GPIO_PORT_T_FR2(p_obj->p_pt_instance->FR2);
                INIT_GPIO_PORT_T_FR3(p_obj->p_pt_instance->FR3);
                INIT_GPIO_PORT_T_FR4(p_obj->p_pt_instance->FR4);
                INIT_GPIO_PORT_T_FR5(p_obj->p_pt_instance->FR5);
                INIT_GPIO_PORT_T_FR6(p_obj->p_pt_instance->FR6);
                INIT_GPIO_PORT_T_FR7(p_obj->p_pt_instance->FR7);
                INIT_GPIO_PORT_T_FR8(p_obj->p_pt_instance->FR8);
                INIT_GPIO_PORT_T_OD(p_obj->p_pt_instance->OD);
                INIT_GPIO_PORT_T_PUP(p_obj->p_pt_instance->PUP);
                INIT_GPIO_PORT_T_PDN(p_obj->p_pt_instance->PDN);
                INIT_GPIO_PORT_T_IE(p_obj->p_pt_instance->IE);
                /* PT Clock Disable */
                PORT_CLOCK_DISABLE_PORTT(TSB_CG_FSYSMENB_IPMENB17);
                break;
            case GPIO_PORT_U:
                INIT_GPIO_PORT_U_DATA(p_obj->p_pu_instance->DATA);
                INIT_GPIO_PORT_U_CR(p_obj->p_pu_instance->CR);
                INIT_GPIO_PORT_U_FR1(p_obj->p_pu_instance->FR1);
                INIT_GPIO_PORT_U_FR2(p_obj->p_pu_instance->FR2);
                INIT_GPIO_PORT_U_FR3(p_obj->p_pu_instance->FR3);
                INIT_GPIO_PORT_U_FR4(p_obj->p_pu_instance->FR4);
                INIT_GPIO_PORT_U_FR5(p_obj->p_pu_instance->FR5);
                INIT_GPIO_PORT_U_FR6(p_obj->p_pu_instance->FR6);
                INIT_GPIO_PORT_U_FR7(p_obj->p_pu_instance->FR7);
                INIT_GPIO_PORT_U_FR8(p_obj->p_pu_instance->FR8);
                INIT_GPIO_PORT_U_OD(p_obj->p_pu_instance->OD);
                INIT_GPIO_PORT_U_PUP(p_obj->p_pu_instance->PUP);
                INIT_GPIO_PORT_U_PDN(p_obj->p_pu_instance->PDN);
                INIT_GPIO_PORT_U_IE(p_obj->p_pu_instance->IE);
                /* PU Clock Disable */
                PORT_CLOCK_DISABLE_PORTU(TSB_CG_FSYSMENB_IPMENB18);
                break;
            case GPIO_PORT_V:
                INIT_GPIO_PORT_V_DATA(p_obj->p_pv_instance->DATA);
                INIT_GPIO_PORT_V_CR(p_obj->p_pv_instance->CR);
                INIT_GPIO_PORT_V_FR1(p_obj->p_pv_instance->FR1);
                INIT_GPIO_PORT_V_FR2(p_obj->p_pv_instance->FR2);
                INIT_GPIO_PORT_V_FR3(p_obj->p_pv_instance->FR3);
                INIT_GPIO_PORT_V_FR4(p_obj->p_pv_instance->FR4);
                INIT_GPIO_PORT_V_FR5(p_obj->p_pv_instance->FR5);
                INIT_GPIO_PORT_V_FR6(p_obj->p_pv_instance->FR6);
                INIT_GPIO_PORT_V_FR7(p_obj->p_pv_instance->FR7);
                INIT_GPIO_PORT_V_FR8(p_obj->p_pv_instance->FR8);
                INIT_GPIO_PORT_V_OD(p_obj->p_pv_instance->OD);
                INIT_GPIO_PORT_V_PUP(p_obj->p_pv_instance->PUP);
                INIT_GPIO_PORT_V_PDN(p_obj->p_pv_instance->PDN);
                INIT_GPIO_PORT_V_IE(p_obj->p_pv_instance->IE);
                /* PV Clock Disable */
                PORT_CLOCK_DISABLE_PORTV(TSB_CG_FSYSMENB_IPMENB19);
                break;
            case GPIO_PORT_W:
                INIT_GPIO_PORT_W_DATA(p_obj->p_pw_instance->DATA);
                INIT_GPIO_PORT_W_CR(p_obj->p_pw_instance->CR);
                INIT_GPIO_PORT_W_FR1(p_obj->p_pw_instance->FR1);
                INIT_GPIO_PORT_W_FR2(p_obj->p_pw_instance->FR2);
                INIT_GPIO_PORT_W_FR3(p_obj->p_pw_instance->FR3);
                INIT_GPIO_PORT_W_FR4(p_obj->p_pw_instance->FR4);
                INIT_GPIO_PORT_W_FR5(p_obj->p_pw_instance->FR5);
                INIT_GPIO_PORT_W_FR6(p_obj->p_pw_instance->FR6);
                INIT_GPIO_PORT_W_FR7(p_obj->p_pw_instance->FR7);
                INIT_GPIO_PORT_W_FR8(p_obj->p_pw_instance->FR8);
                INIT_GPIO_PORT_W_OD(p_obj->p_pw_instance->OD);
                INIT_GPIO_PORT_W_PUP(p_obj->p_pw_instance->PUP);
                INIT_GPIO_PORT_W_PDN(p_obj->p_pw_instance->PDN);
                INIT_GPIO_PORT_W_IE(p_obj->p_pw_instance->IE);
                /* PW Clock Disable */
                PORT_CLOCK_DISABLE_PORTW(TSB_CG_FSYSMENB_IPMENB20);
                break;
            case GPIO_PORT_Y:
                INIT_GPIO_PORT_Y_DATA(p_obj->p_py_instance->DATA);
                INIT_GPIO_PORT_Y_CR(p_obj->p_py_instance->CR);
                INIT_GPIO_PORT_Y_FR1(p_obj->p_py_instance->FR1);
                INIT_GPIO_PORT_Y_FR2(p_obj->p_py_instance->FR2);
                INIT_GPIO_PORT_Y_FR3(p_obj->p_py_instance->FR3);
                INIT_GPIO_PORT_Y_FR4(p_obj->p_py_instance->FR4);
                INIT_GPIO_PORT_Y_FR5(p_obj->p_py_instance->FR5);
                INIT_GPIO_PORT_Y_FR6(p_obj->p_py_instance->FR6);
                INIT_GPIO_PORT_Y_FR7(p_obj->p_py_instance->FR7);
                INIT_GPIO_PORT_Y_FR8(p_obj->p_py_instance->FR8);
                INIT_GPIO_PORT_Y_OD(p_obj->p_py_instance->OD);
                INIT_GPIO_PORT_Y_PUP(p_obj->p_py_instance->PUP);
                INIT_GPIO_PORT_Y_PDN(p_obj->p_py_instance->PDN);
                INIT_GPIO_PORT_Y_IE(p_obj->p_py_instance->IE);
                /* PY Clock Disable */
                PORT_CLOCK_DISABLE_PORTY(TSB_CG_FSYSMENB_IPMENB21);
                break;
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
    for (i = GPIO_PORT_0; i < GPIO_PORT_Max; i++) {
        param_result = check_param_pin_exist(p_obj, group, (uint32_t)i, mode);
        if (param_result == PARAM_OK) {
            break;
        } else {
            result = TXZ_ERROR;
        }
    }
    if (((void *)(p_obj) == (void *)0) || (param_result == PARAM_NG)) {
        result = TXZ_ERROR;
    } else {
        switch (group) {
            case GPIO_PORT_A:
                WRITE_PORT(mode, p_obj->p_pa_instance, val, PinExistTbl[GPIO_PORT_A][i]);
                break;
            case GPIO_PORT_B:
                WRITE_PORT(mode, p_obj->p_pb_instance, val, PinExistTbl[GPIO_PORT_B][i]);
                break;
            case GPIO_PORT_C:
                WRITE_PORT(mode, p_obj->p_pc_instance, val, PinExistTbl[GPIO_PORT_C][i]);
                break;
            case GPIO_PORT_D:
                WRITE_PORT(mode, p_obj->p_pd_instance, val, PinExistTbl[GPIO_PORT_D][i]);
                break;
            case GPIO_PORT_E:
                WRITE_PORT(mode, p_obj->p_pe_instance, val, PinExistTbl[GPIO_PORT_E][i]);
                break;
            case GPIO_PORT_F:
                WRITE_PORT(mode, p_obj->p_pf_instance, val, PinExistTbl[GPIO_PORT_F][i]);
                break;
            case GPIO_PORT_G:
                WRITE_PORT(mode, p_obj->p_pg_instance, val, PinExistTbl[GPIO_PORT_G][i]);
                break;
            case GPIO_PORT_H:
                WRITE_PORT(mode, p_obj->p_ph_instance, val, PinExistTbl[GPIO_PORT_H][i]);
                break;
            case GPIO_PORT_J:
                WRITE_PORT(mode, p_obj->p_pj_instance, val, PinExistTbl[GPIO_PORT_J][i]);
                break;
            case GPIO_PORT_K:
                WRITE_PORT(mode, p_obj->p_pk_instance, val, PinExistTbl[GPIO_PORT_K][i]);
                break;
            case GPIO_PORT_L:
                WRITE_PORT(mode, p_obj->p_pl_instance, val, PinExistTbl[GPIO_PORT_L][i]);
                break;
            case GPIO_PORT_M:
                WRITE_PORT(mode, p_obj->p_pm_instance, val, PinExistTbl[GPIO_PORT_M][i]);
                break;
            case GPIO_PORT_N:
                WRITE_PORT(mode, p_obj->p_pn_instance, val, PinExistTbl[GPIO_PORT_N][i]);
                break;
            case GPIO_PORT_P:
                WRITE_PORT(mode, p_obj->p_pp_instance, val, PinExistTbl[GPIO_PORT_P][i]);
                break;
            case GPIO_PORT_R:
                WRITE_PORT(mode, p_obj->p_pr_instance, val, PinExistTbl[GPIO_PORT_R][i]);
                break;
            case GPIO_PORT_T:
                WRITE_PORT(mode, p_obj->p_pt_instance, val, PinExistTbl[GPIO_PORT_T][i]);
                break;
            case GPIO_PORT_U:
                WRITE_PORT(mode, p_obj->p_pu_instance, val, PinExistTbl[GPIO_PORT_U][i]);
                break;
            case GPIO_PORT_V:
                WRITE_PORT(mode, p_obj->p_pv_instance, val, PinExistTbl[GPIO_PORT_V][i]);
                break;
            case GPIO_PORT_W:
                WRITE_PORT(mode, p_obj->p_pw_instance, val, PinExistTbl[GPIO_PORT_W][i]);
                break;
            case GPIO_PORT_Y:
                WRITE_PORT(mode, p_obj->p_py_instance, val, PinExistTbl[GPIO_PORT_Y][i]);
                break;
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
    for (i = GPIO_PORT_0; i < GPIO_PORT_Max; i++) {
        param_result = check_param_pin_exist(p_obj, group, (uint32_t)i, mode);
        if (param_result == PARAM_OK) {
            break;
        } else {
            result = TXZ_ERROR;
        }
    }
    if (((void *)(p_obj) == (void *)0) || (param_result == PARAM_NG)) {
        result = TXZ_ERROR;
    } else {
        switch (group) {
            case GPIO_PORT_A:
                READ_PORT(mode, p_obj->p_pa_instance, val, PinExistTbl[GPIO_PORT_A][i]);
                break;
            case GPIO_PORT_B:
                READ_PORT(mode, p_obj->p_pb_instance, val, PinExistTbl[GPIO_PORT_B][i]);
                break;
            case GPIO_PORT_C:
                READ_PORT(mode, p_obj->p_pc_instance, val, PinExistTbl[GPIO_PORT_C][i]);
                break;
            case GPIO_PORT_D:
                READ_PORT(mode, p_obj->p_pd_instance, val, PinExistTbl[GPIO_PORT_D][i]);
                break;
            case GPIO_PORT_E:
                READ_PORT(mode, p_obj->p_pe_instance, val, PinExistTbl[GPIO_PORT_E][i]);
                break;
            case GPIO_PORT_F:
                READ_PORT(mode, p_obj->p_pf_instance, val, PinExistTbl[GPIO_PORT_F][i]);
                break;
            case GPIO_PORT_G:
                READ_PORT(mode, p_obj->p_pg_instance, val, PinExistTbl[GPIO_PORT_G][i]);
                break;
            case GPIO_PORT_H:
                READ_PORT(mode, p_obj->p_ph_instance, val, PinExistTbl[GPIO_PORT_H][i]);
                break;
            case GPIO_PORT_J:
                READ_PORT(mode, p_obj->p_pj_instance, val, PinExistTbl[GPIO_PORT_J][i]);
                break;
            case GPIO_PORT_K:
                READ_PORT(mode, p_obj->p_pk_instance, val, PinExistTbl[GPIO_PORT_K][i]);
                break;
            case GPIO_PORT_L:
                READ_PORT(mode, p_obj->p_pl_instance, val, PinExistTbl[GPIO_PORT_L][i]);
                break;
            case GPIO_PORT_M:
                READ_PORT(mode, p_obj->p_pm_instance, val, PinExistTbl[GPIO_PORT_M][i]);
                break;
            case GPIO_PORT_N:
                READ_PORT(mode, p_obj->p_pn_instance, val, PinExistTbl[GPIO_PORT_N][i]);
                break;
            case GPIO_PORT_P:
                READ_PORT(mode, p_obj->p_pp_instance, val, PinExistTbl[GPIO_PORT_P][i]);
                break;
            case GPIO_PORT_R:
                READ_PORT(mode, p_obj->p_pr_instance, val, PinExistTbl[GPIO_PORT_R][i]);
                break;
            case GPIO_PORT_T:
                READ_PORT(mode, p_obj->p_pt_instance, val, PinExistTbl[GPIO_PORT_T][i]);
                break;
            case GPIO_PORT_U:
                READ_PORT(mode, p_obj->p_pu_instance, val, PinExistTbl[GPIO_PORT_U][i]);
                break;
            case GPIO_PORT_V:
                READ_PORT(mode, p_obj->p_pv_instance, val, PinExistTbl[GPIO_PORT_V][i]);
                break;
            case GPIO_PORT_W:
                READ_PORT(mode, p_obj->p_pw_instance, val, PinExistTbl[GPIO_PORT_W][i]);
                break;
            case GPIO_PORT_Y:
                READ_PORT(mode, p_obj->p_py_instance, val, PinExistTbl[GPIO_PORT_Y][i]);
                break;
            default:
                result = TXZ_ERROR;
                break;
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
    if (((void *)(p_obj) == (void *)0) || (param_result == PARAM_NG)) {
        result = TXZ_ERROR;
    } else {
        switch (group) {
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
            default:
                result = TXZ_ERROR;
                break;
        }
        if (result == TXZ_ERROR) {
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
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR8);
        BITBAND_PORT_CLR(mode_base, num);

        /* Initialize Input/Output */
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
        BITBAND_PORT_CLR(mode_base, num);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
        BITBAND_PORT_CLR(mode_base, num);

        switch (func) {
            case 0:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_NOTINOUT) {
                    /* No Process */
                }
                break;
            case GPIO_FR_1:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR1);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR1);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR1);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_NOTINOUT) {
                    /* No Process */
                }
                break;
            case GPIO_FR_2:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR2);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR2);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR2);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_NOTINOUT) {
                    /* No Process */
                }
                break;
            case GPIO_FR_3:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR3);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR3);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR3);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_NOTINOUT) {
                    /* No Process */
                }
                break;
            case GPIO_FR_4:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR4);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR4);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR4);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_NOTINOUT) {
                    /* No Process */
                }
                break;
            case GPIO_FR_5:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR5);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR5);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR5);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                }
                break;
            case GPIO_FR_6:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR6);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR6);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR6);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_NOTINOUT) {
                    /* No Process */
                }
                break;
            case GPIO_FR_7:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR7);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR7);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR7);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_NOTINOUT) {
                    /* No Process */
                }
                break;
            case GPIO_FR_8:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR8);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR8);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR8);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_NOTINOUT) {
                    /* No Process */
                }
                break;
            case GPIO_FR_NA:
                if (inout == GPIO_PIN_OUTPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_CLR(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_INPUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_CLR(mode_base, num);
                } else if (inout == GPIO_PIN_INOUT) {
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
                    BITBAND_PORT_SET(mode_base, num);
                    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
                    BITBAND_PORT_SET(mode_base, num);
                } else if (inout == GPIO_PIN_NOTINOUT) {
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

    if ((void *)(p_obj) == (void *)0) {
        result = TXZ_ERROR;
    } else if (check_param_pin_exist(p_obj, group, num, GPIO_Mode_PUP) == PARAM_NG) {
        result = TXZ_ERROR;
    } else {
        port_base = BITBAND_PORT_BASE(group);

        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_PUP);
        if (val == GPIO_PIN_SET) {
            BITBAND_PORT_SET(mode_base, num);
        } else if (val == GPIO_PIN_RESET) {
            BITBAND_PORT_CLR(mode_base, num);
        } else {
            result = TXZ_ERROR;
        }
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

    if ((void *)(p_obj) == (void *)0) {
        result = TXZ_ERROR;
    } else if (check_param_pin_exist(p_obj, group, num, GPIO_Mode_PDN) == PARAM_NG) {
        result = TXZ_ERROR;
    } else {
        port_base = BITBAND_PORT_BASE(group);

        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_PDN);
        if (val == GPIO_PIN_SET) {
            BITBAND_PORT_SET(mode_base, num);
        } else if (val == GPIO_PIN_RESET) {
            BITBAND_PORT_CLR(mode_base, num);
        } else {
            result = TXZ_ERROR;
        }
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

    if ((void *)(p_obj) == (void *)0) {
        result = TXZ_ERROR;
    } else if (check_param_pin_exist(p_obj, group, num, GPIO_Mode_OD) == PARAM_NG) {
        result = TXZ_ERROR;
    } else {
        port_base = BITBAND_PORT_BASE(group);

        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_OD);
        if (val == GPIO_PIN_SET) {
            BITBAND_PORT_SET(mode_base, num);
        } else if (val == GPIO_PIN_RESET) {
            BITBAND_PORT_CLR(mode_base, num);
        } else {
            result = TXZ_ERROR;
        }
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
    if ((void *)(p_obj) == (void *)0) {
        result = TXZ_ERROR;
    } else if (check_param_pin_exist(p_obj, group, num, mode) == PARAM_NG) {
        result = TXZ_ERROR;
    } else {
        base = BITBAND_PORT_BASE(group);
        base = BITBAND_PORT_MODE_BASE(base, mode);
        if (val == GPIO_PIN_SET) {
            BITBAND_PORT_SET(base, num);
        } else if (val == GPIO_PIN_RESET) {
            BITBAND_PORT_CLR(base, num);
        } else {
            result = TXZ_ERROR;
        }
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
    if ((void *)(p_obj) == (void *)0) {
        result = TXZ_ERROR;
    } else if (check_param_pin_exist(p_obj, group, num, mode) == PARAM_NG) {
        result = TXZ_ERROR;
    } else {
        base = BITBAND_PORT_BASE(group);
        base = BITBAND_PORT_MODE_BASE(base, mode);
        BITBAND_PORT_READ(val, base, num);
        if (val == GPIO_PIN_RESET) {
            *pinstate = GPIO_PIN_RESET;
        } else if (val == GPIO_PIN_SET) {
            *pinstate = GPIO_PIN_SET;
        } else {
            result = TXZ_ERROR;
        }
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


