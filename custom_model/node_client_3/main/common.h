    #ifndef _COMMON_H_
#define _COMMON_H_

#include "stdio.h"

#define SERVER_UINCAST_ADDR 0x0003
#define CLIENT_1_UNICAST_ADDR 0x0005
#define CLIENT_2_UNICAST_ADDR 0x0008
#define CLIENT_3_UNICAST_ADDR 0x000B

//all pins
#define SERVO_PIN 22

#define RST_PIN 2
#define MISO_PIN 19
#define MOSI_PIN 23
#define SCK_PIN 18
#define SDA_PIN 5

// chọn thẻ trong file rfid_servo.c

//servo
#define DEGREE_OPEN_DOOR 90
#define DEGREE_CLOSE_DOOR 0

#define TIME_OPEN_DOOR 475 // 5 seconds, tham số này chia cho 100 ra số giây

#define TIME_SEND_DATA 600 // seconds

#endif