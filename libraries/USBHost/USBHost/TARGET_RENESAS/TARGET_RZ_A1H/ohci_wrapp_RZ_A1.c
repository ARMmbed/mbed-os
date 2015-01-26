/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <string.h>
#include "cmsis.h"
#include "cmsis_os.h"
#include "ohci_wrapp_RZ_A1.h"
#include "ohci_wrapp_RZ_A1_local.h"

#include "rza_io_regrw.h"
#include "usb0_host.h"

/* ------------------ HcControl Register --------------------- */
#define OR_CONTROL_PLE                  (0x00000004)
#define OR_CONTROL_CLE                  (0x00000010)
#define OR_CONTROL_BLE                  (0x00000020)
/* ----------------- HcCommandStatus Register ----------------- */
#define OR_CMD_STATUS_HCR               (0x00000001)
#define OR_CMD_STATUS_CLF               (0x00000002)
#define OR_CMD_STATUS_BLF               (0x00000004)
#define OR_CMD_STATUS_OCR               (0x00000008)
/* --------------- HcInterruptStatus Register ----------------- */
#define OR_INTR_STATUS_WDH              (0x00000002)
#define OR_INTR_STATUS_RHSC             (0x00000040)
/* --------------- HcInterruptEnable Register ----------------- */
#define OR_INTR_ENABLE_WDH              (0x00000002)
#define OR_INTR_ENABLE_RHSC             (0x00000040)
/* -------------- HcRhPortStatus[1:NDP] Register -------------- */
#define OR_RH_PORT_CSC                  (0x00010000)
#define OR_RH_PORT_LSDA                 (0x00000200)
#define OR_RH_PORT_PRS                  (0x00000010)
#define OR_RH_PORT_POCI                 (0x00000008)
#define OR_RH_PORT_CCS                  (0x00000001)

#define ED_SKIP                         (0x00004000)   /* Skip this ep in queue */
#define ED_TOGLE_CARRY                  (0x00000002)
#define ED_HALTED                       (0x00000001)

#define TD_SETUP                        (0x00000000)   /* Direction of Setup Packet */
#define TD_OUT                          (0x00080000)   /* Direction Out */
#define TD_TOGGLE_0                     (0x02000000)   /* Toggle 0 */
#define TD_TOGGLE_1                     (0x03000000)   /* Toggle 1 */

/* -------------- USB Standard Requests  -------------- */
#define GET_STATUS                      (0x00)
#define SET_FEATURE                     (0x03)
#define SET_ADDRESS                     (0x05)

#define TD_CTL_MSK_DP                   (0x00180000)
#define TD_CTL_MSK_T                    (0x03000000)
#define TD_CTL_MSK_CC                   (0xF0000000)
#define TD_CTL_MSK_EC                   (0x0C000000)
#define TD_CTL_SHFT_CC                  (28)
#define TD_CTL_SHFT_EC                  (26)
#define TD_CTL_SHFT_T                   (24)
#define ED_SHFT_TOGLE_CARRY             (1)
#define SIG_GEN_LIST_REQ                (1)

#define CTL_TRANS_TIMEOUT               (1000)
#define BLK_TRANS_TIMEOUT               (5)
#define INT_TRANS_MAX_NUM               (4)    /* min:1 max:4 */
#define TOTAL_SEM_NUM                   (5 + (2 * INT_TRANS_MAX_NUM))

#define PORT_LOW_SPEED                  (0x00000200)
#define PORT_HIGH_SPEED                 (0x00000400)
#define PORT_NUM                        (16 + 1) /* num + root(1) */

typedef struct tag_hctd {
    uint32_t         control;        /* Transfer descriptor control */
    uint8_t          *currBufPtr;    /* Physical address of current buffer pointer */
    struct tag_hctd  *nextTD;        /* Physical pointer to next Transfer Descriptor */
    uint8_t          *bufEnd;        /* Physical address of end of buffer */
} hctd_t;

typedef struct tag_hced {
    uint32_t         control;        /* Endpoint descriptor control */
    hctd_t           *tailTD;        /* Physical address of tail in Transfer descriptor list */
    hctd_t           *headTD;        /* Physcial address of head in Transfer descriptor list */
    struct tag_hced  *nextED;        /* Physical address of next Endpoint descriptor */
} hced_t;

typedef struct tag_hcca {
    uint32_t         IntTable[32];   /* Interrupt Table */
    uint32_t         FrameNumber;    /* Frame Number */
    uint32_t         DoneHead;       /* Done Head */
    volatile uint8_t Reserved[116];  /* Reserved for future use */
    volatile uint8_t Unknown[4];     /* Unused */
} hcca_t;

typedef struct tag_usb_ohci_reg {
    volatile uint32_t HcRevision;
    volatile uint32_t HcControl;
    volatile uint32_t HcCommandStatus;
    volatile uint32_t HcInterruptStatus;
    volatile uint32_t HcInterruptEnable;
    volatile uint32_t HcInterruptDisable;
    volatile uint32_t HcHCCA;
    volatile uint32_t HcPeriodCurrentED;
    volatile uint32_t HcControlHeadED;
    volatile uint32_t HcControlCurrentED;
    volatile uint32_t HcBulkHeadED;
    volatile uint32_t HcBulkCurrentED;
    volatile uint32_t HcDoneHead;
    volatile uint32_t HcFmInterval;
    volatile uint32_t HcFmRemaining;
    volatile uint32_t HcFmNumber;
    volatile uint32_t HcPeriodicStart;
    volatile uint32_t HcLSThreshold;
    volatile uint32_t HcRhDescriptorA;
    volatile uint32_t HcRhDescriptorB;
    volatile uint32_t HcRhStatus;
    volatile uint32_t HcRhPortStatus1;
} usb_ohci_reg_t;

typedef struct tag_genelal_ed {
    osThreadId      tskid;
    osSemaphoreId   semid_wait;
    osSemaphoreId   semid_list;
    hctd_t          *p_curr_td;
    hced_t          *p_curr_ed;
    uint32_t        pipe_no;
    uint32_t        trans_wait;
    uint32_t        cycle_time;
    uint8_t         *p_start_buf;
} genelal_ed_t;

typedef struct tag_tdinfo {
    uint32_t         count;
    uint32_t         direction;
    uint32_t         msp;
    uint16_t         devadr;
    uint16_t         speed;         /* 1:Speed = Low */
    uint8_t          endpoint_no;
} tdinfo_t;

