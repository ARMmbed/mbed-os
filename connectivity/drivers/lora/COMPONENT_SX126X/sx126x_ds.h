/*!
 * \file      sx126x.h
 *
 * \brief     SX126x driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * Copyright (c) 2019, Arm Limited and affiliates.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBED_LORA_RADIO_DRV_SX126X_SX126X_DS_H_
#define MBED_LORA_RADIO_DRV_SX126X_SX126X_DS_H_

#include "LoRaRadio.h"
/*!
 * \brief Provides the frequency of the chip running on the radio and the frequency step
 *
 * \remark These defines are used for computing the frequency divider to set the RF frequency
 */
#define XTAL_FREQ                                   32000000
#define FREQ_DIV                                    33554432
#define FREQ_STEP                                   0.95367431640625 // ((double)(XTAL_FREQ / (double)FREQ_DIV))
#define FREQ_ERR                                    0.47683715820312


/*!
 * \brief List of devices supported by this driver
 */
#define SX1261  0
#define SX1262  1
#define SX1268  2

/*!
 * \brief List of matching supported by the sx126x
 */
#define MATCHING_FREQ_915                           0
#define MATCHING_FREQ_780                           1
#define MATCHING_FREQ_490                           2
#define MATCHING_FREQ_434                           3
#define MATCHING_FREQ_280                           4
#define MATCHING_FREQ_169                           5
#define MATCHING_FREQ_868                           6

/*!
 * \brief Compensation delay for SetAutoTx/Rx functions in 15.625 microseconds
 */
#define AUTO_RX_TX_OFFSET                           2

/*!
 * \brief LFSR initial value to compute IBM type CRC
 */
#define CRC_IBM_SEED                                0xFFFF

/*!
 * \brief LFSR initial value to compute CCIT type CRC
 */
#define CRC_CCITT_SEED                              0x1D0F

/*!
 * \brief Polynomial used to compute IBM CRC
 */
#define CRC_POLYNOMIAL_IBM                          0x8005

/*!
 * \brief Polynomial used to compute CCIT CRC
 */
#define CRC_POLYNOMIAL_CCITT                        0x1021

/*!
 * \brief The address of the register holding the first byte defining the CRC seed
 *
 */
#define REG_LR_CRCSEEDBASEADDR                      0x06BC

/*!
 * \brief The address of the register holding the first byte defining the CRC polynomial
 */
#define REG_LR_CRCPOLYBASEADDR                      0x06BE

/*!
 * \brief The address of the register holding the first byte defining the whitening seed
 */
#define REG_LR_WHITSEEDBASEADDR_MSB                 0x06B8
#define REG_LR_WHITSEEDBASEADDR_LSB                 0x06B9

/*!
 * \brief The address of the register holding the packet configuration
 */
#define REG_LR_PACKETPARAMS                         0x0704

/*!
 * \brief The address of the register holding the payload size
 */
#define REG_LR_PAYLOADLENGTH                        0x0702

/*!
 * \brief The addresses of the registers holding SyncWords values
 */
#define REG_LR_SYNCWORDBASEADDRESS                  0x06C0

/*!
 * \brief The addresses of the register holding LoRa Modem SyncWord value
 */
#define REG_LR_SYNCWORD                             0x0740

/*!
 * Syncword for Private LoRa networks
 */
#define LORA_MAC_PRIVATE_SYNCWORD                   0x1424

/*!
 * Syncword for Public LoRa networks
 */
#define LORA_MAC_PUBLIC_SYNCWORD                    0x3444

/*!
 * The address of the register giving a 4 bytes random number
 */
#define RANDOM_NUMBER_GENERATORBASEADDR             0x0819

/*!
 * The address of the register holding RX Gain value (0x94: power saving, 0x96: rx boosted)
 */
#define REG_RX_GAIN                                 0x08AC

/*!
 * The address of the register holding frequency error indication
 */
#define REG_FREQUENCY_ERRORBASEADDR                 0x076B

/*!
 * Change the value on the device internal trimming capacitor
 */
#define REG_XTA_TRIM                                0x0911

/*!
 * Set the current max value in the over current protection
 */
