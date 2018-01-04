/*
 * bl_ABI_HDR.S
 *
 * Created: 31.10.2017 19:41:36
 *  Author: Guenter.Prossliner
 */

#include "WD_ABI.h"
#include "bl_asmoptions.inc"


/*************************************************************************
	WD ABI HEADER for Bootloader
*/

.section  .bl_abi_hdr,"a",%progbits
.type  g_bl_abi_hdr, %object

g_bl_abi_hdr:

/* 00: MAGIC */	.word (WD_ABI_HDR_MAGIC << 16) | (WD_ABI_HDR_BL << 8)
/* 04: SIZE */		.word 0xFFFFFFFF
/* 08: CPUID_MASK */.word WD_ABI_CPUID_MASK
/* 0C: CPUID */		.word WD_ABI_CPUID
/* 10: RES0: */		.word 0						// due a bug in V1 bootloader, this has to be keept at value 0
.=WD_ABI_BLHDR_OFFSET_SRVCALL
/* 20: SRVCALL */	.word bl_srv_call			// WDABI: entry vector for indirect calls from blsrv
/* 24: LENGTH */	.word __bootloader_length	// defined in linker-script
/* 28: BANK2START*/	.word __bank2_start			// defined in linker-script

.size  g_bl_abi_hdr, .-g_bl_abi_hdr