typedef struct tag_split_trans {
    uint16_t        root_devadr;
    uint16_t        get_port;
    uint16_t        port_speed;
    uint16_t        reset_port;
    uint32_t        seq_cnt;
    uint32_t        port_sts_bits[PORT_NUM];
} split_trans_t;

static void callback_task(void const * argument);
static void control_ed_task(void const * argument);
static void bulk_ed_task(void const * argument);
static void int_ed_task(void const * argument);
static int32_t int_trans_doing(hced_t *p_ed, uint32_t index);
static int32_t chk_genelal_ed(genelal_ed_t *p_g_ed);
static void chk_td_done(genelal_ed_t *p_g_ed);
static void chk_split_trans_setting(genelal_ed_t *p_g_ed);
static void set_split_trans_setting(void);
static void control_trans(genelal_ed_t *p_g_ed);
static void bulk_trans(genelal_ed_t *p_g_ed);
static void int_trans_setting(genelal_ed_t *p_g_ed, uint32_t index);
static uint32_t chk_cycle(hced_t *p_ed);
static void int_trans(genelal_ed_t *p_g_ed);
static void get_td_info(genelal_ed_t *p_g_ed, tdinfo_t *p_td_info);
static void set_togle(uint32_t pipe, hctd_t *p_td, hced_t *p_ed);
static void connect_check(void);

extern USB_HOST_CFG_PIPETBL_t  usb0_host_blk_ep_tbl1[];
extern USB_HOST_CFG_PIPETBL_t  usb0_host_int_ep_tbl1[];

static usb_ohci_reg_t usb_reg;
static usb_ohci_reg_t *p_usb_reg     = &usb_reg;
static usbisr_fnc_t   *p_usbisr_cb   = NULL;
static osSemaphoreId  semid_cb       = NULL;
static uint32_t       connect_change = 0xFFFFFFFF;
static uint32_t       init_end       = 0;
static genelal_ed_t   ctl_ed;
static genelal_ed_t   blk_ed;
static genelal_ed_t   int_ed[INT_TRANS_MAX_NUM];
static split_trans_t  split_ctl;

osSemaphoreDef(ohciwrapp_sem_01);
osSemaphoreDef(ohciwrapp_sem_02);
osSemaphoreDef(ohciwrapp_sem_03);
osSemaphoreDef(ohciwrapp_sem_04);
osSemaphoreDef(ohciwrapp_sem_05);
osSemaphoreDef(ohciwrapp_sem_06);
osSemaphoreDef(ohciwrapp_sem_07);
#if (INT_TRANS_MAX_NUM >= 2)
osSemaphoreDef(ohciwrapp_sem_08);
osSemaphoreDef(ohciwrapp_sem_09);
#endif
#if (INT_TRANS_MAX_NUM >= 3)
osSemaphoreDef(ohciwrapp_sem_10);
osSemaphoreDef(ohciwrapp_sem_11);
#endif
#if (INT_TRANS_MAX_NUM >= 4)
osSemaphoreDef(ohciwrapp_sem_12);
osSemaphoreDef(ohciwrapp_sem_13);
#endif

osThreadDef(callback_task,   osPriorityHigh,        512);
osThreadDef(control_ed_task, osPriorityNormal,      512);
osThreadDef(bulk_ed_task,    osPriorityNormal,      512);
osThreadDef(int_ed_task,     osPriorityAboveNormal, 512);

void ohciwrapp_init(usbisr_fnc_t *p_usbisr_fnc, uint32_t hi_speed) {
    static const osSemaphoreDef_t * const sem_def_tbl[TOTAL_SEM_NUM] = {
        osSemaphore(ohciwrapp_sem_01), osSemaphore(ohciwrapp_sem_02), osSemaphore(ohciwrapp_sem_03)
      , osSemaphore(ohciwrapp_sem_04), osSemaphore(ohciwrapp_sem_05), osSemaphore(ohciwrapp_sem_06)
      , osSemaphore(ohciwrapp_sem_07)
#if (INT_TRANS_MAX_NUM >= 2)
      , osSemaphore(ohciwrapp_sem_08), osSemaphore(ohciwrapp_sem_09)
#endif
#if (INT_TRANS_MAX_NUM >= 3)
      , osSemaphore(ohciwrapp_sem_10), osSemaphore(ohciwrapp_sem_11)
#endif
#if (INT_TRANS_MAX_NUM >= 4)
      , osSemaphore(ohciwrapp_sem_12), osSemaphore(ohciwrapp_sem_13)
#endif
    };
    uint32_t cnt;
    uint32_t index = 0;

    /* Disables interrupt for usb */
    GIC_DisableIRQ(USBI0_IRQn);

    /* P4_1(USB0_EN) */
    GPIOP4      &= ~0x0002;         /* Outputs low level */
    GPIOPMC4    &= ~0x0002;         /* Port mode */
    GPIOPM4     &= ~0x0002;         /* Output mode */

    p_usbisr_cb = p_usbisr_fnc;
    if (hi_speed == 0) {
        g_usb0_host_SupportUsbDeviceSpeed = USB_HOST_FULL_SPEED;
    } else {
        g_usb0_host_SupportUsbDeviceSpeed = USB_HOST_HIGH_SPEED;
    }
    p_usb_reg->HcRevision         = 0x00000010;
    p_usb_reg->HcControl          = 0x00000000;
    p_usb_reg->HcCommandStatus    = 0x00000000;
    p_usb_reg->HcInterruptStatus  = 0x00000000;
    p_usb_reg->HcInterruptEnable  = 0x00000000;
    p_usb_reg->HcInterruptDisable = 0x00000000;
    p_usb_reg->HcHCCA             = 0x00000000;
    p_usb_reg->HcPeriodCurrentED  = 0x00000000;
    p_usb_reg->HcControlHeadED    = 0x00000000;
    p_usb_reg->HcControlCurrentED = 0x00000000;
    p_usb_reg->HcBulkHeadED       = 0x00000000;
    p_usb_reg->HcBulkCurrentED    = 0x00000000;
    p_usb_reg->HcDoneHead         = 0x00000000;
    p_usb_reg->HcFmInterval       = 0x00002EDF;
    p_usb_reg->HcFmRemaining      = 0x00002EDF;
    p_usb_reg->HcFmNumber         = 0x00000000;
    p_usb_reg->HcPeriodicStart    = 0x00000000;
    p_usb_reg->HcLSThreshold      = 0x00000628;
    p_usb_reg->HcRhDescriptorA    = 0xFF000901;
    p_usb_reg->HcRhDescriptorB    = 0x00020000;
    p_usb_reg->HcRhStatus         = 0x00000000;
    p_usb_reg->HcRhPortStatus1    = 0x00000000;

    GPIOP4      |=  0x0002;         /* P4_1 Outputs high level */
    osDelay(5);
    GPIOP4      &= ~0x0002;         /* P4_1 Outputs low level */
    osDelay(10);

    if (init_end == 0) {
        (void)memset(&ctl_ed, 0, sizeof(ctl_ed));
        (void)memset(&blk_ed, 0, sizeof(blk_ed));
        (void)memset(&int_ed[0], 0, sizeof(int_ed));

        /* callback */
        semid_cb = osSemaphoreCreate(sem_def_tbl[index], 0);
        index++;
        (void)osThreadCreate(osThread(callback_task), 0);

        /* control transfer */
        ctl_ed.semid_wait = osSemaphoreCreate(sem_def_tbl[index], 0);
        index++;
        ctl_ed.semid_list = osSemaphoreCreate(sem_def_tbl[index], 0);
        index++;
        ctl_ed.tskid = osThreadCreate(osThread(control_ed_task), 0);

        /* bulk transfer */
        blk_ed.semid_wait = osSemaphoreCreate(sem_def_tbl[index], 0);
        index++;
        blk_ed.semid_list =  osSemaphoreCreate(sem_def_tbl[index], 0);
        index++;
        blk_ed.tskid = osThreadCreate(osThread(bulk_ed_task), 0);

        /* interrupt transfer */
        for (cnt = 0; cnt < INT_TRANS_MAX_NUM; cnt++) {
            int_ed[cnt].semid_wait = osSemaphoreCreate(sem_def_tbl[index], 0);
            index++;
            int_ed[cnt].semid_list = osSemaphoreCreate(sem_def_tbl[index], 0);
            index++;
            int_ed[cnt].tskid = osThreadCreate(osThread(int_ed_task), (void *)cnt);
        }
        init_end = 1;
    }
}

