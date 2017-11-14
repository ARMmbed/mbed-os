/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

/* The serial driver connects UART HW to mbed and also associates the UART
 * HW with physical pins.  Any physical pin can be linked to any UART,
 * however the mbed serial port initialisation API makes no mention of
 * which UART HW is to be used (only the pins) and hence the driver needs
 * to make some decisions for itself.
 *
 * There are two and a half UARTs on the chip: UART0, UART1 and a
 * lower-power, receive-only UART that is clocked from 32 kHz and can
 * therefore be awake while the rest of the chip is sleeping peacefully.
 * This provides maximal power saving, however the LP UART can only run
 * at 9600 bits/s (which is quite sufficient for all NB-IoT needs).
 *
 * So, if the baud rate is 9600 the driver code configures the LP UART
 * for Rx and UART0 for Tx. If the baud rate is not 9600 then it configures
 * UART0 for both Rx and Tx.  Unless... the Tx pin is the pin UART1_TX (it
 * is an mbed convention to use the Tx pin), which is p6, in which case UART1
 * is configured instead.  This latter is not the normal case as this pin
 * is intended to be used as a GPIO.
 *
 * If the baud rate is changed the driver reconfigures to match.
 *
 * TODO: implement asynchronous and flow control APIs.
 */

#include "mbed_assert.h"
#include "serial_api.h"
#include "pinmap.h"

#include "cmsis.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

/* Registers banks for the standard UARTs */
#define UART1_REG (*(volatile uart_ctrl_t *) UART1_CONTROL_BASE)
#define UART2_REG (*(volatile uart_ctrl_t *) UART2_CONTROL_BASE)

/* Masks for the UART control bits in the reset and clock enable registers */
#define UART0_CTRL  (1 << 3)
#define UART1_CTRL  (1 << 4)
#define UARTLP_CTRL (1 << 6)

/* Convert number of data bits to register values */
#define UART_DATA_BITS_7 7
#define UART_DATA_BITS_8 8
#define REGISTER_DATA_BITS(x) ((x) - MIN_NUM_UART_DATA_BITS)

/* Number of stop bits */
#define NUM_UART_STOP_BITS_1 1
#define NUM_UART_STOP_BITS_2 2

/* uart reference frequency in Hz*/
#define UART_REF_FREQ   (26000000UL)

/** Macro generating the mask for a bitfield of \p n bits */
#define DRIVER_BIT_SET(n)                      (1u<<(n))

#define DRIVER_BITS_CLR(data, mask)            ((data) & ~(mask))
#define DRIVER_BITS_SET(data, bits)            ((data) |  (bits))

/** Macro generating the mask for a bitfield of \p n bits */
#define DRIVER_BIT_MASK(n)                      (DRIVER_BIT_SET(n) - 1)

/** Macro generating the mask for a bitfield defined as `name_OFFSET` and `name_SIZE` */
#define DRIVER_BITFIELD_MASK_(name)             (DRIVER_BIT_MASK(name##_SIZE) << (name##_OFFSET))
#define DRIVER_BITFIELD_MASK(name)              DRIVER_BITFIELD_MASK_(name)

/** Extract bitfield defined as `name_OFFSET` and `name_SIZE` from \p data */
#define DRIVER_BITFIELD_GET_(data, name)        (((data) >> name##_OFFSET) & DRIVER_BIT_MASK(name##_SIZE))
#define DRIVER_BITFIELD_GET(data, name)         DRIVER_BITFIELD_GET_(data, name)

/** Return \p data with bitfield defined as `name_OFFSET` and `name_SIZE` cleared */
#define DRIVER_BITFIELD_CLR(data, name)         ((data) & ~DRIVER_BITFIELD_MASK(name))

/** Return \p bitfield defined as `name_OFFSET` and `name_SIZE` set to \p value */
#define DRIVER_BITFIELD_VAL_(name, value)       (((value) & DRIVER_BIT_MASK(name##_SIZE)) << name##_OFFSET)
#define DRIVER_BITFIELD_VAL(name, value)        DRIVER_BITFIELD_VAL_(name, value)

/** Return \p data with bitfield defined as `name_OFFSET` and `name_SIZE` set to \p value */
#define DRIVER_BITFIELD_SET(data, name, value)  (DRIVER_BITFIELD_CLR(data, name) | DRIVER_BITFIELD_VAL(name, value))

