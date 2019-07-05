/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "flash_api.h"
#include "mbed_critical.h"

#if DEVICE_FLASH
#include <string.h>
#include "iodefine.h"
#include "spibsc_iobitmask.h"
#include "spibsc.h"
#include "mbed_drv_cfg.h"

/* ---- serial flash command ---- */
#if (FLASH_SIZE > 0x1000000)
#define SPIBSC_OUTPUT_ADDR           SPIBSC_OUTPUT_ADDR_32
#define SFLASHCMD_SECTOR_ERASE       (0x21u)    /* SE4B   4-byte address(1bit)             */
#define SFLASHCMD_PAGE_PROGRAM       (0x12u)    /* PP4B   4-byte address(1bit), data(1bit) */
#else
#define SPIBSC_OUTPUT_ADDR           SPIBSC_OUTPUT_ADDR_24
#define SFLASHCMD_SECTOR_ERASE       (0x20u)    /* SE     3-byte address(1bit)             */
#define SFLASHCMD_PAGE_PROGRAM       (0x02u)    /* PP     3-byte address(1bit), data(1bit) */
#endif
#define SFLASHCMD_READ_STATUS_REG    (0x05u)    /* RDSR                         data(1bit) */
#define SFLASHCMD_WRITE_ENABLE       (0x06u)    /* WREN                                    */
/* ---- serial flash register definitions ---- */
#define STREG_BUSY_BIT               (0x01u)    /* SR.[0]BUSY Erase/Write In Progress (RO) */

/* Definition of the base address for the MMU translation table */
#if defined(__CC_ARM) || defined(__ARMCC_VERSION) || defined(__GNUC__)
extern uint32_t Image$$TTB$$ZI$$Base;
#define TTB         ((uint32_t)&Image$$TTB$$ZI$$Base)   /* using linker symbol */
#elif defined(__ICCARM__)
#pragma section="TTB"
#define TTB         ((uint32_t)__section_begin("TTB"))
#endif

typedef struct {
    uint32_t cdb;       /* bit-width : command */
    uint32_t ocdb;      /* bit-width : optional command */
    uint32_t adb;       /* bit-width : address */
    uint32_t opdb;      /* bit-width : option data */
    uint32_t spidb;     /* bit-width : data */

    uint32_t cde;       /* Enable : command */
    uint32_t ocde;      /* Enable : optional command */
    uint32_t ade;       /* Enable : address */
    uint32_t opde;      /* Enable : option data */
    uint32_t spide;     /* Enable : data */

    uint32_t sslkp;     /* SPBSSL level */
    uint32_t spire;     /* Enable data read */
    uint32_t spiwe;     /* Enable data write */

    uint32_t dme;       /* Enable : dummy cycle */

    uint32_t addre;     /* DDR enable : address  */
    uint32_t opdre;     /* DDR enable : option data */
    uint32_t spidre;    /* DDR enable : data */

    uint8_t dmdb;       /* bit-width : dummy cycle */
    uint8_t dmcyc;      /* number of dummy cycles */

    uint8_t  cmd;       /* command */
    uint8_t  ocmd;      /* optional command */
    uint32_t addr;      /* address */
    uint8_t  opd[4];    /* option data 3/2/1/0 */
    uint32_t smrdr[2];  /* read data */
    uint32_t smwdr[2];  /* write data */
} st_spibsc_spimd_reg_t;

typedef struct {
    uint32_t b0             : 1 ;       /* bit 0        : -         (0)                                   */
    uint32_t b1             : 1 ;       /* bit 1        : -         (1)                                   */
    uint32_t B              : 1 ;       /* bit 2        : B         Memory region attribute bit           */
    uint32_t C              : 1 ;       /* bit 3        : C         Memory region attribute bit           */
    uint32_t XN             : 1 ;       /* bit 4        : XN        Execute-never bit                     */
    uint32_t Domain         : 4 ;       /* bit 8-5      : Domain    Domain field                          */
    uint32_t b9             : 1 ;       /* bit 9        : IMP       IMPLEMENTATION DEFINED                */
    uint32_t AP1_0          : 2 ;       /* bit 11-10    : AP[1:0]   Access permissions bits:bit1-0        */
    uint32_t TEX            : 3 ;       /* bit 14-12    : TEX[2:0]  Memory region attribute bits          */
    uint32_t AP2            : 1 ;       /* bit 15       : AP[2]     Access permissions bits:bit2          */
    uint32_t S              : 1 ;       /* bit 16       : S         Shareable bit                         */
    uint32_t nG             : 1 ;       /* bit 17       : nG        Not global bit                        */
    uint32_t b18            : 1 ;       /* bit 18       : -         (0)                                   */
    uint32_t NS             : 1 ;       /* bit 19       : NS        Non-secure bit                        */
    uint32_t base_addr      : 12;       /* bit 31-20    : PA[31:20] PA(physical address) bits:bit31-20    */
} mmu_ttbl_desc_section_t;

