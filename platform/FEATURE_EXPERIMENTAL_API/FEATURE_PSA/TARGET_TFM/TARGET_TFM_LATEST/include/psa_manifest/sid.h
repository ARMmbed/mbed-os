/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __PSA_MANIFEST_SID_H__
#define __PSA_MANIFEST_SID_H__

#ifdef __cplusplus
extern "C" {
#endif

/******** TFM_SP_PS ********/
#define TFM_PS_SET_SID                                             (0x00000060U)
#define TFM_PS_SET_VERSION                                         (1U)
#define TFM_PS_GET_SID                                             (0x00000061U)
#define TFM_PS_GET_VERSION                                         (1U)
#define TFM_PS_GET_INFO_SID                                        (0x00000062U)
#define TFM_PS_GET_INFO_VERSION                                    (1U)
#define TFM_PS_REMOVE_SID                                          (0x00000063U)
#define TFM_PS_REMOVE_VERSION                                      (1U)
#define TFM_PS_GET_SUPPORT_SID                                     (0x00000064U)
#define TFM_PS_GET_SUPPORT_VERSION                                 (1U)

/******** TFM_SP_ITS ********/
#define TFM_ITS_SET_SID                                            (0x00000070U)
#define TFM_ITS_SET_VERSION                                        (1U)
#define TFM_ITS_GET_SID                                            (0x00000071U)
#define TFM_ITS_GET_VERSION                                        (1U)
#define TFM_ITS_GET_INFO_SID                                       (0x00000072U)
#define TFM_ITS_GET_INFO_VERSION                                   (1U)
#define TFM_ITS_REMOVE_SID                                         (0x00000073U)
#define TFM_ITS_REMOVE_VERSION                                     (1U)

/******** TFM_SP_CRYPTO ********/
#define TFM_CRYPTO_SID                                             (0x00000080U)
#define TFM_CRYPTO_VERSION                                         (1U)

/******** TFM_SP_PLATFORM ********/
#define TFM_SP_PLATFORM_SYSTEM_RESET_SID                           (0x00000040U)
#define TFM_SP_PLATFORM_SYSTEM_RESET_VERSION                       (1U)
#define TFM_SP_PLATFORM_IOCTL_SID                                  (0x00000041U)
#define TFM_SP_PLATFORM_IOCTL_VERSION                              (1U)
#define TFM_SP_PLATFORM_NV_COUNTER_SID                             (0x00000042U)
#define TFM_SP_PLATFORM_NV_COUNTER_VERSION                         (1U)

/******** TFM_SP_INITIAL_ATTESTATION ********/
#define TFM_ATTEST_GET_TOKEN_SID                                   (0x00000020U)
#define TFM_ATTEST_GET_TOKEN_VERSION                               (1U)
#define TFM_ATTEST_GET_TOKEN_SIZE_SID                              (0x00000021U)
#define TFM_ATTEST_GET_TOKEN_SIZE_VERSION                          (1U)
#define TFM_ATTEST_GET_PUBLIC_KEY_SID                              (0x00000022U)
#define TFM_ATTEST_GET_PUBLIC_KEY_VERSION                          (1U)

/******** TFM_SP_CORE_TEST ********/
#define SPM_CORE_TEST_INIT_SUCCESS_SID                             (0x0000F020U)
#define SPM_CORE_TEST_INIT_SUCCESS_VERSION                         (1U)
#define SPM_CORE_TEST_DIRECT_RECURSION_SID                         (0x0000F021U)
#define SPM_CORE_TEST_DIRECT_RECURSION_VERSION                     (1U)
#define SPM_CORE_TEST_SS_TO_SS_SID                                 (0x0000F024U)
#define SPM_CORE_TEST_SS_TO_SS_VERSION                             (1U)
#define SPM_CORE_TEST_SS_TO_SS_BUFFER_SID                          (0x0000F025U)
#define SPM_CORE_TEST_SS_TO_SS_BUFFER_VERSION                      (1U)
#define SPM_CORE_TEST_OUTVEC_WRITE_SID                             (0x0000F026U)
#define SPM_CORE_TEST_OUTVEC_WRITE_VERSION                         (1U)
#define SPM_CORE_TEST_PERIPHERAL_ACCESS_SID                        (0x0000F027U)
#define SPM_CORE_TEST_PERIPHERAL_ACCESS_VERSION                    (1U)
#define SPM_CORE_TEST_GET_CALLER_CLIENT_ID_SID                     (0x0000F028U)
#define SPM_CORE_TEST_GET_CALLER_CLIENT_ID_VERSION                 (1U)
#define SPM_CORE_TEST_SPM_REQUEST_SID                              (0x0000F029U)
#define SPM_CORE_TEST_SPM_REQUEST_VERSION                          (1U)
#define SPM_CORE_TEST_BLOCK_SID                                    (0x0000F02AU)
#define SPM_CORE_TEST_BLOCK_VERSION                                (1U)
#define SPM_CORE_TEST_NS_THREAD_SID                                (0x0000F02BU)
#define SPM_CORE_TEST_NS_THREAD_VERSION                            (1U)

/******** TFM_SP_CORE_TEST_2 ********/
#define SPM_CORE_TEST_2_SLAVE_SERVICE_SID                          (0x0000F040U)
#define SPM_CORE_TEST_2_SLAVE_SERVICE_VERSION                      (1U)
#define SPM_CORE_TEST_2_CHECK_CALLER_CLIENT_ID_SID                 (0x0000F041U)
#define SPM_CORE_TEST_2_CHECK_CALLER_CLIENT_ID_VERSION             (1U)
#define SPM_CORE_TEST_2_GET_EVERY_SECOND_BYTE_SID                  (0x0000F042U)
#define SPM_CORE_TEST_2_GET_EVERY_SECOND_BYTE_VERSION              (1U)
#define SPM_CORE_TEST_2_INVERT_SID                                 (0x0000F043U)
#define SPM_CORE_TEST_2_INVERT_VERSION                             (1U)
#define SPM_CORE_TEST_2_PREPARE_TEST_SCENARIO_SID                  (0x0000F044U)
#define SPM_CORE_TEST_2_PREPARE_TEST_SCENARIO_VERSION              (1U)
#define SPM_CORE_TEST_2_EXECUTE_TEST_SCENARIO_SID                  (0x0000F045U)
#define SPM_CORE_TEST_2_EXECUTE_TEST_SCENARIO_VERSION              (1U)

/******** TFM_SP_SECURE_TEST_PARTITION ********/
#define TFM_SECURE_CLIENT_SFN_RUN_TESTS_SID                        (0x0000F000U)
#define TFM_SECURE_CLIENT_SFN_RUN_TESTS_VERSION                    (1U)

/******** TFM_SP_IPC_SERVICE_TEST ********/
#define IPC_SERVICE_TEST_BASIC_SID                                 (0x0000F080U)
#define IPC_SERVICE_TEST_BASIC_VERSION                             (1U)
#define IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SID                    (0x0000F081U)
#define IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_VERSION                (1U)
#define IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID          (0x0000F082U)
#define IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_VERSION      (1U)
#define IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SID                    (0x0000F083U)
#define IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_VERSION                (1U)
#define IPC_SERVICE_TEST_CLIENT_PROGRAMMER_ERROR_SID               (0x0000F084U)
#define IPC_SERVICE_TEST_CLIENT_PROGRAMMER_ERROR_VERSION           (1U)

/******** TFM_SP_IPC_CLIENT_TEST ********/
#define IPC_CLIENT_TEST_BASIC_SID                                  (0x0000F060U)
#define IPC_CLIENT_TEST_BASIC_VERSION                              (1U)
#define IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SID                     (0x0000F061U)
#define IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_VERSION                 (1U)
#define IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID           (0x0000F062U)
#define IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_VERSION       (1U)
#define IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_SID                     (0x0000F063U)
#define IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_VERSION                 (1U)
#define IPC_CLIENT_TEST_MEM_CHECK_SID                              (0x0000F064U)
#define IPC_CLIENT_TEST_MEM_CHECK_VERSION                          (1U)
#define IPC_CLIENT_TEST_RETRIEVE_APP_MEM_SID                       (0x0000F065U)
#define IPC_CLIENT_TEST_RETRIEVE_APP_MEM_VERSION                   (1U)

/******** TFM_IRQ_TEST_1 ********/
#define SPM_CORE_IRQ_TEST_1_PREPARE_TEST_SCENARIO_SID              (0x0000F0A0U)
#define SPM_CORE_IRQ_TEST_1_PREPARE_TEST_SCENARIO_VERSION          (1U)
#define SPM_CORE_IRQ_TEST_1_EXECUTE_TEST_SCENARIO_SID              (0x0000F0A1U)
#define SPM_CORE_IRQ_TEST_1_EXECUTE_TEST_SCENARIO_VERSION          (1U)

/******** TFM_SP_PS_TEST ********/
#define TFM_PS_TEST_PREPARE_SID                                    (0x0000F0C0U)
#define TFM_PS_TEST_PREPARE_VERSION                                (1U)

/******** TFM_SP_SECURE_CLIENT_2 ********/
#define TFM_SECURE_CLIENT_2_SID                                    (0x0000F0E0U)
#define TFM_SECURE_CLIENT_2_VERSION                                (1U)

/******** TFM_SP_PSA_PROXY ********/
#define TFM_CRYPTO_SID                                             (0x00000080U)
#define TFM_CRYPTO_VERSION                                         (1U)
#define TFM_ATTEST_GET_TOKEN_SID                                   (0x00000020U)
#define TFM_ATTEST_GET_TOKEN_VERSION                               (1U)
#define TFM_ATTEST_GET_TOKEN_SIZE_SID                              (0x00000021U)
#define TFM_ATTEST_GET_TOKEN_SIZE_VERSION                          (1U)
#define TFM_ATTEST_GET_PUBLIC_KEY_SID                              (0x00000022U)
#define TFM_ATTEST_GET_PUBLIC_KEY_VERSION                          (1U)
#define TFM_ITS_SET_SID                                            (0x00000070U)
#define TFM_ITS_SET_VERSION                                        (1U)
#define TFM_ITS_GET_SID                                            (0x00000071U)
#define TFM_ITS_GET_VERSION                                        (1U)
#define TFM_ITS_GET_INFO_SID                                       (0x00000072U)
#define TFM_ITS_GET_INFO_VERSION                                   (1U)
#define TFM_ITS_REMOVE_SID                                         (0x00000073U)
#define TFM_ITS_REMOVE_VERSION                                     (1U)
#define TFM_SP_PLATFORM_SYSTEM_RESET_SID                           (0x00000040U)
#define TFM_SP_PLATFORM_SYSTEM_RESET_VERSION                       (1U)
#define TFM_SP_PLATFORM_IOCTL_SID                                  (0x00000041U)
#define TFM_SP_PLATFORM_IOCTL_VERSION                              (1U)
#define TFM_SP_PLATFORM_NV_COUNTER_SID                             (0x00000042U)
#define TFM_SP_PLATFORM_NV_COUNTER_VERSION                         (1U)
#define TFM_PS_SET_SID                                             (0x00000060U)
#define TFM_PS_SET_VERSION                                         (1U)
#define TFM_PS_GET_SID                                             (0x00000061U)
#define TFM_PS_GET_VERSION                                         (1U)
#define TFM_PS_GET_INFO_SID                                        (0x00000062U)
#define TFM_PS_GET_INFO_VERSION                                    (1U)
#define TFM_PS_REMOVE_SID                                          (0x00000063U)
#define TFM_PS_REMOVE_VERSION                                      (1U)
#define TFM_PS_GET_SUPPORT_SID                                     (0x00000064U)
#define TFM_PS_GET_SUPPORT_VERSION                                 (1U)

/******** TFM_SP_FWU ********/
#define TFM_FWU_WRITE_SID                                          (0x000000A0U)
#define TFM_FWU_WRITE_VERSION                                      (1U)
#define TFM_FWU_INSTALL_SID                                        (0x000000A1U)
#define TFM_FWU_INSTALL_VERSION                                    (1U)
#define TFM_FWU_ABORT_SID                                          (0x000000A2U)
#define TFM_FWU_ABORT_VERSION                                      (1U)
#define TFM_FWU_QUERY_SID                                          (0x000000A3U)
#define TFM_FWU_QUERY_VERSION                                      (1U)
#define TFM_FWU_REQUEST_REBOOT_SID                                 (0x000000A4U)
#define TFM_FWU_REQUEST_REBOOT_VERSION                             (1U)
#define TFM_FWU_ACCEPT_SID                                         (0x000000A5U)
#define TFM_FWU_ACCEPT_VERSION                                     (1U)

/******** TFM_SP_FFM11 ********/
#define TFM_FFM11_SERVICE1_SID                                     (0x0000F120U)
#define TFM_FFM11_SERVICE1_VERSION                                 (1U)
#define TFM_FFM11_SERVICE1_HANDLE                                  (0x40000104U)
#define TFM_FFM11_SERVICE2_SID                                     (0x0000F121U)
#define TFM_FFM11_SERVICE2_VERSION                                 (1U)
#define TFM_FFM11_SERVICE2_HANDLE                                  (0x40000101U)
#define TFM_FFM11_SERVICE3_SID                                     (0x0000F122U)
#define TFM_FFM11_SERVICE3_VERSION                                 (1U)
#define TFM_FFM11_SERVICE3_HANDLE                                  (0x40000103U)

#ifdef __cplusplus
}
#endif

#endif /* __PSA_MANIFEST_SID_H__ */
