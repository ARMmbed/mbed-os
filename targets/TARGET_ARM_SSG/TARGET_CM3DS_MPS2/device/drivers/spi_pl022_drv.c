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

#include "spi_pl022_drv.h"

/******************************************************************************
 * PL022 device specific definitions based on DDI0194H_ssp_pl022_trm.pdf which
 * is available from infocenter.arm.com.
 *
 * This version of driver aims at necessary functionality for MPS2 board only
 *****************************************************************************/

/* Structure for the SSP Primary Cell device registers */
struct spi_pl022_dev_reg_map_t {
    volatile uint32_t sspcr0;        /* Control register 0 */
    volatile uint32_t sspcr1;        /* Control register 1 */
    volatile uint32_t sspdr;         /* Data register */
    volatile uint32_t sspsr;         /* Status register */
    volatile uint32_t sspcpsr;       /* Clock prescale register */
    volatile uint32_t sspimsc;       /* Interrupt mask set or clear register */
    volatile uint32_t sspris;        /* Raw interrupt status register */
    volatile uint32_t sspmis;        /* Masked interrupt status register */
    volatile uint32_t sspicr;        /* Interrupt clear register */
    volatile uint32_t sspdmacr;      /* DMA control register */
    volatile uint32_t reserved[1006];/* Reserved from Base+0x28-0xFE0 */
    volatile uint32_t sspperiphid0;  /* Peripheral id register 0 */
    volatile uint32_t sspperiphid1;  /* Peripheral id register 1 */
    volatile uint32_t sspperiphid2;  /* Peripheral id register 2 */
    volatile uint32_t sspperiphid3;  /* Peripheral id register 3 */
    volatile uint32_t ssppcellid0;   /* Primary cell id register 0 */
    volatile uint32_t ssppcellid1;   /* Primary cell id register 1 */
    volatile uint32_t ssppcellid2;   /* Primary cell id register 2 */
    volatile uint32_t ssppcellid3;   /* Primary cell id register 3 */
};

/*--- SSP Control Register 0 ---*/

/* Data Size Select {0x3=4 ... 0xF=16} */
#define SPI_PL022_SSPCR0_DSS_POS         0
#define SPI_PL022_SSPCR0_DSS_MSK         0xF

/* Frame format */
#define SPI_PL022_SSPCR0_FRF_POS         4
#define SPI_PL022_SSPCR0_FRF_MSK         (0x3ul<<SPI_PL022_SSPCR0_FRF_POS)
#define SPI_PL022_SSPCR0_FRF_MOT_SPI     0x0
#define SPI_PL022_SSPCR0_FRF_TI_SS       0x1
#define SPI_PL022_SSPCR0_FRF_MICROWIRE   0x2
#define SPI_PL022_SSPCR0_FRF_RES         0x3

/* Clock polarity applicable to Motorola SPI format only */
#define SPI_PL022_SSPCR0_SPO_POS         6
#define SPI_PL022_SSPCR0_SPO_MSK         (0x1ul<<SPI_PL022_SSPCR0_SPO_POS)

/* Clock phase applicable to Motorola SPI format only */
#define SPI_PL022_SSPCR0_SPH_POS         7
#define SPI_PL022_SSPCR0_SPH_MSK         (0x1ul<<SPI_PL022_SSPCR0_SPH_POS)

/* Serial clock rate */
#define SPI_PL022_SSPCR0_SCR_POS         8
#define SPI_PL022_SSPCR0_SCR_MSK         (0xFFul<<SPI_PL022_SSPCR0_SCR_POS)

/*--- SSP Control Register 1 ---*/

/* Loopback mode*/
#define SPI_PL022_SSPCR1_LBM_POS         0
#define SPI_PL022_SSPCR1_LBM_MSK         (0x1ul<<SPI_PL022_SSPCR1_LBM_POS)

/* Syncrhonous serial port enable*/
#define SPI_PL022_SSPCR1_SSE_POS         1
#define SPI_PL022_SSPCR1_SSE_MSK         (0x1ul<<SPI_PL022_SSPCR1_SSE_POS)

/* Master or Slave select */
#define SPI_PL022_SSPCR1_MS_POS          2
#define SPI_PL022_SSPCR1_MS_MSK          (0x1ul<<SPI_PL022_SSPCR1_MS_POS)

/* Slave mode output disable */
#define SPI_PL022_SSPCR1_SOD_POS         3
#define SPI_PL022_SSPCR1_SOD_MSK         (0x1ul<<SPI_PL022_SSPCR1_SOD_POS)

/*--- Clock PreScale Register ---*/

/* Divisor */
#define SPI_PL022_SSPCPSR_CPSDVSR_POS   0
#define SPI_PL022_SSPCPSR_CPSDVSR_MSK   (0xFFul<<SPI_PL022_SSPCPSR_CPSDVSR_POS)
#define SPI_PL022_INVALID_SSPCPSR_VALUE 0
#define SPI_PL022_MIN_SSPCPSR_VALUE     2
#define SPI_PL022_MAX_SSPCPSR_VALUE     254
#define SPI_PL022_MAX_SCR_VALUE         255


