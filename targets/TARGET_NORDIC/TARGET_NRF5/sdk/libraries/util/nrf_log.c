#include "nrf.h"
#include "nrf_log.h"
#include "nrf_error.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#if defined(NRF_LOG_USES_RTT) && NRF_LOG_USES_RTT == 1

#include <SEGGER_RTT_Conf.h>
#include <SEGGER_RTT.h>

static char buf_normal_up[BUFFER_SIZE_UP];
static char buf_down[BUFFER_SIZE_DOWN];

uint32_t log_rtt_init(void)
{
    static bool initialized = false;
    if (initialized)
    {
        return NRF_SUCCESS;
    }

    if (SEGGER_RTT_ConfigUpBuffer(LOG_TERMINAL_NORMAL,
                                  "Normal",
                                  buf_normal_up,
                                  BUFFER_SIZE_UP,
                                  SEGGER_RTT_MODE_NO_BLOCK_TRIM
                                 )
        != 0)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (SEGGER_RTT_ConfigDownBuffer(LOG_TERMINAL_INPUT,
                                   "Input",
                                   buf_down,
                                   BUFFER_SIZE_DOWN,
                                   SEGGER_RTT_MODE_NO_BLOCK_SKIP
                                  )
        != 0)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    initialized = true;

    return NRF_SUCCESS;
}

// Forward declaration of SEGGER RTT vprintf function
int SEGGER_RTT_vprintf(unsigned BufferIndex, const char * sFormat, va_list * pParamList);

void log_rtt_printf(int terminal_index, char * format_msg, ...)
{
    //lint -save -e526 -e628 -e530
    va_list p_args;
    va_start(p_args, format_msg);
    (void)SEGGER_RTT_vprintf(terminal_index, format_msg, &p_args);
    va_end(p_args);
    //lint -restore
}

__INLINE void log_rtt_write_string(int terminal_index, int num_args, ...)
{
    const char* msg;
    //lint -save -e516 -e530
    va_list p_args;
    va_start(p_args, num_args);
    //lint -restore

    for (int i = 0; i < num_args; i++)
    {
        //lint -save -e26 -e10 -e64 -e526 -e628 -e530
        msg = va_arg(p_args, const char*);
        //lint -restore
        (void)SEGGER_RTT_WriteString(terminal_index, msg);
    }
    va_end(p_args);
}

void log_rtt_write_hex(int terminal_index, uint32_t value)
{
    char temp[11];
    temp[0] = '0';
    temp[1] = 'x';
    temp[10] = 0; // Null termination
    uint8_t nibble;
    uint8_t i = 8;

    while(i-- != 0)
    {
        nibble = (value >> (4 * i)) & 0x0F;
        temp[9-i] = (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble);
    }

    (void)SEGGER_RTT_WriteString(terminal_index, temp);
}

void log_rtt_write_hex_char(int terminal_index, uint8_t value)
{
    char temp[3];
    temp[2] = 0; // Null termination
    uint8_t nibble;
    uint8_t i = 2;

    while(i-- != 0)
    {
        nibble = (value >> (4 * i)) & 0x0F;
        temp[1-i] = (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble);
    }

    (void)SEGGER_RTT_WriteString(terminal_index, temp);
}

__INLINE int log_rtt_has_input()
{
    return SEGGER_RTT_HasKey();
}

uint32_t log_rtt_read_input(char * c)
{
    int r;

    r = SEGGER_RTT_Read(LOG_TERMINAL_INPUT, c, 1);
    if (r == 1)
        return NRF_SUCCESS;
    else
        return NRF_ERROR_NULL;
}

#elif defined(NRF_LOG_USES_UART) && NRF_LOG_USES_UART == 1

#include "app_uart.h"
#include "app_error.h"
#include <stdio.h>
#include <string.h>
#include "nrf.h"
#include "bsp.h"

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 512                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */

static uint8_t m_uart_data;
static bool m_uart_has_input;

void uart_error_cb(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

uint32_t log_uart_init()
{
    static bool initialized = false;
    if (initialized)
    {
        return NRF_SUCCESS;
    }

    uint32_t err_code;
    const app_uart_comm_params_t comm_params =
    {
        RX_PIN_NUMBER,
        TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        APP_UART_FLOW_CONTROL_ENABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud115200
    };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_cb,
#ifdef NRF51
                         APP_IRQ_PRIORITY_LOW
#elif defined(NRF52)
                         APP_IRQ_PRIORITY_LOWEST
#endif
                         err_code);

    initialized = true;

    return err_code;
}

//lint -save -e530 -e64
void log_uart_printf(const char * format_msg, ...)
{
    va_list p_args;
    va_start(p_args, format_msg);
    (void)vprintf(format_msg, p_args);
    va_end(p_args);
}

__INLINE void log_uart_write_string_many(int num_args, ...)
{
    const char* msg;
    va_list p_args;
    va_start(p_args, num_args);

    for (int i = 0; i < num_args; i++)
    {
        msg = va_arg(p_args, const char*);
        log_uart_write_string(msg);
    }
    va_end(p_args);
}

__INLINE void log_uart_write_string(const char* msg)
{
    while( *msg )
    {
        (void)app_uart_put(*msg++);
    }
}
//lint -restore

