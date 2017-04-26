#include "W7500x.h"
#include "W7500x_adc.h"

void ADC_PowerDownEnable (FunctionalState NewState)
{
	if (NewState != DISABLE)    ADC->ADC_CTR = ADC_CTR_PWD_PD;
	else                        ADC->ADC_CTR = ADC_CTR_PWD_NRMOP;
}

void ADC_ChannelSelect (ADC_CH num)
{
	assert_param(IS_ADC_CH_NUM(num));
	ADC->ADC_CHSEL = num;
}

void ADC_Start (void)
{
	ADC->ADC_START = ADC_START_START;
}

uint16_t ADC_ReadData (void)
{
	return ((uint16_t)ADC->ADC_DATA);
}

void ADC_InterruptMask (FunctionalState NewState)
{
	if (NewState != DISABLE)    ADC->ADC_INT = ADC_INT_MASK_ENA;
	else                        ADC->ADC_INT = ADC_INT_MASK_DIS;
}

uint8_t ADC_IsInterrupt (void)
{
	return (((uint8_t)ADC->ADC_INT && 0x01ul));
}
		
void ADC_InterruptClear (void)
{
    ADC->ADC_INT = ADC_INTCLEAR;
}

void ADC_Init (void)
{
    // ADC_CLK on
    ADC_PowerDownEnable(ENABLE);
    ADC_PowerDownEnable(DISABLE);
    //ADC_ChannelSelect(num);
}

void ADC_DeInit (void)
{
    // ADC_CLK off
    ADC_PowerDownEnable(ENABLE);
    ADC_InterruptMask(DISABLE);
}

