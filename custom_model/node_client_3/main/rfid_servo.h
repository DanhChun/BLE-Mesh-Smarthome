#ifndef _RFID_SERVO_H_
#define _RFID_SERVO_H_

#include "header.h"

void Rfid_Servo_Init(void);
void Servo_Write(uint8_t degree);
void Servo_Open(void);
void Servo_Close(void);
void Servo_Close_Auto(void);
uint8_t Check_Owner(void);
void Reset_Owner(void);

#endif