/*--- Interrupt Mask Set or Clear Register --- */

/* Receive Overrun Interrupt Mask */
#define SPI_PL022_SSPIMSC_RORIM_POS     0
#define SPI_PL022_SSPIMSC_RORIM_MSK     (0x1ul<<SPI_PL022_SSPIMSC_RORIM_POS)

/* Receive Timeout Interrupt Mask */
#define SPI_PL022_SSPIMSC_RTIM_POS      1
#define SPI_PL022_SSPIMSC_RTIM_MSK      (0x1ul<<SPI_PL022_SSPIMSC_RTIM_POS)

/* Receive FIFO Interrupt Mask */
#define SPI_PL022_SSPIMSC_RXIM_POS      2
#define SPI_PL022_SSPIMSC_RXIM_MSK      (0x1ul<<SPI_PL022_SSPIMSC_RXIM_POS)

/* Transmit FIFO Interrupt Mask */
#define SPI_PL022_SSPIMSC_TXIM_POS      3
#define SPI_PL022_SSPIMSC_TXIM_MSK      (0x1ul<<SPI_PL022_SSPIMSC_TXIM_POS)

/*--- Interrupt Mask Set or Clear Register ---*/

/* Receive Overrun Interrupt Mask */
#define SPI_PL022_SSPIMSC_RORIM_POS     0
#define SPI_PL022_SSPIMSC_RORIM_MSK     (0x1ul<<SPI_PL022_SSPIMSC_RORIM_POS)

/* Receive Timeout Interrupt Mask */
#define SPI_PL022_SSPIMSC_RTIM_POS      1
#define SPI_PL022_SSPIMSC_RTIM_MSK      (0x1ul<<SPI_PL022_SSPIMSC_RTIM_POS)

/* Receive FIFO Interrupt Mask */
#define SPI_PL022_SSPIMSC_RXIM_POS      2
#define SPI_PL022_SSPIMSC_RXIM_MSK      (0x1ul<<SPI_PL022_SSPIMSC_RXIM_POS)

/* Transmit FIFO Interrupt Mask */
#define SPI_PL022_SSPIMSC_TXIM_POS      3
#define SPI_PL022_SSPIMSC_TXIM_MSK      (0x1ul<<SPI_PL022_SSPIMSC_TXIM_POS)

#define SPI_PL022_SSPIMSC_VALID_MSK     \
                               ((0x1ul<<(SPI_PL022_SSPIMSC_TXIM_POS+1))-1)

/*--- Raw Interrupt Status Register ---*/

/* SSPRORINTR */
#define SPI_PL022_SSPRIS_RORRIS_POS     0
#define SPI_PL022_SSPRIS_RORRIS_MSK     (0x1ul<<SPI_PL022_SSPRIS_RORRIS_POS)

/* SSPRTINTR */
#define SPI_PL022_SSPRIS_RTRIS_POS      1
#define SPI_PL022_SSPRIS_RTRIS_MSK      (0x1ul<<SPI_PL022_SSPRIS_RTRIS_POS)

/* SSPRXINTR */
#define SPI_PL022_SSPRIS_RXRIS_POS      2
#define SPI_PL022_SSPRIS_RXRIS_MSK      (0x1ul<<SPI_PL022_SSPRIS_RXRIS_POS)

/* SSPTXINTR */
#define SPI_PL022_SSPRIS_TXRIS_POS      3
#define SPI_PL022_SSPRIS_TXRIS_MSK      (0x1ul<<SPI_PL022_SSPRIS_TXRIS_POS)

#define SPI_PL022_SSPRIS_VALID_MSK      \
                               ((0x1ul<<(SPI_PL022_SSPRIS_TXRIS_POS+1))-1)

/*--- Masked Interrupt Status Register ---*/

/* SSPRORINTR */
#define SPI_PL022_SSPMIS_RORMIS_POS     0
#define SPI_PL022_SSPMIS_RORMIS_MSK     (0x1ul<<SPI_PL022_SSPMIS_RORMIS_POS)

/* SSPRTINTR */
#define SPI_PL022_SSPMIS_RTMIS_POS      1
#define SPI_PL022_SSPMIS_RTMIS_MSK      (0x1ul<<SPI_PL022_SSPMIS_RTMIS_POS)

/* SSPRXINTR */
#define SPI_PL022_SSPMIS_RXMIS_POS      2
#define SPI_PL022_SSPMIS_RXMIS_MSK      (0x1ul<<SPI_PL022_SSPMIS_RXMIS_POS)

/* SSPTXINTR */
#define SPI_PL022_SSPMIS_TXMIS_POS      3
#define SPI_PL022_SSPMIS_TXMIS_MSK      (0x1ul<<SPI_PL022_SSPMIS_TXMIS_POS)