static mmu_ttbl_desc_section_t desc_tbl[(FLASH_SIZE >> 20)];
static volatile struct st_spibsc*  SPIBSC = &SPIBSC0;
static st_spibsc_spimd_reg_t spimd_reg;
static uint8_t write_tmp_buf[FLASH_PAGE_SIZE];

#if defined(__ICCARM__)
#define RAM_CODE_SEC    __ramfunc
#else
#define RAM_CODE_SEC    __attribute__((section("RAM_CODE")))
#endif

/* Global function for optimization */
RAM_CODE_SEC int32_t _sector_erase(uint32_t addr);
RAM_CODE_SEC int32_t _page_program(uint32_t addr, const uint8_t * buf, int32_t size);

static RAM_CODE_SEC int32_t write_enable(void);
static RAM_CODE_SEC int32_t busy_wait(void);
static RAM_CODE_SEC int32_t read_register(uint8_t cmd, uint8_t * status);
static RAM_CODE_SEC int32_t data_send(uint32_t bit_width, uint32_t spbssl_level, const uint8_t * buf, int32_t size);
static RAM_CODE_SEC void spi_mode(void);
static RAM_CODE_SEC void ex_mode(void);
static RAM_CODE_SEC void clear_spimd_reg(volatile st_spibsc_spimd_reg_t * regset);
static RAM_CODE_SEC int32_t spibsc_transfer(st_spibsc_spimd_reg_t * regset);
static RAM_CODE_SEC uint32_t RegRead_32(volatile uint32_t * ioreg, uint32_t shift, uint32_t mask);
static RAM_CODE_SEC void RegWwrite_32(volatile uint32_t * ioreg, uint32_t write_value, uint32_t shift, uint32_t mask);
static RAM_CODE_SEC void change_mmu_ttbl_spibsc(uint32_t type);
static RAM_CODE_SEC void spibsc_stop(void);
static RAM_CODE_SEC void cache_control(void);

int32_t flash_init(flash_t *obj)
{
    return 0;
}

int32_t flash_free(flash_t *obj)
{
    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    return _sector_erase(address - FLASH_BASE);
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    return _page_program(address - FLASH_BASE, data, size);
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return MBED_FLASH_INVALID_SIZE;
    }

    return FLASH_SECTOR_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return 8;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_BASE;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_SIZE;
}

int32_t _sector_erase(uint32_t addr)
{
    int32_t ret;

    core_util_critical_section_enter();
    spi_mode();

    /* ---- Write enable   ---- */
    ret = write_enable();      /* WREN Command */
    if (ret != 0) {
        ex_mode();
        core_util_critical_section_exit();
        return ret;
    }

    /* ---- spimd_reg init ---- */
    clear_spimd_reg(&spimd_reg);

    /* ---- command ---- */
    spimd_reg.cde    = SPIBSC_OUTPUT_ENABLE;
    spimd_reg.cdb    = SPIBSC_1BIT;
    spimd_reg.cmd    = SFLASHCMD_SECTOR_ERASE;

    /* ---- address ---- */
    spimd_reg.ade    = SPIBSC_OUTPUT_ADDR;
    spimd_reg.addre  = SPIBSC_SDR_TRANS;       /* SDR */
    spimd_reg.adb    = SPIBSC_1BIT;
    spimd_reg.addr   = addr;

    ret = spibsc_transfer(&spimd_reg);
    if (ret != 0) {
        ex_mode();
        core_util_critical_section_exit();
        return ret;
    }

    ret = busy_wait();

    ex_mode();
    core_util_critical_section_exit();
    return ret;
}

