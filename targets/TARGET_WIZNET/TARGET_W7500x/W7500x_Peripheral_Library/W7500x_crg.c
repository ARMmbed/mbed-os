#include "W7500x_crg.h"

void CRG_DeInit(void)
{
//To Do
}

void CRG_OSC_PowerDownEnable(FunctionalState NewState)
{
    if(NewState != DISABLE)     CRG->OSC_PDR = CRG_OSC_PDR_PD;    
    else                        CRG->OSC_PDR = CRG_OSC_PDR_NRMLOP;
}

void CRG_PLL_PowerDownEnable(FunctionalState NewState)
{
    if(NewState != DISABLE)     CRG->PLL_PDR = CRG_PLL_PDR_PD;
    else                        CRG->PLL_PDR = CRG_PLL_PDR_NRMLOP;
}

void CRG_PLL_OutputEnable(FunctionalState NewState)
{
    if(NewState != DISABLE)     CRG->PLL_OER = CRG_PLL_OER_EN;
    else                        CRG->PLL_OER = CRG_PLL_OER_DIS;
}

void CRG_PLL_BypassEnable(FunctionalState NewState)
{
    if(NewState != DISABLE)     CRG->PLL_BPR = CRG_PLL_BPR_EN;
    else                        CRG->PLL_BPR = CRG_PLL_BPR_DIS;
}

void CRG_PLL_InputFrequencySelect(CRG_CLK_SOURCE src)
{
    assert_param(IS_CRG_PLL_SRC(src));

    if( src == CRG_RCLK )   CRG->PLL_IFSR = CRG_PLL_IFSR_RCLK; 
    else                    CRG->PLL_IFSR = CRG_PLL_IFSR_OCLK;
}

void CRG_FCLK_SourceSelect(CRG_CLK_SOURCE src)
{
    assert_param(IS_CRG_FCLK_SRC(src));

    if      ( src == CRG_RCLK )     CRG->FCLK_SSR = CRG_FCLK_SSR_RCLK; 
    else if ( src == CRG_OCLK )     CRG->FCLK_SSR = CRG_FCLK_SSR_OCLK;
    else                            CRG->FCLK_SSR = CRG_FCLK_SSR_MCLK;
}

void CRG_FCLK_SetPrescale(CRG_PREDIV prediv)
{
    assert_param(IS_CRG_FCLK_PREDIV(prediv));

    if      ( prediv == CRG_PREDIV1 )   CRG->FCLK_PVSR = CRG_FCLK_PVSR_DIV1; 
    else if ( prediv == CRG_PREDIV2 )   CRG->FCLK_PVSR = CRG_FCLK_PVSR_DIV2; 
    else if ( prediv == CRG_PREDIV4 )   CRG->FCLK_PVSR = CRG_FCLK_PVSR_DIV4; 
    else                                CRG->FCLK_PVSR = CRG_FCLK_PVSR_DIV8; 
}

void CRG_SSPCLK_SourceSelect(CRG_CLK_SOURCE src)
{
    assert_param(IS_CRG_SSPCLK_SRC(src));

    if      ( src == CRG_CLK_DIS )     CRG->SSPCLK_SSR = CRG_SSPCLK_SSR_DIS;
    else if ( src == CRG_MCLK )        CRG->SSPCLK_SSR = CRG_SSPCLK_SSR_MCLK;
    else if ( src == CRG_RCLK )        CRG->SSPCLK_SSR = CRG_SSPCLK_SSR_RCLK;
    else                               CRG->SSPCLK_SSR = CRG_SSPCLK_SSR_OCLK;
}

void CRG_SSPCLK_SetPrescale(CRG_PREDIV prediv)
{
    assert_param(IS_CRG_SSPCLK_PREDIV(prediv));

    if      ( prediv == CRG_PREDIV1 )   CRG->SSPCLK_PVSR = CRG_SSPCLK_PVSR_DIV1; 
    else if ( prediv == CRG_PREDIV2 )   CRG->SSPCLK_PVSR = CRG_SSPCLK_PVSR_DIV2; 
    else if ( prediv == CRG_PREDIV4 )   CRG->SSPCLK_PVSR = CRG_SSPCLK_PVSR_DIV4; 
    else                                CRG->SSPCLK_PVSR = CRG_SSPCLK_PVSR_DIV8; 
}

void CRG_ADCCLK_SourceSelect(CRG_CLK_SOURCE src)
{
    assert_param(IS_CRG_ADCCLK_SRC(src));

    if      ( src == CRG_CLK_DIS )   CRG->ADCCLK_SSR = CRG_ADCCLK_SSR_DIS;
    else if ( src == CRG_MCLK )      CRG->ADCCLK_SSR = CRG_ADCCLK_SSR_MCLK;
    else if ( src == CRG_RCLK )      CRG->ADCCLK_SSR = CRG_ADCCLK_SSR_RCLK;
    else                             CRG->ADCCLK_SSR = CRG_ADCCLK_SSR_OCLK;
}

