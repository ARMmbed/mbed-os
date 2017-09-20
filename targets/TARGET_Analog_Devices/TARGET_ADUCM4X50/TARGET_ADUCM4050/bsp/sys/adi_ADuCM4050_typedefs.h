/* ================================================================================

     Project      :   ADuCM4050
     File         :   ADuCM4050_typedefs.h
     Description  :   C Register Structures

     Date         :   Feb 7, 2017

     Copyright (c) 2014-2017 Analog Devices, Inc.  All Rights Reserved.
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

/** @defgroup LOAD 16-bit Load Value (LOAD) Register
 *  16-bit Load Value (LOAD) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_LOAD_Struct
 *! \brief  16-bit Load Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_LOAD_t__
typedef struct _ADI_TMR_LOAD_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Load Value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_LOAD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_LOAD_t__ */

/*@}*/

/** @defgroup CURCNT 16-bit Timer Value (CURCNT) Register
 *  16-bit Timer Value (CURCNT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_CURCNT_Struct
 *! \brief  16-bit Timer Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_CURCNT_t__
typedef struct _ADI_TMR_CURCNT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Current Count */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_CURCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_CURCNT_t__ */

/*@}*/

/** @defgroup CTL Control (CTL) Register
 *  Control (CTL) Register.
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
      unsigned int MODE       :  1;  /**< Timer Mode */
      unsigned int EN         :  1;  /**< Timer Enable */
      unsigned int CLK        :  2;  /**< Clock Select */
      unsigned int RLD        :  1;  /**< Reload Control */
      unsigned int reserved8  :  5;
      unsigned int EVTEN      :  1;  /**< Event Select */
      unsigned int RSTEN      :  1;  /**< Counter and Prescale Reset Enable */
      unsigned int SYNCBYP    :  1;  /**< Synchronization Bypass */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_CTL_t__ */

/*@}*/

/** @defgroup CLRINT Clear Interrupt (CLRINT) Register
 *  Clear Interrupt (CLRINT) Register.
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
      unsigned int TIMEOUT    :  1;  /**< Clear Timeout Interrupt */
      unsigned int EVTCAPT    :  1;  /**< Clear Captured Event Interrupt */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_CLRINT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_CLRINT_t__ */

/*@}*/

/** @defgroup CAPTURE Capture (CAPTURE) Register
 *  Capture (CAPTURE) Register.
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
      unsigned int VALUE      : 16;  /**< 16-bit Captured Value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_CAPTURE_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_CAPTURE_t__ */

/*@}*/

/** @defgroup ALOAD 16-bit Load Value, Asynchronous (ALOAD) Register
 *  16-bit Load Value, Asynchronous (ALOAD) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_ALOAD_Struct
 *! \brief  16-bit Load Value, Asynchronous Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_ALOAD_t__
typedef struct _ADI_TMR_ALOAD_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Load Value, Asynchronous */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_ALOAD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_ALOAD_t__ */

/*@}*/

/** @defgroup ACURCNT 16-bit Timer Value, Asynchronous (ACURCNT) Register
 *  16-bit Timer Value, Asynchronous (ACURCNT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_ACURCNT_Struct
 *! \brief  16-bit Timer Value, Asynchronous Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_ACURCNT_t__
typedef struct _ADI_TMR_ACURCNT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Counter Value */
    };
    uint16_t VALUE16;
  };
} ADI_TMR_ACURCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_ACURCNT_t__ */

/*@}*/

/** @defgroup STAT Status (STAT) Register
 *  Status (STAT) Register.
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
      unsigned int TIMEOUT    :  1;  /**< Timeout Event Occurred */
      unsigned int CAPTURE    :  1;  /**< Capture Event Pending */
      unsigned int reserved2  :  4;
      unsigned int BUSY       :  1;  /**< Timer Busy */
      unsigned int PDOK       :  1;  /**< Clear Interrupt Register Synchronization */
      unsigned int CNTRST     :  1;  /**< Counter Reset Occurring */
      unsigned int reserved9  :  7;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_STAT_t__ */

/*@}*/

/** @defgroup PWMCTL PWM Control Register (PWMCTL) Register
 *  PWM Control Register (PWMCTL) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_TMR_PWMCTL_MATCH
 *! \brief  PWM Match Enabled (MATCH) Enumerations
 *  ========================================================================= */
typedef enum
{
  TMR_PWMCTL_PWM_TOGGLE = 0,  /**< PWM in toggle mode */
  TMR_PWMCTL_PWM_MATCH  = 1   /**< PWM in match mode  */
} ADI_TMR_PWMCTL_MATCH;


/*  =========================================================================
 *! \enum   ADI_TMR_PWMCTL_IDLESTATE
 *! \brief  PWM Idle State (IDLESTATE) Enumerations
 *  ========================================================================= */
typedef enum
{
  TMR_PWMCTL_IDLE_LOW  = 0,  /**< PWM idles low  */
  TMR_PWMCTL_IDLE_HIGH = 1   /**< PWM idles high */
} ADI_TMR_PWMCTL_IDLESTATE;


/* ==========================================================================
 *! \struct ADI_TMR_PWMCTL_Struct
 *! \brief  PWM Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_PWMCTL_t__
typedef struct _ADI_TMR_PWMCTL_t {
  union {
    struct {
      unsigned int MATCH      :  1;  /**< PWM Match Enabled */
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

/** @defgroup EVENTSELECT Timer Event Selection Register (EVENTSELECT) Register
 *  Timer Event Selection Register (EVENTSELECT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_EVENTSELECT_Struct
 *! \brief  Timer Event Selection Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_EVENTSELECT_t__
typedef struct _ADI_TMR_EVENTSELECT_t {
  union {
    struct {
      unsigned int EVTRANGE   :  6;  /**< Event Select Range */
      unsigned int reserved6  : 10;
    };
    uint16_t VALUE16;
  };
} ADI_TMR_EVENTSELECT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_EVENTSELECT_t__ */

/*@}*/

/** @defgroup LOAD 16-bit load value (LOAD) Register
 *  16-bit load value (LOAD) Register.
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

/** @defgroup CLRINT Clear interrupt (CLRINT) Register
 *  Clear interrupt (CLRINT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_TMR_RGB_CLRINT_Struct
 *! \brief  Clear interrupt Register bit field structure
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
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CR0_Struct
 *! \brief  RTC Control 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR0_t__
typedef struct _ADI_RTC_CR0_t {
  union {
    struct {
      unsigned int CNTEN      :  1;  /**< Global Enable for the RTC */
      unsigned int ALMEN      :  1;  /**< Enable the RTC Alarm (Absolute) Operation */
      unsigned int ALMINTEN   :  1;  /**< Enable ALMINT Sourced Alarm Interrupts to the CPU */
      unsigned int TRMEN      :  1;  /**< Enable RTC Digital Trimming */
      unsigned int MOD60ALMEN :  1;  /**< Enable RTC Modulo-60 Counting of Time Past a Modulo-60 Boundary */
      unsigned int MOD60ALM   :  6;  /**< Periodic, Modulo-60 Alarm Time in Prescaled RTC Time Units Beyond a Modulo-60 Boundary */
      unsigned int MOD60ALMINTEN :  1;  /**< Enable Periodic Modulo-60 RTC Alarm  Sourced Interrupts to the CPU */
      unsigned int ISOINTEN      :  1;  /**< Enable ISOINT Sourced Interrupts to the CPU When Isolation of the RTC Power Domain is Activated and Subsequently De-activated */
      unsigned int WPNDERRINTEN  :  1;  /**< Enable Write Pending Error Sourced Interrupts to the CPU When an RTC Register-write Pending Error Occurs */
      unsigned int WSYNCINTEN    :  1;  /**< Enable Write Synchronization Sourced Interrupts to the CPU */
      unsigned int WPNDINTEN     :  1;  /**< Enable Write Pending Sourced Interrupts to the CPU */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR0_t__ */

/*@}*/

/** @defgroup SR0 RTC Status 0 (SR0) Register
 *  RTC Status 0 (SR0) Register.
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
      unsigned int ALMINT     :  1;  /**< Alarm Interrupt Source */
      unsigned int MOD60ALMINT :  1;  /**< Modulo-60 RTC Alarm Interrupt Source */
      unsigned int ISOINT      :  1;  /**< RTC Power-Domain Isolation Interrupt Source */
      unsigned int WPNDERRINT  :  1;  /**< Write Pending Error Interrupt Source */
      unsigned int WSYNCINT    :  1;  /**< Write Synchronisation Interrupt */
      unsigned int WPNDINT     :  1;  /**< Write Pending Interrupt */
      unsigned int WSYNCCR0    :  1;  /**< Synchronisation Status of Posted Writes to CR0 */
      unsigned int WSYNCSR0    :  1;  /**< Synchronisation Status of Posted Writes to SR0 */
      unsigned int WSYNCCNT0   :  1;  /**< Synchronisation Status of Posted Writes to CNT0 */
      unsigned int WSYNCCNT1   :  1;  /**< Synchronisation Status of Posted Writes to CNT1 */
      unsigned int WSYNCALM0   :  1;  /**< Synchronisation Status of Posted Writes to ALM0 */
      unsigned int WSYNCALM1   :  1;  /**< Synchronisation Status of Posted Writes to ALM1 */
      unsigned int WSYNCTRM    :  1;  /**< Synchronisation Status of Posted Writes to TRM */
      unsigned int ISOENB      :  1;  /**< Visibility of 32kHz Sourced Registers */
      unsigned int reserved15  :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR0_t__ */

/*@}*/

/** @defgroup SR1 RTC Status 1 (SR1) Register
 *  RTC Status 1 (SR1) Register.
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
      unsigned int WPNDCR0    :  1;  /**< Pending Status of Posted Writes to CR0 */
      unsigned int WPNDSR0    :  1;  /**< Pending Status of Posted Clearances of Interrupt Sources in SR0 */
      unsigned int WPNDCNT0   :  1;  /**< Pending Status of Posted Writes to CNT0 */
      unsigned int WPNDCNT1   :  1;  /**< Pending Status of Posted Writes to CNT1 */
      unsigned int WPNDALM0   :  1;  /**< Pending Status of Posted Writes to ALM0 */
      unsigned int WPNDALM1   :  1;  /**< Pending Status of Posted Writes to ALM1 */
      unsigned int WPNDTRM    :  1;  /**< Pending Status of Posted Writes to TRM */
      unsigned int reserved14 :  2;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR1_t__ */

/*@}*/

/** @defgroup CNT0 RTC Count 0 (CNT0) Register
 *  RTC Count 0 (CNT0) Register.
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
      unsigned int VALUE      : 16;  /**< Lower 16 Prescaled (Non-Fractional) Bits of the RTC Real-Time Count */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CNT0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CNT0_t__ */

/*@}*/

/** @defgroup CNT1 RTC Count 1 (CNT1) Register
 *  RTC Count 1 (CNT1) Register.
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
      unsigned int VALUE      : 16;  /**< Upper 16 Prescaled (Non-Fractional) Bits of the RTC Real-Time Count */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CNT1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CNT1_t__ */

/*@}*/

/** @defgroup ALM0 RTC Alarm 0 (ALM0) Register
 *  RTC Alarm 0 (ALM0) Register.
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
      unsigned int VALUE      : 16;  /**< Lower 16 Prescaled (i.e. Non-Fractional) Bits of the RTC Alarm Target Time */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_ALM0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_ALM0_t__ */

/*@}*/

/** @defgroup ALM1 RTC Alarm 1 (ALM1) Register
 *  RTC Alarm 1 (ALM1) Register.
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
      unsigned int VALUE      : 16;  /**< Upper 16 Prescaled (Non-Fractional) Bits of the RTC Alarm Target Time */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_ALM1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_ALM1_t__ */

/*@}*/

/** @defgroup TRM RTC Trim (TRM) Register
 *  RTC Trim (TRM) Register.
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
      unsigned int VALUE      :  3;  /**< Trim Value in Prescaled RTC Time Units to Be Added or Subtracted from the RTC Count at the End of a Periodic Interval Selected by TRM:TRMIVL */
      unsigned int ADD        :  1;  /**< Trim Polarity */
      unsigned int IVL        :  2;  /**< Trim Interval in Prescaled RTC Time Units */
      unsigned int IVL2EXPMIN :  4;  /**< Minimum Power-of-two Interval of Prescaled RTC Time Units Which TRM:TRMIVL TRMIVL Can Select */
      unsigned int reserved10 :  6;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_TRM_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_TRM_t__ */

/*@}*/

/** @defgroup GWY RTC Gateway (GWY) Register
 *  RTC Gateway (GWY) Register.
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
      unsigned int SWKEY      : 16;  /**< Software-keyed Command Issued by the CPU */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_GWY_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_GWY_t__ */

/*@}*/

/** @defgroup CR1 RTC Control 1 (CR1) Register
 *  RTC Control 1 (CR1) Register.
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
      unsigned int CNTINTEN   :  1;  /**< Enable for the RTC Count Interrupt Source */
      unsigned int PSINTEN    :  1;  /**< Enable for the Prescaled, Modulo-1 Interrupt Source, in SR2:RTCPSINT */
      unsigned int TRMINTEN   :  1;  /**< Enable for the RTC Trim Interrupt Source, in SR2:RTCTRMINT */
      unsigned int CNTROLLINTEN :  1;  /**< Enable for the RTC Count Roll-Over Interrupt Source, in SR2:RTCCNTROLLINT */
      unsigned int CNTMOD60ROLLINTEN :  1;  /**< Enable for the RTC Modulo-60 Count Roll-Over Interrupt Source, in SR2:RTCCNTMOD60ROLLINT    */
      unsigned int PRESCALE2EXP      :  4;  /**< Prescale Power of 2 Division Factor for the RTC Base Clock */
      unsigned int reserved9         :  7;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR1_t__ */

/*@}*/

/** @defgroup SR2 RTC Status 2 (SR2) Register
 *  RTC Status 2 (SR2) Register.
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
      unsigned int CNTINT     :  1;  /**< RTC Count Interrupt Source */
      unsigned int PSINT      :  1;  /**< RTC Prescaled, Modulo-1 Boundary Interrupt Source */
      unsigned int TRMINT     :  1;  /**< RTC Trim Interrupt Source */
      unsigned int CNTROLLINT :  1;  /**< RTC Count Roll-Over Interrupt Source */
      unsigned int CNTMOD60ROLLINT :  1;  /**< RTC Modulo-60 Count Roll-Over Interrupt Source */
      unsigned int CNTROLL         :  1;  /**< RTC Count Roll-Over */
      unsigned int CNTMOD60ROLL    :  1;  /**< RTC Count Modulo-60 Roll-Over */
      unsigned int TRMBDYMIR       :  1;  /**< Mirror of MOD:RTCTRMBDY */
      unsigned int reserved8       :  4;
      unsigned int WPNDCR1MIR      :  1;  /**< Pending Status of Posted Writes to CR1 */
      unsigned int WPNDALM2MIR     :  1;  /**< Pending Status of Posted Writes to ALM2 */
      unsigned int WSYNCCR1MIR     :  1;  /**< Synchronization Status of Posted Writes to CR1 */
      unsigned int WSYNCALM2MIR    :  1;  /**< Synchronization Status of Posted Writes to ALM2 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR2_t__ */

/*@}*/

/** @defgroup SNAP0 RTC Snapshot 0 (SNAP0) Register
 *  RTC Snapshot 0 (SNAP0) Register.
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
      unsigned int VALUE      : 16;  /**< Constituent Part of the 47-bit Input Capture Channel 0, Containing a Sticky Snapshot of  CNT0 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SNAP0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SNAP0_t__ */

/*@}*/

/** @defgroup SNAP1 RTC Snapshot 1 (SNAP1) Register
 *  RTC Snapshot 1 (SNAP1) Register.
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
      unsigned int VALUE      : 16;  /**< Part of the 47-bit Input Capture Channel 0 Containing a Sticky Snapshot of  CNT1 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SNAP1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SNAP1_t__ */

/*@}*/

/** @defgroup SNAP2 RTC Snapshot 2 (SNAP2) Register
 *  RTC Snapshot 2 (SNAP2) Register.
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
      unsigned int VALUE      : 15;  /**< Part of the 47-bit Input Capture Channel 0 Containing a Sticky Snapshot of CNT2 */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SNAP2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SNAP2_t__ */

/*@}*/

/** @defgroup MOD RTC Modulo (MOD) Register
 *  RTC Modulo (MOD) Register.
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
      unsigned int CNTMOD60   :  6;  /**< Modulo-60 Value of the RTC Count: CNT1 and CNT0 */
      unsigned int INCR       :  4;  /**< Most Recent Increment Value Added to the RTC Count in CNT1 and CNT0 */
      unsigned int TRMBDY     :  1;  /**< Trim Boundary Indicator */
      unsigned int CNT0_4TOZERO :  5;  /**< Mirror of CNT0[4:0] */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_MOD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_MOD_t__ */

/*@}*/

/** @defgroup CNT2 RTC Count 2 (CNT2) Register
 *  RTC Count 2 (CNT2) Register.
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
      unsigned int VALUE      : 15;  /**< Fractional Bits of the RTC Real-Time Count */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CNT2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CNT2_t__ */

/*@}*/

/** @defgroup ALM2 RTC Alarm 2 (ALM2) Register
 *  RTC Alarm 2 (ALM2) Register.
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
      unsigned int VALUE      : 15;  /**< Fractional Bits of the Alarm Target Time */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_ALM2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_ALM2_t__ */

/*@}*/

/** @defgroup SR3 RTC Status 3 (SR3) Register
 *  RTC Status 3 (SR3) Register.
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
      unsigned int IC0IRQ     :  1;  /**< Sticky Interrupt Source for the RTC Input Capture Channel 0 */
      unsigned int reserved1  :  1;
      unsigned int IC2IRQ     :  1;  /**< Sticky Interrupt Source for the RTC Input Capture Channel 2 */
      unsigned int IC3IRQ     :  1;  /**< Sticky Interrupt Source for the RTC Input Capture Channel 3 */
      unsigned int IC4IRQ     :  1;  /**< Sticky Interrupt Source for the RTC Input Capture Channel 4 */
      unsigned int SS1FEIRQ   :  1;  /**< Sticky Interrupt Source for the SensorStrobe Channel 1 Falling Edge */
      unsigned int SS2FEIRQ   :  1;  /**< Sticky Interrupt Source for the SensorStrobe Channel 2 Falling Edge */
      unsigned int SS3FEIRQ   :  1;  /**< Sticky Interrupt Source for the SensorStrobe Channel 3 Falling Edge */
      unsigned int SS4FEIRQ   :  1;  /**< Sticky Interrupt Source for the SensorStrobe Channel 4 Falling Edge */
      unsigned int SS1IRQ     :  1;  /**< Sticky Interrupt Source for SensorStrobe Channel 1 */
      unsigned int SS2IRQ     :  1;  /**< Sticky Interrupt Source for the SensorStrobe Channel 2 */
      unsigned int SS3IRQ     :  1;  /**< Sticky Interrupt Source for the SensorStrobe Channel 3 */
      unsigned int SS4IRQ     :  1;  /**< Sticky Interrupt Source for the SensorStrobe Channel 4 */
      unsigned int ALMINTMIR  :  1;  /**< Read-only Mirror of the SR0:ALMINT Interrupt Source */
      unsigned int reserved14 :  2;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR3_t__ */