int32_t _page_program(uint32_t addr, const uint8_t * buf, int32_t size)
{
    int32_t ret;
    int32_t program_size;
    int32_t remainder;
    int32_t idx = 0;

    while (size > 0) {
        if (size > FLASH_PAGE_SIZE) {
            program_size = FLASH_PAGE_SIZE;
        } else {
            program_size = size;
        }
        remainder = FLASH_PAGE_SIZE - (addr % FLASH_PAGE_SIZE);
        if ((remainder != 0) && (program_size > remainder)) {
            program_size = remainder;
        }

        core_util_critical_section_enter();
        memcpy(write_tmp_buf, &buf[idx], program_size);
        spi_mode();

        /* ---- Write enable   ---- */
        ret = write_enable();      /* WREN Command */
        if (ret != 0) {
            ex_mode();
            core_util_critical_section_exit();
            return ret;
        }

        /* ----------- 1. Command, Address ---------------*/
        /* ---- spimd_reg init ---- */
        clear_spimd_reg(&spimd_reg);

        /* ---- command ---- */
        spimd_reg.cde    = SPIBSC_OUTPUT_ENABLE;
        spimd_reg.cdb    = SPIBSC_1BIT;
        spimd_reg.cmd    = SFLASHCMD_PAGE_PROGRAM;

        /* ---- address ---- */
        spimd_reg.ade    = SPIBSC_OUTPUT_ADDR;
        spimd_reg.addre  = SPIBSC_SDR_TRANS;       /* SDR */
        spimd_reg.adb    = SPIBSC_1BIT;
        spimd_reg.addr   = addr;

        /* ---- Others ---- */
        spimd_reg.sslkp  = SPIBSC_SPISSL_KEEP;     /* SPBSSL level */

        ret = spibsc_transfer(&spimd_reg);         /* Command,Address */
        if (ret != 0) {
            ex_mode();
            core_util_critical_section_exit();
            return ret;
        }

        /* ----------- 2. Data ---------------*/
        ret = data_send(SPIBSC_1BIT, SPIBSC_SPISSL_NEGATE, write_tmp_buf, program_size);
        if (ret != 0) {
            ex_mode();
            core_util_critical_section_exit();
            return ret;
        }

        ret = busy_wait();
        if (ret != 0) {
            ex_mode();
            core_util_critical_section_exit();
            return ret;
        }

        ex_mode();
        core_util_critical_section_exit();

        addr += program_size;
        idx  += program_size;
        size -= program_size;
    }

    return ret;
}

static int32_t write_enable(void)
{
    int32_t ret;

    /* ---- spimd_reg init ---- */
    clear_spimd_reg(&spimd_reg);

    /* ---- command ---- */
    spimd_reg.cde    = SPIBSC_OUTPUT_ENABLE;
    spimd_reg.cdb    = SPIBSC_1BIT;
    spimd_reg.cmd    = SFLASHCMD_WRITE_ENABLE;

    ret = spibsc_transfer(&spimd_reg);

    return ret;
}

static int32_t busy_wait(void)
{
    int32_t ret;
    uint8_t st_reg;

    while (1) {
        ret = read_register(SFLASHCMD_READ_STATUS_REG, &st_reg);
        if (ret != 0) {
            break;
        }
        if ((st_reg & STREG_BUSY_BIT) == 0) {
            break;
        }
    }

    return ret;
}

static int32_t read_register(uint8_t cmd, uint8_t * status)
{
    int32_t ret;

    /* ---- spimd_reg init ---- */
    clear_spimd_reg(&spimd_reg);

    /* ---- command ---- */
    spimd_reg.cde    = SPIBSC_OUTPUT_ENABLE;
    spimd_reg.cdb    = SPIBSC_1BIT;
    spimd_reg.cmd    = cmd;

    /* ---- Others ---- */
    spimd_reg.sslkp  = SPIBSC_SPISSL_NEGATE;   /* SPBSSL level */
    spimd_reg.spire  = SPIBSC_SPIDATA_ENABLE;  /* read enable/disable */
    spimd_reg.spiwe  = SPIBSC_SPIDATA_ENABLE;  /* write enable/disable */

    /* ---- data ---- */
    spimd_reg.spide  = SPIBSC_OUTPUT_SPID_8;   /* Enable(8bit) */
    spimd_reg.spidre = SPIBSC_SDR_TRANS;       /* SDR */
    spimd_reg.spidb  = SPIBSC_1BIT;
    spimd_reg.smwdr[0] = 0x00;                 /* Output 0 in read status */
    spimd_reg.smwdr[1] = 0x00;                 /* Output 0 in read status */

    ret = spibsc_transfer(&spimd_reg);
    if (ret == 0) {
        *status = (uint8_t)(spimd_reg.smrdr[0]);   /* Data[7:0]  */
    }

    return ret;
}

