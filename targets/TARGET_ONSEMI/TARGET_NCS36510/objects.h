/**
 ******************************************************************************
 * @file Objects.h
 * @brief Implements an assertion.
 * @internal
 * @author ON Semiconductor
 * $Rev: 0.1 $
 * $Date: 2015-11-06 $
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
 * @ingroup debug
 */
#ifndef OBJECTS_H_
#define OBJECTS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "gpio_map.h"
#include "uart_16c550_map.h"
#include "PinNames.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "target_config.h"

typedef enum {
    FlowControlNone_1,
    FlowControlRTS_1,
    FlowControlCTS_1,
    FlowControlRTSCTS_1
} FlowControl_1;

struct serial_s {
    Uart16C550Reg_pt    UARTREG;
    FlowControl_1       FlowCtrl;
    IRQn_Type           IRQType;
    int index;
};

typedef struct _gpio_t {
    GpioReg_pt          GPIOMEMBASE;
    PinName             gpioPin;
    uint32_t            gpioMask;

} gpio_t;


/* TODO: This is currently a dummy structure; implementation will be done along
 * with the sleep API implementation
 */
typedef struct sleep_s {
    uint32_t timeToSleep;    /* 0: Use sleep type variable to select low power mode; Noz-zero: Selects sleep type based on timeToSleep duration using table 1. sleep below */
    uint8_t     SleepType;   /* 0: Sleep; 1: DeepSleep; 2: Coma */
} sleep_t;

/* Table 1. Sleep
     ___________________________________________________________________________________
    |    Sleep duration                                                 |  Sleep Type   |
    |-------------------------------------------------------------------|---------------|
    |  > Zero AND <= SLEEP_DURATION_SLEEP_MAX                           |  sleep        |
    |  > SLEEP_DURATION_SLEEP_MAX AND <= SLEEP_DURATION_DEEPSLEEP_MAX   |  deepsleep    |
    |  > SLEEP_DURATION_DEEPSLEEP_MAX                                   |  coma         |
    |___________________________________________________________________|_______________|

*/

struct gpio_irq_s {
    uint32_t            pin;
    uint32_t            pinMask;
    GpioReg_pt          GPIOMEMBASE;
};

typedef struct {

    /* options to configure the ADC */
    uint8_t             interruptConfig;       /**< 1= interrupt Enable  0=Interrupt Disable  */
    uint8_t             PrescaleVal;           /**< Prescaler:    Sets the converter clock frequency.  Fclk = 32 MHz/(prescaler + 1) where prescaler is the value of this register segment. The minimum tested value is 07 (4 MHz clock) */
    uint8_t             measurementType;       /**< 1= Absolute 0= Differential */
    uint8_t             mode;                  /**< 1= Continuous Conversion 0= Single Shot  */
    uint8_t             referenceCh;           /**< Selects 1 to 8 channels for reference channel  */
    uint8_t             convCh;                /**< Selects 1 or 8 channels to do a conversion on.*/
    uint8_t             inputScale;            /**< Sets the input scale, 000 ? 1.0, 001 ? 0.6923, 010 ? 0.5294, 011 ? 0.4286, 100 ? 0.3600, 101 ? 0.3103, 110 ? 0.2728, 111 ? 0.2432 */
    uint8_t             samplingTime;          /**< Sample Time. Sets the measure time in units of PCLKperiod * (Prescale + 1).*/
    uint8_t             WarmUpTime;            /**< The number of converter clock cycles that the state machine dwells in the warm or warm_meas state */
    uint16_t            samplingRate;          /**< Sets the sample rate in units of PCLKperiod * (Prescale + 1).  */

} analog_config_s;

struct analogin_s {

    analog_config_s      *adcConf;
    AdcReg_pt             adcReg;
    PinName               pin;
    uint8_t               pinFlag;
    uint32_t              ADC_Offset_Value;
};

struct pwmout_s {

    PwmReg_pt pwmReg;
};

