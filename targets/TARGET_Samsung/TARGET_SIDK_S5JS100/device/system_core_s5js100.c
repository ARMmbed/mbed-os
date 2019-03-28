/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "cmsis.h"
#include "os_tick.h"


#ifndef SYSTICK_IRQ_PRIORITY
#define SYSTICK_IRQ_PRIORITY    0xFFU
#endif

static uint8_t PendST;
#if 0	//for Vector Debug
#define S5JS100_IRQ(num)  void s5js100_irq_##num() { asm("b ."); }
S5JS100_IRQ(0)
S5JS100_IRQ(1)
S5JS100_IRQ(2)
S5JS100_IRQ(3)
S5JS100_IRQ(4)
S5JS100_IRQ(5)
S5JS100_IRQ(6)
S5JS100_IRQ(7)
S5JS100_IRQ(8)
S5JS100_IRQ(9)
S5JS100_IRQ(10)
S5JS100_IRQ(11)
S5JS100_IRQ(12)
S5JS100_IRQ(13)
S5JS100_IRQ(14)
S5JS100_IRQ(15)
S5JS100_IRQ(16)
S5JS100_IRQ(17)
S5JS100_IRQ(18)
S5JS100_IRQ(19)
S5JS100_IRQ(20)
S5JS100_IRQ(21)
S5JS100_IRQ(22)
S5JS100_IRQ(23)
S5JS100_IRQ(24)
S5JS100_IRQ(25)
S5JS100_IRQ(26)
S5JS100_IRQ(27)
S5JS100_IRQ(28)
S5JS100_IRQ(29)
S5JS100_IRQ(30)
S5JS100_IRQ(31)
S5JS100_IRQ(32)
S5JS100_IRQ(33)
S5JS100_IRQ(34)
S5JS100_IRQ(35)
S5JS100_IRQ(36)
S5JS100_IRQ(37)
S5JS100_IRQ(38)
S5JS100_IRQ(39)
S5JS100_IRQ(40)
S5JS100_IRQ(41)

#define S5JS100_VECTOR(num)  &s5js100_irq_##num
#endif


/**
  * @brief  HAL Status structures definition
  */
#define  TICK_INT_PRIORITY            ((uint32_t)(1U<<__NVIC_PRIO_BITS) - 1U) /*!< tick interrupt priority (lowest by default)             */
/*  Warning: Must be set to higher priority for HAL_Delay()  */
/*  and HAL_GetTick() usage under interrupt context          */
/**
  * @brief  Initializes the System Timer and its interrupt, and starts the System Tick Timer.
  *         Counter is in free running mode to generate periodic interrupts.
  * @param  TicksNumb Specifies the ticks Number of ticks between two interrupts.
  * @retval status:  - 0  Function succeeded.
  *                  - 1  Function failed.
  */
uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb)
{
	return SysTick_Config(TicksNumb);
}

/**
  * @brief This function configures the source of the time base:
  *        The time source is configured to have 1ms time base with a dedicated
  *        Tick interrupt priority.
  * @note This function is called  automatically at the beginning of program after
  *       reset by HAL_Init() or at any time when clock is reconfigured  by HAL_RCC_ClockConfig().
  * @note In the default implementation, SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals.
  *       Care must be taken if HAL_Delay() is called from a peripheral ISR process,
  *       The SysTick interrupt must have higher priority (numerically lower)
  *       than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
  *       The function is declared as __weak  to be overwritten  in case of other
  *       implementation  in user file.
  * @param TickPriority  Tick interrupt priority.
  * @retval HAL status
  */
#define SYSTICK_CLOCK (23660000) 
#define SYSTICK_RELOAD (SYSTICK_CLOCK / 100) + 1 //10ms
int32_t OS_Tick_Setup (uint32_t freq, IRQHandler_t handler) {
  uint32_t load;
  (void)handler;

  if (freq == 0U) {
    //lint -e{904} "Return statement before end of function"
    return (-1);
  }

  load = (SYSTICK_CLOCK / freq ) - 1U;
  if (load > 0x00FFFFFFU) {
    //lint -e{904} "Return statement before end of function"
    return (-1);
  }

  // Set SysTick Interrupt Priority
#if   ((defined(__ARM_ARCH_8M_MAIN__) && (__ARM_ARCH_8M_MAIN__ != 0)) || \
       (defined(__CORTEX_M)           && (__CORTEX_M           == 7U)))
  SCB->SHPR[11] = SYSTICK_IRQ_PRIORITY;
