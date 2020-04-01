/***************************************************************************//**
 * @file
 * @brief Secure Element API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#ifndef EM_SE_H
#define EM_SE_H

#include "em_device.h"

#if defined(SEMAILBOX_PRESENT) || defined(CRYPTOACC_PRESENT)

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup SE
 *
 * @brief Secure Element peripheral API
 *
 * @details
 *   Abstraction of the Secure Element's mailbox interface.
 *
 *   @note Although commands to interact with the mailbox directly are
 *   available, it is always recommended to use the higher level APIs available
 *   in em_se and through mbedTLS.
 *
 *   @note Using the SE's mailbox is not thread-safe in emlib, and accessing the
 *   SE's mailbox both in regular and IRQ context is not safe, either. If
 *   mbedTLS is compiled into the application, SE operations should be wrapped
 *   in se_management_acquire()/se_management_release() calls to synchronize
 *   access. If mbedTLS is not in use, it is the user's responsibility to not
 *   trigger simultaneous use of the SE mailbox.
 *
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   DEFINES    ***********************************
 ******************************************************************************/

#if defined(SEMAILBOX_PRESENT)

/* Command words for the Security Engine. */
#if (defined(_SILICON_LABS_SECURITY_FEATURE) \
  && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT))
#define SE_COMMAND_WRAP_KEY                 0x01000000UL
#define SE_COMMAND_UNWRAP_KEY               0x01020000UL
#define SE_COMMAND_TRANSFER_KEY             0x01060000UL
#endif /* _SILICON_LABS_SECURITY_FEATURE_VAULT */

#define SE_COMMAND_CREATE_KEY               0x02000000UL
#define SE_COMMAND_READPUB_KEY              0x02010000UL

#if (defined(_SILICON_LABS_SECURITY_FEATURE) \
  && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT))
#define SE_COMMAND_DERIVE_KEY_PBKDF2        0x02020002UL
#define SE_COMMAND_DERIVE_KEY_HKDF          0x02020003UL
#endif /* _SILICON_LABS_SECURITY_FEATURE_VAULT */

#define SE_COMMAND_HASH                     0x03000000UL
#define SE_COMMAND_HASHUPDATE               0x03010000UL
#define SE_COMMAND_HMAC                     0x03020000UL

#define SE_COMMAND_AES_ENCRYPT              0x04000000UL
#define SE_COMMAND_AES_DECRYPT              0x04010000UL
#define SE_COMMAND_AES_GCM_ENCRYPT          0x04020000UL
#define SE_COMMAND_AES_GCM_DECRYPT          0x04030000UL
#define SE_COMMAND_AES_CMAC                 0x04040000UL
#define SE_COMMAND_AES_CCM_ENCRYPT          0x04050000UL
#define SE_COMMAND_AES_CCM_DECRYPT          0x04060000UL

#define SE_COMMAND_SIGNATURE_SIGN           0x06000000UL
#define SE_COMMAND_SIGNATURE_VERIFY         0x06010000UL
#if (defined(_SILICON_LABS_SECURITY_FEATURE) \
  && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT))
#define SE_COMMAND_EDDSA_SIGN               0x06020000UL
#define SE_COMMAND_EDDSA_VERIFY             0x06030000UL
#endif /* _SILICON_LABS_SECURITY_FEATURE_VAULT */

#define SE_COMMAND_TRNG_GET_RANDOM          0x07000000UL
#define SE_COMMAND_READ_CLOCK               0x07020000UL

#if (defined(_SILICON_LABS_SECURITY_FEATURE) \
  && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT))
#define SE_COMMAND_ATTEST_CERTIFY           0x0A000000UL
#define SE_COMMAND_ATTEST_TIME              0x0A010000UL
#define SE_COMMAND_ATTEST_PUBKEY            0x0A020000UL
#endif /* _SILICON_LABS_SECURITY_FEATURE_VAULT */

#define SE_COMMAND_JPAKE_R1_GENERATE        0x0B000000UL
#define SE_COMMAND_JPAKE_R1_VERIFY          0x0B000100UL
#define SE_COMMAND_JPAKE_R2_GENERATE        0x0B010000UL
#define SE_COMMAND_JPAKE_R2_VERIFY          0x0B010100UL
#define SE_COMMAND_JPAKE_GEN_SESSIONKEY     0x0B020000UL

#if (defined(_SILICON_LABS_SECURITY_FEATURE) \
  && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT))
