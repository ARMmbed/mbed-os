/*********************************************************************************

Copyright (c) 2011-2016 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/**
 *  @defgroup RTOS_INTERRUPTS RTOS-specific Interrupt APIs
 * This is the Analog Devices' RTOS interrupt inferface.  It is used to hook and unhook interrupts within an RTOS environment.
 * This interface is not used for non-threading/non-multitasking applications as the IVT is pre-configured as part of the startup code.
 */

/** @addtogroup RTOS_INTERRUPTS RTOS-specific Interrupt APIs
 *  @{
 */

/*!
* @file      adi_int.h
*
* @brief     RTOS-specific Interrupt Handler API header file
*
* @details
*            This is the primary header file for installing interrupt handlers in RTOS applications
*/

#ifndef ADI_INT_H_
#define ADI_INT_H_

#include <adi_global_config.h>
#include <adi_types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
#if defined (USER_SPECIFIED_RTOS)
#include <user_rtos_support.h>
#else
#include <adi_osal.h>

#define ADI_CRITICAL_REGION_VAR_DECL 

#define ADI_CRITICAL_REGION_VAR_DEFINE 

#define ADI_SET_CRITICAL_REGION_VAR 

#endif  
#else
#define ADI_CRITICAL_REGION_VAR_DECL \
extern volatile uint32_t gnCriticalRegionNestingCount;\
extern volatile uint32_t gnCriticalRegionIntStatus;

#define ADI_CRITICAL_REGION_VAR_DEFINE \
volatile uint32_t gnCriticalRegionNestingCount = 0u; \
volatile uint32_t gnCriticalRegionIntStatus = 0u;

#define ADI_SET_CRITICAL_REGION_VAR \
gnCriticalRegionNestingCount = 0u; \
gnCriticalRegionIntStatus = 0u;

#endif /* ADI_CFG_ENABLE_RTOS_SUPPORT */



#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)

#if !defined (USER_SPECIFIED_RTOS)  
/* Enter critical section */
#define ADI_ENTER_CRITICAL_REGION()  \
        adi_osal_EnterCriticalRegion()

/* Exit critical region */
#define ADI_EXIT_CRITICAL_REGION()  \
        adi_osal_ExitCriticalRegion()

/* Used to define the interrupt handler */
#define ADI_INT_HANDLER(Handler) \
        void (Handler)(uint32_t  __iid, void*  __handlerArg)

/* Macro to install the handler */
#define ADI_INSTALL_HANDLER(IRQn, Handler) \
        adi_int_InstallHandler((uint32_t)(IRQn), (Handler), NULL, false)

/* Macro to uninstall the handler */
#define ADI_UNINSTALL_HANDLER(IRQn) \
        adi_int_UninstallHandler((uint32_t)(IRQn))

/* Macro to enable the interrupt */
#define ADI_ENABLE_INT(IRQn) \
        adi_int_EnableInt((uint32_t)(IRQn), true)

/* Macro to disable the interrupt */
#define ADI_DISABLE_INT(IRQn) \
        adi_int_EnableInt((uint32_t)(IRQn), false)

typedef void (*ADI_INT_HANDLER_PTR)(
    uint32_t            iid,
    void*               handlerArg
);
#endif
#else /* No multithread support */

ADI_CRITICAL_REGION_VAR_DECL

#define ADI_ENTER_CRITICAL_REGION()     \
do {                                    \
  uint32_t state = __get_PRIMASK();     \
                                        \
  if(state == 0u)                       \
  {                                     \
    __disable_irq();                    \
  }                                     \
                                        \
  if(gnCriticalRegionNestingCount == 0u)\
  {                                     \
    gnCriticalRegionIntStatus = state;  \
  }                                     \
  gnCriticalRegionNestingCount++;       \
} while (0)



/* Exit critical region */
/*! Macro that enables interrupts. */
#define ADI_EXIT_CRITICAL_REGION()      \
do {                                    \
  gnCriticalRegionNestingCount--;       \
  if(gnCriticalRegionNestingCount <= 0u)\
  {                                     \
    gnCriticalRegionNestingCount = 0u;  \
                                        \
    if(gnCriticalRegionIntStatus == 0u) \
    {                                   \
      __enable_irq();                   \
    }                                   \
  }                                     \
} while (0)

/* Used to define the interrupt handler */
#define ADI_INT_HANDLER(Handler) \
        void (Handler)(void)

/* Macro to install the handler (defined to nothing as no installation required, the driver just override the weak interrupt handler symbol) */
#define ADI_INSTALL_HANDLER(IRQn, Handler)

/* Macro to uninstall the handler */
#define ADI_UNINSTALL_HANDLER(IRQn)

/* Macro to enable the interrupt */
#define ADI_ENABLE_INT(IRQn) \
        NVIC_EnableIRQ(IRQn)


/* Macro to disable the interrupt */
#define ADI_DISABLE_INT(IRQn) \
        NVIC_DisableIRQ(IRQn)

typedef void (*ADI_INT_HANDLER_PTR)(void);          

#endif  /* ADI_CFG_ENABLE_RTOS_SUPPORT */

/*! Enumeration of different error codes returned by adi_int* APIs. */
typedef enum
{
    ADI_INT_SUCCESS, /*!< The operation is completed successfully. */
    ADI_INT_FAILURE  /*!< The operation failed. */

} ADI_INT_STATUS;

/* Install the handler for a Cortex-M3 Interrupt. */
ADI_INT_STATUS  adi_int_InstallHandler (
    uint32_t            iid,
    ADI_INT_HANDLER_PTR pfHandler,
    void*               pCBParam,
    bool_t              bEnable
               );

/* Enable or disable the interrupt */
ADI_INT_STATUS adi_int_EnableInt(
    uint32_t            iid,
    bool_t              bEnable
               );

/* Uninstall the interrupt handler */
ADI_INT_STATUS  adi_int_UninstallHandler (uint32_t iid);


/**
 * @brief Device Drivers Callback function definition
 */
typedef void (* ADI_CALLBACK) (  /*!< Callback function pointer */
    void      *pCBParam,         /*!< Client supplied callback param */
    uint32_t   Event,            /*!< Event ID specific to the Driver/Service */
    void      *pArg);            /*!< Pointer to the event specific argument */

#ifdef __cplusplus
}
#endif

/*! @} */

#endif /* ADI_INT_H_ */
