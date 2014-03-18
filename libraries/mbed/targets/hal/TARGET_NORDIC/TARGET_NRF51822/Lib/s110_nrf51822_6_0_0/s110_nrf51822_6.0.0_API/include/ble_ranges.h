/*
  Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.

  The information contained herein is confidential property of Nordic Semiconductor. The use,
  copying, transfer or disclosure of such information is prohibited except by express written
  agreement with Nordic Semiconductor.
 */
/**
  @addtogroup BLE_COMMON
  @{
  @defgroup ble_ranges Module specific SVC and event number subranges
  @{

  @brief Definition of SVC and event number subranges for each API module.

  @note
  SVCs and event numbers are split into subranges for each API module.
  Each module receives its entire allocated range of SVC calls, whether implemented or not,
  but return BLE_ERROR_NOT_SUPPORTED for unimplemented or undefined calls in its range.

  Note that the symbols BLE_<module>_SVC_LAST is the end of the allocated SVC range,
  rather than the last SVC function call actually defined and implemented.

  Specific SVC and event values are defined in each module's ble_<module>.h file,
  which defines names of each individual SVC code based on the range start value.
*/

#ifndef BLE_RANGES_H__
#define BLE_RANGES_H__

#define BLE_SVC_BASE           0x60
#define BLE_SVC_LAST           0x6B       /* Total: 12. */

#define BLE_RESERVED_SVC_BASE  0x6C
#define BLE_RESERVED_SVC_LAST  0x6F       /* Total: 4. */

#define BLE_GAP_SVC_BASE       0x70
#define BLE_GAP_SVC_LAST       0x8F       /* Total: 32. */

#define BLE_GATTC_SVC_BASE     0x90
#define BLE_GATTC_SVC_LAST     0x9F       /* Total: 16. */

#define BLE_GATTS_SVC_BASE     0xA0
#define BLE_GATTS_SVC_LAST     0xAF       /* Total: 16. */

#define BLE_L2CAP_SVC_BASE     0xB0
#define BLE_L2CAP_SVC_LAST     0xBF       /* Total: 16. */


#define BLE_EVT_INVALID        0x00

#define BLE_EVT_BASE           0x01
#define BLE_EVT_LAST           0x0F       /* Total: 15. */

#define BLE_GAP_EVT_BASE       0x10
#define BLE_GAP_EVT_LAST       0x2F       /* Total: 32. */

#define BLE_GATTC_EVT_BASE     0x30
#define BLE_GATTC_EVT_LAST     0x4F       /* Total: 32. */

#define BLE_GATTS_EVT_BASE     0x50
#define BLE_GATTS_EVT_LAST     0x6F       /* Total: 32. */

#define BLE_L2CAP_EVT_BASE     0x70
#define BLE_L2CAP_EVT_LAST     0x8F       /* Total: 32.  */

#endif /* BLE_RANGES_H__ */

/**
  @}
  @}
*/
