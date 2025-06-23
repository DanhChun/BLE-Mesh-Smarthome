#include "header.h"

static const char *TAG = "MAIN";

SemaphoreHandle_t data_mutex = NULL; // Khởi tạo mutex

QueueHandle_t ble_mesh_received_data_queue = NULL;
uint8_t blink = 0;

model_sensor_data_t common_data = {
    .device_num = 0,
    .temperature = NO_DATA_TEMPERATURE,
    .humidity = NO_DATA_HUMIDITY,
    .relay_1 = 0,
    .brightness = NO_DATA_BRIGHTNESS,
    .relay_2 = 0,
    .rfid = NO_DATA_RFID,
    .door = 0};

static void Display_Task(void *pvParameters)
{
    // model_sensor_data_t local_data;
    bool show_sensor = true; // Cờ để chuyển đổi loại hiển thị
    uint32_t switch_display_counter = 0;

    while (1)
    {
        model_sensor_data_t local_data;
        // Lấy mutex và sao chép dữ liệu vào biến cục bộ
        if (xSemaphoreTake(data_mutex, pdMS_TO_TICKS(100)) == pdTRUE)
        {
            local_data = common_data;   // Sao chép dữ liệu
            xSemaphoreGive(data_mutex); // Giải phóng mutex
        }
        else
        {
            ESP_LOGW(TAG, "Failed to acquire mutex for display update");
            vTaskDelay(pdMS_TO_TICKS(100)); // Đợi và thử lại
            continue;
        }

        // Cập nhật màn hình bằng dữ liệu cục bộ
        esp_err_t ret;
        if (show_sensor)
        {
            ret = ssd1306_show_sensor_data(&local_data);
        }
        else
        {
            ret = ssd1306_show_status(&local_data);
        }

        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to update OLED");
        }

        // Chuyển đổi loại hiển thị sau 5 giây
        switch_display_counter++;
        if (switch_display_counter >= 10)
        { // 10 lần * 500ms = 5 giây
            show_sensor = !show_sensor;
            switch_display_counter = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(200)); // Cập nhật màn hình 500ms/lần
    }
}

static void read_received_items(void *arg)
{
    ESP_LOGI(TAG, "Task initializing..");

    model_sensor_data_t _received_data;

    while (1)
    {
        // vTaskDelay(500 / portTICK_PERIOD_MS);

        if (xQueueReceive(ble_mesh_received_data_queue, &_received_data, 1000 / portTICK_PERIOD_MS) == pdPASS)
        {
            if (xSemaphoreTake(data_mutex, pdMS_TO_TICKS(100)) == pdTRUE)
            {
                // Cập nhật `common_data`
                if (_received_data.device_num == 1)
                {
                    common_data.temperature = _received_data.temperature;
                    common_data.humidity = _received_data.humidity;
                }
                else if (_received_data.device_num == 2)
                {
                    common_data.brightness = _received_data.brightness;
                }
                else if (_received_data.device_num == 3)
                {
                    common_data.rfid = _received_data.rfid;
                    common_data.door = _received_data.door;
                }
                xSemaphoreGive(data_mutex);
            }
            else
            {
                ESP_LOGW(TAG, "Failed to acquire mutex for updating common_data");
            }
        }
    }
}

static void Sync_Data(void *arg)
{
    ESP_LOGI(TAG, "Task initializing..");
    uint8_t sync_count = 0;
    // model_sensor_data_t sync_data;
    while (1)
    {
        // vTaskDelay(4000 / portTICK_PERIOD_MS);
        // model_sensor_data_t sync_data;
        sync_count++;
        vTaskDelay(pdMS_TO_TICKS(50));
        if (flag_new_message == 1)
        {
            uint8_t relay_1_temp, relay_2_temp, door_temp;
            if (parse_control_message(mqtt_receive_buffer, &relay_1_temp, &relay_2_temp, &door_temp))
            {
                if (xSemaphoreTake(data_mutex, pdMS_TO_TICKS(100)) == pdTRUE)
                {
                    common_data.relay_1 = relay_1_temp;    
                    common_data.relay_2 = relay_2_temp;
                    common_data.door = door_temp;
                    sync_count = 0xFF; // Gửi ngay lập tức vào mạng mesh
                    xSemaphoreGive(data_mutex);
                }
                else
                {
                    ESP_LOGW(TAG, "Failed to acquire mutex for mqtt receive");
                }
            }
            flag_new_message = 0;
        }

        // đếm 8s gửi bản tin trong mạng mesh 1 lần
        if (sync_count >= 200)
        {
            sync_count = 0;
            if (xSemaphoreTake(data_mutex, pdMS_TO_TICKS(500)) == pdTRUE)
            {
                // printf common struct data
                printf("Device Number: %d\n", common_data.device_num);
                printf("Temperature: %f\n", common_data.temperature);
                printf("Humidity: %f\n", common_data.humidity);
                printf("Relay 1: %d\n", common_data.relay_1);
                printf("Brightness: %f\n", common_data.brightness);
                printf("Relay 2: %d\n", common_data.relay_2);
                printf("RFID: %d\n", common_data.rfid);
                printf("Door: %d\n", common_data.door);

                ble_mesh_custom_sensor_server_model_message_status(common_data);
                xSemaphoreGive(data_mutex);
            }
            else
            {
                ESP_LOGW(TAG, "Failed to acquire mutex for syncing data");
            }
        }
    }
}

static void Mqtt_Task(void *pvParameters)
{
    uint8_t mqtt_count_send = 0;
    // char publish_buffer[256];
    model_sensor_data_t mqtt_send_data;
    while (1)
    {
        mqtt_count_send++;
        mqtt_loop();
        vTaskDelay(pdMS_TO_TICKS(40));

        if (mqtt_count_send >= TIME_SEND_TO_WEB)  // 25 * 40ms = 1s
        {
            mqtt_count_send = 0;
            // Lấy mutex và sao chép dữ liệu vào biến cục bộ
            if (xSemaphoreTake(data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
            {
                mqtt_send_data = common_data;   // Sao chép dữ liệu
                xSemaphoreGive(data_mutex); // Giải phóng mutex
            }
            else
            {
                ESP_LOGW(TAG, "Failed to acquire mutex for mqtt update");
                vTaskDelay(pdMS_TO_TICKS(50)); // Đợi và thử lại
                continue;
            }

            char* mqtt_json_str = pack_sensor_data(&mqtt_send_data);
            if (mqtt_json_str != NULL)
            {
                mqtt_publish_message(mqtt_json_str);
                free(mqtt_json_str);
            }
        }
    }
}

void app_main(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing...");

    ble_mesh_received_data_queue = xQueueCreate(5, sizeof(model_sensor_data_t));

    // Khởi tạo mutex
    data_mutex = xSemaphoreCreateMutex();
    if (data_mutex == NULL)
    {
        ESP_LOGE(TAG, "Failed to create mutex");
        return;
    }

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = ble_mesh_device_init();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    if (err)
    {
        ESP_LOGE(TAG, "Bluetooth mesh init failed (err 0x%06x)", err);
    }
    Oled_Init();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    mqtt_init();

    xTaskCreate(read_received_items, "Read queue task", 4096, (void *)0, 19, NULL);
    xTaskCreate(Sync_Data, "sync_data_all_node", 7000, (void *)0, 20, NULL);
    xTaskCreate(&Display_Task, "Display_Task", 4096, NULL, 5, NULL);
    xTaskCreate(&Mqtt_Task, "mqtt_task", 7000, NULL, 10, NULL);
}