uint32_t ohciwrapp_reg_r(uint32_t reg_ofs) {
    if (init_end == 0) {
        return 0;
    }

    return *(uint32_t *)((uint8_t *)p_usb_reg + reg_ofs);
}

void ohciwrapp_reg_w(uint32_t reg_ofs, uint32_t set_data) {
    uint32_t cnt;
    uint32_t last_data;
    hcca_t   *p_hcca;

    if (init_end == 0) {
        return;
    }

    switch (reg_ofs) {
        case OHCI_REG_CONTROL:
            last_data            = p_usb_reg->HcControl;
            p_usb_reg->HcControl = (set_data & 0x000007FF);
            if ((last_data & OR_CONTROL_CLE) != (set_data & OR_CONTROL_CLE)) {
                /* change CLE */
                if ((set_data & OR_CONTROL_CLE) != 0) {
                    (void)osSemaphoreRelease(ctl_ed.semid_list);
                } else {
                    if (ctl_ed.trans_wait == 1) {
                        ctl_ed.trans_wait = 0;
                        (void)osSemaphoreRelease(ctl_ed.semid_wait);
                    }
                    (void)osSemaphoreWait(ctl_ed.semid_list, osWaitForever);
                }
            }
            if ((last_data & OR_CONTROL_BLE) != (set_data & OR_CONTROL_BLE)) {
                /* change BLE */
                if ((set_data & OR_CONTROL_BLE) != 0) {
                    (void)osSemaphoreRelease(blk_ed.semid_list);
                } else {
                    if (blk_ed.trans_wait == 1) {
                        blk_ed.trans_wait = 0;
                        (void)osSemaphoreRelease(blk_ed.semid_wait);
                    }
                    (void)osSemaphoreWait(blk_ed.semid_list, osWaitForever);
                }
            }
            if ((last_data & OR_CONTROL_PLE) != (set_data & OR_CONTROL_PLE)) {
                /* change PLE */
                for (cnt = 0; cnt < INT_TRANS_MAX_NUM; cnt++) {
                    if ((set_data & OR_CONTROL_PLE) != 0) {
                        (void)osSemaphoreRelease(int_ed[cnt].semid_list);
                    } else {
                        if (int_ed[cnt].trans_wait == 1) {
                            int_ed[cnt].trans_wait = 0;
                            (void)osSemaphoreRelease(int_ed[cnt].semid_wait);
                        }
                        (void)osSemaphoreWait(int_ed[cnt].semid_list, osWaitForever);
                    }
                }
            }
            break;
        case OHCI_REG_COMMANDSTATUS:
            if ((set_data & OR_CMD_STATUS_HCR) != 0) {    /* HostController Reset */
                p_usb_reg->HcCommandStatus |= OR_CMD_STATUS_HCR;
                if (usb0_api_host_init(16, g_usb0_host_SupportUsbDeviceSpeed, USBHCLOCK_X1_48MHZ) == USB_HOST_ATTACH) {
                    ohciwrapp_loc_Connect(1);
                }
                p_usb_reg->HcCommandStatus &= ~OR_CMD_STATUS_HCR;
            }
            if ((set_data & OR_CMD_STATUS_CLF) != 0) {
                p_usb_reg->HcCommandStatus |= OR_CMD_STATUS_CLF;
                osSignalSet(ctl_ed.tskid, SIG_GEN_LIST_REQ);
            }
            if ((set_data & OR_CMD_STATUS_BLF) != 0) {
                p_usb_reg->HcCommandStatus |= OR_CMD_STATUS_BLF;
                osSignalSet(blk_ed.tskid, SIG_GEN_LIST_REQ);
            }
            if ((set_data & OR_CMD_STATUS_OCR) != 0) {
                p_usb_reg->HcCommandStatus |= OR_CMD_STATUS_OCR;
            } else {
                p_usb_reg->HcCommandStatus &= ~OR_CMD_STATUS_OCR;
            }
            break;
        case OHCI_REG_INTERRUPTSTATUS:
            if (((p_usb_reg->HcInterruptStatus & OR_INTR_STATUS_WDH) != 0)
             && ((set_data & OR_INTR_STATUS_WDH) != 0)) {
                if (p_usb_reg->HcDoneHead != 0x00000000) {
                    p_hcca                       =  (hcca_t *)p_usb_reg->HcHCCA;
                    p_hcca->DoneHead             =  p_usb_reg->HcDoneHead;
                    p_usb_reg->HcDoneHead        =  0x00000000;
                    p_usb_reg->HcInterruptStatus |= OR_INTR_STATUS_WDH;
                    (void)osSemaphoreRelease(semid_cb);
                } else {
                    p_usb_reg->HcInterruptStatus &= ~OR_INTR_STATUS_WDH;
                }
            }
            if ((set_data & OR_INTR_STATUS_RHSC) != 0) {
                p_usb_reg->HcInterruptStatus &= ~OR_INTR_STATUS_RHSC;
            }
            break;
        case OHCI_REG_INTERRUPTENABLE:
        case OHCI_REG_INTERRUPTDISABLE:
        case OHCI_REG_HCCA:
        case OHCI_REG_CONTROLHEADED:
        case OHCI_REG_CONTROLCURRENTED:
        case OHCI_REG_BULKHEADED:
        case OHCI_REG_BULKCURRENTED:
        case OHCI_REG_FMINTERVAL:
        case OHCI_REG_FMREMAINING:
        case OHCI_REG_PERIODICSTART:
        case OHCI_REG_LSTHRESHOLD:
        case OHCI_REG_RHDESCRIPTORA:
        case OHCI_REG_RHDESCRIPTORB:
        case OHCI_REG_RHSTATUS:
            *(uint32_t *)((uint8_t *)p_usb_reg + reg_ofs) = set_data;
            break;
        case OHCI_REG_RHPORTSTATUS1:
            p_usb_reg->HcRhPortStatus1 &= ~(set_data & 0xFFFF0000);
            if ((set_data & OR_RH_PORT_PRS) != 0) {    /* Set Port Reset */
                p_usb_reg->HcRhPortStatus1 |= OR_RH_PORT_PRS;
                usb0_host_UsbBusReset();
                p_usb_reg->HcRhPortStatus1 &= ~OR_RH_PORT_PRS;
            }
            break;
        case OHCI_REG_REVISION:
        case OHCI_REG_PERIODCURRENTED:
        case OHCI_REG_DONEHEADED:
        case OHCI_REG_FMNUMBER:
        default:
            /* Do Nothing */
            break;
    }
}