static int32_t data_send(uint32_t bit_width, uint32_t spbssl_level, const uint8_t * buf, int32_t size)
{
    int32_t ret = 0;
    int32_t unit;
    uint8_t  *buf_b;
    uint16_t *buf_s;
    uint32_t *buf_l;

    /* ---- spimd_reg init ---- */
    clear_spimd_reg(&spimd_reg);

    /* ---- Others ---- */
    spimd_reg.sslkp  = SPIBSC_SPISSL_KEEP;     /* SPBSSL level */
    spimd_reg.spiwe  = SPIBSC_SPIDATA_ENABLE;  /* write enable/disable */

    /* ---- data ---- */
    spimd_reg.spidb = bit_width;
    spimd_reg.spidre= SPIBSC_SDR_TRANS;        /* SDR */

    if (((uint32_t)size & 0x3)  == 0) {
        spimd_reg.spide = SPIBSC_OUTPUT_SPID_32;  /* Enable(32bit) */
        unit = 4;
    } else if (((uint32_t)size & 0x1) == 0) {
        spimd_reg.spide = SPIBSC_OUTPUT_SPID_16;  /* Enable(16bit) */
        unit = 2;
    } else {
        spimd_reg.spide = SPIBSC_OUTPUT_SPID_8;   /* Enable(8bit) */
        unit = 1;
    }

    while (size > 0) {
        if (unit == 1) {
            buf_b = (uint8_t *)buf;
            spimd_reg.smwdr[0] = (uint32_t)(((uint32_t)*buf_b) & 0x000000FF);
        } else if (unit == 2) {
            buf_s = (uint16_t *)buf;
            spimd_reg.smwdr[0] = (uint32_t)(((uint32_t)*buf_s) & 0x0000FFFF);
        } else if (unit == 4) {
            buf_l = (uint32_t *)buf;
            spimd_reg.smwdr[0] = (uint32_t)(((uint32_t)(*buf_l)) & 0xfffffffful);
        } else {
            /* Do Nothing */
        }

        buf  += unit;
        size -= unit;

        if (size <= 0) {
            spimd_reg.sslkp = spbssl_level;
        }

        ret = spibsc_transfer(&spimd_reg);    /* Data */
        if (ret != 0) {
            return ret;
        }
    }

    return ret;
}

static void spi_mode(void)
{
    volatile uint32_t dummy_read_32;

    if (RegRead_32(&SPIBSC->CMNCR, SPIBSC_CMNCR_MD_SHIFT, SPIBSC_CMNCR_MD) != SPIBSC_CMNCR_MD_SPI) {
        /* ==== Change the MMU translation table SPI Multi-I/O bus space settings
                for use in SPI operating mode ==== */
        change_mmu_ttbl_spibsc(0);

        /* ==== Cleaning and invalidation of cache ==== */
        cache_control();

        /* ==== Switch to SPI operating mode ==== */
        spibsc_stop();

        dummy_read_32 = SPIBSC->CMNCR; /* dummy read */
        /* SPI Mode */
        RegWwrite_32(&SPIBSC->CMNCR, SPIBSC_CMNCR_MD_SPI, SPIBSC_CMNCR_MD_SHIFT, SPIBSC_CMNCR_MD);
        dummy_read_32 = SPIBSC->CMNCR; /* dummy read */

    }
    (void)dummy_read_32;
}

static void ex_mode(void)
{
    volatile uint32_t dummy_read_32;

    if (RegRead_32(&SPIBSC->CMNCR, SPIBSC_CMNCR_MD_SHIFT, SPIBSC_CMNCR_MD) != SPIBSC_CMNCR_MD_EXTRD) {
        /* ==== Switch to external address space read mode and clear SPIBSC read cache ==== */
        spibsc_stop();

        /* Flush SPIBSC's read cache */
        RegWwrite_32(&SPIBSC->DRCR, SPIBSC_DRCR_RCF_EXE, SPIBSC_DRCR_RCF_SHIFT, SPIBSC_DRCR_RCF);
        dummy_read_32 = SPIBSC->DRCR;  /* dummy read */

        /* External address space read mode */
        RegWwrite_32(&SPIBSC->CMNCR, SPIBSC_CMNCR_MD_EXTRD, SPIBSC_CMNCR_MD_SHIFT, SPIBSC_CMNCR_MD);
        dummy_read_32 = SPIBSC->CMNCR; /* dummy read */

        /* ==== Change the MMU translation table SPI Multi-I/O bus space settings
                for use in external address space read mode ==== */
        change_mmu_ttbl_spibsc(1);

        /* ==== Cleaning and invalidation of cache ==== */
        cache_control();
    }
    (void)dummy_read_32;
}

