/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   This file contains all the functions prototypes for the UART 
  *          firmware library.
  ******************************************************************************
  *
  ******************************************************************************
  */
  
/* Includes -------------------------------------------*/
#include "W7500x.h"
#include "W7500x_pwm.h"

void PWM_DeInit(PWM_CHn_TypeDef* PWM_CHn)
{
    if( PWM_CHn == PWM_CH0 )
    {
        PWM->IER        &= PWM_IER_IE0_Disable;  ///< Interrupt enable register 
        PWM->SSR        &= PWM_SSR_SS0_Stop;     ///< Start Stop register 
        PWM->PSR        &= PWM_PSR_PS0_Restart;  ///< Pause register 
        PWM_CH0->IER    = 0;                    ///< Interrupt enable register 
        PWM_CH0->ICR    =  PWM_CHn_ICR_MatchInterruptClear      | 
                           PWM_CHn_ICR_OverflowInterruptClear   | 
                           PWM_CHn_ICR_CaptureInterruptClear;   ///< Interrupt clear register
        PWM_CH0->PR     = 0;                    ///< Prescale register 
        PWM_CH0->MR     = 0;                    ///< Match register 
        PWM_CH0->LR     = 0xFFFFFFFF;           ///< Limit register 
        PWM_CH0->UDMR   = 0;                    ///< Up Dowm mode register
        PWM_CH0->TCMR   = 0;                    ///< Timer Counter mode register
        PWM_CH0->PEEER  = 0;                    ///< PWM output Enable and External input Enable register
        PWM_CH0->CMR    = 0;                    ///< Capture mode register
        PWM_CH0->PDMR   = 0;                    ///< Periodic Mode register
        PWM_CH0->DZER   = 0;                    ///< Dead Zone Enable register
        PWM_CH0->DZCR   = 0;                    ///< Dead Zone Counter register
    }
    else if( PWM_CHn == PWM_CH1 )
    {
        PWM->IER        &= PWM_IER_IE1_Disable;  ///< Reset Interrupt enable register 
        PWM->SSR        &= PWM_SSR_SS1_Stop;     ///< Reset Start Stop register 
        PWM->PSR        &= PWM_PSR_PS1_Restart;  ///< Reset Pause register 
        PWM_CH1->IER    = 0;                    ///< Interrupt enable register 
        PWM_CH1->ICR    =  PWM_CHn_ICR_MatchInterruptClear      | 
                           PWM_CHn_ICR_OverflowInterruptClear   | 
                           PWM_CHn_ICR_CaptureInterruptClear;   ///< Interrupt clear register
        PWM_CH1->PR     = 0;                    ///< Prescale register 
        PWM_CH1->MR     = 0;                    ///< Match register 
        PWM_CH1->LR     = 0xFFFFFFFF;           ///< Limit register 
        PWM_CH1->UDMR   = 0;                    ///< Up Dowm mode register
        PWM_CH1->TCMR   = 0;                    ///< Timer Counter mode register
        PWM_CH1->PEEER  = 0;                    ///< PWM output Enable and External input Enable register
        PWM_CH1->CMR    = 0;                    ///< Capture mode register
        PWM_CH1->PDMR   = 0;                    ///< Periodic Mode register
        PWM_CH1->DZER   = 0;                    ///< Dead Zone Enable register
        PWM_CH1->DZCR   = 0;                    ///< Dead Zone Counter register
    }
    else if( PWM_CHn == PWM_CH2)
    {
        PWM->IER        &= PWM_IER_IE2_Disable;  ///< Interrupt enable register 
        PWM->SSR        &= PWM_SSR_SS2_Stop;     ///< Start Stop register 
        PWM->PSR        &= PWM_PSR_PS2_Restart;  ///< Pause register 
        PWM_CH2->IER    = 0;                    ///< Interrupt enable register 
        PWM_CH2->ICR    =  PWM_CHn_ICR_MatchInterruptClear      | 
                           PWM_CHn_ICR_OverflowInterruptClear   | 
                           PWM_CHn_ICR_CaptureInterruptClear;   ///< Interrupt clear register
        PWM_CH2->PR     = 0;                    ///< Prescale register 
        PWM_CH2->MR     = 0;                    ///< Match register 
        PWM_CH2->LR     = 0xFFFFFFFF;           ///< Limit register 
        PWM_CH2->UDMR   = 0;                    ///< Up Dowm mode register
        PWM_CH2->TCMR   = 0;                    ///< Timer Counter mode register
        PWM_CH2->PEEER  = 0;                    ///< PWM output Enable and External input Enable register
        PWM_CH2->CMR    = 0;                    ///< Capture mode register
        PWM_CH2->PDMR   = 0;                    ///< Periodic Mode register
        PWM_CH2->DZER   = 0;                    ///< Dead Zone Enable register
        PWM_CH2->DZCR   = 0;                    ///< Dead Zone Counter register
    }
    else if( PWM_CHn == PWM_CH3 )
    {
        PWM->IER        &= PWM_IER_IE3_Disable;  ///< Interrupt enable register 
        PWM->SSR        &= PWM_SSR_SS3_Stop;     ///< Start Stop register 
        PWM->PSR        &= PWM_PSR_PS3_Restart;  ///< Pause register 
        PWM_CH3->IER    = 0;                    ///< Interrupt enable register 
        PWM_CH3->ICR    =  PWM_CHn_ICR_MatchInterruptClear      | 
                           PWM_CHn_ICR_OverflowInterruptClear   | 
                           PWM_CHn_ICR_CaptureInterruptClear;   ///< Interrupt clear register
        PWM_CH3->MR     = 0;                    ///< Match register 
        PWM_CH3->LR     = 0xFFFFFFFF;           ///< Limit register 
        PWM_CH3->UDMR   = 0;                    ///< Up Dowm mode register
        PWM_CH3->TCMR   = 0;                    ///< Timer Counter mode register
        PWM_CH3->PEEER  = 0;                    ///< PWM output Enable and External input Enable register
        PWM_CH3->CMR    = 0;                    ///< Capture mode register
        PWM_CH3->PDMR   = 0;                    ///< Periodic Mode register
        PWM_CH3->DZER   = 0;                    ///< Dead Zone Enable register
        PWM_CH3->DZCR   = 0;                    ///< Dead Zone Counter register
    }
    else if( PWM_CHn == PWM_CH4 )
    {
        PWM->SSR        &= PWM_IER_IE4_Disable;  ///< Start Stop register 
        PWM->PSR        &= PWM_SSR_SS4_Stop;     ///< Pause register 
        PWM->IER        &= PWM_PSR_PS4_Restart;  ///< Interrupt enable register 
        PWM_CH4->IER    = 0;                    ///< Interrupt enable register 
        PWM_CH4->ICR    =  PWM_CHn_ICR_MatchInterruptClear      | 
                           PWM_CHn_ICR_OverflowInterruptClear   | 
                           PWM_CHn_ICR_CaptureInterruptClear;   ///< Interrupt clear register
        PWM_CH4->PR     = 0;                    ///< Prescale register 
        PWM_CH4->MR     = 0;                    ///< Match register 
        PWM_CH4->LR     = 0xFFFF;               ///< Limit register 
        PWM_CH4->UDMR   = 0;                    ///< Up Dowm mode register
        PWM_CH4->TCMR   = 0;                    ///< Timer Counter mode register
        PWM_CH4->PEEER  = 0;                    ///< PWM output Enable and External input Enable register
        PWM_CH4->CMR    = 0;                    ///< Capture mode register
        PWM_CH4->PDMR   = 0;                    ///< Periodic Mode register
        PWM_CH4->DZER   = 0;                    ///< Dead Zone Enable register
        PWM_CH4->DZCR   = 0;                    ///< Dead Zone Counter register
    }
    else if( PWM_CHn == PWM_CH5 )
    {
        PWM->SSR        &= PWM_IER_IE5_Disable;  ///< Start Stop register 
        PWM->PSR        &= PWM_SSR_SS5_Stop;     ///< Pause register 
        PWM->IER        &= PWM_PSR_PS5_Restart;  ///< Interrupt enable register 
        PWM_CH5->IER    = 0;                    ///< Interrupt enable register 
        PWM_CH5->ICR    =  PWM_CHn_ICR_MatchInterruptClear      |
                           PWM_CHn_ICR_OverflowInterruptClear   | 
                           PWM_CHn_ICR_CaptureInterruptClear;   ///< Interrupt clear register
        PWM_CH5->PR     = 0;                    ///< Prescale register 
        PWM_CH5->MR     = 0;                    ///< Match register 
        PWM_CH5->LR     = 0xFFFFFFFF;           ///< Limit register 
        PWM_CH5->UDMR   = 0;                    ///< Up Dowm mode register
        PWM_CH5->TCMR   = 0;                    ///< Timer Counter mode register
        PWM_CH5->PEEER  = 0;                    ///< PWM output Enable and External input Enable register
        PWM_CH5->CMR    = 0;                    ///< Capture mode register
        PWM_CH5->PDMR   = 0;                    ///< Periodic Mode register
        PWM_CH5->DZER   = 0;                    ///< Dead Zone Enable register
        PWM_CH5->DZCR   = 0;                    ///< Dead Zone Counter register
    }
    else if( PWM_CHn == PWM_CH6 )
    {
        PWM->SSR        &= PWM_IER_IE6_Disable;  ///< Start Stop register 
        PWM->PSR        &= PWM_SSR_SS6_Stop;     ///< Pause register 
        PWM->IER        &= PWM_PSR_PS6_Restart;  ///< Interrupt enable register 
        PWM_CH6->IER    = 0;                    ///< Interrupt enable register 
        PWM_CH6->ICR    =  PWM_CHn_ICR_MatchInterruptClear      | 
                           PWM_CHn_ICR_OverflowInterruptClear   | 
                           PWM_CHn_ICR_CaptureInterruptClear;   ///< Interrupt clear register
        PWM_CH6->PR     = 0;                    ///< Prescale register 
        PWM_CH6->MR     = 0;                    ///< Match register 
        PWM_CH6->LR     = 0xFFFFFFFF;           ///< Limit register 
        PWM_CH6->UDMR   = 0;                    ///< Up Dowm mode register
        PWM_CH6->TCMR   = 0;                    ///< Timer Counter mode register
        PWM_CH6->PEEER  = 0;                    ///< PWM output Enable and External input Enable register
        PWM_CH6->CMR    = 0;                    ///< Capture mode register
        PWM_CH6->PDMR   = 0;                    ///< Periodic Mode register
        PWM_CH6->DZER   = 0;                    ///< Dead Zone Enable register
        PWM_CH6->DZCR   = 0;                    ///< Dead Zone Counter register
    }
    else if( PWM_CHn == PWM_CH7 )
    {
        PWM->SSR        &= PWM_IER_IE7_Disable;  ///< Start Stop register 
        PWM->PSR        &= PWM_SSR_SS7_Stop;     ///< Pause register 
        PWM->IER        &= PWM_PSR_PS7_Restart;  ///< Interrupt enable register 
        PWM_CH7->IER    = 0;                    ///< Interrupt enable register 
        PWM_CH7->ICR    = PWM_CHn_ICR_MatchInterruptClear      | 
                          PWM_CHn_ICR_OverflowInterruptClear   | 
                          PWM_CHn_ICR_CaptureInterruptClear;   ///< Interrupt clear register
        PWM_CH7->PR     = 0;                    ///< Prescale register 
        PWM_CH7->MR     = 0;                    ///< Match register 
        PWM_CH7->LR     = 0xFFFFFFFF;           ///< Limit register 
        PWM_CH7->UDMR   = 0;                    ///< Up Dowm mode register
        PWM_CH7->TCMR   = 0;                    ///< Timer Counter mode register
        PWM_CH7->PEEER  = 0;                    ///< PWM output Enable and External input Enable register
        PWM_CH7->CMR    = 0;                    ///< Capture mode register
        PWM_CH7->PDMR   = 0;                    ///< Periodic Mode register
        PWM_CH7->DZER   = 0;                    ///< Dead Zone Enable register
        PWM_CH7->DZCR   = 0;                    ///< Dead Zone Counter register
    }
}


