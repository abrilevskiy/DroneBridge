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

#ifndef DB_ESP32_GLOBALS_H
#define DB_ESP32_GLOBALS_H

#include <freertos/event_groups.h>

#define MAX_LTM_FRAMES_IN_BUFFER 5
#define BUILDVERSION 11    //v0.11

// can be set by user
extern uint8_t DEFAULT_PWD[64];
extern uint8_t DEFAULT_CHANNEL;
extern char DEST_IP[15];
extern uint8_t SERIAL_PROTOCOL;  // 1,2=MSP, 3,4,5=MAVLink/transparent
extern uint8_t DB_UART_PIN_TX;
extern uint8_t DB_UART_PIN_RX;
extern uint32_t DB_UART_BAUD_RATE;
extern uint16_t TRANSPARENT_BUF_SIZE;
extern uint8_t LTM_FRAME_NUM_BUFFER;    // Number of LTM frames per UDP packet (min = 1; max = 5)
extern EventGroupHandle_t wifi_event_group;

// 0 = to separate ports (MSP to proxy_port: 1607; LTM to telemetry_port: 1604); 1=all data to proxy_port/request origin
extern uint8_t MSP_LTM_TO_SAME_PORT;

// system internally
extern volatile bool client_connected;

#endif //DB_ESP32_GLOBALS_H