#elif  (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
  SCB->SHPR[1] |= ((uint32_t)SYSTICK_IRQ_PRIORITY << 24);
#elif ((defined(__ARM_ARCH_7M__)      && (__ARM_ARCH_7M__      != 0)) || \
       (defined(__ARM_ARCH_7EM__)     && (__ARM_ARCH_7EM__     != 0)))
  SCB->SHP[11]  = SYSTICK_IRQ_PRIORITY;
#elif  (defined(__ARM_ARCH_6M__)      && (__ARM_ARCH_6M__      != 0))
  SCB->SHP[1]  |= ((uint32_t)SYSTICK_IRQ_PRIORITY << 24);
#else
#error "Unknown ARM Core!"
#endif
	/* 
	 override systick clock source 
	 NVIC_SYSTICK_CTRL_CLKSOURCE=1 : Use processor clock
	 NVIC_SYSTICK_CTRL_CLKSOURCE=0 : Use OSC clock
	*/

  SysTick->CTRL =  0x10003;//SysTick_CTRL_COUNTFLAG_Msk | /*SysTick_CTRL_CLKSOURCE_Msk |*/ SysTick_CTRL_TICKINT_Msk;
  SysTick->LOAD =  load;
//  SysTick->VAL  =  0U;

  PendST = 0U;

  return (0);
}

void OS_Tick_Enable (void) {

}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
	/*Configure the SysTick to have interrupt in 1ms time basis*/
	HAL_SYSTICK_Config(SYSTICK_RELOAD);

	/*Configure the SysTick IRQ priority */
	//HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority ,0);

	/* Return function status */
	return HAL_OK;
}

#if defined ( __ICCARM__ )
#define S5JS100_BOOTMEM_BASE 0x00000000
extern uint32_t __vector_table;
extern uint32_t __Vectors_Size;
#elif defined (TOOLCHAIN_ARM_STD)/*__ARMCC_VERSION)*/
#define S5JS100_BOOTMEM_BASE 0x00000000
extern uint32_t __Vectors;
extern uint32_t __Vectors_Size;
#elif defined ( __GNUC__ )
extern uint32_t _v_start;
extern uint32_t __vector_table;
extern uint32_t __vector_table_end;
extern uint32_t __isr_vector[];
#endif
extern int cal_init(void);

/*
 * SystemCoreConfig(): Configure the System Core
 */
