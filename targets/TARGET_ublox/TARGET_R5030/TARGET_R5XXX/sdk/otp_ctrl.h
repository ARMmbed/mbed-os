#ifndef INCLUDED_OTP_CTRL
#define INCLUDED_OTP_CTRL
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** Here comes the decription of this IP component
*/
struct otp_ctrl_s {
   /** The current status of the OTP controller at address offset 0x000, read-only */
   uint32_t otp_status;
   /** Instructs the controller to execute a command. at address offset 0x004, write-only */
   uint32_t otp_command_set;
   /** The OTP address which will be used for reads and writes. at address offset 0x008, read-write */
   uint32_t otp_address;
   /** The OTP data that will be written at address offset 0x00C, read-write */
   uint32_t otp_write_data;
   /** Configures internal timers used to sequence reads and writes on the OTP fuses. at address offset 0x010, read-write */
   uint32_t otp_count_step;
   /** Configures internal timers used to control the number of clocks during the OTP fuse burn at address offset 0x014, read-write */
   uint32_t otp_burn_count;
   /** This register indicates if an error has occurred during the last access to the OTP. at address offset 0x018, read-write */
   uint32_t otp_error;
   /** OTP Page read lock control.  Each bit can be set once but not cleared using the register interface. Reset PDS to clear at address offset 0x01C, read-write */
   uint32_t otp_read_lock_1;
   /** OTP Page read lock control.  As OTP_READ_LOCK_1 for additional OTP pages. Reset PDS to clear at address offset 0x020, read-write */
   uint32_t otp_read_lock_2;
   /** IRQ Bit Mask. Set each bit to include in combined IRQ output at address offset 0x024, read-write */
   uint32_t otp_irq_mask;
   /** Reserved space */
   uint8_t fill0[216];
   /** Read back for OTP Shadow Region LS Bytes at address offset 0x100, read-only */
   uint32_t otp_shadow_region_1;
   /** Read back for OTP Shadow Region MS Bytes at address offset 0x104, read-only */
   uint32_t otp_shadow_region_2;
   /** Reserved space */
   uint8_t fill1[3752];
   /**  at address offset 0xFB0, read-write */
   uint32_t otp_write_protect;
   /** Reserved space */
   uint8_t fill2[28];
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
   uint32_t componentid2;
   /** PrimeCell Component ID 2 at address offset 0xFF8, read-only */
   uint32_t componentid3;
   /** PrimeCell Component ID 3 at address offset 0xFFC, read-only */
   uint32_t componentid4;
};

/** bit field defines for otp_ctrl_s#otp_status */
#define OTP_CTRL_OTP_STATUS_OTP_SM_INIT_OFFSET 0
#define OTP_CTRL_OTP_STATUS_OTP_SM_INIT_SIZE 1
#define OTP_CTRL_OTP_STATUS_OTP_SM_STANDBY_OFFSET 1
#define OTP_CTRL_OTP_STATUS_OTP_SM_STANDBY_SIZE 1
#define OTP_CTRL_OTP_STATUS_OTP_SM_READ_OFFSET 2
#define OTP_CTRL_OTP_STATUS_OTP_SM_READ_SIZE 1
#define OTP_CTRL_OTP_STATUS_OTP_SM_WRITE_OFFSET 3
#define OTP_CTRL_OTP_STATUS_OTP_SM_WRITE_SIZE 1
#define OTP_CTRL_OTP_STATUS_OTP_READ_DATA_OFFSET 16
#define OTP_CTRL_OTP_STATUS_OTP_READ_DATA_SIZE 8
#define OTP_CTRL_OTP_STATUS_OTP_WRITE_VERIFY_DATA_OFFSET 24
#define OTP_CTRL_OTP_STATUS_OTP_WRITE_VERIFY_DATA_SIZE 1
#define OTP_CTRL_OTP_STATUS_OTP_LOCK_OFFSET 25
#define OTP_CTRL_OTP_STATUS_OTP_LOCK_SIZE 1
#define OTP_CTRL_OTP_STATUS_OTP_ERROR_FLAG_OFFSET 31
#define OTP_CTRL_OTP_STATUS_OTP_ERROR_FLAG_SIZE 1

