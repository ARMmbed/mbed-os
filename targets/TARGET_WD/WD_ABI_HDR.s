/*
 * WD_ABI_HDR.S
 *
 * Created: 31.10.2017 19:40:30
 *  Author: Guenter.Prossliner
 */

#include "WD_ABI.h"
#include "bl_asmoptions.inc"

.section .abi_hdr,"a",%progbits
/* 00: MAGIC */		.word (WD_ABI_HDR_MAGIC << 16) | (WD_ABI_HDR_APP << 8)
/* 04: SIZE */		.word 0xFFFFFFFF
/* 08: CPUID_MASK */.word WD_ABI_CPUID_MASK
/* 0C: CPUID */		.word WD_ABI_CPUID
/* 10: NAME */		.=WD_ABI_APPHDR_OFFSET_NAME;.asciz EXPSTR(APPLICATION_NAME)
/* 30: VERSION */	.=WD_ABI_APPHDR_OFFSET_VERSION;.asciz EXPSTR(APPLICATION_VERSION)