static void callback_task(void const * argument) {
    usbisr_fnc_t *p_wk_cb = p_usbisr_cb;

    if (p_wk_cb == NULL) {
        return;
    }

    while (1) {
        osSemaphoreWait(semid_cb, osWaitForever);
        if (connect_change != 0xFFFFFFFF) {
            connect_change = 0xFFFFFFFF;
            connect_check();
        }
        p_wk_cb();
    }
}

static void control_ed_task(void const * argument) {
    while (1) {
        osSignalWait(SIG_GEN_LIST_REQ, osWaitForever);
        (void)osSemaphoreWait(ctl_ed.semid_list, osWaitForever);
        while ((p_usb_reg->HcControl & OR_CONTROL_CLE) != 0) {
            if ((p_usb_reg->HcControlCurrentED == 0)
             && ((p_usb_reg->HcCommandStatus & OR_CMD_STATUS_CLF) != 0)) {
                p_usb_reg->HcControlCurrentED =  p_usb_reg->HcControlHeadED;
                p_usb_reg->HcCommandStatus    &= ~OR_CMD_STATUS_CLF;
            }
            if (p_usb_reg->HcControlCurrentED != 0) {
                ctl_ed.p_curr_ed = (hced_t *)p_usb_reg->HcControlCurrentED;
                if (chk_genelal_ed(&ctl_ed) != 0) {
                    control_trans(&ctl_ed);
                    chk_split_trans_setting(&ctl_ed);
                    p_usb_reg->HcCommandStatus |= OR_CMD_STATUS_CLF;
                    chk_td_done(&ctl_ed);
                }
                p_usb_reg->HcControlCurrentED = (uint32_t)ctl_ed.p_curr_ed->nextED;
            } else {
                break;
            }
        }
        if ((p_usb_reg->HcCommandStatus & OR_CMD_STATUS_CLF) != 0) {
            osSignalSet(ctl_ed.tskid, SIG_GEN_LIST_REQ);
        }
        (void)osSemaphoreRelease(ctl_ed.semid_list);
    }
}

static void bulk_ed_task(void const * argument) {
    while (1) {
        osSignalWait(SIG_GEN_LIST_REQ, osWaitForever);
        (void)osSemaphoreWait(blk_ed.semid_list, osWaitForever);
        while ((p_usb_reg->HcControl & OR_CONTROL_BLE) != 0) {
            if ((p_usb_reg->HcBulkCurrentED == 0)
             && ((p_usb_reg->HcCommandStatus & OR_CMD_STATUS_BLF) != 0)) {
                p_usb_reg->HcBulkCurrentED =  p_usb_reg->HcBulkHeadED;
                p_usb_reg->HcCommandStatus &= ~OR_CMD_STATUS_BLF;
            }
            if (p_usb_reg->HcBulkCurrentED != 0) {
                blk_ed.p_curr_ed = (hced_t *)p_usb_reg->HcBulkCurrentED;
                if (chk_genelal_ed(&blk_ed) != 0) {
                    bulk_trans(&blk_ed);
                    p_usb_reg->HcCommandStatus |= OR_CMD_STATUS_BLF;
                    chk_td_done(&blk_ed);
                }
                p_usb_reg->HcBulkCurrentED = (uint32_t)blk_ed.p_curr_ed->nextED;
            } else {
                break;
            }
        }
        if ((p_usb_reg->HcCommandStatus & OR_CMD_STATUS_BLF) != 0) {
            osSignalSet(blk_ed.tskid, SIG_GEN_LIST_REQ);
        }
        (void)osSemaphoreRelease(blk_ed.semid_list);
    }
}

