/******************************************************************************
 * @brief    Register map for the boudica Apps core
 * Copyright (c) 2015 NEUL LIMITED
******************************************************************************/
#define RTC_IRQ_TIME_LSBS (*(volatile unsigned long *)(0x40002000))
#define RTC_IRQ_TIME_LSBS_BITSET (*(volatile unsigned long *)(0x40002400))
#define RTC_IRQ_TIME_LSBS_BITCLR (*(volatile unsigned long *)(0x40002800))
#define RTC_IRQ_TIME_LSBS_BITTOG (*(volatile unsigned long *)(0x40002C00))
#define RTC_IRQ_TIME_MSBS (*(volatile unsigned long *)(0x40002004))
#define RTC_IRQ_TIME_MSBS_BITSET (*(volatile unsigned long *)(0x40002404))
#define RTC_IRQ_TIME_MSBS_BITCLR (*(volatile unsigned long *)(0x40002804))
#define RTC_IRQ_TIME_MSBS_BITTOG (*(volatile unsigned long *)(0x40002C04))
#define RTC_IRQ_CLR (*(volatile unsigned long *)(0x40002008))
#define RTC_IRQ_CLR_BITSET (*(volatile unsigned long *)(0x40002408))
#define RTC_IRQ_CLR_BITCLR (*(volatile unsigned long *)(0x40002808))
#define RTC_IRQ_CLR_BITTOG (*(volatile unsigned long *)(0x40002C08))
#define TIMER0_LOAD (*(volatile unsigned long *)(0x4000200C))
#define TIMER0_LOAD_BITSET (*(volatile unsigned long *)(0x4000240C))
#define TIMER0_LOAD_BITCLR (*(volatile unsigned long *)(0x4000280C))
#define TIMER0_LOAD_BITTOG (*(volatile unsigned long *)(0x40002C0C))
#define TIMER0_CTRL (*(volatile unsigned long *)(0x40002010))
#define TIMER0_CTRL_BITSET (*(volatile unsigned long *)(0x40002410))
#define TIMER0_CTRL_BITCLR (*(volatile unsigned long *)(0x40002810))
#define TIMER0_CTRL_BITTOG (*(volatile unsigned long *)(0x40002C10))
#define TIMER0_TIME (*(volatile unsigned long *)(0x40002014))
#define ARM_IRQ_REG (*(volatile unsigned long *)(0x40002018))
#define ARM_IRQ_REG_BITSET (*(volatile unsigned long *)(0x40002418))
#define ARM_IRQ_REG_BITCLR (*(volatile unsigned long *)(0x40002818))
#define ARM_IRQ_REG_BITTOG (*(volatile unsigned long *)(0x40002C18))
#define PIO_FUNC0 (*(volatile unsigned long *)(0x4000201C))
#define PIO_FUNC0_BITSET (*(volatile unsigned long *)(0x4000241C))
#define PIO_FUNC0_BITCLR (*(volatile unsigned long *)(0x4000281C))
#define PIO_FUNC0_BITTOG (*(volatile unsigned long *)(0x40002C1C))
#define PIO_FUNC1 (*(volatile unsigned long *)(0x40002020))
#define PIO_FUNC1_BITSET (*(volatile unsigned long *)(0x40002420))
#define PIO_FUNC1_BITCLR (*(volatile unsigned long *)(0x40002820))
#define PIO_FUNC1_BITTOG (*(volatile unsigned long *)(0x40002C20))
#define PIO_FUNC2 (*(volatile unsigned long *)(0x40002024))
#define PIO_FUNC2_BITSET (*(volatile unsigned long *)(0x40002424))
#define PIO_FUNC2_BITCLR (*(volatile unsigned long *)(0x40002824))
#define PIO_FUNC2_BITTOG (*(volatile unsigned long *)(0x40002C24))
#define PIO_FUNC3 (*(volatile unsigned long *)(0x40002028))
#define PIO_FUNC3_BITSET (*(volatile unsigned long *)(0x40002428))
#define PIO_FUNC3_BITCLR (*(volatile unsigned long *)(0x40002828))
#define PIO_FUNC3_BITTOG (*(volatile unsigned long *)(0x40002C28))
#define PIO_FUNC4 (*(volatile unsigned long *)(0x4000202C))
#define PIO_FUNC4_BITSET (*(volatile unsigned long *)(0x4000242C))
#define PIO_FUNC4_BITCLR (*(volatile unsigned long *)(0x4000282C))
#define PIO_FUNC4_BITTOG (*(volatile unsigned long *)(0x40002C2C))
#define GPIO_DIR (*(volatile unsigned long *)(0x40002030))
#define GPIO_DIR_BITSET (*(volatile unsigned long *)(0x40002430))
#define GPIO_DIR_BITCLR (*(volatile unsigned long *)(0x40002830))
#define GPIO_DIR_BITTOG (*(volatile unsigned long *)(0x40002C30))
#define GPIO_OUT (*(volatile unsigned long *)(0x40002034))
#define GPIO_OUT_BITSET (*(volatile unsigned long *)(0x40002434))
#define GPIO_OUT_BITCLR (*(volatile unsigned long *)(0x40002834))
#define GPIO_OUT_BITTOG (*(volatile unsigned long *)(0x40002C34))
#define GPIO_DRIVE (*(volatile unsigned long *)(0x40002038))
#define GPIO_DRIVE_BITSET (*(volatile unsigned long *)(0x40002438))
#define GPIO_DRIVE_BITCLR (*(volatile unsigned long *)(0x40002838))
#define GPIO_DRIVE_BITTOG (*(volatile unsigned long *)(0x40002C38))
#define GPIO_PULLEN (*(volatile unsigned long *)(0x4000203C))
#define GPIO_PULLEN_BITSET (*(volatile unsigned long *)(0x4000243C))
#define GPIO_PULLEN_BITCLR (*(volatile unsigned long *)(0x4000283C))
#define GPIO_PULLEN_BITTOG (*(volatile unsigned long *)(0x40002C3C))
#define GPIO_INT_RISE (*(volatile unsigned long *)(0x40002040))
#define GPIO_INT_RISE_BITSET (*(volatile unsigned long *)(0x40002440))
#define GPIO_INT_RISE_BITCLR (*(volatile unsigned long *)(0x40002840))
#define GPIO_INT_RISE_BITTOG (*(volatile unsigned long *)(0x40002C40))
#define GPIO_INT_FALL (*(volatile unsigned long *)(0x40002044))
#define GPIO_INT_FALL_BITSET (*(volatile unsigned long *)(0x40002444))
#define GPIO_INT_FALL_BITCLR (*(volatile unsigned long *)(0x40002844))
#define GPIO_INT_FALL_BITTOG (*(volatile unsigned long *)(0x40002C44))
#define GPIO_INT_HIGH (*(volatile unsigned long *)(0x40002048))
#define GPIO_INT_HIGH_BITSET (*(volatile unsigned long *)(0x40002448))
#define GPIO_INT_HIGH_BITCLR (*(volatile unsigned long *)(0x40002848))
#define GPIO_INT_HIGH_BITTOG (*(volatile unsigned long *)(0x40002C48))
#define GPIO_INT_LOW (*(volatile unsigned long *)(0x4000204C))
#define GPIO_INT_LOW_BITSET (*(volatile unsigned long *)(0x4000244C))
#define GPIO_INT_LOW_BITCLR (*(volatile unsigned long *)(0x4000284C))
#define GPIO_INT_LOW_BITTOG (*(volatile unsigned long *)(0x40002C4C))
#define GPIO_INT_CLR (*(volatile unsigned long *)(0x40002050))
#define GPIO_INT_CLR_BITSET (*(volatile unsigned long *)(0x40002450))
#define GPIO_INT_CLR_BITCLR (*(volatile unsigned long *)(0x40002850))
#define GPIO_INT_CLR_BITTOG (*(volatile unsigned long *)(0x40002C50))
#define GPIO_VALUE (*(volatile unsigned long *)(0x40002054))
#define GPIO_IRQ (*(volatile unsigned long *)(0x40002058))
#define WDT_INTERVAL (*(volatile unsigned long *)(0x4000205C))
#define WDT_INTERVAL_BITSET (*(volatile unsigned long *)(0x4000245C))
#define WDT_INTERVAL_BITCLR (*(volatile unsigned long *)(0x4000285C))
#define WDT_INTERVAL_BITTOG (*(volatile unsigned long *)(0x40002C5C))
#define WDT_CTRL (*(volatile unsigned long *)(0x40002060))
#define WDT_CTRL_BITSET (*(volatile unsigned long *)(0x40002460))
#define WDT_CTRL_BITCLR (*(volatile unsigned long *)(0x40002860))
#define WDT_CTRL_BITTOG (*(volatile unsigned long *)(0x40002C60))
#define WDT_TIME (*(volatile unsigned long *)(0x40002064))
#define RESET_CAUSE (*(volatile unsigned long *)(0x40002134))
#define PWM0_CTRL (*(volatile unsigned long *)(0x40002068))
#define PWM0_CTRL_BITSET (*(volatile unsigned long *)(0x40002468))
#define PWM0_CTRL_BITCLR (*(volatile unsigned long *)(0x40002868))
#define PWM0_CTRL_BITTOG (*(volatile unsigned long *)(0x40002C68))
#define PWM0_COUNT (*(volatile unsigned long *)(0x4000206C))
#define PWM0_COUNT_BITSET (*(volatile unsigned long *)(0x4000246C))
#define PWM0_COUNT_BITCLR (*(volatile unsigned long *)(0x4000286C))
#define PWM0_COUNT_BITTOG (*(volatile unsigned long *)(0x40002C6C))
#define PWM1_CTRL (*(volatile unsigned long *)(0x40002070))
#define PWM1_CTRL_BITSET (*(volatile unsigned long *)(0x40002470))
#define PWM1_CTRL_BITCLR (*(volatile unsigned long *)(0x40002870))
#define PWM1_CTRL_BITTOG (*(volatile unsigned long *)(0x40002C70))
#define PWM1_COUNT (*(volatile unsigned long *)(0x40002074))
#define PWM1_COUNT_BITSET (*(volatile unsigned long *)(0x40002474))
#define PWM1_COUNT_BITCLR (*(volatile unsigned long *)(0x40002874))
#define PWM1_COUNT_BITTOG (*(volatile unsigned long *)(0x40002C74))
#define PWM_STATUS (*(volatile unsigned long *)(0x40002078))
#define CLKEN_REG (*(volatile unsigned long *)(0x4000207C))
#define CLKEN_REG_BITSET (*(volatile unsigned long *)(0x4000247C))
#define CLKEN_REG_BITCLR (*(volatile unsigned long *)(0x4000287C))
#define CLKEN_REG_BITTOG (*(volatile unsigned long *)(0x40002C7C))
#define I2C_INTERRUPT_STATUS (*(volatile unsigned long *)(0x40002080))
#define I2C_INTERRUPT_CLEAR (*(volatile unsigned long *)(0x40002084))
#define I2C_INTERRUPT_CLEAR_BITSET (*(volatile unsigned long *)(0x40002484))
#define I2C_INTERRUPT_CLEAR_BITCLR (*(volatile unsigned long *)(0x40002884))
#define I2C_INTERRUPT_CLEAR_BITTOG (*(volatile unsigned long *)(0x40002C84))
#define I2C_INTERRUPT_ENABLE (*(volatile unsigned long *)(0x40002088))
#define I2C_INTERRUPT_ENABLE_BITSET (*(volatile unsigned long *)(0x40002488))
#define I2C_INTERRUPT_ENABLE_BITCLR (*(volatile unsigned long *)(0x40002888))
#define I2C_INTERRUPT_ENABLE_BITTOG (*(volatile unsigned long *)(0x40002C88))
#define I2C_MODE (*(volatile unsigned long *)(0x4000208C))
#define I2C_MODE_BITSET (*(volatile unsigned long *)(0x4000248C))
#define I2C_MODE_BITCLR (*(volatile unsigned long *)(0x4000288C))
#define I2C_MODE_BITTOG (*(volatile unsigned long *)(0x40002C8C))
#define I2C_TX_DATA (*(volatile unsigned long *)(0x40002090))
#define I2C_TX_DATA_BITSET (*(volatile unsigned long *)(0x40002490))
#define I2C_TX_DATA_BITCLR (*(volatile unsigned long *)(0x40002890))
#define I2C_TX_DATA_BITTOG (*(volatile unsigned long *)(0x40002C90))
#define I2C_RX_DATA (*(volatile unsigned long *)(0x40002144))
#define I2C_TX_RD_WRB (*(volatile unsigned long *)(0x40002094))
#define I2C_TX_RD_WRB_BITSET (*(volatile unsigned long *)(0x40002494))
#define I2C_TX_RD_WRB_BITCLR (*(volatile unsigned long *)(0x40002894))
#define I2C_TX_RD_WRB_BITTOG (*(volatile unsigned long *)(0x40002C94))
#define I2C_TX_NO_BYTES (*(volatile unsigned long *)(0x40002098))
#define I2C_TX_NO_BYTES_BITSET (*(volatile unsigned long *)(0x40002498))
#define I2C_TX_NO_BYTES_BITCLR (*(volatile unsigned long *)(0x40002898))
#define I2C_TX_NO_BYTES_BITTOG (*(volatile unsigned long *)(0x40002C98))
#define I2C_RX_NO_BYTES (*(volatile unsigned long *)(0x4000209C))
#define I2C_RX_NO_BYTES_MASTER (*(volatile unsigned long *)(0x400020A0))
#define I2C_RX_NO_BYTES_MASTER_BITSET (*(volatile unsigned long *)(0x400024A0))
#define I2C_RX_NO_BYTES_MASTER_BITCLR (*(volatile unsigned long *)(0x400028A0))
#define I2C_RX_NO_BYTES_MASTER_BITTOG (*(volatile unsigned long *)(0x40002CA0))
#define I2C_GO (*(volatile unsigned long *)(0x400020A4))
#define I2C_GO_BITSET (*(volatile unsigned long *)(0x400024A4))
#define I2C_GO_BITCLR (*(volatile unsigned long *)(0x400028A4))
#define I2C_GO_BITTOG (*(volatile unsigned long *)(0x40002CA4))
#define I2C_RX_EARLY_THRESHOLD (*(volatile unsigned long *)(0x400020A8))
#define I2C_RX_EARLY_THRESHOLD_BITSET (*(volatile unsigned long *)(0x400024A8))
#define I2C_RX_EARLY_THRESHOLD_BITCLR (*(volatile unsigned long *)(0x400028A8))
#define I2C_RX_EARLY_THRESHOLD_BITTOG (*(volatile unsigned long *)(0x40002CA8))
#define I2C_RX_AUTO_NAG_BYTE_CNT (*(volatile unsigned long *)(0x400020AC))
#define I2C_RX_AUTO_NAG_BYTE_CNT_BITSET (*(volatile unsigned long *)(0x400024AC))
#define I2C_RX_AUTO_NAG_BYTE_CNT_BITCLR (*(volatile unsigned long *)(0x400028AC))
#define I2C_RX_AUTO_NAG_BYTE_CNT_BITTOG (*(volatile unsigned long *)(0x40002CAC))
#define I2C_HALF_TIME (*(volatile unsigned long *)(0x400020B0))
#define I2C_HALF_TIME_BITSET (*(volatile unsigned long *)(0x400024B0))
#define I2C_HALF_TIME_BITCLR (*(volatile unsigned long *)(0x400028B0))
#define I2C_HALF_TIME_BITTOG (*(volatile unsigned long *)(0x40002CB0))
#define I2C_ADDRESS (*(volatile unsigned long *)(0x400020B4))
#define I2C_ADDRESS_BITSET (*(volatile unsigned long *)(0x400024B4))
#define I2C_ADDRESS_BITCLR (*(volatile unsigned long *)(0x400028B4))
#define I2C_ADDRESS_BITTOG (*(volatile unsigned long *)(0x40002CB4))
#define I2C_ADDR_TYPE (*(volatile unsigned long *)(0x400020B8))
#define I2C_ADDR_TYPE_BITSET (*(volatile unsigned long *)(0x400024B8))
#define I2C_ADDR_TYPE_BITCLR (*(volatile unsigned long *)(0x400028B8))
#define I2C_ADDR_TYPE_BITTOG (*(volatile unsigned long *)(0x40002CB8))
#define I2C_SOFT_RESET (*(volatile unsigned long *)(0x400020BC))
#define I2C_SOFT_RESET_BITSET (*(volatile unsigned long *)(0x400024BC))
#define I2C_SOFT_RESET_BITCLR (*(volatile unsigned long *)(0x400028BC))
#define I2C_SOFT_RESET_BITTOG (*(volatile unsigned long *)(0x40002CBC))
#define I2C_SLAVE_RWB (*(volatile unsigned long *)(0x400020C0))
#define I2C_MASTER_SM (*(volatile unsigned long *)(0x400020C4))
#define I2C_SLAVE_SM (*(volatile unsigned long *)(0x400020C8))
#define I2C_SLAVE_ENABLE (*(volatile unsigned long *)(0x400020CC))
#define I2C_SLAVE_ENABLE_BITSET (*(volatile unsigned long *)(0x400024CC))
#define I2C_SLAVE_ENABLE_BITCLR (*(volatile unsigned long *)(0x400028CC))
#define I2C_SLAVE_ENABLE_BITTOG (*(volatile unsigned long *)(0x40002CCC))
#define I2C_MASTER_SEND_RESTART (*(volatile unsigned long *)(0x400020D0))
#define I2C_MASTER_SEND_RESTART_BITSET (*(volatile unsigned long *)(0x400024D0))
#define I2C_MASTER_SEND_RESTART_BITCLR (*(volatile unsigned long *)(0x400028D0))
#define I2C_MASTER_SEND_RESTART_BITTOG (*(volatile unsigned long *)(0x40002CD0))
#define DMA_MUX (*(volatile unsigned long *)(0x400020D4))
#define DMA_MUX_BITSET (*(volatile unsigned long *)(0x400024D4))
#define DMA_MUX_BITCLR (*(volatile unsigned long *)(0x400028D4))
#define DMA_MUX_BITTOG (*(volatile unsigned long *)(0x40002CD4))
#define DMA_CTRL_STAT (*(volatile unsigned long *)(0x400020D8))
#define COMP_CTRL (*(volatile unsigned long *)(0x400020DC))
#define COMP_CTRL_BITSET (*(volatile unsigned long *)(0x400024DC))
#define COMP_CTRL_BITCLR (*(volatile unsigned long *)(0x400028DC))
#define COMP_CTRL_BITTOG (*(volatile unsigned long *)(0x40002CDC))
#define COMP_STAT (*(volatile unsigned long *)(0x400020E0))
#define LP_UART_CTRL (*(volatile unsigned long *)(0x400020E4))
#define LP_UART_CTRL_BITSET (*(volatile unsigned long *)(0x400024E4))
#define LP_UART_CTRL_BITCLR (*(volatile unsigned long *)(0x400028E4))
#define LP_UART_CTRL_BITTOG (*(volatile unsigned long *)(0x40002CE4))
#define LP_UART_STATUS (*(volatile unsigned long *)(0x400020E8))
#define LP_UART_DATA (*(volatile unsigned long *)(0x40002154))
#define CAP_FILT_CONF (*(volatile unsigned long *)(0x400020EC))
#define CAP_FILT_CONF_BITSET (*(volatile unsigned long *)(0x400024EC))
#define CAP_FILT_CONF_BITCLR (*(volatile unsigned long *)(0x400028EC))
#define CAP_FILT_CONF_BITTOG (*(volatile unsigned long *)(0x40002CEC))
#define CAP_IRQ_CONF (*(volatile unsigned long *)(0x400020F0))
#define CAP_IRQ_CONF_BITSET (*(volatile unsigned long *)(0x400024F0))
#define CAP_IRQ_CONF_BITCLR (*(volatile unsigned long *)(0x400028F0))
#define CAP_IRQ_CONF_BITTOG (*(volatile unsigned long *)(0x40002CF0))
#define CAP_STATUS (*(volatile unsigned long *)(0x400020F4))
#define CORE_ENABLE_SWD_ACCESS_APPS (*(volatile unsigned long *)(0x400020F8))
#define CORE_ENABLE_SWD_ACCESS_APPS_BITSET (*(volatile unsigned long *)(0x400024F8))
#define CORE_ENABLE_SWD_ACCESS_APPS_BITCLR (*(volatile unsigned long *)(0x400028F8))
#define CORE_ENABLE_SWD_ACCESS_APPS_BITTOG (*(volatile unsigned long *)(0x40002CF8))
#define APPS_DEBUGGER_TO_CORE_DATA (*(volatile unsigned long *)(0x400020FC))
#define APPS_CORE_TO_DEBUGGER_DATA (*(volatile unsigned long *)(0x40002100))
#define APPS_CORE_TO_DEBUGGER_DATA_BITSET (*(volatile unsigned long *)(0x40002500))
#define APPS_CORE_TO_DEBUGGER_DATA_BITCLR (*(volatile unsigned long *)(0x40002900))
#define APPS_CORE_TO_DEBUGGER_DATA_BITTOG (*(volatile unsigned long *)(0x40002D00))
#define APPS_DEBUG_DATA_TO_CORE_AVAILABLE (*(volatile unsigned long *)(0x40002104))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED (*(volatile unsigned long *)(0x40002108))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED_BITSET (*(volatile unsigned long *)(0x40002508))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED_BITCLR (*(volatile unsigned long *)(0x40002908))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED_BITTOG (*(volatile unsigned long *)(0x40002D08))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE (*(volatile unsigned long *)(0x4000210C))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE_BITSET (*(volatile unsigned long *)(0x4000250C))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE_BITCLR (*(volatile unsigned long *)(0x4000290C))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE_BITTOG (*(volatile unsigned long *)(0x40002D0C))
#define APPS_CORE_DATA_TO_DEBUGGER_ACCEPTED (*(volatile unsigned long *)(0x40002110))
#define SWD_REQUEST (*(volatile unsigned long *)(0x40002114))
#define EDGE_CTRL0 (*(volatile unsigned long *)(0x40002118))
#define EDGE_CTRL0_BITSET (*(volatile unsigned long *)(0x40002518))
#define EDGE_CTRL0_BITCLR (*(volatile unsigned long *)(0x40002918))
#define EDGE_CTRL0_BITTOG (*(volatile unsigned long *)(0x40002D18))
#define EDGE_CTRL1 (*(volatile unsigned long *)(0x4000211C))
#define EDGE_CTRL1_BITSET (*(volatile unsigned long *)(0x4000251C))
#define EDGE_CTRL1_BITCLR (*(volatile unsigned long *)(0x4000291C))
#define EDGE_CTRL1_BITTOG (*(volatile unsigned long *)(0x40002D1C))
#define EDGE_COUNT (*(volatile unsigned long *)(0x40002120))
#define RESET_REG (*(volatile unsigned long *)(0x40002124))
#define RESET_REG_BITSET (*(volatile unsigned long *)(0x40002524))
#define RESET_REG_BITCLR (*(volatile unsigned long *)(0x40002924))
#define RESET_REG_BITTOG (*(volatile unsigned long *)(0x40002D24))