#define SPI_PL022_SSPMIS_VALID_MSK      \
                               ((0x1ul<<(SPI_PL022_SSPMIS_TXMIS_POS+1))-1)

/*--- Interrupt Clear Register --- */

/* SSPRORINTR Clear */
#define SPI_PL022_SSPICR_RORIC_POS      0
#define SPI_PL022_SSPICR_RORIC_MSK      (0x1ul<<SPI_PL022_SSPICR_RORIC_POS)

/* SSPRTINTR Clear */
#define SPI_PL022_SSPICR_RTIC_POS       1
#define SPI_PL022_SSPICR_RTIC_MSK       (0x1ul<<SPI_PL022_SSPICR_RTIC_POS)

#define SPI_PL022_SSPICR_VALID_MSK      ((0x1ul<<(SPI_PL022_SSPICR_RTIC_POS+1))-1)

/*--- DMA Control Register --- */

/* Receive DMA Enable */
#define SPI_PL022_SSPDMACR_RXDMAE_POS   0
#define SPI_PL022_SSPDMACR_RXDMAE_MSK   (0x1ul<<SPI_PL022_SSPDMACR_RXDMAE_POS)

/* Transmit DMA Enable */
#define SPI_PL022_SSPDMACR_TXDMAE_POS   1
#define SPI_PL022_SSPDMACR_TXDMAE_MSK   (0x1ul<<SPI_PL022_SSPDMACR_TXDMAE_POS)

#define SPI_PL022_SSPDMACR_VALID_MSK    ((0x1ul<<(SPI_PL022_SSPDMACR_TXDMAE_POS+1))-1)

/*--- Peripheral Identification Registers ---*/

#define SPI_PL022_SSPPERIPH_ID_OFFSET (0xFE0ul)

/* Part Number 0 */
#define SPI_PL022_SSPPERIPH_ID0_PARTNO_0_POS   0
#define SPI_PL022_SSPPERIPH_ID0_PARTNO_0_SIZE  8
#define SPI_PL022_SSPPERIPH_ID0_PARTNO_0_MSK   \
                                 (0xFFul<<SPI_PL022_SSPPERIPH_ID0_PARTNO_0_POS)

/* Part Number 1 */
#define SPI_PL022_SSPPERIPH_ID1_PARTNO_1_POS  0
#define SPI_PL022_SSPPERIPH_ID1_PARTNO_1_SIZE 0
#define SPI_PL022_SSPPERIPH_ID1_PARTNO_1_MSK \
                                 (0xFul<<SPI_PL022_SSPPERIPH_ID1_PARTNO_1_POS)

/* Designer 0 */
#define SPI_PL022_SSPPERIPH_ID1_DESIGNER_0_POS  4
#define SPI_PL022_SSPPERIPH_ID1_DESIGNER_0_SIZE 4
#define SPI_PL022_SSPPERIPH_ID1_DESIGNER_0_MSK \
                                 (0xFul<<SPI_PL022_SSPPERIPH_ID1_DESIGNER_0_POS)

/* Designer 1 */
#define SPI_PL022_SSPPERIPH_ID2_DESIGNER_1_POS  0
#define SPI_PL022_SSPPERIPH_ID2_DESIGNER_1_SIZE 4
#define SPI_PL022_SSPPERIPH_ID2_DESIGNER_1_MSK \
                                 (0xFul<<SPI_PL022_SSPPERIPH_ID2_DESIGNER_1_POS)

/* Revision */
#define SPI_PL022_SSPPERIPH_ID2_REVISION_POS  4
#define SPI_PL022_SSPPERIPH_ID2_REVISION_SIZE 4
#define SPI_PL022_SSPPERIPH_ID2_REVIISON_MSK \
                                 (0xFul<<SPI_PL022_SSPPERIPH_ID2_REVISION_POS)

/* Config */
#define SPI_PL022_SSPPERIPH_ID3_CONFIG_POS   0
#define SPI_PL022_SSPPERIPH_ID3_CONFIG_MSK   \
                                 (0xFFul<<SPI_PL022_SSPPERIPH_ID3_CONFIG_POS)

/*--- PrimeCell Identification Registers ---*/

#define SPI_PL022_SSPPCELL_ID_OFFSET   (0xFF0ul)

#define SPI_PL022_SSPPCELL_ID0_POS     0
#define SPI_PL022_SSPPCELL_ID0_MSK     (0xFFul<<SPI_PL022_SSPPCELL_ID0_POS)

#define SPI_PL022_SSPPCELL_ID1_POS     0
#define SPI_PL022_SSPPCELL_ID1_MSK     (0xFFul<<SPI_PL022_SSPPCELL_ID1_POS)

#define SPI_PL022_SSPPCELL_ID2_POS     0
#define SPI_PL022_SSPPCELL_ID2_MSK     (0xFFul<<SPI_PL022_SSPPCELL_ID2_POS)