static void clear_spimd_reg(volatile st_spibsc_spimd_reg_t * regset)
{
    /* ---- command ---- */
    regset->cde    = SPIBSC_OUTPUT_DISABLE;
    regset->cdb    = SPIBSC_1BIT;
    regset->cmd    = 0x00;

    /* ---- optional command ---- */
    regset->ocde   = SPIBSC_OUTPUT_DISABLE;
    regset->ocdb   = SPIBSC_1BIT;
    regset->ocmd   = 0x00;

    /* ---- address ---- */
    regset->ade    = SPIBSC_OUTPUT_DISABLE;
    regset->addre  = SPIBSC_SDR_TRANS;       /* SDR */
    regset->adb    = SPIBSC_1BIT;
    regset->addr   = 0x00000000;

    /* ---- option data ---- */
    regset->opde   = SPIBSC_OUTPUT_DISABLE;
    regset->opdre  = SPIBSC_SDR_TRANS;       /* SDR */
    regset->opdb   = SPIBSC_1BIT;
    regset->opd[0] = 0x00;    /* OPD3 */
    regset->opd[1] = 0x00;    /* OPD2 */
    regset->opd[2] = 0x00;    /* OPD1 */
    regset->opd[3] = 0x00;    /* OPD0 */

    /* ---- dummy cycle ---- */
    regset->dme    = SPIBSC_DUMMY_CYC_DISABLE;
    regset->dmdb   = SPIBSC_1BIT;
    regset->dmcyc  = SPIBSC_DUMMY_1CYC;

    /* ---- data ---- */
    regset->spide  = SPIBSC_OUTPUT_DISABLE;
    regset->spidre = SPIBSC_SDR_TRANS;       /* SDR */
    regset->spidb  = SPIBSC_1BIT;

    /* ---- Others ---- */
    regset->sslkp  = SPIBSC_SPISSL_NEGATE;   /* SPBSSL level */
    regset->spire  = SPIBSC_SPIDATA_DISABLE; /* read enable/disable */
    regset->spiwe  = SPIBSC_SPIDATA_DISABLE; /* write enable/disable */
}

