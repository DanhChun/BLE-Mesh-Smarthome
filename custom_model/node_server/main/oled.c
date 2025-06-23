#include "oled.h"

const char *TAG = "OLED";

#define I2C_MASTER_SCL_IO 22      /*!< GPIO number for I2C master clock */
#define I2C_MASTER_SDA_IO 21      /*!< GPIO number for I2C master data */
#define I2C_MASTER_NUM I2C_NUM_1  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */

static ssd1306_handle_t dev = NULL;

/**
 * @brief Khởi tạo I2C
 */
static void i2c_bus_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
        .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));
}

/**
 * @brief Khởi tạo SSD1306
 */
void Oled_Init(void)
{
    ESP_LOGI(TAG, "OLED initialization");
    i2c_bus_init();
    dev = ssd1306_create(I2C_MASTER_NUM, SSD1306_I2C_ADDRESS);
    if (dev == NULL) {
        ESP_LOGE(TAG, "Failed to initialize SSD1306");
        return;
    }
    ssd1306_clear_screen(dev, 0x00);
    ssd1306_refresh_gram(dev);
}

/**
 * @brief Hiển thị dữ liệu cảm biến trên màn hình SSD1306
 */
esp_err_t ssd1306_show_sensor_data(model_sensor_data_t *sensor_data)
{
    ssd1306_clear_screen(dev, 0x00);

    char buffer[32];

    snprintf(buffer, sizeof(buffer), "Temp:   %.1fC", sensor_data->temperature);
    ssd1306_draw_string(dev, 0, 0, (const uint8_t *)buffer, 16, 1);

    snprintf(buffer, sizeof(buffer), "Humi:   %.1f%%", sensor_data->humidity);
    ssd1306_draw_string(dev, 0, 20, (const uint8_t *)buffer, 16, 1);

    snprintf(buffer, sizeof(buffer), "Bright: %.1f", sensor_data->brightness);
    ssd1306_draw_string(dev, 0, 40, (const uint8_t *)buffer, 16, 1);

    return ssd1306_refresh_gram(dev);
}
esp_err_t ssd1306_show_status(model_sensor_data_t *sensor_data)
{
    ssd1306_clear_screen(dev, 0x00);

    char buffer[32];

    snprintf(buffer, sizeof(buffer), "Relay 1: %s", sensor_data->relay_1 ? "ON" : "OFF");
    ssd1306_draw_string(dev, 0, 0, (const uint8_t *)buffer, 16, 1);

    snprintf(buffer, sizeof(buffer), "Relay 2: %s", sensor_data->relay_2 ? "ON" : "OFF");
    ssd1306_draw_string(dev, 0, 20, (const uint8_t *)buffer, 16, 1);

    snprintf(buffer, sizeof(buffer), "Door:    %s", sensor_data->door ? "OPEN" : "CLOSED");
    ssd1306_draw_string(dev, 0, 40, (const uint8_t *)buffer, 16, 1);

    return ssd1306_refresh_gram(dev);
}


