/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited, All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#if defined(DEVICE_USBDEVICE) && DEVICE_USBDEVICE

extern "C"
{
#include "r_typedefs.h"
#include "iodefine.h"
}
#include "USBPhyHw.h"
#include "rza_io_regrw.h"
#include "USBDevice_Types.h"
#include "USBEndpoints_RZ_A1.h"
#include "USBPhy_RZ_A1_Def.h"


/**** User Selection ****/
#define USB_FUNCTION_CH        0
#define USB_FUNCTION_HISPEED   0        // 1: High-Speed  0: Full-Speed

#if (USB_FUNCTION_CH == 0)
#define USB_MX       USB200
#define USBIX_IRQn   USBI0_IRQn
#else
#define USB_MX       USB201
#define USBIX_IRQn   USBI1_IRQn
#endif

/* There are maintenance routine of SHTNAK and BFRE bits in original sample program.
* This sample is not programmed. Do maintenance the "def_pipecfg" array if you want it. */
const struct PIPECFGREC {
    uint16_t    endpoint;
    uint16_t    pipesel;
    uint16_t    pipecfg;
    uint16_t    pipebuf;
    uint16_t    pipemaxp;
    uint16_t    pipeperi;
} def_pipecfg[] = {
    /* EP0OUT and EP0IN are configured by USB IP */
    {
        EP1OUT, /* EP1: Host -> Func, INT */
        6 | USB_FUNCTION_D0FIFO_USE,
        USB_TYPFIELD_INT  | USB_BFREOFF | USB_CFG_DBLBOFF | USB_CFG_CNTMDON  |                   USB_DIR_P_OUT | 1,
        USB_BUF_SIZE(64) | 0x04u,
        MAX_PACKET_SIZE_EP1,
        3,
    },
    {
        EP1IN,  /* EP1: Host <- Func, INT */
        7 | USB_FUNCTION_D1FIFO_USE,
        USB_TYPFIELD_INT  | USB_BFREOFF | USB_CFG_DBLBOFF | USB_CFG_CNTMDOFF |                   USB_DIR_P_IN  | 1,
        USB_BUF_SIZE(64) | 0x05u,
        MAX_PACKET_SIZE_EP1,
        3,
    },
    {
        EP2OUT, /* EP2: Host -> Func, BULK */
        3 | USB_FUNCTION_D0FIFO_USE,
        USB_TYPFIELD_BULK | USB_BFREOFF | USB_CFG_DBLBON  | USB_CFG_CNTMDON  | USB_SHTNAKFIELD | USB_DIR_P_OUT | 2,
        USB_BUF_SIZE(2048) | 0x30u,
        MAX_PACKET_SIZE_EP2,
        0,
    },
    {
        EP2IN,  /* EP2: Host <- Func, BULK */
        4 | USB_FUNCTION_D1FIFO_USE,
        USB_TYPFIELD_BULK | USB_BFREOFF | USB_CFG_DBLBOFF | USB_CFG_CNTMDON  |                   USB_DIR_P_IN  | 2,
        USB_BUF_SIZE(2048) | 0x50u,
        MAX_PACKET_SIZE_EP2,
        0,
    },
    {
        EP3OUT, /* EP3: Host -> Func, ISO */
        1 | USB_FUNCTION_D0FIFO_USE,
        USB_TYPFIELD_ISO  | USB_BFREOFF | USB_CFG_DBLBON  | USB_CFG_CNTMDOFF | USB_SHTNAKFIELD | USB_DIR_P_OUT | 3,
        USB_BUF_SIZE(512) | 0x10u,
        MAX_PACKET_SIZE_EP3,
        0,
    },
    {
        EP3IN,  /* EP3: Host <- Func, ISO */
        2 | USB_FUNCTION_D1FIFO_USE,
        USB_TYPFIELD_ISO  | USB_BFREOFF | USB_CFG_DBLBON  | USB_CFG_CNTMDOFF |                    USB_DIR_P_IN  | 3,
        USB_BUF_SIZE(512) | 0x20u,
        MAX_PACKET_SIZE_EP3,
        0,
    },
    { /* terminator */
        0, 0, 0, 0, 0, 0
    },
};

static USBPhyHw *instance;
static uint8_t _usb_speed = USB_FUNCTION_HISPEED;
static bool run_later_ctrl_comp = false;

USBPhy *get_usb_phy()
{
    static USBPhyHw usbphy;
    return &usbphy;
}

USBPhyHw::USBPhyHw(): events(NULL)
{
}

USBPhyHw::~USBPhyHw()
{
}

void USBPhyHw::init(USBPhyEvents *events)
{
    volatile uint8_t dummy_read;

    if (this->events == NULL) {
        sleep_manager_lock_deep_sleep();
    }
    this->events = events;

    /* registers me */
    instance = this;

    /* Disable IRQ */
    GIC_DisableIRQ(USBIX_IRQn);

#if (USB_FUNCTION_CH == 0)
    CPG.STBCR7 &= ~(CPG_STBCR7_MSTP71);
#else
    CPG.STBCR7 &= ~(CPG_STBCR7_MSTP71 | CPG_STBCR7_MSTP70);
#endif
    dummy_read = CPG.STBCR7;
    (void)dummy_read;

    /* module reset and clock select */
    reset_usb(USB_X1_48MHZ);                            /* USB_X1 48MHz */

    /* Set to USB Function and select speed */
    USB_MX.SYSCFG0 &= ~USB_DPRPU;
    USB_MX.SYSCFG0 &= ~USB_DRPD;
    USB_MX.SYSCFG0 &= ~USB_DCFM;                        /* USB Functoin */
    USB_MX.SYSCFG0 |= USB_USBE;

    if (_usb_speed == 0) {
        USB_MX.SYSCFG0 &= ~USB_HSE;                     /* Full-Speed */
    } else {
        USB_MX.SYSCFG0 |= USB_HSE;                      /* High-Speed */
    }
    cpu_delay_1us(1500);
}

void USBPhyHw::deinit()
{
    volatile uint8_t dummy_read;

    disconnect();

#if (USB_FUNCTION_CH == 0)
    CPG.STBCR7 |= CPG_STBCR7_MSTP71;
#else
    CPG.STBCR7 |= (CPG_STBCR7_MSTP71 | CPG_STBCR7_MSTP70);
#endif
    dummy_read = CPG.STBCR7;
    (void)dummy_read;

    if (events != NULL) {
        sleep_manager_unlock_deep_sleep();
    }
    events = NULL;
}

bool USBPhyHw::powered()
{
    // return true if powered false otherwise. Devices which don't support
    //    this should always return true
    return true;
}

void USBPhyHw::connect()
{
    /* Enable pullup on D+ */
    USB_MX.INTENB0 |= (USB_VBSE | USB_SOFE | USB_DVSE | USB_CTRE | USB_BEMPE | USB_NRDYE | USB_BRDYE);
    USB_MX.SYSCFG0 |= USB_DPRPU;

    /* Enable USB */
    InterruptHandlerRegister(USBIX_IRQn, &_usbisr);
    GIC_SetPriority(USBIX_IRQn, 16);
    GIC_SetConfiguration(USBIX_IRQn, 1);
    GIC_EnableIRQ(USBIX_IRQn);
}

