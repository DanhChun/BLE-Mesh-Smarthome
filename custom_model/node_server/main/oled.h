#ifndef _OLED_H_
#define _OLED_H_

#include "header.h"

esp_err_t ssd1306_show_sensor_data(model_sensor_data_t *sensor_data);
esp_err_t ssd1306_show_status(model_sensor_data_t *sensor_data);
void Oled_Init(void);

#endif  // _OLED_H_