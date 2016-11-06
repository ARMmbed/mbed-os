/**
 *******************************************************************************
 * @file    rtc.c
 * @brief Implementation of a Rtc driver
 * @internal
 * @author  ON Semiconductor
 * $Rev: 3525 $
 * $Date: 2015-07-20 15:24:25 +0530 (Mon, 20 Jul 2015) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup rtc
 *
 * @details
 * A real-time clock (RTC) is a computer clock ,that keeps track of the current time.  The heart of the RTC is a series of
 * freely running counters one for each time unit, The series of counters is linked as follows: a roll over event of
 * the seconds counter produces a minutes enable pulse; a roll over event of the minutes counter produces an hours
 * enable pulse, etc.Note that all Counter registers are in an undefined state on power-up.
 * Use the Reset bit in the Control Register to reset the counters to their default values.
 * DIVISOR is the register containing the value to divide the clock frequency to produce 1Hz strobe ; 1Hz strobe is used
 * internally to time the incrementing of the Seconds Counter.
 * There is a set of register to set the values in the counter for each time unit.from where time is start to increment.
 * There is another set of register to set the ALARM ...Each of the Alarm Registers can be programmed with a value that
 * is used to compare to a Counter Register in order to produce an alarm (an interrupt) when the values match.
 * There is a programmable bit in each Alarm Register that determines if the alarm occurs upon a value match, or
 * if the alarm occurs upon a Counter increment condition.
 *
 */
#include "rtc.h"
#include "mbed_assert.h"
#include "lp_ticker_api.h"

static uint16_t SubSecond;
static uint64_t LastRtcTimeus;

/* See rtc.h for details */
void fRtcInit(void)
{
    CLOCK_ENABLE(CLOCK_RTC);             /* enable rtc peripheral */
    CLOCKREG->CCR.BITS.RTCEN = True;     /* Enable RTC clock 32K */

    /* Reset RTC control register */
    RTCREG->CONTROL.WORD     = False;

    /* Initialize all counters */
    RTCREG->SECOND_COUNTER       = False;
    RTCREG->SUB_SECOND_COUNTER   = False;
    RTCREG->SECOND_ALARM         = False;
    RTCREG->SUB_SECOND_ALARM     = False;
    LastRtcTimeus = 0;

    /* Reset RTC Status register */
    RTCREG->STATUS.WORD      = False;

    /* Clear interrupt status */
    RTCREG->INT_CLEAR.WORD   = False;

    /* Start sec & sub_sec counter */
    while(RTCREG->STATUS.BITS.BSY_CTRL_REG_WRT == True);/* Wait previous write to complete */
    RTCREG->CONTROL.WORD |= ((True << RTC_CONTROL_SUBSEC_CNT_START_BIT_POS) |
                             (True << RTC_CONTROL_SEC_CNT_START_BIT_POS));

    /* enable interruption associated with the rtc at NVIC level */
    NVIC_SetVector(Rtc_IRQn,(uint32_t)fRtcHandler); /* TODO define lp_ticker_isr */
    NVIC_ClearPendingIRQ(Rtc_IRQn);
    NVIC_EnableIRQ(Rtc_IRQn);

    while(RTCREG->STATUS.BITS.BSY_CTRL_REG_WRT == True); /* Wait for RTC to finish writing register - RTC operates on 32K clock as compared to 32M core*/

    return;
}

/* See rtc.h for details */
void fRtcFree(void)
{
    /* Reset RTC control register */
    RTCREG->CONTROL.WORD = False;

    /* disable interruption associated with the rtc */
    NVIC_DisableIRQ(Rtc_IRQn);

    while(RTCREG->STATUS.BITS.BSY_CTRL_REG_WRT == True); /* Wait for RTC to finish writing register - RTC operates on 32K clock as compared to 32M core*/
}

