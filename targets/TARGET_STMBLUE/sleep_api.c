/*
 * sleep_api.h
 *
 *  Created on: 25 jan 2019
 *      Author: Antonio O.
 */

#if DEVICE_SLEEP

#define GPIO_WAKE_BIT_MASK   31    // IO13 | IO12 | IO11 | IO10 | IO9
#define GPIO_WAKE_LEVEL_MASK 0  // asserted pin in GPIO_WAKE_BIT_MASK are asserted at the value in the same position of this mask

#define SHPR3_REG 0xE000ED20

#define WAKENED_FROM_IO9          0x09
#define WAKENED_FROM_IO10         0x11
#define WAKENED_FROM_IO11         0x21
#define WAKENED_FROM_IO12         0x41
#define WAKENED_FROM_IO13         0x81
#define WAKENED_FROM_BLUE_TIMER1  0x101
#define WAKENED_FROM_BLUE_TIMER2  0x401

#define LOW_POWER_STANDBY  0x03

#define BLUE_CURRENT_TIME_REG 0x48000010

#include "sleep_api.h"
#include "bluenrg1_stack.h"
#include "misc.h"
#include "miscutil.h"


//BlueNRG2 sleepmodes types
typedef enum {
  SLEEPMODE_RUNNING       = 0,
  SLEEPMODE_CPU_HALT      = 1,
  SLEEPMODE_WAKETIMER     = 2,
  SLEEPMODE_NOTIMER       = 3,
} SleepModes;

uint32_t cStackPreamble[CSTACK_PREAMBLE_NUMBER];
volatile uint32_t* ptr ;

static void BlueNRG_HaltCPU(void){
	// Store the watchdog configuration and the disable it to avoid reset during CPU halt.
	uint32_t WDG_CR_saved = WDG->CR;
	WDG->CR = 0;

	// Wait for interrupt is called: the core execution is halted until an event occurs.
	__WFI();

	// Restore the watchdog functionality.
	WDG->CR= WDG_CR_saved;
}

