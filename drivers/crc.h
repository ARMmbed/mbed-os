/*************************************************************************//**
 *
 * \file    crc.h
 * \brief   ...
 *
 * \note    None
 *
 * \author  Daniel Cesarini <daniel.cesarini@tridonic.com>
 * \author  Martin Tomasini <martin.tomasini2@tridonic.com>
 *
 * \version 0.1
 * \date    4 July 2016
 * \bug     None
 *
 * \copyright 2016, Tridonic GmbH & Co KG
 *                  Färbergasse 15 - 6851 Dornbirn, Austria
 *
 *****************************************************************************/


#ifndef YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_CRC_H_
#define YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_CRC_H_

/* Includes -----------------------------------------------------------------*/

#include "platform.h"
#include "stdint.h"

/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/
#define HW_CRC
#define CRC_INITVALUE 0xFFFFFFFF

/* Public macro -------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/
namespace mbed
{
    class Crc
    {
    public:
        Crc();
        ~Crc();
        int _UseCrc(void);
        int _ReleaseCrc(void);
        void _Reset(void);
        int _Calculate(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC);
        int _Accumulate(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC);
		
    private:
        void acquireMutex();
        void releaseMutex();
        static uint32_t InstanceCounter;
        static bool Initialized;
        uint32_t localCrc;
    };
} // namespace mbed


#endif /* YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_INTERNALFLASH_H_ */