#define SE_COMMAND_AEAD_ENCRYPT             0x0C000000UL
#define SE_COMMAND_AEAD_DECRYPT             0x0C010000UL
#define SE_COMMAND_CHACHA20_ENCRYPT         0x0C020000UL
#define SE_COMMAND_CHACHA20_DECRYPT         0x0C030000UL
#define SE_COMMAND_POLY1305_KEY_MAC         0x0C040000UL
#endif /* _SILICON_LABS_SECURITY_FEATURE_VAULT */

#define SE_COMMAND_DH                       0x0E000000UL

#endif // #if defined(SEMAILBOX_PRESENT)

#define SE_COMMAND_CHECK_SE_IMAGE           0x43020000UL
#define SE_COMMAND_APPLY_SE_IMAGE           0x43030000UL
#define SE_COMMAND_STATUS_SE_IMAGE          0x43040000UL
#define SE_COMMAND_CHECK_HOST_IMAGE         0x43050001UL
#define SE_COMMAND_APPLY_HOST_IMAGE         0x43060001UL
#define SE_COMMAND_STATUS_HOST_IMAGE        0x43070000UL

#if defined(SEMAILBOX_PRESENT)

#define SE_COMMAND_STATUS_SE_VERSION        0x43080000UL
#define SE_COMMAND_STATUS_OTP_VERSION       0x43080100UL

#define SE_COMMAND_WRITE_USER_DATA          0x43090000UL
#define SE_COMMAND_ERASE_USER_DATA          0x430A0000UL

#define SE_COMMAND_DBG_LOCK_APPLY           0x430C0000
#define SE_COMMAND_DBG_LOCK_ENABLE_SECURE   0x430D0000
#define SE_COMMAND_DBG_LOCK_DISABLE_SECURE  0x430E0000
#define SE_COMMAND_DEVICE_ERASE             0x430F0000
#define SE_COMMAND_DEVICE_ERASE_DISABLE     0x43100000
#define SE_COMMAND_DBG_LOCK_STATUS          0x43110000

#define SE_COMMAND_PROTECTED_REGISTER       0x43210000

#define SE_COMMAND_GET_CHALLENGE            0xFD000000UL
#define SE_COMMAND_ROLL_CHALLENGE           0xFD000100UL
#define SE_COMMAND_OPEN_DEBUG               0xFD010001UL
#define SE_COMMAND_DISABLE_TAMPER           0xFD020001UL

#define SE_COMMAND_READ_SERIAL              0xFE000000UL
#define SE_COMMAND_GET_STATUS               0xFE010000UL
#define SE_COMMAND_READ_PUBKEYBOOT          0xFE020001UL

#define SE_COMMAND_SET_UPGRADEFLAG_SE       0xFE030000UL
#define SE_COMMAND_SET_UPGRADEFLAG_HOST     0xFE030001UL

#define SE_COMMAND_INIT_OTP                 0xFF000001UL
#define SE_COMMAND_INIT_PUBKEY              0xFF070001UL
#define SE_COMMAND_READ_PUBKEY              0xFF080001UL
#define SE_COMMAND_INIT_PUBKEY_SIGNATURE    0xFF090001UL
#define SE_COMMAND_READ_PUBKEY_SIGNATURE    0xFF0A0001UL

/* Command options for the Secure Element commands. */
/** Use MD5 as hash algorithm */
#define SE_COMMAND_OPTION_HASH_MD5          0x00000100UL
/** Use SHA1 as hash algorithm */
#define SE_COMMAND_OPTION_HASH_SHA1         0x00000200UL
/** Use SHA224 as hash algorithm */
#define SE_COMMAND_OPTION_HASH_SHA224       0x00000300UL
/** Use SHA256 as hash algorithm */
#define SE_COMMAND_OPTION_HASH_SHA256       0x00000400UL

#if (defined(_SILICON_LABS_SECURITY_FEATURE) \
  && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT))
/** Use SHA384 as hash algorithm */
#define SE_COMMAND_OPTION_HASH_SHA384       0x00000500UL
/** Use SHA512 as hash algorithm */
#define SE_COMMAND_OPTION_HASH_SHA512       0x00000600UL
#endif /* _SILICON_LABS_SECURITY_FEATURE_VAULT */

/** Execute algorithm in ECB mode */
#define SE_COMMAND_OPTION_MODE_ECB          0x00000100UL
/** Execute algorithm in CBC mode */
#define SE_COMMAND_OPTION_MODE_CBC          0x00000200UL
/** Execute algorithm in CTR mode */
#define SE_COMMAND_OPTION_MODE_CTR          0x00000300UL
/** Execute algorithm in CFB mode */
#define SE_COMMAND_OPTION_MODE_CFB          0x00000400UL
/** Execute algorithm in OFB mode */
#define SE_COMMAND_OPTION_MODE_OFB          0x00000500UL
/** Execute algorithm in XTS mode */
#define SE_COMMAND_OPTION_MODE_XTS          0x00000800UL

