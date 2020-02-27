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
#include "em_device.h"

#if defined(SEMAILBOX_PRESENT) || defined(CRYPTOACC_PRESENT)

#include "em_se.h"
#include "em_assert.h"

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup SE
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   DEFINES    ***********************************
 ******************************************************************************/

#if defined(SEMAILBOX_PRESENT)

/* OTP initialization structure defines. */
#define SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_ENABLE (1 << 16)
#define SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_VERIFY_CERTIFICATE (1 << 17)
#define SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_ANTI_ROLLBACK (1 << 18)
#define SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_PAGE_LOCK_NARROW (1 << 19)
#define SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_PAGE_LOCK_FULL (1 << 20)

#elif defined(CRYPTOACC_PRESENT)

/* Size of Root Code Mailbox instance.
   There are two instances, input and output. */
#define ROOT_MAILBOX_SIZE  (512UL)

/* Base addresses of the Root Code Input and Output Mailbox data structures.
   (Must be stored in a RAM area which is not used by the root code)
   We use the upper 1KB of FRC RAM for the root code mailboxes. */
#define ROOT_MAILBOX_OUTPUT_BASE (RDMEM_FRCRAM_S_MEM_END + 1 - ROOT_MAILBOX_SIZE)
#define ROOT_MAILBOX_INPUT_BASE  (ROOT_MAILBOX_OUTPUT_BASE - ROOT_MAILBOX_SIZE)

/* Position of parameter number field in Root Code Input Mailbox LENGTH field.*/
#define ROOT_MB_LENGTH_PARAM_NUM_SHIFT (24)

/* Done flag indicating that the Root Code Mailbox handler has completed
   processing the mailbox command. */
#define ROOT_MB_DONE  (1 << 23)

/* Root Code Configuration Status bits mask */
#define ROOT_MB_OUTPUT_STATUS_CONFIG_BITS_MASK  (0xFFFF)

#endif // #if defined(SEMAILBOX_PRESENT)

/*******************************************************************************
 ******************************   TYPEDEFS   ***********************************
 ******************************************************************************/
#if defined(CRYPTOACC_PRESENT)

// Root Code Input Mailbox structure
typedef struct {
  volatile uint32_t magic;
  volatile uint32_t command;
  volatile uint32_t length;
  volatile uint32_t data[0];
} root_InputMailbox_t;

// Root Code Output Mailbox structure
typedef struct {
  volatile uint32_t magic;
  volatile uint32_t version;
  volatile uint32_t status;
  volatile uint32_t command;
  volatile uint32_t length;
  volatile uint32_t data[0];
} root_OutputMailbox_t;

#endif // #if defined(CRYPTOACC_PRESENT)

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Add input data to a command
 *
 * @details
 *   This function adds a buffer of input data to the given SE command structure
 *   The buffer gets appended by reference at the end of the list of already
 *   added buffers.
 *
 * @note
 *   Note that this function does not copy either the data buffer or the buffer
 *   structure, so make sure to keep the data object in scope until the command
 *   has been executed by the secure element.
 *
 * @param[in]  command
 *   Pointer to an SE command structure.
 *
 * @param[in]  data
 *   Pointer to a data transfer structure.
 ******************************************************************************/
void SE_addDataInput(SE_Command_t *command, SE_DataTransfer_t *data)
{
  if (command->data_in == NULL) {
    command->data_in = data;
  } else {
    SE_DataTransfer_t *next = command->data_in;
    while (next->next != (void*)SE_DATATRANSFER_STOP) {
      next = (SE_DataTransfer_t*)next->next;
    }
    next->next = data;
  }
}

/***************************************************************************//**
 * @brief
 *   Add output data to a command
 *
 * @details
 *   This function adds a buffer of output data to the given command structure
 *   The buffer gets appended by reference at the end of the list of already
 *   added buffers.
 *
 * @note
 *   Note that this function does not copy either the data buffer or the buffer
 *   structure, so make sure to keep the data object in scope until the command
 *   has been executed by the secure element.
 *
 * @param[in]  command
 *   Pointer to an SE command structure.
 *
 * @param[in]  data
 *   Pointer to a data transfer structure.
 ******************************************************************************/
void SE_addDataOutput(SE_Command_t *command,
                      SE_DataTransfer_t *data)
{
  if (command->data_out == NULL) {
    command->data_out = data;
  } else {
    SE_DataTransfer_t *next = command->data_out;
    while (next->next != (void*)SE_DATATRANSFER_STOP) {
      next = (SE_DataTransfer_t*)next->next;
    }
    next->next = data;
  }
}

/***************************************************************************//**
 * @brief
 *   Add a parameter to a command
 *
 * @details
 *   This function adds a parameter word to the passed command.
 *
 * @note
 *   Make sure to not exceed @ref SE_MAX_PARAMETERS.
 *
 * @param[in]  command
 *   Pointer to a filled-out SE command structure.
 * @param[in]  parameter
 *   Parameter to add.
 ******************************************************************************/