static int32_t spibsc_transfer(st_spibsc_spimd_reg_t * regset)
{
    if (RegRead_32(&SPIBSC->CMNCR, SPIBSC_CMNCR_MD_SHIFT, SPIBSC_CMNCR_MD) != SPIBSC_CMNCR_MD_SPI) {
        if (RegRead_32(&SPIBSC->CMNSR, SPIBSC_CMNSR_SSLF_SHIFT, SPIBSC_CMNSR_SSLF) != SPIBSC_SSL_NEGATE) {
            return -1;
        }
        /* SPI Mode */
        RegWwrite_32(&SPIBSC->CMNCR, SPIBSC_CMNCR_MD_SPI, SPIBSC_CMNCR_MD_SHIFT, SPIBSC_CMNCR_MD);
    }

    if (RegRead_32(&SPIBSC->CMNSR, SPIBSC_CMNSR_TEND_SHIFT, SPIBSC_CMNSR_TEND) != SPIBSC_TRANS_END) {
        return -1;
    }

    /* ---- Command ---- */
    /* Enable/Disable */
    RegWwrite_32(&SPIBSC->SMENR, regset->cde, SPIBSC_SMENR_CDE_SHIFT, SPIBSC_SMENR_CDE);
    if (regset->cde != SPIBSC_OUTPUT_DISABLE) {
        /* Command */
        RegWwrite_32(&SPIBSC->SMCMR, regset->cmd, SPIBSC_SMCMR_CMD_SHIFT, SPIBSC_SMCMR_CMD);
        /* Single/Dual/Quad */
        RegWwrite_32(&SPIBSC->SMENR, regset->cdb, SPIBSC_SMENR_CDB_SHIFT, SPIBSC_SMENR_CDB);
    }

    /* ---- Option Command ---- */
    /* Enable/Disable */
    RegWwrite_32(&SPIBSC->SMENR, regset->ocde, SPIBSC_SMENR_OCDE_SHIFT, SPIBSC_SMENR_OCDE);
    if (regset->ocde != SPIBSC_OUTPUT_DISABLE) {
        /* Option Command */
        RegWwrite_32(&SPIBSC->SMCMR, regset->ocmd, SPIBSC_SMCMR_OCMD_SHIFT, SPIBSC_SMCMR_OCMD);
        /* Single/Dual/Quad */
        RegWwrite_32(&SPIBSC->SMENR, regset->ocdb, SPIBSC_SMENR_OCDB_SHIFT, SPIBSC_SMENR_OCDB);
    }

    /* ---- Address ---- */
    /* Enable/Disable */
    RegWwrite_32(&SPIBSC->SMENR, regset->ade, SPIBSC_SMENR_ADE_SHIFT, SPIBSC_SMENR_ADE);
    if (regset->ade != SPIBSC_OUTPUT_DISABLE) {
        /* Address */
        RegWwrite_32(&SPIBSC->SMADR, regset->addr, SPIBSC_SMADR_ADR_SHIFT, SPIBSC_SMADR_ADR);
        /* Single/Dual/Quad */
        RegWwrite_32(&SPIBSC->SMENR, regset->adb, SPIBSC_SMENR_ADB_SHIFT, SPIBSC_SMENR_ADB);
    }

    /* ---- Option Data ---- */
    /* Enable/Disable */
    RegWwrite_32(&SPIBSC->SMENR, regset->opde, SPIBSC_SMENR_OPDE_SHIFT, SPIBSC_SMENR_OPDE);
    if (regset->opde != SPIBSC_OUTPUT_DISABLE) {
        /* Option Data */
        RegWwrite_32(&SPIBSC->SMOPR, regset->opd[0], SPIBSC_SMOPR_OPD3_SHIFT, SPIBSC_SMOPR_OPD3);
        RegWwrite_32(&SPIBSC->SMOPR, regset->opd[1], SPIBSC_SMOPR_OPD2_SHIFT, SPIBSC_SMOPR_OPD2);
        RegWwrite_32(&SPIBSC->SMOPR, regset->opd[2], SPIBSC_SMOPR_OPD1_SHIFT, SPIBSC_SMOPR_OPD1);
        RegWwrite_32(&SPIBSC->SMOPR, regset->opd[3], SPIBSC_SMOPR_OPD0_SHIFT, SPIBSC_SMOPR_OPD0);
        /* Single/Dual/Quad */
        RegWwrite_32(&SPIBSC->SMENR, regset->opdb, SPIBSC_SMENR_OPDB_SHIFT, SPIBSC_SMENR_OPDB);
    }

    /* ---- Dummy ---- */
     /* Enable/Disable */
     RegWwrite_32(&SPIBSC->SMENR, regset->dme, SPIBSC_SMENR_DME_SHIFT, SPIBSC_SMENR_DME);
     if (regset->dme != SPIBSC_DUMMY_CYC_DISABLE) {
         RegWwrite_32(&SPIBSC->SMDMCR, regset->dmdb, SPIBSC_SMDMCR_DMDB_SHIFT, SPIBSC_SMDMCR_DMDB);
         /* Dummy Cycle */
         RegWwrite_32(&SPIBSC->SMDMCR, regset->dmcyc, SPIBSC_SMDMCR_DMCYC_SHIFT, SPIBSC_SMDMCR_DMCYC);
     }

    /* ---- Data ---- */
    /* Enable/Disable */
    RegWwrite_32(&SPIBSC->SMENR, regset->spide, SPIBSC_SMENR_SPIDE_SHIFT, SPIBSC_SMENR_SPIDE);
    if (regset->spide != SPIBSC_OUTPUT_DISABLE) {
        if (SPIBSC_OUTPUT_SPID_8 == regset->spide) {
            if (RegRead_32(&SPIBSC0.CMNCR, SPIBSC_CMNCR_BSZ_SHIFT, SPIBSC_CMNCR_BSZ) == SPIBSC_CMNCR_BSZ_SINGLE) {
                SPIBSC->SMWDR0.UINT8[0] = (uint8_t)(regset->smwdr[0]);
            } else {
                SPIBSC->SMWDR0.UINT16[0] = (uint16_t)(regset->smwdr[0]);
            }
        } else if (regset->spide == SPIBSC_OUTPUT_SPID_16) {
            if (RegRead_32(&SPIBSC0.CMNCR, SPIBSC_CMNCR_BSZ_SHIFT, SPIBSC_CMNCR_BSZ) == SPIBSC_CMNCR_BSZ_SINGLE) {
                SPIBSC->SMWDR0.UINT16[0] = (uint16_t)(regset->smwdr[0]);
            } else {
                SPIBSC->SMWDR0.UINT32 = regset->smwdr[0];
            }
        } else if (regset->spide == SPIBSC_OUTPUT_SPID_32) {
            if (RegRead_32(&SPIBSC0.CMNCR, SPIBSC_CMNCR_BSZ_SHIFT, SPIBSC_CMNCR_BSZ) == SPIBSC_CMNCR_BSZ_SINGLE) {
                SPIBSC->SMWDR0.UINT32 = (uint32_t)(regset->smwdr[0]);
            } else {
                SPIBSC->SMWDR0.UINT32 = (uint32_t)(regset->smwdr[0]);
                SPIBSC->SMWDR1.UINT32 = (uint32_t)(regset->smwdr[1]);  /* valid in two serial-flash */
            }
        } else {
            /* none */
        }

        /* Single/Dual/Quad */
        RegWwrite_32(&SPIBSC->SMENR, regset->spidb, SPIBSC_SMENR_SPIDB_SHIFT, SPIBSC_SMENR_SPIDB);
    }

    RegWwrite_32(&SPIBSC->SMCR, regset->sslkp, SPIBSC_SMCR_SSLKP_SHIFT, SPIBSC_SMCR_SSLKP);

    if ((regset->spidb != SPIBSC_1BIT) && (regset->spide != SPIBSC_OUTPUT_DISABLE)) {
        if ((regset->spire == SPIBSC_SPIDATA_ENABLE) && (regset->spiwe == SPIBSC_SPIDATA_ENABLE)) {
            /* not set in same time */
            return -1;
        }
    }

    RegWwrite_32(&SPIBSC->SMCR, regset->spire, SPIBSC_SMCR_SPIRE_SHIFT, SPIBSC_SMCR_SPIRE);
    RegWwrite_32(&SPIBSC->SMCR, regset->spiwe, SPIBSC_SMCR_SPIWE_SHIFT, SPIBSC_SMCR_SPIWE);

    /* SDR Transmission/DDR Transmission Setting */
    RegWwrite_32(&SPIBSC->SMDRENR, regset->addre, SPIBSC_SMDRENR_ADDRE_SHIFT, SPIBSC_SMDRENR_ADDRE);
    RegWwrite_32(&SPIBSC->SMDRENR, regset->opdre, SPIBSC_SMDRENR_OPDRE_SHIFT, SPIBSC_SMDRENR_OPDRE);
    RegWwrite_32(&SPIBSC->SMDRENR, regset->spidre, SPIBSC_SMDRENR_SPIDRE_SHIFT, SPIBSC_SMDRENR_SPIDRE);

    /* execute after setting SPNDL bit */
    RegWwrite_32(&SPIBSC->SMCR, SPIBSC_SPI_ENABLE, SPIBSC_SMCR_SPIE_SHIFT, SPIBSC_SMCR_SPIE);

    /* wait for transfer-start */
    while (RegRead_32(&SPIBSC->CMNSR, SPIBSC_CMNSR_TEND_SHIFT, SPIBSC_CMNSR_TEND) != SPIBSC_TRANS_END) {
        /* wait for transfer-end */
    }

    if (SPIBSC_OUTPUT_SPID_8 == regset->spide) {
        if (RegRead_32(&SPIBSC0.CMNCR, SPIBSC_CMNCR_BSZ_SHIFT, SPIBSC_CMNCR_BSZ) == SPIBSC_CMNCR_BSZ_SINGLE) {
            regset->smrdr[0] = SPIBSC->SMRDR0.UINT8[0];
        } else {
            regset->smrdr[0] = SPIBSC->SMRDR0.UINT16[0];        /* valid in two serial-flash  */
        }
    } else if (regset->spide == SPIBSC_OUTPUT_SPID_16) {
        if (RegRead_32(&SPIBSC0.CMNCR, SPIBSC_CMNCR_BSZ_SHIFT, SPIBSC_CMNCR_BSZ) == SPIBSC_CMNCR_BSZ_SINGLE) {
            regset->smrdr[0] = SPIBSC->SMRDR0.UINT16[0];
        } else {
            regset->smrdr[0] = SPIBSC->SMRDR0.UINT32;           /* valid in two serial-flash  */
        }
    } else if (regset->spide == SPIBSC_OUTPUT_SPID_32) {
        if (RegRead_32(&SPIBSC0.CMNCR, SPIBSC_CMNCR_BSZ_SHIFT, SPIBSC_CMNCR_BSZ) == SPIBSC_CMNCR_BSZ_SINGLE) {
            regset->smrdr[0] = SPIBSC->SMRDR0.UINT32;
        } else {
            regset->smrdr[0] = SPIBSC->SMRDR0.UINT32;           /* valid in two serial-flash  */
            regset->smrdr[1] = SPIBSC->SMRDR1.UINT32;
        }
    } else {
        /* none */
    }

    return 0;
}