#define REG_OCP                                     0x08E7


/*!
 * \brief Represents the Rx internal counters values when GFSK or LoRa packet type is used
 */
typedef struct {
    radio_modems_t modem_type; //!< Packet to which the packet status are referring to.
    uint16_t packet_received;
    uint16_t crc_ok;
    uint16_t length_error;
} rx_counter_t;

/*!
 * \brief Represents a calibration configuration
 */
typedef union {
    struct {
        uint8_t rc64k_enable     : 1;                             //!< Calibrate RC64K clock
        uint8_t rc13m_enable     : 1;                             //!< Calibrate RC13M clock
        uint8_t pll_enable       : 1;                             //!< Calibrate PLL
        uint8_t adc_pulse_enable : 1;                             //!< Calibrate ADC Pulse
        uint8_t adc_bulkN_enable : 1;                             //!< Calibrate ADC bulkN
        uint8_t adc_bulkP_enable : 1;                             //!< Calibrate ADC bulkP
        uint8_t img_enable       : 1;
        uint8_t pad              : 1;
    } fields;

    uint8_t value;

} caliberation_params_t;

/*!
 * \brief Represents the possible radio system error states
 */
typedef union {
    struct {
        uint8_t rc64k_calib              : 1;                    //!< RC 64kHz oscillator calibration failed
        uint8_t rc13m_calib              : 1;                    //!< RC 13MHz oscillator calibration failed
        uint8_t pll_calib                : 1;                    //!< PLL calibration failed
        uint8_t adc_calib                : 1;                    //!< ADC calibration failed
        uint8_t img_calib                : 1;                    //!< Image calibration failed
        uint8_t xosc_start               : 1;                    //!< XOSC oscillator failed to start
        uint8_t pll_lock                 : 1;                    //!< PLL lock failed
        uint8_t buck_start               : 1;                    //!< Buck converter failed to start
        uint8_t pa_ramp                  : 1;                    //!< PA ramp failed
        uint8_t reserved                 : 7;                    //!< reserved
    } fields;

    uint16_t value;

} radio_error_t;

/*!
 * \brief Represents the operating mode the radio is actually running
 */
typedef enum {
    MODE_SLEEP                              = 0x00,         //! The radio is in sleep mode
    MODE_DEEP_SLEEP,                                        //! The radio is in deep-sleep mode
    MODE_STDBY_RC,                                          //! The radio is in standby mode with RC oscillator
    MODE_STDBY_XOSC,                                        //! The radio is in standby mode with XOSC oscillator
    MODE_FS,                                                //! The radio is in frequency synthesis mode
    MODE_TX,                                                //! The radio is in transmit mode
    MODE_RX,                                                //! The radio is in receive mode
    MODE_RX_DC,                                             //! The radio is in receive duty cycle mode
    MODE_CAD                                                //! The radio is in channel activity detection mode
} radio_operating_mode_t;

/*!
 * \brief Declares the oscillator in use while in standby mode
 *
 * Using the STDBY_RC standby mode allow to reduce the energy consumption
 * STDBY_XOSC should be used for time critical applications
 */
typedef enum {
    STDBY_RC                                = 0x00,
    STDBY_XOSC                              = 0x01,
} radio_standby_mode_t;

/*!
 * \brief Declares the power regulation used to power the device
 *
 * This command allows the user to specify if DC-DC or LDO is used for power regulation.
 * Using only LDO implies that the Rx or Tx current is doubled
 */
typedef enum {
    USE_LDO                                 = 0x00, // default
    USE_DCDC                                = 0x01,
} radio_regulator_mode_t;

/*!
 * \brief Represents the ramping time for power amplifier
 */
typedef enum {
    RADIO_RAMP_10_US                        = 0x00,
    RADIO_RAMP_20_US                        = 0x01,
    RADIO_RAMP_40_US                        = 0x02,
    RADIO_RAMP_80_US                        = 0x03,
    RADIO_RAMP_200_US                       = 0x04,
    RADIO_RAMP_800_US                       = 0x05,
    RADIO_RAMP_1700_US                      = 0x06,
    RADIO_RAMP_3400_US                      = 0x07,
} radio_ramp_time_t;