void SE_addParameter(SE_Command_t *command, uint32_t parameter)
{
  if (command->num_parameters >= SE_MAX_PARAMETERS) {
    EFM_ASSERT(command->num_parameters < SE_MAX_PARAMETERS);
    return;
  }

  command->parameters[command->num_parameters] = parameter;
  command->num_parameters += 1;
}

/***************************************************************************//**
 * @brief
 *   Execute the passed command
 *
 * @details
 *   This function starts the execution of the passed command by the secure
 *   element. When started, wait for the RXINT interrupt flag, or call
 *   @ref SE_waitCommandCompletion to busy-wait. After completion, you have to
 *   call @ref SE_readCommandResponse to get the command's execution status.
 *
 * @param[in]  command
 *   Pointer to a filled-out SE command structure.
 ******************************************************************************/
void SE_executeCommand(SE_Command_t *command)
{
  // Don't overflow our struct
  if (command->num_parameters > SE_MAX_PARAMETERS) {
    EFM_ASSERT(command->num_parameters <= SE_MAX_PARAMETERS);
    return;
  }

#if defined(SEMAILBOX_PRESENT)

  // Wait for room available in the mailbox
  while (!(SEMAILBOX_HOST->TX_STATUS & SEMAILBOX_TX_STATUS_TXINT)) ;

  // Write header to start transaction
  SEMAILBOX_HOST->TX_HEADER = sizeof(uint32_t) * (4 + command->num_parameters);

  // Write command into FIFO
  SEMAILBOX_HOST->FIFO[0].DATA = command->command;

  // Write DMA descriptors into FIFO
  SEMAILBOX_HOST->FIFO[0].DATA = (uint32_t)command->data_in;
  SEMAILBOX_HOST->FIFO[0].DATA = (uint32_t)command->data_out;

  // Write applicable parameters into FIFO
  for (size_t i = 0; i < command->num_parameters; i++) {
    SEMAILBOX_HOST->FIFO[0].DATA = command->parameters[i];
  }

#elif defined(CRYPTOACC_PRESENT)

  // Setup pointer to the Root Code Mailbox Input data structure
  // (must be stored in a RAM area which is not used by the root code)
  root_InputMailbox_t *rootInMb = (root_InputMailbox_t*)ROOT_MAILBOX_INPUT_BASE;
  uint32_t *mbData;
  unsigned int mbDataLen, inDataLen, i;
  SE_DataTransfer_t *inDataDesc;
  uint32_t *inData;
  uint32_t checksum;
  bool sysCfgClkWasEnabled = ((CMU->CLKEN0 & CMU_CLKEN0_SYSCFG) != 0);
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
  // Set base of Mailbox Input data structure in SYSCFG register in order
  // for Root Code to find it.
  SYSCFG->ROOTDATA0 = ROOT_MAILBOX_INPUT_BASE;

  // Set base of Mailbox Output data structure in SYSCFG register in order
  // for Root Code to know where to write output data.
  // Write command into FIFO
  SYSCFG->ROOTDATA1 = ROOT_MAILBOX_OUTPUT_BASE;
  if (!sysCfgClkWasEnabled) {
    CMU->CLKEN0_CLR = CMU_CLKEN0_SYSCFG;
  }

  rootInMb->magic   = SE_RESPONSE_MAILBOX_VALID;
  rootInMb->command = command->command;

  // Write applicable parameters into Mailbox DATA array
  mbData = (uint32_t*) rootInMb->data;
  for (mbDataLen = 0; mbDataLen < command->num_parameters; mbDataLen++) {
    mbData[mbDataLen] = command->parameters[mbDataLen];
  }

  // Write input data into Mailbox DATA array
  for (inDataDesc = command->data_in, inDataLen = 0; inDataDesc;
       inDataDesc = (SE_DataTransfer_t*) inDataDesc->next) {
    inData = (uint32_t*) inDataDesc->data;
    for (i = 0; i < inDataDesc->length; i++, inDataLen++) {
      // Make sure we do not overflow the input mailbox.
      EFM_ASSERT(mbDataLen < ROOT_MAILBOX_SIZE);
      mbData[mbDataLen++] = inData[i];
    }
  }

  // Write number of parameters and data words to 'length' field of mailbox.
  rootInMb->length =
    inDataLen | (command->num_parameters << ROOT_MB_LENGTH_PARAM_NUM_SHIFT);

  // Calculate checksum using bitwise XOR over the all words in the mailbox
  // data structure, minus the CHECKSUM word (32bit = 4bytes ) at the end.
  checksum = rootInMb->magic;
  checksum ^= rootInMb->command;
  checksum ^= rootInMb->length;
  for (i = 0; i < mbDataLen; i++) {
    checksum ^= mbData[i];
  }

  // Finally, write the calculated checksum to mailbox checksum field
  mbData[mbDataLen] = checksum;

  __NVIC_SystemReset();

#endif // #if defined(SEMAILBOX_PRESENT)

  return;
}

