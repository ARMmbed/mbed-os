/*---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component   : Bluetooth Test
 * File        : cb_bt_test_man.h
 *
 * Description : Functionality for Bluetooth radio tests and qualification.
 *-------------------------------------------------------------------------*/

/**
* @file cb_bt_test_man.h
* @brief Functionality for Bluetooth radio tests and qualification.
 */

#ifndef _CB_BT_TEST_MAN_H_
#define _CB_BT_TEST_MAN_H_

#include "cb_comdefs.h"

#include "cb_bt_man.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * DEFINES
 *=========================================================================*/
#define cbBTM_OK             (0)
#define cbBTM_ERROR          (-1)

/*===========================================================================
 * TYPES
 *=========================================================================*/
typedef enum
{
    cbBTM_TEST_CONF_POS,
    cbBTM_TEST_CONF_NEG
} cbBTM_TestEvt;

typedef void (*cbBTM_TestCallback)(cbBTM_TestEvt evt);
typedef void (*cbBTM_LeTestEndCallback)(cbBTM_TestEvt evt, cb_uint16 nbrOfPackets);


/*===========================================================================
 * FUNCTIONS
 *=========================================================================*/

/**
 * Init Bluetooth test manager
 * @returns None 
 */
extern void cbBTM_init(void);

/**
 * Enable Device under test mode. Used for Bluetooth Classic radio tests.
 *
 * @param callback Test callback used to notify if the test was successfully started.
 * @returns cbBTM_OK is returned 
 */
extern cb_int32 cbBTM_enableDUT(cbBTM_TestCallback callback);

/**
 * This command operates the RF transceiver in continuous transmission mode (which is most likely used in
 * regulatory and standardization procedures and tests, such as FCC and ETSI certifications). Activating the
 * VS runs the TX START sequence code using the configured frequency, modulation, pattern, and power
 * level. The VS also enables the generation of a user-defined pattern (or correcting definitions without a
 * patch) by setting a new pattern generator (also known as a PN generator) init value and mask.
 * based on HCI_VS_DRPb_Tester_Con_TX, HCI Opcode 0xFDCA
 * The cbBTM_TestCallback is used to notify if the test is sucessfully started.
 * 
 * @param frequency Transmission frequency in MHz Range: 2402 - 2480
 * @param modulationScheme Range: 0x00 - 0x05 where
 *                         0x00 = CW
 *                         0x01 = BT BR (GFSK)
 *                         0x02 = BT EDR 2MB (p/4-DQPSK)
 *                         0x03 = BT EDR 3MB (8-DPSK)
 *                         0x04 = BT LE (BLE, GMSK)
 *                         0x05 = ANT (GFSK)
 * @param testPattern Range: 0x00 - 0x07 
 *                         0x00 = PN9
 *                         0x01 = PN15
 *                         0x02 = ZOZO (101010101010101010)
 *                         0x03 = All 1
 *                         0x04 = All 0
 *                         0x05 = FOFO (1111000011110000)
 *                         0x06 = FFOO (1111111100000000)
 *                         0x07 = Not used
 * @param powerLevelIndex Range: 0-7: 7 = Max Output Power, 0 = Min Output Power, 0x08 (PA off) 8 = PA Off (leakage)
 * @param reserved1 shall be set to 0
 * @param reserved2 shall be set to 0
 * @param callback Test callback used to notify if the test was successfully started.
 *
 * @return  cbBTM_OK is returned 
 */
extern cb_int32 cbBTM_tiDrpbTesterConTx(
    cb_uint16 frequency,
    cb_uint8 modulationScheme,
    cb_uint8 testPattern,
    cb_uint8 powerLevelIndex,
    cb_uint32 reserved1,
    cb_uint32 reserved2,
    cbBTM_TestCallback callback);

/**
 *This command operates the RF transceiver in continuous reception mode (most likely used in regulatory
 * and standardization procedures and tests, such as FCC and ETSI certifications). By activating the VS, the
 * RX START sequence code runs, using the configured frequency, RX mode, and modulation type.
 * based on HCI_VS_DRPb_Tester_Con_RX, HCI Opcode 0xFDCB
 *
 * The cbBTM_TestCallback is used to notify if the test is successfully started.
 * @param frequency Transmission frequency in MHz Range: 2402 - 2480
 * @param rxMode Range: 0 -3:
 *                         0x00 = Connection mode
 *                         0x01 = Best RF mode (ADPL closed loop) - For expert use only!
 *                         0x02 = Low current mode (ADPLL open loop) - For expert use only!
 *                         0x03 = Scan mode
 * @param modulationScheme Range: 0x03 - 0x05 where
 *                         0x03 = BT (BR, EDR 2MB, EDR 3MB)
 *                         0x04 = BT LE (BLE, GMSK)
 *                         0x05 = ANT (GFSK)
 * @param callback Test callback used to notify if the test was successfully started.
 *
 * @return  cbBTM_OK is returned 
 */
extern cb_int32 cbBTM_tiDrpbTesterConRx(
    cb_uint16 frequency,
    cb_uint8 rxMode,
    cb_uint8 modulationScheme,
    cbBTM_TestCallback callback);

