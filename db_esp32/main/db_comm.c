/*
 *   This file is part of DroneBridge: https://github.com/seeul8er/DroneBridge
 *
 *   Copyright 2018 Wolfgang Christl
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */
#include <cJSON.h>
#include <stdint.h>
#include <string.h>
#include "db_comm.h"
#include "db_comm_protocol.h"

uint32_t calc_crc32(uint32_t crc, unsigned char *buf, size_t len)
{
    int k;

    crc = ~crc;
    while (len--) {
        crc ^= *buf++;
        for (k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ 0xedb88320 : crc >> 1;
    }
    return ~crc;
}

/**
 * @brief Check CRC32 of a message
 * @param buf Buffer containing compete message (JSON+CRC)
 * @param msg_length Length of entire message (JSON+CRC)
 * @return 1 if CRC is good else 0
 */
int crc_ok(uint8_t * buf, int msg_length){
    uint32_t c_crc = calc_crc32((uint32_t) 0, buf, (size_t) (msg_length-4));
    uint8_t crc_bytes[4];
    crc_bytes[0] = c_crc;
    crc_bytes[1] = c_crc >>  8;
    crc_bytes[2] = c_crc >> 16;
    crc_bytes[3] = c_crc >> 24;
    if ((crc_bytes[0] == buf[msg_length-4]) && (crc_bytes[1] == buf[msg_length-3]) &&
        (crc_bytes[2] == buf[msg_length-2]) && (crc_bytes[3] == buf[msg_length-1])){
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief Add crc32 at end and build final array
 * @return
 */
int finalize_message(uint8_t *msg_buf, char *req_json){
    size_t json_length = strlen(req_json);
    uint32_t new_crc = calc_crc32((uint32_t) 0, (unsigned char *) req_json, strlen(req_json));
    msg_buf[json_length] = new_crc;
    msg_buf[json_length+1] = new_crc >>  8;
    msg_buf[json_length+2] = new_crc >> 16;
    msg_buf[json_length+3] = new_crc >> 24;
    memcpy(msg_buf, req_json, json_length);
    return (int) json_length+4;
}


int gen_db_comm_sys_ident_json(uint8_t *message_buffer, int new_id, int new_hw_id, int new_fw_id){
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, DB_COMM_KEY_DEST, 4);
    cJSON_AddStringToObject(root, DB_COMM_KEY_TYPE, DB_COMM_TYPE_SYS_IDENT_RESPONSE);
    cJSON_AddStringToObject(root, DB_COMM_KEY_ORIGIN, DB_COMM_ORIGIN_GRND);
    cJSON_AddNumberToObject(root, DB_COMM_KEY_HARDWID, DB_COMM_SYS_HID_ESP32);
    cJSON_AddNumberToObject(root, DB_COMM_KEY_FIRMWID, new_fw_id);
    cJSON_AddNumberToObject(root, DB_COMM_KEY_ID, new_id);
    return finalize_message(message_buffer, cJSON_Print(root));
}