#define SE_COMMAND_OPTION_CERT_DEVICE       0x00000100UL
#define SE_COMMAND_OPTION_CERT_BATCH        0x00000200UL
#define SE_COMMAND_OPTION_CERT_FACTORY      0x00000300UL

/** Pubkey types */
#define SE_KEY_TYPE_BOOT                    0x00000100UL
#define SE_KEY_TYPE_AUTH                    0x00000200UL
#define SE_KEY_TYPE_ROOT                    0x00000300UL

/** Run the whole algorithm, all data present */
#define SE_COMMAND_OPTION_CONTEXT_WHOLE     0x00000000UL
/** Start the algorithm, but get a context to later add more data */
#define SE_COMMAND_OPTION_CONTEXT_START     0x00000001UL
/** End the algorithm, get the result */
#define SE_COMMAND_OPTION_CONTEXT_END       0x00000002UL
/** Add more data input to the algorithm. Need to supply previous context,
 *  and get a context back */
#define SE_COMMAND_OPTION_CONTEXT_ADD       0x00000003UL

/** Padding options for signature functionality. */
#define SE_COMMAND_OPTION_PADDING_NONE      0x00000000UL
#define SE_COMMAND_OPTION_PADDING_EMSA_PKCS 0x00000003UL
#define SE_COMMAND_OPTION_PADDING_PSS       0x00000004UL

/* Special parameters for the Secure Element commands. */
#define SE_COMMAND_OPTION_READ              0x00000000UL
#define SE_COMMAND_OPTION_WRITE             0x00000100UL

/** Magic paramater for deleting user data */
#define SE_COMMAND_OPTION_ERASE_UD          0xDE1E7EADUL

#endif // #if defined(SEMAILBOX_PRESENT)

/* Response status codes for the Secure Element */
#define SE_RESPONSE_MASK                    0x000F0000UL
/** Command executed successfully or signature was successfully validated. */
#define SE_RESPONSE_OK                      0x00000000UL
/**
 * Command was not recognized as a valid command, or is not allowed in the
 * current context.
 */
#define SE_RESPONSE_INVALID_COMMAND         0x00010000UL
/**
 * User did not provide the required credentials to be allowed to execute the
 * command.
 */
#define SE_RESPONSE_AUTHORIZATION_ERROR     0x00020000UL
/**
 * Signature validation command (e.g. SE_COMMAND_SIGNATURE_VERIFY) failed to
 * verify the given signature as being correct.
 */
#define SE_RESPONSE_INVALID_SIGNATURE       0x00030000UL
/** A command started in non-secure mode is trying to access secure memory. */
#define SE_RESPONSE_BUS_ERROR               0x00040000UL
/** Internal error */
#define SE_RESPONSE_INTERNAL_ERROR          0x00050000UL
/** An internal error was raised and the command did not execute. */
#define SE_RESPONSE_CRYPTO_ERROR            0x00060000UL
/** One of the passed parameters is deemed invalid (e.g. out of bounds). */
#define SE_RESPONSE_INVALID_PARAMETER       0x00070000UL
/* Abort status code is given when no operation is attempted. */
#define SE_RESPONSE_ABORT                   0x00090000UL
#if defined(CRYPTOACC_PRESENT) && !defined(SEMAILBOX_PRESENT)
/* Root Code Mailbox is invalid. */
#define SE_RESPONSE_MAILBOX_INVALID         0x000A0000UL
/* Root Code Mailbox magic word */
#define SE_RESPONSE_MAILBOX_VALID           0xE5ECC0DEUL
#endif

#define SE_DATATRANSFER_STOP                0x00000001UL
#define SE_DATATRANSFER_DISCARD             0x40000000UL
#define SE_DATATRANSFER_REALIGN             0x20000000UL
#define SE_DATATRANSFER_CONSTADDRESS        0x10000000UL
#define SE_DATATRANSFER_LENGTH_MASK         0x0FFFFFFFUL

/** Maximum amount of parameters for largest command in defined command set */
#ifndef SE_MAX_PARAMETERS
#define SE_MAX_PARAMETERS                   4U
#endif

/** Maximum amount of parameters supported by the hardware FIFO */
#define SE_FIFO_MAX_PARAMETERS              13U