/** Return \p bitfield defined as `name_OFFSET` and `name_SIZE` set to \p name_ENUM_value */
#define DRIVER_BITFIELD_ENUM_(name, enumValue)      DRIVER_BITFIELD_VAL(name, name##_ENUM_##enumValue)
#define DRIVER_BITFIELD_ENUM(name, enumValue)       DRIVER_BITFIELD_ENUM_(name, enumValue)

/** Return \p data with bitfield defined as `name_OFFSET` and `name_SIZE` set to \p name_ENUM_value */
#define DRIVER_BITFIELD_SET_ENUM(data, name, enumValue)  DRIVER_BITFIELD_SET(data, name,  DRIVER_BITFIELD_ENUM(name, enumValue))

/* PIO macros */
//#define PIO_CHANNEL_SUB_32_MASK       (0x1F)
//#define PIO_CHANNEL_OVER_32_SHIFT     (5)

#define Uart1RXDxSIO_CHANNEL    (42)
#define Uart1TXDxSIO_CHANNEL    (43)
#define UART1CTSxSIO_CHANNEL    (44)
#define Uart1RTSxSIO_CHANNEL    (45)

#define Uart2RXDxSIO_CHANNEL    (46)
#define Uart2TXDxSIO_CHANNEL    (47)
#define Uart2CTSxSIO_CHANNEL    (48)
#define Uart2RTSxSIO_CHANNEL    (49)

/* ----------------------------------------------------------------
 * TYPES
 * ----------------------------------------------------------------*/

/* Enum to identify the interrupt to the UART handler */
typedef enum {
    IRQ_UART_ID_1,
    IRQ_UART_ID_2,
    NUM_IRQ_IDS
} irq_uart_id_t;

/* ----------------------------------------------------------------
 * GLOBAL VARIABLES
 * ----------------------------------------------------------------*/

// /* The IRQ configuration variables, set up and named by mbed */
 static uint32_t serial_irq_ids[NUM_IRQ_IDS] = {0};
 static uart_irq_handler irq_handler = NULL;
 static uint8_t serial_channel[2]={0x00};

/* RTX needs these */
int stdio_uart_inited = 0;
serial_t stdio_uart;

/* ----------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------*/
static uint32_t zeroBitNumFromLsb(uint32_t data);
static void irq_enable(serial_t *obj);
static void irq_disable(serial_t *obj);

/* TODO: the following two non-api functions are for the sake of testing only, once pinnames and pinmaps are implemented, it will be replaced */
static void config_pio_channel(uint8_t channel);
static void disable_pio_channel(uint8_t channel);
/* ----------------------------------------------------------------
 * NON-API FUNCTIONS
 * ----------------------------------------------------------------*/
/* calculate the number of zero bits from LSB */
static uint32_t zeroBitNumFromLsb(uint32_t data)
{
    uint32_t zeroBitNum = 0;
    for (uint32_t i = 0; i < 32; ++i) {
        if ((data & 0x01) == 0) {
          ++zeroBitNum;
          data >>= 1;
        }
        else {
            //find bit 1.
            break;
        }
    }
    return zeroBitNum;
}

/* TODO: the following two non-api functions are for the sake of testing only, once pinnames and pinmaps are implemented, it will be replaced */
/* setup pio channels and mux for Uart pins */
static void config_pio_channel(uint8_t channel)
{
    struct pio_s *pio_channel_regbase;
    uint8_t channel_offset_in_reg;
    
    channel_offset_in_reg = channel & PIO_CHANNEL_SUB_32_MASK;
    
    if( (channel >> PIO_CHANNEL_OVER_32_SHIFT) == 0) {      
        pio_channel_regbase = (struct pio_s *)PIO_CONTROL_BASE;
    } else {          
        pio_channel_regbase = (struct pio_s *)(PIO_CONTROL_BASE + 0x220);
    }          

    pio_channel_regbase->pio_pdr_0 = (1 << channel_offset_in_reg);     //pio disable
    pio_channel_regbase->pio_asr_0 |= (1 << channel_offset_in_reg);     //mux0 enable
    pio_channel_regbase->pio_percpdr_0 = (1 << channel_offset_in_reg); //periph pullup/down disable
    pio_channel_regbase->pio_odr_0  = (1 << channel_offset_in_reg);    //pad config: output driver disable
    pio_channel_regbase->pio_iner_0 = (1 << channel_offset_in_reg);    //pad config: receiver enable
    pio_channel_regbase->pio_pldr_0 = (1 << channel_offset_in_reg);    //pad config: pulldown disabled
    pio_channel_regbase->pio_phdr_0 = (1 << channel_offset_in_reg);    //pad config: pullup disabled
    pio_channel_regbase->pio_per_0  = (1 << channel_offset_in_reg);    //pio enable
    
}
static void disable_pio_channel(uint8_t channel)
{
    MBED_ASSERT(channel != 0);
    struct pio_s *pio_channel_regbase;
    uint8_t channel_offset_in_reg;
    
    channel_offset_in_reg = channel & PIO_CHANNEL_SUB_32_MASK;
    
    if( (channel >> PIO_CHANNEL_OVER_32_SHIFT) == 0) {      
        pio_channel_regbase = (struct pio_s *)PIO_CONTROL_BASE;
    } else {          
        pio_channel_regbase = (struct pio_s *)(PIO_CONTROL_BASE + 0x220);
    }          
    pio_channel_regbase->pio_pdr_0 = (1 << channel_offset_in_reg);     //pio disable
}
/* ----------------------------------------------------------------
 * MBED API CALLS: SETUP FUNCTIONS
 * ----------------------------------------------------------------*/

