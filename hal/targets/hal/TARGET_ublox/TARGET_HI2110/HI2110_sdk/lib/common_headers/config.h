#ifndef CONFIG_H
#define CONFIG_H

/********************************************************************
 *
 *  File:          /home/bath/neul/gsm/include/SCCS/s.config.h
 *  -----
 *
 *  SCCS Version:  2.32
 *  -------------
 *
 *  Copyright Statement :
 *  ------------------------
 *  This software is protected by Copyright and the
 *  information contained herein is confidential.
 *  The software may not be copied and the information
 *  contained herein may not be used or disclosed
 *  except with the written permission of Communications
 *  Consultants Worldwide Limited (C) 1997-2008
 *
 *  Project:       P/60 - Generic GSM Handset Protocol Stack
 *  --------
 *
 *  Description:
 *  -----------
 *
 *
 *  Functions Used:
 *  ---------------
 *
 *  Comment:
 *  --------
 *
 ******************************************************************************/

/********************************************/
/* Define the SVN (Software Version Number) */
/********************************************/

#define SVN 01

/************************************************/
/* Define the MS capability in term of channels */
/************************************************/

#define DUAL_RATE_SUPPORTED 0
#define FULL_RATE_ONLY      1
#define SDCCH_ONLY          2

/***********************************************/
/* Define the MT SMS point to point capability */
/***********************************************/

#define PP_SMS_NOT_SUPPORTED                    0
#define PP_SMS_SUPPORTED_VIA_DEDICATED          1
#define PP_SMS_SUPPORTED_VIA_GPRS               2
#define PP_SMS_SUPPORTED_VIA_DEDICATED_AND_GPRS 3

/************************************************/
/* Define the Pseudo Synchronisation capability */
/************************************************/

#define PS_CAPABILITY_NOT_PRESENT 0
#define PS_CAPABILITY_PRESENT     1

/******************/
/* Revision Level */
/******************/

#define PHASE_1 0
#define PHASE_2 1
#define R99_ON  2

/**********************************************/
/* Controlled Early Classmark Sending support */
/**********************************************/

#define CECS_NOT_IMPLEMENTED     0
#define CECS_IMPLEMENTED         1

/**********************************************
 * UCS2 Support
 *
 * Reference: 24.008 section 10.5.5.12
 **********************************************/
#define DEFAULT_ALPHABET_PREFERRED FALSE
#define NO_PREFERENCE              TRUE

/**********************************************
 * SoLSA Capability
 *
 * Reference: 24.008 section 10.5.5.12
 **********************************************/
#define SOLSA_NOT_SUPPORTED FALSE
#define SOLSA_SUPPORTED     TRUE

/**********************************************
 * PCF Feature Mode
 *
 * Reference: 24.008 section 10.5.5.12
 **********************************************/
#define BSS_PACKET_FLOW_PROCEDURES_NOT_SUPPORTED FALSE
#define BSS_PACKET_FLOW_PROCEDURES_SUPPORTED     TRUE

/**********************************************
 * Voice Group Call Service (VGCS)
 *
 * Reference: 24.008 section 10.5.5.12a
 **********************************************/
#define NO_VGCS_CAPABILITY_NO_NOTIFICATIONS_WANTED 0
#define VGCS_CAPABILITY_AND_NOTIFICATIONS_WANTED   1

/**********************************************
 * Voice Broadcast Service (VBS)
 *
 * Reference: 24.008 section 10.5.5.12a
 **********************************************/
#define NO_VBS_CAPABILITY_NO_NOTIFICATIONS_WANTED 0
#define VBS_CAPABILITY_AND_NOTIFICATIONS_WANTED   1

/**********************************************
 * Multislot Capability
 *
 * Reference: 24.008 section 10.5.5.12a
 **********************************************/
#define MULTISLOT_NOT_SUPPORTED 0
#define MULTISLOT_SUPPORTED     1

#define SMS_VALUE_1_4         0x0
#define SMS_VALUE_2_4         0x1
#define SMS_VALUE_3_4         0x2
#define SMS_VALUE_4_4         0x3
#define SMS_VALUE_5_4         0x4
#define SMS_VALUE_6_4         0x5
#define SMS_VALUE_7_4         0x6
#define SMS_VALUE_8_4         0x7
#define SMS_VALUE_9_4         0x8
#define SMS_VALUE_10_4        0x9
#define SMS_VALUE_11_4        0xa
#define SMS_VALUE_12_4        0xb
#define SMS_VALUE_13_4        0xc
#define SMS_VALUE_14_4        0xd
#define SMS_VALUE_15_4        0xe
#define SMS_VALUE_16_4        0xf

#define SM_VALUE_1_4          0x0
#define SM_VALUE_2_4          0x1
#define SM_VALUE_3_4          0x2
#define SM_VALUE_4_4          0x3
#define SM_VALUE_5_4          0x4
#define SM_VALUE_6_4          0x5
#define SM_VALUE_7_4          0x6
#define SM_VALUE_8_4          0x7
#define SM_VALUE_9_4          0x8
#define SM_VALUE_10_4         0x9
#define SM_VALUE_11_4         0xa
#define SM_VALUE_12_4         0xb
#define SM_VALUE_13_4         0xc
#define SM_VALUE_14_4         0xd
#define SM_VALUE_15_4         0xe
#define SM_VALUE_16_4         0xf


/**********************************************
 * 8PSK Power Capability
 *
 * Reference: 24.008 section 10.5.5.12a
 **********************************************/
#define EIGHTPSK_NOT_SUPPORTED 0
#define EIGHTPSK_SUPPORTED     1

/**********************************************
 * COMPACT Interference Measurement Capability
 *
 * Reference: 24.008 section 10.5.5.12a
 **********************************************/
#define NO_COMPACT_INTERFERENCE_MEASUREMENT_CAPABILITY 0
#define COMPACT_INTERFERENCE_MEASUREMENT_CAPABILITY    1

/**********************************************
 * UMTS FDD Radio Access Technology Capability
 *
 * Reference: 24.008 section 10.5.5.12a
 **********************************************/
#define UMTS_FDD_NOT_SUPPORTED 0
#define UMTS_FDD_SUPPORTED     1

/**********************************************
 * UMTS TDD Radio Access Technology Capability
 *
 * Reference: 24.008 section 10.5.5.12a
 **********************************************/
#define UMTS_TDD_NOT_SUPPORTED 0
#define UMTS_TDD_SUPPORTED     1

/**********************************************
 * CDMA 2000 Radio Access Technology Capability
 *
 * Reference: 24.008 section 10.5.5.12a
 **********************************************/
#define CDMA2000_NOT_SUPPORTED 0
#define CDMA2000_SUPPORTED     1

/**********************************************/
/* class revert mode                          */
/**********************************************/
typedef enum class_revert_mode_tag
{
   REVERT_MODE_REMAIN_CLASS_B,
   REVERT_MODE_REVERT_CLASS_CG,
   REVERT_MODE_REVERT_CLASS_CC
} class_revert_mode_T;


#endif

