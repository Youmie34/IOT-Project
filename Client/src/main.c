#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_check.h"
#include "esp_log.h"
#include "server.h"
#include "servo.h"
#include "esp_event.h"
#include <string.h>

void app_main()
{
    gpio_set_direction(SERVO_LED, GPIO_MODE_OUTPUT); // set SERVO_LED as output
    gpio_set_level(SERVO_LED, 0);                    // set GPIO low , LED goes off
    wifi_init();
    start_webserver();
}