#if defined(CRYPTOACC_PRESENT)

/***************************************************************************//**
 * @brief
 *   Check whether the Root Code Output Mailbox is valid.
 *
 * @return True if the Root Code Output Mailbox is valid (magic and checksum OK)
 ******************************************************************************/
bool rootIsOutputMailboxValid(void)
{
  // Setup pointer to the Root Code Output Mailbox data structure
  // (must be stored in a RAM area which is not used by the root code)
  bool sysCfgClkWasEnabled = ((CMU->CLKEN0 & CMU_CLKEN0_SYSCFG) != 0);
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
  root_OutputMailbox_t *rootOutMb = (root_OutputMailbox_t *) SYSCFG->ROOTDATA1;
  if (!sysCfgClkWasEnabled) {
    CMU->CLKEN0_CLR = CMU_CLKEN0_SYSCFG;
  }

  uint32_t *mbPtr = (uint32_t*) rootOutMb;
  uint32_t checksum;
  unsigned int mbLen, cnt;

  // Verify magic word of mailbox
  if (rootOutMb->magic != SE_RESPONSE_MAILBOX_VALID) {
    return false;
  }

  // Get length of mailbox
  mbLen = sizeof(root_OutputMailbox_t) / sizeof(uint32_t) + rootOutMb->length;
  if (mbLen >= ROOT_MAILBOX_SIZE) {
    return false;
  }
  // Calculate checksum using bitwise XOR over all words in the mailbox
  // data structure, minus the CHECKSUM word at the end.
  for (checksum = 0, cnt = 0; cnt < mbLen; cnt++) {
    checksum ^= mbPtr[cnt];
  }

  // Verify that the calculated checksum is equal to the mailbox checksum.
  return (mbPtr[mbLen] == checksum);
}

/***************************************************************************//**
 * @brief
 *   Get current SE version
 *
 * @details
 *   This function returns the current root code version
 *
 * @param[in]  version
 *   Pointer to location where to copy the version of root code to.
 *
 * @return
 *   One of the SE_RESPONSE return codes:
 *   SE_RESPONSE_OK when the command was executed successfully
 *   SE_RESPONSE_INVALID_PARAMETER when an invalid parameter was passed
 *   SE_RESPONSE_MAILBOX_INVALID when the mailbox content is invalid
 ******************************************************************************/
SE_Response_t SE_getVersion(uint32_t *version)
{
  bool sysCfgClkWasEnabled = ((CMU->CLKEN0 & CMU_CLKEN0_SYSCFG) != 0);
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
  root_OutputMailbox_t *rootOutMb = (root_OutputMailbox_t *) SYSCFG->ROOTDATA1;
  if (!sysCfgClkWasEnabled) {
    CMU->CLKEN0_CLR = CMU_CLKEN0_SYSCFG;
  }

  if (version == NULL) {
    return SE_RESPONSE_INVALID_PARAMETER;
  }

  // First verify that the response is ok.
  if (!rootIsOutputMailboxValid()) {
    return SE_RESPONSE_MAILBOX_INVALID;
  }

  // Return the 'version' from the Output Mailbox
  *version = rootOutMb->version;

  return SE_RESPONSE_OK;
}

/***************************************************************************//**
 * @brief
 *   Get Root Code Configuration Status bits
 *
 * @details
 *   This function returns the current Root Code Configuration Status bits
 *
 * @param[in]  cfgStatus
 *   Pointer to location where to copy Configuration Status bits
 *   of the root code.
 *
 * @return
 *   One of the SE_RESPONSE return codes:
 *   SE_RESPONSE_OK when the command was executed successfully
 *   SE_RESPONSE_INVALID_PARAMETER when an invalid parameter was passed
 *   SE_RESPONSE_MAILBOX_INVALID when the mailbox content is invalid
 ******************************************************************************/
SE_Response_t SE_getConfigStatusBits(uint32_t *cfgStatus)
{
  bool sysCfgClkWasEnabled = ((CMU->CLKEN0 & CMU_CLKEN0_SYSCFG) != 0);
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
  root_OutputMailbox_t *rootOutMb = (root_OutputMailbox_t *) SYSCFG->ROOTDATA1;
  if (!sysCfgClkWasEnabled) {
    CMU->CLKEN0_CLR = CMU_CLKEN0_SYSCFG;
  }

  if (cfgStatus == NULL) {
    return SE_RESPONSE_INVALID_PARAMETER;
  }

  // First verify that the response is ok.
  if (!rootIsOutputMailboxValid()) {
    return SE_RESPONSE_MAILBOX_INVALID;
  }

  // Return the configuration status bits
  *cfgStatus = rootOutMb->status & ROOT_MB_OUTPUT_STATUS_CONFIG_BITS_MASK;

  return SE_RESPONSE_OK;
}

/***************************************************************************//**
 * @brief
 *   Check whether the running command has completed.
 *
 * @details
 *   This function polls the SE-to-host mailbox interrupt flag.
 *
 * @return True if a command has completed and the result is available
 ******************************************************************************/
