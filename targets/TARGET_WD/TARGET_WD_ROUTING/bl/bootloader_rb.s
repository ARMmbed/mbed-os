
/*
 * bootloader_rb.s
 *
 * Created: 23.08.2017 17:24:13
 *  Author: Guenter.Prossliner
 */ 

#include "bl_asmoptions.inc"

// these defines has been taken from stm32f103xg.h
#define PERIPH_BASE           0x40000000
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define CRC_BASE              (AHBPERIPH_BASE + 0x3000)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x2000) 
#define FLASH_BANK1_END       (0x0807FFFF)


/*************************************************************************
	void bl_hal_init(void):
	Initializes the HW resources needed for other functions in the hal
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_init):
//////////////////////////////////////////////////////////////////////////
bl_hal_init:

PUSH {lr}

	/////////////////////////////////////////////////////
	// Enabled Clocks with RCC:
	LDR r0, bl_hal_rcc_base_address

	// first we need to enable the clocks needed in bootloader in AHB bus
	//	RCC_AHBENR (0x14):
	//		Bit 4 FLITFEN
	//		Bit 6 CRCEN
	//			-> 0x50
	LDR r1, [r0, #0x14]	// load existing value, because this register has some undefined bits we want to keep
	MOV r2, #0x50
	ORR r1, r2	// new value
	STR r1, [r0, 0x14]	// write it back

	//	RCC_APB2ENR (0x18)
	//		Bit 3 IOPBEN (for Bus-Led)	-> 0x08
	LDR r1, [r0, #0x18]
	MOV r2, #0x08
	ORR r1, r2
	STR r1, [r0, 0x18]

	/////////////////////////////////////////////////////
	// Enabled Output for PB_8
	//	BUS_LED	= PB_8 (0x18)#
	//	Port: 0x18 >> 4 = 1 (=GPIOB)
	//  Pin:  0x18 & 0f = 8	

	LDR r0, bl_hal_gpiob_base_address

	// we need to use GPIOx_CRH (Offset 4) for the Pin 8
	// enable "Alternate function output Push-pull" output on BUS-LED
	// needs MODE8 to be 0b10, so the mask is 2

	LDR r1, [r0, #4]	// this has an ugly default of 0x44..., so we don't overwrite it with a constant
	BFC r1, #0, #4
	MOV r2, #0x2
	ORR r1, r1, r2
	STR r1, [r0, #4]


POP {pc}


/*************************************************************************
	void bl_hal_ui(value):
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_ui):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	// we use the Port bit set/reset register (GPIOx_BSRR)
	// Pin = PB_8 (0x18) & 0f = 8	
	// to turn it on, we have to RESET, to turn off, we have to SET
	// SET:		1 << 0x8			= 0x100
	// RESET:	1 << (0x8 + 0x10)	= 0x1000000

	CMP r0, #0
	ITE eq
	MOVEQ r1, #0x1000000
	MOVNE r1, #0x100

	LDR r2, bl_hal_gpiob_base_address
	STR r1, [r2, #0x10]

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

	MOV r0, #0x80000
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
	
	LDR r0, bl_hal_crc_address
	MOV r1, #1

	STR r1, [r0, #0x08]	// (CRC_CR)

POP {pc}

/*************************************************************************
	unsigned int bl_hal_crc_update(unsigned int):
	Updates the crc calculation 
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_crc_update):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}
	
	LDR r1, bl_hal_crc_address
	STR r0, [r1, #0]		// CRC_DR, we have the argument passed in r0
	LDR r0, [r1, #0]		// read the updated value and return it in r0
	
POP {pc}


/*************************************************************************
	void bl_hal_ldr_flash_reg(ptr):
	
	Returns the FLASH-SR address for the given ptr.
	This is needed because the Dual-Bank Flash uses spearate registers for both banks.
	
	The registers can be accessed by:
		[0]	=> FLASH_SR
		[4] => FLASH_CR
		[8] => FLASH_AR
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_ldr_flash_reg):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	MOV r2, r0
	LDR r0, bl_hal_flash_base_address
	ADD r0, #0x0C	// SR bank1

	LDR r1, bl_hal_flash_bank1_end
	CMP r2, r1
	BLS 0f

	ADD r0, #0x40	// SR bank2

0:
POP {pc}

/*************************************************************************
	void bl_hal_flash_unlock(ptr):
	
	Unlocks flash controller for ptr if not yet done 
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_flash_unlock):
//////////////////////////////////////////////////////////////////////////
PUSH {r4, lr}

	MOV r4, r0	// ptr

	BL bl_hal_flash_wait_idle

	MOV r0, r4
	BL bl_hal_ldr_flash_reg	// r0: &FLASH_SR

	LDR r1, [r0, #0x04]		// r1: FLASH_CR
	ANDS r1, #0x80
	BEQ 0f	// return if true

	// unlock sequence
	LDR r1, bl_hal_flash_key1
	STR r1, [r0, #-0x08] // FLASH_KEYR
	LDR r1, bl_hal_flash_key2
	STR r1, [r0, #-0x08] // FLASH_KEYR

0:
POP {r4, pc}

/*************************************************************************
	void bl_hal_flash_lock(ptr):
	
	Locks flash controller if not yet done 
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_flash_lock):
//////////////////////////////////////////////////////////////////////////
PUSH {r4, lr}

	MOV r4, r0

	BL bl_hal_flash_wait_idle

	MOV r0, r4
	BL bl_hal_ldr_flash_reg	// r0: &FLASH_SR
	
	LDR r1, [r0, #0x04]		// FLASH_CR
	ANDS r1, #0x80
	BNE 0f	// return if already locked

	MOV r1, #0x80
	STR r1, [r0, #0x04] // FLASH_CR

0:
POP {r4, pc}

/*************************************************************************
	void bl_hal_flash_memcpy(dest, src, size):
	
	Programms data from 'src' to 'dest'
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_flash_memcpy):
//////////////////////////////////////////////////////////////////////////
PUSH {r4, r5, r6, r7, lr}

	MOV r4, r0 // dest, will be postincremented
	MOV r5, r1 // src, will be postincremented
	MOV r6, r2 // size, will be decremented to check for completion

	BL bl_hal_ldr_flash_reg
	MOV r7, r0	// FLASH-SR

	MOV r0, r4 // ptr arg
	BL bl_hal_flash_unlock

	MOV r0, r4 // ptr arg
	BL bl_hal_flash_wait_idle

	// enable programming mode 
	MOV r0, #0x01
	STR r0, [r7, #0x04]

	.L_memcpy_next:
		LDRH r0, [r5], #2	// load src halfword
		STRH r0, [r4], #2	// write to dest

		MOV r0, r4 // ptr arg
		BL bl_hal_flash_wait_idle

		SUBS r6, #2	// size -=4
		BNE .L_memcpy_next	// while !=0

	// disable programming mode
	MOV r0, r4 // ptr arg
	BL bl_hal_flash_lock

0:
POP {r4, r5, r6, r7, pc}


/*************************************************************************
	void bl_hal_erase_boot_image(void):
	
	Erases the whole boot-image
*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_erase_boot_image):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	LDR r0, =#0x08002000	// start boot area
	LDR r1, =#0x08080000	// end boot area

	BL bl_hal_erase_region
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
		ST-LINK_CLI -w32 0x08002000 0x12345678
		ST-LINK_CLI -w32 0x0807FFFC 0x12345678

		REM write to config start
		ST-LINK_CLI -w32 0x08080000 0xCCCCCCCC
		ST-LINK_CLI -w32 0x08081FFC 0xCCCCCCCC

		REM write to update 
		ST-LINK_CLI -w32 0x08082000 0x12345678
		ST-LINK_CLI -w32 0x080FFFFC 0x12345678

*/
//////////////////////////////////////////////////////////////////////////
BL_GLOBAL_FUNCTION(bl_hal_erase_update_image):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}

	LDR r0, =#0x08082000	// start update area
	LDR r1, =#0x08100000	// end update area

	BL bl_hal_erase_region