void SystemCoreConfig()
{

	const uint32_t *src;
	uint32_t *dest, size;
	uint32_t vector;

#if defined ( __ICCARM__ ) 
	src = &__vector_table;

	dest = (uint32_t *)S5JS100_BOOTMEM_BASE;
	size = (uint32_t)&__Vectors_Size;

	/* Move vector table to the right location. */
	for (src = &__vector_table, dest = (uint32_t *)S5JS100_BOOTMEM_BASE; dest < ((volatile unsigned int *)S5JS100_BOOTMEM_BASE + size);) {
            *dest++ = *src++;
            //*dest++ = *src++;
	}

#elif defined (TOOLCHAIN_ARM_STD)/*__ARMCC_VERSION)*/
	src = &__Vectors;

	dest = (volatile unsigned int *)S5JS100_BOOTMEM_BASE;
	size = (uint32_t)&__Vectors_Size;

	/* Move vector table to the right location. */
	for (src = &__Vectors, dest = (volatile unsigned int *)S5JS100_BOOTMEM_BASE; dest < ((volatile unsigned int *)S5JS100_BOOTMEM_BASE + size);) {
            *dest++ = *src++;
            //*dest++ = *src++;
	}
#elif defined ( __GNUC__ )
	src = __isr_vector;
	dest = &_v_start;
	size = (uint32_t)&__vector_table_end - (uint32_t)&__vector_table;

	/* Move vector table to the right location. */
	for (src = __isr_vector, dest = &_v_start; dest < (&_v_start + size);) {
		*dest++ = *src++;
	}
#endif

#if 0 //for Vector Debug
	NVIC_SetVector(0, S5JS100_VECTOR(0));
	NVIC_SetVector(1, S5JS100_VECTOR(1));
	NVIC_SetVector(2, S5JS100_VECTOR(2));
	NVIC_SetVector(3, S5JS100_VECTOR(3));
	NVIC_SetVector(4, S5JS100_VECTOR(4));
	NVIC_SetVector(5, S5JS100_VECTOR(5));
	NVIC_SetVector(6, S5JS100_VECTOR(6));
	NVIC_SetVector(7, S5JS100_VECTOR(7));
	NVIC_SetVector(8, S5JS100_VECTOR(8));
	NVIC_SetVector(9, S5JS100_VECTOR(9));
	NVIC_SetVector(10, S5JS100_VECTOR(10));
	NVIC_SetVector(11, S5JS100_VECTOR(11));
	NVIC_SetVector(12, S5JS100_VECTOR(12));
	NVIC_SetVector(13, S5JS100_VECTOR(13));
	NVIC_SetVector(14, S5JS100_VECTOR(14));
	NVIC_SetVector(15, S5JS100_VECTOR(15));
	NVIC_SetVector(16, S5JS100_VECTOR(16));
	NVIC_SetVector(17, S5JS100_VECTOR(17));
	NVIC_SetVector(18, S5JS100_VECTOR(18));
	NVIC_SetVector(19, S5JS100_VECTOR(19));
	NVIC_SetVector(20, S5JS100_VECTOR(20));
	NVIC_SetVector(21, S5JS100_VECTOR(21));
	NVIC_SetVector(22, S5JS100_VECTOR(22));
	NVIC_SetVector(23, S5JS100_VECTOR(23));
	NVIC_SetVector(24, S5JS100_VECTOR(24));
	NVIC_SetVector(25, S5JS100_VECTOR(25));
	NVIC_SetVector(26, S5JS100_VECTOR(26));
	NVIC_SetVector(27, S5JS100_VECTOR(27));
	NVIC_SetVector(28, S5JS100_VECTOR(28));
	NVIC_SetVector(29, S5JS100_VECTOR(29));
	NVIC_SetVector(30, S5JS100_VECTOR(30));
	NVIC_SetVector(31, S5JS100_VECTOR(31));
	NVIC_SetVector(32, S5JS100_VECTOR(32));
	NVIC_SetVector(33, S5JS100_VECTOR(33));
	NVIC_SetVector(34, S5JS100_VECTOR(34));
	NVIC_SetVector(35, S5JS100_VECTOR(35));
	NVIC_SetVector(36, S5JS100_VECTOR(36));
	NVIC_SetVector(37, S5JS100_VECTOR(37));
	NVIC_SetVector(38, S5JS100_VECTOR(38));
	NVIC_SetVector(39, S5JS100_VECTOR(39));
	NVIC_SetVector(40, S5JS100_VECTOR(40));
	NVIC_SetVector(41, S5JS100_VECTOR(41));
#endif
	cal_init();

	HAL_InitTick(TICK_INT_PRIORITY);
}

/* POWER MANAGEMENT */

/*
 * SystemPowerConfig(): Configures the System Power Modes
 */
void SystemPowerConfig()
{
}

/*
 * SystemPowerSuspend(): Enters in System Suspend
 */
void SystemPowerSuspend(power_mode_t mode)
{
	if (mode == POWER_MODE_DEEP_SLEEP) {
		/* Enable deepsleep */
		SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
		/* Ensure effect of last store takes effect */
		__DSB();
		/* Enter sleep mode */
		__WFI();
	} else {
		/* Enter sleep mode */
		__WFI();
	}
}

/*
 * SystemPowerResume(): Returns from System Suspend
 */
void SystemPowerResume(power_mode_t mode)
{
	if (mode == POWER_MODE_DEEP_SLEEP) {
		/* Disable sleeponexit */
		SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
		/* Ensure effect of last store takes effect */
		__DSB();
	}
}


/*
 * System config data storage functions
 * Reserved as the data is not strictly persistent
 */

