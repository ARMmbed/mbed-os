; 
; Copyright (C) u-blox
; All rights reserved.
; This source file is the sole property of u-blox. Reproduction or utilization
; of this source in whole or part is forbidden without the written consent of
; u-blox.
; 
; FILE: outputs/asm/hal_debug.hs

BB_MODEM_SS_DBG_VSP0_DE0_DBG_AR1DE_DBG_AR1DE_NR_BP EQU 8
BB_MODEM_SS_DBG_VSP0_DE0_MWU_AR1DE_MWU_AR1DE_MWU_NR_REGIONS EQU 4
BB_MODEM_SS_DBG_VSP0_PMU_PMU_NR_PMUEVCNT EQU 4
STM_STIM_STM_STIM_STM_STIM_NR_PORTS EQU 65536
STM_STIM_STM_STIM_STM_STIM_WIDTH EQU 256
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGIN_DBGHALTED EQU 0
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGIN_DWT_TRIGGER_0 EQU 4
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGIN_DWT_TRIGGER_1 EQU 5
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGIN_DWT_TRIGGER_2 EQU 6
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGIN_ETM_TRIGOUT EQU 7
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGOUT_DBGREQ EQU 0
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGOUT_INTERRUPT_REQUEST_0 EQU 2
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGOUT_INTERRUPT_REQUEST_1 EQU 3
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGOUT_EXITIN_4 EQU 4
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGOUT_EXITIN_5 EQU 5
CTI_CFG_CORTEX_M3_CORTEX_M3_CTI_TRIGOUT_DBGRESTART EQU 7
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGIN_DBGHALTED EQU 0
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGIN_DWT_CMP_0 EQU 1
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGIN_DWT_CMP_1 EQU 2
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGIN_DWT_CMP_2 EQU 3
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGIN_ETM_EVENT_OUT_0 EQU 4
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGIN_ETM_EVENT_OUT_1 EQU 5
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGIN_ETM_EVENT_OUT_2 EQU 6
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGIN_ETM_EVENT_OUT_3 EQU 7
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGOUT_DBGREQ EQU 0
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGOUT_INTERRUPT_REQUEST_0 EQU 1
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGOUT_INTERRUPT_REQUEST_1 EQU 2
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGOUT_ETM_EVENT_INPUT_0 EQU 3
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGOUT_ETM_EVENT_INPUT_1 EQU 4
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGOUT_ETM_EVENT_INPUT_2 EQU 5
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGOUT_ETM_EVENT_INPUT_3 EQU 6
CTI_CFG_CORTEX_M7_CORTEX_M7_CTI_TRIGOUT_DBGRESTART EQU 7
DAP_CFG_APPCPU_AP EQU 0
DAP_CFG_RFCPU_AP EQU 1
DAP_CFG_MDMCPU_AP EQU 2
DAP_CFG_PHYCPU_AP EQU 3
DAP_CFG_CS_AP EQU 4
DAP_CFG_SYS_AP EQU 5
DAP_CFG_SEC_AP EQU 6
FUNNEL_CFG_STM EQU 0
FUNNEL_CFG_MDMCPU_INSTR EQU 1
FUNNEL_CFG_MDMCPU_DATA EQU 2
FUNNEL_CFG_RFCPU_ETM EQU 3
FUNNEL_CFG_RFCPU_ITM EQU 4
FUNNEL_CFG_PHYCPU_ETM EQU 5
FUNNEL_CFG_APPCPU_INSTR EQU 6
FUNNEL_CFG_APPCPU_DATA EQU 7
CS_TRACE_SS_DBG_CTITRACE_TRIGIN_TMC_FULL EQU 0
CS_TRACE_SS_DBG_CTITRACE_TRIGIN_TMC_ACQCOMP EQU 1
CS_TRACE_SS_DBG_CTITRACE_TRIGIN_ETR_FULL EQU 2
CS_TRACE_SS_DBG_CTITRACE_TRIGIN_ETR_ACQCOMP EQU 3
CS_TRACE_SS_DBG_CTITRACE_TRIGIN_STM_TRIGOUTSPTE EQU 4
CS_TRACE_SS_DBG_CTITRACE_TRIGIN_STM_TRIGOUTSW EQU 5
CS_TRACE_SS_DBG_CTITRACE_TRIGIN_STM_TRIGOUTHETE EQU 6
CS_TRACE_SS_DBG_CTITRACE_TRIGIN_STM_ASYNCOUT EQU 7
CS_TRACE_SS_DBG_CTITRACE_TRIGOUT_TPIU_TRIGIN EQU 0
CS_TRACE_SS_DBG_CTITRACE_TRIGOUT_TPIU_FLUSHIN EQU 1
CS_TRACE_SS_DBG_CTITRACE_TRIGOUT_TMC_TRIGIN EQU 2
CS_TRACE_SS_DBG_CTITRACE_TRIGOUT_TMC_FLUSHIN EQU 3
CS_TRACE_SS_DBG_CTITRACE_TRIGOUT_ETR_TRIGIN EQU 4
CS_TRACE_SS_DBG_CTITRACE_TRIGOUT_ETR_FLUSHIN EQU 5
CS_TRACE_SS_DBG_CTITRACE_TRIGOUT_WDGTIM_DBGREQ EQU 6
CS_TRACE_SS_DBG_CTITRACE_TRIGOUT_WDGTIM_DBGRESTART EQU 7
BB_MODEM_SS_DBG_CTIMODEM_TRIGIN_HWSEQ_DBGTRIGGER EQU 0
BB_MODEM_SS_DBG_CTIMODEM_TRIGOUT_TCU0_DBGREQ EQU 2
BB_MODEM_SS_DBG_CTIMODEM_TRIGOUT_TCU0_DBGRESTART EQU 3
BB_MODEM_SS_DBG_CTIMODEM_TRIGOUT_TCU1_DBGREQ EQU 4
BB_MODEM_SS_DBG_CTIMODEM_TRIGOUT_TCU1_DBGRESTART EQU 5
BB_MODEM_SS_DBG_CTIMODEM_TRIGOUT_HWSEQ_DBGREQ EQU 6
BB_MODEM_SS_DBG_CTIMODEM_TRIGOUT_HWSEQ_DBGRESTART EQU 7
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_SWI EQU 0
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_QUEUE0_VDMA_CHANNEL_START EQU 1
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_QUEUE0_VDMA_CHANNEL_LAST_READ EQU 2
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_QUEUE0_VDMA_CHANNEL_END EQU 3
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_QUEUE1_VDMA_CHANNEL_START EQU 4
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_QUEUE1_VDMA_CHANNEL_LAST_READ EQU 5
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_QUEUE1_VDMA_CHANNEL_END EQU 6
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_MWU_REGION0_HIT EQU 7
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_MWU_REGION1_HIT EQU 8
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_MWU_REGION2_HIT EQU 9
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_MWU_REGION3_HIT EQU 10
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_DPU_ITCM_READ_TRANS EQU 11
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_DPU_DTCM_READ_TRANS EQU 12
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_DPU_DTCM_WRITE_TRANS EQU 13
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_DPU_DMEM_READ_TRANS EQU 14
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_DPU_DMEM_WRITE_TRANS EQU 15
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_DPU_STALL EQU 16
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_DPU_DMEM_ACCESS_STALL EQU 17
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_DPU_PPC_STALL EQU 18
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_DPU_AXI_STALL EQU 19
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_SLV_READ_TRANS EQU 20
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_SLV_WRITE_TRANS EQU 21
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_START EQU 22
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_DE0_END EQU 23
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_BANK0_CONFLICT EQU 24
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_BANK1_CONFLICT EQU 25
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_BANK2_CONFLICT EQU 26
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_BANK3_CONFLICT EQU 27
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_OVF EQU 254
BB_MODEM_SS_DBG_VSP0_VSPPMU_CFG_ALWAYS EQU 255
BB_MODEM_SS_DBG_VSP0_VSPSS_CTI_TRIGIN_VDE_DBGTRIGGER EQU 0
BB_MODEM_SS_DBG_VSP0_VSPSS_CTI_TRIGOUT_VDE_DBGREQ EQU 0
BB_MODEM_SS_DBG_VSP0_VSPSS_CTI_TRIGOUT_VDMA_DBGREQ EQU 2
BB_MODEM_SS_DBG_VSP0_VSPSS_CTI_TRIGOUT_VDE_DBGRESTART EQU 4
BB_MODEM_SS_DBG_VSP0_VSPSS_CTI_TRIGOUT_VDMA_DBGRESTART EQU 6
CS_TRACE_SS_DBG_CTITRACE EQU (0x49810000)
CS_TRACE_SS_DBG_TSGEN EQU (0x49811000)
CS_TRACE_SS_DBG_STM EQU (0x49812000)
CS_TRACE_SS_DBG_FUNNEL EQU (0x49813000)
CS_TRACE_SS_DBG_TMC EQU (0x49814000)
CS_TRACE_SS_DBG_REPLICATOR EQU (0x49815000)
CS_TRACE_SS_DBG_TPIU EQU (0x49816000)
CS_TRACE_SS_DBG_ETR EQU (0x49817000)
CS_TRACE_SS_DBG_ETR_LIMITER EQU (0x49818000)
CS_TRACE_SS_DBG_STM_FILTER EQU (0x49819000)
BB_MODEM_SS_DBG_VSP0_DE0_DBG EQU (0x49820000)
BB_MODEM_SS_DBG_VSP0_DE0_MWU EQU (0x49821000)
BB_MODEM_SS_DBG_VSP0_CTI EQU (0x49824000)
BB_MODEM_SS_DBG_VSP0_PMU EQU (0x49825000)
BB_MODEM_SS_DBG_HWSEQ EQU (0x49828000)
BB_MODEM_SS_DBG_CTIMODEM EQU (0x49829000)
STM_STIM EQU (0x50000000)
; EOF: outputs/asm/hal_debug.hs
   END