void USBPhyHw::disconnect()
{
    /* Disable USB */
    GIC_DisableIRQ(USBIX_IRQn);
    InterruptHandlerRegister(USBIX_IRQn, NULL);

    /* Disable pullup on D+ */
    USB_MX.SYSCFG0 &= ~USB_DPRPU;
    cpu_delay_1us(1);
    USB_MX.SYSCFG0 |= USB_DCFM;
    cpu_delay_1us(1);
    USB_MX.SYSCFG0 &= ~USB_DCFM;
}

void USBPhyHw::configure()
{
}

void USBPhyHw::unconfigure()
{
}

void USBPhyHw::sof_enable()
{
    /* Enable SOF interrupt */
    USB_MX.INTENB0 |= USB_SOFE;
}

void USBPhyHw::sof_disable()
{
    /* Disable SOF interrupt */
    USB_MX.INTENB0 &= ~USB_SOFE;
}

void USBPhyHw::set_address(uint8_t address)
{
    if (address <= 127) {
        set_pid(USB_PIPE0, USB_PID_BUF);        /* Set BUF */
    } else {
        set_pid(USB_PIPE0, USB_PID_STALL);      /* Not specification */
    }
}

void USBPhyHw::remote_wakeup()
{
}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t rza1_table = {
        1, // No cost per endpoint - everything allocated up front
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_INT  | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_ISO  | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
        }
    };
    return &rza1_table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    return MAX_PACKET_SIZE_EP0;
}

void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    memcpy(buffer, setup_buffer, size);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    pipe_ctrl[USB_PIPE0].req_size  = size;
    pipe_ctrl[USB_PIPE0].data_cnt  = size;
    pipe_ctrl[USB_PIPE0].p_data    = data;

    chg_curpipe(USB_PIPE0, USB_ISEL_READ);      /* Switch FIFO and pipe number. */
    USB_MX.CFIFOCTR = USB_BCLR;                 /* Buffer clear */
    set_pid(USB_PIPE0, USB_PID_BUF);            /* Set BUF */
    USB_MX.BRDYENB |= (1 << USB_PIPE0);         /* Enable ready interrupt */
    USB_MX.NRDYENB |= (1 << USB_PIPE0);         /* Enable not ready interrupt */
}

uint32_t USBPhyHw::ep0_read_result()
{
    return pipe_ctrl[USB_PIPE0].req_size;
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    if ((buffer == NULL) || (size == 0)) {
        set_pid(USB_PIPE0, USB_PID_BUF);            /* Set BUF */
        return;
    }

    pipe_ctrl[USB_PIPE0].req_size  = size;
    pipe_ctrl[USB_PIPE0].data_cnt  = size;
    pipe_ctrl[USB_PIPE0].p_data    = buffer;

    chg_curpipe(USB_PIPE0, USB_ISEL_WRITE);         /* Switch FIFO and pipe number. */
    USB_MX.CFIFOCTR = USB_BCLR;                     /* Buffer clear */
    /* Clear the PIPExBEMP status bit of the specified pipe to clear */
    USB_MX.BEMPSTS = (uint16_t)((~(1 << USB_PIPE0)) & BEMPSTS_MASK);

    /* Peripheral control sequence */
    switch (write_data(USB_PIPE0)) {
        case USB_WRITING :                          /* Continue of data write */
            USB_MX.BRDYENB |= (1 << USB_PIPE0);     /* Enable Ready interrupt */
            USB_MX.NRDYENB |= (1 << USB_PIPE0);     /* Enable Not Ready Interrupt */
            set_pid(USB_PIPE0, USB_PID_BUF);
            break;
        case USB_WRITEEND :                         /* End of data write */
        case USB_WRITESHRT :                        /* End of data write */
            USB_MX.BEMPENB |= (1 << USB_PIPE0);     /* Enable Empty Interrupt */
            USB_MX.NRDYENB |= (1 << USB_PIPE0);     /* Enable Not Ready Interrupt */
            set_pid(USB_PIPE0, USB_PID_BUF);
            break;
        case USB_FIFOERROR :                        /* FIFO access error */
            ctrl_end((uint16_t)USB_DATA_ERR);
            break;
        default :
            break;
    }
}