/*@}*/

/** @defgroup CR2IC RTC Control 2 for Configuring Input Capture Channels (CR2IC) Register
 *  RTC Control 2 for Configuring Input Capture Channels (CR2IC) Register.
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
      unsigned int IC0EN      :  1;  /**< Enable for the RTC Input Capture Channel 0 */
      unsigned int reserved1  :  1;
      unsigned int IC2EN      :  1;  /**< Enable for the RTC Input Capture Channel 2 */
      unsigned int IC3EN      :  1;  /**< Enable for the RTC Input Capture Channel 3 */
      unsigned int IC4EN      :  1;  /**< Enable for the RTC Input Capture Channel 4 */
      unsigned int IC0LH      :  1;  /**< Polarity of the Active-Going Capture Edge for the RTC Input Capture Channel 0 */
      unsigned int reserved6  :  1;
      unsigned int IC2LH      :  1;  /**< Polarity of the Active-going Capture Edge for the Input Capture Channel 2 */
      unsigned int IC3LH      :  1;  /**< Polarity of the Active-going Capture Edge for the Input Capture Channel 3 */
      unsigned int IC4LH      :  1;  /**< Polarity of the Active-going Capture Edge for the Input Capture Channel 4 */
      unsigned int IC0IRQEN   :  1;  /**< Interrupt Enable for the RTC Input Capture Channel 0 */
      unsigned int reserved11 :  1;
      unsigned int IC2IRQEN   :  1;  /**< Interrupt Enable for the RTC Input Capture Channel 2 */
      unsigned int IC3IRQEN   :  1;  /**< Interrupt Enable for the RTC Input Capture Channel 3 */
      unsigned int IC4IRQEN   :  1;  /**< Interrupt Enable for the RTC Input Capture Channel 4 */
      unsigned int ICOWUSEN   :  1;  /**< Enable Overwrite of Unread Snapshots for All Input Capture Channels */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR2IC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR2IC_t__ */

/*@}*/

/** @defgroup CR3SS RTC Control 3 for Configuring SensorStrobe Channel (CR3SS) Register
 *  RTC Control 3 for Configuring SensorStrobe Channel (CR3SS) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CR3SS_Struct
 *! \brief  RTC Control 3 for Configuring SensorStrobe Channel Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR3SS_t__
typedef struct _ADI_RTC_CR3SS_t {
  union {
    struct {
      unsigned int reserved0  :  1;
      unsigned int SS1EN      :  1;  /**< Enable for SensorStrobe Channel 1 */
      unsigned int SS2EN      :  1;  /**< Enable for the SensorStrobe Channel 2 */
      unsigned int SS3EN      :  1;  /**< Enable for the SensorStrobe Channel 3 */
      unsigned int SS4EN      :  1;  /**< Enable for the SensorStrobe Channel 4 */
      unsigned int SS1FEIRQEN :  1;  /**< Falling Edge Interrupt Enable for the SensorStrobe Channel 1 */
      unsigned int SS2FEIRQEN :  1;  /**< Falling Edge Interrupt Enable for the SensorStrobe Channel 2 */
      unsigned int SS3FEIRQEN :  1;  /**< Falling Edge Interrupt Enable for the SensorStrobe Channel 3 */
      unsigned int SS4FEIRQEN :  1;  /**< Falling Edge Interrupt Enable for the SensorStrobe Channel 4 */
      unsigned int SS1IRQEN   :  1;  /**< Interrupt Enable for SensorStrobe Channel 1 */
      unsigned int SS2IRQEN   :  1;  /**< Posedge EdgeInterrupt Enable for the SensorStrobe Channel 2 */
      unsigned int SS3IRQEN   :  1;  /**< Posedge EdgeInterrupt Enable for the SensorStrobe Channel 3 */
      unsigned int SS4IRQEN   :  1;  /**< Posedge EdgeInterrupt Enable for the SensorStrobe Channel 4 */
      unsigned int reserved13 :  3;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR3SS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR3SS_t__ */

/*@}*/

/** @defgroup CR4SS RTC Control 4 for Configuring SensorStrobe Channel (CR4SS) Register
 *  RTC Control 4 for Configuring SensorStrobe Channel (CR4SS) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_RTC_CR4SS_SS1MSKEN
 *! \brief  Enable for Thermometer-Code Masking of the SensorStrobe Channel 1 (SS1MSKEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR4SS_NO_MSK    = 0,  /**< Do not apply a mask to SensorStrobe Channel 1 Register */
  RTC_CR4SS_THERM_MSK = 1   /**< Apply thermometer decoded mask                         */
} ADI_RTC_CR4SS_SS1MSKEN;


/* ==========================================================================
 *! \struct ADI_RTC_CR4SS_Struct
 *! \brief  RTC Control 4 for Configuring SensorStrobe Channel Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR4SS_t__
typedef struct _ADI_RTC_CR4SS_t {
  union {
    struct {
      unsigned int reserved0  :  1;
      unsigned int SS1MSKEN   :  1;  /**< Enable for Thermometer-Code Masking of the SensorStrobe Channel 1 */
      unsigned int SS2MSKEN   :  1;  /**< Enable for Thermometer-Code Masking of the SensorStrobe Channel 2 */
      unsigned int SS3MSKEN   :  1;  /**< Enable for Thermometer-Code Masking of the SensorStrobe Channel 3 */
      unsigned int SS4MSKEN   :  1;  /**< Enable for Thermometer-Code Masking of the SensorStrobe Channel 4 */
      unsigned int SS1POL     :  1;  /**< SensorSTrobe Channel 1 Polarity Control */
      unsigned int SS2POL     :  1;  /**< SensorStrobe Channel 2 Polarity Control */
      unsigned int SS3POL     :  1;  /**< SensorStrobe Channel 3 Polarity Control */
      unsigned int SS4POL     :  1;  /**< SensorStrobe Channel 4 Polarity Control */
      unsigned int SS1ARLEN   :  1;  /**< Enable for Fine Control on SensorStrobe Channel 1 Period and Duty Cycle */
      unsigned int SS2ARLEN   :  1;  /**< Enable for Fine Control on SensorStrobe Channel 2 Period and Duty Cycle */
      unsigned int SS3ARLEN   :  1;  /**< Enable for Fine Control on SensorStrobe Channel 3 Period and Duty Cycle */
      unsigned int reserved12 :  4;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR4SS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR4SS_t__ */

/*@}*/

/** @defgroup SSMSK RTC Mask for SensorStrobe Channel (SSMSK) Register
 *  RTC Mask for SensorStrobe Channel (SSMSK) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SSMSK_Struct
 *! \brief  RTC Mask for SensorStrobe Channel Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SSMSK_t__
typedef struct _ADI_RTC_SSMSK_t {
  union {
    struct {
      unsigned int SS1MSK     :  4;  /**< Concatenation of Thermometer-Encoded Masks for the 16-bit SensorStrobe Channels */
      unsigned int SS2MSK     :  4;  /**< SensorStrobe Channel 2 Period Control */
      unsigned int SS3MSK     :  4;  /**< SensorStrobe Channel 3 Period Control */
      unsigned int SS4MSK     :  4;  /**< SensorStrobe Channel 4 Period Control */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SSMSK_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SSMSK_t__ */

/*@}*/

/** @defgroup IC2 RTC Input Capture Channel 2 (IC2) Register
 *  RTC Input Capture Channel 2 (IC2) Register.
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
      unsigned int IC2        : 16;  /**< RTC Input Capture Channel 2 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_IC2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_IC2_t__ */

/*@}*/

/** @defgroup IC3 RTC Input Capture Channel 3 (IC3) Register
 *  RTC Input Capture Channel 3 (IC3) Register.
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
      unsigned int IC3        : 16;  /**< RTC Input Capture Channel 3 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_IC3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_IC3_t__ */

/*@}*/

/** @defgroup IC4 RTC Input Capture Channel 4 (IC4) Register
 *  RTC Input Capture Channel 4 (IC4) Register.
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
      unsigned int IC4        : 16;  /**< RTC Input Capture Channel 4 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_IC4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_IC4_t__ */

/*@}*/

/** @defgroup SS1 RTC SensorStrobe Channel 1 (SS1) Register
 *  RTC SensorStrobe Channel 1 (SS1) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS1_Struct
 *! \brief  RTC SensorStrobe Channel 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS1_t__
typedef struct _ADI_RTC_SS1_t {
  union {
    struct {
      unsigned int SS1        : 16;  /**< SensorStrobe Channel 1 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS1_t__ */

/*@}*/

/** @defgroup SS2 RTC SensorStrobe Channel 2 (SS2) Register
 *  RTC SensorStrobe Channel 2 (SS2) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS2_Struct
 *! \brief  RTC SensorStrobe Channel 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS2_t__
typedef struct _ADI_RTC_SS2_t {
  union {
    struct {
      unsigned int SS2        : 16;  /**< SensorStrobe Channel 2 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS2_t__ */

/*@}*/

/** @defgroup SS3 RTC SensorStrobe Channel 3 (SS3) Register
 *  RTC SensorStrobe Channel 3 (SS3) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS3_Struct
 *! \brief  RTC SensorStrobe Channel 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS3_t__
typedef struct _ADI_RTC_SS3_t {
  union {
    struct {
      unsigned int SS3        : 16;  /**< SensorStrobe Channel 3 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS3_t__ */

/*@}*/

/** @defgroup SS4 RTC SensorStrobe Channel 4 (SS4) Register
 *  RTC SensorStrobe Channel 4 (SS4) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS4_Struct
 *! \brief  RTC SensorStrobe Channel 4 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS4_t__
typedef struct _ADI_RTC_SS4_t {
  union {
    struct {
      unsigned int SS4        : 16;  /**< SensorStrobe Channel 4 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS4_t__ */

/*@}*/

/** @defgroup SR4 RTC Status 4 (SR4) Register
 *  RTC Status 4 (SR4) Register.
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
      unsigned int WSYNCSR3   :  1;  /**< Synchronisation Status of Posted Writes to SR3 */
      unsigned int WSYNCCR2IC :  1;  /**< Synchronization Status of Posted Writes to RTC Control 2 for Configuring Input Capture Channels Register */
      unsigned int WSYNCCR3SS :  1;  /**< Synchronization Status of Posted Writes to RTC Control 3 for Configuring SensorStrobe Channel Register */
      unsigned int WSYNCCR4SS :  1;  /**< Synchronization Status of Posted Writes to RTC Control 4 for Configuring SensorStrobe Channel Register */
      unsigned int WSYNCSSMSK :  1;  /**< Synchronization Status of Posted Writes to Masks for SensorStrobe Channel Register */
      unsigned int reserved5  :  1;
      unsigned int WSYNCSS1   :  1;  /**< Synchronization Status of Posted Writes to SensorStrobe Channel 1 */
      unsigned int WSYNCSS2   :  1;  /**< Synchronization Status of Posted Writes to SensorStrobe Channel 2 */
      unsigned int WSYNCSS3   :  1;  /**< Synchronization Status of Posted Writes to SensorStrobe Channel 3 */
      unsigned int WSYNCSS4   :  1;  /**< Synchronization Status of Posted Writes to SensorStrobe Channel 4 */
      unsigned int RSYNCIC0   :  1;  /**< Synchronization Status of Posted Reads of RTC Input Channel 0 */
      unsigned int reserved11 :  1;
      unsigned int RSYNCIC2   :  1;  /**< Synchronization Status of Posted Reads of RTC Input Channel 2 */
      unsigned int RSYNCIC3   :  1;  /**< Synchronization Status of Posted Reads of RTC Input Channel 3 */
      unsigned int RSYNCIC4   :  1;  /**< Synchronization Status of Posted Reads of RTC Input Channel 4 */
      unsigned int WSYNCSSMSKOT :  1;  /**< Synchronization Status of Posted Reads Writes to Mask for SensorStrobe Channels on Time Control Register */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR4_t__ */

/*@}*/

/** @defgroup SR5 RTC Status 5 (SR5) Register
 *  RTC Status 5 (SR5) Register.
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
      unsigned int WPENDSR3   :  1;  /**< Pending Status of Posted Clearances of Interrupt Sources in RTC Status 3 Register */
      unsigned int WPENDCR2IC :  1;  /**< Pending Status of Posted Writes to RTC Control 2 for Configuring Input Capture Channels Register */
      unsigned int WPENDCR3SS :  1;  /**< Pending Status of Posted Writes to RTC Control 3 for Configuring SensorStrobe Channel Register */
      unsigned int WPENDCR4SS :  1;  /**< Pending Status of Posted Writes to RTC Control 4 for Configuring SensorStrobe Channel Register */
      unsigned int WPENDSSMSK :  1;  /**< Pending Status of Posted Writes to RTC Masks for SensorStrobe Channel Register */
      unsigned int reserved5  :  1;
      unsigned int WPENDSS1   :  1;  /**< Pending Status of Posted Writes to SensorStrobe Channel 1 */
      unsigned int WPENDSS2   :  1;  /**< Pending Status of Posted Writes to SensorStrobe Channel 2 */
      unsigned int WPENDSS3   :  1;  /**< Pending Status of Posted Writes to SensorStrobe Channel 3 */
      unsigned int WPENDSS4   :  1;  /**< Pending Status of Posted Writes to SensorStrobe Channel 4 */
      unsigned int RPENDIC0   :  1;  /**< Pending Status of Posted Reads of Input Capture Channel 0 */
      unsigned int reserved11 :  1;
      unsigned int RPENDIC2   :  1;  /**< Pending Status of Posted Reads of IC2 */
      unsigned int RPENDIC3   :  1;  /**< Pending Status of Posted Reads of IC3 */
      unsigned int RPENDIC4   :  1;  /**< Pending Status of Posted Reads of IC4 */
      unsigned int WPENDSSMSKOT :  1;  /**< Pending Status of Posted Writes to RTC Masks for SensorStrobe Channel Register */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR5_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR5_t__ */

/*@}*/

/** @defgroup SR6 RTC Status 6 (SR6) Register
 *  RTC Status 6 (SR6) Register.
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
      unsigned int IC0UNR     :  1;  /**< Sticky Unread Status of the Input Capture Channel 0 */
      unsigned int reserved1  :  1;
      unsigned int IC2UNR     :  1;  /**< Sticky Unread Status of the Input Capture Channel 2 */
      unsigned int IC3UNR     :  1;  /**< Sticky Unread Status of the Input Capture Channel 3 */
      unsigned int IC4UNR     :  1;  /**< Sticky Unread Status of the Input Capture Channel 4 */
      unsigned int reserved5  :  3;
      unsigned int IC0SNAP    :  1;  /**< Confirmation That RTC Snapshot 0, 1, 2 Registers Reflect the Value of Input-Capture Channel RTC Input Capture Channel 0 */
      unsigned int FRZCNTPTR  :  2;  /**< Pointer for the Triple-Read Sequence of FRZCNT */
      unsigned int reserved11 :  5;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR6_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR6_t__ */

/*@}*/

/** @defgroup SS1TGT RTC SensorStrobe Channel 1 Target (SS1TGT) Register
 *  RTC SensorStrobe Channel 1 Target (SS1TGT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS1TGT_Struct
 *! \brief  RTC SensorStrobe Channel 1 Target Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS1TGT_t__
typedef struct _ADI_RTC_SS1TGT_t {
  union {
    struct {
      unsigned int SS1TGT     : 16;  /**< Current Target Value for the SensorStrobe Channel 1 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS1TGT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS1TGT_t__ */

/*@}*/

/** @defgroup FRZCNT RTC Freeze Count (FRZCNT) Register
 *  RTC Freeze Count (FRZCNT) Register.
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
      unsigned int FRZCNT     : 16;  /**< RTC Freeze Count. Coherent, Triple 16-Bit Read of the 47-Bit RTC Count */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_FRZCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_FRZCNT_t__ */

/*@}*/

/** @defgroup SS2TGT RTC SensorStrobe Channel 2 Target (SS2TGT) Register
 *  RTC SensorStrobe Channel 2 Target (SS2TGT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS2TGT_Struct
 *! \brief  RTC SensorStrobe Channel 2 Target Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS2TGT_t__
typedef struct _ADI_RTC_SS2TGT_t {
  union {
    struct {
      unsigned int SS2TGT     : 16;  /**< Current, Cumulative Target Time for SensorStrobe Channel 2, Taking Account of Any Auto-reloading */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS2TGT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS2TGT_t__ */

/*@}*/

/** @defgroup SS3TGT RTC SensorStrobe Channel 3 Target (SS3TGT) Register
 *  RTC SensorStrobe Channel 3 Target (SS3TGT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS3TGT_Struct
 *! \brief  RTC SensorStrobe Channel 3 Target Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS3TGT_t__
typedef struct _ADI_RTC_SS3TGT_t {
  union {
    struct {
      unsigned int SS3TGT     : 16;  /**< Current, Cumulative Target Time for SensorStrobe Channel 3, Taking Account of Any Auto-reloading */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS3TGT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS3TGT_t__ */

/*@}*/

/** @defgroup SS1LOWDUR RTC Auto-Reload Low Duration for SensorStrobe Channel 1 (SS1LOWDUR) Register
 *  RTC Auto-Reload Low Duration for SensorStrobe Channel 1 (SS1LOWDUR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS1LOWDUR_Struct
 *! \brief  RTC Auto-Reload Low Duration for SensorStrobe Channel 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS1LOWDUR_t__
typedef struct _ADI_RTC_SS1LOWDUR_t {
  union {
    struct {
      unsigned int SS1LOWDUR  : 16;  /**< Low Duration for SensorStrobe Channel 1. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS1LOWDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS1LOWDUR_t__ */

/*@}*/

/** @defgroup SS2LOWDUR RTC Auto-Reload Low Duration for SensorStrobe Channel 2 (SS2LOWDUR) Register
 *  RTC Auto-Reload Low Duration for SensorStrobe Channel 2 (SS2LOWDUR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS2LOWDUR_Struct
 *! \brief  RTC Auto-Reload Low Duration for SensorStrobe Channel 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS2LOWDUR_t__
typedef struct _ADI_RTC_SS2LOWDUR_t {
  union {
    struct {
      unsigned int SS2LOWDUR  : 16;  /**< Low Duration for SensorStrobe Channel 2. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS2LOWDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS2LOWDUR_t__ */

