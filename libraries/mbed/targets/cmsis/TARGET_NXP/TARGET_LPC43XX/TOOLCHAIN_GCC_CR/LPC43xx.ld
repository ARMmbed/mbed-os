/*
 * LPC43XX Dual core Blinky stand-alone Cortex-M4 LD script
*/

MEMORY
{
  /* Define each memory region */
  RO_MEM (rx) : ORIGIN = 0x14000000, LENGTH = 0x40000 /* 256K */
  RW_MEM (rwx) : ORIGIN = 0x10000000, LENGTH = 0x8000 /* 32k */
  RW_MEM1 (rwx) : ORIGIN = 0x20004000, LENGTH = 0x4000 /* 16K */
  SH_MEM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x2000 /* 8k */
  FAT12_MEM (rwx) : ORIGIN = 0x20002000, LENGTH = 0x2000 /* 8k */

}

  __top_RW_MEM = 0x10000000 + 0x8000;

INCLUDE "lpc43xx_dualcore_lib.ld"
INCLUDE "lpc43xx_dualcore.ld"
