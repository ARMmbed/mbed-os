#include "ble_advdata_parser.h"

uint32_t ble_advdata_parser_field_find(uint8_t type, uint8_t * p_advdata, uint8_t * len, uint8_t ** pp_field_data)
{
    uint32_t index = 0;
    
    while (index < *len)
    {
        uint8_t field_length = p_advdata[index];
        uint8_t field_type = p_advdata[index+1];
        
        if (field_type == type)
        {
            *pp_field_data = &p_advdata[index+2];
            *len = field_length-1;
            return NRF_SUCCESS;
        }
        index += field_length+1;
    }
    return NRF_ERROR_NOT_FOUND;
}
