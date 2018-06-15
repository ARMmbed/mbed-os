/*
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "arm_mps2_io_drv.h"

/* There is at most 8 LEDs and switches on MPS2 SCC and 2 on FPGA IO */
#define MAX_PIN_NBR_SCC    8
#define MAX_PIN_NBR_FPGAIO 2

/* Mask to 1 the first X bits */
#define MASK(X)            ((1 << (X)) - 1)

/* MPS2 IO register map structure */
struct arm_mps2_io_reg_map_t {
    union {
        volatile uint32_t scc_leds;     /* Offset: 0x000 (R/W) Controls the MCC
                                         *                     user LEDs
                                         *         [31:8] : Reserved
                                         *         [7:0]  : MCC LEDs */
        volatile uint32_t fpgaio_leds;  /* Offset: 0x000 (R/W) LED connections
                                         *         [31:2] : Reserved
                                         *         [1:0]  : FPGAIO LEDs */
    } led_reg;
    volatile uint32_t reserved[1];
    union {
        volatile uint32_t scc_switches;  /* Offset: 0x008 (R/ ) Denotes the
                                          *                     state of the MCC
                                          *                     user switches
                                          *         [31:8] : Reserved
                                          *         [7:0]  : State of the MCC
                                          *                  switches */
        volatile uint32_t fpgaio_buttons;/* Offset: 0x008 (R/ ) Buttons
                                          *         [31:2] : Reserved
                                          *         [1:0]  : Buttons */
    } button_reg;
    volatile uint32_t reserved1[16];
    volatile uint32_t misc;              /* Offset: 0x04C (R/W)  Misc control
                                          *              [31:7] : Reserved
                                          *              [6] : CLCD_BL_CTRL
                                          *              [5] : CLCD_RD
                                          *              [4] : CLCD_RS
                                          *              [3] : CLCD_RESET
                                          *              [2] : Reserved
                                          *              [1] : SPI_nSS
                                          *              [0] : CLCD_CS */
};

void arm_mps2_io_write_leds(struct arm_mps2_io_dev_t* dev,
                            enum arm_mps2_io_access_t access,
                            uint8_t pin_num,
                            uint32_t value)
{
    struct arm_mps2_io_reg_map_t* p_mps2_io_port =
                                  (struct arm_mps2_io_reg_map_t*)dev->cfg->base;
    /* Mask of involved bits */
    uint32_t write_mask = 0;

    switch (dev->cfg->type) {
    case ARM_MPS2_IO_TYPE_SCC:
        if (pin_num >= MAX_PIN_NBR_SCC) {
            return;
        }

        switch (access) {
        case ARM_MPS2_IO_ACCESS_PIN:
            write_mask = (1UL << pin_num);
            break;
        case ARM_MPS2_IO_ACCESS_PORT:
            write_mask = MASK(MAX_PIN_NBR_SCC);
            break;
        /*
         * default: explicitely not used to force to cover all enumeration
         * cases
         */
        }

        if (value) {
            p_mps2_io_port->led_reg.scc_leds |= write_mask;
        } else {
            p_mps2_io_port->led_reg.scc_leds &= ~write_mask;
        }

        break;
    case ARM_MPS2_IO_TYPE_FPGAIO:
        if (pin_num >= MAX_PIN_NBR_FPGAIO) {
            return;
        }

        switch (access) {
        case ARM_MPS2_IO_ACCESS_PIN:
            write_mask = (1UL << pin_num);
            break;
        case ARM_MPS2_IO_ACCESS_PORT:
            write_mask = MASK(MAX_PIN_NBR_FPGAIO);
            break;
        /*
         * default: explicitely not used to force to cover all enumeration
         * cases
         */
        }

        if (value) {
            p_mps2_io_port->led_reg.fpgaio_leds |= write_mask;
        } else {
            p_mps2_io_port->led_reg.fpgaio_leds &= ~write_mask;
        }

        break;
    /* default: explicitely not used to force to cover all enumeration cases */
    }
}

void arm_mps2_io_write_misc(struct arm_mps2_io_dev_t* dev,
                            enum arm_mps2_io_access_t access,
                            uint8_t pin_num,
                            uint32_t value)
{
    struct arm_mps2_io_reg_map_t* p_mps2_io_port =
                                  (struct arm_mps2_io_reg_map_t*)dev->cfg->base;

    /* The MISC write is for FPGAIO only */
    if (dev->cfg->type != ARM_MPS2_IO_TYPE_FPGAIO)
        return;

    if (value) {
        p_mps2_io_port->misc |= (1UL << pin_num);
    } else {
        p_mps2_io_port->misc &= ~(1UL << pin_num);
    }
}

uint32_t arm_mps2_io_read_buttons(struct arm_mps2_io_dev_t* dev,
                                  enum arm_mps2_io_access_t access,
                                  uint8_t pin_num)
{
    struct arm_mps2_io_reg_map_t* p_mps2_io_port =
                                  (struct arm_mps2_io_reg_map_t*)dev->cfg->base;
    uint32_t value = 0;

    switch (dev->cfg->type) {
    case ARM_MPS2_IO_TYPE_SCC:
        if (pin_num >= MAX_PIN_NBR_SCC) {
            return 0;
        }

        /* Only read significant bits from this register */
        value = p_mps2_io_port->button_reg.scc_switches & MASK(MAX_PIN_NBR_SCC);

        break;
    case ARM_MPS2_IO_TYPE_FPGAIO:
        if (pin_num >= MAX_PIN_NBR_FPGAIO) {
            return 0;
        }

        /* Only read significant bits from this register */
        value = p_mps2_io_port->button_reg.fpgaio_buttons &
                MASK(MAX_PIN_NBR_FPGAIO);

        break;
    /* default: explicitely not used to force to cover all enumeration cases */
    }

    if (access == ARM_MPS2_IO_ACCESS_PIN) {
        value = ((value >> pin_num) & 1UL);
    }

    return value;
}

uint32_t arm_mps2_io_read_leds(struct arm_mps2_io_dev_t* dev,
                               enum arm_mps2_io_access_t access,
                               uint8_t pin_num)
{
    struct arm_mps2_io_reg_map_t* p_mps2_io_port =
                                  (struct arm_mps2_io_reg_map_t*)dev->cfg->base;
    uint32_t value = 0;

    switch (dev->cfg->type) {
    case ARM_MPS2_IO_TYPE_SCC:
        if (pin_num >= MAX_PIN_NBR_SCC) {
            return 0;
        }

        /* Only read significant bits from this register */
        value = p_mps2_io_port->led_reg.scc_leds & MASK(MAX_PIN_NBR_SCC);

        break;
    case ARM_MPS2_IO_TYPE_FPGAIO:
        if (pin_num >= MAX_PIN_NBR_FPGAIO) {
            return 0;
        }

        /* Only read significant bits from this register */
        value = p_mps2_io_port->led_reg.fpgaio_leds & MASK(MAX_PIN_NBR_FPGAIO);

        break;
    /* default: explicitely not used to force to cover all enumeration cases */
    }

    if (access == ARM_MPS2_IO_ACCESS_PIN) {
        value = ((value >> pin_num) & 1UL);
    }

    return value;
}