static void int_ed_task(void const * argument) {
    genelal_ed_t *p_int_ed = &int_ed[(uint32_t)argument];
    uint32_t     cnt;
    uint32_t     wait_cnt = 0;
    hcca_t       *p_hcca;
    hced_t       *p_ed;

    while (1) {
        (void)osSemaphoreWait(p_int_ed->semid_list, osWaitForever);
        if (p_int_ed->p_curr_ed == NULL) {
            for (cnt = 0; (cnt < 32) && ((p_usb_reg->HcControl & OR_CONTROL_PLE) != 0)
                                                 && (p_int_ed->p_curr_ed == NULL); cnt++) {
                p_hcca = (hcca_t *)p_usb_reg->HcHCCA;
                p_ed   = (hced_t *)p_hcca->IntTable[cnt];
                while ((p_ed != NULL) && ((p_usb_reg->HcControl & OR_CONTROL_PLE) != 0)
                                                        && (p_int_ed->p_curr_ed == NULL)) {
                    if (int_trans_doing(p_ed, (uint32_t)argument) == 0) {
                        p_int_ed->p_curr_ed = p_ed;
                        if (chk_genelal_ed(p_int_ed) != 0) {
                            int_trans_setting(p_int_ed, (uint32_t)argument);
                        } else {
                            p_int_ed->p_curr_ed = NULL;
                        }
                    }
                    p_ed = p_ed->nextED;
                }
            }
        }
        if (p_int_ed->p_curr_ed != NULL) {
            while ((p_usb_reg->HcControl & OR_CONTROL_PLE) != 0) {
                if (chk_genelal_ed(p_int_ed) != 0) {
                    int_trans(p_int_ed);
                    chk_td_done(p_int_ed);
                    wait_cnt = p_int_ed->cycle_time;
                } else {
                    if (wait_cnt > 0) {
                        wait_cnt--;
                    } else {
                        p_int_ed->p_curr_ed = NULL;
                    }
                    break;
                }
            }
        }
        (void)osSemaphoreRelease(p_int_ed->semid_list);
        if (p_int_ed->p_curr_ed == NULL) {
            osDelay(10);
        } else {
            osDelay(1);
        }
    }
}

static int32_t int_trans_doing(hced_t *p_ed, uint32_t index) {
    uint32_t cnt;
    int32_t  ret = 0;

    for (cnt = 0; cnt < INT_TRANS_MAX_NUM; cnt++) {
        if ((index != cnt) && (int_ed[cnt].p_curr_ed == p_ed)) {
            ret = 1;
        }
    }

    return ret;
}

static int32_t chk_genelal_ed(genelal_ed_t *p_g_ed){
    int32_t ret   = 0;
    hced_t  *p_ed = p_g_ed->p_curr_ed;

    if (((p_ed->control & ED_SKIP)   != 0)
     || (((uint32_t)p_ed->headTD & ED_HALTED)  != 0)
     || (((uint32_t)p_ed->tailTD & 0xFFFFFFF0) == ((uint32_t)p_ed->headTD & 0xFFFFFFF0))) {
        /* Do Nothing */
    } else if ((p_ed->control & 0x0000007F) > 10) {
        p_ed->headTD = (hctd_t *)((uint32_t)p_ed->headTD | ED_HALTED);
    } else {
        p_g_ed->p_curr_td = (hctd_t *)((uint32_t)p_ed->headTD & 0xFFFFFFF0);
        if (p_g_ed->p_curr_td == NULL) {
            p_ed->headTD = (hctd_t *)((uint32_t)p_ed->headTD | ED_HALTED);
        } else {
            p_g_ed->p_start_buf = p_g_ed->p_curr_td->currBufPtr;
            ret = 1;
        }
    }

    return ret;
}

static void chk_td_done(genelal_ed_t *p_g_ed) {
    hcca_t   *p_hcca;
    uint32_t ConditionCode = RZA_IO_RegRead_32(&p_g_ed->p_curr_td->control, TD_CTL_SHFT_CC, TD_CTL_MSK_CC);

    if ((ConditionCode != TD_CC_NOT_ACCESSED_1) && (ConditionCode != TD_CC_NOT_ACCESSED_2)) {
        p_g_ed->p_curr_ed->headTD = (hctd_t *)(((uint32_t)p_g_ed->p_curr_td->nextTD & 0xFFFFFFF0)
                                  |            ((uint32_t)p_g_ed->p_curr_ed->headTD & 0x0000000F));
        p_g_ed->p_curr_td->nextTD = (hctd_t *)p_usb_reg->HcDoneHead;
        p_usb_reg->HcDoneHead     = (uint32_t)p_g_ed->p_curr_td;
        if ((p_usb_reg->HcInterruptStatus & OR_INTR_STATUS_WDH) == 0) {
            p_hcca                       =  (hcca_t *)p_usb_reg->HcHCCA;
            p_hcca->DoneHead             =  p_usb_reg->HcDoneHead;
            p_usb_reg->HcDoneHead        =  0x00000000;
            p_usb_reg->HcInterruptStatus |= OR_INTR_STATUS_WDH;
            (void)osSemaphoreRelease(semid_cb);
        }
    }
}

static void chk_split_trans_setting(genelal_ed_t *p_g_ed) {
    uint8_t   *p_buf;
    tdinfo_t  td_info;

    /* Hi-Speed mode only */
    if (g_usb0_host_UsbDeviceSpeed != USB_HOST_HIGH_SPEED) {
        return;
    }

    if (RZA_IO_RegRead_32(&p_g_ed->p_curr_td->control, TD_CTL_SHFT_CC, TD_CTL_MSK_CC) != TD_CC_NOERROR) {
        return;
    }

    get_td_info(p_g_ed, &td_info);
    p_buf = p_g_ed->p_start_buf;

    if (td_info.direction == 0) {
        uint8_t  bRequest = p_buf[1];
        uint16_t wValue   = (p_buf[3] << 8) + p_buf[2];
        uint16_t wIndx    = (p_buf[5] << 8) + p_buf[4];
        uint16_t devadd;

        if ((td_info.devadr == 0) && (bRequest == SET_ADDRESS)) {
            /* SET_ADDRESS */
            usb0_host_get_devadd(USB_HOST_DEVICE_0, &devadd);
            usb0_host_set_devadd(wValue, &devadd);
            if (split_ctl.root_devadr == 0) {
                split_ctl.root_devadr = wValue; /* New Address */
            }
        } else if ((td_info.devadr == split_ctl.root_devadr) && (bRequest == SET_FEATURE)
                && (wValue == 0x0004) && (split_ctl.root_devadr != 0)) {
            /* SET_FEATURE PORT_RESET */
            split_ctl.reset_port = (wIndx & 0x00FF);
        } else if ((td_info.devadr == split_ctl.root_devadr) && (bRequest == GET_STATUS)) {
            /* GET_STATUS */
            split_ctl.get_port = wIndx;
            split_ctl.seq_cnt = 1;
        } else {
            /* Do Nothing */
        }
    } else if (td_info.direction == 2) {
        if ((td_info.devadr == split_ctl.root_devadr) && (split_ctl.seq_cnt == 1)) {
            if (split_ctl.get_port < PORT_NUM) {
                split_ctl.port_sts_bits[split_ctl.get_port] = (p_buf[1] << 8) + p_buf[0];
            }
            split_ctl.seq_cnt = 0;
        }
    } else {
        /* Do Nothing */
    }
}