/** bit field defines for otp_ctrl_s#otp_command_set */
#define OTP_CTRL_OTP_COMMAND_SET_OTP_SM_INIT_OFFSET 0
#define OTP_CTRL_OTP_COMMAND_SET_OTP_SM_INIT_SIZE 1
#define OTP_CTRL_OTP_COMMAND_SET_OTP_SM_STANDBY_OFFSET 1
#define OTP_CTRL_OTP_COMMAND_SET_OTP_SM_STANDBY_SIZE 1
#define OTP_CTRL_OTP_COMMAND_SET_OTP_SM_READ_OFFSET 2
#define OTP_CTRL_OTP_COMMAND_SET_OTP_SM_READ_SIZE 1
#define OTP_CTRL_OTP_COMMAND_SET_OTP_SM_WRITE_OFFSET 3
#define OTP_CTRL_OTP_COMMAND_SET_OTP_SM_WRITE_SIZE 1

/** bit field defines for otp_ctrl_s#otp_address */
#define OTP_CTRL_OTP_ADDRESS_OTP_ADDRESS_OFFSET 0
#define OTP_CTRL_OTP_ADDRESS_OTP_ADDRESS_SIZE 14

/** bit field defines for otp_ctrl_s#otp_write_data */
#define OTP_CTRL_OTP_WRITE_DATA_OTP_WRITE_DATA_OFFSET 0
#define OTP_CTRL_OTP_WRITE_DATA_OTP_WRITE_DATA_SIZE 1

/** bit field defines for otp_ctrl_s#otp_count_step */
#define OTP_CTRL_OTP_COUNT_STEP_DIVIDE_VALUE_OFFSET 0
#define OTP_CTRL_OTP_COUNT_STEP_DIVIDE_VALUE_SIZE 4

/** bit field defines for otp_ctrl_s#otp_burn_count */
#define OTP_CTRL_OTP_BURN_COUNT_COUNT_VALUE_OFFSET 0
#define OTP_CTRL_OTP_BURN_COUNT_COUNT_VALUE_SIZE 10

/** bit field defines for otp_ctrl_s#otp_error */
#define OTP_CTRL_OTP_ERROR_PAGE_PROTECTED_ERROR_OFFSET 0
#define OTP_CTRL_OTP_ERROR_PAGE_PROTECTED_ERROR_SIZE 1
#define OTP_CTRL_OTP_ERROR_IF_PROTECTED_ERROR_OFFSET 1
#define OTP_CTRL_OTP_ERROR_IF_PROTECTED_ERROR_SIZE 1
#define OTP_CTRL_OTP_ERROR_IF_SLOWCLK_WRITE_ERROR_OFFSET 2
#define OTP_CTRL_OTP_ERROR_IF_SLOWCLK_WRITE_ERROR_SIZE 1
#define OTP_CTRL_OTP_ERROR_OTP_SUP_NOT_OK_OFFSET 3
#define OTP_CTRL_OTP_ERROR_OTP_SUP_NOT_OK_SIZE 1
#define OTP_CTRL_OTP_ERROR_OTP_READ_LOCKED_OFFSET 4
#define OTP_CTRL_OTP_ERROR_OTP_READ_LOCKED_SIZE 1

/** bit field defines for otp_ctrl_s#otp_read_lock_1 */
#define OTP_CTRL_OTP_READ_LOCK_1_READ_LOCK_LSBS_OFFSET 1
#define OTP_CTRL_OTP_READ_LOCK_1_READ_LOCK_LSBS_SIZE 31

/** bit field defines for otp_ctrl_s#otp_read_lock_2 */
#define OTP_CTRL_OTP_READ_LOCK_2_READ_LOCK_MSBS_OFFSET 0
#define OTP_CTRL_OTP_READ_LOCK_2_READ_LOCK_MSBS_SIZE 16

