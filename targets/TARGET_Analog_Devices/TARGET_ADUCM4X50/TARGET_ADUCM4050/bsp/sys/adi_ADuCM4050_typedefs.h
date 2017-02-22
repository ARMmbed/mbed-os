/* ================================================================================

     Project      :   ADuCM4050
     File         :   ADuCM4050_typedefs.h
     Description  :   C Register Structures

     Date         :   Sep 28, 2016

     Copyright (c) 2014-2016 Analog Devices, Inc.  All Rights Reserved.
     This software is proprietary and confidential to Analog Devices, Inc. and
     its licensors.

     This file was auto-generated. Do not make local changes to this file.

   ================================================================================ */

#ifndef _ADUCM4050_TYPEDEFS_H
#define _ADUCM4050_TYPEDEFS_H

/* pickup integer types */
#if defined(_LANGUAGE_C) || (defined(__GNUC__) && !defined(__ASSEMBLER__))
#include <stdint.h>
#endif /* _LANGUAGE_C */

#if defined ( __CC_ARM   )
#pragma push
#pragma anon_unions
#endif


#if defined (_MISRA_RULES)
/*
  anonymous unions violate ISO 9899:1990 and therefore MISRA Rule 1.1.
  Use of unions violates MISRA Rule 18.4.
  Anonymous unions are required for this implementation.
  Re-use of identifiers violates MISRA Rule 5.7.
  Field names are repeated for the ADuCM4050 register map.
*/
#pragma diag(push)
#pragma diag(suppress:misra_rule_1_1:"Allow anonymous unions")
#pragma diag(suppress:misra_rule_5_1:"Allow names over 32 character limit")
#pragma diag(suppress:misra_rule_5_3:"Header will re-use typedef identifiers")
#pragma diag(suppress:misra_rule_5_6:"Header will re-use identifiers in the same scope")
#pragma diag(suppress:misra_rule_5_7:"Header will re-use identifiers")
#pragma diag(suppress:misra_rule_18_4:"Allow the use of a union")
#endif /* _MISRA_RULES */

/** @defgroup LOAD 16-bit load value (LOAD) Register
 *  16-bit load value (LOAD) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_LOAD_Struct
 *! \brief  16-bit load value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_LOAD_t__
typedef struct _ADI_TMR_LOAD_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Load value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_LOAD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_LOAD_t__ */

/*@}*/

/** @defgroup CURCNT 16-bit timer value (CURCNT) Register
 *  16-bit timer value (CURCNT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_CURCNT_Struct
 *! \brief  16-bit timer value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_CURCNT_t__
typedef struct _ADI_TMR_CURCNT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Current count */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_CURCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_CURCNT_t__ */

/*@}*/

/** @defgroup CTL Control (CTL) Register
 *  Control (CTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_CTL_Struct
 *! \brief  Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_CTL_t__
typedef struct _ADI_TMR_CTL_t {
  union {
    struct {
      unsigned int PRE        :  2;  /**< Prescaler */
      unsigned int UP         :  1;  /**< Count up */
      unsigned int MODE       :  1;  /**< Timer mode */
      unsigned int EN         :  1;  /**< Timer enable */
      unsigned int CLK        :  2;  /**< Clock select */
      unsigned int RLD        :  1;  /**< Reload control */
      unsigned int reserved8  :  5;
      unsigned int EVTEN      :  1;  /**< Event select */
      unsigned int RSTEN      :  1;  /**< Counter and prescale reset enable */
      unsigned int SYNCBYP    :  1;  /**< Synchronization bypass */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_CTL_t__ */

/*@}*/

/** @defgroup CLRINT Clear Interrupt (CLRINT) Register
 *  Clear Interrupt (CLRINT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_CLRINT_Struct
 *! \brief  Clear Interrupt Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_CLRINT_t__
typedef struct _ADI_TMR_CLRINT_t {
  union {
    struct {
      unsigned int TIMEOUT    :  1;  /**< Clear timeout interrupt */
      unsigned int EVTCAPT    :  1;  /**< Clear captured event interrupt */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_CLRINT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_CLRINT_t__ */

/*@}*/

/** @defgroup CAPTURE Capture (CAPTURE) Register
 *  Capture (CAPTURE) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_CAPTURE_Struct
 *! \brief  Capture Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_CAPTURE_t__
typedef struct _ADI_TMR_CAPTURE_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< 16-bit captured value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_CAPTURE_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_CAPTURE_t__ */

/*@}*/

/** @defgroup ALOAD 16-bit load value, asynchronous (ALOAD) Register
 *  16-bit load value, asynchronous (ALOAD) Register.
 *
 *  Only use when a synchronous clock source is selected (CON:CLK=00)
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_ALOAD_Struct
 *! \brief  16-bit load value, asynchronous Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_ALOAD_t__
typedef struct _ADI_TMR_ALOAD_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Load value, asynchronous */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_ALOAD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_ALOAD_t__ */

/*@}*/

/** @defgroup ACURCNT 16-bit timer value, asynchronous (ACURCNT) Register
 *  16-bit timer value, asynchronous (ACURCNT) Register.
 *
 *  Only use when a synchronous clock source is selected (CON:CLK=00)
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_ACURCNT_Struct
 *! \brief  16-bit timer value, asynchronous Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_ACURCNT_t__
typedef struct _ADI_TMR_ACURCNT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Counter value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_ACURCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_ACURCNT_t__ */

/*@}*/

/** @defgroup STAT Status (STAT) Register
 *  Status (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_STAT_Struct
 *! \brief  Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_STAT_t__
typedef struct _ADI_TMR_STAT_t {
  union {
    struct {
      unsigned int TIMEOUT    :  1;  /**< Timeout event occurred */
      unsigned int CAPTURE    :  1;  /**< Capture event pending */
      unsigned int reserved2  :  4;
      unsigned int BUSY       :  1;  /**< Timer Busy */
      unsigned int PDOK       :  1;  /**< Clear Interrupt Register synchronization */
      unsigned int CNTRST     :  1;  /**< Counter reset occurring */
      unsigned int reserved9  :  7;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_STAT_t__ */

/*@}*/

/** @defgroup PWMCTL PWM Control Register (PWMCTL) Register
 *  PWM Control Register (PWMCTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_PWMCTL_Struct
 *! \brief  PWM Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_PWMCTL_t__
typedef struct _ADI_TMR_PWMCTL_t {
  union {
    struct {
      unsigned int MATCH      :  1;  /**< PWM Match enabled */
      unsigned int IDLESTATE  :  1;  /**< PWM Idle State */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_PWMCTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_PWMCTL_t__ */

/*@}*/

/** @defgroup PWMMATCH PWM Match Value (PWMMATCH) Register
 *  PWM Match Value (PWMMATCH) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_PWMMATCH_Struct
 *! \brief  PWM Match Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_PWMMATCH_t__
typedef struct _ADI_TMR_PWMMATCH_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< PWM Match Value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_PWMMATCH_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_PWMMATCH_t__ */

/*@}*/

/** @defgroup EVENTSELECT Timer Event selection Register. (EVENTSELECT) Register
 *  Timer Event selection Register. (EVENTSELECT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_EVENTSELECT_Struct
 *! \brief  Timer Event selection Register. Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_EVENTSELECT_t__
typedef struct _ADI_TMR_EVENTSELECT_t {
  union {
    struct {
      unsigned int EVTRANGE   :  6;  /**< Event select range */
      unsigned int reserved6  : 10;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_EVENTSELECT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_EVENTSELECT_t__ */

/*@}*/

/** @defgroup LOAD 16-bit load value (LOAD) Register
 *  16-bit load value (LOAD) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_LOAD_Struct
 *! \brief  16-bit load value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_LOAD_t__
typedef struct _ADI_TMR_RGB_LOAD_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Load value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_LOAD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_LOAD_t__ */

/*@}*/

/** @defgroup CURCNT 16-bit timer value (CURCNT) Register
 *  16-bit timer value (CURCNT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_CURCNT_Struct
 *! \brief  16-bit timer value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_CURCNT_t__
typedef struct _ADI_TMR_RGB_CURCNT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Current count */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_CURCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_CURCNT_t__ */

/*@}*/

/** @defgroup CTL Control (CTL) Register
 *  Control (CTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_CTL_Struct
 *! \brief  Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_CTL_t__
typedef struct _ADI_TMR_RGB_CTL_t {
  union {
    struct {
      unsigned int PRE        :  2;  /**< Prescaler */
      unsigned int UP         :  1;  /**< Count up */
      unsigned int MODE       :  1;  /**< Timer mode */
      unsigned int EN         :  1;  /**< Timer enable */
      unsigned int CLK        :  2;  /**< Clock select */
      unsigned int RLD        :  1;  /**< Reload control */
      unsigned int reserved8  :  5;
      unsigned int EVTEN      :  1;  /**< Event select */
      unsigned int RSTEN      :  1;  /**< Counter and prescale reset enable */
      unsigned int SYNCBYP    :  1;  /**< Synchronization bypass */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_CTL_t__ */

/*@}*/

/** @defgroup CLRINT Clear Interrupt (CLRINT) Register
 *  Clear Interrupt (CLRINT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_CLRINT_Struct
 *! \brief  Clear Interrupt Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_CLRINT_t__
typedef struct _ADI_TMR_RGB_CLRINT_t {
  union {
    struct {
      unsigned int TIMEOUT    :  1;  /**< Clear timeout interrupt */
      unsigned int EVTCAPT    :  1;  /**< Clear captured event interrupt */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_CLRINT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_CLRINT_t__ */

/*@}*/

/** @defgroup CAPTURE Capture (CAPTURE) Register
 *  Capture (CAPTURE) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_CAPTURE_Struct
 *! \brief  Capture Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_CAPTURE_t__
typedef struct _ADI_TMR_RGB_CAPTURE_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< 16-bit captured value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_CAPTURE_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_CAPTURE_t__ */

/*@}*/

/** @defgroup ALOAD 16-bit load value, asynchronous (ALOAD) Register
 *  16-bit load value, asynchronous (ALOAD) Register.
 *
 *  Only use when a synchronous clock source is selected (CON:CLK=00)
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_ALOAD_Struct
 *! \brief  16-bit load value, asynchronous Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_ALOAD_t__
typedef struct _ADI_TMR_RGB_ALOAD_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Load value, asynchronous */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_ALOAD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_ALOAD_t__ */

/*@}*/

/** @defgroup ACURCNT 16-bit timer value, asynchronous (ACURCNT) Register
 *  16-bit timer value, asynchronous (ACURCNT) Register.
 *
 *  Only use when a synchronous clock source is selected (CON:CLK=00)
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_ACURCNT_Struct
 *! \brief  16-bit timer value, asynchronous Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_ACURCNT_t__
typedef struct _ADI_TMR_RGB_ACURCNT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Counter value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_ACURCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_ACURCNT_t__ */

/*@}*/

/** @defgroup STAT Status (STAT) Register
 *  Status (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_STAT_Struct
 *! \brief  Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_STAT_t__
typedef struct _ADI_TMR_RGB_STAT_t {
  union {
    struct {
      unsigned int TIMEOUT    :  1;  /**< Timeout event occurred */
      unsigned int CAPTURE    :  1;  /**< Capture event pending */
      unsigned int reserved2  :  4;
      unsigned int BUSY       :  1;  /**< Timer Busy */
      unsigned int PDOK       :  1;  /**< Clear Interrupt Register synchronization */
      unsigned int CNTRST     :  1;  /**< Counter reset occurring */
      unsigned int reserved9  :  7;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_STAT_t__ */

/*@}*/

/** @defgroup PWM0CTL PWM0 Control Register (PWM0CTL) Register
 *  PWM0 Control Register (PWM0CTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_PWM0CTL_Struct
 *! \brief  PWM0 Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM0CTL_t__
typedef struct _ADI_TMR_RGB_PWM0CTL_t {
  union {
    struct {
      unsigned int MATCH      :  1;  /**< PWM Match enabled */
      unsigned int IDLESTATE  :  1;  /**< PWM Idle State */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_PWM0CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM0CTL_t__ */

/*@}*/

/** @defgroup PWM0MATCH PWM0 Match Value (PWM0MATCH) Register
 *  PWM0 Match Value (PWM0MATCH) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_PWM0MATCH_Struct
 *! \brief  PWM0 Match Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM0MATCH_t__
typedef struct _ADI_TMR_RGB_PWM0MATCH_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< PWM Match Value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_PWM0MATCH_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM0MATCH_t__ */

/*@}*/

/** @defgroup EVENTSELECT Timer Event selection Register (EVENTSELECT) Register
 *  Timer Event selection Register (EVENTSELECT) Register.
 *
 *  The register value selects one of the 40 event inputs connected to the module.The event select range is 0 to 39 only. If any value above 39 is written, the  register content will be cleared to zero.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_EVENTSELECT_Struct
 *! \brief  Timer Event selection Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_EVENTSELECT_t__
typedef struct _ADI_TMR_RGB_EVENTSELECT_t {
  union {
    struct {
      unsigned int EVTRANGE   :  6;  /**< Event select range */
      unsigned int reserved6  : 10;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_EVENTSELECT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_EVENTSELECT_t__ */

/*@}*/

/** @defgroup PWM1CTL PWM1 Control Register (PWM1CTL) Register
 *  PWM1 Control Register (PWM1CTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_PWM1CTL_Struct
 *! \brief  PWM1 Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM1CTL_t__
typedef struct _ADI_TMR_RGB_PWM1CTL_t {
  union {
    struct {
      unsigned int MATCH      :  1;  /**< PWM Match enabled */
      unsigned int IDLESTATE  :  1;  /**< PWM Idle State */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_PWM1CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM1CTL_t__ */

/*@}*/

/** @defgroup PWM1MATCH PWM1 Match Value (PWM1MATCH) Register
 *  PWM1 Match Value (PWM1MATCH) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_PWM1MATCH_Struct
 *! \brief  PWM1 Match Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM1MATCH_t__
typedef struct _ADI_TMR_RGB_PWM1MATCH_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< PWM Match Value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_PWM1MATCH_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM1MATCH_t__ */

/*@}*/

/** @defgroup PWM2CTL PWM2 Control Register (PWM2CTL) Register
 *  PWM2 Control Register (PWM2CTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_PWM2CTL_Struct
 *! \brief  PWM2 Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM2CTL_t__
typedef struct _ADI_TMR_RGB_PWM2CTL_t {
  union {
    struct {
      unsigned int MATCH      :  1;  /**< PWM Match enabled */
      unsigned int IDLESTATE  :  1;  /**< PWM Idle State */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_PWM2CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM2CTL_t__ */

/*@}*/

/** @defgroup PWM2MATCH PWM2 Match Value (PWM2MATCH) Register
 *  PWM2 Match Value (PWM2MATCH) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_PWM2MATCH_Struct
 *! \brief  PWM2 Match Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM2MATCH_t__
typedef struct _ADI_TMR_RGB_PWM2MATCH_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< PWM Match Value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_RGB_PWM2MATCH_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_PWM2MATCH_t__ */

/*@}*/

/** @defgroup CR0 RTC Control 0 (CR0) Register
 *  RTC Control 0 (CR0) Register.
 *
 *  CR0 is the primary of two control registers for the RTC, the other being CR1. All mainstream RTC operations are enabled and disabled by the CPU using CR0.
 *  
 *  
 *  The CR1 MMR is a secondary control register which expands the granularity of RTC control which is provided by CR0. 
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_RTC_CR0_MOD60ALM
 *! \brief  Periodic, modulo-60 alarm time in prescaled RTC time units beyond a modulo-60 boundary (MOD60ALM) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR0_EX_30_TIME_UNITS = 30,  /**< Example of setting a modulo-60 periodic interrupt from the RTC to be issued to the CPU at 30 time units past a modulo-60 boundary. */
  RTC_CR0_EX_55_TIME_UNITS = 55   /**< Example of setting a modulo-60 periodic interrupt from the RTC to be issued to the CPU at 55 time units past a modulo-60 boundary. */
} ADI_RTC_CR0_MOD60ALM;


/* ==========================================================================
 *! \struct ADI_RTC_CR0_Struct
 *! \brief  RTC Control 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR0_t__
typedef struct _ADI_RTC_CR0_t {
  union {
    struct {
      unsigned int CNTEN      :  1;  /**< Global enable for the RTC */
      unsigned int ALMEN      :  1;  /**< Enable the RTC alarm (absolute) operation */
      unsigned int ALMINTEN   :  1;  /**< Enable sourced alarm interrupts to the CPU */
      unsigned int TRMEN      :  1;  /**< Enable RTC digital trimming */
      unsigned int MOD60ALMEN :  1;  /**< Enable RTC modulo-60 counting of time past a modulo-60 boundary */
      unsigned int MOD60ALM   :  6;  /**< Periodic, modulo-60 alarm time in prescaled RTC time units beyond a modulo-60 boundary */
      unsigned int MOD60ALMINTEN :  1;  /**< Enable periodic Modulo-60 RTC alarm sourced interrupts to the CPU */
      unsigned int ISOINTEN      :  1;  /**< Enable RTC power-domain isolation sourced interrupts to the CPU when isolation of the RTC power domain is activated and subsequently de-activated */
      unsigned int WPNDERRINTEN  :  1;  /**< Enable Write pending error sourced interrupts to the CPU when an RTC register-write pending error occurs */
      unsigned int WSYNCINTEN    :  1;  /**< Enable Write synchronization sourced interrupts to the CPU */
      unsigned int WPNDINTEN     :  1;  /**< Enable Write Pending sourced interrupts to the CPU */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR0_t__ */

/*@}*/

/** @defgroup SR0 RTC Status 0 (SR0) Register
 *  RTC Status 0 (SR0) Register.
 *
 *  Information on RTC operation is made available to the CPU via three status registers SR0, SR1, and SR2. These registers include all flags related to CPU interrupt sources and error conditions within the RTC.
 *  
 *  
 *  Note that there is a one-for-one correspondence between bit positions in SR0 and SR1 in terms of their WSYNC<mmr> and WPEND<mmr> fields respectively. The WSYNC status for an MMR in SR0 is located at the same bit position as the WPEND status for the same MMR in SR1.
 *  
 *  
 *  The distinction between WSYNC and WPEND is as follows. If a posted write transaction to an MMR in the RTC has completed execution and the effects of the transaction are visible to the processor, the WSYNC status for the MMR in question is set to 1. Otherwise, if the effects of a posted transaction to an MMR are not yet visible, the WSYNC status for that MMR is 0.
 *  
 *  
 *  WPEND, on the other hand, indicates whether there is room in the RTC to accept a new posted write to a given MMR. If a previously posted write to an MMR is awaiting execution and is occupying the drop point for posted writes to that MMR, the WPEND status of the MMR concerned is 1. Otherwise, if the RTC has room to accept a new posted write for an individual MMR, the WPEND status for that MMR is 0.
 *  
 *  
 *  Posted writes take time to complete if they concern MMRs or MMR fields which are sourced in the 32 kHz clock domain of the RTC. These MMRs (all fields) are as follows : CR0, CNT0, CNT1, ALM0, ALM1, ALM2, TRM, CR1.
 *  
 *  
 *  The following sticky interrupt source fields of the SR0 MMR are also sourced in the 32 kHz domain:  ALMINT, MOD60ALMINT, ISOINT. Any write-one-to-clear clearances of these interrupt source fields accumulate (if still pending) into a common posted write transaction for the SR0 MMR. This determines the WPEND status of the SR0 MMR itself.
 *  
 *  
 *  Note that the results of posted clearances of interrupt source fields in SR0 are immediately visible by the processor. For this reason, the WSYNC status of SR0 is always 1, to confirm that results of writes (clearances) are immediately available to the CPU.
 *  
 *  
 *  Note that the WPEND status and WSYNC status for the SR0 are encompassing values for the MMR as a whole. There is no distinction made in these status values as to how many interrupt source fields are being cleared at the same time by a single posted write to SR0.
 *  
 *  
 *  The SR1 status register, in comparison, is a read-only status register. SR1 has no WPEND or WSYNC status values, as there are never any posted writes to the MMR. SR2, on the other hand, is a read-write status register, containing RW1C interrupt sources but all of these reside wholly in the processor's clock domain and can be cleared instantly. Hence SR2 has no WPEND or WSYNC values associated with it.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR0_Struct
 *! \brief  RTC Status 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR0_t__
typedef struct _ADI_RTC_SR0_t {
  union {
    struct {
      unsigned int reserved0  :  1;
      unsigned int ALMINT     :  1;  /**< Alarm interrupt source */
      unsigned int MOD60ALMINT :  1;  /**< Modulo-60 RTC alarm interrupt source */
      unsigned int ISOINT      :  1;  /**< RTC power-domain isolation interrupt source */
      unsigned int WPNDERRINT  :  1;  /**< Write pending error interrupt source */
      unsigned int WSYNCINT    :  1;  /**< Write synchronization interrupt */
      unsigned int WPNDINT     :  1;  /**< Write pending interrupt */
      unsigned int WSYNCCR0    :  1;  /**< Synchronization status of posted writes to RTC Control 0 Register */
      unsigned int WSYNCSR0    :  1;  /**< Synchronization status of posted clearances to interrupt sources in RTC Status 0 Register */
      unsigned int WSYNCCNT0   :  1;  /**< Synchronization status of posted writes to RTC Count 0 Register */
      unsigned int WSYNCCNT1   :  1;  /**< Synchronization status of posted writes to RTC Count 1 Register */
      unsigned int WSYNCALM0   :  1;  /**< Synchronization status of posted writes to RTC Alarm 0 Register */
      unsigned int WSYNCALM1   :  1;  /**< Synchronization status of posted writes to RTC Alarm 1 Register */
      unsigned int WSYNCTRM    :  1;  /**< Synchronization status of posted writes to RTC Trim Register */
      unsigned int ISOENB      :  1;  /**< Visibility status of 32 kHz sourced registers, taking account of power-domain isolation */
      unsigned int reserved15  :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR0_t__ */

/*@}*/

/** @defgroup SR1 RTC Status 1 (SR1) Register
 *  RTC Status 1 (SR1) Register.
 *
 *  Information on RTC operation is made available to the CPU via three status registers SR0, SR1 and SR2. These registers include all flags related to CPU interrupt sources and error conditions within the RTC.
 *  
 *  
 *  Note that there is a one-for-one correspondence between bit positions in SR0 and SR1 in terms of  their WSYNC<mmr> and WPEND<mmr> fields respectively. The WSYNC status for an MMR in SR0 is located at the same bit position as the WPEND status for the same MMR in SR1.
 *  
 *  
 *  The distinction between WSYNC and WPEND is as follows. If a posted write transaction to an MMR in the RTC has completed execution and the effects of the transaction are visible to the processor, the WSYNC status for the MMR in question is set to 1. Otherwise, if the effects of a posted transaction to an MMR are not yet visible, the WSYNC status for that MMR is 0.
 *  
 *  
 *  WPEND, on the other hand, indicates whether there is room in the RTC to accept a new posted write to a given MMR. If a previously posted write to an MMR is awaiting execution and is occupying the drop point for posted writes to that MMR, the WPEND status of the MMR concerned is 1. Otherwise, if the RTC has room to accept a new posted write for an individual MMR, the WPEND status for that MMR is 0.
 *  
 *  
 *  Posted writes take time to complete if they concern MMRs or MMR fields which are sourced in the 32 kHz clock domain of the RTC. These MMRs (all fields) are as follows: CR0, CNT0, CNT1, ALM0, ALM1, ALM2, TRM, CR1
 *  
 *  
 *  The following sticky interrupt source fields of the SR0 MMR are also sourced in the 32 kHz domain : SR0:ALMINT, SR0:MOD60ALMINT, SR0:ISOINT. Any write-one-to-clear clearances of these interrupt source fields accumulate (if still pending) into a common posted write transaction for the SR0 MMR. This determines the WPEND status of the  SR0 MMR itself.
 *  
 *  
 *  Note that the results of posted clearances of interrupt source fields in SR0 are immediately visible by the processor. For this reason, the WSYNC status of SR0 is always 1, to confirm that results of writes (clearances) are immediately available to the CPU.
 *  
 *  
 *  Note that the WPEND status and WSYNC status for the SR0 are encompassing values for the MMR as a whole. There is no distinction made in these status values as to how many interrupt source fields are being cleared at the same time by a single posted write to  SR0.
 *  
 *  
 *  The SR1 status register, in comparison, is a read-only status register. SR1 has no WPEND or WSYNC status values, as there are never any posted writes to the MMR. SR2, on the other hand, is a read-write status register, containing RW1C interrupt sources but all of these reside wholly in the processor's clock domain and can be cleared instantly. Hence, SR2 has no WPEND or WSYNC values associated with it.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR1_Struct
 *! \brief  RTC Status 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR1_t__
typedef struct _ADI_RTC_SR1_t {
  union {
    struct {
      unsigned int reserved0  :  7;
      unsigned int WPNDCR0    :  1;  /**< Pending status of posted writes to RTC Control 0 Register */
      unsigned int WPNDSR0    :  1;  /**< Pending status of posted clearances of interrupt sources in RTC Status 0 Register */
      unsigned int WPNDCNT0   :  1;  /**< Pending status of posted writes to RTC Count 0 Register */
      unsigned int WPNDCNT1   :  1;  /**< Pending status of posted writes to RTC Count 1 Register */
      unsigned int WPNDALM0   :  1;  /**< Pending status of posted writes to RTC ALARM 0 Register */
      unsigned int WPNDALM1   :  1;  /**< Pending status of posted writes to RTC ALARM 1 Register */
      unsigned int WPNDTRM    :  1;  /**< Pending status of posted writes to RTC Trim Register */
      unsigned int reserved14 :  2;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR1_t__ */

/*@}*/

/** @defgroup CNT0 RTC Count 0 (CNT0) Register
 *  RTC Count 0 (CNT0) Register.
 *
 *  CNT0 contains the lower 16 bits of the RTC counter which maintains a real-time count in elapsed prescaled RTC time units.
 *  
 *  
 *  The instantaneous value of CNT0 can be read back by the CPU. The CPU can also redefine the value in this register. In this case, the RTC will continue its real-time count from the redefined value.
 *  
 *  
 *  Any write to CNT0 will pend until a corresponding write to CNT1 is carried out by the CPU, so that the combined 32-bit count redefinition can be executed as a single transaction.
 *  
 *  
 *  CNT0 and CNT1 can be written in either order, but paired, twin writes must be carried out by the CPU to have any effect on the RTC count. A paired write to CNT0 and CNT1 (in whichever order) zeroes the prescaler in the RTC and thus causes a redefinition of elapsed time by the CPU to align exactly with newly-created modulo-1 and modulo-60 boundaries.
 *  
 *  
 *  Such a redefinition also causes the RTC to create a trim boundary and initiate a new trim interval. When the RTC count is redefined by the CPU, no coincident trim adjustment of the count is carried out.
 *  
 *  
 *  The RTC supports on-the-fly redefinition of CNT0 and CNT1 while CR0:CNTEN is active. Alternatively, the CPU can disable the RTC (by first taking CR0:CNTEN inactive) while redefining these registers.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CNT0_Struct
 *! \brief  RTC Count 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CNT0_t__
typedef struct _ADI_RTC_CNT0_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Lower 16 prescaled (non-fractional) bits of the RTC real-time count */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CNT0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CNT0_t__ */

/*@}*/

/** @defgroup CNT1 RTC Count 1 (CNT1) Register
 *  RTC Count 1 (CNT1) Register.
 *
 *  CNT1 contains the upper 16 bits of the RTC counter which maintains a real-time count in elapsed prescaled RTC time units.
 *  
 *  
 *  The instantaneous value of CNT1 can be read back by the CPU. The CPU can also redefine the value in this register. In this case, the RTC will continue its real-time count from the redefined value.
 *  
 *  
 *  Any write to CNT1 will pend until a corresponding write to CNT0 is carried out by the CPU, so that the combined 32-bit count redefinition can be executed as a single transaction.
 *  
 *  
 *  CNT1 and CNT0 can be written in either order, but paired, twin writes must be carried out by the CPU to have any effect on the RTC count. A paired write to CNT1 and CNT0 (in whichever order) zeroes the prescaler in the RTC and thus causes a redefinition of elapsed time by the CPU to align exactly with newly-created modulo-1 and modulo-60 boundaries. 
 *  
 *  
 *  Such a redefinition also causes the RTC to create a trim boundary and initiate a new trim interval. When the RTC count is redefined by the CPU, no coincident trim adjustment of the count is carried out.
 *  
 *  
 *  The RTC supports on-the-fly redefinition of CNT1 and CNT0 while CR0:CNTEN is active. Alternatively, the CPU can disable the RTC (by first taking CR0:CNTEN inactive) while redefining these registers.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CNT1_Struct
 *! \brief  RTC Count 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CNT1_t__
typedef struct _ADI_RTC_CNT1_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Upper 16 prescaled (non-fractional) bits of the RTC real-time count */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CNT1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CNT1_t__ */

/*@}*/

/** @defgroup ALM0 RTC Alarm 0 (ALM0) Register
 *  RTC Alarm 0 (ALM0) Register.
 *
 *  ALM0 contains the lower 16 bits of the non-fractional (that is, prescaled) RTC alarm target time value, where the overall alarm is defined as {ALM1, ALM0} . {ALM2}
 *  
 *  
 *  Any write to ALM0 will pend (that is, stayed execution, with write data held in a staging-post buffer) until corresponding writes to ALM1 and ALM2 are carried out by the CPU, so that the combined 47-bit alarm redefinition can be executed as a single transaction. ALM0, ALM1, and ALM2 can be written in any order, but co-ordinated, triple writes (one write to each of the three MMRs) must be carried out by the CPU to have any effect on the RTC alarm.
 *  
 *  
 *  Note that ALM0 can be written to regardless of whether CR0:ALMEN or CR0:CNTEN is active.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_ALM0_Struct
 *! \brief  RTC Alarm 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_ALM0_t__
typedef struct _ADI_RTC_ALM0_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Lower 16 prescaled (that is, non-fractional) bits of the RTC alarm target time */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_ALM0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_ALM0_t__ */

/*@}*/

/** @defgroup ALM1 RTC Alarm 1 (ALM1) Register
 *  RTC Alarm 1 (ALM1) Register.
 *
 *  ALM1 contains the upper 16 bits of the non-fractional (prescaled) RTC alarm target time value, where the overall alarm is defined as {ALM1, ALM0} . {ALM2}
 *  
 *  
 *  Any write to ALM1 will pend (that is, stayed execution, with write data held in a staging-post buffer) until corresponding writes to ALM0 and ALM2 are carried out by the CPU, so that the combined 47-bit alarm redefinition can be executed as a single transaction. ALM0, ALM1, and ALM2 can be written in any order, but co-ordinated, triple writes (one write to each of the three MMRs) must be carried out by the CPU to have any effect on the RTC alarm.
 *  
 *  
 *  Note that ALM1 can be written to regardless of whether CR0:ALMEN or CR0:CNTEN is active.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_ALM1_Struct
 *! \brief  RTC Alarm 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_ALM1_t__
typedef struct _ADI_RTC_ALM1_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Upper 16 prescaled (non-fractional) bits of the RTC alarm target time */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_ALM1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_ALM1_t__ */

/*@}*/

/** @defgroup TRM RTC Trim (TRM) Register
 *  RTC Trim (TRM) Register.
 *
 *  TRM contains the trim value and interval for a periodic adjustment of the RTC count value to track time with the required accuracy. Trimming is enabled and disabled via the CR0:TRMEN bit. For trimming to occur, the global enable for the RTC, CR0:CNTEN, must also be active.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_TRM_Struct
 *! \brief  RTC Trim Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_TRM_t__
typedef struct _ADI_RTC_TRM_t {
  union {
    struct {
      unsigned int VALUE      :  3;  /**< Trim value in prescaled RTC time units to be added or subtracted from the RTC count at the end of a periodic interval selected by RTC Trim Register */
      unsigned int ADD        :  1;  /**< Trim Polarity */
      unsigned int IVL        :  2;  /**< Trim interval in prescaled RTC time units */
      unsigned int IVL2EXPMIN :  4;  /**< Minimum power-of-two interval of prescaled RTC time units, which RTC Trim Register can select */
      unsigned int reserved10 :  6;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_TRM_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_TRM_t__ */

/*@}*/

/** @defgroup GWY RTC Gateway (GWY) Register
 *  RTC Gateway (GWY) Register.
 *
 *  GWY is a gateway MMR address through which the CPU can order actions to be taken within the RTC. The CPU does this by writing specific keys to GWY. Note that GWY reads back as all zeros.
 *  
 *  The RTC supports the following independent, software-keyed commands:
 *  
 *  (i) Cancel all posted and executing write transactions in the RTC with immediate effect.
 *  
 *  (ii) Capture a sticky snapshot of the {CNT1, CNT0, CNT2} MMRs into {SNAP1, SNAP0, SNAP2}.
 *  
 *  (iii) Zero the SR6:RTCFRZCNTPTR  pointer and re-initialize a 0-1-2 sequence for reads of FRZCNT:RTCFRZCNT.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_GWY_Struct
 *! \brief  RTC Gateway Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_GWY_t__
typedef struct _ADI_RTC_GWY_t {
  union {
    struct {
      unsigned int SWKEY      : 16;  /**< Software-keyed command issued by the CPU */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_GWY_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_GWY_t__ */

/*@}*/

/** @defgroup CR1 RTC Control 1 (CR1) Register
 *  RTC Control 1 (CR1) Register.
 *
 *  CR1 register expands the granularity of RTC control which is already available via CR0. 
 *  
 *  
 *  CR1 allows additional sticky interrupt sources in SR2 to be enabled. These sources fan into the RTC peripheral interrupt to optionally tell the CPU when: 
 *  (i) The RTC count has changed
 *   (ii) A prescaled (modulo-1)  gated-clock event has occurred to advance the RTC count
 *  (iii) A trim boundary (trimming) has occurred
 *  (iv) The 32-bit RTC count in {CNT1, CNT0} has rolled over 
 *  (v) The modulo-60 version of the RTC count has rolled over
 *  
 *  
 *  
 *  
 *  Note that CR1 is only configurable in RTC1, whereas in RTC0 it is a read-only register with fixed (reset) settings.
 *  
 *  
 *  Nonetheless, if the CPU attempts to write to CR1 on RTC0, the write is accepted and enqueued for execution with normal SR2:WPENDCR1_MIRROR and SR2:WSYNCCR1_MIRROR indications, as on RTC1, in terms of the progress of the posted write. However, because the bit fields in CR1 on RTC0 are read-only, such a write, when executed, has no effect.
 *  
 *  
 *  In contrast, full write configurability of CR1 is supported on RTC1.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CR1_Struct
 *! \brief  RTC Control 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR1_t__
typedef struct _ADI_RTC_CR1_t {
  union {
    struct {
      unsigned int CNTINTEN   :  1;  /**< Enable for the RTC count interrupt source */
      unsigned int PSINTEN    :  1;  /**< Enable for the prescaled, modulo-1 interrupt source */
      unsigned int RTCTRMINTEN :  1;  /**< Enable for the RTC Trim interrupt source */
      unsigned int CNTROLLINTEN :  1;  /**< Enable for the RTC count roll-over interrupt source in RTC Status 2 Register */
      unsigned int CNTMOD60ROLLINTEN :  1;  /**< Enable for the RTC modulo-60 count roll-over interrupt source in RTC Status 2 Register */
      unsigned int PRESCALE2EXP      :  4;  /**< Prescale power of 2 division factor for the RTC base clock */
      unsigned int reserved9         :  7;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR1_t__ */

/*@}*/

/** @defgroup SR2 RTC Status 2 (SR2) Register
 *  RTC Status 2 (SR2) Register.
 *
 *  It is a status register which further complements the status information provided by SR0 and SR1. 
 *  Note that RTC1 has full SR2 functionality, RTC0 has reduced features. Details of RTC0 differences are given in individual bit fields of SR2.
 *  
 *  
 *  SR2 contains optionally-enabled (via CR1) sticky interrupt sources which are fan-in terms of the RTC peripheral interrupt. These tell the CPU when 
 *  
 *  (i) the RTC count has changed
 *  
 *  (ii) the prescaled gated clock which controls the advancement of  the RTC count has activated
 *   
 *  (iii) a trim boundary (and thus trimming) has occurred
 *  
 *  (iv) the 32-bit RTC count has rolled over
 *  
 *  (v) the modulo-60 version of the RTC count has rolled over.
 *  
 *  
 *  All interrupt sources in SR2 are sticky, active high, level signals. Each one can individually be cleared by writing a value of 1'b1 to its bit position in SR2.
 *  
 *  
 *  SR2 also contains mirrors of WSYNC and WPEND flags from SR1 and SR0.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR2_Struct
 *! \brief  RTC Status 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR2_t__
typedef struct _ADI_RTC_SR2_t {
  union {
    struct {
      unsigned int CNTINT     :  1;  /**< RTC count interrupt source */
      unsigned int PSINT      :  1;  /**< RTC prescaled, modulo-1 boundary interrupt source */
      unsigned int TRMINT     :  1;  /**< RTC Trim interrupt source */
      unsigned int CNTROLLINT :  1;  /**< RTC count roll-over interrupt source */
      unsigned int CNTMOD60ROLLINT :  1;  /**< RTC modulo-60 count roll-over interrupt source */
      unsigned int CNTROLL         :  1;  /**< RTC count roll-over */
      unsigned int CNTMOD60ROLL    :  1;  /**< RTC count modulo-60 roll-over */
      unsigned int TRMBDYMIR       :  1;  /**< Mirror of the RTCTRMBDY field of RTC Modulo Register */
      unsigned int reserved8       :  4;
      unsigned int WPNDCR1MIR      :  1;  /**< Pending status of posted writes to RTC Control 1 Register */
      unsigned int WPNDALM2MIR     :  1;  /**< Pending status of posted writes to RTC Alarm 2 Register */
      unsigned int WSYNCCR1MIR     :  1;  /**< Synchronization status of posted writes to RTC Control 1 Register */
      unsigned int WSYNCALM2MIR    :  1;  /**< Synchronization status of posted writes to RTC Alarm 2 Register */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR2_t__ */

/*@}*/

/** @defgroup SNAP0 RTC Snapshot 0 (SNAP0) Register
 *  RTC Snapshot 0 (SNAP0) Register.
 *
 *  It is a sticky snapshot of the value of CNT0. It is updated (along with its counterparts SNAP1 and SNAP2), thereby overwriting any previous value of {SNAP1, SNAP0}. {SNAP2}, whenever either of the following two events occurs:
 *  
 *  	(i) the CPU writes a snapshot request key of 16'h7627 to the GWY MMR.
 *  
 *  	(ii) an input-capture event occurs on the RTC_IC0 channel when enabled, provided the setting CR2IC:RTCICOWUSEN allows such overwriting.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SNAP0_Struct
 *! \brief  RTC Snapshot 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SNAP0_t__
typedef struct _ADI_RTC_SNAP0_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Constituent part of the 47-bit RTC Input Capture Channel 0, containing a sticky snapshot of  RTC Count 0 Register */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SNAP0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SNAP0_t__ */

/*@}*/

/** @defgroup SNAP1 RTC Snapshot 1 (SNAP1) Register
 *  RTC Snapshot 1 (SNAP1) Register.
 *
 *  SNAP1 is a sticky snapshot of the value of CNT1. It is updated (along with its counterparts SNAP0 and SNAP2, thereby overwriting any previous value of {SNAP1, SNAP0}. {SNAP2}, whenever either of the following two events occurs: 
 *  
 *  	(i) the CPU writes a snapshot request key of 16'h7627 to the GWY MMR.
 *  
 *  	(ii) an input-capture event occurs on the RTCIC0 channel when enabled, provided the setting of CR2IC:RTCICOWUSEN allows such overwriting.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SNAP1_Struct
 *! \brief  RTC Snapshot 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SNAP1_t__
typedef struct _ADI_RTC_SNAP1_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Constituent part of the 47-bit RTC Input Capture Channel 0, containing a sticky snapshot of  RTC Count 1 Register */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SNAP1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SNAP1_t__ */

/*@}*/

/** @defgroup SNAP2 RTC Snapshot 2 (SNAP2) Register
 *  RTC Snapshot 2 (SNAP2) Register.
 *
 *  Sticky snapshot of the value of CNT2. It is updated (along with its counterparts SNAP0 and SNAP1), thereby overwriting any previous value of {SNAP1, SNAP0}. {SNAP2}, whenever either of the following two events occurs:
 *  
 *  	(i) the CPU writes a snapshot request key of 16'h7627 to the GWY MMR.
 *  
 *  	(ii) an input-capture event occurs on the RTCIC0 channel when enabled, provided the setting of CR2IC:RTCICOWUSEN allows such overwriting.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SNAP2_Struct
 *! \brief  RTC Snapshot 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SNAP2_t__
typedef struct _ADI_RTC_SNAP2_t {
  union {
    struct {
      unsigned int VALUE      : 15;  /**< Constituent part of the 47-bit RTC Input Capture Channel 0, containing a sticky snapshot of RTC Count 2 Register */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SNAP2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SNAP2_t__ */

/*@}*/

/** @defgroup MOD RTC Modulo (MOD) Register
 *  RTC Modulo (MOD) Register.
 *
 *  MOD is a read-only register which makes available RTCCNTMOD60, which is the modulo-60 equivalent of the {CNT1, CNT0} count value. This modulo-60 value is equal to the displacement in prescaled RTC time units past the most recent modulo-60 roll-over event. A roll-over is a synonym for a modulo-60 boundary.
 *  
 *  
 *  Boundaries are defined in the following way. The RTC realigns itself to create coincident modulo-60 and modulo-1 boundaries whenever either of the following events occurs :
 *  
 *  (i) the CPU writes a new pair of values to the CNT1 and CNT0 registers to redefine the elapsed time units count while the RTC is enabled and this posted twin write is subsequently executed or 
 *  
 *  (ii) the CPU enables the RTC from a disabled state using the CR0:CNTEN field.
 *  
 *  
 *  Other read-only fields accessible via the MOD MMR are the magnitude of the most recent increment to the RTC count, RTCINCR and confirmation as to whether this increment coincided with a trim boundary, RTCTRMBDY. Note that the same increment is applied by the RTC to both its absolute (32-bit) count and the modulo-60 equivalent.
 *  
 *  
 *  To facilitate debug and to clarify the relationship between RTCCNTMOD60 and {CNT1, CNT0}, the upper bits of RTCMOD are padded out with the LSBs of {CNT1, CNT0}. This allows RTCCNTMOD60 and part of {CNT1, CNT0} to be read out at the same time and their alignment with each other to be understood.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_MOD_Struct
 *! \brief  RTC Modulo Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_MOD_t__
typedef struct _ADI_RTC_MOD_t {
  union {
    struct {
      unsigned int CNTMOD60   :  6;  /**< Modulo-60 value of prescaled  RTC Count 1 and RTC Count 0 Registers */
      unsigned int INCR       :  4;  /**< Most recent increment value added to the RTC Count in RTC Count 1 and RTC Count 0 Registers */
      unsigned int TRMBDY     :  1;  /**< Trim boundary indicator that the most recent RTC count increment has coincided with trimming of the count value */
      unsigned int CNT0_4TOZERO :  5;  /**< Mirror of RTC Count 0 Register */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_MOD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_MOD_t__ */

/*@}*/

/** @defgroup CNT2 RTC Count 2 (CNT2) Register
 *  RTC Count 2 (CNT2) Register.
 *
 *  CNT2 contains the fractional part of the RTC count, where the count is denominated in prescaled time units and is given by: {CNT1, CNT0} . {CNT2}. The overall resolution of the real-time count, including the fractional bits in CNT2, is therefore one 32 kHz clock period.
 *  
 *  
 *  CNT2 makes available to the CPU a read-only view of the internal sequence count in the RTC prescaler as it steps its way (in units of one 32 kHz period) from all zeros to all ones across the number of prescale sequence bits given by the CR1:PRESCALE2EXP field of the CR1 MMR. By completely traversing such a sequence, the prescaler effectively advances the main (non-fractional) part of the RTC count in {CNT1, CNT0} by one prescaled time unit. The prescale sequence count thus equates to the fractional part of the main RTC count.
 *  
 *  
 *  Note that the CNT2 register only exists in RTC1. In RTC0, the fractional part of the RTC count cannot be read by the CPU.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CNT2_Struct
 *! \brief  RTC Count 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CNT2_t__
typedef struct _ADI_RTC_CNT2_t {
  union {
    struct {
      unsigned int VALUE      : 15;  /**< Fractional bits of the RTC real-time count */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CNT2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CNT2_t__ */

/*@}*/

/** @defgroup ALM2 RTC Alarm 2 (ALM2) Register
 *  RTC Alarm 2 (ALM2) Register.
 *
 *  ALM2 specifies the fractional (non-prescaled) bits of the RTC alarm target time value, down to an individual 32 kHz clock cycle, where the overall alarm is defined as {ALM1, ALM0}. {ALM2}.
 *  
 *  
 *  The number of valid bits (which can legitimately have values of one optionally written to them) of ALM2 equals the number of prescale bits specified by the CR1:PRESCALE2EXP field of the CR1 MMR. If a bit position in ALM2 is set to one, such that this is incompatible with the CR1:PRESCALE2EXP field, the whole value of ALM2 is treated as zero by the RTC.
 *  
 *  
 *  Any write to ALM2 will pend (stayed execution, with write data held in a staging-post buffer) until corresponding writes to ALM0 and ALM1 are carried out by the CPU, so that the combined 47-bit alarm redefinition can be executed as a single transaction.  ALM0,  ALM1, and  ALM2 can be written in any order, but co-ordinated, triple writes (one write to each of the three MMRs) must be carried out by the CPU to have any effect on the RTC alarm.
 *  
 *  
 *  ALM2 can be written to regardless of whether  CR0:ALMEN or CR0:CNTEN is active.
 *  
 *  
 *  In RTC0, a value (typically zero) must still be written to ALM2 to invoke an alarm time definition via a triple write to {ALM1, ALM0, ALM2}. This applies even though RTC0 always treats the fractional part of an alarm time as zero.
 *  
 *  
 *  In RTC1, full support for fractional alarm time is present, such that an alarm can be specified down to an individual 32 kHz clock cycle using ALM2.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_ALM2_Struct
 *! \brief  RTC Alarm 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_ALM2_t__
typedef struct _ADI_RTC_ALM2_t {
  union {
    struct {
      unsigned int VALUE      : 15;  /**< Fractional (non-prescaled) bits of the RTC alarm target time */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_ALM2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_ALM2_t__ */

/*@}*/

/** @defgroup SR3 RTC Status 3 (SR3) Register
 *  RTC Status 3 (SR3) Register.
 *
 *  It is a status register containing write-one-to-clear, interrupt sources which stick active high whenever events occur for enabled input capture or output compare channels.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR3_Struct
 *! \brief  RTC Status 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR3_t__
typedef struct _ADI_RTC_SR3_t {
  union {
    struct {
      unsigned int RTCIC0IRQ  :  1;  /**< Sticky Interrupt Source for the RTC Input Capture Channel 0 */
      unsigned int reserved1  :  1;
      unsigned int RTCIC2IRQ  :  1;  /**< Sticky Interrupt Source for the RTC Input Capture Channel 2 */
      unsigned int RTCIC3IRQ  :  1;  /**< Sticky Interrupt Source for the RTC Input Capture Channel 3 */
      unsigned int RTCIC4IRQ  :  1;  /**< Sticky Interrupt Source for the RTC Input Capture Channel 4 */
      unsigned int RTCOC1FEIRQ :  1;  /**< Sticky Interrupt Source for Output Compare Channel 1 */
      unsigned int RTCOC2FEIRQ :  1;  /**< Sticky Interrupt Source for the RTCOC2 Output Compare channel falling edge */
      unsigned int RTCOC3FEIRQ :  1;  /**< Sticky Interrupt Source for the RTCOC3 Output Compare channel falling edge */
      unsigned int RTCOC4FEIRQ :  1;  /**< Sticky Interrupt Source for the RTCOC4 Output Compare channel falling edge */
      unsigned int RTCOC1IRQ   :  1;  /**< Sticky Interrupt Source for Output Compare Channel 1 */
      unsigned int RTCOC2IRQ   :  1;  /**< Sticky Interrupt Source for the RTCOC2 Output Compare channel */
      unsigned int RTCOC3IRQ   :  1;  /**< Sticky Interrupt Source for the RTCOC3 Output Compare channel */
      unsigned int RTCOC4IRQ   :  1;  /**< Sticky Interrupt Source for the RTCOC4 Output Compare channel */
      unsigned int ALMINTMIR   :  1;  /**< Read-only mirror of the ALMINT interrupt source in RTC Status 0 Register, acting as RTCOC0IRQ */
      unsigned int reserved14  :  2;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR3_t__ */

/*@}*/

/** @defgroup CR2IC RTC Control 2 for Configuring Input Capture Channels (CR2IC) Register
 *  RTC Control 2 for Configuring Input Capture Channels (CR2IC) Register.
 *
 *  CR2IC is a control register for configuring enables related to input-capture channels. CR2IC  contains enables for both the input-capture function itself for each channel, as well as enables for whether an event on a channel should contribute to the interrupt lines from the RTC to both the CPU and the wake-up controller.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CR2IC_Struct
 *! \brief  RTC Control 2 for Configuring Input Capture Channels Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR2IC_t__
typedef struct _ADI_RTC_CR2IC_t {
  union {
    struct {
      unsigned int RTCIC0EN   :  1;  /**< Enable for the RTC Input Capture Channel 0 */
      unsigned int reserved1  :  1;
      unsigned int RTCIC2EN   :  1;  /**< Enable for the RTC Input Capture Channel 2 */
      unsigned int RTCIC3EN   :  1;  /**< Enable for the RTC Input Capture Channel 3 */
      unsigned int RTCIC4EN   :  1;  /**< Enable for the RTC Input Capture Channel 4 */
      unsigned int RTCIC0LH   :  1;  /**< Polarity of the active-going capture edge for the RTC Input Capture Channel 0 */
      unsigned int reserved6  :  1;
      unsigned int RTCIC2LH   :  1;  /**< Polarity of the active-going capture edge for the RTC Input Capture Channel 2 */
      unsigned int RTCIC3LH   :  1;  /**< Polarity of the active-going capture edge for the RTC Input Capture Channel 3 */
      unsigned int RTCIC4LH   :  1;  /**< Polarity of the active-going capture edge for the RTC Input Capture Channel 4 */
      unsigned int RTCIC0IRQEN :  1;  /**< Interrupt Enable for the RTC Input Capture Channel 0 */
      unsigned int reserved11  :  1;
      unsigned int RTCIC2IRQEN :  1;  /**< Interrupt Enable for the RTC Input Capture Channel 2 */
      unsigned int RTCIC3IRQEN :  1;  /**< Interrupt Enable for the RTC Input Capture Channel 3 */
      unsigned int RTCIC4IRQEN :  1;  /**< Interrupt Enable for the RTC Input Capture Channel 4 */
      unsigned int RTCICOWUSEN :  1;  /**< Enable Overwrite of Unread Snapshots for all RTC Input Capture Channels */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR2IC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR2IC_t__ */

/*@}*/

/** @defgroup CR3OC RTC Control 3 for Configuring Output Compare Channel (CR3OC) Register
 *  RTC Control 3 for Configuring Output Compare Channel (CR3OC) Register.
 *
 *  CR3OC is a control register for configuring enables related to 16-bit output-compare channel. CR3OC contains enable for the output-compare function, as well as enable for whether an event on the channel should contribute to the interrupt lines from the RTC to both the CPU and the wake-up controller.
 *  The 47-bit RTCOC0 channel is not controlled by CR3OC. This is because RTCOC0 is a synonym for the main 47-bit RTC alarm (whose interrupt source is ALMINT), which is controlled by the CR0:ALMEN and CR0:ALMINTEN fields
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CR3OC_Struct
 *! \brief  RTC Control 3 for Configuring Output Compare Channel Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR3OC_t__
typedef struct _ADI_RTC_CR3OC_t {
  union {
    struct {
      unsigned int reserved0  :  1;
      unsigned int RTCOC1EN   :  1;  /**< Enable for Output Compare Channel 1 */
      unsigned int RTCOC2EN   :  1;  /**< Enable for the RTCOC2 Output Compare channel */
      unsigned int RTCOC3EN   :  1;  /**< Enable for the RTCOC3 Output Compare channel */
      unsigned int RTCOC4EN   :  1;  /**< Enable for the RTCOC4 Output Compare channel */
      unsigned int RTCOC1FEIRQEN :  1;  /**< Falling Edge Interrupt Enable for the RTCOC1 Output Compare channel */
      unsigned int RTCOC2FEIRQEN :  1;  /**< Falling Edge Interrupt Enable for the RTCOC2 Output Compare channel */
      unsigned int RTCOC3FEIRQEN :  1;  /**< Falling Edge Interrupt Enable for the RTCOC3 Output Compare channel */
      unsigned int RTCOC4FEIRQEN :  1;  /**< Falling Edge Interrupt Enable for the RTCOC4 Output Compare channel */
      unsigned int RTCOC1IRQEN   :  1;  /**< Interrupt Enable for Output Compare Channel 1 */
      unsigned int RTCOC2IRQEN   :  1;  /**< Posedge EdgeInterrupt Enable for the RTCOC2 Output Compare channel */
      unsigned int RTCOC3IRQEN   :  1;  /**< Posedge EdgeInterrupt Enable for the RTCOC3 Output Compare channel */
      unsigned int RTCOC4IRQEN   :  1;  /**< Posedge EdgeInterrupt Enable for the RTCOC4 Output Compare channel */
      unsigned int reserved13    :  3;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR3OC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR3OC_t__ */

/*@}*/

/** @defgroup CR4OC RTC Control 4 for Configuring Output Compare Channel (CR4OC) Register
 *  RTC Control 4 for Configuring Output Compare Channel (CR4OC) Register.
 *
 *  CR4OC is a control register for configuring enables related to masking and auto-reloading of the 16-bit output-compare channel OC1.
 *  
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_RTC_CR4OC_RTCOC1MSKEN
 *! \brief  Enable for thermometer-code masking of the Output Compare 1 Channel (RTCOC1MSKEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR4OC_EN000 = 0,  /**< Do not apply a mask to the 16-bit Output Compare channel ${.:OC1}.                                                                      */
  RTC_CR4OC_EN001 = 1   /**< Apply a thermometer-decoded mask to the 16-bit Output Compare channel ${.:OC1} provided that channel is enabled via ${.:CR3OC:RTCOC1EN} */
} ADI_RTC_CR4OC_RTCOC1MSKEN;


/* ==========================================================================
 *! \struct ADI_RTC_CR4OC_Struct
 *! \brief  RTC Control 4 for Configuring Output Compare Channel Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR4OC_t__
typedef struct _ADI_RTC_CR4OC_t {
  union {
    struct {
      unsigned int reserved0   :  1;
      unsigned int RTCOC1MSKEN :  1;  /**< Enable for thermometer-code masking of the Output Compare 1 Channel */
      unsigned int RTCOC2MSKEN :  1;  /**< Enable for thermometer-code masking of the Output Compare 2 channel. */
      unsigned int RTCOC3MSKEN :  1;  /**< Enable for thermometer-code masking of the Output Compare 3 channel. */
      unsigned int RTCOC4MSKEN :  1;  /**< Enable for thermometer-code masking of the Output Compare 4 channel. */
      unsigned int RTCOC1POL   :  1;  /**< RTCOC1 Polarity Control */
      unsigned int RTCOC2POL   :  1;  /**< RTCOC2 Polarity Control */
      unsigned int RTCOC3POL   :  1;  /**< RTCOC3 Polarity Control */
      unsigned int RTCOC4POL   :  1;  /**< RTCOC4 Polarity Control */
      unsigned int RTCOC1ARLEN :  1;  /**< Enable for fine control on RTCOC1 period and duty cycle. */
      unsigned int RTCOC2ARLEN :  1;  /**< Enable for fine control on RTCOC2 period and duty cycle. */
      unsigned int RTCOC3ARLEN :  1;  /**< Enable for fine control on RTCOC3 period and duty cycle. */
      unsigned int reserved12  :  4;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR4OC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR4OC_t__ */

/*@}*/

/** @defgroup OCMSK RTC Masks for Output Compare Channel (OCMSK) Register
 *  RTC Masks for Output Compare Channel (OCMSK) Register.
 *
 *  It contains a 4-bit encoded mask, which are decoded out to four 16-bit thermometer-code mask to define contiguous don't care bit positions for target alarm times in the 16-bit output compare channel 1.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OCMSK_Struct
 *! \brief  RTC Masks for Output Compare Channel Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OCMSK_t__
typedef struct _ADI_RTC_OCMSK_t {
  union {
    struct {
      unsigned int RTCOC1MSK  :  4;  /**< Concatenation of thermometer-encoded masks for the 16-bit output compare channels */
      unsigned int RTCOC2MSK  :  4;  /**< RTCOC2 period control */
      unsigned int RTCOC3MSK  :  4;  /**< RTCOC3 period control */
      unsigned int RTCOC4MSK  :  4;  /**< RTCOC4 period control */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OCMSK_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OCMSK_t__ */

/*@}*/

/** @defgroup IC2 RTC Input Capture Channel 2 (IC2) Register
 *  RTC Input Capture Channel 2 (IC2) Register.
 *
 *  IC2 is a read-only snapshot of the 16 lowest {integer_bits, fractional_bits} with meaning of the main 47-bit RTC count at the most recent event on input capture channel 2.
 *  
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_IC2_Struct
 *! \brief  RTC Input Capture Channel 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_IC2_t__
typedef struct _ADI_RTC_IC2_t {
  union {
    struct {
      unsigned int RTCIC2     : 16;  /**< RTC Input Capture Channel 2 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_IC2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_IC2_t__ */

/*@}*/

/** @defgroup IC3 RTC Input Capture Channel 3 (IC3) Register
 *  RTC Input Capture Channel 3 (IC3) Register.
 *
 *  IC3is a read-only snapshot of the 16 lowest {integer_bits, fractional_bits} with meaning of the main 47-bit RTC count at the most recent event on input capture channel 3.
 *  
 *  
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_IC3_Struct
 *! \brief  RTC Input Capture Channel 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_IC3_t__
typedef struct _ADI_RTC_IC3_t {
  union {
    struct {
      unsigned int RTCIC3     : 16;  /**< RTC Input Capture Channel 3 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_IC3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_IC3_t__ */

/*@}*/

/** @defgroup IC4 RTC Input Capture Channel 4 (IC4) Register
 *  RTC Input Capture Channel 4 (IC4) Register.
 *
 *  IC4 is a read-only snapshot of the 16 lowest {integer_bits, fractional_bits} with meaning of the main 47-bit RTC count at the most recent event on input capture channel 4.
 *  
 *  
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_IC4_Struct
 *! \brief  RTC Input Capture Channel 4 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_IC4_t__
typedef struct _ADI_RTC_IC4_t {
  union {
    struct {
      unsigned int RTCIC4     : 16;  /**< RTC Input Capture Channel 4 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_IC4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_IC4_t__ */

/*@}*/

/** @defgroup OC1 RTC Output Compare Channel 1 (OC1) Register
 *  RTC Output Compare Channel 1 (OC1) Register.
 *
 *  OC1 is the scheduled alarm time for output capture channel 1 with respect to the 16 lowest {integer_bits, fractional_bits} with meaning of the main 47-bit RTC count. The upper bits of the main RTC count, beyond these 16 bit positions, are don't cares for the purposes of the 16-bit  OC1 output compare channel.
 *  
 *  
 *  Note that if auto-reloading on the OC1 output-compare channel is enabled using CR4OC:RTCOC1ARLEN, the CPU is given visibility of both the starting value in the output-compare sequence and the current, cumulative target value, taking account of reloads.
 *  
 *  
 *  In such circumstances, a read-back of the OC1 register always returns the starting value of such a reload sequence, whereas a read-back of the OC1TGT register returns the value of the current target value for the OC1 channel, including the effects of cumulative reloads.
 *  
 *  
 *  If reloading is not enabled, the read-back values of OC1 and  OC1TGT are the same, namely the starting (and not reloaded because not enabled) output-compare value in OC1.
 *  
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC1_Struct
 *! \brief  RTC Output Compare Channel 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC1_t__
typedef struct _ADI_RTC_OC1_t {
  union {
    struct {
      unsigned int RTCOC1     : 16;  /**< RTC Output Compare 1 Channel */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC1_t__ */

/*@}*/

/** @defgroup OC2 RTC Output Compare Channel 2 (OC2) Register
 *  RTC Output Compare Channel 2 (OC2) Register.
 *
 *  RTCOC2 is the scheduled alarm time for output capture channel 2 with respect to the 16 lowest {integer_bits, fractional_bits} with meaning of the main 47-bit RTC count. The upper bits of the main RTC count, beyond these 16 bit positions, are don't cares for the purposes of the 16-bit RTCOC2 output compare channel.
 *  
 *  
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC2_Struct
 *! \brief  RTC Output Compare Channel 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC2_t__
typedef struct _ADI_RTC_OC2_t {
  union {
    struct {
      unsigned int RTCOC2     : 16;  /**< RTC Output Compare 2 channel. Scheduled alarm target time. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC2_t__ */

/*@}*/

/** @defgroup OC3 RTC Output Compare Channel 3 (OC3) Register
 *  RTC Output Compare Channel 3 (OC3) Register.
 *
 *  RTCOC3 is the scheduled alarm time for output capture channel 3 with respect to the 16 lowest {integer_bits, fractional_bits} with meaning of the main 47-bit RTC count. The upper bits of the main RTC count, beyond these 16 bit positions, are don't cares for the purposes of the 16-bit RTCOC3 output compare channel.
 *  
 *  
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC3_Struct
 *! \brief  RTC Output Compare Channel 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC3_t__
typedef struct _ADI_RTC_OC3_t {
  union {
    struct {
      unsigned int RTCOC3     : 16;  /**< RTC Output Compare 3 channel. Scheduled alarm target time. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC3_t__ */

/*@}*/

/** @defgroup OC4 RTC Output Compare Channel 4 (OC4) Register
 *  RTC Output Compare Channel 4 (OC4) Register.
 *
 *  RTCOC4 is the scheduled alarm time for output capture channel 4 with respect to the 16 lowest {integer_bits, fractional_bits} with meaning of the main 47-bit RTC count. The upper bits of the main RTC count, beyond these 16 bit positions, are don't cares for the purposes of the 16-bit RTCOC4 output compare channel.
 *  
 *  
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC4_Struct
 *! \brief  RTC Output Compare Channel 4 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC4_t__
typedef struct _ADI_RTC_OC4_t {
  union {
    struct {
      unsigned int RTCOC4     : 16;  /**< RTC Output Compare 4 channel. Scheduled alarm target time. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC4_t__ */

/*@}*/

/** @defgroup SR4 RTC Status 4 (SR4) Register
 *  RTC Status 4 (SR4) Register.
 *
 *  It is a status register which provides the synchronization status of posted writes and posted reads to those registers related to input capture and output control which are sourced in the 32 kHz always-on half of the RTC.
 *  
 *  A WSYNC value of zero for a given register means the results of a previously-posted write to that register are not yet visible to the CPU. A WSYNC value of one for a register means no posted write to that register has yet to complete execution.
 *  
 *  Likewise, an RSYNC value of zero for a given register means the results of a previously-posted read of that register are not yet visible to the CPU. An RSYNC value of one for a register means no posted read of that register has yet to complete execution.
 *  
 *  Note that SR4 only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR4_Struct
 *! \brief  RTC Status 4 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR4_t__
typedef struct _ADI_RTC_SR4_t {
  union {
    struct {
      unsigned int WSYNCSR3   :  1;  /**< Synchronization status of posted clearances to interrupt sources in RTC Status 3 Register */
      unsigned int WSYNCCR2IC :  1;  /**< Synchronization status of posted writes to RTC Control 2 for Configuring Input Capture Channels Register */
      unsigned int WSYNCCR3OC :  1;  /**< Synchronization status of posted writes to RTC Control 3 for Configuring Output Compare Channel Register */
      unsigned int WSYNCCR4OC :  1;  /**< Synchronization status of posted writes to RTC Control 4 for Configuring Output Compare Channel Register */
      unsigned int WSYNCOCMSK :  1;  /**< Synchronization status of posted writes to RTC Masks for Output Compare Channel Register */
      unsigned int reserved5  :  1;
      unsigned int WSYNCOC1   :  1;  /**< Synchronization status of posted writes to RTC Output Compare Channel 1 Register */
      unsigned int WSYNCOC2   :  1;  /**< Synchronization status of posted writes to RTCOC2. */
      unsigned int WSYNCOC3   :  1;  /**< Synchronisation status of posted writes to RTCOC3 */
      unsigned int WSYNCOC4   :  1;  /**< Synchronisation status of posted writes to RTCOC4 */
      unsigned int RSYNCIC0   :  1;  /**< Synchronization status of posted reads of RTC Input Channel 0 */
      unsigned int reserved11 :  1;
      unsigned int RSYNCIC2   :  1;  /**< Synchronization status of posted reads of RTC Input Channel 2 */
      unsigned int RSYNCIC3   :  1;  /**< Synchronization status of posted reads of RTC Input Channel 3 */
      unsigned int RSYNCIC4   :  1;  /**< Synchronization status of posted reads of RTC Input Channel 4 */
      unsigned int WSYNCOCMSKOT :  1;  /**< Synchronization status of posted writes to RTC On Time Masks for Output Compare Channel Register */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR4_t__ */

/*@}*/

/** @defgroup SR5 RTC Status 5 (SR5) Register
 *  RTC Status 5 (SR5) Register.
 *
 *  It is a status register which provides the pending (i.e. buffered and enqueued) status of posted writes to those registers related to input capture and output control which are sourced in the 32 kHz always-on half of the RTC.
 *  
 *  A WPEND value of one for a given register means a previously-posted write to that register is currently queued up in a one-deep buffer and has yet to begin execution. When WPEND is therefore one, no further write to the register in question can be accommodated by the RTC until the existing write begins execution and vacates the buffer.
 *  
 *  When WPEND is zero for a register, there is room in the RTC to accept a new posted write to that register.
 *  
 *  Note that SR5 only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR5_Struct
 *! \brief  RTC Status 5 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR5_t__
typedef struct _ADI_RTC_SR5_t {
  union {
    struct {
      unsigned int WPENDSR3   :  1;  /**< Pending status of posted clearances of interrupt sources in RTC Status 3 Register */
      unsigned int WPENDCR2IC :  1;  /**< Pending status of posted writes to RTC Control 2 for Configuring Input Capture Channels Register */
      unsigned int WPENDCR3OC :  1;  /**< Pending status of posted writes to RTC Control 3 for Configuring Output Compare Channel Register */
      unsigned int WPENDCR4OC :  1;  /**< Pending status of posted writes to RTC Control 4 for Configuring Output Compare Channel Register */
      unsigned int WPENDOCMSK :  1;  /**< Pending status of posted writes to RTC Masks for Output Compare Channel Register */
      unsigned int reserved5  :  1;
      unsigned int WPENDOC1   :  1;  /**< Pending status of posted writes to Output Compare Channel 1 */
      unsigned int WPENDOC2   :  1;  /**< Pending status of posted writes to RTCOC2 */
      unsigned int WPENDOC3   :  1;  /**< Pending status of posted writes to RTCOC3 */
      unsigned int WPENDOC4   :  1;  /**< Pending status of posted writes to RTCOC4 */
      unsigned int RPENDIC0   :  1;  /**< Pending status of posted reads of RTC Input Channel 0 */
      unsigned int reserved11 :  1;
      unsigned int RPENDIC2   :  1;  /**< Pending status of posted reads of RTC Input Channel 2 */
      unsigned int RPENDIC3   :  1;  /**< Pending status of posted reads of RTC Input Channel 3 */
      unsigned int RPENDIC4   :  1;  /**< Pending status of posted reads of RTC Input Channel 4 */
      unsigned int WPENDOCMSKOT :  1;  /**< Pending status of posted writes to RTC Masks for Output Compare Channel Register */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR5_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR5_t__ */

/*@}*/

/** @defgroup SR6 RTC Status 6 (SR6) Register
 *  RTC Status 6 (SR6) Register.
 *
 *  It is a status register which provides the unread status of snapshots of input-capture channels, RTC_IC0, RTC_IC2, RTC_IC3 and RTC_IC4.
 *  
 *  Both the captured snapshots themselves and the sticky flags, RTCIC<channel number>UNR, which indicate if the snapshots have been read by the CPU, are all sourced in the in the 32 kHz always-on half of the RTC.
 *  
 *  Note that SR6 only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR6_Struct
 *! \brief  RTC Status 6 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR6_t__
typedef struct _ADI_RTC_SR6_t {
  union {
    struct {
      unsigned int RTCIC0UNR  :  1;  /**< Sticky unread status of the RTC Input Capture Channel 0 */
      unsigned int reserved1  :  1;
      unsigned int RTCIC2UNR  :  1;  /**< Sticky unread status of the RTC Input Capture Channel 2 */
      unsigned int RTCIC3UNR  :  1;  /**< Sticky unread status of the RTC Input Capture Channel 3 */
      unsigned int RTCIC4UNR  :  1;  /**< Sticky unread status of the RTC Input Capture Channel 4 */
      unsigned int reserved5  :  3;
      unsigned int RTCIC0SNAP :  1;  /**< Confirmation that RTC Snapshot 0, 1, 2 registers reflect the value of RTC Input Capture Channel 0 */
      unsigned int RTCFRZCNTPTR :  2;  /**< Pointer for the triple-read sequence of the RTC Freeze Count MMR */
      unsigned int reserved11   :  5;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR6_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR6_t__ */

/*@}*/

/** @defgroup OC1TGT RTC Output Compare Channel 1 Target (OC1TGT) Register
 *  RTC Output Compare Channel 1 Target (OC1TGT) Register.
 *
 *  Read-only register which reflects the current, cumulative target alarm time for the OC1 channel, taking account of any auto-reloading upon output-compare events.
 *  OC1TGT only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  
 *  
 *  Any of the following configuration changes, when synchronized to the 32 kHz domain, causes a re-initialization of the cumulative OC1TGT:RTCOC1TGT target time, such that it is set back again to the starting value in the OC1 register for any subsequent auto-reload sequence:
 *  
 *  
 *  (i) A redefinition occurs of the starting value itself due to a write to the OC1 MMR.
 *  
 *  (ii) Whenever  CR3OC:RTCOC1EN transitions from 0 to 1, thus enabling the OC1 output-compare channel.
 *  
 *  (iii) Whenever CR4OC:RTCOC1ARLEN transitions from 1 to 0, thus disabling auto-reloading for OC1.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC1TGT_Struct
 *! \brief  RTC Output Compare Channel 1 Target Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC1TGT_t__
typedef struct _ADI_RTC_OC1TGT_t {
  union {
    struct {
      unsigned int RTCOC1TGT  : 16;  /**< Provides visibility to the CPU of the current target value for the output-compare channel 1, taking account of any possible auto-reloading */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC1TGT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC1TGT_t__ */

/*@}*/

/** @defgroup FRZCNT RTC Freeze Count (FRZCNT) Register
 *  RTC Freeze Count (FRZCNT) Register.
 *
 *  RTC Freeze Count MMR allows a coherent, triple 16-bit read of the 47-bit RTC count contained in {CNT2, CNT1, CNT0}.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_FRZCNT_Struct
 *! \brief  RTC Freeze Count Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_FRZCNT_t__
typedef struct _ADI_RTC_FRZCNT_t {
  union {
    struct {
      unsigned int RTCFRZCNT  : 16;  /**< RTC Freeze Count */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_FRZCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_FRZCNT_t__ */

/*@}*/

/** @defgroup OC2TGT RTC Output Compare Channel 2 Target (OC2TGT) Register
 *  RTC Output Compare Channel 2 Target (OC2TGT) Register.
 *
 *  Read-only register which reflects the current, cumulative target alarm time for the OC2 channel, taking account of any auto-reloading upon output-compare events.
 *  Note that OC2TGT only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC2TGT_Struct
 *! \brief  RTC Output Compare Channel 2 Target Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC2TGT_t__
typedef struct _ADI_RTC_OC2TGT_t {
  union {
    struct {
      unsigned int RTCOC2TGT  : 16;  /**< Current, cumulative target time for Output Compare Channel 1, taking account of any auto-reloading */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC2TGT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC2TGT_t__ */

/*@}*/

/** @defgroup OC3TGT RTC Output Compare Channel 3 Target (OC3TGT) Register
 *  RTC Output Compare Channel 3 Target (OC3TGT) Register.
 *
 *  Read-only register which reflects the current, cumulative target alarm time for the OC3 channel, taking account of any auto-reloading upon output-compare events.
 *  Note that OC3TGT only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC3TGT_Struct
 *! \brief  RTC Output Compare Channel 3 Target Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC3TGT_t__
typedef struct _ADI_RTC_OC3TGT_t {
  union {
    struct {
      unsigned int RTCOC3TGT  : 16;  /**< Current, cumulative target time for Output Compare Channel 1, taking account of any auto-reloading */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC3TGT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC3TGT_t__ */

/*@}*/

/** @defgroup OC1LOWDUR RTC Auto-Reload Low Duration for Output Compare Channel 1 (OC1LOWDUR) Register
 *  RTC Auto-Reload Low Duration for Output Compare Channel 1 (OC1LOWDUR) Register.
 *
 *  OC1LOWDUR contains the 16-bit reload value which is optionally (enabled by CR4OC:RTCOC1ARLEN) added to the cumulative value of OC1, visible in the OC1TGT register, whenever a enabled output compare event occurs on that channel. 
 *  
 *  The use of OC1LOWDUR allows a repeating alarm whose periodicity either is or is not a power of 2 to be put into effect for OC1. 
 *  
 *  If reloading is not enabled, the read-back values of OC1 and OC1TGT are the same, namely the starting (and not reloaded because not enabled) output-compare value in OC1.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC1LOWDUR_Struct
 *! \brief  RTC Auto-Reload Low Duration for Output Compare Channel 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC1LOWDUR_t__
typedef struct _ADI_RTC_OC1LOWDUR_t {
  union {
    struct {
      unsigned int RTCOC1LOWDUR : 16;  /**< Low Duration for RTCOC1. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC1LOWDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC1LOWDUR_t__ */

/*@}*/

/** @defgroup OC2LOWDUR RTC Auto-Reload Low Duration for Output Compare Channel 2 (OC2LOWDUR) Register
 *  RTC Auto-Reload Low Duration for Output Compare Channel 2 (OC2LOWDUR) Register.
 *
 *  OC2LOWDUR contains the 16-bit reload value which is optionally (enabled by CR4OC:RTCOC2ARLEN) added to the cumulative value of OC2, visible in the OC2TGT register, whenever a enabled output compare event occurs on that channel. 
 *  
 *  The use of OC2LOWDUR allows a repeating alarm whose periodicity either is or is not a power of 2 to be put into effect for OC2. 
 *  
 *  If reloading is not enabled, the read-back values of OC2 and OC2TGT are the same, namely the starting (and not reloaded because not enabled) output-compare value in OC2.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC2LOWDUR_Struct
 *! \brief  RTC Auto-Reload Low Duration for Output Compare Channel 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC2LOWDUR_t__
typedef struct _ADI_RTC_OC2LOWDUR_t {
  union {
    struct {
      unsigned int RTCOC2LOWDUR : 16;  /**< Low Duration for RTCOC2. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC2LOWDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC2LOWDUR_t__ */

/*@}*/

/** @defgroup OC3LOWDUR RTC Auto-Reload Low Duration for Output Compare Channel 3 (OC3LOWDUR) Register
 *  RTC Auto-Reload Low Duration for Output Compare Channel 3 (OC3LOWDUR) Register.
 *
 *  OC3LOWDUR contains the 16-bit reload value which is optionally (enabled by CR4OC:RTCOC3ARLEN) added to the cumulative value of OC3, visible in the OC3TGT register, whenever a enabled output compare event occurs on that channel. 
 *  
 *  The use of OC3LOWDUR allows a repeating alarm whose periodicity either is or is not a power of 2 to be put into effect for OC3. 
 *  
 *  If reloading is not enabled, the read-back values of OC3 and OC3TGT are the same, namely the starting (and not reloaded because not enabled) output-compare value in OC3.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC3LOWDUR_Struct
 *! \brief  RTC Auto-Reload Low Duration for Output Compare Channel 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC3LOWDUR_t__
typedef struct _ADI_RTC_OC3LOWDUR_t {
  union {
    struct {
      unsigned int RTCOC3LOWDUR : 16;  /**< Low Duration for RTCOC3. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC3LOWDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC3LOWDUR_t__ */

/*@}*/

/** @defgroup OC1HIGHDUR RTC Auto-Reload High Duration for Output Compare Channel 1 (OC1HIGHDUR) Register
 *  RTC Auto-Reload High Duration for Output Compare Channel 1 (OC1HIGHDUR) Register.
 *
 *  OC1HIGHDUR contains the 16-bit reload value which is optionally (enabled by CR4OC:RTCOC1ARLEN) added to the cumulative value of OC1, visible in the OC1TGT register, whenever a enabled output compare event occurs on that channel. 
 *  
 *  The use of OC1HIGHDUR allows a repeating alarm whose periodicity either is or is not a power of 2 to be put into effect for OC1. 
 *  
 *  If reloading is not enabled, the read-back values of OC1 and OC1TGT are the same, namely the starting (and not reloaded because not enabled) output-compare value in OC1.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC1HIGHDUR_Struct
 *! \brief  RTC Auto-Reload High Duration for Output Compare Channel 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC1HIGHDUR_t__
typedef struct _ADI_RTC_OC1HIGHDUR_t {
  union {
    struct {
      unsigned int RTCOC1HIGHDUR : 16;  /**< High Duration for RTCOC1. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC1HIGHDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC1HIGHDUR_t__ */

/*@}*/

/** @defgroup OC2HIGHDUR RTC Auto-Reload High Duration for Output Compare Channel 2 (OC2HIGHDUR) Register
 *  RTC Auto-Reload High Duration for Output Compare Channel 2 (OC2HIGHDUR) Register.
 *
 *  OC2HIGHDUR contains the 16-bit reload value which is optionally (enabled by CR4OC:RTCOC2ARLEN) added to the cumulative value of OC2, visible in the OC2TGT register, whenever a enabled output compare event occurs on that channel. 
 *  
 *  The use of OC2HIGHDUR allows a repeating alarm whose periodicity either is or is not a power of 2 to be put into effect for OC2. 
 *  
 *  If reloading is not enabled, the read-back values of OC2 and OC2TGT are the same, namely the starting (and not reloaded because not enabled) output-compare value in OC2.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC2HIGHDUR_Struct
 *! \brief  RTC Auto-Reload High Duration for Output Compare Channel 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC2HIGHDUR_t__
typedef struct _ADI_RTC_OC2HIGHDUR_t {
  union {
    struct {
      unsigned int RTCOC2HIGHDUR : 16;  /**< High Duration for RTCOC2. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC2HIGHDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC2HIGHDUR_t__ */

/*@}*/

/** @defgroup OC3HIGHDUR RTC Auto-Reload High Duration for Output Compare Channel 3 (OC3HIGHDUR) Register
 *  RTC Auto-Reload High Duration for Output Compare Channel 3 (OC3HIGHDUR) Register.
 *
 *  OC3HIGHDUR contains the 16-bit reload value which is optionally (enabled by CR4OC:RTCOC3ARLEN) added to the cumulative value of OC3, visible in the OC3TGT register, whenever a enabled output compare event occurs on that channel. 
 *  
 *  The use of OC3HIGHDUR allows a repeating alarm whose periodicity either is or is not a power of 2 to be put into effect for OC3. 
 *  
 *  If reloading is not enabled, the read-back values of OC3 and OC3TGT are the same, namely the starting (and not reloaded because not enabled) output-compare value in OC3.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OC3HIGHDUR_Struct
 *! \brief  RTC Auto-Reload High Duration for Output Compare Channel 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OC3HIGHDUR_t__
typedef struct _ADI_RTC_OC3HIGHDUR_t {
  union {
    struct {
      unsigned int RTCOC3HIGHDUR : 16;  /**< High Duration for RTCOC3. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OC3HIGHDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OC3HIGHDUR_t__ */

/*@}*/

/** @defgroup OCMSKOT RTC Masks for Output Compare Channel (OCMSKOT) Register
 *  RTC Masks for Output Compare Channel (OCMSKOT) Register.
 *
 *  It contains four 4-bit encoded masks, which are decoded out to four 16-bit thermometer-code masks to define contiguous don't care bit positions for target alarm times in the 16-bit output compare channel.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_OCMSKOT_Struct
 *! \brief  RTC Masks for Output Compare Channel Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_OCMSKOT_t__
typedef struct _ADI_RTC_OCMSKOT_t {
  union {
    struct {
      unsigned int RTCOC1MSKOT :  4;  /**< Concatenation of thermometer-encoded masks for the 16-bit output compare channels */
      unsigned int RTCOC2MSKOT :  4;  /**< RTCOC2 on time control */
      unsigned int RTCOC3MSKOT :  4;  /**< RTCOC3 on time control */
      unsigned int RTCOC4MSKOT :  4;  /**< RTCOC4 on time control */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_OCMSKOT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_OCMSKOT_t__ */

/*@}*/

/** @defgroup CR5OCS RTC Control 5 for Configuring Output Compare Channel GPIO Sampling (CR5OCS) Register
 *  RTC Control 5 for Configuring Output Compare Channel GPIO Sampling (CR5OCS) Register.
 *
 *  CR5OCS is a control register for configuring enables related to sampling of GPIO for every 16-bit output-compare channels. CR5OCS contains enables for both the gpio sampling for each channel, as well as enables for whether an event on a channel should contribute to the interrupt lines from the RTC to both the CPU and the wake-up controller.
 *  
 *  Note that the 47-bit RTC_OC0 channel is not controlled by CR5OCS. This is because RTCOC0 is a synonym for the main 47-bit RTC alarm (whose interrupt source is ALMINT), which is controlled by the CR0:ALMEN and CR0:ALMINTEN fields
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CR5OCS_Struct
 *! \brief  RTC Control 5 for Configuring Output Compare Channel GPIO Sampling Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR5OCS_t__
typedef struct _ADI_RTC_CR5OCS_t {
  union {
    struct {
      unsigned int OC1SMPEN   :  3;  /**< GPIO input sample enable for RTC Output Compare Channel 1 */
      unsigned int OC1SMPMTCHIRQEN :  1;  /**< Sample activity Interrupt enable for RTC Output Compare Channel 1. */
      unsigned int OC2SMPEN        :  3;  /**< GPIO input sample enable for RTC Output Compare Channel 2 */
      unsigned int OC2SMPMTCHIRQEN :  1;  /**< Sample activity Interrupt enable for RTC Output Compare Channel 2 */
      unsigned int OC3SMPEN        :  3;  /**< GPIO input sample enable for RTC Output Compare Channel 3 */
      unsigned int OC3SMPMTCHIRQEN :  1;  /**< Sample activity Interrupt enable for RTC Output Compare Channel 3 */
      unsigned int reserved12      :  4;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR5OCS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR5OCS_t__ */

/*@}*/

/** @defgroup CR6OCS RTC Control 5 for Configuring Output Compare Channel GPIO Sampling Edge (CR6OCS) Register
 *  RTC Control 5 for Configuring Output Compare Channel GPIO Sampling Edge (CR6OCS) Register.
 *
 *  CR6OCS is a control register for configuring GPIO sampling edges with respective to every 16-bit output-compare channel activity. CR6OCS contains enables to configure sampling around rising edge of OPC or falling edge of OPC or both for every OPC channel.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_RTC_CR6OCS_OC1SMPONFE
 *! \brief  GPIO Sample around Falling Edge of Output Compare Channel 1 (OC1SMPONFE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6OCS_OC1NOFES = 0,  /**< No sampling of input around falling edge                 */
  RTC_CR6OCS_OC1BFES  = 1,  /**< Input sampled one clock cycle before falling edge of OPC */
  RTC_CR6OCS_OC1FES   = 2,  /**< Input sampled at falling edge of OPC                     */
  RTC_CR6OCS_OC1AFES  = 3   /**< Input sampled one clock cycle after falling edge of OPC  */
} ADI_RTC_CR6OCS_OC1SMPONFE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6OCS_OC1SMPONRE
 *! \brief  GPIO Sample around Rising Edge of Output Compare Channel 1 (OC1SMPONRE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6OCS_OC1NORES = 0,  /**< No sampling of input around rising edge                 */
  RTC_CR6OCS_OC1BRES  = 1,  /**< Input sampled one clock cycle before rising edge of OPC */
  RTC_CR6OCS_OC1RES   = 2,  /**< Input sampled at rising edge of OPC                     */
  RTC_CR6OCS_OC1ARES  = 3   /**< Input sampled one clock cycle after rising edge of OPC  */
} ADI_RTC_CR6OCS_OC1SMPONRE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6OCS_OC2SMPONFE
 *! \brief  GPIO Sample around Falling Edge of Output Compare Channel 2 (OC2SMPONFE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6OCS_OC2NOFES = 0,  /**< No sampling of input around falling edge                 */
  RTC_CR6OCS_OC2BFES  = 1,  /**< Input sampled one clock cycle before falling edge of OPC */
  RTC_CR6OCS_OC2FES   = 2,  /**< Input sampled at falling edge of OPC                     */
  RTC_CR6OCS_OC2AFES  = 3   /**< Input sampled one clock cycle after falling edge of OPC  */
} ADI_RTC_CR6OCS_OC2SMPONFE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6OCS_OC2SMPONRE
 *! \brief  GPIO Sample around Rising Edge of Output Compare Channel 2 (OC2SMPONRE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6OCS_OC2NORES = 0,  /**< No sampling of input around rising edge                 */
  RTC_CR6OCS_OC2BRES  = 1,  /**< Input sampled one clock cycle before rising edge of OPC */
  RTC_CR6OCS_OC2RES   = 2,  /**< Input sampled at rising edge of OPC                     */
  RTC_CR6OCS_OC2ARES  = 3   /**< Input sampled one clock cycle after rising edge of OPC  */
} ADI_RTC_CR6OCS_OC2SMPONRE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6OCS_OC3SMPONFE
 *! \brief  GPIO Sample around Falling Edge of Output Compare Channel 3 (OC3SMPONFE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6OCS_OC3NOFES = 0,  /**< No sampling of input around falling edge                 */
  RTC_CR6OCS_OC3BFES  = 1,  /**< Input sampled one clock cycle before falling edge of OPC */
  RTC_CR6OCS_OC3FES   = 2,  /**< Input sampled at falling edge of OPC                     */
  RTC_CR6OCS_OC3AFES  = 3   /**< Input sampled one clock cycle after falling edge of OPC  */
} ADI_RTC_CR6OCS_OC3SMPONFE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6OCS_OC3SMPONRE
 *! \brief  GPIO Sample around Rising Edge of Output Compare Channel 3 (OC3SMPONRE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6OCS_OC3NORES = 0,  /**< No sampling of input around rising edge                 */
  RTC_CR6OCS_OC3BRES  = 1,  /**< Input sampled one clock cycle before rising edge of OPC */
  RTC_CR6OCS_OC3RES   = 2,  /**< Input sampled at rising edge of OPC                     */
  RTC_CR6OCS_OC3ARES  = 3   /**< Input sampled one clock cycle after rising edge of OPC  */
} ADI_RTC_CR6OCS_OC3SMPONRE;


/* ==========================================================================
 *! \struct ADI_RTC_CR6OCS_Struct
 *! \brief  RTC Control 5 for Configuring Output Compare Channel GPIO Sampling Edge Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR6OCS_t__
typedef struct _ADI_RTC_CR6OCS_t {
  union {
    struct {
      unsigned int OC1SMPONFE :  2;  /**< GPIO Sample around Falling Edge of Output Compare Channel 1 */
      unsigned int OC1SMPONRE :  2;  /**< GPIO Sample around Rising Edge of Output Compare Channel 1 */
      unsigned int OC2SMPONFE :  2;  /**< GPIO Sample around Falling Edge of Output Compare Channel 2 */
      unsigned int OC2SMPONRE :  2;  /**< GPIO Sample around Rising Edge of Output Compare Channel 2 */
      unsigned int OC3SMPONFE :  2;  /**< GPIO Sample around Falling Edge of Output Compare Channel 3 */
      unsigned int OC3SMPONRE :  2;  /**< GPIO Sample around Rising Edge of Output Compare Channel 3 */
      unsigned int reserved12 :  4;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR6OCS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR6OCS_t__ */

/*@}*/

/** @defgroup CR7OCS RTC Control 5 for Configuring Output Compare Channel GPIO Sampling Activity (CR7OCS) Register
 *  RTC Control 5 for Configuring Output Compare Channel GPIO Sampling Activity (CR7OCS) Register.
 *
 *  CR7OCS is a control register for configuring GPIO activity at which interrupt to core is sent. GPIO activity can be denoted as inputs changing or inputs not changing or input matching expected value or input not matching int he expected value. This activity is recorded in SR7 register and can be optionally sent as interrupt to core.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_RTC_CR7OCS_OC1SMPPTRN
 *! \brief  Sample activity selection for Output Compare Channel 1 (OC1SMPPTRN) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR7OCS_OC1SMPCHNG   = 0,  /**< Current GPIO sample is not same as previous sample */
  RTC_CR7OCS_OC1SMPSAME   = 1,  /**< Current GPIO sample is same as previous sample     */
  RTC_CR7OCS_OC1SMPMTCH   = 2,  /**< Current GPIO sample is same as expected sample     */
  RTC_CR7OCS_OC1SMPNOMTCH = 3   /**< Current GPIO sample is not same as expected sample */
} ADI_RTC_CR7OCS_OC1SMPPTRN;


/*  =========================================================================
 *! \enum   ADI_RTC_CR7OCS_OC2SMPPTRN
 *! \brief  Sample activity selection for Output Compare Channel 2 (OC2SMPPTRN) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR7OCS_OC2SMPCHNG   = 0,  /**< Current GPIO sample is not same as previous sample */
  RTC_CR7OCS_OC2SMPSAME   = 1,  /**< Current GPIO sample is same as previous sample     */
  RTC_CR7OCS_OC2SMPMTCH   = 2,  /**< Current GPIO sample is same as expected sample     */
  RTC_CR7OCS_OC2SMPNOMTCH = 3   /**< Current GPIO sample is not same as expected sample */
} ADI_RTC_CR7OCS_OC2SMPPTRN;


/*  =========================================================================
 *! \enum   ADI_RTC_CR7OCS_OC3SMPPTRN
 *! \brief  Sample activity selection for Output Compare Channel 3 (OC3SMPPTRN) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR7OCS_OC3SMPCHNG   = 0,  /**< Current GPIO sample is not same as previous sample */
  RTC_CR7OCS_OC3SMPSAME   = 1,  /**< Current GPIO sample is same as previous sample     */
  RTC_CR7OCS_OC3SMPMTCH   = 2,  /**< Current GPIO sample is same as expected sample     */
  RTC_CR7OCS_OC3SMPNOMTCH = 3   /**< Current GPIO sample is not same as expected sample */
} ADI_RTC_CR7OCS_OC3SMPPTRN;


/* ==========================================================================
 *! \struct ADI_RTC_CR7OCS_Struct
 *! \brief  RTC Control 5 for Configuring Output Compare Channel GPIO Sampling Activity Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR7OCS_t__
typedef struct _ADI_RTC_CR7OCS_t {
  union {
    struct {
      unsigned int OC1SMPEXP  :  3;  /**< Expected GPIO Sample for Output Compare Channel 1 */
      unsigned int OC1SMPPTRN :  2;  /**< Sample activity selection for Output Compare Channel 1 */
      unsigned int OC2SMPEXP  :  3;  /**< Expected GPIO Sample for Output Compare Channel 2 */
      unsigned int OC2SMPPTRN :  2;  /**< Sample activity selection for Output Compare Channel 2 */
      unsigned int OC3SMPEXP  :  3;  /**< Expected GPIO Sample for Output Compare Channel 3 */
      unsigned int OC3SMPPTRN :  2;  /**< Sample activity selection for Output Compare Channel 3 */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR7OCS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR7OCS_t__ */

/*@}*/

/** @defgroup SR7 RTC Status 7 (SR7) Register
 *  RTC Status 7 (SR7) Register.
 *
 *  It is a status register which provides the sampled GPIO data for every OPC channel. And the status of pattern match in these values.
 *  
 *  Note that SR7 only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR7_Struct
 *! \brief  RTC Status 7 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR7_t__
typedef struct _ADI_RTC_SR7_t {
  union {
    struct {
      unsigned int RTCOC1SMP  :  3;  /**< Latest GPIO sample for RTC Output Compare Channel 1 */
      unsigned int RTCOC1SMPMTCHIRQ :  1;  /**< Sticky status of GPIO sample pattern match for RTC Output Compare Channel 1 */
      unsigned int RTCOC2SMP        :  3;  /**< Latest GPIO sample for RTC Output Compare Channel 2 */
      unsigned int RTCOC2SMPMTCHIRQ :  1;  /**< Sticky status of GPIO sample pattern match for RTC Output Compare Channel 2 */
      unsigned int RTCOC3SMP        :  3;  /**< Latest GPIO sample for RTC Output Compare Channel 3 */
      unsigned int RTCOC3SMPMTCHIRQ :  1;  /**< Sticky status of GPIO sample pattern match for RTC Output Compare Channel 3 */
      unsigned int RTCOC1OUT        :  1;  /**< Output value for RTC Output Compare Channel 1 */
      unsigned int RTCOC2OUT        :  1;  /**< Output value for RTC Output Compare Channel 2 */
      unsigned int RTCOC3OUT        :  1;  /**< Output value for RTC Output Compare Channel 3 */
      unsigned int RTCOC4OUT        :  1;  /**< Output value for RTC Output Compare Channel 4 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR7_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR7_t__ */

/*@}*/

/** @defgroup SR8 RTC Status 8 (SR8) Register
 *  RTC Status 8 (SR8) Register.
 *
 *  It is a status register which provides the synchronization status of posted writes and posted reads to those registers related to input capture and output control which are sourced in the 32 kHz always-on half of the RTC.
 *  
 *  A WSYNC value of zero for a given register means the results of a previously-posted write to that register are not yet visible to the CPU. A WSYNC value of one for a register means no posted write to that register has yet to complete execution.
 *  
 *  Likewise, an RSYNC value of zero for a given register means the results of a previously-posted read of that register are not yet visible to the CPU. An RSYNC value of one for a register means no posted read of that register has yet to complete execution.
 *  
 *  Note that SR8 only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR8_Struct
 *! \brief  RTC Status 8 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR8_t__
typedef struct _ADI_RTC_SR8_t {
  union {
    struct {
      unsigned int WSYNCOC1LOWDUR :  1;  /**< Synchronization status of posted writes to RTCOC1 low duration for Output Compare Channel Register */
      unsigned int WSYNCOC2LOWDUR :  1;  /**< Synchronization status of posted writes to RTCOC2 low duration for Output Compare Channel Register */
      unsigned int WSYNCOC3LOWDUR :  1;  /**< Synchronization status of posted writes to RTCOC3 low duration for Output Compare Channel Register */
      unsigned int reserved3       :  1;
      unsigned int WSYNCOC1HIGHDUR :  1;  /**< Synchronization status of posted writes to RTCOC1 low duration for Output Compare Channel Register */
      unsigned int WSYNCOC2HIGHDUR :  1;  /**< Synchronization status of posted writes to RTCOC2 low duration for Output Compare Channel Register */
      unsigned int WSYNCOC3HIGHDUR :  1;  /**< Synchronization status of posted writes to RTCOC3 low duration for Output Compare Channel Register */
      unsigned int reserved7       :  1;
      unsigned int WSYNCCR5OCS     :  1;  /**< Synchronization status of posted writes to RTCOC1 sample control for Output Compare Channel Register */
      unsigned int WSYNCCR6OCS     :  1;  /**< Synchronization status of posted writes to RTCOC2 sample control for Output Compare Channel Register */
      unsigned int WSYNCCR7OCS     :  1;  /**< Synchronization status of posted writes to RTCOC3 sample control for Output Compare Channel Register */
      unsigned int WSYNCSR7        :  1;  /**< Synchronization status of posted writes to RTCSR7 sample control for Output Compare Channel Register */
      unsigned int WSYNCGPMUX0     :  1;  /**< Synchronization status of posted writes to RTCGPMUX0 pin mux control for Output Compare Channel Register */
      unsigned int WSYNCGPMUX1     :  1;  /**< Synchronization status of posted writes to RTCGPMUX1 pin mux control for Output Compare Channel Register */
      unsigned int reserved14      :  2;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR8_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR8_t__ */

/*@}*/

/** @defgroup SR9 RTC Status 9 (SR9) Register
 *  RTC Status 9 (SR9) Register.
 *
 *  It is a status register which provides the pending (i.e. buffered and enqueued) status of posted writes to those registers related to input capture and output control which are sourced in the 32 kHz always-on half of the RTC.
 *  
 *  A WPEND value of one for a given register means a previously-posted write to that register is currently queued up in a one-deep buffer and has yet to begin execution. When WPEND is therefore one, no further write to the register in question can be accommodated by the RTC until the existing write begins execution and vacates the buffer.
 *  
 *  When WPEND is zero for a register, there is room in the RTC to accept a new posted write to that register.
 *  
 *  Note that SR9 only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SR9_Struct
 *! \brief  RTC Status 9 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SR9_t__
typedef struct _ADI_RTC_SR9_t {
  union {
    struct {
      unsigned int WPENDOC1LOWDUR :  1;  /**< Pending status of posted writes to RTCOC1 Low Duration for Output Compare Channel Register */
      unsigned int WPENDOC2LOWDUR :  1;  /**< Pending status of posted writes to RTCOC2 Low Duration for Output Compare Channel Register */
      unsigned int WPENDOC3LOWDUR :  1;  /**< Pending status of posted writes to RTCOC3 Low Duration for Output Compare Channel Register */
      unsigned int reserved3       :  1;
      unsigned int WPENDOC1HIGHDUR :  1;  /**< Pending status of posted writes to RTCOC1 High Duration for Output Compare Channel Register */
      unsigned int WPENDOC2HIGHDUR :  1;  /**< Pending status of posted writes to RTCOC2 High Duration for Output Compare Channel Register */
      unsigned int WPENDOC3HIGHDUR :  1;  /**< Pending status of posted writes to RTCOC3 High Duration for Output Compare Channel Register */
      unsigned int reserved7       :  1;
      unsigned int WPENDCR5OCS     :  1;  /**< Pending status of posted writes to RTCOC1 Sample control for Output Compare Channel Register */
      unsigned int WPENDCR6OCS     :  1;  /**< Pending status of posted writes to RTCOC2 Sample control for Output Compare Channel Register */
      unsigned int WPENDCR7OCS     :  1;  /**< Pending status of posted writes to RTCOC3 Sample control for Output Compare Channel Register */
      unsigned int WPENDSR7        :  1;  /**< Pending status of posted writes to RTCSR7 status for Output Compare Channel Register */
      unsigned int WPENDGPMUX0     :  1;  /**< Pending status of posted writes to RTCGPMUX0 for Output Compare Channel Register */
      unsigned int WPENDGPMUX1     :  1;  /**< Pending status of posted writes to RTCGPMUX1 for Output Compare Channel Register */
      unsigned int reserved14      :  2;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR9_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR9_t__ */

/*@}*/

/** @defgroup GPMUX0 RTC GPIO pin mux control register 0 (GPMUX0) Register
 *  RTC GPIO pin mux control register 0 (GPMUX0) Register.
 *
 *  GPMUX0is a control register for selecting a GPIO (pin) as data to be sampled by a output compare channel. This register has to be updated only when appropriate OPC channel is disabled.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_GPMUX0_Struct
 *! \brief  RTC GPIO pin mux control register 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_GPMUX0_t__
typedef struct _ADI_RTC_GPMUX0_t {
  union {
    struct {
      unsigned int OC1GPIN0SEL :  3;  /**< GPIO mux selection for output compare channel1 input0 */
      unsigned int OC1GPIN1SEL :  3;  /**< GPIO mux selection for output compare channel1 input1 */
      unsigned int OC1GPIN2SEL :  3;  /**< GPIO mux selection for output compare channel1 input2 */
      unsigned int OC2GPIN0SEL :  3;  /**< GPIO mux selection for output compare channel2 input0 */
      unsigned int OC2GPIN1SEL :  3;  /**< GPIO mux selection for output compare channel2 input1 */
      unsigned int reserved15  :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_GPMUX0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_GPMUX0_t__ */

/*@}*/

/** @defgroup GPMUX1 RTC GPIO pin mux control register 1 (GPMUX1) Register
 *  RTC GPIO pin mux control register 1 (GPMUX1) Register.
 *
 *  GPMUX1is a control register for selecting a GPIO (pin) as data to be sampled by a output compare channel. This register has to be updated only when appropriate OPC channel is disabled.
 *  
 *  Note that this register only exists in RTC1. In RTC0, the register address is reserved and reads back as the register's reset value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_GPMUX1_Struct
 *! \brief  RTC GPIO pin mux control register 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_GPMUX1_t__
typedef struct _ADI_RTC_GPMUX1_t {
  union {
    struct {
      unsigned int OC2GPIN2SEL :  3;  /**< GPIO mux selection for output compare channel2 input2 */
      unsigned int OC3GPIN0SEL :  3;  /**< GPIO mux selection for output compare channel3 input0 */
      unsigned int OC3GPIN1SEL :  3;  /**< GPIO mux selection for output compare channel3 input1 */
      unsigned int OC3GPIN2SEL :  3;  /**< GPIO mux selection for output compare channel3 input2 */
      unsigned int reserved12  :  2;
      unsigned int OC1DIFFOUT  :  1;  /**< Differential OPC out option for output compare channel1 */
      unsigned int OC3DIFFOUT  :  1;  /**< Differential OPC out option for output compare channel3 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_GPMUX1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_GPMUX1_t__ */

/*@}*/

/** @defgroup ADIID ADI Identification (ADIID) Register
 *  ADI Identification (ADIID) Register.
 *
 *  ADI Cortex device identification.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SYS_ADIID_Struct
 *! \brief  ADI Identification Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SYS_ADIID_t__
typedef struct _ADI_SYS_ADIID_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Reads a fixed value of 0x4144 to indicate to debuggers that they are connected to an Analog Devices implemented Cortex based part. */
    };
    uint16_t VALUE16;
  };
} ADI_SYS_ADIID_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SYS_ADIID_t__ */

/*@}*/

/** @defgroup CHIPID Chip Identifier (CHIPID) Register
 *  Chip Identifier (CHIPID) Register.
 *
 *  Chip identification.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SYS_CHIPID_Struct
 *! \brief  Chip Identifier Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SYS_CHIPID_t__
typedef struct _ADI_SYS_CHIPID_t {
  union {
    struct {
      unsigned int REV        :  4;  /**< Silicon revision */
      unsigned int PARTID     : 12;  /**< Part identifier */
    };
    uint16_t VALUE16;
  };
} ADI_SYS_CHIPID_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SYS_CHIPID_t__ */

/*@}*/

/** @defgroup SWDEN Serial Wire Debug Enable (SWDEN) Register
 *  Serial Wire Debug Enable (SWDEN) Register.
 *
 *  The SWDEN register is used to enable the Serial Wire Debug (SWD) interface. This register is reset upon an internal power on reset or an external pin reset. This register is not affected by a software reset.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SYS_SWDEN_Struct
 *! \brief  Serial Wire Debug Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SYS_SWDEN_t__
typedef struct _ADI_SYS_SWDEN_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Writing the SWDEN register with "en" (0x6E65) or "EN" (0x4E45) will enable the SWD interface */
    };
    uint16_t VALUE16;
  };
} ADI_SYS_SWDEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SYS_SWDEN_t__ */

/*@}*/

/** @defgroup LOAD Load value (LOAD) Register
 *  Load value (LOAD) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_WDT_LOAD_Struct
 *! \brief  Load value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_LOAD_t__
typedef struct _ADI_WDT_LOAD_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Load value */
    };
    uint16_t VALUE16;
  };
} ADI_WDT_LOAD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_LOAD_t__ */

/*@}*/

/** @defgroup CCNT Current count value (CCNT) Register
 *  Current count value (CCNT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_WDT_CCNT_Struct
 *! \brief  Current count value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_CCNT_t__
typedef struct _ADI_WDT_CCNT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Current count value */
    };
    uint16_t VALUE16;
  };
} ADI_WDT_CCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_CCNT_t__ */

/*@}*/

/** @defgroup CTL Control (CTL) Register
 *  Control (CTL) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_WDT_CTL_PRE
 *! \brief  Prescaler (PRE) Enumerations
 *  ========================================================================= */
typedef enum
{
  WDT_CTL_DIV1   = 0,  /**< source clock/1.             */
  WDT_CTL_DIV16  = 1,  /**< source clock/16.            */
  WDT_CTL_DIV256 = 2   /**< source clock/256 (default). */
} ADI_WDT_CTL_PRE;


/* ==========================================================================
 *! \struct ADI_WDT_CTL_Struct
 *! \brief  Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_CTL_t__
typedef struct _ADI_WDT_CTL_t {
  union {
    struct {
      unsigned int reserved0  :  1;
      unsigned int IRQ        :  1;  /**< Timer interrupt */
      unsigned int PRE        :  2;  /**< Prescaler */
      unsigned int reserved4  :  1;
      unsigned int EN         :  1;  /**< Timer enable */
      unsigned int MODE       :  1;  /**< Timer mode */
      unsigned int SPARE      :  1;  /**< Unused spare bit */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_WDT_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_CTL_t__ */

/*@}*/

/** @defgroup RESTART Clear interrupt (RESTART) Register
 *  Clear interrupt (RESTART) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_WDT_RESTART_Struct
 *! \brief  Clear interrupt Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_RESTART_t__
typedef struct _ADI_WDT_RESTART_t {
  union {
    struct {
      unsigned int CLRWORD    : 16;  /**< Clear watchdog */
    };
    uint16_t VALUE16;
  };
} ADI_WDT_RESTART_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_RESTART_t__ */

/*@}*/

/** @defgroup STAT Status (STAT) Register
 *  Status (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_WDT_STAT_Struct
 *! \brief  Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_STAT_t__
typedef struct _ADI_WDT_STAT_t {
  union {
    struct {
      unsigned int IRQ        :  1;  /**< WDT Interrupt */
      unsigned int CLRIRQ     :  1;  /**< Clear Interrupt Register write sync in progress */
      unsigned int LOADING    :  1;  /**< Load Register write sync in progress */
      unsigned int COUNTING   :  1;  /**< Control Register write sync in progress */
      unsigned int LOCKED     :  1;  /**< Lock status bit */
      unsigned int RSTCTL     :  1;  /**< Reset Control Register written and locked */
      unsigned int reserved6  : 10;
    };
    uint16_t VALUE16;
  };
} ADI_WDT_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_STAT_t__ */

/*@}*/

/** @defgroup MCTL Master Control (MCTL) Register
 *  Master Control (MCTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_MCTL_Struct
 *! \brief  Master Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_MCTL_t__
typedef struct _ADI_I2C_MCTL_t {
  union {
    struct {
      unsigned int MASEN      :  1;  /**< Master enable */
      unsigned int COMPLETE   :  1;  /**< Start back-off disable */
      unsigned int LOOPBACK   :  1;  /**< Internal loopback enable */
      unsigned int STRETCHSCL :  1;  /**< Stretch SCL enable */
      unsigned int IENMRX     :  1;  /**< Receive request interrupt enable */
      unsigned int IENMTX     :  1;  /**< Transmit request interrupt enable */
      unsigned int IENALOST   :  1;  /**< Arbitration lost interrupt enable */
      unsigned int IENACK     :  1;  /**< ACK not received interrupt enable */
      unsigned int IENCMP     :  1;  /**< Transaction completed (or stop detected) interrupt enable */
      unsigned int MXMITDEC   :  1;  /**< Decrement master TX FIFO status when a byte has been transmitted */
      unsigned int MRXDMA     :  1;  /**< Enable master Rx DMA request */
      unsigned int MTXDMA     :  1;  /**< Enable master Tx DMA request */
      unsigned int BUSCLR     :  1;  /**< Bus-Clear Enable */
      unsigned int STOPBUSCLR :  1;  /**< Prestop Bus-Clear */
      unsigned int reserved14 :  2;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MCTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MCTL_t__ */

/*@}*/

/** @defgroup MSTAT Master Status (MSTAT) Register
 *  Master Status (MSTAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_MSTAT_Struct
 *! \brief  Master Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_MSTAT_t__
typedef struct _ADI_I2C_MSTAT_t {
  union {
    struct {
      unsigned int MTXF       :  2;  /**< Master Transmit FIFO status */
      unsigned int MTXREQ     :  1;  /**< This bit will assert when the direction bit is 0 and transmit FIFO is not full */
      unsigned int MRXREQ     :  1;  /**< Master Receive request */
      unsigned int NACKADDR   :  1;  /**< ACK not received in response to an address */
      unsigned int ALOST      :  1;  /**< Arbitration lost */
      unsigned int MBUSY      :  1;  /**< Master busy */
      unsigned int NACKDATA   :  1;  /**< ACK not received in response to data write */
      unsigned int TCOMP      :  1;  /**< Transaction complete or stop detected */
      unsigned int MRXOVR     :  1;  /**< Master Receive FIFO overflow */
      unsigned int LINEBUSY   :  1;  /**< Line is busy */
      unsigned int MSTOP      :  1;  /**< STOP driven by this I2C Master */
      unsigned int MTXUNDR    :  1;  /**< Master Transmit Underflow */
      unsigned int SDAFILT    :  1;  /**< State of SDA Line */
      unsigned int SCLFILT    :  1;  /**< State of SCL Line */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MSTAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MSTAT_t__ */

/*@}*/

/** @defgroup MRX Master Receive Data (MRX) Register
 *  Master Receive Data (MRX) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_MRX_Struct
 *! \brief  Master Receive Data Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_MRX_t__
typedef struct _ADI_I2C_MRX_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Master receive register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MRX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MRX_t__ */

/*@}*/

/** @defgroup MTX Master Transmit Data (MTX) Register
 *  Master Transmit Data (MTX) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_MTX_Struct
 *! \brief  Master Transmit Data Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_MTX_t__
typedef struct _ADI_I2C_MTX_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Master transmit register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MTX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MTX_t__ */

/*@}*/

/** @defgroup MRXCNT Master Receive Data Count (MRXCNT) Register
 *  Master Receive Data Count (MRXCNT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_MRXCNT_Struct
 *! \brief  Master Receive Data Count Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_MRXCNT_t__
typedef struct _ADI_I2C_MRXCNT_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Receive count */
      unsigned int EXTEND     :  1;  /**< Extended read */
      unsigned int reserved9  :  7;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MRXCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MRXCNT_t__ */

/*@}*/

/** @defgroup MCRXCNT Master Current Receive Data Count (MCRXCNT) Register
 *  Master Current Receive Data Count (MCRXCNT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_MCRXCNT_Struct
 *! \brief  Master Current Receive Data Count Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_MCRXCNT_t__
typedef struct _ADI_I2C_MCRXCNT_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Current receive count */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MCRXCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MCRXCNT_t__ */

/*@}*/

/** @defgroup ADDR1 Master Address Byte 1 (ADDR1) Register
 *  Master Address Byte 1 (ADDR1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_ADDR1_Struct
 *! \brief  Master Address Byte 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_ADDR1_t__
typedef struct _ADI_I2C_ADDR1_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Address byte 1 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ADDR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ADDR1_t__ */

/*@}*/

/** @defgroup ADDR2 Master Address Byte 2 (ADDR2) Register
 *  Master Address Byte 2 (ADDR2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_ADDR2_Struct
 *! \brief  Master Address Byte 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_ADDR2_t__
typedef struct _ADI_I2C_ADDR2_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Address byte 2 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ADDR2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ADDR2_t__ */

/*@}*/

/** @defgroup BYT Start Byte (BYT) Register
 *  Start Byte (BYT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_BYT_Struct
 *! \brief  Start Byte Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_BYT_t__
typedef struct _ADI_I2C_BYT_t {
  union {
    struct {
      unsigned int SBYTE      :  8;  /**< Start byte */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_BYT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_BYT_t__ */

/*@}*/

/** @defgroup DIV Serial Clock Period Divisor (DIV) Register
 *  Serial Clock Period Divisor (DIV) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_DIV_Struct
 *! \brief  Serial Clock Period Divisor Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_DIV_t__
typedef struct _ADI_I2C_DIV_t {
  union {
    struct {
      unsigned int LOW        :  8;  /**< Serial clock low time */
      unsigned int HIGH       :  8;  /**< Serial clock high time */
    };
    uint16_t VALUE16;
  };
} ADI_I2C_DIV_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_DIV_t__ */

/*@}*/

/** @defgroup SCTL Slave Control (SCTL) Register
 *  Slave Control (SCTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_SCTL_Struct
 *! \brief  Slave Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_SCTL_t__
typedef struct _ADI_I2C_SCTL_t {
  union {
    struct {
      unsigned int SLVEN      :  1;  /**< Slave enable */
      unsigned int ADR10EN    :  1;  /**< Enabled 10-bit addressing */
      unsigned int GCEN       :  1;  /**< General call enable */
      unsigned int HGCEN      :  1;  /**< Hardware general call enable */
      unsigned int GCSBCLR    :  1;  /**< General call status bit clear */
      unsigned int EARLYTXR   :  1;  /**< Early transmit request mode */
      unsigned int reserved6  :  1;
      unsigned int NACK       :  1;  /**< NACK next communication */
      unsigned int IENSTOP    :  1;  /**< Stop condition detected interrupt enable */
      unsigned int IENSRX     :  1;  /**< Slave Receive request interrupt enable */
      unsigned int IENSTX     :  1;  /**< Slave Transmit request interrupt enable */
      unsigned int STXDEC     :  1;  /**< Decrement Slave Tx FIFO status when a byte has been transmitted */
      unsigned int IENREPST   :  1;  /**< Repeated start interrupt enable */
      unsigned int SRXDMA     :  1;  /**< Enable slave Rx DMA request */
      unsigned int STXDMA     :  1;  /**< Enable slave Tx DMA request */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_SCTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_SCTL_t__ */

/*@}*/

/** @defgroup SSTAT Slave I2C Status/Error/IRQ (SSTAT) Register
 *  Slave I2C Status/Error/IRQ (SSTAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_SSTAT_Struct
 *! \brief  Slave I2C Status/Error/IRQ Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_SSTAT_t__
typedef struct _ADI_I2C_SSTAT_t {
  union {
    struct {
      unsigned int STXFSEREQ  :  1;  /**< Slave Tx FIFO Status or early request */
      unsigned int STXUNDR    :  1;  /**< Slave Transmit FIFO underflow */
      unsigned int STXREQ     :  1;  /**< When read is slave transmit request; when write is clear slave transmit interrupt bit */
      unsigned int SRXREQ     :  1;  /**< Slave Receive request */
      unsigned int SRXOVR     :  1;  /**< Slave Receive FIFO overflow */
      unsigned int NOACK      :  1;  /**< Ack not generated by the slave */
      unsigned int SBUSY      :  1;  /**< Slave busy */
      unsigned int GCINT      :  1;  /**< General call interrupt */
      unsigned int GCID       :  2;  /**< General ID */
      unsigned int STOP       :  1;  /**< Stop after start and matching address */
      unsigned int IDMAT      :  2;  /**< Device ID matched */
      unsigned int REPSTART   :  1;  /**< Repeated start and matching address */
      unsigned int START      :  1;  /**< Start and matching address */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_SSTAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_SSTAT_t__ */

/*@}*/

/** @defgroup SRX Slave Receive (SRX) Register
 *  Slave Receive (SRX) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_SRX_Struct
 *! \brief  Slave Receive Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_SRX_t__
typedef struct _ADI_I2C_SRX_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Slave receive register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_SRX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_SRX_t__ */

/*@}*/

/** @defgroup STX Slave Transmit (STX) Register
 *  Slave Transmit (STX) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_STX_Struct
 *! \brief  Slave Transmit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_STX_t__
typedef struct _ADI_I2C_STX_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Slave transmit register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_STX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_STX_t__ */

/*@}*/

/** @defgroup ALT Hardware General Call ID (ALT) Register
 *  Hardware General Call ID (ALT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_ALT_Struct
 *! \brief  Hardware General Call ID Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_ALT_t__
typedef struct _ADI_I2C_ALT_t {
  union {
    struct {
      unsigned int ID         :  8;  /**< Slave Alt */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ALT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ALT_t__ */

/*@}*/

/** @defgroup ID0 First Slave Address Device ID (ID0) Register
 *  First Slave Address Device ID (ID0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_ID0_Struct
 *! \brief  First Slave Address Device ID Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_ID0_t__
typedef struct _ADI_I2C_ID0_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Slave device ID 0 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ID0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ID0_t__ */

/*@}*/

/** @defgroup ID1 Second Slave Address Device ID (ID1) Register
 *  Second Slave Address Device ID (ID1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_ID1_Struct
 *! \brief  Second Slave Address Device ID Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_ID1_t__
typedef struct _ADI_I2C_ID1_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Slave device ID 1 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ID1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ID1_t__ */

/*@}*/

/** @defgroup ID2 Third Slave Address Device ID (ID2) Register
 *  Third Slave Address Device ID (ID2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_ID2_Struct
 *! \brief  Third Slave Address Device ID Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_ID2_t__
typedef struct _ADI_I2C_ID2_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Slave device ID 2 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ID2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ID2_t__ */

/*@}*/

/** @defgroup ID3 Fourth Slave Address Device ID (ID3) Register
 *  Fourth Slave Address Device ID (ID3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_ID3_Struct
 *! \brief  Fourth Slave Address Device ID Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_ID3_t__
typedef struct _ADI_I2C_ID3_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Slave device ID 3 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ID3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ID3_t__ */

/*@}*/

/** @defgroup STAT Master and Slave FIFO Status (STAT) Register
 *  Master and Slave FIFO Status (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_STAT_Struct
 *! \brief  Master and Slave FIFO Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_STAT_t__
typedef struct _ADI_I2C_STAT_t {
  union {
    struct {
      unsigned int STXF       :  2;  /**< Slave transmit FIFO status */
      unsigned int SRXF       :  2;  /**< Slave receive FIFO status */
      unsigned int MTXF       :  2;  /**< Master transmit FIFO status */
      unsigned int MRXF       :  2;  /**< Master receive FIFO status */
      unsigned int SFLUSH     :  1;  /**< Flush the slave transmit FIFO */
      unsigned int MFLUSH     :  1;  /**< Flush the master transmit FIFO */
      unsigned int reserved10 :  6;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_STAT_t__ */

/*@}*/

/** @defgroup SHCTL Shared Control (SHCTL) Register
 *  Shared Control (SHCTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_SHCTL_Struct
 *! \brief  Shared Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_SHCTL_t__
typedef struct _ADI_I2C_SHCTL_t {
  union {
    struct {
      unsigned int RST        :  1;  /**< Reset START STOP detect circuit */
      unsigned int reserved1  : 15;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_SHCTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_SHCTL_t__ */

/*@}*/

/** @defgroup TCTL Timing Control Register (TCTL) Register
 *  Timing Control Register (TCTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_TCTL_Struct
 *! \brief  Timing Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_TCTL_t__
typedef struct _ADI_I2C_TCTL_t {
  union {
    struct {
      unsigned int THDATIN    :  5;  /**< Data In Hold Start */
      unsigned int reserved5  :  3;
      unsigned int FILTEROFF  :  1;  /**< Input Filter Control */
      unsigned int reserved9  :  7;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_TCTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_TCTL_t__ */

/*@}*/

/** @defgroup ASTRETCH_SCL Automatic Stretch SCL (ASTRETCH_SCL) Register
 *  Automatic Stretch SCL (ASTRETCH_SCL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_I2C_ASTRETCH_SCL_Struct
 *! \brief  Automatic Stretch SCL Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_ASTRETCH_SCL_t__
typedef struct _ADI_I2C_ASTRETCH_SCL_t {
  union {
    struct {
      unsigned int MST        :  4;  /**< Master automatic stretch mode */
      unsigned int SLV        :  4;  /**< Slave automatic stretch mode */
      unsigned int MSTTMO     :  1;  /**< Master automatic stretch timeout */
      unsigned int SLVTMO     :  1;  /**< Slave automatic stretch timeout */
      unsigned int reserved10 :  6;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ASTRETCH_SCL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ASTRETCH_SCL_t__ */

/*@}*/

/** @defgroup STAT Status (STAT) Register
 *  Status (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_STAT_Struct
 *! \brief  Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_STAT_t__
typedef struct _ADI_SPI_STAT_t {
  union {
    struct {
      unsigned int IRQ        :  1;  /**< SPI Interrupt status */
      unsigned int XFRDONE    :  1;  /**< SPI transfer completion */
      unsigned int TXEMPTY    :  1;  /**< SPI Tx FIFO empty interrupt */
      unsigned int TXDONE     :  1;  /**< SPI Tx Done in read command mode */
      unsigned int TXUNDR     :  1;  /**< SPI Tx FIFO underflow */
      unsigned int TXIRQ      :  1;  /**< SPI Tx IRQ */
      unsigned int RXIRQ      :  1;  /**< SPI Rx IRQ */
      unsigned int RXOVR      :  1;  /**< SPI Rx FIFO overflow */
      unsigned int reserved8  :  3;
      unsigned int CS         :  1;  /**< CS Status */
      unsigned int CSERR      :  1;  /**< Detected a CS error condition in slave mode */
      unsigned int CSFALL     :  1;  /**< Detected a falling edge on CS, in slave CON mode */
      unsigned int CSRISE     :  1;  /**< Detected a rising edge on CS, in slave CON mode */
      unsigned int RDY        :  1;  /**< Detected an edge on Ready indicator for flow-control */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_STAT_t__ */

/*@}*/

/** @defgroup RX Receive (RX) Register
 *  Receive (RX) Register.
 *
 *  This register allows access to the 8-deep receive FIFO.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_RX_Struct
 *! \brief  Receive Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_RX_t__
typedef struct _ADI_SPI_RX_t {
  union {
    struct {
      unsigned int BYTE1      :  8;  /**< 8-bit receive buffer */
      unsigned int BYTE2      :  8;  /**< 8-bit receive buffer, used only in DMA modes */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_RX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_RX_t__ */

/*@}*/

/** @defgroup TX Transmit (TX) Register
 *  Transmit (TX) Register.
 *
 *  This register allows access to the 8-deep transmit FIFO.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_TX_Struct
 *! \brief  Transmit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_TX_t__
typedef struct _ADI_SPI_TX_t {
  union {
    struct {
      unsigned int BYTE1      :  8;  /**< 8-bit transmit buffer */
      unsigned int BYTE2      :  8;  /**< 8-bit transmit buffer, used only in DMA modes */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_TX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_TX_t__ */

/*@}*/

/** @defgroup DIV SPI baud rate selection (DIV) Register
 *  SPI baud rate selection (DIV) Register.
 *
 *  This register is only used in master mode.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_DIV_Struct
 *! \brief  SPI baud rate selection Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_DIV_t__
typedef struct _ADI_SPI_DIV_t {
  union {
    struct {
      unsigned int VALUE      :  6;  /**< SPI clock divider */
      unsigned int reserved6  : 10;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_DIV_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_DIV_t__ */

/*@}*/

/** @defgroup CTL SPI configuration 1 (CTL) Register
 *  SPI configuration 1 (CTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_CTL_Struct
 *! \brief  SPI configuration 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_CTL_t__
typedef struct _ADI_SPI_CTL_t {
  union {
    struct {
      unsigned int SPIEN      :  1;  /**< SPI enable */
      unsigned int MASEN      :  1;  /**< Master mode enable */
      unsigned int CPHA       :  1;  /**< Serial clock phase mode */
      unsigned int CPOL       :  1;  /**< Serial Clock Polarity */
      unsigned int WOM        :  1;  /**< SPI Wired Or mode */
      unsigned int LSB        :  1;  /**< LSB first transfer enable */
      unsigned int TIM        :  1;  /**< SPI transfer and interrupt mode */
      unsigned int ZEN        :  1;  /**< Transmit zeros enable */
      unsigned int RXOF       :  1;  /**< RX overflow overwrite enable */
      unsigned int OEN        :  1;  /**< Slave MISO output enable */
      unsigned int LOOPBACK   :  1;  /**< Loopback enable */
      unsigned int CON        :  1;  /**< Continuous transfer enable */
      unsigned int RFLUSH     :  1;  /**< SPI Rx FIFO Flush enable */
      unsigned int TFLUSH     :  1;  /**< SPI Tx FIFO Flush enable */
      unsigned int CSRST      :  1;  /**< Reset Mode for CS Error bit */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_CTL_t__ */

/*@}*/

/** @defgroup IEN SPI configuration 2 (IEN) Register
 *  SPI configuration 2 (IEN) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_IEN_Struct
 *! \brief  SPI configuration 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_IEN_t__
typedef struct _ADI_SPI_IEN_t {
  union {
    struct {
      unsigned int IRQMODE    :  3;  /**< SPI IRQ mode bits */
      unsigned int reserved3  :  5;
      unsigned int CS         :  1;  /**< Enable interrupt on every CS edge in slave CON mode */
      unsigned int TXUNDR     :  1;  /**< Tx-underflow interrupt enable */
      unsigned int RXOVR      :  1;  /**< Rx-overflow interrupt enable */
      unsigned int RDY        :  1;  /**< Ready signal edge interrupt enable */
      unsigned int TXDONE     :  1;  /**< SPI transmit done interrupt enable */
      unsigned int XFRDONE    :  1;  /**< SPI transfer completion interrupt enable */
      unsigned int TXEMPTY    :  1;  /**< Tx-FIFO Empty interrupt enable */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_IEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_IEN_t__ */

/*@}*/

/** @defgroup CNT Transfer byte count (CNT) Register
 *  Transfer byte count (CNT) Register.
 *
 *  This register is only used in master mode.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_CNT_Struct
 *! \brief  Transfer byte count Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_CNT_t__
typedef struct _ADI_SPI_CNT_t {
  union {
    struct {
      unsigned int VALUE      : 14;  /**< Transfer byte count */
      unsigned int reserved14 :  1;
      unsigned int FRAMECONT  :  1;  /**< Continue frame */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_CNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_CNT_t__ */

/*@}*/

/** @defgroup DMA SPI DMA enable (DMA) Register
 *  SPI DMA enable (DMA) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_DMA_Struct
 *! \brief  SPI DMA enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_DMA_t__
typedef struct _ADI_SPI_DMA_t {
  union {
    struct {
      unsigned int EN         :  1;  /**< Enable DMA for data transfer */
      unsigned int TXEN       :  1;  /**< Enable transmit DMA request */
      unsigned int RXEN       :  1;  /**< Enable receive DMA request */
      unsigned int reserved3  : 13;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_DMA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_DMA_t__ */

/*@}*/

/** @defgroup FIFO_STAT FIFO Status (FIFO_STAT) Register
 *  FIFO Status (FIFO_STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_FIFO_STAT_Struct
 *! \brief  FIFO Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_FIFO_STAT_t__
typedef struct _ADI_SPI_FIFO_STAT_t {
  union {
    struct {
      unsigned int TX         :  4;  /**< SPI Tx FIFO status */
      unsigned int reserved4  :  4;
      unsigned int RX         :  4;  /**< SPI Rx FIFO status */
      unsigned int reserved12 :  4;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_FIFO_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_FIFO_STAT_t__ */

/*@}*/

/** @defgroup RD_CTL Read Control (RD_CTL) Register
 *  Read Control (RD_CTL) Register.
 *
 *  This register is only used in master mode.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_RD_CTL_Struct
 *! \brief  Read Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_RD_CTL_t__
typedef struct _ADI_SPI_RD_CTL_t {
  union {
    struct {
      unsigned int CMDEN      :  1;  /**< Read command enable */
      unsigned int OVERLAP    :  1;  /**< Tx/Rx Overlap mode */
      unsigned int TXBYTES    :  4;  /**< Transmit byte count minus 1 for read command */
      unsigned int reserved6  :  2;
      unsigned int THREEPIN   :  1;  /**< Three pin SPI mode */
      unsigned int reserved9  :  7;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_RD_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_RD_CTL_t__ */

/*@}*/

/** @defgroup FLOW_CTL Flow Control (FLOW_CTL) Register
 *  Flow Control (FLOW_CTL) Register.
 *
 *  This register is only used in master mode.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_FLOW_CTL_Struct
 *! \brief  Flow Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_FLOW_CTL_t__
typedef struct _ADI_SPI_FLOW_CTL_t {
  union {
    struct {
      unsigned int MODE       :  2;  /**< Flow control mode */
      unsigned int reserved2  :  2;
      unsigned int RDYPOL     :  1;  /**< Polarity of RDY/MISO line */
      unsigned int reserved5  :  1;
      unsigned int RDBURSTSZ  : 10;  /**< Read data burst size minus 1 */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_FLOW_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_FLOW_CTL_t__ */

/*@}*/

/** @defgroup WAIT_TMR Wait timer for flow control (WAIT_TMR) Register
 *  Wait timer for flow control (WAIT_TMR) Register.
 *
 *  This register is only used in master mode.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_WAIT_TMR_Struct
 *! \brief  Wait timer for flow control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_WAIT_TMR_t__
typedef struct _ADI_SPI_WAIT_TMR_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Wait timer for flow-control */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_WAIT_TMR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_WAIT_TMR_t__ */

/*@}*/

/** @defgroup CS_CTL Chip-Select control for multi-slave connections (CS_CTL) Register
 *  Chip-Select control for multi-slave connections (CS_CTL) Register.
 *
 *  This register is only used in master mode.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_CS_CTL_Struct
 *! \brief  Chip-Select control for multi-slave connections Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_CS_CTL_t__
typedef struct _ADI_SPI_CS_CTL_t {
  union {
    struct {
      unsigned int SEL        :  4;  /**< Chip-Select control */
      unsigned int reserved4  : 12;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_CS_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_CS_CTL_t__ */

/*@}*/

/** @defgroup CS_OVERRIDE Chip-Select Override (CS_OVERRIDE) Register
 *  Chip-Select Override (CS_OVERRIDE) Register.
 *
 *  This register is only used in master mode.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_CS_OVERRIDE_Struct
 *! \brief  Chip-Select Override Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_CS_OVERRIDE_t__
typedef struct _ADI_SPI_CS_OVERRIDE_t {
  union {
    struct {
      unsigned int CTL        :  2;  /**< CS Override Control */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_CS_OVERRIDE_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_CS_OVERRIDE_t__ */

/*@}*/

/** @defgroup RX Receive Buffer Register (RX) Register
 *  Receive Buffer Register (RX) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_RX_Struct
 *! \brief  Receive Buffer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_RX_t__
typedef struct _ADI_UART_RX_t {
  union {
    struct {
      unsigned int RBR        :  8;  /**< Receive Buffer Register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_RX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_RX_t__ */

/*@}*/

/** @defgroup TX Transmit Holding Register (TX) Register
 *  Transmit Holding Register (TX) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_TX_Struct
 *! \brief  Transmit Holding Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_TX_t__
typedef struct _ADI_UART_TX_t {
  union {
    struct {
      unsigned int THR        :  8;  /**< Transmit Holding Register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_TX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_TX_t__ */

/*@}*/

/** @defgroup IEN Interrupt Enable (IEN) Register
 *  Interrupt Enable (IEN) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_IEN_Struct
 *! \brief  Interrupt Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_IEN_t__
typedef struct _ADI_UART_IEN_t {
  union {
    struct {
      unsigned int ERBFI      :  1;  /**< Receive buffer full interrupt */
      unsigned int ETBEI      :  1;  /**< Transmit buffer empty interrupt */
      unsigned int ELSI       :  1;  /**< Rx status interrupt */
      unsigned int EDSSI      :  1;  /**< Modem status interrupt */
      unsigned int EDMAT      :  1;  /**< DMA requests in transmit mode */
      unsigned int EDMAR      :  1;  /**< DMA requests in receive mode */
      unsigned int reserved6  : 10;
    };
    uint16_t VALUE16;
  };
} ADI_UART_IEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_IEN_t__ */

/*@}*/

/** @defgroup IIR Interrupt ID (IIR) Register
 *  Interrupt ID (IIR) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_UART_IIR_STAT
 *! \brief  Interrupt status (STAT) Enumerations
 *  ========================================================================= */
typedef enum
{
  UART_IIR_STAT_EDSSI = 0,  /**< Modem status interrupt (Read MSR register to clear)                                  */
  UART_IIR_STAT_ETBEI = 1,  /**< Transmit buffer empty interrupt (Write to Tx register or read IIR register to clear) */
  UART_IIR_STAT_ERBFI = 2,  /**< Receive buffer full interrupt (Read Rx register to clear)                            */
  UART_IIR_STAT_RLSI  = 3,  /**< Receive line status interrupt (Read LSR register to clear)                           */
  UART_IIR_STAT_RFTOI = 6   /**< Receive FIFO time-out interrupt (Read Rx register to clear)                          */
} ADI_UART_IIR_STAT;


/* ==========================================================================
 *! \struct ADI_UART_IIR_Struct
 *! \brief  Interrupt ID Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_IIR_t__
typedef struct _ADI_UART_IIR_t {
  union {
    struct {
      unsigned int NIRQ       :  1;  /**< Interrupt flag */
      unsigned int STAT       :  3;  /**< Interrupt status */
      unsigned int reserved4  :  2;
      unsigned int FEND       :  2;  /**< FIFO enabled */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_IIR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_IIR_t__ */

/*@}*/

/** @defgroup LCR Line Control (LCR) Register
 *  Line Control (LCR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_LCR_Struct
 *! \brief  Line Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_LCR_t__
typedef struct _ADI_UART_LCR_t {
  union {
    struct {
      unsigned int WLS        :  2;  /**< Word Length Select */
      unsigned int STOP       :  1;  /**< Stop Bit */
      unsigned int PEN        :  1;  /**< Parity Enable */
      unsigned int EPS        :  1;  /**< Parity Select */
      unsigned int SP         :  1;  /**< Stick Parity */
      unsigned int BRK        :  1;  /**< Set Break */
      unsigned int reserved7  :  9;
    };
    uint16_t VALUE16;
  };
} ADI_UART_LCR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_LCR_t__ */

/*@}*/

/** @defgroup MCR Modem Control (MCR) Register
 *  Modem Control (MCR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_MCR_Struct
 *! \brief  Modem Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_MCR_t__
typedef struct _ADI_UART_MCR_t {
  union {
    struct {
      unsigned int DTR        :  1;  /**< Data Terminal Ready */
      unsigned int RTS        :  1;  /**< Request to send */
      unsigned int OUT1       :  1;  /**< Output 1 */
      unsigned int OUT2       :  1;  /**< Output 2 */
      unsigned int LOOPBACK   :  1;  /**< Loopback mode */
      unsigned int reserved5  : 11;
    };
    uint16_t VALUE16;
  };
} ADI_UART_MCR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_MCR_t__ */

/*@}*/

/** @defgroup LSR Line Status (LSR) Register
 *  Line Status (LSR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_LSR_Struct
 *! \brief  Line Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_LSR_t__
typedef struct _ADI_UART_LSR_t {
  union {
    struct {
      unsigned int DR         :  1;  /**< Data Ready */
      unsigned int OE         :  1;  /**< Overrun Error */
      unsigned int PE         :  1;  /**< Parity Error */
      unsigned int FE         :  1;  /**< Framing Error */
      unsigned int BI         :  1;  /**< Break Indicator */
      unsigned int THRE       :  1;  /**< Transmit Register Empty */
      unsigned int TEMT       :  1;  /**< Transmit and Shift Register Empty Status */
      unsigned int FIFOERR    :  1;  /**< data byte(s) in RX FIFO have either parity error, frame error or break indication. only used in 16550 mode; Read-clear if no more error in RX FIFO */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_LSR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_LSR_t__ */

/*@}*/

/** @defgroup MSR Modem Status (MSR) Register
 *  Modem Status (MSR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_MSR_Struct
 *! \brief  Modem Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_MSR_t__
typedef struct _ADI_UART_MSR_t {
  union {
    struct {
      unsigned int DCTS       :  1;  /**< Delta CTS */
      unsigned int DDSR       :  1;  /**< Delta DSR */
      unsigned int TERI       :  1;  /**< Trailing Edge RI */
      unsigned int DDCD       :  1;  /**< Delta DCD */
      unsigned int CTS        :  1;  /**< Clear To Send */
      unsigned int DSR        :  1;  /**< Data Set Ready */
      unsigned int RI         :  1;  /**< Ring Indicator */
      unsigned int DCD        :  1;  /**< Data Carrier Detect */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_MSR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_MSR_t__ */

/*@}*/

/** @defgroup SCR Scratch Buffer (SCR) Register
 *  Scratch Buffer (SCR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_SCR_Struct
 *! \brief  Scratch Buffer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_SCR_t__
typedef struct _ADI_UART_SCR_t {
  union {
    struct {
      unsigned int SCR        :  8;  /**< Scratch */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_SCR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_SCR_t__ */

/*@}*/

/** @defgroup FCR FIFO Control (FCR) Register
 *  FIFO Control (FCR) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_UART_FCR_FDMAMD
 *! \brief  FIFO DMA mode (FDMAMD) Enumerations
 *  ========================================================================= */
typedef enum
{
  UART_FCR_MODE0 = 0,  /**< In DMA mode 0, RX DMA request will be asserted whenever there's data in RBR or RX FIFO and de-assert whenever RBR or RX FIFO is empty; TX DMA request will be asserted whenever THR or TX FIFO is empty and de-assert whenever data written to.                              */
  UART_FCR_MODE1 = 1   /**< in DMA mode 1, RX DMA request will be asserted whenever RX FIFO trig level or time out reached and de-assert thereafter when RX FIFO is empty; TX DMA request will be asserted whenever TX FIFO is empty and de-assert thereafter when TX FIFO is completely filled up full. */
} ADI_UART_FCR_FDMAMD;


/* ==========================================================================
 *! \struct ADI_UART_FCR_Struct
 *! \brief  FIFO Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_FCR_t__
typedef struct _ADI_UART_FCR_t {
  union {
    struct {
      unsigned int FIFOEN     :  1;  /**< FIFO enable as to work in 16550 mode */
      unsigned int RFCLR      :  1;  /**< clear RX FIFO */
      unsigned int TFCLR      :  1;  /**< clear TX FIFO */
      unsigned int FDMAMD     :  1;  /**< FIFO DMA mode */
      unsigned int reserved4  :  2;
      unsigned int RFTRIG     :  2;  /**< RX FIFO Trig level */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_FCR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_FCR_t__ */

/*@}*/

/** @defgroup FBR Fractional Baud Rate (FBR) Register
 *  Fractional Baud Rate (FBR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_FBR_Struct
 *! \brief  Fractional Baud Rate Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_FBR_t__
typedef struct _ADI_UART_FBR_t {
  union {
    struct {
      unsigned int DIVN       : 11;  /**< Fractional baud rate N divide bits 0 to 2047 */
      unsigned int DIVM       :  2;  /**< Fractional baud rate M divide bits 1 to 3 */
      unsigned int reserved13 :  2;
      unsigned int FBEN       :  1;  /**< Fractional baud rate generator enable */
    };
    uint16_t VALUE16;
  };
} ADI_UART_FBR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_FBR_t__ */

/*@}*/

/** @defgroup DIV Baudrate Divider (DIV) Register
 *  Baudrate Divider (DIV) Register.
 *
 *  Internal UART baud generation counters are restarted whenever COMDIV register accessed by writing, regardless same or different value.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_DIV_Struct
 *! \brief  Baudrate Divider Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_DIV_t__
typedef struct _ADI_UART_DIV_t {
  union {
    struct {
      unsigned int DIV        : 16;  /**< Baud rate divider */
    };
    uint16_t VALUE16;
  };
} ADI_UART_DIV_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_DIV_t__ */

/*@}*/

/** @defgroup LCR2 Second Line Control (LCR2) Register
 *  Second Line Control (LCR2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_LCR2_Struct
 *! \brief  Second Line Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_LCR2_t__
typedef struct _ADI_UART_LCR2_t {
  union {
    struct {
      unsigned int OSR        :  2;  /**< Over Sample Rate */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_UART_LCR2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_LCR2_t__ */

/*@}*/

/** @defgroup CTL UART Control Register (CTL) Register
 *  UART Control Register (CTL) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_UART_CTL_RXINV
 *! \brief  invert receiver line (RXINV) Enumerations
 *  ========================================================================= */
typedef enum
{
  UART_CTL_EN000 = 0,  /**< don't invert receiver line (idling high) */
  UART_CTL_EN001 = 1   /**< invert receiver line (idling low)        */
} ADI_UART_CTL_RXINV;


/* ==========================================================================
 *! \struct ADI_UART_CTL_Struct
 *! \brief  UART Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_CTL_t__
typedef struct _ADI_UART_CTL_t {
  union {
    struct {
      unsigned int reserved0  :  1;
      unsigned int FORCECLK   :  1;  /**< Force UCLK on */
      unsigned int reserved2  :  2;
      unsigned int RXINV      :  1;  /**< invert receiver line */
      unsigned int reserved5  :  3;
      unsigned int REV        :  8;  /**< UART revision ID */
    };
    uint16_t VALUE16;
  };
} ADI_UART_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_CTL_t__ */

/*@}*/

/** @defgroup RFC RX FIFO Byte Count (RFC) Register
 *  RX FIFO Byte Count (RFC) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_RFC_Struct
 *! \brief  RX FIFO Byte Count Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_RFC_t__
typedef struct _ADI_UART_RFC_t {
  union {
    struct {
      unsigned int RFC        :  5;  /**< Current RX FIFO data bytes */
      unsigned int reserved5  : 11;
    };
    uint16_t VALUE16;
  };
} ADI_UART_RFC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_RFC_t__ */

/*@}*/

/** @defgroup TFC TX FIFO Byte Count (TFC) Register
 *  TX FIFO Byte Count (TFC) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_TFC_Struct
 *! \brief  TX FIFO Byte Count Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_TFC_t__
typedef struct _ADI_UART_TFC_t {
  union {
    struct {
      unsigned int TFC        :  5;  /**< Current TX FIFO data bytes */
      unsigned int reserved5  : 11;
    };
    uint16_t VALUE16;
  };
} ADI_UART_TFC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_TFC_t__ */

/*@}*/

/** @defgroup RSC RS485 Half-duplex Control (RSC) Register
 *  RS485 Half-duplex Control (RSC) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_RSC_Struct
 *! \brief  RS485 Half-duplex Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_RSC_t__
typedef struct _ADI_UART_RSC_t {
  union {
    struct {
      unsigned int OENP       :  1;  /**< SOUT_EN polarity */
      unsigned int OENSP      :  1;  /**< SOUT_EN de-assert before full stop bit(s) */
      unsigned int DISRX      :  1;  /**< disable RX when transmitting */
      unsigned int DISTX      :  1;  /**< Hold off TX when receiving */
      unsigned int reserved4  : 12;
    };
    uint16_t VALUE16;
  };
} ADI_UART_RSC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_RSC_t__ */

/*@}*/

/** @defgroup ACR Auto Baud Control (ACR) Register
 *  Auto Baud Control (ACR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_ACR_Struct
 *! \brief  Auto Baud Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_ACR_t__
typedef struct _ADI_UART_ACR_t {
  union {
    struct {
      unsigned int ABE        :  1;  /**< Auto Baud Enable */
      unsigned int DNIEN      :  1;  /**< enable done interrupt */
      unsigned int TOIEN      :  1;  /**< enable time-out interrupt */
      unsigned int reserved3  :  1;
      unsigned int SEC        :  3;  /**< Starting Edge Count */
      unsigned int reserved7  :  1;
      unsigned int EEC        :  4;  /**< Ending Edge Count */
      unsigned int reserved12 :  4;
    };
    uint16_t VALUE16;
  };
} ADI_UART_ACR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_ACR_t__ */

/*@}*/

/** @defgroup ASRL Auto Baud Status (Low) (ASRL) Register
 *  Auto Baud Status (Low) (ASRL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_ASRL_Struct
 *! \brief  Auto Baud Status (Low) Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_ASRL_t__
typedef struct _ADI_UART_ASRL_t {
  union {
    struct {
      unsigned int DONE       :  1;  /**< Auto Baud Done successfully */
      unsigned int BRKTO      :  1;  /**< Timed out due to long break condition */
      unsigned int NSETO      :  1;  /**< Timed out due to no valid start edge found */
      unsigned int NEETO      :  1;  /**< Timed out due to no valid ending edge found */
      unsigned int CNT        : 12;  /**< CNT[11:0] Auto Baud Counter value */
    };
    uint16_t VALUE16;
  };
} ADI_UART_ASRL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_ASRL_t__ */

/*@}*/

/** @defgroup ASRH Auto Baud Status (High) (ASRH) Register
 *  Auto Baud Status (High) (ASRH) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_ASRH_Struct
 *! \brief  Auto Baud Status (High) Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_ASRH_t__
typedef struct _ADI_UART_ASRH_t {
  union {
    struct {
      unsigned int CNT        :  8;  /**< CNT[19:12] Auto Baud Counter value */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_ASRH_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_ASRH_t__ */

/*@}*/

/** @defgroup CFG Beeper configuration (CFG) Register
 *  Beeper configuration (CFG) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BEEP_CFG_Struct
 *! \brief  Beeper configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BEEP_CFG_t__
typedef struct _ADI_BEEP_CFG_t {
  union {
    struct {
      unsigned int SEQREPEAT  :  8;  /**< Beeper Sequence Repeat value */
      unsigned int EN         :  1;  /**< Beeper Enable */
      unsigned int reserved9  :  1;
      unsigned int ASTARTIRQ  :  1;  /**< Tone A start IRQ */
      unsigned int AENDIRQ    :  1;  /**< Tone A end IRQ */
      unsigned int BSTARTIRQ  :  1;  /**< Tone B start IRQ */
      unsigned int BENDIRQ    :  1;  /**< Tone B end IRQ */
      unsigned int SEQNEARENDIRQ :  1;  /**< Sequence 1 cycle from end IRQ */
      unsigned int SEQATENDIRQ   :  1;  /**< Sequence end IRQ */
    };
    uint16_t VALUE16;
  };
} ADI_BEEP_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BEEP_CFG_t__ */

/*@}*/

/** @defgroup STAT Beeper status (STAT) Register
 *  Beeper status (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BEEP_STAT_Struct
 *! \brief  Beeper status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BEEP_STAT_t__
typedef struct _ADI_BEEP_STAT_t {
  union {
    struct {
      unsigned int SEQREMAIN  :  8;  /**< Remaining tone-pair sequence iterations to play in SEQ mode */
      unsigned int BUSY       :  1;  /**< Beeper is busy */
      unsigned int reserved9  :  1;
      unsigned int ASTARTED   :  1;  /**< Tone A has started */
      unsigned int AENDED     :  1;  /**< Tone A has ended */
      unsigned int BSTARTED   :  1;  /**< Tone B has started */
      unsigned int BENDED     :  1;  /**< Tone B has ended */
      unsigned int SEQNEAREND :  1;  /**< Sequencer last tone-pair has started */
      unsigned int SEQENDED   :  1;  /**< Sequencer has ended */
    };
    uint16_t VALUE16;
  };
} ADI_BEEP_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BEEP_STAT_t__ */

/*@}*/

/** @defgroup TONEA Tone A Data (TONEA) Register
 *  Tone A Data (TONEA) Register.
 *
 *  Tone A is the first tone to play in Sequence Mode, and the only tone to play in Pulse Mode. Writing 0x0 to the DUR field while Tone A is playing will immediately terminate the tone. All other writes to TONE_A affect the next play back of the tone only and do not affect the currently playing tone.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BEEP_TONEA_Struct
 *! \brief  Tone A Data Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BEEP_TONEA_t__
typedef struct _ADI_BEEP_TONEA_t {
  union {
    struct {
      unsigned int DUR        :  8;  /**< Tone duration */
      unsigned int FREQ       :  7;  /**< Tone frequency */
      unsigned int DIS        :  1;  /**< Output disable */
    };
    uint16_t VALUE16;
  };
} ADI_BEEP_TONEA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BEEP_TONEA_t__ */

/*@}*/

/** @defgroup TONEB Tone B Data (TONEB) Register
 *  Tone B Data (TONEB) Register.
 *
 *  Tone B is the second tone to play in Sequence Mode, and is not played Pulse Mode. Writing 0x0 to the DUR field while Tone B is playing will immediately terminate the tone. All other writes to TONE_B affect the next play back of the tone only and do not affect the currently playing tone.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BEEP_TONEB_Struct
 *! \brief  Tone B Data Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BEEP_TONEB_t__
typedef struct _ADI_BEEP_TONEB_t {
  union {
    struct {
      unsigned int DUR        :  8;  /**< Tone duration */
      unsigned int FREQ       :  7;  /**< Tone frequency */
      unsigned int DIS        :  1;  /**< Output disable */
    };
    uint16_t VALUE16;
  };
} ADI_BEEP_TONEB_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BEEP_TONEB_t__ */

/*@}*/

/** @defgroup CFG ADC Configuration (CFG) Register
 *  ADC Configuration (CFG) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_ADC_CFG_VREFSEL
 *! \brief  To select Vref as 1.25V or 2.5V (VREFSEL) Enumerations
 *  ========================================================================= */
typedef enum
{
  ADC_CFG_V_2P5  = 0,  /**< Vref = 2.5V  */
  ADC_CFG_V_1P25 = 1   /**< Vref = 1.25V */
} ADI_ADC_CFG_VREFSEL;


/*  =========================================================================
 *! \enum   ADI_ADC_CFG_REFBUFEN
 *! \brief  To enable internal reference buffer (REFBUFEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  ADC_CFG_EN000 = 0,  /**< External reference is used  */
  ADC_CFG_EN001 = 1   /**< Reference buffer is enabled */
} ADI_ADC_CFG_REFBUFEN;


/* ==========================================================================
 *! \struct ADI_ADC_CFG_Struct
 *! \brief  ADC Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CFG_t__
typedef struct _ADI_ADC_CFG_t {
  union {
    struct {
      unsigned int PWRUP      :  1;  /**< Powering up ADC */
      unsigned int VREFSEL    :  1;  /**< To select Vref as 1.25V or 2.5V */
      unsigned int REFBUFEN   :  1;  /**< To enable internal reference buffer */
      unsigned int VREFVBAT   :  1;  /**< VRef VBAT */
      unsigned int EN         :  1;  /**< To enable ADC subsystem */
      unsigned int STARTCAL   :  1;  /**< To start a new offset calibration cycle */
      unsigned int RST        :  1;  /**< Resets internal buffers and registers when high */
      unsigned int SINKEN     :  1;  /**< To enable additional 50uA sink current capability @1.25V, 100uA current capability @2.5V. */
      unsigned int TMPEN      :  1;  /**< To power up temperature sensor */
      unsigned int FAST_DISCH :  1;  /**< For fast switchover of vref from 2.5 to 1.25 */
      unsigned int VREFVBAT_DEL :  1;  /**< VREFBAT_DEL field must be set to 1 after minimum delay of 700us from VREFBAT field being set to 1. Note: VREFBAT_DEL and VREFBAT must be reset to 0 together. */
      unsigned int reserved11   :  5;
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CFG_t__ */

/*@}*/

/** @defgroup PWRUP ADC Power-up Time (PWRUP) Register
 *  ADC Power-up Time (PWRUP) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_PWRUP_Struct
 *! \brief  ADC Power-up Time Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_PWRUP_t__
typedef struct _ADI_ADC_PWRUP_t {
  union {
    struct {
      unsigned int WAIT       : 11;  /**< Program this count to generate 20us wait time with respect to the PCLK frequency */
      unsigned int reserved11 :  5;
    };
    uint16_t VALUE16;
  };
} ADI_ADC_PWRUP_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_PWRUP_t__ */

/*@}*/

/** @defgroup CAL_WORD Calibration Word (CAL_WORD) Register
 *  Calibration Word (CAL_WORD) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CAL_WORD_Struct
 *! \brief  Calibration Word Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CAL_WORD_t__
typedef struct _ADI_ADC_CAL_WORD_t {
  union {
    struct {
      unsigned int VALUE      :  7;  /**< Offset calibration word */
      unsigned int reserved7  :  9;
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CAL_WORD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CAL_WORD_t__ */

/*@}*/

/** @defgroup CNV_CFG ADC Conversion Configuration (CNV_CFG) Register
 *  ADC Conversion Configuration (CNV_CFG) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CNV_CFG_Struct
 *! \brief  ADC Conversion Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CNV_CFG_t__
typedef struct _ADI_ADC_CNV_CFG_t {
  union {
    struct {
      unsigned int SEL        :  8;  /**< To select channel(s) to convert */
      unsigned int BAT        :  1;  /**< To enable battery monitoring */
      unsigned int TMP        :  1;  /**< To select temperature measurement 1 */
      unsigned int TMP2       :  1;  /**< To select temperature measurement 2 */
      unsigned int reserved11 :  1;
      unsigned int AUTOMODE   :  1;  /**< To enable auto mode */
      unsigned int DMAEN      :  1;  /**< To enable DMA channel */
      unsigned int SINGLE     :  1;  /**< Set to start single conversion */
      unsigned int MULTI      :  1;  /**< Set to start multiple conversions */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CNV_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CNV_CFG_t__ */

/*@}*/

/** @defgroup CNV_TIME ADC Conversion Time (CNV_TIME) Register
 *  ADC Conversion Time (CNV_TIME) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CNV_TIME_Struct
 *! \brief  ADC Conversion Time Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CNV_TIME_t__
typedef struct _ADI_ADC_CNV_TIME_t {
  union {
    struct {
      unsigned int SAMPTIME   :  8;  /**< Number of clock cycles (ACLK) required for sampling */
      unsigned int DLY        :  8;  /**< Delay between two consecutive conversions in terms of number of ACLK cycles */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CNV_TIME_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CNV_TIME_t__ */

/*@}*/

/** @defgroup AVG_CFG Averaging Configuration (AVG_CFG) Register
 *  Averaging Configuration (AVG_CFG) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_AVG_CFG_Struct
 *! \brief  Averaging Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_AVG_CFG_t__
typedef struct _ADI_ADC_AVG_CFG_t {
  union {
    struct {
      unsigned int FACTOR     :  8;  /**< Program averaging factor for averaging enabled channels (1-256) */
      unsigned int reserved8  :  6;
      unsigned int OS         :  1;  /**< Enable oversampling */
      unsigned int EN         :  1;  /**< To enable averaging on Channels enabled in enable register */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_AVG_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_AVG_CFG_t__ */

/*@}*/

/** @defgroup IRQ_EN Interrupt Enable (IRQ_EN) Register
 *  Interrupt Enable (IRQ_EN) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_IRQ_EN_Struct
 *! \brief  Interrupt Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_IRQ_EN_t__
typedef struct _ADI_ADC_IRQ_EN_t {
  union {
    struct {
      unsigned int CNVDONE    :  1;  /**< Set it to enable interrupt after conversion is done */
      unsigned int reserved1  :  9;
      unsigned int CALDONE    :  1;  /**< Set it to enable interrupt for calibration done */
      unsigned int OVF        :  1;  /**< Set to enable interrupt in case of overflow */
      unsigned int ALERT      :  1;  /**< Set to enable interrupt on crossing lower or higher limit */
      unsigned int RDY        :  1;  /**< Set to enable interrupt when ADC is ready to convert */
      unsigned int reserved14 :  2;
    };
    uint16_t VALUE16;
  };
} ADI_ADC_IRQ_EN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_IRQ_EN_t__ */

/*@}*/

/** @defgroup STAT ADC Status (STAT) Register
 *  ADC Status (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_STAT_Struct
 *! \brief  ADC Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_STAT_t__
typedef struct _ADI_ADC_STAT_t {
  union {
    struct {
      unsigned int DONE0      :  1;  /**< Indicates conversion done on Channel 0 */
      unsigned int DONE1      :  1;  /**< Indicates conversion done on Channel 1 */
      unsigned int DONE2      :  1;  /**< Indicates conversion done on Channel 2 */
      unsigned int DONE3      :  1;  /**< Indicates conversion done on Channel 3 */
      unsigned int DONE4      :  1;  /**< Indicates conversion done on Channel 4 */
      unsigned int DONE5      :  1;  /**< Indicates conversion done on Channel 5 */
      unsigned int DONE6      :  1;  /**< Indicates conversion done on Channel 6 */
      unsigned int DONE7      :  1;  /**< Indicates conversion done on Channel 7 */
      unsigned int BATDONE    :  1;  /**< Indicates conversion done for battery monitoring */
      unsigned int TMPDONE    :  1;  /**< Indicates conversion is done for temperature sensing */
      unsigned int TMP2DONE   :  1;  /**< Indicates conversion is done for temperature sensing 2 */
      unsigned int reserved11 :  3;
      unsigned int CALDONE    :  1;  /**< Indicates calibration is done */
      unsigned int RDY        :  1;  /**< Indicates ADC is ready to start converting, when using external reference buffer */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_STAT_t__ */

/*@}*/

/** @defgroup OVF Overflow of Output Registers (OVF) Register
 *  Overflow of Output Registers (OVF) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_OVF_Struct
 *! \brief  Overflow of Output Registers Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_OVF_t__
typedef struct _ADI_ADC_OVF_t {
  union {
    struct {
      unsigned int CH0        :  1;  /**< Indicates overflow in channel 0 output register */
      unsigned int CH1        :  1;  /**< Indicates overflow in channel 1 output register */
      unsigned int CH2        :  1;  /**< Indicates overflow in channel 2 output register */
      unsigned int CH3        :  1;  /**< Indicates overflow in channel 3 output register */
      unsigned int CH4        :  1;  /**< Indicates overflow in channel 4 output register */
      unsigned int CH5        :  1;  /**< Indicates overflow in channel 5 output register */
      unsigned int CH6        :  1;  /**< Indicates overflow in channel 6 output register */
      unsigned int CH7        :  1;  /**< Indicates overflow in channel 7 output register */
      unsigned int BAT        :  1;  /**< Indicates overflow in battery monitoring output register */
      unsigned int TMP        :  1;  /**< Indicates overflow in temperature output register */
      unsigned int TMP2       :  1;  /**< Indicates overflow in temperature 2 output register */
      unsigned int reserved11 :  5;
    };
    uint16_t VALUE16;
  };
} ADI_ADC_OVF_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_OVF_t__ */

/*@}*/

/** @defgroup ALERT Alert Indication (ALERT) Register
 *  Alert Indication (ALERT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_ALERT_Struct
 *! \brief  Alert Indication Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_ALERT_t__
typedef struct _ADI_ADC_ALERT_t {
  union {
    struct {
      unsigned int HI0        :  1;  /**< Channel 0 High alert status */
      unsigned int LO0        :  1;  /**< Channel 0 Low alert status */
      unsigned int HI1        :  1;  /**< Channel 1 High alert status */
      unsigned int LO1        :  1;  /**< Channel 1 Low alert status */
      unsigned int HI2        :  1;  /**< Channel 2 High alert status */
      unsigned int LO2        :  1;  /**< Channel 2 Low alert status */
      unsigned int HI3        :  1;  /**< Channel 3 High alert status */
      unsigned int LO3        :  1;  /**< Channel 3 Low alert status */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_ADC_ALERT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_ALERT_t__ */

/*@}*/

/** @defgroup CH0_OUT Conversion Result Ch0 (CH0_OUT) Register
 *  Conversion Result Ch0 (CH0_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH0_OUT_Struct
 *! \brief  Conversion Result Ch0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH0_OUT_t__
typedef struct _ADI_ADC_CH0_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 0 is stored here. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH0_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH0_OUT_t__ */

/*@}*/

/** @defgroup CH1_OUT Conversion Result Ch1 (CH1_OUT) Register
 *  Conversion Result Ch1 (CH1_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH1_OUT_Struct
 *! \brief  Conversion Result Ch1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH1_OUT_t__
typedef struct _ADI_ADC_CH1_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 1 is stored here. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH1_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH1_OUT_t__ */

/*@}*/

/** @defgroup CH2_OUT Conversion Result Ch2 (CH2_OUT) Register
 *  Conversion Result Ch2 (CH2_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH2_OUT_Struct
 *! \brief  Conversion Result Ch2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH2_OUT_t__
typedef struct _ADI_ADC_CH2_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 2 is stored here. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH2_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH2_OUT_t__ */

/*@}*/

/** @defgroup CH3_OUT Conversion Result Ch3 (CH3_OUT) Register
 *  Conversion Result Ch3 (CH3_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH3_OUT_Struct
 *! \brief  Conversion Result Ch3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH3_OUT_t__
typedef struct _ADI_ADC_CH3_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 3 is stored here. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH3_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH3_OUT_t__ */

/*@}*/

/** @defgroup CH4_OUT Conversion Result Ch4 (CH4_OUT) Register
 *  Conversion Result Ch4 (CH4_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH4_OUT_Struct
 *! \brief  Conversion Result Ch4 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH4_OUT_t__
typedef struct _ADI_ADC_CH4_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 4 is stored here. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH4_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH4_OUT_t__ */

/*@}*/

/** @defgroup CH5_OUT Conversion Result Ch5 (CH5_OUT) Register
 *  Conversion Result Ch5 (CH5_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH5_OUT_Struct
 *! \brief  Conversion Result Ch5 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH5_OUT_t__
typedef struct _ADI_ADC_CH5_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 5 is stored here. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH5_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH5_OUT_t__ */

/*@}*/

/** @defgroup CH6_OUT Conversion Result Ch6 (CH6_OUT) Register
 *  Conversion Result Ch6 (CH6_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH6_OUT_Struct
 *! \brief  Conversion Result Ch6 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH6_OUT_t__
typedef struct _ADI_ADC_CH6_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 6 is stored here. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH6_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH6_OUT_t__ */

/*@}*/

/** @defgroup CH7_OUT Conversion Result Ch7 (CH7_OUT) Register
 *  Conversion Result Ch7 (CH7_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH7_OUT_Struct
 *! \brief  Conversion Result Ch7 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH7_OUT_t__
typedef struct _ADI_ADC_CH7_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 7 is stored here. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH7_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH7_OUT_t__ */

/*@}*/

/** @defgroup BAT_OUT Battery Monitoring Result (BAT_OUT) Register
 *  Battery Monitoring Result (BAT_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_BAT_OUT_Struct
 *! \brief  Battery Monitoring Result Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_BAT_OUT_t__
typedef struct _ADI_ADC_BAT_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of battery monitoring is stored here. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_BAT_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_BAT_OUT_t__ */

/*@}*/

/** @defgroup TMP_OUT Temperature Result (TMP_OUT) Register
 *  Temperature Result (TMP_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_TMP_OUT_Struct
 *! \brief  Temperature Result Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_TMP_OUT_t__
typedef struct _ADI_ADC_TMP_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of Temperature measurement 1 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_TMP_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_TMP_OUT_t__ */

/*@}*/

/** @defgroup TMP2_OUT Temperature Result 2 (TMP2_OUT) Register
 *  Temperature Result 2 (TMP2_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_TMP2_OUT_Struct
 *! \brief  Temperature Result 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_TMP2_OUT_t__
typedef struct _ADI_ADC_TMP2_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of Temperature measurement 2 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_TMP2_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_TMP2_OUT_t__ */

/*@}*/

/** @defgroup DMA_OUT DMA Output Register (DMA_OUT) Register
 *  DMA Output Register (DMA_OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_DMA_OUT_Struct
 *! \brief  DMA Output Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_DMA_OUT_t__
typedef struct _ADI_ADC_DMA_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Register to store conversion result for DMA */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_DMA_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_DMA_OUT_t__ */

/*@}*/

/** @defgroup LIM0_LO Channel 0 Low Limit (LIM0_LO) Register
 *  Channel 0 Low Limit (LIM0_LO) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_LIM0_LO_Struct
 *! \brief  Channel 0 Low Limit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_LIM0_LO_t__
typedef struct _ADI_ADC_LIM0_LO_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< Low limit value for channel 0 */
      unsigned int reserved12 :  3;
      unsigned int EN         :  1;  /**< To enable low limit comparison on Channel 0 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_LIM0_LO_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_LIM0_LO_t__ */

/*@}*/

/** @defgroup LIM0_HI Channel 0 High Limit (LIM0_HI) Register
 *  Channel 0 High Limit (LIM0_HI) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_LIM0_HI_Struct
 *! \brief  Channel 0 High Limit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_LIM0_HI_t__
typedef struct _ADI_ADC_LIM0_HI_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< High limit value for channel 0 */
      unsigned int reserved12 :  3;
      unsigned int EN         :  1;  /**< To enable high limit comparison on Channel 0 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_LIM0_HI_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_LIM0_HI_t__ */

/*@}*/

/** @defgroup HYS0 Channel 0 Hysteresis (HYS0) Register
 *  Channel 0 Hysteresis (HYS0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_HYS0_Struct
 *! \brief  Channel 0 Hysteresis Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_HYS0_t__
typedef struct _ADI_ADC_HYS0_t {
  union {
    struct {
      unsigned int VALUE      :  9;  /**< Hysteresis value for Channel 0 */
      unsigned int reserved9  :  3;
      unsigned int MONCYC     :  3;  /**< Program number of conversion cycles to monitor channel 0 before raising alert */
      unsigned int EN         :  1;  /**< To enable hysteresis for comparison on Channel 0 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_HYS0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_HYS0_t__ */

/*@}*/

/** @defgroup LIM1_LO Channel 1 Low Limit (LIM1_LO) Register
 *  Channel 1 Low Limit (LIM1_LO) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_LIM1_LO_Struct
 *! \brief  Channel 1 Low Limit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_LIM1_LO_t__
typedef struct _ADI_ADC_LIM1_LO_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< Low limit value for channel 1 */
      unsigned int reserved12 :  3;
      unsigned int EN         :  1;  /**< To enable low limit comparison on Channel 1 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_LIM1_LO_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_LIM1_LO_t__ */

/*@}*/

/** @defgroup LIM1_HI Channel 1 High Limit (LIM1_HI) Register
 *  Channel 1 High Limit (LIM1_HI) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_LIM1_HI_Struct
 *! \brief  Channel 1 High Limit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_LIM1_HI_t__
typedef struct _ADI_ADC_LIM1_HI_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< High limit value for channel 1 */
      unsigned int reserved12 :  3;
      unsigned int EN         :  1;  /**< To enable high limit comparison on Channel 1 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_LIM1_HI_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_LIM1_HI_t__ */

/*@}*/

/** @defgroup HYS1 Channel 1 Hysteresis (HYS1) Register
 *  Channel 1 Hysteresis (HYS1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_HYS1_Struct
 *! \brief  Channel 1 Hysteresis Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_HYS1_t__
typedef struct _ADI_ADC_HYS1_t {
  union {
    struct {
      unsigned int VALUE      :  9;  /**< Hysteresis value for Channel 1 */
      unsigned int reserved9  :  3;
      unsigned int MONCYC     :  3;  /**< Program number of conversion cycles to monitor channel 1 before raising alert */
      unsigned int EN         :  1;  /**< To enable hysteresis for comparison on Channel 1 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_HYS1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_HYS1_t__ */

/*@}*/

/** @defgroup LIM2_LO Channel 2 Low Limit (LIM2_LO) Register
 *  Channel 2 Low Limit (LIM2_LO) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_LIM2_LO_Struct
 *! \brief  Channel 2 Low Limit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_LIM2_LO_t__
typedef struct _ADI_ADC_LIM2_LO_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< Low limit value for channel 2 */
      unsigned int reserved12 :  3;
      unsigned int EN         :  1;  /**< To enable low limit comparison on Channel 2 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_LIM2_LO_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_LIM2_LO_t__ */

/*@}*/

/** @defgroup LIM2_HI Channel 2 High Limit (LIM2_HI) Register
 *  Channel 2 High Limit (LIM2_HI) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_LIM2_HI_Struct
 *! \brief  Channel 2 High Limit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_LIM2_HI_t__
typedef struct _ADI_ADC_LIM2_HI_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< High limit value for channel 2 */
      unsigned int reserved12 :  3;
      unsigned int EN         :  1;  /**< To enable high limit comparison on Channel 2 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_LIM2_HI_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_LIM2_HI_t__ */

/*@}*/

/** @defgroup HYS2 Channel 2 Hysteresis (HYS2) Register
 *  Channel 2 Hysteresis (HYS2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_HYS2_Struct
 *! \brief  Channel 2 Hysteresis Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_HYS2_t__
typedef struct _ADI_ADC_HYS2_t {
  union {
    struct {
      unsigned int VALUE      :  9;  /**< Hysteresis value for Channel 2 */
      unsigned int reserved9  :  3;
      unsigned int MONCYC     :  3;  /**< Program number of conversion cycles to monitor channel 2 before raising alert */
      unsigned int EN         :  1;  /**< To enable hysteresis for comparison on Channel 2 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_HYS2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_HYS2_t__ */

/*@}*/

/** @defgroup LIM3_LO Channel 3 Low Limit (LIM3_LO) Register
 *  Channel 3 Low Limit (LIM3_LO) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_LIM3_LO_Struct
 *! \brief  Channel 3 Low Limit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_LIM3_LO_t__
typedef struct _ADI_ADC_LIM3_LO_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< Low limit value for channel 3 */
      unsigned int reserved12 :  3;
      unsigned int EN         :  1;  /**< To enable low limit comparison on Channel 3 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_LIM3_LO_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_LIM3_LO_t__ */

/*@}*/

/** @defgroup LIM3_HI Channel 3 High Limit (LIM3_HI) Register
 *  Channel 3 High Limit (LIM3_HI) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_LIM3_HI_Struct
 *! \brief  Channel 3 High Limit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_LIM3_HI_t__
typedef struct _ADI_ADC_LIM3_HI_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< High limit value for channel 3 */
      unsigned int reserved12 :  3;
      unsigned int EN         :  1;  /**< To enable high limit comparison on Channel 3 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_LIM3_HI_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_LIM3_HI_t__ */

/*@}*/

/** @defgroup HYS3 Channel 3 Hysteresis (HYS3) Register
 *  Channel 3 Hysteresis (HYS3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_HYS3_Struct
 *! \brief  Channel 3 Hysteresis Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_HYS3_t__
typedef struct _ADI_ADC_HYS3_t {
  union {
    struct {
      unsigned int VALUE      :  9;  /**< Hysteresis value for Channel 3 */
      unsigned int reserved9  :  3;
      unsigned int MONCYC     :  3;  /**< Program number of conversion cycles to monitor channel 3 before raising alert */
      unsigned int EN         :  1;  /**< To enable hysteresis for comparison on Channel 3. */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_HYS3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_HYS3_t__ */

/*@}*/

/** @defgroup CFG1 Reference Buffer Low Power Mode (CFG1) Register
 *  Reference Buffer Low Power Mode (CFG1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CFG1_Struct
 *! \brief  Reference Buffer Low Power Mode Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CFG1_t__
typedef struct _ADI_ADC_CFG1_t {
  union {
    struct {
      unsigned int RBUFLP     :  1;  /**< Enable low power mode for reference buffer */
      unsigned int reserved1  : 15;
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CFG1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CFG1_t__ */

/*@}*/

/** @defgroup STAT DMA Status (STAT) Register
 *  DMA Status (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_STAT_Struct
 *! \brief  DMA Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_STAT_t__
typedef struct _ADI_DMA_STAT_t {
  union {
    struct {
      unsigned int MEN        :  1;  /**< Enable status of the controller */
      unsigned int reserved1  : 15;
      unsigned int CHANM1     :  5;  /**< Number of available DMA channels minus 1 */
      unsigned int reserved21 : 11;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_STAT_t__ */

/*@}*/

/** @defgroup CFG DMA Configuration (CFG) Register
 *  DMA Configuration (CFG) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_CFG_Struct
 *! \brief  DMA Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_CFG_t__
typedef struct _ADI_DMA_CFG_t {
  union {
    struct {
      unsigned int MEN        :  1;  /**< Controller enable */
      unsigned int reserved1  : 31;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_CFG_t__ */

/*@}*/

/** @defgroup PDBPTR DMA channel primary control data base pointer (PDBPTR) Register
 *  DMA channel primary control data base pointer (PDBPTR) Register.
 *
 *  The DMAPDBPTR register must be programmed to point to the primary channel control base pointer in the system memory. The amount of system memory that must be assigned to the DMA controller depends on the number of DMA channels used and whether the alternate channel control data structure is used. This register cannot be read when the DMA controller is in the reset state.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_PDBPTR_Struct
 *! \brief  DMA channel primary control data base pointer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_PDBPTR_t__
typedef struct _ADI_DMA_PDBPTR_t {
  union {
    struct {
      unsigned int ADDR       : 32;  /**< Pointer to the base address of the primary data structure */
    };
    uint32_t VALUE32;
  };
} ADI_DMA_PDBPTR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_PDBPTR_t__ */

/*@}*/

/** @defgroup ADBPTR DMA channel alternate control data base pointer (ADBPTR) Register
 *  DMA channel alternate control data base pointer (ADBPTR) Register.
 *
 *  The DMAADBPTR read-only register returns the base address of the alternate channel control data structure. This register removes the necessity for application software to calculate the base address of the alternate data structure. This register cannot be read when the DMA controller is in the reset state.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ADBPTR_Struct
 *! \brief  DMA channel alternate control data base pointer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ADBPTR_t__
typedef struct _ADI_DMA_ADBPTR_t {
  union {
    struct {
      unsigned int ADDR       : 32;  /**< Base address of the alternate data structure */
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ADBPTR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ADBPTR_t__ */

/*@}*/

/** @defgroup SWREQ DMA channel software request (SWREQ) Register
 *  DMA channel software request (SWREQ) Register.
 *
 *  The DMASWREQ register enables the generation of software DMA request. Each bit of the register represents the corresponding channel number in the DMA controller.  M is the number of DMA channels
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_SWREQ_Struct
 *! \brief  DMA channel software request Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_SWREQ_t__
typedef struct _ADI_DMA_SWREQ_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Generate software request */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_SWREQ_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_SWREQ_t__ */

/*@}*/

/** @defgroup RMSK_SET DMA channel request mask set (RMSK_SET) Register
 *  DMA channel request mask set (RMSK_SET) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_RMSK_SET_Struct
 *! \brief  DMA channel request mask set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_RMSK_SET_t__
typedef struct _ADI_DMA_RMSK_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Mask requests from DMA channels */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_RMSK_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_RMSK_SET_t__ */

/*@}*/

/** @defgroup RMSK_CLR DMA channel request mask clear (RMSK_CLR) Register
 *  DMA channel request mask clear (RMSK_CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_RMSK_CLR_Struct
 *! \brief  DMA channel request mask clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_RMSK_CLR_t__
typedef struct _ADI_DMA_RMSK_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Clear Request Mask Set bits */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_RMSK_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_RMSK_CLR_t__ */

/*@}*/

/** @defgroup EN_SET DMA channel enable set (EN_SET) Register
 *  DMA channel enable set (EN_SET) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_EN_SET_Struct
 *! \brief  DMA channel enable set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_EN_SET_t__
typedef struct _ADI_DMA_EN_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Enable DMA channels */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_EN_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_EN_SET_t__ */

/*@}*/

/** @defgroup EN_CLR DMA channel enable clear (EN_CLR) Register
 *  DMA channel enable clear (EN_CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_EN_CLR_Struct
 *! \brief  DMA channel enable clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_EN_CLR_t__
typedef struct _ADI_DMA_EN_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Disable DMA channels */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_EN_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_EN_CLR_t__ */

/*@}*/

/** @defgroup ALT_SET DMA channel primary-alternate set (ALT_SET) Register
 *  DMA channel primary-alternate set (ALT_SET) Register.
 *
 *  The DMAALTSET register enables the user to configure the appropriate DMA channel to use the alternate control data structure. Reading the register returns the status of which data structure is in use for the corresponding DMA channel. Each bit of the register represents the corresponding channel number in the DMA controller.  
 *  
 *  Note: The DMA controller sets/clears these bits automatically as necessary for ping-pong, memory scatter-gather and peripheral scatter-gather transfers.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ALT_SET_Struct
 *! \brief  DMA channel primary-alternate set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ALT_SET_t__
typedef struct _ADI_DMA_ALT_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Control struct status / select alt struct */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ALT_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ALT_SET_t__ */

/*@}*/

/** @defgroup ALT_CLR DMA channel primary-alternate clear (ALT_CLR) Register
 *  DMA channel primary-alternate clear (ALT_CLR) Register.
 *
 *  The DMAALTCLR write-only register enables the user to configure the appropriate DMA channel to use the primary control data structure. Each bit of the register represents the corresponding channel number in the DMA controller.  
 *  
 *  Note: The DMA controller sets/clears these bits automatically as necessary for ping-pong, memory scatter-gather and peripheral scatter-gather transfers.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ALT_CLR_Struct
 *! \brief  DMA channel primary-alternate clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ALT_CLR_t__
typedef struct _ADI_DMA_ALT_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Select primary data struct */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ALT_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ALT_CLR_t__ */

/*@}*/

/** @defgroup PRI_SET DMA channel priority set (PRI_SET) Register
 *  DMA channel priority set (PRI_SET) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_PRI_SET_Struct
 *! \brief  DMA channel priority set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_PRI_SET_t__
typedef struct _ADI_DMA_PRI_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Configure channel for high priority */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_PRI_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_PRI_SET_t__ */

/*@}*/

/** @defgroup PRI_CLR DMA channel priority clear (PRI_CLR) Register
 *  DMA channel priority clear (PRI_CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_PRI_CLR_Struct
 *! \brief  DMA channel priority clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_PRI_CLR_t__
typedef struct _ADI_DMA_PRI_CLR_t {
  union {
    struct {
      unsigned int CHPRICLR   : 27;  /**< Configure channel for default priority level */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_PRI_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_PRI_CLR_t__ */

/*@}*/

/** @defgroup ERRCHNL_CLR DMA Per Channel Error Clear (ERRCHNL_CLR) Register
 *  DMA Per Channel Error Clear (ERRCHNL_CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ERRCHNL_CLR_Struct
 *! \brief  DMA Per Channel Error Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ERRCHNL_CLR_t__
typedef struct _ADI_DMA_ERRCHNL_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Per channel Bus error status/ Per channel bus error clear */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ERRCHNL_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ERRCHNL_CLR_t__ */

/*@}*/

/** @defgroup ERR_CLR DMA bus error clear (ERR_CLR) Register
 *  DMA bus error clear (ERR_CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ERR_CLR_Struct
 *! \brief  DMA bus error clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ERR_CLR_t__
typedef struct _ADI_DMA_ERR_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Bus error status */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ERR_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ERR_CLR_t__ */

/*@}*/

/** @defgroup INVALIDDESC_CLR DMA Per Channel Invalid Descriptor Clear (INVALIDDESC_CLR) Register
 *  DMA Per Channel Invalid Descriptor Clear (INVALIDDESC_CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_INVALIDDESC_CLR_Struct
 *! \brief  DMA Per Channel Invalid Descriptor Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_INVALIDDESC_CLR_t__
typedef struct _ADI_DMA_INVALIDDESC_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Per channel Invalid Descriptor status/ Per channel Invalid descriptor status clear */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_INVALIDDESC_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_INVALIDDESC_CLR_t__ */

/*@}*/

/** @defgroup BS_SET DMA channel bytes swap enable set (BS_SET) Register
 *  DMA channel bytes swap enable set (BS_SET) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_BS_SET_Struct
 *! \brief  DMA channel bytes swap enable set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_BS_SET_t__
typedef struct _ADI_DMA_BS_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Byte swap status */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_BS_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_BS_SET_t__ */

/*@}*/

/** @defgroup BS_CLR DMA channel bytes swap enable clear (BS_CLR) Register
 *  DMA channel bytes swap enable clear (BS_CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_BS_CLR_Struct
 *! \brief  DMA channel bytes swap enable clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_BS_CLR_t__
typedef struct _ADI_DMA_BS_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Disable byte swap */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_BS_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_BS_CLR_t__ */

/*@}*/

/** @defgroup SRCADDR_SET DMA channel source address decrement enable set (SRCADDR_SET) Register
 *  DMA channel source address decrement enable set (SRCADDR_SET) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_SRCADDR_SET_Struct
 *! \brief  DMA channel source address decrement enable set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_SRCADDR_SET_t__
typedef struct _ADI_DMA_SRCADDR_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Source Address decrement status / configure Source address decrement */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_SRCADDR_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_SRCADDR_SET_t__ */

/*@}*/

/** @defgroup SRCADDR_CLR DMA channel source address decrement enable clear (SRCADDR_CLR) Register
 *  DMA channel source address decrement enable clear (SRCADDR_CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_SRCADDR_CLR_Struct
 *! \brief  DMA channel source address decrement enable clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_SRCADDR_CLR_t__
typedef struct _ADI_DMA_SRCADDR_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Disable source address decrement */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_SRCADDR_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_SRCADDR_CLR_t__ */

/*@}*/

/** @defgroup DSTADDR_SET DMA channel destination address decrement enable set (DSTADDR_SET) Register
 *  DMA channel destination address decrement enable set (DSTADDR_SET) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_DSTADDR_SET_Struct
 *! \brief  DMA channel destination address decrement enable set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_DSTADDR_SET_t__
typedef struct _ADI_DMA_DSTADDR_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Destination Address decrement status / configure destination address decrement */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_DSTADDR_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_DSTADDR_SET_t__ */

/*@}*/

/** @defgroup DSTADDR_CLR DMA channel destination address decrement enable clear (DSTADDR_CLR) Register
 *  DMA channel destination address decrement enable clear (DSTADDR_CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_DSTADDR_CLR_Struct
 *! \brief  DMA channel destination address decrement enable clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_DSTADDR_CLR_t__
typedef struct _ADI_DMA_DSTADDR_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Disable destination address decrement */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_DSTADDR_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_DSTADDR_CLR_t__ */

/*@}*/

/** @defgroup REVID DMA Controller Revision ID (REVID) Register
 *  DMA Controller Revision ID (REVID) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_REVID_Struct
 *! \brief  DMA Controller Revision ID Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_REVID_t__
typedef struct _ADI_DMA_REVID_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< DMA Controller revision ID */
      unsigned int reserved8  : 24;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_REVID_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_REVID_t__ */

/*@}*/

/** @defgroup STAT Status (STAT) Register
 *  Status (STAT) Register.
 *
 *  Provides information on current command states and error detection/correction
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_FLCC_STAT_ACCESS_MODE
 *! \brief  Indicates whether the controller is in Direct Access (default) or Indirect Access (servicing crypto commands) mode of operation (ACCESS_MODE) Enumerations
 *  ========================================================================= */
typedef enum
{
  FLCC_STAT_DIRECT   = 0,  /**< Flash controller is currently in Direct Access mode; user access to all registers is enabled                                                                                                                                                                                                                                         */
  FLCC_STAT_INDIRECT = 1   /**< Flash Controller is currently in Indirect Access mode; user access to registers is limited to read-only access of the status register. Full register access will be restored when the Cryptographic module releases control of the flash controller (crypto completes the ongoing operation within the protected key storage region) */
} ADI_FLCC_STAT_ACCESS_MODE;


/* ==========================================================================
 *! \struct ADI_FLCC_STAT_Struct
 *! \brief  Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_STAT_t__
typedef struct _ADI_FLCC_STAT_t {
  union {
    struct {
      unsigned int CMDBUSY    :  1;  /**< Command busy */
      unsigned int WRCLOSE    :  1;  /**< WRITE registers are closed */
      unsigned int CMDCOMP    :  1;  /**< Command complete */
      unsigned int WRALCOMP   :  1;  /**< Write almost complete */
      unsigned int CMDFAIL    :  2;  /**< Provides information on command failures */
      unsigned int SLEEPING   :  1;  /**< Flash array is in low power (sleep) mode */
      unsigned int ECCERRCMD  :  2;  /**< ECC errors detected during user issued SIGN command */
      unsigned int ECCRDERR   :  2;  /**< ECC IRQ cause */
      unsigned int OVERLAP    :  1;  /**< Overlapping Command */
      unsigned int reserved12 :  1;
      unsigned int SIGNERR    :  1;  /**< Signature check failure during initialization */
      unsigned int INIT       :  1;  /**< Flash controller initialization in progress */
      unsigned int ECCINFOSIGN :  2;  /**< ECC status of flash initialization */
      unsigned int ECCERRCNT   :  3;  /**< ECC correction counter */
      unsigned int reserved20  :  5;
      unsigned int ECCICODE    :  2;  /**< ICode AHB Bus Error ECC status */
      unsigned int ECCDCODE    :  2;  /**< DCode AHB Bus Error ECC status */
      unsigned int CACHESRAMPERR :  1;  /**< SRAM parity errors in Cache Controller */
      unsigned int reserved30    :  1;
      unsigned int ACCESS_MODE   :  1;  /**< Indicates whether the controller is in Direct Access (default) or Indirect Access (servicing crypto commands) mode of operation */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_STAT_t__ */

/*@}*/

/** @defgroup IEN Interrupt Enable (IEN) Register
 *  Interrupt Enable (IEN) Register.
 *
 *  Used to specify when interrupts will be generated.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_FLCC_IEN_ECC_CORRECT
 *! \brief  Control whether to generate bus errors, interrupts, or neither in response to 1-bit ECC Correction events (ECC_CORRECT) Enumerations
 *  ========================================================================= */
typedef enum
{
  FLCC_IEN_NONE_COR    = 0,  /**< Do not generate a response to ECC events      */
  FLCC_IEN_BUS_ERR_COR = 1,  /**< Generate Bus Errors in response to ECC events */
  FLCC_IEN_IRQ_COR     = 2   /**< Generate IRQs in response to ECC events       */
} ADI_FLCC_IEN_ECC_CORRECT;


/*  =========================================================================
 *! \enum   ADI_FLCC_IEN_ECC_ERROR
 *! \brief  Control whether to generate bus errors, interrupts, or neither in response to 2-bit ECC Error events (ECC_ERROR) Enumerations
 *  ========================================================================= */
typedef enum
{
  FLCC_IEN_NONE_ERR    = 0,  /**< Do not generate a response to ECC events      */
  FLCC_IEN_BUS_ERR_ERR = 1,  /**< Generate Bus Errors in response to ECC events */
  FLCC_IEN_IRQ_ERR     = 2   /**< Generate IRQs in response to ECC events       */
} ADI_FLCC_IEN_ECC_ERROR;


/* ==========================================================================
 *! \struct ADI_FLCC_IEN_Struct
 *! \brief  Interrupt Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_IEN_t__
typedef struct _ADI_FLCC_IEN_t {
  union {
    struct {
      unsigned int CMDCMPLT   :  1;  /**< Command complete interrupt enable */
      unsigned int WRALCMPLT  :  1;  /**< Write almost complete interrupt enable */
      unsigned int CMDFAIL    :  1;  /**< Command fail interrupt enable */
      unsigned int reserved3   :  1;
      unsigned int ECC_CORRECT :  2;  /**< Control whether to generate bus errors, interrupts, or neither in response to 1-bit ECC Correction events */
      unsigned int ECC_ERROR   :  2;  /**< Control whether to generate bus errors, interrupts, or neither in response to 2-bit ECC Error events */
      unsigned int reserved8   : 24;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_IEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_IEN_t__ */

/*@}*/

/** @defgroup CMD Command (CMD) Register
 *  Command (CMD) Register.
 *
 *  Write this register to execute a specified command. The user key (see KEY register for details) must first be written to the KEY Register for most command requests to be honored (see details below).
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_FLCC_CMD_VALUE
 *! \brief  Commands (VALUE) Enumerations
 *  ========================================================================= */
typedef enum
{
  FLCC_CMD_IDLE        = 0,  /**< IDLE                                                                  */
  FLCC_CMD_ABORT       = 1,  /**< ABORT                                                                 */
  FLCC_CMD_SLEEP       = 2,  /**< Requests flash to enter Sleep mode                                    */
  FLCC_CMD_SIGN        = 3,  /**< SIGN                                                                  */
  FLCC_CMD_WRITE       = 4,  /**< WRITE                                                                 */
  FLCC_CMD_BLANK_CHECK = 5,  /**< Checks all of User Space; fails if any bits in user space are cleared */
  FLCC_CMD_ERASEPAGE   = 6,  /**< ERASEPAGE                                                             */
  FLCC_CMD_MASSERASE   = 7   /**< MASSERASE                                                             */
} ADI_FLCC_CMD_VALUE;


/* ==========================================================================
 *! \struct ADI_FLCC_CMD_Struct
 *! \brief  Command Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_CMD_t__
typedef struct _ADI_FLCC_CMD_t {
  union {
    struct {
      unsigned int VALUE      :  4;  /**< Commands */
      unsigned int reserved4  : 28;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_CMD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_CMD_t__ */

/*@}*/

/** @defgroup KH_ADDR WRITE address (KH_ADDR) Register
 *  WRITE address (KH_ADDR) Register.
 *
 *  Write the byte-address of any byte of a 64-bit dual-word flash location to be targeted by a WRITE command.
 *  
 *  All writes target 64-bit dual-word elements in the flash array. User code may byte-mask data to emulate byte, hword, or word writes. Flash IP specifications warn that no location should be written more than twice between erasures. When writing a location more than once, user should be aware that ECC meta-data cannot be updated appropriately; user code should disable ECC for the relevant region of flash.
 *  
 *  (Writing any address above the valid range of flash memory will saturate the address to prevent aliasing; user code should take care to target valid flash address locations)
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_KH_ADDR_Struct
 *! \brief  WRITE address Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_KH_ADDR_t__
typedef struct _ADI_FLCC_KH_ADDR_t {
  union {
    struct {
      unsigned int reserved0  :  3;
      unsigned int VALUE      : 17;  /**< Address to be written on a WRITE command */
      unsigned int reserved20 : 12;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_KH_ADDR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_KH_ADDR_t__ */

/*@}*/

/** @defgroup KH_DATA0 WRITE lower data (KH_DATA0) Register
 *  WRITE lower data (KH_DATA0) Register.
 *
 *  The lower half of 64-bit dualword data to be written to flash
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_KH_DATA0_Struct
 *! \brief  WRITE lower data Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_KH_DATA0_t__
typedef struct _ADI_FLCC_KH_DATA0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Lower half of 64-bit dualword data to be written on a WRITE command */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_KH_DATA0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_KH_DATA0_t__ */

/*@}*/

/** @defgroup KH_DATA1 WRITE upper data (KH_DATA1) Register
 *  WRITE upper data (KH_DATA1) Register.
 *
 *  The lower half of 64-bit dualword data to be written to flash
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_KH_DATA1_Struct
 *! \brief  WRITE upper data Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_KH_DATA1_t__
typedef struct _ADI_FLCC_KH_DATA1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Upper half of 64-bit dualword data to be written on a WRITE command.

If DMA is enabled, then this register acts as a FIFO. Writes to this register will push the old data to lower 32 bit of 64 bit data (KH_DATA0). When this register is written twice (in DMA mode) the FIFO becomes full and a flash write command will automatically be executed. */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_KH_DATA1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_KH_DATA1_t__ */

/*@}*/

/** @defgroup PAGE_ADDR0 Lower page address (PAGE_ADDR0) Register
 *  Lower page address (PAGE_ADDR0) Register.
 *
 *  Write a byte-address to this register to select the page in which that byte exists. 
 *  
 *  The selected page may be used for a ERASEPAGE command (selecting which page to erase) or for a SIGN command (selecting the start page for a block on which a signature should be calculated)
 *  
 *  For commands using both PAGE_ADDR0 and PAGE_ADDR1, user should ensure that PAGE_ADDR0 is always less than or equal to PAGE_ADDR1, else the command will be denied.
 *  
 *  (Writing any address above the valid range of flash memory will saturate the address register to prevent aliasing in the flash memory space)
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_PAGE_ADDR0_Struct
 *! \brief  Lower page address Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_PAGE_ADDR0_t__
typedef struct _ADI_FLCC_PAGE_ADDR0_t {
  union {
    struct {
      unsigned int reserved0  : 10;
      unsigned int VALUE      : 10;  /**< Lower address bits of the page address */
      unsigned int reserved20 : 12;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_PAGE_ADDR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_PAGE_ADDR0_t__ */

/*@}*/

/** @defgroup PAGE_ADDR1 Upper page address (PAGE_ADDR1) Register
 *  Upper page address (PAGE_ADDR1) Register.
 *
 *  Write a byte-address to this register to select the page in which that byte exists. 
 *  
 *  The selected page may be used for a SIGN command (selecting the end page for a block on which a signature should be calculated)
 *  
 *  For commands using both PAGE_ADDR0 and PAGE_ADDR1, user should ensure that PAGE_ADDR0 is always less than or equal to PAGE_ADDR1, else the command will be denied.
 *  
 *  (Writing any address above the valid range of flash memory will saturate the address register to prevent aliasing in the flash memory space)
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_PAGE_ADDR1_Struct
 *! \brief  Upper page address Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_PAGE_ADDR1_t__
typedef struct _ADI_FLCC_PAGE_ADDR1_t {
  union {
    struct {
      unsigned int reserved0  : 10;
      unsigned int VALUE      : 10;  /**< Upper address bits of the page address */
      unsigned int reserved20 : 12;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_PAGE_ADDR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_PAGE_ADDR1_t__ */

/*@}*/

/** @defgroup KEY Key (KEY) Register
 *  Key (KEY) Register.
 *
 *  When user code must write a key to access protected features, the key value must be written to this register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_FLCC_KEY_VALUE
 *! \brief  Key register (VALUE) Enumerations
 *  ========================================================================= */
typedef enum
{
  FLCC_KEY_USERKEY = 1735161189   /**< USERKEY */
} ADI_FLCC_KEY_VALUE;


/* ==========================================================================
 *! \struct ADI_FLCC_KEY_Struct
 *! \brief  Key Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_KEY_t__
typedef struct _ADI_FLCC_KEY_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Key register */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_KEY_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_KEY_t__ */

/*@}*/

/** @defgroup WR_ABORT_ADDR Write abort address (WR_ABORT_ADDR) Register
 *  Write abort address (WR_ABORT_ADDR) Register.
 *
 *  Address of recently aborted write command. This address is only populated if the aborted write command was started; if the command is aborted early enough to have no affect on the flash IP this address will not be updated.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_WR_ABORT_ADDR_Struct
 *! \brief  Write abort address Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_WR_ABORT_ADDR_t__
typedef struct _ADI_FLCC_WR_ABORT_ADDR_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Address of recently aborted write command */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_WR_ABORT_ADDR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_WR_ABORT_ADDR_t__ */

/*@}*/

/** @defgroup WRPROT Write protection (WRPROT) Register
 *  Write protection (WRPROT) Register.
 *
 *  [User Key] is required to modified this register.
 *  
 *  The WRPROT register may be automatically configured during device boot up; in this event the boot loader reads data from user space and loads that data into this register. 
 *  
 *  User code may affect non-volatile write protection by writing to the appropriate location in the flash memory (see chapter on Protection for details). By default, the relevant location in flash is 0x3FFF0 (the 4th most significant word in user space), but may be relocated by ADI's secure bootloader.
 *  
 *  User code may alternatively assert protection at runtime for any unprotected blocks by directly writing this register: Blocks may have protection added but cannot have protection removed; changes will be lost on reset. This approach is suggested especially during user code development.
 *  
 *  All write protection is cleared on a power-on-reset but note that the ADI secure bootloader will reassert write protection as defined by the WRPROT word in user space before enabling user access to the flash array. Therefore removing write protection can only be performed by an ERASEPAGE command of the most significant page in user space (provided that page is not currently protected) or by a MASSERASE command. Following a successful MASSERASE command all protection of pages in user space is immediately cleared (user may write to user space immediately following such an erase without a device reset required).
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_WRPROT_Struct
 *! \brief  Write protection Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_WRPROT_t__
typedef struct _ADI_FLCC_WRPROT_t {
  union {
    struct {
      unsigned int WORD       : 32;  /**< Clear bits to write protect related groups of user space pages. Once cleared these bits can only be set again by resetting the part */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_WRPROT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_WRPROT_t__ */

/*@}*/

/** @defgroup SIGNATURE Signature (SIGNATURE) Register
 *  Signature (SIGNATURE) Register.
 *
 *  Provides read access to the most recently generated signature.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_SIGNATURE_Struct
 *! \brief  Signature Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_SIGNATURE_t__
typedef struct _ADI_FLCC_SIGNATURE_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Read signature */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_SIGNATURE_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_SIGNATURE_t__ */

/*@}*/

/** @defgroup UCFG User configuration (UCFG) Register
 *  User configuration (UCFG) Register.
 *
 *  [User key] is required (see KEY register for details). 
 *  
 *  Write to this register to enable user control of DMA and Auto-increment features. 
 *  
 *  When user code has finished accessing this register, garbage data should be written to the KEY register to re-assert protection.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_UCFG_Struct
 *! \brief  User configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_UCFG_t__
typedef struct _ADI_FLCC_UCFG_t {
  union {
    struct {
      unsigned int KHDMAEN    :  1;  /**< Key-Hole DMA enable. */
      unsigned int AUTOINCEN  :  1;  /**< Auto address increment for Key hole access. */
      unsigned int reserved2  : 30;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_UCFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_UCFG_t__ */

/*@}*/

/** @defgroup TIME_PARAM0 Time parameter 0 (TIME_PARAM0) Register
 *  Time parameter 0 (TIME_PARAM0) Register.
 *
 *  [User Key] is required to write this register.
 *  
 *  This register should not be modified while a flash write or erase command is in progress.
 *  
 *  This register defines a set of parameters used to control the timing of signals driven to the Flash Memory. The default values are appropriate for a system clock of 26MHz and a reference clock (driven by the internal oscillator) operating within 10% of 13MHz.
 *  
 *  The value of each timing parameter consists of a user programmable nibble (4 bits) as well as some number of hard-coded bits. User programmable bits are the most significant bits for each parameter. 
 *  
 *  Time parameters describe the number of ref-clk periods to wait when meeting the associated timing constraint of the flash memory itself. Note that clock-domain-crossings and the constraints of signals not described by these parameters will increase the effective delays by a small margin. When programming the time parameter registers the user should select a value approaching the minimum time for each constraint.
 *  
 *  CAUTION: Improper programming of this register may result in damage to the flash memory during PROGRAM or ERASE operations.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_TIME_PARAM0_Struct
 *! \brief  Time parameter 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_TIME_PARAM0_t__
typedef struct _ADI_FLCC_TIME_PARAM0_t {
  union {
    struct {
      unsigned int DIVREFCLK  :  1;  /**< Divide Reference Clock (by 2) */
      unsigned int reserved1  :  3;
      unsigned int TNVS       :  4;  /**< PROG/ERASE to NVSTR setup time */
      unsigned int TPGS       :  4;  /**< NVSTR to Program setup time */
      unsigned int TPROG      :  4;  /**< Program time */
      unsigned int TNVH       :  4;  /**< NVSTR Hold time */
      unsigned int TRCV       :  4;  /**< Recovery time */
      unsigned int TERASE     :  4;  /**< Erase Time */
      unsigned int TNVH1      :  4;  /**< NVSTR Hold time during Mass Erase */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_TIME_PARAM0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_TIME_PARAM0_t__ */

/*@}*/

/** @defgroup TIME_PARAM1 Time parameter 1 (TIME_PARAM1) Register
 *  Time parameter 1 (TIME_PARAM1) Register.
 *
 *  See TIME_PARAM0 for documentation
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_TIME_PARAM1_Struct
 *! \brief  Time parameter 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_TIME_PARAM1_t__
typedef struct _ADI_FLCC_TIME_PARAM1_t {
  union {
    struct {
      unsigned int TWK        :  4;  /**< Wake up time */
      unsigned int WAITSTATES :  3;  /**< Number of wait states to access flash. */
      unsigned int reserved7     :  1;
      unsigned int CURWAITSTATES :  3;  /**< Current wait states[2:0] */
      unsigned int reserved11    : 21;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_TIME_PARAM1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_TIME_PARAM1_t__ */

/*@}*/

/** @defgroup ABORT_EN_LO IRQ Abort Enable (lower bits) (ABORT_EN_LO) Register
 *  IRQ Abort Enable (lower bits) (ABORT_EN_LO) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_ABORT_EN_LO_Struct
 *! \brief  IRQ Abort Enable (lower bits) Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_ABORT_EN_LO_t__
typedef struct _ADI_FLCC_ABORT_EN_LO_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< VALUE[31:0] Sys IRQ abort enable */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_ABORT_EN_LO_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_ABORT_EN_LO_t__ */

/*@}*/

/** @defgroup ABORT_EN_HI IRQ Abort Enable (upper bits) (ABORT_EN_HI) Register
 *  IRQ Abort Enable (upper bits) (ABORT_EN_HI) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_ABORT_EN_HI_Struct
 *! \brief  IRQ Abort Enable (upper bits) Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_ABORT_EN_HI_t__
typedef struct _ADI_FLCC_ABORT_EN_HI_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< VALUE[63:32] Sys IRQ abort enable */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_ABORT_EN_HI_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_ABORT_EN_HI_t__ */

/*@}*/

/** @defgroup ECC_CFG ECC Config (ECC_CFG) Register
 *  ECC Config (ECC_CFG) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_ECC_CFG_Struct
 *! \brief  ECC Config Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_ECC_CFG_t__
typedef struct _ADI_FLCC_ECC_CFG_t {
  union {
    struct {
      unsigned int EN         :  1;  /**< ECC Enable */
      unsigned int INFOEN     :  1;  /**< Info space ECC Enable bit */
      unsigned int reserved2  :  6;
      unsigned int PTR        : 24;  /**< ECC start page pointer (user should write bits [31:8] of the start page address into bits [31:8] of this register) */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_ECC_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_ECC_CFG_t__ */

/*@}*/

/** @defgroup ECC_ADDR ECC Status (address) (ECC_ADDR) Register
 *  ECC Status (address) (ECC_ADDR) Register.
 *
 *  This register is updated on ECC errors or corrections as selected to generate interrupts (IRQ) in the IEN register. this register is not updated in the event of an ECC error or correction which instead generates a bus fault.
 *  
 *  This register records the address of the first ECC error or correction event to generate an interrupt since the last time the ECC status bits were cleared (or since reset).
 *  
 *  If the status bits are cleared in the same cycle as a new ECC event (selected to generate an IRQ), a new address will be recorded and the status bits will remain set.
 *  
 *  Errors have priority over corrections (2 or more bits corrupt = ERROR; a correction results in proper data being returned after a single bit is corrected). If an error and a correction occur in the same cycle, this register will report the ERROR address.
 *  
 *  When two of the same priority ECC events occur (both ERROR or both CORRECTION) the ICODE bus has priority over DCODE. Therefore if both ICODE and DCODE buses generate the same type of ECC event in the same cycle, the ICODE address will be stored in this register.
 *  
 *  The register cannot be cleared except by reset; it will always hold the address of the most recently reported ECC correction or error.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_ECC_ADDR_Struct
 *! \brief  ECC Status (address) Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_ECC_ADDR_t__
typedef struct _ADI_FLCC_ECC_ADDR_t {
  union {
    struct {
      unsigned int VALUE      : 20;  /**< This register has the address for which ECC error is detected */
      unsigned int reserved20 : 12;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_ECC_ADDR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_ECC_ADDR_t__ */

/*@}*/

/** @defgroup STAT Cache Status register (STAT) Register
 *  Cache Status register (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_CACHE_STAT_Struct
 *! \brief  Cache Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_STAT_t__
typedef struct _ADI_FLCC_CACHE_STAT_t {
  union {
    struct {
      unsigned int ICEN       :  1;  /**< If this bit is set then I-Cache is enabled and when cleared I-Cache is disabled. */
      unsigned int reserved1  : 31;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_CACHE_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_STAT_t__ */

/*@}*/

/** @defgroup SETUP Cache Setup register (SETUP) Register
 *  Cache Setup register (SETUP) Register.
 *
 *  Cache User key is required to enable a write to this location.  Key will be cleared after a write to this Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_CACHE_SETUP_Struct
 *! \brief  Cache Setup Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_SETUP_t__
typedef struct _ADI_FLCC_CACHE_SETUP_t {
  union {
    struct {
      unsigned int ICEN       :  1;  /**< If this bit set, then I-Cache is enabled for AHB accesses. If 0, then I-cache is disabled, and all AHB accesses will be via Flash memory. This bit is cleared when Init bit is set. */
      unsigned int reserved1  : 31;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_CACHE_SETUP_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_SETUP_t__ */

/*@}*/

/** @defgroup KEY Cache Key register (KEY) Register
 *  Cache Key register (KEY) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_CACHE_KEY_Struct
 *! \brief  Cache Key Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_KEY_t__
typedef struct _ADI_FLCC_CACHE_KEY_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Cache Key register */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_CACHE_KEY_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_KEY_t__ */

/*@}*/

/** @defgroup CFG Port  Configuration (CFG) Register
 *  Port  Configuration (CFG) Register.
 *
 *  The CFG register is reserved for top-level pin muxing for the GPIO block.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_CFG_Struct
 *! \brief  Port  Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_CFG_t__
typedef struct _ADI_GPIO_CFG_t {
  union {
    struct {
      unsigned int PIN00      :  2;  /**< Pin 0 configuration bits */
      unsigned int PIN01      :  2;  /**< Pin 1 configuration bits */
      unsigned int PIN02      :  2;  /**< Pin 2 configuration bits */
      unsigned int PIN03      :  2;  /**< Pin 3 configuration bits */
      unsigned int PIN04      :  2;  /**< Pin 4 configuration bits */
      unsigned int PIN05      :  2;  /**< Pin 5 configuration bits */
      unsigned int PIN06      :  2;  /**< Pin 6 configuration bits */
      unsigned int PIN07      :  2;  /**< Pin 7 configuration bits */
      unsigned int PIN08      :  2;  /**< Pin 8 configuration bits */
      unsigned int PIN09      :  2;  /**< Pin 9 configuration bits */
      unsigned int PIN10      :  2;  /**< Pin 10  configuration bits */
      unsigned int PIN11      :  2;  /**< Pin 11  configuration bits */
      unsigned int PIN12      :  2;  /**< Pin 12  configuration bits */
      unsigned int PIN13      :  2;  /**< Pin 13  configuration bits */
      unsigned int PIN14      :  2;  /**< Pin 14  configuration bits */
      unsigned int PIN15      :  2;  /**< Pin 15  configuration bits */
    };
    uint32_t VALUE32;
  };
} ADI_GPIO_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_CFG_t__ */

/*@}*/

/** @defgroup OEN Port Output Enable (OEN) Register
 *  Port Output Enable (OEN) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_OEN_Struct
 *! \brief  Port Output Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_OEN_t__
typedef struct _ADI_GPIO_OEN_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Pin Output Drive enable */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_OEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_OEN_t__ */

/*@}*/

/** @defgroup PE Port Output Pull-up/Pull-down Enable (PE) Register
 *  Port Output Pull-up/Pull-down Enable (PE) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_PE_Struct
 *! \brief  Port Output Pull-up/Pull-down Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_PE_t__
typedef struct _ADI_GPIO_PE_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Pin Pull enable */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_PE_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_PE_t__ */

/*@}*/

/** @defgroup IEN Port  Input Path Enable (IEN) Register
 *  Port  Input Path Enable (IEN) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_IEN_Struct
 *! \brief  Port  Input Path Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_IEN_t__
typedef struct _ADI_GPIO_IEN_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Input path enable */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_IEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_IEN_t__ */

/*@}*/

/** @defgroup IN Port  Registered Data Input (IN) Register
 *  Port  Registered Data Input (IN) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_IN_Struct
 *! \brief  Port  Registered Data Input Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_IN_t__
typedef struct _ADI_GPIO_IN_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Registered data input */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_IN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_IN_t__ */

/*@}*/

/** @defgroup OUT Port Data Output (OUT) Register
 *  Port Data Output (OUT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_OUT_Struct
 *! \brief  Port Data Output Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_OUT_t__
typedef struct _ADI_GPIO_OUT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Data out */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_OUT_t__ */

/*@}*/

/** @defgroup SET Port Data Out Set (SET) Register
 *  Port Data Out Set (SET) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_SET_Struct
 *! \brief  Port Data Out Set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_SET_t__
typedef struct _ADI_GPIO_SET_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Set the output HIGH for the pin */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_SET_t__ */

/*@}*/

/** @defgroup CLR Port Data Out Clear (CLR) Register
 *  Port Data Out Clear (CLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_CLR_Struct
 *! \brief  Port Data Out Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_CLR_t__
typedef struct _ADI_GPIO_CLR_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Set the output low  for the port pin */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_CLR_t__ */

/*@}*/

/** @defgroup TGL Port Pin Toggle (TGL) Register
 *  Port Pin Toggle (TGL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_TGL_Struct
 *! \brief  Port Pin Toggle Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_TGL_t__
typedef struct _ADI_GPIO_TGL_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Toggle the output of the port pin */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_TGL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_TGL_t__ */

/*@}*/

/** @defgroup POL Port Interrupt Polarity (POL) Register
 *  Port Interrupt Polarity (POL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_POL_Struct
 *! \brief  Port Interrupt Polarity Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_POL_t__
typedef struct _ADI_GPIO_POL_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Interrupt polarity */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_POL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_POL_t__ */

/*@}*/

/** @defgroup IENA Port Interrupt A Enable (IENA) Register
 *  Port Interrupt A Enable (IENA) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_IENA_Struct
 *! \brief  Port Interrupt A Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_IENA_t__
typedef struct _ADI_GPIO_IENA_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Interrupt A enable */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_IENA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_IENA_t__ */

/*@}*/

/** @defgroup IENB Port Interrupt B Enable (IENB) Register
 *  Port Interrupt B Enable (IENB) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_IENB_Struct
 *! \brief  Port Interrupt B Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_IENB_t__
typedef struct _ADI_GPIO_IENB_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Interrupt B enable */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_IENB_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_IENB_t__ */

/*@}*/

/** @defgroup INT Port Interrupt Status (INT) Register
 *  Port Interrupt Status (INT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_INT_Struct
 *! \brief  Port Interrupt Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_INT_t__
typedef struct _ADI_GPIO_INT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Interrupt Status */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_INT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_INT_t__ */

/*@}*/

/** @defgroup DS Port Drive Strength Select (DS) Register
 *  Port Drive Strength Select (DS) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_GPIO_DS_Struct
 *! \brief  Port Drive Strength Select Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_DS_t__
typedef struct _ADI_GPIO_DS_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Drive strength select */
    };
    uint16_t VALUE16;
  };
} ADI_GPIO_DS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_DS_t__ */

/*@}*/

/** @defgroup CTL_A Half SPORT 'A' Control Register (CTL_A) Register
 *  Half SPORT 'A' Control Register (CTL_A) Register.
 *
 *  The CTL_A contains transmit and receive control bits for SPORT half 'A', including serial port mode selection
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_SPEN
 *! \brief  Serial Port Enable (SPEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_DIS = 0,  /**< Disable */
  SPORT_CTL_A_CTL_EN  = 1   /**< Enable  */
} ADI_SPORT_CTL_A_SPEN;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_FSMUXSEL
 *! \brief  Frame Sync Multiplexer Select (FSMUXSEL) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_FS_MUX_DIS = 0,  /**< Disable frame sync multiplexing */
  SPORT_CTL_A_CTL_FS_MUX_EN  = 1   /**< Enable frame sync multiplexing  */
} ADI_SPORT_CTL_A_FSMUXSEL;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_CKMUXSEL
 *! \brief  Clock Multiplexer Select (CKMUXSEL) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_CLK_MUX_DIS = 0,  /**< Disable serial clock multiplexing */
  SPORT_CTL_A_CTL_CLK_MUX_EN  = 1   /**< Enable serial clock multiplexing  */
} ADI_SPORT_CTL_A_CKMUXSEL;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_LSBF
 *! \brief  Least-Significant Bit First (LSBF) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_MSB_FIRST = 0,  /**< MSB first sent/received */
  SPORT_CTL_A_CTL_LSB_FIRST = 1   /**< LSB first sent/received */
} ADI_SPORT_CTL_A_LSBF;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_ICLK
 *! \brief  Internal Clock (ICLK) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_EXTERNAL_CLK = 0,  /**< External clock */
  SPORT_CTL_A_CTL_INTERNAL_CLK = 1   /**< Internal clock */
} ADI_SPORT_CTL_A_ICLK;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_OPMODE
 *! \brief  Operation mode (OPMODE) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_SERIAL        = 0,  /**< DSP standard      */
  SPORT_CTL_A_CTL_TIMER_EN_MODE = 1   /**< Timer_enable mode */
} ADI_SPORT_CTL_A_OPMODE;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_CKRE
 *! \brief  Clock Rising Edge (CKRE) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_CLK_FALL_EDGE = 0,  /**< Clock falling edge */
  SPORT_CTL_A_CTL_CLK_RISE_EDGE = 1   /**< Clock rising edge  */
} ADI_SPORT_CTL_A_CKRE;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_FSR
 *! \brief  Frame Sync Required (FSR) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_FS_NOT_REQ = 0,  /**< No frame sync required */
  SPORT_CTL_A_CTL_FS_REQ     = 1   /**< Frame sync required    */
} ADI_SPORT_CTL_A_FSR;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_IFS
 *! \brief  Internal Frame Sync (IFS) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_EXTERNAL_FS = 0,  /**< External frame sync */
  SPORT_CTL_A_CTL_INTERNAL_FS = 1   /**< Internal frame sync */
} ADI_SPORT_CTL_A_IFS;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_DIFS
 *! \brief  Data-Independent Frame Sync (DIFS) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_DATA_DEP_FS  = 0,  /**< Data-dependent frame sync   */
  SPORT_CTL_A_CTL_DATA_INDP_FS = 1   /**< Data-independent frame sync */
} ADI_SPORT_CTL_A_DIFS;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_LFS
 *! \brief  Active-Low Frame Sync (LFS) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_FS_LO = 0,  /**< Active high frame sync */
  SPORT_CTL_A_CTL_FS_HI = 1   /**< Active low frame sync  */
} ADI_SPORT_CTL_A_LFS;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_LAFS
 *! \brief  Late Frame Sync (LAFS) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_EARLY_FS = 0,  /**< Early frame sync */
  SPORT_CTL_A_CTL_LATE_FS  = 1   /**< Late frame sync  */
} ADI_SPORT_CTL_A_LAFS;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_PACK
 *! \brief  Packing Enable (PACK) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_PACK_DIS   = 0,  /**< Disable               */
  SPORT_CTL_A_CTL_PACK_8BIT  = 1,  /**< 8-bit packing enable  */
  SPORT_CTL_A_CTL_PACK_16BIT = 2   /**< 16-bit packing enable */
} ADI_SPORT_CTL_A_PACK;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_GCLKEN
 *! \brief  Gated Clock Enable (GCLKEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_GCLK_DIS = 0,  /**< Disable */
  SPORT_CTL_A_CTL_GCLK_EN  = 1   /**< Enable  */
} ADI_SPORT_CTL_A_GCLKEN;


/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_A_SPTRAN
 *! \brief  Serial Port Transfer Direction (SPTRAN) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_A_CTL_RX = 0,  /**< Receive  */
  SPORT_CTL_A_CTL_TX = 1   /**< Transmit */
} ADI_SPORT_CTL_A_SPTRAN;


/* ==========================================================================
 *! \struct ADI_SPORT_CTL_A_Struct
 *! \brief  Half SPORT 'A' Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_CTL_A_t__
typedef struct _ADI_SPORT_CTL_A_t {
  union {
    struct {
      unsigned int SPEN       :  1;  /**< Serial Port Enable */
      unsigned int FSMUXSEL   :  1;  /**< Frame Sync Multiplexer Select */
      unsigned int CKMUXSEL   :  1;  /**< Clock Multiplexer Select */
      unsigned int LSBF       :  1;  /**< Least-Significant Bit First */
      unsigned int SLEN       :  5;  /**< Serial Word Length */
      unsigned int reserved9  :  1;
      unsigned int ICLK       :  1;  /**< Internal Clock */
      unsigned int OPMODE     :  1;  /**< Operation mode */
      unsigned int CKRE       :  1;  /**< Clock Rising Edge */
      unsigned int FSR        :  1;  /**< Frame Sync Required */
      unsigned int IFS        :  1;  /**< Internal Frame Sync */
      unsigned int DIFS       :  1;  /**< Data-Independent Frame Sync */
      unsigned int LFS        :  1;  /**< Active-Low Frame Sync */
      unsigned int LAFS       :  1;  /**< Late Frame Sync */
      unsigned int PACK       :  2;  /**< Packing Enable */
      unsigned int FSERRMODE  :  1;  /**< Frame Sync Error Operation */
      unsigned int GCLKEN     :  1;  /**< Gated Clock Enable */
      unsigned int reserved22 :  3;
      unsigned int SPTRAN     :  1;  /**< Serial Port Transfer Direction */
      unsigned int DMAEN      :  1;  /**< DMA Enable */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_CTL_A_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_CTL_A_t__ */

/*@}*/

/** @defgroup DIV_A Half SPORT 'A' Divisor Register (DIV_A) Register
 *  Half SPORT 'A' Divisor Register (DIV_A) Register.
 *
 *  The DIV_A contains divisor values that determine frequencies of internally-generated clocks and frame syncs for half SPORT  'A'.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_DIV_A_Struct
 *! \brief  Half SPORT 'A' Divisor Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_DIV_A_t__
typedef struct _ADI_SPORT_DIV_A_t {
  union {
    struct {
      unsigned int CLKDIV     : 16;  /**< Clock Divisor */
      unsigned int FSDIV      :  8;  /**< Frame Sync Divisor */
      unsigned int reserved24 :  8;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_DIV_A_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_DIV_A_t__ */

/*@}*/

/** @defgroup IEN_A Half SPORT A's Interrupt Enable register (IEN_A) Register
 *  Half SPORT A's Interrupt Enable register (IEN_A) Register.
 *
 *  This register contains all the fields related to the Enable given for the various interrupts related to errors and data requests present in the  half SPORT A.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_SPORT_IEN_A_TF
 *! \brief  Transfer Finish Interrupt Enable (TF) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_IEN_A_CTL_TXFIN_DIS = 0,  /**< Transfer finish Interrupt is disabled */
  SPORT_IEN_A_CTL_TXFIN_EN  = 1   /**< Transfer Finish Interrupt is Enabled  */
} ADI_SPORT_IEN_A_TF;


/* ==========================================================================
 *! \struct ADI_SPORT_IEN_A_Struct
 *! \brief  Half SPORT A's Interrupt Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_IEN_A_t__
typedef struct _ADI_SPORT_IEN_A_t {
  union {
    struct {
      unsigned int TF         :  1;  /**< Transfer Finish Interrupt Enable */
      unsigned int DERRMSK    :  1;  /**< Data Error (Interrupt) Mask */
      unsigned int FSERRMSK   :  1;  /**< Frame Sync Error (Interrupt) Mask */
      unsigned int DATA       :  1;  /**< Data request interrupt to the core */
      unsigned int SYSDATERR  :  1;  /**< Data error for system writes or reads */
      unsigned int reserved5  : 27;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_IEN_A_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_IEN_A_t__ */

/*@}*/

/** @defgroup STAT_A Half SPORT 'A' Status register (STAT_A) Register
 *  Half SPORT 'A' Status register (STAT_A) Register.
 *
 *  This register contains all the status fields in the half SPORT A. Detected errors are frame sync violations or buffer over/underflow conditions.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_SPORT_STAT_A_DXS
 *! \brief  Data Transfer Buffer Status (DXS) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_STAT_A_CTL_EMPTY     = 0,  /**< Empty          */
  SPORT_STAT_A_CTL_PART_FULL = 2,  /**< Partially full */
  SPORT_STAT_A_CTL_FULL      = 3   /**< Full           */
} ADI_SPORT_STAT_A_DXS;


/* ==========================================================================
 *! \struct ADI_SPORT_STAT_A_Struct
 *! \brief  Half SPORT 'A' Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_STAT_A_t__
typedef struct _ADI_SPORT_STAT_A_t {
  union {
    struct {
      unsigned int TFI        :  1;  /**< Transmit Finish Interrupt Status */
      unsigned int DERR       :  1;  /**< Data Error Status */
      unsigned int FSERR      :  1;  /**< Frame Sync Error Status */
      unsigned int DATA       :  1;  /**< Data Buffer status */
      unsigned int SYSDATERR  :  1;  /**< System Data Error Status */
      unsigned int reserved5  :  3;
      unsigned int DXS        :  2;  /**< Data Transfer Buffer Status */
      unsigned int reserved10 : 22;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_STAT_A_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_STAT_A_t__ */

/*@}*/

/** @defgroup NUMTRAN_A Half SPORT A Number of transfers register (NUMTRAN_A) Register
 *  Half SPORT A Number of transfers register (NUMTRAN_A) Register.
 *
 *  This register specifies the number of transfers of words to transfer or receive depending on CTL_A:SPTRAN.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_NUMTRAN_A_Struct
 *! \brief  Half SPORT A Number of transfers Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_NUMTRAN_A_t__
typedef struct _ADI_SPORT_NUMTRAN_A_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< Number of transfers (Half SPORT A) */
      unsigned int reserved12 : 20;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_NUMTRAN_A_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_NUMTRAN_A_t__ */

/*@}*/

/** @defgroup CNVT_A Half SPORT 'A' CONVT width (CNVT_A) Register
 *  Half SPORT 'A' CONVT width (CNVT_A) Register.
 *
 *  This register contains the settings related to the CONVT signal for Half SPORT A
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_CNVT_A_Struct
 *! \brief  Half SPORT 'A' CONVT width Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_CNVT_A_t__
typedef struct _ADI_SPORT_CNVT_A_t {
  union {
    struct {
      unsigned int WID        :  4;  /**< CONVT signal width: Half SPORT A */
      unsigned int reserved4  :  4;
      unsigned int POL        :  1;  /**< Polarity of the Convt signal */
      unsigned int reserved9  :  7;
      unsigned int CNVT2FS    :  8;  /**< CONVT to FS duration: Half SPORT A */
      unsigned int reserved24 :  8;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_CNVT_A_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_CNVT_A_t__ */

/*@}*/

/** @defgroup TX_A Half SPORT 'A' Tx Buffer Register (TX_A) Register
 *  Half SPORT 'A' Tx Buffer Register (TX_A) Register.
 *
 *  The TX_A register buffers the half SPORT's transmit data. This register must be loaded with the data to be transmitted if the half SPORT is configured to transmit. Either a program running on the processor core may load the data into the buffer (word-by-word process) or the DMA controller may automatically load the data into the buffer (DMA process).
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_TX_A_Struct
 *! \brief  Half SPORT 'A' Tx Buffer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_TX_A_t__
typedef struct _ADI_SPORT_TX_A_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Transmit Buffer */
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_TX_A_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_TX_A_t__ */

/*@}*/

/** @defgroup RX_A Half SPORT 'A' Rx Buffer Register (RX_A) Register
 *  Half SPORT 'A' Rx Buffer Register (RX_A) Register.
 *
 *  The RX_A register buffers the half SPORT's receive data. This buffer becomes active when the half SPORT is configured to receive data. After a complete word has been received in receive shifter, it is placed into the RX_A register. This data can be read in core mode (in interrupt-based or polling-based mechanism) or directly DMA'd into processor memory using DMA controller.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_RX_A_Struct
 *! \brief  Half SPORT 'A' Rx Buffer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_RX_A_t__
typedef struct _ADI_SPORT_RX_A_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Receive Buffer */
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_RX_A_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_RX_A_t__ */

/*@}*/

/** @defgroup CTL_B Half SPORT 'B' Control Register (CTL_B) Register
 *  Half SPORT 'B' Control Register (CTL_B) Register.
 *
 *  The CTL_B contains transmit and receive control bits for SPORT half 'B', including serial port mode selection for the channels. The function of some bits in CTL_B vary, depending on the SPORT's operating mode. For more information, see the SPORT operating modes description. If reading reserved bits, the read value is the last written value to these bits or is the reset value of these bits.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_SPORT_CTL_B_PACK
 *! \brief  Packing Enable (PACK) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_CTL_B_CTL_PACK_DIS   = 0,  /**< Disable               */
  SPORT_CTL_B_CTL_PACK_8BIT  = 1,  /**< 8-bit packing enable  */
  SPORT_CTL_B_CTL_PACK_16BIT = 2   /**< 16-bit packing enable */
} ADI_SPORT_CTL_B_PACK;


/* ==========================================================================
 *! \struct ADI_SPORT_CTL_B_Struct
 *! \brief  Half SPORT 'B' Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_CTL_B_t__
typedef struct _ADI_SPORT_CTL_B_t {
  union {
    struct {
      unsigned int SPEN       :  1;  /**< Serial Port Enable */
      unsigned int reserved1  :  2;
      unsigned int LSBF       :  1;  /**< Least-Significant Bit First */
      unsigned int SLEN       :  5;  /**< Serial Word Length */
      unsigned int reserved9  :  1;
      unsigned int ICLK       :  1;  /**< Internal Clock */
      unsigned int OPMODE     :  1;  /**< Operation mode */
      unsigned int CKRE       :  1;  /**< Clock Rising Edge */
      unsigned int FSR        :  1;  /**< Frame Sync Required */
      unsigned int IFS        :  1;  /**< Internal Frame Sync */
      unsigned int DIFS       :  1;  /**< Data-Independent Frame Sync */
      unsigned int LFS        :  1;  /**< Active-Low Frame Sync */
      unsigned int LAFS       :  1;  /**< Late Frame Sync */
      unsigned int PACK       :  2;  /**< Packing Enable */
      unsigned int FSERRMODE  :  1;  /**< Frame Sync Error Operation */
      unsigned int GCLKEN     :  1;  /**< Gated Clock Enable */
      unsigned int reserved22 :  3;
      unsigned int SPTRAN     :  1;  /**< Serial Port Transfer Direction */
      unsigned int DMAEN      :  1;  /**< DMA Enable */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_CTL_B_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_CTL_B_t__ */

/*@}*/

/** @defgroup DIV_B Half SPORT 'B' Divisor Register (DIV_B) Register
 *  Half SPORT 'B' Divisor Register (DIV_B) Register.
 *
 *  The DIV_B contains divisor values that determine frequencies of internally-generated clocks and frame syncs for SPORT half 'B'.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_DIV_B_Struct
 *! \brief  Half SPORT 'B' Divisor Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_DIV_B_t__
typedef struct _ADI_SPORT_DIV_B_t {
  union {
    struct {
      unsigned int CLKDIV     : 16;  /**< Clock Divisor */
      unsigned int FSDIV      :  8;  /**< Frame Sync Divisor */
      unsigned int reserved24 :  8;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_DIV_B_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_DIV_B_t__ */

/*@}*/

/** @defgroup IEN_B Half SPORT B's Interrupt Enable register (IEN_B) Register
 *  Half SPORT B's Interrupt Enable register (IEN_B) Register.
 *
 *  This register contains all the fields related to the Enable given for the various interrupts related to errors and data requests present in the half SPORT B.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_SPORT_IEN_B_TF
 *! \brief  Transmit Finish Interrupt Enable (TF) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_IEN_B_CTL_TXFIN_DIS = 0,  /**< Transfer Finish Interrupt is disabled */
  SPORT_IEN_B_CTL_TXFIN_EN  = 1   /**< Transfer Finish Interrupt is Enabled  */
} ADI_SPORT_IEN_B_TF;


/* ==========================================================================
 *! \struct ADI_SPORT_IEN_B_Struct
 *! \brief  Half SPORT B's Interrupt Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_IEN_B_t__
typedef struct _ADI_SPORT_IEN_B_t {
  union {
    struct {
      unsigned int TF         :  1;  /**< Transmit Finish Interrupt Enable */
      unsigned int DERRMSK    :  1;  /**< Data Error (Interrupt) Mask */
      unsigned int FSERRMSK   :  1;  /**< Frame Sync Error (Interrupt) Mask */
      unsigned int DATA       :  1;  /**< Data request interrupt to the core */
      unsigned int SYSDATERR  :  1;  /**< Data error for system writes or reads */
      unsigned int reserved5  : 27;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_IEN_B_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_IEN_B_t__ */

/*@}*/

/** @defgroup STAT_B Half SPORT 'B' Status register (STAT_B) Register
 *  Half SPORT 'B' Status register (STAT_B) Register.
 *
 *  This register contains all the status fields present in the half SPORT B. Detected errors are frame sync violations or buffer over/underflow conditions.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_SPORT_STAT_B_DXS
 *! \brief  Data Transfer Buffer Status (DXS) Enumerations
 *  ========================================================================= */
typedef enum
{
  SPORT_STAT_B_CTL_EMPTY     = 0,  /**< Empty          */
  SPORT_STAT_B_CTL_PART_FULL = 2,  /**< Partially full */
  SPORT_STAT_B_CTL_FULL      = 3   /**< Full           */
} ADI_SPORT_STAT_B_DXS;


/* ==========================================================================
 *! \struct ADI_SPORT_STAT_B_Struct
 *! \brief  Half SPORT 'B' Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_STAT_B_t__
typedef struct _ADI_SPORT_STAT_B_t {
  union {
    struct {
      unsigned int TFI        :  1;  /**< Transmit Finish Interrupt Status */
      unsigned int DERR       :  1;  /**< Data Error Status */
      unsigned int FSERR      :  1;  /**< Frame Sync Error Status */
      unsigned int DATA       :  1;  /**< Data Buffer status */
      unsigned int SYSDATERR  :  1;  /**< System Data Error Status */
      unsigned int reserved5  :  3;
      unsigned int DXS        :  2;  /**< Data Transfer Buffer Status */
      unsigned int reserved10 : 22;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_STAT_B_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_STAT_B_t__ */

/*@}*/

/** @defgroup NUMTRAN_B Half SPORT B Number of transfers register (NUMTRAN_B) Register
 *  Half SPORT B Number of transfers register (NUMTRAN_B) Register.
 *
 *  This register specifies the number of transfers of the words to transfer or receive depending on CTL_B:SPTRAN.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_NUMTRAN_B_Struct
 *! \brief  Half SPORT B Number of transfers Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_NUMTRAN_B_t__
typedef struct _ADI_SPORT_NUMTRAN_B_t {
  union {
    struct {
      unsigned int VALUE      : 12;  /**< Number of transfers (Half SPORT A) */
      unsigned int reserved12 : 20;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_NUMTRAN_B_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_NUMTRAN_B_t__ */

/*@}*/

/** @defgroup CNVT_B Half SPORT 'B' CONVT width register (CNVT_B) Register
 *  Half SPORT 'B' CONVT width register (CNVT_B) Register.
 *
 *  This register contains the settings related to the CONVT signal for Half SPORT B
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_CNVT_B_Struct
 *! \brief  Half SPORT 'B' CONVT width Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_CNVT_B_t__
typedef struct _ADI_SPORT_CNVT_B_t {
  union {
    struct {
      unsigned int WID        :  4;  /**< CONVT signal width: Half SPORT B */
      unsigned int reserved4  :  4;
      unsigned int POL        :  1;  /**< Polarity of the Convt signal */
      unsigned int reserved9  :  7;
      unsigned int CNVT2FS    :  8;  /**< CONVT to FS duration: Half SPORT B */
      unsigned int reserved24 :  8;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_CNVT_B_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_CNVT_B_t__ */

/*@}*/

/** @defgroup TX_B Half SPORT 'B' Tx Buffer Register (TX_B) Register
 *  Half SPORT 'B' Tx Buffer Register (TX_B) Register.
 *
 *  The TX_B register buffers the half SPORT's channel transmit data. This register must be loaded with the data to be transmitted. Either a program running on the processor core may load the data into the buffer (word-by-word process) or the DMA controller may automatically load the data into the buffer (DMA process).
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_TX_B_Struct
 *! \brief  Half SPORT 'B' Tx Buffer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_TX_B_t__
typedef struct _ADI_SPORT_TX_B_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Transmit Buffer */
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_TX_B_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_TX_B_t__ */

/*@}*/

/** @defgroup RX_B Half SPORT 'B' Rx Buffer Register (RX_B) Register
 *  Half SPORT 'B' Rx Buffer Register (RX_B) Register.
 *
 *  The RX_B register buffers the half SPORT's channel receive data. This buffer becomes active when the half SPORT is configured to receive data. After a complete word has been received in receive shifter, it is placed into the RX_B register. This data can be read in core mode or directly DMA'd into processor memory using DMA controller.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_RX_B_Struct
 *! \brief  Half SPORT 'B' Rx Buffer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_RX_B_t__
typedef struct _ADI_SPORT_RX_B_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Receive Buffer */
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_RX_B_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_RX_B_t__ */

/*@}*/

/** @defgroup CTL CRC Control Register (CTL) Register
 *  CRC Control Register (CTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRC_CTL_Struct
 *! \brief  CRC Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRC_CTL_t__
typedef struct _ADI_CRC_CTL_t {
  union {
    struct {
      unsigned int EN         :  1;  /**< CRC Peripheral Enable */
      unsigned int LSBFIRST   :  1;  /**< LSB First Calculation Order */
      unsigned int BITMIRR    :  1;  /**< Bit Mirroring */
      unsigned int BYTMIRR    :  1;  /**< Byte Mirroring */
      unsigned int W16SWP     :  1;  /**< Word16 Swap */
      unsigned int reserved5  : 23;
      unsigned int RevID      :  4;  /**< Revision ID */
    };
    uint32_t VALUE32;
  };
} ADI_CRC_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRC_CTL_t__ */

/*@}*/

/** @defgroup IPDATA Input Data Word Register (IPDATA) Register
 *  Input Data Word Register (IPDATA) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRC_IPDATA_Struct
 *! \brief  Input Data Word Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRC_IPDATA_t__
typedef struct _ADI_CRC_IPDATA_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Data Input */
    };
    uint32_t VALUE32;
  };
} ADI_CRC_IPDATA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRC_IPDATA_t__ */

/*@}*/

/** @defgroup RESULT CRC Result Register (RESULT) Register
 *  CRC Result Register (RESULT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRC_RESULT_Struct
 *! \brief  CRC Result Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRC_RESULT_t__
typedef struct _ADI_CRC_RESULT_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< CRC Residue */
    };
    uint32_t VALUE32;
  };
} ADI_CRC_RESULT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRC_RESULT_t__ */

/*@}*/

/** @defgroup POLY Programmable CRC Polynomial (POLY) Register
 *  Programmable CRC Polynomial (POLY) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRC_POLY_Struct
 *! \brief  Programmable CRC Polynomial Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRC_POLY_t__
typedef struct _ADI_CRC_POLY_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< CRC Reduction Polynomial */
    };
    uint32_t VALUE32;
  };
} ADI_CRC_POLY_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRC_POLY_t__ */

/*@}*/

/** @defgroup IPBITS Input Data Bits (IPBITS) Register
 *  Input Data Bits (IPBITS) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRC_IPBITS_Struct
 *! \brief  Input Data Bits Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRC_IPBITS_t__
typedef struct _ADI_CRC_IPBITS_t {
  union {
    struct {
      unsigned int DATA_BITS  :  8;  /**< Input Data Bits */
    };
    uint8_t VALUE8;
  };
} ADI_CRC_IPBITS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRC_IPBITS_t__ */

/*@}*/

/** @defgroup IPBYTE Input Data Byte (IPBYTE) Register
 *  Input Data Byte (IPBYTE) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRC_IPBYTE_Struct
 *! \brief  Input Data Byte Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRC_IPBYTE_t__
typedef struct _ADI_CRC_IPBYTE_t {
  union {
    struct {
      unsigned int DATA_BYTE  :  8;  /**< Input Data Byte */
    };
    uint8_t VALUE8;
  };
} ADI_CRC_IPBYTE_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRC_IPBYTE_t__ */

/*@}*/

/** @defgroup CTL RNG Control Register (CTL) Register
 *  RNG Control Register (CTL) Register.
 *
 *  The CTL register is used to enable the random number generator.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_RNG_CTL_EN
 *! \brief  RNG Enable (EN) Enumerations
 *  ========================================================================= */
typedef enum
{
  RNG_CTL_DISABLE = 0,  /**< Disable the RNG */
  RNG_CTL_ENABLE  = 1   /**< Enable the RNG  */
} ADI_RNG_CTL_EN;


/*  =========================================================================
 *! \enum   ADI_RNG_CTL_SINGLE
 *! \brief  Generate a single number (SINGLE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RNG_CTL_WORD   = 0,  /**< Buffer Word */
  RNG_CTL_SINGLE = 1   /**< Single Byte */
} ADI_RNG_CTL_SINGLE;


/* ==========================================================================
 *! \struct ADI_RNG_CTL_Struct
 *! \brief  RNG Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RNG_CTL_t__
typedef struct _ADI_RNG_CTL_t {
  union {
    struct {
      unsigned int EN         :  1;  /**< RNG Enable */
      unsigned int reserved1  :  2;
      unsigned int SINGLE     :  1;  /**< Generate a single number */
      unsigned int reserved4  : 12;
    };
    uint16_t VALUE16;
  };
} ADI_RNG_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_CTL_t__ */

/*@}*/

/** @defgroup LEN RNG Sample Length Register (LEN) Register
 *  RNG Sample Length Register (LEN) Register.
 *
 *  The LEN register defines the number of samples to accumulate in the CRC register when generating a random number. The number of samples accumulated is RLD scaled by 2^PRE.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RNG_LEN_Struct
 *! \brief  RNG Sample Length Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RNG_LEN_t__
typedef struct _ADI_RNG_LEN_t {
  union {
    struct {
      unsigned int RELOAD     : 12;  /**< Reload value for the sample counter */
      unsigned int PRESCALE   :  4;  /**< Prescaler for the sample counter */
    };
    uint16_t VALUE16;
  };
} ADI_RNG_LEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_LEN_t__ */

/*@}*/

/** @defgroup STAT RNG Status Register (STAT) Register
 *  RNG Status Register (STAT) Register.
 *
 *  The STAT register indicates when the RNG has finished generating a random number.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RNG_STAT_Struct
 *! \brief  RNG Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RNG_STAT_t__
typedef struct _ADI_RNG_STAT_t {
  union {
    struct {
      unsigned int RNRDY      :  1;  /**< Random number ready */
      unsigned int STUCK      :  1;  /**< Sampled data stuck high or low */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_RNG_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_STAT_t__ */

/*@}*/

/** @defgroup DATA RNG Data Register (DATA) Register
 *  RNG Data Register (DATA) Register.
 *
 *  DATA register provides the CPU with read-only access of the entropy accumulator (8-bit CRC) and data buffer. When the data buffer is not enabled, an 8-bit result is provided. When the data buffer is enabled, 32-bits (four 8-bit values) are provided. The contents of this register are valid when the STAT:RDY bit is set. This register is reset when the STAT:RDY bit is cleared. The STAT:RDY bit is automatically cleared when this register is read and the CPU is not in debug halt. Reading this register by the CPU when CTL:EN is set will cause a new random number to be generated.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RNG_DATA_Struct
 *! \brief  RNG Data Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RNG_DATA_t__
typedef struct _ADI_RNG_DATA_t {
  union {
    struct {
      unsigned int VALUE      :  8;  /**< Value of the CRC accumulator */
      unsigned int BUFF       : 24;  /**< Buffer for RNG data */
    };
    uint32_t VALUE32;
  };
} ADI_RNG_DATA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_DATA_t__ */

/*@}*/

/** @defgroup OSCCNT Oscillator Count (OSCCNT) Register
 *  Oscillator Count (OSCCNT) Register.
 *
 *  The oscillator counter counts the number of ring oscillator cycles which occur during the generation of a random number. The oscillator counter is 28-bits. The oscillator counter will saturate at the maximum value to prevent overflow.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RNG_OSCCNT_Struct
 *! \brief  Oscillator Count Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RNG_OSCCNT_t__
typedef struct _ADI_RNG_OSCCNT_t {
  union {
    struct {
      unsigned int VALUE      : 28;  /**< Oscillator count */
      unsigned int reserved28 :  4;
    };
    uint32_t VALUE32;
  };
} ADI_RNG_OSCCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_OSCCNT_t__ */

/*@}*/

/** @defgroup OSCDIFF Oscillator Difference (OSCDIFF) Register
 *  Oscillator Difference (OSCDIFF) Register.
 *
 *  The oscillator difference register stores the difference in OSCCNT from the current value compared to the previous value (OSCCNT[n] - OSCCNT[n-1]). This difference is represented as a signed 8-bit value. It saturates at the maximum and minimum values. This can be used to reconstruct OSCCNT for the values currently in the DATA buffer. This information can be used to compute the OSCCNT variance to check the health of the random number generator and ensure there is adequate entropy.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RNG_OSCDIFF_Struct
 *! \brief  Oscillator Difference Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RNG_OSCDIFF_t__
typedef struct _ADI_RNG_OSCDIFF_t {
  union {
    struct {
      unsigned int DELTA      :  8;  /**< Oscillator Count difference */
    };
    uint8_t VALUE8;
  };
} ADI_RNG_OSCDIFF_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_OSCDIFF_t__ */

/*@}*/

/** @defgroup CFG Configuration Register (CFG) Register
 *  Configuration Register (CFG) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_CRYPT_CFG_BLKEN
 *! \brief  Enable BIT for the Crypto Block (BLKEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRYPT_CFG_ENABLE  = 0,  /**< Enable Crypto Block  */
  CRYPT_CFG_DISABLE = 1   /**< Disable Crypto Block */
} ADI_CRYPT_CFG_BLKEN;


/*  =========================================================================
 *! \enum   ADI_CRYPT_CFG_INDMAEN
 *! \brief  Enable DMA for Input Buffer (INDMAEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRYPT_CFG_DMA_DISABLE_INBUF = 0,  /**< Disable DMA Requesting for Input Buffer */
  CRYPT_CFG_DMA_ENABLE_INBUF  = 1   /**< Enable DMA Requesting for Input Buffer  */
} ADI_CRYPT_CFG_INDMAEN;


/*  =========================================================================
 *! \enum   ADI_CRYPT_CFG_OUTDMAEN
 *! \brief  Enable DMA for Output Buffer (OUTDMAEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRYPT_CFG_DMA_DISABLE_OUTBUF = 0,  /**< Disable DMA Requesting for Output Buffer */
  CRYPT_CFG_DMA_ENABLE_OUTBUF  = 1   /**< Enable DMA Requesting for Output Buffer  */
} ADI_CRYPT_CFG_OUTDMAEN;


/*  =========================================================================
 *! \enum   ADI_CRYPT_CFG_KUWKeyLen
 *! \brief  Key Length  Key Wrap Unwrap (KUWKeyLen) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRYPT_CFG_LEN128 = 1,  /**< The key size of KUW key is 128 bits */
  CRYPT_CFG_LEN256 = 2,  /**< The key size of KUW key is 256 bits */
  CRYPT_CFG_LEN512 = 3   /**< The key size of KUW key is 512 bits */
} ADI_CRYPT_CFG_KUWKeyLen;


/* ==========================================================================
 *! \struct ADI_CRYPT_CFG_Struct
 *! \brief  Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_CFG_t__
typedef struct _ADI_CRYPT_CFG_t {
  union {
    struct {
      unsigned int BLKEN      :  1;  /**< Enable BIT for the Crypto Block */
      unsigned int ENCR       :  1;  /**< Encrypt or Decrypt */
      unsigned int INDMAEN    :  1;  /**< Enable DMA for Input Buffer */
      unsigned int OUTDMAEN   :  1;  /**< Enable DMA for Output Buffer */
      unsigned int INFLUSH    :  1;  /**< Input Buffer Flush */
      unsigned int OUTFLUSH   :  1;  /**< Output Buffer Flush */
      unsigned int reserved6  :  2;
      unsigned int AESKEYLEN  :  2;  /**< Select Key Length for AES Cipher */
      unsigned int KUWKeyLen  :  2;  /**< Key Length  Key Wrap Unwrap */
      unsigned int AES_BYTESWAP :  1;  /**< Byteswap for AES Input */
      unsigned int SHA_BYTESWAP :  1;  /**< Enable Key Wrap */
      unsigned int KEY_BYTESWAP :  1;  /**< Use Key Unwrap Before HMAC */
      unsigned int PRKSTOREN    :  1;  /**< Enable PRKSTOR Commands */
      unsigned int ECBEN        :  1;  /**< Enable ECB Mode Operation */
      unsigned int CTREN        :  1;  /**< Enable CTR Mode Operation */
      unsigned int CBCEN        :  1;  /**< Enable CBC Mode Operation */
      unsigned int CCMEN        :  1;  /**< Enable CCM/CCM* Mode Operation */
      unsigned int CMACEN       :  1;  /**< Enable CMAC Mode Operation */
      unsigned int HMACEN       :  1;  /**< HMAC Enable */
      unsigned int reserved22   :  3;
      unsigned int SHA256EN     :  1;  /**< Enable SHA-256 Operation */
      unsigned int SHAINIT      :  1;  /**< Restarts SHA Computation */
      unsigned int reserved27   :  1;
      unsigned int RevID        :  4;  /**< Rev ID for Crypto */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_CFG_t__ */

/*@}*/

/** @defgroup DATALEN Payload Data Length (DATALEN) Register
 *  Payload Data Length (DATALEN) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_DATALEN_Struct
 *! \brief  Payload Data Length Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_DATALEN_t__
typedef struct _ADI_CRYPT_DATALEN_t {
  union {
    struct {
      unsigned int VALUE      : 20;  /**< Length of Payload Data */
      unsigned int reserved20 : 12;
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_DATALEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_DATALEN_t__ */

/*@}*/

/** @defgroup PREFIXLEN Authentication Data Length (PREFIXLEN) Register
 *  Authentication Data Length (PREFIXLEN) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_PREFIXLEN_Struct
 *! \brief  Authentication Data Length Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_PREFIXLEN_t__
typedef struct _ADI_CRYPT_PREFIXLEN_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Length of Associated Data */
      unsigned int reserved16 : 16;
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_PREFIXLEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_PREFIXLEN_t__ */

/*@}*/

/** @defgroup INTEN Interrupt Enable Register (INTEN) Register
 *  Interrupt Enable Register (INTEN) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_INTEN_Struct
 *! \brief  Interrupt Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_INTEN_t__
typedef struct _ADI_CRYPT_INTEN_t {
  union {
    struct {
      unsigned int INRDYEN    :  1;  /**< Enable Input Ready Interrupt */
      unsigned int OUTRDYEN   :  1;  /**< Enables the Output Ready Interrupt */
      unsigned int INOVREN    :  1;  /**< Enable Input Overflow Interrupt */
      unsigned int reserved3  :  2;
      unsigned int SHADONEN   :  1;  /**< Enable SHA_Done Interrupt */
      unsigned int HMACDONEEN :  1;  /**< Interrupt Enable for HMAC Done */
      unsigned int HMACMSGRDYEN :  1;  /**< Status Bit for HMAC Message Input Ready */
      unsigned int PRKSTRCMDONEEN :  1;  /**< PRKSTOR CMD DONE INTEN */
      unsigned int reserved9      : 23;
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_INTEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_INTEN_t__ */

/*@}*/

/** @defgroup STAT Status Register (STAT) Register
 *  Status Register (STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_STAT_Struct
 *! \brief  Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_STAT_t__
typedef struct _ADI_CRYPT_STAT_t {
  union {
    struct {
      unsigned int INRDY      :  1;  /**< Input Buffer Status */
      unsigned int OUTRDY     :  1;  /**< Output Data Ready */
      unsigned int INOVR      :  1;  /**< Overflow in the Input Buffer */
      unsigned int reserved3  :  2;
      unsigned int SHADONE    :  1;  /**< SHA Computation Complete */
      unsigned int SHABUSY    :  1;  /**< SHA Busy. in Computation */
      unsigned int INWORDS    :  3;  /**< Number of Words in the Input Buffer */
      unsigned int OUTWORDS   :  3;  /**< Number of Words in the Output Buffer */
      unsigned int HMACBUSY   :  1;  /**< Status Bit Indicates HMAC Busy */
      unsigned int HMACDONE   :  1;  /**< Status Bit Indicates HMAC Done */
      unsigned int HMACMSGRDY :  1;  /**< Status Bit Indicates HMAC is Message Ready */
      unsigned int reserved16       :  7;
      unsigned int PRKSTOR_CMD_DONE :  1;  /**< Indicates Command Done for PrKStor */
      unsigned int PRKSTOR_CMD_FAIL :  1;  /**< Indicates Last Command Issued Failed */
      unsigned int PRKSTOR_RET_STATUS :  2;  /**< ECC Errors in the PRKSTOR_RETRIEVE Command */
      unsigned int CMD_ISSUED         :  4;  /**< Last Command Issued to PrKStor; */
      unsigned int PRKSTOR_BUSY       :  1;  /**< Indicates PrKSTOR is Busy */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_STAT_t__ */

/*@}*/

/** @defgroup INBUF Input Buffer (INBUF) Register
 *  Input Buffer (INBUF) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_INBUF_Struct
 *! \brief  Input Buffer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_INBUF_t__
typedef struct _ADI_CRYPT_INBUF_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Input Buffer */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_INBUF_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_INBUF_t__ */

/*@}*/

/** @defgroup OUTBUF Output Buffer (OUTBUF) Register
 *  Output Buffer (OUTBUF) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_OUTBUF_Struct
 *! \brief  Output Buffer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_OUTBUF_t__
typedef struct _ADI_CRYPT_OUTBUF_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Output Buffer */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_OUTBUF_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_OUTBUF_t__ */

/*@}*/

/** @defgroup NONCE0 Nonce Bits [31:0] (NONCE0) Register
 *  Nonce Bits [31:0] (NONCE0) Register.
 *
 *  Nonce is used in some modes of operations. Depending on the mode, different NONCE lengths will be used.
 *  1. CTR Mode: This takes a 108-Bit Nonce. This nonce is formed as follows - {NONCE3[11:0], NONCE2, NONCE1, NONCE0}
 *  2. CBC Mode: This takes a 128-Bit Nonce. This nonce is formed as follows - {NONCE3, NONCE2, NONCE1, NONCE0}
 *  3. CTR Mode: This takes a 108-Bit Nonce. This nonce is formed as follows - {NONCE3[15:0], NONCE2, NONCE1, NONCE0}
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_NONCE0_Struct
 *! \brief  Nonce Bits [31:0] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE0_t__
typedef struct _ADI_CRYPT_NONCE0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Word 0: Nonce : Bits [31:0] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_NONCE0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE0_t__ */

/*@}*/

/** @defgroup NONCE1 Nonce Bits [63:32] (NONCE1) Register
 *  Nonce Bits [63:32] (NONCE1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_NONCE1_Struct
 *! \brief  Nonce Bits [63:32] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE1_t__
typedef struct _ADI_CRYPT_NONCE1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Word 1: Nonce : Bits [63:32] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_NONCE1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE1_t__ */

/*@}*/

/** @defgroup NONCE2 Nonce Bits [95:64] (NONCE2) Register
 *  Nonce Bits [95:64] (NONCE2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_NONCE2_Struct
 *! \brief  Nonce Bits [95:64] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE2_t__
typedef struct _ADI_CRYPT_NONCE2_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Word 2: Nonce : Bits [95:64] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_NONCE2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE2_t__ */

/*@}*/

/** @defgroup NONCE3 Nonce Bits [127:96] (NONCE3) Register
 *  Nonce Bits [127:96] (NONCE3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_NONCE3_Struct
 *! \brief  Nonce Bits [127:96] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE3_t__
typedef struct _ADI_CRYPT_NONCE3_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Word 3: Nonce : Bits [127:96] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_NONCE3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE3_t__ */

/*@}*/

/** @defgroup AESKEY0 AES Key Bits [31:0] (AESKEY0) Register
 *  AES Key Bits [31:0] (AESKEY0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_AESKEY0_Struct
 *! \brief  AES Key Bits [31:0] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY0_t__
typedef struct _ADI_CRYPT_AESKEY0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Key: Bytes [3:0] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_AESKEY0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY0_t__ */

/*@}*/

/** @defgroup AESKEY1 AES Key Bits [63:32] (AESKEY1) Register
 *  AES Key Bits [63:32] (AESKEY1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_AESKEY1_Struct
 *! \brief  AES Key Bits [63:32] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY1_t__
typedef struct _ADI_CRYPT_AESKEY1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Key: Bytes [7:4] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_AESKEY1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY1_t__ */

/*@}*/

/** @defgroup AESKEY2 AES Key Bits [95:64] (AESKEY2) Register
 *  AES Key Bits [95:64] (AESKEY2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_AESKEY2_Struct
 *! \brief  AES Key Bits [95:64] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY2_t__
typedef struct _ADI_CRYPT_AESKEY2_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Key: Bytes [11:8] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_AESKEY2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY2_t__ */

/*@}*/

/** @defgroup AESKEY3 AES Key Bits [127:96] (AESKEY3) Register
 *  AES Key Bits [127:96] (AESKEY3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_AESKEY3_Struct
 *! \brief  AES Key Bits [127:96] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY3_t__
typedef struct _ADI_CRYPT_AESKEY3_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Key: Bytes [15:12] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_AESKEY3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY3_t__ */

/*@}*/

/** @defgroup AESKEY4 AES Key Bits [159:128] (AESKEY4) Register
 *  AES Key Bits [159:128] (AESKEY4) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_AESKEY4_Struct
 *! \brief  AES Key Bits [159:128] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY4_t__
typedef struct _ADI_CRYPT_AESKEY4_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Key: Bytes [19:16] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_AESKEY4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY4_t__ */

/*@}*/

/** @defgroup AESKEY5 AES Key Bits [191:160] (AESKEY5) Register
 *  AES Key Bits [191:160] (AESKEY5) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_AESKEY5_Struct
 *! \brief  AES Key Bits [191:160] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY5_t__
typedef struct _ADI_CRYPT_AESKEY5_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Key: Bytes [23:20] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_AESKEY5_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY5_t__ */

/*@}*/

/** @defgroup AESKEY6 AES Key Bits [223:192] (AESKEY6) Register
 *  AES Key Bits [223:192] (AESKEY6) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_AESKEY6_Struct
 *! \brief  AES Key Bits [223:192] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY6_t__
typedef struct _ADI_CRYPT_AESKEY6_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Key: Bytes [27:24] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_AESKEY6_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY6_t__ */

/*@}*/

/** @defgroup AESKEY7 AES Key Bits [255:224] (AESKEY7) Register
 *  AES Key Bits [255:224] (AESKEY7) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_AESKEY7_Struct
 *! \brief  AES Key Bits [255:224] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY7_t__
typedef struct _ADI_CRYPT_AESKEY7_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Key: Bytes [31:28] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_AESKEY7_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_AESKEY7_t__ */

/*@}*/

/** @defgroup CNTRINIT Counter Initialization Vector (CNTRINIT) Register
 *  Counter Initialization Vector (CNTRINIT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_CNTRINIT_Struct
 *! \brief  Counter Initialization Vector Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_CNTRINIT_t__
typedef struct _ADI_CRYPT_CNTRINIT_t {
  union {
    struct {
      unsigned int VALUE      : 20;  /**< Counter Initialization Value */
      unsigned int reserved20 : 12;
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_CNTRINIT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_CNTRINIT_t__ */

/*@}*/

/** @defgroup SHAH0 SHA Bits [31:0] (SHAH0) Register
 *  SHA Bits [31:0] (SHAH0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_SHAH0_Struct
 *! \brief  SHA Bits [31:0] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH0_t__
typedef struct _ADI_CRYPT_SHAH0_t {
  union {
    struct {
      unsigned int SHAHASH0   : 32;  /**< Word 0: SHA Hash */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_SHAH0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH0_t__ */

/*@}*/

/** @defgroup SHAH1 SHA Bits [63:32] (SHAH1) Register
 *  SHA Bits [63:32] (SHAH1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_SHAH1_Struct
 *! \brief  SHA Bits [63:32] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH1_t__
typedef struct _ADI_CRYPT_SHAH1_t {
  union {
    struct {
      unsigned int SHAHASH1   : 32;  /**< Word 1: SHA Hash */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_SHAH1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH1_t__ */

/*@}*/

/** @defgroup SHAH2 SHA Bits [95:64] (SHAH2) Register
 *  SHA Bits [95:64] (SHAH2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_SHAH2_Struct
 *! \brief  SHA Bits [95:64] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH2_t__
typedef struct _ADI_CRYPT_SHAH2_t {
  union {
    struct {
      unsigned int SHAHASH2   : 32;  /**< Word 2: SHA Hash */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_SHAH2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH2_t__ */

/*@}*/

/** @defgroup SHAH3 SHA Bits [127:96] (SHAH3) Register
 *  SHA Bits [127:96] (SHAH3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_SHAH3_Struct
 *! \brief  SHA Bits [127:96] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH3_t__
typedef struct _ADI_CRYPT_SHAH3_t {
  union {
    struct {
      unsigned int SHAHASH3   : 32;  /**< Word 3: SHA Hash */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_SHAH3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH3_t__ */

/*@}*/

/** @defgroup SHAH4 SHA Bits [159:128] (SHAH4) Register
 *  SHA Bits [159:128] (SHAH4) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_SHAH4_Struct
 *! \brief  SHA Bits [159:128] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH4_t__
typedef struct _ADI_CRYPT_SHAH4_t {
  union {
    struct {
      unsigned int SHAHASH4   : 32;  /**< Word 4: SHA Hash */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_SHAH4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH4_t__ */

/*@}*/

/** @defgroup SHAH5 SHA Bits [191:160] (SHAH5) Register
 *  SHA Bits [191:160] (SHAH5) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_SHAH5_Struct
 *! \brief  SHA Bits [191:160] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH5_t__
typedef struct _ADI_CRYPT_SHAH5_t {
  union {
    struct {
      unsigned int SHAHASH5   : 32;  /**< Word 5: SHA Hash */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_SHAH5_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH5_t__ */

/*@}*/

/** @defgroup SHAH6 SHA Bits [223:192] (SHAH6) Register
 *  SHA Bits [223:192] (SHAH6) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_SHAH6_Struct
 *! \brief  SHA Bits [223:192] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH6_t__
typedef struct _ADI_CRYPT_SHAH6_t {
  union {
    struct {
      unsigned int SHAHASH6   : 32;  /**< Word 6: SHA Hash */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_SHAH6_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH6_t__ */

/*@}*/

/** @defgroup SHAH7 SHA Bits [255:224] (SHAH7) Register
 *  SHA Bits [255:224] (SHAH7) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_SHAH7_Struct
 *! \brief  SHA Bits [255:224] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH7_t__
typedef struct _ADI_CRYPT_SHAH7_t {
  union {
    struct {
      unsigned int SHAHASH7   : 32;  /**< Word 7: SHA Hash */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_SHAH7_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_SHAH7_t__ */

/*@}*/

/** @defgroup SHA_LAST_WORD SHA Last Word and Valid Bits Information (SHA_LAST_WORD) Register
 *  SHA Last Word and Valid Bits Information (SHA_LAST_WORD) Register.
 *
 *  This register is to be written before writing the last word to SHA input register. This is to inform the SHA engine that last word is about to be written by writing to the O_Last_Word. Also, the number of valid bits has to be programmed in the O_Bits_Valid.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_SHA_LAST_WORD_Struct
 *! \brief  SHA Last Word and Valid Bits Information Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_SHA_LAST_WORD_t__
typedef struct _ADI_CRYPT_SHA_LAST_WORD_t {
  union {
    struct {
      unsigned int O_Last_Word :  1;  /**< Last SHA Input Word */
      unsigned int O_Bits_Valid :  5;  /**< Bits Valid in SHA Last Word Input */
      unsigned int reserved6    : 26;
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_SHA_LAST_WORD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_SHA_LAST_WORD_t__ */

/*@}*/

/** @defgroup CCM_NUM_VALID_BYTES NUM_VALID_BYTES (CCM_NUM_VALID_BYTES) Register
 *  NUM_VALID_BYTES (CCM_NUM_VALID_BYTES) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_CCM_NUM_VALID_BYTES_Struct
 *! \brief  NUM_VALID_BYTES Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_CCM_NUM_VALID_BYTES_t__
typedef struct _ADI_CRYPT_CCM_NUM_VALID_BYTES_t {
  union {
    struct {
      unsigned int NUM_VALID_BYTES :  4;  /**< Number of Valid Bytes in CCM Last Data */
      unsigned int reserved4       : 28;
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_CCM_NUM_VALID_BYTES_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_CCM_NUM_VALID_BYTES_t__ */

/*@}*/

/** @defgroup PRKSTORCFG PRKSTOR Configuration (PRKSTORCFG) Register
 *  PRKSTOR Configuration (PRKSTORCFG) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_PRKSTORCFG_Struct
 *! \brief  PRKSTOR Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_PRKSTORCFG_t__
typedef struct _ADI_CRYPT_PRKSTORCFG_t {
  union {
    struct {
      unsigned int KEY_INDEX  :  7;  /**< Index of Key in PRKSTOR */
      unsigned int CMD        :  4;  /**< Command Input for PRKSTOR */
      unsigned int reserved11 : 21;
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_PRKSTORCFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_PRKSTORCFG_t__ */

/*@}*/

/** @defgroup KUW0 Key Wrap Unwrap Register 0 (KUW0) Register
 *  Key Wrap Unwrap Register 0 (KUW0) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW0_Struct
 *! \brief  Key Wrap Unwrap Register 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW0_t__
typedef struct _ADI_CRYPT_KUW0_t {
  union {
    struct {
      unsigned int KUW0       : 32;  /**< KUW[31:0] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW0_t__ */

/*@}*/

/** @defgroup KUW1 Key Wrap Unwrap Register 1 (KUW1) Register
 *  Key Wrap Unwrap Register 1 (KUW1) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW1_Struct
 *! \brief  Key Wrap Unwrap Register 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW1_t__
typedef struct _ADI_CRYPT_KUW1_t {
  union {
    struct {
      unsigned int KUW1       : 32;  /**< KUW[63:32] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW1_t__ */

/*@}*/

/** @defgroup KUW2 Key Wrap Unwrap Register 2 (KUW2) Register
 *  Key Wrap Unwrap Register 2 (KUW2) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW2_Struct
 *! \brief  Key Wrap Unwrap Register 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW2_t__
typedef struct _ADI_CRYPT_KUW2_t {
  union {
    struct {
      unsigned int KUW2       : 32;  /**< KUW[95:64] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW2_t__ */

/*@}*/

/** @defgroup KUW3 Key Wrap Unwrap Register 3 (KUW3) Register
 *  Key Wrap Unwrap Register 3 (KUW3) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW3_Struct
 *! \brief  Key Wrap Unwrap Register 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW3_t__
typedef struct _ADI_CRYPT_KUW3_t {
  union {
    struct {
      unsigned int KUW3       : 32;  /**< KUW[127:96] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW3_t__ */

/*@}*/

/** @defgroup KUW4 Key Wrap Unwrap Register 4 (KUW4) Register
 *  Key Wrap Unwrap Register 4 (KUW4) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW4_Struct
 *! \brief  Key Wrap Unwrap Register 4 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW4_t__
typedef struct _ADI_CRYPT_KUW4_t {
  union {
    struct {
      unsigned int KUW4       : 32;  /**< KUW[159:128] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW4_t__ */

/*@}*/

/** @defgroup KUW5 Key Wrap Unwrap Register 5 (KUW5) Register
 *  Key Wrap Unwrap Register 5 (KUW5) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW5_Struct
 *! \brief  Key Wrap Unwrap Register 5 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW5_t__
typedef struct _ADI_CRYPT_KUW5_t {
  union {
    struct {
      unsigned int KUW5       : 32;  /**< KUW[191:160] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW5_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW5_t__ */

/*@}*/

/** @defgroup KUW6 Key Wrap Unwrap Register 6 (KUW6) Register
 *  Key Wrap Unwrap Register 6 (KUW6) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW6_Struct
 *! \brief  Key Wrap Unwrap Register 6 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW6_t__
typedef struct _ADI_CRYPT_KUW6_t {
  union {
    struct {
      unsigned int KUW6       : 32;  /**< KUW[223:192] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW6_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW6_t__ */

/*@}*/

/** @defgroup KUW7 Key Wrap Unwrap Register 7 (KUW7) Register
 *  Key Wrap Unwrap Register 7 (KUW7) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW7_Struct
 *! \brief  Key Wrap Unwrap Register 7 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW7_t__
typedef struct _ADI_CRYPT_KUW7_t {
  union {
    struct {
      unsigned int KUW7       : 32;  /**< KUW[255:224] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW7_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW7_t__ */

/*@}*/

/** @defgroup KUW8 Key Wrap Unwrap Register 8 (KUW8) Register
 *  Key Wrap Unwrap Register 8 (KUW8) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW8_Struct
 *! \brief  Key Wrap Unwrap Register 8 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW8_t__
typedef struct _ADI_CRYPT_KUW8_t {
  union {
    struct {
      unsigned int KUW8       : 32;  /**< KUW[287:256] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW8_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW8_t__ */

/*@}*/

/** @defgroup KUW9 Key Wrap Unwrap Register 9 (KUW9) Register
 *  Key Wrap Unwrap Register 9 (KUW9) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW9_Struct
 *! \brief  Key Wrap Unwrap Register 9 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW9_t__
typedef struct _ADI_CRYPT_KUW9_t {
  union {
    struct {
      unsigned int KUW9       : 32;  /**< KUW[319:288] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW9_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW9_t__ */

/*@}*/

/** @defgroup KUW10 Key Wrap Unwrap Register 10 (KUW10) Register
 *  Key Wrap Unwrap Register 10 (KUW10) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW10_Struct
 *! \brief  Key Wrap Unwrap Register 10 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW10_t__
typedef struct _ADI_CRYPT_KUW10_t {
  union {
    struct {
      unsigned int KUW10      : 32;  /**< KUW[351:320] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW10_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW10_t__ */

/*@}*/

/** @defgroup KUW11 Key Wrap Unwrap Register 11 (KUW11) Register
 *  Key Wrap Unwrap Register 11 (KUW11) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW11_Struct
 *! \brief  Key Wrap Unwrap Register 11 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW11_t__
typedef struct _ADI_CRYPT_KUW11_t {
  union {
    struct {
      unsigned int KUW11      : 32;  /**< KUW[383:352] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW11_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW11_t__ */

/*@}*/

/** @defgroup KUW12 Key Wrap Unwrap Register 12 (KUW12) Register
 *  Key Wrap Unwrap Register 12 (KUW12) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW12_Struct
 *! \brief  Key Wrap Unwrap Register 12 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW12_t__
typedef struct _ADI_CRYPT_KUW12_t {
  union {
    struct {
      unsigned int KUW12      : 32;  /**< KUW[415:384] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW12_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW12_t__ */

/*@}*/

/** @defgroup KUW13 Key Wrap Unwrap Register 13 (KUW13) Register
 *  Key Wrap Unwrap Register 13 (KUW13) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW13_Struct
 *! \brief  Key Wrap Unwrap Register 13 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW13_t__
typedef struct _ADI_CRYPT_KUW13_t {
  union {
    struct {
      unsigned int KUW13      : 32;  /**< KUW[447:416] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW13_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW13_t__ */

/*@}*/

/** @defgroup KUW14 Key Wrap Unwrap Register 14 (KUW14) Register
 *  Key Wrap Unwrap Register 14 (KUW14) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW14_Struct
 *! \brief  Key Wrap Unwrap Register 14 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW14_t__
typedef struct _ADI_CRYPT_KUW14_t {
  union {
    struct {
      unsigned int KUW14      : 32;  /**< KUW[479:448] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW14_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW14_t__ */

/*@}*/

/** @defgroup KUW15 Key Wrap Unwrap Register 15 (KUW15) Register
 *  Key Wrap Unwrap Register 15 (KUW15) Register.
 *
 *  This register holds part of the wrapped or unwrapped key in respective modes.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUW15_Struct
 *! \brief  Key Wrap Unwrap Register 15 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW15_t__
typedef struct _ADI_CRYPT_KUW15_t {
  union {
    struct {
      unsigned int KUW15      : 32;  /**< KUW[511:480] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW15_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW15_t__ */

/*@}*/

/** @defgroup KUWValStr1 Key Wrap Unwrap Validation String[63:32] (KUWValStr1) Register
 *  Key Wrap Unwrap Validation String[63:32] (KUWValStr1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUWValStr1_Struct
 *! \brief  Key Wrap Unwrap Validation String[63:32] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUWValStr1_t__
typedef struct _ADI_CRYPT_KUWValStr1_t {
  union {
    struct {
      unsigned int InitalValue0 : 32;  /**< Initial Value */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUWValStr1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUWValStr1_t__ */

/*@}*/

/** @defgroup KUWValStr2 Key Wrap Unwrap Validation String[31:0] (KUWValStr2) Register
 *  Key Wrap Unwrap Validation String[31:0] (KUWValStr2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUWValStr2_Struct
 *! \brief  Key Wrap Unwrap Validation String[31:0] Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_KUWValStr2_t__
typedef struct _ADI_CRYPT_KUWValStr2_t {
  union {
    struct {
      unsigned int InitialValue1 : 32;  /**< Initial Value */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUWValStr2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUWValStr2_t__ */

/*@}*/

/** @defgroup IEN Power Supply Monitor Interrupt Enable (IEN) Register
 *  Power Supply Monitor Interrupt Enable (IEN) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_PMG_IEN_RANGEBAT
 *! \brief  Battery Monitor Range (RANGEBAT) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_IEN_REGION1 = 0,  /**< Configure to generate interrupt if VBAT in Region1 */
  PMG_IEN_REGION2 = 1,  /**< Configure to generate interrupt if VBAT in Region2 */
  PMG_IEN_REGION3 = 2,  /**< Configure to generate interrupt if VBAT in Region3 */
  PMG_IEN_NA      = 3   /**< N/A                                                */
} ADI_PMG_IEN_RANGEBAT;


/* ==========================================================================
 *! \struct ADI_PMG_IEN_Struct
 *! \brief  Power Supply Monitor Interrupt Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_IEN_t__
typedef struct _ADI_PMG_IEN_t {
  union {
    struct {
      unsigned int VBAT       :  1;  /**< Enable Interrupt for VBAT */
      unsigned int VREGUNDR   :  1;  /**< Enable Interrupt when VREG under-voltage: below 1V */
      unsigned int VREGOVR    :  1;  /**< Enable Interrupt when VREG over-voltage: over- 1.32V */
      unsigned int reserved3  :  5;
      unsigned int RANGEBAT   :  2;  /**< Battery Monitor Range */
      unsigned int IENBAT     :  1;  /**< Interrupt enable for VBAT range */
      unsigned int reserved11 : 21;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_IEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_IEN_t__ */

/*@}*/

/** @defgroup PSM_STAT Power supply monitor status (PSM_STAT) Register
 *  Power supply monitor status (PSM_STAT) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_PMG_PSM_STAT_RORANGE1
 *! \brief  VBAT range1 (RORANGE1) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_PSM_STAT_BATSTAT1 = 0,  /**< VBAT NOT in the range specified */
  PMG_PSM_STAT_BATSTAT0 = 1   /**< VBAT in the range specified     */
} ADI_PMG_PSM_STAT_RORANGE1;


/* ==========================================================================
 *! \struct ADI_PMG_PSM_STAT_Struct
 *! \brief  Power supply monitor status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_PSM_STAT_t__
typedef struct _ADI_PMG_PSM_STAT_t {
  union {
    struct {
      unsigned int VBATUNDR   :  1;  /**< Status bit indicating an Alarm that battery is below 1.8V */
      unsigned int VREGUNDR   :  1;  /**< Status bit for Alarm indicating VREG is below 1V */
      unsigned int VREGOVR    :  1;  /**< Status bit for alarm indicating Overvoltage for VREG */
      unsigned int reserved3  :  4;
      unsigned int WICENACK   :  1;  /**< WIC Enable Acknowledge from Cortex */
      unsigned int RANGE1     :  1;  /**< VBAT range1 */
      unsigned int RANGE2     :  1;  /**< VBAT range2 */
      unsigned int RANGE3     :  1;  /**< VBAT range3 */
      unsigned int reserved11 :  2;
      unsigned int RORANGE1   :  1;  /**< VBAT range1 */
      unsigned int RORANGE2   :  1;  /**< VBAT range2 */
      unsigned int RORANGE3   :  1;  /**< VBAT range3 */
      unsigned int reserved16 : 16;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_PSM_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_PSM_STAT_t__ */

/*@}*/

/** @defgroup PWRMOD Power Mode Register (PWRMOD) Register
 *  Power Mode Register (PWRMOD) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_PMG_PWRMOD_MODE
 *! \brief  Power Mode Bits (MODE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_PWRMOD_FLEXI     = 0,  /**< FLEXI Mode     */
  PMG_PWRMOD_HIBERNATE = 2,  /**< HIBERNATE Mode */
  PMG_PWRMOD_SHUTDOWN  = 3   /**< SHUTDOWN Mode  */
} ADI_PMG_PWRMOD_MODE;


/* ==========================================================================
 *! \struct ADI_PMG_PWRMOD_Struct
 *! \brief  Power Mode Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_PWRMOD_t__
typedef struct _ADI_PMG_PWRMOD_t {
  union {
    struct {
      unsigned int MODE       :  2;  /**< Power Mode Bits */
      unsigned int reserved2  :  1;
      unsigned int MONVBATN   :  1;  /**< Monitor VBAT during HIBERNATE Mode. Monitors VBAT by default */
      unsigned int reserved4  : 28;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_PWRMOD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_PWRMOD_t__ */

/*@}*/

/** @defgroup PWRKEY Key protection for PWRMOD and  SRAMRET (PWRKEY) Register
 *  Key protection for PWRMOD and  SRAMRET (PWRKEY) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_PWRKEY_Struct
 *! \brief  Key protection for PWRMOD and  SRAMRET Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_PWRKEY_t__
typedef struct _ADI_PMG_PWRKEY_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Power control key register */
      unsigned int reserved16 : 16;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_PWRKEY_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_PWRKEY_t__ */

/*@}*/

/** @defgroup SHDN_STAT SHUTDOWN Status Register (SHDN_STAT) Register
 *  SHUTDOWN Status Register (SHDN_STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_SHDN_STAT_Struct
 *! \brief  SHUTDOWN Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_SHDN_STAT_t__
typedef struct _ADI_PMG_SHDN_STAT_t {
  union {
    struct {
      unsigned int EXTINT0    :  1;  /**< Interrupt from External Interrupt 0 */
      unsigned int EXTINT1    :  1;  /**< Interrupt from External Interrupt 1 */
      unsigned int EXTINT2    :  1;  /**< Interrupt from External Interrupt 2 */
      unsigned int RTC        :  1;  /**< Interrupt from RTC */
      unsigned int reserved4  : 28;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_SHDN_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_SHDN_STAT_t__ */

/*@}*/

/** @defgroup SRAMRET Control for Retention SRAM during HIBERNATE Mode (SRAMRET) Register
 *  Control for Retention SRAM during HIBERNATE Mode (SRAMRET) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_SRAMRET_Struct
 *! \brief  Control for Retention SRAM during HIBERNATE Mode Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_SRAMRET_t__
typedef struct _ADI_PMG_SRAMRET_t {
  union {
    struct {
      unsigned int RET1       :  1;  /**< Enable retention bank 1 (12kB) */
      unsigned int RET2       :  1;  /**< Enable retention bank 3 and bank 4 (32kB) */
      unsigned int reserved2  :  6;
      unsigned int RET3       :  1;  /**< Enable retention bank5 (32kB) */
      unsigned int RET4       :  1;  /**< Enable retention bank6 and bank7 (32kB) */
      unsigned int reserved10               : 13;
      unsigned int HIBERNATE_SRAM_LOAD_MODE :  1;  /**< Hibernate mode SRAM load mode control */
      unsigned int reserved24               :  8;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_SRAMRET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_SRAMRET_t__ */

/*@}*/

/** @defgroup RST_STAT Reset status (RST_STAT) Register
 *  Reset status (RST_STAT) Register.
 *
 *  This register is recommended to be read at the beginning of the user-code to determine the cause of the reset.
 *  Default values of this register is 'unspecified' because the cause of reset can be any source.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_PMG_RST_STAT_PORSRC
 *! \brief  Power on reset Source (pmg_rst_src) (PORSRC) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_RST_STAT_FAILSAFE_HV = 0,  /**< POR triggered because VBAT drops below Fail Safe */
  PMG_RST_STAT_RST_VBAT    = 1,  /**< POR trigger because VBAT supply (VBAT<1.7v)      */
  PMG_RST_STAT_RST_VREG    = 2,  /**< POR triggered because VDD supply (VDD < 1.08v)   */
  PMG_RST_STAT_FAILSAFE_LV = 3   /**< POR triggered because VREG drops below Fail Safe */
} ADI_PMG_RST_STAT_PORSRC;


/* ==========================================================================
 *! \struct ADI_PMG_RST_STAT_Struct
 *! \brief  Reset status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_RST_STAT_t__
typedef struct _ADI_PMG_RST_STAT_t {
  union {
    struct {
      unsigned int POR        :  1;  /**< Power-on reset */
      unsigned int EXTRST     :  1;  /**< External reset */
      unsigned int WDRST      :  1;  /**< Watchdog timeout */
      unsigned int SWRST      :  1;  /**< Software reset */
      unsigned int PORSRC     :  2;  /**< Power on reset Source (pmg_rst_src) */
      unsigned int reserved6  : 26;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_RST_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_RST_STAT_t__ */

/*@}*/

/** @defgroup CTL1 HPBUCK control (CTL1) Register
 *  HPBUCK control (CTL1) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_PMG_CTL1_HPBUCK_LD_MODE
 *! \brief  HP Buck load mode (HPBUCK_LD_MODE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_CTL1_HPBUCKLOWLOAD  = 0,  /**< HPBUCK Low load mode is enabled  */
  PMG_CTL1_HPBUCKHIGHLOAD = 1   /**< HPBUCK High load mode is enabled */
} ADI_PMG_CTL1_HPBUCK_LD_MODE;


/*  =========================================================================
 *! \enum   ADI_PMG_CTL1_HPBUCK_LOWPWR_MODE
 *! \brief  HP Buck low power mode (HPBUCK_LOWPWR_MODE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_CTL1_LOWPWRDISABLE = 0,  /**< HPBUCK Low power mode is disabled, When LOW */
  PMG_CTL1_LOWPWRENABLE  = 1   /**< HPBUCK Low power mode is enabled, When HIGH */
} ADI_PMG_CTL1_HPBUCK_LOWPWR_MODE;


/* ==========================================================================
 *! \struct ADI_PMG_CTL1_Struct
 *! \brief  HPBUCK control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_CTL1_t__
typedef struct _ADI_PMG_CTL1_t {
  union {
    struct {
      unsigned int HPBUCKEN   :  1;  /**< Enable HP Buck */
      unsigned int HPBUCK_LD_MODE :  1;  /**< HP Buck load mode */
      unsigned int HPBUCK_LOWPWR_MODE :  1;  /**< HP Buck low power mode */
      unsigned int reserved3          : 29;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_CTL1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_CTL1_t__ */

/*@}*/

/** @defgroup CFG0 External Interrupt configuration (CFG0) Register
 *  External Interrupt configuration (CFG0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_XINT_CFG0_Struct
 *! \brief  External Interrupt configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_XINT_CFG0_t__
typedef struct _ADI_XINT_CFG0_t {
  union {
    struct {
      unsigned int IRQ0MDE    :  3;  /**< External Interrupt 0 Mode registers */
      unsigned int IRQ0EN     :  1;  /**< External Interrupt 0 Enable bit */
      unsigned int IRQ1MDE    :  3;  /**< External Interrupt 1 Mode registers */
      unsigned int IRQ1EN     :  1;  /**< External Interrupt 1 Enable bit */
      unsigned int IRQ2MDE    :  3;  /**< External Interrupt 2 Mode registers */
      unsigned int IRQ2EN     :  1;  /**< External Interrupt 2 Enable bit */
      unsigned int IRQ3MDE    :  3;  /**< External Interrupt 3 Mode registers */
      unsigned int IRQ3EN     :  1;  /**< External Interrupt 3 enable bit */
      unsigned int reserved16 :  4;
      unsigned int UART_RX_EN :  1;  /**< External Interrupt using SIP_UPDATE enable bit */
      unsigned int UART_RX_MDE :  3;  /**< External Interrupt using UART_RX wakeup Mode registers */
      unsigned int reserved24  :  8;
    };
    uint32_t VALUE32;
  };
} ADI_XINT_CFG0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_XINT_CFG0_t__ */

/*@}*/

/** @defgroup EXT_STAT External Wakeup Interrupt Status register (EXT_STAT) Register
 *  External Wakeup Interrupt Status register (EXT_STAT) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_XINT_EXT_STAT_Struct
 *! \brief  External Wakeup Interrupt Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_XINT_EXT_STAT_t__
typedef struct _ADI_XINT_EXT_STAT_t {
  union {
    struct {
      unsigned int STAT_EXTINT0 :  1;  /**< Interrupt status bit for External Interrupt 0 */
      unsigned int STAT_EXTINT1 :  1;  /**< Interrupt status bit for External Interrupt 1 */
      unsigned int STAT_EXTINT2 :  1;  /**< Interrupt status bit for External Interrupt 2 */
      unsigned int STAT_EXTINT3 :  1;  /**< Interrupt status bit for External Interrupt 3 */
      unsigned int reserved4        :  1;
      unsigned int STAT_UART_RXWKUP :  1;  /**< Interrupt status bit for UART RX WAKEUP interrupt */
      unsigned int reserved6        : 26;
    };
    uint32_t VALUE32;
  };
} ADI_XINT_EXT_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_XINT_EXT_STAT_t__ */

/*@}*/

/** @defgroup CLR External Interrupt clear (CLR) Register
 *  External Interrupt clear (CLR) Register.
 *
 *  This register has W1C bits that are used to clear the corresponding EXT_STAT bits
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_XINT_CLR_Struct
 *! \brief  External Interrupt clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_XINT_CLR_t__
typedef struct _ADI_XINT_CLR_t {
  union {
    struct {
      unsigned int IRQ0       :  1;  /**< External interrupt 0 */
      unsigned int IRQ1       :  1;  /**< External interrupt 1 */
      unsigned int IRQ2       :  1;  /**< External interrupt 2 */
      unsigned int IRQ3       :  1;  /**< External interrupt 3 */
      unsigned int reserved4   :  1;
      unsigned int UART_RX_CLR :  1;  /**< External interrupt Clear for UART_RX WAKEUP interrupt */
      unsigned int reserved6   : 26;
    };
    uint32_t VALUE32;
  };
} ADI_XINT_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_XINT_CLR_t__ */

/*@}*/

/** @defgroup NMICLR Non-maskable interrupt clear (NMICLR) Register
 *  Non-maskable interrupt clear (NMICLR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_XINT_NMICLR_Struct
 *! \brief  Non-maskable interrupt clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_XINT_NMICLR_t__
typedef struct _ADI_XINT_NMICLR_t {
  union {
    struct {
      unsigned int CLR        :  1;  /**< NMI clear */
      unsigned int reserved1  : 31;
    };
    uint32_t VALUE32;
  };
} ADI_XINT_NMICLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_XINT_NMICLR_t__ */

/*@}*/

/** @defgroup KEY Key Protection for OSCCTRL (KEY) Register
 *  Key Protection for OSCCTRL (KEY) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_OSC_KEY_Struct
 *! \brief  Key Protection for OSCCTRL Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_OSC_KEY_t__
typedef struct _ADI_CLKG_OSC_KEY_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Oscillator key */
      unsigned int reserved16 : 16;
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_OSC_KEY_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_OSC_KEY_t__ */

/*@}*/

/** @defgroup CTL Oscillator Control (CTL) Register
 *  Oscillator Control (CTL) Register.
 *
 *  The OSCCTRL register is key-protected. To unlock this protection 0xCB14 should be written to OSCKEY before writing to OSCCTRL  A write to any other register on the APB bus before writing to OSCCTRL will return the protection to the lock state.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_OSC_CTL_Struct
 *! \brief  Oscillator Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_OSC_CTL_t__
typedef struct _ADI_CLKG_OSC_CTL_t {
  union {
    struct {
      unsigned int LFCLK_MUX  :  1;  /**< 32 KHz clock select mux */
      unsigned int HFOSC_EN   :  1;  /**< High frequency internal oscillator enable */
      unsigned int LFX_EN     :  1;  /**< Low frequency crystal oscillator enable */
      unsigned int HFX_EN     :  1;  /**< High frequency crystal oscillator enable */
      unsigned int LFX_BYP    :  1;  /**< Low frequency crystal oscillator Bypass */
      unsigned int LFX_MON_EN :  1;  /**< LFXTAL clock monitor and Clock FAIL interrupt enable */
      unsigned int reserved6  :  2;
      unsigned int LFOSC_OK   :  1;  /**< Status of LFOSC oscillator */
      unsigned int HFOSC_OK   :  1;  /**< Status of HFOSC oscillator */
      unsigned int LFX_OK     :  1;  /**< Status of LFXTAL oscillator */
      unsigned int HFX_OK     :  1;  /**< Status of HFXTAL oscillator */
      unsigned int LFX_AUTSW_EN :  1;  /**< Enables automatic Switching of the LF Mux to LF OSC on LF XTAL Failure */
      unsigned int LFX_AUTSW_STA :  1;  /**< Status of automatic switching of the LF Mux to LF OSC upon detection of LF XTAL failure */
      unsigned int LFX_ROBUST_EN :  1;  /**< LFXTAL Mode select */
      unsigned int LFX_ROBUST_LD :  2;  /**< LFXTAL Robust Mode Load select */
      unsigned int reserved17    :  3;
      unsigned int ROOT_MON_EN   :  1;  /**< ROOT clock monitor and Clock FAIL interrupt enable */
      unsigned int ROOT_AUTSW_EN :  1;  /**< Enables automatic Switching of the Root clock  to HFOSC on Root clock Failure */
      unsigned int ROOT_AUTSW_STA :  1;  /**< Status of automatic switching of the Root clock to HF OSC upon detection of Root clock failure */
      unsigned int reserved23     :  7;
      unsigned int ROOT_FAIL_STA  :  1;  /**< Root clock (crystal clock) Not Stable */
      unsigned int LFX_FAIL_STA   :  1;  /**< LF XTAL (crystal clock) Not Stable */
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_OSC_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_OSC_CTL_t__ */

/*@}*/

/** @defgroup SRAM_CTL Control for SRAM parity and instruction SRAM (SRAM_CTL) Register
 *  Control for SRAM parity and instruction SRAM (SRAM_CTL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_SRAM_CTL_Struct
 *! \brief  Control for SRAM parity and instruction SRAM Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_SRAM_CTL_t__
typedef struct _ADI_PMG_TST_SRAM_CTL_t {
  union {
    struct {
      unsigned int reserved0  :  1;
      unsigned int BNK1EN     :  1;  /**< Enable initialization */
      unsigned int BNK2EN     :  1;  /**< Enable initialization */
      unsigned int reserved3  :  4;
      unsigned int BNK7EN     :  1;  /**< Enable initialization */
      unsigned int reserved8  :  5;
      unsigned int STARTINIT  :  1;  /**< Write one to trigger initialization. Self-cleared */
      unsigned int AUTOINIT   :  1;  /**< Automatic initialization on wake up from Hibernate mode */
      unsigned int ABTINIT    :  1;  /**< Abort current initialization. Self-cleared */
      unsigned int PENBNK0    :  1;  /**< Enable parity check */
      unsigned int PENBNK1    :  1;  /**< Enable parity check */
      unsigned int PENBNK2    :  1;  /**< Enable parity check */
      unsigned int PENBNK3    :  1;  /**< Enable parity check */
      unsigned int PENBNK4    :  1;  /**< Enable parity check */
      unsigned int PENBNK5    :  1;  /**< Enable parity check */
      unsigned int PENBNK6    :  1;  /**< Enable parity check */
      unsigned int PENBNK7    :  1;  /**< Enable parity check */
      unsigned int reserved24 :  7;
      unsigned int INSTREN    :  1;  /**< Enables 32kB instruction SRAM */
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TST_SRAM_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_SRAM_CTL_t__ */

/*@}*/

/** @defgroup SRAM_INITSTAT Initialization Status Register (SRAM_INITSTAT) Register
 *  Initialization Status Register (SRAM_INITSTAT) Register.
 *
 *  
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_PMG_TST_SRAM_INITSTAT_BNK0DONE
 *! \brief  Bank 0 initialization status (BNK0DONE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_TST_SRAM_INITSTAT_NO_BANK0_INIT = 0,  /**< Bank 0 not initialized */
  PMG_TST_SRAM_INITSTAT_BANK0_INIT    = 1   /**< Bank 0 initialized     */
} ADI_PMG_TST_SRAM_INITSTAT_BNK0DONE;


/*  =========================================================================
 *! \enum   ADI_PMG_TST_SRAM_INITSTAT_BNK1DONE
 *! \brief  Bank 1 initialization status (BNK1DONE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_TST_SRAM_INITSTAT_NO_BANK1_INIT = 0,  /**< Bank 1 not initialized */
  PMG_TST_SRAM_INITSTAT_BANK1_INIT    = 1   /**< Bank 1 initialized     */
} ADI_PMG_TST_SRAM_INITSTAT_BNK1DONE;


/*  =========================================================================
 *! \enum   ADI_PMG_TST_SRAM_INITSTAT_BNK2DONE
 *! \brief  Bank 2 initialization status (BNK2DONE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_TST_SRAM_INITSTAT_NO_BANK2_INIT = 0,  /**< Bank 2 not initialized */
  PMG_TST_SRAM_INITSTAT_BANK2_INIT    = 1   /**< Bank 2 initialized     */
} ADI_PMG_TST_SRAM_INITSTAT_BNK2DONE;


/*  =========================================================================
 *! \enum   ADI_PMG_TST_SRAM_INITSTAT_BNK3DONE
 *! \brief  Bank 3 initialization status (BNK3DONE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_TST_SRAM_INITSTAT_NO_BANK3_INIT = 0,  /**< Bank 3 not initialized */
  PMG_TST_SRAM_INITSTAT_BANK3_INIT    = 1   /**< Bank 3 initialized     */
} ADI_PMG_TST_SRAM_INITSTAT_BNK3DONE;


/*  =========================================================================
 *! \enum   ADI_PMG_TST_SRAM_INITSTAT_BNK4DONE
 *! \brief  Bank 4 initialization status (BNK4DONE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_TST_SRAM_INITSTAT_NO_BANK4_INIT = 0,  /**< Bank 4 not initialized */
  PMG_TST_SRAM_INITSTAT_BANK4_INIT    = 1   /**< Bank 4 initialized     */
} ADI_PMG_TST_SRAM_INITSTAT_BNK4DONE;


/*  =========================================================================
 *! \enum   ADI_PMG_TST_SRAM_INITSTAT_BNK5DONE
 *! \brief  Bank 5 initialization status (BNK5DONE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_TST_SRAM_INITSTAT_NO_BANK5_INIT = 0,  /**< Bank 5 not initialized */
  PMG_TST_SRAM_INITSTAT_BANK5_INIT    = 1   /**< Bank 5 initialized     */
} ADI_PMG_TST_SRAM_INITSTAT_BNK5DONE;


/*  =========================================================================
 *! \enum   ADI_PMG_TST_SRAM_INITSTAT_BNK6DONE
 *! \brief  Bank 6 initialization status (BNK6DONE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_TST_SRAM_INITSTAT_NO_BANK6_INIT = 0,  /**< Bank 6 not initialized */
  PMG_TST_SRAM_INITSTAT_BANK6_INIT    = 1   /**< Bank 6 initialized     */
} ADI_PMG_TST_SRAM_INITSTAT_BNK6DONE;


/*  =========================================================================
 *! \enum   ADI_PMG_TST_SRAM_INITSTAT_BNK7DONE
 *! \brief  Bank 7 initialization status (BNK7DONE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_TST_SRAM_INITSTAT_NO_BANK7_INIT = 0,  /**< Bank 7 not initialized */
  PMG_TST_SRAM_INITSTAT_BANK7_INIT    = 1   /**< Bank 7 initialized     */
} ADI_PMG_TST_SRAM_INITSTAT_BNK7DONE;


/* ==========================================================================
 *! \struct ADI_PMG_TST_SRAM_INITSTAT_Struct
 *! \brief  Initialization Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_SRAM_INITSTAT_t__
typedef struct _ADI_PMG_TST_SRAM_INITSTAT_t {
  union {
    struct {
      unsigned int BNK0DONE   :  1;  /**< Bank 0 initialization status */
      unsigned int BNK1DONE   :  1;  /**< Bank 1 initialization status */
      unsigned int BNK2DONE   :  1;  /**< Bank 2 initialization status */
      unsigned int BNK3DONE   :  1;  /**< Bank 3 initialization status */
      unsigned int BNK4DONE   :  1;  /**< Bank 4 initialization status */
      unsigned int BNK5DONE   :  1;  /**< Bank 5 initialization status */
      unsigned int BNK6DONE   :  1;  /**< Bank 6 initialization status */
      unsigned int BNK7DONE   :  1;  /**< Bank 7 initialization status */
      unsigned int reserved8  : 24;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TST_SRAM_INITSTAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_SRAM_INITSTAT_t__ */

/*@}*/

/** @defgroup CLR_LATCH_GPIOS CLEAR GPIO AFTER SHUTDOWN MODE (CLR_LATCH_GPIOS) Register
 *  CLEAR GPIO AFTER SHUTDOWN MODE (CLR_LATCH_GPIOS) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_CLR_LATCH_GPIOS_Struct
 *! \brief  CLEAR GPIO AFTER SHUTDOWN MODE Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_CLR_LATCH_GPIOS_t__
typedef struct _ADI_PMG_TST_CLR_LATCH_GPIOS_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Writing 0x58FA creates a pulse to clear the latches for the GPIOs */
    };
    uint16_t VALUE16;
  };
} ADI_PMG_TST_CLR_LATCH_GPIOS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_CLR_LATCH_GPIOS_t__ */

/*@}*/

/** @defgroup SCRPAD_IMG SCRATCH PAD IMAGE (SCRPAD_IMG) Register
 *  SCRATCH PAD IMAGE (SCRPAD_IMG) Register.
 *
 *  This register is useful in Shutdown mode.
 *  The GPIO configuration, OUT registers lose its contents when in shutdown mode.
 *  32-bit Scratch register can be used to store the GPIO configuration. 
 *  Note that the bits are not wide enough to store all GPIO related configuration and output values. It is envisaged that the user might have only few combination of shutdown port configurations. Hence these combination can be remembered by storing an equivalent encoded data in the scratch 32-bit registers.
 *  The USER after reading back the SCRATCHPAD_3V_READ register (wakeup from shutdown) can determine the GPIO configuration.
 *  Note that the scratch register content will be lost in shutdown mode if VBAT < 1.54v
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_SCRPAD_IMG_Struct
 *! \brief  SCRATCH PAD IMAGE Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_SCRPAD_IMG_t__
typedef struct _ADI_PMG_TST_SCRPAD_IMG_t {
  union {
    struct {
      unsigned int DATA       : 32;  /**< Anything written to this register will be saved in 3V when going to shutdown mode */
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TST_SCRPAD_IMG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_SCRPAD_IMG_t__ */

/*@}*/

/** @defgroup SCRPAD_3V_RD SCRATCH PAD SAVED IN BATTERY DOMAIN (SCRPAD_3V_RD) Register
 *  SCRATCH PAD SAVED IN BATTERY DOMAIN (SCRPAD_3V_RD) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_SCRPAD_3V_RD_Struct
 *! \brief  SCRATCH PAD SAVED IN BATTERY DOMAIN Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_SCRPAD_3V_RD_t__
typedef struct _ADI_PMG_TST_SCRPAD_3V_RD_t {
  union {
    struct {
      unsigned int DATA       : 32;  /**< Read Only register. Reading the scratch pad stored in shutdown mode */
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TST_SCRPAD_3V_RD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_SCRPAD_3V_RD_t__ */

/*@}*/

/** @defgroup FAST_SHT_WAKEUP Fast shutdown wakeup enable (FAST_SHT_WAKEUP) Register
 *  Fast shutdown wakeup enable (FAST_SHT_WAKEUP) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_FAST_SHT_WAKEUP_Struct
 *! \brief  Fast shutdown wakeup enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_FAST_SHT_WAKEUP_t__
typedef struct _ADI_PMG_TST_FAST_SHT_WAKEUP_t {
  union {
    struct {
      unsigned int FAST_SHT_WAKEUP :  1;  /**< Enables fast shutdown wakeup */
      unsigned int reserved1       : 31;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TST_FAST_SHT_WAKEUP_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_FAST_SHT_WAKEUP_t__ */

/*@}*/

/** @defgroup CTL0 Misc clock settings (CTL0) Register
 *  Misc clock settings (CTL0) Register.
 *
 *  Clock Control 0 is used to configure clock sources used by various systems such as the core and memories and peripherals. All unused bits are read only returning a value of 0. Writing unused bits has no effect.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_CTL0_Struct
 *! \brief  Misc clock settings Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL0_t__
typedef struct _ADI_CLKG_CLK_CTL0_t {
  union {
    struct {
      unsigned int CLKMUX     :  2;  /**< Clock mux select */
      unsigned int reserved2  :  1;
      unsigned int CLKOUT     :  4;  /**< GPIO clock out select */
      unsigned int reserved7  :  1;
      unsigned int RCLKMUX    :  2;  /**< Flash reference clock and HPBUCK clock source mux */
      unsigned int reserved10 :  1;
      unsigned int PLL_IPSEL  :  2;  /**< SPLL source select mux */
      unsigned int reserved13 :  1;
      unsigned int LFXTALIE   :  1;  /**< Low frequency crystal interrupt enable */
      unsigned int HFXTALIE   :  1;  /**< High frequency crystal interrupt enable */
      unsigned int reserved16 : 16;
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_CLK_CTL0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL0_t__ */

/*@}*/

/** @defgroup CTL1 Clock dividers (CTL1) Register
 *  Clock dividers (CTL1) Register.
 *
 *  Clock Control 1 is used to set the divide rates for the  HCLK, and PCLK and ACLK dividers. This register can be written to at any time. All unused bits are read only, returning a value of 0. Writing to unused bits has no effect.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_CTL1_Struct
 *! \brief  Clock dividers Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL1_t__
typedef struct _ADI_CLKG_CLK_CTL1_t {
  union {
    struct {
      unsigned int HCLKDIVCNT :  6;  /**< HCLK divide count */
      unsigned int reserved6  :  2;
      unsigned int PCLKDIVCNT :  6;  /**< PCLK divide count */
      unsigned int reserved14 :  2;
      unsigned int ACLKDIVCNT :  9;  /**< ACLK Divide Count. */
      unsigned int reserved25 :  7;
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_CLK_CTL1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL1_t__ */

/*@}*/

/** @defgroup CTL2 HF Oscillator divided clock select (CTL2) Register
 *  HF Oscillator divided clock select (CTL2) Register.
 *
 *  Clock Control 2  is used to select the clock from the frequency divided clocks derived from the hf oscillator clock. All unused bits are read only, returning a value of 0. Writing to unused bits has no effect.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_CTL2_Struct
 *! \brief  HF Oscillator divided clock select Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL2_t__
typedef struct _ADI_CLKG_CLK_CTL2_t {
  union {
    struct {
      unsigned int HFOSCAUTODIV_EN :  1;  /**< HF Oscillator auto divide by one clock selection during wakeup from Flexi power mode */
      unsigned int HFOSCDIVCLKSEL  :  3;  /**< HF Oscillator divided clock select */
      unsigned int reserved4       : 28;
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_CLK_CTL2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL2_t__ */

/*@}*/

/** @defgroup CTL3 System PLL (CTL3) Register
 *  System PLL (CTL3) Register.
 *
 *  Clock Control 3 is used to control the system PLL. This register should be written to only when the PLL is not selected as the clock source (ROOT_CLK). All unused bits are read only, returning a value of 0. Writing to unused bits has no effect.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_CTL3_Struct
 *! \brief  System PLL Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL3_t__
typedef struct _ADI_CLKG_CLK_CTL3_t {
  union {
    struct {
      unsigned int SPLLNSEL   :  5;  /**< System PLL N multiplier */
      unsigned int reserved5  :  3;
      unsigned int SPLLDIV2   :  1;  /**< System PLL division by 2 */
      unsigned int SPLLEN     :  1;  /**< System PLL enable */
      unsigned int SPLLIE     :  1;  /**< System PLL interrupt enable */
      unsigned int SPLLMSEL   :  4;  /**< System PLL M Divider */
      unsigned int reserved15 :  1;
      unsigned int SPLLMUL2   :  1;  /**< system PLL multiply by 2 */
      unsigned int reserved17 : 15;
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_CLK_CTL3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL3_t__ */

/*@}*/

/** @defgroup CTL5 User clock gating control (CTL5) Register
 *  User clock gating control (CTL5) Register.
 *
 *  Clock Control 5 is used to control the gates of the peripheral UCLKs.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_CTL5_Struct
 *! \brief  User clock gating control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL5_t__
typedef struct _ADI_CLKG_CLK_CTL5_t {
  union {
    struct {
      unsigned int GPTCLK0OFF :  1;  /**< GP timer 0 user control */
      unsigned int GPTCLK1OFF :  1;  /**< GP timer 1 user control */
      unsigned int GPTCLK2OFF :  1;  /**< GP timer 2 user control */
      unsigned int UCLKI2COFF :  1;  /**< I2C clock user control */
      unsigned int GPIOCLKOFF :  1;  /**< GPIO clock control */
      unsigned int PERCLKOFF  :  1;  /**< This bit is used to disable all clocks connected to all peripherals */
      unsigned int TMRRGBCLKOFF :  1;  /**< Timer RGB user control */
      unsigned int reserved7    : 25;
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_CLK_CTL5_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL5_t__ */

/*@}*/

/** @defgroup STAT0 Clocking status (STAT0) Register
 *  Clocking status (STAT0) Register.
 *
 *  Clock Status is used to monitor PLL and Oscillator status. With interrupts enabled the user is free to continue to run initialization code or idle the core while clock components stabilize.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_STAT0_Struct
 *! \brief  Clocking status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_STAT0_t__
typedef struct _ADI_CLKG_CLK_STAT0_t {
  union {
    struct {
      unsigned int SPLL       :  1;  /**< System PLL status */
      unsigned int SPLLLK     :  1;  /**< System PLL lock */
      unsigned int SPLLUNLK   :  1;  /**< System PLL unlock */
      unsigned int reserved3  :  5;
      unsigned int LFXTAL     :  1;  /**< LF crystal status */
      unsigned int LFXTALOK   :  1;  /**< LF crystal stable */
      unsigned int LFXTALNOK  :  1;  /**< LF crystal not stable */
      unsigned int reserved11 :  1;
      unsigned int HFXTAL     :  1;  /**< HF crystal status */
      unsigned int HFXTALOK   :  1;  /**< HF crystal stable */
      unsigned int HFXTALNOK  :  1;  /**< HF crystal not stable */
      unsigned int reserved15 : 17;
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_CLK_STAT0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_STAT0_t__ */

/*@}*/

/** @defgroup ARBIT0 Arbitration Priority Configuration for FLASH and SRAM0 (ARBIT0) Register
 *  Arbitration Priority Configuration for FLASH and SRAM0 (ARBIT0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BUSM_ARBIT0_Struct
 *! \brief  Arbitration Priority Configuration for FLASH and SRAM0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT0_t__
typedef struct _ADI_BUSM_ARBIT0_t {
  union {
    struct {
      unsigned int FLSH_DCODE :  2;  /**< Flash priority for DCODE */
      unsigned int FLSH_SBUS  :  2;  /**< Flash priority for SBUS */
      unsigned int FLSH_DMA0  :  2;  /**< Flash priority for DMA0 */
      unsigned int reserved6   : 10;
      unsigned int SRAM0_DCODE :  2;  /**< SRAM0 priority for Dcode */
      unsigned int SRAM0_SBUS  :  2;  /**< SRAM0 priority for SBUS */
      unsigned int SRAM0_DMA0  :  2;  /**< SRAM0 priority for DMA0 */
      unsigned int reserved22  : 10;
    };
    uint32_t VALUE32;
  };
} ADI_BUSM_ARBIT0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT0_t__ */

/*@}*/

/** @defgroup ARBIT1 Arbitration Priority Configuration for SRAM1 and SIP (ARBIT1) Register
 *  Arbitration Priority Configuration for SRAM1 and SIP (ARBIT1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BUSM_ARBIT1_Struct
 *! \brief  Arbitration Priority Configuration for SRAM1 and SIP Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT1_t__
typedef struct _ADI_BUSM_ARBIT1_t {
  union {
    struct {
      unsigned int SRAM1_DCODE :  2;  /**< SRAM1 priority for Dcode */
      unsigned int SRAM1_SBUS  :  2;  /**< SRAM1 priority for SBUS */
      unsigned int SRAM1_DMA0  :  2;  /**< SRAM1 priority for DMA0 */
      unsigned int reserved6   : 10;
      unsigned int SIP_DCODE   :  2;  /**< SIP priority for DCODE */
      unsigned int SIP_SBUS    :  2;  /**< SIP priority for SBUS */
      unsigned int SIP_DMA0    :  2;  /**< SIP priority for DMA0 */
      unsigned int reserved22  : 10;
    };
    uint32_t VALUE32;
  };
} ADI_BUSM_ARBIT1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT1_t__ */

/*@}*/

/** @defgroup ARBIT2 Arbitration Priority Configuration for APB32 and APB16 (ARBIT2) Register
 *  Arbitration Priority Configuration for APB32 and APB16 (ARBIT2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BUSM_ARBIT2_Struct
 *! \brief  Arbitration Priority Configuration for APB32 and APB16 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT2_t__
typedef struct _ADI_BUSM_ARBIT2_t {
  union {
    struct {
      unsigned int APB32_DCODE :  2;  /**< APB32 priority for DCODE */
      unsigned int APB32_SBUS  :  2;  /**< APB32 priority for SBUS */
      unsigned int APB32_DMA0  :  2;  /**< APB32 priority for DMA0 */
      unsigned int reserved6   : 10;
      unsigned int APB16_DCODE :  2;  /**< APB16 priority for DCODE */
      unsigned int APB16_SBUS  :  2;  /**< APB16 priority for SBUS */
      unsigned int APB16_DMA0  :  2;  /**< APB16 priority for DMA0 */
      unsigned int reserved22  : 10;
    };
    uint32_t VALUE32;
  };
} ADI_BUSM_ARBIT2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT2_t__ */

/*@}*/

/** @defgroup ARBIT3 Arbitration Priority Configuration for APB16 priority for core and for DMA1 (ARBIT3) Register
 *  Arbitration Priority Configuration for APB16 priority for core and for DMA1 (ARBIT3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BUSM_ARBIT3_Struct
 *! \brief  Arbitration Priority Configuration for APB16 priority for core and for DMA1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT3_t__
typedef struct _ADI_BUSM_ARBIT3_t {
  union {
    struct {
      unsigned int APB16_CORE :  1;  /**< APB16 priority for CORE */
      unsigned int APB16_DMA1 :  1;  /**< APB16 priority for DMA1 */
      unsigned int reserved2       : 14;
      unsigned int APB16_4DMA_CORE :  1;  /**< APB16 for dma priority for CORE */
      unsigned int APB16_4DMA_DMA1 :  1;  /**< APB16 for dma priority for DMA1 */
      unsigned int reserved18      : 14;
    };
    uint32_t VALUE32;
  };
} ADI_BUSM_ARBIT3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT3_t__ */

/*@}*/

/** @defgroup ARBIT4 Arbitration Priority Configuration for SRAM1 and SIP (ARBIT4) Register
 *  Arbitration Priority Configuration for SRAM1 and SIP (ARBIT4) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BUSM_ARBIT4_Struct
 *! \brief  Arbitration Priority Configuration for SRAM1 and SIP Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT4_t__
typedef struct _ADI_BUSM_ARBIT4_t {
  union {
    struct {
      unsigned int SRAM2_DCODE :  2;  /**< SRAM2 priority for Dcode */
      unsigned int SRAM2_SBUS  :  2;  /**< SRAM2 priority for SBUS */
      unsigned int SRAM2_DMA0  :  2;  /**< SRAM2 priority for DMA0 */
      unsigned int reserved6   : 26;
    };
    uint32_t VALUE32;
  };
} ADI_BUSM_ARBIT4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BUSM_ARBIT4_t__ */

/*@}*/

/** @defgroup RST_ISR_STARTADDR Reset ISR Start Address (RST_ISR_STARTADDR) Register
 *  Reset ISR Start Address (RST_ISR_STARTADDR) Register.
 *
 *  This register is used only when Parallel Test Interface is enabled. 
 *  Contains 32 bit Reset ISR start address.
 *  LSB should be 1.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PTI_RST_ISR_STARTADDR_Struct
 *! \brief  Reset ISR Start Address Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PTI_RST_ISR_STARTADDR_t__
typedef struct _ADI_PTI_RST_ISR_STARTADDR_t {
  union {
    struct {
      unsigned int VALUE      : 32;
    };
    uint32_t VALUE32;
  };
} ADI_PTI_RST_ISR_STARTADDR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PTI_RST_ISR_STARTADDR_t__ */

/*@}*/

/** @defgroup RST_STACK_PTR Reset Stack Pointer (RST_STACK_PTR) Register
 *  Reset Stack Pointer (RST_STACK_PTR) Register.
 *
 *  This register is used only when Parallel Test Interface is enabled. 
 *  Contains 32 bit initial stack pointer address.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PTI_RST_STACK_PTR_Struct
 *! \brief  Reset Stack Pointer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PTI_RST_STACK_PTR_t__
typedef struct _ADI_PTI_RST_STACK_PTR_t {
  union {
    struct {
      unsigned int VALUE      : 32;
    };
    uint32_t VALUE32;
  };
} ADI_PTI_RST_STACK_PTR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PTI_RST_STACK_PTR_t__ */

/*@}*/

/** @defgroup CTL Parallel Test Interface Control Register (CTL) Register
 *  Parallel Test Interface Control Register (CTL) Register.
 *
 *  This register is used enable Parallel Test Interface.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PTI_CTL_Struct
 *! \brief  Parallel Test Interface Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PTI_CTL_t__
typedef struct _ADI_PTI_CTL_t {
  union {
    struct {
      unsigned int EN         :  1;
      unsigned int reserved1  : 31;
    };
    uint32_t VALUE32;
  };
} ADI_PTI_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PTI_CTL_t__ */

/*@}*/

/** @defgroup INTNUM Interrupt Control Type (INTNUM) Register
 *  Interrupt Control Type (INTNUM) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTNUM_Struct
 *! \brief  Interrupt Control Type Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTNUM_t__
typedef struct _ADI_NVIC_INTNUM_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Interrupt Control Type */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTNUM_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTNUM_t__ */

/*@}*/

/** @defgroup STKSTA Systick Control and Status (STKSTA) Register
 *  Systick Control and Status (STKSTA) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_STKSTA_Struct
 *! \brief  Systick Control and Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_STKSTA_t__
typedef struct _ADI_NVIC_STKSTA_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Systick Control and Status */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_STKSTA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_STKSTA_t__ */

/*@}*/

/** @defgroup STKLD Systick Reload Value (STKLD) Register
 *  Systick Reload Value (STKLD) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_STKLD_Struct
 *! \brief  Systick Reload Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_STKLD_t__
typedef struct _ADI_NVIC_STKLD_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Systick Reload Value */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_STKLD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_STKLD_t__ */

/*@}*/

/** @defgroup STKVAL Systick Current Value (STKVAL) Register
 *  Systick Current Value (STKVAL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_STKVAL_Struct
 *! \brief  Systick Current Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_STKVAL_t__
typedef struct _ADI_NVIC_STKVAL_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Systick Current Value */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_STKVAL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_STKVAL_t__ */

/*@}*/

/** @defgroup STKCAL Systick Calibration Value (STKCAL) Register
 *  Systick Calibration Value (STKCAL) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_STKCAL_Struct
 *! \brief  Systick Calibration Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_STKCAL_t__
typedef struct _ADI_NVIC_STKCAL_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Systick Calibration Value */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_STKCAL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_STKCAL_t__ */

/*@}*/

/** @defgroup INTSETE0 IRQ0..31 Set_Enable (INTSETE0) Register
 *  IRQ0..31 Set_Enable (INTSETE0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTSETE0_Struct
 *! \brief  IRQ0..31 Set_Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTSETE0_t__
typedef struct _ADI_NVIC_INTSETE0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ0..31 Set_Enable */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTSETE0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTSETE0_t__ */

/*@}*/

/** @defgroup INTSETE1 IRQ32..63 Set_Enable (INTSETE1) Register
 *  IRQ32..63 Set_Enable (INTSETE1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTSETE1_Struct
 *! \brief  IRQ32..63 Set_Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTSETE1_t__
typedef struct _ADI_NVIC_INTSETE1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ32..63 Set_Enable */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTSETE1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTSETE1_t__ */

/*@}*/

/** @defgroup INTCLRE0 IRQ0..31 Clear_Enable (INTCLRE0) Register
 *  IRQ0..31 Clear_Enable (INTCLRE0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCLRE0_Struct
 *! \brief  IRQ0..31 Clear_Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCLRE0_t__
typedef struct _ADI_NVIC_INTCLRE0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ0..31 Clear_Enable */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCLRE0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCLRE0_t__ */

/*@}*/

/** @defgroup INTCLRE1 IRQ32..63 Clear_Enable (INTCLRE1) Register
 *  IRQ32..63 Clear_Enable (INTCLRE1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCLRE1_Struct
 *! \brief  IRQ32..63 Clear_Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCLRE1_t__
typedef struct _ADI_NVIC_INTCLRE1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ32..63 Clear_Enable */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCLRE1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCLRE1_t__ */

/*@}*/

/** @defgroup INTSETP0 IRQ0..31 Set_Pending (INTSETP0) Register
 *  IRQ0..31 Set_Pending (INTSETP0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTSETP0_Struct
 *! \brief  IRQ0..31 Set_Pending Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTSETP0_t__
typedef struct _ADI_NVIC_INTSETP0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ0..31 Set_Pending */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTSETP0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTSETP0_t__ */

/*@}*/

/** @defgroup INTSETP1 IRQ32..63 Set_Pending (INTSETP1) Register
 *  IRQ32..63 Set_Pending (INTSETP1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTSETP1_Struct
 *! \brief  IRQ32..63 Set_Pending Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTSETP1_t__
typedef struct _ADI_NVIC_INTSETP1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ32..63 Set_Pending */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTSETP1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTSETP1_t__ */

/*@}*/

/** @defgroup INTCLRP0 IRQ0..31 Clear_Pending (INTCLRP0) Register
 *  IRQ0..31 Clear_Pending (INTCLRP0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCLRP0_Struct
 *! \brief  IRQ0..31 Clear_Pending Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCLRP0_t__
typedef struct _ADI_NVIC_INTCLRP0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ0..31 Clear_Pending */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCLRP0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCLRP0_t__ */

/*@}*/

/** @defgroup INTCLRP1 IRQ32..63 Clear_Pending (INTCLRP1) Register
 *  IRQ32..63 Clear_Pending (INTCLRP1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCLRP1_Struct
 *! \brief  IRQ32..63 Clear_Pending Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCLRP1_t__
typedef struct _ADI_NVIC_INTCLRP1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ32..63 Clear_Pending */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCLRP1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCLRP1_t__ */

/*@}*/

/** @defgroup INTACT0 IRQ0..31 Active Bit (INTACT0) Register
 *  IRQ0..31 Active Bit (INTACT0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTACT0_Struct
 *! \brief  IRQ0..31 Active Bit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTACT0_t__
typedef struct _ADI_NVIC_INTACT0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ0..31 Active Bit */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTACT0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTACT0_t__ */

/*@}*/

/** @defgroup INTACT1 IRQ32..63 Active Bit (INTACT1) Register
 *  IRQ32..63 Active Bit (INTACT1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTACT1_Struct
 *! \brief  IRQ32..63 Active Bit Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTACT1_t__
typedef struct _ADI_NVIC_INTACT1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ32..63 Active Bit */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTACT1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTACT1_t__ */

/*@}*/

/** @defgroup INTPRI0 IRQ0..3 Priority (INTPRI0) Register
 *  IRQ0..3 Priority (INTPRI0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI0_Struct
 *! \brief  IRQ0..3 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI0_t__
typedef struct _ADI_NVIC_INTPRI0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ0..3 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI0_t__ */

/*@}*/

/** @defgroup INTPRI1 IRQ4..7 Priority (INTPRI1) Register
 *  IRQ4..7 Priority (INTPRI1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI1_Struct
 *! \brief  IRQ4..7 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI1_t__
typedef struct _ADI_NVIC_INTPRI1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ4..7 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI1_t__ */

/*@}*/

/** @defgroup INTPRI2 IRQ8..11 Priority (INTPRI2) Register
 *  IRQ8..11 Priority (INTPRI2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI2_Struct
 *! \brief  IRQ8..11 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI2_t__
typedef struct _ADI_NVIC_INTPRI2_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ8..11 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI2_t__ */

/*@}*/

/** @defgroup INTPRI3 IRQ12..15 Priority (INTPRI3) Register
 *  IRQ12..15 Priority (INTPRI3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI3_Struct
 *! \brief  IRQ12..15 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI3_t__
typedef struct _ADI_NVIC_INTPRI3_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ12..15 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI3_t__ */

/*@}*/

/** @defgroup INTPRI4 IRQ16..19 Priority (INTPRI4) Register
 *  IRQ16..19 Priority (INTPRI4) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI4_Struct
 *! \brief  IRQ16..19 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI4_t__
typedef struct _ADI_NVIC_INTPRI4_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ16..19 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI4_t__ */

/*@}*/

/** @defgroup INTPRI5 IRQ20..23 Priority (INTPRI5) Register
 *  IRQ20..23 Priority (INTPRI5) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI5_Struct
 *! \brief  IRQ20..23 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI5_t__
typedef struct _ADI_NVIC_INTPRI5_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ20..23 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI5_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI5_t__ */

/*@}*/

/** @defgroup INTPRI6 IRQ24..27 Priority (INTPRI6) Register
 *  IRQ24..27 Priority (INTPRI6) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI6_Struct
 *! \brief  IRQ24..27 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI6_t__
typedef struct _ADI_NVIC_INTPRI6_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ24..27 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI6_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI6_t__ */

/*@}*/

/** @defgroup INTPRI7 IRQ28..31 Priority (INTPRI7) Register
 *  IRQ28..31 Priority (INTPRI7) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI7_Struct
 *! \brief  IRQ28..31 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI7_t__
typedef struct _ADI_NVIC_INTPRI7_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ28..31 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI7_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI7_t__ */

/*@}*/

/** @defgroup INTPRI8 IRQ32..35 Priority (INTPRI8) Register
 *  IRQ32..35 Priority (INTPRI8) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI8_Struct
 *! \brief  IRQ32..35 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI8_t__
typedef struct _ADI_NVIC_INTPRI8_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ32..35 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI8_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI8_t__ */

/*@}*/

/** @defgroup INTPRI9 IRQ36..39 Priority (INTPRI9) Register
 *  IRQ36..39 Priority (INTPRI9) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI9_Struct
 *! \brief  IRQ36..39 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI9_t__
typedef struct _ADI_NVIC_INTPRI9_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ36..39 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI9_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI9_t__ */

/*@}*/

/** @defgroup INTPRI10 IRQ40..43 Priority (INTPRI10) Register
 *  IRQ40..43 Priority (INTPRI10) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPRI10_Struct
 *! \brief  IRQ40..43 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI10_t__
typedef struct _ADI_NVIC_INTPRI10_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< IRQ40..43 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPRI10_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPRI10_t__ */

/*@}*/

/** @defgroup INTCPID CPUID Base (INTCPID) Register
 *  CPUID Base (INTCPID) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCPID_Struct
 *! \brief  CPUID Base Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCPID_t__
typedef struct _ADI_NVIC_INTCPID_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< CPUID Base */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCPID_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCPID_t__ */

/*@}*/

/** @defgroup INTSTA Interrupt Control State (INTSTA) Register
 *  Interrupt Control State (INTSTA) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTSTA_Struct
 *! \brief  Interrupt Control State Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTSTA_t__
typedef struct _ADI_NVIC_INTSTA_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Interrupt Control State */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTSTA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTSTA_t__ */

/*@}*/

/** @defgroup INTVEC Vector Table Offset (INTVEC) Register
 *  Vector Table Offset (INTVEC) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTVEC_Struct
 *! \brief  Vector Table Offset Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTVEC_t__
typedef struct _ADI_NVIC_INTVEC_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Vector Table Offset */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTVEC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTVEC_t__ */

/*@}*/

/** @defgroup INTAIRC Application Interrupt/Reset Control (INTAIRC) Register
 *  Application Interrupt/Reset Control (INTAIRC) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTAIRC_Struct
 *! \brief  Application Interrupt/Reset Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTAIRC_t__
typedef struct _ADI_NVIC_INTAIRC_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Application Interrupt/Reset Control */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTAIRC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTAIRC_t__ */

/*@}*/

/** @defgroup INTCON0 System Control (INTCON0) Register
 *  System Control (INTCON0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCON0_Struct
 *! \brief  System Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCON0_t__
typedef struct _ADI_NVIC_INTCON0_t {
  union {
    struct {
      unsigned int reserved0   :  1;
      unsigned int SLEEPONEXIT :  1;  /**< Sleeps the core on exit from an ISR */
      unsigned int SLEEPDEEP   :  1;  /**< deep sleep flag for HIBERNATE mode */
      unsigned int reserved3   : 13;
    };
    uint16_t VALUE16;
  };
} ADI_NVIC_INTCON0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCON0_t__ */

/*@}*/

/** @defgroup INTCON1 Configuration Control (INTCON1) Register
 *  Configuration Control (INTCON1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCON1_Struct
 *! \brief  Configuration Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCON1_t__
typedef struct _ADI_NVIC_INTCON1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Configuration Control */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCON1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCON1_t__ */

/*@}*/

/** @defgroup INTSHPRIO0 System Handlers 4-7 Priority (INTSHPRIO0) Register
 *  System Handlers 4-7 Priority (INTSHPRIO0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTSHPRIO0_Struct
 *! \brief  System Handlers 4-7 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTSHPRIO0_t__
typedef struct _ADI_NVIC_INTSHPRIO0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< System Handlers 4-7 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTSHPRIO0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTSHPRIO0_t__ */

/*@}*/

/** @defgroup INTSHPRIO1 System Handlers 8-11 Priority (INTSHPRIO1) Register
 *  System Handlers 8-11 Priority (INTSHPRIO1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTSHPRIO1_Struct
 *! \brief  System Handlers 8-11 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTSHPRIO1_t__
typedef struct _ADI_NVIC_INTSHPRIO1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< System Handlers 8-11 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTSHPRIO1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTSHPRIO1_t__ */

/*@}*/

/** @defgroup INTSHPRIO3 System Handlers 12-15 Priority (INTSHPRIO3) Register
 *  System Handlers 12-15 Priority (INTSHPRIO3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTSHPRIO3_Struct
 *! \brief  System Handlers 12-15 Priority Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTSHPRIO3_t__
typedef struct _ADI_NVIC_INTSHPRIO3_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< System Handlers 12-15 Priority */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTSHPRIO3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTSHPRIO3_t__ */

/*@}*/

/** @defgroup INTSHCSR System Handler Control and State (INTSHCSR) Register
 *  System Handler Control and State (INTSHCSR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTSHCSR_Struct
 *! \brief  System Handler Control and State Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTSHCSR_t__
typedef struct _ADI_NVIC_INTSHCSR_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< System Handler Control and State */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTSHCSR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTSHCSR_t__ */

/*@}*/

/** @defgroup INTCFSR Configurable Fault Status (INTCFSR) Register
 *  Configurable Fault Status (INTCFSR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCFSR_Struct
 *! \brief  Configurable Fault Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCFSR_t__
typedef struct _ADI_NVIC_INTCFSR_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Configurable Fault Status */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCFSR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCFSR_t__ */

/*@}*/

/** @defgroup INTHFSR Hard Fault Status (INTHFSR) Register
 *  Hard Fault Status (INTHFSR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTHFSR_Struct
 *! \brief  Hard Fault Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTHFSR_t__
typedef struct _ADI_NVIC_INTHFSR_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Hard Fault Status */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTHFSR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTHFSR_t__ */

/*@}*/

/** @defgroup INTDFSR Debug Fault Status (INTDFSR) Register
 *  Debug Fault Status (INTDFSR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTDFSR_Struct
 *! \brief  Debug Fault Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTDFSR_t__
typedef struct _ADI_NVIC_INTDFSR_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Debug Fault Status */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTDFSR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTDFSR_t__ */

/*@}*/

/** @defgroup INTMMAR Mem Manage Address (INTMMAR) Register
 *  Mem Manage Address (INTMMAR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTMMAR_Struct
 *! \brief  Mem Manage Address Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMAR_t__
typedef struct _ADI_NVIC_INTMMAR_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Mem Manage Address */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTMMAR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMAR_t__ */

/*@}*/

/** @defgroup INTBFAR Bus Fault Address (INTBFAR) Register
 *  Bus Fault Address (INTBFAR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTBFAR_Struct
 *! \brief  Bus Fault Address Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTBFAR_t__
typedef struct _ADI_NVIC_INTBFAR_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Bus Fault Address */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTBFAR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTBFAR_t__ */

/*@}*/

/** @defgroup INTAFSR Auxiliary Fault Status (INTAFSR) Register
 *  Auxiliary Fault Status (INTAFSR) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTAFSR_Struct
 *! \brief  Auxiliary Fault Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTAFSR_t__
typedef struct _ADI_NVIC_INTAFSR_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Auxiliary Fault Status */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTAFSR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTAFSR_t__ */

/*@}*/

/** @defgroup INTPFR0 Processor Feature Register 0 (INTPFR0) Register
 *  Processor Feature Register 0 (INTPFR0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPFR0_Struct
 *! \brief  Processor Feature Register 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPFR0_t__
typedef struct _ADI_NVIC_INTPFR0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Processor Feature Register 0 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPFR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPFR0_t__ */

/*@}*/

/** @defgroup INTPFR1 Processor Feature Register 1 (INTPFR1) Register
 *  Processor Feature Register 1 (INTPFR1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPFR1_Struct
 *! \brief  Processor Feature Register 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPFR1_t__
typedef struct _ADI_NVIC_INTPFR1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Processor Feature Register 1 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPFR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPFR1_t__ */

/*@}*/

/** @defgroup INTDFR0 Debug Feature Register 0 (INTDFR0) Register
 *  Debug Feature Register 0 (INTDFR0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTDFR0_Struct
 *! \brief  Debug Feature Register 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTDFR0_t__
typedef struct _ADI_NVIC_INTDFR0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Debug Feature Register 0 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTDFR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTDFR0_t__ */

/*@}*/

/** @defgroup INTAFR0 Auxiliary Feature Register 0 (INTAFR0) Register
 *  Auxiliary Feature Register 0 (INTAFR0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTAFR0_Struct
 *! \brief  Auxiliary Feature Register 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTAFR0_t__
typedef struct _ADI_NVIC_INTAFR0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Auxiliary Feature Register 0 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTAFR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTAFR0_t__ */

/*@}*/

/** @defgroup INTMMFR0 Memory Model Feature Register 0 (INTMMFR0) Register
 *  Memory Model Feature Register 0 (INTMMFR0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTMMFR0_Struct
 *! \brief  Memory Model Feature Register 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMFR0_t__
typedef struct _ADI_NVIC_INTMMFR0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Memory Model Feature Register 0 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTMMFR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMFR0_t__ */

/*@}*/

/** @defgroup INTMMFR1 Memory Model Feature Register 1 (INTMMFR1) Register
 *  Memory Model Feature Register 1 (INTMMFR1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTMMFR1_Struct
 *! \brief  Memory Model Feature Register 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMFR1_t__
typedef struct _ADI_NVIC_INTMMFR1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Memory Model Feature Register 1 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTMMFR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMFR1_t__ */

/*@}*/

/** @defgroup INTMMFR2 Memory Model Feature Register 2 (INTMMFR2) Register
 *  Memory Model Feature Register 2 (INTMMFR2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTMMFR2_Struct
 *! \brief  Memory Model Feature Register 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMFR2_t__
typedef struct _ADI_NVIC_INTMMFR2_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Memory Model Feature Register 2 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTMMFR2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMFR2_t__ */

/*@}*/

/** @defgroup INTMMFR3 Memory Model Feature Register 3 (INTMMFR3) Register
 *  Memory Model Feature Register 3 (INTMMFR3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTMMFR3_Struct
 *! \brief  Memory Model Feature Register 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMFR3_t__
typedef struct _ADI_NVIC_INTMMFR3_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Memory Model Feature Register 3 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTMMFR3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTMMFR3_t__ */

/*@}*/

/** @defgroup INTISAR0 ISA Feature Register 0 (INTISAR0) Register
 *  ISA Feature Register 0 (INTISAR0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTISAR0_Struct
 *! \brief  ISA Feature Register 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR0_t__
typedef struct _ADI_NVIC_INTISAR0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< ISA Feature Register 0 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTISAR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR0_t__ */

/*@}*/

/** @defgroup INTISAR1 ISA Feature Register 1 (INTISAR1) Register
 *  ISA Feature Register 1 (INTISAR1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTISAR1_Struct
 *! \brief  ISA Feature Register 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR1_t__
typedef struct _ADI_NVIC_INTISAR1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< ISA Feature Register 1 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTISAR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR1_t__ */

/*@}*/

/** @defgroup INTISAR2 ISA Feature Register 2 (INTISAR2) Register
 *  ISA Feature Register 2 (INTISAR2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTISAR2_Struct
 *! \brief  ISA Feature Register 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR2_t__
typedef struct _ADI_NVIC_INTISAR2_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< ISA Feature Register 2 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTISAR2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR2_t__ */

/*@}*/

/** @defgroup INTISAR3 ISA Feature Register 3 (INTISAR3) Register
 *  ISA Feature Register 3 (INTISAR3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTISAR3_Struct
 *! \brief  ISA Feature Register 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR3_t__
typedef struct _ADI_NVIC_INTISAR3_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< ISA Feature Register 3 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTISAR3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR3_t__ */

/*@}*/

/** @defgroup INTISAR4 ISA Feature Register 4 (INTISAR4) Register
 *  ISA Feature Register 4 (INTISAR4) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTISAR4_Struct
 *! \brief  ISA Feature Register 4 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR4_t__
typedef struct _ADI_NVIC_INTISAR4_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< ISA Feature Register 4 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTISAR4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTISAR4_t__ */

/*@}*/

/** @defgroup INTTRGI Software Trigger Interrupt Register (INTTRGI) Register
 *  Software Trigger Interrupt Register (INTTRGI) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTTRGI_Struct
 *! \brief  Software Trigger Interrupt Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTTRGI_t__
typedef struct _ADI_NVIC_INTTRGI_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Software Trigger Interrupt Register */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTTRGI_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTTRGI_t__ */

/*@}*/

/** @defgroup INTPID4 Peripheral Identification Register 4 (INTPID4) Register
 *  Peripheral Identification Register 4 (INTPID4) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPID4_Struct
 *! \brief  Peripheral Identification Register 4 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID4_t__
typedef struct _ADI_NVIC_INTPID4_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Peripheral Identification Register 4 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPID4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID4_t__ */

/*@}*/

/** @defgroup INTPID5 Peripheral Identification Register 5 (INTPID5) Register
 *  Peripheral Identification Register 5 (INTPID5) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPID5_Struct
 *! \brief  Peripheral Identification Register 5 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID5_t__
typedef struct _ADI_NVIC_INTPID5_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Peripheral Identification Register 5 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPID5_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID5_t__ */

/*@}*/

/** @defgroup INTPID6 Peripheral Identification Register 6 (INTPID6) Register
 *  Peripheral Identification Register 6 (INTPID6) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPID6_Struct
 *! \brief  Peripheral Identification Register 6 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID6_t__
typedef struct _ADI_NVIC_INTPID6_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Peripheral Identification Register 6 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPID6_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID6_t__ */

/*@}*/

/** @defgroup INTPID7 Peripheral Identification Register 7 (INTPID7) Register
 *  Peripheral Identification Register 7 (INTPID7) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPID7_Struct
 *! \brief  Peripheral Identification Register 7 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID7_t__
typedef struct _ADI_NVIC_INTPID7_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Peripheral Identification Register 7 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPID7_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID7_t__ */

/*@}*/

/** @defgroup INTPID0 Peripheral Identification Bits7:0 (INTPID0) Register
 *  Peripheral Identification Bits7:0 (INTPID0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPID0_Struct
 *! \brief  Peripheral Identification Bits7:0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID0_t__
typedef struct _ADI_NVIC_INTPID0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Peripheral Identification Bits7:0 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPID0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID0_t__ */

/*@}*/

/** @defgroup INTPID1 Peripheral Identification Bits15:8 (INTPID1) Register
 *  Peripheral Identification Bits15:8 (INTPID1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPID1_Struct
 *! \brief  Peripheral Identification Bits15:8 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID1_t__
typedef struct _ADI_NVIC_INTPID1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Peripheral Identification Bits15:8 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPID1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID1_t__ */

/*@}*/

/** @defgroup INTPID2 Peripheral Identification Bits16:23 (INTPID2) Register
 *  Peripheral Identification Bits16:23 (INTPID2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPID2_Struct
 *! \brief  Peripheral Identification Bits16:23 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID2_t__
typedef struct _ADI_NVIC_INTPID2_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Peripheral Identification Bits16:23 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPID2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID2_t__ */

/*@}*/

/** @defgroup INTPID3 Peripheral Identification Bits24:31 (INTPID3) Register
 *  Peripheral Identification Bits24:31 (INTPID3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTPID3_Struct
 *! \brief  Peripheral Identification Bits24:31 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID3_t__
typedef struct _ADI_NVIC_INTPID3_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Peripheral Identification Bits24:31 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTPID3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTPID3_t__ */

/*@}*/

/** @defgroup INTCID0 Component Identification Bits7:0 (INTCID0) Register
 *  Component Identification Bits7:0 (INTCID0) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCID0_Struct
 *! \brief  Component Identification Bits7:0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCID0_t__
typedef struct _ADI_NVIC_INTCID0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Component Identification Bits7:0 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCID0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCID0_t__ */

/*@}*/

/** @defgroup INTCID1 Component Identification Bits15:8 (INTCID1) Register
 *  Component Identification Bits15:8 (INTCID1) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCID1_Struct
 *! \brief  Component Identification Bits15:8 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCID1_t__
typedef struct _ADI_NVIC_INTCID1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Component Identification Bits15:8 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCID1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCID1_t__ */

/*@}*/

/** @defgroup INTCID2 Component Identification Bits16:23 (INTCID2) Register
 *  Component Identification Bits16:23 (INTCID2) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCID2_Struct
 *! \brief  Component Identification Bits16:23 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCID2_t__
typedef struct _ADI_NVIC_INTCID2_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Component Identification Bits16:23 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCID2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCID2_t__ */

/*@}*/

/** @defgroup INTCID3 Component Identification Bits24:31 (INTCID3) Register
 *  Component Identification Bits24:31 (INTCID3) Register.
 *
 *  
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_NVIC_INTCID3_Struct
 *! \brief  Component Identification Bits24:31 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_INTCID3_t__
typedef struct _ADI_NVIC_INTCID3_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Component Identification Bits24:31 */
    };
    uint32_t VALUE32;
  };
} ADI_NVIC_INTCID3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_INTCID3_t__ */

/*@}*/


#if defined (_MISRA_RULES)
#pragma diag(pop)
#endif /* _MISRA_RULES */


#if defined (__CC_ARM)
#pragma pop
#endif 

#endif
