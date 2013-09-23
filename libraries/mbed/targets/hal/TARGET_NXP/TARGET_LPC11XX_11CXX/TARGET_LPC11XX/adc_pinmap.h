// ADC pin mappings for LPC11XX
// This should be included ONLY from analogin_api.c

#ifndef ADC_PINMAP_H
#define ADC_PINMAP_H

static const PinMap PinMap_ADC[] = {
    {P0_11, ADC0_0, 2},
    {P1_0 , ADC0_1, 2},
    {P1_1 , ADC0_2, 2},
    {P1_2 , ADC0_3, 2},
    // ADC0_4 (P1_3) should be mapped to SWDIO only
    {P1_4 , ADC0_5, 1},
    {P1_10, ADC0_6, 1},
    {P1_11, ADC0_7, 1},
    {NC   , NC    , 0}
};

#endif