/*!
 * \brief Represents the number of symbols to be used for channel activity detection operation
 */
typedef enum {
    LORA_CAD_01_SYMBOL                      = 0x00,
    LORA_CAD_02_SYMBOL                      = 0x01,
    LORA_CAD_04_SYMBOL                      = 0x02,
    LORA_CAD_08_SYMBOL                      = 0x03,
    LORA_CAD_16_SYMBOL                      = 0x04,
} lora_cad_symbols_t;

/*!
 * \brief Represents the Channel Activity Detection actions after the CAD operation is finished
 */
typedef enum {
    LORA_CAD_ONLY                           = 0x00,
    LORA_CAD_RX                             = 0x01,
    LORA_CAD_LBT                            = 0x10,
} cad_exit_modes_t;

/*!
 * \brief Represents the modulation shaping parameter
 */
typedef enum {
    MOD_SHAPING_OFF                         = 0x00,
    MOD_SHAPING_G_BT_03                     = 0x08,
    MOD_SHAPING_G_BT_05                     = 0x09,
    MOD_SHAPING_G_BT_07                     = 0x0A,
    MOD_SHAPING_G_BT_1                      = 0x0B,
} radio_mod_shaping_t;

/*!
 * \brief Represents the modulation shaping parameter
 */
typedef enum {
    RX_BW_4800                              = 0x1F,
    RX_BW_5800                              = 0x17,
    RX_BW_7300                              = 0x0F,
    RX_BW_9700                              = 0x1E,
    RX_BW_11700                             = 0x16,
    RX_BW_14600                             = 0x0E,
    RX_BW_19500                             = 0x1D,
    RX_BW_23400                             = 0x15,
    RX_BW_29300                             = 0x0D,
    RX_BW_39000                             = 0x1C,
    RX_BW_46900                             = 0x14,
    RX_BW_58600                             = 0x0C,
    RX_BW_78200                             = 0x1B,
    RX_BW_93800                             = 0x13,
    RX_BW_117300                            = 0x0B,
    RX_BW_156200                            = 0x1A,
    RX_BW_187200                            = 0x12,
    RX_BW_234300                            = 0x0A,
    RX_BW_312000                            = 0x19,
    RX_BW_373600                            = 0x11,
    RX_BW_467000                            = 0x09,
} radio_rx_bandwidth_t;

/*!
 * \brief Represents the possible spreading factor values in LoRa packet types
 */
typedef enum {
    LORA_SF5                                = 0x05,
    LORA_SF6                                = 0x06,
    LORA_SF7                                = 0x07,
    LORA_SF8                                = 0x08,
    LORA_SF9                                = 0x09,
    LORA_SF10                               = 0x0A,
    LORA_SF11                               = 0x0B,
    LORA_SF12                               = 0x0C,
} lora_spread_factors_t;

/*!
 * \brief Represents the bandwidth values for LoRa packet type
 */
typedef enum {
    LORA_BW_500                             = 6,
    LORA_BW_250                             = 5,
    LORA_BW_125                             = 4,
    LORA_BW_062                             = 3,
    LORA_BW_041                             = 10,
    LORA_BW_031                             = 2,
    LORA_BW_020                             = 9,
    LORA_BW_015                             = 1,
    LORA_BW_010                             = 8,
    LORA_BW_007                             = 0,
} lora_bandwidths_t;

const uint8_t lora_bandwidhts [] = {LORA_BW_125, LORA_BW_250, LORA_BW_500};

/*!
 * \brief Represents the coding rate values for LoRa packet type
 */
typedef enum {
    LORA_CR_4_5                             = 0x01,
    LORA_CR_4_6                             = 0x02,
    LORA_CR_4_7                             = 0x03,
    LORA_CR_4_8                             = 0x04,
} lora_coding_tates_t;

/*!
 * \brief Represents the preamble length used to detect the packet on Rx side
 */