#define SPI_PL022_SSPPCELL_ID3_POS     0
#define SPI_PL022_SSPPCELL_ID3_MSK     (0xFFul<<SPI_PL022_SSPPCELL_ID3_POS)

/* ARM SPI PL022 state definitions */
#define SPI_PL022_INITIALIZED          (1 << 0)

#define WORD_1BYTE_MASK                (0xFFul)
#define WORD_2BYTES_MASK               (0xFFFFul)


/************************* PL022 TEST Definitions ******************************/
#define SPI_PL022_TEST_REG_BASE        (0x80ul)

struct spi_pl022_dev_test_reg_map_t {
    volatile uint32_t ssptcr;        /* Test Control register */
    volatile uint32_t sspitip;       /* Integration test input register */
    volatile uint32_t sspitop;       /* Integration test output register */
    volatile uint32_t ssptdr;        /* Test data register */
};

/* Test control register */
#define SPI_PL022_SSPTCR_ITEN_POS           0
#define SPI_PL022_SSPTCR_ITEN_MSK          (0x1ul<<SPI_PL022_SSPTCR_ITEN_POS)

#define SPI_PL022_SSPTCR_TESTFIFO_POS       1
#define SPI_PL022_SSPTCR_TESTFIFO_MSK       (0x1ul<<SPI_PL022_SSPTCR_TESTFIFO_POS)


/* Integration test input register */
#define SPI_PL022_SSPITIP_RXD_POS           0
#define SPI_PL022_SSPITIP_RXD_MSK           (0x1ul<<SPI_PL022_SSPITIP_RXD_POS)

#define SPI_PL022_SSPITIP_FSSIN_POS         1
#define SPI_PL022_SSPITIP_FSSIN_MSK         (0x1ul<<SPI_PL022_SSPITIP_FSSIN_POS)

#define SPI_PL022_SSPITIP_CLKIN_POS         2
#define SPI_PL022_SSPITIP_CLKIN_MSK         (0x1ul<<SPI_PL022_SSPITIP_CLKIN_POS)

#define SPI_PL022_SSPITIP_RXDMACLR_POS      3
#define SPI_PL022_SSPITIP_RXDMACLR_MSK      (0x1ul<<SPI_PL022_SSPITIP_RXDMACLR_POS)

#define SPI_PL022_SSPITIP_TXDMACLR_POS      4
#define SPI_PL022_SSPITIP_TXDMACLR_MSK      (0x1ul<<SPI_PL022_SSPITIP_TXDMACLR_POS)

/* Integration test output register */
#define SPI_PL022_SSPITOP_RXDMABREQ_POS     10
#define SPI_PL022_SSPITOP_RXDMABREQ_MSK     (0x1ul<<SPI_PL022_SSPITOP_RXDMABREQ_POS)

#define SPI_PL022_SSPITOP_RXDMASREQ_POS     11
#define SPI_PL022_SSPITOP_RXDMASREQ_MSK     (0x1ul<<SPI_PL022_SSPITOP_RXDMASREQ_POS)

#define SPI_PL022_SSPITOP_TXDMABREQ_POS     12
#define SPI_PL022_SSPITOP_TXDMABREQ_MSK     (0x1ul<<SPI_PL022_SSPITOP_TXDMABREQ_POS)

#define SPI_PL022_SSPITOP_TXDMASREQ_POS     13
#define SPI_PL022_SSPITOP_TXDMASREQ_MSK     (0x1ul<<SPI_PL022_SSPITOP_TXDMASREQ_POS)

/************************* PL022 Definitions End ******************************/


/*
 * \brief Calculates clock prescale divisor and sets serial clock rate
 *        for the SPI PL022 device.
 *
 * \param[in] ctrl_cfg  SPI control configuration \ref spi_pl022_ctrl_cfg_t
 * \param[in] sys_clk   System clock.
 * \param[in/out] cr0   Pointer to PL022 control register 0
 *                                             \ref spi_pl022_dev_reg_map_t
 *
 * \return Value of the SSPCPSR register \ref spi_pl022_dev_reg_map_t
 *
 * \note This function doesn't check if sys_clk or ctrl_cfg->bit_rate is 0
 */
static uint32_t spi_calc_clock_rate(
                            const struct spi_pl022_ctrl_cfg_t* ctrl_cfg,
                            uint32_t sys_clk, volatile uint32_t *cr0)
{
    uint32_t clkps_dvsr;  /* clock prescale divisor */
    uint32_t scr;         /* serial clock rate */

    for(clkps_dvsr = SPI_PL022_MIN_SSPCPSR_VALUE;
        clkps_dvsr <= SPI_PL022_MAX_SSPCPSR_VALUE; clkps_dvsr += 2) {

        /* Calculate clock rate based on the new clock prescale divisor */
        scr = (sys_clk / (clkps_dvsr * ctrl_cfg->bit_rate)) - 1;

        /* Checks if it can be supported by the divider */
        if (scr <= SPI_PL022_MAX_SCR_VALUE) {
            *cr0 &= ~SPI_PL022_SSPCR0_SCR_MSK;
            *cr0 |= (scr << SPI_PL022_SSPCR0_SCR_POS);
            return clkps_dvsr;
        }
    }

    /* no good value was found */
    *cr0 &= ~SPI_PL022_SSPCR0_SCR_MSK;
    return SPI_PL022_INVALID_SSPCPSR_VALUE;
}

