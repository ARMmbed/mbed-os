/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#if !defined(__FSL_DSPI_FEATURES_H__)
#define __FSL_DSPI_FEATURES_H__

#if defined(CPU_MK10DN512VLK10) || defined(CPU_MK10DN512VMB10) || defined(CPU_MK10DX128VMP5) || defined(CPU_MK10DN128VMP5) || \
    defined(CPU_MK10DX64VMP5) || defined(CPU_MK10DN64VMP5) || defined(CPU_MK10DX32VMP5) || defined(CPU_MK10DN32VMP5) || \
    defined(CPU_MK10DX128VLH5) || defined(CPU_MK10DN128VLH5) || defined(CPU_MK10DX64VLH5) || defined(CPU_MK10DN64VLH5) || \
    defined(CPU_MK10DX32VLH5) || defined(CPU_MK10DN32VLH5) || defined(CPU_MK10DX128VFT5) || defined(CPU_MK10DN128VFT5) || \
    defined(CPU_MK10DX64VFT5) || defined(CPU_MK10DN64VFT5) || defined(CPU_MK10DX32VFT5) || defined(CPU_MK10DN32VFT5) || \
    defined(CPU_MK10DX128VLF5) || defined(CPU_MK10DN128VLF5) || defined(CPU_MK10DX64VLF5) || defined(CPU_MK10DN64VLF5) || \
    defined(CPU_MK10DX32VLF5) || defined(CPU_MK10DN32VLF5) || defined(CPU_MK10DX64VLH7) || defined(CPU_MK10DX128VLH7) || \
    defined(CPU_MK10DX256VLH7) || defined(CPU_MK10DX64VLK7) || defined(CPU_MK10DX128VLK7) || defined(CPU_MK10DX256VLK7) || \
    defined(CPU_MK10DX64VMB7) || defined(CPU_MK10DX128VMB7) || defined(CPU_MK10DX256VMB7) || defined(CPU_MK10DN512ZVLK10) || \
    defined(CPU_MK10DN512ZVMB10) || defined(CPU_MK20DN512VLK10) || defined(CPU_MK20DN512VMB10) || defined(CPU_MK20DX128VMP5) || \
    defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DN64VMP5) || defined(CPU_MK20DX32VMP5) || \
    defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || defined(CPU_MK20DX64VLH5) || \
    defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5) || defined(CPU_MK20DX128VFT5) || \
    defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || defined(CPU_MK20DN64VFT5) || defined(CPU_MK20DX32VFT5) || \
    defined(CPU_MK20DN32VFT5) || defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || defined(CPU_MK20DX64VLF5) || \
    defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5) || defined(CPU_MK20DX64VLH7) || \
    defined(CPU_MK20DX128VLH7) || defined(CPU_MK20DX256VLH7) || defined(CPU_MK20DX64VLK7) || defined(CPU_MK20DX128VLK7) || \
    defined(CPU_MK20DX256VLK7) || defined(CPU_MK20DX64VMB7) || defined(CPU_MK20DX128VMB7) || defined(CPU_MK20DX256VMB7) || \
    defined(CPU_MK20DN512ZVLK10) || defined(CPU_MK20DX256ZVLK10) || defined(CPU_MK20DN512ZVMB10) || defined(CPU_MK20DX256ZVMB10) || \
    defined(CPU_MK22FX512VLH12) || defined(CPU_MK22FN1M0VLH12) || defined(CPU_MK22FX512VLK12) || defined(CPU_MK22FN1M0VLK12) || \
    defined(CPU_MK30DN512VLK10) || defined(CPU_MK30DN512VMB10) || defined(CPU_MK30DX64VLH7) || defined(CPU_MK30DX128VLH7) || \
    defined(CPU_MK30DX256VLH7) || defined(CPU_MK30DX64VLK7) || defined(CPU_MK30DX128VLK7) || defined(CPU_MK30DX256VLK7) || \
    defined(CPU_MK30DX64VMB7) || defined(CPU_MK30DX128VMB7) || defined(CPU_MK30DX256VMB7) || defined(CPU_MK30DN512ZVLK10) || \
    defined(CPU_MK30DN512ZVMB10) || defined(CPU_MK40DN512VLK10) || defined(CPU_MK40DN512VMB10) || defined(CPU_MK40DX64VLH7) || \
    defined(CPU_MK40DX128VLH7) || defined(CPU_MK40DX256VLH7) || defined(CPU_MK40DX64VLK7) || defined(CPU_MK40DX128VLK7) || \
    defined(CPU_MK40DX256VLK7) || defined(CPU_MK40DX64VMB7) || defined(CPU_MK40DX128VMB7) || defined(CPU_MK40DX256VMB7) || \
    defined(CPU_MK40DN512ZVLK10) || defined(CPU_MK40DN512ZVMB10) || defined(CPU_MK50DX128CLH7) || defined(CPU_MK50DX256CLK10) || \
    defined(CPU_MK50DX128CLK7) || defined(CPU_MK50DX256CLK7) || defined(CPU_MK50DX256CMB10) || defined(CPU_MK50DX128CMB7) || \
    defined(CPU_MK50DX256CMB7) || defined(CPU_MK50DX256ZCLK10) || defined(CPU_MK50DX256ZCMB10) || defined(CPU_MK51DX128CLH7) || \
    defined(CPU_MK51DX256CLK10) || defined(CPU_MK51DX128CLK7) || defined(CPU_MK51DX256CLK7) || defined(CPU_MK51DX256CMB10) || \
    defined(CPU_MK51DX128CMB7) || defined(CPU_MK51DX256CMB7) || defined(CPU_MK51DX256ZCLK10) || defined(CPU_MK51DX256ZCMB10)
    /* @brief Deserial serial peripheral interface (registers MCR, TCR, CTARn, CTARn_SLAVE, SR, RSER, PUSHR, PUSHR_SLAVE, POPR, TXFRn, RXFRn if non-zero, otherwise C1, S, C2, BR, D, M).*/
    #define FSL_FEATURE_SPI_IS_DSPI (1)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of items.*/
    #define FSL_FEATURE_SPI_FIFO_SIZE (4)
    /* @brief Maximum transfer data width in bits.*/
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (16)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS] on DSPI modules.)*/
    #define FSL_FEATURE_SPI_MAX_CHIP_SELECT_COUNT (6)
    /* @brief Number of chip select pins. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_CHIP_SELECT_COUNTn(x) \
        ((x) == 0 ? (5) : \
        ((x) == 1 ? (3) : (-1)))
    /* @brief Has chip select strobe capability on the PCS5 pin. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_CHIP_SELECT_STROBE (1)
    /* @brief The data register name has postfix (L as low and H as high). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support.*/
    #define FSL_FEATURE_SPI_FEATURE_16BIT_TRANSFERS (1)
