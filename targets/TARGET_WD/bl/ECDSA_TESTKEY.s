/*
	THIS IS FOR DEBUGGING THE BOOTLOADER DSA IMPLEMENTATION ONLY.

	Generated: 26.09.2017 13:25:06
	Key-File: .\testkey.pem commited in the vNextElfTool
	Key-Thumbprint: E1-38-51-D3-34-77-3A-80-11-0B-37-A4-34-61-D3-CB-87-AD-CC-9E-07-7A-32-23-AE-AE-CB-39-CD-EA-E0-19
	Endianess: Big
	Word-Order: Reversed

	This Key is always compiled into the .elf, but is is not active, until WD_ABI_BL_HEADER_FLAG_PRODUCTION header flag is set
*/

#include "bl_asmoptions.inc"
#include "WD_ABI.h"

.section .bl_ecdsa_testkey,"ax",%progbits

#define BL_ECDSA_THUMBPRINT 0xB886082B, 0xB3645FFE, 0x89BFFA44, 0x69B92EA8, 0x59887799, 0x3648971F, 0xBCEE5E6A, 0x2ED509F5
#define BL_ECDSA_PUBX		0xA14DDD6C, 0x43F9223C, 0x69177D35, 0xA900FC19, 0xE6F81304, 0x4A6456DE, 0x23137AD5, 0x5EF8A2D3
#define BL_ECDSA_PUBY		0x1B44945A, 0x2EFEED18, 0xE35EC97F, 0x2EF16341, 0x6203D844, 0x184D0AFA, 0x6BD934AD, 0xBFE8A50F

	.word 0xFFFFFFFF	// this is just to avoid, that if the test-key get linked directly after the bootloader, that this can be misinterpreted as a bootloader 

	.word WD_ABI_KEYSTORE_CURRENT_VERSION	// Key-Store Version (we currently allow only one cert)
	.word BL_ECDSA_THUMBPRINT
	.word BL_ECDSA_PUBX
	.word BL_ECDSA_PUBY

