#include "AP3CordioHCITransportDriver.h"
#include "am_mcu_apollo.h"
#include "stdio.h"
#include <cstring>

#define MAX_INT_DEBUG_VALUES 32

volatile bool bleBusy = false;
uint8_t ample_buffer[256];
void *ble_handle = NULL;

uint16_t lenLastRx = 0;
uint8_t lastRx[256];

uint32_t numInterruptsSinceLast = 0;
uint32_t IntStats[MAX_INT_DEBUG_VALUES];

using namespace ble::vendor::ambiq_micro;
using namespace ble::vendor::cordio;
//AP3CordioHCITransportDriver::AP3CordioHCITransportDriver(/* specific constructor arguments*/) {}

AP3CordioHCITransportDriver::~AP3CordioHCITransportDriver() {}

void AP3CordioHCITransportDriver::initialize()
{
    //getting a grip on our handle
    am_hal_ble_initialize(0, &handle);
    ble_handle = handle;
}

void AP3CordioHCITransportDriver::terminate()
{
    am_hal_ble_deinitialize(handle);
    handle = NULL;
    ble_handle = NULL;
}

uint16_t AP3CordioHCITransportDriver::write(uint8_t packet_type, uint16_t len, uint8_t *data)
{
    uint32_t intAtEnter = numInterruptsSinceLast;
    if (handle)
    {
        while (bleBusy == true)
        {
        }
        //am_hal_ble_wakeup_set(handle, true);
        bleBusy = true;

        // am_hal_flash_delay(FLASH_CYCLES_US(1000 * (50)));
        // printf("Num interrupts since last action = %d\r\n", numInterruptsSinceLast);
        // if (len > 30)
        // {
        //     len = 30;
        // }
        uint16_t retVal = (uint16_t)am_hal_ble_blocking_hci_write(handle, packet_type, (uint32_t *)data, (uint16_t)len);

        // uint8_t aFIFO0REM = BLEIFn(0)->FIFOPTR_b.FIFO0REM;
        // uint8_t aFIFO0SIZ = BLEIFn(0)->FIFOPTR_b.FIFO0SIZ;
        // uint8_t aFIFO1REM = BLEIFn(0)->FIFOPTR_b.FIFO1REM;
        // uint8_t aFIFO1SIZ = BLEIFn(0)->FIFOPTR_b.FIFO1SIZ;
        // uint8_t aFIFO1SIZ = BLEIFn(0)->FIFOPTR_b.FIFO1SIZ;

        // printf("last rx:");
        // for (int i = 0; i < lenLastRx; i++)
        // {
        //     printf(" %02X", lastRx[i]);
        // }
        // printf("\r\n");
        // printf("sent tx:");
        // for (int i = 0; i < len; i++)
        // {
        //     printf(" %02X", data[i]);
        // }
        // printf("\r\n");

        // printf("Writing %d length,FIFO0REM = % d FIFO0SIZ = % d FIFO1REM = % d FIFO1SIZ = % d \r\n", len, aFIFO0REM, aFIFO0SIZ, aFIFO1REM, aFIFO1SIZ);
        // printf("Bytes to written = %d, Num at enter = %d, Num after write = %d\r\n", len, intAtEnter, numInterruptsSinceLast);
        // for (int i = 0; i < numInterruptsSinceLast; i++)
        // {
        //     uint32_t status = IntStats[i];
        //     printf("Interrupt %d status %05X", i, status);
        //     if (status & AM_HAL_BLE_INT_B2MSHUTDN)
        //     {
        //         printf(" - B2MSHUTDN");
        //     }
        //     if (status & AM_HAL_BLE_INT_B2MACTIVE)
        //     {
        //         printf(" - B2MACTIVE");
        //     }
        //     if (status & AM_HAL_BLE_INT_B2MSLEEP)
        //     {
        //         printf(" - B2MSLEEP");
        //     }
        //     if (status & AM_HAL_BLE_INT_CQERR)
        //     {
        //         printf(" - CQERR");
        //     }
        //     if (status & AM_HAL_BLE_INT_CQUPD)
        //     {
        //         printf(" - CQUPD");
        //     }
        //     if (status & AM_HAL_BLE_INT_CQPAUSED)
        //     {
        //         printf(" - CQPAUSED");
        //     }
        //     if (status & AM_HAL_BLE_INT_DERR)
        //     {
        //         printf(" - DERR");
        //     }
        //     if (status & AM_HAL_BLE_INT_DCMP)
        //     {
        //         printf(" - DCMP");
        //     }
        //     if (status & AM_HAL_BLE_INT_BLECSSTAT)
        //     {
        //         printf(" - BLECSSTAT");
        //     }
        //     if (status & AM_HAL_BLE_INT_BLECIRQ)
        //     {
        //         printf(" - BLECIRQ");
        //     }
        //     if (status & AM_HAL_BLE_INT_ICMD)
        //     {
        //         printf(" - ICMD");
        //     }
        //     if (status & AM_HAL_BLE_INT_IACC)
        //     {
        //         printf(" - IACC");
        //     }
        //     if (status & AM_HAL_BLE_INT_B2MST)
        //     {
        //         printf(" - B2MST");
        //     }
        //     if (status & AM_HAL_BLE_INT_FOVFL)
        //     {
        //         printf(" - FOVFL");
        //     }
        //     if (status & AM_HAL_BLE_INT_FUNDFL)
        //     {
        //         printf(" - FUNDFL");
        //     }
        //     if (status & AM_HAL_BLE_INT_THR)
        //     {
        //         printf(" - THR");
        //     }
        //     if (status & AM_HAL_BLE_INT_CMDCMP)
        //     {
        //         printf(" - CMDCMP");
        //     }
        //     printf("\r\n");
        // }
        // printf("Num after print = %d\r\n", numInterruptsSinceLast);
        // printf("retVal = % d \r\n", retVal);
        numInterruptsSinceLast = 0;
        return len;
    }
    return 0;
}