/* Sanity-check defines */
#if SE_MAX_PARAMETERS > SE_FIFO_MAX_PARAMETERS
#error "Trying to configure more parameters than supported by the hardware"
#endif

/*******************************************************************************
 ******************************   TYPEDEFS   ***********************************
 ******************************************************************************/

/**
 * SE DMA transfer descriptor. Can be linked to each other to provide
 * scatter-gather behavior.
 */
typedef struct {
  volatile void* volatile data;
  void* volatile next;
  volatile uint32_t length;
} SE_DataTransfer_t;

/** Default initialization of data transfer struct */
#define SE_DATATRANSFER_DEFAULT(address, length)                               \
  {                                                                            \
    (void*)(address),                  /* Pointer to data block */             \
    (void*)SE_DATATRANSFER_STOP,       /* This is the last block by default */ \
    (length) | SE_DATATRANSFER_REALIGN /* Add size, use realign by default */  \
  }

/**
 * SE Command structure to which all commands to the SE must adhere.
 */
typedef struct {
  uint32_t command;
  SE_DataTransfer_t* data_in;
  SE_DataTransfer_t* data_out;
  uint32_t parameters[SE_MAX_PARAMETERS];
  size_t num_parameters;
} SE_Command_t;

/** Default initialization of command struct */
#define SE_COMMAND_DEFAULT(command)       \
  {                                       \
    (command),        /* Given command */ \
    NULL,             /* No data in */    \
    NULL,             /* No data out */   \
    { 0, 0, 0, 0 },   /* No parameters */ \
    0                 /* No parameters */ \
  }

/** Possible responses to a command */
typedef uint32_t SE_Response_t;

typedef struct {
  /** Enable secure boot for the host. */
  bool enableSecureBoot;
  /** Require certificate based secure boot signing. */
  bool verifySecureBootCertificate;
  /** Enable anti-rollback for host application upgrades. */
  bool enableAntiRollback;

  /** Set flag to enable locking down all flash pages that cover the
   * secure-booted image, except the last page if end of signature is not
   * page-aligned. */
  bool secureBootPageLockNarrow;
  /** Set flag to enable locking down all flash pages that cover the
   * secure-booted image, including the last page if end of signature is not
   * page-aligned. */
  bool secureBootPageLockFull;
} SE_OTPInit_t;

typedef struct {
  /** Whether debug lock is enabled */
  bool debugLockEnabled;
  /** Whether device erase is enabled */
  bool deviceEraseEnabled;
  /** Whether secure debug is enabled */
  bool secureDebugEnabled;
} SE_DebugStatus_t;

typedef struct {
  /** Boot status code / error code (Bits [7:0]). */
  uint32_t bootStatus;
  /** SE firmware version. */
  uint32_t seFwVersion;
  /** Host firmware version (if available). */
  uint32_t hostFwVersion;
  /** Debug lock status. */
  SE_DebugStatus_t debugStatus;
  /** Secure boot enabled. */
  bool secureBootEnabled;
} SE_Status_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void SE_addDataInput(SE_Command_t *command,
                     SE_DataTransfer_t *data);

void SE_addDataOutput(SE_Command_t *command,
                      SE_DataTransfer_t *data);

void SE_addParameter(SE_Command_t *command, uint32_t parameter);

void SE_executeCommand(SE_Command_t *command);

#if defined(SEMAILBOX_PRESENT)

// User data commands
SE_Response_t SE_writeUserData(uint32_t offset,
                               void *data,
                               uint32_t numBytes);

SE_Response_t SE_eraseUserData(void);

// Initialization commands
SE_Response_t SE_readPubkey(uint32_t key_type,
                            void* pubkey,
                            uint32_t numBytes,
                            bool signature);
SE_Response_t SE_initPubkey(uint32_t key_type,
                            void* pubkey,
                            uint32_t numBytes,
                            bool signature);
SE_Response_t SE_initOTP(SE_OTPInit_t *otp_init);

// Debug commands
SE_Response_t SE_debugLockStatus(SE_DebugStatus_t *status);
SE_Response_t SE_debugLockApply(void);
SE_Response_t SE_debugSecureEnable(void);
SE_Response_t SE_debugSecureDisable(void);
SE_Response_t SE_deviceEraseDisable(void);
SE_Response_t SE_deviceErase(void);

// Device status commands
SE_Response_t SE_getStatus(SE_Status_t *output);
SE_Response_t SE_serialNumber(void *serial);

#else

SE_Response_t SE_getVersion(uint32_t *version);
SE_Response_t SE_getConfigStatusBits(uint32_t *cfgStatus);
SE_Response_t SE_ackCommand(SE_Command_t *command);

