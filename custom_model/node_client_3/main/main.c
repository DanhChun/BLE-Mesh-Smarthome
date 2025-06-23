#include "header.h"
#include "esp_task_wdt.h"

#define TAG "MAIN"

static uint8_t ticks = 0;
model_sensor_data_t device_sensor_data;
uint8_t stt_old = 0;
uint8_t stt_new = 0;
uint16_t count_no_new_update = 0;

static void Rfid_Servo_Task(void *arg)
{
    ESP_LOGI(TAG, "task initializing...");
    while (1)
    {
        
        vTaskDelay(10 / portTICK_PERIOD_MS);

        // Servo_Close_Auto();

        stt_old = stt_new;
        stt_new = Check_Owner();

        if (stt_old == 0 && stt_new == 1)
        {
            device_sensor_data.device_num = 3;
            device_sensor_data.rfid = stt_new;
            device_sensor_data.door = stt_new;

            device_sensor_data.temperature = NO_DATA_TEMPERATURE;
            device_sensor_data.humidity = NO_DATA_HUMIDITY;
            device_sensor_data.relay_1 = NO_DATA_RELAY;
            device_sensor_data.brightness = NO_DATA_BRIGHTNESS;
            device_sensor_data.relay_2 = NO_DATA_RELAY;

            ble_mesh_custom_sensor_client_model_message_set(device_sensor_data);
            count_no_new_update = 0;
        }
        if (stt_old == 1 && stt_new == 0)
        {
            device_sensor_data.device_num = 3;
            device_sensor_data.rfid = stt_new;
            device_sensor_data.door = stt_new;

            device_sensor_data.temperature = NO_DATA_TEMPERATURE;
            device_sensor_data.humidity = NO_DATA_HUMIDITY;
            device_sensor_data.relay_1 = NO_DATA_RELAY;
            device_sensor_data.brightness = NO_DATA_BRIGHTNESS;
            device_sensor_data.relay_2 = NO_DATA_RELAY;

            ble_mesh_custom_sensor_client_model_message_set(device_sensor_data);
            count_no_new_update = 0;
        }
        count_no_new_update++;
        if (count_no_new_update >= 1000)
        {
            device_sensor_data.device_num = 3;
            device_sensor_data.rfid = stt_new;
            device_sensor_data.door = stt_new;

            device_sensor_data.temperature = NO_DATA_TEMPERATURE;
            device_sensor_data.humidity = NO_DATA_HUMIDITY;
            device_sensor_data.relay_1 = NO_DATA_RELAY;
            device_sensor_data.brightness = NO_DATA_BRIGHTNESS;
            device_sensor_data.relay_2 = NO_DATA_RELAY;

            ble_mesh_custom_sensor_client_model_message_set(device_sensor_data);
            count_no_new_update = 0;
        }

    }
}

void app_main(void)
{
    esp_err_t err;
    Rfid_Servo_Init();
    ESP_LOGI(TAG, "Initializing...");

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = ble_mesh_device_init();
    if (err)
    {
        ESP_LOGE(TAG, "Bluetooth mesh init failed (err %d)", err);
    }

    xTaskCreate(Rfid_Servo_Task, "rfid_servo_task", 4096, (void *)0, 15, NULL);
}
