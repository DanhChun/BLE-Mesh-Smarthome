#include "relay.h"

uint8_t stt_relay;

void Relay_Init(void)
{
    gpio_reset_pin(RELAY_PIN);
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(RELAY_PIN, RELAY_OFF);
    stt_relay = RELAY_OFF;
}

void Relay_Control(uint8_t state)
{
    stt_relay = state;
    if (state == 1)
    {
        printf("Relay ON\n");
        gpio_set_level(RELAY_PIN, RELAY_ON);
    }
    else if (state == 0)
    {
        printf("Relay OFF\n");
        gpio_set_level(RELAY_PIN, RELAY_OFF);
    }
}