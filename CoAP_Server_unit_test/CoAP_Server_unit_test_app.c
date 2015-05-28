#include <stdio.h>
#include <stdlib.h>
#include "ns_types.h"
#include "coap_service_api.h"

int main(void) {
	printf("!!!CoAP Server test!!!"); /* prints !!!Hello World!!! */

	int8_t service_id = 0;

	service_id = coap_service_initialize(0xff, 666, 0);

	return EXIT_SUCCESS;
}