static void BlueNRG_DeepSleep(SleepModes sleepMode, uint8_t gpioWakeBitMask)
{
  uint32_t savedCurrentTime, nvicPendingMask;
  PartInfoType partInfo;
  uint8_t i;
  /* System Control saved */
  uint32_t SYS_Ctrl_saved;
  /* NVIC Information Saved */
  uint32_t NVIC_ISER_saved, NVIC_IPR_saved[8], PENDSV_SYSTICK_IPR_saved;
  /* CKGEN SOC Enabled */
  uint32_t CLOCK_EN_saved;
  /* GPIO Information saved */
  uint32_t GPIO_DATA_saved, GPIO_OEN_saved, GPIO_PE_saved, GPIO_DS_saved, GPIO_IS_saved, GPIO_IBE_saved;
  uint32_t GPIO_IEV_saved, GPIO_IE_saved, GPIO_MODE0_saved, GPIO_MODE1_saved, GPIO_IOSEL_MFTX_saved;
#ifdef BLUENRG2_DEVICE
  uint32_t GPIO_MODE2_saved, GPIO_MODE3_saved;
#endif
  /* UART Information saved */
  uint32_t UART_TIMEOUT_saved, UART_LCRH_RX_saved, UART_IBRD_saved, UART_FBRD_saved;
  uint32_t UART_LCRH_TX_saved, UART_CR_saved, UART_IFLS_saved, UART_IMSC_saved;
  uint32_t UART_DMACR_saved, UART_XFCR_saved, UART_XON1_saved, UART_XON2_saved;
  uint32_t UART_XOFF1_saved, UART_XOFF2_saved;
  /* SPI Information saved */
  uint32_t SPI_CR0_saved, SPI_CR1_saved, SPI_CPSR_saved, SPI_IMSC_saved, SPI_DMACR_saved;
  uint32_t SPI_RXFRM_saved, SPI_CHN_saved, SPI_WDTXF_saved;
  /* I2C Information saved */
  uint32_t I2C_CR_saved[2], I2C_SCR_saved[2], I2C_TFTR_saved[2], I2C_RFTR_saved[2];
  uint32_t I2C_DMAR_saved[2], I2C_BRCR_saved[2], I2C_IMSCR_saved[2], I2C_THDDAT_saved[2];
  uint32_t I2C_THDSTA_FST_STD_saved[2], I2C_TSUSTA_FST_STD_saved[2];
  /* RNG Information saved */
  uint32_t RNG_CR_saved;
  /* SysTick Information saved */
  uint32_t SYST_CSR_saved, SYST_RVR_saved;
  /* RTC Information saved */
  uint32_t RTC_CWDMR_saved, RTC_CWDLR_saved, RTC_CWYMR_saved, RTC_CWYLR_saved, RTC_CTCR_saved;
  uint32_t RTC_IMSC_saved, RTC_TCR_saved, RTC_TLR1_saved, RTC_TLR2_saved, RTC_TPR1_saved;
  uint32_t RTC_TPR2_saved, RTC_TPR3_saved, RTC_TPR4_saved;
  /* MFTX Information saved */
  uint32_t T1CRA_saved, T1CRB_saved, T1PRSC_saved, T1CKC_saved, T1MCTRL_saved, T1ICTRL_saved;
  uint32_t T2CRA_saved, T2CRB_saved, T2PRSC_saved, T2CKC_saved, T2MCTRL_saved, T2ICTRL_saved;
  /* WDT Information saved */
  uint32_t WDG_LR_saved, WDG_CR_saved, WDG_LOCK_saved;
  /* DMA channel [0..7] Information saved */
  uint32_t DMA_CCR_saved[8], DMA_CNDTR_saved[8], DMA_CPAR_saved[8], DMA_CMAR[8];
  /* ADC Information saved */
  uint32_t ADC_CTRL_saved, ADC_CONF_saved, ADC_IRQMASK_saved, ADC_OFFSET_LSB_saved, ADC_OFFSET_MSB_saved;
  uint32_t ADC_THRESHOLD_HI_saved, ADC_THRESHOLD_LO_saved;
  /* FlASH Config saved */
  uint32_t FLASH_CONFIG_saved;
  /* PKA Information saved */
  uint32_t PKA_IEN_saved;

  /* Get partInfo */
  HAL_GetPartInfo(&partInfo);

  /* Save the peripherals configuration */
  /* System Control */
  SYS_Ctrl_saved = SYSTEM_CTRL->CTRL;
  /* FLASH CONFIG */
  FLASH_CONFIG_saved = FLASH->CONFIG;
  /* NVIC */
  NVIC_ISER_saved = NVIC->ISER[0];

  // Issue with Atollic compiler
//  memcpy(NVIC_IPR_saved, (void const *)NVIC->IP, sizeof(NVIC_IPR_saved));
  for (i=0; i<8; i++) {
  	NVIC_IPR_saved[i] = NVIC->IP[i];
  }


  PENDSV_SYSTICK_IPR_saved = *(volatile uint32_t *)SHPR3_REG;
  /* CKGEN SOC Enabled */
  CLOCK_EN_saved = CKGEN_SOC->CLOCK_EN;
  /* GPIO */
  GPIO_DATA_saved = GPIO->DATA;
  GPIO_OEN_saved = GPIO->OEN;
  GPIO_PE_saved = GPIO->PE;
  GPIO_DS_saved = GPIO->DS;
  GPIO_IS_saved = GPIO->IS;
  GPIO_IBE_saved = GPIO->IBE;
  GPIO_IEV_saved = GPIO->IEV;
  GPIO_IE_saved = GPIO->IE;
  GPIO_MODE0_saved = GPIO->MODE0;
  GPIO_MODE1_saved = GPIO->MODE1;
#ifdef BLUENRG2_DEVICE
  GPIO_MODE2_saved = GPIO->MODE2;
  GPIO_MODE3_saved = GPIO->MODE3;
#endif
  GPIO_IOSEL_MFTX_saved = GPIO->MFTX;
  /* UART */
  UART_TIMEOUT_saved = UART->TIMEOUT;
  UART_LCRH_RX_saved = UART->LCRH_RX;
  UART_IBRD_saved = UART->IBRD;
  UART_FBRD_saved = UART->FBRD;
  UART_LCRH_TX_saved =  UART->LCRH_TX;
  UART_CR_saved = UART->CR;
  UART_IFLS_saved = UART->IFLS;
  UART_IMSC_saved = UART->IMSC;
  UART_DMACR_saved = UART->DMACR;
  UART_XFCR_saved = UART->XFCR;
  UART_XON1_saved = UART->XON1;
  UART_XON2_saved = UART->XON2;
  UART_XOFF1_saved = UART->XOFF1;
  UART_XOFF2_saved = UART->XOFF2;
  /* SPI */
  SPI_CR0_saved = SPI1->CR0;
  SPI_CR1_saved = SPI1->CR1;
  SPI_CPSR_saved = SPI1->CPSR;
  SPI_IMSC_saved = SPI1->IMSC;
  SPI_DMACR_saved = SPI1->DMACR;
  SPI_RXFRM_saved = SPI1->RXFRM;
  SPI_CHN_saved = SPI1->CHN;
  SPI_WDTXF_saved = SPI1->WDTXF;
  /* I2C */
  for (i=0; i<2; i++) {
    I2C_Type *I2Cx = (I2C_Type*)(I2C2_BASE+ 0x100000*i);
    I2C_CR_saved[i] = I2Cx->CR;
    I2C_SCR_saved[i] = I2Cx->SCR;
    I2C_TFTR_saved[i] = I2Cx->TFTR;
    I2C_RFTR_saved[i] = I2Cx->RFTR;
    I2C_DMAR_saved[i] = I2Cx->DMAR;
    I2C_BRCR_saved[i] = I2Cx->BRCR;
    I2C_IMSCR_saved[i] = I2Cx->IMSCR;
    I2C_THDDAT_saved[i] = I2Cx->THDDAT;
    I2C_THDSTA_FST_STD_saved[i] = I2Cx->THDSTA_FST_STD;
    I2C_TSUSTA_FST_STD_saved[i] = I2Cx->TSUSTA_FST_STD;
  }
  /* RNG */
  RNG_CR_saved = RNG->CR;
  /* RTC */
  RTC_CWDMR_saved = RTC->CWDMR;
  RTC_CWDLR_saved = RTC->CWDLR;
  RTC_CWYMR_saved = RTC->CWYMR;
  RTC_CWYLR_saved = RTC->CWYLR;
  RTC_CTCR_saved = RTC->CTCR;
  RTC_IMSC_saved = RTC->IMSC;
  RTC_TCR_saved = RTC->TCR;
  RTC_TLR1_saved = RTC->TLR1;
  RTC_TLR2_saved = RTC->TLR2;
  RTC_TPR1_saved = RTC->TPR1;
  RTC_TPR2_saved = RTC->TPR2;
  RTC_TPR3_saved = RTC->TPR3;
  RTC_TPR4_saved = RTC->TPR4;
  /* MFTX */
  T1CRA_saved = MFT1->TNCRA;
  T1CRB_saved = MFT1->TNCRB;
  T1PRSC_saved = MFT1->TNPRSC;
  T1CKC_saved = MFT1->TNCKC;
  T1MCTRL_saved = MFT1->TNMCTRL;
  T1ICTRL_saved = MFT1->TNICTRL;
  T2CRA_saved = MFT2->TNCRA;
  T2CRB_saved = MFT2->TNCRB;
  T2PRSC_saved = MFT2->TNPRSC;
  T2CKC_saved = MFT2->TNCKC;
  T2MCTRL_saved = MFT2->TNMCTRL;
  T2ICTRL_saved = MFT2->TNICTRL;
  /* SysTick */
  SYST_CSR_saved = SysTick->CTRL;
  SYST_RVR_saved = SysTick->LOAD;
  /* WDT */
  WDG_LR_saved = WDG->LR;
  WDG_CR_saved = WDG->CR;
  if(WDG->LOCK == 0) {
    WDG_LOCK_saved = 0x1ACCE551;
  } else {
    WDG_LOCK_saved = 0;
  }
  /* DMA */
  for (i=0; i<8; i++) {
    DMA_CH_Type *DMAx = (DMA_CH_Type*)(DMA_CH0_BASE+ 0x14*i);
    DMA_CNDTR_saved[i] = DMAx->CNDTR;
    DMA_CCR_saved[i] = DMAx->CCR;
    DMA_CPAR_saved[i] = DMAx->CPAR;
    DMA_CMAR[i] = DMAx->CMAR;
  }
  /* ADC */
  ADC_CONF_saved = ADC->CONF;
  ADC_IRQMASK_saved = ADC->IRQMASK;
  ADC_OFFSET_MSB_saved = ADC->OFFSET_MSB;
  ADC_OFFSET_LSB_saved = ADC->OFFSET_LSB;
  ADC_THRESHOLD_HI_saved = ADC->THRESHOLD_HI;
  ADC_THRESHOLD_LO_saved = ADC->THRESHOLD_LO;
  ADC_CTRL_saved = ADC->CTRL;

  /* PKA */
  PKA_IEN_saved = PKA->IEN;

  // Enable the STANDBY mode
  if (sleepMode == SLEEPMODE_NOTIMER) {
    BLUE_CTRL->TIMEOUT |= LOW_POWER_STANDBY<<28;
  }

  //Save the CSTACK number of words that will be restored at wakeup reset
  i = 0;
  ptr = __vector_table[0].__ptr ;
  ptr -= CSTACK_PREAMBLE_NUMBER;
  do {
    cStackPreamble[i] = *ptr;
    i++;
    ptr++;
  } while (i < CSTACK_PREAMBLE_NUMBER);

  if (((partInfo.die_major<<4)|(partInfo.die_cut)) >= WA_DEVICE_VERSION) {
    /* Lock the flash */
    flash_sw_lock = FLASH_LOCK_WORD;
    /* Disable BOR */
    SET_BORconfigStatus(FALSE);
  }

  //Enable deep sleep
  SystemSleepCmd(ENABLE);
  //The __disable_irq() used at the beginning of the BlueNRG_Sleep() function
  //masks all the interrupts. The interrupts will be enabled at the end of the
  //context restore. Now induce a context save.
  void CS_contextSave(void);
  CS_contextSave();

  //Disable deep sleep, because if no reset occours for an interrrupt pending,
  //the register value remain set and if a simple CPU_HALT command is called from the
  //application the BlueNRG-1 enters in deep sleep without make a context save.
  //So, exiting from the deep sleep the context is restored with wrong random value.
  SystemSleepCmd(DISABLE);

  if (!wakeupFromSleepFlag) {
    if((NVIC->ISPR[0]&(1<<BLUE_CTRL_IRQn)) == 0) { //At this stage the Blue Control Interrupt shall not be pending.
                                                   //So, if this happens means that the application has called the
                                                   //BlueNRG_Sleep() API with the wakeup source already acrive.
                                                   //In this scenario we don't need to wait the 91 us, otherwise
                                                   //the radio activity will be compromised.
      nvicPendingMask = savedNVIC_ISPR ^ NVIC->ISPR[0];
      if ((savedSHCSR != SCB->SHCSR)  ||                                                                //Verified if a SVCall Interrupt is pending
          ((savedNVIC_ISPR != NVIC->ISPR[0]) && (nvicPendingMask & NVIC->ISER[0]))  ||                  //Verified if a NVIC Interrupt is pending
          ((savedICSR & 0x10000000) != (SCB->ICSR & 0x10000000)) ||                                   // Verified if a PendSV interrupt is pending
          (((savedICSR & 0x4000000) != (SCB->ICSR & 0x4000000)) && (SysTick->CTRL & 0x02))) {       // Verified if a SysTick interrupt is pending
        savedCurrentTime = (*(volatile uint32_t *)BLUE_CURRENT_TIME_REG) >> 4;
        if (0xFFFFF >= (savedCurrentTime+3)) { //Check if the counter are wrapping
          while ((savedCurrentTime+3) > ((*(volatile uint32_t *)BLUE_CURRENT_TIME_REG) >> 4)); //Not Wrap
        } else {
          while (((*(volatile uint32_t *)BLUE_CURRENT_TIME_REG) >> 4) != (savedCurrentTime + 3 - 0xFFFFF)); //Wrap
        }
      }
    }

    if (((partInfo.die_major<<4)|(partInfo.die_cut)) >= WA_DEVICE_VERSION) {
      /* Restore BOR configuration */
      SET_BORconfigStatus(TRUE);
      /* Unlock the flash */
      flash_sw_lock = FLASH_UNLOCK_WORD;
    }

    // Disable the STANDBY mode
    if (sleepMode == SLEEPMODE_NOTIMER) {
      BLUE_CTRL->TIMEOUT &= ~(LOW_POWER_STANDBY<<28);
    }

  } else {

    /* Start a new calibration, needed to signal if the HS is ready */
    CKGEN_BLE->CLK32K_IT = 1;
    CKGEN_BLE->CLK32K_COUNT = 0;
    CKGEN_BLE->CLK32K_PERIOD = 0;

    /* Restore the CSTACK number of words that will be saved before the sleep */
    i = 0;
    ptr = __vector_table[0].__ptr ;
    ptr -= CSTACK_PREAMBLE_NUMBER;
    do {
      *ptr = cStackPreamble[i];
      i++;
      ptr++;
    } while (i < CSTACK_PREAMBLE_NUMBER);


    /* Restore the peripherals configuration */
    /* FLASH CONFIG */
    FLASH->CONFIG = FLASH_CONFIG_saved;
    /* NVIC */
    NVIC->ISER[0] = NVIC_ISER_saved;

    for (i=0; i<8; i++) {
    	NVIC->IP[i] = NVIC_IPR_saved[i];
    }

    *(volatile uint32_t *)SHPR3_REG = PENDSV_SYSTICK_IPR_saved;
    /* CKGEN SOC Enabled */
    CKGEN_SOC->CLOCK_EN = CLOCK_EN_saved;
    /* GPIO */
    GPIO->DATA = GPIO_DATA_saved;
    GPIO->OEN = GPIO_OEN_saved;
    GPIO->PE = GPIO_PE_saved;
    GPIO->DS = GPIO_DS_saved;
    GPIO->IEV = GPIO_IEV_saved;
    GPIO->IBE = GPIO_IBE_saved;
    GPIO->IS = GPIO_IS_saved;
    GPIO->IC = GPIO_IE_saved;
    GPIO->IE = GPIO_IE_saved;
    GPIO->MODE0 = GPIO_MODE0_saved;
    GPIO->MODE1 = GPIO_MODE1_saved;
#ifdef BLUENRG2_DEVICE
    GPIO->MODE2 = GPIO_MODE2_saved;
    GPIO->MODE3 = GPIO_MODE3_saved;
#endif
    GPIO->MFTX = GPIO_IOSEL_MFTX_saved;
    /* UART */
    UART->TIMEOUT = UART_TIMEOUT_saved;
    UART->LCRH_RX = UART_LCRH_RX_saved;
    UART->IBRD = UART_IBRD_saved;
    UART->FBRD = UART_FBRD_saved;
    UART->LCRH_TX = UART_LCRH_TX_saved;
    UART->CR = UART_CR_saved;
    UART->IFLS = UART_IFLS_saved;
    UART->IMSC = UART_IMSC_saved;
    UART->DMACR = UART_DMACR_saved;
    UART->XFCR = UART_XFCR_saved;
    UART->XON1 = UART_XON1_saved;
    UART->XON2 = UART_XON2_saved;
    UART->XOFF1 = UART_XOFF1_saved;
    UART->XOFF2 = UART_XOFF2_saved;
    /* SPI */
    SPI1->CR0 = SPI_CR0_saved;
    SPI1->CR1 = SPI_CR1_saved;
    SPI1->CPSR = SPI_CPSR_saved;
    SPI1->IMSC = SPI_IMSC_saved;
    SPI1->DMACR = SPI_DMACR_saved;
    SPI1->RXFRM = SPI_RXFRM_saved;
    SPI1->CHN = SPI_CHN_saved;
    SPI1->WDTXF = SPI_WDTXF_saved;
    /* I2C */
    for (i=0; i<2; i++) {
      I2C_Type *I2Cx = (I2C_Type*)(I2C2_BASE+ 0x100000*i);
      I2Cx->CR = I2C_CR_saved[i];
      I2Cx->SCR = I2C_SCR_saved[i];
      I2Cx->TFTR = I2C_TFTR_saved[i];
      I2Cx->RFTR = I2C_RFTR_saved[i];
      I2Cx->DMAR = I2C_DMAR_saved[i];
      I2Cx->BRCR = I2C_BRCR_saved[i];
      I2Cx->IMSCR = I2C_IMSCR_saved[i];
      I2Cx->THDDAT = I2C_THDDAT_saved[i];
      I2Cx->THDSTA_FST_STD = I2C_THDSTA_FST_STD_saved[i];
      I2Cx->TSUSTA_FST_STD = I2C_TSUSTA_FST_STD_saved[i];
    }
    /* RNG */
    RNG->CR = RNG_CR_saved;
    /* SysTick */
    SysTick->LOAD = SYST_RVR_saved;
    SysTick->VAL = 0;
    SysTick->CTRL = SYST_CSR_saved;
    /* RTC */
    RTC->CWDMR = RTC_CWDMR_saved;
    RTC->CWDLR = RTC_CWDLR_saved;
    RTC->CWYMR = RTC_CWYMR_saved;
    RTC->CWYLR = RTC_CWYLR_saved;
    RTC->CTCR = RTC_CTCR_saved;
    RTC->IMSC = RTC_IMSC_saved;
    RTC->TLR1 = RTC_TLR1_saved;
    RTC->TLR2 = RTC_TLR2_saved;
    RTC->TPR1 = RTC_TPR1_saved;
    RTC->TPR2 = RTC_TPR2_saved;
    RTC->TPR3 = RTC_TPR3_saved;
    RTC->TPR4 = RTC_TPR4_saved;
    RTC->TCR = RTC_TCR_saved; /* Enable moved at the end of RTC configuration */
    /* MFTX */
    MFT1->TNCRA = T1CRA_saved;
    MFT1->TNCRB = T1CRB_saved;
    MFT1->TNPRSC = T1PRSC_saved;
    MFT1->TNCKC = T1CKC_saved;
    MFT1->TNMCTRL = T1MCTRL_saved;
    MFT1->TNICTRL = T1ICTRL_saved;
    MFT2->TNCRA = T2CRA_saved;
    MFT2->TNCRB = T2CRB_saved;
    MFT2->TNPRSC = T2PRSC_saved;
    MFT2->TNCKC = T2CKC_saved;
    MFT2->TNMCTRL = T2MCTRL_saved;
    MFT2->TNICTRL = T2ICTRL_saved;
    /* WDT */
    WDG->LR = WDG_LR_saved;
    WDG->CR = WDG_CR_saved;
    WDG->LOCK = WDG_LOCK_saved;
    /* DMA */
    for (i=0; i<8; i++) {
      DMA_CH_Type *DMAx = (DMA_CH_Type*)(DMA_CH0_BASE+ 0x14*i);
      DMAx->CNDTR = DMA_CNDTR_saved[i];
      DMAx->CCR = DMA_CCR_saved[i] ;
      DMAx->CPAR = DMA_CPAR_saved[i];
      DMAx->CMAR = DMA_CMAR[i];
    }
    /* ADC */
    ADC->CONF = ADC_CONF_saved;
    ADC->IRQMASK = ADC_IRQMASK_saved;
    ADC->OFFSET_MSB = ADC_OFFSET_MSB_saved;
    ADC->OFFSET_LSB = ADC_OFFSET_LSB_saved;
    ADC->THRESHOLD_HI = ADC_THRESHOLD_HI_saved;
    ADC->THRESHOLD_LO = ADC_THRESHOLD_LO_saved;
    ADC->CTRL = ADC_CTRL_saved;

    /* PKA */
    PKA->IEN = PKA_IEN_saved;
    //The five IRQs are linked to a real ISR. If any of the five IRQs
    //triggered, then pend their ISR
    //Capture the wake source from the BLE_REASON_RESET register
    if ((CKGEN_SOC->REASON_RST == 0) &&
        (CKGEN_BLE->REASON_RST >= WAKENED_FROM_IO9) &&
          (CKGEN_BLE->REASON_RST <= WAKENED_FROM_IO13) &&
            gpioWakeBitMask) {
              if ((((CKGEN_BLE->REASON_RST & WAKENED_FROM_IO9) == WAKENED_FROM_IO9) && (GPIO->IE & GPIO_Pin_9))   ||
                  (((CKGEN_BLE->REASON_RST & WAKENED_FROM_IO10) == WAKENED_FROM_IO10) && (GPIO->IE & GPIO_Pin_10)) ||
                    (((CKGEN_BLE->REASON_RST & WAKENED_FROM_IO11) == WAKENED_FROM_IO11) && (GPIO->IE & GPIO_Pin_11)) ||
                      (((CKGEN_BLE->REASON_RST & WAKENED_FROM_IO12) == WAKENED_FROM_IO12) && (GPIO->IE & GPIO_Pin_12)) ||
                        (((CKGEN_BLE->REASON_RST & WAKENED_FROM_IO13) == WAKENED_FROM_IO13) && (GPIO->IE & GPIO_Pin_13))) {
                          NVIC->ISPR[0] = 1<<GPIO_IRQn;
                        }
            }

    // Disable the STANDBY mode
    if (sleepMode == SLEEPMODE_NOTIMER) {
      BLUE_CTRL->TIMEOUT &= ~(LOW_POWER_STANDBY<<28);
    }

    /* Restore the System Control register to indicate which HS crystal is used */
    SYSTEM_CTRL->CTRL = SYS_Ctrl_saved;

    // Wait until the HS clock is ready.
    // If SLEEPMODE_NOTIMER is set, wait the LS clock is ready.
    if (sleepMode == SLEEPMODE_NOTIMER) {
      DeviceConfiguration(FALSE, TRUE);
    } else {
      DeviceConfiguration(FALSE, FALSE);
    }

    /* If the HS is a 32 MHz */
    if (SYS_Ctrl_saved & 1) {
#if (FORCE_CORE_TO_16MHZ == 1)
      /* AHB up converter command register write*/
      AHBUPCONV->COMMAND = 0x14;
#else
      /* AHB up converter command register write*/
      AHBUPCONV->COMMAND = 0x15;
#endif
    }
  }

  //We can clear PRIMASK to reenable global interrupt operation.
  //__enable_irq();  //done in hal_deepsleep
}