#elif defined(CPU_MK10DN512VLL10) || defined(CPU_MK10DX128VLQ10) || defined(CPU_MK10DX256VLQ10) || defined(CPU_MK10DN512VLQ10) || \
    defined(CPU_MK10DN512VMC10) || defined(CPU_MK10DX128VMD10) || defined(CPU_MK10DX256VMD10) || defined(CPU_MK10DN512VMD10) || \
    defined(CPU_MK10DX128VLL7) || defined(CPU_MK10DX256VLL7) || defined(CPU_MK10DX128VML7) || defined(CPU_MK10DX256VML7) || \
    defined(CPU_MK10FN1M0VLQ12) || defined(CPU_MK10FX512VLQ12) || defined(CPU_MK10FN1M0VMD12) || defined(CPU_MK10FX512VMD12) || \
    defined(CPU_MK10DN512ZVLL10) || defined(CPU_MK10DN512ZVLQ10) || defined(CPU_MK10DX256ZVLQ10) || defined(CPU_MK10DX128ZVLQ10) || \
    defined(CPU_MK10DN512ZVMC10) || defined(CPU_MK10DN512ZVMD10) || defined(CPU_MK10DX256ZVMD10) || defined(CPU_MK10DX128ZVMD10) || \
    defined(CPU_MK20DN512VLL10) || defined(CPU_MK20DX128VLQ10) || defined(CPU_MK20DX256VLQ10) || defined(CPU_MK20DN512VLQ10) || \
    defined(CPU_MK20DX256VMC10) || defined(CPU_MK20DN512VMC10) || defined(CPU_MK20DX128VMD10) || defined(CPU_MK20DX256VMD10) || \
    defined(CPU_MK20DN512VMD10) || defined(CPU_MK20DX128VLL7) || defined(CPU_MK20DX256VLL7) || defined(CPU_MK20DX128VML7) || \
    defined(CPU_MK20DX256VML7) || defined(CPU_MK20FN1M0VLQ12) || defined(CPU_MK20FX512VLQ12) || defined(CPU_MK20FN1M0VMD12) || \
    defined(CPU_MK20FX512VMD12) || defined(CPU_MK20DN512ZVLL10) || defined(CPU_MK20DX256ZVLL10) || defined(CPU_MK20DN512ZVLQ10) || \
    defined(CPU_MK20DX256ZVLQ10) || defined(CPU_MK20DX128ZVLQ10) || defined(CPU_MK20DN512ZVMC10) || defined(CPU_MK20DX256ZVMC10) || \
    defined(CPU_MK20DN512ZVMD10) || defined(CPU_MK20DX256ZVMD10) || defined(CPU_MK20DX128ZVMD10) || defined(CPU_MK21FX512VLQ12) || \
    defined(CPU_MK21FN1M0VLQ12) || defined(CPU_MK21FX512VLQ12WS) || defined(CPU_MK21FN1M0VLQ12WS) || defined(CPU_MK21FX512VMC12) || \
    defined(CPU_MK21FN1M0VMC12) || defined(CPU_MK21FX512VMC12WS) || defined(CPU_MK21FN1M0VMC12WS) || defined(CPU_MK21FX512VMD12) || \
    defined(CPU_MK21FN1M0VMD12) || defined(CPU_MK21FX512VMD12WS) || defined(CPU_MK21FN1M0VMD12WS) || defined(CPU_MK22FX512VLL12) || \
    defined(CPU_MK22FN1M0VLL12) || defined(CPU_MK22FX512VLQ12) || defined(CPU_MK22FN1M0VLQ12) || defined(CPU_MK22FX512VMC12) || \
    defined(CPU_MK22FN1M0VMC12) || defined(CPU_MK22FX512VMD12) || defined(CPU_MK22FN1M0VMD12) || defined(CPU_MK30DN512VLL10) || \
    defined(CPU_MK30DX128VLQ10) || defined(CPU_MK30DX256VLQ10) || defined(CPU_MK30DN512VLQ10) || defined(CPU_MK30DN512VMC10) || \
    defined(CPU_MK30DX128VMD10) || defined(CPU_MK30DX256VMD10) || defined(CPU_MK30DN512VMD10) || defined(CPU_MK30DX128VLL7) || \
    defined(CPU_MK30DX256VLL7) || defined(CPU_MK30DX128VML7) || defined(CPU_MK30DX256VML7) || defined(CPU_MK30DN512ZVLL10) || \
    defined(CPU_MK30DN512ZVLQ10) || defined(CPU_MK30DX256ZVLQ10) || defined(CPU_MK30DX128ZVLQ10) || defined(CPU_MK30DN512ZVMC10) || \
    defined(CPU_MK30DN512ZVMD10) || defined(CPU_MK30DX256ZVMD10) || defined(CPU_MK30DX128ZVMD10) || defined(CPU_MK40DN512VLL10) || \
    defined(CPU_MK40DX128VLQ10) || defined(CPU_MK40DX256VLQ10) || defined(CPU_MK40DN512VLQ10) || defined(CPU_MK40DN512VMC10) || \
    defined(CPU_MK40DX128VMD10) || defined(CPU_MK40DX256VMD10) || defined(CPU_MK40DN512VMD10) || defined(CPU_MK40DX128VLL7) || \
    defined(CPU_MK40DX256VLL7) || defined(CPU_MK40DX128VML7) || defined(CPU_MK40DX256VML7) || defined(CPU_MK40DN512ZVLL10) || \
    defined(CPU_MK40DN512ZVLQ10) || defined(CPU_MK40DX256ZVLQ10) || defined(CPU_MK40DX128ZVLQ10) || defined(CPU_MK40DN512ZVMC10) || \
    defined(CPU_MK40DN512ZVMD10) || defined(CPU_MK40DX256ZVMD10) || defined(CPU_MK40DX128ZVMD10) || defined(CPU_MK50DX256CLL10) || \
    defined(CPU_MK50DN512CLL10) || defined(CPU_MK50DN512CLQ10) || defined(CPU_MK50DX256CMC10) || defined(CPU_MK50DN512CMC10) || \
    defined(CPU_MK50DN512CMD10) || defined(CPU_MK50DX256CMD10) || defined(CPU_MK50DX256CLL7) || defined(CPU_MK50DX256CML7) || \
    defined(CPU_MK50DN512ZCLL10) || defined(CPU_MK50DX256ZCLL10) || defined(CPU_MK50DN512ZCLQ10) || defined(CPU_MK50DN512ZCMC10) || \
    defined(CPU_MK50DX256ZCMC10) || defined(CPU_MK50DN512ZCMD10) || defined(CPU_MK51DX256CLL10) || defined(CPU_MK51DN512CLL10) || \
    defined(CPU_MK51DN256CLQ10) || defined(CPU_MK51DN512CLQ10) || defined(CPU_MK51DX256CMC10) || defined(CPU_MK51DN512CMC10) || \
    defined(CPU_MK51DN256CMD10) || defined(CPU_MK51DN512CMD10) || defined(CPU_MK51DX256CLL7) || defined(CPU_MK51DX256CML7) || \
    defined(CPU_MK51DN512ZCLL10) || defined(CPU_MK51DX256ZCLL10) || defined(CPU_MK51DN512ZCLQ10) || defined(CPU_MK51DN256ZCLQ10) || \
    defined(CPU_MK51DN512ZCMC10) || defined(CPU_MK51DX256ZCMC10) || defined(CPU_MK51DN512ZCMD10) || defined(CPU_MK51DN256ZCMD10) || \
    defined(CPU_MK52DN512CLQ10) || defined(CPU_MK52DN512CMD10) || defined(CPU_MK52DN512ZCLQ10) || defined(CPU_MK52DN512ZCMD10) || \
    defined(CPU_MK53DN512CLQ10) || defined(CPU_MK53DX256CLQ10) || defined(CPU_MK53DN512CMD10) || defined(CPU_MK53DX256CMD10) || \
    defined(CPU_MK53DN512ZCLQ10) || defined(CPU_MK53DX256ZCLQ10) || defined(CPU_MK53DN512ZCMD10) || defined(CPU_MK53DX256ZCMD10) || \
    defined(CPU_MK60DN256VLL10) || defined(CPU_MK60DX256VLL10) || defined(CPU_MK60DN512VLL10) || defined(CPU_MK60DN256VLQ10) || \
    defined(CPU_MK60DX256VLQ10) || defined(CPU_MK60DN512VLQ10) || defined(CPU_MK60DN256VMC10) || defined(CPU_MK60DX256VMC10) || \
    defined(CPU_MK60DN512VMC10) || defined(CPU_MK60DN256VMD10) || defined(CPU_MK60DX256VMD10) || defined(CPU_MK60DN512VMD10) || \
    defined(CPU_MK60FN1M0VLQ12) || defined(CPU_MK60FX512VLQ12) || defined(CPU_MK60FN1M0VLQ15) || defined(CPU_MK60FX512VLQ15) || \
    defined(CPU_MK60FN1M0VMD12) || defined(CPU_MK60FX512VMD12) || defined(CPU_MK60FN1M0VMD15) || defined(CPU_MK60FX512VMD15) || \
    defined(CPU_MK60DN512ZVLL10) || defined(CPU_MK60DX256ZVLL10) || defined(CPU_MK60DN256ZVLL10) || defined(CPU_MK60DN512ZVLQ10) || \
    defined(CPU_MK60DX256ZVLQ10) || defined(CPU_MK60DN256ZVLQ10) || defined(CPU_MK60DN512ZVMC10) || defined(CPU_MK60DX256ZVMC10) || \
    defined(CPU_MK60DN256ZVMC10) || defined(CPU_MK60DN512ZVMD10) || defined(CPU_MK60DX256ZVMD10) || defined(CPU_MK60DN256ZVMD10) || \
    defined(CPU_MK61FN1M0VMD12) || defined(CPU_MK61FX512VMD12) || defined(CPU_MK61FN1M0VMD15) || defined(CPU_MK61FX512VMD15) || \
    defined(CPU_MK61FN1M0VMD12WS) || defined(CPU_MK61FX512VMD12WS) || defined(CPU_MK61FN1M0VMD15WS) || defined(CPU_MK61FX512VMD15WS) || \
    defined(CPU_MK61FN1M0VMF12) || defined(CPU_MK61FX512VMF12) || defined(CPU_MK61FN1M0VMF15) || defined(CPU_MK61FX512VMF15) || \
    defined(CPU_MK61FN1M0VMJ12) || defined(CPU_MK61FX512VMJ12) || defined(CPU_MK61FN1M0VMJ15) || defined(CPU_MK61FX512VMJ15) || \
    defined(CPU_MK61FN1M0VMJ12WS) || defined(CPU_MK61FX512VMJ12WS) || defined(CPU_MK61FN1M0VMJ15WS) || defined(CPU_MK61FX512VMJ15WS) || \
    defined(CPU_MK63FN1M0VMD12) || defined(CPU_MK63FN1M0VMD12WS) || defined(CPU_MK64FN1M0VMD12) || defined(CPU_MK64FX512VMD12) || \
    defined(CPU_MK70FN1M0VMF12) || defined(CPU_MK70FX512VMF12) || defined(CPU_MK70FN1M0VMF15) || defined(CPU_MK70FX512VMF15) || \
    defined(CPU_MK70FN1M0VMJ12) || defined(CPU_MK70FX512VMJ12) || defined(CPU_MK70FN1M0VMJ15) || defined(CPU_MK70FX512VMJ15) || \
    defined(CPU_MK70FN1M0VMJ12WS) || defined(CPU_MK70FX512VMJ12WS) || defined(CPU_MK70FN1M0VMJ15WS) || defined(CPU_MK70FX512VMJ15WS)
    /* @brief Deserial serial peripheral interface (registers MCR, TCR, CTARn, CTARn_SLAVE, SR, RSER, PUSHR, PUSHR_SLAVE, POPR, TXFRn, RXFRn if non-zero, otherwise C1, S, C2, BR, D, M).*/
    #define FSL_FEATURE_SPI_IS_DSPI (1)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of items.*/