void spi_pl022_dev_enable(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                              (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    p_spi->sspcr1 |= SPI_PL022_SSPCR1_SSE_MSK;
}

void spi_pl022_dev_disable(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                              (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    p_spi->sspcr1 &= ~SPI_PL022_SSPCR1_SSE_MSK;
}

uint32_t spi_pl022_get_status(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    return p_spi->sspsr;
}

/*
 * \brief Configures the SPI PL022 device.
 *
 * \param[in] spi_dev   Pointer to SPI memory map \ref spi_pl022_dev_reg_map_t
 * \param[in] ctrl_cfg  SPI control configuration \ref spi_pl022_ctrl_cfg_t
 * \param[in] sys_clk   System clock.
 *
 * \return Error code from \ref spi_pl022_error_t
 */
static enum spi_pl022_error_t spi_set_configuration(
                                    struct spi_pl022_dev_reg_map_t* spi_dev,
                                    const struct spi_pl022_ctrl_cfg_t* ctrl_cfg,
                                    uint32_t sys_clk)
{
    uint32_t tmp_cr0, tmp_cr1;
    uint32_t clk_dvsr;

    if(!sys_clk || !ctrl_cfg->bit_rate) {
        return SPI_PL022_ERR_INVALID_ARGS;
    }

    /* Word size */
    tmp_cr0 = ((ctrl_cfg->word_size -1) << SPI_PL022_SSPCR0_DSS_POS)
              & SPI_PL022_SSPCR0_DSS_MSK;

    /* Frame format is stored in the least 2 bits*/
    switch(ctrl_cfg->frame_format & 0x3ul)
    {
    case SPI_PL022_CFG_FRF_MOT:
        tmp_cr0 |= (SPI_PL022_SSPCR0_FRF_MOT_SPI << SPI_PL022_SSPCR0_FRF_POS)
                   & SPI_PL022_SSPCR0_FRF_MSK;
        /* Add motorola phase & polarity */
        tmp_cr0 |= (SPI_PL022_SSPCR0_SPO_MSK & SPI_PL022_SSPCR0_SPH_MSK);
        break;
    case SPI_PL022_CFG_FRF_TI:
        tmp_cr0 |= (SPI_PL022_SSPCR0_FRF_TI_SS << SPI_PL022_SSPCR0_FRF_POS)
                   & SPI_PL022_SSPCR0_FRF_MSK;
        break;
    case SPI_PL022_CFG_FRF_MICROWIRE:
        tmp_cr0 |= (SPI_PL022_SSPCR0_FRF_MICROWIRE << SPI_PL022_SSPCR0_FRF_POS)
                   & SPI_PL022_SSPCR0_FRF_MSK;
        break;
    default:
        return SPI_PL022_ERR_BAD_CONFIG;
        /* break; */
    }

    /* Clock logic */
    clk_dvsr = spi_calc_clock_rate(ctrl_cfg, sys_clk, &tmp_cr0);

    if (SPI_PL022_INVALID_SSPCPSR_VALUE == clk_dvsr) {
        return SPI_PL022_ERR_BAD_CONFIG;
    }

    /* Enable device and set configured mode */
    tmp_cr1 = (0x1 << SPI_PL022_SSPCR1_SSE_POS) & SPI_PL022_SSPCR1_SSE_MSK;
    tmp_cr1 |= ((ctrl_cfg->spi_mode << SPI_PL022_SSPCR1_MS_POS)
                                                    & SPI_PL022_SSPCR1_MS_MSK);


    /* Start initialization by disabling the device */
    spi_dev->sspcr1 = 0;

    /* Set the value received for the configuration */
    spi_dev->sspcpsr = clk_dvsr;
    spi_dev->sspcr0  = tmp_cr0;

    /* Default setup hard coded */
    spi_dev->sspimsc  = 0;
    spi_dev->sspdmacr = 0;
    spi_dev->sspicr = (SPI_PL022_SSPICR_RORIC_MSK | SPI_PL022_SSPICR_RTIC_MSK);

    spi_dev->sspcr1 = tmp_cr1;

    return SPI_PL022_ERR_NONE;
}

enum spi_pl022_error_t spi_pl022_init(struct spi_pl022_dev_t* dev,
                                      uint32_t sys_clk)
{
    enum spi_pl022_error_t ret;
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    ret = spi_set_configuration(p_spi, &dev->cfg->default_ctrl_cfg, sys_clk);

    if(ret != SPI_PL022_ERR_NONE) {
        return ret;
    }

    dev->data->sys_clk = sys_clk;

    /* Initilizes current SPI control configuration */
    memcpy(&dev->data->ctrl_cfg, &dev->cfg->default_ctrl_cfg,
           sizeof(struct spi_pl022_ctrl_cfg_t));

    dev->data->state = SPI_PL022_INITIALIZED;

    return ret;
}

enum spi_pl022_error_t spi_pl022_set_ctrl_cfg(struct spi_pl022_dev_t* dev,
                                    const struct spi_pl022_ctrl_cfg_t* ctrl_cfg)
{
    enum spi_pl022_error_t ret;
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    if(!(dev->data->state & SPI_PL022_INITIALIZED)) {
        return SPI_PL022_ERR_NOT_INIT;
    }

    if(ctrl_cfg == NULL) {
        return SPI_PL022_ERR_INVALID_ARGS;
    }

    ret = spi_set_configuration(p_spi, ctrl_cfg, dev->data->sys_clk);

    if(ret != SPI_PL022_ERR_NONE) {
        return ret;
    }

    /* Updates current SPI control configuration */
    memcpy(&dev->data->ctrl_cfg, ctrl_cfg,
           sizeof(struct spi_pl022_ctrl_cfg_t));

    return SPI_PL022_ERR_NONE;
}

enum spi_pl022_error_t spi_pl022_get_ctrl_cfg(struct spi_pl022_dev_t* dev,
                                          struct spi_pl022_ctrl_cfg_t* ctrl_cfg)
{
    if(!(dev->data->state & SPI_PL022_INITIALIZED)) {
        return SPI_PL022_ERR_NOT_INIT;
    }

    if(ctrl_cfg == NULL) {
        return SPI_PL022_ERR_INVALID_ARGS;
    }

    /* Copy current SPI control configuration */
    memcpy(ctrl_cfg, &dev->data->ctrl_cfg,
           sizeof(struct spi_pl022_ctrl_cfg_t));

    return SPI_PL022_ERR_NONE;
}

void spi_pl022_select_mode(struct spi_pl022_dev_t* dev,
                           enum spi_pl022_mode_select_t mode)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                             (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;


    /* Disable device */
    p_spi->sspcr1 &= ~SPI_PL022_SSPCR1_SSE_MSK;
    /* Set mode */
    p_spi->sspcr1 = (p_spi->sspcr1 & ~SPI_PL022_SSPCR1_MS_MSK)
                                           | (mode << SPI_PL022_SSPCR1_MS_POS);
    dev->data->ctrl_cfg.spi_mode = mode;
    /* Re-enable device */
    p_spi->sspcr1 |= SPI_PL022_SSPCR1_SSE_MSK;
}

void spi_pl022_set_slave_output(struct spi_pl022_dev_t* dev,
                                enum spi_pl022_slave_output_mode_t mode)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    p_spi->sspcr1 = (p_spi->sspcr1 & ~SPI_PL022_SSPCR1_SOD_MSK)
                                          | (mode << SPI_PL022_SSPCR1_SOD_POS);
}

void spi_pl022_set_loopback_mode(struct spi_pl022_dev_t* dev,
                                 enum spi_pl022_loopback_select_t mode)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    p_spi->sspcr1 = (p_spi->sspcr1 & ~SPI_PL022_SSPCR1_LBM_MSK)
                                          | (mode << SPI_PL022_SSPCR1_LBM_POS);
}