static void set_split_trans_setting(void) {
    uint16_t port_speed;
    uint16_t devadd;

    if ((split_ctl.root_devadr != 0) && (split_ctl.reset_port != 0) && (split_ctl.reset_port < PORT_NUM)) {
        usb0_host_get_devadd(USB_HOST_DEVICE_0, &devadd);
        RZA_IO_RegWrite_16(&devadd, split_ctl.root_devadr, USB_DEVADDn_UPPHUB_SHIFT, USB_DEVADDn_UPPHUB);
        RZA_IO_RegWrite_16(&devadd, split_ctl.reset_port, USB_DEVADDn_HUBPORT_SHIFT, USB_DEVADDn_HUBPORT);
        if ((split_ctl.port_sts_bits[split_ctl.reset_port] & PORT_HIGH_SPEED) != 0) {
            port_speed = USB_HOST_HIGH_SPEED;
        } else if ((split_ctl.port_sts_bits[split_ctl.reset_port] & PORT_LOW_SPEED) != 0) {
            port_speed = USB_HOST_LOW_SPEED;
        } else {
            port_speed = USB_HOST_FULL_SPEED;
        }
        RZA_IO_RegWrite_16(&devadd, port_speed, USB_DEVADDn_USBSPD_SHIFT, USB_DEVADDn_USBSPD);
        usb0_host_set_devadd(USB_HOST_DEVICE_0, &devadd);
        split_ctl.reset_port = 0;
    }
}

static void control_trans(genelal_ed_t *p_g_ed) {
    hctd_t   *p_td     = p_g_ed->p_curr_td;
    tdinfo_t td_info;
    uint16_t devadd;

    get_td_info(p_g_ed, &td_info);

    if (g_usb0_host_UsbDeviceSpeed == USB_HOST_HIGH_SPEED) {
        if (td_info.devadr == 0) {
            set_split_trans_setting();
        }
    } else {
        /* When a non-Hi-Speed, the communication speed is determined from the TD. */
        usb0_host_get_devadd(USB_HOST_DEVICE_0, &devadd);
        if (td_info.speed == 1) {
            RZA_IO_RegWrite_16(&devadd, USB_HOST_LOW_SPEED, USB_DEVADDn_USBSPD_SHIFT, USB_DEVADDn_USBSPD);
        } else {
            RZA_IO_RegWrite_16(&devadd, USB_HOST_FULL_SPEED, USB_DEVADDn_USBSPD_SHIFT, USB_DEVADDn_USBSPD);
        }
        usb0_host_set_devadd(td_info.devadr, &devadd);
    }

    USB200.DCPMAXP  = (td_info.devadr << 12) + td_info.msp;
    if (td_info.direction == 0) {
        g_usb0_host_CmdStage = (USB_HOST_STAGE_SETUP | USB_HOST_CMD_IDLE);
    } else  if (td_info.count != 0) {
        g_usb0_host_CmdStage = (USB_HOST_STAGE_DATA | USB_HOST_CMD_IDLE);
    } else {
        g_usb0_host_CmdStage = (USB_HOST_STAGE_STATUS | USB_HOST_CMD_IDLE);
    }
    g_usb0_host_pipe_status[USB_HOST_PIPE0]  = USB_HOST_PIPE_WAIT;
    p_g_ed->pipe_no    = USB_HOST_PIPE0;

    p_g_ed->trans_wait = 1;

    if (td_info.direction == 0) {
        uint16_t Req  = (p_td->currBufPtr[1] << 8) + p_td->currBufPtr[0];
        uint16_t Val  = (p_td->currBufPtr[3] << 8) + p_td->currBufPtr[2];
        uint16_t Indx = (p_td->currBufPtr[5] << 8) + p_td->currBufPtr[4];
        uint16_t Len  = (p_td->currBufPtr[7] << 8) + p_td->currBufPtr[6];

        g_usb0_host_data_pointer[USB_HOST_PIPE0] = p_td->bufEnd;
        usb0_host_SetupStage(Req, Val, Indx, Len);
    } else if (td_info.direction == 1) {
        usb0_host_CtrlWriteStart(td_info.count, p_td->currBufPtr);
    } else {
        usb0_host_CtrlReadStart(td_info.count, p_td->currBufPtr);
    }

    (void)osSemaphoreWait(p_g_ed->semid_wait, CTL_TRANS_TIMEOUT);
    if (p_g_ed->trans_wait == 1) {
        p_g_ed->trans_wait = 0;
        RZA_IO_RegWrite_32(&p_td->control, TD_CC_DEVICENOTRESPONDING, TD_CTL_SHFT_CC, TD_CTL_MSK_CC);
    }

    g_usb0_host_CmdStage &= (~USB_HOST_CMD_FIELD);
    g_usb0_host_CmdStage |= USB_HOST_CMD_IDLE;
    g_usb0_host_pipe_status[USB_HOST_PIPE0] = USB_HOST_PIPE_IDLE;
}

static void bulk_trans(genelal_ed_t *p_g_ed) {
    hctd_t                 *p_td = p_g_ed->p_curr_td;
    hced_t                 *p_ed = p_g_ed->p_curr_ed;
    tdinfo_t               td_info;
    USB_HOST_CFG_PIPETBL_t *user_table = &usb0_host_blk_ep_tbl1[0];
    uint8_t                wk_table[6];

    get_td_info(p_g_ed, &td_info);

    wk_table[0] = 0;
    wk_table[1] = USB_HOST_ENDPOINT_DESC;
    wk_table[2] = td_info.endpoint_no;
    if (td_info.direction == 2) {
        wk_table[2] |= USB_HOST_EP_IN;
    }
    wk_table[3] = USB_HOST_EP_BULK;
    wk_table[4] = (uint8_t)td_info.msp;
    wk_table[5] = (uint8_t)(td_info.msp >> 8);
    p_g_ed->pipe_no    = user_table->pipe_number;
    usb0_api_host_SetEndpointTable(td_info.devadr, user_table, wk_table);

    set_togle(p_g_ed->pipe_no, p_td, p_ed);

    p_g_ed->trans_wait = 1;
    if (td_info.direction == 1) {
        usb0_host_start_send_transfer(p_g_ed->pipe_no, td_info.count, p_td->currBufPtr);
    } else {
        usb0_host_start_receive_transfer(p_g_ed->pipe_no, td_info.count, p_td->currBufPtr);
    }

    (void)osSemaphoreWait(p_g_ed->semid_wait, BLK_TRANS_TIMEOUT);
    usb0_host_stop_transfer(p_g_ed->pipe_no);
}

