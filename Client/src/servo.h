#ifndef _SERVO_H_
#define _SERVO_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "iot_servo.h"

// servo on pin 22 (jumper 6)
#define SERVO_PIN 22
#define SERVO_LED 18

// TODO: return 1 if action is completed
//  open window
// TODO: change 180° to 90°/45°
void openWindow();

// close window
void closeWindow();

// parameter

// initiate Servo
void startServo(int param);

// TODO: new task for led
void ledOn();

#endif /* _SERVO_H_ */