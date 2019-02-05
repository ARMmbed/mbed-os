/* mbed Microcontroller Library
 * @Author: Antonio O.
 *
 * NOTE:
 *
 */

#include "us_ticker_api.h"
#include "PeripheralNames.h"
#include "us_ticker_device.h"
#include "hal_types.h"

enum InitStatus{
	noinit = 0,
	init = 1,
}status;

const ticker_info_t *us_ticker_get_info()
{
    static const ticker_info_t info = {
        FREQ_TICK,
        NUMBITS
    };
    return &info;
}

void us_ticker_isr(void){
	//clear interrupt flag and call the us ticker handler
	MFT_ClearIT(MFT2, MFT_IT_TND);
	us_ticker_irq_handler();
}

void us_ticker_init(void){
	if(status == init) return;

	status = init;

	MFT_InitType timer_init;
	NVIC_InitType NVIC_InitStructure;
	//tick_count = 0;

	NVIC_SetVector(MFT2B_IRQn, (uint32_t)&us_ticker_isr);
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_MTFX2, ENABLE); //timer 2 is used as timebase, maybe mtfx1 is useless in this context
	MFT_StructInit(&timer_init);

	timer_init.MFT_Mode = MFT_MODE_3;
	timer_init.MFT_Prescaler = SYST_CLOCK/FREQ_TICK - 1;
	timer_init.MFT_Clock1 = MFT_NO_CLK;
	timer_init.MFT_Clock2 = MFT_PRESCALED_CLK;
	timer_init.MFT_CRA = 0x0000;
	timer_init.MFT_CRB = 0xFFFF;

	MFT_Init(MFT2, &timer_init);

	//Set counter for timer2
	MFT_SetCounter2(MFT2, 0xFFFF);

	//Enable MFT2B Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = MFT2B_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = HIGH_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//Enable the MFT2 interrupt
	MFT_EnableIT(MFT2, MFT_IT_TND, ENABLE);

	//Start MTF2B
	MFT_Cmd(MFT2, ENABLE);
}

void us_ticker_free(){
	if(status == init){
		MFT_Cmd(MFT2, DISABLE);  //Disable MTF2B
		MFT_DeInit(MFT2);        //Deinit peripheral
		NVIC_DisableIRQ(MFT2B_IRQn);	 //Disable IRQ
		SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_MTFX2, DISABLE);  //Disable Clock Gate peripheral
		status = noinit;
	}
}

uint32_t us_ticker_read(){
	if(!status)
		us_ticker_init();

	return ~MFT_GetCounter2(MFT2); //two's complement, MFT2 is a backward counter
}

/* We get here absolute interrupt time which takes into account counter overflow.
 * Since we use additional count-DOWN timer to generate interrupt we need to calculate
 * load value based on timestamp.
 * If the timestamp value is higher than the actual count, then the interrupt will be
 * triggered after "timestamp - actual_ticks".
 * If the timestamp value is lower than the actual count, then the interrupt is triggered
 * at the next count cycle after the overflow, i.e.
 * "timestamp + 1 x overflow_count (0xFFFF) - actual_count".
 */
void us_ticker_set_interrupt(timestamp_t timestamp){
	if(!status)
		us_ticker_init();

//    const uint16_t actual_ticks  = (uint16_t)us_ticker_read();
//    uint16_t delta_ticks =
//            (uint16_t)timestamp >= actual_ticks ? (uint16_t)timestamp - actual_ticks : (uint16_t)((uint32_t)timestamp + 0xFFFF - actual_ticks);

	uint16_t delta_ticks = (uint16_t)(timestamp - us_ticker_read());  //back counter
    if (delta_ticks == 0) {
        // The requested delay is less than the minimum resolution of this counter.
        delta_ticks = 1;
    }
    //SPOSTARE ALL'INIZIO???
    SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_MTFX2, DISABLE);  //Disable Clock Gate peripheral, I can do it since IRQ are disabled
	MFT2->TNCRB = delta_ticks;
	MFT_EnableIT(MFT2, MFT_IT_TND, ENABLE);
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_MTFX2, ENABLE);  //Disable Clock Gate peripheral
}

/* NOTE: must be called with interrupts disabled! */
void us_ticker_disable_interrupt(void){
	if(!status)
		us_ticker_init();
	MFT_EnableIT(MFT2, MFT_IT_TND, DISABLE);

}

/* NOTE: must be called with interrupts disabled! */
void us_ticker_clear_interrupt(void){
	MFT_ClearIT(MFT2, MFT_IT_TND);
}

/* NOTE: must be called with interrupts disabled! */
void us_ticker_fire_interrupt(void){
	NVIC_SetPendingIRQ(MFT2B_IRQn);
}