bool SE_isCommandCompleted(void)
{
  bool sysCfgClkWasEnabled = ((CMU->CLKEN0 & CMU_CLKEN0_SYSCFG) != 0);
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
  root_OutputMailbox_t *rootOutMb = (root_OutputMailbox_t *) SYSCFG->ROOTDATA1;
  if (!sysCfgClkWasEnabled) {
    CMU->CLKEN0_CLR = CMU_CLKEN0_SYSCFG;
  }

  // First verify that the response is ok
  if (!rootIsOutputMailboxValid()) {
    return false;
  }

  // Check status MB_DONE flag of the mailbox
  return ((rootOutMb->status & ROOT_MB_DONE) == ROOT_MB_DONE);
}

/***************************************************************************//**
 * @brief
 *   Read the previously executed command.
 *
 * @details
 *   This function reads the previously executed command.
 *
 * @return
 *   One of the SE command words.
 *   SE_RESPONSE_MAILBOX_INVALID when the mailbox content is invalid.
 ******************************************************************************/
uint32_t SE_readExecutedCommand(void)
{
  bool sysCfgClkWasEnabled = ((CMU->CLKEN0 & CMU_CLKEN0_SYSCFG) != 0);
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
  root_OutputMailbox_t *rootOutMb = (root_OutputMailbox_t *) SYSCFG->ROOTDATA1;
  if (!sysCfgClkWasEnabled) {
    CMU->CLKEN0_CLR = CMU_CLKEN0_SYSCFG;
  }

  // First verify that the Output Mailbox includes a valid response.
  if (!SE_isCommandCompleted()) {
    return SE_RESPONSE_MAILBOX_INVALID;
  }

  return rootOutMb->command;
}

/***************************************************************************//**
 * @brief
 *   Read the status of the previously executed command.
 *
 * @details
 *   This function reads the status of the previously executed command.
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
 *   SE_RESPONSE_MAILBOX_INVALID when the mailbox content is invalid
 ******************************************************************************/
SE_Response_t SE_readCommandResponse(void)
{
  bool sysCfgClkWasEnabled = ((CMU->CLKEN0 & CMU_CLKEN0_SYSCFG) != 0);
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
  root_OutputMailbox_t *rootOutMb = (root_OutputMailbox_t *) SYSCFG->ROOTDATA1;
  if (!sysCfgClkWasEnabled) {
    CMU->CLKEN0_CLR = CMU_CLKEN0_SYSCFG;
  }

  // First verify that the Output Mailbox includes a valid response.
  if (!SE_isCommandCompleted()) {
    return SE_RESPONSE_MAILBOX_INVALID;
  }

  return (SE_Response_t)(rootOutMb->status & SE_RESPONSE_MASK);
}

/***************************************************************************//**
 * @brief
 *   Acknowledge and get status and output data of a completed command.
 *
 * @details
 *   This function acknowledges and gets the status and output data of a
 *   completed mailbox command.
 *   The mailbox command is acknowledged by inverting all bits in the checksum
 *   (XOR with 0xFFFFFFFF).
 *   The output data is copied into the linked list of output buffers pointed
 *   to in the given command data structure.
 *
 * @param[in]  command
 *   Pointer to a filled-out SE command structure.
 *
 * @return
 *   One of the SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully or a
 *                        signature was successfully verified,
 * @retval SE_RESPONSE_INVALID_COMMAND when the command ID was not recognized,
 * @retval SE_RESPONSE_AUTHORIZATION_ERROR when the command is not authorized,
 * @retval SE_RESPONSE_INVALID_SIGNATURE when signature verification failed,
 * @retval SE_RESPONSE_BUS_ERROR when a bus error was thrown during the command,
 *                               e.g. because of conflicting Secure/Non-Secure
 *                               memory accesses,
 * @retval SE_RESPONSE_CRYPTO_ERROR on an internal SE failure, or
 * @retval SE_RESPONSE_INVALID_PARAMETER when an invalid parameter was passed
 * @retval SE_RESPONSE_MAILBOX_INVALID when mailbox command not done or invalid
 ******************************************************************************/
