#ifndef LOCATIONS_H_
#define LOCATIONS_H_

#include "core.h"
#include "boudica_memory_config.h"

#if CORE == SECURITY
/* some symbols defined in the linker script */
extern unsigned int __FLASH_REGION_ORIGIN__;
extern unsigned int __FLASH_PROGRAM_LENGTH__;
extern unsigned int __FLASH_REGION_LENGTH__;

/*
 * Region 0, Flash Area! Lengths and start addresses should be multiple of page size
 * Make it consisten with the .lds file.
 */
#define SECURITY_PROC_OFFSET	FLASH_START
#define SECURITY_PROC_LENGTH	SC_FLASH_REGION_LENGTH

#define PROTOCOL_PROC_OFFSET	SECURITY_PROC_OFFSET + SECURITY_PROC_LENGTH
#define PROTOCOL_PROC_LENGTH	RC_FLASH_REGION_LENGTH

#define APPS_PROC_OFFSET		SECURITY_PROC_OFFSET + SECURITY_PROC_LENGTH + PROTOCOL_PROC_LENGTH
#define APPS_PROC_LENGTH		AC_FLASH_REGION_LENGTH 

#define PROGRAM_END				SECURITY_PROC_OFFSET + SECURITY_PROC_LENGTH + PROTOCOL_PROC_LENGTH + APPS_PROC_LENGTH

#endif
/*
 * This define should not be trusted, as it requires the owners of the above to update it
 * Use it as a guide
 */
#define FLASH_FREE_START		PROGRAM_END

#define CODE					0x00000000
#define PRIVATE_RAM				BOUDICA_PRIVATE_RAM_ORIGIN

#define SHARED_SEC_RAM			0x02000000
#define PUBLIC_RAM				0x03000000

/*
 * Region 1 Shared Memory Area!
 */
#define SHARED_MEM_START		0x20000000

#define APPS_MEM_OFFSET			(SHARED_MEM_START + 0)
#define APPS_MEM_LENGTH			AC_RAM_LENGTH

#define PROTOCOL_MEM_OFFSET		(SHARED_MEM_START + AC_RAM_LENGTH)
#define PROTOCOL_MEM_LENGTH		RC_RAM_LENGTH


/*
 * The security only RAM area
 */
#define SECURITY_MEM_OFFSET		SC_RAM_ORIGIN
#define SECURITY_MEM_LENGTH		SC_RAM_LENGTH

#endif
