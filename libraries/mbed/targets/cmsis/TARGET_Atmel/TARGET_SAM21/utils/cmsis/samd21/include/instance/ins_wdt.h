#ifndef _SAMD21_WDT_INSTANCE_
#define _SAMD21_WDT_INSTANCE_

/* ========== Register definition for WDT peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_WDT_CTRL               (0x40001000U) /**< \brief (WDT) Control */
#define REG_WDT_CONFIG             (0x40001001U) /**< \brief (WDT) Configuration */
#define REG_WDT_EWCTRL             (0x40001002U) /**< \brief (WDT) Early Warning Interrupt Control */
#define REG_WDT_INTENCLR           (0x40001004U) /**< \brief (WDT) Interrupt Enable Clear */
#define REG_WDT_INTENSET           (0x40001005U) /**< \brief (WDT) Interrupt Enable Set */
#define REG_WDT_INTFLAG            (0x40001006U) /**< \brief (WDT) Interrupt Flag Status and Clear */
#define REG_WDT_STATUS             (0x40001007U) /**< \brief (WDT) Status */
#define REG_WDT_CLEAR              (0x40001008U) /**< \brief (WDT) Clear */
#else
#define REG_WDT_CTRL               (*(RwReg8 *)0x40001000U) /**< \brief (WDT) Control */
#define REG_WDT_CONFIG             (*(RwReg8 *)0x40001001U) /**< \brief (WDT) Configuration */
#define REG_WDT_EWCTRL             (*(RwReg8 *)0x40001002U) /**< \brief (WDT) Early Warning Interrupt Control */
#define REG_WDT_INTENCLR           (*(RwReg8 *)0x40001004U) /**< \brief (WDT) Interrupt Enable Clear */
#define REG_WDT_INTENSET           (*(RwReg8 *)0x40001005U) /**< \brief (WDT) Interrupt Enable Set */
#define REG_WDT_INTFLAG            (*(RwReg8 *)0x40001006U) /**< \brief (WDT) Interrupt Flag Status and Clear */
#define REG_WDT_STATUS             (*(RoReg8 *)0x40001007U) /**< \brief (WDT) Status */
#define REG_WDT_CLEAR              (*(WoReg8 *)0x40001008U) /**< \brief (WDT) Clear */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for WDT peripheral ========== */
#define WDT_GCLK_ID                 3        // Index of Generic Clock

#endif /* _SAMD21_WDT_INSTANCE_ */
