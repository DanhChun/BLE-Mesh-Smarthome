#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "stdbool.h"

#include "header.h"

// Struct để lưu trạng thái các thiết bị
typedef struct {
    int door;
    int relay_1;
    int relay_2;
} device_status_t;

char* pack_sensor_data(model_sensor_data_t* data);
bool parse_control_message(char* json_string, uint8_t* relay_1_return, uint8_t* relay_2_return, uint8_t* door_return);


#endif // _MESSAGE_H_