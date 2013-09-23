// SPI pin mappings for LPC11CXX
// This should be included ONLY from spi_api.c

#ifndef SPI_PINMAP_H
#define SPI_PINMAP_H

static const PinMap PinMap_SPI_SCLK[] = {
    {P0_6 , SPI_0, 0x02},
    {P0_10, SPI_0, 0x02},
    {P2_11, SPI_0, 0x01},
    {P2_1 , SPI_1, 0x02},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {P0_9 , SPI_0, 0x01},
    {P2_3 , SPI_1, 0x02},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {P0_8 , SPI_0, 0x01},
    {P2_2 , SPI_1, 0x02},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {P0_2 , SPI_0, 0x01},
    {P2_0 , SPI_1, 0x02},
    {NC   , NC   , 0}
};

#endif

