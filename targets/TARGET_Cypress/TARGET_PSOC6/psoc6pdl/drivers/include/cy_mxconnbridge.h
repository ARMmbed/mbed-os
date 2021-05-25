/***************************************************************************//**
* \file cy_mxconnbridge.h
* \version 1.0
*
* This file provides an API declaration of the MXCONNBRIDGE driver
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_connbridge
* \{
* MXCONNBRIDGE IP acts as a bridging interface between the MCU Subsystem and
* WLAN Subsystem. The IP handles signal routing between the two subsystems, and
* also handles AHB based register access from MCUSS side to access WLAN
* subsystem resources. The high level functionalities supported by the IP
* include:
* 1) Enables AHB access to the Global Coexistence Interface (GCI) block on the
* WLAN side through an internal bridge.
* 2) Provides interrupt generation, masking capability from the GCI block to
* the MCUSS
* 3) Enables AHB register access to control RF switch control IO's, and
* read/write WLAN GPIOs
* 4) Enables configuration of the WLAN reset logic, WLAN SDIO signal
* multiplexing.
* 
* \section group_connbridge_more_information More Information
* For more information on the Connbridge peripheral, refer to the technical reference manual (TRM).
*
* \section group_connbridge_MISRA MISRA-C Compliance
* The Connbridge driver does not have any specific deviations.
*
* \section group_connbridge_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_connbridge_functions Functions
*/

#if !defined (CY_MXCONNBRIDGE_H)
#define CY_MXCONNBRIDGE_H

#include "cy_device.h"

#if defined (CY_IP_MXCONNBRIDGE)