/* See rtc.h for details */
void fRtcSetInterrupt(uint32_t timestamp)
{
    SubSecond             = False;
    uint32_t Second = False, EnableInterrupt = False;
    uint8_t DividerAdjust = 1;

    if(timestamp) {
        if(timestamp >= RTC_SEC_TO_US) {
            /* TimeStamp is big enough to set second alarm */
            Second =  ((timestamp / RTC_SEC_TO_US) & RTC_SEC_MASK); /* Convert micro second to second */
            RTCREG->SECOND_ALARM = Second; /* Write to alarm register */

            /* Enable second interrupt */
              EnableInterrupt = True << RTC_CONTROL_SEC_CNT_INT_BIT_POS;
        }
        timestamp = timestamp - Second * RTC_SEC_TO_US; /* Take out micro second for sub second alarm */
        if(timestamp > False) {
            /* We have some thing for sub second */

            /* Convert micro second to sub_seconds(each count = 30.5 us) */
            if(timestamp > 131000) {
                DividerAdjust = 100;
            }

            volatile uint64_t Temp = (timestamp / DividerAdjust * RTC_CLOCK_HZ);
            Temp = (uint64_t)(Temp / RTC_SEC_TO_US * DividerAdjust);
            SubSecond = Temp & RTC_SUB_SEC_MASK;

            if(SubSecond <= 5) {
                SubSecond = 0;
            }

            if(SubSecond > False) {
                /* Second interrupt not enabled */

                /* Set SUB SEC_ALARM */
                RTCREG->SUB_SECOND_ALARM = SubSecond;    /* Write to sub second alarm */

                /* Enable sub second interrupt */
                EnableInterrupt |= (True << RTC_CONTROL_SUBSEC_CNT_INT_BIT_POS);
            }
        }
        
        RTCREG->CONTROL.WORD |= EnableInterrupt;
        /* Enable RTC interrupt */
        NVIC_EnableIRQ(Rtc_IRQn);
        
        /* Wait for RTC to finish writing register - RTC operates on 32K clock as compared to 32M core*/
        while((RTCREG->STATUS.WORD & ((True << RTC_STATUS_SUB_SEC_ALARM_WRT_BIT_POS) |
                                      (True << RTC_STATUS_SEC_ALARM_WRT_BIT_POS) |
                                      (True << RTC_STATUS_CONTROL_WRT_BIT_POS))) == True); 
    }
    return;
}

/* See rtc.h for details */
void fRtcDisableInterrupt(void)
{
    /* Disable RTC interrupt */
    NVIC_DisableIRQ(Rtc_IRQn);
}

/* See rtc.h for details */
void fRtcEnableInterrupt(void)
{
    /* Enable RTC interrupt */
    NVIC_EnableIRQ(Rtc_IRQn);
}

/* See rtc.h for details */
void fRtcClearInterrupt(void)
{
    /* Disable subsec/sec interrupt */
    /* Clear sec & sub_sec interrupts */
    RTCREG->INT_CLEAR.WORD = ((True << RTC_INT_CLR_SUB_SEC_BIT_POS) |
                              (True << RTC_INT_CLR_SEC_BIT_POS));
    
    while((RTCREG->STATUS.WORD & ((True << RTC_STATUS_SUB_SEC_INT_CLR_WRT_BIT_POS) |
                                  (True << RTC_STATUS_SEC_INT_CLR_WRT_BIT_POS))) == True);  /* Wait for RTC to finish writing register - RTC operates on 32K clock as compared to 32M core*/
}

/* See rtc.h for details */
uint64_t fRtcRead(void)
{
    uint32_t Second;
    uint16_t SubSecond;

    /* Hardware Bug fix: The rollover of the sub-second counter initiates the increment of the second counter.
     * That means there is one cycle where the sub-second has rolled back to zero and the second counter has not incremented
     * and a read during that cycle will be incorrect.  That will occur for one RTC cycle and that is about 31us of exposure.
     * If you read a zero in the sub-second counter then increment the second counter by 1.
     * Alternatively, subtract 1 from the Sub-seconds counter to align the Second and Sub-Second rollover.
     */

    /* Read the Second and Sub-second counters, then read the Second counter again.
     * If it changed, then the Second rolled over while reading Sub-seconds, so go back and read them both again.
     */

    do {
        Second    = RTCREG->SECOND_COUNTER;                            /* Get SEC_COUNTER reg value */
        SubSecond = (RTCREG->SUB_SECOND_COUNTER - 1) & SUB_SEC_MASK;   /* Get SUB_SEC_COUNTER reg value */
    } while (Second != RTCREG->SECOND_COUNTER);                        /* Repeat if the second has changed */

    //note: casting to float removed to avoid reduction in resolution
    uint64_t RtcTimeus = ((uint64_t)SubSecond * RTC_SEC_TO_US / RTC_CLOCK_HZ) + ((uint64_t)Second * RTC_SEC_TO_US);

    /*check that the time did not go backwards */
    MBED_ASSERT(RtcTimeus >= LastRtcTimeus);
    LastRtcTimeus = RtcTimeus;

    return RtcTimeus;
}

