/*
 * factory_options_cb.S
 *
 * Created: 11.09.2017 11:35:32
 *  Author: Guenter.Prossliner
 */ 

// this section is only used for the vNextElfTool, to extract the address and value of fuses to be programmed in the factory.hex
// this data is not included in the image
.section .factory_options,"",%note
.word 0x1FFFC008	// option address of WRP 0-11
.word 0xC0013FFE	// option value write-protect sector 0
