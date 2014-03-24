#ifndef __BLE_CENTRAL_BONDMNGR_H_
#define __BLE_CENTRAL_BONDMNGR_H_

#include "nordic_global.h"
#include "ble.h"
#include "ble_gap.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    ble_gap_sec_params_t * p_sec_params;
} ble_central_bondmngr_t;

typedef struct
{
    ble_gap_sec_params_t * p_sec_params;
    bool delete_bonds;
} ble_central_bondmngr_init_t;

uint32_t ble_central_bondmngr_init(ble_central_bondmngr_t * p_bm, ble_central_bondmngr_init_t * p_bm_init);
void ble_central_bondmngr_on_ble_evt(ble_central_bondmngr_t * p_bm, ble_evt_t * p_ble_evt);
uint32_t ble_central_bondmngr_store(ble_central_bondmngr_t * p_bm);

#endif