SE_Response_t SE_ackCommand(SE_Command_t *command)
{
  // Setup pointer to the Root Code Output Mailbox data structure
  // (must be stored in a RAM area which is not used by the root code)
  bool sysCfgClkWasEnabled = ((CMU->CLKEN0 & CMU_CLKEN0_SYSCFG) != 0);
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
  root_OutputMailbox_t *rootOutMb = (root_OutputMailbox_t *) SYSCFG->ROOTDATA1;
  if (!sysCfgClkWasEnabled) {
    CMU->CLKEN0_CLR = CMU_CLKEN0_SYSCFG;
  }
  uint32_t *mbData = (uint32_t*) rootOutMb->data;
  SE_DataTransfer_t *outDataDesc = command->data_out;
  unsigned int outDataLen, outDataCnt, i, outDescLen;
  uint32_t *outData;

  // First verify that the Output Mailbox includes a valid response.
  if (!SE_isCommandCompleted()) {
    return SE_RESPONSE_MAILBOX_INVALID;
  }

  // Get output data length
  outDataLen = rootOutMb->length;

  // Acknowledge the output mailbox response by invalidating checksum
  mbData[outDataLen] ^= 0xFFFFFFFFUL;

  // Check command status code
  if ((rootOutMb->status & SE_RESPONSE_MASK) != SE_RESPONSE_OK) {
    return rootOutMb->status & SE_RESPONSE_MASK;
  }

  // Copy data from the Output Mailbox to the linked list of output
  // buffers provided by the user
  outDataCnt = 0;
  while (outDataDesc && (outDataCnt < outDataLen)) {
    outData = (uint32_t*) outDataDesc->data;
    outDescLen =
      (outDataDesc->length & SE_DATATRANSFER_LENGTH_MASK) / sizeof(uint32_t);
    for (i = 0; (i < outDescLen) && (outDataCnt < outDataLen); i++) {
      outData[i] = mbData[outDataCnt++];
    }
    // If we have reached the end of a buffer, go to next buffer descriptor
    if (i == outDescLen) {
      outDataDesc = (SE_DataTransfer_t*)
                    ((uint32_t)outDataDesc->next & ~SE_DATATRANSFER_STOP);
    }
  }

  // Check if the output data list is too small to copy all output data in
  // mailbox.
  if ((outDataDesc == 0) && (outDataCnt < outDataLen)) {
    return SE_RESPONSE_INVALID_PARAMETER;
  }

  return SE_RESPONSE_OK;
}

#endif // #if defined(CRYPTOACC_PRESENT)

#if defined(SEMAILBOX_PRESENT)

/***************************************************************************//**
 * @brief
 *   Writes data to User Data section in MTP. Write data must be aligned to
 *    word size and contain a number of bytes that is divisable by four.
 * @note
 *   It is recommended to erase the flash page before performing a write.
 *
 * @param[in] offset
 *   Offset to the flash word to write to. Must be aligned to words.
 * @param[in] data
 *   Data to write to flash.
 * @param[in] numBytes
 *   Number of bytes to write to flash. NB: Must be divisable by four.
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully or a
 *                        signature was successfully verified,
 * @retval SE_RESPONSE_INVALID_COMMAND when the command ID was not recognized,
 * @retval SE_RESPONSE_AUTHORIZATION_ERROR when the command is not authorized,
 * @retval SE_RESPONSE_INVALID_SIGNATURE when signature verification failed,
 * @retval SE_RESPONSE_BUS_ERROR when a bus error was thrown during the command,
 *                               e.g. because of conflicting Secure/Non-Secure
 *                               memory accesses,
 * @retval SE_RESPONSE_CRYPTO_ERROR on an internal SE failure, or
 * @retval SE_RESPONSE_INVALID_PARAMETER when an invalid parameter was passed
 ******************************************************************************/
SE_Response_t SE_writeUserData(uint32_t offset,
                               void *data,
                               uint32_t numBytes)
{
  // SE command structures
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_WRITE_USER_DATA);
  SE_DataTransfer_t userData = SE_DATATRANSFER_DEFAULT(data, numBytes);

  SE_addDataInput(&command, &userData);

  SE_addParameter(&command, offset);
  SE_addParameter(&command, numBytes);

  SE_executeCommand(&command);
  SE_Response_t res = SE_readCommandResponse();
  return res;
}

/***************************************************************************//**
 * @brief
 *   Erases User Data section in MTP.
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully or a
 *                        signature was successfully verified,
 * @retval SE_RESPONSE_INVALID_COMMAND when the command ID was not recognized,
 * @retval SE_RESPONSE_AUTHORIZATION_ERROR when the command is not authorized,
 * @retval SE_RESPONSE_INVALID_SIGNATURE when signature verification failed,
 * @retval SE_RESPONSE_BUS_ERROR when a bus error was thrown during the command,
 *                               e.g. because of conflicting Secure/Non-Secure
 *                               memory accesses,
 * @retval SE_RESPONSE_CRYPTO_ERROR on an internal SE failure, or
 * @retval SE_RESPONSE_INVALID_PARAMETER when an invalid parameter was passed
 ******************************************************************************/
SE_Response_t SE_eraseUserData()
{
  // SE command structures
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_ERASE_USER_DATA);

  SE_addParameter(&command, SE_COMMAND_OPTION_ERASE_UD);
  SE_executeCommand(&command);
  SE_Response_t res = SE_readCommandResponse();
  return res;
}

/***************************************************************************//**
 * @brief
 *   Returns the current boot status, versions and system configuration.
 *
 * @param[out] status
 *   @ref SE_Status_t containing current SE status.
 *
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK upon command completion. Errors are encoded in the
 *                        different parts of the returned status object.
 ******************************************************************************/