#if defined(CPU_MK64FN1M0VMD12) || defined(CPU_MK64FX512VMD12)  
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (4) : \
        ((x) == 1 ? (1) : \
        ((x) == 2 ? (1) : (-1))))
#else
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (4) : \
        ((x) == 1 ? (4) : \
        ((x) == 2 ? (4) : (-1))))
#endif
    /* @brief Maximum transfer data width in bits.*/
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (16)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS] on DSPI modules.)*/
    #define FSL_FEATURE_SPI_MAX_CHIP_SELECT_COUNT (6)
    /* @brief Number of chip select pins. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_CHIP_SELECT_COUNTn(x) \
        ((x) == 0 ? (6) : \
        ((x) == 1 ? (4) : \
        ((x) == 2 ? (1) : (-1))))
    /* @brief Has chip select strobe capability on the PCS5 pin. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_CHIP_SELECT_STROBE (1)
    /* @brief The data register name has postfix (L as low and H as high). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support.*/
    #define FSL_FEATURE_SPI_FEATURE_16BIT_TRANSFERS (1)
#elif defined(CPU_MK10DX128VFM5) || defined(CPU_MK10DN128VFM5) || defined(CPU_MK10DX64VFM5) || defined(CPU_MK10DN64VFM5) || \
    defined(CPU_MK10DX32VFM5) || defined(CPU_MK10DN32VFM5) || defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || \
    defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DN64VFM5) || defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5)
    /* @brief Deserial serial peripheral interface (registers MCR, TCR, CTARn, CTARn_SLAVE, SR, RSER, PUSHR, PUSHR_SLAVE, POPR, TXFRn, RXFRn if non-zero, otherwise C1, S, C2, BR, D, M).*/
    #define FSL_FEATURE_SPI_IS_DSPI (1)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of items.*/
    #define FSL_FEATURE_SPI_FIFO_SIZE (4)
    /* @brief Maximum transfer data width in bits.*/
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (16)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS] on DSPI modules.)*/
    #define FSL_FEATURE_SPI_MAX_CHIP_SELECT_COUNT (6)
    /* @brief Number of chip select pins. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_CHIP_SELECT_COUNT (4)
    /* @brief Has chip select strobe capability on the PCS5 pin. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_CHIP_SELECT_STROBE (1)
    /* @brief The data register name has postfix (L as low and H as high). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support.*/
    #define FSL_FEATURE_SPI_FEATURE_16BIT_TRANSFERS (1)