void CRG_ADCCLK_SetPrescale(CRG_PREDIV prediv)
{
    assert_param(IS_CRG_ADCCLK_PREDIV(prediv));

    if      ( prediv == CRG_PREDIV1 )   CRG->ADCCLK_PVSR = CRG_ADCCLK_PVSR_DIV1; 
    else if ( prediv == CRG_PREDIV2 )   CRG->ADCCLK_PVSR = CRG_ADCCLK_PVSR_DIV2; 
    else if ( prediv == CRG_PREDIV4 )   CRG->ADCCLK_PVSR = CRG_ADCCLK_PVSR_DIV4; 
    else                                CRG->ADCCLK_PVSR = CRG_ADCCLK_PVSR_DIV8; 
}

void CRG_TIMERCLK_SourceSelect(CRG_TIMER num, CRG_CLK_SOURCE src)
{
    assert_param(IS_CRG_TIMERCLK_NUM(num));
    assert_param(IS_CRG_TIMERCLK_SRC(src));

    if      ( src == CRG_CLK_DIS )   CRG_SET_TIMERCLK_SSR(num,CRG_TIMERCLK_SSR_DIS);
    else if ( src == CRG_MCLK )      CRG_SET_TIMERCLK_SSR(num,CRG_TIMERCLK_SSR_MCLK);
    else if ( src == CRG_RCLK )      CRG_SET_TIMERCLK_SSR(num,CRG_TIMERCLK_SSR_RCLK); 
    else                             CRG_SET_TIMERCLK_SSR(num,CRG_TIMERCLK_SSR_OCLK);
}

void CRG_TIMERCLK_SetPrescale(CRG_TIMER num, CRG_PREDIV prediv)
{
    assert_param(IS_CRG_TIMERCLK_NUM(num));
    assert_param(IS_CRG_TIMERCLK_PREDIV(prediv));

    if      ( prediv == CRG_PREDIV1 )   CRG_SET_TIMERCLK_PREDIV(num,CRG_TIMERCLK_PVSR_DIV1); 
    else if ( prediv == CRG_PREDIV2 )   CRG_SET_TIMERCLK_PREDIV(num,CRG_TIMERCLK_PVSR_DIV2); 
    else if ( prediv == CRG_PREDIV4 )   CRG_SET_TIMERCLK_PREDIV(num,CRG_TIMERCLK_PVSR_DIV4);
    else if ( prediv == CRG_PREDIV8 )   CRG_SET_TIMERCLK_PREDIV(num,CRG_TIMERCLK_PVSR_DIV8);
    else if ( prediv == CRG_PREDIV16 )  CRG_SET_TIMERCLK_PREDIV(num,CRG_TIMERCLK_PVSR_DIV16);
    else if ( prediv == CRG_PREDIV32 )  CRG_SET_TIMERCLK_PREDIV(num,CRG_TIMERCLK_PVSR_DIV32);
    else if ( prediv == CRG_PREDIV64 )  CRG_SET_TIMERCLK_PREDIV(num,CRG_TIMERCLK_PVSR_DIV64);
    else                                CRG_SET_TIMERCLK_PREDIV(num,CRG_TIMERCLK_PVSR_DIV128);
}

void CRG_PWMCLK_SourceSelect(CRG_PWM num, CRG_CLK_SOURCE src)
{
    assert_param(IS_CRG_PWMCLK_NUM(num));
    assert_param(IS_CRG_PWMCLK_SRC(src));

    if      ( src == CRG_CLK_DIS )   CRG_SET_PWMCLK_SSR(num,CRG_PWMCLK_SSR_DIS);
    else if ( src == CRG_MCLK )      CRG_SET_PWMCLK_SSR(num,CRG_PWMCLK_SSR_MCLK);
    else if ( src == CRG_RCLK )      CRG_SET_PWMCLK_SSR(num,CRG_PWMCLK_SSR_RCLK); 
    else                             CRG_SET_PWMCLK_SSR(num,CRG_PWMCLK_SSR_OCLK);
}