0:
POP {pc}

/*************************************************************************
	void bl_hal_erase_region(intpr_t from, intpr_t to):
	
	Erases the pages in the specifed address region
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_erase_region):
//////////////////////////////////////////////////////////////////////////
PUSH {r4, r5, lr}

	MOV r4, r0	// from
	MOV r5, r1  // to

	1:
		MOV r0, r4
		BL bl_hal_erase_page
		ADD r4, #0x800
		CMP r4, r5
	BNE 1b

0:
POP {r4, r5, pc}

/*************************************************************************
	void bl_hal_erase_page(intptr_t page_address):
	
	Erases the specified page
*/
//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_erase_page):
//////////////////////////////////////////////////////////////////////////
PUSH {r4, lr}

	MOV r4, r0 // r4: page_address

	MOV r0, r4	// ptr arg
	BL bl_hal_flash_unlock
	
	MOV r0, r4	// ptr arg
	BL bl_hal_ldr_flash_reg	// r0: FLASH_SR
	
	// enable PER in CR
	MOV r1, #0x02	
	STR r1, [r0, #0x04]

	// set address in FLASH_AR
	STR r4, [r0, #0x08]

	// start by setting STRT (we keep PER)
	MOV r1, #0x42
	STR r1, [r0, #0x04]

	MOV r0, r4	// ptr arg, lock waits for !BSY
	BL bl_hal_flash_lock
0:
POP {r4, pc}


//////////////////////////////////////////////////////////////////////////
BL_LOCAL_FUNCTION(bl_hal_flash_wait_idle):
//////////////////////////////////////////////////////////////////////////
PUSH {lr}
	BL bl_hal_ldr_flash_reg	// r0: FLASH_SR
1:
	LDR r1, [r0, #0x00]		// FLASH_SR
	ANDS r1, #0x01			// bit 1: BSY
	BNE 1b	// retest

POP {pc}


.align 4
bl_hal_rcc_base_address:	.word RCC_BASE
bl_hal_gpiob_base_address:	.word GPIOB_BASE
bl_hal_crc_address:			.word CRC_BASE
bl_hal_flash_base_address:	.word FLASH_R_BASE
bl_hal_flash_bank1_end:		.word FLASH_BANK1_END
bl_hal_flash_key1:			.word 0x45670123
bl_hal_flash_key2:			.word 0xCDEF89AB


// the assembler will emit it's data from the LDR r2, =<constant> expressions at the end of the file
// so this label is to clean up the dissassembly.
bl_hal_misc_data: