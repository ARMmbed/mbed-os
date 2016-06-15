/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "ble_dtm.h"
#include <stdbool.h>
#include <string.h>
#include "nrf.h"

#define DTM_HEADER_OFFSET        0                                         /**< Index where the header of the pdu is located. */
#define DTM_HEADER_SIZE          2                                         /**< Size of PDU header. */
#define DTM_PAYLOAD_MAX_SIZE     37                                        /**< Maximum payload size allowed during dtm execution. */
#define DTM_LENGTH_OFFSET        (DTM_HEADER_OFFSET + 1)                   /**< Index where the length of the payload is encoded. */
#define DTM_PDU_MAX_MEMORY_SIZE  (DTM_HEADER_SIZE + DTM_PAYLOAD_MAX_SIZE)  /**< Maximum PDU size allowed during dtm execution. */

/**@details The UART poll cycle in micro seconds. 
 *          Baud rate of 19200 bits / second, and 8 data bits, 1 start/stop bit, no flow control, 
 *          give the time to transmit a byte: 10 bits * 1/19200 = approx: 520 us. 
 *
 *          To ensure no loss of bytes, the UART should be polled every 260 us. 
 *
 * @note If UART bit rate is changed, this value should be recalculated as well.
 */
#define UART_POLL_CYCLE  260

#define RX_MODE          true   /**< Constant defining RX mode for radio during dtm test. */
#define TX_MODE          false  /**< Constant defining TX mode for radio during dtm test. */

#define PHYS_CH_MAX      39     /**< Maximum number of valid channels in BLE. */

// Values that for now are "constants" - they could be configured by a function setting them,
// but most of these are set by the BLE DTM standard, so changing them is not relevant.
#define RFPHY_TEST_0X0F_REF_PATTERN  0x0f  /**<  RF-PHY test packet patterns, for the repeated octet packets. */
#define RFPHY_TEST_0X55_REF_PATTERN  0x55  /**<  RF-PHY test packet patterns, for the repeated octet packets. */

#define PRBS9_CONTENT {0xff, 0xc1, 0xfb, 0xe8, 0x4c, 0x90, 0x72, 0x8b,  \
                       0xe7, 0xb3, 0x51, 0x89, 0x63, 0xab, 0x23, 0x23,  \
                       0x2,  0x84, 0x18, 0x72, 0xaa, 0x61, 0x2f, 0x3b,  \
                       0x51, 0xa8, 0xe5, 0x37, 0x49, 0xfb, 0xc9, 0xca,  \
                       0xc,  0x18, 0x53, 0x2c, 0xfd}                         /**< The PRBS9 sequence used as packet payload. */

/**@brief Structure holding the PDU used for transmitting/receiving a PDU.
 */
typedef struct
{
    uint8_t content[DTM_HEADER_SIZE + DTM_PAYLOAD_MAX_SIZE];                 /**< PDU packet content. */
} pdu_type_t;

/**@brief States used for the DTM test implementation.
 */
typedef enum
{
    STATE_UNINITIALIZED,                                                     /**< The DTM is uninitialized. */
    STATE_IDLE,                                                              /**< State when system has just initialized, or current test has completed. */
    STATE_TRANSMITTER_TEST,                                                  /**< State used when a DTM Transmission test is running. */
    STATE_CARRIER_TEST,                                                      /**< State used when a DTM Carrier test is running (Vendor specific test). */
    STATE_RECEIVER_TEST                                                      /**< State used when a DTM Receive test is running. */
} state_t;

// Internal variables set as side effects of commands or events.
static state_t           m_state = STATE_UNINITIALIZED;                      /**< Current machine state. */
static uint16_t          m_rx_pkt_count;                                     /**< Number of valid packets received. */
static pdu_type_t        m_pdu;                                              /**< PDU to be sent. */
static uint16_t          m_event;                                            /**< current command status - initially "ok", may be set if error detected, or to packet count. */
static bool              m_new_event;                                        /**< Command has been processed - number of not yet reported event bytes. */
static uint8_t           m_packet_length;                                    /**< Payload length of transmitted PDU, bits 2:7 of 16-bit dtm command. */
static dtm_pkt_type_t    m_packet_type;                                      /**< Bits 0..1 of 16-bit transmit command, or 0xFFFFFFFF. */
static dtm_freq_t        m_phys_ch;                                          /**< 0..39 physical channel number (base 2402 MHz, Interval 2 MHz), bits 8:13 of 16-bit dtm command. */
static uint32_t          m_current_time = 0;                                 /**< Counter for interrupts from timer to ensure that the 2 bytes forming a DTM command are received within the time window. */

// Nordic specific configuration values (not defined by BLE standard).
// Definition of initial values found in ble_dtm.h
static int32_t           m_tx_power          = DEFAULT_TX_POWER;             /**< TX power for transmission test, default to maximum value (+4 dBm). */
static NRF_TIMER_Type *  mp_timer            = DEFAULT_TIMER;                /**< Timer to be used. */
static IRQn_Type         m_timer_irq         = DEFAULT_TIMER_IRQn;           /**< which interrupt line to clear on every timeout */

static uint8_t const     m_prbs_content[]    = PRBS9_CONTENT;                /**< Pseudo-random bit sequence defined by the BLE standard. */
static uint8_t           m_packetHeaderLFlen = 8;                            /**< Length of length field in packet Header (in bits). */
static uint8_t           m_packetHeaderS0len = 1;                            /**< Length of S0 field in packet Header (in bytes). */
static uint8_t           m_packetHeaderS1len = 0;                            /**< Length of S1 field in packet Header (in bits). */
static uint8_t           m_crcConfSkipAddr   = 1;                            /**< Leave packet address field out of CRC calculation. */
static uint8_t           m_static_length     = 0;                            /**< Number of bytes sent in addition to the var.length payload. */
static uint32_t          m_balen             = 3;                            /**< Base address length in bytes. */
static uint32_t          m_endian            = RADIO_PCNF1_ENDIAN_Little;    /**< On air endianess of packet, this applies to the S0, LENGTH, S1 and the PAYLOAD fields. */
static uint32_t          m_whitening         = RADIO_PCNF1_WHITEEN_Disabled; /**< Whitening disabled. */
static uint8_t           m_crcLength         = RADIO_CRCCNF_LEN_Three;       /**< CRC Length (in bytes). */
static uint32_t          m_address           = 0x71764129;                   /**< Address. */
static uint32_t          m_crc_poly          = 0x0000065B;                   /**< CRC polynomial. */
static uint32_t          m_crc_init          = 0x00555555;                   /**< Initial value for CRC calculation. */
static uint8_t           m_radio_mode        = RADIO_MODE_MODE_Ble_1Mbit;    /**< nRF51 specific radio mode vale. */
static uint32_t          m_txIntervaluS      = 625;                          /**< Time between start of Tx packets (in uS). */


/**@brief Function for verifying that a received PDU has the expected structure and content.
 */
static bool check_pdu(void)
{
    uint8_t        k;                // Byte pointer for running through PDU payload
    uint8_t        pattern;          // Repeating octet value in payload
    dtm_pkt_type_t pdu_packet_type;  // Note: PDU packet type is a 4-bit field in HCI, but 2 bits in BLE DTM
    uint8_t        length;

    pdu_packet_type = (dtm_pkt_type_t)(m_pdu.content[DTM_HEADER_OFFSET] & 0x0F);
    length          = m_pdu.content[DTM_LENGTH_OFFSET];

    if ((pdu_packet_type > (dtm_pkt_type_t)PACKET_TYPE_MAX) || (length > DTM_PAYLOAD_MAX_SIZE))
    {
        return false;
    }

    if (pdu_packet_type == DTM_PKT_PRBS9)
    {
        // Payload does not consist of one repeated octet; must compare ir with entire block into
        return (memcmp(m_pdu.content+DTM_HEADER_SIZE, m_prbs_content, length) == 0);
    }

    if (pdu_packet_type == DTM_PKT_0X0F)
    {
        pattern = RFPHY_TEST_0X0F_REF_PATTERN;
    }
    else
    {
        pattern = RFPHY_TEST_0X55_REF_PATTERN;
    }

    for (k = 0; k < length; k++)
    {
        // Check repeated pattern filling the PDU payload 
        if (m_pdu.content[k + 2] != pattern)
        {
            return false;
        }
    }
    return true;
}


/**@brief Function for turning off the radio after a test.
 *        Also called after test done, to be ready for next test.
 */
static void radio_reset(void)
{
    NRF_PPI->CHENCLR = PPI_CHENCLR_CH0_Msk | PPI_CHENCLR_CH1_Msk;

    NRF_RADIO->SHORTS          = 0;
    NRF_RADIO->EVENTS_DISABLED = 0;
    NRF_RADIO->TASKS_DISABLE   = 1;

    while (NRF_RADIO->EVENTS_DISABLED == 0)
    {
        // Do nothing
    }

    NRF_RADIO->EVENTS_DISABLED = 0;
    NRF_RADIO->TASKS_RXEN      = 0;
    NRF_RADIO->TASKS_TXEN      = 0;

    m_rx_pkt_count = 0;
}


/**@brief Function for initializing the radio for DTM.
 */
static uint32_t radio_init(void)
{
#ifdef NRF51
    // Handle BLE Radio tuning parameters from production for DTM if required.
    // Only needed for DTM without SoftDevice, as the SoftDevice normally handles this.
    // PCN-083.
    if ( ((NRF_FICR->OVERRIDEEN) & FICR_OVERRIDEEN_BLE_1MBIT_Msk) == FICR_OVERRIDEEN_BLE_1MBIT_Override)
    {
        NRF_RADIO->OVERRIDE0 = NRF_FICR->BLE_1MBIT[0];
        NRF_RADIO->OVERRIDE1 = NRF_FICR->BLE_1MBIT[1];
        NRF_RADIO->OVERRIDE2 = NRF_FICR->BLE_1MBIT[2];
        NRF_RADIO->OVERRIDE3 = NRF_FICR->BLE_1MBIT[3];
        NRF_RADIO->OVERRIDE4 = NRF_FICR->BLE_1MBIT[4];
    }
#endif // NRF51

    // Initializing code below is quite generic - for BLE, the values are fixed, and expressions
    // are constant. Non-constant values are essentially set in radio_prepare().
    if (((m_tx_power & 0x03) != 0)           ||      // tx_power should be a multiple of 4
         ((m_tx_power & 0xffffff00) != 0)    ||      // Upper 24 bits are required to be zeroed
         ((int8_t)m_tx_power > 4)            ||      // Max tx_power is +4 dBm
         ((int8_t)(m_tx_power & 0xff) < -40) ||      // Min tx_power is -40 dBm
         (m_radio_mode > RADIO_MODE_MODE_Ble_1Mbit)  // Values 0-2: Proprietary mode, 3 (last valid): BLE
       )
    {
        return DTM_ERROR_ILLEGAL_CONFIGURATION;
    }

    // Turn off radio before configuring it
    radio_reset();

    NRF_RADIO->TXPOWER = m_tx_power;
    NRF_RADIO->MODE    = m_radio_mode << RADIO_MODE_MODE_Pos;

    // Set the access address, address0/prefix0 used for both Rx and Tx address
    NRF_RADIO->PREFIX0    &= ~RADIO_PREFIX0_AP0_Msk;
    NRF_RADIO->PREFIX0    |= (m_address >> 24) & RADIO_PREFIX0_AP0_Msk;
    NRF_RADIO->BASE0       = m_address << 8;
    NRF_RADIO->RXADDRESSES = RADIO_RXADDRESSES_ADDR0_Enabled << RADIO_RXADDRESSES_ADDR0_Pos;
    NRF_RADIO->TXADDRESS   = (0x00 << RADIO_TXADDRESS_TXADDRESS_Pos) & RADIO_TXADDRESS_TXADDRESS_Msk;

    // Configure CRC calculation
    NRF_RADIO->CRCCNF = (m_crcConfSkipAddr << RADIO_CRCCNF_SKIP_ADDR_Pos) |
                        (m_crcLength << RADIO_CRCCNF_LEN_Pos);

    NRF_RADIO->PCNF0 = (m_packetHeaderS1len << RADIO_PCNF0_S1LEN_Pos) |
                       (m_packetHeaderS0len << RADIO_PCNF0_S0LEN_Pos) |
                       (m_packetHeaderLFlen << RADIO_PCNF0_LFLEN_Pos);

    NRF_RADIO->PCNF1 = (m_whitening          << RADIO_PCNF1_WHITEEN_Pos) |
                       (m_endian             << RADIO_PCNF1_ENDIAN_Pos)  |
                       (m_balen              << RADIO_PCNF1_BALEN_Pos)   |
                       (m_static_length      << RADIO_PCNF1_STATLEN_Pos) |
                       (DTM_PAYLOAD_MAX_SIZE << RADIO_PCNF1_MAXLEN_Pos);

    return DTM_SUCCESS;
}


/**@brief Function for preparing the radio. At start of each test: Turn off RF, clear interrupt flags of RF, initialize the radio
 *        at given RF channel.
 *
 *@param[in] rx     boolean indicating if radio should be prepared in rx mode (true) or tx mode.
 */
static void radio_prepare(bool rx)
{
#ifdef NRF51
    NRF_RADIO->TEST         = 0;
#elif defined(NRF52)
    NRF_RADIO->MODECNF0     = (RADIO_MODECNF0_RU_Default << RADIO_MODECNF0_RU_Pos) |
                              (RADIO_MODECNF0_DTX_B1 << RADIO_MODECNF0_DTX_Pos);
#endif // NRF51 / NRF52
    NRF_RADIO->CRCPOLY      = m_crc_poly;
    NRF_RADIO->CRCINIT      = m_crc_init;
    NRF_RADIO->FREQUENCY    = (m_phys_ch << 1) + 2;                  // Actual frequency (MHz): 2400 + register value
    NRF_RADIO->PACKETPTR    = (uint32_t)&m_pdu;                      // Setting packet pointer will start the radio
    NRF_RADIO->EVENTS_READY = 0;
    NRF_RADIO->SHORTS       = (1 << RADIO_SHORTS_READY_START_Pos) |  // Shortcut between READY event and START task
                              (1 << RADIO_SHORTS_END_DISABLE_Pos);   // Shortcut between END event and DISABLE task

    if (rx)
    {
        NRF_RADIO->EVENTS_END = 0;
        NRF_RADIO->TASKS_RXEN = 1;  // shorts will start radio in RX mode when it is ready
    }
    else // tx
    {
        NRF_RADIO->TXPOWER = m_tx_power;
    }
}


/**@brief Function for terminating the ongoing test (if any) and closing down the radio.
 */
static void dtm_test_done(void)
{
#ifdef NRF51
    NRF_RADIO->TEST = 0;
#elif defined(NRF52)
    NRF_RADIO->MODECNF0 = (RADIO_MODECNF0_RU_Default << RADIO_MODECNF0_RU_Pos) |
                          (RADIO_MODECNF0_DTX_B1 << RADIO_MODECNF0_DTX_Pos);
#endif // NRF51 / NRF52
    NRF_PPI->CHENCLR = 0x01;
    NRF_PPI->CH[0].EEP = 0;     // Break connection from timer to radio to stop transmit loop
    NRF_PPI->CH[0].TEP = 0;

    radio_reset();
    m_state = STATE_IDLE;
}


/**@brief Function for configuring the timer for 625us cycle time.
 */
static uint32_t timer_init(void)
{
    // Use 16MHz from external crystal
    // This could be customized for RC/Xtal, or even to use a 32 kHz crystal
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing while waiting for the clock to start
    }

    mp_timer->TASKS_STOP        = 1;                      // Stop timer, if it was running
    mp_timer->TASKS_CLEAR       = 1;
    mp_timer->MODE              = TIMER_MODE_MODE_Timer;  // Timer mode (not counter)
    mp_timer->EVENTS_COMPARE[0] = 0;                      // clean up possible old events
    mp_timer->EVENTS_COMPARE[1] = 0;
    mp_timer->EVENTS_COMPARE[2] = 0;
    mp_timer->EVENTS_COMPARE[3] = 0;

    // Timer is polled, but enable the compare0 interrupt in order to wakeup from CPU sleep
    mp_timer->INTENSET    = TIMER_INTENSET_COMPARE0_Msk;
    mp_timer->SHORTS      = 1 << TIMER_SHORTS_COMPARE0_CLEAR_Pos;  // Clear the count every time timer reaches the CCREG0 count
    mp_timer->PRESCALER   = 4;                                     // Input clock is 16MHz, timer clock = 2 ^ prescale -> interval 1us
    mp_timer->CC[0]       = m_txIntervaluS;                        // 625uS with 1MHz clock to the timer
    mp_timer->CC[1]       = UART_POLL_CYCLE;                       // 260uS with 1MHz clock to the timer
    mp_timer->TASKS_START = 1;                                     // Start the timer - it will be running continuously
    m_current_time        = 0;
    return DTM_SUCCESS;
}


/**@brief Function for handling vendor specific commands.
 *        Used when packet type is set to Vendor specific.
 *        The length field is used for encoding vendor specific command.
 *        The frequency field is used for encoding vendor specific options to the command.
 *
 * @param[in]   vendor_cmd      Vendor specific command to be executed.
 * @param[in]   vendor_option   Vendor specific option to the vendor command.
 *
 * @return      DTM_SUCCESS or one of the DTM_ERROR_ values
 */
static uint32_t dtm_vendor_specific_pkt(uint32_t vendor_cmd, dtm_freq_t vendor_option)
{
    switch (vendor_cmd)
    {
        // nRFgo Studio uses CARRIER_TEST_STUDIO to indicate a continuous carrier without
        // a modulated signal.
        case CARRIER_TEST:
        case CARRIER_TEST_STUDIO:
            // Not a packet type, but used to indicate that a continuous carrier signal
            // should be transmitted by the radio.
            radio_prepare(TX_MODE);
#ifdef NRF51
            NRF_RADIO->TEST = (RADIO_TEST_PLL_LOCK_Enabled << RADIO_TEST_PLL_LOCK_Pos) |
                              (RADIO_TEST_CONST_CARRIER_Enabled << RADIO_TEST_CONST_CARRIER_Pos);
#elif defined(NRF52)
            NRF_RADIO->MODECNF0 = (RADIO_MODECNF0_RU_Default << RADIO_MODECNF0_RU_Pos) |
                                  (RADIO_MODECNF0_DTX_B1 << RADIO_MODECNF0_DTX_Pos);
#endif // NRF51 / NRF52

            // Shortcut between READY event and START task
            NRF_RADIO->SHORTS = 1 << RADIO_SHORTS_READY_START_Pos;

            // Shortcut will start radio in Tx mode when it is ready
            NRF_RADIO->TASKS_TXEN = 1;
            m_state               = STATE_CARRIER_TEST;
            break;

        case SET_TX_POWER:
            if (!dtm_set_txpower(vendor_option))
            {
                return DTM_ERROR_ILLEGAL_CONFIGURATION;
            }
            break;

        case SELECT_TIMER:
            if (!dtm_set_timer(vendor_option))
            {
                return DTM_ERROR_ILLEGAL_CONFIGURATION;
            }
            break;
    }
    // Event code is unchanged, successful
    return DTM_SUCCESS;
}


uint32_t dtm_init(void)
{
    if ((timer_init() != DTM_SUCCESS) || (radio_init() != DTM_SUCCESS))
    {
        return DTM_ERROR_ILLEGAL_CONFIGURATION;
    }
    m_new_event = false;
    m_state     = STATE_IDLE;

    // Enable wake-up on event
    SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

    return DTM_SUCCESS;
}