SE_Response_t SE_getStatus(SE_Status_t *status)
{
  volatile uint32_t output[9] = { 0 };

  // SE command structures
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_GET_STATUS);
  SE_DataTransfer_t outData = SE_DATATRANSFER_DEFAULT((void*)output, 4 * 9);

  SE_addDataOutput(&command, &outData);

  // Execute command and return response
  SE_executeCommand(&command);
  SE_Response_t res = SE_readCommandResponse();

  // Update status object
  status->bootStatus = output[4];
  status->seFwVersion = output[5];
  status->hostFwVersion = output[6];

  SE_DebugStatus_t debugStatus;
  debugStatus.debugLockEnabled = (output[7] & (1 << 0));
  debugStatus.deviceEraseEnabled = (output[7] & (1 << 1));
  debugStatus.secureDebugEnabled = (output[7] & (1 << 2));
  status->debugStatus = debugStatus;

  status->secureBootEnabled = ((output[8] & 0x1) && ((output[8] & ~0x1) == 0));

  return res;
}

/***************************************************************************//**
 * @brief
 *   Read the serial number of the SE module.
 *
 * @param[out] serial
 *   Pointer to array of size 16 bytes.
 *
 * @return
 *   One of the @ref SE_Response_t return codes.
 * @retval SE_RESPONSE_OK when serial number is returned successfully,
 * @retval SE_RESPONSE_INTERNAL_ERROR if not.
 ******************************************************************************/
SE_Response_t SE_serialNumber(void *serial)
{
  // SE command structures
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_READ_SERIAL);
  SE_DataTransfer_t outData = SE_DATATRANSFER_DEFAULT(serial, 16);

  SE_addDataOutput(&command, &outData);

  // Execute command and return response
  SE_executeCommand(&command);
  SE_Response_t res = SE_readCommandResponse();
  return res;
}

/***************************************************************************//**
 * @brief
 *   Read pubkey or pubkey signature.
 *
 * @details
 *   Read out a public key stored in the SE, or its signature. The command can
 *   be used to read:
 *   * @ref SE_KEY_TYPE_BOOT
 *   * @ref SE_KEY_TYPE_AUTH
 *
 * @param[in] key_type
 *   ID of key type to read.
 *
 * @param[out] pubkey
 *   Pointer to a buffer to contain the returned public key.
 *   Must be word aligned and have a length of 64 bytes.
 *
 * @param[in] numBytes
 *   Length of pubkey buffer (64 bytes).
 *
 * @param[in] signature
 *   If true, the function will return the signature programmed for the
 *   specified public key instead of the public key itself.
 *
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully
 * @retval SE_RESPONSE_TEST_FAILED when the pubkey is not set
 * @retval SE_RESPONSE_INVALID_PARAMETER when an invalid type is passed
 ******************************************************************************/
SE_Response_t SE_readPubkey(uint32_t key_type, void *pubkey, uint32_t numBytes, bool signature)
{
  EFM_ASSERT((key_type == SE_KEY_TYPE_BOOT)
             || (key_type == SE_KEY_TYPE_AUTH));

  EFM_ASSERT(numBytes == 64);
  EFM_ASSERT(!((size_t)pubkey & 3U));

  // SE command structures
  uint32_t commandWord =
    (signature) ? SE_COMMAND_READ_PUBKEY_SIGNATURE : SE_COMMAND_READ_PUBKEY;
  SE_Command_t command = SE_COMMAND_DEFAULT(commandWord | key_type);

  SE_DataTransfer_t pubkeyData = SE_DATATRANSFER_DEFAULT(pubkey, numBytes);
  SE_addDataOutput(&command, &pubkeyData);

  SE_executeCommand(&command);
  SE_Response_t res = SE_readCommandResponse();
  return res;
}

/***************************************************************************//**
 * @brief
 *   Init pubkey or pubkey signature.
 *
 * @details
 *   Initialize public key stored in the SE, or its corresponding signature. The
 *   command can be used to write:
 *   * @ref SE_KEY_TYPE_BOOT -- public key used to perform secure boot
 *   * @ref SE_KEY_TYPE_AUTH -- public key used to perform secure debug
 *
 * @note
 *   These keys can not be overwritten, so this command can only be issued once
 *   per key per part.
 *
 * @param[in] key_type
 *   ID of key type to initialize.
 *
 * @param[in] pubkey
 *   Pointer to a buffer that contains the public key or signature.
 *   Must be word aligned and have a length of 64 bytes.
 *
 * @param[in] numBytes
 *   Length of pubkey buffer (64 bytes).
 *
 * @param[in] signature
 *   If true, initialize signature for the specified key type instead of the
 *   public key itself.
 *
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully
 * @retval SE_RESPONSE_TEST_FAILED when the pubkey is not set
 * @retval SE_RESPONSE_INVALID_PARAMETER when an invalid type is passed
 ******************************************************************************/
