
/*
 * bootloader_cb.s
 *
 * Created: 14.08.2017 14:37:44
 *  Author: Guenter.Prossliner
 */ 

#include "bl_asmoptions.inc"


.section .bl_text,"ax",%progbits

/*************************************************************************
	void bl_hal_init(void):
	Initializes the HW resources needed for other functions in the hal
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_init):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	/////////////////////////////////////////////////////
	// CLOCKS:
	// 
	// first we need to enable the clocks needed in bootloader in AHB1 bus
	//	GPIOE (for bus-led): bit #4 (GPIOEEN) on the RCC_AHB1ENR register (@40023830)
	//  CRC (for validation): bit #12 (CRCEN) also on the RCC_AHB1ENR register (@40023830)

	// set enable bits in RCC_AHB1ENR register (@40023830)
	// this register has a default value of != 0, so we need to read it
	LDR r3, bl_hal_rcc_ahb1enr_address	// use r3 for register address
	LDR r2, [r3]		// read current value into r2 (should be 0x00100000 as documented)
	LDR r1, =#0x1010	// the mask
	ORR r2, r2, r1	// perform OR
	STR r2, [r3, #0]	// and write to register

	/////////////////////////////////////////////////////
	// Enabled Output for PE10
	//	BUS_LED	== PE10 == 0x4A
	//	Port = 0x4A >> 4 ==  0x4 (GPIOE) = 0x40021000
	//	Pin = 0x4A & 0xF = 0xA

	LDR r3, bl_hal_gpioe_address
	MOV r2, #0x100000
	STR r2, [r3, #0]

POP {pc}

/*************************************************************************
	void bl_hal_ui(value):
		Turn an binary output (LED) on or off to signal bootloader state
		if(value) turnon(); else turnoff();

	ON: Bootloader running
		-> OFF all ok!
		-> 3 blink fast => DEVELOPMENT MODE
		-> endless blink slow => DSA verification failed

	Signals the user about the running bootloader (by BUS_LED)
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_ui):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	// we use the Port bit set/reset register (GPIOx_BSRR)
	// Pin = 0xA
	// to turn it on, we have to RESET, to turn off, we have to SET
	// SET:		1 << 0xA			= 0x400
	// RESET:	1 << (0xA + 0x10)	= 0x4000000

	CMP r0, #0
	ITE eq
	MOVEQ r1, #0x4000000
	MOVNE r1, #0x400

	LDR r2, bl_hal_gpioe_address
	STR r1, [r2, #0x18]

POP {pc}


/*************************************************************************
	void bl_hal_sleep(void):
		Sleeps a little bit.
		This should be around 250ms, but only used for blinking, so it don't have to be exact
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_sleep):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	MOV r0, #0x100000
	1:
		SUBS r0, #1
		BNE 1b 

POP {pc}



/*************************************************************************
	void bl_hal_crc_init(void):
	Initializes the crc generator to the init value of 0xFFFFFFFF
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_crc_init):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}
	
	MOV r1, #1
	LDR r2, bl_hal_crc_address		// CRC_ base address
	STR r1, [r2, #0x8]				// CRC_CR, set bit #0 (RESET) which clears the DR

POP {pc}

/*************************************************************************
	unsigned int bl_hal_crc_update(unsigned int):
	Updates the crc calculation 
*/
.global bl_hal_crc_update
.type bl_hal_crc_update, %function
bl_hal_crc_update:

