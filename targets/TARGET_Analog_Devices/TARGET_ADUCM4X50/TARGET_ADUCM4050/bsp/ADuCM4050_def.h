/*
** ADuCM4050_def.h
**
** Copyright (C) 2016 Analog Devices, Inc. All Rights Reserved.
**
*/

#ifndef _WRAP_ADUCM4050_DEF_H
#define _WRAP_ADUCM4050_DEF_H

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions:
 *
 * Pm008 (rule 2.4): sections of code should not be 'commented out'.
 *   Some comments are wrongly identified as code.
 *
 * Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
 *   The YODA-generated headers rely on more. The IAR compiler supports that.
 */
_Pragma("diag_suppress=Pm008,Pm009")
#endif /* __ICCARM__ */

#ifdef __IASMARM__
/* Define masks to plain numeric literal for IAR assembler. */
#define _ADI_MSK_3( mask, smask, type ) (mask)
#endif /* __IASMARM__ */

#include <sys/adi_ADuCM4050.h>

/* Workarounds for MSKEW-180. Field names were shortened.
*/

#define BITP_CLKG_OSC_CTL_LFXTAL_MON_FAIL_STAT       (BITP_CLKG_OSC_CTL_LFX_FAIL_STA)   /* LF XTAL (crystal clock) Not Stable */
#define BITP_CLKG_OSC_CTL_ROOTCLK_MON_FAIL_STAT      (BITP_CLKG_OSC_CTL_ROOT_FAIL_STA)  /* Root clock (crystal clock) Not Stable */
#define BITP_CLKG_OSC_CTL_ROOTCLKFAIL_AUTOSWT_STAT   (BITP_CLKG_OSC_CTL_ROOT_AUTSW_STA) /* Status of automatic switching of the Root clock to HF SOC upon detection of Root clock failure */
#define BITP_CLKG_OSC_CTL_ROOTCLKFAIL_AUTOSWT_EN     (BITP_CLKG_OSC_CTL_ROOT_AUTSW_EN)  /* Enables automatic Switching of the Root clock to HFOSC on Root clock Failure */
#define BITP_CLKG_OSC_CTL_ROOTCLK_MON_EN             (BITP_CLKG_OSC_CTL_ROOT_MON_EN)    /* Root clock monitor and Clock FAIL interrupt enable */
#define BITP_CLKG_OSC_CTL_XO32K_ROBUST_MODE_LOAD_SEL (BITP_CLKG_OSC_CTL_LFX_ROBUST_LD)  /* LFXTAL Robust Mode Load Select */
#define BITP_CLKG_OSC_CTL_XO32K_ROBUST_MODE_ENABLE   (BITP_CLKG_OSC_CTL_LFX_ROBUST_EN)  /* LFXTAL Mode select */
#define BITP_CLKG_OSC_CTL_LFXTALFAIL_AUTOSWT_STAT    (BITP_CLKG_OSC_CTL_LFX_AUTSW_STA)  /* Status of automatic switching of the LF Mux to LF OSC upon detection of LF XTAL failure */
#define BITP_CLKG_OSC_CTL_LFXTALFAIL_AUTOSWT_EN      (BITP_CLKG_OSC_CTL_LFX_AUTSW_EN)   /* Enables automatic Switching of the LF Mux to LF OSC on LF XTAL Failure */
#define BITP_CLKG_OSC_CTL_HFXTALOK                   (BITP_CLKG_OSC_CTL_HFX_OK)         /* Status of HFXTAL oscillator */
#define BITP_CLKG_OSC_CTL_LFXTALOK                   (BITP_CLKG_OSC_CTL_LFX_OK)         /* Status of LFXTAL oscillator */
#define BITP_CLKG_OSC_CTL_HFOSCOK                    (BITP_CLKG_OSC_CTL_HFOSC_OK)       /* Status of HFOSC oscillator */
#define BITP_CLKG_OSC_CTL_LFOSCOK                    (BITP_CLKG_OSC_CTL_LFOSC_OK)       /* Status of LFOSC oscillator */
#define BITP_CLKG_OSC_CTL_LFXTAL_MON_EN              (BITP_CLKG_OSC_CTL_LFX_MON_EN)     /* LFXTAL clock monitor and Clock FAIL interrupt enable */
#define BITP_CLKG_OSC_CTL_LFXTAL_BYPASS              (BITP_CLKG_OSC_CTL_LFX_BYP)        /* Low frequency crystal oscillator Bypass */
#define BITP_CLKG_OSC_CTL_HFXTALEN                   (BITP_CLKG_OSC_CTL_HFX_EN)         /* High frequency crystal oscillator enable */
#define BITP_CLKG_OSC_CTL_LFXTALEN                   (BITP_CLKG_OSC_CTL_LFX_EN)         /* Low frequency crystal oscillator enable */
#define BITP_CLKG_OSC_CTL_HFOSCEN                    (BITP_CLKG_OSC_CTL_HFOSC_EN)       /* High frequency internal oscillator enable */
#define BITP_CLKG_OSC_CTL_LFCLKMUX                   (BITP_CLKG_OSC_CTL_LFCLK_MUX)      /* 32 KHz clock select mux */

