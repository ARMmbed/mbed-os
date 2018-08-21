/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef CELLULAR_TARGETS_H_
#define CELLULAR_TARGETS_H_

namespace mbed {

#ifndef CELLULAR_DEVICE
#if defined(TARGET_ADV_WISE_1570) || defined(TARGET_MTB_ADV_WISE_1570)
#define CELLULAR_DEVICE QUECTEL_BC95
#elif TARGET_WIO_3G
#define CELLULAR_DEVICE QUECTEL_UG96
#elif TARGET_MTS_DRAGONFLY_F411RE
#define CELLULAR_DEVICE TELIT_HE910
#elif TARGET_MTB_MTS_DRAGONFLY
#define CELLULAR_DEVICE TELIT_HE910
#elif TARGET_UBLOX_C030
#ifdef TARGET_UBLOX_C030_N211
#define CELLULAR_DEVICE UBLOX_AT
#else
#define CELLULAR_DEVICE UBLOX_PPP
#endif
#elif TARGET_UBLOX_C027
#define CELLULAR_DEVICE UBLOX_PPP
#else
//#error Cellular target not defined, see cellular/targets.h
//#define CELLULAR_TARGET <target-modem>
//#define MDMTXD <pin-name>
//#define MDMRXD <pin-name>
#endif
#endif

} // namespace mbed
#endif // CELLULAR_TARGETS_H_
