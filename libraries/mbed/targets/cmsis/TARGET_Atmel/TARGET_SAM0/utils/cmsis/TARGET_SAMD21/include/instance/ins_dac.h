#ifndef _SAMD21_DAC_INSTANCE_
#define _SAMD21_DAC_INSTANCE_

/* ========== Register definition for DAC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_DAC_CTRLA              (0x42004800U) /**< \brief (DAC) Control A */
#define REG_DAC_CTRLB              (0x42004801U) /**< \brief (DAC) Control B */
#define REG_DAC_EVCTRL             (0x42004802U) /**< \brief (DAC) Event Control */
#define REG_DAC_INTENCLR           (0x42004804U) /**< \brief (DAC) Interrupt Enable Clear */
#define REG_DAC_INTENSET           (0x42004805U) /**< \brief (DAC) Interrupt Enable Set */
#define REG_DAC_INTFLAG            (0x42004806U) /**< \brief (DAC) Interrupt Flag Status and Clear */
#define REG_DAC_STATUS             (0x42004807U) /**< \brief (DAC) Status */
#define REG_DAC_DATA               (0x42004808U) /**< \brief (DAC) Data */
#define REG_DAC_DATABUF            (0x4200480CU) /**< \brief (DAC) Data Buffer */
#else
#define REG_DAC_CTRLA              (*(RwReg8 *)0x42004800U) /**< \brief (DAC) Control A */
#define REG_DAC_CTRLB              (*(RwReg8 *)0x42004801U) /**< \brief (DAC) Control B */
#define REG_DAC_EVCTRL             (*(RwReg8 *)0x42004802U) /**< \brief (DAC) Event Control */
#define REG_DAC_INTENCLR           (*(RwReg8 *)0x42004804U) /**< \brief (DAC) Interrupt Enable Clear */
#define REG_DAC_INTENSET           (*(RwReg8 *)0x42004805U) /**< \brief (DAC) Interrupt Enable Set */
#define REG_DAC_INTFLAG            (*(RwReg8 *)0x42004806U) /**< \brief (DAC) Interrupt Flag Status and Clear */
#define REG_DAC_STATUS             (*(RoReg8 *)0x42004807U) /**< \brief (DAC) Status */
#define REG_DAC_DATA               (*(RwReg16*)0x42004808U) /**< \brief (DAC) Data */
#define REG_DAC_DATABUF            (*(RwReg16*)0x4200480CU) /**< \brief (DAC) Data Buffer */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for DAC peripheral ========== */
#define DAC_DMAC_ID_EMPTY           40       // Index of DMAC EMPTY trigger
#define DAC_GCLK_ID                 33       // Index of Generic Clock

#endif /* _SAMD21_DAC_INSTANCE_ */
