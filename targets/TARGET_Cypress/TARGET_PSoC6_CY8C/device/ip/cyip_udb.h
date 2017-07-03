/***************************************************************************//**
* \file cyip_udb.h
*
* \brief
* UDB IP definitions
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

#ifndef _CYIP_UDB_H_
#define _CYIP_UDB_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                            UDB                                            ================ */
/* =========================================================================================================================== */

#define UDB_WRKONE_SECTION_SIZE     0x00000800UL
#define UDB_WRKMULT_SECTION_SIZE    0x00001000UL
#define UDB_UDBPAIR_UDBSNG_SECTION_SIZE 0x00000080UL
#define UDB_UDBPAIR_ROUTE_SECTION_SIZE 0x00000100UL
#define UDB_UDBPAIR_SECTION_SIZE    0x00000200UL
#define UDB_DSI_SECTION_SIZE        0x00000080UL
#define UDB_PA_SECTION_SIZE         0x00000040UL
#define UDB_BCTL_SECTION_SIZE       0x00000080UL
#define UDB_UDBIF_SECTION_SIZE      0x00000020UL
#define UDB_SECTION_SIZE            0x00010000UL

/**
  * \brief UDB Working Registers (2 registers from one UDB at a time) (UDB_WRKONE)
  */
typedef struct {
  __IOM uint32_t A[64];                         /*!< 0x00000000 Accumulator Registers {A1,A0}                                  */
  __IOM uint32_t D[64];                         /*!< 0x00000100 Data Registers {D1,D0}                                         */
  __IOM uint32_t F[64];                         /*!< 0x00000200 FIFOs {F1,F0}                                                  */
  __IOM uint32_t CTL_ST[64];                    /*!< 0x00000300 Status and Control Registers {CTL,ST}                          */
  __IOM uint32_t ACTL_MSK[64];                  /*!< 0x00000400 Mask and Auxiliary Control Registers {ACTL,MSK}                */
   __IM uint32_t MC[64];                        /*!< 0x00000500 PLD Macrocell Read Registers {00,MC}                           */
   __IM uint32_t RESERVED[128];
} UDB_WRKONE_Type;                              /*!< Size = 1536 (0x600)                                                       */

/**
  * \brief UDB Working Registers (1 register from multiple UDBs at a time) (UDB_WRKMULT)
  */
typedef struct {
  __IOM uint32_t A0[64];                        /*!< 0x00000000 Accumulator 0                                                  */
  __IOM uint32_t A1[64];                        /*!< 0x00000100 Accumulator 1                                                  */
  __IOM uint32_t D0[64];                        /*!< 0x00000200 Data 0                                                         */
  __IOM uint32_t D1[64];                        /*!< 0x00000300 Data 1                                                         */
  __IOM uint32_t F0[64];                        /*!< 0x00000400 FIFO 0                                                         */
  __IOM uint32_t F1[64];                        /*!< 0x00000500 FIFO 1                                                         */
   __IM uint32_t ST[64];                        /*!< 0x00000600 Status Register                                                */
  __IOM uint32_t CTL[64];                       /*!< 0x00000700 Control Register                                               */
  __IOM uint32_t MSK[64];                       /*!< 0x00000800 Interrupt Mask                                                 */
  __IOM uint32_t ACTL[64];                      /*!< 0x00000900 Auxiliary Control                                              */
   __IM uint32_t MC[64];                        /*!< 0x00000A00 PLD Macrocell reading                                          */
   __IM uint32_t RESERVED[320];
} UDB_WRKMULT_Type;                             /*!< Size = 2816 (0xB00)                                                       */

/**
  * \brief Single UDB Configuration (UDB_UDBPAIR_UDBSNG)
  */
typedef struct {
  __IOM uint32_t PLD_IT[12];                    /*!< 0x00000000 PLD Input Terms                                                */
  __IOM uint32_t PLD_ORT0;                      /*!< 0x00000030 PLD OR Terms                                                   */
  __IOM uint32_t PLD_ORT1;                      /*!< 0x00000034 PLD OR Terms                                                   */
  __IOM uint32_t PLD_CFG0;                      /*!< 0x00000038 PLD configuration for Carry Enable, Constant, and XOR feedback */
  __IOM uint32_t PLD_CFG1;                      /*!< 0x0000003C PLD configuration for Set / Reset selection, and Bypass control*/
  __IOM uint32_t DPATH_CFG0;                    /*!< 0x00000040 Datapath input selections (RAD0, RAD1, RAD2, F0_LD, F1_LD,
                                                                D0_LD, D1_LD)                                                  */
  __IOM uint32_t DPATH_CFG1;                    /*!< 0x00000044 Datapath input and output selections (SCI_MUX, SI_MUX, OUT0
                                                                thru OUT5)                                                     */
  __IOM uint32_t DPATH_CFG2;                    /*!< 0x00000048 Datapath output synchronization, ALU mask, compare 0 and 1
                                                                masks                                                          */
  __IOM uint32_t DPATH_CFG3;                    /*!< 0x0000004C Datapath mask enables, shift in, carry in, compare, chaining,
                                                                MSB configs; FIFO, shift and parallel input control            */
  __IOM uint32_t DPATH_CFG4;                    /*!< 0x00000050 Datapath FIFO and register access configuration control        */
  __IOM uint32_t SC_CFG0;                       /*!< 0x00000054 SC Mode 0 and 1 control registers; status register input mode;
                                                                general SC configuration                                       */
  __IOM uint32_t SC_CFG1;                       /*!< 0x00000058 SC counter control                                             */
  __IOM uint32_t RC_CFG0;                       /*!< 0x0000005C PLD0, PLD1, Datatpath, and SC clock and reset control          */
  __IOM uint32_t RC_CFG1;                       /*!< 0x00000060 PLD0, PLD1, Datatpath, and SC clock selection, general reset
                                                                control                                                        */
  __IOM uint32_t DPATH_OPC[4];                  /*!< 0x00000064 Datapath opcode configuration                                  */
   __IM uint32_t RESERVED[3];
} UDB_UDBPAIR_UDBSNG_Type;                      /*!< Size = 116 (0x74)                                                         */

/**
  * \brief Routing Configuration for one UDB Pair (UDB_UDBPAIR_ROUTE)
  */
typedef struct {
  __IOM uint32_t TOP_V_BOT;                     /*!< 0x00000000 See mxudb BROS 002-10714 Appendix A section 'TVI vs. BVI
                                                                Muxing'                                                        */
  __IOM uint32_t LVI1_V_2;                      /*!< 0x00000004 See mxudb BROS 002-10714 Appendix A section 'LVI / RVI for
                                                                Horizontal Muxing'                                             */
  __IOM uint32_t RVI1_V_2;                      /*!< 0x00000008 See mxudb BROS 002-10714 Appendix A section 'LVI / RVI for
                                                                Horizontal Muxing'                                             */
  __IOM uint32_t TUI_CFG0;                      /*!< 0x0000000C See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t TUI_CFG1;                      /*!< 0x00000010 See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t TUI_CFG2;                      /*!< 0x00000014 See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t TUI_CFG3;                      /*!< 0x00000018 See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t TUI_CFG4;                      /*!< 0x0000001C See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t TUI_CFG5;                      /*!< 0x00000020 See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t BUI_CFG0;                      /*!< 0x00000024 See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t BUI_CFG1;                      /*!< 0x00000028 See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t BUI_CFG2;                      /*!< 0x0000002C See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t BUI_CFG3;                      /*!< 0x00000030 See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t BUI_CFG4;                      /*!< 0x00000034 See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t BUI_CFG5;                      /*!< 0x00000038 See mxudb BROS 002-10714 Appendix A section 'TUI / BUI Muxing' */
  __IOM uint32_t RVO_CFG0;                      /*!< 0x0000003C See mxudb BROS 002-10714 Appendix A section 'RVO Muxing'       */
  __IOM uint32_t RVO_CFG1;                      /*!< 0x00000040 See mxudb BROS 002-10714 Appendix A section 'RVO Muxing'       */
  __IOM uint32_t RVO_CFG2;                      /*!< 0x00000044 See mxudb BROS 002-10714 Appendix A section 'RVO Muxing'       */
  __IOM uint32_t RVO_CFG3;                      /*!< 0x00000048 See mxudb BROS 002-10714 Appendix A section 'RVO Muxing'       */
  __IOM uint32_t LVO_CFG0;                      /*!< 0x0000004C See mxudb BROS 002-10714 Appendix A section 'LVO Muxing'       */
  __IOM uint32_t LVO_CFG1;                      /*!< 0x00000050 See mxudb BROS 002-10714 Appendix A section 'LVO Muxing'       */
  __IOM uint32_t RHO_CFG0;                      /*!< 0x00000054 See mxudb BROS 002-10714 Appendix A section 'RHO Muxing'       */
  __IOM uint32_t RHO_CFG1;                      /*!< 0x00000058 See mxudb BROS 002-10714 Appendix A section 'RHO Muxing'       */
  __IOM uint32_t RHO_CFG2;                      /*!< 0x0000005C See mxudb BROS 002-10714 Appendix A section 'RHO Muxing'       */
  __IOM uint32_t LHO_CFG0;                      /*!< 0x00000060 See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG1;                      /*!< 0x00000064 See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG2;                      /*!< 0x00000068 See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG3;                      /*!< 0x0000006C See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG4;                      /*!< 0x00000070 See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG5;                      /*!< 0x00000074 See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG6;                      /*!< 0x00000078 See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG7;                      /*!< 0x0000007C See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG8;                      /*!< 0x00000080 See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG9;                      /*!< 0x00000084 See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG10;                     /*!< 0x00000088 See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
  __IOM uint32_t LHO_CFG11;                     /*!< 0x0000008C See mxudb BROS 002-10714 Appendix A section 'LHO Muxing'       */
   __IM uint32_t RESERVED[28];
} UDB_UDBPAIR_ROUTE_Type;                       /*!< Size = 144 (0x90)                                                         */

/**
  * \brief UDB Pair Configuration (up to 32 Pairs) (UDB_UDBPAIR)
  */
typedef struct {
        UDB_UDBPAIR_UDBSNG_Type UDBSNG[2];      /*!< 0x00000000 Single UDB Configuration                                       */
        UDB_UDBPAIR_ROUTE_Type ROUTE;           /*!< 0x00000100 Routing Configuration for one UDB Pair                         */
} UDB_UDBPAIR_Type;                             /*!< Size = 512 (0x200)                                                        */

/**
  * \brief DSI Configuration (up to 32 DSI) (UDB_DSI)
  */
typedef struct {
  __IOM uint32_t LVI1_V_2;                      /*!< 0x00000000 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LVI / RVI for Horizontal Muxing' as
                                                                being equivalent                                               */
  __IOM uint32_t RVI1_V_2;                      /*!< 0x00000004 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LVI / RVI for Horizontal Muxing' as
                                                                being equivalent                                               */
  __IOM uint32_t DOP_CFG0;                      /*!< 0x00000008 See mxudb BROS 002-10714 Appendix B section 'DOP Muxing'       */
  __IOM uint32_t DOP_CFG1;                      /*!< 0x0000000C See mxudb BROS 002-10714 Appendix B section 'DOP Muxing'       */
  __IOM uint32_t DOP_CFG2;                      /*!< 0x00000010 See mxudb BROS 002-10714 Appendix B section 'DOP Muxing'       */
  __IOM uint32_t DOP_CFG3;                      /*!< 0x00000014 See mxudb BROS 002-10714 Appendix B section 'DOP Muxing'       */
  __IOM uint32_t DOT_CFG0;                      /*!< 0x00000018 See mxudb BROS 002-10714 Appendix B section 'DOT Muxing'       */
  __IOM uint32_t DOT_CFG1;                      /*!< 0x0000001C See mxudb BROS 002-10714 Appendix B section 'DOT Muxing'       */
  __IOM uint32_t DOT_CFG2;                      /*!< 0x00000020 See mxudb BROS 002-10714 Appendix B section 'DOT Muxing'       */
  __IOM uint32_t DOT_CFG3;                      /*!< 0x00000024 See mxudb BROS 002-10714 Appendix B section 'DOT Muxing'       */
  __IOM uint32_t RVO_CFG0;                      /*!< 0x00000028 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'RVO Muxing' as being equivalent         */
  __IOM uint32_t RVO_CFG1;                      /*!< 0x0000002C See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'RVO Muxing' as being equivalent         */
  __IOM uint32_t RVO_CFG2;                      /*!< 0x00000030 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'RVO Muxing' as being equivalent         */
  __IOM uint32_t RVO_CFG3;                      /*!< 0x00000034 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'RVO Muxing' as being equivalent         */
  __IOM uint32_t LVO_CFG0;                      /*!< 0x00000038 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LVO Muxing' as being equivalent         */
  __IOM uint32_t LVO_CFG1;                      /*!< 0x0000003C See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LVO Muxing' as being equivalent         */
  __IOM uint32_t RHO_CFG0;                      /*!< 0x00000040 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'RHO Muxing' as being equivalent         */
  __IOM uint32_t RHO_CFG1;                      /*!< 0x00000044 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'RHO Muxing' as being equivalent         */
  __IOM uint32_t RHO_CFG2;                      /*!< 0x00000048 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'RHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG0;                      /*!< 0x0000004C See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG1;                      /*!< 0x00000050 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG2;                      /*!< 0x00000054 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG3;                      /*!< 0x00000058 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG4;                      /*!< 0x0000005C See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG5;                      /*!< 0x00000060 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG6;                      /*!< 0x00000064 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG7;                      /*!< 0x00000068 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG8;                      /*!< 0x0000006C See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG9;                      /*!< 0x00000070 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG10;                     /*!< 0x00000074 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
  __IOM uint32_t LHO_CFG11;                     /*!< 0x00000078 See mxudb BROS 002-10714 Appendix B DSI Routing which refers
                                                                to Appendix A section 'LHO Muxing' as being equivalent         */
   __IM uint32_t RESERVED;
} UDB_DSI_Type;                                 /*!< Size = 124 (0x7C)                                                         */

/**
  * \brief Port Adapter Configuration (up to 32 PA) (UDB_PA)
  */
typedef struct {
  __IOM uint32_t CFG0;                          /*!< 0x00000000 PA Data In Clock Control Register                              */
  __IOM uint32_t CFG1;                          /*!< 0x00000004 PA Data Out Clock Control Register                             */
  __IOM uint32_t CFG2;                          /*!< 0x00000008 PA Clock Select Register                                       */
  __IOM uint32_t CFG3;                          /*!< 0x0000000C PA Reset Select Register                                       */
  __IOM uint32_t CFG4;                          /*!< 0x00000010 PA Reset Enable Register                                       */
  __IOM uint32_t CFG5;                          /*!< 0x00000014 PA Reset Pin Select Register                                   */
  __IOM uint32_t CFG6;                          /*!< 0x00000018 PA Input Data Sync Control Register - Low                      */
  __IOM uint32_t CFG7;                          /*!< 0x0000001C PA Input Data Sync Control Register - High                     */
  __IOM uint32_t CFG8;                          /*!< 0x00000020 PA Output Data Sync Control Register - Low                     */
  __IOM uint32_t CFG9;                          /*!< 0x00000024 PA Output Data Sync Control Register - High                    */
  __IOM uint32_t CFG10;                         /*!< 0x00000028 PA Output Data Select Register - Low                           */
  __IOM uint32_t CFG11;                         /*!< 0x0000002C PA Output Data Select Register - High                          */
  __IOM uint32_t CFG12;                         /*!< 0x00000030 PA OE Select Register - Low                                    */
  __IOM uint32_t CFG13;                         /*!< 0x00000034 PA OE Select Register - High                                   */
  __IOM uint32_t CFG14;                         /*!< 0x00000038 PA OE Sync Register                                            */
   __IM uint32_t RESERVED;
} UDB_PA_Type;                                  /*!< Size = 60 (0x3C)                                                          */

/**
  * \brief UDB Array Bank Control (UDB_BCTL)
  */
typedef struct {
  __IOM uint32_t MDCLK_EN;                      /*!< 0x00000000 Master Digital Clock Enable Register                           */
  __IOM uint32_t MBCLK_EN;                      /*!< 0x00000004 Master clk_peri_app Enable Register                            */
  __IOM uint32_t BOTSEL_L;                      /*!< 0x00000008 Lower Nibble Bottom Digital Clock Select Register              */
  __IOM uint32_t BOTSEL_U;                      /*!< 0x0000000C Upper Nibble Bottom Digital Clock Select Register              */
  __IOM uint32_t QCLK_EN[16];                   /*!< 0x00000010 Quadrant Digital Clock Enable Registers                        */
   __IM uint32_t RESERVED[12];
} UDB_BCTL_Type;                                /*!< Size = 80 (0x50)                                                          */

/**
  * \brief UDB Subsystem Interface Configuration (UDB_UDBIF)
  */
typedef struct {
  __IOM uint32_t BANK_CTL;                      /*!< 0x00000000 Bank Control                                                   */
  __IOM uint32_t INT_CLK_CTL;                   /*!< 0x00000004 Interrupt Clock Control                                        */
  __IOM uint32_t INT_CFG;                       /*!< 0x00000008 Interrupt Configuration                                        */
  __IOM uint32_t TR_CLK_CTL;                    /*!< 0x0000000C Trigger Clock Control                                          */
  __IOM uint32_t TR_CFG;                        /*!< 0x00000010 Trigger Configuration                                          */
  __IOM uint32_t PRIVATE;                       /*!< 0x00000014 Internal use only                                              */
   __IM uint32_t RESERVED[2];
} UDB_UDBIF_Type;                               /*!< Size = 24 (0x18)                                                          */

/**
  * \brief Programmable Digital Subsystem (UDB)
  */
typedef struct {                                /*!< UDB Structure                                                             */
        UDB_WRKONE_Type WRKONE;                 /*!< 0x00000000 UDB Working Registers (2 registers from one UDB at a time)     */
   __IM uint32_t RESERVED[512];
        UDB_WRKMULT_Type WRKMULT;               /*!< 0x00001000 UDB Working Registers (1 register from multiple UDBs at a time)*/
        UDB_UDBPAIR_Type UDBPAIR[32];           /*!< 0x00002000 UDB Pair Configuration (up to 32 Pairs)                        */
        UDB_DSI_Type DSI[32];                   /*!< 0x00006000 DSI Configuration (up to 32 DSI)                               */
        UDB_PA_Type PA[32];                     /*!< 0x00007000 Port Adapter Configuration (up to 32 PA)                       */
        UDB_BCTL_Type BCTL;                     /*!< 0x00007800 UDB Array Bank Control                                         */
   __IM uint32_t RESERVED1[32];
        UDB_UDBIF_Type UDBIF;                   /*!< 0x00007900 UDB Subsystem Interface Configuration                          */
} UDB_Type;                                     /*!< Size = 31008 (0x7920)                                                     */


/* =====================================================  UDB_WRKONE.A  ====================================================== */
#define UDB_WRKONE_A_A0_Pos               (0UL)                 /*!< UDB_WRKONE A: A0 (Bit 0)                                  */
#define UDB_WRKONE_A_A0_Msk               (0xffUL)              /*!< UDB_WRKONE A: A0 (Bitfield-Mask: 0xff)                    */
#define UDB_WRKONE_A_A1_Pos               (8UL)                 /*!< UDB_WRKONE A: A1 (Bit 8)                                  */
#define UDB_WRKONE_A_A1_Msk               (0xff00UL)            /*!< UDB_WRKONE A: A1 (Bitfield-Mask: 0xff)                    */
/* =====================================================  UDB_WRKONE.D  ====================================================== */
#define UDB_WRKONE_D_D0_Pos               (0UL)                 /*!< UDB_WRKONE D: D0 (Bit 0)                                  */
#define UDB_WRKONE_D_D0_Msk               (0xffUL)              /*!< UDB_WRKONE D: D0 (Bitfield-Mask: 0xff)                    */
#define UDB_WRKONE_D_D1_Pos               (8UL)                 /*!< UDB_WRKONE D: D1 (Bit 8)                                  */
#define UDB_WRKONE_D_D1_Msk               (0xff00UL)            /*!< UDB_WRKONE D: D1 (Bitfield-Mask: 0xff)                    */
/* =====================================================  UDB_WRKONE.F  ====================================================== */
#define UDB_WRKONE_F_F0_Pos               (0UL)                 /*!< UDB_WRKONE F: F0 (Bit 0)                                  */
#define UDB_WRKONE_F_F0_Msk               (0xffUL)              /*!< UDB_WRKONE F: F0 (Bitfield-Mask: 0xff)                    */
#define UDB_WRKONE_F_F1_Pos               (8UL)                 /*!< UDB_WRKONE F: F1 (Bit 8)                                  */
#define UDB_WRKONE_F_F1_Msk               (0xff00UL)            /*!< UDB_WRKONE F: F1 (Bitfield-Mask: 0xff)                    */
/* ===================================================  UDB_WRKONE.CTL_ST  =================================================== */
#define UDB_WRKONE_CTL_ST_ST_Pos          (0UL)                 /*!< UDB_WRKONE CTL_ST: ST (Bit 0)                             */
#define UDB_WRKONE_CTL_ST_ST_Msk          (0xffUL)              /*!< UDB_WRKONE CTL_ST: ST (Bitfield-Mask: 0xff)               */
#define UDB_WRKONE_CTL_ST_CTL_Pos         (8UL)                 /*!< UDB_WRKONE CTL_ST: CTL (Bit 8)                            */
#define UDB_WRKONE_CTL_ST_CTL_Msk         (0xff00UL)            /*!< UDB_WRKONE CTL_ST: CTL (Bitfield-Mask: 0xff)              */
/* ==================================================  UDB_WRKONE.ACTL_MSK  ================================================== */
#define UDB_WRKONE_ACTL_MSK_MSK_Pos       (0UL)                 /*!< UDB_WRKONE ACTL_MSK: MSK (Bit 0)                          */
#define UDB_WRKONE_ACTL_MSK_MSK_Msk       (0x7fUL)              /*!< UDB_WRKONE ACTL_MSK: MSK (Bitfield-Mask: 0x7f)            */
#define UDB_WRKONE_ACTL_MSK_FIFO0_CLR_Pos (8UL)                 /*!< UDB_WRKONE ACTL_MSK: FIFO0_CLR (Bit 8)                    */
#define UDB_WRKONE_ACTL_MSK_FIFO0_CLR_Msk (0x100UL)             /*!< UDB_WRKONE ACTL_MSK: FIFO0_CLR (Bitfield-Mask: 0x01)      */
#define UDB_WRKONE_ACTL_MSK_FIFO1_CLR_Pos (9UL)                 /*!< UDB_WRKONE ACTL_MSK: FIFO1_CLR (Bit 9)                    */
#define UDB_WRKONE_ACTL_MSK_FIFO1_CLR_Msk (0x200UL)             /*!< UDB_WRKONE ACTL_MSK: FIFO1_CLR (Bitfield-Mask: 0x01)      */
#define UDB_WRKONE_ACTL_MSK_FIFO0_LVL_Pos (10UL)                /*!< UDB_WRKONE ACTL_MSK: FIFO0_LVL (Bit 10)                   */
#define UDB_WRKONE_ACTL_MSK_FIFO0_LVL_Msk (0x400UL)             /*!< UDB_WRKONE ACTL_MSK: FIFO0_LVL (Bitfield-Mask: 0x01)      */
#define UDB_WRKONE_ACTL_MSK_FIFO1_LVL_Pos (11UL)                /*!< UDB_WRKONE ACTL_MSK: FIFO1_LVL (Bit 11)                   */
#define UDB_WRKONE_ACTL_MSK_FIFO1_LVL_Msk (0x800UL)             /*!< UDB_WRKONE ACTL_MSK: FIFO1_LVL (Bitfield-Mask: 0x01)      */
#define UDB_WRKONE_ACTL_MSK_INT_EN_Pos    (12UL)                /*!< UDB_WRKONE ACTL_MSK: INT_EN (Bit 12)                      */
#define UDB_WRKONE_ACTL_MSK_INT_EN_Msk    (0x1000UL)            /*!< UDB_WRKONE ACTL_MSK: INT_EN (Bitfield-Mask: 0x01)         */
#define UDB_WRKONE_ACTL_MSK_CNT_START_Pos (13UL)                /*!< UDB_WRKONE ACTL_MSK: CNT_START (Bit 13)                   */
#define UDB_WRKONE_ACTL_MSK_CNT_START_Msk (0x2000UL)            /*!< UDB_WRKONE ACTL_MSK: CNT_START (Bitfield-Mask: 0x01)      */
/* =====================================================  UDB_WRKONE.MC  ===================================================== */
#define UDB_WRKONE_MC_PLD0_MC_Pos         (0UL)                 /*!< UDB_WRKONE MC: PLD0_MC (Bit 0)                            */
#define UDB_WRKONE_MC_PLD0_MC_Msk         (0xfUL)               /*!< UDB_WRKONE MC: PLD0_MC (Bitfield-Mask: 0x0f)              */
#define UDB_WRKONE_MC_PLD1_MC_Pos         (4UL)                 /*!< UDB_WRKONE MC: PLD1_MC (Bit 4)                            */
#define UDB_WRKONE_MC_PLD1_MC_Msk         (0xf0UL)              /*!< UDB_WRKONE MC: PLD1_MC (Bitfield-Mask: 0x0f)              */