#endif // #if defined(SEMAILBOX_PRESENT)

// Utilities
#if defined(SEMAILBOX_PRESENT)
__STATIC_INLINE bool SE_isCommandCompleted(void);
__STATIC_INLINE SE_Response_t SE_readCommandResponse(void);
#elif defined(CRYPTOACC_PRESENT)
bool SE_isCommandCompleted(void);
uint32_t SE_readExecutedCommand(void);
SE_Response_t SE_readCommandResponse(void);
#endif // #if defined(SEMAILBOX_PRESENT)

__STATIC_INLINE void SE_waitCommandCompletion(void);
__STATIC_INLINE void SE_disableInterrupt(uint32_t flags);
__STATIC_INLINE void SE_enableInterrupt(uint32_t flags);

#if defined(SEMAILBOX_PRESENT)
/***************************************************************************//**
 * @brief
 *   Check whether the running command has completed.
 *
 * @details
 *   This function polls the SE-to-host mailbox interrupt flag.
 *
 * @return True if a command has completed and the result is available
 ******************************************************************************/
__STATIC_INLINE bool SE_isCommandCompleted(void)
{
  return (bool)(SEMAILBOX_HOST->RX_STATUS & SEMAILBOX_RX_STATUS_RXINT);
}
#endif

/***************************************************************************//**
 * @brief
 *   Wait for completion of the current command.
 *
 * @details
 *   This function "busy"-waits until the execution of the ongoing instruction
 *   has completed.
 ******************************************************************************/
__STATIC_INLINE void SE_waitCommandCompletion(void)
{
  /* Wait for completion */
  while (!SE_isCommandCompleted()) {
  }
}

#if defined(SEMAILBOX_PRESENT)
/***************************************************************************//**
 * @brief
 *   Read the status of the previously executed command.
 *
 * @details
 *   This function reads the status of the previously executed command.
 *
 * @note
 *   The command response needs to be read for every executed command, and can
 *   only be read once per executed command (FIFO behavior).
 *
 * @return
 *   One of the SE_RESPONSE return codes:
 *   SE_RESPONSE_OK when the command was executed successfully or a signature
 *   was successfully verified,
 *   SE_RESPONSE_INVALID_COMMAND when the command ID was not recognized,
 *   SE_RESPONSE_AUTHORIZATION_ERROR when the command is not authorized,
 *   SE_RESPONSE_INVALID_SIGNATURE when signature verification failed,
 *   SE_RESPONSE_BUS_ERROR when a bus error was thrown during the command, e.g.
 *   because of conflicting Secure/Non-Secure memory accesses,
 *   SE_RESPONSE_CRYPTO_ERROR on an internal SE failure, or
 *   SE_RESPONSE_INVALID_PARAMETER when an invalid parameter was passed
 ******************************************************************************/
__STATIC_INLINE SE_Response_t SE_readCommandResponse(void)
{
  SE_waitCommandCompletion();
  return (SE_Response_t)(SEMAILBOX_HOST->RX_HEADER & SE_RESPONSE_MASK);
}
#endif // #if defined(SEMAILBOX_PRESENT)

/***************************************************************************//**
 * @brief
 *   Disable one or more SE interrupts.
 *
 * @param[in] flags
 *   SE interrupt sources to disable. Use a bitwise logic OR combination of
 *   valid interrupt flags for the Secure Element module
 *    (SE_CONFIGURATION_(TX/RX)INTEN).
 ******************************************************************************/
__STATIC_INLINE void SE_disableInterrupt(uint32_t flags)
{
#if defined(SEMAILBOX_PRESENT)
  SEMAILBOX_HOST->CONFIGURATION &= ~flags;
#else
  (void) flags;
#endif
}

/***************************************************************************//**
 * @brief
 *   Enable one or more SE interrupts.
 *
 * @param[in] flags
 *   SE interrupt sources to enable. Use a bitwise logic OR combination of
 *   valid interrupt flags for the Secure Element module
 *   (SEMAILBOX_CONFIGURATION_TXINTEN or SEMAILBOX_CONFIGURATION_RXINTEN).
 ******************************************************************************/
__STATIC_INLINE void SE_enableInterrupt(uint32_t flags)
{
#if defined(SEMAILBOX_PRESENT)
  SEMAILBOX_HOST->CONFIGURATION |= flags;
#else
  (void) flags;
#endif
}

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup SE) */
/** @} (end addtogroup emlib) */

#endif /* defined(SEMAILBOX_PRESENT)
       || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2) */

#endif /* EM_SE_H */