void CRG_PWMCLK_SetPrescale(CRG_PWM num, CRG_PREDIV prediv)
{
    assert_param(IS_CRG_PWMCLK_NUM(num));
    assert_param(IS_CRG_PWMCLK_PREDIV(prediv));

    if      ( prediv == CRG_PREDIV1 )   CRG_SET_PWMCLK_PREDIV(num,CRG_PWMCLK_PVSR_DIV1); 
    else if ( prediv == CRG_PREDIV2 )   CRG_SET_PWMCLK_PREDIV(num,CRG_PWMCLK_PVSR_DIV2); 
    else if ( prediv == CRG_PREDIV4 )   CRG_SET_PWMCLK_PREDIV(num,CRG_PWMCLK_PVSR_DIV4);
    else if ( prediv == CRG_PREDIV8 )   CRG_SET_PWMCLK_PREDIV(num,CRG_PWMCLK_PVSR_DIV8);
    else if ( prediv == CRG_PREDIV16 )  CRG_SET_PWMCLK_PREDIV(num,CRG_PWMCLK_PVSR_DIV16);
    else if ( prediv == CRG_PREDIV32 )  CRG_SET_PWMCLK_PREDIV(num,CRG_PWMCLK_PVSR_DIV32);
    else if ( prediv == CRG_PREDIV64 )  CRG_SET_PWMCLK_PREDIV(num,CRG_PWMCLK_PVSR_DIV64);
    else                                CRG_SET_PWMCLK_PREDIV(num,CRG_PWMCLK_PVSR_DIV128);
}

void CRG_RTC_HS_SourceSelect(CRG_CLK_SOURCE src)
{
    assert_param(IS_CRG_RTC_HS_SRC(src));

    if      ( src == CRG_CLK_DIS )   CRG->RTC_HS_SSR = CRG_RTC_HS_SSR_DIS;
    else if ( src == CRG_MCLK )      CRG->RTC_HS_SSR = CRG_RTC_HS_SSR_MCLK;
    else if ( src == CRG_RCLK )      CRG->RTC_HS_SSR = CRG_RTC_HS_SSR_RCLK;
    else                             CRG->RTC_HS_SSR = CRG_RTC_HS_SSR_OCLK;

    if ( src != CRG_CLK_DIS )        CRG_RTC_SourceSelect(CRG_CLK_HIGH);
}

void CRG_RTC_HS_SetPrescale(CRG_PREDIV prediv)
{
    assert_param(IS_CRG_RTC_HS_PREDIV(prediv));

    if      ( prediv == CRG_PREDIV1 )   CRG->RTC_HS_PVSR = CRG_RTC_HS_PVSR_DIV1; 
    else if ( prediv == CRG_PREDIV2 )   CRG->RTC_HS_PVSR = CRG_RTC_HS_PVSR_DIV2; 
    else if ( prediv == CRG_PREDIV4 )   CRG->RTC_HS_PVSR = CRG_RTC_HS_PVSR_DIV4;
    else if ( prediv == CRG_PREDIV8 )   CRG->RTC_HS_PVSR = CRG_RTC_HS_PVSR_DIV8;
    else if ( prediv == CRG_PREDIV16 )  CRG->RTC_HS_PVSR = CRG_RTC_HS_PVSR_DIV16;
    else if ( prediv == CRG_PREDIV32 )  CRG->RTC_HS_PVSR = CRG_RTC_HS_PVSR_DIV32;
    else if ( prediv == CRG_PREDIV64 )  CRG->RTC_HS_PVSR = CRG_RTC_HS_PVSR_DIV64;
    else                                CRG->RTC_HS_PVSR = CRG_RTC_HS_PVSR_DIV128;
}

void CRG_RTC_SourceSelect(CRG_CLK_LOW_SOURCE src)
{
    assert_param(IS_CRG_RTC_LOW_SRC(src));

    if (src == CRG_CLK_LOW)
    {
        CRG_RTC_HS_SourceSelect(CRG_CLK_DIS);
        CRG->RTC_SSR = CRG_RTC_SSR_LW; 
    }
    else
    {
        CRG->RTC_SSR = CRG_RTC_SSR_HS;
    }
}

void CRG_WDOGCLK_HS_SourceSelect(CRG_CLK_SOURCE src)
{
    assert_param(IS_CRG_WDOGCLK_HS_SRC(src));

    if      ( src == CRG_CLK_DIS )   CRG->WDOGCLK_HS_SSR = CRG_WDOGCLK_HS_SSR_DIS;
    else if ( src == CRG_MCLK )      CRG->WDOGCLK_HS_SSR = CRG_WDOGCLK_HS_SSR_MCLK;
    else if ( src == CRG_RCLK )      CRG->WDOGCLK_HS_SSR = CRG_WDOGCLK_HS_SSR_RCLK;
    else                             CRG->WDOGCLK_HS_SSR = CRG_WDOGCLK_HS_SSR_OCLK;

    if ( src != CRG_CLK_DIS )        CRG_WDOGCLK_SourceSelect(CRG_CLK_HIGH);
}