/** bit field defines for otp_ctrl_s#otp_irq_mask */
#define OTP_CTRL_OTP_IRQ_MASK_PAGE_PROTECTED_ERROR_OFFSET 0
#define OTP_CTRL_OTP_IRQ_MASK_PAGE_PROTECTED_ERROR_SIZE 1
#define OTP_CTRL_OTP_IRQ_MASK_IF_PROTECTED_ERROR_OFFSET 1
#define OTP_CTRL_OTP_IRQ_MASK_IF_PROTECTED_ERROR_SIZE 1
#define OTP_CTRL_OTP_IRQ_MASK_IF_SLOWCLK_WRITE_ERROR_OFFSET 2
#define OTP_CTRL_OTP_IRQ_MASK_IF_SLOWCLK_WRITE_ERROR_SIZE 1
#define OTP_CTRL_OTP_IRQ_MASK_OTP_SUP_NOT_OK_OFFSET 3
#define OTP_CTRL_OTP_IRQ_MASK_OTP_SUP_NOT_OK_SIZE 1
#define OTP_CTRL_OTP_IRQ_MASK_OTP_READ_LOCKED_OFFSET 4
#define OTP_CTRL_OTP_IRQ_MASK_OTP_READ_LOCKED_SIZE 1

/** bit field defines for otp_ctrl_s#otp_shadow_region_1 */
#define OTP_CTRL_OTP_SHADOW_REGION_1_OTP_SHADOW_REGION_1_OFFSET 0
#define OTP_CTRL_OTP_SHADOW_REGION_1_OTP_SHADOW_REGION_1_SIZE 32

/** bit field defines for otp_ctrl_s#otp_shadow_region_2 */
#define OTP_CTRL_OTP_SHADOW_REGION_2_OTP_SHADOW_REGION_2_OFFSET 0
#define OTP_CTRL_OTP_SHADOW_REGION_2_OTP_SHADOW_REGION_2_SIZE 32

/** bit field defines for otp_ctrl_s#otp_write_protect */
#define OTP_CTRL_OTP_WRITE_PROTECT_PROTECT_COOKIE_OFFSET 0
#define OTP_CTRL_OTP_WRITE_PROTECT_PROTECT_COOKIE_SIZE 32

/** bit field defines for otp_ctrl_s#peripheralid4 */
#define OTP_CTRL_PERIPHERALID4_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_PERIPHERALID4_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#peripheralid5 */
#define OTP_CTRL_PERIPHERALID5_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_PERIPHERALID5_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#peripheralid6 */
#define OTP_CTRL_PERIPHERALID6_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_PERIPHERALID6_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#peripheralid7 */
#define OTP_CTRL_PERIPHERALID7_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_PERIPHERALID7_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#peripheralid0 */
#define OTP_CTRL_PERIPHERALID0_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_PERIPHERALID0_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#peripheralid1 */
#define OTP_CTRL_PERIPHERALID1_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_PERIPHERALID1_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#peripheralid2 */
#define OTP_CTRL_PERIPHERALID2_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_PERIPHERALID2_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#peripheralid3 */
#define OTP_CTRL_PERIPHERALID3_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_PERIPHERALID3_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#componentid0 */
#define OTP_CTRL_COMPONENTID0_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_COMPONENTID0_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#componentid2 */
#define OTP_CTRL_COMPONENTID2_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_COMPONENTID2_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#componentid3 */
#define OTP_CTRL_COMPONENTID3_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_COMPONENTID3_PRIMECELLIDENTIFICATION_SIZE 8

/** bit field defines for otp_ctrl_s#componentid4 */
#define OTP_CTRL_COMPONENTID4_PRIMECELLIDENTIFICATION_OFFSET 0
#define OTP_CTRL_COMPONENTID4_PRIMECELLIDENTIFICATION_SIZE 8

/* EOF otp_ctrl.h */
#endif