#elif defined(CPU_MK11DX128VLK5) || defined(CPU_MK11DX256VLK5) || defined(CPU_MK11DN512VLK5) || defined(CPU_MK11DX128VLK5WS) || \
    defined(CPU_MK11DX256VLK5WS) || defined(CPU_MK11DN512VLK5WS) || defined(CPU_MK11DX128VMC5) || defined(CPU_MK11DX256VMC5) || \
    defined(CPU_MK11DN512VMC5) || defined(CPU_MK11DX128VMC5WS) || defined(CPU_MK11DX256VMC5WS) || defined(CPU_MK11DN512VMC5WS) || \
    defined(CPU_MK12DX128VLH5) || defined(CPU_MK12DX256VLH5) || defined(CPU_MK12DN512VLH5) || defined(CPU_MK12DX128VLK5) || \
    defined(CPU_MK12DX256VLK5) || defined(CPU_MK12DN512VLK5) || defined(CPU_MK12DX128VMC5) || defined(CPU_MK12DX256VMC5) || \
    defined(CPU_MK12DN512VMC5) || defined(CPU_MK12DX128VLF5) || defined(CPU_MK12DX256VLF5) || defined(CPU_MK21DX128VLK5) || \
    defined(CPU_MK21DX256VLK5) || defined(CPU_MK21DN512VLK5) || defined(CPU_MK21DX128VLK5WS) || defined(CPU_MK21DX256VLK5WS) || \
    defined(CPU_MK21DN512VLK5WS) || defined(CPU_MK21DX128VMC5) || defined(CPU_MK21DX256VMC5) || defined(CPU_MK21DN512VMC5) || \
    defined(CPU_MK21DX128VMC5WS) || defined(CPU_MK21DX256VMC5WS) || defined(CPU_MK21DN512VMC5WS) || defined(CPU_MK22DX128VLH5) || \
    defined(CPU_MK22DX256VLH5) || defined(CPU_MK22DN512VLH5) || defined(CPU_MK22DX128VLK5) || defined(CPU_MK22DX256VLK5) || \
    defined(CPU_MK22DN512VLK5) || defined(CPU_MK22DX128VMC5) || defined(CPU_MK22DX256VMC5) || defined(CPU_MK22DN512VMC5) || \
    defined(CPU_MK22DX128VLF5) || defined(CPU_MK22DX256VLF5)
    /* @brief Deserial serial peripheral interface (registers MCR, TCR, CTARn, CTARn_SLAVE, SR, RSER, PUSHR, PUSHR_SLAVE, POPR, TXFRn, RXFRn if non-zero, otherwise C1, S, C2, BR, D, M).*/
    #define FSL_FEATURE_SPI_IS_DSPI (1)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of items.*/
    #define FSL_FEATURE_SPI_FIFO_SIZE (4)
    /* @brief Maximum transfer data width in bits.*/
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (16)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS] on DSPI modules.)*/
    #define FSL_FEATURE_SPI_MAX_CHIP_SELECT_COUNT (5)
    /* @brief Number of chip select pins. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_CHIP_SELECT_COUNTn(x) \
        ((x) == 0 ? (5) : \
        ((x) == 1 ? (3) : (-1)))
    /* @brief Has chip select strobe capability on the PCS5 pin. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_CHIP_SELECT_STROBE (0)
    /* @brief The data register name has postfix (L as low and H as high). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support.*/
    #define FSL_FEATURE_SPI_FEATURE_16BIT_TRANSFERS (1)
