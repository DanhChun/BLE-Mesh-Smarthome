#include "app_mqtt.h"

StreamBufferHandle_t xStreamBuffer;
char mqtt_receive_buffer[128];
uint8_t flag_new_message;

extern const char ca_cert_pem_start[] asm("_binary_certs_ca_cert_pem_start");
extern const char ca_cert_pem_end[] asm("_binary_certs_ca_cert_pem_end");

static const char *TAG = "MQTT_EXAMPLE";
static esp_mqtt_client_handle_t mqtt_client = NULL;
static bool mqtt_connected = false;

// Hàm xử lý sự kiện WiFi - đơn giản hơn
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                             int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "WiFi disconnected, trying to reconnect...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

// Hàm khởi tạo WiFi - đơn giản hơn
void wifi_init_sta(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Đăng ký handler cho sự kiện WiFi và IP
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                      ESP_EVENT_ANY_ID,
                                                      &wifi_event_handler,
                                                      NULL,
                                                      NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                      IP_EVENT_STA_GOT_IP,
                                                      &wifi_event_handler,
                                                      NULL,
                                                      NULL));

    // Cấu hình WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };

    // Khởi động WiFi
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished");
}

// Hàm xử lý sự kiện MQTT
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected to broker");
            mqtt_connected = true;
            // Subscribe khi kết nối thành công
            esp_mqtt_client_subscribe(mqtt_client, MQTT_TOPIC_SUB, 0);
            break;
            
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT Disconnected from broker");
            mqtt_connected = false;
            break;
            
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT Subscribed to topic: %s", MQTT_TOPIC_SUB);
            break;
            
        case MQTT_EVENT_DATA:
            // In ra màn hình bản tin nhận được
            ESP_LOGI(TAG, "MQTT Received Message:");
            ESP_LOGI(TAG, "Topic: %.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "Data: %.*s", event->data_len, event->data);
            // Copy dữ liệu vào mqtt_receive_buffer
            if (event->data_len < sizeof(mqtt_receive_buffer)) {
                strncpy(mqtt_receive_buffer, event->data, event->data_len);
                mqtt_receive_buffer[event->data_len] = '\0'; // Kết thúc chuỗi
            } else {
                ESP_LOGW(TAG, "MQTT data length exceeds buffer size. Data truncated.");
                strncpy(mqtt_receive_buffer, event->data, sizeof(mqtt_receive_buffer) - 1);
                mqtt_receive_buffer[sizeof(mqtt_receive_buffer) - 1] = '\0'; // Kết thúc chuỗi
            }
            flag_new_message = 1;
            // xStreamBufferSend(xStreamBuffer, event->data, event->data_len, pdMS_TO_TICKS(100));
            break;
            
        default:
            break;
    }
}

// Hàm khởi tạo MQTT
void mqtt_init(void)
{
    xStreamBuffer = xStreamBufferCreate(STREAM_BUFFER_SIZE, 1);
    wifi_init_sta();
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtts://" MQTT_BROKER,
        .broker.address.port = MQTT_PORT,
        .credentials.username = MQTT_USERNAME,
        .credentials.authentication.password = MQTT_PASSWORD,
        .broker.verification.crt_bundle_attach = esp_crt_bundle_attach,
        // .broker.verification.certificate = {
        //     .cert_data = ca_cert_pem_start, 
        //     .cert_len = ca_cert_pem_end - ca_cert_pem_start,
        // },
    };
    
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}

// Hàm gửi bản tin MQTT

//example:
    // snprintf(publish_buffer, sizeof(publish_buffer), "Hello from ESP32! Count: %d", counter++);
    // mqtt_publish_message(publish_buffer);
bool mqtt_publish_message(const char *message)
{
    if (!mqtt_connected || !mqtt_client) {
        return false;
    }
    
    int msg_id = esp_mqtt_client_publish(mqtt_client, MQTT_TOPIC_PUB, message, 0, 1, 0);
    if (msg_id != -1) {
        ESP_LOGI(TAG, "Published message: %s", message);
        return true;
    }
    return false;
}

void mqtt_loop(void)
{
    if (!mqtt_connected) {
        ESP_LOGI(TAG, "MQTT disconnected, attempting to reconnect...");
        esp_mqtt_client_reconnect(mqtt_client);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Đợi 5 giây trước khi thử lại
        // continue;
    }
}