/* ====================================================  UDB_WRKMULT.A0  ===================================================== */
#define UDB_WRKMULT_A0_A0_0_Pos           (0UL)                 /*!< UDB_WRKMULT A0: A0_0 (Bit 0)                              */
#define UDB_WRKMULT_A0_A0_0_Msk           (0xffUL)              /*!< UDB_WRKMULT A0: A0_0 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_A0_A0_1_Pos           (8UL)                 /*!< UDB_WRKMULT A0: A0_1 (Bit 8)                              */
#define UDB_WRKMULT_A0_A0_1_Msk           (0xff00UL)            /*!< UDB_WRKMULT A0: A0_1 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_A0_A0_2_Pos           (16UL)                /*!< UDB_WRKMULT A0: A0_2 (Bit 16)                             */
#define UDB_WRKMULT_A0_A0_2_Msk           (0xff0000UL)          /*!< UDB_WRKMULT A0: A0_2 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_A0_A0_3_Pos           (24UL)                /*!< UDB_WRKMULT A0: A0_3 (Bit 24)                             */
#define UDB_WRKMULT_A0_A0_3_Msk           (0xff000000UL)        /*!< UDB_WRKMULT A0: A0_3 (Bitfield-Mask: 0xff)                */
/* ====================================================  UDB_WRKMULT.A1  ===================================================== */
#define UDB_WRKMULT_A1_A1_0_Pos           (0UL)                 /*!< UDB_WRKMULT A1: A1_0 (Bit 0)                              */
#define UDB_WRKMULT_A1_A1_0_Msk           (0xffUL)              /*!< UDB_WRKMULT A1: A1_0 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_A1_A1_1_Pos           (8UL)                 /*!< UDB_WRKMULT A1: A1_1 (Bit 8)                              */
#define UDB_WRKMULT_A1_A1_1_Msk           (0xff00UL)            /*!< UDB_WRKMULT A1: A1_1 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_A1_A1_2_Pos           (16UL)                /*!< UDB_WRKMULT A1: A1_2 (Bit 16)                             */
#define UDB_WRKMULT_A1_A1_2_Msk           (0xff0000UL)          /*!< UDB_WRKMULT A1: A1_2 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_A1_A1_3_Pos           (24UL)                /*!< UDB_WRKMULT A1: A1_3 (Bit 24)                             */
#define UDB_WRKMULT_A1_A1_3_Msk           (0xff000000UL)        /*!< UDB_WRKMULT A1: A1_3 (Bitfield-Mask: 0xff)                */
/* ====================================================  UDB_WRKMULT.D0  ===================================================== */
#define UDB_WRKMULT_D0_D0_0_Pos           (0UL)                 /*!< UDB_WRKMULT D0: D0_0 (Bit 0)                              */
#define UDB_WRKMULT_D0_D0_0_Msk           (0xffUL)              /*!< UDB_WRKMULT D0: D0_0 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_D0_D0_1_Pos           (8UL)                 /*!< UDB_WRKMULT D0: D0_1 (Bit 8)                              */
#define UDB_WRKMULT_D0_D0_1_Msk           (0xff00UL)            /*!< UDB_WRKMULT D0: D0_1 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_D0_D0_2_Pos           (16UL)                /*!< UDB_WRKMULT D0: D0_2 (Bit 16)                             */
#define UDB_WRKMULT_D0_D0_2_Msk           (0xff0000UL)          /*!< UDB_WRKMULT D0: D0_2 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_D0_D0_3_Pos           (24UL)                /*!< UDB_WRKMULT D0: D0_3 (Bit 24)                             */
#define UDB_WRKMULT_D0_D0_3_Msk           (0xff000000UL)        /*!< UDB_WRKMULT D0: D0_3 (Bitfield-Mask: 0xff)                */
/* ====================================================  UDB_WRKMULT.D1  ===================================================== */
#define UDB_WRKMULT_D1_D1_0_Pos           (0UL)                 /*!< UDB_WRKMULT D1: D1_0 (Bit 0)                              */
#define UDB_WRKMULT_D1_D1_0_Msk           (0xffUL)              /*!< UDB_WRKMULT D1: D1_0 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_D1_D1_1_Pos           (8UL)                 /*!< UDB_WRKMULT D1: D1_1 (Bit 8)                              */
#define UDB_WRKMULT_D1_D1_1_Msk           (0xff00UL)            /*!< UDB_WRKMULT D1: D1_1 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_D1_D1_2_Pos           (16UL)                /*!< UDB_WRKMULT D1: D1_2 (Bit 16)                             */
#define UDB_WRKMULT_D1_D1_2_Msk           (0xff0000UL)          /*!< UDB_WRKMULT D1: D1_2 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_D1_D1_3_Pos           (24UL)                /*!< UDB_WRKMULT D1: D1_3 (Bit 24)                             */
#define UDB_WRKMULT_D1_D1_3_Msk           (0xff000000UL)        /*!< UDB_WRKMULT D1: D1_3 (Bitfield-Mask: 0xff)                */
/* ====================================================  UDB_WRKMULT.F0  ===================================================== */
#define UDB_WRKMULT_F0_F0_0_Pos           (0UL)                 /*!< UDB_WRKMULT F0: F0_0 (Bit 0)                              */
#define UDB_WRKMULT_F0_F0_0_Msk           (0xffUL)              /*!< UDB_WRKMULT F0: F0_0 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_F0_F0_1_Pos           (8UL)                 /*!< UDB_WRKMULT F0: F0_1 (Bit 8)                              */
#define UDB_WRKMULT_F0_F0_1_Msk           (0xff00UL)            /*!< UDB_WRKMULT F0: F0_1 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_F0_F0_2_Pos           (16UL)                /*!< UDB_WRKMULT F0: F0_2 (Bit 16)                             */
#define UDB_WRKMULT_F0_F0_2_Msk           (0xff0000UL)          /*!< UDB_WRKMULT F0: F0_2 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_F0_F0_3_Pos           (24UL)                /*!< UDB_WRKMULT F0: F0_3 (Bit 24)                             */
#define UDB_WRKMULT_F0_F0_3_Msk           (0xff000000UL)        /*!< UDB_WRKMULT F0: F0_3 (Bitfield-Mask: 0xff)                */
/* ====================================================  UDB_WRKMULT.F1  ===================================================== */
#define UDB_WRKMULT_F1_F1_0_Pos           (0UL)                 /*!< UDB_WRKMULT F1: F1_0 (Bit 0)                              */
#define UDB_WRKMULT_F1_F1_0_Msk           (0xffUL)              /*!< UDB_WRKMULT F1: F1_0 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_F1_F1_1_Pos           (8UL)                 /*!< UDB_WRKMULT F1: F1_1 (Bit 8)                              */
#define UDB_WRKMULT_F1_F1_1_Msk           (0xff00UL)            /*!< UDB_WRKMULT F1: F1_1 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_F1_F1_2_Pos           (16UL)                /*!< UDB_WRKMULT F1: F1_2 (Bit 16)                             */
#define UDB_WRKMULT_F1_F1_2_Msk           (0xff0000UL)          /*!< UDB_WRKMULT F1: F1_2 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_F1_F1_3_Pos           (24UL)                /*!< UDB_WRKMULT F1: F1_3 (Bit 24)                             */
#define UDB_WRKMULT_F1_F1_3_Msk           (0xff000000UL)        /*!< UDB_WRKMULT F1: F1_3 (Bitfield-Mask: 0xff)                */
/* ====================================================  UDB_WRKMULT.ST  ===================================================== */
#define UDB_WRKMULT_ST_ST_0_Pos           (0UL)                 /*!< UDB_WRKMULT ST: ST_0 (Bit 0)                              */
#define UDB_WRKMULT_ST_ST_0_Msk           (0xffUL)              /*!< UDB_WRKMULT ST: ST_0 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_ST_ST_1_Pos           (8UL)                 /*!< UDB_WRKMULT ST: ST_1 (Bit 8)                              */
#define UDB_WRKMULT_ST_ST_1_Msk           (0xff00UL)            /*!< UDB_WRKMULT ST: ST_1 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_ST_ST_2_Pos           (16UL)                /*!< UDB_WRKMULT ST: ST_2 (Bit 16)                             */
#define UDB_WRKMULT_ST_ST_2_Msk           (0xff0000UL)          /*!< UDB_WRKMULT ST: ST_2 (Bitfield-Mask: 0xff)                */
#define UDB_WRKMULT_ST_ST_3_Pos           (24UL)                /*!< UDB_WRKMULT ST: ST_3 (Bit 24)                             */
#define UDB_WRKMULT_ST_ST_3_Msk           (0xff000000UL)        /*!< UDB_WRKMULT ST: ST_3 (Bitfield-Mask: 0xff)                */
/* ====================================================  UDB_WRKMULT.CTL  ==================================================== */
#define UDB_WRKMULT_CTL_CTL_0_Pos         (0UL)                 /*!< UDB_WRKMULT CTL: CTL_0 (Bit 0)                            */
#define UDB_WRKMULT_CTL_CTL_0_Msk         (0xffUL)              /*!< UDB_WRKMULT CTL: CTL_0 (Bitfield-Mask: 0xff)              */
#define UDB_WRKMULT_CTL_CTL_1_Pos         (8UL)                 /*!< UDB_WRKMULT CTL: CTL_1 (Bit 8)                            */
#define UDB_WRKMULT_CTL_CTL_1_Msk         (0xff00UL)            /*!< UDB_WRKMULT CTL: CTL_1 (Bitfield-Mask: 0xff)              */
#define UDB_WRKMULT_CTL_CTL_2_Pos         (16UL)                /*!< UDB_WRKMULT CTL: CTL_2 (Bit 16)                           */
#define UDB_WRKMULT_CTL_CTL_2_Msk         (0xff0000UL)          /*!< UDB_WRKMULT CTL: CTL_2 (Bitfield-Mask: 0xff)              */
#define UDB_WRKMULT_CTL_CTL_3_Pos         (24UL)                /*!< UDB_WRKMULT CTL: CTL_3 (Bit 24)                           */
#define UDB_WRKMULT_CTL_CTL_3_Msk         (0xff000000UL)        /*!< UDB_WRKMULT CTL: CTL_3 (Bitfield-Mask: 0xff)              */
/* ====================================================  UDB_WRKMULT.MSK  ==================================================== */
#define UDB_WRKMULT_MSK_MSK_0_Pos         (0UL)                 /*!< UDB_WRKMULT MSK: MSK_0 (Bit 0)                            */
#define UDB_WRKMULT_MSK_MSK_0_Msk         (0x7fUL)              /*!< UDB_WRKMULT MSK: MSK_0 (Bitfield-Mask: 0x7f)              */
#define UDB_WRKMULT_MSK_MSK_1_Pos         (8UL)                 /*!< UDB_WRKMULT MSK: MSK_1 (Bit 8)                            */
#define UDB_WRKMULT_MSK_MSK_1_Msk         (0x7f00UL)            /*!< UDB_WRKMULT MSK: MSK_1 (Bitfield-Mask: 0x7f)              */
#define UDB_WRKMULT_MSK_MSK_2_Pos         (16UL)                /*!< UDB_WRKMULT MSK: MSK_2 (Bit 16)                           */
#define UDB_WRKMULT_MSK_MSK_2_Msk         (0x7f0000UL)          /*!< UDB_WRKMULT MSK: MSK_2 (Bitfield-Mask: 0x7f)              */
#define UDB_WRKMULT_MSK_MSK_3_Pos         (24UL)                /*!< UDB_WRKMULT MSK: MSK_3 (Bit 24)                           */
#define UDB_WRKMULT_MSK_MSK_3_Msk         (0x7f000000UL)        /*!< UDB_WRKMULT MSK: MSK_3 (Bitfield-Mask: 0x7f)              */
/* ===================================================  UDB_WRKMULT.ACTL  ==================================================== */
#define UDB_WRKMULT_ACTL_FIFO0_CLR_0_Pos  (0UL)                 /*!< UDB_WRKMULT ACTL: FIFO0_CLR_0 (Bit 0)                     */
#define UDB_WRKMULT_ACTL_FIFO0_CLR_0_Msk  (0x1UL)               /*!< UDB_WRKMULT ACTL: FIFO0_CLR_0 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO1_CLR_0_Pos  (1UL)                 /*!< UDB_WRKMULT ACTL: FIFO1_CLR_0 (Bit 1)                     */
#define UDB_WRKMULT_ACTL_FIFO1_CLR_0_Msk  (0x2UL)               /*!< UDB_WRKMULT ACTL: FIFO1_CLR_0 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO0_LVL_0_Pos  (2UL)                 /*!< UDB_WRKMULT ACTL: FIFO0_LVL_0 (Bit 2)                     */
#define UDB_WRKMULT_ACTL_FIFO0_LVL_0_Msk  (0x4UL)               /*!< UDB_WRKMULT ACTL: FIFO0_LVL_0 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO1_LVL_0_Pos  (3UL)                 /*!< UDB_WRKMULT ACTL: FIFO1_LVL_0 (Bit 3)                     */
#define UDB_WRKMULT_ACTL_FIFO1_LVL_0_Msk  (0x8UL)               /*!< UDB_WRKMULT ACTL: FIFO1_LVL_0 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_INT_EN_0_Pos     (4UL)                 /*!< UDB_WRKMULT ACTL: INT_EN_0 (Bit 4)                        */
#define UDB_WRKMULT_ACTL_INT_EN_0_Msk     (0x10UL)              /*!< UDB_WRKMULT ACTL: INT_EN_0 (Bitfield-Mask: 0x01)          */
#define UDB_WRKMULT_ACTL_CNT_START_0_Pos  (5UL)                 /*!< UDB_WRKMULT ACTL: CNT_START_0 (Bit 5)                     */
#define UDB_WRKMULT_ACTL_CNT_START_0_Msk  (0x20UL)              /*!< UDB_WRKMULT ACTL: CNT_START_0 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO0_CLR_1_Pos  (8UL)                 /*!< UDB_WRKMULT ACTL: FIFO0_CLR_1 (Bit 8)                     */
#define UDB_WRKMULT_ACTL_FIFO0_CLR_1_Msk  (0x100UL)             /*!< UDB_WRKMULT ACTL: FIFO0_CLR_1 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO1_CLR_1_Pos  (9UL)                 /*!< UDB_WRKMULT ACTL: FIFO1_CLR_1 (Bit 9)                     */
#define UDB_WRKMULT_ACTL_FIFO1_CLR_1_Msk  (0x200UL)             /*!< UDB_WRKMULT ACTL: FIFO1_CLR_1 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO0_LVL_1_Pos  (10UL)                /*!< UDB_WRKMULT ACTL: FIFO0_LVL_1 (Bit 10)                    */
#define UDB_WRKMULT_ACTL_FIFO0_LVL_1_Msk  (0x400UL)             /*!< UDB_WRKMULT ACTL: FIFO0_LVL_1 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO1_LVL_1_Pos  (11UL)                /*!< UDB_WRKMULT ACTL: FIFO1_LVL_1 (Bit 11)                    */
#define UDB_WRKMULT_ACTL_FIFO1_LVL_1_Msk  (0x800UL)             /*!< UDB_WRKMULT ACTL: FIFO1_LVL_1 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_INT_EN_1_Pos     (12UL)                /*!< UDB_WRKMULT ACTL: INT_EN_1 (Bit 12)                       */
#define UDB_WRKMULT_ACTL_INT_EN_1_Msk     (0x1000UL)            /*!< UDB_WRKMULT ACTL: INT_EN_1 (Bitfield-Mask: 0x01)          */
#define UDB_WRKMULT_ACTL_CNT_START_1_Pos  (13UL)                /*!< UDB_WRKMULT ACTL: CNT_START_1 (Bit 13)                    */
#define UDB_WRKMULT_ACTL_CNT_START_1_Msk  (0x2000UL)            /*!< UDB_WRKMULT ACTL: CNT_START_1 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO0_CLR_2_Pos  (16UL)                /*!< UDB_WRKMULT ACTL: FIFO0_CLR_2 (Bit 16)                    */
#define UDB_WRKMULT_ACTL_FIFO0_CLR_2_Msk  (0x10000UL)           /*!< UDB_WRKMULT ACTL: FIFO0_CLR_2 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO1_CLR_2_Pos  (17UL)                /*!< UDB_WRKMULT ACTL: FIFO1_CLR_2 (Bit 17)                    */
#define UDB_WRKMULT_ACTL_FIFO1_CLR_2_Msk  (0x20000UL)           /*!< UDB_WRKMULT ACTL: FIFO1_CLR_2 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO0_LVL_2_Pos  (18UL)                /*!< UDB_WRKMULT ACTL: FIFO0_LVL_2 (Bit 18)                    */
#define UDB_WRKMULT_ACTL_FIFO0_LVL_2_Msk  (0x40000UL)           /*!< UDB_WRKMULT ACTL: FIFO0_LVL_2 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO1_LVL_2_Pos  (19UL)                /*!< UDB_WRKMULT ACTL: FIFO1_LVL_2 (Bit 19)                    */
#define UDB_WRKMULT_ACTL_FIFO1_LVL_2_Msk  (0x80000UL)           /*!< UDB_WRKMULT ACTL: FIFO1_LVL_2 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_INT_EN_2_Pos     (20UL)                /*!< UDB_WRKMULT ACTL: INT_EN_2 (Bit 20)                       */
#define UDB_WRKMULT_ACTL_INT_EN_2_Msk     (0x100000UL)          /*!< UDB_WRKMULT ACTL: INT_EN_2 (Bitfield-Mask: 0x01)          */
#define UDB_WRKMULT_ACTL_CNT_START_2_Pos  (21UL)                /*!< UDB_WRKMULT ACTL: CNT_START_2 (Bit 21)                    */
#define UDB_WRKMULT_ACTL_CNT_START_2_Msk  (0x200000UL)          /*!< UDB_WRKMULT ACTL: CNT_START_2 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO0_CLR_3_Pos  (24UL)                /*!< UDB_WRKMULT ACTL: FIFO0_CLR_3 (Bit 24)                    */
#define UDB_WRKMULT_ACTL_FIFO0_CLR_3_Msk  (0x1000000UL)         /*!< UDB_WRKMULT ACTL: FIFO0_CLR_3 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO1_CLR_3_Pos  (25UL)                /*!< UDB_WRKMULT ACTL: FIFO1_CLR_3 (Bit 25)                    */
#define UDB_WRKMULT_ACTL_FIFO1_CLR_3_Msk  (0x2000000UL)         /*!< UDB_WRKMULT ACTL: FIFO1_CLR_3 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO0_LVL_3_Pos  (26UL)                /*!< UDB_WRKMULT ACTL: FIFO0_LVL_3 (Bit 26)                    */
#define UDB_WRKMULT_ACTL_FIFO0_LVL_3_Msk  (0x4000000UL)         /*!< UDB_WRKMULT ACTL: FIFO0_LVL_3 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_FIFO1_LVL_3_Pos  (27UL)                /*!< UDB_WRKMULT ACTL: FIFO1_LVL_3 (Bit 27)                    */
#define UDB_WRKMULT_ACTL_FIFO1_LVL_3_Msk  (0x8000000UL)         /*!< UDB_WRKMULT ACTL: FIFO1_LVL_3 (Bitfield-Mask: 0x01)       */
#define UDB_WRKMULT_ACTL_INT_EN_3_Pos     (28UL)                /*!< UDB_WRKMULT ACTL: INT_EN_3 (Bit 28)                       */
#define UDB_WRKMULT_ACTL_INT_EN_3_Msk     (0x10000000UL)        /*!< UDB_WRKMULT ACTL: INT_EN_3 (Bitfield-Mask: 0x01)          */
#define UDB_WRKMULT_ACTL_CNT_START_3_Pos  (29UL)                /*!< UDB_WRKMULT ACTL: CNT_START_3 (Bit 29)                    */
#define UDB_WRKMULT_ACTL_CNT_START_3_Msk  (0x20000000UL)        /*!< UDB_WRKMULT ACTL: CNT_START_3 (Bitfield-Mask: 0x01)       */
/* ====================================================  UDB_WRKMULT.MC  ===================================================== */
#define UDB_WRKMULT_MC_PLD0_MC_0_Pos      (0UL)                 /*!< UDB_WRKMULT MC: PLD0_MC_0 (Bit 0)                         */
#define UDB_WRKMULT_MC_PLD0_MC_0_Msk      (0xfUL)               /*!< UDB_WRKMULT MC: PLD0_MC_0 (Bitfield-Mask: 0x0f)           */
#define UDB_WRKMULT_MC_PLD1_MC_0_Pos      (4UL)                 /*!< UDB_WRKMULT MC: PLD1_MC_0 (Bit 4)                         */
#define UDB_WRKMULT_MC_PLD1_MC_0_Msk      (0xf0UL)              /*!< UDB_WRKMULT MC: PLD1_MC_0 (Bitfield-Mask: 0x0f)           */
#define UDB_WRKMULT_MC_PLD0_MC_1_Pos      (8UL)                 /*!< UDB_WRKMULT MC: PLD0_MC_1 (Bit 8)                         */
#define UDB_WRKMULT_MC_PLD0_MC_1_Msk      (0xf00UL)             /*!< UDB_WRKMULT MC: PLD0_MC_1 (Bitfield-Mask: 0x0f)           */
#define UDB_WRKMULT_MC_PLD1_MC_1_Pos      (12UL)                /*!< UDB_WRKMULT MC: PLD1_MC_1 (Bit 12)                        */
#define UDB_WRKMULT_MC_PLD1_MC_1_Msk      (0xf000UL)            /*!< UDB_WRKMULT MC: PLD1_MC_1 (Bitfield-Mask: 0x0f)           */
#define UDB_WRKMULT_MC_PLD0_MC_2_Pos      (16UL)                /*!< UDB_WRKMULT MC: PLD0_MC_2 (Bit 16)                        */
#define UDB_WRKMULT_MC_PLD0_MC_2_Msk      (0xf0000UL)           /*!< UDB_WRKMULT MC: PLD0_MC_2 (Bitfield-Mask: 0x0f)           */
#define UDB_WRKMULT_MC_PLD1_MC_2_Pos      (20UL)                /*!< UDB_WRKMULT MC: PLD1_MC_2 (Bit 20)                        */
#define UDB_WRKMULT_MC_PLD1_MC_2_Msk      (0xf00000UL)          /*!< UDB_WRKMULT MC: PLD1_MC_2 (Bitfield-Mask: 0x0f)           */
#define UDB_WRKMULT_MC_PLD0_MC_3_Pos      (24UL)                /*!< UDB_WRKMULT MC: PLD0_MC_3 (Bit 24)                        */
#define UDB_WRKMULT_MC_PLD0_MC_3_Msk      (0xf000000UL)         /*!< UDB_WRKMULT MC: PLD0_MC_3 (Bitfield-Mask: 0x0f)           */
#define UDB_WRKMULT_MC_PLD1_MC_3_Pos      (28UL)                /*!< UDB_WRKMULT MC: PLD1_MC_3 (Bit 28)                        */
#define UDB_WRKMULT_MC_PLD1_MC_3_Msk      (0xf0000000UL)        /*!< UDB_WRKMULT MC: PLD1_MC_3 (Bitfield-Mask: 0x0f)           */


