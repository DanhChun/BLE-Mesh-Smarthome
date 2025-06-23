#ifndef _COMMON_H_
#define _COMMON_H_

#include "stdio.h"

#define SERVER_UINCAST_ADDR 0x0003
#define CLIENT_1_UNICAST_ADDR 0x0005
#define CLIENT_2_UNICAST_ADDR 0x0008
#define CLIENT_3_UNICAST_ADDR 0x000B

#define TIME_SEND_TO_WEB 125 // T = TIME_SEND_TO_WEB & 40MS

// Định nghĩa thông tin WiFi - có thể sửa ở đây
#define WIFI_SSID      "Thuy"
#define WIFI_PASSWORD  "12345678"

// Định nghĩa thông tin MQTT Broker - có thể sửa ở đây
#define MQTT_BROKER    "897e4e4bd28b411ba2464a4019281121.s1.eu.hivemq.cloud"
#define MQTT_PORT      8883
#define MQTT_USERNAME  "my_mqtt"    // Nếu có
#define MQTT_PASSWORD  "hellomqtt"    // Nếu có

// Định nghĩa các topic MQTT - có thể sửa ở đây
#define MQTT_TOPIC_PUB   "homePod/6332f122-db2a-4ad2-9792-7998524fdaf5"
    #define MQTT_TOPIC_SUB   "homePod/6332f122-db2a-4ad2-9792-7998524fdaf5/controlDevice"

#endif