void CRG_WDOGCLK_HS_SetPrescale(CRG_PREDIV prediv)
{
    assert_param(IS_CRG_WDOGCLK_HS_PREDIV(prediv));

    if      ( prediv == CRG_PREDIV1 )   CRG->WDOGCLK_HS_PVSR = CRG_WDOGCLK_HS_PVSR_DIV1; 
    else if ( prediv == CRG_PREDIV2 )   CRG->WDOGCLK_HS_PVSR = CRG_WDOGCLK_HS_PVSR_DIV2; 
    else if ( prediv == CRG_PREDIV4 )   CRG->WDOGCLK_HS_PVSR = CRG_WDOGCLK_HS_PVSR_DIV4;
    else if ( prediv == CRG_PREDIV8 )   CRG->WDOGCLK_HS_PVSR = CRG_WDOGCLK_HS_PVSR_DIV8;
    else if ( prediv == CRG_PREDIV16 )  CRG->WDOGCLK_HS_PVSR = CRG_WDOGCLK_HS_PVSR_DIV16;
    else if ( prediv == CRG_PREDIV32 )  CRG->WDOGCLK_HS_PVSR = CRG_WDOGCLK_HS_PVSR_DIV32;
    else if ( prediv == CRG_PREDIV64 )  CRG->WDOGCLK_HS_PVSR = CRG_WDOGCLK_HS_PVSR_DIV64;
    else                                CRG->WDOGCLK_HS_PVSR = CRG_WDOGCLK_HS_PVSR_DIV128;
}

void CRG_WDOGCLK_SourceSelect(CRG_CLK_LOW_SOURCE src)
{
    assert_param(IS_CRG_WDOGCLK_LOW_SRC(src));

    if (src == CRG_CLK_LOW)
    {
        CRG_WDOGCLK_HS_SourceSelect(CRG_CLK_DIS);
        CRG->WDOGCLK_SSR = CRG_WDOGCLK_SSR_LW; 
    }
    else
    {
        CRG->WDOGCLK_SSR = CRG_WDOGCLK_SSR_HS;
    }
}

void CRG_UARTCLK_SourceSelect(CRG_CLK_SOURCE src)
{
    assert_param(IS_CRG_UARTCLK_SRC(src));

    if      ( src == CRG_CLK_DIS )   CRG->UARTCLK_SSR = CRG_UARTCLK_SSR_DIS;
    else if ( src == CRG_MCLK )      CRG->UARTCLK_SSR = CRG_UARTCLK_SSR_MCLK;
    else if ( src == CRG_RCLK )      CRG->UARTCLK_SSR = CRG_UARTCLK_SSR_RCLK;
    else                             CRG->UARTCLK_SSR = CRG_UARTCLK_SSR_OCLK;
}

void CRG_UARTCLK_SetPrescale(CRG_PREDIV prediv)
{
    assert_param(IS_CRG_UARTCLK_PREDIV(prediv));

    if      ( prediv == CRG_PREDIV1 )   CRG->UARTCLK_PVSR = CRG_UARTCLK_PVSR_DIV1; 
    else if ( prediv == CRG_PREDIV2 )   CRG->UARTCLK_PVSR = CRG_UARTCLK_PVSR_DIV2; 
    else if ( prediv == CRG_PREDIV4 )   CRG->UARTCLK_PVSR = CRG_UARTCLK_PVSR_DIV4; 
    else                                CRG->UARTCLK_PVSR = CRG_UARTCLK_PVSR_DIV8; 
}

void CRG_MII_Enable(FunctionalState rx_clk, FunctionalState tx_clk)
{
    assert_param(IS_FUNCTIONAL_STATE(rx_clk));
    assert_param(IS_FUNCTIONAL_STATE(tx_clk));

    if  ( rx_clk != DISABLE )   CRG->MIICLK_ECR |= CRG_MIICLK_ECR_EN_RXCLK;
    else                        CRG->MIICLK_ECR &= ~(CRG_MIICLK_ECR_EN_RXCLK);    

    if  ( tx_clk != DISABLE )   CRG->MIICLK_ECR |= CRG_MIICLK_ECR_EN_TXCLK;
    else                        CRG->MIICLK_ECR &= ~(CRG_MIICLK_ECR_EN_TXCLK);
}

void CRG_SetMonitoringClock(uint32_t value)
{
    assert_param(IS_CRG_MONCLK_SSR(value));

    CRG->MONCLK_SSR = value;
}

uint32_t CRG_GetMonitoringClock(void)
{
    return (uint8_t)CRG->MONCLK_SSR;
}