typedef enum {
    RADIO_PREAMBLE_DETECTOR_OFF             = 0x00,         //!< Preamble detection length off
    RADIO_PREAMBLE_DETECTOR_08_BITS         = 0x04,         //!< Preamble detection length 8 bits
    RADIO_PREAMBLE_DETECTOR_16_BITS         = 0x05,         //!< Preamble detection length 16 bits
    RADIO_PREAMBLE_DETECTOR_24_BITS         = 0x06,         //!< Preamble detection length 24 bits
    RADIO_PREAMBLE_DETECTOR_32_BITS         = 0x07,         //!< Preamble detection length 32 bit
} radio_preamble_detection_t;

/*!
 * \brief Represents the possible combinations of SyncWord correlators activated
 */
typedef enum {
    RADIO_ADDRESSCOMP_FILT_OFF              = 0x00,         //!< No correlator turned on, i.e. do not search for SyncWord
    RADIO_ADDRESSCOMP_FILT_NODE             = 0x01,
    RADIO_ADDRESSCOMP_FILT_NODE_BROAD       = 0x02,
} radio_address_filter_t;

/*!
 *  \brief Radio packet length mode
 */
typedef enum {
    RADIO_PACKET_VARIABLE_LENGTH            = 0x00,         //!< The packet is on variable size, header included
    RADIO_PACKET_FIXED_LENGTH               = 0x01,         //!< The packet is known on both sides, no header included in the packet
} radio_pkt_length_t;

/*!
 * \brief Represents the CRC length
 */
typedef enum radio_crc_types_e {
    RADIO_CRC_OFF                           = 0x01,         //!< No CRC in use
    RADIO_CRC_1_BYTES                       = 0x00,
    RADIO_CRC_2_BYTES                       = 0x02,
    RADIO_CRC_1_BYTES_INV                   = 0x04,
    RADIO_CRC_2_BYTES_INV                   = 0x06,
    RADIO_CRC_2_BYTES_IBM                   = 0xF1,
    RADIO_CRC_2_BYTES_CCIT                  = 0xF2,
} radio_crc_types_t;

/*!
 * \brief Radio whitening mode activated or deactivated
 */
typedef enum {
    RADIO_DC_FREE_OFF                       = 0x00,
    RADIO_DC_FREEWHITENING                  = 0x01,
} radio_whitening_mode_t;

/*!
 * \brief Holds the lengths mode of a LoRa packet type
 */
typedef enum {
    LORA_PACKET_VARIABLE_LENGTH             = 0x00,         //!< The packet is on variable size, header included
    LORA_PACKET_FIXED_LENGTH                = 0x01,         //!< The packet is known on both sides, no header included in the packet
    LORA_PACKET_EXPLICIT                    = LORA_PACKET_VARIABLE_LENGTH,
    LORA_PACKET_IMPLICIT                    = LORA_PACKET_FIXED_LENGTH,
} lora_pkt_length_t;

/*!
 * \brief Represents the CRC mode for LoRa packet type
 */
typedef enum {
    LORA_CRC_ON                             = 0x01,         //!< CRC activated
    LORA_CRC_OFF                            = 0x00,         //!< CRC not used
} lora_crc_mode_t;

/*!
 * \brief Represents the IQ mode for LoRa packet type
 */
typedef enum {
    LORA_IQ_NORMAL                          = 0x00,
    LORA_IQ_INVERTED                        = 0x01,
} lora_IQ_mode_t;

/*!
 * \brief Represents the volatge used to control the TCXO on/off from DIO3
 */
typedef enum {
    TCXO_CTRL_1_6V                          = 0x00,
    TCXO_CTRL_1_7V                          = 0x01,
    TCXO_CTRL_1_8V                          = 0x02,
    TCXO_CTRL_2_2V                          = 0x03,
    TCXO_CTRL_2_4V                          = 0x04,
    TCXO_CTRL_2_7V                          = 0x05,
    TCXO_CTRL_3_0V                          = 0x06,
    TCXO_CTRL_3_3V                          = 0x07,
} radio_TCXO_ctrl_voltage_t;

/*!
 * \brief Represents the interruption masks available for the radio
 *
 * \remark Note that not all these interruptions are available for all packet types
 */
