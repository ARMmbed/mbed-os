/**
 * \file      FragAssembler.h
 *
 * \brief     Helps the FragmentationControlPackage in de-fragmentation
 *            of the received data block.
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

#include <plugins/FragAssembler.h>
#include "mbed_trace.h"
#define TRACE_GROUP "FMTH"

FragAssembler::FragAssembler()
    : _flash(NULL),
      _nb_frags(0),
      _frag_size(0),
      _nb_red_frags(0),
      _flash_offset(0),
      matrix_m2bin(NULL),
      missing_frags_vector(NULL),
      matrix_row(NULL),
      matrix_data_temp(NULL),
      data_temp_vector(NULL),
      data_temp_vector2(NULL),
      s(NULL),
      xor_row_data_temp(NULL),
      number_of_missing_frames(0),
      last_recvd_frag_idx(0)
{
}

FragAssembler::~FragAssembler()
{
    if (matrix_m2bin) {
        free(matrix_m2bin);
    }

    if (missing_frags_vector) {
        free(missing_frags_vector);
    }

    if (matrix_row) {
        free(matrix_row);
    }

    if (matrix_data_temp) {
        free(matrix_data_temp);
    }

    if (data_temp_vector) {
        free(data_temp_vector);
    }

    if (data_temp_vector2) {
        free(data_temp_vector2);
    }
    if (s) {
        free(s);
    }

    if (xor_row_data_temp) {
        free(xor_row_data_temp);
    }
}

bool FragAssembler::initialize(FragBDWrapper *flash, uint16_t frame_count,
                               uint8_t frame_size, uint16_t redundancy_max,
                               uint32_t flash_offset)
{
    _flash = flash;

    // global for this session
    matrix_m2bin = (uint8_t *) calloc((_nb_red_frags / 8) * _nb_red_frags, 1);

    missing_frags_vector = (uint16_t *) calloc(_nb_frags, sizeof(uint16_t));

    // these get reset for every frame
    matrix_row = (bool *) calloc(_nb_frags, 1);
    matrix_data_temp = (uint8_t *) calloc(_frag_size, 1);
    data_temp_vector = (bool *) calloc(_nb_red_frags, 1);
    data_temp_vector2 = (bool *) calloc(_nb_red_frags, 1);
    xor_row_data_temp = (uint8_t *) calloc(_frag_size, 1);
    s = (bool *) calloc(_nb_red_frags, 1);

    number_of_missing_frames = 0;
    last_recvd_frag_idx = 0;

    if (!matrix_m2bin ||
        !missing_frags_vector ||
        !matrix_row ||
        !matrix_data_temp ||
        !data_temp_vector ||
        !data_temp_vector2 ||
        !s ||
        !xor_row_data_temp)
    {
        tr_warn("Could not allocate memory");
        return false;
    }

    for (size_t ix = 0; ix < _nb_frags; ix++)
    {
        missing_frags_vector[ix] = 1;
    }


    return true;
}

void FragAssembler::set_frag_received(uint16_t frameCounter)
{
    missing_frags_vector[frameCounter - 1] = 0;

    update_missing_frag_vector(frameCounter);
}

int FragAssembler::process_redundant_frag(uint16_t frag_idx,
                                          const uint8_t *frag_pload/*,
                                          frag_assembler_params_t params*/)
{
    int l;
    int i;
    int j;
    int li;
    int lj;
    int row_number;
    static int m2l = 0;
    int first = 0;
    int no_info = 0;

    memset(matrix_row, 0, _nb_frags);
    memset(matrix_data_temp, 0, _frag_size);
    memset(data_temp_vector, 0, _nb_red_frags);
    memset(data_temp_vector2, 0, _nb_red_frags);
    // we should not mess with original payload
    memcpy(xor_row_data_temp, frag_pload, _frag_size);

    update_missing_frag_vector(frag_idx);

    // frag_index
    get_parity_matrix_row(frag_idx - _nb_frags, _nb_frags, matrix_row);

    for (l = 0; l < _nb_frags; l++) {
        if (matrix_row[l] == 1) {
            if (missing_frags_vector[l] == 0) {
                // XOR with already received fragment
                matrix_row[l] = 0;
                if (!_flash) {
                    tr_error("Frag Abort: No BlockDevice");
                    return -1;
                }
                get_row_from_flash(l, matrix_data_temp);
                xor_line_data(xor_row_data_temp, matrix_data_temp, _frag_size);
            } else {
                // fill the "little" boolean matrix m2
                data_temp_vector[missing_frags_vector[l] - 1] = 1;
                if (first == 0) {
                    first = 1;
                }
            }
        }
    }

    row_number = find_first_1_in_line(data_temp_vector, number_of_missing_frames);

    if (first > 0) {
        //manage a new line in MatrixM2
        while (s[row_number] == 1) {
            // row already diagonalized exist&(params.MatrixM2[firstOneInRow][0])
            manipulate_bin_mat_line(data_temp_vector2, row_number, number_of_missing_frames, FETCH_LINE_FLAG);
            xor_line_bool(data_temp_vector, data_temp_vector2, number_of_missing_frames);
            li = find_missing_frag_index(row_number); // have to store it in the mi th position of the missing frame
            get_row_from_flash(li, matrix_data_temp);
            xor_line_data(xor_row_data_temp, matrix_data_temp, _frag_size);

            if (is_vector_null(data_temp_vector, number_of_missing_frames)) {
                no_info = 1;
                break;
            }
            row_number = find_first_1_in_line(data_temp_vector, number_of_missing_frames);
        }

        if (no_info == 0) {
            manipulate_bin_mat_line(data_temp_vector, row_number, number_of_missing_frames, PUSH_LINE_FLAG);
            li = find_missing_frag_index(row_number);
            store_row_in_flash(xor_row_data_temp, li);
            s[row_number] = 1;
            m2l++;
        }

        if (m2l == number_of_missing_frames) {
            // then last step diagonalized
            if (number_of_missing_frames > 1) {
                for (i = (number_of_missing_frames - 2); i >= 0; i--) {
                    li = find_missing_frag_index(i);
                    get_row_from_flash(li, matrix_data_temp);
                    for (j = (number_of_missing_frames - 1); j > i; j--) {
                        manipulate_bin_mat_line(data_temp_vector2, i, number_of_missing_frames, FETCH_LINE_FLAG);
                        manipulate_bin_mat_line(data_temp_vector, j, number_of_missing_frames, FETCH_LINE_FLAG);
                        if (data_temp_vector2[j] == 1) {
                            xor_line_bool(data_temp_vector2, data_temp_vector, number_of_missing_frames);
                            manipulate_bin_mat_line(data_temp_vector2, i, number_of_missing_frames, PUSH_LINE_FLAG);
                            lj = find_missing_frag_index(j);
                            get_row_from_flash(lj, xor_row_data_temp);
                            xor_line_data(matrix_data_temp, xor_row_data_temp, _frag_size);
                        }
                    }
                    store_row_in_flash(matrix_data_temp, li);
                }
                return (number_of_missing_frames);
            } else {
                //if not ( number_of_missing_frames > 1 )
                return (number_of_missing_frames);
            }
        }
    }

    return FRAG_SESSION_ONGOING;
}