void serial_init(serial_t *obj, PinName tx, PinName rx)
{    
    obj->rx_pin = rx;
    obj->tx_pin = tx;
        
    if (tx == UART1_TX) {
        serial_channel[0]=Uart1TXDxSIO_CHANNEL;
        serial_channel[1]=Uart1RXDxSIO_CHANNEL;
        config_pio_channel(serial_channel[0]);
        config_pio_channel(serial_channel[1]);
        obj->reg_base = app_ss_app_uart1;
        obj->index = IRQ_UART_ID_1;
        
    } else if (tx == UART2_TX) {    
        serial_channel[0]=Uart2TXDxSIO_CHANNEL;
        serial_channel[1]=Uart2RXDxSIO_CHANNEL;
        config_pio_channel(serial_channel[0]);
        config_pio_channel(serial_channel[1]);
        obj->reg_base = app_ss_app_uart2;
        obj->index = IRQ_UART_ID_2;
    }
    else {
        MBED_ASSERT(false);
    }
    
    /* flush FIFOs and disable interrupts */
    obj->reg_base->tffr = 0x0; 
    obj->reg_base->rffr = 0x0; 
    obj->reg_base->imrc = 0xFFFFFFF;
    //obj->reg_base->imrs = 0x201; //enable tx shift register empty and rx not empty interrupts 
    
    serial_baud(obj, 9600);
    serial_format(obj, 8, ParityNone, 1);
    
    if (tx == UART1_TX) {        
        /* The UART1 pins are the stdio pins */
        stdio_uart_inited = 1;
        stdio_uart = *obj;
    }        

}

void serial_free(serial_t *obj)
{
    if (obj->tx_pin == UART1_TX) {
        stdio_uart_inited = 0;
    }
    serial_irq_ids[obj->index] = 0;
    
    /* Release the port HW */
//    pin_function(obj->rx_pin, PIN_FUNCTION_UNCLAIMED); //pinnames and pinmap not yet implemented
//    pin_function(obj->tx_pin, PIN_FUNCTION_UNCLAIMED);
    disable_pio_channel(serial_channel[0]);
    disable_pio_channel(serial_channel[1]);     
    obj->reg_base->crc = 0xFFFFFFFF; //clear everything
    obj->reg_base->imrc = 0xFFFFFFFF; //disable all interrupts
    obj->reg_base = NULL;
}