static void int_trans_setting(genelal_ed_t *p_g_ed, uint32_t index) {
    hctd_t                 *p_td = p_g_ed->p_curr_td;
    hced_t                 *p_ed = p_g_ed->p_curr_ed;
    tdinfo_t               td_info;
    USB_HOST_CFG_PIPETBL_t *user_table = &usb0_host_int_ep_tbl1[index];
    uint8_t                wk_table[6];
    uint32_t               cycle_time;
    uint16_t               devadd;

    get_td_info(p_g_ed, &td_info);

    wk_table[0] = 0;
    wk_table[1] = USB_HOST_ENDPOINT_DESC;
    wk_table[2] = td_info.endpoint_no;
    if (td_info.direction == 2) {
        wk_table[2] |= USB_HOST_EP_IN;
    }
    wk_table[3] = USB_HOST_EP_INT;
    wk_table[4] = (uint8_t)td_info.msp;
    wk_table[5] = (uint8_t)(td_info.msp >> 8);
    cycle_time  = chk_cycle(p_ed);
    p_g_ed->cycle_time = cycle_time;
    user_table->pipe_cycle = 0;
    while (cycle_time > 1) {
        cycle_time >>= 1;
        user_table->pipe_cycle++;
    }
    if (g_usb0_host_UsbDeviceSpeed == USB_HOST_HIGH_SPEED) {
        usb0_host_get_devadd(td_info.devadr, &devadd);
        if (RZA_IO_RegRead_16(&devadd, USB_DEVADDn_USBSPD_SHIFT, USB_DEVADDn_USBSPD) == USB_HOST_HIGH_SPEED) {
            user_table->pipe_cycle += 3;
            if (user_table->pipe_cycle > 7) {
                user_table->pipe_cycle = 7;
            }
        }
    }

    p_g_ed->pipe_no    = user_table->pipe_number;
    usb0_api_host_SetEndpointTable(td_info.devadr, user_table, wk_table);

    set_togle(p_g_ed->pipe_no, p_td, p_ed);
}

static uint32_t chk_cycle(hced_t *p_ed) {
    uint32_t     cnt;
    uint32_t     hit_cnt    = 0;
    uint32_t     cycle_time = 1;
    hcca_t       *p_hcca;
    hced_t       *p_wk_ed;

    p_hcca = (hcca_t *)p_usb_reg->HcHCCA;

    for (cnt = 0; cnt < 32; cnt++) {
        p_wk_ed = (hced_t *)p_hcca->IntTable[cnt];
        while (p_wk_ed != NULL) {
            if (p_wk_ed == p_ed) {
                hit_cnt++;
                break;
            }
            p_wk_ed = p_wk_ed->nextED;
        }
    }
    if (hit_cnt < 2) {
        cycle_time = 32;
    } else if (hit_cnt < 4) {
        cycle_time = 16;
    } else if (hit_cnt < 8) {
        cycle_time = 8;
    } else if (hit_cnt < 16) {
        cycle_time = 4;
    } else if (hit_cnt < 32) {
        cycle_time = 2;
    } else{
        cycle_time = 1;
    }

    return cycle_time;
}

static void int_trans(genelal_ed_t *p_g_ed) {
    hctd_t   *p_td = p_g_ed->p_curr_td;
    tdinfo_t td_info;

    get_td_info(p_g_ed, &td_info);
    p_g_ed->trans_wait = 1;
    if (td_info.direction == 1) {
        usb0_host_start_send_transfer(p_g_ed->pipe_no, td_info.count, p_td->currBufPtr);
    } else {
        usb0_host_start_receive_transfer(p_g_ed->pipe_no, td_info.count, p_td->currBufPtr);
    }
    (void)osSemaphoreWait(p_g_ed->semid_wait, osWaitForever);
    usb0_host_stop_transfer(p_g_ed->pipe_no);
}

static void get_td_info(genelal_ed_t *p_g_ed, tdinfo_t *p_td_info) {
    hctd_t *p_td = p_g_ed->p_curr_td;
    hced_t *p_ed = p_g_ed->p_curr_ed;

    p_td_info->endpoint_no = (uint8_t)((p_ed->control >> 7) & 0x0000000F);
    p_td_info->msp         = (p_ed->control >> 16) & 0x000007FF;
    p_td_info->devadr      = p_ed->control & 0x0000000F;
    p_td_info->speed       = (p_ed->control >> 13) & 0x00000001;
    p_td_info->direction   = (p_ed->control >> 11) & 0x00000003;
    if ((p_td_info->direction == 0) || (p_td_info->direction == 3)) {
        if ((p_td->control & TD_CTL_MSK_DP) == TD_SETUP) {
            p_td_info->direction = 0;
        } else if ((p_td->control & TD_CTL_MSK_DP) == TD_OUT) {
            p_td_info->direction = 1;
        } else {
            p_td_info->direction = 2;
        }
    }
    if (p_td->currBufPtr != NULL) {
        p_td_info->count = (uint32_t)p_td->bufEnd - (uint32_t)p_td->currBufPtr + 1;
    } else {
        p_td_info->count     = 0;
    }
}

static void set_togle(uint32_t pipe, hctd_t *p_td, hced_t *p_ed) {
    if ((p_td->control & TD_CTL_MSK_T) == TD_TOGGLE_0) {
        usb0_host_set_sqclr(pipe);
    } else if ((p_td->control & TD_CTL_MSK_T) == TD_TOGGLE_1) {
        usb0_host_set_sqset(pipe);
    } else if (((uint32_t)p_ed->headTD & ED_TOGLE_CARRY) == 0) {
        usb0_host_set_sqclr(pipe);
    } else {
        usb0_host_set_sqset(pipe);
    }
}

