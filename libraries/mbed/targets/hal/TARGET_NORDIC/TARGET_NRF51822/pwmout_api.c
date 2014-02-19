/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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
#include "pwmout_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

#define NO_PWMS        2

static const PinMap PinMap_PWM[] = {
    {p0,  PWM_1, 1},
    {p1,  PWM_1, 1},
    {p2,  PWM_1, 1},
    {p3,  PWM_1, 1},
    {p4,  PWM_1, 1},
    {p5,  PWM_1, 1},
    {p6,  PWM_1, 1},
    {p7,  PWM_1, 1},
    {p8,  PWM_1, 1},
    {p9,  PWM_1, 1},
    {p10,  PWM_1, 1},
    {p11,  PWM_1, 1},
    {p12,  PWM_1, 1},
    {p13,  PWM_1, 1},
    {p14,  PWM_1, 1},
    {p15,  PWM_1, 1},
    {p16,  PWM_1, 1},
    {p17,  PWM_1, 1},
    {p18,  PWM_1, 1},
    {p19,  PWM_1, 1},
    {p20,  PWM_1, 1},
    {p21,  PWM_1, 1},
    {p22,  PWM_1, 1},    
    {p23,  PWM_1, 1},    
    {p24,  PWM_1, 1},
    {p25,  PWM_1, 1},
    {p28,  PWM_1, 1},
    {p29,  PWM_1, 1},
    {p30,  PWM_1, 1},
    {NC, NC, 0}
};

static NRF_TIMER_Type *Timers[1] = {
    NRF_TIMER2    
};

uint8_t PWM_taken[NO_PWMS]     = {0,0};
uint16_t PERIOD[NO_PWMS]       = {2500,2500};//20ms
uint16_t PULSE_WIDTH[NO_PWMS]  = {1,1};//set to 1 instead of 0
uint16_t ACTUAL_PULSE[NO_PWMS] = {0,0};


/** @brief Function for handling timer 2 peripheral interrupts.
 */
 #ifdef __cplusplus
extern "C" {
#endif 
void TIMER2_IRQHandler(void)
{
    static uint16_t CCVal1 = 2501;
    static uint16_t CCVal2 = 2501;
    
    if ((NRF_TIMER2->EVENTS_COMPARE[1] != 0) && 
       ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE1_Msk) != 0)){
       
        NRF_TIMER2->CC[0]             = CCVal1;            
        NRF_TIMER2->EVENTS_COMPARE[1] = 0;
        NRF_TIMER2->CC[1]             = (NRF_TIMER2->CC[1] + PERIOD[0]);

        CCVal1 = NRF_TIMER2->CC[1] + PULSE_WIDTH[0];        
    }
    if ((NRF_TIMER2->EVENTS_COMPARE[3] != 0) && 
       ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE3_Msk) != 0)){
       
        NRF_TIMER2->CC[2]             = CCVal2;
        NRF_TIMER2->EVENTS_COMPARE[3] = 0;
        NRF_TIMER2->CC[3]             = (NRF_TIMER2->CC[3] + PERIOD[1]);
    
        CCVal2 = NRF_TIMER2->CC[3] + PULSE_WIDTH[1];        
    }    
}
#ifdef __cplusplus
}
#endif 
/** @brief Function for initializing the Timer peripherals.
 */
