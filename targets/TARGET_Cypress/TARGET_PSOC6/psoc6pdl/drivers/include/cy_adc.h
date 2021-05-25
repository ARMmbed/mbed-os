#if !defined (CY_ADC_H)
#define CY_ADC_H

#include "cy_device_headers.h"

#if defined (CY_IP_MXS28ADCCOMP)

#include "cy_syslib.h"

#if defined(__cplusplus)
extern "C" {
#endif


/**< ADC PDL ID */
#define CY_ADC_ID                                CY_PDL_DRV_ID(0x46U)

/**
* \addtogroup group_comp_macros_interrupt_masks Interrupt Masks
* \{
*/

/** ADC interrupt mask. */
#define CY_ADC_CIC     (MXS28ADCCOMP_INTR_INTERRUPT_CIC_Msk)

/** \} group_comp_macros_interrupt_masks */


/** ADC enable mask. */
#define CY_ADC             (MXS28ADCCOMP_PD_CTRL_PD_ADC_Msk)
/** GmC calibration mask. */
#define CY_ADC_GMC_CAL     (MXS28ADCCOMP_PD_CTRL_PD_ADC_GMC_CAL_Msk)
/** DC calibration mask. */
#define CY_ADC_DC_CAL      (MXS28ADCCOMP_PD_CTRL_PD_ADC_DC_CAL_Msk)



/** The ADC error codes. */
typedef enum
{
    CY_ADC_SUCCESS   = 0x00u,                                       /**< Successful */
    CY_ADC_BAD_PARAM = CY_ADC_ID | CY_PDL_STATUS_ERROR | 0x01u,     /**< One or more invalid parameters */
    CY_ADC_UNKNOWN   = CY_ADC_ID | CY_PDL_STATUS_ERROR | 0xFFu,     /**< Unknown failure */
} cy_en_adc_status_t;

typedef enum
{
    CY_ADC_CIC_CLK_DIV2_1       = 0UL, /**< Divide by 1 for clock frequency 1024 kHz or 2048 kHz */
    CY_ADC_CIC_CLK_DIV2_2       = 1UL, /**< Divide by 2 for clock frequency 2048 kHz or 4096 kHz */
    CY_ADC_CIC_CLK_DIV2_1_SHIFT = 2UL, /**< Divide by 1 for clock frequency 1024 kHz or 2048 kHz shifted */
    CY_ADC_CIC_CLK_DIV2_2_SHIFT = 3UL  /**< Divide by 2 for clock frequency 2048 kHz or 4096 kHz shifted */
}cy_en_adc_cic_clk_div2_ctrl_t;

typedef enum
{
    CY_ADC_CLK_DIV2_1           = 0UL, /**< Divide by 1 for clock frequency 1024 kHz or 2048 kHz */
    CY_ADC_CLK_DIV2_2           = 1UL  /**< Divide by 2 for clock frequency 2048 kHz or 4096 kHz */
}cy_en_adc_clk_in_div2_ctrl_t;


typedef struct
{
    cy_en_adc_clk_in_div2_ctrl_t clkDiv2;
    uint16_t                   trigDelay;
    bool                         trigClr;
} cy_stc_adc_cic_config_t;

typedef struct
{
    cy_en_adc_clk_in_div2_ctrl_t clkInDiv2;
    cy_stc_adc_cic_config_t *    cicConfig;
} cy_stc_adc_config_t;


cy_en_adc_status_t Cy_ADC_Init(MXS28ADCCOMP_Type * base, cy_stc_adc_config_t const * config);
void Cy_ADC_Enable(MXS28ADCCOMP_Type * base, uint32_t pdMsk);
void Cy_ADC_Disable(MXS28ADCCOMP_Type * base, uint32_t pdMsk);




/*******************************************************************************
* Function Name: Cy_ADC_GetInterruptStatus
****************************************************************************//**
*
* Returns the interrupt status of the specified channel.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \return
* The interrupt status.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_ADC_GetInterruptStatus
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_ADC_GetInterruptStatus(MXS28ADCCOMP_Type const * base)
{
    return (base->INTR & MXS28ADCCOMP_INTR_INTERRUPT_CIC_Msk);
}


/*******************************************************************************
* Function Name: Cy_ADC_ClearInterrupt
****************************************************************************//**
*
* Clears the interrupt status.
*
* \param base
* The pointer to the hardware ADC block.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_ADC_ClearInterrupt
*
*******************************************************************************/
__STATIC_INLINE void Cy_ADC_ClearInterrupt(MXS28ADCCOMP_Type * base, uint32_t interrupt)
{
    base->INTR = MXS28ADCCOMP_INTR_INTERRUPT_CIC_Msk;
}


/*******************************************************************************
* Function Name: Cy_ADC_SetInterrupt
****************************************************************************//**
*
* Sets the interrupt for the specified channel.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_ADC_SetInterruptMask
*
*******************************************************************************/
__STATIC_INLINE void Cy_ADC_SetInterrupt(MXS28ADCCOMP_Type * base, uint32_t interrupt)
{
    base->INTR_SET = MXS28ADCCOMP_INTR_INTERRUPT_CIC_Msk;
}


/*******************************************************************************
* Function Name: Cy_ADC_GetInterruptMask
****************************************************************************//**
*
* Returns the interrupt mask value of the specified channel.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \return
* The interrupt mask value.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_ADC_SetInterruptMask
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_ADC_GetInterruptMask(MXS28ADCCOMP_Type const * base)
{
    return (base->INTR_MASK & MXS28ADCCOMP_INTR_INTERRUPT_CIC_Msk);
}


/*******************************************************************************
* Function Name: Cy_ADC_SetInterruptMask
****************************************************************************//**
*
* Sets an interrupt mask value for the specified channel.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_ADC_SetInterruptMask
*
*******************************************************************************/
__STATIC_INLINE void Cy_ADC_SetInterruptMask(MXS28ADCCOMP_Type * base, uint32_t interrupt)
{
    uint32_t interruptState = Cy_SysLib_EnterCriticalSection();
    base->INTR_MASK = (base->INTR_MASK & ~CY_ADC_CIC) | (interrupt & CY_ADC_CIC);
    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_ADC_GetInterruptStatusMasked
****************************************************************************//**
*
* Returns the logical AND of the corresponding INTR and INTR_MASK fields
* in a single-load operation.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \return
* The masked interrupt status.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_ADC_ClearInterrupt
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_ADC_GetInterruptStatusMasked(MXS28ADCCOMP_Type const * base)
{
    return (base->INTR_MASKED & MXS28ADCCOMP_INTR_INTERRUPT_CIC_Msk);
}



__STATIC_INLINE void Cy_ADC_SetTrigger(MXS28ADCCOMP_Type * base, uint32_t trigger)
{
    base->TRIGGER_SET = trigger & MXS28ADCCOMP_TRIGGER_SET_TR_CIC_Msk;
}

__STATIC_INLINE void Cy_ADC_ClearTrigger(MXS28ADCCOMP_Type * base, uint32_t trigger)
{
    base->TRIGGER_CLR = trigger & MXS28ADCCOMP_TRIGGER_CLR_TR_CIC_Msk;
}




#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_ADCCOMP */

#endif  /* (CY_ADC_H) */
