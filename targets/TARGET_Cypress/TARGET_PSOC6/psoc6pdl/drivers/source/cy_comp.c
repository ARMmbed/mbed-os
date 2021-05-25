#include "cy_device.h"

#if defined (CY_IP_MXS28ADCCOMP)

#include "cy_comp.h"

/*******************************************************************************
* Function Name: Cy_Comp_Init
****************************************************************************//**
*
*  Initializes ADCCOMP comparators.
*
* \param *base
*     LPCOMP registers structure pointer.
*
* \param *config
*     The pointer to the configuration structure for PDL.
*
* \return cy_en_lpcomp_status_t
*     *base checking result. If the pointer is NULL, returns error.
*
*******************************************************************************/
cy_en_comp_status_t Cy_COMP_Init(MXS28ADCCOMP_Type* base, cy_stc_comp_config_t const * config)
{
    cy_en_comp_status_t ret = CY_COMP_BAD_PARAM;

    if ((NULL != base) && (NULL != config))
    {
        uint32_t interruptState;
        uint32_t locLpCompCtrlMsk = 0UL;
        uint32_t locLpCompCtrlVal = 0UL;

        if (0UL != (CY_COMP_1 & config->chanMsk))
        {
            locLpCompCtrlMsk = MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP1_HYST_Msk |
                               MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP1_HYST_X2_Msk |
                               MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP1_NTD_EN_Msk;

            locLpCompCtrlVal = _VAL2FLD(MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP1_HYST, config->hyst) |
                              _BOOL2FLD(MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP1_HYST_X2, config->hyst_x2) |
                              _BOOL2FLD(MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP1_NTD_EN, config->ntd_en);
        }

        if (0UL != (CY_COMP_2 & config->chanMsk))
        {
            locLpCompCtrlMsk |= MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP2_HYST_Msk |
                                MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP2_HYST_X2_Msk |
                                MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP2_NTD_EN_Msk;

            locLpCompCtrlVal |= _VAL2FLD(MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP2_HYST, config->hyst) |
                               _BOOL2FLD(MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP2_HYST_X2, config->hyst_x2) |
                               _BOOL2FLD(MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP2_NTD_EN, config->ntd_en);
        }

        interruptState = Cy_SysLib_EnterCriticalSection();
        base->LPCOMP_CTRL = (base->LPCOMP_CTRL & ~locLpCompCtrlMsk) | locLpCompCtrlVal;
        Cy_SysLib_ExitCriticalSection(interruptState);

        ret = CY_COMP_SUCCESS;
    }

    return (ret);
}


/*******************************************************************************
* Function Name: Cy_Comp_DeInit
****************************************************************************//**
*
*  Deinitializes ADCCOMP comparators.
*
* \param *base
*     LPCOMP registers structure pointer.
*
* \param chanMsk
*     The channel mask specifies channel(s) which should be deinitialized.
*
*******************************************************************************/
void Cy_COMP_DeInit(MXS28ADCCOMP_Type* base, uint32_t chanMsk)
{
    if (NULL != base)
    {
        uint32_t interruptState;
        uint32_t locLpCompCtrlMsk = 0UL;

        if (0UL != (CY_COMP_1 & chanMsk))
        {
            locLpCompCtrlMsk = MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP1_HYST_Msk |
                               MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP1_HYST_X2_Msk |
                               MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP1_NTD_EN_Msk;
        }

        if (0UL != (CY_COMP_2 & chanMsk))
        {
            locLpCompCtrlMsk |= MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP2_HYST_Msk |
                                MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP2_HYST_X2_Msk |
                                MXS28ADCCOMP_LPCOMP_CTRL_LPCOMP2_NTD_EN_Msk;
        }

        interruptState = Cy_SysLib_EnterCriticalSection();
        base->LPCOMP_CTRL &= ~locLpCompCtrlMsk;
        Cy_SysLib_ExitCriticalSection(interruptState);
    }
}

#endif /* CY_IP_MXS28ADCCOMP */