typedef enum {
    IRQ_RADIO_NONE                          = 0x0000,
    IRQ_TX_DONE                             = 0x0001,
    IRQ_RX_DONE                             = 0x0002,
    IRQ_PREAMBLE_DETECTED                   = 0x0004,
    IRQ_SYNCWORD_VALID                      = 0x0008,
    IRQ_HEADER_VALID                        = 0x0010,
    IRQ_HEADER_ERROR                        = 0x0020,
    IRQ_CRC_ERROR                           = 0x0040,
    IRQ_CAD_DONE                            = 0x0080,
    IRQ_CAD_ACTIVITY_DETECTED               = 0x0100,
    IRQ_RX_TX_TIMEOUT                       = 0x0200,
    IRQ_RADIO_ALL                           = 0xFFFF,
} radio_irq_masks_t;

typedef enum {
    RADIO_GET_STATUS                        = 0xC0,
    RADIO_WRITE_REGISTER                    = 0x0D,
    RADIO_READ_REGISTER                     = 0x1D,
    RADIO_WRITE_BUFFER                      = 0x0E,
    RADIO_READ_BUFFER                       = 0x1E,
    RADIO_SET_SLEEP                         = 0x84,
    RADIO_SET_STANDBY                       = 0x80,
    RADIO_SET_FS                            = 0xC1,
    RADIO_SET_TX                            = 0x83,
    RADIO_SET_RX                            = 0x82,
    RADIO_SET_RXDUTYCYCLE                   = 0x94,
    RADIO_SET_CAD                           = 0xC5,
    RADIO_SET_TXCONTINUOUSWAVE              = 0xD1,
    RADIO_SET_TXCONTINUOUSPREAMBLE          = 0xD2,
    RADIO_SET_PACKETTYPE                    = 0x8A,
    RADIO_GET_PACKETTYPE                    = 0x11,
    RADIO_SET_RFFREQUENCY                   = 0x86,
    RADIO_SET_TXPARAMS                      = 0x8E,
    RADIO_SET_PACONFIG                      = 0x95,
    RADIO_SET_CADPARAMS                     = 0x88,
    RADIO_SET_BUFFERBASEADDRESS             = 0x8F,
    RADIO_SET_MODULATIONPARAMS              = 0x8B,
    RADIO_SET_PACKETPARAMS                  = 0x8C,
    RADIO_GET_RXBUFFERSTATUS                = 0x13,
    RADIO_GET_PACKETSTATUS                  = 0x14,
    RADIO_GET_RSSIINST                      = 0x15,
    RADIO_GET_STATS                         = 0x10,
    RADIO_RESET_STATS                       = 0x00,
    RADIO_CFG_DIOIRQ                        = 0x08,
    RADIO_GET_IRQSTATUS                     = 0x12,
    RADIO_CLR_IRQSTATUS                     = 0x02,
    RADIO_CALIBRATE                         = 0x89,
    RADIO_CALIBRATEIMAGE                    = 0x98,
    RADIO_SET_REGULATORMODE                 = 0x96,
    RADIO_GET_ERROR                         = 0x17,
    RADIO_CLR_ERROR                         = 0x07,
    RADIO_SET_TCXOMODE                      = 0x97,
    RADIO_SET_TXFALLBACKMODE                = 0x93,
    RADIO_SET_RFSWITCHMODE                  = 0x9D,
    RADIO_SET_STOPRXTIMERONPREAMBLE         = 0x9F,
    RADIO_SET_LORASYMBTIMEOUT               = 0xA0,
} opmode_commands_t;

/*!
 * \brief Structure describing the radio status
 */
typedef union {
    uint8_t value;
    struct {
        //bit order is lsb -> msb
        uint8_t reserved  : 1;  //!< Reserved
        uint8_t cmd_status : 3;  //!< Command status
        uint8_t chip_mode  : 3;  //!< Chip mode
        uint8_t cpu_busy   : 1;  //!< Flag for CPU radio busy
    } fields;
} radio_status_t;

/*!
 * \brief Structure describing the error codes for callback functions
 */
typedef enum {
    IRQ_HEADER_ERROR_CODE                   = 0x01,
    IRQ_SYNCWORD_ERROR_CODE                 = 0x02,
    IRQ_CRC_ERROR_CODE                      = 0x04,
} irq_error_t;


