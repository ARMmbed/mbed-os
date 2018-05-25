#ifndef INCLUDED_SECURITY
#define INCLUDED_SECURITY
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** 
*/
struct security_s {
   /** An 8 bit identifier that is used to help to calculate the response. at address offset 0x000, read-write */
   uint32_t sec_id;
   /** Allows the status of the controller to be read back. at address offset 0x004, read-only */
   uint32_t sec_status;
   /**  at address offset 0x008, read-write */
   uint32_t sec_cfg;
   /** Reserved space */
   uint8_t fill0[500];
   /** Challenge buffer. Writable via APB, Readable via JTAG. at address offset 0x200, write-only */
   uint32_t sec_challenge[128];
   /** Response buffer. Writable via APB. Writable via JTAG. at address offset 0x400, write-only */
   uint32_t sec_response[128];
   /** Reserved space */
   uint8_t fill1[2512];
   /** PrimeCell Peripheral ID 4 at address offset 0xFD0, read-only */
   uint32_t peripheralid4;
   /** PrimeCell Peripheral ID 5 at address offset 0xFD4, read-only */
   uint32_t peripheralid5;
   /** PrimeCell Peripheral ID 6 at address offset 0xFD8, read-only */
   uint32_t peripheralid6;
   /** PrimeCell Peripheral ID 7 at address offset 0xFDC, read-only */
   uint32_t peripheralid7;
   /** PrimeCell Peripheral ID 0 at address offset 0xFE0, read-only */
   uint32_t peripheralid0;
   /** PrimeCell Peripheral ID 1 at address offset 0xFE4, read-only */
   uint32_t peripheralid1;
   /** PrimeCell Peripheral ID 2 at address offset 0xFE8, read-only */
   uint32_t peripheralid2;
   /** PrimeCell Peripheral ID 3 at address offset 0xFEC, read-only */
   uint32_t peripheralid3;
   /** PrimeCell Component ID 0 at address offset 0xFF0, read-only */
   uint32_t componentid0;
   /** PrimeCell Component ID 1 at address offset 0xFF4, read-only */
   uint32_t componentid1;
   /** PrimeCell Component ID 2 at address offset 0xFF8, read-only */
   uint32_t componentid2;
   /** PrimeCell Component ID 3 at address offset 0xFFC, read-only */
   uint32_t componentid3;
};

/** bit field defines for security_s#sec_id */
#define SECURITY_SEC_ID_DEVICE_ID_OFFSET 0
#define SECURITY_SEC_ID_DEVICE_ID_SIZE 8

/** bit field defines for security_s#sec_status */
#define SECURITY_SEC_STATUS_SEC_STAT_KEY_LEN_OFFSET 0
#define SECURITY_SEC_STATUS_SEC_STAT_KEY_LEN_SIZE 4
#define SECURITY_SEC_STATUS_SEC_STAT_LOCKED_OFFSET 8
#define SECURITY_SEC_STATUS_SEC_STAT_LOCKED_SIZE 1
#define SECURITY_SEC_STATUS_SEC_STAT_UNLOCKED_OFFSET 9
#define SECURITY_SEC_STATUS_SEC_STAT_UNLOCKED_SIZE 1
#define SECURITY_SEC_STATUS_SEC_STAT_KEYS_STORED_OFFSET 31
#define SECURITY_SEC_STATUS_SEC_STAT_KEYS_STORED_SIZE 1

/** bit field defines for security_s#sec_cfg */
#define SECURITY_SEC_CFG_SEC_CFG_KEY_LEN_OFFSET 0
#define SECURITY_SEC_CFG_SEC_CFG_KEY_LEN_SIZE 4
#define SECURITY_SEC_CFG_SEC_CFG_LOCKED_OFFSET 8
#define SECURITY_SEC_CFG_SEC_CFG_LOCKED_SIZE 1
#define SECURITY_SEC_CFG_SEC_CFG_UNLOCKED_OFFSET 9
#define SECURITY_SEC_CFG_SEC_CFG_UNLOCKED_SIZE 1
#define SECURITY_SEC_CFG_SEC_CFG_KEYS_STORED_OFFSET 31
#define SECURITY_SEC_CFG_SEC_CFG_KEYS_STORED_SIZE 1

/** bit field defines for security_s#sec_challenge */
#define SECURITY_SEC_CHALLENGE_SEC_CHALLENGE_OFFSET 0
#define SECURITY_SEC_CHALLENGE_SEC_CHALLENGE_SIZE 32

/** bit field defines for security_s#sec_response */
#define SECURITY_SEC_RESPONSE_SEC_RESPONSE_OFFSET 0
#define SECURITY_SEC_RESPONSE_SEC_RESPONSE_SIZE 32

/** bit field defines for security_s#peripheralid4 */
#define SECURITY_PERIPHERALID4_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_PERIPHERALID4_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#peripheralid5 */
#define SECURITY_PERIPHERALID5_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_PERIPHERALID5_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#peripheralid6 */
#define SECURITY_PERIPHERALID6_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_PERIPHERALID6_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#peripheralid7 */
#define SECURITY_PERIPHERALID7_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_PERIPHERALID7_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#peripheralid0 */
#define SECURITY_PERIPHERALID0_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_PERIPHERALID0_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#peripheralid1 */
#define SECURITY_PERIPHERALID1_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_PERIPHERALID1_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#peripheralid2 */
#define SECURITY_PERIPHERALID2_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_PERIPHERALID2_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#peripheralid3 */
#define SECURITY_PERIPHERALID3_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_PERIPHERALID3_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#componentid0 */
#define SECURITY_COMPONENTID0_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_COMPONENTID0_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#componentid1 */
#define SECURITY_COMPONENTID1_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_COMPONENTID1_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#componentid2 */
#define SECURITY_COMPONENTID2_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_COMPONENTID2_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for security_s#componentid3 */
#define SECURITY_COMPONENTID3_PRIMECELLIDENTIFICATION_OFFSET 0
#define SECURITY_COMPONENTID3_PRIMECELLIDENTIFICATION_SIZE 8

/* EOF security.h */
#endif