extern "C" void am_ble_isr(void)
{

    uint32_t status = am_hal_ble_int_status(ble_handle, false);
    if (numInterruptsSinceLast < MAX_INT_DEBUG_VALUES)
    {
        IntStats[numInterruptsSinceLast] = status;
    }
    numInterruptsSinceLast++;
    // am_hal_ble_int_clear(ble_handle, status);
    if (status & AM_HAL_BLE_INT_B2MSHUTDN)
    {
        am_hal_ble_int_clear(ble_handle, AM_HAL_BLE_INT_B2MSHUTDN);
    }
    if (status & AM_HAL_BLE_INT_B2MACTIVE)
    {
        am_hal_ble_int_clear(ble_handle, AM_HAL_BLE_INT_B2MACTIVE);
    }
    if (status & AM_HAL_BLE_INT_B2MSLEEP)
    {
        am_hal_ble_int_clear(ble_handle, AM_HAL_BLE_INT_B2MSLEEP);
    }

    if (status & AM_HAL_BLE_INT_BLECIRQ)
    {
        uint32_t len = 0;
        am_hal_ble_blocking_hci_read(ble_handle, (uint32_t *)ample_buffer, &len);
        CordioHCITransportDriver::on_data_received(ample_buffer, len);
        lenLastRx = len;
        memcpy(lastRx, ample_buffer, len);
        am_hal_ble_int_clear(ble_handle, AP3_STUPID_DEF_OF_BLECIRQ_BIT);
    }
    if (status & AM_HAL_BLE_INT_BLECSSTAT)
    {
        bleBusy = false;
        am_hal_ble_int_clear(ble_handle, AM_HAL_BLE_INT_BLECSSTAT);
    }
    if (status & AM_HAL_BLE_INT_ICMD)
    {
        am_hal_ble_int_clear(ble_handle, AM_HAL_BLE_INT_ICMD);
    }
    if (status & AM_HAL_BLE_INT_B2MST)
    {
        am_hal_ble_int_clear(ble_handle, AM_HAL_BLE_INT_B2MST);
    }
    //am_hal_ble_int_clear(ble_handle, 0xFFFF);
    // else
    // {
    //     while (1)
    //     {
    //     }
    // }
}