/*typedef struct {
   uint32 RTC_IRQ_TIME_LSBS;
   uint32 RTC_IRQ_TIME_MSBS;
   uint32 RTC_IRQ_CLR;
   uint32 TIMER0_LOAD;
   uint32 TIMER0_CTRL;
   uint32 TIMER0_TIME;
   uint32 ARM_IRQ_REG;
   uint32 PIO_FUNC0;
   uint32 PIO_FUNC1;
   uint32 PIO_FUNC2;
   uint32 PIO_FUNC3;
   uint32 PIO_FUNC4;
   uint32 GPIO_DIR;
   uint32 GPIO_OUT;
   uint32 GPIO_DRIVE;
   uint32 GPIO_PULLEN;
   uint32 GPIO_INT_RISE;
   uint32 GPIO_INT_FALL;
   uint32 GPIO_INT_HIGH;
   uint32 GPIO_INT_LOW;
   uint32 GPIO_INT_CLR;
   uint32 GPIO_VALUE;
   uint32 GPIO_IRQ;
   uint32 WDT_INTERVAL;
   uint32 WDT_CTRL;
   uint32 WDT_TIME;
   uint32 PWM0_CTRL;
   uint32 PWM0_COUNT;
   uint32 PWM1_CTRL;
   uint32 PWM1_COUNT;
   uint32 PWM_STATUS;
   uint32 CLKEN_REG;
   uint32 I2C_INTERRUPT_STATUS;
   uint32 I2C_INTERRUPT_CLEAR;
   uint32 I2C_INTERRUPT_ENABLE;
   uint32 I2C_MODE;
   uint32 I2C_TX_DATA;
   uint32 I2C_TX_RD_WRB;
   uint32 I2C_TX_NO_BYTES;
   uint32 I2C_RX_NO_BYTES;
   uint32 I2C_RX_NO_BYTES_MASTER;
   uint32 I2C_GO;
   uint32 I2C_RX_EARLY_THRESHOLD;
   uint32 I2C_RX_AUTO_NAG_BYTE_CNT;
   uint32 I2C_HALF_TIME;
   uint32 I2C_ADDRESS;
   uint32 I2C_ADDR_TYPE;
   uint32 I2C_SOFT_RESET;
   uint32 I2C_SLAVE_RWB;
   uint32 I2C_MASTER_SM;
   uint32 I2C_SLAVE_SM;
   uint32 I2C_SLAVE_ENABLE;
   uint32 I2C_MASTER_SEND_RESTART;
   uint32 DMA_MUX;
   uint32 DMA_CTRL_STAT;
   uint32 COMP_CTRL;
   uint32 COMP_STAT;
   uint32 LP_UART_CTRL;
   uint32 LP_UART_STATUS;
   uint32 CAP_FILT_CONF;
   uint32 CAP_IRQ_CONF;
   uint32 CAP_STATUS;
   uint32 CORE_ENABLE_SWD_ACCESS_APPS;
   uint32 APPS_DEBUGGER_TO_CORE_DATA;
   uint32 APPS_CORE_TO_DEBUGGER_DATA;
   uint32 APPS_DEBUG_DATA_TO_CORE_AVAILABLE;
   uint32 APPS_DEBUG_DATA_TO_CORE_ACCEPTED;
   uint32 APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE;
   uint32 APPS_CORE_DATA_TO_DEBUGGER_ACCEPTED;
   uint32 SWD_REQUEST;
   uint32 EDGE_CTRL0;
   uint32 EDGE_CTRL1;
   uint32 EDGE_COUNT;
   uint32 RESET_REG;
   uint32 empty0;
   uint32 empty1;
   uint32 empty2;
   uint32 RESET_CAUSE;
   uint32 empty3;
   uint32 empty4;
   uint32 empty5;
   uint32 I2C_RX_DATA;
   uint32 empty6;
   uint32 empty7;
   uint32 empty8;
   uint32 LP_UART_DATA;
} apps_regs_t;*/