struct port_s {
    GpioReg_pt GPIOMEMBASE;
    PortName   port;
    uint32_t   mask;
};

typedef enum {
    littleEndian = 0,
    bigEndian
} spi_ipc7207_endian_t, *spi_ipc7207_endian_pt;

/** Type for the clock polarity. */
typedef enum {
    activeLow = 0,
    activeHigh
} spi_clockPolarity_t, *spi_clockPolarity_pt;

/** Type for the clock phase. */
typedef enum {
    risingEdge = 0,
    fallingEdge
} spi_clockPhase_t, *spi_clockPhase_pt;

struct spi_s {
    SpiIpc7207Reg_pt membase;     /* Register address */
    IRQn_Type irq;                /* IRQ number of the IRQ associated to the device. */
    uint8_t irqEnable;            /* IRQ enables for 8 IRQ sources:
                                             * - bit 7 = Receive FIFO Full
                                             * - bit 6 = Receive FIFO 'Half' Full (watermark level)
                                             * - bit 5 = Receive FIFO Not Empty
                                             * - bit 4 = Transmit FIFO Not Full
                                             * - bit 3 = Transmit FIFO 'Half' Empty (watermark level)
                                             * - bit 2 = Transmit FIFO Empty
                                             * - bit 1 = Transfer Error
                                             * - bit 0 = ssIn (conditionally inverted and synchronized to PCLK)
                                     * (unused option in current implementation / irq 6 and 7 used) */
    uint8_t slaveSelectEnable;    /* Slave Select enables (x4):
                                    * - 0 (x4) = Slave select enable
                                    * - 1 (x4) = Slave select disable */
    uint8_t slaveSelectBurst;     /* Slave Select burst mode:
                                    * - NO_BURST_MODE = Burst mode disable
                                    * - BURST_MODE = Burst mode enable */
    uint8_t slaveSelectPolarity;  /* Slave Select polarity (x4) for up to 4 slaves:
                                    * - 0 (x4) = Slave select is active low
                                    * - 1 (x4) = Slave select is active high */
    uint8_t txWatermark;          /* Transmit FIFO Watermark: Defines level of RX Half Full Flag
                                            * - Value between 1 and 15
                                            * (unused option in current implementation / not txWatermark irq used) */
    uint8_t rxWatermark;          /* Receive FIFO Watermark: Defines level of TX Half Full Flag:
                                            * - Value between 1 and 15
                                            * * (unused option in current implementation / rxWatermark fixed to 1) */
    spi_ipc7207_endian_t endian;  /* Bits endianness:
                                    * - LITTLE_ENDIAN = LSB first
                                    * - BIG_ENDIAN = MSB first */
    uint8_t samplingEdge;         /* SDI sampling edge (relative to SDO sampling edge):
                                    * - 0 = opposite to SDO sampling edge
                                    * - 1 = same as SDO sampling edge */
    uint32_t baudrate;                     /* The expected baud rate. */
    spi_clockPolarity_t clockPolarity;     /* The clock polarity (active high or low). */
    spi_clockPhase_t clockPhase;           /* The clock phase (sample on rising or falling edge). */
    uint8_t wordSize;                      /* The size word size in number of bits. */
    uint8_t Mode;
    uint32_t event;
};

struct i2c_s {
    uint32_t baudrate;             /**< The expected baud rate. */
    uint32_t I2cStatusFromInt;
    uint8_t ClockSource;           /**< I2C clock source, 0 – clkI2C pin, 1 – PCLK */
    uint8_t irqEnable;             /**< IRQs to be enabled */
    I2cIpc7208Reg_pt membase;      /**< The memory base for the device's registers. */
    IRQn_Type irq;                 /**< The IRQ number of the IRQ associated to the device. */
    //queue_pt rxQueue;            /**< The receive queue for the device instance. */
};

struct trng_s {
    RandReg_pt membase;    /**< The memory base for the device's registers. */
};

#ifdef __cplusplus
}
#endif

#endif //OBJECTS_H_
