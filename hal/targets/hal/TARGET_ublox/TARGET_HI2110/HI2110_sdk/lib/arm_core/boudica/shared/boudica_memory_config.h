/******************************************************************************
 * @brief    Default memory configurations
 * Copyright (c) 2015 NEUL LIMITED
******************************************************************************/
#ifndef BOUDICA_MEMORY_CONFIG_H_
#define BOUDICA_MEMORY_CONFIG_H_

/*
 * ********************* ROM ALLOCATION ***********************
 *
 * Used solely by the Security Core ROM image built into the chip.
 */

/* 64K ROM, but 1 word is reserved for digits use */
#define ROM_START               (0x30000000)
#define ROM_LENGTH              (0x10000 - 0x4)

/*
 * ********************* FLASH ALLOCATION *********************
 *

 * Flash is shared between the cores, and also handles a limited number of
 * non-volatile storage areas. Split is follows:
 *
 *      Config Data             1 page (4K)
 *      Non Volatile storage    8 pages (32K)
 *      Apps Code               64K  (0x10000)
 *      Protocol Code           860K (0xD7000)
 *      Security Code           64K  (0x10000)
 */

#define FLASH_START             (0x10000000)
#define FLASH_LENGTH            (0x00100000)


/* Reserve 9 * 4K for non-volatile data. */
#define FLASH_RESERVED_LENGTH	0x9000

/* 200K for Security core - matches eventual ROM */
#define SC_FLASH_REGION_LENGTH 	0x10000

/* 200K for Apps core - enough to allow testing */
#define AC_FLASH_REGION_LENGTH 	0x10000

/* Remainder for Protocol core. */
#define RC_FLASH_REGION_LENGTH  (FLASH_LENGTH - FLASH_RESERVED_LENGTH - SC_FLASH_REGION_LENGTH - AC_FLASH_REGION_LENGTH)

#define FLASH_END               (FLASH_START + FLASH_LENGTH)

#if (( FLASH_START + SC_FLASH_REGION_LENGTH + RC_FLASH_REGION_LENGTH + AC_FLASH_REGION_LENGTH + FLASH_RESERVED_LENGTH) > FLASH_END )
#error "The flash regions specified do not fit in the flash size."
#endif


/* ROM based security code */
#define ROM_VECTORS_ORIGIN  ( ROM_START )
#define ROM_VECTORS_LENGTH  ( 192 )

#define ROM_VERSION_ORIGIN  ( ROM_VECTORS_ORIGIN + ROM_VECTORS_LENGTH )
#define ROM_VERSION_LENGTH  ( 64 )
#define ROM_API_ORIGIN      ( ROM_VERSION_ORIGIN + ROM_VERSION_LENGTH )
#define ROM_API_LENGTH      ( 256 )

#define ROM_PROGRAM_ORIGIN  ( ROM_API_ORIGIN + ROM_API_LENGTH )
#define ROM_PROGRAM_LENGTH  ( ROM_LENGTH - ROM_VECTORS_LENGTH - ROM_VERSION_LENGTH - ROM_API_LENGTH )

/* Security Configuration pages in FLASH reserved and used in ROM Code: The last flash page is used for security core configuration.*/
#define SC_RESERVED_FLASH_PAGES 1


/*
 * ********************* RAM ALLOCATION *********************
 *
 * Two main RAM areas, the 'security core' RAM, and the 'shared' RAM.
 * As the name implies, the apps and protocol cores can only access the
 * 'shared' RAM, while the security core can access everything.
 *
 * The security core RAM is used by security code running from ROM and RAM,
 * so a section is reserved for ROM use to avoid memory corruption when
 * calling ROM based utility functions.
 *
 * SECURITY RAM             SECURITY IPC mailbox
 *                          ROM code RAM area
 *                          FLASH code RAM area
 *
 * The 'shared' RAM is used by the application and protocol cores, with a
 * small area visible to both to exchange larger amounts of data.
 *
 * SHARED RAM               SHARED
 *                          PROTOCOL code RAM
 *                          APPS code RAM
 */

/* 16K 'dedicated' for Security core. Outside of 192K total. */
#define SECURITY_RAM_ORIGIN             (0x38000000)
#define SECURITY_RAM_LENGTH             (0x04000)

/* Allocated a small amount of the 'security' ram for use by the code in ROM. */
#define SECURITY_RAM_ALLOCATED_TO_ROM   (0x01000)

#define SC_RAM_ORIGIN                   (SECURITY_RAM_ORIGIN)
#define SC_RAM_LENGTH                   (SECURITY_RAM_LENGTH - SECURITY_RAM_ALLOCATED_TO_ROM)


/* Each core sees RAM starting at the same relative location (using region 1). */
#define BOUDICA_PRIVATE_RAM_ORIGIN  0x01000000

/* 192K total 'shared' RAM. Split across APPS & PROTOCOL core + common 'shared' area. */
#define RAM_LENGTH                  0x0030000

/* Allocate 16K for 'shared' use. */
#define SHARED_RAM_LENGTH           0x04000

/* 16K for APPS core. */
#define AC_RAM_ORIGIN       BOUDICA_PRIVATE_RAM_ORIGIN
#define AC_RAM_LENGTH       0x04000