void serial_baud(serial_t *obj, int baudrate)
{
    /*          uart baudrate calculation formula
    *   baudrate = (Pha * Fin) / OS * (Brdiv + 1) * 2^N
    *
    *   Pha: NCO phase accumulator value. Default is 0x80
    *   Fin: Uart ref clock frequency. Default is 26MHz
    *   OS: Oversampling rate. Default is 16
    *   Brdiv: BR register counter reload value ref. HAL doxygen
    *   N: Number of NCO bits i.e 8
    *   baudrate = (Pha * Fin) / 16 * (Brdiv + 1) * 2^8
    */    
    
    uint8_t nco_phase_acc_value = 0;
    uint16_t baudrate_divider_value = 0;
    uint32_t dividend = 0;
    uint32_t divisor = 0;    
    uint32_t pclk = UART_REF_FREQ;
    uint32_t req_baudrate = (uint32_t)baudrate;   
    const uint32_t nco_phase_acc_initial_shift_num = 7; 
    const uint32_t os_nco_bits_multiplier_shift_num = 12;     
    uint32_t pclk_shift_num = zeroBitNumFromLsb(pclk);
    uint32_t baudrate_shift_num = zeroBitNumFromLsb(req_baudrate);
    int32_t total_shift_num = 0; 
           
    pclk >>= pclk_shift_num;
    req_baudrate >>= baudrate_shift_num;
    
    /* calculating baudrate divider*/
    total_shift_num = (pclk_shift_num + nco_phase_acc_initial_shift_num - os_nco_bits_multiplier_shift_num - baudrate_shift_num);    
    if (total_shift_num >= 0) {
        dividend = pclk << total_shift_num;
        divisor = req_baudrate;
    }
    else {
        dividend = pclk;
        divisor = req_baudrate << (0 - total_shift_num);  
    }	
    //printf("step 1 calculate baudrate_divider_value, reqBaudrate=%d, dividend=%d, divisor=%d, totalShiftNum=%d,\n", baudrate, dividend, divisor, total_shift_num);    
    if (divisor != 0) {
        baudrate_divider_value = (dividend + (divisor >> 1)) / divisor;         
    }
    else {
        MBED_ASSERT(false);
    }
    
    /* fine tune nco phase accumulator value*/
    total_shift_num = baudrate_shift_num + os_nco_bits_multiplier_shift_num - pclk_shift_num;    
    if (total_shift_num >= 0)
    {
        dividend = (req_baudrate * baudrate_divider_value) << total_shift_num;
        divisor = pclk;
    }
    else
    {
        dividend = req_baudrate * baudrate_divider_value;
        divisor = pclk << (0 - total_shift_num);
    }
    //printf("step 2 calculate ncoPhase, reqBaudrate=%d, dividend=%d, divisor=%d, totalShiftNum=%d,\n", baudrate, dividend, divisor, total_shift_num);
    if (divisor != 0) {
        nco_phase_acc_value = (dividend + (divisor >> 1)) / divisor; 
    }
    else {
        MBED_ASSERT(false);
    }
    
    //MBED_ASSERT(!((baudrate_divider_value > DRIVER_BIT_MASK(UART_BR_BAUDRATE_SIZE)) || (nco_phase_acc_value > DRIVER_BIT_MASK(UART_NCO_NCO_PHASE_SIZE))));

    /* disable baudrate generator (uart control clear reg) */
    obj->reg_base->crc = DRIVER_BITFIELD_MASK(UART_CRC_BR_EN);
        
	/* disable DMA as we are using FIFO */
    obj->reg_base->crc = DRIVER_BITFIELD_MASK(UART_CRC_CR_RXDMA_EN);
    obj->reg_base->crc = DRIVER_BITFIELD_MASK(UART_CRC_CR_TXDMA_EN);
	
    /* set baudrate divider value */
	baudrate_divider_value--; /* the actual register value should minus 1 */
    obj->reg_base->br = DRIVER_BITFIELD_SET(obj->reg_base->br, UART_BR_BAUDRATE, baudrate_divider_value);

    /* set nco phase value */
    obj->reg_base->nco = DRIVER_BITFIELD_SET(obj->reg_base->nco, UART_NCO_NCO_PHASE, nco_phase_acc_value);
    
    /* enable baudrate generator (uart control set reg) */
    obj->reg_base->crs = DRIVER_BITFIELD_MASK(UART_CRC_BR_EN);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{    
    MBED_ASSERT((data_bits == UART_DATA_BITS_7) || (data_bits == UART_DATA_BITS_8));     
    /* Uart_IP supports 0.5, 1, 1.5 and 2 stop bits however SerialBase Class only allows 1 or 2 stop bits*/
    MBED_ASSERT((stop_bits == NUM_UART_STOP_BITS_1) || (stop_bits == NUM_UART_STOP_BITS_2));     
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven));

    uint32_t register_value=0;
    
    /* disable uart */ 
    register_value = (DRIVER_BITFIELD_MASK(UART_CRC_BR_EN) | DRIVER_BITFIELD_MASK(UART_CRC_RX_EN) | DRIVER_BITFIELD_MASK(UART_CRC_TX_EN));
    obj->reg_base->crc = register_value;
    
    /* read the existing value, change what is required and write once. This is done so that register access is only once not again and again */
    register_value = obj->reg_base->cr;
    
    /* set uart_driver_mode */
    if (parity == ParityNone) { /* Parity is disabled, set mode with parity disabled */        
        if (data_bits == UART_DATA_BITS_7) {
            register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_MODE, UART_CR_MODE_7NP_VALUE);            
        } else if (data_bits == UART_DATA_BITS_8) {
            register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_MODE, UART_CR_MODE_8NP_VALUE);            
        }       
    } else { /* Parity is enabled, set mode with parity enabled */        
        if (data_bits == UART_DATA_BITS_7) {
            register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_MODE, UART_CR_MODE_7P_VALUE);             
        } else if (data_bits == UART_DATA_BITS_8) {
            register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_MODE, UART_CR_MODE_8P_VALUE);            
        } 
                
        if (parity == ParityEven) { /* Set reg field depending upon parity even or odd */
            register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_PARITYMODE, UART_CR_PARITYMODE_EVEN_VALUE);            
        } else if (parity == ParityOdd) {
            register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_PARITYMODE, UART_CR_PARITYMODE_ODD_VALUE);            
        }        
    }
    
    /* set uart_stop_bits */
    if (stop_bits == NUM_UART_STOP_BITS_1) {
        register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_RX_STOPBITS, UART_CR_RX_STOPBITS_10_VALUE);      
        register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_TX_STOPBITS, UART_CR_RX_STOPBITS_10_VALUE);   // macro UART_CR_TX_STOPBITS_10_VALUE is not defined in uart.h but since possible values for both UART_CR_RX_STOPBITS and UART_CR_TX_STOPBITS are the same so it is not necessary.   
    } else if (stop_bits == NUM_UART_STOP_BITS_2) {
        register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_RX_STOPBITS, UART_CR_RX_STOPBITS_20_VALUE);      
        register_value = DRIVER_BITFIELD_SET(register_value, UART_CR_TX_STOPBITS, UART_CR_RX_STOPBITS_20_VALUE);           
    }
             
    /* enable uart */ 
    register_value |= (DRIVER_BITFIELD_MASK(UART_CRS_BR_EN) | DRIVER_BITFIELD_MASK(UART_CRS_RX_EN) | DRIVER_BITFIELD_MASK(UART_CRS_TX_EN));
    
    /* write final value to register */
    obj->reg_base->cr = register_value;
}