#elif defined(CPU_MK22FN512VDC12)
    /* @brief Deserial serial peripheral interface (registers MCR, TCR, CTARn, CTARn_SLAVE, SR, RSER, PUSHR, PUSHR_SLAVE, POPR, TXFRn, RXFRn if non-zero, otherwise C1, S, C2, BR, D, M).*/
    #define FSL_FEATURE_SPI_IS_DSPI (1)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of items.*/
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (4) : \
        ((x) == 1 ? (1) : (-1)))
    /* @brief Maximum transfer data width in bits.*/
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (16)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS] on DSPI modules.)*/
    #define FSL_FEATURE_SPI_MAX_CHIP_SELECT_COUNT (6)
    /* @brief Number of chip select pins. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_CHIP_SELECT_COUNT (0)
    /* @brief Has chip select strobe capability on the PCS5 pin. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_CHIP_SELECT_STROBE (1)
    /* @brief The data register name has postfix (L as low and H as high). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support.*/
    #define FSL_FEATURE_SPI_FEATURE_16BIT_TRANSFERS (1)
#elif defined(CPU_MK22FN512VDC12)
    /* @brief Deserial serial peripheral interface (registers MCR, TCR, CTARn, CTARn_SLAVE, SR, RSER, PUSHR, PUSHR_SLAVE, POPR, TXFRn, RXFRn if non-zero, otherwise C1, S, C2, BR, D, M).*/
    #define FSL_FEATURE_SPI_IS_DSPI (1)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of items.*/
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (4) : \
        ((x) == 1 ? (1) : (-1)))
    /* @brief Maximum transfer data width in bits.*/
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (16)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS] on DSPI modules.)*/
    #define FSL_FEATURE_SPI_MAX_CHIP_SELECT_COUNT (6)
    /* @brief Number of chip select pins. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_CHIP_SELECT_COUNTn(x) \
        ((x) == 0 ? (5) : \
        ((x) == 1 ? (3) : (-1)))
    /* @brief Has chip select strobe capability on the PCS5 pin. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_CHIP_SELECT_STROBE (1)
    /* @brief The data register name has postfix (L as low and H as high). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support.*/
    #define FSL_FEATURE_SPI_FEATURE_16BIT_TRANSFERS (1)
