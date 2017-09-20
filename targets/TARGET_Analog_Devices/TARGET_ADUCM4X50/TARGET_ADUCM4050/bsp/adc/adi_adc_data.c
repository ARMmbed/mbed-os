#ifndef ADI_ADC_DATA_C
#define ADI_ADC_DATA_C

#include <drivers/adc/adi_adc.h>
#include <drivers/dma/adi_dma.h>
#include "adi_adc_def.h"

/*! \cond PRIVATE */

static ADI_ADC_INFO AdcDevInfo[] = {
    {
        NULL,
        (ADI_ADC_TypeDef*)REG_ADC0_CFG
    }
};

/*! \endcond */

#endif /* ADI_ADC_DATA_C */