/* ----------------------------------------------------------------
 * MBED API CALLS: INTERRUPT FUNCTIONS
 * ----------------------------------------------------------------*/

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    /* TODO: does uart tx/rx and baudrate generator need to be disabled? */
    obj->reg_base->crc = (/*DRIVER_BITFIELD_MASK(UART_CRC_BR_EN) | */DRIVER_BITFIELD_MASK(UART_CRC_RX_EN) | DRIVER_BITFIELD_MASK(UART_CRC_TX_EN));

    if (enable) {
        switch (irq) {
           case RxIrq:
            {
                /* rx fifo not empty interrupt enable */
                obj->reg_base->imrs = 0x01;
                obj->irq_rx_setting = IRQ_ON;
                irq_enable(obj);
            }
            break;
            case TxIrq:
            {
                /* tx shift register empty interrupt enable */
                obj->reg_base->imrs = 0x200;
                obj->irq_tx_setting = IRQ_ON;
                irq_enable(obj);
            }
            break;
            default:
            {
                MBED_ASSERT(false);
            }
            break;
        }
    } else {
        switch (irq) {
            case RxIrq:
            {
                /* rx fifo not empty interrupt disable */
                obj->reg_base->imrc = 0x01;
                obj->irq_rx_setting = IRQ_OFF;
            }
            break;
            case TxIrq:
            {
                /* tx shift register empty interrupt disable */
                obj->reg_base->imrc = 0x200;
                obj->irq_tx_setting = IRQ_OFF;
            }
            break;
            default:
            {
                MBED_ASSERT(false);
            }
            break;
        }
         if ((obj->irq_rx_setting == IRQ_OFF) && (obj->irq_tx_setting == IRQ_OFF)) {
            irq_disable(obj);
        }
    }
     /* Enable the UART again */
    obj->reg_base->crs = (DRIVER_BITFIELD_MASK(UART_CRC_BR_EN) | DRIVER_BITFIELD_MASK(UART_CRC_RX_EN) | DRIVER_BITFIELD_MASK(UART_CRC_TX_EN));
}


