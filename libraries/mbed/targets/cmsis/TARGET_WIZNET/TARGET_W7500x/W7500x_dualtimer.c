/**
  ******************************************************************************
  * @file    W7500x_stdPeriph_Driver/src/W7500x_dualtimer.c    
  * @author  IOP Team
  * @version v1.0.0
  * @date    01-May-2015
  * @brief   This file contains all the functions prototypes for the dualtimer 
  *          firmware library.
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Includes -------------------------------------------*/
#include "W7500x.h"

void DUALTIMER_ClockEnable(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    if(DUALTIMERn == DUALTIMER0_0)
        TIMCLKEN0_0 = DUALTIMER_Clock_Enable;
    else if(DUALTIMERn == DUALTIMER0_1)
        TIMCLKEN0_1 = DUALTIMER_Clock_Enable;
    else if(DUALTIMERn == DUALTIMER1_0)
        TIMCLKEN1_0 = DUALTIMER_Clock_Enable;
    else if(DUALTIMERn == DUALTIMER1_1)
        TIMCLKEN1_1 = DUALTIMER_Clock_Enable;
}

void DUALTIMER_ClockDisable(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    if(DUALTIMERn == DUALTIMER0_0)
        TIMCLKEN0_0 = DUALTIMER_Clock_Disable;
    else if(DUALTIMERn == DUALTIMER0_1)
        TIMCLKEN0_1 = DUALTIMER_Clock_Disable;
    else if(DUALTIMERn == DUALTIMER1_0)
        TIMCLKEN1_0 = DUALTIMER_Clock_Disable;
    else if(DUALTIMERn == DUALTIMER1_1)
        TIMCLKEN1_1 = DUALTIMER_Clock_Disable;
}

void DUALTIMER_DeInit(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    DUALTIMER_Stop(DUALTIMERn);

    DUALTIMERn->TimerLoad = 0x0;
    DUALTIMERn->TimerControl = 0x20;
    DUALTIMERn->TimerBGLoad = 0x0;
}

void DUALTIMER_Init(DUALTIMER_TypeDef* DUALTIMERn, DUALTIMER_InitTypDef* DUALTIMER_InitStruct)
{
    uint32_t tmp = 0;
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));
    assert_param(IS_DUALTIMER_TimerMode(DUALTIMER_InitStruct->TimerControl_Mode));
    assert_param(IS_DUALTIMER_TimerPre(DUALTIMER_InitStruct->TimerControl_Pre));
    assert_param(IS_DUALTIMER_TimerSize(DUALTIMER_InitStruct->TimerControl_Size));
    assert_param(IS_DUALTIMER_OneShot(DUALTIMER_InitStruct->TimerControl_OneShot));
    
    DUALTIMER_Stop(DUALTIMERn);

    DUALTIMERn->TimerLoad = DUALTIMER_InitStruct->TimerLoad;

    tmp = DUALTIMERn->TimerControl;
    tmp |= (DUALTIMER_InitStruct->TimerControl_Mode << DUALTIMER_TimerControl_TimerMode_Pos);
    tmp |= (DUALTIMER_InitStruct->TimerControl_Pre << DUALTIMER_TimerControl_Pre_Pos); 
    tmp |= (DUALTIMER_InitStruct->TimerControl_Size << DUALTIMER_TimerControl_Size_Pos);
    tmp |= (DUALTIMER_InitStruct->TimerControl_OneShot << DUALTIMER_TimerControl_OneShot_Pos);
    //Reset values not used
    tmp &= 0xEF;

    DUALTIMERn->TimerControl = tmp;
}

void DUALTIMER_IntConfig(DUALTIMER_TypeDef* DUALTIMERn, FunctionalState state)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    if(state == ENABLE)
        DUALTIMERn->TimerControl |= (DUALTIMER_TimerControl_IntEnable << DUALTIMER_TimerControl_IntEnable_Pos);
    else 
        DUALTIMERn->TimerControl &= ~(DUALTIMER_TimerControl_IntEnable << DUALTIMER_TimerControl_IntEnable_Pos);
}

void DUALTIMER_IntClear(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    DUALTIMERn->TimerIntClr = DUALTIMER_Int_Clear;
}

ITStatus DUALTIMER_GetIntStatus(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    return (ITStatus)DUALTIMERn->TimerMIS;
}

FlagStatus DUALTIMER_GetIntEnableStatus(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    return (FlagStatus)((DUALTIMERn->TimerControl >> DUALTIMER_TimerControl_IntEnable_Pos) & 0x1);
}

void DUALTIMER_Start(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    DUALTIMERn->TimerControl |= (DUALTIMER_TimerControl_TimerEnable << DUALTIMER_TimerControl_TimerEnable_Pos);
}

void DUALTIMER_Stop(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    DUALTIMERn->TimerControl &= ~(DUALTIMER_TimerControl_TimerEnable << DUALTIMER_TimerControl_TimerEnable_Pos);
}

uint32_t DUALTIMER_GetTimerLoad(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    return DUALTIMERn->TimerLoad;
}

void DUALTIMER_SetTimerLoad(DUALTIMER_TypeDef* DUALTIMERn, uint32_t TimerLoad)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    DUALTIMERn->TimerLoad = TimerLoad;
}

uint32_t DUALTIMER_GetTimerValue(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    return DUALTIMERn->TimerValue;
}

uint32_t DUALTIMER_GetTimerControl(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    return DUALTIMERn->TimerControl;
}

void DUALTIMER_SetTimerControl(DUALTIMER_TypeDef* DUALTIMERn, uint32_t TimerControl)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    DUALTIMERn->TimerControl = TimerControl;
}

uint32_t DUALTIMER_GetTimerRIS(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    return DUALTIMERn->TimerRIS;
}

uint32_t DUALTIMER_GetTimerMIS(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    return DUALTIMERn->TimerMIS;
}

uint32_t DUALTIMER_GetTimerBGLoad(DUALTIMER_TypeDef* DUALTIMERn)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    return DUALTIMERn->TimerBGLoad;
}

void DUALTIMER_SetTimerBGLoad(DUALTIMER_TypeDef* DUALTIMERn, uint32_t TimerBGLoad)
{
    /* Check the parameters */
    assert_param(IS_DUALTIMER_ALL_CH(DUALTIMERn));

    DUALTIMERn->TimerBGLoad = TimerBGLoad;
}