/* Allocate majority of RAM to PROTOCOL core. L3 is very hungry. */
#define RC_RAM_ORIGIN       BOUDICA_PRIVATE_RAM_ORIGIN
#define RC_RAM_LENGTH       ( RAM_LENGTH - SHARED_RAM_LENGTH - AC_RAM_LENGTH )


#if (( AC_RAM_LENGTH + RC_RAM_LENGTH + SHARED_RAM_LENGTH) > RAM_LENGTH )
#error "The memory regions specified do not fit in the available RAM size."
#endif


/*
 * Vector tables
 */
#define SC_VECTORS_ORIGIN FLASH_START
#define SC_VECTORS_LENGTH 192
#define SC_PROGRAM_ORIGIN (SC_VECTORS_ORIGIN + SC_VECTORS_LENGTH)
#define SC_PROGRAM_LENGTH (SC_FLASH_REGION_LENGTH - SC_VECTORS_LENGTH)

#define RC_VECTORS_ORIGIN 0
#define RC_VECTORS_LENGTH 192
#define RC_PROGRAM_ORIGIN (RC_VECTORS_ORIGIN + RC_VECTORS_LENGTH)
#define RC_PROGRAM_LENGTH (RC_FLASH_REGION_LENGTH - RC_VECTORS_LENGTH)

#define AC_VECTORS_ORIGIN 0
#define AC_VECTORS_LENGTH 192
#define AC_PROGRAM_ORIGIN (AC_VECTORS_ORIGIN + AC_VECTORS_LENGTH)
#define AC_PROGRAM_LENGTH (AC_FLASH_REGION_LENGTH - AC_VECTORS_LENGTH)



/*
 * Region 0, Code in Flash! Physical addresses of FLASH code. Used by security core to configure Region 0.
 */
#define SECURITY_PROC_OFFSET    (FLASH_START)
#define SECURITY_PROC_LENGTH    (SC_FLASH_REGION_LENGTH)

#define PROTOCOL_PROC_OFFSET    (SECURITY_PROC_OFFSET + SECURITY_PROC_LENGTH)
#define PROTOCOL_PROC_LENGTH    (RC_FLASH_REGION_LENGTH)

#define APPS_PROC_OFFSET        (SECURITY_PROC_OFFSET + SECURITY_PROC_LENGTH + PROTOCOL_PROC_LENGTH)
#define APPS_PROC_LENGTH        (AC_FLASH_REGION_LENGTH)


/*
 * Region 1, Main RAM! Physical addresses of RAM. USed by security core to configure Region 1.
 */
#define SHARED_MEM_START        (0x20000000)

#define APPS_MEM_OFFSET         (SHARED_MEM_START + 0)
#define APPS_MEM_LENGTH         (AC_RAM_LENGTH)

#define PROTOCOL_MEM_OFFSET     (SHARED_MEM_START + AC_RAM_LENGTH)
#define PROTOCOL_MEM_LENGTH     (RC_RAM_LENGTH)


/* Security core has it's own dedicated RAM */
#define SECURITY_MEM_OFFSET     (SC_RAM_ORIGIN)
#define SECURITY_MEM_LENGTH     (SC_RAM_LENGTH)



/* Reserved for IPC mailboxes. */
#define MESSAGE_QUEUE_RESERVE_LENGTH (256)

#define SC_IPC_MAILBOX_ORIGIN   (SECURITY_RAM_ORIGIN + SECURITY_RAM_LENGTH - MESSAGE_QUEUE_RESERVE_LENGTH)
#define SC_IPC_MAILBOX_LENGTH   (MESSAGE_QUEUE_RESERVE_LENGTH)

#define RC_IPC_MAILBOX_ORIGIN   (RC_RAM_ORIGIN + RC_RAM_LENGTH - MESSAGE_QUEUE_RESERVE_LENGTH)
#define RC_IPC_MAILBOX_LENGTH   (MESSAGE_QUEUE_RESERVE_LENGTH)

#define AC_IPC_MAILBOX_ORIGIN   (AC_RAM_ORIGIN + AC_RAM_LENGTH - MESSAGE_QUEUE_RESERVE_LENGTH)
#define AC_IPC_MAILBOX_LENGTH   (MESSAGE_QUEUE_RESERVE_LENGTH)


/*
 * ********************* OTP REGIONS *********************
 * OTP Regions, Start at 0 and has a maximum length of OTP_AMOUNT.
 * By default assign the whole OTP to the security core and make the other
 * 2 cores OTP of length 0 and start at the last byte of the OTP.
 */
#define OTP_SIZE_IN_BYTES      ( 3072 / 8 )

#define SECURITY_OTP_OFFSET    0
#define SECURITY_OTP_LENGTH    OTP_SIZE_IN_BYTES

#define PROTOCOL_OTP_OFFSET    (OTP_SIZE_IN_BYTES - 1) // it could be 0 as well but just an extra security level
#define PROTOCOL_OTP_LENGTH    0

#define APPS_OTP_OFFSET        (OTP_SIZE_IN_BYTES - 1) // it could be 0 as well but just an extra security level
#define APPS_OTP_LENGTH        0


#endif /* BOUDICA_MEMORY_CONFIG_H_ */