/*@}*/

/** @defgroup SS3LOWDUR RTC Auto-Reload Low Duration for SensorStrobe Channel 3 (SS3LOWDUR) Register
 *  RTC Auto-Reload Low Duration for SensorStrobe Channel 3 (SS3LOWDUR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS3LOWDUR_Struct
 *! \brief  RTC Auto-Reload Low Duration for SensorStrobe Channel 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS3LOWDUR_t__
typedef struct _ADI_RTC_SS3LOWDUR_t {
  union {
    struct {
      unsigned int SS3LOWDUR  : 16;  /**< Low Duration for SensorStrobe Channel 3. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS3LOWDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS3LOWDUR_t__ */

/*@}*/

/** @defgroup SS1HIGHDUR RTC Auto-Reload High Duration for SensorStrobe Channel 1 (SS1HIGHDUR) Register
 *  RTC Auto-Reload High Duration for SensorStrobe Channel 1 (SS1HIGHDUR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS1HIGHDUR_Struct
 *! \brief  RTC Auto-Reload High Duration for SensorStrobe Channel 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS1HIGHDUR_t__
typedef struct _ADI_RTC_SS1HIGHDUR_t {
  union {
    struct {
      unsigned int SS1HIGHDUR : 16;  /**< High Duration for SensorStrobe Channel 1. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS1HIGHDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS1HIGHDUR_t__ */

/*@}*/

/** @defgroup SS2HIGHDUR RTC Auto-Reload High Duration for SensorStrobe Channel 2 (SS2HIGHDUR) Register
 *  RTC Auto-Reload High Duration for SensorStrobe Channel 2 (SS2HIGHDUR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS2HIGHDUR_Struct
 *! \brief  RTC Auto-Reload High Duration for SensorStrobe Channel 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS2HIGHDUR_t__
typedef struct _ADI_RTC_SS2HIGHDUR_t {
  union {
    struct {
      unsigned int SS2HIGHDUR : 16;  /**< High Duration for SensorStrobe Channel 2. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS2HIGHDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS2HIGHDUR_t__ */

/*@}*/

/** @defgroup SS3HIGHDUR RTC Auto-Reload High Duration for SensorStrobe Channel 3 (SS3HIGHDUR) Register
 *  RTC Auto-Reload High Duration for SensorStrobe Channel 3 (SS3HIGHDUR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SS3HIGHDUR_Struct
 *! \brief  RTC Auto-Reload High Duration for SensorStrobe Channel 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SS3HIGHDUR_t__
typedef struct _ADI_RTC_SS3HIGHDUR_t {
  union {
    struct {
      unsigned int SS3HIGHDUR : 16;  /**< High Duration for SensorStrobe Channel 3. */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SS3HIGHDUR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SS3HIGHDUR_t__ */

/*@}*/

/** @defgroup SSMSKOT RTC Masks for SensorStrobe Channels on Time Control (SSMSKOT) Register
 *  RTC Masks for SensorStrobe Channels on Time Control (SSMSKOT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_SSMSKOT_Struct
 *! \brief  RTC Masks for SensorStrobe Channels on Time Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_SSMSKOT_t__
typedef struct _ADI_RTC_SSMSKOT_t {
  union {
    struct {
      unsigned int SS1MSKOT   :  4;  /**< Concatenation of Thermometer-encoded Masks for the 16-bit SensorStrobe Channels */
      unsigned int SS2MSKOT   :  4;  /**< SensorStrobe Channel 2 on Time Control */
      unsigned int SS3MSKOT   :  4;  /**< SensorStrobe Channel 3 on Time Control */
      unsigned int SS4MSKOT   :  4;  /**< SensorStrobe Channel 4 on Time Control */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SSMSKOT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SSMSKOT_t__ */

/*@}*/

/** @defgroup CR5SSS RTC Control 5 for Configuring SensorStrobe Channel GPIO Sampling (CR5SSS) Register
 *  RTC Control 5 for Configuring SensorStrobe Channel GPIO Sampling (CR5SSS) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_CR5SSS_Struct
 *! \brief  RTC Control 5 for Configuring SensorStrobe Channel GPIO Sampling Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR5SSS_t__
typedef struct _ADI_RTC_CR5SSS_t {
  union {
    struct {
      unsigned int SS1SMPEN   :  3;  /**< GPIO Input Sample Enable for SensorStrobe Channel 1 */
      unsigned int SS1SMPMTCHIRQEN :  1;  /**< Sample Activity Interrupt Enable for SensorStrobe Channel 1 */
      unsigned int SS2SMPEN        :  3;  /**< GPIO Input Sample Enable for SensorStrobe Channel 2 */
      unsigned int SS2SMPMTCHIRQEN :  1;  /**< Sample Activity Interrupt Enable for SensorStrobe Channel 2 */
      unsigned int SS3SMPEN        :  3;  /**< GPIO Input Sample Enable for SensorStrobe Channel 3 */
      unsigned int SS3SMPMTCHIRQEN :  1;  /**< Sample Activity Interrupt Enable for SensorStrobe Channel 3 */
      unsigned int reserved12      :  4;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR5SSS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR5SSS_t__ */

/*@}*/

/** @defgroup CR6SSS RTC Control 6 for Configuring SensorStrobe Channel GPIO Sampling Edge (CR6SSS) Register
 *  RTC Control 6 for Configuring SensorStrobe Channel GPIO Sampling Edge (CR6SSS) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_RTC_CR6SSS_SS1SMPONFE
 *! \brief  GPIO Sample Around Falling Edge of SensorStrobe Channel 1 (SS1SMPONFE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6SSS_SS1NOFES = 0,  /**< No sampling of input around falling edge                                        */
  RTC_CR6SSS_SS1BFES  = 1,  /**< Input sampled one clock cycle before falling edge of the SensorStrobe channel 1 */
  RTC_CR6SSS_SS1FES   = 2,  /**< Input sampled at falling edge of the SensorStrobe channel 1                     */
  RTC_CR6SSS_SS1AFES  = 3   /**< Input sampled one clock cycle after falling edge of the SensorStrobe channel 1  */
} ADI_RTC_CR6SSS_SS1SMPONFE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6SSS_SS1SMPONRE
 *! \brief  GPIO Sample Around Rising Edge of SensorStrobe Channel 1 (SS1SMPONRE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6SSS_SS1NORES = 0,  /**< No sampling of input around rising edge                                        */
  RTC_CR6SSS_SS1BRES  = 1,  /**< Input sampled one clock cycle before rising edge of the SensorStrobe channel 1 */
  RTC_CR6SSS_SS1RES   = 2,  /**< Input sampled at rising edge of the SensorStrobe channel 1                     */
  RTC_CR6SSS_SS1ARES  = 3   /**< Input sampled one clock cycle after rising edge of the SensorStrobe channel 1  */
} ADI_RTC_CR6SSS_SS1SMPONRE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6SSS_SS2SMPONFE
 *! \brief  GPIO Sample Around Falling Edge of SensorStrobe Channel 2 (SS2SMPONFE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6SSS_SS2NOFES = 0,  /**< No sampling of input around falling edge                                        */
  RTC_CR6SSS_SS2BFES  = 1,  /**< Input sampled one clock cycle before falling edge of the SensorStrobe channel 2 */
  RTC_CR6SSS_SS2FES   = 2,  /**< Input sampled at falling edge of the SensorStrobe channel 2                     */
  RTC_CR6SSS_SS2AFES  = 3   /**< Input sampled one clock cycle after falling edge of the SensorStrobe channel 2  */
} ADI_RTC_CR6SSS_SS2SMPONFE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6SSS_SS2SMPONRE
 *! \brief  GPIO Sample Around Rising Edge of SensorStrobe Channel 2 (SS2SMPONRE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6SSS_SS2NORES = 0,  /**< No sampling of input around rising edge                                        */
  RTC_CR6SSS_SS2BRES  = 1,  /**< Input sampled one clock cycle before rising edge of the SensorStrobe channel 2 */
  RTC_CR6SSS_SS2RES   = 2,  /**< Input sampled at rising edge of the SensorStrobe channel 2                     */
  RTC_CR6SSS_SS2ARES  = 3   /**< Input sampled one clock cycle after rising edge of the SensorStrobe channel 2  */
} ADI_RTC_CR6SSS_SS2SMPONRE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6SSS_SS3SMPONFE
 *! \brief  GPIO Sample Around Falling Edge of SensorStrobe Channel 3 (SS3SMPONFE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6SSS_SS3NOFES = 0,  /**< No sampling of input around falling edge                                        */
  RTC_CR6SSS_SS3BFES  = 1,  /**< Input sampled one clock cycle before falling edge of the SensorStrobe channel 3 */
  RTC_CR6SSS_SS3FES   = 2,  /**< Input sampled at falling edge of the SensorStrobe channel 3                     */
  RTC_CR6SSS_SS3AFES  = 3   /**< Input sampled one clock cycle after falling edge of the SensorStrobe channel 3  */
} ADI_RTC_CR6SSS_SS3SMPONFE;


/*  =========================================================================
 *! \enum   ADI_RTC_CR6SSS_SS3SMPONRE
 *! \brief  GPIO Sample Around Rising Edge of SensorStrobe Channel 3 (SS3SMPONRE) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR6SSS_SS3NORES = 0,  /**< No sampling of input around rising edge                                        */
  RTC_CR6SSS_SS3BRES  = 1,  /**< Input sampled one clock cycle before rising edge of the SensorStrobe channel 3 */
  RTC_CR6SSS_SS3RES   = 2,  /**< Input sampled at rising edge of the SensorStrobe channel 3                     */
  RTC_CR6SSS_SS3ARES  = 3   /**< Input sampled one clock cycle after rising edge of the SensorStrobe channel 3  */
} ADI_RTC_CR6SSS_SS3SMPONRE;


/* ==========================================================================
 *! \struct ADI_RTC_CR6SSS_Struct
 *! \brief  RTC Control 6 for Configuring SensorStrobe Channel GPIO Sampling Edge Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR6SSS_t__
typedef struct _ADI_RTC_CR6SSS_t {
  union {
    struct {
      unsigned int SS1SMPONFE :  2;  /**< GPIO Sample Around Falling Edge of SensorStrobe Channel 1 */
      unsigned int SS1SMPONRE :  2;  /**< GPIO Sample Around Rising Edge of SensorStrobe Channel 1 */
      unsigned int SS2SMPONFE :  2;  /**< GPIO Sample Around Falling Edge of SensorStrobe Channel 2 */
      unsigned int SS2SMPONRE :  2;  /**< GPIO Sample Around Rising Edge of SensorStrobe Channel 2 */
      unsigned int SS3SMPONFE :  2;  /**< GPIO Sample Around Falling Edge of SensorStrobe Channel 3 */
      unsigned int SS3SMPONRE :  2;  /**< GPIO Sample Around Rising Edge of SensorStrobe Channel 3 */
      unsigned int reserved12 :  4;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR6SSS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR6SSS_t__ */

/*@}*/

/** @defgroup CR7SSS RTC Control 7 for Configuring SensorStrobe Channel GPIO Sampling Activity (CR7SSS) Register
 *  RTC Control 7 for Configuring SensorStrobe Channel GPIO Sampling Activity (CR7SSS) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_RTC_CR7SSS_SS1SMPPTRN
 *! \brief  Sample Activity Selection for SensorStrobe Channel 1 (SS1SMPPTRN) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR7SSS_SS1SMPCHNG   = 0,  /**< Current GPIO sample is not same as previous sample */
  RTC_CR7SSS_SS1SMPSAME   = 1,  /**< Current GPIO sample is same as previous sample     */
  RTC_CR7SSS_SS1SMPMTCH   = 2,  /**< Current GPIO sample is same as expected sample     */
  RTC_CR7SSS_SS1SMPNOMTCH = 3   /**< Current GPIO sample is not same as expected sample */
} ADI_RTC_CR7SSS_SS1SMPPTRN;


/*  =========================================================================
 *! \enum   ADI_RTC_CR7SSS_SS2SMPPTRN
 *! \brief  Sample Activity Selection for SensorStrobe Channel 2 (SS2SMPPTRN) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR7SSS_SS2SMPCHNG   = 0,  /**< Current GPIO sample is not same as previous sample */
  RTC_CR7SSS_SS2SMPSAME   = 1,  /**< Current GPIO sample is same as previous sample     */
  RTC_CR7SSS_SS2SMPMTCH   = 2,  /**< Current GPIO sample is same as expected sample     */
  RTC_CR7SSS_SS2SMPNOMTCH = 3   /**< Current GPIO sample is not same as expected sample */
} ADI_RTC_CR7SSS_SS2SMPPTRN;


/*  =========================================================================
 *! \enum   ADI_RTC_CR7SSS_SS3SMPPTRN
 *! \brief  Sample Activity Selection for SensorStrobe Channel 3 (SS3SMPPTRN) Enumerations
 *  ========================================================================= */
typedef enum
{
  RTC_CR7SSS_SS3SMPCHNG   = 0,  /**< Current GPIO sample is not same as previous sample */
  RTC_CR7SSS_SS3SMPSAME   = 1,  /**< Current GPIO sample is same as previous sample     */
  RTC_CR7SSS_SS3SMPMTCH   = 2,  /**< Current GPIO sample is same as expected sample     */
  RTC_CR7SSS_SS3SMPNOMTCH = 3   /**< Current GPIO sample is not same as expected sample */
} ADI_RTC_CR7SSS_SS3SMPPTRN;


/* ==========================================================================
 *! \struct ADI_RTC_CR7SSS_Struct
 *! \brief  RTC Control 7 for Configuring SensorStrobe Channel GPIO Sampling Activity Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_CR7SSS_t__
typedef struct _ADI_RTC_CR7SSS_t {
  union {
    struct {
      unsigned int SS1SMPEXP  :  3;  /**< Expected GPIO Sample for SensorStrobe Channel 1 */
      unsigned int SS1SMPPTRN :  2;  /**< Sample Activity Selection for SensorStrobe Channel 1 */
      unsigned int SS2SMPEXP  :  3;  /**< Expected GPIO Sample for SensorStrobe Channel 2 */
      unsigned int SS2SMPPTRN :  2;  /**< Sample Activity Selection for SensorStrobe Channel 2 */
      unsigned int SS3SMPEXP  :  3;  /**< Expected GPIO Sample for SensorStrobe Channel 3 */
      unsigned int SS3SMPPTRN :  2;  /**< Sample Activity Selection for SensorStrobe Channel 3 */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_CR7SSS_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_CR7SSS_t__ */

/*@}*/

/** @defgroup SR7 RTC Status 7 (SR7) Register
 *  RTC Status 7 (SR7) Register.
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
      unsigned int SS1SMP     :  3;  /**< Latest GPIO Sample for SensorStrobe Channel 1 */
      unsigned int SS1SMPMTCHIRQ :  1;  /**< Sticky Status of GPIO Sample Pattern Match for SensorStrobe Channel 1 */
      unsigned int SS2SMP        :  3;  /**< Latest GPIO Sample for SensorStrobe Channel 2 */
      unsigned int SS2SMPMTCHIRQ :  1;  /**< Sticky Status of GPIO Sample Pattern Match for SensorStrobe Channel 2 */
      unsigned int SS3SMP        :  3;  /**< Latest GPIO Sample for SensorStrobe Channel 3 */
      unsigned int SS3SMPMTCHIRQ :  1;  /**< Sticky Status of GPIO Sample Pattern Match for SensorStrobe Channel 3 */
      unsigned int SS1OUT        :  1;  /**< Output Value for SensorStrobe Channel 1 */
      unsigned int SS2OUT        :  1;  /**< Output Value for SensorStrobe Channel 2 */
      unsigned int SS3OUT        :  1;  /**< Output Value for SensorStrobe Channel 3 */
      unsigned int SS4OUT        :  1;  /**< Output Value for SensorStrobe Channel 4 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR7_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR7_t__ */

/*@}*/

/** @defgroup SR8 RTC Status 8 (SR8) Register
 *  RTC Status 8 (SR8) Register.
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
      unsigned int WSYNCSS1LOWDUR :  1;  /**< Synchronisation Status of Posted Writes to SensorStrobe Channel 1 Low Duration Register */
      unsigned int WSYNCSS2LOWDUR :  1;  /**< Synchronisation Status of Posted Writes to SensorStrobe Channel 2 Low Duration Register */
      unsigned int WSYNCSS3LOWDUR :  1;  /**< Synchronisation Status of Posted Writes to SensorStrobe Channel 3 Low Duration Register */
      unsigned int reserved3       :  1;
      unsigned int WSYNCSS1HIGHDUR :  1;  /**< Synchronisation Status of Posted Writes to SensorStrobe Channel 1 High Duration Register */
      unsigned int WSYNCSS2HIGHDUR :  1;  /**< Synchronisation Status of Posted Writes to SensorStrobe Channel 2 High Duration Register */
      unsigned int WSYNCSS3HIGHDUR :  1;  /**< Synchronisation Status of Posted Writes to SensorStrobe Channel 3 High Duration Register */
      unsigned int reserved7       :  1;
      unsigned int WSYNCCR5SSS     :  1;  /**< Synchronisation Status of Posted Writes to Control 5 for Configuring SensorStrobe Channel Register */
      unsigned int WSYNCCR6SSS     :  1;  /**< Synchronisation Status of Posted Writes to Control 6 for Configuring SensorStrobe Channel Register */
      unsigned int WSYNCCR7SSS     :  1;  /**< Synchronisation Status of Posted Writes to Control 7 for Configuring SensorStrobe Channel Register */
      unsigned int WSYNCSR7        :  1;  /**< Synchronisation Status of Posted Writes to Status 7 Register */
      unsigned int WSYNCGPMUX0     :  1;  /**< Synchronisation Status of Posted Writes to GPIO Pin Mux Control Register 0 */
      unsigned int WSYNCGPMUX1     :  1;  /**< Synchronisation Status of Posted Writes to GPIO Pin Mux Control Register 1 */
      unsigned int reserved14      :  2;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR8_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR8_t__ */

/*@}*/