void spi_pl022_enable_interrupt(struct spi_pl022_dev_t* dev,
                                uint32_t irq_mask)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                              (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    p_spi->sspimsc |= (irq_mask & SPI_PL022_SSPIMSC_VALID_MSK);
}

void spi_pl022_disable_interrupt(struct spi_pl022_dev_t* dev,
                                 uint32_t irq_mask)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                              (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    p_spi->sspimsc &= ~(irq_mask & SPI_PL022_SSPIMSC_VALID_MSK);
}

uint32_t spi_pl022_get_raw_irq_status(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                              (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    return (p_spi->sspris & SPI_PL022_SSPRIS_VALID_MSK);
}

uint32_t spi_pl022_get_masked_irq_status(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                              (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    return (p_spi->sspmis & SPI_PL022_SSPMIS_VALID_MSK);
}

void spi_pl022_clear_interrupt(struct spi_pl022_dev_t* dev,
                               uint32_t irq_mask)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                              (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    p_spi->sspicr = (irq_mask & SPI_PL022_SSPICR_VALID_MSK);
}

void spi_pl022_dma_mode_enable(struct spi_pl022_dev_t* dev,
                               uint32_t dma)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    p_spi->sspdmacr |= (dma & SPI_PL022_SSPDMACR_VALID_MSK);
}

void spi_pl022_dma_mode_disable(struct spi_pl022_dev_t* dev,
                                uint32_t dma)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    p_spi->sspdmacr &= ~(dma & SPI_PL022_SSPDMACR_VALID_MSK);
}