uint32_t dtm_wait(void)
{
    // Enable wake-up on event
    SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

    for (;;)
    {
        // Event may be the reception of a packet - 
        // handle radio first, to give it highest priority:
        if (NRF_RADIO->EVENTS_END != 0)
        {
            NRF_RADIO->EVENTS_END = 0;
            NVIC_ClearPendingIRQ(RADIO_IRQn);

            if (m_state == STATE_RECEIVER_TEST)
            {
                NRF_RADIO->TASKS_RXEN = 1;
                if ((NRF_RADIO->CRCSTATUS == 1) && check_pdu())
                {
                    // Count the number of successfully received packets
                    m_rx_pkt_count++;
                }
                // Note that failing packets are simply ignored (CRC or contents error).

                // Zero fill all pdu fields to avoid stray data
                memset(&m_pdu, 0, DTM_PDU_MAX_MEMORY_SIZE);
            }
            // If no RECEIVER_TEST is running, ignore incoming packets (but do clear IRQ!)
        }

        // Check for timeouts:
        if (mp_timer->EVENTS_COMPARE[0] != 0)
        {
            mp_timer->EVENTS_COMPARE[0] = 0;
        }
        else if (mp_timer->EVENTS_COMPARE[1] != 0)
        {
            // Reset timeout event flag for next iteration.
            mp_timer->EVENTS_COMPARE[1] = 0;
            NVIC_ClearPendingIRQ(m_timer_irq);
            return ++m_current_time;
        }

        // Other events: No processing
    }
}


uint32_t dtm_cmd(dtm_cmd_t cmd, dtm_freq_t freq, uint32_t length, dtm_pkt_type_t payload)
{
    // Save specified packet in static variable for tx/rx functions to use.
    // Note that BLE conformance testers always use full length packets.
    m_packet_length = ((uint8_t)length & 0xFF);
    m_packet_type   = payload;
    m_phys_ch       = freq;

    // Clean out any non-retrieved event that might linger from an earlier test
    m_new_event     = true;
 
    // Set default event; any error will set it to LE_TEST_STATUS_EVENT_ERROR
    m_event         = LE_TEST_STATUS_EVENT_SUCCESS;
    
    if (m_state == STATE_UNINITIALIZED)
    {
        // Application has not explicitly initialized DTM, 
        return DTM_ERROR_UNINITIALIZED;
    }

    if (cmd == LE_RESET)
    {
        // Note that timer will continue running after a reset
        dtm_test_done();
        return DTM_SUCCESS;
    }

    if (cmd == LE_TEST_END)
    {
        if (m_state == STATE_IDLE)
        {
            // Sequencing error - only rx or tx test may be ended!
            m_event = LE_TEST_STATUS_EVENT_ERROR;
            return DTM_ERROR_INVALID_STATE;
        }
        m_event = LE_PACKET_REPORTING_EVENT | m_rx_pkt_count;
        dtm_test_done();
        return DTM_SUCCESS;
    }

    if (m_state != STATE_IDLE)
    {
        // Sequencing error - only TEST_END/RESET are legal while test is running
        // Note: State is unchanged; ongoing test not affected
        m_event = LE_TEST_STATUS_EVENT_ERROR;               
        return DTM_ERROR_INVALID_STATE;   
    }

    if (m_phys_ch > PHYS_CH_MAX)
    {
        // Parameter error
        // Note: State is unchanged; ongoing test not affected
        m_event = LE_TEST_STATUS_EVENT_ERROR;               
        return DTM_ERROR_ILLEGAL_CHANNEL;
    }

    m_rx_pkt_count = 0;

    if (cmd == LE_RECEIVER_TEST)
    {
        // Zero fill all pdu fields to avoid stray data from earlier test run
        memset(&m_pdu, 0, DTM_PDU_MAX_MEMORY_SIZE);
        radio_prepare(RX_MODE);                      // Reinitialize "everything"; RF interrupts OFF
        m_state = STATE_RECEIVER_TEST;
        return DTM_SUCCESS;
    }

    if (cmd == LE_TRANSMITTER_TEST)
    {
        if (m_packet_length > DTM_PAYLOAD_MAX_SIZE)
        {
            // Parameter error
            m_event = LE_TEST_STATUS_EVENT_ERROR;
            return DTM_ERROR_ILLEGAL_LENGTH;
        }

        // Note that PDU uses 4 bits even though BLE DTM uses only 2 (the HCI SDU uses all 4)
        m_pdu.content[DTM_HEADER_OFFSET] = ((uint8_t)m_packet_type & 0x0F); 
        m_pdu.content[DTM_LENGTH_OFFSET] = m_packet_length;
        
        switch (m_packet_type)
        {
            case DTM_PKT_PRBS9:
                // Non-repeated, must copy entire pattern to PDU
                memcpy(m_pdu.content + DTM_HEADER_SIZE, m_prbs_content, length);
                break;
                
            case DTM_PKT_0X0F:
                // Bit pattern 00001111 repeated
                memset(m_pdu.content + DTM_HEADER_SIZE, RFPHY_TEST_0X0F_REF_PATTERN, length);
                break;
                
            case DTM_PKT_0X55:
                // Bit pattern 01010101 repeated
                memset(m_pdu.content + DTM_HEADER_SIZE, RFPHY_TEST_0X55_REF_PATTERN, length);
                break;
                
            case DTM_PKT_VENDORSPECIFIC:
                // The length field is for indicating the vendor specific command to execute.
                // The frequency field is used for vendor specific options to the command.
                return dtm_vendor_specific_pkt(length, freq);
                
            default:
                // Parameter error 
                m_event = LE_TEST_STATUS_EVENT_ERROR;         
                return DTM_ERROR_ILLEGAL_CONFIGURATION;
        }

        // Initialize CRC value, set channel:
        radio_prepare(TX_MODE);
        // Configure PPI so that timer will activate radio every 625 us
        NRF_PPI->CH[0].EEP = (uint32_t)&mp_timer->EVENTS_COMPARE[0];
        NRF_PPI->CH[0].TEP = (uint32_t)&NRF_RADIO->TASKS_TXEN;
        NRF_PPI->CHENSET   = 0x01;
        m_state            = STATE_TRANSMITTER_TEST;
    }
    return DTM_SUCCESS;
}