/** @defgroup SR9 RTC Status 9 (SR9) Register
 *  RTC Status 9 (SR9) Register.
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
      unsigned int WPENDSS1LOWDUR :  1;  /**< Pending Status of Posted Writes to SensortStrobe Channel 1 Low Duration Register */
      unsigned int WPENDSS2LOWDUR :  1;  /**< Pending Status of Posted Writes to SensortStrobe Channel 2 Low Duration Register */
      unsigned int WPENDSS3LOWDUR :  1;  /**< Pending Status of Posted Writes to SensortStrobe Channel 3 Low Duration Register */
      unsigned int reserved3       :  1;
      unsigned int WPENDSS1HIGHDUR :  1;  /**< Pending Status of Posted Writes to SensortStrobe Channel 1 High Duration Register */
      unsigned int WPENDSS2HIGHDUR :  1;  /**< Pending Status of Posted Writes to SensortStrobe Channel 2 High Duration Register */
      unsigned int WPENDSS3HIGHDUR :  1;  /**< Pending Status of Posted Writes to SensortStrobe Channel 3 High Duration Register */
      unsigned int reserved7       :  1;
      unsigned int WPENDCR5SSS     :  1;  /**< Pending Status of Posted Writes to Control 5 for Configuring SensorStrobe Channel Register */
      unsigned int WPENDCR6SSS     :  1;  /**< Pending Status of Posted Writes to Control 6 for Configuring SensorStrobe Channel Register */
      unsigned int WPENDCR7SSS     :  1;  /**< Pending Status of Posted Writes to Control 7 for Configuring SensorStrobe Channel Register */
      unsigned int WPENDSR7        :  1;  /**< Pending Status of Posted Writes to SR7 */
      unsigned int WPENDGPMUX0     :  1;  /**< Pending Status of Posted Writes to GPMUX0 */
      unsigned int WPENDGPMUX1     :  1;  /**< Pending Status of Posted Writes to  GPMUX1 */
      unsigned int reserved14      :  2;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_SR9_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_SR9_t__ */

/*@}*/

/** @defgroup GPMUX0 RTC GPIO Pin Mux Control Register 0 (GPMUX0) Register
 *  RTC GPIO Pin Mux Control Register 0 (GPMUX0) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_GPMUX0_Struct
 *! \brief  RTC GPIO Pin Mux Control Register 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_GPMUX0_t__
typedef struct _ADI_RTC_GPMUX0_t {
  union {
    struct {
      unsigned int SS1GPIN0SEL :  3;  /**< GPIO Mux Selection for SensorStrobe Channel 1 Input0 */
      unsigned int SS1GPIN1SEL :  3;  /**< GPIO Mux Selection for SensorStrobe Channel 1 Input 1 */
      unsigned int SS1GPIN2SEL :  3;  /**< GPIO Mux Selection for SensorStrobe Channel 1 Input 2 */
      unsigned int SS2GPIN0SEL :  3;  /**< GPIO Mux Selection for SensorStrobe Channel 2 Input 0 */
      unsigned int SS2GPIN1SEL :  3;  /**< GPIO Mux Selection for SensorStrobe Channel 2 Input 1 */
      unsigned int reserved15  :  1;
    };
    uint16_t VALUE16;
  };
} ADI_RTC_GPMUX0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_GPMUX0_t__ */

/*@}*/

/** @defgroup GPMUX1 RTC GPIO Pin Mux Control Register 1 (GPMUX1) Register
 *  RTC GPIO Pin Mux Control Register 1 (GPMUX1) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_RTC_GPMUX1_Struct
 *! \brief  RTC GPIO Pin Mux Control Register 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_GPMUX1_t__
typedef struct _ADI_RTC_GPMUX1_t {
  union {
    struct {
      unsigned int SS2GPIN2SEL :  3;  /**< GPIO Mux Selection for SensorStrobe Channel 2 Input 2 */
      unsigned int SS3GPIN0SEL :  3;  /**< GPIO Mux Selection for SensorStrobe Channel 3 Input 0 */
      unsigned int SS3GPIN1SEL :  3;  /**< GPIO Mux Selection for SensorStrobe Channel 3 Input 1 */
      unsigned int SS3GPIN2SEL :  3;  /**< GPIO Mux Selection for SensorStrobe Channel 3 Input 2 */
      unsigned int reserved12  :  2;
      unsigned int SS1DIFFOUT  :  1;  /**< Differential SensorStrobe Out Option for SensorStrobe Channel 1 */
      unsigned int SS3DIFFOUT  :  1;  /**< Differential SensorStrobe Out Option for SensorStrobe Channel 3 */
    };
    uint16_t VALUE16;
  };
} ADI_RTC_GPMUX1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_GPMUX1_t__ */

/*@}*/

/** @defgroup ADIID ADI Identification (ADIID) Register
 *  ADI Identification (ADIID) Register.
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
      unsigned int VALUE      : 16;  /**< Reads a fixed value of 0x4144 to indicate to debuggers that they are connected to an Analog Devices implemented Cortex based part */
    };
    uint16_t VALUE16;
  };
} ADI_SYS_ADIID_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SYS_ADIID_t__ */

/*@}*/

/** @defgroup CHIPID Chip Identifier (CHIPID) Register
 *  Chip Identifier (CHIPID) Register.
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
      unsigned int VALUE      : 16;  /**< To enable SWD interface */
    };
    uint16_t VALUE16;
  };
} ADI_SYS_SWDEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SYS_SWDEN_t__ */

/*@}*/

/** @defgroup LOAD Load Value (LOAD) Register
 *  Load Value (LOAD) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_WDT_LOAD_Struct
 *! \brief  Load Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_LOAD_t__
typedef struct _ADI_WDT_LOAD_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Load Value */
    };
    uint16_t VALUE16;
  };
} ADI_WDT_LOAD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_LOAD_t__ */

/*@}*/

/** @defgroup CCNT Current Count Value (CCNT) Register
 *  Current Count Value (CCNT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_WDT_CCNT_Struct
 *! \brief  Current Count Value Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_CCNT_t__
typedef struct _ADI_WDT_CCNT_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Current Count Value */
    };
    uint16_t VALUE16;
  };
} ADI_WDT_CCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_CCNT_t__ */

/*@}*/

/** @defgroup CTL Control (CTL) Register
 *  Control (CTL) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_WDT_CTL_IRQ
 *! \brief  Timer Interrupt (IRQ) Enumerations
 *  ========================================================================= */
typedef enum
{
  WDT_CTL_RST = 0,  /**< WDT asserts reset when timed out       */
  WDT_CTL_INT = 1   /**< WDT generates interrupt when timed out */
} ADI_WDT_CTL_IRQ;


/*  =========================================================================
 *! \enum   ADI_WDT_CTL_PRE
 *! \brief  Prescaler (PRE) Enumerations
 *  ========================================================================= */
typedef enum
{
  WDT_CTL_DIV1   = 0,  /**< Source clock/1             */
  WDT_CTL_DIV16  = 1,  /**< Source clock/16            */
  WDT_CTL_DIV256 = 2   /**< Source clock/256 (default) */
} ADI_WDT_CTL_PRE;


/*  =========================================================================
 *! \enum   ADI_WDT_CTL_EN
 *! \brief  Timer Enable (EN) Enumerations
 *  ========================================================================= */
typedef enum
{
  WDT_CTL_WDT_DIS = 0,  /**< WDT not enabled */
  WDT_CTL_WDT_EN  = 1   /**< WDT enabled     */
} ADI_WDT_CTL_EN;


/*  =========================================================================
 *! \enum   ADI_WDT_CTL_MODE
 *! \brief  Timer Mode (MODE) Enumerations
 *  ========================================================================= */
typedef enum
{
  WDT_CTL_FREE_RUN = 0,  /**< Free running mode */
  WDT_CTL_PERIODIC = 1   /**< Periodic mode     */
} ADI_WDT_CTL_MODE;


/* ==========================================================================
 *! \struct ADI_WDT_CTL_Struct
 *! \brief  Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_CTL_t__
typedef struct _ADI_WDT_CTL_t {
  union {
    struct {
      unsigned int reserved0  :  1;
      unsigned int IRQ        :  1;  /**< Timer Interrupt */
      unsigned int PRE        :  2;  /**< Prescaler */
      unsigned int reserved4  :  1;
      unsigned int EN         :  1;  /**< Timer Enable */
      unsigned int MODE       :  1;  /**< Timer Mode */
      unsigned int SPARE      :  1;  /**< Unused Spare Bit */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_WDT_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_CTL_t__ */

/*@}*/

/** @defgroup RESTART Clear Interrupt (RESTART) Register
 *  Clear Interrupt (RESTART) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_WDT_RESTART_Struct
 *! \brief  Clear Interrupt Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_RESTART_t__
typedef struct _ADI_WDT_RESTART_t {
  union {
    struct {
      unsigned int CLRWORD    : 16;  /**< Clear Watchdog */
    };
    uint16_t VALUE16;
  };
} ADI_WDT_RESTART_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_RESTART_t__ */

/*@}*/

/** @defgroup STAT Status (STAT) Register
 *  Status (STAT) Register.
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
      unsigned int CLRIRQ     :  1;  /**< Clear Interrupt Register Write Sync in Progress */
      unsigned int LOADING    :  1;  /**< Load Register Write Sync in Progress */
      unsigned int COUNTING   :  1;  /**< Control Register Write Sync in Progress */
      unsigned int LOCKED     :  1;  /**< Lock Status Bit */
      unsigned int RSTCTL     :  1;  /**< Reset Control Register Written and Locked */
      unsigned int reserved6  : 10;
    };
    uint16_t VALUE16;
  };
} ADI_WDT_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_STAT_t__ */

/*@}*/

/** @defgroup MCTL Master Control (MCTL) Register
 *  Master Control (MCTL) Register.
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
      unsigned int MASEN      :  1;  /**< Master Enable */
      unsigned int COMPLETE   :  1;  /**< Start Back-off Disable */
      unsigned int LOOPBACK   :  1;  /**< Internal Loopback Enable */
      unsigned int STRETCHSCL :  1;  /**< Stretch SCL Enable */
      unsigned int IENMRX     :  1;  /**< Receive Request Interrupt Enable */
      unsigned int IENMTX     :  1;  /**< Transmit Request Interrupt Enable */
      unsigned int IENALOST   :  1;  /**< Arbitration Lost Interrupt Enable */
      unsigned int IENACK     :  1;  /**< ACK Not Received Interrupt Enable */
      unsigned int IENCMP     :  1;  /**< Transaction Completed (or Stop Detected) Interrupt Enable */
      unsigned int MXMITDEC   :  1;  /**< Decrement Master Tx FIFO Status When a Byte Txed */
      unsigned int MRXDMA     :  1;  /**< Enable Master Rx DMA Request */
      unsigned int MTXDMA     :  1;  /**< Enable Master Tx DMA Request */
      unsigned int BUSCLR     :  1;  /**< Bus-Clear Enable */
      unsigned int STOPBUSCLR :  1;  /**< Prestop Bus Clear */
      unsigned int reserved14 :  2;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MCTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MCTL_t__ */

/*@}*/

/** @defgroup MSTAT Master Status (MSTAT) Register
 *  Master Status (MSTAT) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_I2C_MSTAT_MTXF
 *! \brief  Master Transmit FIFO Status (MTXF) Enumerations
 *  ========================================================================= */
typedef enum
{
  I2C_MSTAT_FIFO_EMPTY = 0,  /**< FIFO Empty.     */
  I2C_MSTAT_FIFO_1BYTE = 2,  /**< 1 byte in FIFO. */
  I2C_MSTAT_FIFO_FULL  = 3   /**< FIFO Full.      */
} ADI_I2C_MSTAT_MTXF;


/* ==========================================================================
 *! \struct ADI_I2C_MSTAT_Struct
 *! \brief  Master Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_MSTAT_t__
typedef struct _ADI_I2C_MSTAT_t {
  union {
    struct {
      unsigned int MTXF       :  2;  /**< Master Transmit FIFO Status */
      unsigned int MTXREQ     :  1;  /**< Master Transmit Request/Clear Master Transmit Interrupt */
      unsigned int MRXREQ     :  1;  /**< Master Receive Request */
      unsigned int NACKADDR   :  1;  /**< ACK Not Received in Response to an Address */
      unsigned int ALOST      :  1;  /**< Arbitration Lost */
      unsigned int MBUSY      :  1;  /**< Master Busy */
      unsigned int NACKDATA   :  1;  /**< ACK Not Received in Response to Data Write */
      unsigned int TCOMP      :  1;  /**< Transaction Complete or Stop Detected */
      unsigned int MRXOVR     :  1;  /**< Master Receive FIFO Overflow */
      unsigned int LINEBUSY   :  1;  /**< Line is Busy */
      unsigned int MSTOP      :  1;  /**< STOP Driven by This I2C Master */
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
      unsigned int VALUE      :  8;  /**< Master Receive Register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MRX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MRX_t__ */

/*@}*/

/** @defgroup MTX Master Transmit Data (MTX) Register
 *  Master Transmit Data (MTX) Register.
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
      unsigned int VALUE      :  8;  /**< Master Transmit Register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MTX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MTX_t__ */

/*@}*/

/** @defgroup MRXCNT Master Receive Data Count (MRXCNT) Register
 *  Master Receive Data Count (MRXCNT) Register.
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
      unsigned int VALUE      :  8;  /**< Receive Count */
      unsigned int EXTEND     :  1;  /**< Extended Read */
      unsigned int reserved9  :  7;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MRXCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MRXCNT_t__ */

/*@}*/

/** @defgroup MCRXCNT Master Current Receive Data Count (MCRXCNT) Register
 *  Master Current Receive Data Count (MCRXCNT) Register.
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
      unsigned int VALUE      :  8;  /**< Current Receive Count */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_MCRXCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_MCRXCNT_t__ */

/*@}*/

/** @defgroup ADDR1 Master Address Byte 1 (ADDR1) Register
 *  Master Address Byte 1 (ADDR1) Register.
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
      unsigned int VALUE      :  8;  /**< Address Byte 1 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ADDR1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ADDR1_t__ */

/*@}*/

/** @defgroup ADDR2 Master Address Byte 2 (ADDR2) Register
 *  Master Address Byte 2 (ADDR2) Register.
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
      unsigned int VALUE      :  8;  /**< Address Byte 2 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ADDR2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ADDR2_t__ */

/*@}*/

/** @defgroup BYT Start Byte (BYT) Register
 *  Start Byte (BYT) Register.
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
      unsigned int SBYTE      :  8;  /**< Start Byte */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_BYT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_BYT_t__ */

/*@}*/

/** @defgroup DIV Serial Clock Period Divisor (DIV) Register
 *  Serial Clock Period Divisor (DIV) Register.
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
      unsigned int LOW        :  8;  /**< Serial Clock Low Time */
      unsigned int HIGH       :  8;  /**< Serial Clock High Time */
    };
    uint16_t VALUE16;
  };
} ADI_I2C_DIV_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_DIV_t__ */

/*@}*/

/** @defgroup SCTL Slave Control (SCTL) Register
 *  Slave Control (SCTL) Register.
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
      unsigned int SLVEN      :  1;  /**< Slave Enable */
      unsigned int ADR10EN    :  1;  /**< Enabled 10-bit Addressing */
      unsigned int GCEN       :  1;  /**< General Call Enable */
      unsigned int HGCEN      :  1;  /**< Hardware General Call Enable */
      unsigned int GCSBCLR    :  1;  /**< General Call Status Bit Clear */
      unsigned int EARLYTXR   :  1;  /**< Early Transmit Request Mode */
      unsigned int reserved6  :  1;
      unsigned int NACK       :  1;  /**< NACK Next Communication */
      unsigned int IENSTOP    :  1;  /**< Stop Condition Detected Interrupt Enable */
      unsigned int IENSRX     :  1;  /**< Slave Receive Request Interrupt Enable */
      unsigned int IENSTX     :  1;  /**< Slave Transmit Request Interrupt Enable */
      unsigned int STXDEC     :  1;  /**< Decrement Slave Tx FIFO Status When a Byte is Txed */
      unsigned int IENREPST   :  1;  /**< Repeated Start Interrupt Enable */
      unsigned int SRXDMA     :  1;  /**< Enable Slave Rx DMA Request */
      unsigned int STXDMA     :  1;  /**< Enable Slave Tx DMA Request */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_SCTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_SCTL_t__ */

/*@}*/

/** @defgroup SSTAT Slave I2C Status/Error/IRQ (SSTAT) Register
 *  Slave I2C Status/Error/IRQ (SSTAT) Register.
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
      unsigned int STXFSEREQ  :  1;  /**< Slave Tx FIFO Status or Early Request */
      unsigned int STXUNDR    :  1;  /**< Slave Transmit FIFO Underflow */
      unsigned int STXREQ     :  1;  /**< Slave Transmit Request/Slave Transmit Interrupt */
      unsigned int SRXREQ     :  1;  /**< Slave Receive Request */
      unsigned int SRXOVR     :  1;  /**< Slave Receive FIFO Overflow */
      unsigned int NOACK      :  1;  /**< ACK Not Generated by the Slave */
      unsigned int SBUSY      :  1;  /**< Slave Busy */
      unsigned int GCINT      :  1;  /**< General Call Interrupt */
      unsigned int GCID       :  2;  /**< General ID */
      unsigned int STOP       :  1;  /**< Stop After Start and Matching Address */
      unsigned int IDMAT      :  2;  /**< Device ID Matched */
      unsigned int REPSTART   :  1;  /**< Repeated Start and Matching Address */
      unsigned int START      :  1;  /**< Start and Matching Address */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_SSTAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_SSTAT_t__ */

/*@}*/

/** @defgroup SRX Slave Receive (SRX) Register
 *  Slave Receive (SRX) Register.
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
      unsigned int VALUE      :  8;  /**< Slave Receive Register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_SRX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_SRX_t__ */

/*@}*/

/** @defgroup STX Slave Transmit (STX) Register
 *  Slave Transmit (STX) Register.
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
      unsigned int VALUE      :  8;  /**< Slave Transmit Register */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_STX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_STX_t__ */

/*@}*/

/** @defgroup ALT Hardware General Call ID (ALT) Register
 *  Hardware General Call ID (ALT) Register.
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
      unsigned int VALUE      :  8;  /**< Slave Device ID 0 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ID0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ID0_t__ */

/*@}*/

/** @defgroup ID1 Second Slave Address Device ID (ID1) Register
 *  Second Slave Address Device ID (ID1) Register.
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
      unsigned int VALUE      :  8;  /**< Slave Device ID 1 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ID1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ID1_t__ */

/*@}*/

/** @defgroup ID2 Third Slave Address Device ID (ID2) Register
 *  Third Slave Address Device ID (ID2) Register.
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
      unsigned int VALUE      :  8;  /**< Slave Device ID 2 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ID2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ID2_t__ */

/*@}*/

/** @defgroup ID3 Fourth Slave Address Device ID (ID3) Register
 *  Fourth Slave Address Device ID (ID3) Register.
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
      unsigned int VALUE      :  8;  /**< Slave Device ID 3 */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ID3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ID3_t__ */