#elif defined(CPU_MKE02Z64VLC2) || defined(CPU_MKE02Z32VLC2) || defined(CPU_MKE02Z16VLC2) || defined(CPU_MKE02Z64VLD2) || \
    defined(CPU_MKE02Z32VLD2) || defined(CPU_MKE02Z16VLD2) || defined(CPU_MKE02Z64VLH2) || defined(CPU_MKE02Z64VQH2) || \
    defined(CPU_MKE02Z32VLH2) || defined(CPU_MKE02Z32VQH2) || defined(CPU_MKE04Z8VFK4) || defined(CPU_MKE04Z8VTG4) || \
    defined(CPU_MKE04Z8VWJ4) || defined(CPU_MKL02Z32CAF4) || defined(CPU_MKL02Z8VFG4) || defined(CPU_MKL02Z16VFG4) || \
    defined(CPU_MKL02Z32VFG4) || defined(CPU_MKL02Z16VFK4) || defined(CPU_MKL02Z32VFK4) || defined(CPU_MKL02Z16VFM4) || \
    defined(CPU_MKL02Z32VFM4)
    /* @brief Deserial serial peripheral interface (registers MCR, TCR, CTARn, CTARn_SLAVE, SR, RSER, PUSHR, PUSHR_SLAVE, POPR, TXFRn, RXFRn if non-zero, otherwise C1, S, C2, BR, D, M).*/
    #define FSL_FEATURE_SPI_IS_DSPI (0)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (0)
    /* @brief Receive/transmit FIFO size in number of items.*/
    #define FSL_FEATURE_SPI_FIFO_SIZE (1)
    /* @brief Maximum transfer data width in bits.*/
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (8)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS] on DSPI modules.)*/
    #define FSL_FEATURE_SPI_MAX_CHIP_SELECT_COUNT (1)
    /* @brief Number of chip select pins. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_CHIP_SELECT_COUNT (0)
    /* @brief Has chip select strobe capability on the PCS5 pin. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_CHIP_SELECT_STROBE (0)
    /* @brief The data register name has postfix (L as low and H as high). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (1)
    /* @brief Has 16-bit data transfer support.*/
    #define FSL_FEATURE_SPI_16BIT_TRANSFERS (0)