static void connect_check(void) {
    uint32_t cnt;
    uint32_t type = 0;
    uint16_t stat;
    uint16_t devadd = 0;
    uint32_t wk_HcRhPortStatus1 = p_usb_reg->HcRhPortStatus1;

    if (usb0_host_CheckAttach() == USB_HOST_ATTACH) {
        type = 1;
    }

    if ((((wk_HcRhPortStatus1 & OR_RH_PORT_CCS) == 0) && (type == 0))
     || (((wk_HcRhPortStatus1 & OR_RH_PORT_CCS) != 0) && (type != 0))) {
        return;
    }

    if (type == 0) {
        usb0_host_UsbDetach();
        wk_HcRhPortStatus1 &= ~OR_RH_PORT_CCS;
    } else {
        usb0_host_UsbAttach();
        stat = usb0_host_UsbBusReset();
        RZA_IO_RegWrite_16(&devadd, 0, USB_DEVADDn_UPPHUB_SHIFT, USB_DEVADDn_UPPHUB);
        RZA_IO_RegWrite_16(&devadd, 0, USB_DEVADDn_HUBPORT_SHIFT, USB_DEVADDn_HUBPORT);
        if (stat == USB_HOST_HSMODE) {
            wk_HcRhPortStatus1 &= ~OR_RH_PORT_LSDA;
            RZA_IO_RegWrite_16(&USB200.SOFCFG, 0, USB_SOFCFG_TRNENSEL_SHIFT, USB_SOFCFG_TRNENSEL);
            g_usb0_host_UsbDeviceSpeed = USB_HOST_HIGH_SPEED;
        } else if (stat == USB_HOST_FSMODE) {
            wk_HcRhPortStatus1 &= ~OR_RH_PORT_LSDA;
            RZA_IO_RegWrite_16(&USB200.SOFCFG, 0, USB_SOFCFG_TRNENSEL_SHIFT, USB_SOFCFG_TRNENSEL);
            g_usb0_host_UsbDeviceSpeed = USB_HOST_FULL_SPEED;
        } else {
            wk_HcRhPortStatus1 |= OR_RH_PORT_LSDA;
            RZA_IO_RegWrite_16(&USB200.SOFCFG, 1, USB_SOFCFG_TRNENSEL_SHIFT, USB_SOFCFG_TRNENSEL);
            g_usb0_host_UsbDeviceSpeed = USB_HOST_LOW_SPEED;
        }
        RZA_IO_RegWrite_16(&devadd, g_usb0_host_UsbDeviceSpeed, USB_DEVADDn_USBSPD_SHIFT, USB_DEVADDn_USBSPD);
        usb0_host_init_pipe_status();
        usb0_host_set_devadd(USB_HOST_DEVICE_0, &devadd);
        wk_HcRhPortStatus1 |= OR_RH_PORT_CCS;
    }
    wk_HcRhPortStatus1           |= OR_RH_PORT_CSC;
    p_usb_reg->HcRhPortStatus1   =  wk_HcRhPortStatus1;
    p_usb_reg->HcInterruptStatus |= OR_INTR_STATUS_RHSC;
    (void)memset(&split_ctl, 0, sizeof(split_ctl));

    if (type == 0) {
        if (ctl_ed.trans_wait == 1) {
            ohciwrapp_loc_TransEnd(ctl_ed.pipe_no, TD_CC_DEVICENOTRESPONDING);
        }
        if (blk_ed.trans_wait == 1) {
            ohciwrapp_loc_TransEnd(blk_ed.pipe_no, TD_CC_DEVICENOTRESPONDING);
        }
        for (cnt = 0; cnt< INT_TRANS_MAX_NUM; cnt++) {
            if (int_ed[cnt].trans_wait == 1) {
                ohciwrapp_loc_TransEnd(int_ed[cnt].pipe_no, TD_CC_DEVICENOTRESPONDING);
            }
        }
    }
}

void ohciwrapp_loc_Connect(uint32_t type) {
    connect_change = type;
    (void)osSemaphoreRelease(semid_cb);
}

void ohciwrapp_loc_TransEnd(uint32_t pipe, uint32_t ConditionCode) {
    uint32_t     cnt;
    uint32_t     sqmon;
    hctd_t       *p_td;
    hced_t       *p_ed;
    genelal_ed_t *p_wait_ed = NULL;

    if (ctl_ed.pipe_no == pipe) {
        p_wait_ed = &ctl_ed;
    } else if (blk_ed.pipe_no == pipe) {
        p_wait_ed = &blk_ed;
    } else {
        for (cnt = 0; cnt< INT_TRANS_MAX_NUM; cnt++) {
            if (int_ed[cnt].pipe_no == pipe) {
                p_wait_ed = &int_ed[cnt];
                break;
            }
        }
    }
    if (p_wait_ed == NULL) {
        return;
    }

    p_td  = p_wait_ed->p_curr_td;
    p_ed  = p_wait_ed->p_curr_ed;
    if ((p_td == NULL) || (p_ed == NULL)) {
        return;
    }

    if (ConditionCode == TD_CC_NOERROR) {
        /* ErrorCount */
        RZA_IO_RegWrite_32(&p_td->control, 0, TD_CTL_SHFT_EC, TD_CTL_MSK_EC);

        /* CurrentBufferPointer */
        p_td->currBufPtr += ((uint32_t)p_td->bufEnd - (uint32_t)p_td->currBufPtr + 1) - g_usb0_host_data_count[pipe];
    } else {
        /* ErrorCount */
        RZA_IO_RegWrite_32(&p_td->control, 3, TD_CTL_SHFT_EC, TD_CTL_MSK_EC);
    }

    /* DataToggle */
    sqmon = usb0_host_get_sqmon(pipe);
    RZA_IO_RegWrite_32(&p_td->control, sqmon, TD_CTL_SHFT_T, TD_CTL_MSK_T);
    if (sqmon == 0) {
        p_ed->headTD = (hctd_t *)((uint32_t)p_ed->headTD & ~ED_TOGLE_CARRY);
    } else {
        p_ed->headTD = (hctd_t *)((uint32_t)p_ed->headTD | ED_TOGLE_CARRY);
    }

    /* ConditionCode */
    RZA_IO_RegWrite_32(&p_td->control, ConditionCode, TD_CTL_SHFT_CC, TD_CTL_MSK_CC);

    p_wait_ed->trans_wait = 0;

    (void)osSemaphoreRelease(p_wait_ed->semid_wait);
}