/*@}*/

/** @defgroup STAT Master and Slave FIFO Status (STAT) Register
 *  Master and Slave FIFO Status (STAT) Register.
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
      unsigned int STXF       :  2;  /**< Slave Transmit FIFO Status */
      unsigned int SRXF       :  2;  /**< Slave Receive FIFO Status */
      unsigned int MTXF       :  2;  /**< Master Transmit FIFO Status */
      unsigned int MRXF       :  2;  /**< Master Receive FIFO Status */
      unsigned int SFLUSH     :  1;  /**< Flush the Slave Transmit FIFO */
      unsigned int MFLUSH     :  1;  /**< Flush the Master Transmit FIFO */
      unsigned int reserved10 :  6;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_STAT_t__ */

/*@}*/

/** @defgroup SHCTL Shared Control (SHCTL) Register
 *  Shared Control (SHCTL) Register.
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
      unsigned int RST        :  1;  /**< Reset START STOP Detect Circuit */
      unsigned int reserved1  : 15;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_SHCTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_SHCTL_t__ */

/*@}*/

/** @defgroup TCTL Timing Control Register (TCTL) Register
 *  Timing Control Register (TCTL) Register.
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
      unsigned int THDATIN    :  5;  /**< Data in Hold Start */
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
      unsigned int MST        :  4;  /**< Master Automatic Stretch Mode */
      unsigned int SLV        :  4;  /**< Slave Automatic Stretch Mode */
      unsigned int MSTTMO     :  1;  /**< Master Automatic Stretch Timeout */
      unsigned int SLVTMO     :  1;  /**< Slave Automatic Stretch Timeout */
      unsigned int reserved10 :  6;
    };
    uint16_t VALUE16;
  };
} ADI_I2C_ASTRETCH_SCL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_ASTRETCH_SCL_t__ */

/*@}*/

/** @defgroup STAT Status (STAT) Register
 *  Status (STAT) Register.
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
      unsigned int IRQ        :  1;  /**< SPI Interrupt Status */
      unsigned int XFRDONE    :  1;  /**< SPI Transfer Completion */
      unsigned int TXEMPTY    :  1;  /**< SPI Tx FIFO Empty Interrupt */
      unsigned int TXDONE     :  1;  /**< SPI Tx Done in Read Command Mode */
      unsigned int TXUNDR     :  1;  /**< SPI Tx FIFO Underflow */
      unsigned int TXIRQ      :  1;  /**< SPI Tx IRQ */
      unsigned int RXIRQ      :  1;  /**< SPI Rx IRQ */
      unsigned int RXOVR      :  1;  /**< SPI Rx FIFO Overflow */
      unsigned int reserved8  :  3;
      unsigned int CS         :  1;  /**< CS Status */
      unsigned int CSERR      :  1;  /**< Detected a CS Error Condition in Slave Mode */
      unsigned int CSRISE     :  1;  /**< Detected a Rising Edge on CS, in Slave CON Mode */
      unsigned int CSFALL     :  1;  /**< Detected a Falling Edge on CS, in Slave CON Mode */
      unsigned int RDY        :  1;  /**< Detected an Edge on Ready Indicator for Flow Control */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_STAT_t__ */

/*@}*/

/** @defgroup RX Receive (RX) Register
 *  Receive (RX) Register.
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
      unsigned int BYTE1      :  8;  /**< 8-bit Receive Buffer */
      unsigned int BYTE2      :  8;  /**< 8-bit Receive Buffer, Used Only in DMA Modes */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_RX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_RX_t__ */

/*@}*/

/** @defgroup TX Transmit (TX) Register
 *  Transmit (TX) Register.
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
      unsigned int BYTE1      :  8;  /**< 8-bit Transmit Buffer */
      unsigned int BYTE2      :  8;  /**< 8-bit Transmit Buffer, Used Only in DMA Modes */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_TX_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_TX_t__ */

/*@}*/

/** @defgroup DIV SPI Baud Rate Selection (DIV) Register
 *  SPI Baud Rate Selection (DIV) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_DIV_Struct
 *! \brief  SPI Baud Rate Selection Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_DIV_t__
typedef struct _ADI_SPI_DIV_t {
  union {
    struct {
      unsigned int VALUE      :  6;  /**< SPI Clock Divider */
      unsigned int reserved6  : 10;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_DIV_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_DIV_t__ */

/*@}*/

/** @defgroup CTL SPI Configuration (CTL) Register
 *  SPI Configuration (CTL) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_CTL_Struct
 *! \brief  SPI Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_CTL_t__
typedef struct _ADI_SPI_CTL_t {
  union {
    struct {
      unsigned int SPIEN      :  1;  /**< SPI Enable */
      unsigned int MASEN      :  1;  /**< Master Mode Enable */
      unsigned int CPHA       :  1;  /**< Serial Clock Phase Mode */
      unsigned int CPOL       :  1;  /**< Serial Clock Polarity */
      unsigned int WOM        :  1;  /**< SPI Wired-OR Mode */
      unsigned int LSB        :  1;  /**< LSB First Transfer Enable */
      unsigned int TIM        :  1;  /**< SPI Transfer and Interrupt Mode */
      unsigned int ZEN        :  1;  /**< Transmit Zeros Enable */
      unsigned int RXOF       :  1;  /**< Rx Overflow Overwrite Enable */
      unsigned int OEN        :  1;  /**< Slave MISO Output Enable */
      unsigned int LOOPBACK   :  1;  /**< Loopback Enable */
      unsigned int CON        :  1;  /**< Continuous Transfer Enable */
      unsigned int RFLUSH     :  1;  /**< SPI Rx FIFO Flush Enable */
      unsigned int TFLUSH     :  1;  /**< SPI Tx FIFO Flush Enable */
      unsigned int CSRST      :  1;  /**< Reset Mode for CS Error Bit */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_CTL_t__ */

/*@}*/

/** @defgroup IEN SPI Interrupts Enable (IEN) Register
 *  SPI Interrupts Enable (IEN) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_IEN_Struct
 *! \brief  SPI Interrupts Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_IEN_t__
typedef struct _ADI_SPI_IEN_t {
  union {
    struct {
      unsigned int IRQMODE    :  3;  /**< SPI IRQ Mode Bits */
      unsigned int reserved3  :  5;
      unsigned int CS         :  1;  /**< Enable Interrupt on Every CS Edge in Slave CON Mode */
      unsigned int TXUNDR     :  1;  /**< Tx Underflow Interrupt Enable */
      unsigned int RXOVR      :  1;  /**< Rx Overflow Interrupt Enable */
      unsigned int RDY        :  1;  /**< Ready Signal Edge Interrupt Enable */
      unsigned int TXDONE     :  1;  /**< SPI Transmit Done Interrupt Enable */
      unsigned int XFRDONE    :  1;  /**< SPI Transfer Completion Interrupt Enable */
      unsigned int TXEMPTY    :  1;  /**< Tx FIFO Empty Interrupt Enable */
      unsigned int reserved15 :  1;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_IEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_IEN_t__ */

/*@}*/

/** @defgroup CNT Transfer Byte Count (CNT) Register
 *  Transfer Byte Count (CNT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_CNT_Struct
 *! \brief  Transfer Byte Count Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_CNT_t__
typedef struct _ADI_SPI_CNT_t {
  union {
    struct {
      unsigned int VALUE      : 14;  /**< Transfer Byte Count */
      unsigned int reserved14 :  1;
      unsigned int FRAMECONT  :  1;  /**< Continue Frame */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_CNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_CNT_t__ */

/*@}*/

/** @defgroup DMA SPI DMA Enable (DMA) Register
 *  SPI DMA Enable (DMA) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_DMA_Struct
 *! \brief  SPI DMA Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_DMA_t__
typedef struct _ADI_SPI_DMA_t {
  union {
    struct {
      unsigned int EN         :  1;  /**< Enable DMA for Data Transfer */
      unsigned int TXEN       :  1;  /**< Enable Transmit DMA Request */
      unsigned int RXEN       :  1;  /**< Enable Receive DMA Request */
      unsigned int reserved3  : 13;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_DMA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_DMA_t__ */

/*@}*/

/** @defgroup FIFO_STAT FIFO Status (FIFO_STAT) Register
 *  FIFO Status (FIFO_STAT) Register.
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
      unsigned int TX         :  4;  /**< SPI Tx FIFO Status */
      unsigned int reserved4  :  4;
      unsigned int RX         :  4;  /**< SPI Rx FIFO Dtatus */
      unsigned int reserved12 :  4;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_FIFO_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_FIFO_STAT_t__ */

/*@}*/

/** @defgroup RD_CTL Read Control (RD_CTL) Register
 *  Read Control (RD_CTL) Register.
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
      unsigned int CMDEN      :  1;  /**< Read Command Enable */
      unsigned int OVERLAP    :  1;  /**< Tx/Rx Overlap Mode */
      unsigned int TXBYTES    :  4;  /**< Transmit Byte Count - 1 (Read Command) */
      unsigned int reserved6  :  2;
      unsigned int THREEPIN   :  1;  /**< Three Pin SPI Mode */
      unsigned int reserved9  :  7;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_RD_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_RD_CTL_t__ */

/*@}*/

/** @defgroup FLOW_CTL Flow Control (FLOW_CTL) Register
 *  Flow Control (FLOW_CTL) Register.
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
      unsigned int MODE       :  2;  /**< Flow Control Mode */
      unsigned int reserved2  :  2;
      unsigned int RDYPOL     :  1;  /**< Polarity of RDY/MISO Line */
      unsigned int reserved5  :  1;
      unsigned int RDBURSTSZ  : 10;  /**< Read Data Burst Size - 1 */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_FLOW_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_FLOW_CTL_t__ */

/*@}*/

/** @defgroup WAIT_TMR Wait Timer for Flow Control (WAIT_TMR) Register
 *  Wait Timer for Flow Control (WAIT_TMR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_WAIT_TMR_Struct
 *! \brief  Wait Timer for Flow Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_WAIT_TMR_t__
typedef struct _ADI_SPI_WAIT_TMR_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Wait Timer */
    };
    uint16_t VALUE16;
  };
} ADI_SPI_WAIT_TMR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_WAIT_TMR_t__ */

/*@}*/

/** @defgroup CS_CTL Chip Select Control for Multi-slave Connections (CS_CTL) Register
 *  Chip Select Control for Multi-slave Connections (CS_CTL) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_CS_CTL_Struct
 *! \brief  Chip Select Control for Multi-slave Connections Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_CS_CTL_t__
typedef struct _ADI_SPI_CS_CTL_t {
  union {
    struct {
      unsigned int SEL        :  4;  /**< Chip Select Control */
      unsigned int reserved4  : 12;
    };
    uint16_t VALUE16;
  };
} ADI_SPI_CS_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_CS_CTL_t__ */

/*@}*/

/** @defgroup CS_OVERRIDE Chip Select Override (CS_OVERRIDE) Register
 *  Chip Select Override (CS_OVERRIDE) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPI_CS_OVERRIDE_Struct
 *! \brief  Chip Select Override Register bit field structure
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
      unsigned int ERBFI      :  1;  /**< Receive Buffer Full Interrupt */
      unsigned int ETBEI      :  1;  /**< Transmit Buffer Empty Interrupt */
      unsigned int ELSI       :  1;  /**< Rx Status Interrupt */
      unsigned int EDSSI      :  1;  /**< Modem Status Interrupt */
      unsigned int EDMAT      :  1;  /**< DMA Requests in Transmit Mode */
      unsigned int EDMAR      :  1;  /**< DMA Requests in Receive Mode */
      unsigned int reserved6  : 10;
    };
    uint16_t VALUE16;
  };
} ADI_UART_IEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_IEN_t__ */

/*@}*/

/** @defgroup IIR Interrupt ID (IIR) Register
 *  Interrupt ID (IIR) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_UART_IIR_STAT
 *! \brief  Interrupt Status (STAT) Enumerations
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
      unsigned int NIRQ       :  1;  /**< Interrupt Flag */
      unsigned int STAT       :  3;  /**< Interrupt Status */
      unsigned int reserved4  :  2;
      unsigned int FEND       :  2;  /**< FIFO Enabled */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_IIR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_IIR_t__ */

/*@}*/

/** @defgroup LCR Line Control (LCR) Register
 *  Line Control (LCR) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_UART_LCR_SP
 *! \brief  Stick Parity (SP) Enumerations
 *  ========================================================================= */
typedef enum
{
  UART_LCR_PAR_NOTFORCED = 0,  /**< Parity will not be forced based on Parity Select and Parity Enable bits. */
  UART_LCR_PAR_FORCED    = 1   /**< Parity forced based on Parity Select and Parity Enable bits.             */
} ADI_UART_LCR_SP;


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
      unsigned int RTS        :  1;  /**< Request to Send */
      unsigned int OUT1       :  1;  /**< Output 1 */
      unsigned int OUT2       :  1;  /**< Output 2 */
      unsigned int LOOPBACK   :  1;  /**< Loopback Mode */
      unsigned int reserved5  : 11;
    };
    uint16_t VALUE16;
  };
} ADI_UART_MCR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_MCR_t__ */

/*@}*/

/** @defgroup LSR Line Status (LSR) Register
 *  Line Status (LSR) Register.
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
      unsigned int FIFOERR    :  1;  /**< Rx FIFO Parity Error/Frame Error/Break Indication */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_LSR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_LSR_t__ */

/*@}*/

/** @defgroup MSR Modem Status (MSR) Register
 *  Modem Status (MSR) Register.
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
      unsigned int CTS        :  1;  /**< Clear to Send */
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
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_UART_FCR_FDMAMD
 *! \brief  FIFO DMA Mode (FDMAMD) Enumerations
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
      unsigned int FIFOEN     :  1;  /**< FIFO Enable as to Work in 16550 Mode */
      unsigned int RFCLR      :  1;  /**< Clear Rx FIFO */
      unsigned int TFCLR      :  1;  /**< Clear Tx FIFO */
      unsigned int FDMAMD     :  1;  /**< FIFO DMA Mode */
      unsigned int reserved4  :  2;
      unsigned int RFTRIG     :  2;  /**< Rx FIFO Trigger Level */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_FCR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_FCR_t__ */

/*@}*/

/** @defgroup FBR Fractional Baud Rate (FBR) Register
 *  Fractional Baud Rate (FBR) Register.
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
      unsigned int DIVN       : 11;  /**< Fractional Baud Rate N Divide Bits 0 to 2047 */
      unsigned int DIVM       :  2;  /**< Fractional Baud Rate M Divide Bits 1 to 3 */
      unsigned int reserved13 :  2;
      unsigned int FBEN       :  1;  /**< Fractional Baud Rate Generator Enable */
    };
    uint16_t VALUE16;
  };
} ADI_UART_FBR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_FBR_t__ */

/*@}*/

/** @defgroup DIV Baud Rate Divider (DIV) Register
 *  Baud Rate Divider (DIV) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_UART_DIV_Struct
 *! \brief  Baud Rate Divider Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_DIV_t__
typedef struct _ADI_UART_DIV_t {
  union {
    struct {
      unsigned int DIV        : 16;  /**< Baud Rate Divider */
    };
    uint16_t VALUE16;
  };
} ADI_UART_DIV_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_DIV_t__ */

/*@}*/

/** @defgroup LCR2 Second Line Control (LCR2) Register
 *  Second Line Control (LCR2) Register.
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
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_UART_CTL_RXINV
 *! \brief  Invert Receiver Line (RXINV) Enumerations
 *  ========================================================================= */
typedef enum
{
  UART_CTL_NOTINV_RX = 0,  /**< Don't invert receiver line (idling high). */
  UART_CTL_INV_RX    = 1   /**< Invert receiver line (idling low).        */
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
      unsigned int RXINV      :  1;  /**< Invert Receiver Line */
      unsigned int reserved5  :  3;
      unsigned int REV        :  8;  /**< UART Revision ID */
    };
    uint16_t VALUE16;
  };
} ADI_UART_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_CTL_t__ */

/*@}*/

/** @defgroup RFC RX FIFO Byte Count (RFC) Register
 *  RX FIFO Byte Count (RFC) Register.
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
      unsigned int RFC        :  5;  /**< Current Rx FIFO Data Bytes */
      unsigned int reserved5  : 11;
    };
    uint16_t VALUE16;
  };
} ADI_UART_RFC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_RFC_t__ */

/*@}*/

/** @defgroup TFC TX FIFO Byte Count (TFC) Register
 *  TX FIFO Byte Count (TFC) Register.
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
      unsigned int TFC        :  5;  /**< Current Tx FIFO Data Bytes */
      unsigned int reserved5  : 11;
    };
    uint16_t VALUE16;
  };
} ADI_UART_TFC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_TFC_t__ */

/*@}*/

/** @defgroup RSC RS485 Half-duplex Control (RSC) Register
 *  RS485 Half-duplex Control (RSC) Register.
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
      unsigned int OENP       :  1;  /**< SOUT_EN Polarity */
      unsigned int OENSP      :  1;  /**< SOUT_EN De-assert Before Full Stop Bit(s) */
      unsigned int DISRX      :  1;  /**< Disable Rx When Transmitting */
      unsigned int DISTX      :  1;  /**< Hold off Tx When Receiving */
      unsigned int reserved4  : 12;
    };
    uint16_t VALUE16;
  };
} ADI_UART_RSC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_RSC_t__ */

/*@}*/

/** @defgroup ACR Auto Baud Control (ACR) Register
 *  Auto Baud Control (ACR) Register.
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
      unsigned int DNIEN      :  1;  /**< Enable Done Interrupt */
      unsigned int TOIEN      :  1;  /**< Enable Time-out Interrupt */
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
      unsigned int DONE       :  1;  /**< Auto Baud Done Successfully */
      unsigned int BRKTO      :  1;  /**< Timed Out Due to Long Time Break Condition */
      unsigned int NSETO      :  1;  /**< Timed Out Due to No Valid Start Edge Found */
      unsigned int NEETO      :  1;  /**< Timed Out Due to No Valid Ending Edge Found */
      unsigned int CNT        : 12;  /**< CNT[11:0] Auto Baud Counter Value */
    };
    uint16_t VALUE16;
  };
} ADI_UART_ASRL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_ASRL_t__ */

/*@}*/

/** @defgroup ASRH Auto Baud Status (High) (ASRH) Register
 *  Auto Baud Status (High) (ASRH) Register.
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
      unsigned int CNT        :  8;  /**< CNT[19:12] Auto Baud Counter Value */
      unsigned int reserved8  :  8;
    };
    uint16_t VALUE16;
  };
} ADI_UART_ASRH_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_ASRH_t__ */

/*@}*/