SE_Response_t SE_initPubkey(uint32_t key_type, void *pubkey, uint32_t numBytes, bool signature)
{
  EFM_ASSERT((key_type == SE_KEY_TYPE_BOOT)
             || (key_type == SE_KEY_TYPE_AUTH));

  EFM_ASSERT(numBytes == 64);
  EFM_ASSERT(!((size_t)pubkey & 3U));

  // Find parity word
  volatile uint32_t parity = 0;
  for (size_t i = 0; i < numBytes / 4; i++) {
    parity = parity ^ ((uint32_t *)pubkey)[i];
  }

  // SE command structures
  uint32_t commandWord =
    (signature) ? SE_COMMAND_INIT_PUBKEY_SIGNATURE : SE_COMMAND_INIT_PUBKEY;
  SE_Command_t command = SE_COMMAND_DEFAULT(commandWord | key_type);

  SE_DataTransfer_t parityData = SE_DATATRANSFER_DEFAULT(&parity, 4);
  SE_addDataInput(&command, &parityData);

  SE_DataTransfer_t pubkeyData = SE_DATATRANSFER_DEFAULT(pubkey, numBytes);
  SE_addDataInput(&command, &pubkeyData);

  SE_executeCommand(&command);
  SE_Response_t res = SE_readCommandResponse();
  return res;
}

/***************************************************************************//**
 * @brief
 *   Initialize SE one-time-programmable (OTP) configuration.
 *
 * @details
 *   Configuration is performed by setting the desired options in the
 *   @ref SE_OTPInit_t structure.
 *
 *   This function can be used to enable secure boot, to configure flash page
 *   locking, and to enable anti-rollback protection when using the SE to
 *   perform an application upgrade, typically a Gecko bootloader upgrade.
 *
 *   Before secure boot can be enabled, the public key used for secure boot
 *   verification must be uploaded using @ref SE_initPubkey().
 *
 * @warning
 *   This command can only be executed once per device! When the configuration
 *   has been programmed it is not possible to update any of the fields.
 *
 * @param[in] otp_init
 *   @ref SE_OTPInit_t structure containing the SE configuration.
 *
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully
 ******************************************************************************/
SE_Response_t SE_initOTP(SE_OTPInit_t *otp_init)
{
  volatile uint32_t mcuSettingsFlags = 0;

  SE_Response_t res;

  if (otp_init->enableSecureBoot) {
    mcuSettingsFlags |= SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_ENABLE;

    uint8_t pubkey[64];
    res = SE_readPubkey(SE_KEY_TYPE_BOOT, &pubkey, 64, false);
    if (res != SE_RESPONSE_OK) {
      return SE_RESPONSE_ABORT;
    }
  }
  if (otp_init->verifySecureBootCertificate) {
    mcuSettingsFlags |= SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_VERIFY_CERTIFICATE;
  }
  if (otp_init->enableAntiRollback) {
    mcuSettingsFlags |= SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_ANTI_ROLLBACK;
  }

  if (otp_init->secureBootPageLockNarrow && otp_init->secureBootPageLockFull) {
    return SE_RESPONSE_ABORT;
  }
  if (otp_init->secureBootPageLockNarrow) {
    mcuSettingsFlags |= SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_PAGE_LOCK_NARROW;
  }
  if (otp_init->secureBootPageLockFull) {
    mcuSettingsFlags |= SE_OTP_MCU_SETTINGS_FLAG_SECURE_BOOT_PAGE_LOCK_FULL;
  }

  volatile struct ReservedSettings {
    uint8_t reserved1[16];
    uint8_t reserved2[2];
    uint8_t reserved3[2];
  } reservedSettings = {
    { 0x00 },
    { 0xFF },
    { 0x00 }
  };

  // Find parity word
  uint32_t parity = 0;
  parity = parity ^ mcuSettingsFlags;
  for (size_t i = 0; i < 5; i++) {
    parity = parity ^ ((uint32_t*)(&reservedSettings))[i];
  }

  // SE command structures
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_INIT_OTP);

  volatile uint32_t parameters[2] = {
    parity,
    sizeof(mcuSettingsFlags)
    + sizeof(reservedSettings)
  };
  SE_DataTransfer_t parametersData = SE_DATATRANSFER_DEFAULT(&parameters, 8);
  SE_addDataInput(&command, &parametersData);

  SE_DataTransfer_t mcuSettingsFlagsData = SE_DATATRANSFER_DEFAULT(&mcuSettingsFlags, sizeof(mcuSettingsFlags));
  SE_addDataInput(&command, &mcuSettingsFlagsData);

  SE_DataTransfer_t reservedSettingsData = SE_DATATRANSFER_DEFAULT(&reservedSettings, sizeof(reservedSettings));
  SE_addDataInput(&command, &reservedSettingsData);

  SE_executeCommand(&command);
  res = SE_readCommandResponse();

  return res;
}