// man_regs register definitions
#define DIGITAL_VERSION (*(volatile unsigned long *)(0x40000000))
#define CLK_FREQ_DAC (*(volatile unsigned long *)(0x40000004))
#define CLK_FREQ_SET (*(volatile unsigned long *)(0x40000008))
#define CLK_FREQ_SET_BITSET (*(volatile unsigned long *)(0x40000408))
#define CLK_FREQ_SET_BITCLR (*(volatile unsigned long *)(0x40000808))
#define CLK_FREQ_SET_BITTOG (*(volatile unsigned long *)(0x40000C08))
#define CLK_FREQ_NREFCLKS (*(volatile unsigned long *)(0x4000000C))
#define CLK_FREQ_NREFCLKS_BITSET (*(volatile unsigned long *)(0x4000040C))
#define CLK_FREQ_NREFCLKS_BITCLR (*(volatile unsigned long *)(0x4000080C))
#define CLK_FREQ_NREFCLKS_BITTOG (*(volatile unsigned long *)(0x40000C0C))
#define CLK_FREQ_REF_SEL (*(volatile unsigned long *)(0x40000010))
#define CLK_FREQ_REF_SEL_BITSET (*(volatile unsigned long *)(0x40000410))
#define CLK_FREQ_REF_SEL_BITCLR (*(volatile unsigned long *)(0x40000810))
#define CLK_FREQ_REF_SEL_BITTOG (*(volatile unsigned long *)(0x40000C10))
#define CLK_FREQ_DIG_CLKS (*(volatile unsigned long *)(0x40000014))
#define CLK_FREQ_HIGHTARGET (*(volatile unsigned long *)(0x40000018))
#define CLK_FREQ_HIGHTARGET_BITSET (*(volatile unsigned long *)(0x40000418))
#define CLK_FREQ_HIGHTARGET_BITCLR (*(volatile unsigned long *)(0x40000818))
#define CLK_FREQ_HIGHTARGET_BITTOG (*(volatile unsigned long *)(0x40000C18))
#define CLK_FREQ_LOWTARGET (*(volatile unsigned long *)(0x4000001C))
#define CLK_FREQ_LOWTARGET_BITSET (*(volatile unsigned long *)(0x4000041C))
#define CLK_FREQ_LOWTARGET_BITCLR (*(volatile unsigned long *)(0x4000081C))
#define CLK_FREQ_LOWTARGET_BITTOG (*(volatile unsigned long *)(0x40000C1C))
#define CLK_FREQ_LP_BACKOFF (*(volatile unsigned long *)(0x40000020))
#define CLK_FREQ_LP_BACKOFF_BITSET (*(volatile unsigned long *)(0x40000420))
#define CLK_FREQ_LP_BACKOFF_BITCLR (*(volatile unsigned long *)(0x40000820))
#define CLK_FREQ_LP_BACKOFF_BITTOG (*(volatile unsigned long *)(0x40000C20))
#define CLK_FREQ_ENABLE (*(volatile unsigned long *)(0x40000024))
#define CLK_FREQ_ENABLE_BITSET (*(volatile unsigned long *)(0x40000424))
#define CLK_FREQ_ENABLE_BITCLR (*(volatile unsigned long *)(0x40000824))
#define CLK_FREQ_ENABLE_BITTOG (*(volatile unsigned long *)(0x40000C24))
#define CLK_GATE_SYS (*(volatile unsigned long *)(0x40000028))
#define CLK_GATE_SYS_BITSET (*(volatile unsigned long *)(0x40000428))
#define CLK_GATE_SYS_BITCLR (*(volatile unsigned long *)(0x40000828))
#define CLK_GATE_SYS_BITTOG (*(volatile unsigned long *)(0x40000C28))
#define CLK_GATE_MODEM (*(volatile unsigned long *)(0x4000002C))
#define CLK_GATE_MODEM_BITSET (*(volatile unsigned long *)(0x4000042C))
#define CLK_GATE_MODEM_BITCLR (*(volatile unsigned long *)(0x4000082C))
#define CLK_GATE_MODEM_BITTOG (*(volatile unsigned long *)(0x40000C2C))
#define CLK_GATE_RADIO (*(volatile unsigned long *)(0x40000030))
#define CLK_GATE_RADIO_BITSET (*(volatile unsigned long *)(0x40000430))
#define CLK_GATE_RADIO_BITCLR (*(volatile unsigned long *)(0x40000830))
#define CLK_GATE_RADIO_BITTOG (*(volatile unsigned long *)(0x40000C30))
#define CLK_GATE_DEBUG (*(volatile unsigned long *)(0x40000034))
#define CLK_GATE_DEBUG_BITSET (*(volatile unsigned long *)(0x40000434))
#define CLK_GATE_DEBUG_BITCLR (*(volatile unsigned long *)(0x40000834))
#define CLK_GATE_DEBUG_BITTOG (*(volatile unsigned long *)(0x40000C34))
#define CLK_GATE_RBIST (*(volatile unsigned long *)(0x40000038))
#define CLK_GATE_RBIST_BITSET (*(volatile unsigned long *)(0x40000438))
#define CLK_GATE_RBIST_BITCLR (*(volatile unsigned long *)(0x40000838))
#define CLK_GATE_RBIST_BITTOG (*(volatile unsigned long *)(0x40000C38))
#define LPC_CTRL (*(volatile unsigned long *)(0x4000003C))
#define LPC_CTRL_BITSET (*(volatile unsigned long *)(0x4000043C))
#define LPC_CTRL_BITCLR (*(volatile unsigned long *)(0x4000083C))
#define LPC_CTRL_BITTOG (*(volatile unsigned long *)(0x40000C3C))
#define LPC_TEST (*(volatile unsigned long *)(0x40000040))
#define LPC_TEST_BITSET (*(volatile unsigned long *)(0x40000440))
#define LPC_TEST_BITCLR (*(volatile unsigned long *)(0x40000840))
#define LPC_TEST_BITTOG (*(volatile unsigned long *)(0x40000C40))
#define FPGA_FLASH_WR (*(volatile unsigned long *)(0x40000044))
#define FPGA_FLASH_WR_BITSET (*(volatile unsigned long *)(0x40000444))
#define FPGA_FLASH_WR_BITCLR (*(volatile unsigned long *)(0x40000844))
#define FPGA_FLASH_WR_BITTOG (*(volatile unsigned long *)(0x40000C44))
#define FPGA_FLASH_RD (*(volatile unsigned long *)(0x40000048))
#define PMU_CTRL (*(volatile unsigned long *)(0x4000004C))
#define PMU_CTRL_BITSET (*(volatile unsigned long *)(0x4000044C))
#define PMU_CTRL_BITCLR (*(volatile unsigned long *)(0x4000084C))
#define PMU_CTRL_BITTOG (*(volatile unsigned long *)(0x40000C4C))
#define APP_CTRL0 (*(volatile unsigned long *)(0x40000050))
#define APP_CTRL0_BITSET (*(volatile unsigned long *)(0x40000450))
#define APP_CTRL0_BITCLR (*(volatile unsigned long *)(0x40000850))
#define APP_CTRL0_BITTOG (*(volatile unsigned long *)(0x40000C50))
#define APP_CTRL1 (*(volatile unsigned long *)(0x40000054))
#define APP_CTRL1_BITSET (*(volatile unsigned long *)(0x40000454))
#define APP_CTRL1_BITCLR (*(volatile unsigned long *)(0x40000854))
#define APP_CTRL1_BITTOG (*(volatile unsigned long *)(0x40000C54))
#define APP_CTRL2 (*(volatile unsigned long *)(0x40000058))
#define APP_CTRL2_BITSET (*(volatile unsigned long *)(0x40000458))
#define APP_CTRL2_BITCLR (*(volatile unsigned long *)(0x40000858))
#define APP_CTRL2_BITTOG (*(volatile unsigned long *)(0x40000C58))
#define APP_CTRL3 (*(volatile unsigned long *)(0x4000005C))
#define APP_CTRL3_BITSET (*(volatile unsigned long *)(0x4000045C))
#define APP_CTRL3_BITCLR (*(volatile unsigned long *)(0x4000085C))
#define APP_CTRL3_BITTOG (*(volatile unsigned long *)(0x40000C5C))
#define PMU_STAT (*(volatile unsigned long *)(0x40000060))
#define PMUBIST_ADC_CONF (*(volatile unsigned long *)(0x40000064))
#define PMUBIST_ADC_CONF_BITSET (*(volatile unsigned long *)(0x40000464))
#define PMUBIST_ADC_CONF_BITCLR (*(volatile unsigned long *)(0x40000864))
#define PMUBIST_ADC_CONF_BITTOG (*(volatile unsigned long *)(0x40000C64))
#define PMUBIST_ADC_DATA (*(volatile unsigned long *)(0x40000068))
#define STATUS (*(volatile unsigned long *)(0x4000006C))
#define LPC_STATUS (*(volatile unsigned long *)(0x40000070))
#define LPC_PDTIMER (*(volatile unsigned long *)(0x40000074))
#define LPC_PDTIMER_BITSET (*(volatile unsigned long *)(0x40000474))
#define LPC_PDTIMER_BITCLR (*(volatile unsigned long *)(0x40000874))
#define LPC_PDTIMER_BITTOG (*(volatile unsigned long *)(0x40000C74))
#define PIO_OWNER0 (*(volatile unsigned long *)(0x40000078))
#define PIO_OWNER1 (*(volatile unsigned long *)(0x4000007C))
#define RTC_TIME_LSBS (*(volatile unsigned long *)(0x40000080))
#define RTC_TIME_MSBS (*(volatile unsigned long *)(0x40000084))
#define DEBUG_SEL (*(volatile unsigned long *)(0x40000088))
#define DEBUG_SEL_BITSET (*(volatile unsigned long *)(0x40000488))
#define DEBUG_SEL_BITCLR (*(volatile unsigned long *)(0x40000888))
#define DEBUG_SEL_BITTOG (*(volatile unsigned long *)(0x40000C88))
#define FLASH_STATUS (*(volatile unsigned long *)(0x4000008C))
#define CHIP_WDT_INTERVAL (*(volatile unsigned long *)(0x40000090))
#define CHIP_WDT_INTERVAL_BITSET (*(volatile unsigned long *)(0x40000490))
#define CHIP_WDT_INTERVAL_BITCLR (*(volatile unsigned long *)(0x40000890))
#define CHIP_WDT_INTERVAL_BITTOG (*(volatile unsigned long *)(0x40000C90))
#define CHIP_WDT_CTRL (*(volatile unsigned long *)(0x40000094))
#define CHIP_WDT_CTRL_BITSET (*(volatile unsigned long *)(0x40000494))
#define CHIP_WDT_CTRL_BITCLR (*(volatile unsigned long *)(0x40000894))
#define CHIP_WDT_CTRL_BITTOG (*(volatile unsigned long *)(0x40000C94))
#define CHIP_WDT_TIME (*(volatile unsigned long *)(0x40000098))
#define CHIP_RESET (*(volatile unsigned long *)(0x4000009C))
#define CHIP_RESET_BITSET (*(volatile unsigned long *)(0x4000049C))
#define CHIP_RESET_BITCLR (*(volatile unsigned long *)(0x4000089C))
#define CHIP_RESET_BITTOG (*(volatile unsigned long *)(0x40000C9C))
#define SWD_PIN_CFG (*(volatile unsigned long *)(0x400000A0))
#define SWD_PIN_CFG_BITSET (*(volatile unsigned long *)(0x400004A0))
#define SWD_PIN_CFG_BITCLR (*(volatile unsigned long *)(0x400008A0))
#define SWD_PIN_CFG_BITTOG (*(volatile unsigned long *)(0x40000CA0))

