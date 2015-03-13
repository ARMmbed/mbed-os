#include <mbed.h>
#include <string.h>
#include "spi_flash.h"

SPI spi(FLASH_SPIMOSI, FLASH_SPIMISO, FLASH_SPICLK); // mosi, miso, sclk
DigitalOut cs(FLASH_SPICS);

#define MFG_ID_WINBOND (0xEF)
#define DEVICE_ID_WINBOND_8M (0x5014)

#define CMD_POWER_UP (0xAB)
#define CMD_JEDEC_ID (0x9F)
#define CMD_READ_STATUS (0x05)
#define CMD_READ_SECU (0x48)
#define CMD_POWER_DOWN (0xB9)
#define CMD_DUMMY (0x00)

#define MAC_LENGTH 6

bool spi_flash_init(void)
{
    bool transfer_succeeded = true;
    
    uint8_t mfgId;
    uint16_t deviceID;    
    
    // Chip must be deselected
    cs = 1;

    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 1MHz clock rate
    spi.format(8,0);
    spi.frequency(1000000);

    // wake up the flash
    cs = 1;
    spi.write(CMD_POWER_UP);
    cs = 0;

    // Wait 30 ms for SPI flash Wakeup
    wait_ms(30);    

    // Send the command to read the mfg ID and device ID
    cs = 0;
    spi.write(CMD_JEDEC_ID);
    
    // Send a dummy byte to receive the contents of the mfgId
    mfgId = spi.write(CMD_DUMMY);
    deviceID = ( spi.write(CMD_DUMMY) << 8);
    deviceID |= spi.write(CMD_DUMMY);
    
    // Deselect the device
    cs = 1;
    
    if (mfgId != MFG_ID_WINBOND || deviceID != DEVICE_ID_WINBOND_8M) {
        transfer_succeeded = false;
    }
    
    return transfer_succeeded;
}

static bool spi_flash_waitBusy(void)
{
    uint8_t status;
        
    cs = 0;
    spi.write(CMD_READ_STATUS);
    status = spi.write(CMD_DUMMY);
    cs = 1;
        
    if ( (status & 0x01) == 0x01 )
        return true;
    else
        return false;
}

static void spi_flash_read_security(uint32_t address, uint8_t *data, uint16_t len)
{
    //wait busy
    while(spi_flash_waitBusy()) {};
        
    cs = 0;
    
    // send read security register command
    spi.write(CMD_READ_SECU);    
        
    // send desired address
    spi.write((address >> 16) & 0xFF);
    spi.write((address >> 8) & 0xFF);
    spi.write((address & 0xFF));
    
    spi.write(CMD_DUMMY);
    /* read data */
    while(len--) {
        *data++ = spi.write(CMD_DUMMY);          
    }
    
    cs = 1;      
}

void spi_flash_get_MACAddress(uint8_t *addr_MAC_DELTA)
{
    /* DELTA MAC address save in the Exteral flash, to use the mac address,
     * application need to read from the Exteral flash and call sd_ble_gap_address_set()
     */
    uint8_t bufferMAC[12];
    uint32_t Security_Reg2 = 0x002000;  // used to store BAT/WIFI MAC address

    spi_flash_read_security(Security_Reg2, bufferMAC, 12);
    
    memset( addr_MAC_DELTA, 0, MAC_LENGTH );
    memcpy( addr_MAC_DELTA, bufferMAC, MAC_LENGTH);
    
    return;
}

bool spi_flash_powerDown(void)
{
    cs = 0;
    spi.write(CMD_POWER_DOWN);
    cs = 1;
        
    //wait for sleep
    wait_us(3);
        
    return true;
}