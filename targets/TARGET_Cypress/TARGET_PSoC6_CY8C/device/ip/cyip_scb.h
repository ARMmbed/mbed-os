/***************************************************************************//**
* \file cyip_scb.h
*
* \brief
* SCB IP definitions
*
* \note
* Generated 2/9/2017 by CyDeviceHeaderGenerator v1.1.0.47
* from the register map configuration rev#961378
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CYIP_SCB_H_
#define _CYIP_SCB_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                            SCB                                            ================ */
/* =========================================================================================================================== */

#define SCB_SECTION_SIZE            0x00010000UL

/**
  * \brief Serial Communications Block (SPI/UART/I2C) (CySCB)
  */
typedef struct {                                /*!< CySCB Structure                                                           */
  __IOM uint32_t CTRL;                          /*!< 0x00000000 Generic control                                                */
   __IM uint32_t STATUS;                        /*!< 0x00000004 Generic status                                                 */
  __IOM uint32_t CMD_RESP_CTRL;                 /*!< 0x00000008 Command/response control                                       */
   __IM uint32_t CMD_RESP_STATUS;               /*!< 0x0000000C Command/response status                                        */
   __IM uint32_t RESERVED[4];
  __IOM uint32_t SPI_CTRL;                      /*!< 0x00000020 SPI control                                                    */
   __IM uint32_t SPI_STATUS;                    /*!< 0x00000024 SPI status                                                     */
   __IM uint32_t RESERVED1[6];
  __IOM uint32_t UART_CTRL;                     /*!< 0x00000040 UART control                                                   */
  __IOM uint32_t UART_TX_CTRL;                  /*!< 0x00000044 UART transmitter control                                       */
  __IOM uint32_t UART_RX_CTRL;                  /*!< 0x00000048 UART receiver control                                          */
   __IM uint32_t UART_RX_STATUS;                /*!< 0x0000004C UART receiver status                                           */
  __IOM uint32_t UART_FLOW_CTRL;                /*!< 0x00000050 UART flow control                                              */
   __IM uint32_t RESERVED2[3];
  __IOM uint32_t I2C_CTRL;                      /*!< 0x00000060 I2C control                                                    */
   __IM uint32_t I2C_STATUS;                    /*!< 0x00000064 I2C status                                                     */
  __IOM uint32_t I2C_M_CMD;                     /*!< 0x00000068 I2C master command                                             */
  __IOM uint32_t I2C_S_CMD;                     /*!< 0x0000006C I2C slave command                                              */
  __IOM uint32_t I2C_CFG;                       /*!< 0x00000070 I2C configuration                                              */
   __IM uint32_t RESERVED3[35];
  __IOM uint32_t DDFT_CTRL;                     /*!< 0x00000100 Digital DfT control                                            */
   __IM uint32_t RESERVED4[63];
  __IOM uint32_t TX_CTRL;                       /*!< 0x00000200 Transmitter control                                            */
  __IOM uint32_t TX_FIFO_CTRL;                  /*!< 0x00000204 Transmitter FIFO control                                       */
   __IM uint32_t TX_FIFO_STATUS;                /*!< 0x00000208 Transmitter FIFO status                                        */
   __IM uint32_t RESERVED5[13];
   __OM uint32_t TX_FIFO_WR;                    /*!< 0x00000240 Transmitter FIFO write                                         */
   __IM uint32_t RESERVED6[47];
  __IOM uint32_t RX_CTRL;                       /*!< 0x00000300 Receiver control                                               */
  __IOM uint32_t RX_FIFO_CTRL;                  /*!< 0x00000304 Receiver FIFO control                                          */
   __IM uint32_t RX_FIFO_STATUS;                /*!< 0x00000308 Receiver FIFO status                                           */
   __IM uint32_t RESERVED7;
  __IOM uint32_t RX_MATCH;                      /*!< 0x00000310 Slave address and mask                                         */
   __IM uint32_t RESERVED8[11];
   __IM uint32_t RX_FIFO_RD;                    /*!< 0x00000340 Receiver FIFO read                                             */
   __IM uint32_t RX_FIFO_RD_SILENT;             /*!< 0x00000344 Receiver FIFO read silent                                      */
   __IM uint32_t RESERVED9[46];
  __IOM uint32_t EZ_DATA[512];                  /*!< 0x00000400 Memory buffer                                                  */
   __IM uint32_t RESERVED10[128];
   __IM uint32_t INTR_CAUSE;                    /*!< 0x00000E00 Active clocked interrupt signal                                */
   __IM uint32_t RESERVED11[31];
  __IOM uint32_t INTR_I2C_EC;                   /*!< 0x00000E80 Externally clocked I2C interrupt request                       */
   __IM uint32_t RESERVED12;
  __IOM uint32_t INTR_I2C_EC_MASK;              /*!< 0x00000E88 Externally clocked I2C interrupt mask                          */
   __IM uint32_t INTR_I2C_EC_MASKED;            /*!< 0x00000E8C Externally clocked I2C interrupt masked                        */
   __IM uint32_t RESERVED13[12];
  __IOM uint32_t INTR_SPI_EC;                   /*!< 0x00000EC0 Externally clocked SPI interrupt request                       */
   __IM uint32_t RESERVED14;
  __IOM uint32_t INTR_SPI_EC_MASK;              /*!< 0x00000EC8 Externally clocked SPI interrupt mask                          */
   __IM uint32_t INTR_SPI_EC_MASKED;            /*!< 0x00000ECC Externally clocked SPI interrupt masked                        */
   __IM uint32_t RESERVED15[12];
  __IOM uint32_t INTR_M;                        /*!< 0x00000F00 Master interrupt request                                       */
  __IOM uint32_t INTR_M_SET;                    /*!< 0x00000F04 Master interrupt set request                                   */
  __IOM uint32_t INTR_M_MASK;                   /*!< 0x00000F08 Master interrupt mask                                          */
   __IM uint32_t INTR_M_MASKED;                 /*!< 0x00000F0C Master interrupt masked request                                */
   __IM uint32_t RESERVED16[12];
  __IOM uint32_t INTR_S;                        /*!< 0x00000F40 Slave interrupt request                                        */
  __IOM uint32_t INTR_S_SET;                    /*!< 0x00000F44 Slave interrupt set request                                    */
  __IOM uint32_t INTR_S_MASK;                   /*!< 0x00000F48 Slave interrupt mask                                           */
   __IM uint32_t INTR_S_MASKED;                 /*!< 0x00000F4C Slave interrupt masked request                                 */
   __IM uint32_t RESERVED17[12];
  __IOM uint32_t INTR_TX;                       /*!< 0x00000F80 Transmitter interrupt request                                  */
  __IOM uint32_t INTR_TX_SET;                   /*!< 0x00000F84 Transmitter interrupt set request                              */
  __IOM uint32_t INTR_TX_MASK;                  /*!< 0x00000F88 Transmitter interrupt mask                                     */
   __IM uint32_t INTR_TX_MASKED;                /*!< 0x00000F8C Transmitter interrupt masked request                           */
   __IM uint32_t RESERVED18[12];
  __IOM uint32_t INTR_RX;                       /*!< 0x00000FC0 Receiver interrupt request                                     */
  __IOM uint32_t INTR_RX_SET;                   /*!< 0x00000FC4 Receiver interrupt set request                                 */
  __IOM uint32_t INTR_RX_MASK;                  /*!< 0x00000FC8 Receiver interrupt mask                                        */
   __IM uint32_t INTR_RX_MASKED;                /*!< 0x00000FCC Receiver interrupt masked request                              */
} CySCB_Type;                                   /*!< Size = 4048 (0xFD0)                                                       */