static uint32_t RegRead_32(volatile uint32_t * ioreg, uint32_t shift, uint32_t mask)
{
    uint32_t reg_value;

    reg_value = *ioreg;                        /* Read from register            */
    reg_value = (reg_value & mask) >> shift;   /* Clear other bit and Bit shift */

    return reg_value;
}

static void RegWwrite_32(volatile uint32_t * ioreg, uint32_t write_value, uint32_t shift, uint32_t mask)
{
    uint32_t reg_value;

    reg_value = *ioreg;                                         /* Read from register */
    reg_value = (reg_value & (~mask)) | (write_value << shift); /* Modify value       */
    *ioreg    = reg_value;                                      /* Write to register  */
}

static void change_mmu_ttbl_spibsc(uint32_t type)
{
    uint32_t index;               /* Loop variable: table index */
    mmu_ttbl_desc_section_t desc; /* Loop variable: descriptor */
    mmu_ttbl_desc_section_t * table = (mmu_ttbl_desc_section_t *)TTB;

    /* ==== Modify SPI Multi-I/O bus space settings in the MMU translation table ==== */
    for (index = (FLASH_BASE >> 20); index < ((FLASH_BASE + FLASH_SIZE) >> 20); index++) {
        /* Modify memory attribute descriptor */
        if (type == 0) {         /* Spi */
            desc = table[index];
            desc_tbl[index - (FLASH_BASE >> 20)] = desc;
            desc.AP1_0 = 0x0u;   /* AP[2:0] = b'000 (No access) */
            desc.AP2   = 0x0u;
            desc.XN    = 0x1u;   /* XN = 1 (Execute never) */
        } else {                 /* Xip */
            desc = desc_tbl[index - (FLASH_BASE >> 20)];
        }
        /* Write descriptor back to translation table */
        table[index] = desc;
    }
}

