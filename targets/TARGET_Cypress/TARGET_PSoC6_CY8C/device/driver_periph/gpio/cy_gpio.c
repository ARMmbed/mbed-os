/***************************************************************************//**
* \file cy_gpio.c
* \version 1.0
*
* \brief
* Provides an API implementation of the GPIO driver
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_gpio.h"

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Function Name: Cy_GPIO_Pin_Init
****************************************************************************//**
*
* \brief Initialize all pin configuration setting for the pin.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param config
* Pointer to the pin config structure base address
*
* \return
* void
*
* \note
* This function modifies port registers in read-modify-write operations. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
*******************************************************************************/
cy_en_gpio_status_t Cy_GPIO_Pin_Init(GPIO_PRT_Type *base, uint32_t pinNum, const cy_stc_gpio_pin_config_t *config)
{
    cy_en_gpio_status_t status = CY_GPIO_SUCCESS;
    uint32_t maskCfgOut;
    uint32_t tempReg;

    if((NULL != base) && (NULL != config))
    {
        Cy_GPIO_Write(base, pinNum, config->outVal);
        Cy_GPIO_SetDrivemode(base, pinNum, config->driveMode);
        Cy_GPIO_SetHSIOM(base, pinNum, config->hsiom);

        Cy_GPIO_SetInterruptEdge(base, pinNum, config->intEdge);
        Cy_GPIO_SetInterruptMask(base, pinNum, config->intMask);
        Cy_GPIO_SetVtrip(base, pinNum, config->vtrip);

        /* Slew rate and Driver strength */
        maskCfgOut = (CY_GPIO_CFG_OUT_SLOW_MASK << pinNum) | (CY_GPIO_CFG_OUT_DRIVE_SEL_MASK << (pinNum + CY_GPIO_CFG_OUT_DRIVE_OFFSET));
        tempReg = base->CFG_OUT & ~(maskCfgOut);
        base->CFG_OUT = tempReg | ((config->slewRate & CY_GPIO_CFG_OUT_SLOW_MASK) << pinNum)
                            | ((config->driveSel & CY_GPIO_CFG_OUT_DRIVE_SEL_MASK) << (pinNum + CY_GPIO_CFG_OUT_DRIVE_OFFSET));

        /* SIO specific configuration */
        tempReg = base->CFG_SIO & ~(CY_GPIO_SIO_PIN_MASK);
        base->CFG_SIO = tempReg | (((config->vregEn & CY_GPIO_VREG_EN_MASK)
                                        | ((config->ibufMode & CY_GPIO_IBUF_MASK) << CY_GPIO_IBUF_SHIFT)
                                        | ((config->vtripSel & CY_GPIO_VTRIP_SEL_MASK) << CY_GPIO_VTRIP_SEL_SHIFT)
                                        | ((config->vrefSel & CY_GPIO_VREF_SEL_MASK)  << CY_GPIO_VREF_SEL_SHIFT)
                                        | ((config->vohSel & CY_GPIO_VOH_SEL_MASK) << CY_GPIO_VOH_SEL_SHIFT))
                                            << ((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET));
    }
    else
    {
        status = CY_GPIO_BAD_PARAM;
    }

    return(status);
}


/*******************************************************************************
* Function Name: Cy_GPIO_Port_Init
****************************************************************************//**
*
* \brief Initialize a complete port of pins from a single init structure.
*
* \param base
* Pointer to the pin's port register base address
*
* \param config
* Pointer to the pin config structure base address
*
* \return
* void
*
*******************************************************************************/
cy_en_gpio_status_t Cy_GPIO_Port_Init(GPIO_PRT_Type* base, const cy_stc_gpio_prt_config_t *config)
{
    cy_en_gpio_status_t status = CY_GPIO_SUCCESS;
    uint32_t portNum;
    HSIOM_PRT_Type* baseHSIOM;

    if((NULL != base) && (NULL != config))
    {
        portNum = ((uint32_t)(base) - GPIO_BASE) / GPIO_PRT_SECTION_SIZE;
        baseHSIOM = (HSIOM_PRT_Type*)(HSIOM_BASE + (HSIOM_PRT_SECTION_SIZE * portNum));

        base->OUT            = config->out;
        base->CFG            = config->cfg;
        base->CFG_IN         = config->cfgIn;
        base->CFG_OUT        = config->cfgOut;
        base->INTR_CFG       = config->intrCfg;
        base->INTR_MASK      = config->intrMask;
        base->CFG_SIO        = config->cfgSIO;
        baseHSIOM->PORT_SEL0 = config->sel0Active;
        baseHSIOM->PORT_SEL1 = config->sel1Active;
    }
    else
    {
        status = CY_GPIO_BAD_PARAM;
    }

    return(status);
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