/***************************************************************************//**
 * @brief
 *   Returns the current debug lock configuration.
 * @param[out] status
 *   The command returns a @ref DebugStatus_t with the current status of the
 *   debug configuration.
 * @return
 *   One of the SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully.
 * @retval SE_RESPONSE_INTERNAL_ERROR if there are configuration errors.
 ******************************************************************************/
SE_Response_t SE_debugLockStatus(SE_DebugStatus_t *status)
{
  SE_Response_t res;

  // SE command structures
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_DBG_LOCK_STATUS);

  volatile uint32_t status_word = 0;
  SE_DataTransfer_t statusData = SE_DATATRANSFER_DEFAULT((void*)&status_word, 4);
  SE_addDataOutput(&command, &statusData);

  SE_executeCommand(&command);
  res = SE_readCommandResponse();

  status->debugLockEnabled = (status_word & (1 << 0));
  status->deviceEraseEnabled = (status_word & (1 << 1));
  status->secureDebugEnabled = (status_word & (1 << 2));

  return res;
}

/***************************************************************************//**
 * @brief
 *   Enables the debug lock for the part.
 * @details
 *   The debug port will be closed and the only way to open it is through
 *   device erase (if enabled) or temporarily through secure debug unlock (if
 *   enabled).
 *
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully.
 * @retval SE_RESPONSE_INTERNAL_ERROR there was a problem locking the debug port.
 ******************************************************************************/
SE_Response_t SE_debugLockApply(void)
{
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_DBG_LOCK_APPLY);
  SE_executeCommand(&command);

  return SE_readCommandResponse();
}

/***************************************************************************//**
 * @brief
 *   Enables the secure debug functionality.
 * @details
 *   Enables the secure debug functionality. This functionality makes it
 *   possible to open a locked debug port by signing a cryptographic challenge
 *   and using the debug command interface (DCI).
 *
 *   This command can only be executed before the debug port is locked, and
 *   after a secure debug public key has been installed in the SE using
 *   @ref SE_initPubkey() or the corresponding DCI command.
 *
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully.
 * @retval SE_RESPONSE_INVALID_COMMAND if debug port is locked.
 * @retval SE_RESPONSE_INVALID_PARAMETER if secure debug certificates are
 *                                       missing.
 * @retval SE_RESPONSE_INTERNAL_ERROR if there was a problem during execution.
 ******************************************************************************/
SE_Response_t SE_debugSecureEnable(void)
{
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_DBG_LOCK_ENABLE_SECURE);
  SE_executeCommand(&command);

  return SE_readCommandResponse();
}

/***************************************************************************//**
 * @brief
 *   Disables the secure debug functionality.
 * @details
 *   Disables the secure debug functionality that can be used to open a
 *   locked debug port.
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully.
 * @retval SE_RESPONSE_INTERNAL_ERROR if there was a problem during execution.
 ******************************************************************************/
SE_Response_t SE_debugSecureDisable(void)
{
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_DBG_LOCK_DISABLE_SECURE);
  SE_executeCommand(&command);

  return SE_readCommandResponse();
}

/***************************************************************************//**
 * @brief
 *   Performs a device mass erase and debug unlock.
 *
 * @details
 *   Performs a device mass erase and resets the debug configuration to its
 *   initial unlocked state. Only available before @ref SE_deviceEraseDisable or
 *   the corresponding DCI command has been executed.
 *
 * @note
 *   This command clears and verifies the complete flash and ram of the
 *   system, excluding the user data pages and one-time programmable
 *   commissioning information in the secure element.
 *
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully.
 * @retval SE_RESPONSE_INVALID_COMMAND if device erase is disabled.
 * @retval SE_RESPONSE_INTERNAL_ERROR if there was a problem during execution.
 ******************************************************************************/
SE_Response_t SE_deviceErase(void)
{
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_DEVICE_ERASE);
  SE_executeCommand(&command);

  return SE_readCommandResponse();
}

/***************************************************************************//**
 * @brief
 *   Disabled device erase functionality.
 *
 * @details
 *   This command disables the device erase command. It does not lock the
 *   debug interface to the part, but it is a permanent action for the part.
 *   If device erase is disabled and the device is debug locked, there is no
 *   way to permanently unlock the part. If secure debug unlock is enabled,
 *   secure debug unlock can still be used to temporarily open the debug port.
 *
 * @warning
 *   This command permanently disables the device erase functionality!
 *
 * @return
 *   One of the @ref SE_RESPONSE return codes.
 * @retval SE_RESPONSE_OK when the command was executed successfully.
 * @retval SE_RESPONSE_INTERNAL_ERROR if there was a problem during execution.
 ******************************************************************************/
SE_Response_t SE_deviceEraseDisable(void)
{
  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_DEVICE_ERASE_DISABLE);
  SE_executeCommand(&command);

  return SE_readCommandResponse();
}

#endif // #if defined(SEMAILBOX_PRESENT)

/** @} (end addtogroup SE) */
/** @} (end addtogroup emlib) */

#endif /* defined(SEMAILBOX_PRESENT) || defined(CRYPTOACC_PRESENT) */
