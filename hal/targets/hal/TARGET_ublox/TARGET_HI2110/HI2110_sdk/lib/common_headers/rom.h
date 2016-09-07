/******************************************************************************
 * @brief    ROM API Interface Definition
 * Copyright (c) 2016 NEUL LIMITED
 ******************************************************************************/

#ifndef SRC_LIB_COMMON_HEADERS_ROM_H_
#define SRC_LIB_COMMON_HEADERS_ROM_H_

#include "boudica_memory_config.h"

#include "iflash.h"
#include "iflash_config.h"
#include "tomcrypt.h"

/* ROM version identifier */
typedef struct
{
    const uint32 version;
    const uint32 epoch;
    const char string[];
}
ROM_Version_Info;


typedef struct
{
    /* Flash */
    IFLASH_STATUS (*iflash_init_section)( FLASH_SECTION_TYPE type, uint8 id, uint32 offset, uint32 length );
    IFLASH_STATUS (*iflash_claim_section)( FLASH_SECTION_TYPE type, uint8 id, CORES core, uint8 * buffer_p, uint32 length );
    IFLASH_STATUS (*iflash_release_section)( FLASH_SECTION_TYPE type, uint8 id );
    IFLASH_STATUS (*iflash_flush_section)( FLASH_SECTION_TYPE type, uint8 id );
    IFLASH_STATUS (*iflash_erase_section)( FLASH_SECTION_TYPE type, uint8 id );
    IFLASH_STATUS (*iflash_perform_actions_on_sleep)( void );

    void (*iflash_config_add_nv_section)( FLASH_SECTION_TYPE type, uint8 id, uint32 length );
    memory_map * (*iflash_config_get_memory_config)( CORES core );
    void (*iflash_config_set_memory_region_0_config)( CORES core, uint32 start, uint32 length, uint32 section_length );
    void (*iflash_config_set_memory_region_1_config)( CORES core, uint32 start, uint32 length );
    const char * (*iflash_config_get_sha256_checksum)( CORES core );

    /* SHA256 */
    int (*sha256_init)( hash_state * md );
    int (*sha256_process)( hash_state * md, const unsigned char *in, unsigned long inlen );
    int (*sha256_done)( hash_state * md, unsigned char *hash );
}
ROM_Api;


/* Specific instance of the jump table, located at a well known location! */
#define ROM_API ((ROM_Api *)(ROM_API_ORIGIN))

/* IFLASH abstraction */
#define IFLASH_INIT_SECTION(type,id,offset,length)                  ( ROM_API->iflash_init_section((type), (id), (offset), (length) ) )
#define IFLASH_CLAIM_SECTION(type,id,core,buffer,length)            ( ROM_API->iflash_claim_section((type), (id), (core), (buffer), (length) ) )
#define IFLASH_RELEASE_SECTION(type,id)                             ( ROM_API->iflash_release_section((type), (id) ) )
#define IFLASH_FLUSH_SECTION(type,id)                               ( ROM_API->iflash_flush_section((type), (id) ) )
#define IFLASH_ERASE_SECTION(type,id)                               ( ROM_API->iflash_erase_section((type), (id) ) )
#define IFLASH_PERFORM_ACTIONS_ON_SLEEP()                           ( ROM_API->iflash_perform_actions_on_sleep() )

#define IFLASH_CONFIG_ADD_SECTION(type,id,length)                   ( ROM_API->iflash_config_add_nv_section((type), (id), (length) ))
#define IFLASH_CONFIG_GET_MEMORY_CONFIG(core)                       ( ROM_API->iflash_config_get_memory_config((core) ) )
#define IFLASH_CONFIG_SET_MEMORY_0_CONFIG(core,start,length,max)    ( ROM_API->iflash_config_set_memory_region_0_config((core), (start), (length), (max) ) )
#define IFLASH_CONFIG_SET_MEMORY_1_CONFIG(core,start,length)        ( ROM_API->iflash_config_set_memory_region_1_config((core), (start), (length) ) )
#define IFLASH_CONFIG_GET_SHA256_CHECKSUM(core)                     ( ROM_API->iflash_config_get_sha256_checksum((core) ) )

/* SHA256 abstraction */
#define SHA256_INIT(md)                                             ( ROM_API->sha256_init((md)) )
#define SHA256_PROCESS(md,in,len)                                   ( ROM_API->sha256_process((md),( in),( len)) )
#define SHA256_DONE(md, hash)                                       ( ROM_API->sha256_done((md),(hash)) )

#endif /* SRC_LIB_COMMON_HEADERS_ROM_H_ */