/** @defgroup CFG Beeper Configuration (CFG) Register
 *  Beeper Configuration (CFG) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BEEP_CFG_Struct
 *! \brief  Beeper Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BEEP_CFG_t__
typedef struct _ADI_BEEP_CFG_t {
  union {
    struct {
      unsigned int SEQREPEAT  :  8;  /**< Beeper Sequence Repeat Value */
      unsigned int EN         :  1;  /**< Beeper Enable */
      unsigned int reserved9  :  1;
      unsigned int ASTARTIRQ  :  1;  /**< Tone A Start IRQ */
      unsigned int AENDIRQ    :  1;  /**< Tone A End IRQ */
      unsigned int BSTARTIRQ  :  1;  /**< Tone B Start IRQ */
      unsigned int BENDIRQ    :  1;  /**< Tone B End IRQ */
      unsigned int SEQNEARENDIRQ :  1;  /**< Sequence 1 Cycle from End IRQ */
      unsigned int SEQATENDIRQ   :  1;  /**< Sequence End IRQ */
    };
    uint16_t VALUE16;
  };
} ADI_BEEP_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BEEP_CFG_t__ */

/*@}*/

/** @defgroup STAT Beeper Status (STAT) Register
 *  Beeper Status (STAT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_BEEP_STAT_Struct
 *! \brief  Beeper Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BEEP_STAT_t__
typedef struct _ADI_BEEP_STAT_t {
  union {
    struct {
      unsigned int SEQREMAIN  :  8;  /**< Remaining Tone-pair Iterations to Play in Sequence Mode */
      unsigned int BUSY       :  1;  /**< Beeper is Busy */
      unsigned int reserved9  :  1;
      unsigned int ASTARTED   :  1;  /**< Tone A Has Started */
      unsigned int AENDED     :  1;  /**< Tone A Has Ended */
      unsigned int BSTARTED   :  1;  /**< Tone B Has Started */
      unsigned int BENDED     :  1;  /**< Tone B Has Ended */
      unsigned int SEQNEAREND :  1;  /**< Sequencer Last Tone-pair Has Started */
      unsigned int SEQENDED   :  1;  /**< Sequencer Has Ended */
    };
    uint16_t VALUE16;
  };
} ADI_BEEP_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BEEP_STAT_t__ */

/*@}*/

/** @defgroup TONEA Tone A Data (TONEA) Register
 *  Tone A Data (TONEA) Register.
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
      unsigned int DUR        :  8;  /**< Tone Duration */
      unsigned int FREQ       :  7;  /**< Tone Frequency */
      unsigned int DIS        :  1;  /**< Output Disable */
    };
    uint16_t VALUE16;
  };
} ADI_BEEP_TONEA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BEEP_TONEA_t__ */

/*@}*/

/** @defgroup TONEB Tone B Data (TONEB) Register
 *  Tone B Data (TONEB) Register.
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
      unsigned int DUR        :  8;  /**< Tone Duration */
      unsigned int FREQ       :  7;  /**< Tone Frequency */
      unsigned int DIS        :  1;  /**< Output Disable */
    };
    uint16_t VALUE16;
  };
} ADI_BEEP_TONEB_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BEEP_TONEB_t__ */

/*@}*/

/** @defgroup CFG ADC Configuration (CFG) Register
 *  ADC Configuration (CFG) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_ADC_CFG_VREFSEL
 *! \brief  To select Vref as 1.25 V or 2.5 V (VREFSEL) Enumerations
 *  ========================================================================= */
typedef enum
{
  ADC_CFG_V_2P5  = 0,  /**< Vref = 2.5 V  */
  ADC_CFG_V_1P25 = 1   /**< Vref = 1.25 V */
} ADI_ADC_CFG_VREFSEL;


/*  =========================================================================
 *! \enum   ADI_ADC_CFG_REFBUFEN
 *! \brief  To enable internal reference buffer (REFBUFEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  ADC_CFG_EXT_REF = 0,  /**< External reference is used  */
  ADC_CFG_BUF_REF = 1   /**< Reference buffer is enabled */
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
      unsigned int VREFSEL    :  1;  /**< To select Vref as 1.25 V or 2.5 V */
      unsigned int REFBUFEN   :  1;  /**< To enable internal reference buffer */
      unsigned int VREFVBAT   :  1;  /**< VRef VBAT */
      unsigned int EN         :  1;  /**< To enable ADC subsystem */
      unsigned int STARTCAL   :  1;  /**< To start a new offset calibration cycle */
      unsigned int RST        :  1;  /**< Resets internal buffers and registers when high */
      unsigned int SINKEN     :  1;  /**< To enable additional 50 uA sink current capability @1.25 V, 100 uA current capability @2.5 V */
      unsigned int TMPEN      :  1;  /**< To power up temperature sensor */
      unsigned int FAST_DISCH :  1;  /**< For fast switchover of Vref from 2.5 V to 1.25 V */
      unsigned int VREFVBAT_DEL :  1;  /**< Set to 1 after minimum delay of 700 us from VREFBAT field being set to 1 */
      unsigned int reserved11   :  5;
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CFG_t__ */

/*@}*/

/** @defgroup PWRUP ADC Power-up Time (PWRUP) Register
 *  ADC Power-up Time (PWRUP) Register.
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

/** @defgroup CH0_OUT Conversion Result Channel 0 (CH0_OUT) Register
 *  Conversion Result Channel 0 (CH0_OUT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH0_OUT_Struct
 *! \brief  Conversion Result Channel 0 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH0_OUT_t__
typedef struct _ADI_ADC_CH0_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 0 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH0_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH0_OUT_t__ */

/*@}*/

/** @defgroup CH1_OUT Conversion Result Channel 1 (CH1_OUT) Register
 *  Conversion Result Channel 1 (CH1_OUT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH1_OUT_Struct
 *! \brief  Conversion Result Channel 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH1_OUT_t__
typedef struct _ADI_ADC_CH1_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 1 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH1_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH1_OUT_t__ */

/*@}*/

/** @defgroup CH2_OUT Conversion Result Channel 2 (CH2_OUT) Register
 *  Conversion Result Channel 2 (CH2_OUT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH2_OUT_Struct
 *! \brief  Conversion Result Channel 2 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH2_OUT_t__
typedef struct _ADI_ADC_CH2_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 2 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH2_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH2_OUT_t__ */

/*@}*/

/** @defgroup CH3_OUT Conversion Result Channel 3 (CH3_OUT) Register
 *  Conversion Result Channel 3 (CH3_OUT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH3_OUT_Struct
 *! \brief  Conversion Result Channel 3 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH3_OUT_t__
typedef struct _ADI_ADC_CH3_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 3 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH3_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH3_OUT_t__ */

/*@}*/

/** @defgroup CH4_OUT Conversion Result Channel 4 (CH4_OUT) Register
 *  Conversion Result Channel 4 (CH4_OUT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH4_OUT_Struct
 *! \brief  Conversion Result Channel 4 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH4_OUT_t__
typedef struct _ADI_ADC_CH4_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 4 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH4_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH4_OUT_t__ */

/*@}*/

/** @defgroup CH5_OUT Conversion Result Channel 5 (CH5_OUT) Register
 *  Conversion Result Channel 5 (CH5_OUT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH5_OUT_Struct
 *! \brief  Conversion Result Channel 5 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH5_OUT_t__
typedef struct _ADI_ADC_CH5_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 5 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH5_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH5_OUT_t__ */

/*@}*/

/** @defgroup CH6_OUT Conversion Result Channel 6 (CH6_OUT) Register
 *  Conversion Result Channel 6 (CH6_OUT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH6_OUT_Struct
 *! \brief  Conversion Result Channel 6 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH6_OUT_t__
typedef struct _ADI_ADC_CH6_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 6 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH6_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH6_OUT_t__ */

/*@}*/

/** @defgroup CH7_OUT Conversion Result Channel 7 (CH7_OUT) Register
 *  Conversion Result Channel 7 (CH7_OUT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_ADC_CH7_OUT_Struct
 *! \brief  Conversion Result Channel 7 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_CH7_OUT_t__
typedef struct _ADI_ADC_CH7_OUT_t {
  union {
    struct {
      unsigned int RESULT     : 16;  /**< Conversion result of channel 7 is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_CH7_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_CH7_OUT_t__ */

/*@}*/

/** @defgroup BAT_OUT Battery Monitoring Result (BAT_OUT) Register
 *  Battery Monitoring Result (BAT_OUT) Register.
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
      unsigned int RESULT     : 16;  /**< Conversion result of battery monitoring is stored here */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_BAT_OUT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_BAT_OUT_t__ */

/*@}*/

/** @defgroup TMP_OUT Temperature Result (TMP_OUT) Register
 *  Temperature Result (TMP_OUT) Register.
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
      unsigned int EN         :  1;  /**< To enable hysteresis for comparison on Channel 3 */
    };
    uint16_t VALUE16;
  };
} ADI_ADC_HYS3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_HYS3_t__ */

/*@}*/

/** @defgroup CFG1 Reference Buffer Low Power Mode (CFG1) Register
 *  Reference Buffer Low Power Mode (CFG1) Register.
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
      unsigned int MEN        :  1;  /**< Enable Status of the Controller */
      unsigned int reserved1  : 15;
      unsigned int CHANM1     :  5;  /**< Number of Available DMA Channels Minus 1 */
      unsigned int reserved21 : 11;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_STAT_t__ */

/*@}*/

/** @defgroup CFG DMA Configuration (CFG) Register
 *  DMA Configuration (CFG) Register.
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
      unsigned int MEN        :  1;  /**< Controller Enable */
      unsigned int reserved1  : 31;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_CFG_t__ */

/*@}*/

/** @defgroup PDBPTR DMA Channel Primary Control Database Pointer (PDBPTR) Register
 *  DMA Channel Primary Control Database Pointer (PDBPTR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_PDBPTR_Struct
 *! \brief  DMA Channel Primary Control Database Pointer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_PDBPTR_t__
typedef struct _ADI_DMA_PDBPTR_t {
  union {
    struct {
      unsigned int ADDR       : 32;  /**< Pointer to the Base Address of the Primary Data Structure */
    };
    uint32_t VALUE32;
  };
} ADI_DMA_PDBPTR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_PDBPTR_t__ */

/*@}*/

/** @defgroup ADBPTR DMA Channel Alternate Control Database Pointer (ADBPTR) Register
 *  DMA Channel Alternate Control Database Pointer (ADBPTR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ADBPTR_Struct
 *! \brief  DMA Channel Alternate Control Database Pointer Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ADBPTR_t__
typedef struct _ADI_DMA_ADBPTR_t {
  union {
    struct {
      unsigned int ADDR       : 32;  /**< Base Address of the Alternate Data Structure */
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ADBPTR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ADBPTR_t__ */

/*@}*/

/** @defgroup SWREQ DMA Channel Software Request (SWREQ) Register
 *  DMA Channel Software Request (SWREQ) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_SWREQ_Struct
 *! \brief  DMA Channel Software Request Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_SWREQ_t__
typedef struct _ADI_DMA_SWREQ_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Generate Software Request */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_SWREQ_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_SWREQ_t__ */

/*@}*/

/** @defgroup RMSK_SET DMA Channel Request Mask Set (RMSK_SET) Register
 *  DMA Channel Request Mask Set (RMSK_SET) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_RMSK_SET_Struct
 *! \brief  DMA Channel Request Mask Set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_RMSK_SET_t__
typedef struct _ADI_DMA_RMSK_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Mask Requests from DMA Channels */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_RMSK_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_RMSK_SET_t__ */

/*@}*/

/** @defgroup RMSK_CLR DMA Channel Request Mask Clear (RMSK_CLR) Register
 *  DMA Channel Request Mask Clear (RMSK_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_RMSK_CLR_Struct
 *! \brief  DMA Channel Request Mask Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_RMSK_CLR_t__
typedef struct _ADI_DMA_RMSK_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Clear Request Mask Set Bits */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_RMSK_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_RMSK_CLR_t__ */

/*@}*/

/** @defgroup EN_SET DMA Channel Enable Set (EN_SET) Register
 *  DMA Channel Enable Set (EN_SET) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_EN_SET_Struct
 *! \brief  DMA Channel Enable Set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_EN_SET_t__
typedef struct _ADI_DMA_EN_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Enable DMA Channels */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_EN_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_EN_SET_t__ */

/*@}*/

/** @defgroup EN_CLR DMA Channel Enable Clear (EN_CLR) Register
 *  DMA Channel Enable Clear (EN_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_EN_CLR_Struct
 *! \brief  DMA Channel Enable Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_EN_CLR_t__
typedef struct _ADI_DMA_EN_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Disable DMA Channels */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_EN_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_EN_CLR_t__ */

/*@}*/

/** @defgroup ALT_SET DMA Channel Primary Alternate Set (ALT_SET) Register
 *  DMA Channel Primary Alternate Set (ALT_SET) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ALT_SET_Struct
 *! \brief  DMA Channel Primary Alternate Set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ALT_SET_t__
typedef struct _ADI_DMA_ALT_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Control Structure Status / Select Alternate Structure */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ALT_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ALT_SET_t__ */

/*@}*/

/** @defgroup ALT_CLR DMA Channel Primary Alternate Clear (ALT_CLR) Register
 *  DMA Channel Primary Alternate Clear (ALT_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ALT_CLR_Struct
 *! \brief  DMA Channel Primary Alternate Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ALT_CLR_t__
typedef struct _ADI_DMA_ALT_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Select Primary Data Structure */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ALT_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ALT_CLR_t__ */

/*@}*/

/** @defgroup PRI_SET DMA Channel Priority Set (PRI_SET) Register
 *  DMA Channel Priority Set (PRI_SET) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_PRI_SET_Struct
 *! \brief  DMA Channel Priority Set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_PRI_SET_t__
typedef struct _ADI_DMA_PRI_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Configure Channel for High Priority */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_PRI_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_PRI_SET_t__ */

/*@}*/

/** @defgroup PRI_CLR DMA Channel Priority Clear (PRI_CLR) Register
 *  DMA Channel Priority Clear (PRI_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_PRI_CLR_Struct
 *! \brief  DMA Channel Priority Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_PRI_CLR_t__
typedef struct _ADI_DMA_PRI_CLR_t {
  union {
    struct {
      unsigned int CHPRICLR   : 27;  /**< Configure Channel for Default Priority Level */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_PRI_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_PRI_CLR_t__ */

/*@}*/

/** @defgroup ERRCHNL_CLR DMA per Channel Error Clear (ERRCHNL_CLR) Register
 *  DMA per Channel Error Clear (ERRCHNL_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ERRCHNL_CLR_Struct
 *! \brief  DMA per Channel Error Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ERRCHNL_CLR_t__
typedef struct _ADI_DMA_ERRCHNL_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Per Channel Bus Error Status/Clear */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ERRCHNL_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ERRCHNL_CLR_t__ */

/*@}*/

/** @defgroup ERR_CLR DMA Bus Error Clear (ERR_CLR) Register
 *  DMA Bus Error Clear (ERR_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_ERR_CLR_Struct
 *! \brief  DMA Bus Error Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_ERR_CLR_t__
typedef struct _ADI_DMA_ERR_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Bus Error Status */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_ERR_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_ERR_CLR_t__ */

/*@}*/

/** @defgroup INVALIDDESC_CLR DMA per Channel Invalid Descriptor Clear (INVALIDDESC_CLR) Register
 *  DMA per Channel Invalid Descriptor Clear (INVALIDDESC_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_INVALIDDESC_CLR_Struct
 *! \brief  DMA per Channel Invalid Descriptor Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_INVALIDDESC_CLR_t__
typedef struct _ADI_DMA_INVALIDDESC_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Per Channel Invalid Descriptor Status/Clear */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_INVALIDDESC_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_INVALIDDESC_CLR_t__ */

/*@}*/

/** @defgroup BS_SET DMA Channel Bytes Swap Enable Set (BS_SET) Register
 *  DMA Channel Bytes Swap Enable Set (BS_SET) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_BS_SET_Struct
 *! \brief  DMA Channel Bytes Swap Enable Set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_BS_SET_t__
typedef struct _ADI_DMA_BS_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Byte Swap Status */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_BS_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_BS_SET_t__ */

/*@}*/

/** @defgroup BS_CLR DMA Channel Bytes Swap Enable Clear (BS_CLR) Register
 *  DMA Channel Bytes Swap Enable Clear (BS_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_BS_CLR_Struct
 *! \brief  DMA Channel Bytes Swap Enable Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_BS_CLR_t__
typedef struct _ADI_DMA_BS_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Disable Byte Swap */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_BS_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_BS_CLR_t__ */

/*@}*/

/** @defgroup SRCADDR_SET DMA Channel Source Address Decrement Enable Set (SRCADDR_SET) Register
 *  DMA Channel Source Address Decrement Enable Set (SRCADDR_SET) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_SRCADDR_SET_Struct
 *! \brief  DMA Channel Source Address Decrement Enable Set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_SRCADDR_SET_t__
typedef struct _ADI_DMA_SRCADDR_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Source Address Decrement Status */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_SRCADDR_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_SRCADDR_SET_t__ */

/*@}*/

/** @defgroup SRCADDR_CLR DMA Channel Source Address Decrement Enable Clear (SRCADDR_CLR) Register
 *  DMA Channel Source Address Decrement Enable Clear (SRCADDR_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_SRCADDR_CLR_Struct
 *! \brief  DMA Channel Source Address Decrement Enable Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_SRCADDR_CLR_t__
typedef struct _ADI_DMA_SRCADDR_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Disable Source Address Decrement */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_SRCADDR_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_SRCADDR_CLR_t__ */

/*@}*/

/** @defgroup DSTADDR_SET DMA Channel Destination Address Decrement Enable Set (DSTADDR_SET) Register
 *  DMA Channel Destination Address Decrement Enable Set (DSTADDR_SET) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_DSTADDR_SET_Struct
 *! \brief  DMA Channel Destination Address Decrement Enable Set Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_DSTADDR_SET_t__
typedef struct _ADI_DMA_DSTADDR_SET_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Destination Address Decrement Status */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_DSTADDR_SET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_DSTADDR_SET_t__ */

/*@}*/

/** @defgroup DSTADDR_CLR DMA Channel Destination Address Decrement Enable Clear (DSTADDR_CLR) Register
 *  DMA Channel Destination Address Decrement Enable Clear (DSTADDR_CLR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_DMA_DSTADDR_CLR_Struct
 *! \brief  DMA Channel Destination Address Decrement Enable Clear Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_DSTADDR_CLR_t__
typedef struct _ADI_DMA_DSTADDR_CLR_t {
  union {
    struct {
      unsigned int CHAN       : 27;  /**< Disable Destination Address Decrement */
      unsigned int reserved27 :  5;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_DSTADDR_CLR_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_DSTADDR_CLR_t__ */

