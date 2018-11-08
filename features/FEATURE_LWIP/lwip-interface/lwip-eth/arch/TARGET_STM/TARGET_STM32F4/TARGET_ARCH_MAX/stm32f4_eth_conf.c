/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "stm32f4xx_hal.h"

void _eth_config_mac(ETH_HandleTypeDef *heth)
{
    ETH_MACInitTypeDef macconf = {
        .Watchdog = ETH_WATCHDOG_ENABLE,
        .Jabber = ETH_JABBER_ENABLE,
        .InterFrameGap = ETH_INTERFRAMEGAP_96BIT,
        .CarrierSense = ETH_CARRIERSENCE_ENABLE,
        .ReceiveOwn = ETH_RECEIVEOWN_ENABLE,
        .LoopbackMode = ETH_LOOPBACKMODE_DISABLE,
        .ChecksumOffload = ETH_CHECKSUMOFFLAOD_ENABLE,
        .RetryTransmission = ETH_RETRYTRANSMISSION_DISABLE,
        .AutomaticPadCRCStrip = ETH_AUTOMATICPADCRCSTRIP_DISABLE,
        .BackOffLimit = ETH_BACKOFFLIMIT_10,
        .DeferralCheck = ETH_DEFFERRALCHECK_DISABLE,
        .ReceiveAll = ETH_RECEIVEAll_DISABLE,
        .SourceAddrFilter = ETH_SOURCEADDRFILTER_DISABLE,
        .PassControlFrames = ETH_PASSCONTROLFRAMES_BLOCKALL,
        .BroadcastFramesReception = ETH_BROADCASTFRAMESRECEPTION_ENABLE,
        .DestinationAddrFilter = ETH_DESTINATIONADDRFILTER_NORMAL,
        .PromiscuousMode = ETH_PROMISCUOUS_MODE_DISABLE,
        .MulticastFramesFilter = ETH_MULTICASTFRAMESFILTER_NONE, // Disable multicast filter
        .UnicastFramesFilter = ETH_UNICASTFRAMESFILTER_PERFECT,
        .HashTableHigh = 0x0U,
        .HashTableLow = 0x0U,
        .PauseTime = 0x0U,
        .ZeroQuantaPause = ETH_ZEROQUANTAPAUSE_DISABLE,
        .PauseLowThreshold = ETH_PAUSELOWTHRESHOLD_MINUS4,
        .UnicastPauseFrameDetect = ETH_UNICASTPAUSEFRAMEDETECT_DISABLE,
        .ReceiveFlowControl = ETH_RECEIVEFLOWCONTROL_DISABLE,
        .TransmitFlowControl = ETH_TRANSMITFLOWCONTROL_DISABLE,
        .VLANTagComparison = ETH_VLANTAGCOMPARISON_16BIT,
        .VLANTagIdentifier = 0x0U,
    };

    if (heth->Init.ChecksumMode == ETH_CHECKSUM_BY_HARDWARE) {
        macconf.ChecksumOffload = ETH_CHECKSUMOFFLAOD_ENABLE;
    } else {
        macconf.ChecksumOffload = ETH_CHECKSUMOFFLAOD_DISABLE;
    }

    (void) HAL_ETH_ConfigMAC(heth, &macconf);
}
