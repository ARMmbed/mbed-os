#ifndef _SAMD21_NVMCTRL_INSTANCE_
#define _SAMD21_NVMCTRL_INSTANCE_

/* ========== Register definition for NVMCTRL peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_NVMCTRL_CTRLA          (0x41004000U) /**< \brief (NVMCTRL) Control A */
#define REG_NVMCTRL_CTRLB          (0x41004004U) /**< \brief (NVMCTRL) Control B */
#define REG_NVMCTRL_PARAM          (0x41004008U) /**< \brief (NVMCTRL) NVM Parameter */
#define REG_NVMCTRL_INTENCLR       (0x4100400CU) /**< \brief (NVMCTRL) Interrupt Enable Clear */
#define REG_NVMCTRL_INTENSET       (0x41004010U) /**< \brief (NVMCTRL) Interrupt Enable Set */
#define REG_NVMCTRL_INTFLAG        (0x41004014U) /**< \brief (NVMCTRL) Interrupt Flag Status and Clear */
#define REG_NVMCTRL_STATUS         (0x41004018U) /**< \brief (NVMCTRL) Status */
#define REG_NVMCTRL_ADDR           (0x4100401CU) /**< \brief (NVMCTRL) Address */
#define REG_NVMCTRL_LOCK           (0x41004020U) /**< \brief (NVMCTRL) Lock Section */
#else
#define REG_NVMCTRL_CTRLA          (*(RwReg16*)0x41004000U) /**< \brief (NVMCTRL) Control A */
#define REG_NVMCTRL_CTRLB          (*(RwReg  *)0x41004004U) /**< \brief (NVMCTRL) Control B */
#define REG_NVMCTRL_PARAM          (*(RwReg  *)0x41004008U) /**< \brief (NVMCTRL) NVM Parameter */
#define REG_NVMCTRL_INTENCLR       (*(RwReg8 *)0x4100400CU) /**< \brief (NVMCTRL) Interrupt Enable Clear */
#define REG_NVMCTRL_INTENSET       (*(RwReg8 *)0x41004010U) /**< \brief (NVMCTRL) Interrupt Enable Set */
#define REG_NVMCTRL_INTFLAG        (*(RwReg8 *)0x41004014U) /**< \brief (NVMCTRL) Interrupt Flag Status and Clear */
#define REG_NVMCTRL_STATUS         (*(RwReg16*)0x41004018U) /**< \brief (NVMCTRL) Status */
#define REG_NVMCTRL_ADDR           (*(RwReg  *)0x4100401CU) /**< \brief (NVMCTRL) Address */
#define REG_NVMCTRL_LOCK           (*(RwReg16*)0x41004020U) /**< \brief (NVMCTRL) Lock Section */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for NVMCTRL peripheral ========== */
#define NVMCTRL_AUX0_ADDRESS        0x00804000
#define NVMCTRL_AUX1_ADDRESS        0x00806000
#define NVMCTRL_AUX2_ADDRESS        0x00808000
#define NVMCTRL_AUX3_ADDRESS        0x0080A000
#define NVMCTRL_CLK_AHB_ID          4        // Index of AHB Clock in PM.AHBMASK register
#define NVMCTRL_FACTORY_WORD_IMPLEMENTED_MASK 0xC0000007FFFFFFFF
#define NVMCTRL_FLASH_SIZE          262144
#define NVMCTRL_LOCKBIT_ADDRESS     0x00802000
#define NVMCTRL_PAGES               4096
#define NVMCTRL_PAGE_HW             32
#define NVMCTRL_PAGE_SIZE           64
#define NVMCTRL_PAGE_W              16
#define NVMCTRL_PMSB                3
#define NVMCTRL_PSZ_BITS            6
#define NVMCTRL_ROW_PAGES           4
#define NVMCTRL_ROW_SIZE            256
#define NVMCTRL_TEMP_LOG_ADDRESS    0x00806030
#define NVMCTRL_USER_PAGE_ADDRESS   0x00800000
#define NVMCTRL_USER_PAGE_OFFSET    0x00800000
#define NVMCTRL_USER_WORD_IMPLEMENTED_MASK 0xC01FFFFFFFFFFFFF

#endif /* _SAMD21_NVMCTRL_INSTANCE_ */