void USBPhyHw::ep0_stall()
{
    set_pid(USB_PIPE0, USB_PID_STALL);
    run_later_ctrl_comp = false;
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    const struct PIPECFGREC *cfg;
    uint16_t pipe;
    volatile uint16_t *p_reg;

    if ((endpoint == EP0OUT) || (endpoint == EP0IN)) {
        return true;
    }

    for (cfg = &def_pipecfg[0]; cfg->pipesel != 0; cfg++) {
        if (cfg->endpoint == endpoint) {
            break;
        }
    }
    if (cfg->pipesel == 0) {
        return false;
    }

    pipe = (cfg->pipesel & USB_CURPIPE);

    /* Interrupt Disable */
    USB_MX.BRDYENB &= (~(1 << pipe));   /* Disable Ready Interrupt */
    USB_MX.NRDYENB &= (~(1 << pipe));   /* Disable Not Ready Interrupt */
    USB_MX.BEMPENB &= (~(1 << pipe));   /* Disable Empty Interrupt */

    set_pid(pipe, USB_PID_NAK);

    /* CurrentPIPE Clear */
    if ((USB_MX.CFIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX.CFIFOSEL &= ~USB_CURPIPE;
    }
    if ((USB_MX.D0FIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX.D0FIFOSEL &= ~USB_CURPIPE;
    }
    if ((USB_MX.D1FIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX.D1FIFOSEL &= ~USB_CURPIPE;
    }

    /* PIPE Configuration */
    USB_MX.PIPESEL  = pipe;             /* Pipe select */
    USB_MX.PIPECFG  = cfg->pipecfg;
    USB_MX.PIPEBUF  = cfg->pipebuf;
    USB_MX.PIPEMAXP = cfg->pipemaxp;
    USB_MX.PIPEPERI = cfg->pipeperi;

    p_reg = get_pipectr_reg(pipe);
    /* Set toggle bit to DATA0 */
    *p_reg |= USB_SQCLR;
    /* Buffer Clear */
    *p_reg |= USB_ACLRM;
    *p_reg &= ~USB_ACLRM;

    return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    uint16_t pipe = EP2PIPE(endpoint);

    /* Interrupt Disable */
    USB_MX.BRDYENB &= (~(1 << pipe));   /* Disable Ready Interrupt */
    USB_MX.NRDYENB &= (~(1 << pipe));   /* Disable Not Ready Interrupt */
    USB_MX.BEMPENB &= (~(1 << pipe));   /* Disable Empty Interrupt */

    set_pid(pipe, USB_PID_NAK);

    /* CurrentPIPE Clear */
    if ((USB_MX.CFIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX.CFIFOSEL &= ~USB_CURPIPE;
    }
    if ((USB_MX.D0FIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX.D0FIFOSEL &= ~USB_CURPIPE;
    }
    if ((USB_MX.D1FIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX.D1FIFOSEL &= ~USB_CURPIPE;
    }

    /* PIPE Configuration */
    USB_MX.PIPESEL = pipe;              /* Pipe select */
    USB_MX.PIPECFG = 0;

    pipe_ctrl[pipe].enable = false;
    pipe_ctrl[pipe].status = USB_DATA_NONE;
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    uint16_t pipe = EP2PIPE(endpoint);

    set_pid(pipe, USB_PID_STALL);

    pipe_ctrl[pipe].enable = false;
    pipe_ctrl[pipe].status = USB_DATA_STALL;
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    uint16_t pipe = EP2PIPE(endpoint);
    volatile uint16_t *p_reg;

    set_pid(pipe, USB_PID_NAK);

    p_reg = get_pipectr_reg(pipe);
    /* Set toggle bit to DATA0 */
    *p_reg |= USB_SQCLR;
    /* Buffer Clear */
    *p_reg |= USB_ACLRM;
    *p_reg &= ~USB_ACLRM;

    pipe_ctrl[pipe].enable = false;
    pipe_ctrl[pipe].status = USB_DATA_NONE;
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    uint16_t mxps;
    uint16_t trncnt;
    volatile uint16_t *p_reg;
    uint16_t pipe = EP2PIPE(endpoint);

    if (pipe_ctrl[pipe].status == USB_DATA_STALL) {
        return false;
    }

    pipe_ctrl[pipe].status   = USB_DATA_READING;
    pipe_ctrl[pipe].req_size = size;
    pipe_ctrl[pipe].data_cnt = size;
    pipe_ctrl[pipe].p_data   = data;
    pipe_ctrl[pipe].enable   = true;

    set_pid(pipe, USB_PID_NAK);                                     /* Set NAK */

    USB_MX.BEMPSTS = (uint16_t)((~(1 << pipe)) & BEMPSTS_MASK);     /* BEMP Status Clear */
    USB_MX.BRDYSTS = (uint16_t)((~(1 << pipe)) & BRDYSTS_MASK);     /* BRDY Status Clear */
    USB_MX.NRDYSTS = (uint16_t)((~(1 << pipe)) & NRDYSTS_MASK);     /* NRDY Status Clear */

    chg_curpipe(pipe, USB_ISEL_READ);                               /* Switch FIFO and pipe number. */
    p_reg = get_fifoctr_reg(pipe);
    *p_reg = USB_BCLR;                                              /* Clear BCLR */

    if (size != 0) {
        /* Max Packet Size */
        USB_MX.PIPESEL = pipe;                                          /* Pipe select */
        mxps = (uint16_t)(USB_MX.PIPEMAXP & USB_MXPS);
        /* Data size check */
        if ((size % mxps) == (uint32_t)0u) {
            trncnt = (uint16_t)(size / mxps);
        } else {
            trncnt = (uint16_t)((size / mxps) + (uint32_t)1u);
        }

        /* Set Transaction counter */
        p_reg = get_pipetre_reg(pipe);
        if (p_reg != NULL) {
            *p_reg |= USB_TRCLR;
        }
        p_reg = get_pipetrn_reg(pipe);
        if (p_reg != NULL) {
            *p_reg = trncnt;
        }
        p_reg = get_pipetre_reg(pipe);
        if (p_reg != NULL) {
            *p_reg |= USB_TRENB;
        }

        p_reg = get_pipectr_reg(pipe);
        /* Buffer Clear */
        *p_reg |= USB_ACLRM;
        *p_reg &= ~USB_ACLRM;
    }

    set_pid(pipe, USB_PID_BUF);                                     /* Set BUF */
    USB_MX.BRDYENB |= (1 << pipe);                                  /* Enable Ready Interrupt */
    USB_MX.NRDYENB |= (1 << pipe);                                  /* Enable Not Ready Interrupt */

    return true;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint16_t pipe = EP2PIPE(endpoint);

    return pipe_ctrl[pipe].req_size;
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    volatile uint16_t *p_reg;
    uint16_t pipe = EP2PIPE(endpoint);

    if (pipe_ctrl[pipe].status == USB_DATA_STALL) {
        return false;
    }

    pipe_ctrl[pipe].status   = USB_DATA_WRITING;
    pipe_ctrl[pipe].req_size = size;
    pipe_ctrl[pipe].data_cnt = size;
    pipe_ctrl[pipe].p_data   = data;
    pipe_ctrl[pipe].enable   = true;

    set_pid(pipe, USB_PID_NAK);                                     /* Set NAK */

    USB_MX.BEMPSTS = (uint16_t)((~(1 << pipe)) & BEMPSTS_MASK);     /* BEMP Status Clear */
    USB_MX.BRDYSTS = (uint16_t)((~(1 << pipe)) & BRDYSTS_MASK);     /* BRDY Status Clear */
    USB_MX.NRDYSTS = (uint16_t)((~(1 << pipe)) & NRDYSTS_MASK);     /* NRDY Status Clear */

    p_reg = get_pipectr_reg(pipe);
    /* Buffer Clear */
    *p_reg |= USB_ACLRM;
    *p_reg &= ~USB_ACLRM;

    buf_to_fifo(pipe);                                              /* Buffer to FIFO data write */
    set_pid(pipe, USB_PID_BUF);                                     /* Set BUF */

    return true;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    forced_termination(EP2PIPE(endpoint), (uint16_t)USB_DATA_NONE);
}

void USBPhyHw::process()
{
    /* Register Save */
    uint16_t intsts0 = USB_MX.INTSTS0;
    uint16_t brdysts = USB_MX.BRDYSTS;
    uint16_t nrdysts = USB_MX.NRDYSTS;
    uint16_t bempsts = USB_MX.BEMPSTS;
    uint16_t intenb0 = USB_MX.INTENB0;
    uint16_t brdyenb = USB_MX.BRDYENB;
    uint16_t nrdyenb = USB_MX.NRDYENB;
    uint16_t bempenb = USB_MX.BEMPENB;

    /* Interrupt status get */
    uint16_t ists0 = (uint16_t)(intsts0 & intenb0);
    uint16_t bsts  = (uint16_t)(brdysts & brdyenb);
    uint16_t nsts  = (uint16_t)(nrdysts & nrdyenb);
    uint16_t ests  = (uint16_t)(bempsts & bempenb);

    uint16_t i;

    if ((intsts0 & (USB_VBINT | USB_RESM | USB_SOFR | USB_DVST |
                    USB_CTRT | USB_BEMP | USB_NRDY | USB_BRDY)) == 0u) {
        return;
    }

    /***** Processing USB bus signal *****/
    /***** Resume signal *****/
    if ((ists0 & USB_RESM) == USB_RESM) {
        USB_MX.INTSTS0 = (uint16_t)~USB_RESM;
        USB_MX.INTENB0 &= (~USB_RSME);        /* RESM interrupt disable */
        events->suspend(true);
    }

    /***** Vbus change *****/
    else if ((ists0 & USB_VBINT) == USB_VBINT) {
        USB_MX.INTSTS0 = (uint16_t)~USB_VBINT;
        if (chk_vbsts()) {
            /* USB attach */
            /* Non processing. */
        } else {
            /* USB detach */
            for (i = USB_MIN_PIPE_NO; i < PIPE_NUM; i++) {
                if (pipe_ctrl[i].enable) {
                    forced_termination(i, (uint16_t)USB_DATA_NONE);
                }
            }
            USB_MX.INTSTS0 = 0;
            USB_MX.BRDYSTS = 0;
            USB_MX.NRDYSTS = 0;
            USB_MX.BEMPSTS = 0;
            USB_MX.BRDYENB = 0;
            USB_MX.NRDYENB = 0;
            USB_MX.BEMPENB = 0;
        }
    }

    /***** SOFR change *****/
    else if ((ists0 & USB_SOFR) == USB_SOFR) {
        USB_MX.INTSTS0 = (uint16_t)~USB_SOFR;
        events->sof(USB_MX.FRMNUM & USB_FRNM);
    }

    /***** Processing device state *****/
    /***** DVST change *****/
    else if ((ists0 & USB_DVST) == USB_DVST) {
        USB_MX.INTSTS0 = (uint16_t)~USB_DVST;

        switch ((uint16_t)(intsts0 & USB_DVSQ)) {
            case USB_DS_POWR :
                break;
            case USB_DS_DFLT :
                USB_MX.DCPCFG = 0;                    /* DCP configuration register  (0x5C) */
                USB_MX.DCPMAXP = MAX_PACKET_SIZE_EP0; /* DCP maxpacket size register (0x5E) */

                events->reset();
                break;
            case USB_DS_ADDS :
                break;
            case USB_DS_CNFG :
                break;
            case USB_DS_SPD_POWR :
            case USB_DS_SPD_DFLT :
            case USB_DS_SPD_ADDR :
            case USB_DS_SPD_CNFG :
                events->suspend(false);
                break;
            default :
                break;
        }
    }

    /***** Processing PIPE0 data *****/
    else if (((ists0 & USB_BRDY) == USB_BRDY) && ((bsts & USB_BRDY0) == USB_BRDY0)) {
        /* ==== BRDY PIPE0 ==== */
        USB_MX.BRDYSTS = (uint16_t)((~USB_BRDY0) & BRDYSTS_MASK);

        /* When operating by the peripheral function, usb_brdy_pipe() is executed with PIPEx request because */
        /* two BRDY messages are issued even when the demand of PIPE0 and PIPEx has been generated at the same time. */
        if ((USB_MX.CFIFOSEL & USB_ISEL_WRITE) == USB_ISEL_WRITE) {
            switch (write_data(USB_PIPE0)) {
                case USB_WRITEEND :
                case USB_WRITESHRT :
                    USB_MX.BRDYENB &= (~(1 << USB_PIPE0));
                    break;
                case USB_WRITING :
                    set_pid(USB_PIPE0, USB_PID_BUF);
                    break;
                case USB_FIFOERROR :
                    ctrl_end((uint16_t)USB_DATA_ERR);
                    break;
                default :
                    break;
            }
            events->ep0_in();
        } else {
            switch (read_data(USB_PIPE0)) {
                case USB_READEND :
                case USB_READSHRT :
                    USB_MX.BRDYENB &= (~(1 << USB_PIPE0));
                    pipe_ctrl[USB_PIPE0].req_size -= pipe_ctrl[USB_PIPE0].data_cnt;
                    break;
                case USB_READING :
                    set_pid(USB_PIPE0, USB_PID_BUF);
                    break;
                case USB_READOVER :
                    ctrl_end((uint16_t)USB_DATA_OVR);
                    pipe_ctrl[USB_PIPE0].req_size -= pipe_ctrl[USB_PIPE0].data_cnt;
                    break;
                case USB_FIFOERROR :
                    ctrl_end((uint16_t)USB_DATA_ERR);
                    break;
                default :
                    break;
            }
            events->ep0_out();
        }
    } else if (((ists0 & USB_BEMP) == USB_BEMP) && ((ests & USB_BEMP0) == USB_BEMP0)) {
        /* ==== BEMP PIPE0 ==== */
        USB_MX.BEMPSTS = (uint16_t)((~USB_BEMP0) & BEMPSTS_MASK);

        events->ep0_in();
    } else if (((ists0 & USB_NRDY) == USB_NRDY) && ((nsts & USB_NRDY0) == USB_NRDY0)) {
        /* ==== NRDY PIPE0 ==== */
        USB_MX.NRDYSTS = (uint16_t)((~USB_NRDY0) & NRDYSTS_MASK);
        /* Non processing. */
    }

    /***** Processing setup transaction *****/
    else if ((ists0 & USB_CTRT) == USB_CTRT) {
        USB_MX.INTSTS0 = (uint16_t)~USB_CTRT;

        /* CTSQ bit changes later than CTRT bit for ASSP. */
        /* CTSQ reloading */
        uint16_t stginfo = (uint16_t)(intsts0 & USB_CTSQ);
        if (stginfo != USB_CS_IDST) {
            if (((USB_CS_RDDS == stginfo) || (USB_CS_WRDS == stginfo)) || (USB_CS_WRND == stginfo)) {
                /* Save request register */
                uint16_t *bufO = &setup_buffer[0];

                USB_MX.INTSTS0 = (uint16_t)~USB_VALID;
                *bufO++ = USB_MX.USBREQ;   /* data[0] <= bmRequest, data[1] <= bmRequestType */
                *bufO++ = USB_MX.USBVAL;   /* data[2] data[3] <= wValue */
                *bufO++ = USB_MX.USBINDX;  /* data[4] data[5] <= wIndex */
                *bufO++ = USB_MX.USBLENG;  /* data[6] data[7] <= wLength */
            }
        }

        /* Switch on the control transfer stage (CTSQ). */
        switch (stginfo) {
            case USB_CS_IDST :  /* Idle or setup stage */
                break;
            case USB_CS_RDDS :  /* Control read data stage */
                events->ep0_setup();
                break;
            case USB_CS_WRDS :  /* Control write data stage */
                events->ep0_setup();
                break;
            case USB_CS_WRND :  /* Status stage of a control write where there is no data stage. */
                events->ep0_setup();
                run_later_ctrl_comp = true;
                break;
            case USB_CS_RDSS :  /* Control read status stage */
                USB_MX.DCPCTR |= USB_CCPL;
                break;
            case USB_CS_WRSS :  /* Control write status stage */
                USB_MX.DCPCTR |= USB_CCPL;
                break;
            case USB_CS_SQER :  /* Control sequence error */
            default :           /* Illegal */
                ctrl_end((uint16_t)USB_DATA_ERR);
                break;
        }
    }

    /***** Processing PIPE1-MAX_PIPE_NO data *****/
    else if ((ists0 & USB_BRDY) == USB_BRDY) {
        /* ==== BRDY PIPEx ==== */
        USB_MX.BRDYSTS = (uint16_t)((~bsts) & BRDYSTS_MASK);

        for (i = USB_MIN_PIPE_NO; i < PIPE_NUM; i++) {
            if ((bsts & USB_BITSET(i)) != 0u) {
                /* Interrupt check */
                if (pipe_ctrl[i].enable) {
                    USB_MX.PIPESEL = i;
                    if (USB_BUF2FIFO == (uint16_t)(USB_MX.PIPECFG & USB_DIRFIELD)) {
                        /* write */
                        buf_to_fifo(i);         /* Buffer to FIFO data write */
                        events->in(PIPE2EP(i));
                    } else {
                        /* read */
                        fifo_to_buf(i);         /* FIFO to Buffer data read */
                        events->out(PIPE2EP(i));
                    }
                }
            }
        }
    } else if ((ists0 & USB_BEMP) == USB_BEMP) {
        /* ==== BEMP PIPEx ==== */
        USB_MX.BEMPSTS = (uint16_t)((~ests) & BEMPSTS_MASK);

        for (i = USB_MIN_PIPE_NO; i < PIPE_NUM; i++) {
            if ((ests & USB_BITSET(i)) != 0) {
                /* Interrupt check */
                if (pipe_ctrl[i].enable) {
                    /* MAX packet size error ? */
                    if (((get_pid(i) & USB_PID_STALL) == USB_PID_STALL) || ((get_pid(i) & USB_PID_STALL2) == USB_PID_STALL2)) {
                        forced_termination(i, (uint16_t)USB_DATA_STALL);
                    } else {
                        if ((i >= USB_PIPE6) || ((*get_pipectr_reg(i) & USB_INBUFM) != USB_INBUFM)) {
                            data_end(i, (uint16_t)USB_DATA_NONE);       /* End of data transfer */
                        } else {
                            USB_MX.BEMPENB |= (1 << i);
                        }
                    }
                    events->in(PIPE2EP(i));
                }
            }
        }
    } else if ((ists0 & USB_NRDY) == USB_NRDY) {
        /* ==== NRDY PIPEx ==== */
        USB_MX.NRDYSTS = (uint16_t)((~nsts) & NRDYSTS_MASK);

        for (i = USB_MIN_PIPE_NO; i < PIPE_NUM; i++) {
            if ((nsts & USB_BITSET(i)) != 0) {
                /* Interrupt check */
                if (pipe_ctrl[i].enable) {
                    if (((get_pid(i) & USB_PID_STALL) != USB_PID_STALL) && ((get_pid(i) & USB_PID_STALL2) != USB_PID_STALL2)) {
                        set_pid(i, USB_PID_BUF);
                    }
                }
            }
        }
    } else {
        /* Non processing. */
    }
}

void USBPhyHw::_usbisr(void)
{
    GIC_DisableIRQ(USBIX_IRQn);

    run_later_ctrl_comp = false;

    instance->events->start_process();

    if (run_later_ctrl_comp) {
        USB_MX.DCPCTR &= (~USB_PID);
        USB_MX.DCPCTR |= USB_PID_BUF;

        USB_MX.DCPCTR |= USB_CCPL;
    }

    /* Re-enable interrupt */
    GIC_ClearPendingIRQ(USBIX_IRQn);
    GIC_EnableIRQ(USBIX_IRQn);
}

void USBPhyHw::chg_curpipe(uint16_t pipe, uint16_t isel)
{
    volatile uint16_t *p_reg;
    uint16_t fifo_use;
    uint16_t isel_val;
    uint16_t buf;

    fifo_use = PIPE2FIFO(pipe);
    if ((fifo_use == USB_FUNCTION_D0FIFO_USE) || (fifo_use == USB_FUNCTION_D1FIFO_USE)) {
        isel_val = 0;
    } else {
        isel_val = isel;
    }

    p_reg = get_fifosel_reg(pipe);
    buf  = *p_reg;
    buf &= (uint16_t)(~(USB_RCNT | USB_ISEL | USB_CURPIPE | USB_MBW));
    buf |= (uint16_t)((USB_RCNT | isel_val | pipe | USB_MBW_32) & (USB_RCNT | USB_ISEL | USB_CURPIPE | USB_MBW));
    *p_reg = buf;

    do {
        cpu_delay_1us(1);
        buf = *p_reg;
    } while ((buf & (uint16_t)(USB_ISEL | USB_CURPIPE)) != (uint16_t)(isel_val | pipe));
}

uint16_t USBPhyHw::is_set_frdy(uint16_t pipe, uint16_t isel)
{
    volatile uint16_t *p_reg;
    uint16_t buffer;
    int retry_cnt = 0;

    chg_curpipe(pipe, isel);                    /* Changes the FIFO port by the pipe. */
    p_reg = get_fifoctr_reg(pipe);
    for (retry_cnt = 0; retry_cnt < 10; retry_cnt++) {
        buffer = *p_reg;
        if ((uint16_t)(buffer & USB_FRDY) == USB_FRDY) {
            return (buffer);
        }
        cpu_delay_1us(1);
    }

    return (USB_FIFOERROR);
}

uint8_t *USBPhyHw::read_fifo(uint16_t pipe, uint16_t count, uint8_t *read_p)
{
    iodefine_reg32_t *p_reg;
    uint16_t even;
    uint16_t odd;
    uint32_t odd_byte_data_temp;

    p_reg = (iodefine_reg32_t *)get_fifo_reg(pipe);
    for (even = (uint16_t)(count >> 2); (even != 0); --even) {
        /* 32bit FIFO access */
        *((uint32_t *)read_p) = p_reg->UINT32;
        read_p += sizeof(uint32_t);
    }
    odd = count % 4;
    if (count < 4) {
        odd = count;
    }
    if (odd != 0) {
        /* 32bit FIFO access */
        odd_byte_data_temp = p_reg->UINT32;
        /* Condition compilation by the difference of the endian */
        do {
            *read_p = (uint8_t)(odd_byte_data_temp & 0x000000ff);
            odd_byte_data_temp = odd_byte_data_temp >> 8;
            /* Renewal read pointer */
            read_p += sizeof(uint8_t);
            odd--;
        } while (odd != 0);
    }

    return read_p;
}

uint16_t USBPhyHw::read_data(uint16_t pipe)
{
    volatile uint16_t *p_reg;
    uint16_t count;
    uint16_t buffer;
    uint16_t mxps;
    uint16_t dtln;
    uint16_t end_flag;

    /* Changes FIFO port by the pipe. */
    buffer = is_set_frdy(pipe, 0);
    if (buffer == USB_FIFOERROR) {
        return (USB_FIFOERROR);                 /* FIFO access error */
    }
    dtln = (uint16_t)(buffer & USB_DTLN);

    /* Max Packet Size */
    if (pipe == USB_PIPE0) {
        mxps = (uint16_t)(USB_MX.DCPMAXP & USB_MAXP);
    } else {
        USB_MX.PIPESEL = pipe;                  /* Pipe select */
        mxps = (uint16_t)(USB_MX.PIPEMAXP & USB_MXPS);
    }

    if (pipe_ctrl[pipe].data_cnt < dtln) {
        /* Buffer Over ? */
        end_flag = USB_READOVER;
        set_pid(pipe, USB_PID_NAK);             /* Set NAK */
        count = (uint16_t)pipe_ctrl[pipe].data_cnt;
        pipe_ctrl[pipe].data_cnt = dtln;
    } else if (pipe_ctrl[pipe].data_cnt == dtln) {
        /* Just Receive Size */
        count = dtln;
        if ((count == 0) || ((dtln % mxps) != 0)) {
            /* Just Receive Size */
            /* Peripheral Function */
            end_flag = USB_READSHRT;
        } else {
            end_flag = USB_READEND;
            set_pid(pipe, USB_PID_NAK);         /* Set NAK */
        }
    } else {
        /* Continuous Receive data */
        count = dtln;
        end_flag = USB_READING;
        if (count == 0) {
            /* Null Packet receive */
            end_flag = USB_READSHRT;
            set_pid(pipe, USB_PID_NAK);         /* Set NAK */
        }
        if ((count % mxps) != 0) {
            /* Null Packet receive */
            end_flag = USB_READSHRT;
            set_pid(pipe, USB_PID_NAK);         /* Set NAK */
        }
    }

    if (dtln == 0) { /* 0 length packet */
        p_reg = get_fifoctr_reg(pipe);
        *p_reg = USB_BCLR;                      /* Clear BCLR */
    } else {
        pipe_ctrl[pipe].p_data = read_fifo(pipe, count, pipe_ctrl[pipe].p_data);
    }
    pipe_ctrl[pipe].data_cnt -= count;

    return end_flag;
}

void USBPhyHw::fifo_to_buf(uint16_t pipe)
{
    /* Check FIFO access sequence */
    switch (read_data(pipe)) {
        case USB_READING :                                      /* Continue of data read */
            break;
        case USB_READEND :                                      /* End of data read */
            data_end(pipe, (uint16_t)USB_DATA_OK);
            pipe_ctrl[pipe].req_size -= pipe_ctrl[pipe].data_cnt;
            break;
        case USB_READSHRT :                                     /* End of data read */
            data_end(pipe, (uint16_t)USB_DATA_SHT);
            pipe_ctrl[pipe].req_size -= pipe_ctrl[pipe].data_cnt;
            break;
        case USB_READOVER :                                     /* Buffer over */
            forced_termination(pipe, (uint16_t)USB_DATA_OVR);
            pipe_ctrl[pipe].req_size -= pipe_ctrl[pipe].data_cnt;
            break;
        case USB_FIFOERROR :                                    /* FIFO access error */
        default:
            forced_termination(pipe, (uint16_t)USB_DATA_ERR);
            break;
    }
}

uint8_t *USBPhyHw::write_fifo(uint16_t pipe, uint16_t count, uint8_t *write_p)
{
    iodefine_reg32_t *p_reg;
    uint16_t even;
    uint16_t odd;

    p_reg = (iodefine_reg32_t *)get_fifo_reg(pipe);
    set_mbw(pipe, USB_MBW_32);                                /* 32bit access */
    for (even = (uint16_t)(count >> 2); (even != 0); --even) {
        p_reg->UINT32 = *((uint32_t *)write_p);
        write_p += sizeof(uint32_t);
    }
    odd = count % 4;
    if (count < 4) {
        odd = count;
    }
    if ((odd & (uint16_t)0x0002u) != 0u) {
        set_mbw(pipe, USB_MBW_16);                            /* 16bit access */
        p_reg->UINT16[H] = *((uint16_t *)write_p);
        write_p += sizeof(uint16_t);
    }
    if ((odd & (uint16_t)0x0001u) != 0u) {
        set_mbw(pipe, USB_MBW_8);                             /* 8bit access */
        p_reg->UINT8[HH] = *write_p;
        write_p++;
    }

    return write_p;
}

uint16_t USBPhyHw::write_data(uint16_t pipe)
{
    volatile uint16_t *p_reg;
    uint16_t size;
    uint16_t count;
    uint16_t mxps;
    uint16_t end_flag;
    uint16_t buffer;

    /* Changes FIFO port by the pipe. */
    if (pipe == USB_PIPE0) {
        buffer = is_set_frdy(pipe, USB_ISEL_WRITE);
    } else {
        buffer = is_set_frdy(pipe, 0);
    }

    if (buffer == USB_FIFOERROR) {
        return (USB_FIFOERROR);
    }

    if (pipe == USB_PIPE0) {
        /* Max Packet Size */
        mxps = (uint16_t)(USB_MX.DCPMAXP & USB_MAXP);

        /* Data buffer size */
        if ((USB_MX.DCPCFG & USB_CNTMDFIELD) == USB_CFG_CNTMDON) {
            size = USB_PIPE0BUF;
        } else {
            size = mxps;
        }
    } else {
        /* Max Packet Size */
        USB_MX.PIPESEL = pipe;                          /* Pipe select */
        mxps = (uint16_t)(USB_MX.PIPEMAXP & USB_MXPS);

        /* Data buffer size */
        if ((USB_MX.PIPECFG & USB_CNTMDFIELD) == USB_CFG_CNTMDON) {
            size = (uint16_t)((uint16_t)((USB_MX.PIPEBUF >> USB_BUFSIZE_BIT) + 1) * USB_PIPEXBUF);
        } else {
            size = mxps;
        }
    }

    /* Data size check */
    if (pipe_ctrl[pipe].data_cnt <= (uint32_t)size) {
        count = (uint16_t)pipe_ctrl[pipe].data_cnt;
        if (count == 0) {
            end_flag = USB_WRITESHRT;                   /* Null Packet is end of write */
        } else if ((count % mxps) != 0) {
            end_flag = USB_WRITESHRT;                   /* Short Packet is end of write */
        } else {
            end_flag = USB_WRITEEND;                    /* Just Send Size */
        }
    } else {
        /* Write continues */
        end_flag = USB_WRITING;
        count = size;
    }

    pipe_ctrl[pipe].p_data = write_fifo(pipe, count, pipe_ctrl[pipe].p_data);

    /* Check data count to remain */
    if (pipe_ctrl[pipe].data_cnt < (uint32_t)size) {
        pipe_ctrl[pipe].data_cnt = 0u;                  /* Clear data count */

        p_reg = get_fifoctr_reg(pipe);
        if ((*p_reg & USB_BVAL) == 0u) {                /* Check BVAL */
            *p_reg |= USB_BVAL;                         /* Short Packet */
        }
    } else {
        pipe_ctrl[pipe].data_cnt -= count;              /* Total data count - count */
    }

    return end_flag;
}

void USBPhyHw::buf_to_fifo(uint16_t pipe)
{
    /* Disable Ready Interrupt */
    USB_MX.BRDYENB &= (~(1 << pipe));

    /* Peripheral control sequence */
    switch (write_data(pipe)) {
        case USB_WRITING :                          /* Continue of data write */
            USB_MX.BRDYENB |= (1 << pipe);          /* Enable Ready Interrupt */
            USB_MX.NRDYENB |= (1 << pipe);          /* Enable Not Ready Interrupt */
            break;
        case USB_WRITEEND :                         /* End of data write */
        case USB_WRITESHRT :                        /* End of data write */
            USB_MX.BEMPENB |= (1 << pipe);          /* Enable Empty Interrupt */
            USB_MX.NRDYENB |= (1 << pipe);          /* Enable Not Ready Interrupt */
            break;
        case USB_FIFOERROR :                        /* FIFO access error */
        default:
            forced_termination(pipe, (uint16_t)USB_DATA_ERR);
            break;
    }
}

uint16_t *USBPhyHw::get_pipectr_reg(uint16_t pipe)
{
    if (pipe == USB_PIPE0) {
        return (uint16_t *) & (USB_MX.DCPCTR);
    } else {
        return (uint16_t *) & (USB_MX.PIPE1CTR) + (pipe - USB_PIPE1);
    }
}

uint16_t *USBPhyHw::get_pipetre_reg(uint16_t pipe)
{
    if ((pipe >= USB_PIPE1) && (pipe <= USB_PIPE5)) {
        return (uint16_t *) & (USB_MX.PIPE1TRE) + ((pipe - USB_PIPE1) * 2);
    } else if ((pipe >= USB_PIPE9) && (pipe <= USB_PIPE10)) {
        return (uint16_t *) & (USB_MX.PIPE9TRE) + ((pipe - USB_PIPE9) * 2);
    } else if ((pipe >= USB_PIPE11) && (pipe <= USB_PIPE15)) {
        return (uint16_t *) & (USB_MX.PIPEBTRE) + ((pipe - USB_PIPE11) * 2);
    } else {
        return NULL;
    }
}

uint16_t *USBPhyHw::get_pipetrn_reg(uint16_t pipe)
{
    if ((pipe >= USB_PIPE1) && (pipe <= USB_PIPE5)) {
        return (uint16_t *) & (USB_MX.PIPE1TRN) + ((pipe - USB_PIPE1) * 2);
    } else if ((pipe >= USB_PIPE9) && (pipe <= USB_PIPE10)) {
        return (uint16_t *) & (USB_MX.PIPE9TRN) + ((pipe - USB_PIPE9) * 2);
    } else if ((pipe >= USB_PIPE11) && (pipe <= USB_PIPE15)) {
        return (uint16_t *) & (USB_MX.PIPEBTRN) + ((pipe - USB_PIPE11) * 2);
    } else {
        return NULL;
    }
}

uint16_t *USBPhyHw::get_fifoctr_reg(uint16_t pipe)
{
    uint16_t fifo_use = PIPE2FIFO(pipe);

    if ((fifo_use & USB_FUNCTION_D0FIFO_USE) == USB_FUNCTION_D0FIFO_USE) {
        return (uint16_t *) & (USB_MX.D0FIFOCTR);
    } else if ((fifo_use & USB_FUNCTION_D1FIFO_USE) == USB_FUNCTION_D1FIFO_USE) {
        return (uint16_t *) & (USB_MX.D1FIFOCTR);
    } else {
        return (uint16_t *) & (USB_MX.CFIFOCTR);
    }
}

uint16_t *USBPhyHw::get_fifosel_reg(uint16_t pipe)
{
    uint16_t fifo_use = PIPE2FIFO(pipe);

    if ((fifo_use & USB_FUNCTION_D0FIFO_USE) == USB_FUNCTION_D0FIFO_USE) {
        return (uint16_t *) & (USB_MX.D0FIFOSEL);
    } else if ((fifo_use & USB_FUNCTION_D1FIFO_USE) == USB_FUNCTION_D1FIFO_USE) {
        return (uint16_t *) & (USB_MX.D1FIFOSEL);
    } else {
        return (uint16_t *) & (USB_MX.CFIFOSEL);
    }
}

uint32_t *USBPhyHw::get_fifo_reg(uint16_t pipe)
{
    uint16_t fifo_use = PIPE2FIFO(pipe);

    if ((fifo_use & USB_FUNCTION_D0FIFO_USE) == USB_FUNCTION_D0FIFO_USE) {
        return (uint32_t *) & (USB_MX.D0FIFO);
    } else if ((fifo_use & USB_FUNCTION_D1FIFO_USE) == USB_FUNCTION_D1FIFO_USE) {
        return (uint32_t *) & (USB_MX.D1FIFO);
    } else {
        return (uint32_t *) & (USB_MX.CFIFO);
    }
}

uint16_t USBPhyHw::get_pid(uint16_t pipe)
{
    volatile uint16_t *p_reg;

    p_reg = get_pipectr_reg(pipe);
    return (uint16_t)(*p_reg & USB_PID);
}

void USBPhyHw::set_mbw(uint16_t pipe, uint16_t data)
{
    volatile uint16_t *p_reg;

    p_reg = get_fifosel_reg(pipe);
    *p_reg &= (~USB_MBW);
    if (data != 0) {
        *p_reg |= data;
    }
}

void USBPhyHw::set_pid(uint16_t pipe, uint16_t new_pid)
{
    volatile uint16_t *p_reg;
    uint16_t old_pid;

    p_reg = get_pipectr_reg(pipe);
    old_pid = get_pid(pipe);

    switch (new_pid) {
        case USB_PID_STALL:
            if ((old_pid & USB_PID_BUF) == USB_PID_BUF) {
                *p_reg &= (~USB_PID);
                *p_reg |= USB_PID_STALL2;
            } else {
                *p_reg &= (~USB_PID);
                *p_reg |= new_pid;
            }
            break;
        case USB_PID_BUF:
            if (((old_pid & USB_PID_STALL) == USB_PID_STALL) ||
                    ((old_pid & USB_PID_STALL2) == USB_PID_STALL2)) {
                *p_reg &= (~USB_PID);
                *p_reg |= USB_PID_NAK;
            }
            *p_reg &= (~USB_PID);
            *p_reg |= new_pid;
            break;
        case USB_PID_NAK:
            if ((old_pid & USB_PID_STALL2) == USB_PID_STALL2) {
                *p_reg &= (~USB_PID);
                *p_reg |= USB_PID_STALL;
            }
            *p_reg &= (~USB_PID);
            *p_reg |= new_pid;

            do {
                cpu_delay_1us(1);
                p_reg = get_pipectr_reg(pipe);
            } while ((*p_reg & USB_PBUSY) == USB_PBUSY);
            break;
        default:
            *p_reg &= (~USB_PID);
            *p_reg |= new_pid;
            break;
    }
}

void USBPhyHw::cpu_delay_1us(uint16_t time)
{
    volatile uint32_t i = 38 * time;

    while (i > 0) {
        i--;
    }
}

uint16_t USBPhyHw::EP2PIPE(uint16_t endpoint)
{
    const struct PIPECFGREC *cfg;

    for (cfg = &def_pipecfg[0]; cfg->pipesel != 0; cfg++) {
        if (cfg->endpoint == endpoint) {
            break;
        }
    }
    return (cfg->pipesel & USB_CURPIPE);
}

uint16_t USBPhyHw::PIPE2EP(uint16_t pipe)
{
    const struct PIPECFGREC *cfg;

    if (pipe == USB_PIPE0) {
        return 0;
    }
    for (cfg = &def_pipecfg[0]; cfg->pipesel != 0; cfg++) {
        if ((cfg->pipesel & USB_CURPIPE) == pipe) {
            break;
        }
    }
    return cfg->endpoint;
}

uint16_t USBPhyHw::PIPE2FIFO(uint16_t pipe)
{
    const struct PIPECFGREC *cfg;
    uint16_t fifo_use;

    if (pipe == USB_PIPE0) {
        fifo_use = USB_FUNCTION_CFIFO_USE;
    } else {
        for (cfg = &def_pipecfg[0]; cfg->pipesel != 0; cfg++) {
            if ((cfg->pipesel & USB_CURPIPE) == pipe) {
                break;
            }
        }
        if ((cfg->pipesel & USB_FUNCTION_D0FIFO_USE) == USB_FUNCTION_D0FIFO_USE) {
            fifo_use = USB_FUNCTION_D0FIFO_USE;
        } else if ((cfg->pipesel & USB_FUNCTION_D1FIFO_USE) == USB_FUNCTION_D1FIFO_USE) {
            fifo_use = USB_FUNCTION_D1FIFO_USE;
        } else {
            fifo_use = USB_FUNCTION_CFIFO_USE;
        }
    }

    return fifo_use;
}

void USBPhyHw::reset_usb(uint16_t clockmode)
{
#if (USB_FUNCTION_CH == 0)
    if ((USB200.SYSCFG0 & USB_UPLLE) == USB_UPLLE) {
        if ((USB200.SYSCFG0 & USB_UCKSEL) != clockmode) {
            USB200.SUSPMODE &= ~(USB_SUSPMODE_SUSPM);
            USB200.SYSCFG0 = 0;
            USB200.SYSCFG0 = (clockmode | USB_UPLLE);
            cpu_delay_1us(1000);
            USB200.SUSPMODE |= USB_SUSPMODE_SUSPM;
        } else {
            USB200.SUSPMODE &= ~(USB_SUSPMODE_SUSPM);
            cpu_delay_1us(1000);
            USB200.SUSPMODE |= USB_SUSPMODE_SUSPM;
        }
    } else {
        USB200.SUSPMODE &= ~(USB_SUSPMODE_SUSPM);
        USB200.SYSCFG0 = 0;
        USB200.SYSCFG0 = (clockmode | USB_UPLLE);
        cpu_delay_1us(1000);
        USB200.SUSPMODE |= USB_SUSPMODE_SUSPM;
    }
#else
    /* UCKSEL and UPLLE bit is only USB0. If USB1, set to SYSCFG0 for USB0. */
    if ((USB200.SYSCFG0 & USB_UPLLE) == USB_UPLLE) {
        if ((USB200.SYSCFG0 & USB_UCKSEL) != clockmode) {
            USB201.SUSPMODE &= ~(USB_SUSPMODE_SUSPM);
            USB200.SUSPMODE &= ~(USB_SUSPMODE_SUSPM);
            USB201.SYSCFG0 = 0;
            USB200.SYSCFG0 = 0;
            USB200.SYSCFG0 = (clockmode | USB_UPLLE);
            cpu_delay_1us(1000);
            USB200.SUSPMODE |= USB_SUSPMODE_SUSPM;
            USB201.SUSPMODE |= USB_SUSPMODE_SUSPM;
        } else {
            USB201.SUSPMODE &= ~(USB_SUSPMODE_SUSPM);
            cpu_delay_1us(1000);
            USB201.SUSPMODE |= USB_SUSPMODE_SUSPM;
        }
    } else {
        USB201.SUSPMODE &= ~(USB_SUSPMODE_SUSPM);
        USB200.SUSPMODE &= ~(USB_SUSPMODE_SUSPM);
        USB201.SYSCFG0 = 0;
        USB200.SYSCFG0 = 0;
        USB200.SYSCFG0 = (clockmode | USB_UPLLE);
        cpu_delay_1us(1000);
        USB200.SUSPMODE |= USB_SUSPMODE_SUSPM;
        USB201.SUSPMODE |= USB_SUSPMODE_SUSPM;
    }
#endif
    USB_MX.BUSWAIT = (uint16_t)(USB_BWAIT_3 & USB_BWAIT);   /* 3 : 5 access cycles  waits */
    USB_MX.CFIFOSEL  = USB_MBW_32;
    USB_MX.D0FIFOSEL = USB_MBW_32;
    USB_MX.D1FIFOSEL = USB_MBW_32;
}

bool USBPhyHw::chk_vbsts(void)
{
    uint16_t buf1;
    uint16_t buf2;
    uint16_t buf3;
    bool connect_flg = false;

    /* VBUS chattering cut */
    do {
        buf1 = USB_MX.INTSTS0;
        cpu_delay_1us(10);
        buf2 = USB_MX.INTSTS0;
        cpu_delay_1us(10);
        buf3 = USB_MX.INTSTS0;
    } while (((buf1 & USB_VBSTS) != (buf2 & USB_VBSTS)) || ((buf2 & USB_VBSTS) != (buf3 & USB_VBSTS)));

    /* VBUS status judge */
    if ((buf1 & USB_VBSTS) != (uint16_t)0) {
        connect_flg = true;
    }

    return connect_flg;
}

void USBPhyHw::ctrl_end(uint16_t status)
{
    /* Interrupt disable */
    USB_MX.BEMPENB &= (~(1 << USB_PIPE0));  /* Disable Empty Interrupt */
    USB_MX.BRDYENB &= (~(1 << USB_PIPE0));  /* Disable Ready Interrupt */
    USB_MX.NRDYENB &= (~(1 << USB_PIPE0));  /* Disable Not Ready Interrupt */

    set_mbw(USB_PIPE0, USB_MBW_32);

    if ((status == USB_DATA_ERR) || (status == USB_DATA_OVR)) {
        set_pid(USB_PIPE0, USB_PID_STALL);  /* Request error */
    } else if (status == USB_DATA_STOP) {
        set_pid(USB_PIPE0, USB_PID_NAK);    /* Pipe stop */
    } else {
        USB_MX.DCPCTR |= USB_CCPL;          /* Set CCPL bit */
    }
}

void USBPhyHw::data_end(uint16_t pipe, uint16_t status)
{
    volatile uint16_t *p_reg;

    /* Disable Interrupt */
    USB_MX.BRDYENB &= (~(1 << pipe));           /* Disable Ready Interrupt */
    USB_MX.NRDYENB &= (~(1 << pipe));           /* Disable Not Ready Interrupt */
    USB_MX.BEMPENB &= (~(1 << pipe));           /* Disable Empty Interrupt */

    set_pid(pipe, USB_PID_NAK);                 /* Set NAK */

    /* Disable Transaction count */
    p_reg = get_pipetre_reg(pipe);
    if (p_reg != NULL) {
        *p_reg &= (~USB_TRENB);
        *p_reg |= USB_TRCLR;
    }

    if (pipe_ctrl[pipe].enable) {
        /* Check PIPE TYPE */
        USB_MX.PIPESEL = pipe;                  /* Pipe select */
        if ((USB_MX.PIPECFG & USB_TYPFIELD) != USB_TYPFIELD_ISO) {
            /* Transfer information set */
            pipe_ctrl[pipe].enable = false;
            pipe_ctrl[pipe].status = status;
        } else if ((uint16_t)(USB_MX.PIPECFG & USB_DIRFIELD) == USB_BUF2FIFO) {
            /* ISO OUT Transfer (restart) */
            pipe_ctrl[pipe].status = USB_DATA_WRITING;
        } else {
            /* ISO IN Transfer (restart) */
            pipe_ctrl[pipe].status = USB_DATA_READING;
        }
    }
}

void USBPhyHw::forced_termination(uint16_t pipe, uint16_t status)
{
    volatile uint16_t *p_reg;

    /* Disable Interrupt */
    USB_MX.BRDYENB &= (~(1 << pipe));           /* Disable Ready Interrupt */
    USB_MX.NRDYENB &= (~(1 << pipe));           /* Disable Not Ready Interrupt */
    USB_MX.BEMPENB &= (~(1 << pipe));           /* Disable Empty Interrupt */

    set_pid(pipe, USB_PID_NAK);                 /* Set NAK */

    /* Disable Transaction count */
    p_reg = get_pipetre_reg(pipe);
    if (p_reg != NULL) {
        *p_reg &= (~USB_TRENB);
        *p_reg |= USB_TRCLR;
    }

    set_mbw(pipe, USB_MBW_32);

    chg_curpipe(pipe, 0);                   /* Changes the FIFO port by the pipe. */

    p_reg = get_pipectr_reg(pipe);
    /* Buffer Clear */
    *p_reg |= USB_ACLRM;
    *p_reg &= ~USB_ACLRM;

    pipe_ctrl[pipe].enable = false;
    pipe_ctrl[pipe].status  = status;
}

#endif