#elif defined(CPU_MKL04Z8VFK4) || defined(CPU_MKL04Z16VFK4) || defined(CPU_MKL04Z32VFK4) || defined(CPU_MKL04Z8VLC4) || \
    defined(CPU_MKL04Z16VLC4) || defined(CPU_MKL04Z32VLC4) || defined(CPU_MKL04Z8VFM4) || defined(CPU_MKL04Z16VFM4) || \
    defined(CPU_MKL04Z32VFM4) || defined(CPU_MKL04Z16VLF4) || defined(CPU_MKL04Z32VLF4) || defined(CPU_MKL05Z8VFK4) || \
    defined(CPU_MKL05Z16VFK4) || defined(CPU_MKL05Z32VFK4) || defined(CPU_MKL05Z8VLC4) || defined(CPU_MKL05Z16VLC4) || \
    defined(CPU_MKL05Z32VLC4) || defined(CPU_MKL05Z8VFM4) || defined(CPU_MKL05Z16VFM4) || defined(CPU_MKL05Z32VFM4) || \
    defined(CPU_MKL05Z16VLF4) || defined(CPU_MKL05Z32VLF4) || defined(CPU_MKL14Z32VFM4) || defined(CPU_MKL14Z64VFM4) || \
    defined(CPU_MKL14Z32VFT4) || defined(CPU_MKL14Z64VFT4) || defined(CPU_MKL14Z32VLH4) || defined(CPU_MKL14Z64VLH4) || \
    defined(CPU_MKL14Z32VLK4) || defined(CPU_MKL14Z64VLK4) || defined(CPU_MKL15Z32VFM4) || defined(CPU_MKL15Z64VFM4) || \
    defined(CPU_MKL15Z128VFM4) || defined(CPU_MKL15Z32VFT4) || defined(CPU_MKL15Z64VFT4) || defined(CPU_MKL15Z128VFT4) || \
    defined(CPU_MKL15Z32VLH4) || defined(CPU_MKL15Z64VLH4) || defined(CPU_MKL15Z128VLH4) || defined(CPU_MKL15Z32VLK4) || \
    defined(CPU_MKL15Z64VLK4) || defined(CPU_MKL15Z128VLK4) || defined(CPU_MKL24Z32VFM4) || defined(CPU_MKL24Z64VFM4) || \
    defined(CPU_MKL24Z32VFT4) || defined(CPU_MKL24Z64VFT4) || defined(CPU_MKL24Z32VLH4) || defined(CPU_MKL24Z64VLH4) || \
    defined(CPU_MKL24Z32VLK4) || defined(CPU_MKL24Z64VLK4) || defined(CPU_MKL25Z32VFM4) || defined(CPU_MKL25Z64VFM4) || \
    defined(CPU_MKL25Z128VFM4) || defined(CPU_MKL25Z32VFT4) || defined(CPU_MKL25Z64VFT4) || defined(CPU_MKL25Z128VFT4) || \
    defined(CPU_MKL25Z32VLH4) || defined(CPU_MKL25Z64VLH4) || defined(CPU_MKL25Z128VLH4) || defined(CPU_MKL25Z32VLK4) || \
    defined(CPU_MKL25Z64VLK4) || defined(CPU_MKL25Z128VLK4)
    /* @brief Deserial serial peripheral interface (registers MCR, TCR, CTARn, CTARn_SLAVE, SR, RSER, PUSHR, PUSHR_SLAVE, POPR, TXFRn, RXFRn if non-zero, otherwise C1, S, C2, BR, D, M).*/
    #define FSL_FEATURE_SPI_IS_DSPI (0)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of items.*/
    #define FSL_FEATURE_SPI_FIFO_SIZE (1)
    /* @brief Maximum transfer data width in bits.*/
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (8)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS] on DSPI modules.)*/
    #define FSL_FEATURE_SPI_MAX_CHIP_SELECT_COUNT (1)
    /* @brief Number of chip select pins. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_CHIP_SELECT_COUNT (0)
    /* @brief Has chip select strobe capability on the PCS5 pin. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_CHIP_SELECT_STROBE (0)
    /* @brief The data register name has postfix (L as low and H as high). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (1)
    /* @brief Has 16-bit data transfer support.*/
    #define FSL_FEATURE_SPI_16BIT_TRANSFERS (0)