/* Set the NVIC bits */
static void irq_enable(serial_t *obj)
{
    switch (obj->tx_pin) {
        case UART1_TX:
        {
            NVIC_EnableIRQ(APP_CPU_APP_IRQ_UART1_INT_IRQn);
        }
        break;
        case UART2_TX:
        {
            NVIC_EnableIRQ(APP_CPU_APP_IRQ_UART2_INT_IRQn);
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }
}

/* Unset the NVIC bits */
static void irq_disable(serial_t *obj)
{
    switch (obj->tx_pin) {
        case UART1_TX:
        {
            NVIC_DisableIRQ(APP_CPU_APP_IRQ_UART1_INT_IRQn);
        }
        break;
        case UART2_TX:
        {
            NVIC_DisableIRQ(APP_CPU_APP_IRQ_UART2_INT_IRQn);
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }
}



/* UART1 IRQ */
void APP_CPU_APP_IRQ_UART1_INT_IRQHandler()
{
    uint32_t id = serial_irq_ids[IRQ_UART_ID_1];

    /* Check Rx interrupt */
    if (app_ss_app_uart1->imr & (1 << 0) ) {
        if (id != 0) {
            irq_handler(id, RxIrq);
            /* TODO: does the interrupt clear on its own?? */
        }
    }
    /* Check Tx bit */
    if (app_ss_app_uart1->imr & (1 << 9)) {
        if (id != 0) {
            irq_handler(id, TxIrq);
            /* TODO: does the interrupt clear on its own?? */
        }
        /* Not sure what clears the interrupt so clear it explicitly */
        NVIC_ClearPendingIRQ(APP_CPU_APP_IRQ_UART1_INT_IRQn);
    }
}

/* UART2 IRQ */
void APP_CPU_APP_IRQ_UART2_INT_IRQHandler()
{
    uint32_t id = serial_irq_ids[IRQ_UART_ID_2];

    /* Check Rx interrupt */
    if (app_ss_app_uart2->imr & (1 << 0) ) {
        if (id != 0) {
            irq_handler(id, RxIrq);
            /* TODO: does the interrupt clear on its own?? */
        }
    }
    /* Check Tx bit */
    if (app_ss_app_uart2->imr & (1 << 9)) {
        if (id != 0) {
            irq_handler(id, TxIrq);
            /* TODO: does the interrupt clear on its own?? */
        }
        /* Not sure what clears the interrupt so clear it explicitly */
        NVIC_ClearPendingIRQ(APP_CPU_APP_IRQ_UART2_INT_IRQn);
    }
}

/* ----------------------------------------------------------------
 * MBED API CALLS: TRANSMIT AND RECEIVE FUNCTIONS
 * ----------------------------------------------------------------*/

int serial_getc(serial_t *obj)
{
    uint8_t data = 0;

    /* Block until there is data to read */
    while (!serial_readable(obj)) {}        
    data = (uint8_t) obj->reg_base->rhr;
        
    return (int)data;
}

void serial_putc(serial_t *obj, int c)
{
    /* Block until there is room to write */
    while (!serial_writable(obj)) {}

    /* Write the data */
    obj->reg_base->thr = (uint8_t) c;
}

int serial_readable(serial_t *obj)
{
    if (DRIVER_BITFIELD_GET(obj->reg_base->rwcr, UART_RWCR_NONE) != 0) {
       return true;
    } else {    
       return false;
    }
}

int serial_writable(serial_t *obj)
{    
    /* only if this is 2 can we transmit */
    if (DRIVER_BITFIELD_GET(obj->reg_base->twcr, UART_TWCR_NONE) == 2) {
       return true;
    } else {    
       return false;
    }
}

void serial_break_set(serial_t *obj)
{
    // /* Disable UART while writing to control registers */
    // obj->reg_base->UARTCR &= ~(1 << 0);

    // /* Set bit 1 of the line control register */
    // obj->reg_base->UARTLCR_H |= 1 << 0;

    // /* Enable the UART again */
    // obj->reg_base->UARTCR |= 1 << 0;
}

void serial_break_clear(serial_t *obj)
{
    // /* Disable UART while writing to control registers */
    // obj->reg_base->UARTCR &= ~(1 << 0);

    // /* Clear bit 1 of the line control register */
    // obj->reg_base->UARTLCR_H &= ~(1 << 0);

    // /* Enable the UART again */
    // obj->reg_base->UARTCR |= 1 << 0;
}