#define BITM_CLKG_OSC_CTL_LFXTAL_MON_FAIL_STAT       (BITM_CLKG_OSC_CTL_LFX_FAIL_STA)   /* LF XTAL (crystal clock) Not Stable */
#define BITM_CLKG_OSC_CTL_ROOTCLK_MON_FAIL_STAT      (BITM_CLKG_OSC_CTL_ROOT_FAIL_STA)  /* Root clock (crystal clock) Not Stable */
#define BITM_CLKG_OSC_CTL_ROOTCLKFAIL_AUTOSWT_STAT   (BITM_CLKG_OSC_CTL_ROOT_AUTSW_STA) /* Status of automatic switching of the Root clock to HF SOC upon detection of Root clock failure */
#define BITM_CLKG_OSC_CTL_ROOTCLKFAIL_AUTOSWT_EN     (BITM_CLKG_OSC_CTL_ROOT_AUTSW_EN)  /* Enables automatic Switching of the Root clock to HFOSC on Root clock Failure */
#define BITM_CLKG_OSC_CTL_ROOTCLK_MON_EN             (BITM_CLKG_OSC_CTL_ROOT_MON_EN)    /* Root clock monitor and Clock FAIL interrupt enable */
#define BITM_CLKG_OSC_CTL_XO32K_ROBUST_MODE_LOAD_SEL (BITM_CLKG_OSC_CTL_LFX_ROBUST_LD)  /* LFXTAL Robust Mode Load Select */
#define BITM_CLKG_OSC_CTL_XO32K_ROBUST_MODE_ENABLE   (BITM_CLKG_OSC_CTL_LFX_ROBUST_EN)  /* LFXTAL Mode select */
#define BITM_CLKG_OSC_CTL_LFXTALFAIL_AUTOSWT_STAT    (BITM_CLKG_OSC_CTL_LFX_AUTSW_STA)  /* Status of automatic switching of the LF Mux to LF OSC upon detection of LF XTAL failure */
#define BITM_CLKG_OSC_CTL_LFXTALFAIL_AUTOSWT_EN      (BITM_CLKG_OSC_CTL_LFX_AUTSW_EN)   /* Enables automatic Switching of the LF Mux to LF OSC on LF XTAL Failure */
#define BITM_CLKG_OSC_CTL_HFXTALOK                   (BITM_CLKG_OSC_CTL_HFX_OK)         /* Status of HFXTAL oscillator */
#define BITM_CLKG_OSC_CTL_LFXTALOK                   (BITM_CLKG_OSC_CTL_LFX_OK)         /* Status of LFXTAL oscillator */
#define BITM_CLKG_OSC_CTL_HFOSCOK                    (BITM_CLKG_OSC_CTL_HFOSC_OK)       /* Status of HFOSC oscillator */
#define BITM_CLKG_OSC_CTL_LFOSCOK                    (BITM_CLKG_OSC_CTL_LFOSC_OK)       /* Status of LFOSC oscillator */
#define BITM_CLKG_OSC_CTL_LFXTAL_MON_EN              (BITM_CLKG_OSC_CTL_LFX_MON_EN)     /* LFXTAL clock monitor and Clock FAIL interrupt enable */
#define BITM_CLKG_OSC_CTL_LFXTAL_BYPASS              (BITM_CLKG_OSC_CTL_LFX_BYP)        /* Low frequency crystal oscillator Bypass */
#define BITM_CLKG_OSC_CTL_HFXTALEN                   (BITM_CLKG_OSC_CTL_HFX_EN)         /* High frequency crystal oscillator enable */
#define BITM_CLKG_OSC_CTL_LFXTALEN                   (BITM_CLKG_OSC_CTL_LFX_EN)         /* Low frequency crystal oscillator enable */
#define BITM_CLKG_OSC_CTL_HFOSCEN                    (BITM_CLKG_OSC_CTL_HFOSC_EN)       /* High frequency internal oscillator enable */
#define BITM_CLKG_OSC_CTL_LFCLKMUX                   (BITM_CLKG_OSC_CTL_LFCLK_MUX)      /* 32 KHz clock select mux */


#ifdef __ICCARM__
_Pragma("diag_default=Pm008,Pm009")
#endif /* __ICCARM__ */

#endif /* _WRAP_ADUCM4050_DEF_H */