void log_uart_write_hex(uint32_t value)
{
    uint8_t nibble;
    uint8_t i = 8;

    (void)app_uart_put('0');
    (void)app_uart_put('x');
    while( i-- != 0 )
    {
        nibble = (value >> (4 * i)) & 0x0F;
        (void)app_uart_put( (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble) );
    }
}

void log_uart_write_hex_char(uint8_t c)
{
    uint8_t nibble;
    uint8_t i = 2;

    while( i-- != 0 )
    {
        nibble = (c >> (4 * i)) & 0x0F;
        (void)app_uart_put( (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble) );
    }
}

__INLINE int log_uart_has_input()
{
    if (m_uart_has_input) return 1;
    if (app_uart_get(&m_uart_data) == NRF_SUCCESS)
    {
        m_uart_has_input = true;
        return 1;
    }
    return 0;
}

uint32_t log_uart_read_input(char * c)
{
    if (m_uart_has_input)
    {
        *c = (char)m_uart_data;
        m_uart_has_input = false;
        return NRF_SUCCESS;
    }
    if (app_uart_get((uint8_t *)c) == NRF_SUCCESS)
    {
        return NRF_SUCCESS;
    }
    return NRF_ERROR_NULL;
}

#elif defined(NRF_LOG_USES_RAW_UART) && NRF_LOG_USES_RAW_UART == 1

#include "app_uart.h"
#include <stdio.h>
#include <string.h>
#include "bsp.h"

uint32_t log_raw_uart_init()
{
    // Disable UART
    NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Disabled;

    // Configure RX/TX pins
    nrf_gpio_cfg_output( TX_PIN_NUMBER );
    nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);

    // Set a default baud rate of UART0_CONFIG_BAUDRATE
    NRF_UART0->PSELTXD  = TX_PIN_NUMBER;
    NRF_UART0->BAUDRATE = UART0_CONFIG_BAUDRATE;

    NRF_UART0->PSELRTS  = 0xFFFFFFFF;
    NRF_UART0->PSELCTS  = 0xFFFFFFFF;

    // Disable parity and interrupt
    NRF_UART0->CONFIG   = (UART_CONFIG_PARITY_Excluded  << UART_CONFIG_PARITY_Pos );
    NRF_UART0->CONFIG  |= (UART_CONFIG_HWFC_Disabled    << UART_CONFIG_HWFC_Pos   );

    // Re-enable the UART
    NRF_UART0->ENABLE           = UART_ENABLE_ENABLE_Enabled;
    NRF_UART0->INTENSET         = 0;
    NRF_UART0->TASKS_STARTTX    = 1;
    NRF_UART0->TASKS_STARTRX    = 1;

    return NRF_SUCCESS;
}

void log_raw_uart_printf(const char * format_msg, ...)
{
    static char buffer[256];

    va_list p_args;
    va_start(p_args, format_msg);
    sprintf(buffer, format_msg, p_args);
    va_end(p_args);

    log_raw_uart_write_string(buffer);
}

__INLINE void log_raw_uart_write_char(const char c)
{
    NRF_UART0->TXD = c;
    while( NRF_UART0->EVENTS_TXDRDY != 1 );
    NRF_UART0->EVENTS_TXDRDY = 0;
}

__INLINE void log_raw_uart_write_string_many(int num_args, ...)
{

    const char* msg;
    va_list p_args;
    va_start(p_args, num_args);

    for (int i = 0; i < num_args; i++)
    {
        msg = va_arg(p_args, const char*);
        log_raw_uart_write_string(msg);
    }
    va_end(p_args);
}

__INLINE void log_raw_uart_write_string(const char* msg)
{
    while( *msg )
    {
        NRF_UART0->TXD = *msg++;
        while( NRF_UART0->EVENTS_TXDRDY != 1 );
        NRF_UART0->EVENTS_TXDRDY = 0;
    }
}

void log_raw_uart_write_hex(uint32_t value)
{
    uint8_t nibble;
    uint8_t i = 8;

    log_raw_uart_write_string( "0x" );
    while( i-- != 0 )
    {
        nibble = (value >> (4 * i)) & 0x0F;
        log_raw_uart_write_char( (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble) );
    }
}

void log_raw_uart_write_hex_char(uint8_t c)
{
    uint8_t nibble;
    uint8_t i = 2;

    while( i-- != 0 )
    {
        nibble = (c >> (4 * i)) & 0x0F;
        log_raw_uart_write_hex( (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble) );
    }
}

__INLINE int log_raw_uart_has_input()
{
    return 0;
}

uint32_t log_raw_uart_read_input(char * c)
{
    return NRF_ERROR_NULL;
}

#endif // NRF_LOG_USES_RAW_UART == 1


const char* log_hex_char(const char c)
{
    static volatile char hex_string[3];
    hex_string[2] = 0; // Null termination
    uint8_t nibble;
    uint8_t i = 2;
    while(i-- != 0)
    {
        nibble = (c >> (4 * i)) & 0x0F;
        hex_string[1-i] = (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble);
    }
    return (const char*) hex_string;
}

const char* log_hex(uint32_t value)
{
    static volatile char hex_string[11];
    hex_string[0] = '0';
    hex_string[1] = 'x';
    hex_string[10] = 0;
    uint8_t nibble;
    uint8_t i = 8;

    while(i-- != 0)
    {
        nibble = (value >> (4 * i)) & 0x0F;
        hex_string[9-i] = (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble);
    }

    return (const char*)hex_string;
}