bool dtm_event_get(dtm_event_t *p_dtm_event)
{
    bool was_new = m_new_event;
    // mark the current event as retrieved
    m_new_event  = false;
    *p_dtm_event = m_event;
    // return value indicates whether this value was already retrieved.
    return was_new;
}


// =================================================================================================
// Configuration functions (only for parameters not definitely determined by the BLE DTM standard).
// These functions return true if successful, false if value could not be set


/**@brief Function for configuring the output power for transmitter test.
          This function may be called directly, or through dtm_cmd() specifying
          DTM_PKT_VENDORSPECIFIC as payload, SET_TX_POWER as length, and the dBm value as frequency.
 */
bool dtm_set_txpower(uint32_t new_tx_power)
{
    // radio->TXPOWER register is 32 bits, low octet a signed value, upper 24 bits zeroed
    int8_t new_power8 = (int8_t)(new_tx_power & 0xFF);
    
    if (m_state > STATE_IDLE)
    {
        // radio must be idle to change the tx power
        return false;
    }

    if ((new_power8 > 4) || (new_power8 < -40))
    {
        // Parameter outside valid range: nRF radio is restricted to the range -40 dBm to +4 dBm
        return false;
    }

    if (new_tx_power & 0x03) 
    {
        // Parameter error: The nRF51 radio requires settings that are a multiple of 4.
        return false;
    }
    m_tx_power = new_tx_power;

    return true;
}


/**@brief Function for selecting a timer resource.
 *        This function may be called directly, or through dtm_cmd() specifying
 *        DTM_PKT_VENDORSPECIFIC as payload, SELECT_TIMER as length, and the timer as freq
 *
 * @param[in] new_timer     Timer id for the timer to use: 0, 1, or 2.
 *
 * @return true if the timer was successfully changed, false otherwise.
 */
bool dtm_set_timer(uint32_t new_timer)
{
    if (m_state > STATE_IDLE)
    {
        return false;
    }
    if (new_timer == 0)
    {
        mp_timer    = NRF_TIMER0;
        m_timer_irq = TIMER0_IRQn;
    }
    else if (new_timer == 1)
    {
        mp_timer    = NRF_TIMER1;
        m_timer_irq = TIMER1_IRQn;
    }
    else if (new_timer == 2)
    {
        mp_timer    = NRF_TIMER2;
        m_timer_irq = TIMER2_IRQn;
    }
    else
    {
        // Parameter error: Only TIMER 0, 1, 2 provided by nRF51   
        return false;
    }
    // New timer has been selected:
    return true;
}

/// @} 