static void spibsc_stop(void)
{
    if (((SPIBSC->DRCR & SPIBSC_DRCR_RBE)  != 0) &&
        ((SPIBSC->DRCR & SPIBSC_DRCR_SSLE) != 0)) {
        RegWwrite_32(&SPIBSC->DRCR, 1, SPIBSC_DRCR_SSLN_SHIFT, SPIBSC_DRCR_SSLN);
    }

    while (RegRead_32(&SPIBSC->CMNSR, SPIBSC_CMNSR_SSLF_SHIFT, SPIBSC_CMNSR_SSLF) != SPIBSC_SSL_NEGATE) {
        ;
    }

    while (RegRead_32(&SPIBSC->CMNSR, SPIBSC_CMNSR_TEND_SHIFT, SPIBSC_CMNSR_TEND) != SPIBSC_TRANS_END) {
        ;
    }
}

static void cache_control(void)
{
    unsigned int assoc;

    /* ==== Cleaning and invalidation of the L1 data cache ==== */
    L1C_CleanInvalidateDCacheAll();
    __DSB();

    /* ==== Cleaning and invalidation of the L2 cache ==== */
    if (L2C_310->AUX_CNT & (1U << 16U)) {
        assoc = 16U;
    } else {
        assoc =  8U;
    }
    L2C_310->CLEAN_INV_WAY = (1U << assoc) - 1U;
    while (L2C_310->CLEAN_INV_WAY & ((1U << assoc) - 1U)); // poll invalidate
    L2C_310->CACHE_SYNC = 0x0;

    /* ==== Invalidate all TLB entries ==== */
    __set_TLBIALL(0);
    __DSB();     // ensure completion of the invalidation
    __ISB();     // ensure instruction fetch path sees new state

    /* ==== Invalidate the L1 instruction cache ==== */
    __set_ICIALLU(0);
    __DSB();     // ensure completion of the invalidation
    __ISB();     // ensure instruction fetch path sees new I cache state
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