void PWM_TimerModeInit(PWM_CHn_TypeDef* PWM_CHn, PWM_TimerModeInitTypeDef* PWM_TimerModeInitStruct) //complet
{
    /* Stop PWM_CHn */
    PWM_CHn_Stop(PWM_CHn);
    /* Select Timer/Counter mode as Timer mode */ 
    PWM_CHn->TCMR = PWM_CHn_TCMR_TimerMode;
    /* Set Prescale register value */
    PWM_CHn->PR = PWM_TimerModeInitStruct->PWM_CHn_PR;
    /* Set Match register value */
    PWM_CHn->MR = PWM_TimerModeInitStruct->PWM_CHn_MR;
    /* Set Limit register value */
    PWM_CHn->LR = PWM_TimerModeInitStruct->PWM_CHn_LR;
    /* Select Up-down mode */
    PWM_CHn->UDMR = PWM_TimerModeInitStruct->PWM_CHn_UDMR;
    /* Select Periodic mode */ 
    PWM_CHn->PDMR = PWM_TimerModeInitStruct->PWM_CHn_PDMR;
}

void PWM_CaptureModeInit(PWM_CHn_TypeDef* PWM_CHn, PWM_CaptureModeInitTypeDef* PWM_CaptureModeInitStruct) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_PR_FILTER(PWM_CaptureModeInitStruct->PWM_CHn_PR));
    assert_param(IS_PWM_MR_FILTER(PWM_CHn, PWM_CaptureModeInitStruct->PWM_CHn_MR));
    assert_param(IS_PWM_LR_FILTER(PWM_CHn, PWM_CaptureModeInitStruct->PWM_CHn_LR));
    assert_param(IS_PWM_CHn_UDMR(PWM_CaptureModeInitStruct->PWM_CHn_UDMR));
    assert_param(IS_PWM_CHn_PDMR(PWM_CaptureModeInitStruct->PWM_CHn_PDMR));
    assert_param(IS_PWM_CHn_CMR(PWM_CaptureModeInitStruct->PWM_CHn_CMR));

    /* Stop PWM_CHn */
    PWM_CHn_Stop(PWM_CHn);
    /* Select Timer/Counter mode as Timer mode */ 
    PWM_CHn->TCMR = PWM_CHn_TCMR_TimerMode;
    /* Set Prescale register value */
    PWM_CHn->PR = PWM_CaptureModeInitStruct->PWM_CHn_PR;
    /* Set Match register value */
    PWM_CHn->MR = PWM_CaptureModeInitStruct->PWM_CHn_MR;
    /* Set Limit register value */
    PWM_CHn->LR = PWM_CaptureModeInitStruct->PWM_CHn_LR;
    /* Select Up-down mode */
    PWM_CHn->UDMR = PWM_CaptureModeInitStruct->PWM_CHn_UDMR;
    /* Select Periodic mode */ 
    PWM_CHn->PDMR = PWM_CaptureModeInitStruct->PWM_CHn_PDMR;
    /* Select Capture mode */
    PWM_CHn->CMR = PWM_CaptureModeInitStruct->PWM_CHn_CMR;
    /* External input enable */
    PWM_CHn->PEEER = PWM_CHn_PEEER_ExtEnable;
}

