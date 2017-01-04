/*********************************************************************************

Copyright (c) 2011-2013 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
* @file      adi_int.c
*
* @brief     Interrupt handler installation API implementation
*
* @details
*            This is the primary API header file for installing interrupt
*            handlers for all types of interrupts on cortex-M3 processor.
*/

/** @addtogroup RTOS_INTERRUPTS RTOS-specific Interrupt APIs 
 *  @{
 */

#include <services/int/adi_int.h>
#include <adi_processor.h>
#include <services/int/adi_nvic.h>

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*/
#pragma diag_suppress=Pm073,Pm143
#endif /* __ICCARM__ */

#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
/**
 * @brief       Install the handler for a Cortex-M3 Interrupt.
 *
 * @details     This API can be used to install an interrupt handler for
 *              a given system interrupt.
 *
 * @param [in]  iid                 Interrupt ID. For system interrupts this is the
 *                                  system interrupt ID. This will be a unique number
 *                                  to identify a particular interrupt. Please
 *                                  refer to the Hardware Reference Manual to
 *                                  find the number for a specific interrupt.
 *
 * @param [in]  pfHandler           Handler which handles the interrupt. This handler
 *                                  will be called when the interrupt occurs.
 *
 * @param [in]  pCBParam            Application provided callback parameter which
 *                                  will be passed back when the given handler
 *                                  is called.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable/disable
 *                                  the given Interrupt after the
 *                                  interrupt handler installation. The interrupt
 *                                  can be disabled during the installation and
 *                                  enabled later by calling the function
 *                                  adi_int_EnableInt().\n\n
 *                                  This argument should be set to:
 *                                  - 'true'     to enable the interrupt.
 *                                  - 'false'    to disable the interrupt.
 *
 * @return
 *              -  #ADI_INT_SUCCESS  if successfully installed the interrupt
 *                                  handler.
 *              -  #ADI_INT_FAILURE  if it was unable to install the interrupt handler.
 *
 * @sa          adi_int_EnableInt()
 * @sa          adi_int_UninstallHandler()
 *
 */

ADI_INT_STATUS  adi_int_InstallHandler (uint32_t iid,
   ADI_INT_HANDLER_PTR pfHandler,
   void* pCBParam,
   bool_t bEnable)
{
    if (ADI_OSAL_SUCCESS != adi_osal_InstallHandler(iid, pfHandler, pCBParam))
    {
        return ADI_INT_FAILURE;
    }

    /*
     * We've just installed a handler for a system interrupt,
     * now we need to set it up on the NVIC.
     */
    if (bEnable)
    {
        adi_nvic_EnableInt(iid, true);
    }

    return ADI_INT_SUCCESS;
}


/**
 * @brief       Uninstall the interrupt handler
 *
 * @details     Uninstalls the interrupt handler and disables the source and
 *              interrupt.
 *
 * @param [in]  iid         Interrupt ID for which the handler to be uninstalled.
 *
 * @return
 *                - #ADI_INT_SUCCESS  if successfully uninstalled interrupt
 *                handler.
 *                - #ADI_INT_FAILURE if failed to uninstall the given
 *                interrupt handler.
 *
 * @sa adi_int_InstallHandler()
 *
 */
ADI_INT_STATUS  adi_int_UninstallHandler (uint32_t iid)
{
    adi_nvic_EnableInt(iid, false);

	if (ADI_OSAL_SUCCESS != adi_osal_UninstallHandler(iid))
	{
		return ADI_INT_FAILURE;
	}

	return ADI_INT_SUCCESS;
}

/**
 * @brief  Enable or disable the interrupt
 *
 * @details     This API enables or disables the spacified interrupt. If an
 *              Interrupt is disabled then the interrupt handler will not be
 *              run even if the interrupt is raised
 *
 * @param [in]  iid                 Interrupt ID.
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable the given Interrupt.\n\n
 *                                  This argument should be set to:
 *                                  - 'true'     to enable the interrupt.
 *                                  - 'false'    to disable the interrupt.
 * @return
 *          - #ADI_INT_SUCCESS if successfully enabled or disabled the
 *                             interrupt.
 *          - #ADI_INT_FAILURE if the interrupt could not be enabled or disabled.
 *
 */

ADI_INT_STATUS adi_int_EnableInt(
                   uint32_t             iid,
                   bool_t               bEnable
               )
{

     adi_nvic_EnableInt(iid, bEnable);

	return ADI_INT_SUCCESS;
}
#endif

/*! @} */
