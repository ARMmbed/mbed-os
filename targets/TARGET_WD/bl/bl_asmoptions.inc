/*
 * bl_asmoptions.inc
 *
 * Created: 27.09.2017 14:35:30
 *  Author: Guenter.Prossliner
 */ 

.syntax unified
.thumb
#include "bl_asmoptions_targetspecific.inc"

#define BL_GLOBAL_FUNCTION(name) \
	.type name, %function;	\
	.global name;		\
	name

#define BL_LOCAL_FUNCTION(name)	\
	.type name, %function;	\
	name

#define EXPSTR(a) EXPSTRHLP(a)
#define EXPSTRHLP(a) #a

#define CONCAT2(a,b) a ## b

#define BL_ASSERT(reg, op, comp)	\
	CMP reg, comp;	\
	CONCAT2(B, op) 666f;	\
	B bl_die;	\
	666:

//////////////////////////////////////////////////////////////////////////
//	CONSTANTS
//////////////////////////////////////////////////////////////////////////
#define IMAGE_STATE_FLDOFST_STAGE_INDEX	0x00
#define IMAGE_STATE_FLDOFST_IMAGE_START	0x04
#define IMAGE_STATE_FLDOFST_HEADER_WORD	0x08
#define IMAGE_STATE_FLDOFST_IMAGE_SIZE	0x0C
#define IMAGE_STATE_FLDOFST_KEYSTORE	0x10
#define IMAGE_STATE_STRUCT_SIZE			0x14

#define SYSTEM_STATE_STRUCT_SIZE						0x14
#define SYSTEM_STATE_FLDOFST_BOOTLOADER_IMAGE_STATE		0x00
#define SYSTEM_STATE_FLDOFST_APPLICATION_IMAGE_STATE	0x04
#define SYSTEM_STATE_FLDOFST_UPDATE_IMAGE_STATE			0x08
#define SYSTEM_STATE_FLDOFST_COMMAND_WORD_PTR			0x0C
#define SYSTEM_STATE_FLDOFST_UPDATE_SIZE				0x10


// we use .bl_text as default, it may get redefined if needed in the .S file
.section .bl_text,"ax",%progbits