void PWM_CounterModeInit(PWM_CHn_TypeDef* PWM_CHn, PWM_CounterModeInitTypeDef* PWM_CounterModeInitStruct) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    /* Stop PWM_CHn */
    PWM_CHn_Stop(PWM_CHn);
    /* Select Timer/Counter mode as Timer mode */ 
    PWM_CHn->TCMR = PWM_CHn_TCMR_TimerMode;    
    /* Set Match register value */
    PWM_CHn->MR = PWM_CounterModeInitStruct->PWM_CHn_MR;
    /* Set Limit register value */
    PWM_CHn->LR = PWM_CounterModeInitStruct->PWM_CHn_LR;
    /* Select Up-down mode */
    PWM_CHn->UDMR = PWM_CounterModeInitStruct->PWM_CHn_UDMR;
    /* Select Periodic mode */ 
    PWM_CHn->PDMR = PWM_CounterModeInitStruct->PWM_CHn_PDMR;
    /* Select Counter mode */ 
    PWM_CHn->TCMR = PWM_CounterModeInitStruct->PWM_CHn_TCMR;
    /* Enable external input */
    PWM_CHn->PEEER = PWM_CHn_PEEER_ExtEnable; 
}

void PWM_DeadzoneModeInit(PWM_CHn_TypeDef* PWM_CHn, PWM_DeadzoneModeInitTypDef* PWM_DeadzoneModeInitStruct) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_PR_FILTER(PWM_DeadzoneModeInitStruct->PWM_CHn_PR));
    assert_param(IS_PWM_MR_FILTER(PWM_CHn, PWM_DeadzoneModeInitStruct->PWM_CHn_MR));
    assert_param(IS_PWM_LR_FILTER(PWM_CHn, PWM_DeadzoneModeInitStruct->PWM_CHn_LR));
    assert_param(IS_PWM_CHn_UDMR(PWM_DeadzoneModeInitStruct->PWM_CHn_UDMR));
    assert_param(IS_PWM_CHn_PDMR(PWM_DeadzoneModeInitStruct->PWM_CHn_PDMR));
    assert_param(IS_PWM_Deadznoe(PWM_CHn));

    /* Stop PWM_CHn */
    PWM_CHn_Stop(PWM_CHn);
    /* Select Timer/Counter mode as Timer mode */ 
    PWM_CHn->TCMR = PWM_CHn_TCMR_TimerMode;
    /* Set Prescale register value */
    PWM_CHn->PR = PWM_DeadzoneModeInitStruct->PWM_CHn_PR;
    /* Set Match register value */
    PWM_CHn->MR = PWM_DeadzoneModeInitStruct->PWM_CHn_MR;
    /* Set Limit register value */
    PWM_CHn->LR = PWM_DeadzoneModeInitStruct->PWM_CHn_LR;
    /* Select Up-down mode */
    PWM_CHn->UDMR = PWM_DeadzoneModeInitStruct->PWM_CHn_UDMR;
    /* Select Periodic mode */ 
    PWM_CHn->PDMR = PWM_DeadzoneModeInitStruct->PWM_CHn_PDMR;
    /* Enable Dead Zone generation */
    PWM_CHn->DZER = PWM_CHn_DZER_Enable; 
    /* Set Dead Zone Counter */
    PWM_CHn->DZCR = PWM_DeadzoneModeInitStruct->PWM_CHn_DZCR;
}

void PWM_CtrlPWMOutput(PWM_CHn_TypeDef* PWM_CHn, uint32_t outputEnDisable ) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_Output(outputEnDisable)); 
    if( PWM_CHn->DZER )
        assert_param(IS_PWM_Deadznoe(PWM_CHn));

    /* Stop PWM_CHn */
    PWM_CHn_Stop(PWM_CHn);
    /*Config PWM output and External input */
    PWM_CHn->PEEER = outputEnDisable; 
}

void PWM_CtrlPWMOutputEnable(PWM_CHn_TypeDef* PWM_CHn) 
{
    PWM_CtrlPWMOutput(PWM_CHn, PWM_CHn_PEEER_PWMEnable);
}

void PWM_CtrlPWMOutputDisable(PWM_CHn_TypeDef* PWM_CHn) 
{
    PWM_CtrlPWMOutput(PWM_CHn, PWM_CHn_PEEER_Disable);
}

void PWM_IntConfig(PWM_CHn_TypeDef* PWM_CHn, FunctionalState state) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    
    /* Stop PWM_CHn */
    PWM_CHn_Stop(PWM_CHn);

    if(state == ENABLE)
    {
        if(PWM_CHn == PWM_CH0) { 
            PWM->IER |= PWM_IER_IE0_Enable;
        }
        else if(PWM_CHn == PWM_CH1) {
            PWM->IER |= PWM_IER_IE1_Enable;
        }
        else if(PWM_CHn == PWM_CH2) {
            PWM->IER |= PWM_IER_IE2_Enable;
        }
        else if(PWM_CHn == PWM_CH3) {
            PWM->IER |= PWM_IER_IE3_Enable;
        }
        else if(PWM_CHn == PWM_CH4) {
            PWM->IER |= PWM_IER_IE4_Enable;
        }
        else if(PWM_CHn == PWM_CH5) {
            PWM->IER |= PWM_IER_IE5_Enable;
        }
        else if(PWM_CHn == PWM_CH6) {
            PWM->IER |= PWM_IER_IE6_Enable;
        }
        else if(PWM_CHn == PWM_CH7) {
            PWM->IER |= PWM_IER_IE7_Enable;
        }
    }
    else
    {
        if(PWM_CHn == PWM_CH0) { 
            PWM->IER &= PWM_IER_IE0_Disable;
        }
        else if(PWM_CHn == PWM_CH1) {
            PWM->IER &= PWM_IER_IE1_Disable;
        }
        else if(PWM_CHn == PWM_CH2) {
            PWM->IER &= PWM_IER_IE2_Disable;
        }
        else if(PWM_CHn == PWM_CH3) {
            PWM->IER &= PWM_IER_IE3_Disable;
        }
        else if(PWM_CHn == PWM_CH4) {
            PWM->IER &= PWM_IER_IE4_Disable;
        }
        else if(PWM_CHn == PWM_CH5) {
            PWM->IER &= PWM_IER_IE5_Disable;
        }
        else if(PWM_CHn == PWM_CH6) {
            PWM->IER &= PWM_IER_IE6_Disable;
        }
        else if(PWM_CHn == PWM_CH7) {
            PWM->IER &= PWM_IER_IE7_Disable;
        }
    }
}
 
FlagStatus PWM_GetIntEnableStatus(PWM_CHn_TypeDef* PWM_CHn)
{
    FlagStatus ret_val = RESET;

    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    if(PWM_CHn == PWM_CH0) { 
        ret_val = (FlagStatus)((PWM->IER & 0x01) >> 0);
    }
    else if(PWM_CHn == PWM_CH1) {
        ret_val = (FlagStatus)((PWM->IER & 0x02) >> 1);
    }
    else if(PWM_CHn == PWM_CH2) {
        ret_val = (FlagStatus)((PWM->IER & 0x04) >> 2);
    }
    else if(PWM_CHn == PWM_CH3) {
        ret_val = (FlagStatus)((PWM->IER & 0x08) >> 3);
    }
    else if(PWM_CHn == PWM_CH4) {
        ret_val = (FlagStatus)((PWM->IER & 0x10) >> 4);
    }
    else if(PWM_CHn == PWM_CH5) {
        ret_val = (FlagStatus)((PWM->IER & 0x20) >> 5);
    }
    else if(PWM_CHn == PWM_CH6) {
        ret_val = (FlagStatus)((PWM->IER & 0x40) >> 6);
    }
    else if(PWM_CHn == PWM_CH7) {
        ret_val =  (FlagStatus)((PWM->IER & 0x80) >> 7);
    }

    return ret_val;
}