void hal_sleep(void){
	//only CPU halt, wakeup from any interrupt source
	//wakeup timer is not available

    // Disable IRQs
    core_util_critical_section_enter();

    //Flag to signal if a wakeup from standby or sleep occurred
    wakeupFromSleepFlag = 0;

    //ask the BLE controller if link layer termination is ongoing,
    //to go sleep at least it shall return at least SLEEPMODE_CPU_HALT
    SleepModes sleepMode_allowed = (SleepModes)BlueNRG_Stack_Perform_Deep_Sleep_Check();

    if(sleepMode_allowed >= SLEEPMODE_CPU_HALT){
    	BlueNRG_HaltCPU();
    }
	// Unmask all the interrupt
	core_util_critical_section_exit();
}

void hal_deepsleep(void){
	//check no active UART RX - when tx ongoing fifo empty flag is 0 (RESET)
#if DEVICE_SERIAL
	serialTxActive();
#endif

	// Disable IRQs
	core_util_critical_section_enter();


	//Flag to signal if a wakeup from standby or sleep occurred
	wakeupFromSleepFlag = 0;

    //ask the BLE controller if link layer termination is ongoing,
    //to go sleep at least it shall return at least SLEEPMODE_WAKETIMER - only GPIO wakeup available
    volatile SleepModes sleepMode_allowed = (SleepModes)BlueNRG_Stack_Perform_Deep_Sleep_Check();

    switch(sleepMode_allowed){
    case SLEEPMODE_CPU_HALT:
    	BlueNRG_HaltCPU();
    	break;
    case SLEEPMODE_WAKETIMER:
    case SLEEPMODE_NOTIMER:
    	//Setup the GPIO Wakeup Source
    	//sleepMode_allowed = SLEEPMODE_WAKETIMER;
    	SYSTEM_CTRL->WKP_IO_IS = GPIO_WAKE_LEVEL_MASK;
    	SYSTEM_CTRL->WKP_IO_IE = GPIO_WAKE_BIT_MASK;
    	BlueNRG_DeepSleep(sleepMode_allowed,GPIO_WAKE_BIT_MASK);
    	break;
    default:
    	break;
    }
	// Unmask all the interrupt
	core_util_critical_section_exit();
}

#endif //DEVICE_SLEEP