PUSH {lr}
	
	LDR r2, bl_hal_crc_address
	STR r0, [r2, #0]		// CRC_DR, we have the argument passed in r0
	LDR r0, [r2, #0]		// read the updated value and return it in r0

POP {pc}


/*************************************************************************
	void bl_hal_flash_unlock():
	
	Unlocks flash controller if not yet done 
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_flash_unlock):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	BL bl_hal_flash_wait_idle
	
	LDR r1, bl_hal_flashc_address
	LDR r0, [r1, #0x10]		// FLASH_CR
	ANDS r0, #0x80000000
	BEQ 0f	// return if true

	// unlock sequence
	LDR r0, bl_hal_flash_key1
	STR r0, [r1, #0x04] // FLASH_KEYR
	LDR r0, bl_hal_flash_key2
	STR r0, [r1, #0x04] // FLASH_KEYR

0:
POP {pc}

/*************************************************************************
	void bl_hal_flash_lock():
	
	Locks flash controller if not yet done 
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_flash_lock):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	BL bl_hal_flash_wait_idle
	
	LDR r1, bl_hal_flashc_address
	LDR r0, [r1, #0x10]		// FLASH_CR
	ANDS r0, #0x80000000
	BNE 0f	// return if already locked

	MOV r0, #0x80000000
	STR r0, [r1, #0x10] // FLASH_CR

0:
POP {pc}

/*************************************************************************
	void bl_hal_flash_memcpy(dest, src, size):
	
	Programms data from 'src' to 'dest'
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_flash_memcpy):
//////////////////////////////////////////////////////////////////////////
PUSH {r4, r5, r6, lr}

	MOV r4, r0 // dest, will be postincremented
	MOV r5, r1 // src, will be postincremented
	MOV r6, r2 // size, will be decremented to check for completion

	BL bl_hal_flash_unlock
	BL bl_hal_flash_wait_idle

	/* enable programming mode for 32 bit
	bit 25:		ERRIE
	bits 8..9:	PSIZE 10 (32)
	bit 0:		PG (enable Programming) */
	LDR r2, =#0x2000201

	LDR r1, bl_hal_flashc_address
	STR r2, [r1, #0x10]

	.L_memcpy_next:
		LDR r0, [r5], #4	// load src word
		STR r0, [r4], #4	// write to dest

		BL bl_hal_flash_wait_idle

		SUBS r6, #4	// size -=4
		BNE .L_memcpy_next	// while !=0

	// disable programming mode
	BL bl_hal_flash_lock

0:
POP {r4, r5, r6, pc}


/*************************************************************************
	void bl_hal_erase_boot_image(void):
	
	Erases the whole boot-image
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_erase_boot_image):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	MOV r0, #1	// start sector
	MOV r1, #11  // noumber of sectors

	BL bl_hal_erase_sectors

0:
POP {pc}

/*************************************************************************
	void bl_hal_erase_update_image(void):
	
	Erases the whole update image.
	This is not directly called by the bootloader, but from the downloader.
	We will keep this here, because we already have everything needed here

	Test (with cmd.exe):

		set path=%path%;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility

		REM write to app
		ST-LINK_CLI -w32 0x08004000 0x12345678
		ST-LINK_CLI -w32 0x080FFFFC 0x12345678

		REM write to config start
		ST-LINK_CLI -w32 0x08100000 0xCCCCCCCC
		ST-LINK_CLI -w32 0x08103FFC 0xCCCCCCCC

		REM write to update 
		ST-LINK_CLI -w32 0x08104000 0x12345678
		ST-LINK_CLI -w32 0x081FFFFC 0x12345678

*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_erase_update_image):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	MOV r0, #17	// start sector ATTENTION: BANK 2 STARTS @0x10, even if it is sector 12 in the datasheet! 
	MOV r1, #11		// noumber of sectors

	BL bl_hal_erase_sectors

0:
POP {pc}

/*************************************************************************
	void bl_hal_erase_sectors(int from, int count):
	
	Erases the sectors starting in the specified range of sector numbers
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_erase_sectors):
//////////////////////////////////////////////////////////////////////////
PUSH {r4, r5, lr}

	MOV r4, r0	// start and current sector
	MOV r5, r1  // remaining sectors

	1:
		MOV r0, r4
		BL bl_hal_erase_sector
		ADD r4, #1
		SUBS r5, #1
	BNE 1b


0:
POP {r4, r5, pc}

/*************************************************************************
	void bl_hal_erase_sector(int sector_nr):
	
	Erases the specified sector
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_erase_sector):
//////////////////////////////////////////////////////////////////////////
PUSH {r4, lr}

	MOV r4, r0 // sector_nr

	BL bl_hal_flash_unlock
	BL bl_hal_flash_wait_idle

	LDR r1, bl_hal_flashc_address

	// construct value of FLASH_CR
	MOV r2, r4, LSL #3	// sector number bit 3-6
	ORR r2, #0x2		// SER: bit1
	ORR r2, #0x200		// PSIZE=b10 : bit 89
	STR r2, [r1, #0x10]

	ORR r2, #0x10000	// STRT: bit 16
	STR r2, [r1, #0x10]

	BL bl_hal_flash_lock
0:
POP {r4, pc}


/*************************************************************************
	void bl_hal_flash_wait_idle():
	
	Polls to the FLASH_SR to wait for an operation to finish
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_flash_wait_idle):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}
	
	LDR r1, bl_hal_flashc_address

1:
	LDR r0, [r1, #0x0C]		// FLASH_SR
	ANDS r0, #0x00010000		// bit 16: BSY
	BNE 1b	// retest
POP {pc}


bl_hal_gpioe_address:	.word 0x40021000
bl_hal_rcc_ahb1enr_address: .word 0x40023830
bl_hal_crc_address:		.word 0x40023000
bl_hal_flashc_address:	.word 0x40023C00
bl_hal_flash_key1:		.word 0x45670123
bl_hal_flash_key2:		.word 0xCDEF89AB



// the assembler will emit it's data from the LDR r2, =<constant> expressions at the end of the file
// so this label is to clean up the dissassembly.
bl_hal_misc_data:

