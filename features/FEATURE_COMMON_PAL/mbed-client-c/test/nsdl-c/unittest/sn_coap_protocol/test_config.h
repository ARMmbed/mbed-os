#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

/**
 * \def SN_COAP_DUPLICATION_MAX_MSGS_COUNT
 * \brief For Message duplication detection
 * Init value for the maximum count of messages to be stored for duplication detection
 * Setting of this value to 0 will disable duplication check, also reduce use of ROM memory
 * Default is set to 0.
 */
#define SN_COAP_DUPLICATION_MAX_MSGS_COUNT  1

#endif 