void spi_pl022_get_periphID(struct spi_pl022_dev_t* dev,
                            struct spi_pl022_periphid_t* periphid)
{
    uint32_t tempid, tempid2;
    struct spi_pl022_dev_reg_map_t* p_spi =
                                   (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    tempid = p_spi->sspperiphid0 & SPI_PL022_SSPPERIPH_ID0_PARTNO_0_MSK;
    tempid2 = (p_spi->sspperiphid1 & SPI_PL022_SSPPERIPH_ID1_PARTNO_1_MSK)
                                    << SPI_PL022_SSPPERIPH_ID0_PARTNO_0_SIZE;
    periphid->partNumber = tempid | tempid2;

    tempid =  (p_spi->sspperiphid1 & SPI_PL022_SSPPERIPH_ID1_DESIGNER_0_MSK)
                                    >> SPI_PL022_SSPPERIPH_ID1_DESIGNER_0_POS;
    tempid2 = (p_spi->sspperiphid2 & SPI_PL022_SSPPERIPH_ID2_DESIGNER_1_MSK)
                                    << SPI_PL022_SSPPERIPH_ID1_DESIGNER_0_SIZE;
    periphid->designerID = tempid | tempid2;

    tempid = (p_spi->sspperiphid2 & SPI_PL022_SSPPERIPH_ID2_REVIISON_MSK)
                                    >> SPI_PL022_SSPPERIPH_ID2_REVISION_SIZE;
    periphid->revision = tempid;

    tempid = p_spi->sspperiphid3 & SPI_PL022_SSPPERIPH_ID3_CONFIG_MSK;
    periphid->configuration = tempid;
}

void spi_pl022_get_PrimeCell_ID(struct spi_pl022_dev_t* dev,
                          struct spi_pl022_primecell_id_t* cellid)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                                   (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    cellid->cellid0 = (uint8_t) p_spi->ssppcellid0;
    cellid->cellid1 = (uint8_t) p_spi->ssppcellid1;
    cellid->cellid2 = (uint8_t) p_spi->ssppcellid2;
    cellid->cellid3 = (uint8_t) p_spi->ssppcellid3;
}

enum spi_pl022_error_t spi_pl022_set_sys_clk(struct spi_pl022_dev_t* dev,
                                             uint32_t sys_clk)
{
    uint32_t clk_dvsr;
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    if(!(dev->data->state & SPI_PL022_INITIALIZED)) {
        return SPI_PL022_ERR_NOT_INIT;
    }

    if(!sys_clk) {
        return SPI_PL022_ERR_INVALID_ARGS;
    }

    clk_dvsr = spi_calc_clock_rate(&dev->data->ctrl_cfg, sys_clk, &p_spi->sspcr0);

    if(SPI_PL022_INVALID_SSPCPSR_VALUE == clk_dvsr) {
        return SPI_PL022_ERR_BAD_CONFIG;
    }

    p_spi->sspcpsr = clk_dvsr;
    dev->data->sys_clk = sys_clk;

    return SPI_PL022_ERR_NONE;
}

enum spi_pl022_error_t spi_pl022_read(struct spi_pl022_dev_t* dev,
                                      void *rx_ptr)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    if(p_spi->sspsr & SPI_PL022_SSPSR_RNE_MSK) {
        if(dev->data->ctrl_cfg.word_size <= 8) {
            *(uint8_t*)rx_ptr = p_spi->sspdr & WORD_1BYTE_MASK;
        } else {
            *(uint16_t*)rx_ptr = p_spi->sspdr & WORD_2BYTES_MASK;
        }
        return SPI_PL022_ERR_NONE;
    }
    return SPI_PL022_ERR_NO_RX;
}

uint32_t spi_pl022_slave_read(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;
    uint32_t data;

    if(dev->data->ctrl_cfg.word_size <= 8) {
        data = p_spi->sspdr & WORD_1BYTE_MASK;
    } else {
        data = p_spi->sspdr & WORD_2BYTES_MASK;
    }
    return data;
}

enum spi_pl022_error_t spi_pl022_write(struct spi_pl022_dev_t* dev,
                                       const enum spi_pl022_mode_select_t mode,
                                       const void *tx_ptr)
{
   struct spi_pl022_dev_reg_map_t* p_spi =
                               (struct spi_pl022_dev_reg_map_t*) dev->cfg->base;

    if(p_spi->sspsr & SPI_PL022_SSPSR_TNF_MSK){
        if(dev->data->ctrl_cfg.word_size <= 8) {
            p_spi->sspdr = *(const uint8_t*)tx_ptr;
        } else {
            p_spi->sspdr = *(const uint16_t*)tx_ptr;
        }
        /* Wait for write to go through */
        if (mode == SPI_PL022_MASTER_SELECT) {
            while(p_spi->sspsr & SPI_PL022_SSPSR_BSY_MSK) {};
        }
        return SPI_PL022_ERR_NONE;
    }
    return SPI_PL022_ERR_NO_TX;
}

