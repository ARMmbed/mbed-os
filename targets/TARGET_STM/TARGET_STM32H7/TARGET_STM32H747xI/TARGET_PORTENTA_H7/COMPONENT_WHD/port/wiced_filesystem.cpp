/*
 * Copyright 2020 Arduino SA
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file
 *  Provides wiced fs porting to generic mbed APIs
 */

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string>
#include "sockets.h"
#include "resources.h"
#include "wiced_filesystem.h"
#include "wiced_bd.h"
#include "QSPIFBlockDevice.h"
#include "MBRBlockDevice.h"
#include "FATFileSystem.h"

#define WIFI_DEFAULT_FIRMWARE_PATH "/wlan/4343WA1.BIN"
#define WIFI_DEFAULT_MOUNT_NAME "wlan"
#define WIFI_DEFAULT_PARTITION 1
#define WIFI_DEFAULT_FS 0

QSPIFBlockDevice *qspi_bd = NULL;
MBRBlockDevice   *mbr_bd = NULL;
FATFileSystem    *wifi_fs = NULL;

wiced_filesystem_t resource_fs_handle;

MBED_WEAK void wiced_filesystem_mount_error(void)
{
    WPRINT_WHD_ERROR(("Failed to mount the filesystem containing the WiFi firmware.\n\r"));
    whd_print_logbuffer();
    while (1) {}
}

MBED_WEAK void wiced_filesystem_firmware_error(void)
{
    WPRINT_WHD_ERROR(("Please run the \"PortentaWiFiFirmwareUpdater\" sketch once to install the WiFi firmware.\n\r"));
    whd_print_logbuffer();
    while (1) {}
}

MBED_WEAK wiced_result_t whd_firmware_check_hook(const char *mounted_name, int mount_err)
{
    DIR *dir;
    struct dirent *ent;
    std::string dir_name(mounted_name);
    if (mount_err) {
        wiced_filesystem_mount_error();
    } else {
        if ((dir = opendir(mounted_name)) != NULL) {
            // print all the files and directories within directory
            while ((ent = readdir(dir)) != NULL) {
                std::string fullname =  "/" + dir_name + "/" + std::string(ent->d_name);
                if (fullname == WIFI_DEFAULT_FIRMWARE_PATH) {
                    closedir(dir);
                    return WICED_SUCCESS;
                }
            }
            closedir(dir);
        }
        wiced_filesystem_firmware_error();
    }
    return WICED_ERROR;
}

static wiced_result_t whd_default_firmware_restore(void)
{
#if MBED_CONF_APP_WIFI_FIRMWARE_RESTORE
    size_t ret;
    FILE *fp;

    //This should only happen the firs time or if the partition table has been overwritten i.e QSPI tests
    WPRINT_WHD_DEBUG(("MBRBlockDevice init failed, repatitioning\n\r"));
    if (mbr_bd->partition(qspi_bd, 1, 0x0B, 0, 1024 * 1024) != BD_ERROR_OK) {
        return WICED_ERROR;
    }
    WPRINT_WHD_DEBUG(("MBRBockDevice repatitioning OK, reinit\n\r"));

    if (mbr_bd->init() != BD_ERROR_OK) {
        return WICED_ERROR;
    }
    WPRINT_WHD_DEBUG(("MBRBockDevice init OK\n\r"));

    wifi_fs = new FATFileSystem(WIFI_DEFAULT_MOUNT_NAME);

    if (wifi_fs->reformat(mbr_bd) != 0) {
        return WICED_ERROR;
    }
    WPRINT_WHD_DEBUG(("FATFileSystem reformat and mount OK\n\r"));

    fp = fopen(WIFI_DEFAULT_FIRMWARE_PATH, "wb");
    if (fp == NULL) {
        return WICED_ERROR;
    }
    ret = fwrite(wifi_firmware_image_data, wifi_firmware_image.size, 1, fp);
    if (ret != wifi_firmware_image.size) {
        return WICED_ERROR;
    }
    fclose(fp);

    if (mbr_bd->sync() != 0) {
        return WICED_ERROR;
    }
    WPRINT_WHD_DEBUG(("Sync BlockDevice OK\n\r"));

    if (wifi_fs->unmount() != 0) {
        return WICED_ERROR;
    }
    WPRINT_WHD_DEBUG(("Unmount FS\n\r"));
    wifi_fs = NULL;
#endif

    return WICED_SUCCESS;
}

wiced_result_t wiced_filesystem_init(void)
{
    if (mbr_bd == NULL && wifi_fs == NULL) {
        WPRINT_WHD_DEBUG(("Initialize FileSystem with Mbed default settings\n\r"));
        qspi_bd = new QSPIFBlockDevice(PD_11, PD_12, PF_7, PD_13,  PF_10, PG_6, QSPIF_POLARITY_MODE_1, 40000000);

        if (qspi_bd->init() == BD_ERROR_OK) {
            mbr_bd = new MBRBlockDevice(qspi_bd, WIFI_DEFAULT_PARTITION);
            if (mbr_bd->init() == BD_ERROR_OK) {
                return WICED_SUCCESS;
            } else {
                return whd_default_firmware_restore();
            }
        }
        return WICED_ERROR;
    } else {
        WPRINT_WHD_DEBUG(("FileSystem initialized with user settings\n\r"));
        return WICED_SUCCESS;
    }
}

wiced_result_t wiced_filesystem_mount(BlockDevice *device, wiced_filesystem_handle_type_t fs_type, wiced_filesystem_t *fs_handle_out, const char *mounted_name)
{
    wifi_fs = new FATFileSystem(mounted_name);

    int err = wifi_fs->mount(device);
    whd_firmware_check_hook(mounted_name, err);
    if (!err) {
        //fs_handle_out = wifi_fs
        return WICED_SUCCESS;
    }
    return WICED_ERROR;
}

wiced_result_t wiced_filesystem_file_open(wiced_filesystem_t *fs_handle, wiced_file_t *file_handle_out, const char *filename, wiced_filesystem_open_mode_t mode)
{
    /* This is called by mbed test system */
    //if (mbr_bd == NULL && wifi_fs == NULL) {
    //    wiced_filesystem_init();
    //}
    //This can be called from user sketch to provide custom block device and mount point before WiFi.beginAP or WiFi.begin
    if (wifi_fs == NULL) {
        wiced_filesystem_mount(mbr_bd, WIFI_DEFAULT_FS, fs_handle, WIFI_DEFAULT_MOUNT_NAME);
    }

    if (wifi_fs == NULL) {
        return WICED_ERROR;
    }

    *file_handle_out = open(filename, mode);
    if (*file_handle_out == -1) {
        return WICED_ERROR;
    }
    return WICED_SUCCESS;
}

wiced_result_t wiced_filesystem_file_seek(wiced_file_t *file_handle, int64_t offset, wiced_filesystem_seek_type_t whence)
{
    if (*file_handle == -1) {
        return WICED_ERROR;
    }
    lseek(*file_handle, offset, whence);
    return WICED_SUCCESS;
}

wiced_result_t wiced_filesystem_file_read(wiced_file_t *file_handle, void *data, uint64_t bytes_to_read, uint64_t *returned_bytes_count)
{
    if (*file_handle == -1) {
        return WICED_ERROR;
    }
    *returned_bytes_count = read(*file_handle, data, bytes_to_read);
    return WICED_SUCCESS;
}

wiced_result_t wiced_filesystem_file_close(wiced_file_t *file_handle)
{
    if (*file_handle == -1) {
        return WICED_ERROR;
    }
    close(*file_handle);
    return WICED_SUCCESS;
}
