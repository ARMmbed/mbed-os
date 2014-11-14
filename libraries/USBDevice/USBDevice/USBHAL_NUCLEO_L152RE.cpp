// USB Device Support for STM Nucleo-L152RE Board

#if defined(TARGET_NUCLEO_L152RE)

#warning USB Device not yet functional for Nucleo L152RE board - this is work in progress

#include "USBHAL.h"

// Stubs for the 21 needed USBHAL class routines needed by the Nucleo-L152RE Board

USBHAL::USBHAL(){}
USBHAL::~USBHAL(){}
void USBHAL::connect(){}
void USBHAL::disconnect(){}
void USBHAL::configureDevice(){}
void USBHAL::unconfigureDevice(){}
void USBHAL::setAddress(uint8_t address){}
void USBHAL::remoteWakeup(){}
void USBHAL::EP0read (){}
void USBHAL::EP0readStage (){}
void USBHAL::EP0write (uint8_t *buffer, uint32_t size){}
uint32_t USBHAL::EP0getReadResult (uint8_t *buffer){ return 1;}
void USBHAL::EP0getWriteResult (){}
void USBHAL::EP0stall (){}
void USBHAL::EP0setup (uint8_t *buffer){}
EP_STATUS USBHAL::endpointRead (uint8_t endpoint, uint32_t maximumSize){ return EP_COMPLETED;}
EP_STATUS USBHAL::endpointWrite (uint8_t endpoint, uint8_t *data, uint32_t size){return EP_COMPLETED;}
EP_STATUS USBHAL::endpointReadResult (uint8_t endpoint, uint8_t *data, uint32_t *bytesRd){return EP_COMPLETED;}
EP_STATUS USBHAL::endpointWriteResult (uint8_t endpoint){return EP_COMPLETED;}
bool USBHAL::realiseEndpoint (uint8_t endpoint, uint32_t maxPacket, uint32_t options){return true;}
void USBHAL::stallEndpoint (uint8_t endpoint){}
void USBHAL::unstallEndpoint (uint8_t endpoint){}
bool USBHAL::getEndpointStallState (unsigned char endpoint){return true;}

#endif
