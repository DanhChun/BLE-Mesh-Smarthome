#ifndef __MESH_SERVER_H__
#define __MESH_SERVER_H__

#include "header.h"

extern QueueHandle_t ble_mesh_received_data_queue;

esp_err_t ble_mesh_device_init_server(void);

#endif  // __MESH_SERVER_H__