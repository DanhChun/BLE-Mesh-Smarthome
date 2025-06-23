/**
 * @file mesh_server.h
 * 
 * @brief
 * 
 * @author
 * 
 * @date  11/2020
 */

#ifndef __MESH_SERVER_H__
#define __MESH_SERVER_H__

#include "header.h"

extern QueueHandle_t ble_mesh_received_data_queue;

/**
 * @brief Initializes BLE Mesh stack, initializing Models and it's callback functions
 * 
 */
esp_err_t ble_mesh_device_init_server(void);
esp_err_t ble_mesh_custom_sensor_server_model_message_status(model_sensor_data_t stt_data);



#endif  // __MESH_SERVER_H__