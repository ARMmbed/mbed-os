#if !defined (CY_COMP_H)
#define CY_COMP_H

#include "cy_device_headers.h"

#if defined (CY_IP_MXS28ADCCOMP)

#include "cy_syslib.h"

#if defined(__cplusplus)
extern "C" {
#endif



/**
* \addtogroup group_comp_macros_interrupt_masks Interrupt Masks
* \{
*/

/** Bit 0: Comparator 1 interrupt mask. */
#define CY_COMP_1         (MXS28ADCCOMP_INTR_INTERRUPT_LPCOMP1_Msk)
/** Bit 1: Comparator 2 interrupt mask. */
#define CY_COMP_2         (MXS28ADCCOMP_INTR_INTERRUPT_LPCOMP2_Msk)
/** Bits 1 and 2: Combined mask for both comparators. */
#define CY_COMP_BOTH      (CY_COMP_1 | CY_COMP_2)

/** \} group_comp_macros_interrupt_masks */

/**< COMP PDL ID */
#define CY_COMP_ID                                CY_PDL_DRV_ID(0x47u)





/** The COMP error codes. */
typedef enum
{
    CY_COMP_SUCCESS   = 0x00U,                                        /**< Successful */
    CY_COMP_BAD_PARAM = CY_COMP_ID | CY_PDL_STATUS_ERROR | 0x01U,     /**< One or more invalid parameters */
    CY_COMP_UNKNOWN   = CY_COMP_ID | CY_PDL_STATUS_ERROR | 0xFFU,     /**< Unknown failure */
} cy_en_comp_status_t;


typedef enum
{
    CY_COMP_IN_N_GPIO04 = 0x00U,   /**< GPIO0 for COMP2 and GPIO4 for COMP1 */
    CY_COMP_IN_N_GPIO15 = 0x01U    /**< GPIO1 for COMP2 and GPIO5 for COMP1 */
} cy_en_comp_in_n_t;


typedef enum
{
    CY_COMP_IN_P_GPIO26 = 0x00U,   /**< GPIO2 for COMP2 and GPIO6 for COMP1 */
    CY_COMP_IN_P_GPIO37 = 0x01U,   /**< GPIO3 for COMP2 and GPIO7 for COMP1 */
    CY_COMP_IN_P_MIC    = 0x02U    /**< MIC for both COMP2 and COMP1 */
} cy_en_comp_in_p_t;


typedef struct
{
    uint32_t          chanMsk; /**< Channel mask */
    uint32_t          hyst;
    bool              hyst_x2;
    bool              ntd_en;
    cy_en_comp_in_n_t inN;
    cy_en_comp_in_p_t inP;
} cy_stc_comp_config_t;


cy_en_comp_status_t Cy_COMP_Init(MXS28ADCCOMP_Type* base, cy_stc_comp_config_t const * config);
void Cy_COMP_DeInit(MXS28ADCCOMP_Type* base, uint32_t chanMsk);


/*******************************************************************************
* Function Name: Cy_COMP_GetInterruptStatus
****************************************************************************//**
*
* Returns the interrupt status of the specified channel.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \return
* The interrupt status, see \ref group_comp_macros_interrupt_masks.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_COMP_GetInterruptStatus
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_COMP_GetInterruptStatus(MXS28ADCCOMP_Type const * base)
{
    return (base->INTR & CY_COMP_BOTH);
}


/*******************************************************************************
* Function Name: Cy_ADC_ClearInterrupt
****************************************************************************//**
*
* Clears the interrupt status.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_ADC_ClearInterrupt
*
*******************************************************************************/
__STATIC_INLINE void Cy_COMP_ClearInterrupt(MXS28ADCCOMP_Type * base, uint32_t interrupt)
{
    base->INTR = interrupt & CY_COMP_BOTH;
}


/*******************************************************************************
* Function Name: Cy_COMP_SetInterrupt
****************************************************************************//**
*
* Sets the interrupt for the specified channel.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \param interrupt
* The interrupt mask. See \ref group_comp_macros_interrupt_masks.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_COMP_SetInterruptMask
*
*******************************************************************************/
__STATIC_INLINE void Cy_COMP_SetInterrupt(MXS28ADCCOMP_Type * base, uint32_t interrupt)
{
    base->INTR_SET = interrupt & CY_COMP_BOTH;
}


/*******************************************************************************
* Function Name: Cy_COMP_GetInterruptMask
****************************************************************************//**
*
* Returns the interrupt mask value of the specified channel.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \return
* The interrupt mask value. See \ref group_comp_macros_interrupt_masks.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_COMP_SetInterruptMask
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_COMP_GetInterruptMask(MXS28ADCCOMP_Type const * base)
{
    return (base->INTR_MASK & CY_COMP_BOTH);
}


/*******************************************************************************
* Function Name: Cy_COMP_SetInterruptMask
****************************************************************************//**
*
* Sets an interrupt mask value for the specified comparator.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \param interrupt
* The interrupt mask, see \ref group_comp_macros_interrupt_masks.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_COMP_SetInterruptMask
*
*******************************************************************************/
__STATIC_INLINE void Cy_COMP_SetInterruptMask(MXS28ADCCOMP_Type * base, uint32_t interrupt)
{
    uint32_t interruptState = Cy_SysLib_EnterCriticalSection();
    base->INTR_MASK = (base->INTR_MASK & ~CY_COMP_BOTH) | (interrupt & CY_COMP_BOTH);
    Cy_SysLib_ExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Cy_COMP_GetInterruptStatusMasked
****************************************************************************//**
*
* Returns the logical AND of the corresponding INTR and INTR_MASK fields
* in a single-load operation.
*
* \param base
* The pointer to the hardware ADCCOMP block.
*
* \return
* The masked interrupt status. See \ref group_comp_macros_interrupt_masks.
*
* \funcusage
* \snippet adc/snippet/main.c snippet_Cy_COMP_ClearInterrupt
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_COMP_GetInterruptStatusMasked(MXS28ADCCOMP_Type const * base)
{
    return (base->INTR_MASKED & CY_COMP_BOTH);
}




#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_ADCCOMP */

#endif  /* (CY_COMP_H) */