typedef enum {
    IRQ_PBL_DETECT_CODE                     = 0x01,
    IRQ_SYNCWORD_VALID_CODE                 = 0x02,
    IRQ_HEADER_VALID_CODE                   = 0x04,
} irq_valid_codes_t;

typedef enum {
    IRQ_RX_TIMEOUT                          = 0x00,
    IRQ_TX_TIMEOUT                          = 0x01,
} irq_timeout_t;

typedef enum {
    RECEPTION_MODE_SINGLE = 0,
    RECEPTION_MODE_CONTINUOUS,
    RECEPTION_MODE_OTHER
} reception_mode_t;

/*!
 * \brief The type describing the modulation parameters for every packet types
 */
typedef struct {
    radio_modems_t modem_type; //!< Packet to which the modulation parameters are referring to.
    struct {
        struct {
            uint32_t bit_rate;
            uint32_t fdev;
            radio_mod_shaping_t modulation_shaping;
            uint8_t bandwidth;
            uint32_t operational_frequency;
        } gfsk;

        struct {
            lora_spread_factors_t spreading_factor; //!< Spreading Factor for the LoRa modulation
            lora_bandwidths_t bandwidth;  //!< Bandwidth for the LoRa modulation
            lora_coding_tates_t coding_rate; //!< Coding rate for the LoRa modulation
            uint8_t low_datarate_optimization; //!< Indicates if the modem uses the low datarate optimization
            uint32_t operational_frequency;
        } lora;
    } params;                     //!< Holds the modulation parameters structure
} modulation_params_t;

/*!
 * \brief The type describing the packet parameters for every packet types
 */
typedef struct packet_params {
    radio_modems_t modem_type; //!< Packet to which the packet parameters are referring to.
    struct {
        /*!
         * \brief Holds the GFSK packet parameters
         */
        struct {
            uint16_t preamble_length; //!< The preamble Tx length for GFSK packet type in bit
            radio_preamble_detection_t preamble_min_detect; //!< The preamble Rx length minimal for GFSK packet type
            uint8_t syncword_length; //!< The synchronization word length for GFSK packet type
            radio_address_filter_t addr_comp; //!< Activated SyncWord correlators
            radio_pkt_length_t header_type; //!< If the header is explicit, it will be transmitted in the GFSK packet. If the header is implicit, it will not be transmitted
            uint8_t payload_length;   //!< Size of the payload in the GFSK packet
            radio_crc_types_t crc_length; //!< Size of the CRC block in the GFSK packet
            radio_whitening_mode_t whitening_mode;
        } gfsk;
        /*!
         * \brief Holds the LoRa packet parameters
         */
        struct {
            uint16_t preamble_length; //!< The preamble length is the number of LoRa symbols in the preamble
            lora_pkt_length_t header_type; //!< If the header is explicit, it will be transmitted in the LoRa packet. If the header is implicit, it will not be transmitted
            uint8_t payload_length;   //!< Size of the payload in the LoRa packet
            lora_crc_mode_t crc_mode;       //!< Size of CRC block in LoRa packet
            lora_IQ_mode_t invert_IQ;      //!< Allows to swap IQ for LoRa packet
        } lora;
    } params;                         //!< Holds the packet parameters structure
} packet_params_t;

/*!
 * \brief Represents the packet status for every packet type
 */
typedef struct {
    radio_modems_t modem_type;      //!< Packet to which the packet status are referring to.
    struct {
        struct {
            uint8_t rx_status;
            int8_t rssi_avg;                                //!< The averaged RSSI
            int8_t rssi_sync;                               //!< The RSSI measured on last packet
            uint32_t freq_error;
        } gfsk;
        struct {
            int8_t rssi_pkt;                                //!< The RSSI of the last packet
            int8_t snr_pkt;                                 //!< The SNR of the last packet
            int8_t signal_rssi_pkt;
            uint32_t freq_error;
        } lora;
    } params;
} packet_status_t;


#endif /* MBED_LORA_RADIO_DRV_SX126X_SX126X_DS_H_ */