/*@}*/

/** @defgroup REVID DMA Controller Revision ID (REVID) Register
 *  DMA Controller Revision ID (REVID) Register.
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
      unsigned int VALUE      :  8;  /**< DMA Controller Revision ID */
      unsigned int reserved8  : 24;
    };
    uint32_t VALUE32;
  };
} ADI_DMA_REVID_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_REVID_t__ */

/*@}*/

/** @defgroup STAT Status (STAT) Register
 *  Status (STAT) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_FLCC_STAT_ACCESS_MODE
 *! \brief  Access Mode (ACCESS_MODE) Enumerations
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
      unsigned int ACCESS_MODE   :  1;  /**< Access Mode */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_STAT_t__ */

/*@}*/

/** @defgroup IEN Interrupt Enable (IEN) Register
 *  Interrupt Enable (IEN) Register.
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

/** @defgroup KH_ADDR Write Address (KH_ADDR) Register
 *  Write Address (KH_ADDR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_KH_ADDR_Struct
 *! \brief  Write Address Register bit field structure
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

/** @defgroup KH_DATA0 Write Lower Data (KH_DATA0) Register
 *  Write Lower Data (KH_DATA0) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_KH_DATA0_Struct
 *! \brief  Write Lower Data Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_KH_DATA0_t__
typedef struct _ADI_FLCC_KH_DATA0_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Lower half of 64-bit dual word data to be written on a Write command */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_KH_DATA0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_KH_DATA0_t__ */

/*@}*/

/** @defgroup KH_DATA1 Write Upper Data (KH_DATA1) Register
 *  Write Upper Data (KH_DATA1) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_KH_DATA1_Struct
 *! \brief  Write Upper Data Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_KH_DATA1_t__
typedef struct _ADI_FLCC_KH_DATA1_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< Upper half of 64-bit dual word data to be written on a Write command */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_KH_DATA1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_KH_DATA1_t__ */

/*@}*/

/** @defgroup PAGE_ADDR0 Lower Page Address (PAGE_ADDR0) Register
 *  Lower Page Address (PAGE_ADDR0) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_PAGE_ADDR0_Struct
 *! \brief  Lower Page Address Register bit field structure
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

/** @defgroup PAGE_ADDR1 Upper Page Address (PAGE_ADDR1) Register
 *  Upper Page Address (PAGE_ADDR1) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_PAGE_ADDR1_Struct
 *! \brief  Upper Page Address Register bit field structure
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

/** @defgroup WR_ABORT_ADDR Write Abort Address (WR_ABORT_ADDR) Register
 *  Write Abort Address (WR_ABORT_ADDR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_WR_ABORT_ADDR_Struct
 *! \brief  Write Abort Address Register bit field structure
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

/** @defgroup WRPROT Write Protection (WRPROT) Register
 *  Write Protection (WRPROT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_WRPROT_Struct
 *! \brief  Write Protection Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_WRPROT_t__
typedef struct _ADI_FLCC_WRPROT_t {
  union {
    struct {
      unsigned int WORD       : 32;  /**< Clear bits to write protect related groups of user space pages */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_WRPROT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_WRPROT_t__ */

/*@}*/

/** @defgroup SIGNATURE Signature (SIGNATURE) Register
 *  Signature (SIGNATURE) Register.
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

/** @defgroup UCFG User Configuration (UCFG) Register
 *  User Configuration (UCFG) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_UCFG_Struct
 *! \brief  User Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_UCFG_t__
typedef struct _ADI_FLCC_UCFG_t {
  union {
    struct {
      unsigned int KHDMAEN    :  1;  /**< Key hole DMA enable */
      unsigned int AUTOINCEN  :  1;  /**< Auto Address Increment for Key Hole Access */
      unsigned int reserved2  : 30;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_UCFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_UCFG_t__ */

/*@}*/

/** @defgroup TIME_PARAM0 Time Parameter 0 (TIME_PARAM0) Register
 *  Time Parameter 0 (TIME_PARAM0) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_TIME_PARAM0_Struct
 *! \brief  Time Parameter 0 Register bit field structure
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

/** @defgroup TIME_PARAM1 Time Parameter 1 (TIME_PARAM1) Register
 *  Time Parameter 1 (TIME_PARAM1) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_TIME_PARAM1_Struct
 *! \brief  Time Parameter 1 Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_TIME_PARAM1_t__
typedef struct _ADI_FLCC_TIME_PARAM1_t {
  union {
    struct {
      unsigned int TWK        :  4;  /**< Wake up time */
      unsigned int WAITSTATES :  3;  /**< Number of wait states to access flash */
      unsigned int reserved7     :  1;
      unsigned int CURWAITSTATES :  3;  /**< Current wait states [2:0] */
      unsigned int reserved11    : 21;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_TIME_PARAM1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_TIME_PARAM1_t__ */

/*@}*/

/** @defgroup ABORT_EN_LO IRQ Abort Enable (Lower Bits) (ABORT_EN_LO) Register
 *  IRQ Abort Enable (Lower Bits) (ABORT_EN_LO) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_ABORT_EN_LO_Struct
 *! \brief  IRQ Abort Enable (Lower Bits) Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_ABORT_EN_LO_t__
typedef struct _ADI_FLCC_ABORT_EN_LO_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< VALUE[31:0] Sys IRQ Abort Enable */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_ABORT_EN_LO_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_ABORT_EN_LO_t__ */

/*@}*/

/** @defgroup ABORT_EN_HI IRQ Abort Enable (Upper Bits) (ABORT_EN_HI) Register
 *  IRQ Abort Enable (Upper Bits) (ABORT_EN_HI) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_ABORT_EN_HI_Struct
 *! \brief  IRQ Abort Enable (Upper Bits) Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_ABORT_EN_HI_t__
typedef struct _ADI_FLCC_ABORT_EN_HI_t {
  union {
    struct {
      unsigned int VALUE      : 32;  /**< VALUE[63:32] Sys IRQ Abort Enable */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_ABORT_EN_HI_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_ABORT_EN_HI_t__ */

/*@}*/

/** @defgroup ECC_CFG ECC Configuration (ECC_CFG) Register
 *  ECC Configuration (ECC_CFG) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_ECC_CFG_Struct
 *! \brief  ECC Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_ECC_CFG_t__
typedef struct _ADI_FLCC_ECC_CFG_t {
  union {
    struct {
      unsigned int EN         :  1;  /**< ECC Enable */
      unsigned int INFOEN     :  1;  /**< Info space ECC Enable bit */
      unsigned int reserved2  :  6;
      unsigned int PTR        : 24;  /**< ECC start page pointer */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_ECC_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_ECC_CFG_t__ */

/*@}*/

/** @defgroup ECC_ADDR ECC Status (Address) (ECC_ADDR) Register
 *  ECC Status (Address) (ECC_ADDR) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_ECC_ADDR_Struct
 *! \brief  ECC Status (Address) Register bit field structure
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

/** @defgroup POR_SEC Flash Security (POR_SEC) Register
 *  Flash Security (POR_SEC) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_POR_SEC_Struct
 *! \brief  Flash Security Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_POR_SEC_t__
typedef struct _ADI_FLCC_POR_SEC_t {
  union {
    struct {
      unsigned int SECURE     :  1;  /**< Set this bit to prevent read or write access to User Space (sticky when set) */
      unsigned int reserved1  : 31;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_POR_SEC_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_POR_SEC_t__ */

/*@}*/

/** @defgroup VOL_CFG Volatile Flash Configuration (VOL_CFG) Register
 *  Volatile Flash Configuration (VOL_CFG) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_FLCC_VOL_CFG_Struct
 *! \brief  Volatile Flash Configuration Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_VOL_CFG_t__
typedef struct _ADI_FLCC_VOL_CFG_t {
  union {
    struct {
      unsigned int INFO_REMAP :  1;  /**< Alias the info space to the base address of user space */
      unsigned int reserved1  : 31;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_VOL_CFG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_VOL_CFG_t__ */

/*@}*/

/** @defgroup STAT Cache Status Register (STAT) Register
 *  Cache Status Register (STAT) Register.
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
      unsigned int ICEN       :  1;  /**< If this bit is set,  I-Cache is enabled */
      unsigned int reserved1  : 31;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_CACHE_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_STAT_t__ */

/*@}*/

/** @defgroup SETUP Cache Setup Register (SETUP) Register
 *  Cache Setup Register (SETUP) Register.
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
      unsigned int ICEN       :  1;  /**< If this bit set, I-Cache is enabled for AHB accesses */
      unsigned int LCKIC      :  1;  /**< If this bit is set, I-Cache contents are locked */
      unsigned int reserved2  : 30;
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_CACHE_SETUP_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_SETUP_t__ */

/*@}*/

/** @defgroup KEY Cache Key Register (KEY) Register
 *  Cache Key Register (KEY) Register.
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
      unsigned int VALUE      : 32;  /**< Cache Key */
    };
    uint32_t VALUE32;
  };
} ADI_FLCC_CACHE_KEY_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_KEY_t__ */

/*@}*/

/** @defgroup CFG Port  Configuration (CFG) Register
 *  Port  Configuration (CFG) Register.
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

/** @defgroup CNVT_A Half SPORT 'A' CNV width (CNVT_A) Register
 *  Half SPORT 'A' CNV width (CNVT_A) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_CNVT_A_Struct
 *! \brief  Half SPORT 'A' CNV width Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_CNVT_A_t__
typedef struct _ADI_SPORT_CNVT_A_t {
  union {
    struct {
      unsigned int WID        :  4;  /**< CNV signal width: Half SPORT A */
      unsigned int reserved4  :  4;
      unsigned int POL        :  1;  /**< Polarity of the CNV signal */
      unsigned int reserved9  :  7;
      unsigned int CNVT2FS    :  8;  /**< CNV to FS duration: Half SPORT A */
      unsigned int reserved24 :  8;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_CNVT_A_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_CNVT_A_t__ */

/*@}*/

/** @defgroup TX_A Half SPORT 'A' Tx Buffer Register (TX_A) Register
 *  Half SPORT 'A' Tx Buffer Register (TX_A) Register.
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

/** @defgroup CNVT_B Half SPORT 'B' CNV width register (CNVT_B) Register
 *  Half SPORT 'B' CNV width register (CNVT_B) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_SPORT_CNVT_B_Struct
 *! \brief  Half SPORT 'B' CNV width Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_CNVT_B_t__
typedef struct _ADI_SPORT_CNVT_B_t {
  union {
    struct {
      unsigned int WID        :  4;  /**< CNV signal width: Half SPORT B */
      unsigned int reserved4  :  4;
      unsigned int POL        :  1;  /**< Polarity of the CNV signal */
      unsigned int reserved9  :  7;
      unsigned int CNVT2FS    :  8;  /**< CNV to FS duration: Half SPORT B */
      unsigned int reserved24 :  8;
    };
    uint32_t VALUE32;
  };
} ADI_SPORT_CNVT_B_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_CNVT_B_t__ */

/*@}*/

/** @defgroup TX_B Half SPORT 'B' Tx Buffer Register (TX_B) Register
 *  Half SPORT 'B' Tx Buffer Register (TX_B) Register.
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

/** @defgroup CTL CRC Control (CTL) Register
 *  CRC Control (CTL) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_CRC_CTL_EN
 *! \brief  CRC Peripheral Enable (EN) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRC_CTL_CRC_DIS = 0,  /**< CRC peripheral is disabled */
  CRC_CTL_CRC_EN  = 1   /**< CRC peripheral is enabled  */
} ADI_CRC_CTL_EN;


/*  =========================================================================
 *! \enum   ADI_CRC_CTL_LSBFIRST
 *! \brief  LSB First Calculation Order (LSBFIRST) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRC_CTL_MSB_FIRST = 0,  /**< MSB First CRC calculation is done */
  CRC_CTL_LSB_FIRST = 1   /**< LSB First CRC calculation is done */
} ADI_CRC_CTL_LSBFIRST;


/*  =========================================================================
 *! \enum   ADI_CRC_CTL_BITMIRR
 *! \brief  Bit Mirroring (BITMIRR) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRC_CTL_BITMIRR_DIS = 0,  /**< Bit Mirroring is disabled */
  CRC_CTL_BITMIRR_EN  = 1   /**< Bit Mirroring is enabled  */
} ADI_CRC_CTL_BITMIRR;


/*  =========================================================================
 *! \enum   ADI_CRC_CTL_BYTMIRR
 *! \brief  Byte Mirroring (BYTMIRR) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRC_CTL_BYTEMIR_DIS = 0,  /**< Byte Mirroring is disabled */
  CRC_CTL_BYTEMIR_EN  = 1   /**< Byte Mirroring is enabled  */
} ADI_CRC_CTL_BYTMIRR;


/*  =========================================================================
 *! \enum   ADI_CRC_CTL_W16SWP
 *! \brief  Word16 Swap (W16SWP) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRC_CTL_W16SP_DIS = 0,  /**< Word16 Swap disabled */
  CRC_CTL_W16SP_EN  = 1   /**< Word16 Swap enabled  */
} ADI_CRC_CTL_W16SWP;


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

/** @defgroup IPDATA Input Data Word (IPDATA) Register
 *  Input Data Word (IPDATA) Register.
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

/** @defgroup RESULT CRC Result (RESULT) Register
 *  CRC Result (RESULT) Register.
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
 *! \brief  Generate a Single Number (SINGLE) Enumerations
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
      unsigned int SINGLE     :  1;  /**< Generate a Single Number */
      unsigned int reserved4  : 12;
    };
    uint16_t VALUE16;
  };
} ADI_RNG_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_CTL_t__ */

/*@}*/

/** @defgroup LEN RNG Sample Length Register (LEN) Register
 *  RNG Sample Length Register (LEN) Register.
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
      unsigned int RELOAD     : 12;  /**< Reload Value for the Sample Counter */
      unsigned int PRESCALE   :  4;  /**< Prescaler for the Sample Counter */
    };
    uint16_t VALUE16;
  };
} ADI_RNG_LEN_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_LEN_t__ */

/*@}*/

/** @defgroup STAT RNG Status Register (STAT) Register
 *  RNG Status Register (STAT) Register.
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
      unsigned int RNRDY      :  1;  /**< Random Number Ready */
      unsigned int STUCK      :  1;  /**< Sampled Data Stuck High or Low */
      unsigned int reserved2  : 14;
    };
    uint16_t VALUE16;
  };
} ADI_RNG_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_STAT_t__ */

/*@}*/

/** @defgroup DATA RNG Data Register (DATA) Register
 *  RNG Data Register (DATA) Register.
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
      unsigned int VALUE      :  8;  /**< Value of the CRC Accumulator */
      unsigned int BUFF       : 24;  /**< Buffer for RNG Data */
    };
    uint32_t VALUE32;
  };
} ADI_RNG_DATA_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_DATA_t__ */

/*@}*/

/** @defgroup OSCCNT Oscillator Count (OSCCNT) Register
 *  Oscillator Count (OSCCNT) Register.
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
      unsigned int VALUE      : 28;  /**< Oscillator Count */
      unsigned int reserved28 :  4;
    };
    uint32_t VALUE32;
  };
} ADI_RNG_OSCCNT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_OSCCNT_t__ */

/*@}*/

/** @defgroup OSCDIFF Oscillator Difference (OSCDIFF) Register
 *  Oscillator Difference (OSCDIFF) Register.
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
      signed int DELTA      :  8;  /**< Oscillator Count Difference */
    };
    int8_t VALUE8;
  };
} ADI_RNG_OSCDIFF_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_OSCDIFF_t__ */

/*@}*/

/** @defgroup CFG Configuration Register (CFG) Register
 *  Configuration Register (CFG) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_CRYPT_CFG_BLKEN
 *! \brief  Enable Bit for Crypto Block (BLKEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRYPT_CFG_ENABLE  = 0,  /**< Enable Crypto Block  */
  CRYPT_CFG_DISABLE = 1   /**< Disable Crypto Block */
} ADI_CRYPT_CFG_BLKEN;


/*  =========================================================================
 *! \enum   ADI_CRYPT_CFG_INDMAEN
 *! \brief  Enable DMA Channel Request for Input Buffer (INDMAEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRYPT_CFG_DMA_DISABLE_INBUF = 0,  /**< Disable DMA Requesting for Input Buffer */
  CRYPT_CFG_DMA_ENABLE_INBUF  = 1   /**< Enable DMA Requesting for Input Buffer  */
} ADI_CRYPT_CFG_INDMAEN;


/*  =========================================================================
 *! \enum   ADI_CRYPT_CFG_OUTDMAEN
 *! \brief  Enable DMA Channel Request for Output Buffer (OUTDMAEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRYPT_CFG_DMA_DISABLE_OUTBUF = 0,  /**< Disable DMA Requesting for Output Buffer */
  CRYPT_CFG_DMA_ENABLE_OUTBUF  = 1   /**< Enable DMA Requesting for Output Buffer  */
} ADI_CRYPT_CFG_OUTDMAEN;


/*  =========================================================================
 *! \enum   ADI_CRYPT_CFG_AESKEYLEN
 *! \brief  Select Key Length for AES Cipher (AESKEYLEN) Enumerations
 *  ========================================================================= */
typedef enum
{
  CRYPT_CFG_AESKEYLEN128 = 0,  /**< Uses 128-bit long key */
  CRYPT_CFG_AESKEYLEN256 = 2   /**< Uses 256-bit long key */
} ADI_CRYPT_CFG_AESKEYLEN;


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
      unsigned int BLKEN      :  1;  /**< Enable Bit for Crypto Block */
      unsigned int ENCR       :  1;  /**< Encrypt or Decrypt */
      unsigned int INDMAEN    :  1;  /**< Enable DMA Channel Request for Input Buffer */
      unsigned int OUTDMAEN   :  1;  /**< Enable DMA Channel Request for Output Buffer */
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
      unsigned int VALUE      : 32;  /**< Word 0: Nonce Bits [31:0] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_NONCE0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE0_t__ */

/*@}*/

/** @defgroup NONCE1 Nonce Bits [63:32] (NONCE1) Register
 *  Nonce Bits [63:32] (NONCE1) Register.
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
      unsigned int VALUE      : 32;  /**< Word 1: Nonce Bits [63:32] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_NONCE1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE1_t__ */

/*@}*/

/** @defgroup NONCE2 Nonce Bits [95:64] (NONCE2) Register
 *  Nonce Bits [95:64] (NONCE2) Register.
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
      unsigned int VALUE      : 32;  /**< Word 2: Nonce Bits [95:64] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_NONCE2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE2_t__ */

/*@}*/

