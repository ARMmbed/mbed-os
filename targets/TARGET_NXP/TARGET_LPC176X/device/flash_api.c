/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed_critical.h"

#if DEVICE_FLASH
#include "mbed_assert.h"
#include "cmsis.h"
#include <stdlib.h>
#include <string.h>

  #define MEMMAP     (*((volatile unsigned long *) 0x400FC040))

  #define PLL0CON    (*((volatile unsigned long *) 0x400FC080))
  #define PLL0CFG    (*((volatile unsigned long *) 0x400FC084))
  #define PLL0STAT   (*((volatile unsigned long *) 0x400FC088))
  #define PLL0FEED   (*((volatile unsigned long *) 0x400FC08C))
  #define CCLKSEL    (*((volatile unsigned long *) 0x400FC104))
  #define CLKSRCSEL  (*((volatile unsigned long *) 0x400FC10C))

#define STACK_SIZE     64      // Stack Size

#define SET_VALID_CODE 1       // Set Valid User Code Signature
/* IAP Call */
typedef void (*IAP_Entry) (unsigned long *cmd, unsigned long *stat);
#define IAP_Call ((IAP_Entry) 0x1FFF1FF1)

typedef struct flash_s flash_t;
unsigned long CCLK;            // CCLK in kHz


struct sIAP {				  // IAP Structure
  unsigned long cmd;		   // Command
  unsigned long par[4];		// Parameters
  unsigned long stat;		  // Status
  unsigned long res[2];		// Result
} IAP;

/*
 * Get Sector Number
 *	Parameter:	  address:  Sector Address
 *	Return Value:   Sector Number
 */

unsigned long GetSecNum (unsigned long address) {
  unsigned long n;


  n = address >> 12;							   //  4kB Sector
  if (n >= 0x10) {
	n = 0x0E + (n >> 3);					   // 32kB Sector
  }


  return (n);								  // Sector Number
}
int32_t flash_init(flash_t *obj)
{
	CCLK = SystemCoreClock / 1000;						   // CCLK value is in kHz, clk in Hz

  MEMMAP   = 0x01;							 // User Flash Mode

  return (0);
}

int32_t flash_free(flash_t *obj)
{
   return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
	unsigned long n;

  n = GetSecNum(address);						  // Get Sector Number

  IAP.cmd	= 50;							 // Prepare Sector for Erase
  IAP.par[0] = n;							  // Start Sector
  IAP.par[1] = n;							  // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);			  // Call IAP Command
  if (IAP.stat) return (1);					// Command Failed

  IAP.cmd	= 52;							 // Erase Sector
  IAP.par[0] = n;							  // Start Sector
  IAP.par[1] = n;							  // End Sector
  IAP.par[2] = CCLK;						   // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);			  // Call IAP Command
  if (IAP.stat) return (1);					// Command Failed

  return (0);								  // Finished without Errors

}

/* IAP Call */
typedef void (*IAP_Entry) (unsigned long *cmd, unsigned long *stat);
#define IAP_Call ((IAP_Entry) 0x1FFF1FF1)


int32_t flash_program_page(flash_t *obj, uint32_t address,
		const uint8_t *datain, uint32_t size)
{
	uint8_t *data;
	unsigned long n;
	
	if ((unsigned long)datain%4==0)//word boundary
	{
		data = datain;
	}
	else
	{
		data = malloc(size);
		memcpy(data,datain,size);
	}
	

#if SET_VALID_CODE != 0						// Set valid User Code Signature
  if (address == 0) {							  // Check for Vector Table
	n = *((unsigned long *)(data + 0x00)) +
		*((unsigned long *)(data + 0x04)) +
		*((unsigned long *)(data + 0x08)) +
		*((unsigned long *)(data + 0x0C)) +
		*((unsigned long *)(data + 0x10)) +
		*((unsigned long *)(data + 0x14)) +
		*((unsigned long *)(data + 0x18));
	*((unsigned long *)(data + 0x1C)) = 0 - n;  // Signature at Reserved Vector
  }
#endif

  n = GetSecNum(address);						  // Get Sector Number

  IAP.cmd	= 50;							 // Prepare Sector for Write
  IAP.par[0] = n;							  // Start Sector
  IAP.par[1] = n;							  // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);			  // Call IAP Command
  if (IAP.stat) return (1);					// Command Failed

  IAP.cmd	= 51;							 // Copy RAM to Flash
  IAP.par[0] = address;							// Destination Flash Address
  IAP.par[1] = (unsigned long)data;			 // Source RAM Address

  IAP.par[2] = 1024;						   // Fixed Page Size
  IAP.par[3] = CCLK;						   // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);			  // Call IAP Command
  if(data !=datain)//We allocated our own memory
  {
	free(data);
  }
  if (IAP.stat) return (1);					// Command Failed

  return (0);								  // Finished without Errors
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address) 
{
  
 if(GetSecNum(address)>=0x10)
	 {
		 return 0x8000;
	 }
	else
	{
		return 0x1000;
	}
}


uint32_t flash_get_page_size(const flash_t *obj)
{
	return 1024;
}

uint32_t flash_get_start_address(const flash_t *obj) 
{
	return LPC_FLASH_BASE;
}

uint32_t flash_get_size(const flash_t *obj) 
{
	return 0x80000;
}

#endif