void PWM_CHn_IntConfig(PWM_CHn_TypeDef* PWM_CHn, uint32_t PWM_CHn_IER, FunctionalState state) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    
	
	 assert_param(IS_PWM_CHn_IER(PWM_CHn_IER));
    
    /* Stop PWM_CHn */
    PWM_CHn_Stop(PWM_CHn);

    if(state == ENABLE)
        PWM_CHn->IER |= PWM_CHn_IER;
    else 
        PWM_CHn->IER &= ~PWM_CHn_IER;
}

uint32_t PWM_CHn_GetIntEnableStatus(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->IER;
}

uint32_t PWM_CHn_GetIntFlagStatus(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->IR;
}

void PWM_CHn_ClearInt(PWM_CHn_TypeDef* PWM_CHn, uint32_t PWM_CHn_ICR)
{
    /* Check the parameters */

    PWM_CHn->ICR = PWM_CHn_ICR;
}

void PWM_CHn_Start(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    /* Set Start Stop register */
    if(PWM_CHn == PWM_CH0) { 
        PWM->SSR |= PWM_SSR_SS0_Start;
    }
    else if(PWM_CHn == PWM_CH1) {
        PWM->SSR |= PWM_SSR_SS1_Start;
    }
    else if(PWM_CHn == PWM_CH2) {
        PWM->SSR |= PWM_SSR_SS2_Start;
    }
    else if(PWM_CHn == PWM_CH3) {
        PWM->SSR |= PWM_SSR_SS3_Start;
    }
    else if(PWM_CHn == PWM_CH4) {
        PWM->SSR |= PWM_SSR_SS4_Start;
    }
    else if(PWM_CHn == PWM_CH5) {
        PWM->SSR |= PWM_SSR_SS5_Start;
    }
    else if(PWM_CHn == PWM_CH6) {
        PWM->SSR |= PWM_SSR_SS6_Start;
    }
    else if(PWM_CHn == PWM_CH7) {
        PWM->SSR |= PWM_SSR_SS7_Start;
    }
}

void PWM_Multi_Start(uint32_t ssr_bit_flag) //complete
{
    /* Set Start Stop register */
    PWM->SSR |= ssr_bit_flag;
}

void PWM_CHn_Stop(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Reset Start Stop register */
    if(PWM_CHn == PWM_CH0) { 
        PWM->SSR &= PWM_SSR_SS0_Stop;
    }
    else if(PWM_CHn == PWM_CH1) {
        PWM->SSR &= PWM_SSR_SS1_Stop;
    }
    else if(PWM_CHn == PWM_CH2) {
        PWM->SSR &= PWM_SSR_SS2_Stop;
    }
    else if(PWM_CHn == PWM_CH3) {
        PWM->SSR &= PWM_SSR_SS3_Stop;
    }
    else if(PWM_CHn == PWM_CH4) {
        PWM->SSR &= PWM_SSR_SS4_Stop;
    }
    else if(PWM_CHn == PWM_CH5) {
        PWM->SSR &= PWM_SSR_SS5_Stop;
    }
    else if(PWM_CHn == PWM_CH6) {
        PWM->SSR &= PWM_SSR_SS6_Stop;
    }
    else if(PWM_CHn == PWM_CH7) {
        PWM->SSR &= PWM_SSR_SS7_Stop;
    }
}

void PWM_Multi_Stop(uint32_t ssr_bit_flag) //complete
{
    /* Reset Start Stop register */
    PWM->SSR &= ~ssr_bit_flag;
}

void PWM_CHn_Pause(PWM_CHn_TypeDef* PWM_CHn)
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    /* Set Pause register */
    if(PWM_CHn == PWM_CH0) { 
        PWM->PSR |= PWM_PSR_PS0_Pause;
    }
    else if(PWM_CHn == PWM_CH1) {
        PWM->PSR |= PWM_PSR_PS1_Pause;
    }
    else if(PWM_CHn == PWM_CH2) {
        PWM->PSR |= PWM_PSR_PS2_Pause;
    }
    else if(PWM_CHn == PWM_CH3) {
        PWM->PSR |= PWM_PSR_PS3_Pause;
    }
    else if(PWM_CHn == PWM_CH4) {
        PWM->PSR |= PWM_PSR_PS4_Pause;
    }
    else if(PWM_CHn == PWM_CH5) {
        PWM->PSR |= PWM_PSR_PS5_Pause;
    }
    else if(PWM_CHn == PWM_CH6) {
        PWM->PSR |= PWM_PSR_PS6_Pause;
    }
    else if(PWM_CHn == PWM_CH7) {
        PWM->PSR |= PWM_PSR_PS7_Pause;
    }
}