#elif defined(CPU_MKL16Z32VFM4) || defined(CPU_MKL16Z64VFM4) || defined(CPU_MKL16Z128VFM4) || defined(CPU_MKL16Z32VFT4) || \
    defined(CPU_MKL16Z64VFT4) || defined(CPU_MKL16Z128VFT4) || defined(CPU_MKL16Z32VLH4) || defined(CPU_MKL16Z64VLH4) || \
    defined(CPU_MKL16Z128VLH4) || defined(CPU_MKL16Z256VLH4) || defined(CPU_MKL16Z256VLK4) || defined(CPU_MKL26Z32VFM4) || \
    defined(CPU_MKL26Z64VFM4) || defined(CPU_MKL26Z128VFM4) || defined(CPU_MKL26Z32VFT4) || defined(CPU_MKL26Z64VFT4) || \
    defined(CPU_MKL26Z128VFT4) || defined(CPU_MKL26Z32VLH4) || defined(CPU_MKL26Z64VLH4) || defined(CPU_MKL26Z128VLH4) || \
    defined(CPU_MKL26Z256VLH4) || defined(CPU_MKL26Z256VLK4) || defined(CPU_MKL26Z128VLL4) || defined(CPU_MKL26Z256VLL4) || \
    defined(CPU_MKL26Z128VMC4) || defined(CPU_MKL26Z256VMC4) || defined(CPU_MKL34Z64VLH4) || defined(CPU_MKL34Z64VLL4) || \
    defined(CPU_MKL36Z64VLH4) || defined(CPU_MKL36Z128VLH4) || defined(CPU_MKL36Z256VLH4) || defined(CPU_MKL36Z64VLL4) || \
    defined(CPU_MKL36Z128VLL4) || defined(CPU_MKL36Z256VLL4) || defined(CPU_MKL36Z128VMC4) || defined(CPU_MKL36Z256VMC4) || \
    defined(CPU_MKL46Z128VLH4) || defined(CPU_MKL46Z256VLH4) || defined(CPU_MKL46Z128VLL4) || defined(CPU_MKL46Z256VLL4) || \
    defined(CPU_MKL46Z128VMC4) || defined(CPU_MKL46Z256VMC4)
    /* @brief Deserial serial peripheral interface (registers MCR, TCR, CTARn, CTARn_SLAVE, SR, RSER, PUSHR, PUSHR_SLAVE, POPR, TXFRn, RXFRn if non-zero, otherwise C1, S, C2, BR, D, M).*/
    #define FSL_FEATURE_SPI_IS_DSPI (0)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of items.*/
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (1) : \
        ((x) == 1 ? (8) : (-1)))
    /* @brief Maximum transfer data width in bits.*/
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (16)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS] on DSPI modules.)*/
    #define FSL_FEATURE_SPI_MAX_CHIP_SELECT_COUNT (1)
    /* @brief Number of chip select pins. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_CHIP_SELECT_COUNT (0)
    /* @brief Has chip select strobe capability on the PCS5 pin. (Only valid for DSPI modules.)*/
    #define FSL_FEATURE_SPI_HAS_CHIP_SELECT_STROBE (0)
    /* @brief The data register name has postfix (L as low and H as high). (Only valid for non-DSPI modules.)*/
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (1)
    /* @brief Has 16-bit data transfer support.*/
    #define FSL_FEATURE_SPI_16BIT_TRANSFERS (1)
#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_DSPI_FEATURES_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

