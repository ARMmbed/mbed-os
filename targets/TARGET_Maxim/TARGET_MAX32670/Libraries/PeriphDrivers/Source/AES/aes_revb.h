/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */

#include <stdint.h>
#include "mxc_aes.h"
#include "aes_revb_regs.h"
#include "aes_key_revb_regs.h"
#include "trng_revb_regs.h"

/**
  * @brief  Enumeration type to select AES key
  *
  */
typedef enum {
    MXC_AES_REVB_128BITS    = MXC_S_AES_REVB_CTRL_KEY_SIZE_AES128,    ///< Select AES-128 bit key
    MXC_AES_REVB_192BITS    = MXC_S_AES_REVB_CTRL_KEY_SIZE_AES192,    ///< Select AES-192 bit key
    MXC_AES_REVB_256BITS    = MXC_S_AES_REVB_CTRL_KEY_SIZE_AES256,    ///< Select AES-256 bit key
} mxc_aes_revb_keys_t;

/**
  * @brief  Enumeration type to select AES key source and encryption type
  *
  */
typedef enum {
    MXC_AES_REVB_ENCRYPT_EXT_KEY  = 0,   ///< Encryption using External key
    MXC_AES_REVB_DECRYPT_EXT_KEY  = 1,   ///< Encryption using internal key
    MXC_AES_REVB_DECRYPT_INT_KEY  = 2    ///< Decryption using internal key
} mxc_aes_revb_enc_type_t;

/**
  * @brief  Structure used to set up AES request
  *
  */
typedef struct _mxc_aes_revb_cipher_req_t {
    uint32_t length;                ///< Length of the data
    uint32_t *inputData;            ///< Pointer to input data
    uint32_t *resultData;           ///< Pointer to encrypted data
    mxc_aes_revb_keys_t keySize;         ///< Size of AES key
    mxc_aes_revb_enc_type_t encryption;  ///< Encrytion type or \ref mxc_aes_enc_type_t
    mxc_aes_complete_t callback;    ///< Callback function
} mxc_aes_revb_req_t;


int MXC_AES_RevB_Init (mxc_aes_revb_regs_t* aes);
void MXC_AES_RevB_EnableInt (mxc_aes_revb_regs_t* aes, uint32_t interrupt);
void MXC_AES_RevB_DisableInt (mxc_aes_revb_regs_t* aes, uint32_t interrupt);
int MXC_AES_RevB_IsBusy (mxc_aes_revb_regs_t* aes);
int MXC_AES_RevB_Shutdown (mxc_aes_revb_regs_t* aes);
void MXC_AES_RevB_GenerateKey(mxc_trng_revb_regs_t* trng);
void MXC_AES_RevB_SetKeySize(mxc_aes_revb_regs_t* aes, mxc_aes_revb_keys_t key);
mxc_aes_keys_t MXC_AES_RevB_GetKeySize(mxc_aes_revb_regs_t* aes);
void MXC_AES_RevB_FlushInputFIFO(mxc_aes_revb_regs_t* aes);
void MXC_AES_RevB_FlushOutputFIFO(mxc_aes_revb_regs_t* aes);
void MXC_AES_RevB_Start(mxc_aes_revb_regs_t* aes);
uint32_t MXC_AES_RevB_GetFlags(mxc_aes_revb_regs_t* aes);
void MXC_AES_RevB_ClearFlags(mxc_aes_revb_regs_t* aes, uint32_t flags);
int MXC_AES_RevB_Generic(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req);
int MXC_AES_RevB_Encrypt(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req);
int MXC_AES_RevB_Decrypt(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req);
int MXC_AES_RevB_TXDMAConfig(void *src_addr, int len);
int MXC_AES_RevB_RXDMAConfig(void *dest_addr, int len);
int MXC_AES_RevB_GenericAsync(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req, uint8_t enc);
int MXC_AES_RevB_EncryptAsync(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req);
int MXC_AES_RevB_DecryptAsync(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req);
void MXC_AES_RevB_DMACallback (int ch, int error);
void MXC_AES_RevB_SetExtKey(mxc_aes_key_revb_regs_t* aeskey, const void* key, mxc_aes_keys_t len);
