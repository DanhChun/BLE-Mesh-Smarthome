#ifndef _HEADER_H_
#define _HEADER_H_

#include <sdkconfig.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "mesh_device_app.h"
#include "esp_ble_mesh_defs.h"
#include <stdio.h>
#include <string.h>
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "freertos/queue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#include "common.h"
#include "custom_sensor_model_defs.h"
#include "bh1750.h"
#include "mesh_client.h"
#include "mesh_server.h"
#include "relay.h"


#define TIME_SEND_DATA 5 // seconds

#define NO_DATA_TEMPERATURE 0xFFFFFFFF
#define NO_DATA_HUMIDITY 0xFFFFFFFF
#define NO_DATA_BRIGHTNESS 0xFFFFFFFF
#define NO_DATA_RFID 0xFF
#define NO_DATA_DOOR 0xFF
#define NO_DATA_RELAY 0xFF


#endif  // _HEADER_H_