void PWM_Multi_Pause(uint32_t psr_bit_flag)
{
    PWM->PSR |= psr_bit_flag;
}

void PWM_CHn_Restart(PWM_CHn_TypeDef* PWM_CHn)
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    /* Reset Pause register */
    if(PWM_CHn == PWM_CH0) { 
        PWM->PSR &= PWM_PSR_PS0_Restart;
    }
    else if(PWM_CHn == PWM_CH1) {
        PWM->PSR &= PWM_PSR_PS1_Restart;
    }
    else if(PWM_CHn == PWM_CH2) {
        PWM->PSR &= PWM_PSR_PS2_Restart;
    }
    else if(PWM_CHn == PWM_CH3) {
        PWM->PSR &= PWM_PSR_PS3_Restart;
    }
    else if(PWM_CHn == PWM_CH4) {
        PWM->PSR &= PWM_PSR_PS4_Restart;
    }
    else if(PWM_CHn == PWM_CH5) {
        PWM->PSR &= PWM_PSR_PS5_Restart;
    }
    else if(PWM_CHn == PWM_CH6) {
        PWM->PSR &= PWM_PSR_PS6_Restart;
    }
    else if(PWM_CHn == PWM_CH7) {
        PWM->PSR &= PWM_PSR_PS7_Restart;
    }
}

void PWM_Multi_Restart(uint32_t psr_bit_flag)
{
    PWM->PSR &= ~psr_bit_flag;
}


uint32_t PWM_CHn_GetTCR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->TCR;
}

uint32_t PWM_CHn_GetPCR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->PCR;
}

uint32_t PWM_CHn_GetPR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->PR;
}

void PWM_CHn_SetPR(PWM_CHn_TypeDef* PWM_CHn, uint32_t PR) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_PR_FILTER(PR));

    PWM_CHn->PR = PR;
}

uint32_t PWM_CHn_GetMR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->MR;
}

void PWM_CHn_SetMR(PWM_CHn_TypeDef* PWM_CHn, uint32_t MR) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    PWM_CHn->MR = MR;
}

uint32_t PWM_CHn_GetLR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->LR;
}

void PWM_CHn_SetLR(PWM_CHn_TypeDef* PWM_CHn, uint32_t LR) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    PWM_CHn->LR = LR;
}

uint32_t PWM_CHn_GetUDMR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->UDMR;
}

void PWM_CHn_SetUDMR(PWM_CHn_TypeDef* PWM_CHn, uint32_t UDMR) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_CHn_UDMR(UDMR));

    PWM_CHn->UDMR = UDMR;
}

uint32_t PWM_CHn_GetTCMR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->TCMR;
}

void PWM_CHn_SetTCMR(PWM_CHn_TypeDef* PWM_CHn, uint32_t TCMR) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_CHn_TCMR(TCMR));

    PWM_CHn->TCMR = TCMR;
}

uint32_t PWM_CHn_GetPEEER(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->PEEER;
}

void PWM_CHn_SetPEEER(PWM_CHn_TypeDef* PWM_CHn, uint32_t PEEER) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_CHn_PEEER(PEEER));

    PWM_CHn->PEEER = PEEER;
}

uint32_t PWM_CHn_GetCMR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->CMR;
}

void PWM_CHn_SetCMR(PWM_CHn_TypeDef* PWM_CHn, uint32_t CMR) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_CHn_CMR(CMR));

    PWM_CHn->CMR = CMR;
}

uint32_t PWM_CHn_GetCR(PWM_CHn_TypeDef* PWM_CHn)
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->CR;
}

uint32_t PWM_CHn_GetPDMR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

    return PWM_CHn->PDMR;
}

void PWM_CHn_SetPDMR(PWM_CHn_TypeDef* PWM_CHn, uint32_t PDMR) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_CHn_PDMR(PDMR));

    PWM_CHn->PDMR = PDMR; 
}

uint32_t PWM_CHn_GetDZER(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

   return PWM_CHn->DZER;
}

void PWM_CHn_SetDZER(PWM_CHn_TypeDef* PWM_CHn, uint32_t DZER) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_CHn_DZER(DZER));

    PWM_CHn->DZER = DZER;
}

uint32_t PWM_CHn_GetDZCR(PWM_CHn_TypeDef* PWM_CHn) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));

   return PWM_CHn->DZCR;
}

