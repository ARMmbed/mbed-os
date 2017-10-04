#ifndef INCLUDED_TDR_ACCESS_UNIT
#define INCLUDED_TDR_ACCESS_UNIT
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** tdr_access_unit HAL Spreadsheet version number */
#define TDR_ACCESS_UNIT_HAL_VERSION 1

/** TDR Access Unit register block
*/
struct tdr_access_unit_s {
   /** CtrlReg at address offset 0x000, read-write */
   uint32_t ctrlreg;
   /** DataReg at address offset 0x004, read-write */
   uint32_t datareg;
   /** WriteKey at address offset 0x008, read-write */
   uint32_t writekey;
};

/** bit field defines for tdr_access_unit_s#ctrlreg */
#define TDR_ACCESS_UNIT_CTRLREG_WRITETDR_OFFSET 0
#define TDR_ACCESS_UNIT_CTRLREG_WRITETDR_SIZE 1
#define TDR_ACCESS_UNIT_CTRLREG_READTDR_OFFSET 1
#define TDR_ACCESS_UNIT_CTRLREG_READTDR_SIZE 1
#define TDR_ACCESS_UNIT_CTRLREG_FUNCEN_OFFSET 2
#define TDR_ACCESS_UNIT_CTRLREG_FUNCEN_SIZE 1
#define TDR_ACCESS_UNIT_CTRLREG_TDRIRDECODING_OFFSET 3
#define TDR_ACCESS_UNIT_CTRLREG_TDRIRDECODING_SIZE 5
#define TDR_ACCESS_UNIT_CTRLREG_TDRPACKETNUMB_OFFSET 8
#define TDR_ACCESS_UNIT_CTRLREG_TDRPACKETNUMB_SIZE 10
#define TDR_ACCESS_UNIT_CTRLREG_TDRREADDONE_OFFSET 31
#define TDR_ACCESS_UNIT_CTRLREG_TDRREADDONE_SIZE 1

/** bit field defines for tdr_access_unit_s#datareg */
#define TDR_ACCESS_UNIT_DATAREG_NONE_OFFSET 0
#define TDR_ACCESS_UNIT_DATAREG_NONE_SIZE 32

/** bit field defines for tdr_access_unit_s#writekey */
#define TDR_ACCESS_UNIT_WRITEKEY_CODE_OFFSET 0
#define TDR_ACCESS_UNIT_WRITEKEY_CODE_SIZE 8

/* EOF tdr_access_unit.h */
#endif