void timer_init(uint8_t pwmChoice)
{
    NRF_TIMER_Type *timer = Timers[pwmChoice/2];
    if(!(pwmChoice%2)){
        timer->POWER     = 0;
        timer->POWER     = 1;    
        timer->MODE      = TIMER_MODE_MODE_Timer;    
        timer->BITMODE   = TIMER_BITMODE_BITMODE_16Bit << TIMER_BITMODE_BITMODE_Pos;
        timer->PRESCALER = 7;//8us ticks    
    }
    
    if(pwmChoice%2){
        timer->CC[2] = PERIOD[pwmChoice] + PULSE_WIDTH[pwmChoice];
        timer->CC[3] = PERIOD[pwmChoice];
        
        // Interrupt setup.
        timer->INTENSET = (TIMER_INTENSET_COMPARE3_Enabled << TIMER_INTENSET_COMPARE3_Pos);
    }
    else{
        timer->CC[0] = PERIOD[pwmChoice] + PULSE_WIDTH[pwmChoice];
        timer->CC[1] = PERIOD[pwmChoice];
        
        // Interrupt setup.
        timer->INTENSET |= (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
    }
    //high priority application interrupt
    NVIC_SetPriority(TIMER2_IRQn, 1);
    NVIC_EnableIRQ(TIMER2_IRQn);
    
    timer->TASKS_START = 0x01;
}
/** @brief Function for initializing the GPIO Tasks/Events peripheral.
 */
void gpiote_init(PinName pin,uint8_t channel_number)
{
    // Connect GPIO input buffers and configure PWM_OUTPUT_PIN_NUMBER as an output.
    NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                            | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                            | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                            | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                            | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
    NRF_GPIO->OUTCLR = (1UL << pin);
    // Configure GPIOTE channel 0 to toggle the PWM pin state
    // @note Only one GPIOTE task can be connected to an output pin.
    /* Configure channel to Pin31, not connected to the pin, and configure as a tasks that will set it to proper level */
    NRF_GPIOTE->CONFIG[channel_number] = (GPIOTE_CONFIG_MODE_Task       << GPIOTE_CONFIG_MODE_Pos)     |
                                         (31UL                          << GPIOTE_CONFIG_PSEL_Pos)     |
                                         (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos);
     /* Three NOPs are required to make sure configuration is written before setting tasks or getting events */
    __NOP();
    __NOP();
    __NOP(); 
    /* Launch the task to take the GPIOTE channel output to the desired level */
    NRF_GPIOTE->TASKS_OUT[channel_number] = 1;
    
    /* Finally configure the channel as the caller expects. If OUTINIT works, the channel is configured properly. 
       If it does not, the channel output inheritance sets the proper level. */
    NRF_GPIOTE->CONFIG[channel_number] = (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos)     |
                                         ((uint32_t)pin    << GPIOTE_CONFIG_PSEL_Pos)     |
                                         ((uint32_t)GPIOTE_CONFIG_POLARITY_Toggle      << GPIOTE_CONFIG_POLARITY_Pos) |
                                         ((uint32_t)GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos);

    /* Three NOPs are required to make sure configuration is written before setting tasks or getting events */
    __NOP();
    __NOP();
    __NOP(); 
}
/** @brief Function for initializing the Programmable Peripheral Interconnect peripheral.
 */
static void ppi_init(uint8_t pwm)
{
//using ppi channels 0-3 (0-7 are available)
    uint8_t channel_number = 2*pwm;
    NRF_TIMER_Type *timer = Timers[pwm/2];
    
    // Configure PPI channel 0 to toggle ADVERTISING_LED_PIN_NO on every TIMER1 COMPARE[0] match
    NRF_PPI->CH[channel_number].TEP     = (uint32_t)&NRF_GPIOTE->TASKS_OUT[pwm];
    NRF_PPI->CH[channel_number+1].TEP   = (uint32_t)&NRF_GPIOTE->TASKS_OUT[pwm];
    NRF_PPI->CH[channel_number].EEP     = (uint32_t)&timer->EVENTS_COMPARE[channel_number-(4*(channel_number/4))];    
    NRF_PPI->CH[channel_number+1].EEP   = (uint32_t)&timer->EVENTS_COMPARE[channel_number+1-(4*(channel_number/4))];    
    
    // Enable PPI channels.
    NRF_PPI->CHEN |= (1 << channel_number)
                  |  (1 << (channel_number+1));
}

void setModulation(pwmout_t* obj,uint8_t toggle,uint8_t high)
{
    if(high){
        NRF_GPIOTE->CONFIG[obj->pwm] |= ((uint32_t)GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos);
        if(toggle){
            NRF_GPIOTE->CONFIG[obj->pwm] |= (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos)   |
                                            ((uint32_t)GPIOTE_CONFIG_POLARITY_Toggle      << GPIOTE_CONFIG_POLARITY_Pos);
        }
        else{
            NRF_GPIOTE->CONFIG[obj->pwm] &= ~((uint32_t)GPIOTE_CONFIG_POLARITY_Toggle      << GPIOTE_CONFIG_POLARITY_Pos);
            NRF_GPIOTE->CONFIG[obj->pwm] |= ((uint32_t)GPIOTE_CONFIG_POLARITY_LoToHi      << GPIOTE_CONFIG_POLARITY_Pos);
        }
    }
    else{
        NRF_GPIOTE->CONFIG[obj->pwm] &= ~((uint32_t)GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos);
        
        if(toggle){
            NRF_GPIOTE->CONFIG[obj->pwm] |= (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos)   |
                                            ((uint32_t)GPIOTE_CONFIG_POLARITY_Toggle      << GPIOTE_CONFIG_POLARITY_Pos);
        }
        else{
            NRF_GPIOTE->CONFIG[obj->pwm] &= ~((uint32_t)GPIOTE_CONFIG_POLARITY_Toggle      << GPIOTE_CONFIG_POLARITY_Pos);
            NRF_GPIOTE->CONFIG[obj->pwm] |= ((uint32_t)GPIOTE_CONFIG_POLARITY_HiToLo      << GPIOTE_CONFIG_POLARITY_Pos);
        }
    }
}
void pwmout_init(pwmout_t* obj, PinName pin) {
    // determine the channel
    uint8_t pwmOutSuccess = 0;
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    
    if (pwm == (PWMName)NC){
        error("PwmOut pin mapping failed");
    }
        
    if(PWM_taken[(uint8_t)pwm]){
        for(uint8_t i = 1; !pwmOutSuccess && (i<NO_PWMS) ;i++){
            if(!PWM_taken[i]){
                pwm           = (PWMName)i;
                PWM_taken[i]  = 1;
                pwmOutSuccess = 1;
            }
        }
    }
    else{
        pwmOutSuccess           = 1;
        PWM_taken[(uint8_t)pwm] = 1;
    }
    
    if(!pwmOutSuccess){
        error("PwmOut pin mapping failed. All available PWM channels are in use.");
    }
    
    obj->pwm = pwm;
    obj->pin = pin;
    
    gpiote_init(pin,(uint8_t)pwm);
    ppi_init((uint8_t)pwm);
    
    if(pwm == 0){
        NRF_POWER->TASKS_CONSTLAT = 1;
    }
    
    timer_init((uint8_t)pwm);
    
    //default to 20ms: standard for servos, and fine for e.g. brightness control
    pwmout_period_ms(obj, 20);
    pwmout_write    (obj, 0);
    
}

void pwmout_free(pwmout_t* obj) {
    // [TODO]
}

void pwmout_write(pwmout_t* obj, float value) {
    uint16_t oldPulseWidth;
    
    if (value < 0.0f) {
        value = 0.0;
    } else if (value > 1.0f) {
        value = 1.0;
    }        
    
    oldPulseWidth           = ACTUAL_PULSE[obj->pwm];
    ACTUAL_PULSE[obj->pwm]  = PULSE_WIDTH[obj->pwm]  = value* PERIOD[obj->pwm];
    
    if(PULSE_WIDTH[obj->pwm] == 0){
        PULSE_WIDTH[obj->pwm] = 1;
        setModulation(obj,0,0);    
    }
    else if(PULSE_WIDTH[obj->pwm] == PERIOD[obj->pwm]){
        PULSE_WIDTH[obj->pwm] = PERIOD[obj->pwm]-1;
        setModulation(obj,0,1);
    }
    else if( (oldPulseWidth == 0) || (oldPulseWidth == PERIOD[obj->pwm]) ){
        setModulation(obj,1,oldPulseWidth == PERIOD[obj->pwm]);
    }    
}

float pwmout_read(pwmout_t* obj) {
    return ((float)PULSE_WIDTH[obj->pwm]/(float)PERIOD[obj->pwm]);
}

void pwmout_period(pwmout_t* obj, float seconds) {
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us) {
    uint32_t periodInTicks = us/8;
    
    if(periodInTicks>((1<<16) -1))
    {
        PERIOD[obj->pwm] = (1<<16 )-1;//262ms
    }
    else if(periodInTicks<5){
        PERIOD[obj->pwm] = 5;
    }
    else{
        PERIOD[obj->pwm] =periodInTicks;
    }    
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    uint32_t pulseInTicks  = us/8;
    uint16_t oldPulseWidth = ACTUAL_PULSE[obj->pwm];
    
    ACTUAL_PULSE[obj->pwm] = PULSE_WIDTH[obj->pwm]  = pulseInTicks;
    
    if(PULSE_WIDTH[obj->pwm] == 0){
        PULSE_WIDTH[obj->pwm] = 1;
        setModulation(obj,0,0);    
    }
    else if(PULSE_WIDTH[obj->pwm] == PERIOD[obj->pwm]){
        PULSE_WIDTH[obj->pwm] = PERIOD[obj->pwm]-1;
        setModulation(obj,0,1);
    }
    else if( (oldPulseWidth == 0) || (oldPulseWidth == PERIOD[obj->pwm]) ){
        setModulation(obj,1,oldPulseWidth == PERIOD[obj->pwm]);
    }    
}
