/* 
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 * 
 *   4. This software must only be used in a processor manufactured by Nordic
 *   Semiconductor ASA, or in a processor manufactured by a third party that
 *   is used in combination with a processor manufactured by Nordic Semiconductor.
 * 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/**
  @addtogroup BLE_COMMON
  @{
  @defgroup ble_ranges Module specific SVC, event and option number subranges
  @{

  @brief Definition of SVC, event and option number subranges for each API module.

  @note
  SVCs, event and option numbers are split into subranges for each API module.
  Each module receives its entire allocated range of SVC calls, whether implemented or not,
  but return BLE_ERROR_NOT_SUPPORTED for unimplemented or undefined calls in its range.

  Note that the symbols BLE_<module>_SVC_LAST is the end of the allocated SVC range,
  rather than the last SVC function call actually defined and implemented.

  Specific SVC, event and option values are defined in each module's ble_<module>.h file,
  which defines names of each individual SVC code based on the range start value.
*/

#ifndef BLE_RANGES_H__
#define BLE_RANGES_H__

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_SVC_BASE           0x60       /**< Common BLE SVC base. */
#define BLE_SVC_LAST           0x6B       /**< Total: 12. */

#define BLE_RESERVED_SVC_BASE  0x6C       /**< Reserved BLE SVC base. */
#define BLE_RESERVED_SVC_LAST  0x6F       /**< Total: 4. */

#define BLE_GAP_SVC_BASE       0x70       /**< GAP BLE SVC base. */
#define BLE_GAP_SVC_LAST       0x8F       /**< Total: 32. */

#define BLE_GATTC_SVC_BASE     0x90       /**< GATTC BLE SVC base. */
#define BLE_GATTC_SVC_LAST     0x9F       /**< Total: 32. */

#define BLE_GATTS_SVC_BASE     0xA0       /**< GATTS BLE SVC base. */
#define BLE_GATTS_SVC_LAST     0xAF       /**< Total: 16. */

#define BLE_L2CAP_SVC_BASE     0xB0       /**< L2CAP BLE SVC base. */
#define BLE_L2CAP_SVC_LAST     0xBF       /**< Total: 16. */


#define BLE_EVT_INVALID        0x00       /**< Invalid BLE Event. */

#define BLE_EVT_BASE           0x01       /**< Common BLE Event base. */
#define BLE_EVT_LAST           0x0F       /**< Total: 15. */

#define BLE_GAP_EVT_BASE       0x10       /**< GAP BLE Event base. */
#define BLE_GAP_EVT_LAST       0x2F       /**< Total: 32. */

#define BLE_GATTC_EVT_BASE     0x30       /**< GATTC BLE Event base. */
#define BLE_GATTC_EVT_LAST     0x4F       /**< Total: 32. */

#define BLE_GATTS_EVT_BASE     0x50       /**< GATTS BLE Event base. */
#define BLE_GATTS_EVT_LAST     0x6F       /**< Total: 32. */

#define BLE_L2CAP_EVT_BASE     0x70       /**< L2CAP BLE Event base. */
#define BLE_L2CAP_EVT_LAST     0x8F       /**< Total: 32.  */


#define BLE_OPT_INVALID        0x00       /**< Invalid BLE Option. */

#define BLE_OPT_BASE           0x01       /**< Common BLE Option base. */
#define BLE_OPT_LAST           0x1F       /**< Total: 31. */

#define BLE_GAP_OPT_BASE       0x20       /**< GAP BLE Option base. */
#define BLE_GAP_OPT_LAST       0x3F       /**< Total: 32. */

#define BLE_GATTC_OPT_BASE     0x40       /**< GATTC BLE Option base. */
#define BLE_GATTC_OPT_LAST     0x5F       /**< Total: 32. */

#define BLE_GATTS_OPT_BASE     0x60       /**< GATTS BLE Option base. */
#define BLE_GATTS_OPT_LAST     0x7F       /**< Total: 32. */

#define BLE_L2CAP_OPT_BASE     0x80       /**< L2CAP BLE Option base. */
#define BLE_L2CAP_OPT_LAST     0x9F       /**< Total: 32.  */

#ifdef __cplusplus
}
#endif
#endif /* BLE_RANGES_H__ */

/**
  @}
  @}
*/
