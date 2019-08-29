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
 * Component   : WIFI
 * File        : enterprize_handle.h
 *
 * Description : 
 *-------------------------------------------------------------------------*/
#include <string.h>
#include "cb_comdefs.h"
#include <string.h>
#include <stdlib.h>
#include "cb_cert_utils.h"
#include "cb_wlan.h"
#include "cb_hw.h"

/*===========================================================================
 * DEFINES
 *=========================================================================*/
#define cbSECMNG_MAX_CERTIFICATE_SIZE               4096
#define cbSECMNG_MAX_DER_FILE_SIZE                  (4096 + 3)      // we need to make room for the 3 size bytes
#define cbSECMNG_MD5_LEN                            16
#define cbSECMNG_MD5_STRING_LEN                     ((cbSECMNG_MD5_LEN * 2) + 1)
#define cbSECMNG_MAX_CERTIFICATE_NAME_LEN           32
#define cbSECMNG_MAX_CERTIFICATE_PASSWORD_LEN       64

/*===========================================================================
 * TYPES
 *=========================================================================*/
typedef enum {
    cbSECMNG_TYPE_ALL = -1,
    cbSECMNG_TRUSTED_ROOT,
    cbSECMNG_CLIENT_CERT,
    cbSECMNG_CLIENT_PRIVATE_KEY,
} cbSECMNG_Type;

/*===========================================================================
 * DECLARATIONS
 *=========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/** Call eap connection handler
 *
 *  @param cert                 Reference to certificate e.g client or CA server certificate
 *  @param pvt_key              Reference to private key in case if private key is inside cert reference it can be passed as NULL. 
                                In case of EAP_TLS we need private key along with client certificate and not require in case of PEAP so NULL can be passed.
 *  @param commonParams         Connection parameters.
 *  @param enterpriseParams     Enterprise parameters.
 *  @return                     0 on success, or error code on failure
 */
cbWLAN_Handle cb_eap_conn_handler(cb_char const* cert, cb_char const* pvt_key, cbWLAN_CommonConnectParameters *commonParams, cbWLAN_EnterpriseConnectParameters *enterpriseParams);

#ifdef __cplusplus
}
#endif
