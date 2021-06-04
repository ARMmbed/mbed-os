#include "cy_device.h"

#if defined (CY_IP_MXS28ADCCOMP)

#include "cy_adc.h"

#define CIC_CTRL_DEFAULT         (_VAL2FLD(MXS28ADCCOMP_CIC_CTRL_CIC_COMB_DELAY_SEL, 0x244U))
//#define CIC_CLOCK_CTRL_DEFAULT   (_VAL2FLD(MXS28ADCCOMP_CIC_CLOCK_CTRL_CLK_EN, 0x3UL))


#define CY_ADC_PD_MASK (MXS28ADCCOMP_PD_CTRL_PD_ADC_Msk | \
                        MXS28ADCCOMP_PD_CTRL_PD_ADC_GMC_CAL_Msk | \
                        MXS28ADCCOMP_PD_CTRL_PD_ADC_DC_CAL_Msk)

/*******************************************************************************
* Function Name: Cy_ADC_Init
****************************************************************************//**
*
* Initializes the ADC block from a pre-initialized configuration structure.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \param config
* The pointer to the configuration structure \ref cy_stc_adc_config_t.
*
* \return
* The status /ref cy_en_adc_status_t.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_ADC_Init
*
*******************************************************************************/
cy_en_adc_status_t Cy_ADC_Init(MXS28ADCCOMP_Type * base, cy_stc_adc_config_t const * config)
{
    cy_en_adc_status_t ret = CY_ADC_BAD_PARAM;

    if ((NULL != base) && (NULL != config))
    {
        /* configure sigma-delta modulator */

        if(NULL != config->cicConfig)
        {
            /* configure decimator */
        }
        else
        {
            /* reset cic registers to their default values */
            base->CIC_CTRL = CIC_CTRL_DEFAULT;
            base->CIC_GAIN = 0UL;
//            base->CIC_CLOCK_CTRL = CIC_CLOCK_CTRL_DEFAULT;
        }

        ret = CY_ADC_SUCCESS;
    }

    return ret;
}



void Cy_ADC_Enable(MXS28ADCCOMP_Type * base, uint32_t pdMsk)
{
    uint32_t interruptState = Cy_SysLib_EnterCriticalSection();
    base->PD_CTRL = (base->PD_CTRL & ~CY_ADC_PD_MASK) | (~pdMsk & CY_ADC_PD_MASK);
    Cy_SysLib_ExitCriticalSection(interruptState);
}

void Cy_ADC_Disable(MXS28ADCCOMP_Type * base, uint32_t pdMsk)
{
    uint32_t interruptState = Cy_SysLib_EnterCriticalSection();
    base->PD_CTRL = (base->PD_CTRL & ~CY_ADC_PD_MASK) | (pdMsk & CY_ADC_PD_MASK);
    Cy_SysLib_ExitCriticalSection(interruptState);
}



cy_en_adc_status_t Cy_ADC_Configure(MXS28ADCCOMP_Type * base, uint32_t clockFreq, uint32_t sampleRate, uint32_t bitWidth)
{
    cy_en_adc_status_t ret = CY_ADC_BAD_PARAM;

    if(true /* arguments are ok */)
    {
        cy_stc_adc_cic_config_t locCicConfig;
        cy_stc_adc_config_t locConfig;

        locConfig.cicConfig = &locCicConfig;


        /* do some math with input parameters */


        ret = Cy_ADC_Init(base, &locConfig);
    }

    return ret;
}

#endif /* CY_IP_MXS28ADCCOMP */