/* =======================================================  SCB.CTRL  ======================================================== */
#define SCB_CTRL_OVS_Pos                  (0UL)                 /*!< SCB CTRL: OVS (Bit 0)                                     */
#define SCB_CTRL_OVS_Msk                  (0xfUL)               /*!< SCB CTRL: OVS (Bitfield-Mask: 0x0f)                       */
#define SCB_CTRL_EC_AM_MODE_Pos           (8UL)                 /*!< SCB CTRL: EC_AM_MODE (Bit 8)                              */
#define SCB_CTRL_EC_AM_MODE_Msk           (0x100UL)             /*!< SCB CTRL: EC_AM_MODE (Bitfield-Mask: 0x01)                */
#define SCB_CTRL_EC_OP_MODE_Pos           (9UL)                 /*!< SCB CTRL: EC_OP_MODE (Bit 9)                              */
#define SCB_CTRL_EC_OP_MODE_Msk           (0x200UL)             /*!< SCB CTRL: EC_OP_MODE (Bitfield-Mask: 0x01)                */
#define SCB_CTRL_EZ_MODE_Pos              (10UL)                /*!< SCB CTRL: EZ_MODE (Bit 10)                                */
#define SCB_CTRL_EZ_MODE_Msk              (0x400UL)             /*!< SCB CTRL: EZ_MODE (Bitfield-Mask: 0x01)                   */
#define SCB_CTRL_BYTE_MODE_Pos            (11UL)                /*!< SCB CTRL: BYTE_MODE (Bit 11)                              */
#define SCB_CTRL_BYTE_MODE_Msk            (0x800UL)             /*!< SCB CTRL: BYTE_MODE (Bitfield-Mask: 0x01)                 */
#define SCB_CTRL_CMD_RESP_MODE_Pos        (12UL)                /*!< SCB CTRL: CMD_RESP_MODE (Bit 12)                          */
#define SCB_CTRL_CMD_RESP_MODE_Msk        (0x1000UL)            /*!< SCB CTRL: CMD_RESP_MODE (Bitfield-Mask: 0x01)             */
#define SCB_CTRL_ADDR_ACCEPT_Pos          (16UL)                /*!< SCB CTRL: ADDR_ACCEPT (Bit 16)                            */
#define SCB_CTRL_ADDR_ACCEPT_Msk          (0x10000UL)           /*!< SCB CTRL: ADDR_ACCEPT (Bitfield-Mask: 0x01)               */
#define SCB_CTRL_BLOCK_Pos                (17UL)                /*!< SCB CTRL: BLOCK (Bit 17)                                  */
#define SCB_CTRL_BLOCK_Msk                (0x20000UL)           /*!< SCB CTRL: BLOCK (Bitfield-Mask: 0x01)                     */
#define SCB_CTRL_MODE_Pos                 (24UL)                /*!< SCB CTRL: MODE (Bit 24)                                   */
#define SCB_CTRL_MODE_Msk                 (0x3000000UL)         /*!< SCB CTRL: MODE (Bitfield-Mask: 0x03)                      */
#define SCB_CTRL_ENABLED_Pos              (31UL)                /*!< SCB CTRL: ENABLED (Bit 31)                                */
#define SCB_CTRL_ENABLED_Msk              (0x80000000UL)        /*!< SCB CTRL: ENABLED (Bitfield-Mask: 0x01)                   */
/* ======================================================  SCB.STATUS  ======================================================= */
#define SCB_STATUS_EC_BUSY_Pos            (0UL)                 /*!< SCB STATUS: EC_BUSY (Bit 0)                               */
#define SCB_STATUS_EC_BUSY_Msk            (0x1UL)               /*!< SCB STATUS: EC_BUSY (Bitfield-Mask: 0x01)                 */
/* ===================================================  SCB.CMD_RESP_CTRL  =================================================== */
#define SCB_CMD_RESP_CTRL_BASE_RD_ADDR_Pos (0UL)                /*!< SCB CMD_RESP_CTRL: BASE_RD_ADDR (Bit 0)                   */
#define SCB_CMD_RESP_CTRL_BASE_RD_ADDR_Msk (0x1ffUL)            /*!< SCB CMD_RESP_CTRL: BASE_RD_ADDR (Bitfield-Mask: 0x1ff)    */
#define SCB_CMD_RESP_CTRL_BASE_WR_ADDR_Pos (16UL)               /*!< SCB CMD_RESP_CTRL: BASE_WR_ADDR (Bit 16)                  */
#define SCB_CMD_RESP_CTRL_BASE_WR_ADDR_Msk (0x1ff0000UL)        /*!< SCB CMD_RESP_CTRL: BASE_WR_ADDR (Bitfield-Mask: 0x1ff)    */
/* ==================================================  SCB.CMD_RESP_STATUS  ================================================== */
#define SCB_CMD_RESP_STATUS_CURR_RD_ADDR_Pos (0UL)              /*!< SCB CMD_RESP_STATUS: CURR_RD_ADDR (Bit 0)                 */
#define SCB_CMD_RESP_STATUS_CURR_RD_ADDR_Msk (0x1ffUL)          /*!< SCB CMD_RESP_STATUS: CURR_RD_ADDR (Bitfield-Mask: 0x1ff)  */
#define SCB_CMD_RESP_STATUS_CURR_WR_ADDR_Pos (16UL)             /*!< SCB CMD_RESP_STATUS: CURR_WR_ADDR (Bit 16)                */
#define SCB_CMD_RESP_STATUS_CURR_WR_ADDR_Msk (0x1ff0000UL)      /*!< SCB CMD_RESP_STATUS: CURR_WR_ADDR (Bitfield-Mask: 0x1ff)  */
#define SCB_CMD_RESP_STATUS_CMD_RESP_EC_BUS_BUSY_Pos (30UL)     /*!< SCB CMD_RESP_STATUS: CMD_RESP_EC_BUS_BUSY (Bit 30)        */
#define SCB_CMD_RESP_STATUS_CMD_RESP_EC_BUS_BUSY_Msk (0x40000000UL) /*!< SCB CMD_RESP_STATUS: CMD_RESP_EC_BUS_BUSY (Bitfield-Mask: 0x01)*/
#define SCB_CMD_RESP_STATUS_CMD_RESP_EC_BUSY_Pos (31UL)         /*!< SCB CMD_RESP_STATUS: CMD_RESP_EC_BUSY (Bit 31)            */
#define SCB_CMD_RESP_STATUS_CMD_RESP_EC_BUSY_Msk (0x80000000UL) /*!< SCB CMD_RESP_STATUS: CMD_RESP_EC_BUSY (Bitfield-Mask: 0x01)*/
/* =====================================================  SCB.SPI_CTRL  ====================================================== */
#define SCB_SPI_CTRL_SSEL_CONTINUOUS_Pos  (0UL)                 /*!< SCB SPI_CTRL: SSEL_CONTINUOUS (Bit 0)                     */
#define SCB_SPI_CTRL_SSEL_CONTINUOUS_Msk  (0x1UL)               /*!< SCB SPI_CTRL: SSEL_CONTINUOUS (Bitfield-Mask: 0x01)       */
#define SCB_SPI_CTRL_SELECT_PRECEDE_Pos   (1UL)                 /*!< SCB SPI_CTRL: SELECT_PRECEDE (Bit 1)                      */
#define SCB_SPI_CTRL_SELECT_PRECEDE_Msk   (0x2UL)               /*!< SCB SPI_CTRL: SELECT_PRECEDE (Bitfield-Mask: 0x01)        */
#define SCB_SPI_CTRL_CPHA_Pos             (2UL)                 /*!< SCB SPI_CTRL: CPHA (Bit 2)                                */
#define SCB_SPI_CTRL_CPHA_Msk             (0x4UL)               /*!< SCB SPI_CTRL: CPHA (Bitfield-Mask: 0x01)                  */
#define SCB_SPI_CTRL_CPOL_Pos             (3UL)                 /*!< SCB SPI_CTRL: CPOL (Bit 3)                                */
#define SCB_SPI_CTRL_CPOL_Msk             (0x8UL)               /*!< SCB SPI_CTRL: CPOL (Bitfield-Mask: 0x01)                  */
#define SCB_SPI_CTRL_LATE_MISO_SAMPLE_Pos (4UL)                 /*!< SCB SPI_CTRL: LATE_MISO_SAMPLE (Bit 4)                    */
#define SCB_SPI_CTRL_LATE_MISO_SAMPLE_Msk (0x10UL)              /*!< SCB SPI_CTRL: LATE_MISO_SAMPLE (Bitfield-Mask: 0x01)      */
#define SCB_SPI_CTRL_SCLK_CONTINUOUS_Pos  (5UL)                 /*!< SCB SPI_CTRL: SCLK_CONTINUOUS (Bit 5)                     */
#define SCB_SPI_CTRL_SCLK_CONTINUOUS_Msk  (0x20UL)              /*!< SCB SPI_CTRL: SCLK_CONTINUOUS (Bitfield-Mask: 0x01)       */
#define SCB_SPI_CTRL_SSEL_POLARITY0_Pos   (8UL)                 /*!< SCB SPI_CTRL: SSEL_POLARITY0 (Bit 8)                      */
#define SCB_SPI_CTRL_SSEL_POLARITY0_Msk   (0x100UL)             /*!< SCB SPI_CTRL: SSEL_POLARITY0 (Bitfield-Mask: 0x01)        */
#define SCB_SPI_CTRL_SSEL_POLARITY1_Pos   (9UL)                 /*!< SCB SPI_CTRL: SSEL_POLARITY1 (Bit 9)                      */
#define SCB_SPI_CTRL_SSEL_POLARITY1_Msk   (0x200UL)             /*!< SCB SPI_CTRL: SSEL_POLARITY1 (Bitfield-Mask: 0x01)        */
#define SCB_SPI_CTRL_SSEL_POLARITY2_Pos   (10UL)                /*!< SCB SPI_CTRL: SSEL_POLARITY2 (Bit 10)                     */
#define SCB_SPI_CTRL_SSEL_POLARITY2_Msk   (0x400UL)             /*!< SCB SPI_CTRL: SSEL_POLARITY2 (Bitfield-Mask: 0x01)        */
#define SCB_SPI_CTRL_SSEL_POLARITY3_Pos   (11UL)                /*!< SCB SPI_CTRL: SSEL_POLARITY3 (Bit 11)                     */
#define SCB_SPI_CTRL_SSEL_POLARITY3_Msk   (0x800UL)             /*!< SCB SPI_CTRL: SSEL_POLARITY3 (Bitfield-Mask: 0x01)        */
#define SCB_SPI_CTRL_LOOPBACK_Pos         (16UL)                /*!< SCB SPI_CTRL: LOOPBACK (Bit 16)                           */
#define SCB_SPI_CTRL_LOOPBACK_Msk         (0x10000UL)           /*!< SCB SPI_CTRL: LOOPBACK (Bitfield-Mask: 0x01)              */
#define SCB_SPI_CTRL_MODE_Pos             (24UL)                /*!< SCB SPI_CTRL: MODE (Bit 24)                               */
#define SCB_SPI_CTRL_MODE_Msk             (0x3000000UL)         /*!< SCB SPI_CTRL: MODE (Bitfield-Mask: 0x03)                  */
#define SCB_SPI_CTRL_SSEL_Pos             (26UL)                /*!< SCB SPI_CTRL: SSEL (Bit 26)                               */
#define SCB_SPI_CTRL_SSEL_Msk             (0xc000000UL)         /*!< SCB SPI_CTRL: SSEL (Bitfield-Mask: 0x03)                  */
#define SCB_SPI_CTRL_MASTER_MODE_Pos      (31UL)                /*!< SCB SPI_CTRL: MASTER_MODE (Bit 31)                        */
#define SCB_SPI_CTRL_MASTER_MODE_Msk      (0x80000000UL)        /*!< SCB SPI_CTRL: MASTER_MODE (Bitfield-Mask: 0x01)           */
/* ====================================================  SCB.SPI_STATUS  ===================================================== */
#define SCB_SPI_STATUS_BUS_BUSY_Pos       (0UL)                 /*!< SCB SPI_STATUS: BUS_BUSY (Bit 0)                          */
#define SCB_SPI_STATUS_BUS_BUSY_Msk       (0x1UL)               /*!< SCB SPI_STATUS: BUS_BUSY (Bitfield-Mask: 0x01)            */
#define SCB_SPI_STATUS_SPI_EC_BUSY_Pos    (1UL)                 /*!< SCB SPI_STATUS: SPI_EC_BUSY (Bit 1)                       */
#define SCB_SPI_STATUS_SPI_EC_BUSY_Msk    (0x2UL)               /*!< SCB SPI_STATUS: SPI_EC_BUSY (Bitfield-Mask: 0x01)         */
#define SCB_SPI_STATUS_CURR_EZ_ADDR_Pos   (8UL)                 /*!< SCB SPI_STATUS: CURR_EZ_ADDR (Bit 8)                      */
#define SCB_SPI_STATUS_CURR_EZ_ADDR_Msk   (0xff00UL)            /*!< SCB SPI_STATUS: CURR_EZ_ADDR (Bitfield-Mask: 0xff)        */
#define SCB_SPI_STATUS_BASE_EZ_ADDR_Pos   (16UL)                /*!< SCB SPI_STATUS: BASE_EZ_ADDR (Bit 16)                     */
#define SCB_SPI_STATUS_BASE_EZ_ADDR_Msk   (0xff0000UL)          /*!< SCB SPI_STATUS: BASE_EZ_ADDR (Bitfield-Mask: 0xff)        */
/* =====================================================  SCB.UART_CTRL  ===================================================== */
#define SCB_UART_CTRL_LOOPBACK_Pos        (16UL)                /*!< SCB UART_CTRL: LOOPBACK (Bit 16)                          */
#define SCB_UART_CTRL_LOOPBACK_Msk        (0x10000UL)           /*!< SCB UART_CTRL: LOOPBACK (Bitfield-Mask: 0x01)             */
#define SCB_UART_CTRL_MODE_Pos            (24UL)                /*!< SCB UART_CTRL: MODE (Bit 24)                              */
#define SCB_UART_CTRL_MODE_Msk            (0x3000000UL)         /*!< SCB UART_CTRL: MODE (Bitfield-Mask: 0x03)                 */
/* ===================================================  SCB.UART_TX_CTRL  ==================================================== */
#define SCB_UART_TX_CTRL_STOP_BITS_Pos    (0UL)                 /*!< SCB UART_TX_CTRL: STOP_BITS (Bit 0)                       */
#define SCB_UART_TX_CTRL_STOP_BITS_Msk    (0x7UL)               /*!< SCB UART_TX_CTRL: STOP_BITS (Bitfield-Mask: 0x07)         */
#define SCB_UART_TX_CTRL_PARITY_Pos       (4UL)                 /*!< SCB UART_TX_CTRL: PARITY (Bit 4)                          */
#define SCB_UART_TX_CTRL_PARITY_Msk       (0x10UL)              /*!< SCB UART_TX_CTRL: PARITY (Bitfield-Mask: 0x01)            */
#define SCB_UART_TX_CTRL_PARITY_ENABLED_Pos (5UL)               /*!< SCB UART_TX_CTRL: PARITY_ENABLED (Bit 5)                  */
#define SCB_UART_TX_CTRL_PARITY_ENABLED_Msk (0x20UL)            /*!< SCB UART_TX_CTRL: PARITY_ENABLED (Bitfield-Mask: 0x01)    */
#define SCB_UART_TX_CTRL_RETRY_ON_NACK_Pos (8UL)                /*!< SCB UART_TX_CTRL: RETRY_ON_NACK (Bit 8)                   */
#define SCB_UART_TX_CTRL_RETRY_ON_NACK_Msk (0x100UL)            /*!< SCB UART_TX_CTRL: RETRY_ON_NACK (Bitfield-Mask: 0x01)     */
/* ===================================================  SCB.UART_RX_CTRL  ==================================================== */
#define SCB_UART_RX_CTRL_STOP_BITS_Pos    (0UL)                 /*!< SCB UART_RX_CTRL: STOP_BITS (Bit 0)                       */
#define SCB_UART_RX_CTRL_STOP_BITS_Msk    (0x7UL)               /*!< SCB UART_RX_CTRL: STOP_BITS (Bitfield-Mask: 0x07)         */
#define SCB_UART_RX_CTRL_PARITY_Pos       (4UL)                 /*!< SCB UART_RX_CTRL: PARITY (Bit 4)                          */
#define SCB_UART_RX_CTRL_PARITY_Msk       (0x10UL)              /*!< SCB UART_RX_CTRL: PARITY (Bitfield-Mask: 0x01)            */
#define SCB_UART_RX_CTRL_PARITY_ENABLED_Pos (5UL)               /*!< SCB UART_RX_CTRL: PARITY_ENABLED (Bit 5)                  */
#define SCB_UART_RX_CTRL_PARITY_ENABLED_Msk (0x20UL)            /*!< SCB UART_RX_CTRL: PARITY_ENABLED (Bitfield-Mask: 0x01)    */
#define SCB_UART_RX_CTRL_POLARITY_Pos     (6UL)                 /*!< SCB UART_RX_CTRL: POLARITY (Bit 6)                        */
#define SCB_UART_RX_CTRL_POLARITY_Msk     (0x40UL)              /*!< SCB UART_RX_CTRL: POLARITY (Bitfield-Mask: 0x01)          */
#define SCB_UART_RX_CTRL_DROP_ON_PARITY_ERROR_Pos (8UL)         /*!< SCB UART_RX_CTRL: DROP_ON_PARITY_ERROR (Bit 8)            */
#define SCB_UART_RX_CTRL_DROP_ON_PARITY_ERROR_Msk (0x100UL)     /*!< SCB UART_RX_CTRL: DROP_ON_PARITY_ERROR (Bitfield-Mask: 0x01)*/
#define SCB_UART_RX_CTRL_DROP_ON_FRAME_ERROR_Pos (9UL)          /*!< SCB UART_RX_CTRL: DROP_ON_FRAME_ERROR (Bit 9)             */
#define SCB_UART_RX_CTRL_DROP_ON_FRAME_ERROR_Msk (0x200UL)      /*!< SCB UART_RX_CTRL: DROP_ON_FRAME_ERROR (Bitfield-Mask: 0x01)*/
#define SCB_UART_RX_CTRL_MP_MODE_Pos      (10UL)                /*!< SCB UART_RX_CTRL: MP_MODE (Bit 10)                        */
#define SCB_UART_RX_CTRL_MP_MODE_Msk      (0x400UL)             /*!< SCB UART_RX_CTRL: MP_MODE (Bitfield-Mask: 0x01)           */
#define SCB_UART_RX_CTRL_LIN_MODE_Pos     (12UL)                /*!< SCB UART_RX_CTRL: LIN_MODE (Bit 12)                       */
#define SCB_UART_RX_CTRL_LIN_MODE_Msk     (0x1000UL)            /*!< SCB UART_RX_CTRL: LIN_MODE (Bitfield-Mask: 0x01)          */
#define SCB_UART_RX_CTRL_SKIP_START_Pos   (13UL)                /*!< SCB UART_RX_CTRL: SKIP_START (Bit 13)                     */
#define SCB_UART_RX_CTRL_SKIP_START_Msk   (0x2000UL)            /*!< SCB UART_RX_CTRL: SKIP_START (Bitfield-Mask: 0x01)        */
#define SCB_UART_RX_CTRL_BREAK_WIDTH_Pos  (16UL)                /*!< SCB UART_RX_CTRL: BREAK_WIDTH (Bit 16)                    */
#define SCB_UART_RX_CTRL_BREAK_WIDTH_Msk  (0xf0000UL)           /*!< SCB UART_RX_CTRL: BREAK_WIDTH (Bitfield-Mask: 0x0f)       */
/* ==================================================  SCB.UART_RX_STATUS  =================================================== */
#define SCB_UART_RX_STATUS_BR_COUNTER_Pos (0UL)                 /*!< SCB UART_RX_STATUS: BR_COUNTER (Bit 0)                    */
#define SCB_UART_RX_STATUS_BR_COUNTER_Msk (0xfffUL)             /*!< SCB UART_RX_STATUS: BR_COUNTER (Bitfield-Mask: 0xfff)     */
/* ==================================================  SCB.UART_FLOW_CTRL  =================================================== */
#define SCB_UART_FLOW_CTRL_TRIGGER_LEVEL_Pos (0UL)              /*!< SCB UART_FLOW_CTRL: TRIGGER_LEVEL (Bit 0)                 */
#define SCB_UART_FLOW_CTRL_TRIGGER_LEVEL_Msk (0xffUL)           /*!< SCB UART_FLOW_CTRL: TRIGGER_LEVEL (Bitfield-Mask: 0xff)   */
#define SCB_UART_FLOW_CTRL_RTS_POLARITY_Pos (16UL)              /*!< SCB UART_FLOW_CTRL: RTS_POLARITY (Bit 16)                 */
#define SCB_UART_FLOW_CTRL_RTS_POLARITY_Msk (0x10000UL)         /*!< SCB UART_FLOW_CTRL: RTS_POLARITY (Bitfield-Mask: 0x01)    */
#define SCB_UART_FLOW_CTRL_CTS_POLARITY_Pos (24UL)              /*!< SCB UART_FLOW_CTRL: CTS_POLARITY (Bit 24)                 */
#define SCB_UART_FLOW_CTRL_CTS_POLARITY_Msk (0x1000000UL)       /*!< SCB UART_FLOW_CTRL: CTS_POLARITY (Bitfield-Mask: 0x01)    */
#define SCB_UART_FLOW_CTRL_CTS_ENABLED_Pos (25UL)               /*!< SCB UART_FLOW_CTRL: CTS_ENABLED (Bit 25)                  */
#define SCB_UART_FLOW_CTRL_CTS_ENABLED_Msk (0x2000000UL)        /*!< SCB UART_FLOW_CTRL: CTS_ENABLED (Bitfield-Mask: 0x01)     */
/* =====================================================  SCB.I2C_CTRL  ====================================================== */
#define SCB_I2C_CTRL_HIGH_PHASE_OVS_Pos   (0UL)                 /*!< SCB I2C_CTRL: HIGH_PHASE_OVS (Bit 0)                      */
#define SCB_I2C_CTRL_HIGH_PHASE_OVS_Msk   (0xfUL)               /*!< SCB I2C_CTRL: HIGH_PHASE_OVS (Bitfield-Mask: 0x0f)        */
#define SCB_I2C_CTRL_LOW_PHASE_OVS_Pos    (4UL)                 /*!< SCB I2C_CTRL: LOW_PHASE_OVS (Bit 4)                       */
#define SCB_I2C_CTRL_LOW_PHASE_OVS_Msk    (0xf0UL)              /*!< SCB I2C_CTRL: LOW_PHASE_OVS (Bitfield-Mask: 0x0f)         */
#define SCB_I2C_CTRL_M_READY_DATA_ACK_Pos (8UL)                 /*!< SCB I2C_CTRL: M_READY_DATA_ACK (Bit 8)                    */
#define SCB_I2C_CTRL_M_READY_DATA_ACK_Msk (0x100UL)             /*!< SCB I2C_CTRL: M_READY_DATA_ACK (Bitfield-Mask: 0x01)      */
#define SCB_I2C_CTRL_M_NOT_READY_DATA_NACK_Pos (9UL)            /*!< SCB I2C_CTRL: M_NOT_READY_DATA_NACK (Bit 9)               */
#define SCB_I2C_CTRL_M_NOT_READY_DATA_NACK_Msk (0x200UL)        /*!< SCB I2C_CTRL: M_NOT_READY_DATA_NACK (Bitfield-Mask: 0x01) */
#define SCB_I2C_CTRL_S_GENERAL_IGNORE_Pos (11UL)                /*!< SCB I2C_CTRL: S_GENERAL_IGNORE (Bit 11)                   */
#define SCB_I2C_CTRL_S_GENERAL_IGNORE_Msk (0x800UL)             /*!< SCB I2C_CTRL: S_GENERAL_IGNORE (Bitfield-Mask: 0x01)      */
#define SCB_I2C_CTRL_S_READY_ADDR_ACK_Pos (12UL)                /*!< SCB I2C_CTRL: S_READY_ADDR_ACK (Bit 12)                   */
#define SCB_I2C_CTRL_S_READY_ADDR_ACK_Msk (0x1000UL)            /*!< SCB I2C_CTRL: S_READY_ADDR_ACK (Bitfield-Mask: 0x01)      */
#define SCB_I2C_CTRL_S_READY_DATA_ACK_Pos (13UL)                /*!< SCB I2C_CTRL: S_READY_DATA_ACK (Bit 13)                   */
#define SCB_I2C_CTRL_S_READY_DATA_ACK_Msk (0x2000UL)            /*!< SCB I2C_CTRL: S_READY_DATA_ACK (Bitfield-Mask: 0x01)      */
#define SCB_I2C_CTRL_S_NOT_READY_ADDR_NACK_Pos (14UL)           /*!< SCB I2C_CTRL: S_NOT_READY_ADDR_NACK (Bit 14)              */
#define SCB_I2C_CTRL_S_NOT_READY_ADDR_NACK_Msk (0x4000UL)       /*!< SCB I2C_CTRL: S_NOT_READY_ADDR_NACK (Bitfield-Mask: 0x01) */
#define SCB_I2C_CTRL_S_NOT_READY_DATA_NACK_Pos (15UL)           /*!< SCB I2C_CTRL: S_NOT_READY_DATA_NACK (Bit 15)              */
#define SCB_I2C_CTRL_S_NOT_READY_DATA_NACK_Msk (0x8000UL)       /*!< SCB I2C_CTRL: S_NOT_READY_DATA_NACK (Bitfield-Mask: 0x01) */
#define SCB_I2C_CTRL_LOOPBACK_Pos         (16UL)                /*!< SCB I2C_CTRL: LOOPBACK (Bit 16)                           */
#define SCB_I2C_CTRL_LOOPBACK_Msk         (0x10000UL)           /*!< SCB I2C_CTRL: LOOPBACK (Bitfield-Mask: 0x01)              */
#define SCB_I2C_CTRL_SLAVE_MODE_Pos       (30UL)                /*!< SCB I2C_CTRL: SLAVE_MODE (Bit 30)                         */
#define SCB_I2C_CTRL_SLAVE_MODE_Msk       (0x40000000UL)        /*!< SCB I2C_CTRL: SLAVE_MODE (Bitfield-Mask: 0x01)            */
#define SCB_I2C_CTRL_MASTER_MODE_Pos      (31UL)                /*!< SCB I2C_CTRL: MASTER_MODE (Bit 31)                        */
#define SCB_I2C_CTRL_MASTER_MODE_Msk      (0x80000000UL)        /*!< SCB I2C_CTRL: MASTER_MODE (Bitfield-Mask: 0x01)           */
/* ====================================================  SCB.I2C_STATUS  ===================================================== */
#define SCB_I2C_STATUS_BUS_BUSY_Pos       (0UL)                 /*!< SCB I2C_STATUS: BUS_BUSY (Bit 0)                          */
#define SCB_I2C_STATUS_BUS_BUSY_Msk       (0x1UL)               /*!< SCB I2C_STATUS: BUS_BUSY (Bitfield-Mask: 0x01)            */
#define SCB_I2C_STATUS_I2C_EC_BUSY_Pos    (1UL)                 /*!< SCB I2C_STATUS: I2C_EC_BUSY (Bit 1)                       */
#define SCB_I2C_STATUS_I2C_EC_BUSY_Msk    (0x2UL)               /*!< SCB I2C_STATUS: I2C_EC_BUSY (Bitfield-Mask: 0x01)         */
#define SCB_I2C_STATUS_S_READ_Pos         (4UL)                 /*!< SCB I2C_STATUS: S_READ (Bit 4)                            */
#define SCB_I2C_STATUS_S_READ_Msk         (0x10UL)              /*!< SCB I2C_STATUS: S_READ (Bitfield-Mask: 0x01)              */
#define SCB_I2C_STATUS_M_READ_Pos         (5UL)                 /*!< SCB I2C_STATUS: M_READ (Bit 5)                            */
#define SCB_I2C_STATUS_M_READ_Msk         (0x20UL)              /*!< SCB I2C_STATUS: M_READ (Bitfield-Mask: 0x01)              */
#define SCB_I2C_STATUS_CURR_EZ_ADDR_Pos   (8UL)                 /*!< SCB I2C_STATUS: CURR_EZ_ADDR (Bit 8)                      */
#define SCB_I2C_STATUS_CURR_EZ_ADDR_Msk   (0xff00UL)            /*!< SCB I2C_STATUS: CURR_EZ_ADDR (Bitfield-Mask: 0xff)        */
#define SCB_I2C_STATUS_BASE_EZ_ADDR_Pos   (16UL)                /*!< SCB I2C_STATUS: BASE_EZ_ADDR (Bit 16)                     */
#define SCB_I2C_STATUS_BASE_EZ_ADDR_Msk   (0xff0000UL)          /*!< SCB I2C_STATUS: BASE_EZ_ADDR (Bitfield-Mask: 0xff)        */
/* =====================================================  SCB.I2C_M_CMD  ===================================================== */
#define SCB_I2C_M_CMD_M_START_Pos         (0UL)                 /*!< SCB I2C_M_CMD: M_START (Bit 0)                            */
#define SCB_I2C_M_CMD_M_START_Msk         (0x1UL)               /*!< SCB I2C_M_CMD: M_START (Bitfield-Mask: 0x01)              */
#define SCB_I2C_M_CMD_M_START_ON_IDLE_Pos (1UL)                 /*!< SCB I2C_M_CMD: M_START_ON_IDLE (Bit 1)                    */
#define SCB_I2C_M_CMD_M_START_ON_IDLE_Msk (0x2UL)               /*!< SCB I2C_M_CMD: M_START_ON_IDLE (Bitfield-Mask: 0x01)      */
#define SCB_I2C_M_CMD_M_ACK_Pos           (2UL)                 /*!< SCB I2C_M_CMD: M_ACK (Bit 2)                              */
#define SCB_I2C_M_CMD_M_ACK_Msk           (0x4UL)               /*!< SCB I2C_M_CMD: M_ACK (Bitfield-Mask: 0x01)                */
#define SCB_I2C_M_CMD_M_NACK_Pos          (3UL)                 /*!< SCB I2C_M_CMD: M_NACK (Bit 3)                             */
#define SCB_I2C_M_CMD_M_NACK_Msk          (0x8UL)               /*!< SCB I2C_M_CMD: M_NACK (Bitfield-Mask: 0x01)               */
#define SCB_I2C_M_CMD_M_STOP_Pos          (4UL)                 /*!< SCB I2C_M_CMD: M_STOP (Bit 4)                             */
#define SCB_I2C_M_CMD_M_STOP_Msk          (0x10UL)              /*!< SCB I2C_M_CMD: M_STOP (Bitfield-Mask: 0x01)               */
/* =====================================================  SCB.I2C_S_CMD  ===================================================== */
#define SCB_I2C_S_CMD_S_ACK_Pos           (0UL)                 /*!< SCB I2C_S_CMD: S_ACK (Bit 0)                              */
#define SCB_I2C_S_CMD_S_ACK_Msk           (0x1UL)               /*!< SCB I2C_S_CMD: S_ACK (Bitfield-Mask: 0x01)                */
#define SCB_I2C_S_CMD_S_NACK_Pos          (1UL)                 /*!< SCB I2C_S_CMD: S_NACK (Bit 1)                             */
#define SCB_I2C_S_CMD_S_NACK_Msk          (0x2UL)               /*!< SCB I2C_S_CMD: S_NACK (Bitfield-Mask: 0x01)               */
/* ======================================================  SCB.I2C_CFG  ====================================================== */
#define SCB_I2C_CFG_SDA_IN_FILT_TRIM_Pos  (0UL)                 /*!< SCB I2C_CFG: SDA_IN_FILT_TRIM (Bit 0)                     */
#define SCB_I2C_CFG_SDA_IN_FILT_TRIM_Msk  (0x3UL)               /*!< SCB I2C_CFG: SDA_IN_FILT_TRIM (Bitfield-Mask: 0x03)       */
#define SCB_I2C_CFG_SDA_IN_FILT_SEL_Pos   (4UL)                 /*!< SCB I2C_CFG: SDA_IN_FILT_SEL (Bit 4)                      */
#define SCB_I2C_CFG_SDA_IN_FILT_SEL_Msk   (0x10UL)              /*!< SCB I2C_CFG: SDA_IN_FILT_SEL (Bitfield-Mask: 0x01)        */
#define SCB_I2C_CFG_SCL_IN_FILT_TRIM_Pos  (8UL)                 /*!< SCB I2C_CFG: SCL_IN_FILT_TRIM (Bit 8)                     */
#define SCB_I2C_CFG_SCL_IN_FILT_TRIM_Msk  (0x300UL)             /*!< SCB I2C_CFG: SCL_IN_FILT_TRIM (Bitfield-Mask: 0x03)       */
#define SCB_I2C_CFG_SCL_IN_FILT_SEL_Pos   (12UL)                /*!< SCB I2C_CFG: SCL_IN_FILT_SEL (Bit 12)                     */
#define SCB_I2C_CFG_SCL_IN_FILT_SEL_Msk   (0x1000UL)            /*!< SCB I2C_CFG: SCL_IN_FILT_SEL (Bitfield-Mask: 0x01)        */
#define SCB_I2C_CFG_SDA_OUT_FILT0_TRIM_Pos (16UL)               /*!< SCB I2C_CFG: SDA_OUT_FILT0_TRIM (Bit 16)                  */
#define SCB_I2C_CFG_SDA_OUT_FILT0_TRIM_Msk (0x30000UL)          /*!< SCB I2C_CFG: SDA_OUT_FILT0_TRIM (Bitfield-Mask: 0x03)     */
#define SCB_I2C_CFG_SDA_OUT_FILT1_TRIM_Pos (18UL)               /*!< SCB I2C_CFG: SDA_OUT_FILT1_TRIM (Bit 18)                  */
#define SCB_I2C_CFG_SDA_OUT_FILT1_TRIM_Msk (0xc0000UL)          /*!< SCB I2C_CFG: SDA_OUT_FILT1_TRIM (Bitfield-Mask: 0x03)     */
#define SCB_I2C_CFG_SDA_OUT_FILT2_TRIM_Pos (20UL)               /*!< SCB I2C_CFG: SDA_OUT_FILT2_TRIM (Bit 20)                  */
#define SCB_I2C_CFG_SDA_OUT_FILT2_TRIM_Msk (0x300000UL)         /*!< SCB I2C_CFG: SDA_OUT_FILT2_TRIM (Bitfield-Mask: 0x03)     */
#define SCB_I2C_CFG_SDA_OUT_FILT_SEL_Pos  (28UL)                /*!< SCB I2C_CFG: SDA_OUT_FILT_SEL (Bit 28)                    */
#define SCB_I2C_CFG_SDA_OUT_FILT_SEL_Msk  (0x30000000UL)        /*!< SCB I2C_CFG: SDA_OUT_FILT_SEL (Bitfield-Mask: 0x03)       */
/* =====================================================  SCB.DDFT_CTRL  ===================================================== */
#define SCB_DDFT_CTRL_DDFT_IN0_SEL_Pos    (0UL)                 /*!< SCB DDFT_CTRL: DDFT_IN0_SEL (Bit 0)                       */
#define SCB_DDFT_CTRL_DDFT_IN0_SEL_Msk    (0x1UL)               /*!< SCB DDFT_CTRL: DDFT_IN0_SEL (Bitfield-Mask: 0x01)         */
#define SCB_DDFT_CTRL_DDFT_IN1_SEL_Pos    (4UL)                 /*!< SCB DDFT_CTRL: DDFT_IN1_SEL (Bit 4)                       */
#define SCB_DDFT_CTRL_DDFT_IN1_SEL_Msk    (0x10UL)              /*!< SCB DDFT_CTRL: DDFT_IN1_SEL (Bitfield-Mask: 0x01)         */
#define SCB_DDFT_CTRL_DDFT_OUT0_SEL_Pos   (16UL)                /*!< SCB DDFT_CTRL: DDFT_OUT0_SEL (Bit 16)                     */
#define SCB_DDFT_CTRL_DDFT_OUT0_SEL_Msk   (0x70000UL)           /*!< SCB DDFT_CTRL: DDFT_OUT0_SEL (Bitfield-Mask: 0x07)        */
#define SCB_DDFT_CTRL_DDFT_OUT1_SEL_Pos   (20UL)                /*!< SCB DDFT_CTRL: DDFT_OUT1_SEL (Bit 20)                     */
#define SCB_DDFT_CTRL_DDFT_OUT1_SEL_Msk   (0x700000UL)          /*!< SCB DDFT_CTRL: DDFT_OUT1_SEL (Bitfield-Mask: 0x07)        */
/* ======================================================  SCB.TX_CTRL  ====================================================== */
#define SCB_TX_CTRL_DATA_WIDTH_Pos        (0UL)                 /*!< SCB TX_CTRL: DATA_WIDTH (Bit 0)                           */
#define SCB_TX_CTRL_DATA_WIDTH_Msk        (0xfUL)               /*!< SCB TX_CTRL: DATA_WIDTH (Bitfield-Mask: 0x0f)             */
#define SCB_TX_CTRL_MSB_FIRST_Pos         (8UL)                 /*!< SCB TX_CTRL: MSB_FIRST (Bit 8)                            */
#define SCB_TX_CTRL_MSB_FIRST_Msk         (0x100UL)             /*!< SCB TX_CTRL: MSB_FIRST (Bitfield-Mask: 0x01)              */
/* ===================================================  SCB.TX_FIFO_CTRL  ==================================================== */
#define SCB_TX_FIFO_CTRL_TRIGGER_LEVEL_Pos (0UL)                /*!< SCB TX_FIFO_CTRL: TRIGGER_LEVEL (Bit 0)                   */
#define SCB_TX_FIFO_CTRL_TRIGGER_LEVEL_Msk (0xffUL)             /*!< SCB TX_FIFO_CTRL: TRIGGER_LEVEL (Bitfield-Mask: 0xff)     */
#define SCB_TX_FIFO_CTRL_CLEAR_Pos        (16UL)                /*!< SCB TX_FIFO_CTRL: CLEAR (Bit 16)                          */
#define SCB_TX_FIFO_CTRL_CLEAR_Msk        (0x10000UL)           /*!< SCB TX_FIFO_CTRL: CLEAR (Bitfield-Mask: 0x01)             */
#define SCB_TX_FIFO_CTRL_FREEZE_Pos       (17UL)                /*!< SCB TX_FIFO_CTRL: FREEZE (Bit 17)                         */
#define SCB_TX_FIFO_CTRL_FREEZE_Msk       (0x20000UL)           /*!< SCB TX_FIFO_CTRL: FREEZE (Bitfield-Mask: 0x01)            */
/* ==================================================  SCB.TX_FIFO_STATUS  =================================================== */
#define SCB_TX_FIFO_STATUS_USED_Pos       (0UL)                 /*!< SCB TX_FIFO_STATUS: USED (Bit 0)                          */
#define SCB_TX_FIFO_STATUS_USED_Msk       (0x1ffUL)             /*!< SCB TX_FIFO_STATUS: USED (Bitfield-Mask: 0x1ff)           */
#define SCB_TX_FIFO_STATUS_SR_VALID_Pos   (15UL)                /*!< SCB TX_FIFO_STATUS: SR_VALID (Bit 15)                     */
#define SCB_TX_FIFO_STATUS_SR_VALID_Msk   (0x8000UL)            /*!< SCB TX_FIFO_STATUS: SR_VALID (Bitfield-Mask: 0x01)        */
#define SCB_TX_FIFO_STATUS_RD_PTR_Pos     (16UL)                /*!< SCB TX_FIFO_STATUS: RD_PTR (Bit 16)                       */
#define SCB_TX_FIFO_STATUS_RD_PTR_Msk     (0xff0000UL)          /*!< SCB TX_FIFO_STATUS: RD_PTR (Bitfield-Mask: 0xff)          */
#define SCB_TX_FIFO_STATUS_WR_PTR_Pos     (24UL)                /*!< SCB TX_FIFO_STATUS: WR_PTR (Bit 24)                       */
#define SCB_TX_FIFO_STATUS_WR_PTR_Msk     (0xff000000UL)        /*!< SCB TX_FIFO_STATUS: WR_PTR (Bitfield-Mask: 0xff)          */
/* ====================================================  SCB.TX_FIFO_WR  ===================================================== */
#define SCB_TX_FIFO_WR_DATA_Pos           (0UL)                 /*!< SCB TX_FIFO_WR: DATA (Bit 0)                              */
#define SCB_TX_FIFO_WR_DATA_Msk           (0xffffUL)            /*!< SCB TX_FIFO_WR: DATA (Bitfield-Mask: 0xffff)              */
/* ======================================================  SCB.RX_CTRL  ====================================================== */
#define SCB_RX_CTRL_DATA_WIDTH_Pos        (0UL)                 /*!< SCB RX_CTRL: DATA_WIDTH (Bit 0)                           */
#define SCB_RX_CTRL_DATA_WIDTH_Msk        (0xfUL)               /*!< SCB RX_CTRL: DATA_WIDTH (Bitfield-Mask: 0x0f)             */
#define SCB_RX_CTRL_MSB_FIRST_Pos         (8UL)                 /*!< SCB RX_CTRL: MSB_FIRST (Bit 8)                            */
#define SCB_RX_CTRL_MSB_FIRST_Msk         (0x100UL)             /*!< SCB RX_CTRL: MSB_FIRST (Bitfield-Mask: 0x01)              */
#define SCB_RX_CTRL_MEDIAN_Pos            (9UL)                 /*!< SCB RX_CTRL: MEDIAN (Bit 9)                               */
#define SCB_RX_CTRL_MEDIAN_Msk            (0x200UL)             /*!< SCB RX_CTRL: MEDIAN (Bitfield-Mask: 0x01)                 */
/* ===================================================  SCB.RX_FIFO_CTRL  ==================================================== */
#define SCB_RX_FIFO_CTRL_TRIGGER_LEVEL_Pos (0UL)                /*!< SCB RX_FIFO_CTRL: TRIGGER_LEVEL (Bit 0)                   */
#define SCB_RX_FIFO_CTRL_TRIGGER_LEVEL_Msk (0xffUL)             /*!< SCB RX_FIFO_CTRL: TRIGGER_LEVEL (Bitfield-Mask: 0xff)     */
#define SCB_RX_FIFO_CTRL_CLEAR_Pos        (16UL)                /*!< SCB RX_FIFO_CTRL: CLEAR (Bit 16)                          */
#define SCB_RX_FIFO_CTRL_CLEAR_Msk        (0x10000UL)           /*!< SCB RX_FIFO_CTRL: CLEAR (Bitfield-Mask: 0x01)             */
#define SCB_RX_FIFO_CTRL_FREEZE_Pos       (17UL)                /*!< SCB RX_FIFO_CTRL: FREEZE (Bit 17)                         */
#define SCB_RX_FIFO_CTRL_FREEZE_Msk       (0x20000UL)           /*!< SCB RX_FIFO_CTRL: FREEZE (Bitfield-Mask: 0x01)            */
/* ==================================================  SCB.RX_FIFO_STATUS  =================================================== */
#define SCB_RX_FIFO_STATUS_USED_Pos       (0UL)                 /*!< SCB RX_FIFO_STATUS: USED (Bit 0)                          */
#define SCB_RX_FIFO_STATUS_USED_Msk       (0x1ffUL)             /*!< SCB RX_FIFO_STATUS: USED (Bitfield-Mask: 0x1ff)           */
#define SCB_RX_FIFO_STATUS_SR_VALID_Pos   (15UL)                /*!< SCB RX_FIFO_STATUS: SR_VALID (Bit 15)                     */
#define SCB_RX_FIFO_STATUS_SR_VALID_Msk   (0x8000UL)            /*!< SCB RX_FIFO_STATUS: SR_VALID (Bitfield-Mask: 0x01)        */
#define SCB_RX_FIFO_STATUS_RD_PTR_Pos     (16UL)                /*!< SCB RX_FIFO_STATUS: RD_PTR (Bit 16)                       */
#define SCB_RX_FIFO_STATUS_RD_PTR_Msk     (0xff0000UL)          /*!< SCB RX_FIFO_STATUS: RD_PTR (Bitfield-Mask: 0xff)          */
#define SCB_RX_FIFO_STATUS_WR_PTR_Pos     (24UL)                /*!< SCB RX_FIFO_STATUS: WR_PTR (Bit 24)                       */
#define SCB_RX_FIFO_STATUS_WR_PTR_Msk     (0xff000000UL)        /*!< SCB RX_FIFO_STATUS: WR_PTR (Bitfield-Mask: 0xff)          */
/* =====================================================  SCB.RX_MATCH  ====================================================== */
#define SCB_RX_MATCH_ADDR_Pos             (0UL)                 /*!< SCB RX_MATCH: ADDR (Bit 0)                                */
#define SCB_RX_MATCH_ADDR_Msk             (0xffUL)              /*!< SCB RX_MATCH: ADDR (Bitfield-Mask: 0xff)                  */
#define SCB_RX_MATCH_MASK_Pos             (16UL)                /*!< SCB RX_MATCH: MASK (Bit 16)                               */
#define SCB_RX_MATCH_MASK_Msk             (0xff0000UL)          /*!< SCB RX_MATCH: MASK (Bitfield-Mask: 0xff)                  */
/* ====================================================  SCB.RX_FIFO_RD  ===================================================== */
#define SCB_RX_FIFO_RD_DATA_Pos           (0UL)                 /*!< SCB RX_FIFO_RD: DATA (Bit 0)                              */
#define SCB_RX_FIFO_RD_DATA_Msk           (0xffffUL)            /*!< SCB RX_FIFO_RD: DATA (Bitfield-Mask: 0xffff)              */
/* =================================================  SCB.RX_FIFO_RD_SILENT  ================================================= */
#define SCB_RX_FIFO_RD_SILENT_DATA_Pos    (0UL)                 /*!< SCB RX_FIFO_RD_SILENT: DATA (Bit 0)                       */
#define SCB_RX_FIFO_RD_SILENT_DATA_Msk    (0xffffUL)            /*!< SCB RX_FIFO_RD_SILENT: DATA (Bitfield-Mask: 0xffff)       */
/* ======================================================  SCB.EZ_DATA  ====================================================== */
#define SCB_EZ_DATA_EZ_DATA_Pos           (0UL)                 /*!< SCB EZ_DATA: EZ_DATA (Bit 0)                              */
#define SCB_EZ_DATA_EZ_DATA_Msk           (0xffUL)              /*!< SCB EZ_DATA: EZ_DATA (Bitfield-Mask: 0xff)                */
/* ====================================================  SCB.INTR_CAUSE  ===================================================== */
#define SCB_INTR_CAUSE_M_Pos              (0UL)                 /*!< SCB INTR_CAUSE: M (Bit 0)                                 */
#define SCB_INTR_CAUSE_M_Msk              (0x1UL)               /*!< SCB INTR_CAUSE: M (Bitfield-Mask: 0x01)                   */
#define SCB_INTR_CAUSE_S_Pos              (1UL)                 /*!< SCB INTR_CAUSE: S (Bit 1)                                 */
#define SCB_INTR_CAUSE_S_Msk              (0x2UL)               /*!< SCB INTR_CAUSE: S (Bitfield-Mask: 0x01)                   */
#define SCB_INTR_CAUSE_TX_Pos             (2UL)                 /*!< SCB INTR_CAUSE: TX (Bit 2)                                */
#define SCB_INTR_CAUSE_TX_Msk             (0x4UL)               /*!< SCB INTR_CAUSE: TX (Bitfield-Mask: 0x01)                  */
#define SCB_INTR_CAUSE_RX_Pos             (3UL)                 /*!< SCB INTR_CAUSE: RX (Bit 3)                                */
#define SCB_INTR_CAUSE_RX_Msk             (0x8UL)               /*!< SCB INTR_CAUSE: RX (Bitfield-Mask: 0x01)                  */
#define SCB_INTR_CAUSE_I2C_EC_Pos         (4UL)                 /*!< SCB INTR_CAUSE: I2C_EC (Bit 4)                            */
#define SCB_INTR_CAUSE_I2C_EC_Msk         (0x10UL)              /*!< SCB INTR_CAUSE: I2C_EC (Bitfield-Mask: 0x01)              */
#define SCB_INTR_CAUSE_SPI_EC_Pos         (5UL)                 /*!< SCB INTR_CAUSE: SPI_EC (Bit 5)                            */
#define SCB_INTR_CAUSE_SPI_EC_Msk         (0x20UL)              /*!< SCB INTR_CAUSE: SPI_EC (Bitfield-Mask: 0x01)              */
/* ====================================================  SCB.INTR_I2C_EC  ==================================================== */
#define SCB_INTR_I2C_EC_WAKE_UP_Pos       (0UL)                 /*!< SCB INTR_I2C_EC: WAKE_UP (Bit 0)                          */
#define SCB_INTR_I2C_EC_WAKE_UP_Msk       (0x1UL)               /*!< SCB INTR_I2C_EC: WAKE_UP (Bitfield-Mask: 0x01)            */
#define SCB_INTR_I2C_EC_EZ_STOP_Pos       (1UL)                 /*!< SCB INTR_I2C_EC: EZ_STOP (Bit 1)                          */
#define SCB_INTR_I2C_EC_EZ_STOP_Msk       (0x2UL)               /*!< SCB INTR_I2C_EC: EZ_STOP (Bitfield-Mask: 0x01)            */
#define SCB_INTR_I2C_EC_EZ_WRITE_STOP_Pos (2UL)                 /*!< SCB INTR_I2C_EC: EZ_WRITE_STOP (Bit 2)                    */
#define SCB_INTR_I2C_EC_EZ_WRITE_STOP_Msk (0x4UL)               /*!< SCB INTR_I2C_EC: EZ_WRITE_STOP (Bitfield-Mask: 0x01)      */
#define SCB_INTR_I2C_EC_EZ_READ_STOP_Pos  (3UL)                 /*!< SCB INTR_I2C_EC: EZ_READ_STOP (Bit 3)                     */
#define SCB_INTR_I2C_EC_EZ_READ_STOP_Msk  (0x8UL)               /*!< SCB INTR_I2C_EC: EZ_READ_STOP (Bitfield-Mask: 0x01)       */
/* =================================================  SCB.INTR_I2C_EC_MASK  ================================================== */
#define SCB_INTR_I2C_EC_MASK_WAKE_UP_Pos  (0UL)                 /*!< SCB INTR_I2C_EC_MASK: WAKE_UP (Bit 0)                     */
#define SCB_INTR_I2C_EC_MASK_WAKE_UP_Msk  (0x1UL)               /*!< SCB INTR_I2C_EC_MASK: WAKE_UP (Bitfield-Mask: 0x01)       */
#define SCB_INTR_I2C_EC_MASK_EZ_STOP_Pos  (1UL)                 /*!< SCB INTR_I2C_EC_MASK: EZ_STOP (Bit 1)                     */
#define SCB_INTR_I2C_EC_MASK_EZ_STOP_Msk  (0x2UL)               /*!< SCB INTR_I2C_EC_MASK: EZ_STOP (Bitfield-Mask: 0x01)       */
#define SCB_INTR_I2C_EC_MASK_EZ_WRITE_STOP_Pos (2UL)            /*!< SCB INTR_I2C_EC_MASK: EZ_WRITE_STOP (Bit 2)               */
#define SCB_INTR_I2C_EC_MASK_EZ_WRITE_STOP_Msk (0x4UL)          /*!< SCB INTR_I2C_EC_MASK: EZ_WRITE_STOP (Bitfield-Mask: 0x01) */
#define SCB_INTR_I2C_EC_MASK_EZ_READ_STOP_Pos (3UL)             /*!< SCB INTR_I2C_EC_MASK: EZ_READ_STOP (Bit 3)                */
#define SCB_INTR_I2C_EC_MASK_EZ_READ_STOP_Msk (0x8UL)           /*!< SCB INTR_I2C_EC_MASK: EZ_READ_STOP (Bitfield-Mask: 0x01)  */
/* ================================================  SCB.INTR_I2C_EC_MASKED  ================================================= */
#define SCB_INTR_I2C_EC_MASKED_WAKE_UP_Pos (0UL)                /*!< SCB INTR_I2C_EC_MASKED: WAKE_UP (Bit 0)                   */
#define SCB_INTR_I2C_EC_MASKED_WAKE_UP_Msk (0x1UL)              /*!< SCB INTR_I2C_EC_MASKED: WAKE_UP (Bitfield-Mask: 0x01)     */
#define SCB_INTR_I2C_EC_MASKED_EZ_STOP_Pos (1UL)                /*!< SCB INTR_I2C_EC_MASKED: EZ_STOP (Bit 1)                   */
#define SCB_INTR_I2C_EC_MASKED_EZ_STOP_Msk (0x2UL)              /*!< SCB INTR_I2C_EC_MASKED: EZ_STOP (Bitfield-Mask: 0x01)     */
#define SCB_INTR_I2C_EC_MASKED_EZ_WRITE_STOP_Pos (2UL)          /*!< SCB INTR_I2C_EC_MASKED: EZ_WRITE_STOP (Bit 2)             */
#define SCB_INTR_I2C_EC_MASKED_EZ_WRITE_STOP_Msk (0x4UL)        /*!< SCB INTR_I2C_EC_MASKED: EZ_WRITE_STOP (Bitfield-Mask: 0x01)*/
#define SCB_INTR_I2C_EC_MASKED_EZ_READ_STOP_Pos (3UL)           /*!< SCB INTR_I2C_EC_MASKED: EZ_READ_STOP (Bit 3)              */
#define SCB_INTR_I2C_EC_MASKED_EZ_READ_STOP_Msk (0x8UL)         /*!< SCB INTR_I2C_EC_MASKED: EZ_READ_STOP (Bitfield-Mask: 0x01)*/
/* ====================================================  SCB.INTR_SPI_EC  ==================================================== */
#define SCB_INTR_SPI_EC_WAKE_UP_Pos       (0UL)                 /*!< SCB INTR_SPI_EC: WAKE_UP (Bit 0)                          */
#define SCB_INTR_SPI_EC_WAKE_UP_Msk       (0x1UL)               /*!< SCB INTR_SPI_EC: WAKE_UP (Bitfield-Mask: 0x01)            */
#define SCB_INTR_SPI_EC_EZ_STOP_Pos       (1UL)                 /*!< SCB INTR_SPI_EC: EZ_STOP (Bit 1)                          */
#define SCB_INTR_SPI_EC_EZ_STOP_Msk       (0x2UL)               /*!< SCB INTR_SPI_EC: EZ_STOP (Bitfield-Mask: 0x01)            */
#define SCB_INTR_SPI_EC_EZ_WRITE_STOP_Pos (2UL)                 /*!< SCB INTR_SPI_EC: EZ_WRITE_STOP (Bit 2)                    */
#define SCB_INTR_SPI_EC_EZ_WRITE_STOP_Msk (0x4UL)               /*!< SCB INTR_SPI_EC: EZ_WRITE_STOP (Bitfield-Mask: 0x01)      */
#define SCB_INTR_SPI_EC_EZ_READ_STOP_Pos  (3UL)                 /*!< SCB INTR_SPI_EC: EZ_READ_STOP (Bit 3)                     */
#define SCB_INTR_SPI_EC_EZ_READ_STOP_Msk  (0x8UL)               /*!< SCB INTR_SPI_EC: EZ_READ_STOP (Bitfield-Mask: 0x01)       */
/* =================================================  SCB.INTR_SPI_EC_MASK  ================================================== */
#define SCB_INTR_SPI_EC_MASK_WAKE_UP_Pos  (0UL)                 /*!< SCB INTR_SPI_EC_MASK: WAKE_UP (Bit 0)                     */
#define SCB_INTR_SPI_EC_MASK_WAKE_UP_Msk  (0x1UL)               /*!< SCB INTR_SPI_EC_MASK: WAKE_UP (Bitfield-Mask: 0x01)       */
#define SCB_INTR_SPI_EC_MASK_EZ_STOP_Pos  (1UL)                 /*!< SCB INTR_SPI_EC_MASK: EZ_STOP (Bit 1)                     */
#define SCB_INTR_SPI_EC_MASK_EZ_STOP_Msk  (0x2UL)               /*!< SCB INTR_SPI_EC_MASK: EZ_STOP (Bitfield-Mask: 0x01)       */
#define SCB_INTR_SPI_EC_MASK_EZ_WRITE_STOP_Pos (2UL)            /*!< SCB INTR_SPI_EC_MASK: EZ_WRITE_STOP (Bit 2)               */
#define SCB_INTR_SPI_EC_MASK_EZ_WRITE_STOP_Msk (0x4UL)          /*!< SCB INTR_SPI_EC_MASK: EZ_WRITE_STOP (Bitfield-Mask: 0x01) */
#define SCB_INTR_SPI_EC_MASK_EZ_READ_STOP_Pos (3UL)             /*!< SCB INTR_SPI_EC_MASK: EZ_READ_STOP (Bit 3)                */
#define SCB_INTR_SPI_EC_MASK_EZ_READ_STOP_Msk (0x8UL)           /*!< SCB INTR_SPI_EC_MASK: EZ_READ_STOP (Bitfield-Mask: 0x01)  */
/* ================================================  SCB.INTR_SPI_EC_MASKED  ================================================= */
#define SCB_INTR_SPI_EC_MASKED_WAKE_UP_Pos (0UL)                /*!< SCB INTR_SPI_EC_MASKED: WAKE_UP (Bit 0)                   */
#define SCB_INTR_SPI_EC_MASKED_WAKE_UP_Msk (0x1UL)              /*!< SCB INTR_SPI_EC_MASKED: WAKE_UP (Bitfield-Mask: 0x01)     */
#define SCB_INTR_SPI_EC_MASKED_EZ_STOP_Pos (1UL)                /*!< SCB INTR_SPI_EC_MASKED: EZ_STOP (Bit 1)                   */
#define SCB_INTR_SPI_EC_MASKED_EZ_STOP_Msk (0x2UL)              /*!< SCB INTR_SPI_EC_MASKED: EZ_STOP (Bitfield-Mask: 0x01)     */
#define SCB_INTR_SPI_EC_MASKED_EZ_WRITE_STOP_Pos (2UL)          /*!< SCB INTR_SPI_EC_MASKED: EZ_WRITE_STOP (Bit 2)             */
#define SCB_INTR_SPI_EC_MASKED_EZ_WRITE_STOP_Msk (0x4UL)        /*!< SCB INTR_SPI_EC_MASKED: EZ_WRITE_STOP (Bitfield-Mask: 0x01)*/
#define SCB_INTR_SPI_EC_MASKED_EZ_READ_STOP_Pos (3UL)           /*!< SCB INTR_SPI_EC_MASKED: EZ_READ_STOP (Bit 3)              */
#define SCB_INTR_SPI_EC_MASKED_EZ_READ_STOP_Msk (0x8UL)         /*!< SCB INTR_SPI_EC_MASKED: EZ_READ_STOP (Bitfield-Mask: 0x01)*/
/* ======================================================  SCB.INTR_M  ======================================================= */
#define SCB_INTR_M_I2C_ARB_LOST_Pos       (0UL)                 /*!< SCB INTR_M: I2C_ARB_LOST (Bit 0)                          */
#define SCB_INTR_M_I2C_ARB_LOST_Msk       (0x1UL)               /*!< SCB INTR_M: I2C_ARB_LOST (Bitfield-Mask: 0x01)            */
#define SCB_INTR_M_I2C_NACK_Pos           (1UL)                 /*!< SCB INTR_M: I2C_NACK (Bit 1)                              */
#define SCB_INTR_M_I2C_NACK_Msk           (0x2UL)               /*!< SCB INTR_M: I2C_NACK (Bitfield-Mask: 0x01)                */
#define SCB_INTR_M_I2C_ACK_Pos            (2UL)                 /*!< SCB INTR_M: I2C_ACK (Bit 2)                               */
#define SCB_INTR_M_I2C_ACK_Msk            (0x4UL)               /*!< SCB INTR_M: I2C_ACK (Bitfield-Mask: 0x01)                 */
#define SCB_INTR_M_I2C_STOP_Pos           (4UL)                 /*!< SCB INTR_M: I2C_STOP (Bit 4)                              */
#define SCB_INTR_M_I2C_STOP_Msk           (0x10UL)              /*!< SCB INTR_M: I2C_STOP (Bitfield-Mask: 0x01)                */
#define SCB_INTR_M_I2C_BUS_ERROR_Pos      (8UL)                 /*!< SCB INTR_M: I2C_BUS_ERROR (Bit 8)                         */
#define SCB_INTR_M_I2C_BUS_ERROR_Msk      (0x100UL)             /*!< SCB INTR_M: I2C_BUS_ERROR (Bitfield-Mask: 0x01)           */
#define SCB_INTR_M_SPI_DONE_Pos           (9UL)                 /*!< SCB INTR_M: SPI_DONE (Bit 9)                              */
#define SCB_INTR_M_SPI_DONE_Msk           (0x200UL)             /*!< SCB INTR_M: SPI_DONE (Bitfield-Mask: 0x01)                */
/* ====================================================  SCB.INTR_M_SET  ===================================================== */
#define SCB_INTR_M_SET_I2C_ARB_LOST_Pos   (0UL)                 /*!< SCB INTR_M_SET: I2C_ARB_LOST (Bit 0)                      */
#define SCB_INTR_M_SET_I2C_ARB_LOST_Msk   (0x1UL)               /*!< SCB INTR_M_SET: I2C_ARB_LOST (Bitfield-Mask: 0x01)        */
#define SCB_INTR_M_SET_I2C_NACK_Pos       (1UL)                 /*!< SCB INTR_M_SET: I2C_NACK (Bit 1)                          */
#define SCB_INTR_M_SET_I2C_NACK_Msk       (0x2UL)               /*!< SCB INTR_M_SET: I2C_NACK (Bitfield-Mask: 0x01)            */
#define SCB_INTR_M_SET_I2C_ACK_Pos        (2UL)                 /*!< SCB INTR_M_SET: I2C_ACK (Bit 2)                           */
#define SCB_INTR_M_SET_I2C_ACK_Msk        (0x4UL)               /*!< SCB INTR_M_SET: I2C_ACK (Bitfield-Mask: 0x01)             */
#define SCB_INTR_M_SET_I2C_STOP_Pos       (4UL)                 /*!< SCB INTR_M_SET: I2C_STOP (Bit 4)                          */
#define SCB_INTR_M_SET_I2C_STOP_Msk       (0x10UL)              /*!< SCB INTR_M_SET: I2C_STOP (Bitfield-Mask: 0x01)            */
#define SCB_INTR_M_SET_I2C_BUS_ERROR_Pos  (8UL)                 /*!< SCB INTR_M_SET: I2C_BUS_ERROR (Bit 8)                     */
#define SCB_INTR_M_SET_I2C_BUS_ERROR_Msk  (0x100UL)             /*!< SCB INTR_M_SET: I2C_BUS_ERROR (Bitfield-Mask: 0x01)       */
#define SCB_INTR_M_SET_SPI_DONE_Pos       (9UL)                 /*!< SCB INTR_M_SET: SPI_DONE (Bit 9)                          */
#define SCB_INTR_M_SET_SPI_DONE_Msk       (0x200UL)             /*!< SCB INTR_M_SET: SPI_DONE (Bitfield-Mask: 0x01)            */
/* ====================================================  SCB.INTR_M_MASK  ==================================================== */
#define SCB_INTR_M_MASK_I2C_ARB_LOST_Pos  (0UL)                 /*!< SCB INTR_M_MASK: I2C_ARB_LOST (Bit 0)                     */
#define SCB_INTR_M_MASK_I2C_ARB_LOST_Msk  (0x1UL)               /*!< SCB INTR_M_MASK: I2C_ARB_LOST (Bitfield-Mask: 0x01)       */
#define SCB_INTR_M_MASK_I2C_NACK_Pos      (1UL)                 /*!< SCB INTR_M_MASK: I2C_NACK (Bit 1)                         */
#define SCB_INTR_M_MASK_I2C_NACK_Msk      (0x2UL)               /*!< SCB INTR_M_MASK: I2C_NACK (Bitfield-Mask: 0x01)           */
#define SCB_INTR_M_MASK_I2C_ACK_Pos       (2UL)                 /*!< SCB INTR_M_MASK: I2C_ACK (Bit 2)                          */
#define SCB_INTR_M_MASK_I2C_ACK_Msk       (0x4UL)               /*!< SCB INTR_M_MASK: I2C_ACK (Bitfield-Mask: 0x01)            */
#define SCB_INTR_M_MASK_I2C_STOP_Pos      (4UL)                 /*!< SCB INTR_M_MASK: I2C_STOP (Bit 4)                         */
#define SCB_INTR_M_MASK_I2C_STOP_Msk      (0x10UL)              /*!< SCB INTR_M_MASK: I2C_STOP (Bitfield-Mask: 0x01)           */
#define SCB_INTR_M_MASK_I2C_BUS_ERROR_Pos (8UL)                 /*!< SCB INTR_M_MASK: I2C_BUS_ERROR (Bit 8)                    */
#define SCB_INTR_M_MASK_I2C_BUS_ERROR_Msk (0x100UL)             /*!< SCB INTR_M_MASK: I2C_BUS_ERROR (Bitfield-Mask: 0x01)      */
#define SCB_INTR_M_MASK_SPI_DONE_Pos      (9UL)                 /*!< SCB INTR_M_MASK: SPI_DONE (Bit 9)                         */
#define SCB_INTR_M_MASK_SPI_DONE_Msk      (0x200UL)             /*!< SCB INTR_M_MASK: SPI_DONE (Bitfield-Mask: 0x01)           */
/* ===================================================  SCB.INTR_M_MASKED  =================================================== */
#define SCB_INTR_M_MASKED_I2C_ARB_LOST_Pos (0UL)                /*!< SCB INTR_M_MASKED: I2C_ARB_LOST (Bit 0)                   */
#define SCB_INTR_M_MASKED_I2C_ARB_LOST_Msk (0x1UL)              /*!< SCB INTR_M_MASKED: I2C_ARB_LOST (Bitfield-Mask: 0x01)     */
#define SCB_INTR_M_MASKED_I2C_NACK_Pos    (1UL)                 /*!< SCB INTR_M_MASKED: I2C_NACK (Bit 1)                       */
#define SCB_INTR_M_MASKED_I2C_NACK_Msk    (0x2UL)               /*!< SCB INTR_M_MASKED: I2C_NACK (Bitfield-Mask: 0x01)         */
#define SCB_INTR_M_MASKED_I2C_ACK_Pos     (2UL)                 /*!< SCB INTR_M_MASKED: I2C_ACK (Bit 2)                        */
#define SCB_INTR_M_MASKED_I2C_ACK_Msk     (0x4UL)               /*!< SCB INTR_M_MASKED: I2C_ACK (Bitfield-Mask: 0x01)          */
#define SCB_INTR_M_MASKED_I2C_STOP_Pos    (4UL)                 /*!< SCB INTR_M_MASKED: I2C_STOP (Bit 4)                       */
#define SCB_INTR_M_MASKED_I2C_STOP_Msk    (0x10UL)              /*!< SCB INTR_M_MASKED: I2C_STOP (Bitfield-Mask: 0x01)         */
#define SCB_INTR_M_MASKED_I2C_BUS_ERROR_Pos (8UL)               /*!< SCB INTR_M_MASKED: I2C_BUS_ERROR (Bit 8)                  */
#define SCB_INTR_M_MASKED_I2C_BUS_ERROR_Msk (0x100UL)           /*!< SCB INTR_M_MASKED: I2C_BUS_ERROR (Bitfield-Mask: 0x01)    */
#define SCB_INTR_M_MASKED_SPI_DONE_Pos    (9UL)                 /*!< SCB INTR_M_MASKED: SPI_DONE (Bit 9)                       */
#define SCB_INTR_M_MASKED_SPI_DONE_Msk    (0x200UL)             /*!< SCB INTR_M_MASKED: SPI_DONE (Bitfield-Mask: 0x01)         */
/* ======================================================  SCB.INTR_S  ======================================================= */
#define SCB_INTR_S_I2C_ARB_LOST_Pos       (0UL)                 /*!< SCB INTR_S: I2C_ARB_LOST (Bit 0)                          */
#define SCB_INTR_S_I2C_ARB_LOST_Msk       (0x1UL)               /*!< SCB INTR_S: I2C_ARB_LOST (Bitfield-Mask: 0x01)            */
#define SCB_INTR_S_I2C_NACK_Pos           (1UL)                 /*!< SCB INTR_S: I2C_NACK (Bit 1)                              */
#define SCB_INTR_S_I2C_NACK_Msk           (0x2UL)               /*!< SCB INTR_S: I2C_NACK (Bitfield-Mask: 0x01)                */
#define SCB_INTR_S_I2C_ACK_Pos            (2UL)                 /*!< SCB INTR_S: I2C_ACK (Bit 2)                               */
#define SCB_INTR_S_I2C_ACK_Msk            (0x4UL)               /*!< SCB INTR_S: I2C_ACK (Bitfield-Mask: 0x01)                 */
#define SCB_INTR_S_I2C_WRITE_STOP_Pos     (3UL)                 /*!< SCB INTR_S: I2C_WRITE_STOP (Bit 3)                        */
#define SCB_INTR_S_I2C_WRITE_STOP_Msk     (0x8UL)               /*!< SCB INTR_S: I2C_WRITE_STOP (Bitfield-Mask: 0x01)          */
#define SCB_INTR_S_I2C_STOP_Pos           (4UL)                 /*!< SCB INTR_S: I2C_STOP (Bit 4)                              */
#define SCB_INTR_S_I2C_STOP_Msk           (0x10UL)              /*!< SCB INTR_S: I2C_STOP (Bitfield-Mask: 0x01)                */
#define SCB_INTR_S_I2C_START_Pos          (5UL)                 /*!< SCB INTR_S: I2C_START (Bit 5)                             */
#define SCB_INTR_S_I2C_START_Msk          (0x20UL)              /*!< SCB INTR_S: I2C_START (Bitfield-Mask: 0x01)               */
#define SCB_INTR_S_I2C_ADDR_MATCH_Pos     (6UL)                 /*!< SCB INTR_S: I2C_ADDR_MATCH (Bit 6)                        */
#define SCB_INTR_S_I2C_ADDR_MATCH_Msk     (0x40UL)              /*!< SCB INTR_S: I2C_ADDR_MATCH (Bitfield-Mask: 0x01)          */
#define SCB_INTR_S_I2C_GENERAL_Pos        (7UL)                 /*!< SCB INTR_S: I2C_GENERAL (Bit 7)                           */
#define SCB_INTR_S_I2C_GENERAL_Msk        (0x80UL)              /*!< SCB INTR_S: I2C_GENERAL (Bitfield-Mask: 0x01)             */
#define SCB_INTR_S_I2C_BUS_ERROR_Pos      (8UL)                 /*!< SCB INTR_S: I2C_BUS_ERROR (Bit 8)                         */
#define SCB_INTR_S_I2C_BUS_ERROR_Msk      (0x100UL)             /*!< SCB INTR_S: I2C_BUS_ERROR (Bitfield-Mask: 0x01)           */
#define SCB_INTR_S_SPI_EZ_WRITE_STOP_Pos  (9UL)                 /*!< SCB INTR_S: SPI_EZ_WRITE_STOP (Bit 9)                     */
#define SCB_INTR_S_SPI_EZ_WRITE_STOP_Msk  (0x200UL)             /*!< SCB INTR_S: SPI_EZ_WRITE_STOP (Bitfield-Mask: 0x01)       */
#define SCB_INTR_S_SPI_EZ_STOP_Pos        (10UL)                /*!< SCB INTR_S: SPI_EZ_STOP (Bit 10)                          */
#define SCB_INTR_S_SPI_EZ_STOP_Msk        (0x400UL)             /*!< SCB INTR_S: SPI_EZ_STOP (Bitfield-Mask: 0x01)             */
#define SCB_INTR_S_SPI_BUS_ERROR_Pos      (11UL)                /*!< SCB INTR_S: SPI_BUS_ERROR (Bit 11)                        */
#define SCB_INTR_S_SPI_BUS_ERROR_Msk      (0x800UL)             /*!< SCB INTR_S: SPI_BUS_ERROR (Bitfield-Mask: 0x01)           */
/* ====================================================  SCB.INTR_S_SET  ===================================================== */
#define SCB_INTR_S_SET_I2C_ARB_LOST_Pos   (0UL)                 /*!< SCB INTR_S_SET: I2C_ARB_LOST (Bit 0)                      */
#define SCB_INTR_S_SET_I2C_ARB_LOST_Msk   (0x1UL)               /*!< SCB INTR_S_SET: I2C_ARB_LOST (Bitfield-Mask: 0x01)        */
#define SCB_INTR_S_SET_I2C_NACK_Pos       (1UL)                 /*!< SCB INTR_S_SET: I2C_NACK (Bit 1)                          */
#define SCB_INTR_S_SET_I2C_NACK_Msk       (0x2UL)               /*!< SCB INTR_S_SET: I2C_NACK (Bitfield-Mask: 0x01)            */
#define SCB_INTR_S_SET_I2C_ACK_Pos        (2UL)                 /*!< SCB INTR_S_SET: I2C_ACK (Bit 2)                           */
#define SCB_INTR_S_SET_I2C_ACK_Msk        (0x4UL)               /*!< SCB INTR_S_SET: I2C_ACK (Bitfield-Mask: 0x01)             */
#define SCB_INTR_S_SET_I2C_WRITE_STOP_Pos (3UL)                 /*!< SCB INTR_S_SET: I2C_WRITE_STOP (Bit 3)                    */
#define SCB_INTR_S_SET_I2C_WRITE_STOP_Msk (0x8UL)               /*!< SCB INTR_S_SET: I2C_WRITE_STOP (Bitfield-Mask: 0x01)      */
#define SCB_INTR_S_SET_I2C_STOP_Pos       (4UL)                 /*!< SCB INTR_S_SET: I2C_STOP (Bit 4)                          */
#define SCB_INTR_S_SET_I2C_STOP_Msk       (0x10UL)              /*!< SCB INTR_S_SET: I2C_STOP (Bitfield-Mask: 0x01)            */
#define SCB_INTR_S_SET_I2C_START_Pos      (5UL)                 /*!< SCB INTR_S_SET: I2C_START (Bit 5)                         */
#define SCB_INTR_S_SET_I2C_START_Msk      (0x20UL)              /*!< SCB INTR_S_SET: I2C_START (Bitfield-Mask: 0x01)           */
#define SCB_INTR_S_SET_I2C_ADDR_MATCH_Pos (6UL)                 /*!< SCB INTR_S_SET: I2C_ADDR_MATCH (Bit 6)                    */
#define SCB_INTR_S_SET_I2C_ADDR_MATCH_Msk (0x40UL)              /*!< SCB INTR_S_SET: I2C_ADDR_MATCH (Bitfield-Mask: 0x01)      */
#define SCB_INTR_S_SET_I2C_GENERAL_Pos    (7UL)                 /*!< SCB INTR_S_SET: I2C_GENERAL (Bit 7)                       */
#define SCB_INTR_S_SET_I2C_GENERAL_Msk    (0x80UL)              /*!< SCB INTR_S_SET: I2C_GENERAL (Bitfield-Mask: 0x01)         */
#define SCB_INTR_S_SET_I2C_BUS_ERROR_Pos  (8UL)                 /*!< SCB INTR_S_SET: I2C_BUS_ERROR (Bit 8)                     */
#define SCB_INTR_S_SET_I2C_BUS_ERROR_Msk  (0x100UL)             /*!< SCB INTR_S_SET: I2C_BUS_ERROR (Bitfield-Mask: 0x01)       */
#define SCB_INTR_S_SET_SPI_EZ_WRITE_STOP_Pos (9UL)              /*!< SCB INTR_S_SET: SPI_EZ_WRITE_STOP (Bit 9)                 */
#define SCB_INTR_S_SET_SPI_EZ_WRITE_STOP_Msk (0x200UL)          /*!< SCB INTR_S_SET: SPI_EZ_WRITE_STOP (Bitfield-Mask: 0x01)   */
#define SCB_INTR_S_SET_SPI_EZ_STOP_Pos    (10UL)                /*!< SCB INTR_S_SET: SPI_EZ_STOP (Bit 10)                      */
#define SCB_INTR_S_SET_SPI_EZ_STOP_Msk    (0x400UL)             /*!< SCB INTR_S_SET: SPI_EZ_STOP (Bitfield-Mask: 0x01)         */
#define SCB_INTR_S_SET_SPI_BUS_ERROR_Pos  (11UL)                /*!< SCB INTR_S_SET: SPI_BUS_ERROR (Bit 11)                    */
#define SCB_INTR_S_SET_SPI_BUS_ERROR_Msk  (0x800UL)             /*!< SCB INTR_S_SET: SPI_BUS_ERROR (Bitfield-Mask: 0x01)       */
/* ====================================================  SCB.INTR_S_MASK  ==================================================== */
#define SCB_INTR_S_MASK_I2C_ARB_LOST_Pos  (0UL)                 /*!< SCB INTR_S_MASK: I2C_ARB_LOST (Bit 0)                     */
#define SCB_INTR_S_MASK_I2C_ARB_LOST_Msk  (0x1UL)               /*!< SCB INTR_S_MASK: I2C_ARB_LOST (Bitfield-Mask: 0x01)       */
#define SCB_INTR_S_MASK_I2C_NACK_Pos      (1UL)                 /*!< SCB INTR_S_MASK: I2C_NACK (Bit 1)                         */
#define SCB_INTR_S_MASK_I2C_NACK_Msk      (0x2UL)               /*!< SCB INTR_S_MASK: I2C_NACK (Bitfield-Mask: 0x01)           */
#define SCB_INTR_S_MASK_I2C_ACK_Pos       (2UL)                 /*!< SCB INTR_S_MASK: I2C_ACK (Bit 2)                          */
#define SCB_INTR_S_MASK_I2C_ACK_Msk       (0x4UL)               /*!< SCB INTR_S_MASK: I2C_ACK (Bitfield-Mask: 0x01)            */
#define SCB_INTR_S_MASK_I2C_WRITE_STOP_Pos (3UL)                /*!< SCB INTR_S_MASK: I2C_WRITE_STOP (Bit 3)                   */
#define SCB_INTR_S_MASK_I2C_WRITE_STOP_Msk (0x8UL)              /*!< SCB INTR_S_MASK: I2C_WRITE_STOP (Bitfield-Mask: 0x01)     */
#define SCB_INTR_S_MASK_I2C_STOP_Pos      (4UL)                 /*!< SCB INTR_S_MASK: I2C_STOP (Bit 4)                         */
#define SCB_INTR_S_MASK_I2C_STOP_Msk      (0x10UL)              /*!< SCB INTR_S_MASK: I2C_STOP (Bitfield-Mask: 0x01)           */
#define SCB_INTR_S_MASK_I2C_START_Pos     (5UL)                 /*!< SCB INTR_S_MASK: I2C_START (Bit 5)                        */
#define SCB_INTR_S_MASK_I2C_START_Msk     (0x20UL)              /*!< SCB INTR_S_MASK: I2C_START (Bitfield-Mask: 0x01)          */
#define SCB_INTR_S_MASK_I2C_ADDR_MATCH_Pos (6UL)                /*!< SCB INTR_S_MASK: I2C_ADDR_MATCH (Bit 6)                   */
#define SCB_INTR_S_MASK_I2C_ADDR_MATCH_Msk (0x40UL)             /*!< SCB INTR_S_MASK: I2C_ADDR_MATCH (Bitfield-Mask: 0x01)     */
#define SCB_INTR_S_MASK_I2C_GENERAL_Pos   (7UL)                 /*!< SCB INTR_S_MASK: I2C_GENERAL (Bit 7)                      */
#define SCB_INTR_S_MASK_I2C_GENERAL_Msk   (0x80UL)              /*!< SCB INTR_S_MASK: I2C_GENERAL (Bitfield-Mask: 0x01)        */
#define SCB_INTR_S_MASK_I2C_BUS_ERROR_Pos (8UL)                 /*!< SCB INTR_S_MASK: I2C_BUS_ERROR (Bit 8)                    */
#define SCB_INTR_S_MASK_I2C_BUS_ERROR_Msk (0x100UL)             /*!< SCB INTR_S_MASK: I2C_BUS_ERROR (Bitfield-Mask: 0x01)      */
#define SCB_INTR_S_MASK_SPI_EZ_WRITE_STOP_Pos (9UL)             /*!< SCB INTR_S_MASK: SPI_EZ_WRITE_STOP (Bit 9)                */
#define SCB_INTR_S_MASK_SPI_EZ_WRITE_STOP_Msk (0x200UL)         /*!< SCB INTR_S_MASK: SPI_EZ_WRITE_STOP (Bitfield-Mask: 0x01)  */
#define SCB_INTR_S_MASK_SPI_EZ_STOP_Pos   (10UL)                /*!< SCB INTR_S_MASK: SPI_EZ_STOP (Bit 10)                     */
#define SCB_INTR_S_MASK_SPI_EZ_STOP_Msk   (0x400UL)             /*!< SCB INTR_S_MASK: SPI_EZ_STOP (Bitfield-Mask: 0x01)        */
#define SCB_INTR_S_MASK_SPI_BUS_ERROR_Pos (11UL)                /*!< SCB INTR_S_MASK: SPI_BUS_ERROR (Bit 11)                   */
#define SCB_INTR_S_MASK_SPI_BUS_ERROR_Msk (0x800UL)             /*!< SCB INTR_S_MASK: SPI_BUS_ERROR (Bitfield-Mask: 0x01)      */
/* ===================================================  SCB.INTR_S_MASKED  =================================================== */
#define SCB_INTR_S_MASKED_I2C_ARB_LOST_Pos (0UL)                /*!< SCB INTR_S_MASKED: I2C_ARB_LOST (Bit 0)                   */
#define SCB_INTR_S_MASKED_I2C_ARB_LOST_Msk (0x1UL)              /*!< SCB INTR_S_MASKED: I2C_ARB_LOST (Bitfield-Mask: 0x01)     */
#define SCB_INTR_S_MASKED_I2C_NACK_Pos    (1UL)                 /*!< SCB INTR_S_MASKED: I2C_NACK (Bit 1)                       */
#define SCB_INTR_S_MASKED_I2C_NACK_Msk    (0x2UL)               /*!< SCB INTR_S_MASKED: I2C_NACK (Bitfield-Mask: 0x01)         */
#define SCB_INTR_S_MASKED_I2C_ACK_Pos     (2UL)                 /*!< SCB INTR_S_MASKED: I2C_ACK (Bit 2)                        */
#define SCB_INTR_S_MASKED_I2C_ACK_Msk     (0x4UL)               /*!< SCB INTR_S_MASKED: I2C_ACK (Bitfield-Mask: 0x01)          */
#define SCB_INTR_S_MASKED_I2C_WRITE_STOP_Pos (3UL)              /*!< SCB INTR_S_MASKED: I2C_WRITE_STOP (Bit 3)                 */
#define SCB_INTR_S_MASKED_I2C_WRITE_STOP_Msk (0x8UL)            /*!< SCB INTR_S_MASKED: I2C_WRITE_STOP (Bitfield-Mask: 0x01)   */
#define SCB_INTR_S_MASKED_I2C_STOP_Pos    (4UL)                 /*!< SCB INTR_S_MASKED: I2C_STOP (Bit 4)                       */
#define SCB_INTR_S_MASKED_I2C_STOP_Msk    (0x10UL)              /*!< SCB INTR_S_MASKED: I2C_STOP (Bitfield-Mask: 0x01)         */
#define SCB_INTR_S_MASKED_I2C_START_Pos   (5UL)                 /*!< SCB INTR_S_MASKED: I2C_START (Bit 5)                      */
#define SCB_INTR_S_MASKED_I2C_START_Msk   (0x20UL)              /*!< SCB INTR_S_MASKED: I2C_START (Bitfield-Mask: 0x01)        */
#define SCB_INTR_S_MASKED_I2C_ADDR_MATCH_Pos (6UL)              /*!< SCB INTR_S_MASKED: I2C_ADDR_MATCH (Bit 6)                 */
#define SCB_INTR_S_MASKED_I2C_ADDR_MATCH_Msk (0x40UL)           /*!< SCB INTR_S_MASKED: I2C_ADDR_MATCH (Bitfield-Mask: 0x01)   */
#define SCB_INTR_S_MASKED_I2C_GENERAL_Pos (7UL)                 /*!< SCB INTR_S_MASKED: I2C_GENERAL (Bit 7)                    */
#define SCB_INTR_S_MASKED_I2C_GENERAL_Msk (0x80UL)              /*!< SCB INTR_S_MASKED: I2C_GENERAL (Bitfield-Mask: 0x01)      */
#define SCB_INTR_S_MASKED_I2C_BUS_ERROR_Pos (8UL)               /*!< SCB INTR_S_MASKED: I2C_BUS_ERROR (Bit 8)                  */
#define SCB_INTR_S_MASKED_I2C_BUS_ERROR_Msk (0x100UL)           /*!< SCB INTR_S_MASKED: I2C_BUS_ERROR (Bitfield-Mask: 0x01)    */
#define SCB_INTR_S_MASKED_SPI_EZ_WRITE_STOP_Pos (9UL)           /*!< SCB INTR_S_MASKED: SPI_EZ_WRITE_STOP (Bit 9)              */
#define SCB_INTR_S_MASKED_SPI_EZ_WRITE_STOP_Msk (0x200UL)       /*!< SCB INTR_S_MASKED: SPI_EZ_WRITE_STOP (Bitfield-Mask: 0x01)*/
#define SCB_INTR_S_MASKED_SPI_EZ_STOP_Pos (10UL)                /*!< SCB INTR_S_MASKED: SPI_EZ_STOP (Bit 10)                   */
#define SCB_INTR_S_MASKED_SPI_EZ_STOP_Msk (0x400UL)             /*!< SCB INTR_S_MASKED: SPI_EZ_STOP (Bitfield-Mask: 0x01)      */
#define SCB_INTR_S_MASKED_SPI_BUS_ERROR_Pos (11UL)              /*!< SCB INTR_S_MASKED: SPI_BUS_ERROR (Bit 11)                 */
#define SCB_INTR_S_MASKED_SPI_BUS_ERROR_Msk (0x800UL)           /*!< SCB INTR_S_MASKED: SPI_BUS_ERROR (Bitfield-Mask: 0x01)    */
/* ======================================================  SCB.INTR_TX  ====================================================== */
#define SCB_INTR_TX_TRIGGER_Pos           (0UL)                 /*!< SCB INTR_TX: TRIGGER (Bit 0)                              */
#define SCB_INTR_TX_TRIGGER_Msk           (0x1UL)               /*!< SCB INTR_TX: TRIGGER (Bitfield-Mask: 0x01)                */
#define SCB_INTR_TX_NOT_FULL_Pos          (1UL)                 /*!< SCB INTR_TX: NOT_FULL (Bit 1)                             */
#define SCB_INTR_TX_NOT_FULL_Msk          (0x2UL)               /*!< SCB INTR_TX: NOT_FULL (Bitfield-Mask: 0x01)               */
#define SCB_INTR_TX_EMPTY_Pos             (4UL)                 /*!< SCB INTR_TX: EMPTY (Bit 4)                                */
#define SCB_INTR_TX_EMPTY_Msk             (0x10UL)              /*!< SCB INTR_TX: EMPTY (Bitfield-Mask: 0x01)                  */
#define SCB_INTR_TX_OVERFLOW_Pos          (5UL)                 /*!< SCB INTR_TX: OVERFLOW (Bit 5)                             */
#define SCB_INTR_TX_OVERFLOW_Msk          (0x20UL)              /*!< SCB INTR_TX: OVERFLOW (Bitfield-Mask: 0x01)               */
#define SCB_INTR_TX_UNDERFLOW_Pos         (6UL)                 /*!< SCB INTR_TX: UNDERFLOW (Bit 6)                            */
#define SCB_INTR_TX_UNDERFLOW_Msk         (0x40UL)              /*!< SCB INTR_TX: UNDERFLOW (Bitfield-Mask: 0x01)              */
#define SCB_INTR_TX_BLOCKED_Pos           (7UL)                 /*!< SCB INTR_TX: BLOCKED (Bit 7)                              */
#define SCB_INTR_TX_BLOCKED_Msk           (0x80UL)              /*!< SCB INTR_TX: BLOCKED (Bitfield-Mask: 0x01)                */
#define SCB_INTR_TX_UART_NACK_Pos         (8UL)                 /*!< SCB INTR_TX: UART_NACK (Bit 8)                            */
#define SCB_INTR_TX_UART_NACK_Msk         (0x100UL)             /*!< SCB INTR_TX: UART_NACK (Bitfield-Mask: 0x01)              */
#define SCB_INTR_TX_UART_DONE_Pos         (9UL)                 /*!< SCB INTR_TX: UART_DONE (Bit 9)                            */
#define SCB_INTR_TX_UART_DONE_Msk         (0x200UL)             /*!< SCB INTR_TX: UART_DONE (Bitfield-Mask: 0x01)              */
#define SCB_INTR_TX_UART_ARB_LOST_Pos     (10UL)                /*!< SCB INTR_TX: UART_ARB_LOST (Bit 10)                       */
#define SCB_INTR_TX_UART_ARB_LOST_Msk     (0x400UL)             /*!< SCB INTR_TX: UART_ARB_LOST (Bitfield-Mask: 0x01)          */
/* ====================================================  SCB.INTR_TX_SET  ==================================================== */
#define SCB_INTR_TX_SET_TRIGGER_Pos       (0UL)                 /*!< SCB INTR_TX_SET: TRIGGER (Bit 0)                          */
#define SCB_INTR_TX_SET_TRIGGER_Msk       (0x1UL)               /*!< SCB INTR_TX_SET: TRIGGER (Bitfield-Mask: 0x01)            */
#define SCB_INTR_TX_SET_NOT_FULL_Pos      (1UL)                 /*!< SCB INTR_TX_SET: NOT_FULL (Bit 1)                         */
#define SCB_INTR_TX_SET_NOT_FULL_Msk      (0x2UL)               /*!< SCB INTR_TX_SET: NOT_FULL (Bitfield-Mask: 0x01)           */
#define SCB_INTR_TX_SET_EMPTY_Pos         (4UL)                 /*!< SCB INTR_TX_SET: EMPTY (Bit 4)                            */
#define SCB_INTR_TX_SET_EMPTY_Msk         (0x10UL)              /*!< SCB INTR_TX_SET: EMPTY (Bitfield-Mask: 0x01)              */
#define SCB_INTR_TX_SET_OVERFLOW_Pos      (5UL)                 /*!< SCB INTR_TX_SET: OVERFLOW (Bit 5)                         */
#define SCB_INTR_TX_SET_OVERFLOW_Msk      (0x20UL)              /*!< SCB INTR_TX_SET: OVERFLOW (Bitfield-Mask: 0x01)           */
#define SCB_INTR_TX_SET_UNDERFLOW_Pos     (6UL)                 /*!< SCB INTR_TX_SET: UNDERFLOW (Bit 6)                        */
#define SCB_INTR_TX_SET_UNDERFLOW_Msk     (0x40UL)              /*!< SCB INTR_TX_SET: UNDERFLOW (Bitfield-Mask: 0x01)          */
#define SCB_INTR_TX_SET_BLOCKED_Pos       (7UL)                 /*!< SCB INTR_TX_SET: BLOCKED (Bit 7)                          */
#define SCB_INTR_TX_SET_BLOCKED_Msk       (0x80UL)              /*!< SCB INTR_TX_SET: BLOCKED (Bitfield-Mask: 0x01)            */
#define SCB_INTR_TX_SET_UART_NACK_Pos     (8UL)                 /*!< SCB INTR_TX_SET: UART_NACK (Bit 8)                        */
#define SCB_INTR_TX_SET_UART_NACK_Msk     (0x100UL)             /*!< SCB INTR_TX_SET: UART_NACK (Bitfield-Mask: 0x01)          */
#define SCB_INTR_TX_SET_UART_DONE_Pos     (9UL)                 /*!< SCB INTR_TX_SET: UART_DONE (Bit 9)                        */
#define SCB_INTR_TX_SET_UART_DONE_Msk     (0x200UL)             /*!< SCB INTR_TX_SET: UART_DONE (Bitfield-Mask: 0x01)          */
#define SCB_INTR_TX_SET_UART_ARB_LOST_Pos (10UL)                /*!< SCB INTR_TX_SET: UART_ARB_LOST (Bit 10)                   */
#define SCB_INTR_TX_SET_UART_ARB_LOST_Msk (0x400UL)             /*!< SCB INTR_TX_SET: UART_ARB_LOST (Bitfield-Mask: 0x01)      */
/* ===================================================  SCB.INTR_TX_MASK  ==================================================== */
#define SCB_INTR_TX_MASK_TRIGGER_Pos      (0UL)                 /*!< SCB INTR_TX_MASK: TRIGGER (Bit 0)                         */
#define SCB_INTR_TX_MASK_TRIGGER_Msk      (0x1UL)               /*!< SCB INTR_TX_MASK: TRIGGER (Bitfield-Mask: 0x01)           */
#define SCB_INTR_TX_MASK_NOT_FULL_Pos     (1UL)                 /*!< SCB INTR_TX_MASK: NOT_FULL (Bit 1)                        */
#define SCB_INTR_TX_MASK_NOT_FULL_Msk     (0x2UL)               /*!< SCB INTR_TX_MASK: NOT_FULL (Bitfield-Mask: 0x01)          */
#define SCB_INTR_TX_MASK_EMPTY_Pos        (4UL)                 /*!< SCB INTR_TX_MASK: EMPTY (Bit 4)                           */
#define SCB_INTR_TX_MASK_EMPTY_Msk        (0x10UL)              /*!< SCB INTR_TX_MASK: EMPTY (Bitfield-Mask: 0x01)             */
#define SCB_INTR_TX_MASK_OVERFLOW_Pos     (5UL)                 /*!< SCB INTR_TX_MASK: OVERFLOW (Bit 5)                        */
#define SCB_INTR_TX_MASK_OVERFLOW_Msk     (0x20UL)              /*!< SCB INTR_TX_MASK: OVERFLOW (Bitfield-Mask: 0x01)          */
#define SCB_INTR_TX_MASK_UNDERFLOW_Pos    (6UL)                 /*!< SCB INTR_TX_MASK: UNDERFLOW (Bit 6)                       */
#define SCB_INTR_TX_MASK_UNDERFLOW_Msk    (0x40UL)              /*!< SCB INTR_TX_MASK: UNDERFLOW (Bitfield-Mask: 0x01)         */
#define SCB_INTR_TX_MASK_BLOCKED_Pos      (7UL)                 /*!< SCB INTR_TX_MASK: BLOCKED (Bit 7)                         */
#define SCB_INTR_TX_MASK_BLOCKED_Msk      (0x80UL)              /*!< SCB INTR_TX_MASK: BLOCKED (Bitfield-Mask: 0x01)           */
#define SCB_INTR_TX_MASK_UART_NACK_Pos    (8UL)                 /*!< SCB INTR_TX_MASK: UART_NACK (Bit 8)                       */
#define SCB_INTR_TX_MASK_UART_NACK_Msk    (0x100UL)             /*!< SCB INTR_TX_MASK: UART_NACK (Bitfield-Mask: 0x01)         */
#define SCB_INTR_TX_MASK_UART_DONE_Pos    (9UL)                 /*!< SCB INTR_TX_MASK: UART_DONE (Bit 9)                       */
#define SCB_INTR_TX_MASK_UART_DONE_Msk    (0x200UL)             /*!< SCB INTR_TX_MASK: UART_DONE (Bitfield-Mask: 0x01)         */
#define SCB_INTR_TX_MASK_UART_ARB_LOST_Pos (10UL)               /*!< SCB INTR_TX_MASK: UART_ARB_LOST (Bit 10)                  */
#define SCB_INTR_TX_MASK_UART_ARB_LOST_Msk (0x400UL)            /*!< SCB INTR_TX_MASK: UART_ARB_LOST (Bitfield-Mask: 0x01)     */
/* ==================================================  SCB.INTR_TX_MASKED  =================================================== */
#define SCB_INTR_TX_MASKED_TRIGGER_Pos    (0UL)                 /*!< SCB INTR_TX_MASKED: TRIGGER (Bit 0)                       */
#define SCB_INTR_TX_MASKED_TRIGGER_Msk    (0x1UL)               /*!< SCB INTR_TX_MASKED: TRIGGER (Bitfield-Mask: 0x01)         */
#define SCB_INTR_TX_MASKED_NOT_FULL_Pos   (1UL)                 /*!< SCB INTR_TX_MASKED: NOT_FULL (Bit 1)                      */
#define SCB_INTR_TX_MASKED_NOT_FULL_Msk   (0x2UL)               /*!< SCB INTR_TX_MASKED: NOT_FULL (Bitfield-Mask: 0x01)        */
#define SCB_INTR_TX_MASKED_EMPTY_Pos      (4UL)                 /*!< SCB INTR_TX_MASKED: EMPTY (Bit 4)                         */
#define SCB_INTR_TX_MASKED_EMPTY_Msk      (0x10UL)              /*!< SCB INTR_TX_MASKED: EMPTY (Bitfield-Mask: 0x01)           */
#define SCB_INTR_TX_MASKED_OVERFLOW_Pos   (5UL)                 /*!< SCB INTR_TX_MASKED: OVERFLOW (Bit 5)                      */
#define SCB_INTR_TX_MASKED_OVERFLOW_Msk   (0x20UL)              /*!< SCB INTR_TX_MASKED: OVERFLOW (Bitfield-Mask: 0x01)        */
#define SCB_INTR_TX_MASKED_UNDERFLOW_Pos  (6UL)                 /*!< SCB INTR_TX_MASKED: UNDERFLOW (Bit 6)                     */
#define SCB_INTR_TX_MASKED_UNDERFLOW_Msk  (0x40UL)              /*!< SCB INTR_TX_MASKED: UNDERFLOW (Bitfield-Mask: 0x01)       */
#define SCB_INTR_TX_MASKED_BLOCKED_Pos    (7UL)                 /*!< SCB INTR_TX_MASKED: BLOCKED (Bit 7)                       */
#define SCB_INTR_TX_MASKED_BLOCKED_Msk    (0x80UL)              /*!< SCB INTR_TX_MASKED: BLOCKED (Bitfield-Mask: 0x01)         */
#define SCB_INTR_TX_MASKED_UART_NACK_Pos  (8UL)                 /*!< SCB INTR_TX_MASKED: UART_NACK (Bit 8)                     */
#define SCB_INTR_TX_MASKED_UART_NACK_Msk  (0x100UL)             /*!< SCB INTR_TX_MASKED: UART_NACK (Bitfield-Mask: 0x01)       */
#define SCB_INTR_TX_MASKED_UART_DONE_Pos  (9UL)                 /*!< SCB INTR_TX_MASKED: UART_DONE (Bit 9)                     */
#define SCB_INTR_TX_MASKED_UART_DONE_Msk  (0x200UL)             /*!< SCB INTR_TX_MASKED: UART_DONE (Bitfield-Mask: 0x01)       */
#define SCB_INTR_TX_MASKED_UART_ARB_LOST_Pos (10UL)             /*!< SCB INTR_TX_MASKED: UART_ARB_LOST (Bit 10)                */
#define SCB_INTR_TX_MASKED_UART_ARB_LOST_Msk (0x400UL)          /*!< SCB INTR_TX_MASKED: UART_ARB_LOST (Bitfield-Mask: 0x01)   */
/* ======================================================  SCB.INTR_RX  ====================================================== */
#define SCB_INTR_RX_TRIGGER_Pos           (0UL)                 /*!< SCB INTR_RX: TRIGGER (Bit 0)                              */
#define SCB_INTR_RX_TRIGGER_Msk           (0x1UL)               /*!< SCB INTR_RX: TRIGGER (Bitfield-Mask: 0x01)                */
#define SCB_INTR_RX_NOT_EMPTY_Pos         (2UL)                 /*!< SCB INTR_RX: NOT_EMPTY (Bit 2)                            */
#define SCB_INTR_RX_NOT_EMPTY_Msk         (0x4UL)               /*!< SCB INTR_RX: NOT_EMPTY (Bitfield-Mask: 0x01)              */
#define SCB_INTR_RX_FULL_Pos              (3UL)                 /*!< SCB INTR_RX: FULL (Bit 3)                                 */
#define SCB_INTR_RX_FULL_Msk              (0x8UL)               /*!< SCB INTR_RX: FULL (Bitfield-Mask: 0x01)                   */
#define SCB_INTR_RX_OVERFLOW_Pos          (5UL)                 /*!< SCB INTR_RX: OVERFLOW (Bit 5)                             */
#define SCB_INTR_RX_OVERFLOW_Msk          (0x20UL)              /*!< SCB INTR_RX: OVERFLOW (Bitfield-Mask: 0x01)               */
#define SCB_INTR_RX_UNDERFLOW_Pos         (6UL)                 /*!< SCB INTR_RX: UNDERFLOW (Bit 6)                            */
#define SCB_INTR_RX_UNDERFLOW_Msk         (0x40UL)              /*!< SCB INTR_RX: UNDERFLOW (Bitfield-Mask: 0x01)              */
#define SCB_INTR_RX_BLOCKED_Pos           (7UL)                 /*!< SCB INTR_RX: BLOCKED (Bit 7)                              */
#define SCB_INTR_RX_BLOCKED_Msk           (0x80UL)              /*!< SCB INTR_RX: BLOCKED (Bitfield-Mask: 0x01)                */
#define SCB_INTR_RX_FRAME_ERROR_Pos       (8UL)                 /*!< SCB INTR_RX: FRAME_ERROR (Bit 8)                          */
#define SCB_INTR_RX_FRAME_ERROR_Msk       (0x100UL)             /*!< SCB INTR_RX: FRAME_ERROR (Bitfield-Mask: 0x01)            */
#define SCB_INTR_RX_PARITY_ERROR_Pos      (9UL)                 /*!< SCB INTR_RX: PARITY_ERROR (Bit 9)                         */
#define SCB_INTR_RX_PARITY_ERROR_Msk      (0x200UL)             /*!< SCB INTR_RX: PARITY_ERROR (Bitfield-Mask: 0x01)           */
#define SCB_INTR_RX_BAUD_DETECT_Pos       (10UL)                /*!< SCB INTR_RX: BAUD_DETECT (Bit 10)                         */
#define SCB_INTR_RX_BAUD_DETECT_Msk       (0x400UL)             /*!< SCB INTR_RX: BAUD_DETECT (Bitfield-Mask: 0x01)            */
#define SCB_INTR_RX_BREAK_DETECT_Pos      (11UL)                /*!< SCB INTR_RX: BREAK_DETECT (Bit 11)                        */
#define SCB_INTR_RX_BREAK_DETECT_Msk      (0x800UL)             /*!< SCB INTR_RX: BREAK_DETECT (Bitfield-Mask: 0x01)           */
/* ====================================================  SCB.INTR_RX_SET  ==================================================== */
#define SCB_INTR_RX_SET_TRIGGER_Pos       (0UL)                 /*!< SCB INTR_RX_SET: TRIGGER (Bit 0)                          */
#define SCB_INTR_RX_SET_TRIGGER_Msk       (0x1UL)               /*!< SCB INTR_RX_SET: TRIGGER (Bitfield-Mask: 0x01)            */
#define SCB_INTR_RX_SET_NOT_EMPTY_Pos     (2UL)                 /*!< SCB INTR_RX_SET: NOT_EMPTY (Bit 2)                        */
#define SCB_INTR_RX_SET_NOT_EMPTY_Msk     (0x4UL)               /*!< SCB INTR_RX_SET: NOT_EMPTY (Bitfield-Mask: 0x01)          */
#define SCB_INTR_RX_SET_FULL_Pos          (3UL)                 /*!< SCB INTR_RX_SET: FULL (Bit 3)                             */
#define SCB_INTR_RX_SET_FULL_Msk          (0x8UL)               /*!< SCB INTR_RX_SET: FULL (Bitfield-Mask: 0x01)               */
#define SCB_INTR_RX_SET_OVERFLOW_Pos      (5UL)                 /*!< SCB INTR_RX_SET: OVERFLOW (Bit 5)                         */
#define SCB_INTR_RX_SET_OVERFLOW_Msk      (0x20UL)              /*!< SCB INTR_RX_SET: OVERFLOW (Bitfield-Mask: 0x01)           */
#define SCB_INTR_RX_SET_UNDERFLOW_Pos     (6UL)                 /*!< SCB INTR_RX_SET: UNDERFLOW (Bit 6)                        */
#define SCB_INTR_RX_SET_UNDERFLOW_Msk     (0x40UL)              /*!< SCB INTR_RX_SET: UNDERFLOW (Bitfield-Mask: 0x01)          */
#define SCB_INTR_RX_SET_BLOCKED_Pos       (7UL)                 /*!< SCB INTR_RX_SET: BLOCKED (Bit 7)                          */
#define SCB_INTR_RX_SET_BLOCKED_Msk       (0x80UL)              /*!< SCB INTR_RX_SET: BLOCKED (Bitfield-Mask: 0x01)            */
#define SCB_INTR_RX_SET_FRAME_ERROR_Pos   (8UL)                 /*!< SCB INTR_RX_SET: FRAME_ERROR (Bit 8)                      */
#define SCB_INTR_RX_SET_FRAME_ERROR_Msk   (0x100UL)             /*!< SCB INTR_RX_SET: FRAME_ERROR (Bitfield-Mask: 0x01)        */
#define SCB_INTR_RX_SET_PARITY_ERROR_Pos  (9UL)                 /*!< SCB INTR_RX_SET: PARITY_ERROR (Bit 9)                     */
#define SCB_INTR_RX_SET_PARITY_ERROR_Msk  (0x200UL)             /*!< SCB INTR_RX_SET: PARITY_ERROR (Bitfield-Mask: 0x01)       */
#define SCB_INTR_RX_SET_BAUD_DETECT_Pos   (10UL)                /*!< SCB INTR_RX_SET: BAUD_DETECT (Bit 10)                     */
#define SCB_INTR_RX_SET_BAUD_DETECT_Msk   (0x400UL)             /*!< SCB INTR_RX_SET: BAUD_DETECT (Bitfield-Mask: 0x01)        */
#define SCB_INTR_RX_SET_BREAK_DETECT_Pos  (11UL)                /*!< SCB INTR_RX_SET: BREAK_DETECT (Bit 11)                    */
#define SCB_INTR_RX_SET_BREAK_DETECT_Msk  (0x800UL)             /*!< SCB INTR_RX_SET: BREAK_DETECT (Bitfield-Mask: 0x01)       */
/* ===================================================  SCB.INTR_RX_MASK  ==================================================== */
#define SCB_INTR_RX_MASK_TRIGGER_Pos      (0UL)                 /*!< SCB INTR_RX_MASK: TRIGGER (Bit 0)                         */
#define SCB_INTR_RX_MASK_TRIGGER_Msk      (0x1UL)               /*!< SCB INTR_RX_MASK: TRIGGER (Bitfield-Mask: 0x01)           */
#define SCB_INTR_RX_MASK_NOT_EMPTY_Pos    (2UL)                 /*!< SCB INTR_RX_MASK: NOT_EMPTY (Bit 2)                       */
#define SCB_INTR_RX_MASK_NOT_EMPTY_Msk    (0x4UL)               /*!< SCB INTR_RX_MASK: NOT_EMPTY (Bitfield-Mask: 0x01)         */
#define SCB_INTR_RX_MASK_FULL_Pos         (3UL)                 /*!< SCB INTR_RX_MASK: FULL (Bit 3)                            */
#define SCB_INTR_RX_MASK_FULL_Msk         (0x8UL)               /*!< SCB INTR_RX_MASK: FULL (Bitfield-Mask: 0x01)              */
#define SCB_INTR_RX_MASK_OVERFLOW_Pos     (5UL)                 /*!< SCB INTR_RX_MASK: OVERFLOW (Bit 5)                        */
#define SCB_INTR_RX_MASK_OVERFLOW_Msk     (0x20UL)              /*!< SCB INTR_RX_MASK: OVERFLOW (Bitfield-Mask: 0x01)          */
#define SCB_INTR_RX_MASK_UNDERFLOW_Pos    (6UL)                 /*!< SCB INTR_RX_MASK: UNDERFLOW (Bit 6)                       */
#define SCB_INTR_RX_MASK_UNDERFLOW_Msk    (0x40UL)              /*!< SCB INTR_RX_MASK: UNDERFLOW (Bitfield-Mask: 0x01)         */
#define SCB_INTR_RX_MASK_BLOCKED_Pos      (7UL)                 /*!< SCB INTR_RX_MASK: BLOCKED (Bit 7)                         */
#define SCB_INTR_RX_MASK_BLOCKED_Msk      (0x80UL)              /*!< SCB INTR_RX_MASK: BLOCKED (Bitfield-Mask: 0x01)           */
#define SCB_INTR_RX_MASK_FRAME_ERROR_Pos  (8UL)                 /*!< SCB INTR_RX_MASK: FRAME_ERROR (Bit 8)                     */
#define SCB_INTR_RX_MASK_FRAME_ERROR_Msk  (0x100UL)             /*!< SCB INTR_RX_MASK: FRAME_ERROR (Bitfield-Mask: 0x01)       */
#define SCB_INTR_RX_MASK_PARITY_ERROR_Pos (9UL)                 /*!< SCB INTR_RX_MASK: PARITY_ERROR (Bit 9)                    */
#define SCB_INTR_RX_MASK_PARITY_ERROR_Msk (0x200UL)             /*!< SCB INTR_RX_MASK: PARITY_ERROR (Bitfield-Mask: 0x01)      */
#define SCB_INTR_RX_MASK_BAUD_DETECT_Pos  (10UL)                /*!< SCB INTR_RX_MASK: BAUD_DETECT (Bit 10)                    */
#define SCB_INTR_RX_MASK_BAUD_DETECT_Msk  (0x400UL)             /*!< SCB INTR_RX_MASK: BAUD_DETECT (Bitfield-Mask: 0x01)       */
#define SCB_INTR_RX_MASK_BREAK_DETECT_Pos (11UL)                /*!< SCB INTR_RX_MASK: BREAK_DETECT (Bit 11)                   */
#define SCB_INTR_RX_MASK_BREAK_DETECT_Msk (0x800UL)             /*!< SCB INTR_RX_MASK: BREAK_DETECT (Bitfield-Mask: 0x01)      */
/* ==================================================  SCB.INTR_RX_MASKED  =================================================== */
#define SCB_INTR_RX_MASKED_TRIGGER_Pos    (0UL)                 /*!< SCB INTR_RX_MASKED: TRIGGER (Bit 0)                       */
#define SCB_INTR_RX_MASKED_TRIGGER_Msk    (0x1UL)               /*!< SCB INTR_RX_MASKED: TRIGGER (Bitfield-Mask: 0x01)         */
#define SCB_INTR_RX_MASKED_NOT_EMPTY_Pos  (2UL)                 /*!< SCB INTR_RX_MASKED: NOT_EMPTY (Bit 2)                     */
#define SCB_INTR_RX_MASKED_NOT_EMPTY_Msk  (0x4UL)               /*!< SCB INTR_RX_MASKED: NOT_EMPTY (Bitfield-Mask: 0x01)       */
#define SCB_INTR_RX_MASKED_FULL_Pos       (3UL)                 /*!< SCB INTR_RX_MASKED: FULL (Bit 3)                          */
#define SCB_INTR_RX_MASKED_FULL_Msk       (0x8UL)               /*!< SCB INTR_RX_MASKED: FULL (Bitfield-Mask: 0x01)            */
#define SCB_INTR_RX_MASKED_OVERFLOW_Pos   (5UL)                 /*!< SCB INTR_RX_MASKED: OVERFLOW (Bit 5)                      */
#define SCB_INTR_RX_MASKED_OVERFLOW_Msk   (0x20UL)              /*!< SCB INTR_RX_MASKED: OVERFLOW (Bitfield-Mask: 0x01)        */
#define SCB_INTR_RX_MASKED_UNDERFLOW_Pos  (6UL)                 /*!< SCB INTR_RX_MASKED: UNDERFLOW (Bit 6)                     */
#define SCB_INTR_RX_MASKED_UNDERFLOW_Msk  (0x40UL)              /*!< SCB INTR_RX_MASKED: UNDERFLOW (Bitfield-Mask: 0x01)       */
#define SCB_INTR_RX_MASKED_BLOCKED_Pos    (7UL)                 /*!< SCB INTR_RX_MASKED: BLOCKED (Bit 7)                       */
#define SCB_INTR_RX_MASKED_BLOCKED_Msk    (0x80UL)              /*!< SCB INTR_RX_MASKED: BLOCKED (Bitfield-Mask: 0x01)         */
#define SCB_INTR_RX_MASKED_FRAME_ERROR_Pos (8UL)                /*!< SCB INTR_RX_MASKED: FRAME_ERROR (Bit 8)                   */
#define SCB_INTR_RX_MASKED_FRAME_ERROR_Msk (0x100UL)            /*!< SCB INTR_RX_MASKED: FRAME_ERROR (Bitfield-Mask: 0x01)     */
#define SCB_INTR_RX_MASKED_PARITY_ERROR_Pos (9UL)               /*!< SCB INTR_RX_MASKED: PARITY_ERROR (Bit 9)                  */
#define SCB_INTR_RX_MASKED_PARITY_ERROR_Msk (0x200UL)           /*!< SCB INTR_RX_MASKED: PARITY_ERROR (Bitfield-Mask: 0x01)    */
#define SCB_INTR_RX_MASKED_BAUD_DETECT_Pos (10UL)               /*!< SCB INTR_RX_MASKED: BAUD_DETECT (Bit 10)                  */
#define SCB_INTR_RX_MASKED_BAUD_DETECT_Msk (0x400UL)            /*!< SCB INTR_RX_MASKED: BAUD_DETECT (Bitfield-Mask: 0x01)     */
#define SCB_INTR_RX_MASKED_BREAK_DETECT_Pos (11UL)              /*!< SCB INTR_RX_MASKED: BREAK_DETECT (Bit 11)                 */
#define SCB_INTR_RX_MASKED_BREAK_DETECT_Msk (0x800UL)           /*!< SCB INTR_RX_MASKED: BREAK_DETECT (Bitfield-Mask: 0x01)    */


#endif /* _CYIP_SCB_H_ */


/* [] END OF FILE */