/**
 * 
 * This command operates the RF transceiver in continuous reception mode (most likely used in regulatory
 * and standardization procedures and tests, such as FCC and ETSI certifications). Activating the VS runs
 * the RX START sequence code using the configured frequency, RX mode, and modulation type.
 * This command emulates Bluetooth connection mode. Connection does not require a setup procedure.
 * Based on HCI_VS_DRPb_Tester_Packet_TX_RX  HCI Opcode 0xFDCC
 * 
 * @param aclPacketType ACL TX packet type. Range: 0x00 - 0x0B
 *                         0x00 = DM1
 *                         0x01 = DH1
 *                         0x02 = DM3
 *                         0x03 = DH3
 *                         0x04 = DM5
 *                         0x05 = DH5
 *                         0x06 = 2-DH1
 *                         0x07 = 2-DH3
 *                         0x08 = 2-DH5
 *                         0x09 = 3-DH1
 *                         0x0A = 3-DH3
 *                         0x0B = 3-DH5
 * @param frequencyMode 0x00 = Hopping 0x03 = Single frequency
 * @param txSingleFrequency Transmission frequency in MHz,Range 2402 - 2480, 0xFFFF = no TX
 * @param rxSingleFrequency Transmission frequency in MHz,Range 2402 - 2480, 0xFFFF = no RX
 * @param aclDataPattern ACL TX packet data pattern Range: 0x00 - 0x05 
 *                         0x00 = All 0
 *                         0x01 = All 1
 *                         0x02 = ZOZO (101010101010101010)
 *                         0x03 = FOFO (1111000011110000)
 *                         0x04 = Ordered (1, 2, 3, 4, and so on)
 *                         0x05 = PRBS9 (pseudo-random bit sequence)
 * @param useExtendedFeatures Shall be SET to 0
 * @param aclDataLength ACL packet data length.
 *                         DM1: 0 - 17 ACL packet data length in bytes
 *                         DH1: 0 - 27
 *                         DM3: 0 - 121
 *                         DH3: 0 - 183
 *                         DM5: 0 - 224
 *                         DH5: 0 - 339
 *                         2-DH1: 0 - 54
 *                         2-DH3: 0 - 367
 *                         2-DH5: 0 - 679
 *                         3-DH1: 0 - 83
 *                         3-DH3: 0 - 552
 *                         3-DH5: 0 - 1021
 * @param powerLevel       Range 0-7: 7 = Max Output Power; 0 = Min Output; Power 8 = PA Off (leakage) 0x08 (PA off)
 * @param disableWhitening 0x00 = Enable whitening, 0x01 = Disable whitening
 * @param prbs9Init PRBS9 Init, range 0x0000 - 0x01FF
 * @param callback Test callback used to notify if the test was successfully started.
 *
 * @return cbBTM_OK is returned 
 */
extern cb_int32 cbBTM_tiDrpbTesterPacketTxRx(
    cb_uint8 aclPacketType,
    cb_uint8 frequencyMode,
    cb_uint16 txSingleFrequency,
    cb_uint16 rxSingleFrequency,
    cb_uint8 aclDataPattern,
    cb_uint8 useExtendedFeatures,
    cb_uint16 aclDataLength,
    cb_uint8 powerLevel,
    cb_uint8 disableWhitening,
    cb_uint16 prbs9Init,
    cbBTM_TestCallback callback);

/**
 * Enable Bluetooth Low Energy Transmitter test.
 * @param   txFreq          Transmit frequency. N = (F - 2402) / 2, Range: 0x00 to 0x27, Frequency Range : 2402 MHz to 2480 MHz,
                            Use oxFF to generate a pseudo random hopping frequency useful for some scenarios during type approval.
 * @param   length          Length in bytes of payload data in each packet
 * @param   packetPayload   0x00 Pseudo-Random bit sequence 9
 *                          0x01 Pattern of alternating bits 11110000
 *                          0x02 Pattern of alternating bits 10101010
 *                          0x03 Pseudo-Random bit sequence 15
 *                          0x04 Pattern of All 1 bits
 *                          0x05 Pattern of All 0 bits
 *                          0x06 Pattern of alternating bits 00001111
 *                          0x07 Pattern of alternating bits 0101
 * @param   callback Test callback used to notify if the test was successfully started.
 * @return  cbBTM_OK is returned 
 */
extern cb_int32 cbBTM_enableBleTransmitterTest(
    cb_uint8 txFreq, 
    cb_uint8 length,
    cb_uint8 packetPayload, 
    cbBTM_TestCallback callback);

/**
 * Enable Bluetooth Low Energy Receiver test.
 * @param   rxFreq      Receive frequency. N = (F - 2402) / 2, Range: 0x00 to 0x27, Frequency Range : 2402 MHz to 2480 MHz,
 * @param   callback    Test callback used to notify if the test was successfully started.
 * @returns cbBTM_OK is returned 
 */
extern cb_int32 cbBTM_enableBleReceiverTest(
    cb_uint8 rxFreq,
    cbBTM_TestCallback callback);

/**
 * End Bluetooth Low Energy Receiver or Transmitter test.
 * @param   callback    Test callback used to notify if the test was successfully ended.
 * @returns cbBTM_OK is returned 
 */
extern cb_int32 cbBTM_bleTestEnd(cbBTM_LeTestEndCallback callback);

#ifdef __cplusplus
}
#endif

#endif /* _CB_BT_TEST_MAN_H_ */

