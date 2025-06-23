
#include "rfid_servo.h"

//chọn 1 trong 2 thẻ dưới đây để làm thẻ chủ nhà
uint8_t owner_id[5] = {0x61, 0xC6, 0x25, 0x10, 0x92}; // thẻ trắng
// uint8_t owner_id[5] = {0xED, 0x88, 0xDA, 0x2D, 0x92}; // thẻ xanh

#define SERVO_MIN_PULSEWIDTH_US 600  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2400  // Maximum pulse width in microsecond
#define SERVO_PULSE_GPIO             SERVO_PIN        // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms
mcpwm_cmpr_handle_t comparator = NULL;

uint16_t count_open_door = 0;
uint8_t flag_owner = 0;

static const char* TAG = "RFID_SERVO";

void tag_handler(uint8_t* sn) { // serial number is always 5 bytes long
    ESP_LOGI(TAG, "Tag: %#x %#x %#x %#x %#x",
        sn[0], sn[1], sn[2], sn[3], sn[4]
    );
    if (   sn[0] == owner_id[0] 
        && sn[1] == owner_id[1]
        && sn[2] == owner_id[2]
        && sn[3] == owner_id[3]
        && sn[4] == owner_id[4]) {
        ESP_LOGI(TAG, "Owner tag detected");
        flag_owner = 1;
        Servo_Open();
    }
    else {
        ESP_LOGI(TAG, "Unknown tag detected");
    }
}

void Rfid_Servo_Init(void)
{
    const rc522_start_args_t start_args = {
        .miso_io  = MISO_PIN,
        .mosi_io  = MOSI_PIN,
        .sck_io   = SCK_PIN,
        .sda_io   = SDA_PIN,
        .callback = &tag_handler,

        // Uncomment next line for attaching RC522 to SPI2 bus. Default is VSPI_HOST (SPI3)
        //.spi_host_id = HSPI_HOST
    };
    rc522_start(start_args);

    
    ESP_LOGI(TAG, "Create timer and operator");
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");
    // mcpwm_cmpr_handle_t comparator = NULL;
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = SERVO_PULSE_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

    // set the initial compare value, so that the servo will spin to the center position
    // ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
}

void Servo_Write(uint8_t angle) {
    mcpwm_comparator_set_compare_value(comparator, SERVO_MIN_PULSEWIDTH_US + (angle * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / 180));
}

void Servo_Open(void) {
    count_open_door = 1;
    Servo_Write(DEGREE_OPEN_DOOR);
}

void Servo_Close(void) {
    Servo_Write(DEGREE_CLOSE_DOOR);
}

void Servo_Close_Auto(void) {
    if (count_open_door > 0 && count_open_door < TIME_OPEN_DOOR) {
        count_open_door++;
    }
    else if (count_open_door >= TIME_OPEN_DOOR) {
        Servo_Close();
        Reset_Owner();
        count_open_door = 0;
    }
}

uint8_t Check_Owner()
{
    return flag_owner;
}

void Reset_Owner()
{
    flag_owner = 0;
}