#include <stddef.h>
#include "cy_syslib.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** \addtogroup group_connbridge_macros
* \{
*/

/** Driver major version */
#define CY_CONNBRIDGE_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_CONNBRIDGE_DRV_VERSION_MINOR       0

/** Connbridge driver ID */
#define CY_CONNBRIDGE_ID CY_PDL_DRV_ID(0x49U)

/** \} group_connbridge_macros */

/***************************************
*        Constants
***************************************/

/** \cond INTERNAL */

/* Parameter validation constants */
#define CY_MXCONNBRIDGE_INTRS_MAX                       (4UL)       /**< Number of interrupt lines */
#define CY_MXCONNBRIDGE_RFCTL_MAX                       (32UL)      /**< Number of RF controls */
#define CY_MXCONNBRIDGE_GPIO_PIN_MAX                    (8UL)      /**< Number of GPIO IN pins */


/* connbridge Mask */
#define CY_MXCONNBRIDGE_INTR_STATUS_MASK                (0x01UL)    /**< Single interrupt line mask for interrupt status in INTR register */
#define CY_MXCONNBRIDGE_INTR_EN_MASK                    (0x01UL)    /**< Single interrupt line mask for interrupt in INTR register */
#define CY_MXCONNBRIDGE_RFCTL_EN_MASK                   (0x01UL)    /**< RF control enable mask for RF_SWITCH_CTRL register */
#define CY_MXCONNBRIDGE_GPIO_PIN_MASK                   (0x01UL)    /**< GPIO IN/OUT bit mask for GPIO_IN and GPIO_OUT register */
#define CY_MXCONNBRIDGE_WLAN_RESET_STATUS_MASK          (0x01UL)
#define CY_MXCONNBRIDGE_WLAN_RESET_MUX_MASK             (0x01UL)
#define CY_MXCONNBRIDGE_WLAN_SDIO_MUX_MASK              (0x01UL)

/* Offsets */
#define CY_MXCONNBRIDGE_WLAN_RESET_STATUS_OFFSET        (0UL)      /**< Offset for HSIOM */
#define CY_MXCONNBRIDGE_WLAN_RESET_MUX_OFFSET           (1UL)      /**< Offset for HSIOM */
#define CY_MXCONNBRIDGE_WLAN_SDIO_MUX_OFFSET            (2UL)      /**< Offset for HSIOM */


/* Parameter validation macros */
#define CY_MXCONNBRIDGE_IS_INTRNUM_VALID(intrNum)       (CY_MXCONNBRIDGE_INTRS_MAX > (intrNum))
#define CY_MXCONNBRIDGE_IS_RFCTLNUM_VALID(rfNum)        (CY_MXCONNBRIDGE_RFCTL_MAX > (rfNum))
#define CY_MXCONNBRIDGE_IS_VALUE_VALID(outVal)          (1UL >= (outVal))
#define CY_MXCONNBRIDGE_IS_GPIO_PIN_VALID(pinNum)       (CY_MXCONNBRIDGE_GPIO_PIN_MAX > (pinNum))

/** \endcond */

/**
* \addtogroup group_connbridge_functions
* \{
*/
__STATIC_INLINE void Cy_MXCONNBRIDGE_Enable(MXCONNBRIDGE_Type *base);
__STATIC_INLINE void Cy_MXCONNBRIDGE_Disable(MXCONNBRIDGE_Type *base);
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetInterruptStatusMasked(MXCONNBRIDGE_Type *base, uint32_t intrNum);
__STATIC_INLINE void Cy_MXCONNBRIDGE_SetInterruptMask(MXCONNBRIDGE_Type* base, uint32_t intrNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetInterruptMask(MXCONNBRIDGE_Type* base, uint32_t intrNum);
__STATIC_INLINE void Cy_MXCONNBRIDGE_SetRfSwitchControl(MXCONNBRIDGE_Type *base, uint32_t rfNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetRfSwitchControl(MXCONNBRIDGE_Type* base, uint32_t rfNum);
__STATIC_INLINE void Cy_MXCONNBRIDGE_WriteGpioInPin(MXCONNBRIDGE_Type *base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetGpioInPin(MXCONNBRIDGE_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_MXCONNBRIDGE_WriteGpioOutPin(MXCONNBRIDGE_Type *base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetGpioOutPin(MXCONNBRIDGE_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_MXCONNBRIDGE_DEVWAKEEnable(MXCONNBRIDGE_Type *base);
__STATIC_INLINE void Cy_MXCONNBRIDGE_DEVWAKEDisable(MXCONNBRIDGE_Type *base);
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_WLANResetStatus(MXCONNBRIDGE_Type *base);
void Cy_MXCONNBRIDGE_WLANResetMux(MXCONNBRIDGE_Type *base, uint32_t value);
void Cy_MXCONNBRIDGE_SetWLANSDIOMux(MXCONNBRIDGE_Type *base, uint32_t value);
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetWLANSDIOMux(MXCONNBRIDGE_Type *base);


/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_Enable
****************************************************************************//**
*
* Enables the mxconnbridge ip
*
* \param base
* The pointer to a MXCONNBRIDGE instance.
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXCONNBRIDGE_Enable(MXCONNBRIDGE_Type *base)
{
    MXCONNBRIDGE_CTL(base) = (unsigned int)(1<<31);
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_Disable
****************************************************************************//**
*
* Disables the mxconnbridge ip
*
* \param base
* The pointer to a MXCONNBRIDGE instance.
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXCONNBRIDGE_Disable(MXCONNBRIDGE_Type *base)
{
    MXCONNBRIDGE_CTL(base) = 0UL;
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_GetInterruptStatusMasked
****************************************************************************//**
*
* Returns the interrupt status for incoming signal interrupt_0, interrupt_1, 
* interrupt_2 and interrupt_3 after HW masking
*
* \param base
* The pointer to a MXCONNBRIDGE instance.
*
* \param intrNum
* Interrupt number (incoming signal)
*
* \return
* 0 = interrupt condition not detected
* 1 = interrupt condition detected
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetInterruptStatusMasked(MXCONNBRIDGE_Type *base, uint32_t intrNum)
{
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_INTRNUM_VALID(intrNum));
    return (MXCONNBRIDGE_INTR_STATUS(base) >> intrNum) & CY_MXCONNBRIDGE_INTR_STATUS_MASK;
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_SetInterruptMask
****************************************************************************//**
*
* Configures the interrupt line to be forwarded to the CPU NVIC.
*
* \param base
* Pointer to the mxconnbridge register base address
*
* \param intrNum
* Incoming intrrupt line number.
*
* \param value
* 0 = interrupt not forwarded to CPU interrupt controller
* 1 = interrupt masked and forwarded to CPU interrupt controller
*
* \note
* This function modifies a register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple interrupt lines.
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXCONNBRIDGE_SetInterruptMask(MXCONNBRIDGE_Type* base, uint32_t intrNum, uint32_t value)
{
    uint32_t tempReg;

    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_INTRNUM_VALID(intrNum));
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_VALUE_VALID(value));

    tempReg= MXCONNBRIDGE_INTR_MASK(base) & ~(CY_MXCONNBRIDGE_INTR_EN_MASK << intrNum);
    MXCONNBRIDGE_INTR_MASK(base) = tempReg | ((value & CY_MXCONNBRIDGE_INTR_EN_MASK) << intrNum);
}


/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_GetInterruptMask
****************************************************************************//**
*
* Returns the state of the interrupt mask of interrupt line.
*
* This mask is used to determine whether the line is configured to be forwarded
* to the CPU NVIC.
*
* \param base
* Pointer to the mxconnbridge base address
*
* \param intrNum
* Position of the line bit-field within the mask register.
*
* \return
* 0 = interrupt not forwarded to CPU interrupt controller
* 1 = interrupt masked and forwarded to CPU interrupt controller
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetInterruptMask(MXCONNBRIDGE_Type* base, uint32_t intrNum)
{
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_INTRNUM_VALID(intrNum));

    return (MXCONNBRIDGE_INTR_MASK(base) >> intrNum) & CY_MXCONNBRIDGE_INTR_EN_MASK;
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_SetRfSwitchControl
****************************************************************************//**
*
* Configure to enable/disable RF SWITCH CONTROL
*
* \param base
* Pointer to the mxconnbridge base address
*
* \param rfNum
* Position of the line bit-field within the rf switch ctrl register.
*
* \param value
* 0 = dissable rf switch control
* 1 = enable rf switch control
*
* \note
* This function modifies a register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple rf switch ctrl.
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXCONNBRIDGE_SetRfSwitchControl(MXCONNBRIDGE_Type *base, uint32_t rfNum, uint32_t value)
{
    uint32_t tempReg;

    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_RFCTLNUM_VALID(rfNum));
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_VALUE_VALID(value));

    tempReg= MXCONNBRIDGE_RF_SWITCH_CTRL(base) & ~(CY_MXCONNBRIDGE_RFCTL_EN_MASK << rfNum);
    MXCONNBRIDGE_RF_SWITCH_CTRL(base) = tempReg | ((value & CY_MXCONNBRIDGE_RFCTL_EN_MASK) << rfNum);
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_GetRfSwitchControl
****************************************************************************//**
*
* Returns the state of the RF switch ctrl.
*
* \param base
* Pointer to the mxconnbridge base address
*
* \param rfNum
* Position of the line bit-field within the rf switch ctrl register.
*
* \return
* 0 = RF switch ctrl enabled
* 1 = RF switch ctrl disabled
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetRfSwitchControl(MXCONNBRIDGE_Type* base, uint32_t rfNum)
{
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_RFCTLNUM_VALID(rfNum));

    return (MXCONNBRIDGE_RF_SWITCH_CTRL(base) >> rfNum) & CY_MXCONNBRIDGE_RFCTL_EN_MASK;
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_WriteGpioInPin
****************************************************************************//**
*
* Configure to write to a GPIO-IN pin
*
* \param base
* Pointer to the mxconnbridge base address
*
* \param pinNum
* GPIO pin number
*
* \param value
* Logic level to drive out on the pin
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXCONNBRIDGE_WriteGpioInPin(MXCONNBRIDGE_Type *base, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;

    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_GPIO_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_VALUE_VALID(value));

    tempReg= MXCONNBRIDGE_GPIO_IN(base) & ~(CY_MXCONNBRIDGE_GPIO_PIN_MASK << pinNum);
    MXCONNBRIDGE_GPIO_IN(base) = tempReg | ((value & CY_MXCONNBRIDGE_GPIO_PIN_MASK) << pinNum);
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_GetGpioInPin
****************************************************************************//**
*
* Reads the current logic level on the input buffer of the pin.
*
* \param base
* Pointer to the mxconnbridge base address
*
* \param pinNum
* Position of the pin bit-field within the GPIO_IN register.
*
* \return
* Logic level present on the pin
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetGpioInPin(MXCONNBRIDGE_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_GPIO_PIN_VALID(pinNum));

    return (MXCONNBRIDGE_GPIO_IN(base) >> pinNum) & CY_MXCONNBRIDGE_GPIO_PIN_MASK;
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_WriteGpioOutPin
****************************************************************************//**
*
* Configure to write to a GPIO-OUT pin
*
* \param base
* Pointer to the mxconnbridge base address
*
* \param pinNum
* GPIO-OUT pin number
*
* \param value
* Logic level to drive out on the pin
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXCONNBRIDGE_WriteGpioOutPin(MXCONNBRIDGE_Type *base, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;

    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_GPIO_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_VALUE_VALID(value));

    tempReg= MXCONNBRIDGE_GPIO_OUT(base) & ~(CY_MXCONNBRIDGE_GPIO_PIN_MASK << pinNum);
    MXCONNBRIDGE_GPIO_OUT(base) = tempReg | ((value & CY_MXCONNBRIDGE_GPIO_PIN_MASK) << pinNum);
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_GetGpioOutPin
****************************************************************************//**
*
* Reads the current logic level on the input buffer of the pin.
*
* \param base
* Pointer to the mxconnbridge base address
*
* \param pinNum
* Position of the pin bit-field within the GPIO_OUT register.
*
* \return
* Logic level present on the pin
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetGpioOutPin(MXCONNBRIDGE_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_GPIO_PIN_VALID(pinNum));

    return (MXCONNBRIDGE_GPIO_OUT(base) >> pinNum) & CY_MXCONNBRIDGE_GPIO_PIN_MASK;
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_DEVWAKEEnable
****************************************************************************//**
*
* Configure to enable dev-wake signal from mcuss to wlan
*
* \param base
* Pointer to the mxconnbridge base address
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXCONNBRIDGE_DEVWAKEEnable(MXCONNBRIDGE_Type *base)
{
    MXCONNBRIDGE_DEV_WAKE(base) = 1UL;
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_DEVWAKEDisable
****************************************************************************//**
*
* Configure to clear the dev-wake signal from mcuss to wlan
*
* \param base
* Pointer to the mxconnbridge base address
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXCONNBRIDGE_DEVWAKEDisable(MXCONNBRIDGE_Type *base)
{
    MXCONNBRIDGE_DEV_WAKE(base) = 0UL;
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_WLANResetStatus
****************************************************************************//**
*
* Get the status of WLAN RESET signal
*
* \param base
* Pointer to the mxconnbridge base address
*
* \return
* Logic level present on the signal
* 0 = WLAN is in reset
* 1 = WLAN is out of reset
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_WLANResetStatus(MXCONNBRIDGE_Type *base)
{
    return _FLD2VAL(MXCONNBRIDGE_SDIO_RESET_CTL_WLAN_RESET_N_CTL, MXCONNBRIDGE_AP_WLAN_CTL(base));
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_GetWLANSDIOMux
****************************************************************************//**
*
* Reads the current logic level of WLAN_SDIO bit.
*
* \param base
* Pointer to the mxconnbridge base address
*
* \return
* returns sdio mode
*
* \funcusage
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXCONNBRIDGE_GetWLANSDIOMux(MXCONNBRIDGE_Type *base)
{
    return _FLD2VAL(MXCONNBRIDGE_SDIO_RESET_CTL_WLAN_SDIO_MUX,MXCONNBRIDGE_AP_WLAN_CTL(base));
}

/** \} group_connbridge_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCONNBRIDGE */

#endif /* CY_MXCONNBRIDGE_H */

/** \} group_connbridge */

/* [] END OF FILE */