/** @defgroup NONCE3 Nonce Bits [127:96] (NONCE3) Register
 *  Nonce Bits [127:96] (NONCE3) Register.
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
      unsigned int VALUE      : 32;  /**< Word 3: Nonce Bits [127:96] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_NONCE3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_NONCE3_t__ */

/*@}*/

/** @defgroup AESKEY0 AES Key Bits [31:0] (AESKEY0) Register
 *  AES Key Bits [31:0] (AESKEY0) Register.
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
      unsigned int KUW0       : 32;  /**< KUW [31:0] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW0_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW0_t__ */

/*@}*/

/** @defgroup KUW1 Key Wrap Unwrap Register 1 (KUW1) Register
 *  Key Wrap Unwrap Register 1 (KUW1) Register.
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
      unsigned int KUW1       : 32;  /**< KUW [63:32] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW1_t__ */

/*@}*/

/** @defgroup KUW2 Key Wrap Unwrap Register 2 (KUW2) Register
 *  Key Wrap Unwrap Register 2 (KUW2) Register.
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
      unsigned int KUW2       : 32;  /**< KUW [95:64] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW2_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW2_t__ */

/*@}*/

/** @defgroup KUW3 Key Wrap Unwrap Register 3 (KUW3) Register
 *  Key Wrap Unwrap Register 3 (KUW3) Register.
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
      unsigned int KUW3       : 32;  /**< KUW [127:96] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW3_t__ */

/*@}*/

/** @defgroup KUW4 Key Wrap Unwrap Register 4 (KUW4) Register
 *  Key Wrap Unwrap Register 4 (KUW4) Register.
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
      unsigned int KUW4       : 32;  /**< KUW [159:128] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW4_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW4_t__ */

/*@}*/

/** @defgroup KUW5 Key Wrap Unwrap Register 5 (KUW5) Register
 *  Key Wrap Unwrap Register 5 (KUW5) Register.
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
      unsigned int KUW5       : 32;  /**< KUW [191:160] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW5_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW5_t__ */

/*@}*/

/** @defgroup KUW6 Key Wrap Unwrap Register 6 (KUW6) Register
 *  Key Wrap Unwrap Register 6 (KUW6) Register.
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
      unsigned int KUW6       : 32;  /**< KUW [223:192] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW6_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW6_t__ */

/*@}*/

/** @defgroup KUW7 Key Wrap Unwrap Register 7 (KUW7) Register
 *  Key Wrap Unwrap Register 7 (KUW7) Register.
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
      unsigned int KUW7       : 32;  /**< KUW [255:224] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW7_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW7_t__ */

/*@}*/

/** @defgroup KUW8 Key Wrap Unwrap Register 8 (KUW8) Register
 *  Key Wrap Unwrap Register 8 (KUW8) Register.
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
      unsigned int KUW8       : 32;  /**< KUW [287:256] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW8_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW8_t__ */

/*@}*/

/** @defgroup KUW9 Key Wrap Unwrap Register 9 (KUW9) Register
 *  Key Wrap Unwrap Register 9 (KUW9) Register.
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
      unsigned int KUW9       : 32;  /**< KUW [319:288] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW9_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW9_t__ */

/*@}*/

/** @defgroup KUW10 Key Wrap Unwrap Register 10 (KUW10) Register
 *  Key Wrap Unwrap Register 10 (KUW10) Register.
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
      unsigned int KUW10      : 32;  /**< KUW [351:320] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW10_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW10_t__ */

/*@}*/

/** @defgroup KUW11 Key Wrap Unwrap Register 11 (KUW11) Register
 *  Key Wrap Unwrap Register 11 (KUW11) Register.
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
      unsigned int KUW11      : 32;  /**< KUW [383:352] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW11_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW11_t__ */

/*@}*/

/** @defgroup KUW12 Key Wrap Unwrap Register 12 (KUW12) Register
 *  Key Wrap Unwrap Register 12 (KUW12) Register.
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
      unsigned int KUW12      : 32;  /**< KUW [415:384] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW12_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW12_t__ */

/*@}*/

/** @defgroup KUW13 Key Wrap Unwrap Register 13 (KUW13) Register
 *  Key Wrap Unwrap Register 13 (KUW13) Register.
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
      unsigned int KUW13      : 32;  /**< KUW [447:416] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW13_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW13_t__ */

/*@}*/

/** @defgroup KUW14 Key Wrap Unwrap Register 14 (KUW14) Register
 *  Key Wrap Unwrap Register 14 (KUW14) Register.
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
      unsigned int KUW14      : 32;  /**< KUW [479:448] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW14_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW14_t__ */

/*@}*/

/** @defgroup KUW15 Key Wrap Unwrap Register 15 (KUW15) Register
 *  Key Wrap Unwrap Register 15 (KUW15) Register.
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
      unsigned int KUW15      : 32;  /**< KUW [511:480] */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_KUW15_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_KUW15_t__ */

/*@}*/

/** @defgroup KUWValStr1 Key Wrap Unwrap Validation String [63:32] (KUWValStr1) Register
 *  Key Wrap Unwrap Validation String [63:32] (KUWValStr1) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUWValStr1_Struct
 *! \brief  Key Wrap Unwrap Validation String [63:32] Register bit field structure
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

/** @defgroup KUWValStr2 Key Wrap Unwrap Validation String [31:0] (KUWValStr2) Register
 *  Key Wrap Unwrap Validation String [31:0] (KUWValStr2) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CRYPT_KUWValStr2_Struct
 *! \brief  Key Wrap Unwrap Validation String [31:0] Register bit field structure
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
  PMG_IEN_NA      = 3   /**< NA                                                 */
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
      unsigned int VREGUNDR   :  1;  /**< Enable Interrupt when VREG under-voltage (below 1 V) */
      unsigned int VREGOVR    :  1;  /**< Enable Interrupt when VREG over-voltage (above 1.32 V) */
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

/** @defgroup PSM_STAT Power Supply Monitor Status (PSM_STAT) Register
 *  Power Supply Monitor Status (PSM_STAT) Register.
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
 *! \brief  Power Supply Monitor Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_PSM_STAT_t__
typedef struct _ADI_PMG_PSM_STAT_t {
  union {
    struct {
      unsigned int VBATUNDR   :  1;  /**< Status bit indicating an Alarm that battery is below 1.8 V */
      unsigned int VREGUNDR   :  1;  /**< Status bit for Alarm indicating VREG is below 1 V */
      unsigned int VREGOVR    :  1;  /**< Status bit for alarm indicating Over Voltage for VREG */
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
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_PMG_PWRMOD_MODE
 *! \brief  Power Mode Bits (MODE) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_PWRMOD_FLEXI     = 0,  /**< Flexi Mode     */
  PMG_PWRMOD_HIBERNATE = 2,  /**< Hibernate Mode */
  PMG_PWRMOD_SHUTDOWN  = 3   /**< Shutdown Mode  */
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
      unsigned int MONVBATN   :  1;  /**< Monitor VBAT during Hibernate Mode. Monitors VBAT by default */
      unsigned int reserved4  : 28;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_PWRMOD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_PWRMOD_t__ */

/*@}*/

/** @defgroup PWRKEY Key Protection for PWRMOD and  SRAMRET (PWRKEY) Register
 *  Key Protection for PWRMOD and  SRAMRET (PWRKEY) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_PWRKEY_Struct
 *! \brief  Key Protection for PWRMOD and  SRAMRET Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_PWRKEY_t__
typedef struct _ADI_PMG_PWRKEY_t {
  union {
    struct {
      unsigned int VALUE      : 16;  /**< Power Control Key */
      unsigned int reserved16 : 16;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_PWRKEY_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_PWRKEY_t__ */

/*@}*/

/** @defgroup SHDN_STAT Shutdown Status Register (SHDN_STAT) Register
 *  Shutdown Status Register (SHDN_STAT) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_SHDN_STAT_Struct
 *! \brief  Shutdown Status Register bit field structure
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

/** @defgroup SRAMRET Control for Retention SRAM in Hibernate Mode (SRAMRET) Register
 *  Control for Retention SRAM in Hibernate Mode (SRAMRET) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_SRAMRET_Struct
 *! \brief  Control for Retention SRAM in Hibernate Mode Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_SRAMRET_t__
typedef struct _ADI_PMG_SRAMRET_t {
  union {
    struct {
      unsigned int RET1       :  1;  /**< Enable retention bank 1 (12 KB) */
      unsigned int RET2       :  1;  /**< Enable retention bank 3 and bank 4 (32 KB) */
      unsigned int reserved2  :  6;
      unsigned int RET3       :  1;  /**< Enable retention bank 5 (32 KB) */
      unsigned int RET4       :  1;  /**< Enable retention bank 6 and bank 7 (32 KB) */
      unsigned int reserved10               : 13;
      unsigned int HIBERNATE_SRAM_LOAD_MODE :  1;  /**< Hibernate mode SRAM load mode control */
      unsigned int reserved24               :  8;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_SRAMRET_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_SRAMRET_t__ */

/*@}*/

/** @defgroup TRIM Trimming Bits (TRIM) Register
 *  Trimming Bits (TRIM) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_PMG_TRIM_hibernate_load_mode
 *! \brief  Hibernate mode load mode control (hibernate_load_mode) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_TRIM_HIGH_LOAD = 0,  /**< High hibernate load */
  PMG_TRIM_LOW_LOAD  = 7   /**< Low hibernate load  */
} ADI_PMG_TRIM_hibernate_load_mode;


/* ==========================================================================
 *! \struct ADI_PMG_TRIM_Struct
 *! \brief  Trimming Bits Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TRIM_t__
typedef struct _ADI_PMG_TRIM_t {
  union {
    struct {
      unsigned int reserved0           : 29;
      unsigned int hibernate_load_mode :  3;  /**< Hibernate mode load mode control */
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TRIM_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TRIM_t__ */

/*@}*/

/** @defgroup RST_STAT Reset Status (RST_STAT) Register
 *  Reset Status (RST_STAT) Register.
 *  @{
 */

/*  =========================================================================
 *! \enum   ADI_PMG_RST_STAT_PORSRC
 *! \brief  Power on reset Source (PORSRC) Enumerations
 *  ========================================================================= */
typedef enum
{
  PMG_RST_STAT_FAILSAFE_HV = 0,  /**< POR triggered because VBAT drops below Fail Safe */
  PMG_RST_STAT_RST_VBAT    = 1,  /**< POR trigger because VBAT supply (VBAT < 1.7 V)   */
  PMG_RST_STAT_RST_VREG    = 2,  /**< POR triggered because VDD supply (VDD < 1.08 V)  */
  PMG_RST_STAT_FAILSAFE_LV = 3   /**< POR triggered because VREG drops below Fail Safe */
} ADI_PMG_RST_STAT_PORSRC;


/* ==========================================================================
 *! \struct ADI_PMG_RST_STAT_Struct
 *! \brief  Reset Status Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_RST_STAT_t__
typedef struct _ADI_PMG_RST_STAT_t {
  union {
    struct {
      unsigned int POR        :  1;  /**< Power-on reset */
      unsigned int EXTRST     :  1;  /**< External reset */
      unsigned int WDRST      :  1;  /**< Watchdog timeout */
      unsigned int SWRST      :  1;  /**< Software reset */
      unsigned int PORSRC     :  2;  /**< Power on reset Source */
      unsigned int reserved6  : 26;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_RST_STAT_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_RST_STAT_t__ */

/*@}*/

/** @defgroup CTL1 HPBUCK Control (CTL1) Register
 *  HPBUCK Control (CTL1) Register.
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
  PMG_CTL1_LOWPWRDISABLE = 0,  /**< HPBUCK Low power mode is disabled */
  PMG_CTL1_LOWPWRENABLE  = 1   /**< HPBUCK Low power mode is enabled  */
} ADI_PMG_CTL1_HPBUCK_LOWPWR_MODE;


/* ==========================================================================
 *! \struct ADI_PMG_CTL1_Struct
 *! \brief  HPBUCK Control Register bit field structure
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
      unsigned int LFCLK_MUX  :  1;  /**< 32 kHz clock select mux */
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
      unsigned int LFX_AUTSW_EN :  1;  /**< Enables automatic Switching of the LF Mux to LFOSC on LFXTAL Failure */
      unsigned int LFX_AUTSW_STA :  1;  /**< Status of automatic switching of the LF Mux to LFOSC upon detection of LFXTAL failure */
      unsigned int LFX_ROBUST_EN :  1;  /**< LFXTAL Mode select */
      unsigned int LFX_ROBUST_LD :  2;  /**< LFXTAL Robust Mode Load select */
      unsigned int reserved17    :  3;
      unsigned int ROOT_MON_EN   :  1;  /**< ROOT clock monitor and Clock FAIL interrupt enable */
      unsigned int ROOT_AUTSW_EN :  1;  /**< Enables automatic Switching of the Root clock  to HFOSC on Root clock Failure */
      unsigned int ROOT_AUTSW_STA :  1;  /**< Status of automatic switching of the Root clock to HFOSC upon detection of Root clock failure */
      unsigned int reserved23     :  7;
      unsigned int ROOT_FAIL_STA  :  1;  /**< Root clock (crystal clock) Not Stable */
      unsigned int LFX_FAIL_STA   :  1;  /**< LF XTAL (crystal clock) Not Stable */
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_OSC_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_OSC_CTL_t__ */

/*@}*/

/** @defgroup SRAM_CTL Control for SRAM Parity and Instruction SRAM (SRAM_CTL) Register
 *  Control for SRAM Parity and Instruction SRAM (SRAM_CTL) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_SRAM_CTL_Struct
 *! \brief  Control for SRAM Parity and Instruction SRAM Register bit field structure
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
      unsigned int AUTOINIT   :  1;  /**< Automatic initialization on wake up from hibernate mode */
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
      unsigned int INSTREN    :  1;  /**< Enables 32 KB instruction SRAM */
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TST_SRAM_CTL_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_SRAM_CTL_t__ */

/*@}*/

/** @defgroup SRAM_INITSTAT Initialization Status Register (SRAM_INITSTAT) Register
 *  Initialization Status Register (SRAM_INITSTAT) Register.
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

/** @defgroup CLR_LATCH_GPIOS Clear GPIO After Shutdown Mode (CLR_LATCH_GPIOS) Register
 *  Clear GPIO After Shutdown Mode (CLR_LATCH_GPIOS) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_CLR_LATCH_GPIOS_Struct
 *! \brief  Clear GPIO After Shutdown Mode Register bit field structure
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

/** @defgroup SCRPAD_IMG Scratch Pad Image (SCRPAD_IMG) Register
 *  Scratch Pad Image (SCRPAD_IMG) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_SCRPAD_IMG_Struct
 *! \brief  Scratch Pad Image Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_SCRPAD_IMG_t__
typedef struct _ADI_PMG_TST_SCRPAD_IMG_t {
  union {
    struct {
      unsigned int DATA       : 32;  /**< Value written to this register is saved in 3 V when going to shutdown */
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TST_SCRPAD_IMG_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_SCRPAD_IMG_t__ */

/*@}*/

/** @defgroup SCRPAD_3V_RD Scratch Pad Saved in Battery Domain (SCRPAD_3V_RD) Register
 *  Scratch Pad Saved in Battery Domain (SCRPAD_3V_RD) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_SCRPAD_3V_RD_Struct
 *! \brief  Scratch Pad Saved in Battery Domain Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_SCRPAD_3V_RD_t__
typedef struct _ADI_PMG_TST_SCRPAD_3V_RD_t {
  union {
    struct {
      unsigned int DATA       : 32;  /**< Reading the scratch pad stored in shutdown mode */
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TST_SCRPAD_3V_RD_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_SCRPAD_3V_RD_t__ */

/*@}*/

/** @defgroup FAST_SHT_WAKEUP Fast Shutdown Wake-up Enable (FAST_SHT_WAKEUP) Register
 *  Fast Shutdown Wake-up Enable (FAST_SHT_WAKEUP) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_PMG_TST_FAST_SHT_WAKEUP_Struct
 *! \brief  Fast Shutdown Wake-up Enable Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_FAST_SHT_WAKEUP_t__
typedef struct _ADI_PMG_TST_FAST_SHT_WAKEUP_t {
  union {
    struct {
      unsigned int FAST_SHT_WAKEUP :  1;  /**< Enables fast shutdown wake-up */
      unsigned int reserved1       : 31;
    };
    uint32_t VALUE32;
  };
} ADI_PMG_TST_FAST_SHT_WAKEUP_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_FAST_SHT_WAKEUP_t__ */

/*@}*/

/** @defgroup CTL0 Misc Clock Settings (CTL0) Register
 *  Misc Clock Settings (CTL0) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_CTL0_Struct
 *! \brief  Misc Clock Settings Register bit field structure
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

/** @defgroup CTL1 Clock Dividers (CTL1) Register
 *  Clock Dividers (CTL1) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_CTL1_Struct
 *! \brief  Clock Dividers Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL1_t__
typedef struct _ADI_CLKG_CLK_CTL1_t {
  union {
    struct {
      unsigned int HCLKDIVCNT :  6;  /**< HCLK divide count */
      unsigned int reserved6  :  2;
      unsigned int PCLKDIVCNT :  6;  /**< PCLK divide count */
      unsigned int reserved14 :  2;
      unsigned int ACLKDIVCNT :  9;  /**< ACLK Divide Count */
      unsigned int reserved25 :  7;
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_CLK_CTL1_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL1_t__ */

/*@}*/

/** @defgroup CTL2 HF Oscillator Divided Clock Select (CTL2) Register
 *  HF Oscillator Divided Clock Select (CTL2) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_CTL2_Struct
 *! \brief  HF Oscillator Divided Clock Select Register bit field structure
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
      unsigned int SPLLMUL2   :  1;  /**< System PLL multiply by 2 */
      unsigned int reserved17 : 15;
    };
    uint32_t VALUE32;
  };
} ADI_CLKG_CLK_CTL3_t;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL3_t__ */

/*@}*/

/** @defgroup CTL5 User Clock Gating Control (CTL5) Register
 *  User Clock Gating Control (CTL5) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_CTL5_Struct
 *! \brief  User Clock Gating Control Register bit field structure
 * ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_CTL5_t__
typedef struct _ADI_CLKG_CLK_CTL5_t {
  union {
    struct {
      unsigned int GPTCLK0OFF :  1;  /**< GP Timer 0 user control */
      unsigned int GPTCLK1OFF :  1;  /**< GP Timer 1 user control */
      unsigned int GPTCLK2OFF :  1;  /**< GP Timer 2 user control */
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

/** @defgroup STAT0 Clocking Status (STAT0) Register
 *  Clocking Status (STAT0) Register.
 *  @{
 */

/* ==========================================================================
 *! \struct ADI_CLKG_CLK_STAT0_Struct
 *! \brief  Clocking Status Register bit field structure
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
