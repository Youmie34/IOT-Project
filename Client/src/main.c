#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "iot_servo.h"
#include "servo.h"

void app_main()
{
   startServo(1);
}