/*typedef struct {
   uint32 DIGITAL_VERSION;
   uint32 CLK_FREQ_DAC;
   uint32 CLK_FREQ_SET;
   uint32 CLK_FREQ_NREFCLKS;
   uint32 CLK_FREQ_REF_SEL;
   uint32 CLK_FREQ_DIG_CLKS;
   uint32 CLK_FREQ_HIGHTARGET;
   uint32 CLK_FREQ_LOWTARGET;
   uint32 CLK_FREQ_LP_BACKOFF;
   uint32 CLK_FREQ_ENABLE;
   uint32 CLK_GATE_SYS;
   uint32 CLK_GATE_MODEM;
   uint32 CLK_GATE_RADIO;
   uint32 CLK_GATE_DEBUG;
   uint32 CLK_GATE_RBIST;
   uint32 LPC_CTRL;
   uint32 LPC_TEST;
   uint32 FPGA_FLASH_WR;
   uint32 FPGA_FLASH_RD;
   uint32 PMU_CTRL;
   uint32 APP_CTRL0;
   uint32 APP_CTRL1;
   uint32 APP_CTRL2;
   uint32 APP_CTRL3;
   uint32 PMU_STAT;
   uint32 PMUBIST_ADC_CONF;
   uint32 PMUBIST_ADC_DATA;
   uint32 STATUS;
   uint32 LPC_STATUS;
   uint32 LPC_PDTIMER;
   uint32 PIO_OWNER0;
   uint32 PIO_OWNER1;
   uint32 RTC_TIME_LSBS;
   uint32 RTC_TIME_MSBS;
   uint32 DEBUG_SEL;
   uint32 FLASH_STATUS;
   uint32 CHIP_WDT_INTERVAL;
   uint32 CHIP_WDT_CTRL;
   uint32 CHIP_WDT_TIME;
   uint32 CHIP_RESET;
   uint32 SWD_PIN_CFG;
} man_regs_t;*/

