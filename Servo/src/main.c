#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "iot_servo.c"
#include "esp_log.h"
#include "esp_err.h"

#define GPIO_LED 5 // port pin of on - board LED

void app_main () {
//  initialize a channel
servo_init(LEDC_HIGH_SPEED_MODE, g_cfg);
iot_servo_write_angle(LEDC_HIGH_SPEED_MODE, );
}