/* See rtc.h for details */
void fRtcWrite(uint64_t RtcTimeus)
{
    uint32_t Second    = False;
    uint16_t SubSecond = False;
    /* Stop RTC */
    RTCREG->CONTROL.WORD &= ~((True << RTC_CONTROL_SUBSEC_CNT_START_BIT_POS) |
                              (True << RTC_CONTROL_SEC_CNT_START_BIT_POS));

    if(RtcTimeus > RTC_SEC_TO_US) {
        /* TimeStamp is big enough to set second counter */
        Second =  ((RtcTimeus / RTC_SEC_TO_US) & RTC_SEC_MASK);
    }
    RTCREG->SECOND_COUNTER = Second;
    RtcTimeus = RtcTimeus - (Second * RTC_SEC_TO_US);
    if(RtcTimeus > False) {
        /* Convert TimeStamp to sub_seconds */
        SubSecond =  (uint16_t)((float)(RtcTimeus * RTC_CLOCK_HZ / RTC_SEC_TO_US)) & RTC_SUB_SEC_MASK;
    }
    /* Set SUB_SEC_ALARM */
    RTCREG->SUB_SECOND_COUNTER = SubSecond;

    while(RTCREG->STATUS.BITS.BSY_CTRL_REG_WRT == True); /* Wait for RTC to finish writing register - RTC operates on 32K clock as compared to 32M core*/
    /* Start RTC */
    RTCREG->CONTROL.WORD |= ((True << RTC_CONTROL_SUBSEC_CNT_START_BIT_POS) |
                             (True << RTC_CONTROL_SEC_CNT_START_BIT_POS));

    while(RTCREG->STATUS.BITS.BSY_ANY_WRT == True); /* Wait for RTC to finish writing register - RTC operates on 32K clock as compared to 32M core*/
}

/* See rtc.h for details */
void fRtcHandler(void)
{
    /* SUB_SECOND/SECOND interrupt occured */
    volatile uint32_t TempStatus = RTCREG->STATUS.WORD;

    /* Disable RTC interrupt */
    NVIC_DisableIRQ(Rtc_IRQn);

    /* Clear sec & sub_sec interrupts */
    RTCREG->INT_CLEAR.WORD = ((True << RTC_INT_CLR_SUB_SEC_BIT_POS) |
                              (True << RTC_INT_CLR_SEC_BIT_POS));

    /* TODO  ANDing SUB_SEC & SEC interrupt - work around for RTC issue - will be resolved in REV G */
    if(TempStatus & RTC_SEC_INT_STATUS_MASK) {
        /* Second interrupt occured */
        if(SubSecond > False) {
            /* Set SUB SEC_ALARM */
            RTCREG->SUB_SECOND_ALARM = SubSecond + RTCREG->SUB_SECOND_COUNTER;
            /* Enable sub second interrupt */
            RTCREG->CONTROL.WORD |= (True << RTC_CONTROL_SUBSEC_CNT_INT_BIT_POS);
        } else {
            /* We reach here after second interrupt is occured */
            RTCREG->CONTROL.WORD &= ~(True << RTC_CONTROL_SUBSEC_CNT_INT_BIT_POS) |
                                    (True << RTC_CONTROL_SEC_CNT_INT_BIT_POS);
        }
    } else {
        /* We reach here after sub_second or (Sub second + second) interrupt occured */
		/* Disable Second and sub_second interrupt */
        RTCREG->CONTROL.WORD &= ~(True << RTC_CONTROL_SUBSEC_CNT_INT_BIT_POS) |
                                (True << RTC_CONTROL_SEC_CNT_INT_BIT_POS);
    }

	NVIC_EnableIRQ(Rtc_IRQn);
	
    /* Wait for RTC to finish writing register - RTC operates on 32K clock as compared to 32M core*/
    while((RTCREG->STATUS.WORD & ((True << RTC_STATUS_SUB_SEC_ALARM_WRT_BIT_POS) |
                                  (True << RTC_STATUS_CONTROL_WRT_BIT_POS) |
                                  (True << RTC_STATUS_SUB_SEC_INT_CLR_WRT_BIT_POS) |
                                  (True << RTC_STATUS_SEC_INT_CLR_WRT_BIT_POS))) == True);  
	
	lp_ticker_irq_handler();
}

boolean fIsRtcEnabled(void)
{
    if(RTCREG->CONTROL.BITS.SUB_SEC_COUNTER_EN | RTCREG->CONTROL.BITS.SEC_COUNTER_EN) {
        return True;
    } else {
        return False;
    }
}