void PWM_CHn_SetDZCR(PWM_CHn_TypeDef* PWM_CHn, uint32_t DZCR) //complete
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_CH(PWM_CHn));
    assert_param(IS_PWM_CHn_DZCR_FILTER(DZCR));

    PWM_CHn->DZCR = DZCR; 
}

void PWM_CH0_ClearMatchInt(void)
{
    PWM_CHn_ClearInt(PWM_CH0, PWM_CHn_ICR_MatchInterruptClear);
}

void PWM_CH0_ClearOverflowInt(void)
{
   PWM_CHn_ClearInt(PWM_CH0, PWM_CHn_ICR_OverflowInterruptClear);
}

void PWM_CH0_ClearCaptureInt(void)
{
   PWM_CHn_ClearInt(PWM_CH0, PWM_CHn_ICR_CaptureInterruptClear);
}

void PWM_CH1_ClearMatchInt(void)
{
    PWM_CHn_ClearInt(PWM_CH1, PWM_CHn_ICR_MatchInterruptClear);
}

void PWM_CH1_ClearOverflowInt(void)
{
   PWM_CHn_ClearInt(PWM_CH1, PWM_CHn_ICR_OverflowInterruptClear);
}

void PWM_CH1_ClearCaptureInt(void)
{
   PWM_CHn_ClearInt(PWM_CH1, PWM_CHn_ICR_CaptureInterruptClear);
}

void PWM_CH2_ClearMatchInt(void)
{
    PWM_CHn_ClearInt(PWM_CH2, PWM_CHn_ICR_MatchInterruptClear);
}

void PWM_CH2_ClearOverflowInt(void)
{
   PWM_CHn_ClearInt(PWM_CH2, PWM_CHn_ICR_OverflowInterruptClear);
}

void PWM_CH2_ClearCaptureInt(void)
{
   PWM_CHn_ClearInt(PWM_CH2, PWM_CHn_ICR_CaptureInterruptClear);
}

void PWM_CH3_ClearMatchInt(void)
{
    PWM_CHn_ClearInt(PWM_CH3, PWM_CHn_ICR_MatchInterruptClear);
}

void PWM_CH3_ClearOverflowInt(void)
{
   PWM_CHn_ClearInt(PWM_CH3, PWM_CHn_ICR_OverflowInterruptClear);
}

void PWM_CH3_ClearCaptureInt(void)
{
   PWM_CHn_ClearInt(PWM_CH3, PWM_CHn_ICR_CaptureInterruptClear);
}

void PWM_CH4_ClearMatchInt(void)
{
    PWM_CHn_ClearInt(PWM_CH4, PWM_CHn_ICR_MatchInterruptClear);
}

void PWM_CH4_ClearOverflowInt(void)
{
   PWM_CHn_ClearInt(PWM_CH4, PWM_CHn_ICR_OverflowInterruptClear);
}

void PWM_CH4_ClearCaptureInt(void)
{
   PWM_CHn_ClearInt(PWM_CH4, PWM_CHn_ICR_CaptureInterruptClear);
}

void PWM_CH5_ClearMatchInt(void)
{
    PWM_CHn_ClearInt(PWM_CH5, PWM_CHn_ICR_MatchInterruptClear);
}

void PWM_CH5_ClearOverflowInt(void)
{
   PWM_CHn_ClearInt(PWM_CH5, PWM_CHn_ICR_OverflowInterruptClear);
}

void PWM_CH5_ClearCaptureInt(void)
{
   PWM_CHn_ClearInt(PWM_CH5, PWM_CHn_ICR_CaptureInterruptClear);
}

void PWM_CH6_ClearMatchInt(void)
{
    PWM_CHn_ClearInt(PWM_CH6, PWM_CHn_ICR_MatchInterruptClear);
}

void PWM_CH6_ClearOverflowInt(void)
{
   PWM_CHn_ClearInt(PWM_CH6, PWM_CHn_ICR_OverflowInterruptClear);
}

void PWM_CH6_ClearCaptureInt(void)
{
   PWM_CHn_ClearInt(PWM_CH6, PWM_CHn_ICR_CaptureInterruptClear);
}

void PWM_CH7_ClearMatchInt(void)
{
    PWM_CHn_ClearInt(PWM_CH7, PWM_CHn_ICR_MatchInterruptClear);
}

void PWM_CH7_ClearOverflowInt(void)
{
   PWM_CHn_ClearInt(PWM_CH7, PWM_CHn_ICR_OverflowInterruptClear);
}

void PWM_CH7_ClearCaptureInt(void)
{
   PWM_CHn_ClearInt(PWM_CH7, PWM_CHn_ICR_CaptureInterruptClear);
}