int FragAssembler::get_missing_frag_count()
{
    return number_of_missing_frames;
}

void FragAssembler::get_row_from_flash(int idx, uint8_t *row_data)
{
    int r = _flash->read(row_data, _flash_offset + (idx * _frag_size), _frag_size);
    if (r != 0) {
        tr_warn("get_row_from_flash: for row %d failed (%d)", idx, r);
    }
}

void FragAssembler::store_row_in_flash(uint8_t *row_data, int idx)
{
    int r = _flash->program(row_data, _flash_offset + (_frag_size * idx), _frag_size);
    if (r != 0) {
        tr_warn("StoreRowInFlash for row %d failed (%d)", idx, r);
    }
}

uint16_t FragAssembler::find_missing_frag_index(uint16_t x)
{
    for (int i = 0; i < _nb_frags; i++) {
        if (missing_frags_vector[i] == (x + 1)) {
            return i;
        }
    }

    return 0;
}

void FragAssembler::update_missing_frag_vector(uint16_t frag_index)
{
    uint16_t q;

    for (q = last_recvd_frag_idx; q < (frag_index - 1); q++) {
        if (q < _nb_frags) {
            number_of_missing_frames++;
            missing_frags_vector[q] = number_of_missing_frames;
        }
    }

    if (q < _nb_frags) {
        last_recvd_frag_idx = frag_index;
    } else {
        last_recvd_frag_idx = _nb_frags + 1;
    }
}

