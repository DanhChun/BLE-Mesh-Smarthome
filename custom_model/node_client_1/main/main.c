#include "header.h"

#define TAG "MAIN"

static uint8_t ticks = 0;
model_sensor_data_t device_sensor_data;
dht11_t dht11_sensor;
float temperature = 0.0;
float humidity = 0.0;
uint8_t blink = 0;

static void Sensor_Task(void *arg)
{
    ESP_LOGI(TAG, "Sensor task initializing...");
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if(!dht11_read(&dht11_sensor, 2))
        {  
            temperature = dht11_sensor.temperature;
            humidity = dht11_sensor.humidity;
            // printf("[Temperature]> %.2f \n",dht11_sensor.temperature);
            // printf("[Humidity]> %.2f \n",dht11_sensor.humidity);
        }
        else
        {
            printf("Failed to read data from DHT11 sensor\n");
        }

        if (ticks++ >= TIME_SEND_DATA)
        {
            printf ("Temperature: %.2f \n",temperature);
            printf ("Humidity %.2f \n",humidity);

            device_sensor_data.device_num = 1;
            device_sensor_data.temperature = temperature;
            device_sensor_data.humidity = humidity;

            device_sensor_data.relay_1 = NO_DATA_RELAY;
            device_sensor_data.brightness = NO_DATA_BRIGHTNESS;
            device_sensor_data.relay_2 = NO_DATA_RELAY;
            device_sensor_data.rfid = NO_DATA_RFID;
            device_sensor_data.door = NO_DATA_DOOR;

            ble_mesh_custom_sensor_client_model_message_set(device_sensor_data);
            ticks = 0;
        }
    }
}

void app_main(void)
{
    esp_err_t err;
    dht11_sensor.dht11_pin = DHT11_PIN;
    Relay_Init();
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

    xTaskCreate(Sensor_Task, "sensor_main_task", 1024 * 2, (void *)0, 15, NULL);
}
