#ifndef _APP_MQTT_H_
#define _APP_MQTT_H_

#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"

#include "esp_crt_bundle.h"

#include "header.h"

// void wifi_init_sta(void);
void mqtt_init(void);
bool mqtt_publish_message(const char *message);
void mqtt_loop(void);

extern StreamBufferHandle_t xStreamBuffer;
extern char mqtt_receive_buffer[128];
extern uint8_t flag_new_message;

#endif