/* ===============================================  UDB_UDBPAIR_UDBSNG.PLD_IT  =============================================== */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT0_Pos (0UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT0 (Bit 0)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT0_Msk (0x1UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT1_Pos (1UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT1 (Bit 1)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT1_Msk (0x2UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT2_Pos (2UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT2 (Bit 2)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT2_Msk (0x4UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT3_Pos (3UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT3 (Bit 3)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT3_Msk (0x8UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT4_Pos (4UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT4 (Bit 4)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT4_Msk (0x10UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT4 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT5_Pos (5UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT5 (Bit 5)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT5_Msk (0x20UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT5 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT6_Pos (6UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT6 (Bit 6)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT6_Msk (0x40UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT6 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT7_Pos (7UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT7 (Bit 7)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_C_FOR_PT7_Msk (0x80UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_C_FOR_PT7 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT0_Pos (8UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT0 (Bit 8)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT0_Msk (0x100UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT1_Pos (9UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT1 (Bit 9)     */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT1_Msk (0x200UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT2_Pos (10UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT2 (Bit 10)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT2_Msk (0x400UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT3_Pos (11UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT3 (Bit 11)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT3_Msk (0x800UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT4_Pos (12UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT4 (Bit 12)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT4_Msk (0x1000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT4 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT5_Pos (13UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT5 (Bit 13)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT5_Msk (0x2000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT5 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT6_Pos (14UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT6 (Bit 14)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT6_Msk (0x4000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT6 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT7_Pos (15UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT7 (Bit 15)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_C_FOR_PT7_Msk (0x8000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_C_FOR_PT7 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT0_Pos (16UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT0 (Bit 16)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT0_Msk (0x10000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT1_Pos (17UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT1 (Bit 17)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT1_Msk (0x20000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT2_Pos (18UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT2 (Bit 18)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT2_Msk (0x40000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT3_Pos (19UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT3 (Bit 19)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT3_Msk (0x80000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT4_Pos (20UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT4 (Bit 20)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT4_Msk (0x100000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT4 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT5_Pos (21UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT5 (Bit 21)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT5_Msk (0x200000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT5 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT6_Pos (22UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT6 (Bit 22)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT6_Msk (0x400000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT6 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT7_Pos (23UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT7 (Bit 23)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD0_INX_T_FOR_PT7_Msk (0x800000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD0_INX_T_FOR_PT7 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT0_Pos (24UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT0 (Bit 24)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT0_Msk (0x1000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT1_Pos (25UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT1 (Bit 25)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT1_Msk (0x2000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT2_Pos (26UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT2 (Bit 26)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT2_Msk (0x4000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT3_Pos (27UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT3 (Bit 27)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT3_Msk (0x8000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT4_Pos (28UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT4 (Bit 28)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT4_Msk (0x10000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT4 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT5_Pos (29UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT5 (Bit 29)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT5_Msk (0x20000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT5 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT6_Pos (30UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT6 (Bit 30)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT6_Msk (0x40000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT6 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT7_Pos (31UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT7 (Bit 31)    */
#define UDB_UDBPAIR_UDBSNG_PLD_IT_PLD1_INX_T_FOR_PT7_Msk (0x80000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_IT: PLD1_INX_T_FOR_PT7 (Bitfield-Mask: 0x01)*/
/* ==============================================  UDB_UDBPAIR_UDBSNG.PLD_ORT0  ============================================== */
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT0_T_FOR_OUT0_Pos (0UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT0_T_FOR_OUT0 (Bit 0)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT0_T_FOR_OUT0_Msk (0x1UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT0_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT1_T_FOR_OUT0_Pos (1UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT1_T_FOR_OUT0 (Bit 1)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT1_T_FOR_OUT0_Msk (0x2UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT1_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT2_T_FOR_OUT0_Pos (2UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT2_T_FOR_OUT0 (Bit 2)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT2_T_FOR_OUT0_Msk (0x4UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT2_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT3_T_FOR_OUT0_Pos (3UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT3_T_FOR_OUT0 (Bit 3)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT3_T_FOR_OUT0_Msk (0x8UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT3_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT4_T_FOR_OUT0_Pos (4UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT4_T_FOR_OUT0 (Bit 4)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT4_T_FOR_OUT0_Msk (0x10UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT4_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT5_T_FOR_OUT0_Pos (5UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT5_T_FOR_OUT0 (Bit 5)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT5_T_FOR_OUT0_Msk (0x20UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT5_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT6_T_FOR_OUT0_Pos (6UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT6_T_FOR_OUT0 (Bit 6)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT6_T_FOR_OUT0_Msk (0x40UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT6_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT7_T_FOR_OUT0_Pos (7UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT7_T_FOR_OUT0 (Bit 7)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT7_T_FOR_OUT0_Msk (0x80UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT7_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT0_T_FOR_OUT0_Pos (8UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT0_T_FOR_OUT0 (Bit 8)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT0_T_FOR_OUT0_Msk (0x100UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT0_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT1_T_FOR_OUT0_Pos (9UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT1_T_FOR_OUT0 (Bit 9)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT1_T_FOR_OUT0_Msk (0x200UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT1_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT2_T_FOR_OUT0_Pos (10UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT2_T_FOR_OUT0 (Bit 10)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT2_T_FOR_OUT0_Msk (0x400UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT2_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT3_T_FOR_OUT0_Pos (11UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT3_T_FOR_OUT0 (Bit 11)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT3_T_FOR_OUT0_Msk (0x800UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT3_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT4_T_FOR_OUT0_Pos (12UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT4_T_FOR_OUT0 (Bit 12)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT4_T_FOR_OUT0_Msk (0x1000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT4_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT5_T_FOR_OUT0_Pos (13UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT5_T_FOR_OUT0 (Bit 13)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT5_T_FOR_OUT0_Msk (0x2000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT5_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT6_T_FOR_OUT0_Pos (14UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT6_T_FOR_OUT0 (Bit 14)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT6_T_FOR_OUT0_Msk (0x4000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT6_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT7_T_FOR_OUT0_Pos (15UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT7_T_FOR_OUT0 (Bit 15)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT7_T_FOR_OUT0_Msk (0x8000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT7_T_FOR_OUT0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT0_T_FOR_OUT1_Pos (16UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT0_T_FOR_OUT1 (Bit 16)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT0_T_FOR_OUT1_Msk (0x10000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT0_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT1_T_FOR_OUT1_Pos (17UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT1_T_FOR_OUT1 (Bit 17)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT1_T_FOR_OUT1_Msk (0x20000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT1_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT2_T_FOR_OUT1_Pos (18UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT2_T_FOR_OUT1 (Bit 18)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT2_T_FOR_OUT1_Msk (0x40000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT2_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT3_T_FOR_OUT1_Pos (19UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT3_T_FOR_OUT1 (Bit 19)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT3_T_FOR_OUT1_Msk (0x80000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT3_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT4_T_FOR_OUT1_Pos (20UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT4_T_FOR_OUT1 (Bit 20)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT4_T_FOR_OUT1_Msk (0x100000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT4_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT5_T_FOR_OUT1_Pos (21UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT5_T_FOR_OUT1 (Bit 21)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT5_T_FOR_OUT1_Msk (0x200000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT5_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT6_T_FOR_OUT1_Pos (22UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT6_T_FOR_OUT1 (Bit 22)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT6_T_FOR_OUT1_Msk (0x400000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT6_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT7_T_FOR_OUT1_Pos (23UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT7_T_FOR_OUT1 (Bit 23)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD0_PT7_T_FOR_OUT1_Msk (0x800000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD0_PT7_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT0_T_FOR_OUT1_Pos (24UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT0_T_FOR_OUT1 (Bit 24)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT0_T_FOR_OUT1_Msk (0x1000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT0_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT1_T_FOR_OUT1_Pos (25UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT1_T_FOR_OUT1 (Bit 25)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT1_T_FOR_OUT1_Msk (0x2000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT1_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT2_T_FOR_OUT1_Pos (26UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT2_T_FOR_OUT1 (Bit 26)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT2_T_FOR_OUT1_Msk (0x4000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT2_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT3_T_FOR_OUT1_Pos (27UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT3_T_FOR_OUT1 (Bit 27)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT3_T_FOR_OUT1_Msk (0x8000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT3_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT4_T_FOR_OUT1_Pos (28UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT4_T_FOR_OUT1 (Bit 28)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT4_T_FOR_OUT1_Msk (0x10000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT4_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT5_T_FOR_OUT1_Pos (29UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT5_T_FOR_OUT1 (Bit 29)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT5_T_FOR_OUT1_Msk (0x20000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT5_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT6_T_FOR_OUT1_Pos (30UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT6_T_FOR_OUT1 (Bit 30)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT6_T_FOR_OUT1_Msk (0x40000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT6_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT7_T_FOR_OUT1_Pos (31UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT7_T_FOR_OUT1 (Bit 31)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT0_PLD1_PT7_T_FOR_OUT1_Msk (0x80000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT0: PLD1_PT7_T_FOR_OUT1 (Bitfield-Mask: 0x01)*/
/* ==============================================  UDB_UDBPAIR_UDBSNG.PLD_ORT1  ============================================== */
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT0_T_FOR_OUT2_Pos (0UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT0_T_FOR_OUT2 (Bit 0)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT0_T_FOR_OUT2_Msk (0x1UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT0_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT1_T_FOR_OUT2_Pos (1UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT1_T_FOR_OUT2 (Bit 1)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT1_T_FOR_OUT2_Msk (0x2UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT1_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT2_T_FOR_OUT2_Pos (2UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT2_T_FOR_OUT2 (Bit 2)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT2_T_FOR_OUT2_Msk (0x4UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT2_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT3_T_FOR_OUT2_Pos (3UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT3_T_FOR_OUT2 (Bit 3)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT3_T_FOR_OUT2_Msk (0x8UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT3_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT4_T_FOR_OUT2_Pos (4UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT4_T_FOR_OUT2 (Bit 4)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT4_T_FOR_OUT2_Msk (0x10UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT4_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT5_T_FOR_OUT2_Pos (5UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT5_T_FOR_OUT2 (Bit 5)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT5_T_FOR_OUT2_Msk (0x20UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT5_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT6_T_FOR_OUT2_Pos (6UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT6_T_FOR_OUT2 (Bit 6)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT6_T_FOR_OUT2_Msk (0x40UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT6_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT7_T_FOR_OUT2_Pos (7UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT7_T_FOR_OUT2 (Bit 7)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT7_T_FOR_OUT2_Msk (0x80UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT7_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT0_T_FOR_OUT2_Pos (8UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT0_T_FOR_OUT2 (Bit 8)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT0_T_FOR_OUT2_Msk (0x100UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT0_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT1_T_FOR_OUT2_Pos (9UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT1_T_FOR_OUT2 (Bit 9)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT1_T_FOR_OUT2_Msk (0x200UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT1_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT2_T_FOR_OUT2_Pos (10UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT2_T_FOR_OUT2 (Bit 10)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT2_T_FOR_OUT2_Msk (0x400UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT2_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT3_T_FOR_OUT2_Pos (11UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT3_T_FOR_OUT2 (Bit 11)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT3_T_FOR_OUT2_Msk (0x800UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT3_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT4_T_FOR_OUT2_Pos (12UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT4_T_FOR_OUT2 (Bit 12)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT4_T_FOR_OUT2_Msk (0x1000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT4_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT5_T_FOR_OUT2_Pos (13UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT5_T_FOR_OUT2 (Bit 13)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT5_T_FOR_OUT2_Msk (0x2000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT5_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT6_T_FOR_OUT2_Pos (14UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT6_T_FOR_OUT2 (Bit 14)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT6_T_FOR_OUT2_Msk (0x4000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT6_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT7_T_FOR_OUT2_Pos (15UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT7_T_FOR_OUT2 (Bit 15)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT7_T_FOR_OUT2_Msk (0x8000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT7_T_FOR_OUT2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT0_T_FOR_OUT3_Pos (16UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT0_T_FOR_OUT3 (Bit 16)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT0_T_FOR_OUT3_Msk (0x10000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT0_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT1_T_FOR_OUT3_Pos (17UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT1_T_FOR_OUT3 (Bit 17)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT1_T_FOR_OUT3_Msk (0x20000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT1_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT2_T_FOR_OUT3_Pos (18UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT2_T_FOR_OUT3 (Bit 18)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT2_T_FOR_OUT3_Msk (0x40000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT2_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT3_T_FOR_OUT3_Pos (19UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT3_T_FOR_OUT3 (Bit 19)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT3_T_FOR_OUT3_Msk (0x80000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT3_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT4_T_FOR_OUT3_Pos (20UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT4_T_FOR_OUT3 (Bit 20)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT4_T_FOR_OUT3_Msk (0x100000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT4_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT5_T_FOR_OUT3_Pos (21UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT5_T_FOR_OUT3 (Bit 21)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT5_T_FOR_OUT3_Msk (0x200000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT5_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT6_T_FOR_OUT3_Pos (22UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT6_T_FOR_OUT3 (Bit 22)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT6_T_FOR_OUT3_Msk (0x400000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT6_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT7_T_FOR_OUT3_Pos (23UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT7_T_FOR_OUT3 (Bit 23)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD0_PT7_T_FOR_OUT3_Msk (0x800000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD0_PT7_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT0_T_FOR_OUT3_Pos (24UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT0_T_FOR_OUT3 (Bit 24)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT0_T_FOR_OUT3_Msk (0x1000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT0_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT1_T_FOR_OUT3_Pos (25UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT1_T_FOR_OUT3 (Bit 25)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT1_T_FOR_OUT3_Msk (0x2000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT1_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT2_T_FOR_OUT3_Pos (26UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT2_T_FOR_OUT3 (Bit 26)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT2_T_FOR_OUT3_Msk (0x4000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT2_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT3_T_FOR_OUT3_Pos (27UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT3_T_FOR_OUT3 (Bit 27)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT3_T_FOR_OUT3_Msk (0x8000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT3_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT4_T_FOR_OUT3_Pos (28UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT4_T_FOR_OUT3 (Bit 28)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT4_T_FOR_OUT3_Msk (0x10000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT4_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT5_T_FOR_OUT3_Pos (29UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT5_T_FOR_OUT3 (Bit 29)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT5_T_FOR_OUT3_Msk (0x20000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT5_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT6_T_FOR_OUT3_Pos (30UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT6_T_FOR_OUT3 (Bit 30)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT6_T_FOR_OUT3_Msk (0x40000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT6_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT7_T_FOR_OUT3_Pos (31UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT7_T_FOR_OUT3 (Bit 31)*/
#define UDB_UDBPAIR_UDBSNG_PLD_ORT1_PLD1_PT7_T_FOR_OUT3_Msk (0x80000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_ORT1: PLD1_PT7_T_FOR_OUT3 (Bitfield-Mask: 0x01)*/
/* ==============================================  UDB_UDBPAIR_UDBSNG.PLD_CFG0  ============================================== */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC0_CEN_Pos (0UL)      /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC0_CEN (Bit 0)         */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC0_CEN_Msk (0x1UL)    /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC0_CEN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC0_DFF_C_Pos (1UL)    /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC0_DFF_C (Bit 1)       */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC0_DFF_C_Msk (0x2UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC0_DFF_C (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC1_CEN_Pos (2UL)      /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC1_CEN (Bit 2)         */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC1_CEN_Msk (0x4UL)    /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC1_CEN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC1_DFF_C_Pos (3UL)    /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC1_DFF_C (Bit 3)       */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC1_DFF_C_Msk (0x8UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC1_DFF_C (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC2_CEN_Pos (4UL)      /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC2_CEN (Bit 4)         */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC2_CEN_Msk (0x10UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC2_CEN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC2_DFF_C_Pos (5UL)    /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC2_DFF_C (Bit 5)       */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC2_DFF_C_Msk (0x20UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC2_DFF_C (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC3_CEN_Pos (6UL)      /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC3_CEN (Bit 6)         */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC3_CEN_Msk (0x40UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC3_CEN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC3_DFF_C_Pos (7UL)    /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC3_DFF_C (Bit 7)       */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC3_DFF_C_Msk (0x80UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC3_DFF_C (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC0_CEN_Pos (8UL)      /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC0_CEN (Bit 8)         */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC0_CEN_Msk (0x100UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC0_CEN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC0_DFF_C_Pos (9UL)    /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC0_DFF_C (Bit 9)       */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC0_DFF_C_Msk (0x200UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC0_DFF_C (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC1_CEN_Pos (10UL)     /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC1_CEN (Bit 10)        */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC1_CEN_Msk (0x400UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC1_CEN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC1_DFF_C_Pos (11UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC1_DFF_C (Bit 11)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC1_DFF_C_Msk (0x800UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC1_DFF_C (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC2_CEN_Pos (12UL)     /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC2_CEN (Bit 12)        */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC2_CEN_Msk (0x1000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC2_CEN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC2_DFF_C_Pos (13UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC2_DFF_C (Bit 13)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC2_DFF_C_Msk (0x2000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC2_DFF_C (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC3_CEN_Pos (14UL)     /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC3_CEN (Bit 14)        */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC3_CEN_Msk (0x4000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC3_CEN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC3_DFF_C_Pos (15UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC3_DFF_C (Bit 15)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC3_DFF_C_Msk (0x8000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC3_DFF_C (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC0_XORFB_Pos (16UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC0_XORFB (Bit 16)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC0_XORFB_Msk (0x30000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC0_XORFB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC1_XORFB_Pos (18UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC1_XORFB (Bit 18)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC1_XORFB_Msk (0xc0000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC1_XORFB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC2_XORFB_Pos (20UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC2_XORFB (Bit 20)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC2_XORFB_Msk (0x300000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC2_XORFB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC3_XORFB_Pos (22UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC3_XORFB (Bit 22)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD0_MC3_XORFB_Msk (0xc00000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD0_MC3_XORFB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC0_XORFB_Pos (24UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC0_XORFB (Bit 24)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC0_XORFB_Msk (0x3000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC0_XORFB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC1_XORFB_Pos (26UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC1_XORFB (Bit 26)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC1_XORFB_Msk (0xc000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC1_XORFB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC2_XORFB_Pos (28UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC2_XORFB (Bit 28)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC2_XORFB_Msk (0x30000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC2_XORFB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC3_XORFB_Pos (30UL)   /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC3_XORFB (Bit 30)      */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG0_PLD1_MC3_XORFB_Msk (0xc0000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG0: PLD1_MC3_XORFB (Bitfield-Mask: 0x03)*/
/* ==============================================  UDB_UDBPAIR_UDBSNG.PLD_CFG1  ============================================== */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC0_SET_SEL_Pos (0UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC0_SET_SEL (Bit 0)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC0_SET_SEL_Msk (0x1UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC0_SET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC0_RESET_SEL_Pos (1UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC0_RESET_SEL (Bit 1)  */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC0_RESET_SEL_Msk (0x2UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC0_RESET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC1_SET_SEL_Pos (2UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC1_SET_SEL (Bit 2)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC1_SET_SEL_Msk (0x4UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC1_SET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC1_RESET_SEL_Pos (3UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC1_RESET_SEL (Bit 3)  */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC1_RESET_SEL_Msk (0x8UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC1_RESET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC2_SET_SEL_Pos (4UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC2_SET_SEL (Bit 4)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC2_SET_SEL_Msk (0x10UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC2_SET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC2_RESET_SEL_Pos (5UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC2_RESET_SEL (Bit 5)  */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC2_RESET_SEL_Msk (0x20UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC2_RESET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC3_SET_SEL_Pos (6UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC3_SET_SEL (Bit 6)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC3_SET_SEL_Msk (0x40UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC3_SET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC3_RESET_SEL_Pos (7UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC3_RESET_SEL (Bit 7)  */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC3_RESET_SEL_Msk (0x80UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC3_RESET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC0_SET_SEL_Pos (8UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC0_SET_SEL (Bit 8)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC0_SET_SEL_Msk (0x100UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC0_SET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC0_RESET_SEL_Pos (9UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC0_RESET_SEL (Bit 9)  */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC0_RESET_SEL_Msk (0x200UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC0_RESET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC1_SET_SEL_Pos (10UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC1_SET_SEL (Bit 10)    */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC1_SET_SEL_Msk (0x400UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC1_SET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC1_RESET_SEL_Pos (11UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC1_RESET_SEL (Bit 11)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC1_RESET_SEL_Msk (0x800UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC1_RESET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC2_SET_SEL_Pos (12UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC2_SET_SEL (Bit 12)    */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC2_SET_SEL_Msk (0x1000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC2_SET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC2_RESET_SEL_Pos (13UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC2_RESET_SEL (Bit 13)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC2_RESET_SEL_Msk (0x2000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC2_RESET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC3_SET_SEL_Pos (14UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC3_SET_SEL (Bit 14)    */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC3_SET_SEL_Msk (0x4000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC3_SET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC3_RESET_SEL_Pos (15UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC3_RESET_SEL (Bit 15)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC3_RESET_SEL_Msk (0x8000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC3_RESET_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC0_BYPASS_Pos (16UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC0_BYPASS (Bit 16)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC0_BYPASS_Msk (0x10000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC0_BYPASS (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC1_BYPASS_Pos (18UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC1_BYPASS (Bit 18)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC1_BYPASS_Msk (0x40000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC1_BYPASS (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC2_BYPASS_Pos (20UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC2_BYPASS (Bit 20)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC2_BYPASS_Msk (0x100000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC2_BYPASS (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC3_BYPASS_Pos (22UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC3_BYPASS (Bit 22)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD0_MC3_BYPASS_Msk (0x400000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD0_MC3_BYPASS (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC0_BYPASS_Pos (24UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC0_BYPASS (Bit 24)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC0_BYPASS_Msk (0x1000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC0_BYPASS (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC1_BYPASS_Pos (26UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC1_BYPASS (Bit 26)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC1_BYPASS_Msk (0x4000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC1_BYPASS (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC2_BYPASS_Pos (28UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC2_BYPASS (Bit 28)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC2_BYPASS_Msk (0x10000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC2_BYPASS (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC3_BYPASS_Pos (30UL)  /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC3_BYPASS (Bit 30)     */
#define UDB_UDBPAIR_UDBSNG_PLD_CFG1_PLD1_MC3_BYPASS_Msk (0x40000000UL) /*!< UDB_UDBPAIR_UDBSNG PLD_CFG1: PLD1_MC3_BYPASS (Bitfield-Mask: 0x01)*/
/* =============================================  UDB_UDBPAIR_UDBSNG.DPATH_CFG0  ============================================= */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_RAD0_Pos (0UL)            /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: RAD0 (Bit 0)               */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_RAD0_Msk (0x7UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: RAD0 (Bitfield-Mask: 0x07) */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_RAD1_Pos (4UL)            /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: RAD1 (Bit 4)               */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_RAD1_Msk (0x70UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: RAD1 (Bitfield-Mask: 0x07) */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_RAD2_Pos (8UL)            /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: RAD2 (Bit 8)               */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_RAD2_Msk (0x700UL)        /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: RAD2 (Bitfield-Mask: 0x07) */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS0_Pos (11UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS0 (Bit 11)    */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS0_Msk (0x800UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS1_Pos (12UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS1 (Bit 12)    */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS1_Msk (0x1000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS2_Pos (13UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS2 (Bit 13)    */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS2_Msk (0x2000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS2 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS3_Pos (14UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS3 (Bit 14)    */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS3_Msk (0x4000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS3 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS4_Pos (15UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS4 (Bit 15)    */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS4_Msk (0x8000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS4 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_F0_LD_Pos (16UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: F0_LD (Bit 16)             */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_F0_LD_Msk (0x70000UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: F0_LD (Bitfield-Mask: 0x07)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS5_Pos (19UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS5 (Bit 19)    */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_DP_RTE_BYPASS5_Msk (0x80000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: DP_RTE_BYPASS5 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_F1_LD_Pos (20UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: F1_LD (Bit 20)             */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_F1_LD_Msk (0x700000UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: F1_LD (Bitfield-Mask: 0x07)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_D0_LD_Pos (24UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: D0_LD (Bit 24)             */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_D0_LD_Msk (0x7000000UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: D0_LD (Bitfield-Mask: 0x07)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_D1_LD_Pos (28UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: D1_LD (Bit 28)             */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG0_D1_LD_Msk (0x70000000UL)  /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG0: D1_LD (Bitfield-Mask: 0x07)*/
/* =============================================  UDB_UDBPAIR_UDBSNG.DPATH_CFG1  ============================================= */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_SI_MUX_Pos (0UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: SI_MUX (Bit 0)             */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_SI_MUX_Msk (0x7UL)        /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: SI_MUX (Bitfield-Mask: 0x07)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_CI_MUX_Pos (4UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: CI_MUX (Bit 4)             */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_CI_MUX_Msk (0x70UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: CI_MUX (Bitfield-Mask: 0x07)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT0_Pos (8UL)            /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT0 (Bit 8)               */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT0_Msk (0xf00UL)        /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT0 (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT1_Pos (12UL)           /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT1 (Bit 12)              */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT1_Msk (0xf000UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT1 (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT2_Pos (16UL)           /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT2 (Bit 16)              */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT2_Msk (0xf0000UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT2 (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT3_Pos (20UL)           /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT3 (Bit 20)              */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT3_Msk (0xf00000UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT3 (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT4_Pos (24UL)           /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT4 (Bit 24)              */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT4_Msk (0xf000000UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT4 (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT5_Pos (28UL)           /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT5 (Bit 28)              */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG1_OUT5_Msk (0xf0000000UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG1: OUT5 (Bitfield-Mask: 0x0f) */
/* =============================================  UDB_UDBPAIR_UDBSNG.DPATH_CFG2  ============================================= */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG2_OUT_SYNC_Pos (0UL)        /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG2: OUT_SYNC (Bit 0)           */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG2_OUT_SYNC_Msk (0x3fUL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG2: OUT_SYNC (Bitfield-Mask: 0x3f)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG2_AMASK_Pos (8UL)           /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG2: AMASK (Bit 8)              */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG2_AMASK_Msk (0xff00UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG2: AMASK (Bitfield-Mask: 0xff)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG2_CMASK0_Pos (16UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG2: CMASK0 (Bit 16)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG2_CMASK0_Msk (0xff0000UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG2: CMASK0 (Bitfield-Mask: 0xff)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG2_CMASK1_Pos (24UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG2: CMASK1 (Bit 24)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG2_CMASK1_Msk (0xff000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG2: CMASK1 (Bitfield-Mask: 0xff)*/
/* =============================================  UDB_UDBPAIR_UDBSNG.DPATH_CFG3  ============================================= */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_SI_SELA_Pos (0UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: SI_SELA (Bit 0)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_SI_SELA_Msk (0x3UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: SI_SELA (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_SI_SELB_Pos (2UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: SI_SELB (Bit 2)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_SI_SELB_Msk (0xcUL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: SI_SELB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_DEF_SI_Pos (4UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: DEF_SI (Bit 4)             */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_DEF_SI_Msk (0x10UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: DEF_SI (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_AMASK_EN_Pos (5UL)        /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: AMASK_EN (Bit 5)           */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_AMASK_EN_Msk (0x20UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: AMASK_EN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CMASK0_EN_Pos (6UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CMASK0_EN (Bit 6)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CMASK0_EN_Msk (0x40UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CMASK0_EN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CMASK1_EN_Pos (7UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CMASK1_EN (Bit 7)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CMASK1_EN_Msk (0x80UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CMASK1_EN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CI_SELA_Pos (8UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CI_SELA (Bit 8)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CI_SELA_Msk (0x300UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CI_SELA (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CI_SELB_Pos (10UL)        /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CI_SELB (Bit 10)           */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CI_SELB_Msk (0xc00UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CI_SELB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CMP_SELA_Pos (12UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CMP_SELA (Bit 12)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CMP_SELA_Msk (0x3000UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CMP_SELA (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CMP_SELB_Pos (14UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CMP_SELB (Bit 14)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CMP_SELB_Msk (0xc000UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CMP_SELB (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CHAIN0_Pos (16UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CHAIN0 (Bit 16)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CHAIN0_Msk (0x10000UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CHAIN0 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CHAIN1_Pos (17UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CHAIN1 (Bit 17)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CHAIN1_Msk (0x20000UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CHAIN1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CHAIN_FB_Pos (18UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CHAIN_FB (Bit 18)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CHAIN_FB_Msk (0x40000UL)  /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CHAIN_FB (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CHAIN_CMSB_Pos (19UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CHAIN_CMSB (Bit 19)        */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_CHAIN_CMSB_Msk (0x80000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: CHAIN_CMSB (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_MSB_SEL_Pos (20UL)        /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: MSB_SEL (Bit 20)           */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_MSB_SEL_Msk (0x700000UL)  /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: MSB_SEL (Bitfield-Mask: 0x07)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_MSB_EN_Pos (23UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: MSB_EN (Bit 23)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_MSB_EN_Msk (0x800000UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: MSB_EN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_F0_INSEL_Pos (24UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: F0_INSEL (Bit 24)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_F0_INSEL_Msk (0x3000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: F0_INSEL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_F1_INSEL_Pos (26UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: F1_INSEL (Bit 26)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_F1_INSEL_Msk (0xc000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: F1_INSEL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_MSB_SI_Pos (28UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: MSB_SI (Bit 28)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_MSB_SI_Msk (0x10000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: MSB_SI (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_PI_DYN_Pos (29UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: PI_DYN (Bit 29)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_PI_DYN_Msk (0x20000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: PI_DYN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_SHIFT_SEL_Pos (30UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: SHIFT_SEL (Bit 30)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_SHIFT_SEL_Msk (0x40000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: SHIFT_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_PI_SEL_Pos (31UL)         /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: PI_SEL (Bit 31)            */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG3_PI_SEL_Msk (0x80000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG3: PI_SEL (Bitfield-Mask: 0x01)*/
/* =============================================  UDB_UDBPAIR_UDBSNG.DPATH_CFG4  ============================================= */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_EXT_CRCPRS_Pos (1UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: EXT_CRCPRS (Bit 1)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_EXT_CRCPRS_Msk (0x2UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: EXT_CRCPRS (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_ASYNC_Pos (2UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_ASYNC (Bit 2)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_ASYNC_Msk (0x4UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_ASYNC (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_EDGE_Pos (3UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_EDGE (Bit 3)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_EDGE_Msk (0x8UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_EDGE (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_CAP_Pos (4UL)        /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_CAP (Bit 4)           */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_CAP_Msk (0x10UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_CAP (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_FAST_Pos (5UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_FAST (Bit 5)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_FAST_Msk (0x20UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_FAST (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_F0_CK_INV_Pos (6UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: F0_CK_INV (Bit 6)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_F0_CK_INV_Msk (0x40UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: F0_CK_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_F1_CK_INV_Pos (7UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: F1_CK_INV (Bit 7)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_F1_CK_INV_Msk (0x80UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: F1_CK_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_F0_DYN_Pos (8UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: F0_DYN (Bit 8)             */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_F0_DYN_Msk (0x100UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: F0_DYN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_F1_DYN_Pos (9UL)          /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: F1_DYN (Bit 9)             */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_F1_DYN_Msk (0x200UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: F1_DYN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_ADD_SYNC_Pos (12UL)  /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_ADD_SYNC (Bit 12)     */
#define UDB_UDBPAIR_UDBSNG_DPATH_CFG4_FIFO_ADD_SYNC_Msk (0x1000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_CFG4: FIFO_ADD_SYNC (Bitfield-Mask: 0x01)*/
/* ==============================================  UDB_UDBPAIR_UDBSNG.SC_CFG0  =============================================== */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_CTL_MD0_Pos (0UL)            /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: CTL_MD0 (Bit 0)               */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_CTL_MD0_Msk (0xffUL)         /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: CTL_MD0 (Bitfield-Mask: 0xff) */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_CTL_MD1_Pos (8UL)            /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: CTL_MD1 (Bit 8)               */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_CTL_MD1_Msk (0xff00UL)       /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: CTL_MD1 (Bitfield-Mask: 0xff) */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_STAT_MD_Pos (16UL)           /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: STAT_MD (Bit 16)              */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_STAT_MD_Msk (0xff0000UL)     /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: STAT_MD (Bitfield-Mask: 0xff) */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_SC_OUT_CTL_Pos (24UL)        /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: SC_OUT_CTL (Bit 24)           */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_SC_OUT_CTL_Msk (0x3000000UL) /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: SC_OUT_CTL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_SC_INT_MD_Pos (26UL)         /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: SC_INT_MD (Bit 26)            */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_SC_INT_MD_Msk (0x4000000UL)  /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: SC_INT_MD (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_SC_SYNC_MD_Pos (27UL)        /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: SC_SYNC_MD (Bit 27)           */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_SC_SYNC_MD_Msk (0x8000000UL) /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: SC_SYNC_MD (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_SC_EXT_RES_Pos (28UL)        /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: SC_EXT_RES (Bit 28)           */
#define UDB_UDBPAIR_UDBSNG_SC_CFG0_SC_EXT_RES_Msk (0x10000000UL) /*!< UDB_UDBPAIR_UDBSNG SC_CFG0: SC_EXT_RES (Bitfield-Mask: 0x01)*/
/* ==============================================  UDB_UDBPAIR_UDBSNG.SC_CFG1  =============================================== */
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_CNT_LD_SEL_Pos (0UL)         /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: CNT_LD_SEL (Bit 0)            */
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_CNT_LD_SEL_Msk (0x3UL)       /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: CNT_LD_SEL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_CNT_EN_SEL_Pos (2UL)         /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: CNT_EN_SEL (Bit 2)            */
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_CNT_EN_SEL_Msk (0xcUL)       /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: CNT_EN_SEL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_ROUTE_LD_Pos (4UL)           /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: ROUTE_LD (Bit 4)              */
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_ROUTE_LD_Msk (0x10UL)        /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: ROUTE_LD (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_ROUTE_EN_Pos (5UL)           /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: ROUTE_EN (Bit 5)              */
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_ROUTE_EN_Msk (0x20UL)        /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: ROUTE_EN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_ALT_CNT_Pos (6UL)            /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: ALT_CNT (Bit 6)               */
#define UDB_UDBPAIR_UDBSNG_SC_CFG1_ALT_CNT_Msk (0x40UL)         /*!< UDB_UDBPAIR_UDBSNG SC_CFG1: ALT_CNT (Bitfield-Mask: 0x01) */
/* ==============================================  UDB_UDBPAIR_UDBSNG.RC_CFG0  =============================================== */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_EN_SEL_Pos (0UL)     /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_EN_SEL (Bit 0)        */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_EN_SEL_Msk (0x3UL)   /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_EN_SEL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_EN_MODE_Pos (2UL)    /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_EN_MODE (Bit 2)       */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_EN_MODE_Msk (0xcUL)  /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_EN_MODE (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_EN_INV_Pos (4UL)     /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_EN_INV (Bit 4)        */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_EN_INV_Msk (0x10UL)  /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_EN_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_INV_Pos (5UL)        /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_INV (Bit 5)           */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_INV_Msk (0x20UL)     /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_RES_SEL0_OR_FRES_Pos (6UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_RES_SEL0_OR_FRES (Bit 6)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_RES_SEL0_OR_FRES_Msk (0x40UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_RES_SEL0_OR_FRES (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_RES_SEL1_Pos (7UL)   /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_RES_SEL1 (Bit 7)      */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD0_RC_RES_SEL1_Msk (0x80UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD0_RC_RES_SEL1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_EN_SEL_Pos (8UL)     /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_EN_SEL (Bit 8)        */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_EN_SEL_Msk (0x300UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_EN_SEL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_EN_MODE_Pos (10UL)   /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_EN_MODE (Bit 10)      */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_EN_MODE_Msk (0xc00UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_EN_MODE (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_EN_INV_Pos (12UL)    /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_EN_INV (Bit 12)       */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_EN_INV_Msk (0x1000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_EN_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_INV_Pos (13UL)       /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_INV (Bit 13)          */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_INV_Msk (0x2000UL)   /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_RES_SEL0_OR_FRES_Pos (14UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_RES_SEL0_OR_FRES (Bit 14)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_PLD1_RC_RES_SEL0_OR_FRES_Msk (0x4000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: PLD1_RC_RES_SEL0_OR_FRES (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_EN_SEL_Pos (16UL)      /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_EN_SEL (Bit 16)         */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_EN_SEL_Msk (0x30000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_EN_SEL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_EN_MODE_Pos (18UL)     /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_EN_MODE (Bit 18)        */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_EN_MODE_Msk (0xc0000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_EN_MODE (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_EN_INV_Pos (20UL)      /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_EN_INV (Bit 20)         */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_EN_INV_Msk (0x100000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_EN_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_INV_Pos (21UL)         /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_INV (Bit 21)            */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_INV_Msk (0x200000UL)   /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_RES_SEL0_OR_FRES_Pos (22UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_RES_SEL0_OR_FRES (Bit 22)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_RES_SEL0_OR_FRES_Msk (0x400000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_RES_SEL0_OR_FRES (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_RES_SEL1_Pos (23UL)    /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_RES_SEL1 (Bit 23)       */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_DP_RC_RES_SEL1_Msk (0x800000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: DP_RC_RES_SEL1 (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_EN_SEL_Pos (24UL)      /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_EN_SEL (Bit 24)         */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_EN_SEL_Msk (0x3000000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_EN_SEL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_EN_MODE_Pos (26UL)     /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_EN_MODE (Bit 26)        */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_EN_MODE_Msk (0xc000000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_EN_MODE (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_EN_INV_Pos (28UL)      /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_EN_INV (Bit 28)         */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_EN_INV_Msk (0x10000000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_EN_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_INV_Pos (29UL)         /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_INV (Bit 29)            */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_INV_Msk (0x20000000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_INV (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_RES_SEL0_OR_FRES_Pos (30UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_RES_SEL0_OR_FRES (Bit 30)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_RES_SEL0_OR_FRES_Msk (0x40000000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_RES_SEL0_OR_FRES (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_RES_SEL1_Pos (31UL)    /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_RES_SEL1 (Bit 31)       */
#define UDB_UDBPAIR_UDBSNG_RC_CFG0_SC_RC_RES_SEL1_Msk (0x80000000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG0: SC_RC_RES_SEL1 (Bitfield-Mask: 0x01)*/
/* ==============================================  UDB_UDBPAIR_UDBSNG.RC_CFG1  =============================================== */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_PLD0_CK_SEL_Pos (0UL)        /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: PLD0_CK_SEL (Bit 0)           */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_PLD0_CK_SEL_Msk (0xfUL)      /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: PLD0_CK_SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_PLD1_CK_SEL_Pos (4UL)        /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: PLD1_CK_SEL (Bit 4)           */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_PLD1_CK_SEL_Msk (0xf0UL)     /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: PLD1_CK_SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_DP_CK_SEL_Pos (8UL)          /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: DP_CK_SEL (Bit 8)             */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_DP_CK_SEL_Msk (0xf00UL)      /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: DP_CK_SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_SC_CK_SEL_Pos (12UL)         /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: SC_CK_SEL (Bit 12)            */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_SC_CK_SEL_Msk (0xf000UL)     /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: SC_CK_SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_RES_SEL_Pos (16UL)           /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: RES_SEL (Bit 16)              */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_RES_SEL_Msk (0x30000UL)      /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: RES_SEL (Bitfield-Mask: 0x03) */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_RES_POL_Pos (18UL)           /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: RES_POL (Bit 18)              */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_RES_POL_Msk (0x40000UL)      /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: RES_POL (Bitfield-Mask: 0x01) */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EN_RES_CNTCTL_Pos (19UL)     /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EN_RES_CNTCTL (Bit 19)        */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EN_RES_CNTCTL_Msk (0x80000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EN_RES_CNTCTL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_DP_RES_POL_Pos (22UL)        /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: DP_RES_POL (Bit 22)           */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_DP_RES_POL_Msk (0x400000UL)  /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: DP_RES_POL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_SC_RES_POL_Pos (23UL)        /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: SC_RES_POL (Bit 23)           */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_SC_RES_POL_Msk (0x800000UL)  /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: SC_RES_POL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_ALT_RES_Pos (24UL)           /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: ALT_RES (Bit 24)              */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_ALT_RES_Msk (0x1000000UL)    /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: ALT_RES (Bitfield-Mask: 0x01) */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EXT_SYNC_Pos (25UL)          /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EXT_SYNC (Bit 25)             */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EXT_SYNC_Msk (0x2000000UL)   /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EXT_SYNC (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EN_RES_STAT_Pos (26UL)       /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EN_RES_STAT (Bit 26)          */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EN_RES_STAT_Msk (0x4000000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EN_RES_STAT (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EN_RES_DP_Pos (27UL)         /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EN_RES_DP (Bit 27)            */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EN_RES_DP_Msk (0x8000000UL)  /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EN_RES_DP (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EXT_CK_SEL_Pos (28UL)        /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EXT_CK_SEL (Bit 28)           */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_EXT_CK_SEL_Msk (0x30000000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: EXT_CK_SEL (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_PLD0_RES_POL_Pos (30UL)      /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: PLD0_RES_POL (Bit 30)         */
#define UDB_UDBPAIR_UDBSNG_RC_CFG1_PLD0_RES_POL_Msk (0x40000000UL) /*!< UDB_UDBPAIR_UDBSNG RC_CFG1: PLD0_RES_POL (Bitfield-Mask: 0x01)*/
/* =============================================  UDB_UDBPAIR_UDBSNG.DPATH_OPC  ============================================== */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_CMP_SEL_Pos (0UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_CMP_SEL (Bit 0)        */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_CMP_SEL_Msk (0x1UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_CMP_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_SI_SEL_Pos (1UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_SI_SEL (Bit 1)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_SI_SEL_Msk (0x2UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_SI_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_CI_SEL_Pos (2UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_CI_SEL (Bit 2)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_CI_SEL_Msk (0x4UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_CI_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_CFB_EN_Pos (3UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_CFB_EN (Bit 3)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_CFB_EN_Msk (0x8UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_CFB_EN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_A1_WR_SRC_Pos (4UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_A1_WR_SRC (Bit 4)      */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_A1_WR_SRC_Msk (0x30UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_A1_WR_SRC (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_A0_WR_SRC_Pos (6UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_A0_WR_SRC (Bit 6)      */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_A0_WR_SRC_Msk (0xc0UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_A0_WR_SRC (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_SHIFT_Pos (8UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_SHIFT (Bit 8)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_SHIFT_Msk (0x300UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_SHIFT (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_SRC_B_Pos (10UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_SRC_B (Bit 10)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_SRC_B_Msk (0xc00UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_SRC_B (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_SRC_A_Pos (12UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_SRC_A (Bit 12)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_SRC_A_Msk (0x1000UL)  /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_SRC_A (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_FUNC_Pos (13UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_FUNC (Bit 13)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC0_FUNC_Msk (0xe000UL)   /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC0_FUNC (Bitfield-Mask: 0x07)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_CMP_SEL_Pos (16UL)    /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_CMP_SEL (Bit 16)       */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_CMP_SEL_Msk (0x10000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_CMP_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_SI_SEL_Pos (17UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_SI_SEL (Bit 17)        */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_SI_SEL_Msk (0x20000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_SI_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_CI_SEL_Pos (18UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_CI_SEL (Bit 18)        */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_CI_SEL_Msk (0x40000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_CI_SEL (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_CFB_EN_Pos (19UL)     /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_CFB_EN (Bit 19)        */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_CFB_EN_Msk (0x80000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_CFB_EN (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_A1_WR_SRC_Pos (20UL)  /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_A1_WR_SRC (Bit 20)     */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_A1_WR_SRC_Msk (0x300000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_A1_WR_SRC (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_A0_WR_SRC_Pos (22UL)  /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_A0_WR_SRC (Bit 22)     */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_A0_WR_SRC_Msk (0xc00000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_A0_WR_SRC (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_SHIFT_Pos (24UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_SHIFT (Bit 24)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_SHIFT_Msk (0x3000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_SHIFT (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_SRC_B_Pos (26UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_SRC_B (Bit 26)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_SRC_B_Msk (0xc000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_SRC_B (Bitfield-Mask: 0x03)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_SRC_A_Pos (28UL)      /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_SRC_A (Bit 28)         */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_SRC_A_Msk (0x10000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_SRC_A (Bitfield-Mask: 0x01)*/
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_FUNC_Pos (29UL)       /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_FUNC (Bit 29)          */
#define UDB_UDBPAIR_UDBSNG_DPATH_OPC_OPC1_FUNC_Msk (0xe0000000UL) /*!< UDB_UDBPAIR_UDBSNG DPATH_OPC: OPC1_FUNC (Bitfield-Mask: 0x07)*/


/* ==============================================  UDB_UDBPAIR_ROUTE.TOP_V_BOT  ============================================== */
#define UDB_UDBPAIR_ROUTE_TOP_V_BOT_TOP_V_BOT_Pos (0UL)         /*!< UDB_UDBPAIR_ROUTE TOP_V_BOT: TOP_V_BOT (Bit 0)            */
#define UDB_UDBPAIR_ROUTE_TOP_V_BOT_TOP_V_BOT_Msk (0xffffffffUL) /*!< UDB_UDBPAIR_ROUTE TOP_V_BOT: TOP_V_BOT (Bitfield-Mask: 0xffffffff)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LVI1_V_2  =============================================== */
#define UDB_UDBPAIR_ROUTE_LVI1_V_2_LVI1_V_2_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE LVI1_V_2: LVI1_V_2 (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_LVI1_V_2_LVI1_V_2_Msk (0xffffffffUL)  /*!< UDB_UDBPAIR_ROUTE LVI1_V_2: LVI1_V_2 (Bitfield-Mask: 0xffffffff)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.RVI1_V_2  =============================================== */
#define UDB_UDBPAIR_ROUTE_RVI1_V_2_RVI1_V_2_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE RVI1_V_2: RVI1_V_2 (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_RVI1_V_2_RVI1_V_2_Msk (0xffffffffUL)  /*!< UDB_UDBPAIR_ROUTE RVI1_V_2: RVI1_V_2 (Bitfield-Mask: 0xffffffff)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.TUI_CFG0  =============================================== */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI0SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI0SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI0SEL_Msk (0xfUL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI0SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI1SEL_Pos (4UL)            /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI1SEL (Bit 4)               */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI1SEL_Msk (0xf0UL)         /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI1SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI2SEL_Pos (8UL)            /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI2SEL (Bit 8)               */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI2SEL_Msk (0xf00UL)        /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI2SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI3SEL_Pos (12UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI3SEL (Bit 12)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI3SEL_Msk (0xf000UL)       /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI3SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI4SEL_Pos (16UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI4SEL (Bit 16)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI4SEL_Msk (0xf0000UL)      /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI4SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI5SEL_Pos (20UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI5SEL (Bit 20)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI5SEL_Msk (0xf00000UL)     /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI5SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI6SEL_Pos (24UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI6SEL (Bit 24)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI6SEL_Msk (0xf000000UL)    /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI6SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI7SEL_Pos (28UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI7SEL (Bit 28)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG0_TUI7SEL_Msk (0xf0000000UL)   /*!< UDB_UDBPAIR_ROUTE TUI_CFG0: TUI7SEL (Bitfield-Mask: 0x0f) */
/* ==============================================  UDB_UDBPAIR_ROUTE.TUI_CFG1  =============================================== */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI8SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI8SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI8SEL_Msk (0xfUL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI8SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI9SEL_Pos (4UL)            /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI9SEL (Bit 4)               */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI9SEL_Msk (0xf0UL)         /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI9SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI10SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI10SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI10SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI10SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI11SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI11SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI11SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI11SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI12SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI12SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI12SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI12SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI13SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI13SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI13SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI13SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI14SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI14SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI14SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI14SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI15SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI15SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG1_TUI15SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE TUI_CFG1: TUI15SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.TUI_CFG2  =============================================== */
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI16SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI16SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI16SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI16SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI17SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI17SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI17SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI17SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI18SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI18SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI18SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI18SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI19SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI19SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI19SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI19SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI20SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI20SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI20SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI20SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI21SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI21SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI21SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI21SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI22SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI22SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI22SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI22SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI23SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI23SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG2_TUI23SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE TUI_CFG2: TUI23SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.TUI_CFG3  =============================================== */
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI24SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI24SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI24SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI24SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI25SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI25SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI25SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI25SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI26SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI26SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI26SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI26SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI27SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI27SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI27SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI27SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI28SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI28SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI28SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI28SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI29SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI29SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI29SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI29SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI30SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI30SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI30SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI30SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI31SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI31SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG3_TUI31SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE TUI_CFG3: TUI31SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.TUI_CFG4  =============================================== */
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI32SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI32SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI32SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI32SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI33SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI33SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI33SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI33SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI34SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI34SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI34SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI34SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI35SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI35SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI35SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI35SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI36SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI36SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI36SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI36SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI37SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI37SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI37SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI37SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI38SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI38SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI38SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI38SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI39SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI39SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_TUI_CFG4_TUI39SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE TUI_CFG4: TUI39SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.TUI_CFG5  =============================================== */
#define UDB_UDBPAIR_ROUTE_TUI_CFG5_TUI40SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG5: TUI40SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG5_TUI40SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE TUI_CFG5: TUI40SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_TUI_CFG5_TUI41SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE TUI_CFG5: TUI41SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_TUI_CFG5_TUI41SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE TUI_CFG5: TUI41SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.BUI_CFG0  =============================================== */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI0SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI0SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI0SEL_Msk (0xfUL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI0SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI1SEL_Pos (4UL)            /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI1SEL (Bit 4)               */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI1SEL_Msk (0xf0UL)         /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI1SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI2SEL_Pos (8UL)            /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI2SEL (Bit 8)               */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI2SEL_Msk (0xf00UL)        /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI2SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI3SEL_Pos (12UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI3SEL (Bit 12)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI3SEL_Msk (0xf000UL)       /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI3SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI4SEL_Pos (16UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI4SEL (Bit 16)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI4SEL_Msk (0xf0000UL)      /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI4SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI5SEL_Pos (20UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI5SEL (Bit 20)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI5SEL_Msk (0xf00000UL)     /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI5SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI6SEL_Pos (24UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI6SEL (Bit 24)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI6SEL_Msk (0xf000000UL)    /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI6SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI7SEL_Pos (28UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI7SEL (Bit 28)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG0_BUI7SEL_Msk (0xf0000000UL)   /*!< UDB_UDBPAIR_ROUTE BUI_CFG0: BUI7SEL (Bitfield-Mask: 0x0f) */
/* ==============================================  UDB_UDBPAIR_ROUTE.BUI_CFG1  =============================================== */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI8SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI8SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI8SEL_Msk (0xfUL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI8SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI9SEL_Pos (4UL)            /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI9SEL (Bit 4)               */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI9SEL_Msk (0xf0UL)         /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI9SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI10SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI10SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI10SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI10SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI11SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI11SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI11SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI11SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI12SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI12SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI12SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI12SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI13SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI13SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI13SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI13SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI14SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI14SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI14SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI14SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI15SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI15SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG1_BUI15SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE BUI_CFG1: BUI15SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.BUI_CFG2  =============================================== */
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI16SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI16SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI16SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI16SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI17SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI17SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI17SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI17SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI18SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI18SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI18SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI18SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI19SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI19SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI19SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI19SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI20SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI20SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI20SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI20SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI21SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI21SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI21SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI21SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI22SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI22SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI22SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI22SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI23SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI23SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG2_BUI23SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE BUI_CFG2: BUI23SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.BUI_CFG3  =============================================== */
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI24SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI24SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI24SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI24SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI25SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI25SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI25SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI25SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI26SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI26SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI26SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI26SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI27SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI27SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI27SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI27SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI28SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI28SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI28SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI28SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI29SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI29SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI29SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI29SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI30SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI30SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI30SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI30SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI31SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI31SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG3_BUI31SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE BUI_CFG3: BUI31SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.BUI_CFG4  =============================================== */
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI32SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI32SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI32SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI32SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI33SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI33SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI33SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI33SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI34SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI34SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI34SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI34SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI35SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI35SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI35SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI35SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI36SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI36SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI36SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI36SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI37SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI37SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI37SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI37SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI38SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI38SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI38SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI38SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI39SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI39SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_BUI_CFG4_BUI39SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE BUI_CFG4: BUI39SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.BUI_CFG5  =============================================== */
#define UDB_UDBPAIR_ROUTE_BUI_CFG5_BUI40SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG5: BUI40SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG5_BUI40SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE BUI_CFG5: BUI40SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_BUI_CFG5_BUI41SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE BUI_CFG5: BUI41SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_BUI_CFG5_BUI41SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE BUI_CFG5: BUI41SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.RVO_CFG0  =============================================== */
#define UDB_UDBPAIR_ROUTE_RVO_CFG0_RVO0SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE RVO_CFG0: RVO0SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_RVO_CFG0_RVO0SEL_Msk (0x1fUL)         /*!< UDB_UDBPAIR_ROUTE RVO_CFG0: RVO0SEL (Bitfield-Mask: 0x1f) */
#define UDB_UDBPAIR_ROUTE_RVO_CFG0_RVO1SEL_Pos (8UL)            /*!< UDB_UDBPAIR_ROUTE RVO_CFG0: RVO1SEL (Bit 8)               */
#define UDB_UDBPAIR_ROUTE_RVO_CFG0_RVO1SEL_Msk (0x1f00UL)       /*!< UDB_UDBPAIR_ROUTE RVO_CFG0: RVO1SEL (Bitfield-Mask: 0x1f) */
#define UDB_UDBPAIR_ROUTE_RVO_CFG0_RVO2SEL_Pos (16UL)           /*!< UDB_UDBPAIR_ROUTE RVO_CFG0: RVO2SEL (Bit 16)              */
#define UDB_UDBPAIR_ROUTE_RVO_CFG0_RVO2SEL_Msk (0x1f0000UL)     /*!< UDB_UDBPAIR_ROUTE RVO_CFG0: RVO2SEL (Bitfield-Mask: 0x1f) */
#define UDB_UDBPAIR_ROUTE_RVO_CFG0_RVO3SEL_Pos (24UL)           /*!< UDB_UDBPAIR_ROUTE RVO_CFG0: RVO3SEL (Bit 24)              */
#define UDB_UDBPAIR_ROUTE_RVO_CFG0_RVO3SEL_Msk (0x1f000000UL)   /*!< UDB_UDBPAIR_ROUTE RVO_CFG0: RVO3SEL (Bitfield-Mask: 0x1f) */
/* ==============================================  UDB_UDBPAIR_ROUTE.RVO_CFG1  =============================================== */
#define UDB_UDBPAIR_ROUTE_RVO_CFG1_RVO4SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE RVO_CFG1: RVO4SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_RVO_CFG1_RVO4SEL_Msk (0x1fUL)         /*!< UDB_UDBPAIR_ROUTE RVO_CFG1: RVO4SEL (Bitfield-Mask: 0x1f) */
#define UDB_UDBPAIR_ROUTE_RVO_CFG1_RVO5SEL_Pos (8UL)            /*!< UDB_UDBPAIR_ROUTE RVO_CFG1: RVO5SEL (Bit 8)               */
#define UDB_UDBPAIR_ROUTE_RVO_CFG1_RVO5SEL_Msk (0x1f00UL)       /*!< UDB_UDBPAIR_ROUTE RVO_CFG1: RVO5SEL (Bitfield-Mask: 0x1f) */
#define UDB_UDBPAIR_ROUTE_RVO_CFG1_RVO6SEL_Pos (16UL)           /*!< UDB_UDBPAIR_ROUTE RVO_CFG1: RVO6SEL (Bit 16)              */
#define UDB_UDBPAIR_ROUTE_RVO_CFG1_RVO6SEL_Msk (0x1f0000UL)     /*!< UDB_UDBPAIR_ROUTE RVO_CFG1: RVO6SEL (Bitfield-Mask: 0x1f) */
#define UDB_UDBPAIR_ROUTE_RVO_CFG1_RVO7SEL_Pos (24UL)           /*!< UDB_UDBPAIR_ROUTE RVO_CFG1: RVO7SEL (Bit 24)              */
#define UDB_UDBPAIR_ROUTE_RVO_CFG1_RVO7SEL_Msk (0x1f000000UL)   /*!< UDB_UDBPAIR_ROUTE RVO_CFG1: RVO7SEL (Bitfield-Mask: 0x1f) */
/* ==============================================  UDB_UDBPAIR_ROUTE.RVO_CFG2  =============================================== */
#define UDB_UDBPAIR_ROUTE_RVO_CFG2_RVO8SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE RVO_CFG2: RVO8SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_RVO_CFG2_RVO8SEL_Msk (0x1fUL)         /*!< UDB_UDBPAIR_ROUTE RVO_CFG2: RVO8SEL (Bitfield-Mask: 0x1f) */
#define UDB_UDBPAIR_ROUTE_RVO_CFG2_RVO9SEL_Pos (8UL)            /*!< UDB_UDBPAIR_ROUTE RVO_CFG2: RVO9SEL (Bit 8)               */
#define UDB_UDBPAIR_ROUTE_RVO_CFG2_RVO9SEL_Msk (0x1f00UL)       /*!< UDB_UDBPAIR_ROUTE RVO_CFG2: RVO9SEL (Bitfield-Mask: 0x1f) */
#define UDB_UDBPAIR_ROUTE_RVO_CFG2_RVO10SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE RVO_CFG2: RVO10SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_RVO_CFG2_RVO10SEL_Msk (0x1f0000UL)    /*!< UDB_UDBPAIR_ROUTE RVO_CFG2: RVO10SEL (Bitfield-Mask: 0x1f)*/
#define UDB_UDBPAIR_ROUTE_RVO_CFG2_RVO11SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE RVO_CFG2: RVO11SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_RVO_CFG2_RVO11SEL_Msk (0x1f000000UL)  /*!< UDB_UDBPAIR_ROUTE RVO_CFG2: RVO11SEL (Bitfield-Mask: 0x1f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.RVO_CFG3  =============================================== */
#define UDB_UDBPAIR_ROUTE_RVO_CFG3_RVO12SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE RVO_CFG3: RVO12SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_RVO_CFG3_RVO12SEL_Msk (0x1fUL)        /*!< UDB_UDBPAIR_ROUTE RVO_CFG3: RVO12SEL (Bitfield-Mask: 0x1f)*/
#define UDB_UDBPAIR_ROUTE_RVO_CFG3_RVO13SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE RVO_CFG3: RVO13SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_RVO_CFG3_RVO13SEL_Msk (0x1f00UL)      /*!< UDB_UDBPAIR_ROUTE RVO_CFG3: RVO13SEL (Bitfield-Mask: 0x1f)*/
#define UDB_UDBPAIR_ROUTE_RVO_CFG3_RVO14SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE RVO_CFG3: RVO14SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_RVO_CFG3_RVO14SEL_Msk (0x1f0000UL)    /*!< UDB_UDBPAIR_ROUTE RVO_CFG3: RVO14SEL (Bitfield-Mask: 0x1f)*/
#define UDB_UDBPAIR_ROUTE_RVO_CFG3_RVO15SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE RVO_CFG3: RVO15SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_RVO_CFG3_RVO15SEL_Msk (0x1f000000UL)  /*!< UDB_UDBPAIR_ROUTE RVO_CFG3: RVO15SEL (Bitfield-Mask: 0x1f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LVO_CFG0  =============================================== */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO0SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO0SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO0SEL_Msk (0xfUL)          /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO0SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO1SEL_Pos (4UL)            /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO1SEL (Bit 4)               */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO1SEL_Msk (0xf0UL)         /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO1SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO2SEL_Pos (8UL)            /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO2SEL (Bit 8)               */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO2SEL_Msk (0xf00UL)        /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO2SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO3SEL_Pos (12UL)           /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO3SEL (Bit 12)              */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO3SEL_Msk (0xf000UL)       /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO3SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO4SEL_Pos (16UL)           /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO4SEL (Bit 16)              */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO4SEL_Msk (0xf0000UL)      /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO4SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO5SEL_Pos (20UL)           /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO5SEL (Bit 20)              */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO5SEL_Msk (0xf00000UL)     /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO5SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO6SEL_Pos (24UL)           /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO6SEL (Bit 24)              */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO6SEL_Msk (0xf000000UL)    /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO6SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO7SEL_Pos (28UL)           /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO7SEL (Bit 28)              */
#define UDB_UDBPAIR_ROUTE_LVO_CFG0_LVO7SEL_Msk (0xf0000000UL)   /*!< UDB_UDBPAIR_ROUTE LVO_CFG0: LVO7SEL (Bitfield-Mask: 0x0f) */
/* ==============================================  UDB_UDBPAIR_ROUTE.LVO_CFG1  =============================================== */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO8SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO8SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO8SEL_Msk (0xfUL)          /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO8SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO9SEL_Pos (4UL)            /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO9SEL (Bit 4)               */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO9SEL_Msk (0xf0UL)         /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO9SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO10SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO10SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO10SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO10SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO11SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO11SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO11SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO11SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO12SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO12SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO12SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO12SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO13SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO13SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO13SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO13SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO14SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO14SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO14SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO14SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO15SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO15SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LVO_CFG1_LVO15SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LVO_CFG1: LVO15SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.RHO_CFG0  =============================================== */
#define UDB_UDBPAIR_ROUTE_RHO_CFG0_RHOSEL_Pos (0UL)             /*!< UDB_UDBPAIR_ROUTE RHO_CFG0: RHOSEL (Bit 0)                */
#define UDB_UDBPAIR_ROUTE_RHO_CFG0_RHOSEL_Msk (0xffffffffUL)    /*!< UDB_UDBPAIR_ROUTE RHO_CFG0: RHOSEL (Bitfield-Mask: 0xffffffff)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.RHO_CFG1  =============================================== */
#define UDB_UDBPAIR_ROUTE_RHO_CFG1_RHOSEL_Pos (0UL)             /*!< UDB_UDBPAIR_ROUTE RHO_CFG1: RHOSEL (Bit 0)                */
#define UDB_UDBPAIR_ROUTE_RHO_CFG1_RHOSEL_Msk (0xffffffffUL)    /*!< UDB_UDBPAIR_ROUTE RHO_CFG1: RHOSEL (Bitfield-Mask: 0xffffffff)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.RHO_CFG2  =============================================== */
#define UDB_UDBPAIR_ROUTE_RHO_CFG2_RHOSEL_Pos (0UL)             /*!< UDB_UDBPAIR_ROUTE RHO_CFG2: RHOSEL (Bit 0)                */
#define UDB_UDBPAIR_ROUTE_RHO_CFG2_RHOSEL_Msk (0xffffffffUL)    /*!< UDB_UDBPAIR_ROUTE RHO_CFG2: RHOSEL (Bitfield-Mask: 0xffffffff)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG0  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO0SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO0SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO0SEL_Msk (0xfUL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO0SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO1SEL_Pos (4UL)            /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO1SEL (Bit 4)               */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO1SEL_Msk (0xf0UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO1SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO2SEL_Pos (8UL)            /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO2SEL (Bit 8)               */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO2SEL_Msk (0xf00UL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO2SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO3SEL_Pos (12UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO3SEL (Bit 12)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO3SEL_Msk (0xf000UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO3SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO4SEL_Pos (16UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO4SEL (Bit 16)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO4SEL_Msk (0xf0000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO4SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO5SEL_Pos (20UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO5SEL (Bit 20)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO5SEL_Msk (0xf00000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO5SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO6SEL_Pos (24UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO6SEL (Bit 24)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO6SEL_Msk (0xf000000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO6SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO7SEL_Pos (28UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO7SEL (Bit 28)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG0_LHO7SEL_Msk (0xf0000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG0: LHO7SEL (Bitfield-Mask: 0x0f) */
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG1  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO8SEL_Pos (0UL)            /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO8SEL (Bit 0)               */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO8SEL_Msk (0xfUL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO8SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO9SEL_Pos (4UL)            /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO9SEL (Bit 4)               */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO9SEL_Msk (0xf0UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO9SEL (Bitfield-Mask: 0x0f) */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO10SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO10SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO10SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO10SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO11SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO11SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO11SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO11SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO12SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO12SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO12SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO12SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO13SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO13SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO13SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO13SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO14SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO14SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO14SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO14SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO15SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO15SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG1_LHO15SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG1: LHO15SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG2  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO16SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO16SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO16SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO16SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO17SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO17SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO17SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO17SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO18SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO18SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO18SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO18SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO19SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO19SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO19SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO19SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO20SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO20SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO20SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO20SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO21SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO21SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO21SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO21SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO22SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO22SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO22SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO22SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO23SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO23SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG2_LHO23SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG2: LHO23SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG3  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO24SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO24SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO24SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO24SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO25SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO25SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO25SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO25SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO26SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO26SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO26SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO26SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO27SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO27SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO27SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO27SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO28SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO28SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO28SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO28SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO29SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO29SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO29SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO29SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO30SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO30SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO30SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO30SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO31SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO31SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG3_LHO31SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG3: LHO31SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG4  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO32SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO32SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO32SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO32SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO33SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO33SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO33SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO33SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO34SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO34SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO34SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO34SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO35SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO35SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO35SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO35SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO36SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO36SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO36SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO36SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO37SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO37SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO37SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO37SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO38SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO38SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO38SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO38SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO39SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO39SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG4_LHO39SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG4: LHO39SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG5  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO40SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO40SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO40SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO40SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO41SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO41SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO41SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO41SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO42SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO42SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO42SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO42SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO43SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO43SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO43SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO43SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO44SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO44SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO44SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO44SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO45SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO45SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO45SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO45SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO46SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO46SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO46SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO46SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO47SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO47SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG5_LHO47SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG5: LHO47SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG6  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO48SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO48SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO48SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO48SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO49SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO49SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO49SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO49SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO50SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO50SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO50SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO50SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO51SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO51SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO51SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO51SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO52SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO52SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO52SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO52SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO53SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO53SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO53SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO53SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO54SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO54SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO54SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO54SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO55SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO55SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG6_LHO55SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG6: LHO55SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG7  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO56SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO56SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO56SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO56SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO57SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO57SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO57SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO57SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO58SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO58SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO58SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO58SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO59SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO59SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO59SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO59SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO60SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO60SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO60SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO60SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO61SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO61SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO61SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO61SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO62SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO62SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO62SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO62SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO63SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO63SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG7_LHO63SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG7: LHO63SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG8  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO64SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO64SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO64SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO64SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO65SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO65SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO65SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO65SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO66SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO66SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO66SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO66SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO67SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO67SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO67SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO67SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO68SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO68SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO68SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO68SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO69SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO69SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO69SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO69SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO70SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO70SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO70SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO70SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO71SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO71SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG8_LHO71SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG8: LHO71SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG9  =============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO72SEL_Pos (0UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO72SEL (Bit 0)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO72SEL_Msk (0xfUL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO72SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO73SEL_Pos (4UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO73SEL (Bit 4)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO73SEL_Msk (0xf0UL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO73SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO74SEL_Pos (8UL)           /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO74SEL (Bit 8)              */
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO74SEL_Msk (0xf00UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO74SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO75SEL_Pos (12UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO75SEL (Bit 12)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO75SEL_Msk (0xf000UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO75SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO76SEL_Pos (16UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO76SEL (Bit 16)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO76SEL_Msk (0xf0000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO76SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO77SEL_Pos (20UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO77SEL (Bit 20)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO77SEL_Msk (0xf00000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO77SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO78SEL_Pos (24UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO78SEL (Bit 24)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO78SEL_Msk (0xf000000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO78SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO79SEL_Pos (28UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO79SEL (Bit 28)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG9_LHO79SEL_Msk (0xf0000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG9: LHO79SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG10  ============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO80SEL_Pos (0UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO80SEL (Bit 0)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO80SEL_Msk (0xfUL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO80SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO81SEL_Pos (4UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO81SEL (Bit 4)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO81SEL_Msk (0xf0UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO81SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO82SEL_Pos (8UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO82SEL (Bit 8)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO82SEL_Msk (0xf00UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO82SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO83SEL_Pos (12UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO83SEL (Bit 12)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO83SEL_Msk (0xf000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO83SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO84SEL_Pos (16UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO84SEL (Bit 16)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO84SEL_Msk (0xf0000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO84SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO85SEL_Pos (20UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO85SEL (Bit 20)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO85SEL_Msk (0xf00000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO85SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO86SEL_Pos (24UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO86SEL (Bit 24)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO86SEL_Msk (0xf000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO86SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO87SEL_Pos (28UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO87SEL (Bit 28)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG10_LHO87SEL_Msk (0xf0000000UL) /*!< UDB_UDBPAIR_ROUTE LHO_CFG10: LHO87SEL (Bitfield-Mask: 0x0f)*/
/* ==============================================  UDB_UDBPAIR_ROUTE.LHO_CFG11  ============================================== */
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO88SEL_Pos (0UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO88SEL (Bit 0)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO88SEL_Msk (0xfUL)        /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO88SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO89SEL_Pos (4UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO89SEL (Bit 4)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO89SEL_Msk (0xf0UL)       /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO89SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO90SEL_Pos (8UL)          /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO90SEL (Bit 8)             */
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO90SEL_Msk (0xf00UL)      /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO90SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO91SEL_Pos (12UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO91SEL (Bit 12)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO91SEL_Msk (0xf000UL)     /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO91SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO92SEL_Pos (16UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO92SEL (Bit 16)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO92SEL_Msk (0xf0000UL)    /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO92SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO93SEL_Pos (20UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO93SEL (Bit 20)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO93SEL_Msk (0xf00000UL)   /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO93SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO94SEL_Pos (24UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO94SEL (Bit 24)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO94SEL_Msk (0xf000000UL)  /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO94SEL (Bitfield-Mask: 0x0f)*/
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO95SEL_Pos (28UL)         /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO95SEL (Bit 28)            */
#define UDB_UDBPAIR_ROUTE_LHO_CFG11_LHO95SEL_Msk (0xf0000000UL) /*!< UDB_UDBPAIR_ROUTE LHO_CFG11: LHO95SEL (Bitfield-Mask: 0x0f)*/


/* ===================================================  UDB_DSI.LVI1_V_2  ==================================================== */
#define UDB_DSI_LVI1_V_2_LVI1_V_2_Pos     (0UL)                 /*!< UDB_DSI LVI1_V_2: LVI1_V_2 (Bit 0)                        */
#define UDB_DSI_LVI1_V_2_LVI1_V_2_Msk     (0xffffffffUL)        /*!< UDB_DSI LVI1_V_2: LVI1_V_2 (Bitfield-Mask: 0xffffffff)    */
/* ===================================================  UDB_DSI.RVI1_V_2  ==================================================== */
#define UDB_DSI_RVI1_V_2_RVI1_V_2_Pos     (0UL)                 /*!< UDB_DSI RVI1_V_2: RVI1_V_2 (Bit 0)                        */
#define UDB_DSI_RVI1_V_2_RVI1_V_2_Msk     (0xffffffffUL)        /*!< UDB_DSI RVI1_V_2: RVI1_V_2 (Bitfield-Mask: 0xffffffff)    */
/* ===================================================  UDB_DSI.DOP_CFG0  ==================================================== */
#define UDB_DSI_DOP_CFG0_DOP0SEL_Pos      (0UL)                 /*!< UDB_DSI DOP_CFG0: DOP0SEL (Bit 0)                         */
#define UDB_DSI_DOP_CFG0_DOP0SEL_Msk      (0x1fUL)              /*!< UDB_DSI DOP_CFG0: DOP0SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOP_CFG0_DOP1SEL_Pos      (8UL)                 /*!< UDB_DSI DOP_CFG0: DOP1SEL (Bit 8)                         */
#define UDB_DSI_DOP_CFG0_DOP1SEL_Msk      (0x1f00UL)            /*!< UDB_DSI DOP_CFG0: DOP1SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOP_CFG0_DOP2SEL_Pos      (16UL)                /*!< UDB_DSI DOP_CFG0: DOP2SEL (Bit 16)                        */
#define UDB_DSI_DOP_CFG0_DOP2SEL_Msk      (0x1f0000UL)          /*!< UDB_DSI DOP_CFG0: DOP2SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOP_CFG0_DOP3SEL_Pos      (24UL)                /*!< UDB_DSI DOP_CFG0: DOP3SEL (Bit 24)                        */
#define UDB_DSI_DOP_CFG0_DOP3SEL_Msk      (0x1f000000UL)        /*!< UDB_DSI DOP_CFG0: DOP3SEL (Bitfield-Mask: 0x1f)           */
/* ===================================================  UDB_DSI.DOP_CFG1  ==================================================== */
#define UDB_DSI_DOP_CFG1_DOP4SEL_Pos      (0UL)                 /*!< UDB_DSI DOP_CFG1: DOP4SEL (Bit 0)                         */
#define UDB_DSI_DOP_CFG1_DOP4SEL_Msk      (0x1fUL)              /*!< UDB_DSI DOP_CFG1: DOP4SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOP_CFG1_DOP5SEL_Pos      (8UL)                 /*!< UDB_DSI DOP_CFG1: DOP5SEL (Bit 8)                         */
#define UDB_DSI_DOP_CFG1_DOP5SEL_Msk      (0x1f00UL)            /*!< UDB_DSI DOP_CFG1: DOP5SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOP_CFG1_DOP6SEL_Pos      (16UL)                /*!< UDB_DSI DOP_CFG1: DOP6SEL (Bit 16)                        */
#define UDB_DSI_DOP_CFG1_DOP6SEL_Msk      (0x1f0000UL)          /*!< UDB_DSI DOP_CFG1: DOP6SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOP_CFG1_DOP7SEL_Pos      (24UL)                /*!< UDB_DSI DOP_CFG1: DOP7SEL (Bit 24)                        */
#define UDB_DSI_DOP_CFG1_DOP7SEL_Msk      (0x1f000000UL)        /*!< UDB_DSI DOP_CFG1: DOP7SEL (Bitfield-Mask: 0x1f)           */
/* ===================================================  UDB_DSI.DOP_CFG2  ==================================================== */
#define UDB_DSI_DOP_CFG2_DOP8SEL_Pos      (0UL)                 /*!< UDB_DSI DOP_CFG2: DOP8SEL (Bit 0)                         */
#define UDB_DSI_DOP_CFG2_DOP8SEL_Msk      (0x1fUL)              /*!< UDB_DSI DOP_CFG2: DOP8SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOP_CFG2_DOP9SEL_Pos      (8UL)                 /*!< UDB_DSI DOP_CFG2: DOP9SEL (Bit 8)                         */
#define UDB_DSI_DOP_CFG2_DOP9SEL_Msk      (0x1f00UL)            /*!< UDB_DSI DOP_CFG2: DOP9SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOP_CFG2_DOP10SEL_Pos     (16UL)                /*!< UDB_DSI DOP_CFG2: DOP10SEL (Bit 16)                       */
#define UDB_DSI_DOP_CFG2_DOP10SEL_Msk     (0x1f0000UL)          /*!< UDB_DSI DOP_CFG2: DOP10SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_DOP_CFG2_DOP11SEL_Pos     (24UL)                /*!< UDB_DSI DOP_CFG2: DOP11SEL (Bit 24)                       */
#define UDB_DSI_DOP_CFG2_DOP11SEL_Msk     (0x1f000000UL)        /*!< UDB_DSI DOP_CFG2: DOP11SEL (Bitfield-Mask: 0x1f)          */
/* ===================================================  UDB_DSI.DOP_CFG3  ==================================================== */
#define UDB_DSI_DOP_CFG3_DOP12SEL_Pos     (0UL)                 /*!< UDB_DSI DOP_CFG3: DOP12SEL (Bit 0)                        */
#define UDB_DSI_DOP_CFG3_DOP12SEL_Msk     (0x1fUL)              /*!< UDB_DSI DOP_CFG3: DOP12SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_DOP_CFG3_DOP13SEL_Pos     (8UL)                 /*!< UDB_DSI DOP_CFG3: DOP13SEL (Bit 8)                        */
#define UDB_DSI_DOP_CFG3_DOP13SEL_Msk     (0x1f00UL)            /*!< UDB_DSI DOP_CFG3: DOP13SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_DOP_CFG3_DOP14SEL_Pos     (16UL)                /*!< UDB_DSI DOP_CFG3: DOP14SEL (Bit 16)                       */
#define UDB_DSI_DOP_CFG3_DOP14SEL_Msk     (0x1f0000UL)          /*!< UDB_DSI DOP_CFG3: DOP14SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_DOP_CFG3_DOP15SEL_Pos     (24UL)                /*!< UDB_DSI DOP_CFG3: DOP15SEL (Bit 24)                       */
#define UDB_DSI_DOP_CFG3_DOP15SEL_Msk     (0x1f000000UL)        /*!< UDB_DSI DOP_CFG3: DOP15SEL (Bitfield-Mask: 0x1f)          */
/* ===================================================  UDB_DSI.DOT_CFG0  ==================================================== */
#define UDB_DSI_DOT_CFG0_DOT0SEL_Pos      (0UL)                 /*!< UDB_DSI DOT_CFG0: DOT0SEL (Bit 0)                         */
#define UDB_DSI_DOT_CFG0_DOT0SEL_Msk      (0x1fUL)              /*!< UDB_DSI DOT_CFG0: DOT0SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOT_CFG0_DOT1SEL_Pos      (8UL)                 /*!< UDB_DSI DOT_CFG0: DOT1SEL (Bit 8)                         */
#define UDB_DSI_DOT_CFG0_DOT1SEL_Msk      (0x1f00UL)            /*!< UDB_DSI DOT_CFG0: DOT1SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOT_CFG0_DOT2SEL_Pos      (16UL)                /*!< UDB_DSI DOT_CFG0: DOT2SEL (Bit 16)                        */
#define UDB_DSI_DOT_CFG0_DOT2SEL_Msk      (0x1f0000UL)          /*!< UDB_DSI DOT_CFG0: DOT2SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOT_CFG0_DOT3SEL_Pos      (24UL)                /*!< UDB_DSI DOT_CFG0: DOT3SEL (Bit 24)                        */
#define UDB_DSI_DOT_CFG0_DOT3SEL_Msk      (0x1f000000UL)        /*!< UDB_DSI DOT_CFG0: DOT3SEL (Bitfield-Mask: 0x1f)           */
/* ===================================================  UDB_DSI.DOT_CFG1  ==================================================== */
#define UDB_DSI_DOT_CFG1_DOT4SEL_Pos      (0UL)                 /*!< UDB_DSI DOT_CFG1: DOT4SEL (Bit 0)                         */
#define UDB_DSI_DOT_CFG1_DOT4SEL_Msk      (0x1fUL)              /*!< UDB_DSI DOT_CFG1: DOT4SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOT_CFG1_DOT5SEL_Pos      (8UL)                 /*!< UDB_DSI DOT_CFG1: DOT5SEL (Bit 8)                         */
#define UDB_DSI_DOT_CFG1_DOT5SEL_Msk      (0x1f00UL)            /*!< UDB_DSI DOT_CFG1: DOT5SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOT_CFG1_DOT6SEL_Pos      (16UL)                /*!< UDB_DSI DOT_CFG1: DOT6SEL (Bit 16)                        */
#define UDB_DSI_DOT_CFG1_DOT6SEL_Msk      (0x1f0000UL)          /*!< UDB_DSI DOT_CFG1: DOT6SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOT_CFG1_DOT7SEL_Pos      (24UL)                /*!< UDB_DSI DOT_CFG1: DOT7SEL (Bit 24)                        */
#define UDB_DSI_DOT_CFG1_DOT7SEL_Msk      (0x1f000000UL)        /*!< UDB_DSI DOT_CFG1: DOT7SEL (Bitfield-Mask: 0x1f)           */
/* ===================================================  UDB_DSI.DOT_CFG2  ==================================================== */
#define UDB_DSI_DOT_CFG2_DOT8SEL_Pos      (0UL)                 /*!< UDB_DSI DOT_CFG2: DOT8SEL (Bit 0)                         */
#define UDB_DSI_DOT_CFG2_DOT8SEL_Msk      (0x1fUL)              /*!< UDB_DSI DOT_CFG2: DOT8SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOT_CFG2_DOT9SEL_Pos      (8UL)                 /*!< UDB_DSI DOT_CFG2: DOT9SEL (Bit 8)                         */
#define UDB_DSI_DOT_CFG2_DOT9SEL_Msk      (0x1f00UL)            /*!< UDB_DSI DOT_CFG2: DOT9SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_DOT_CFG2_DOT10SEL_Pos     (16UL)                /*!< UDB_DSI DOT_CFG2: DOT10SEL (Bit 16)                       */
#define UDB_DSI_DOT_CFG2_DOT10SEL_Msk     (0x1f0000UL)          /*!< UDB_DSI DOT_CFG2: DOT10SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_DOT_CFG2_DOT11SEL_Pos     (24UL)                /*!< UDB_DSI DOT_CFG2: DOT11SEL (Bit 24)                       */
#define UDB_DSI_DOT_CFG2_DOT11SEL_Msk     (0x1f000000UL)        /*!< UDB_DSI DOT_CFG2: DOT11SEL (Bitfield-Mask: 0x1f)          */
/* ===================================================  UDB_DSI.DOT_CFG3  ==================================================== */
#define UDB_DSI_DOT_CFG3_DOT12SEL_Pos     (0UL)                 /*!< UDB_DSI DOT_CFG3: DOT12SEL (Bit 0)                        */
#define UDB_DSI_DOT_CFG3_DOT12SEL_Msk     (0x1fUL)              /*!< UDB_DSI DOT_CFG3: DOT12SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_DOT_CFG3_DOT13SEL_Pos     (8UL)                 /*!< UDB_DSI DOT_CFG3: DOT13SEL (Bit 8)                        */
#define UDB_DSI_DOT_CFG3_DOT13SEL_Msk     (0x1f00UL)            /*!< UDB_DSI DOT_CFG3: DOT13SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_DOT_CFG3_DOT14SEL_Pos     (16UL)                /*!< UDB_DSI DOT_CFG3: DOT14SEL (Bit 16)                       */
#define UDB_DSI_DOT_CFG3_DOT14SEL_Msk     (0x1f0000UL)          /*!< UDB_DSI DOT_CFG3: DOT14SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_DOT_CFG3_DOT15SEL_Pos     (24UL)                /*!< UDB_DSI DOT_CFG3: DOT15SEL (Bit 24)                       */
#define UDB_DSI_DOT_CFG3_DOT15SEL_Msk     (0x1f000000UL)        /*!< UDB_DSI DOT_CFG3: DOT15SEL (Bitfield-Mask: 0x1f)          */
/* ===================================================  UDB_DSI.RVO_CFG0  ==================================================== */
#define UDB_DSI_RVO_CFG0_RVO0SEL_Pos      (0UL)                 /*!< UDB_DSI RVO_CFG0: RVO0SEL (Bit 0)                         */
#define UDB_DSI_RVO_CFG0_RVO0SEL_Msk      (0x1fUL)              /*!< UDB_DSI RVO_CFG0: RVO0SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_RVO_CFG0_RVO1SEL_Pos      (8UL)                 /*!< UDB_DSI RVO_CFG0: RVO1SEL (Bit 8)                         */
#define UDB_DSI_RVO_CFG0_RVO1SEL_Msk      (0x1f00UL)            /*!< UDB_DSI RVO_CFG0: RVO1SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_RVO_CFG0_RVO2SEL_Pos      (16UL)                /*!< UDB_DSI RVO_CFG0: RVO2SEL (Bit 16)                        */
#define UDB_DSI_RVO_CFG0_RVO2SEL_Msk      (0x1f0000UL)          /*!< UDB_DSI RVO_CFG0: RVO2SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_RVO_CFG0_RVO3SEL_Pos      (24UL)                /*!< UDB_DSI RVO_CFG0: RVO3SEL (Bit 24)                        */
#define UDB_DSI_RVO_CFG0_RVO3SEL_Msk      (0x1f000000UL)        /*!< UDB_DSI RVO_CFG0: RVO3SEL (Bitfield-Mask: 0x1f)           */
/* ===================================================  UDB_DSI.RVO_CFG1  ==================================================== */
#define UDB_DSI_RVO_CFG1_RVO4SEL_Pos      (0UL)                 /*!< UDB_DSI RVO_CFG1: RVO4SEL (Bit 0)                         */
#define UDB_DSI_RVO_CFG1_RVO4SEL_Msk      (0x1fUL)              /*!< UDB_DSI RVO_CFG1: RVO4SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_RVO_CFG1_RVO5SEL_Pos      (8UL)                 /*!< UDB_DSI RVO_CFG1: RVO5SEL (Bit 8)                         */
#define UDB_DSI_RVO_CFG1_RVO5SEL_Msk      (0x1f00UL)            /*!< UDB_DSI RVO_CFG1: RVO5SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_RVO_CFG1_RVO6SEL_Pos      (16UL)                /*!< UDB_DSI RVO_CFG1: RVO6SEL (Bit 16)                        */
#define UDB_DSI_RVO_CFG1_RVO6SEL_Msk      (0x1f0000UL)          /*!< UDB_DSI RVO_CFG1: RVO6SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_RVO_CFG1_RVO7SEL_Pos      (24UL)                /*!< UDB_DSI RVO_CFG1: RVO7SEL (Bit 24)                        */
#define UDB_DSI_RVO_CFG1_RVO7SEL_Msk      (0x1f000000UL)        /*!< UDB_DSI RVO_CFG1: RVO7SEL (Bitfield-Mask: 0x1f)           */
/* ===================================================  UDB_DSI.RVO_CFG2  ==================================================== */
#define UDB_DSI_RVO_CFG2_RVO8SEL_Pos      (0UL)                 /*!< UDB_DSI RVO_CFG2: RVO8SEL (Bit 0)                         */
#define UDB_DSI_RVO_CFG2_RVO8SEL_Msk      (0x1fUL)              /*!< UDB_DSI RVO_CFG2: RVO8SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_RVO_CFG2_RVO9SEL_Pos      (8UL)                 /*!< UDB_DSI RVO_CFG2: RVO9SEL (Bit 8)                         */
#define UDB_DSI_RVO_CFG2_RVO9SEL_Msk      (0x1f00UL)            /*!< UDB_DSI RVO_CFG2: RVO9SEL (Bitfield-Mask: 0x1f)           */
#define UDB_DSI_RVO_CFG2_RVO10SEL_Pos     (16UL)                /*!< UDB_DSI RVO_CFG2: RVO10SEL (Bit 16)                       */
#define UDB_DSI_RVO_CFG2_RVO10SEL_Msk     (0x1f0000UL)          /*!< UDB_DSI RVO_CFG2: RVO10SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_RVO_CFG2_RVO11SEL_Pos     (24UL)                /*!< UDB_DSI RVO_CFG2: RVO11SEL (Bit 24)                       */
#define UDB_DSI_RVO_CFG2_RVO11SEL_Msk     (0x1f000000UL)        /*!< UDB_DSI RVO_CFG2: RVO11SEL (Bitfield-Mask: 0x1f)          */
/* ===================================================  UDB_DSI.RVO_CFG3  ==================================================== */
#define UDB_DSI_RVO_CFG3_RVO12SEL_Pos     (0UL)                 /*!< UDB_DSI RVO_CFG3: RVO12SEL (Bit 0)                        */
#define UDB_DSI_RVO_CFG3_RVO12SEL_Msk     (0x1fUL)              /*!< UDB_DSI RVO_CFG3: RVO12SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_RVO_CFG3_RVO13SEL_Pos     (8UL)                 /*!< UDB_DSI RVO_CFG3: RVO13SEL (Bit 8)                        */
#define UDB_DSI_RVO_CFG3_RVO13SEL_Msk     (0x1f00UL)            /*!< UDB_DSI RVO_CFG3: RVO13SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_RVO_CFG3_RVO14SEL_Pos     (16UL)                /*!< UDB_DSI RVO_CFG3: RVO14SEL (Bit 16)                       */
#define UDB_DSI_RVO_CFG3_RVO14SEL_Msk     (0x1f0000UL)          /*!< UDB_DSI RVO_CFG3: RVO14SEL (Bitfield-Mask: 0x1f)          */
#define UDB_DSI_RVO_CFG3_RVO15SEL_Pos     (24UL)                /*!< UDB_DSI RVO_CFG3: RVO15SEL (Bit 24)                       */
#define UDB_DSI_RVO_CFG3_RVO15SEL_Msk     (0x1f000000UL)        /*!< UDB_DSI RVO_CFG3: RVO15SEL (Bitfield-Mask: 0x1f)          */
/* ===================================================  UDB_DSI.LVO_CFG0  ==================================================== */
#define UDB_DSI_LVO_CFG0_LVO0SEL_Pos      (0UL)                 /*!< UDB_DSI LVO_CFG0: LVO0SEL (Bit 0)                         */
#define UDB_DSI_LVO_CFG0_LVO0SEL_Msk      (0xfUL)               /*!< UDB_DSI LVO_CFG0: LVO0SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LVO_CFG0_LVO1SEL_Pos      (4UL)                 /*!< UDB_DSI LVO_CFG0: LVO1SEL (Bit 4)                         */
#define UDB_DSI_LVO_CFG0_LVO1SEL_Msk      (0xf0UL)              /*!< UDB_DSI LVO_CFG0: LVO1SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LVO_CFG0_LVO2SEL_Pos      (8UL)                 /*!< UDB_DSI LVO_CFG0: LVO2SEL (Bit 8)                         */
#define UDB_DSI_LVO_CFG0_LVO2SEL_Msk      (0xf00UL)             /*!< UDB_DSI LVO_CFG0: LVO2SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LVO_CFG0_LVO3SEL_Pos      (12UL)                /*!< UDB_DSI LVO_CFG0: LVO3SEL (Bit 12)                        */
#define UDB_DSI_LVO_CFG0_LVO3SEL_Msk      (0xf000UL)            /*!< UDB_DSI LVO_CFG0: LVO3SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LVO_CFG0_LVO4SEL_Pos      (16UL)                /*!< UDB_DSI LVO_CFG0: LVO4SEL (Bit 16)                        */
#define UDB_DSI_LVO_CFG0_LVO4SEL_Msk      (0xf0000UL)           /*!< UDB_DSI LVO_CFG0: LVO4SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LVO_CFG0_LVO5SEL_Pos      (20UL)                /*!< UDB_DSI LVO_CFG0: LVO5SEL (Bit 20)                        */
#define UDB_DSI_LVO_CFG0_LVO5SEL_Msk      (0xf00000UL)          /*!< UDB_DSI LVO_CFG0: LVO5SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LVO_CFG0_LVO6SEL_Pos      (24UL)                /*!< UDB_DSI LVO_CFG0: LVO6SEL (Bit 24)                        */
#define UDB_DSI_LVO_CFG0_LVO6SEL_Msk      (0xf000000UL)         /*!< UDB_DSI LVO_CFG0: LVO6SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LVO_CFG0_LVO7SEL_Pos      (28UL)                /*!< UDB_DSI LVO_CFG0: LVO7SEL (Bit 28)                        */
#define UDB_DSI_LVO_CFG0_LVO7SEL_Msk      (0xf0000000UL)        /*!< UDB_DSI LVO_CFG0: LVO7SEL (Bitfield-Mask: 0x0f)           */
/* ===================================================  UDB_DSI.LVO_CFG1  ==================================================== */
#define UDB_DSI_LVO_CFG1_LVO8SEL_Pos      (0UL)                 /*!< UDB_DSI LVO_CFG1: LVO8SEL (Bit 0)                         */
#define UDB_DSI_LVO_CFG1_LVO8SEL_Msk      (0xfUL)               /*!< UDB_DSI LVO_CFG1: LVO8SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LVO_CFG1_LVO9SEL_Pos      (4UL)                 /*!< UDB_DSI LVO_CFG1: LVO9SEL (Bit 4)                         */
#define UDB_DSI_LVO_CFG1_LVO9SEL_Msk      (0xf0UL)              /*!< UDB_DSI LVO_CFG1: LVO9SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LVO_CFG1_LVO10SEL_Pos     (8UL)                 /*!< UDB_DSI LVO_CFG1: LVO10SEL (Bit 8)                        */
#define UDB_DSI_LVO_CFG1_LVO10SEL_Msk     (0xf00UL)             /*!< UDB_DSI LVO_CFG1: LVO10SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LVO_CFG1_LVO11SEL_Pos     (12UL)                /*!< UDB_DSI LVO_CFG1: LVO11SEL (Bit 12)                       */
#define UDB_DSI_LVO_CFG1_LVO11SEL_Msk     (0xf000UL)            /*!< UDB_DSI LVO_CFG1: LVO11SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LVO_CFG1_LVO12SEL_Pos     (16UL)                /*!< UDB_DSI LVO_CFG1: LVO12SEL (Bit 16)                       */
#define UDB_DSI_LVO_CFG1_LVO12SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LVO_CFG1: LVO12SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LVO_CFG1_LVO13SEL_Pos     (20UL)                /*!< UDB_DSI LVO_CFG1: LVO13SEL (Bit 20)                       */
#define UDB_DSI_LVO_CFG1_LVO13SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LVO_CFG1: LVO13SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LVO_CFG1_LVO14SEL_Pos     (24UL)                /*!< UDB_DSI LVO_CFG1: LVO14SEL (Bit 24)                       */
#define UDB_DSI_LVO_CFG1_LVO14SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LVO_CFG1: LVO14SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LVO_CFG1_LVO15SEL_Pos     (28UL)                /*!< UDB_DSI LVO_CFG1: LVO15SEL (Bit 28)                       */
#define UDB_DSI_LVO_CFG1_LVO15SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LVO_CFG1: LVO15SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.RHO_CFG0  ==================================================== */
#define UDB_DSI_RHO_CFG0_RHOSEL_Pos       (0UL)                 /*!< UDB_DSI RHO_CFG0: RHOSEL (Bit 0)                          */
#define UDB_DSI_RHO_CFG0_RHOSEL_Msk       (0xffffffffUL)        /*!< UDB_DSI RHO_CFG0: RHOSEL (Bitfield-Mask: 0xffffffff)      */
/* ===================================================  UDB_DSI.RHO_CFG1  ==================================================== */
#define UDB_DSI_RHO_CFG1_RHOSEL_Pos       (0UL)                 /*!< UDB_DSI RHO_CFG1: RHOSEL (Bit 0)                          */
#define UDB_DSI_RHO_CFG1_RHOSEL_Msk       (0xffffffffUL)        /*!< UDB_DSI RHO_CFG1: RHOSEL (Bitfield-Mask: 0xffffffff)      */
/* ===================================================  UDB_DSI.RHO_CFG2  ==================================================== */
#define UDB_DSI_RHO_CFG2_RHOSEL_Pos       (0UL)                 /*!< UDB_DSI RHO_CFG2: RHOSEL (Bit 0)                          */
#define UDB_DSI_RHO_CFG2_RHOSEL_Msk       (0xffffffffUL)        /*!< UDB_DSI RHO_CFG2: RHOSEL (Bitfield-Mask: 0xffffffff)      */
/* ===================================================  UDB_DSI.LHO_CFG0  ==================================================== */
#define UDB_DSI_LHO_CFG0_LHO0SEL_Pos      (0UL)                 /*!< UDB_DSI LHO_CFG0: LHO0SEL (Bit 0)                         */
#define UDB_DSI_LHO_CFG0_LHO0SEL_Msk      (0xfUL)               /*!< UDB_DSI LHO_CFG0: LHO0SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LHO_CFG0_LHO1SEL_Pos      (4UL)                 /*!< UDB_DSI LHO_CFG0: LHO1SEL (Bit 4)                         */
#define UDB_DSI_LHO_CFG0_LHO1SEL_Msk      (0xf0UL)              /*!< UDB_DSI LHO_CFG0: LHO1SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LHO_CFG0_LHO2SEL_Pos      (8UL)                 /*!< UDB_DSI LHO_CFG0: LHO2SEL (Bit 8)                         */
#define UDB_DSI_LHO_CFG0_LHO2SEL_Msk      (0xf00UL)             /*!< UDB_DSI LHO_CFG0: LHO2SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LHO_CFG0_LHO3SEL_Pos      (12UL)                /*!< UDB_DSI LHO_CFG0: LHO3SEL (Bit 12)                        */
#define UDB_DSI_LHO_CFG0_LHO3SEL_Msk      (0xf000UL)            /*!< UDB_DSI LHO_CFG0: LHO3SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LHO_CFG0_LHO4SEL_Pos      (16UL)                /*!< UDB_DSI LHO_CFG0: LHO4SEL (Bit 16)                        */
#define UDB_DSI_LHO_CFG0_LHO4SEL_Msk      (0xf0000UL)           /*!< UDB_DSI LHO_CFG0: LHO4SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LHO_CFG0_LHO5SEL_Pos      (20UL)                /*!< UDB_DSI LHO_CFG0: LHO5SEL (Bit 20)                        */
#define UDB_DSI_LHO_CFG0_LHO5SEL_Msk      (0xf00000UL)          /*!< UDB_DSI LHO_CFG0: LHO5SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LHO_CFG0_LHO6SEL_Pos      (24UL)                /*!< UDB_DSI LHO_CFG0: LHO6SEL (Bit 24)                        */
#define UDB_DSI_LHO_CFG0_LHO6SEL_Msk      (0xf000000UL)         /*!< UDB_DSI LHO_CFG0: LHO6SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LHO_CFG0_LHO7SEL_Pos      (28UL)                /*!< UDB_DSI LHO_CFG0: LHO7SEL (Bit 28)                        */
#define UDB_DSI_LHO_CFG0_LHO7SEL_Msk      (0xf0000000UL)        /*!< UDB_DSI LHO_CFG0: LHO7SEL (Bitfield-Mask: 0x0f)           */
/* ===================================================  UDB_DSI.LHO_CFG1  ==================================================== */
#define UDB_DSI_LHO_CFG1_LHO8SEL_Pos      (0UL)                 /*!< UDB_DSI LHO_CFG1: LHO8SEL (Bit 0)                         */
#define UDB_DSI_LHO_CFG1_LHO8SEL_Msk      (0xfUL)               /*!< UDB_DSI LHO_CFG1: LHO8SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LHO_CFG1_LHO9SEL_Pos      (4UL)                 /*!< UDB_DSI LHO_CFG1: LHO9SEL (Bit 4)                         */
#define UDB_DSI_LHO_CFG1_LHO9SEL_Msk      (0xf0UL)              /*!< UDB_DSI LHO_CFG1: LHO9SEL (Bitfield-Mask: 0x0f)           */
#define UDB_DSI_LHO_CFG1_LHO10SEL_Pos     (8UL)                 /*!< UDB_DSI LHO_CFG1: LHO10SEL (Bit 8)                        */
#define UDB_DSI_LHO_CFG1_LHO10SEL_Msk     (0xf00UL)             /*!< UDB_DSI LHO_CFG1: LHO10SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG1_LHO11SEL_Pos     (12UL)                /*!< UDB_DSI LHO_CFG1: LHO11SEL (Bit 12)                       */
#define UDB_DSI_LHO_CFG1_LHO11SEL_Msk     (0xf000UL)            /*!< UDB_DSI LHO_CFG1: LHO11SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG1_LHO12SEL_Pos     (16UL)                /*!< UDB_DSI LHO_CFG1: LHO12SEL (Bit 16)                       */
#define UDB_DSI_LHO_CFG1_LHO12SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LHO_CFG1: LHO12SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG1_LHO13SEL_Pos     (20UL)                /*!< UDB_DSI LHO_CFG1: LHO13SEL (Bit 20)                       */
#define UDB_DSI_LHO_CFG1_LHO13SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LHO_CFG1: LHO13SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG1_LHO14SEL_Pos     (24UL)                /*!< UDB_DSI LHO_CFG1: LHO14SEL (Bit 24)                       */
#define UDB_DSI_LHO_CFG1_LHO14SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LHO_CFG1: LHO14SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG1_LHO15SEL_Pos     (28UL)                /*!< UDB_DSI LHO_CFG1: LHO15SEL (Bit 28)                       */
#define UDB_DSI_LHO_CFG1_LHO15SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LHO_CFG1: LHO15SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.LHO_CFG2  ==================================================== */
#define UDB_DSI_LHO_CFG2_LHO16SEL_Pos     (0UL)                 /*!< UDB_DSI LHO_CFG2: LHO16SEL (Bit 0)                        */
#define UDB_DSI_LHO_CFG2_LHO16SEL_Msk     (0xfUL)               /*!< UDB_DSI LHO_CFG2: LHO16SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG2_LHO17SEL_Pos     (4UL)                 /*!< UDB_DSI LHO_CFG2: LHO17SEL (Bit 4)                        */
#define UDB_DSI_LHO_CFG2_LHO17SEL_Msk     (0xf0UL)              /*!< UDB_DSI LHO_CFG2: LHO17SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG2_LHO18SEL_Pos     (8UL)                 /*!< UDB_DSI LHO_CFG2: LHO18SEL (Bit 8)                        */
#define UDB_DSI_LHO_CFG2_LHO18SEL_Msk     (0xf00UL)             /*!< UDB_DSI LHO_CFG2: LHO18SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG2_LHO19SEL_Pos     (12UL)                /*!< UDB_DSI LHO_CFG2: LHO19SEL (Bit 12)                       */
#define UDB_DSI_LHO_CFG2_LHO19SEL_Msk     (0xf000UL)            /*!< UDB_DSI LHO_CFG2: LHO19SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG2_LHO20SEL_Pos     (16UL)                /*!< UDB_DSI LHO_CFG2: LHO20SEL (Bit 16)                       */
#define UDB_DSI_LHO_CFG2_LHO20SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LHO_CFG2: LHO20SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG2_LHO21SEL_Pos     (20UL)                /*!< UDB_DSI LHO_CFG2: LHO21SEL (Bit 20)                       */
#define UDB_DSI_LHO_CFG2_LHO21SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LHO_CFG2: LHO21SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG2_LHO22SEL_Pos     (24UL)                /*!< UDB_DSI LHO_CFG2: LHO22SEL (Bit 24)                       */
#define UDB_DSI_LHO_CFG2_LHO22SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LHO_CFG2: LHO22SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG2_LHO23SEL_Pos     (28UL)                /*!< UDB_DSI LHO_CFG2: LHO23SEL (Bit 28)                       */
#define UDB_DSI_LHO_CFG2_LHO23SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LHO_CFG2: LHO23SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.LHO_CFG3  ==================================================== */
#define UDB_DSI_LHO_CFG3_LHO24SEL_Pos     (0UL)                 /*!< UDB_DSI LHO_CFG3: LHO24SEL (Bit 0)                        */
#define UDB_DSI_LHO_CFG3_LHO24SEL_Msk     (0xfUL)               /*!< UDB_DSI LHO_CFG3: LHO24SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG3_LHO25SEL_Pos     (4UL)                 /*!< UDB_DSI LHO_CFG3: LHO25SEL (Bit 4)                        */
#define UDB_DSI_LHO_CFG3_LHO25SEL_Msk     (0xf0UL)              /*!< UDB_DSI LHO_CFG3: LHO25SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG3_LHO26SEL_Pos     (8UL)                 /*!< UDB_DSI LHO_CFG3: LHO26SEL (Bit 8)                        */
#define UDB_DSI_LHO_CFG3_LHO26SEL_Msk     (0xf00UL)             /*!< UDB_DSI LHO_CFG3: LHO26SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG3_LHO27SEL_Pos     (12UL)                /*!< UDB_DSI LHO_CFG3: LHO27SEL (Bit 12)                       */
#define UDB_DSI_LHO_CFG3_LHO27SEL_Msk     (0xf000UL)            /*!< UDB_DSI LHO_CFG3: LHO27SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG3_LHO28SEL_Pos     (16UL)                /*!< UDB_DSI LHO_CFG3: LHO28SEL (Bit 16)                       */
#define UDB_DSI_LHO_CFG3_LHO28SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LHO_CFG3: LHO28SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG3_LHO29SEL_Pos     (20UL)                /*!< UDB_DSI LHO_CFG3: LHO29SEL (Bit 20)                       */
#define UDB_DSI_LHO_CFG3_LHO29SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LHO_CFG3: LHO29SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG3_LHO30SEL_Pos     (24UL)                /*!< UDB_DSI LHO_CFG3: LHO30SEL (Bit 24)                       */
#define UDB_DSI_LHO_CFG3_LHO30SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LHO_CFG3: LHO30SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG3_LHO31SEL_Pos     (28UL)                /*!< UDB_DSI LHO_CFG3: LHO31SEL (Bit 28)                       */
#define UDB_DSI_LHO_CFG3_LHO31SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LHO_CFG3: LHO31SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.LHO_CFG4  ==================================================== */
#define UDB_DSI_LHO_CFG4_LHO32SEL_Pos     (0UL)                 /*!< UDB_DSI LHO_CFG4: LHO32SEL (Bit 0)                        */
#define UDB_DSI_LHO_CFG4_LHO32SEL_Msk     (0xfUL)               /*!< UDB_DSI LHO_CFG4: LHO32SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG4_LHO33SEL_Pos     (4UL)                 /*!< UDB_DSI LHO_CFG4: LHO33SEL (Bit 4)                        */
#define UDB_DSI_LHO_CFG4_LHO33SEL_Msk     (0xf0UL)              /*!< UDB_DSI LHO_CFG4: LHO33SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG4_LHO34SEL_Pos     (8UL)                 /*!< UDB_DSI LHO_CFG4: LHO34SEL (Bit 8)                        */
#define UDB_DSI_LHO_CFG4_LHO34SEL_Msk     (0xf00UL)             /*!< UDB_DSI LHO_CFG4: LHO34SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG4_LHO35SEL_Pos     (12UL)                /*!< UDB_DSI LHO_CFG4: LHO35SEL (Bit 12)                       */
#define UDB_DSI_LHO_CFG4_LHO35SEL_Msk     (0xf000UL)            /*!< UDB_DSI LHO_CFG4: LHO35SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG4_LHO36SEL_Pos     (16UL)                /*!< UDB_DSI LHO_CFG4: LHO36SEL (Bit 16)                       */
#define UDB_DSI_LHO_CFG4_LHO36SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LHO_CFG4: LHO36SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG4_LHO37SEL_Pos     (20UL)                /*!< UDB_DSI LHO_CFG4: LHO37SEL (Bit 20)                       */
#define UDB_DSI_LHO_CFG4_LHO37SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LHO_CFG4: LHO37SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG4_LHO38SEL_Pos     (24UL)                /*!< UDB_DSI LHO_CFG4: LHO38SEL (Bit 24)                       */
#define UDB_DSI_LHO_CFG4_LHO38SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LHO_CFG4: LHO38SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG4_LHO39SEL_Pos     (28UL)                /*!< UDB_DSI LHO_CFG4: LHO39SEL (Bit 28)                       */
#define UDB_DSI_LHO_CFG4_LHO39SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LHO_CFG4: LHO39SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.LHO_CFG5  ==================================================== */
#define UDB_DSI_LHO_CFG5_LHO40SEL_Pos     (0UL)                 /*!< UDB_DSI LHO_CFG5: LHO40SEL (Bit 0)                        */
#define UDB_DSI_LHO_CFG5_LHO40SEL_Msk     (0xfUL)               /*!< UDB_DSI LHO_CFG5: LHO40SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG5_LHO41SEL_Pos     (4UL)                 /*!< UDB_DSI LHO_CFG5: LHO41SEL (Bit 4)                        */
#define UDB_DSI_LHO_CFG5_LHO41SEL_Msk     (0xf0UL)              /*!< UDB_DSI LHO_CFG5: LHO41SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG5_LHO42SEL_Pos     (8UL)                 /*!< UDB_DSI LHO_CFG5: LHO42SEL (Bit 8)                        */
#define UDB_DSI_LHO_CFG5_LHO42SEL_Msk     (0xf00UL)             /*!< UDB_DSI LHO_CFG5: LHO42SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG5_LHO43SEL_Pos     (12UL)                /*!< UDB_DSI LHO_CFG5: LHO43SEL (Bit 12)                       */
#define UDB_DSI_LHO_CFG5_LHO43SEL_Msk     (0xf000UL)            /*!< UDB_DSI LHO_CFG5: LHO43SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG5_LHO44SEL_Pos     (16UL)                /*!< UDB_DSI LHO_CFG5: LHO44SEL (Bit 16)                       */
#define UDB_DSI_LHO_CFG5_LHO44SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LHO_CFG5: LHO44SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG5_LHO45SEL_Pos     (20UL)                /*!< UDB_DSI LHO_CFG5: LHO45SEL (Bit 20)                       */
#define UDB_DSI_LHO_CFG5_LHO45SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LHO_CFG5: LHO45SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG5_LHO46SEL_Pos     (24UL)                /*!< UDB_DSI LHO_CFG5: LHO46SEL (Bit 24)                       */
#define UDB_DSI_LHO_CFG5_LHO46SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LHO_CFG5: LHO46SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG5_LHO47SEL_Pos     (28UL)                /*!< UDB_DSI LHO_CFG5: LHO47SEL (Bit 28)                       */
#define UDB_DSI_LHO_CFG5_LHO47SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LHO_CFG5: LHO47SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.LHO_CFG6  ==================================================== */
#define UDB_DSI_LHO_CFG6_LHO48SEL_Pos     (0UL)                 /*!< UDB_DSI LHO_CFG6: LHO48SEL (Bit 0)                        */
#define UDB_DSI_LHO_CFG6_LHO48SEL_Msk     (0xfUL)               /*!< UDB_DSI LHO_CFG6: LHO48SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG6_LHO49SEL_Pos     (4UL)                 /*!< UDB_DSI LHO_CFG6: LHO49SEL (Bit 4)                        */
#define UDB_DSI_LHO_CFG6_LHO49SEL_Msk     (0xf0UL)              /*!< UDB_DSI LHO_CFG6: LHO49SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG6_LHO50SEL_Pos     (8UL)                 /*!< UDB_DSI LHO_CFG6: LHO50SEL (Bit 8)                        */
#define UDB_DSI_LHO_CFG6_LHO50SEL_Msk     (0xf00UL)             /*!< UDB_DSI LHO_CFG6: LHO50SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG6_LHO51SEL_Pos     (12UL)                /*!< UDB_DSI LHO_CFG6: LHO51SEL (Bit 12)                       */
#define UDB_DSI_LHO_CFG6_LHO51SEL_Msk     (0xf000UL)            /*!< UDB_DSI LHO_CFG6: LHO51SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG6_LHO52SEL_Pos     (16UL)                /*!< UDB_DSI LHO_CFG6: LHO52SEL (Bit 16)                       */
#define UDB_DSI_LHO_CFG6_LHO52SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LHO_CFG6: LHO52SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG6_LHO53SEL_Pos     (20UL)                /*!< UDB_DSI LHO_CFG6: LHO53SEL (Bit 20)                       */
#define UDB_DSI_LHO_CFG6_LHO53SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LHO_CFG6: LHO53SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG6_LHO54SEL_Pos     (24UL)                /*!< UDB_DSI LHO_CFG6: LHO54SEL (Bit 24)                       */
#define UDB_DSI_LHO_CFG6_LHO54SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LHO_CFG6: LHO54SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG6_LHO55SEL_Pos     (28UL)                /*!< UDB_DSI LHO_CFG6: LHO55SEL (Bit 28)                       */
#define UDB_DSI_LHO_CFG6_LHO55SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LHO_CFG6: LHO55SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.LHO_CFG7  ==================================================== */
#define UDB_DSI_LHO_CFG7_LHO56SEL_Pos     (0UL)                 /*!< UDB_DSI LHO_CFG7: LHO56SEL (Bit 0)                        */
#define UDB_DSI_LHO_CFG7_LHO56SEL_Msk     (0xfUL)               /*!< UDB_DSI LHO_CFG7: LHO56SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG7_LHO57SEL_Pos     (4UL)                 /*!< UDB_DSI LHO_CFG7: LHO57SEL (Bit 4)                        */
#define UDB_DSI_LHO_CFG7_LHO57SEL_Msk     (0xf0UL)              /*!< UDB_DSI LHO_CFG7: LHO57SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG7_LHO58SEL_Pos     (8UL)                 /*!< UDB_DSI LHO_CFG7: LHO58SEL (Bit 8)                        */
#define UDB_DSI_LHO_CFG7_LHO58SEL_Msk     (0xf00UL)             /*!< UDB_DSI LHO_CFG7: LHO58SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG7_LHO59SEL_Pos     (12UL)                /*!< UDB_DSI LHO_CFG7: LHO59SEL (Bit 12)                       */
#define UDB_DSI_LHO_CFG7_LHO59SEL_Msk     (0xf000UL)            /*!< UDB_DSI LHO_CFG7: LHO59SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG7_LHO60SEL_Pos     (16UL)                /*!< UDB_DSI LHO_CFG7: LHO60SEL (Bit 16)                       */
#define UDB_DSI_LHO_CFG7_LHO60SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LHO_CFG7: LHO60SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG7_LHO61SEL_Pos     (20UL)                /*!< UDB_DSI LHO_CFG7: LHO61SEL (Bit 20)                       */
#define UDB_DSI_LHO_CFG7_LHO61SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LHO_CFG7: LHO61SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG7_LHO62SEL_Pos     (24UL)                /*!< UDB_DSI LHO_CFG7: LHO62SEL (Bit 24)                       */
#define UDB_DSI_LHO_CFG7_LHO62SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LHO_CFG7: LHO62SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG7_LHO63SEL_Pos     (28UL)                /*!< UDB_DSI LHO_CFG7: LHO63SEL (Bit 28)                       */
#define UDB_DSI_LHO_CFG7_LHO63SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LHO_CFG7: LHO63SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.LHO_CFG8  ==================================================== */
#define UDB_DSI_LHO_CFG8_LHO64SEL_Pos     (0UL)                 /*!< UDB_DSI LHO_CFG8: LHO64SEL (Bit 0)                        */
#define UDB_DSI_LHO_CFG8_LHO64SEL_Msk     (0xfUL)               /*!< UDB_DSI LHO_CFG8: LHO64SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG8_LHO65SEL_Pos     (4UL)                 /*!< UDB_DSI LHO_CFG8: LHO65SEL (Bit 4)                        */
#define UDB_DSI_LHO_CFG8_LHO65SEL_Msk     (0xf0UL)              /*!< UDB_DSI LHO_CFG8: LHO65SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG8_LHO66SEL_Pos     (8UL)                 /*!< UDB_DSI LHO_CFG8: LHO66SEL (Bit 8)                        */
#define UDB_DSI_LHO_CFG8_LHO66SEL_Msk     (0xf00UL)             /*!< UDB_DSI LHO_CFG8: LHO66SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG8_LHO67SEL_Pos     (12UL)                /*!< UDB_DSI LHO_CFG8: LHO67SEL (Bit 12)                       */
#define UDB_DSI_LHO_CFG8_LHO67SEL_Msk     (0xf000UL)            /*!< UDB_DSI LHO_CFG8: LHO67SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG8_LHO68SEL_Pos     (16UL)                /*!< UDB_DSI LHO_CFG8: LHO68SEL (Bit 16)                       */
#define UDB_DSI_LHO_CFG8_LHO68SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LHO_CFG8: LHO68SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG8_LHO69SEL_Pos     (20UL)                /*!< UDB_DSI LHO_CFG8: LHO69SEL (Bit 20)                       */
#define UDB_DSI_LHO_CFG8_LHO69SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LHO_CFG8: LHO69SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG8_LHO70SEL_Pos     (24UL)                /*!< UDB_DSI LHO_CFG8: LHO70SEL (Bit 24)                       */
#define UDB_DSI_LHO_CFG8_LHO70SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LHO_CFG8: LHO70SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG8_LHO71SEL_Pos     (28UL)                /*!< UDB_DSI LHO_CFG8: LHO71SEL (Bit 28)                       */
#define UDB_DSI_LHO_CFG8_LHO71SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LHO_CFG8: LHO71SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.LHO_CFG9  ==================================================== */
#define UDB_DSI_LHO_CFG9_LHO72SEL_Pos     (0UL)                 /*!< UDB_DSI LHO_CFG9: LHO72SEL (Bit 0)                        */
#define UDB_DSI_LHO_CFG9_LHO72SEL_Msk     (0xfUL)               /*!< UDB_DSI LHO_CFG9: LHO72SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG9_LHO73SEL_Pos     (4UL)                 /*!< UDB_DSI LHO_CFG9: LHO73SEL (Bit 4)                        */
#define UDB_DSI_LHO_CFG9_LHO73SEL_Msk     (0xf0UL)              /*!< UDB_DSI LHO_CFG9: LHO73SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG9_LHO74SEL_Pos     (8UL)                 /*!< UDB_DSI LHO_CFG9: LHO74SEL (Bit 8)                        */
#define UDB_DSI_LHO_CFG9_LHO74SEL_Msk     (0xf00UL)             /*!< UDB_DSI LHO_CFG9: LHO74SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG9_LHO75SEL_Pos     (12UL)                /*!< UDB_DSI LHO_CFG9: LHO75SEL (Bit 12)                       */
#define UDB_DSI_LHO_CFG9_LHO75SEL_Msk     (0xf000UL)            /*!< UDB_DSI LHO_CFG9: LHO75SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG9_LHO76SEL_Pos     (16UL)                /*!< UDB_DSI LHO_CFG9: LHO76SEL (Bit 16)                       */
#define UDB_DSI_LHO_CFG9_LHO76SEL_Msk     (0xf0000UL)           /*!< UDB_DSI LHO_CFG9: LHO76SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG9_LHO77SEL_Pos     (20UL)                /*!< UDB_DSI LHO_CFG9: LHO77SEL (Bit 20)                       */
#define UDB_DSI_LHO_CFG9_LHO77SEL_Msk     (0xf00000UL)          /*!< UDB_DSI LHO_CFG9: LHO77SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG9_LHO78SEL_Pos     (24UL)                /*!< UDB_DSI LHO_CFG9: LHO78SEL (Bit 24)                       */
#define UDB_DSI_LHO_CFG9_LHO78SEL_Msk     (0xf000000UL)         /*!< UDB_DSI LHO_CFG9: LHO78SEL (Bitfield-Mask: 0x0f)          */
#define UDB_DSI_LHO_CFG9_LHO79SEL_Pos     (28UL)                /*!< UDB_DSI LHO_CFG9: LHO79SEL (Bit 28)                       */
#define UDB_DSI_LHO_CFG9_LHO79SEL_Msk     (0xf0000000UL)        /*!< UDB_DSI LHO_CFG9: LHO79SEL (Bitfield-Mask: 0x0f)          */
/* ===================================================  UDB_DSI.LHO_CFG10  =================================================== */
#define UDB_DSI_LHO_CFG10_LHO80SEL_Pos    (0UL)                 /*!< UDB_DSI LHO_CFG10: LHO80SEL (Bit 0)                       */
#define UDB_DSI_LHO_CFG10_LHO80SEL_Msk    (0xfUL)               /*!< UDB_DSI LHO_CFG10: LHO80SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG10_LHO81SEL_Pos    (4UL)                 /*!< UDB_DSI LHO_CFG10: LHO81SEL (Bit 4)                       */
#define UDB_DSI_LHO_CFG10_LHO81SEL_Msk    (0xf0UL)              /*!< UDB_DSI LHO_CFG10: LHO81SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG10_LHO82SEL_Pos    (8UL)                 /*!< UDB_DSI LHO_CFG10: LHO82SEL (Bit 8)                       */
#define UDB_DSI_LHO_CFG10_LHO82SEL_Msk    (0xf00UL)             /*!< UDB_DSI LHO_CFG10: LHO82SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG10_LHO83SEL_Pos    (12UL)                /*!< UDB_DSI LHO_CFG10: LHO83SEL (Bit 12)                      */
#define UDB_DSI_LHO_CFG10_LHO83SEL_Msk    (0xf000UL)            /*!< UDB_DSI LHO_CFG10: LHO83SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG10_LHO84SEL_Pos    (16UL)                /*!< UDB_DSI LHO_CFG10: LHO84SEL (Bit 16)                      */
#define UDB_DSI_LHO_CFG10_LHO84SEL_Msk    (0xf0000UL)           /*!< UDB_DSI LHO_CFG10: LHO84SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG10_LHO85SEL_Pos    (20UL)                /*!< UDB_DSI LHO_CFG10: LHO85SEL (Bit 20)                      */
#define UDB_DSI_LHO_CFG10_LHO85SEL_Msk    (0xf00000UL)          /*!< UDB_DSI LHO_CFG10: LHO85SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG10_LHO86SEL_Pos    (24UL)                /*!< UDB_DSI LHO_CFG10: LHO86SEL (Bit 24)                      */
#define UDB_DSI_LHO_CFG10_LHO86SEL_Msk    (0xf000000UL)         /*!< UDB_DSI LHO_CFG10: LHO86SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG10_LHO87SEL_Pos    (28UL)                /*!< UDB_DSI LHO_CFG10: LHO87SEL (Bit 28)                      */
#define UDB_DSI_LHO_CFG10_LHO87SEL_Msk    (0xf0000000UL)        /*!< UDB_DSI LHO_CFG10: LHO87SEL (Bitfield-Mask: 0x0f)         */
/* ===================================================  UDB_DSI.LHO_CFG11  =================================================== */
#define UDB_DSI_LHO_CFG11_LHO88SEL_Pos    (0UL)                 /*!< UDB_DSI LHO_CFG11: LHO88SEL (Bit 0)                       */
#define UDB_DSI_LHO_CFG11_LHO88SEL_Msk    (0xfUL)               /*!< UDB_DSI LHO_CFG11: LHO88SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG11_LHO89SEL_Pos    (4UL)                 /*!< UDB_DSI LHO_CFG11: LHO89SEL (Bit 4)                       */
#define UDB_DSI_LHO_CFG11_LHO89SEL_Msk    (0xf0UL)              /*!< UDB_DSI LHO_CFG11: LHO89SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG11_LHO90SEL_Pos    (8UL)                 /*!< UDB_DSI LHO_CFG11: LHO90SEL (Bit 8)                       */
#define UDB_DSI_LHO_CFG11_LHO90SEL_Msk    (0xf00UL)             /*!< UDB_DSI LHO_CFG11: LHO90SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG11_LHO91SEL_Pos    (12UL)                /*!< UDB_DSI LHO_CFG11: LHO91SEL (Bit 12)                      */
#define UDB_DSI_LHO_CFG11_LHO91SEL_Msk    (0xf000UL)            /*!< UDB_DSI LHO_CFG11: LHO91SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG11_LHO92SEL_Pos    (16UL)                /*!< UDB_DSI LHO_CFG11: LHO92SEL (Bit 16)                      */
#define UDB_DSI_LHO_CFG11_LHO92SEL_Msk    (0xf0000UL)           /*!< UDB_DSI LHO_CFG11: LHO92SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG11_LHO93SEL_Pos    (20UL)                /*!< UDB_DSI LHO_CFG11: LHO93SEL (Bit 20)                      */
#define UDB_DSI_LHO_CFG11_LHO93SEL_Msk    (0xf00000UL)          /*!< UDB_DSI LHO_CFG11: LHO93SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG11_LHO94SEL_Pos    (24UL)                /*!< UDB_DSI LHO_CFG11: LHO94SEL (Bit 24)                      */
#define UDB_DSI_LHO_CFG11_LHO94SEL_Msk    (0xf000000UL)         /*!< UDB_DSI LHO_CFG11: LHO94SEL (Bitfield-Mask: 0x0f)         */
#define UDB_DSI_LHO_CFG11_LHO95SEL_Pos    (28UL)                /*!< UDB_DSI LHO_CFG11: LHO95SEL (Bit 28)                      */
#define UDB_DSI_LHO_CFG11_LHO95SEL_Msk    (0xf0000000UL)        /*!< UDB_DSI LHO_CFG11: LHO95SEL (Bitfield-Mask: 0x0f)         */


/* ======================================================  UDB_PA.CFG0  ====================================================== */
#define UDB_PA_CFG0_CLKIN_EN_SEL_Pos      (0UL)                 /*!< UDB_PA CFG0: CLKIN_EN_SEL (Bit 0)                         */
#define UDB_PA_CFG0_CLKIN_EN_SEL_Msk      (0x3UL)               /*!< UDB_PA CFG0: CLKIN_EN_SEL (Bitfield-Mask: 0x03)           */
#define UDB_PA_CFG0_CLKIN_EN_MODE_Pos     (2UL)                 /*!< UDB_PA CFG0: CLKIN_EN_MODE (Bit 2)                        */
#define UDB_PA_CFG0_CLKIN_EN_MODE_Msk     (0xcUL)               /*!< UDB_PA CFG0: CLKIN_EN_MODE (Bitfield-Mask: 0x03)          */
#define UDB_PA_CFG0_CLKIN_EN_INV_Pos      (4UL)                 /*!< UDB_PA CFG0: CLKIN_EN_INV (Bit 4)                         */
#define UDB_PA_CFG0_CLKIN_EN_INV_Msk      (0x10UL)              /*!< UDB_PA CFG0: CLKIN_EN_INV (Bitfield-Mask: 0x01)           */
#define UDB_PA_CFG0_CLKIN_INV_Pos         (5UL)                 /*!< UDB_PA CFG0: CLKIN_INV (Bit 5)                            */
#define UDB_PA_CFG0_CLKIN_INV_Msk         (0x20UL)              /*!< UDB_PA CFG0: CLKIN_INV (Bitfield-Mask: 0x01)              */
/* ======================================================  UDB_PA.CFG1  ====================================================== */
#define UDB_PA_CFG1_CLKOUT_EN_SEL_Pos     (0UL)                 /*!< UDB_PA CFG1: CLKOUT_EN_SEL (Bit 0)                        */
#define UDB_PA_CFG1_CLKOUT_EN_SEL_Msk     (0x3UL)               /*!< UDB_PA CFG1: CLKOUT_EN_SEL (Bitfield-Mask: 0x03)          */
#define UDB_PA_CFG1_CLKOUT_EN_MODE_Pos    (2UL)                 /*!< UDB_PA CFG1: CLKOUT_EN_MODE (Bit 2)                       */
#define UDB_PA_CFG1_CLKOUT_EN_MODE_Msk    (0xcUL)               /*!< UDB_PA CFG1: CLKOUT_EN_MODE (Bitfield-Mask: 0x03)         */
#define UDB_PA_CFG1_CLKOUT_EN_INV_Pos     (4UL)                 /*!< UDB_PA CFG1: CLKOUT_EN_INV (Bit 4)                        */
#define UDB_PA_CFG1_CLKOUT_EN_INV_Msk     (0x10UL)              /*!< UDB_PA CFG1: CLKOUT_EN_INV (Bitfield-Mask: 0x01)          */
#define UDB_PA_CFG1_CLKOUT_INV_Pos        (5UL)                 /*!< UDB_PA CFG1: CLKOUT_INV (Bit 5)                           */
#define UDB_PA_CFG1_CLKOUT_INV_Msk        (0x20UL)              /*!< UDB_PA CFG1: CLKOUT_INV (Bitfield-Mask: 0x01)             */
/* ======================================================  UDB_PA.CFG2  ====================================================== */
#define UDB_PA_CFG2_CLKIN_SEL_Pos         (0UL)                 /*!< UDB_PA CFG2: CLKIN_SEL (Bit 0)                            */
#define UDB_PA_CFG2_CLKIN_SEL_Msk         (0xfUL)               /*!< UDB_PA CFG2: CLKIN_SEL (Bitfield-Mask: 0x0f)              */
#define UDB_PA_CFG2_CLKOUT_SEL_Pos        (4UL)                 /*!< UDB_PA CFG2: CLKOUT_SEL (Bit 4)                           */
#define UDB_PA_CFG2_CLKOUT_SEL_Msk        (0xf0UL)              /*!< UDB_PA CFG2: CLKOUT_SEL (Bitfield-Mask: 0x0f)             */
/* ======================================================  UDB_PA.CFG3  ====================================================== */
#define UDB_PA_CFG3_RES_IN_SEL_Pos        (0UL)                 /*!< UDB_PA CFG3: RES_IN_SEL (Bit 0)                           */
#define UDB_PA_CFG3_RES_IN_SEL_Msk        (0x3UL)               /*!< UDB_PA CFG3: RES_IN_SEL (Bitfield-Mask: 0x03)             */
#define UDB_PA_CFG3_RES_IN_INV_Pos        (2UL)                 /*!< UDB_PA CFG3: RES_IN_INV (Bit 2)                           */
#define UDB_PA_CFG3_RES_IN_INV_Msk        (0x4UL)               /*!< UDB_PA CFG3: RES_IN_INV (Bitfield-Mask: 0x01)             */
#define UDB_PA_CFG3_RES_OUT_SEL_Pos       (4UL)                 /*!< UDB_PA CFG3: RES_OUT_SEL (Bit 4)                          */
#define UDB_PA_CFG3_RES_OUT_SEL_Msk       (0x30UL)              /*!< UDB_PA CFG3: RES_OUT_SEL (Bitfield-Mask: 0x03)            */
#define UDB_PA_CFG3_RES_OUT_INV_Pos       (6UL)                 /*!< UDB_PA CFG3: RES_OUT_INV (Bit 6)                          */
#define UDB_PA_CFG3_RES_OUT_INV_Msk       (0x40UL)              /*!< UDB_PA CFG3: RES_OUT_INV (Bitfield-Mask: 0x01)            */
/* ======================================================  UDB_PA.CFG4  ====================================================== */
#define UDB_PA_CFG4_RES_IN_EN_Pos         (0UL)                 /*!< UDB_PA CFG4: RES_IN_EN (Bit 0)                            */
#define UDB_PA_CFG4_RES_IN_EN_Msk         (0x1UL)               /*!< UDB_PA CFG4: RES_IN_EN (Bitfield-Mask: 0x01)              */
#define UDB_PA_CFG4_RES_OUT_EN_Pos        (1UL)                 /*!< UDB_PA CFG4: RES_OUT_EN (Bit 1)                           */
#define UDB_PA_CFG4_RES_OUT_EN_Msk        (0x2UL)               /*!< UDB_PA CFG4: RES_OUT_EN (Bitfield-Mask: 0x01)             */
#define UDB_PA_CFG4_RES_OE_EN_Pos         (2UL)                 /*!< UDB_PA CFG4: RES_OE_EN (Bit 2)                            */
#define UDB_PA_CFG4_RES_OE_EN_Msk         (0x4UL)               /*!< UDB_PA CFG4: RES_OE_EN (Bitfield-Mask: 0x01)              */
/* ======================================================  UDB_PA.CFG5  ====================================================== */
#define UDB_PA_CFG5_PIN_SEL_Pos           (0UL)                 /*!< UDB_PA CFG5: PIN_SEL (Bit 0)                              */
#define UDB_PA_CFG5_PIN_SEL_Msk           (0x7UL)               /*!< UDB_PA CFG5: PIN_SEL (Bitfield-Mask: 0x07)                */
/* ======================================================  UDB_PA.CFG6  ====================================================== */
#define UDB_PA_CFG6_IN_SYNC0_Pos          (0UL)                 /*!< UDB_PA CFG6: IN_SYNC0 (Bit 0)                             */
#define UDB_PA_CFG6_IN_SYNC0_Msk          (0x3UL)               /*!< UDB_PA CFG6: IN_SYNC0 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG6_IN_SYNC1_Pos          (2UL)                 /*!< UDB_PA CFG6: IN_SYNC1 (Bit 2)                             */
#define UDB_PA_CFG6_IN_SYNC1_Msk          (0xcUL)               /*!< UDB_PA CFG6: IN_SYNC1 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG6_IN_SYNC2_Pos          (4UL)                 /*!< UDB_PA CFG6: IN_SYNC2 (Bit 4)                             */
#define UDB_PA_CFG6_IN_SYNC2_Msk          (0x30UL)              /*!< UDB_PA CFG6: IN_SYNC2 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG6_IN_SYNC3_Pos          (6UL)                 /*!< UDB_PA CFG6: IN_SYNC3 (Bit 6)                             */
#define UDB_PA_CFG6_IN_SYNC3_Msk          (0xc0UL)              /*!< UDB_PA CFG6: IN_SYNC3 (Bitfield-Mask: 0x03)               */
/* ======================================================  UDB_PA.CFG7  ====================================================== */
#define UDB_PA_CFG7_IN_SYNC4_Pos          (0UL)                 /*!< UDB_PA CFG7: IN_SYNC4 (Bit 0)                             */
#define UDB_PA_CFG7_IN_SYNC4_Msk          (0x3UL)               /*!< UDB_PA CFG7: IN_SYNC4 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG7_IN_SYNC5_Pos          (2UL)                 /*!< UDB_PA CFG7: IN_SYNC5 (Bit 2)                             */
#define UDB_PA_CFG7_IN_SYNC5_Msk          (0xcUL)               /*!< UDB_PA CFG7: IN_SYNC5 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG7_IN_SYNC6_Pos          (4UL)                 /*!< UDB_PA CFG7: IN_SYNC6 (Bit 4)                             */
#define UDB_PA_CFG7_IN_SYNC6_Msk          (0x30UL)              /*!< UDB_PA CFG7: IN_SYNC6 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG7_IN_SYNC7_Pos          (6UL)                 /*!< UDB_PA CFG7: IN_SYNC7 (Bit 6)                             */
#define UDB_PA_CFG7_IN_SYNC7_Msk          (0xc0UL)              /*!< UDB_PA CFG7: IN_SYNC7 (Bitfield-Mask: 0x03)               */
/* ======================================================  UDB_PA.CFG8  ====================================================== */
#define UDB_PA_CFG8_OUT_SYNC0_Pos         (0UL)                 /*!< UDB_PA CFG8: OUT_SYNC0 (Bit 0)                            */
#define UDB_PA_CFG8_OUT_SYNC0_Msk         (0x3UL)               /*!< UDB_PA CFG8: OUT_SYNC0 (Bitfield-Mask: 0x03)              */
#define UDB_PA_CFG8_OUT_SYNC1_Pos         (2UL)                 /*!< UDB_PA CFG8: OUT_SYNC1 (Bit 2)                            */
#define UDB_PA_CFG8_OUT_SYNC1_Msk         (0xcUL)               /*!< UDB_PA CFG8: OUT_SYNC1 (Bitfield-Mask: 0x03)              */
#define UDB_PA_CFG8_OUT_SYNC2_Pos         (4UL)                 /*!< UDB_PA CFG8: OUT_SYNC2 (Bit 4)                            */
#define UDB_PA_CFG8_OUT_SYNC2_Msk         (0x30UL)              /*!< UDB_PA CFG8: OUT_SYNC2 (Bitfield-Mask: 0x03)              */
#define UDB_PA_CFG8_OUT_SYNC3_Pos         (6UL)                 /*!< UDB_PA CFG8: OUT_SYNC3 (Bit 6)                            */
#define UDB_PA_CFG8_OUT_SYNC3_Msk         (0xc0UL)              /*!< UDB_PA CFG8: OUT_SYNC3 (Bitfield-Mask: 0x03)              */
/* ======================================================  UDB_PA.CFG9  ====================================================== */
#define UDB_PA_CFG9_OUT_SYNC4_Pos         (0UL)                 /*!< UDB_PA CFG9: OUT_SYNC4 (Bit 0)                            */
#define UDB_PA_CFG9_OUT_SYNC4_Msk         (0x3UL)               /*!< UDB_PA CFG9: OUT_SYNC4 (Bitfield-Mask: 0x03)              */
#define UDB_PA_CFG9_OUT_SYNC5_Pos         (2UL)                 /*!< UDB_PA CFG9: OUT_SYNC5 (Bit 2)                            */
#define UDB_PA_CFG9_OUT_SYNC5_Msk         (0xcUL)               /*!< UDB_PA CFG9: OUT_SYNC5 (Bitfield-Mask: 0x03)              */
#define UDB_PA_CFG9_OUT_SYNC6_Pos         (4UL)                 /*!< UDB_PA CFG9: OUT_SYNC6 (Bit 4)                            */
#define UDB_PA_CFG9_OUT_SYNC6_Msk         (0x30UL)              /*!< UDB_PA CFG9: OUT_SYNC6 (Bitfield-Mask: 0x03)              */
#define UDB_PA_CFG9_OUT_SYNC7_Pos         (6UL)                 /*!< UDB_PA CFG9: OUT_SYNC7 (Bit 6)                            */
#define UDB_PA_CFG9_OUT_SYNC7_Msk         (0xc0UL)              /*!< UDB_PA CFG9: OUT_SYNC7 (Bitfield-Mask: 0x03)              */
/* =====================================================  UDB_PA.CFG10  ====================================================== */
#define UDB_PA_CFG10_DATA_SEL0_Pos        (0UL)                 /*!< UDB_PA CFG10: DATA_SEL0 (Bit 0)                           */
#define UDB_PA_CFG10_DATA_SEL0_Msk        (0x3UL)               /*!< UDB_PA CFG10: DATA_SEL0 (Bitfield-Mask: 0x03)             */
#define UDB_PA_CFG10_DATA_SEL1_Pos        (2UL)                 /*!< UDB_PA CFG10: DATA_SEL1 (Bit 2)                           */
#define UDB_PA_CFG10_DATA_SEL1_Msk        (0xcUL)               /*!< UDB_PA CFG10: DATA_SEL1 (Bitfield-Mask: 0x03)             */
#define UDB_PA_CFG10_DATA_SEL2_Pos        (4UL)                 /*!< UDB_PA CFG10: DATA_SEL2 (Bit 4)                           */
#define UDB_PA_CFG10_DATA_SEL2_Msk        (0x30UL)              /*!< UDB_PA CFG10: DATA_SEL2 (Bitfield-Mask: 0x03)             */
#define UDB_PA_CFG10_DATA_SEL3_Pos        (6UL)                 /*!< UDB_PA CFG10: DATA_SEL3 (Bit 6)                           */
#define UDB_PA_CFG10_DATA_SEL3_Msk        (0xc0UL)              /*!< UDB_PA CFG10: DATA_SEL3 (Bitfield-Mask: 0x03)             */
/* =====================================================  UDB_PA.CFG11  ====================================================== */
#define UDB_PA_CFG11_DATA_SEL4_Pos        (0UL)                 /*!< UDB_PA CFG11: DATA_SEL4 (Bit 0)                           */
#define UDB_PA_CFG11_DATA_SEL4_Msk        (0x3UL)               /*!< UDB_PA CFG11: DATA_SEL4 (Bitfield-Mask: 0x03)             */
#define UDB_PA_CFG11_DATA_SEL5_Pos        (2UL)                 /*!< UDB_PA CFG11: DATA_SEL5 (Bit 2)                           */
#define UDB_PA_CFG11_DATA_SEL5_Msk        (0xcUL)               /*!< UDB_PA CFG11: DATA_SEL5 (Bitfield-Mask: 0x03)             */
#define UDB_PA_CFG11_DATA_SEL6_Pos        (4UL)                 /*!< UDB_PA CFG11: DATA_SEL6 (Bit 4)                           */
#define UDB_PA_CFG11_DATA_SEL6_Msk        (0x30UL)              /*!< UDB_PA CFG11: DATA_SEL6 (Bitfield-Mask: 0x03)             */
#define UDB_PA_CFG11_DATA_SEL7_Pos        (6UL)                 /*!< UDB_PA CFG11: DATA_SEL7 (Bit 6)                           */
#define UDB_PA_CFG11_DATA_SEL7_Msk        (0xc0UL)              /*!< UDB_PA CFG11: DATA_SEL7 (Bitfield-Mask: 0x03)             */
/* =====================================================  UDB_PA.CFG12  ====================================================== */
#define UDB_PA_CFG12_OE_SEL0_Pos          (0UL)                 /*!< UDB_PA CFG12: OE_SEL0 (Bit 0)                             */
#define UDB_PA_CFG12_OE_SEL0_Msk          (0x3UL)               /*!< UDB_PA CFG12: OE_SEL0 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG12_OE_SEL1_Pos          (2UL)                 /*!< UDB_PA CFG12: OE_SEL1 (Bit 2)                             */
#define UDB_PA_CFG12_OE_SEL1_Msk          (0xcUL)               /*!< UDB_PA CFG12: OE_SEL1 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG12_OE_SEL2_Pos          (4UL)                 /*!< UDB_PA CFG12: OE_SEL2 (Bit 4)                             */
#define UDB_PA_CFG12_OE_SEL2_Msk          (0x30UL)              /*!< UDB_PA CFG12: OE_SEL2 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG12_OE_SEL3_Pos          (6UL)                 /*!< UDB_PA CFG12: OE_SEL3 (Bit 6)                             */
#define UDB_PA_CFG12_OE_SEL3_Msk          (0xc0UL)              /*!< UDB_PA CFG12: OE_SEL3 (Bitfield-Mask: 0x03)               */
/* =====================================================  UDB_PA.CFG13  ====================================================== */
#define UDB_PA_CFG13_OE_SEL4_Pos          (0UL)                 /*!< UDB_PA CFG13: OE_SEL4 (Bit 0)                             */
#define UDB_PA_CFG13_OE_SEL4_Msk          (0x3UL)               /*!< UDB_PA CFG13: OE_SEL4 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG13_OE_SEL5_Pos          (2UL)                 /*!< UDB_PA CFG13: OE_SEL5 (Bit 2)                             */
#define UDB_PA_CFG13_OE_SEL5_Msk          (0xcUL)               /*!< UDB_PA CFG13: OE_SEL5 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG13_OE_SEL6_Pos          (4UL)                 /*!< UDB_PA CFG13: OE_SEL6 (Bit 4)                             */
#define UDB_PA_CFG13_OE_SEL6_Msk          (0x30UL)              /*!< UDB_PA CFG13: OE_SEL6 (Bitfield-Mask: 0x03)               */
#define UDB_PA_CFG13_OE_SEL7_Pos          (6UL)                 /*!< UDB_PA CFG13: OE_SEL7 (Bit 6)                             */
#define UDB_PA_CFG13_OE_SEL7_Msk          (0xc0UL)              /*!< UDB_PA CFG13: OE_SEL7 (Bitfield-Mask: 0x03)               */
/* =====================================================  UDB_PA.CFG14  ====================================================== */
#define UDB_PA_CFG14_OE_SYNC0_Pos         (0UL)                 /*!< UDB_PA CFG14: OE_SYNC0 (Bit 0)                            */
#define UDB_PA_CFG14_OE_SYNC0_Msk         (0x3UL)               /*!< UDB_PA CFG14: OE_SYNC0 (Bitfield-Mask: 0x03)              */
#define UDB_PA_CFG14_OE_SYNC1_Pos         (2UL)                 /*!< UDB_PA CFG14: OE_SYNC1 (Bit 2)                            */
#define UDB_PA_CFG14_OE_SYNC1_Msk         (0xcUL)               /*!< UDB_PA CFG14: OE_SYNC1 (Bitfield-Mask: 0x03)              */
#define UDB_PA_CFG14_OE_SYNC2_Pos         (4UL)                 /*!< UDB_PA CFG14: OE_SYNC2 (Bit 4)                            */
#define UDB_PA_CFG14_OE_SYNC2_Msk         (0x30UL)              /*!< UDB_PA CFG14: OE_SYNC2 (Bitfield-Mask: 0x03)              */
#define UDB_PA_CFG14_OE_SYNC3_Pos         (6UL)                 /*!< UDB_PA CFG14: OE_SYNC3 (Bit 6)                            */
#define UDB_PA_CFG14_OE_SYNC3_Msk         (0xc0UL)              /*!< UDB_PA CFG14: OE_SYNC3 (Bitfield-Mask: 0x03)              */


/* ===================================================  UDB_BCTL.MDCLK_EN  =================================================== */
#define UDB_BCTL_MDCLK_EN_DCEN_Pos        (0UL)                 /*!< UDB_BCTL MDCLK_EN: DCEN (Bit 0)                           */
#define UDB_BCTL_MDCLK_EN_DCEN_Msk        (0xffUL)              /*!< UDB_BCTL MDCLK_EN: DCEN (Bitfield-Mask: 0xff)             */
/* ===================================================  UDB_BCTL.MBCLK_EN  =================================================== */
#define UDB_BCTL_MBCLK_EN_BCEN_Pos        (0UL)                 /*!< UDB_BCTL MBCLK_EN: BCEN (Bit 0)                           */
#define UDB_BCTL_MBCLK_EN_BCEN_Msk        (0x1UL)               /*!< UDB_BCTL MBCLK_EN: BCEN (Bitfield-Mask: 0x01)             */
/* ===================================================  UDB_BCTL.BOTSEL_L  =================================================== */
#define UDB_BCTL_BOTSEL_L_CLK_SEL0_Pos    (0UL)                 /*!< UDB_BCTL BOTSEL_L: CLK_SEL0 (Bit 0)                       */
#define UDB_BCTL_BOTSEL_L_CLK_SEL0_Msk    (0x3UL)               /*!< UDB_BCTL BOTSEL_L: CLK_SEL0 (Bitfield-Mask: 0x03)         */
#define UDB_BCTL_BOTSEL_L_CLK_SEL1_Pos    (2UL)                 /*!< UDB_BCTL BOTSEL_L: CLK_SEL1 (Bit 2)                       */
#define UDB_BCTL_BOTSEL_L_CLK_SEL1_Msk    (0xcUL)               /*!< UDB_BCTL BOTSEL_L: CLK_SEL1 (Bitfield-Mask: 0x03)         */
#define UDB_BCTL_BOTSEL_L_CLK_SEL2_Pos    (4UL)                 /*!< UDB_BCTL BOTSEL_L: CLK_SEL2 (Bit 4)                       */
#define UDB_BCTL_BOTSEL_L_CLK_SEL2_Msk    (0x30UL)              /*!< UDB_BCTL BOTSEL_L: CLK_SEL2 (Bitfield-Mask: 0x03)         */
#define UDB_BCTL_BOTSEL_L_CLK_SEL3_Pos    (6UL)                 /*!< UDB_BCTL BOTSEL_L: CLK_SEL3 (Bit 6)                       */
#define UDB_BCTL_BOTSEL_L_CLK_SEL3_Msk    (0xc0UL)              /*!< UDB_BCTL BOTSEL_L: CLK_SEL3 (Bitfield-Mask: 0x03)         */
/* ===================================================  UDB_BCTL.BOTSEL_U  =================================================== */
#define UDB_BCTL_BOTSEL_U_CLK_SEL4_Pos    (0UL)                 /*!< UDB_BCTL BOTSEL_U: CLK_SEL4 (Bit 0)                       */
#define UDB_BCTL_BOTSEL_U_CLK_SEL4_Msk    (0x3UL)               /*!< UDB_BCTL BOTSEL_U: CLK_SEL4 (Bitfield-Mask: 0x03)         */
#define UDB_BCTL_BOTSEL_U_CLK_SEL5_Pos    (2UL)                 /*!< UDB_BCTL BOTSEL_U: CLK_SEL5 (Bit 2)                       */
#define UDB_BCTL_BOTSEL_U_CLK_SEL5_Msk    (0xcUL)               /*!< UDB_BCTL BOTSEL_U: CLK_SEL5 (Bitfield-Mask: 0x03)         */
#define UDB_BCTL_BOTSEL_U_CLK_SEL6_Pos    (4UL)                 /*!< UDB_BCTL BOTSEL_U: CLK_SEL6 (Bit 4)                       */
#define UDB_BCTL_BOTSEL_U_CLK_SEL6_Msk    (0x30UL)              /*!< UDB_BCTL BOTSEL_U: CLK_SEL6 (Bitfield-Mask: 0x03)         */
#define UDB_BCTL_BOTSEL_U_CLK_SEL7_Pos    (6UL)                 /*!< UDB_BCTL BOTSEL_U: CLK_SEL7 (Bit 6)                       */
#define UDB_BCTL_BOTSEL_U_CLK_SEL7_Msk    (0xc0UL)              /*!< UDB_BCTL BOTSEL_U: CLK_SEL7 (Bitfield-Mask: 0x03)         */
/* ===================================================  UDB_BCTL.QCLK_EN  ==================================================== */
#define UDB_BCTL_QCLK_EN_DCEN_Q_Pos       (0UL)                 /*!< UDB_BCTL QCLK_EN: DCEN_Q (Bit 0)                          */
#define UDB_BCTL_QCLK_EN_DCEN_Q_Msk       (0xffUL)              /*!< UDB_BCTL QCLK_EN: DCEN_Q (Bitfield-Mask: 0xff)            */
#define UDB_BCTL_QCLK_EN_BCEN_Q_Pos       (8UL)                 /*!< UDB_BCTL QCLK_EN: BCEN_Q (Bit 8)                          */
#define UDB_BCTL_QCLK_EN_BCEN_Q_Msk       (0x100UL)             /*!< UDB_BCTL QCLK_EN: BCEN_Q (Bitfield-Mask: 0x01)            */
#define UDB_BCTL_QCLK_EN_DISABLE_ROUTE_Pos (11UL)               /*!< UDB_BCTL QCLK_EN: DISABLE_ROUTE (Bit 11)                  */
#define UDB_BCTL_QCLK_EN_DISABLE_ROUTE_Msk (0x800UL)            /*!< UDB_BCTL QCLK_EN: DISABLE_ROUTE (Bitfield-Mask: 0x01)     */


/* ==================================================  UDB_UDBIF.BANK_CTL  =================================================== */
#define UDB_UDBIF_BANK_CTL_DIS_COR_Pos    (0UL)                 /*!< UDB_UDBIF BANK_CTL: DIS_COR (Bit 0)                       */
#define UDB_UDBIF_BANK_CTL_DIS_COR_Msk    (0x1UL)               /*!< UDB_UDBIF BANK_CTL: DIS_COR (Bitfield-Mask: 0x01)         */
#define UDB_UDBIF_BANK_CTL_ROUTE_EN_Pos   (1UL)                 /*!< UDB_UDBIF BANK_CTL: ROUTE_EN (Bit 1)                      */
#define UDB_UDBIF_BANK_CTL_ROUTE_EN_Msk   (0x2UL)               /*!< UDB_UDBIF BANK_CTL: ROUTE_EN (Bitfield-Mask: 0x01)        */
#define UDB_UDBIF_BANK_CTL_BANK_EN_Pos    (2UL)                 /*!< UDB_UDBIF BANK_CTL: BANK_EN (Bit 2)                       */
#define UDB_UDBIF_BANK_CTL_BANK_EN_Msk    (0x4UL)               /*!< UDB_UDBIF BANK_CTL: BANK_EN (Bitfield-Mask: 0x01)         */
#define UDB_UDBIF_BANK_CTL_READ_WAIT_Pos  (8UL)                 /*!< UDB_UDBIF BANK_CTL: READ_WAIT (Bit 8)                     */
#define UDB_UDBIF_BANK_CTL_READ_WAIT_Msk  (0x100UL)             /*!< UDB_UDBIF BANK_CTL: READ_WAIT (Bitfield-Mask: 0x01)       */
/* =================================================  UDB_UDBIF.INT_CLK_CTL  ================================================= */
#define UDB_UDBIF_INT_CLK_CTL_INT_CLK_ENABLE_Pos (0UL)          /*!< UDB_UDBIF INT_CLK_CTL: INT_CLK_ENABLE (Bit 0)             */
#define UDB_UDBIF_INT_CLK_CTL_INT_CLK_ENABLE_Msk (0x1UL)        /*!< UDB_UDBIF INT_CLK_CTL: INT_CLK_ENABLE (Bitfield-Mask: 0x01)*/
/* ===================================================  UDB_UDBIF.INT_CFG  =================================================== */
#define UDB_UDBIF_INT_CFG_INT_MODE_CFG_Pos (0UL)                /*!< UDB_UDBIF INT_CFG: INT_MODE_CFG (Bit 0)                   */
#define UDB_UDBIF_INT_CFG_INT_MODE_CFG_Msk (0xffffffffUL)       /*!< UDB_UDBIF INT_CFG: INT_MODE_CFG (Bitfield-Mask: 0xffffffff)*/
/* =================================================  UDB_UDBIF.TR_CLK_CTL  ================================================== */
#define UDB_UDBIF_TR_CLK_CTL_TR_CLOCK_ENABLE_Pos (0UL)          /*!< UDB_UDBIF TR_CLK_CTL: TR_CLOCK_ENABLE (Bit 0)             */
#define UDB_UDBIF_TR_CLK_CTL_TR_CLOCK_ENABLE_Msk (0x1UL)        /*!< UDB_UDBIF TR_CLK_CTL: TR_CLOCK_ENABLE (Bitfield-Mask: 0x01)*/
/* ===================================================  UDB_UDBIF.TR_CFG  ==================================================== */
#define UDB_UDBIF_TR_CFG_TR_MODE_CFG_Pos  (0UL)                 /*!< UDB_UDBIF TR_CFG: TR_MODE_CFG (Bit 0)                     */
#define UDB_UDBIF_TR_CFG_TR_MODE_CFG_Msk  (0xffffffffUL)        /*!< UDB_UDBIF TR_CFG: TR_MODE_CFG (Bitfield-Mask: 0xffffffff) */
/* ===================================================  UDB_UDBIF.PRIVATE  =================================================== */
#define UDB_UDBIF_PRIVATE_PIPELINE_MD_Pos (0UL)                 /*!< UDB_UDBIF PRIVATE: PIPELINE_MD (Bit 0)                    */
#define UDB_UDBIF_PRIVATE_PIPELINE_MD_Msk (0x1UL)               /*!< UDB_UDBIF PRIVATE: PIPELINE_MD (Bitfield-Mask: 0x01)      */


#endif /* _CYIP_UDB_H_ */


/* [] END OF FILE */
