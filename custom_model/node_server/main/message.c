#include "message.h"

// Hàm đóng gói dữ liệu sensor thành JSON
char* pack_sensor_data(model_sensor_data_t* data) {
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return NULL; // Xử lý trường hợp không tạo được đối tượng JSON
    }

    // Thêm các giá trị từ struct vào object JSON
    // cJSON_AddNumberToObject(root, "device_num", data->device_num);
    cJSON_AddNumberToObject(root, "temp", data->temperature);
    cJSON_AddNumberToObject(root, "humi", data->humidity);
    cJSON_AddNumberToObject(root, "light", data->brightness);
    // cJSON_AddNumberToObject(root, "relay_1", data->relay_1);
    // cJSON_AddNumberToObject(root, "relay_2", data->relay_2);
    // cJSON_AddNumberToObject(root, "rfid", data->rfid);
    // cJSON_AddNumberToObject(root, "door", data->door);

    // Chuyển JSON thành chuỗi
    char *string = cJSON_Print(root);

    // Giải phóng bộ nhớ JSON object
    cJSON_Delete(root);

    return string; // Chuỗi này cần được giải phóng sau khi sử dụng
}

// Hàm bóc tách JSON control message
// bool parse_control_message(char* json_string, model_sensor_data_t* status) {
bool parse_control_message(char* json_string, uint8_t* relay_1_return, uint8_t* relay_2_return, uint8_t* door_return) {
    cJSON *root = cJSON_Parse(json_string);
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error parsing JSON: %s\n", error_ptr);
        }
        return false;
    }

    // Đọc các giá trị từ JSON (nếu có)
    cJSON *door = cJSON_GetObjectItem(root, "door");
    cJSON *relay1 = cJSON_GetObjectItem(root, "relay_1");
    cJSON *relay2 = cJSON_GetObjectItem(root, "relay_2");

    bool parsed = false;

    if (cJSON_IsNumber(door)) {
        *door_return = (uint8_t)door->valueint;
        parsed = true;
    }
    if (cJSON_IsNumber(relay1)) {
        *relay_1_return = (uint8_t)relay1->valueint;
        parsed = true;
    }
    if (cJSON_IsNumber(relay2)) {
        *relay_2_return = (uint8_t)relay2->valueint;
        parsed = true;
    }

    cJSON_Delete(root);
    return parsed; // Trả về true nếu có ít nhất 1 trường được xử lý
}