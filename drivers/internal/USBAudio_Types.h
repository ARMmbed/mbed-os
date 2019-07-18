/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef USBAUDIO_TYPES_H
#define USBAUDIO_TYPES_H


#define DEFAULT_CONFIGURATION (1)

// Audio Request Codes
#define REQUEST_SET_CUR     0x01
#define REQUEST_GET_CUR     0x81
#define REQUEST_SET_MIN     0x02
#define REQUEST_GET_MIN     0x82
#define REQUEST_SET_MAX     0x03
#define REQUEST_GET_MAX     0x83
#define REQUEST_SET_RES     0x04
#define REQUEST_GET_RES     0x84

#define MUTE_CONTROL        0x01
#define VOLUME_CONTROL      0x02


// Audio Descriptor Sizes
#define CONTROL_INTERFACE_DESCRIPTOR_LENGTH       0x09
#define STREAMING_INTERFACE_DESCRIPTOR_LENGTH     0x07
#define INPUT_TERMINAL_DESCRIPTOR_LENGTH          0x0C
#define OUTPUT_TERMINAL_DESCRIPTOR_LENGTH         0x09
#define FEATURE_UNIT_DESCRIPTOR_LENGTH            0x09
#define STREAMING_ENDPOINT_DESCRIPTOR_LENGTH      0x07

// Audio Format Type Descriptor Sizes
#define FORMAT_TYPE_I_DESCRIPTOR_LENGTH   0x0b

#define AUDIO_CLASS                       0x01
#define SUBCLASS_AUDIOCONTROL             0x01
#define SUBCLASS_AUDIOSTREAMING           0x02

// Audio Descriptor Types
#define INTERFACE_DESCRIPTOR_TYPE         0x24
#define ENDPOINT_DESCRIPTOR_TYPE          0x25

// Audio Control Interface Descriptor Subtypes
#define CONTROL_HEADER                    0x01
#define CONTROL_INPUT_TERMINAL            0x02
#define CONTROL_OUTPUT_TERMINAL           0x03
#define CONTROL_FEATURE_UNIT              0x06

// USB Terminal Types
#define TERMINAL_USB_STREAMING            0x0101

// Predefined Audio Channel Configuration Bits
// Mono
#define CHANNEL_M                         0x0000
#define CHANNEL_L                         0x0001  /* Left Front */
#define CHANNEL_R                         0x0002  /* Right Front */

// Feature Unit Control Bits
#define CONTROL_MUTE                      0x0001
#define CONTROL_VOLUME                    0x0002

// Input Terminal Types
#define TERMINAL_MICROPHONE               0x0201

// Output Terminal Types
#define TERMINAL_SPEAKER                  0x0301
#define TERMINAL_HEADPHONES               0x0302

// Audio Streaming Interface Descriptor Subtypes
#define STREAMING_GENERAL                 0x01
#define STREAMING_FORMAT_TYPE             0x02

// Audio Data Format Type I Codes
#define FORMAT_PCM                        0x0001

// Audio Format Types
#define FORMAT_TYPE_I                     0x01

// Audio Endpoint Descriptor Subtypes
#define ENDPOINT_GENERAL                  0x01

#endif
