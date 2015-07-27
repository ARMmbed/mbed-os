#ifndef _SAMD21_ADC_INSTANCE_
#define _SAMD21_ADC_INSTANCE_

/* ========== Register definition for ADC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_ADC_CTRLA              (0x42004000U) /**< \brief (ADC) Control A */
#define REG_ADC_REFCTRL            (0x42004001U) /**< \brief (ADC) Reference Control */
#define REG_ADC_AVGCTRL            (0x42004002U) /**< \brief (ADC) Average Control */
#define REG_ADC_SAMPCTRL           (0x42004003U) /**< \brief (ADC) Sampling Time Control */
#define REG_ADC_CTRLB              (0x42004004U) /**< \brief (ADC) Control B */
#define REG_ADC_WINCTRL            (0x42004008U) /**< \brief (ADC) Window Monitor Control */
#define REG_ADC_SWTRIG             (0x4200400CU) /**< \brief (ADC) Software Trigger */
#define REG_ADC_INPUTCTRL          (0x42004010U) /**< \brief (ADC) Input Control */
#define REG_ADC_EVCTRL             (0x42004014U) /**< \brief (ADC) Event Control */
#define REG_ADC_INTENCLR           (0x42004016U) /**< \brief (ADC) Interrupt Enable Clear */
#define REG_ADC_INTENSET           (0x42004017U) /**< \brief (ADC) Interrupt Enable Set */
#define REG_ADC_INTFLAG            (0x42004018U) /**< \brief (ADC) Interrupt Flag Status and Clear */
#define REG_ADC_STATUS             (0x42004019U) /**< \brief (ADC) Status */
#define REG_ADC_RESULT             (0x4200401AU) /**< \brief (ADC) Result */
#define REG_ADC_WINLT              (0x4200401CU) /**< \brief (ADC) Window Monitor Lower Threshold */
#define REG_ADC_WINUT              (0x42004020U) /**< \brief (ADC) Window Monitor Upper Threshold */
#define REG_ADC_GAINCORR           (0x42004024U) /**< \brief (ADC) Gain Correction */
#define REG_ADC_OFFSETCORR         (0x42004026U) /**< \brief (ADC) Offset Correction */
#define REG_ADC_CALIB              (0x42004028U) /**< \brief (ADC) Calibration */
#define REG_ADC_DBGCTRL            (0x4200402AU) /**< \brief (ADC) Debug Control */
#else
#define REG_ADC_CTRLA              (*(RwReg8 *)0x42004000U) /**< \brief (ADC) Control A */
#define REG_ADC_REFCTRL            (*(RwReg8 *)0x42004001U) /**< \brief (ADC) Reference Control */
#define REG_ADC_AVGCTRL            (*(RwReg8 *)0x42004002U) /**< \brief (ADC) Average Control */
#define REG_ADC_SAMPCTRL           (*(RwReg8 *)0x42004003U) /**< \brief (ADC) Sampling Time Control */
#define REG_ADC_CTRLB              (*(RwReg16*)0x42004004U) /**< \brief (ADC) Control B */
#define REG_ADC_WINCTRL            (*(RwReg8 *)0x42004008U) /**< \brief (ADC) Window Monitor Control */
#define REG_ADC_SWTRIG             (*(RwReg8 *)0x4200400CU) /**< \brief (ADC) Software Trigger */
#define REG_ADC_INPUTCTRL          (*(RwReg  *)0x42004010U) /**< \brief (ADC) Input Control */
#define REG_ADC_EVCTRL             (*(RwReg8 *)0x42004014U) /**< \brief (ADC) Event Control */
#define REG_ADC_INTENCLR           (*(RwReg8 *)0x42004016U) /**< \brief (ADC) Interrupt Enable Clear */
#define REG_ADC_INTENSET           (*(RwReg8 *)0x42004017U) /**< \brief (ADC) Interrupt Enable Set */
#define REG_ADC_INTFLAG            (*(RwReg8 *)0x42004018U) /**< \brief (ADC) Interrupt Flag Status and Clear */
#define REG_ADC_STATUS             (*(RoReg8 *)0x42004019U) /**< \brief (ADC) Status */
#define REG_ADC_RESULT             (*(RoReg16*)0x4200401AU) /**< \brief (ADC) Result */
#define REG_ADC_WINLT              (*(RwReg16*)0x4200401CU) /**< \brief (ADC) Window Monitor Lower Threshold */
#define REG_ADC_WINUT              (*(RwReg16*)0x42004020U) /**< \brief (ADC) Window Monitor Upper Threshold */
#define REG_ADC_GAINCORR           (*(RwReg16*)0x42004024U) /**< \brief (ADC) Gain Correction */
#define REG_ADC_OFFSETCORR         (*(RwReg16*)0x42004026U) /**< \brief (ADC) Offset Correction */
#define REG_ADC_CALIB              (*(RwReg16*)0x42004028U) /**< \brief (ADC) Calibration */
#define REG_ADC_DBGCTRL            (*(RwReg8 *)0x4200402AU) /**< \brief (ADC) Debug Control */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for ADC peripheral ========== */
#define ADC_DMAC_ID_RESRDY          39       // Index of DMA RESRDY trigger
#define ADC_EXTCHANNEL_MSB          19       // Number of external channels
#define ADC_GCLK_ID                 30       // Index of Generic Clock
#define ADC_RESULT_BITS             16       // Size of RESULT.RESULT bitfield
#define ADC_RESULT_MSB              15       // Size of Result

#endif /* _SAMD21_ADC_INSTANCE_ */
