/**
 * \file      FragAssembler.h
 *
 * \brief     Helps the FragmentationControlPackage in performing FEC over
 *            the received data block using LDPC.
 *
 * \copyright Revised BSD License, see LICENSE.TXT file include in the project
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013 Semtech
 *
 * \endcode
 *
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MBED_LORAWAN_UPDATE_CLIENT_CRYPTO_FRAGMENTATION_MATH
#define _MBED_LORAWAN_UPDATE_CLIENT_CRYPTO_FRAGMENTATION_MATH

#include <string.h>
#include "mbed.h"
#include "mbed_debug.h"
#include "FragBDWrapper.h"

#define PUSH_LINE_FLAG          0x01
#define FETCH_LINE_FLAG          0x02
#define FRAG_SESSION_ONGOING    0xffff

typedef struct {
    int nb_frags;     // number of fragments (M) excluding the redundancy fragments
    int nb_red_frags; // number of redundancy fragments (N)
    int frag_size;    // size of a single fragment
} frag_assembler_params_t;

/** Assembles received fragments into the original intended data block
 *
 * This class is an implementation of FEC algorithm developed by Semtech and
 * published in LoRaWAN Fragmented Data Block Transport specification v1.0.0.
 * It's purpose is to provide forward error correction facility for the received
 * fragments belonging to a large data block.
 */
class FragAssembler
{
  public:
    /**
     * CTOR
     */
    FragAssembler();

    /**
     * DTOR
     */
    ~FragAssembler();

    /**
     * Initialize the FragAssembler library.
     * This function allocates the required buffers.
     *
     * @param flash          Instance of wrapped BlockDevice
     * @param nb_frags       Number of expected fragments (without redundancy fragments)
     * @param frag_size      Size of a fragment (without DataFragment overhead)
     * @param nb_red_frags   Number of redundancy fragments
     * @param flash_offset   Offset, if needed for writing to the BlockDevice
     *
     * @returns true if the memory was allocated, false if one or more allocations
     *               failed.
     */
    bool initialize(FragBDWrapper *flash, uint16_t nb_frags,
                    uint8_t frag_size, uint16_t nb_red_frags,
                    uint32_t flash_offset);

    /**
     * This method is used to mark a fragment (identified by its index)
     * received.
     *
     * @param frag_idx    Index of the fragment
     */
    void set_frag_received(uint16_t frag_idx);

    /**
     * Process a coded redundant fragment.
     *
     * @param frag_idx          The index of the fragment
     * @param frag_pload        Payload of the DataFragment received (without overhead)
     * @param params            Configuration parameters for the assembler
     *
     * @returns FRAG_SESSION_ONGOING if the packets are not completed yet,
                any other value between 0..FRAG_SESSION_ONGOING if the packet was deconstructed
     */
    int process_redundant_frag(uint16_t frag_idx, const uint8_t *frag_pload/*,
                               frag_assembler_params_t params*/);

    /**
     * Get the number of fragments still missing
     */
    int get_missing_frag_count(void);

  private:
    void get_row_from_flash(int line_number, uint8_t *row_data);
    void store_row_in_flash(uint8_t *row_data, int index);
    uint16_t find_missing_frag_index(uint16_t missing_frag);
    void update_missing_frag_vector(uint16_t frag_index);
    void xor_line_data(uint8_t *dataL1, uint8_t *dataL2, int size);
    void xor_line_bool(bool *dataL1, bool *dataL2, int size);
    int find_first_1_in_line(bool *boolData, int size);
    bool is_vector_null(bool *boolData, int size);
    void manipulate_bin_mat_line(bool *bool_vector, int row_number, int number_of_bits, int flag);
    void get_parity_matrix_row(int N, int M, bool *matrixRow);
    int get_prbs_23bit(int x);

    FragBDWrapper *_flash;
    uint16_t _nb_frags;
    uint8_t _frag_size;
    uint16_t _nb_red_frags;
    size_t _flash_offset;

    uint8_t *matrix_m2bin;
    uint16_t *missing_frags_vector;

    bool *matrix_row;
    uint8_t *matrix_data_temp;
    bool *data_temp_vector;
    bool *data_temp_vector2;
    bool *s;
    uint8_t *xor_row_data_temp;

    int number_of_missing_frames;
    int last_recvd_frag_idx;
};

#endif // _MBED_LORAWAN_UPDATE_CLIENT_CRYPTO_FRAGMENTATION_MATH