void FragAssembler::xor_line_data(uint8_t *data_line1, uint8_t *data_line2, int size)
{
    int i;
    uint8_t *data_temp = (uint8_t *) malloc(size);
    if (!data_temp) {
        tr_error("xor_line_data: malloc out of memory!");
        return;
    }

    for (i = 0; i < size; i++) {
        data_temp[i] = data_line1[i] ^ data_line2[i];
    }

    for (i = 0; i < size; i++) {
        data_line1[i] = data_temp[i];
    }

    free(data_temp);
}

void FragAssembler::xor_line_bool(bool *data_line1, bool *data_line2, int size)
{
    int i;
    bool *dataTemp = (bool *)malloc(size);
    if (!dataTemp) {
        tr_error("xor_line_bool: malloc failed");
        return;
    }

    for (i = 0; i < size; i++) {
        dataTemp[i] = data_line1[i] ^ data_line2[i];
    }

    for (i = 0; i < size; i++) {
        data_line1[i] = dataTemp[i];
    }

    free(dataTemp);
}

int FragAssembler::find_first_1_in_line(bool *boolData, int size)
{
    for (int i = 0; i < size; i++) {
        if (boolData[i] == 1) {
            return i;
        }
    }

    return 0;
}

bool FragAssembler::is_vector_null(bool *bool_data, int size)
{
    for (int i = 0; i < size; i++) {
        if (bool_data[i] == 1) {
            return false;
        }
    }

    return true;
}

void FragAssembler::manipulate_bin_mat_line(bool *bool_vector, int row_number,
                                            int number_of_bits, int flag)
{
    int i;
    int find_byte;
    int find_bit_in_byte;
    if (row_number > 0) {
        find_byte = (row_number * number_of_bits - ((row_number * (row_number - 1)) / 2)) / 8;
        find_bit_in_byte = (row_number * number_of_bits - ((row_number * (row_number - 1)) / 2)) % 8;
    } else {
        find_byte = 0;
        find_bit_in_byte = 0;
    }

    if (flag == FETCH_LINE_FLAG) {
        if (row_number > 0) {
            for (i = 0; i < row_number; i++) {
                bool_vector[i] = 0;
            }
        }
    }

    for (i = row_number; i < number_of_bits; i++) {
        if (flag == FETCH_LINE_FLAG) {
            bool_vector[i] = (matrix_m2bin[find_byte] >> (7 - find_bit_in_byte)) & 0x01;
        } else if (flag == PUSH_LINE_FLAG) {
            if (bool_vector[i] == 1) {
                matrix_m2bin[find_byte] = (matrix_m2bin[find_byte] & (0xFF - (1 << (7 - find_bit_in_byte)))) + (1 << (7 - find_bit_in_byte));
            } else {
                matrix_m2bin[find_byte] = (matrix_m2bin[find_byte] & (0xFF - (1 << (7 - find_bit_in_byte))));
            }
        }

        find_bit_in_byte++;

        if (find_bit_in_byte == 8) {
            find_bit_in_byte = 0;
            find_byte++;
        }
    }
}

void FragAssembler::get_parity_matrix_row(int N, int M, bool *matrix_row)
{
    // A factor m that stops repeating patterns in case M is a power of two
    int m = 0;

    // A counter that terminates the loop after 50 % of the bits are set
    // in the parity matrix
    int coeff = 0;
    // Pseudo-randomly selected column index
    int rand_col_idx = 0;

    // if m is power of two, set m_factor to be 1 to prevent pattern forming.
    if ((M != 0) && !(M & (M - 1))) {
        m = 1;
    }

    // A known seed based on N
    int seed = 1 + (1001 * N);

    for (int i = 0; i < M; i++) {
        matrix_row[i] = 0;
    }

    while (coeff < (M >> 1)) {
        rand_col_idx = 1 << 16;
        while (rand_col_idx >= M) {
            seed = get_prbs_23bit(seed);
            rand_col_idx = seed % (M + m);
        }

        matrix_row[rand_col_idx] = 1;
        coeff += 1;
    }
}

int FragAssembler::get_prbs_23bit(int x)
{
    int b0 = x & 1;
    int b1 = (x & 0x20) >> 5;
    x = (int)floor((float)x / 2) + ((b0 ^ b1) << 22);
    return x;
}