enum spi_pl022_error_t spi_pl022_txrx_blocking(struct spi_pl022_dev_t* dev,
                                               const void *tx_ptr,
                                               uint32_t *tx_len_ptr,
                                               void *rx_ptr,
                                               uint32_t *rx_len_ptr)
{
    uint32_t i;
    enum spi_pl022_error_t retval = SPI_PL022_ERR_NONE;
    uint8_t word_size = 1;
    uint32_t rx_data, tx_data, total_len;

    if(dev->data->ctrl_cfg.word_size > 8) {
        word_size = 2;
        /* return error if sizes are not word_size aligned */
        if ((*tx_len_ptr & 0x1) || (*rx_len_ptr & 0x1)) {
            return SPI_PL022_ERR_INVALID_ARGS;
        }
    }

    total_len = (*tx_len_ptr > *rx_len_ptr) ? *tx_len_ptr : *rx_len_ptr;

    for(i=0;i<total_len;i+=word_size){
        if (i<*tx_len_ptr) {
            tx_data = *(const uint16_t*)tx_ptr;
        } else {
            /* send FF if there is no more valid data to send */
            tx_data = 0xFFFF;
        }
        retval = spi_pl022_write(dev, dev->data->ctrl_cfg.spi_mode, &tx_data);
        if(retval != SPI_PL022_ERR_NONE) {
            *tx_len_ptr = i;
            *rx_len_ptr = i;
            break;
        }
        if(i < *tx_len_ptr) {
            tx_ptr = (const uint8_t*)tx_ptr + word_size;
        }
        retval = spi_pl022_read(dev, &rx_data);
        if(retval != SPI_PL022_ERR_NONE) {
            /* send went through, align tx_len to the updated tx_ptr */
            *tx_len_ptr = i + word_size;
            /* don't update rx_len if there is an overflow */
            if (i < *rx_len_ptr) {
                *rx_len_ptr = i;
            }
            break;
        }
        /* do not overflow rx buffer */
        if(i<*rx_len_ptr) {
            if (word_size == 1) {
                *(uint8_t*)rx_ptr = (uint8_t) rx_data;
            } else {
                *(uint16_t*)rx_ptr = (uint16_t) rx_data;
            }
            rx_ptr = (uint8_t*)rx_ptr + word_size;
        }
    }

    return retval;
}

/*
 * TEST APIs
 */
void spi_pl022_test_fifo_enable(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_test_reg_map_t* p_spi =
                 (struct spi_pl022_dev_test_reg_map_t*)
                                    (dev->cfg->base + SPI_PL022_TEST_REG_BASE);

    p_spi->ssptcr |= SPI_PL022_SSPTCR_TESTFIFO_MSK;
}

void spi_pl022_test_fifo_disable(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_test_reg_map_t* p_spi =
                 (struct spi_pl022_dev_test_reg_map_t*)
                                    (dev->cfg->base + SPI_PL022_TEST_REG_BASE);

    p_spi->ssptcr &= ~SPI_PL022_SSPTCR_TESTFIFO_MSK;
}

void spi_pl022_integration_test_enable(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_test_reg_map_t* p_spi =
                 (struct spi_pl022_dev_test_reg_map_t*)
                                    (dev->cfg->base + SPI_PL022_TEST_REG_BASE);

    p_spi->ssptcr |= SPI_PL022_SSPTCR_ITEN_MSK;
}

void spi_pl022_integration_test_disable(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_test_reg_map_t* p_spi =
                 (struct spi_pl022_dev_test_reg_map_t*)
                                    (dev->cfg->base + SPI_PL022_TEST_REG_BASE);

    p_spi->ssptcr &= ~SPI_PL022_SSPTCR_ITEN_MSK;
}


void spi_pl022_write_test_data(struct spi_pl022_dev_t* dev,
                                                    void *tx_ptr)
{
    struct spi_pl022_dev_test_reg_map_t* p_spi =
                 (struct spi_pl022_dev_test_reg_map_t*)
                                    (dev->cfg->base + SPI_PL022_TEST_REG_BASE);

    if(dev->data->ctrl_cfg.word_size <= 8) {
        p_spi->ssptdr = *(const uint8_t*)tx_ptr;
    } else {
        p_spi->ssptdr = *(const uint16_t*)tx_ptr;
    }
}

uint32_t spi_pl022_read_test_output_reg(struct spi_pl022_dev_t* dev)
{
    struct spi_pl022_dev_test_reg_map_t* p_spi =
                 (struct spi_pl022_dev_test_reg_map_t*)
                                    (dev->cfg->base + SPI_PL022_TEST_REG_BASE);

    return p_spi->sspitop;
}

